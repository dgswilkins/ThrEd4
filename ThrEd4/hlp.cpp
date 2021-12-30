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

// Open Source headers

// Local Headers
#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "thred.h"
#include "hlp.h"

void hlp::help() {
  auto const* homeDir = thred::getHomeDir();
  if (nullptr != homeDir) {
	auto const helpfile = *homeDir / displayText::loadStr(IDS_HELPFN);
#pragma warning(suppress : 26462) // con.4 NOLINTNEXTLINE(readability-qualified-auto)
	auto const helpWindow = HtmlHelp(ThrEdWindow, helpfile.c_str(), HH_DISPLAY_TOPIC, 0);
	if (nullptr == helpWindow) {
	  displayText::tabmsg(IDS_NOHLP);
	}
  }
}
