#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// C RunTime Header Files

// Open Source headers
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header
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

fPOINT ClipReference;       // clipboard reference formOrigin
fPOINT BorderClipReference; // reference for clipboard line border
double AdjustedSpace;       // adjusted space
size_t NextStart;           // index of the endpoint of the line segment being processed

bool clip::iseclp(size_t iForm) noexcept {
	if (FormList[iForm].edgeType == EDGECLIP || FormList[iForm].edgeType == EDGEPICOT || FormList[iForm].edgeType == EDGECLIPX)
		return true;
	return false;
}

bool clip::isclp(size_t iForm) noexcept {
	if ((1 << FormList[iForm].fillType) & ClipTypeMap)
		return true;
	return false;
}

bool clip::isclpx(size_t iForm) noexcept {
	if (clip::isclp(iForm) && FormList[iForm].lengthOrCount.clipCount)
		return true;
	return false;
}

bool clip::iseclpx(size_t iForm) noexcept {
	if (clip::iseclp(iForm) && FormList[iForm].clipEntries)
		return true;
	return false;
}

size_t clipPriv::findclp(size_t formIndex) noexcept {
	for (auto iForm = formIndex; iForm != 0; iForm--) {
		if (clip::iseclp(iForm - 1))
			return FormList[iForm - 1].borderClipData - ClipPoints + FormList[iForm - 1].clipEntries;
		if (clip::isclp(iForm - 1))
			return FormList[iForm - 1].angleOrClipData.clip - ClipPoints + FormList[iForm - 1].lengthOrCount.clipCount;
	}
	return 0;
}

void clipPriv::clpsub(size_t fpnt, size_t cnt) noexcept {
	for (auto iForm = fpnt + 1; iForm < FormIndex; iForm++) {
		if (clip::isclpx(iForm))
			FormList[iForm].angleOrClipData.clip -= cnt;
		if (clip::iseclpx(fpnt))
			FormList[iForm].borderClipData -= cnt;
	}
}

void clip::delmclp(size_t iForm) {
	if (ClipPointIndex) {
		if (clip::isclp(iForm)) {
			auto destination = clipPriv::findclp(iForm);
			auto source      = destination + FormList[iForm].lengthOrCount.clipCount;
			std::copy(ClipPoints + source,
			          ClipPoints + ClipPointIndex,
			          stdext::make_checked_array_iterator((ClipPoints + destination), (MAXITEMS - destination)));
			if (clip::iseclp(iForm))
				FormList[iForm].borderClipData -= FormList[iForm].lengthOrCount.clipCount;
			clipPriv::clpsub(iForm, FormList[iForm].lengthOrCount.clipCount);
			if (ClipPointIndex > FormList[iForm].lengthOrCount.clipCount)
				ClipPointIndex -= FormList[iForm].lengthOrCount.clipCount;
			else
				ClipPointIndex = 0;
			FormList[iForm].lengthOrCount.clipCount = 0;
		}
	}
}

void clip::deleclp(size_t iForm) noexcept {
	if (ClipPointIndex) {
		if (clip::iseclpx(iForm)) {
			auto destination = clipPriv::findclp(iForm);
			auto source      = destination + FormList[iForm].clipEntries;
			while (source < ClipPointIndex) {
				ClipPoints[destination++] = ClipPoints[source++];
			}
			clipPriv::clpsub(iForm, FormList[iForm].clipEntries);
			if (ClipPointIndex > FormList[iForm].clipEntries)
				ClipPointIndex -= FormList[iForm].clipEntries;
			else
				ClipPointIndex = 0;
			FormList[iForm].clipEntries = 0;
		}
	}
}

void clip::delclps(size_t iForm) {
	clip::deleclp(iForm);
	clip::delmclp(iForm);
}

