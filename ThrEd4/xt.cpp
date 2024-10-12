// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "clip.h"
#include "displayText.h"
#include "EnumMap.h"
#include "form.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "menu.h"
#include "point.h"
#include "Resources/resource.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "fmt/compile.h"
#include "gsl/narrow"
#include "gsl/pointers"
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
#include <basetsd.h>
#include <minwindef.h>
#include <sysinfoapi.h>
#include <windef.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <filesystem>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

class FEATHER
{
  public:
  uint32_t fillType {};          // type of feather fill
  float    ratio {};             // feather ratio
  float    minStitch {};         // smallest stitch allowed
  float    ratioLocal {};        // local feather ratio
  float    formRatio {};         // feather ratio from form
  uint32_t extendedAttribute {}; // extended form attribute
  uint32_t upCount {};           // feather up count
  uint32_t downCount {};         // feather down count
  uint32_t totalCount {};        // up count plus down count
  uint32_t phaseIndex {};
  uint32_t phase {};
  uint32_t countUp {};
  uint32_t countDown {};
  float    globalStep {};
  float    globalPosition {};
  float    globalPhase {};
  float    globalUp {};
  float    globalDown {};
  float    globalRatio {};
};

class INT_INFO
{
  public:
  uint32_t      pins {};
  uint32_t      coloc {};
  uint32_t      layerIndex {};
  uint32_t      start {};
  uint32_t      output {};
  F_POINT_ATTR* highStitchBuffer {};
};

class O_REC
{
  public:
  uint32_t start {};
  uint32_t finish {};
  uint32_t startStitch {};
  uint32_t endStitch {};
  uint32_t color {};
  uint32_t type {};
  uint32_t form {};
  uint32_t otyp {};
};

class SORT_REC
{
  public:
  uint32_t start {};          // start region index
  uint32_t finish {};         // finish region index
  uint32_t count {};          // number of regions in range
  uint32_t currentRegion {};  // current region index
  bool     direction = false; // direction of sort
};

constexpr auto M_AP     = 1U << 1U;
constexpr auto M_CWLK   = 1U << 2U;
constexpr auto M_WALK   = 1U << 3U;
constexpr auto M_UND    = 1U << 4U;
constexpr auto M_FIL    = 1U << 5U;
constexpr auto M_FTH    = 1U << 6U;
constexpr auto M_BRD    = 1U << 7U;
constexpr auto M_APCOL  = 1U << 8U;
constexpr auto M_FCOL   = 1U << 9U;
constexpr auto M_FTHCOL = 1U << 10U;
constexpr auto M_ECOL   = 1U << 11U;

enum StitchStyles : char {
  TYPE_APPLIQUE = 1, // applique
  TYPE_CWALK,        // center walk
  TYPE_EWALK,        // edge walk
  TYPE_UNDERLAY,     // underlay
  TYPE_FTHR,         // feather
  TYPE_FILL,         // fill
  TYPE_BORDER        // border
};

static constexpr auto STITCH_TYPES = std::array<char, 13> {
    0,             // 0 no type
    TYPE_APPLIQUE, // 1 applique
    TYPE_CWALK,    // 2 center walk
    TYPE_EWALK,    // 3 edge walk
    TYPE_UNDERLAY, // 4 underlay
    0,             // 5 knot
    TYPE_FTHR,     // 6 feather
    0,             // 7 layer
    0,             // 8 layer
    0,             // 9 layer
    0,             // 10 reserved
    TYPE_FILL,     // 11 fill
    TYPE_BORDER,   // 12 border
};

#ifdef _DEBUG

class ATFLD
{
  public:
  uint32_t color {};
  uint32_t form {};
  uint32_t type {};
  uint32_t layer {};
  uint32_t user {};
};

#endif

// xt internal namespace
namespace {
auto DesignSize = F_POINT {};                        // design size
auto ColorOrder = std::array<uint32_t, COLORCNT> {}; // color order adjusted for applique

constexpr auto FSED = uint32_t {1340007303U}; // feather sequence seed

// Definitions
void addNewStitches(INT_INFO& ilData, const FRM_HEAD& form);
void bcolfn(uint32_t formIndex, uint8_t color);
void blenfn(uint32_t formIndex, float length);
void bmaxfn(uint32_t formIndex, float length);
void bminfn(uint32_t formIndex, float length);
auto bpsg() noexcept -> uint32_t;
void bspacfn(uint32_t formIndex, float length);
auto chkasp(F_POINT& point, float aspectRatio, HWND dialog) -> bool;
void chkend(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t code, INT_INFO& ilData);
auto chkrdun(std::vector<uint32_t> const& formFillCounter,
             std::vector<O_REC*> const&   pRecs,
             SORT_REC const&              stitchRecord) noexcept -> bool;
void delwlk(uint32_t code);

#ifdef _DEBUG
void dmprec(std::vector<O_REC*> const& stitchRegion, uint32_t count);
void duatf(uint32_t ind);
#endif

void duint(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t code, INT_INFO& ilData);
auto duprecs(std::vector<F_POINT_ATTR>& tempStitchBuffer, std::vector<O_REC*> const& pRecs, SORT_REC& sortRecord)
    -> uint32_t;

constexpr auto durat(float start, float finish, float featherRatio) -> float;

void durats(uint32_t iSequence, std::vector<F_POINT>& sequence, FEATHER& feather);
void durec(O_REC& record) noexcept(!std::is_same_v<ptrdiff_t, int>);
auto dutyp(uint32_t attribute) noexcept -> uint32_t;
void duxrats(uint32_t start, uint32_t finish, F_POINT& point, float featherRatioLocal) noexcept;

auto CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept -> BOOL;

void fangfn(uint32_t formIndex, float angle);
void fcolfn(uint32_t formIndex, uint8_t color);
void fhifn(uint32_t formIndex, float length);
void findfn(uint32_t formIndex, float indent);
void flenfn(uint32_t formIndex, float length);
void fmaxfn(uint32_t formIndex, float length);
void fminfn(uint32_t formIndex, float length);
void fncwlk(FRM_HEAD& form);
void fnund(uint32_t formIndex, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
void fnwlk(FRM_HEAD& form);
void fritfil(FRM_HEAD const& form, std::vector<F_POINT> const& featherSequence);
void fspacfn(uint32_t formIndex, float spacing);
void fthdfn(uint32_t iSequence, FEATHER& feather);
void fthfn(uint32_t iSequence, FEATHER& feather);
void fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<F_POINT>& featherSequence);
void fthvars(FRM_HEAD const& form, FEATHER& feather);
void fwidfn(uint32_t formIndex, float length);
auto getstxt(int32_t stringIndex, HWND dialog) -> float;
auto gucon(FRM_HEAD const&            form,
           std::vector<F_POINT_ATTR>& buffer,
           F_POINT const&             start,
           F_POINT const&             finish,
           uint32_t                   destination,
           uint32_t                   code) -> uint32_t;
auto handleSetsWMCOMMAND(WPARAM const& wparam, HWND hwndlg) -> bool;
void handleSetsWMINITDIALOG(HWND hwndlg);
auto isfil(FRM_HEAD const& form) noexcept -> bool;
auto lastcol(uint32_t index, F_POINT& point) noexcept -> bool;
auto midpnt(F_POINT const& startPoint, F_POINT const& endPoint) noexcept -> F_POINT;
void notundfn(uint32_t code);
void nudfn(F_RECTANGLE const& designSizeRect) noexcept;
void nurat(FEATHER& feather) noexcept;
auto precjmps(std::vector<F_POINT_ATTR>& tempStitchBuffer, std::vector<O_REC*> const& pRecs, SORT_REC const& sortRecord)
    -> double;
void ratpnt(uint32_t iPoint, uint32_t iNextPoint, F_POINT& point, float featherRatio) noexcept;
auto orComp(gsl::not_null<O_REC const*> record1, gsl::not_null<O_REC const*> record2) -> bool;
auto orfComp(gsl::not_null<O_REC const*> record1, gsl::not_null<O_REC const*> record2) noexcept -> bool;
void ritwlk(FRM_HEAD& form, uint32_t walkMask);
void sadj(F_POINT& point, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept;
void sadj(F_POINT_ATTR& stitch, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept;
void setColorOrder() noexcept;

auto CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

void setstxt(int32_t stringIndex, float value, HWND dialog);
void setundfn(uint32_t code);
auto srtchk(std::vector<O_REC*> const& stitchRegion,
            uint32_t                   count,
            uint32_t&                  badForm) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool;

constexpr auto tim2int(FILETIME time) noexcept -> ULARGE_INTEGER;

void uangfn(uint32_t formIndex, float angle);
void ucolfn(uint32_t formIndex, uint8_t color);
void ulenfn(uint32_t formIndex, float length);
void undclp(float& underlayStitchLen);
void uspacfn(uint32_t formIndex, float spacing);
void xratf(F_POINT const& startPoint, F_POINT const& endPoint, F_POINT& point, float featherRatioLocal) noexcept;

// Functions
constexpr auto tim2int(FILETIME const time) noexcept -> ULARGE_INTEGER {
  auto const largeInt = ULARGE_INTEGER {{.LowPart = time.dwLowDateTime, .HighPart = time.dwHighDateTime}};
  return largeInt;
}

void uangfn(uint32_t const formIndex, float const angle) {
  if (auto& form = Instance->FormList.operator[](formIndex); (form.extendedAttribute & AT_UND) != 0U) {
	form.underlayStitchAngle = angle;
	form::refilfn(formIndex);
  }
}

void ucolfn(uint32_t const formIndex, uint8_t const color) {
  if (auto& form = Instance->FormList.operator[](formIndex);
      (form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	form.underlayColor = color;
	form::refilfn(formIndex);
  }
}

void ulenfn(uint32_t const formIndex, float const length) {
  if (auto& form = Instance->FormList.operator[](formIndex);
      (form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	form.underlayStitchLen = length;
	form::refilfn(formIndex);
  }
}

void undclp(float& underlayStitchLen) {
  auto& clipBuffer = Instance->ClipBuffer;

  clipBuffer.clear();
  clipBuffer.reserve(2);
  ClipRectSize = F_LSIZ {.cx = 0, .cy = underlayStitchLen};
  clipBuffer.emplace_back(0.0F, 00.0F, 0U);
  clipBuffer.emplace_back(0.0F, underlayStitchLen, 0U);
}

void uspacfn(uint32_t const formIndex, float const spacing) {
  if (auto& form = Instance->FormList.operator[](formIndex); (form.extendedAttribute & AT_UND) != 0U) {
	form.underlaySpacing = spacing;
	form::refilfn(formIndex);
  }
}

void xratf(F_POINT const& startPoint, F_POINT const& endPoint, F_POINT& point, float const featherRatioLocal) noexcept {
  point = F_POINT {(((endPoint.x - startPoint.x) * featherRatioLocal) + startPoint.x),
                   (((endPoint.y - startPoint.y) * featherRatioLocal) + startPoint.y)};
}

void addNewStitches(INT_INFO& ilData, FRM_HEAD const& form) {
  auto        code                      = 0U;
  auto&       interleaveSequence        = Instance->InterleaveSequence;
  auto const& interleaveSequenceIndices = Instance->InterleaveSequenceIndices;

  for (auto iSequence = 0U; iSequence < wrap::toUnsigned(interleaveSequenceIndices.size() - 1U); ++iSequence) {
	code = ilData.layerIndex | interleaveSequenceIndices.operator[](iSequence).code |
	       interleaveSequenceIndices.operator[](iSequence).color;
	if ((form.extendedAttribute & AT_STRT) != 0U) {
	  if (!Instance->StateMap.testAndSet(StateFlag::DIDSTRT)) {
		auto itVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + form.fillStart);
		ilData.output +=
		    gucon(form,
		          Instance->StitchBuffer,
		          *itVertex,
		          interleaveSequence.operator[](interleaveSequenceIndices.operator[](ilData.pins).index),
		          ilData.output,
		          code);
	  }
	}
	if (auto colpnt = F_POINT {}; lastcol(iSequence, colpnt)) {
	  ilData.output +=
	      gucon(form,
	            Instance->StitchBuffer,
	            colpnt,
	            interleaveSequence.operator[](interleaveSequenceIndices.operator[](iSequence).index),
	            ilData.output,
	            code);
	}
	auto const nextIndex = interleaveSequenceIndices.operator[](wrap::toSize(iSequence) + 1U).index;
	auto const thisIndex = interleaveSequenceIndices.operator[](iSequence).index;
	for (auto index = thisIndex; index < nextIndex; ++index) {
	  if (ilData.output > 0) {
		auto& interleave = interleaveSequence.operator[](index);
		if (auto const& stitch = Instance->StitchBuffer.operator[](ilData.output - 1U);
		    !util::closeEnough(interleave.x, stitch.x) || !util::closeEnough(interleave.y, stitch.y)) {
		  Instance->StitchBuffer.emplace_back(interleave.x, interleave.y, code);
		  ++ilData.output;
		}
	  }
	  else {
		auto& interleave = interleaveSequence.operator[](index);
		Instance->StitchBuffer.emplace_back(interleave.x, interleave.y, code);
		++ilData.output;
	  }
	}
  }
  chkend(form, Instance->StitchBuffer, code, ilData);
}

void bcolfn(uint32_t const formIndex, uint8_t const color) {
  auto& form = Instance->FormList.operator[](formIndex);
  if (form.edgeType == 0U) {
	return;
  }
  form.borderColor = color;
  form::refilfn(formIndex);
}

void blenfn(uint32_t const formIndex, float const length) {
  auto& form = Instance->FormList.operator[](formIndex);
  if (form.isClip()) {
	return;
  }
  form.stitchLength = length;
  form::refilfn(formIndex);
}

void bmaxfn(uint32_t const formIndex, float const length) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.edgeType != 0U) {
	form.maxBorderStitchLen = length;
	form::refilfn(formIndex);
  }
}

void bminfn(uint32_t const formIndex, float const length) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.edgeType != 0U) {
	form.minBorderStitchLen = length;
	form::refilfn(formIndex);
  }
}

