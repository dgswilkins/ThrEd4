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
  else {
	if constexpr (std::is_same_v<inType, size_t> && !std::is_same_v<uint32_t, size_t>) {
	  return gsl::narrow<float>(invar);
	}
	else {
	  return gsl::narrow_cast<float>(invar);
	}
  }
}

template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const& vec) noexcept(std::is_same_v<uint32_t, size_t>)
    -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T);
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T));
  }
}

template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const* vec) noexcept(std::is_same_v<uint32_t, size_t>)
    -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T);
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T));
  }
}

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
  // static_assert(!std::is_same_v<outType, float>, "no need to use wrap::ceil here.");
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
	  return std::ceilf(gsl::narrow<float>(invar));
	}
	else {
	  return gsl::narrow<outType>(std::ceilf(gsl::narrow<float>(invar)));
	}
  }
}

auto createPen(int32_t iStyle, int32_t width, COLORREF color) noexcept -> HPEN;

template <class outType, class inType>
auto floor(inType invar) noexcept((std::is_same_v<inType, float>)&&(std::is_same_v<outType, float>))
    -> outType {
  // static_assert(!std::is_same_v<outType, float>, "no need to use wrap::floor here.");
  // static_assert(std::is_same_v<inType, float>, "cannot use wrap::floor here.");
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
	  return std::floorf(gsl::narrow<float>(invar));
	}
	else {
	  return gsl::narrow<outType>(std::floorf(gsl::narrow<float>(invar)));
	}
  }
}

void getTextExtentPoint32(HDC hdc, LPCTSTR lpString, uint32_t iLen, LPSIZE lpSize) noexcept;

auto getFormVertices() noexcept -> std::vector<F_POINT>*;

template <class inType>
auto midl(inType high, inType low) noexcept(std::is_same_v<inType, float>) -> float {
  // static_assert(!std::is_same_v<inType, float>, "no need to use wrap::midl here.");
  if constexpr (std::is_same_v<inType, float>) {
	return (high - low) / 2.0F + low;
  }
  else {
	return (gsl::narrow<float>(high) - gsl::narrow<float>(low)) / 2.0F + gsl::narrow<float>(low);
  }
}

template <class outType, class inType>
auto distance(inType start, inType end) noexcept(std::is_same_v<outType, ptrdiff_t>) -> outType {
  // static_assert(!std::is_same_v<outType, ptrdiff_t>, "no need to use wrap::distance here.");
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
  // static_assert(!std::is_same_v<inType, ptrdiff_t>, "no need to use wrap::next here.");
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
  // static_assert(!std::is_same_v<inType, outType>, "no need to use wrap::narrow here.");
  if constexpr (std::is_same_v<inType, outType>) {
	outvar = invar;
  }
  else {
	outvar = gsl::narrow<outType>(invar);
  }
}

template <class outType, class inType>
void narrow_cast(outType& outvar, inType invar) noexcept { // NOLINT(readability-identifier-naming)
  // static_assert(!std::is_same_v<inType, outType>, "no need to use wrap::narrow_cast here.");
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
  // static_assert(!std::is_same_v<outType, float>, "no need to use wrap::round here.");
  // static_assert(std::is_same_v<inType, float>, "cannot use wrap::round here.");
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
auto to_ptrdiff(inType invar) noexcept(!std::is_same_v<ptrdiff_t, int> || !std::is_same_v<inType, uint32_t>)
    -> ptrdiff_t {
  static_assert(std::is_same_v<inType, ptrdiff_t> || std::is_same_v<inType, uint32_t> ||
                    std::is_same_v<inType, int>,
                "to_ptrdiff cannot be used here.");
  if constexpr (std::is_same_v<inType, ptrdiff_t>) {
	return invar;
  }
  else {
	if constexpr (std::is_same_v<ptrdiff_t, int>) {
	  if constexpr (std::is_same_v<inType, uint32_t>) {
		return gsl::narrow<ptrdiff_t>(invar);
	  }
	}
	else {
	  return gsl::narrow_cast<ptrdiff_t>(invar);
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
void wcsToULong(outType& outvar, wchar_t const* invar) noexcept(std::is_same_v<outType, unsigned long>) {
  // static_assert(!std::is_same_v<outType, unsigned long>, "no need to use wrap::wcstoULong here.");
  if constexpr (std::is_same_v<outType, unsigned long>) {
	outvar = std::wcstoul(invar, nullptr, 10);
  }
  else {
	outvar = gsl::narrow<outType>(std::wcstoul(invar, nullptr, 10));
  }
}

template <class outType>
auto wcsToLong(wchar_t const* buffer) noexcept(std::is_same_v<outType, long>) -> outType {
  // static_assert(!std::is_same_v<outType, long>, "no need to use wrap::wcstoLong here.");
  if constexpr (std::is_same_v<outType, long>) {
	return std::wcstol(buffer, nullptr, 10);
  }
  else {
	return gsl::narrow<outType>(std::wcstol(buffer, nullptr, 10));
  }
}

void writeFile(HANDLE file, LPCVOID buffer, uint32_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped) noexcept;
} // namespace wrap
