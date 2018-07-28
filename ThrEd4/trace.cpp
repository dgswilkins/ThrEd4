#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Local Headers
#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "form.h"
#include "thred.h"

unsigned                 PixelColors[3];        // separated pixel reference colors
HBITMAP                  TraceBitmap;           // trace bitmap
HWND                     TraceControlWindow[3]; // trace control windows
HWND                     TraceDownWindow[3];    // trace down number windows
HWND                     TraceSelectWindow[3];  // trace select windows
HWND                     TraceUpWindow[3];      // trace up number windows
POINT                    CurrentTracePoint;     // current point being traced
unsigned                 TraceDataSize;         // size of the trace bitmap in double words
unsigned*                TracedPixels;          // bitmap of selected trace pixels
HWND                     TraceStepWin;          // trace stepSize window
StateFlag                TraceRGBFlag[] = { StateFlag::TRCRED, StateFlag::TRCGRN, StateFlag::TRCBLU }; // trace bits
unsigned                 TraceRGBMask[] = { REDMSK, GRNMSK, BLUMSK };                                  // trace masks
unsigned                 TraceRGB[]     = { BLUCOL, GRNCOL, REDCOL };                                  // trace colors
unsigned                 TraceAdjacentColors[9];      // separated colors for adjacent pixels
wchar_t                  TraceInputBuffer[4];         // for user input color numbers
COLORREF                 UpPixelColor;                // color of the up reference pixel
COLORREF                 DownPixelColor;              // color of the down reference pixel
COLORREF                 InvertUpColor;               // complement color of the up reference pixel
COLORREF                 InvertDownColor;             // complement color of the down reference pixel
POINT                    TraceMsgPoint;               // message point for trace parsing
unsigned                 HighColors[3];               // separated upper reference colors
unsigned                 LowColors[3];                // separated lower reference colors
unsigned                 ColumnColor;                 // trace color column
unsigned                 TraceShift[] = { 0, 8, 16 }; // trace shift values
HBRUSH                   TraceBrush[3];               // red,green,and blue brushes
HWND                     TraceNumberInput;            // trace number input window
HPEN                     BlackPen;                    // black pen

void initColorRef(){
	UpPixelColor = 0;
	DownPixelColor = 0x7f7f7f;
	InvertUpColor = 0xffffff;
	InvertDownColor = 0x808080;
}

void trcsub(HWND* window, unsigned xCoordinate, unsigned yCoordinate, unsigned buttonHeight) noexcept {
	if (window) {
		*window = CreateWindow(L"STATIC",
			L"",
			SS_OWNERDRAW | WS_CHILD | WS_BORDER,
			xCoordinate,
			yCoordinate,
			ButtonWidth,
			buttonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);
	}
}

