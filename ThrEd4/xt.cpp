// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "globals.h"
#include "menu.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"

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

  // constexpr FEATHER() noexcept = default;
  // FEATHER(FEATHER const&) = default;
  // FEATHER(FEATHER&&) = default;
  // FEATHER& operator=(FEATHER const& rhs) = default;
  // FEATHER& operator=(FEATHER&&) = default;
  //~FEATHER() = default;
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

  // constexpr INT_INFO() noexcept = default;
  // INT_INFO(INT_INFO const&) = default;
  // INT_INFO(INT_INFO&&) = default;
  // INT_INFO& operator=(INT_INFO const& rhs) = default;
  // INT_INFO& operator=(INT_INFO&&) = default;
  //~INT_INFO() = default;
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

  // constexpr O_REC() noexcept = default;
  // O_REC(O_REC const&) = default;
  // O_REC(O_REC&&) = default;
  // O_REC& operator=(O_REC const& rhs) = default;
  // O_REC& operator=(O_REC&&) = default;
  //~O_REC() = default;
};

class SORT_REC
{
  public:
  uint32_t start {};          // start region index
  uint32_t finish {};         // finish region index
  uint32_t count {};          // number of regions in range
  uint32_t currentRegion {};  // current region index
  bool     direction = false; // direction of sort

