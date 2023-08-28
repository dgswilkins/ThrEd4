// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "formClip.h"
#include "globals.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"

#pragma pack(push, 1)
class FORM_CLIP // form data clipboard header
{
  public:
  uint32_t clipType {};
  uint32_t reserved {};
  FRM_HEAD form {};

  // constexpr FORM_CLIP() noexcept = default;
  // FORM_CLIP(FORM_CLIP const&) = default;
  // FORM_CLIP(FORM_CLIP&&) = default;
  // FORM_CLIP& operator=(FORM_CLIP const& rhs) = default;
  // FORM_CLIP& operator=(FORM_CLIP&&) = default;
  //~FORM_CLIP() = default;
};
#pragma pack(pop)

#pragma pack(push, 1)
class FORMS_CLIP // multiple forms clipboard header
{
  public:
  uint32_t clipType {};
  uint16_t formCount {};
  uint16_t reserved {};

  // constexpr FORMS_CLIP() noexcept = default;
  // FORMS_CLIP(FORMS_CLIP const&) = default;
  // FORMS_CLIP(FORMS_CLIP&&) = default;
  // FORMS_CLIP& operator=(FORMS_CLIP const& rhs) = default;
  // FORMS_CLIP& operator=(FORMS_CLIP&&) = default;
  //~FORMS_CLIP() = default;
};
#pragma pack(pop)

#pragma pack(push, 1)
class FORM_VERTEX_CLIP // form points clipboard header
{
  public:
  uint32_t clipType {};
  uint32_t vertexCount {};
  bool     direction {};

  // constexpr FORM_VERTEX_CLIP() noexcept = default;
  // FORM_VERTEX_CLIP(FORM_VERTEX_CLIP const&) = default;
  // FORM_VERTEX_CLIP(FORM_VERTEX_CLIP&&) = default;
  // FORM_VERTEX_CLIP& operator=(FORM_VERTEX_CLIP const& rhs) = default;
  // FORM_VERTEX_CLIP& operator=(FORM_VERTEX_CLIP&&) = default;
  //~FORM_VERTEX_CLIP() = default;
};
#pragma pack(pop)

namespace fci {
void dupclp() noexcept(std::is_same_v<size_t, uint32_t>);
auto frmcnt(uint32_t iForm, uint32_t& formFirstStitchIndex) noexcept -> uint32_t;
void setpclp();
void unpclp();
} // namespace fci

// main variables
namespace {
auto ClipFormsCount = uint32_t {}; // number of forms the on the clipboard
auto ClipOrigin     = POINT {};    // origin of clipboard box in stitch coordinates
auto FormVerticesAsLine =
    gsl::narrow_cast<std::vector<POINT>*>(nullptr); // form vertex clipboard paste into form line
} // namespace

auto tfc::getClipFormCount() noexcept -> uint32_t {
  return ClipFormsCount;
}

void tfc::setFVAS(std::vector<POINT>* source) noexcept {
  FormVerticesAsLine = source;
}

void tfc::setClipOrigin(POINT source) noexcept {
  ClipOrigin = source;
}

auto tfc::getClipForm(LPVOID clipMemory) noexcept -> FRM_HEAD* {
  if (clipMemory == nullptr) {
	return nullptr;
  }
  auto* clipFormHeader = gsl::narrow_cast<FORM_CLIP*>(clipMemory);
  if (clipFormHeader->clipType == CLP_FRM) {
	auto* clipForm = &clipFormHeader->form;
	return clipForm;
  }
  return nullptr;
}

auto tfc::sizfclp(FRM_HEAD const& form) noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t {
  auto clipSize = wrap::toUnsigned(sizeof(FORM_CLIP)) + form.vertexCount * wrap::sizeofType(FormVertices);
  if (form.type == SAT) {
	clipSize += form.satinGuideCount * wrap::sizeofType(SatinGuides);
  }
  if (form.isEdgeClip()) {
	clipSize += form.clipEntries * wrap::sizeofType(ClipPoints);
  }
  if (form.isClipX()) {
	clipSize += form.lengthOrCount.clipCount * wrap::sizeofType(ClipPoints);
  }
  if (form.isTexture()) {
	clipSize += form.fillInfo.texture.count * wrap::sizeofType(TexturePointsBuffer);
  }
  return clipSize;
}

void tfc::sizclp(FRM_HEAD const& form,
                 uint32_t&       formFirstStitchIndex,
                 uint32_t&       formStitchCount,
                 uint32_t&       length,
                 uint32_t&       fileSize) noexcept(std::is_same_v<size_t, uint32_t>) {
  fileSize = wrap::toUnsigned(sizeof(FORM_CLIP)) + form.vertexCount * wrap::sizeofType(FormVertices);
  length = fileSize;
  if (form.type == SAT) {
	fileSize += form.satinGuideCount * wrap::sizeofType(SatinGuides);
  }
  if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	formStitchCount = fci::frmcnt(ClosestFormToCursor, formFirstStitchIndex);
	length += formStitchCount;
	fileSize += length * wrap::sizeofType(StitchBuffer);
  }
  if (form.isEdgeClip()) {
	fileSize += form.clipEntries * wrap::sizeofType(ClipPoints);
  }
  if (form.isClipX()) {
	fileSize += form.lengthOrCount.clipCount * wrap::sizeofType(ClipPoints);
  }
  if (form.isTexture()) {
	fileSize += form.fillInfo.texture.count * wrap::sizeofType(TexturePointsBuffer);
  }
}

