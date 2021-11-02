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
#include <crtdbg.h> // For _ASSERTE()

// Standard Libraries
#include <string> // For std::string and std::wstring
#include <codecvt>

namespace utf {

inline auto Utf8ToUtf16(std::string const& utf8) -> std::wstring {
  std::wstring utf16;

  // handle the special case of empty input string
  if (utf8.empty()) {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-vararg)
	_ASSERTE(utf16.empty());
	return utf16;
  }
#pragma warning(suppress : 4996) // wstring_convert was declared deprecated in c++17
  utf16 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> {}.from_bytes(utf8);

  return utf16;
}

inline auto Utf16ToUtf8(std::wstring const& utf16) -> std::string {
  std::string utf8;

  if (utf16.empty()) {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-vararg)
	_ASSERTE(utf8.empty());
	return utf8;
  }

#pragma warning(suppress : 4996) // wstring_convert was declared deprecated in c++17
  utf8 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> {}.to_bytes(utf16);

  return utf8;
}

} // namespace utf