auto bpsg() noexcept -> uint32_t {
  constexpr auto BIT4  = 0x8U;
  constexpr auto BIT31 = 0x40000000U;
  if (PseudoRandomValue == 0U) {
	PseudoRandomValue = FSED;
  }
  auto const testValue = PseudoRandomValue & (BIT4 | BIT31);
  PseudoRandomValue >>= 1U;
  if (testValue == BIT4 || testValue == BIT31) {
	PseudoRandomValue |= BIT31;
  }
  return PseudoRandomValue;
}

void bspacfn(uint32_t const formIndex, float const length) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.edgeType != 0U) {
	form.edgeSpacing = length;
	form::refilfn(formIndex);
  }
}

// ReSharper disable CppParameterMayBeConst
auto chkasp(F_POINT& point, float const aspectRatio, HWND dialog) -> bool {
  point = F_POINT {getstxt(IDC_DESWID, dialog), getstxt(IDC_DESHI, dialog)};
  return util::closeEnough(point.y / point.x, aspectRatio);
}
// ReSharper restore CppParameterMayBeConst

void chkend(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t const code, INT_INFO& ilData) {
  if (isfil(form)) {
	Instance->StateMap.set(StateFlag::ISEND);
	if ((form.extendedAttribute & AT_END) != 0U) {
	  auto const itVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + form.fillEnd);
	  ilData.output +=
	      gucon(form, buffer, Instance->InterleaveSequence.back(), *itVertex, ilData.output, code);
	}
  }
}

auto chkrdun(std::vector<uint32_t> const& formFillCounter,
             std::vector<O_REC*> const&   pRecs,
             SORT_REC const&              stitchRecord) noexcept -> bool {
  for (auto iStitch = stitchRecord.start; iStitch < stitchRecord.finish; ++iStitch) {
	if (pRecs[iStitch]->otyp == formFillCounter[pRecs[iStitch]->form]) {
	  return true;
	}
  }
  return false;
}

void delwlk(uint32_t code) {
  if (!Instance->StitchBuffer.empty()) {
	std::erase_if(Instance->StitchBuffer, [&code](F_POINT_ATTR const& stitch) -> bool {
	  return (stitch.attribute & WLKFMSK) == code;
	});
  }
}

#ifdef _DEBUG

void dmprec(std::vector<O_REC*> const& stitchRegion, uint32_t const count) {
  for (auto iRegion = 0U; iRegion < count; ++iRegion) {
	outDebugString(
	    L"{:4d} attrb: 0x{:08x} form: {:4d} type: {} color: {:2d} start: {:5d} finish: {:5d}\n",
	    iRegion,
	    Instance->StitchBuffer.operator[](stitchRegion[iRegion]->start).attribute,
	    stitchRegion[iRegion]->form,
	    stitchRegion[iRegion]->type,
	    stitchRegion[iRegion]->color,
	    stitchRegion[iRegion]->start,
	    stitchRegion[iRegion]->finish);
  }
}
#endif

#ifdef _DEBUG

void duatf(uint32_t ind) {
  // clang-format off
  auto const attribute       = Instance->StitchBuffer.operator[](ind).attribute;
  auto       attributeFields = ATFLD {.color = (attribute & COLMSK),
									  .form = ((attribute & FRMSK) >> FRMSHFT),
									  .type  = gsl::narrow_cast<uint32_t>(STITCH_TYPES.at(dutyp(attribute))),
									  .layer = (attribute >> LAYSHFT & MSK3BITS),
									  .user = 0};
  // clang-format on
  if ((attribute & 1U << USHFT) != 0U) {
	attributeFields.user = 1;
  }
  else {
	attributeFields.user = 0;
  }
  outDebugString(L"{:5d}: color: {:2d} form: {:5d} type: {:2d}, layer: {:1d}: user: "
                 L"{:1d} attrib: 0x{:08x}\n",
                 ind,
                 attributeFields.color,
                 attributeFields.form,
                 attributeFields.type,
                 attributeFields.layer,
                 attributeFields.user,
                 attribute);
}
#endif

void duint(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t code, INT_INFO& ilData) {
  auto const& interleaveSequence = Instance->InterleaveSequence;
  if (ilData.coloc > ilData.start) {
	auto const count         = ilData.coloc > Instance->StitchBuffer.size()
	                               ? wrap::toUnsigned(Instance->StitchBuffer.size()) - ilData.start
	                               : ilData.coloc - ilData.start;
	auto const itStartStitch = wrap::next(Instance->StitchBuffer.begin(), ilData.start);
	auto const itEndStitch   = wrap::next(itStartStitch, count);
	auto const itDestination = wrap::next(buffer.begin(), ilData.output);
	buffer.insert(itDestination, itStartStitch, itEndStitch);
	ilData.start += count;
	ilData.output += count;
  }
  auto const& interleaveSequenceIndices = Instance->InterleaveSequenceIndices;
  if ((form.extendedAttribute & AT_STRT) != 0U) {
	if (!Instance->StateMap.testAndSet(StateFlag::DIDSTRT)) {
	  auto const itVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + form.fillStart);
	  ilData.output +=
	      gucon(form,
	            buffer,
	            *itVertex,
	            interleaveSequence.operator[](interleaveSequenceIndices.operator[](ilData.pins).index),
	            ilData.output,
	            code);
	}
  }
  if (auto point = F_POINT {}; lastcol(ilData.pins, point)) {
	ilData.output +=
	    gucon(form,
	          buffer,
	          point,
	          interleaveSequence.operator[](interleaveSequenceIndices.operator[](ilData.pins).index),
	          ilData.output,
	          code);
  }
  for (auto iSequence = interleaveSequenceIndices.operator[](ilData.pins).index;
       iSequence < interleaveSequenceIndices.operator[](wrap::toSize(ilData.pins) + 1U).index;
       ++iSequence) {
	if (ilData.output > 0) {
	  auto const prevOutput = ilData.output - 1U;
	  if (auto const& interleave = interleaveSequence.operator[](iSequence);
	      !util::closeEnough(interleave.x, buffer[prevOutput].x) ||
	      !util::closeEnough(interleave.y, buffer[prevOutput].y)) {
		buffer.emplace_back(interleave.x, interleave.y, code);
		++ilData.output;
	  }
	}
	else {
	  auto const& interleave = interleaveSequence.operator[](iSequence);
	  buffer.emplace_back(interleave.x, interleave.y, code);
	  ++ilData.output;
	}
  }
}

auto duprecs(std::vector<F_POINT_ATTR>& tempStitchBuffer, std::vector<O_REC*> const& pRecs, SORT_REC& sortRecord)
    -> uint32_t {
  sortRecord.direction = false;
  auto const jumps0    = wrap::round<uint32_t>(precjmps(tempStitchBuffer, pRecs, sortRecord));
  sortRecord.direction = true;
  auto const jumps1    = wrap::round<uint32_t>(precjmps(tempStitchBuffer, pRecs, sortRecord));
  if (jumps0 < jumps1) {
	sortRecord.direction = false;
	return jumps0;
  }
  sortRecord.direction = true;
  return jumps1;
}

constexpr auto durat(float const start, float const finish, float const featherRatio) -> float {
  return ((finish - start) * featherRatio) + start;
}

void durats(uint32_t const iSequence, std::vector<F_POINT>& sequence, FEATHER& feather) {
  auto const& bSequence = Instance->BSequence;

  auto const& bCurrent = bSequence.operator[](iSequence);
  auto const& bNext    = bSequence.operator[](wrap::toSize(iSequence) + 1U);

  if (auto const stitchLength = hypot(bNext.x - bCurrent.x, bNext.y - bCurrent.y);
      stitchLength < feather.minStitch) {
	sequence.emplace_back(bCurrent.x, bCurrent.y);
  }
  else {
	feather.ratioLocal       = feather.minStitch / stitchLength;
	auto const adjustedPoint = F_POINT {durat(bNext.x, bCurrent.x, feather.ratioLocal),
	                                    durat(bNext.y, bCurrent.y, feather.ratioLocal)};
	sequence.emplace_back(durat(adjustedPoint.x, bCurrent.x, feather.ratio),
	                      durat(adjustedPoint.y, bCurrent.y, feather.ratio));
  }
}

void durec(O_REC& record) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto const itStitch = wrap::next(Instance->StitchBuffer.begin(), record.start);
  record.type = gsl::narrow_cast<decltype(record.type)>(STITCH_TYPES.at(dutyp(itStitch->attribute)));
  auto const attribute = itStitch->attribute & SRTMSK;
  record.color         = attribute & COLMSK;
  record.form          = (attribute & FRMSK) >> FRMSHFT;
}

