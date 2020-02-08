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

uint32_t  PixelColors[3];        // separated pixel reference colors
HBITMAP   TraceBitmap;           // trace bitmap
HWND      TraceControlWindow[3]; // trace control windows
HWND      TraceDownWindow[3];    // trace down number windows
HWND      TraceSelectWindow[3];  // trace select windows
HWND      TraceUpWindow[3];      // trace up number windows
POINT     CurrentTracePoint;     // current point being traced
uint32_t  TraceDataSize;         // size of the trace bitmap in double words
uint32_t* TracedPixels;          // bitmap of selected trace pixels
HWND      TraceStepWin;          // trace stepSize window
StateFlag TraceRGBFlag[] = {StateFlag::TRCRED, StateFlag::TRCGRN, StateFlag::TRCBLU}; // trace bits
uint32_t  TraceRGBMask[] = {REDMSK, GRNMSK, BLUMSK};                                  // trace masks
uint32_t  TraceRGB[]     = {BLUCOL, GRNCOL, REDCOL}; // trace colors
uint32_t  TraceAdjacentColors[9];                    // separated colors for adjacent pixels
wchar_t   TraceInputBuffer[4];                       // for user input color numbers
COLORREF  UpPixelColor;                              // color of the up reference pixel
COLORREF  DownPixelColor;                            // color of the down reference pixel
COLORREF  InvertUpColor;                             // complement color of the up reference pixel
COLORREF  InvertDownColor;                           // complement color of the down reference pixel
POINT     TraceMsgPoint;                             // message point for trace parsing
uint32_t  HighColors[3];                             // separated upper reference colors
uint32_t  LowColors[3];                              // separated lower reference colors
uint32_t  ColumnColor;                               // trace color column
uint32_t  TraceShift[] = {0U, 8U, 16U};              // trace shift values
HBRUSH    TraceBrush[3];                             // red,green,and blue brushes
HWND      TraceNumberInput;                          // trace number input window
HPEN      BlackPen;                                  // black pen

void trace::initColorRef() noexcept {
  UpPixelColor    = 0;
  DownPixelColor  = 0x7f7f7f;
  InvertUpColor   = 0xffffff;
  InvertDownColor = 0x808080;
}

