#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "ExtendedBitSet.h"
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "form.h"
#include "hlp.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"
#include "clip.h"

namespace ci = clip::internal;

fPOINT   ClipReference;       // clipboard reference formOrigin
fPOINT   BorderClipReference; // reference for clipboard line border
float    AdjustedSpace;       // adjusted space
uint32_t NextStart;           // index of the endpoint of the line segment being processed

auto clip::iseclp(uint32_t iForm) noexcept -> bool {
  auto const& form = FormList->operator[](iForm);
  return form.edgeType == EDGECLIP || form.edgeType == EDGEPICOT || form.edgeType == EDGECLIPX;
}

auto clip::iseclp(FRMHED const& form) noexcept -> bool {
  return form.edgeType == EDGECLIP || form.edgeType == EDGEPICOT || form.edgeType == EDGECLIPX;
}

auto clip::isclp(FRMHED const& form) noexcept -> bool {
  return ((1U << form.fillType) & ClipTypeMap) != 0;
}

auto clip::isclpx(uint32_t iForm) noexcept -> bool {
  auto const& form = FormList->operator[](iForm);
  return clip::isclp(form) && (form.lengthOrCount.clipCount != 0U);
}

auto clip::isclpx(FRMHED const& form) noexcept -> bool {
  return clip::isclp(form) && (form.lengthOrCount.clipCount != 0U);
}

auto clip::iseclpx(uint32_t iForm) noexcept -> bool {
  auto const& form = FormList->operator[](iForm);
  return clip::iseclp(iForm) && (form.clipEntries != 0U);
}

auto clip::iseclpx(FRMHED const& form) noexcept -> bool {
  return clip::iseclp(form) && (form.clipEntries != 0U);
}

auto clip::internal::findclp(uint32_t formIndex) noexcept -> uint32_t {
  for (auto iForm = formIndex; iForm != 0; iForm--) {
	auto& form = FormList->operator[](iForm - 1U);
	if (clip::iseclp(form)) {
	  return form.borderClipData + form.clipEntries;
	}
	if (clip::isclp(form)) {
	  return form.angleOrClipData.clip + form.lengthOrCount.clipCount;
	}
  }
  return 0;
}

void clip::internal::clpsub(uint32_t fpnt, uint32_t cnt) {
  auto& formList = *FormList;
  for (auto iForm = fpnt + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	if (clip::isclpx(iForm)) {
	  formList[iForm].angleOrClipData.clip -= cnt;
	}
	if (clip::iseclpx(iForm)) {
	  formList[iForm].borderClipData -= cnt;
	}
  }
}

void clip::delmclp(uint32_t formIndex) {
  if (!ClipPoints->empty()) {
	auto& form = FormList->operator[](formIndex);
	if (clip::isclp(form)) {
	  auto const destIndex  = ci::findclp(formIndex);
	  auto       eraseStart = std::next(ClipPoints->cbegin(), destIndex);
	  auto       eraseEnd   = std::next(eraseStart, form.lengthOrCount.clipCount);
	  ClipPoints->erase(eraseStart, eraseEnd);
	  if (clip::iseclp(form)) {
		form.borderClipData -= form.lengthOrCount.clipCount;
	  }
	  ci::clpsub(formIndex, form.lengthOrCount.clipCount);
	  form.lengthOrCount.clipCount = 0;
	}
  }
}

void clip::deleclp(uint32_t formIndex) {
  if (!ClipPoints->empty()) {
	auto& form = FormList->operator[](formIndex);
	if (clip::iseclp(form)) {
	  auto destIndex = ci::findclp(formIndex);
	  if (clip::isclpx(form)) {
		destIndex += form.lengthOrCount.clipCount;
	  }
	  auto eraseStart = std::next(ClipPoints->cbegin(), destIndex);
	  auto eraseEnd   = std::next(eraseStart, form.clipEntries);
	  ClipPoints->erase(eraseStart, eraseEnd);
	  ci::clpsub(formIndex, form.clipEntries);
	  form.clipEntries    = 0;
	  form.borderClipData = 0;
	  form.edgeType       = 0;
	}
  }
}

void clip::delclps(uint32_t formIndex) {
  clip::deleclp(formIndex);
  clip::delmclp(formIndex);
}

auto clip::nueclp(uint32_t currentForm, uint32_t count) -> uint32_t {
  auto        find = ci::findclp(currentForm);
  auto const& form = FormList->operator[](currentForm);
  if (clip::isclp(form)) {
	find += form.lengthOrCount.clipCount;
  }
  auto const it  = std::next(ClipPoints->cbegin(), find);
  auto const val = fPOINT {};
  ClipPoints->insert(it, count, val);
  for (auto iform = ClosestFormToCursor; iform < wrap::toUnsigned(FormList->size()); iform++) {
	auto& thisForm = FormList->operator[](iform);
	if (clip::iseclpx(thisForm)) {
	  thisForm.borderClipData += count;
	}
  }
  for (auto iform = ClosestFormToCursor + 1U; iform < wrap::toUnsigned(FormList->size()); iform++) {
	auto& thisForm = FormList->operator[](iform);
	if (clip::isclp(thisForm)) {
	  thisForm.angleOrClipData.clip += count;
	}
  }
  return find;
}