auto dutyp(uint32_t const attribute) noexcept -> uint32_t {
  auto const maskedAttribute = gsl::narrow_cast<DWORD>(attribute & SRTYPMSK);
  auto       bit             = gsl::narrow_cast<DWORD>(std::bit_width(maskedAttribute));
  --bit;
  if (bit == 0) {
	return 0U;
  }
  auto const result = gsl::narrow_cast<uint8_t>((bit & B1MASK) - STSHFT);
  if (constexpr auto USRSTCH = 12U; result != USRSTCH || // check if bit 31 is set (user edited stitch)
                                    (maskedAttribute & TYPATMSK) == 0) { // check if the stitch is a form stitch
	return result & NIBMASK;
  }
  return 1U; // must be an applique stitch
}

void duxrats(uint32_t const start, uint32_t const finish, F_POINT& point, float const featherRatioLocal) noexcept {
  auto const& bSequence = Instance->BSequence;

  point = F_POINT {durat(bSequence.operator[](finish).x, bSequence.operator[](start).x, featherRatioLocal),
                   durat(bSequence.operator[](finish).y, bSequence.operator[](start).y, featherRatioLocal)};
}

// ReSharper disable CppParameterMayBeConst
auto CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept -> BOOL {
  UNREFERENCED_PARAMETER(lParam);
  DestroyWindow(hwnd);
  return TRUE;
}
// ReSharper restore CppParameterMayBeConst

void fangfn(uint32_t const formIndex, float const angle) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.type == FRMFPOLY && form.fillType != 0U) {
	switch (form.fillType) {
	  case VRTF:
	  case HORF:
	  case ANGF: {
		form.fillType  = ANGF;
		form.fillAngle = angle;
		break;
	  }
	  case VCLPF:
	  case HCLPF:
	  case ANGCLPF: {
		form.fillType      = ANGCLPF;
		form.clipFillAngle = angle;
		break;
	  }
	  case TXVRTF:
	  case TXHORF:
	  case TXANGF: {
		form.fillType  = TXANGF;
		form.fillAngle = angle;
		break;
	  }
	  default: {
		outDebugString(L"default hit in fangfn: form.fillType [{}]\n", form.fillType);
		break;
	  }
	}
	form::refilfn(formIndex);
  }
}

void fcolfn(uint32_t const formIndex, uint8_t const color) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.fillType != 0U) {
	form.fillColor = color;
	form::refilfn(formIndex);
  }
}

void fhifn(uint32_t const formIndex, float const length) {
  auto& form = Instance->FormList.operator[](formIndex);

  auto const reference = form.rectangle.bottom;
  auto const ratio     = length / (form.rectangle.top - reference);
  auto       itVertex  = wrap::next(Instance->FormVertices.begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	itVertex->y = (itVertex->y - reference) * ratio + reference;
	++itVertex;
  }
  form.outline();
  form::refilfn(formIndex);
}

void findfn(uint32_t const formIndex, float const indent) {
  auto& form          = Instance->FormList.operator[](formIndex);
  form.underlayIndent = indent;
  if ((form.extendedAttribute & (AT_UND | AT_WALK)) != 0U) {
	form::refilfn(formIndex);
  }
}

void flenfn(uint32_t const formIndex, float const length) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.fillType != 0U && !form.isClip()) {
	form.stitchLength = length;
	form::refilfn(formIndex);
  }
}

void fmaxfn(uint32_t const formIndex, float const length) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.fillType != 0U) {
	form.maxFillStitchLen = length;
	form::refilfn(formIndex);
  }
}

void fminfn(uint32_t const formIndex, float const length) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.fillType != 0U) {
	form.minFillStitchLen = length;
	form::refilfn(formIndex);
  }
}

void fncwlk(FRM_HEAD& form) {
  Instance->OSequence.clear();
  form.extendedAttribute |= AT_CWLK;
  if (form.satinGuideCount != 0U) {
	if (form.wordParam != 0U) {
	  auto const thisVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + form.wordParam);
	  auto const nextVertex = std::next(thisVertex);
	  Instance->OSequence.emplace_back(wrap::midl(thisVertex->x, nextVertex->x),
	                                   wrap::midl(thisVertex->y, nextVertex->y));
	}
	auto itGuide =
	    wrap::next(Instance->SatinGuides.cbegin(), form.satinGuideIndex + form.satinGuideCount - 1U);
	auto const itVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex);
	for (auto iGuide = form.satinGuideCount - 1U; iGuide != 0; --iGuide) {
	  auto const startVertex  = wrap::next(itVertex, itGuide->start);
	  auto const finishVertex = wrap::next(itVertex, itGuide->finish);
	  Instance->OSequence.emplace_back(wrap::midl(finishVertex->x, startVertex->x),
	                                   wrap::midl(finishVertex->y, startVertex->y));
	  --itGuide;
	}
	if ((form.attribute & FRMEND) != 0U) {
	  Instance->OSequence.emplace_back(wrap::midl(itVertex[0].x, itVertex[1].x),
	                                   wrap::midl(itVertex[0].y, itVertex[1].y));
	}
  }
  else {
	auto start = 0U;
	if ((form.extendedAttribute & AT_STRT) != 0U) {
	  start = form.fillStart;
	}
	auto endVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + start);
	Instance->OSequence.push_back(*endVertex);
	auto finish = form::prv(form, start);
	start       = form::nxt(form, start);
	for (auto iGuide = 1U; iGuide < form.vertexCount / 2U; ++iGuide) {
	  auto const startVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + start);
	  auto const finishVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + finish);
	  if (auto const pnt = F_POINT {wrap::midl(finishVertex->x, startVertex->x),
	                                wrap::midl(finishVertex->y, startVertex->y)};
	      form::cisin(form, pnt.x, pnt.y)) {
		Instance->OSequence.push_back(pnt);
	  }
	  start  = form::nxt(form, start);
	  finish = form::prv(form, finish);
	}
	endVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex + start);
	Instance->OSequence.push_back(*endVertex);
  }
  ritwlk(form, CWLKMSK);
}

void fnund(uint32_t const formIndex, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices) {
  auto& form = Instance->FormList.operator[](formIndex);

  auto const savedStitchSize = UserStitchLength;
  UserStitchLength           = BIGFLOAT;
  if (form.underlaySpacing == 0.0F) {
	form.underlaySpacing = IniFile.underlaySpacing;
  }
  if (form.underlayStitchLen == 0.0F) {
	form.underlayStitchLen = IniFile.underlayStitchLen;
  }
  undclp(form.underlayStitchLen);
  Instance->StateMap.set(StateFlag::ISUND);
  form::angclpfn(form, textureSegments, angledFormVertices);
  ritwlk(form, UNDMSK);
  UserStitchLength = savedStitchSize;
}

void fnwlk(FRM_HEAD& form) {
  if (form.type == FRMLINE) {
	form.type = FRMFPOLY;
  }
  auto start = 0U;
  if ((form.extendedAttribute & AT_STRT) != 0U && form.type != FRMLINE) {
	start = form.fillStart;
  }
  auto count = form.vertexCount;
  if (form.type != FRMLINE) {
	++count;
  }
  auto const& walkPoints = xt::insid(form);
  Instance->OSequence.clear();
  while (count != 0U) {
	Instance->OSequence.push_back(walkPoints[start]);
	start = form::nxt(form, start);
	--count;
  }
  ritwlk(form, WLKMSK);
}

void fritfil(FRM_HEAD const& form, std::vector<F_POINT> const& featherSequence) {
  if (Instance->OSequence.empty()) {
	return;
  }
  auto const& interleaveSequence        = Instance->InterleaveSequence;
  auto&       interleaveSequenceIndices = Instance->InterleaveSequenceIndices;

  interleaveSequenceIndices.emplace_back(INS_REC {
      .code = TYPFRM, .color = form.fillColor, .index = wrap::toUnsigned(interleaveSequence.size()), .seq = I_FIL});
  form::chkseq(false);
  if ((form.extendedAttribute & AT_FTHBLND) == 0U ||
      ~(form.extendedAttribute & (AT_FTHUP | AT_FTHBTH)) == (AT_FTHUP | AT_FTHBTH)) {
	return;
  }
  interleaveSequenceIndices.emplace_back(INS_REC {
      .code = FTHMSK, .color = form.feather.color, .index = wrap::toUnsigned(interleaveSequence.size()), .seq = I_FTH});
  auto const sequenceMax      = wrap::toUnsigned(featherSequence.size());
  auto       iReverseSequence = sequenceMax - 1U;
  for (auto iSequence = 0U; iSequence < sequenceMax; ++iSequence) {
	Instance->OSequence.operator[](iSequence) = featherSequence[iReverseSequence];
	--iReverseSequence;
  }
  Instance->OSequence.resize(sequenceMax);
  form::chkseq(false);
}

void fspacfn(uint32_t const formIndex, float const spacing) {
  if (auto& form = Instance->FormList.operator[](formIndex); form.fillType != 0U) {
	if (spacing < 0) {
	  if (!form.isClip()) {
		return;
	  }
	}
	form.fillSpacing = spacing;
	form::refilfn(formIndex);
  }
}

void fthdfn(uint32_t const iSequence, FEATHER& feather) {
  auto const& bSequence = Instance->BSequence;

  auto const& bCurrent = bSequence.operator[](iSequence);
  auto const& bNext    = bSequence.operator[](wrap::toSize(iSequence) + 1U);

  auto const length = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);
  nurat(feather);
  Instance->OSequence.emplace_back(bCurrent.x, bCurrent.y);
  Instance->OSequence.emplace_back(bNext.x, bNext.y);
  if (length <= feather.minStitch) {
	return;
  }
  auto adjustedPoint = F_POINT {};
  auto currentPoint  = F_POINT {};
  auto nextPoint     = F_POINT {};
  feather.ratioLocal = HALF;
  duxrats(iSequence + 1, iSequence, adjustedPoint, feather.ratioLocal);
  feather.ratioLocal       = feather.minStitch / length / 2;
  auto const& sequence     = Instance->OSequence.operator[](iSequence);
  auto const& sequenceFwd1 = Instance->OSequence.operator[](wrap::toSize(iSequence) + 1U);
  xratf(adjustedPoint, sequence, currentPoint, feather.ratioLocal);
  xratf(adjustedPoint, sequenceFwd1, nextPoint, feather.ratioLocal);
  feather.ratioLocal = feather.ratio;
  xratf(currentPoint, sequence, Instance->OSequence.operator[](iSequence), feather.ratioLocal);
  xratf(
      nextPoint, sequenceFwd1, Instance->OSequence.operator[](wrap::toSize(iSequence) + 1U), feather.ratioLocal);
}

void fthfn(uint32_t const iSequence, FEATHER& feather) {
  nurat(feather);
  durats(iSequence, Instance->OSequence, feather);
}

