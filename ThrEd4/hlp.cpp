#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header
#include <HtmlHelp.h>

// C RunTime Header Files

// Open Source headers
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"

HWND HelpWindow;    // help window

void help() {
	std::wstring helpFileName;

	loadString(helpFileName, IDS_HELPFN);
	HelpWindow = HtmlHelp(ThrEdWindow, fmt::format(L"{}{}", HomeDirectory->wstring(), helpFileName).c_str(), HH_DISPLAY_TOPIC, 0);
	if (!HelpWindow)
		tabmsg(IDS_NOHLP);
}

