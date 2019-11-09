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
#pragma warning(disable : 26814)
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
	void initTearDlg(HWND hwndlg);
	auto numwin(const std::wstring& winName, const RECT& location) -> HWND;
	void nxtlin(uint32_t& formMenuEntryCount) noexcept;
	void nxtlinprf() noexcept;
	void prflin(const std::wstring& msg, uint32_t row) noexcept;
	auto prfnwin(const std::wstring& text) noexcept -> HWND;
	void prftwin(const std::wstring& text) noexcept;
	void refrmfn(const FRMHED& form, uint32_t& formMenuEntryCount);

	auto CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

	auto txtrwin(const std::wstring& winName, const RECT& location) -> HWND;
	auto txtwin(const std::wstring& windowName, const RECT& location) -> HWND;
	void wavinit(HWND hwndlg);

	auto CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;
} // namespace internal
} // namespace formForms
