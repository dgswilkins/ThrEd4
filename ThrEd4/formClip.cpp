// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "clip.h"
#include "clipStitch.h"
#include "displayText.h"
#include "form.h"
#include "formClip.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "point.h"
#include "Resources/resource.h"
#include "satCon.h"
#include "satin.h"
#include "texture.h"
#include "textureHeader.h"
#include "thred.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "gsl/narrow"
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <minwindef.h>
#include <WinBase.h>
#include <windef.h>
#include <wingdi.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
// ReSharper disable CppUnusedIncludeDirective
#include <ranges>
#include <type_traits>
// ReSharper restore CppUnusedIncludeDirective
#include <vector>

#pragma pack(push, 1)
class FORM_CLIP // form data clipboard header
{
  public:
  uint32_t clipType {};
  uint32_t reserved {};
  FRM_HEAD form;
};
#pragma pack(pop)

#pragma pack(push, 1)
class FORMS_CLIP // multiple forms clipboard header
{
  public:
  uint32_t clipType {};
  uint16_t formCount {};
  uint16_t reserved {};
};
#pragma pack(pop)

#pragma pack(push, 1)
class FORM_VERTEX_CLIP // form points clipboard header
{
  public:
  uint32_t clipType {};
  uint32_t vertexCount {};
  bool     direction {};
};
#pragma pack(pop)

class FCLIP_SINGLE
{
  public:
  static auto getInstance() noexcept -> FCLIP_SINGLE* {
	// NOLINTNEXTLINE(clang-diagnostic-exit-time-destructors)
	static FCLIP_SINGLE instance;
	return &instance;
  }

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::vector<POINT> FormVerticesAsLine; // form vertex clipboard paste into form line
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  FCLIP_SINGLE(const FCLIP_SINGLE&)                    = delete;
  auto operator=(const FCLIP_SINGLE&) -> FCLIP_SINGLE& = delete;
  FCLIP_SINGLE(FCLIP_SINGLE&&)                         = delete;
  auto operator=(FCLIP_SINGLE&&) -> FCLIP_SINGLE&      = delete;

  private:
  FCLIP_SINGLE() noexcept = default;
  ~FCLIP_SINGLE()         = default;
};

