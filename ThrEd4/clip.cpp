// Local Headers
#include "stdafx.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "point.h"
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
#include "gsl/util"
#pragma warning(pop)

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
// ReSharper disable CppUnusedIncludeDirective
#include <cstddef>
// ReSharper restore CppUnusedIncludeDirective
#include <iterator>
#include <ranges>
// ReSharper disable CppUnusedIncludeDirective
#include <type_traits>
// ReSharper restore CppUnusedIncludeDirective
#include <vector>

// clip internal namespace
namespace {
auto ClipReference = F_POINT {}; // clipboard reference formOrigin

// Definitions
void clpcrnr(FRM_HEAD const&       form,
             F_RECTANGLE const&    clipRect,
             std::vector<F_POINT>& clipFillData,
             uint32_t              vertex,
             F_POINT const&        rotationCenter);
auto clpsid(uint32_t                    vertexIndex,
            F_RECTANGLE const&          clipRect,
            std::vector<F_POINT> const& clipReversedData,
            std::vector<F_POINT>&       clipFillData,
            uint32_t                    start,
            uint32_t                    finish,
            F_POINT const&              rotationCenter) -> bool;
void clpsub(uint32_t formIndex, uint32_t cnt);
void clpxadj(std::vector<F_POINT>& tempClipPoints, std::vector<F_POINT>& chainEndPoints);
void duch(std::vector<F_POINT> const& chainEndPoints);
void duchfn(std::vector<F_POINT> const& chainEndPoints, uint32_t start, uint32_t finish);
void dufxlen(FRM_HEAD const& form, std::vector<F_POINT>& chainEndPoints);
void dulast(std::vector<F_POINT>& chainEndPoints);
void durev(F_RECTANGLE const& clipRect, std::vector<F_POINT>& clipReversedData) noexcept;
auto findclp(uint32_t formIndex) -> uint32_t;
void fxlen(FRM_HEAD const&           form,
           std::vector<F_POINT>&     chainEndPoints,
           std::vector<float> const& listSINEs,
           std::vector<float> const& listCOSINEs);
void fxlin(uint32_t                  vertexIndex,
           std::vector<F_POINT>&     chainEndPoints,
           std::vector<float> const& ListSINEs,
           std::vector<float> const& ListCOSINEs,
           F_POINT&                  moveToCoords,
           uint32_t                  currentSide,
           F_POINT&                  stitchPoint,
           float                     adjustedSpace,
           uint32_t                  nextStart);
void fxlit(uint32_t                  vertexIndex,
           std::vector<float> const& listSINEs,
           std::vector<float> const& listCOSINEs,
           F_POINT&                  moveToCoords,
           uint32_t                  currentSide,
           F_POINT&                  stitchPoint,
           uint32_t&                 adjCount,
           float                     adjustedSpace,
           uint32_t                  nextStart) noexcept(!std::is_same_v<ptrdiff_t, int>);
auto fxpnt(uint32_t                  vertexIndex,
           std::vector<float> const& listSINEs,
           std::vector<float> const& listCOSINEs,
           F_POINT&                  moveToCoords,
           uint32_t                  currentSide,
           F_POINT const&            stitchPoint,
           float                     adjustedSpace,
           uint32_t                  nextStart) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool;
void lincrnr(uint32_t                    vertexIndex,
             std::vector<F_POINT> const& clipReversedData,
             std::vector<F_POINT>&       clipFillData,
             float                       clipAngle,
             F_POINT const&              rotationCenter,
             uint32_t                    currentSide,
             F_POINT&                    stitchPoint,
             F_POINT const&              borderClipReference);
void linsid(uint32_t                    vertexIndex,
            std::vector<F_POINT> const& clipReversedData,
            std::vector<F_POINT>&       clipFillData,
            float                       clipAngle,
            F_POINT const&              vector0,
            F_POINT const&              rotationCenter,
            uint32_t                    currentSide,
            F_POINT&                    stitchPoint,
            F_POINT const&              borderClipReference);
auto nupnt(float clipAngle, F_POINT& moveToCoords, F_POINT const& stitchPoint) noexcept -> bool;
void picfn(FRM_HEAD const&       form,
           F_RECTANGLE const&    clipRect,
           std::vector<F_POINT>& clipFillData,
           uint32_t              start,
           uint32_t              finish,
           float                 spacing,
           F_POINT const&        rotationCenter);
auto ritclp(std::vector<F_POINT> const& clipFillData, F_POINT const& point) -> bool;
void setvct(uint32_t vertexIndex,
            uint32_t start,
            uint32_t finish,
            float&   clipAngle,
            F_POINT& vector0) noexcept(!std::is_same_v<ptrdiff_t, int>);
void xclpfn(std::vector<F_POINT> const& tempClipPoints,
            std::vector<F_POINT> const& chainEndPoints,
            uint32_t                    start,
            F_POINT const&              rotationCenter);

// Functions
void clpcrnr(FRM_HEAD const&       form,
             F_RECTANGLE const&    clipRect,
             std::vector<F_POINT>& clipFillData,
             uint32_t const        vertex,
             F_POINT const&        rotationCenter) {
  auto const  nextVertex = form::nxt(form, vertex);
  auto const  itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + nextVertex);
  auto const* points   = Instance->stateMap.test(StateFlag::INDIR) ? OutsidePoints : InsidePoints;
  if (nullptr == points) {
	return;
  }
  auto delta = F_POINT {points->operator[](nextVertex).x - itVertex->x,
                        points->operator[](nextVertex).y - itVertex->y};

