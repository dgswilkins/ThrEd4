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
auto bufToFloat(const wchar_t* buffer) -> float;
auto CreatePen(int32_t iStyle, uint32_t width, COLORREF color) noexcept -> HPEN;
void GetTextExtentPoint(HDC hdc, LPCTSTR lpString, uint32_t cbString, LPSIZE lpSize) noexcept;
void GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t c, LPSIZE lpSize) noexcept;
void Polyline(HDC hdc, const POINT* apt, uint32_t cpt) noexcept;
auto pressed(int virtKey) noexcept -> bool;
auto ReadFile(HANDLE file, LPVOID buffer, uint32_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) noexcept -> bool;
void setCursor(HCURSOR hCursor) noexcept;
void setSideWinVal(int index) noexcept;
void TextOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept;
auto toFloat(double invar) -> float;
auto toFloat(int32_t invar) noexcept -> float;
auto toFloat(LONG invar) noexcept -> float;
auto toFloat(uint32_t invar) noexcept -> float;
auto toSize(uint32_t invar) noexcept -> size_t;
auto toUnsigned(size_t invar) -> uint32_t;
auto wcstof(const wchar_t (&buffer)[HBUFSIZ]) noexcept -> float;
void WriteFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;

// pragma required until MSVC /analyze recognizes noexcept(false)
#pragma warning(push)
#pragma warning(disable : 26440)
template <class outType, class inType> outType ceil(inType invar) {
	return gsl::narrow<outType>(std::ceil(invar));
}

template <class outType, class inType> outType floor(inType invar) {
	return gsl::narrow<outType>(std::floor(invar));
}

template <class outType, class inType> outType round(inType invar) {
	return gsl::narrow<outType>(std::round(invar));
}

template <class outType> outType wcstoi(const wchar_t (&buffer)[HBUFSIZ]) {
	return gsl::narrow<outType>(std::wcstoul(static_cast<const wchar_t*>(buffer), nullptr, 10));
}

#pragma warning(pop)

} // namespace wrap
