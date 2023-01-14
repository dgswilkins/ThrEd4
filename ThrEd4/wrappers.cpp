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

auto wrap::toFloat(double invar) -> float {
  try {
	return gsl::narrow<float>(invar);
  }
  catch (gsl::narrowing_error const& e) { // check if we are seeing a rounding error
	UNREFERENCED_PARAMETER(e);
	auto const var  = gsl::narrow_cast<float>(invar);
	auto const diff = abs(invar - gsl::narrow_cast<double>(var));
	if (diff > 4e-5) {
	  throw std::runtime_error("conversion error above limit");
	}
	return var;
  }
  catch (...) { // otherwise throw
	throw std::runtime_error("gsl::narrow failed in wrap:toFloat");
  }
}

auto wrap::toFloat(int16_t invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

auto wrap::toFloat(int32_t invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

auto wrap::toFloat(LONG invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

auto wrap::toFloat(uint32_t invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

// this is used in 64bit mode only. In 32 bit, size_t is an int
#ifdef _WIN64
auto wrap::toFloat(size_t invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}
#endif

auto wrap::toUnsigned(int invar) -> uint32_t {
  return gsl::narrow<uint32_t>(invar);
}

// NOLINTNEXTLINE(google-runtime-int)
auto wrap::toFloat(unsigned long invar) noexcept -> float {
  return gsl::narrow_cast<float>(invar);
}

auto wrap::toUnsigned(float invar) -> uint32_t {
  return gsl::narrow<uint32_t>(invar);
}

// NOLINTNEXTLINE(google-runtime-int)
auto wrap::toUnsigned(long invar) -> uint32_t {
  return gsl::narrow<uint32_t>(invar);
}

// this is used in 64bit mode only. In 32 bit, ptrdiff_t is an int
#ifdef _WIN64
auto wrap::toUnsigned(ptrdiff_t invar) -> uint32_t {
  return gsl::narrow<uint32_t>(invar);
}
#endif

// since this is used in other translation units, "constexpr"
// cannot be used as it implicitly defines the function as inline.
#pragma warning(suppress : 26497 26440)
auto wrap::toUnsigned(size_t invar) -> uint32_t {
#ifdef _WIN64
  return gsl::narrow<uint32_t>(invar);
#else
  return invar;
#endif
}

auto wrap::toSize(int32_t invar) -> size_t {
  return gsl::narrow<size_t>(invar);
}

// NOLINTNEXTLINE(google-runtime-int)
auto wrap::toSize(long invar) -> size_t {
  return gsl::narrow<size_t>(invar);
}

// this is used in 64bit mode only. In 32 bit, ptrdiff_t is an int
#ifdef _WIN64
auto wrap::toSize(ptrdiff_t invar) noexcept -> size_t {
  return gsl::narrow_cast<size_t>(invar);
}
#endif

auto wrap::toSize(uint16_t invar) noexcept -> size_t {
  return gsl::narrow_cast<size_t>(invar);
}

// since this is used in other translation units, "constexpr"
// cannot be used as it implicitly defines the function as inline.
#pragma warning(suppress : 26497 26440)
auto wrap::toSize(uint32_t invar) noexcept -> size_t {
#ifdef _WIN64
  return gsl::narrow_cast<size_t>(invar);
#else
  return invar;
#endif
}

#ifdef _WIN64
auto wrap::toSize(DWORD invar) noexcept -> size_t {
  return gsl::narrow_cast<size_t>(invar);
}
#else
auto wrap::toSize(DWORD invar) -> size_t {
  return gsl::narrow<size_t>(invar);
}
#endif

// since this is used in other translation units, "constexpr"
// cannot be used as it implicitly defines the function as inline.
#pragma warning(suppress : 26497 26440)
#ifdef _WIN64
auto wrap::toSize(uintmax_t invar) noexcept -> size_t {
  return invar;
}
#else
auto wrap::toSize(uintmax_t invar) -> size_t {
  return gsl::narrow<size_t>(invar);
}
#endif

void wrap::setCursor(HCURSOR hCursor) noexcept {
  static HCURSOR saveCursor = nullptr;
  if (saveCursor != hCursor) {
	SetCursor(hCursor);
	saveCursor = hCursor;
  }
}

auto wrap::createPen(int32_t iStyle, int32_t width, COLORREF color) noexcept -> HPEN {
  auto const scaledWidth = MulDiv(width, *ScreenDPI, STDDPI);
  return ::CreatePen(iStyle, scaledWidth, color);
}

auto wrap::getFormVertices() noexcept -> std::vector<F_POINT>* {
  return FormVertices;
}
