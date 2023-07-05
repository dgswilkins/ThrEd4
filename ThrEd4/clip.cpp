// Local Headers
#include "stdafx.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "globals.h"
#include "satin.h"
#include "thred.h"

// Standard Libraries
#include <ranges>

// clip internal namespace
namespace ci {
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
           uint32_t                  nextStart) noexcept(!(std::is_same_v<ptrdiff_t, int>));
auto fxpnt(uint32_t                  vertexIndex,
           std::vector<float> const& listSINEs,
           std::vector<float> const& listCOSINEs,
           F_POINT&                  moveToCoords,
           uint32_t                  currentSide,
           F_POINT const&            stitchPoint,
           float                     adjustedSpace,
           uint32_t                  nextStart) noexcept(!(std::is_same_v<ptrdiff_t, int>)) -> bool;
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
            F_POINT& vector0) noexcept(!(std::is_same_v<ptrdiff_t, int>));
void xclpfn(std::vector<F_POINT> const& tempClipPoints,
            std::vector<F_POINT> const& chainEndPoints,
            uint32_t                    start,
            F_POINT const&              rotationCenter);
} // namespace ci

namespace {
auto ClipReference = F_POINT {}; // clipboard reference formOrigin
} // namespace

auto ci::findclp(uint32_t formIndex) -> uint32_t {
  for (auto spForms = std::ranges::subrange(FormList->begin(), wrap::next(FormList->begin(), formIndex));
       auto& form : spForms | std::views::reverse) {
	if (form.isEdgeClip()) {
	  return form.borderClipData + form.clipEntries;
	}
	if (form.isClip()) {
	  return form.angleOrClipData.clip + form.lengthOrCount.clipCount;
	}
  }
  return 0;
}

void ci::clpsub(uint32_t formIndex, uint32_t cnt) {
  for (auto spForms = std::ranges::subrange(wrap::next(FormList->begin(), formIndex + 1U), FormList->end());
       auto& form : spForms) {
	if (form.isClipX()) {
	  form.angleOrClipData.clip -= cnt;
	}
	if (form.isEdgeClipX()) {
	  form.borderClipData -= cnt;
	}
  }
}

void clip::delmclp(uint32_t formIndex) {
  if (!ClipPoints->empty()) {
	if (auto& form = FormList->operator[](formIndex); form.isClip()) {
	  auto const destIndex   = ci::findclp(formIndex);
	  auto const itStartClip = wrap::next(ClipPoints->cbegin(), destIndex);
	  auto const itEndClip   = wrap::next(itStartClip, form.lengthOrCount.clipCount);
	  ClipPoints->erase(itStartClip, itEndClip);
	  if (form.isEdgeClip()) {
		form.borderClipData -= form.lengthOrCount.clipCount;
	  }
	  ci::clpsub(formIndex, form.lengthOrCount.clipCount);
	  form.lengthOrCount.clipCount = 0;
	}
  }
}

void clip::deleclp(uint32_t formIndex) {
  if (ClipPoints->empty()) {
	return;
  }
  auto& form = FormList->operator[](formIndex);
  if (!form.isEdgeClip()) {
	return;
  }
  auto destIndex = ci::findclp(formIndex);
  if (form.isClipX()) {
	destIndex += form.lengthOrCount.clipCount;
  }
  auto const itStartClip = wrap::next(ClipPoints->cbegin(), destIndex);
  auto const itEndClip   = wrap::next(itStartClip, form.clipEntries);
  ClipPoints->erase(itStartClip, itEndClip);
  ci::clpsub(formIndex, form.clipEntries);
  form.clipEntries    = 0;
  form.borderClipData = 0;
  form.edgeType       = 0;
}

void clip::delclps(uint32_t formIndex) {
  clip::deleclp(formIndex);
  clip::delmclp(formIndex);
}

auto clip::nueclp(uint32_t currentForm, uint32_t count) -> uint32_t {
  auto find = ci::findclp(currentForm);
  if (auto const& form = FormList->operator[](currentForm); form.isClip()) {
	find += form.lengthOrCount.clipCount;
  }
  auto const itClipPoint = wrap::next(ClipPoints->cbegin(), find);
  auto constexpr VAL     = F_POINT {};
  ClipPoints->insert(itClipPoint, count, VAL);
  auto itStart = wrap::next(FormList->begin(), currentForm);
  if (itStart->isEdgeClipX()) {
	itStart->borderClipData += count;
  }
  ++itStart;
  for (auto spForms = std::ranges::subrange(itStart, FormList->end()); auto& form : spForms) {
	if (form.isEdgeClipX()) {
	  form.borderClipData += count;
	}
	if (form.isClip()) {
	  form.angleOrClipData.clip += count;
	}
  }
  return find;
}

