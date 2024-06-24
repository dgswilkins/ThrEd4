#pragma once

// Local Headers
#include "point.h"
#include "reporting.h"
#include "ThrEdTypes.h"

// Open Source headers
#include "gsl/gsl"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#ifndef _DEBUG
// ReSharper disable CppUnusedIncludeDirective
#include <cmath>
// ReSharper restore CppUnusedIncludeDirective
#endif
#include <vector>

constexpr auto MINDBL = 4e-5; // small number for conversions

namespace wrap {

template <class inType>
auto toFloat(inType invar) noexcept(!(std::is_same_v<inType, double> ||
                                      (std::is_same_v<inType, size_t> && !std::is_same_v<uint32_t, size_t>)))
    -> float {
  static_assert((std::is_same_v<inType, double> || std::is_same_v<inType, char> || std::is_same_v<inType, int16_t> ||
                 std::is_same_v<inType, int32_t> || std::is_same_v<inType, LONG> ||
                 std::is_same_v<inType, uint8_t> || std::is_same_v<inType, uint16_t> ||
                 std::is_same_v<inType, uint32_t> || std::is_same_v<inType, uint64_t>),
                "cannot use wrap::toFloat here.");
  if constexpr (std::is_same_v<inType, double>) {
	return util::doubleToFloat(invar);
  }
  else {
	if constexpr (std::is_same_v<inType, size_t> && !std::is_same_v<uint32_t, size_t>) {
	  return gsl::narrow<float>(invar);
	}
	else {
	  return gsl::narrow_cast<float>(invar);
	}
  }
}

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

template <class T>
#pragma warning(suppress : 4100) // NOLINTNEXTLINE(bugprone-exception-escape, misc-unused-parameters)
constexpr auto sizeofType(std::vector<T> const& vec) noexcept(std::is_same_v<uint32_t, size_t>) -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T);
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T));
  }
}

template <class T>
#pragma warning(suppress : 4100) // NOLINTNEXTLINE(bugprone-exception-escape, misc-unused-parameters)
constexpr auto sizeofType(std::vector<T> const* vec) noexcept(std::is_same_v<uint32_t, size_t>) -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T);
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T));
  }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

template <class T>
auto sizeofVector(std::vector<T> const& vec) noexcept(std::is_same_v<uint32_t, size_t>) -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T) * vec.size();
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T) * vec.size());
  }
}

template <class T>
auto sizeofVector(std::vector<T> const* vec) noexcept(std::is_same_v<uint32_t, size_t>) -> uint32_t {
  if (nullptr != vec) {
	if constexpr (std::is_same_v<uint32_t, size_t>) {
	  return sizeof(T) * vec->size();
	}
	else {
	  return gsl::narrow<uint32_t>(sizeof(T) * vec->size());
	}
  }
  return 0;
}

template <class outType, class inType>
auto ceil(inType invar) noexcept((std::is_same_v<inType, float>)&&(std::is_same_v<outType, float>))
    -> outType {
  if constexpr (std::is_same_v<inType, float>) {
	if constexpr (std::is_same_v<outType, float>) {
	  return std::ceilf(invar);
	}
	else {
	  return gsl::narrow<outType>(std::ceilf(invar));
	}
  }
  else {
	if constexpr (std::is_same_v<outType, float>) {
	  return std::ceilf(wrap::toFloat(invar));
	}
	else {
	  return gsl::narrow<outType>(std::ceilf(wrap::toFloat(invar)));
	}
  }
}

auto createPen(int32_t iStyle, int32_t width, COLORREF color) noexcept -> HPEN;

template <class outType, class inType>
auto floor(inType invar) noexcept((std::is_same_v<inType, float>)&&(std::is_same_v<outType, float>))
    -> outType {
  if constexpr (std::is_same_v<inType, float>) {
	if constexpr (std::is_same_v<outType, float>) {
	  return std::floorf(invar);
	}
	else {
	  return gsl::narrow<outType>(std::floorf(invar));
	}
  }
  else {
	if constexpr (std::is_same_v<outType, float>) {
	  return std::floorf(wrap::toFloat(invar));
	}
	else {
	  return gsl::narrow<outType>(std::floorf(wrap::toFloat(invar)));
	}
  }
}

void getTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t iLen, LPSIZE lpSize) noexcept;

auto getFormVertices() noexcept -> std::vector<F_POINT>*;

template <class inType>
auto midl(inType high, inType low) noexcept(std::is_same_v<inType, float>) -> float {
  if constexpr (std::is_same_v<inType, float>) {
	return (high - low) * HALF + low;
  }
  else {
	return (wrap::toFloat(high) - wrap::toFloat(low)) * HALF + wrap::toFloat(low);
  }
}

template <class outType, class inType>
auto distance(inType start, inType end) noexcept(std::is_same_v<outType, ptrdiff_t>) -> outType {
  if constexpr (std::is_same_v<outType, ptrdiff_t>) {
	return std::distance(start, end);
  }
  else {
	return gsl::narrow<outType>(std::distance(start, end));
  }
}

template <class itType, class inType>
auto next(itType iterator,
          inType index) noexcept(!(std::is_same_v<inType, size_t> ||
                                   (std::is_same_v<ptrdiff_t, int> && std::is_same_v<inType, uint32_t>)))
    -> itType {
  static_assert((std::is_same_v<inType, size_t> || std::is_same_v<inType, uint32_t> ||
                 std::is_same_v<inType, uint16_t> || std::is_same_v<inType, uint8_t> ||
                 std::is_same_v<inType, int> || std::is_same_v<inType, char>),
                "next cannot be used here.");
  if constexpr (std::is_same_v<inType, ptrdiff_t>) {
	return std::next(iterator, index);
  }
  else {
	if constexpr (std::is_same_v<inType, size_t>) {
	  return std::next(iterator, gsl::narrow<ptrdiff_t>(index));
	}
	else {
	  if constexpr (std::is_same_v<ptrdiff_t, int> && std::is_same_v<inType, uint32_t>) {
		return std::next(iterator, gsl::narrow<ptrdiff_t>(index));
	  }
	  else {
		return std::next(iterator, gsl::narrow_cast<ptrdiff_t>(index));
	  }
	}
  }
}

template <class outType, class inType>
void narrow(outType& outvar, inType invar) noexcept(std::is_same_v<inType, outType>) {
  if constexpr (std::is_same_v<inType, outType>) {
	outvar = invar;
  }
  else {
	outvar = gsl::narrow<outType>(invar);
  }
}

