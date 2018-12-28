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

long         dToL(double invar);
unsigned int dToUI(double invar);
long         fToL(float invar);

void GetTextExtentPoint32Int(HDC hdc, LPCTSTR lpString, unsigned int c, LPSIZE lpSize);
void GetTextExtentPointInt(HDC hdc, LPCTSTR lpString, unsigned int cbString, LPSIZE lpSize);
void PolylineInt(HDC hdc, const POINT* apt, unsigned int cpt);
bool ReadFileInt(HANDLE file, LPVOID buffer, unsigned int bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped);
void setCursorInt(HCURSOR hCursor) noexcept;
void TextOutInt(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, unsigned int cchString);
void WriteFileInt(HANDLE file, LPCVOID buffer, unsigned int bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped);
HPEN CreatePenInt(int iStyle, unsigned width, COLORREF color) noexcept;
