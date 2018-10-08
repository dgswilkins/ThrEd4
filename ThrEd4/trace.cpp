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
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "form.h"
#include "thred.h"
#include "trace.h"

namespace ti = trace::internal;

unsigned  PixelColors[3];        // separated pixel reference colors
HBITMAP   TraceBitmap;           // trace bitmap
HWND      TraceControlWindow[3]; // trace control windows
HWND      TraceDownWindow[3];    // trace down number windows
HWND      TraceSelectWindow[3];  // trace select windows
HWND      TraceUpWindow[3];      // trace up number windows
POINT     CurrentTracePoint;     // current point being traced
unsigned  TraceDataSize;         // size of the trace bitmap in double words
unsigned* TracedPixels;          // bitmap of selected trace pixels
HWND      TraceStepWin;          // trace stepSize window
StateFlag TraceRGBFlag[] = { StateFlag::TRCRED, StateFlag::TRCGRN, StateFlag::TRCBLU }; // trace bits
unsigned  TraceRGBMask[] = { REDMSK, GRNMSK, BLUMSK };                                  // trace masks
unsigned  TraceRGB[]     = { BLUCOL, GRNCOL, REDCOL };                                  // trace colors
unsigned  TraceAdjacentColors[9];                                                       // separated colors for adjacent pixels
wchar_t   TraceInputBuffer[4];                                                          // for user input color numbers
COLORREF  UpPixelColor;                                                                 // color of the up reference pixel
COLORREF  DownPixelColor;                                                               // color of the down reference pixel
COLORREF  InvertUpColor;               // complement color of the up reference pixel
COLORREF  InvertDownColor;             // complement color of the down reference pixel
POINT     TraceMsgPoint;               // message point for trace parsing
unsigned  HighColors[3];               // separated upper reference colors
unsigned  LowColors[3];                // separated lower reference colors
unsigned  ColumnColor;                 // trace color column
unsigned  TraceShift[] = { 0, 8, 16 }; // trace shift values
HBRUSH    TraceBrush[3];               // red,green,and blue brushes
HWND      TraceNumberInput;            // trace number input window
HPEN      BlackPen;                    // black pen

void trace::initColorRef() noexcept {
	UpPixelColor    = 0;
	DownPixelColor  = 0x7f7f7f;
	InvertUpColor   = 0xffffff;
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
		                       nullptr,
		                       ThrEdInstance,
		                       nullptr);
	}
}

void trace::initTraceWindows() noexcept {
	TraceStepWin = CreateWindow(L"STATIC",
	                            L"",
	                            SS_NOTIFY | SS_CENTER | WS_CHILD | WS_BORDER,
	                            0,
	                            ButtonHeight * 18,
	                            ButtonWidthX3,
	                            ButtonHeight,
	                            ThrEdWindow,
	                            nullptr,
	                            ThrEdInstance,
	                            nullptr);
	for (auto iRGB = 0; iRGB < 3; iRGB++) {
		trcsub(&TraceControlWindow[iRGB], ButtonWidth * iRGB, 0, ButtonHeight * 15);
		trcsub(&TraceSelectWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 15, ButtonHeight);
		trcsub(&TraceUpWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 16, ButtonHeight);
		trcsub(&TraceDownWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 17, ButtonHeight);
		TraceBrush[iRGB] = CreateSolidBrush(TraceRGB[iRGB]);
	}
}

void trace::internal::trcols(COLORREF color) noexcept {
	PixelColors[0] = color & 0xff;
	PixelColors[1] = (color & 0xff00) >> 8;
	PixelColors[2] = (color & 0xff0000) >> 16;
}

void trace::internal::trcstpnum() {
	std::wstring fmtStr;
	displayText::loadString(fmtStr, IDS_TRCSTP);
	SetWindowText(TraceStepWin, fmt::format(fmtStr, (IniFile.traceLength / PFGRAN)).c_str());
}

void trace::internal::trcratnum() {
	std::wstring fmtStr;
	displayText::loadString(fmtStr, IDS_TRCRAT);
	displayText::butxt(HLIN, fmt::format(fmtStr, -log10(IniFile.traceRatio - 1)));
}

bool trace::internal::trcin(COLORREF color) {
	if (color) {
		ti::trcols(color);
		if (StateMap.test(StateFlag::TRCRED)) {
			if (PixelColors[0] > HighColors[0]) {
				return false;
			}
			if (PixelColors[0] < LowColors[0]) {
				return false;
			}
		}
		if (StateMap.test(StateFlag::TRCGRN)) {
			if (PixelColors[1] > HighColors[1]) {
				return false;
			}
			if (PixelColors[1] < LowColors[1]) {
				return false;
			}
		}
		if (StateMap.test(StateFlag::TRCBLU)) {
			if (PixelColors[2] > HighColors[2]) {
				return false;
			}
			if (PixelColors[2] < LowColors[2]) {
				return false;
			}
		}
		return true;
	}
	return false;
}

