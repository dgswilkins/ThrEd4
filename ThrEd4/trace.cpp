// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "bitmap.h"
#include "displayText.h"
#include "EnumMap.h"
#include "form.h"
#include "formHeader.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "Resources/resource.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "trace.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "gsl/narrow"
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

constexpr auto ADJCOUNT = uint32_t {9U}; // including the center pixel there are 9 pixels immediately adjacent
constexpr auto BLUCOL   = uint32_t {0xff0000U}; // code for the color blue
constexpr auto BLUMSK   = uint32_t {0x00ffffU}; // mask for the color blue
constexpr auto BYTEMAXV = uint8_t {255U};       // max color value in a byte wide counter
constexpr auto CCRATIO  = 1.0F / 255.0F;        // This is used to convert 0-255 to 0-1
constexpr auto CHANLCNT = uint32_t {3U};        // number of color channels i.e. RGB
constexpr auto GRNCOL   = uint32_t {0x00ff00U}; // code for the color green
constexpr auto GRNMSK   = uint32_t {0xff00ffU}; // mask for the color green
constexpr auto LEVELCNT = uint32_t {256U};      // number of color levels in a byte wide counter
constexpr auto POINTMAX = size_t {500000U};     // maximum number of trace points to consider
constexpr auto REDCOL   = uint32_t {0x0000ffU}; // code for the color red
constexpr auto REDMSK   = uint32_t {0xffff00U}; // mask for the color red
constexpr auto TRBASE   = 0.1F;                 // Trace ratio base
constexpr auto TROFF    = 1.0F;                 // Trace ratio offset

enum TraceWin : int8_t {
  TRWINROW01 = 15,
  TRWINROW02,
  TRWINROW03,
  TRWINROW04,
  TRWINROW05,
  TRWINROW06,
  TRWINROW07,
  TRWINROW08,
  TRWINROW09,
  TRWINROW10
};

// edge tracing directions
enum TraceDirection : uint8_t {
  TRCU, // top edge
  TRCR, // right edge
  TRCD, // bottom edge
  TRCL  // left edge
};

class TRACE_PNT
{
  public:
  int16_t x {};
  int16_t y {};
};

// trace internal namespace
namespace ti {
void decForm(std::vector<TRACE_PNT>& src, std::vector<F_POINT>& dst);
void decLen(std::vector<TRACE_PNT>& src, std::vector<TRACE_PNT>& dst);
void decSlope(std::vector<TRACE_PNT>& src, std::vector<TRACE_PNT>& dst);
void difbits(uint32_t shift, gsl::details::span_iterator<uint32_t> point) noexcept;

inline void difsub(uint32_t source, uint32_t shift, uint32_t& destination) noexcept;

void dublk(HDC hDC, RECT const& traceHighMask, RECT const& traceLowMask, HBRUSH brush);
auto ducolm() -> uint32_t;
void durct(uint32_t shift, RECT const& traceControlRect, RECT& traceHighMask, RECT& traceMiddleMask, RECT& traceLowMask);
void dutrac();
void dutrnum0(uint32_t colNum);
void getColors();
void hidwnd(HWND hwnd) noexcept;
void hideTraceWin() noexcept;
void ritrcol(COLORREF& colRef, uint32_t colNum) noexcept;
void shownd(HWND hwnd) noexcept;
void showTraceWin() noexcept;
void tracwnd();
auto trcbit(uint32_t initialDirection, uint32_t& traceDirection, std::vector<TRACE_PNT>& tracedPoints) -> bool;
auto trcin(COLORREF color) -> bool;
void trcnum(uint32_t shift, COLORREF color, uint32_t backColor);
auto trcols(COLORREF color) noexcept -> std::array<uint32_t, CHANLCNT>;
void trcratnum();
void trcstpnum();
auto trcsub(int32_t xCoordinate, int32_t yCoordinate, int32_t buttonHeight) -> HWND;
void trnumwnd0(int32_t position) noexcept;
void trnumwnd1(int32_t position) noexcept;
auto trsum() -> uint32_t;

#if TRCMTH == 0
auto colsum(COLORREF col) -> uint32_t;
auto icolsum(COLORREF col) -> uint32_t;
#endif
} // namespace ti

namespace {
auto TraceControlWindow = std::array<HWND, CHANLCNT> {}; // trace control windows
auto TraceDownWindow    = std::array<HWND, CHANLCNT> {}; // trace down number windows
auto TraceSelectWindow  = std::array<HWND, CHANLCNT> {}; // trace select windows
auto TraceUpWindow      = std::array<HWND, CHANLCNT> {}; // trace up number windows
auto CurrentTracePoint  = POINT {};                      // current point being traced
auto TraceDataSize      = uint32_t {};                   // size of the trace bitmap in double words
auto TraceStepWin       = gsl::narrow_cast<HWND>(nullptr); // trace stepSize window
auto TraceRGBFlag = std::array {StateFlag::TRCRED, StateFlag::TRCGRN, StateFlag::TRCBLU}; // trace bits
auto TraceRGBMask = std::array {REDMSK, GRNMSK, BLUMSK};      // trace masks
auto TraceRGB     = std::array {BLUCOL, GRNCOL, REDCOL};      // trace colors
auto TraceAdjacentColors = std::array<uint32_t, ADJCOUNT> {}; // separated colors for adjacent pixels
auto TraceInputBuffer = std::array<wchar_t, 4> {};            // for user input color numbers
auto TraceMsgIndex    = uint32_t {};                          // pointer to the trace buffer
auto UpPixelColor     = COLORREF {};                          // color of the up reference pixel
auto DownPixelColor   = COLORREF {};                          // color of the down reference pixel
auto InvertUpColor    = COLORREF {}; // complement color of the up reference pixel
auto InvertDownColor  = COLORREF {}; // complement color of the down reference pixel
auto TraceMsgPoint    = POINT {};    // message point for trace parsing
auto HighColors       = std::array<uint32_t, CHANLCNT> {}; // separated upper reference colors
auto LowColors        = std::array<uint32_t, CHANLCNT> {}; // separated lower reference colors
auto ColumnColor      = uint32_t {};                       // trace color column
auto TraceShift       = std::array {0U, BYTSHFT, WRDSHFT}; // trace shift values
auto TraceBrush       = std::array<HBRUSH, CHANLCNT> {};   // red,green,and blue brushes
auto TraceNumberInput = gsl::narrow_cast<HWND>(nullptr);   // trace number input window
auto BlackPen         = gsl::narrow_cast<HPEN>(nullptr);   // black pen
} // namespace

void trace::initColorRef() noexcept {
  UpPixelColor    = 0;
  DownPixelColor  = 0x7f7f7f; // light DimGray NOLINT(readability-magic-numbers)
  InvertUpColor   = PENWHITE;
  InvertDownColor = PENGRAY;
}

