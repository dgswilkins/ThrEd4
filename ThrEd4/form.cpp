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
#include "clip.h"
#include "displayText.h"
#include "formForms.h"
#include "hlp.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"

unsigned   BeanCount;                 // number of stitches added by convert to bean
fRECTANGLE BoundingRect;              // isin rectangle
size_t     ClipIntersectSide;         // clipboard intersect side;
float      ClipWidth;                 // horizontal spacing for vertical clipboard fill
fPOINT*    CurrentFillVertices;       // pointer to the line of the polygon being filled
size_t     CurrentFormGuidesCount;    // number of connections in the currently selected form
REGION*    CurrentRegion;             // region currently being sequenced
unsigned   DoneRegion;                // last region sequenced
double     EggRatio;                  // ratio for shrinking eggs
FRMHED*    FormForInsert;             // insert form vertex in this form
FORMINFO   FormInfo;                  // form info used in drawing forms
FLOAT      FormOffset;                // form offset for clipboard fills
unsigned   FormRelocationIndex;       // form relocator pointer
size_t     FormVertexNext;            // form vertex storage for form vertex insert
size_t     FormVertexPrev;            // form vertex storage for form vertex insert
double     GapToClosestRegion;        // region close enough threshold for sequencing
size_t     GroupIndexCount;           // number of group indices
unsigned   InOutFlag;                 // is intersection of line and cursor before, in or after the line
unsigned   LastGroup;                 // group of the last line written in the previous region;
double*    Lengths;                   // array of cumulative lengths used in satin fills
unsigned   LineGroupIndex;            // pointer for groups of fill line segments
fPOINT     LineSegmentEnd;            // vertical clipboard line segment end
fPOINT     LineSegmentStart;          // vertical clipboard line segment start
float      MaxStitchLen;              // maximum stitch length
unsigned   NextGroup;                 // group that connects to the next region
unsigned   PathIndex;                 // formOrigin to the next path element for vertical fill sequencing
unsigned   PathMapIndex;              // number of entries in the path map
unsigned   RegionCount;               // number of regions to be sequenced
RGSEQ*     RegionPath;                // path to a region
unsigned   SatinBackupIndex;          // pointer for backup stitches in satin fills
unsigned   SatinIndex;                // pointer to next satin formOrigin to enter
SMALPNTL*  SequenceLines;             // line for vertical/horizontal/angle fills
unsigned   SequencePathIndex;         // index to path of sequenced regions
double     Slope;                     // slope of line in angle fills
size_t     StartPoint;                // starting formOrigin for a satin stitch guide-line
unsigned   StitchLineCount;           // count of stitch lines
HDC        TimeDC;                    // progress bar device context
double     TimePosition;              // progress bar postiion
double     TimeStep;                  // progress bar step
HWND       TimeWindow;                // progress bar
float      UserStitchLen;             // user stitch length
unsigned   VisitedIndex;              // next unvisited region for sequencing
fPOINT*    WorkingFormVertices;       // form points for angle fills

unsigned char Level00   = 0;
unsigned char Level01   = 1;
unsigned char Level02[] = { 0, 1 };
unsigned char Level03[] = { 1, 0, 2 };
unsigned char Level04[] = { 1, 3, 0, 2 };
unsigned char Level05[] = { 2, 0, 3, 1, 4 };
unsigned char Level06[] = { 3, 0, 2, 4, 1, 5 };
unsigned char Level07[] = { 3, 0, 4, 1, 6, 2, 5 };
unsigned char Level08[] = { 4, 0, 5, 1, 3, 6, 2, 7 };
unsigned char Level09[] = { 4, 0, 5, 1, 6, 2, 7, 3, 8 };
unsigned char Level10[] = { 5, 0, 6, 1, 7, 2, 8, 3, 9, 4 };
unsigned char Level11[] = { 5, 0, 6, 1, 7, 2, 8, 3, 9, 10 };
unsigned char Level12[] = { 6, 0, 7, 1, 8, 2, 9, 3, 10, 4, 11 };
unsigned char Level13[] = { 6, 0, 1, 7, 2, 8, 3, 9, 4, 10, 5, 11, 6, 12 };
unsigned char Level14[] = { 7, 0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6 };
unsigned char Level15[] = { 7, 0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15 };

unsigned char* Levels[]
    = { &Level00,    &Level01,    &Level02[0], &Level03[0], &Level04[0], &Level05[0], &Level06[0], &Level07[0],
	    &Level08[0], &Level09[0], &Level10[0], &Level11[0], &Level12[0], &Level13[0], &Level14[0], &Level15[0] };

void frmclr(FRMHED* const destination) noexcept {
	*destination = {};
}

void duinf(const FRMHED& formHeader) noexcept {
	// Correct
	FormInfo.type      = formHeader.attribute & 0xf;
	FormInfo.attribute = (formHeader.attribute >> FRMSHFT) & 0xf;
	FormInfo.sideCount = formHeader.vertexCount;
}

bool comp(const dPOINTLINE& point1, const dPOINTLINE& point2) noexcept {
	if (point1.y < point2.y)
		return true;
	if (point1.x < point2.x)
		return true;
	return false;
}

void getfinfo(size_t iForm) noexcept {
	duinf(FormList[iForm]);
}

unsigned satind(const SATCON* const guide) noexcept {
	return guide - SatinGuides;
}

void dusqr() {
	if (UserFlagMap.test(UserFlag::SQRFIL))
		SelectedForm->extendedAttribute |= AT_SQR;
	else
		SelectedForm->extendedAttribute &= ~(AT_SQR);
}

void sacspac(const SATCON* const startGuide, unsigned guideCount) noexcept {
	size_t iStartGuide = 0;
	size_t source = 0, destination = 0;

	iStartGuide = satind(startGuide);
	source      = SatinGuideIndex - 1;
	destination = SatinGuideIndex + guideCount - 1;
	while (source >= iStartGuide) {
		SatinGuides[destination--] = SatinGuides[source--];
	}
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			FormList[iForm].satinOrAngle.guide += guideCount;
	}
	SatinGuideIndex += guideCount;
}

SATCON* nusac(size_t formIndex, unsigned guideCount) noexcept {
	size_t guideIndex = 0;

	for (size_t iForm = 0u; iForm < formIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			guideIndex += FormList[iForm].satinGuideCount;
	}
	FormList[formIndex].satinOrAngle.guide = &SatinGuides[guideIndex];
	sacspac(FormList[formIndex].satinOrAngle.guide, guideCount);
	return FormList[formIndex].satinOrAngle.guide;
}

bool chkmax(size_t arg0, size_t arg1) noexcept {
	if (arg0 & MAXMSK)
		return 1;
	if (arg1 & MAXMSK)
		return 1;
	if ((arg1 + arg0) & MAXMSK)
		return 1;
	return 0;
}

unsigned fltind(const fPOINT* const point) noexcept {
	return point - FormVertices;
}

unsigned clpind(const fPOINT* const point) noexcept {
	return point - ClipPoints;
}

void fltspac(const fPOINT* const start, size_t count) noexcept {
	const unsigned int startIndex  = fltind(start);
	size_t             source      = FormVertexIndex - 1;
	size_t             destination = FormVertexIndex + count - 1;
	size_t             iForm       = ClosestFormToCursor + 1;

	while (source >= startIndex) {
		FormVertices[destination--] = FormVertices[source--];
	}
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
		FormList[iForm].vertices += count;
	FormVertexIndex += count;
}

void delsac(size_t formIndex) noexcept {
	size_t iForm  = 0;
	size_t source = 0, destination = 0;

	if (SatinGuideIndex) {
		if (FormList[formIndex].type == SAT && FormList[formIndex].satinGuideCount) {
			destination = satind(FormList[formIndex].satinOrAngle.guide);
			source      = destination + FormList[formIndex].satinGuideCount;
			while (source < SatinGuideIndex) {
				SatinGuides[destination++] = SatinGuides[source++];
			}
			for (iForm = formIndex + 1; iForm < FormIndex; iForm++) {
				if (FormList[iForm].type == SAT && FormList[iForm].satinGuideCount)
					FormList[iForm].satinOrAngle.guide -= FormList[formIndex].satinGuideCount;
			}
			SatinGuideIndex -= FormList[formIndex].satinGuideCount;
		}
	}
	FormList[formIndex].satinGuideCount = 0;
}

void delflt(size_t formIndex) noexcept {
	size_t source = 0, destination = 0;

	if (FormList[formIndex].vertexCount) {
		destination = fltind(FormList[formIndex].vertices);
		source      = destination + FormList[formIndex].vertexCount;
		while (source < FormVertexIndex) {
			FormVertices[destination++] = FormVertices[source++];
		}
		for (auto iForm = formIndex + 1; iForm < FormIndex; iForm++)
			FormList[iForm].vertices -= FormList[formIndex].vertexCount;
		FormVertexIndex -= FormList[formIndex].vertexCount;
		if (FormVertexIndex & 0x8000000)
			FormVertexIndex = 0;
	}
}

bool chk2of() {
	if (!StateMap.test(StateFlag::SELBOX))
		return 0;
	if (UserFlagMap.test(UserFlag::FIL2OF))
		return 0;
	return 1;
}

void rotbak(double rotationAngle, const dPOINT& rotationCenter) {
	unsigned iSequence;

	for (iSequence = 0; iSequence < SequenceIndex; iSequence++)
		rotflt(OSequence[iSequence], rotationAngle, rotationCenter);
}

void delfil(unsigned attribute) {
	unsigned iSource = 0, iDestination = 0;

	attribute &= TYPMSK | FRMSK;
	StateMap.reset(StateFlag::WASDEL);
	while ((StitchBuffer[iSource].attribute & (TYPMSK | FRMSK)) != attribute && iSource < PCSHeader.stitchCount)
		iSource++;
	iDestination = iSource;
	if (iSource < PCSHeader.stitchCount) {
		StateMap.set(StateFlag::WASDEL);
	}
	while (iSource < PCSHeader.stitchCount) {
		if ((StitchBuffer[iSource].attribute & (TYPMSK | FRMSK)) != attribute) {
			StitchBuffer[iDestination++] = StitchBuffer[iSource];
		}
		iSource++;
	}
	PCSHeader.stitchCount = iDestination;
}

void delmfil() {
	const size_t attribute       = ClosestFormToCursor << FRMSHFT;
	unsigned     stitchAttribute = 0;
	unsigned     iSource = 0, iDestination = 0;

	if (istx(ClosestFormToCursor))
		deltx();
	delmclp(ClosestFormToCursor);
	for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
		stitchAttribute = StitchBuffer[iSource].attribute;
		if (!(stitchAttribute & NOTFRM)) {
			if (!((stitchAttribute & FRMSK) == attribute && (stitchAttribute & (TYPFRM | FTHMSK))))
				mvstch(iDestination++, iSource);
		}
		else
			mvstch(iDestination++, iSource);
	}
	PCSHeader.stitchCount = iDestination;
}

void fsizpar() noexcept {
	SelectedForm->maxFillStitchLen           = IniFile.maxStitchLength;
	SelectedForm->lengthOrCount.stitchLength = UserStitchLength;
	SelectedForm->minFillStitchLen           = MinStitchLength;
}

void chkcont() {
	unsigned shortestGuideIndex = 0;
	size_t   length = 0, minimumLength = 10000;

	delmclp(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	fsizpar();
	if (SelectedForm->fillType != CONTF) {
		if (CurrentFormGuidesCount) {
			for (auto iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
				length = SelectedForm->satinOrAngle.guide[iGuide].finish - SelectedForm->satinOrAngle.guide[iGuide].start;
				if (length < minimumLength) {
					minimumLength      = length;
					shortestGuideIndex = iGuide;
				}
			}
			SelectedForm->angleOrClipData.guide.start  = SelectedForm->satinOrAngle.guide[shortestGuideIndex].start;
			SelectedForm->angleOrClipData.guide.finish = SelectedForm->satinOrAngle.guide[shortestGuideIndex].finish;
			delsac(ClosestFormToCursor);
		}
		else {
			SelectedForm->angleOrClipData.guide.start  = 1;
			SelectedForm->angleOrClipData.guide.finish = SelectedForm->vertexCount - 2;
		}
	}
	SelectedForm->type     = FRMLINE;
	SelectedForm->fillType = CONTF;
	SelectedForm->attribute |= FRECONT;
}

unsigned find1st() {
	unsigned iStitch   = 0;
	size_t   attribute = 0;

	if (chk2of())
		return ClosestPointIndex;
	else {
		attribute = ClosestFormToCursor << 4;
		while (iStitch < PCSHeader.stitchCount) {
			if ((StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				if (!(StitchBuffer[iStitch].attribute & NOTFRM))
					return iStitch;
			}
			iStitch++;
		}
		return iStitch;
	}
}

void ispcdclp() {
	StateMap.reset(StateFlag::WASPCDCLP);
	if (OpenClipboard(ThrEdWindow)) {
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory)
			StateMap.set(StateFlag::WASPCDCLP);
	}
	CloseClipboard();
}

void frmout(size_t formIndex) noexcept {
	fRECTANGLE* rectangle = nullptr;
	unsigned    iVertex   = 0;
	float       offset    = 0.0;

	if (FormList[formIndex].vertexCount) {
		rectangle           = &FormList[formIndex].rectangle;
		CurrentFormVertices = FormList[formIndex].vertices;
		rectangle->left = rectangle->right = CurrentFormVertices[0].x;
		rectangle->bottom = rectangle->top = CurrentFormVertices[0].y;
		for (iVertex = 1; iVertex < FormList[formIndex].vertexCount; iVertex++) {
			if (CurrentFormVertices[iVertex].x > rectangle->right)
				rectangle->right = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].x < rectangle->left)
				rectangle->left = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].y < rectangle->bottom)
				rectangle->bottom = CurrentFormVertices[iVertex].y;
			if (CurrentFormVertices[iVertex].y > rectangle->top)
				rectangle->top = CurrentFormVertices[iVertex].y;
		}
		if (rectangle->top - rectangle->bottom < MINRCT) {
			offset = (MINRCT - (rectangle->top - rectangle->bottom)) / 2;
			rectangle->top += offset;
			rectangle->bottom -= offset;
		}
		if (rectangle->right - rectangle->left < MINRCT) {
			offset = (MINRCT - (rectangle->right - rectangle->left)) / 2;
			rectangle->left -= offset;
			rectangle->right += offset;
		}
	}
}

void sfCor2px(const fPOINT& stitchPoint, POINT& screen) noexcept {
	screen.x = (stitchPoint.x - ZoomRect.left) * ZoomRatio.x + 0.5;
	screen.y = StitchWindowClientRect.bottom - (stitchPoint.y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
}

void px2stchf(const POINT& screen, fPOINT& stitchPoint) noexcept {
	double factorX = 0.0, factorY = 0.0;

	factorX       = static_cast<double>(screen.x) / static_cast<double>(StitchWindowClientRect.right);
	stitchPoint.x = factorX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	factorY       = static_cast<double>(StitchWindowClientRect.bottom - screen.y) / StitchWindowClientRect.bottom;
	stitchPoint.y = factorY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

void frmlin(const fPOINT* const vertices, size_t vertexCount) noexcept {
	if (vertices) {
		unsigned iVertex = 0;

		if (VertexCount) {
			for (iVertex = 0; iVertex < vertexCount; iVertex++) {
				FormLines[iVertex].x = (vertices[iVertex].x - ZoomRect.left) * ZoomRatio.x;
				FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[iVertex].y - ZoomRect.bottom) * ZoomRatio.y;
			}
			FormLines[iVertex].x = (vertices[0].x - ZoomRect.left) * ZoomRatio.x;
			FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[0].y - ZoomRect.bottom) * ZoomRatio.y;
		}
	}
}

void frmlin(const std::vector<fPOINT>& vertices) {
	unsigned     iVertex   = 0;
	const size_t vertexMax = vertices.size();

	if (vertexMax) {
		for (iVertex = 0; iVertex < vertexMax; iVertex++) {
			FormLines[iVertex].x = (vertices[iVertex].x - ZoomRect.left) * ZoomRatio.x;
			FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[iVertex].y - ZoomRect.bottom) * ZoomRatio.y;
		}
		FormLines[iVertex].x = (vertices[0].x - ZoomRect.left) * ZoomRatio.x;
		FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[0].y - ZoomRect.bottom) * ZoomRatio.y;
	}
}

void dufrm() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	PolylineInt(StitchWindowDC, FormLines, NewFormVertexCount);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unfrm() {
	if (StateMap.testAndReset(StateFlag::SHOFRM))
		dufrm();
}

void mdufrm() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if (FormList[ClosestFormToCursor].type == FRMLINE)
		PolylineInt(StitchWindowDC, FormLines, NewFormVertexCount - 1);
	else
		PolylineInt(StitchWindowDC, FormLines, NewFormVertexCount);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void munfrm() {
	if (StateMap.testAndReset(StateFlag::SHOFRM))
		mdufrm();
}

void rats() {
	if (StateMap.test(StateFlag::ZUMED)) {
		HorizontalRatio = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
		VerticalRatio   = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
	}
	else {
		HorizontalRatio = static_cast<double>(UnzoomedRect.x) / StitchWindowClientRect.right;
		VerticalRatio   = static_cast<double>(UnzoomedRect.y) / StitchWindowClientRect.bottom;
	}
}

void fvars(size_t iForm) noexcept {
	SelectedForm           = &FormList[iForm];
	CurrentFormVertices    = FormList[iForm].vertices;
	VertexCount            = FormList[iForm].vertexCount;
	CurrentFormGuides      = FormList[iForm].satinOrAngle.guide;
	CurrentFormGuidesCount = FormList[iForm].satinGuideCount;
	SatinEndGuide          = FormList[iForm].wordParam;
}

void setfrm() {
	unsigned iVertex = 0;
	fPOINT   point   = {};
	fPOINT   delta   = {};

	rats();
	ClosestFormToCursor = FormIndex;
	fvars(ClosestFormToCursor);
	px2stchf(FormLines[0], point);
	delta.x                      = point.x - CurrentFormVertices[0].x;
	delta.y                      = point.y - CurrentFormVertices[0].y;
	SelectedForm->rectangle.left = SelectedForm->rectangle.bottom = static_cast<float>(1e30);
	SelectedForm->rectangle.right = SelectedForm->rectangle.top = 0;
	for (iVertex = 0; iVertex < NewFormVertexCount - 1; iVertex++) {
		CurrentFormVertices[iVertex].x += delta.x;
		CurrentFormVertices[iVertex].y += delta.y;
		if (CurrentFormVertices[iVertex].x < SelectedForm->rectangle.left)
			SelectedForm->rectangle.left = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].x > SelectedForm->rectangle.right)
			SelectedForm->rectangle.right = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].y > SelectedForm->rectangle.top)
			SelectedForm->rectangle.top = CurrentFormVertices[iVertex].y;
		if (CurrentFormVertices[iVertex].y < SelectedForm->rectangle.bottom)
			SelectedForm->rectangle.bottom = CurrentFormVertices[iVertex].y;
	}
	FormIndex++;
	StateMap.reset(StateFlag::FORMIN);
	StateMap.set(StateFlag::INIT);
	StateMap.set(StateFlag::RESTCH);
}

void form() {
	shoMsg((*StringTable)[STR_FMEN]);
	StateMap.set(StateFlag::FORMIN);
	StateMap.reset(StateFlag::INSRT);
	duzrat();
}

void frmsqr(unsigned iVertex) {
	double angle         = 0.0;
	double ratio         = 0.0;
	float  length        = 0.0;
	fPOINT offset        = {};
	fPOINT point         = {};
	fPOINT adjustedPoint = {};
	dPOINT delta         = {};
	POINT  line[4]       = {};

	stch2pxr(CurrentFormVertices[iVertex]);
	line[1]  = StitchCoordinatesPixels;
	ratio    = static_cast<double>(IniFile.formVertexSizePixels) / StitchWindowClientRect.right;
	length   = (ZoomRect.right - ZoomRect.left) * ratio * 2;
	delta.x  = CurrentFormVertices[iVertex - 1].x - CurrentFormVertices[iVertex].x;
	delta.y  = CurrentFormVertices[iVertex - 1].y - CurrentFormVertices[iVertex].y;
	angle    = atan2(delta.y, delta.x);
	offset.x = length * cos(angle);
	offset.y = length * sin(angle);
	point.x  = CurrentFormVertices[iVertex].x + offset.x;
	point.y  = CurrentFormVertices[iVertex].y + offset.y;
	angle    = atan2(-delta.x, delta.y);
	length /= 2;
	offset.x        = length * cos(angle);
	offset.y        = length * sin(angle);
	adjustedPoint.x = point.x + offset.x;
	adjustedPoint.y = point.y + offset.y;
	stch2pxr(adjustedPoint);
	line[0] = line[3] = StitchCoordinatesPixels;
	adjustedPoint.x   = point.x - offset.x;
	adjustedPoint.y   = point.y - offset.y;
	stch2pxr(adjustedPoint);
	line[2] = StitchCoordinatesPixels;
	Polyline(StitchWindowMemDC, line, 4);
}

void selsqr(const POINT& controlPoint, HDC dc) noexcept {
	POINT line[5];

	line[0].x = line[3].x = line[4].x = controlPoint.x - IniFile.formVertexSizePixels;
	line[0].y = line[1].y = controlPoint.y - IniFile.formVertexSizePixels;
	line[1].x = line[2].x = controlPoint.x + IniFile.formVertexSizePixels;
	line[2].y = line[3].y = controlPoint.y + IniFile.formVertexSizePixels;
	line[4].y             = controlPoint.y - IniFile.formVertexSizePixels;
	Polyline(dc, line, 5);
}

void frmsqr0(const POINT& controlPoint) noexcept {
	POINT          line[5] = {};
	const unsigned offset  = IniFile.formBoxSizePixels;

	if (offset) {
		line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
		line[0].y = line[1].y = controlPoint.y - offset;
		line[1].x = line[2].x = controlPoint.x + offset + 1;
		line[2].y = line[3].y = controlPoint.y + offset + 1;
		line[4].y             = controlPoint.y - 1;
		Polyline(StitchWindowMemDC, line, 5);
	}
}

void frmx(const POINT& controlPoint, HDC dc) noexcept {
	POINT line[2];

	SelectObject(dc, FormSelectedPen);
	line[0].x = line[1].x = controlPoint.x;
	line[0].y             = controlPoint.y + 8;
	line[1].y             = controlPoint.y - 8;
	Polyline(dc, line, 2);
	line[0].y = line[1].y = controlPoint.y;
	line[0].x             = controlPoint.x - 8;
	line[1].x             = controlPoint.x + 8;
	Polyline(dc, line, 2);
	SelectObject(dc, FormPen);
}

void ratsr() {
	if (StateMap.test(StateFlag::ZUMED)) {
		HorizontalRatio = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
		VerticalRatio   = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
	}
	else {
		HorizontalRatio = static_cast<double>(StitchWindowClientRect.right) / UnzoomedRect.x;
		VerticalRatio   = static_cast<double>(StitchWindowClientRect.bottom) / UnzoomedRect.y;
	}
}

constexpr float midl(float high, float low) {
	return (high - low) / 2 + low;
}

void ritfrct(size_t iForm, HDC dc) {
	unsigned    controlPoint     = 0;
	POINT       pixelOutline[10] = {};
	fRECTANGLE* rectangle        = nullptr;
	fPOINT      formOutline[10]  = {};

	ratsr();
	SelectObject(StitchWindowDC, FormPen);
	SetROP2(StitchWindowDC, R2_XORPEN);
	getfinfo(iForm);
	rectangle = &FormList[iForm].rectangle;
	SelectObject(dc, FormSelectedPen);
	formOutline[0].x = formOutline[6].x = formOutline[7].x = formOutline[8].x = rectangle->left;
	formOutline[1].x = formOutline[5].x = midl(rectangle->right, rectangle->left);
	formOutline[0].y = formOutline[1].y = formOutline[2].y = formOutline[8].y = rectangle->top;
	formOutline[3].y = formOutline[7].y = midl(rectangle->top, rectangle->bottom);
	formOutline[4].y = formOutline[5].y = formOutline[6].y = rectangle->bottom;
	formOutline[2].x = formOutline[3].x = formOutline[4].x = rectangle->right;
	for (controlPoint = 0; controlPoint < 8; controlPoint++)
		sfCor2px(formOutline[controlPoint], pixelOutline[controlPoint]);
	sfCor2px(formOutline[0], pixelOutline[controlPoint]);
	Polyline(dc, pixelOutline, 9);
	for (controlPoint = 0; controlPoint < 8; controlPoint++)
		selsqr(pixelOutline[controlPoint], dc);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		StateMap.reset(StateFlag::SELSHO);
		SearchLineIndex = 0;
		StateMap.set(StateFlag::RESTCH);
	}
}

void delfrms() {
	unsigned iStitch = 0;

	savdo();
	FormIndex = FormVertexIndex = SatinGuideIndex = ClipPointIndex = 0;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].attribute &= NFRM_NTYP;
		StitchBuffer[iStitch].attribute |= NOTFRM;
	}
}

void fselrct(size_t iForm) noexcept {
	const FRMHED& formHeader     = FormList[iForm];
	fPOINT        formOutline[5] = {};
	POINT         line[6]        = {};
	unsigned      iPoint         = 0;

	formOutline[0].x = formOutline[3].x = formOutline[4].x = formHeader.rectangle.left;
	formOutline[1].x = formOutline[2].x = formHeader.rectangle.right;
	formOutline[0].y = formOutline[1].y = formOutline[4].y = formHeader.rectangle.top;
	formOutline[2].y = formOutline[3].y = formHeader.rectangle.bottom;
	for (iPoint = 0; iPoint < 5; iPoint++) {
		line[iPoint].x = (formOutline[iPoint].x - ZoomRect.left) * HorizontalRatio;
		line[iPoint].y = (ZoomRect.top - formOutline[iPoint].y) * VerticalRatio;
		if (line[iPoint].x < SelectedFormsRect.left)
			SelectedFormsRect.left = line[iPoint].x;
		if (line[iPoint].x > SelectedFormsRect.right)
			SelectedFormsRect.right = line[iPoint].x;
		if (line[iPoint].y < SelectedFormsRect.top)
			SelectedFormsRect.top = line[iPoint].y;
		if (line[iPoint].y > SelectedFormsRect.bottom)
			SelectedFormsRect.bottom = line[iPoint].y;
	}
	line[5].x = (formOutline[0].x - ZoomRect.left) * HorizontalRatio;
	line[5].y = (ZoomRect.top - formOutline[0].y) * VerticalRatio;
	if (line[5].x < SelectedFormsRect.left)
		SelectedFormsRect.left = line[5].x;
	if (line[5].x > SelectedFormsRect.right)
		SelectedFormsRect.right = line[5].x;
	if (line[5].y < SelectedFormsRect.top)
		SelectedFormsRect.top = line[5].y;
	if (line[5].y > SelectedFormsRect.bottom)
		SelectedFormsRect.bottom = line[5].y;
	Polyline(StitchWindowMemDC, line, 5);
}

void rct2sel(const RECT& rectangle, std::vector<POINT>& line) {
	line[0].x = line[6].x = line[7].x = line[8].x = rectangle.left;
	line[1].x = line[5].x = ((rectangle.right - rectangle.left) >> 1) + rectangle.left;
	line[2].x = line[3].x = line[4].x = rectangle.right;
	line[0].y = line[1].y = line[2].y = line[8].y = rectangle.top;
	line[3].y = line[7].y = ((rectangle.bottom - rectangle.top) >> 1) + rectangle.top;
	line[4].y = line[5].y = line[6].y = rectangle.bottom;
}

void dubig() {
	unsigned iPoint;

	rct2sel(SelectedFormsRect, *SelectedFormsLine);
	SelectObject(StitchWindowMemDC, SelectAllPen);
	Polyline(StitchWindowMemDC, SelectedFormsLine->data(), 9);
	for (iPoint = 0; iPoint < 8; iPoint++)
		selsqr((*SelectedFormsLine)[iPoint], StitchWindowMemDC);
}

void frmpoly(const POINT* const line, size_t count) noexcept {
	if (line) {
		unsigned iPoint = 0;

		if (count) {
			for (iPoint = 0; iPoint < count - 1; iPoint++)
				Polyline(StitchWindowMemDC, &line[iPoint], 2);
		}
	}
}

void dupsel(HDC dc) {
	size_t iPoint;

	SelectObject(dc, FormPen);
	SetROP2(dc, R2_XORPEN);
	Polyline(dc, SelectedPointsLine->data(), 9);
	iPoint = SelectedFormVertices.start;
	for (iPoint = 0; iPoint < 8; iPoint++)
		selsqr((*SelectedPointsLine)[iPoint], dc);
	frmx(EndPointCross, dc);
}

void unpsel() {
	if (StateMap.testAndReset(StateFlag::SHOPSEL))
		dupsel(StitchWindowDC);
}

void sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect) noexcept {
	screenRect.left   = (stitchRect.left - ZoomRect.left) * ZoomRatio.x + 0.5;
	screenRect.right  = (stitchRect.right - ZoomRect.left) * ZoomRatio.x + 0.5;
	screenRect.top    = (StitchWindowClientRect.bottom) - (stitchRect.top - ZoomRect.bottom) * ZoomRatio.y + 0.5;
	screenRect.bottom = (StitchWindowClientRect.bottom) - (stitchRect.bottom - ZoomRect.bottom) * ZoomRatio.y + 0.5;
}

void drwfrm() {
	unsigned iForm = 0, iGuide = 0, iVertex = 0, layer = 0;
	size_t   lastPoint = 0;
	POINT    line[2]   = {};
	dPOINT   point     = {};

	StateMap.reset(StateFlag::SHOMOV);
	StateMap.reset(StateFlag::SHOPSEL);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	ratsr();
	duzrat();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		fvars(iForm);
		frmlin(SelectedForm->vertices, VertexCount);
		lastPoint = 0;
		layer     = ((SelectedForm->attribute & FRMLMSK) >> 1);
		if (!ActiveLayer || !layer || layer == ActiveLayer) {
			if (SelectedForm->type == SAT) {
				if (SelectedForm->attribute & FRMEND) {
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, FormLines, 2);
					lastPoint = 1;
				}
				if (SelectedForm->wordParam) {
					SelectObject(StitchWindowMemDC, FormPen);
					frmpoly(&FormLines[1], SelectedForm->wordParam);
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, &FormLines[SelectedForm->wordParam], 2);
					SelectObject(StitchWindowMemDC, LayerPen[layer]);
					lastPoint = SelectedForm->wordParam + 1;
				}
				for (iGuide = 0; iGuide < FormList[iForm].satinGuideCount; iGuide++) {
					sfCor2px(CurrentFormVertices[CurrentFormGuides[iGuide].start], line[0]);
					sfCor2px(CurrentFormVertices[CurrentFormGuides[iGuide].finish], line[1]);
					SelectObject(StitchWindowMemDC, FormPen);
					Polyline(StitchWindowMemDC, line, 2);
				}
			}
			SelectObject(StitchWindowMemDC, LayerPen[layer]);
			if (SelectedForm->type == FRMLINE) {
				frmpoly(FormLines, VertexCount);
				if (SelectedForm->fillType == CONTF) {
					point = CurrentFormVertices[SelectedForm->angleOrClipData.guide.start];
					sCor2px(point, line[0]);
					point = CurrentFormVertices[SelectedForm->angleOrClipData.guide.finish];
					sCor2px(point, line[1]);
					Polyline(StitchWindowMemDC, line, 2);
				}
			}
			else
				frmpoly(&FormLines[lastPoint], VertexCount + 1 - lastPoint);
			if (ClosestFormToCursor == iForm && StateMap.test(StateFlag::FRMPSEL)) {
				for (iVertex = 1; iVertex < VertexCount; iVertex++) {
					if (iVertex == ClosestVertexToCursor)
						frmx(FormLines[iVertex], StitchWindowMemDC);
					else
						frmsqr(iVertex);
				}
				if (ClosestVertexToCursor)
					frmsqr0(FormLines[0]);
				else
					frmx(FormLines[0], StitchWindowMemDC);
				ritnum(STR_NUMPNT, ClosestVertexToCursor);
			}
			else {
				for (iVertex = 1; iVertex < VertexCount; iVertex++)
					frmsqr(iVertex);
				SelectObject(StitchWindowMemDC, FormSelectedPen);
				frmsqr0(FormLines[0]);
			}
			if (StateMap.test(StateFlag::FPSEL) && ClosestFormToCursor == iForm) {
				sRct2px(SelectedVerticesRect, SelectedPixelsRect);
				rct2sel(SelectedPixelsRect, *SelectedPointsLine);
				StateMap.set(StateFlag::SHOPSEL);
				dupsel(StitchWindowMemDC);
			}
		}
	}
	if (SelectedFormList->size()) {
		SelectObject(StitchWindowMemDC, MultiFormPen);
		ratsr();
		SelectedFormsRect.top = SelectedFormsRect.left = 0x7fffffff;
		SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
		for (auto selectedForm : (*SelectedFormList)) {
			fselrct(selectedForm);
		}
		SelectedFormsSize.x = SelectedFormsRect.right - SelectedFormsRect.left;
		SelectedFormsSize.y = SelectedFormsRect.bottom - SelectedFormsRect.top;
		dubig();
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL))
			ritfrct(ClosestFormToCursor, StitchWindowMemDC);
		if (StateMap.test(StateFlag::FRMPMOV)) {
			ritmov();
			(*RubberBandLine)[1].x = Msg.pt.x - StitchWindowOrigin.x;
			(*RubberBandLine)[1].y = Msg.pt.y - StitchWindowOrigin.y;
			StateMap.set(StateFlag::SHOMOV);
			ritmov();
		}
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void setpoli() {
	frmclr(&FormList[FormIndex]);
	FormList[FormIndex].type           = FRMFPOLY;
	FormList[FormIndex].attribute      = ActiveLayer << 1;
	FormList[FormIndex].underlayIndent = IniFile.underlayIndent;
	StateMap.set(StateFlag::SATIN);
	StateMap.set(StateFlag::INIT);
}

void setlin() {
	frmclr(&FormList[FormIndex]);
	FormList[FormIndex].type      = FRMLINE;
	FormList[FormIndex].attribute = ActiveLayer << 1;
	StateMap.set(StateFlag::SATIN);
	StateMap.set(StateFlag::INIT);
}

void setrpoli() {
	unmsg();
	pntmsg(IDS_REGP);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRPOL);
	numWnd();
}

void setstar() {
	unmsg();
	pntmsg(IDS_STAR);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRSTAR);
	numWnd();
}

void setspir() {
	unmsg();
	pntmsg(IDS_SPIR);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRSPIR);
	numWnd();
}

void sethart() {
	unmsg();
	pntmsg(IDS_HEART);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRHART);
	numWnd();
}

void setlens() {
	unmsg();
	pntmsg(IDS_LENS);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRLENS);
	numWnd();
}

void seteg() {
	unmsg();
	pntmsg(IDS_EGG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTREG);
	numWnd();
}

void setzig() {
	unmsg();
	pntmsg(IDS_ZIG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRZIG);
	numWnd();
}

void setmfrm() noexcept {
	unsigned iForm  = 0;
	POINT    point  = {};
	POINT    offset = {};

	sfCor2px(FormList[ClosestFormToCursor].vertices[0], point);
	offset.x = Msg.pt.x - StitchWindowOrigin.x - point.x + FormMoveDelta.x;
	offset.y = Msg.pt.y - StitchWindowOrigin.y - point.y + FormMoveDelta.y;
	for (iForm = 0; iForm < FormList[ClosestFormToCursor].vertexCount; iForm++) {
		sfCor2px(FormList[ClosestFormToCursor].vertices[iForm], point);
		FormLines[iForm].x = point.x + offset.x;
		FormLines[iForm].y = point.y + offset.y;
	}
	FormLines[iForm] = FormLines[0];
}

void durpoli(unsigned vertexCount) {
	double   stepAngle = 0.0;
	double   angle     = 0.0;
	double   length    = 0.0;
	unsigned iVertex   = 0;
	dPOINT   point     = {};

	if (vertexCount < 3)
		vertexCount = 3;
	if (vertexCount > 100)
		vertexCount = 100;
	stepAngle = PI * 2 / vertexCount;
	// ToDo - why 500?
	length              = 500 / vertexCount * ZoomFactor * (UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm        = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices    = adflt(vertexCount);
	SelectedForm->vertexCount = vertexCount;
	SelectedForm->attribute   = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x = point.x;
		CurrentFormVertices[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	SelectedForm->type  = FRMFPOLY;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount                = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

constexpr size_t nxt(size_t iVertex) {
	iVertex++;
	if (iVertex > VertexCount - 1)
		iVertex = 0;
	return iVertex;
}

constexpr size_t prv(size_t iVertex) {
	if (iVertex)
		iVertex--;
	else
		iVertex = VertexCount - 1;
	return iVertex;
}

size_t pdir(size_t vertex) {
	if (StateMap.test(StateFlag::PSELDIR))
		return nxt(vertex);
	else
		return prv(vertex);
}

void pxrct2stch(const RECT& screenRect, fRECTANGLE& stitchRect) {
	POINT corner;

	corner.x = screenRect.left + StitchWindowOrigin.x;
	corner.y = screenRect.top + StitchWindowOrigin.y;
	pxCor2stch(corner);
	stitchRect.left = SelectedPoint.x;
	stitchRect.top  = SelectedPoint.y;
	corner.x        = screenRect.right + StitchWindowOrigin.x;
	corner.y        = screenRect.bottom + StitchWindowOrigin.y;
	pxCor2stch(corner);
	stitchRect.right  = SelectedPoint.x;
	stitchRect.bottom = SelectedPoint.y;
}

void flipv() {
	unsigned   iForm = 0, iStitch = 0, iVertex = 0, decodedForm = 0;
	size_t     currentVertex = 0;
	float      midpoint      = 0;
	fRECTANGLE rectangle     = {};

	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		savdo();
		midpoint      = (SelectedVerticesRect.top - SelectedVerticesRect.bottom) * 0.5 + SelectedVerticesRect.bottom;
		currentVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			CurrentFormVertices[currentVertex].y = midpoint + midpoint - CurrentFormVertices[currentVertex].y;
			currentVertex                        = pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		savdo();
		midpoint = (AllItemsRect.top - AllItemsRect.bottom) / 2 + AllItemsRect.bottom;
		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
			FormVertices[iVertex].y = midpoint + midpoint - FormVertices[iVertex].y;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			FormList[iForm].rectangle.bottom = midpoint + midpoint - FormList[iForm].rectangle.bottom;
			FormList[iForm].rectangle.top    = midpoint + midpoint - FormList[iForm].rectangle.top;
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedFormList->size()) {
		savdo();
		boost::dynamic_bitset<> formMap(FormIndex);
		pxrct2stch(SelectedFormsRect, rectangle);
		midpoint = (rectangle.top - rectangle.bottom) / 2 + rectangle.bottom;
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			formMap.set(ClosestFormToCursor);
			fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++)
				CurrentFormVertices[iVertex].y = midpoint + midpoint - CurrentFormVertices[iVertex].y;
			frmout(ClosestFormToCursor);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			decodedForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && !(StitchBuffer[iStitch].attribute & NOTFRM))
				StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			midpoint = (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom) / 2 + SelectedForm->rectangle.bottom;
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
				CurrentFormVertices[iVertex].y = midpoint + midpoint - CurrentFormVertices[iVertex].y;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor
				    && !(StitchBuffer[iStitch].attribute & NOTFRM))
					StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
			}
			frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				selRct(rectangle);
				midpoint = (rectangle.top - rectangle.bottom) / 2 + rectangle.bottom;
				for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
					StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void duform(unsigned formType) {
	frmclr(&FormList[FormIndex]);
	switch (formType + 1) {
	case FRMLINE:
		setlin();
		break;
	case FRMFPOLY:
		setpoli();
		break;
	case FRMRPOLY:
		setrpoli();
		break;
	case FRMSTAR:
		setstar();
		break;
	case FRMSPIRAL:
		setspir();
		break;
	case FRMHEART - 1:
		sethart();
		break;
	case FRMLENS - 1:
		setlens();
		break;
	case FRMEGG - 1:
		seteg();
		break;
	case FRMTEAR - 1:
		setear();
		break;
	case FRMZIGZAG - 1:
		setzig();
		break;
	case FRMWAVE - 1:
		wavfrm();
		break;
	case FRMDAISY - 1:
		dasyfrm();
		break;
	}
}

float findDistanceToSide(const fPOINT& lineStart, const fPOINT& lineEnd, const fPOINT& point, double& distance) noexcept {
	const double A     = point.x - lineStart.x;
	const double B     = point.y - lineStart.y;
	const double C     = lineEnd.x - lineStart.x;
	const double D     = lineEnd.y - lineStart.y;
	double       diffX = 0.0;
	double       diffY = 0.0;

	if ((C == 0) && (D == 0)) {
		diffX    = A;
		diffY    = B;
		distance = sqrt(diffX * diffX + diffY * diffY);
		// Arbitrarily choose the first point since start and end are the same
		return -0.1f;
	}

	const double dot    = A * C + B * D;
	const double len_sq = C * C + D * D;
	const float  param  = dot / len_sq;

	if (param < 0) {
		// point is nearest to the first point i.e lineStart.x and lineStart.y
		diffX = point.x - lineStart.x;
		diffY = point.y - lineStart.y;
	}
	else if (param > 1) {
		// point is nearest to the end point i.e lineEnd.x and lineEnd.y
		diffX = point.x - lineEnd.x;
		diffY = point.y - lineEnd.y;
	}
	else {
		// if perpendicular line intersect the line segment.
		diffX = point.x - (lineStart.x + param * C);
		diffY = point.y - (lineStart.y + param * D);
	}

	// returning shortest distance
	distance = sqrt(diffX * diffX + diffY * diffY);
	return param;
}

unsigned closfrm() {
	unsigned iForm = 0, iVertex = 0, layerCoded = 0, formLayer = 0;
	size_t   closestVertex = 0, closestForm = 0;
	fPOINT   point            = {};
	POINT    screenCoordinate = {};
	double   length           = 0;
	double   minimumLength    = 1e99;
	float    param            = 0.0;

	if (FormIndex) {
		screenCoordinate.x = Msg.pt.x - StitchWindowOrigin.x;
		screenCoordinate.y = Msg.pt.y - StitchWindowOrigin.y;
		rats();
		closestForm = closestVertex = 0;
		px2stchf(screenCoordinate, point);
		layerCoded = ActiveLayer << 1;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			if (StateMap.test(StateFlag::FRMSAM) && iForm == ClosestFormToCursor) {
				continue;
			}
			formLayer = FormList[iForm].attribute & FRMLMSK;
			if (!ActiveLayer || !formLayer || formLayer == layerCoded) {
				getfinfo(iForm);
				const fPOINT* vertices = FormList[iForm].vertices;
				if (vertices) {
					// find the closest line first and then find the closest vertex on that line
					for (iVertex = 0; iVertex < FormInfo.sideCount; iVertex++) {
						param = findDistanceToSide(vertices[iVertex], vertices[nxt(iVertex)], point, length);
						if ((length < minimumLength) & (length >= 0)) {
							minimumLength = length;
							closestForm   = iForm;
							if (param < 0.5) {
								closestVertex = iVertex;
							}
							else {
								closestVertex = nxt(iVertex);
							}
						}
					}
				}
			}
		}
		if (FormList[closestForm].vertices) {
			stch2pxr(FormList[closestForm].vertices[closestVertex]);
		}
		minimumLength = hypot(StitchCoordinatesPixels.x - screenCoordinate.x, StitchCoordinatesPixels.y - screenCoordinate.y);
		if (minimumLength < CLOSENUF) {
			ClosestFormToCursor   = closestForm;
			ClosestVertexToCursor = closestVertex;
			fvars(ClosestFormToCursor);
			StateMap.set(StateFlag::RELAYR);
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

void frmovlin() {
	size_t previousPoint = 0;

	fvars(ClosestFormToCursor);
	ratsr();
	if (SelectedForm->type == FRMLINE)
		NewFormVertexCount = SelectedForm->vertexCount;
	else
		NewFormVertexCount = SelectedForm->vertexCount + 1;
	frmlin(SelectedForm->vertices, SelectedForm->vertexCount);
	previousPoint = prv(ClosestVertexToCursor);
	for (auto iPoint = 0; iPoint < 3; iPoint++) {
		(*RubberBandLine)[iPoint] = FormLines[previousPoint];
		previousPoint++;
	}
	ritmov();
}

void makspac(unsigned start, unsigned count) {
	if (!chkmax(PCSHeader.stitchCount, count)) {
		std::copy(StitchBuffer + start,
		          StitchBuffer + PCSHeader.stitchCount,
		          stdext::make_checked_array_iterator(StitchBuffer + start + count, MAXITEMS - (start + count)));
		PCSHeader.stitchCount += count;
	}
}

bool ritlin(const fPOINT& start, const fPOINT& finish) noexcept {
	dPOINT   delta = {}, step = {};
	fPOINT   point  = {};
	double   length = 0.0;
	unsigned iStep = 0, count = 0;

	delta.x                                       = finish.x - start.x;
	delta.y                                       = finish.y - start.y;
	length                                        = hypot(delta.x, delta.y);
	InterleaveSequence[InterleaveSequenceIndex++] = start;
	if (length > MaxStitchLen) {
		count = ceil(length / UserStitchLen);
		if (!count)
			count = 1;
		while (length / count > MaxStitchLen)
			count++;
		if (!chkmax(InterleaveSequenceIndex, count)) {
			step.x  = delta.x / count;
			step.y  = delta.y / count;
			point.x = start.x + step.x;
			point.y = start.y + step.y;
			for (iStep = 0; iStep < count - 1; iStep++) {
				if (InterleaveSequenceIndex & MAXMSK) {
					InterleaveSequenceIndex = MAXITEMS - 2;
					return 0;
				}
				InterleaveSequence[InterleaveSequenceIndex++] = point;
				point.x += step.x;
				point.y += step.y;
			}
		}
		else {
			SequenceIndex = MAXITEMS - 2;
			return 0;
		}
	}
	return 1;
}

size_t closflt(float xCoordinate, float yCoordinate) noexcept {
	double length = 0.0, minimumLength = 0.0;
	size_t closestVertex = 0;

	minimumLength = 1e99;
	for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
		length = hypot(xCoordinate - CurrentFormVertices[iVertex].x, yCoordinate - CurrentFormVertices[iVertex].y);
		if (length < minimumLength) {
			closestVertex = iVertex;
			minimumLength = length;
		}
	}
	return closestVertex;
}

void chkseq(bool border) noexcept {
#if BUGBAK

	unsigned index;

	for (index = 0; index < SequenceIndex; index++) {
		InterleaveSequence[index] = OSequence[index];
	}
	InterleaveSequenceIndex = SequenceIndex;
#else

	double   len       = 0.0;
	unsigned iSequence = 0, destination = 0, savedIndex = 0;
	float    minimumStitchLength = 0.0;

	savedIndex = InterleaveSequenceIndex;
	if (border) {
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxBorderStitchLen;
		if (SelectedForm->edgeType == EDGELCHAIN || SelectedForm->edgeType == EDGEOCHAIN)
			UserStitchLen = 9 * PFGRAN;
		else
			UserStitchLen = SelectedForm->edgeStitchLen;
		minimumStitchLength = SelectedForm->minBorderStitchLen;
	}
	else {
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxFillStitchLen;
		if (isclp(ClosestFormToCursor))
			UserStitchLen = MaxStitchLen;
		else
			UserStitchLen = SelectedForm->lengthOrCount.stitchLength;
		minimumStitchLength = SelectedForm->minFillStitchLen;
	}
	if (UserStitchLen > MaxStitchLen)
		UserStitchLen = MaxStitchLen;
	bool flag = true;
	for (iSequence = 0; iSequence < SequenceIndex - 1; iSequence++) {
		if (!ritlin(OSequence[iSequence], OSequence[iSequence + 1])) {
			flag = false;
			break;
		}
	}
	if (flag) {
		InterleaveSequence[InterleaveSequenceIndex++] = OSequence[iSequence];
	}
	if (!minimumStitchLength)
		return;
	destination = savedIndex + 1;
	for (iSequence = savedIndex + 1; iSequence < InterleaveSequenceIndex; iSequence++) {
		len = hypot(InterleaveSequence[iSequence].x - InterleaveSequence[iSequence - 1].x,
		            InterleaveSequence[iSequence].y - InterleaveSequence[iSequence - 1].y);
		if (len > minimumStitchLength) {
			InterleaveSequence[destination] = InterleaveSequence[iSequence];
			destination++;
		}
	}
	InterleaveSequenceIndex = destination;
#endif
}

void ritbrd(unsigned& interleaveSequenceIndex2) noexcept {
	if (SequenceIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_BRD;
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPBRD;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->borderColor & COLMSK;
		chkseq(true);
		interleaveSequenceIndex2++;
	}
}

void ritapbrd(unsigned& interleaveSequenceIndex2) noexcept {
	if (SequenceIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_AP;
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->borderColor >> 4;
		chkseq(true);
		interleaveSequenceIndex2++;
	}
}

void ritfil(unsigned& interleaveSequenceIndex2) noexcept {
	if (SequenceIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPFRM;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillColor;
		chkseq(false);
		interleaveSequenceIndex2++;
	}
}

bool lastch() noexcept {
	if (InterleaveSequenceIndex) {
		LastPoint = InterleaveSequence[InterleaveSequenceIndex - 1];
		return 1;
	}
	else
		return 0;
}

size_t getlast() noexcept {
	size_t closestVertex = 0;
	double length = 0.0, dx = 0.0, dy = 0.0, minimumLength = 1e99;

	if (SelectedForm->fillType) {
		lastch();
		for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
			dx     = LastPoint.x - CurrentFormVertices[iVertex].x;
			dy     = LastPoint.y - CurrentFormVertices[iVertex].y;
			length = hypot(dx, dy);
			if (length < minimumLength) {
				minimumLength = length;
				closestVertex = iVertex;
			}
		}
		return closestVertex;
	}
	else
		return 0;
}

void filinsb(const dPOINT& point) noexcept {
	dPOINT       delta  = { (point.x - SelectedPoint.x), (point.y - SelectedPoint.y) };
	const double length = hypot(delta.x, delta.y);
	unsigned     count  = length / MAXSTCH + 1;
	dPOINT       step   = { (delta.x / count), (delta.y / count) };

	if (length > MAXSTCH) {
		count--;
		if (chkmax(count, SequenceIndex))
			return;
		while (count) {
			SelectedPoint.x += step.x;
			SelectedPoint.y += step.y;
			OSequence[SequenceIndex++] = SelectedPoint;
			count--;
		}
	}
	if (SequenceIndex & 0xffff0000)
		return;
	OSequence[SequenceIndex].x   = point.x;
	OSequence[SequenceIndex++].y = point.y;
	SelectedPoint.x              = point.x;
	SelectedPoint.y              = point.y;
}

void linrutb(unsigned start) noexcept {
	const double spacing = LineSpacing;
	unsigned     iVertex = start + 1;
	dPOINT       point   = { (CurrentFormVertices[start].x), (CurrentFormVertices[start].y) };

	for (iVertex = start + 1; iVertex < VertexCount; iVertex++) {
		point = CurrentFormVertices[iVertex];
		filinsb(point);
	}
	point = CurrentFormVertices[0];
	filinsb(point);
	LineSpacing = spacing;
}

constexpr float getblen() {
	size_t iLength = 0;

	iLength = (FormList[ClosestFormToCursor].clipEntries << 16) | FormList[ClosestFormToCursor].picoLength;
	return static_cast<float>(iLength);
}

void savblen(float fLength) {
	unsigned iLength;
	iLength = gsl::narrow<int>(round(fLength));

	FormList[ClosestFormToCursor].clipEntries = iLength >> 16;
	FormList[ClosestFormToCursor].picoLength  = iLength & 0xFFFF;
}

constexpr float getplen() {
	const unsigned value = SelectedForm->picoLength;

	return (static_cast<float>(value >> 8) + (value & 0xff) / 256);
}

void savplen(float length) {
	unsigned num = 0, fr = 0;
	double   integerPart = 0.0, fractionalPart = 0.0;

	if (length > 255)
		length = 255;
	fractionalPart                           = modf(length, &integerPart);
	fr                                       = gsl::narrow<unsigned>(floor(fractionalPart * 256));
	num                                      = integerPart;
	FormList[ClosestFormToCursor].picoLength = gsl::narrow<unsigned short>(num << 8) | fr;
}

void bdrlin(size_t start, size_t finish, double stitchSize) {
	dPOINT       delta = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
                     (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	dPOINT       step = {}, point = {};
	const double length      = hypot(delta.x, delta.y);
	double       angle       = 0.0;
	unsigned     stitchCount = 0;

	if (UserFlagMap.test(UserFlag::LINSPAC)) {
		stitchCount = length / stitchSize + 0.5;
		if (stitchCount) {
			step.x = delta.x / stitchCount;
			step.y = delta.y / stitchCount;
		}
	}
	else {
		stitchCount = (length - stitchSize / 2) / stitchSize + 1;
		angle       = atan2(delta.y, delta.x);
		step.x      = cos(angle) * stitchSize;
		step.y      = sin(angle) * stitchSize;
	}
	if (stitchCount) {
		point.x = CurrentFormVertices[start].x + step.x;
		point.y = CurrentFormVertices[start].y + step.y;
		stitchCount--;
		while (stitchCount) {
			OSequence[SequenceIndex].x   = point.x;
			OSequence[SequenceIndex++].y = point.y;
			point.x += step.x;
			point.y += step.y;
			stitchCount--;
		}
	}
	OSequence[SequenceIndex++] = CurrentFormVertices[finish];
}

void brdfil(double pd_Size) {
	unsigned iVertex    = 0;
	size_t   nextVertex = 0, currentVertex = 0;

	if (SelectedForm->extendedAttribute & AT_STRT)
		currentVertex = SelectedForm->fillStart;
	else
		currentVertex = getlast();
	SequenceIndex              = 0;
	OSequence[SequenceIndex++] = CurrentFormVertices[currentVertex];
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		nextVertex = nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, pd_Size);
		currentVertex = nextVertex;
	}
	if (SelectedForm->type != FRMLINE) {
		nextVertex = nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, pd_Size);
	}
}

void boldlin(size_t start, size_t finish, double size) noexcept {
	dPOINT       delta  = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
                     (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	fPOINT       step   = {};
	fPOINT       point0 = {}, point1 = {};
	const double length = hypot(delta.x, delta.y);
	unsigned     count  = length / size;

	if (count) {
		step.x   = delta.x / count;
		step.y   = delta.y / count;
		point0   = CurrentFormVertices[start];
		point1.x = point0.x + step.x;
		point1.y = point0.y + step.y;
		while (count) {
			OSequence[SequenceIndex++] = point1;
			OSequence[SequenceIndex++] = point0;
			OSequence[SequenceIndex++] = point1;
			point0.x += step.x;
			point0.y += step.y;
			point1.x += step.x;
			point1.y += step.y;
			count--;
		}
		OSequence[SequenceIndex++] = CurrentFormVertices[finish];
	}
	else {
		OSequence[SequenceIndex++] = CurrentFormVertices[finish];
		OSequence[SequenceIndex++] = CurrentFormVertices[start];
		OSequence[SequenceIndex++] = CurrentFormVertices[finish];
	}
}

void bold(double size) noexcept {
	unsigned iVertex = 0, iSequence = 0, iOutput = 0;
	size_t   iNextLine = getlast();
	size_t   iLine     = iNextLine;
	double   length    = 0.0;

	SequenceIndex              = 0;
	OSequence[SequenceIndex++] = CurrentFormVertices[iLine];
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		iNextLine = nxt(iLine);
		boldlin(iLine, iNextLine, size);
		iLine = iNextLine;
	}
	if (SelectedForm->type != FRMLINE) {
		iNextLine = nxt(iLine);
		boldlin(iLine, iNextLine, size);
	}
	for (iSequence = 0; iSequence < SequenceIndex - 1; iSequence++) {
		length = hypot(OSequence[iSequence + 1].x - OSequence[iSequence].x, OSequence[iSequence + 1].y - OSequence[iSequence].y);
		if (length > TINY) {
			OSequence[iOutput++] = OSequence[iSequence];
		}
	}
	OSequence[iOutput++] = CurrentFormVertices[iNextLine];
	SequenceIndex        = iOutput;
}

void duangs() {
	unsigned int iVertex;

	FormAngles->clear();
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		FormAngles->push_back(atan2(CurrentFormVertices[iVertex + 1].y - CurrentFormVertices[iVertex].y,
		                            CurrentFormVertices[iVertex + 1].x - CurrentFormVertices[iVertex].x));
	}
	FormAngles->push_back(atan2(CurrentFormVertices[0].y - CurrentFormVertices[iVertex].y,
	                            CurrentFormVertices[0].x - CurrentFormVertices[iVertex].x));
}

void outfn(unsigned start, unsigned finish, double satinWidth) {
	double angle   = 0.0;
	double length  = 0.0;
	double xOffset = 0.0, yOffset = 0.0;

	if (fabs((*FormAngles)[start]) < TINY && fabs((*FormAngles)[finish]) < TINY) {
		xOffset = 0.0;
		yOffset = satinWidth;
	}
	else {
#define SATHRESH 10

		angle  = ((*FormAngles)[finish] - (*FormAngles)[start]) / 2;
		length = satinWidth / cos(angle);
		if (length < -satinWidth * SATHRESH)
			length = -satinWidth * SATHRESH;
		if (length > satinWidth * SATHRESH)
			length = satinWidth * SATHRESH;
		angle += (*FormAngles)[start] + PI / 2;
		xOffset = length * cos(angle);
		yOffset = length * sin(angle);
	}
	(*InsidePoints)[finish].x  = CurrentFormVertices[finish].x - xOffset;
	(*InsidePoints)[finish].y  = CurrentFormVertices[finish].y - yOffset;
	(*OutsidePoints)[finish].x = CurrentFormVertices[finish].x + xOffset;
	(*OutsidePoints)[finish].y = CurrentFormVertices[finish].y + yOffset;
}

// find the intersection of a line defined by it's endpoints and a vertical line defined by it's x coordinate
bool projv(double xCoordinate, const fPOINT& lowerPoint, const fPOINT& upperPoint, dPOINT& intersection) noexcept {
	double       slope  = 0.0;
	const double deltaX = upperPoint.x - lowerPoint.x;

	intersection.x = xCoordinate;

	if (deltaX) {
		slope          = (upperPoint.y - lowerPoint.y) / deltaX;
		intersection.y = (xCoordinate - lowerPoint.x) * slope + lowerPoint.y;
		auto lower     = lowerPoint.x;
		auto upper     = upperPoint.x;
		if (lower > upper) {
			std::swap(lower, upper);
		}
		if (xCoordinate < lower || xCoordinate > upper)
			return false;
		else
			return true;
	}
	else
		return false;
}

bool cisin(float xCoordinate, float yCoordinate) noexcept {
	unsigned          count        = 0;
	size_t            nextVertex   = 0;
	dPOINT            intersection = {};
	const fRECTANGLE* rectangle    = &SelectedForm->rectangle;

	if (xCoordinate >= rectangle->right)
		return 0;
	if (xCoordinate <= rectangle->left)
		return 0;
	if (yCoordinate >= rectangle->top)
		return 0;
	if (yCoordinate <= rectangle->bottom)
		return 0;
	count = 0;
	for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
		nextVertex = nxt(iVertex);
		if (projv(xCoordinate, CurrentFormVertices[iVertex], CurrentFormVertices[nextVertex], intersection)) {
			if (intersection.y >= yCoordinate) {
				if (CurrentFormVertices[iVertex].x != xCoordinate && CurrentFormVertices[nextVertex].x != xCoordinate)
					count++;
				else {
					if (CurrentFormVertices[iVertex].x < CurrentFormVertices[nextVertex].x) {
						if (CurrentFormVertices[nextVertex].x != xCoordinate)
							count++;
					}
					else {
						if (CurrentFormVertices[iVertex].x != xCoordinate)
							count++;
					}
				}
			}
		}
	}
	return count & 1;
}

void satout(double satinWidth) {
	unsigned iVertex = 0;
	unsigned count   = 0;

	if (VertexCount) {
		duangs();
		OutsidePointList->resize(VertexCount);
		InsidePointList->resize(VertexCount);
		OutsidePoints = OutsidePointList;
		InsidePoints  = InsidePointList;
		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			outfn(iVertex, iVertex + 1, 0.1);
		count = 0;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			if (cisin((*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y))
				count++;
		}
		satinWidth /= 2;
		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			outfn(iVertex, iVertex + 1, satinWidth);
		outfn(iVertex, 0, satinWidth);
		StateMap.reset(StateFlag::INDIR);
		if (count<VertexCount>> 1) {
			StateMap.set(StateFlag::INDIR);
			OutsidePoints = InsidePointList;
			InsidePoints  = OutsidePointList;
		}
	}
}

/* find the intersection of two lines, one defined by point and slope, the other by the coordinates
   of the endpoints. */
bool proj(const dPOINT& point, double slope, const fPOINT& point0, const fPOINT& point1, dPOINT& intersectionPoint) noexcept {
	dPOINT delta     = {};
	double sideSlope = 0.0, pointConstant = 0.0, sideConstant = 0.0, xMinimum = 0.0;
	double xMaximum = 0.0, yMinimum = 0.0, yMaximum = 0.0;

	delta.x = point1.x - point0.x;
	delta.y = point1.y - point0.y;
	if (delta.x) {
		sideSlope           = delta.y / delta.x;
		sideConstant        = point0.y - sideSlope * point0.x;
		pointConstant       = point.y - slope * point.x;
		intersectionPoint.x = (sideConstant - pointConstant) / (slope - sideSlope);
		intersectionPoint.y = intersectionPoint.x * slope + pointConstant;
	}
	else {
		intersectionPoint.x = point0.x;
		pointConstant       = point.y - slope * point.x;
		intersectionPoint.y = intersectionPoint.x * slope + pointConstant;
	}
	xMinimum = point0.x;
	xMaximum = point1.x;
	if (xMinimum > xMaximum) {
		std::swap(xMinimum, xMaximum);
	}
	if (delta.y) {
		yMinimum = point0.y;
		yMaximum = point1.y;
		if (yMinimum > yMaximum) {
			std::swap(yMinimum, yMaximum);
		}
		if (intersectionPoint.x <= xMinimum || intersectionPoint.x > xMaximum || intersectionPoint.y < yMinimum
		    || intersectionPoint.y > yMaximum)
			return false;
		else
			return true;
	}
	else {
		if (intersectionPoint.x <= xMinimum || intersectionPoint.x > xMaximum)
			return false;
		else
			return true;
	}
}

bool linx(const std::vector<fPOINT>& points, size_t start, size_t finish, dPOINT& intersection) {
	if (OutsidePoints) {
		dPOINT delta = { ((*OutsidePoints)[start].x - points[start].x), ((*OutsidePoints)[start].y - points[start].y) };
		dPOINT point = { (points[start].x), (points[start].y) };

		if (!delta.x && !delta.y)
			return 0;
		if (delta.x) {
			if (proj(point, delta.y / delta.x, (*OutsidePoints)[finish], points[finish], intersection))
				return 1;
			else
				return 0;
		}
		else {
			if (projv(point.x, points[finish], (*OutsidePoints)[finish], intersection))
				return 1;
			else
				return 0;
		}
	}
	else {
		return 0;
	}
}

bool chkbak(const std::vector<dPOINT>& satinBackup, const dPOINT& pnt) {
	unsigned   iBackup = 0;
	double     length  = 0.0;
	const auto maxSB   = satinBackup.size();
	for (iBackup = 0; iBackup < maxSB; iBackup++) {
		length = hypot(satinBackup[iBackup].x - pnt.x, satinBackup[iBackup].y - pnt.y);
		if (length < LineSpacing)
			return 1;
	}
	return 0;
}

void filinsbw(std::vector<dPOINT>& satinBackup, const dPOINT& point) {
	satinBackup[SatinBackupIndex++] = point;
	SatinBackupIndex &= (satinBackup.size() - 1);
	filinsb(point);
}

void sbfn(const std::vector<fPOINT>& insidePoints, size_t start, size_t finish) {
	std::vector<dPOINT> satinBackup(8); // backup stitches in satin fills
	dPOINT innerDelta  = { (insidePoints[finish].x - insidePoints[start].x), (insidePoints[finish].y - insidePoints[start].y) };
	double innerLength = hypot(innerDelta.x, innerDelta.y);
	dPOINT outerDelta
	    = { ((*OutsidePoints)[finish].x - (*OutsidePoints)[start].x), ((*OutsidePoints)[finish].y - (*OutsidePoints)[start].y) };
	double   outerLength = hypot(outerDelta.x, outerDelta.y);
	dPOINT   innerPoint  = { insidePoints[start].x, insidePoints[start].y };
	dPOINT   outerPoint  = { (*OutsidePoints)[start].x, (*OutsidePoints)[start].y };
	dPOINT   innerStep = {}, outerStep = {};
	dPOINT   offsetDelta = {}, offsetStep = {}, offset = {};
	dPOINT   intersection = {};
	double   offsetLength = 0.0;
	unsigned count = 0, innerFlag = 0, outerFlag = 0, offsetCount = 0, iStep = 0;
	unsigned intersectFlag = 0;

	if (!StateMap.testAndSet(StateFlag::SAT1)) {
		SelectedPoint = insidePoints[start];
	}
	SatinBackupIndex = 0;
	for (auto& sb : satinBackup) {
		sb = { 1e12f, 1e12f };
	}
	if (outerLength > innerLength) {
		count     = outerLength / LineSpacing;
		innerFlag = 1;
		if (linx(insidePoints, start, finish, intersection)) {
			intersectFlag = 1;
			innerDelta.x = innerDelta.y = innerLength = 0;
			innerPoint                                = intersection;
		}
	}
	else {
		count     = innerLength / LineSpacing;
		outerFlag = 1;
		if (linx(insidePoints, start, finish, intersection)) {
			intersectFlag = 1;
			outerDelta.x = outerDelta.y = outerLength = 0;
			outerPoint                                = intersection;
		}
	}
	if (!count)
		count = 1;
	if (chkmax(count, SequenceIndex))
		return;
	innerStep.x = innerDelta.x / count;
	innerStep.y = innerDelta.y / count;
	outerStep.x = outerDelta.x / count;
	outerStep.y = outerDelta.y / count;
	for (iStep = 0; iStep < count; iStep++) {
		innerPoint.x += innerStep.x;
		innerPoint.y += innerStep.y;
		outerPoint.x += outerStep.x;
		outerPoint.y += outerStep.y;
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			if (innerFlag) {
				offsetDelta.x = innerPoint.x - SelectedPoint.x;
				offsetDelta.y = innerPoint.y - SelectedPoint.y;
				offsetLength  = hypot(offsetDelta.x, offsetDelta.y);
				offsetCount   = offsetLength / LineSpacing;
				offsetStep.x  = offsetDelta.x / offsetCount;
				offsetStep.y  = offsetDelta.y / offsetCount;
				offset.x      = innerPoint.x;
				offset.y      = innerPoint.y;
				while (chkbak(satinBackup, offset)) {
					offset.x -= offsetStep.x;
					offset.y -= offsetStep.y;
				}
				filinsbw(satinBackup, offset);
			}
			else
				filinsb(innerPoint);
		}
		else {
			if (outerFlag) {
				offsetDelta.x = outerPoint.x - SelectedPoint.x;
				offsetDelta.y = outerPoint.y - SelectedPoint.y;
				offsetLength  = hypot(offsetDelta.x, offsetDelta.y);
				offsetCount   = offsetLength / LineSpacing;
				offsetStep.x  = offsetDelta.x / offsetCount;
				offsetStep.y  = offsetDelta.y / offsetCount;
				offset.x      = outerPoint.x;
				offset.y      = outerPoint.y;
				while (chkbak(satinBackup, offset)) {
					offset.x -= offsetStep.x;
					offset.y -= offsetStep.y;
				}
				filinsbw(satinBackup, offset);
			}
			else
				filinsb(outerPoint);
		}
	}
}

void sfn(size_t startVertex) {
	size_t nextVertex = 0;

	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		nextVertex = nxt(startVertex);
		sbfn(*InsidePoints, startVertex, nextVertex);
		startVertex = nextVertex;
	}
	OSequence[0] = OSequence[SequenceIndex - 1];
	if (SequenceIndex > MAXITEMS - 2)
		SequenceIndex = MAXITEMS - 2;
}

void sbrd() {
	const double spacing = LineSpacing;
	const size_t start   = getlast();

	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SequenceIndex = 1;
	if (SelectedForm->edgeType & EGUND) {
		LineSpacing = USPAC;
		satout(HorizontalLength2 * URAT);
		sfn(start);
		StateMap.set(StateFlag::FILDIR);
		sfn(start);
	}
	fvars(ClosestFormToCursor);
	satout(HorizontalLength2);
	LineSpacing = SelectedForm->edgeSpacing;
	sfn(start);
	LineSpacing = spacing;
}

// find the intersection of a line defined by it's endpoints and a horizontal line defined by it's y coordinate
bool projh(double yCoordinate, const fPOINT& point0, const fPOINT& point1, dPOINT& intersection) noexcept {
	double       slope  = 0.0;
	const double deltaX = point1.x - point0.x;
	double       deltaY = 0.0;

	intersection.y = yCoordinate;
	if (deltaX) {
		deltaY = point1.y - point0.y;
		if (deltaY) {
			slope          = deltaY / deltaX;
			intersection.x = (yCoordinate - point0.y) / slope + point0.x;
		}
		else
			return false;
	}
	else
		intersection.x = point0.x;
	auto leftY  = point0.y;
	auto rightY = point1.y;
	if (leftY > rightY) {
		std::swap(leftY, rightY);
	}
	if (yCoordinate < leftY || yCoordinate > rightY)
		return false;
	else
		return true;
}

void sprct(std::vector<VRCT2>& fillVerticalRect, unsigned start, unsigned finish) {
	dPOINT delta
	    = { ((*OutsidePoints)[finish].x - (*OutsidePoints)[start].x), ((*OutsidePoints)[finish].y - (*OutsidePoints)[start].y) };
	dPOINT point        = {};
	VRCT2* verticalRect = &fillVerticalRect[start];

	if (delta.x && delta.y) {
		Slope = -delta.x / delta.y;
		point = CurrentFormVertices[finish];
		proj(point, Slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->dopnt);
		proj(point, Slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->dipnt);
		point = CurrentFormVertices[start];
		proj(point, Slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->aopnt);
		proj(point, Slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->aipnt);
		point = (*InsidePoints)[start];
		if (proj(point, Slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->bopnt)) {
			verticalRect->bipnt = (*InsidePoints)[start];
		}
		else {
			verticalRect->bopnt = (*OutsidePoints)[start];
			point               = (*OutsidePoints)[start];
			proj(point, Slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->bipnt);
		}
		point = (*InsidePoints)[finish];
		if (proj(point, Slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->copnt)) {
			verticalRect->cipnt = (*InsidePoints)[finish];
		}
		else {
			verticalRect->copnt = (*OutsidePoints)[finish];
			point               = (*OutsidePoints)[finish];
			proj(point, Slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->cipnt);
		}
	}
	else {
		if (delta.x) {
			point.x = CurrentFormVertices[finish].x;
			projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->dopnt);
			projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->dipnt);
			point.x = CurrentFormVertices[start].x;
			projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->aopnt);
			projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->aipnt);
			point.x = (*InsidePoints)[start].x;
			if (projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->bopnt)) {
				verticalRect->bipnt = (*InsidePoints)[start];
			}
			else {
				verticalRect->bopnt = (*OutsidePoints)[start];
				point.x             = (*OutsidePoints)[start].x;
				projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->bipnt);
			}
			point.x = (*InsidePoints)[finish].x;
			if (projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->copnt)) {
				verticalRect->cipnt = (*InsidePoints)[finish];
			}
			else {
				verticalRect->copnt = (*OutsidePoints)[finish];
				point.x             = (*OutsidePoints)[finish].x;
				projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->cipnt);
			}
		}
		else {
			point.y = CurrentFormVertices[finish].y;
			projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->dopnt);
			projh(point.y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->dipnt);
			point.y = CurrentFormVertices[start].y;
			projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->aopnt);
			projh(point.y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->aipnt);
			point.y = (*InsidePoints)[start].y;
			if (projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->bopnt)) {
				verticalRect->bipnt = (*InsidePoints)[start];
			}
			else {
				verticalRect->bopnt = (*OutsidePoints)[start];
				point.y             = (*OutsidePoints)[start].y;
				projh(point.y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->bipnt);
			}
			point.y = (*InsidePoints)[finish].y;
			if (projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->copnt)) {
				verticalRect->cipnt = (*InsidePoints)[finish];
			}
			else {
				verticalRect->copnt = (*OutsidePoints)[finish];
				point.y             = (*OutsidePoints)[finish].y;
				projh((*OutsidePoints)[finish].y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->cipnt);
			}
		}
	}
}

void spurfn(const dPOINT& innerPoint, const dPOINT& outerPoint, dPOINT& underlayInnerPoint, dPOINT& underlayOuterPoint) noexcept {
	dPOINT delta;

	delta.x              = outerPoint.x - innerPoint.x;
	delta.y              = outerPoint.y - innerPoint.y;
	underlayInnerPoint.x = delta.x * DIURAT + innerPoint.x;
	underlayInnerPoint.y = delta.y * DIURAT + innerPoint.y;
	underlayOuterPoint.x = delta.x * DOURAT + innerPoint.x;
	underlayOuterPoint.y = delta.y * DOURAT + innerPoint.y;
}

void spurct(std::vector<VRCT2>& underlayVerticalRect, const std::vector<VRCT2>& fillVerticalRect, unsigned iRect) {
	spurfn(fillVerticalRect[iRect].aipnt,
	       fillVerticalRect[iRect].aopnt,
	       underlayVerticalRect[iRect].aipnt,
	       underlayVerticalRect[iRect].aopnt);
	spurfn(fillVerticalRect[iRect].bipnt,
	       fillVerticalRect[iRect].bopnt,
	       underlayVerticalRect[iRect].bipnt,
	       underlayVerticalRect[iRect].bopnt);
	spurfn(fillVerticalRect[iRect].cipnt,
	       fillVerticalRect[iRect].copnt,
	       underlayVerticalRect[iRect].cipnt,
	       underlayVerticalRect[iRect].copnt);
	spurfn(fillVerticalRect[iRect].dipnt,
	       fillVerticalRect[iRect].dopnt,
	       underlayVerticalRect[iRect].dipnt,
	       underlayVerticalRect[iRect].dopnt);
}

void duromb(const dPOINT& start0, const dPOINT& finish0, const dPOINT& start1, const dPOINT& finish1) {
	dPOINT   delta0 = {}, delta1 = {}, step0 = {}, step1 = {};
	double   length0 = 0.0, length1 = 0.0;
	unsigned count = 0, iStep = 0;

	if (!StateMap.test(StateFlag::UND)) {
		delta0.x = SelectedPoint.x - start0.x;
		delta0.y = SelectedPoint.y - start0.y;
		delta1.x = SelectedPoint.x - start1.x;
		delta1.y = SelectedPoint.y - start1.y;
		length0  = hypot(delta0.x, delta0.y);
		length1  = hypot(delta1.x, delta1.y);
		if (length0 > length1)
			StateMap.set(StateFlag::FILDIR);
		else
			StateMap.reset(StateFlag::FILDIR);
	}
	delta0.x = finish0.x - start0.x;
	delta0.y = finish0.y - start0.y;
	delta1.x = finish1.x - start1.x;
	delta1.y = finish1.y - start1.y;
	length0  = hypot(delta0.x, delta0.y);
	count    = length0 / (LineSpacing / 2);
	if (!count)
		count++;
	step0.x = delta0.x / count;
	step0.y = delta0.y / count;
	step1.x = delta1.x / count;
	step1.y = delta1.y / count;

	dPOINT start00 = start0;
	dPOINT start11 = start1;
	for (iStep = 0; iStep < count; iStep++) {
		if (StateMap.testAndFlip(StateFlag::FILDIR))
			filinsb(start00);
		else
			filinsb(start11);
		start00.x += step0.x;
		start00.y += step0.y;
		start11.x += step1.x;
		start11.y += step1.y;
	}
}

unsigned psg() noexcept {
	unsigned temp = 0;

	if (!PseudoRandomValue)
		PseudoRandomValue = SEED;
	temp = PseudoRandomValue & 0x48000000;
	PseudoRandomValue <<= 1;
	if (temp == 0x40000000 || temp == 0x8000000)
		PseudoRandomValue++;
	return PseudoRandomValue;
}

void spend(const std::vector<VRCT2>& fillVerticalRect, size_t start, size_t finish) {
	dPOINT       innerDelta  = { (fillVerticalRect[finish].cipnt.x - fillVerticalRect[start].bipnt.x),
                          (fillVerticalRect[finish].cipnt.y - fillVerticalRect[start].bipnt.y) };
	const double innerLength = hypot(innerDelta.x, innerDelta.y);
	dPOINT       outerDelta  = { (fillVerticalRect[finish].copnt.x - fillVerticalRect[start].bopnt.x),
                          (fillVerticalRect[finish].copnt.y - fillVerticalRect[start].bopnt.y) };
	const double outerLength = hypot(outerDelta.x, outerDelta.y);
	double       startAngle = 0.0, finishAngle = 0.0, deltaAngle = 0.0, stepAngle = 0.0;
	dPOINT       startDelta = {}, finishDelta = {};
	dPOINT       pivot  = {};
	double       radius = 0.0, arc = 0.0, innerRadius = 0.0;
	unsigned     ind = 0, count = 0, level = 0;
	dPOINT       innerPoint = {}, outerPoint = {};

	if (outerLength > innerLength) {
		pivot.x       = fillVerticalRect[start].cipnt.x;
		pivot.y       = fillVerticalRect[start].cipnt.y;
		startDelta.x  = fillVerticalRect[start].copnt.x - pivot.x;
		startDelta.y  = fillVerticalRect[start].copnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bopnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bopnt.y - pivot.y;
	}
	else {
		pivot.x       = fillVerticalRect[start].copnt.x;
		pivot.y       = fillVerticalRect[start].copnt.y;
		startDelta.x  = fillVerticalRect[start].cipnt.x - pivot.x;
		startDelta.y  = fillVerticalRect[start].cipnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bipnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bipnt.y - pivot.y;
	}
	if (hypot(SelectedPoint.x - pivot.x, SelectedPoint.y - pivot.y) > 2 * PI)
		filinsb(pivot);
	startAngle  = atan2(startDelta.y, startDelta.x);
	finishAngle = atan2(finishDelta.y, finishDelta.x);
	deltaAngle  = finishAngle - startAngle;
	if (deltaAngle > PI)
		deltaAngle -= 2 * PI;
	if (deltaAngle < -PI)
		deltaAngle += 2 * PI;
	radius    = hypot(startDelta.x, startDelta.y);
	arc       = fabs(radius * deltaAngle);
	count     = arc / LineSpacing;
	stepAngle = deltaAngle / count;
	if (!count)
		count = 1;
	for (ind = 0; ind < count; ind++) {
		outerPoint.x = pivot.x + cos(startAngle) * radius;
		outerPoint.y = pivot.y + sin(startAngle) * radius;
		filinsb(outerPoint);
		if (count & 0xfffffff0)
			level = psg() % count;
		else
			level = Levels[count][ind];
		innerRadius  = radius * level / count * 0.4;
		innerPoint.x = pivot.x + cos(startAngle) * innerRadius;
		innerPoint.y = pivot.y + sin(startAngle) * innerRadius;
		filinsb(innerPoint);
		startAngle += stepAngle;
	}
}

void duspnd(const std::vector<VRCT2>& underlayVerticalRect,
            const std::vector<VRCT2>& fillVerticalRect,
            size_t                    start,
            size_t                    finish) {
	double length = 0.0, angle = 0.0;
	dPOINT point = {}, delta = {};

	if (StateMap.test(StateFlag::UND)) {
		if (StateMap.test(StateFlag::UNDPHAS)) {
			filinsb(underlayVerticalRect[start].copnt);
			filinsb(underlayVerticalRect[start].cipnt);
			delta.x = underlayVerticalRect[finish].bipnt.x - underlayVerticalRect[start].cipnt.x;
			delta.y = underlayVerticalRect[finish].bipnt.y - underlayVerticalRect[start].cipnt.y;
			length  = hypot(delta.x, delta.y);
			if (length > SelectedForm->edgeStitchLen) {
				angle   = atan2((*InsidePoints)[finish].y - (*OutsidePoints)[finish].y,
                              (*InsidePoints)[finish].x - (*OutsidePoints)[finish].x);
				point.x = underlayVerticalRect[finish].bopnt.x + cos(angle) * HorizontalLength2;
				point.y = underlayVerticalRect[finish].bopnt.y + sin(angle) * HorizontalLength2;
				filinsb(point);
			}
			filinsb(underlayVerticalRect[finish].bipnt);
			filinsb(underlayVerticalRect[finish].bopnt);
		}
		else {
			filinsb(underlayVerticalRect[start].cipnt);
			filinsb(underlayVerticalRect[start].copnt);
			delta.x = underlayVerticalRect[finish].bopnt.x - underlayVerticalRect[start].copnt.x;
			delta.y = underlayVerticalRect[finish].bopnt.y - underlayVerticalRect[start].copnt.y;
			length  = hypot(delta.x, delta.y);
			if (length > SelectedForm->edgeStitchLen) {
				angle   = atan2((*OutsidePoints)[finish].y - (*InsidePoints)[finish].y,
                              (*OutsidePoints)[finish].x - (*InsidePoints)[finish].x);
				point.x = underlayVerticalRect[finish].bipnt.x + cos(angle) * HorizontalLength2;
				point.y = underlayVerticalRect[finish].bipnt.y + sin(angle) * HorizontalLength2;
				filinsb(point);
			}
			filinsb(underlayVerticalRect[finish].bopnt);
			filinsb(underlayVerticalRect[finish].bipnt);
		}
	}
	else
		spend(fillVerticalRect, start, finish);
}

void pfn(const std::vector<VRCT2>& underlayVerticalRect,
         const std::vector<VRCT2>& fillVerticalRect,
         size_t                    startVertex,
         const std::vector<VRCT2>& vrct) {
	unsigned iVertex       = 0;
	size_t   currentVertex = startVertex;
	size_t   nextVertex    = nxt(currentVertex);

	SelectedPoint = CurrentFormVertices[startVertex];
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		duromb(vrct[currentVertex].bipnt, vrct[currentVertex].cipnt, vrct[currentVertex].bopnt, vrct[currentVertex].copnt);
		duspnd(underlayVerticalRect, fillVerticalRect, currentVertex, nextVertex);
		currentVertex = nextVertex;
		nextVertex    = nxt(nextVertex);
	}
}

void prsmal() noexcept {
	unsigned iSequence = 0, iOutput = 0, iReference = 0;
	double   minimumLength = USPAC * 0.8;
	dPOINT   delta         = {};
	double   length        = 0.0;

	if (minimumLength > HorizontalLength2)
		minimumLength = HorizontalLength2 * 0.9;
	for (iSequence = 1; iSequence < SequenceIndex; iSequence++) {
		delta.x = OSequence[iSequence].x - OSequence[iReference].x;
		delta.y = OSequence[iSequence].y - OSequence[iReference].y;
		length  = hypot(delta.x, delta.y);
		if (length > minimumLength) {
			OSequence[iOutput++] = OSequence[iSequence];
			iReference           = iSequence;
		}
	}
	SequenceIndex = iOutput;
}

void pbrd(double edgeSpacing) {
	const double spacing = LineSpacing;
	unsigned     iVertex = 0;
	const size_t start   = getlast();

	LineSpacing   = SelectedForm->edgeSpacing;
	SequenceIndex = 0;
	std::vector<VRCT2> fillVerticalRect(VertexCount);
	std::vector<VRCT2> underlayVerticalRect(VertexCount);
	satout(SelectedForm->borderSize);
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, iVertex, 0);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	if (SelectedForm->edgeType & EGUND) {
		StateMap.reset(StateFlag::SAT1);
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		satout(HorizontalLength2);
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.set(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect);
		StateMap.reset(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect);
		LineSpacing = edgeSpacing;
		prsmal();
		HorizontalLength2 = SelectedForm->borderSize;
		StateMap.reset(StateFlag::UND);
	}
	pfn(underlayVerticalRect, fillVerticalRect, start, fillVerticalRect);
	LineSpacing = spacing;
}

void apbrd() {
	unsigned iVertex    = 0;
	size_t   nextVertex = 0, currentVertex = 0;

	SequenceIndex              = 0;
	OSequence[SequenceIndex++] = CurrentFormVertices[currentVertex];
	for (iVertex = 0; iVertex < VertexCount << 1; iVertex++) {
		nextVertex = nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, APSPAC);
		currentVertex = nextVertex;
	}
}

void bhfn(size_t start, size_t finish, double spacing) noexcept {
	dPOINT         delta      = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
                     (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	const double   length     = hypot(delta.x, delta.y);
	const unsigned count      = length / spacing;
	dPOINT         step       = { (delta.x / count), (delta.y / count) };
	dPOINT         innerPoint = { CurrentFormVertices[start].x, CurrentFormVertices[start].y };
	dPOINT         outerStep = {}, firstPoint = {}, outerPoint = {};
	unsigned       iStep = 0;

	const double rotationAngle = atan2(-delta.x, delta.y);
	outerStep.x                = SelectedForm->borderSize * cos(rotationAngle);
	outerStep.y                = SelectedForm->borderSize * sin(rotationAngle);
	if (count) {
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
			OSequence[SequenceIndex].x   = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			innerPoint.x += step.x;
			innerPoint.y += step.y;
		}
		firstPoint.x                 = innerPoint.x + step.x;
		firstPoint.y                 = innerPoint.y + step.y;
		OSequence[SequenceIndex].x   = firstPoint.x;
		OSequence[SequenceIndex++].y = firstPoint.y;
		OSequence[SequenceIndex].x   = innerPoint.x;
		OSequence[SequenceIndex++].y = innerPoint.y;
	}
}

void bhcrnr(size_t vertex) {
	const size_t nextVertex = nxt(vertex);
	dPOINT       delta = {}, point = {};
	double       length = 0.0, ratio = 0.0;

	if (StateMap.test(StateFlag::INDIR)) {
		delta.x = (*OutsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = (*OutsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	else {
		delta.x = (*InsidePoints)[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = (*InsidePoints)[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	length = hypot(delta.x, delta.y);
	ratio  = ButtonholeCornerLength / length;
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
	OSequence[SequenceIndex++]   = CurrentFormVertices[nextVertex];
}

void bhbrd(double spacing) {
	size_t vertex     = getlast();
	size_t nextVertex = 0;

	SequenceIndex              = 0;
	OSequence[SequenceIndex++] = CurrentFormVertices[vertex];
	for (size_t iVertex = 0; iVertex < VertexCount; iVertex++) {
		nextVertex = nxt(vertex);
		bhfn(vertex, nextVertex, spacing);
		bhcrnr(vertex);
		vertex = nextVertex;
	}
}

void dubfn() {
	size_t iBackward = 0, iForward = 0;

	brdfil(SelectedForm->edgeStitchLen);
	iForward = SequenceIndex;
	for (iBackward = SequenceIndex; iBackward != 0; iBackward--) {
		OSequence[iForward++] = OSequence[iBackward - 1];
	}
	SequenceIndex = iForward;
}

void chkbrd(unsigned& interleaveSequenceIndex2) {
	float length = 0.0;

	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		switch (SelectedForm->edgeType & NEGUND) {
		case EDGELINE: // Line
			brdfil(SelectedForm->edgeStitchLen);
			break;
		case EDGEBEAN: // Bean
			bold(SelectedForm->edgeStitchLen);
			break;
		case EDGECLIP: // Clipboard
			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpbrd(getlast());
			break;
		case EDGECLIPX: // Even Clipboard
			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			duxclp();
			break;
		case EDGEANGSAT: // Angle Satin
			HorizontalLength2 = SelectedForm->borderSize;
			sbrd();
			break;
		case EDGEPROPSAT: // Proportional Satin
			HorizontalLength2 = SelectedForm->borderSize;
			pbrd(SelectedForm->edgeSpacing);
			break;
		case EDGEAPPL: // Applique
			SequenceIndex = 0;
			apbrd();
			ritapbrd(interleaveSequenceIndex2);
			HorizontalLength2 = SelectedForm->borderSize;
			sbrd();
			break;
		case EDGEBHOL: // BH Buttonhole
			HorizontalLength2      = SelectedForm->borderSize * 2;
			length                 = ButtonholeCornerLength;
			ButtonholeCornerLength = getblen();
			satout(20);
			bhbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = length;
			break;
		case EDGEPICOT: // Picot
			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpic();
			break;
		case EDGEDOUBLE: // Double
			dubfn();
			break;
		case EDGELCHAIN: // Lin Chain
			StateMap.set(StateFlag::LINCHN);
			chnfn();
			break;
		case EDGEOCHAIN: // Open Chain
			StateMap.reset(StateFlag::LINCHN);
			chnfn();
			break;
		}
		ritbrd(interleaveSequenceIndex2);
	}
}

void fnvrt(std::vector<unsigned>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints) {
	unsigned iVertex = 0, iNextVertex = 0, iLine = 0, evenPointCount = 0;
	unsigned iLineCounter = 0, iPoint = 0, fillLineCount = 0, savedLineCount = 0;
	int      lineOffset = 0;
	double   lowX = 0.0, highX = 0.0;
	double   currentX = 0.0, step = 0.0;
	dPOINT   point        = {};
	unsigned maximumLines = 0; // maximum angle fill lines for any adjusted y cordinate

	CurrentFillVertices = SelectedForm->vertices;
	highX = lowX = CurrentFillVertices[0].x;
	VertexCount  = SelectedForm->vertexCount;
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		if (CurrentFillVertices[iVertex].x > highX)
			highX = CurrentFillVertices[iVertex].x;
		if (CurrentFillVertices[iVertex].x < lowX)
			lowX = CurrentFillVertices[iVertex].x;
	}

	lineOffset    = lowX / LineSpacing;
	lowX          = LineSpacing * lineOffset;
	fillLineCount = (highX - lowX) / LineSpacing + 1;
	std::vector<dPOINTLINE> projectedPoints;
	projectedPoints.reserve(VertexCount + 2);
	step     = (highX - lowX) / fillLineCount;
	currentX = lowX;
	for (iLine = 0; iLine < fillLineCount; iLine++) {
		iLineCounter = 0;
		currentX += step;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			iNextVertex = (iVertex + 1) % VertexCount;
			if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[iNextVertex], point))
				iLineCounter++;
		}
		fillLineCount += iLineCounter;
		if (iLineCounter > maximumLines)
			maximumLines = iLineCounter;
	}
	maximumLines = (maximumLines >> 1);
	lineEndpoints.reserve(fillLineCount + 1);
	StitchLineCount = 0;
	LineGroupIndex  = 0;
	// groupIndex cannot be more than fillLineCount so reserve that amount of memory to reduce re-allocations
	groupIndexSequence.reserve(fillLineCount);
	GroupIndexCount = 0;
	currentX        = lowX;
	for (iLine = 0; iLine < fillLineCount; iLine++) {
		projectedPoints.clear();
		currentX += step;
		iPoint = 0;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			iNextVertex = (iVertex + 1) % VertexCount;
			if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[iNextVertex], point)) {
				dPOINTLINE a = { point.x, point.y, gsl::narrow<unsigned short>(iVertex) };
				projectedPoints.push_back(a);
				iPoint++;
			}
		}
		if (iPoint > 1) {
			evenPointCount = iPoint &= 0xfffffffe;
			groupIndexSequence.push_back(StitchLineCount);
			std::sort(projectedPoints.begin(), projectedPoints.end(), comp);
			iPoint         = 0;
			savedLineCount = StitchLineCount;
			while (iPoint < evenPointCount) {
				if (StitchLineCount < fillLineCount) {
					lineEndpoints.push_back(
					    { projectedPoints[iPoint].line, LineGroupIndex, projectedPoints[iPoint].x, projectedPoints[iPoint].y });
					iPoint++;
					lineEndpoints.push_back(
					    { projectedPoints[iPoint].line, LineGroupIndex, projectedPoints[iPoint].x, projectedPoints[iPoint].y });
					iPoint++;
					StitchLineCount += 2;
				}
			}
			if (StitchLineCount != savedLineCount)
				LineGroupIndex++;
		}
	}
	groupIndexSequence.push_back(StitchLineCount);
	GroupIndexCount = groupIndexSequence.size();
	LineGroupIndex--;
}

void fnang(std::vector<unsigned>& groupIndexSequence,
           std::vector<SMALPNTL>& lineEndpoints,
           double                 rotationAngle,
           dPOINT&                rotationCenter) {
	unsigned iVertex = 0;

	AngledForm = FormList[ClosestFormToCursor];
	rotationCenter
	    = { (static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left),
		    (static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom) };
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex] = SelectedForm->vertices[iVertex];
		rotflt(AngledForm.vertices[iVertex], rotationAngle, rotationCenter);
	}
	SelectedForm = &AngledForm;
	fnvrt(groupIndexSequence, lineEndpoints);
	SelectedForm = &FormList[ClosestFormToCursor];
}

void fnhor(std::vector<unsigned>& groupIndexSequence,
           std::vector<SMALPNTL>& lineEndpoints,
           const double           rotationAngle,
           dPOINT&                rotationCenter) {
	unsigned iVertex = 0;

	AngledForm = FormList[ClosestFormToCursor];
	rotationCenter
	    = { (static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left),
		    (static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom) };
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex] = SelectedForm->vertices[iVertex];
		rotflt(AngledForm.vertices[iVertex], rotationAngle, rotationCenter);
	}
	SelectedForm = &AngledForm;
	fnvrt(groupIndexSequence, lineEndpoints);
	SelectedForm = &FormList[ClosestFormToCursor];
}

void satends(unsigned isBlunt) {
	fPOINT step = {};

	if (isBlunt & SBLNT) {
		step.x = sin((*FormAngles)[0]) * HorizontalLength2 / 2;
		step.y = cos((*FormAngles)[0]) * HorizontalLength2 / 2;
		if (StateMap.test(StateFlag::INDIR)) {
			step.x = -step.x;
			step.y = -step.y;
		}
		(*InsidePoints)[0].x  = SelectedForm->vertices[0].x + step.x;
		(*InsidePoints)[0].y  = SelectedForm->vertices[0].y - step.y;
		(*OutsidePoints)[0].x = SelectedForm->vertices[0].x - step.x;
		(*OutsidePoints)[0].y = SelectedForm->vertices[0].y + step.y;
	}
	else {
		(*InsidePoints)[0] = (*OutsidePoints)[0] = CurrentFormVertices[0];
	}
	if (isBlunt & FBLNT) {
		step.x = sin((*FormAngles)[VertexCount - 2]) * HorizontalLength2 / 2;
		step.y = cos((*FormAngles)[VertexCount - 2]) * HorizontalLength2 / 2;
		if (StateMap.test(StateFlag::INDIR)) {
			step.x = -step.x;
			step.y = -step.y;
		}
		(*InsidePoints)[VertexCount - 1].x  = SelectedForm->vertices[VertexCount - 1].x + step.x;
		(*InsidePoints)[VertexCount - 1].y  = SelectedForm->vertices[VertexCount - 1].y - step.y;
		(*OutsidePoints)[VertexCount - 1].x = SelectedForm->vertices[VertexCount - 1].x - step.x;
		(*OutsidePoints)[VertexCount - 1].y = SelectedForm->vertices[VertexCount - 1].y + step.y;
	}
	else {
		(*InsidePoints)[VertexCount - 1] = (*OutsidePoints)[VertexCount - 1] = CurrentFormVertices[VertexCount - 1];
	}
}

void slbrd() {
	const double spacing = LineSpacing;

	SequenceIndex = 0;
	if (SelectedForm->edgeType & EGUND) {
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		satout(HorizontalLength2);
		satends(SelectedForm->attribute);
		StateMap.reset(StateFlag::SAT1);
		StateMap.reset(StateFlag::FILDIR);
		LineSpacing = USPAC;
		for (size_t iVertex = 0u; iVertex < SelectedForm->vertexCount - 1; iVertex++)
			sbfn(*InsidePoints, iVertex, iVertex + 1);
		StateMap.flip(StateFlag::FILDIR);
		for (size_t iVertex = SelectedForm->vertexCount - 1; iVertex != 0; iVertex--)
			sbfn(*InsidePoints, iVertex, iVertex - 1);
	}
	HorizontalLength2 = SelectedForm->borderSize;
	satout(HorizontalLength2);
	satends(SelectedForm->attribute);
	LineSpacing = SelectedForm->edgeSpacing;
	StateMap.reset(StateFlag::SAT1);
	for (size_t iVertex = 0; iVertex < SelectedForm->vertexCount - 1; iVertex++)
		sbfn(*InsidePoints, iVertex, iVertex + 1);
	LineSpacing = spacing;
}

void prebrd() {
	fPOINT delta
	    = { (CurrentFormVertices[1].x - CurrentFormVertices[0].x), (CurrentFormVertices[1].y - CurrentFormVertices[0].y) };
	double ratio = 0.0;

	std::copy(CurrentFormVertices,
	          CurrentFormVertices + VertexCount,
	          stdext::make_checked_array_iterator((AngledFormVertices + 1), (20000 - 1)));
	if (fabs(delta.x) > fabs(delta.y))
		ratio = fabs(0.1 / delta.x);
	else
		ratio = fabs(0.1 / delta.y);
	AngledFormVertices[0].x = CurrentFormVertices[0].x - delta.x * ratio;
	AngledFormVertices[0].y = CurrentFormVertices[0].y - delta.y * ratio;
	AngledForm              = *SelectedForm;
	AngledForm.vertices     = AngledFormVertices;
	AngledForm.vertexCount += 3;
	delta.x = CurrentFormVertices[VertexCount - 1].x - CurrentFormVertices[VertexCount - 2].x;
	delta.y = CurrentFormVertices[VertexCount - 1].y - CurrentFormVertices[VertexCount - 2].y;
	if (fabs(delta.x) > fabs(delta.y))
		ratio = fabs(0.1 / delta.x);
	else
		ratio = fabs(0.1 / delta.y);
	AngledFormVertices[AngledForm.vertexCount - 1].x = CurrentFormVertices[VertexCount - 1].x + delta.x * ratio;
	AngledFormVertices[AngledForm.vertexCount - 1].y = CurrentFormVertices[VertexCount - 1].y + delta.y * ratio;
	SelectedForm                                     = &AngledForm;
	VertexCount                                      = AngledForm.vertexCount;
	CurrentFormVertices                              = AngledForm.vertices;
}

void plfn(const std::vector<VRCT2>& underlayVerticalRect,
          const std::vector<VRCT2>& fillVerticalRect,
          const std::vector<VRCT2>& prct) {
	unsigned iVertex;

	duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt);
	duspnd(underlayVerticalRect, fillVerticalRect, 1, 2);
	for (iVertex = 2; iVertex < VertexCount - 4; iVertex++) {
		duromb(prct[iVertex].bipnt, prct[iVertex].cipnt, prct[iVertex].bopnt, prct[iVertex].copnt);
		duspnd(underlayVerticalRect, fillVerticalRect, iVertex, iVertex + 1);
	}
	duromb(prct[VertexCount - 4].bipnt, prct[VertexCount - 4].dipnt, prct[VertexCount - 4].bopnt, prct[VertexCount - 4].dopnt);
}

void plbak(size_t backPoint) noexcept {
	size_t iSequence = SequenceIndex - 1;
	fPOINT tflt      = {};

	while (iSequence > backPoint) {
		tflt                 = OSequence[iSequence];
		OSequence[iSequence] = OSequence[backPoint];
		OSequence[backPoint] = tflt;
		iSequence--;
		backPoint++;
	}
}

void plbrd(double edgeSpacing) {
	size_t   savedIndex = 0;
	unsigned iVertex    = 0;

	// Ensure that we have at least 4 array members
	std::vector<VRCT2> fillVerticalRect(VertexCount + 3);
	std::vector<VRCT2> underlayVerticalRect(VertexCount + 3);
	prebrd();
	satout(SelectedForm->borderSize);
	InsidePoints->push_back((*InsidePoints)[0]);
	OutsidePoints->push_back((*OutsidePoints)[0]);
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, iVertex, 0);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	if (!(SelectedForm->attribute & SBLNT)) {
		const auto val                = SelectedForm->vertices[1];
		fillVerticalRect[1].aipnt     = val;
		fillVerticalRect[1].aopnt     = val;
		underlayVerticalRect[1].aipnt = val;
		underlayVerticalRect[1].aopnt = val;
	}
	if (!(SelectedForm->attribute & FBLNT)) {
		const auto val                              = SelectedForm->vertices[VertexCount - 1];
		fillVerticalRect[VertexCount - 4].dipnt     = val;
		fillVerticalRect[VertexCount - 4].dopnt     = val;
		underlayVerticalRect[VertexCount - 4].dipnt = val;
		underlayVerticalRect[VertexCount - 4].dopnt = val;
	}
	SequenceIndex = 0;
	SelectedPoint = CurrentFormVertices[0];
	if (SelectedForm->edgeType & EGUND) {
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		plfn(underlayVerticalRect, fillVerticalRect, underlayVerticalRect);
		savedIndex = SequenceIndex;
		StateMap.reset(StateFlag::UNDPHAS);
		SelectedPoint = CurrentFormVertices[0];
		StateMap.set(StateFlag::FILDIR);
		plfn(underlayVerticalRect, fillVerticalRect, underlayVerticalRect);
		plbak(savedIndex);
		prsmal();
		if (SequenceIndex) { // ensure that we can do a valid read from OSequence
			SelectedPoint = OSequence[SequenceIndex - 1];
		}
	}
	StateMap.reset(StateFlag::UND);
	LineSpacing = SelectedForm->edgeSpacing;
	plfn(underlayVerticalRect, fillVerticalRect, fillVerticalRect);
	LineSpacing = edgeSpacing;
	fvars(ClosestFormToCursor);
}

void lapbrd() {
	const double savedStitchLength = UserStitchLength;

	SequenceIndex = 0;
	// ToDo - Should APSPAC be a configurable variable instead?
	UserStitchLength = APSPAC;
	for (size_t iVertex = 0; iVertex < VertexCount - 1; iVertex++)
		bdrlin(iVertex, iVertex + 1, APSPAC);
	for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--)
		bdrlin(iVertex, iVertex - 1, APSPAC);
	UserStitchLength = savedStitchLength;
}

void blbrd(double spacing) {
	unsigned iVertex = 0;

	SequenceIndex              = 0;
	OSequence[SequenceIndex++] = CurrentFormVertices[0];
	for (iVertex = 0; iVertex < VertexCount - 2; iVertex++) {
		bhfn(iVertex, iVertex + 1, spacing);
		bhcrnr(iVertex);
	}
	bhfn(iVertex, iVertex + 1, spacing);
	OSequence[SequenceIndex++] = CurrentFormVertices[VertexCount - 1];
}

void contf() {
	const size_t  start          = SelectedForm->angleOrClipData.guide.start;
	const fPOINT* selectionStart = &CurrentFormVertices[start];
	if (selectionStart) {
		const size_t          finish              = SelectedForm->angleOrClipData.guide.finish;
		const size_t          lowVertexIndex      = start;
		const size_t          highVertexIndex     = VertexCount - start - 1;
		const size_t          selectedVertexCount = finish - start;
		std::vector<unsigned> lowCounts(lowVertexIndex);
		std::vector<unsigned> highCounts(highVertexIndex);
		std::vector<double>   lowLengths(lowVertexIndex);
		std::vector<double>   highLengths(highVertexIndex);
		std::vector<dPOINT>   lowDeltas(lowVertexIndex);
		std::vector<dPOINT>   highDeltas(highVertexIndex);
		std::vector<dPOINT>   lowSteps(lowVertexIndex);
		std::vector<dPOINT>   highSteps(highVertexIndex);
		std::vector<fPOINT>   lowVertices(lowVertexIndex);
		std::vector<dPOINT>   highVertices(highVertexIndex);
		size_t                lowIndex = 0, highIndex = 0;
		unsigned              selind    = 0;
		double                lowLength = 0.0, highLength = 0.0, length = 0.0;
		double                lowSpacing = 0.0, highSpacing = 0.0;
		unsigned              highCount = 0, lowCount = 0, count = 0;
		dPOINT                lowStep = {}, lowPoint = {};
		dPOINT                highStep = {}, highPoint = {};
		dPOINT                delta = {};
		// ToDo - rename pols, polref, polin & poldif
		std::vector<PVEC> pols(selectedVertexCount);
		PVEC              polref = {}, polin = {}, poldif = {};

		SequenceIndex = 0;
		for (auto iVertex = lowVertexIndex; iVertex != 0; iVertex--) {
			lowVertices[lowIndex] = CurrentFormVertices[iVertex];
			lowDeltas[lowIndex].x = CurrentFormVertices[iVertex - 1].x - CurrentFormVertices[iVertex].x;
			lowDeltas[lowIndex].y = CurrentFormVertices[iVertex - 1].y - CurrentFormVertices[iVertex].y;
			lowLengths[lowIndex]  = hypot(lowDeltas[lowIndex].x, lowDeltas[lowIndex].y);
			lowLength += lowLengths[lowIndex];
			lowIndex++;
		}
		selind = 0;
		for (auto iVertex = start + 1; iVertex <= finish; iVertex++) {
			delta.x             = CurrentFormVertices[iVertex].x - selectionStart[0].x;
			delta.y             = CurrentFormVertices[iVertex].y - selectionStart[0].y;
			pols[selind].length = hypot(delta.x, delta.y);
			pols[selind].angle  = atan2(delta.y, delta.x);
			selind++;
		}
		highIndex  = 0;
		highLength = 0;
		for (auto iVertex = finish; iVertex < VertexCount - 1; iVertex++) {
			highVertices[highIndex].x = CurrentFormVertices[iVertex].x;
			highVertices[highIndex].y = CurrentFormVertices[iVertex].y;
			highDeltas[highIndex].x   = CurrentFormVertices[iVertex + 1].x - CurrentFormVertices[iVertex].x;
			highDeltas[highIndex].y   = CurrentFormVertices[iVertex + 1].y - CurrentFormVertices[iVertex].y;
			highLengths[highIndex]    = hypot(highDeltas[highIndex].x, highDeltas[highIndex].y);
			highLength += highLengths[highIndex];
			highIndex++;
		}
		if (highLength > lowLength)
			length = lowLength;
		else
			length = highLength;
		count = length / SelectedForm->fillSpacing;
		if (highLength < lowLength) {
			lowSpacing  = SelectedForm->fillSpacing;
			highSpacing = SelectedForm->fillSpacing * highLength / lowLength;
		}
		else {
			highSpacing = SelectedForm->fillSpacing;
			lowSpacing  = SelectedForm->fillSpacing * lowLength / highLength;
		}
		for (size_t iVertex = 0u; iVertex < lowVertexIndex; iVertex++) {
			lowCounts[iVertex]  = lowLengths[iVertex] / lowSpacing;
			lowSteps[iVertex].x = lowDeltas[iVertex].x / lowCounts[iVertex];
			lowSteps[iVertex].y = lowDeltas[iVertex].y / lowCounts[iVertex];
		}
		for (size_t iVertex = 0u; iVertex < highVertexIndex; iVertex++) {
			highCounts[iVertex]  = highLengths[iVertex] / highSpacing;
			highSteps[iVertex].x = highDeltas[iVertex].x / highCounts[iVertex];
			highSteps[iVertex].y = highDeltas[iVertex].y / highCounts[iVertex];
		}
		lowIndex = highIndex = 0;
		StateMap.reset(StateFlag::FILDIR);
		lowCount = highCount = 0;
		delta.x              = CurrentFormVertices[finish].x - CurrentFormVertices[start].x;
		delta.y              = CurrentFormVertices[finish].y - CurrentFormVertices[start].y;
		polref.length        = hypot(delta.x, delta.y);
		polref.angle         = atan2(delta.y, delta.x);
		while (lowCount || (lowIndex < lowVertexIndex && highIndex < highVertexIndex)) {
			if (lowCount)
				lowCount--;
			else {
				if (lowIndex < lowVertexIndex) {
					lowCount   = lowCounts[lowIndex];
					lowStep    = lowSteps[lowIndex];
					lowPoint.x = lowVertices[lowIndex].x;
					lowPoint.y = lowVertices[lowIndex].y;
					lowIndex++;
				}
			}
			if (highCount)
				highCount--;
			else {
				if (highIndex < highVertexIndex) {
					highCount = highCounts[highIndex];
					highStep  = highSteps[highIndex];
					highPoint = highVertices[highIndex];
					highIndex++;
				}
			}
			delta.x      = highPoint.x - lowPoint.x;
			delta.y      = highPoint.y - lowPoint.y;
			polin.angle  = atan2(delta.y, delta.x);
			polin.length = hypot(delta.x, delta.y);
			poldif.angle = polin.angle - polref.angle;
			if (polref.length > 0.9 * LineSpacing) {
				poldif.length = polin.length / polref.length;
				if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					OSequence[SequenceIndex].x = lowPoint.x;
					OSequence[SequenceIndex].y = lowPoint.y;
					SequenceIndex++;
					for (size_t iVertex = 0; iVertex < (selectedVertexCount - 1); iVertex++) {
						length                     = pols[iVertex].length * poldif.length;
						const auto angle           = pols[iVertex].angle + poldif.angle;
						OSequence[SequenceIndex].x = lowPoint.x + cos(angle) * length;
						OSequence[SequenceIndex].y = lowPoint.y + sin(angle) * length;
						SequenceIndex++;
					}
				}
				else {
					OSequence[SequenceIndex].x = highPoint.x;
					OSequence[SequenceIndex].y = highPoint.y;
					SequenceIndex++;
					for (size_t iVertex = selectedVertexCount - 1; iVertex != 0; iVertex--) {
						length                     = pols[iVertex - 1].length * poldif.length;
						const auto angle           = pols[iVertex - 1].angle + poldif.angle;
						OSequence[SequenceIndex].x = lowPoint.x + cos(angle) * length;
						OSequence[SequenceIndex].y = lowPoint.y + sin(angle) * length;
						SequenceIndex++;
					}
				}
			}
			lowPoint.x += lowStep.x;
			lowPoint.y += lowStep.y;
			highPoint.x += highStep.x;
			highPoint.y += highStep.y;
		}
		if (StateMap.test(StateFlag::FILDIR)) {
			OSequence[SequenceIndex++] = CurrentFormVertices[0];
		}
		else {
			OSequence[SequenceIndex++] = CurrentFormVertices[VertexCount - 1];
		}
		if (SelectedForm->lengthOrCount.stitchLength < MinStitchLength)
			SelectedForm->lengthOrCount.stitchLength = MinStitchLength;
	}
}

void duflt() {
	unsigned iVertex  = 0;
	float    leftEdge = 1e9;

	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		if (CurrentFormVertices[iVertex].x < leftEdge)
			leftEdge = CurrentFormVertices[iVertex].x;
	}
	if (leftEdge < ClipRectSize.cx) {
		StateMap.set(StateFlag::WASNEG);
		FormOffset = ClipRectSize.cx + fabs(leftEdge) + .05;
		for (iVertex = 0; iVertex < VertexCount; iVertex++)
			CurrentFormVertices[iVertex].x += FormOffset;
		SelectedForm->rectangle.left += FormOffset;
		SelectedForm->rectangle.right += FormOffset;
	}
	else
		StateMap.reset(StateFlag::WASNEG);
}

constexpr unsigned leftsid() {
	unsigned iVertex = 0, leftVertex = 0;
	float    minimumX = 1e9;

	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		if (CurrentFormVertices[iVertex].x < minimumX) {
			minimumX   = CurrentFormVertices[iVertex].x;
			leftVertex = iVertex;
		}
	}
	return leftVertex;
}

bool clpcmp(const VCLPX& vclpx1, const VCLPX& vclpx2) noexcept {
	return (vclpx1.segment < vclpx2.segment);
}

bool isin(const std::vector<VCLPX> regionCrossingData,
          float                    xCoordinate,
          float                    yCoordinate,
          unsigned                 regionCrossingStart,
          unsigned                 regionCrossingEnd) {

	unsigned count = 0;
	dPOINT   point = {};

	if (xCoordinate < BoundingRect.left)
		return 0;
	if (xCoordinate > BoundingRect.right)
		return 0;
	if (yCoordinate < BoundingRect.bottom)
		return 0;
	if (yCoordinate > BoundingRect.top)
		return 0;
	for (auto iRegion = regionCrossingStart; iRegion < regionCrossingEnd; iRegion++) {
		const auto startVertex = regionCrossingData[iRegion].vertex;
		const auto endVertex = nxt(startVertex);
		if (projv(xCoordinate, CurrentFormVertices[startVertex], CurrentFormVertices[endVertex], point)) {
			if (point.y > yCoordinate) {
				if (CurrentFormVertices[startVertex].x != xCoordinate && CurrentFormVertices[endVertex].x != xCoordinate)
					count++;
				else {
					if (CurrentFormVertices[startVertex].x < CurrentFormVertices[endVertex].x) {
						if (CurrentFormVertices[endVertex].x != xCoordinate)
							count++;
					}
					else {
						if (CurrentFormVertices[startVertex].x != xCoordinate)
							count++;
					}
				}
			}
		}
	}
	return count & 1;
}

void inspnt(std::vector<CLIPNT>& clipStitchPoints) {
	CLIPNT clipStitchPoint{ clipStitchPoints.back().x, clipStitchPoints.back().y, 0, 0 };
	clipStitchPoints.back().x    = midl(clipStitchPoint.x, clipStitchPoints[clipStitchPoints.size() - 2].x);
	clipStitchPoints.back().y    = midl(clipStitchPoint.y, clipStitchPoints[clipStitchPoints.size() - 2].y);
	clipStitchPoints.back().flag = 1;
	clipStitchPoints.push_back(clipStitchPoint);
}

bool isect(size_t vertex0, size_t vertex1, fPOINT& intersection, float& length) noexcept {
	dPOINT delta            = { (LineSegmentEnd.x - LineSegmentStart.x), (LineSegmentEnd.y - LineSegmentStart.y) };
	dPOINT point            = { (LineSegmentStart.x), (LineSegmentStart.y) };
	dPOINT tempIntersection = {};
	bool   flag             = false;
	float  left             = 0.0;
	float  right            = 0.0;

	if (delta.x && delta.y)
		flag = proj(point, delta.y / delta.x, CurrentFormVertices[vertex0], CurrentFormVertices[vertex1], tempIntersection);
	else {
		if (delta.y)
			flag = projv(point.x, CurrentFormVertices[vertex0], CurrentFormVertices[vertex1], tempIntersection);
		else {
			if (delta.x)
				flag = projh(point.y, CurrentFormVertices[vertex0], CurrentFormVertices[vertex1], tempIntersection);
			else if (CurrentFormVertices[vertex0].y == LineSegmentStart.y
			         && CurrentFormVertices[vertex1].y == LineSegmentStart.y) {
				if (CurrentFormVertices[vertex0].x < CurrentFormVertices[vertex1].x) {
					left  = CurrentFormVertices[vertex0].x;
					right = CurrentFormVertices[vertex1].x;
				}
				else {
					left  = CurrentFormVertices[vertex1].x;
					right = CurrentFormVertices[vertex0].x;
				}
				if (LineSegmentStart.x > left && LineSegmentStart.x < right) {
					intersection.x = LineSegmentStart.x;
					intersection.y = LineSegmentStart.y;
					length         = 0;
					return true;
				}
				return false;
			}
			else
				return false;
		}
	}
	if (tempIntersection.x < TINY)
		tempIntersection.x = 0;
	if (tempIntersection.y < TINY)
		tempIntersection.y = 0;
	intersection.x = static_cast<float>(tempIntersection.x);
	intersection.y = static_cast<float>(tempIntersection.y);
	length         = hypot(tempIntersection.x - LineSegmentStart.x, tempIntersection.y - LineSegmentStart.y);
	// ToDo - should length be determined from start or end?
	//	 hypot(tipnt.x-LineSegmentEnd.x,tipnt.y-LineSegmentEnd.y);
	return flag;
}

bool lencmpa(const CLIPSORT* const arg1, const CLIPSORT* const arg2) noexcept {
	const float local1 = arg1->segmentLength, local2 = arg2->segmentLength;

	return (local1 < local2);
}

void mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, unsigned destination, unsigned source) {
	if (destination != source) {
		arrayOfClipIntersectData[destination] = arrayOfClipIntersectData[source];
	}
}

unsigned insect(std::vector<CLIPSORT>&    clipIntersectData,
                const std::vector<VCLPX>& regionCrossingData,
                std::vector<CLIPSORT*>&   arrayOfClipIntersectData,
                unsigned                  regionCrossingStart,
                unsigned                  regionCrossingEnd) {
	unsigned   iRegions = 0, iDestination = 0, iIntersection = 0, count = 0;
	size_t     currentVertex = 0, nextVertex = 0;
	fRECTANGLE lineSegmentRect = {};
	fPOINT*    intersection    = nullptr;

	if (LineSegmentEnd.x > LineSegmentStart.x) {
		lineSegmentRect.left  = LineSegmentStart.x;
		lineSegmentRect.right = LineSegmentEnd.x;
	}
	else {
		lineSegmentRect.left  = LineSegmentEnd.x;
		lineSegmentRect.right = LineSegmentStart.x;
	}
	if (LineSegmentEnd.y > LineSegmentStart.y) {
		lineSegmentRect.top    = LineSegmentEnd.y;
		lineSegmentRect.bottom = LineSegmentStart.y;
	}
	else {
		lineSegmentRect.top    = LineSegmentStart.y;
		lineSegmentRect.bottom = LineSegmentEnd.y;
	}
	iIntersection = count = 0;
	arrayOfClipIntersectData.clear();
	for (iRegions = regionCrossingStart; iRegions < regionCrossingEnd; iRegions++) {
		currentVertex = regionCrossingData[iRegions].vertex;
		nextVertex    = nxt(currentVertex);
		if (isect(
		        currentVertex, nextVertex, clipIntersectData[iIntersection].point, clipIntersectData[iIntersection].sideLength)) {
			intersection = &clipIntersectData[iIntersection].point;
			if (intersection->x >= lineSegmentRect.left && intersection->x <= lineSegmentRect.right
			    && intersection->y >= lineSegmentRect.bottom && intersection->y <= lineSegmentRect.top) {
				clipIntersectData[iIntersection].segmentLength
				    = hypot(clipIntersectData[iIntersection].point.x - LineSegmentStart.x,
				            clipIntersectData[iIntersection].point.y - LineSegmentStart.y);
				clipIntersectData[iIntersection].vertexIndex = currentVertex;
				arrayOfClipIntersectData.push_back(&clipIntersectData[iIntersection]);
				iIntersection++;
				count++;
			}
		}
	}
	if (count > 1) {
		std::sort(arrayOfClipIntersectData.begin(), arrayOfClipIntersectData.end(), lencmpa);
		iDestination = 1;
		for (iIntersection = 0; iIntersection < count - 1; iIntersection++) {
			if (fabs(arrayOfClipIntersectData[iIntersection]->segmentLength
			         - arrayOfClipIntersectData[iIntersection + 1]->segmentLength)
			    > TINY)
				mvpclp(arrayOfClipIntersectData, iDestination++, iIntersection + 1);
		}
		count = iDestination;
	}
	return count;
}

float getlen(std::vector<CLIPNT>& clipStitchPoints, const std::vector<double>& lengths, unsigned iPoint) {
	clipStitchPoints[iPoint].vertexIndex %= VertexCount;
	return lengths[clipStitchPoints[iPoint].vertexIndex]
	       + hypot(CurrentFormVertices[clipStitchPoints[iPoint].vertexIndex].x - clipStitchPoints[iPoint].x,
	               CurrentFormVertices[clipStitchPoints[iPoint].vertexIndex].y - clipStitchPoints[iPoint].y);
}

unsigned clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
                 std::vector<CLPSEG>&       clipSegments,
                 const std::vector<double>& lengths,
                 unsigned                   start,
                 unsigned                   finish) {
	CLPSEG clipSegment;

	clipSegment.start       = start;
	clipSegment.beginLength = getlen(clipStitchPoints, lengths, start);
	clipSegment.asid        = clipStitchPoints[start].vertexIndex;
	clipSegment.endLength   = getlen(clipStitchPoints, lengths, finish);
	clipSegment.zsid        = clipStitchPoints[finish].vertexIndex;
	clipSegment.finish      = finish;
	clipSegment.dun         = 0;
	clipSegments.push_back(clipSegment);
	return finish + 1;
}

bool lencmp(const LENINFO& arg1, const LENINFO& arg2) noexcept {
	return (arg1.length < arg2.length);
}

void chksid(size_t vertexIndex) noexcept {
	size_t iVertex = 0;
	size_t limit   = 0;

	if (ClipIntersectSide != vertexIndex) {
		if ((vertexIndex - ClipIntersectSide + VertexCount) % VertexCount < (VertexCount >> 1)) {
			iVertex = nxt(ClipIntersectSide);
			limit   = nxt(vertexIndex);
			while (iVertex != limit) {
				OSequence[SequenceIndex++] = CurrentFormVertices[iVertex];
				iVertex                    = nxt(iVertex);
			}
		}
		else {
			iVertex = ClipIntersectSide;
			while (iVertex != vertexIndex) {
				OSequence[SequenceIndex++] = CurrentFormVertices[iVertex];
				iVertex                    = prv(iVertex);
			}
		}
	}
}

void ritseg(const std::vector<CLIPNT>& clipStitchPoints, std::vector<CLPSEG>& clipSegments, const unsigned currentSegmentIndex) {
	unsigned iPoint       = 0;
	bool     isPointedEnd = true;

	if (SelectedForm->extendedAttribute & AT_SQR)
		isPointedEnd = false;
	if (StateMap.test(StateFlag::FILDIR)) {
		iPoint = clipSegments[currentSegmentIndex].start;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd)
			iPoint++;
		chksid(clipSegments[currentSegmentIndex].asid);
		while (iPoint <= clipSegments[currentSegmentIndex].finish) {
			OSequence[SequenceIndex].x   = clipStitchPoints[iPoint].x;
			OSequence[SequenceIndex++].y = clipStitchPoints[iPoint++].y;
		}
		ClipIntersectSide = clipSegments[currentSegmentIndex].zsid;
	}
	else {
		iPoint = clipSegments[currentSegmentIndex].finish;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd)
			iPoint--;
		chksid(clipSegments[currentSegmentIndex].zsid);
		if (clipSegments[currentSegmentIndex].start) {
			while (iPoint >= clipSegments[currentSegmentIndex].start) {
				OSequence[SequenceIndex].x   = clipStitchPoints[iPoint].x;
				OSequence[SequenceIndex++].y = clipStitchPoints[iPoint--].y;
			}
		}
		else {
			while (iPoint < clipSegments[currentSegmentIndex].start) {
				OSequence[SequenceIndex].x   = clipStitchPoints[iPoint].x;
				OSequence[SequenceIndex++].y = clipStitchPoints[iPoint--].y;
			}
		}
		ClipIntersectSide = clipSegments[currentSegmentIndex].asid;
	}
	clipSegments[currentSegmentIndex].dun = 1;
}

bool clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned sind) {
	// ToDo - rename local variables

	unsigned     ind          = 1;
	const size_t indexDoubled = clipSegments.size() << 1;

	StateMap.reset(StateFlag::FILDIR);
	while (ind < clipSegments.size()) {
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			OutputIndex = (sind + ind) % indexDoubled;
			if (!clipSegments[sortedLengths[OutputIndex].index].dun)
				return 0;
			ind++;
		}
		else {
			OutputIndex = (sind + indexDoubled - ind) % indexDoubled;
			if (!clipSegments[sortedLengths[OutputIndex].index].dun)
				return 0;
		}
	}
	return 1;
}

bool nucseg(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned& currentSegmentIndex) {
	unsigned ind = 0;

	if (StateMap.test(StateFlag::FILDIR))
		ind = clipSegments[currentSegmentIndex].endIndex;
	else
		ind = clipSegments[currentSegmentIndex].beginIndex;
	if (clpnxt(clipSegments, sortedLengths, ind))
		return false;
	if (sortedLengths[OutputIndex].isEnd)
		StateMap.reset(StateFlag::FILDIR);
	else
		StateMap.set(StateFlag::FILDIR);
	currentSegmentIndex = sortedLengths[OutputIndex].index;
	return true;
}

bool vscmp(unsigned index1, unsigned index2) noexcept {
	if (OSequence[index1].x != OSequence[index2].x)
		return 1;

	return OSequence[index1].y != OSequence[index2].y ? 1 : 0;
}

void clpcon(const std::vector<RNGCNT>& textureSegments) {
	RECT     clipGrid = {};
	unsigned iSegment = 0, iStitchPoint = 0, iVertex = 0, iPoint = 0, iSorted = 0, iSequence = 0;
	size_t   vertex = 0, nextVertex = 0;
	unsigned iRegion = 0, ine = 0, regionSegment = 0, iStitch = 0;
	unsigned lineOffset = 0, negativeOffset = 0, clipNegative = 0;
	unsigned start = 0, finish = 0, segmentCount = 0, previousPoint = 0;
	// ToDo - rename variables
	unsigned inf = 0, ing = 0, cnt = 0;
	int      iVerticalGrid = 0, textureLine = 0;
	fPOINT   pasteLocation = {};
	double   totalLength = 0.0, minx = 0.0;
	float    formNegativeOffset  = 0.0;
	size_t   clipGridOffset      = 0;
	double   clipVerticalOffset  = 0.0;
	TXPNT*   texture             = nullptr;
	size_t   clplim              = 0; // vertical clipboard search limit
	unsigned regionCrossingStart = 0; // start of region crossing data for a particular region
	unsigned regionCrossingEnd   = 0; // end of region crossing data for a particular region

	duflt();
	ClipWidth = ClipRectSize.cx + SelectedForm->fillSpacing;
	if (StateMap.test(StateFlag::ISUND))
		ClipWidth = SelectedForm->underlaySpacing;
	if (SelectedForm->fillSpacing < 0)
		clipNegative = 1;
	else
		clipNegative = 0;
	if (ClipWidth < CLPMINAUT)
		ClipWidth = CLPMINAUT;
	if (StateMap.test(StateFlag::TXFIL)) {
		if (TextureIndex
		    && SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count
		           <= gsl::narrow<unsigned short>(TextureIndex))
			ClipWidth = SelectedForm->fillSpacing;
		else {
			return;
		}
	}
	std::vector<double>    lengths(VertexCount);
	std::vector<double>    clipSideLengths(VertexCount);   // lengths of form sides for clipboard fill
	std::vector<CLIPSORT>  clipIntersectData(VertexCount); // intersect points for clipboard fill
	std::vector<CLIPSORT*> arrayOfClipIntersectData;
	arrayOfClipIntersectData.reserve(VertexCount);
	vertex      = leftsid();
	totalLength = 0;
	vertex      = nxt(vertex);
	for (iVertex = 0; iVertex <= VertexCount; iVertex++) {
		nextVertex              = nxt(vertex);
		lengths[vertex]         = totalLength;
		clipSideLengths[vertex] = hypot(CurrentFormVertices[nextVertex].x - CurrentFormVertices[vertex].x,
		                                CurrentFormVertices[nextVertex].y - CurrentFormVertices[vertex].y);
		totalLength += clipSideLengths[vertex];
		vertex = nextVertex;
	}
	clipGrid.left   = floor(SelectedForm->rectangle.left / ClipWidth);
	clipGrid.right  = ceil(SelectedForm->rectangle.right / ClipWidth);
	clipGrid.bottom = floor(SelectedForm->rectangle.bottom / ClipRectSize.cy - 1);
	clipGrid.top    = ceil(SelectedForm->rectangle.top / ClipRectSize.cy + 1) + 2;
	negativeOffset  = 0;
	if (SelectedForm->wordParam > 1)
		clipGridOffset = SelectedForm->wordParam;
	else
		clipGridOffset = 0;
	if (clipGridOffset) {
		clipGrid.top++;
		if (SelectedForm->fillSpacing < 0) {
			clipGrid.bottom--;
			clipGrid.left -= gsl::narrow<long>(round(ClipRectSize.cx / ClipWidth));
			clipGrid.right += gsl::narrow<long>(round(ClipRectSize.cx / ClipWidth));
		}
	}
	if (clipNegative && !clipGridOffset)
		clipGrid.left -= gsl::narrow<long>(round(ClipRectSize.cx / ClipWidth));
	if (clipGrid.bottom < 0) {
		negativeOffset = 1 - clipGrid.bottom;
		clipGrid.bottom += negativeOffset;
		clipGrid.top += negativeOffset;
		formNegativeOffset = ClipRectSize.cy * negativeOffset;
		for (iVertex = 0; iVertex < VertexCount; iVertex++)
			CurrentFormVertices[iVertex].y += formNegativeOffset;
	}
	segmentCount = 0;
	std::vector<VCLPX> regionCrossingData; // region crossing data for vertical clipboard fills
	regionCrossingData.reserve(MAXFRMLINS);
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		start  = floor(CurrentFormVertices[iVertex].x / ClipWidth);
		finish = floor((CurrentFormVertices[nxt(iVertex)].x) / ClipWidth);
		if (start > finish) {
			const unsigned int swap = start;
			start                   = finish;
			finish                  = swap;
		}
		if (SelectedForm->fillSpacing < 0)
			finish += ClipRectSize.cx / ClipWidth;
		if (finish > gsl::narrow<unsigned>(clipGrid.right))
			finish = gsl::narrow<unsigned>(clipGrid.right);
		if (clipNegative)
			start -= static_cast<unsigned int>(ClipRectSize.cx / ClipWidth);
		for (iSegment = start; iSegment <= finish; iSegment++) {
			regionCrossingData.push_back({ iSegment, iVertex });
		}
	}
	std::sort(regionCrossingData.begin(), regionCrossingData.end(), clpcmp);
	std::vector<unsigned> iclpx;
	iclpx.reserve(regionCrossingData.size());
	regionSegment = regionCrossingData[0].segment;
	iclpx.push_back(0);
	for (iSegment = 1; iSegment < regionCrossingData.size(); iSegment++) {
		if (regionCrossingData[iSegment].segment != regionSegment) {
			iclpx.push_back(iSegment);
			regionSegment = regionCrossingData[iSegment].segment;
		}
	}
	iclpx.push_back(iSegment);
	BoundingRect.left = BoundingRect.right = CurrentFormVertices[0].x;
	BoundingRect.top = BoundingRect.bottom = CurrentFormVertices[0].y;
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		if (CurrentFormVertices[iVertex].x > BoundingRect.right)
			BoundingRect.right = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].x < BoundingRect.left)
			BoundingRect.left = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].y > BoundingRect.top)
			BoundingRect.top = CurrentFormVertices[iVertex].y;
		if (CurrentFormVertices[iVertex].y < BoundingRect.bottom)
			BoundingRect.bottom = CurrentFormVertices[iVertex].y;
	}
	ActivePointIndex = 0;
	std::vector<CLIPNT> clipStitchPoints;
	// ToDo - find a better number than MAXITEMS
	clipStitchPoints.reserve(MAXITEMS);
	bool breakFlag = false;
	for (iRegion = 0; iRegion < (iclpx.size() - 1); iRegion++) {
		regionCrossingStart = iclpx[iRegion];
		regionCrossingEnd   = iclpx[iRegion + 1];
		pasteLocation.x     = ClipWidth * (iRegion + clipGrid.left);
		clipVerticalOffset  = 0;
		if (StateMap.test(StateFlag::TXFIL)) {
			textureLine     = (iRegion + clipGrid.left) % SelectedForm->fillInfo.texture.lines;
			ClipStitchCount = textureSegments[textureLine].stitchCount;
			texture         = &TexturePointsBuffer->at(SelectedForm->fillInfo.texture.index + textureSegments[textureLine].line);
			LineSegmentStart.x = pasteLocation.x;
			if (SelectedForm->txof) {
				lineOffset         = (iRegion + clipGrid.left) / SelectedForm->fillInfo.texture.lines;
				clipVerticalOffset = fmod(SelectedForm->txof * lineOffset, SelectedForm->fillInfo.texture.height);
			}
		}
		else {
			if (clipGridOffset) {
				clipVerticalOffset = static_cast<float>(iRegion % clipGridOffset) / (clipGridOffset * ClipRectSize.cy);
			}
			LineSegmentStart.x = pasteLocation.x + ClipBuffer[0].x;
		}
		LineSegmentStart.y = clipGrid.bottom * ClipRectSize.cy;
		if (clipGridOffset) {
			clipVerticalOffset = static_cast<float>(iRegion % clipGridOffset) / (clipGridOffset * ClipRectSize.cy);
		}
		for (iVerticalGrid = clipGrid.bottom; iVerticalGrid < clipGrid.top; iVerticalGrid++) {
			pasteLocation.y  = iVerticalGrid * ClipRectSize.cy - clipVerticalOffset;
			LineSegmentEnd.x = pasteLocation.x + ClipBuffer[0].x;
			LineSegmentEnd.y = pasteLocation.y + ClipBuffer[0].y;
			if (!clipStitchPoints.size()) {
				LineSegmentStart.x = LineSegmentEnd.x;
				LineSegmentStart.y = LineSegmentEnd.y;
			}
			for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
				if (StateMap.test(StateFlag::TXFIL)) {
					if (texture != nullptr) {
						LineSegmentEnd.x                          = pasteLocation.x;
						[[gsl::suppress(26413)]] LineSegmentEnd.y = pasteLocation.y + texture[iStitch].y;
					}
				}
				else {
					LineSegmentEnd.x = pasteLocation.x + ClipBuffer[iStitch].x;
					LineSegmentEnd.y = pasteLocation.y + ClipBuffer[iStitch].y;
				}

				clipStitchPoints.push_back({ LineSegmentStart.x, LineSegmentStart.y, 0, 0 });
				if (isin(regionCrossingData, LineSegmentStart.x, LineSegmentStart.y, regionCrossingStart, regionCrossingEnd)) {
					if ((clipStitchPoints.size() > 1) && clipStitchPoints[clipStitchPoints.size() - 1].flag == 2) {
						inspnt(clipStitchPoints);
					}
					clipStitchPoints.back().flag = 0;
				}
				else {
					if ((clipStitchPoints.size() > 1) && !clipStitchPoints[clipStitchPoints.size() - 1].flag) {
						inspnt(clipStitchPoints);
					}
					clipStitchPoints.back().flag = 2;
				}
				cnt = insect(
				    clipIntersectData, regionCrossingData, arrayOfClipIntersectData, regionCrossingStart, regionCrossingEnd);
				if (cnt) {
					for (ing = 0; ing < cnt; ing++) {
						clipStitchPoints.push_back({ arrayOfClipIntersectData[ing]->point.x,
						                             arrayOfClipIntersectData[ing]->point.y,
						                             arrayOfClipIntersectData[ing]->vertexIndex,
						                             1 });
						if (clipStitchPoints.size() > MAXITEMS << 2) {
							breakFlag = true;
							break;
						}
					}
					if (breakFlag) {
						break;
					}
				}
				LineSegmentStart = LineSegmentEnd;
			}
			if (breakFlag) {
				break;
			}
		}
		if (!breakFlag) {
			if (clipStitchPoints.size()) {
				clipStitchPoints[clipStitchPoints.size() - 1].flag = 2;
			};
		}
		else {
			break;
		}
	}
	if (clipStitchPoints.size()) {
		clipStitchPoints[clipStitchPoints.size() - 1].flag = 2;
	};
	if (negativeOffset) {
		formNegativeOffset = negativeOffset * ClipRectSize.cy;
		for (iStitchPoint = 0; iStitchPoint < clipStitchPoints.size(); iStitchPoint++)
			clipStitchPoints[iStitchPoint].y -= formNegativeOffset;
		for (iVertex = 0; iVertex < VertexCount; iVertex++)
			CurrentFormVertices[iVertex].y -= formNegativeOffset;
	}
#define CLPVU 0

#if CLPVU == 1

	goto clp1skp;

#endif

	StateMap.reset(StateFlag::FILDIR);
	previousPoint = 0;
	std::vector<CLPSEG> clipSegments;
	if (clipStitchPoints.size()) {
		// reserve a reasonable amount but not the full amount potentially required
		clipSegments.reserve(clipStitchPoints.size() / 10);

		for (iPoint = 0; iPoint < clipStitchPoints.size() - 1; iPoint++) {
			switch (clipStitchPoints[iPoint].flag) {
			case 0: // inside
				StateMap.set(StateFlag::FILDIR);
				break;
			case 1: // line
				if (StateMap.testAndFlip(StateFlag::FILDIR))
					clpnseg(clipStitchPoints, clipSegments, lengths, previousPoint, iPoint);
				else
					previousPoint = iPoint;
				break;
			case 2: // outside
				StateMap.reset(StateFlag::FILDIR);
				break;
			}
		}
	}

#if CLPVU == 1

clp1skp:;

#endif

	if (clipSegments.size()) {
		clplim = clipSegments.size() >> 3;
		clplim = clipSegments.size() >> 1;
		if (!clplim)
			clplim = 1;
		if (clplim > 12)
			clplim = 12;
		std::vector<LENINFO> sortedLengths;
		sortedLengths.reserve(clipSegments.size() * 2);
		for (iSegment = 0; iSegment < clipSegments.size(); iSegment++) {
			sortedLengths.push_back({ iSegment, false, clipSegments[iSegment].beginLength });
			sortedLengths.push_back({ iSegment, true, clipSegments[iSegment].endLength });
		}
		std::sort(sortedLengths.begin(), sortedLengths.end(), lencmp);
		for (iSorted = 0; iSorted < sortedLengths.size(); iSorted++) {
			if (sortedLengths[iSorted].isEnd)
				clipSegments[sortedLengths[iSorted].index].endIndex = iSorted;
			else
				clipSegments[sortedLengths[iSorted].index].beginIndex = iSorted;
		}

#if CLPVU == 1

		for (unsigned iStitch = 0; iStitch < ActivePointIndex; iStitch++) {
			StitchBuffer[iStitch].x         = ClipStitchPoints[iStitch].x;
			StitchBuffer[iStitch].y         = ClipStitchPoints[iStitch].y;
			StitchBuffer[iStitch].attribute = 0;
		}
		PCSHeader.stitchCount = ActivePointIndex;
#endif

#if CLPVU == 2

		inf = 0;
		for (iSegment = 0; iSegment < ClipSegmentIndex; iSegment++) {
			for (iStitchPoint = clipSegments[iSegment].start; iStitchPoint <= clipSegments[iSegment].finish; iStitchPoint++) {
				StitchBuffer[inf].x           = ClipStitchPoints[iStitchPoint].x;
				StitchBuffer[inf].y           = ClipStitchPoints[iStitchPoint].y;
				StitchBuffer[inf++].attribute = iSegment & 0xf;
			}
		}
		PCSHeader.stitchCount = inf;

#endif

		minx = 1e99;

#if CLPVU == 0

		unsigned currentSegmentIndex = 0;
		StateMap.set(StateFlag::FILDIR);
		SequenceIndex     = 0;
		ClipIntersectSide = clipSegments[0].asid;
		ritseg(clipStitchPoints, clipSegments, currentSegmentIndex);
		while (nucseg(clipSegments, sortedLengths, currentSegmentIndex)) {
			if (SequenceIndex > MAXITEMS - 3)
				break;
			ritseg(clipStitchPoints, clipSegments, currentSegmentIndex);
		}
		chksid(0);
		if (SequenceIndex > MAXITEMS - 100)
			SequenceIndex = MAXITEMS - 100;
		ine = 0;
		inf = 0;
		for (iSequence = 0; iSequence < SequenceIndex; iSequence++) {
			if (vscmp(iSequence, ine)) {
				ine++;
				OSequence[ine] = OSequence[iSequence];
			}
			else
				inf++;
		}
		SequenceIndex = ine;
		if (StateMap.test(StateFlag::WASNEG)) {
			for (iSequence = 0; iSequence < SequenceIndex; iSequence++)
				OSequence[iSequence].x -= FormOffset;
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
				CurrentFormVertices[iVertex].x -= FormOffset;
			SelectedForm->rectangle.left -= FormOffset;
			SelectedForm->rectangle.right -= FormOffset;
		}
#endif
	}
}

void angout() noexcept {
	fRECTANGLE* rectangle = nullptr;
	unsigned    iVertex   = 1;

	if (AngledForm.vertexCount) {
		rectangle           = &AngledForm.rectangle;
		CurrentFormVertices = AngledForm.vertices;
		rectangle->left = rectangle->right = CurrentFormVertices[0].x;
		rectangle->bottom = rectangle->top = CurrentFormVertices[0].y;
		for (iVertex = 1; iVertex < AngledForm.vertexCount; iVertex++) {
			if (CurrentFormVertices[iVertex].x > rectangle->right)
				rectangle->right = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].x < rectangle->left)
				rectangle->left = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].y < rectangle->bottom)
				rectangle->bottom = CurrentFormVertices[iVertex].y;
			if (CurrentFormVertices[iVertex].y > rectangle->top)
				rectangle->top = CurrentFormVertices[iVertex].y;
		}
	}
}

void horclpfn(const std::vector<RNGCNT>& textureSegments) {
	unsigned iVertex = 0;

	AngledForm = FormList[ClosestFormToCursor];
	dPOINT rotationCenter
	    = { (static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left),
		    (static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom) };
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex] = SelectedForm->vertices[iVertex];
		rotflt(AngledForm.vertices[iVertex], (PI / 2), rotationCenter);
	}
	angout();
	SelectedForm        = &AngledForm;
	CurrentFormVertices = AngledForm.vertices;
	clpcon(textureSegments);
	rotbak((-PI / 2), rotationCenter);
	fvars(ClosestFormToCursor);
}

void angclpfn(const std::vector<RNGCNT>& textureSegments) {
	unsigned iVertex       = 0;
	double   rotationAngle = 0;

	AngledForm = FormList[ClosestFormToCursor];
	dPOINT rotationCenter
	    = { (static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left),
		    (static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom) };
	AngledForm.vertices = AngledFormVertices;
	if (StateMap.test(StateFlag::ISUND)) {
		rotationAngle                         = PI / 2 - SelectedForm->underlayStitchAngle;
		const std::vector<fPOINT>& vertexList = insid();
		for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex] = vertexList[iVertex];
			rotflt(AngledFormVertices[iVertex], rotationAngle, rotationCenter);
		}
	}
	else {
		if (StateMap.test(StateFlag::TXFIL))
			rotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
		else
			rotationAngle = PI / 2 - SelectedForm->satinOrAngle.angle;
		for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex] = SelectedForm->vertices[iVertex];
			rotflt(AngledFormVertices[iVertex], rotationAngle, rotationCenter);
		}
	}
	angout();
	SelectedForm        = &AngledForm;
	CurrentFormVertices = AngledFormVertices;
	clpcon(textureSegments);
	rotbak(-rotationAngle, rotationCenter);
	fvars(ClosestFormToCursor);
}

bool sqcomp(const SMALPNTL* arg1, const SMALPNTL* arg2) noexcept {
	if (arg1 && arg2) {
		const SMALPNTL lineEnd1 = *arg1;
		const SMALPNTL lineEnd2 = *arg2;

		if (lineEnd1.line == lineEnd2.line) {
			if (lineEnd1.group == lineEnd2.group) {
				if (lineEnd1.y == lineEnd2.y) {
					return false;
				}
				else {
					return (lineEnd1.y < lineEnd2.y);
				}
			}
			else {
				return (lineEnd1.group < lineEnd2.group);
			}
		}
		else {
			return (lineEnd1.line < lineEnd2.line);
		}
	}
	return false;
}

unsigned short isclos(const SMALPNTL* const lineEndPoint0, const SMALPNTL* const lineEndPoint1) noexcept {
	if (lineEndPoint0 && lineEndPoint1) {
		const float low0  = lineEndPoint0[0].y - GapToClosestRegion;
		const float high0 = lineEndPoint0[1].y + GapToClosestRegion;
		const float low1  = lineEndPoint1[0].y - GapToClosestRegion;
		const float high1 = lineEndPoint1[1].y + GapToClosestRegion;

		if (high0 < low1)
			return 0;
		if (high1 < low0)
			return 0;
	}
	return 1;
}

bool lnclos(std::vector<unsigned>& groupIndexSequence,
            std::vector<SMALPNTL>& lineEndpoints,
            unsigned               group0,
            unsigned               line0,
            unsigned               group1,
            unsigned               line1) {
	unsigned        index0 = 0, index1 = 0;
	unsigned        count0        = (groupIndexSequence[group0 + 1] - groupIndexSequence[group0]) >> 1;
	unsigned        count1        = 0;
	const SMALPNTL* lineEndPoint0 = &lineEndpoints[groupIndexSequence[group0]];

	if (group1 > GroupIndexCount - 2)
		return 0;
	if (group0 == 0)
		return 0;
	if (lineEndPoint0) {
		while (count0 && lineEndPoint0[index0].line != line0) {
			count0--;
			index0 += 2;
		}
		if (count0) {
			count1 = (groupIndexSequence[group1 + 1] - groupIndexSequence[group1]) >> 1;
			index1 = 0;
			if (const SMALPNTL* lineEndPoint1 = &lineEndpoints[groupIndexSequence[group1]]) {
				while (count1 && lineEndPoint1[index1].line != line1) {
					count1--;
					index1 += 2;
				}
				if (count1) {
					if (isclos(&lineEndPoint0[index0], &lineEndPoint1[index1]))
						return 1;
					else
						return 0;
				}
			}
		}
	}
	return 0;
}

bool regclos(std::vector<unsigned>&        groupIndexSequence,
             std::vector<SMALPNTL>&        lineEndpoints,
             const std::vector<SMALPNTL*>& sortedLines,
             unsigned                      iRegion0,
             unsigned                      iRegion1,
             const std::vector<REGION>&    regionsList) {
	// ToDo - More renaming required

	const SMALPNTL* lineEndPoint0Start = sortedLines[regionsList[iRegion0].start];
	SMALPNTL*       lineEndPoint0End   = nullptr;
	const SMALPNTL* lineEndPoint1Start = sortedLines[regionsList[iRegion1].start];
	SMALPNTL*       lineEndPoint1End   = nullptr;
	const unsigned  group0Start        = lineEndPoint0Start->group;
	unsigned        group0End          = 0;
	const unsigned  group1Start        = lineEndPoint1Start->group;
	unsigned        group1End          = 0;
	unsigned        groupStart         = 0;
	unsigned        groupEnd           = 0;
	unsigned        lineStart = 0, lineEnd = 0;
	unsigned        prevLine = 0, lastLine = 0;

	if (group0Start > group1Start) {
		groupStart = group0Start;
		lineStart  = lineEndPoint0Start->line;
		prevLine   = lineEndPoint1Start->line;
	}
	else {
		groupStart = group1Start;
		lineStart  = lineEndPoint1Start->line;
		prevLine   = lineEndPoint0Start->line;
	}
	if (groupStart && lnclos(groupIndexSequence, lineEndpoints, groupStart - 1, prevLine, groupStart, lineStart)) {
		NextGroup = groupStart;
		return 1;
	}
	else {
		lineEndPoint0End = sortedLines[regionsList[iRegion0].end];
		lineEndPoint1End = sortedLines[regionsList[iRegion1].end];
		group1End        = lineEndPoint1End->group;
		group0End        = lineEndPoint0End->group;
		if (group0End < group1End) {
			groupEnd = group0End;
			lineEnd  = lineEndPoint0End->line;
			lastLine = lineEndPoint1End->line;
		}
		else {
			groupEnd = group1End;
			lineEnd  = lineEndPoint1End->line;
			lastLine = lineEndPoint0End->line;
		}
		if (lnclos(groupIndexSequence, lineEndpoints, groupEnd, lineEnd, groupEnd + 1, lastLine)) {
			NextGroup = groupEnd;
			return 1;
		}
	}
	if (((group0Start > group1Start) ? (group0Start - group1Start) : (group1Start - group0Start)) < 2) {
		if (isclos(lineEndPoint0Start, lineEndPoint1Start)) {
			NextGroup = group0Start;
			return 1;
		}
	}
	if (((group0Start > group1End) ? (group0Start - group1End) : (group1End - group0Start)) < 2) {
		if (isclos(lineEndPoint0Start, lineEndPoint1End)) {
			NextGroup = group0Start;
			return 1;
		}
	}
	if (((group0End > group1Start) ? (group0End - group1Start) : (group1Start - group0End)) < 2) {
		if (isclos(lineEndPoint0End, lineEndPoint1Start)) {
			NextGroup = group0End;
			return 1;
		}
	}
	if (((group0End > group1End) ? (group0End - group1End) : (group1End - group0End)) < 2) {
		if (isclos(lineEndPoint0End, lineEndPoint1End)) {
			NextGroup = group0End;
			return 1;
		}
	}
	return 0;
}

bool unvis(const boost::dynamic_bitset<>& visitedRegions) {
	for (VisitedIndex = 0; VisitedIndex < RegionCount; VisitedIndex++) {
		if (!visitedRegions[VisitedIndex])
			return true;
	}
	return false;
}

unsigned notdun(std::vector<RGSEQ>&            tempPath,
                const std::vector<RCON>&       pathMap,
                const std::vector<unsigned>&   mapIndexSequence,
                const boost::dynamic_bitset<>& VisitedRegions,
                unsigned                       level) {
	unsigned iPath         = 1;
	int      pivot         = 0;
	int      previousLevel = level - 1;

	RegionPath          = &tempPath[SequencePathIndex];
	RegionPath[0].pcon  = mapIndexSequence[DoneRegion];
	RegionPath[0].count = mapIndexSequence[DoneRegion + 1] - RegionPath[0].pcon;
	for (iPath = 1; iPath < level; iPath++) {
		RegionPath[iPath].pcon  = mapIndexSequence[pathMap[RegionPath[iPath - 1].pcon].node];
		RegionPath[iPath].count = mapIndexSequence[pathMap[RegionPath[iPath - 1].pcon].node + 1] - RegionPath[iPath].pcon;
	}
	while (VisitedRegions[pathMap[RegionPath[previousLevel].pcon].node] && previousLevel >= 0) {
		if (--RegionPath[previousLevel].count > 0) {
			RegionPath[previousLevel].pcon++;
		}
		else {
			pivot = previousLevel;
			do {
				pivot--;
				if (pivot < 0) {
					return 1;
				}
				RegionPath[pivot].count--;
				RegionPath[pivot].pcon++;
			} while (!RegionPath[pivot].count);
			if (pivot < 0) {
				return 1;
			}
			pivot++;
			while (pivot <= previousLevel) {
				if (pivot) {
					RegionPath[pivot].pcon = mapIndexSequence[pathMap[RegionPath[pivot - 1].pcon].node];
					RegionPath[pivot].count
					    = mapIndexSequence[pathMap[RegionPath[pivot - 1].pcon].node + 1] - RegionPath[pivot].pcon;
				}
				else {
					if (--RegionPath[0].count) {
						RegionPath[0].pcon++;
					}
					else {
						return 1;
					}
				}
				pivot++;
			}
		}
	}
	return 0;
}

double reglen(const std::vector<SMALPNTL*>& sortedLines,
              unsigned                      iRegion,
              const std::vector<fPOINT>&    lastRegionCorners,
              const std::vector<REGION>&    regionsList) {
	double                 length = 0.0, minimumLength = 1e99;
	unsigned               iCorner = 0, iPoint = 0;
	std::vector<SMALPNTL*> lineEndPoints(4);

	lineEndPoints[0] = sortedLines[regionsList[iRegion].start];
	lineEndPoints[1] = &sortedLines[regionsList[iRegion].start][1];
	lineEndPoints[2] = sortedLines[regionsList[iRegion].end];
	lineEndPoints[3] = &sortedLines[regionsList[iRegion].end][1];
	for (iCorner = 0; iCorner < 4; iCorner++) {
		for (iPoint = 0; iPoint < 4; iPoint++) {
			length = hypot(lastRegionCorners[iCorner].x - lineEndPoints[iPoint]->x,
			               lastRegionCorners[iCorner].y - lineEndPoints[iPoint]->y);
			if (length < minimumLength) {
				minimumLength = length;
			}
		}
	}
	return minimumLength;
}

void nxtrgn(std::vector<RGSEQ>&           tempPath,
            const std::vector<RCON>&      pathMap,
            const std::vector<unsigned>&  mapIndexSequence,
            boost::dynamic_bitset<>&      visitedRegions,
            const std::vector<SMALPNTL*>& sortedLines,
            const std::vector<REGION>&    regionsList) {
	unsigned iRegion = 0, iPath = 0, newRegion = 0;
	double   length = 0, minimumLength = 1e99;
	unsigned pathLength = 1; // length of the path to the region

	std::vector<fPOINT> lastRegionCorners(4); // corners of last region sequenced

	while (notdun(tempPath, pathMap, mapIndexSequence, visitedRegions, pathLength)) {
		pathLength++;
		if (pathLength > 8) {
			SMALPNTL* lineEndPoint = sortedLines[regionsList[DoneRegion].start];
			if (lineEndPoint) {
				lastRegionCorners[0] = lineEndPoint[0];
				lastRegionCorners[1] = lineEndPoint[1];
			}
			lineEndPoint = sortedLines[regionsList[DoneRegion].end];
			if (lineEndPoint) {
				lastRegionCorners[2] = lineEndPoint[0];
				lastRegionCorners[3] = lineEndPoint[1];
			}
			newRegion = 0;
			for (iRegion = 0; iRegion < RegionCount; iRegion++) {
				if (!visitedRegions[iRegion]) {
					length = reglen(sortedLines, iRegion, lastRegionCorners, regionsList);
					if (length < minimumLength) {
						minimumLength = length;
						newRegion     = iRegion;
					}
				}
			}
			tempPath[SequencePathIndex].skp = true;
			for (iPath = 0; iPath < PathMapIndex; iPath++) {
				if (pathMap[iPath].node == newRegion) {
					tempPath[SequencePathIndex++].pcon = iPath;
					visitedRegions.set(newRegion);
					DoneRegion = newRegion;
					return;
				}
			}
			tempPath[SequencePathIndex].count  = VisitedIndex;
			tempPath[SequencePathIndex++].pcon = 0xffffffff;
			visitedRegions.set(VisitedIndex);
			DoneRegion = VisitedIndex;
			return;
		}
	}
	for (iPath = 0; iPath < pathLength; iPath++) {
		tempPath[SequencePathIndex].skp    = false;
		tempPath[SequencePathIndex++].pcon = RegionPath[iPath].pcon;
		visitedRegions.set(pathMap[RegionPath[iPath].pcon].node);
	}
	DoneRegion = pathMap[RegionPath[iPath - 1].pcon].node;
}

void nxtseq(std::vector<FSEQ>&           sequencePath,
            const std::vector<RCON>&     pathMap,
            const std::vector<unsigned>& mapIndexSequence,
            unsigned                     pathIndex) {
	unsigned iPath    = mapIndexSequence[sequencePath[pathIndex].node];
	unsigned nextNode = 0;
	if ((pathIndex + 1) < sequencePath.size()) {
		nextNode = sequencePath[pathIndex + 1].node;
		while (iPath < mapIndexSequence[sequencePath[pathIndex].node + 1] && pathMap[iPath].node != nextNode) {
			iPath++;
		}
		sequencePath[PathIndex++].nextGroup = pathMap[iPath].nextGroup;
	}
	else {
		sequencePath[PathIndex++].nextGroup = 0;
	}
}

void rspnt(float xCoordinate, float yCoordinate) noexcept {
	BSequence[OutputIndex].x           = xCoordinate;
	BSequence[OutputIndex].y           = yCoordinate;
	BSequence[OutputIndex++].attribute = 0;
}

void brkdun(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish) {
	rspnt(sortedLines[start]->x, sortedLines[start]->y);
	rspnt(sortedLines[finish]->x, sortedLines[finish]->y);
	rspnt(WorkingFormVertices[sortedLines[start]->line].x, WorkingFormVertices[sortedLines[start]->line].y);
	StateMap.set(StateFlag::BRKFIX);
}

void duseq1() noexcept {
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x,
	      (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void movseq(const std::vector<SMALPNTL*>& sortedLines, size_t ind) {
	SMALPNTL* lineEndPoint = sortedLines[ind];

	BSequence[OutputIndex].attribute = SEQBOT;
	BSequence[OutputIndex].x         = lineEndPoint->x;
	BSequence[OutputIndex].y         = lineEndPoint->y;
	OutputIndex++;
	// Be careful - this makes lineEndPoint point to the next entry in LineEndPoints
	//             and not the next entry in sortedLines
	lineEndPoint++;
	BSequence[OutputIndex].attribute = SEQTOP;
	BSequence[OutputIndex].x         = lineEndPoint->x;
	BSequence[OutputIndex].y         = lineEndPoint->y;
	OutputIndex++;
}

void brkseq(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish, boost::dynamic_bitset<>& sequenceMap) {
	unsigned savedGroup = 0;

	StateMap.reset(StateFlag::SEQDUN);
	if (start > finish) {
		savedGroup = sortedLines[start]->group + 1;
		// This odd construction for iLine is used to ensure
		// loop terminates when finish = 0
		for (auto iLine = start + 1; iLine != finish; iLine--) {
			const auto iLineDec = iLine - 1;
			savedGroup--;
			if (sortedLines[iLineDec]->group != savedGroup) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = sortedLines[iLineDec];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				savedGroup = SequenceLines[0].group;
			}
			else {
				SequenceLines = sortedLines[iLineDec];
			}
			if (sequenceMap.test_set(iLineDec)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN))
					duseq1();
			}
			else {
				movseq(sortedLines, iLineDec);
			}
		}
		LastGroup = SequenceLines->group;
	}
	else {
		savedGroup = sortedLines[start]->group - 1;
		for (auto iLine = start; iLine <= finish; iLine++) {
			savedGroup++;
			if (sortedLines[iLine]->group != savedGroup) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = sortedLines[iLine];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				savedGroup = SequenceLines[0].group;
			}
			else {
				SequenceLines = sortedLines[iLine];
			}
			if (sequenceMap.test_set(iLine)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN))
					duseq1();
			}
			else {
				movseq(sortedLines, iLine);
			}
		}
		LastGroup = SequenceLines->group;
	}
	if (StateMap.testAndReset(StateFlag::SEQDUN)) {
		duseq1();
	}
}

void dunseq(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish) {
	double minimumY = 1e30;

	for (auto iLine = start; iLine <= finish; iLine++) {
		const double deltaY = sortedLines[start][1].y - sortedLines[start][0].y;
		if (deltaY < minimumY)
			minimumY = deltaY;
	}
	if (minimumY == 1e30) {
		minimumY = 0;
	}
	else {
		minimumY /= 2;
	}
	rspnt(sortedLines[start][0].x, sortedLines[start][0].y + minimumY);
	rspnt(sortedLines[finish][0].x, sortedLines[finish][0].y + minimumY);
	LastGroup = sortedLines[finish][0].group;
}

void duseq2(const std::vector<SMALPNTL*>& sortedLines, size_t iLine) {
	SequenceLines = sortedLines[iLine];
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x,
	      (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void duseq(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish, boost::dynamic_bitset<>& sequenceMap) {
	unsigned savedTopLine = sortedLines[start][1].line;
	bool     flag         = false;

	SequenceLines = sortedLines[start];
	StateMap.reset(StateFlag::SEQDUN);
	if (start > finish) {
		auto iLine = start + 1;
		// This odd construction for iLine is used to ensure loop terminates when finish = 0
		for (iLine = start + 1; iLine != finish; iLine--) {
			const auto iLineDec = iLine - 1;
			if (sequenceMap.test_set(iLineDec)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
					flag = true;
					duseq2(sortedLines, iLineDec);
				}
				else {
					if (savedTopLine != sortedLines[iLineDec][1].line) {
						if (iLineDec) {
							duseq2(sortedLines, iLineDec + 1);
						}
						flag = true;
						duseq2(sortedLines, iLineDec);
						savedTopLine = SequenceLines[1].line;
					}
				}
			}
			else {
				if (StateMap.testAndReset(StateFlag::SEQDUN)) {
					duseq2(sortedLines, (iLineDec + 1));
				}
				flag          = true;
				SequenceLines = sortedLines[iLineDec];
				movseq(sortedLines, iLineDec);
			}
		}
		if (StateMap.testAndReset(StateFlag::SEQDUN)) {
			flag = true;
			duseq2(sortedLines, iLine);
		}
		if (flag) {
			LastGroup = SequenceLines->group;
		}
	}
	else {
		auto iLine = start;
		for (iLine = start; iLine <= finish; iLine++) {
			if (sequenceMap.test_set(iLine)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
					flag = true;
					duseq2(sortedLines, iLine);
				}
				else {
					if (savedTopLine != sortedLines[iLine][1].line) {
						if (iLine) {
							duseq2(sortedLines, (iLine - 1));
						}
						flag = true;
						duseq2(sortedLines, iLine);
						savedTopLine = SequenceLines[1].line;
					}
				}
			}
			else {
				if (StateMap.testAndReset(StateFlag::SEQDUN)) {
					if (iLine) {
						duseq2(sortedLines, (iLine - 1));
					}
				}
				flag          = true;
				SequenceLines = sortedLines[iLine];
				movseq(sortedLines, iLine);
			}
		}
		if (StateMap.testAndReset(StateFlag::SEQDUN)) {
			if (iLine) {
				flag = true;
				duseq2(sortedLines, (iLine - 1));
			}
		}
		if (flag) {
			LastGroup = SequenceLines->group;
		}
	}
}

void durgn(const std::vector<FSEQ>&      sequencePath,
           boost::dynamic_bitset<>&      visitedRegions,
           const std::vector<SMALPNTL*>& sortedLines,
           unsigned                      pthi,
           size_t                        lineCount,
           std::vector<REGION>&          regionsList) {
	unsigned dun = 0, gdif = 0, iVertex = 0;
	size_t   ind = 0;

	size_t         mindif = 0, fdif = 0, bdif = 0, seql = 0, seqn = 0;
	size_t         sequenceStart = 0;
	size_t         sequenceEnd   = 0;
	unsigned       groupStart = 0, groupEnd = 0;
	double         length = 0.0, minimumLength = 0.0;
	BSEQPNT*       bpnt      = nullptr;
	const unsigned nextGroup = sequencePath[pthi].nextGroup;
	const unsigned iRegion   = sequencePath[pthi].node;

	boost::dynamic_bitset<> sequenceMap(lineCount);

	// ToDo - More renaming required
	CurrentRegion = &regionsList[iRegion];
	sequenceStart = CurrentRegion->start;
	sequenceEnd   = CurrentRegion->end;
	if (sequencePath[pthi].skp || StateMap.testAndReset(StateFlag::BRKFIX)) {
		if (BSequence[OutputIndex - 1].attribute != SEQBOT) {
			rspnt(BSequence[OutputIndex - 2].x, BSequence[OutputIndex - 2].y);
		}
		dun           = sortedLines[sequenceStart]->line;
		bpnt          = &BSequence[OutputIndex - 1];
		minimumLength = 1e99;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			length = hypot(bpnt->x - WorkingFormVertices[iVertex].x, bpnt->y - WorkingFormVertices[iVertex].y);
			if (length < minimumLength) {
				minimumLength = length;
				mindif        = iVertex;
			}
		}
		if (minimumLength) {
			rspnt(WorkingFormVertices[mindif].x, WorkingFormVertices[mindif].y);
		}
		fdif = (VertexCount + dun - mindif) % VertexCount;
		bdif = (VertexCount - dun + mindif) % VertexCount;
		if (fdif < bdif) {
			ind = nxt(mindif);
			while (ind != dun) {
				rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
				ind = nxt(ind);
			}
			rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
		}
		else {
			ind = prv(mindif);
			while (ind != dun) {
				rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
				ind = prv(ind);
			}
			rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
		}
	}
	if (visitedRegions[iRegion]) {
		dun = 1;
	}
	else {
		dun = 0;
		visitedRegions.set(iRegion);
	}
	groupStart = sortedLines[CurrentRegion->start]->group;
	groupEnd   = sortedLines[CurrentRegion->end]->group;
	if (groupEnd != groupStart) {
		seql = static_cast<double>(LastGroup - groupStart) / (groupEnd - groupStart) * (sequenceEnd - sequenceStart)
		       + sequenceStart;
	}
	else {
		seql = 0;
	}
	if (seql > lineCount) {
		seql = 0;
	}
	length = static_cast<double>(groupEnd - groupStart) * (sequenceEnd - sequenceStart);
	if (length) {
		seqn = static_cast<double>(nextGroup - groupStart) / length + sequenceStart;
	}
	else {
		seqn = sequenceEnd;
	}
	if (seql < sequenceStart) {
		seql = sequenceStart;
	}
	if (seql > sequenceEnd) {
		seql = sequenceEnd;
	}
	if (seqn < sequenceStart) {
		seqn = sequenceStart;
	}
	if (seqn > sequenceEnd) {
		seqn = sequenceEnd;
	}
	if (sortedLines[seql]->group != LastGroup) {
		if (seql < sequenceEnd && sortedLines[seql + 1]->group == LastGroup) {
			seql++;
		}
		else {
			if (seql > sequenceStart && sortedLines[seql - 1]->group == LastGroup)
				seql--;
			else {
				mindif = 0xffffffff;
				for (ind = sequenceStart; ind <= sequenceEnd; ind++) {
					gdif = ((sortedLines[ind]->group > LastGroup) ? (sortedLines[ind]->group - LastGroup)
					                                              : (LastGroup - sortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seql   = ind;
					}
				}
			}
		}
	}
	if (sortedLines[seqn]->group != nextGroup) {
		if (seqn < sequenceEnd && sortedLines[seqn + 1]->group == nextGroup) {
			seqn++;
		}
		else {
			if (seqn > sequenceStart && sortedLines[seqn - 1]->group == nextGroup) {
				seqn--;
			}
			else {
				mindif = 0xffffffff;
				for (ind = sequenceStart; ind <= sequenceEnd; ind++) {
					gdif = ((sortedLines[ind]->group > nextGroup) ? (sortedLines[ind]->group - nextGroup)
					                                              : (nextGroup - sortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seqn   = ind;
					}
				}
			}
		}
	}
	if (CurrentRegion->breakCount) {
		if (dun) {
			brkdun(sortedLines, seql, seqn);
		}
		else {
			if (LastGroup >= groupEnd) {
				brkseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
				if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn) {
					brkseq(sortedLines, sequenceStart, seqn, sequenceMap);
				}
			}
			else {
				if (groupStart <= nextGroup) {
					if (seql != sequenceStart) {
						brkseq(sortedLines, seql, sequenceStart, sequenceMap);
					}
					brkseq(sortedLines, sequenceStart, sequenceEnd, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn) {
						brkseq(sortedLines, sequenceEnd, seqn, sequenceMap);
					}
				}
				else {
					if (seql != sequenceEnd) {
						brkseq(sortedLines, seql, sequenceEnd, sequenceMap);
					}
					brkseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceStart != seqn) {
						brkseq(sortedLines, sequenceStart, seqn, sequenceMap);
					}
				}
			}
		}
	}
	else {
		if (dun) {
			dunseq(sortedLines, seql, seqn);
		}
		else {
			if (LastGroup >= groupEnd) {
				duseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
				duseq(sortedLines, sequenceStart, seqn, sequenceMap);
			}
			else {
				if (groupStart <= nextGroup) {
					if (seql != sequenceStart) {
						duseq(sortedLines, seql, sequenceStart, sequenceMap);
					}
					duseq(sortedLines, sequenceStart, sequenceEnd, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn) {
						duseq(sortedLines, sequenceEnd, seqn, sequenceMap);
					}
				}
				else {
					if (seql != sequenceEnd) {
						duseq(sortedLines, seql, sequenceEnd, sequenceMap);
					}
					duseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceStart != seqn) {
						duseq(sortedLines, sequenceStart, seqn, sequenceMap);
					}
				}
			}
		}
	}
}

#define BUGSEQ 0

void lcon(std::vector<unsigned>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints) {
	unsigned  iPath = 0, iLine = 0, iRegion = 0, iSequence = 0, iNode = 0;
	unsigned  leftRegion = 0, breakLine = 0, count = 0, startGroup = 0;
	bool      isConnected    = false;
	SMALPNTL* lineGroupPoint = nullptr;
	unsigned  iStartLine     = 0;
	size_t    lineCount      = 0;

#if BUGSEQ

	unsigned bugColor;
#endif

	if (StitchLineCount) {
		std::vector<SMALPNTL*> sortedLines;
		sortedLines.reserve(StitchLineCount >> 1);
		for (iLine = 0; iLine < StitchLineCount; iLine += 2) {
			sortedLines.push_back(&lineEndpoints[iLine]);
		}
		std::sort(sortedLines.begin(), sortedLines.end(), sqcomp);
		lineCount   = sortedLines.size();
		RegionCount = 0;
		// Count the regions. There cannot be more regions than lines
		std::vector<REGION> regions(lineCount);
		regions[0].start = 0;
		breakLine        = sortedLines[0]->line;
		for (iLine = 0; iLine < lineCount; iLine++) {
			if (breakLine != sortedLines[iLine]->line) {
				regions[RegionCount++].end = iLine - 1;
				regions[RegionCount].start = iLine;
				breakLine                  = sortedLines[iLine]->line;
			}
		}
		regions[RegionCount++].end = iLine - 1;
		std::vector<REGION>     RegionsList(RegionCount);
		boost::dynamic_bitset<> visitedRegions(RegionCount);
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			RegionsList[iRegion].start      = regions[iRegion].start;
			RegionsList[iRegion].end        = regions[iRegion].end;
			RegionsList[iRegion].breakCount = 0;
		}
		iStartLine = 0;
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			count = 0;
			if ((RegionsList[iRegion].end - RegionsList[iRegion].start) > 1) {
				startGroup = sortedLines[RegionsList[iRegion].start]->group;
				for (iLine = RegionsList[iRegion].start + 1; iLine <= RegionsList[iRegion].end; iLine++) {
					startGroup++;
					if (sortedLines[iLine]->group != startGroup) {
						if (!count) {
							RegionsList[iRegion].regionBreak = iStartLine;
						}
						count++;
						startGroup = sortedLines[iLine]->group;
					}
				}
			}
			RegionsList[iRegion].breakCount = count;
		}

#if BUGSEQ
		bugColor      = 0;
		SequenceIndex = 0;
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			for (iLine = RegionsList[iRegion].start; iLine <= RegionsList[iRegion].end; iLine++) {
				lineGroupPoint                        = &*SortedLines[iLine];
				StitchBuffer[SequenceIndex].attribute = bugColor;
				StitchBuffer[SequenceIndex].x         = lineGroupPoint[0].x;
				StitchBuffer[SequenceIndex++].y       = lineGroupPoint[0].y;
				StitchBuffer[SequenceIndex].attribute = bugColor;
				StitchBuffer[SequenceIndex].x         = lineGroupPoint[1].x;
				StitchBuffer[SequenceIndex++].y       = lineGroupPoint[1].y;
			}
			bugColor++;
			bugColor &= 0xf;
		}
		PCSHeader.stitchCount = SequenceIndex;
		goto seqskip;
#endif
		OutputIndex = 0;

		std::vector<unsigned> mapIndexSequence;
		mapIndexSequence.reserve(RegionCount + 1);
		std::vector<RCON> pathMap;
		std::vector<FSEQ> sequencePath;

		if (RegionCount > 1) {
			PathMapIndex = 0;
			// use the number of possible pairs of nodes n(n - 1)/2 and account for RegionCount possibly being odd
			pathMap.reserve(((RegionCount * (RegionCount - 1)) / 2) + 2);
			for (iSequence = 0; iSequence < RegionCount; iSequence++) {
				mapIndexSequence.push_back(PathMapIndex);
				count              = 0;
				GapToClosestRegion = 0;
				for (iNode = 0; iNode < RegionCount; iNode++) {
					if (iSequence != iNode) {
						isConnected = regclos(groupIndexSequence, lineEndpoints, sortedLines, iSequence, iNode, RegionsList);
						if (isConnected) {
							pathMap.push_back({ iNode, isConnected, NextGroup });
							PathMapIndex++;
							count++;
						}
					}
				}
				while (!count) {
					GapToClosestRegion += LineSpacing;
					count = 0;
					for (iNode = 0; iNode < RegionCount; iNode++) {
						if (iSequence != iNode) {
							isConnected = regclos(groupIndexSequence, lineEndpoints, sortedLines, iSequence, iNode, RegionsList);
							if (isConnected) {
								pathMap.push_back({ iNode, isConnected, NextGroup });
								PathMapIndex++;
								count++;
							}
						}
					}
				}
			}
			mapIndexSequence.push_back(PathMapIndex);
			// find the leftmost region
			startGroup = 0xffffffff;
			leftRegion = 0;
			for (iRegion = 0; iRegion < RegionCount; iRegion++) {
				lineGroupPoint = sortedLines[RegionsList[iRegion].start];
				if (lineGroupPoint->group < startGroup) {
					startGroup = lineGroupPoint->group;
					leftRegion = iRegion;
				}
			}
			OutputIndex = 0;
			std::vector<RGSEQ> tempPath(((RegionCount * (RegionCount - 1)) / 2) + 1);

			// find the leftmost region in pathMap
			SequencePathIndex = 1;
			bool dontSkip     = true;
			for (iPath = 0; iPath < PathMapIndex; iPath++) {
				if (pathMap[iPath].node == leftRegion) {
					dontSkip = false;
					break;
				}
			}
			if (dontSkip) {
				pathMap[PathMapIndex].node      = leftRegion;
				pathMap[PathMapIndex].nextGroup = 0;
				iPath                           = PathMapIndex;
			}
			// set the first entry in the temporary path to the leftmost region
			tempPath[0].pcon  = iPath;
			tempPath[0].count = 1;
			tempPath[0].skp   = false;
			visitedRegions.set(leftRegion);
			DoneRegion = leftRegion;
			while (unvis(visitedRegions)) {
				nxtrgn(tempPath, pathMap, mapIndexSequence, visitedRegions, sortedLines, RegionsList);
			}
			count = 0xffffffff;
			sequencePath.reserve(SequencePathIndex);
			for (iPath = 0; iPath < SequencePathIndex; iPath++) {
				const bool     tmpSkip = tempPath[iPath].skp;
				unsigned short tmpNode = 0;
				if (tempPath[iPath].pcon == 0xffffffff) {
					tmpNode = tempPath[iPath].count;
					count   = tempPath[iPath].count;
				}
				else {
					if (tempPath[iPath].pcon != count) {
						count   = tempPath[iPath].pcon;
						tmpNode = pathMap[tempPath[iPath].pcon].node;
					}
				}
				sequencePath.push_back({ tmpNode, 0, tmpSkip });
			}
			PathIndex = 0;
			for (iPath = 0; iPath < SequencePathIndex; iPath++) {
				nxtseq(sequencePath, pathMap, mapIndexSequence, iPath);
			}
			visitedRegions.reset();
			LastGroup = 0;
			for (iPath = 0; iPath < PathIndex; iPath++) {
				OutputDebugString(
				    fmt::format(L"iterator {},vrt {},grpn {}\n", iPath, pathMap[iPath].node, pathMap[iPath].nextGroup).c_str());
				if (!unvis(visitedRegions))
					break;
				durgn(sequencePath, visitedRegions, sortedLines, iPath, lineCount, RegionsList);
			}
		}
		else {
			sequencePath.resize(1);
			LastGroup                 = 0;
			sequencePath[0].node      = 0;
			sequencePath[0].nextGroup = sortedLines[RegionsList[0].end]->group;
			sequencePath[0].skp       = false;
			durgn(sequencePath, visitedRegions, sortedLines, 0, lineCount, RegionsList);
		}

#if BUGSEQ

	seqskip:;
#endif
	}
}

#define RITSIZ 6

unsigned seqtab[] = {
	12, 7, 15, 11, 13, 17,
};

void bakseq() {
#if BUGBAK

	for (SequenceIndex = 0; SequenceIndex < OutputIndex; SequenceIndex++) {
		OSequence[SequenceIndex].x = BSequence[SequenceIndex].x;
		OSequence[SequenceIndex].y = BSequence[SequenceIndex].y;
	}
	SelectedForm->maxFillStitchLen = 6000;
#else

	// ToDo - More renaming required
	int          count = 0, rcnt = 0;
	size_t       iSequence = OutputIndex - 1;
	int          rit       = 0;
	dPOINT       delta = {}, point = {}, step = {};
	double       length = 0.0, slope = 0.0;
	const double UserStitchLength2 = UserStitchLength * 2;
	const double UserStitchLength9 = UserStitchLength / 9;
	const double StitchSpacing2    = LineSpacing * 2;

	SequenceIndex = 0;
	StateMap.reset(StateFlag::FILDIR);
	OSequence[SequenceIndex] = BSequence[iSequence];
	SequenceIndex++;
	SelectedPoint = BSequence[iSequence];
	iSequence--;
	while (iSequence > 0) {
		rcnt = iSequence % RITSIZ;
		if (SequenceIndex > MAXITEMS) {
			SequenceIndex = MAXITEMS - 1;
			return;
		}
		rit     = BSequence[iSequence].x / StitchSpacing2;
		delta.x = BSequence[iSequence].x - BSequence[iSequence + 1].x;
		delta.y = BSequence[iSequence].y - BSequence[iSequence + 1].y;
		if (delta.y)
			slope = delta.x / delta.y;
		else
			slope = 1e99;
		switch (BSequence[iSequence].attribute) {
		case SEQTOP:
			if (SelectedForm->extendedAttribute & AT_SQR) {
				if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					OSequence[SequenceIndex++] = BSequence[iSequence - 1];
					count                      = ceil(BSequence[iSequence].y / UserStitchLength);
					do {
						OSequence[SequenceIndex].y = count * UserStitchLength + (rit % seqtab[rcnt]) * UserStitchLength9;
						if (OSequence[SequenceIndex].y > BSequence[iSequence].y)
							break;
						delta.y                      = OSequence[SequenceIndex].y - BSequence[iSequence].y;
						OSequence[SequenceIndex++].x = BSequence[iSequence].x;
						count++;
					} while (true);
					OSequence[SequenceIndex++] = BSequence[iSequence];
				}
				else {
					OSequence[SequenceIndex++] = BSequence[iSequence];
					count                      = floor(BSequence[iSequence].y / UserStitchLength);
					do {
						OSequence[SequenceIndex].y = count * UserStitchLength - ((rit + 2) % seqtab[rcnt]) * UserStitchLength9;
						if (OSequence[SequenceIndex].y < BSequence[iSequence - 1].y)
							break;
						delta.y                      = OSequence[SequenceIndex].y - BSequence[iSequence - 1].y;
						OSequence[SequenceIndex++].x = BSequence[iSequence].x;
						count--;
					} while (true);
					OSequence[SequenceIndex++] = BSequence[iSequence - 1];
				}
			}
			else {
				count = ceil(BSequence[iSequence + 1].y / UserStitchLength);
				do {
					OSequence[SequenceIndex].y = count * UserStitchLength + (rit % seqtab[rcnt]) * UserStitchLength9;
					if (OSequence[SequenceIndex].y > BSequence[iSequence].y)
						break;
					delta.y                      = OSequence[SequenceIndex].y - BSequence[iSequence + 1].y;
					delta.x                      = slope * delta.y;
					OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
					count++;
				} while (true);
				OSequence[SequenceIndex++] = BSequence[iSequence];
			}
			break;
		case SEQBOT:
			if (!(SelectedForm->extendedAttribute & AT_SQR)) {
				count = floor(BSequence[iSequence + 1].y / UserStitchLength);
				do {
					OSequence[SequenceIndex].y = count * UserStitchLength - ((rit + 2) % seqtab[rcnt]) * UserStitchLength9;
					if (OSequence[SequenceIndex].y < BSequence[iSequence].y)
						break;
					delta.y                      = OSequence[SequenceIndex].y - BSequence[iSequence + 1].y;
					delta.x                      = slope * delta.y;
					OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
					count--;
				} while (true);
				OSequence[SequenceIndex++] = BSequence[iSequence];
			}
			break;
		case 0:
			delta.x = BSequence[iSequence].x - BSequence[iSequence + 1].x;
			delta.y = BSequence[iSequence].y - BSequence[iSequence + 1].y;
			StateMap.reset(StateFlag::FILDIR);
			length = hypot(delta.x, delta.y);
			if (length) {
				if (length > UserStitchLength2) {
					point = BSequence[iSequence + 1];
					count = length / UserStitchLength - 1;
					if (chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXITEMS - 3)
						return;
					step.x = delta.x / count;
					step.y = delta.y / count;
					while (count) {
						point.x += step.x;
						point.y += step.y;
						OSequence[SequenceIndex++] = point;
						count--;
					}
				}
			}
			OSequence[SequenceIndex++] = BSequence[iSequence];
		}
		iSequence--;
	}
#endif
}

void satcpy(SATCON* const destination, const std::vector<SATCON> source, unsigned int size) {
	if (destination) {
		for (unsigned int iSource = 0; iSource < size; iSource++) {
			destination[iSource] = source[iSource];
		}
	}
}

void satadj() {
	fvars(ClosestFormToCursor);

	unsigned            iDestination = 0;
	size_t              iVertex = 0, iReverse = 0, iForward = 0, iGuide = 0;
	std::vector<SATCON> interiorGuides(CurrentFormGuidesCount);
	SATCON*             sourceGuide      = nullptr;
	SATCON*             destinationGuide = nullptr;
	size_t              savedGuideCount  = SelectedForm->satinGuideCount;
	FRMHED*             formHeader       = nullptr;
	ExtendedBitSet<>    satinMap(VertexCount);

	// ensure all guide endpoints are on valid vertices
	for (iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
		auto endCount = gsl::narrow<unsigned short>(VertexCount - 1);
		if (CurrentFormGuides[iGuide].finish > endCount)
			CurrentFormGuides[iGuide].finish = endCount;
		if (CurrentFormGuides[iGuide].start > endCount)
			CurrentFormGuides[iGuide].start = endCount;
	}

	// remove any guides of 0 length
	iDestination = 0;
	for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
		if (CurrentFormGuides[iSource].start != CurrentFormGuides[iSource].finish) {
			interiorGuides[iDestination++] = CurrentFormGuides[iSource];
		}
	}
	if (CurrentFormGuidesCount != iDestination) {
		OutputDebugString(fmt::format(L"Removed {} zero distance guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
		satcpy(CurrentFormGuides, interiorGuides, iDestination);
	}
	if (SatinEndGuide || SelectedForm->attribute & FRMEND) {
		// there are end guides so set the satinMap for the next step
		satinMap.reset();
		if (SelectedForm->attribute & FRMEND) {
			satinMap.set(0);
			satinMap.set(1);
		}
		if (SatinEndGuide) {
			satinMap.set(SatinEndGuide);
			satinMap.set(SatinEndGuide + 1);
		}
		// check to see if any of the current guides are end guides and add to interiorGuides if not
		iDestination = 0;
		for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
			if (!satinMap.test(CurrentFormGuides[iSource].start) && !satinMap.test(CurrentFormGuides[iSource].finish)) {
				interiorGuides[iDestination++] = CurrentFormGuides[iSource];
			}
		}
		if (CurrentFormGuidesCount != iDestination) {
			OutputDebugString(fmt::format(L"Removed {} end guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
			CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
			satcpy(CurrentFormGuides, interiorGuides, iDestination);
		}
		// remove any guides that start after the end guide
		if (SatinEndGuide) {
			iDestination = 0;
			for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
				if (CurrentFormGuides[iSource].start < SatinEndGuide) {
					interiorGuides[iDestination++] = CurrentFormGuides[iSource];
				}
			}
			if (CurrentFormGuidesCount != iDestination) {
				OutputDebugString(fmt::format(L"Removed {} reversed guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
				CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
				satcpy(CurrentFormGuides, interiorGuides, iDestination);
			}
		}
	}

	if (CurrentFormGuidesCount) {
		satinMap.reset();
		for (iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			iForward = CurrentFormGuides[iGuide].start;
			if (iForward > gsl::narrow<unsigned>(SatinEndGuide) - 1)
				iForward = SatinEndGuide - 1;
			if (satinMap.test_set(iForward)) {
				iReverse = iForward;
				if (iReverse)
					iReverse--;
				while (satinMap.test(iForward) && (iForward < (gsl::narrow<unsigned>(SatinEndGuide) - 1)))
					iForward++;
				while (iReverse && (satinMap.test(iReverse)))
					iReverse--;
				if (satinMap.test(iForward) && satinMap.test(iReverse))
					break;
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - CurrentFormGuides[iGuide].start > CurrentFormGuides[iGuide].start - iReverse)
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
				else {
					if (!satinMap.test(iForward))
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
			}
		}
		iGuide = 0u;
		do {
			iVertex = satinMap.getFirst();
			if (iVertex < VertexCount)
				CurrentFormGuides[iGuide++].start = gsl::narrow<unsigned short>(iVertex);
		} while (iVertex < VertexCount);

		CurrentFormGuidesCount = SelectedForm->satinGuideCount = gsl::narrow<unsigned short>(iGuide);
		satinMap.reset();
		// Todo - are iForward and iReverse appropriate variable names below?
		for (iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			iForward = iReverse = CurrentFormGuides[iGuide].finish;
			if (iForward > VertexCount - 1)
				iForward = VertexCount - 1;
			if (satinMap.test_set(iForward)) {
				if (iForward < VertexCount - 1)
					iForward++;
				if (iReverse > gsl::narrow<unsigned>(SatinEndGuide) + 1)
					iReverse--;
				while (satinMap.test(iForward) && iForward < VertexCount - 1)
					iForward++;
				while (iReverse > gsl::narrow<unsigned>(SatinEndGuide) - 1 && (satinMap.test(iReverse)))
					iReverse--;
				if (satinMap.test(iForward) && satinMap.test(iReverse))
					break;
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - CurrentFormGuides[iGuide].finish > CurrentFormGuides[iGuide].finish - iReverse)
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
				else {
					if (!satinMap.test(iForward))
						satinMap.set(iForward);
					else
						satinMap.set(iReverse);
				}
			}
		}
		iGuide = 0u;
		do {
			iReverse = satinMap.getLast();
			if (iReverse < VertexCount)
				CurrentFormGuides[iGuide++].finish = gsl::narrow<unsigned short>(iReverse);
		} while (iReverse < VertexCount);
		if (iGuide < CurrentFormGuidesCount)
			iGuide = CurrentFormGuidesCount;
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = gsl::narrow<unsigned short>(iGuide);
		if (SatinEndGuide) {
			auto end0 = gsl::narrow<unsigned short>(VertexCount - SatinEndGuide - 2);
			auto end1 = gsl::narrow<unsigned short>(SatinEndGuide - 2);
			if (CurrentFormGuidesCount > end0)
				CurrentFormGuidesCount = end0;
			if (CurrentFormGuidesCount > end1)
				CurrentFormGuidesCount = end1;
			SelectedForm->satinGuideCount = CurrentFormGuidesCount;
		}
	}
	if (SelectedForm->satinGuideCount < savedGuideCount) {
		iGuide      = savedGuideCount - CurrentFormGuidesCount;
		sourceGuide = destinationGuide = SelectedForm->satinOrAngle.guide;
		destinationGuide += SelectedForm->satinGuideCount;
		sourceGuide += savedGuideCount;
		std::copy(sourceGuide,
		          (&SatinGuides[SatinGuideIndex] + 1),
		          stdext::make_checked_array_iterator(destinationGuide, 10000 - SatinGuideIndex));
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			formHeader = &FormList[iForm];
			if (formHeader->type == SAT)
				formHeader->satinOrAngle.guide -= iGuide;
		}
		SatinGuideIndex -= iGuide;
	}
}

void ritseq1(size_t ind) noexcept {
	BSequence[SequenceIndex].x = CurrentFormVertices[ind].x;
	BSequence[SequenceIndex].y = CurrentFormVertices[ind].y;
	SequenceIndex++;
}

void filinu(const dPOINT& inPoint) {
	dPOINT       point  = { SelectedPoint.x, SelectedPoint.y };
	dPOINT       delta  = { (inPoint.x - SelectedPoint.x), (inPoint.y - SelectedPoint.y) };
	const double length = hypot(delta.x, delta.y);
	unsigned     count  = length / UserStitchLength;
	dPOINT       step   = {};

	if (chkmax(count, SequenceIndex) || count + SequenceIndex > MAXITEMS - 3)
		return;
	if (count) {
		if (StateMap.test(StateFlag::FILDIR))
			count++;
		step.x = delta.x / count;
		step.y = delta.y / count;
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence[SequenceIndex].x   = point.x;
			OSequence[SequenceIndex++].y = point.y;
			count--;
		}
	}
	else {
		OSequence[SequenceIndex].x   = inPoint.x;
		OSequence[SequenceIndex++].y = inPoint.y;
	}
	SelectedPoint = inPoint;
}

void filin(dPOINT currentPoint) {
	dPOINT       delta  = { (currentPoint.x - SelectedPoint.x), (currentPoint.y - SelectedPoint.y) };
	dPOINT       step   = {};
	dPOINT       point  = { SelectedPoint.x, SelectedPoint.y };
	const double length = hypot(delta.x, delta.y);
	int          count  = length / UserStitchLength;

	if (chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXITEMS - 3)
		return;
	if (count) {
		if (StateMap.test(StateFlag::FILDIR))
			count++;
		step.x = delta.x / count;
		step.y = delta.y / count;
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence[SequenceIndex].x   = point.x;
			OSequence[SequenceIndex++].y = point.y;
			count--;
		}
	}
	else {
		OSequence[SequenceIndex].x   = currentPoint.x;
		OSequence[SequenceIndex++].y = currentPoint.y;
	}
	SelectedPoint = currentPoint;
}

void satfn(const std::vector<double>& lengths, size_t line1Start, size_t line1End, size_t line2Start, size_t line2End) {
	size_t   line1Next = 0, line2Previous = 0;
	unsigned stitchCount = 0;
	unsigned iSegment = 0, line1Count = 0, line2Count = 0;
	unsigned segmentStitchCount = 0;
	size_t   iLine1Vertex       = 0;
	size_t   iNextVertex = 0, iVertex = 0, iLine2Vertex = 0;
	unsigned iLine1Count = 0, iLine2Count = 0;
	size_t   line1Segments = 0, line2Segments = 0;
	double   line1Length = 0.0, line2Length = 0.0;
	dPOINT   line1Point = {}, line2Point = {}, line1Delta = {}, line2Delta = {};
	dPOINT   line1Step = {}, line2Step = {};

	if (line1Start != line1End && line2Start != line2End) {
		if (!StateMap.testAndSet(StateFlag::SAT1)) {
			if (StateMap.test(StateFlag::FTHR)) {
				BSequence[SequenceIndex].attribute = 0;
				ritseq1(line1Start % VertexCount);
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					ritseq1(line1Start % VertexCount);
					ritseq1(line2Start % VertexCount);
				}
				else {
					SelectedPoint              = CurrentFormVertices[line1Start];
					OSequence[SequenceIndex++] = SelectedPoint;
				}
			}
		}
		line1Length = lengths[line1End] - lengths[line1Start];
		line2Length = lengths[line2Start] - lengths[line2End];
		if (fabs(line1Length) > fabs(line2Length))
			stitchCount = fabs(line2Length) / LineSpacing;
		else
			stitchCount = fabs(line1Length) / LineSpacing;
		line1Segments = ((line1End > line1Start) ? (line1End - line1Start) : (line1Start - line1End));
		line2Segments = ((line2Start > line2End) ? (line2Start - line2End) : (line2End - line2Start));
		std::vector<unsigned> line1StitchCounts;
		line1StitchCounts.reserve(line1Segments);
		std::vector<unsigned> line2StitchCounts;
		line2StitchCounts.reserve(line2Segments);
		iVertex            = line1Start;
		segmentStitchCount = 0;
		for (iSegment = 0; iSegment < line1Segments - 1; iSegment++) {
			iNextVertex        = nxt(iVertex);
			const unsigned val = ((lengths[iNextVertex] - lengths[iVertex]) / line1Length) * stitchCount + 0.5;
			line1StitchCounts.push_back(val);
			segmentStitchCount += val;
			iVertex = nxt(iVertex);
		}
		line1StitchCounts.push_back(stitchCount - segmentStitchCount);
		iNextVertex        = line2Start;
		iVertex            = prv(iNextVertex);
		iSegment           = 0;
		segmentStitchCount = 0;
		while (iVertex > line2End) {
			const unsigned val = ((lengths[iNextVertex] - lengths[iVertex]) / line2Length) * stitchCount + 0.5;
			line2StitchCounts.push_back(val);
			segmentStitchCount += val;
			iNextVertex = prv(iNextVertex);
			iVertex     = prv(iNextVertex);
		}
		line2StitchCounts.push_back(stitchCount - segmentStitchCount);
		line1Point.x  = CurrentFormVertices[line1Start].x;
		line1Point.y  = CurrentFormVertices[line1Start].y;
		line1Next     = nxt(line1Start);
		line2Previous = prv(line2Start);
		line1Count    = line1StitchCounts[iLine1Count++];
		line2Count    = line2StitchCounts[iLine2Count++];
		iLine1Vertex  = line1Start;
		iLine2Vertex  = line2Start;
		line1Delta.x  = CurrentFormVertices[line1Next].x - CurrentFormVertices[iLine1Vertex].x;
		line1Delta.y  = CurrentFormVertices[line1Next].y - CurrentFormVertices[iLine1Vertex].y;
		if (iLine2Vertex == VertexCount) {
			line2Delta.x = CurrentFormVertices[line2Previous].x - CurrentFormVertices[0].x;
			line2Delta.y = CurrentFormVertices[line2Previous].y - CurrentFormVertices[0].y;
			line2Point   = CurrentFormVertices[0];
		}
		else {
			line2Delta.x = CurrentFormVertices[line2Previous].x - CurrentFormVertices[iLine2Vertex].x;
			line2Delta.y = CurrentFormVertices[line2Previous].y - CurrentFormVertices[iLine2Vertex].y;
			line2Point   = CurrentFormVertices[iLine2Vertex];
		}
		iLine1Vertex = nxt(iLine1Vertex);
		iLine2Vertex = prv(iLine2Vertex);
		line1Step.x  = line1Delta.x / line1Count;
		line1Step.y  = line1Delta.y / line1Count;
		line2Step.x  = line2Delta.x / line2Count;
		line2Step.y  = line2Delta.y / line2Count;
		bool flag    = false;
		do {
			flag = false;
			if (StateMap.test(StateFlag::FTHR)) {
				while (line1Count && line2Count) {
					line1Point.x += line1Step.x;
					line1Point.y += line1Step.y;
					line2Point.x += line2Step.x;
					line2Point.y += line2Step.y;
					if (StateMap.testAndFlip(StateFlag::FILDIR)) {
						BSequence[SequenceIndex].attribute = 0;
						BSequence[SequenceIndex].x         = line1Point.x;
						BSequence[SequenceIndex++].y       = line1Point.y;
					}
					else {
						BSequence[SequenceIndex].attribute = 1;
						BSequence[SequenceIndex].x         = line2Point.x;
						BSequence[SequenceIndex++].y       = line2Point.y;
					}
					if (SequenceIndex > MAXITEMS - 6) {
						SequenceIndex = MAXITEMS - 6;
						return;
					}
					line1Count--;
					line2Count--;
				}
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					while (line1Count && line2Count) {
						line1Point.x += line1Step.x;
						line1Point.y += line1Step.y;
						line2Point.x += line2Step.x;
						line2Point.y += line2Step.y;
						if (StateMap.testAndFlip(StateFlag::FILDIR)) {
							BSequence[SequenceIndex].attribute = 0;
							BSequence[SequenceIndex].x         = line1Point.x;
							BSequence[SequenceIndex++].y       = line1Point.y;
							BSequence[SequenceIndex].attribute = 1;
							BSequence[SequenceIndex].x         = line2Point.x;
							BSequence[SequenceIndex++].y       = line2Point.y;
						}
						else {
							BSequence[SequenceIndex].attribute = 2;
							BSequence[SequenceIndex].x         = line2Point.x;
							BSequence[SequenceIndex++].y       = line2Point.y;
							BSequence[SequenceIndex].attribute = 3;
							BSequence[SequenceIndex].x         = line1Point.x;
							BSequence[SequenceIndex++].y       = line1Point.y;
						}
						if (SequenceIndex > MAXITEMS - 6) {
							SequenceIndex = MAXITEMS - 6;
							return;
						}
						line1Count--;
						line2Count--;
					}
				}
				else {
					while (line1Count && line2Count) {
						line1Point.x += line1Step.x;
						line1Point.y += line1Step.y;
						line2Point.x += line2Step.x;
						line2Point.y += line2Step.y;
						if (StateMap.testAndFlip(StateFlag::FILDIR)) {
							if (UserFlagMap.test(UserFlag::SQRFIL))
								filinu(line2Point);
							filin(line1Point);
						}
						else {
							if (UserFlagMap.test(UserFlag::SQRFIL))
								filinu(line1Point);
							filin(line2Point);
						}
						line1Count--;
						line2Count--;
					}
				}
			}
			if ((iLine1Count < line1Segments || iLine2Count < line2Segments)) {
				if (!line1Count && iLine1Count < line1StitchCounts.size()) {
					line1Count   = line1StitchCounts[iLine1Count++];
					line1Next    = nxt(iLine1Vertex);
					line1Delta.x = CurrentFormVertices[line1Next].x - CurrentFormVertices[iLine1Vertex].x;
					line1Delta.y = CurrentFormVertices[line1Next].y - CurrentFormVertices[iLine1Vertex].y;
					iLine1Vertex = nxt(iLine1Vertex);
					line1Step.x  = line1Delta.x / line1Count;
					line1Step.y  = line1Delta.y / line1Count;
				}

				if (!line2Count && iLine2Count < line2StitchCounts.size()) {
					line2Count    = line2StitchCounts[iLine2Count++];
					line2Previous = prv(iLine2Vertex);
					line2Delta.x  = CurrentFormVertices[line2Previous].x - CurrentFormVertices[iLine2Vertex].x;
					line2Delta.y  = CurrentFormVertices[line2Previous].y - CurrentFormVertices[iLine2Vertex].y;
					iLine2Vertex  = prv(iLine2Vertex);
					line2Step.x   = line2Delta.x / line2Count;
					line2Step.y   = line2Delta.y / line2Count;
				}
				if ((line1Count || line2Count) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
					flag = true;
				}
			}
		} while (flag);
	}
}

void satmf(const std::vector<double>& lengths) {
	unsigned iGuide  = 0;
	size_t   iVertex = 0;
	double   length = 0.0, deltaX = 0.0, deltaY = 0.0;

	if (SelectedForm->attribute & FRMEND)
		iGuide = 1;
	satfn(lengths, iGuide, CurrentFormGuides[0].start, VertexCount, CurrentFormGuides[0].finish);
	for (iGuide = 0; iGuide < gsl::narrow<unsigned>(CurrentFormGuidesCount) - 1; iGuide++)
		satfn(lengths,
		      CurrentFormGuides[iGuide].start,
		      CurrentFormGuides[iGuide + 1].start,
		      CurrentFormGuides[iGuide].finish,
		      CurrentFormGuides[iGuide + 1].finish);
	if (SatinEndGuide)
		satfn(lengths, CurrentFormGuides[iGuide].start, SatinEndGuide, CurrentFormGuides[iGuide].finish, SatinEndGuide + 1);
	else {
		if (CurrentFormGuides[iGuide].finish - CurrentFormGuides[iGuide].start > 2) {
			length = (lengths[CurrentFormGuides[iGuide].finish] - lengths[CurrentFormGuides[iGuide].start]) / 2
			         + lengths[CurrentFormGuides[iGuide].start];
			iVertex = CurrentFormGuides[iGuide].start;
			while (length > lengths[iVertex])
				iVertex++;
			deltaX = lengths[iVertex] - length;
			deltaY = length - lengths[iVertex - 1];
			if (deltaY > deltaX)
				iVertex--;
			satfn(lengths, CurrentFormGuides[iGuide].start, iVertex, CurrentFormGuides[iGuide].finish, iVertex);
		}
		else
			satfn(lengths,
			      CurrentFormGuides[iGuide].start,
			      CurrentFormGuides[iGuide].start + 1,
			      CurrentFormGuides[iGuide].finish,
			      CurrentFormGuides[iGuide].start + 1);
	}
}

void satfil() {
	unsigned     iVertex = 0;
	double       length = 0.0, deltaX = 0.0, deltaY = 0.0;
	const double spacing = LineSpacing;

	fvars(ClosestFormToCursor);
	satadj();
	LineSpacing /= 2;
	SequenceIndex = 0;
	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SelectedForm->fillType = SATF;
	std::vector<double> lengths;
	lengths.reserve(VertexCount + 1);
	length = 0;
	lengths.push_back(length);
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		deltaX = CurrentFormVertices[iVertex].x - CurrentFormVertices[iVertex - 1].x;
		deltaY = CurrentFormVertices[iVertex].y - CurrentFormVertices[iVertex - 1].y;
		length += hypot(deltaX, deltaY);
		lengths.push_back(length);
	}
	deltaX = CurrentFormVertices[0].x - CurrentFormVertices[iVertex - 1].x;
	deltaY = CurrentFormVertices[0].y - CurrentFormVertices[iVertex - 1].y;
	length += hypot(deltaX, deltaY);
	lengths.push_back(length);
	do {
		if (SatinEndGuide) {
			if (CurrentFormGuidesCount) {
				satmf(lengths);
				break;
			}
			else {
				satfn(lengths, 1, SatinEndGuide, VertexCount, SatinEndGuide + 1);
				break;
			}
		}
		if (SelectedForm->attribute & FRMEND) {
			if (CurrentFormGuidesCount) {
				satmf(lengths);
				break;
			}
			else {
				if (VertexCount == 3 && FormList[ClosestFormToCursor].attribute & 1) {
					satfn(lengths, 2, 3, 2, 1);
					break;
					;
				}
				else {
					length  = (length - lengths[1]) / 2;
					iVertex = 1;
					if (!StateMap.test(StateFlag::BARSAT)) {
						OSequence[0] = SelectedPoint = CurrentFormVertices[1];
						SequenceIndex                = 1;
					}
					while ((length > lengths[iVertex]) && (iVertex < (VertexCount + 1))) {
						iVertex++;
					}
					deltaX = lengths[iVertex] - length;
					deltaY = length - lengths[iVertex - 1];
					if (deltaY > deltaX) {
						iVertex--;
					}
					satfn(lengths, 1, iVertex, VertexCount, iVertex);
				}
				break;
			}
		}
		if (CurrentFormGuidesCount) {
			satmf(lengths);
			break;
		}
		length /= 2;
		iVertex = 0;
		if (!StateMap.test(StateFlag::BARSAT) && !StateMap.test(StateFlag::FTHR)) {
			OSequence[0] = SelectedPoint = CurrentFormVertices[0];
			SequenceIndex                = 1;
		}
		while (length > lengths[iVertex]) {
			iVertex++;
		}
		deltaX = lengths[iVertex] - length;
		deltaY = length - lengths[iVertex - 1];
		if (deltaY > deltaX)
			iVertex--;
		satfn(lengths, 0, iVertex, VertexCount, iVertex);
	} while (false);

	LineSpacing = spacing;
}

void dudif(const dPOINT& start, const dPOINT& finish, dPOINT* const delta) noexcept {
	delta->x = finish.x - start.x;
	delta->y = finish.y - start.y;
}

void trfrm(const dPOINT& bottomLeftPoint,
           const dPOINT& topLeftPoint,
           const dPOINT& bottomRightPoint,
           const dPOINT& topRightPoint) noexcept {
	unsigned iStitch  = 0;
	dPOINT   topDelta = {}, bottomDelta = {}, leftDelta = {}, rightDelta = {};
	dPOINT   clipRatio = {}, topMidpoint = {}, bottomMidpoint = {}, middleDelta = {};

	dudif(topLeftPoint, topRightPoint, &topDelta);
	dudif(bottomLeftPoint, bottomRightPoint, &bottomDelta);
	dudif(bottomLeftPoint, topLeftPoint, &leftDelta);
	dudif(bottomRightPoint, topRightPoint, &rightDelta);
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		clipRatio.x      = ClipBuffer[iStitch].x / ClipRectSize.cx;
		clipRatio.y      = ClipBuffer[iStitch].y / ClipRectSize.cy;
		topMidpoint.x    = clipRatio.x * (topDelta.x) + topLeftPoint.x;
		topMidpoint.y    = clipRatio.x * (topDelta.y) + topLeftPoint.y;
		bottomMidpoint.x = clipRatio.x * (bottomDelta.x) + bottomLeftPoint.x;
		bottomMidpoint.y = clipRatio.x * (bottomDelta.y) + bottomLeftPoint.y;
		dudif(bottomMidpoint, topMidpoint, &middleDelta);
		OSequence[ActivePointIndex].x = clipRatio.y * middleDelta.x + bottomMidpoint.x;
		OSequence[ActivePointIndex].y = clipRatio.y * middleDelta.y + bottomMidpoint.y;
		ActivePointIndex++;
	}
}

void clpfm() noexcept {
	unsigned iSequence = 0, iStep = 0, count = 0;
	double   leftLength = 0.0, rightLength = 0.0;
	dPOINT   leftDelta = {}, rightDelta = {};
	dPOINT   leftStep = {}, rightStep = {};
	dPOINT   bottomLeft = {}, bottomRight = {};
	dPOINT   topLeft = {}, topRight = {};

	ActivePointIndex = 0;
	for (iSequence = 0; iSequence < SequenceIndex - 2; iSequence += 2) {
		leftLength
		    = hypot(BSequence[iSequence + 1].x - BSequence[iSequence].x, BSequence[iSequence + 1].y - BSequence[iSequence].y);
		rightLength  = hypot(BSequence[iSequence + 3].x - BSequence[iSequence + 2].x,
                            BSequence[iSequence + 3].y - BSequence[iSequence + 2].y);
		leftDelta.x  = BSequence[iSequence + 1].x - BSequence[iSequence].x;
		leftDelta.y  = BSequence[iSequence + 1].y - BSequence[iSequence].y;
		rightDelta.x = BSequence[iSequence + 2].x - BSequence[iSequence + 3].x;
		rightDelta.y = BSequence[iSequence + 2].y - BSequence[iSequence + 3].y;
		if (rightLength > leftLength)
			count = leftLength / ClipRectSize.cy;
		else
			count = rightLength / ClipRectSize.cy;
		if (!count)
			count = 1;
		leftStep.x  = leftDelta.x / count;
		leftStep.y  = leftDelta.y / count;
		rightStep.x = rightDelta.x / count;
		rightStep.y = rightDelta.y / count;
		topLeft.x   = BSequence[iSequence].x;
		topLeft.y   = BSequence[iSequence].y;
		topRight.x  = BSequence[iSequence + 3].x;
		topRight.y  = BSequence[iSequence + 3].y;
		for (iStep = 0; iStep < count; iStep++) {
			bottomLeft  = topLeft;
			bottomRight = topRight;
			topLeft.x += leftStep.x;
			topLeft.y += leftStep.y;
			topRight.x += rightStep.x;
			topRight.y += rightStep.y;
			if (ActivePointIndex > MAXITEMS - ClipStitchCount - 1)
				return;
			trfrm(bottomLeft, topLeft, bottomRight, topRight);
		}
	}
}

void fmclp() {
	const double savedSpacing = LineSpacing;

	LineSpacing = ClipRectSize.cx;
	StateMap.set(StateFlag::BARSAT);
	satfil();
	StateMap.reset(StateFlag::BARSAT);
	clpfm();
	SelectedForm->fillType = CLPF;
	SequenceIndex          = ActivePointIndex;
	LineSpacing            = savedSpacing;
}

void refilfn() {
	double       spacing                  = 0.0;
	const double stitchLength             = UserStitchLength;
	float        length                   = 0.0;
	FILLSTARTS   fillStartsData           = {}; // fill start data for refill
	unsigned     fillStartsMap            = 0;  // fill starts bitmap
	double       rotationAngle            = 0;
	dPOINT       rotationCenter           = {};
	unsigned     interleaveSequenceIndex2 = 0; // index into interleave indices

	StateMap.reset(StateFlag::TXFIL);
	fvars(ClosestFormToCursor);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->underlayIndent = 0;
	savdo();
	fdelstch(fillStartsData, fillStartsMap);
	StateMap.set(StateFlag::WASREFIL);
	if (SelectedForm->fillSpacing < 0.5 && !isclp(ClosestFormToCursor))
		SelectedForm->fillSpacing = 0.5;
	if (SelectedForm->edgeSpacing < 0.5)
		SelectedForm->edgeSpacing = 0.5;
	UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
	if (!StateMap.test(StateFlag::WASDO))
		savdo();
	StateMap.reset(StateFlag::WASDO);
	if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK) && SelectedForm->type == FRMLINE && SelectedForm->fillType != CONTF)
		SelectedForm->type = FRMFPOLY;
	InterleaveSequenceIndex = 0;
	StateMap.reset(StateFlag::ISUND);
	std::vector<RNGCNT> textureSegments(SelectedForm->fillInfo.texture.lines);
	switch (SelectedForm->type) {
	case FRMLINE:
		HorizontalLength2 = SelectedForm->borderSize;
		switch (SelectedForm->edgeType & NEGUND) {
		case EDGELINE:
			brdfil(SelectedForm->edgeStitchLen);
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGEBEAN:
			bold(SelectedForm->edgeStitchLen);
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGECLIP:
			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpout();
			SequenceIndex = 0;
			clpbrd(0);
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGEANGSAT:
			StateMap.reset(StateFlag::SAT1);
			slbrd();
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGEPROPSAT:
			if (SelectedForm->vertexCount > 2) {
				StateMap.reset(StateFlag::SAT1);
				plbrd(SelectedForm->edgeSpacing);
				ritbrd(interleaveSequenceIndex2);
			}
			break;
		case EDGEAPPL:
			lapbrd();
			ritapbrd(interleaveSequenceIndex2);
			StateMap.reset(StateFlag::SAT1);
			slbrd();
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGEBHOL:
			HorizontalLength2      = SelectedForm->borderSize * 2;
			length                 = ButtonholeCornerLength;
			ButtonholeCornerLength = getblen();
			satout(20);
			blbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = length;
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGEPICOT:
			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			SequenceIndex          = 0;
			length                 = ButtonholeCornerLength;
			ButtonholeCornerLength = getplen();
			clpic();
			ButtonholeCornerLength = length;
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGEDOUBLE:
			dubfn();
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGELCHAIN:
			StateMap.set(StateFlag::LINCHN);
			chnfn();
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGEOCHAIN:
			StateMap.reset(StateFlag::LINCHN);
			chnfn();
			ritbrd(interleaveSequenceIndex2);
			break;
		case EDGECLIPX:
			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			duxclp();
			ritbrd(interleaveSequenceIndex2);
			break;
		}
		if (SelectedForm->fillType == CONTF && SelectedForm->attribute & FRECONT) {
			contf();
			ritfil(interleaveSequenceIndex2);
		}
		break;
	case FRMFPOLY:
		chkcwlk(interleaveSequenceIndex2);
		chkwlk(interleaveSequenceIndex2);
		chkund(textureSegments, interleaveSequenceIndex2);
		StateMap.reset(StateFlag::ISUND);
		if (SelectedForm->fillType) {
			spacing     = LineSpacing;
			LineSpacing = SelectedForm->fillSpacing;
			std::vector<SMALPNTL> lineEndpoints;
			std::vector<unsigned> groupIndexSequence;
			bool                  doFill = true;
			switch (gsl::narrow<unsigned>(SelectedForm->fillType)) {
			case VRTF:
				fnvrt(groupIndexSequence, lineEndpoints);
				WorkingFormVertices = SelectedForm->vertices;
				break;
			case HORF:
				rotationAngle = PI / 2;
				fnhor(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter);
				WorkingFormVertices = AngledForm.vertices;
				break;
			case ANGF:
				rotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
				fnang(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter);
				WorkingFormVertices = AngledForm.vertices;
				break;
			case VCLPF:
				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				fvars(ClosestFormToCursor);
				clpcon(textureSegments);
				doFill = false;
				break;
			case HCLPF:
				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				horclpfn(textureSegments);
				doFill = false;
				break;
			case ANGCLPF:
				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				StateMap.reset(StateFlag::ISUND);
				angclpfn(textureSegments);
				doFill = false;
				break;
			case TXVRTF:
				setxt(textureSegments);
				clpcon(textureSegments);
				doFill = false;
				break;
			case TXHORF:
				setxt(textureSegments);
				horclpfn(textureSegments);
				doFill = false;
				break;
			case TXANGF:
				setxt(textureSegments);
				StateMap.reset(StateFlag::ISUND);
				angclpfn(textureSegments);
				doFill = false;
				break;
			}
			if (doFill) {
				lcon(groupIndexSequence, lineEndpoints);
				bakseq();
				if (SelectedForm->fillType != VRTF && SelectedForm->fillType != TXVRTF) {
					rotationAngle = -rotationAngle;
					rotbak(rotationAngle, rotationCenter);
				}
			}
			ritfil(interleaveSequenceIndex2);
			LineSpacing = spacing;
		}
		chkbrd(interleaveSequenceIndex2);
		break;
	case SAT:
		chkcwlk(interleaveSequenceIndex2);
		chkwlk(interleaveSequenceIndex2);
		chkund(textureSegments, interleaveSequenceIndex2);
		StateMap.reset(StateFlag::ISUND);
		switch (SelectedForm->fillType) {
		case SATF:
			spacing          = LineSpacing;
			LineSpacing      = SelectedForm->fillSpacing;
			UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
			satfil();
			LineSpacing = spacing;
			ritfil(interleaveSequenceIndex2);
			break;
		case CLPF:
			oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
			fmclp();
			ritfil(interleaveSequenceIndex2);
			break;
		case FTHF:
			if (StateMap.testAndReset(StateFlag::FLPBLND))
				SelectedForm->extendedAttribute ^= AT_FTHBLND;
			StateMap.set(StateFlag::CNV2FTH);
			fthrfn(interleaveSequenceIndex2);
			break;
		}
		chkbrd(interleaveSequenceIndex2);
	}
	UserStitchLength = stitchLength;
	intlv(fillStartsData, fillStartsMap, interleaveSequenceIndex2);
	ritot(PCSHeader.stitchCount);
	setfchk();
}

void refil() {
	unsigned codedForm = 0, attribute = 0;

	if (!UserFlagMap.test(UserFlag::WRNOF)) {
		codedForm = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT) | USMSK;
		for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			attribute = StitchBuffer[iStitch].attribute;
			if (!(attribute & NOTFRM) && (attribute & (USMSK | FRMSK)) == codedForm) {
				if (FormDataSheet)
					StateMap.set(StateFlag::WASFRMFRM);
				undat();
				tabmsg(IDS_REFIL);
				StateMap.set(StateFlag::MOVMSG);
				okcan();
				return;
			}
		}
	}
	refilfn();
}

void setfpnt() {
	fvars(ClosestFormToCursor);

	POINT screenCoordinate = { (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };
	if (fPOINT* vertices = &SelectedForm->vertices[0]) {
		unfrm();
		rats();
		px2stchf(screenCoordinate, vertices[ClosestVertexToCursor]);
		frmout(ClosestFormToCursor);
		refil();
		StateMap.set(StateFlag::WASFPNT);
		StateMap.reset(StateFlag::SELBOX);
		StateMap.set(StateFlag::FRMPSEL);
		ritfcor(CurrentFormVertices[ClosestVertexToCursor]);
		StateMap.set(StateFlag::RESTCH);
	}
}

void makpoli() noexcept {
	if (SelectedForm->type == SAT) {
		if (SelectedForm->satinGuideCount)
			delsac(ClosestFormToCursor);
	}
	SelectedForm->type = FRMFPOLY;
}

void fsvrt() {
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType  = VRTF;
	SelectedForm->type      = FRMFPOLY;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type        = FRMFPOLY;
	dusqr();
	refilfn();
}

void filvrt() {
	if (filmsgs(FMM_VRT))
		return;
	if (SelectedFormList->size()) {
		savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				fsvrt();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			fsvrt();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void fshor() {
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType  = HORF;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing           = LineSpacing;
	SelectedForm->angleOrClipData.angle = (float)PI / 2;
	SelectedForm->type                  = FRMFPOLY;
	dusqr();
	refil();
}

void filhor() {
	if (filmsgs(FMM_HOR))
		return;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				fshor();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fshor();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void fsangl() {
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType              = ANGF;
	SelectedForm->angleOrClipData.angle = static_cast<float>(IniFile.fillAngle);
	SelectedForm->fillColor             = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type        = FRMFPOLY;
	dusqr();
	refil();
}

void filangl() {
	if (filmsgs(FMM_ANG))
		return;
	if (SelectedFormList->size()) {
		savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				fsangl();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			fsangl();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

unsigned chkfrm(std::vector<POINT>& stretchBoxLine, double& xyRatio) {
	fvars(ClosestFormToCursor);

	POINT    point      = { (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };
	POINT    formOrigin = {};
	RECT     rectangle  = {};
	unsigned iControl   = 0;
	double   length = 0.0, minimumLength = 1e99;

	NewFormVertexCount = SelectedForm->vertexCount + 1;
	duzrat();
	sRct2px(SelectedForm->rectangle, rectangle);
	auto& formControls = *FormControlPoints;
	formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = rectangle.left;
	formControls[1].x = formControls[5].x = midl(rectangle.right, rectangle.left);
	formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = rectangle.top;
	formControls[3].y = formControls[7].y = midl(rectangle.top, rectangle.bottom);
	formControls[4].y = formControls[5].y = formControls[6].y = rectangle.bottom;
	formControls[2].x = formControls[3].x = formControls[4].x = rectangle.right;
	for (iControl = 0; iControl < 10; iControl++) {
		length = hypot(formControls[iControl].x - point.x, formControls[iControl].y - point.y);
		if (length < minimumLength) {
			minimumLength             = length;
			SelectedFormControlVertex = iControl;
		}
		if (minimumLength < CLOSENUF) {
			ritfrct(ClosestFormToCursor, StitchWindowDC);
			for (size_t iCorner = 0u; iCorner < 4; iCorner++) {
				stretchBoxLine[iCorner] = formControls[iCorner << 1];
			}
			stretchBoxLine[4] = stretchBoxLine[0];
			strtchbox(stretchBoxLine);
			if (SelectedFormControlVertex & 1)
				StateMap.set(StateFlag::STRTCH);
			else {
				StateMap.set(StateFlag::EXPAND);
				xyRatio = static_cast<double>(SelectedForm->rectangle.right - SelectedForm->rectangle.left)
				          / (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom);
			}
			SelectedFormControlVertex >>= 1;
			StateMap.set(StateFlag::SHOSTRTCH);
			return 1;
		}
	}
	if (point.x >= rectangle.left && point.x <= rectangle.right && point.y >= rectangle.top && point.y <= rectangle.bottom) {
		sfCor2px(SelectedForm->vertices[0], formOrigin);
		FormMoveDelta.x = formOrigin.x - point.x;
		FormMoveDelta.y = formOrigin.y - point.y;
		StateMap.set(StateFlag::FRMOV);
		return 1;
	}
	else
		return 0;
}

void rstfrm() {
	fPOINT   offset  = {};
	unsigned iVertex = 0, iStitch = 0;
	POINT    point
	    = { (Msg.pt.x + gsl::narrow<long>(round(FormMoveDelta.x))), (Msg.pt.y + gsl::narrow<long>(round(FormMoveDelta.y))) };
	const unsigned attribute = gsl::narrow<unsigned int>(ClosestFormToCursor << 4);

	setmfrm();
	StateMap.reset(StateFlag::FRMOV);
	pxCor2stch(point);
	offset.x = SelectedPoint.x - SelectedForm->vertices[0].x;
	offset.y = SelectedPoint.y - SelectedForm->vertices[0].y;
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		SelectedForm->vertices[iVertex].x += offset.x;
		SelectedForm->vertices[iVertex].y += offset.y;
	}
	SelectedForm->rectangle.bottom += offset.y;
	SelectedForm->rectangle.top += offset.y;
	SelectedForm->rectangle.left += offset.x;
	SelectedForm->rectangle.right += offset.x;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) == attribute && StitchBuffer[iStitch].attribute & ALTYPMSK
		    && !(StitchBuffer[iStitch].attribute & NOTFRM)) {
			StitchBuffer[iStitch].x += offset.x;
			StitchBuffer[iStitch].y += offset.y;
		}
	}
}

void clrfills() noexcept {
	unsigned iForm;

	for (iForm = 0; iForm < FormIndex; iForm++) {
		FormList[iForm].clipEntries             = 0;
		FormList[iForm].lengthOrCount.clipCount = 0;
		FormList[iForm].edgeType                = 0;
		FormList[iForm].fillType                = 0;
		FormList[iForm].attribute &= NFRECONT;
		FormList[iForm].extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
	}
	ClipPointIndex = 0;
}

void dusat() noexcept {
	const POINT* line = &FormLines[SatinIndex - 1];

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, line, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unsat() {
	if (StateMap.testAndReset(StateFlag::SHOSAT))
		dusat();
}

void drwsat() {
	unsat();
	px2stch();
	FormLines[SatinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[SatinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOSAT);
	dusat();
}

void satpnt0() {
	px2stch();
	FormLines[0].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[0].y = Msg.pt.y - StitchWindowOrigin.y;
	TempPolygon->push_back(SelectedPoint);
	SatinIndex = 1;
	StateMap.set(StateFlag::SATPNT);
}

void satpnt1() {
	unsat();
	px2stch();
	FormLines[SatinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[SatinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	dusat();
	TempPolygon->push_back(SelectedPoint);
	SatinIndex++;
	StateMap.set(StateFlag::RESTCH);
}

void satfix() {
	unsigned iVertex = 0;

	if (SatinIndex > 1) {
		FormList[FormIndex].vertices = adflt(SatinIndex);
		for (iVertex = 0; iVertex < SatinIndex; iVertex++) {
			FormList[FormIndex].vertices[iVertex] = (*TempPolygon)[iVertex];
		}
		TempPolygon->clear();
		FormList[FormIndex].vertexCount = SatinIndex;
		frmout(FormIndex);
		FormList[FormIndex].satinGuideCount = 0;
		FormIndex++;
		StateMap.set(StateFlag::INIT);
	}
	StateMap.reset(StateFlag::SHOSAT);
	StateMap.set(StateFlag::RESTCH);
}

void delcon(unsigned GuideIndex) {
	// ToDo - Find a better way to calculate the offset into the SatinGuides array
	const unsigned iGuide = &SelectedForm->satinOrAngle.guide[GuideIndex] - SatinGuides;
	SATCON*        guide  = &SelectedForm->satinOrAngle.guide[GuideIndex];

	if (guide) {
		if (SatinGuideIndex > iGuide) {
			// Todo - use std::vector & member erase() ?
			std::copy(
			    guide + 1, guide + (SatinGuideIndex - iGuide + 1), stdext::make_checked_array_iterator(guide, SatinGuideIndex));
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			FRMHED* formHeader = &FormList[iForm];
			if (formHeader->type == SAT && formHeader->satinGuideCount)
				formHeader->satinOrAngle.guide--;
		}
		if (ClosestVertexToCursor < SatinEndGuide)
			SatinEndGuide--;
		SelectedForm->satinGuideCount--;
		SatinGuideIndex--;
		CurrentFormGuidesCount = SelectedForm->satinGuideCount;
		if (SelectedForm->fillType == SATF)
			refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

bool satselfn() {
	unsigned iForm = 0, iVertex = 0, layerCode = 0;
	double   length = 0, minimumLength = 1e99;
	double   deltaX = 0.0, deltaY = 0.0;

	px2stch();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		layerCode = (FormList[iForm].attribute & FRMLMSK) >> 1;
		if (!ActiveLayer || !layerCode || layerCode == ActiveLayer) {
			for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++) {
				deltaX = SelectedPoint.x - FormList[iForm].vertices[iVertex].x;
				deltaY = SelectedPoint.y - FormList[iForm].vertices[iVertex].y;
				length = hypot(deltaX, deltaY);
				if (length < minimumLength) {
					minimumLength         = length;
					ClosestFormToCursor   = iForm;
					ClosestVertexToCursor = iVertex;
				}
			}
		}
	}
	if (minimumLength < CLOSENUF)
		return 1;
	else
		return 0;
}

void satsel() {
	if (satselfn()) {
		fvars(ClosestFormToCursor);
		duzrat();
		StartPoint = ClosestVertexToCursor;
		sfCor2px(SelectedForm->vertices[ClosestVertexToCursor], FormLines[0]);
		StateMap.reset(StateFlag::SHOCON);
		StateMap.set(StateFlag::SATCNKT);
		if (SelectedForm->type == FRMFPOLY)
			SelectedForm->type = SAT;
	}
}

void ducon() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, FormLines, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uncon() {
	if (StateMap.testAndReset(StateFlag::SHOCON))
		ducon();
}

void drwcon() {
	uncon();
	FormLines[1].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[1].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOCON);
	ducon();
}

bool scomp(const SATCON& arg1, const SATCON& arg2) noexcept {
	return (arg1.start < arg2.start);
}

void duinsf() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uninsf() {
	if (StateMap.testAndReset(StateFlag::SHOINSF))
		duinsf();
}

void rotfrm(size_t newStartVertex) {
	fvars(ClosestFormToCursor);

	fPOINT* selectedVertices = CurrentFormVertices;
	if (selectedVertices) {
		std::vector<fPOINT> rotatedVertices(VertexCount);
		unsigned            iVertex = 0, iGuide = 0, iRotatedGuide = 0;
		size_t              iRotated = newStartVertex;
		size_t              tlin     = 0;

		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			rotatedVertices[iVertex] = selectedVertices[iVertex];
		}
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			selectedVertices[iVertex] = rotatedVertices[iRotated];
			iRotated                  = nxt(iRotated);
		}
		iRotatedGuide = 0;
		if (SelectedForm->type == SAT) {
			if (SelectedForm->wordParam)
				SelectedForm->wordParam
				    = (SelectedForm->wordParam + SelectedForm->vertexCount - newStartVertex) % SelectedForm->vertexCount;
			for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
				if (CurrentFormGuides[iGuide].start != newStartVertex && CurrentFormGuides[iGuide].finish != newStartVertex) {
					CurrentFormGuides[iRotatedGuide].start
					    = (CurrentFormGuides[iGuide].start + VertexCount - newStartVertex) % VertexCount;
					CurrentFormGuides[iRotatedGuide].finish
					    = (CurrentFormGuides[iGuide].finish + VertexCount - newStartVertex) % VertexCount;
					if (CurrentFormGuides[iRotatedGuide].start > CurrentFormGuides[iRotatedGuide].finish) {
						tlin                                   = CurrentFormGuides[iRotatedGuide].start;
						CurrentFormGuides[iRotatedGuide].start = CurrentFormGuides[iRotatedGuide].finish;
						CurrentFormGuides[iGuide].finish       = tlin;
					}
					iRotatedGuide++;
				}
			}
		}
		if (iRotatedGuide) {
			SelectedForm->satinGuideCount = iRotatedGuide;
			// ToDo - Can we do the sort in place?
			std::vector<SATCON> rotatedGuides(iRotatedGuide);
			for (iGuide = 0; iGuide < iRotatedGuide; iGuide++) {
				rotatedGuides[iGuide] = CurrentFormGuides[iGuide];
			}
			std::sort(rotatedGuides.begin(), rotatedGuides.end(), scomp);
			for (iGuide = 0; iGuide < iRotatedGuide; iGuide++) {
				CurrentFormGuides[iGuide] = rotatedGuides[iGuide];
			}
		}
		if (SelectedForm->extendedAttribute & AT_STRT)
			SelectedForm->fillStart = (SelectedForm->fillStart + VertexCount - newStartVertex) % VertexCount;
		if (SelectedForm->extendedAttribute & AT_END)
			SelectedForm->fillEnd = (SelectedForm->fillEnd + VertexCount - newStartVertex) % VertexCount;
	}
}

void satclos() {
	unsigned iVertex           = 0;
	size_t   closestVertex     = StartPoint;
	size_t   initialGuideCount = SelectedForm->satinGuideCount;
	double   deltaX = 0.0, deltaY = 0.0, length = 0.0, minimumLength = 1e99;

	uninsf();
	px2stch();
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		deltaX = SelectedPoint.x - SelectedForm->vertices[iVertex].x;
		deltaY = SelectedPoint.y - SelectedForm->vertices[iVertex].y;
		length = hypot(deltaX, deltaY);
		if (length < minimumLength) {
			minimumLength         = length;
			ClosestVertexToCursor = iVertex;
		}
	}
	StateMap.reset(StateFlag::SATCNKT);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->fillType = CONTF;
		closestVertex          = ClosestVertexToCursor;
		if (StartPoint > closestVertex) {
			std::swap(closestVertex, StartPoint);
		}
		if (!StartPoint)
			StartPoint++;
		if (StartPoint == SelectedForm->vertexCount - 2
		    && closestVertex == gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1) {
			StartPoint    = 1;
			closestVertex = SelectedForm->vertexCount - 2;
		}
		if (closestVertex >= gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2) {
			closestVertex = SelectedForm->vertexCount - 2;
			if (StartPoint >= gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2)
				StartPoint = SelectedForm->vertexCount - 2;
		}
		if (closestVertex - StartPoint < 2) {
			closestVertex = StartPoint + 2;
			if (closestVertex > gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2) {
				closestVertex = closestVertex - SelectedForm->vertexCount - 2;
				closestVertex -= closestVertex;
				StartPoint -= closestVertex;
			}
		}
		SelectedForm->angleOrClipData.guide.start  = StartPoint;
		SelectedForm->angleOrClipData.guide.finish = closestVertex;
	}
	else {
		if (ClosestVertexToCursor < closestVertex) {
			std::swap(ClosestVertexToCursor, closestVertex);
		}
		if (closestVertex == 0 && ClosestVertexToCursor == VertexCount - 1) {
			closestVertex         = VertexCount - 1;
			ClosestVertexToCursor = VertexCount;
		}
		if (closestVertex == 1 && ClosestVertexToCursor == VertexCount) {
			closestVertex         = 0;
			ClosestVertexToCursor = 1;
		}
		if (ClosestVertexToCursor - closestVertex == 1) {
			if (SelectedForm->attribute & FRMEND)
				SelectedForm->wordParam = closestVertex;
			else {
				if (closestVertex)
					rotfrm(closestVertex);
				SelectedForm->attribute |= FRMEND;
			}
			satadj();
		}
		else {
			if (CurrentFormGuidesCount) {
				sacspac(&SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount], 1);
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount].start    = closestVertex;
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount++].finish = ClosestVertexToCursor;
				satadj();
			}
			else {
				SelectedForm->satinOrAngle.guide                           = nusac(ClosestFormToCursor, 1);
				SelectedForm->satinOrAngle.guide[initialGuideCount].start  = closestVertex;
				SelectedForm->satinOrAngle.guide[initialGuideCount].finish = ClosestVertexToCursor;
				SelectedForm->satinGuideCount                              = 1;
			}
		}
	}
}

void satknkt() {
	satclos();
	fvars(ClosestFormToCursor);
	refil();
	StateMap.set(StateFlag::RESTCH);
}

extern void prbug();

void filsfn() {
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->type = SAT;
	fsizpar();
	SelectedForm->fillType    = SATF;
	SelectedForm->fillColor   = ActiveColor;
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type        = SAT;
	refilfn();
}

void filsat() {
	if (filmsgs(FMM_FAN))
		return;
	if (SelectedFormList->size()) {
		savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				filsfn();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			filsfn();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

unsigned closat() {
	unsigned iForm         = 0;
	size_t   lastVertex    = 0;
	double   minimumLength = 1e99;
	double   length        = 0;
	size_t   savedVertex   = 0;
	float    param         = 0.0;

	px2stch();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (!ActiveLayer || gsl::narrow<unsigned>((FormList[iForm].attribute & FRMLMSK) >> 1) == ActiveLayer
		    || !(FormList[iForm].attribute & FRMLMSK)) {
			CurrentFormVertices = FormList[iForm].vertices;
			savedVertex         = VertexCount;
			VertexCount         = FormList[iForm].vertexCount;
			if (FormList[iForm].type == FRMLINE) {
				lastVertex = VertexCount - 1;
			}
			else {
				lastVertex = VertexCount;
			}
			// Loop through for all line segments
			for (size_t iVertex = 0; iVertex < lastVertex; iVertex++) {
				param
				    = findDistanceToSide(CurrentFormVertices[iVertex], CurrentFormVertices[nxt(iVertex)], SelectedPoint, length);
				if ((length < minimumLength)) {
					if ((param < 0.0) && (iVertex == 0)) {
						// this should only happen if the Closest vertex is the start of a line (vertex 0)
						minimumLength         = length;
						ClosestFormToCursor   = iForm;
						ClosestVertexToCursor = iVertex;
						InOutFlag             = POINT_BEFORE_LINE;
					}
					else {
						// return the vertex after the intersection
						if ((param > 1.0) && (iVertex == lastVertex - 1)) {
							minimumLength         = length;
							ClosestFormToCursor   = iForm;
							ClosestVertexToCursor = nxt(iVertex);
							InOutFlag             = POINT_AFTER_LINE;
						}
						else {
							minimumLength         = length;
							ClosestFormToCursor   = iForm;
							ClosestVertexToCursor = nxt(iVertex);
							InOutFlag             = POINT_IN_LINE;
						}
					}
				}
			}
			VertexCount = savedVertex;
		}
	}
	if (minimumLength == 1e99)
		return 0;
	else
		return 1;
}

void nufpnt(size_t vertex) noexcept {
	unsigned ind       = 0;
	size_t   newVertex = vertex + 1;

	fltspac(&FormForInsert->vertices[newVertex], 1);
	FormForInsert->vertices[newVertex] = SelectedPoint;
	FormForInsert->vertexCount++;
	for (ind = 0; ind < FormForInsert->satinGuideCount; ind++) {
		if (FormForInsert->satinOrAngle.guide[ind].start > newVertex - 1)
			FormForInsert->satinOrAngle.guide[ind].start++;
		if (FormForInsert->satinOrAngle.guide[ind].finish > newVertex - 1)
			FormForInsert->satinOrAngle.guide[ind].finish++;
	}
	if (FormForInsert->wordParam >= newVertex) {
		FormForInsert->wordParam++;
		FormForInsert->wordParam %= VertexCount;
	}
	if (FormForInsert->fillType == CONTF) {
		if (FormForInsert->angleOrClipData.guide.start > newVertex - 1)
			FormForInsert->angleOrClipData.guide.start++;
		if (FormForInsert->angleOrClipData.guide.finish > newVertex - 1)
			FormForInsert->angleOrClipData.guide.finish++;
	}
	frmlin(FormForInsert->vertices, FormForInsert->vertexCount);
}

double p2p(const fPOINT& point0, const fPOINT& point1) noexcept {
	return hypot(point0.x - point1.x, point0.y - point1.y);
}

void insat() {
	size_t lastVertex = 0;

	if (closat()) {
		savdo();
		SelectedForm  = &FormList[ClosestFormToCursor];
		FormForInsert = SelectedForm;
		lastVertex    = FormForInsert->vertexCount - 1;
		fvars(ClosestFormToCursor);
		if (InOutFlag) {
			if (ClosestVertexToCursor == 0 && FormForInsert->type == FRMLINE) {
				StateMap.set(StateFlag::PRELIN);
			}
			else {
				if (ClosestVertexToCursor != lastVertex && FormForInsert->type == FRMLINE) {
					ClosestVertexToCursor = prv(ClosestVertexToCursor);
				}
			}
			nufpnt(ClosestVertexToCursor);
			if (StateMap.testAndReset(StateFlag::PRELIN)) {
				SelectedPoint.x            = FormForInsert->vertices[0].x;
				SelectedPoint.y            = FormForInsert->vertices[0].y;
				FormForInsert->vertices[0] = FormForInsert->vertices[1];
				FormForInsert->vertices[1] = SelectedPoint;
			}
		}
		else {
			ClosestVertexToCursor = prv(ClosestVertexToCursor);
			nufpnt(ClosestVertexToCursor);
		}
		refil();
	}
	StateMap.set(StateFlag::RESTCH);
}

bool chkdel() noexcept {
	if (SelectedForm->type == FRMLINE) {
		if (SelectedForm->vertexCount > 2)
			return 0;
		else
			return 1;
	}
	else {
		if (SelectedForm->vertexCount > 3)
			return 0;
		else
			return 1;
	}
}

void delspnt() {
	unsigned iGuide     = 0;
	SATCON*  guide      = nullptr;
	fPOINT   vertex     = {};
	FRMHED*  formHeader = nullptr;

	fvars(ClosestFormToCursor);
	if (chkdel()) {
		StateMap.set(StateFlag::DELTO);
		frmdel();
		StateMap.reset(StateFlag::FRMPSEL);
		coltab();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedForm->type == SAT) {
		if (ClosestVertexToCursor < SelectedForm->wordParam)
			SelectedForm->wordParam--;
		if (SelectedForm->satinGuideCount) {
			guide = SelectedForm->satinOrAngle.guide;
			if (guide) {
				iGuide = 0;
				while (guide[iGuide].start != ClosestVertexToCursor && guide[iGuide].finish != ClosestVertexToCursor
				       && iGuide < SelectedForm->satinGuideCount)
					iGuide++;
				if (iGuide < SelectedForm->satinGuideCount
				    && (guide[iGuide].start == ClosestVertexToCursor || guide[iGuide].finish == ClosestVertexToCursor)) {
					while (iGuide < SelectedForm->satinGuideCount) {
						guide[iGuide] = guide[iGuide + 1];
						iGuide++;
					}
					SelectedForm->satinGuideCount--;
					SatinGuideIndex--;
					for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
						formHeader = &FormList[iForm];
						if (formHeader->type == SAT && formHeader->satinGuideCount)
							formHeader->satinOrAngle.guide++;
					}
				}
				for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
					if (guide[iGuide].start > ClosestVertexToCursor)
						guide[iGuide].start--;
					if (guide[iGuide].finish > ClosestVertexToCursor)
						guide[iGuide].finish--;
				}
			}
		}
	}
	// ToDo - convert to vector and use erase()
	std::copy(SelectedForm->vertices + ClosestVertexToCursor + 1,
	          SelectedForm->vertices + FormVertexIndex,
	          stdext::make_checked_array_iterator(SelectedForm->vertices + ClosestVertexToCursor,
	                                              FormVertexIndex - (ClosestVertexToCursor + 1)));
	SelectedForm->vertexCount--;
	FormVertexIndex--;
	fvars(ClosestFormToCursor);
	if (ClosestVertexToCursor > gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1)
		ClosestVertexToCursor = SelectedForm->vertexCount - 1;
	StateMap.set(StateFlag::FRMPSEL);
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
		FormList[iForm].vertices--;
	ritfcor(CurrentFormVertices[ClosestVertexToCursor]);
	ritnum(STR_NUMPNT, ClosestVertexToCursor);
	frmout(ClosestFormToCursor);
	vertex = SelectedForm->vertices[ClosestVertexToCursor];
	if (vertex.x < ZoomRect.left || vertex.x > ZoomRect.right || vertex.y < ZoomRect.bottom || vertex.y > ZoomRect.top)
		shft(vertex);
	refil();
}

void unfil() {
	unsigned iStitch = 0, iSource = 0, iDestination = 0;
	unsigned codedForm = 0, attribute = 0;
	unsigned mapLength = 0;

	if (filmsgs(FMX_UNF))
		return;
	if (SelectedFormList->size()) {
		boost::dynamic_bitset<> formMap(FormIndex);
		for (auto selectedForm : (*SelectedFormList)) {
			SelectedForm = &FormList[selectedForm];
			if (SelectedForm->fillType || SelectedForm->edgeType) {
				delclps(selectedForm);
				deltx();
				formMap.set(selectedForm);
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				SelectedForm->extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
			}
		}
		iDestination = 0;
		for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
			if (!formMap.test((StitchBuffer[iSource].attribute & FRMSK) >> FRMSHFT)) {
				StitchBuffer[iDestination++] = StitchBuffer[iSource];
			}
		}
		PCSHeader.stitchCount = iDestination;
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (!StateMap.testAndReset(StateFlag::IGNOR) && !UserFlagMap.test(UserFlag::WRNOF)) {
				codedForm = gsl::narrow<unsigned int>((ClosestFormToCursor << FRMSHFT) | USMSK);
				mapLength = StitchBuffer[0].attribute & (FRMSK | USMSK);
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
					attribute = StitchBuffer[iStitch].attribute;
					if (!(attribute & NOTFRM) && (attribute & (USMSK | FRMSK)) == codedForm) {
						tabmsg(IDS_UNFIL);
						StateMap.set(StateFlag::FILMSG);
						okcan();
						StateMap.set(StateFlag::IGNOR);
						return;
					}
				}
			}
			codedForm    = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
			iDestination = 0;
			for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
				if ((StitchBuffer[iSource].attribute & FRMSK) != codedForm || (StitchBuffer[iSource].attribute & NOTFRM)) {
					StitchBuffer[iDestination++] = StitchBuffer[iSource];
				}
			}
			delclps(ClosestFormToCursor);
			deltx();
			SelectedForm->fillType = 0;
			SelectedForm->edgeType = 0;
			SelectedForm->extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
			PCSHeader.stitchCount = iDestination;
			ritot(PCSHeader.stitchCount);
		}
	}
}

void satzum() {
	StateMap.reset(StateFlag::SHOSAT);
	duzrat();
	VertexCount = SatinIndex;
	frmlin(*TempPolygon);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, FormPen);
	PolylineInt(StitchWindowMemDC, FormLines, SatinIndex);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	drwsat();
}

void frm0() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		savdo();
		rotfrm(ClosestVertexToCursor);
		ClosestVertexToCursor = 0;
		satadj();
		refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void rinfrm() {
	frmlin(FormForInsert->vertices, FormForInsert->vertexCount);
	SelectObject(StitchWindowMemDC, FormPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	if (FormVertexNext || FormForInsert->type != FRMLINE)
		Polyline(StitchWindowMemDC, &FormLines[FormVertexPrev], 2);
	InsertLine[0]   = FormLines[FormVertexPrev];
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOINSF);
	duinsf();
}

void infrm() {
	if (closat()) {
		FormForInsert = &FormList[ClosestFormToCursor];
		fvars(ClosestFormToCursor);
		if (InOutFlag) {
			if (!ClosestVertexToCursor && FormForInsert->type == FRMLINE) {
				FormVertexPrev = 0;
				StateMap.set(StateFlag::PRELIN);
			}
			else {
				FormVertexPrev = prv(ClosestVertexToCursor);
				FormVertexNext = ClosestVertexToCursor;
			}
		}
		else {
			FormVertexNext = ClosestVertexToCursor;
			FormVertexPrev = prv(ClosestVertexToCursor);
		}
		StateMap.set(StateFlag::INSFRM);
		StateMap.set(StateFlag::INIT);
		rinfrm();
	}
}

void setins() {
	px2stch();
	nufpnt(FormVertexPrev);
	if (StateMap.test(StateFlag::PRELIN)) {
		SelectedPoint.x            = FormForInsert->vertices[0].x;
		SelectedPoint.y            = FormForInsert->vertices[0].y;
		FormForInsert->vertices[0] = FormForInsert->vertices[1];
		FormForInsert->vertices[1] = SelectedPoint;
	}
	else {
		VertexCount    = FormForInsert->vertexCount;
		FormVertexPrev = nxt(FormVertexPrev);
		FormVertexNext = nxt(FormVertexPrev);
	}
	frmlin(FormForInsert->vertices, FormForInsert->vertexCount);
	InsertLine[0]   = FormLines[FormVertexPrev];
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::INSFRM);
	duinsf();
	StateMap.set(StateFlag::RESTCH);
}

void bsizpar() noexcept {
	SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	SelectedForm->edgeStitchLen      = UserStitchLength;
	SelectedForm->minBorderStitchLen = MinStitchLength;
}

void sbord() {
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGELINE;
	bsizpar();
	refilfn();
}

void bord() {
	if (filmsgs(FML_LIN))
		return;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			SelectedForm->borderColor = ActiveColor;
			sbord();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->borderColor = ActiveColor;
			sbord();
			coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void fsclp() {
	unsigned iStitch = 0;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType       = EDGECLIP;
	SelectedForm->clipEntries    = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize     = ClipRectSize.cy;
	SelectedForm->edgeSpacing    = ClipRectSize.cx;
	SelectedForm->borderColor    = ActiveColor;
	bsizpar();
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->borderClipData[iStitch] = ClipBuffer[iStitch];
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
	clpout();
	refilfn();
}

void fclp() {
	if (filmsgs(FML_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		fvars(ClosestFormToCursor);
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			savdo();
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (SelectedFormList->size()) {
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						fvars(ClosestFormToCursor);
						fsclp();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void satsbrd() {
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEANGSAT;
	if (UserFlagMap.test(UserFlag::DUND))
		SelectedForm->edgeType |= EGUND;
	bsizpar();
	SelectedForm->borderSize  = BorderWidth;
	SelectedForm->edgeSpacing = LineSpacing / 2;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void satbrd() {
	if (filmsgs(FML_ANGS))
		return;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satsbrd();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satsbrd();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void sapliq() {
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEAPPL;
	if (UserFlagMap.test(UserFlag::DUND))
		SelectedForm->edgeType |= EGUND;
	SelectedForm->edgeSpacing = LineSpacing / 2;
	SelectedForm->borderSize  = IniFile.borderWidth;
	bsizpar();
	SelectedForm->borderColor = ActiveColor | (AppliqueColor << 4);
	if (SelectedForm->type != FRMLINE) {
		if (SelectedForm->fillType == SAT && SelectedForm->satinGuideCount)
			delsac(ClosestFormToCursor);
	}
	SelectedForm->fillType = 0;
	SelectedForm->type     = FRMFPOLY;
	refilfn();
}

void apliq() {
	if (filmsgs(FML_APLQ))
		return;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			sapliq();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			sapliq();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void setap() {
	std::wstring fmtStr;
	loadString(fmtStr, IDS_APCOL);

	AppliqueColor = ActiveColor;
	shoMsg(fmt::format(fmtStr, (AppliqueColor + 1)));
}

void getbig() noexcept {
	unsigned    iForm = 0, iStitch = 0;
	fRECTANGLE* trct = nullptr;

	AllItemsRect.bottom = AllItemsRect.left = 1e9;
	AllItemsRect.top = AllItemsRect.right = 0;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		trct = &FormList[iForm].rectangle;
		if (trct->bottom < AllItemsRect.bottom)
			AllItemsRect.bottom = trct->bottom;
		if (trct->left < AllItemsRect.left)
			AllItemsRect.left = trct->left;
		if (trct->right > AllItemsRect.right)
			AllItemsRect.right = trct->right;
		if (trct->top > AllItemsRect.top)
			AllItemsRect.top = trct->top;
	}
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (StitchBuffer[iStitch].x < AllItemsRect.left)
			AllItemsRect.left = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].x > AllItemsRect.right)
			AllItemsRect.right = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].y < AllItemsRect.bottom)
			AllItemsRect.bottom = StitchBuffer[iStitch].y;
		if (StitchBuffer[iStitch].y > AllItemsRect.top)
			AllItemsRect.top = StitchBuffer[iStitch].y;
	}
}

void stchrct2px(const fRECTANGLE& stitchRect, RECT& screenRect) {
	dPOINT stitchCoord = { stitchRect.left, stitchRect.top };
	POINT  screenCoord = {};

	sCor2px(stitchCoord, screenCoord);
	screenRect.left = screenCoord.x;
	screenRect.top  = screenCoord.y;
	stitchCoord.x   = stitchRect.right;
	stitchCoord.y   = stitchRect.bottom;
	sCor2px(stitchCoord, screenCoord);
	screenRect.right  = screenCoord.x;
	screenRect.bottom = screenCoord.y;
}

void selal() {
	StateMap.reset(StateFlag::FORMSEL);
	SelectedFormList->clear();
	StateMap.reset(StateFlag::SELBOX);
	StateMap.reset(StateFlag::GRPSEL);
	getbig();
	ZoomRect.bottom = 0;
	ZoomRect.left   = 0;
	ZoomRect.right  = UnzoomedRect.x;
	ZoomRect.top    = UnzoomedRect.y;
	ZoomFactor      = 1;
	StateMap.reset(StateFlag::ZUMED);
	movStch();
	NearestCount = 0;
	StateMap.reset(StateFlag::RUNPAT);
	duzrat();
	stchrct2px(AllItemsRect, SelectedFormsRect);
	StateMap.set(StateFlag::BIGBOX);
	StateMap.set(StateFlag::RESTCH);
}

void setstrtch() {
	FLOAT      reference = 0.0;
	double     ratio     = 1.0;
	unsigned   iForm = 0, iVertex = 0, iStitch = 0;
	size_t     currentVertex = 0;
	fRECTANGLE stitchRect    = {};
	long       offsetY = 0, offsetX = 0;

	savdo();
	if (StateMap.test(StateFlag::FPSEL)) {
		stitchRect = SelectedVerticesRect;
	}
	else {
		if (SelectedFormList->size() || StateMap.test(StateFlag::BIGBOX))
			pxrct2stch(SelectedFormsRect, stitchRect);
		else {
			fvars(ClosestFormToCursor);
			px2stch();
			stitchRect.bottom = stitchRect.left = stitchRect.right = stitchRect.top = 0;
		}
	}
	switch (SelectedFormControlVertex) {
	case 0: // top control
		if (SelectedFormList->size() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.bottom;
			offsetY   = Msg.pt.y - StitchWindowOrigin.y;
			ratio = static_cast<double>(SelectedFormsRect.bottom - offsetY) / (SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.bottom;
				ratio     = static_cast<double>(SelectedPoint.y - reference) / (SelectedForm->rectangle.top - reference);
				SelectedForm->rectangle.top = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.bottom;
				ratio     = static_cast<double>(SelectedPoint.y - reference) / (StitchRangeRect.top - reference);
			}
		}
		break;
	case 1: // right control
		if (SelectedFormList->size() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.left;
			offsetX   = Msg.pt.x - StitchWindowOrigin.x;
			ratio = static_cast<double>(offsetX - SelectedFormsRect.left) / (SelectedFormsRect.right - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.left;
				ratio     = static_cast<double>(SelectedPoint.x - reference) / (SelectedForm->rectangle.right - reference);
				SelectedForm->rectangle.right = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.left;
				ratio     = static_cast<double>(SelectedPoint.x - reference) / (StitchRangeRect.right - reference);
			}
		}
		break;
	case 2: // bottom control
		if (SelectedFormList->size() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.top;
			offsetY   = Msg.pt.y - StitchWindowOrigin.y;
			ratio     = static_cast<double>(offsetY - SelectedFormsRect.top) / (SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.top;
				ratio     = static_cast<double>(SelectedPoint.y - reference) / (SelectedForm->rectangle.bottom - reference);
				SelectedForm->rectangle.bottom = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.top;
				ratio     = static_cast<double>(SelectedPoint.y - reference) / (StitchRangeRect.bottom - reference);
			}
		}
		break;
	case 3: // left control
		if (SelectedFormList->size() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.right;
			offsetX   = Msg.pt.x - StitchWindowOrigin.x;
			ratio = static_cast<double>(SelectedFormsRect.right - offsetX) / (SelectedFormsRect.right - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.right;
				ratio     = static_cast<double>(SelectedPoint.x - reference) / (SelectedForm->rectangle.left - reference);
				SelectedForm->rectangle.left = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.right;
				ratio     = static_cast<double>(SelectedPoint.x - reference) / (StitchRangeRect.left - reference);
			}
		}
		break;
	}
	if (SelectedFormControlVertex & 1) { // right or left control selected
		if (StateMap.test(StateFlag::FPSEL)) {
			fvars(ClosestFormToCursor);
			currentVertex = SelectedFormVertices.start;
			for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
				CurrentFormVertices[currentVertex].x = (CurrentFormVertices[currentVertex].x - reference) * ratio + reference;
				currentVertex                        = pdir(currentVertex);
			}
			frmout(ClosestFormToCursor);
			setpsel();
			refil();
			StateMap.set(StateFlag::RESTCH);
			return;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			for (iForm = 0; iForm < FormIndex; iForm++) {
				CurrentFormVertices = FormList[iForm].vertices;
				for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++)
					CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference) * ratio + reference;
				frmout(iForm);
			}
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
				StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference) * ratio + reference;
			selal();
			return;
		}
		else {
			if (SelectedFormList->size()) {
				for (auto selectedForm : (*SelectedFormList)) {
					CurrentFormVertices = FormList[selectedForm].vertices;
					for (iVertex = 0; iVertex < FormList[selectedForm].vertexCount; iVertex++)
						CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference) * ratio + reference;
				}
			}
			else {
				if (StateMap.test(StateFlag::FORMSEL)) {
					for (iVertex = 0; iVertex < VertexCount; iVertex++)
						CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference) * ratio + reference;
				}
				else {
					for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
						StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference) * ratio + reference;
				}
			}
		}
	}
	else {
		if (StateMap.test(StateFlag::FPSEL)) {
			fvars(ClosestFormToCursor);
			currentVertex = SelectedFormVertices.start;
			for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
				CurrentFormVertices[currentVertex].y = (CurrentFormVertices[currentVertex].y - reference) * ratio + reference;
				currentVertex                        = pdir(currentVertex);
			}
			frmout(ClosestFormToCursor);
			setpsel();
			refil();
			StateMap.set(StateFlag::RESTCH);
			return;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			for (iForm = 0; iForm < FormIndex; iForm++) {
				CurrentFormVertices = FormList[iForm].vertices;
				for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++)
					CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference) * ratio + reference;
				frmout(iForm);
			}
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
				StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference) * ratio + reference;
			selal();
			return;
		}
		else {
			if (SelectedFormList->size()) {
				for (auto selectedForm : (*SelectedFormList)) {
					CurrentFormVertices = FormList[selectedForm].vertices;
					for (iVertex = 0; iVertex < FormList[selectedForm].vertexCount; iVertex++)
						CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference) * ratio + reference;
				}
			}
			else {
				if (StateMap.test(StateFlag::FORMSEL)) {
					for (iVertex = 0; iVertex < VertexCount; iVertex++)
						CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference) * ratio + reference;
				}
				else {
					for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
						StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference) * ratio + reference;
				}
			}
		}
	}
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			frmout(selectedForm);
			ClosestFormToCursor = selectedForm;
			refil();
		}
	}
	else if (StateMap.test(StateFlag::FORMSEL))
		refil();
	StateMap.set(StateFlag::RESTCH);
}

void setexpand(double xyRatio) {
	dPOINT reference        = {};
	POINT  integerReference = {};
	fPOINT stitchReference  = {};
	dPOINT size0            = {};
	dPOINT size1            = {};
	dPOINT ratio            = {};
	double aspect           = 0.0;
	// unsigned iVertex = 0, iForm = 0, iStitch = 0;
	size_t     iCurrent  = 0;
	fRECTANGLE rectangle = {};

	savdo();
	if (SelectedFormList->size() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		rectangle.bottom = SelectedFormsRect.bottom;
		rectangle.left   = SelectedFormsRect.left;
		rectangle.right  = SelectedFormsRect.right;
		rectangle.top    = SelectedFormsRect.top;
		SelectedPoint.x  = Msg.pt.x - StitchWindowOrigin.x;
		SelectedPoint.y  = Msg.pt.y - StitchWindowOrigin.y;
		size0.y          = rectangle.bottom - rectangle.top;
	}
	else {
		px2stch();
		fvars(ClosestFormToCursor);
		if (StateMap.test(StateFlag::FORMSEL))
			rectangle = SelectedForm->rectangle;
		else {
			rectangle.bottom = StitchRangeRect.bottom;
			rectangle.top    = StitchRangeRect.top;
			rectangle.right  = StitchRangeRect.right;
			rectangle.left   = StitchRangeRect.left;
		}
		size0.y = rectangle.top - rectangle.bottom;
	}
	ratio     = { 1.0, 1.0 };
	reference = {};
	size0.x   = rectangle.right - rectangle.left;
	switch (SelectedFormControlVertex) {
	case 0:
		reference.x = rectangle.right;
		reference.y = rectangle.bottom;
		size1.x     = fabs(SelectedPoint.x - reference.x);
		size1.y     = fabs(SelectedPoint.y - reference.y);
		aspect      = size1.x / size1.y;
		if (aspect < xyRatio)
			size1.x = size1.y * xyRatio;
		else
			size1.y = size1.x / xyRatio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormList->size() && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.left = rectangle.right - size1.x;
			SelectedForm->rectangle.top  = rectangle.bottom + size1.y;
		}
		break;
	case 1:
		reference.x = rectangle.left;
		reference.y = rectangle.bottom;
		size1.x     = fabs(SelectedPoint.x - reference.x);
		size1.y     = fabs(SelectedPoint.y - reference.y);
		aspect      = size1.x / size1.y;
		if (aspect < xyRatio)
			size1.x = size1.y * xyRatio;
		else
			size1.y = size1.x / xyRatio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormList->size() && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.right = rectangle.left + size1.x;
			SelectedForm->rectangle.top   = rectangle.bottom + size1.y;
		}
		break;
	case 2:
		reference.x = rectangle.left;
		reference.y = rectangle.top;
		size1.x     = fabs(SelectedPoint.x - reference.x);
		size1.y     = fabs(SelectedPoint.y - reference.y);
		aspect      = size1.x / size1.y;
		if (aspect < xyRatio)
			size1.x = size1.y * xyRatio;
		else
			size1.y = size1.x / xyRatio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormList->size() && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.right  = rectangle.left + size1.x;
			SelectedForm->rectangle.bottom = rectangle.top - size1.y;
		}
		break;
	case 3:
		reference.x = rectangle.right;
		reference.y = rectangle.top;
		size1.x     = fabs(SelectedPoint.x - reference.x);
		size1.y     = fabs(SelectedPoint.y - reference.y);
		aspect      = size1.x / size1.y;
		if (aspect < xyRatio)
			size1.x = size1.y * xyRatio;
		else
			size1.y = size1.x / xyRatio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormList->size() && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.left   = rectangle.right - size1.x;
			SelectedForm->rectangle.bottom = rectangle.top - size1.y;
		}
		break;
	}
	integerReference.x = reference.x;
	integerReference.y = reference.y;
	px2stchf(integerReference, stitchReference);
	if (StateMap.test(StateFlag::FPSEL)) {
		fvars(ClosestFormToCursor);
		iCurrent = SelectedFormVertices.start;
		for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			CurrentFormVertices[iCurrent].x = (CurrentFormVertices[iCurrent].x - stitchReference.x) * ratio.x + stitchReference.x;
			CurrentFormVertices[iCurrent].y = (CurrentFormVertices[iCurrent].y - stitchReference.y) * ratio.y + stitchReference.y;
			iCurrent                        = pdir(iCurrent);
		}
		setpsel();
		frmout(ClosestFormToCursor);
		refil();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
			fvars(iForm);
			for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
				CurrentFormVertices[iVertex].x
				    = (CurrentFormVertices[iVertex].x - stitchReference.x) * ratio.x + stitchReference.x;
				CurrentFormVertices[iVertex].y
				    = (CurrentFormVertices[iVertex].y - stitchReference.y) * ratio.y + stitchReference.y;
			}
			frmout(iForm);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - stitchReference.x) * ratio.x + stitchReference.x;
			StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - stitchReference.y) * ratio.y + stitchReference.y;
		}
		selal();
		return;
	}
	else {
		if (SelectedFormList->size()) {
			for (auto selectedForm : (*SelectedFormList)) {
				fvars(selectedForm);
				for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
					CurrentFormVertices[iVertex].x
					    = (CurrentFormVertices[iVertex].x - stitchReference.x) * ratio.x + stitchReference.x;
					CurrentFormVertices[iVertex].y
					    = (CurrentFormVertices[iVertex].y - stitchReference.y) * ratio.y + stitchReference.y;
				}
				frmout(selectedForm);
				ClosestFormToCursor = selectedForm;
				refil();
			}
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				for (size_t iVertex = 0u; iVertex < VertexCount; iVertex++) {
					CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference.x) * ratio.x + reference.x;
					CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference.y) * ratio.y + reference.y;
				}
				refil();
			}
			else {
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference.x) * ratio.x + reference.x;
					StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference.y) * ratio.y + reference.y;
				}
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void nufilcol(unsigned color) {
	unsigned attribute = 0, iStitch = 0;

	if (SelectedForm->fillColor != color) {
		SelectedForm->fillColor = color;
		attribute               = gsl::narrow<unsigned int>((ClosestFormToCursor << 4) | FRMFIL);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (FRMSK | TYPMSK | FTHMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void nufthcol(unsigned color) {
	unsigned attribute = 0, iStitch = 0;

	if (SelectedForm->fillInfo.feather.color != color) {
		SelectedForm->fillInfo.feather.color = color;
		attribute                            = gsl::narrow<unsigned int>((ClosestFormToCursor << 4) | FTHMSK);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (FRMSK | FTHMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void nubrdcol(unsigned color) {
	unsigned attribute = 0, iStitch = 0;

	SelectedForm->borderColor = color;
	attribute                 = gsl::narrow<unsigned int>((ClosestFormToCursor << 4) | FRMBFIL);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & (FRMSK | TYPMSK)) == attribute) {
			StitchBuffer[iStitch].attribute &= 0xfffffff0;
			StitchBuffer[iStitch].attribute |= color;
		}
	}
}

void nulapcol(unsigned color) {
	unsigned attribute = 0, iStitch = 0;

	if (gsl::narrow<unsigned>(SelectedForm->borderColor >> 4) != color) {
		SelectedForm->borderColor &= COLMSK;
		SelectedForm->borderColor |= color << 4;
		attribute = gsl::narrow<unsigned int>((ClosestFormToCursor << 4) | TYPMSK);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (TYPMSK | FRMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void sbold() {
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType    = EDGEBEAN;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void dubold() {
	if (filmsgs(FML_BLD))
		return;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			sbold();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			sbold();
			coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
			ritot(PCSHeader.stitchCount);
		}
	}
}

void sethup() noexcept {
	if (IniFile.hoopSizeX == LHUPX && IniFile.hoopSizeY == LHUPY) {
		IniFile.hoopType = LARGHUP;
		return;
	}
	if (IniFile.hoopSizeX == SHUPX && IniFile.hoopSizeY == SHUPY) {
		IniFile.hoopType = SMALHUP;
		return;
	}
	if (IniFile.hoopSizeX == HUP100XY && IniFile.hoopSizeY == HUP100XY) {
		IniFile.hoopType = HUP100;
		return;
	}
	IniFile.hoopType = CUSTHUP;
}

void dustar(unsigned starCount, double length) {
	double   stepAngle = 0.0;
	double   angle     = 0.0;
	unsigned iVertex = 0, vertexCount = 0;
	dPOINT   point = {}, center = {};

	if (starCount < 3)
		starCount = 3;
	if (starCount > 100)
		starCount = 100;
	stepAngle           = PI / starCount;
	angle               = stepAngle / 2 + PI;
	vertexCount         = starCount << 1;
	SelectedForm        = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices    = adflt(vertexCount);
	SelectedForm->vertexCount = vertexCount;
	SelectedForm->attribute   = (ActiveLayer << 1);
	fvars(FormIndex);
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	StateMap.set(StateFlag::FILDIR);
	for (iVertex = 0; iVertex < vertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x = point.x;
		CurrentFormVertices[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	center.x = (CurrentFormVertices[starCount].x - CurrentFormVertices[0].x) / 2 + CurrentFormVertices[0].x;
	center.y = (CurrentFormVertices[starCount].y - CurrentFormVertices[0].y) / 2 + CurrentFormVertices[0].y;
	for (iVertex = 1; iVertex < vertexCount; iVertex += 2) {
		CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - center.x) * StarRatio + center.x;
		CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - center.y) * StarRatio + center.y;
	}
	SelectedForm->type = FRMFPOLY;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount                = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void duspir(unsigned stepCount) {
	double   stepAngle = 0.0;
	double   angle     = 0.0;
	double   length = 0.0, stepRatio = 0.0, ratio = 0.0;
	unsigned iStep = 0, iVertex = 0, vertexCount = 0;
	dPOINT   point  = {};
	dPOINT   center = {};

	if (stepCount < 3)
		stepCount = 3;
	if (stepCount > 100)
		stepCount = 100;
	stepAngle = PI * 2 / stepCount;
	// ToDo - Why 800?
	length              = 800 / stepCount * ZoomFactor * (UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm        = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	vertexCount            = stepCount * SpiralWrap;
	SelectedForm->vertices = adflt(vertexCount);
	std::vector<fPOINT> firstSpiral(stepCount);
	std::vector<fPOINT> centeredSpiral(stepCount);
	SelectedForm->vertexCount = vertexCount;
	SelectedForm->attribute   = (ActiveLayer << 1);
	fvars(FormIndex);
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	for (iStep = 0; iStep < stepCount; iStep++) {
		firstSpiral[iStep].x = point.x;
		firstSpiral[iStep].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	center.x = (firstSpiral[stepCount >> 1].x - firstSpiral[0].x) / 2 + firstSpiral[0].x;
	center.y = (firstSpiral[stepCount >> 1].y - firstSpiral[0].y) / 2 + firstSpiral[0].y;
	for (iStep = 0; iStep < stepCount; iStep++) {
		centeredSpiral[iStep].x = firstSpiral[iStep].x - center.x;
		centeredSpiral[iStep].y = firstSpiral[iStep].y - center.y;
	}
	stepRatio = 1.0 / vertexCount;
	ratio     = stepRatio;
	for (iVertex = 0; iVertex < vertexCount; iVertex++) {
		SelectedForm->vertices[iVertex].x = centeredSpiral[iVertex % stepCount].x * ratio + center.x;
		SelectedForm->vertices[iVertex].y = centeredSpiral[iVertex % stepCount].y * ratio + center.y;
		ratio += stepRatio;
	}
	SelectedForm->type = FRMLINE;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount                = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void duhart(unsigned sideCount) {
	double   stepAngle = 0.0;
	double   angle     = 0.0;
	double   length    = 0.0;
	unsigned iVertex = 0, lastVertex = 0, firstVertex = 0, iDestination = 0;
	float    maximumX = 0.0;
	dPOINT   point    = {};
	double   ratio    = 0.0;

	if (sideCount > 100)
		sideCount = 100;
	if (sideCount < 6)
		sideCount = 6;
	SelectedForm = &FormList[FormIndex];
	frmclr(SelectedForm);
	SelectedForm->attribute = ActiveLayer << 1;
	CurrentFormVertices     = &FormVertices[FormVertexIndex];
	px2stch();
	point.x   = SelectedPoint.x;
	point.y   = SelectedPoint.y;
	stepAngle = PI * 2 / sideCount;
	length    = 300 / sideCount * ZoomFactor * (UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	angle     = PI * 0.28;
	iVertex   = 0;
	maximumX  = 0;
	while (angle > -PI * 0.7) {
		if (point.x > maximumX)
			maximumX = point.x;
		CurrentFormVertices[iVertex].x   = point.x;
		CurrentFormVertices[iVertex++].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	stepAngle /= 4.5;
	lastVertex = iVertex;
	while (point.x > CurrentFormVertices[0].x && iVertex < 200) {
		CurrentFormVertices[iVertex].x   = point.x;
		CurrentFormVertices[iVertex++].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	firstVertex = iVertex;
	ratio       = (CurrentFormVertices[lastVertex - 1].x - CurrentFormVertices[0].x)
	        / (CurrentFormVertices[lastVertex - 1].x - CurrentFormVertices[firstVertex - 1].x);
	for (iVertex = lastVertex; iVertex < firstVertex; iVertex++)
		CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - CurrentFormVertices[lastVertex - 1].x) * ratio
		                                 + CurrentFormVertices[lastVertex - 1].x;
	lastVertex = iDestination = iVertex;
	for (iVertex = lastVertex - 2; iVertex != 0; iVertex--) {
		CurrentFormVertices[iDestination].y = CurrentFormVertices[iVertex].y;
		CurrentFormVertices[iDestination].x
		    = maximumX + maximumX - CurrentFormVertices[iVertex].x - 2 * (maximumX - CurrentFormVertices[0].x);
		iDestination++;
	}
	NewFormVertexCount        = iDestination + 1;
	SelectedForm->vertices    = adflt(iDestination);
	SelectedForm->vertexCount = iDestination;
	SelectedForm->type        = FRMFPOLY;
	ClosestFormToCursor       = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void dulens(unsigned sides) {
	double   stepAngle = 0.0;
	double   angle     = 0.0;
	double   length    = 0.0;
	unsigned iVertex = 0, iDestination = 0, lastVertex = 0, count = 0;
	float    av    = 0.0;
	dPOINT   point = {};

	// ToDo - this does not produce a consistent size of lens
	//        or the correct number of sides
	if (sides < 6)
		sides = 6;
	if (sides > 48)
		sides = 48;
	sides <<= 1;
	stepAngle           = PI * 2 / sides;
	count               = sides / 2 * 0.3;
	angle               = count * stepAngle;
	length              = 500 / sides * ZoomFactor * (UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm        = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices  = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	point   = SelectedPoint;
	iVertex = 0;
	SelectedPoint.x -= 0.0001f;
	while (point.x >= SelectedPoint.x) {
		CurrentFormVertices[iVertex++] = point;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	lastVertex   = iVertex - 1;
	iDestination = iVertex;
	av           = CurrentFormVertices[0].x;
	for (iVertex = lastVertex; iVertex != 0; iVertex--) {
		CurrentFormVertices[iDestination].y = CurrentFormVertices[iVertex - 1].y;
		CurrentFormVertices[iDestination].x = av + av - CurrentFormVertices[iVertex - 1].x;
		iDestination++;
	}
	NewFormVertexCount        = iDestination;
	SelectedForm->vertices    = adflt(iDestination - 1);
	SelectedForm->vertexCount = iDestination - 1;
	SelectedForm->type        = FRMFPOLY;
	ClosestFormToCursor       = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

constexpr float shreg(float highValue, float reference) {
	return (highValue - reference) * EggRatio + reference;
}

void dueg(unsigned sides) {
	fvars(FormIndex);

	if (sides < 8)
		sides = 8;
	durpoli(sides);

	const double reference = midl(CurrentFormVertices[sides / 2].y, CurrentFormVertices[0].y);
	const double maximumY  = CurrentFormVertices[sides >> 2].y - CurrentFormVertices[0].y;

	for (unsigned iVertex = 0; iVertex < sides; iVertex++) {
		if (CurrentFormVertices[iVertex].y < reference)
			CurrentFormVertices[iVertex].y = reference - (reference - CurrentFormVertices[iVertex].y) * IniFile.eggRatio;
	}
	EggRatio = maximumY / (CurrentFormVertices[sides >> 2].y - CurrentFormVertices[0].y);
	for (unsigned iVertex = 1; iVertex < VertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x = shreg(CurrentFormVertices[iVertex].x, CurrentFormVertices[0].x);
		CurrentFormVertices[iVertex].y = shreg(CurrentFormVertices[iVertex].y, CurrentFormVertices[0].y);
	}
}

void duzig(unsigned vertices) {
	fPOINT   offset  = {};
	unsigned iVertex = 0;

	if (vertices < 3)
		vertices = 3;
	if (vertices > 100)
		vertices = 100;
	SelectedForm        = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices    = adflt(vertices);
	SelectedForm->vertexCount = vertices;
	SelectedForm->attribute   = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	offset.x = UnzoomedRect.x / 6;
	offset.y = UnzoomedRect.y / (6 * vertices);
	for (iVertex = 0; iVertex < vertices; iVertex++) {
		CurrentFormVertices[iVertex] = SelectedPoint;
		SelectedPoint.y -= offset.y;
		if (iVertex & 1)
			SelectedPoint.x += offset.x;
		else
			SelectedPoint.x -= offset.x;
	}
	SelectedForm->type  = FRMLINE;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount                = vertices + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void fliph() {
	unsigned   iForm = 0, iVertex = 0, iStitch = 0, decodedForm = 0;
	size_t     currentVertex = 0;
	float      midpoint      = 0;
	fRECTANGLE rectangle     = {};

	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		savdo();
		midpoint      = (SelectedVerticesRect.right - SelectedVerticesRect.left) * 0.5 + SelectedVerticesRect.left;
		currentVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			CurrentFormVertices[currentVertex].x = midpoint + midpoint - CurrentFormVertices[currentVertex].x;
			currentVertex                        = pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		midpoint = (AllItemsRect.right - AllItemsRect.left) / 2 + AllItemsRect.left;
		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
			FormVertices[iVertex].x = midpoint + midpoint - FormVertices[iVertex].x;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			FormList[iForm].rectangle.left  = midpoint + midpoint - FormList[iForm].rectangle.left;
			FormList[iForm].rectangle.right = midpoint + midpoint - FormList[iForm].rectangle.right;
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedFormList->size()) {
		savdo();
		boost::dynamic_bitset<> formMap(FormIndex);
		pxrct2stch(SelectedFormsRect, rectangle);
		midpoint = (rectangle.right - rectangle.left) / 2 + rectangle.left;
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			formMap.set(ClosestFormToCursor);
			fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++)
				CurrentFormVertices[iVertex].x = midpoint + midpoint - CurrentFormVertices[iVertex].x;
			frmout(ClosestFormToCursor);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			decodedForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && !(StitchBuffer[iStitch].attribute & NOTFRM))
				StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			midpoint = (SelectedForm->rectangle.right - SelectedForm->rectangle.left) / 2 + SelectedForm->rectangle.left;
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
				CurrentFormVertices[iVertex].x = midpoint + midpoint - CurrentFormVertices[iVertex].x;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor
				    && !(StitchBuffer[iStitch].attribute & NOTFRM))
					StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
			}
			frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				selRct(rectangle);
				midpoint = (rectangle.right - rectangle.left) / 2 + rectangle.left;
				for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
					StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void prpsbrd() {
	if (SelectedForm->vertexCount > 2) {
		deleclp(ClosestFormToCursor);
		SelectedForm->edgeType = EDGEPROPSAT;
		if (UserFlagMap.test(UserFlag::DUND))
			SelectedForm->edgeType |= EGUND;
		bsizpar();
		SelectedForm->borderSize  = BorderWidth;
		SelectedForm->edgeSpacing = LineSpacing;
		SelectedForm->borderColor = ActiveColor;
		refilfn();
	}
}

void prpbrd(double borderStitchSpacing) {
	const double savedSpacing = LineSpacing;

	if (filmsgs(FML_PRPS))
		return;
	LineSpacing = borderStitchSpacing;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			SelectedForm->borderSize = LineSpacing;
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			prpsbrd();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			prpsbrd();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
	LineSpacing = savedSpacing;
}

void tglfrm() {
	if (StateMap.testAndReset(StateFlag::SATPNT))
		satfix();
	StateMap.reset(StateFlag::HIDSTCH);
	if (StateMap.testAndFlip(StateFlag::FRMOF)) {
		FormOnOff->assign((*StringTable)[STR_FRMPLUS]);
	}
	else {
		FormOnOff->assign((*StringTable)[STR_FRMINUS]);
		StateMap.reset(StateFlag::FORMSEL);
		StateMap.reset(StateFlag::FORMIN);
		StateMap.reset(StateFlag::MOVFRM);
		StateMap.reset(StateFlag::FRMPMOV);
		StateMap.reset(StateFlag::FRMOV);
		StateMap.reset(StateFlag::SATIN);
		StateMap.reset(StateFlag::SATPNT);
		StateMap.reset(StateFlag::SATCNKT);
		StateMap.reset(StateFlag::FENDIN);
		StateMap.reset(StateFlag::DELFRM);
		StateMap.reset(StateFlag::FRMPSEL);
		StateMap.reset(StateFlag::INSFRM);
	}
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
	StateMap.set(StateFlag::DUMEN);
	StateMap.set(StateFlag::RESTCH);
}

void frmon() {
	unbsho();
	StateMap.reset(StateFlag::FRMOF);
	FormOnOff->assign((*StringTable)[STR_FRMPLUS]);
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
	StateMap.set(StateFlag::DUMEN);
}

void fnord() {
	size_t iVertex = 0;

	fvars(ClosestFormToCursor);
	SelectedForm = &FormList[ClosestFormToCursor];
	for (iVertex = 0; iVertex<SelectedForm->vertexCount>> 1; iVertex++) {
		std::swap(SelectedForm->vertices[iVertex], SelectedForm->vertices[SelectedForm->vertexCount - iVertex - 1]);
	}
	refil();
}

void flpord() {
	int      iStitch = 0;
	unsigned iVertex = 0, iForward = 0;
	size_t   start = 0, finish = 0;

	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		savdo();
		start  = SelectedFormVertices.start;
		finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % VertexCount;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount >> 1; iVertex++) {
			std::swap(SelectedForm->vertices[start], SelectedForm->vertices[finish]);
			start = pdir(start);
			StateMap.flip(StateFlag::PSELDIR);
			finish = pdir(finish);
			StateMap.flip(StateFlag::PSELDIR);
		}
		refil();
		return;
	}
	if (SelectedFormList->size()) {
		savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fnord();
		}
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			fnord();
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				iForward = GroupStartStitch;
				for (iStitch = 0; iStitch < (gsl::narrow<signed>(GroupEndStitch - GroupStartStitch) >> 1) + 1; iStitch++) {
					std::swap(StitchBuffer[iForward], StitchBuffer[GroupEndStitch - iStitch]);
					iForward++;
				}
				coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void filsclp() {
	unsigned iClip = 0;

	deltx();
	fvars(ClosestFormToCursor);
	if (SelectedForm->type != SAT)
		SelectedForm->wordParam = 0;
	SelectedForm->type                    = SAT;
	SelectedForm->fillType                = CLPF;
	SelectedForm->angleOrClipData.clip    = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	for (iClip = 0; iClip < ClipStitchCount; iClip++) {
		SelectedForm->angleOrClipData.clip[iClip] = ClipBuffer[iClip];
	}
	refilfn();
}

void clpfil() {
	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN && ClipRectSize.cy > CLPMIN) {
				if (SelectedFormList->size()) {
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							filsclp();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						filsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void snpfn(const std::vector<unsigned>& xPoints, unsigned start, unsigned end, unsigned finish) {
	unsigned iPoint = 0, reference = 0, check = 0;
	double   CheckLength = 0.0;

	if (finish - start) {
		for (unsigned current = start; current < end; current++) {
			reference = xPoints[current];
			for (iPoint = current + 1; iPoint < finish; iPoint++) {
				check = xPoints[iPoint];
				CheckLength
				    = hypot(StitchBuffer[check].x - StitchBuffer[reference].x, StitchBuffer[check].y - StitchBuffer[reference].y);
				if (CheckLength < SnapLength) {
					StitchBuffer[check] = StitchBuffer[reference];
				}
			}
		}
	}
}

void nutim(double size) noexcept {
	TimeWindow     = CreateWindow(L"STATIC",
                              0,
                              WS_CHILD | WS_VISIBLE | WS_BORDER,
                              ButtonWidthX3,
                              0,
                              StitchWindowSize.x,
                              ButtonHeight,
                              ThrEdWindow,
                              NULL,
                              ThrEdInstance,
                              NULL);
	TimeDC         = GetDC(TimeWindow);
	TimeStep       = static_cast<double>(StitchWindowSize.x) / size;
	TimePosition   = 0;
	FormLines[0].y = 0;
	FormLines[1].y = ButtonHeight;
	FormLines[0].x = FormLines[1].x = 0;
	SelectObject(TimeDC, UserPen[0]);
}

void nxtim() noexcept {
	Polyline(TimeDC, FormLines, 2);
	TimePosition += TimeStep;
	FormLines[0].x = FormLines[1].x = TimePosition;
}

void snp(unsigned start, unsigned finish) {
	unsigned iColumn = 0, iStitch = 0, value = 0, accumulator = 0, checkLength = 0, attribute = 0;
	fPOINT   range = {};

	chkrng(range);
	std::vector<unsigned> xPoints(PCSHeader.stitchCount);
	std::vector<unsigned> xHistogram((round(range.x)) + 1);
	if (StateMap.test(StateFlag::FORMSEL)) {
		attribute = (ClosestFormToCursor << 4) & FRMSK;
		for (iStitch = start; iStitch < finish; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				iColumn = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
				xHistogram[iColumn]++;
			}
		}
	}
	else {
		for (iStitch = start; iStitch < finish; iStitch++) {
			iColumn = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
			xHistogram[iColumn]++;
		}
	}
	accumulator = 0;
	for (iColumn = 0; iColumn < range.x; iColumn++) {
		value               = xHistogram[iColumn];
		xHistogram[iColumn] = accumulator;
		accumulator += value;
	}
	xHistogram[iColumn] = accumulator;
	if (StateMap.test(StateFlag::FORMSEL)) {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				iColumn                        = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
				xPoints[xHistogram[iColumn]++] = iStitch;
			}
		}
	}
	else {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			iColumn                        = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
			xPoints[xHistogram[iColumn]++] = iStitch;
		}
	}
	checkLength = SnapLength * 2 + 1;
	nutim(range.x);
	for (iColumn = 1; iColumn < range.x - checkLength - 1; iColumn++) {
		snpfn(xPoints, xHistogram[iColumn], xHistogram[iColumn + 1], xHistogram[iColumn + checkLength]);
		nxtim();
	}
	DestroyWindow(TimeWindow);
}

void snap() {
	savdo();
	if (StateMap.test(StateFlag::GRPSEL)) {
		rngadj();
		snp(GroupStartStitch, GroupEndStitch);
	}
	else
		snp(0, PCSHeader.stitchCount);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void dufcntr(dPOINT& center) {
	fRECTANGLE* formRect = nullptr;
	fRECTANGLE  bigRect  = {};

	formRect       = &FormList[SelectedFormList->front()].rectangle;
	bigRect.left   = formRect->left;
	bigRect.right  = formRect->right;
	bigRect.top    = formRect->top;
	bigRect.bottom = formRect->bottom;
	for (auto selectedForm : (*SelectedFormList)) {
		formRect = &FormList[selectedForm].rectangle;
		if (formRect->left < bigRect.left)
			bigRect.left = formRect->left;
		if (formRect->right > bigRect.right)
			bigRect.right = formRect->right;
		if (formRect->bottom < bigRect.bottom)
			bigRect.bottom = formRect->bottom;
		if (formRect->top > bigRect.top)
			bigRect.top = formRect->top;
	}
	center.x = (bigRect.right - bigRect.left) / 2 + bigRect.left;
	center.y = (bigRect.top - bigRect.bottom) / 2 + bigRect.bottom;
}

void rotpar(dPOINT& rotationCenter) {
	if (IniFile.rotationAngle < (PI / 180))
		tabmsg(IDS_ROTIN);
	if (StateMap.test(StateFlag::FPSEL)) {
		rotationCenter.x = midl(SelectedVerticesRect.right, SelectedVerticesRect.left);
		rotationCenter.y = midl(SelectedVerticesRect.top, SelectedVerticesRect.bottom);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		rotationCenter.x = midl(AllItemsRect.right, AllItemsRect.left);
		rotationCenter.y = midl(AllItemsRect.top, AllItemsRect.bottom);
		return;
	}
	if (SelectedFormList->size()) {
		if (StateMap.test(StateFlag::GMRK)) {
			rotationCenter.x = ZoomMarkPoint.x;
			rotationCenter.y = ZoomMarkPoint.y;
		}
		else
			dufcntr(rotationCenter);
		StateMap.set(StateFlag::FRMSROT);
		return;
	}
	if (StateMap.test(StateFlag::FORMSEL)) {
		RotationRect.left   = SelectedForm->rectangle.left;
		RotationRect.right  = SelectedForm->rectangle.right;
		RotationRect.top    = SelectedForm->rectangle.top;
		RotationRect.bottom = SelectedForm->rectangle.bottom;
		if (!StateMap.test(StateFlag::GMRK)) {
			rotationCenter.x = static_cast<double>(RotationRect.right - RotationRect.left) / 2 + RotationRect.left;
			rotationCenter.y = static_cast<double>(RotationRect.top - RotationRect.bottom) / 2 + RotationRect.bottom;
		}
		StateMap.set(StateFlag::FRMROT);
	}
	else {
		if (StateMap.test(StateFlag::GRPSEL)) {
			rngadj();
			selRct(RotationRect);
		}
	}
	if (StateMap.test(StateFlag::GMRK)) {
		rotationCenter.x = ZoomMarkPoint.x;
		rotationCenter.y = ZoomMarkPoint.y;
	}
	else {
		rotationCenter.x = static_cast<double>(RotationRect.right - RotationRect.left) / 2 + RotationRect.left;
		rotationCenter.y = static_cast<double>(RotationRect.top - RotationRect.bottom) / 2 + RotationRect.bottom;
	}
}

void rotentr(double rotationAngle) {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_ROTA);
	shoMsg(fmt::format(fmtStr, (rotationAngle / PI * 180)));
	StateMap.set(StateFlag::NUMIN);
	numWnd();
}

void setrang() {
	rotentr(IniFile.rotationAngle);
	StateMap.set(StateFlag::NUROT);
}

void rotcmd(dPOINT& rotationCenter) {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || SelectedFormList->size()
	    || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		rotpar(rotationCenter);
		rotentr(IniFile.rotationAngle);
		StateMap.set(StateFlag::ENTROT);
	}
	else
		shoseln(IDS_ALLX, IDS_ROTCMD);
}

void fnagain(double rotationAngle) {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || SelectedFormList->size()
	    || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		dPOINT rotationCenter;
		rotpar(rotationCenter);
		rotfn(rotationAngle, rotationCenter);
	}
	else
		alrotmsg();
}

void rotagain() {
	fnagain(IniFile.rotationAngle);
}

void bakagain() {
	fnagain(2 * PI - IniFile.rotationAngle);
}

void rotdup() {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || SelectedFormList->size()) {
		dPOINT rotationCenter;
		rotpar(rotationCenter);
		rotentr(IniFile.rotationAngle);
		StateMap.set(StateFlag::ENTRDUP);
	}
	else
		shord();
}

void adfrm(size_t iForm) {
	FRMHED* formHeader = &FormList[FormIndex];

	SelectedForm         = &FormList[iForm];
	*formHeader          = *SelectedForm;
	ClosestFormToCursor  = FormIndex;
	formHeader->vertices = adflt(SelectedForm->vertexCount);
	std::copy(SelectedForm->vertices,
	          SelectedForm->vertices + SelectedForm->vertexCount,
	          stdext::make_checked_array_iterator(formHeader->vertices, SelectedForm->vertexCount));
	if (formHeader->type == SAT && formHeader->satinGuideCount) {
		formHeader->satinOrAngle.guide = adsatk(formHeader->satinGuideCount);
		std::copy(SelectedForm->satinOrAngle.guide,
		          SelectedForm->satinOrAngle.guide + SelectedForm->satinGuideCount,
		          stdext::make_checked_array_iterator(formHeader->satinOrAngle.guide, SelectedForm->satinGuideCount));
	}
	if (iseclpx(FormIndex)) {
		formHeader->borderClipData = adclp(formHeader->clipEntries);
		std::copy(SelectedForm->borderClipData,
		          SelectedForm->borderClipData + SelectedForm->clipEntries,
		          stdext::make_checked_array_iterator(formHeader->borderClipData, formHeader->clipEntries));
	}
	if (isclpx(FormIndex)) {
		formHeader->angleOrClipData.clip = adclp(formHeader->lengthOrCount.clipCount);
		std::copy(SelectedForm->angleOrClipData.clip,
		          SelectedForm->angleOrClipData.clip + SelectedForm->lengthOrCount.clipCount,
		          stdext::make_checked_array_iterator(formHeader->angleOrClipData.clip, formHeader->lengthOrCount.clipCount));
	}
	FormIndex++;
}

void duprot(double rotationAngle, const dPOINT& rotationCenter) {
	adfrm(ClosestFormToCursor);
	rotfn(rotationAngle, rotationCenter);
	refil();
	StateMap.set(StateFlag::FORMSEL);
	StateMap.set(StateFlag::RESTCH);
}

void duprotfs(double rotationAngle) {
	const auto savedFormIndex = FormIndex;
	dPOINT     rotationCenter = {};

	rotpar(rotationCenter);
	for (auto selectedForm : (*SelectedFormList)) {
		adfrm(selectedForm);
	}
	FormIndex = savedFormIndex;
	for (auto& selectedForm : (*SelectedFormList)) {
		selectedForm = FormIndex++;
	}
	fnagain(rotationAngle);
}

void duprots(double rotationAngle, const dPOINT& rotationCenter) {
	unsigned source = 0, destination = PCSHeader.stitchCount;

	rngadj();

	for (source = GroupStartStitch; source <= GroupEndStitch; source++) {
		StitchBuffer[destination].x           = StitchBuffer[source].x;
		StitchBuffer[destination].y           = StitchBuffer[source].y;
		StitchBuffer[destination++].attribute = StitchBuffer[source].attribute & (~(FRMSK | TYPMSK));
	}
	ClosestPointIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount += (GroupEndStitch - GroupStartStitch);
	GroupStitchIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount++;
	rngadj();
	rotfn(rotationAngle, rotationCenter);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void cplayfn(size_t iForm, unsigned play) {
	const FRMHED* formHeader = &FormList[iForm];

	fvars(FormIndex);
	*SelectedForm = *formHeader;
	fvars(FormIndex);
	SelectedForm->vertices = adflt(SelectedForm->vertexCount);
	std::copy(formHeader->vertices,
	          formHeader->vertices + VertexCount,
	          stdext::make_checked_array_iterator(SelectedForm->vertices, MAXITEMS - FormVertexIndex));
	if (SelectedForm->type == SAT && SelectedForm->satinGuideCount) {
		SelectedForm->satinOrAngle.guide = adsatk(SelectedForm->satinGuideCount);
		auto       sourceStart           = formHeader->satinOrAngle.guide;
		auto       sourceEnd             = sourceStart + SelectedForm->satinGuideCount;
		const auto destination = stdext::make_checked_array_iterator(SelectedForm->satinOrAngle.guide, 10000 - SatinGuideIndex);
		std::copy(sourceStart, sourceEnd, destination);
	}
	SelectedForm->clipEntries             = 0;
	SelectedForm->fillType                = 0;
	SelectedForm->lengthOrCount.clipCount = 0;
	;
	SelectedForm->edgeType               = 0;
	SelectedForm->fillInfo.texture.index = 0;
	SelectedForm->attribute              = FormList[FormIndex].attribute & NFRMLMSK;
	SelectedForm->attribute |= play;
	SelectedForm->extendedAttribute = 0;
	dusqr();
	FormIndex++;
}

void cpylayr(unsigned codedLayer) {
	unsigned       iStitch = 0, iCurrentStitch = 0;
	const unsigned codedStitchLayer = codedLayer << (LAYSHFT - 1);

	if (SelectedFormList->size()) {
		savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			cplayfn(selectedForm, codedLayer);
		}
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			cplayfn(ClosestFormToCursor, codedLayer);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				iCurrentStitch = PCSHeader.stitchCount;
				for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
					StitchBuffer[iCurrentStitch].x           = StitchBuffer[iStitch].x;
					StitchBuffer[iCurrentStitch].y           = StitchBuffer[iStitch].y;
					StitchBuffer[iCurrentStitch++].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
				}
				PCSHeader.stitchCount = iCurrentStitch;
				coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void movlayr(unsigned codedLayer) {
	unsigned       iStitch          = 0;
	const unsigned codedStitchLayer = codedLayer << (LAYSHFT - 1);

	if (SelectedFormList->size()) {
		savdo();
		boost::dynamic_bitset<> formMap(FormIndex);
		for (auto selectedForm : (*SelectedFormList)) {
			FormList[selectedForm].attribute = (FormList[selectedForm].attribute & NFRMLMSK) | codedLayer;
			formMap.set(selectedForm);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute & ALTYPMSK) {
				const auto iCurrentForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
				if (formMap.test(iCurrentForm))
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
			}
		}
		SelectedFormList->clear();
		StateMap.reset(StateFlag::FORMSEL);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			FormList[ClosestFormToCursor].attribute = (FormList[ClosestFormToCursor].attribute & NFRMLMSK) | codedLayer;
			StateMap.reset(StateFlag::FORMSEL);
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (StitchBuffer[iStitch].attribute & ALTYPMSK
				    && ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor)
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
			}
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++)
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void join() {
	const auto savedFormIndex = ClosestFormToCursor;
	size_t     vertexCount    = 0;

	StateMap.set(StateFlag::FRMSAM);
	if (FormIndex > 1 && StateMap.test(StateFlag::FORMSEL) && closfrm()) {
		vertexCount = FormList[ClosestFormToCursor].vertexCount;
		std::vector<fPOINT> vertexList(vertexCount);
		for (size_t iVertex = 0u; iVertex < vertexCount; iVertex++) {
			vertexList[iVertex]   = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor];
			ClosestVertexToCursor = nxt(ClosestVertexToCursor);
		}
		StateMap.set(StateFlag::DELTO);
		frmdel();
		if (savedFormIndex > ClosestFormToCursor)
			ClosestFormToCursor = savedFormIndex - 1;
		else
			ClosestFormToCursor = savedFormIndex;
		fPOINT* insertedVertex = &FormList[ClosestFormToCursor].vertices[FormList[ClosestFormToCursor].vertexCount];
		if (insertedVertex) {
			fltspac(insertedVertex, vertexCount);
			for (size_t iVertex = 0u; iVertex < vertexCount; iVertex++) {
				insertedVertex[iVertex] = vertexList[iVertex];
			}
		}
		SelectedForm = &FormList[ClosestFormToCursor];
		SelectedForm->vertexCount += vertexCount;
		SelectedForm->rectangle.left = SelectedForm->rectangle.right = SelectedForm->vertices[0].x;
		SelectedForm->rectangle.top = SelectedForm->rectangle.bottom = SelectedForm->vertices[0].y;
		for (size_t iVertex = 1u; iVertex < SelectedForm->vertexCount; iVertex++) {
			if (SelectedForm->vertices[iVertex].x < SelectedForm->rectangle.left)
				SelectedForm->rectangle.left = SelectedForm->vertices[iVertex].x;
			if (SelectedForm->vertices[iVertex].x > SelectedForm->rectangle.right)
				SelectedForm->rectangle.right = SelectedForm->vertices[iVertex].x;
			if (SelectedForm->vertices[iVertex].y > SelectedForm->rectangle.top)
				SelectedForm->rectangle.top = SelectedForm->vertices[iVertex].y;
			if (SelectedForm->vertices[iVertex].y < SelectedForm->rectangle.bottom)
				SelectedForm->rectangle.bottom = SelectedForm->vertices[iVertex].y;
		}
		refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	StateMap.reset(StateFlag::FRMSAM);
}

void refilal() {
	const auto savedFormIndex = ClosestFormToCursor;

	savdo();
	for (ClosestFormToCursor = 0; ClosestFormToCursor < FormIndex; ClosestFormToCursor++)
		refilfn();
	ClosestFormToCursor = savedFormIndex;
	fvars(ClosestFormToCursor);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

bool notsel() noexcept {
	for (auto selectedForm : (*SelectedFormList)) {
		if (selectedForm == ClosestFormToCursor)
			return 0;
	}
	return 1;
}

void nufsel() {
	if (FormIndex) {
		StateMap.reset(StateFlag::FORMSEL);
		if (StateMap.testAndReset(StateFlag::WASEL)) {
			SelectedFormList->push_back(PreviousFormIndex);
		}
		if (notsel()) {
			SelectedFormList->push_back(ClosestFormToCursor);
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void frmadj(size_t formIndex) noexcept {
	unsigned iVertex = 0;

	fvars(formIndex);
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x += FormMoveDelta.x;
		CurrentFormVertices[iVertex].y -= FormMoveDelta.y;
	}
	frmout(formIndex);
}

void frmsadj() {
	unsigned iStitch = 0;

	boost::dynamic_bitset<> formMap(FormIndex);
	for (auto selectedForm : (*SelectedFormList)) {
		formMap.set(selectedForm);
	}
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (StitchBuffer[iStitch].attribute & ALTYPMSK && formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
			StitchBuffer[iStitch].x += FormMoveDelta.x;
			StitchBuffer[iStitch].y -= FormMoveDelta.y;
		}
	}
}

void frmpnts(unsigned type) {
	unsigned   iStitch = 0;
	const auto trg     = gsl::narrow<unsigned int>((ClosestFormToCursor << 4) | type);

	while (iStitch < PCSHeader.stitchCount && (StitchBuffer[iStitch].attribute & (ALTYPMSK | FRMSK)) != trg)
		iStitch++;
	ClosestPointIndex = iStitch;
	if (PCSHeader.stitchCount > 0) {
		iStitch = PCSHeader.stitchCount - 1;
	}
	else {
		iStitch = 0;
	}
	while (iStitch > ClosestPointIndex && (StitchBuffer[iStitch].attribute & (ALTYPMSK | FRMSK)) != trg)
		iStitch--;
	GroupStitchIndex = iStitch;
}

void selfil(unsigned type) {
	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		if (type == FRMFIL && !FormList[ClosestFormToCursor].fillType) {
			tabmsg(IDS_FSELF);
			return;
		}
		if (type == FRMBFIL && !FormList[ClosestFormToCursor].edgeType) {
			tabmsg(IDS_FSELB);
			return;
		}
		if (type == FRMAPFIL && (FormList[ClosestFormToCursor].edgeType & NEGUND) != EDGEAPPL) {
			tabmsg(IDS_FSELA);
			return;
		}
		frmpnts(type);
		StateMap.set(StateFlag::GRPSEL);
		StateMap.reset(StateFlag::FORMSEL);
		rngadj();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		tabmsg(IDS_SEL1FRM);
}

bool notfstch(unsigned attribute) noexcept {
	if (attribute & NOTFRM)
		return 1;
	if (((attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor)
		return 0;
	return 1;
}

void selalfil() {
	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		ClosestPointIndex = 0;
		while (ClosestPointIndex < PCSHeader.stitchCount && notfstch(StitchBuffer[ClosestPointIndex].attribute))
			ClosestPointIndex++;
		if (ClosestPointIndex != PCSHeader.stitchCount) {
			if (ClosestPointIndex)
				ClosestPointIndex--;
			GroupStitchIndex = PCSHeader.stitchCount - 1;
			while (GroupStitchIndex > ClosestPointIndex && notfstch(StitchBuffer[GroupStitchIndex].attribute))
				GroupStitchIndex--;
			StateMap.set(StateFlag::GRPSEL);
			StateMap.reset(StateFlag::FORMSEL);
			rngadj();
			StateMap.set(StateFlag::RESTCH);
		}
		else
			tabmsg(IDS_FSELM);
	}
	else
		tabmsg(IDS_SEL1FRM);
}

bool frmrng(size_t iForm, RANGE& range) noexcept {
	if (PCSHeader.stitchCount) {
		range.start  = 0;
		range.finish = PCSHeader.stitchCount;
		if (FormList[iForm].fillType || FormList[iForm].edgeType) {
			while (range.start < PCSHeader.stitchCount && notfstch(StitchBuffer[range.start].attribute))
				range.start++;
			range.finish = PCSHeader.stitchCount - 1;
			while (range.finish > range.start && notfstch(StitchBuffer[range.finish].attribute))
				range.finish--;
			if (range.finish > range.start)
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	else {
		return 0;
	}
}

void bholbrd() {
	deleclp(ClosestFormToCursor);
	SelectedForm->borderSize = BorderWidth;
	bsizpar();
	SelectedForm->edgeType    = EDGEBHOL;
	SelectedForm->edgeSpacing = LineSpacing;
	SelectedForm->borderColor = ActiveColor;
	savblen(ButtonholeCornerLength);
	refilfn();
}

void bhol() {
	const double savedSpacing = LineSpacing;

	if (filmsgs(FML_BHOL))
		return;
	LineSpacing = savedSpacing;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			bholbrd();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			bholbrd();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
	LineSpacing = savedSpacing;
}

void fcntr() {
	unsigned iVertex       = 0;
	unsigned iStitch       = 0;
	unsigned codedForm     = 0;
	dPOINT   initialCenter = {};
	dPOINT   currentCenter = {};
	dPOINT   delta         = {};

	if (SelectedFormList->size()) {
		savdo();
		auto firstForm = SelectedFormList->front();
		initialCenter.x
		    = (FormList[firstForm].rectangle.right - FormList[firstForm].rectangle.left) / 2 + FormList[firstForm].rectangle.left;
		initialCenter.y = (FormList[firstForm].rectangle.top - FormList[firstForm].rectangle.bottom) / 2
		                  + FormList[firstForm].rectangle.bottom;
		for (auto selectedForm : (*SelectedFormList)) {
			currentCenter.x = (FormList[selectedForm].rectangle.right - FormList[selectedForm].rectangle.left) / 2
			                  + FormList[selectedForm].rectangle.left;
			currentCenter.y = (FormList[selectedForm].rectangle.top - FormList[selectedForm].rectangle.bottom) / 2
			                  + FormList[selectedForm].rectangle.bottom;
			delta.x      = initialCenter.x - currentCenter.x;
			delta.y      = initialCenter.y - currentCenter.y;
			SelectedForm = &FormList[selectedForm];
			for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
				SelectedForm->vertices[iVertex].x += delta.x;
				SelectedForm->vertices[iVertex].y += delta.y;
			}
			frmout(selectedForm);
			codedForm = gsl::narrow<unsigned int>(selectedForm << FRMSHFT);
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) == codedForm && !(StitchBuffer[iStitch].attribute & NOTFRM)) {
					StitchBuffer[iStitch].x += delta.x;
					StitchBuffer[iStitch].y += delta.y;
				}
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else
		tabmsg(IDS_SELC);
}

void boxsel() {
	StateMap.reset(StateFlag::INSRT);
	if (!StateMap.test(StateFlag::THUMSHO)) {
		StateMap.reset(StateFlag::BZUM);
		StateMap.reset(StateFlag::BZUMIN);
		StateMap.set(StateFlag::VCAPT);
		SetCapture(ThrEdWindow);
		StateMap.set(StateFlag::RESTCH);
	}
}

void fspic() {
	unsigned iStitch = 0;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType       = EDGEPICOT;
	SelectedForm->clipEntries    = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize     = ClipRectSize.cy;
	SelectedForm->edgeSpacing    = PicotSpacing;
	SelectedForm->borderColor    = ActiveColor;
	bsizpar();
	savplen(ButtonholeCornerLength);
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->borderClipData[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->borderClipData[iStitch].y = ClipBuffer[iStitch].y;
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
	refilfn();
}

void picot() {
	if (filmsgs(FML_PIC))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		savdo();
		fvars(ClosestFormToCursor);
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (SelectedFormList->size()) {
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						fvars(ClosestFormToCursor);
						fspic();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fspic();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

bool contsf(size_t formIndex) {
	ClosestFormToCursor = formIndex;
	fvars(formIndex);
	if (SelectedForm->vertexCount > 4) {
		delclps(ClosestFormToCursor);
		deltx();
		chkcont();
		SelectedForm->fillSpacing = LineSpacing;
		SelectedForm->fillColor   = ActiveColor;
		fsizpar();
		SelectedForm->attribute |= (ActiveLayer << 1);
		refilfn();
		return 1;
	}
	return 0;
}

void contfil() {
	if (filmsgs(FML_CONT))
		return;
	if (SelectedFormList->size()) {
		savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			contsf(selectedForm);
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		fvars(ClosestFormToCursor);
		savdo();
		if (contsf(ClosestFormToCursor)) {
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		else
			tabmsg(IDS_CONT);
	}
}

void ribon() {
	unsigned iNewVertex = 0, isBlunt = 0;
	size_t   savedFormIndex = 0;

	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (VertexCount > 2) {
			savdo();
			savedFormIndex = ClosestFormToCursor;
			satout(BorderWidth);

			HorizontalLength2  = BorderWidth / 2;
			FRMHED* formHeader = &FormList[FormIndex];
			if (formHeader) {
				frmclr(formHeader);
				iNewVertex                   = 0;
				formHeader->maxFillStitchLen = 9 * PFGRAN;
				formHeader->minFillStitchLen = MinStitchLength;
				MaxStitchLen                 = 9 * PFGRAN;
				if (SelectedForm->type == FRMLINE) {
					// Set blunt flags
					if (UserFlagMap.test(UserFlag::BLUNT))
						isBlunt = SBLNT | FBLNT;
					else
						isBlunt = 0;
					satends(isBlunt);
					formHeader->vertices                 = adflt(VertexCount << 1);
					formHeader->vertices[0].x            = (*OutsidePoints)[0].x;
					formHeader->vertices[iNewVertex++].y = (*OutsidePoints)[0].y;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						formHeader->vertices[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						formHeader->vertices[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				else {
					formHeader->vertices                 = adflt((VertexCount << 1) + 2);
					formHeader->vertices[0].x            = (*OutsidePoints)[0].x;
					formHeader->vertices[iNewVertex++].y = (*OutsidePoints)[0].y;
					formHeader->underlayIndent           = IniFile.underlayIndent;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						formHeader->vertices[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					formHeader->vertices[iNewVertex++] = (*InsidePoints)[0];
					formHeader->vertices[iNewVertex++] = (*OutsidePoints)[0];
					for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						formHeader->vertices[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				formHeader->type                       = SAT;
				formHeader->fillColor                  = ActiveColor;
				formHeader->fillSpacing                = LineSpacing;
				formHeader->lengthOrCount.stitchLength = IniFile.maxStitchLength;
				formHeader->vertexCount                = iNewVertex;
				formHeader->attribute                  = 1;
				formHeader->wordParam                  = iNewVertex >> 1;
				formHeader->satinGuideCount            = formHeader->wordParam - 2;
				formHeader->satinOrAngle.guide         = adsatk(formHeader->satinGuideCount);
				if (StateMap.test(StateFlag::CNV2FTH)) {
					formHeader->fillType                       = FTHF;
					formHeader->fillInfo.feather.ratio         = IniFile.featherRatio;
					formHeader->fillInfo.feather.upCount       = IniFile.featherUpCount;
					formHeader->fillInfo.feather.downCount     = IniFile.featherDownCount;
					formHeader->fillInfo.feather.fillType      = IniFile.featherFillType;
					formHeader->fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
					// ToDo - should this be ORed in to keep other attributes?
					formHeader->extendedAttribute      = IniFile.featherType;
					formHeader->fillInfo.feather.count = IniFile.featherCount;
					formHeader->fillInfo.feather.color = (ActiveColor + 1) & COLMSK;
				}
				else
					formHeader->fillType = SATF;
				for (auto iGuide = 0u; iGuide < formHeader->satinGuideCount; iGuide++) {
					formHeader->satinOrAngle.guide[iGuide].start  = iGuide + 2;
					formHeader->satinOrAngle.guide[iGuide].finish = formHeader->vertexCount - iGuide - 1;
				}
				FormIndex++;
				frmout(FormIndex - 1);
				ClosestFormToCursor = FormIndex - 1;
				refilfn();
				ClosestFormToCursor = savedFormIndex;
				StateMap.set(StateFlag::DELTO);
				frmdel();
				ClosestFormToCursor = FormIndex - 1;
				StateMap.set(StateFlag::FORMSEL);
				StateMap.set(StateFlag::INIT);
				StateMap.set(StateFlag::RESTCH);
			}
			else {
				throw;
			}
		}
		else
			tabmsg(IDS_FRM2);
	}
	else
		shoseln(IDS_FRM1MSG, IDS_CONVRIB);
}

void dupfn(double rotationAngle) {
	dPOINT rotationCenter;
	savdo();
	rotpar(rotationCenter);
	if (IniFile.rotationAngle) {
		if (StateMap.test(StateFlag::FORMSEL))
			duprot(rotationAngle, rotationCenter);
		else {
			if (StateMap.test(StateFlag::GRPSEL))
				duprots(rotationAngle, rotationCenter);
			else {
				if (SelectedFormList->size())
					duprotfs(rotationAngle);
				else
					shord();
			}
		}
	}
	else {
		rotentr(rotationAngle);
		StateMap.set(StateFlag::ENTRDUP);
	}
}

void redup() {
	dupfn(IniFile.rotationAngle);
}

void bakdup() {
	dupfn(2 * PI - IniFile.rotationAngle);
}

void shrnks() {
	unsigned iVertex = 0, ine = 0, count = 0;
	dPOINT   delta  = {};
	double   length = 0.0, ratio = 0.0, deltaLength = 0.0, truncationDelta = 0.0;

	oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		delta.x                            = CurrentFormVertices[iVertex + 1].x - CurrentFormVertices[iVertex].x;
		delta.y                            = CurrentFormVertices[iVertex + 1].y - CurrentFormVertices[iVertex].y;
		length                             = hypot(delta.x, delta.y);
		count                              = length / ClipRectSize.cx + 0.5;
		ratio                              = (ClipRectSize.cx * count + 0.004) / length;
		CurrentFormVertices[iVertex + 1].x = CurrentFormVertices[iVertex].x + delta.x * ratio;
		CurrentFormVertices[iVertex + 1].y = CurrentFormVertices[iVertex].y + delta.y * ratio;
	}
	delta.x              = CurrentFormVertices[0].x - CurrentFormVertices[1].x;
	delta.y              = CurrentFormVertices[0].y - CurrentFormVertices[1].y;
	length               = hypot(delta.x, delta.y);
	double rotationAngle = atan2(delta.y, delta.x);
	// ToDo - what does this loop do?
	for (ine = 0; ine < 5; ine++) {
		delta.x         = CurrentFormVertices[0].x - CurrentFormVertices[iVertex].x;
		delta.y         = CurrentFormVertices[0].y - CurrentFormVertices[iVertex].y;
		deltaLength     = hypot(delta.x, delta.y);
		count           = deltaLength / ClipRectSize.cx;
		truncationDelta = deltaLength - count * ClipRectSize.cx;
		rotationAngle -= truncationDelta / length;
		delta.x                  = cos(rotationAngle) * length;
		delta.y                  = sin(rotationAngle) * length;
		CurrentFormVertices[0].x = CurrentFormVertices[1].x + delta.x;
		CurrentFormVertices[0].y = CurrentFormVertices[1].y + delta.y;
	}
	refil();
}

void shrnk() {
	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FORMSEL) && SelectedForm->edgeType == EDGECLIP) {
		shrnks();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_FRMCLP, IDS_SHRNK);
}

void dufdat(std::vector<fPOINT>& tempClipPoints,
            std::vector<SATCON>& tempGuides,
            std::vector<fPOINT>& destinationFormVertices,
            std::vector<FRMHED>& destinationFormList,
            size_t               formIndex) {
	FRMHED& destination = destinationFormList[FormRelocationIndex];

	destinationFormList[FormRelocationIndex++] = FormList[formIndex];

	const auto res = std::copy(
	    destination.vertices, destination.vertices + destination.vertexCount, destinationFormVertices.begin() + FormVertexIndex);
	destination.vertices = &FormVertices[FormVertexIndex];
	FormVertexIndex += destination.vertexCount;
	if (destination.satinGuideCount) {
		const auto _ = std::copy(destination.satinOrAngle.guide,
		                         destination.satinOrAngle.guide + destination.satinGuideCount,
		                         tempGuides.begin() + SatinGuideIndex);

		destination.satinOrAngle.guide = &SatinGuides[SatinGuideIndex];
		SatinGuideIndex += destination.satinGuideCount;
	}
	if (iseclpx(formIndex)) {
		const auto _ = std::copy(destination.borderClipData,
		                         destination.borderClipData + destination.clipEntries,
		                         tempClipPoints.begin() + ClipPointIndex);

		destination.borderClipData = &ClipPoints[ClipPointIndex];
		ClipPointIndex += destination.clipEntries;
	}
	if (isclpx(formIndex)) {
		const auto _ = std::copy(destination.angleOrClipData.clip,
		                         destination.angleOrClipData.clip + destination.lengthOrCount.clipCount,
		                         tempClipPoints.begin() + ClipPointIndex);

		destination.angleOrClipData.clip = &ClipPoints[ClipPointIndex];
		ClipPointIndex += destination.lengthOrCount.clipCount;
	}
}

void stchfrm(unsigned formIndex, unsigned* attribute) noexcept {
	if (attribute) {
		*attribute &= NFRMSK;
		*attribute |= formIndex << FRMSHFT;
	}
}

void frmnumfn(unsigned newFormIndex) {
	unsigned iStitch = 0, sourceForm = 0, decodedFormIndex = 0;
	size_t   start = 0, finish = 0;

	savdo();
	if (newFormIndex != ClosestFormToCursor) {
		if (ClosestFormToCursor > newFormIndex) {
			start  = newFormIndex;
			finish = ClosestFormToCursor;
		}
		else {
			start  = ClosestFormToCursor;
			finish = newFormIndex;
		}
		sourceForm = FormRelocationIndex = 0;

		std::vector<FRMHED> tempFormList(FormIndex);
		std::vector<fPOINT> tempFormVertices(FormVertexIndex);
		std::vector<SATCON> tempGuides(SatinGuideIndex);
		std::vector<fPOINT> tempClipPoints(ClipPointIndex);

		FormVertexIndex = SatinGuideIndex = ClipPointIndex = 0;
		for (auto iForm = 0u; iForm < FormIndex; iForm++) {
			if (iForm == newFormIndex)
				dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, ClosestFormToCursor);
			else {
				if (sourceForm == ClosestFormToCursor) {
					sourceForm++;
				}
				dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, sourceForm++);
			}
		}
		std::copy(tempFormList.cbegin(), tempFormList.cend(), FormList);
		std::copy(tempFormVertices.cbegin(), tempFormVertices.cend(), FormVertices);
		std::copy(tempGuides.cbegin(), tempGuides.cend(), SatinGuides);
		std::copy(tempClipPoints.cbegin(), tempClipPoints.cend(), ClipPoints);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute & SRTYPMSK) {
				decodedFormIndex = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
				if (decodedFormIndex == ClosestFormToCursor)
					stchfrm(newFormIndex, &StitchBuffer[iStitch].attribute);
				else {
					if (decodedFormIndex >= start && decodedFormIndex <= finish) {
						if (newFormIndex < ClosestFormToCursor)
							stchfrm(decodedFormIndex + 1, &StitchBuffer[iStitch].attribute);
						else
							stchfrm(decodedFormIndex - 1, &StitchBuffer[iStitch].attribute);
					}
				}
			}
		}
		ClosestFormToCursor = newFormIndex;
		ritnum(STR_NUMFRM, ClosestFormToCursor);
	}
}

constexpr unsigned duat(unsigned attribute) {
	const unsigned type = ((attribute >> TYPSHFT) + 1) & 3;
	const unsigned frm  = (attribute & FRMSK) >> 2;

	return type | frm;
}

void srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, unsigned start, unsigned finish) {
	unsigned iForm = 0, iStitch = 0, stitchAccumulator = 0, value = 0;

	if (start != finish) {
		std::vector<unsigned> stitchHistogram(FormIndex << 2);
		for (iStitch = start; iStitch < finish; iStitch++)
			stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++;
		stitchAccumulator = start;
		for (iForm = 0; iForm < FormIndex << 2; iForm++) {
			value                  = stitchHistogram[iForm];
			stitchHistogram[iForm] = stitchAccumulator;
			stitchAccumulator += value;
		}
		for (iStitch = start; iStitch < finish; iStitch++) {
			StitchBuffer[stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++] = tempStitchBuffer[iStitch];
		}
	}
}

void srtbyfrm() {
	unsigned              iStitch = 0, iColor = 0, colorAccumulator = 0, value = 0;
	std::vector<unsigned> colorHistogram(16);
	std::vector<unsigned> color(16);

	if (FormIndex) {
		savdo();
		color[AppliqueColor] = 0;
		for (iColor = 0; iColor < 16; iColor++) {
			if (iColor != AppliqueColor)
				color[iColor] = iColor + 1;
		}
		std::vector<fPOINTATTR> tempStitchBuffer(PCSHeader.stitchCount);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			colorHistogram[color[StitchBuffer[iStitch].attribute & 0xf]]++;
		colorAccumulator = 0;
		for (iColor = 0; iColor < 16; iColor++) {
			value                  = colorHistogram[iColor];
			colorHistogram[iColor] = colorAccumulator;
			colorAccumulator += value;
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			tempStitchBuffer[colorHistogram[color[StitchBuffer[iStitch].attribute & 0xf]]++] = StitchBuffer[iStitch];
		}
		srtf(tempStitchBuffer, 0, colorHistogram[0]);
		for (iColor = 0; iColor < 15; iColor++) {
			srtf(tempStitchBuffer, colorHistogram[iColor], colorHistogram[iColor + 1]);
		}
	}
	else
		srtcol();
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void cntrx() {
	dPOINT      markCenter     = {};
	dPOINT      selectedCenter = {};
	unsigned    iStitch        = 0;
	bool        flag           = false;
	fRECTANGLE* formRect       = nullptr;
	fRECTANGLE  groupRect      = {};

	if (StateMap.test(StateFlag::GMRK)) {
		markCenter.x = ZoomMarkPoint.x;
		markCenter.y = ZoomMarkPoint.y;
	}
	else {
		markCenter.x = UnzoomedRect.x / 2;
		markCenter.y = UnzoomedRect.y / 2;
	}
	if (SelectedFormList->size()) {
		flag = true;
		savdo();
		dufcntr(selectedCenter);
		FormMoveDelta.x = markCenter.x - selectedCenter.x;
		FormMoveDelta.y = -markCenter.y + selectedCenter.y;
		if (StateMap.test(StateFlag::CNTRV))
			FormMoveDelta.y = 0;
		if (StateMap.test(StateFlag::CNTRH))
			FormMoveDelta.x = 0;
		for (auto selectedForm : (*SelectedFormList)) {
			frmadj(selectedForm);
		}
		frmsadj();
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			flag = true;
			savdo();
			formRect         = &FormList[ClosestFormToCursor].rectangle;
			selectedCenter.x = (formRect->right - formRect->left) / 2 + formRect->left;
			selectedCenter.y = (formRect->top - formRect->bottom) / 2 + formRect->bottom;
			FormMoveDelta.x  = markCenter.x - selectedCenter.x;
			FormMoveDelta.y  = -markCenter.y + selectedCenter.y;
			if (StateMap.test(StateFlag::CNTRV))
				FormMoveDelta.y = 0;
			if (StateMap.test(StateFlag::CNTRH))
				FormMoveDelta.x = 0;
			frmadj(ClosestFormToCursor);
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (StitchBuffer[iStitch].attribute & ALTYPMSK
				    && (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
					StitchBuffer[iStitch].x += FormMoveDelta.x;
					StitchBuffer[iStitch].y -= FormMoveDelta.y;
				}
			}
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				flag = true;
				savdo();
				rngadj();
				groupRect.right = groupRect.left = StitchBuffer[GroupStartStitch].x;
				groupRect.top = groupRect.bottom = StitchBuffer[GroupStartStitch].y;
				for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
					if (StitchBuffer[iStitch].x < groupRect.left)
						groupRect.left = StitchBuffer[iStitch].x;
					if (StitchBuffer[iStitch].x > groupRect.right)
						groupRect.right = StitchBuffer[iStitch].x;
					if (StitchBuffer[iStitch].y < groupRect.bottom)
						groupRect.bottom = StitchBuffer[iStitch].y;
					if (StitchBuffer[iStitch].y > groupRect.top)
						groupRect.top = StitchBuffer[iStitch].y;
				}
				selectedCenter.x = (groupRect.right - groupRect.left) / 2 + groupRect.left;
				selectedCenter.y = (groupRect.top - groupRect.bottom) / 2 + groupRect.bottom;
				FormMoveDelta.x  = markCenter.x - selectedCenter.x;
				FormMoveDelta.y  = -markCenter.y + selectedCenter.y;
				if (StateMap.test(StateFlag::CNTRV))
					FormMoveDelta.y = 0;
				if (StateMap.test(StateFlag::CNTRH))
					FormMoveDelta.x = 0;
				for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].x += FormMoveDelta.x;
					StitchBuffer[iStitch].y -= FormMoveDelta.y;
				}
			}
			else
				shoseln(IDS_FGRPF, IDS_CENT);
		}
	}
	if (flag)
		StateMap.set(StateFlag::RESTCH);
}

void centir() {
	dPOINT   hoopCenter = {};
	dPOINT   itemCenter = {};
	dPOINT   delta      = {};
	unsigned iVertex = 0, iForm = 0, iStitch = 0;

	StateMap.reset(StateFlag::BIGBOX);
	getbig();
	itemCenter.x = (AllItemsRect.right - AllItemsRect.left) / 2 + AllItemsRect.left;
	itemCenter.y = (AllItemsRect.top - AllItemsRect.bottom) / 2 + AllItemsRect.bottom;
	hoopCenter.x = UnzoomedRect.x / 2;
	hoopCenter.y = UnzoomedRect.y / 2;
	delta.x      = hoopCenter.x - itemCenter.x;
	delta.y      = hoopCenter.y - itemCenter.y;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].x += delta.x;
		StitchBuffer[iStitch].y += delta.y;
	}
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {
		FormVertices[iVertex].x += delta.x;
		FormVertices[iVertex].y += delta.y;
	}
	for (iForm = 0; iForm < FormIndex; iForm++)
		frmout(iForm);
	setfchk();
	StateMap.set(StateFlag::RESTCH);
}

void bean(unsigned start, unsigned finish) {
	unsigned iOutputStitch = start;
	unsigned iSourceStitch = start;
	unsigned iCopyStitch   = MAXITEMS;

	BeanCount = 0;
	mvstch(iCopyStitch++, iSourceStitch);
	if (StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x
	    || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y) {
		mvstch(iCopyStitch++, iSourceStitch + 1);
		mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	mvstch(iCopyStitch++, iSourceStitch);
	if (StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x
	    || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y) {
		mvstch(iCopyStitch++, iSourceStitch + 1);
		mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	while (iSourceStitch < finish - 2) {
		mvstch(iCopyStitch++, iSourceStitch);
		if ((StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x
		     || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y)
		    && (StitchBuffer[iSourceStitch - 2].x != StitchBuffer[iSourceStitch].x
		        || StitchBuffer[iSourceStitch - 2].y != StitchBuffer[iSourceStitch].y)) {
			mvstch(iCopyStitch++, iSourceStitch + 1);
			mvstch(iCopyStitch++, iSourceStitch);
			BeanCount += 2;
		}
		iSourceStitch++;
	}
	mvstch(iCopyStitch++, iSourceStitch);
	if ((StitchBuffer[iSourceStitch - 2].x != StitchBuffer[iSourceStitch].x
	     || StitchBuffer[iSourceStitch - 2].y != StitchBuffer[iSourceStitch].y)) {
		mvstch(iCopyStitch++, iSourceStitch + 1);
		mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	while (iSourceStitch < PCSHeader.stitchCount)
		mvstch(iCopyStitch++, iSourceStitch++);
	for (iSourceStitch = MAXITEMS; iSourceStitch < iCopyStitch; iSourceStitch++)
		mvstch(iOutputStitch++, iSourceStitch);
	PCSHeader.stitchCount = iOutputStitch;
}

void dubean() {
	if (PCSHeader.stitchCount) {
		savdo();
		if (StateMap.test(StateFlag::GRPSEL)) {
			rngadj();
			bean(GroupStartStitch, GroupEndStitch);
			if (ClosestPointIndex > GroupStitchIndex)
				ClosestPointIndex += BeanCount;
			else
				GroupStitchIndex += BeanCount;
			grpAdj();
		}
		else
			bean(0, PCSHeader.stitchCount - 1);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void unbean(unsigned start, unsigned finish) {
	unsigned iSource = start, iCopy = MAXITEMS;

	BeanCount = 0;
	for (iSource = start; iSource <= finish; iSource++) {
		mvstch(iCopy++, iSource);
		if (StitchBuffer[iSource].x == StitchBuffer[iSource + 2].x && StitchBuffer[iSource].y == StitchBuffer[iSource + 2].y) {
			iSource += 2;
			BeanCount += 2;
		}
	}
	if (BeanCount)
		BeanCount -= 2;
	;
	if (iSource > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
		iSource = PCSHeader.stitchCount - 1;
	while (iSource < PCSHeader.stitchCount)
		mvstch(iCopy++, iSource++);
	mvstchs(start, MAXITEMS, iCopy);
	PCSHeader.stitchCount = start + iCopy;
}

void debean() {
	savdo();
	if (StateMap.test(StateFlag::GRPSEL)) {
		rngadj();
		unbean(GroupStartStitch, GroupEndStitch);
		if (ClosestPointIndex > GroupStitchIndex)
			ClosestPointIndex -= BeanCount;
		else
			GroupStitchIndex -= BeanCount;
		if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		if (GroupStitchIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			GroupStitchIndex = PCSHeader.stitchCount - 1;
		grpAdj();
	}
	else
		unbean(0, PCSHeader.stitchCount - 1);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void mvfrmsb(FRMHED* const destination, const FRMHED* const source, size_t count) noexcept {
	if (source && destination) {
		memmove(destination, source, count * sizeof(*source));
	}
}

void mvfltsb(fPOINT* const destination, const fPOINT* const source, size_t count) noexcept {
	if (source && destination) {
		memmove(destination, source, count * sizeof(*source));
	}
}

void clpspac(const fPOINT* const insertPoint, size_t count) noexcept {
	mvfltsb(&ClipPoints[ClipPointIndex + count - 1], &ClipPoints[ClipPointIndex - 1], ClipPointIndex - clpind(insertPoint));
}

void stchadj() {
	unsigned iStitch = 0, low = 0, high = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		high = StitchBuffer[iStitch].attribute & FRMSK;
		low  = high >> FRMSHFT;
		if (low > ClosestFormToCursor) {
			StitchBuffer[iStitch].attribute &= NFRMSK;
			high += 1 << FRMSHFT;
			StitchBuffer[iStitch].attribute |= high;
		}
	}
	refilfn();
	low = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
	for (iStitch = PCSHeader.stitchCount; iStitch != 0; iStitch--) {
		high = StitchBuffer[iStitch - 1].attribute & FRMSK;
		if ((StitchBuffer[iStitch - 1].attribute & FRMSK) == low) {
			break;
		}
	}
	ClosestFormToCursor++;
	refilfn();
	StateMap.reset(StateFlag::FRMPSEL);
}

void spltsat(const SATCON& currentGuide) {
	// We are adding two additional vertices when splitting the form
	std::vector<fPOINT> vertexBuffer(VertexCount + 2);
	unsigned            iGuide = 0, iVertex = 0;
	size_t              oldLastVertex = 0, iNewVertex = 0, iOldVertex = 0;

	mvfrmsb(&FormList[FormIndex], &FormList[FormIndex - 1], FormIndex - ClosestFormToCursor);
	FormIndex++;
	if (ClosestFormToCursor < FormIndex - 2) {
		mvfltsb(&FormVertices[FormVertexIndex + 1],
		        &FormVertices[FormVertexIndex - 1],
		        FormVertexIndex - fltind(FormList[ClosestFormToCursor + 2].vertices));
	}
	FormVertexIndex += 2;
	for (auto iForm = ClosestFormToCursor + 2; iForm < FormIndex; iForm++)
		FormList[iForm].vertices += 2;
	iOldVertex    = 0;
	oldLastVertex = currentGuide.start + (VertexCount - currentGuide.finish) + 1;
	iNewVertex    = oldLastVertex + 1;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		if (iVertex == currentGuide.start || iVertex == currentGuide.finish) {
			vertexBuffer[iOldVertex++] = CurrentFormVertices[iVertex];
			if (iVertex == currentGuide.start) {
				vertexBuffer[iNewVertex++] = CurrentFormVertices[iVertex];
			}
			else {
				vertexBuffer[oldLastVertex] = CurrentFormVertices[iVertex];
			}
		}
		else {
			if (iVertex < currentGuide.start) {
				vertexBuffer[iOldVertex++] = CurrentFormVertices[iVertex];
			}
			else {
				if (iVertex < currentGuide.finish) {
					vertexBuffer[iNewVertex++] = CurrentFormVertices[iVertex];
				}
				else {
					vertexBuffer[iOldVertex++] = CurrentFormVertices[iVertex];
				}
			}
		}
	}
	for (iVertex = 0; iVertex < iNewVertex; iVertex++) {
		CurrentFormVertices[iVertex] = vertexBuffer[iVertex];
	}
	SelectedForm->vertexCount                     = iOldVertex;
	FormList[ClosestFormToCursor + 1].vertexCount = iNewVertex - iOldVertex;
	FormList[ClosestFormToCursor + 1].vertices    = &CurrentFormVertices[iOldVertex];
	frmout(ClosestFormToCursor);
	frmout(ClosestFormToCursor + 1);
	iOldVertex = currentGuide.start + 1 - currentGuide.finish;
	for (iGuide = 0; iGuide < ActivePointIndex; iGuide++)
		SelectedForm->satinOrAngle.guide[iGuide].finish += iOldVertex;
	if (SelectedForm->wordParam)
		SelectedForm->wordParam = currentGuide.start;
	iOldVertex = iGuide + 1;
	while (iGuide < gsl::narrow<unsigned>(SelectedForm->satinGuideCount)) {
		SelectedForm->satinOrAngle.guide[iGuide].start -= (currentGuide.start - 1);
		SelectedForm->satinOrAngle.guide[iGuide].finish -= (currentGuide.start - 1);
		iGuide++;
	}
	if (FormList[ClosestFormToCursor + 1].wordParam)
		FormList[ClosestFormToCursor + 1].wordParam -= (currentGuide.start - 1);
	auto       sourceStart = SelectedForm->satinOrAngle.guide + iOldVertex;
	auto       sourceRange = SatinGuideIndex - satind(&SelectedForm->satinOrAngle.guide[iOldVertex]);
	auto       sourceEnd   = sourceStart + sourceRange;
	const auto destination = stdext::make_checked_array_iterator((sourceStart - 1), sourceRange);
	std::copy(sourceStart, sourceEnd, destination);
	SatinGuideIndex--;
	FormList[ClosestFormToCursor + 1].satinOrAngle.guide = &SelectedForm->satinOrAngle.guide[ActivePointIndex];
	FormList[ClosestFormToCursor + 1].satinGuideCount    = SelectedForm->satinGuideCount - ActivePointIndex - 1;
	SelectedForm->satinGuideCount                        = ActivePointIndex;
	for (auto iForm = ClosestFormToCursor + 2; iForm < FormIndex; iForm++)
		FormList[iForm].satinOrAngle.guide--;
	if (iseclp(ClosestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
			FormList[iForm].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
}

bool spltlin() {
	if (ClosestVertexToCursor < 2 || SelectedForm->vertexCount - ClosestVertexToCursor < 2) {
		return 0;
	}
	mvfrmsb(&FormList[FormIndex], &FormList[FormIndex - 1], FormIndex - ClosestFormToCursor);
	FormIndex++;
	SelectedForm->vertexCount = ClosestVertexToCursor;
	FormList[ClosestFormToCursor + 1].vertexCount -= ClosestVertexToCursor;
	FormList[ClosestFormToCursor + 1].vertices = &SelectedForm->vertices[ClosestVertexToCursor];
	frmout(ClosestFormToCursor);
	frmout(ClosestFormToCursor + 1);
	if (iseclp(ClosestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
			FormList[iForm].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
	return 1;
}

void spltfrm() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		savdo();
		fvars(ClosestFormToCursor);
		if (SelectedForm->type == SAT) {
			if (SelectedForm->satinGuideCount) {
				delfstchs();
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				for (ActivePointIndex = 0; ActivePointIndex < SelectedForm->satinGuideCount; ActivePointIndex++) {
					if (SelectedForm->satinOrAngle.guide[ActivePointIndex].start == ClosestVertexToCursor
					    || SelectedForm->satinOrAngle.guide[ActivePointIndex].finish == ClosestVertexToCursor) {
						spltsat(SelectedForm->satinOrAngle.guide[ActivePointIndex]);
						return;
					}
				}
				spltmsg();
			}
			else
				spltmsg();
		}
		else {
			if (SelectedForm->type == FRMLINE) {
				if (spltlin()) {
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else
					tabmsg(IDS_FRM3);
				return;
			}
			else
				spltmsg();
		}
	}
	spltmsg();
}

void stchs2frm() {
	unsigned iStitch = 0, iVertex = 0, vertexCount = 0;

	if (StateMap.test(StateFlag::GRPSEL)) {
		rngadj();
		if ((GroupEndStitch - GroupStartStitch) > 12000) {
			tabmsg(IDS_STMAX);
			return;
		}
		vertexCount  = GroupEndStitch - GroupStartStitch + 1;
		SelectedForm = &FormList[FormIndex];
		frmclr(SelectedForm);
		SelectedForm->type        = FRMLINE;
		SelectedForm->vertexCount = vertexCount;
		SelectedForm->vertices    = adflt(vertexCount);
		iVertex                   = 0;
		for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
			SelectedForm->vertices[iVertex].x   = StitchBuffer[iStitch].x;
			SelectedForm->vertices[iVertex++].y = StitchBuffer[iStitch].y;
		}
		frmout(FormIndex);
		FormIndex++;
		if (ClosestPointIndex > GroupStitchIndex) {
			if (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
				ClosestPointIndex++;
		}
		else {
			if (GroupStitchIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
				GroupStitchIndex++;
		}
		delstchm();
		StateMap.reset(StateFlag::GRPSEL);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_GRPMSG, IDS_STCH2FRM);
}

void vrtsclp() {
	unsigned iStitch = 0;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->angleOrClipData.clip    = numclp();
	SelectedForm->wordParam               = IniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->angleOrClipData.clip[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->angleOrClipData.clip[iStitch].y = ClipBuffer[iStitch].y;
	}
	SelectedForm->fillType  = VCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type      = FRMFPOLY;
	refilfn();
}

void vrtclp() {
	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (SelectedFormList->size()) {
					StateMap.set(StateFlag::NOCLP);
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							vrtsclp();
					}
					StateMap.reset(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fvars(ClosestFormToCursor);
						vrtsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void horsclp() {
	unsigned iStitch = 0;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->angleOrClipData.clip    = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->wordParam               = IniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->angleOrClipData.clip[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->angleOrClipData.clip[iStitch].y = ClipBuffer[iStitch].y;
	}
	SelectedForm->fillType  = HCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type      = FRMFPOLY;
	CurrentFormVertices     = SelectedForm->vertices;
	refilfn();
}

void horclp() {
	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (SelectedFormList->size()) {
					StateMap.set(StateFlag::NOCLP);
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							horsclp();
					}
					StateMap.set(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fvars(ClosestFormToCursor);
						horsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void angsclp() {
	unsigned iStitch = 0;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->angleOrClipData.clip    = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->wordParam               = IniFile.fillPhase;
	makpoli();
	SelectedForm->satinOrAngle.angle = IniFile.fillAngle;
	SelectedForm->fillSpacing        = IniFile.clipOffset;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->angleOrClipData.clip[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->angleOrClipData.clip[iStitch].y = ClipBuffer[iStitch].y;
	}
	SelectedForm->fillType  = ANGCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type      = FRMFPOLY;
	refilfn();
}

void angclp() {
	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (SelectedFormList->size()) {
					StateMap.set(StateFlag::NOCLP);
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							angsclp();
					}
					StateMap.reset(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						SelectedForm = &FormList[ClosestFormToCursor];
						angsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void dubsfil() {
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType    = EDGEDOUBLE;
	SequenceIndex             = 0;
	SelectedForm->borderColor = ActiveColor;
	dubfn();
	bsizpar();
	refilfn();
}

void dubfil() {
	if (filmsgs(FML_LIN))
		return;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			dubsfil();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			dubsfil();
			coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void col2frm() {
	unsigned     iColor = 0, formColorCode = 0, count = 0, majorityColor = 0;
	unsigned     startColorOffset = 0, endColorOffset = 0, colorChangedCount = 0;
	const size_t formColorPermutations = FormIndex << 4; // total number of form and color combinations

	if (FormIndex) {
		std::vector<unsigned> fillColorHistogram(formColorPermutations);
		std::vector<unsigned> borderColorHistogram(formColorPermutations);
		std::vector<unsigned> featherColorHistogram(formColorPermutations);
		std::vector<unsigned> underlayColorHistogram(formColorPermutations);
		for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			formColorCode = StitchBuffer[iStitch].attribute & 0x3fff;
			if (StitchBuffer[iStitch].attribute & (WLKMSK | CWLKMSK | UNDMSK))
				underlayColorHistogram[formColorCode]++;
			else {
				if (StitchBuffer[iStitch].attribute & FTHMSK)
					featherColorHistogram[formColorCode]++;
				else {
					switch (StitchBuffer[iStitch].attribute & TYPMSK) {
					case FRMFIL:
						fillColorHistogram[formColorCode]++;
						break;
					case FRMBFIL:
						borderColorHistogram[formColorCode]++;
						break;
					}
				}
			}
		}
		startColorOffset = 0;
		endColorOffset   = 16;
		for (auto iForm = 0ul; iForm < FormIndex; iForm++) {
			if (FormList[iForm].fillType) {
				count = majorityColor = 0;
				for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (fillColorHistogram[iColor] > count) {
						count         = fillColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (FormList[iForm].fillColor != majorityColor) {
					colorChangedCount++;
					FormList[iForm].fillColor = majorityColor;
				}
				if (FormList[iForm].fillType == FTHF && FormList[iForm].extendedAttribute & AT_FTHBLND) {
					count = majorityColor = 0;
					for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
						if (featherColorHistogram[iColor] > count) {
							count         = fillColorHistogram[iColor];
							majorityColor = iColor;
						}
					}
					majorityColor &= 0xf;
					if (FormList[iForm].fillInfo.feather.color != majorityColor) {
						colorChangedCount++;
						FormList[iForm].fillInfo.feather.color = majorityColor;
					}
				}
			}
			if (FormList[iForm].edgeType) {
				count = majorityColor = 0;
				for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (borderColorHistogram[iColor] > count) {
						count         = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (FormList[iForm].borderColor != majorityColor) {
					colorChangedCount++;
					FormList[iForm].borderColor = majorityColor;
				}
			}
			if (FormList[iForm].extendedAttribute & (AT_WALK | AT_CWLK | AT_UND)) {
				count = majorityColor = 0;
				for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (underlayColorHistogram[iColor] > count) {
						count         = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (FormList[iForm].underlayColor != majorityColor) {
					colorChangedCount++;
					FormList[iForm].underlayColor = majorityColor;
				}
			}
			startColorOffset += 16;
			endColorOffset += 16;
		}
	}
	std::wstring fmtStr;
	loadString(fmtStr, IDS_NCOLCHG);
	shoMsg(fmt::format(fmtStr, colorChangedCount));
}

void chan() {
	SelectedForm->borderColor        = ActiveColor;
	SelectedForm->edgeSpacing        = IniFile.chainSpace;
	SelectedForm->borderSize         = BorderWidth;
	SelectedForm->edgeStitchLen      = IniFile.chainRatio;
	SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	SelectedForm->minBorderStitchLen = MinStitchLength;
	if (StateMap.test(StateFlag::LINCHN))
		SelectedForm->edgeType = EDGELCHAIN;
	else
		SelectedForm->edgeType = EDGEOCHAIN;
	refilfn();
}

void chain() {
	if (filmsgs(FML_CHAIN))
		return;
	savdo();
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			chan();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			chan();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void crop() {
	unsigned iSource = 0, iDestination = 0;

	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		savdo();
		fvars(ClosestFormToCursor);
		iDestination = 0;
		for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
			if (cisin(StitchBuffer[iSource].x, StitchBuffer[iSource].y)) {
				StitchBuffer[iDestination++] = StitchBuffer[iSource];
			}
		}
		PCSHeader.stitchCount = iDestination;
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_FRM1MSG, IDS_CROP);
}

void fsclpx() {
	unsigned iPoint = 0;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType       = EDGECLIPX;
	SelectedForm->clipEntries    = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize     = ClipRectSize.cy;
	SelectedForm->edgeSpacing    = ClipRectSize.cx;
	SelectedForm->borderColor    = ActiveColor;
	bsizpar();
	for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
		SelectedForm->borderClipData[iPoint].x = ClipBuffer[iPoint].x;
		SelectedForm->borderClipData[iPoint].y = ClipBuffer[iPoint].y;
	}
	duxclp();
	refilfn();
}

void filclpx() {
	if (filmsgs(FML_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		fvars(ClosestFormToCursor);
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			savdo();
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (SelectedFormList->size()) {
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						fvars(ClosestFormToCursor);
						fsclpx();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fsclpx();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else {
			std::wstring str(L"no clipboard data");
			shoMsg(str);
			CloseClipboard();
		}
	}
}

void srtfrm() {
	unsigned              iStitch = 0, iForm = 0, iHighStitch = 0, totalStitches = 0, formStitchCount = 0;
	std::vector<unsigned> histogram(FormIndex);

	if (PCSHeader.stitchCount) {
		savdo();
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			iForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			histogram[iForm]++;
		}
		totalStitches = 0;
		for (auto& entry : histogram) {
			formStitchCount = entry;
			entry           = totalStitches;
			totalStitches += formStitchCount;
		}
		std::vector<fPOINTATTR> highStitchBuffer(PCSHeader.stitchCount);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			iForm                         = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			iHighStitch                   = histogram[iForm]++;
			highStitchBuffer[iHighStitch] = StitchBuffer[iStitch];
		}
		std::copy(
		    highStitchBuffer.cbegin(), highStitchBuffer.cend(), stdext::make_checked_array_iterator(StitchBuffer, MAXITEMS));
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}