void tfc::clipSelectedForm() {
  auto        firstStitch = 0U; // points to the first stitch in a form
  auto        stitchCount = 0U;
  auto        length      = 0U;
  auto        clipSize    = 0U;
  auto const& form        = FormList->operator[](ClosestFormToCursor);
  sizclp(form, firstStitch, stitchCount, length, clipSize);
  clipSize += sizeof(FORM_CLIP);
  // NOLINTNEXTLINE(hicpp-signed-bitwise, readability-qualified-auto)
  auto clipHandle = GlobalAlloc(GHND, clipSize);
  if (clipHandle == nullptr) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  EmptyClipboard();
  auto const thrEdClip       = RegisterClipboardFormat(ThrEdClipFormat);
  auto*      clipFormHeader  = gsl::narrow_cast<FORM_CLIP*>(GlobalLock(clipHandle));
  clipFormHeader->clipType   = CLP_FRM;
  clipFormHeader->form       = form;
  auto*      ptrFormVertices = convertFromPtr<F_POINT*>(std::next(clipFormHeader));
  auto const startVertex     = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  auto const endVertex       = wrap::next(startVertex, form.vertexCount);

  auto const vertices = gsl::span<F_POINT> {ptrFormVertices, form.vertexCount};
  std::copy(startVertex, endVertex, vertices.begin());
  auto* ptrGuides = convertFromPtr<SAT_CON*>(wrap::next(ptrFormVertices, form.vertexCount));
  auto  iGuide    = 0U;
  if (form.type == SAT) {
	iGuide                = form.satinGuideCount;
	auto const startGuide = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	auto const endGuide   = wrap::next(startGuide, iGuide);

	auto const guides = gsl::span<SAT_CON> {ptrGuides, iGuide};
	std::copy(startGuide, endGuide, guides.begin());
  }
  auto* ptrMclp = convertFromPtr<F_POINT*>(wrap::next(ptrGuides, iGuide));
  auto  iClip   = 0U;
  if (form.isClipX()) {
	iClip                = form.lengthOrCount.clipCount;
	auto const startMclp = wrap::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
	auto const endMclp   = wrap::next(startMclp, iClip);

	auto const mclps = gsl::span<F_POINT> {ptrMclp, iClip};
	std::copy(startMclp, endMclp, mclps.begin());
  }
  auto* ptrPoints = convertFromPtr<F_POINT*>(wrap::next(ptrMclp, iClip));
  iClip           = 0U;
  if (form.isEdgeClipX()) {
	iClip                = form.clipEntries;
	auto const startClip = wrap::next(ClipPoints->cbegin(), form.borderClipData);
	auto const endClip   = wrap::next(startClip, iClip);

	auto const points = gsl::span<F_POINT> {ptrPoints, iClip};
	std::copy(startClip, endClip, points.begin());
  }
  auto* textures = convertFromPtr<TX_PNT*>(wrap::next(ptrPoints, iClip));
  if (form.isTexture()) {
	auto const startTexture = wrap::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
	auto const endTexture = wrap::next(startTexture, form.fillInfo.texture.count);

	auto const spDest = gsl::span<TX_PNT> {textures, form.fillInfo.texture.count};
	std::copy(startTexture, endTexture, spDest.begin());
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(thrEdClip, clipHandle);
  StateMap->reset(StateFlag::WASPCDCLP);
  if (((form.fillType == 0U) && (form.edgeType == 0U))) {
	CloseClipboard();
	return;
  }
  Clip = RegisterClipboardFormat(PcdClipFormat);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  clipHandle = GlobalAlloc(GHND, stitchCount * sizeof(CLIP_STITCH) + 2U);
  if (clipHandle == nullptr) {
	CloseClipboard();
	return;
  }
  auto*      clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
  auto const spData         = gsl::span<CLIP_STITCH> {clipStitchData, stitchCount};
  auto       iTexture       = firstStitch;
  tfc::savclp(spData[0], StitchBuffer->operator[](iTexture), length);
  ++iTexture;
  auto       iDestination   = 1U;
  auto const codedAttribute = ClosestFormToCursor << FRMSHFT;
  while (iTexture < StitchBuffer->size()) {
	if ((StitchBuffer->operator[](iTexture).attribute & FRMSK) == codedAttribute &&
	    ((StitchBuffer->operator[](iTexture).attribute & NOTFRM) == 0U)) {
	  tfc::savclp(spData[iDestination++],
	              StitchBuffer->operator[](iTexture),
	              (StitchBuffer->operator[](iTexture).attribute & COLMSK));
	}
	++iTexture;
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(Clip, clipHandle);
  StateMap->set(StateFlag::WASPCDCLP);
  CloseClipboard();
}

void tfc::clipSelectedForms() {
  auto length = 0U;
  for (auto& selectedForm : (*SelectedFormList)) {
	auto& currentForm = FormList->operator[](selectedForm);
	length += tfc::sizfclp(currentForm);
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise, readability-qualified-auto)
  auto clipHandle = GlobalAlloc(GHND, length + sizeof(FORMS_CLIP));
  if (clipHandle == nullptr) { // exit gracefully without altering the clipboard
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  EmptyClipboard();
  auto const thrEdClip       = RegisterClipboardFormat(ThrEdClipFormat);
  auto*      clipFormsHeader = gsl::narrow_cast<FORMS_CLIP*>(GlobalLock(clipHandle));
  clipFormsHeader->clipType  = CLP_FRMS;
  wrap::narrow(clipFormsHeader->formCount, SelectedFormList->size());
  // Skip past the header
  auto*      ptrForms = convertFromPtr<FRM_HEAD*>(std::next(clipFormsHeader));
  auto const forms    = gsl::span<FRM_HEAD> {ptrForms, SelectedFormList->size()};
  auto       iForm    = 0U;
  for (auto& selectedForm : (*SelectedFormList)) {
	auto& currentForm = FormList->operator[](selectedForm);
	forms[iForm++]    = currentForm;
  }
  // skip past the forms
  auto* ptrFormVertices = convertFromPtr<F_POINT*>(wrap::next(ptrForms, iForm));
  auto  verticesSize    = 0U;
  for (auto& selectedForm : (*SelectedFormList)) {
	verticesSize += FormList->operator[](selectedForm).vertexCount;
  }
  auto const formVertices = gsl::span<F_POINT> {ptrFormVertices, verticesSize};
  auto       iVertex      = 0U;
  for (auto& selectedForm : (*SelectedFormList)) {
	auto& form = FormList->operator[](selectedForm);

	auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	for (auto iSide = 0U; iSide < form.vertexCount; ++iSide) {
	  formVertices[iVertex++] = *itVertex;
	  ++itVertex;
	}
  }
  // skip past the vertex list
  auto* ptrGuides  = convertFromPtr<SAT_CON*>(wrap::next(ptrFormVertices, iVertex));
  auto  guidesSize = 0U;
  for (auto& selectedForm : (*SelectedFormList)) {
	auto& form = FormList->operator[](selectedForm);
	if (form.type == SAT) {
	  guidesSize += form.satinGuideCount;
	}
  }
  auto guideCount = 0U;
  if (guidesSize != 0U) {
	auto const guides = gsl::span<SAT_CON> {ptrGuides, guidesSize};
	for (auto& selectedForm : (*SelectedFormList)) {
	  auto& form = FormList->operator[](selectedForm);
	  if (form.type != SAT) {
		continue;
	  }
	  auto itGuide = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		guides[guideCount++] = *itGuide;
		++itGuide;
	  }
	}
  }
  // skip past the guides
  auto* ptrPoints  = convertFromPtr<F_POINT*>(wrap::next(ptrGuides, guideCount));
  auto  pointsSize = 0U;
  for (auto& selectedForm : (*SelectedFormList)) {
	auto& form = FormList->operator[](selectedForm);
	if (form.isClipX()) {
	  pointsSize += form.lengthOrCount.clipCount;
	}
	if (form.isEdgeClip()) {
	  pointsSize += form.clipEntries;
	}
  }
  auto pointCount = 0;
  if (pointsSize != 0U) {
	auto const points = gsl::span<F_POINT> {ptrPoints, pointsSize};
	for (auto& selectedForm : (*SelectedFormList)) {
	  auto& form = FormList->operator[](selectedForm);
	  if (form.isClipX()) {
		auto offsetStart = wrap::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
		for (auto iClip = 0U; iClip < form.lengthOrCount.clipCount; ++iClip) {
		  points[pointCount++] = *offsetStart;
		  ++offsetStart;
		}
	  }
	  if (!form.isEdgeClip()) {
		continue;
	  }
	  auto offsetStart = wrap::next(ClipPoints->cbegin(), form.borderClipData);
	  for (auto iClip = 0U; iClip < form.clipEntries; ++iClip) {
		points[pointCount++] = *offsetStart;
		++offsetStart;
	  }
	}
  }
  // Skip past the points
  auto* textures     = convertFromPtr<TX_PNT*>(std::next(ptrPoints, pointCount));
  auto  textureCount = uint16_t {};
  iForm              = 0;
  for (auto& selectedForm : (*SelectedFormList)) {
	auto& form = FormList->operator[](selectedForm);
	if (!form.isTexture()) {
	  continue;
	}
	auto startPoint = wrap::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
	auto endPoint   = wrap::next(startPoint, form.fillInfo.texture.count);
	auto const spDest = gsl::span<TX_PNT> {std::next(textures, textureCount), form.fillInfo.texture.count};
	std::copy(startPoint, endPoint, spDest.begin());
	forms[iForm++].fillInfo.texture.index = textureCount;
	textureCount += form.fillInfo.texture.count;
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(thrEdClip, clipHandle);

  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto& selectedForm : (*SelectedFormList)) {
	formMap.set(selectedForm);
  }
  auto astch = std::vector<F_POINT_ATTR> {};
  // Reserve a reasonable amount space, probably not enough though
  constexpr auto RESRATIO = 16U; // reserve space factor
  astch.reserve(StitchBuffer->size() / RESRATIO);
  LowerLeftStitch.x = LowerLeftStitch.y = BIGFLOAT;
  for (auto& stitch : *StitchBuffer) {
	if (((stitch.attribute & NOTFRM) != 0U) || !formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
	  continue;
	}
	if (stitch.x < LowerLeftStitch.x) {
	  LowerLeftStitch.x = stitch.x;
	}
	if (stitch.y < LowerLeftStitch.y) {
	  LowerLeftStitch.y = stitch.y;
	}
	astch.push_back(stitch);
  }
  auto const stitchCount = wrap::toUnsigned(astch.size());
  if (StitchBuffer->empty() || (stitchCount == 0)) {
	CloseClipboard();
	return;
  }
  Clip = RegisterClipboardFormat(PcdClipFormat);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  clipHandle = GlobalAlloc(GHND, stitchCount * sizeof(CLIP_STITCH) + 2U);
  if (clipHandle == nullptr) {
	CloseClipboard();
	return;
  }
  auto*      clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
  auto const spData         = gsl::span<CLIP_STITCH> {clipStitchData, stitchCount};
  auto       iStitch        = 0U;
  auto       iDestination   = 0U;
  tfc::savclp(spData[0], astch[0], stitchCount);
  ++iStitch;
  ++iDestination;
  while (iStitch < stitchCount) {
	tfc::savclp(spData[iDestination], astch[iStitch], astch[iStitch].attribute & COLMSK);
	++iStitch;
	++iDestination;
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(Clip, clipHandle);
  CloseClipboard();
}

void tfc::clipSelectedPoints() {
  // NOLINTNEXTLINE(hicpp-signed-bitwise, readability-qualified-auto)
  auto const clipHandle = GlobalAlloc(
      GHND, (wrap::toSize(SelectedFormVertices.vertexCount) + 1U) * sizeof(F_POINT) + sizeof(FORM_VERTEX_CLIP));
  if (clipHandle == nullptr) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  EmptyClipboard();
  auto const thrEdClip    = RegisterClipboardFormat(ThrEdClipFormat);
  auto*      clipHeader   = gsl::narrow_cast<FORM_VERTEX_CLIP*>(GlobalLock(clipHandle));
  clipHeader->clipType    = CLP_FRMPS;
  clipHeader->vertexCount = SelectedFormVertices.vertexCount;
  clipHeader->direction   = StateMap->test(StateFlag::PSELDIR);
  // skip past the header
  auto* ptrVertices = convertFromPtr<F_POINT*>(std::next(clipHeader));
  auto const vertices = gsl::span<F_POINT> {ptrVertices, wrap::toSize(SelectedFormVertices.vertexCount) + 1U};
  auto const& form = FormList->operator[](ClosestFormToCursor);

  auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  auto       iSource  = SelectedFormVertices.start;
  for (auto& vertex : vertices) {
	auto sourceIt = wrap::next(itVertex, iSource);
	vertex        = *sourceIt;
	iSource       = form::pdir(form, iSource);
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(thrEdClip, clipHandle);
  CloseClipboard();
}

void tfc::clipSelectedStitches() {
  thred::rngadj();
  LowerLeftStitch.x = LowerLeftStitch.y = BIGFLOAT;
  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	auto const& stitch = StitchBuffer->operator[](iStitch);
	if (stitch.x < LowerLeftStitch.x) {
	  LowerLeftStitch.x = stitch.x;
	}
	if (stitch.y < LowerLeftStitch.y) {
	  LowerLeftStitch.y = stitch.y;
	}
  }
  auto const length  = GroupEndStitch - GroupStartStitch + 1U;
  auto       iSource = GroupStartStitch;
  // NOLINTNEXTLINE(hicpp-signed-bitwise,readability-qualified-auto)
  auto const clipHandle = GlobalAlloc(GHND, length * sizeof(CLIP_STITCH) + 2U);
  if (clipHandle == nullptr) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  EmptyClipboard();
  Clip                      = RegisterClipboardFormat(PcdClipFormat);
  auto*      clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
  auto const spData         = gsl::span<CLIP_STITCH> {clipStitchData, length};
  tfc::savclp(spData[0], StitchBuffer->operator[](iSource), length);
  ++iSource;
  for (auto iStitch = 1U; iStitch < length; ++iStitch) {
	tfc::savclp(spData[iStitch],
	            StitchBuffer->operator[](iSource),
	            (StitchBuffer->operator[](iSource).attribute & COLMSK));
	++iSource;
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(Clip, clipHandle);
  CloseClipboard();
}

void tfc::duclip() {
  if (StateMap->test(StateFlag::FPSEL)) {
	clipSelectedPoints();
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	displayText::tabmsg(IDS_INSF, false);
	return;
  }
  if (!SelectedFormList->empty()) {
	clipSelectedForms();
	return;
  }
  if (StateMap->test(StateFlag::FORMSEL)) {
	clipSelectedForm();
	return;
  }
  if (StitchBuffer->empty() || !StateMap->test(StateFlag::GRPSEL)) {
	return;
  }
  clipSelectedStitches();
}

auto fci::frmcnt(uint32_t iForm, uint32_t& formFirstStitchIndex) noexcept -> uint32_t {
  auto const codedAttribute = iForm << FRMSHFT;
  auto       iStitch        = StitchBuffer->begin();
  auto       stitchCount    = 0U;
  auto       flag           = true;
  for (; iStitch != StitchBuffer->end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && ((iStitch->attribute & TYPMSK) != 0U)) {
	  flag = false;
	  break;
	}
	++stitchCount;
  }
  if (flag) {
	return 0;
  }
  formFirstStitchIndex = stitchCount;
  stitchCount          = 0U;
  LowerLeftStitch      = F_POINT {BIGFLOAT, BIGFLOAT};
  for (; iStitch != StitchBuffer->end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && ((iStitch->attribute & TYPMSK) != 0U)) {
	  if (iStitch->x < LowerLeftStitch.x) {
		LowerLeftStitch.x = iStitch->x;
	  }
	  if (iStitch->y < LowerLeftStitch.y) {
		LowerLeftStitch.y = iStitch->y;
	  }
	  ++stitchCount;
	}
  }
  return stitchCount;
}

void tfc::savclp(CLIP_STITCH& destination, F_POINT_ATTR const& source, uint32_t led) {
  auto integer    = 0.0F;
  destination.led = led;
  auto fractional = std::modf(source.x - LowerLeftStitch.x, &integer);
  destination.fx  = wrap::floor<decltype(destination.fx)>(fractional * FRACFACT);
  wrap::narrow(destination.x, integer);
  destination.spcx = 0;
  fractional       = std::modf(source.y - LowerLeftStitch.y, &integer);
  destination.fy   = wrap::floor<decltype(destination.fy)>(fractional * FRACFACT);
  wrap::narrow(destination.y, integer);
  destination.spcy = 0;
  // ToDo - Are these structure members needed?
  destination.myst = 1;
  destination.tag  = 0x14;
}

void tfc::rtrclpfn(FRM_HEAD const& form) {
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  auto count = 0U;
  if (auto clipRect = F_RECTANGLE {}; form.isEdgeClip()) {
	count = form.clipEntries;
	clip::oclp(clipRect, form.borderClipData, count);
  }
  else {
	if (form.isClip()) {
	  count = form.lengthOrCount.clipCount;
	  clip::oclp(clipRect, form.angleOrClipData.clip, count);
	}
  }
  if (count == 0U) {
	CloseClipboard();
	return;
  }
  LowerLeftStitch.x = 0.0F;
  LowerLeftStitch.y = 0.0F;
  EmptyClipboard();
  Clip = RegisterClipboardFormat(PcdClipFormat);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  auto* const clipHandle = GlobalAlloc(GHND, count * sizeof(CLIP_STITCH) + 2U);
  if (nullptr == clipHandle) {
	CloseClipboard();
	return;
  }
  auto* clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
  if (nullptr == clipStitchData) {
	CloseClipboard();
	return;
  }
  auto const spClipData = gsl::span<CLIP_STITCH> {clipStitchData, count};
  tfc::savclp(spClipData[0], ClipBuffer->operator[](0), count);
  for (auto iStitch = 1U; iStitch < count; ++iStitch) {
	tfc::savclp(spClipData[iStitch], ClipBuffer->operator[](iStitch), 0);
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(Clip, clipHandle);
  CloseClipboard();
}

void tfc::rtrclp() {
  if (!StateMap->test(StateFlag::FORMSEL)) {
	return;
  }
  if (auto const& form = FormList->operator[](ClosestFormToCursor); form.isTexture()) {
	texture::rtrtx(form);
  }
  else {
	tfc::rtrclpfn(form);
  }
}

auto tfc::doPaste(std::vector<POINT> const& stretchBoxLine, bool& retflag) -> bool {
  retflag = true;
  thred::savdo();
  auto const thrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
  ClipMemory           = GetClipboardData(thrEdClip);
  if (ClipMemory != nullptr) {
	auto* clipPointer = GlobalLock(ClipMemory);
	if (clipPointer != nullptr) {
	  auto* ptrFormVertexData = convertFromPtr<FORM_VERTEX_CLIP*>(clipPointer);
	  if (ptrFormVertexData->clipType == CLP_FRMPS) {
		thred::duzrat();
		auto const byteCount =
		    sizeof(*ptrFormVertexData) +
		    (wrap::toSize(ptrFormVertexData->vertexCount) + 1U) * wrap::sizeofType(FormVertices);
		auto        clipCopyBuffer = std::vector<uint8_t> {};
		auto* const ptrClip        = convertFromPtr<uint8_t*>(clipPointer);
		auto const  clips          = gsl::span<uint8_t> {ptrClip, byteCount};
		clipCopyBuffer.insert(clipCopyBuffer.end(), clips.begin(), clips.end());
		GlobalUnlock(ClipMemory);
		CloseClipboard();
		ptrFormVertexData = convertFromPtr<FORM_VERTEX_CLIP*>(clipCopyBuffer.data());
		if (StateMap->test(StateFlag::FRMPSEL)) {
		  // clang-format off
		  auto& form     = FormList->operator[](ClosestFormToCursor);
		  auto  itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
		  // clang-format on
		  InterleaveSequence->clear();
		  InterleaveSequence->reserve(wrap::toSize(ptrFormVertexData->vertexCount) + 3U);
		  auto const closestIt = wrap::next(itVertex, ClosestVertexToCursor);
		  InterleaveSequence->push_back(*closestIt);
		  auto*      clipData     = convertFromPtr<F_POINT*>(std::next(ptrFormVertexData));
		  auto const formVertices = gsl::span<F_POINT> {clipData, ptrFormVertexData->vertexCount};
		  InterleaveSequence->insert(InterleaveSequence->end(), formVertices.begin(), formVertices.end());
		  auto const nextVertex = form::nxt(form, ClosestVertexToCursor);
		  auto const nextIt     = wrap::next(itVertex, nextVertex);
		  InterleaveSequence->push_back(*nextIt);
		  fci::setpclp();
		  StateMap->set(StateFlag::FPUNCLP);
		  StateMap->set(StateFlag::SHOP);
		  fci::dupclp();
		}
		else {
		  FormMoveDelta = F_POINT {};
		  StateMap->set(StateFlag::FUNCLP);
		  auto formIter          = FRM_HEAD {};
		  formIter.type          = FRMLINE;
		  formIter.vertexCount   = ptrFormVertexData->vertexCount + 1U;
		  formIter.vertexIndex   = wrap::toUnsigned(FormVertices->size());
		  auto*      ptrVertices = convertFromPtr<F_POINT*>(std::next(ptrFormVertexData));
		  auto const vertices    = gsl::span<F_POINT> {ptrVertices, formIter.vertexCount};
		  FormVertices->insert(FormVertices->end(), vertices.begin(), vertices.end());
		  FormList->push_back(formIter);
		  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
		  StateMap->set(StateFlag::INIT);
		  NewFormVertexCount = formIter.vertexCount;
		  form::unfrm();
		  form::setmfrm(ClosestFormToCursor);
		  StateMap->set(StateFlag::SHOFRM);
		  form::dufrm();
		}
		return true;
	  }
	  // ToDo - Add more information to the clipboard so that memory can be allocated
	  auto* ptrFormsHeader = gsl::narrow_cast<FORMS_CLIP*>(clipPointer);
	  if (ptrFormsHeader->clipType == CLP_FRMS) {
		auto iForm            = 0U;
		ClipFormsCount        = ptrFormsHeader->formCount;
		auto*      ptrForms   = convertFromPtr<FRM_HEAD*>(std::next(ptrFormsHeader));
		auto const forms      = gsl::span<FRM_HEAD> {ptrForms, ClipFormsCount};
		auto const formOffset = wrap::toUnsigned(FormList->size());
		for (auto& form : forms) {
		  FormList->push_back(form);
		  FormList->back().attribute = (gsl::narrow_cast<decltype(form.attribute)>(form.attribute & NFRMLMSK) |
		                                gsl::narrow_cast<decltype(form.attribute)>(ActiveLayer << 1U));
		}
		auto* ptrFormVertices = convertFromPtr<F_POINT*>(wrap::next(ptrForms, ClipFormsCount));
		auto  currentVertex   = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  auto&      form   = FormList->operator[](offset);
		  form.vertexIndex  = wrap::toUnsigned(FormVertices->size());
		  auto const formVertices =
		      gsl::span<F_POINT> {ptrFormVertices, wrap::toSize(currentVertex + form.vertexCount)};
		  FormVertices->insert(
		      FormVertices->end(), wrap::next(formVertices.begin(), currentVertex), formVertices.end());
		  currentVertex += form.vertexCount;
		}
		auto* ptrGuides  = convertFromPtr<SAT_CON*>(wrap::next(ptrFormVertices, currentVertex));
		auto  guideCount = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  auto&      form   = FormList->operator[](offset);
		  if (form.type == SAT && (form.satinGuideCount != 0U)) {
			guideCount += form.satinGuideCount;
		  }
		}
		auto       currentGuide = 0U;
		auto const guides       = gsl::span<SAT_CON> {ptrGuides, guideCount};
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  auto&      form   = FormList->operator[](offset);
		  if (form.type == SAT && (form.satinGuideCount != 0U)) {
			form.satinOrAngle.guide = satin::adsatk(form.satinGuideCount);
			auto itGuide            = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
			  *itGuide = guides[currentGuide++];
			  ++itGuide;
			}
		  }
		}
		auto* ptrClipData = convertFromPtr<F_POINT*>(wrap::next(ptrGuides, currentGuide));
		auto  clipCount   = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  // clang-format off
		  auto const offset = formOffset + iForm;
		  auto&      form   = FormList->operator[](offset);
		  // clang-format on
		  if (form.isClipX()) {
			clipCount += form.lengthOrCount.clipCount;
		  }
		  if (form.isEdgeClipX()) {
			clipCount += form.clipEntries;
		  }
		}
		auto       currentClip = 0U;
		auto const clipData    = gsl::span<F_POINT> {ptrClipData, clipCount};
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  // clang-format off
		  auto const offset = formOffset + iForm;
		  auto&      form   = FormList->operator[](offset);
		  // clang-format on
		  if (form.isClipX()) {
			form.angleOrClipData.clip = thred::adclp(form.lengthOrCount.clipCount);
			auto offsetStart          = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
			for (auto iClip = 0U; iClip < form.lengthOrCount.clipCount; ++iClip) {
			  *offsetStart = clipData[currentClip++];
			  ++offsetStart;
			}
		  }
		  if (form.isEdgeClipX()) {
			form.borderClipData = thred::adclp(form.clipEntries);
			auto offsetStart    = wrap::next(ClipPoints->begin(), form.borderClipData);
			for (auto iClip = 0U; iClip < form.clipEntries; ++iClip) {
			  *offsetStart = clipData[currentClip++];
			  ++offsetStart;
			}
		  }
		}
		auto* ptrTextureSource = convertFromPtr<TX_PNT*>(wrap::next(ptrClipData, currentClip));
		auto  textureCount     = size_t {};
		for (auto  spForms = std::ranges::subrange(wrap::next(FormList->begin(), formOffset),
                                                  wrap::next(wrap::next(FormList->begin(), formOffset), ClipFormsCount));
		     auto& form : spForms) {
		  if (form.isTexture()) {
			textureCount += form.fillInfo.texture.count;
			form.fillInfo.texture.index +=
			    gsl::narrow<decltype(form.fillInfo.texture.index)>(TexturePointsBuffer->size());
		  }
		}
		auto const textureSource = gsl::span<TX_PNT> {ptrTextureSource, textureCount};
		TexturePointsBuffer->insert(TexturePointsBuffer->end(), textureSource.begin(), textureSource.end());
		GlobalUnlock(ClipMemory);
		SelectedFormsRect.top = SelectedFormsRect.left = std::numeric_limits<LONG>::max();
		SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
		form::ratsr();
		SelectedFormList->clear();
		SelectedFormList->reserve(ClipFormsCount);
		for (auto index = 0U; index < (ClipFormsCount); ++index) {
		  form::fselrct(formOffset + index);
		  SelectedFormList->push_back(formOffset + index);
		}
		wrap::narrow_cast(SelectedFormsSize.x, SelectedFormsRect.right - SelectedFormsRect.left);
		wrap::narrow_cast(SelectedFormsSize.y, SelectedFormsRect.bottom - SelectedFormsRect.top);
		StateMap->set(StateFlag::INIT);
		auto& formLines = *FormLines;
		formLines.resize(SQPNTS);
		formLines[0].x = formLines[3].x = formLines[4].x = SelectedFormsRect.left;
		formLines[1].x = formLines[2].x = SelectedFormsRect.right;
		formLines[0].y = formLines[1].y = formLines[4].y = SelectedFormsRect.top;
		formLines[2].y = formLines[3].y = SelectedFormsRect.bottom;
		StateMap->set(StateFlag::SHOSTRTCH);
		thred::strtchbox(stretchBoxLine);
		FormMoveDelta =
		    F_POINT {gsl::narrow<float>((SelectedFormsRect.right - SelectedFormsRect.left) / 2),
		             gsl::narrow<float>((SelectedFormsRect.bottom - SelectedFormsRect.top) / 2)};
		StateMap->set(StateFlag::MOVFRMS);
		StateMap->set(StateFlag::FUNSCLP);
	  }
	  else {
		auto* ptrClipFormHeader = gsl::narrow_cast<FORM_CLIP*>(clipPointer);
		if (ptrClipFormHeader->clipType == CLP_FRM) {
		  FormMoveDelta = F_POINT {};
		  StateMap->set(StateFlag::FUNCLP);
		  auto formIter = ptrClipFormHeader->form;
		  formIter.attribute = gsl::narrow_cast<decltype(formIter.attribute)>(formIter.attribute & NFRMLMSK) |
		                       gsl::narrow_cast<decltype(formIter.attribute)>(ActiveLayer << 1U);
		  formIter.vertexIndex   = wrap::toUnsigned(FormVertices->size());
		  auto*      ptrVertices = convertFromPtr<F_POINT*>(std::next(ptrClipFormHeader));
		  auto const vertices    = gsl::span<F_POINT> {ptrVertices, formIter.vertexCount};
		  FormVertices->insert(FormVertices->end(), vertices.begin(), vertices.end());
		  auto* ptrGuides = convertFromPtr<SAT_CON*>(wrap::next(ptrVertices, formIter.vertexCount));
		  if (formIter.type == SAT && (formIter.satinGuideCount != 0U)) {
			auto const guides           = gsl::span<SAT_CON> {ptrGuides, formIter.satinGuideCount};
			formIter.satinOrAngle.guide = wrap::toUnsigned(SatinGuides->size());
			SatinGuides->insert(SatinGuides->end(), guides.begin(), guides.end());
		  }
		  auto* ptrClipData = convertFromPtr<F_POINT*>(wrap::next(ptrGuides, formIter.satinGuideCount));
		  auto clipCount = 0U;
		  if (formIter.isClipX()) {
			auto const clipData = gsl::span<F_POINT> {ptrClipData, formIter.lengthOrCount.clipCount};
			formIter.angleOrClipData.clip = wrap::toUnsigned(ClipPoints->size());
			ClipPoints->insert(ClipPoints->end(), clipData.begin(), clipData.end());
			clipCount += formIter.lengthOrCount.clipCount;
		  }
		  if (formIter.isEdgeClipX()) {
			ptrClipData             = convertFromPtr<F_POINT*>(wrap::next(ptrClipData, clipCount));
			auto const clipData     = gsl::span<F_POINT> {ptrClipData, formIter.clipEntries};
			formIter.borderClipData = wrap::toUnsigned(ClipPoints->size());
			ClipPoints->insert(ClipPoints->end(), clipData.begin(), clipData.end());
			clipCount += formIter.clipEntries;
		  }
		  if (formIter.isTexture()) {
			auto* ptrTextureSource = convertFromPtr<TX_PNT*>(wrap::next(ptrClipData, clipCount));
			auto const textureSource =
			    gsl::span<TX_PNT> {ptrTextureSource, formIter.fillInfo.texture.count};
			wrap::narrow(formIter.fillInfo.texture.index, TexturePointsBuffer->size());
			TexturePointsBuffer->insert(
			    TexturePointsBuffer->end(), textureSource.begin(), textureSource.end());
		  }
		  NewFormVertexCount = formIter.vertexCount;
		  if (formIter.type != FRMLINE) {
			++NewFormVertexCount;
		  }
		  FormList->push_back(formIter);
		  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
		  StateMap->set(StateFlag::INIT);
		  form::unfrm();
		  thred::duzrat();
		  form::setmfrm(ClosestFormToCursor);
		  StateMap->set(StateFlag::SHOFRM);
		  form::dufrm();
		}
		GlobalUnlock(ClipMemory);
	  }
	}
	CloseClipboard();
  }
  else {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  thred::clpbox();
	  StateMap->set(StateFlag::CLPSHO);
	}
	CloseClipboard();
  }
  retflag = false;
  return {};
}

