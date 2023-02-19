// Local Headers
#include "stdafx.h"
#include "displayText.h"
#include "globals.h"
#include "repair.h"
#include "satin.h"
#include "thred.h"

constexpr auto BADFLT = 1U;
constexpr auto BADCLP = 1U << 1U;
constexpr auto BADSAT = 1U << 2U;
constexpr auto BADTX  = 1U << 3U;

class BAD_COUNTS
{
  public:
  uint32_t attribute {};
  uint32_t flt {};
  uint32_t clip {};
  uint32_t guideCount {}; // ToDo - is this an accurate description?
  uint32_t tx {};

  // constexpr BAD_COUNTS() noexcept = default;
  // BAD_COUNTS(BAD_COUNTS const&) = default;
  // BAD_COUNTS(BAD_COUNTS&&) = default;
  // BAD_COUNTS& operator=(BAD_COUNTS const& rhs) = default;
  // BAD_COUNTS& operator=(BAD_COUNTS&&) = default;
  //~BAD_COUNTS() = default;
};

// repair internal namespace
namespace ri {
void adbad(std::wstring& repairMessage, uint32_t code, uint32_t count);
void bcup(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
void checkClip(const uint32_t&       clipDifference,
               FRM_HEAD&             form,
               std::vector<F_POINT>& clipPoint,
               unsigned int&         clipCount,
               unsigned int&         badClipCount);
void checkEdgeClip(const uint32_t&       clipDifference,
                   FRM_HEAD&             form,
                   std::vector<F_POINT>& clipPoint,
                   unsigned int&         clipCount,
                   unsigned int&         badClipCount);
void chkclp(FRM_HEAD const& formHeader, BAD_COUNTS& badData) noexcept;
void chkeclp(FRM_HEAD const& formHeader, BAD_COUNTS& badData) noexcept;
void chkfstch() noexcept;
void chkSat(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
void chkTxt(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
void chkVrtx(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
void datmsg(uint32_t code);
auto frmchkfn() noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t;
void repclp(std::wstring& repairMessage);
void repflt(std::wstring& repairMessage);
void repsat();
void reptx();
} // namespace ri

void ri::adbad(std::wstring& repairMessage, uint32_t code, uint32_t count) {
  auto fmtStr = displayText::loadStr(code);
  // NOLINTNEXTLINE(clang-diagnostic-sign-conversion)
  fmtStr += fmt::format(fmt::runtime(displayText::loadStr(IDS_NOTREP)), count);
  repairMessage += fmtStr;
}

void repair::lodchk() {
  thred::delinf();
  if (FormList->empty()) {
	return;
  }
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	if (form.type == 0U) {
	  form.type = FRMFPOLY;
	}
	else {
	  if (form.type == FRMLINE) {
		if (form.fillType != CONTF) {
		  form.fillType                = 0;
		  form.lengthOrCount.clipCount = 0;
		}
	  }
	}
	form.outline();
	if (form.maxFillStitchLen == 0.0F) {
	  form.maxFillStitchLen = IniFile.maxStitchLength;
	}
	if (form.maxBorderStitchLen == 0.0F) {
	  form.maxBorderStitchLen = IniFile.maxStitchLength;
	}
  }
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto& stitch : *StitchBuffer) {
	auto const attribute = stitch.attribute;
	if ((attribute & TYPMSK) == TYPFRM) {
	  auto const tform = (attribute & FRMSK) >> FRMSHFT;
	  if (tform < formMap.size()) {
		formMap.set(tform);
	  }
	  else {
		stitch.attribute &= (NFRMSK & NTYPMSK);
		stitch.attribute |= NOTFRM;
	  }
	}
  }
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	if (!formMap.test(iForm)) {
	  form.fillType = 0;
	}
  }
  formMap.reset();
  for (auto const& stitch : *StitchBuffer) {
	auto const attribute = stitch.attribute;
	if ((attribute & TYPBRD) != 0U) {
	  formMap.set((attribute & FRMSK) >> FRMSHFT);
	}
  }
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	if (!formMap.test(iForm)) {
	  form.edgeType = 0;
	}
  }
}

void ri::chkclp(FRM_HEAD const& formHeader, BAD_COUNTS& badData) noexcept {
  if (badData.clip == formHeader.angleOrClipData.clip) {
	badData.clip += formHeader.lengthOrCount.clipCount;
  }
  else {
	badData.attribute |= BADCLP;
  }
}

