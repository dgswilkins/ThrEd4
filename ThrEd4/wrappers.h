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

namespace wrap {
HPEN     CreatePen(int iStyle, unsigned width, COLORREF color) noexcept;
void     GetTextExtentPoint(HDC hdc, LPCTSTR lpString, unsigned int cbString, LPSIZE lpSize) noexcept;
void     GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, unsigned int c, LPSIZE lpSize) noexcept;
void     Polyline(HDC hdc, const POINT* apt, unsigned int cpt) noexcept;
bool     ReadFile(HANDLE file, LPVOID buffer, unsigned int bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) noexcept;
void     setCursor(HCURSOR hCursor) noexcept;
void     TextOut(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, unsigned int cchString) noexcept;
double   toDouble(float invar) noexcept;
double   toDouble(long invar) noexcept;
float    toFloat(double invar);
float    toFloat(long invar) noexcept;
size_t   toSize(uint32_t invar) noexcept;
uint32_t toUnsigned(double invar);
uint32_t toUnsigned(size_t invar);
void WriteFile(HANDLE file, LPCVOID buffer, unsigned int bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;

template <class outType, class inType> outType ceil(inType invar) {
	return gsl::narrow<outType>(std::ceil(invar));
}

template <class outType, class inType> outType floor(inType invar) {
	return gsl::narrow<outType>(std::floor(invar));
}

template <class outType, class inType> outType round(inType invar) {
	return gsl::narrow<outType>(std::round(invar));
}
}