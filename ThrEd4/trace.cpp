// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "bitmap.h"
#include "displayText.h"
#include "form.h"
#include "globals.h"
#include "trace.h"
#include "thred.h"

// Standard Libraries
#include <numeric>

namespace ti = trace::internal;

constexpr auto ADJCOUNT = uint32_t {9U}; // including the center pixel there are 9 pixels immediately adjacent
constexpr auto BLUCOL   = uint32_t {0xff0000U}; // code for the color blue
constexpr auto BLUMSK   = uint32_t {0x00ffffU}; // mask for the color blue
constexpr auto BYTEMAXV = uint8_t {255U};       // max color value in a byte wide counter
constexpr auto CCRATIO  = (1.0F / 255.0F);      // This is used to convert 0-255 to 0-1
constexpr auto CHANLCNT = uint32_t {3U};        // number of color channels i.e. RGB
constexpr auto GRNCOL   = uint32_t {0x00ff00U}; // code for the color green
constexpr auto GRNMSK   = uint32_t {0xff00ffU}; // mask for the color green
constexpr auto LEVELCNT = uint32_t {256U};      // number of color levels in a byte wide counter
constexpr auto POINTMAX = size_t {500000U};     // maximum number of trace points to consider
constexpr auto REDCOL   = uint32_t {0x0000ffU}; // code for the color red
constexpr auto REDMSK   = uint32_t {0xffff00U}; // mask for the color red

// edge tracing directions
enum TraceDirection {
  TRCU, // top edge
  TRCR, // right edge
  TRCD, // bottom edge
  TRCL  // left edge
};

static auto PixelColors = std::array<uint32_t, CHANLCNT> {};    // separated pixel reference colors
static auto TraceControlWindow = std::array<HWND, CHANLCNT> {}; // trace control windows
static auto TraceDownWindow    = std::array<HWND, CHANLCNT> {}; // trace down number windows
static auto TraceSelectWindow  = std::array<HWND, CHANLCNT> {}; // trace select windows
static auto TraceUpWindow      = std::array<HWND, CHANLCNT> {}; // trace up number windows
static auto CurrentTracePoint  = POINT {};                      // current point being traced
static auto TraceDataSize      = uint32_t {}; // size of the trace bitmap in double words
static auto TraceStepWin       = gsl::narrow_cast<HWND>(nullptr); // trace stepSize window
static auto TraceRGBFlag =
    std::array<StateFlag, 3> {StateFlag::TRCRED, StateFlag::TRCGRN, StateFlag::TRCBLU}; // trace bits
static auto TraceRGBMask = std::array<uint32_t, 3> {REDMSK, GRNMSK, BLUMSK}; // trace masks
static auto TraceRGB     = std::array<uint32_t, 3> {BLUCOL, GRNCOL, REDCOL}; // trace colors
static auto TraceAdjacentColors = std::array<uint32_t, ADJCOUNT> {}; // separated colors for adjacent pixels
static auto TraceInputBuffer = std::array<wchar_t, 4> {};            // for user input color numbers
static auto TraceMsgIndex    = uint32_t {};                          // pointer to the trace buffer
static auto UpPixelColor     = COLORREF {}; // color of the up reference pixel
static auto DownPixelColor   = COLORREF {}; // color of the down reference pixel
static auto InvertUpColor    = COLORREF {}; // complement color of the up reference pixel
static auto InvertDownColor  = COLORREF {}; // complement color of the down reference pixel
static auto TraceMsgPoint    = POINT {};    // message point for trace parsing
static auto HighColors  = std::array<uint32_t, CHANLCNT> {}; // separated upper reference colors
static auto LowColors   = std::array<uint32_t, CHANLCNT> {}; // separated lower reference colors
static auto ColumnColor = uint32_t {};                       // trace color column
static auto TraceShift  = std::array<uint32_t, 3> {0U, BYTSHFT, WRDSHFT}; // trace shift values
static auto TraceBrush  = std::array<HBRUSH, CHANLCNT> {};      // red,green,and blue brushes
static auto TraceNumberInput = gsl::narrow_cast<HWND>(nullptr); // trace number input window
static auto BlackPen         = gsl::narrow_cast<HPEN>(nullptr); // black pen

void trace::initColorRef() noexcept {
  UpPixelColor    = 0;
  DownPixelColor  = 0x7f7f7f;
  InvertUpColor   = PENWHITE;
  InvertDownColor = PENGRAY;
}