void ri::chkeclp(FRM_HEAD const& formHeader, BAD_COUNTS& badData) noexcept {
  if (badData.clip == formHeader.borderClipData) {
	badData.clip += formHeader.clipEntries;
  }
  else {
	badData.attribute |= BADCLP;
  }
}

void ri::chkVrtx(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept {
  if (badData.flt == form.vertexIndex) {
	badData.flt += form.vertexCount;
  }
  else {
	badData.attribute |= BADFLT;
  }
}

void ri::chkSat(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept {
  if (badData.guideCount == form.satinOrAngle.guide) {
	badData.guideCount += form.satinGuideCount;
  }
  else {
	badData.attribute |= BADSAT;
  }
}

void ri::chkTxt(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept {
  if (badData.tx == form.fillInfo.texture.index) {
	badData.tx += form.fillInfo.texture.count;
  }
  else {
	badData.attribute |= BADTX;
  }
}

auto ri::frmchkfn() noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t {
  auto badData = BAD_COUNTS {};
  if (!FormList->empty()) {
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  auto& form = FormList->operator[](iForm);
	  if ((badData.attribute & BADFLT) == 0U) {
		if (form.vertexCount == 0U) {
		  badData.attribute |= BADFLT;
		}
		ri::chkVrtx(form, badData);
	  }
	  if ((badData.attribute & BADCLP) == 0U) {
		if (form.isClip()) {
		  ri::chkclp(form, badData);
		}
		if (form.isEdgeClip()) {
		  ri::chkeclp(form, badData);
		}
	  }
	  if ((badData.attribute & BADSAT) == 0U) {
		if (form.type == SAT && (form.satinGuideCount != 0U)) {
		  ri::chkSat(form, badData);
		}
	  }
	  if ((badData.attribute & BADTX) == 0U) {
		if (form.isTexture()) {
		  ri::chkTxt(form, badData);
		}
	  }
	  if (badData.attribute == (BADFLT | BADCLP | BADSAT | BADTX)) {
		break;
	  }
	}
	if (badData.flt != FormVertices->size()) {
	  badData.attribute |= BADFLT;
	}
	if (badData.clip != ClipPoints->size()) {
	  badData.attribute |= BADCLP;
	}
	if (badData.guideCount != SatinGuides->size()) {
	  badData.attribute |= BADSAT;
	}
	if (badData.tx != TexturePointsBuffer->size()) {
	  badData.attribute |= BADTX;
	}
  }
  return badData.attribute;
}

void ri::bcup(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept {
  if (form.isClip()) {
	badData.clip += form.lengthOrCount.clipCount;
  }
  if (form.isEdgeClip()) {
	badData.clip += form.clipEntries;
  }
  if (form.type == SAT) {
	badData.guideCount += form.satinGuideCount;
  }
  if (form.isTexture()) {
	badData.tx += form.fillInfo.texture.count;
  }
}

void ri::chkfstch() noexcept {
  auto const codedFormIndex = FormList->size() << FRMSHFT;
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & FRMSK) >= codedFormIndex) {
	  stitch.attribute = NOTFRM;
	}
  }
}

void ri::repflt(std::wstring& repairMessage) {
  auto  iDestination = 0U;
  auto  badData      = BAD_COUNTS {};
  auto& formList     = *FormList;
  for (auto &form : *FormList) {
	if (form.vertexCount != 0U) {
	  formList[iDestination++] = form;
	}
  }
  formList.resize(iDestination);
  auto vertexPoint = std::vector<F_POINT> {};
  auto iVertex     = 0U;
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = formList[iForm];
	if (FormVertices->size() >= wrap::toSize(form.vertexIndex) + form.vertexCount) {
	  auto startVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	  auto endVertex   = wrap::next(startVertex, form.vertexCount);
	  vertexPoint.insert(vertexPoint.end(), startVertex, endVertex);
	  form.vertexIndex = iVertex;
	  iVertex += form.vertexCount;
	  ri::bcup(form, badData);
	  continue;
	}
	if (form.vertexIndex < FormVertices->size()) {
	  wrap::narrow(form.vertexCount, FormVertices->size() - form.vertexIndex);
	  satin::delsac(iForm);
	  auto startVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	  auto endVertex   = wrap::next(startVertex, form.vertexCount);
	  vertexPoint.insert(vertexPoint.end(), startVertex, endVertex);
	  ri::bcup(form, badData);
	  continue;
	}
	FormList->resize(iForm);
	ClipPoints->resize(badData.clip);
	SatinGuides->resize(badData.guideCount);
	TexturePointsBuffer->resize(badData.tx);
	ri::chkfstch();
	ri::adbad(repairMessage, IDS_FRMDAT, wrap::toUnsigned(FormList->size()));
	break;
  }
  *FormVertices = std::move(vertexPoint);
}