void initTraceWindows() {
	TraceStepWin = CreateWindow(L"STATIC",
		L"",
		SS_NOTIFY | SS_CENTER | WS_CHILD | WS_BORDER,
		0,
		ButtonHeight * 18,
		ButtonWidthX3,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
	for (auto iRGB = 0; iRGB < 3; iRGB++) {
		trcsub(&TraceControlWindow[iRGB], ButtonWidth * iRGB, 0, ButtonHeight * 15);
		trcsub(&TraceSelectWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 15, ButtonHeight);
		trcsub(&TraceUpWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 16, ButtonHeight);
		trcsub(&TraceDownWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 17, ButtonHeight);
		TraceBrush[iRGB] = CreateSolidBrush(TraceRGB[iRGB]);
	}
}

void trcols(COLORREF color) noexcept {
	PixelColors[0] = color & 0xff;
	PixelColors[1] = (color & 0xff00) >> 8;
	PixelColors[2] = (color & 0xff0000) >> 16;
}

unsigned colsum(COLORREF color) {
	unsigned colorSum = 0;
	unsigned iRGB     = 0;

	trcols(color);
	for (iRGB = 0; iRGB < 3; iRGB++) {
		if (StateMap.test(TraceRGBFlag[iRGB]))
			colorSum += PixelColors[iRGB];
	}
	return colorSum;
}

unsigned icolsum(COLORREF color) {
	unsigned colorSum = 0;
	unsigned iRGB     = 0;

	trcols(color);
	for (iRGB = 0; iRGB < 3; iRGB++) {
		if (StateMap.test(TraceRGBFlag[iRGB]))
			colorSum += 255 - PixelColors[iRGB];
	}
	return colorSum;
}

void trcstpnum() {
	std::wstring fmtStr;
	loadString(fmtStr, IDS_TRCSTP);
	SetWindowText(TraceStepWin, fmt::format(fmtStr, (IniFile.traceLength / PFGRAN)).c_str());
}

void trcratnum() {
	std::wstring fmtStr;
	loadString(fmtStr, IDS_TRCRAT);
	butxt(HLIN, fmt::format(fmtStr, -log10(IniFile.traceRatio - 1)));
}

bool trcin(COLORREF color) {
	if (color) {
		trcols(color);
		if (StateMap.test(StateFlag::TRCRED)) {
			if (PixelColors[0] > HighColors[0])
				return 0;
			if (PixelColors[0] < LowColors[0])
				return 0;
		}
		if (StateMap.test(StateFlag::TRCGRN)) {
			if (PixelColors[1] > HighColors[1])
				return 0;
			if (PixelColors[1] < LowColors[1])
				return 0;
		}
		if (StateMap.test(StateFlag::TRCBLU)) {
			if (PixelColors[2] > HighColors[2])
				return 0;
			if (PixelColors[2] < LowColors[2])
				return 0;
		}
		return 1;
	}
	return 0;
}

void getrmap() {
	BITMAPINFO       info   = {};
	BITMAPINFOHEADER header = {};

	header.biSize        = sizeof(header);
	header.biWidth       = BitmapWidth;
	header.biHeight      = BitmapHeight;
	header.biPlanes      = 1;
	header.biBitCount    = 32;
	header.biCompression = BI_RGB;
	info.bmiHeader       = header;
	TraceBitmap          = getBitmap(BitmapDC, &info, &TraceBitmapData);
	TraceDC              = CreateCompatibleDC(StitchWindowDC);
	if (TraceBitmap && TraceDC) {
		SelectObject(TraceDC, TraceBitmap);
		BitBlt(TraceDC, 0, 0, BitmapWidth, BitmapHeight, BitmapDC, 0, 0, SRCCOPY);
		StateMap.set(StateFlag::WASTRAC);
		TraceDataSize = (BitmapWidth + 1) * (BitmapHeight + 1);
		TracedMap->resize(TraceDataSize);
		TracedMap->reset();
		StretchBlt(StitchWindowMemDC,
		           BitmapDstRect.left,
		           BitmapDstRect.top,
		           BitmapDstRect.right - BitmapDstRect.left,
		           BitmapDstRect.bottom - BitmapDstRect.top,
		           BitmapDC,
		           BitmapSrcRect.left,
		           BitmapSrcRect.top,
		           BitmapSrcRect.right - BitmapSrcRect.left,
		           BitmapSrcRect.bottom - BitmapSrcRect.top,
		           SRCCOPY);
	}
}

void shownd(HWND hwnd) noexcept {
	ShowWindow(hwnd, SW_SHOW);
}

void hidwnd(HWND hwnd) noexcept {
	ShowWindow(hwnd, SW_HIDE);
}

void tracwnd() {
	unsigned iColor = 0, iTrace = 0;

	for (iColor = 0; iColor < 16; iColor++) {
		hidwnd(DefaultColorWin[iColor]);
		hidwnd(UserColorWin[iColor]);
		hidwnd(ThreadSizeWin[iColor]);
	}
	for (iTrace = 0; iTrace < 3; iTrace++) {
		shownd(TraceControlWindow[iTrace]);
		shownd(TraceSelectWindow[iTrace]);
		shownd(TraceUpWindow[iTrace]);
		shownd(TraceDownWindow[iTrace]);
	}
	hidwnd((*ButtonWin)[HBOXSEL]);
	hidwnd((*ButtonWin)[HUPTO]);
	shownd(TraceStepWin);
	trcstpnum();
	trcratnum();
	clrhbut(4);
}

void blanklin(std::vector<unsigned>& differenceBitmap, unsigned lineStart) {
	unsigned iPoint;

	for (iPoint = lineStart; iPoint < lineStart + BitmapWidth; iPoint++)
		differenceBitmap[iPoint] = 0;
}

// Check Translation
static inline void difsub(const unsigned source, unsigned shift, unsigned& destination) noexcept {
	destination = (source >> (shift & 0x0f)) & 0xff;
}

void difbits(unsigned shift, unsigned* point) noexcept {
	unsigned* testPoint = point;

	if (testPoint) {
		difsub(*testPoint, shift, TraceAdjacentColors[0]);

		testPoint -= BitmapWidth;
		difsub(*testPoint, shift, TraceAdjacentColors[1]);

		testPoint -= 1;
		difsub(*testPoint, shift, TraceAdjacentColors[2]);

		testPoint += 2;
		difsub(*testPoint, shift, TraceAdjacentColors[3]);

		testPoint += BitmapWidth;
		difsub(*testPoint, shift, TraceAdjacentColors[4]);

		testPoint -= 2;
		difsub(*testPoint, shift, TraceAdjacentColors[5]);

		testPoint += BitmapWidth;
		difsub(*testPoint, shift, TraceAdjacentColors[6]);

		testPoint += 1;
		difsub(*testPoint, shift, TraceAdjacentColors[7]);

		testPoint += 1;
		difsub(*testPoint, shift, TraceAdjacentColors[8]);
	}
}

constexpr unsigned trsum() {
	unsigned sumAdjacent = 0, iAdjacent = 1;

	for (iAdjacent = 1; iAdjacent < 9; iAdjacent++)
		sumAdjacent += ((TraceAdjacentColors[iAdjacent] > TraceAdjacentColors[0])
		                    ? (TraceAdjacentColors[iAdjacent] - TraceAdjacentColors[0])
		                    : (TraceAdjacentColors[0] - TraceAdjacentColors[iAdjacent]));
	return sumAdjacent;
}

void untrace() {
	unsigned iColor = 0, iTrace = 0, iButton = 0;

	if (StateMap.testAndReset(StateFlag::WASTRAC)) {
		DeleteObject(TraceBitmap);
		DeleteObject(TraceDC);
		if (TracedEdges->size() != 0) {
			TracedEdges->resize(0); // allocated in tracedg
		}
		if (TracedMap->size() != 0) {
			TracedMap->resize(0); // allocated in trace
		}
		StateMap.reset(StateFlag::WASEDG);
		for (iColor = 0; iColor < 16; iColor++) {
			shownd(DefaultColorWin[iColor]);
			shownd(UserColorWin[iColor]);
			shownd(ThreadSizeWin[iColor]);
		}
		for (iTrace = 0; iTrace < 3; iTrace++) {
			hidwnd(TraceControlWindow[iTrace]);
			hidwnd(TraceSelectWindow[iTrace]);
			hidwnd(TraceUpWindow[iTrace]);
			hidwnd(TraceDownWindow[iTrace]);
		}
		for (iButton = 0; iButton < 9; iButton++)
			shownd((*ButtonWin)[iButton]);
		hidwnd(TraceStepWin);
	}
	else {
		if (StateMap.test(StateFlag::TRCUP))
			DownPixelColor = 0xffffff;
		else
			UpPixelColor = 0;
	}
}

void trdif() {
	unsigned iHeight = 0, iPixel = 0, iRGB = 0, iWidth = 0, adjustedColorSum = 0;
	unsigned iPoint = 0, colorSum = 0, colorSumMaximum = 0, colorSumMinimum = 0;
	double   ratio = 0.0;

	if (!PCSBMPFileName[0]) {
		tabmsg(IDS_MAPLOD);
		return;
	}
	StateMap.reset(StateFlag::TRSET);
	StateMap.reset(StateFlag::HIDMAP);
	untrace();
	if (BitmapHeight * BitmapWidth) {
		std::vector<unsigned> differenceBitmap(BitmapHeight * BitmapWidth);
		colorSumMaximum = 0;
		colorSumMinimum = 0xffffffff;
		if (!StateMap.test(StateFlag::WASTRAC))
			getrmap();
		for (iRGB = 0; iRGB < 3; iRGB++) {
			blanklin(differenceBitmap, 0);
			for (iHeight = 1; iHeight < BitmapHeight - 1; iHeight++) {
				iPoint                     = iHeight * BitmapWidth;
				differenceBitmap[iPoint++] = 0;
				for (iWidth = 1; iWidth < BitmapWidth - 1; iWidth++) {
					difbits(TraceShift[iRGB], &TraceBitmapData[iPoint]);
					colorSum = differenceBitmap[iPoint] = trsum();
					iPoint++;
					if (colorSum > colorSumMaximum)
						colorSumMaximum = colorSum;
					if (colorSum < colorSumMinimum)
						colorSumMinimum = colorSum;
				}
				differenceBitmap[iPoint++] = 0;
			}
			blanklin(differenceBitmap, iPoint);
			ratio = static_cast<double>(255) / (colorSumMaximum - colorSumMinimum);
			for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
				TraceBitmapData[iPixel] &= TraceRGBMask[iRGB];
				if (differenceBitmap[iPixel]) {
					adjustedColorSum = (differenceBitmap[iPixel] - colorSumMinimum) * ratio;
					TraceBitmapData[iPixel] |= adjustedColorSum << TraceShift[iRGB];
				}
			}
		}
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
		StateMap.set(StateFlag::WASDIF);
		StateMap.set(StateFlag::RESTCH);
		tracwnd();
	}
}

