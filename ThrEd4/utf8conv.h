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

#include <crtdbg.h>  // For _ASSERTE()
#include <Windows.h> // Win32 Platform SDK main header

#include <string> // For std::string and std::wstring

namespace utf {

inline std::wstring Utf8ToUtf16(const std::string& utf8) {
	std::wstring utf16;

	// handle the special case of empty input string
	if (utf8.empty()) {
		_ASSERTE(utf16.empty());
		return utf16;
	}

	utf16 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(utf8);

	return utf16;
}

inline std::string Utf16ToUtf8(const std::wstring& utf16) {
	std::string utf8;

	if (utf16.empty()) {
		_ASSERTE(utf8.empty());
		return utf8;
	}

	utf8 = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(utf16);

	return utf8;
}

} // namespace utf