  // constexpr SORT_REC() noexcept = default;
  // SORT_REC(SORT_REC const&) = default;
  // SORT_REC(SORT_REC&&) = default;
  // SORT_REC& operator=(SORT_REC const& rhs) = default;
  // SORT_REC& operator=(SORT_REC&&) = default;
  //~SORT_REC() = default;
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

enum StitchStyles {
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

namespace xi {

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

void durats(uint32_t iSequence, gsl::not_null<std::vector<F_POINT>*> sequence, FEATHER& feather);
void durec(O_REC& record);
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
void fnund(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
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
void rtrclpfn(FRM_HEAD const& form);
void ritwlk(FRM_HEAD& form, uint32_t walkMask);
void sadj(F_POINT& point, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept;
void sadj(F_POINT_ATTR& stitch, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept;

auto CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

void setstxt(int32_t stringIndex, float value, HWND dialog);
void setundfn(uint32_t code);
auto srtchk(std::vector<O_REC*> const& stitchRegion, uint32_t count, uint32_t& badForm) -> bool;

constexpr auto tim2int(FILETIME time) noexcept -> ULARGE_INTEGER;

void uangfn(uint32_t formIndex, float angle);
void ucolfn(uint32_t formIndex, uint8_t color);
void ulenfn(uint32_t formIndex, float length);
void undclp(FRM_HEAD const& form);
void uspacfn(uint32_t formIndex, float spacing);
void xratf(F_POINT const& startPoint, F_POINT const& endPoint, F_POINT& point, float featherRatioLocal) noexcept;
} // namespace xi

static auto DesignSize = F_POINT {};                        // design size
static auto ColorOrder = std::array<uint32_t, COLORCNT> {}; // color order adjusted for applique

void xt::setfchk() {
  if (IniFile.dataCheck != 0U) {
	StateMap->set(StateFlag::FCHK);
  }
}

void xi::fthvars(FRM_HEAD const& form, FEATHER& feather) {
  StateMap->reset(StateFlag::BARSAT);
  StateMap->reset(StateFlag::FTHR);
  feather.fillType          = form.fillInfo.feather.fillType;
  feather.formRatio         = form.fillInfo.feather.ratio;
  feather.minStitch         = form.fillInfo.feather.minStitchSize;
  feather.totalCount        = form.fillInfo.feather.count;
  feather.extendedAttribute = form.extendedAttribute;
  feather.countUp           = form.fillInfo.feather.upCount;
  feather.upCount           = feather.countUp;
  feather.countDown         = form.fillInfo.feather.downCount;
  feather.downCount         = feather.countDown;
  feather.phaseIndex        = feather.upCount + feather.downCount;
  if ((feather.extendedAttribute & AT_FTHBLND) != 0U) {
	StateMap->set(StateFlag::BARSAT);
  }
  else {
	StateMap->set(StateFlag::FTHR);
  }
}

constexpr auto xi::durat(float start, float finish, float featherRatio) -> float {
  return (finish - start) * featherRatio + start;
}

void xi::duxrats(uint32_t start, uint32_t finish, F_POINT& point, float featherRatioLocal) noexcept {
  point.x = durat(BSequence->operator[](finish).x, BSequence->operator[](start).x, featherRatioLocal);
  point.y = durat(BSequence->operator[](finish).y, BSequence->operator[](start).y, featherRatioLocal);
}

void xi::durats(uint32_t iSequence, gsl::not_null<std::vector<F_POINT>*> sequence, FEATHER& feather) {
  auto const& bCurrent = BSequence->operator[](iSequence);
  auto const& bNext    = BSequence->operator[](wrap::toSize(iSequence) + 1U);

  auto const stitchLength = hypot(bNext.x - bCurrent.x, bNext.y - bCurrent.y);
  if (stitchLength < feather.minStitch) {
	sequence->emplace_back(bCurrent.x, bCurrent.y);
  }
  else {
	feather.ratioLocal       = feather.minStitch / stitchLength;
	auto const adjustedPoint = F_POINT {durat(bNext.x, bCurrent.x, feather.ratioLocal),
	                                    durat(bNext.y, bCurrent.y, feather.ratioLocal)};
	sequence->emplace_back(durat(adjustedPoint.x, bCurrent.x, feather.ratio),
	                       durat(adjustedPoint.y, bCurrent.y, feather.ratio));
  }
}

constexpr auto FSED = uint32_t {1340007303U}; // feather sequence seed

auto xi::bpsg() noexcept -> uint32_t {
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

void xi::nurat(FEATHER& feather) noexcept {
  auto const remainder = fmodf(feather.globalPosition, 1.0F);
  switch (feather.fillType) {
	case FTHPSG: {
	  if (feather.upCount != 0U) {
		if (feather.countUp != 0U) {
		  feather.ratio =
		      (wrap::toFloat(feather.totalCount) - wrap::toFloat(form::psg() % feather.totalCount)) /
		      wrap::toFloat(feather.totalCount);
		  --(feather.countUp);
		}
		else {
		  feather.ratio = (wrap::toFloat(feather.totalCount) - wrap::toFloat(bpsg() % feather.totalCount)) /
		                  wrap::toFloat(feather.totalCount);
		  if (feather.countDown != 0U) {
			--(feather.countDown);
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
		feather.ratio = sin((1.0F - remainder) / (1.0F - feather.globalRatio) * PI_F + PI_F) * 0.5F + 0.5F;
	  }
	  else {
		feather.ratio = sin(remainder / feather.globalRatio * PI_F) * 0.5F + 0.5F;
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

void xi::fthfn(uint32_t iSequence, FEATHER& feather) {
  nurat(feather);
  durats(iSequence, OSequence, feather);
}

void xi::ratpnt(uint32_t iPoint, uint32_t iNextPoint, F_POINT& point, float featherRatio) noexcept {
  auto const& bPoint = BSequence->operator[](iPoint);
  point.x            = (BSequence->operator[](iNextPoint).x - bPoint.x) * featherRatio + bPoint.x;
  point.y            = (BSequence->operator[](iNextPoint).y - bPoint.y) * featherRatio + bPoint.y;
}

auto xi::midpnt(F_POINT const& startPoint, F_POINT const& endPoint) noexcept -> F_POINT {
  return F_POINT {wrap::midl(endPoint.x, startPoint.x), wrap::midl(endPoint.y, startPoint.y)};
}

void xi::xratf(F_POINT const& startPoint, F_POINT const& endPoint, F_POINT& point, float featherRatioLocal) noexcept {
  point.x = (endPoint.x - startPoint.x) * featherRatioLocal + startPoint.x;
  point.y = (endPoint.y - startPoint.y) * featherRatioLocal + startPoint.y;
}

void xi::fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<F_POINT>& featherSequence) {
  // clang-format off
  auto        currentPoint = F_POINT {};
  auto        nextPoint    = F_POINT {};
  auto const& bCurrent     = BSequence->operator[](iSequence);
  auto&       bNext        = BSequence->operator[](wrap::toSize(iSequence) + 1U);
  auto const  length       = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);
  // clang-format on
  nurat(feather);
  if (length < (2.0F * feather.minStitch)) {
	feather.ratio = 0.5F;
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
  OSequence->push_back(F_POINT {bCurrent.x, bCurrent.y});
  OSequence->push_back(midPoint);
  featherSequence.emplace_back(bNext.x, bNext.y);
  featherSequence.push_back(midPoint);
}

void xi::fthdfn(uint32_t iSequence, FEATHER& feather) {
  // clang-format off
  auto const& bCurrent = BSequence->operator[](iSequence);
  auto const& bNext    = BSequence->operator[](wrap::toSize(iSequence) + 1U);
  auto const  length   = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);
  // clang-format on
  nurat(feather);
  OSequence->push_back(F_POINT {bCurrent.x, bCurrent.y});
  OSequence->push_back(F_POINT {bNext.x, bNext.y});
  if (length > feather.minStitch) {
	auto adjustedPoint = F_POINT {};
	auto currentPoint  = F_POINT {};
	auto nextPoint     = F_POINT {};
	feather.ratioLocal = 0.5F;
	duxrats(iSequence + 1, iSequence, adjustedPoint, feather.ratioLocal);
	feather.ratioLocal       = feather.minStitch / length / 2;
	auto const& sequence     = OSequence->operator[](iSequence);
	auto const& sequenceFwd1 = OSequence->operator[](wrap::toSize(iSequence) + 1U);
	xratf(adjustedPoint, sequence, currentPoint, feather.ratioLocal);
	xratf(adjustedPoint, sequenceFwd1, nextPoint, feather.ratioLocal);
	feather.ratioLocal = feather.ratio;
	xratf(currentPoint, sequence, OSequence->operator[](iSequence), feather.ratioLocal);
	xratf(nextPoint, sequenceFwd1, OSequence->operator[](wrap::toSize(iSequence) + 1U), feather.ratioLocal);
  }
}

void xi::fritfil(FRM_HEAD const& form, std::vector<F_POINT> const& featherSequence) {
  if (!OSequence->empty()) {
	InterleaveSequenceIndices->emplace_back(
	    INS_REC {TYPFRM, form.fillColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL});
	form::chkseq(false);
	if (((form.extendedAttribute & AT_FTHBLND) != 0U) &&
	    ~(form.extendedAttribute & (AT_FTHUP | AT_FTHBTH)) != (AT_FTHUP | AT_FTHBTH)) {
	  InterleaveSequenceIndices->emplace_back(INS_REC {
	      FTHMSK, form.fillInfo.feather.color, wrap::toUnsigned(InterleaveSequence->size()), I_FTH});
	  auto const sequenceMax      = wrap::toUnsigned(featherSequence.size());
	  auto       iReverseSequence = sequenceMax - 1U;
	  for (auto iSequence = 0U; iSequence < sequenceMax; ++iSequence) {
		OSequence->operator[](iSequence) = featherSequence[iReverseSequence];
		--iReverseSequence;
	  }
	  OSequence->resize(sequenceMax);
	  form::chkseq(false);
	}
  }
}

void xt::fthrfn(FRM_HEAD& form) {
  auto const savedSpacing    = LineSpacing;
  auto       feather         = FEATHER {};
  auto       featherSequence = std::vector<F_POINT> {};
  // ToDo - what does this function do
  PseudoRandomValue = FSED;
  xi::fthvars(form, feather);
  LineSpacing = form.fillSpacing;
  satin::satfil(form);
  BSequence->front().attribute       = 0;
  BSequence->operator[](1).attribute = 1;
  if (feather.phaseIndex == 0U) {
	feather.phaseIndex = 1U;
  }
  auto       ind = gsl::narrow_cast<uint32_t>(BSequence->size()) / (feather.phaseIndex << 2U);
  auto const res = gsl::narrow_cast<uint32_t>(BSequence->size()) % (feather.phaseIndex << 2U);
  if (res > (feather.phaseIndex << 1U)) {
	++ind;
  }
  feather.globalPosition = 0.0F;
  feather.globalStep     = (4.0F / gsl::narrow_cast<float>(BSequence->size())) * wrap::toFloat(ind);
  feather.globalPhase    = gsl::narrow_cast<float>(BSequence->size()) / wrap::toFloat(ind);
  feather.globalRatio    = wrap::toFloat(feather.countUp) / wrap::toFloat(feather.phaseIndex);
  feather.globalUp       = feather.globalPhase * feather.globalRatio;
  feather.globalDown     = feather.globalPhase - feather.globalUp;
  form.fillType          = FTHF;
  feather.phase          = 1U;
  BSequence->push_back(BSequence->operator[](BSequence->size() - 2U));
  BSequence->push_back(BSequence->operator[](BSequence->size() - 1U));
  if ((feather.extendedAttribute & AT_FTHBLND) != 0U) {
	for (ind = 0U; ind < wrap::toUnsigned(BSequence->size()) - 2U; ++ind) {
	  if (BSequence->operator[](ind).attribute == 0) {
		xi::fthrbfn(ind, feather, featherSequence);
	  }
	}
  }
  else {
	if ((form.extendedAttribute & AT_FTHBTH) != 0U) {
	  for (ind = 0U; ind <= wrap::toUnsigned(BSequence->size()) - 2U; ++ind) {
		if (BSequence->operator[](ind).attribute == 0) {
		  xi::fthdfn(ind, feather);
		}
	  }
	}
	else {
	  for (ind = 0U; ind <= wrap::toUnsigned(BSequence->size()) - 2U; ++ind) {
		if (BSequence->operator[](ind).attribute != 0) {
		  if ((feather.extendedAttribute & AT_FTHUP) != 0U) {
			xi::fthfn(ind, feather);
		  }
		  else {
			OSequence->push_back(F_POINT {BSequence->operator[](ind).x, BSequence->operator[](ind).y});
		  }
		}
		else {
		  if ((feather.extendedAttribute & AT_FTHUP) != 0U) {
			OSequence->push_back(F_POINT {BSequence->operator[](ind).x, BSequence->operator[](ind).y});
		  }
		  else {
			xi::fthfn(ind, feather);
		  }
		}
	  }
	}
  }
  StateMap->reset(StateFlag::FTHR);
  StateMap->reset(StateFlag::BARSAT);
  LineSpacing = savedSpacing;
  xi::fritfil(form, featherSequence);
}

void xt::fethrf() {
  if (displayText::filmsgs(FMM_FTH)) {
	return;
  }
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  form.type                           = SAT;
  form.fillInfo.feather.ratio         = IniFile.featherRatio;
  form.fillInfo.feather.upCount       = IniFile.featherUpCount;
  form.fillInfo.feather.downCount     = IniFile.featherDownCount;
  form.fillInfo.feather.fillType      = IniFile.featherFillType;
  form.fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
  form.extendedAttribute &= ~(AT_FTHUP | AT_FTHBTH | AT_FTHBLND);
  form.extendedAttribute |= IniFile.featherType;
  form.fillInfo.feather.count     = IniFile.featherCount;
  form.lengthOrCount.stitchLength = UserStitchLength;
  form.fillSpacing                = LineSpacing;
  form.fillColor                  = gsl::narrow<uint8_t>(ActiveColor);
  form.fillInfo.feather.color     = (ActiveColor + 1U) & COLMSK;
  form.fillType                   = FTHF;
  form::refilfn(ClosestFormToCursor);
}

void xt::fethr() {
  if (displayText::filmsgs(FMM_FTH)) {
	return;
  }
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xt::fethrf();
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  if (FormList->operator[](selectedForm).vertexCount > 2U) {
		ClosestFormToCursor = selectedForm;
		xt::fethrf();
	  }
	}
  }
  StateMap->set(StateFlag::INIT);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

constexpr auto xi::tim2int(FILETIME time) noexcept -> ULARGE_INTEGER {
  auto const largeInt = ULARGE_INTEGER {{time.dwLowDateTime, time.dwHighDateTime}};
  return largeInt;
}

auto xt::insid(FRM_HEAD const& form) -> std::vector<F_POINT>& {
  satin::satout(form, fabs(form.underlayIndent));
  if (form.underlayIndent > 0) {
	auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
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

void xi::delwlk(uint32_t code) {
  if (!StitchBuffer->empty()) {
	std::erase_if(*StitchBuffer, [&code](F_POINT_ATTR const& stitch) -> bool {
	  return (stitch.attribute & WLKFMSK) == code;
	});
  }
}

void xi::ritwlk(FRM_HEAD& form, uint32_t walkMask) {
  if (!OSequence->empty()) {
	InterleaveSequenceIndices->emplace_back(
	    INS_REC {walkMask, form.underlayColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL});
#if BUGBAK
	for (auto val : *OSequence) {
	  InterleaveSequence->push_back(val);
	}
	InterleaveSequenceIndices->back().color = form.underlayColor;
#else
	constexpr auto MAXWLK = 54.0F; // max underlay/edge walk stitch length
	constexpr auto MINWLK = 2.4F;  // max underlay/edge walk stitch length

	if (form.underlayStitchLen < MINWLK) {
	  form.underlayStitchLen = MINWLK;
	}
	if (form.underlayStitchLen > MAXWLK) {
	  form.underlayStitchLen = MAXWLK;
	}
	auto const underlayStitchLength = form.underlayStitchLen;
	auto const iSeqMax              = OSequence->size() - 1U;
	auto       sequence             = OSequence->begin();
	auto       sequenceFwd1         = std::next(sequence);
	for (auto iSequence = size_t {}; iSequence < iSeqMax; ++iSequence) {
	  auto const delta  = F_POINT {sequenceFwd1->x - sequence->x, sequenceFwd1->y - sequence->y};
	  auto const length = hypot(delta.x, delta.y);
	  auto const stitchCount = wrap::round<uint32_t>(length / underlayStitchLength);
	  if (stitchCount != 0U) {
		auto const step =
		    F_POINT {delta.x / wrap::toFloat(stitchCount), delta.y / wrap::toFloat(stitchCount)};
		auto point = *sequence;
		for (auto index = 0U; index < stitchCount; ++index) {
		  InterleaveSequence->push_back(point);
		  point.x += step.x;
		  point.y += step.y;
		}
	  }
	  else {
		InterleaveSequence->push_back(*sequence);
	  }
	  ++sequence;
	  ++sequenceFwd1;
	}
	InterleaveSequence->push_back(OSequence->back());
	// ToDo - should this be front or (back - 1) ?
	InterleaveSequenceIndices->front().color = form.underlayColor;
#endif
  }
}

auto xi::gucon(FRM_HEAD const&            form,
               std::vector<F_POINT_ATTR>& buffer,
               F_POINT const&             start,
               F_POINT const&             finish,
               uint32_t                   destination,
               uint32_t                   code) -> uint32_t {
  auto       length      = hypot(finish.x - start.x, finish.y - start.y);
  auto       startVertex = form::closflt(form, start.x, start.y);
  auto const endVertex   = form::closflt(form, finish.x, finish.y);
  if (length < 5.0F) {
	return 0;
  }
  if (startVertex == endVertex) {
	return 0;
  }
  auto const& indentedPoint = xt::insid(form);
  auto        upDir         = startVertex;
  auto        downDir       = startVertex;
  do {
	if (upDir == endVertex) {
	  StateMap->reset(StateFlag::WLKDIR);
	  break;
	}
	if (downDir == endVertex) {
	  StateMap->set(StateFlag::WLKDIR);
	  break;
	}
	upDir   = form::nxt(form, upDir);
	downDir = form::prv(form, downDir);
  } while (true);
  auto iStitch = destination;
  while (startVertex != endVertex) {
	if (iStitch != 0U) {
	  if (!util::closeEnough(buffer[iStitch - 1U].x, indentedPoint[startVertex].x) ||
	      !util::closeEnough(buffer[iStitch - 1U].y, indentedPoint[startVertex].y)) {
		buffer.emplace_back(indentedPoint[startVertex].x, indentedPoint[startVertex].y, code);
		++iStitch;
	  }
	}
	else {
	  buffer.emplace_back(indentedPoint[startVertex].x, indentedPoint[startVertex].y, code);
	  ++iStitch;
	}
	auto const intermediateVertex = StateMap->test(StateFlag::WLKDIR) ? form::prv(form, startVertex)
	                                                                  : form::nxt(form, startVertex);
	auto const delta = F_POINT {indentedPoint[intermediateVertex].x - indentedPoint[startVertex].x,
	                            indentedPoint[intermediateVertex].y - indentedPoint[startVertex].y};
	length           = hypot(delta.x, delta.y);

	auto const stitchCount = wrap::round<uint32_t>(length / UserStitchLength);
	if (stitchCount > 1U) {
	  auto const step = F_POINT {delta.x / wrap::toFloat(stitchCount), delta.y / wrap::toFloat(stitchCount)};
	  auto localPoint =
	      F_POINT {indentedPoint[startVertex].x + step.x, indentedPoint[startVertex].y + step.y};
	  for (auto iStep = 0U; iStep < stitchCount - 1U; ++iStep) {
		buffer.emplace_back(localPoint.x, localPoint.y, code);
		++iStitch;
		localPoint.x += step.x;
		localPoint.y += step.y;
	  }
	}
	if (StateMap->test(StateFlag::WLKDIR)) {
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

void xi::fnwlk(FRM_HEAD& form) {
  if (form.type == FRMLINE) {
	form.type = FRMFPOLY;
  }
  auto start = 0U;
  if (((form.extendedAttribute & AT_STRT) != 0U) && form.type != FRMLINE) {
	start = form.fillStart;
  }
  auto count = form.vertexCount;
  if (form.type != FRMLINE) {
	++count;
  }
  auto const& walkPoints = xt::insid(form);
  OSequence->clear();
  while (count != 0U) {
	OSequence->push_back(walkPoints[start]);
	start = form::nxt(form, start);
	--count;
  }
  ritwlk(form, WLKMSK);
}

void xi::undclp(FRM_HEAD const& form) {
  // ToDo - Is it better to initialize individually?
  ClipBuffer->clear();
  ClipBuffer->reserve(2);
  ClipRectSize = F_LSIZ {0, form.underlayStitchLen};
  ClipBuffer->emplace_back(0.0F, 00.0F, 0U);
  ClipBuffer->emplace_back(0.0F, form.underlayStitchLen, 0U);
}

void xi::fncwlk(FRM_HEAD& form) {
  OSequence->clear();
  form.extendedAttribute |= AT_CWLK;
  if (form.satinGuideCount != 0U) {
	if (form.wordParam != 0U) {
	  auto const thisVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + form.wordParam);
	  auto const nextVertex = std::next(thisVertex);
	  OSequence->push_back(F_POINT {wrap::midl(thisVertex->x, nextVertex->x),
	                                wrap::midl(thisVertex->y, nextVertex->y)});
	}
	auto itGuide = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide + form.satinGuideCount - 1U);
	auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	for (auto iGuide = form.satinGuideCount - 1U; iGuide != 0; --iGuide) {
	  auto const startVertex  = wrap::next(itVertex, itGuide->start);
	  auto const finishVertex = wrap::next(itVertex, itGuide->finish);
	  OSequence->push_back(F_POINT {wrap::midl(finishVertex->x, startVertex->x),
	                                wrap::midl(finishVertex->y, startVertex->y)});
	  --itGuide;
	}
	if ((form.attribute & FRMEND) != 0U) {
	  OSequence->push_back(F_POINT {wrap::midl(itVertex[0].x, itVertex[1].x),
	                                wrap::midl(itVertex[0].y, itVertex[1].y)});
	}
  }
  else {
	auto start = 0U;
	if ((form.extendedAttribute & AT_STRT) != 0U) {
	  start = form.fillStart;
	}
	auto endVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + start);
	OSequence->push_back(*endVertex);
	auto finish = form::prv(form, start);
	start       = form::nxt(form, start);
	for (auto iGuide = 1U; iGuide < (form.vertexCount / 2U); ++iGuide) {
	  auto const startVertex  = wrap::next(FormVertices->cbegin(), form.vertexIndex + start);
	  auto const finishVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + finish);
	  auto const pnt          = F_POINT {wrap::midl(finishVertex->x, startVertex->x),
                                wrap::midl(finishVertex->y, startVertex->y)};
	  if (form::cisin(form, pnt.x, pnt.y)) {
		OSequence->push_back(pnt);
	  }
	  start  = form::nxt(form, start);
	  finish = form::prv(form, finish);
	}
	endVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + start);
	OSequence->push_back(*endVertex);
  }
  ritwlk(form, CWLKMSK);
}

void xt::srtcol() {
  auto histogram = std::vector<uint32_t> {};
  histogram.resize(COLORCNT);
  auto colorStartStitch = std::vector<uint32_t> {};
  colorStartStitch.resize(COLORCNT);
  for (auto const& stitch : *StitchBuffer) {
	++(histogram[stitch.attribute & COLMSK]);
  }
  auto startStitch = 0U;
  auto itHist      = histogram.cbegin();
  for (auto& stitchColor : colorStartStitch) {
	stitchColor = startStitch;
	startStitch += *itHist;
	++itHist;
  }
  auto highStitchBuffer = std::vector<F_POINT_ATTR> {};
  highStitchBuffer.resize(StitchBuffer->size());
  for (auto const& stitch : *StitchBuffer) {
	highStitchBuffer[colorStartStitch[stitch.attribute & COLMSK]++] = stitch;
  }
  std::ranges::copy(highStitchBuffer, StitchBuffer->begin());
}

void xt::dubit(FRM_HEAD& form, uint32_t bit) {
  thred::savdo();
  StateMap->set(StateFlag::WASDO);
  if (form.type == FRMLINE) {
	form.type = FRMFPOLY;
  }
  if (((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) == 0U) &&
      ((bit & (AT_UND | AT_WALK | AT_CWLK)) != 0U)) {
	if (form.fillType != 0U) {
	  form.underlayColor = form.fillColor;
	}
	else {
	  form.underlayColor = gsl::narrow<uint8_t>(ActiveColor);
	}
	form.underlayStitchLen = IniFile.underlayStitchLen;
  }
  if (((form.extendedAttribute & AT_UND) == 0U) && ((bit & AT_UND) != 0U)) {
	form.underlayStitchAngle = IniFile.underlayAngle;
	form.underlaySpacing     = IniFile.underlaySpacing;
  }
  auto const code = form.extendedAttribute & bit;
  if (code != 0U) {
	form.extendedAttribute &= ~(bit);
  }
  else {
	form.extendedAttribute |= bit;
  }
  form::refil();
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xt::setuang() {
  StateMap->set(StateFlag::GTUANG);
  displayText::msgflt(IDS_UANG, IniFile.underlayAngle * DEGRADF);
}

void xt::setuspac() {
  StateMap->set(StateFlag::GTUSPAC);
  displayText::msgflt(IDS_USPAC, IniFile.underlaySpacing * IPFGRAN);
}

void xt::setwlkind() {
  StateMap->set(StateFlag::GTWLKIND);
  displayText::msgflt(IDS_WLKIND, IniFile.underlayIndent * IPFGRAN);
}

void xt::setulen() {
  StateMap->set(StateFlag::GTWLKLEN);
  displayText::msgflt(IDS_WLKLEN, IniFile.underlayStitchLen * IPFGRAN);
}

void xt::chkcwlk(FRM_HEAD& form) {
  if ((form.extendedAttribute & AT_CWLK) != 0U) {
	xi::fncwlk(form);
  }
  else {
	xi::delwlk((ClosestFormToCursor << FRMSHFT) | CWLKMSK);
  }
}

void xt::chkwlk(FRM_HEAD& form) {
  if ((form.extendedAttribute & AT_WALK) != 0U) {
	xi::fnwlk(form);
  }
  else {
	xi::delwlk((ClosestFormToCursor << FRMSHFT) | WLKMSK);
  }
}

void xi::fnund(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices) {
  auto const savedStitchSize = UserStitchLength;
  UserStitchLength           = std::numeric_limits<float>::max();
  if (form.underlaySpacing == 0.0F) {
	form.underlaySpacing = IniFile.underlaySpacing;
  }
  if (form.underlayStitchLen == 0.0F) {
	form.underlayStitchLen = IniFile.underlayStitchLen;
  }
  undclp(form);
  StateMap->set(StateFlag::ISUND);
  form::angclpfn(form, textureSegments, angledFormVertices);
  ritwlk(form, UNDMSK);
  UserStitchLength = savedStitchSize;
}

void xt::chkund(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices) {
  if ((form.extendedAttribute & AT_UND) != 0U) {
	xi::fnund(form, textureSegments, angledFormVertices);
  }
  else {
	xi::delwlk((ClosestFormToCursor << FRMSHFT) | UNDMSK);
  }
}

void xt::selalfrm() {
  SelectedFormList->reserve(FormList->size());
  for (auto formIndex = 0U; formIndex < wrap::toUnsigned(FormList->size()); ++formIndex) {
	SelectedFormList->push_back(formIndex);
  }
  StateMap->set(StateFlag::RESTCH);
}

auto xi::dutyp(uint32_t attribute) noexcept -> uint32_t {
  auto       bit             = DWORD {};
  auto const maskedAttribute = gsl::narrow_cast<DWORD>(attribute & SRTYPMSK);
  // ToDo - replace
  _BitScanReverse(&bit, maskedAttribute);
  if (bit == 0) {
	return 0U;
  }
  auto const result = gsl::narrow_cast<uint8_t>((bit & B1MASK) - 18U);
  if ((result != 12U) || ((maskedAttribute & TYPATMSK) == 0)) {
	return result & NIBMASK;
  }
  return 1U;
}

void xi::durec(O_REC& record) {
  auto const itStitch = wrap::next(StitchBuffer->begin(), record.start);
  record.type = gsl::narrow_cast<decltype(record.type)>(STITCH_TYPES[dutyp(itStitch->attribute)]);
  auto const attribute = itStitch->attribute & SRTMSK;
  record.color         = attribute & COLMSK;
  record.form          = (attribute & FRMSK) >> FRMSHFT;
}

auto xi::orComp(gsl::not_null<O_REC const*> record1, gsl::not_null<O_REC const*> record2) -> bool {
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

auto xi::orfComp(gsl::not_null<O_REC const*> record1, gsl::not_null<O_REC const*> record2) noexcept -> bool {
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

auto xi::chkrdun(std::vector<uint32_t> const& formFillCounter,
                 std::vector<O_REC*> const&   pRecs,
                 SORT_REC const&              stitchRecord) noexcept -> bool {
  for (auto iStitch = stitchRecord.start; iStitch < stitchRecord.finish; ++iStitch) {
	if (pRecs[iStitch]->otyp == formFillCounter[pRecs[iStitch]->form]) {
	  return true;
	}
  }
  return false;
}

auto xi::precjmps(std::vector<F_POINT_ATTR>& stitchBuffer, std::vector<O_REC*> const& pRecs, SORT_REC const& sortRecord)
    -> double {
  auto currentRegion   = sortRecord.currentRegion;
  auto direction       = sortRecord.direction;
  auto formFillCounter = std::vector<uint32_t> {};
  formFillCounter.resize((FormList->size() + 2U) << 2U);
  auto totalJumps = 0U;
  while (chkrdun(formFillCounter, pRecs, sortRecord)) {
	auto       minimumLength = std::numeric_limits<float>::max();
	auto const stitchIt      = (direction)
	                               ? wrap::next(StitchBuffer->begin(), pRecs[currentRegion]->finish - 1U)
	                               : wrap::next(StitchBuffer->begin(), pRecs[currentRegion]->start);
	for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; ++iRegion) {
	  if (pRecs[iRegion]->otyp == formFillCounter[pRecs[iRegion]->form]) {
		auto const& startStitch = StitchBuffer->operator[](pRecs[iRegion]->startStitch);
		auto length             = hypot(startStitch.x - stitchIt->x, startStitch.y - stitchIt->y);
		if (length < minimumLength) {
		  minimumLength = length;
		  direction     = false;
		  currentRegion = iRegion;
		}
		auto const& endStitch = StitchBuffer->operator[](pRecs[iRegion]->endStitch);
		length                = hypot(endStitch.x - stitchIt->x, endStitch.y - stitchIt->y);
		if (length < minimumLength) {
		  minimumLength = length;
		  direction     = true;
		  currentRegion = iRegion;
		}
	  }
	}
	constexpr auto SLMAX = 9.0F * PFGRAN; // anything over 9 millimeters should result in another stitch
	if (minimumLength > SLMAX) {
	  ++totalJumps;
	}
	++(formFillCounter[pRecs[currentRegion]->form]);
	if (StateMap->test(StateFlag::DUSRT)) {
	  if (direction) {
		if (pRecs[currentRegion]->start != 0U) {
		  for (auto iRegion = pRecs[currentRegion]->finish - 1U; iRegion >= pRecs[currentRegion]->start; --iRegion) {
			stitchBuffer.push_back(StitchBuffer->operator[](iRegion));
		  }
		}
		else {
		  auto iRegion = pRecs[currentRegion]->finish;
		  while (iRegion != 0U) {
			stitchBuffer.push_back(StitchBuffer->operator[](--iRegion));
		  }
		}
	  }
	  else {
		for (auto iRegion = pRecs[currentRegion]->start; iRegion < pRecs[currentRegion]->finish; ++iRegion) {
		  stitchBuffer.push_back(StitchBuffer->operator[](iRegion));
		}
	  }
	}
  }
  return totalJumps;
}

auto xi::duprecs(std::vector<F_POINT_ATTR>& stitchBuffer, std::vector<O_REC*> const& pRecs, SORT_REC& sortRecord)
    -> uint32_t {
  sortRecord.direction = false;
  auto const jumps0    = wrap::round<uint32_t>(precjmps(stitchBuffer, pRecs, sortRecord));
  sortRecord.direction = true;
  auto const jumps1    = wrap::round<uint32_t>(precjmps(stitchBuffer, pRecs, sortRecord));
  if (jumps0 < jumps1) {
	sortRecord.direction = false;
	return jumps0;
  }
  sortRecord.direction = true;
  return jumps1;
}

#ifdef _DEBUG

void xi::dmprec(std::vector<O_REC*> const& stitchRegion, uint32_t count) {
  for (auto iRegion = 0U; iRegion < count; ++iRegion) {
	// NOLINTNEXTLINE
	outDebugString(
	    L"{:4d} attrb: 0x{:08x} form: {:4d} type: {} color: {:2d} start: {:5d} finish: {:5d}\n",
	    iRegion,
	    StitchBuffer->operator[](stitchRegion[iRegion]->start).attribute,
	    stitchRegion[iRegion]->form,
	    stitchRegion[iRegion]->type,
	    stitchRegion[iRegion]->color,
	    stitchRegion[iRegion]->start,
	    stitchRegion[iRegion]->finish);
  }
}
#endif

auto xi::srtchk(std::vector<O_REC*> const& stitchRegion, uint32_t count, uint32_t& badForm) -> bool {
  auto iStitchRegion = stitchRegion.begin();
  auto formIndex     = (*iStitchRegion)->form;
  auto color         = (*iStitchRegion)->color;
  ++iStitchRegion;
  for (auto iRegion = 1U; iRegion < count; ++iRegion) {
	if ((*iStitchRegion)->form == formIndex) {
	  auto const itSRColorOrder = wrap::next(ColorOrder.begin(), (*iStitchRegion)->color);
	  auto const itColorOrder   = wrap::next(ColorOrder.begin(), color);
	  if (*itSRColorOrder < *itColorOrder) {
		auto const& form = FormList->operator[](formIndex);
		if (form.fillType == FTHF && ((form.extendedAttribute & AT_FTHBLND) != 0U) &&
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

void xt::fsort() {
  if (!StitchBuffer->empty()) {
	auto attribute    = StitchBuffer->front().attribute & SRTMSK;
	auto stitchRegion = std::vector<O_REC> {};

	constexpr auto EXPREGION = 100U; // expected regions
	stitchRegion.reserve(EXPREGION);
	// ToDo - fsort does not appear to be capable of handling the case where the underlay, fill and border colors
	//        in a single form are not in ascending order already.
	thred::savdo();
	stitchRegion.emplace_back(O_REC {});
	stitchRegion.back().startStitch = 0;
	auto itColorOrder               = wrap::next(ColorOrder.begin(), AppliqueColor);
	*itColorOrder                   = 0;
	itColorOrder                    = ColorOrder.begin();
	for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	  if (iColor != AppliqueColor) {
		*itColorOrder = iColor + 1U;
	  }
	  ++itColorOrder;
	}
	for (auto iStitch = 1U; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	  if ((StitchBuffer->operator[](iStitch).attribute & SRTMSK) != attribute) {
		stitchRegion.back().finish    = iStitch;
		stitchRegion.back().endStitch = iStitch - 1U;
		stitchRegion.emplace_back(O_REC {});
		stitchRegion.back().start       = iStitch;
		stitchRegion.back().startStitch = iStitch;
		attribute                       = StitchBuffer->operator[](iStitch).attribute & SRTMSK;
	  }
	}
	stitchRegion.back().finish    = wrap::toUnsigned(StitchBuffer->size());
	stitchRegion.back().endStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
	auto const lastRegion         = wrap::toUnsigned(stitchRegion.size());
	auto       pRecs              = std::vector<O_REC*> {};
	pRecs.reserve(lastRegion);
	auto pFRecs = std::vector<O_REC*> {};
	pFRecs.reserve(lastRegion);
	for (auto& region : stitchRegion) {
	  xi::durec(region);
	  pRecs.push_back(&region);
	  pFRecs.push_back(&region);
	}
	std::ranges::sort(pRecs, xi::orComp);
	std::ranges::sort(pFRecs, xi::orfComp);
#ifdef _DEBUG
	xi::dmprec(pRecs, lastRegion);
#endif
	if (auto badForm = 0U; xi::srtchk(pFRecs, lastRegion, badForm)) {
	  auto stitchRange = std::vector<RANGE> {};
	  stitchRange.resize(lastRegion);
	  stitchRange[0].start = 0;
	  attribute            = pRecs[0]->color;
	  auto currentForm     = std::numeric_limits<uint32_t>::max();
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
	  tempStitchBuffer.reserve(StitchBuffer->size());
	  for (iRange = 0; iRange < lastRange; ++iRange) {
		StateMap->reset(StateFlag::DUSRT);
		auto sortRecord   = SORT_REC {};
		sortRecord.start  = stitchRange[iRange].start;
		sortRecord.finish = stitchRange[iRange].finish;
		sortRecord.count  = sortRecord.finish - sortRecord.start;
		auto minimumJumps = std::numeric_limits<uint32_t>::max();
		// timeout used to put an upper bound on the number of sorting permutations checked
		auto fileTime = FILETIME {};
		GetSystemTimeAsFileTime(&fileTime);
		auto const startTime        = xi::tim2int(fileTime);
		auto       minimumIndex     = 0U;
		auto       minimumDirection = 0U;
		for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; ++iRegion) {
		  sortRecord.currentRegion = iRegion;
		  if (pRecs[iRegion]->otyp == 0U) {
			if (auto const jumps = xi::duprecs(tempStitchBuffer, pRecs, sortRecord); jumps < minimumJumps) {
			  minimumJumps     = jumps;
			  minimumIndex     = iRegion;
			  minimumDirection = gsl::narrow_cast<uint32_t>(sortRecord.direction);
			}
		  }
		  GetSystemTimeAsFileTime(&fileTime);
		  constexpr auto SRTIM = 20000000UI64; // sort time limit in 100 ns intervals
		  if (auto const nextTime = xi::tim2int(fileTime); nextTime.QuadPart - startTime.QuadPart > SRTIM) {
			break;
		  }
		}
		StateMap->set(StateFlag::DUSRT);
		sortRecord.currentRegion = minimumIndex;
		sortRecord.direction     = (minimumDirection != 0U);
		xi::precjmps(tempStitchBuffer, pRecs, sortRecord);
	  }
	  *StitchBuffer = std::move(tempStitchBuffer);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  displayText::showMessage(IDS_SRTER, pFRecs[badForm]->form);
	}
  }
}

#ifdef _DEBUG

class ATFLD
{
  public:
  uint32_t color {};
  uint32_t form {};
  uint32_t type {};
  uint32_t layer {};
  uint32_t user {};

  // constexpr ATFLD() noexcept = default;
  // ATFLD(ATFLD&&) = default;
  // ATFLD& operator=(ATFLD const& rhs) = default;
  // ATFLD& operator=(ATFLD&&) = default;
  //~ATFLD() = default;
};

void xi::duatf(uint32_t ind) {
  // clang-format off
  auto const attribute       = StitchBuffer->operator[](ind).attribute;
  auto       attributeFields = ATFLD {(attribute & COLMSK),
									  ((attribute & FRMSK) >> FRMSHFT),
									  gsl::narrow_cast<uint32_t>(STITCH_TYPES[dutyp(attribute)]),
									  ((attribute >> LAYSHFT) & MSK3BITS),
									  0};
  // clang-format on
  if ((attribute & (1U << USHFT)) != 0U) {
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

void xt::dmpat() {
  auto attribute = StitchBuffer->front().attribute;
  auto iStitch   = 0U;
  xi::duatf(iStitch);
  for (auto const& stitch : *StitchBuffer) {
	if (attribute != stitch.attribute) {
	  xi::duatf(iStitch);
	  attribute = stitch.attribute;
	}
	++iStitch;
  }
}
#endif

void xt::fdelstch(FRM_HEAD const& form, FILL_STARTS& fillStartsData, uint32_t& fillStartsMap) {
  auto iDestinationStitch = 0U;
  auto tmap               = 0U;
  // ToDo - Still not sure what this function does?
  //        I suspect the fillStartsData members are not correctly named
  auto const codedFormIndex = (ClosestFormToCursor << FRMSHFT);
  auto const bordercolor    = gsl::narrow_cast<uint32_t>(form.borderColor & COLMSK);

  auto const appliqueColor = gsl::narrow_cast<uint32_t>(form.borderColor >> FRMSHFT);
  for (auto iSourceStitch = 0U; iSourceStitch < wrap::toUnsigned(StitchBuffer->size()); ++iSourceStitch) {
	if (!UserFlagMap->test(UserFlag::FIL2OF) && StateMap->test(StateFlag::SELBOX) &&
	    iSourceStitch == ClosestPointIndex) {
	  ClosestPointIndex = iDestinationStitch;
	}
	if (auto const attribute = StitchBuffer->operator[](iSourceStitch).attribute;
	    codedFormIndex == (attribute & (FRMSK | NOTFRM))) {
	  switch (auto const type = STITCH_TYPES[xi::dutyp(attribute)]; type) {
		case TYPE_APPLIQUE: {
		  if ((tmap & M_AP) == 0U) {
			tmap |= M_AP;
			fillStartsData.fillNamed.applique = iDestinationStitch;
		  }
		  break;
		}
		case TYPE_FTHR: {
		  if ((tmap & M_FTH) == 0U) {
			tmap |= M_FTH;
			fillStartsData.fillNamed.feather = iDestinationStitch;
		  }
		  break;
		}
		case TYPE_FILL: {
		  if ((tmap & M_FIL) == 0U) {
			tmap |= M_FIL;
			fillStartsData.fillNamed.fill = iDestinationStitch;
		  }
		  break;
		}
		case TYPE_BORDER: {
		  if ((tmap & M_BRD) == 0U) {
			tmap |= M_BRD;
			fillStartsData.fillNamed.border = iDestinationStitch;
		  }
		  break;
		}
		default: {
		  if ((form.fillType != 0U) && ((tmap & M_FIL) == 0U)) {
			tmap |= M_FIL;
			fillStartsData.fillNamed.fill = iDestinationStitch;
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
		fillStartsData.fillNamed.fillColor = iDestinationStitch;
	  }
	  if (color == form.fillInfo.feather.color) {
		tmap |= M_FTHCOL;
		fillStartsData.fillNamed.featherColor = iDestinationStitch;
	  }
	  if (color == bordercolor) {
		tmap |= M_ECOL;
		fillStartsData.fillNamed.borderColor = iDestinationStitch;
	  }
	  if (color == appliqueColor) {
		tmap |= M_APCOL;
		fillStartsData.fillNamed.appliqueColor = iDestinationStitch;
	  }
	  StitchBuffer->operator[](iDestinationStitch) = StitchBuffer->operator[](iSourceStitch);
	  ++iDestinationStitch;
	}
  }
  ++(fillStartsData.fillNamed.fillColor);
  ++(fillStartsData.fillNamed.featherColor);
  ++(fillStartsData.fillNamed.borderColor);
  ++(fillStartsData.fillNamed.appliqueColor);
  fillStartsMap = tmap;
  StitchBuffer->resize(iDestinationStitch);
  if ((tmap & M_ECOL) == 0U) {
	fillStartsData.fillNamed.borderColor = wrap::toUnsigned(StitchBuffer->size());
  }
  if ((tmap & M_FTHCOL) == 0U) {
	fillStartsData.fillNamed.featherColor = wrap::toUnsigned(StitchBuffer->size());
  }
  if ((tmap & M_FCOL) == 0U) {
	fillStartsData.fillNamed.fillColor = wrap::toUnsigned(StitchBuffer->size());
  }
  if (form.edgeType != 0U) {
	if (form.edgeType == EDGEAPPL) {
	  if ((tmap & M_AP) == 0U) {
		if ((tmap & M_APCOL) != 0U) {
		  fillStartsData.fillNamed.applique = fillStartsData.fillNamed.appliqueColor + 1U;
		}
		else {
		  fillStartsData.fillNamed.applique = wrap::toUnsigned(StitchBuffer->size());
		}
	  }
	}
	if ((tmap & M_BRD) == 0U) {
	  if ((tmap & M_ECOL) != 0U) {
		fillStartsData.fillNamed.border = fillStartsData.fillNamed.borderColor + 1U;
	  }
	  else {
		fillStartsData.fillNamed.border = wrap::toUnsigned(StitchBuffer->size());
	  }
	}
  }
  if ((form.fillType != 0U) || ((tmap & (M_WALK | M_UND | M_CWLK)) != 0U)) {
	if ((tmap & M_FIL) == 0U) {
	  if ((tmap & M_FCOL) != 0U) {
		fillStartsData.fillNamed.fill = fillStartsData.fillNamed.fillColor + 1U;
	  }
	  else {
		fillStartsData.fillNamed.fill = wrap::toUnsigned(StitchBuffer->size());
	  }
	}
  }
  if (form.fillType == FTHF) {
	if ((tmap & M_FTH) == 0U) {
	  if ((tmap & M_FTHCOL) != 0U) {
		fillStartsData.fillNamed.feather = fillStartsData.fillNamed.featherColor + 1U;
	  }
	  else {
		fillStartsData.fillNamed.feather = wrap::toUnsigned(StitchBuffer->size());
	  }
	}
  }
  for (auto ind = 3U; ind != 0U; --ind) {
	auto const spFillArray = gsl::span {fillStartsData.fillArray};
	iDestinationStitch     = ind - 1U;
	while (iDestinationStitch < ind) {
	  if (spFillArray[iDestinationStitch] > spFillArray[ind]) {
		spFillArray[ind] = spFillArray[iDestinationStitch];
	  }
	  --iDestinationStitch;
	}
  }
  if (!UserFlagMap->test(UserFlag::FIL2OF) && StateMap->test(StateFlag::SELBOX)) {
	auto& fillArray = fillStartsData.fillArray;
	std::ranges::fill(fillArray, ClosestPointIndex);
  }
}

auto xi::lastcol(uint32_t index, F_POINT& point) noexcept -> bool {
  auto const color = InterleaveSequenceIndices->operator[](index).color;
  while (index != 0U) {
	--index;
	if (InterleaveSequenceIndices->operator[](index).color == color) {
	  point = InterleaveSequence->operator[](
	      InterleaveSequenceIndices->operator[](wrap::toSize(index) + 1U).index - 1U);
	  return true;
	}
  }
  return false;
}

void xi::duint(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t code, INT_INFO& ilData) {
  if (ilData.coloc > ilData.start) {
	auto const count         = ilData.coloc > StitchBuffer->size()
	                               ? wrap::toUnsigned(StitchBuffer->size()) - ilData.start
	                               : ilData.coloc - ilData.start;
	auto const itStartStitch = wrap::next(StitchBuffer->begin(), ilData.start);
	auto const itEndStitch   = wrap::next(itStartStitch, count);
	auto const itDestination = wrap::next(buffer.begin(), ilData.output);
	buffer.insert(itDestination, itStartStitch, itEndStitch);
	ilData.start += count;
	ilData.output += count;
  }
  if ((form.extendedAttribute & AT_STRT) != 0U) {
	if (!StateMap->testAndSet(StateFlag::DIDSTRT)) {
	  auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + form.fillStart);
	  ilData.output +=
	      gucon(form,
	            buffer,
	            *itVertex,
	            InterleaveSequence->operator[](InterleaveSequenceIndices->operator[](ilData.pins).index),
	            ilData.output,
	            code);
	}
  }
  if (auto point = F_POINT {}; lastcol(ilData.pins, point)) {
	ilData.output +=
	    gucon(form,
	          buffer,
	          point,
	          InterleaveSequence->operator[](InterleaveSequenceIndices->operator[](ilData.pins).index),
	          ilData.output,
	          code);
  }
  for (auto iSequence = InterleaveSequenceIndices->operator[](ilData.pins).index;
       iSequence < InterleaveSequenceIndices->     operator[](wrap::toSize(ilData.pins) + 1U).index;
       ++iSequence) {
	if (ilData.output > 0) {
	  if (auto const& interleave = InterleaveSequence->operator[](iSequence);
	      !util::closeEnough(interleave.x, buffer[ilData.output - 1U].x) ||
	      !util::closeEnough(interleave.y, buffer[ilData.output - 1U].y)) {
		buffer.emplace_back(interleave.x, interleave.y, code);
		++(ilData.output);
	  }
	}
	else {
	  auto const& interleave = InterleaveSequence->operator[](iSequence);
	  buffer.emplace_back(interleave.x, interleave.y, code);
	  ++(ilData.output);
	}
  }
}

auto xi::isfil(FRM_HEAD const& form) noexcept -> bool {
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

void xi::chkend(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t code, INT_INFO& ilData) {
  if (isfil(form)) {
	StateMap->set(StateFlag::ISEND);
	if ((form.extendedAttribute & AT_END) != 0U) {
	  auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + form.fillEnd);
	  ilData.output += gucon(form, buffer, InterleaveSequence->back(), *itVertex, ilData.output, code);
	}
  }
}

void xt::intlv(FRM_HEAD const& form, FILL_STARTS const& fillStartsData, uint32_t fillStartsMap) {
  auto ilData = INT_INFO {};
  StateMap->reset(StateFlag::ISEND);
  InterleaveSequenceIndices->emplace_back(INS_REC {0, 0, wrap::toUnsigned(InterleaveSequence->size()), 0});
  ilData.layerIndex = (gsl::narrow_cast<uint32_t>(form.attribute & FRMLMSK) << (LAYSHFT - 1)) |
                      (ClosestFormToCursor << FRMSHFT);
  StateMap->reset(StateFlag::DIDSTRT);
  if (!StitchBuffer->empty()) {
	auto highStitchBuffer = std::vector<F_POINT_ATTR> {};
	auto code             = 0U;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(InterleaveSequenceIndices->size() - 1U); ++iSequence) {
	  ilData.pins = iSequence;
	  switch (InterleaveSequenceIndices->operator[](iSequence).seq) {
		case I_AP: {
		  if (((fillStartsMap & M_FIL) != 0U) && fillStartsData.fillNamed.applique >= ilData.coloc) {
			ilData.coloc = fillStartsData.fillNamed.applique;
		  }
		  else {
			ilData.coloc = fillStartsData.fillNamed.appliqueColor;
			if (ilData.coloc == 1) {
			  ilData.coloc = 0;
			}
		  }
		  break;
		}
		case I_FIL: {
		  if (((fillStartsMap & M_FIL) != 0U) && fillStartsData.fillNamed.fill >= ilData.coloc) {
			ilData.coloc = fillStartsData.fillNamed.fill;
		  }
		  else {
			ilData.coloc = fillStartsData.fillNamed.fillColor;
		  }
		  break;
		}
		case I_FTH: {
		  if (((fillStartsMap & M_FIL) != 0U) && fillStartsData.fillNamed.feather >= ilData.coloc) {
			ilData.coloc = fillStartsData.fillNamed.feather;
		  }
		  else {
			ilData.coloc = fillStartsData.fillNamed.featherColor;
		  }
		  break;
		}
		case I_BRD: {
		  if (((fillStartsMap & M_BRD) != 0U) && fillStartsData.fillNamed.border >= ilData.coloc) {
			ilData.coloc = fillStartsData.fillNamed.border;
		  }
		  else {
			ilData.coloc = fillStartsData.fillNamed.borderColor;
		  }
		  break;
		}
		default: {
		  outDebugString(L"default hit in intlv: seq [{}]\n",
		                 InterleaveSequenceIndices->operator[](iSequence).seq);
		  break;
		}
	  }
	  code = gsl::narrow_cast<uint32_t>(ilData.layerIndex |
	                                    InterleaveSequenceIndices->operator[](ilData.pins).code |
	                                    InterleaveSequenceIndices->operator[](ilData.pins).color);
	  xi::duint(form, highStitchBuffer, code, ilData);
	}
	xi::chkend(form, highStitchBuffer, code, ilData);
	if ((!StitchBuffer->empty()) && ilData.start < wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	  auto const stitchCount = wrap::toUnsigned(StitchBuffer->size() - ilData.start);

	  auto const itStartStitch = wrap::next(StitchBuffer->begin(), ilData.start);
	  auto const itEndStitch   = StitchBuffer->end();

	  auto const itDestination = wrap::next(highStitchBuffer.begin(), ilData.output);
	  highStitchBuffer.insert(itDestination, itStartStitch, itEndStitch);
	  ilData.output += stitchCount;
	}
	auto const itStartStitch = highStitchBuffer.begin();
	auto const itEndStitch   = wrap::next(itStartStitch, ilData.output);
	StitchBuffer->clear();
	StitchBuffer->insert(StitchBuffer->begin(), itStartStitch, itEndStitch);
  }
  else { // no stitches added so far
	auto code = 0U;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(InterleaveSequenceIndices->size() - 1U); ++iSequence) {
	  code = ilData.layerIndex | InterleaveSequenceIndices->operator[](iSequence).code |
	         InterleaveSequenceIndices->                    operator[](iSequence).color;
	  if ((form.extendedAttribute & AT_STRT) != 0U) {
		if (!StateMap->testAndSet(StateFlag::DIDSTRT)) {
		  auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + form.fillStart);
		  ilData.output += xi::gucon(
		      form,
		      *StitchBuffer,
		      *itVertex,
		      InterleaveSequence->operator[](InterleaveSequenceIndices->operator[](ilData.pins).index),
		      ilData.output,
		      code);
		}
	  }
	  if (auto colpnt = F_POINT {}; xi::lastcol(iSequence, colpnt)) {
		ilData.output += xi::gucon(
		    form,
		    *StitchBuffer,
		    colpnt,
		    InterleaveSequence->operator[](InterleaveSequenceIndices->operator[](iSequence).index),
		    ilData.output,
		    code);
	  }
	  auto const nextIndex = InterleaveSequenceIndices->operator[](wrap::toSize(iSequence) + 1U).index;
	  auto const thisIndex = InterleaveSequenceIndices->operator[](iSequence).index;
	  for (auto index = thisIndex; index < nextIndex; ++index) {
		if (ilData.output > 0) {
		  auto& interleave = InterleaveSequence->operator[](index);
		  if (auto const& stitch = StitchBuffer->operator[](ilData.output - 1U);
		      !util::closeEnough(interleave.x, stitch.x) || !util::closeEnough(interleave.y, stitch.y)) {
			StitchBuffer->push_back({interleave.x, interleave.y, code});
			++(ilData.output);
		  }
		}
		else {
		  auto& interleave = InterleaveSequence->operator[](index);
		  StitchBuffer->push_back({interleave.x, interleave.y, code});
		  ++(ilData.output);
		}
	  }
	}
	xi::chkend(form, *StitchBuffer, code, ilData);
  }
  thred::coltab();
}

void xi::setundfn(uint32_t code) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	if (auto& form = FormList->operator[](ClosestFormToCursor); form.type != FRMLINE) {
	  auto const savedAttribute = form.extendedAttribute;
	  form.extendedAttribute |= code;
	  if (savedAttribute != form.extendedAttribute) {
		form::refilfn(ClosestFormToCursor);
	  }
	}
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  auto& form          = FormList->operator[](selectedForm);
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
  StateMap->set(StateFlag::RESTCH);
}

void xt::setund() {
  xi::setundfn(AT_UND);
}

void xt::setwlk() {
  xi::setundfn(AT_WALK);
}

void xt::setcwlk() {
  xi::setundfn(AT_CWLK);
}

void xi::notundfn(uint32_t code) {
  thred::savdo();
  code = ~code;
  if (StateMap->test(StateFlag::FORMSEL)) {
	if (auto& form = FormList->operator[](ClosestFormToCursor); form.type != FRMLINE) {
	  auto const savedAttribute = form.extendedAttribute;
	  form.extendedAttribute &= code;
	  if (savedAttribute != form.extendedAttribute) {
		form::refilfn(ClosestFormToCursor);
	  }
	}
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  auto& form          = FormList->operator[](selectedForm);
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
  StateMap->set(StateFlag::RESTCH);
}

void xt::notund() {
  xi::notundfn(AT_UND);
}

void xt::notwlk() {
  xi::notundfn(AT_WALK);
}

void xt::notcwlk() {
  xi::notundfn(AT_CWLK);
}

void xi::ulenfn(uint32_t formIndex, float length) {
  if (auto& form = FormList->operator[](formIndex);
      (form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	form.underlayStitchLen = length;
	form::refilfn(formIndex);
  }
}

void xt::dusulen(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::ulenfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::ulenfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xt::setEdit(uint32_t code, StateFlag flag) {
  displayText::tabmsg(code, true);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(flag);
  displayText::numWnd();
}

void xi::uspacfn(uint32_t formIndex, float spacing) {
  if (auto& form = FormList->operator[](formIndex); (form.extendedAttribute & AT_UND) != 0U) {
	form.underlaySpacing = spacing;
	form::refilfn(formIndex);
  }
}

void xt::duspac(float spacing) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::uspacfn(ClosestFormToCursor, spacing);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::uspacfn(selectedForm, spacing);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::uangfn(uint32_t formIndex, float angle) {
  if (auto& form = FormList->operator[](formIndex); (form.extendedAttribute & AT_UND) != 0U) {
	form.underlayStitchAngle = angle;
	form::refilfn(formIndex);
  }
}

void xt::dufang(float angle) {
  thred::savdo();
  angle *= DEGRADF;
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::uangfn(ClosestFormToCursor, angle);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::uangfn(selectedForm, angle);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::flenfn(uint32_t formIndex, float length) {
  if (auto& form = FormList->operator[](formIndex); (form.fillType != 0U) && !form.isClip()) {
	form.lengthOrCount.stitchLength = length;
	form::refilfn(formIndex);
  }
}

void xt::duflen(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::flenfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::flenfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::fspacfn(uint32_t formIndex, float spacing) {
  if (auto& form = FormList->operator[](formIndex); form.fillType != 0U) {
	if (spacing < 0) {
	  if (!form.isClip()) {
		return;
	  }
	}
	form.fillSpacing = spacing;
	form::refilfn(formIndex);
  }
}

void xt::dufspac(float spacing) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::fspacfn(ClosestFormToCursor, spacing);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::fspacfn(selectedForm, spacing);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::findfn(uint32_t formIndex, float indent) {
  auto& form          = FormList->operator[](formIndex);
  form.underlayIndent = indent;
  if ((form.extendedAttribute & (AT_UND | AT_WALK)) != 0U) {
	form::refilfn(formIndex);
  }
}

void xt::dufind(float indent) {
  thred::savdo();
  indent *= PFGRAN;
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::findfn(ClosestFormToCursor, indent);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::findfn(selectedForm, indent);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::fangfn(uint32_t formIndex, float angle) {
  if (auto& form = FormList->operator[](formIndex); form.type == FRMFPOLY && (form.fillType != 0U)) {
	switch (form.fillType) {
	  case VRTF:
	  case HORF:
	  case ANGF: {
		form.fillType              = ANGF;
		form.angleOrClipData.angle = angle;
		break;
	  }
	  case VCLPF:
	  case HCLPF:
	  case ANGCLPF: {
		form.fillType           = ANGCLPF;
		form.satinOrAngle.angle = angle;
		break;
	  }
	  case TXVRTF:
	  case TXHORF:
	  case TXANGF: {
		form.fillType              = TXANGF;
		form.angleOrClipData.angle = angle;
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

void xt::dufxang(float angle) {
  thred::savdo();
  angle *= DEGRADF;
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::fangfn(ClosestFormToCursor, angle);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::fangfn(selectedForm, angle);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::ucolfn(uint32_t formIndex, uint8_t color) {
  if (auto& form = FormList->operator[](formIndex);
      (form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	form.underlayColor = color;
	form::refilfn(formIndex);
  }
}

void xt::dundcol(uint8_t color) {
  thred::savdo();
  if (color != 0U) {
	--color;
  }
  color &= COLMSK;
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::ucolfn(ClosestFormToCursor, color);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::ucolfn(selectedForm, color);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::fcolfn(uint32_t formIndex, uint8_t color) {
  if (auto& form = FormList->operator[](formIndex); form.fillType != 0U) {
	form.fillColor = color;
	form::refilfn(formIndex);
  }
}

void xt::dufcol(uint8_t color) {
  thred::savdo();
  if (color != 0U) {
	--color;
  }
  color &= COLMSK;
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::fcolfn(ClosestFormToCursor, color);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::fcolfn(selectedForm, color);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::bcolfn(uint32_t formIndex, uint8_t color) {
  auto& form          = FormList->operator[](formIndex); 
  if (form.edgeType == 0U) {
	return;
  }
  form.borderColor = color;
  form::refilfn(formIndex);
}

void xt::dubcol(uint8_t color) {
  thred::savdo();
  if (color != 0U) {
	--color;
  }
  color &= COLMSK;
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::bcolfn(ClosestFormToCursor, color);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::bcolfn(selectedForm, color);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::blenfn(uint32_t formIndex, float length) {
  auto& form = FormList->operator[](formIndex);
  if (form.isClip()) {
	return;
  }
  form.lengthOrCount.stitchLength = length;
  form::refilfn(formIndex);
}

void xt::dublen(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::blenfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::blenfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::bspacfn(uint32_t formIndex, float length) {
  if (auto& form = FormList->operator[](formIndex); form.edgeType != 0U) {
	form.edgeSpacing = length;
	form::refilfn(formIndex);
  }
}

void xt::dubspac(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::bspacfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::bspacfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::bminfn(uint32_t formIndex, float length) {
  if (auto& form = FormList->operator[](formIndex); form.edgeType != 0U) {
	form.minBorderStitchLen = length;
	form::refilfn(formIndex);
  }
}

void xt::dubmin(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::bminfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::bminfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::bmaxfn(uint32_t formIndex, float length) {
  if (auto& form = FormList->operator[](formIndex); form.edgeType != 0U) {
	form.maxBorderStitchLen = length;
	form::refilfn(formIndex);
  }
}

void xt::dubmax(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::bmaxfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::bmaxfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::fminfn(uint32_t formIndex, float length) {
  if (auto& form = FormList->operator[](formIndex); form.fillType != 0U) {
	form.minFillStitchLen = length;
	form::refilfn(formIndex);
  }
}

void xt::dufmin(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::fminfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::fminfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::fmaxfn(uint32_t formIndex, float length) {
  if (auto& form = FormList->operator[](formIndex); form.fillType != 0U) {
	form.maxFillStitchLen = length;
	form::refilfn(formIndex);
  }
}

void xt::dufmax(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::fmaxfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::fmaxfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::fwidfn(uint32_t formIndex, float length) {
  // clang-format off
  auto&       form      = FormList->operator[](formIndex);
  auto const  reference = form.rectangle.left;
  auto const  ratio     = length / (form.rectangle.right - reference);
  auto        itVertex  = wrap::next(FormVertices->begin(), form.vertexIndex);
  // clang-format on
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	itVertex->x = (itVertex->x - reference) * ratio + reference;
	++itVertex;
  }
  form.outline();
  form::refilfn(formIndex);
}

void xt::dufwid(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::fwidfn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::fwidfn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xi::fhifn(uint32_t formIndex, float length) {
  // clang-format off
  auto&       form      = FormList->operator[](formIndex);
  auto const  reference = form.rectangle.bottom;
  auto const  ratio     = length / (form.rectangle.top - reference);
  auto        itVertex  = wrap::next(FormVertices->begin(), form.vertexIndex);
  // clang-format on
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	itVertex->y = (itVertex->y - reference) * ratio + reference;
	++itVertex;
  }
  form.outline();
  form::refilfn(formIndex);
}

void xt::dufhi(float length) {
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	xi::fhifn(ClosestFormToCursor, length);
  }
  else {
	for (auto const selectedForm : (*SelectedFormList)) {
	  xi::fhifn(selectedForm, length);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xt::setfilstrt() {
  if (!StateMap->test(StateFlag::FRMPSEL)) {
	displayText::shoseln(IDS_FORMP, IDS_FSTRT);
	return;
  }
  auto& form     = FormList->operator[](ClosestFormToCursor);
  form.fillStart = gsl::narrow<uint16_t>(ClosestVertexToCursor);
  form.extendedAttribute |= AT_STRT;
  form::refil();
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void xt::setfilend() {
  if (!StateMap->test(StateFlag::FRMPSEL)) {
	displayText::shoseln(IDS_FORMP, IDS_FEND);
	return;
  }
  auto& form   = FormList->operator[](ClosestFormToCursor);
  form.fillEnd = gsl::narrow<uint16_t>(ClosestVertexToCursor);
  form.extendedAttribute |= AT_END;
  form::refil();
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
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

void xi::rtrclpfn(FRM_HEAD const& form) {
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
  auto const spClipData = gsl::span(clipStitchData, count);
  thred::savclp(spClipData[0], ClipBuffer->operator[](0), count);
  for (auto iStitch = 1U; iStitch < count; ++iStitch) {
	thred::savclp(spClipData[iStitch], ClipBuffer->operator[](iStitch), 0);
  }
  GlobalUnlock(clipHandle);
  SetClipboardData(Clip, clipHandle);
  CloseClipboard();
}

void xt::rtrclp() {
  if (!StateMap->test(StateFlag::FORMSEL)) {
	return;
  }
  if (auto const& form = FormList->operator[](ClosestFormToCursor); form.isTexture()) {
	texture::rtrtx(form);
  }
  else {
	xi::rtrclpfn(form);
  }
}

void xi::setstxt(int32_t stringIndex, float value, HWND dialog) {
  SetWindowText(GetDlgItem(dialog, stringIndex), fmt::format(FMT_COMPILE(L"{:.2f}"), (value * IPFGRAN)).c_str());
}

auto xi::getstxt(int32_t stringIndex, HWND dialog) -> float {
  // ToDo - This is not great code.
  constexpr auto SZBUFFER = 16U;

  auto buffer = std::array<wchar_t, SZBUFFER> {};
  GetWindowText(GetDlgItem(dialog, stringIndex), buffer.data(), gsl::narrow<int>(buffer.size()));
  return wrap::wcsToFloat(buffer.data()) * PFGRAN;
}

auto xi::chkasp(F_POINT& point, float aspectRatio, HWND dialog) -> bool {
  point.x = getstxt(IDC_DESWID, dialog);
  point.y = getstxt(IDC_DESHI, dialog);
  // ToDo - should this have a range? aspectRatio +/- %
  return util::closeEnough((point.y / point.x), aspectRatio);
}

auto CALLBACK xi::setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  setstxt(IDC_DESWID, DesignSize.x, hwndlg);
	  setstxt(IDC_DESHI, DesignSize.y, hwndlg);
	  CheckDlgButton(hwndlg, IDC_REFILF, gsl::narrow_cast<UINT>(UserFlagMap->test(UserFlag::CHREF)));
	  break;
	}
	case WM_COMMAND: {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  DesignSize.x = getstxt(IDC_DESWID, hwndlg);
		  DesignSize.y = getstxt(IDC_DESHI, hwndlg);
		  if (IsDlgButtonChecked(hwndlg, IDC_REFILF) != 0U) {
			UserFlagMap->set(UserFlag::CHREF);
		  }
		  else {
			UserFlagMap->reset(UserFlag::CHREF);
		  }
		  EndDialog(hwndlg, 1);
		  return TRUE;
		}
		case IDC_DESWID: {
		  if ((wparam >> WRDSHFT) == EN_CHANGE) {
			StateMap->reset(StateFlag::DESCHG);
		  }
		  break;
		}
		case IDC_DESHI: {
		  if ((wparam >> WRDSHFT) == EN_CHANGE) {
			StateMap->set(StateFlag::DESCHG);
		  }
		  break;
		}
		case IDC_DUASP: {
		  auto const designAspectRatio = DesignSize.y / DesignSize.x;
		  if (auto designSize = F_POINT {}; !chkasp(designSize, designAspectRatio, hwndlg)) {
			if (StateMap->test(StateFlag::DESCHG)) {
			  setstxt(IDC_DESWID, designSize.y / designAspectRatio, hwndlg);
			}
			else {
			  setstxt(IDC_DESHI, designSize.x * designAspectRatio, hwndlg);
			}
		  }
		  break;
		}
		default: {
		  // NOLINTNEXTLINE(hicpp-signed-bitwise)
		  outDebugString(L"default hit in fangfn: wparam [{}]\n", LOWORD(wparam));
		  break;
		}
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

void xi::sadj(F_POINT_ATTR& stitch, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept {
  stitch.x = (stitch.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
  stitch.y = (stitch.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xi::sadj(F_POINT& point, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept {
  point.x = (point.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
  point.y = (point.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xi::nudfn(F_RECTANGLE const& designSizeRect) noexcept {
  auto const newSize         = F_POINT {(designSizeRect.right - designSizeRect.left),
                                (designSizeRect.top - designSizeRect.bottom)};
  auto const designSizeRatio = F_POINT {(DesignSize.x / newSize.x), (DesignSize.y / newSize.y)};
  for (auto& stitch : *StitchBuffer) {
	sadj(stitch, designSizeRatio, designSizeRect);
  }
  for (auto& formVertice : *FormVertices) {
	sadj(formVertice, designSizeRatio, designSizeRect);
  }
}

void xt::nudsiz() {
  auto designSizeRect = F_RECTANGLE {}; // design size rectangle
  thred::savdo();
  auto flag = 0;
  if (!StitchBuffer->empty()) {
	thred::stchrct(designSizeRect);
	flag = 1;
  }
  else {
	if (!FormList->empty()) {
	  thred::frmrct(designSizeRect);
	  flag = 2;
	}
  }
  if (flag == 0) {
	return;
  }
  DesignSize.x = designSizeRect.right - designSizeRect.left;
  DesignSize.y = designSizeRect.top - designSizeRect.bottom;
  // ReSharper disable CppClangTidyClangDiagnosticCastFunctionType CppClangTidyPerformanceNoIntToPtr
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts
  auto const nResult = DialogBox(
      ThrEdInstance, MAKEINTRESOURCE(IDD_SIZ), ThrEdWindow, reinterpret_cast<DLGPROC>(xi::setsprc)); //  NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  // ReSharper restore CppClangTidyClangDiagnosticCastFunctionType CppClangTidyPerformanceNoIntToPtr

  if (nResult < 1) { // if result is 0 (parent invalid) or -1 (function failed) don't do anything
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
  xi::nudfn(designSizeRect);
  if (UserFlagMap->test(UserFlag::CHREF)) {
	form::refilal();
  }
  if (flag != 0) {
	thred::movStch();
	thred::zumhom();
	displayText::hsizmsg();
  }
  form::centir();
  for (auto& form : *FormList) {
	form.outline();
  }
}

void xt::dushft() {
  //	StateMap->set(StateFlag::BOXSLCT);
  StateMap->set(StateFlag::BZUMIN);
  //	StateMap->set(StateFlag::NOSEL);
  ZoomBoxLine[1].x = ZoomBoxLine[2].x = 0L;
  ZoomBoxLine[2].y = ZoomBoxLine[3].y = 0L;
  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
  ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
  ZoomBoxOrigin                       = thred::pxCor2stch(Msg.pt);
}

void xt::mvshft() {
  if (StateMap->test(StateFlag::BOXZUM) && StateMap->testAndSet(StateFlag::VCAPT)) {
	SetCapture(ThrEdWindow);
  }
  if (!StateMap->test(StateFlag::BZUMIN)) {
	return;
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if ((Msg.wParam & MK_LBUTTON) == 0U) {
	return;
  }
  if (StateMap->testAndSet(StateFlag::VCAPT)) {
	SetCapture(ThrEdWindow);
  }
  thred::unbBox();
  ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
  ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
  StateMap->set(StateFlag::BZUM);
  thred::bBox();
}

void xt::setclpspac() {
  displayText::msgflt(IDS_CLPSPAC, IniFile.clipOffset * IPFGRAN);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::SCLPSPAC);
}

auto CALLBACK xi::enumch(HWND hwnd, LPARAM lParam) noexcept -> BOOL {
  UNREFERENCED_PARAMETER(lParam);
  DestroyWindow(hwnd);
  return TRUE;
}

void xt::clrstch() noexcept {
  while (EnumChildWindows(MainStitchWin, xi::enumch, 0) != 0) { }
}

void xt::chgwrn() {
  UserFlagMap->flip(UserFlag::WRNOF);
  menu::wrnmen();
}

void xt::chgchk(uint8_t code) {
  IniFile.dataCheck = code;
  menu::chkmen();
}