void trace() {
	unsigned color = 0, iPixel = 0, iRGB = 0;
	unsigned traceColorMask = 0;

#if TRCMTH == 0

	unsigned usum, psum, dsum;
#endif

	if (PCSBMPFileName[0]) {
		untrace();
		tracwnd();
		getrmap();
		if (px2stch() && !StateMap.testAndReset(StateFlag::WASTRCOL)) {
			if (StateMap.test(StateFlag::LANDSCAP))
				SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
			BitmapPoint.x = BmpStitchRatio.x * SelectedPoint.x;
			BitmapPoint.y = BmpStitchRatio.y * SelectedPoint.y - 1;
			color         = TraceBitmapData[BitmapPoint.y * BitmapWidth + BitmapPoint.x] ^ 0xffffff;
			if (StateMap.test(StateFlag::TRCUP)) {
				UpPixelColor   = color;
				DownPixelColor = 0xffffff;
			}
			else {
				DownPixelColor = color;
				UpPixelColor   = 0;
			}
			StateMap.set(StateFlag::TRCRED);
			StateMap.set(StateFlag::TRCGRN);
			StateMap.set(StateFlag::TRCBLU);
		}
		traceColorMask = 0xffffff;
		if (!StateMap.test(StateFlag::TRCRED))
			traceColorMask &= REDMSK;
		if (!StateMap.test(StateFlag::TRCGRN))
			traceColorMask &= GRNMSK;
		if (!StateMap.test(StateFlag::TRCBLU))
			traceColorMask &= BLUMSK;
		if (traceColorMask != 0xffffff) {
			for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++)
				TraceBitmapData[iPixel] &= traceColorMask;
		}

#if TRCMTH == 0

		usum = icolsum(UpPixelColor);
		dsum = icolsum(DownPixelColor);
		if (TracedMap == nullptr) {
			TracedMap = new unsigned[TraceDataSize]();
		}
		for (index = 0; index < BitmapWidth * BitmapHeight; index++) {
			psum = colsum(TraceBitmapData[index]);
			if (usum > psum && dsum < psum)
				setrac(index);
			else
				TraceBitmapData[index] = 0;
		}
#endif

#if TRCMTH == 1
		InvertUpColor   = UpPixelColor ^ 0xffffff;
		InvertDownColor = DownPixelColor ^ 0xffffff;
		trcols(InvertUpColor);
		for (iRGB = 0; iRGB < 3; iRGB++)
			HighColors[iRGB] = PixelColors[iRGB];
		trcols(InvertDownColor);
		for (iRGB = 0; iRGB < 3; iRGB++)
			LowColors[iRGB] = PixelColors[iRGB];
		if (TracedMap->size() == 0) {
			TracedMap->resize(TraceDataSize, false);
		}
		for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
			if (trcin(TraceBitmapData[iPixel]))
				TracedMap->set(iPixel);
			else
				TraceBitmapData[iPixel] = 0;
		}
#endif
		StateMap.set(StateFlag::TRSET);
		StateMap.set(StateFlag::RESTCH);
	}
	else
		tabmsg(IDS_MAPLOD);
}

void tracedg() {
	// ToDo - should flag be bool?
	unsigned iHeight = 0, iWidth = 0, iPixel = 0, flag = 0;
	long     pixelIndex = 0;

	if (!StateMap.test(StateFlag::WASTRAC))
		trace();
	TracedEdges->resize(TraceDataSize, false);
	TracedEdges->reset();
	pixelIndex = 0;
	for (iHeight = 0; iHeight < BitmapHeight; iHeight++) {
		flag = 0;
		for (iWidth = 0; iWidth < BitmapWidth; iWidth++) {
			if (TracedMap->test(pixelIndex)) {
				if (!flag) {
					TracedEdges->set(pixelIndex);
					flag = 1;
				}
			}
			else {
				if (flag) {
					TracedEdges->set(pixelIndex - 1);
					flag = 0;
				}
			}
			pixelIndex++;
		}
		if (flag)
			TracedEdges->set(pixelIndex - 1);
	}
	for (iWidth = 0; iWidth < BitmapWidth; iWidth++) {
		pixelIndex = iWidth;
		flag       = 0;
		for (iHeight = 0; iHeight < BitmapHeight; iHeight++) {
			if (TracedMap->test(pixelIndex)) {
				if (!flag) {
					TracedEdges->set(pixelIndex);
					flag = 1;
				}
			}
			else {
				if (flag) {
					TracedEdges->set(pixelIndex - BitmapWidth);
					flag = 0;
				}
			}
			pixelIndex += BitmapWidth;
		}
		if (flag)
			TracedEdges->set(pixelIndex - BitmapWidth);
	}
	for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
		if (TracedEdges->test(iPixel))
			TraceBitmapData[iPixel] = 0xffffff;
		else
			TraceBitmapData[iPixel] = 0;
	}
	StateMap.set(StateFlag::RESTCH);
	StateMap.set(StateFlag::WASEDG);
}

