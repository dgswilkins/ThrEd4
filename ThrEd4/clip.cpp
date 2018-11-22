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

fPOINT       ClipReference;       // clipboard reference formOrigin
fPOINT       BorderClipReference; // reference for clipboard line border
double       AdjustedSpace;       // adjusted space
unsigned int NextStart;           // index of the endpoint of the line segment being processed

bool clip::iseclp(unsigned int iForm) {
	auto& form = (*FormList)[iForm];
	return form.edgeType == EDGECLIP || form.edgeType == EDGEPICOT || form.edgeType == EDGECLIPX;
}

bool clip::iseclp(const FRMHED& form) noexcept {
	return form.edgeType == EDGECLIP || form.edgeType == EDGEPICOT || form.edgeType == EDGECLIPX;
}

bool clip::isclp(unsigned int iForm) {
	auto& form = (*FormList)[iForm];
	return ((1 << form.fillType) & ClipTypeMap) != 0;
}

bool clip::isclp(const FRMHED& form) noexcept {
	return ((1 << form.fillType) & ClipTypeMap) != 0;
}

bool clip::isclpx(unsigned int iForm) {
	auto& form = (*FormList)[iForm];
	return clip::isclp(iForm) && form.lengthOrCount.clipCount;
}

bool clip::isclpx(const FRMHED& form) noexcept {
	return clip::isclp(form) && form.lengthOrCount.clipCount;
}

bool clip::iseclpx(unsigned int iForm) {
	auto& form = (*FormList)[iForm];
	return clip::iseclp(iForm) && form.clipEntries;
}

bool clip::iseclpx(const FRMHED& form) noexcept {
	return clip::iseclp(form) && form.clipEntries;
}

unsigned int clip::internal::findclp(unsigned int formIndex) {
	auto& formList = *FormList;
	for (auto iForm = formIndex; iForm != 0; iForm--) {
		if (clip::iseclp(iForm - 1)) {
			return formList[iForm - 1].borderClipData + formList[iForm - 1].clipEntries;
		}
		if (clip::isclp(iForm - 1)) {
			return formList[iForm - 1].angleOrClipData.clip + formList[iForm - 1].lengthOrCount.clipCount;
		}
	}
	return 0;
}

void clip::internal::clpsub(unsigned int fpnt, unsigned int cnt) {
	auto& formList = *FormList;
	for (auto iForm = fpnt + 1; iForm < FormIndex; iForm++) {
		if (clip::isclpx(iForm)) {
			formList[iForm].angleOrClipData.clip -= cnt;
		}
		if (clip::iseclpx(fpnt)) {
			formList[iForm].borderClipData -= cnt;
		}
	}
}

void clip::delmclp(unsigned int iForm) {
	if (ClipPointIndex) {
		if (clip::isclp(iForm)) {
			auto& form        = (*FormList)[iForm];
			auto  destination = ci::findclp(iForm);
			auto  source      = destination + form.lengthOrCount.clipCount;
			std::copy(&ClipPoints[source],
			          &ClipPoints[ClipPointIndex],
			          stdext::make_checked_array_iterator((&ClipPoints[destination]), (MAXITEMS - destination)));
			if (clip::iseclp(iForm)) {
				form.borderClipData -= form.lengthOrCount.clipCount;
			}
			ci::clpsub(iForm, form.lengthOrCount.clipCount);
			if (ClipPointIndex > form.lengthOrCount.clipCount) {
				ClipPointIndex -= form.lengthOrCount.clipCount;
			}
			else {
				ClipPointIndex = 0;
			}
			form.lengthOrCount.clipCount = 0;
		}
	}
}

void clip::deleclp(unsigned int iForm) {
	if (ClipPointIndex != 0u) {
		if (clip::iseclpx(iForm)) {
			auto& form        = (*FormList)[iForm];
			auto  destination = ci::findclp(iForm);
			auto  source      = destination + form.clipEntries;
			while (source < ClipPointIndex) {
				ClipPoints[destination++] = ClipPoints[source++];
			}
			ci::clpsub(iForm, form.clipEntries);
			if (ClipPointIndex > form.clipEntries) {
				ClipPointIndex -= form.clipEntries;
			}
			else {
				ClipPointIndex = 0;
			}
			form.clipEntries = 0;
		}
	}
}

void clip::delclps(unsigned int iForm) {
	clip::deleclp(iForm);
	clip::delmclp(iForm);
}