auto clip::numclp() -> uint32_t {
  auto const clipSize = wrap::toUnsigned(ClipBuffer->size());
  auto const find     = ci::findclp(ClosestFormToCursor);
  auto const it       = std::next(ClipPoints->cbegin(), find);
  auto const val      = fPOINT {};
  ClipPoints->insert(it, clipSize, val);
  auto& formList                                     = *FormList;
  formList[ClosestFormToCursor].angleOrClipData.clip = find;
  if (clip::iseclpx(ClosestFormToCursor)) {
	formList[ClosestFormToCursor].borderClipData += clipSize;
  }
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	if (clip::isclpx(iForm)) {
	  formList[iForm].angleOrClipData.clip += clipSize;
	}
	if (clip::iseclpx(iForm)) {
	  formList[iForm].borderClipData += clipSize;
	}
  }
  return find;
}

void clip::oclp(fRECTANGLE& clipRect, uint32_t clipIndex, uint32_t clipEntries) {
  auto clipIt = std::next(ClipPoints->begin(), clipIndex);
  if (!StateMap.test(StateFlag::NOCLP)) {
	auto& clipBuffer = *ClipBuffer;
	clipBuffer.clear();
	if (clipEntries != 0U) {
	  clipBuffer.reserve(clipEntries);
	  for (auto iClip = 0U; iClip < clipEntries; iClip++) {
		clipBuffer.emplace_back(fPOINTATTR {(*clipIt).x, (*clipIt).y, 0});
		clipIt++;
	  }
	  clipRect.left = clipRect.right = clipBuffer[0].x;
	  clipRect.bottom = clipRect.top = clipBuffer[0].y;
	  for (auto& clip : *ClipBuffer) {
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
	  clipRect = fRECTANGLE {};
	}
	ClipRectSize.cx = clipRect.right - clipRect.left;
	ClipRectSize.cy = clipRect.top - clipRect.bottom;
  }
}

void clip::internal::durev(fRECTANGLE const& clipRect, std::vector<fPOINT>& clipReversedData) {
  if (!ClipBuffer->empty()) {
	auto const midpoint   = wrap::midl(clipRect.right, clipRect.left);
	auto&      clipBuffer = *ClipBuffer;
	if (clipBuffer[0].x > midpoint) {
	  auto iBuffer = clipBuffer.begin();
	  for (auto& reversed : clipReversedData) {
		reversed = fPOINT {clipRect.right - iBuffer->x, iBuffer->y};
		iBuffer++;
	  }
	}
	else {
	  auto iBuffer = clipBuffer.begin();
	  for (auto& reversed : clipReversedData) {
		reversed = *iBuffer;
		iBuffer++;
	  }
	}
  }
}

void clip::internal::setvct(uint32_t vertexIndex, uint32_t start, uint32_t finish, float& clipAngle, fPOINT& vector0) {
  auto vertexIt = std::next(FormVertices->cbegin(), vertexIndex);
  clipAngle = atan2(vertexIt[finish].y - vertexIt[start].y, vertexIt[finish].x - vertexIt[start].x);
  vector0.x = ClipRectSize.cx * cos(clipAngle);
  vector0.y = ClipRectSize.cx * sin(clipAngle);
}

auto clip::internal::nupnt(float clipAngle, fPOINT& moveToCoords, fPOINT const& stitchPoint) noexcept -> bool {
  auto const sinAngle = sin(clipAngle);
  auto const cosAngle = cos(clipAngle);
  auto       length   = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
  if (length > ClipRectSize.cx) {
	for (auto step = 0U; step < 10U; step++) {
	  length           = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
	  auto const delta = ClipRectSize.cx - length;
	  moveToCoords.x += delta * cosAngle;
	  moveToCoords.y += delta * sinAngle;
	  if (fabs(delta) < 0.01F) {
		break;
	  }
	}
	return true;
  }
  return false;
}

auto clip::internal::ritclp(std::vector<fPOINT> const& clipFillData, fPOINT const& point) -> bool {
  auto       retval        = false;
  auto const adjustedPoint = fPOINT {(point.x - ClipReference.x), (point.y - ClipReference.y)};
  if (form::chkmax(wrap::toUnsigned(clipFillData.size()), wrap::toUnsigned(OSequence->size()))) {
	retval = true;
  }
  else {
	std::transform(clipFillData.begin(),
	               clipFillData.end(),
	               std::back_inserter(*OSequence),
	               [&adjustedPoint](auto const& data) noexcept {
	                 return fPOINT {data.x + adjustedPoint.x, data.y + adjustedPoint.y};
	               });
  }
  return retval;
}

void clip::internal::lincrnr(uint32_t                   vertexIndex,
                             std::vector<fPOINT> const& clipReversedData,
                             std::vector<fPOINT>&       clipFillData,
                             float                      clipAngle,
                             fPOINT const&              rotationCenter,
                             uint32_t                   currentSide,
                             fPOINT&                    stitchPoint) {
  auto vertexIt     = std::next(FormVertices->cbegin(), vertexIndex);
  auto moveToCoords = vertexIt[wrap::toSize(currentSide) + 2U];
  if (ci::nupnt(clipAngle, moveToCoords, stitchPoint)) {
	auto const delta = fPOINT {moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y};
	auto const rotationAngle = atan2(delta.y, delta.x);
	thred::rotangf(BorderClipReference, ClipReference, rotationAngle, rotationCenter);
	auto reversedData = clipReversedData.begin();
	for (auto& data : clipFillData) {
	  thred::rotangf(*reversedData, data, rotationAngle, rotationCenter);
	  reversedData++;
	}
	ci::ritclp(clipFillData, stitchPoint);
	stitchPoint = moveToCoords;
  }
}

void clip::internal::linsid(uint32_t                   vertexIndex,
                            std::vector<fPOINT> const& clipReversedData,
                            std::vector<fPOINT>&       clipFillData,
                            float                      clipAngle,
                            fPOINT const&              vector0,
                            fPOINT const&              rotationCenter,
                            uint32_t                   currentSide,
                            fPOINT&                    stitchPoint) {
  auto        vertexIt  = std::next(FormVertices->cbegin(), vertexIndex);
  auto const& point     = vertexIt[wrap::toSize(currentSide) + 1U];
  auto const  delta     = fPOINT {(point.x - stitchPoint.x), (point.y - stitchPoint.y)};
  auto const  length    = hypot(delta.x, delta.y);
  auto const  clipCount = wrap::floor<uint32_t>(length / ClipRectSize.cx);
  if (clipCount != 0U) {
	thred::rotangf(BorderClipReference, ClipReference, clipAngle, rotationCenter);
	auto reversedData = clipReversedData.begin();
	for (auto& data : clipFillData) {
	  thred::rotangf(*reversedData, data, clipAngle, rotationCenter);
	  reversedData++;
	}
	for (auto iClip = 0U; iClip < clipCount; iClip++) {
	  ci::ritclp(clipFillData, stitchPoint);
	  stitchPoint.x += vector0.x;
	  stitchPoint.y += vector0.y;
	}
  }
}

void clip::clpout(float width) {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (form.type == FRMLINE) {
	satin::satout(form, width);
  }
  else {
	satin::satout(form, ClipRectSize.cy);
	InsidePointList->clear();
	auto srcStart = std::next(FormVertices->cbegin(), form.vertexIndex);
	auto srcEnd   = std::next(srcStart, form.vertexCount);
	InsidePointList->insert(InsidePointList->end(), srcStart, srcEnd);
	InsidePoints = InsidePointList;
  }
}

auto clip::internal::clpsid(uint32_t                   vertexIndex,
                            fRECTANGLE const&          clipRect,
                            std::vector<fPOINT> const& clipReversedData,
                            std::vector<fPOINT>&       clipFillData,
                            uint32_t                   start,
                            uint32_t                   finish,
                            fPOINT const&              rotationCenter) -> bool {
  auto        vertexIt           = std::next(FormVertices->cbegin(), vertexIndex);
  auto const& end                = vertexIt[finish];
  auto const& begin              = vertexIt[start];
  auto const  delta              = fPOINT {(end.x - begin.x), (end.y - begin.y)};
  auto const  length             = hypot(delta.x, delta.y);
  auto const  clipReferencePoint = fPOINTATTR {clipRect.left, clipRect.bottom, 0U};
  auto const  rotationAngle      = atan2(delta.y, delta.x);
  thred::rotang1(clipReferencePoint, ClipReference, rotationAngle, rotationCenter);
  if (ClipRectSize.cx == 0.0F) {
	return false;
  }
  auto const clipCount = wrap::floor<uint32_t>(length / ClipRectSize.cx);
  if (clipCount != 0U) {
	auto remainder = 0.0F;
	if (clipCount > 1U) {
	  remainder = ((length - gsl::narrow_cast<float>(clipCount) * ClipRectSize.cx) /
	                   (gsl::narrow_cast<float>(clipCount) - 1.0F) +
	               ClipRectSize.cx) /
	              length;
	}
	else {
	  remainder = (length - ClipRectSize.cx) / 2;
	}
	auto const step         = fPOINT {delta.x * remainder, delta.y * remainder};
	auto       insertPoint  = begin;
	auto       reversedData = clipReversedData.begin();
	for (auto& data : clipFillData) {
	  thred::rotangf(*reversedData, data, rotationAngle, rotationCenter);
	  reversedData++;
	}
	for (auto stepCount = 0U; stepCount < clipCount; stepCount++) {
	  if (ci::ritclp(clipFillData, insertPoint)) {
		break;
	  }
	  insertPoint.x += step.x;
	  insertPoint.y += step.y;
	}
	return true;
  }
  return false;
}

void clip::clpbrd(FRMHED const& form, fRECTANGLE const& clipRect, uint32_t startVertex) {
  OSequence->clear();
  StateMap.reset(StateFlag::CLPBAK);
  HorizontalLength           = ClipRectSize.cx;
  auto const clipStitchCount = ClipBuffer->size();
  auto       clipFillData    = std::vector<fPOINT> {};
  clipFillData.resize(clipStitchCount);
  auto clipReversedData = std::vector<fPOINT> {};
  clipReversedData.resize(clipStitchCount);
  auto const rotationCenter =
      fPOINT {wrap::midl(clipRect.right, clipRect.left), wrap::midl(clipRect.top, clipRect.bottom)};
  ClipReference.x = clipRect.left;
  ClipReference.y = rotationCenter.y;
  ci::durev(clipRect, clipReversedData);
  if (form.type == FRMLINE) {
	auto vertexIt    = std::next(FormVertices->cbegin(), form.vertexIndex);
	auto stitchPoint = vertexIt[0];
	auto clipAngle   = 0.0F;      // for clipboard border fill
	auto vector0     = fPOINT {}; // x size of the clipboard fill at the fill angle
	ci::setvct(form.vertexIndex, 0, 1, clipAngle, vector0);
	// Since clipRect.bottom is always 0
	BorderClipReference.y = clipRect.top / 2;
	// Use 0 to align left edge of clip with beginning of line, clipRect.right / 2 if you want to
	// align the center of the clip with the beginning of the line
	BorderClipReference.x = 0;
	// BorderClipReference.x = clipRect.right / 2;
	auto       currentSide = 0U;
	auto const sideCount   = form.vertexCount - 2U;
	for (currentSide = 0U; currentSide < sideCount; currentSide++) {
	  ci::linsid(form.vertexIndex, clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide, stitchPoint);
	  ci::setvct(form.vertexIndex, currentSide + 1, currentSide + 2, clipAngle, vector0);
	  ci::lincrnr(form.vertexIndex, clipReversedData, clipFillData, clipAngle, rotationCenter, currentSide, stitchPoint);
	}
	ci::linsid(form.vertexIndex, clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide, stitchPoint);
  }
  else {
	clpout(ClipRectSize.cx / 2);
	auto reference     = startVertex;
	auto currentVertex = startVertex;
	for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	  auto const nextVertex = form::prv(form, currentVertex);
	  if (ci::clpsid(form.vertexIndex, clipRect, clipReversedData, clipFillData, reference, nextVertex, rotationCenter)) {
		reference = nextVertex;
	  }
	  currentVertex = nextVertex;
	}
  }
}

