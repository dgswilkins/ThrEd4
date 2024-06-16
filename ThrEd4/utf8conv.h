#pragma once

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
#include <string> // For std::string and std::wstring

namespace utf {

inline auto utf8ToUtf16(std::string const& utf8) -> std::wstring {
  auto const inLength  = gsl::narrow_cast<int>(utf8.size());
  auto const outLength = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), inLength, nullptr, 0);

  std::wstring utf16(gsl::narrow_cast<size_t>(outLength), '\0');
  MultiByteToWideChar(CP_UTF8, 0, utf8.data(), inLength, utf16.data(), outLength);
  return utf16;
}

inline auto utf16ToUtf8(std::wstring const& utf16) -> std::string {
  auto const inLength = gsl::narrow_cast<int>(utf16.size());
  auto const outLength = WideCharToMultiByte(CP_UTF8, 0, utf16.data(), inLength, nullptr, 0, nullptr, nullptr);

  std::string utf8(gsl::narrow_cast<size_t>(outLength), '\0');
  WideCharToMultiByte(CP_UTF8, 0, utf16.data(), inLength, utf8.data(), outLength, nullptr, nullptr);
  return utf8;
}

} // namespace utf