void ri::checkClip(const uint32_t&       clipDifference,
                   FRM_HEAD&             form,
                   std::vector<F_POINT>& clipPoint,
                   unsigned int&         clipCount,
                   unsigned int&         badClipCount) {
  if (wrap::toSize(clipDifference) + form.lengthOrCount.clipCount < ClipPoints->size()) {
	clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
	auto const startClip   = wrap::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
	auto const endClip     = wrap::next(startClip, form.lengthOrCount.clipCount);
	auto const destination = wrap::next(clipPoint.begin(), clipCount);
	std::copy(startClip, endClip, destination);
	form.angleOrClipData.clip = clipCount;
	clipCount += form.lengthOrCount.clipCount;
	return;
  }
  if (clipDifference < ClipPoints->size()) {
	form.lengthOrCount.clipCount = wrap::toUnsigned(FormVertices->size() - clipDifference);
	clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
	auto const startClip   = wrap::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
	auto const endClip     = wrap::next(startClip, form.lengthOrCount.clipCount);
	auto const destination = wrap::next(clipPoint.begin(), clipCount);
	std::copy(startClip, endClip, destination);
	form.angleOrClipData.clip = clipCount;
	clipCount += form.lengthOrCount.clipCount;
	return;
  }
  ++badClipCount;
  form.fillType = 0;
}

void ri::checkEdgeClip(const uint32_t&       clipDifference,
                       FRM_HEAD&             form,
                       std::vector<F_POINT>& clipPoint,
                       unsigned int&         clipCount,
                       unsigned int&         badClipCount) {
  if (wrap::toSize(clipDifference) + form.clipEntries < ClipPoints->size()) {
	clipPoint.resize(clipPoint.size() + form.clipEntries);
	auto const startClip   = wrap::next(ClipPoints->cbegin(), form.borderClipData);
	auto const endClip     = wrap::next(startClip, form.clipEntries);
	auto const destination = wrap::next(clipPoint.begin(), clipCount);
	std::copy(startClip, endClip, destination);
	form.borderClipData = clipCount;
	clipCount += form.clipEntries;
	return;
  }
  if (clipDifference < ClipPoints->size()) {
	wrap::narrow(form.clipEntries, FormVertices->size() - clipDifference);
	clipPoint.resize(clipPoint.size() + form.clipEntries);
	auto const startClip   = wrap::next(ClipPoints->cbegin(), form.borderClipData);
	auto const endClip     = wrap::next(startClip, form.clipEntries);
	auto const destination = wrap::next(clipPoint.begin(), clipCount);
	std::copy(startClip, endClip, destination);
	form.borderClipData = clipCount;
	clipCount += form.clipEntries;
	return;
  }
  ++badClipCount;
  form.fillType = 0;
}

void ri::repclp(std::wstring& repairMessage) {
  auto badClipCount = 0U;
  auto clipCount    = 0U;
  auto clipPoint    = std::vector<F_POINT> {};
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);

	auto const clipDifference = (form.isClip())       ? form.angleOrClipData.clip
	                            : (form.isEdgeClip()) ? form.borderClipData
	                                                  : 0U;
	if (form.isClip()) {
	  checkClip(clipDifference, form, clipPoint, clipCount, badClipCount);
	}
	if (form.isEdgeClip()) {
	  checkEdgeClip(clipDifference, form, clipPoint, clipCount, badClipCount);
	}
  }
  std::ranges::copy(clipPoint, ClipPoints->begin());
  if (badClipCount != 0U) {
	ri::adbad(repairMessage, IDS_CLPDAT, badClipCount);
  }
}