void fthrbfn(uint32_t const iSequence, FEATHER& feather, std::vector<F_POINT>& featherSequence) {
  auto currentPoint = F_POINT {};
  auto nextPoint    = F_POINT {};

  auto& bSequence = Instance->BSequence;

  auto const& bCurrent = bSequence.operator[](iSequence);
  auto&       bNext    = bSequence.operator[](wrap::toSize(iSequence) + 1U);

  auto const length = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);
  nurat(feather);
  if (length < DBLF * feather.minStitch) {
	feather.ratio = HALF;
	ratpnt(iSequence, iSequence + 1, currentPoint, feather.ratio);
	ratpnt(iSequence + 3, iSequence + 2, nextPoint, feather.ratio);
  }
  else {
	auto currentHighPoint = F_POINT {};
	auto currentLowPoint  = F_POINT {};
	auto nextHighPoint    = F_POINT {};
	auto nextLowPoint     = F_POINT {};
	feather.ratioLocal    = feather.minStitch / length;
	duxrats(iSequence, iSequence + 1, currentLowPoint, feather.ratioLocal);
	duxrats(iSequence + 3, iSequence + 2, nextLowPoint, feather.ratioLocal);
	feather.ratioLocal = 1 - feather.ratioLocal;
	duxrats(iSequence, iSequence + 1, currentHighPoint, feather.ratioLocal);
	duxrats(iSequence + 3, iSequence + 2, nextHighPoint, feather.ratioLocal);
	feather.ratioLocal = feather.ratio;
	xratf(currentLowPoint, currentHighPoint, currentPoint, feather.ratioLocal);
	xratf(nextLowPoint, nextHighPoint, nextPoint, feather.ratioLocal); // NOLINT(readability-suspicious-call-argument)
  }
  auto const midPoint = midpnt(currentPoint, nextPoint);
  Instance->OSequence.emplace_back(bCurrent.x, bCurrent.y);
  Instance->OSequence.push_back(midPoint);
  featherSequence.emplace_back(bNext.x, bNext.y);
  featherSequence.push_back(midPoint);
}

void fthvars(FRM_HEAD const& form, FEATHER& feather) {
  Instance->StateMap.reset(StateFlag::BARSAT);
  Instance->StateMap.reset(StateFlag::FTHR);
  feather.fillType          = form.feather.fillType;
  feather.formRatio         = form.feather.ratio;
  feather.minStitch         = form.feather.minStitchSize;
  feather.totalCount        = form.feather.count;
  feather.extendedAttribute = form.extendedAttribute;
  feather.countUp           = form.feather.upCount;
  feather.upCount           = feather.countUp;
  feather.countDown         = form.feather.downCount;
  feather.downCount         = feather.countDown;
  feather.phaseIndex        = feather.upCount + feather.downCount;
  if ((feather.extendedAttribute & AT_FTHBLND) != 0U) {
	Instance->StateMap.set(StateFlag::BARSAT);
  }
  else {
	Instance->StateMap.set(StateFlag::FTHR);
  }
}

void fwidfn(uint32_t const formIndex, float const length) {
  auto& form = Instance->FormList.operator[](formIndex);

  auto const reference = form.rectangle.left;
  auto const ratio     = length / (form.rectangle.right - reference);
  auto       itVertex  = wrap::next(Instance->FormVertices.begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	itVertex->x = (itVertex->x - reference) * ratio + reference;
	++itVertex;
  }
  form.outline();
  form::refilfn(formIndex);
}

// ReSharper disable CppParameterMayBeConst
auto getstxt(int32_t const stringIndex, HWND dialog) -> float {
  // ToDo - This is not great code.
  constexpr auto SZBUFFER = 16U;

  auto buffer = std::array<wchar_t, SZBUFFER> {};
  GetWindowText(GetDlgItem(dialog, stringIndex), buffer.data(), gsl::narrow<int>(buffer.size()));
  return wrap::wcsToFloat(buffer.data()) * PFGRAN;
}
// ReSharper restore CppParameterMayBeConst

auto gucon(FRM_HEAD const&            form,
           std::vector<F_POINT_ATTR>& buffer,
           F_POINT const&             start,
           F_POINT const&             finish,
           uint32_t const             destination,
           uint32_t                   code) -> uint32_t {
  auto       length      = hypot(finish.x - start.x, finish.y - start.y);
  auto       startVertex = form::closflt(form, start.x, start.y);
  auto const endVertex   = form::closflt(form, finish.x, finish.y);

  // if connector is less than this, don't bother
  if (constexpr auto MINCONN = 5.0F; length < MINCONN) {
	return 0;
  }
  if (startVertex == endVertex) {
	return 0;
  }
  auto const& indentedPoint = xt::insid(form);
  auto        upDir         = startVertex;
  auto        downDir       = startVertex;
  while (true) {
	if (upDir == endVertex) {
	  Instance->StateMap.reset(StateFlag::WLKDIR);
	  break;
	}
	if (downDir == endVertex) {
	  Instance->StateMap.set(StateFlag::WLKDIR);
	  break;
	}
	upDir   = form::nxt(form, upDir);
	downDir = form::prv(form, downDir);
  }
  auto iStitch = destination;
  while (startVertex != endVertex) {
	if (iStitch != 0U) {
	  if (auto const prevStitch = iStitch - 1U;
	      !util::closeEnough(buffer[prevStitch].x, indentedPoint[startVertex].x) ||
	      !util::closeEnough(buffer[prevStitch].y, indentedPoint[startVertex].y)) {
		buffer.emplace_back(indentedPoint[startVertex].x, indentedPoint[startVertex].y, code);
		++iStitch;
	  }
	}
	else {
	  buffer.emplace_back(indentedPoint[startVertex].x, indentedPoint[startVertex].y, code);
	  ++iStitch;
	}
	auto const intermediateVertex = Instance->StateMap.test(StateFlag::WLKDIR)
	                                    ? form::prv(form, startVertex)
	                                    : form::nxt(form, startVertex);
	auto const delta = F_POINT {indentedPoint[intermediateVertex].x - indentedPoint[startVertex].x,
	                            indentedPoint[intermediateVertex].y - indentedPoint[startVertex].y};
	length           = hypot(delta.x, delta.y);

	if (auto const stitchCount = wrap::round<uint32_t>(length / UserStitchLength); stitchCount > 1U) {
	  auto const step = F_POINT {delta.x / wrap::toFloat(stitchCount), delta.y / wrap::toFloat(stitchCount)};
	  auto localPoint =
	      F_POINT {indentedPoint[startVertex].x + step.x, indentedPoint[startVertex].y + step.y};
	  for (auto iStep = 0U; iStep < stitchCount - 1U; ++iStep) {
		buffer.emplace_back(localPoint.x, localPoint.y, code);
		++iStitch;
		localPoint += step;
	  }
	}
	if (Instance->StateMap.test(StateFlag::WLKDIR)) {
	  startVertex = form::prv(form, startVertex);
	}
	else {
	  startVertex = form::nxt(form, startVertex);
	}
  }
  buffer.emplace_back(indentedPoint[startVertex].x, indentedPoint[startVertex].y, code);
  ++iStitch;
  return iStitch - destination;
}