auto clip::numclp(uint32_t formIndex) -> uint32_t {
  auto const clipSize    = wrap::toUnsigned(ClipBuffer->size());
  auto const find        = ci::findclp(formIndex);
  auto const itClipPoint = wrap::next(ClipPoints->cbegin(), find);
  auto constexpr VAL     = F_POINT {};
  ClipPoints->insert(itClipPoint, clipSize, VAL);
  auto itStart                  = wrap::next(FormList->begin(), formIndex);
  itStart->angleOrClipData.clip = find;
  if (itStart->isEdgeClipX()) {
	itStart->borderClipData += clipSize;
  }
  ++itStart;
  for (auto spForms = std::ranges::subrange(itStart, FormList->end()); auto& form : spForms) {
	if (form.isClipX()) {
	  form.angleOrClipData.clip += clipSize;
	}
	if (form.isEdgeClipX()) {
	  form.borderClipData += clipSize;
	}
  }
  return find;
}

void clip::oclp(F_RECTANGLE& clipRect, uint32_t clipIndex, uint32_t clipEntries) {
  if (StateMap->test(StateFlag::NOCLP)) {
	return;
  }
  ClipBuffer->clear();
  if (clipEntries != 0U) {
	ClipBuffer->reserve(clipEntries);
	for (auto const  clipPoints = std::ranges::subrange(wrap::next(ClipPoints->begin(), clipIndex),
                                                       wrap::next(ClipPoints->begin(), clipIndex + clipEntries));
	     auto const& iClip : clipPoints) {
	  ClipBuffer->emplace_back(F_POINT_ATTR {iClip.x, iClip.y, 0});
	}
	clipRect.left = clipRect.right = ClipBuffer->front().x;
	clipRect.bottom = clipRect.top = ClipBuffer->front().y;
	for (auto const& clip : *ClipBuffer) {
	  if (clip.x < clipRect.left) {
		clipRect.left = clip.x;
	  }
	  if (clip.x > clipRect.right) {
		clipRect.right = clip.x;
	  }
	  if (clip.y < clipRect.bottom) {
		clipRect.bottom = clip.y;
	  }
	  if (clip.y > clipRect.top) {
		clipRect.top = clip.y;
	  }
	}
  }
  else {
	clipRect = F_RECTANGLE {};
  }
  ClipRectSize.cx = clipRect.right - clipRect.left;
  ClipRectSize.cy = clipRect.top - clipRect.bottom;
}

