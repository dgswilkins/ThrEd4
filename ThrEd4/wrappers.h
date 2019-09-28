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
float    bufToFloat(const wchar_t* buffer);
HPEN     CreatePen(int32_t iStyle, uint32_t width, COLORREF color) noexcept;
void     GetTextExtentPoint(HDC hdc, LPCTSTR lpString, uint32_t cbString, LPSIZE lpSize) noexcept;
void     GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t c, LPSIZE lpSize) noexcept;
void     Polyline(HDC hdc, const POINT* apt, uint32_t cpt) noexcept;
bool     pressed(int virtKey) noexcept;
bool     ReadFile(HANDLE file, LPVOID buffer, uint32_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) noexcept;
void     setCursor(HCURSOR hCursor) noexcept;
void     setSideWinVal(int index) noexcept;
void     TextOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept;
double   toDouble(float invar) noexcept;
double   toDouble(int32_t invar) noexcept;
float    toFloat(double invar);
float    toFloat(int32_t invar) noexcept;
float    toFloat(LONG invar) noexcept;
float    toFloat(uint32_t invar) noexcept;
size_t   toSize(uint32_t invar) noexcept;
uint32_t toUnsigned(size_t invar);
float    wcstof(const wchar_t (&buffer)[HBUFSIZ]) noexcept;
void     WriteFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;

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