auto ti::trcsub(int32_t const xCoordinate, int32_t const yCoordinate, int32_t const buttonHeight) -> HWND {
  constexpr auto DW_STYLE = DWORD {SS_OWNERDRAW | WS_CHILD | WS_BORDER};
  // NOLINTNEXTLINE(readability-qualified-auto)
  if (auto const window = CreateWindowEx(
          0L, L"STATIC", L"", DW_STYLE, xCoordinate, yCoordinate, ButtonWidth, buttonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
      nullptr != window) {
	return window;
  }
  throw std::runtime_error("No window created in trcsub");
}

void trace::initTraceWindows() {
  constexpr auto DW_STYLE = DWORD {SS_NOTIFY | SS_CENTER | WS_CHILD | WS_BORDER};
  TraceStepWin            = CreateWindowEx(
      0L, L"STATIC", L"", DW_STYLE, 0, ButtonHeight * TRWINROW04, ButtonWidthX3, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
  auto iTraceControlWindow = TraceControlWindow.begin();
  auto iTraceDownWindow    = TraceDownWindow.begin();
  auto iTraceSelectWindow  = TraceSelectWindow.begin();
  auto iTraceUpWindow      = TraceUpWindow.begin();
  auto iTraceBrush         = TraceBrush.begin();
  auto iTraceRGB           = TraceRGB.begin();
  for (auto iRGB = 0U; iRGB < TraceControlWindow.size(); ++iRGB) {
	auto const channel     = gsl::narrow_cast<int32_t>(iRGB);
	*iTraceControlWindow++ = ti::trcsub(ButtonWidth * channel, 0, ButtonHeight * TRWINROW01);
	*iTraceSelectWindow++ = ti::trcsub(ButtonWidth * channel, ButtonHeight * TRWINROW01, ButtonHeight);
	*iTraceUpWindow++ = ti::trcsub(ButtonWidth * channel, ButtonHeight * TRWINROW02, ButtonHeight);
	*iTraceDownWindow++ = ti::trcsub(ButtonWidth * channel, ButtonHeight * TRWINROW03, ButtonHeight);
	*iTraceBrush++ = CreateSolidBrush(*iTraceRGB++);
  }
}

auto ti::trcols(COLORREF const color) noexcept -> std::array<uint32_t, CHANLCNT> {
  auto colors = std::array<uint32_t, CHANLCNT> {};
  colors[0]   = color & B1MASK;
  colors[1]   = (color & B2MASK) >> BYTSHFT;
  colors[2]   = (color & B3MASK) >> WRDSHFT;
  return colors;
}

void ti::trcstpnum() {
  auto const fmtStr = displayText::format(IDS_TRCSTP, IniFile.traceLength * IPFGRAN);
  SetWindowText(TraceStepWin, fmtStr.c_str());
}

void ti::trcratnum() {
  constexpr auto HLIN   = uint32_t {HNUM};
  auto const     fmtStr = displayText::format(IDS_TRCRAT, -log10(IniFile.traceRatio - TROFF));
  displayText::butxt(HLIN, fmtStr);
}

auto ti::trcin(COLORREF const color) -> bool {
  if (color == 0U) {
	return false;
  }
  auto const colors = trcols(color);
  if (Instance->StateMap.test(StateFlag::TRCRED)) {
	if (colors[0] > HighColors[0]) {
	  return false;
	}
	if (colors[0] < LowColors[0]) {
	  return false;
	}
  }
  if (Instance->StateMap.test(StateFlag::TRCGRN)) {
	if (colors[1] > HighColors[1]) {
	  return false;
	}
	if (colors[1] < LowColors[1]) {
	  return false;
	}
  }
  if (Instance->StateMap.test(StateFlag::TRCBLU)) {
	if (colors[2] > HighColors[2]) {
	  return false;
	}
	if (colors[2] < LowColors[2]) {
	  return false;
	}
  }
  return true;
}

// ReSharper disable CppParameterMayBeConst
void ti::shownd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_SHOW);
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void ti::hidwnd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_HIDE);
}
// ReSharper restore CppParameterMayBeConst

void ti::showTraceWin() noexcept {
  auto iTraceSelectWindow = TraceSelectWindow.begin();
  auto iTraceUpWindow     = TraceUpWindow.begin();
  auto iTraceDownWindow   = TraceDownWindow.begin();
  for (auto const& iTraceControlWindow : TraceControlWindow) {
	shownd(iTraceControlWindow);
	shownd(*iTraceSelectWindow++);
	shownd(*iTraceUpWindow++);
	shownd(*iTraceDownWindow++);
  }
}

void ti::tracwnd() {
  thred::hideColorWin();
  hidwnd(Instance->ButtonWin.operator[](HBOXSEL));
  hidwnd(Instance->ButtonWin.operator[](HUPTO));
  shownd(TraceStepWin);
  trcstpnum();
  trcratnum();
  displayText::clrhbut(4);
  showTraceWin();
}

// Check Translation
inline void ti::difsub(uint32_t const source, uint32_t const shift, uint32_t& destination) noexcept {
  destination = (source >> (shift & NIBMASK)) & BYTMASK;
}

// pixel map for adjacent pixels
//  213
//  504
//  678

void ti::difbits(uint32_t const shift, gsl::details::span_iterator<uint32_t> point) noexcept {
  auto iTrcAdjClrs = TraceAdjacentColors.begin();
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 0 - center
  std::advance(point, -bitmap::getBitmapWidth());
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 1 - N
  std::advance(point, -1);
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 2 - NW
  std::advance(point, 2);
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 3 - NE
  std::advance(point, bitmap::getBitmapWidth());
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 4 - E
  std::advance(point, -2);
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 5 - W
  std::advance(point, bitmap::getBitmapWidth());
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 6 - SW
  std::advance(point, 1);
  difsub(*point, shift, *iTrcAdjClrs++); // pixel 7 - S
  std::advance(point, 1);
  difsub(*point, shift, *iTrcAdjClrs); // pixel 8 - SE
}

auto ti::trsum() -> uint32_t {
  auto const firstColor = TraceAdjacentColors.front();
  auto const iBegin     = std::next(TraceAdjacentColors.begin());
  auto const iEnd       = std::next(TraceAdjacentColors.end(), -1);
  auto const fold       = [firstColor](uint32_t const varA, uint32_t const varB) {
    return varA + (varB > firstColor ? varB - firstColor : firstColor - varB);
  };
  return wrap::toUnsigned(std::accumulate(iBegin, iEnd, 0, fold));
}

void ti::hideTraceWin() noexcept {
  auto iTraceSelectWindow = TraceSelectWindow.begin();
  auto iTraceUpWindow     = TraceUpWindow.begin();
  auto iTraceDownWindow   = TraceDownWindow.begin();
  for (auto const& iTraceControlWindow : TraceControlWindow) {
	hidwnd(iTraceControlWindow);
	hidwnd(*iTraceSelectWindow++);
	hidwnd(*iTraceUpWindow++);
	hidwnd(*iTraceDownWindow++);
  }
}

void trace::untrace() {
  if (!Instance->StateMap.testAndReset(StateFlag::WASTRAC)) {
	if (Instance->StateMap.test(StateFlag::TRCUP)) {
	  DownPixelColor = PENWHITE;
	}
	else {
	  UpPixelColor = 0;
	}
	return;
  }
  bitmap::resetDC();
  if (!TracedEdges->empty()) {
	TracedEdges->resize(0); // allocated in tracedg
  }
  if (!TracedMap->empty()) {
	TracedMap->resize(0); // allocated in trace
  }
  Instance->StateMap.reset(StateFlag::WASEDG);
  ti::hideTraceWin();
  ti::hidwnd(TraceStepWin);
  thred::showColorWin();
  for (auto const& iButton : Instance->ButtonWin) {
	ti::shownd(iButton);
  }
}