  auto const rotationAngle = std::atan2(delta.y, delta.x) + PI_FHALF;
  auto const referencePoint = F_POINT_ATTR {wrap::midl(clipRect.right, clipRect.left), clipRect.top, 0U};
  ClipReference = thred::rotang1(referencePoint, rotationAngle, rotationCenter);
  auto iClip    = clipFillData.begin();
  for (auto& clip : Instance->clipBuffer) {
	*iClip = thred::rotang1(clip, rotationAngle, rotationCenter);
	++iClip;
  }
  auto const length = std::hypot(delta.x, delta.y);
  auto const ratio  = form::getplen() / length;
  delta *= ratio;
  auto const point = F_POINT {itVertex->x + delta.x, itVertex->y + delta.y};
  Instance->oSequence.push_back(*itVertex);
  Instance->oSequence.push_back(point);
  Instance->oSequence.push_back(*itVertex);
  Instance->oSequence.push_back(point);
  if (ritclp(clipFillData, point)) {
	Instance->oSequence.push_back(point);
	Instance->oSequence.push_back(*itVertex);
  }
}

auto clpsid(uint32_t const              vertexIndex,
            F_RECTANGLE const&          clipRect,
            std::vector<F_POINT> const& clipReversedData,
            std::vector<F_POINT>&       clipFillData,
            uint32_t const              start,
            uint32_t const              finish,
            F_POINT const&              rotationCenter) -> bool {
  auto const itVertex       = wrap::next(Instance->formVertices.cbegin(), vertexIndex);
  auto const itFinishVertex = wrap::next(itVertex, finish);
  auto const itStartVertex  = wrap::next(itVertex, start);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length             = std::hypot(delta.x, delta.y);
  auto const clipReferencePoint = F_POINT_ATTR {clipRect.left, clipRect.bottom, 0U};
  auto const rotationAngle      = std::atan2(delta.y, delta.x);

  ClipReference = thred::rotang1(clipReferencePoint, rotationAngle, rotationCenter);
  if (ClipRectSize.cx == 0.0F) {
	return false;
  }
  auto const clipCount = wrap::floor<uint32_t>(length / ClipRectSize.cx);
  if (clipCount == 0U) {
	return false;
  }
  auto const fClipCount = wrap::toFloat(clipCount);
  auto const remainder =
      clipCount > 1U
          ? ((length - fClipCount * ClipRectSize.cx) / (fClipCount - 1.0F) + ClipRectSize.cx) / length
          : (length - ClipRectSize.cx) / 2;
  auto const step         = F_POINT {delta.x * remainder, delta.y * remainder};
  auto       insertPoint  = *itStartVertex; // copy intended
  auto       reversedData = clipReversedData.begin();
  for (auto& data : clipFillData) {
	data = thred::rotangf(*reversedData, rotationAngle, rotationCenter);
	++reversedData;
  }
  for (auto stepCount = 0U; stepCount < clipCount; ++stepCount) {
	if (!ritclp(clipFillData, insertPoint)) {
	  break;
	}
	insertPoint += step;
  }
  return true;
}

void clpsub(uint32_t const formIndex, uint32_t const cnt) {
  for (auto  spForms = std::ranges::subrange(wrap::next(Instance->formList.begin(), formIndex + 1U),
                                            Instance->formList.end());
       auto& form : spForms) {
	if (form.isClipX()) {
	  form.clipIndex -= cnt;
	}
	if (form.isEdgeClipX()) {
	  form.borderClipData -= cnt;
	}
  }
}

void clpxadj(std::vector<F_POINT>& tempClipPoints, std::vector<F_POINT>& chainEndPoints) {
  dulast(chainEndPoints);
  auto& clipBuffer = Instance->clipBuffer;

  if (auto const& form = Instance->formList.operator[](ClosestFormToCursor); form.type == FRMLINE) {
	auto const pivot = ClipRectSize.cy / 2;
	std::ranges::transform(clipBuffer, std::back_inserter(tempClipPoints), [&pivot](auto& clip) noexcept {
	  return F_POINT {clip.x, (-clip.y + pivot)};
	});
	return;
  }
  std::ranges::transform(clipBuffer, std::back_inserter(tempClipPoints), [](auto& clip) noexcept {
	return F_POINT {clip.x, (-clip.y)};
  });
}

void duch(std::vector<F_POINT> const& chainEndPoints) {
  Instance->oSequence.clear();
  auto chainLength = wrap::toUnsigned(chainEndPoints.size());
  if (chainLength <= 2U) {
	displayText::tabmsg(IDS_CHANSMAL, false);
	return;
  }
  --chainLength;
  for (auto iPoint = 0U; iPoint < chainLength - 1U; ++iPoint) {
	duchfn(chainEndPoints, iPoint, iPoint + 1U);
  }
  if (auto const& form = Instance->formList.operator[](ClosestFormToCursor); form.type != FRMLINE) {
	duchfn(chainEndPoints, chainLength - 1, 0);
	Instance->oSequence.push_back(chainEndPoints[chainLength]);
	return;
  }
  duchfn(chainEndPoints, chainLength - 1, chainLength);
  constexpr auto CHLEN = size_t {8U}; // number of stitches in a chain

  auto backupAt = CHLEN;
  if (Instance->stateMap.test(StateFlag::LINCHN)) {
	--backupAt;
  }
  if (Instance->oSequence.size() >= backupAt) {
	Instance->oSequence.operator[](Instance->oSequence.size() - backupAt) = chainEndPoints[chainLength];
  }
  Instance->oSequence.push_back(chainEndPoints[chainLength]);
}

