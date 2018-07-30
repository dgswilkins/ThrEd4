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
//#include "formForms.h"
#include "hlp.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"

fPOINT ClipReference;       // clipboard reference formOrigin
fPOINT BorderClipReference; // reference for clipboard line border
double AdjustedSpace;       // adjusted space
size_t NextStart;           // index of the endpoint of the line segment being processed

bool iseclp(size_t iForm) noexcept {
	if (FormList[iForm].edgeType == EDGECLIP || FormList[iForm].edgeType == EDGEPICOT || FormList[iForm].edgeType == EDGECLIPX)
		return 1;
	return 0;
}

bool isclp(size_t iForm) noexcept {
	if ((1 << FormList[iForm].fillType) & ClipTypeMap)
		return 1;
	return 0;
}

bool isclpx(size_t iForm) noexcept {
	if (isclp(iForm) && FormList[iForm].lengthOrCount.clipCount)
		return 1;
	return 0;
}

bool iseclpx(size_t iForm) {
	if (iseclp(iForm) && FormList[iForm].clipEntries)
		return 1;
	return 0;
}

size_t findclp(size_t formIndex) {
	size_t iForm;

	for (iForm = formIndex; iForm != 0; iForm--) {
		if (iseclp(iForm - 1))
			return FormList[iForm - 1].borderClipData - ClipPoints + FormList[iForm - 1].clipEntries;
		if (isclp(iForm - 1))
			return FormList[iForm - 1].angleOrClipData.clip - ClipPoints + FormList[iForm - 1].lengthOrCount.clipCount;
	}
	return 0;
}

void clpsub(size_t fpnt, size_t cnt) {
	size_t iForm;

	for (iForm = fpnt + 1; iForm < FormIndex; iForm++) {
		if (isclpx(iForm))
			FormList[iForm].angleOrClipData.clip -= cnt;
		if (iseclpx(fpnt))
			FormList[iForm].borderClipData -= cnt;
	}
}

void delmclp(size_t iForm) {
	size_t source = 0, destination = 0;

	if (ClipPointIndex) {
		if (isclp(iForm)) {
			destination = findclp(iForm);
			source      = destination + FormList[iForm].lengthOrCount.clipCount;
			std::copy(ClipPoints + source,
			          ClipPoints + ClipPointIndex,
			          stdext::make_checked_array_iterator((ClipPoints + destination), (MAXITEMS - destination)));
			if (iseclp(iForm))
				FormList[iForm].borderClipData -= FormList[iForm].lengthOrCount.clipCount;
			clpsub(iForm, FormList[iForm].lengthOrCount.clipCount);
			if (ClipPointIndex > FormList[iForm].lengthOrCount.clipCount)
				ClipPointIndex -= FormList[iForm].lengthOrCount.clipCount;
			else
				ClipPointIndex = 0;
			FormList[iForm].lengthOrCount.clipCount = 0;
		}
	}
}

void deleclp(size_t iForm) {
	size_t source = 0, destination = 0;

	if (ClipPointIndex) {
		if (iseclpx(iForm)) {
			destination = findclp(iForm);
			source      = destination + FormList[iForm].clipEntries;
			while (source < ClipPointIndex) {
				ClipPoints[destination++] = ClipPoints[source++];
			}
			clpsub(iForm, FormList[iForm].clipEntries);
			if (ClipPointIndex > FormList[iForm].clipEntries)
				ClipPointIndex -= FormList[iForm].clipEntries;
			else
				ClipPointIndex = 0;
			FormList[iForm].clipEntries = 0;
		}
	}
}

void delclps(size_t iForm) {
	deleclp(iForm);
	delmclp(iForm);
}

fPOINT* nueclp(size_t currentForm, size_t count) {
	size_t find        = findclp(ClosestFormToCursor);
	size_t source      = ClipPointIndex - 1;
	size_t destination = ClipPointIndex + count - 1;

	if (isclp(ClosestFormToCursor))
		find += FormList[ClosestFormToCursor].lengthOrCount.clipCount;
	while (source >= find) {
		ClipPoints[destination--] = ClipPoints[source--];
	}
	for (auto iform = currentForm; iform < FormIndex; iform++) {
		if (iseclpx(iform))
			FormList[iform].borderClipData += count;
	}
	for (auto iform = currentForm + 1; iform < FormIndex; iform++) {
		if (isclp(iform))
			FormList[iform].angleOrClipData.clip += count;
	}
	ClipPointIndex += count;
	return &ClipPoints[find];
}