void trace::trdif() {
  if (!bitmap::ismap()) {
	displayText::tabmsg(IDS_MAPLOD, false);
	return;
  }
  Instance->StateMap.reset(StateFlag::TRSET);
  Instance->StateMap.reset(StateFlag::HIDMAP);
  untrace();
  auto const bitmapSize = bitmap::getBitmapHeight() * bitmap::getBitmapWidth();
  if (bitmapSize == 0U) {
	return;
  }
  auto differenceBitmap = std::vector<uint32_t> {};
  differenceBitmap.resize(wrap::toSize(bitmapSize));
  auto colorSumMaximum = 0U;
  auto colorSumMinimum = BIGUINT;
  if (!Instance->StateMap.test(StateFlag::WASTRAC)) {
	TraceDataSize = bitmap::getrmap();
  }
  auto const spTBD = gsl::span(TraceBitmapData, wrap::toSize(bitmapSize));
  for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	for (auto iHeight = 1; iHeight < bitmap::getBitmapHeight() - 1; ++iHeight) {
	  auto iPoint = iHeight * bitmap::getBitmapWidth();
	  ++iPoint;
	  for (auto iWidth = 1; iWidth < bitmap::getBitmapWidth() - 1; ++iWidth) {
		ti::difbits(TraceShift.at(iRGB), std::next(spTBD.begin(), iPoint));
		differenceBitmap[wrap::toSize(iPoint)] = ti::trsum();

		auto const& colorSum = differenceBitmap[wrap::toSize(iPoint++)];

		colorSumMaximum = std::max(colorSumMaximum, colorSum);
		colorSumMinimum = std::min(colorSumMinimum, colorSum);
	  }
	}
	auto const ratio = 255.0F / wrap::toFloat(colorSumMaximum - colorSumMinimum);
	auto       pos   = size_t {0U};
	for (auto& iPixel : spTBD) {
	  iPixel &= TraceRGBMask.at(iRGB);
	  if (differenceBitmap[pos] != 0U) {
		auto const adjustedColorSum =
		    wrap::round<uint32_t>(wrap::toFloat(differenceBitmap[pos] - colorSumMinimum) * ratio);
		iPixel |= adjustedColorSum << TraceShift.at(iRGB);
	  }
	  ++pos;
	}
  }
  bitmap::bitbltBitmap();
  Instance->StateMap.set(StateFlag::WASDIF);
  Instance->StateMap.set(StateFlag::RESTCH);
  ti::tracwnd();
}

#if TRCMTH == 0
auto ti::colsum(COLORREF col) -> uint32_t {
  auto colors   = ti::trcols(col);
  auto itColor  = colors.begin();
  auto colorSum = 0U;
  for (auto iRGB : TraceRGBFlag) {
	if (Instance->StateMap.test(iRGB)) {
	  colorSum += *itColor;
	}
	++itColor;
  }
  return colorSum;
}

auto ti::icolsum(COLORREF col) -> uint32_t {
  auto colors   = ti::trcols(col);
  auto itColor  = colors.begin();
  auto colorSum = 0U;

  constexpr auto RGBMAX = 255U;
  for (auto iRGB : TraceRGBFlag) {
	if (Instance->StateMap.test(iRGB)) {
	  colorSum += RGBMAX - *itColor;
	}
	++itColor;
  }
  return colorSum;
}
#endif

void trace::trace() {
  if (!bitmap::ismap()) {
	displayText::tabmsg(IDS_MAPLOD, false);
	return;
  }
  untrace();
  ti::tracwnd();
  TraceDataSize = bitmap::getrmap();
  auto const spTBD =
      gsl::span(TraceBitmapData, wrap::toSize(bitmap::getBitmapHeight() * bitmap::getBitmapWidth()));
  if (thred::inStitchWin() && !Instance->StateMap.testAndReset(StateFlag::WASTRCOL)) {
	auto stitchPoint = thred::pxCor2stch(WinMsg.pt);
	if (Instance->StateMap.test(StateFlag::LANDSCAP)) {
	  auto const bmpSiS = bitmap::getBitmapSizeinStitches();
	  stitchPoint.y -= wrap::toFloat(UnzoomedRect.cy) - bmpSiS.y;
	}
	auto const bmpSR = bitmap::getBmpStitchRatio();
	auto const bitmapPoint =
	    POINT {std::lround(bmpSR.x * stitchPoint.x), std::lround((bmpSR.y * stitchPoint.y) - 1.0F)};

	auto const color = spTBD[wrap::toSize((bitmapPoint.y * bitmap::getBitmapWidth()) + bitmapPoint.x)] ^ 0xffffffU;
	if (Instance->StateMap.test(StateFlag::TRCUP)) {
	  UpPixelColor   = color;
	  DownPixelColor = PENWHITE;
	}
	else {
	  DownPixelColor = color;
	  UpPixelColor   = 0;
	}
	Instance->StateMap.set(StateFlag::TRCRED);
	Instance->StateMap.set(StateFlag::TRCGRN);
	Instance->StateMap.set(StateFlag::TRCBLU);
  }
  auto traceColorMask = COLMASK;
  if (!Instance->StateMap.test(StateFlag::TRCRED)) {
	traceColorMask &= REDMSK;
  }
  if (!Instance->StateMap.test(StateFlag::TRCGRN)) {
	traceColorMask &= GRNMSK;
  }
  if (!Instance->StateMap.test(StateFlag::TRCBLU)) {
	traceColorMask &= BLUMSK;
  }
  if (traceColorMask != COLMASK) {
	for (auto& iPixel : spTBD) {
	  iPixel &= traceColorMask;
	}
  }

#if TRCMTH == 0
  auto const upBrightness   = ti::icolsum(UpPixelColor);
  auto const downBrightness = ti::icolsum(DownPixelColor);
  if (TracedMap->empty()) {
	TracedMap->resize(TraceDataSize, false);
  }
  auto const lastIndex = wrap::toUnsigned(bitmap::getBitmapWidth() * bitmap::getBitmapHeight());
  auto       itTBD     = spTBD.begin();
  for (auto index = 0U; index < lastIndex; ++index) {
	auto const pointBrightness = ti::colsum(*itTBD);
	if (upBrightness > pointBrightness && downBrightness < pointBrightness) {
	  TracedMap->set(index);
	}
	else {
	  *itTBD = 0;
	}
	++itTBD;
  }
#endif

#if TRCMTH == 1
  InvertUpColor   = UpPixelColor ^ COLMASK;
  InvertDownColor = DownPixelColor ^ COLMASK;
  auto colors     = ti::trcols(InvertUpColor);
  for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	HighColors.at(iRGB) = colors.at(iRGB);
  }
  colors = ti::trcols(InvertDownColor);
  for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	LowColors.at(iRGB) = colors.at(iRGB);
  }
  if (TracedMap->empty()) {
	TracedMap->resize(TraceDataSize, false);
  }
  auto pos = size_t {0U};
  for (auto& iPixel : spTBD) {
	if (ti::trcin(iPixel)) {
	  TracedMap->set(pos);
	}
	else {
	  iPixel = 0;
	}
	++pos;
  }
#endif
  Instance->StateMap.set(StateFlag::TRSET);
  Instance->StateMap.set(StateFlag::RESTCH);
}

