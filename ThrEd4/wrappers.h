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

template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const& vec) -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T);
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T));
  }
}

template <class T>
constexpr auto sizeofType([[maybe_unused]] std::vector<T> const* vec) -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T);
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T));
  }
}

template <class T> auto sizeofVector(std::vector<T> const& vec) -> uint32_t {
  if constexpr (std::is_same_v<uint32_t, size_t>) {
	return sizeof(T) * vec.size();
  }
  else {
	return gsl::narrow<uint32_t>(sizeof(T) * vec.size());
  }
}

template <class T> auto sizeofVector(std::vector<T> const* vec) -> uint32_t {
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

template <class outType, class inType> auto ceil(inType invar) -> outType {
  static_assert(!std::is_same_v<outType, float>, "no need to use wrap::ceil here.");
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

template <class outType, class inType> auto floor(inType invar) -> outType {
  //static_assert(!std::is_same_v<outType, float>, "no need to use wrap::floor here.");
  //static_assert(std::is_same_v<inType, float>, "cannot use wrap::floor here.");
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

template <class inType> auto midl(inType high, inType low) noexcept -> float {
  //static_assert(!std::is_same_v<inType, float>, "no need to use wrap::midl here.");
  if constexpr (std::is_same_v<inType, float>) {
	return (high - low) / 2.0F + low;
  }
  else {
	return (gsl::narrow<float>(high) - gsl::narrow<float>(low)) / 2.0F +
	       gsl::narrow<float>(low);
  }
}

template <class outType, class inType> auto distance(inType start, inType end) -> outType {
  //static_assert(!std::is_same_v<outType, ptrdiff_t>, "no need to use wrap::distance here.");
  if constexpr (std::is_same_v<outType, ptrdiff_t>) {
	return std::distance(start, end);
  }
  else {
	return gsl::narrow<outType>(std::distance(start, end));
  }
}

template <class itType, class inType> auto next(itType iterator, inType index) -> itType {
  //static_assert(!std::is_same_v<inType, ptrdiff_t>, "no need to use wrap::next here.");
  if constexpr (std::is_same_v<inType, ptrdiff_t>) {
	return std::next(iterator, index);
  }
  else {
	return std::next(iterator, gsl::narrow<ptrdiff_t>(index));
  }
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
  //static_assert(!std::is_same_v<inType, outType>, "no need to use wrap::narrow_cast here.");
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

template <class outType, class inType> auto round(inType invar) -> outType {
  //static_assert(!std::is_same_v<outType, float>, "no need to use wrap::round here.");
  //static_assert(std::is_same_v<inType, float>, "cannot use wrap::round here.");
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
	  return std::roundf(gsl::narrow<float>(invar));
	}
	else {
	  return gsl::narrow<outType>(std::roundf(gsl::narrow<float>(invar)));
	}
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

template <class inType>
auto toFloat(inType invar) noexcept(!(std::is_same_v<inType, float> ||
                                      (std::is_same_v<inType, size_t> && !std::is_same_v<uint32_t, size_t>)))
    -> float {
  if constexpr (std::is_same_v<inType, float>) {
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

template <class inType>
auto toSize(inType invar) noexcept(!(std::is_signed_v<inType> || std::is_same_v<inType, unsigned long long>))
    -> size_t {
  if constexpr (std::is_signed_v<inType>) {
	return gsl::narrow<size_t>(invar);
  }
  else {
	if constexpr (std::is_same_v<inType, size_t>) {
	  return invar;
	}
	else {
	  if constexpr (std::is_same_v<size_t, unsigned long long>) {
		return gsl::narrow_cast<size_t>(invar);
	  }
	  else {
		if constexpr (std::is_same_v<inType, unsigned long long>) {
		  return gsl::narrow<size_t>(invar);
		}
		else {
		  return gsl::narrow_cast<size_t>(invar);
		}
	  }
	}
  }
}

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
