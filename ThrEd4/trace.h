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

	void bfrm();
	void blanklin(std::vector<uint32_t>& differenceBitmap, uint32_t lineStart);
	void difbits(uint32_t shift, uint32_t* point) noexcept;

	static inline void difsub(uint32_t source, uint32_t shift, uint32_t& destination) noexcept;

	void     dublk(HDC dc, const RECT& traceHighMask, const RECT& traceLowMask, HBRUSH brush);
	uint32_t ducolm() noexcept;

	void durct(uint32_t shift, const RECT& traceControlRect, RECT& traceHighMask, RECT& traceMiddleMask, RECT& traceLowMask);
	void dutdif(TRCPNT& traceDiff, const TRCPNT* point) noexcept;
	void dutrac();
	void dutrnum0(uint32_t color);
	void dwnum(uint32_t iRGB) noexcept;
	void getrmap();
	void hidwnd(HWND hwnd) noexcept;

	void pxlin(uint32_t start, uint32_t finish);
	void ritrcol(COLORREF* color, uint32_t number) noexcept;
	void shownd(HWND hwnd) noexcept;
	void stch2bit(fPOINT& point);
	void tracwnd();
	bool trcbit(uint32_t initialDirection, uint32_t& traceDirection, std::vector<TRCPNT>& tracedPoints);
	bool trcin(COLORREF color);
	void trcnum(uint32_t shift, COLORREF color, uint32_t iRGB);
	void trcols(COLORREF color) noexcept;
	void trcratnum();
	void trcstpnum();
	void trcsub(HWND* window, uint32_t xCoordinate, uint32_t yCoordinate, uint32_t buttonHeight) noexcept;
	void trnumwnd0(int32_t position) noexcept;
	void trnumwnd1(int32_t position) noexcept;

	uint32_t trsum() noexcept;

	void upnum(uint32_t iRGB);
} // namespace internal
} // namespace trace
