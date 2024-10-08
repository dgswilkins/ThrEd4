// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#ifndef _DEBUG
#include "switches.h"
#if SHOW_ERROR
#include "displayText.h"
#endif
#endif
#include "reporting.h"
#ifdef _DEBUG
#include "ThrEdTypes.h"
#endif
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#ifndef _DEBUG
#if SHOW_ERROR
#include "fmt/xchar.h"
#endif
#endif
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <minwindef.h>
#include <WinBase.h>
#include <winnt.h>

// report the system error from GetLastError
void rpt::reportError([[maybe_unused]] const wchar_t* prompt, DWORD& errorCode) {
  auto* lpMsgBuf = gsl::narrow_cast<LPVOID>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
  if (auto const res = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     nullptr,
                                     errorCode,
                                     0,
                                     reinterpret_cast<LPTSTR>(&lpMsgBuf),
                                     0,
                                     nullptr);
      res != 0U) { // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
	auto const msg = gsl::span {static_cast<wchar_t*>(lpMsgBuf), res};
	// erase the \r\n at the end of the msg
	msg[wrap::toSize(res) - 2U] = 0;
#ifdef _DEBUG
	outDebugString(L"{} failed with error [{}], {}\n", prompt, errorCode, static_cast<wchar_t*>(lpMsgBuf));
#else
#if SHOW_ERROR
	auto const info = format(
	    fmt::runtime(L"{} failed with error [{}], {}\n"), prompt, errorCode, static_cast<wchar_t*>(lpMsgBuf));
	displayText::shoMsg(info, false);
#endif
#endif
	LocalFree(lpMsgBuf);
  }
}
