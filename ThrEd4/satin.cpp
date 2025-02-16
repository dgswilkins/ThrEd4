// Local Headers
#include "stdafx.h"
#include "clip.h"
#include "displayText.h"
#include "EnumMap.h"
#include "ExtendedBitSet.h"
#include "form.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "Resources/resource.h"
#include "satCon.h"
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
#include "gsl/narrow"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <windef.h>
#include <wingdi.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
// ReSharper disable CppUnusedIncludeDirective
#include <ranges>
// ReSharper restore CppUnusedIncludeDirective
#include <stdexcept>
#include <type_traits>
#include <vector>

class SATIN_SINGLE
{
  public:
  static auto getInstance() noexcept -> SATIN_SINGLE* {
	// NOLINTNEXTLINE(clang-diagnostic-exit-time-destructors)
	static SATIN_SINGLE instance;
	return &instance;
  }

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::vector<F_POINT> TempPolygon;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  SATIN_SINGLE(const SATIN_SINGLE&)                    = delete;
  auto operator=(const SATIN_SINGLE&) -> SATIN_SINGLE& = delete;
  SATIN_SINGLE(SATIN_SINGLE&&)                         = delete;
  auto operator=(SATIN_SINGLE&&) -> SATIN_SINGLE&      = delete;

  private:
  SATIN_SINGLE() noexcept = default;
  ~SATIN_SINGLE()         = default;
};

// satin internal namespace
namespace {
auto StartPoint = uint32_t {}; // starting formOrigin for a satin stitch guide-line

SATIN_SINGLE* SatinInstance;

// Definitions
auto chkbak(std::vector<F_POINT> const& satinBackup, F_POINT const& pnt) noexcept -> bool;
void filinsbw(std::vector<F_POINT>& satinBackup, F_POINT const& point, uint32_t& satinBackupIndex, F_POINT& stitchPoint);
auto nusac(uint32_t formIndex, uint32_t guideCount) -> uint32_t;
void outfn(FRM_HEAD const& form,
           uint32_t        start,
           uint32_t        finish,
           float           satinWidth) noexcept(!std::is_same_v<ptrdiff_t, int>);
void sacspac(uint32_t startGuide, uint32_t guideCount);
void satclos();
void satcpy(FRM_HEAD const& form, std::vector<SAT_CON> const& source, uint32_t size);
void satends(FRM_HEAD const& form, uint32_t isBlunt, float width);
void satfn(FRM_HEAD const&           form,
           std::vector<float> const& lengths,
           uint32_t                  line1Start,
           uint32_t                  line1End,
           uint32_t                  line2Start,
           uint32_t                  line2End);
void satmf(FRM_HEAD const& form, std::vector<float> const& lengths);
auto satOffset(const uint32_t& finish, const uint32_t& start, float satinWidth) noexcept -> F_POINT;
void satsbrd(uint32_t formIndex);
auto satselfn() -> bool;
void sbfn(std::vector<F_POINT> const& insidePoints, uint32_t start, uint32_t finish, F_POINT& stitchPoint);
void sfn(FRM_HEAD const& form, uint32_t startVertex, F_POINT& stitchPoint);
void unsat();

// Functions
auto chkbak(std::vector<F_POINT> const& satinBackup, F_POINT const& pnt) noexcept -> bool {
  auto const lenCheck = LineSpacing * LineSpacing;
  return std::ranges::any_of(satinBackup, [&](auto const& backup) {
	auto const deltaX = backup.x - pnt.x;
	auto const deltaY = backup.y - pnt.y;
	auto const length = (deltaX * deltaX) + (deltaY * deltaY);
	return length < lenCheck;
  });
}

void filinsbw(std::vector<F_POINT>& satinBackup, F_POINT const& point, uint32_t& satinBackupIndex, F_POINT& stitchPoint) {
  satinBackup[satinBackupIndex++] = point;
  satinBackupIndex &= satinBackup.size() - 1U; // make sure that satinBackupIndex rolls over when it reaches the end of the buffer
  form::filinsb(point, stitchPoint);
}

auto nusac(uint32_t const formIndex, uint32_t const guideCount) -> uint32_t {
  auto        guideIndex = 0U;
  auto const& formList   = Instance->formList;
  for (auto const formRange = std::ranges::subrange(formList.begin(), wrap::next(formList.begin(), formIndex));
       auto const& form : formRange) {
	if (form.type == SAT && form.satinGuideCount != 0U) {
	  guideIndex += form.satinGuideCount;
	}
  }
  sacspac(guideIndex, guideCount);
  return guideIndex;
}

void outfn(FRM_HEAD const& form,
           uint32_t const  start,
           uint32_t const  finish,
           float const     satinWidth) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto const offset   = fabs(Instance->formAngles.operator[](start)) < TNYFLOAT &&
                              fabs(Instance->formAngles.operator[](finish)) < TNYFLOAT
                            ? F_POINT {0.0F, satinWidth}
                            : satOffset(finish, start, satinWidth);
  auto const itVertex = form.type == FRMLINE && (form.edgeType & NEGUND) == EDGEPROPSAT
                            ? wrap::next(Instance->angledFormVertices.cbegin(), form.vertexIndex + finish)
                            : wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + finish);

  InsidePoints->operator[](finish)  = *itVertex - offset;
  OutsidePoints->operator[](finish) = *itVertex + offset;
}

void sacspac(uint32_t const startGuide, uint32_t const guideCount) {
  auto constexpr VAL = SAT_CON {};
  auto const itGuide = wrap::next(Instance->satinGuides.cbegin(), startGuide);
  Instance->satinGuides.insert(itGuide, VAL);
  auto& formList = Instance->formList;
  for (auto formRange =
           std::ranges::subrange(wrap::next(formList.begin(), ClosestFormToCursor + 1U), formList.end());
       auto& form : formRange) {
	if (form.type == SAT && form.fillType == SATF) {
	  form.satinGuideIndex += guideCount;
	}
  }
}

void satclos() {
  // clang-format off
  auto&      form              = Instance->formList.operator[](ClosestFormToCursor);
  auto const initialGuideCount = form.satinGuideCount;
  // clang-format on
  form::uninsf();
  auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);
  {
	auto       minimumLength = BIGFLOAT;
	auto const itVertex      = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	auto const vertexRange = std::ranges::subrange(itVertex, wrap::next(itVertex, form.vertexCount));
	for (auto iVertex = 0U; auto const& vertex : vertexRange) {
	  auto const deltaX = stitchPoint.x - vertex.x;
	  auto const deltaY = stitchPoint.y - vertex.y;
	  if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
		minimumLength         = length;
		ClosestVertexToCursor = iVertex;
	  }
	  ++iVertex;
	}
  }
  Instance->stateMap.reset(StateFlag::SATCNKT);
  if (form.type == FRMLINE) {
	form.fillType      = CONTF;
	auto closestVertex = ClosestVertexToCursor;
	if (StartPoint > closestVertex) {
	  std::swap(closestVertex, StartPoint);
	}
	if (StartPoint == 0U) {
	  ++StartPoint;
	}
	if (StartPoint == form.vertexCount - 2 && closestVertex == form.vertexCount - 1) {
	  StartPoint    = 1;
	  closestVertex = form.vertexCount - 2U;
	}
	if (closestVertex >= form.vertexCount - 2U) {
	  closestVertex = form.vertexCount - 2U;
	  StartPoint    = std::min(StartPoint, form.vertexCount - 2U);
	}
	if (closestVertex - StartPoint < 2U) {
	  closestVertex = StartPoint + 2U;
	  if (closestVertex > form.vertexCount - 2) {
		closestVertex = form.vertexCount - 2U;
		StartPoint    = closestVertex - 2U;
	  }
	}
	form.fillGuide = SAT_CON {StartPoint, closestVertex};
	return;
  }
  auto closestVertex = StartPoint;
  if (ClosestVertexToCursor < closestVertex) {
	std::swap(ClosestVertexToCursor, closestVertex);
  }
  if (closestVertex == 0 && ClosestVertexToCursor == form.vertexCount - 1) {
	closestVertex         = form.vertexCount - 1U;
	ClosestVertexToCursor = form.vertexCount;
  }
  if (closestVertex == 1 && ClosestVertexToCursor == form.vertexCount) {
	closestVertex         = 0;
	ClosestVertexToCursor = 1;
  }
  if (ClosestVertexToCursor - closestVertex == 1) {
	if ((form.attribute & FRMEND) != 0U) {
	  form.wordParam = closestVertex;
	}
	else {
	  if (closestVertex != 0U) {
		form::rotfrm(form, closestVertex);
	  }
	  form.attribute |= FRMEND;
	}
	satin::satadj(form);
	return;
  }
  if (form.satinGuideCount != 0U) {
	sacspac(form.satinGuideIndex + form.satinGuideCount, 1);
	auto const itGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex + form.satinGuideCount);
	itGuide->start  = closestVertex;
	itGuide->finish = ClosestVertexToCursor;
	++form.satinGuideCount;
	satin::satadj(form);
	return;
  }
  form.satinGuideIndex = nusac(ClosestFormToCursor, 1);
  auto const itGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex + initialGuideCount);
  itGuide->start       = closestVertex;
  itGuide->finish      = ClosestVertexToCursor;
  form.satinGuideCount = 1;
}