void duchfn(std::vector<F_POINT> const& chainEndPoints, uint32_t const start, uint32_t const finish) {
  constexpr auto CHAINLEN = 10U; // Chain length
  constexpr auto CHAIN_SEQUENCE = std::array<uint32_t, CHAINLEN> {0, 1, 2, 3, 0, 1, 4, 3, 0, 3}; // chain stitch sequence
  auto chainPoint = std::vector<F_POINT> {};

  constexpr auto CHPOINTS = 5U; // chainPoint size
  chainPoint.resize(CHPOINTS);
  auto delta = F_POINT {(chainEndPoints[finish].x - chainEndPoints[start].x),
                        (chainEndPoints[finish].y - chainEndPoints[start].y)};

  auto const& form       = Instance->formList.operator[](ClosestFormToCursor);
  auto const lengthDelta = F_POINT {(delta.x * form.edgeStitchLen), (delta.y * form.edgeStitchLen)};
  auto const angle       = std::atan2(delta.y, delta.x) + PI_FHALF;
  auto const offset      = F_POINT {(cos(angle) * form.borderSize), (sin(angle) * form.borderSize)};
  auto const middleXcoord = chainEndPoints[start].x + lengthDelta.x;
  auto const middleYcoord = chainEndPoints[start].y + lengthDelta.y;
  chainPoint[0]           = chainEndPoints[start];
  chainPoint[4]           = chainEndPoints[finish];
  chainPoint[1]           = F_POINT {middleXcoord + offset.x, middleYcoord + offset.y};
  chainPoint[3]           = F_POINT {middleXcoord - offset.x, middleYcoord - offset.y};
  if (finish < chainEndPoints.size() - 1U) {
	delta = F_POINT {chainEndPoints[wrap::toSize(finish) + 1U].x - chainEndPoints[finish].x,
	                 chainEndPoints[wrap::toSize(finish) + 1U].y - chainEndPoints[finish].y};
  }
  else {
	delta = F_POINT {chainEndPoints[finish].x - chainEndPoints[finish - 1U].x,
	                 chainEndPoints[finish].y - chainEndPoints[finish - 1U].y};
  }
  constexpr auto CHFACTOR = 4.0F; // Chain factor
  chainPoint[2]           = F_POINT {chainEndPoints[finish].x + (delta.x / CHFACTOR),
                           chainEndPoints[finish].y + (delta.y / CHFACTOR)};
  auto chainCount         = CHAINLEN;
  if (Instance->stateMap.test(StateFlag::LINCHN)) {
	--chainCount;
  }
  for (auto iChain = 0U; iChain < chainCount; ++iChain) {
	Instance->oSequence.push_back(chainPoint.at(CHAIN_SEQUENCE.at(iChain)));
  }
}

void dufxlen(FRM_HEAD const& form, std::vector<F_POINT>& chainEndPoints) {
  form::duangs(form);
  auto listSINEs = std::vector<float> {};
  listSINEs.reserve(wrap::toSize(form.vertexCount) + 1U);
  auto listCOSINEs = std::vector<float> {};
  listCOSINEs.reserve(form.vertexCount);
  auto const& formAngles = Instance->formAngles;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	listSINEs.push_back(sin(formAngles.operator[](iVertex)));
	listCOSINEs.push_back(cos(formAngles.operator[](iVertex)));
  }
  auto const prevCount = form.vertexCount - 1U;
  listSINEs.push_back(sin(formAngles.front() > formAngles.operator[](prevCount)
                              ? formAngles.front() - formAngles.operator[](prevCount)
                              : formAngles.operator[](prevCount) - formAngles.front()));
  fxlen(form, chainEndPoints, listSINEs, listCOSINEs);
}

void dulast(std::vector<F_POINT>& chainEndPoints) {
  auto tempClipPoints = std::vector<F_POINT> {};
  tempClipPoints.reserve(chainEndPoints.size());
  if (!form::lastch()) {
	return;
  }
  auto minimumLength = BIGFLOAT;
  auto minimumIndex  = 0U;
  for (auto index = 0U; auto const& iPoint : chainEndPoints) {
	auto const deltaX = LastPoint.x - iPoint.x;
	auto const deltaY = LastPoint.y - iPoint.y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
	  minimumLength = length;
	  minimumIndex  = index;
	}
	++index;
  }
  if (minimumIndex != 0U) {
	if (minimumIndex < wrap::toUnsigned(chainEndPoints.size() - 1U)) {
	  auto const spPoints1 = std::ranges::subrange(wrap::next(chainEndPoints.begin(), minimumIndex),
	                                               std::prev(chainEndPoints.end(), 1));
	  tempClipPoints.insert(tempClipPoints.end(), spPoints1.begin(), spPoints1.end());
	}
	auto const spEnd = minimumIndex + 1U;
	auto const spPoints2 =
	    std::ranges::subrange(chainEndPoints.begin(), wrap::next(chainEndPoints.begin(), spEnd));
	tempClipPoints.insert(tempClipPoints.end(), spPoints2.begin(), spPoints2.end());
	chainEndPoints = tempClipPoints;
  }
}