namespace {
auto ClipFormsCount  = uint32_t {}; // number of forms the on the clipboard
auto ClipOrigin      = POINT {};    // origin of clipboard box in stitch coordinates
auto LowerLeftStitch = F_POINT {};  // lower left formOrigin in a form

FCLIP_SINGLE* FormClipInstance;

// Definitions
void clipSelectedForm();
void clipSelectedForms();
void clipSelectedPoints();
void clipSelectedStitches();
void dupclp() noexcept(std::is_same_v<size_t, uint32_t>);
auto frmcnt(uint32_t iForm, uint32_t& formFirstStitchIndex) noexcept -> uint32_t;
auto getClipForm(LPVOID clipMemory) noexcept -> FRM_HEAD*;
auto getThredClipMemory() noexcept -> HGLOBAL;
auto registerPCDFormat() noexcept -> uint32_t;
auto registerThredClipFormat() noexcept -> uint32_t;
void rtrclpfn(FRM_HEAD const& form);
void savclp(CLIP_STITCH& destination, F_POINT_ATTR const& source, uint32_t led);
void setpclp();
void sizclp(FRM_HEAD const& form,
            uint32_t&       formFirstStitchIndex,
            uint32_t&       formStitchCount,
            uint32_t&       length,
            uint32_t&       fileSize) noexcept(std::is_same_v<size_t, uint32_t>);
auto sizfclp(FRM_HEAD const& form) noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t;
void unpclp();

// Functions

auto registerPCDFormat() noexcept -> uint32_t {
  constexpr auto PCD_CLIP_FORMAT = L"PMust_Format";
  return RegisterClipboardFormat(PCD_CLIP_FORMAT);
}

auto registerThredClipFormat() noexcept -> uint32_t {
  constexpr auto THRED_CLIP_FORMAT = L"threditor"; //
  return RegisterClipboardFormat(THRED_CLIP_FORMAT);
}

auto getThredClipMemory() noexcept -> HGLOBAL {
  if (auto const clip = registerThredClipFormat(); clip != 0U) {
	return GetClipboardData(clip);
  }
  return nullptr;
}

void clipSelectedForm() {
  auto        firstStitch = 0U; // points to the first stitch in a form
  auto        stitchCount = 0U;
  auto        length      = 0U;
  auto        clipSize    = 0U;
  auto const& form        = Instance->formList.operator[](ClosestFormToCursor);
  sizclp(form, firstStitch, stitchCount, length, clipSize);
  clipSize += sizeof(FORM_CLIP);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto clipHandle = GlobalAlloc(GHND, clipSize);
  if (clipHandle == nullptr) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  EmptyClipboard();
  auto const thrEdClip       = registerThredClipFormat();
  auto*      clipFormHeader  = gsl::narrow_cast<FORM_CLIP*>(GlobalLock(clipHandle));
  clipFormHeader->clipType   = CLP_FRM;
  clipFormHeader->form       = form;
  auto*      ptrFormVertices = convertFromPtr<F_POINT*>(std::next(clipFormHeader));
  auto const startVertex     = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  auto const endVertex       = wrap::next(startVertex, form.vertexCount);

  auto const vertices = gsl::span {ptrFormVertices, form.vertexCount};
  std::copy(startVertex, endVertex, vertices.begin());
  auto* ptrGuides = convertFromPtr<SAT_CON*>(wrap::next(ptrFormVertices, form.vertexCount));
  auto  iGuide    = 0U;
  if (form.type == SAT && form.satinGuideCount != 0U) {
	iGuide                = form.satinGuideCount;
	auto const startGuide = wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex);
	auto const endGuide   = wrap::next(startGuide, iGuide);

	auto const guides = gsl::span {ptrGuides, iGuide};
	std::copy(startGuide, endGuide, guides.begin());
  }
  auto* ptrMclp = convertFromPtr<F_POINT*>(wrap::next(ptrGuides, iGuide));
  auto  iClip   = 0U;
  if (form.isClipX()) {
	iClip                = form.clipCount;
	auto const startMclp = wrap::next(Instance->clipPoints.cbegin(), form.clipIndex);
	auto const endMclp   = wrap::next(startMclp, iClip);

	auto const mclps = gsl::span {ptrMclp, iClip};
	std::copy(startMclp, endMclp, mclps.begin());
  }
  auto* ptrPoints = convertFromPtr<F_POINT*>(wrap::next(ptrMclp, iClip));
  iClip           = 0U;
  if (form.isEdgeClipX()) {
	iClip                = form.clipEntries;
	auto const startClip = wrap::next(Instance->clipPoints.cbegin(), form.borderClipData);
	auto const endClip   = wrap::next(startClip, iClip);

	auto const points = gsl::span {ptrPoints, iClip};
	std::copy(startClip, endClip, points.begin());
  }
  auto* textures = convertFromPtr<TX_PNT*>(wrap::next(ptrPoints, iClip));
  if (form.isTexture()) {
	auto const startTexture = wrap::next(Instance->texturePointsBuffer.cbegin(), form.texture.index);
	auto const endTexture = wrap::next(startTexture, form.texture.count);

	auto const spDest = gsl::span {textures, form.texture.count};
	std::copy(startTexture, endTexture, spDest.begin());
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(thrEdClip, clipHandle);
  Instance->stateMap.reset(StateFlag::WASPCDCLP);
  if (form.fillType == 0U && form.edgeType == 0U) {
	CloseClipboard();
	return;
  }
  if (auto const clip = registerPCDFormat(); clip != 0U) {
	clipHandle = GlobalAlloc(GHND, (stitchCount * sizeof(CLIP_STITCH)) + 2U);
	if (clipHandle == nullptr) {
	  CloseClipboard();
	  return;
	}
	auto*      clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
	auto const spData         = gsl::span {clipStitchData, stitchCount};
	auto       iTexture       = firstStitch;
	savclp(spData[0], Instance->stitchBuffer.operator[](iTexture), length);
	++iTexture;
	auto       iDestination   = 1U;
	auto const codedAttribute = ClosestFormToCursor << FRMSHFT;
	while (iTexture < Instance->stitchBuffer.size()) {
	  if ((Instance->stitchBuffer.operator[](iTexture).attribute & FRMSK) == codedAttribute &&
	      (Instance->stitchBuffer.operator[](iTexture).attribute & NOTFRM) == 0U) {
		savclp(spData[iDestination++],
		       Instance->stitchBuffer.operator[](iTexture),
		       Instance->stitchBuffer.operator[](iTexture).attribute & COLMSK);
	  }
	  ++iTexture;
	}
	GlobalUnlock(clipHandle);
	SetClipboardData(clip, clipHandle);
	Instance->stateMap.set(StateFlag::WASPCDCLP);
  }
  CloseClipboard();
}

void clipSelectedForms() {
  auto  length   = 0U;
  auto& formList = Instance->formList;

  for (auto& selectedForm : Instance->selectedFormList) {
	auto& currentForm = formList.operator[](selectedForm);
	length += sizfclp(currentForm);
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto clipHandle = GlobalAlloc(GHND, length + sizeof(FORMS_CLIP));
  if (clipHandle == nullptr) { // exit gracefully without altering the clipboard
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  EmptyClipboard();
  auto const thrEdClip       = registerThredClipFormat();
  auto*      clipFormsHeader = gsl::narrow_cast<FORMS_CLIP*>(GlobalLock(clipHandle));
  clipFormsHeader->clipType  = CLP_FRMS;
  wrap::narrow(clipFormsHeader->formCount, Instance->selectedFormList.size());
  // Skip past the header
  auto*      ptrForms = convertFromPtr<FRM_HEAD*>(std::next(clipFormsHeader));
  auto const forms    = gsl::span {ptrForms, Instance->selectedFormList.size()};
  auto       iForm    = 0U;
  for (auto& selectedForm : Instance->selectedFormList) {
	auto& currentForm = formList.operator[](selectedForm);
	forms[iForm++]    = currentForm;
  }
  // skip past the forms
  auto* ptrFormVertices = convertFromPtr<F_POINT*>(wrap::next(ptrForms, iForm));
  auto  verticesSize    = 0U;
  for (auto& selectedForm : Instance->selectedFormList) {
	verticesSize += formList.operator[](selectedForm).vertexCount;
  }
  auto const formVertices = gsl::span {ptrFormVertices, verticesSize};
  auto       iVertex      = 0U;
  for (auto& selectedForm : Instance->selectedFormList) {
	auto& form = formList.operator[](selectedForm);

	auto itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	for (auto iSide = 0U; iSide < form.vertexCount; ++iSide) {
	  formVertices[iVertex++] = *itVertex;
	  ++itVertex;
	}
  }
  // skip past the vertex list
  auto* ptrGuides  = convertFromPtr<SAT_CON*>(wrap::next(ptrFormVertices, iVertex));
  auto  guidesSize = 0U;
  for (auto& selectedForm : Instance->selectedFormList) {
	if (auto& form = formList.operator[](selectedForm); form.type == SAT && form.satinGuideCount != 0U) {
	  guidesSize += form.satinGuideCount;
	}
  }
  auto guideCount = 0U;
  if (guidesSize != 0U) {
	auto const guides = gsl::span {ptrGuides, guidesSize};
	for (auto& selectedForm : Instance->selectedFormList) {
	  if (auto& form = formList.operator[](selectedForm); form.type == SAT && form.satinGuideCount != 0U) {
		auto itGuide = wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex);
		for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		  guides[guideCount++] = *itGuide;
		  ++itGuide;
		}
	  }
	}
  }
  // skip past the guides
  auto* ptrPoints  = convertFromPtr<F_POINT*>(wrap::next(ptrGuides, guideCount));
  auto  pointsSize = 0U;
  for (auto& selectedForm : Instance->selectedFormList) {
	auto& form = formList.operator[](selectedForm);
	if (form.isClipX()) {
	  pointsSize += form.clipCount;
	}
	if (form.isEdgeClip()) {
	  pointsSize += form.clipEntries;
	}
  }
  auto pointCount = uint32_t {0U};
  if (pointsSize != 0U) {
	auto const points = gsl::span {ptrPoints, pointsSize};
	for (auto& selectedForm : Instance->selectedFormList) {
	  auto& form = formList.operator[](selectedForm);
	  if (form.isClipX()) {
		auto offsetStart = wrap::next(Instance->clipPoints.cbegin(), form.clipIndex);
		for (auto iClip = 0U; iClip < form.clipCount; ++iClip) {
		  points[pointCount++] = *offsetStart;
		  ++offsetStart;
		}
	  }
	  if (!form.isEdgeClip()) {
		continue;
	  }
	  auto offsetStart = wrap::next(Instance->clipPoints.cbegin(), form.borderClipData);
	  for (auto iClip = 0U; iClip < form.clipEntries; ++iClip) {
		points[pointCount++] = *offsetStart;
		++offsetStart;
	  }
	}
  }
  // Skip past the points
  auto* textures     = convertFromPtr<TX_PNT*>(std::next(ptrPoints, wrap::toPtrdiff(pointCount)));
  auto  textureCount = uint16_t {};
  iForm              = 0;
  for (auto& selectedForm : Instance->selectedFormList) {
	auto& form = formList.operator[](selectedForm);
	if (!form.isTexture()) {
	  continue;
	}
	auto       startPoint = wrap::next(Instance->texturePointsBuffer.cbegin(), form.texture.index);
	auto       endPoint   = wrap::next(startPoint, form.texture.count);
	auto const spDest     = gsl::span {std::next(textures, textureCount), form.texture.count};
	std::copy(startPoint, endPoint, spDest.begin());
	forms[iForm++].texture.index = textureCount;
	textureCount += form.texture.count;
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(thrEdClip, clipHandle);

  auto formMap = boost::dynamic_bitset(formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  for (auto& selectedForm : Instance->selectedFormList) {
	formMap.set(selectedForm);
  }
  auto astch = std::vector<F_POINT_ATTR> {};
  // Reserve a reasonable amount space, probably not enough though
  constexpr auto RESRATIO = 16U; // reserve space factor
  astch.reserve(Instance->stitchBuffer.size() / RESRATIO);
  LowerLeftStitch = F_POINT {BIGFLOAT, BIGFLOAT};
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & NOTFRM) != 0U || !formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
	  continue;
	}
	LowerLeftStitch.x = std::min(LowerLeftStitch.x, stitch.x);
	LowerLeftStitch.y = std::min(LowerLeftStitch.y, stitch.y);
	astch.push_back(stitch);
  }
  auto const stitchCount = wrap::toUnsigned(astch.size());
  if (Instance->stitchBuffer.empty() || stitchCount == 0) {
	CloseClipboard();
	return;
  }
  if (auto const clip = registerPCDFormat(); clip != 0) {
	clipHandle = GlobalAlloc(GHND, (stitchCount * sizeof(CLIP_STITCH)) + 2U);
	if (clipHandle == nullptr) {
	  CloseClipboard();
	  return;
	}
	auto*      clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
	auto const spData         = gsl::span {clipStitchData, stitchCount};
	auto       iStitch        = 0U;
	auto       iDestination   = 0U;
	savclp(spData[0], astch[0], stitchCount);
	++iStitch;
	++iDestination;
	while (iStitch < stitchCount) {
	  savclp(spData[iDestination], astch[iStitch], astch[iStitch].attribute & COLMSK);
	  ++iStitch;
	  ++iDestination;
	}
	GlobalUnlock(clipHandle);
	SetClipboardData(clip, clipHandle);
  }
  CloseClipboard();
}