void tfc::txtclp(FRM_HEAD& textureForm) {
  auto const thrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
  if (0U == thrEdClip) {
	return;
  }
  auto const clipData = GetClipboardData(thrEdClip); // NOLINT(readability-qualified-auto)
  if (nullptr == clipData) {
	return;
  }
  auto const clipMemory = GlobalLock(clipData); // NOLINT(readability-qualified-auto)
  if (nullptr == clipMemory) {
	return;
  }
  auto* clipForm = tfc::getClipForm(clipMemory);
  if (nullptr == clipForm) {
	GlobalUnlock(clipMemory);
	return;
  }
  textureForm           = *clipForm;
  auto*      vertices   = convertFromPtr<F_POINT*>(std::next(clipForm));
  auto const spVertices = gsl::span<F_POINT> {vertices, textureForm.vertexCount};
  AngledFormVertices->clear();
  AngledFormVertices->insert(AngledFormVertices->end(), spVertices.begin(), spVertices.end());
  textureForm.vertexIndex = 0;
  StateMap->reset(StateFlag::TXTLIN);
  StateMap->set(StateFlag::TXTCLP);
  StateMap->set(StateFlag::TXTMOV);
  texture::setxfrm();
  texture::setTxtCurLoc(POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y});
  GlobalUnlock(clipMemory);
  StateMap->set(StateFlag::RESTCH);
  StateMap->reset(StateFlag::WASWROT);
}

