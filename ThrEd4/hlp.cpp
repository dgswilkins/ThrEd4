// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "displayText.h"
#include "globals.h"
#include "hlp.h"
#include "thred.h"

// Windows Header Files:
#include <HtmlHelp.h>

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