fPOINT* numclp() {
	size_t find        = findclp(ClosestFormToCursor);
	size_t source      = ClipPointIndex - 1;
	size_t destination = ClipPointIndex + ClipStitchCount - 1;
	size_t iForm       = ClosestFormToCursor + 1;

	while ((source + 1) > find) {
		ClipPoints[destination--] = ClipPoints[source--];
	}
	FormList[ClosestFormToCursor].angleOrClipData.clip = &ClipPoints[find];
	if (iseclpx(ClosestFormToCursor))
		FormList[ClosestFormToCursor].borderClipData += ClipStitchCount;
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (isclpx(iForm))
			FormList[iForm].angleOrClipData.clip += ClipStitchCount;
		if (iseclpx(iForm))
			FormList[iForm].borderClipData += ClipStitchCount;
	}
	ClipPointIndex += ClipStitchCount;
	return &ClipPoints[find];
}

void oclp(const fPOINT* const clip, size_t clipEntries) {
	if (clip) {
		unsigned iClip = 1;

		if (!StateMap.test(StateFlag::NOCLP)) {
			for (iClip = 0; iClip < clipEntries; iClip++) {
				ClipBuffer[iClip].x = clip[iClip].x;
				ClipBuffer[iClip].y = clip[iClip].y;
			}
			ClipRect.left = ClipRect.right = ClipBuffer[0].x;
			ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
			for (iClip = 1; iClip < clipEntries; iClip++) {
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

void durev(std::vector<fPOINT>& clipReversedData) {
	unsigned     iStitch  = 0;
	const double midpoint = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;

	if (ClipBuffer[0].x > midpoint) {
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch].x = ClipRect.right - ClipBuffer[iStitch].x;
			clipReversedData[iStitch].y = ClipBuffer[iStitch].y;
		}
	}
	else {
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch] = ClipBuffer[iStitch];
		}
	}
}

void setvct(unsigned start, unsigned finish, double& ClipAngle, dPOINT& vector0) noexcept {
	ClipAngle = atan2(CurrentFormVertices[finish].y - CurrentFormVertices[start].y,
	                  CurrentFormVertices[finish].x - CurrentFormVertices[start].x);
	vector0.x = ClipRectSize.cx * cos(ClipAngle);
	vector0.y = ClipRectSize.cx * sin(ClipAngle);
}

bool nupnt(double clipAngle, dPOINT& moveToCoords, unsigned currentSide) noexcept {
	double       length = 0.0, delta = 0.0;
	unsigned     step     = 0;
	const double sinAngle = sin(clipAngle);
	const double cosAngle = cos(clipAngle);

	// ToDo - should we use a temp variable and assign to moveToCoords before return?
	moveToCoords = CurrentFormVertices[currentSide + 2];
	length       = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
	if (length > ClipRectSize.cx) {
		for (step = 0; step < 10; step++) {
			length = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
			delta  = ClipRectSize.cx - length;
			moveToCoords.x += delta * cosAngle;
			moveToCoords.y += delta * sinAngle;
			if (fabs(delta) < 0.01)
				break;
		}
		return 1;
	}
	return 0;
}

bool ritclp(const std::vector<fPOINT>& clipFillData, const fPOINT& point) {
	fPOINT   adjustedPoint = { (point.x - ClipReference.x), (point.y - ClipReference.y) };
	unsigned iStitch       = 0;

	if (chkmax(ClipStitchCount, SequenceIndex))
		return 1;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		OSequence[SequenceIndex].x   = clipFillData[iStitch].x + adjustedPoint.x;
		OSequence[SequenceIndex++].y = clipFillData[iStitch].y + adjustedPoint.y;
	}
	return 0;
}

