#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <string>

namespace formForms {
void dasyfrm();
void frmnum();
void maxtsiz(std::wstring const& label, SIZE& textSize);
void prfmsg();
void prfsid(HWND wnd);
void refrm();
void setear();
void sidwnd(HWND wnd);
void wavfrm();
} // namespace formForms