void satcpy(FRM_HEAD const& form, std::vector<SAT_CON> const& source, uint32_t const size) {
  auto const& currentFormGuides = form.satinGuideIndex;
  auto const  startGuide        = wrap::next(Instance->satinGuides.cbegin(), currentFormGuides);
  auto const  endGuide          = wrap::next(startGuide, form.satinGuideCount - size);
  Instance->satinGuides.erase(startGuide, endGuide);
  auto const itGuide = wrap::next(Instance->satinGuides.begin(), currentFormGuides);
  std::ranges::copy(source, itGuide);
}

void satends(FRM_HEAD const& form, uint32_t const isBlunt, float const width) {
  auto const& vertexIndex = form.vertexIndex;
  auto        itVertex    = wrap::next(Instance->formVertices.cbegin(), vertexIndex);
  auto const& formAngles  = Instance->formAngles;
  if ((isBlunt & SBLNT) != 0U) {
	auto step = F_POINT {sin(formAngles.front()) * width * HALF, cos(formAngles.front()) * width * HALF};
	if (Instance->stateMap.test(StateFlag::INDIR)) {
	  step = -step;
	}
	InsidePoints->front()  = F_POINT {itVertex->x + step.x, itVertex->y - step.y};
	OutsidePoints->front() = F_POINT {itVertex->x - step.x, itVertex->y + step.y};
  }
  else {
	InsidePoints->front() = OutsidePoints->front() = *itVertex;
  }
  auto const& vertexCount = form.vertexCount;
  itVertex                = wrap::next(itVertex, vertexCount - 1U);
  if ((isBlunt & FBLNT) != 0U) {
	constexpr auto SKIPVL = uint32_t {2U}; // check vertex before last

	auto step = F_POINT {sin(formAngles.operator[](vertexCount - SKIPVL)) * width * HALF,
	                     cos(formAngles.operator[](vertexCount - SKIPVL)) * width * HALF};
	if (Instance->stateMap.test(StateFlag::INDIR)) {
	  step = -step;
	}
	InsidePoints->back()  = F_POINT {itVertex->x + step.x, itVertex->y - step.y};
	OutsidePoints->back() = F_POINT {itVertex->x - step.x, itVertex->y + step.y};
  }
  else {
	InsidePoints->back() = OutsidePoints->back() = *itVertex;
  }
}