auto trace::internal::trcsub(int32_t xCoordinate, int32_t yCoordinate, int32_t buttonHeight) -> HWND {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  constexpr auto DW_STYLE = DWORD {SS_OWNERDRAW | WS_CHILD | WS_BORDER};
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const window = CreateWindowEx(
      0L, L"STATIC", L"", DW_STYLE, xCoordinate, yCoordinate, ButtonWidth, buttonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
  if (nullptr != window) {
	return window;
  }
  throw std::runtime_error("No window created in trcsub");
}

void trace::initTraceWindows() {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  constexpr auto DW_STYLE = DWORD {SS_NOTIFY | SS_CENTER | WS_CHILD | WS_BORDER};
  TraceStepWin            = CreateWindowEx(
      0L, L"STATIC", L"", DW_STYLE, 0, ButtonHeight * 18, ButtonWidthX3, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
  auto iTraceControlWindow = TraceControlWindow.begin();
  auto iTraceDownWindow    = TraceDownWindow.begin();
  auto iTraceSelectWindow  = TraceSelectWindow.begin();
  auto iTraceUpWindow      = TraceUpWindow.begin();
  auto iTraceBrush         = TraceBrush.begin();
  auto iTraceRGB           = TraceRGB.begin();
  for (auto iRGB = 0U; iRGB < TraceControlWindow.size(); ++iRGB) {
	auto const channel       = gsl::narrow_cast<int32_t>(iRGB);
	*(iTraceControlWindow++) = ti::trcsub(ButtonWidth * channel, 0, ButtonHeight * 15);
	*(iTraceSelectWindow++)  = ti::trcsub(ButtonWidth * channel, ButtonHeight * 15, ButtonHeight);
	*(iTraceUpWindow++)      = ti::trcsub(ButtonWidth * channel, ButtonHeight * 16, ButtonHeight);
	*(iTraceDownWindow++)    = ti::trcsub(ButtonWidth * channel, ButtonHeight * 17, ButtonHeight);
	*(iTraceBrush++)         = CreateSolidBrush(*(iTraceRGB++));
  }
}

void trace::internal::trcols(COLORREF color) noexcept {
  PixelColors[0] = color & B1MASK;
  PixelColors[1] = (color & B2MASK) >> BYTSHFT;
  PixelColors[2] = (color & B3MASK) >> WRDSHFT;
}

void trace::internal::trcstpnum() {
  auto const fmtStr = displayText::loadStr(IDS_TRCSTP);
  // NOLINTNEXTLINE(clang-diagnostic-sign-conversion)
  SetWindowText(TraceStepWin, fmt::format(fmtStr, (IniFile.traceLength * IPFGRAN)).c_str());
}

void trace::internal::trcratnum() {
  constexpr auto HLIN = uint32_t {HNUM};
  displayText::butxt(HLIN, fmt::format(displayText::loadStr(IDS_TRCRAT), -log10(IniFile.traceRatio - 1.0F)));
}

auto trace::internal::trcin(COLORREF color) -> bool {
  if (color != 0U) {
	ti::trcols(color);
	if (StateMap->test(StateFlag::TRCRED)) {
	  if (PixelColors[0] > HighColors[0]) {
		return false;
	  }
	  if (PixelColors[0] < LowColors[0]) {
		return false;
	  }
	}
	if (StateMap->test(StateFlag::TRCGRN)) {
	  if (PixelColors[1] > HighColors[1]) {
		return false;
	  }
	  if (PixelColors[1] < LowColors[1]) {
		return false;
	  }
	}
	if (StateMap->test(StateFlag::TRCBLU)) {
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

void trace::internal::shownd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_SHOW);
}

void trace::internal::hidwnd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_HIDE);
}

void trace::internal::showTraceWin() noexcept {
  auto iTraceSelectWindow = TraceSelectWindow.begin();
  auto iTraceUpWindow     = TraceUpWindow.begin();
  auto iTraceDownWindow   = TraceDownWindow.begin();
  for (auto const& iTraceControlWindow : TraceControlWindow) {
	ti::shownd(iTraceControlWindow);
	ti::shownd(*(iTraceSelectWindow++));
	ti::shownd(*(iTraceUpWindow++));
	ti::shownd(*(iTraceDownWindow++));
  }
}

void trace::internal::tracwnd() {
  thred::hideColorWin();
  ti::hidwnd(ButtonWin->operator[](HBOXSEL));
  ti::hidwnd(ButtonWin->operator[](HUPTO));
  ti::shownd(TraceStepWin);
  ti::trcstpnum();
  ti::trcratnum();
  displayText::clrhbut(4);
  showTraceWin();
}

// Check Translation
static inline void trace::internal::difsub(uint32_t const source, uint32_t shift, uint32_t& destination) noexcept {
  destination = (source >> (shift & NIBMASK)) & BYTMASK;
}

// pixel map for adjacent pixels
//  213
//  504
//  678

void trace::internal::difbits(uint32_t shift, uint32_t* point) noexcept {
  auto* testPoint = point;
  if (testPoint != nullptr) {
	auto iTrcAdjClrs = TraceAdjacentColors.begin();
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 0 - center
	testPoint -= bitmap::getBitmapWidth();
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 1 - N
	testPoint -= 1;
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 2 - NW
	testPoint += 2;
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 3 - NE
	testPoint += bitmap::getBitmapWidth();
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 4 - E
	testPoint -= 2;
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 5 - W
	testPoint += bitmap::getBitmapWidth();
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 6 - SW
	testPoint += 1;
	ti::difsub(*testPoint, shift, *(iTrcAdjClrs++)); // pixel 7 - S
	testPoint += 1;
	ti::difsub(*testPoint, shift, *iTrcAdjClrs); // pixel 8 - SE
  }
}

auto trace::internal::trsum() -> uint32_t {
  auto const firstColor = TraceAdjacentColors.front();
  auto const iBegin     = std::next(TraceAdjacentColors.begin());
  auto const iEnd       = std::next(TraceAdjacentColors.end(), -1);
  auto const fold       = [firstColor](uint32_t a, uint32_t b) {
    return a + ((b > firstColor) ? (b - firstColor) : (firstColor - b));
  };
  return wrap::toUnsigned(std::accumulate(iBegin, iEnd, 0, fold));
}

void trace::internal::hideTraceWin() noexcept {
  auto iTraceSelectWindow = TraceSelectWindow.begin();
  auto iTraceUpWindow     = TraceUpWindow.begin();
  auto iTraceDownWindow   = TraceDownWindow.begin();
  for (auto const& iTraceControlWindow : TraceControlWindow) {
	ti::hidwnd(iTraceControlWindow);
	ti::hidwnd(*(iTraceSelectWindow++));
	ti::hidwnd(*(iTraceUpWindow++));
	ti::hidwnd(*(iTraceDownWindow++));
  }
}

void trace::untrace() {
  if (StateMap->testAndReset(StateFlag::WASTRAC)) {
	bitmap::resetDC();
	if (!TracedEdges->empty()) {
	  TracedEdges->resize(0); // allocated in tracedg
	}
	if (!TracedMap->empty()) {
	  TracedMap->resize(0); // allocated in trace
	}
	StateMap->reset(StateFlag::WASEDG);
	ti::hideTraceWin();
	ti::hidwnd(TraceStepWin);
	thred::showColorWin();
	for (auto const& iButton : *ButtonWin) {
	  ti::shownd(iButton);
	}
  }
  else {
	if (StateMap->test(StateFlag::TRCUP)) {
	  DownPixelColor = PENWHITE;
	}
	else {
	  UpPixelColor = 0;
	}
  }
}

void trace::trdif() {
  if (!bitmap::ismap()) {
	displayText::tabmsg(IDS_MAPLOD);
	return;
  }
  StateMap->reset(StateFlag::TRSET);
  StateMap->reset(StateFlag::HIDMAP);
  trace::untrace();
  if ((bitmap::getBitmapHeight() * bitmap::getBitmapWidth()) != 0U) {
	auto differenceBitmap = std::vector<uint32_t> {};
	differenceBitmap.resize(wrap::toSize(bitmap::getBitmapHeight() * bitmap::getBitmapWidth()));
	auto colorSumMaximum = 0U;
	auto colorSumMinimum = std::numeric_limits<uint32_t>::max();
	if (!StateMap->test(StateFlag::WASTRAC)) {
	  TraceDataSize = bitmap::getrmap();
	}
	for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	  for (auto iHeight = 1; iHeight < bitmap::getBitmapHeight() - 1; ++iHeight) {
		auto iPoint = iHeight * bitmap::getBitmapWidth();
		++iPoint;
		for (auto iWidth = 1; iWidth < bitmap::getBitmapWidth() - 1; ++iWidth) {
		  ti::difbits(TraceShift[iRGB], wrap::next(TraceBitmapData, iPoint));
		  differenceBitmap[wrap::toSize(iPoint)] = ti::trsum();

		  auto const& colorSum = differenceBitmap[wrap::toSize(iPoint++)];
		  if (colorSum > colorSumMaximum) {
			colorSumMaximum = colorSum;
		  }
		  if (colorSum < colorSumMinimum) {
			colorSumMinimum = colorSum;
		  }
		}
	  }
	  auto const ratio = (255.0F) / wrap::toFloat(colorSumMaximum - colorSumMinimum);
	  for (auto iPixel = 0; iPixel < bitmap::getBitmapWidth() * bitmap::getBitmapHeight(); ++iPixel) {
		TraceBitmapData[iPixel] &= TraceRGBMask[iRGB];
		if (differenceBitmap[wrap::toSize(iPixel)] != 0U) {
		  auto const adjustedColorSum = wrap::round<uint32_t>(
		      wrap::toFloat(differenceBitmap[wrap::toSize(iPixel)] - colorSumMinimum) * ratio);
		  TraceBitmapData[iPixel] |= adjustedColorSum << TraceShift[iRGB];
		}
	  }
	}
	bitmap::bitbltBitmap();
	StateMap->set(StateFlag::WASDIF);
	StateMap->set(StateFlag::RESTCH);
	ti::tracwnd();
  }
}