void trace::tracedg() {
  if (!Instance->StateMap.test(StateFlag::WASTRAC)) {
	trace();
  }
  TracedEdges->resize(TraceDataSize, false);
  TracedEdges->reset();
  auto pixelIndex = 0L;
  for (auto iHeight = 0; iHeight < bitmap::getBitmapHeight(); ++iHeight) {
	auto flag = false;
	for (auto iWidth = 0; iWidth < bitmap::getBitmapWidth(); ++iWidth) {
	  if (TracedMap->test(wrap::toSize(pixelIndex))) {
		if (!flag) {
		  TracedEdges->set(wrap::toSize(pixelIndex));
		  flag = true;
		}
	  }
	  else {
		if (flag) {
		  TracedEdges->set(wrap::toSize(pixelIndex) - 1U);
		  flag = false;
		}
	  }
	  ++pixelIndex;
	}
	if (flag) {
	  TracedEdges->set(wrap::toSize(pixelIndex) - 1U);
	}
  }
  for (auto iWidth = 0; iWidth < bitmap::getBitmapWidth(); ++iWidth) {
	pixelIndex = iWidth;
	auto flag  = false;
	for (auto iHeight = 0; iHeight < bitmap::getBitmapHeight(); ++iHeight) {
	  if (TracedMap->test(wrap::toSize(pixelIndex))) {
		if (!flag) {
		  TracedEdges->set(wrap::toSize(pixelIndex));
		  flag = true;
		}
	  }
	  else {
		if (flag) {
		  TracedEdges->set(wrap::toSize(pixelIndex - bitmap::getBitmapWidth()));
		  flag = false;
		}
	  }
	  pixelIndex += bitmap::getBitmapWidth();
	}
	if (flag) {
	  TracedEdges->set(wrap::toSize(pixelIndex - bitmap::getBitmapWidth()));
	}
  }
  auto const bitmapSize = wrap::toSize(bitmap::getBitmapWidth() * bitmap::getBitmapHeight());
  auto const spTBD      = gsl::span(TraceBitmapData, bitmapSize);
  auto       pos        = size_t {0U};
  for (auto& iPixel : spTBD) {
	if (TracedEdges->test(pos)) {
	  iPixel = PENWHITE;
	}
	else {
	  iPixel = 0;
	}
	++pos;
  }
  Instance->StateMap.set(StateFlag::RESTCH);
  Instance->StateMap.set(StateFlag::WASEDG);
}

auto ti::trcbit(uint32_t const initialDirection, uint32_t& traceDirection, std::vector<TRACE_PNT>& tracedPoints)
    -> bool {
  auto pixelIndex = (CurrentTracePoint.y * bitmap::getBitmapWidth()) + CurrentTracePoint.x;
  // use the initial direction to determine the next direction
  switch (traceDirection) {
	case TRCR: { // was tracing right

	  pixelIndex += 1 - bitmap::getBitmapWidth(); // look at the pixel down and to the right
	  if (CurrentTracePoint.x == gsl::narrow<int32_t>(bitmap::getBitmapWidth()) - 1) { // at the edge go up
		traceDirection = TRCU;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
		  ++CurrentTracePoint.x;
		  --CurrentTracePoint.y;
		  traceDirection = TRCD;
		}
		else {
		  pixelIndex += bitmap::getBitmapWidth();            // look at the pixel below
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
			++CurrentTracePoint.x;
		  }
		  else { // start tracing up
			traceDirection = TRCU;
		  }
		}
	  }
	  break;
	}
	case TRCD: { // was tracing down

	  pixelIndex -= bitmap::getBitmapWidth() + 1; // look at the pixel down and to the left
	  if (CurrentTracePoint.y == 0) {             // if we are at the bottom edge
		traceDirection = TRCR;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
		  --CurrentTracePoint.x;
		  --CurrentTracePoint.y;
		  traceDirection = TRCL;
		}
		else {
		  ++pixelIndex;                                      // look at the pixel to the right
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
			--CurrentTracePoint.y;
		  }
		  else { // start tracing right
			traceDirection = TRCR;
		  }
		}
	  }
	  break;
	}
	case TRCL: { // was tracing left

	  pixelIndex += bitmap::getBitmapWidth() - 1; // look at the pixel above and to the left
	  if (CurrentTracePoint.x == 0) {
		traceDirection = TRCD;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
		  --CurrentTracePoint.x;
		  ++CurrentTracePoint.y;
		  traceDirection = TRCU;
		}
		else {
		  pixelIndex -= bitmap::getBitmapWidth();            // look at the pixel above
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
			--CurrentTracePoint.x;
		  }
		  else { // start tracing down
			traceDirection = TRCD;
		  }
		}
	  }
	  break;
	}
	case TRCU: { // was tracing up

	  pixelIndex += 1 + bitmap::getBitmapWidth(); // look at the pixel up and to the right
	  if (CurrentTracePoint.y == bitmap::getBitmapHeight() - 1) { // if we are at the top edge
		traceDirection = TRCL;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
		  ++CurrentTracePoint.x;
		  ++CurrentTracePoint.y;
		  traceDirection = TRCR;
		}
		else {
		  --pixelIndex;                                      // look at the pixel to the left
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) { // if pixel already traced
			++CurrentTracePoint.y;
		  }
		  else {
			traceDirection = TRCL;
		  }
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in trcbit: traceDirection [{}]\n", traceDirection);
	  break;
	}
  }
  if (tracedPoints.back().x != CurrentTracePoint.x || tracedPoints.back().y != CurrentTracePoint.y) {
	tracedPoints.push_back(TRACE_PNT {.x = gsl::narrow<int16_t>(CurrentTracePoint.x),
	                                  .y = gsl::narrow<int16_t>(CurrentTracePoint.y)});
	if (tracedPoints.size() >= POINTMAX) {
	  return false;
	}
  }
  return traceDirection != initialDirection || CurrentTracePoint.x != tracedPoints[0].x ||
         CurrentTracePoint.y != tracedPoints[0].y;
}

// remove all points on straight lines except the endpoints
// check for straightness by comparing slope of sections
void ti::decSlope(std::vector<TRACE_PNT>& src, std::vector<TRACE_PNT>& dst) {
  std::array<TRACE_PNT, 2> traceDiff;
  // always keep the first point
  dst.push_back(src.front());
  auto const itTP  = src.begin();
  auto       itTP1 = std::next(itTP);
  wrap::narrow(traceDiff[0].x, itTP1->x - itTP->x);
  wrap::narrow(traceDiff[0].y, itTP1->y - itTP->y);
  for (auto itTP2 = std::next(itTP1); itTP2 != src.end(); ++itTP2) {
	wrap::narrow(traceDiff[1].x, itTP2->x - itTP1->x);
	wrap::narrow(traceDiff[1].y, itTP2->y - itTP1->y);
	if (traceDiff[1].x != traceDiff[0].x || traceDiff[1].y != traceDiff[0].y) {
	  dst.push_back(*itTP1);
	}
	traceDiff[0] = traceDiff[1];
	++itTP1;
  }
  dst.push_back(src.back());
}