void durev(F_RECTANGLE const& clipRect, std::vector<F_POINT>& clipReversedData) noexcept {
  auto& clipBuffer = Instance->clipBuffer;

  if (clipBuffer.empty()) {
	return;
  }
  if (auto const midpoint = wrap::midl(clipRect.right, clipRect.left); clipBuffer[0].x > midpoint) {
	auto iBuffer = clipBuffer.begin();
	for (auto& reversed : clipReversedData) {
	  reversed = F_POINT {clipRect.right - iBuffer->x, iBuffer->y};
	  ++iBuffer;
	}
  }
  else {
	auto iBuffer = clipBuffer.begin();
	for (auto& reversed : clipReversedData) {
	  reversed = *iBuffer;
	  ++iBuffer;
	}
  }
}

auto findclp(uint32_t const formIndex) -> uint32_t {
  auto& formList = Instance->formList;
  for (auto spForms = std::ranges::subrange(formList.begin(), wrap::next(formList.begin(), formIndex));
       auto& form : spForms | std::views::reverse) {
	if (form.isEdgeClip()) {
	  return form.borderClipData + form.clipEntries;
	}
	if (form.isClip()) {
	  return form.clipIndex + form.clipCount;
	}
  }
  return 0;
}

void fxlen(FRM_HEAD const&           form,
           std::vector<F_POINT>&     chainEndPoints,
           std::vector<float> const& listSINEs,
           std::vector<float> const& listCOSINEs) {
  auto       moveToCoords  = F_POINT {}; // moving formOrigin for clipboard fill
  auto       flag          = true;
  auto const itFirstVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  auto const vNext         = std::next(itFirstVertex);
  auto const minimumLength = form.edgeSpacing * form.edgeSpacing;
  for (auto const spVertices = std::ranges::subrange(vNext, wrap::next(itFirstVertex, form.vertexCount));
       auto const& iVertex : spVertices) {
	auto const deltaX = iVertex.x - itFirstVertex->x;
	auto const deltaY = iVertex.y - itFirstVertex->y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length > minimumLength) {
	  flag = false;
	  break;
	}
  }
  if (flag) {
	chainEndPoints.push_back(*itFirstVertex);
	chainEndPoints.push_back(*vNext);
	return;
  }
  auto const halfSpacing = form.edgeSpacing / 2.0F;

  auto adjustedSpace   = form.edgeSpacing;
  auto minimumSpacing  = form.edgeSpacing;
  auto minimumInterval = BIGFLOAT;
  auto loopCount       = 0U;
  auto initialCount    = 0U;
  auto smallestSpacing = 0.0F;
  auto largestSpacing  = 1.0F;
  auto nextStart       = 0U;

  constexpr auto ITLIMIT = 50U; // Iterate at most 50 times to try to guarantee convergence
  while (loopCount < ITLIMIT && largestSpacing - smallestSpacing > TNYFLOAT) {
	auto adjCount = 0U;
	// intentional copy
	auto stitchPoint = *itFirstVertex;
	for (auto currentSide = 0U; currentSide < form.vertexCount - 1U; ++currentSide) {
	  nextStart = currentSide + 1U;
	  fxlit(form.vertexIndex, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint, adjCount, adjustedSpace, nextStart);
	}
	if (form.type != FRMLINE) {
	  nextStart = 0;
	  fxlit(form.vertexIndex, listSINEs, listCOSINEs, moveToCoords, form.vertexCount - 1U, stitchPoint, adjCount, adjustedSpace, nextStart);
	}
	else {
	  nextStart = form.vertexCount - 1U;
	}
	auto const itVertex = wrap::next(itFirstVertex, nextStart);
	if (initialCount == 0U) {
	  initialCount    = adjCount;
	  smallestSpacing = adjustedSpace;
	  minimumInterval = std::hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
	  auto interval   = minimumInterval;
	  minimumSpacing  = adjustedSpace;
	  interval /= wrap::toFloat(initialCount);
	  adjustedSpace += interval * HALF;
	  largestSpacing = smallestSpacing + interval;
	}
	else {
	  auto interval = std::hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
	  if (interval > halfSpacing) {
		interval = form.edgeSpacing - interval;
	  }
	  if (interval < minimumInterval) {
		minimumInterval = interval;
		minimumSpacing  = adjustedSpace;
	  }
	  if (initialCount == adjCount) {
		smallestSpacing = adjustedSpace;
	  }
	  else {
		if (adjCount > initialCount) {
		  largestSpacing = adjustedSpace;
		}
		else {
		  smallestSpacing = adjustedSpace;
		}
	  }
	  adjustedSpace = smallestSpacing + (largestSpacing - smallestSpacing) / 2;
	}
	++loopCount;
  }
  // intentional copy
  auto stitchPoint = *itFirstVertex;
  adjustedSpace    = minimumSpacing;
  chainEndPoints.push_back(stitchPoint);
  for (auto currentSide = 0U; currentSide < form.vertexCount - 1U; ++currentSide) {
	nextStart = currentSide + 1U;
	fxlin(form.vertexIndex, chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint, adjustedSpace, nextStart);
  }
  if (form.type != FRMLINE) {
	nextStart = 0;
	fxlin(form.vertexIndex, chainEndPoints, listSINEs, listCOSINEs, moveToCoords, form.vertexCount - 1U, stitchPoint, adjustedSpace, nextStart);
  }
  auto const lastVertex = wrap::next(itFirstVertex, nextStart);
  if (auto const interval = std::hypot(lastVertex->x - stitchPoint.x, lastVertex->y - stitchPoint.y);
      interval > halfSpacing) {
	chainEndPoints.push_back(*lastVertex);
  }
  else {
	chainEndPoints.back() = *lastVertex;
  }
}

