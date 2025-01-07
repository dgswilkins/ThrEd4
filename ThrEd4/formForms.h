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
#include <string>

namespace formForms {
void dasyfrm();
void destroyFormDataSheet() noexcept;
void destroyPreferencesWindow() noexcept;
void frmnum();
void getFormDataSheetRect(RECT& dataRect) noexcept;
void getPreferencesRect(RECT& prefRect) noexcept;
auto inFormDataSheet() noexcept -> bool;
void maxtsiz(std::wstring const& label, SIZE& textSize) noexcept(std::is_same_v<size_t, uint32_t>);
void prfmsg();
void prfsid(HWND wnd);
void redrawFormDataSheet() noexcept;
void redrawPreferencesWindow() noexcept;
void refrm();
void setear();
void sidwnd(HWND wnd);
void wavfrm();
} // namespace formForms