// write src points to destination keeping only points
// which are further apart than traceLength
void ti::decLen(std::vector<TRACE_PNT>& src, std::vector<TRACE_PNT>& dst) {
  auto itDL = src.begin();
  dst.push_back(*itDL);
  for (auto itDL1 = std::next(itDL); itDL1 != src.end(); ++itDL1) {
	if (auto const traceLength =
	        std::hypotf(wrap::toFloat(itDL1->x - itDL->x), wrap::toFloat(itDL1->y - itDL->y));
	    traceLength > IniFile.traceLength) {
	  dst.push_back(*itDL1);
	}
	++itDL;
  }
}

// write src points into destination form vertices, keeping only
// points where the sum of the distances is greater than the ratio(?)
void ti::decForm(std::vector<TRACE_PNT>& src, std::vector<F_POINT>& dst) {
  dst.emplace_back(wrap::toFloat(src[0].x) * StitchBmpRatio.x,
                   wrap::toFloat(src[0].y) * StitchBmpRatio.y);
  auto itTP             = src.begin();
  auto itNext           = src.begin();
  auto traceLengthSum1  = 0.0F;
  auto landscapeOffset1 = 0.0F;
  if (Instance->StateMap.test(StateFlag::LANDSCAP)) {
	auto const bmpSiS = bitmap::getBitmapSizeinStitches();
	landscapeOffset1  = wrap::toFloat(UnzoomedRect.cy) - bmpSiS.y;
  }
  for (auto itTP1 = std::next(itTP); itTP1 < src.end(); ++itTP1) {
	traceLengthSum1 += std::hypotf(wrap::toFloat(itTP1->x - itTP->x), wrap::toFloat(itTP1->y - itTP->y));
	if (auto const traceLength =
	        std::hypotf(wrap::toFloat(itTP1->x - itNext->x), wrap::toFloat(itTP1->y - itNext->y));
	    traceLengthSum1 > traceLength * IniFile.traceRatio) {
	  dst.emplace_back(wrap::toFloat(itTP->x) * StitchBmpRatio.x,
	                   (wrap::toFloat(itTP->y) * StitchBmpRatio.y) + landscapeOffset1);

	  itNext          = itTP;
	  itTP1           = itTP;
	  traceLengthSum1 = 0.0;
	}
	else {
	  ++itTP;
	}
  }
}

void ti::dutrac() {
  if (!thred::inStitchWin()) { // only trace in stitch window
	return;
  }
  auto stitchPoint = thred::pxCor2stch(WinMsg.pt);
  if (!Instance->StateMap.test(StateFlag::WASEDG)) { // if there is an edge map, trace it
	trace::tracedg();
	return;
  }
  thred::savdo();
  if (Instance->StateMap.test(StateFlag::LANDSCAP)) { // adjust for landscape mode
	auto const bmpSiS = bitmap::getBitmapSizeinStitches();
	stitchPoint.y -= wrap::toFloat(UnzoomedRect.cy) - bmpSiS.y;
  }
  auto const bmpSR = bitmap::getBmpStitchRatio();
  CurrentTracePoint = POINT {std::lround(bmpSR.x * stitchPoint.x), std::lround(bmpSR.y * stitchPoint.y)};
  CurrentTracePoint.x   = std::min(CurrentTracePoint.x, bitmap::getBitmapWidth());
  CurrentTracePoint.y   = std::min(CurrentTracePoint.y, bitmap::getBitmapHeight());
  auto const savedPoint = (CurrentTracePoint.y * bitmap::getBitmapWidth()) + CurrentTracePoint.x;
  auto       traceDirection = 0U;
  if (!TracedEdges->test(wrap::toSize(savedPoint))) {
	auto point = savedPoint;
	auto limit = (CurrentTracePoint.y + 1L) * bitmap::getBitmapWidth();
	// find the right edge
	while (point < limit && !TracedEdges->test(wrap::toSize(point))) {
	  ++point;
	}
	auto const right = point < limit ? point - (CurrentTracePoint.y * bitmap::getBitmapWidth())
	                                 : bitmap::getBitmapWidth();
	point            = savedPoint;
	limit            = CurrentTracePoint.y * bitmap::getBitmapWidth();
	// find the left edge
	while (point > limit && !TracedEdges->test(wrap::toSize(point))) {
	  --point;
	}
	auto const left = point == limit ? 0 : point - limit;
	point           = savedPoint;
	// find the bottom edge
	while (point > 0 && !TracedEdges->test(wrap::toSize(point))) {
	  if (point > bitmap::getBitmapWidth()) {
		point -= bitmap::getBitmapWidth();
	  }
	  else {
		point = 0;
	  }
	}
	auto const bottom = point > 0 ? point / bitmap::getBitmapWidth() : 0;
	point             = savedPoint;
	limit             = bitmap::getBitmapWidth() * bitmap::getBitmapHeight();
	// find the top edge
	while (point < limit && !TracedEdges->test(wrap::toSize(point))) {
	  point += bitmap::getBitmapWidth();
	}
	auto const top  = point < limit ? point / bitmap::getBitmapWidth() : bitmap::getBitmapHeight();
	auto       flag = 0U;
	auto       minimumEdgeDistance = BIGINT32;
	if (left != 0) { // trace left edge
	  minimumEdgeDistance = CurrentTracePoint.x - left;
	  flag                = TRCL;
	}
	if (right < bitmap::getBitmapWidth()) { // trace right edge
	  if (auto const edgeDistance = right - CurrentTracePoint.x; edgeDistance < minimumEdgeDistance) {
		minimumEdgeDistance = edgeDistance;
		flag                = TRCR;
	  }
	}
	if (bottom != 0) { // trace bottom edge
	  if (auto const edgeDistance = CurrentTracePoint.y - bottom; edgeDistance < minimumEdgeDistance) {
		minimumEdgeDistance = edgeDistance;
		flag                = TRCD;
	  }
	}
	if (top < bitmap::getBitmapHeight()) { // trace top edge
	  if (auto const edgeDistance = top - CurrentTracePoint.y; edgeDistance < minimumEdgeDistance) {
		flag = TRCU;
	  }
	}
	// trace in the direction of the closest edge
	switch (flag) {
	  case TRCU: {
		CurrentTracePoint.y = top;
		traceDirection      = TRCR;
		break;
	  }
	  case TRCR: {
		CurrentTracePoint.x = right;
		traceDirection      = TRCD;
		break;
	  }
	  case TRCD: {
		CurrentTracePoint.y = bottom;
		traceDirection      = TRCL;
		break;
	  }
	  case TRCL: {
		CurrentTracePoint.x = left;
		traceDirection      = TRCU;
		break;
	  }
	  default:
		return;
	}
  }
  uint32_t const initialDirection = traceDirection;
  auto           tracedPoints     = std::vector<TRACE_PNT> {};
  tracedPoints.push_back(TRACE_PNT {.x = gsl::narrow<int16_t>(CurrentTracePoint.x),
                                    .y = gsl::narrow<int16_t>(CurrentTracePoint.y)});
  while (trcbit(initialDirection, traceDirection, tracedPoints)) { }
  if (tracedPoints.size() >= POINTMAX) { // too many points
	displayText::tabmsg(IDS_FRM2L, false);
	return;
  }
#ifndef TESTTRC
  // decimate the traced points
  auto decimatedLine = std::vector<TRACE_PNT> {};
  decimatedLine.reserve(tracedPoints.size());
  decSlope(tracedPoints, decimatedLine);
  tracedPoints.clear();
  tracedPoints.reserve(decimatedLine.size());
  decLen(decimatedLine, tracedPoints);
#endif
  auto form        = FRM_HEAD {};
  form.vertexIndex = wrap::toUnsigned(Instance->FormVertices.size());
  decForm(tracedPoints, Instance->FormVertices);
  form.vertexCount     = wrap::toUnsigned(Instance->FormVertices.size() - form.vertexIndex);
  form.type            = FRMFPOLY;
  form.attribute       = gsl::narrow<uint8_t>(ActiveLayer << 1U);
  form.satinGuideCount = 0;
  form.outline();
  Instance->FormList.push_back(form);
  Instance->StateMap.set(StateFlag::RESTCH);
  Instance->StateMap.set(StateFlag::FRMOF);
  form::tglfrm();
}