auto clip::internal::fxpnt(uint32_t                  vertexIndex,
                           std::vector<float> const& listSINEs,
                           std::vector<float> const& listCOSINEs,
                           fPOINT&                   moveToCoords,
                           uint32_t                  currentSide,
                           fPOINT const&             stitchPoint) -> bool {
  auto vertexIt = std::next(FormVertices->cbegin(), vertexIndex);
  moveToCoords  = vertexIt[NextStart];
  auto length   = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
  if (length > AdjustedSpace) {
	for (auto iGuess = 0U; iGuess < 10U; iGuess++) {
	  length           = hypot(moveToCoords.x - stitchPoint.x, moveToCoords.y - stitchPoint.y);
	  auto const delta = AdjustedSpace - length;
	  moveToCoords.x += delta * listCOSINEs[currentSide];
	  moveToCoords.y += delta * listSINEs[currentSide];
	  if (fabs(delta) < 0.2F) {
		break;
	  }
	}
	return true;
  }
  return false;
}

void clip::internal::fxlit(uint32_t                  vertexIndex,
                           std::vector<float> const& listSINEs,
                           std::vector<float> const& listCOSINEs,
                           fPOINT&                   moveToCoords,
                           uint32_t                  currentSide,
                           fPOINT&                   stitchPoint) {
  if (ci::fxpnt(vertexIndex, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint)) {
	stitchPoint = moveToCoords;
	BeanCount++;
	auto       vertexIt = std::next(FormVertices->cbegin(), vertexIndex);
	auto const length =
	    hypot(vertexIt[NextStart].x - stitchPoint.x, vertexIt[NextStart].y - stitchPoint.y);
	auto const count = wrap::floor<uint32_t>(length / AdjustedSpace);
	auto const delta =
	    fPOINT {AdjustedSpace * listCOSINEs[currentSide], AdjustedSpace * listSINEs[currentSide]};
	stitchPoint.x += delta.x * count;
	stitchPoint.y += delta.y * count;
	BeanCount += count;
  }
}

