// Local Headers
#include "stdafx.h"
#include "globals.h"
#include "wrappers.h"

auto wrap::wcsToFloat(wchar_t const* buffer) -> float {
  try {
	return std::wcstof(buffer, nullptr);
  }
  catch (...) {
	// NOLINTNEXTLINE
	outDebugString(L"wcstof failed trying to convert '{}'\n", buffer);
	return 0.0F;
  }
}

void wrap::polyline(HDC hdc, POINT const* apt, uint32_t cpt) noexcept {
  Polyline(hdc, apt, gsl::narrow<int32_t>(cpt));
}

auto wrap::pressed(int virtKey) noexcept -> bool {
  return (gsl::narrow_cast<uint16_t>(GetKeyState(virtKey)) & 0x8000U) != 0U;
}

void wrap::writeFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept {
  WriteFile(file, buffer, gsl::narrow<DWORD>(bytesToWrite), bytesWritten, overlapped);
}

void wrap::getTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t iLen, LPSIZE lpSize) noexcept {
  GetTextExtentPoint32(hdc, lpString, gsl::narrow<int32_t>(iLen), lpSize);
}

void wrap::textOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept {
  TextOut(hdc, nXStart, nYStart, lpString, gsl::narrow<int32_t>(cchString));
}

void wrap::setCursor(HCURSOR hCursor) noexcept {
  static HCURSOR saveCursor = nullptr;
  if (saveCursor == hCursor) {
	return;
  }
  SetCursor(hCursor);
  saveCursor = hCursor;
}

auto wrap::createPen(int32_t iStyle, int32_t width, COLORREF color) noexcept -> HPEN {
  auto const scaledWidth = MulDiv(width, *ScreenDPI, STDDPI);
  return ::CreatePen(iStyle, scaledWidth, color);
}

auto wrap::getFormVertices() noexcept -> std::vector<F_POINT>* {
  return FormVertices;
}