void trace::trinit() {
  auto histogramData = std::array<std::array<uint32_t, LEVELCNT>, CHANLCNT> {};
  if (!bitmap::ismap()) {
	displayText::tabmsg(IDS_MAPLOD, false);
	return;
  }
  if (Instance->StateMap.test(StateFlag::TRSET)) {
	Instance->StateMap.set(StateFlag::WASTRCOL);
	trace();
	return;
  }
  Instance->StateMap.set(StateFlag::TRCRED);
  Instance->StateMap.set(StateFlag::TRCGRN);
  Instance->StateMap.set(StateFlag::TRCBLU);
  auto componentPeak = std::array<uint32_t, CHANLCNT> {};
  if (!Instance->StateMap.test(StateFlag::WASTRAC)) {
	TraceDataSize = bitmap::getrmap();
  }
  if (Instance->StateMap.test(StateFlag::MONOMAP)) {
	auto const spTBD =
	    gsl::span {TraceBitmapData, wrap::toSize(bitmap::getBitmapWidth() * bitmap::getBitmapHeight())};
	auto const color     = gsl::narrow<COLORREF>(spTBD[0]);
	auto       highColor = color;
	if (auto const pixel =
	        std::ranges::find_if(spTBD.begin(),
	                             spTBD.end(),
	                             [color](uint32_t const& data) -> bool { return data != color; });
	    pixel != spTBD.end()) {
	  highColor = *pixel;
	}
	auto colors = ti::trcols(highColor);
	std::ranges::copy(colors, HighColors.begin());
	colors = ti::trcols(color);
	for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	  if (colors.at(iRGB) > HighColors.at(iRGB)) {
		std::swap(colors.at(iRGB), HighColors.at(iRGB));
	  }
	  componentPeak.at(iRGB) = ((HighColors.at(iRGB) - colors.at(iRGB)) / 2) + colors.at(iRGB);
	}
  }
  else {
	for (auto const spTBD =
	         gsl::span {TraceBitmapData, wrap::toSize(bitmap::getBitmapWidth() * bitmap::getBitmapHeight())};
	     auto const pixel : spTBD) {
	  auto colors  = ti::trcols(pixel);
	  auto iColors = colors.begin();
	  for (auto& iHistogramData : histogramData) {
		auto const itHD = wrap::next(iHistogramData.begin(), *iColors++);
		++*itHD;
	  }
	}
	auto componentPeakCount  = std::array<uint32_t, CHANLCNT> {};
	auto iComponentPeakCount = componentPeakCount.begin();
	auto iComponentPeak      = componentPeak.begin();
	for (auto& channelData : histogramData) {
	  auto iLevel = 0U;
	  for (auto const& channel : channelData) {
		if (channel > *iComponentPeakCount) {
		  *iComponentPeakCount = channel;
		  *iComponentPeak      = iLevel;
		}
		++iLevel;
	  }
	  ++iComponentPeakCount;
	  ++iComponentPeak;
	}
  }
  InvertDownColor = 0U;
  for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	if (componentPeak.at(iRGB) != 0U) {
	  --componentPeak.at(iRGB);
	}
	InvertDownColor |= componentPeak.at(iRGB) << TraceShift.at(iRGB);
  }
  DownPixelColor = InvertDownColor ^ COLMASK;
  InvertUpColor  = PENWHITE;
  UpPixelColor   = 0U;
  Instance->StateMap.set(StateFlag::WASTRCOL);
  trace();
}

void trace::trcsel() {
  if (!bitmap::ismap()) {
	displayText::tabmsg(IDS_MAPLOD, false);
	return;
  }
  Instance->StateMap.set(StateFlag::WASTRCOL);
  Instance->StateMap.set(StateFlag::TRCRED);
  Instance->StateMap.set(StateFlag::TRCBLU);
  Instance->StateMap.set(StateFlag::TRCGRN);
  DownPixelColor = PENWHITE;
  UpPixelColor   = 0;
  trace();
  Instance->StateMap.reset(StateFlag::HIDMAP);
  Instance->StateMap.reset(StateFlag::TRSET);
  auto const bitmapSize = wrap::toSize(bitmap::getBitmapWidth() * bitmap::getBitmapHeight());
  for (auto const spTBD = gsl::span(TraceBitmapData, bitmapSize); auto& iPixel : spTBD) {
	auto colors                = ti::trcols(iPixel);
	auto maximumColorComponent = colors[0];
	auto iRGB                  = 2U;
	if (colors[1] > maximumColorComponent) {
	  maximumColorComponent = colors[1];
	  iRGB                  = 1;
	}
	if (colors[2] > maximumColorComponent) {
	  iRGB = 0;
	}
	iPixel &= TraceRGB.at(iRGB);
  }
  bitmap::bitbltBitmap();
  Instance->StateMap.set(StateFlag::WASDSEL);
  Instance->StateMap.set(StateFlag::RESTCH);
  ti::tracwnd();
}

void ti::ritrcol(COLORREF& colRef, uint32_t colNum) noexcept {
  colRef &= TraceRGBMask.at(ColumnColor);
  colNum &= BYTMASK;
  colRef |= colNum << TraceShift.at(ColumnColor);
}

void ti::dutrnum0(uint32_t const colNum) {
  Instance->StateMap.reset(StateFlag::NUMIN);
  Instance->StateMap.reset(StateFlag::TRNIN0);
  if (Instance->StateMap.test(StateFlag::TRNUP)) {
	auto const itTraceUpWindow = wrap::next(TraceUpWindow.begin(), ColumnColor);
	ritrcol(InvertUpColor, colNum);
	UpPixelColor = InvertUpColor ^ COLMASK;
	thred::redraw(*itTraceUpWindow);
  }
  else {
	auto const itTraceDownWindow = wrap::next(TraceDownWindow.begin(), ColumnColor);
	ritrcol(InvertDownColor, colNum);
	DownPixelColor = InvertDownColor ^ COLMASK;
	thred::redraw(*itTraceDownWindow);
  }
  auto const itTraceControlWindow = wrap::next(TraceControlWindow.begin(), ColumnColor);
  thred::redraw(*itTraceControlWindow);
  DestroyWindow(TraceNumberInput);
  Instance->StateMap.set(StateFlag::WASTRCOL);
  trace::trace();
}

void trace::dutrnum2() {
  ti::dutrnum0(wrap::toUnsigned(std::wcstol(TraceInputBuffer.data(), nullptr, DECRAD)));
}