void clip::internal::fxlin(uint32_t                  vertexIndex,
                           std::vector<fPOINT>&      chainEndPoints,
                           std::vector<float> const& ListSINEs,
                           std::vector<float> const& ListCOSINEs,
                           fPOINT&                   moveToCoords,
                           uint32_t                  currentSide,
                           fPOINT&                   stitchPoint) {
  if (ci::fxpnt(vertexIndex, ListSINEs, ListCOSINEs, moveToCoords, currentSide, stitchPoint)) {
	stitchPoint = moveToCoords;
	chainEndPoints.push_back(stitchPoint);
	auto       vertexIt = std::next(FormVertices->cbegin(), vertexIndex);
	auto const length =
	    hypot(vertexIt[NextStart].x - stitchPoint.x, vertexIt[NextStart].y - stitchPoint.y);
	auto const count = wrap::floor<uint32_t>(length / AdjustedSpace);
	auto const delta =
	    fPOINT {AdjustedSpace * ListCOSINEs[currentSide], AdjustedSpace * ListSINEs[currentSide]};
	for (auto iChain = 0U; iChain < count; iChain++) {
	  stitchPoint.x += delta.x;
	  stitchPoint.y += delta.y;
	  chainEndPoints.push_back(stitchPoint);
	}
  }
}

