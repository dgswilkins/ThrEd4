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

// Open Source headers

// Local Headers
#include "ThrEdTypes.h"

namespace formForms {
void dazdef();
void dasyfrm();
void frmnum();
void maxtsiz(std::wstring const& label, POINT& textSize);
auto maxwid(uint32_t start, uint32_t finish);
void prfmsg();
void prfsid(HWND wnd) noexcept;
void refrm();
void setear();
void sidwnd(HWND wnd) noexcept;
void wavfrm();

namespace internal {
  void chkdaz();

  auto CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  void initdaz(HWND hWinDialog);
  void initTearDlg(HWND hwndlg);
  auto numwin(std::wstring const& winName, RECT const& location) -> HWND;
  void nxtlin(uint32_t& formMenuEntryCount) noexcept;
  void nxtlinprf() noexcept;
  void prflin(std::wstring const& msg, uint32_t row) noexcept;
  auto prfnwin(std::wstring const& text) noexcept -> HWND;
  void prftwin(std::wstring const& text) noexcept;
  void refrmfn(FRMHED const& form, uint32_t& formMenuEntryCount);

  auto CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  auto txtrwin(std::wstring const& winName, RECT const& location) -> HWND;
  auto txtwin(std::wstring const& windowName, RECT const& location) -> HWND;
  void wavinit(HWND hwndlg);

  auto CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;
} // namespace internal
} // namespace formForms