fPOINT* clip::nueclp(size_t currentForm, size_t count) noexcept {
	auto find        = clipPriv::findclp(ClosestFormToCursor);
	auto source      = ClipPointIndex - 1;
	auto destination = ClipPointIndex + count - 1;

	if (clip::isclp(ClosestFormToCursor))
		find += FormList[ClosestFormToCursor].lengthOrCount.clipCount;
	while (source >= find) {
		ClipPoints[destination--] = ClipPoints[source--];
	}
	for (auto iform = currentForm; iform < FormIndex; iform++) {
		if (clip::iseclpx(iform))
			FormList[iform].borderClipData += count;
	}
	for (auto iform = currentForm + 1; iform < FormIndex; iform++) {
		if (clip::isclp(iform))
			FormList[iform].angleOrClipData.clip += count;
	}
	ClipPointIndex += count;
	return &ClipPoints[find];
}

fPOINT* clip::numclp() noexcept {
	auto find        = clipPriv::findclp(ClosestFormToCursor);
	auto source      = ClipPointIndex - 1;
	auto destination = ClipPointIndex + ClipStitchCount - 1;

	while ((source + 1) > find) {
		ClipPoints[destination--] = ClipPoints[source--];
	}
	FormList[ClosestFormToCursor].angleOrClipData.clip = &ClipPoints[find];
	if (clip::iseclpx(ClosestFormToCursor))
		FormList[ClosestFormToCursor].borderClipData += ClipStitchCount;
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (clip::isclpx(iForm))
			FormList[iForm].angleOrClipData.clip += ClipStitchCount;
		if (clip::iseclpx(iForm))
			FormList[iForm].borderClipData += ClipStitchCount;
	}
	ClipPointIndex += ClipStitchCount;
	return &ClipPoints[find];
}

void clip::oclp(const fPOINT* const clip, size_t clipEntries) {
	if (clip) {
		if (!StateMap.test(StateFlag::NOCLP)) {
			for (size_t iClip = 0u; iClip < clipEntries; iClip++) {
				ClipBuffer[iClip].x = clip[iClip].x;
				ClipBuffer[iClip].y = clip[iClip].y;
			}
			ClipRect.left = ClipRect.right = ClipBuffer[0].x;
			ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
			for (size_t iClip = 1u; iClip < clipEntries; iClip++) {
				if (ClipBuffer[iClip].x < ClipRect.left)
					ClipRect.left = ClipBuffer[iClip].x;
				if (ClipBuffer[iClip].x > ClipRect.right)
					ClipRect.right = ClipBuffer[iClip].x;
				if (ClipBuffer[iClip].y < ClipRect.bottom)
					ClipRect.bottom = ClipBuffer[iClip].y;
				if (ClipBuffer[iClip].y > ClipRect.top)
					ClipRect.top = ClipBuffer[iClip].y;
			}
			ClipRectSize.cx = ClipRect.right - ClipRect.left;
			ClipRectSize.cy = ClipRect.top - ClipRect.bottom;
			ClipStitchCount = clipEntries;
		}
	}
}

void clipPriv::durev(std::vector<fPOINT>& clipReversedData) {
	const double midpoint = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;

	if (ClipBuffer[0].x > midpoint) {
		for (size_t iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch].x = ClipRect.right - ClipBuffer[iStitch].x;
			clipReversedData[iStitch].y = ClipBuffer[iStitch].y;
		}
	}
	else {
		for (size_t iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch] = ClipBuffer[iStitch];
		}
	}
}

void clipPriv::setvct(size_t start, size_t finish, double& clipAngle, dPOINT& vector0) noexcept {
	clipAngle = atan2(CurrentFormVertices[finish].y - CurrentFormVertices[start].y,
	                  CurrentFormVertices[finish].x - CurrentFormVertices[start].x);
	vector0.x = ClipRectSize.cx * cos(clipAngle);
	vector0.y = ClipRectSize.cx * sin(clipAngle);
}