void ci::durev(F_RECTANGLE const& clipRect, std::vector<F_POINT>& clipReversedData) noexcept {
  if (ClipBuffer->empty()) {
	return;
  }
  auto const midpoint = wrap::midl(clipRect.right, clipRect.left);
  if (auto const& clipBuffer = *ClipBuffer; clipBuffer[0].x > midpoint) {
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

void ci::setvct(uint32_t vertexIndex,
                uint32_t start,
                uint32_t finish,
                float&   clipAngle,
                F_POINT& vector0) noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  auto const itVertex       = wrap::next(FormVertices->cbegin(), vertexIndex);
  auto const itStartVertex  = wrap::next(itVertex, start);
  auto const itFinishVertex = wrap::next(itVertex, finish);
  clipAngle = atan2(itFinishVertex->y - itStartVertex->y, itFinishVertex->x - itStartVertex->x);
  vector0.x = ClipRectSize.cx * cos(clipAngle);
  vector0.y = ClipRectSize.cx * sin(clipAngle);
}

auto ci::nupnt(float clipAngle, F_POINT& moveToCoords, F_POINT const& stitchPoint) noexcept -> bool {
  auto const sinAngle = sin(clipAngle);
  auto const cosAngle = cos(clipAngle);
  auto       length   = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
  if (length <= ClipRectSize.cx) {
	return false;
  }
  constexpr auto ITLIMIT = 10U; // Iteration limit
  for (auto step = 0U; step < ITLIMIT; ++step) {
	length           = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
	auto const delta = ClipRectSize.cx - length;
	moveToCoords.x += delta * cosAngle;
	moveToCoords.y += delta * sinAngle;
	if (constexpr auto DLTLIMIT = 0.01F; fabs(delta) < DLTLIMIT) {
	  break;
	}
  }
  return true;
}

auto ci::ritclp(std::vector<F_POINT> const& clipFillData, F_POINT const& point) -> bool {
  auto const adjustedPoint = F_POINT {(point.x - ClipReference.x), (point.y - ClipReference.y)};
  if (form::chkmax(wrap::toUnsigned(clipFillData.size()), wrap::toUnsigned(OSequence->size()))) {
	return false;
  }
  std::ranges::transform(clipFillData, std::back_inserter(*OSequence), [&adjustedPoint](auto const& data) noexcept {
	return F_POINT {data.x + adjustedPoint.x, data.y + adjustedPoint.y};
  });
  return true;
}

void ci::lincrnr(uint32_t                    vertexIndex,
                 std::vector<F_POINT> const& clipReversedData,
                 std::vector<F_POINT>&       clipFillData,
                 float                       clipAngle,
                 F_POINT const&              rotationCenter,
                 uint32_t                    currentSide,
                 F_POINT&                    stitchPoint,
                 F_POINT const&              borderClipReference) {
  auto const itVertex     = wrap::next(FormVertices->cbegin(), vertexIndex + currentSide + 2);
  auto       moveToCoords = *itVertex; // intended copy
  if (!ci::nupnt(clipAngle, moveToCoords, stitchPoint)) {
	return;
  }
  auto const delta = F_POINT {moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y};
  auto const rotationAngle = atan2(delta.y, delta.x);
  ClipReference            = thred::rotangf(borderClipReference, rotationAngle, rotationCenter);
  auto reversedData        = clipReversedData.begin();
  for (auto& data : clipFillData) {
	data = thred::rotangf(*reversedData, rotationAngle, rotationCenter);
	++reversedData;
  }
  ci::ritclp(clipFillData, stitchPoint);
  stitchPoint = moveToCoords;
}

void ci::linsid(uint32_t                    vertexIndex,
                std::vector<F_POINT> const& clipReversedData,
                std::vector<F_POINT>&       clipFillData,
                float                       clipAngle,
                F_POINT const&              vector0,
                F_POINT const&              rotationCenter,
                uint32_t                    currentSide,
                F_POINT&                    stitchPoint,
                F_POINT const&              borderClipReference) {
  auto const itVertex  = wrap::next(FormVertices->cbegin(), vertexIndex + currentSide + 1);
  auto const length    = hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
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
	if (!ci::ritclp(clipFillData, stitchPoint)) {
	  break;
	}
	stitchPoint.x += vector0.x;
	stitchPoint.y += vector0.y;
  }
}

void clip::clpout(float width) {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (form.type == FRMLINE) {
	satin::satout(form, width);
	return;
  }
  satin::satout(form, ClipRectSize.cy);
  InsidePointList->clear();
  auto const itStartVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  auto const itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
  InsidePointList->insert(InsidePointList->end(), itStartVertex, itEndVertex);
  InsidePoints = InsidePointList;
}

auto ci::clpsid(uint32_t                    vertexIndex,
                F_RECTANGLE const&          clipRect,
                std::vector<F_POINT> const& clipReversedData,
                std::vector<F_POINT>&       clipFillData,
                uint32_t                    start,
                uint32_t                    finish,
                F_POINT const&              rotationCenter) -> bool {
  auto const itVertex       = wrap::next(FormVertices->cbegin(), vertexIndex);
  auto const itFinishVertex = wrap::next(itVertex, finish);
  auto const itStartVertex  = wrap::next(itVertex, start);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length             = hypot(delta.x, delta.y);
  auto const clipReferencePoint = F_POINT_ATTR {clipRect.left, clipRect.bottom, 0U};
  auto const rotationAngle      = atan2(delta.y, delta.x);

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
      (clipCount > 1U)
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
	if (!ci::ritclp(clipFillData, insertPoint)) {
	  break;
	}
	insertPoint.x += step.x;
	insertPoint.y += step.y;
  }
  return true;
}