void fci::setpclp() {
  FormVerticesAsLine->clear();
  auto itIntlvSeq = InterleaveSequence->begin();
  auto point      = form::sfCor2px(*itIntlvSeq);
  ++itIntlvSeq;
  FormVerticesAsLine->push_back(point);
  point = form::sfCor2px(*itIntlvSeq);
  auto const offset =
      POINT {Msg.pt.x - StitchWindowOrigin.x - point.x, Msg.pt.y - StitchWindowOrigin.y - point.y};
  for (auto ine = 1U; ine < wrap::toUnsigned(InterleaveSequence->size()) - 1U; ++ine) {
	point = form::sfCor2px(*itIntlvSeq);
	++itIntlvSeq;
	FormVerticesAsLine->push_back(POINT {point.x + offset.x, point.y + offset.y});
  }
  point = form::sfCor2px(InterleaveSequence->back());
  FormVerticesAsLine->push_back(point);
}

void fci::dupclp() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, FormVerticesAsLine->data(), wrap::toUnsigned(FormVerticesAsLine->size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void fci::unpclp() {
  if (StateMap->testAndReset(StateFlag::SHOP)) {
	fci::dupclp();
  }
}

void tfc::fpUnClip() {
  fci::unpclp();
  fci::setpclp();
  StateMap->set(StateFlag::SHOP);
  fci::dupclp();
}

void tfc::lodclp(uint32_t iStitch) {
  StitchBuffer->insert(wrap::next(StitchBuffer->begin(), iStitch), ClipBuffer->size(), F_POINT_ATTR {});
  ClosestPointIndex  = iStitch;
  auto const originX = wrap::toFloat(ClipOrigin.x);
  auto const originY = wrap::toFloat(ClipOrigin.y);
  for (auto const& clip : *ClipBuffer) {
	StitchBuffer->operator[](iStitch++) =
	    F_POINT_ATTR {clip.x + originX,
	                  clip.y + originY,
	                  (clip.attribute & COLMSK) | gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT) | NOTFRM};
  }
  GroupStitchIndex = iStitch - 1U;
  StateMap->set(StateFlag::GRPSEL);
  if (!StitchBuffer->empty()) {
	StateMap->set(StateFlag::INIT);
  }
}
