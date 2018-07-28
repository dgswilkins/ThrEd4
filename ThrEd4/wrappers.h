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
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

void PolylineInt(HDC hdc, const POINT* apt, size_t cpt);
void WriteFileInt(HANDLE file, LPCVOID buffer, size_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped);
void GetTextExtentPoint32Int(HDC hdc, LPCTSTR lpString, size_t c, LPSIZE lpSize);
void TextOutInt(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, size_t cchString);
bool ReadFileInt(HANDLE file, LPVOID buffer, size_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped);
void GetTextExtentPointInt(HDC hdc, LPCTSTR lpString, size_t cbString, LPSIZE lpSize);