void clip::clpbrd(FRM_HEAD const& form, F_RECTANGLE const& clipRect, uint32_t startVertex) {
  OSequence->clear();
  StateMap->reset(StateFlag::CLPBAK);
  auto const clipStitchCount = ClipBuffer->size();
  auto       clipFillData    = std::vector<F_POINT> {};
  clipFillData.resize(clipStitchCount);
  auto clipReversedData = std::vector<F_POINT> {};
  clipReversedData.resize(clipStitchCount);
  auto const rotationCenter =
      F_POINT {wrap::midl(clipRect.right, clipRect.left), wrap::midl(clipRect.top, clipRect.bottom)};
  ClipReference.x = clipRect.left;
  ClipReference.y = rotationCenter.y;
  ci::durev(clipRect, clipReversedData);
  if (form.type == FRMLINE) {
	auto const itVertex    = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	auto       stitchPoint = *itVertex;
	auto       clipAngle   = 0.0F;       // for clipboard border fill
	auto       vector0     = F_POINT {}; // x size of the clipboard fill at the fill angle
	ci::setvct(form.vertexIndex, 0, 1, clipAngle, vector0);
	// Since clipRect.bottom is always 0
	// Use 0 to align left edge of clip with beginning of line, clipRect.right / 2 if you want to
	// align the center of the clip with the beginning of the line
	// auto borderClipReference = F_POINT {clipRect.right / 2.0F, clipRect.top / 2.0F};
	auto const borderClipReference = F_POINT {0.0F, clipRect.top / 2.0F};
	auto const sideCount           = form.vertexCount - 2U;
	for (auto currentSide = 0U; currentSide < sideCount; ++currentSide) {
	  ci::linsid(form.vertexIndex, clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide, stitchPoint, borderClipReference);
	  ci::setvct(form.vertexIndex, currentSide + 1U, currentSide + 2U, clipAngle, vector0);
	  ci::lincrnr(form.vertexIndex, clipReversedData, clipFillData, clipAngle, rotationCenter, currentSide, stitchPoint, borderClipReference);
	}
	ci::linsid(form.vertexIndex, clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, sideCount, stitchPoint, borderClipReference);
	return;
  }
  clpout(ClipRectSize.cx / 2);
  auto reference     = startVertex;
  auto currentVertex = startVertex;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::prv(form, currentVertex);
	if (ci::clpsid(form.vertexIndex, clipRect, clipReversedData, clipFillData, reference, nextVertex, rotationCenter)) {
	  reference = nextVertex;
	}
	currentVertex = nextVertex;
  }
}

auto ci::fxpnt(uint32_t                  vertexIndex,
               std::vector<float> const& listSINEs,
               std::vector<float> const& listCOSINEs,
               F_POINT&                  moveToCoords,
               uint32_t                  currentSide,
               F_POINT const&            stitchPoint,
               float                     adjustedSpace,
               uint32_t nextStart) noexcept(!(std::is_same_v<ptrdiff_t, int>)) -> bool {
  auto const itVertex = wrap::next(FormVertices->cbegin(), vertexIndex + nextStart);
  moveToCoords        = *itVertex;
  auto length         = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
  if (length <= adjustedSpace) {
	return false;
  }
  constexpr auto ITLIMIT = 10U; // Iteration limit
  for (auto iGuess = 0U; iGuess < ITLIMIT; ++iGuess) {
	length           = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
	auto const delta = adjustedSpace - length;
	moveToCoords.x += delta * listCOSINEs[currentSide];
	moveToCoords.y += delta * listSINEs[currentSide];
	if (constexpr auto DLTLIMIT = 0.01F; fabs(delta) < DLTLIMIT) {
	  break;
	}
  }
  return true;
}

void ci::fxlit(uint32_t                  vertexIndex,
               std::vector<float> const& listSINEs,
               std::vector<float> const& listCOSINEs,
               F_POINT&                  moveToCoords,
               uint32_t                  currentSide,
               F_POINT&                  stitchPoint,
               uint32_t&                 adjCount,
               float                     adjustedSpace,
               uint32_t                  nextStart) noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  if (!ci::fxpnt(vertexIndex, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint, adjustedSpace, nextStart)) {
	return;
  }
  stitchPoint = moveToCoords;
  ++adjCount;
  auto const itVertex = wrap::next(FormVertices->cbegin(), vertexIndex + nextStart);
  auto const length   = hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
  auto const count    = std::floor(length / adjustedSpace);
  auto const delta =
      F_POINT {adjustedSpace * listCOSINEs[currentSide], adjustedSpace * listSINEs[currentSide]};
  stitchPoint.x += delta.x * count;
  stitchPoint.y += delta.y * count;
  adjCount += gsl::narrow_cast<uint32_t>(count);
}