void clip::internal::fxlen(FRMHED const&             form,
                           std::vector<fPOINT>&      chainEndPoints,
                           std::vector<float> const& listSINEs,
                           std::vector<float> const& listCOSINEs) {
  auto moveToCoords = fPOINT {}; // moving formOrigin for clipboard fill
  AdjustedSpace     = 0.0F;
  auto flag         = true;
  auto vertexIt     = std::next(FormVertices->cbegin(), form.vertexIndex);
  for (auto iVertex = 1U; iVertex < form.vertexCount; iVertex++) {
	auto const length = hypot(vertexIt[iVertex].x - vertexIt[0].x, vertexIt[iVertex].y - vertexIt[0].y);
	if (length > form.edgeSpacing) {
	  flag = false;
	  break;
	}
	if (length > AdjustedSpace) {
	  AdjustedSpace = length;
	}
  }
  if (flag) {
	chainEndPoints.push_back(vertexIt[0]);
	chainEndPoints.push_back(vertexIt[1]);
	return;
  }
  AdjustedSpace              = form.edgeSpacing;
  auto       minimumSpacing  = form.edgeSpacing;
  auto const halfSpacing     = AdjustedSpace / 2.0F;
  auto       interval        = 1e9F;
  auto       minimumInterval = 1e9F;
  auto       loopCount       = 0U;
  auto       initialCount    = 0U;
  auto       smallestSpacing = 0.0F;
  auto       largestSpacing  = 1.0F;
  // loop at least 50 times to guarantee convergence
  while (loopCount < 50U && (largestSpacing - smallestSpacing) > TINY) {
	BeanCount        = 0;
	auto stitchPoint = vertexIt[0];
	auto currentSide = 0U;
	for (currentSide = 0U; currentSide < form.vertexCount - 1U; currentSide++) {
	  NextStart = currentSide + 1U;
	  ci::fxlit(form.vertexIndex, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint);
	}
	if (form.type != FRMLINE) {
	  NextStart = 0;
	  ci::fxlit(form.vertexIndex, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint);
	}
	else {
	  NextStart = form.vertexCount - 1U;
	}
	if (initialCount == 0U) {
	  initialCount    = BeanCount;
	  smallestSpacing = AdjustedSpace;
	  minimumInterval =
	      hypot(vertexIt[NextStart].x - stitchPoint.x, vertexIt[NextStart].y - stitchPoint.y);
	  interval       = minimumInterval;
	  minimumSpacing = AdjustedSpace;
	  interval /= initialCount;
	  AdjustedSpace += interval / 2.0F;
	  largestSpacing = smallestSpacing + interval;
	}
	else {
	  interval = hypot(vertexIt[NextStart].x - stitchPoint.x, vertexIt[NextStart].y - stitchPoint.y);
	  if (interval > halfSpacing) {
		interval = form.edgeSpacing - interval;
	  }
	  if (interval < minimumInterval) {
		minimumInterval = interval;
		minimumSpacing  = AdjustedSpace;
	  }
	  if (initialCount == BeanCount) {
		smallestSpacing = AdjustedSpace;
	  }
	  else {
		if (BeanCount > initialCount) {
		  largestSpacing = AdjustedSpace;
		}
		else {
		  smallestSpacing = AdjustedSpace;
		}
	  }
	  AdjustedSpace = smallestSpacing + (largestSpacing - smallestSpacing) / 2;
	}
	loopCount++;
  }
  auto stitchPoint = vertexIt[0];
  OutputIndex      = 1;
  AdjustedSpace    = minimumSpacing;
  chainEndPoints.push_back(stitchPoint);
  auto currentSide = 0U;
  for (currentSide = 0; currentSide < form.vertexCount - 1U; currentSide++) {
	NextStart = currentSide + 1U;
	ci::fxlin(form.vertexIndex, chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint);
  }
  if (form.type != FRMLINE) {
	NextStart = 0;
	ci::fxlin(form.vertexIndex, chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide, stitchPoint);
  }
  interval = hypot(vertexIt[NextStart].x - stitchPoint.x, vertexIt[NextStart].y - stitchPoint.y);
  if (interval > halfSpacing) {
	chainEndPoints.push_back(vertexIt[NextStart]);
  }
  else {
	chainEndPoints.back() = vertexIt[NextStart];
  }
}