void trace::dutrnum1() {
  DestroyWindow(GeneralNumberInputBox);
  Instance->StateMap.reset(StateFlag::NUMIN);
  Instance->StateMap.reset(StateFlag::TRNIN1);
  auto traceLength = thred::getMsgBufferValue();
  traceLength      = std::min(traceLength, MAXSIZ);
  if (Instance->StateMap.test(StateFlag::TRNUP)) {
	IniFile.traceLength = traceLength * PFGRAN;
	ti::trcstpnum();
  }
  else {
	IniFile.traceRatio = TROFF + pow(TRBASE, traceLength);
	ti::trcratnum();
  }
}

auto ti::ducolm() -> uint32_t {
  if (TraceMsgPoint.x < gsl::narrow<int32_t>(ButtonWidth)) {
	return 0U;
  }
  if (TraceMsgPoint.x < gsl::narrow<int32_t>(ButtonWidth) * 2) {
	return 1U;
  }

  return 2U;
}

void ti::trnumwnd0(int32_t const position) noexcept {
  constexpr auto DW_STYLE = DWORD {SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER};
  TraceNumberInput        = CreateWindowEx(
      0L, L"STATIC", nullptr, DW_STYLE, ButtonWidthX3, position, ButtonWidth, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
}

void ti::trnumwnd1(int32_t const position) noexcept {
  constexpr auto DW_STYLE = DWORD {WS_CHILD | WS_VISIBLE | WS_BORDER};
  GeneralNumberInputBox   = CreateWindowEx(
      0L, L"STATIC", nullptr, DW_STYLE, ButtonWidthX3, position, ButtonWidthX3, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
}

void trace::blak() {
  if (!bitmap::ismap()) {
	displayText::tabmsg(IDS_MAPLOD, false);
	return;
  }
  auto const& formList = Instance->FormList;

  if (formList.empty()) {
	displayText::tabmsg(IDS_FRMNO, false);
	return;
  }
  ti::tracwnd();
  BlackPen = wrap::createPen(PS_SOLID, PENNWID, PENBLK);
  SelectObject(bitmap::getBitmapDC(), BlackPen);
  SelectObject(bitmap::getTraceDC(), BlackPen);
  if (!Instance->StateMap.test(StateFlag::WASTRAC)) {
	TraceDataSize = bitmap::getrmap();
  }
  for (auto const& iForm : formList) {
	bitmap::bfrm(iForm);
  }
  DeleteObject(BlackPen);
  Instance->StateMap.set(StateFlag::WASBLAK);
  Instance->StateMap.set(StateFlag::RESTCH);
}

void ti::getColors() {
  auto const invColumn = 2U - ColumnColor;
  if (wrap::pressed(VK_SHIFT)) {
	UpPixelColor &= TraceRGBMask.at(ColumnColor);
	DownPixelColor |= TraceRGB.at(invColumn);
	return;
  }
  auto const ratio         = TraceMsgPoint.y / (ButtonHeight * 15.0);
  auto const position      = wrap::floor<uint32_t>(ratio * 255.0);
  auto       traceColor    = gsl::narrow_cast<COLORREF>(UpPixelColor & TraceRGB.at(invColumn));
  auto const tracePosition = gsl::narrow_cast<COLORREF>(position << TraceShift.at(ColumnColor));
  if (tracePosition < traceColor) {
	UpPixelColor &= TraceRGBMask.at(ColumnColor);
	UpPixelColor |= tracePosition;
	return;
  }
  traceColor = DownPixelColor & TraceRGB.at(invColumn);
  if (tracePosition > traceColor) {
	DownPixelColor &= TraceRGBMask.at(ColumnColor);
	DownPixelColor |= tracePosition;
	return;
  }
  if (WinMsg.message == WM_LBUTTONDOWN) {
	UpPixelColor &= TraceRGBMask.at(ColumnColor);
	UpPixelColor |= position << TraceShift.at(ColumnColor);
  }
  else {
	DownPixelColor &= TraceRGBMask.at(ColumnColor);
	DownPixelColor |= position << TraceShift.at(ColumnColor);
  }
}

void trace::tracpar() {
  if (Instance->StateMap.test(StateFlag::TRNIN0)) {
	dutrnum2();
  }
  if (Instance->StateMap.test(StateFlag::TRNIN1)) {
	dutrnum1();
  }
  TraceMsgPoint = POINT {WinMsg.pt.x - ThredWindowOrigin.x, WinMsg.pt.y - ThredWindowOrigin.y};
  if (TraceMsgPoint.x > ButtonWidthX3) {
	ti::dutrac();
  }
  else {
	ColumnColor = ti::ducolm();
	if (TraceMsgPoint.y < ButtonHeight * TRWINROW01) {
	  ti::getColors();
	  auto const itTraceControlWindow = wrap::next(TraceControlWindow.begin(), ColumnColor);
	  thred::redraw(*itTraceControlWindow);
	  trace();
	}
	else {
	  if (auto const position = wrap::floor<int32_t>(TraceMsgPoint.y / ButtonHeight); position < TRWINROW02) {
		Instance->StateMap.flip(TraceRGBFlag.at(ColumnColor));
		auto const itTraceSelectWindow = wrap::next(TraceSelectWindow.begin(), ColumnColor);
		thred::redraw(*itTraceSelectWindow);
		trace();
	  }
	  else {
		if (position < TRWINROW04) {
		  Instance->StateMap.set(StateFlag::NUMIN);
		  Instance->StateMap.set(StateFlag::TRNIN0);
		  TraceMsgIndex       = 0;
		  TraceInputBuffer[0] = 0;
		  if (position < TRWINROW03) {
			ti::trnumwnd0(ButtonHeight * TRWINROW02);
			Instance->StateMap.set(StateFlag::TRNUP);
		  }
		  else {
			ti::trnumwnd0(ButtonHeight * TRWINROW03);
			Instance->StateMap.reset(StateFlag::TRNUP);
		  }
		}
		else {
		  if (position < TRWINROW06) {
			Instance->StateMap.set(StateFlag::NUMIN);
			Instance->StateMap.set(StateFlag::TRNIN1);
			thred::resetMsgBuffer();
			if (position < TRWINROW05) {
			  ti::trnumwnd1(ButtonHeight * TRWINROW04);
			  Instance->StateMap.set(StateFlag::TRNUP);
			}
			else {
			  ti::trnumwnd1(ButtonHeight * TRWINROW05);
			  Instance->StateMap.reset(StateFlag::TRNUP);
			}
		  }
		  else {
			switch (position) {
			  case TRWINROW06: {
				trdif();
				break;
			  }
			  case TRWINROW07: {
				thred::hidbit();
				break;
			  }
			  case TRWINROW08: {
				blak();
				break;
			  }
			  case TRWINROW09: {
				trcsel();
				break;
			  }
			  case TRWINROW10: {
				tracedg();
				break;
			  }
			  default: {
				outDebugString(L"default hit in tracpar: position [{}]\n", position);
				break;
			  }
			}
		  }
		}
	  }
	}
  }
}

void ti::trcnum(uint32_t const shift, COLORREF color, uint32_t const backColor) {
  auto const zeroWidth = thred::txtWid(L"0");
  color >>= shift;
  color &= BYTMASK;
  auto const val       = std::to_wstring(color);
  auto const xPosition = zeroWidth.cx * gsl::narrow<int32_t>(3U - val.size() + 1U);
  SetBkColor(DrawItem->hDC, backColor);
  wrap::textOut(DrawItem->hDC, xPosition, 1, val.c_str(), wrap::toUnsigned(val.size()));
}

void ti::durct(uint32_t const shift, RECT const& traceControlRect, RECT& traceHighMask, RECT& traceMiddleMask, RECT& traceLowMask) {
  auto const lowerColor    = gsl::narrow_cast<uint8_t>((UpPixelColor >> shift) & 0xffU);
  auto const upperColor    = gsl::narrow_cast<uint8_t>((DownPixelColor >> shift) & 0xffU);
  auto const controlHeight = wrap::toFloat(traceControlRect.bottom - traceControlRect.top);
  auto       ratio         = wrap::toFloat(lowerColor) * CCRATIO;
  traceHighMask.left = traceLowMask.left = traceMiddleMask.left = traceControlRect.left;
  traceHighMask.right = traceLowMask.right = traceMiddleMask.right = traceControlRect.right;
  traceMiddleMask.top = std::lround((controlHeight * ratio) + wrap::toFloat(traceControlRect.top));
  ratio               = wrap::toFloat(upperColor) * CCRATIO;
  traceMiddleMask.bottom = std::lround((controlHeight * ratio) + wrap::toFloat(traceControlRect.top));
  Instance->StateMap.reset(StateFlag::DUHI);
  Instance->StateMap.reset(StateFlag::DULO);
  if (lowerColor != 0U) {
	Instance->StateMap.set(StateFlag::DULO);
	traceLowMask.bottom = traceMiddleMask.top;
	traceLowMask.top    = 0;
  }
  if (upperColor != BYTEMAXV) {
	Instance->StateMap.set(StateFlag::DUHI);
	traceHighMask.top    = traceMiddleMask.bottom;
	traceHighMask.bottom = traceControlRect.bottom;
  }
}

// ReSharper disable CppParameterMayBeConst
void ti::dublk(HDC hDC, RECT const& traceHighMask, RECT const& traceLowMask, HBRUSH brush) {
  if (Instance->StateMap.test(StateFlag::DUHI)) {
	FillRect(hDC, &traceHighMask, brush);
  }
  if (Instance->StateMap.test(StateFlag::DULO)) {
	FillRect(hDC, &traceLowMask, brush);
  }
}
// ReSharper restore CppParameterMayBeConst

void trace::wasTrace() {
  auto traceHighMaskRect   = RECT {}; // high trace mask rectangle
  auto traceMiddleMaskRect = RECT {}; // middle trace mask rectangle
  auto traceLowMaskRect    = RECT {}; // low trace mask rectangle
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const blackBrush          = CreateSolidBrush(0); // black brush
  auto       iTraceUpWindow      = TraceUpWindow.begin();
  auto       iTraceDownWindow    = TraceDownWindow.begin();
  auto       iTraceControlWindow = TraceControlWindow.begin();
  auto       iTraceSelectWindow  = TraceSelectWindow.begin();
  auto       iTraceShift         = TraceShift.begin();
  auto       iTraceRGB           = TraceRGB.begin();
  auto       iTraceRGBFlag       = TraceRGBFlag.begin();
  for (auto const& brush : TraceBrush) {
	if (DrawItem->hwndItem == *iTraceUpWindow++) {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, brush);
	  ti::trcnum(*iTraceShift, InvertUpColor, *iTraceRGB);
	  break;
	}
	if (DrawItem->hwndItem == *iTraceDownWindow++) {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, brush);
	  ti::trcnum(*iTraceShift, InvertDownColor, *iTraceRGB);
	}
	if (DrawItem->hwndItem == *iTraceControlWindow++) {
	  ti::durct(*iTraceShift, DrawItem->rcItem, traceHighMaskRect, traceMiddleMaskRect, traceLowMaskRect);
	  FillRect(DrawItem->hDC, &traceMiddleMaskRect, brush);
	  ti::dublk(DrawItem->hDC, traceHighMaskRect, traceLowMaskRect, blackBrush);
	  break;
	}
	if (DrawItem->hwndItem == *iTraceSelectWindow++) {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto tempBrush = blackBrush;
	  SetBkColor(DrawItem->hDC, 0);
	  SetTextColor(DrawItem->hDC, *iTraceRGB);
	  if (Instance->StateMap.test(*iTraceRGBFlag)) {
		tempBrush = brush;
		SetTextColor(DrawItem->hDC, 0);
		SetBkColor(DrawItem->hDC, *iTraceRGB);
	  }
#pragma warning(suppress : 26812) // Enum.3 prefer 'enum class' over 'enum'
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, tempBrush);
	  auto const strOnOff = displayText::loadStr(Instance->StateMap.test(*iTraceRGBFlag) ? IDS_ON : IDS_OFF);
	  wrap::textOut(DrawItem->hDC, 1, 1, strOnOff.c_str(), wrap::toUnsigned(strOnOff.size()));
	  break;
	}
	if (DrawItem->hwndItem == TraceNumberInput) {
	  auto const itColTraceBrush = wrap::next(TraceBrush.begin(), ColumnColor);
	  auto const itColTraceRGB   = wrap::next(TraceRGB.begin(), ColumnColor);
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, *itColTraceBrush);
	  SetBkColor(DrawItem->hDC, *itColTraceRGB);
	  wrap::textOut(DrawItem->hDC,
	                1,
	                1,
	                TraceInputBuffer.data(),
	                gsl::narrow<uint32_t>(wcslen(TraceInputBuffer.data())));
	  break;
	}
	++iTraceShift;
	++iTraceRGB;
	++iTraceRGBFlag;
  }
}