void clipSelectedPoints() {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const clipHandle = GlobalAlloc(
      GHND, ((wrap::toSize(SelectedFormVertices.vertexCount) + 1U) * sizeof(F_POINT)) + sizeof(FORM_VERTEX_CLIP));
  if (clipHandle == nullptr) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  EmptyClipboard();
  auto const thrEdClip    = registerThredClipFormat();
  auto*      clipHeader   = gsl::narrow_cast<FORM_VERTEX_CLIP*>(GlobalLock(clipHandle));
  clipHeader->clipType    = CLP_FRMPS;
  clipHeader->vertexCount = SelectedFormVertices.vertexCount;
  clipHeader->direction   = Instance->stateMap.test(StateFlag::PSELDIR);
  // skip past the header
  auto* ptrVertices = convertFromPtr<F_POINT*>(std::next(clipHeader));
  auto const vertices = gsl::span {ptrVertices, wrap::toSize(SelectedFormVertices.vertexCount) + 1U};
  auto const& form = Instance->formList.operator[](ClosestFormToCursor);

  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
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

void clipSelectedStitches() {
  thred::rngadj();
  if (GroupStartStitch != GroupEndStitch) {
	LowerLeftStitch = F_POINT {BIGFLOAT, BIGFLOAT};
	for (auto const groupStitchRange =
	         std::ranges::subrange(wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch),
	                               wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch));
	     auto& stitch : groupStitchRange) {
	  LowerLeftStitch.x = std::min(LowerLeftStitch.x, stitch.x);
	  LowerLeftStitch.y = std::min(LowerLeftStitch.y, stitch.y);
	}
	auto const length  = GroupEndStitch - GroupStartStitch;
	auto       iSource = GroupStartStitch;
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const clipHandle = GlobalAlloc(GHND, (length * sizeof(CLIP_STITCH)) + 2U);
	if (clipHandle == nullptr) {
	  return;
	}
	if (OpenClipboard(ThrEdWindow) == 0) {
	  return;
	}
	EmptyClipboard();
	if (auto const clip = registerPCDFormat(); clip != 0) {
	  auto*      clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
	  auto const spData         = gsl::span {clipStitchData, length};
	  savclp(spData[0], Instance->stitchBuffer.operator[](iSource), length);
	  ++iSource;
	  for (auto iStitch = 1U; iStitch < length; ++iStitch) {
		savclp(spData[iStitch],
		       Instance->stitchBuffer.operator[](iSource),
		       Instance->stitchBuffer.operator[](iSource).attribute & COLMSK);
		++iSource;
	  }
	  GlobalUnlock(clipHandle);
	  SetClipboardData(clip, clipHandle);
	}
	CloseClipboard();
  }
  else {
	if (OpenClipboard(ThrEdWindow) == 0) {
	  return;
	}
	EmptyClipboard();
	CloseClipboard();
  }
}

