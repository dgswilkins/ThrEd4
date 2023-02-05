#pragma once

// Local Headers
#include "point.h"
#include "reporting.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/gsl"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <vector>
#include <cmath>

namespace wrap {

#ifdef _WIN64
template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const& vec) -> uint32_t {
  return gsl::narrow<uint32_t>(sizeof(T));
}

template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const* vec) -> uint32_t {
  return gsl::narrow<uint32_t>(sizeof(T));
}

template <class T> auto sizeofVector(std::vector<T> const& vec) -> uint32_t {
  return gsl::narrow<uint32_t>(sizeof(T) * vec.size());
}

template <class T> auto sizeofVector(std::vector<T> const* vec) -> uint32_t {
  if (nullptr != vec) {
	return gsl::narrow<uint32_t>(sizeof(T) * vec->size());
  }
  return 0;
}
#else
template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const& vec) -> uint32_t {
  return sizeof(T);
}

#pragma warning(suppress : 4100)
template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const* vec) -> uint32_t {
  return sizeof(T);
}

template <class T> auto sizeofVector(std::vector<T> const& vec) noexcept -> uint32_t {
  return sizeof(T) * vec.size();
}

template <class T> auto sizeofVector(std::vector<T> const* vec) noexcept -> uint32_t {
  if (nullptr != vec) {
	return sizeof(T) * vec->size();
  }
  return 0;
}
#endif // _win64

template <class outType, class inType> auto ceil(inType invar) -> outType {
  if constexpr (std::is_same_v<inType, float>) {
	return gsl::narrow<outType>(std::ceilf(invar));
  }
  else {
	return gsl::narrow<outType>(std::ceil(invar));
  }
}

auto createPen(int32_t iStyle, int32_t width, COLORREF color) noexcept -> HPEN;

template <class outType, class inType> auto floor(inType invar) -> outType {
  return gsl::narrow<outType>(std::floor(invar));
}

void getTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t iLen, LPSIZE lpSize) noexcept;

auto getFormVertices() noexcept -> std::vector<F_POINT>*;

template <class inType> auto midl(inType high, inType low) noexcept -> float {
  return (gsl::narrow_cast<float>(high) - gsl::narrow_cast<float>(low)) / 2.0F + gsl::narrow_cast<float>(low);
}

template <class outType, class inIt> auto distance(inIt start, inIt end) -> outType {
  return gsl::narrow<outType>(std::distance(start, end));
}

template <class inIt, class inIn> auto next(inIt iterator, inIn index) -> inIt {
  return std::next(iterator, gsl::narrow<ptrdiff_t>(index));
}

template <class outType, class inType> void narrow(outType& outvar, inType invar) {
  //static_assert(!std::is_same_v<inType, outType>, "no need to use wrap::narrow here.");
  if constexpr (std::is_same_v<inType, outType>) {
	outvar = invar;
  }
  else {
  outvar = gsl::narrow<outType>(invar);
}
}

template <class outType, class inType>
void narrow_cast(outType& outvar, inType invar) noexcept { // NOLINT(readability-identifier-naming)
  outvar = gsl::narrow_cast<outType>(invar);
}

void polyline(HDC hdc, POINT const* apt, uint32_t cpt) noexcept;
auto pressed(int virtKey) noexcept -> bool;

template <class bufType, class inType>
auto readFile(HANDLE fileHandle, bufType* buffer, inType bytesToRead, LPDWORD bytesRead, const wchar_t* prompt)
    -> bool {
  if constexpr (std::is_same_v<inType, DWORD>) {
	if (0 == ReadFile(fileHandle, gsl::narrow<LPVOID>(buffer), bytesToRead, bytesRead, nullptr)) {
	  auto errorCode = GetLastError();
	  CloseHandle(fileHandle);
	  rpt::reportError(prompt, errorCode);
	  return false;
	}
	return true;
  }
  else {
	if (0 == ReadFile(fileHandle, gsl::narrow<LPVOID>(buffer), gsl::narrow<DWORD>(bytesToRead), bytesRead, nullptr)) {
	  auto errorCode = GetLastError();
	  CloseHandle(fileHandle);
	  rpt::reportError(prompt, errorCode);
	  return false;
	}
	return true;
  }
}

template <class outType, class inType> auto round(inType invar) -> outType {
  if constexpr (std::is_same_v<inType, float>) {
	return gsl::narrow<outType>(std::lroundf(invar));
  }
  else {
	return gsl::narrow<outType>(std::lround(invar));
  }
}

#ifdef _WIN64
template <class inType> auto ptrdiff(inType invar) noexcept -> ptrdiff_t {
  return gsl::narrow_cast<ptrdiff_t>(invar);
}
#else
template <class inType> auto ptrdiff(inType invar) -> ptrdiff_t {
  return gsl::narrow<ptrdiff_t>(invar);
}
#endif

void setCursor(HCURSOR hCursor) noexcept;
void textOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept;

template <class inType> auto toDouble(inType invar) noexcept -> double {
  return gsl::narrow_cast<double>(invar);
}

auto toFloat(double invar) -> float;
auto toFloat(int32_t invar) noexcept -> float;
auto toFloat(LONG invar) noexcept -> float;
// NOLINTNEXTLINE(google-runtime-int)
auto toFloat(unsigned long invar) noexcept -> float;
auto toFloat(uint32_t invar) noexcept -> float;
#ifdef _WIN64
auto toFloat(size_t invar) noexcept -> float;
#endif
auto toFloat(int16_t invar) noexcept -> float;
auto toSize(int32_t invar) -> size_t;
// NOLINTNEXTLINE(google-runtime-int)
auto toSize(long invar) -> size_t;
#ifdef _WIN64
auto toSize(ptrdiff_t invar) noexcept -> size_t;
#endif
auto toSize(uint16_t invar) noexcept -> size_t;
auto toSize(uint32_t invar) noexcept -> size_t;
#ifdef _WIN64
auto toSize(DWORD invar) noexcept -> size_t;
#else
auto toSize(DWORD invar) -> size_t;
#endif
#ifdef _WIN64
auto toSize(uintmax_t invar) noexcept -> size_t;
#else
auto toSize(uintmax_t invar) -> size_t;
#endif
auto toUnsigned(float invar) -> uint32_t;
auto toUnsigned(size_t invar) -> uint32_t;
auto toUnsigned(int invar) -> uint32_t;
// NOLINTNEXTLINE(google-runtime-int)
auto toUnsigned(long invar) -> uint32_t;
#ifdef _WIN64
auto toUnsigned(ptrdiff_t invar) -> uint32_t;
#endif
auto wcsToFloat(wchar_t const* buffer) -> float;

template <class outType> void wcsToULong(outType& outvar, wchar_t const* invar) {
  outvar = gsl::narrow<outType>(std::wcstoul(invar, nullptr, 10));
}

template <class outType> auto wcsToLong(wchar_t const* buffer) -> outType {
  return gsl::narrow<outType>(std::wcstol(buffer, nullptr, 10));
}

void writeFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;
} // namespace wrap