void fxlin(uint32_t const            vertexIndex,
           std::vector<F_POINT>&     chainEndPoints,
           std::vector<float> const& ListSINEs,
           std::vector<float> const& ListCOSINEs,
           F_POINT&                  moveToCoords,
           uint32_t const            currentSide,
           F_POINT&                  stitchPoint,
           float const               adjustedSpace,
           uint32_t const            nextStart) {
  if (!fxpnt(vertexIndex, ListSINEs, ListCOSINEs, moveToCoords, currentSide, stitchPoint, adjustedSpace, nextStart)) {
	return;
  }
  stitchPoint = moveToCoords;
  chainEndPoints.push_back(stitchPoint);
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), vertexIndex + nextStart);
  auto const length   = std::hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
  auto const count    = wrap::floor<uint32_t>(length / adjustedSpace);
  auto const delta =
      F_POINT {adjustedSpace * ListCOSINEs[currentSide], adjustedSpace * ListSINEs[currentSide]};
  for (auto iChain = 0U; iChain < count; ++iChain) {
	stitchPoint += delta;
	chainEndPoints.push_back(stitchPoint);
  }
}

void fxlit(uint32_t const            vertexIndex,
           std::vector<float> const& listSINEs,
           std::vector<float> const& listCOSINEs,
           F_POINT&                  moveToCoords,
           uint32_t const            currentSide,
           F_POINT&                  stitchPoint,
           uint32_t&                 adjCount,
           float const               adjustedSpace,
           uint32_t const            nextStart) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  if (!fxpnt(vertexIndex, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint, adjustedSpace, nextStart)) {
	return;
  }
  stitchPoint = moveToCoords;
  ++adjCount;
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), vertexIndex + nextStart);
  auto const length   = std::hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
  auto const count    = std::floor(length / adjustedSpace);
  auto const delta =
      F_POINT {adjustedSpace * listCOSINEs[currentSide], adjustedSpace * listSINEs[currentSide]} * count;
  stitchPoint += delta;
  adjCount += gsl::narrow_cast<uint32_t>(count);
}

auto fxpnt(uint32_t const            vertexIndex,
           std::vector<float> const& listSINEs,
           std::vector<float> const& listCOSINEs,
           F_POINT&                  moveToCoords,
           uint32_t const            currentSide,
           F_POINT const&            stitchPoint,
           float const               adjustedSpace,
           uint32_t const            nextStart) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool {
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), vertexIndex + nextStart);
  moveToCoords        = *itVertex;
  auto length         = std::hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
  if (length <= adjustedSpace) {
	return false;
  }
  constexpr auto ITLIMIT = 10U; // Iteration limit
  for (auto iGuess = 0U; iGuess < ITLIMIT; ++iGuess) {
	length           = std::hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
	auto const delta = adjustedSpace - length;
	moveToCoords += F_POINT {delta * listCOSINEs[currentSide], delta * listSINEs[currentSide]};
	if (constexpr auto DLTLIMIT = 0.01F; fabs(delta) < DLTLIMIT) {
	  break;
	}
  }
  return true;
}

void lincrnr(uint32_t const              vertexIndex,
             std::vector<F_POINT> const& clipReversedData,
             std::vector<F_POINT>&       clipFillData,
             float const                 clipAngle,
             F_POINT const&              rotationCenter,
             uint32_t const              currentSide,
             F_POINT&                    stitchPoint,
             F_POINT const&              borderClipReference) {
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), vertexIndex + currentSide + 2);
  auto       moveToCoords = *itVertex; // intended copy
  if (!nupnt(clipAngle, moveToCoords, stitchPoint)) {
	return;
  }
  auto const delta = F_POINT {moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y};
  auto const rotationAngle = std::atan2(delta.y, delta.x);
  ClipReference            = thred::rotangf(borderClipReference, rotationAngle, rotationCenter);
  auto reversedData        = clipReversedData.begin();
  for (auto& data : clipFillData) {
	data = thred::rotangf(*reversedData, rotationAngle, rotationCenter);
	++reversedData;
  }
  ritclp(clipFillData, stitchPoint);
  stitchPoint = moveToCoords;
}

void linsid(uint32_t const              vertexIndex,
            std::vector<F_POINT> const& clipReversedData,
            std::vector<F_POINT>&       clipFillData,
            float const                 clipAngle,
            F_POINT const&              vector0,
            F_POINT const&              rotationCenter,
            uint32_t const              currentSide,
            F_POINT&                    stitchPoint,
            F_POINT const&              borderClipReference) {
  auto const itVertex  = wrap::next(Instance->formVertices.cbegin(), vertexIndex + currentSide + 1);
  auto const length    = std::hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
  auto const clipCount = wrap::floor<uint32_t>(length / ClipRectSize.cx);
  if (clipCount == 0U) {
	return;
  }
  ClipReference     = thred::rotangf(borderClipReference, clipAngle, rotationCenter);
  auto reversedData = clipReversedData.begin();
  for (auto& data : clipFillData) {
	data = thred::rotangf(*reversedData, clipAngle, rotationCenter);
	++reversedData;
  }
  for (auto iClip = 0U; iClip < clipCount; ++iClip) {
	if (!ritclp(clipFillData, stitchPoint)) {
	  break;
	}
	stitchPoint += vector0;
  }
}