bool clipPriv::nupnt(double clipAngle, dPOINT& moveToCoords, size_t currentSide) noexcept {
	const double sinAngle = sin(clipAngle);
	const double cosAngle = cos(clipAngle);

	moveToCoords = CurrentFormVertices[currentSide + 2];
	auto length  = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
	if (length > ClipRectSize.cx) {
		for (auto step = 0u; step < 10; step++) {
			length           = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
			const auto delta = ClipRectSize.cx - length;
			moveToCoords.x += delta * cosAngle;
			moveToCoords.y += delta * sinAngle;
			if (fabs(delta) < 0.01)
				break;
		}
		return true;
	}
	return false;
}

bool clipPriv::ritclp(const std::vector<fPOINT>& clipFillData, const fPOINT& point) {
	const fPOINT adjustedPoint = { (point.x - ClipReference.x), (point.y - ClipReference.y) };

	if (chkmax(ClipStitchCount, SequenceIndex))
		return true;
	for (size_t iStitch = 0u; iStitch < ClipStitchCount; iStitch++) {
		OSequence[SequenceIndex].x   = clipFillData[iStitch].x + adjustedPoint.x;
		OSequence[SequenceIndex++].y = clipFillData[iStitch].y + adjustedPoint.y;
	}
	return false;
}