void trace::wasTrace1() {
  if (Instance->StateMap.test(StateFlag::TRNIN0)) {
	dutrnum2();
  }
  if (Instance->StateMap.test(StateFlag::TRNIN1)) {
	dutrnum1();
  }
  if (!Instance->StateMap.test(StateFlag::WASEDG)) {
	tracpar();
  }
}

void trace::traceNumberInput(wchar_t const NumericCode) {
  auto itTraceInputBuffer = wrap::next(TraceInputBuffer.begin(), TraceMsgIndex);
  *itTraceInputBuffer++   = NumericCode;
  *itTraceInputBuffer     = 0;
  ++TraceMsgIndex;
  // ToDo - should this be uint8_t?
  auto traceColor = wrap::toUnsigned(std::wcstol(TraceInputBuffer.data(), nullptr, DECRAD));
  switch (TraceMsgIndex) {
	case 2: {
	  // if it is greater than 25, we can process as-is since the maximum value is 255
	  if (constexpr auto TWODIGIT = uint32_t {25U}; traceColor > TWODIGIT) {
		ti::dutrnum0(traceColor);
	  }
	  break;
	}
	case 3: {
	  traceColor = std::min(traceColor, uint32_t {BYTEMAXV});
	  ti::dutrnum0(traceColor);
	  break;
	}
	default: {
	  outDebugString(L"default hit in traceNumberInput: TraceMsgIndex [{}]\n", TraceMsgIndex);
	  break;
	}
  }
  thred::redraw(TraceNumberInput);
}

void trace::traceNumberReset() noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto const itTraceInputBuffer = wrap::next(TraceInputBuffer.begin(), TraceMsgIndex);
  *itTraceInputBuffer           = 0;
  thred::redraw(TraceNumberInput);
}
