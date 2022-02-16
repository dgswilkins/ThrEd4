#pragma once

// Local Headers
#include "formHeader.h"

// Open Source headers

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cstdint>
#include <string>

namespace formForms {
void dazdef();
void defWav() noexcept;
void dasyfrm();
void frmnum();
void maxtsiz(std::wstring const& label, SIZE& textSize);
void prfmsg();
void prfsid(HWND wnd);
void refrm();
void setear();
void setDaisy();
void setWav();
void sidwnd(HWND wnd);
void wavfrm();

namespace internal {
  void chkdaz();

  auto CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  void initdaz(HWND hWinDialog);
  void initTearDlg(HWND hwndlg);
  auto maxwid() -> SIZE;
  auto numwin(std::wstring const& winName, RECT const& location) -> HWND;
  void nxtlin(uint32_t& formMenuEntryCount) noexcept;
  void nxtlinprf() noexcept;
  void prflin(std::wstring const& msg, LIST_TYPE const& row);
  auto prfnwin(std::wstring const& text) noexcept -> HWND;
  void prftwin(std::wstring const& text) noexcept;
  void refrmfn(FRM_HEAD& form, uint32_t& formMenuEntryCount);

  auto CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  auto txtrwin(std::wstring const& winName, RECT const& location) -> HWND;
  auto txtwin(std::wstring const& windowName, RECT const& location) -> HWND;
  void wavinit(HWND hwndlg);

  auto CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;
} // namespace internal
} // namespace formForms