void clip::internal::dufxlen(FRMHED const& form, std::vector<fPOINT>& chainEndPoints) {
  form::duangs(form);
  auto listSINEs = std::vector<float> {};
  listSINEs.reserve(wrap::toSize(form.vertexCount) + 1U);
  auto listCOSINEs = std::vector<float> {};
  listCOSINEs.reserve(form.vertexCount);
  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	listSINEs.push_back(sin(FormAngles->operator[](iVertex)));
	listCOSINEs.push_back(cos(FormAngles->operator[](iVertex)));
  }
  listSINEs.push_back(sin((FormAngles->front() > FormAngles->operator[](form.vertexCount - 1U))
                              ? (FormAngles->front() - FormAngles->operator[](form.vertexCount - 1U))
                              : (FormAngles->operator[](form.vertexCount - 1U) - FormAngles->front())));
  ci::fxlen(form, chainEndPoints, listSINEs, listCOSINEs);
}

void clip::internal::dulast(std::vector<fPOINT>& chainEndPoints) {
  auto tempClipPoints = std::vector<fPOINT> {};
  tempClipPoints.reserve(chainEndPoints.size());
  if (form::lastch()) {
	auto minimumLength = 1e99;
	auto minimumIndex  = 0U;
	for (auto iPoint = 0U; iPoint < wrap::toUnsigned(chainEndPoints.size()) - 1U; iPoint++) {
	  auto const length =
	      hypot(LastPoint.x - chainEndPoints[iPoint].x, LastPoint.y - chainEndPoints[iPoint].y);
	  if (length < minimumLength) {
		minimumLength = length;
		minimumIndex  = iPoint;
	  }
	}
	if (minimumIndex != 0U) {
	  for (auto iPoint = minimumIndex; iPoint < wrap::toUnsigned(chainEndPoints.size()) - 2U; iPoint++) {
		tempClipPoints.push_back(chainEndPoints[iPoint]);
	  }
	  for (auto iPoint = 0U; iPoint <= minimumIndex; iPoint++) {
		tempClipPoints.push_back(chainEndPoints[iPoint]);
	  }
	  chainEndPoints = tempClipPoints;
	}
  }
}

void clip::internal::clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints) {
  ci::dulast(chainEndPoints);
  auto&       clipBuffer = *ClipBuffer;
  auto const& form       = FormList->operator[](ClosestFormToCursor);
  if (form.type == FRMLINE) {
	auto const pivot = ClipRectSize.cy / 2;
	std::transform(clipBuffer.begin(), clipBuffer.end(), std::back_inserter(tempClipPoints), [&pivot](auto& clip) noexcept {
	  return fPOINT {clip.x, (-clip.y + pivot)};
	});
  }
  else {
	std::transform(clipBuffer.begin(), clipBuffer.end(), std::back_inserter(tempClipPoints), [](auto& clip) noexcept {
	  return fPOINT {clip.x, (-clip.y)};
	});
  }
}

void clip::internal::xclpfn(std::vector<fPOINT> const& tempClipPoints,
                            std::vector<fPOINT> const& chainEndPoints,
                            uint32_t                   start,
                            uint32_t                   finish,
                            fPOINT const&              rotationCenter) {
  auto const delta         = fPOINT {(chainEndPoints[finish].x - chainEndPoints[start].x),
                             (chainEndPoints[finish].y - chainEndPoints[start].y)};
  auto const rotationAngle = atan2(delta.y, delta.x);
  auto       chainEndPoint = chainEndPoints[start];
  for (auto const& clip : tempClipPoints) {
	auto point = clip;
	thred::rotflt(point, rotationAngle, rotationCenter);
	OSequence->push_back(fPOINT {chainEndPoint.x + point.x, chainEndPoint.y + point.y});
  }
}