void ci::fxlin(uint32_t                  vertexIndex,
               std::vector<F_POINT>&     chainEndPoints,
               std::vector<float> const& ListSINEs,
               std::vector<float> const& ListCOSINEs,
               F_POINT&                  moveToCoords,
               uint32_t                  currentSide,
               F_POINT&                  stitchPoint,
               float                     adjustedSpace,
               uint32_t                  nextStart) {
  if (!ci::fxpnt(vertexIndex, ListSINEs, ListCOSINEs, moveToCoords, currentSide, stitchPoint, adjustedSpace, nextStart)) {
	return;
  }
  stitchPoint = moveToCoords;
  chainEndPoints.push_back(stitchPoint);
  auto const itVertex = wrap::next(FormVertices->cbegin(), vertexIndex + nextStart);
  auto const length   = hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
  auto const count    = wrap::floor<uint32_t>(length / adjustedSpace);
  auto const delta =
      F_POINT {adjustedSpace * ListCOSINEs[currentSide], adjustedSpace * ListSINEs[currentSide]};
  for (auto iChain = 0U; iChain < count; ++iChain) {
	stitchPoint.x += delta.x;
	stitchPoint.y += delta.y;
	chainEndPoints.push_back(stitchPoint);
  }
}

void ci::fxlen(FRM_HEAD const&           form,
               std::vector<F_POINT>&     chainEndPoints,
               std::vector<float> const& listSINEs,
               std::vector<float> const& listCOSINEs) {
  auto       moveToCoords  = F_POINT {}; // moving formOrigin for clipboard fill
  auto       adjustedSpace = 0.0F;
  auto       flag          = true;
  auto const itFirstVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  auto const vNext         = std::next(itFirstVertex);
  for (auto const spVertices = std::ranges::subrange(vNext, wrap::next(itFirstVertex, form.vertexCount));
       auto const& iVertex : spVertices) {
	auto const length = hypot(iVertex.x - itFirstVertex->x, iVertex.y - itFirstVertex->y);
	if (length > form.edgeSpacing) {
	  flag = false;
	  break;
	}
	if (length > adjustedSpace) {
	  adjustedSpace = length;
	}
  }
  if (flag) {
	chainEndPoints.push_back(*itFirstVertex);
	chainEndPoints.push_back(*vNext);
	return;
  }
  adjustedSpace              = form.edgeSpacing;
  auto       minimumSpacing  = form.edgeSpacing;
  auto const halfSpacing     = adjustedSpace / 2.0F;
  auto       minimumInterval = BIGFLOAT;
  auto       loopCount       = 0U;
  auto       initialCount    = 0U;
  auto       smallestSpacing = 0.0F;
  auto       largestSpacing  = 1.0F;
  auto       nextStart       = 0U;

  constexpr auto ITLIMIT = 50U; // Iterate at most 50 times to try to guarantee convergence
  while (loopCount < ITLIMIT && (largestSpacing - smallestSpacing) > TNYFLOAT) {
	auto adjCount = 0U;
	// intentional copy
	auto stitchPoint = *itFirstVertex;
	for (auto currentSide = 0U; currentSide < form.vertexCount - 1U; ++currentSide) {
	  nextStart = currentSide + 1U;
	  ci::fxlit(form.vertexIndex, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint, adjCount, adjustedSpace, nextStart);
	}
	if (form.type != FRMLINE) {
	  nextStart = 0;
	  ci::fxlit(form.vertexIndex, listSINEs, listCOSINEs, moveToCoords, form.vertexCount - 1U, stitchPoint, adjCount, adjustedSpace, nextStart);
	}
	else {
	  nextStart = form.vertexCount - 1U;
	}
	auto const itVertex = wrap::next(itFirstVertex, nextStart);
	if (initialCount == 0U) {
	  initialCount    = adjCount;
	  smallestSpacing = adjustedSpace;
	  minimumInterval = hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
	  auto interval   = minimumInterval;
	  minimumSpacing  = adjustedSpace;
	  interval /= wrap::toFloat(initialCount);
	  adjustedSpace += interval / 2.0F;
	  largestSpacing = smallestSpacing + interval;
	}
	else {
	  auto interval = hypot(itVertex->x - stitchPoint.x, itVertex->y - stitchPoint.y);
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
	ci::fxlin(form.vertexIndex, chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint, adjustedSpace, nextStart);
  }
  if (form.type != FRMLINE) {
	nextStart = 0;
	ci::fxlin(form.vertexIndex, chainEndPoints, listSINEs, listCOSINEs, moveToCoords, form.vertexCount - 1U, stitchPoint, adjustedSpace, nextStart);
  }
  auto const lastVertex = wrap::next(itFirstVertex, nextStart);
  if (auto const interval = hypot(lastVertex->x - stitchPoint.x, lastVertex->y - stitchPoint.y);
      interval > halfSpacing) {
	chainEndPoints.push_back(*lastVertex);
  }
  else {
	chainEndPoints.back() = *lastVertex;
  }
}

void ci::dufxlen(FRM_HEAD const& form, std::vector<F_POINT>& chainEndPoints) {
  form::duangs(form);
  auto listSINEs = std::vector<float> {};
  listSINEs.reserve(wrap::toSize(form.vertexCount) + 1U);
  auto listCOSINEs = std::vector<float> {};
  listCOSINEs.reserve(form.vertexCount);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	listSINEs.push_back(sin(FormAngles->operator[](iVertex)));
	listCOSINEs.push_back(cos(FormAngles->operator[](iVertex)));
  }
  listSINEs.push_back(sin((FormAngles->front() > FormAngles->operator[](form.vertexCount - 1U))
                              ? (FormAngles->front() - FormAngles->operator[](form.vertexCount - 1U))
                              : (FormAngles->operator[](form.vertexCount - 1U) - FormAngles->front())));
  ci::fxlen(form, chainEndPoints, listSINEs, listCOSINEs);
}