void ri::repsat() {
  auto guideCount = 0U;
  auto badData    = BAD_COUNTS {};
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	if (form.type != SAT) {
	  continue;
	}
	auto const guideDifference = form.satinOrAngle.guide;
	if (FormVertices->size() > wrap::toSize(guideDifference) + form.vertexCount) {
	  auto const startGuide  = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	  auto const endGuide    = wrap::next(startGuide, form.satinGuideCount);
	  auto const destination = wrap::next(SatinGuides->begin(), guideCount);
	  std::copy(startGuide, endGuide, destination);
	  form.satinOrAngle.guide = guideCount;
	  guideCount += form.satinGuideCount;
	  ri::bcup(form, badData);
	  continue;
	}
	if (guideDifference < SatinGuides->size()) {
	  wrap::narrow(form.satinGuideCount, SatinGuides->size() - guideDifference);
	  auto const startGuide  = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	  auto const endGuide    = wrap::next(startGuide, form.satinGuideCount);
	  auto const destination = wrap::next(SatinGuides->begin(), guideCount);
	  std::copy(startGuide, endGuide, destination);
	  ri::bcup(form, badData);
	  continue;
	}
	guideCount           = badData.guideCount;
	form.satinGuideCount = 0;
  }
  SatinGuides->resize(guideCount);
}

void ri::reptx() {
  auto textureCount = 0U;
  auto badData      = BAD_COUNTS {};
  for (auto& formIter : *FormList) {
	if (!formIter.isTexture()) {
	  continue;
	}
	if (wrap::toUnsigned(TexturePointsBuffer->size()) >
	    wrap::toUnsigned(formIter.fillInfo.texture.index) + formIter.fillInfo.texture.count) {
	  auto const startTexture = wrap::next(TexturePointsBuffer->cbegin(), formIter.fillInfo.texture.index);
	  auto const endTexture  = wrap::next(startTexture, formIter.fillInfo.texture.count);
	  auto const destination = wrap::next(TexturePointsBuffer->begin(), textureCount);
	  std::copy(startTexture, endTexture, destination);
	  wrap::narrow(formIter.fillInfo.texture.index, textureCount);
	  textureCount += formIter.fillInfo.texture.count;
	  ri::bcup(formIter, badData);
	  continue;
	}
	if (TexturePointsBuffer->size() > formIter.fillInfo.texture.index) {
	  wrap::narrow(formIter.fillInfo.texture.count,
	               TexturePointsBuffer->size() - formIter.fillInfo.texture.index);
	  auto const startTexture = wrap::next(TexturePointsBuffer->cbegin(), formIter.fillInfo.texture.index);
	  auto const endTexture  = wrap::next(startTexture, formIter.fillInfo.texture.count);
	  auto const destination = wrap::next(TexturePointsBuffer->begin(), textureCount);
	  std::copy(startTexture, endTexture, destination);
	  wrap::narrow(formIter.fillInfo.texture.index, textureCount);
	  ri::bcup(formIter, badData);
	  textureCount = badData.tx;
	  continue;
	}
	formIter.fillType = 0;
  }
  TexturePointsBuffer->resize(textureCount);
}

void repair::repar() {
  auto       repairMessage = std::wstring {};
  auto const repairType    = ri::frmchkfn();
  thred::savdo();
  if ((repairType & BADFLT) != 0U) {
	ri::repflt(repairMessage);
  }
  if ((repairType & BADCLP) != 0U) {
	ri::repclp(repairMessage);
  }
  if ((repairType & BADSAT) != 0U) {
	ri::repsat();
  }
  if ((repairType & BADTX) != 0U) {
	ri::reptx();
  }
  repair::lodchk();
  StateMap->set(StateFlag::RESTCH);
  displayText::shoMsg(repairMessage, false);
}

void ri::datmsg(uint32_t code) {
  auto dataErrorID = wrap::toUnsigned(IDS_BADUKN);
  switch (code) {
	case BADFLT:
	  dataErrorID = IDS_BADFLT;
	  break;
	case BADCLP:
	  dataErrorID = IDS_BADCLP;
	  break;
	case BADSAT:
	  dataErrorID = IDS_BADSAT;
	  break;
	case BADTX:
	  dataErrorID = IDS_BADTX;
	  break;
	default:
	  break;
  }
  displayText::shoMsg(displayText::loadStr(dataErrorID), false);
}

void repair::frmchkx() {
  if (IniFile.dataCheck == 0U) {
	return;
  }
  auto const code = ri::frmchkfn();
  switch (IniFile.dataCheck) {
	case 1: {
	  if (code != 0U) {
		ri::datmsg(code);
	  }
	  break;
	}
	case 2: {
	  if (code != 0U) {
		repair::repar();
	  }
	  break;
	}
	case 3: {
	  if (code != 0U) {
		repair::repar();
		displayText::tabmsg(IDS_DATREP, false);
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in frmchkx: dataCheck [{}]\n", IniFile.dataCheck);
	  break;
	}
  }
}