// ReSharper disable CppParameterMayBeConst
auto handleSetsWMCOMMAND(WPARAM const& wparam, HWND hwndlg) -> bool {
  switch (LOWORD(wparam)) {
	case IDCANCEL: {
	  EndDialog(hwndlg, FALSE);
	  return true;
	}
	case IDOK: {
	  DesignSize = F_POINT {getstxt(IDC_DESWID, hwndlg), getstxt(IDC_DESHI, hwndlg)};
	  if (IsDlgButtonChecked(hwndlg, IDC_REFILF) != 0U) {
		Instance->UserFlagMap.set(UserFlag::CHREF);
	  }
	  else {
		Instance->UserFlagMap.reset(UserFlag::CHREF);
	  }
	  EndDialog(hwndlg, TRUE);
	  break;
	}
	case IDC_DESWID: {
	  if (wparam >> WRDSHFT == EN_CHANGE) {
		Instance->StateMap.reset(StateFlag::DESCHG);
	  }
	  break;
	}
	case IDC_DESHI: {
	  if (wparam >> WRDSHFT == EN_CHANGE) {
		Instance->StateMap.set(StateFlag::DESCHG);
	  }
	  break;
	}
	case IDC_DUASP: {
	  auto const designAspectRatio = DesignSize.y / DesignSize.x;
	  if (auto designSize = F_POINT {}; !chkasp(designSize, designAspectRatio, hwndlg)) {
		if (Instance->StateMap.test(StateFlag::DESCHG)) {
		  setstxt(IDC_DESWID, designSize.y / designAspectRatio, hwndlg);
		}
		else {
		  setstxt(IDC_DESHI, designSize.x * designAspectRatio, hwndlg);
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in fangfn: wparam [{}]\n", LOWORD(wparam));
	  break;
	}
  }
  return false;
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handleSetsWMINITDIALOG(HWND hwndlg) {
  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
  setstxt(IDC_DESWID, DesignSize.x, hwndlg);
  setstxt(IDC_DESHI, DesignSize.y, hwndlg);
  CheckDlgButton(hwndlg, IDC_REFILF, gsl::narrow_cast<UINT>(Instance->UserFlagMap.test(UserFlag::CHREF)));
}
// ReSharper restore CppParameterMayBeConst

auto isfil(FRM_HEAD const& form) noexcept -> bool {
  if (form.fillType != 0U) {
	return true;
  }
  if (form.edgeType != 0U) {
	return true;
  }
  if ((form.extendedAttribute & (AT_CWLK | AT_WALK | AT_UND)) != 0U) {
	return true;
  }
  return false;
}

auto lastcol(uint32_t index, F_POINT& point) noexcept -> bool {
  auto const& interleaveSequenceIndices = Instance->InterleaveSequenceIndices;

  auto const color = interleaveSequenceIndices.operator[](index).color;
  while (index != 0U) {
	--index;
	if (interleaveSequenceIndices.operator[](index).color == color) {
	  auto const nextIndex = interleaveSequenceIndices.operator[](wrap::toSize(index) + 1U).index;
	  point                = Instance->InterleaveSequence.operator[](wrap::toSize(nextIndex) - 1U);
	  return true;
	}
  }
  return false;
}

auto midpnt(F_POINT const& startPoint, F_POINT const& endPoint) noexcept -> F_POINT {
  return F_POINT {wrap::midl(endPoint.x, startPoint.x), wrap::midl(endPoint.y, startPoint.y)};
}

void notundfn(uint32_t code) {
  thred::savdo();
  code = ~code;

  auto& formList = Instance->FormList;

  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	if (auto& form = formList.operator[](ClosestFormToCursor); form.type != FRMLINE) {
	  auto const savedAttribute = form.extendedAttribute;
	  form.extendedAttribute &= code;
	  if (savedAttribute != form.extendedAttribute) {
		form::refilfn(ClosestFormToCursor);
	  }
	}
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  auto& form = formList.operator[](selectedForm);
	  if (form.type == FRMLINE) {
		continue;
	  }
	  auto const savedAttribute = form.extendedAttribute;
	  form.extendedAttribute &= code;
	  if (savedAttribute != form.extendedAttribute) {
		form::refilfn(selectedForm);
	  }
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void nudfn(F_RECTANGLE const& designSizeRect) noexcept {
  auto const newSize         = F_POINT {(designSizeRect.right - designSizeRect.left),
                                (designSizeRect.top - designSizeRect.bottom)};
  auto const designSizeRatio = F_POINT {(DesignSize.x / newSize.x), (DesignSize.y / newSize.y)};
  for (auto& stitch : Instance->StitchBuffer) {
	sadj(stitch, designSizeRatio, designSizeRect);
  }
  for (auto& formVertice : Instance->FormVertices) {
	sadj(formVertice, designSizeRatio, designSizeRect);
  }
}

void nurat(FEATHER& feather) noexcept {
  auto const remainder = fmodf(feather.globalPosition, 1.0F);
  switch (feather.fillType) {
	case FTHPSG: {
	  if (feather.upCount != 0U) {
		if (feather.countUp != 0U) {
		  feather.ratio =
		      (wrap::toFloat(feather.totalCount) - wrap::toFloat(form::psg() % feather.totalCount)) /
		      wrap::toFloat(feather.totalCount);
		  --feather.countUp;
		}
		else {
		  feather.ratio = (wrap::toFloat(feather.totalCount) - wrap::toFloat(bpsg() % feather.totalCount)) /
		                  wrap::toFloat(feather.totalCount);
		  if (feather.countDown != 0U) {
			--feather.countDown;
		  }
		  else {
			PseudoRandomValue = FSED;
			feather.countUp   = feather.upCount;
			feather.countDown = feather.downCount;
		  }
		}
	  }
	  else {
		feather.ratio = (wrap::toFloat(feather.totalCount) - wrap::toFloat(form::psg() % feather.totalCount)) /
		                wrap::toFloat(feather.totalCount);
	  }
	  feather.ratio *= feather.formRatio;
	  break;
	}
	case FTHFAZ: {
	  if (feather.phase >= feather.upCount) {
		feather.ratio = 1;
	  }
	  else {
		feather.ratio = feather.formRatio;
	  }
	  break;
	}
	case FTHSIN: {
	  if (remainder > feather.globalRatio) {
		feather.ratio = sin(((1.0F - remainder) / (1.0F - feather.globalRatio) * PI_F) + PI_F) * HALF + HALF;
	  }
	  else {
		feather.ratio = sin(remainder / feather.globalRatio * PI_F) * HALF + HALF;
	  }
	  feather.ratio *= feather.formRatio;
	  break;
	}
	case FTHSIN2: {
	  if (remainder > feather.globalRatio) {
		feather.ratio = sin((1.0F - remainder) / (1.0F - feather.globalRatio) * PI_F);
	  }
	  else {
		feather.ratio = sin(remainder / feather.globalRatio * PI_F);
	  }
	  feather.ratio *= feather.formRatio;
	  break;
	}
	case FTHRMP: {
	  if (remainder > feather.globalRatio) {
		feather.ratio = (1.0F - remainder) / (1.0F - feather.globalRatio);
	  }
	  else {
		feather.ratio = remainder / feather.globalRatio;
	  }
	  feather.ratio *= feather.formRatio;
	  break;
	}
	case FTHLIN:
	default: {
	  feather.ratio = feather.formRatio;
	  break;
	}
  }
  ++feather.phase %= feather.phaseIndex;
  feather.globalPosition += feather.globalStep;
}

auto precjmps(std::vector<F_POINT_ATTR>& tempStitchBuffer, std::vector<O_REC*> const& pRecs, SORT_REC const& sortRecord)
    -> double {
  auto currentRegion   = sortRecord.currentRegion;
  auto direction       = sortRecord.direction;
  auto formFillCounter = std::vector<uint32_t> {};
  formFillCounter.resize((Instance->FormList.size() + 2U) << 2U);
  auto totalJumps = 0U;
  while (chkrdun(formFillCounter, pRecs, sortRecord)) {
	auto       minimumLength = BIGFLOAT;
	auto const stitchIt =
	    direction ? wrap::next(Instance->StitchBuffer.begin(), pRecs[currentRegion]->finish - 1U)
	              : wrap::next(Instance->StitchBuffer.begin(), pRecs[currentRegion]->start);
	for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; ++iRegion) {
	  if (pRecs[iRegion]->otyp == formFillCounter[pRecs[iRegion]->form]) {
		auto const& startStitch = Instance->StitchBuffer.operator[](pRecs[iRegion]->startStitch);
		auto        length      = hypot(startStitch.x - stitchIt->x, startStitch.y - stitchIt->y);
		if (length < minimumLength) {
		  minimumLength = length;
		  direction     = false;
		  currentRegion = iRegion;
		}
		auto const& endStitch = Instance->StitchBuffer.operator[](pRecs[iRegion]->endStitch);
		length                = hypot(endStitch.x - stitchIt->x, endStitch.y - stitchIt->y);
		if (length < minimumLength) {
		  minimumLength = length;
		  direction     = true;
		  currentRegion = iRegion;
		}
	  }
	}
	// anything over 9 millimeters should result in another stitch
	if (constexpr auto SLMAX = 9.0F * PFGRAN; minimumLength > SLMAX) {
	  ++totalJumps;
	}
	++formFillCounter[pRecs[currentRegion]->form];
	if (Instance->StateMap.test(StateFlag::DUSRT)) {
	  if (direction) {
		if (pRecs[currentRegion]->start != 0U) {
		  for (auto iRegion = pRecs[currentRegion]->finish - 1U; iRegion >= pRecs[currentRegion]->start; --iRegion) {
			tempStitchBuffer.push_back(Instance->StitchBuffer.operator[](iRegion));
		  }
		}
		else {
		  auto iRegion = pRecs[currentRegion]->finish;
		  while (iRegion != 0U) {
			tempStitchBuffer.push_back(Instance->StitchBuffer.operator[](--iRegion));
		  }
		}
	  }
	  else {
		for (auto iRegion = pRecs[currentRegion]->start; iRegion < pRecs[currentRegion]->finish; ++iRegion) {
		  tempStitchBuffer.push_back(Instance->StitchBuffer.operator[](iRegion));
		}
	  }
	}
  }
  return totalJumps;
}

void ratpnt(uint32_t const iPoint, uint32_t const iNextPoint, F_POINT& point, float const featherRatio) noexcept {
  auto const& bSequence = Instance->BSequence;

  auto const& bPoint = bSequence.operator[](iPoint);

  point = F_POINT {(((bSequence.operator[](iNextPoint).x - bPoint.x) * featherRatio) + bPoint.x),
                   (((bSequence.operator[](iNextPoint).y - bPoint.y) * featherRatio) + bPoint.y)};
}

auto orComp(gsl::not_null<O_REC const*> const record1, gsl::not_null<O_REC const*> const record2) -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  auto const itColor1 = wrap::next(ColorOrder.begin(), record1->color);
  auto const itColor2 = wrap::next(ColorOrder.begin(), record2->color);
  if (*itColor1 < *itColor2) {
	return true;
  }
  if (*itColor2 < *itColor1) {
	return false;
  }
  if (record1->form < record2->form) {
	return true;
  }
  if (record2->form < record1->form) {
	return false;
  }
  if (record1->type < record2->type) {
	return true;
  }
  if (record2->type < record1->type) {
	return false;
  }
  if (record1->start < record2->start) {
	return true;
  }
  if (record2->start < record1->start) {
	return false;
  }
  return false;
}

auto orfComp(gsl::not_null<O_REC const*> const record1, gsl::not_null<O_REC const*> const record2) noexcept
    -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (record1->form < record2->form) {
	return true;
  }
  if (record2->form < record1->form) {
	return false;
  }
  if (record1->type < record2->type) {
	return true;
  }
  if (record2->type < record1->type) {
	return false;
  }

  return false;
}

void ritwlk(FRM_HEAD& form, uint32_t const walkMask) {
  auto& interleaveSequence = Instance->InterleaveSequence;

  if (!Instance->OSequence.empty()) {
	Instance->InterleaveSequenceIndices.emplace_back(INS_REC {
	    .code = walkMask, .color = form.underlayColor, .index = wrap::toUnsigned(interleaveSequence.size()), .seq = I_FIL});
#if BUGBAK
	for (auto val : Instance->OSequence) {
	  Instance->InterleaveSequence.push_back(val);
	}
#else
	constexpr auto MAXWLK  = 54.0F; // max underlay/edge walk stitch length
	constexpr auto MINWLK  = 2.4F;  // min underlay/edge walk stitch length
	form.underlayStitchLen = std::clamp(form.underlayStitchLen, MINWLK, MAXWLK);

	auto const underlayStitchLength = form.underlayStitchLen;
	auto const iSeqMax              = Instance->OSequence.size() - 1U;
	auto       sequence             = Instance->OSequence.begin();
	auto       sequenceFwd1         = std::next(sequence);
	for (auto iSequence = size_t {}; iSequence < iSeqMax; ++iSequence) {
	  auto const delta  = F_POINT {sequenceFwd1->x - sequence->x, sequenceFwd1->y - sequence->y};
	  auto const length = hypot(delta.x, delta.y);
	  if (auto const stitchCount = wrap::round<uint32_t>(length / underlayStitchLength); stitchCount != 0U) {
		auto const step =
		    F_POINT {delta.x / wrap::toFloat(stitchCount), delta.y / wrap::toFloat(stitchCount)};
		auto point = *sequence;
		for (auto index = 0U; index < stitchCount; ++index) {
		  interleaveSequence.push_back(point);
		  point += step;
		}
	  }
	  else {
		interleaveSequence.push_back(*sequence);
	  }
	  ++sequence;
	  ++sequenceFwd1;
	}
	interleaveSequence.push_back(Instance->OSequence.back());
#endif
  }
}

void sadj(F_POINT& point, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept {
  point.x = (point.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
  point.y = (point.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void sadj(F_POINT_ATTR& stitch, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept {
  stitch.x = (stitch.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
  stitch.y = (stitch.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void setColorOrder() noexcept {
  auto itColorOrder = wrap::next(ColorOrder.begin(), AppliqueColor);
  *itColorOrder     = 0;
  itColorOrder      = ColorOrder.begin();
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	if (iColor != AppliqueColor) {
	  *itColorOrder = iColor + 1U;
	}
	++itColorOrder;
  }
}

// ReSharper disable CppParameterMayBeConst
auto CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  handleSetsWMINITDIALOG(hwndlg);
	  break;
	}
	case WM_COMMAND: {
	  if (handleSetsWMCOMMAND(wparam, hwndlg)) {
		return TRUE;
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in fangfn: umsg [{}]\n", umsg);
	  break;
	}
  }
  return FALSE;
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void setstxt(int32_t const stringIndex, float const value, HWND dialog) {
  SetWindowText(GetDlgItem(dialog, stringIndex), format(FMT_COMPILE(L"{:.2f}"), value * IPFGRAN).c_str());
}
// ReSharper restore CppParameterMayBeConst

void setundfn(uint32_t const code) {
  thred::savdo();
  auto& formList = Instance->FormList;

  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	if (auto& form = formList.operator[](ClosestFormToCursor); form.type != FRMLINE) {
	  auto const savedAttribute = form.extendedAttribute;
	  form.extendedAttribute |= code;
	  if (savedAttribute != form.extendedAttribute) {
		form::refilfn(ClosestFormToCursor);
	  }
	}
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  auto& form = formList.operator[](selectedForm);
	  if (form.type == FRMLINE) {
		continue;
	  }
	  auto const savedAttribute = form.extendedAttribute;
	  form.extendedAttribute |= code;
	  if (savedAttribute != form.extendedAttribute) {
		form::refilfn(selectedForm);
	  }
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

auto srtchk(std::vector<O_REC*> const& stitchRegion,
            uint32_t const             count,
            uint32_t&                  badForm) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool {
  auto iStitchRegion = stitchRegion.begin();
  auto formIndex     = (*iStitchRegion)->form;
  auto color         = (*iStitchRegion)->color;
  ++iStitchRegion;
  for (auto iRegion = 1U; iRegion < count; ++iRegion) {
	if ((*iStitchRegion)->form == formIndex) {
	  auto const itSRColorOrder = wrap::next(ColorOrder.begin(), (*iStitchRegion)->color);
	  if (auto const itColorOrder = wrap::next(ColorOrder.begin(), color); *itSRColorOrder < *itColorOrder) {
		if (auto const& form = Instance->FormList.operator[](formIndex);
		    form.fillType == FTHF && (form.extendedAttribute & AT_FTHBLND) != 0U &&
		    (*iStitchRegion)->color == form.fillColor) {
		  continue;
		}
		badForm = iRegion;
		return false;
	  }

	  color = (*iStitchRegion)->color;
	}
	else {
	  color     = (*iStitchRegion)->color;
	  formIndex = (*iStitchRegion)->form;
	}
	++iStitchRegion;
  }
  return true;
}

} // namespace

void xt::fsort() {
  if (!Instance->StitchBuffer.empty()) {
	auto attribute    = Instance->StitchBuffer.front().attribute & SRTMSK;
	auto stitchRegion = std::vector<O_REC> {};

	constexpr auto EXPREGION = 100U; // expected regions
	stitchRegion.reserve(EXPREGION);
	// ToDo - fsort does not appear to be capable of handling the case where the underlay, fill and border colors
	//        in a single form are not in ascending order already.
	thred::savdo();
	stitchRegion.emplace_back(O_REC {});
	stitchRegion.back().startStitch = 0;
	setColorOrder();
	for (auto iStitch = 1U; iStitch < wrap::toUnsigned(Instance->StitchBuffer.size()); ++iStitch) {
	  if ((Instance->StitchBuffer.operator[](iStitch).attribute & SRTMSK) != attribute) {
		stitchRegion.back().finish    = iStitch;
		stitchRegion.back().endStitch = iStitch - 1U;
		stitchRegion.emplace_back(O_REC {
		    .start = iStitch, .finish = 0, .startStitch = iStitch, .endStitch = 0, .color = 0, .type = 0, .form = 0, .otyp = 0});
		attribute = Instance->StitchBuffer.operator[](iStitch).attribute & SRTMSK;
	  }
	}
	stitchRegion.back().finish    = wrap::toUnsigned(Instance->StitchBuffer.size());
	stitchRegion.back().endStitch = wrap::toUnsigned(Instance->StitchBuffer.size() - 1U);
	auto const lastRegion         = wrap::toUnsigned(stitchRegion.size());
	auto       pRecs              = std::vector<O_REC*> {};
	pRecs.reserve(lastRegion);
	auto pFRecs = std::vector<O_REC*> {};
	pFRecs.reserve(lastRegion);
	for (auto& region : stitchRegion) {
	  durec(region);
	  pRecs.push_back(&region);
	  pFRecs.push_back(&region);
	}
	std::ranges::sort(pRecs, orComp);
	std::ranges::sort(pFRecs, orfComp);
#ifdef _DEBUG
	dmprec(pRecs, lastRegion);
#endif
	if (Instance->FormList.empty()) {
	  return;
	}
	if (auto badForm = 0U; srtchk(pFRecs, lastRegion, badForm)) {
	  auto stitchRange = std::vector<RANGE> {};
	  stitchRange.resize(lastRegion);
	  stitchRange[0].start = 0;
	  attribute            = pRecs[0]->color;
	  auto currentForm     = BIGUINT;
	  auto typeCount       = 0U;
	  auto iRange          = 0U;
	  for (auto iRegion = 0U; iRegion < lastRegion; ++iRegion) {
		auto srtskp = true;
		if (attribute != pRecs[iRegion]->color) {
		  stitchRange[iRange++].finish = iRegion;
		  stitchRange[iRange].start    = iRegion;
		  attribute                    = pRecs[iRegion]->color;
		  currentForm                  = pRecs[iRegion]->form;
		  typeCount                    = 0;
		  srtskp                       = false;
		}
		if (srtskp) {
		  if (pRecs[iRegion]->form == currentForm) {
			++typeCount;
		  }
		  else {
			typeCount   = 0;
			currentForm = pRecs[iRegion]->form;
		  }
		}
		pRecs[iRegion]->otyp = typeCount;
	  }
	  stitchRange[iRange].finish  = lastRegion;
	  auto const lastRange        = ++iRange;
	  auto       tempStitchBuffer = std::vector<F_POINT_ATTR> {};
	  tempStitchBuffer.reserve(Instance->StitchBuffer.size());
	  for (iRange = 0; iRange < lastRange; ++iRange) {
		Instance->StateMap.reset(StateFlag::DUSRT);
		auto sortRecord   = SORT_REC {};
		sortRecord.start  = stitchRange[iRange].start;
		sortRecord.finish = stitchRange[iRange].finish;
		sortRecord.count  = sortRecord.finish - sortRecord.start;
		auto minimumJumps = BIGUINT;
		// timeout used to put an upper bound on the number of sorting permutations checked
		auto fileTime = FILETIME {};
		GetSystemTimeAsFileTime(&fileTime);
		auto const startTime        = tim2int(fileTime);
		auto       minimumIndex     = 0U;
		auto       minimumDirection = 0U;
		for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; ++iRegion) {
		  sortRecord.currentRegion = iRegion;
		  if (pRecs[iRegion]->otyp == 0U) {
			if (auto const jumps = duprecs(tempStitchBuffer, pRecs, sortRecord); jumps < minimumJumps) {
			  minimumJumps     = jumps;
			  minimumIndex     = iRegion;
			  minimumDirection = gsl::narrow_cast<uint32_t>(sortRecord.direction);
			}
		  }
		  GetSystemTimeAsFileTime(&fileTime);
		  constexpr auto SRTIM = 20000000UI64; // sort time limit in 100 ns intervals
		  if (auto const nextTime = tim2int(fileTime); nextTime.QuadPart - startTime.QuadPart > SRTIM) {
			break;
		  }
		}
		Instance->StateMap.set(StateFlag::DUSRT);
		sortRecord.currentRegion = minimumIndex;
		sortRecord.direction     = minimumDirection != 0U;
		precjmps(tempStitchBuffer, pRecs, sortRecord);
	  }
	  Instance->StitchBuffer = std::move(tempStitchBuffer);
	  thred::coltab();
	  Instance->StateMap.set(StateFlag::RESTCH);
	}
	else {
	  displayText::showMessage(IDS_SRTER, pFRecs[badForm]->form);
	}
  }
}

void xt::setfchk() {
  if (IniFile.dataCheck != 0U) {
	Instance->StateMap.set(StateFlag::FCHK);
  }
}

void xt::fthrfn(FRM_HEAD& form) {
  auto const savedSpacing    = LineSpacing;
  auto       feather         = FEATHER {};
  auto       featherSequence = std::vector<F_POINT> {};
  // ToDo - what does this function do
  PseudoRandomValue = FSED;
  fthvars(form, feather);
  LineSpacing = form.fillSpacing;
  satin::satfil(form);
  auto& bSequence = Instance->BSequence;

  bSequence.front().attribute       = 0;
  bSequence.operator[](1).attribute = 1;
  if (feather.phaseIndex == 0U) {
	feather.phaseIndex = 1U;
  }
  auto ind = gsl::narrow_cast<uint32_t>(bSequence.size()) / (feather.phaseIndex << 2U);
  if (auto const res = gsl::narrow_cast<uint32_t>(bSequence.size()) % (feather.phaseIndex << 2U);
      res > feather.phaseIndex << 1U) {
	++ind;
  }
  constexpr auto FSTEPSZ = 4.0F;
  feather.globalPosition = 0.0F;
  feather.globalStep  = (FSTEPSZ / gsl::narrow_cast<float>(bSequence.size())) * wrap::toFloat(ind);
  feather.globalPhase = gsl::narrow_cast<float>(bSequence.size()) / wrap::toFloat(ind);
  feather.globalRatio = wrap::toFloat(feather.countUp) / wrap::toFloat(feather.phaseIndex);
  feather.globalUp    = feather.globalPhase * feather.globalRatio;
  feather.globalDown  = feather.globalPhase - feather.globalUp;
  form.fillType       = FTHF;
  feather.phase       = 1U;
  bSequence.push_back(bSequence.operator[](bSequence.size() - 2U));
  bSequence.push_back(bSequence.operator[](bSequence.size() - 1U));
  if ((feather.extendedAttribute & AT_FTHBLND) != 0U) {
	for (ind = 0U; ind < wrap::toUnsigned(bSequence.size()) - 2U; ++ind) {
	  if (bSequence.operator[](ind).attribute == 0) {
		fthrbfn(ind, feather, featherSequence);
	  }
	}
  }
  else {
	if ((form.extendedAttribute & AT_FTHBTH) != 0U) {
	  for (ind = 0U; ind <= wrap::toUnsigned(bSequence.size()) - 2U; ++ind) {
		if (bSequence.operator[](ind).attribute == 0) {
		  fthdfn(ind, feather);
		}
	  }
	}
	else {
	  for (ind = 0U; ind <= wrap::toUnsigned(bSequence.size()) - 2U; ++ind) {
		if (bSequence.operator[](ind).attribute != 0) {
		  if ((feather.extendedAttribute & AT_FTHUP) != 0U) {
			fthfn(ind, feather);
		  }
		  else {
			Instance->OSequence.emplace_back(bSequence.operator[](ind).x, bSequence.operator[](ind).y);
		  }
		}
		else {
		  if ((feather.extendedAttribute & AT_FTHUP) != 0U) {
			Instance->OSequence.emplace_back(bSequence.operator[](ind).x, bSequence.operator[](ind).y);
		  }
		  else {
			fthfn(ind, feather);
		  }
		}
	  }
	}
  }
  Instance->StateMap.reset(StateFlag::FTHR);
  Instance->StateMap.reset(StateFlag::BARSAT);
  LineSpacing = savedSpacing;
  fritfil(form, featherSequence);
}

void xt::fethrf(uint32_t const formIndex) {
  if (!displayText::filmsgs(FMM_FTH)) {
	return;
  }
  auto& form = Instance->FormList.operator[](formIndex);
  clip::delmclp(formIndex);
  texture::deltx(formIndex);
  form.type                  = SAT;
  form.feather.ratio         = IniFile.featherRatio;
  form.feather.upCount       = IniFile.featherUpCount;
  form.feather.downCount     = IniFile.featherDownCount;
  form.feather.fillType      = IniFile.featherFillType;
  form.feather.minStitchSize = IniFile.featherMinStitchSize;
  form.extendedAttribute &= ~(AT_FTHUP | AT_FTHBTH | AT_FTHBLND);
  form.extendedAttribute |= IniFile.featherType;
  form.feather.count = IniFile.featherCount;
  form.stitchLength  = UserStitchLength;
  form.fillSpacing   = LineSpacing;
  form.fillColor     = gsl::narrow<uint8_t>(ActiveColor);
  form.feather.color = (ActiveColor + 1U) & COLMSK;
  form.fillType      = FTHF;
  form::refilfn(formIndex);
}

void xt::fethr() {
  if (!displayText::filmsgs(FMM_FTH)) {
	return;
  }
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fethrf(ClosestFormToCursor);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  if (Instance->FormList.operator[](selectedForm).vertexCount > 2U) {
		fethrf(selectedForm);
	  }
	}
  }
  Instance->StateMap.set(StateFlag::INIT);
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

auto xt::insid(FRM_HEAD const& form) -> std::vector<F_POINT>& {
  satin::satout(form, fabs(form.underlayIndent));
  if (form.underlayIndent > 0) {
	auto itVertex = wrap::next(Instance->FormVertices.cbegin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  if (!form::cisin(form, InsidePoints->operator[](iVertex).x, InsidePoints->operator[](iVertex).y)) {
		InsidePoints->operator[](iVertex) = *itVertex;
	  }
	  ++itVertex;
	}
	if (nullptr != InsidePoints) {
	  return *InsidePoints;
	}
	throw std::runtime_error("InsidePoints is null");
  }
  if (nullptr != OutsidePoints) {
	return *OutsidePoints;
  }
  throw std::runtime_error("OutsidePoints is null");
}

void xt::srtcol() {
  auto histogram = std::vector<uint32_t> {};
  histogram.resize(COLORCNT);
  auto colorStartStitch = std::vector<uint32_t> {};
  colorStartStitch.resize(COLORCNT);
  for (auto const& stitch : Instance->StitchBuffer) {
	++histogram[stitch.attribute & COLMSK];
  }
  auto startStitch = 0U;
  auto itHist      = histogram.cbegin();
  for (auto& stitchColor : colorStartStitch) {
	stitchColor = startStitch;
	startStitch += *itHist;
	++itHist;
  }
  auto highStitchBuffer = std::vector<F_POINT_ATTR> {};
  highStitchBuffer.resize(Instance->StitchBuffer.size());
  for (auto const& stitch : Instance->StitchBuffer) {
	highStitchBuffer[colorStartStitch[stitch.attribute & COLMSK]++] = stitch;
  }
  std::ranges::copy(highStitchBuffer, Instance->StitchBuffer.begin());
}

void xt::dubit(FRM_HEAD& form, uint32_t const bit) {
  thred::savdo();
  Instance->StateMap.set(StateFlag::WASDO);
  if (form.type == FRMLINE) {
	form.type = FRMFPOLY;
  }
  if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) == 0U &&
      (bit & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	if (form.fillType != 0U) {
	  form.underlayColor = form.fillColor;
	}
	else {
	  form.underlayColor = gsl::narrow<uint8_t>(ActiveColor);
	}
	form.underlayStitchLen = IniFile.underlayStitchLen;
  }
  if ((form.extendedAttribute & AT_UND) == 0U && (bit & AT_UND) != 0U) {
	form.underlayStitchAngle = IniFile.underlayAngle;
	form.underlaySpacing     = IniFile.underlaySpacing;
  }
  if (auto const code = form.extendedAttribute & bit; code != 0U) {
	form.extendedAttribute &= ~bit;
  }
  else {
	form.extendedAttribute |= bit;
  }
  form::refil(ClosestFormToCursor);
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::setuang() {
  Instance->StateMap.set(StateFlag::GTUANG);
  displayText::msgflt(IDS_UANG, IniFile.underlayAngle * DEGRADF);
}

void xt::setuspac() {
  Instance->StateMap.set(StateFlag::GTUSPAC);
  displayText::msgflt(IDS_USPAC, IniFile.underlaySpacing * IPFGRAN);
}

void xt::setwlkind() {
  Instance->StateMap.set(StateFlag::GTWLKIND);
  displayText::msgflt(IDS_WLKIND, IniFile.underlayIndent * IPFGRAN);
}

void xt::setulen() {
  Instance->StateMap.set(StateFlag::GTWLKLEN);
  displayText::msgflt(IDS_WLKLEN, IniFile.underlayStitchLen * IPFGRAN);
}

void xt::chkcwlk(uint32_t const formIndex) {
  if (auto& form = Instance->FormList.operator[](formIndex); (form.extendedAttribute & AT_CWLK) != 0U) {
	fncwlk(form);
  }
  else {
	delwlk((formIndex << FRMSHFT) | CWLKMSK);
  }
}

void xt::chkwlk(uint32_t const formIndex) {
  if (auto& form = Instance->FormList.operator[](formIndex); (form.extendedAttribute & AT_WALK) != 0U) {
	fnwlk(form);
  }
  else {
	delwlk((formIndex << FRMSHFT) | WLKMSK);
  }
}

void xt::chkund(uint32_t const                formIndex,
                std::vector<RNG_COUNT> const& textureSegments,
                std::vector<F_POINT>&         angledFormVertices) {
  if (auto const& form = Instance->FormList.operator[](formIndex); (form.extendedAttribute & AT_UND) != 0U) {
	fnund(formIndex, textureSegments, angledFormVertices);
  }
  else {
	delwlk(formIndex << FRMSHFT | UNDMSK);
  }
}

void xt::selalfrm() {
  auto const& formList = Instance->FormList;

  Instance->SelectedFormList.reserve(formList.size());
  for (auto formIndex = 0U; formIndex < wrap::toUnsigned(formList.size()); ++formIndex) {
	Instance->SelectedFormList.push_back(formIndex);
  }
  Instance->StateMap.set(StateFlag::RESTCH);
}

#ifdef _DEBUG

void xt::dmpat() {
  auto attribute = Instance->StitchBuffer.front().attribute;
  auto iStitch   = 0U;
  duatf(iStitch);
  for (auto const& stitch : Instance->StitchBuffer) {
	if (attribute != stitch.attribute) {
	  duatf(iStitch);
	  attribute = stitch.attribute;
	}
	++iStitch;
  }
}
#endif

void xt::fdelstch(uint32_t const formIndex, FillStartsDataType& fillStartsData, uint32_t& fillStartsMap) {
  auto const& form = Instance->FormList.operator[](formIndex);

  auto iDestinationStitch = 0U;
  auto tmap               = 0U;
  // ToDo - Still not sure what this function does?
  //        I suspect the fillStartsData members are not correctly named
  auto const codedFormIndex = formIndex << FRMSHFT;
  auto const bordercolor    = gsl::narrow_cast<uint32_t>(form.borderColor & COLMSK);

  auto const appliqueColor = gsl::narrow_cast<uint32_t>(form.borderColor >> FRMSHFT);
  for (auto iSourceStitch = 0U; iSourceStitch < wrap::toUnsigned(Instance->StitchBuffer.size()); ++iSourceStitch) {
	if (!Instance->UserFlagMap.test(UserFlag::FIL2OF) &&
	    Instance->StateMap.test(StateFlag::SELBOX) && iSourceStitch == ClosestPointIndex) {
	  ClosestPointIndex = iDestinationStitch;
	}
	if (auto const attribute = Instance->StitchBuffer.operator[](iSourceStitch).attribute;
	    codedFormIndex == (attribute & (FRMSK | NOTFRM))) {
	  switch (auto const type = STITCH_TYPES.at(dutyp(attribute)); type) {
		case TYPE_APPLIQUE: {
		  if ((tmap & M_AP) == 0U) {
			tmap |= M_AP;
			fillStartsData[applique] = iDestinationStitch;
		  }
		  break;
		}
		case TYPE_FTHR: {
		  if ((tmap & M_FTH) == 0U) {
			tmap |= M_FTH;
			fillStartsData[feather] = iDestinationStitch;
		  }
		  break;
		}
		case TYPE_FILL: {
		  if ((tmap & M_FIL) == 0U) {
			tmap |= M_FIL;
			fillStartsData[fill] = iDestinationStitch;
		  }
		  break;
		}
		case TYPE_BORDER: {
		  if ((tmap & M_BRD) == 0U) {
			tmap |= M_BRD;
			fillStartsData[border] = iDestinationStitch;
		  }
		  break;
		}
		default: {
		  if (form.fillType != 0U && (tmap & M_FIL) == 0U) {
			tmap |= M_FIL;
			fillStartsData[fill] = iDestinationStitch;
		  }
		  break;
		}
	  }
	}
	else {
	  // ToDo - there is a potential problem here when the active color is 0
	  //        because the default color is 0 as well. Should the default color
	  //        be e.g. -1 ?
	  auto const color = attribute & COLMSK;
	  if (color == form.fillColor) {
		tmap |= M_FCOL;
		fillStartsData[fillColor] = iDestinationStitch;
	  }
	  if (color == form.feather.color) {
		tmap |= M_FTHCOL;
		fillStartsData[featherColor] = iDestinationStitch;
	  }
	  if (color == bordercolor) {
		tmap |= M_ECOL;
		fillStartsData[borderColor] = iDestinationStitch;
	  }
	  if (color == appliqueColor) {
		tmap |= M_APCOL;
		fillStartsData[FSI::appliqueColor] = iDestinationStitch;
	  }
	  Instance->StitchBuffer.operator[](iDestinationStitch) = Instance->StitchBuffer.operator[](iSourceStitch);
	  ++iDestinationStitch;
	}
  }
  ++fillStartsData[fillColor];
  ++fillStartsData[featherColor];
  ++fillStartsData[borderColor];
  ++fillStartsData[FSI::appliqueColor];
  fillStartsMap = tmap;
  Instance->StitchBuffer.resize(iDestinationStitch);
  if ((tmap & M_ECOL) == 0U) {
	fillStartsData[borderColor] = wrap::toUnsigned(Instance->StitchBuffer.size());
  }
  if ((tmap & M_FTHCOL) == 0U) {
	fillStartsData[featherColor] = wrap::toUnsigned(Instance->StitchBuffer.size());
  }
  if ((tmap & M_FCOL) == 0U) {
	fillStartsData[fillColor] = wrap::toUnsigned(Instance->StitchBuffer.size());
  }
  if (form.edgeType != 0U) {
	if (form.edgeType == EDGEAPPL) {
	  if ((tmap & M_AP) == 0U) {
		if ((tmap & M_APCOL) != 0U) {
		  fillStartsData[applique] = fillStartsData[FSI::appliqueColor] + 1U;
		}
		else {
		  fillStartsData[applique] = wrap::toUnsigned(Instance->StitchBuffer.size());
		}
	  }
	}
	if ((tmap & M_BRD) == 0U) {
	  if ((tmap & M_ECOL) != 0U) {
		fillStartsData[border] = fillStartsData[borderColor] + 1U;
	  }
	  else {
		fillStartsData[border] = wrap::toUnsigned(Instance->StitchBuffer.size());
	  }
	}
  }
  if (form.fillType != 0U || (tmap & (M_WALK | M_UND | M_CWLK)) != 0U) {
	if ((tmap & M_FIL) == 0U) {
	  if ((tmap & M_FCOL) != 0U) {
		fillStartsData[fill] = fillStartsData[fillColor] + 1U;
	  }
	  else {
		fillStartsData[fill] = wrap::toUnsigned(Instance->StitchBuffer.size());
	  }
	}
  }
  if (form.fillType == FTHF) {
	if ((tmap & M_FTH) == 0U) {
	  if ((tmap & M_FTHCOL) != 0U) {
		fillStartsData[feather] = fillStartsData[featherColor] + 1U;
	  }
	  else {
		fillStartsData[feather] = wrap::toUnsigned(Instance->StitchBuffer.size());
	  }
	}
  }
  for (auto ind = 3U; ind != 0U; --ind) {
	auto const spFillArray = gsl::span {fillStartsData};
	iDestinationStitch     = ind - 1U;
	while (iDestinationStitch < ind) {
	  spFillArray[ind] = std::max(spFillArray[iDestinationStitch], spFillArray[ind]);
	  --iDestinationStitch;
	}
  }
  if (!Instance->UserFlagMap.test(UserFlag::FIL2OF) && Instance->StateMap.test(StateFlag::SELBOX)) {
	std::ranges::fill(fillStartsData, ClosestPointIndex);
  }
}

void xt::intlv(uint32_t const formIndex, FillStartsDataType const& fillStartsData, uint32_t const fillStartsMap) {
  auto ilData = INT_INFO {};
  Instance->StateMap.reset(StateFlag::ISEND);
  auto const& form                      = Instance->FormList.operator[](formIndex);
  auto&       interleaveSequenceIndices = Instance->InterleaveSequenceIndices;

  interleaveSequenceIndices.emplace_back(INS_REC {
      .code = 0, .color = 0, .index = wrap::toUnsigned(Instance->InterleaveSequence.size()), .seq = 0});
  ilData.layerIndex =
      gsl::narrow_cast<uint32_t>(form.attribute & FRMLMSK) << (LAYSHFT - 1) | formIndex << FRMSHFT;
  Instance->StateMap.reset(StateFlag::DIDSTRT);
  if (!Instance->StitchBuffer.empty()) {
	auto highStitchBuffer = std::vector<F_POINT_ATTR> {};
	auto code             = 0U;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(interleaveSequenceIndices.size() - 1U); ++iSequence) {
	  ilData.pins = iSequence;
	  switch (interleaveSequenceIndices.operator[](iSequence).seq) {
		case I_AP: {
		  if ((fillStartsMap & M_FIL) != 0U && fillStartsData[applique] >= ilData.coloc) {
			ilData.coloc = fillStartsData[applique];
		  }
		  else {
			ilData.coloc = fillStartsData[appliqueColor];
			if (ilData.coloc == 1) {
			  ilData.coloc = 0;
			}
		  }
		  break;
		}
		case I_FIL: {
		  if ((fillStartsMap & M_FIL) != 0U && fillStartsData[fill] >= ilData.coloc) {
			ilData.coloc = fillStartsData[fill];
		  }
		  else {
			ilData.coloc = fillStartsData[fillColor];
		  }
		  break;
		}
		case I_FTH: {
		  if ((fillStartsMap & M_FIL) != 0U && fillStartsData[feather] >= ilData.coloc) {
			ilData.coloc = fillStartsData[feather];
		  }
		  else {
			ilData.coloc = fillStartsData[featherColor];
		  }
		  break;
		}
		case I_BRD: {
		  if ((fillStartsMap & M_BRD) != 0U && fillStartsData[border] >= ilData.coloc) {
			ilData.coloc = fillStartsData[border];
		  }
		  else {
			ilData.coloc = fillStartsData[borderColor];
		  }
		  break;
		}
		default: {
		  outDebugString(L"default hit in intlv: seq [{}]\n",
		                 interleaveSequenceIndices.operator[](iSequence).seq);
		  break;
		}
	  }
	  code = gsl::narrow_cast<uint32_t>(ilData.layerIndex |
	                                    interleaveSequenceIndices.operator[](ilData.pins).code |
	                                    interleaveSequenceIndices.operator[](ilData.pins).color);
	  duint(form, highStitchBuffer, code, ilData);
	}
	chkend(form, highStitchBuffer, code, ilData);
	if (!Instance->StitchBuffer.empty() && ilData.start < wrap::toUnsigned(Instance->StitchBuffer.size() - 1U)) {
	  auto const stitchCount = wrap::toUnsigned(Instance->StitchBuffer.size() - ilData.start);

	  auto const itStartStitch = wrap::next(Instance->StitchBuffer.begin(), ilData.start);
	  auto const itEndStitch   = Instance->StitchBuffer.end();

	  auto const itDestination = wrap::next(highStitchBuffer.begin(), ilData.output);
	  highStitchBuffer.insert(itDestination, itStartStitch, itEndStitch);
	  ilData.output += stitchCount;
	}
	auto const itStartStitch = highStitchBuffer.begin();
	auto const itEndStitch   = wrap::next(itStartStitch, ilData.output);
	Instance->StitchBuffer.clear();
	Instance->StitchBuffer.insert(Instance->StitchBuffer.begin(), itStartStitch, itEndStitch);
  }
  else { // no stitches added so far
	addNewStitches(ilData, form);
  }
  thred::coltab();
}

void xt::setund() {
  setundfn(AT_UND);
}

void xt::setwlk() {
  setundfn(AT_WALK);
}

void xt::setcwlk() {
  setundfn(AT_CWLK);
}

void xt::notund() {
  notundfn(AT_UND);
}

void xt::notwlk() {
  notundfn(AT_WALK);
}

void xt::notcwlk() {
  notundfn(AT_CWLK);
}

void xt::dusulen(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	ulenfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  ulenfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::setEdit(uint32_t const code, StateFlag const flag) {
  displayText::tabmsg(code, true);
  Instance->StateMap.set(StateFlag::NUMIN);
  Instance->StateMap.set(flag);
  displayText::numWnd();
}

void xt::duspac(float const spacing) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	uspacfn(ClosestFormToCursor, spacing);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  uspacfn(selectedForm, spacing);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufang(float angle) {
  thred::savdo();
  angle *= DEGRADF;
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	uangfn(ClosestFormToCursor, angle);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  uangfn(selectedForm, angle);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::duflen(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	flenfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  flenfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufspac(float const spacing) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fspacfn(ClosestFormToCursor, spacing);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  fspacfn(selectedForm, spacing);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufind(float indent) {
  thred::savdo();
  indent *= PFGRAN;
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	findfn(ClosestFormToCursor, indent);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  findfn(selectedForm, indent);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufxang(float angle) {
  thred::savdo();
  angle *= DEGRADF;
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fangfn(ClosestFormToCursor, angle);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  fangfn(selectedForm, angle);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dundcol(uint8_t color) {
  thred::savdo();
  if (color != 0U) {
	--color;
  }
  color &= COLMSK;
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	ucolfn(ClosestFormToCursor, color);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  ucolfn(selectedForm, color);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufcol(uint8_t color) {
  thred::savdo();
  if (color != 0U) {
	--color;
  }
  color &= COLMSK;
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fcolfn(ClosestFormToCursor, color);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  fcolfn(selectedForm, color);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dubcol(uint8_t color) {
  thred::savdo();
  if (color != 0U) {
	--color;
  }
  color &= COLMSK;
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	bcolfn(ClosestFormToCursor, color);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  bcolfn(selectedForm, color);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dublen(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	blenfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  blenfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dubspac(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	bspacfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  bspacfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dubmin(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	bminfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  bminfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dubmax(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	bmaxfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  bmaxfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufmin(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fminfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  fminfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufmax(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fmaxfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  fmaxfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufwid(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fwidfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  fwidfn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::dufhi(float const length) {
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	fhifn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : Instance->SelectedFormList) {
	  fhifn(selectedForm, length);
	}
  }
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::setfilstrt() {
  if (!Instance->StateMap.test(StateFlag::FRMPSEL)) {
	displayText::shoseln(IDS_FORMP, IDS_FSTRT);
	return;
  }
  auto& form     = Instance->FormList.operator[](ClosestFormToCursor);
  form.fillStart = gsl::narrow<uint16_t>(ClosestVertexToCursor);
  form.extendedAttribute |= AT_STRT;
  form::refil(ClosestFormToCursor);
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::setfilend() {
  if (!Instance->StateMap.test(StateFlag::FRMPSEL)) {
	displayText::shoseln(IDS_FORMP, IDS_FEND);
	return;
  }
  auto& form   = Instance->FormList.operator[](ClosestFormToCursor);
  form.fillEnd = gsl::narrow<uint16_t>(ClosestVertexToCursor);
  form.extendedAttribute |= AT_END;
  form::refil(ClosestFormToCursor);
  thred::coltab();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void xt::duauxnam(fs::path& auxName) {
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  auxName.replace_extension(".dst");
	  break;
	}
#if PESACT
	case AUXPES: {
	  auxName.replace_extension(".pes");
	  break;
	}
#endif
	default: {
	  auxName.replace_extension("pcs");
	  break;
	}
  }
}

void xt::nudsiz() {
  auto designSizeRect = F_RECTANGLE {}; // design size rectangle
  thred::savdo();
  auto  flag     = 0;
  auto& formList = Instance->FormList;

  if (!Instance->StitchBuffer.empty()) {
	thred::stchrct(designSizeRect);
	flag = 1;
  }
  else {
	if (!formList.empty()) {
	  thred::frmrct(designSizeRect);
	  flag = 2;
	}
  }
  if (flag == 0) {
	return;
  }
  DesignSize =
      F_POINT {designSizeRect.right - designSizeRect.left, designSizeRect.top - designSizeRect.bottom};
  if (auto const nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_SIZ), ThrEdWindow, &setsprc);
      nResult < 1) { // if result is 0 (parent invalid) or -1 (function failed) don't do anything
	return;
  }
  flag = 0;

  constexpr auto HUPRATIO = 1.05F; // make the hoop 5% bigger
  if (DesignSize.x > IniFile.hoopSizeX) {
	IniFile.hoopSizeX = DesignSize.x * HUPRATIO;
	UnzoomedRect.cx   = std::lround(IniFile.hoopSizeX);
	flag              = 1;
  }
  if (DesignSize.y > IniFile.hoopSizeY) {
	IniFile.hoopSizeY = DesignSize.y * HUPRATIO;
	UnzoomedRect.cy   = std::lround(IniFile.hoopSizeY);
	flag              = 1;
  }
  nudfn(designSizeRect);
  if (Instance->UserFlagMap.test(UserFlag::CHREF)) {
	form::refilal();
  }
  if (flag != 0) {
	thred::movStch();
	thred::zumhom();
	displayText::hsizmsg();
  }
  form::centir();
  for (auto& iForm : formList) {
	iForm.outline();
  }
}

void xt::dushft() {
  Instance->StateMap.set(StateFlag::BZUMIN);
  ZoomBoxLine[1].x = ZoomBoxLine[2].x = 0L;
  ZoomBoxLine[2].y = ZoomBoxLine[3].y = 0L;
  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = WinMsg.pt.x - StitchWindowOrigin.x;
  ZoomBoxLine[0].y = ZoomBoxLine[1].y = WinMsg.pt.y - StitchWindowOrigin.y;
  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
  ZoomBoxOrigin                       = thred::pxCor2stch(WinMsg.pt);
}

void xt::mvshft() {
  if (Instance->StateMap.test(StateFlag::BOXZUM) && Instance->StateMap.testAndSet(StateFlag::VCAPT)) {
	SetCapture(ThrEdWindow);
  }
  if (!Instance->StateMap.test(StateFlag::BZUMIN)) {
	return;
  }
  if ((WinMsg.wParam & MK_LBUTTON) == 0U) {
	return;
  }
  if (Instance->StateMap.testAndSet(StateFlag::VCAPT)) {
	SetCapture(ThrEdWindow);
  }
  thred::unbBox();
  ZoomBoxLine[1].x = ZoomBoxLine[2].x = WinMsg.pt.x - StitchWindowOrigin.x;
  ZoomBoxLine[2].y = ZoomBoxLine[3].y = WinMsg.pt.y - StitchWindowOrigin.y;
  Instance->StateMap.set(StateFlag::BZUM);
  thred::bBox();
}

void xt::setclpspac() {
  displayText::msgflt(IDS_CLPSPAC, IniFile.clipOffset * IPFGRAN);
  Instance->StateMap.set(StateFlag::NUMIN);
  Instance->StateMap.set(StateFlag::SCLPSPAC);
}

void xt::clrstch() noexcept {
  while (EnumChildWindows(MainStitchWin, enumch, 0) != 0) { }
}

void xt::chgwrn() {
  Instance->UserFlagMap.flip(UserFlag::WRNOF);
  menu::wrnmen();
}

void xt::chgchk(uint8_t const code) {
  IniFile.dataCheck = code;
  menu::chkmen();
}
