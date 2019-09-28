#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <vector>

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace formForms {

void dazdef();
void dasyfrm();
void frmnum();
void maxtsiz(const std::wstring& label, POINT& textSize);
auto maxwid(uint32_t start, uint32_t finish);
void prfmsg();
void prfsid(HWND wnd) noexcept;
void refrm();
void setear();
void sidwnd(HWND wnd) noexcept;
void wavfrm();

namespace internal {

	void chkdaz();

	BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void initdaz(HWND hWinDialog);

	inline void initTearDlg(HWND hwndlg);

	HWND numwin(const std::wstring& winName, const RECT& location);
	void nxtlin(uint32_t& formMenuEntryCount) noexcept;
	void nxtlinprf() noexcept;
	void prflin(const std::wstring& msg, uint32_t row) noexcept;
	HWND prfnwin(const std::wstring& text) noexcept;
	void prftwin(const std::wstring& text) noexcept;
	void refrmfn(uint32_t& formMenuEntryCount);

	bool CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	HWND txtrwin(const std::wstring& winName, const RECT& location);
	HWND txtwin(const std::wstring& windowName, const RECT& location);

	inline void wavinit(HWND hwndlg);

	bool CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);
} // namespace internal
} // namespace formForms