void trace::internal::getrmap() {
	BITMAPINFO       info   = {};
	BITMAPINFOHEADER header = {};

	header.biSize        = sizeof(header);
	header.biWidth       = BitmapWidth;
	header.biHeight      = BitmapHeight;
	header.biPlanes      = 1;
	header.biBitCount    = 32;
	header.biCompression = BI_RGB;
	info.bmiHeader       = header;
	TraceBitmap          = thred::getBitmap(BitmapDC, &info, &TraceBitmapData);
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

void trace::internal::shownd(HWND hwnd) noexcept {
	ShowWindow(hwnd, SW_SHOW);
}

void trace::internal::hidwnd(HWND hwnd) noexcept {
	ShowWindow(hwnd, SW_HIDE);
}

void trace::internal::tracwnd() {
	for (auto iColor = 0u; iColor < 16; iColor++) {
		ti::hidwnd(DefaultColorWin[iColor]);
		ti::hidwnd(UserColorWin[iColor]);
		ti::hidwnd(ThreadSizeWin[iColor]);
	}
	for (auto iTrace = 0u; iTrace < 3; iTrace++) {
		ti::shownd(TraceControlWindow[iTrace]);
		ti::shownd(TraceSelectWindow[iTrace]);
		ti::shownd(TraceUpWindow[iTrace]);
		ti::shownd(TraceDownWindow[iTrace]);
	}
	ti::hidwnd((*ButtonWin)[HBOXSEL]);
	ti::hidwnd((*ButtonWin)[HUPTO]);
	ti::shownd(TraceStepWin);
	ti::trcstpnum();
	ti::trcratnum();
	displayText::clrhbut(4);
}

void trace::internal::blanklin(std::vector<unsigned>& differenceBitmap, unsigned lineStart) {
	for (auto iPoint = lineStart; iPoint < lineStart + BitmapWidth; iPoint++)
		differenceBitmap[iPoint] = 0;
}

// Check Translation
static inline void trace::internal::difsub(const unsigned source, unsigned shift, unsigned& destination) noexcept {
	destination = (source >> (shift & 0x0f)) & 0xff;
}

void trace::internal::difbits(unsigned shift, unsigned* point) noexcept {
	auto testPoint = point;

	if (testPoint) {
		ti::difsub(*testPoint, shift, TraceAdjacentColors[0]);

		testPoint -= BitmapWidth;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[1]);

		testPoint -= 1;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[2]);

		testPoint += 2;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[3]);

		testPoint += BitmapWidth;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[4]);

		testPoint -= 2;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[5]);

		testPoint += BitmapWidth;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[6]);

		testPoint += 1;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[7]);

		testPoint += 1;
		ti::difsub(*testPoint, shift, TraceAdjacentColors[8]);
	}
}

constexpr unsigned trace::internal::trsum() {
	auto sumAdjacent = 0u;

	for (auto iAdjacent = 1u; iAdjacent < 9; iAdjacent++) {
		sumAdjacent += ((TraceAdjacentColors[iAdjacent] > TraceAdjacentColors[0])
		                    ? (TraceAdjacentColors[iAdjacent] - TraceAdjacentColors[0])
		                    : (TraceAdjacentColors[0] - TraceAdjacentColors[iAdjacent]));
	}
	return sumAdjacent;
}

void trace::untrace() {
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
		for (auto iColor = 0u; iColor < 16u; iColor++) {
			ti::shownd(DefaultColorWin[iColor]);
			ti::shownd(UserColorWin[iColor]);
			ti::shownd(ThreadSizeWin[iColor]);
		}
		for (auto iTrace = 0u; iTrace < 3u; iTrace++) {
			ti::hidwnd(TraceControlWindow[iTrace]);
			ti::hidwnd(TraceSelectWindow[iTrace]);
			ti::hidwnd(TraceUpWindow[iTrace]);
			ti::hidwnd(TraceDownWindow[iTrace]);
		}
		for (auto iButton = 0u; iButton < 9u; iButton++)
			ti::shownd((*ButtonWin)[iButton]);
		ti::hidwnd(TraceStepWin);
	}
	else {
		if (StateMap.test(StateFlag::TRCUP))
			DownPixelColor = 0xffffff;
		else
			UpPixelColor = 0;
	}
}

void trace::trdif() {
	if (!PCSBMPFileName[0]) {
		displayText::tabmsg(IDS_MAPLOD);
		return;
	}
	StateMap.reset(StateFlag::TRSET);
	StateMap.reset(StateFlag::HIDMAP);
	trace::untrace();
	if (BitmapHeight * BitmapWidth) {
		std::vector<unsigned> differenceBitmap(BitmapHeight * BitmapWidth);

		auto colorSumMaximum = 0u;
		auto colorSumMinimum = 0xffffffffu;
		if (!StateMap.test(StateFlag::WASTRAC))
			ti::getrmap();
		for (auto iRGB = 0u; iRGB < 3u; iRGB++) {
			ti::blanklin(differenceBitmap, 0);
			for (auto iHeight = 1u; iHeight < BitmapHeight - 1; iHeight++) {
				auto iPoint                = iHeight * BitmapWidth;
				differenceBitmap[iPoint++] = 0;
				for (auto iWidth = 1u; iWidth < BitmapWidth - 1; iWidth++) {
					ti::difbits(TraceShift[iRGB], &TraceBitmapData[iPoint]);
					differenceBitmap[iPoint] = ti::trsum();
					auto& colorSum           = differenceBitmap[iPoint];
					iPoint++;
					if (colorSum > colorSumMaximum)
						colorSumMaximum = colorSum;
					if (colorSum < colorSumMinimum)
						colorSumMinimum = colorSum;
				}
				differenceBitmap[iPoint++] = 0;
			}
			ti::blanklin(differenceBitmap, ((BitmapHeight - 1) * BitmapWidth));
			const auto ratio = (255.0) / (colorSumMaximum - colorSumMinimum);
			for (auto iPixel = 0u; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
				TraceBitmapData[iPixel] &= TraceRGBMask[iRGB];
				if (differenceBitmap[iPixel]) {
					const auto adjustedColorSum = dToUI((differenceBitmap[iPixel] - colorSumMinimum) * ratio);
					TraceBitmapData[iPixel] |= adjustedColorSum << TraceShift[iRGB];
				}
			}
		}
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
		StateMap.set(StateFlag::WASDIF);
		StateMap.set(StateFlag::RESTCH);
		ti::tracwnd();
	}
}

