// Local Headers
#include "stdafx.h"
#include "globals.h"
#include "point.h"
#include "thred.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/narrow"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <fileapi.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <WinBase.h>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <cstdint>
#include <cwchar>
#include <vector>

auto wrap::wcsToFloat(wchar_t const* buffer) -> float {
  try {
	return std::wcstof(buffer, nullptr);
  }
  catch (...) {
	outDebugString(L"wcstof failed trying to convert '{}'\n", buffer);
	return 0.0F;
  }
}

// ReSharper disable CppParameterMayBeConst
void wrap::polyline(HDC hdc, POINT const* apt, uint32_t const cpt) noexcept {
  Polyline(hdc, apt, gsl::narrow<int32_t>(cpt));
}
// ReSharper restore CppParameterMayBeConst

auto wrap::pressed(int const virtKey) noexcept -> bool {
  constexpr auto HIGHBIT = 0x8000U;
  return (gsl::narrow_cast<uint16_t>(GetKeyState(virtKey)) & HIGHBIT) != 0U;
}

// ReSharper disable CppParameterMayBeConst
void wrap::writeFile(HANDLE file, LPCVOID buffer, uint32_t const bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept {
  WriteFile(file, buffer, gsl::narrow<DWORD>(bytesToWrite), bytesWritten, overlapped);
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void wrap::getTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t const iLen, LPSIZE lpSize) noexcept {
  GetTextExtentPoint32(hdc, lpString, gsl::narrow<int32_t>(iLen), lpSize);
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void wrap::textOut(HDC hdc, int32_t const nXStart, int32_t const nYStart, LPCTSTR lpString, uint32_t const cchString) noexcept {
  TextOut(hdc, nXStart, nYStart, lpString, gsl::narrow<int32_t>(cchString));
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void wrap::setCursor(HCURSOR hCursor) noexcept {
  static HCURSOR saveCursor = nullptr;
  if (saveCursor == hCursor) {
	return;
  }
  SetCursor(hCursor);
  saveCursor = hCursor;
}
// ReSharper restore CppParameterMayBeConst

auto wrap::createPen(int32_t const iStyle, int32_t const width, COLORREF const color) noexcept -> HPEN {
  auto const scaledWidth = thred::duScale(width);
  return CreatePen(iStyle, scaledWidth, color);
}

auto wrap::getFormVertices() noexcept -> std::vector<F_POINT>& {
  return Instance->formVertices;
}