void satfn(FRM_HEAD const&           form,
           std::vector<float> const& lengths,
           uint32_t                  line1Start,
           uint32_t                  line1End,
           uint32_t                  line2Start,
           uint32_t                  line2End) {
  // check for zero length lines
  if (line1Start != line1End && line2Start == line2End) {
	return;
  }
  auto  itFirstVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
  auto& bSequence     = Instance->bSequence;

  // setup the initial stitch point
  if (!Instance->stateMap.testAndSet(StateFlag::SAT1)) {
	if (Instance->stateMap.test(StateFlag::FTHR)) {
	  if (form.vertexCount != 0U) {
		// add the first vertex to the bare sequence
		auto itVertex = wrap::next(itFirstVertex, line1Start % form.vertexCount);
		bSequence.emplace_back(itVertex->x, itVertex->y, 0);
	  }
	}
	else {
	  if (Instance->stateMap.test(StateFlag::BARSAT)) {
		// add the first vertices from line1 and line2 to the bare sequence
		if (form.vertexCount != 0U) {
		  auto itVertex = wrap::next(itFirstVertex, line1Start % form.vertexCount);
		  bSequence.emplace_back(itVertex->x, itVertex->y, 0);
		  itVertex = wrap::next(itFirstVertex, line2Start % form.vertexCount);
		  bSequence.emplace_back(itVertex->x, itVertex->y, 0);
		}
	  }
	  else {
		// add the first vertex to the output sequence
		auto itVertex = wrap::next(itFirstVertex, line1Start);
		Instance->oSequence.push_back(*itVertex);
	  }
	}
  }
  // determine which line should be the basis for the stitch count
  auto line1Length = lengths[line1End] - lengths[line1Start];
  auto line2Length = lengths[line2Start] - lengths[line2End];
  auto stitchCount = 0U;
  if (fabs(line1Length) > fabs(line2Length)) {
	stitchCount = wrap::round<uint32_t>(fabs(line2Length) / LineSpacing);
  }
  else {
	stitchCount = wrap::round<uint32_t>(fabs(line1Length) / LineSpacing);
  }
  // determine the segment stitch counts
  auto const line1Segments = line1End > line1Start ? line1End - line1Start : line1Start - line1End;
  auto const line2Segments = line2Start > line2End ? line2Start - line2End : line2End - line2Start;
  auto       line1StitchCounts = std::vector<uint32_t> {};
  line1StitchCounts.reserve(line1Segments);
  auto line2StitchCounts = std::vector<uint32_t> {};
  line2StitchCounts.reserve(line2Segments);
  auto iVertex            = line1Start;
  auto segmentStitchCount = 0U;
  // calculate the stitch counts for the first line
  for (auto iSegment = 0U; iSegment < line1Segments - 1U; ++iSegment) {
	auto const nextVertex = form::nxt(form, iVertex);
	auto const val = wrap::ceil<uint32_t>(((lengths[nextVertex] - lengths[iVertex]) / line1Length) *
	                                      wrap::toFloat(stitchCount));
	line1StitchCounts.push_back(val);
	segmentStitchCount += val;
	iVertex = form::nxt(form, iVertex);
  }
  line1StitchCounts.push_back(stitchCount - segmentStitchCount);
  auto iNextVertex   = line2Start;
  iVertex            = form::prv(form, iNextVertex);
  segmentStitchCount = 0;
  // calculate the stitch counts for the second line
  while (iVertex > line2End) {
	auto const val = wrap::ceil<uint32_t>(((lengths[iNextVertex] - lengths[iVertex]) / line2Length) *
	                                      wrap::toFloat(stitchCount));
	line2StitchCounts.push_back(val);
	segmentStitchCount += val;
	iNextVertex = form::prv(form, iNextVertex);
	iVertex     = form::prv(form, iNextVertex);
  }
  line2StitchCounts.push_back(stitchCount - segmentStitchCount);
  // setup the initial vertices and stitch points
  auto line1Point    = *wrap::next(itFirstVertex, line1Start);
  auto line1Next     = wrap::next(itFirstVertex, form::nxt(form, line1Start));
  auto line2Previous = wrap::next(itFirstVertex, form::prv(form, line2Start));
  auto line1Count    = line1StitchCounts[0];
  auto line2Count    = line2StitchCounts[0];
  auto iLine1Vertex  = line1Start;
  auto iLine2Vertex  = line2Start;

  auto itLine1Vertex = wrap::next(itFirstVertex, iLine1Vertex);
  auto line1Delta    = F_POINT {line1Next->x - itLine1Vertex->x, line1Next->y - itLine1Vertex->y};
  auto line2Point = iLine2Vertex == form.vertexCount ? *itFirstVertex : *wrap::next(itFirstVertex, iLine2Vertex);
  auto line2Delta = F_POINT {line2Previous->x - line2Point.x, line2Previous->y - line2Point.y};
  iLine1Vertex    = form::nxt(form, iLine1Vertex);
  iLine2Vertex    = form::prv(form, iLine2Vertex);
  auto line1Step =
      F_POINT {line1Delta.x / wrap::toFloat(line1Count), line1Delta.y / wrap::toFloat(line1Count)};
  auto line2Step =
      F_POINT {line2Delta.x / wrap::toFloat(line2Count), line2Delta.y / wrap::toFloat(line2Count)};
  bool flag        = true;
  auto iLine1Count = 1U;
  auto iLine2Count = 1U;
  auto stitchPoint = *wrap::next(itFirstVertex, line1Start); // intentional copy
  auto loop        = 0U;

  constexpr auto LOOPLIM = 20000U; // limit the iterations
  // loop until all the stitches have been calculated
  while (flag && loop < LOOPLIM) {
	flag = false;
	++loop;
	if (Instance->stateMap.test(StateFlag::FTHR)) {
	  // feathered satin
	  while (line1Count != 0U && line2Count != 0U) {
		line1Point += line1Step;
		line2Point += line2Step;
		if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
		  bSequence.emplace_back(line1Point.x, line1Point.y, 0);
		}
		else {
		  bSequence.emplace_back(line2Point.x, line2Point.y, 1);
		}
		--line1Count;
		--line2Count;
	  }
	}
	else {
	  if (Instance->stateMap.test(StateFlag::BARSAT)) {
		// bare satin
		while (line1Count != 0U && line2Count != 0U) {
		  line1Point += line1Step;
		  line2Point += line2Step;
		  // zig zag the stitches
		  if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
			bSequence.emplace_back(line1Point.x, line1Point.y, 0);
			bSequence.emplace_back(line2Point.x, line2Point.y, 1);
		  }
		  else {
			bSequence.emplace_back(line2Point.x, line2Point.y, 2);
			bSequence.emplace_back(line1Point.x, line1Point.y, 3);
		  }
		  --line1Count;
		  --line2Count;
		}
	  }
	  else {
		// normal satin
		while (line1Count != 0U && line2Count != 0U) {
		  line1Point += line1Step;
		  line2Point += line2Step;
		  // check the direction of the stitches and reverse line 1 or line 2
		  // depending on the flag for square ends
		  if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
			if (Instance->userFlagMap.test(UserFlag::SQRFIL)) {
			  form::filinu(line2Point, stitchPoint);
			  stitchPoint = line2Point;
			}
			form::filin(line1Point, stitchPoint);
			stitchPoint = line1Point;
		  }
		  else {
			if (Instance->userFlagMap.test(UserFlag::SQRFIL)) {
			  form::filinu(line1Point, stitchPoint);
			  stitchPoint = line1Point;
			}
			form::filin(line2Point, stitchPoint);
			stitchPoint = line2Point;
		  }
		  --line1Count;
		  --line2Count;
		}
	  }
	}
	// check if there are more stitches to calculate
	if (iLine1Count < line1Segments || iLine2Count < line2Segments) {
	  if (line1Count == 0U && iLine1Count < line1StitchCounts.size()) {
		// calculate the next segment stitch count for line 1
		line1Count           = line1StitchCounts[iLine1Count++];
		line1Next            = wrap::next(itFirstVertex, form::nxt(form, iLine1Vertex));
		auto itCurrentVertex = wrap::next(itFirstVertex, iLine1Vertex);
		line1Delta           = *line1Next - *itCurrentVertex;
		iLine1Vertex         = form::nxt(form, iLine1Vertex);
		line1Step            = line1Delta / wrap::toFloat(line1Count);
	  }
	  if (line2Count == 0U && iLine2Count < line2StitchCounts.size()) {
		// calculate the next segment stitch count for line 2
		line2Count           = line2StitchCounts[iLine2Count++];
		line2Previous        = wrap::next(itFirstVertex, form::prv(form, iLine2Vertex));
		auto itCurrentVertex = wrap::next(itFirstVertex, iLine2Vertex);
		line2Delta           = *line2Previous - *itCurrentVertex;
		iLine2Vertex         = form::prv(form, iLine2Vertex);
		line2Step            = line2Delta / wrap::toFloat(line2Count);
	  }
	  if ((line1Count != 0U || line2Count != 0U) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
		flag = true;
	  }
	}
  }
}

void satmf(FRM_HEAD const& form, std::vector<float> const& lengths) {
  auto start = 0U;
  if ((form.attribute & FRMEND) != 0U) {
	start = 1;
  }
  auto itGuide = wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex);
  satfn(form, lengths, start, itGuide->start, form.vertexCount, itGuide->finish);
  auto endGuideIndex = form.satinGuideCount;
  if (endGuideIndex != 0U) {
	--endGuideIndex;
  }
  auto const itEndGuide = wrap::next(itGuide, endGuideIndex);
  for (auto iGuide = 0U; iGuide < endGuideIndex; ++iGuide) {
	auto const& thisGuide = *itGuide;
	++itGuide;
	auto const& nextGuide = *itGuide;
	satfn(form, lengths, thisGuide.start, nextGuide.start, thisGuide.finish, nextGuide.finish);
  }
  if (auto const& endGuide = form.wordParam; endGuide != 0U) {
	satfn(form, lengths, itEndGuide->start, endGuide, itEndGuide->finish, endGuide + 1U);
  }
  else {
	if (itEndGuide->finish - itEndGuide->start > 2) {
	  auto const length =
	      ((lengths[itEndGuide->finish] - lengths[itEndGuide->start]) / 2) + lengths[itEndGuide->start];
	  auto iVertex = itEndGuide->start;
	  while (length > lengths[iVertex]) {
		++iVertex;
	  }
	  auto const deltaX     = lengths[iVertex] - length;
	  auto const prevVertex = iVertex - 1U;
	  if (auto const deltaY = length - lengths[prevVertex]; deltaY > deltaX) {
		--iVertex;
	  }
	  satfn(form, lengths, itEndGuide->start, iVertex, itEndGuide->finish, iVertex);
	}
	else {
	  satfn(form, lengths, itEndGuide->start, itEndGuide->start + 1, itEndGuide->finish, itEndGuide->start + 1U);
	}
  }
}

auto satOffset(const uint32_t& finish, const uint32_t& start, float const satinWidth) noexcept -> F_POINT {
  constexpr auto SATHRESH = 10.0F;

  auto const& formAngles = Instance->formAngles;
  auto        angle      = (formAngles.operator[](finish) - formAngles.operator[](start)) * HALF;
  auto const  factor     = std::clamp(1.0F / cos(angle), -SATHRESH, SATHRESH);

  auto const length = satinWidth * factor;

  angle += formAngles.operator[](start) + PI_FHALF;

  auto const xVal = length * cos(angle);
  auto const yVal = length * sin(angle);
  return F_POINT {xVal, yVal};
}

void satsbrd(uint32_t const formIndex) {
  auto& currentForm = Instance->formList.operator[](formIndex);
  clip::deleclp(ClosestFormToCursor);
  currentForm.edgeType = EDGEANGSAT;
  if (Instance->userFlagMap.test(UserFlag::DUND)) {
	currentForm.edgeType |= EGUND;
  }
  form::bsizpar(currentForm);
  currentForm.borderSize  = Instance->borderWidth;
  currentForm.edgeSpacing = LineSpacing / 2;
  currentForm.borderColor = ActiveColor;
  form::refilfn(formIndex);
}

