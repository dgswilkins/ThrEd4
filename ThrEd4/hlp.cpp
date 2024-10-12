// Local Headers
#include "stdafx.h"
#include "displayText.h"
#include "globals.h"
#include "hlp.h"
#include "Resources/resource.h"
#include "thred.h"

// Windows Header Files:
#include <HtmlHelp.h>

void hlp::help() {
  auto const& homeDir = thred::getHomeDir(); 
	auto const helpfile = homeDir / displayText::loadStr(IDS_HELPFN);
#pragma warning(suppress : 26462) // con.4 NOLINTNEXTLINE(readability-qualified-auto)
	if (auto const helpWindow = HtmlHelp(ThrEdWindow, helpfile.c_str(), HH_DISPLAY_TOPIC, 0); nullptr == helpWindow) {
	  displayText::tabmsg(IDS_NOHLP, false);
	}
}