bool trcbit(const unsigned initialDirection, unsigned& traceDirection, std::vector<TRCPNT>& tracedPoints) {
	unsigned pixelIndex;

	pixelIndex = CurrentTracePoint.y * BitmapWidth + CurrentTracePoint.x;
	switch (traceDirection) {
	case TRCR:
		pixelIndex += (1 - BitmapWidth);
		if (CurrentTracePoint.x == gsl::narrow<int>(BitmapWidth) - 1)
			traceDirection = TRCU;
		else {
			if (TracedEdges->test(pixelIndex)) {
				CurrentTracePoint.x++;
				CurrentTracePoint.y--;
				traceDirection = TRCD;
			}
			else {
				pixelIndex += BitmapWidth;
				if (TracedEdges->test(pixelIndex))
					CurrentTracePoint.x++;
				else
					traceDirection = TRCU;
			}
		}
		break;
	case TRCD:
		pixelIndex -= (BitmapWidth + 1);
		if (!CurrentTracePoint.y)
			traceDirection = TRCR;
		else {
			if (TracedEdges->test(pixelIndex)) {
				CurrentTracePoint.x--;
				CurrentTracePoint.y--;
				traceDirection = TRCL;
			}
			else {
				pixelIndex++;
				if (TracedEdges->test(pixelIndex))
					CurrentTracePoint.y--;
				else
					traceDirection = TRCR;
			}
		}
		break;
	case TRCL:
		pixelIndex += (BitmapWidth - 1);
		if (!CurrentTracePoint.x)
			traceDirection = TRCD;
		else {
			if (TracedEdges->test(pixelIndex)) {
				CurrentTracePoint.x--;
				CurrentTracePoint.y++;
				traceDirection = TRCU;
			}
			else {
				pixelIndex -= BitmapWidth;
				if (TracedEdges->test(pixelIndex))
					CurrentTracePoint.x--;
				else
					traceDirection = TRCD;
			}
		}
		break;
	case TRCU:
		pixelIndex += (1 + BitmapWidth);
		if (CurrentTracePoint.y == gsl::narrow<int>(BitmapHeight) - 1)
			traceDirection = TRCL;
		else {
			if (TracedEdges->test(pixelIndex)) {
				CurrentTracePoint.x++;
				CurrentTracePoint.y++;
				traceDirection = TRCR;
			}
			else {
				pixelIndex--;
				if (TracedEdges->test(pixelIndex))
					CurrentTracePoint.y++;
				else
					traceDirection = TRCL;
			}
		}
		break;
	}
	if (tracedPoints.back().x != CurrentTracePoint.x || tracedPoints.back().y != CurrentTracePoint.y) {
		tracedPoints.push_back({ gsl::narrow<short>(CurrentTracePoint.x), gsl::narrow<short>(CurrentTracePoint.y) });
		if (tracedPoints.size() >= 500000)
			return 0;
	}
	if (traceDirection == initialDirection && CurrentTracePoint.x == tracedPoints[0].x
	    && CurrentTracePoint.y == tracedPoints[0].y)
		return 0;
	else
		return 1;
}

void dutdif(TRCPNT& traceDiff, const TRCPNT* point) noexcept {
	if (point) {
		traceDiff.x = point[1].x - point[0].x;
		// ToDo - this is likely incorrect
		traceDiff.y = point[1].y - point[0].y;
	}
}