auto satselfn() -> bool {
  auto        minimumLength = BIGFLOAT;
  auto const  stitchPoint   = thred::pxCor2stch(WinMsg.pt);
  auto const& formList      = Instance->formList;

  for (auto const& form : formList) {
	if (auto const layerCode =
	        gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(form.attribute & FRMLMSK) >> 1U);
	    ActiveLayer != 0U && layerCode != 0U && layerCode != ActiveLayer) {
	  continue;
	}
	auto const itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	auto vertexRange    = std::ranges::subrange(itVertex, wrap::next(itVertex, form.vertexCount));
	for (auto iVertex = 0U; auto const& vertex : vertexRange) {
	  auto const deltaX = stitchPoint.x - vertex.x;
	  auto const deltaY = stitchPoint.y - vertex.y;
	  if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
		minimumLength = length;
		ClosestFormToCursor = wrap::toUnsigned(&form - formList.data()); // index of the form. Possible with vectors
		ClosestVertexToCursor = iVertex;
	  }
	  ++iVertex;
	}
  }
  return minimumLength < FCLOSNUF;
}

void sbfn(std::vector<F_POINT> const& insidePoints, uint32_t const start, uint32_t const finish, F_POINT& stitchPoint) {
  auto const& outsidePoints = *OutsidePoints;
  auto        innerDelta    = F_POINT {(insidePoints[finish].x - insidePoints[start].x),
                             (insidePoints[finish].y - insidePoints[start].y)};
  auto        outerDelta    = F_POINT {(outsidePoints[finish].x - outsidePoints[start].x),
                             (outsidePoints[finish].y - outsidePoints[start].y)};

  auto const innerLength = std::hypot(innerDelta.x, innerDelta.y);
  auto const outerLength = std::hypot(outerDelta.x, outerDelta.y);
  auto       innerPoint  = F_POINT {insidePoints[start].x, insidePoints[start].y};
  auto       outerPoint  = F_POINT {outsidePoints[start].x, outsidePoints[start].y};
  auto       innerFlag   = false;
  auto       outerFlag   = false;
  if (!Instance->stateMap.testAndSet(StateFlag::SAT1)) {
	stitchPoint = insidePoints[start];
  }
  if (outerLength > innerLength) {
	innerFlag = true;
	if (auto intersection = F_POINT {}; form::linx(insidePoints, start, finish, intersection, outsidePoints)) {
	  innerDelta = F_POINT {};
	  innerPoint = intersection;
	}
  }
  else {
	outerFlag = true;
	if (auto intersection = F_POINT {}; form::linx(insidePoints, start, finish, intersection, outsidePoints)) {
	  outerDelta = F_POINT {};
	  outerPoint = intersection;
	}
  }
  auto count = wrap::round<uint32_t>((outerLength > innerLength ? outerLength : innerLength) / LineSpacing);
  if (count == 0U) {
	count = 1;
  }
  if (form::chkmax(count, wrap::toUnsigned(Instance->oSequence.size()))) {
	return;
  }
  auto satinBackup = std::vector<F_POINT> {}; // backup stitches in satin fills

  constexpr auto SATBUFSZ = 8U; // satin buffer size
  satinBackup.resize(SATBUFSZ);
  std::ranges::fill(satinBackup, F_POINT {BIGFLOAT, BIGFLOAT});
  auto const innerStep = F_POINT {innerDelta.x / wrap::toFloat(count), innerDelta.y / wrap::toFloat(count)};
  auto const outerStep = F_POINT {outerDelta.x / wrap::toFloat(count), outerDelta.y / wrap::toFloat(count)};
  auto satinBackupIndex = 0U;
  for (auto iStep = 0U; iStep < count; ++iStep) {
	innerPoint += innerStep;
	outerPoint += outerStep;
	if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
	  if (innerFlag) {
		auto const offsetDelta = F_POINT {innerPoint.x - stitchPoint.x, innerPoint.y - stitchPoint.y};
		auto const offsetLength = std::hypot(offsetDelta.x, offsetDelta.y);
		auto const offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
		auto const offsetStep   = F_POINT {offsetDelta.x / wrap::toFloat(offsetCount),
                                         offsetDelta.y / wrap::toFloat(offsetCount)};
		auto       offset       = innerPoint;
		while (chkbak(satinBackup, offset)) {
		  offset -= offsetStep;
		}
		filinsbw(satinBackup, offset, satinBackupIndex, stitchPoint);
	  }
	  else {
		form::filinsb(innerPoint, stitchPoint);
	  }
	}
	else {
	  if (outerFlag) {
		auto const offsetDelta = F_POINT {outerPoint.x - stitchPoint.x, outerPoint.y - stitchPoint.y};
		auto const offsetLength = std::hypot(offsetDelta.x, offsetDelta.y);
		auto const offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
		auto const offsetStep   = F_POINT {offsetDelta.x / wrap::toFloat(offsetCount),
                                         offsetDelta.y / wrap::toFloat(offsetCount)};
		auto       offset       = outerPoint;
		while (chkbak(satinBackup, offset)) {
		  offset -= offsetStep;
		}
		filinsbw(satinBackup, offset, satinBackupIndex, stitchPoint);
	  }
	  else {
		form::filinsb(outerPoint, stitchPoint);
	  }
	}
  }
}

void sfn(FRM_HEAD const& form, uint32_t startVertex, F_POINT& stitchPoint) {
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::nxt(form, startVertex);
	sbfn(*InsidePoints, startVertex, nextVertex, stitchPoint);
	startVertex = nextVertex;
  }
  Instance->oSequence.front() = Instance->oSequence.back();
}

void unsat() {
  if (Instance->stateMap.testAndReset(StateFlag::SHOSAT)) {
	satin::dusat();
  }
}

} // namespace

void satin::delsac(uint32_t const formIndex) {
  auto& formList    = Instance->formList;
  auto& currentForm = formList[formIndex];
  if (Instance->satinGuides.empty() || currentForm.type != SAT || currentForm.satinGuideCount == 0U) {
	formList[formIndex].satinGuideCount = 0;
	return;
  }
  auto const startGuide = wrap::next(Instance->satinGuides.cbegin(), currentForm.satinGuideIndex);
  auto const endGuide   = wrap::next(startGuide, currentForm.satinGuideCount);
  Instance->satinGuides.erase(startGuide, endGuide);
  auto const sgCount = currentForm.satinGuideCount;
  std::for_each(std::next(formList.begin(), wrap::toPtrdiff(formIndex + 1U)), formList.end(), [sgCount](auto& iForm) {
	if (iForm.type == SAT && iForm.satinGuideCount != 0U && iForm.satinGuideIndex >= sgCount) {
	  iForm.satinGuideIndex -= sgCount;
	}
  });
  currentForm.satinGuideCount = 0;
  currentForm.wordParam       = 0;
  currentForm.attribute &= NFRMEND;
}

