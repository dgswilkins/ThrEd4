#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Open Source headers
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "wrappers.h"

void PolylineInt(HDC hdc, CONST POINT* apt, size_t cpt) {
	Polyline(hdc, apt, gsl::narrow<int>(cpt));
}

void WriteFileInt(HANDLE file, LPCVOID buffer, size_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) {
	WriteFile(file, buffer, gsl::narrow<DWORD>(bytesToWrite), bytesWritten, overlapped);
}

bool ReadFileInt(HANDLE file, LPVOID buffer, size_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) {
	return ReadFile(file, buffer, gsl::narrow<DWORD>(bytesToRead), bytesRead, overlapped);
}

void GetTextExtentPoint32Int(HDC hdc, LPCTSTR lpString, size_t c, LPSIZE lpSize) {
	GetTextExtentPoint32(hdc, lpString, gsl::narrow<int>(c), lpSize);
}

void TextOutInt(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, size_t cchString) {
	TextOut(hdc, nXStart, nYStart, lpString, gsl::narrow<int>(cchString));
}

void GetTextExtentPointInt(HDC hdc, LPCTSTR lpString, size_t cbString, LPSIZE lpSize) {
	GetTextExtentPoint(hdc, lpString, gsl::narrow<int>(cbString), lpSize);
}
