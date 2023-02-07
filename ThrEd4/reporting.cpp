// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#ifndef _DEBUG
#include "displayText.h"
#endif
#include "reporting.h"
#include "ThrEdTypes.h"
#include "wrappers.h"

// report the system error from GetLastError
void rpt::reportError(const wchar_t* prompt, DWORD& errorCode) {
  auto* lpMsgBuf = gsl::narrow_cast<LPVOID>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto const res = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 nullptr,
                                 errorCode,
                                 0,
                                 reinterpret_cast<LPTSTR>(&lpMsgBuf),
                                 0,
                                 nullptr);
  if (res != 0U) {
	auto const msg = gsl::span(static_cast<wchar_t*>(lpMsgBuf), res);
	// erase the \r\n at the end of the msg
	msg[wrap::toSize(res) - 2U] = 0;
#ifdef _DEBUG
	outDebugString(L"{} failed with error [{}], {}\n", prompt, errorCode, static_cast<wchar_t*>(lpMsgBuf));
#else
	auto info = std::wstring {};
	info      = fmt::format(
        fmt::runtime(L"{} failed with error [{}], {}\n"), prompt, errorCode, static_cast<wchar_t*>(lpMsgBuf));
	// displayText::shoMsg(info, false);
#endif
	LocalFree(lpMsgBuf);
  }
}
