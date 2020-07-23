#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header
#include <iostream>

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

// Local Headers
#include "globals.h"
#include "switches.h"
#include "wrappers.h"

auto wrap::bufToFloat(wchar_t const* buffer) -> float {
  auto value = 0.0F;
  try {
	value = std::stof(std::wstring(buffer));
  }
  catch (...) {
	outDebugString(L"bufToFloat:stof failed trying to convert '{}'\n", buffer);
	value = 0.0F;
  }
  return value;
}

void wrap::Polyline(HDC hdc, POINT const* apt, uint32_t cpt) noexcept {
  Polyline(hdc, apt, gsl::narrow<int32_t>(cpt));
}

auto wrap::pressed(int virtKey) noexcept -> bool {
  return (gsl::narrow_cast<uint16_t>(GetKeyState(virtKey)) & 0x8000U) != 0U;
}

void wrap::WriteFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept {
  WriteFile(file, buffer, gsl::narrow<DWORD>(bytesToWrite), bytesWritten, overlapped);
}

auto wrap::ReadFile(HANDLE file, LPVOID buffer, uint32_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) noexcept
    -> bool {
  return (ReadFile(file, buffer, gsl::narrow<DWORD>(bytesToRead), bytesRead, overlapped) == TRUE);
}

void wrap::GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t c, LPSIZE lpSize) noexcept {
  GetTextExtentPoint32(hdc, lpString, gsl::narrow<int32_t>(c), lpSize);
}

void wrap::TextOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept {
  TextOut(hdc, nXStart, nYStart, lpString, gsl::narrow<int32_t>(cchString));
}

void wrap::GetTextExtentPoint(HDC hdc, LPCTSTR lpString, uint32_t cbString, LPSIZE lpSize) noexcept {
  GetTextExtentPoint(hdc, lpString, gsl::narrow<int32_t>(cbString), lpSize);
}

auto wrap::toFloat(double invar) -> float {
  auto result = 0.0F;
  try {
	result = gsl::narrow<float>(invar);
  }
  catch (gsl::narrowing_error const& e) { // check if we are seeing a rounding error
	UNREFERENCED_PARAMETER(e);
	auto const var  = gsl::narrow_cast<float>(invar);
	auto const diff = abs(invar - var);
	if (diff < 4e-5) {
	  result = var;
	}
	else {
	  throw;
	}
  }
  catch (...) { // otherwise throw
	throw;
  }
  return result;
}

auto wrap::toFloat(int32_t invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

auto wrap::toFloat(LONG invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

auto wrap::toFloat(uint32_t invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

#pragma warning(suppress : 26497 26440) // since this is used in other translation units, "constexpr"
                                        // cannot be used as it implicitly defines the function as inline.
auto wrap::toUnsigned(size_t invar) -> uint32_t {
#ifdef _WIN64
  return gsl::narrow<uint32_t>(invar);
#else
  return invar;
#endif
}

auto wrap::toSize(uint32_t invar) noexcept -> size_t {
  return gsl::narrow_cast<size_t>(invar);
}

auto wrap::wcstof(wchar_t const (&buffer)[HBUFSIZ]) noexcept -> float {
  return std::wcstof(static_cast<wchar_t const*>(buffer), nullptr);
}

void wrap::setCursor(HCURSOR hCursor) noexcept {
  static HCURSOR saveCursor = nullptr;
  if (saveCursor != hCursor) {
	SetCursor(hCursor);
	saveCursor = hCursor;
  }
}

auto wrap::CreatePen(int32_t iStyle, uint32_t width, COLORREF color) noexcept -> HPEN {
  auto const scaledWidth = MulDiv(width, *screenDPI, 96);
  return ::CreatePen(iStyle, scaledWidth, color);
}

void wrap::setSideWinVal(int index) noexcept {
  SetWindowText(ValueWindow->operator[](index), SideWindowEntryBuffer.data());
}