#if TRCMTH == 0
unsigned trace::internal::colsum(COLORREF col) {
  ti::trcols(col);
  auto colorSum = 0U;
  for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	if (StateMap->test(TraceRGBFlag[iRGB]))
	  colorSum += PixelColors[iRGB];
  }
  return colorSum;
}

unsigned trace::internal::icolsum(COLORREF col) {
  ti::trcols(col);
  auto colorSum = 0U;
  for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	if (StateMap->test(TraceRGBFlag[iRGB]))
	  colorSum += 255 - PixelColors[iRGB];
  }
  return colorSum;
}
#endif

void trace::trace() {
  if (bitmap::ismap()) {
	trace::untrace();
	ti::tracwnd();
	TraceDataSize = bitmap::getrmap();
	if (thred::inStitchWin() && !StateMap->testAndReset(StateFlag::WASTRCOL)) {
	  auto stitchPoint = thred::pxCor2stch(Msg.pt);
	  if (StateMap->test(StateFlag::LANDSCAP)) {
		auto const bmpSiS = bitmap::getBitmapSizeinStitches();
		stitchPoint.y -= (wrap::toFloat(UnzoomedRect.cy) - bmpSiS.y);
	  }
	  auto const bmpSR = bitmap::getBmpStitchRatio();
	  auto const bitmapPoint =
	      POINT {std::lround(bmpSR.x * stitchPoint.x), std::lround(bmpSR.y * stitchPoint.y - 1.0F)};

	  auto const color = TraceBitmapData[bitmapPoint.y * bitmap::getBitmapWidth() + bitmapPoint.x] ^ 0xffffffU;
	  if (StateMap->test(StateFlag::TRCUP)) {
		UpPixelColor   = color;
		DownPixelColor = PENWHITE;
	  }
	  else {
		DownPixelColor = color;
		UpPixelColor   = 0;
	  }
	  StateMap->set(StateFlag::TRCRED);
	  StateMap->set(StateFlag::TRCGRN);
	  StateMap->set(StateFlag::TRCBLU);
	}
	auto traceColorMask = COLMASK;
	if (!StateMap->test(StateFlag::TRCRED)) {
	  traceColorMask &= REDMSK;
	}
	if (!StateMap->test(StateFlag::TRCGRN)) {
	  traceColorMask &= GRNMSK;
	}
	if (!StateMap->test(StateFlag::TRCBLU)) {
	  traceColorMask &= BLUMSK;
	}
	if (traceColorMask != COLMASK) {
	  for (auto iPixel = 0; iPixel < bitmap::getBitmapWidth() * bitmap::getBitmapHeight(); ++iPixel) {
		TraceBitmapData[iPixel] &= traceColorMask;
	  }
	}

#if TRCMTH == 0
	auto upBrightness   = ti::icolsum(UpPixelColor);
	auto downBrightness = ti::icolsum(DownPixelColor);
	if (TracedMap->empty()) {
	  TracedMap->resize(TraceDataSize, false);
	}
	for (auto index = 0U; index < BitmapWidth * BitmapHeight; ++index) {
	  auto pointBrightness = ti::colsum(TraceBitmapData[index]);
	  if (upBrightness > pointBrightness && downBrightness < pointBrightness)
		TracedMap->set(index);
	  else
		TraceBitmapData[index] = 0;
	}
#endif

#if TRCMTH == 1
	InvertUpColor   = UpPixelColor ^ COLMASK;
	InvertDownColor = DownPixelColor ^ COLMASK;
	ti::trcols(InvertUpColor);
	for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	  HighColors[iRGB] = PixelColors[iRGB];
	}
	ti::trcols(InvertDownColor);
	for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
	  LowColors[iRGB] = PixelColors[iRGB];
	}
	if (TracedMap->empty()) {
	  TracedMap->resize(TraceDataSize, false);
	}
	for (auto iPixel = 0; iPixel < bitmap::getBitmapWidth() * bitmap::getBitmapHeight(); ++iPixel) {
	  if (ti::trcin(TraceBitmapData[iPixel])) {
		TracedMap->set(wrap::toSize(iPixel));
	  }
	  else {
		TraceBitmapData[iPixel] = 0;
	  }
	}
