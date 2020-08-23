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

template <class outType, class inType> auto ceil(inType invar) -> outType {
  return gsl::narrow<outType>(std::ceil(invar));
}

auto CreatePen(int32_t iStyle, int32_t width, COLORREF color) noexcept -> HPEN;

template <class outType, class inType> auto floor(inType invar) -> outType {
  return gsl::narrow<outType>(std::floor(invar));
}

void GetTextExtentPoint(HDC hdc, LPCTSTR lpString, uint32_t cbString, LPSIZE lpSize) noexcept;
void GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t c, LPSIZE lpSize) noexcept;

template <class inType> auto midl(inType high, inType low) noexcept -> float {
  return (gsl::narrow_cast<float>(high) - gsl::narrow_cast<float>(low)) / 2.0F + gsl::narrow_cast<float>(low);
}

template <class outType, class inType> void narrow(outType& outvar, inType invar) {
  outvar = gsl::narrow<outType>(invar);
}

template <class outType, class inType> void narrow_cast(outType& outvar, inType invar) noexcept {
  outvar = gsl::narrow_cast<outType>(invar);
}

void Polyline(HDC hdc, POINT const* apt, uint32_t cpt) noexcept;
auto pressed(int virtKey) noexcept -> bool;
auto ReadFile(HANDLE file, LPVOID buffer, uint32_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) noexcept
    -> bool;

template <class outType, class inType> auto round(inType invar) -> outType {
  return gsl::narrow<outType>(std::round(invar));
}

void setCursor(HCURSOR hCursor) noexcept;
void setSideWinVal(int index) noexcept;
void TextOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept;
auto toFloat(double invar) -> float;
auto toFloat(int32_t invar) noexcept -> float;
auto toFloat(LONG invar) noexcept -> float;
auto toFloat(unsigned long invar) noexcept -> float;
auto toFloat(uint32_t invar) noexcept -> float;
auto toFloat(int16_t invar) noexcept -> float;
auto toSize(uint32_t invar) noexcept -> size_t;
auto toSize(DWORD invar) noexcept -> size_t;
auto toSize(int invar) -> size_t;
auto toSize(long invar) noexcept -> size_t;
auto toSize(uintmax_t invar) noexcept -> size_t;
auto toUnsigned(size_t invar) -> uint32_t;
auto toUnsigned(int invar) -> uint32_t;
auto wcstof(wchar_t const* buffer) -> float;

template <class outType> void wcstoul(outType& outvar, wchar_t const* invar) {
  outvar = gsl::narrow<outType>(std::wcstoul(invar, nullptr, 10));
}

template <class outType> auto wcstol(wchar_t const* buffer) -> outType {
  return gsl::narrow<outType>(std::wcstol(buffer, nullptr, 10));
}

void WriteFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;
} // namespace wrap