auto nupnt(float const clipAngle, F_POINT& moveToCoords, F_POINT const& stitchPoint) noexcept -> bool {
  auto const sinAngle = sin(clipAngle);
  auto const cosAngle = cos(clipAngle);
  auto       length   = std::hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
  if (length <= ClipRectSize.cx) {
	return false;
  }
  constexpr auto ITLIMIT = 10U; // Iteration limit
  for (auto step = 0U; step < ITLIMIT; ++step) {
	length           = std::hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
	auto const delta = ClipRectSize.cx - length;
	moveToCoords += F_POINT {delta * cosAngle, delta * sinAngle};
	if (constexpr auto DLTLIMIT = 0.01F; fabs(delta) < DLTLIMIT) {
	  break;
	}
  }
  return true;
}

void picfn(FRM_HEAD const&       form,
           F_RECTANGLE const&    clipRect,
           std::vector<F_POINT>& clipFillData,
           uint32_t const        start,
           uint32_t const        finish,
           float                 spacing,
           F_POINT const&        rotationCenter) {
  auto const itStartVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + start);
  auto const itFinishVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto       rotationAngle = std::atan2(-delta.x, delta.y);
  auto const length        = std::hypot(delta.x, delta.y);
  auto const referencePoint = F_POINT_ATTR {wrap::midl(clipRect.right, clipRect.left), clipRect.top, 0U};
  auto const outerStep = F_POINT {form.borderSize * cos(rotationAngle), form.borderSize * sin(rotationAngle)};
  spacing += ClipRectSize.cx;
  auto const count = wrap::round<uint32_t>(length / spacing);
  rotationAngle    = std::atan2(delta.y, delta.x);
  ClipReference    = thred::rotang1(referencePoint, rotationAngle, rotationCenter);
  if (count == 0U) {
	return;
  }
  auto step = F_POINT {};
  if (count > 1) {
	auto const tdub =
	    ((length - wrap::toFloat(count) * spacing) / (wrap::toFloat(count) - 1.0F) + spacing) / length;
	auto const val = F_POINT {delta.x * tdub, delta.y * tdub};
	step           = val;
  }
  auto iClip = clipFillData.begin();
  for (auto& clip : Instance->clipBuffer) {
	*iClip = thred::rotang1(clip, rotationAngle, rotationCenter);
	++iClip;
  }
  auto flag       = true;
  auto innerPoint = F_POINT {itStartVertex->x, itStartVertex->y};
  for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	auto const firstPoint = F_POINT {innerPoint.x + step.x, innerPoint.y + step.y};
	auto const outerPoint = F_POINT {firstPoint.x + outerStep.x, firstPoint.y + outerStep.y};
	Instance->oSequence.push_back(firstPoint);
	Instance->oSequence.push_back(innerPoint);
	Instance->oSequence.push_back(firstPoint);
	Instance->oSequence.push_back(outerPoint);
	Instance->oSequence.push_back(firstPoint);
	Instance->oSequence.push_back(outerPoint);
	if (!ritclp(clipFillData, outerPoint)) {
	  flag = false;
	  break;
	}
	Instance->oSequence.push_back(outerPoint);
	Instance->oSequence.push_back(firstPoint);
	innerPoint += step;
  }
  if (flag) {
	Instance->oSequence.push_back(*itFinishVertex);
	Instance->oSequence.push_back(innerPoint);
  }
}

auto ritclp(std::vector<F_POINT> const& clipFillData, F_POINT const& point) -> bool {
  auto const adjustedPoint = F_POINT {(point.x - ClipReference.x), (point.y - ClipReference.y)};
  if (form::chkmax(wrap::toUnsigned(clipFillData.size()), wrap::toUnsigned(Instance->oSequence.size()))) {
	return false;
  }
  std::ranges::transform(
      clipFillData, std::back_inserter(Instance->oSequence), [&adjustedPoint](auto const& data) noexcept {
	    return F_POINT {data.x + adjustedPoint.x, data.y + adjustedPoint.y};
      });
  return true;
}

void setvct(uint32_t const vertexIndex,
            uint32_t const start,
            uint32_t const finish,
            float&         clipAngle,
            F_POINT&       vector0) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto const itVertex       = wrap::next(Instance->formVertices.cbegin(), vertexIndex);
  auto const itStartVertex  = wrap::next(itVertex, start);
  auto const itFinishVertex = wrap::next(itVertex, finish);
  clipAngle = std::atan2(itFinishVertex->y - itStartVertex->y, itFinishVertex->x - itStartVertex->x);
  vector0 = F_POINT {ClipRectSize.cx * cos(clipAngle), ClipRectSize.cx * sin(clipAngle)};
}

void xclpfn(std::vector<F_POINT> const& tempClipPoints,
            std::vector<F_POINT> const& chainEndPoints,
            uint32_t const              start,
            F_POINT const&              rotationCenter) {
  auto const chainStartPoint = wrap::next(chainEndPoints.begin(), start);
  auto const chainNextPoint  = std::next(chainStartPoint);
  auto const delta =
      F_POINT {(chainNextPoint->x - chainStartPoint->x), (chainNextPoint->y - chainStartPoint->y)};
  auto const rotationAngle = std::atan2(delta.y, delta.x);
  for (auto const& clip : tempClipPoints) {
	auto const point = thred::rotangf(clip, rotationAngle, rotationCenter);
	Instance->oSequence.emplace_back(chainStartPoint->x + point.x, chainStartPoint->y + point.y);
  }
}

} // namespace