void trace::trace() {
	if (PCSBMPFileName[0]) {
		trace::untrace();
		ti::tracwnd();
		ti::getrmap();
		if (thred::px2stch() && !StateMap.testAndReset(StateFlag::WASTRCOL)) {
			if (StateMap.test(StateFlag::LANDSCAP))
				SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
			BitmapPoint.x    = dToL(BmpStitchRatio.x * SelectedPoint.x);
			BitmapPoint.y    = dToL(BmpStitchRatio.y * SelectedPoint.y - 1.0);
			const auto color = TraceBitmapData[BitmapPoint.y * BitmapWidth + BitmapPoint.x] ^ 0xffffff;
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
		auto traceColorMask = 0xffffffu;
		if (!StateMap.test(StateFlag::TRCRED))
			traceColorMask &= REDMSK;
		if (!StateMap.test(StateFlag::TRCGRN))
			traceColorMask &= GRNMSK;
		if (!StateMap.test(StateFlag::TRCBLU))
			traceColorMask &= BLUMSK;
		if (traceColorMask != 0xffffff) {
			for (auto iPixel = 0u; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
				TraceBitmapData[iPixel] &= traceColorMask;
			}
		}

#if TRCMTH == 0
		// ToDo - Fix TracedMap and setrac references

		auto usum = ti::icolsum(UpPixelColor);
		auto dsum = ti::icolsum(DownPixelColor);
		if (TracedMap == nullptr) {
			TracedMap = new unsigned[TraceDataSize]();
		}
		for (auto index = 0u; index < BitmapWidth * BitmapHeight; index++) {
			auto psum = colsum(TraceBitmapData[index]);
			if (usum > psum && dsum < psum)
				setrac(index);
			else
				TraceBitmapData[index] = 0;
		}
#endif

#if TRCMTH == 1
		InvertUpColor   = UpPixelColor ^ 0xffffff;
		InvertDownColor = DownPixelColor ^ 0xffffff;
		ti::trcols(InvertUpColor);
		for (auto iRGB = 0u; iRGB < 3; iRGB++) {
			HighColors[iRGB] = PixelColors[iRGB];
		}
		ti::trcols(InvertDownColor);
		for (auto iRGB = 0u; iRGB < 3; iRGB++) {
			LowColors[iRGB] = PixelColors[iRGB];
		}
		if (TracedMap->size() == 0) {
			TracedMap->resize(TraceDataSize, false);
		}
		for (auto iPixel = 0u; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
			if (ti::trcin(TraceBitmapData[iPixel])) {
				TracedMap->set(iPixel);
			}
			else {
				TraceBitmapData[iPixel] = 0;
			}
		}
#endif
		StateMap.set(StateFlag::TRSET);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::tabmsg(IDS_MAPLOD);
	}
}

void trace::tracedg() {
	if (!StateMap.test(StateFlag::WASTRAC))
		trace::trace();
	TracedEdges->resize(TraceDataSize, false);
	TracedEdges->reset();
	auto pixelIndex = 0l;
	for (auto iHeight = 0u; iHeight < BitmapHeight; iHeight++) {
		auto flag = false;
		for (auto iWidth = 0u; iWidth < BitmapWidth; iWidth++) {
			if (TracedMap->test(pixelIndex)) {
				if (!flag) {
					TracedEdges->set(pixelIndex);
					flag = true;
				}
			}
			else {
				if (flag) {
					TracedEdges->set(pixelIndex - 1);
					flag = false;
				}
			}
			pixelIndex++;
		}
		if (flag)
			TracedEdges->set(pixelIndex - 1);
	}
	for (auto iWidth = 0u; iWidth < BitmapWidth; iWidth++) {
		pixelIndex = iWidth;
		auto flag  = false;
		for (auto iHeight = 0u; iHeight < BitmapHeight; iHeight++) {
			if (TracedMap->test(pixelIndex)) {
				if (!flag) {
					TracedEdges->set(pixelIndex);
					flag = true;
				}
			}
			else {
				if (flag) {
					TracedEdges->set(pixelIndex - BitmapWidth);
					flag = false;
				}
			}
			pixelIndex += BitmapWidth;
		}
		if (flag)
			TracedEdges->set(pixelIndex - BitmapWidth);
	}
	for (auto iPixel = 0u; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
		if (TracedEdges->test(iPixel))
			TraceBitmapData[iPixel] = 0xffffff;
		else
			TraceBitmapData[iPixel] = 0;
	}
	StateMap.set(StateFlag::RESTCH);
	StateMap.set(StateFlag::WASEDG);
}

bool trace::internal::trcbit(const unsigned initialDirection, unsigned& traceDirection, std::vector<TRCPNT>& tracedPoints) {
	auto pixelIndex = CurrentTracePoint.y * BitmapWidth + CurrentTracePoint.x;
	switch (traceDirection) {
	case TRCR: {
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
	}
	case TRCD: {
		pixelIndex -= (BitmapWidth + 1);
		if (!CurrentTracePoint.y) {
			traceDirection = TRCR;
		}
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
	}
	case TRCL: {
		pixelIndex += (BitmapWidth - 1);
		if (!CurrentTracePoint.x) {
			traceDirection = TRCD;
		}
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
	}
	case TRCU: {
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
	}
	if (tracedPoints.back().x != CurrentTracePoint.x || tracedPoints.back().y != CurrentTracePoint.y) {
		tracedPoints.push_back({ gsl::narrow<short>(CurrentTracePoint.x), gsl::narrow<short>(CurrentTracePoint.y) });
		if (tracedPoints.size() >= 500000)
			return false;
	}
	return !(traceDirection == initialDirection && CurrentTracePoint.x == tracedPoints[0].x
	         && CurrentTracePoint.y == tracedPoints[0].y);
}

void trace::internal::dutdif(TRCPNT& traceDiff, const TRCPNT* point) noexcept {
	if (point) {
		traceDiff.x = point[1].x - point[0].x;
		// ToDo - this is likely incorrect
		traceDiff.y = point[1].y - point[0].y;
	}
}

void trace::internal::dutrac() {
	if (thred::px2stch()) {
		if (!StateMap.test(StateFlag::WASEDG)) {
			trace::tracedg();
			return;
		}
		thred::savdo();
		if (StateMap.test(StateFlag::LANDSCAP))
			SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
		CurrentTracePoint.x = dToL(BmpStitchRatio.x * SelectedPoint.x);
		CurrentTracePoint.y = dToL(BmpStitchRatio.y * SelectedPoint.y);
		if (CurrentTracePoint.x > gsl::narrow<long>(BitmapWidth))
			CurrentTracePoint.x = BitmapWidth;
		if (CurrentTracePoint.y > gsl::narrow<long>(BitmapHeight))
			CurrentTracePoint.y = BitmapHeight;
		const auto savedPoint     = CurrentTracePoint.y * BitmapWidth + CurrentTracePoint.x;
		auto       traceDirection = 0u;
		if (!TracedEdges->test(savedPoint)) {
			RECT findRectangle = {};
			auto point         = savedPoint;
			auto limit         = (CurrentTracePoint.y + 1) * BitmapWidth;
			while (point < limit && !TracedEdges->test(point))
				point++;
			if (point < limit) {
				findRectangle.right = point - CurrentTracePoint.y * BitmapWidth;
			}
			else {
				findRectangle.right = BitmapWidth;
			}
			point = savedPoint;
			limit = CurrentTracePoint.y * BitmapWidth;
			while (point > limit && !TracedEdges->test(point))
				point--;
			if (point == limit) {
				findRectangle.left = 0;
			}
			else {
				findRectangle.left = point - limit;
			}
			point = savedPoint;
			while (point > 0 && !TracedEdges->test(point))
				point -= BitmapWidth;
			if (point > 0) {
				findRectangle.bottom = point / BitmapWidth;
			}
			else {
				findRectangle.bottom = 0;
			}
			point = savedPoint;
			limit = BitmapWidth * BitmapHeight;
			while (point < limit && !TracedEdges->test(point))
				point += BitmapWidth;
			if (point < limit) {
				findRectangle.top = point / BitmapWidth;
			}
			else {
				findRectangle.top = BitmapHeight;
			}
			auto flag                = 0u;
			auto minimumEdgeDistance = 0x7fffffff;
			if (findRectangle.left) {
				minimumEdgeDistance = CurrentTracePoint.x - findRectangle.left;
				flag                = TRCL;
			}
			if (findRectangle.right < gsl::narrow<long>(BitmapWidth)) {
				const auto edgeDistance = findRectangle.right - CurrentTracePoint.x;
				if (edgeDistance < minimumEdgeDistance) {
					minimumEdgeDistance = edgeDistance;
					flag                = TRCR;
				}
			}
			if (findRectangle.bottom) {
				const auto edgeDistance = CurrentTracePoint.y - findRectangle.bottom;
				if (edgeDistance < minimumEdgeDistance) {
					minimumEdgeDistance = edgeDistance;
					flag                = TRCD;
				}
			}
			if (findRectangle.top < gsl::narrow<long>(BitmapHeight)) {
				const auto edgeDistance = findRectangle.top - CurrentTracePoint.y;
				if (edgeDistance < minimumEdgeDistance) {
					minimumEdgeDistance = edgeDistance;
					flag                = TRCU;
				}
			}
			switch (flag) {
			case TRCU: {
				CurrentTracePoint.y = findRectangle.top;
				traceDirection      = TRCR;
				break;
			}
			case TRCR: {
				CurrentTracePoint.x = findRectangle.right;
				traceDirection      = TRCD;
				break;
			}
			case TRCD: {
				CurrentTracePoint.y = findRectangle.bottom;
				traceDirection      = TRCL;
				break;
			}
			case TRCL: {
				CurrentTracePoint.x = findRectangle.left;
				traceDirection      = TRCU;
				break;
			}
			default:
				return;
			}
		}
		const unsigned      initialDirection = traceDirection;
		std::vector<TRCPNT> tracedPoints;
		tracedPoints.push_back({ gsl::narrow<short>(CurrentTracePoint.x), gsl::narrow<short>(CurrentTracePoint.y) });
		while (ti::trcbit(initialDirection, traceDirection, tracedPoints))
			;
		if (tracedPoints.size() >= 500000) {
			displayText::tabmsg(IDS_FRM2L);
			return;
		}
		std::vector<TRCPNT> decimatedLine;
		decimatedLine.reserve(tracedPoints.size());
		TRCPNT traceDiff[2] = {};
		decimatedLine.push_back(tracedPoints[0]);
		ti::dutdif(traceDiff[0], tracedPoints.data());
		OutputIndex = 1;
		for (auto iPoint = 1u; iPoint < tracedPoints.size(); iPoint++) {
			traceDiff[1] = traceDiff[0];
			ti::dutdif(traceDiff[0], &tracedPoints[iPoint]);
			if (traceDiff[1].x != traceDiff[0].x || traceDiff[1].y != traceDiff[0].y) {
				decimatedLine.push_back(tracedPoints[iPoint]);
			}
		}
		tracedPoints.clear();
		tracedPoints.push_back(decimatedLine[0]);
		auto iNext = 0u;
		for (auto iCurrent = 1u; iCurrent < decimatedLine.size(); iCurrent++) {
			const auto traceLength
			    = hypot(decimatedLine[iCurrent].x - decimatedLine[iNext].x, decimatedLine[iCurrent].y - decimatedLine[iNext].y);
			if (traceLength > IniFile.traceLength) {
				tracedPoints.push_back(decimatedLine[iNext]);
				iNext = iCurrent;
			}
		}
		for (auto iCurrent = iNext + 1; iCurrent < decimatedLine.size(); iCurrent++) {
			tracedPoints.push_back(decimatedLine[iCurrent]);
		}
		SelectedForm = &FormList[FormIndex];
		form::frmclr(SelectedForm);
		CurrentFormVertices      = &FormVertices[FormVertexIndex];
		CurrentFormVertices[0].x = tracedPoints[0].x * StitchBmpRatio.x;
		CurrentFormVertices[0].y = tracedPoints[0].y * StitchBmpRatio.y;
		iNext                    = 0;
		OutputIndex              = 0;
		auto traceLengthSum      = 0.0;
		auto landscapeOffset     = 0.0;
		if (StateMap.test(StateFlag::LANDSCAP))
			landscapeOffset = UnzoomedRect.y - BitmapSizeinStitches.y;
		for (auto iCurrent = 1u; iCurrent < tracedPoints.size(); iCurrent++) {
			traceLengthSum += hypot(tracedPoints[iCurrent].x - tracedPoints[iCurrent - 1].x,
			                        tracedPoints[iCurrent].y - tracedPoints[iCurrent - 1].y);
			const auto traceLength
			    = hypot(tracedPoints[iCurrent].x - tracedPoints[iNext].x, tracedPoints[iCurrent].y - tracedPoints[iNext].y);
			if (traceLengthSum > traceLength * IniFile.traceRatio) {
				CurrentFormVertices[OutputIndex].x = tracedPoints[iCurrent - 1].x * StitchBmpRatio.x;
				CurrentFormVertices[OutputIndex].y = tracedPoints[iCurrent - 1].y * StitchBmpRatio.y + landscapeOffset;
				OutputIndex++;
				iCurrent--;
				iNext          = iCurrent;
				traceLengthSum = 0.0;
			}
		}
		if (FormVertexIndex + OutputIndex > MAXITEMS) {
			displayText::tabmsg(IDS_FRMOVR);
			return;
		}
		SelectedForm->vertices    = thred::adflt(OutputIndex);
		SelectedForm->vertexCount = gsl::narrow<unsigned short>(OutputIndex);
		SelectedForm->type        = FRMFPOLY;
		SelectedForm->attribute   = gsl::narrow<unsigned char>(ActiveLayer << 1);
		form::frmout(FormIndex);
		FormList[FormIndex].satinGuideCount = 0;
		FormIndex++;
		StateMap.set(StateFlag::RESTCH);
		StateMap.set(StateFlag::FRMOF);
		form::tglfrm();
	}
}

void trace::trinit() {
	unsigned histogramData[3][256] = { 0 };

	if (PCSBMPFileName[0]) {
		if (!StateMap.test(StateFlag::TRSET)) {
			StateMap.set(StateFlag::TRCRED);
			StateMap.set(StateFlag::TRCGRN);
			StateMap.set(StateFlag::TRCBLU);
			unsigned componentPeak[3] = { 0 };
			if (!StateMap.test(StateFlag::WASTRAC))
				ti::getrmap();
			if (StateMap.test(StateFlag::MONOMAP)) {
				// ToDo - Is this intializing correctly?
				auto color  = gsl::narrow<COLORREF>(TraceBitmapData[0]);
				auto iPixel = 0u;
				for (iPixel = 0; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
					if (TraceBitmapData[iPixel] != color)
						break;
				}
				if (iPixel < BitmapWidth * BitmapHeight) {
					ti::trcols(TraceBitmapData[iPixel]);
					HighColors[0] = PixelColors[0];
					HighColors[1] = PixelColors[1];
					HighColors[2] = PixelColors[2];
					ti::trcols(color);
					for (auto iRGB = 0u; iRGB < 3; iRGB++) {
						if (PixelColors[iRGB] > HighColors[iRGB]) {
							const auto swapComponent = PixelColors[iRGB];
							PixelColors[iRGB]        = HighColors[iRGB];
							HighColors[iRGB]         = swapComponent;
						}
						componentPeak[iRGB] = ((HighColors[iRGB] - PixelColors[iRGB]) >> 1) + PixelColors[iRGB];
					}
				}
			}
			else {
				for (auto iPixel = 0u; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
					ti::trcols(TraceBitmapData[iPixel]);
					for (auto iRGB = 0u; iRGB < 3; iRGB++) {
						histogramData[iRGB][PixelColors[iRGB]]++;
					}
				}
				unsigned componentPeakCount[3] = { 0 };
				for (auto iLevel = 0u; iLevel < 256; iLevel++) {
					for (auto iRGB = 0u; iRGB < 3; iRGB++) {
						if (histogramData[iRGB][iLevel] > componentPeakCount[iRGB]) {
							componentPeakCount[iRGB] = histogramData[iRGB][iLevel];
							componentPeak[iRGB]      = iLevel;
						}
					}
				}
			}
			InvertDownColor = 0;
			for (auto iRGB = 0u; iRGB < 3; iRGB++) {
				if (componentPeak[iRGB]) {
					componentPeak[iRGB]--;
				}
				InvertDownColor |= componentPeak[iRGB] << TraceShift[iRGB];
			}
			DownPixelColor = InvertDownColor ^ 0xffffff;
			InvertUpColor  = 0xffffff;
			UpPixelColor   = 0;
		}
		StateMap.set(StateFlag::WASTRCOL);
		trace::trace();
	}
	else {
		displayText::tabmsg(IDS_MAPLOD);
	}
}

void trace::trcsel() {
	if (PCSBMPFileName[0]) {
		StateMap.set(StateFlag::WASTRCOL);
		StateMap.set(StateFlag::TRCRED);
		StateMap.set(StateFlag::TRCBLU);
		StateMap.set(StateFlag::TRCGRN);
		DownPixelColor = 0xffffff;
		UpPixelColor   = 0;
		trace::trace();
		StateMap.reset(StateFlag::HIDMAP);
		StateMap.reset(StateFlag::TRSET);
		for (auto iPixel = 0u; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
			ti::trcols(TraceBitmapData[iPixel]);
			auto maximumColorComponent = PixelColors[0];
			auto iRGB                  = 2u;
			if (PixelColors[1] > maximumColorComponent) {
				maximumColorComponent = PixelColors[1];
				iRGB                  = 1;
			}
			if (PixelColors[2] > maximumColorComponent) {
				iRGB = 0;
			}
			TraceBitmapData[iPixel] &= TraceRGB[iRGB];
		}
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
		StateMap.set(StateFlag::WASDSEL);
		StateMap.set(StateFlag::RESTCH);
		ti::tracwnd();
	}
	else {
		displayText::tabmsg(IDS_MAPLOD);
	}
}

void trace::internal::ritrcol(COLORREF* color, unsigned number) noexcept {
	if (color) {
		*color &= TraceRGBMask[ColumnColor];
		number &= 0xff;
		*color |= (number << TraceShift[ColumnColor]);
	}
}

void trace::internal::dutrnum0(unsigned color) {
	StateMap.reset(StateFlag::NUMIN);
	StateMap.reset(StateFlag::TRNIN0);
	if (StateMap.test(StateFlag::TRNUP)) {
		ti::ritrcol(&InvertUpColor, color);
		UpPixelColor = InvertUpColor ^ 0xffffff;
		thred::redraw(TraceUpWindow[ColumnColor]);
	}
	else {
		ti::ritrcol(&InvertDownColor, color);
		DownPixelColor = InvertDownColor ^ 0xffffff;
		thred::redraw(TraceDownWindow[ColumnColor]);
	}
	thred::redraw(TraceControlWindow[ColumnColor]);
	DestroyWindow(TraceNumberInput);
	StateMap.set(StateFlag::WASTRCOL);
	trace::trace();
}

void trace::dutrnum2() {
	ti::dutrnum0(std::stoi(TraceInputBuffer));
}

void trace::dutrnum1() {
	DestroyWindow(GeneralNumberInputBox);
	StateMap.reset(StateFlag::NUMIN);
	StateMap.reset(StateFlag::TRNIN1);
	auto traceLength = std::stof(MsgBuffer);
	if (traceLength > 9.0f) {
		traceLength = 9.0f;
	}
	if (StateMap.test(StateFlag::TRNUP)) {
		IniFile.traceLength = traceLength * PFGRAN;
		ti::trcstpnum();
	}
	else {
		IniFile.traceRatio = 1 + pow(0.1, traceLength);
		ti::trcratnum();
	}
}

unsigned trace::internal::ducolm() {
	if (TraceMsgPoint.x < gsl::narrow<int>(ButtonWidth))
		return 0;
	if (TraceMsgPoint.x < gsl::narrow<int>(ButtonWidth) << 1)
		return 1;

	return 2;
}

void trace::internal::trnumwnd0(int position) noexcept {
	TraceNumberInput = CreateWindow(L"STATIC",
	                                nullptr,
	                                SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                ButtonWidthX3,
	                                position,
	                                ButtonWidth,
	                                ButtonHeight,
	                                ThrEdWindow,
	                                nullptr,
	                                ThrEdInstance,
	                                nullptr);
}

void trace::internal::trnumwnd1(int position) noexcept {
	GeneralNumberInputBox = CreateWindow(L"STATIC",
	                                     nullptr,
	                                     WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                     ButtonWidthX3,
	                                     position,
	                                     ButtonWidthX3,
	                                     ButtonHeight,
	                                     ThrEdWindow,
	                                     nullptr,
	                                     ThrEdInstance,
	                                     nullptr);
}

void trace::internal::stch2bit(fPOINT& point) {
	if (StateMap.test(StateFlag::LANDSCAP))
		point.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
	BitmapPoint.x = dToL(BmpStitchRatio.x * point.x);
	BitmapPoint.y = dToL(BitmapHeight - BmpStitchRatio.y * point.y);
}

void trace::internal::pxlin(unsigned int start, unsigned int finish) {
	POINT line[2];

	ti::stch2bit(CurrentFormVertices[start]);
	line[0] = BitmapPoint;
	ti::stch2bit(CurrentFormVertices[finish]);
	line[1] = BitmapPoint;
	Polyline(BitmapDC, line, 2);
	Polyline(TraceDC, line, 2);
}

void trace::internal::bfrm() {
	if (VertexCount) {
		for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
			ti::pxlin(iVertex, iVertex + 1);
		}
		if (SelectedForm->type != FRMLINE) {
			ti::pxlin(VertexCount - 1, 0);
		}
	}
}

void trace::blak() {
	if (!PCSBMPFileName[0]) {
		displayText::tabmsg(IDS_MAPLOD);
		return;
	}
	ti::tracwnd();
	if (FormIndex) {
		BlackPen = CreatePenInt(PS_SOLID, 1, 0);
		SelectObject(BitmapDC, BlackPen);
		SelectObject(TraceDC, BlackPen);
		if (!StateMap.test(StateFlag::WASTRAC))
			ti::getrmap();
		for (auto iForm = 0u; iForm < FormIndex; iForm++) {
			form::fvars(iForm);
			ti::bfrm();
		}
		DeleteObject(BlackPen);
		StateMap.set(StateFlag::WASBLAK);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::tabmsg(IDS_FRMNO);
	}
}

void trace::tracpar() {
	if (StateMap.test(StateFlag::TRNIN0))
		trace::dutrnum2();
	if (StateMap.test(StateFlag::TRNIN1))
		trace::dutrnum1();
	TraceMsgPoint.x = Msg.pt.x - ThredWindowOrigin.x;
	TraceMsgPoint.y = Msg.pt.y - ThredWindowOrigin.y;
	if (TraceMsgPoint.x > gsl::narrow<int>(ButtonWidthX3))
		ti::dutrac();
	else {
		ColumnColor = ti::ducolm();
		if (TraceMsgPoint.y < gsl::narrow<int>(ButtonHeight) * 15) {
			do {
				if (GetKeyState(VK_SHIFT) & 0X8000) {
					UpPixelColor &= TraceRGBMask[ColumnColor];
					DownPixelColor |= TraceRGB[2 - ColumnColor];
					break;
				}
				const auto     ratio         = (TraceMsgPoint.y) / (ButtonHeight * 15.0);
				const auto     position      = dToUI(std::floor(ratio * 255.0));
				COLORREF       traceColor    = UpPixelColor & TraceRGB[2 - ColumnColor];
				const COLORREF tracePosition = position << TraceShift[ColumnColor];
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
			thred::redraw(TraceControlWindow[ColumnColor]);
			trace::trace();
		}
		else {
			const auto position = dToUI(std::floor(TraceMsgPoint.y / ButtonHeight));
			if (position < 16) {
				StateMap.flip(TraceRGBFlag[ColumnColor]);
				thred::redraw(TraceSelectWindow[ColumnColor]);
				trace::trace();
			}
			else {
				if (position < 18) {
					StateMap.set(StateFlag::NUMIN);
					StateMap.set(StateFlag::TRNIN0);
					MsgIndex          = 0;
					*TraceInputBuffer = 0;
					if (position < 17) {
						ti::trnumwnd0(ButtonHeight * 16);
						StateMap.set(StateFlag::TRNUP);
					}
					else {
						ti::trnumwnd0(ButtonHeight * 17);
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
							ti::trnumwnd1(ButtonHeight * 18);
							StateMap.set(StateFlag::TRNUP);
						}
						else {
							ti::trnumwnd1(ButtonHeight * 19);
							StateMap.reset(StateFlag::TRNUP);
						}
					}
					else {
						switch (position) {
						case 20: {
							trace::trdif();
							break;
						}
						case 21: {
							thred::hidbit();
							break;
						}
						case 22: {
							trace::blak();
							break;
						}
						case 23: {
							trace::trcsel();
							break;
						}
						case 24: {
							trace::tracedg();
							break;
						}
						}
					}
				}
			}
		}
	}
}

void trace::internal::trcnum(unsigned shift, COLORREF color, unsigned iRGB) {
	const unsigned NumeralWidth = thred::txtWid(L"0");

	wchar_t buffer[11] = { 0 };

	color >>= shift;
	color &= 0xff;
	_itow_s(color, buffer, 10);
	const auto bufferLength = gsl::narrow<unsigned int>(wcslen(buffer));
	const auto xPosition    = NumeralWidth * (3 - bufferLength) + 1;
	SetBkColor(DrawItem->hDC, TraceRGB[iRGB]);
	TextOutInt(DrawItem->hDC, xPosition, 1, buffer, bufferLength);
}

void trace::internal::upnum(unsigned iRGB) {
	ti::trcnum(TraceShift[iRGB], InvertUpColor, iRGB);
}

void trace::internal::dwnum(unsigned iRGB) {
	ti::trcnum(TraceShift[iRGB], InvertDownColor, iRGB);
}

void trace::internal::durct(unsigned    shift,
                            const RECT& traceControlRect,
                            RECT&       traceHighMask,
                            RECT&       traceMiddleMask,
                            RECT&       traceLowMask) {
	const unsigned lowerColor    = (UpPixelColor >> shift) & 0xff;
	const unsigned upperColor    = (DownPixelColor >> shift) & 0xff;
	const unsigned controlHeight = traceControlRect.bottom - traceControlRect.top;

	traceHighMask.left = traceLowMask.left = traceMiddleMask.left = traceControlRect.left;
	traceHighMask.right = traceLowMask.right = traceMiddleMask.right = traceControlRect.right;

	auto ratio             = static_cast<double>(lowerColor) / 255;
	traceMiddleMask.top    = dToL(controlHeight * ratio + traceControlRect.top);
	ratio                  = static_cast<double>(upperColor) / 255;
	traceMiddleMask.bottom = dToL(controlHeight * ratio + traceControlRect.top);
	StateMap.reset(StateFlag::DUHI);
	StateMap.reset(StateFlag::DULO);
	if (lowerColor) {
		StateMap.set(StateFlag::DULO);
		traceLowMask.bottom = traceMiddleMask.top;
		traceLowMask.top    = 0;
	}
	if (upperColor != 255) {
		StateMap.set(StateFlag::DUHI);
		traceHighMask.top    = traceMiddleMask.bottom;
		traceHighMask.bottom = traceControlRect.bottom;
	}
}

void trace::internal::dublk(HDC dc, const RECT& traceHighMask, const RECT& traceLowMask, HBRUSH brush) {
	if (StateMap.test(StateFlag::DUHI))
		FillRect(dc, &traceHighMask, brush);
	if (StateMap.test(StateFlag::DULO))
		FillRect(dc, &traceLowMask, brush);
}

void trace::wasTrace() {
	RECT         traceHighMaskRect   = {};                  // high trace mask rectangle
	RECT         traceMiddleMaskRect = {};                  // middle trace mask rectangle
	RECT         traceLowMaskRect    = {};                  // low trace mask rectangle
	const HBRUSH BlackBrush          = CreateSolidBrush(0); // black brush

	for (auto iRGB = 0; iRGB < 3; iRGB++) {
		if (DrawItem->hwndItem == TraceUpWindow[iRGB]) {
			FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[iRGB]);
			ti::upnum(iRGB);
			break;
		}
		if (DrawItem->hwndItem == TraceDownWindow[iRGB]) {
			FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[iRGB]);
			ti::dwnum(iRGB);
		}
		if (DrawItem->hwndItem == TraceControlWindow[iRGB]) {
			ti::durct(TraceShift[iRGB], DrawItem->rcItem, traceHighMaskRect, traceMiddleMaskRect, traceLowMaskRect);
			FillRect(DrawItem->hDC, &traceMiddleMaskRect, TraceBrush[iRGB]);
			ti::dublk(DrawItem->hDC, traceHighMaskRect, traceLowMaskRect, BlackBrush);
			break;
		}
		if (DrawItem->hwndItem == TraceSelectWindow[iRGB]) {
			auto    TempBrush  = BlackBrush;
			wchar_t buffer[10] = { 0 }; // for integer to string conversion
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
			TextOutInt(DrawItem->hDC, 1, 1, buffer, gsl::narrow<unsigned int>(wcslen(buffer)));
			break;
		}
		if (DrawItem->hwndItem == TraceNumberInput) {
			FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[ColumnColor]);
			SetBkColor(DrawItem->hDC, TraceRGB[ColumnColor]);
			TextOutInt(DrawItem->hDC, 1, 1, TraceInputBuffer, gsl::narrow<unsigned int>(wcslen(TraceInputBuffer)));
			break;
		}
	}
}

void trace::wasTrace1() {
	if (StateMap.test(StateFlag::TRNIN0))
		trace::dutrnum2();
	if (StateMap.test(StateFlag::TRNIN1))
		trace::dutrnum1();
	if (!StateMap.test(StateFlag::WASEDG))
		trace::tracpar();
}

void trace::traceNumberInput(unsigned NumericCode) {
	TraceInputBuffer[MsgIndex++] = NumericCode;
	TraceInputBuffer[MsgIndex]   = 0;
	auto traceColor              = std::stoi(TraceInputBuffer);
	switch (MsgIndex) {
	case 2: {
		if (traceColor > 25) {
			ti::dutrnum0(traceColor);
		}
		break;
	}
	case 3: {
		if (traceColor > 255) {
			traceColor = 255;
		}
		ti::dutrnum0(traceColor);
		break;
	}
	}
	thred::redraw(TraceNumberInput);
}

void trace::traceNumberReset() noexcept {
	TraceInputBuffer[MsgIndex] = 0;
	thred::redraw(TraceNumberInput);
}