unsigned int clip::nueclp(unsigned int currentForm, unsigned int count) {
	auto  find        = ci::findclp(ClosestFormToCursor);
	auto  destination = ClipPointIndex + count - 1u;
	auto& formList    = *FormList;

	if (clip::isclp(ClosestFormToCursor)) {
		find += formList[ClosestFormToCursor].lengthOrCount.clipCount;
	}
	if (ClipPointIndex) {
		auto source = ClipPointIndex - 1;
		while (source + 1 >= find + 1) {
			ClipPoints[destination--] = ClipPoints[source--];
		}
	}
	for (auto iform = currentForm; iform < FormIndex; iform++) {
		if (clip::iseclpx(iform)) {
			formList[iform].borderClipData += count;
		}
	}
	for (auto iform = currentForm + 1; iform < FormIndex; iform++) {
		if (clip::isclp(iform)) {
			formList[iform].angleOrClipData.clip += count;
		}
	}
	ClipPointIndex += count;
	return find;
}

unsigned int clip::numclp() {
	const auto find        = ci::findclp(ClosestFormToCursor);
	auto       source      = ClipPointIndex - 1;
	auto       destination = ClipPointIndex + ClipStitchCount - 1;
	auto&      formList    = *FormList;

	while ((source + 1) > find) {
		ClipPoints[destination--] = ClipPoints[source--];
	}
	formList[ClosestFormToCursor].angleOrClipData.clip = find;
	if (clip::iseclpx(ClosestFormToCursor)) {
		formList[ClosestFormToCursor].borderClipData += ClipStitchCount;
	}
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (clip::isclpx(iForm)) {
			formList[iForm].angleOrClipData.clip += ClipStitchCount;
		}
		if (clip::iseclpx(iForm)) {
			formList[iForm].borderClipData += ClipStitchCount;
		}
	}
	ClipPointIndex += ClipStitchCount;
	return find;
}

void clip::oclp(unsigned int clipIndex, unsigned int clipEntries) {
	const auto* clip = &ClipPoints[clipIndex];
	if (!StateMap.test(StateFlag::NOCLP)) {
		// ToDo - use local vector for buffer
		for (auto iClip = 0u; iClip < clipEntries; iClip++) {
			ClipBuffer[iClip].x = clip[iClip].x;
			ClipBuffer[iClip].y = clip[iClip].y;
		}
		ClipRect.left = ClipRect.right = ClipBuffer[0].x;
		ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
		for (auto iClip = 1u; iClip < clipEntries; iClip++) {
			if (ClipBuffer[iClip].x < ClipRect.left) {
				ClipRect.left = ClipBuffer[iClip].x;
			}
			if (ClipBuffer[iClip].x > ClipRect.right) {
				ClipRect.right = ClipBuffer[iClip].x;
			}
			if (ClipBuffer[iClip].y < ClipRect.bottom) {
				ClipRect.bottom = ClipBuffer[iClip].y;
			}
			if (ClipBuffer[iClip].y > ClipRect.top) {
				ClipRect.top = ClipBuffer[iClip].y;
			}
		}
		ClipRectSize.cx = ClipRect.right - ClipRect.left;
		ClipRectSize.cy = ClipRect.top - ClipRect.bottom;
		ClipStitchCount = clipEntries;
	}
}

void clip::internal::durev(std::vector<fPOINT>& clipReversedData) {
	const auto midpoint = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;

	if (ClipBuffer[0].x > midpoint) {
		for (auto iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch].x = ClipRect.right - ClipBuffer[iStitch].x;
			clipReversedData[iStitch].y = ClipBuffer[iStitch].y;
		}
	}
	else {
		for (auto iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch] = ClipBuffer[iStitch];
		}
	}
}

void clip::internal::setvct(unsigned int start, unsigned int finish, double& clipAngle, dPOINT& vector0) noexcept {
	clipAngle = atan2(CurrentFormVertices[finish].y - CurrentFormVertices[start].y,
	                  CurrentFormVertices[finish].x - CurrentFormVertices[start].x);
	vector0.x = ClipRectSize.cx * cos(clipAngle);
	vector0.y = ClipRectSize.cx * sin(clipAngle);
}

