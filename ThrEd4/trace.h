#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

namespace trace {

void blak();
void dutrnum1();
void dutrnum2();
void initColorRef() noexcept;
void initTraceWindows() noexcept;
void trace();
void tracedg();
void traceNumberInput(uint32_t NumericCode);
void traceNumberReset() noexcept;
void tracpar();
void trcsel();
void trdif();
void trinit();
void untrace();
void wasTrace();
void wasTrace1();

namespace internal {

  void blanklin(std::vector<uint32_t>& differenceBitmap, uint32_t lineStart) noexcept;
  void difbits(uint32_t shift, uint32_t* point) noexcept;

  static inline void difsub(uint32_t source, uint32_t shift, uint32_t& destination) noexcept;

  void dublk(HDC dc, RECT const& traceHighMask, RECT const& traceLowMask, HBRUSH brush);
  auto ducolm() -> uint32_t;
  void durct(uint32_t shift, RECT const& traceControlRect, RECT& traceHighMask, RECT& traceMiddleMask, RECT& traceLowMask);
  void dutdif(TRCPNT& traceDiff, TRCPNT const* point) noexcept;
  void dutrac();
  void dutrnum0(uint32_t color);
  void dwnum(uint32_t iRGB);
  void hidwnd(HWND hwnd) noexcept;
  void ritrcol(COLORREF* color, uint32_t number) noexcept;
  void shownd(HWND hwnd) noexcept;
  void tracwnd();
  auto trcbit(uint32_t initialDirection, uint32_t& traceDirection, std::vector<TRCPNT>& tracedPoints) -> bool;
  auto trcin(COLORREF color) -> bool;
  void trcnum(uint32_t shift, COLORREF color, uint32_t iRGB);
  void trcols(COLORREF color) noexcept;
  void trcratnum();
  void trcstpnum();
  void trcsub(HWND* window, uint32_t xCoordinate, uint32_t yCoordinate, uint32_t buttonHeight) noexcept;
  void trnumwnd0(int32_t position) noexcept;
  void trnumwnd1(int32_t position) noexcept;
  auto trsum() noexcept -> uint32_t;
  void upnum(uint32_t iRGB);

#if TRCMTH == 0
  auto colsum(COLORREF col) -> uint32_t;
  auto icolsum(COLORREF col) -> uint32_t;
#endif
} // namespace internal
} // namespace trace