void lincrnr(const std::vector<fPOINT>& clipReversedData,
             std::vector<fPOINT>&       clipFillData,
             double                     clipAngle,
             dPOINT&                    moveToCoords,
             const dPOINT&              rotationCenter,
             unsigned                   currentSide) {
	dPOINT   delta   = {};
	unsigned iStitch = 0;

	if (nupnt(clipAngle, moveToCoords, currentSide)) {
		delta.x                    = moveToCoords.x - SelectedPoint.x;
		delta.y                    = moveToCoords.y - SelectedPoint.y;
		const double rotationAngle = atan2(delta.y, delta.x);
		rotangf(BorderClipReference, ClipReference, rotationAngle, rotationCenter);
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotangf(clipReversedData[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
		ritclp(clipFillData, SelectedPoint);
		SelectedPoint = moveToCoords;
	}
}

void linsid(const std::vector<fPOINT>& clipReversedData,
            std::vector<fPOINT>&       clipFillData,
            double                     clipAngle,
            const dPOINT&              vector0,
            const dPOINT&              rotationCenter,
            unsigned int               currentSide) {
	fPOINT         delta     = { (CurrentFormVertices[currentSide + 1].x - SelectedPoint.x),
                     (CurrentFormVertices[currentSide + 1].y - SelectedPoint.y) };
	const double   length    = hypot(delta.x, delta.y);
	const unsigned clipCount = length / ClipRectSize.cx;
	unsigned       iStitch = 0, iClip = 0;

	if (clipCount) {
		rotangf(BorderClipReference, ClipReference, clipAngle, rotationCenter);
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotangf(clipReversedData[iStitch], clipFillData[iStitch], clipAngle, rotationCenter);
		for (iClip = 0; iClip < clipCount; iClip++) {
			ritclp(clipFillData, SelectedPoint);
			SelectedPoint.x += vector0.x;
			SelectedPoint.y += vector0.y;
		}
	}
}

void clpout() {
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

bool clpsid(const std::vector<fPOINT>& clipReversedData,
            std::vector<fPOINT>&       clipFillData,
            size_t                     start,
            size_t                     finish,
            const dPOINT&              rotationCenter) {
	unsigned     ind = 0, clipCount = 0;
	fPOINT       delta = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
                     (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	fPOINT       step = {}, insertPoint = {};
	const double length             = hypot(delta.x, delta.y);
	double       tdub               = 0.0;
	fPOINTATTR   clipReferencePoint = { ClipRect.left, ClipRect.bottom };
	double       rotationAngle      = atan2(delta.y, delta.x);

	rotang1(clipReferencePoint, ClipReference, rotationAngle, rotationCenter);
	clipCount = length / ClipRectSize.cx;
	if (clipCount) {
		if (clipCount > 1)
			tdub = ((length - clipCount * ClipRectSize.cx) / (clipCount - 1) + ClipRectSize.cx) / length;
		else
			tdub = (length - ClipRectSize.cx) / 2;
		step.x        = delta.x * tdub;
		step.y        = delta.y * tdub;
		insertPoint   = CurrentFormVertices[start];
		rotationAngle = atan2(delta.y, delta.x);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(clipReversedData[ind], clipFillData[ind], rotationAngle, rotationCenter);
		for (ind = 0; ind < clipCount; ind++) {
			if (ritclp(clipFillData, insertPoint))
				break;
			insertPoint.x += step.x;
			insertPoint.y += step.y;
		}
		return 1;
	}
	return 0;
}

void clpbrd(size_t startVertex) {
	size_t reference     = 0;
	size_t currentVertex = 0, nextVertex = 0;
	double clipAngle      = 0;  // for clipboard border fill
	dPOINT vector0        = {}; // x size of the clipboard fill at the fill angle
	dPOINT moveToCoords   = {}; // moving formOrigin for clipboard fill
	dPOINT rotationCenter = {};

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength  = ClipRectSize.cx;
	std::vector<fPOINT> clipFillData(ClipStitchCount);
	std::vector<fPOINT> clipReversedData(ClipStitchCount);
	rotationCenter.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	ClipReference.y = rotationCenter.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
	ClipReference.x                    = ClipRect.left;
	durev(clipReversedData);
	if (SelectedForm->type == FRMLINE) {
		SelectedPoint = CurrentFormVertices[0];
		setvct(0, 1, clipAngle, vector0);
		// Since ClipRect.bottom is always 0
		BorderClipReference.y = ClipRect.top / 2;
		// Use 0 to align left edge of clip with beginning of line, ClipRect.right / 2 if you want to align
		// the center of the clip with the beginning of the line
		BorderClipReference.x = 0;
		// BorderClipReference.x = ClipRect.right / 2;
		unsigned currentSide;
		for (currentSide = 0; currentSide < VertexCount - 2; currentSide++) {
			linsid(clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide);
			setvct(currentSide + 1, currentSide + 2, clipAngle, vector0);
			lincrnr(clipReversedData, clipFillData, clipAngle, moveToCoords, rotationCenter, currentSide);
		}
		linsid(clipReversedData, clipFillData, clipAngle, vector0, rotationCenter, currentSide);
	}
	else {
		clpout();
		reference = currentVertex = startVertex;
		for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
			nextVertex = prv(currentVertex);
			if (clpsid(clipReversedData, clipFillData, reference, nextVertex, rotationCenter))
				reference = nextVertex;
			currentVertex = nextVertex;
		}
	}
}

bool fxpnt(const std::vector<double>& listSINEs,
	const std::vector<double>& listCOSINEs,
	dPOINT&                    moveToCoords,
	unsigned                   currentSide) {
	double   length = 0.0, delta = 0.0;
	unsigned iGuess = 0;

	moveToCoords.x = CurrentFormVertices[NextStart].x;
	moveToCoords.y = CurrentFormVertices[NextStart].y;
	length = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
	if (length > AdjustedSpace) {
		for (iGuess = 0; iGuess < 10; iGuess++) {
			length = hypot(moveToCoords.x - SelectedPoint.x, moveToCoords.y - SelectedPoint.y);
			delta = AdjustedSpace - length;
			moveToCoords.x += delta * listCOSINEs[currentSide];
			moveToCoords.y += delta * listSINEs[currentSide];
			if (fabs(delta) < 0.2)
				break;
		}
		return 1;
	}
	return 0;
}

void fxlit(const std::vector<double>& listSINEs,
           const std::vector<double>& listCOSINEs,
           dPOINT&                    moveToCoords,
           unsigned                   currentSide) {
	double   length = 0.0;
	unsigned count  = 0;
	dPOINT   delta  = {};

	if (fxpnt(listSINEs, listCOSINEs, moveToCoords, currentSide)) {
		SelectedPoint.x = moveToCoords.x;
		SelectedPoint.y = moveToCoords.y;
		BeanCount++;
		length  = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		count   = floor(length / AdjustedSpace);
		delta.x = AdjustedSpace * listCOSINEs[currentSide];
		delta.y = AdjustedSpace * listSINEs[currentSide];
		SelectedPoint.x += delta.x * count;
		SelectedPoint.y += delta.y * count;
		BeanCount += count;
	}
}

void fxlin(std::vector<fPOINT>&       chainEndPoints,
           const std::vector<double>& ListSINEs,
           const std::vector<double>& ListCOSINEs,
           dPOINT&                    moveToCoords,
           unsigned                   currentSide) {
	double   length = 0.0;
	unsigned count  = 0;
	unsigned iChain = 0;
	dPOINT   delta  = {};

	if (fxpnt(ListSINEs, ListCOSINEs, moveToCoords, currentSide)) {
		SelectedPoint.x = moveToCoords.x;
		SelectedPoint.y = moveToCoords.y;
		chainEndPoints.push_back(SelectedPoint);
		length  = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		count   = floor(length / AdjustedSpace);
		delta.x = AdjustedSpace * ListCOSINEs[currentSide];
		delta.y = AdjustedSpace * ListSINEs[currentSide];
		for (iChain = 0; iChain < count; iChain++) {
			SelectedPoint.x += delta.x;
			SelectedPoint.y += delta.y;
			chainEndPoints.push_back(SelectedPoint);
		}
	}
}

void fxlen(std::vector<fPOINT>&       chainEndPoints,
           const std::vector<double>& listSINEs,
           const std::vector<double>& listCOSINEs) {
	double   interval        = 0.0;
	double   minimumInterval = 0.0;
	double   minimumSpacing  = 0.0;
	double   largestSpacing  = 0.0;
	double   smallestSpacing = 0.0;
	double   halfSpacing     = 0.0;
	unsigned initialCount    = 0;
	unsigned loopCount       = 0;
	unsigned iVertex         = 0;
	double   length          = 0.0;
	dPOINT   moveToCoords    = {}; // moving formOrigin for clipboard fill

	AdjustedSpace = 0;
	bool flag     = true;
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		length = hypot(CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x,
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
	AdjustedSpace = minimumSpacing = SelectedForm->edgeSpacing;
	halfSpacing                    = AdjustedSpace / 2;
	interval = minimumInterval = 1e9;
	loopCount = initialCount = 0;
	smallestSpacing          = 0;
	largestSpacing           = 1;
	// loop at least 50 times to guarantee convergence
	while (loopCount < 50 && (largestSpacing - smallestSpacing) > TINY) {
		BeanCount     = 0;
		SelectedPoint = CurrentFormVertices[0];
		unsigned currentSide;
		for (currentSide = 0; currentSide < VertexCount - 1; currentSide++) {
			NextStart = currentSide + 1;
			fxlit(listSINEs, listCOSINEs, moveToCoords, currentSide);
		}
		if (SelectedForm->type != FRMLINE) {
			NextStart = 0;
			fxlit(listSINEs, listCOSINEs, moveToCoords, currentSide);
		}
		else
			NextStart = VertexCount - 1;
		if (!initialCount) {
			initialCount    = BeanCount;
			smallestSpacing = AdjustedSpace;
			minimumInterval = interval = static_cast<double>(
			    hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y));
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
		fxlin(chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide);
	}
	if (SelectedForm->type != FRMLINE) {
		NextStart = 0;
		fxlin(chainEndPoints, listSINEs, listCOSINEs, moveToCoords, currentSide);
	}
	interval = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
	if (interval > halfSpacing) {
		chainEndPoints.push_back(CurrentFormVertices[NextStart]);
	}
	else {
		chainEndPoints.back() = CurrentFormVertices[NextStart];
	}
}

void dufxlen(std::vector<fPOINT>& chainEndPoints) {
	unsigned iVertex = 0;

	duangs();
	std::vector<double> listSINEs;
	listSINEs.reserve(VertexCount + 1);
	std::vector<double> listCOSINEs;
	listCOSINEs.reserve(VertexCount);
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		listSINEs.push_back(sin((*FormAngles)[iVertex]));
		listCOSINEs.push_back(cos((*FormAngles)[iVertex]));
	}
	listSINEs.push_back(sin(((*FormAngles)[0] > (*FormAngles)[VertexCount - 1])
	                            ? ((*FormAngles)[0] - (*FormAngles)[VertexCount - 1])
	                            : ((*FormAngles)[VertexCount - 1] - (*FormAngles)[0])));
	fxlen(chainEndPoints, listSINEs, listCOSINEs);
}

void dulast(std::vector<fPOINT>& chainEndPoints) {
	unsigned iPoint = 0, iDestination = 0, minimumIndex = 0;
	double   minimumLength = 0.0;
	double   length        = 0.0;

	std::vector<fPOINT> tempClipPoints;
	tempClipPoints.reserve(chainEndPoints.size());
	if (lastch()) {
		minimumLength = 1e99;
		minimumIndex  = 0;
		for (iPoint = 0; iPoint < chainEndPoints.size() - 1; iPoint++) {
			length = hypot(LastPoint.x - chainEndPoints[iPoint].x, LastPoint.y - chainEndPoints[iPoint].y);
			if (length < minimumLength) {
				minimumLength = length;
				minimumIndex  = iPoint;
			}
		}
		if (minimumIndex) {
			iDestination = 0;
			for (iPoint = minimumIndex; iPoint < chainEndPoints.size() - 2; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			for (iPoint = 0; iPoint <= minimumIndex; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			chainEndPoints = tempClipPoints;
		}
	}
}

void clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints) {
	unsigned iPoint = 0;
	float    pivot  = 0.0;

	dulast(chainEndPoints);
	if (SelectedForm->type == FRMLINE) {
		pivot = ClipRectSize.cy / 2;
		for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.push_back({ ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y + pivot) });
		}
	}
	else {
		for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.push_back({ ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y) });
		}
	}
}