void ci::dulast(std::vector<F_POINT>& chainEndPoints) {
  auto tempClipPoints = std::vector<F_POINT> {};
  tempClipPoints.reserve(chainEndPoints.size());
  if (!form::lastch()) {
	return;
  }
  auto minimumLength = BIGFLOAT;
  auto minimumIndex  = 0U;
  for (auto index = 0U; auto const& iPoint : chainEndPoints) {
	if (auto const length = hypot(LastPoint.x - iPoint.x, LastPoint.y - iPoint.y); length < minimumLength) {
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

void ci::clpxadj(std::vector<F_POINT>& tempClipPoints, std::vector<F_POINT>& chainEndPoints) {
  ci::dulast(chainEndPoints);
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (form.type == FRMLINE) {
	auto const pivot = ClipRectSize.cy / 2;
	std::ranges::transform(*ClipBuffer, std::back_inserter(tempClipPoints), [&pivot](auto& clip) noexcept {
	  return F_POINT {clip.x, (-clip.y + pivot)};
	});
	return;
  }
  std::ranges::transform(*ClipBuffer, std::back_inserter(tempClipPoints), [](auto& clip) noexcept {
	return F_POINT {clip.x, (-clip.y)};
  });
}

void ci::xclpfn(std::vector<F_POINT> const& tempClipPoints,
                std::vector<F_POINT> const& chainEndPoints,
                uint32_t                    start,
                F_POINT const&              rotationCenter) {
  auto const chainStartPoint = wrap::next(chainEndPoints.begin(), start);
  auto const chainNextPoint  = std::next(chainStartPoint);
  auto const delta =
      F_POINT {(chainNextPoint->x - chainStartPoint->x), (chainNextPoint->y - chainStartPoint->y)};
  auto const rotationAngle = atan2(delta.y, delta.x);
  for (auto const& clip : tempClipPoints) {
	auto const point = thred::rotangf(clip, rotationAngle, rotationCenter);
	OSequence->push_back(F_POINT {chainStartPoint->x + point.x, chainStartPoint->y + point.y});
  }
}

void clip::duxclp(FRM_HEAD const& form) {
  auto chainEndPoints = std::vector<F_POINT> {};
  // reserve some memory and rely on push_back behaviour
  // and geometric memory re-allocation for efficiency
  chainEndPoints.reserve(50U);
  ci::dufxlen(form, chainEndPoints);
  auto tempClipPoints = std::vector<F_POINT> {};
  tempClipPoints.reserve(ClipBuffer->size());
  ci::clpxadj(tempClipPoints, chainEndPoints);
  OSequence->clear();
  auto constexpr ROTATION_CENTER = F_POINT {};
  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(chainEndPoints.size() - 1U); ++iPoint) {
	ci::xclpfn(tempClipPoints, chainEndPoints, iPoint, ROTATION_CENTER);
  }
  if (form.type != FRMLINE) {
	OSequence->push_back(chainEndPoints[0]);
  }
}

void ci::clpcrnr(FRM_HEAD const&       form,
                 F_RECTANGLE const&    clipRect,
                 std::vector<F_POINT>& clipFillData,
                 uint32_t              vertex,
                 F_POINT const&        rotationCenter) {
  auto const  nextVertex = form::nxt(form, vertex);
  auto const  itVertex   = wrap::next(FormVertices->cbegin(), form.vertexIndex + nextVertex);
  auto const* points     = StateMap->test(StateFlag::INDIR) ? OutsidePoints : InsidePoints;
  if (nullptr == points) {
	return;
  }
  auto delta = F_POINT {points->operator[](nextVertex).x - itVertex->x,
                        points->operator[](nextVertex).y - itVertex->y};

  auto const rotationAngle = atan2(delta.y, delta.x) + PI_FHALF;
  auto const referencePoint = F_POINT_ATTR {wrap::midl(clipRect.right, clipRect.left), clipRect.top, 0U};
  ClipReference = thred::rotang1(referencePoint, rotationAngle, rotationCenter);
  auto iClip    = clipFillData.begin();
  for (auto& clip : *ClipBuffer) {
	*iClip = thred::rotang1(clip, rotationAngle, rotationCenter);
	++iClip;
  }
  auto const length = hypot(delta.x, delta.y);
  auto const ratio  = form::getplen() / length;
  delta.x *= ratio;
  delta.y *= ratio;
  auto const point = F_POINT {itVertex->x + delta.x, itVertex->y + delta.y};
  OSequence->push_back(*itVertex);
  OSequence->push_back(point);
  OSequence->push_back(*itVertex);
  OSequence->push_back(point);
  if (ci::ritclp(clipFillData, point)) {
	OSequence->push_back(point);
	OSequence->push_back(*itVertex);
  }
}

void ci::picfn(FRM_HEAD const&       form,
               F_RECTANGLE const&    clipRect,
               std::vector<F_POINT>& clipFillData,
               uint32_t              start,
               uint32_t              finish,
               float                 spacing,
               F_POINT const&        rotationCenter) {
  auto const itStartVertex  = wrap::next(FormVertices->cbegin(), form.vertexIndex + start);
  auto const itFinishVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto       rotationAngle = atan2(-delta.x, delta.y);
  auto const length        = hypot(delta.x, delta.y);
  auto const referencePoint = F_POINT_ATTR {wrap::midl(clipRect.right, clipRect.left), clipRect.top, 0U};
  auto const outerStep = F_POINT {form.borderSize * cos(rotationAngle), form.borderSize * sin(rotationAngle)};
  spacing += ClipRectSize.cx;
  auto const count = wrap::round<uint32_t>(length / spacing);
  rotationAngle    = atan2(delta.y, delta.x);
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
  for (auto& clip : *ClipBuffer) {
	*iClip = thred::rotang1(clip, rotationAngle, rotationCenter);
	++iClip;
  }
  auto flag       = true;
  auto innerPoint = F_POINT {itStartVertex->x, itStartVertex->y};
  for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	auto const firstPoint = F_POINT {innerPoint.x + step.x, innerPoint.y + step.y};
	auto const outerPoint = F_POINT {firstPoint.x + outerStep.x, firstPoint.y + outerStep.y};
	OSequence->push_back(firstPoint);
	OSequence->push_back(innerPoint);
	OSequence->push_back(firstPoint);
	OSequence->push_back(outerPoint);
	OSequence->push_back(firstPoint);
	OSequence->push_back(outerPoint);
	if (!ci::ritclp(clipFillData, outerPoint)) {
	  flag = false;
	  break;
	}
	OSequence->push_back(outerPoint);
	OSequence->push_back(firstPoint);
	innerPoint.x += step.x;
	innerPoint.y += step.y;
  }
  if (flag) {
	OSequence->push_back(*itFinishVertex);
	OSequence->push_back(innerPoint);
  }
}

void clip::clpic(FRM_HEAD const& form, F_RECTANGLE const& clipRect) {
  auto clipFillData = std::vector<F_POINT> {};
  clipFillData.resize(ClipBuffer->size());
  auto const rotationCenter =
      F_POINT {wrap::midl(clipRect.right, clipRect.left), wrap::midl(clipRect.top, clipRect.bottom)};
  OSequence->clear();
  StateMap->reset(StateFlag::CLPBAK);
  ClipReference.y = rotationCenter.y;
  ClipReference.x = clipRect.left;

  constexpr auto SATWIDTH = 20.0F;
  satin::satout(form, SATWIDTH);
  if (form.type == FRMLINE) {
	for (auto iVertex = 0U; iVertex < form.vertexCount - 2U; ++iVertex) {
	  ci::picfn(form, clipRect, clipFillData, iVertex, iVertex + 1, form.edgeSpacing, rotationCenter);
	  ci::clpcrnr(form, clipRect, clipFillData, iVertex, rotationCenter);
	}
	ci::picfn(form, clipRect, clipFillData, form.vertexCount - 2, form.vertexCount - 1, form.edgeSpacing, rotationCenter);
	return;
  }
  auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  if (form.fillType == 0U) {
	OSequence->push_back(*itVertex);
  }
  auto currentVertex = 0U;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::nxt(form, currentVertex);
	ci::picfn(form, clipRect, clipFillData, currentVertex, nextVertex, form.edgeSpacing, rotationCenter);
	ci::clpcrnr(form, clipRect, clipFillData, currentVertex, rotationCenter);
	currentVertex = nextVertex;
  }
  itVertex = wrap::next(itVertex, currentVertex);
  OSequence->push_back(*itVertex);
}