void clipPriv::lincrnr(const std::vector<fPOINT>& clipReversedData,
             std::vector<fPOINT>&       clipFillData,
             double                     clipAngle,
             dPOINT&                    moveToCoords,
             const dPOINT&              rotationCenter,
             size_t                     currentSide) {
	dPOINT delta = {};

	if (clipPriv::nupnt(clipAngle, moveToCoords, currentSide)) {
		delta.x                    = moveToCoords.x - SelectedPoint.x;
		delta.y                    = moveToCoords.y - SelectedPoint.y;
		const double rotationAngle = atan2(delta.y, delta.x);
		rotangf(BorderClipReference, ClipReference, rotationAngle, rotationCenter);
		for (size_t iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotangf(clipReversedData[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
		clipPriv::ritclp(clipFillData, SelectedPoint);
		SelectedPoint = moveToCoords;
	}
}

void clipPriv::linsid(const std::vector<fPOINT>& clipReversedData,
            std::vector<fPOINT>&       clipFillData,
            double                     clipAngle,
            const dPOINT&              vector0,
            const dPOINT&              rotationCenter,
            size_t                     currentSide) {
	const auto&    point     = CurrentFormVertices[currentSide + 1];
	const fPOINT   delta     = { (point.x - SelectedPoint.x), (point.y - SelectedPoint.y) };
	const auto     length    = hypot(delta.x, delta.y);
	const unsigned clipCount = floor(length / ClipRectSize.cx);

	if (clipCount) {
		rotangf(BorderClipReference, ClipReference, clipAngle, rotationCenter);
		for (size_t iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotangf(clipReversedData[iStitch], clipFillData[iStitch], clipAngle, rotationCenter);
		for (auto iClip = 0u; iClip < clipCount; iClip++) {
			clipPriv::ritclp(clipFillData, SelectedPoint);
			SelectedPoint.x += vector0.x;
			SelectedPoint.y += vector0.y;
		}
	}
}

void clip::clpout() {
	if (SelectedForm->type == FRMLINE)
		satout(HorizontalLength2);
	else {
		satout(ClipRectSize.cy);
		InsidePointList->clear();
		// ToDo - insert should be replaced with this line once vertices is a std::vector * as a copy will not be
		// required  InsidePoints = SelectedForm->vertices;
		auto _
		    = InsidePointList->insert(InsidePointList->end(), &SelectedForm->vertices[0], &SelectedForm->vertices[VertexCount]);
		InsidePoints = InsidePointList;
	}
}

bool clipPriv::clpsid(const std::vector<fPOINT>& clipReversedData,
            std::vector<fPOINT>&       clipFillData,
            size_t                     start,
            size_t                     finish,
            const dPOINT&              rotationCenter) {
	const auto&      end                = CurrentFormVertices[finish];
	const auto&      begin              = CurrentFormVertices[start];
	const fPOINT     delta              = { (end.x - begin.x), (end.y - begin.y) };
	const auto       length             = hypot(delta.x, delta.y);
	const fPOINTATTR clipReferencePoint = { ClipRect.left, ClipRect.bottom };
	const double     rotationAngle      = atan2(delta.y, delta.x);

	rotang1(clipReferencePoint, ClipReference, rotationAngle, rotationCenter);
	const unsigned int clipCount = floor(length / ClipRectSize.cx);
	if (clipCount) {
		float remainder = 0.0;
		if (clipCount > 1)
			remainder = ((length - clipCount * ClipRectSize.cx) / (clipCount - 1) + ClipRectSize.cx) / length;
		else
			remainder = (length - ClipRectSize.cx) / 2;
		const fPOINT step        = { delta.x * remainder, delta.y * remainder };
		auto         insertPoint = begin;
		for (size_t index = 0; index < ClipStitchCount; index++)
			rotangf(clipReversedData[index], clipFillData[index], rotationAngle, rotationCenter);
		for (auto stepCount = 0u; stepCount < clipCount; stepCount++) {
			if (clipPriv::ritclp(clipFillData, insertPoint))
				break;
			insertPoint.x += step.x;
			insertPoint.y += step.y;
		}
		return true;
	}
	return false;
}

void clip::clpbrd(size_t startVertex) {
	dPOINT moveToCoords = {}; // moving formOrigin for clipboard fill

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength  = ClipRectSize.cx;
	std::vector<fPOINT> clipFillData(ClipStitchCount);
	std::vector<fPOINT> clipReversedData(ClipStitchCount);
	const dPOINT        rotationCenter
	    = { (ClipRect.right - ClipRect.left) / 2 + ClipRect.left, (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom };
	ClipReference.x = ClipRect.left;
	ClipReference.y = rotationCenter.y;
	clipPriv::durev(clipReversedData);
	if (SelectedForm->type == FRMLINE) {
		SelectedPoint    = CurrentFormVertices[0];
		double clipAngle = 0.0; // for clipboard border fill
		dPOINT vector0   = {};  // x size of the clipboard fill at the fill angle
		clipPriv::setvct(0, 1, clipAngle, vector0);
		// Since ClipRect.bottom is always 0
		BorderClipReference.y = ClipRect.top / 2;
		// Use 0 to align left edge of clip with beginning of line, ClipRect.right / 2 if you want to align
		// the center of the clip with the beginning of the line
		BorderClipReference.x = 0;
		// BorderClipReference.x = ClipRect.right / 2;
		size_t currentSide;
		for (currentSide = 0; currentSide < VertexCount - 2; currentSide++) {
			clipPriv::linsid(clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide);
			clipPriv::setvct(currentSide + 1, currentSide + 2, clipAngle, vector0);
			clipPriv::lincrnr(clipReversedData, clipFillData, clipAngle, moveToCoords, rotationCenter, currentSide);
		}
		clipPriv::linsid(clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide);
	}
	else {
		clpout();
		auto reference     = startVertex;
		auto currentVertex = startVertex;
		for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
			const auto nextVertex = prv(currentVertex);
			if (clipPriv::clpsid(clipReversedData, clipFillData, reference, nextVertex, rotationCenter)) {
				reference = nextVertex;
			}
			currentVertex = nextVertex;
		}
	}
}

bool clipPriv::fxpnt(const std::vector<double>& listSINEs,
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
			if (fabs(delta) < 0.2)
				break;
		}
		return 1;
	}
	return 0;
}

void clipPriv::fxlit(const std::vector<double>& listSINEs,
           const std::vector<double>& listCOSINEs,
           dPOINT&                    moveToCoords,
           unsigned                   currentSide) {
	if (clipPriv::fxpnt(listSINEs, listCOSINEs, moveToCoords, currentSide)) {
		SelectedPoint = moveToCoords;
		BeanCount++;
		const auto length
		    = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		const unsigned int count = floor(length / AdjustedSpace);
		dPOINT             delta = { AdjustedSpace * listCOSINEs[currentSide], AdjustedSpace * listSINEs[currentSide] };
		SelectedPoint.x += delta.x * count;
		SelectedPoint.y += delta.y * count;
		BeanCount += count;
	}
}

void clipPriv::fxlin(std::vector<fPOINT>&       chainEndPoints,
           const std::vector<double>& ListSINEs,
           const std::vector<double>& ListCOSINEs,
           dPOINT&                    moveToCoords,
           unsigned                   currentSide) {
	if (clipPriv::fxpnt(ListSINEs, ListCOSINEs, moveToCoords, currentSide)) {
		SelectedPoint = moveToCoords;
		chainEndPoints.push_back(SelectedPoint);
		const auto length
		    = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		const size_t count = floor(length / AdjustedSpace);
		const dPOINT delta = { AdjustedSpace * ListCOSINEs[currentSide], AdjustedSpace * ListSINEs[currentSide] };
		for (size_t iChain = 0; iChain < count; iChain++) {
			SelectedPoint.x += delta.x;
			SelectedPoint.y += delta.y;
			chainEndPoints.push_back(SelectedPoint);
		}
	}
}

void clipPriv::fxlen(std::vector<fPOINT>& chainEndPoints, const std::vector<double>& listSINEs, const std::vector<double>& listCOSINEs) {
	dPOINT moveToCoords = {}; // moving formOrigin for clipboard fill

	AdjustedSpace = 0;
	bool flag     = true;
	for (size_t iVertex = 1; iVertex < VertexCount; iVertex++) {
		const auto length = hypot(CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x,
		                          CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y);
		if (length > SelectedForm->edgeSpacing) {
			flag = false;
			break;
		}
		else {
			if (length > AdjustedSpace) {
				AdjustedSpace = length;
			}
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
	size_t     loopCount       = 0;
	size_t     initialCount    = 0;
	auto       smallestSpacing = 0.0;
	auto       largestSpacing  = 1.0;
	// loop at least 50 times to guarantee convergence
	while (loopCount < 50 && (largestSpacing - smallestSpacing) > TINY) {
		BeanCount     = 0;
		SelectedPoint = CurrentFormVertices[0];
		unsigned currentSide;
		for (currentSide = 0; currentSide < VertexCount - 1; currentSide++) {
			NextStart = currentSide + 1;
			clipPriv::fxlit(listSINEs, listCOSINEs, moveToCoords, currentSide);
		}
		if (SelectedForm->type != FRMLINE) {
			NextStart = 0;
			clipPriv::fxlit(listSINEs, listCOSINEs, moveToCoords, currentSide);
		}
		else
			NextStart = VertexCount - 1;
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
			if (interval > halfSpacing)
				interval = SelectedForm->edgeSpacing - interval;
			if (interval < minimumInterval) {
				minimumInterval = interval;
				minimumSpacing  = AdjustedSpace;
			}
			if (initialCount == BeanCount)
				smallestSpacing = AdjustedSpace;
			else {
				if (BeanCount > initialCount)
					largestSpacing = AdjustedSpace;
				else
					smallestSpacing = AdjustedSpace;
			}
			AdjustedSpace = smallestSpacing + (largestSpacing - smallestSpacing) / 2;
		}
		loopCount++;
	}
	SelectedPoint = CurrentFormVertices[0];
	OutputIndex   = 1;
	AdjustedSpace = minimumSpacing;
	chainEndPoints.push_back(SelectedPoint);
	unsigned currentSide;
	for (currentSide = 0; currentSide < VertexCount - 1; currentSide++) {
		NextStart = currentSide + 1;
		clipPriv::fxlin(chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide);
	}
	if (SelectedForm->type != FRMLINE) {
		NextStart = 0;
		clipPriv::fxlin(chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide);
	}
	interval = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
	if (interval > halfSpacing) {
		chainEndPoints.push_back(CurrentFormVertices[NextStart]);
	}
	else {
		chainEndPoints.back() = CurrentFormVertices[NextStart];
	}
}

void clipPriv::dufxlen(std::vector<fPOINT>& chainEndPoints) {
	duangs();
	std::vector<double> listSINEs;
	listSINEs.reserve(VertexCount + 1);
	std::vector<double> listCOSINEs;
	listCOSINEs.reserve(VertexCount);
	for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
		listSINEs.push_back(sin((*FormAngles)[iVertex]));
		listCOSINEs.push_back(cos((*FormAngles)[iVertex]));
	}
	listSINEs.push_back(sin(((*FormAngles)[0] > (*FormAngles)[VertexCount - 1])
	                            ? ((*FormAngles)[0] - (*FormAngles)[VertexCount - 1])
	                            : ((*FormAngles)[VertexCount - 1] - (*FormAngles)[0])));
	clipPriv::fxlen(chainEndPoints, listSINEs, listCOSINEs);
}

void clipPriv::dulast(std::vector<fPOINT>& chainEndPoints) {
	std::vector<fPOINT> tempClipPoints;
	tempClipPoints.reserve(chainEndPoints.size());
	if (lastch()) {
		auto   minimumLength = 1e99;
		size_t minimumIndex  = 0;
		for (size_t iPoint = 0; iPoint < chainEndPoints.size() - 1; iPoint++) {
			const auto length = hypot(LastPoint.x - chainEndPoints[iPoint].x, LastPoint.y - chainEndPoints[iPoint].y);
			if (length < minimumLength) {
				minimumLength = length;
				minimumIndex  = iPoint;
			}
		}
		if (minimumIndex) {
			for (size_t iPoint = minimumIndex; iPoint < chainEndPoints.size() - 2; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			for (size_t iPoint = 0; iPoint <= minimumIndex; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			chainEndPoints = tempClipPoints;
		}
	}
}

void clipPriv::clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints) {
	clipPriv::dulast(chainEndPoints);
	if (SelectedForm->type == FRMLINE) {
		const auto pivot = ClipRectSize.cy / 2;
		for (size_t iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.push_back({ ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y + pivot) });
		}
	}
	else {
		for (size_t iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.push_back({ ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y) });
		}
	}
}

void clipPriv::xclpfn(const std::vector<fPOINT>& tempClipPoints,
            const std::vector<fPOINT>& chainEndPoints,
            unsigned                   start,
            unsigned                   finish,
            const dPOINT&              rotationCenter) {
	std::vector<fPOINT> points(ClipStitchCount);

	const dPOINT delta
	    = { (chainEndPoints[finish].x - chainEndPoints[start].x), (chainEndPoints[finish].y - chainEndPoints[start].y) };
	const double rotationAngle = atan2(delta.y, delta.x);
	for (size_t iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
		points[iPoint] = tempClipPoints[iPoint];
		rotflt(points[iPoint], rotationAngle, rotationCenter);
		OSequence[SequenceIndex].x   = chainEndPoints[start].x + points[iPoint].x;
		OSequence[SequenceIndex++].y = chainEndPoints[start].y + points[iPoint].y;
	}
}

void clip::duxclp() {
	std::vector<fPOINT> chainEndPoints;
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	clipPriv::dufxlen(chainEndPoints);
	std::vector<fPOINT> tempClipPoints;
	tempClipPoints.reserve(ClipStitchCount);
	clipPriv::clpxadj(tempClipPoints, chainEndPoints);
	SequenceIndex         = 0;
	dPOINT rotationCenter = {};
	for (unsigned iPoint = 1; iPoint < chainEndPoints.size(); iPoint++) {
		clipPriv::xclpfn(tempClipPoints, chainEndPoints, iPoint - 1, iPoint, rotationCenter);
	}
	if (SelectedForm->type != FRMLINE) {
		OSequence[SequenceIndex++] = chainEndPoints[0];
	}
}

void clipPriv::clpcrnr(std::vector<fPOINT>& clipFillData, size_t vertex, const dPOINT& rotationCenter) {
	const size_t nextVertex = nxt(vertex);
	dPOINT       delta      = {};

	if (StateMap.test(StateFlag::INDIR)) {
		delta = { (*OutsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x,
			      (*OutsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y };
	}
	else {
		delta = { (*InsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x,
			      (*InsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y };
	}
	const double     rotationAngle  = atan2(delta.y, delta.x) + PI / 2;
	const fPOINTATTR referencePoint = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ClipRect.top };
	rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
	for (size_t iStitch = 0; iStitch < ClipStitchCount; iStitch++)
		rotang1(ClipBuffer[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
	const auto length = hypot(delta.x, delta.y);
	const auto ratio  = getplen() / length;
	delta.x *= ratio;
	delta.y *= ratio;
	const fPOINT point         = { CurrentFormVertices[nextVertex].x + delta.x, CurrentFormVertices[nextVertex].y + delta.y };
	OSequence[SequenceIndex++] = CurrentFormVertices[nextVertex];
	OSequence[SequenceIndex++] = point;
	OSequence[SequenceIndex++] = CurrentFormVertices[nextVertex];
	OSequence[SequenceIndex++] = point;
	if (!clipPriv::ritclp(clipFillData, point)) {
		OSequence[SequenceIndex++] = point;
		OSequence[SequenceIndex++] = CurrentFormVertices[nextVertex];
	}
}

void clipPriv::picfn(std::vector<fPOINT>& clipFillData, size_t start, size_t finish, double spacing, const dPOINT& rotationCenter) {
	const dPOINT     delta          = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
                           (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	const double     length         = hypot(delta.x, delta.y);
	const fPOINTATTR referencePoint = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ClipRect.top };

	double       rotationAngle = atan2(-delta.x, delta.y);
	const dPOINT outerStep     = { SelectedForm->borderSize * cos(rotationAngle), SelectedForm->borderSize * sin(rotationAngle) };
	spacing += ClipRectSize.cx;
	const unsigned int count = length / spacing;
	rotationAngle            = atan2(delta.y, delta.x);
	rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
	if (count) {
		dPOINT step = {};
		if (count > 1) {
			const auto tdub = ((length - count * spacing) / (count - 1) + spacing) / length;
			step            = { delta.x * tdub, delta.y * tdub };
		}
		for (size_t iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotang1(ClipBuffer[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
		bool   flag       = true;
		fPOINT innerPoint = { CurrentFormVertices[start].x, CurrentFormVertices[start].y };
		for (size_t iStep = 0; iStep < count - 1; iStep++) {
			const fPOINT firstPoint    = { innerPoint.x + step.x, innerPoint.y + step.y };
			const fPOINT outerPoint    = { firstPoint.x + outerStep.x, firstPoint.y + outerStep.y };
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = innerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = outerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = outerPoint;
			if (clipPriv::ritclp(clipFillData, outerPoint)) {
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
	std::vector<fPOINT> clipFillData(ClipStitchCount);

	const dPOINT rotationCenter
	    = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ((ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom) };

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength  = ClipRectSize.cx;
	ClipReference.y   = rotationCenter.y;
	ClipReference.x   = ClipRect.left;
	satout(20);
	if (SelectedForm->type == FRMLINE) {
		for (size_t iVertex = 0; iVertex < VertexCount - 2; iVertex++) {
			clipPriv::picfn(clipFillData, iVertex, iVertex + 1, SelectedForm->edgeSpacing, rotationCenter);
			clipPriv::clpcrnr(clipFillData, iVertex, rotationCenter);
		}
		clipPriv::picfn(clipFillData, VertexCount - 1, VertexCount, SelectedForm->edgeSpacing, rotationCenter);
	}
	else {
		if (!SelectedForm->fillType) {
			OSequence[SequenceIndex++] = CurrentFormVertices[0];
		}
		size_t currentVertex = 0;
		for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
			const auto nextVertex = nxt(currentVertex);
			clipPriv::picfn(clipFillData, currentVertex, nextVertex, SelectedForm->edgeSpacing, rotationCenter);
			clipPriv::clpcrnr(clipFillData, currentVertex, rotationCenter);
			currentVertex = nextVertex;
		}
		OSequence[SequenceIndex++] = CurrentFormVertices[currentVertex];
	}
}

void clipPriv::duchfn(const std::vector<fPOINT>& chainEndPoints, size_t start, size_t finish) {
	const std::vector<unsigned> chainSequence = { 0, 1, 2, 3, 0, 1, 4, 3, 0, 3 }; // chain stitch sequence

	std::vector<fPOINT> chainPoint(5);
	dPOINT delta = { (chainEndPoints[finish].x - chainEndPoints[start].x), (chainEndPoints[finish].y - chainEndPoints[start].y) };
	const dPOINT lengthDelta  = { (delta.x * SelectedForm->edgeStitchLen), (delta.y * SelectedForm->edgeStitchLen) };
	const double angle        = atan2(delta.y, delta.x) + PI / 2;
	const dPOINT offset       = { (cos(angle) * SelectedForm->borderSize), (sin(angle) * SelectedForm->borderSize) };
	const float  middleXcoord = chainEndPoints[start].x + lengthDelta.x;
	const float  middleYcoord = chainEndPoints[start].y + lengthDelta.y;

	chainPoint[0]   = chainEndPoints[start];
	chainPoint[4]   = chainEndPoints[finish];
	chainPoint[1].x = middleXcoord + offset.x;
	chainPoint[1].y = middleYcoord + offset.y;
	chainPoint[3].x = middleXcoord - offset.x;
	chainPoint[3].y = middleYcoord - offset.y;
	if (finish < chainEndPoints.size() - 1) {
		delta.x = chainEndPoints[finish + 1].x - chainEndPoints[finish].x;
		delta.y = chainEndPoints[finish + 1].y - chainEndPoints[finish].y;
	}
	else {
		delta.x = chainEndPoints[finish].x - chainEndPoints[finish - 1].x;
		delta.y = chainEndPoints[finish].y - chainEndPoints[finish - 1].y;
	}
	chainPoint[2].x   = chainEndPoints[finish].x + delta.x / 4;
	chainPoint[2].y   = chainEndPoints[finish].y + delta.y / 4;
	size_t chainCount = chainSequence.size();
	if (StateMap.test(StateFlag::LINCHN))
		chainCount--;
	for (size_t iChain = 0; iChain < chainCount; iChain++) {
		OSequence[SequenceIndex] = chainPoint[chainSequence[iChain]];
		SequenceIndex++;
	}
}

void clipPriv::duch(std::vector<fPOINT>& chainEndPoints) {
	SequenceIndex          = 0;
	const auto chainLength = chainEndPoints.size() - 1;
	if (chainLength) {
		for (size_t iPoint = 0; iPoint < chainLength - 1; iPoint++) {
			clipPriv::duchfn(chainEndPoints, iPoint, iPoint + 1);
		}
		if (SelectedForm->type == FRMLINE) {
			clipPriv::duchfn(chainEndPoints, chainLength - 1, chainLength);
			size_t backupAt = 8;
			if (StateMap.test(StateFlag::LINCHN))
				backupAt--;
			if ((SequenceIndex >= backupAt)) {
				OSequence[SequenceIndex - backupAt] = chainEndPoints[chainLength];
			}
			OSequence[SequenceIndex++] = chainEndPoints[chainLength];
		}
		else {
			clipPriv::duchfn(chainEndPoints, chainLength - 1, 0);
			OSequence[SequenceIndex++] = chainEndPoints[chainLength];
		}
	}
	else
		tabmsg(IDS_CHANSMAL);
}

void clip::chnfn() {
	std::vector<fPOINT> chainEndPoints;
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	fvars(ClosestFormToCursor);
	clip::deleclp(ClosestFormToCursor);
	clipPriv::dufxlen(chainEndPoints);
	clipPriv::dulast(chainEndPoints);
	SequenceIndex = 0;
	clipPriv::duch(chainEndPoints);
}