void clip::delmclp(uint32_t const formIndex) {
  if (Instance->clipPoints.empty()) {
	return;
  }
  auto& form = Instance->formList.operator[](formIndex);
  if (!form.isClip()) {
	return;
  }
  auto const destIndex   = findclp(formIndex);
  auto const itStartClip = wrap::next(Instance->clipPoints.cbegin(), destIndex);
  auto const itEndClip   = wrap::next(itStartClip, form.clipCount);
  Instance->clipPoints.erase(itStartClip, itEndClip);
  if (form.isEdgeClip()) {
	form.borderClipData -= form.clipCount;
  }
  clpsub(formIndex, form.clipCount);
  form.clipCount = 0;
}

void clip::deleclp(uint32_t const formIndex) {
  if (Instance->clipPoints.empty()) {
	return;
  }
  auto& form = Instance->formList.operator[](formIndex);
  if (!form.isEdgeClip()) {
	return;
  }
  auto destIndex = findclp(formIndex);
  if (form.isClipX()) {
	destIndex += form.clipCount;
  }
  auto const itStartClip = wrap::next(Instance->clipPoints.cbegin(), destIndex);
  auto const itEndClip   = wrap::next(itStartClip, form.clipEntries);
  Instance->clipPoints.erase(itStartClip, itEndClip);
  clpsub(formIndex, form.clipEntries);
  form.clipEntries    = 0;
  form.borderClipData = 0;
  form.edgeType       = 0;
}

void clip::delclps(uint32_t const formIndex) {
  deleclp(formIndex);
  delmclp(formIndex);
}

auto clip::nueclp(uint32_t const currentForm, uint32_t const count) -> uint32_t {
  auto& formList = Instance->formList;

  auto find = findclp(currentForm);
  if (auto const& form = formList.operator[](currentForm); form.isClip()) {
	find += form.clipCount;
  }
  auto const itClipPoint = wrap::next(Instance->clipPoints.cbegin(), find);
  auto constexpr VAL     = F_POINT {};
  Instance->clipPoints.insert(itClipPoint, count, VAL);
  auto itStart = wrap::next(formList.begin(), currentForm);
  if (itStart->isEdgeClipX()) {
	itStart->borderClipData += count;
  }
  ++itStart;
  for (auto spForms = std::ranges::subrange(itStart, formList.end()); auto& form : spForms) {
	if (form.isEdgeClipX()) {
	  form.borderClipData += count;
	}
	if (form.isClip()) {
	  form.clipIndex += count;
	}
  }
  return find;
}

auto clip::numclp(uint32_t const formIndex) -> uint32_t {
  auto const clipSize    = wrap::toUnsigned(Instance->clipBuffer.size());
  auto const find        = findclp(formIndex);
  auto const itClipPoint = wrap::next(Instance->clipPoints.cbegin(), find);
  auto constexpr VAL     = F_POINT {};
  Instance->clipPoints.insert(itClipPoint, clipSize, VAL);
  auto& formList = Instance->formList;

  auto itStart       = wrap::next(formList.begin(), formIndex);
  itStart->clipIndex = find;
  if (itStart->isEdgeClipX()) {
	itStart->borderClipData += clipSize;
  }
  ++itStart;
  for (auto spForms = std::ranges::subrange(itStart, formList.end()); auto& form : spForms) {
	if (form.isClipX()) {
	  form.clipIndex += clipSize;
	}
	if (form.isEdgeClipX()) {
	  form.borderClipData += clipSize;
	}
  }
  return find;
}

void clip::oclp(F_RECTANGLE& clipRect, uint32_t const clipIndex, uint32_t const clipEntries) {
  if (Instance->stateMap.test(StateFlag::NOCLP)) {
	return;
  }
  auto& clipBuffer = Instance->clipBuffer;

  clipBuffer.clear();
  if (clipEntries != 0U) {
	clipBuffer.reserve(clipEntries);
	for (auto const clipPoints =
	         std::ranges::subrange(wrap::next(Instance->clipPoints.begin(), clipIndex),
	                               wrap::next(Instance->clipPoints.begin(), clipIndex + clipEntries));
	     auto const& iClip : clipPoints) {
	  clipBuffer.emplace_back(iClip.x, iClip.y, 0);
	}
	auto minX = BIGFLOAT;
	auto minY = BIGFLOAT;
	auto maxX = LOWFLOAT;
	auto maxY = LOWFLOAT;
	for (auto const& clip : clipBuffer) {
	  minX = std::min(minX, clip.x);
	  minY = std::min(minY, clip.y);
	  maxX = std::max(maxX, clip.x);
	  maxY = std::max(maxY, clip.y);
	}
	clipRect = F_RECTANGLE {minX, maxY, maxX, minY};
  }
  else {
	clipRect = F_RECTANGLE {};
  }
  ClipRectSize.cx = clipRect.right - clipRect.left;
  ClipRectSize.cy = clipRect.top - clipRect.bottom;
}

void clip::clpout(float const width) {
  auto const& form = Instance->formList.operator[](ClosestFormToCursor);
  if (form.type == FRMLINE) {
	satin::satout(form, width);
	return;
  }
  satin::satout(form, ClipRectSize.cy);
  auto& insidePointList = Instance->insidePointList;
  insidePointList.clear();
  auto const itStartVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  auto const itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
  insidePointList.insert(insidePointList.end(), itStartVertex, itEndVertex);
  InsidePoints = &insidePointList;
}