void clip::duxclp(FRMHED const& form) {
  auto chainEndPoints = std::vector<fPOINT> {};
  // reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
  chainEndPoints.reserve(50U);
  ci::dufxlen(form, chainEndPoints);
  auto tempClipPoints = std::vector<fPOINT> {};
  tempClipPoints.reserve(ClipBuffer->size());
  ci::clpxadj(tempClipPoints, chainEndPoints);
  OSequence->clear();
  auto const rotationCenter = fPOINT {};
  for (auto iPoint = 1U; iPoint < wrap::toUnsigned(chainEndPoints.size()); iPoint++) {
	ci::xclpfn(tempClipPoints, chainEndPoints, iPoint - 1, iPoint, rotationCenter);
  }
  if (form.type != FRMLINE) {
	OSequence->push_back(chainEndPoints[0]);
  }
}

void clip::internal::clpcrnr(FRMHED const&        form,
                             fRECTANGLE const&    clipRect,
                             std::vector<fPOINT>& clipFillData,
                             uint32_t             vertex,
                             fPOINT const&        rotationCenter) {
  auto const  nextVertex = form::nxt(form, vertex);
  auto        vertexIt   = std::next(FormVertices->cbegin(), form.vertexIndex);
  auto const* points     = StateMap.test(StateFlag::INDIR) ? OutsidePoints : InsidePoints;
  if (points != nullptr) {
	auto delta = fPOINT {points->operator[](nextVertex).x - vertexIt[nextVertex].x,
	                     points->operator[](nextVertex).y - vertexIt[nextVertex].y};

	auto const rotationAngle = atan2(delta.y, delta.x) + PI / 2;
	auto const referencePoint = fPOINTATTR {wrap::midl(clipRect.right, clipRect.left), clipRect.top, 0U};
	thred::rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
	auto iClip = clipFillData.begin();
	for (auto& clip : *ClipBuffer) {
	  thred::rotang1(clip, *iClip, rotationAngle, rotationCenter);
	  iClip++;
	}
	auto const length = hypot(delta.x, delta.y);
	auto const ratio  = form::getplen() / length;
	delta.x *= ratio;
	delta.y *= ratio;
	auto const point = fPOINT {vertexIt[nextVertex].x + delta.x, vertexIt[nextVertex].y + delta.y};
	OSequence->push_back(vertexIt[nextVertex]);
	OSequence->push_back(point);
	OSequence->push_back(vertexIt[nextVertex]);
	OSequence->push_back(point);
	if (!ci::ritclp(clipFillData, point)) {
	  OSequence->push_back(point);
	  OSequence->push_back(vertexIt[nextVertex]);
	}
  }
}

void clip::internal::picfn(FRMHED const&        form,
                           fRECTANGLE const&    clipRect,
                           std::vector<fPOINT>& clipFillData,
                           uint32_t             start,
                           uint32_t             finish,
                           float                spacing,
                           fPOINT const&        rotationCenter) {
  auto       vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
  auto const delta =
      fPOINT {(vertexIt[finish].x - vertexIt[start].x), (vertexIt[finish].y - vertexIt[start].y)};
  auto       rotationAngle = atan2(-delta.x, delta.y);
  auto const length        = hypot(delta.x, delta.y);
  auto const referencePoint = fPOINTATTR {wrap::midl(clipRect.right, clipRect.left), clipRect.top, 0U};
  auto const outerStep = fPOINT {form.borderSize * cos(rotationAngle), form.borderSize * sin(rotationAngle)};
  spacing += ClipRectSize.cx;
  auto const count = wrap::round<uint32_t>(length / spacing);
  rotationAngle    = atan2(delta.y, delta.x);
  thred::rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
  if (count != 0U) {
	auto step = fPOINT {};
	if (count > 1) {
	  auto const tdub =
	      ((length - gsl::narrow_cast<float>(count) * spacing) / (gsl::narrow_cast<float>(count) - 1.0F) + spacing) / length;
	  auto const val = fPOINT {delta.x * tdub, delta.y * tdub};
	  step           = val;
	}
	auto iClip = clipFillData.begin();
	for (auto& clip : *ClipBuffer) {
	  thred::rotang1(clip, *iClip, rotationAngle, rotationCenter);
	  iClip++;
	}
	auto flag       = true;
	auto innerPoint = fPOINT {vertexIt[start].x, vertexIt[start].y};
	for (auto iStep = 0U; iStep < count - 1U; iStep++) {
	  auto const firstPoint = fPOINT {innerPoint.x + step.x, innerPoint.y + step.y};
	  auto const outerPoint = fPOINT {firstPoint.x + outerStep.x, firstPoint.y + outerStep.y};
	  OSequence->push_back(firstPoint);
	  OSequence->push_back(innerPoint);
	  OSequence->push_back(firstPoint);
	  OSequence->push_back(outerPoint);
	  OSequence->push_back(firstPoint);
	  OSequence->push_back(outerPoint);
	  if (ci::ritclp(clipFillData, outerPoint)) {
		flag = false;
		break;
	  }
	  OSequence->push_back(outerPoint);
	  OSequence->push_back(firstPoint);
	  innerPoint.x += step.x;
	  innerPoint.y += step.y;
	}
	if (flag) {
	  OSequence->push_back(vertexIt[finish]);
	  OSequence->push_back(innerPoint);
	}
  }
}

