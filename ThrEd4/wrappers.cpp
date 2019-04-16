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

void wrap::Polyline(HDC hdc, CONST POINT* apt, uint32_t cpt) noexcept {
	Polyline(hdc, apt, gsl::narrow<int32_t>(cpt));
}

bool wrap::pressed(int virtKey) noexcept {
	return (gsl::narrow_cast<uint16_t>(GetKeyState(virtKey)) & 0x8000u) != 0u;
}

void wrap::WriteFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept {
	WriteFile(file, buffer, gsl::narrow<DWORD>(bytesToWrite), bytesWritten, overlapped);
}

bool wrap::ReadFile(HANDLE file, LPVOID buffer, uint32_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped) noexcept {
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

float wrap::toFloat(double invar) {
	auto result = 0.0f;
	try {
		result = gsl::narrow<float>(invar);
	}
	catch (const gsl::narrowing_error& e) { // check if we are seeing a rounding error
		UNREFERENCED_PARAMETER(e);

		const auto var  = gsl::narrow_cast<float>(invar);
		const auto diff = abs(invar - var);
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

float wrap::toFloat(int32_t invar) noexcept {
	return gsl::narrow_cast<float>(invar);
}

// pragma required until MSVC /analyze recognizes noexcept(false)
#pragma warning(push)
#pragma warning(disable : 26440)
uint32_t wrap::toUnsigned(size_t invar) {
	return gsl::narrow<uint32_t>(invar);
}
#pragma warning(pop)

double wrap::toDouble(float invar) noexcept {
	return gsl::narrow_cast<double>(invar);
}

double wrap::toDouble(int32_t invar) noexcept {
	return gsl::narrow_cast<double>(invar);
}

size_t wrap::toSize(uint32_t invar) noexcept {
	return gsl::narrow_cast<size_t>(invar);
}

float wrap::wcstof(const wchar_t (&buffer)[HBUFSIZ]) noexcept {
	return std::wcstof(static_cast<const wchar_t*>(buffer), nullptr);
}

void wrap::setCursor(HCURSOR hCursor) noexcept {
	static HCURSOR saveCursor = nullptr;
	if (saveCursor != hCursor) {
		SetCursor(hCursor);
		saveCursor = hCursor;
	}
}

HPEN wrap::CreatePen(int32_t iStyle, uint32_t width, COLORREF color) noexcept {
	const auto scaledWidth = MulDiv(width, *screenDPI, 96);
	return ::CreatePen(iStyle, scaledWidth, color);
}