bool clip::internal::nupnt(double clipAngle, dPOINT& moveToCoords, unsigned int currentSide) noexcept {
	const auto sinAngle = sin(clipAngle);
	const auto cosAngle = cos(clipAngle);

	moveToCoords = CurrentFormVertices[currentSide + 2];
	auto length  = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
	if (length > ClipRectSize.cx) {
		for (auto step = 0u; step < 10; step++) {
			length           = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
			const auto delta = ClipRectSize.cx - length;
			moveToCoords.x += delta * cosAngle;
			moveToCoords.y += delta * sinAngle;
			if (fabs(delta) < 0.01) {
				break;
			}
		}
		return true;
	}
	return false;
}

bool clip::internal::ritclp(const std::vector<fPOINT>& clipFillData, const fPOINT& point) {
	const auto adjustedPoint = fPOINT{ (point.x - ClipReference.x), (point.y - ClipReference.y) };

	if (form::chkmax(ClipStitchCount, SequenceIndex)) {
		return true;
	}
	for (auto iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
		OSequence[SequenceIndex].x   = clipFillData[iStitch].x + adjustedPoint.x;
		OSequence[SequenceIndex++].y = clipFillData[iStitch].y + adjustedPoint.y;
	}
	return false;
}

void clip::internal::lincrnr(const std::vector<fPOINT>& clipReversedData,
                             std::vector<fPOINT>&       clipFillData,
                             double                     clipAngle,
                             dPOINT&                    moveToCoords,
                             const dPOINT&              rotationCenter,
                             unsigned int               currentSide) {
	auto delta = dPOINT{};

	if (ci::nupnt(clipAngle, moveToCoords, currentSide)) {
		delta = dPOINT{ moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y };

		const auto rotationAngle = atan2(delta.y, delta.x);
		thred::rotangf(BorderClipReference, ClipReference, rotationAngle, rotationCenter);
		for (auto iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
			thred::rotangf(clipReversedData[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
		}
		ci::ritclp(clipFillData, SelectedPoint);
		SelectedPoint = moveToCoords;
	}
}

void clip::internal::linsid(const std::vector<fPOINT>& clipReversedData,
                            std::vector<fPOINT>&       clipFillData,
                            double                     clipAngle,
                            const dPOINT&              vector0,
                            const dPOINT&              rotationCenter,
                            unsigned int               currentSide) {
	const auto& point     = CurrentFormVertices[currentSide + 1];
	const auto  delta     = fPOINT{ (point.x - SelectedPoint.x), (point.y - SelectedPoint.y) };
	const auto  length    = hypot(delta.x, delta.y);
	const auto  clipCount = gsl::narrow<unsigned int>(std::floor(length / ClipRectSize.cx));

	if (clipCount) {
		thred::rotangf(BorderClipReference, ClipReference, clipAngle, rotationCenter);
		for (auto iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
			thred::rotangf(clipReversedData[iStitch], clipFillData[iStitch], clipAngle, rotationCenter);
		}
		for (auto iClip = 0u; iClip < clipCount; iClip++) {
			ci::ritclp(clipFillData, SelectedPoint);
			SelectedPoint.x += vector0.x;
			SelectedPoint.y += vector0.y;
		}
	}
}

void clip::clpout() {
	if (SelectedForm->type == FRMLINE) {
		satin::satout(HorizontalLength2);
	}
	else {
		satin::satout(ClipRectSize.cy);
		InsidePointList->clear();
		// ToDo - insert should be replaced with this line once vertices is a std::vector * as a copy will not be
		// required  InsidePoints = SelectedForm->vertices;
		auto _
		    = InsidePointList->insert(InsidePointList->end(), &SelectedForm->vertices[0], &SelectedForm->vertices[VertexCount]);
		InsidePoints = InsidePointList;
	}
}

bool clip::internal::clpsid(const std::vector<fPOINT>& clipReversedData,
                            std::vector<fPOINT>&       clipFillData,
                            unsigned int               start,
                            unsigned int               finish,
                            const dPOINT&              rotationCenter) {
	const auto& end                = CurrentFormVertices[finish];
	const auto& begin              = CurrentFormVertices[start];
	const auto  delta              = fPOINT{ (end.x - begin.x), (end.y - begin.y) };
	const auto  length             = hypot(delta.x, delta.y);
	const auto  clipReferencePoint = fPOINTATTR{ ClipRect.left, ClipRect.bottom };
	const auto  rotationAngle      = atan2(delta.y, delta.x);

	thred::rotang1(clipReferencePoint, ClipReference, rotationAngle, rotationCenter);
	const auto clipCount = gsl::narrow<unsigned int>(std::floor(length / ClipRectSize.cx));
	if (clipCount) {
		auto remainder = 0.0f;
		if (clipCount > 1) {
			remainder = ((length - clipCount * ClipRectSize.cx) / (clipCount - 1) + ClipRectSize.cx) / length;
		}
		else {
			remainder = (length - ClipRectSize.cx) / 2;
		}
		const auto step        = fPOINT{ delta.x * remainder, delta.y * remainder };
		auto       insertPoint = begin;
		for (auto index = 0u; index < ClipStitchCount; index++) {
			thred::rotangf(clipReversedData[index], clipFillData[index], rotationAngle, rotationCenter);
		}
		for (auto stepCount = 0u; stepCount < clipCount; stepCount++) {
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

void clip::clpbrd(unsigned int startVertex) {
	auto moveToCoords = dPOINT{}; // moving formOrigin for clipboard fill

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2           = ClipRectSize.cx / 2;
	HorizontalLength            = ClipRectSize.cx;
	auto       clipFillData     = std::vector<fPOINT>(ClipStitchCount);
	auto       clipReversedData = std::vector<fPOINT>(ClipStitchCount);
	const auto rotationCenter
	    = dPOINT{ (ClipRect.right - ClipRect.left) / 2 + ClipRect.left, (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom };
	ClipReference.x = ClipRect.left;
	ClipReference.y = rotationCenter.y;
	ci::durev(clipReversedData);
	if (SelectedForm->type == FRMLINE) {
		SelectedPoint  = CurrentFormVertices[0];
		auto clipAngle = 0.0;      // for clipboard border fill
		auto vector0   = dPOINT{}; // x size of the clipboard fill at the fill angle
		ci::setvct(0, 1, clipAngle, vector0);
		// Since ClipRect.bottom is always 0
		BorderClipReference.y = ClipRect.top / 2;
		// Use 0 to align left edge of clip with beginning of line, ClipRect.right / 2 if you want to align
		// the center of the clip with the beginning of the line
		BorderClipReference.x = 0;
		// BorderClipReference.x = ClipRect.right / 2;
		auto currentSide = 0u;
		for (currentSide = 0u; currentSide < VertexCount - 2; currentSide++) {
			ci::linsid(clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide);
			ci::setvct(currentSide + 1, currentSide + 2, clipAngle, vector0);
			ci::lincrnr(clipReversedData, clipFillData, clipAngle, moveToCoords, rotationCenter, currentSide);
		}
		ci::linsid(clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide);
	}
	else {
		clpout();
		auto reference     = startVertex;
		auto currentVertex = startVertex;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			const auto nextVertex = form::prv(currentVertex);
			if (ci::clpsid(clipReversedData, clipFillData, reference, nextVertex, rotationCenter)) {
				reference = nextVertex;
			}
			currentVertex = nextVertex;
		}
	}
}

bool clip::internal::fxpnt(const std::vector<double>& listSINEs,
                           const std::vector<double>& listCOSINEs,
                           dPOINT&                    moveToCoords,
                           unsigned                   currentSide) {
	moveToCoords = CurrentFormVertices[NextStart];
	auto length  = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
	if (length > AdjustedSpace) {
		for (auto iGuess = 0u; iGuess < 10; iGuess++) {
			length           = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
			const auto delta = AdjustedSpace - length;
			moveToCoords.x += delta * listCOSINEs[currentSide];
			moveToCoords.y += delta * listSINEs[currentSide];
			if (fabs(delta) < 0.2) {
				break;
			}
		}
		return true;
	}
	return false;
}

void clip::internal::fxlit(const std::vector<double>& listSINEs,
                           const std::vector<double>& listCOSINEs,
                           dPOINT&                    moveToCoords,
                           unsigned                   currentSide) {
	if (ci::fxpnt(listSINEs, listCOSINEs, moveToCoords, currentSide)) {
		SelectedPoint = moveToCoords;
		BeanCount++;
		const auto length
		    = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		const auto count = gsl::narrow<unsigned int>(std::floor(length / AdjustedSpace));
		const auto delta = dPOINT{ AdjustedSpace * listCOSINEs[currentSide], AdjustedSpace * listSINEs[currentSide] };
		SelectedPoint.x += delta.x * count;
		SelectedPoint.y += delta.y * count;
		BeanCount += count;
	}
}

void clip::internal::fxlin(std::vector<fPOINT>&       chainEndPoints,
                           const std::vector<double>& ListSINEs,
                           const std::vector<double>& ListCOSINEs,
                           dPOINT&                    moveToCoords,
                           unsigned                   currentSide) {
	if (ci::fxpnt(ListSINEs, ListCOSINEs, moveToCoords, currentSide)) {
		SelectedPoint = moveToCoords;
		chainEndPoints.push_back(SelectedPoint);
		const auto length
		    = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		const auto count = gsl::narrow<unsigned int>(std::floor(length / AdjustedSpace));
		const auto delta = dPOINT{ AdjustedSpace * ListCOSINEs[currentSide], AdjustedSpace * ListSINEs[currentSide] };
		for (auto iChain = 0u; iChain < count; iChain++) {
			SelectedPoint.x += delta.x;
			SelectedPoint.y += delta.y;
			chainEndPoints.push_back(SelectedPoint);
		}
	}
}

void clip::internal::fxlen(std::vector<fPOINT>&       chainEndPoints,
                           const std::vector<double>& listSINEs,
                           const std::vector<double>& listCOSINEs) {
	auto moveToCoords = dPOINT{}; // moving formOrigin for clipboard fill

	AdjustedSpace = 0;
	auto flag     = true;
	for (auto iVertex = 1u; iVertex < VertexCount; iVertex++) {
		const auto length = hypot(CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x,
		                          CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y);
		if (length > SelectedForm->edgeSpacing) {
			flag = false;
			break;
		}

		if (length > AdjustedSpace) {
			AdjustedSpace = length;
		}
	}
	if (flag) {
		chainEndPoints.push_back(CurrentFormVertices[0]);
		chainEndPoints.push_back(CurrentFormVertices[1]);
		return;
	}
	AdjustedSpace              = SelectedForm->edgeSpacing;
	auto       minimumSpacing  = SelectedForm->edgeSpacing;
	const auto halfSpacing     = AdjustedSpace / 2;
	auto       interval        = 1e9;
	auto       minimumInterval = 1e9;
	auto       loopCount       = 0u;
	auto       initialCount    = 0u;
	auto       smallestSpacing = 0.0;
	auto       largestSpacing  = 1.0;
	// loop at least 50 times to guarantee convergence
	while (loopCount < 50 && (largestSpacing - smallestSpacing) > TINY) {
		BeanCount        = 0;
		SelectedPoint    = CurrentFormVertices[0];
		auto currentSide = 0u;
		for (currentSide = 0u; currentSide < VertexCount - 1; currentSide++) {
			NextStart = currentSide + 1;
			ci::fxlit(listSINEs, listCOSINEs, moveToCoords, currentSide);
		}
		if (SelectedForm->type != FRMLINE) {
			NextStart = 0;
			ci::fxlit(listSINEs, listCOSINEs, moveToCoords, currentSide);
		}
		else {
			NextStart = VertexCount - 1;
		}
		if (!initialCount) {
			initialCount    = BeanCount;
			smallestSpacing = AdjustedSpace;
			minimumInterval
			    = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
			interval       = minimumInterval;
			minimumSpacing = AdjustedSpace;
			interval /= initialCount;
			AdjustedSpace += interval / 2;
			largestSpacing = smallestSpacing + interval;
		}
		else {
			interval
			    = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
			if (interval > halfSpacing) {
				interval = SelectedForm->edgeSpacing - interval;
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
	SelectedPoint = CurrentFormVertices[0];
	OutputIndex   = 1;
	AdjustedSpace = minimumSpacing;
	chainEndPoints.push_back(SelectedPoint);
	auto currentSide = 0u;
	for (currentSide = 0; currentSide < VertexCount - 1; currentSide++) {
		NextStart = currentSide + 1;
		ci::fxlin(chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide);
	}
	if (SelectedForm->type != FRMLINE) {
		NextStart = 0;
		ci::fxlin(chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide);
	}
	interval = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
	if (interval > halfSpacing) {
		chainEndPoints.push_back(CurrentFormVertices[NextStart]);
	}
	else {
		chainEndPoints.back() = CurrentFormVertices[NextStart];
	}
}

void clip::internal::dufxlen(std::vector<fPOINT>& chainEndPoints) {
	form::duangs();
	auto listSINEs = std::vector<double>();
	listSINEs.reserve(gsl::narrow<size_t>(VertexCount) + 1);
	auto listCOSINEs = std::vector<double>();
	listCOSINEs.reserve(VertexCount);
	// Todo - would emplace_back be better here?
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
		listSINEs.push_back(sin((*FormAngles)[iVertex]));
		listCOSINEs.push_back(cos((*FormAngles)[iVertex]));
	}
	listSINEs.push_back(sin(((*FormAngles)[0] > (*FormAngles)[VertexCount - 1])
	                            ? ((*FormAngles)[0] - (*FormAngles)[VertexCount - 1])
	                            : ((*FormAngles)[VertexCount - 1] - (*FormAngles)[0])));
	ci::fxlen(chainEndPoints, listSINEs, listCOSINEs);
}

void clip::internal::dulast(std::vector<fPOINT>& chainEndPoints) {
	auto tempClipPoints = std::vector<fPOINT>();
	tempClipPoints.reserve(chainEndPoints.size());
	if (form::lastch()) {
		auto minimumLength = 1e99;
		auto minimumIndex  = 0u;
		for (auto iPoint = 0u; iPoint < chainEndPoints.size() - 1; iPoint++) {
			const auto length = hypot(LastPoint.x - chainEndPoints[iPoint].x, LastPoint.y - chainEndPoints[iPoint].y);
			if (length < minimumLength) {
				minimumLength = length;
				minimumIndex  = iPoint;
			}
		}
		if (minimumIndex) {
			// Todo - would emplace_back be better here?
			for (auto iPoint = minimumIndex; iPoint < chainEndPoints.size() - 2; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			for (auto iPoint = 0u; iPoint <= minimumIndex; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			chainEndPoints = tempClipPoints;
		}
	}
}

void clip::internal::clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints) {
	ci::dulast(chainEndPoints);
	if (SelectedForm->type == FRMLINE) {
		const auto pivot = ClipRectSize.cy / 2;
		for (auto iPoint = 0u; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.emplace_back(ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y + pivot));
		}
	}
	else {
		for (auto iPoint = 0u; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.emplace_back(ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y));
		}
	}
}

void clip::internal::xclpfn(const std::vector<fPOINT>& tempClipPoints,
                            const std::vector<fPOINT>& chainEndPoints,
                            unsigned                   start,
                            unsigned                   finish,
                            const dPOINT&              rotationCenter) {
	auto points = std::vector<fPOINT>(ClipStitchCount);

	const auto delta
	    = dPOINT{ (chainEndPoints[finish].x - chainEndPoints[start].x), (chainEndPoints[finish].y - chainEndPoints[start].y) };
	const auto rotationAngle = atan2(delta.y, delta.x);
	for (auto iPoint = 0u; iPoint < ClipStitchCount; iPoint++) {
		points[iPoint] = tempClipPoints[iPoint];
		thred::rotflt(points[iPoint], rotationAngle, rotationCenter);
		OSequence[SequenceIndex].x   = chainEndPoints[start].x + points[iPoint].x;
		OSequence[SequenceIndex++].y = chainEndPoints[start].y + points[iPoint].y;
	}
}

void clip::duxclp() {
	auto chainEndPoints = std::vector<fPOINT>();
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	ci::dufxlen(chainEndPoints);
	auto tempClipPoints = std::vector<fPOINT>();
	tempClipPoints.reserve(ClipStitchCount);
	ci::clpxadj(tempClipPoints, chainEndPoints);
	SequenceIndex = 0;
	// ToDo - does this make sense?
	const auto rotationCenter = dPOINT{};
	for (auto iPoint = 1u; iPoint < chainEndPoints.size(); iPoint++) {
		ci::xclpfn(tempClipPoints, chainEndPoints, iPoint - 1, iPoint, rotationCenter);
	}
	if (SelectedForm->type != FRMLINE) {
		OSequence[SequenceIndex++] = chainEndPoints[0];
	}
}

void clip::internal::clpcrnr(std::vector<fPOINT>& clipFillData, unsigned int vertex, const dPOINT& rotationCenter) {
	const auto nextVertex = form::nxt(vertex);
	auto       delta      = dPOINT{};

	if (StateMap.test(StateFlag::INDIR)) {
		delta = fPOINT{ (*OutsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x,
			            (*OutsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y };
	}
	else {
		delta = fPOINT{ (*InsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x,
			            (*InsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y };
	}
	const auto rotationAngle  = atan2(delta.y, delta.x) + PI / 2;
	const auto referencePoint = fPOINTATTR{ ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ClipRect.top };
	thred::rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
	for (auto iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
		thred::rotang1(ClipBuffer[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
	}
	const auto length = hypot(delta.x, delta.y);
	const auto ratio  = form::getplen() / length;
	delta.x *= ratio;
	delta.y *= ratio;
	const auto point = fPOINT{ CurrentFormVertices[nextVertex].x + delta.x, CurrentFormVertices[nextVertex].y + delta.y };
	OSequence[SequenceIndex++] = CurrentFormVertices[nextVertex];
	OSequence[SequenceIndex++] = point;
	OSequence[SequenceIndex++] = CurrentFormVertices[nextVertex];
	OSequence[SequenceIndex++] = point;
	if (!ci::ritclp(clipFillData, point)) {
		OSequence[SequenceIndex++] = point;
		OSequence[SequenceIndex++] = CurrentFormVertices[nextVertex];
	}
}

void clip::internal::picfn(std::vector<fPOINT>& clipFillData,
                           unsigned int         start,
                           unsigned int         finish,
                           double               spacing,
                           const dPOINT&        rotationCenter) {
	const auto       delta          = dPOINT{ (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
                               (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	const auto       length         = hypot(delta.x, delta.y);
	const fPOINTATTR referencePoint = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ClipRect.top };

	auto       rotationAngle = atan2(-delta.x, delta.y);
	const auto outerStep = dPOINT{ SelectedForm->borderSize * cos(rotationAngle), SelectedForm->borderSize * sin(rotationAngle) };
	spacing += ClipRectSize.cx;
	const auto count = gsl::narrow<unsigned int>(std::round(length / spacing));
	rotationAngle    = atan2(delta.y, delta.x);
	thred::rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
	if (count) {
		auto step = dPOINT{};
		if (count > 1) {
			const auto tdub = ((length - count * spacing) / (count - 1) + spacing) / length;
			const auto val  = dPOINT{ delta.x * tdub, delta.y * tdub };
			step            = val;
		}
		for (auto iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
			thred::rotang1(ClipBuffer[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
		}
		auto flag       = true;
		auto innerPoint = fPOINT{ CurrentFormVertices[start].x, CurrentFormVertices[start].y };
		for (auto iStep = 0u; iStep < count - 1; iStep++) {
			const auto firstPoint      = fPOINT{ innerPoint.x + step.x, innerPoint.y + step.y };
			const auto outerPoint      = fPOINT{ firstPoint.x + outerStep.x, firstPoint.y + outerStep.y };
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = innerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = outerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = outerPoint;
			if (ci::ritclp(clipFillData, outerPoint)) {
				flag = false;
				break;
			}
			OSequence[SequenceIndex++] = outerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			innerPoint.x += step.x;
			innerPoint.y += step.y;
		}
		if (flag) {
			OSequence[SequenceIndex++] = CurrentFormVertices[finish];
			OSequence[SequenceIndex++] = innerPoint;
		}
	}
}

void clip::clpic() {
	auto clipFillData = std::vector<fPOINT>(ClipStitchCount);

	const auto rotationCenter = dPOINT{ ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left),
		                                ((ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom) };

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength  = ClipRectSize.cx;
	ClipReference.y   = rotationCenter.y;
	ClipReference.x   = ClipRect.left;
	satin::satout(20);
	if (SelectedForm->type == FRMLINE) {
		for (auto iVertex = 0u; iVertex < VertexCount - 2; iVertex++) {
			ci::picfn(clipFillData, iVertex, iVertex + 1, SelectedForm->edgeSpacing, rotationCenter);
			ci::clpcrnr(clipFillData, iVertex, rotationCenter);
		}
		ci::picfn(clipFillData, VertexCount - 2, VertexCount - 1, SelectedForm->edgeSpacing, rotationCenter);
	}
	else {
		if (!SelectedForm->fillType) {
			OSequence[SequenceIndex++] = CurrentFormVertices[0];
		}
		auto currentVertex = 0u;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			const auto nextVertex = form::nxt(currentVertex);
			ci::picfn(clipFillData, currentVertex, nextVertex, SelectedForm->edgeSpacing, rotationCenter);
			ci::clpcrnr(clipFillData, currentVertex, rotationCenter);
			currentVertex = nextVertex;
		}
		OSequence[SequenceIndex++] = CurrentFormVertices[currentVertex];
	}
}

void clip::internal::duchfn(const std::vector<fPOINT>& chainEndPoints, unsigned int start, unsigned int finish) {
	const auto chainSequence = std::vector<unsigned>{ 0, 1, 2, 3, 0, 1, 4, 3, 0, 3 }; // chain stitch sequence

	auto chainPoint = std::vector<fPOINT>(5);
	auto delta
	    = dPOINT{ (chainEndPoints[finish].x - chainEndPoints[start].x), (chainEndPoints[finish].y - chainEndPoints[start].y) };
	const auto lengthDelta  = dPOINT{ (delta.x * SelectedForm->edgeStitchLen), (delta.y * SelectedForm->edgeStitchLen) };
	const auto angle        = atan2(delta.y, delta.x) + PI / 2;
	const auto offset       = dPOINT{ (cos(angle) * SelectedForm->borderSize), (sin(angle) * SelectedForm->borderSize) };
	const auto middleXcoord = chainEndPoints[start].x + lengthDelta.x;
	const auto middleYcoord = chainEndPoints[start].y + lengthDelta.y;

	chainPoint[0]   = chainEndPoints[start];
	chainPoint[4]   = chainEndPoints[finish];
	chainPoint[1].x = middleXcoord + offset.x;
	chainPoint[1].y = middleYcoord + offset.y;
	chainPoint[3].x = middleXcoord - offset.x;
	chainPoint[3].y = middleYcoord - offset.y;
	if (finish < chainEndPoints.size() - 1) {
		delta.x = static_cast<double>(chainEndPoints[gsl::narrow<size_t>(finish) + 1].x) - chainEndPoints[finish].x;
		delta.y = static_cast<double>(chainEndPoints[gsl::narrow<size_t>(finish) + 1].y) - chainEndPoints[finish].y;
	}
	else {
		delta.x = static_cast<double>(chainEndPoints[finish].x) - chainEndPoints[finish - 1].x;
		delta.y = static_cast<double>(chainEndPoints[finish].y) - chainEndPoints[finish - 1].y;
	}
	chainPoint[2].x = chainEndPoints[finish].x + delta.x / 4.0;
	chainPoint[2].y = chainEndPoints[finish].y + delta.y / 4.0;
	auto chainCount = gsl::narrow<unsigned int>(chainSequence.size());
	if (StateMap.test(StateFlag::LINCHN)) {
		chainCount--;
	}
	for (auto iChain = 0u; iChain < chainCount; iChain++) {
		OSequence[SequenceIndex] = chainPoint[chainSequence[iChain]];
		SequenceIndex++;
	}
}

void clip::internal::duch(std::vector<fPOINT>& chainEndPoints) {
	SequenceIndex    = 0;
	auto chainLength = gsl::narrow<unsigned int>(chainEndPoints.size());
	if (chainLength > 2u) {
		chainLength--;
		for (auto iPoint = 0u; iPoint < chainLength - 1; iPoint++) {
			ci::duchfn(chainEndPoints, iPoint, iPoint + 1);
		}
		if (SelectedForm->type == FRMLINE) {
			ci::duchfn(chainEndPoints, chainLength - 1, chainLength);
			auto backupAt = 8u;
			if (StateMap.test(StateFlag::LINCHN)) {
				backupAt--;
			}
			if ((SequenceIndex >= backupAt)) {
				OSequence[SequenceIndex - backupAt] = chainEndPoints[chainLength];
			}
			OSequence[SequenceIndex++] = chainEndPoints[chainLength];
		}
		else {
			ci::duchfn(chainEndPoints, chainLength - 1, 0);
			OSequence[SequenceIndex++] = chainEndPoints[chainLength];
		}
	}
	else {
		displayText::tabmsg(IDS_CHANSMAL);
	}
}

void clip::chnfn() {
	auto chainEndPoints = std::vector<fPOINT>();
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	form::fvars(ClosestFormToCursor);
	clip::deleclp(ClosestFormToCursor);
	ci::dufxlen(chainEndPoints);
	ci::dulast(chainEndPoints);
	SequenceIndex = 0;
	ci::duch(chainEndPoints);
}