void dupclp() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC,
                 FormClipInstance->FormVerticesAsLine.data(),
                 wrap::toUnsigned(FormClipInstance->FormVerticesAsLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

auto frmcnt(uint32_t const iForm, uint32_t& formFirstStitchIndex) noexcept -> uint32_t {
  auto const codedAttribute = iForm << FRMSHFT;
  auto       iStitch        = Instance->stitchBuffer.begin();
  auto       stitchCount    = 0U;
  auto       flag           = true;
  for (; iStitch != Instance->stitchBuffer.end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && (iStitch->attribute & ALTYPMSK) != 0U) {
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
  for (; iStitch != Instance->stitchBuffer.end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && (iStitch->attribute & ALTYPMSK) != 0U) {
	  LowerLeftStitch.x = std::min(LowerLeftStitch.x, iStitch->x);
	  LowerLeftStitch.y = std::min(LowerLeftStitch.y, iStitch->y);
	  ++stitchCount;
	}
  }
  return stitchCount;
}

auto getClipForm(LPVOID clipMemory) noexcept -> FRM_HEAD* {
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

void rtrclpfn(FRM_HEAD const& form) {
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
	  count = form.clipCount;
	  clip::oclp(clipRect, form.clipIndex, count);
	}
  }
  if (count == 0U) {
	CloseClipboard();
	return;
  }
  LowerLeftStitch = F_POINT {0.0F, 0.0F};
  EmptyClipboard();
  if (auto const clip = registerPCDFormat(); clip != 0) {
	auto* const clipHandle = GlobalAlloc(GHND, (count * sizeof(CLIP_STITCH)) + 2U);
	if (nullptr == clipHandle) {
	  CloseClipboard();
	  return;
	}
	auto* clipStitchData = gsl::narrow_cast<CLIP_STITCH*>(GlobalLock(clipHandle));
	if (nullptr == clipStitchData) {
	  CloseClipboard();
	  return;
	}
	auto const  spClipData = gsl::span {clipStitchData, count};
	auto const& clipBuffer = Instance->clipBuffer;

	savclp(spClipData[0], clipBuffer.operator[](0), count);
	for (auto iStitch = 1U; iStitch < count; ++iStitch) {
	  savclp(spClipData[iStitch], clipBuffer.operator[](iStitch), 0);
	}
	GlobalUnlock(clipHandle);
	SetClipboardData(clip, clipHandle);
  }
  CloseClipboard();
}