void satin::spltsat(uint32_t const guideIndex) {
  auto& formList = Instance->formList;
  {
	auto const& currentForm = formList.operator[](ClosestFormToCursor);
	formList.insert(wrap::next(formList.cbegin(), ClosestFormToCursor), currentForm);
  }
  auto& firstForm = formList.operator[](ClosestFormToCursor); // insert may have invalidated reference
  // We are adding two additional vertices when splitting the form
  auto vertexBuffer = std::vector<F_POINT> {};
  vertexBuffer.resize(wrap::toSize(firstForm.vertexCount) + 2U);
  auto const lastFormVertex =
      wrap::next(Instance->formVertices.cbegin(), firstForm.vertexIndex + firstForm.vertexCount);
  Instance->formVertices.insert(lastFormVertex, 2, F_POINT {});
  for (auto formRange =
           std::ranges::subrange(wrap::next(formList.begin(), ClosestFormToCursor + 2U), formList.end());
       auto& form : formRange) {
	form.vertexIndex += 2;
  }
  auto iOldVertex = 0U;
  auto const currentGuide = *wrap::next(Instance->satinGuides.begin(), firstForm.satinGuideIndex + guideIndex);
  auto const oldLastVertex = currentGuide.start + (firstForm.vertexCount - currentGuide.finish) + 1U;
  auto       iNewVertex    = oldLastVertex + 1U;
  auto const itFirstVertex = wrap::next(Instance->formVertices.begin(), firstForm.vertexIndex);
  auto       itVertex      = itFirstVertex; // copy is intended
  for (auto iVertex = 0U; iVertex < firstForm.vertexCount; ++iVertex) {
	if (iVertex == currentGuide.start || iVertex == currentGuide.finish) {
	  vertexBuffer[iOldVertex++] = *itVertex;
	  if (iVertex == currentGuide.start) {
		vertexBuffer[iNewVertex++] = *itVertex;
	  }
	  else {
		vertexBuffer[oldLastVertex] = *itVertex;
	  }
	}
	else {
	  if (iVertex < currentGuide.start) {
		vertexBuffer[iOldVertex++] = *itVertex;
	  }
	  else {
		if (iVertex < currentGuide.finish) {
		  vertexBuffer[iNewVertex++] = *itVertex;
		}
		else {
		  vertexBuffer[iOldVertex++] = *itVertex;
		}
	  }
	}
	++itVertex;
  }
  itVertex = itFirstVertex;
  for (auto iVertex = 0U; iVertex < iNewVertex; ++iVertex) {
	*itVertex++ = vertexBuffer[iVertex];
  }
  firstForm.vertexCount = iOldVertex;
  auto& nextForm        = formList.operator[](wrap::toSize(ClosestFormToCursor) + 1U);
  nextForm.vertexCount  = iNewVertex - iOldVertex;
  nextForm.vertexIndex  = firstForm.vertexIndex + iOldVertex;
  firstForm.outline();
  nextForm.outline();
  auto       itGuide      = wrap::next(Instance->satinGuides.begin(), firstForm.satinGuideIndex);
  auto const finishOffset = currentGuide.finish - 1U - currentGuide.start;
  for (auto iGuide = 0U; iGuide < guideIndex; ++iGuide) {
	(itGuide++)->finish -= finishOffset;
  }
  if (firstForm.wordParam != 0U) {
	firstForm.wordParam = currentGuide.start;
  }
  for (auto iGuide = guideIndex; iGuide < firstForm.satinGuideCount; ++iGuide) {
	itGuide->start -= currentGuide.start - 1;
	itGuide->finish -= currentGuide.start - 1;
	++itGuide;
  }
  if (nextForm.wordParam != 0U) {
	nextForm.wordParam -= currentGuide.start - 1;
  }
  auto const offset      = firstForm.satinGuideIndex + guideIndex;
  auto const itThisGuide = wrap::next(Instance->satinGuides.cbegin(), offset);
  Instance->satinGuides.erase(itThisGuide);
  nextForm.satinGuideIndex  = firstForm.satinGuideIndex + guideIndex;
  nextForm.satinGuideCount  = firstForm.satinGuideCount - guideIndex - 1U;
  firstForm.satinGuideCount = guideIndex;
  for (auto formRange =
           std::ranges::subrange(wrap::next(formList.begin(), ClosestFormToCursor + 2U), formList.end());
       auto& form : formRange) {
	if (form.type == SAT && form.satinGuideCount != 0U && form.satinGuideIndex != 0U) {
	  --form.satinGuideIndex;
	}
  }
  if (firstForm.isEdgeClip()) {
	form::clpspac(firstForm.borderClipData, firstForm.clipEntries);
	for (auto formRange = std::ranges::subrange(wrap::next(formList.begin(), ClosestFormToCursor + 1U),
	                                            formList.end());
	     auto& form : formRange) {
	  form.borderClipData += firstForm.clipEntries;
	}
  }
  form::stchadj();
}