void dutrac() {
	unsigned flag          = 0;
	RECT     findRectangle = {};
	long     point = 0, limit = 0, savedPoint = 0;
	long     edgeDistance = 0, minimumEdgeDistance = 0;
	unsigned iPoint = 0, iCurrent = 0, iNext = 0;
	double   traceLength     = 0.0;
	double   traceLengthSum  = 0.0;
	float    landscapeOffset = 0.0;
	unsigned traceDirection  = 0;

	if (px2stch()) {
		if (!StateMap.test(StateFlag::WASEDG)) {
			tracedg();
			return;
		}
		savdo();
		if (StateMap.test(StateFlag::LANDSCAP))
			SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
		CurrentTracePoint.x = BmpStitchRatio.x * SelectedPoint.x;
		CurrentTracePoint.y = BmpStitchRatio.y * SelectedPoint.y;
		if (CurrentTracePoint.x > gsl::narrow<long>(BitmapWidth))
			CurrentTracePoint.x = BitmapWidth;
		if (CurrentTracePoint.y > gsl::narrow<long>(BitmapHeight))
			CurrentTracePoint.y = BitmapHeight;
		savedPoint = point = CurrentTracePoint.y * BitmapWidth + CurrentTracePoint.x;
		if (!TracedEdges->test(point)) {
			flag  = 20;
			point = savedPoint;
			limit = (CurrentTracePoint.y + 1) * BitmapWidth;
			while (point < limit && !TracedEdges->test(point))
				point++;
			if (point < limit)
				findRectangle.right = point - CurrentTracePoint.y * BitmapWidth;
			else
				findRectangle.right = BitmapWidth;
			point = savedPoint;
			limit = CurrentTracePoint.y * BitmapWidth;
			while (point > limit && !TracedEdges->test(point))
				point--;
			if (point == limit)
				findRectangle.left = 0;
			else
				findRectangle.left = point - limit;
			point = savedPoint;
			while (point > 0 && !TracedEdges->test(point))
				point -= BitmapWidth;
			if (point > 0)
				findRectangle.bottom = point / BitmapWidth;
			else
				findRectangle.bottom = 0;
			point = savedPoint;
			limit = BitmapWidth * BitmapHeight;
			while (point < limit && !TracedEdges->test(point))
				point += BitmapWidth;
			if (point < limit)
				findRectangle.top = point / BitmapWidth;
			else
				findRectangle.top = BitmapHeight;
			flag                = 0;
			minimumEdgeDistance = 0x7fffffff;
			if (findRectangle.left) {
				minimumEdgeDistance = CurrentTracePoint.x - findRectangle.left;
				flag                = TRCL;
			}
			if (findRectangle.right < gsl::narrow<long>(BitmapWidth)) {
				edgeDistance = findRectangle.right - CurrentTracePoint.x;
				if (edgeDistance < minimumEdgeDistance) {
					minimumEdgeDistance = edgeDistance;
					flag                = TRCR;
				}
			}
			if (findRectangle.bottom) {
				edgeDistance = CurrentTracePoint.y - findRectangle.bottom;
				if (edgeDistance < minimumEdgeDistance) {
					minimumEdgeDistance = edgeDistance;
					flag                = TRCD;
				}
			}
			if (findRectangle.top < gsl::narrow<long>(BitmapHeight)) {
				edgeDistance = findRectangle.top - CurrentTracePoint.y;
				if (edgeDistance < minimumEdgeDistance) {
					minimumEdgeDistance = edgeDistance;
					flag                = TRCU;
				}
			}
			switch (flag) {
			case TRCU:
				CurrentTracePoint.y = findRectangle.top;
				traceDirection      = TRCR;
				break;
			case TRCR:
				CurrentTracePoint.x = findRectangle.right;
				traceDirection      = TRCD;
				break;
			case TRCD:
				CurrentTracePoint.y = findRectangle.bottom;
				traceDirection      = TRCL;
				break;
			case TRCL:
				CurrentTracePoint.x = findRectangle.left;
				traceDirection      = TRCU;
				break;

			default:
				return;
			}
		}
		const unsigned initialDirection = traceDirection;
		point                           = CurrentTracePoint.y * BitmapWidth + CurrentTracePoint.x;
		std::vector<TRCPNT> tracedPoints;
		tracedPoints.push_back({ gsl::narrow<short>(CurrentTracePoint.x), gsl::narrow<short>(CurrentTracePoint.y) });
		while (trcbit(initialDirection, traceDirection, tracedPoints))
			;
		if (tracedPoints.size() >= 500000) {
			tabmsg(IDS_FRM2L);
			return;
		}
		std::vector<TRCPNT> decimatedLine;
		decimatedLine.reserve(tracedPoints.size());
		TRCPNT traceDiff[2] = {};
		decimatedLine.push_back(tracedPoints[0]);
		dutdif(traceDiff[0], tracedPoints.data());
		OutputIndex = 1;
		for (iPoint = 1; iPoint < tracedPoints.size(); iPoint++) {
			traceDiff[1] = traceDiff[0];
			dutdif(traceDiff[0], &tracedPoints[iPoint]);
			if (traceDiff[1].x != traceDiff[0].x || traceDiff[1].y != traceDiff[0].y) {
				decimatedLine.push_back(tracedPoints[iPoint]);
			}
		}
		tracedPoints.clear();
		tracedPoints.push_back(decimatedLine[0]);
		iNext = 0;
		for (iCurrent = 1; iCurrent < decimatedLine.size(); iCurrent++) {
			traceLength
			    = hypot(decimatedLine[iCurrent].x - decimatedLine[iNext].x, decimatedLine[iCurrent].y - decimatedLine[iNext].y);
			if (traceLength > IniFile.traceLength) {
				tracedPoints.push_back(decimatedLine[iNext]);
				iNext = iCurrent;
			}
		}
		for (iCurrent = iNext + 1; iCurrent < decimatedLine.size(); iCurrent++) {
			tracedPoints.push_back(decimatedLine[iCurrent]);
		}
		SelectedForm = &FormList[FormIndex];
		frmclr(SelectedForm);
		CurrentFormVertices      = &FormVertices[FormVertexIndex];
		CurrentFormVertices[0].x = tracedPoints[0].x * StitchBmpRatio.x;
		CurrentFormVertices[0].y = tracedPoints[0].y * StitchBmpRatio.y;
		iNext                    = 0;
		OutputIndex              = 0;
		traceLengthSum           = 0;
		if (StateMap.test(StateFlag::LANDSCAP))
			landscapeOffset = UnzoomedRect.y - BitmapSizeinStitches.y;
		else
			landscapeOffset = 0;
		for (iCurrent = 1; iCurrent < tracedPoints.size(); iCurrent++) {
			traceLengthSum += hypot(tracedPoints[iCurrent].x - tracedPoints[iCurrent - 1].x,
			                        tracedPoints[iCurrent].y - tracedPoints[iCurrent - 1].y);
			traceLength
			    = hypot(tracedPoints[iCurrent].x - tracedPoints[iNext].x, tracedPoints[iCurrent].y - tracedPoints[iNext].y);
			if (traceLengthSum > traceLength * IniFile.traceRatio) {
				CurrentFormVertices[OutputIndex].x = tracedPoints[iCurrent - 1].x * StitchBmpRatio.x;
				CurrentFormVertices[OutputIndex].y = tracedPoints[iCurrent - 1].y * StitchBmpRatio.y + landscapeOffset;
				OutputIndex++;
				iCurrent--;
				iNext          = iCurrent;
				traceLengthSum = 0;
			}
		}
		if (FormVertexIndex + OutputIndex > MAXITEMS) {
			tabmsg(IDS_FRMOVR);
			return;
		}
		SelectedForm->vertices    = adflt(OutputIndex);
		SelectedForm->vertexCount = gsl::narrow<unsigned short>(OutputIndex);
		SelectedForm->type        = FRMFPOLY;
		SelectedForm->attribute   = ActiveLayer << 1;
		frmout(FormIndex);
		FormList[FormIndex].satinGuideCount = 0;
		FormIndex++;
		StateMap.set(StateFlag::RESTCH);
		StateMap.set(StateFlag::FRMOF);
		tglfrm();
	}
}