void savclp(CLIP_STITCH& destination, F_POINT_ATTR const& source, uint32_t const led) {
  auto integer    = 0.0F;
  destination.led = led;
  auto fractional = std::modf(source.x - LowerLeftStitch.x, &integer);
  destination.fx  = wrap::floor<decltype(destination.fx)>(fractional * FRACFACT);
  wrap::narrow(destination.x, integer);
  fractional     = std::modf(source.y - LowerLeftStitch.y, &integer);
  destination.fy = wrap::floor<decltype(destination.fy)>(fractional * FRACFACT);
  wrap::narrow(destination.y, integer);
}

void setpclp() {
  auto& interleaveSequence = Instance->interleaveSequence;

  FormClipInstance->FormVerticesAsLine.clear();
  auto itIntlvSeq = interleaveSequence.begin();
  auto point      = form::sfCor2px(*itIntlvSeq);
  ++itIntlvSeq;
  FormClipInstance->FormVerticesAsLine.push_back(point);
  point             = form::sfCor2px(*itIntlvSeq);
  auto const offset = POINT {WinMsg.pt.x - StitchWindowOrigin.x - point.x,
                             WinMsg.pt.y - StitchWindowOrigin.y - point.y};
  for (auto ine = 1U; ine < wrap::toUnsigned(interleaveSequence.size()) - 1U; ++ine) {
	point = form::sfCor2px(*itIntlvSeq);
	++itIntlvSeq;
	FormClipInstance->FormVerticesAsLine.push_back(POINT {point.x + offset.x, point.y + offset.y});
  }
  point = form::sfCor2px(interleaveSequence.back());
  FormClipInstance->FormVerticesAsLine.push_back(point);
}

void sizclp(FRM_HEAD const& form,
            uint32_t&       formFirstStitchIndex,
            uint32_t&       formStitchCount,
            uint32_t&       length,
            uint32_t&       fileSize) noexcept(std::is_same_v<size_t, uint32_t>) {
  fileSize = wrap::toUnsigned(sizeof(FORM_CLIP)) + form.vertexCount * wrap::sizeofType(Instance->formVertices);
  length = fileSize;
  if (form.type == SAT && form.satinGuideCount != 0U) {
	fileSize += form.satinGuideCount * wrap::sizeofType(Instance->satinGuides);
  }
  if (form.fillType != 0U || form.edgeType != 0U) {
	formStitchCount = frmcnt(ClosestFormToCursor, formFirstStitchIndex);
	length += formStitchCount;
	fileSize += length * wrap::sizeofType(Instance->stitchBuffer);
  }
  if (form.isEdgeClip()) {
	fileSize += form.clipEntries * wrap::sizeofType(Instance->clipPoints);
  }
  if (form.isClipX()) {
	fileSize += form.clipCount * wrap::sizeofType(Instance->clipPoints);
  }
  if (form.isTexture()) {
	fileSize += form.texture.count * wrap::sizeofType(Instance->texturePointsBuffer);
  }
}

auto sizfclp(FRM_HEAD const& form) noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t {
  auto clipSize = wrap::toUnsigned(sizeof(FORM_CLIP)) +
                  (form.vertexCount * wrap::sizeofType(Instance->formVertices));
  if (form.type == SAT && form.satinGuideCount != 0U) {
	clipSize += form.satinGuideCount * wrap::sizeofType(Instance->satinGuides);
  }
  if (form.isEdgeClip()) {
	clipSize += form.clipEntries * wrap::sizeofType(Instance->clipPoints);
  }
  if (form.isClipX()) {
	clipSize += form.clipCount * wrap::sizeofType(Instance->clipPoints);
  }
  if (form.isTexture()) {
	clipSize += form.texture.count * wrap::sizeofType(Instance->texturePointsBuffer);
  }
  return clipSize;
}

void unpclp() {
  if (Instance->stateMap.testAndReset(StateFlag::SHOP)) {
	dupclp();
  }
}

} // namespace

auto tfc::getClipFormCount() noexcept -> uint32_t {
  return ClipFormsCount;
}

void tfc::setClipOrigin(POINT const source) noexcept {
  ClipOrigin = source;
}

void tfc::duclip() {
  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	clipSelectedPoints();
	return;
  }
  if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	displayText::tabmsg(IDS_INSF, false);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	clipSelectedForms();
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	clipSelectedForm();
	return;
  }
  if (Instance->stitchBuffer.empty() || !Instance->stateMap.test(StateFlag::GRPSEL)) {
	return;
  }
  clipSelectedStitches();
}

void tfc::rtrclp() {
  if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	return;
  }
  if (auto const& form = Instance->formList.operator[](ClosestFormToCursor); form.isTexture()) {
	texture::rtrtx(form);
  }
  else {
	rtrclpfn(form);
  }
}

auto tfc::getPCDClipMemory() noexcept -> HGLOBAL {
  if (auto const clip = registerPCDFormat(); clip != 0U) {
	return GetClipboardData(clip);
  }
  return nullptr;
}