void satin::satknkt() {
  satclos();
  form::refil(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void satin::satsel() {
  if (!satselfn()) {
	return;
  }
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  thred::duzrat();
  StartPoint = ClosestVertexToCursor;
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + ClosestVertexToCursor);
  Instance->formLines.front() = form::sfCor2px(*itVertex);
  Instance->stateMap.reset(StateFlag::SHOCON);
  Instance->stateMap.set(StateFlag::SATCNKT);
  if (form.type == FRMFPOLY) {
	form.type = SAT;
  }
}

auto satin::scomp(SAT_CON const& arg1, SAT_CON const& arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (arg1.start < arg2.start) {
	return true;
  }
  if (arg2.start < arg1.start) {
	return false;
  }
  if (arg1.finish < arg2.finish) {
	return true;
  }
  if (arg2.finish < arg1.finish) {
	return false;
  }
  return false;
}

void satin::satadj(FRM_HEAD& form) {
  if (form.satinGuideCount == 0U) { // no guides so return
	return;
  }
  auto& currentGuidesCount = form.satinGuideCount;
  auto  interiorGuides     = std::vector<SAT_CON> {};
  interiorGuides.reserve(currentGuidesCount);
  auto const savedGuideCount = form.satinGuideCount;
  auto satinMap = BIT_SET_EX(form.vertexCount); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  // ensure all guide endpoints are on valid vertices
  auto itFirstGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex);
  for (auto vertexRange = std::ranges::subrange(itFirstGuide, wrap::next(itFirstGuide, form.satinGuideCount));
       auto& guide : vertexRange) {
	auto const endCount = form.vertexCount - 1;

	guide.finish = std::min(guide.finish, endCount);
	guide.start  = std::min(guide.start, endCount);
  }
  // remove any guides of 0 length
  std::copy_if(itFirstGuide,
               std::next(itFirstGuide, wrap::toPtrdiff(currentGuidesCount)),
               std::back_inserter(interiorGuides),
               [](const SAT_CON& guide) { return guide.start != guide.finish; });
  auto iDestination = wrap::toUnsigned(interiorGuides.size());
  if (currentGuidesCount > iDestination) {
	outDebugString(L"Removed {} zero distance guides\n", currentGuidesCount - iDestination);
	satcpy(form, interiorGuides, iDestination);
	itFirstGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex); // iterator may be invalidated by erase
	currentGuidesCount = iDestination;
  }
  auto const endGuide = form.wordParam;
  if (endGuide != 0U || (form.attribute & FRMEND) != 0U) {
	// there are end guides so set the satinMap for the next step
	satinMap.reset();
	if ((form.attribute & FRMEND) != 0U) {
	  satinMap.set(0);
	  satinMap.set(1);
	}
	if (endGuide != 0U) {
	  satinMap.set(endGuide);
	  satinMap.set((wrap::toSize(endGuide) + 1U) % form.vertexCount);
	}
	// check to see if any of the current guides are end guides and add to interiorGuides if not
	interiorGuides.clear();
	std::copy_if(itFirstGuide,
	             std::next(itFirstGuide, wrap::toPtrdiff(currentGuidesCount)),
	             std::back_inserter(interiorGuides),
	             [&satinMap](const auto& guide) {
	               return !satinMap.test(guide.start) && !satinMap.test(guide.finish);
	             });
	iDestination = wrap::toUnsigned(interiorGuides.size());
	if (currentGuidesCount > iDestination) {
	  outDebugString(L"Removed {} end guides\n", currentGuidesCount - iDestination);
	  satcpy(form, interiorGuides, iDestination);
	  itFirstGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex); // iterator may be invalidated by erase
	  currentGuidesCount = iDestination;
	}
	// remove any guides that start after the end guide
	if (endGuide != 0U) {
	  interiorGuides.clear();
	  std::copy_if(itFirstGuide,
	               std::next(itFirstGuide, wrap::toPtrdiff(currentGuidesCount)),
	               std::back_inserter(interiorGuides),
	               [endGuide](const auto& guide) { return guide.start < endGuide; });
	  iDestination = wrap::toUnsigned(interiorGuides.size());
	  if (currentGuidesCount > iDestination) {
		outDebugString(L"Removed {} reversed guides\n", currentGuidesCount - iDestination);
		satcpy(form, interiorGuides, iDestination);
		itFirstGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex); // iterator may be invalidated by erase
		form.satinGuideCount = iDestination;
		currentGuidesCount   = iDestination;
	  }
	}
  }

  if (currentGuidesCount != 0U && endGuide != 0) {
	satinMap.reset();
	{
	  auto itGuide = itFirstGuide;
	  for (auto iGuide = 0U; iGuide < currentGuidesCount; ++iGuide) {
		auto iForward = itGuide->start;
		if (iForward > endGuide - 1) {
		  iForward = endGuide - 1U;
		}
		if (satinMap.test_set(iForward)) {
		  auto iReverse = iForward;
		  if (iReverse != 0U) {
			--iReverse;
		  }
		  while (satinMap.test(iForward) && iForward < endGuide - 1) {
			++iForward;
		  }
		  while (iReverse != 0U && satinMap.test(iReverse)) {
			--iReverse;
		  }
		  if (satinMap.test(iForward) && satinMap.test(iReverse)) {
			break;
		  }
		  if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
			if (iForward - itGuide->start > itGuide->start - iReverse) {
			  satinMap.set(iReverse);
			}
			else {
			  satinMap.set(iForward);
			}
		  }
		  else {
			if (!satinMap.test(iForward)) {
			  satinMap.set(iReverse);
			}
			else {
			  satinMap.set(iForward);
			}
		  }
		}
		++itGuide;
	  }
	}
	{
	  auto iVertex = 0U;
	  auto itGuide = itFirstGuide; // intentional copy
	  while (iVertex < form.vertexCount) {
		if (auto const bitpos = satinMap.getFirst(); bitpos != BIGSIZE) {
		  iVertex = wrap::toUnsigned(bitpos);
		  if (iVertex < form.vertexCount) {
			(itGuide++)->start = iVertex;
		  }
		}
		else { // there are no more guides so exit the loop
		  break;
		}
	  }
	  currentGuidesCount = wrap::distance<uint32_t>(itFirstGuide, itGuide);
	}
	satinMap.reset();
	auto itGuide = itFirstGuide; // intentional copy
	for (auto iGuide = 0U; iGuide < currentGuidesCount; ++iGuide) {
	  auto iForward = itGuide->finish;
	  auto iReverse = iForward;
	  iForward      = std::min(iForward, form.vertexCount - 1U);
	  if (satinMap.test_set(iForward)) {
		if (iForward < form.vertexCount - 1U) {
		  ++iForward;
		}
		if (iReverse > endGuide + 1U) {
		  --iReverse;
		}
		while (satinMap.test(iForward) && iForward < form.vertexCount - 1U) {
		  ++iForward;
		}
		while (iReverse > endGuide - 1 && satinMap.test(iReverse)) {
		  --iReverse;
		}
		if (satinMap.test(iForward) && satinMap.test(iReverse)) {
		  break;
		}
		if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
		  if (iForward - itGuide->finish > itGuide->finish - iReverse) {
			satinMap.set(iReverse);
		  }
		  else {
			satinMap.set(iForward);
		  }
		}
		else {
		  if (!satinMap.test(iForward)) {
			satinMap.set(iForward);
		  }
		  else {
			satinMap.set(iReverse);
		  }
		}
	  }
	  ++itGuide;
	}
	auto iReverse = 0U;
	itGuide       = itFirstGuide;
	while (iReverse < form.vertexCount) {
	  if (auto const bitpos = satinMap.getLast(); bitpos != BIGSIZE) {
		iReverse = wrap::toUnsigned(bitpos);
		if (iReverse < form.vertexCount) {
		  (itGuide++)->finish = iReverse;
		}
	  }
	  else { // there are no more guides so exit the loop
		break;
	  }
	}
	auto iGuide        = wrap::distance<uint32_t>(itFirstGuide, itGuide);
	iGuide             = std::max(iGuide, currentGuidesCount);
	currentGuidesCount = iGuide;
	auto const end0    = form.vertexCount - endGuide - 2U;
	auto const end1    = endGuide - 2U;
	currentGuidesCount = std::min(currentGuidesCount, end0);
	currentGuidesCount = std::min(currentGuidesCount, end1);
  }
  else {
	// sort the guides
	auto const itStartGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex);
	auto const itEndGuide   = wrap::next(itStartGuide, form.satinGuideCount);
	std::sort(itStartGuide, itEndGuide, scomp);
  }
  if (form.satinGuideCount < savedGuideCount) {
	auto const iGuide = savedGuideCount - currentGuidesCount;
	outDebugString(L"Guides adjusted by {}, so updating forms\n", iGuide);
	std::for_each(wrap::next(Instance->formList.begin(), ClosestFormToCursor + 1U),
	              Instance->formList.end(),
	              [iGuide](auto& iForm) {
	                if (iForm.type == SAT && iForm.satinGuideIndex >= iGuide) {
		              iForm.satinGuideIndex -= iGuide;
	                }
	              });
  }
}

