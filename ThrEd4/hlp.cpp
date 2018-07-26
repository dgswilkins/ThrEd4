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
#include <fmt/format.h>
#pragma warning(pop)

#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "form.h"

HWND HelpWindow;    // help window

void help() {
	std::wstring helpFileName;

	loadString(helpFileName, IDS_HELPFN);
	HelpWindow = HtmlHelp(ThrEdWindow, fmt::format(L"{}{}", HomeDirectory->wstring(), helpFileName).c_str(), HH_DISPLAY_TOPIC, 0);
	if (!HelpWindow)
		tabmsg(IDS_NOHLP);
}

void adbad(std::wstring& repairMessage, unsigned code, size_t count) {
	std::wstring fmtStr;

	loadString(fmtStr, code);
	repairMessage += fmtStr;
	loadString(fmtStr, IDS_NOTREP);
	repairMessage += fmt::format(fmtStr, count);
}


bool clpmsgs(unsigned code) {
	ispcdclp();
	if ((code == FML_CLP || code == FMM_CLP || code == FML_PIC) && !StateMap.test(StateFlag::WASPCDCLP)) {
		tabmsg(IDS_CLPS);
		return 1;
	}
	return 0;
}

void frm1pnt() {
	if (FormIndex == 1) {
		StateMap.set(StateFlag::FORMSEL);
		ClosestFormToCursor = 0;
	}
}

bool filmsgs(unsigned code) {
	if (SelectedFormList->size())
		return clpmsgs(code);
	if (FormIndex) {
		frm1pnt();
		if (StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm = &FormList[ClosestFormToCursor];
			if (SelectedForm->vertexCount == 2) {
				if (code < FML_LIN) {
					tabmsg(IDS_FRM3X);
					return 1;
				}
				else {
					if (code == FML_PRPS) {
						tabmsg(IDS_ANGS);
						return 1;
					}
				}
			}
			return clpmsgs(code);
		}
		else {
			tabmsg(IDS_FILSEL);
			return 1;
		}
	}
	else {
		tabmsg(IDS_FILCR);
		return 1;
	}
}

void grpmsg() {
	shoseln(IDS_FGRPS, IDS_UNGRP);
}

void grpmsg1() {
	tabmsg(IDS_NOGRP);
}

void sdmsg() {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_SAVDISC);
	shoMsg(fmt::format(fmtStr, ThrName->wstring()));
}

void alrotmsg() {
	shoseln(IDS_ALLX, IDS_ROTAGIN);
}

void shord() {
	shoseln(IDS_FGRPF, IDS_ROTDUP);
}

void spltmsg() {
	shoseln(IDS_FRMGUID, IDS_SPLT);
}

void datmsg(unsigned code) {
	unsigned     dataErrorID = 0;
	std::wstring dataError;

	switch (code) {
	case BADFLT:
		dataErrorID = IDS_BADFLT;
	case BADCLP:
		dataErrorID = IDS_BADCLP;
	case BADSAT:
		dataErrorID = IDS_BADSAT;
	case BADTX:
		dataErrorID = IDS_BADTX;
	default:
		dataErrorID = IDS_BADUKN;
	}
	loadString(dataError, dataErrorID);
	shoMsg(dataError);
}