template <class outType, class inType>
void narrow_cast(outType& outvar, inType invar) noexcept { // NOLINT(readability-identifier-naming)
  if constexpr (std::is_same_v<inType, outType>) {
	outvar = invar;
  }
  else {
	outvar = gsl::narrow_cast<outType>(invar);
  }
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

template <class outType, class inType>
auto round(inType invar) noexcept((std::is_same_v<inType, float>)&&(std::is_same_v<outType, float>))
    -> outType {
  if constexpr (std::is_same_v<inType, float>) {
	if constexpr (std::is_same_v<outType, float>) {
	  return std::roundf(invar);
	}
	else {
	  return gsl::narrow<outType>(std::roundf(invar));
	}
  }
  else {
	if constexpr (std::is_same_v<outType, float>) {
	  return std::roundf(wrap::toFloat(invar));
	}
	else {
	  return gsl::narrow<outType>(std::roundf(wrap::toFloat(invar)));
	}
  }
}

void setCursor(HCURSOR hCursor) noexcept;
void textOut(HDC hdc, int32_t nXStart, int32_t nYStart, LPCTSTR lpString, uint32_t cchString) noexcept;

template <class inType>
auto toPtrdiff(inType invar) noexcept(!std::is_same_v<ptrdiff_t, int> || !std::is_same_v<inType, uint32_t>)
    -> ptrdiff_t {
  static_assert(std::is_same_v<inType, ptrdiff_t> || std::is_same_v<inType, uint32_t> ||
                    std::is_same_v<inType, int> || std::is_same_v<inType, size_t>,
                "toPtrdiff cannot be used here.");
  if constexpr (std::is_same_v<inType, ptrdiff_t>) {
	return invar;
  }
  else {
	if constexpr (std::is_same_v<ptrdiff_t, int>) {
	  if constexpr (std::is_same_v<inType, uint32_t>) {
		return gsl::narrow<ptrdiff_t>(invar); // uint32_t -> ptrdiff (32 bit)
	  }
	  else {
		return invar; // int -> ptrdiff
	  }
	}
	else {
	  return gsl::narrow_cast<ptrdiff_t>(invar); // any -> ptrdiff (64 bit)
	}
  }
}

template <class inType> auto toDouble(inType invar) noexcept -> double {
  return gsl::narrow_cast<double>(invar);
}

template <class inType>
auto toSize(inType invar) noexcept(!(std::is_signed_v<inType> || (std::is_same_v<inType, uint64_t> &&
                                                                  std::is_same_v<size_t, uint32_t>)))
    -> size_t {
  static_assert((std::is_signed_v<inType> || std::is_same_v<inType, uint64_t> ||
                 std::is_same_v<inType, uint8_t> || std::is_same_v<inType, uint16_t> ||
                 std::is_same_v<inType, uint32_t> || std::is_same_v<inType, DWORD>),
                "cannot use wrap::toSize here.");
  if constexpr (std::is_signed_v<inType>) {
	return gsl::narrow<size_t>(invar);
  }
  else {
	if constexpr (std::is_same_v<inType, size_t>) {
	  return invar;
	}
	else {
	  if constexpr (std::is_same_v<inType, uint64_t> && std::is_same_v<size_t, uint32_t>) {
		return gsl::narrow<size_t>(invar);
	  }
	  else {
		return gsl::narrow_cast<size_t>(invar);
	  }
	}
  }
}

template <class inType>
auto toUnsigned(inType invar) noexcept(std::is_same_v<inType, uint32_t> || std::is_same_v<inType, uint16_t>)
    -> uint32_t {
  static_assert((std::is_signed_v<inType> || std::is_same_v<inType, ptrdiff_t> || std::is_same_v<inType, uint16_t> ||
                 std::is_same_v<inType, uint32_t> || std::is_same_v<inType, size_t>),
                "cannot use wrap::toUnsigned here.");
  if constexpr (std::is_same_v<inType, uint16_t>) {
	return gsl::narrow_cast<uint32_t>(invar);
  }
  else {
	if constexpr (std::is_same_v<inType, uint32_t>) {
	  return invar;
	}
	else {
	  return gsl::narrow<uint32_t>(invar);
	}
  }
}

auto wcsToFloat(wchar_t const* buffer) -> float;

template <class outType>
void wcsToULong(outType& outvar, wchar_t const* invar) noexcept(std::is_same_v<outType, unsigned long>) { // NOLINT(google-runtime-int)
  // static_assert(!std::is_same_v<outType, unsigned long>, "no need to use wrap::wcstoULong here.");
  if constexpr (std::is_same_v<outType, unsigned long>) { // NOLINT(google-runtime-int)
	outvar = std::wcstoul(invar, nullptr, DECRAD);
  }
  else {
	outvar = gsl::narrow<outType>(std::wcstoul(invar, nullptr, DECRAD));
  }
}

template <class outType>
auto wcsToLong(wchar_t const* buffer) noexcept(std::is_same_v<outType, long>) -> outType { // NOLINT(google-runtime-int)
  // static_assert(!std::is_same_v<outType, long>, "no need to use wrap::wcstoLong here.");
  if constexpr (std::is_same_v<outType, long>) { // NOLINT(google-runtime-int)
	return std::wcstol(buffer, nullptr, DECRAD);
  }
  else {
	return gsl::narrow<outType>(std::wcstol(buffer, nullptr, DECRAD));
  }
}

void writeFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;
} // namespace wrap