auto tfc::doPaste(std::vector<POINT> const& stretchBoxLine, bool& retflag) -> bool {
  retflag = true;
  thred::savdo();
  if (auto* clipMemory = getThredClipMemory(); clipMemory != nullptr) {
	if (auto* clipPointer = GlobalLock(clipMemory); clipPointer != nullptr) {
	  if (auto* ptrFormVertexData = convertFromPtr<FORM_VERTEX_CLIP*>(clipPointer);
	      ptrFormVertexData->clipType == CLP_FRMPS) {
		thred::duzrat();
		auto const byteCount =
		    sizeof(*ptrFormVertexData) + ((wrap::toSize(ptrFormVertexData->vertexCount) + 1U) *
		                                  wrap::sizeofType(Instance->formVertices));
		auto        clipCopyBuffer = std::vector<uint8_t> {};
		auto* const ptrClip        = convertFromPtr<uint8_t*>(clipPointer);
		auto const  clips          = gsl::span {ptrClip, byteCount};
		clipCopyBuffer.insert(clipCopyBuffer.end(), clips.begin(), clips.end());
		GlobalUnlock(clipMemory);
		CloseClipboard();
		ptrFormVertexData = convertFromPtr<FORM_VERTEX_CLIP*>(clipCopyBuffer.data());
		if (Instance->stateMap.test(StateFlag::FRMPSEL)) {
		  // clang-format off
		  auto& form     = Instance->formList.operator[](ClosestFormToCursor);
		  auto  itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
		  // clang-format on
		  auto& interleaveSequence = Instance->interleaveSequence;

		  interleaveSequence.clear();
		  interleaveSequence.reserve(wrap::toSize(ptrFormVertexData->vertexCount) + 3U);
		  auto const closestIt = wrap::next(itVertex, ClosestVertexToCursor);
		  interleaveSequence.push_back(*closestIt);
		  auto*      clipData     = convertFromPtr<F_POINT*>(std::next(ptrFormVertexData));
		  auto const formVertices = gsl::span {clipData, ptrFormVertexData->vertexCount};
		  interleaveSequence.insert(interleaveSequence.end(), formVertices.begin(), formVertices.end());
		  auto const nextVertex = form::nxt(form, ClosestVertexToCursor);
		  auto const nextIt     = wrap::next(itVertex, nextVertex);
		  interleaveSequence.push_back(*nextIt);
		  setpclp();
		  Instance->stateMap.set(StateFlag::FPUNCLP);
		  Instance->stateMap.set(StateFlag::SHOP);
		  dupclp();
		}
		else {
		  FormMoveDelta = F_POINT {};
		  Instance->stateMap.set(StateFlag::FUNCLP);
		  auto formIter          = FRM_HEAD {};
		  formIter.type          = FRMLINE;
		  formIter.vertexCount   = ptrFormVertexData->vertexCount + 1U;
		  formIter.vertexIndex   = wrap::toUnsigned(Instance->formVertices.size());
		  auto*      ptrVertices = convertFromPtr<F_POINT*>(std::next(ptrFormVertexData));
		  auto const vertices    = gsl::span {ptrVertices, formIter.vertexCount};
		  Instance->formVertices.insert(Instance->formVertices.end(), vertices.begin(), vertices.end());
		  Instance->formList.push_back(formIter);
		  ClosestFormToCursor = wrap::toUnsigned(Instance->formList.size() - 1U);
		  Instance->stateMap.set(StateFlag::INIT);
		  form::setNewFormVertexCount(formIter.vertexCount);
		  form::unfrm();
		  form::setmfrm(ClosestFormToCursor);
		  Instance->stateMap.set(StateFlag::SHOFRM);
		  form::dufrm();
		}
		return true;
	  }
	  auto& formList = Instance->formList;

	  // ToDo - Add more information to the clipboard so that memory can be allocated
	  if (auto* ptrFormsHeader = gsl::narrow_cast<FORMS_CLIP*>(clipPointer); ptrFormsHeader->clipType == CLP_FRMS) {
		auto iForm            = 0U;
		ClipFormsCount        = ptrFormsHeader->formCount;
		auto*      ptrForms   = convertFromPtr<FRM_HEAD*>(std::next(ptrFormsHeader));
		auto const forms      = gsl::span {ptrForms, ClipFormsCount};
		auto const formOffset = wrap::toUnsigned(formList.size());
		for (auto& cForm : forms) {
		  formList.push_back(cForm);
		  formList.back().attribute = gsl::narrow_cast<decltype(cForm.attribute)>(cForm.attribute & NFRMLMSK) |
		                              gsl::narrow_cast<decltype(cForm.attribute)>(ActiveLayer << 1U);
		}
		auto* ptrFormVertices = convertFromPtr<F_POINT*>(wrap::next(ptrForms, ClipFormsCount));
		auto  currentVertex   = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  auto&      form   = formList.operator[](offset);
		  form.vertexIndex  = wrap::toUnsigned(Instance->formVertices.size());
		  auto const formVertices =
		      gsl::span {ptrFormVertices, wrap::toSize(currentVertex + form.vertexCount)};
		  Instance->formVertices.insert(Instance->formVertices.end(),
		                                wrap::next(formVertices.begin(), currentVertex),
		                                formVertices.end());
		  currentVertex += form.vertexCount;
		}
		auto* ptrGuides  = convertFromPtr<SAT_CON*>(wrap::next(ptrFormVertices, currentVertex));
		auto  guideCount = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  if (auto& form = formList.operator[](offset); form.type == SAT && form.satinGuideCount != 0U) {
			guideCount += form.satinGuideCount;
		  }
		}
		auto       currentGuide = 0U;
		auto const guides       = gsl::span {ptrGuides, guideCount};
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  if (auto& form = formList.operator[](offset); form.type == SAT && form.satinGuideCount != 0U) {
			form.satinGuideIndex = satin::adsatk(form.satinGuideCount);
			auto itGuide         = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex);
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
		  auto&      form   = formList.operator[](offset);
		  // clang-format on
		  if (form.isClipX()) {
			clipCount += form.clipCount;
		  }
		  if (form.isEdgeClipX()) {
			clipCount += form.clipEntries;
		  }
		}
		auto       currentClip = 0U;
		auto const clipData    = gsl::span {ptrClipData, clipCount};
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  // clang-format off
		  auto const offset = formOffset + iForm;
		  auto&      form   = formList.operator[](offset);
		  // clang-format on
		  if (form.isClipX()) {
			form.clipIndex   = thred::adclp(form.clipCount);
			auto offsetStart = wrap::next(Instance->clipPoints.begin(), form.clipIndex);
			for (auto iClip = 0U; iClip < form.clipCount; ++iClip) {
			  *offsetStart = clipData[currentClip++];
			  ++offsetStart;
			}
		  }
		  if (form.isEdgeClipX()) {
			form.borderClipData = thred::adclp(form.clipEntries);
			auto offsetStart    = wrap::next(Instance->clipPoints.begin(), form.borderClipData);
			for (auto iClip = 0U; iClip < form.clipEntries; ++iClip) {
			  *offsetStart = clipData[currentClip++];
			  ++offsetStart;
			}
		  }
		}
		auto* ptrTextureSource = convertFromPtr<TX_PNT*>(wrap::next(ptrClipData, currentClip));
		auto  textureCount     = size_t {};
		for (auto  spForms = std::ranges::subrange(wrap::next(formList.begin(), formOffset),
                                                  wrap::next(wrap::next(formList.begin(), formOffset), ClipFormsCount));
		     auto& form : spForms) {
		  if (form.isTexture()) {
			textureCount += form.texture.count;
			form.texture.index +=
			    gsl::narrow<decltype(form.texture.index)>(Instance->texturePointsBuffer.size());
		  }
		}
		auto const textureSource = gsl::span {ptrTextureSource, textureCount};
		Instance->texturePointsBuffer.insert(
		    Instance->texturePointsBuffer.end(), textureSource.begin(), textureSource.end());
		GlobalUnlock(clipMemory);
		SelectedFormsRect.top = SelectedFormsRect.right = LOWLONG;
		SelectedFormsRect.bottom = SelectedFormsRect.left = BIGLONG;
		thred::ratsr();
		Instance->selectedFormList.clear();
		Instance->selectedFormList.reserve(ClipFormsCount);
		for (auto index = 0U; index < ClipFormsCount; ++index) {
		  form::fselrct(formOffset + index);
		  Instance->selectedFormList.push_back(formOffset + index);
		}
		wrap::narrow_cast(SelectedFormsSize.x, SelectedFormsRect.right - SelectedFormsRect.left);
		wrap::narrow_cast(SelectedFormsSize.y, SelectedFormsRect.bottom - SelectedFormsRect.top);
		Instance->stateMap.set(StateFlag::INIT);
		auto& formLines = Instance->formLines;
		formLines.resize(SQPNTS);
		formLines[0].x = formLines[3].x = formLines[4].x = SelectedFormsRect.left;
		formLines[1].x = formLines[2].x = SelectedFormsRect.right;
		formLines[0].y = formLines[1].y = formLines[4].y = SelectedFormsRect.top;
		formLines[2].y = formLines[3].y = SelectedFormsRect.bottom;
		Instance->stateMap.set(StateFlag::SHOSTRTCH);
		thred::strtchbox(stretchBoxLine);
		FormMoveDelta = F_POINT {wrap::toFloat((SelectedFormsRect.right - SelectedFormsRect.left) / 2),
		                         wrap::toFloat((SelectedFormsRect.bottom - SelectedFormsRect.top) / 2)};
		Instance->stateMap.set(StateFlag::MOVFRMS);
		Instance->stateMap.set(StateFlag::FUNSCLP);
	  }
	  else {
		if (auto* ptrClipFormHeader = gsl::narrow_cast<FORM_CLIP*>(clipPointer);
		    ptrClipFormHeader->clipType == CLP_FRM) {
		  FormMoveDelta = F_POINT {};
		  Instance->stateMap.set(StateFlag::FUNCLP);
		  auto formIter = ptrClipFormHeader->form;
		  formIter.attribute = gsl::narrow_cast<decltype(formIter.attribute)>(formIter.attribute & NFRMLMSK) |
		                       gsl::narrow_cast<decltype(formIter.attribute)>(ActiveLayer << 1U);
		  formIter.vertexIndex   = wrap::toUnsigned(Instance->formVertices.size());
		  auto*      ptrVertices = convertFromPtr<F_POINT*>(std::next(ptrClipFormHeader));
		  auto const vertices    = gsl::span {ptrVertices, formIter.vertexCount};
		  Instance->formVertices.insert(Instance->formVertices.end(), vertices.begin(), vertices.end());
		  auto* ptrGuides = convertFromPtr<SAT_CON*>(wrap::next(ptrVertices, formIter.vertexCount));
		  if (formIter.type == SAT && formIter.satinGuideCount != 0U) {
			auto const guides        = gsl::span {ptrGuides, formIter.satinGuideCount};
			formIter.satinGuideIndex = wrap::toUnsigned(Instance->satinGuides.size());
			Instance->satinGuides.insert(Instance->satinGuides.end(), guides.begin(), guides.end());
		  }
		  auto* ptrClipData = convertFromPtr<F_POINT*>(wrap::next(ptrGuides, formIter.satinGuideCount));
		  auto clipCount = 0U;
		  if (formIter.isClipX()) {
			auto const clipData = gsl::span {ptrClipData, formIter.clipCount};
			formIter.clipIndex  = wrap::toUnsigned(Instance->clipPoints.size());
			Instance->clipPoints.insert(Instance->clipPoints.end(), clipData.begin(), clipData.end());
			clipCount += formIter.clipCount;
		  }
		  if (formIter.isEdgeClipX()) {
			ptrClipData             = convertFromPtr<F_POINT*>(wrap::next(ptrClipData, clipCount));
			auto const clipData     = gsl::span {ptrClipData, formIter.clipEntries};
			formIter.borderClipData = wrap::toUnsigned(Instance->clipPoints.size());
			Instance->clipPoints.insert(Instance->clipPoints.end(), clipData.begin(), clipData.end());
			clipCount += formIter.clipEntries;
		  }
		  if (formIter.isTexture()) {
			auto* ptrTextureSource   = convertFromPtr<TX_PNT*>(wrap::next(ptrClipData, clipCount));
			auto const textureSource = gsl::span {ptrTextureSource, formIter.texture.count};
			wrap::narrow(formIter.texture.index, Instance->texturePointsBuffer.size());
			Instance->texturePointsBuffer.insert(
			    Instance->texturePointsBuffer.end(), textureSource.begin(), textureSource.end());
		  }
		  if (formIter.type == FRMLINE) {
			form::setNewFormVertexCount(formIter.vertexCount);
		  }
		  else {
			form::setNewFormVertexCount(formIter.vertexCount + 1U);
		  }
		  formList.push_back(formIter);
		  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
		  Instance->stateMap.set(StateFlag::INIT);
		  form::unfrm();
		  thred::duzrat();
		  form::setmfrm(ClosestFormToCursor);
		  Instance->stateMap.set(StateFlag::SHOFRM);
		  form::dufrm();
		}
		GlobalUnlock(clipMemory);
	  }
	}
	CloseClipboard();
  }
  else {
	clipMemory = getPCDClipMemory();
	if (clipMemory != nullptr) {
	  thred::redclp(clipMemory);
	  thred::clpbox();
	  Instance->stateMap.set(StateFlag::CLPSHO);
	}
	CloseClipboard();
  }
  retflag = false;
  return {};
}

