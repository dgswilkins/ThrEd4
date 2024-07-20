// Local Headers
#include "stdafx.h"
#include "displayText.h"
#include "EnumMap.h"
#include "formHeader.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "repair.h"
#include "Resources/resource.h"
#include "satin.h"
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
#pragma warning(pop)

// Standard Libraries
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

enum Repair : uint32_t { BADFLT = 1U, BADCLP = 1U << 1U, BADSAT = 1U << 2U, BADTX = 1U << 3U };

class BAD_COUNTS
{
  public:
  uint32_t attribute {};
  uint32_t flt {};
  uint32_t clip {};
  uint32_t guideCount {};
  uint32_t tx {};
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

void ri::adbad(std::wstring& repairMessage, uint32_t const code, uint32_t const count) {
  auto fmtStr = displayText::loadStr(code);
  fmtStr += displayText::format(IDS_NOTREP, count);
  repairMessage += fmtStr;
}

void repair::lodchk() {
  thred::delinf();
  if (FormList->empty()) {
	return;
  }
  for (auto& form : *FormList) {
	if (form.type == 0U) {
	  form.type = FRMFPOLY;
	}
	else {
	  if (form.type == FRMLINE) {
		if (form.fillType != CONTF) {
		  form.fillType  = 0;
		  form.clipCount = 0;
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
  auto formMap = boost::dynamic_bitset(FormList->size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  for (auto& stitch : *StitchBuffer) {
	if (auto const attribute = stitch.attribute; (attribute & TYPMSK) == TYPFRM) {
	  if (auto const tform = (attribute & FRMSK) >> FRMSHFT; tform < formMap.size()) {
		formMap.set(tform);
	  }
	  else {
		stitch.attribute &= NFRMSK & NTYPMSK;
		stitch.attribute |= NOTFRM;
	  }
	}
  }
  for (auto iForm = 0U; auto& form : *FormList) {
	if (!formMap.test(iForm)) {
	  form.fillType = 0;
	}
	++iForm;
  }
  formMap.reset();
  for (auto const& stitch : *StitchBuffer) {
	if (auto const attribute = stitch.attribute; (attribute & TYPBRD) != 0U) {
	  formMap.set((attribute & FRMSK) >> FRMSHFT);
	}
  }
  for (auto iForm = 0U; auto& form : *FormList) {
	if (!formMap.test(iForm)) {
	  form.edgeType = 0;
	}
	++iForm;
  }
}

void ri::chkclp(FRM_HEAD const& formHeader, BAD_COUNTS& badData) noexcept {
  if (badData.clip == formHeader.clipIndex) {
	badData.clip += formHeader.clipCount;
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
  if (badData.guideCount == form.satinGuideIndex) {
	badData.guideCount += form.satinGuideCount;
  }
  else {
	badData.attribute |= BADSAT;
  }
}

void ri::chkTxt(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept {
  if (badData.tx == form.texture.index) {
	badData.tx += form.texture.count;
  }
  else {
	badData.attribute |= BADTX;
  }
}

auto ri::frmchkfn() noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t {
  auto badData = BAD_COUNTS {};
  if (!FormList->empty()) {
	for (auto& form : *FormList) {
	  if ((badData.attribute & BADFLT) == 0U) {
		if (form.vertexCount == 0U) {
		  badData.attribute |= BADFLT;
		}
		chkVrtx(form, badData);
	  }
	  if ((badData.attribute & BADCLP) == 0U) {
		if (form.isClip()) {
		  chkclp(form, badData);
		}
		if (form.isEdgeClip()) {
		  chkeclp(form, badData);
		}
	  }
	  if ((badData.attribute & BADSAT) == 0U) {
		if (form.type == SAT && form.satinGuideCount != 0U) {
		  chkSat(form, badData);
		}
	  }
	  if ((badData.attribute & BADTX) == 0U) {
		if (form.isTexture()) {
		  chkTxt(form, badData);
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
	badData.clip += form.clipCount;
  }
  if (form.isEdgeClip()) {
	badData.clip += form.clipEntries;
  }
  if (form.type == SAT && form.satinGuideCount != 0U) {
	badData.guideCount += form.satinGuideCount;
  }
  if (form.isTexture()) {
	badData.tx += form.texture.count;
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
  for (auto const& iForm : formList) {
	if (iForm.vertexCount != 0U) {
	  formList[iDestination++] = iForm;
	}
  }
  formList.resize(iDestination);
  auto vertexPoint = std::vector<F_POINT> {};
  auto iVertex     = 0U;
  for (auto iForm = 0U; auto& form : formList) {
	if (FormVertices->size() >= wrap::toSize(form.vertexIndex) + form.vertexCount) {
	  auto const startVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	  auto const endVertex   = wrap::next(startVertex, form.vertexCount);
	  vertexPoint.insert(vertexPoint.end(), startVertex, endVertex);
	  form.vertexIndex = iVertex;
	  iVertex += form.vertexCount;
	  bcup(form, badData);
	  ++iForm;
	  continue;
	}
	if (form.vertexIndex < FormVertices->size()) {
	  wrap::narrow(form.vertexCount, FormVertices->size() - form.vertexIndex);
	  satin::delsac(iForm);
	  auto const startVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	  auto const endVertex   = wrap::next(startVertex, form.vertexCount);
	  vertexPoint.insert(vertexPoint.end(), startVertex, endVertex);
	  bcup(form, badData);
	  ++iForm;
	  continue;
	}
	FormList->resize(iForm);
	ClipPoints->resize(badData.clip);
	SatinGuides->resize(badData.guideCount);
	TexturePointsBuffer->resize(badData.tx);
	chkfstch();
	adbad(repairMessage, IDS_FRMDAT, wrap::toUnsigned(FormList->size()));
	break;
  }
  *FormVertices = std::move(vertexPoint);
}

void ri::checkClip(const uint32_t&       clipDifference,
                   FRM_HEAD&             form,
                   std::vector<F_POINT>& clipPoint,
                   unsigned int&         clipCount,
                   unsigned int&         badClipCount) {
  if (wrap::toSize(clipDifference) + form.clipCount < ClipPoints->size()) {
	clipPoint.resize(clipPoint.size() + form.clipCount);
	auto const startClip   = wrap::next(ClipPoints->cbegin(), form.clipIndex);
	auto const endClip     = wrap::next(startClip, form.clipCount);
	auto const destination = wrap::next(clipPoint.begin(), clipCount);
	std::copy(startClip, endClip, destination);
	form.clipIndex = clipCount;
	clipCount += form.clipCount;
	return;
  }
  if (clipDifference < ClipPoints->size()) {
	form.clipCount = wrap::toUnsigned(FormVertices->size() - clipDifference);
	clipPoint.resize(clipPoint.size() + form.clipCount);
	auto const startClip   = wrap::next(ClipPoints->cbegin(), form.clipIndex);
	auto const endClip     = wrap::next(startClip, form.clipCount);
	auto const destination = wrap::next(clipPoint.begin(), clipCount);
	std::copy(startClip, endClip, destination);
	form.clipIndex = clipCount;
	clipCount += form.clipCount;
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
  for (auto& form : *FormList) {
	// NOLINTBEGIN(readability-avoid-nested-conditional-operator)
	auto const clipDifference = form.isClip()       ? form.clipIndex
	                            : form.isEdgeClip() ? form.borderClipData
	                                                  : 0U;
	// NOLINTEND(readability-avoid-nested-conditional-operator)
	if (form.isClip()) {
	  checkClip(clipDifference, form, clipPoint, clipCount, badClipCount);
	}
	if (form.isEdgeClip()) {
	  checkEdgeClip(clipDifference, form, clipPoint, clipCount, badClipCount);
	}
  }
  std::ranges::copy(clipPoint, ClipPoints->begin());
  if (badClipCount != 0U) {
	adbad(repairMessage, IDS_CLPDAT, badClipCount);
  }
}

void ri::repsat() {
  auto guideCount = 0U;
  auto badData    = BAD_COUNTS {};
  for (auto& form : *FormList) {
	if (form.type != SAT || form.satinGuideCount == 0U) {
	  continue;
	}
	auto const guideDifference = form.satinGuideIndex;
	if (FormVertices->size() > wrap::toSize(guideDifference) + form.vertexCount) {
	  auto const startGuide  = wrap::next(SatinGuides->cbegin(), form.satinGuideIndex);
	  auto const endGuide    = wrap::next(startGuide, form.satinGuideCount);
	  auto const destination = wrap::next(SatinGuides->begin(), guideCount);
	  std::copy(startGuide, endGuide, destination);
	  form.satinGuideIndex = guideCount;
	  guideCount += form.satinGuideCount;
	  bcup(form, badData);
	  continue;
	}
	if (guideDifference < SatinGuides->size()) {
	  wrap::narrow(form.satinGuideCount, SatinGuides->size() - guideDifference);
	  auto const startGuide  = wrap::next(SatinGuides->cbegin(), form.satinGuideIndex);
	  auto const endGuide    = wrap::next(startGuide, form.satinGuideCount);
	  auto const destination = wrap::next(SatinGuides->begin(), guideCount);
	  std::copy(startGuide, endGuide, destination);
	  bcup(form, badData);
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
  for (auto& iForm : *FormList) {
	if (!iForm.isTexture()) {
	  continue;
	}
	if (wrap::toUnsigned(TexturePointsBuffer->size()) >
	    wrap::toUnsigned(iForm.texture.index) + iForm.texture.count) {
	  auto const startTexture = wrap::next(TexturePointsBuffer->cbegin(), iForm.texture.index);
	  auto const endTexture   = wrap::next(startTexture, iForm.texture.count);
	  auto const destination  = wrap::next(TexturePointsBuffer->begin(), textureCount);
	  std::copy(startTexture, endTexture, destination);
	  wrap::narrow(iForm.texture.index, textureCount);
	  textureCount += iForm.texture.count;
	  bcup(iForm, badData);
	  continue;
	}
	if (TexturePointsBuffer->size() > iForm.texture.index) {
	  wrap::narrow(iForm.texture.count, TexturePointsBuffer->size() - iForm.texture.index);
	  auto const startTexture = wrap::next(TexturePointsBuffer->cbegin(), iForm.texture.index);
	  auto const endTexture   = wrap::next(startTexture, iForm.texture.count);
	  auto const destination  = wrap::next(TexturePointsBuffer->begin(), textureCount);
	  std::copy(startTexture, endTexture, destination);
	  wrap::narrow(iForm.texture.index, textureCount);
	  bcup(iForm, badData);
	  textureCount = badData.tx;
	  continue;
	}
	iForm.fillType = 0;
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
  lodchk();
  StateMap->set(StateFlag::RESTCH);
  displayText::shoMsg(repairMessage, false);
}

void ri::datmsg(uint32_t const code) {
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
		repar();
	  }
	  break;
	}
	case 3: {
	  if (code != 0U) {
		repar();
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