void xclpfn(const std::vector<fPOINT>& tempClipPoints,
            const std::vector<fPOINT>& chainEndPoints,
            unsigned                   start,
            unsigned                   finish,
            const dPOINT&              rotationCenter) {
	dPOINT delta = { (chainEndPoints[finish].x - chainEndPoints[start].x), (chainEndPoints[finish].y - chainEndPoints[start].y) };
	unsigned            iPoint = 0;
	const double        length = hypot(delta.x, delta.y);
	const double        ratio  = length / ClipRectSize.cx;
	std::vector<fPOINT> points(ClipStitchCount);

	const double rotationAngle = atan2(delta.y, delta.x);
	for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
		points[iPoint] = tempClipPoints[iPoint];
		rotflt(points[iPoint], rotationAngle, rotationCenter);
		OSequence[SequenceIndex].x   = chainEndPoints[start].x + points[iPoint].x;
		OSequence[SequenceIndex++].y = chainEndPoints[start].y + points[iPoint].y;
	}
}

void duxclp() {
	std::vector<fPOINT> chainEndPoints;
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	dufxlen(chainEndPoints);
	std::vector<fPOINT> tempClipPoints;
	tempClipPoints.reserve(ClipStitchCount);
	clpxadj(tempClipPoints, chainEndPoints);
	SequenceIndex         = 0;
	dPOINT rotationCenter = {};
	for (unsigned iPoint = 1; iPoint < chainEndPoints.size(); iPoint++) {
		xclpfn(tempClipPoints, chainEndPoints, iPoint - 1, iPoint, rotationCenter);
	}
	if (SelectedForm->type != FRMLINE) {
		OSequence[SequenceIndex++] = chainEndPoints[0];
	}
}