void ci::duchfn(std::vector<F_POINT> const& chainEndPoints, uint32_t start, uint32_t finish) {
  constexpr auto CHAINLEN = 10U; // Chain length
  constexpr auto CHAIN_SEQUENCE = std::array<uint32_t, CHAINLEN> {0, 1, 2, 3, 0, 1, 4, 3, 0, 3}; // chain stitch sequence
  auto chainPoint = std::vector<F_POINT> {};

  constexpr auto CHPOINTS = 5U; // chainPoint size
  chainPoint.resize(CHPOINTS);
  auto delta = F_POINT {(chainEndPoints[finish].x - chainEndPoints[start].x),
                        (chainEndPoints[finish].y - chainEndPoints[start].y)};

  auto const& form       = FormList->operator[](ClosestFormToCursor);
  auto const lengthDelta = F_POINT {(delta.x * form.edgeStitchLen), (delta.y * form.edgeStitchLen)};
  auto const angle       = atan2(delta.y, delta.x) + PI_FHALF;
  auto const offset      = F_POINT {(cos(angle) * form.borderSize), (sin(angle) * form.borderSize)};
  auto const middleXcoord = chainEndPoints[start].x + lengthDelta.x;
  auto const middleYcoord = chainEndPoints[start].y + lengthDelta.y;
  chainPoint[0]           = chainEndPoints[start];
  chainPoint[4]           = chainEndPoints[finish];
  chainPoint[1].x         = middleXcoord + offset.x;
  chainPoint[1].y         = middleYcoord + offset.y;
  chainPoint[3].x         = middleXcoord - offset.x;
  chainPoint[3].y         = middleYcoord - offset.y;
  if (finish < chainEndPoints.size() - 1U) {
	delta.x = chainEndPoints[wrap::toSize(finish) + 1U].x - chainEndPoints[finish].x;
	delta.y = chainEndPoints[wrap::toSize(finish) + 1U].y - chainEndPoints[finish].y;
  }
  else {
	delta.x = chainEndPoints[finish].x - chainEndPoints[finish - 1U].x;
	delta.y = chainEndPoints[finish].y - chainEndPoints[finish - 1U].y;
  }
  constexpr auto CHFACTOR = 4.0F; // Chain factor
  chainPoint[2].x         = chainEndPoints[finish].x + delta.x / CHFACTOR;
  chainPoint[2].y         = chainEndPoints[finish].y + delta.y / CHFACTOR;
  auto chainCount         = CHAINLEN;
  if (StateMap->test(StateFlag::LINCHN)) {
	--chainCount;
  }
  for (auto iChain = 0U; iChain < chainCount; ++iChain) {
	OSequence->push_back(chainPoint[CHAIN_SEQUENCE[iChain]]);
  }
}