void trinit() {
	unsigned iPixel = 0, iRGB = 0, swapComponent = 0, iLevel = 0;
	unsigned histogramData[3][256] = { 0 };
	unsigned componentPeakCount[3] = { 0 };
	unsigned componentPeak[3]      = { 0 };
	COLORREF color                 = {};

	if (PCSBMPFileName[0]) {
		if (!StateMap.test(StateFlag::TRSET)) {
			StateMap.set(StateFlag::TRCRED);
			StateMap.set(StateFlag::TRCGRN);
			StateMap.set(StateFlag::TRCBLU);
			if (!StateMap.test(StateFlag::WASTRAC))
				getrmap();
			if (StateMap.test(StateFlag::MONOMAP)) {
				// ToDo - Is this intializing correctly?
				color = TraceBitmapData[0];
				for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
					if (TraceBitmapData[iPixel] != color)
						break;
				}
				if (iPixel < BitmapWidth * BitmapHeight) {
					trcols(TraceBitmapData[iPixel]);
					HighColors[0] = PixelColors[0];
					HighColors[1] = PixelColors[1];
					HighColors[2] = PixelColors[2];
					trcols(color);
					for (iRGB = 0; iRGB < 3; iRGB++) {
						if (PixelColors[iRGB] > HighColors[iRGB]) {
							swapComponent     = PixelColors[iRGB];
							PixelColors[iRGB] = HighColors[iRGB];
							HighColors[iRGB]  = swapComponent;
						}
						componentPeak[iRGB] = ((HighColors[iRGB] - PixelColors[iRGB]) >> 1) + PixelColors[iRGB];
					}
				}
			}
			else {
				for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
					trcols(TraceBitmapData[iPixel]);
					for (iRGB = 0; iRGB < 3; iRGB++)
						histogramData[iRGB][PixelColors[iRGB]]++;
				}
				for (iRGB = 0; iRGB < 3; iRGB++)
					componentPeakCount[iRGB] = 0;
				for (iLevel = 0; iLevel < 256; iLevel++) {
					for (iRGB = 0; iRGB < 3; iRGB++) {
						if (histogramData[iRGB][iLevel] > componentPeakCount[iRGB]) {
							componentPeakCount[iRGB] = histogramData[iRGB][iLevel];
							componentPeak[iRGB]      = iLevel;
						}
					}
				}
			}
			InvertDownColor = 0;
			for (iRGB = 0; iRGB < 3; iRGB++) {
				if (componentPeak[iRGB])
					componentPeak[iRGB]--;
				InvertDownColor |= componentPeak[iRGB] << TraceShift[iRGB];
			}
			DownPixelColor = InvertDownColor ^ 0xffffff;
			InvertUpColor  = 0xffffff;
			UpPixelColor   = 0;
		}
		StateMap.set(StateFlag::WASTRCOL);
		trace();
	}
	else
		tabmsg(IDS_MAPLOD);
}

void trcsel() {
	unsigned iPixel = 0, maximumColorComponent = 0, iRGB = 0;

	if (PCSBMPFileName[0]) {
		StateMap.set(StateFlag::WASTRCOL);
		StateMap.set(StateFlag::TRCRED);
		StateMap.set(StateFlag::TRCBLU);
		StateMap.set(StateFlag::TRCGRN);
		DownPixelColor = 0xffffff;
		UpPixelColor   = 0;
		trace();
		StateMap.reset(StateFlag::HIDMAP);
		StateMap.reset(StateFlag::TRSET);
		for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
			trcols(TraceBitmapData[iPixel]);
			maximumColorComponent = PixelColors[0];
			iRGB                  = 2;
			if (PixelColors[1] > maximumColorComponent) {
				maximumColorComponent = PixelColors[1];
				iRGB                  = 1;
			}
			if (PixelColors[2] > maximumColorComponent)
				iRGB = 0;
			TraceBitmapData[iPixel] &= TraceRGB[iRGB];
		}
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
		StateMap.set(StateFlag::WASDSEL);
		StateMap.set(StateFlag::RESTCH);
		tracwnd();
	}
	else
		tabmsg(IDS_MAPLOD);
}

void ritrcol(COLORREF* color, unsigned number) noexcept {
	if (color) {
		*color &= TraceRGBMask[ColumnColor];
		number &= 0xff;
		*color |= (number << TraceShift[ColumnColor]);
	}
}

void dutrnum0(unsigned color) {
	StateMap.reset(StateFlag::NUMIN);
	StateMap.reset(StateFlag::TRNIN0);
	if (StateMap.test(StateFlag::TRNUP)) {
		ritrcol(&InvertUpColor, color);
		UpPixelColor = InvertUpColor ^ 0xffffff;
		redraw(TraceUpWindow[ColumnColor]);
	}
	else {
		ritrcol(&InvertDownColor, color);
		DownPixelColor = InvertDownColor ^ 0xffffff;
		redraw(TraceDownWindow[ColumnColor]);
	}
	redraw(TraceControlWindow[ColumnColor]);
	DestroyWindow(TraceNumberInput);
	StateMap.set(StateFlag::WASTRCOL);
	trace();
}

void dutrnum2(){
	dutrnum0(std::stoi(TraceInputBuffer));
}

void dutrnum1() {
	double traceLength;

	DestroyWindow(GeneralNumberInputBox);
	StateMap.reset(StateFlag::NUMIN);
	StateMap.reset(StateFlag::TRNIN1);
	traceLength = std::stof(MsgBuffer);
	if (traceLength > 9)
		traceLength = 9;
	if (StateMap.test(StateFlag::TRNUP)) {
		IniFile.traceLength = traceLength * PFGRAN;
		trcstpnum();
	}
	else {
		IniFile.traceRatio = 1 + pow(0.1, traceLength);
		trcratnum();
	}
}

unsigned ducolm() {
	if (TraceMsgPoint.x < gsl::narrow<int>(ButtonWidth))
		return 0;
	if (TraceMsgPoint.x < gsl::narrow<int>(ButtonWidth) << 1)
		return 1;
	else
		return 2;
}