void clpcrnr(std::vector<fPOINT>& clipFillData, size_t vertex, const dPOINT& rotationCenter) {
	const size_t nextVertex = nxt(vertex);
	unsigned     iStitch    = 0;
	dPOINT       delta = {}, point = {};
	fPOINT       tpnt   = {};
	double       length = 0.0, ratio = 0.0;
	fPOINTATTR   referencePoint = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ClipRect.top };

	if (StateMap.test(StateFlag::INDIR)) {
		delta.x = (*OutsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = (*OutsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	else {
		delta.x = (*InsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = (*InsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	const double rotationAngle = atan2(delta.y, delta.x) + PI / 2;
	rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
		rotang1(ClipBuffer[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
	length = hypot(delta.x, delta.y);
	ratio  = getplen() / length;
	delta.x *= ratio;
	delta.y *= ratio;
	point.x                      = CurrentFormVertices[nextVertex].x + delta.x;
	point.y                      = CurrentFormVertices[nextVertex].y + delta.y;
	OSequence[SequenceIndex++]   = CurrentFormVertices[nextVertex];
	OSequence[SequenceIndex].x   = point.x;
	OSequence[SequenceIndex++].y = point.y;
	OSequence[SequenceIndex++]   = CurrentFormVertices[nextVertex];
	OSequence[SequenceIndex].x   = point.x;
	OSequence[SequenceIndex++].y = point.y;
	tpnt.x                       = point.x;
	tpnt.y                       = point.y;
	if (!ritclp(clipFillData, tpnt)) {
		OSequence[SequenceIndex].x   = point.x;
		OSequence[SequenceIndex++].y = point.y;
		OSequence[SequenceIndex++]   = CurrentFormVertices[nextVertex];
	}
}

void picfn(std::vector<fPOINT>& clipFillData, size_t start, size_t finish, double spacing, const dPOINT& rotationCenter) {
	dPOINT       delta          = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
                     (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	const double length         = hypot(delta.x, delta.y);
	dPOINT       innerPoint     = { CurrentFormVertices[start].x, CurrentFormVertices[start].y };
	fPOINTATTR   referencePoint = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ClipRect.top };
	double       tdub           = 0.0;
	dPOINT       step = {}, outerStep = {}, firstPoint = {}, outerPoint = {};
	fPOINT       tpnt  = {};
	unsigned     count = 0, iStitch = 0, iStep = 0;

	double rotationAngle = atan2(-delta.x, delta.y);
	outerStep.x          = SelectedForm->borderSize * cos(rotationAngle);
	outerStep.y          = SelectedForm->borderSize * sin(rotationAngle);
	spacing += ClipRectSize.cx;
	count         = length / spacing;
	rotationAngle = atan2(delta.y, delta.x);
	rotang1(referencePoint, ClipReference, rotationAngle, rotationCenter);
	if (count) {
		step.x = 0;
		step.y = 0;
		if (count > 1) {
			tdub   = ((length - count * spacing) / (count - 1) + spacing) / length;
			step.x = delta.x * tdub;
			step.y = delta.y * tdub;
		}
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotang1(ClipBuffer[iStitch], clipFillData[iStitch], rotationAngle, rotationCenter);
		bool flag = true;
		for (iStep = 0; iStep < count - 1; iStep++) {
			firstPoint.x                 = innerPoint.x + step.x;
			firstPoint.y                 = innerPoint.y + step.y;
			outerPoint.x                 = firstPoint.x + outerStep.x;
			outerPoint.y                 = firstPoint.y + outerStep.y;
			OSequence[SequenceIndex].x   = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x   = innerPoint.x;
			OSequence[SequenceIndex++].y = innerPoint.y;
			OSequence[SequenceIndex].x   = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x   = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			OSequence[SequenceIndex].x   = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x   = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			tpnt.x                       = outerPoint.x;
			tpnt.y                       = outerPoint.y;
			if (ritclp(clipFillData, tpnt)) {
				flag = false;
				break;
			}
			OSequence[SequenceIndex].x   = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			OSequence[SequenceIndex].x   = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			innerPoint.x += step.x;
			innerPoint.y += step.y;
		}
		if (flag) {
			OSequence[SequenceIndex++]   = CurrentFormVertices[finish];
			OSequence[SequenceIndex].x   = innerPoint.x;
			OSequence[SequenceIndex++].y = innerPoint.y;
		}
	}
}

void clpic() {
	const unsigned short start         = 0;
	unsigned             iVertex       = 0;
	size_t               currentVertex = start, nextVertex = 0;
	std::vector<fPOINT>  clipFillData(ClipStitchCount);

	dPOINT rotationCenter
	    = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left), ((ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom) };

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength  = ClipRectSize.cx;
	ClipReference.y   = rotationCenter.y;
	ClipReference.x   = ClipRect.left;
	satout(20);
	if (SelectedForm->type == FRMLINE) {
		for (iVertex = 0; iVertex < VertexCount - 2; iVertex++) {
			picfn(clipFillData, iVertex, iVertex + 1, SelectedForm->edgeSpacing, rotationCenter);
			clpcrnr(clipFillData, iVertex, rotationCenter);
		}
		picfn(clipFillData, iVertex, iVertex + 1, SelectedForm->edgeSpacing, rotationCenter);
	}
	else {
		if (!SelectedForm->fillType) {
			OSequence[SequenceIndex++] = CurrentFormVertices[start];
		}
		currentVertex = start;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			nextVertex = nxt(currentVertex);
			picfn(clipFillData, currentVertex, nextVertex, SelectedForm->edgeSpacing, rotationCenter);
			clpcrnr(clipFillData, currentVertex, rotationCenter);
			currentVertex = nextVertex;
		}
		OSequence[SequenceIndex++] = CurrentFormVertices[currentVertex];
	}
}

void duchfn(const std::vector<fPOINT>& chainEndPoints, unsigned start, unsigned finish) {
	unsigned              iChain        = 0;
	std::vector<unsigned> chainSequence = { 0, 1, 2, 3, 0, 1, 4, 3, 0, 3 }; // chain stitch sequence

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
	// ToDo - This may not be correct
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
	for (iChain = 0; iChain < chainCount; iChain++) {
		OSequence[SequenceIndex] = chainPoint[chainSequence[iChain]];
		SequenceIndex++;
	}
}

void duch(std::vector<fPOINT>& chainEndPoints) {
	unsigned iPoint = 0, backupAt = 0;

	SequenceIndex = 0;
	if (chainEndPoints.size()) {
		for (iPoint = 0; iPoint < chainEndPoints.size() - 2; iPoint++) {
			duchfn(chainEndPoints, iPoint, iPoint + 1);
		}
		if (SelectedForm->type == FRMLINE) {
			duchfn(chainEndPoints, iPoint, iPoint + 1);
			backupAt = 8;
			if (StateMap.test(StateFlag::LINCHN))
				backupAt--;
			if ((SequenceIndex >= backupAt)) {
				OSequence[SequenceIndex - backupAt] = chainEndPoints[iPoint + 1];
			}
			OSequence[SequenceIndex++] = chainEndPoints[iPoint + 1];
		}
		else {
			duchfn(chainEndPoints, iPoint, 0);
			OSequence[SequenceIndex++] = chainEndPoints[chainEndPoints.size() - 1];
		}
	}
	else
		tabmsg(IDS_CHANSMAL);
}

void chnfn() {
	std::vector<fPOINT> chainEndPoints;
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	dufxlen(chainEndPoints);
	dulast(chainEndPoints);
	SequenceIndex = 0;
	duch(chainEndPoints);
}
