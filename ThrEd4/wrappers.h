#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace wrap {
auto bufToFloat(wchar_t const* buffer) -> float;
auto CreatePen(int32_t iStyle, uint32_t width, COLORREF color) noexcept -> HPEN;
void GetTextExtentPoint(HDC hdc, LPCTSTR lpString, uint32_t cbString, LPSIZE lpSize) noexcept;
void GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t c, LPSIZE lpSize) noexcept;

template <class inType> auto midl(inType high, inType low) noexcept -> float {
  return (gsl::narrow_cast<float>(high) - gsl::narrow_cast<float>(low)) / 2.0F + gsl::narrow_cast<float>(low);
}

void Polyline(HDC hdc, POINT const* apt, uint32_t cpt) noexcept;
auto pressed(int virtKey) noexcept -> bool;
auto ReadFile(HANDLE file, LPVOID buffer, uint32_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) noexcept
    -> bool;
void setCursor(HCURSOR hCursor) noexcept;
void setSideWinVal(int index) noexcept;
void TextOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept;
auto toFloat(double invar) -> float;
auto toFloat(int32_t invar) noexcept -> float;
auto toFloat(LONG invar) noexcept -> float;
auto toFloat(uint32_t invar) noexcept -> float;
auto toSize(uint32_t invar) noexcept -> size_t;
auto toUnsigned(size_t invar) -> uint32_t;
auto wcstof(wchar_t const (&buffer)[HBUFSIZ]) noexcept -> float;
void WriteFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;

template <class outType, class inType> auto ceil(inType invar) -> outType {
  return gsl::narrow<outType>(std::ceil(invar));
}

template <class outType, class inType> auto floor(inType invar) -> outType {
  return gsl::narrow<outType>(std::floor(invar));
}

template <class outType, class inType> auto round(inType invar) -> outType {
  return gsl::narrow<outType>(std::round(invar));
}

template <class outType> auto wcstoi(wchar_t const (&buffer)[HBUFSIZ]) -> outType {
  return gsl::narrow<outType>(std::wcstoul(static_cast<wchar_t const*>(buffer), nullptr, 10));
}

} // namespace wrap