void trnumwnd0(int position) noexcept {
	TraceNumberInput = CreateWindow(L"STATIC",
	                                0,
	                                SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                ButtonWidthX3,
	                                position,
	                                ButtonWidth,
	                                ButtonHeight,
	                                ThrEdWindow,
	                                NULL,
	                                ThrEdInstance,
	                                NULL);
}

void trnumwnd1(int position) noexcept {
	GeneralNumberInputBox = CreateWindow(L"STATIC",
	                                     0,
	                                     WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                     ButtonWidthX3,
	                                     position,
	                                     ButtonWidthX3,
	                                     ButtonHeight,
	                                     ThrEdWindow,
	                                     NULL,
	                                     ThrEdInstance,
	                                     NULL);
}

void stch2bit(fPOINT& point) {
	if (StateMap.test(StateFlag::LANDSCAP))
		point.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
	BitmapPoint.x = BmpStitchRatio.x * point.x;
	BitmapPoint.y = (BitmapHeight - BmpStitchRatio.y * point.y);
}

void pxlin(unsigned start, unsigned finish) {
	POINT line[2];

	stch2bit(CurrentFormVertices[start]);
	line[0] = BitmapPoint;
	stch2bit(CurrentFormVertices[finish]);
	line[1] = BitmapPoint;
	Polyline(BitmapDC, line, 2);
	Polyline(TraceDC, line, 2);
}

void bfrm() {
	unsigned iVertex = 0;

	if (VertexCount) {
		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			pxlin(iVertex, iVertex + 1);
		if (SelectedForm->type != FRMLINE)
			pxlin(iVertex, 0);
	}
}

void blak() {
	unsigned iForm = 0;

	if (!PCSBMPFileName[0]) {
		tabmsg(IDS_MAPLOD);
		return;
	}
	tracwnd();
	if (FormIndex) {
		BlackPen = CreatePen(PS_SOLID, 1, 0);
		SelectObject(BitmapDC, BlackPen);
		SelectObject(TraceDC, BlackPen);
		if (!StateMap.test(StateFlag::WASTRAC))
			getrmap();
		for (iForm = 0; iForm < FormIndex; iForm++) {
			fvars(iForm);
			bfrm();
		}
		DeleteObject(BlackPen);
		StateMap.set(StateFlag::WASBLAK);
		StateMap.set(StateFlag::RESTCH);
	}
	else
		tabmsg(IDS_FRMNO);
}

void tracpar() {

	unsigned position      = 0;
	double   ratio         = 0.0;
	COLORREF traceColor    = {};
	COLORREF tracePosition = {};

	if (StateMap.test(StateFlag::TRNIN0))
		dutrnum2();
	if (StateMap.test(StateFlag::TRNIN1))
		dutrnum1();
	TraceMsgPoint.x = Msg.pt.x - ThredWindowOrigin.x;
	TraceMsgPoint.y = Msg.pt.y - ThredWindowOrigin.y;
	if (TraceMsgPoint.x > gsl::narrow<int>(ButtonWidthX3))
		dutrac();
	else {
		ColumnColor = ducolm();
		if (TraceMsgPoint.y < gsl::narrow<int>(ButtonHeight) * 15) {
			do {
				if (GetKeyState(VK_SHIFT) & 0X8000) {
					UpPixelColor &= TraceRGBMask[ColumnColor];
					DownPixelColor |= TraceRGB[2 - ColumnColor];
					break;
				}
				ratio         = static_cast<float>(TraceMsgPoint.y) / (ButtonHeight * 15);
				position      = ratio * 255;
				traceColor    = UpPixelColor & TraceRGB[2 - ColumnColor];
				tracePosition = position << TraceShift[ColumnColor];
				if (tracePosition < traceColor) {
					UpPixelColor &= TraceRGBMask[ColumnColor];
					UpPixelColor |= tracePosition;
					break;
				}
				traceColor = DownPixelColor & TraceRGB[2 - ColumnColor];
				if (tracePosition > traceColor) {
					DownPixelColor &= TraceRGBMask[ColumnColor];
					DownPixelColor |= tracePosition;
					break;
				}
				if (Msg.message == WM_LBUTTONDOWN) {
					UpPixelColor &= TraceRGBMask[ColumnColor];
					UpPixelColor |= position << TraceShift[ColumnColor];
				}
				else {
					DownPixelColor &= TraceRGBMask[ColumnColor];
					DownPixelColor |= position << TraceShift[ColumnColor];
				}
			} while (false);
			redraw(TraceControlWindow[ColumnColor]);
			trace();
		}
		else {
			position = floor(TraceMsgPoint.y / ButtonHeight);
			if (position < 16) {
				StateMap.flip(TraceRGBFlag[ColumnColor]);
				redraw(TraceSelectWindow[ColumnColor]);
				trace();
			}
			else {
				if (position < 18) {
					StateMap.set(StateFlag::NUMIN);
					StateMap.set(StateFlag::TRNIN0);
					MsgIndex          = 0;
					*TraceInputBuffer = 0;
					if (position < 17) {
						trnumwnd0(ButtonHeight * 16);
						StateMap.set(StateFlag::TRNUP);
					}
					else {
						trnumwnd0(ButtonHeight * 17);
						StateMap.reset(StateFlag::TRNUP);
					}
				}
				else {
					if (position < 20) {
						StateMap.set(StateFlag::NUMIN);
						StateMap.set(StateFlag::TRNIN1);
						MsgIndex          = 0;
						*TraceInputBuffer = 0;
						if (position < 19) {
							trnumwnd1(ButtonHeight * 18);
							StateMap.set(StateFlag::TRNUP);
						}
						else {
							trnumwnd1(ButtonHeight * 19);
							StateMap.reset(StateFlag::TRNUP);
						}
					}
					else {
						switch (position) {
						case 20:
							trdif();
							break;
						case 21:
							hidbit();
							break;
						case 22:
							blak();
							break;
						case 23:
							trcsel();
							break;
						case 24:
							tracedg();
							break;
						}
					}
				}
			}
		}
	}
}

