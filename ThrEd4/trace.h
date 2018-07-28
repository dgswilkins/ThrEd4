#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// void               bfrm();
void blak();
// void               blanklin(std::vector<unsigned>& differenceBitmap, unsigned lineStart);
// unsigned           colsum(COLORREF color);
// void               difbits(unsigned shift, unsigned* point);
// static inline void difsub(const unsigned source, unsigned shift, unsigned& destination);
// void               dublk(HDC dc, const RECT& traceHighMask, const RECT& traceLowMask, HBRUSH brush);
// unsigned           ducolm();
// void               durct(unsigned shift, const RECT& traceControlRect, RECT& traceHighMask, RECT& traceMiddleMask, RECT& traceLowMask);  
// void               dutdif(TRCPNT& traceDiff, const TRCPNT* point);  
// void               dutrac();
void dutrnum0(unsigned color);
void dutrnum1();
void dutrnum2();
// void               dwnum(unsigned iRGB);
// void               getrmap();
// void               hidwnd(HWND hwnd);
// unsigned           icolsum(COLORREF color);
void initColorRef() noexcept;
void initTraceWindows() noexcept;
// void               pxlin(unsigned start, unsigned finish);
// void               ritrcol(COLORREF* color, unsigned number);
// void               shownd(HWND hwnd);
// void               stch2bit(fPOINT& point);
void trace();
void tracedg();
void traceNumberInput(unsigned NumericCode);
void traceNumberReset();
void tracpar();
// void               tracwnd();
// bool               trcbit(const unsigned initialDirection, unsigned& traceDirection, std::vector<TRCPNT>& tracedPoints);
// bool               trcin(COLORREF color);
// void               trcnum(unsigned shift, COLORREF color, unsigned iRGB);
// void               trcols(COLORREF color);
// void               trcratnum();
void trcsel();
// void               trcstpnum();
void trdif();
void trinit();
// void               trnumwnd0(int position);
// void               trnumwnd1(int position);
// constexpr unsigned trsum();
void untrace();
// void               upnum(unsigned iRGB);
void wasTrace();
void wasTrace1();