void ci::duch(std::vector<F_POINT> const& chainEndPoints) {
  OSequence->clear();
  auto chainLength = wrap::toUnsigned(chainEndPoints.size());
  if (chainLength <= 2U) {
	displayText::tabmsg(IDS_CHANSMAL, false);
	return;
  }
  --chainLength;
  for (auto iPoint = 0U; iPoint < chainLength - 1U; ++iPoint) {
	ci::duchfn(chainEndPoints, iPoint, iPoint + 1U);
  }
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (form.type != FRMLINE) {
	ci::duchfn(chainEndPoints, chainLength - 1, 0);
	OSequence->push_back(chainEndPoints[chainLength]);
	return;
  }
  ci::duchfn(chainEndPoints, chainLength - 1, chainLength);
  auto backupAt = 8U;
  if (StateMap->test(StateFlag::LINCHN)) {
	--backupAt;
  }
  if ((OSequence->size() >= backupAt)) {
	OSequence->operator[](OSequence->size() - backupAt) = chainEndPoints[chainLength];
  }
  OSequence->push_back(chainEndPoints[chainLength]);
}

void clip::chnfn(FRM_HEAD const& form) {
  auto chainEndPoints = std::vector<F_POINT> {};
  // reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
  chainEndPoints.reserve(50U);
  clip::deleclp(ClosestFormToCursor);
  ci::dufxlen(form, chainEndPoints);
  ci::dulast(chainEndPoints);
  ci::duch(chainEndPoints);
}