void clip::clpbrd(FRM_HEAD const& form, F_RECTANGLE const& clipRect, uint32_t const startVertex) {
  Instance->oSequence.clear();
  Instance->stateMap.reset(StateFlag::CLPBAK);
  auto const clipStitchCount = Instance->clipBuffer.size();
  auto       clipFillData    = std::vector<F_POINT> {};
  clipFillData.resize(clipStitchCount);
  auto clipReversedData = std::vector<F_POINT> {};
  clipReversedData.resize(clipStitchCount);
  auto const rotationCenter =
      F_POINT {wrap::midl(clipRect.right, clipRect.left), wrap::midl(clipRect.top, clipRect.bottom)};
  ClipReference = F_POINT {clipRect.left, rotationCenter.y};
  durev(clipRect, clipReversedData);
  if (form.type == FRMLINE) {
	auto const itVertex    = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	auto       stitchPoint = *itVertex;
	auto       clipAngle   = 0.0F;       // for clipboard border fill
	auto       vector0     = F_POINT {}; // x size of the clipboard fill at the fill angle
	setvct(form.vertexIndex, 0, 1, clipAngle, vector0);
	// Since clipRect.bottom is always 0
	// Use 0 to align left edge of clip with beginning of line, clipRect.right / 2 if you want to
	// align the center of the clip with the beginning of the line
	// auto borderClipReference = F_POINT {clipRect.right / 2.0F, clipRect.top / 2.0F};
	auto const borderClipReference = F_POINT {0.0F, clipRect.top / 2.0F};
	auto const sideCount           = form.vertexCount - 2U;
	for (auto currentSide = 0U; currentSide < sideCount; ++currentSide) {
	  linsid(form.vertexIndex, clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide, stitchPoint, borderClipReference);
	  setvct(form.vertexIndex, currentSide + 1U, currentSide + 2U, clipAngle, vector0);
	  lincrnr(form.vertexIndex, clipReversedData, clipFillData, clipAngle, rotationCenter, currentSide, stitchPoint, borderClipReference);
	}
	linsid(form.vertexIndex, clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, sideCount, stitchPoint, borderClipReference);
	return;
  }
  clpout(ClipRectSize.cx / 2);
  auto reference     = startVertex;
  auto currentVertex = startVertex;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::prv(form, currentVertex);
	if (clpsid(form.vertexIndex, clipRect, clipReversedData, clipFillData, reference, nextVertex, rotationCenter)) {
	  reference = nextVertex;
	}
	currentVertex = nextVertex;
  }
}

void clip::duxclp(FRM_HEAD const& form) {
  auto chainEndPoints = std::vector<F_POINT> {};
  // reserve some memory and rely on push_back behaviour
  // and geometric memory re-allocation for efficiency
  constexpr auto CLRES = size_t {50U}; // testing shows this to be a good value
  chainEndPoints.reserve(CLRES);
  dufxlen(form, chainEndPoints);
  auto tempClipPoints = std::vector<F_POINT> {};
  tempClipPoints.reserve(Instance->clipBuffer.size());
  clpxadj(tempClipPoints, chainEndPoints);
  Instance->oSequence.clear();
  auto constexpr ROTATION_CENTER = F_POINT {};
  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(chainEndPoints.size() - 1U); ++iPoint) {
	xclpfn(tempClipPoints, chainEndPoints, iPoint, ROTATION_CENTER);
  }
  if (form.type != FRMLINE) {
	Instance->oSequence.push_back(chainEndPoints[0]);
  }
}

void clip::clpic(FRM_HEAD const& form, F_RECTANGLE const& clipRect) {
  auto clipFillData = std::vector<F_POINT> {};
  clipFillData.resize(Instance->clipBuffer.size());
  auto const rotationCenter =
      F_POINT {wrap::midl(clipRect.right, clipRect.left), wrap::midl(clipRect.top, clipRect.bottom)};
  Instance->oSequence.clear();
  Instance->stateMap.reset(StateFlag::CLPBAK);
  ClipReference = F_POINT {rotationCenter.y, clipRect.left};

  constexpr auto SATWIDTH = 20.0F;
  satin::satout(form, SATWIDTH);
  if (form.type == FRMLINE) {
	for (auto iVertex = 0U; iVertex < form.vertexCount - 2U; ++iVertex) {
	  picfn(form, clipRect, clipFillData, iVertex, iVertex + 1, form.edgeSpacing, rotationCenter);
	  clpcrnr(form, clipRect, clipFillData, iVertex, rotationCenter);
	}
	picfn(form, clipRect, clipFillData, form.vertexCount - 2, form.vertexCount - 1, form.edgeSpacing, rotationCenter);
	return;
  }
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  if (form.fillType == 0U) {
	Instance->oSequence.push_back(*itVertex);
  }
  auto currentVertex = 0U;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::nxt(form, currentVertex);
	picfn(form, clipRect, clipFillData, currentVertex, nextVertex, form.edgeSpacing, rotationCenter);
	clpcrnr(form, clipRect, clipFillData, currentVertex, rotationCenter);
	currentVertex = nextVertex;
  }
  itVertex = wrap::next(itVertex, currentVertex);
  Instance->oSequence.push_back(*itVertex);
}

void clip::chnfn(FRM_HEAD const& form) {
  auto chainEndPoints = std::vector<F_POINT> {};
  // reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
  constexpr auto CLRES = size_t {50U}; // testing shows this to be a good value
  chainEndPoints.reserve(CLRES);
  deleclp(ClosestFormToCursor);
  dufxlen(form, chainEndPoints);
  dulast(chainEndPoints);
  duch(chainEndPoints);
}