void tfc::txtclp(FRM_HEAD& textureForm) {
  auto const thrEdClip = registerThredClipFormat();
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
  auto* clipForm = getClipForm(clipMemory);
  if (nullptr == clipForm) {
	GlobalUnlock(clipMemory);
	return;
  }
  textureForm           = *clipForm;
  auto*      vertices   = convertFromPtr<F_POINT*>(std::next(clipForm));
  auto const spVertices = gsl::span {vertices, textureForm.vertexCount};
  Instance->angledFormVertices.clear();
  Instance->angledFormVertices.insert(
      Instance->angledFormVertices.end(), spVertices.begin(), spVertices.end());
  textureForm.vertexIndex = 0;
  Instance->stateMap.reset(StateFlag::TXTLIN);
  Instance->stateMap.set(StateFlag::TXTCLP);
  Instance->stateMap.set(StateFlag::TXTMOV);
  texture::setxfrm();
  texture::setTxtCurLoc(POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y});
  GlobalUnlock(clipMemory);
  Instance->stateMap.set(StateFlag::RESTCH);
  Instance->stateMap.reset(StateFlag::WASWROT);
}

void tfc::fpUnClip() {
  unpclp();
  setpclp();
  Instance->stateMap.set(StateFlag::SHOP);
  dupclp();
}

void tfc::lodclp(uint32_t iStitch) {
  auto const& clipBuffer = Instance->clipBuffer;

  Instance->stitchBuffer.insert(
      wrap::next(Instance->stitchBuffer.begin(), iStitch), clipBuffer.size(), F_POINT_ATTR {});
  ClosestPointIndex  = iStitch;
  auto const originX = wrap::toFloat(ClipOrigin.x);
  auto const originY = wrap::toFloat(ClipOrigin.y);
  for (auto const& clip : clipBuffer) {
	Instance->stitchBuffer.operator[](iStitch++) =
	    F_POINT_ATTR {clip.x + originX,
	                  clip.y + originY,
	                  (clip.attribute & COLMSK) | gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT) | NOTFRM};
  }
  GroupStitchIndex = iStitch - 1U;
  Instance->stateMap.set(StateFlag::GRPSEL);
  if (!Instance->stitchBuffer.empty()) {
	Instance->stateMap.set(StateFlag::INIT);
  }
}

void tfc::fClipInit() noexcept {
  FormClipInstance = FCLIP_SINGLE::getInstance();
}