#endif
	StateMap->set(StateFlag::TRSET);
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_MAPLOD);
  }
}

void trace::tracedg() {
  if (!StateMap->test(StateFlag::WASTRAC)) {
	trace::trace();
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
  for (auto iPixel = 0; iPixel < bitmap::getBitmapWidth() * bitmap::getBitmapHeight(); ++iPixel) {
	if (TracedEdges->test(wrap::toSize(iPixel))) {
	  TraceBitmapData[iPixel] = PENWHITE;
	}
	else {
	  TraceBitmapData[iPixel] = 0;
	}
  }
  StateMap->set(StateFlag::RESTCH);
  StateMap->set(StateFlag::WASEDG);
}

auto trace::internal::trcbit(uint32_t const          initialDirection,
                             uint32_t&               traceDirection,
                             std::vector<TRACE_PNT>& tracedPoints) -> bool {
  auto pixelIndex = CurrentTracePoint.y * bitmap::getBitmapWidth() + CurrentTracePoint.x;
  switch (traceDirection) {
	case TRCR: {
	  pixelIndex += (1 - bitmap::getBitmapWidth());
	  if (CurrentTracePoint.x == gsl::narrow<int32_t>(bitmap::getBitmapWidth()) - 1) {
		traceDirection = TRCU;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) {
		  ++(CurrentTracePoint.x);
		  --(CurrentTracePoint.y);
		  traceDirection = TRCD;
		}
		else {
		  pixelIndex += bitmap::getBitmapWidth();
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) {
			++(CurrentTracePoint.x);
		  }
		  else {
			traceDirection = TRCU;
		  }
		}
	  }
	  break;
	}
	case TRCD: {
	  pixelIndex -= (bitmap::getBitmapWidth() + 1);
	  if (CurrentTracePoint.y == 0) {
		traceDirection = TRCR;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) {
		  --(CurrentTracePoint.x);
		  --(CurrentTracePoint.y);
		  traceDirection = TRCL;
		}
		else {
		  ++pixelIndex;
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) {
			--(CurrentTracePoint.y);
		  }
		  else {
			traceDirection = TRCR;
		  }
		}
	  }
	  break;
	}
	case TRCL: {
	  pixelIndex += (bitmap::getBitmapWidth() - 1);
	  if (CurrentTracePoint.x == 0) {
		traceDirection = TRCD;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) {
		  --(CurrentTracePoint.x);
		  ++(CurrentTracePoint.y);
		  traceDirection = TRCU;
		}
		else {
		  pixelIndex -= bitmap::getBitmapWidth();
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) {
			--(CurrentTracePoint.x);
		  }
		  else {
			traceDirection = TRCD;
		  }
		}
	  }
	  break;
	}
	case TRCU: {
	  pixelIndex += (1 + bitmap::getBitmapWidth());
	  if (CurrentTracePoint.y == bitmap::getBitmapHeight() - 1) {
		traceDirection = TRCL;
	  }
	  else {
		if (TracedEdges->test(wrap::toSize(pixelIndex))) {
		  ++(CurrentTracePoint.x);
		  ++(CurrentTracePoint.y);
		  traceDirection = TRCR;
		}
		else {
		  --pixelIndex;
		  if (TracedEdges->test(wrap::toSize(pixelIndex))) {
			++(CurrentTracePoint.y);
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
	tracedPoints.push_back(TRACE_PNT {gsl::narrow<int16_t>(CurrentTracePoint.x),
	                                  gsl::narrow<int16_t>(CurrentTracePoint.y)});
	if (tracedPoints.size() >= POINTMAX) {
	  return false;
	}
  }
  return !(traceDirection == initialDirection && CurrentTracePoint.x == tracedPoints[0].x &&
           CurrentTracePoint.y == tracedPoints[0].y);
}

void trace::internal::dutdif(TRACE_PNT& traceDiff, TRACE_PNT const* point) {
  if (point != nullptr) {
	wrap::narrow(traceDiff.x, point[1].x - point[0].x);
	wrap::narrow(traceDiff.y, point[1].y - point[0].y);
  }
}

void trace::internal::dutrac() {
  if (thred::inStitchWin()) {
	auto stitchPoint = thred::pxCor2stch(Msg.pt);
	if (!StateMap->test(StateFlag::WASEDG)) {
	  trace::tracedg();
	  return;
	}
	thred::savdo();
	if (StateMap->test(StateFlag::LANDSCAP)) {
	  auto const bmpSiS = bitmap::getBitmapSizeinStitches();
	  stitchPoint.y -= (wrap::toFloat(UnzoomedRect.cy) - bmpSiS.y);
	}
	auto const bmpSR = bitmap::getBmpStitchRatio();
	CurrentTracePoint =
	    POINT {std::lround(bmpSR.x * stitchPoint.x), std::lround(bmpSR.y * stitchPoint.y)};
	if (CurrentTracePoint.x > bitmap::getBitmapHeight()) {
	  CurrentTracePoint.x = bitmap::getBitmapWidth();
	}
	if (CurrentTracePoint.y > bitmap::getBitmapHeight()) {
	  CurrentTracePoint.y = bitmap::getBitmapHeight();
	}
	auto const savedPoint = CurrentTracePoint.y * bitmap::getBitmapWidth() + CurrentTracePoint.x;
	auto       traceDirection = 0U;
	if (!TracedEdges->test(wrap::toSize(savedPoint))) {
	  auto point = savedPoint;
	  auto limit = (CurrentTracePoint.y + 1) * bitmap::getBitmapWidth();
	  while (point < limit && !TracedEdges->test(wrap::toSize(point))) {
		++point;
	  }
	  auto const right = (point < limit) ? point - CurrentTracePoint.y * bitmap::getBitmapWidth()
	                                     : bitmap::getBitmapWidth();
	  point            = savedPoint;
	  limit            = CurrentTracePoint.y * bitmap::getBitmapWidth();
	  while (point > limit && !TracedEdges->test(wrap::toSize(point))) {
		--point;
	  }
	  auto const left = (point == limit) ? 0 : point - limit;
	  point           = savedPoint;
	  while (point > 0 && !TracedEdges->test(wrap::toSize(point))) {
		if (point > bitmap::getBitmapWidth()) {
		  point -= bitmap::getBitmapWidth();
		}
		else {
		  point = 0;
		}
	  }
	  auto const bottom = (point > 0) ? point / bitmap::getBitmapWidth() : 0;
	  point             = savedPoint;
	  limit             = bitmap::getBitmapWidth() * bitmap::getBitmapHeight();
	  while (point < limit && !TracedEdges->test(wrap::toSize(point))) {
		point += bitmap::getBitmapWidth();
	  }
	  auto const top = (point < limit) ? point / bitmap::getBitmapWidth() : bitmap::getBitmapHeight();
	  auto       flag                = 0U;
	  auto       minimumEdgeDistance = std::numeric_limits<int32_t>::max();
	  if (left != 0) {
		minimumEdgeDistance = CurrentTracePoint.x - left;
		flag                = TRCL;
	  }
	  if (right < bitmap::getBitmapWidth()) {
		auto const edgeDistance = right - CurrentTracePoint.x;
		if (edgeDistance < minimumEdgeDistance) {
		  minimumEdgeDistance = edgeDistance;
		  flag                = TRCR;
		}
	  }
	  if (bottom != 0) {
		auto const edgeDistance = CurrentTracePoint.y - bottom;
		if (edgeDistance < minimumEdgeDistance) {
		  minimumEdgeDistance = edgeDistance;
		  flag                = TRCD;
		}
	  }
	  if (top < bitmap::getBitmapHeight()) {
		auto const edgeDistance = top - CurrentTracePoint.y;
		if (edgeDistance < minimumEdgeDistance) {
		  flag = TRCU;
		}
	  }
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
	tracedPoints.push_back(TRACE_PNT {gsl::narrow<int16_t>(CurrentTracePoint.x),
	                                  gsl::narrow<int16_t>(CurrentTracePoint.y)});
	while (ti::trcbit(initialDirection, traceDirection, tracedPoints)) { }
	if (tracedPoints.size() >= POINTMAX) {
	  displayText::tabmsg(IDS_FRM2L);
	  return;
	}
#ifndef TESTTRC
	auto decimatedLine = std::vector<TRACE_PNT> {};
	decimatedLine.reserve(tracedPoints.size());
	auto traceDiff = std::array<TRACE_PNT, 2> {};
	decimatedLine.push_back(tracedPoints[0]);
	ti::dutdif(traceDiff[0], tracedPoints.data());
	for (auto iPoint = 1U; iPoint < wrap::toUnsigned(tracedPoints.size()); ++iPoint) {
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
	for (auto iCurrent = 1U; iCurrent < wrap::toUnsigned(decimatedLine.size()); ++iCurrent) {
	  auto const traceLength =
	      wrap::toFloat(std::hypot(decimatedLine[iCurrent].x - decimatedLine[iNextDec].x,
	                               decimatedLine[iCurrent].y - decimatedLine[iNextDec].y));
	  if (traceLength > IniFile.traceLength) {
		tracedPoints.push_back(decimatedLine[iNextDec]);
		iNextDec = iCurrent;
	  }
	}
	for (auto iCurrent = iNextDec + 1U; iCurrent < wrap::toUnsigned(decimatedLine.size()); ++iCurrent) {
	  tracedPoints.push_back(decimatedLine[iCurrent]);
	}
#endif
	auto form        = FRM_HEAD {};
	form.vertexIndex = wrap::toUnsigned(FormVertices->size());
	FormVertices->push_back(F_POINT {wrap::toFloat(tracedPoints[0].x) * StitchBmpRatio.x,
	                                 wrap::toFloat(tracedPoints[0].y) * StitchBmpRatio.y});
	auto iNext           = size_t {};
	auto traceLengthSum  = 0.0F;
	auto landscapeOffset = 0.0F;
	if (StateMap->test(StateFlag::LANDSCAP)) {
	  auto const bmpSiS = bitmap::getBitmapSizeinStitches();
	  landscapeOffset   = wrap::toFloat(UnzoomedRect.cy) - bmpSiS.y;
	}
	for (auto iCurrent = size_t {1U}; iCurrent < tracedPoints.size(); ++iCurrent) {
	  traceLengthSum += hypotf(wrap::toFloat(tracedPoints[iCurrent].x - tracedPoints[iCurrent - 1U].x),
	                           wrap::toFloat(tracedPoints[iCurrent].y - tracedPoints[iCurrent - 1U].y));
	  auto const traceLength = hypotf(wrap::toFloat(tracedPoints[iCurrent].x - tracedPoints[iNext].x),
	                                  wrap::toFloat(tracedPoints[iCurrent].y - tracedPoints[iNext].y));
	  if (traceLengthSum > traceLength * IniFile.traceRatio) {
		FormVertices->push_back(
		    F_POINT {wrap::toFloat(tracedPoints[iCurrent - 1U].x) * StitchBmpRatio.x,
		             wrap::toFloat(tracedPoints[iCurrent - 1U].y) * StitchBmpRatio.y + landscapeOffset});
		--iCurrent;
		iNext          = iCurrent;
		traceLengthSum = 0.0;
	  }
	}
	form.vertexCount     = wrap::toUnsigned(FormVertices->size() - form.vertexIndex);
	form.type            = FRMFPOLY;
	form.attribute       = gsl::narrow<uint8_t>(ActiveLayer << 1U);
	form.satinGuideCount = 0;
	form.outline();
	FormList->push_back(form);
	StateMap->set(StateFlag::RESTCH);
	StateMap->set(StateFlag::FRMOF);
	form::tglfrm();
  }
}

void trace::trinit() {
  auto histogramData = std::array<std::array<uint32_t, LEVELCNT>, CHANLCNT> {};
  if (bitmap::ismap()) {
	if (!StateMap->test(StateFlag::TRSET)) {
	  StateMap->set(StateFlag::TRCRED);
	  StateMap->set(StateFlag::TRCGRN);
	  StateMap->set(StateFlag::TRCBLU);
	  auto componentPeak = std::array<uint32_t, CHANLCNT> {};
	  if (!StateMap->test(StateFlag::WASTRAC)) {
		TraceDataSize = bitmap::getrmap();
	  }
	  if (StateMap->test(StateFlag::MONOMAP)) {
		auto const color     = gsl::narrow<COLORREF>(TraceBitmapData[0]);
		auto       highColor = color;
		auto const spTBD =
		    gsl::span(TraceBitmapData, wrap::toSize(bitmap::getBitmapWidth() * bitmap::getBitmapHeight()));
		auto const pixel = std::find_if(
		    spTBD.begin(), spTBD.end(), [color](uint32_t const& m) -> bool { return m != color; });
		if (pixel != spTBD.end()) {
		  highColor = *pixel;
		}
		ti::trcols(highColor);
		std::copy(PixelColors.begin(), PixelColors.end(), HighColors.begin());
		ti::trcols(color);
		for (auto iRGB = 0U; iRGB < CHANLCNT; ++iRGB) {
		  if (PixelColors[iRGB] > HighColors[iRGB]) {
			std::swap(PixelColors[iRGB], HighColors[iRGB]);
		  }
		  componentPeak[iRGB] = ((HighColors[iRGB] - PixelColors[iRGB]) / 2) + PixelColors[iRGB];
		}
	  }
	  else {
		auto const spTBD =
		    gsl::span(TraceBitmapData, wrap::toSize(bitmap::getBitmapWidth() * bitmap::getBitmapHeight()));
		for (auto const pixel : spTBD) {
		  ti::trcols(pixel);
		  auto iPixelColors = PixelColors.begin();
		  for (auto& iHistogramData : histogramData) {
			auto const itHD = wrap::next(iHistogramData.begin(), *(iPixelColors++));
			++(*itHD);
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
		if (componentPeak[iRGB] != 0U) {
		  --(componentPeak[iRGB]);
		}
		InvertDownColor |= componentPeak[iRGB] << TraceShift[iRGB];
	  }
	  DownPixelColor = InvertDownColor ^ COLMASK;
	  InvertUpColor  = PENWHITE;
	  UpPixelColor   = 0U;
	}
	StateMap->set(StateFlag::WASTRCOL);
	trace::trace();
  }
  else {
	displayText::tabmsg(IDS_MAPLOD);
  }
}

void trace::trcsel() {
  if (bitmap::ismap()) {
	StateMap->set(StateFlag::WASTRCOL);
	StateMap->set(StateFlag::TRCRED);
	StateMap->set(StateFlag::TRCBLU);
	StateMap->set(StateFlag::TRCGRN);
	DownPixelColor = PENWHITE;
	UpPixelColor   = 0;
	trace::trace();
	StateMap->reset(StateFlag::HIDMAP);
	StateMap->reset(StateFlag::TRSET);
	for (auto iPixel = 0; iPixel < bitmap::getBitmapWidth() * bitmap::getBitmapHeight(); ++iPixel) {
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
	bitmap::bitbltBitmap();
	StateMap->set(StateFlag::WASDSEL);
	StateMap->set(StateFlag::RESTCH);
	ti::tracwnd();
  }
  else {
	displayText::tabmsg(IDS_MAPLOD);
  }
}

void trace::internal::ritrcol(COLORREF* color, uint32_t number) noexcept {
  if (color != nullptr) {
	*color &= TraceRGBMask[ColumnColor];
	number &= BYTMASK;
	*color |= (number << TraceShift[ColumnColor]);
  }
}

void trace::internal::dutrnum0(uint32_t color) {
  StateMap->reset(StateFlag::NUMIN);
  StateMap->reset(StateFlag::TRNIN0);
  if (StateMap->test(StateFlag::TRNUP)) {
	auto const itTraceUpWindow = wrap::next(TraceUpWindow.begin(), ColumnColor);
	ti::ritrcol(&InvertUpColor, color);
	UpPixelColor = InvertUpColor ^ COLMASK;
	thred::redraw(*itTraceUpWindow);
  }
  else {
	auto const itTraceDownWindow = wrap::next(TraceDownWindow.begin(), ColumnColor);
	ti::ritrcol(&InvertDownColor, color);
	DownPixelColor = InvertDownColor ^ COLMASK;
	thred::redraw(*itTraceDownWindow);
  }
  auto const itTraceControlWindow = wrap::next(TraceControlWindow.begin(), ColumnColor);
  thred::redraw(*itTraceControlWindow);
  DestroyWindow(TraceNumberInput);
  StateMap->set(StateFlag::WASTRCOL);
  trace::trace();
}

void trace::dutrnum2() {
  ti::dutrnum0(wrap::toUnsigned(std::wcstol(TraceInputBuffer.data(), nullptr, 10)));
}

void trace::dutrnum1() {
  DestroyWindow(GeneralNumberInputBox);
  StateMap->reset(StateFlag::NUMIN);
  StateMap->reset(StateFlag::TRNIN1);
  auto traceLength = thred::getMsgBufferValue();
  if (traceLength > MAXSIZ) {
	traceLength = MAXSIZ;
  }
  if (StateMap->test(StateFlag::TRNUP)) {
	IniFile.traceLength = traceLength * PFGRAN;
	ti::trcstpnum();
  }
  else {
	IniFile.traceRatio = 1.0F + pow(0.1F, traceLength);
	ti::trcratnum();
  }
}

auto trace::internal::ducolm() -> uint32_t {
  if (TraceMsgPoint.x < gsl::narrow<int32_t>(ButtonWidth)) {
	return 0U;
  }
  if (TraceMsgPoint.x < gsl::narrow<int32_t>(ButtonWidth) * 2) {
	return 1U;
  }

  return 2U;
}

void trace::internal::trnumwnd0(int32_t position) noexcept {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  constexpr auto DW_STYLE = DWORD {SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER};
  TraceNumberInput        = CreateWindowEx(
      0L, L"STATIC", nullptr, DW_STYLE, ButtonWidthX3, position, ButtonWidth, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
}

void trace::internal::trnumwnd1(int32_t position) noexcept {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  constexpr auto DW_STYLE = DWORD {WS_CHILD | WS_VISIBLE | WS_BORDER};
  GeneralNumberInputBox   = CreateWindowEx(
      0L, L"STATIC", nullptr, DW_STYLE, ButtonWidthX3, position, ButtonWidthX3, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
}

void trace::blak() {
  if (!bitmap::ismap()) {
	displayText::tabmsg(IDS_MAPLOD);
	return;
  }
  ti::tracwnd();
  if (!FormList->empty()) {
	BlackPen = wrap::createPen(PS_SOLID, PENNWID, PENBLK);
	SelectObject(bitmap::getBitmapDC(), BlackPen);
	SelectObject(bitmap::getTraceDC(), BlackPen);
	if (!StateMap->test(StateFlag::WASTRAC)) {
	  TraceDataSize = bitmap::getrmap();
	}
	for (auto& iForm : *FormList) {
	  bitmap::bfrm(iForm);
	}
	DeleteObject(BlackPen);
	StateMap->set(StateFlag::WASBLAK);
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_FRMNO);
  }
}

void trace::tracpar() {
  if (StateMap->test(StateFlag::TRNIN0)) {
	trace::dutrnum2();
  }
  if (StateMap->test(StateFlag::TRNIN1)) {
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
	  auto const itTraceControlWindow = wrap::next(TraceControlWindow.begin(), ColumnColor);
	  thred::redraw(*itTraceControlWindow);
	  trace::trace();
	}
	else {
	  auto const position = wrap::floor<uint32_t>(TraceMsgPoint.y / ButtonHeight);
	  if (position < 16U) {
		StateMap->flip(TraceRGBFlag[ColumnColor]);
		auto const itTraceSelectWindow = wrap::next(TraceSelectWindow.begin(), ColumnColor);
		thred::redraw(*itTraceSelectWindow);
		trace::trace();
	  }
	  else {
		if (position < 18U) {
		  StateMap->set(StateFlag::NUMIN);
		  StateMap->set(StateFlag::TRNIN0);
		  TraceMsgIndex       = 0;
		  TraceInputBuffer[0] = 0;
		  if (position < 17U) {
			ti::trnumwnd0(ButtonHeight * 16);
			StateMap->set(StateFlag::TRNUP);
		  }
		  else {
			ti::trnumwnd0(ButtonHeight * 17);
			StateMap->reset(StateFlag::TRNUP);
		  }
		}
		else {
		  if (position < 20) {
			StateMap->set(StateFlag::NUMIN);
			StateMap->set(StateFlag::TRNIN1);
			TraceMsgIndex       = 0;
			TraceInputBuffer[0] = 0;
			if (position < 19U) {
			  ti::trnumwnd1(ButtonHeight * 18);
			  StateMap->set(StateFlag::TRNUP);
			}
			else {
			  ti::trnumwnd1(ButtonHeight * 19);
			  StateMap->reset(StateFlag::TRNUP);
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

void trace::internal::trcnum(uint32_t shift, COLORREF color, uint32_t backColor) {
  auto const zeroWidth = thred::txtWid(L"0");
  color >>= shift;
  color &= BYTMASK;
  auto const val       = std::to_wstring(color);
  auto const xPosition = zeroWidth.cx * gsl::narrow<int32_t>((3U - val.size()) + 1U);
  SetBkColor(DrawItem->hDC, backColor);
  wrap::textOut(DrawItem->hDC, xPosition, 1, val.c_str(), wrap::toUnsigned(val.size()));
}

void trace::internal::durct(uint32_t    shift,
                            RECT const& traceControlRect,
                            RECT&       traceHighMask,
                            RECT&       traceMiddleMask,
                            RECT&       traceLowMask) {
  auto const lowerColor    = gsl::narrow_cast<byte>((UpPixelColor >> shift) & 0xffU);
  auto const upperColor    = gsl::narrow_cast<byte>((DownPixelColor >> shift) & 0xffU);
  auto const controlHeight = wrap::toFloat(traceControlRect.bottom - traceControlRect.top);
  auto       ratio         = wrap::toFloat(lowerColor) * CCRATIO;
  traceHighMask.left = traceLowMask.left = traceMiddleMask.left = traceControlRect.left;
  traceHighMask.right = traceLowMask.right = traceMiddleMask.right = traceControlRect.right;
  traceMiddleMask.top    = std::lround(controlHeight * ratio + wrap::toFloat(traceControlRect.top));
  ratio                  = wrap::toFloat(upperColor) * CCRATIO;
  traceMiddleMask.bottom = std::lround(controlHeight * ratio + wrap::toFloat(traceControlRect.top));
  StateMap->reset(StateFlag::DUHI);
  StateMap->reset(StateFlag::DULO);
  if (lowerColor != 0U) {
	StateMap->set(StateFlag::DULO);
	traceLowMask.bottom = traceMiddleMask.top;
	traceLowMask.top    = 0;
  }
  if (upperColor != BYTEMAXV) {
	StateMap->set(StateFlag::DUHI);
	traceHighMask.top    = traceMiddleMask.bottom;
	traceHighMask.bottom = traceControlRect.bottom;
  }
}

void trace::internal::dublk(HDC dc, RECT const& traceHighMask, RECT const& traceLowMask, HBRUSH brush) {
  if (StateMap->test(StateFlag::DUHI)) {
	FillRect(dc, &traceHighMask, brush);
  }
  if (StateMap->test(StateFlag::DULO)) {
	FillRect(dc, &traceLowMask, brush);
  }
}

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
  auto       iRGB                = 0U;
  for (auto const& brush : TraceBrush) {
	if (DrawItem->hwndItem == *(iTraceUpWindow++)) {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, brush);
	  ti::trcnum(*iTraceShift, InvertUpColor, *iTraceRGB);
	  break;
	}
	if (DrawItem->hwndItem == *(iTraceDownWindow++)) {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, brush);
	  ti::trcnum(*iTraceShift, InvertDownColor, *iTraceRGB);
	}
	if (DrawItem->hwndItem == *(iTraceControlWindow++)) {
	  ti::durct(*iTraceShift, DrawItem->rcItem, traceHighMaskRect, traceMiddleMaskRect, traceLowMaskRect);
	  FillRect(DrawItem->hDC, &traceMiddleMaskRect, brush);
	  ti::dublk(DrawItem->hDC, traceHighMaskRect, traceLowMaskRect, blackBrush);
	  break;
	}
	if (DrawItem->hwndItem == *(iTraceSelectWindow++)) {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto tempBrush = blackBrush;
	  SetBkColor(DrawItem->hDC, 0);
	  SetTextColor(DrawItem->hDC, *iTraceRGB);
	  if (StateMap->test(*iTraceRGBFlag)) {
		tempBrush = brush;
		SetTextColor(DrawItem->hDC, 0);
		SetBkColor(DrawItem->hDC, *iTraceRGB);
	  }
#pragma warning(suppress : 26812) // Enum.3 prefer 'enum class' over 'enum'
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, tempBrush);
	  auto const strOnOff = displayText::loadStr((StateMap->test(*iTraceRGBFlag)) ? IDS_ON : IDS_OFF);
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
	++iRGB;
  }
}

void trace::wasTrace1() {
  if (StateMap->test(StateFlag::TRNIN0)) {
	trace::dutrnum2();
  }
  if (StateMap->test(StateFlag::TRNIN1)) {
	trace::dutrnum1();
  }
  if (!StateMap->test(StateFlag::WASEDG)) {
	trace::tracpar();
  }
}

void trace::traceNumberInput(wchar_t NumericCode) {
  auto itTraceInputBuffer = wrap::next(TraceInputBuffer.begin(), TraceMsgIndex);
  *(itTraceInputBuffer++) = NumericCode;
  *itTraceInputBuffer     = 0;
  ++TraceMsgIndex;
  auto traceColor = wrap::toUnsigned(std::wcstol(TraceInputBuffer.data(), nullptr, 10));
  switch (TraceMsgIndex) {
	case 2: {
	  if (traceColor > 25) {
		ti::dutrnum0(traceColor);
	  }
	  break;
	}
	case 3: {
	  if (traceColor > BYTEMAXV) {
		traceColor = BYTEMAXV;
	  }
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

void trace::traceNumberReset() {
  auto const itTraceInputBuffer = wrap::next(TraceInputBuffer.begin(), TraceMsgIndex);
  *itTraceInputBuffer           = 0;
  thred::redraw(TraceNumberInput);
}