void clip::clpic(FRMHED const& form, fRECTANGLE const& clipRect) {
  auto clipFillData = std::vector<fPOINT> {};
  clipFillData.resize(ClipBuffer->size());
  auto const rotationCenter =
      fPOINT {wrap::midl(clipRect.right, clipRect.left), wrap::midl(clipRect.top, clipRect.bottom)};
  OSequence->clear();
  StateMap.reset(StateFlag::CLPBAK);
  HorizontalLength = ClipRectSize.cx;
  ClipReference.y  = rotationCenter.y;
  ClipReference.x  = clipRect.left;
  satin::satout(form, 20);
  if (form.type == FRMLINE) {
	for (auto iVertex = 0U; iVertex < form.vertexCount - 2U; iVertex++) {
	  ci::picfn(form, clipRect, clipFillData, iVertex, iVertex + 1, form.edgeSpacing, rotationCenter);
	  ci::clpcrnr(form, clipRect, clipFillData, iVertex, rotationCenter);
	}
	ci::picfn(form, clipRect, clipFillData, form.vertexCount - 2, form.vertexCount - 1, form.edgeSpacing, rotationCenter);
  }
  else {
	auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	if (form.fillType == 0U) {
	  OSequence->push_back(vertexIt[0]);
	}
	auto currentVertex = 0U;
	for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	  auto const nextVertex = form::nxt(form, currentVertex);
	  ci::picfn(form, clipRect, clipFillData, currentVertex, nextVertex, form.edgeSpacing, rotationCenter);
	  ci::clpcrnr(form, clipRect, clipFillData, currentVertex, rotationCenter);
	  currentVertex = nextVertex;
	}
	OSequence->push_back(vertexIt[currentVertex]);
  }
}

void clip::internal::duchfn(std::vector<fPOINT> const& chainEndPoints, uint32_t start, uint32_t finish) {
  auto const chainSequence = std::vector<uint32_t> {0, 1, 2, 3, 0, 1, 4, 3, 0, 3}; // chain stitch sequence
  auto       chainPoint = std::vector<fPOINT> {};
  chainPoint.resize(5U);
  auto delta = fPOINT {(chainEndPoints[finish].x - chainEndPoints[start].x),
                       (chainEndPoints[finish].y - chainEndPoints[start].y)};

  auto const& form        = FormList->operator[](ClosestFormToCursor);
  auto const lengthDelta  = fPOINT {(delta.x * form.edgeStitchLen), (delta.y * form.edgeStitchLen)};
  auto const angle        = atan2(delta.y, delta.x) + PI_F / 2.0F;
  auto const offset       = fPOINT {(cos(angle) * form.borderSize), (sin(angle) * form.borderSize)};
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
  chainPoint[2].x = chainEndPoints[finish].x + delta.x / 4.0F;
  chainPoint[2].y = chainEndPoints[finish].y + delta.y / 4.0F;
  auto chainCount = wrap::toUnsigned(chainSequence.size());
  if (StateMap.test(StateFlag::LINCHN)) {
	chainCount--;
  }
  for (auto iChain = 0U; iChain < chainCount; iChain++) {
	OSequence->push_back(chainPoint[chainSequence[iChain]]);
  }
}

void clip::internal::duch(std::vector<fPOINT>& chainEndPoints) {
  OSequence->clear();
  auto chainLength = wrap::toUnsigned(chainEndPoints.size());
  if (chainLength > 2U) {
	chainLength--;
	for (auto iPoint = 0U; iPoint < chainLength - 1U; iPoint++) {
	  ci::duchfn(chainEndPoints, iPoint, iPoint + 1U);
	}
	auto const& form = FormList->operator[](ClosestFormToCursor);
	if (form.type == FRMLINE) {
	  ci::duchfn(chainEndPoints, chainLength - 1, chainLength);
	  auto backupAt = 8U;
	  if (StateMap.test(StateFlag::LINCHN)) {
		backupAt--;
	  }
	  if ((OSequence->size() >= backupAt)) {
		OSequence->operator[](OSequence->size() - backupAt) = chainEndPoints[chainLength];
	  }
	  OSequence->push_back(chainEndPoints[chainLength]);
	}
	else {
	  ci::duchfn(chainEndPoints, chainLength - 1, 0);
	  OSequence->push_back(chainEndPoints[chainLength]);
	}
  }
  else {
	displayText::tabmsg(IDS_CHANSMAL);
  }
}

void clip::chnfn(FRMHED const& form) {
  auto chainEndPoints = std::vector<fPOINT> {};
  // reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
  chainEndPoints.reserve(50U);
  clip::deleclp(ClosestFormToCursor);
  ci::dufxlen(form, chainEndPoints);
  ci::dulast(chainEndPoints);
  ci::duch(chainEndPoints);
}