void satin::delcon(FRM_HEAD& form, uint32_t const GuideIndex) {
  auto const offset  = form.satinGuideIndex + GuideIndex;
  auto const itGuide = wrap::next(Instance->satinGuides.cbegin(), offset);
  Instance->satinGuides.erase(itGuide);
  std::for_each(wrap::next(Instance->formList.begin(), ClosestFormToCursor + 1U),
                Instance->formList.end(),
                [](auto& iForm) {
	              if (iForm.type == SAT && iForm.satinGuideCount != 0U && iForm.satinGuideIndex != 0U) {
	                --iForm.satinGuideIndex;
	              }
                });
  --form.satinGuideCount;
  if (form.fillType == SATF) {
	form::refil(ClosestFormToCursor);
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void satin::delspnt() {
  auto& formList = Instance->formList;

  auto& currentForm = formList.operator[](ClosestFormToCursor);
  if (form::chkdel(currentForm)) {
	Instance->stateMap.set(StateFlag::DELTO);
	thred::frmdel();
	Instance->stateMap.reset(StateFlag::FRMPSEL);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (currentForm.type == SAT && currentForm.fillType == SATF) {
	if (ClosestVertexToCursor < currentForm.wordParam) {
	  --currentForm.wordParam;
	}
	if (currentForm.satinGuideCount != 0U) {
	  auto const itStartGuide = wrap::next(Instance->satinGuides.begin(), currentForm.satinGuideIndex);
	  auto itGuide = itStartGuide;
	  auto iGuide  = 0U;
	  while (iGuide < currentForm.satinGuideCount) {
		if (itGuide->start != ClosestVertexToCursor && itGuide->finish != ClosestVertexToCursor) {
		  ++itGuide;
		  ++iGuide;
		}
		else {
		  break;
		}
	  }
	  if (iGuide < currentForm.satinGuideCount &&
	      (itGuide->start == ClosestVertexToCursor || itGuide->finish == ClosestVertexToCursor)) {
		while (iGuide < currentForm.satinGuideCount) {
		  *itGuide = *std::next(itGuide);
		  ++itGuide;
		  ++iGuide;
		}
		--currentForm.satinGuideCount;
		std::for_each(wrap::next(formList.begin(), ClosestFormToCursor + 1U), formList.end(), [](auto& iForm) {
		  if (iForm.type == SAT && iForm.satinGuideCount != 0U) {
			++iForm.satinGuideIndex;
		  }
		});
	  }
	  itGuide = itStartGuide;
	  for (iGuide = 0; iGuide < currentForm.satinGuideCount; ++iGuide) {
		if (itGuide->start > ClosestVertexToCursor) {
		  --itGuide->start;
		}
		if (itGuide->finish > ClosestVertexToCursor) {
		  --itGuide->finish;
		}
		++itGuide;
	  }
	}
  }
  auto const closestVertexIt =
      wrap::next(Instance->formVertices.cbegin(), currentForm.vertexIndex + ClosestVertexToCursor);
  Instance->formVertices.erase(closestVertexIt);
  --currentForm.vertexCount;
  if (ClosestVertexToCursor > currentForm.vertexCount - 1) {
	ClosestVertexToCursor = currentForm.vertexCount - 1U;
  }
  Instance->stateMap.set(StateFlag::FRMPSEL);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
	auto& form = formList.operator[](iForm);
	--form.vertexIndex;
  }
  auto const itVertex =
      wrap::next(Instance->formVertices.cbegin(), currentForm.vertexIndex + ClosestVertexToCursor);
  thred::ritfcor(*itVertex);
  displayText::ritnum(IDS_NUMPNT, ClosestVertexToCursor);
  currentForm.outline();
  if (itVertex->x < ZoomRect.left || itVertex->x > ZoomRect.right ||
      itVertex->y < ZoomRect.bottom || itVertex->y > ZoomRect.top) {
	thred::shft(*itVertex);
  }
  form::refil(ClosestFormToCursor);
}

void satin::satbrd() {
  if (!displayText::filmsgs(FML_ANGS)) {
	return;
  }
  auto& formList = Instance->formList;

  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  auto& form = formList.operator[](selectedForm);
	  if (Instance->userFlagMap.test(UserFlag::BLUNT)) {
		form.attribute |= gsl::narrow_cast<decltype(form.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		form.attribute &= NOBLNT;
	  }
	  satsbrd(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
  else {
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  auto& form = formList.operator[](ClosestFormToCursor);
	  if (Instance->userFlagMap.test(UserFlag::BLUNT)) {
		form.attribute |= gsl::narrow<decltype(form.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		form.attribute &= NOBLNT;
	  }
	  satsbrd(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
  }
}

void satin::ribon() {
  displayText::frm1pnt();
  if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	displayText::shoseln(IDS_FRM1MSG, IDS_CONVRIB);
	return;
  }
  auto& formList = Instance->formList;

  if (formList.operator[](ClosestFormToCursor).vertexCount < 2) {
	displayText::tabmsg(IDS_FRM2, false);
	return;
  }
  thred::savdo();
  auto const savedFormIndex = ClosestFormToCursor;
  satout(formList.operator[](ClosestFormToCursor), Instance->borderWidth);
  if (formList.empty()) {
	throw std::runtime_error("formList is empty");
  }
  auto       newForm            = FRM_HEAD {};
  auto const currentType        = formList.operator[](ClosestFormToCursor).type;
  auto const currentVertexCount = formList.operator[](ClosestFormToCursor).vertexCount;
  newForm.maxFillStitchLen      = MAXSIZ * PFGRAN;
  newForm.minFillStitchLen      = MinStitchLength;
  if (currentType == FRMLINE) {
	// Set blunt flags
	auto isBlunt = 0U;
	if (Instance->userFlagMap.test(UserFlag::BLUNT)) {
	  isBlunt = SBLNT | FBLNT;
	}
	satends(formList.operator[](ClosestFormToCursor), isBlunt, Instance->borderWidth);
  }
  newForm.vertexIndex    = currentType == FRMLINE ? thred::adflt(currentVertexCount * 2U)
                                                  : thred::adflt((currentVertexCount * 2U) + 2U);
  auto const startVertex = wrap::next(Instance->formVertices.begin(), newForm.vertexIndex);
  auto       itVertex    = startVertex;
  *itVertex++            = OutsidePoints->front();
  itVertex               = std::ranges::copy(*InsidePoints, itVertex).out;
  if (currentType != FRMLINE) {
	*itVertex++ = InsidePoints->front();
	*itVertex++ = OutsidePoints->front();

	newForm.underlayIndent = IniFile.underlayIndent;
  }
  itVertex = std::copy(OutsidePoints->rbegin(), wrap::next(OutsidePoints->rend(), -1), itVertex);
  auto const newVertexCount = wrap::distance<uint32_t>(startVertex, itVertex);

  newForm.type         = SAT;
  newForm.fillColor    = ActiveColor;
  newForm.fillSpacing  = LineSpacing;
  newForm.stitchLength = IniFile.maxStitchLength;
  newForm.vertexCount  = newVertexCount;
  newForm.attribute    = formList.operator[](ClosestFormToCursor).attribute;
  newForm.attribute &= FRMLMSK;
  newForm.attribute |= FRMEND;
  newForm.wordParam       = newVertexCount / 2;
  newForm.satinGuideCount = newForm.wordParam - 2U;
  newForm.satinGuideIndex = adsatk(newForm.satinGuideCount);
  if (Instance->stateMap.test(StateFlag::CNV2FTH)) {
	newForm.fillType              = FTHF;
	newForm.feather.ratio         = IniFile.featherRatio;
	newForm.feather.upCount       = IniFile.featherUpCount;
	newForm.feather.downCount     = IniFile.featherDownCount;
	newForm.feather.fillType      = IniFile.featherFillType;
	newForm.feather.minStitchSize = IniFile.featherMinStitchSize;
	newForm.extendedAttribute     = IniFile.featherType;
	newForm.feather.count         = IniFile.featherCount;
	newForm.feather.color         = (ActiveColor + 1U) & COLMSK;
  }
  else {
	newForm.fillType = SATF;
  }
  auto itGuide = wrap::next(Instance->satinGuides.begin(), newForm.satinGuideIndex);
  for (auto iGuide = 0U; iGuide < newForm.satinGuideCount; ++iGuide) {
	itGuide->start  = iGuide + 2U;
	itGuide->finish = newForm.vertexCount - iGuide - 1U;
	++itGuide;
  }
  newForm.outline();
  formList.push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  form::refilfn(ClosestFormToCursor);
  ClosestFormToCursor = savedFormIndex;
  Instance->stateMap.set(StateFlag::DELTO);
  thred::frmdel();
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  thred::coltab();
  Instance->stateMap.set(StateFlag::FORMSEL);
  Instance->stateMap.set(StateFlag::INIT);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void satin::slbrd(FRM_HEAD const& form) {
  auto const savedSpacing = LineSpacing;
  auto       stitchPoint  = F_POINT {};
  Instance->oSequence.clear();
  if ((form.edgeType & EGUND) != 0U) {
	auto const width = form.borderSize * URAT;
	satout(form, width);
	satends(form, form.attribute, width);
	Instance->stateMap.reset(StateFlag::SAT1);
	Instance->stateMap.reset(StateFlag::FILDIR);
	LineSpacing = USPAC;
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	  sbfn(*InsidePoints, iVertex, iVertex + 1U, stitchPoint);
	}
	Instance->stateMap.flip(StateFlag::FILDIR);
	for (auto iVertex = form.vertexCount - 1U; iVertex != 0; --iVertex) {
	  sbfn(*InsidePoints, iVertex, iVertex - 1, stitchPoint);
	}
  }
  satout(form, form.borderSize);
  satends(form, form.attribute, form.borderSize);
  LineSpacing = form.edgeSpacing;
  Instance->stateMap.reset(StateFlag::SAT1);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	sbfn(*InsidePoints, iVertex, iVertex + 1U, stitchPoint);
  }
  LineSpacing = savedSpacing;
}

void satin::satfil(FRM_HEAD& form) {
  auto const savedSpacing = LineSpacing;
  satadj(form);
  LineSpacing /= 2;
  Instance->oSequence.clear();
  Instance->bSequence.clear();
  Instance->stateMap.reset(StateFlag::SAT1);
  Instance->stateMap.reset(StateFlag::FILDIR);
  form.fillType = SATF;
  auto lengths  = std::vector<float> {};
  lengths.reserve(wrap::toSize(form.vertexCount) + 1U);
  auto length = 0.0F;
  lengths.push_back(length);
  auto const itFirstVertex    = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  auto       itPreviousVertex = itFirstVertex;
  auto       itCurrentVertex  = std::next(itPreviousVertex);
  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	auto const delta =
	    F_POINT {itCurrentVertex->x - itPreviousVertex->x, itCurrentVertex->y - itPreviousVertex->y};
	length += std::hypot(delta.x, delta.y);
	lengths.push_back(length);
	++itPreviousVertex;
	++itCurrentVertex;
  }
  auto const lastDelta =
      F_POINT {itFirstVertex->x - itPreviousVertex->x, itFirstVertex->y - itPreviousVertex->y};
  length += std::hypot(lastDelta.x, lastDelta.y);
  lengths.push_back(length);
  while (true) {
	if (auto const& endGuide = form.wordParam; endGuide != 0U) {
	  if (form.satinGuideCount != 0U) {
		satmf(form, lengths);
		break;
	  }
	  satfn(form, lengths, 1, endGuide, form.vertexCount, endGuide + 1U);
	  break;
	}
	if ((form.attribute & FRMEND) != 0U) {
	  if (form.satinGuideCount != 0U) {
		satmf(form, lengths);
		break;
	  }
	  if (form.vertexCount == 3 && (form.attribute & 1U) != 0) {
		satfn(form, lengths, 2, 3, 2, 1);
		break;
	  }
	  length       = (length - lengths[1]) * HALF;
	  auto iVertex = 1U;
	  if (!Instance->stateMap.test(StateFlag::BARSAT)) {
		auto const vNext = std::next(itFirstVertex);
		Instance->oSequence.push_back(*vNext);
	  }
	  while (iVertex < form.vertexCount + 1U && length > lengths[iVertex]) {
		++iVertex;
	  }
	  auto const deltaA     = lengths[iVertex] - length;
	  auto const prevVertex = iVertex - 1U;
	  if (auto const deltaB = length - lengths[prevVertex]; deltaB > deltaA) {
		--iVertex;
	  }
	  satfn(form, lengths, 1, iVertex, form.vertexCount, iVertex);
	  break;
	}
	if (form.satinGuideCount != 0U) {
	  satmf(form, lengths);
	  break;
	}
	length *= HALF;
	auto iVertex = 0U;
	if (!Instance->stateMap.test(StateFlag::BARSAT) && !Instance->stateMap.test(StateFlag::FTHR)) {
	  Instance->oSequence.push_back(*itFirstVertex);
	}
	while (length > lengths[iVertex]) {
	  ++iVertex;
	}
	auto const deltaA = lengths[iVertex] - length;
	if (auto const deltaB = length - lengths[wrap::toSize(iVertex) - 1U]; deltaB > deltaA) {
	  --iVertex;
	}
	satfn(form, lengths, 0, iVertex, form.vertexCount, iVertex);
	break;
  }
  LineSpacing = savedSpacing;
}

void satin::satfix() {
  auto&      tempPolygon = SatinInstance->TempPolygon;
  auto const vertexCount = wrap::toUnsigned(tempPolygon.size());
  auto       minSize     = 1U;
  auto&      formList    = Instance->formList;
  auto&      form        = formList.back();
  if (form.type == FRMFPOLY) {
	minSize = 2U;
  }
  if (tempPolygon.size() > minSize) {
	form.vertexIndex    = thred::adflt(vertexCount);
	auto const itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	std::ranges::copy(tempPolygon, itVertex);
	tempPolygon.clear();
	form.vertexCount = vertexCount;
	form.outline();
	form.satinGuideCount = 0;
	Instance->stateMap.set(StateFlag::INIT);
  }
  else {
	tempPolygon.clear();
	formList.pop_back();
  }
  Instance->stateMap.reset(StateFlag::SHOSAT);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void satin::dusat() noexcept {
  auto const  vertexCount = SatinInstance->TempPolygon.size();
  auto const& formLines   = Instance->formLines;
  auto const* line        = &formLines[vertexCount - 1U];
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, line, LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void satin::drwsat() {
  unsat();
  auto const vertexCount = SatinInstance->TempPolygon.size();
  auto&      formLines   = Instance->formLines;
  formLines.resize(vertexCount + 1U);
  formLines[vertexCount] =
      POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
  Instance->stateMap.set(StateFlag::SHOSAT);
  dusat();
}

void satin::satpnt0() {
  auto& formLines = Instance->formLines;
  formLines.clear();
  formLines.push_back(POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y});
  SatinInstance->TempPolygon.push_back(thred::pxCor2stch(WinMsg.pt));
  Instance->stateMap.set(StateFlag::SATPNT);
}

void satin::satpnt1() {
  unsat();
  auto const vertexCount = SatinInstance->TempPolygon.size();
  auto&      formLines   = Instance->formLines;
  formLines[vertexCount] =
      POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
  dusat();
  SatinInstance->TempPolygon.push_back(thred::pxCor2stch(WinMsg.pt));
  Instance->stateMap.set(StateFlag::RESTCH);
}

void satin::satzum() {
  Instance->stateMap.reset(StateFlag::SHOSAT);
  thred::duzrat();
  auto const vertexCount = wrap::toUnsigned(SatinInstance->TempPolygon.size());
  form::frmlin(SatinInstance->TempPolygon);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  SelectObject(StitchWindowMemDC, FormPen);
  wrap::polyline(StitchWindowMemDC, Instance->formLines.data(), vertexCount);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  drwsat();
}

void satin::satout(FRM_HEAD const& form, float satinWidth) {
  if (form.vertexCount == 0U) {
	return;
  }
  form::duangs(form);
  Instance->outsidePointList.resize(form.vertexCount);
  auto& insidePointList = Instance->insidePointList;
  insidePointList.resize(form.vertexCount);
  OutsidePoints = std::addressof(Instance->outsidePointList);
  InsidePoints  = &insidePointList;

  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	constexpr auto DEFWIDTH = 0.1F; // default satin width
	outfn(form, iVertex, iVertex + 1, DEFWIDTH);
  }
  auto count = 0U;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	if (form::cisin(form, InsidePoints->operator[](iVertex).x, InsidePoints->operator[](iVertex).y)) {
	  ++count;
	}
  }
  satinWidth *= HALF;
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	outfn(form, iVertex, iVertex + 1, satinWidth);
  }
  outfn(form, form.vertexCount - 1U, 0, satinWidth);
  Instance->stateMap.reset(StateFlag::INDIR);
  if (count < form.vertexCount / 2U) {
	Instance->stateMap.set(StateFlag::INDIR);
	OutsidePoints = std::addressof(Instance->insidePointList);
	InsidePoints  = std::addressof(Instance->outsidePointList);
  }
}

void satin::sbrd(FRM_HEAD const& form) {
  auto const savedSpacing = LineSpacing;
  auto const start        = form::getlast(form);
  auto       stitchPoint  = F_POINT {};
  Instance->stateMap.reset(StateFlag::SAT1);
  Instance->stateMap.reset(StateFlag::FILDIR);
  Instance->oSequence.clear();
  Instance->oSequence.emplace_back();
  if ((form.edgeType & EGUND) != 0U) {
	LineSpacing = USPAC;
	satout(form, form.borderSize * URAT);
	sfn(form, start, stitchPoint);
	Instance->stateMap.set(StateFlag::FILDIR);
	sfn(form, start, stitchPoint);
  }
  satout(form, form.borderSize);
  LineSpacing = form.edgeSpacing;
  sfn(form, start, stitchPoint);
  LineSpacing = savedSpacing;
}

auto satin::adsatk(uint32_t const count) -> uint32_t {
  auto const oldSize = wrap::toUnsigned(Instance->satinGuides.size());
  auto const dest    = Instance->satinGuides.end();
  auto constexpr VAL = SAT_CON {};
  Instance->satinGuides.insert(dest, count, VAL);
  return oldSize;
}

void satin::satinInit() noexcept {
  SatinInstance = SATIN_SINGLE::getInstance();
}