#pragma warning(push)
#pragma warning(disable : 26487)
void trace::internal::trcsub(HWND* window, uint32_t xCoordinate, uint32_t yCoordinate, uint32_t buttonHeight) noexcept {
  if (window != nullptr) {
	*window = CreateWindow(L"STATIC", // NOLINT
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
#pragma warning(pop)

void trace::initTraceWindows() noexcept {
  TraceStepWin = CreateWindow(L"STATIC", // NOLINT
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
	ti::trcsub(&TraceControlWindow[iRGB], ButtonWidth * iRGB, 0, ButtonHeight * 15);
	ti::trcsub(&TraceSelectWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 15, ButtonHeight);
	ti::trcsub(&TraceUpWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 16, ButtonHeight);
	ti::trcsub(&TraceDownWindow[iRGB], ButtonWidth * iRGB, ButtonHeight * 17, ButtonHeight);
	TraceBrush[iRGB] = CreateSolidBrush(TraceRGB[iRGB]);
  }
}

void trace::internal::trcols(COLORREF color) noexcept {
  PixelColors[0] = color & 0xffU;
  PixelColors[1] = (color & 0xff00U) >> 8U;
  PixelColors[2] = (color & 0xff0000U) >> 16U;
}

void trace::internal::trcstpnum() {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_TRCSTP);
  SetWindowText(TraceStepWin, fmt::format(fmtStr, (IniFile.traceLength / PFGRAN)).c_str());
}

void trace::internal::trcratnum() {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_TRCRAT);
  displayText::butxt(HLIN, fmt::format(fmtStr, -log10(IniFile.traceRatio - 1.0F)));
}

auto trace::internal::trcin(COLORREF color) -> bool {
  if (color != 0U) {
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
  auto header = BITMAPINFOHEADER {gsl::narrow<DWORD>(sizeof(BITMAPINFOHEADER)),
                                  gsl::narrow_cast<LONG>(BitmapWidth),
                                  gsl::narrow_cast<LONG>(BitmapHeight),
                                  1U,
                                  32U,
                                  BI_RGB,
                                  0U,
                                  0L,
                                  0L,
                                  0U,
                                  0U};
  auto info   = BITMAPINFO {header, {RGBQUAD {0, 0, 0, 0}}};
  TraceBitmap = thred::getBitmap(BitmapDC, &info, &TraceBitmapData);
  TraceDC     = CreateCompatibleDC(StitchWindowDC);
  if ((TraceBitmap != nullptr) && (TraceDC != nullptr)) {
	SelectObject(TraceDC, TraceBitmap);
	BitBlt(TraceDC, 0, 0, BitmapWidth, BitmapHeight, BitmapDC, 0, 0, SRCCOPY);
	StateMap.set(StateFlag::WASTRAC);
	TraceDataSize = (BitmapWidth + 1U) * (BitmapHeight + 1U);
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
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	ti::hidwnd(DefaultColorWin->operator[](iColor));
	ti::hidwnd(UserColorWin->operator[](iColor));
	ti::hidwnd(ThreadSizeWin[iColor]);
  }
  for (auto iTrace = 0U; iTrace < 3; iTrace++) {
	ti::shownd(TraceControlWindow[iTrace]);
	ti::shownd(TraceSelectWindow[iTrace]);
	ti::shownd(TraceUpWindow[iTrace]);
	ti::shownd(TraceDownWindow[iTrace]);
  }
  ti::hidwnd(ButtonWin->operator[](HBOXSEL));
  ti::hidwnd(ButtonWin->operator[](HUPTO));
  ti::shownd(TraceStepWin);
  ti::trcstpnum();
  ti::trcratnum();
  displayText::clrhbut(4);
}

void trace::internal::blanklin(std::vector<uint32_t>& differenceBitmap, uint32_t lineStart) noexcept {
  for (auto iPoint = lineStart; iPoint < lineStart + BitmapWidth; iPoint++) {
	differenceBitmap[iPoint] = 0;
  }
}

// Check Translation
static inline void trace::internal::difsub(uint32_t const source, uint32_t shift, uint32_t& destination) noexcept {
  destination = (source >> (shift & 0x0fU)) & 0xffU;
}

void trace::internal::difbits(uint32_t shift, uint32_t* point) noexcept {
  auto testPoint = point;

  if (testPoint != nullptr) {
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

auto trace::internal::trsum() noexcept -> uint32_t {
  auto sumAdjacent = 0U;

  for (auto iAdjacent = 1U; iAdjacent < 9U; iAdjacent++) {
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
	if (!TracedEdges->empty()) {
	  TracedEdges->resize(0); // allocated in tracedg
	}
	if (!TracedMap->empty()) {
	  TracedMap->resize(0); // allocated in trace
	}
	StateMap.reset(StateFlag::WASEDG);
	for (auto iColor = 0U; iColor < 16U; iColor++) {
	  ti::shownd(DefaultColorWin->operator[](iColor));
	  ti::shownd(UserColorWin->operator[](iColor));
	  ti::shownd(ThreadSizeWin[iColor]);
	}
	for (auto iTrace = 0U; iTrace < 3U; iTrace++) {
	  ti::hidwnd(TraceControlWindow[iTrace]);
	  ti::hidwnd(TraceSelectWindow[iTrace]);
	  ti::hidwnd(TraceUpWindow[iTrace]);
	  ti::hidwnd(TraceDownWindow[iTrace]);
	}
	for (auto iButton = 0U; iButton < 9U; iButton++) {
	  ti::shownd(ButtonWin->operator[](iButton));
	}
	ti::hidwnd(TraceStepWin);
  }
  else {
	if (StateMap.test(StateFlag::TRCUP)) {
	  DownPixelColor = 0xffffff;
	}
	else {
	  UpPixelColor = 0;
	}
  }
}

void trace::trdif() {
  if (PCSBMPFileName[0] == 0) {
	displayText::tabmsg(IDS_MAPLOD);
	return;
  }
  StateMap.reset(StateFlag::TRSET);
  StateMap.reset(StateFlag::HIDMAP);
  trace::untrace();
  if ((BitmapHeight * BitmapWidth) != 0U) {
	auto differenceBitmap = std::vector<uint32_t> {};
	differenceBitmap.resize(wrap::toSize(BitmapHeight) * BitmapWidth);

	auto colorSumMaximum = 0U;
	auto colorSumMinimum = 0xffffffffU;
	if (!StateMap.test(StateFlag::WASTRAC)) {
	  ti::getrmap();
	}
	for (auto iRGB = 0U; iRGB < 3U; iRGB++) {
	  ti::blanklin(differenceBitmap, 0);
	  for (auto iHeight = 1U; iHeight < BitmapHeight - 1U; iHeight++) {
		auto iPoint = iHeight * BitmapWidth;

		differenceBitmap[iPoint++] = 0;
		for (auto iWidth = 1U; iWidth < BitmapWidth - 1U; iWidth++) {
		  ti::difbits(TraceShift[iRGB], &TraceBitmapData[iPoint]);
		  differenceBitmap[iPoint] = ti::trsum();
		  auto& colorSum           = differenceBitmap[iPoint];
		  iPoint++;
		  if (colorSum > colorSumMaximum) {
			colorSumMaximum = colorSum;
		  }
		  if (colorSum < colorSumMinimum) {
			colorSumMinimum = colorSum;
		  }
		}
		differenceBitmap[iPoint++] = 0;
	  }
	  ti::blanklin(differenceBitmap, ((BitmapHeight - 1) * BitmapWidth));
	  auto const ratio = (255.0) / (colorSumMaximum - colorSumMinimum);
	  for (auto iPixel = 0U; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
		TraceBitmapData[iPixel] &= TraceRGBMask[iRGB];
		if (differenceBitmap[iPixel] != 0U) {
		  auto const adjustedColorSum =
		      wrap::round<uint32_t>((differenceBitmap[iPixel] - colorSumMinimum) * ratio);
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

#if TRCMTH == 0
unsigned trace::internal::colsum(COLORREF col) {
  ti::trcols(col);
  auto colorSum = 0U;
  for (auto iRGB = 0U; iRGB < 3U; iRGB++) {
	if (StateMap.test(TraceRGBFlag[iRGB]))
	  colorSum += PixelColors[iRGB];
  }
  return colorSum;
}

unsigned trace::internal::icolsum(COLORREF col) {
  ti::trcols(col);
  auto colorSum = 0U;
  for (auto iRGB = 0U; iRGB < 3U; iRGB++) {
	if (StateMap.test(TraceRGBFlag[iRGB]))
	  colorSum += 255 - PixelColors[iRGB];
  }
  return colorSum;
}
#endif

void trace::trace() {
  if (PCSBMPFileName[0] != 0) {
	trace::untrace();
	ti::tracwnd();
	ti::getrmap();
	if (thred::px2stch() && !StateMap.testAndReset(StateFlag::WASTRCOL)) {
	  if (StateMap.test(StateFlag::LANDSCAP)) {
		SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
	  }
	  BitmapPoint.x    = wrap::round<int32_t>(BmpStitchRatio.x * SelectedPoint.x);
	  BitmapPoint.y    = wrap::round<int32_t>(BmpStitchRatio.y * SelectedPoint.y - 1.0F);
	  auto const color = TraceBitmapData[BitmapPoint.y * BitmapWidth + BitmapPoint.x] ^ 0xffffffU;
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
	auto traceColorMask = 0xffffffU;
	if (!StateMap.test(StateFlag::TRCRED)) {
	  traceColorMask &= REDMSK;
	}
	if (!StateMap.test(StateFlag::TRCGRN)) {
	  traceColorMask &= GRNMSK;
	}
	if (!StateMap.test(StateFlag::TRCBLU)) {
	  traceColorMask &= BLUMSK;
	}
	if (traceColorMask != 0xffffff) {
	  for (auto iPixel = 0U; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
		TraceBitmapData[iPixel] &= traceColorMask;
	  }
	}

#if TRCMTH == 0
	auto upBrightness   = ti::icolsum(UpPixelColor);
	auto downBrightness = ti::icolsum(DownPixelColor);
	if (TracedMap->empty()) {
	  TracedMap->resize(TraceDataSize, false);
	}
	for (auto index = 0U; index < BitmapWidth * BitmapHeight; index++) {
	  auto pointBrightness = ti::colsum(TraceBitmapData[index]);
	  if (upBrightness > pointBrightness && downBrightness < pointBrightness)
		TracedMap->set(index);
	  else
		TraceBitmapData[index] = 0;
	}
#endif

#if TRCMTH == 1
	InvertUpColor   = UpPixelColor ^ 0xffffffU;
	InvertDownColor = DownPixelColor ^ 0xffffffU;
	ti::trcols(InvertUpColor);
	for (auto iRGB = 0U; iRGB < 3; iRGB++) {
	  HighColors[iRGB] = PixelColors[iRGB];
	}
	ti::trcols(InvertDownColor);
	for (auto iRGB = 0U; iRGB < 3; iRGB++) {
	  LowColors[iRGB] = PixelColors[iRGB];
	}
	if (TracedMap->empty()) {
	  TracedMap->resize(TraceDataSize, false);
	}
	for (auto iPixel = 0U; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
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
  if (!StateMap.test(StateFlag::WASTRAC)) {
	trace::trace();
  }
  TracedEdges->resize(TraceDataSize, false);
  TracedEdges->reset();
  auto pixelIndex = 0L;
  for (auto iHeight = 0U; iHeight < BitmapHeight; iHeight++) {
	auto flag = false;
	for (auto iWidth = 0U; iWidth < BitmapWidth; iWidth++) {
	  if (TracedMap->test(pixelIndex)) {
		if (!flag) {
		  TracedEdges->set(pixelIndex);
		  flag = true;
		}
	  }
	  else {
		if (flag) {
		  TracedEdges->set(wrap::toSize(pixelIndex) - 1U);
		  flag = false;
		}
	  }
	  pixelIndex++;
	}
	if (flag) {
	  TracedEdges->set(wrap::toSize(pixelIndex) - 1U);
	}
  }
  for (auto iWidth = 0U; iWidth < BitmapWidth; iWidth++) {
	pixelIndex = iWidth;
	auto flag  = false;
	for (auto iHeight = 0U; iHeight < BitmapHeight; iHeight++) {
	  if (TracedMap->test(pixelIndex)) {
		if (!flag) {
		  TracedEdges->set(pixelIndex);
		  flag = true;
		}
	  }
	  else {
		if (flag) {
		  TracedEdges->set(wrap::toSize(pixelIndex) - BitmapWidth);
		  flag = false;
		}
	  }
	  pixelIndex += BitmapWidth;
	}
	if (flag) {
	  TracedEdges->set(wrap::toSize(pixelIndex) - BitmapWidth);
	}
  }
  for (auto iPixel = 0U; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
	if (TracedEdges->test(iPixel)) {
	  TraceBitmapData[iPixel] = 0xffffff;
	}
	else {
	  TraceBitmapData[iPixel] = 0;
	}
  }
  StateMap.set(StateFlag::RESTCH);
  StateMap.set(StateFlag::WASEDG);
}

auto trace::internal::trcbit(uint32_t const       initialDirection,
                             uint32_t&            traceDirection,
                             std::vector<TRCPNT>& tracedPoints) -> bool {
  auto pixelIndex = CurrentTracePoint.y * BitmapWidth + CurrentTracePoint.x;
  switch (traceDirection) {
	case TRCR: {
	  pixelIndex += (1 - BitmapWidth);
	  if (CurrentTracePoint.x == gsl::narrow<int32_t>(BitmapWidth) - 1) {
		traceDirection = TRCU;
	  }
	  else {
		if (TracedEdges->test(pixelIndex)) {
		  CurrentTracePoint.x++;
		  CurrentTracePoint.y--;
		  traceDirection = TRCD;
		}
		else {
		  pixelIndex += BitmapWidth;
		  if (TracedEdges->test(pixelIndex)) {
			CurrentTracePoint.x++;
		  }
		  else {
			traceDirection = TRCU;
		  }
		}
	  }
	  break;
	}
	case TRCD: {
	  pixelIndex -= (BitmapWidth + 1U);
	  if (CurrentTracePoint.y == 0) {
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
		  if (TracedEdges->test(pixelIndex)) {
			CurrentTracePoint.y--;
		  }
		  else {
			traceDirection = TRCR;
		  }
		}
	  }
	  break;
	}
	case TRCL: {
	  pixelIndex += (BitmapWidth - 1);
	  if (CurrentTracePoint.x == 0) {
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
		  if (TracedEdges->test(pixelIndex)) {
			CurrentTracePoint.x--;
		  }
		  else {
			traceDirection = TRCD;
		  }
		}
	  }
	  break;
	}
	case TRCU: {
	  pixelIndex += (1 + BitmapWidth);
	  if (CurrentTracePoint.y == gsl::narrow<int32_t>(BitmapHeight) - 1) {
		traceDirection = TRCL;
	  }
	  else {
		if (TracedEdges->test(pixelIndex)) {
		  CurrentTracePoint.x++;
		  CurrentTracePoint.y++;
		  traceDirection = TRCR;
		}
		else {
		  pixelIndex--;
		  if (TracedEdges->test(pixelIndex)) {
			CurrentTracePoint.y++;
		  }
		  else {
			traceDirection = TRCL;
		  }
		}
	  }
	  break;
	}
	default: {
	}
  }
  if (tracedPoints.back().x != CurrentTracePoint.x || tracedPoints.back().y != CurrentTracePoint.y) {
	tracedPoints.push_back(
	    {gsl::narrow<int16_t>(CurrentTracePoint.x), gsl::narrow<int16_t>(CurrentTracePoint.y)});
	if (tracedPoints.size() >= 500000U) {
	  return false;
	}
  }
  return !(traceDirection == initialDirection && CurrentTracePoint.x == tracedPoints[0].x &&
           CurrentTracePoint.y == tracedPoints[0].y);
}

void trace::internal::dutdif(TRCPNT& traceDiff, TRCPNT const* point) noexcept {
  if (point != nullptr) {
	traceDiff.x = point[1].x - point[0].x;
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
	if (StateMap.test(StateFlag::LANDSCAP)) {
	  SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
	}
	CurrentTracePoint.x = wrap::round<int32_t>(BmpStitchRatio.x * SelectedPoint.x);
	CurrentTracePoint.y = wrap::round<int32_t>(BmpStitchRatio.y * SelectedPoint.y);
	if (CurrentTracePoint.x > gsl::narrow<int32_t>(BitmapWidth)) {
	  CurrentTracePoint.x = BitmapWidth;
	}
	if (CurrentTracePoint.y > gsl::narrow<int32_t>(BitmapHeight)) {
	  CurrentTracePoint.y = BitmapHeight;
	}
	auto const savedPoint     = CurrentTracePoint.y * BitmapWidth + CurrentTracePoint.x;
	auto       traceDirection = 0U;
	if (!TracedEdges->test(savedPoint)) {
	  auto findRectangle = RECT {0L, 0L, 0L, 0L};
	  auto point         = savedPoint;
	  auto limit         = (CurrentTracePoint.y + 1) * BitmapWidth;
	  while (point < limit && !TracedEdges->test(point)) {
		point++;
	  }
	  if (point < limit) {
		findRectangle.right = point - CurrentTracePoint.y * BitmapWidth;
	  }
	  else {
		findRectangle.right = BitmapWidth;
	  }
	  point = savedPoint;
	  limit = CurrentTracePoint.y * BitmapWidth;
	  while (point > limit && !TracedEdges->test(point)) {
		point--;
	  }
	  if (point == limit) {
		findRectangle.left = 0;
	  }
	  else {
		findRectangle.left = point - limit;
	  }
	  point = savedPoint;
	  while (point > 0 && !TracedEdges->test(point)) {
		if (point > BitmapWidth) {
		  point -= BitmapWidth;
		}
		else {
		  point = 0;
		}
	  }
	  if (point > 0) {
		findRectangle.bottom = point / BitmapWidth;
	  }
	  else {
		findRectangle.bottom = 0;
	  }
	  point = savedPoint;
	  limit = BitmapWidth * BitmapHeight;
	  while (point < limit && !TracedEdges->test(point)) {
		point += BitmapWidth;
	  }
	  if (point < limit) {
		findRectangle.top = point / BitmapWidth;
	  }
	  else {
		findRectangle.top = BitmapHeight;
	  }
	  auto flag                = 0U;
	  auto minimumEdgeDistance = 0x7fffffff;
	  if (findRectangle.left != 0) {
		minimumEdgeDistance = CurrentTracePoint.x - findRectangle.left;
		flag                = TRCL;
	  }
	  if (findRectangle.right < gsl::narrow<int32_t>(BitmapWidth)) {
		auto const edgeDistance = findRectangle.right - CurrentTracePoint.x;
		if (edgeDistance < minimumEdgeDistance) {
		  minimumEdgeDistance = edgeDistance;
		  flag                = TRCR;
		}
	  }
	  if (findRectangle.bottom != 0) {
		auto const edgeDistance = CurrentTracePoint.y - findRectangle.bottom;
		if (edgeDistance < minimumEdgeDistance) {
		  minimumEdgeDistance = edgeDistance;
		  flag                = TRCD;
		}
	  }
	  if (findRectangle.top < gsl::narrow<int32_t>(BitmapHeight)) {
		auto const edgeDistance = findRectangle.top - CurrentTracePoint.y;
		if (edgeDistance < minimumEdgeDistance) {
		  flag = TRCU;
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
	uint32_t const initialDirection = traceDirection;
	auto           tracedPoints     = std::vector<TRCPNT> {};
	tracedPoints.push_back(
	    {gsl::narrow<int16_t>(CurrentTracePoint.x), gsl::narrow<int16_t>(CurrentTracePoint.y)});
	while (ti::trcbit(initialDirection, traceDirection, tracedPoints)) {
	  ;
	}
	if (tracedPoints.size() >= 500000U) {
	  displayText::tabmsg(IDS_FRM2L);
	  return;
	}
#ifndef TESTTRC
	auto decimatedLine = std::vector<TRCPNT> {};
	decimatedLine.reserve(tracedPoints.size());
	TRCPNT traceDiff[2] = {};
	decimatedLine.push_back(tracedPoints[0]);
	ti::dutdif(traceDiff[0], tracedPoints.data());
	OutputIndex = 1;
	for (auto iPoint = 1U; iPoint < wrap::toUnsigned(tracedPoints.size()); iPoint++) {
	  traceDiff[1] = traceDiff[0];
	  ti::dutdif(traceDiff[0], &tracedPoints[iPoint]);
	  if (traceDiff[1].x != traceDiff[0].x || traceDiff[1].y != traceDiff[0].y) {
		decimatedLine.push_back(tracedPoints[iPoint]);
	  }
	}
	tracedPoints.clear();
	tracedPoints.reserve(decimatedLine.size());
	tracedPoints.push_back(decimatedLine[0]);
	auto iNextDec = 0U;
	for (auto iCurrent = 1U; iCurrent < wrap::toUnsigned(decimatedLine.size()); iCurrent++) {
	  auto const traceLength = hypot(decimatedLine[iCurrent].x - decimatedLine[iNextDec].x,
	                                 decimatedLine[iCurrent].y - decimatedLine[iNextDec].y);
	  if (traceLength > IniFile.traceLength) {
		tracedPoints.push_back(decimatedLine[iNextDec]);
		iNextDec = iCurrent;
	  }
	}
	for (auto iCurrent = iNextDec + 1U; iCurrent < wrap::toUnsigned(decimatedLine.size()); iCurrent++) {
	  tracedPoints.push_back(decimatedLine[iCurrent]);
	}
#endif
	FormList->push_back(FRMHED {});
	auto& form = FormList->back();
	form::frmclr(form);
	form.vertexIndex = gsl::narrow<decltype(form.vertexIndex)>(FormVertices->size());
	OutputIndex      = 0;
	FormVertices->push_back(fPOINT {gsl::narrow_cast<float>(tracedPoints[0].x) * StitchBmpRatio.x,
	                                gsl::narrow_cast<float>(tracedPoints[0].y) * StitchBmpRatio.y});
	OutputIndex++;
	auto iNext           = 0;
	auto traceLengthSum  = 0.0F;
	auto landscapeOffset = 0.0F;
	if (StateMap.test(StateFlag::LANDSCAP)) {
	  landscapeOffset = gsl::narrow_cast<float>(UnzoomedRect.y) - BitmapSizeinStitches.y;
	}
	for (auto iCurrent = 1U; iCurrent < wrap::toUnsigned(tracedPoints.size()); iCurrent++) {
	  traceLengthSum +=
	      hypotf(gsl::narrow_cast<float>(tracedPoints[iCurrent].x - tracedPoints[iCurrent - 1U].x),
	             gsl::narrow_cast<float>(tracedPoints[iCurrent].y - tracedPoints[iCurrent - 1U].y));
	  auto const traceLength =
	      hypotf(gsl::narrow_cast<float>(tracedPoints[iCurrent].x - tracedPoints[iNext].x),
	             gsl::narrow_cast<float>(tracedPoints[iCurrent].y - tracedPoints[iNext].y));
	  if (traceLengthSum > traceLength * IniFile.traceRatio) {
		FormVertices->push_back(fPOINT {
		    gsl::narrow_cast<float>(tracedPoints[iCurrent - 1U].x) * StitchBmpRatio.x,
		    gsl::narrow_cast<float>(tracedPoints[iCurrent - 1U].y) * StitchBmpRatio.y + landscapeOffset});
		OutputIndex++;
		iCurrent--;
		iNext          = iCurrent;
		traceLengthSum = 0.0;
	  }
	}
	form.vertexCount = gsl::narrow<decltype(form.vertexCount)>(OutputIndex);
	form.type        = FRMFPOLY;
	form.attribute   = gsl::narrow<uint8_t>(ActiveLayer << 1U);
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	form.satinGuideCount = 0;
	StateMap.set(StateFlag::RESTCH);
	StateMap.set(StateFlag::FRMOF);
	form::tglfrm();
  }
}

void trace::trinit() {
  uint32_t histogramData[3][256] = {{0}};

  if (PCSBMPFileName[0] != 0) {
	if (!StateMap.test(StateFlag::TRSET)) {
	  StateMap.set(StateFlag::TRCRED);
	  StateMap.set(StateFlag::TRCGRN);
	  StateMap.set(StateFlag::TRCBLU);
	  uint32_t componentPeak[3] = {0U};
	  if (!StateMap.test(StateFlag::WASTRAC)) {
		ti::getrmap();
	  }
	  if (StateMap.test(StateFlag::MONOMAP)) {
		auto color  = gsl::narrow<COLORREF>(TraceBitmapData[0]);
		auto iPixel = 0U;
		for (; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
		  if (TraceBitmapData[iPixel] != color) {
			break;
		  }
		}
		if (iPixel < BitmapWidth * BitmapHeight) {
		  ti::trcols(TraceBitmapData[iPixel]);
		  HighColors[0] = PixelColors[0];
		  HighColors[1] = PixelColors[1];
		  HighColors[2] = PixelColors[2];
		  ti::trcols(color);
		  for (auto iRGB = 0U; iRGB < 3; iRGB++) {
			if (PixelColors[iRGB] > HighColors[iRGB]) {
			  auto const swapComponent = PixelColors[iRGB];
			  PixelColors[iRGB]        = HighColors[iRGB];
			  HighColors[iRGB]         = swapComponent;
			}
			componentPeak[iRGB] = ((HighColors[iRGB] - PixelColors[iRGB]) / 2) + PixelColors[iRGB];
		  }
		}
	  }
	  else {
		for (auto iPixel = 0U; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
		  ti::trcols(TraceBitmapData[iPixel]);
		  for (auto iRGB = 0U; iRGB < 3; iRGB++) {
			histogramData[iRGB][PixelColors[iRGB]]++;
		  }
		}
		uint32_t componentPeakCount[3] = {0U};
		for (auto iLevel = 0U; iLevel < 256U; iLevel++) {
		  for (auto iRGB = 0U; iRGB < 3U; iRGB++) {
			if (histogramData[iRGB][iLevel] > componentPeakCount[iRGB]) {
			  componentPeakCount[iRGB] = histogramData[iRGB][iLevel];
			  componentPeak[iRGB]      = iLevel;
			}
		  }
		}
	  }
	  InvertDownColor = 0U;
	  for (auto iRGB = 0U; iRGB < 3U; iRGB++) {
		if (componentPeak[iRGB] != 0U) {
		  componentPeak[iRGB]--;
		}
		InvertDownColor |= componentPeak[iRGB] << TraceShift[iRGB];
	  }
	  DownPixelColor = InvertDownColor ^ 0xffffffU;
	  InvertUpColor  = 0xffffffU;
	  UpPixelColor   = 0U;
	}
	StateMap.set(StateFlag::WASTRCOL);
	trace::trace();
  }
  else {
	displayText::tabmsg(IDS_MAPLOD);
  }
}

void trace::trcsel() {
  if (PCSBMPFileName[0] != 0) {
	StateMap.set(StateFlag::WASTRCOL);
	StateMap.set(StateFlag::TRCRED);
	StateMap.set(StateFlag::TRCBLU);
	StateMap.set(StateFlag::TRCGRN);
	DownPixelColor = 0xffffff;
	UpPixelColor   = 0;
	trace::trace();
	StateMap.reset(StateFlag::HIDMAP);
	StateMap.reset(StateFlag::TRSET);
	for (auto iPixel = 0U; iPixel < BitmapWidth * BitmapHeight; iPixel++) {
	  ti::trcols(TraceBitmapData[iPixel]);
	  auto maximumColorComponent = PixelColors[0];
	  auto iRGB                  = 2U;
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

void trace::internal::ritrcol(COLORREF* color, uint32_t number) noexcept {
  if (color != nullptr) {
	*color &= TraceRGBMask[ColumnColor];
	number &= 0xffU;
	*color |= (number << TraceShift[ColumnColor]);
  }
}

void trace::internal::dutrnum0(uint32_t color) {
  StateMap.reset(StateFlag::NUMIN);
  StateMap.reset(StateFlag::TRNIN0);
  if (StateMap.test(StateFlag::TRNUP)) {
	ti::ritrcol(&InvertUpColor, color);
	UpPixelColor = InvertUpColor ^ 0xffffffU;
	thred::redraw(TraceUpWindow[ColumnColor]);
  }
  else {
	ti::ritrcol(&InvertDownColor, color);
	DownPixelColor = InvertDownColor ^ 0xffffffU;
	thred::redraw(TraceDownWindow[ColumnColor]);
  }
  thred::redraw(TraceControlWindow[ColumnColor]);
  DestroyWindow(TraceNumberInput);
  StateMap.set(StateFlag::WASTRCOL);
  trace::trace();
}

void trace::dutrnum2() {
  ti::dutrnum0(std::stoi(std::begin(TraceInputBuffer)));
}

void trace::dutrnum1() {
  DestroyWindow(GeneralNumberInputBox);
  StateMap.reset(StateFlag::NUMIN);
  StateMap.reset(StateFlag::TRNIN1);
  auto traceLength = wrap::bufToFloat(std::begin(MsgBuffer));
  if (traceLength > 9.0F) {
	traceLength = 9.0F;
  }
  if (StateMap.test(StateFlag::TRNUP)) {
	IniFile.traceLength = traceLength * PFGRAN;
	ti::trcstpnum();
  }
  else {
	IniFile.traceRatio = 1.0F + pow(0.1F, traceLength);
	ti::trcratnum();
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) auto trace::internal::ducolm() -> uint32_t {
  if (TraceMsgPoint.x < gsl::narrow<int32_t>(ButtonWidth)) {
	return 0U;
  }
  if (TraceMsgPoint.x < gsl::narrow<int32_t>(ButtonWidth) * 2) {
	return 1U;
  }

  return 2U;
}

void trace::internal::trnumwnd0(int32_t position) noexcept {
  TraceNumberInput = CreateWindow(L"STATIC", // NOLINT
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

void trace::internal::trnumwnd1(int32_t position) noexcept {
  GeneralNumberInputBox = CreateWindow(L"STATIC", // NOLINT
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
  if (StateMap.test(StateFlag::LANDSCAP)) {
	point.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
  }
  BitmapPoint.x = wrap::round<int32_t>(BmpStitchRatio.x * point.x);
  BitmapPoint.y = wrap::round<int32_t>(BitmapHeight - BmpStitchRatio.y * point.y);
}

void trace::internal::pxlin(FRMHED const& form, uint32_t start, uint32_t finish) {
  POINT line[2];

  auto vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
  ti::stch2bit(vertexIt[start]);
  line[0] = BitmapPoint;
  ti::stch2bit(vertexIt[finish]);
  line[1] = BitmapPoint;
  Polyline(BitmapDC, static_cast<POINT const*>(line), 2);
  Polyline(TraceDC, static_cast<POINT const*>(line), 2);
}

void trace::internal::bfrm(FRMHED const& form) {
  if (form.vertexCount != 0U) {
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; iVertex++) {
	  ti::pxlin(form, iVertex, iVertex + 1U);
	}
	if (form.type != FRMLINE) {
	  ti::pxlin(form, form.vertexCount - 1U, 0);
	}
  }
}

void trace::blak() {
  if (PCSBMPFileName[0] == 0) {
	displayText::tabmsg(IDS_MAPLOD);
	return;
  }
  ti::tracwnd();
  if (!FormList->empty()) {
	BlackPen = wrap::CreatePen(PS_SOLID, 1, 0);
	SelectObject(BitmapDC, BlackPen);
	SelectObject(TraceDC, BlackPen);
	if (!StateMap.test(StateFlag::WASTRAC)) {
	  ti::getrmap();
	}
	for (auto iForm : *FormList) {
	  ti::bfrm(iForm);
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
  if (StateMap.test(StateFlag::TRNIN0)) {
	trace::dutrnum2();
  }
  if (StateMap.test(StateFlag::TRNIN1)) {
	trace::dutrnum1();
  }
  TraceMsgPoint.x = Msg.pt.x - ThredWindowOrigin.x;
  TraceMsgPoint.y = Msg.pt.y - ThredWindowOrigin.y;
  if (TraceMsgPoint.x > ButtonWidthX3) {
	ti::dutrac();
  }
  else {
	ColumnColor = ti::ducolm();
	if (TraceMsgPoint.y < ButtonHeight * 15) {
	  do {
		if (wrap::pressed(VK_SHIFT)) {
		  UpPixelColor &= TraceRGBMask[ColumnColor];
		  DownPixelColor |= TraceRGB[2 - ColumnColor];
		  break;
		}
		auto const ratio    = (TraceMsgPoint.y) / (ButtonHeight * 15.0);
		auto const position = wrap::floor<uint32_t>(ratio * 255.0);
		auto traceColor     = gsl::narrow_cast<COLORREF>(UpPixelColor & TraceRGB[2 - ColumnColor]);
		auto const tracePosition = gsl::narrow_cast<COLORREF>(position << TraceShift[ColumnColor]);
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
	  auto const position = wrap::floor<uint32_t>(TraceMsgPoint.y / ButtonHeight);
	  if (position < 16U) {
		StateMap.flip(TraceRGBFlag[ColumnColor]);
		thred::redraw(TraceSelectWindow[ColumnColor]);
		trace::trace();
	  }
	  else {
		if (position < 18U) {
		  StateMap.set(StateFlag::NUMIN);
		  StateMap.set(StateFlag::TRNIN0);
		  MsgIndex            = 0;
		  TraceInputBuffer[0] = 0;
		  if (position < 17U) {
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
			MsgIndex            = 0;
			TraceInputBuffer[0] = 0;
			if (position < 19U) {
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
			  case 20U: {
				trace::trdif();
				break;
			  }
			  case 21U: {
				thred::hidbit();
				break;
			  }
			  case 22U: {
				trace::blak();
				break;
			  }
			  case 23U: {
				trace::trcsel();
				break;
			  }
			  case 24U: {
				trace::tracedg();
				break;
			  }
			  default: {
			  }
			}
		  }
		}
	  }
	}
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void trace::internal::trcnum(uint32_t shift, COLORREF color, uint32_t iRGB) {
  uint32_t const NumeralWidth = thred::txtWid(L"0");

  wchar_t buffer[11] = {0};

  color >>= shift;
  color &= 0xffU;
  _itow_s(color, buffer, 10);
  auto const bufferLength = gsl::narrow<uint32_t>(wcslen(std::begin(buffer)));
  auto const xPosition    = NumeralWidth * (3U - bufferLength) + 1U;
  SetBkColor(DrawItem->hDC, TraceRGB[iRGB]);
  wrap::TextOut(DrawItem->hDC, xPosition, 1, static_cast<LPCTSTR>(buffer), bufferLength);
}

void trace::internal::upnum(uint32_t iRGB) {
  ti::trcnum(TraceShift[iRGB], InvertUpColor, iRGB);
}

void trace::internal::dwnum(uint32_t iRGB) {
  ti::trcnum(TraceShift[iRGB], InvertDownColor, iRGB);
}

void trace::internal::durct(uint32_t    shift,
                            RECT const& traceControlRect,
                            RECT&       traceHighMask,
                            RECT&       traceMiddleMask,
                            RECT&       traceLowMask) {
  auto const lowerColor    = (UpPixelColor >> shift) & 0xffU;
  auto const upperColor    = (DownPixelColor >> shift) & 0xffU;
  auto const controlHeight = traceControlRect.bottom - traceControlRect.top;

  traceHighMask.left = traceLowMask.left = traceMiddleMask.left = traceControlRect.left;
  traceHighMask.right = traceLowMask.right = traceMiddleMask.right = traceControlRect.right;

  auto ratio             = gsl::narrow_cast<double>(lowerColor) / 255.0;
  traceMiddleMask.top    = wrap::round<int32_t>(controlHeight * ratio + traceControlRect.top);
  ratio                  = gsl::narrow_cast<double>(upperColor) / 255.0;
  traceMiddleMask.bottom = wrap::round<int32_t>(controlHeight * ratio + traceControlRect.top);
  StateMap.reset(StateFlag::DUHI);
  StateMap.reset(StateFlag::DULO);
  if (lowerColor != 0U) {
	StateMap.set(StateFlag::DULO);
	traceLowMask.bottom = traceMiddleMask.top;
	traceLowMask.top    = 0;
  }
  if (upperColor != 255U) {
	StateMap.set(StateFlag::DUHI);
	traceHighMask.top    = traceMiddleMask.bottom;
	traceHighMask.bottom = traceControlRect.bottom;
  }
}

void trace::internal::dublk(HDC dc, RECT const& traceHighMask, RECT const& traceLowMask, HBRUSH brush) {
  if (StateMap.test(StateFlag::DUHI)) {
	FillRect(dc, &traceHighMask, brush);
  }
  if (StateMap.test(StateFlag::DULO)) {
	FillRect(dc, &traceLowMask, brush);
  }
}

void trace::wasTrace() {
  auto       traceHighMaskRect   = RECT {0L, 0L, 0L, 0L}; // high trace mask rectangle
  auto       traceMiddleMaskRect = RECT {0L, 0L, 0L, 0L}; // middle trace mask rectangle
  auto       traceLowMaskRect    = RECT {0L, 0L, 0L, 0L}; // low trace mask rectangle
  auto const BlackBrush          = CreateSolidBrush(0);   // black brush

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
	  wchar_t buffer[10] = {0}; // for integer to string conversion
	  wcscpy_s(buffer, StringTable->operator[](STR_OFF).c_str());
	  SetBkColor(DrawItem->hDC, 0);
	  SetTextColor(DrawItem->hDC, TraceRGB[iRGB]);
	  if (StateMap.test(TraceRGBFlag[iRGB])) {
		TempBrush = TraceBrush[iRGB];
		wcscpy_s(buffer, StringTable->operator[](STR_ON).c_str());
		SetTextColor(DrawItem->hDC, 0);
		SetBkColor(DrawItem->hDC, TraceRGB[iRGB]);
	  }
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, TempBrush);
	  wrap::TextOut(
	      DrawItem->hDC, 1, 1, static_cast<LPCTSTR>(buffer), gsl::narrow<uint32_t>(wcslen(std::begin(buffer))));
	  break;
	}
	if (DrawItem->hwndItem == TraceNumberInput) {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[ColumnColor]);
	  SetBkColor(DrawItem->hDC, TraceRGB[ColumnColor]);
	  wrap::TextOut(DrawItem->hDC,
	                1,
	                1,
	                static_cast<LPCTSTR>(TraceInputBuffer),
	                gsl::narrow<uint32_t>(wcslen(std::begin(TraceInputBuffer))));
	  break;
	}
  }
}

void trace::wasTrace1() {
  if (StateMap.test(StateFlag::TRNIN0)) {
	trace::dutrnum2();
  }
  if (StateMap.test(StateFlag::TRNIN1)) {
	trace::dutrnum1();
  }
  if (!StateMap.test(StateFlag::WASEDG)) {
	trace::tracpar();
  }
}

void trace::traceNumberInput(uint32_t NumericCode) {
  TraceInputBuffer[MsgIndex++] = NumericCode;
  TraceInputBuffer[MsgIndex]   = 0;
  auto traceColor              = std::stoi(std::begin(TraceInputBuffer));
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
	default: {
	}
  }
  thred::redraw(TraceNumberInput);
}

void trace::traceNumberReset() noexcept {
  TraceInputBuffer[MsgIndex] = 0;
  thred::redraw(TraceNumberInput);
}
