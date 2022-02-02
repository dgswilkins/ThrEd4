//  ReSharper disable CppDeprecatedEntity CppClangTidyClangDiagnosticDeprecatedDeclarations
////////////////////////////////////////////////////////////////////////////////
//
// This header file defines a couple of C++ functions to convert between
// UTF-8 and UTF-16 Unicode encodings.
// see https://en.cppreference.com/w/cpp/locale/codecvt_utf8_utf16 for reference code
//
// Function implementations are inline, so this module can be reused in
// C++ projects just by #including this header file.
//
// std::string is used to store UTF-8-encoded strings.
// std::wstring is used to store UTF-16-encoded strings.
//
// Note that std::wstring is non-portable, as wchar_t's size is non-portable
// (e.g. 16 bits on Win32/Visual C++, 32 bits on Linux/GCC).
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:

// C RunTime Header Files

// Standard Libraries
#include <string> // For std::string and std::wstring

namespace utf {

inline auto utf8ToUtf16(std::string const& utf8) -> std::wstring {
  auto const inLength  = gsl::narrow_cast<int>(utf8.size());
  auto const outLength = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), inLength, nullptr, 0);

  std::wstring utf16(outLength, '\0');
  MultiByteToWideChar(CP_UTF8, 0, utf8.data(), inLength, utf16.data(), outLength);
  return utf16;
}

inline auto utf16ToUtf8(std::wstring const& utf16) -> std::string {
  auto const inLength  = gsl::narrow_cast<int>(utf16.size());
  auto const outLength = WideCharToMultiByte(CP_UTF8, 0, utf16.data(), inLength, nullptr, 0, nullptr, nullptr);

  std::string utf8(outLength, '\0');
  WideCharToMultiByte(CP_UTF8, 0, utf16.data(), inLength, utf8.data(), outLength, nullptr, nullptr);
  return utf8;
}

} // namespace utf