void trcnum(unsigned shift, COLORREF color, unsigned iRGB) {
	const unsigned NumeralWidth = txtWid(L"0");

	size_t   bufferLength = 0;
	unsigned xPosition = 0;
	wchar_t  buffer[11] = { 0 };

	color >>= shift;
	color &= 0xff;
	_itow_s(color, buffer, 10);
	bufferLength = wcslen(buffer);
	xPosition = gsl::narrow<unsigned int>(NumeralWidth * (3 - bufferLength) + 1);
	SetBkColor(DrawItem->hDC, TraceRGB[iRGB]);
	TextOutInt(DrawItem->hDC, xPosition, 1, buffer, wcslen(buffer));
}

void upnum(unsigned iRGB) {
	trcnum(TraceShift[iRGB], InvertUpColor, iRGB);
}

void dwnum(unsigned iRGB) {
	trcnum(TraceShift[iRGB], InvertDownColor, iRGB);
}

void durct(unsigned shift, const RECT& traceControlRect, RECT& traceHighMask, RECT& traceMiddleMask, RECT& traceLowMask) {
	const unsigned lowerColor = (UpPixelColor >> shift) & 0xff;
	const unsigned upperColor = (DownPixelColor >> shift) & 0xff;
	const unsigned controlHeight = traceControlRect.bottom - traceControlRect.top;
	double         ratio = 0.0;

	traceHighMask.left = traceLowMask.left = traceMiddleMask.left = traceControlRect.left;
	traceHighMask.right = traceLowMask.right = traceMiddleMask.right = traceControlRect.right;

	ratio = static_cast<double>(lowerColor) / 255;
	traceMiddleMask.top = controlHeight * ratio + traceControlRect.top;
	ratio = static_cast<double>(upperColor) / 255;
	traceMiddleMask.bottom = controlHeight * ratio + traceControlRect.top;
	StateMap.reset(StateFlag::DUHI);
	StateMap.reset(StateFlag::DULO);
	if (lowerColor) {
		StateMap.set(StateFlag::DULO);
		traceLowMask.bottom = traceMiddleMask.top;
		traceLowMask.top = 0;
	}
	if (upperColor != 255) {
		StateMap.set(StateFlag::DUHI);
		traceHighMask.top = traceMiddleMask.bottom;
		traceHighMask.bottom = traceControlRect.bottom;
	}
}

void dublk(HDC dc, const RECT& traceHighMask, const RECT& traceLowMask, HBRUSH   brush) {
	if (StateMap.test(StateFlag::DUHI))
		FillRect(dc, &traceHighMask, brush);
	if (StateMap.test(StateFlag::DULO))
		FillRect(dc, &traceLowMask, brush);
}

void wasTrace () {
	RECT     traceHighMaskRect = {}; // high trace mask rectangle
	RECT     traceMiddleMaskRect = {}; // middle trace mask rectangle
	RECT     traceLowMaskRect = {}; // low trace mask rectangle
	HBRUSH   BlackBrush = CreateSolidBrush(0);                  // black brush

	for (auto iRGB = 0; iRGB < 3; iRGB++) {
		if (DrawItem->hwndItem == TraceUpWindow[iRGB]) {
			FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[iRGB]);
			upnum(iRGB);
			break;
		}
		if (DrawItem->hwndItem == TraceDownWindow[iRGB]) {
			FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[iRGB]);
			dwnum(iRGB);
		}
		if (DrawItem->hwndItem == TraceControlWindow[iRGB]) {
			durct(TraceShift[iRGB], DrawItem->rcItem, traceHighMaskRect, traceMiddleMaskRect, traceLowMaskRect);
			FillRect(DrawItem->hDC, &traceMiddleMaskRect, TraceBrush[iRGB]);
			dublk(DrawItem->hDC, traceHighMaskRect, traceLowMaskRect, BlackBrush);
			break;
		}
		if (DrawItem->hwndItem == TraceSelectWindow[iRGB]) {
			auto TempBrush = BlackBrush;
			wchar_t  buffer[10] = { 0 }; // for integer to string conversion
			wcscpy_s(buffer, (*StringTable)[STR_OFF].c_str());
			SetBkColor(DrawItem->hDC, 0);
			SetTextColor(DrawItem->hDC, TraceRGB[iRGB]);
			if (StateMap.test(TraceRGBFlag[iRGB])) {
				TempBrush = TraceBrush[iRGB];
				wcscpy_s(buffer, (*StringTable)[STR_ON].c_str());
				SetTextColor(DrawItem->hDC, 0);
				SetBkColor(DrawItem->hDC, TraceRGB[iRGB]);
			}
			FillRect(DrawItem->hDC, &DrawItem->rcItem, TempBrush);
			TextOutInt(DrawItem->hDC, 1, 1, buffer, wcslen(buffer));
			break;
		}
		if (DrawItem->hwndItem == TraceNumberInput) {
			FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[ColumnColor]);
			SetBkColor(DrawItem->hDC, TraceRGB[ColumnColor]);
			TextOutInt(DrawItem->hDC, 1, 1, TraceInputBuffer, wcslen(TraceInputBuffer));
			break;
		}
	}
}

void wasTrace1() {
	if (StateMap.test(StateFlag::TRNIN0))
		dutrnum2();
	if (StateMap.test(StateFlag::TRNIN1))
		dutrnum1();
	if (!StateMap.test(StateFlag::WASEDG))
		tracpar();
}

void traceNumberInput(unsigned NumericCode) {
	TraceInputBuffer[MsgIndex++] = NumericCode;
	TraceInputBuffer[MsgIndex] = 0;
	auto traceColor = std::stoi(TraceInputBuffer);
	switch (MsgIndex) {
	case 2:
		if (traceColor > 25)
			dutrnum0(traceColor);
		break;
	case 3:
		if (traceColor > 255)
			traceColor = 255;
		dutrnum0(traceColor);
		break;
	}
	redraw(TraceNumberInput);
}

void traceNumberReset () {
	TraceInputBuffer[MsgIndex] = 0;
	redraw(TraceNumberInput);
}