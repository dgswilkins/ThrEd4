#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// C RunTime Header Files
#include <Shlwapi.h>

// Open Source headers
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "ExtendedBitSet.h"
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "hlp.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"

unsigned FormMenuEntryCount;        // lines in the form-form
RECT     LabelWindowCoords;         // location of left windows in the form data sheet
POINT    LabelWindowSize;           // size of the left windows in the form data sheet
unsigned PreferenceWindowTextWidth; // size of the text part of the preference window
RECT     ValueWindowCoords;         // location of right windows in the form data sheet
POINT    ValueWindowSize;           // size of the right windows in the form data sheet

unsigned short DaisyTypeStrings[] = {
	IDS_DAZCRV, IDS_DAZSAW, IDS_DAZRMP, IDS_DAZRAG, IDS_DAZCOG, IDS_DAZHART,
};

void maxtsiz(const std::wstring& label, POINT& textSize) {
	SIZE labelSize;

	GetTextExtentPoint32Int(StitchWindowMemDC, label.data(), label.size(), &labelSize);
	textSize.y = labelSize.cy;
	if (labelSize.cx > textSize.x)
		textSize.x = labelSize.cx;
}

void maxwid(unsigned start, unsigned finish) {
	POINT textSize = {};

	while (start <= finish) {
		maxtsiz((*StringTable)[start++], textSize);
	}
	PreferenceWindowTextWidth = textSize.x + 6;
}

HWND txtwin(const std::wstring& windowName, const RECT& location) {
	if (StateMap.test(StateFlag::REFCNT)) {
		maxtsiz(windowName, LabelWindowSize);
		return 0;
	}
	return CreateWindow(L"STATIC",
	                    windowName.c_str(),
	                    WS_CHILD | WS_VISIBLE,
	                    location.left,
	                    location.top,
	                    location.right - location.left,
	                    location.bottom - location.top,
	                    FormDataSheet,
	                    NULL,
	                    ThrEdInstance,
	                    NULL);
}

HWND txtrwin(const std::wstring& winName, const RECT& location) {
	if (StateMap.test(StateFlag::REFCNT)) {
		maxtsiz(winName, ValueWindowSize);
		return 0;
	}
	return CreateWindow(L"STATIC",
	                    winName.c_str(),
	                    SS_NOTIFY | WS_BORDER | WS_CHILD | WS_VISIBLE,
	                    location.left,
	                    location.top,
	                    location.right - location.left,
	                    location.bottom - location.top,
	                    FormDataSheet,
	                    NULL,
	                    ThrEdInstance,
	                    NULL);
}

HWND numwin(const std::wstring& winName, const RECT& location) {
	if (StateMap.test(StateFlag::REFCNT)) {
		maxtsiz(winName, ValueWindowSize);
		return 0;
	}
	return CreateWindow(L"STATIC",
	                    winName.c_str(),
	                    SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
	                    location.left,
	                    location.top,
	                    location.right - location.left,
	                    location.bottom - location.top,
	                    FormDataSheet,
	                    NULL,
	                    ThrEdInstance,
	                    NULL);
}

void nxtlin() noexcept {
	FormMenuEntryCount++;
	LabelWindowCoords.top += LabelWindowSize.y;
	LabelWindowCoords.bottom += LabelWindowSize.y;
	ValueWindowCoords.top += ValueWindowSize.y;
	ValueWindowCoords.bottom += ValueWindowSize.y;
	;
}

void refrmfn() {
	const unsigned short edgeArray[]
	    = { MEGLIN, MEGBLD, MEGCLP, MEGSAT, MEGAP, MEGPRP, MEGHOL, MEGPIC, MEGDUB, MEGCHNH, MEGCHNL, MEGCLPX, 0 };

	std::wstring choice;
	unsigned     edgeFillType = 0, iEdge = 0;
	auto&        stringTable = *StringTable;
	auto&        labelWindow = *LabelWindow;
	auto&        valueWindow = *ValueWindow;

	edgeFillType = SelectedForm->edgeType & NEGUND;
	if (edgeFillType >= EDGELAST) {
		edgeFillType = EDGELAST - 1;
	}
	iEdge                    = edgeFillType - 1;
	ValueWindowCoords.top    = 3;
	LabelWindowCoords.top    = ValueWindowCoords.top;
	ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom;
	LabelWindowCoords.left   = 3;
	LabelWindowCoords.right  = 3 + LabelWindowSize.x;
	ValueWindowCoords.left   = 6 + LabelWindowSize.x;
	ValueWindowCoords.right  = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	labelWindow[LFRM]        = txtwin(stringTable[STR_TXT0], LabelWindowCoords);
	if (SelectedForm->type == FRMLINE)
		choice = stringTable[STR_EDG1];
	else
		choice = stringTable[STR_FREH];
	valueWindow[LFRM] = txtrwin(choice, ValueWindowCoords);
	nxtlin();
	labelWindow[LLAYR] = txtwin(stringTable[STR_TXT1], LabelWindowCoords);
	valueWindow[LLAYR] = txtrwin(fmt::format(L"{}", ((SelectedForm->attribute & FRMLMSK) >> 1)), ValueWindowCoords);
	nxtlin();
	if (SelectedForm->type != FRMLINE) {
		labelWindow[LCWLK] = txtwin(stringTable[STR_CWLK], LabelWindowCoords);
		if (SelectedForm->extendedAttribute & AT_CWLK)
			choice = stringTable[STR_ON];
		else
			choice = stringTable[STR_OFF];
		valueWindow[LCWLK] = txtrwin(choice, ValueWindowCoords);
		nxtlin();
		labelWindow[LWALK] = txtwin(stringTable[STR_WALK], LabelWindowCoords);
		if (SelectedForm->extendedAttribute & AT_WALK)
			choice = stringTable[STR_ON];
		else
			choice = stringTable[STR_OFF];
		valueWindow[LWALK] = txtrwin(choice, ValueWindowCoords);
		nxtlin();
		labelWindow[LUND] = txtwin(stringTable[STR_UND], LabelWindowCoords);
		if (SelectedForm->extendedAttribute & AT_UND)
			choice = stringTable[STR_ON];
		else
			choice = stringTable[STR_OFF];
		valueWindow[LUND] = txtrwin(choice, ValueWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute & (AT_WALK | AT_UND | AT_CWLK)) {
			labelWindow[LUNDCOL] = txtwin(stringTable[STR_UNDCOL], LabelWindowCoords);
			valueWindow[LUNDCOL] = txtrwin(fmt::format(L"{}", (SelectedForm->underlayColor + 1)), ValueWindowCoords);
			nxtlin();
			labelWindow[LULEN] = txtwin(stringTable[STR_ULEN], LabelWindowCoords);
			valueWindow[LULEN] = txtrwin(fmt::format(L"{:.2f}", (SelectedForm->underlayStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		labelWindow[LWLKIND] = txtwin(stringTable[STR_UWLKIND], LabelWindowCoords);
		valueWindow[LWLKIND] = txtrwin(fmt::format(L"{:.2f}", (SelectedForm->underlayIndent / PFGRAN)), ValueWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute & AT_UND) {
			labelWindow[LUSPAC] = txtwin(stringTable[STR_FUSPAC], LabelWindowCoords);
			valueWindow[LUSPAC] = txtrwin(fmt::format(L"{:.2f}", (SelectedForm->underlaySpacing / PFGRAN)), ValueWindowCoords);
			nxtlin();
			labelWindow[LUANG] = txtwin(stringTable[STR_FUANG], LabelWindowCoords);
			valueWindow[LUANG]
			    = txtrwin(fmt::format(L"{:.2f}", (SelectedForm->underlayStitchAngle * 180 / PI)), ValueWindowCoords);
			nxtlin();
		}
	}
	labelWindow[LFRMFIL] = txtwin(stringTable[STR_TXT2], LabelWindowCoords);
	valueWindow[LFRMFIL] = txtrwin(stringTable[STR_FIL0 + SelectedForm->fillType], ValueWindowCoords);
	nxtlin();
	if (SelectedForm->fillType) {
		labelWindow[LFRMCOL] = txtwin(stringTable[STR_TXT3], LabelWindowCoords);
		valueWindow[LFRMCOL] = numwin(fmt::format(L"{}", (SelectedForm->fillColor + 1)), ValueWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == FTHF) {
			labelWindow[LFTHCOL] = txtwin(stringTable[STR_FTHCOL], LabelWindowCoords);
			valueWindow[LFTHCOL] = numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.color + 1)), ValueWindowCoords);
			nxtlin();
			labelWindow[LFTHTYP] = txtwin(stringTable[STR_FTHTYP], LabelWindowCoords);
			valueWindow[LFTHTYP] = numwin(stringTable[STR_FTH0 + SelectedForm->fillInfo.feather.fillType - 1], ValueWindowCoords);
			nxtlin();
			labelWindow[LFTHBLND] = txtwin(stringTable[STR_FTHBLND], LabelWindowCoords);
			if (SelectedForm->extendedAttribute & AT_FTHBLND)
				choice = stringTable[STR_ON];
			else
				choice = stringTable[STR_OFF];
			valueWindow[LFTHBLND] = txtrwin(choice, ValueWindowCoords);
			nxtlin();
			if (!(SelectedForm->extendedAttribute & AT_FTHBLND)) {
				labelWindow[LFTHBTH] = txtwin(stringTable[STR_FTHBOTH], LabelWindowCoords);
				if (SelectedForm->extendedAttribute & (AT_FTHBTH))
					choice = stringTable[STR_ON];
				else
					choice = stringTable[STR_OFF];
				valueWindow[LFTHBTH] = txtrwin(choice, ValueWindowCoords);
				nxtlin();
				if (!(SelectedForm->extendedAttribute & AT_FTHBTH)) {
					labelWindow[LFTHUP] = txtwin(stringTable[STR_FTHUP], LabelWindowCoords);
					if (SelectedForm->extendedAttribute & AT_FTHUP)
						choice = stringTable[STR_ON];
					else
						choice = stringTable[STR_OFF];
					valueWindow[LFTHUP] = txtrwin(choice, ValueWindowCoords);
					nxtlin();
				}
			}
			labelWindow[LFTHUPCNT] = txtwin(stringTable[STR_FTHUPCNT], LabelWindowCoords);
			valueWindow[LFTHUPCNT] = numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.upCount)), ValueWindowCoords);
			nxtlin();
			labelWindow[LFTHDWNCNT] = txtwin(stringTable[STR_FTHDWNCNT], LabelWindowCoords);
			valueWindow[LFTHDWNCNT] = numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.downCount)), ValueWindowCoords);
			nxtlin();
			labelWindow[LFTHSIZ] = txtwin(stringTable[STR_FTHSIZ], LabelWindowCoords);
			valueWindow[LFTHSIZ] = numwin(fmt::format(L"{:.2f}", (SelectedForm->fillInfo.feather.ratio)), ValueWindowCoords);
			nxtlin();
			if (SelectedForm->fillInfo.feather.fillType == FTHPSG) {
				labelWindow[LFTHNUM] = txtwin(stringTable[STR_FTHNUM], LabelWindowCoords);
				valueWindow[LFTHNUM] = numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.count)), ValueWindowCoords);
				nxtlin();
			}
			labelWindow[LFTHFLR] = txtwin(stringTable[STR_FTHFLR], LabelWindowCoords);
			valueWindow[LFTHFLR]
			    = numwin(fmt::format(L"{:.2f}", (SelectedForm->fillInfo.feather.minStitchSize / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType != CLPF) {
			labelWindow[LFRMSPAC] = txtwin(stringTable[STR_TXT4], LabelWindowCoords);
			valueWindow[LFRMSPAC] = numwin(fmt::format(L"{:.2f}", (SelectedForm->fillSpacing / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (istx(ClosestFormToCursor)) {
			labelWindow[LTXOF] = txtwin(stringTable[STR_TXOF], LabelWindowCoords);
			valueWindow[LTXOF] = numwin(fmt::format(L"{:.2f}", (SelectedForm->txof / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		labelWindow[LMAXFIL] = txtwin(stringTable[STR_TXT20], LabelWindowCoords);
		valueWindow[LMAXFIL] = numwin(fmt::format(L"{:.2f}", (SelectedForm->maxFillStitchLen / PFGRAN)), ValueWindowCoords);
		nxtlin();
		if (!isclp(ClosestFormToCursor) && !istx(ClosestFormToCursor)) {
			labelWindow[LFRMLEN] = txtwin(stringTable[STR_TXT5], LabelWindowCoords);
			valueWindow[LFRMLEN]
			    = numwin(fmt::format(L"{:.2f}", (SelectedForm->lengthOrCount.stitchLength / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		labelWindow[LMINFIL] = txtwin(stringTable[STR_TXT21], LabelWindowCoords);
		valueWindow[LMINFIL] = numwin(fmt::format(L"{:.2f}", (SelectedForm->minFillStitchLen / PFGRAN)), ValueWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == ANGF || SelectedForm->fillType == TXANGF) {
			labelWindow[LFRMANG] = txtwin(stringTable[STR_TXT6], LabelWindowCoords);
			valueWindow[LFRMANG]
			    = numwin(fmt::format(L"{:.2f}", (SelectedForm->angleOrClipData.angle * 180 / PI)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == ANGCLPF) {
			labelWindow[LSACANG] = txtwin(stringTable[STR_TXT6], LabelWindowCoords);
			valueWindow[LSACANG]
			    = numwin(fmt::format(L"{:.2f}", (SelectedForm->satinOrAngle.angle * 180 / PI)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VCLPF || SelectedForm->fillType == HCLPF || SelectedForm->fillType == ANGCLPF) {
			labelWindow[LFRMFAZ] = txtwin(stringTable[STR_TXT18], LabelWindowCoords);
			valueWindow[LFRMFAZ] = numwin(fmt::format(L"{}", (SelectedForm->wordParam)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VRTF || SelectedForm->fillType == HORF || SelectedForm->fillType == ANGF
		    || istx(ClosestFormToCursor)) {
			labelWindow[LBFILSQR] = txtwin(stringTable[STR_PRF2], LabelWindowCoords);
			if (SelectedForm->extendedAttribute & AT_SQR)
				choice = stringTable[STR_SQR];
			else
				choice = stringTable[STR_PNTD];
			valueWindow[LBFILSQR] = txtrwin(choice, ValueWindowCoords);
			nxtlin();
		}
	}
	labelWindow[LFSTRT] = txtwin(stringTable[STR_FSTRT], LabelWindowCoords);
	if (SelectedForm->extendedAttribute & AT_STRT)
		choice = stringTable[STR_ON];
	else
		choice = stringTable[STR_OFF];
	valueWindow[LFSTRT] = txtrwin(choice, ValueWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute & AT_STRT) {
		labelWindow[LDSTRT] = txtwin(stringTable[STR_FSTRT], LabelWindowCoords);
		valueWindow[LDSTRT] = numwin(fmt::format(L"{}", (SelectedForm->fillStart)), ValueWindowCoords);
		nxtlin();
	}
	labelWindow[LFEND] = txtwin(stringTable[STR_FEND], LabelWindowCoords);
	if (SelectedForm->extendedAttribute & AT_END)
		choice = stringTable[STR_ON];
	else
		choice = stringTable[STR_OFF];
	valueWindow[LFEND] = txtrwin(choice, ValueWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute & AT_END) {
		labelWindow[LDEND] = txtwin(stringTable[STR_FEND], LabelWindowCoords);
		valueWindow[LDEND] = numwin(fmt::format(L"{}", (SelectedForm->fillEnd)), ValueWindowCoords);
		nxtlin();
	}
	labelWindow[LBRD] = txtwin(stringTable[STR_TXT7], LabelWindowCoords);
	valueWindow[LBRD] = txtrwin(stringTable[STR_EDG0 + edgeFillType], ValueWindowCoords);
	nxtlin();
	if (edgeFillType) {
		labelWindow[LBRDCOL] = txtwin(stringTable[STR_TXT8], LabelWindowCoords);
		valueWindow[LBRDCOL] = numwin(fmt::format(L"{}", ((SelectedForm->borderColor & COLMSK) + 1)), ValueWindowCoords);
		nxtlin();
		if (edgeArray[iEdge] & BESPAC) {
			labelWindow[LBRDSPAC] = txtwin(stringTable[STR_TXT9], LabelWindowCoords);
			if (edgeFillType == EDGEPROPSAT || edgeFillType == EDGEOCHAIN || edgeFillType == EDGELCHAIN)
				choice = fmt::format(L"{:.2f}", (SelectedForm->edgeSpacing / PFGRAN));
			else
				choice = fmt::format(L"{:.2f}", (SelectedForm->edgeSpacing / PFGRAN * 2));
			valueWindow[LBRDSPAC] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & BPICSPAC) {
			labelWindow[LBRDPIC] = txtwin(stringTable[STR_TXT16], LabelWindowCoords);
			valueWindow[LBRDPIC] = numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeSpacing / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & BEMAX) {
			labelWindow[LMAXBRD] = txtwin(stringTable[STR_TXT22], LabelWindowCoords);
			valueWindow[LMAXBRD] = numwin(fmt::format(L"{:.2f}", (SelectedForm->maxBorderStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & BELEN) {
			labelWindow[LBRDLEN] = txtwin(stringTable[STR_TXT10], LabelWindowCoords);
			valueWindow[LBRDLEN] = numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & BEMIN) {
			labelWindow[LMINBRD] = txtwin(stringTable[STR_TXT23], LabelWindowCoords);
			valueWindow[LMINBRD] = numwin(fmt::format(L"{:.2f}", (SelectedForm->minBorderStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & BESIZ) {
			labelWindow[LBRDSIZ] = txtwin(stringTable[STR_TXT11], LabelWindowCoords);
			valueWindow[LBRDSIZ] = numwin(fmt::format(L"{:.2f}", (SelectedForm->borderSize / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & BRDPOS) {
			labelWindow[LBRDPOS] = txtwin(stringTable[STR_TXT18], LabelWindowCoords);
			valueWindow[LBRDPOS] = numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeStitchLen)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & CHNPOS) {
			labelWindow[LBRDPOS] = txtwin(stringTable[STR_TXT19], LabelWindowCoords);
			valueWindow[LBRDPOS] = numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeStitchLen)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeFillType == EDGEAPPL) {
			labelWindow[LAPCOL] = txtwin(stringTable[STR_TXT12], LabelWindowCoords);
			valueWindow[LAPCOL] = numwin(fmt::format(L"{}", ((SelectedForm->borderColor >> 4) + 1)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeFillType == EDGEANGSAT || edgeFillType == EDGEAPPL || edgeFillType == EDGEPROPSAT) {
			labelWindow[LBRDUND] = txtwin(stringTable[STR_TXT17], LabelWindowCoords);
			if (SelectedForm->edgeType & EGUND)
				choice = stringTable[STR_ON];
			else
				choice = stringTable[STR_OFF];
			valueWindow[LBRDUND] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
		if (edgeArray[iEdge] & BCNRSIZ) {
			if (SelectedForm->edgeType == EDGEBHOL)
				choice = fmt::format(L"{:.2f}", (getblen() / PFGRAN));
			else
				choice = fmt::format(L"{:.2f}", (getplen() / PFGRAN));
			labelWindow[LBCSIZ] = txtwin(stringTable[STR_TXT13], LabelWindowCoords);
			valueWindow[LBCSIZ] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->type == FRMLINE && edgeArray[iEdge] & BRDEND) {
			labelWindow[LBSTRT] = txtwin(stringTable[STR_TXT14], LabelWindowCoords);
			if (SelectedForm->attribute & SBLNT)
				choice = stringTable[STR_BLUNT];
			else
				choice = stringTable[STR_TAPR];
			valueWindow[LBSTRT] = numwin(choice, ValueWindowCoords);
			nxtlin();
			labelWindow[LBFIN] = txtwin(stringTable[STR_TXT15], LabelWindowCoords);
			if (SelectedForm->attribute & FBLNT)
				choice = stringTable[STR_BLUNT];
			else
				choice = stringTable[STR_TAPR];
			valueWindow[LBFIN] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
	}
}

BOOL CALLBACK chenum(HWND hwnd, LPARAM lParam) noexcept {
	UNREFERENCED_PARAMETER(lParam);

	return DestroyWindow(hwnd);
}

void refrm() {
	SelectedForm = &FormList[ClosestFormToCursor];
	if (StateMap.testAndReset(StateFlag::PRFACT)) {
		DestroyWindow(PreferencesWindow);
		StateMap.reset(StateFlag::WASRT);
	}
	LabelWindowSize = ValueWindowSize = {};
	StateMap.set(StateFlag::REFCNT);
	FormMenuEntryCount = 0;
	refrmfn();
	if (FormDataSheet) {
		while (EnumChildWindows(FormDataSheet, chenum, 0))
			;
		MoveWindow(FormDataSheet,
		           ButtonWidthX3 + 3,
		           3,
		           LabelWindowSize.x + ValueWindowSize.x + 18,
		           LabelWindowSize.y * FormMenuEntryCount + 12,
		           TRUE);
		redraw(FormDataSheet);
	}
	else {
		FormDataSheet = CreateWindow(L"STATIC",
		                             0,
		                             WS_CHILD | WS_VISIBLE | WS_BORDER,
		                             ButtonWidthX3 + 3,
		                             3,
		                             LabelWindowSize.x + ValueWindowSize.x + 18,
		                             LabelWindowSize.y * FormMenuEntryCount + 12,
		                             ThrEdWindow,
		                             NULL,
		                             ThrEdInstance,
		                             NULL);
	}
	StateMap.reset(StateFlag::REFCNT);
	refrmfn();
}

void sidwnd(HWND wnd) {
	RECT     windowRect  = {};
	unsigned savedChoice = 0;

	MsgIndex                 = 0;
	SideWindowEntryBuffer[0] = 0;
	savedChoice              = FormMenuChoice;
	unsid();
	FormMenuChoice = savedChoice;
	GetWindowRect(wnd, &windowRect);
	GetWindowRect(FormDataSheet, &MsgRect);
	SideMessageWindow = CreateWindow(L"STATIC",
	                                 0,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 MsgRect.right - ThredWindowOrigin.x + 3,
	                                 windowRect.top - ThredWindowOrigin.y - 3,
	                                 ButtonWidthX3,
	                                 windowRect.bottom - windowRect.top + 3,
	                                 ThrEdWindow,
	                                 NULL,
	                                 ThrEdInstance,
	                                 NULL);
}

void prfsid(HWND wnd) {
	RECT windowRect = {};

	MsgIndex                 = 0;
	SideWindowEntryBuffer[0] = 0;
	unsid();
	GetWindowRect(wnd, &windowRect);
	GetClientRect(PreferencesWindow, &MsgRect);
	SideMessageWindow = CreateWindow(L"STATIC",
	                                 0,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 windowRect.right - ThredWindowOrigin.x + 6,
	                                 windowRect.top - ThredWindowOrigin.y - 3,
	                                 ValueWindowSize.x,
	                                 windowRect.bottom - windowRect.top + 3,
	                                 ThrEdWindow,
	                                 NULL,
	                                 ThrEdInstance,
	                                 NULL);
}

void prftwin(const std::wstring& text) noexcept {
	CreateWindow(L"STATIC",
	             text.c_str(),
	             WS_CHILD | WS_VISIBLE,
	             LabelWindowCoords.left,
	             LabelWindowCoords.top,
	             LabelWindowCoords.right - LabelWindowCoords.left,
	             LabelWindowCoords.bottom - LabelWindowCoords.top,
	             PreferencesWindow,
	             NULL,
	             ThrEdInstance,
	             NULL);
}

HWND prfnwin(const std::wstring& text) noexcept {
	return CreateWindow(L"STATIC",
	                    text.c_str(),
	                    SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
	                    ValueWindowCoords.left,
	                    ValueWindowCoords.top,
	                    ValueWindowCoords.right - ValueWindowCoords.left,
	                    ValueWindowCoords.bottom - ValueWindowCoords.top,
	                    PreferencesWindow,
	                    NULL,
	                    ThrEdInstance,
	                    NULL);
}

void prflin(const std::wstring& msg, unsigned row) {
	prftwin((*StringTable)[row]);
	(*ValueWindow)[row - STR_PRF0] = prfnwin(msg);
	nxtlin();
}

void prfmsg() {
	std::wstring choice;
	HDC          preferenceDC   = {};
	RECT         preferenceRect = {};

	if (StateMap.testAndReset(StateFlag::INSRT))
		StateMap.set(StateFlag::WASRT);
	StateMap.reset(StateFlag::BIGBOX);
	SelectedFormList->clear();
	if (FormDataSheet) {
		undat();
		unsid();
		FormMenuChoice = 0;
	}
	LabelWindowSize.x = LabelWindowSize.y = 0;
	ValueWindowSize.x = ValueWindowSize.y = 0;
	maxtsiz((*StringTable)[STR_PRF0 + 4], LabelWindowSize);
	maxtsiz((*StringTable)[STR_TAPR], ValueWindowSize);
	LabelWindowSize.x = PreferenceWindowTextWidth;
	LabelWindowSize.x += 4;
	DestroyWindow(PreferencesWindow);
	PreferenceWindowWidth = LabelWindowSize.x + ValueWindowSize.x + 18;
	PreferencesWindow     = CreateWindow(L"STATIC",
                                     0,
                                     WS_CHILD | WS_VISIBLE | WS_BORDER,
                                     ButtonWidthX3 + 3,
                                     3,
                                     PreferenceWindowWidth,
                                     LabelWindowSize.y * PRFLINS + 12,
                                     ThrEdWindow,
                                     NULL,
                                     ThrEdInstance,
                                     NULL);
	preferenceDC          = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &preferenceRect);
	FillRect(preferenceDC, &preferenceRect, GetSysColorBrush(COLOR_WINDOW));
	LabelWindowCoords.top = ValueWindowCoords.top = 3;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.left                              = 3;
	LabelWindowCoords.right                             = 3 + LabelWindowSize.x;
	ValueWindowCoords.left                              = 6 + LabelWindowSize.x;
	ValueWindowCoords.right                             = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	prflin(fmt::format(L"{}", (AppliqueColor + 1)), STR_PRF10);
	prflin(fmt::format(L"{:.2f}", (BorderWidth / PFGRAN)), STR_PRF3);
	prflin(fmt::format(L"{:.2f}", (ButtonholeCornerLength / PFGRAN)), STR_PRF14);
	prflin(fmt::format(L"{:.2f}", (IniFile.chainSpace / PFGRAN)), STR_PRF23);
	prflin(fmt::format(L"{:.2f}", (IniFile.chainRatio)), STR_PRF24);
	prflin(fmt::format(L"{:.2f} mm", (IniFile.clipOffset / PFGRAN)), STR_PRF21);
	prflin(fmt::format(L"{}", (IniFile.fillPhase)), STR_PRF22);
	prflin(fmt::format(L"{:.2f}", (IniFile.eggRatio)), STR_PRF26);
	prflin(fmt::format(L"{:.2f}", (IniFile.fillAngle / PI * 180)), STR_PRF1);
	if (UserFlagMap.test(UserFlag::SQRFIL))
		choice = (*StringTable)[STR_SQR];
	else
		choice = (*StringTable)[STR_PNTD];
	prflin(choice, STR_PRF2);
	prflin(fmt::format(L"{:.2f}", (LineSpacing / PFGRAN)), STR_PRF0);
	prflin(fmt::format(L"{}", (duthrsh(ShowStitchThreshold))), STR_PRF7);
	prflin(fmt::format(L"{:.2f} mm", (IniFile.gridSize / PFGRAN)), STR_PRF20);
	sethup();
	prflin(fmt::format(L"{}", (*StringTable)[STR_HUP0 + IniFile.hoopType - 1]), STR_PRF17);
	prflin(fmt::format(L"{:.0f} mm", (IniFile.hoopSizeY / PFGRAN)), STR_PRF27);
	prflin(fmt::format(L"{:.0f} mm", (IniFile.hoopSizeX / PFGRAN)), STR_PRF18);
	prflin(fmt::format(L"{:.2f}", (IniFile.cursorNudgeStep)), STR_PRF25);
	prflin(fmt::format(L"{:.2f}", (PicotSpacing / PFGRAN)), STR_PRF16);
	if (UserFlagMap.test(UserFlag::BLUNT))
		choice = (*StringTable)[STR_BLUNT];
	else
		choice = (*StringTable)[STR_TAPR];
	prflin(choice, STR_PRF15);
	if (UserFlagMap.test(UserFlag::DUND))
		choice = (*StringTable)[STR_ON];
	else
		choice = (*StringTable)[STR_OFF];
	prflin(choice, STR_PRF19);
	prflin(fmt::format(L"{:.2f}", (SmallStitchLength / PFGRAN)), STR_PRF9);
	prflin(fmt::format(L"{:.2f}", (SnapLength / PFGRAN)), STR_PRF11);
	prflin(fmt::format(L"{:.2f}", (SpiralWrap)), STR_PRF13);
	prflin(fmt::format(L"{:.2f}", (StarRatio)), STR_PRF12);
	prflin(fmt::format(L"{}", (duthrsh(StitchBoxesThreshold))), STR_PRF8);
	prflin(fmt::format(L"{:.2f}", (IniFile.maxStitchLength / PFGRAN)), STR_PRF4);
	prflin(fmt::format(L"{:.2f}", (UserStitchLength / PFGRAN)), STR_PRF5);
	prflin(fmt::format(L"{:.2f}", (MinStitchLength / PFGRAN)), STR_PRF6);
	StateMap.set(StateFlag::PRFACT);
	ReleaseDC(ThrEdWindow, preferenceDC);
}

void frmnum() {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_FRML);
	if (FormIndex && StateMap.test(StateFlag::FORMSEL)) {
		shoMsg(fmt::format(fmtStr, FormIndex));
		StateMap.set(StateFlag::NUMIN);
		StateMap.set(StateFlag::ENTRFNUM);
		numWnd();
	}
	else
		shoseln(IDS_FRM1MSG, IDS_SETFRM);
}

void chkdaz() noexcept {
	if (!IniFile.daisyPetalPoints)
		IniFile.daisyPetalPoints = 1;
	if (!IniFile.daisyInnerCount)
		IniFile.daisyInnerCount = 1;
	if (!IniFile.daisyPetalCount)
		IniFile.daisyPetalCount = 1;
	if (IniFile.daisyHeartCount > IniFile.daisyPetalPoints)
		IniFile.daisyHeartCount = IniFile.daisyPetalPoints;
}

void dazdef() {
	IniFile.daisyPetalPoints = DAZCNT;
	IniFile.daisyHoleDiameter = DAZHLEN;
	IniFile.daisyInnerCount = DAZICNT;
	IniFile.daisyDiameter = DAZLEN;
	IniFile.daisyPetalCount = DAZPETS;
	IniFile.daisyPetalLen = DAZPLEN;
	IniFile.daisyHeartCount = DAZMCNT;
	UserFlagMap.set(UserFlag::DAZHOL);
	UserFlagMap.set(UserFlag::DAZD);
	IniFile.daisyBorderType = DAZTYP;
}

void initdaz(HWND hWinDialog) {
	unsigned iType = 0;

	chkdaz();
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLPNTS), fmt::format(L"{}", IniFile.daisyPetalPoints).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_DAZPCNT), fmt::format(L"{}", IniFile.daisyHeartCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_CNTLEN), fmt::format(L"{:.2f}", IniFile.daisyDiameter).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_HOLSIZ), fmt::format(L"{:.2f}", IniFile.daisyHoleDiameter).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_INPNTS), fmt::format(L"{}", IniFile.daisyInnerCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETALS), fmt::format(L"{}", IniFile.daisyPetalCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLEN), fmt::format(L"{:.2f}", IniFile.daisyPetalLen).c_str());
	CheckDlgButton(hWinDialog, IDC_HOLE, UserFlagMap.test(UserFlag::DAZHOL));
	CheckDlgButton(hWinDialog, IDC_DLIN, UserFlagMap.test(UserFlag::DAZD));
	std::wstring daisyType;
	for (iType = 0; iType < 6; iType++) {
		loadString(daisyType, DaisyTypeStrings[iType]);
		[[gsl::suppress(type .1)]] SendMessage(
		    GetDlgItem(hWinDialog, IDC_DAZTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(daisyType.c_str()));
	}
	SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
}

BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	wchar_t  buffer[HBUFSIZ]        = { 0 };
	wchar_t  compareBuffer[HBUFSIZ] = { 0 };
	unsigned iType                  = 0;

	switch (umsg) {
	case WM_INITDIALOG:
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		initdaz(hwndlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDCANCEL:
			EndDialog(hwndlg, 0);
			return TRUE;
		case IDOK:
			GetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buffer, HBUFSIZ);
			IniFile.daisyPetalPoints = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buffer, HBUFSIZ);
			IniFile.daisyHeartCount = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buffer, HBUFSIZ);
			IniFile.daisyDiameter = std::stof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buffer, HBUFSIZ);
			IniFile.daisyHoleDiameter = std::stof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buffer, HBUFSIZ);
			IniFile.daisyInnerCount = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buffer, HBUFSIZ);
			IniFile.daisyPetalCount = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buffer, HBUFSIZ);
			IniFile.daisyPetalLen = std::stof(buffer);
			if (IsDlgButtonChecked(hwndlg, IDC_HOLE))
				UserFlagMap.set(UserFlag::DAZHOL);
			else
				UserFlagMap.reset(UserFlag::DAZHOL);
			if (IsDlgButtonChecked(hwndlg, IDC_DLIN))
				UserFlagMap.set(UserFlag::DAZD);
			else
				UserFlagMap.reset(UserFlag::DAZD);
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZTYP), buffer, HBUFSIZ);
			for (iType = 0; iType < 6; iType++) {
				LoadString(ThrEdInstance, DaisyTypeStrings[iType], compareBuffer, HBUFSIZ);
				if (!StrCmpW(buffer, compareBuffer)) {
					IniFile.daisyBorderType = iType;
					break;
				}
			}
			chkdaz();
			EndDialog(hwndlg, 1);
			break;
		case IDC_DAZRST:
			dazdef();
			initdaz(hwndlg);
			break;
		case IDC_DLIN:
			if (IsDlgButtonChecked(hwndlg, IDC_DLIN))
				CheckDlgButton(hwndlg, IDC_HOLE, BST_CHECKED);
			break;
		case IDC_HOLE:
			if (!IsDlgButtonChecked(hwndlg, IDC_HOLE))
				CheckDlgButton(hwndlg, IDC_DLIN, BST_UNCHECKED);
			break;
		}
	}
	return 0;
}

void dasyfrm() {
	double   holeSegmentAngle = 0.0, petalSegmentAngle = 0.0;
	double   angle           = 0.0;
	double   deltaPetalAngle = 0.0;
	double   petalPointAngle = 0.0;
	unsigned holeVertexCount = 0, petalVertexCount = 0, iSegment = 0, iMacroPetal = 0, iPoint = 0;
	unsigned iVertex = 0, borderType = 0, sawPointCount = 0, halfPetalPointCount = 0, fref = 0, petalPointCount = 0;
	double   distanceFromDaisyCenter = 0.0;
	double   diameter                = 0.0;
	double   petalLength             = 0.0;
	double   holeDiameter            = 0.0;
	double   ratio                   = 0.0;
	float    maximumXsize = 0.0, maximumYsize = 0.0;
	fPOINT   referencePoint = {};

	unmsg();
	if (!DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, (DLGPROC)dasyproc)) {
		StateMap.reset(StateFlag::FORMIN);
		return;
	}
	IniFile.daisyPetalCount = IniFile.daisyPetalCount;
	diameter                = IniFile.daisyDiameter;
	petalLength             = IniFile.daisyPetalLen;
	holeDiameter            = IniFile.daisyHoleDiameter;
	borderType              = IniFile.daisyBorderType;
	referencePoint.x        = midl(ZoomRect.right, ZoomRect.left);
	referencePoint.y        = midl(ZoomRect.top, ZoomRect.bottom);
	SelectedForm            = &FormList[FormIndex];
	ClosestFormToCursor     = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices  = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	halfPetalPointCount = IniFile.daisyPetalPoints >> 1;
	maximumXsize        = ZoomRect.right - ZoomRect.left;
	maximumYsize        = ZoomRect.top - ZoomRect.bottom;
	if (maximumYsize > maximumXsize)
		maximumXsize = maximumYsize;
	maximumXsize /= 6;
	ratio = maximumXsize / (diameter + petalLength);
	diameter *= ratio;
	petalLength *= ratio;
	holeDiameter *= ratio;
	SelectedForm->type = FRMFPOLY;
	iVertex = fref = 0;
	if (UserFlagMap.test(UserFlag::DAZHOL)) {
		angle                          = PI2;
		holeVertexCount                = IniFile.daisyPetalCount * IniFile.daisyInnerCount;
		holeSegmentAngle               = PI2 / holeVertexCount;
		CurrentFormVertices[iVertex].x = referencePoint.x + diameter * cos(angle);
		CurrentFormVertices[iVertex].y = referencePoint.y + diameter * sin(angle);
		iVertex++;
		for (iSegment = 0; iSegment < holeVertexCount + 1; iSegment++) {
			CurrentFormVertices[iVertex].x = referencePoint.x + holeDiameter * cos(angle);
			CurrentFormVertices[iVertex].y = referencePoint.y + holeDiameter * sin(angle);
			iVertex++;
			angle -= holeSegmentAngle;
		}
		fref = iVertex;
	}
	angle            = 0;
	petalVertexCount = IniFile.daisyPetalCount * IniFile.daisyPetalPoints;
	petalPointCount  = IniFile.daisyPetalPoints;
	if (borderType == DHART) {
		petalPointCount  = (IniFile.daisyHeartCount + 1) << 1;
		petalVertexCount = IniFile.daisyPetalCount * petalPointCount;
	}
	petalSegmentAngle = PI2 / petalVertexCount;
	deltaPetalAngle   = PI / IniFile.daisyPetalPoints;
	if (UserFlagMap.test(UserFlag::DAZD)) {
		SelectedForm->satinGuideCount    = IniFile.daisyPetalCount - 1;
		SelectedForm->wordParam          = IniFile.daisyPetalCount * IniFile.daisyInnerCount + 1;
		SelectedForm->satinOrAngle.guide = adsatk(IniFile.daisyPetalCount - 1);
	}
	for (iMacroPetal = 0; iMacroPetal < IniFile.daisyPetalCount; iMacroPetal++) {
		petalPointAngle   = 0;
		PseudoRandomValue = SEED;
		for (iPoint = 0; iPoint < petalPointCount; iPoint++) {
			switch (borderType) {
			case DSIN:
				distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
				petalPointAngle += deltaPetalAngle;
				break;
			case DRAMP:
				distanceFromDaisyCenter = diameter + (static_cast<double>(iPoint) / IniFile.daisyPetalPoints * petalLength);
				break;
			case DSAW:
				if (iPoint > halfPetalPointCount)
					sawPointCount = IniFile.daisyPetalPoints - iPoint;
				else
					sawPointCount = iPoint;
				distanceFromDaisyCenter
				    = diameter + (static_cast<double>(sawPointCount) / IniFile.daisyPetalPoints * petalLength);
				break;
			case DRAG:
				distanceFromDaisyCenter
				    = diameter + (static_cast<double>(psg() % IniFile.daisyPetalPoints) / IniFile.daisyPetalPoints * petalLength);
				break;
			case DCOG:
				distanceFromDaisyCenter = diameter;
				if (iPoint > halfPetalPointCount)
					distanceFromDaisyCenter += petalLength;
				break;
			case DHART:
				distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
				if (iPoint > IniFile.daisyHeartCount)
					petalPointAngle -= deltaPetalAngle;
				else
					petalPointAngle += deltaPetalAngle;
				break;
			}
			CurrentFormVertices[iVertex].x = referencePoint.x + cos(angle) * distanceFromDaisyCenter;
			CurrentFormVertices[iVertex].y = referencePoint.y + sin(angle) * distanceFromDaisyCenter;
			iVertex++;
			angle += petalSegmentAngle;
			if (UserFlagMap.test(UserFlag::DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
				SelectedForm->satinOrAngle.guide[iMacroPetal].start
				    = (IniFile.daisyPetalCount - iMacroPetal - 1) * IniFile.daisyInnerCount + 1;
				SelectedForm->satinOrAngle.guide[iMacroPetal].finish = iVertex;
			}
		}
	}
	if (UserFlagMap.test(UserFlag::DAZHOL)) {
		CurrentFormVertices[fref - 1].y += 0.01f;
		CurrentFormVertices[fref].y += 0.01f;
	}
	SelectedForm->vertexCount = iVertex;
	if (UserFlagMap.test(UserFlag::DAZD)) {
		SelectedForm->type      = SAT;
		SelectedForm->attribute = 1;
	}
	FormVertexIndex += iVertex;
	StateMap.set(StateFlag::INIT);
	frmout(FormIndex);
	for (iMacroPetal = 0; iMacroPetal < iVertex; iMacroPetal++) {
		CurrentFormVertices[iMacroPetal].x -= SelectedForm->rectangle.left;
		CurrentFormVertices[iMacroPetal].y -= SelectedForm->rectangle.bottom;
	}
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount                = iVertex + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	mdufrm();
}

inline void initTearDlg(HWND hwndlg) {
	SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), fmt::format(L"{:d}", IniFile.formSides).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), fmt::format(L"{:.3f}", IniFile.tearTailLength).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), fmt::format(L"{:.3f}", IniFile.tearTwistStep / PFGRAN).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), fmt::format(L"{:.3f}", IniFile.tearTwistRatio).c_str());
}

BOOL CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	wchar_t buffer[HBUFSIZ] = { 0 };

	switch (umsg) {
	case WM_INITDIALOG:
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		initTearDlg(hwndlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDCANCEL:
			EndDialog(hwndlg, 0);
			return TRUE;
		case IDOK:
			GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buffer, HBUFSIZ);
			IniFile.formSides = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buffer, HBUFSIZ);
			IniFile.tearTailLength = std::stof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buffer, HBUFSIZ);
			IniFile.tearTwistStep = std::stof(buffer) * PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buffer, HBUFSIZ);
			IniFile.tearTwistRatio = std::stof(buffer);
			EndDialog(hwndlg, 1);
			break;
		case IDC_DEFTEAR:
			IniFile.formSides      = 20;
			IniFile.tearTailLength = 1.1f;
			IniFile.tearTwistStep  = 0.0f;
			IniFile.tearTwistRatio = 1.6f;
			initTearDlg(hwndlg);
			break;
		case IDC_DEFPAIS:
			IniFile.formSides      = 24;
			IniFile.tearTailLength = 1.15f;
			IniFile.tearTwistStep  = 0.3f * PFGRAN;
			IniFile.tearTwistRatio = 1.8f;
			initTearDlg(hwndlg);
			break;
		}
	}
	return 0;
}

void setear() {
	INT_PTR nResult = 0;
	size_t  count = 0, iRightVertices = 0, iLeftVertices = 0;
	double  middle           = 0.0;
	double  verticalPosition = 0.0;
	double  length           = 0.0;
	double  step             = 0.0;
	double  horizontalRatio  = 0.0;
	double  verticalRatio    = 0.0;
	float   twistStep        = 0.0;
	fPOINT  size             = {};

	unmsg();
	nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_TEAR), ThrEdWindow, (DLGPROC)tearprc);
	if (nResult > 0) {
		twistStep = IniFile.tearTwistStep;
		durpoli(IniFile.formSides);
		fvars(FormIndex);
		count            = VertexCount / 4;
		middle           = (CurrentFormVertices[1].x - CurrentFormVertices[0].x) / 2 + CurrentFormVertices[0].x;
		length           = CurrentFormVertices[count].y - CurrentFormVertices[0].y;
		step             = CurrentFormVertices[count + 1].y - CurrentFormVertices[count].y;
		verticalPosition = CurrentFormVertices[count + 1].y;
		iLeftVertices    = VertexCount - count;
		iRightVertices   = count + 1;
		for (size_t iStep = 0; iStep < count; iStep++) {
			CurrentFormVertices[iRightVertices].y = CurrentFormVertices[iLeftVertices].y = verticalPosition;
			CurrentFormVertices[iRightVertices].x += twistStep;
			CurrentFormVertices[iLeftVertices].x += twistStep;
			twistStep *= IniFile.tearTwistRatio;
			verticalPosition -= step;
			step *= IniFile.tearTailLength;
			iRightVertices--;
			iLeftVertices++;
		}
		CurrentFormVertices[0].y = CurrentFormVertices[1].y = verticalPosition;
		CurrentFormVertices[0].x += twistStep;
		CurrentFormVertices[1].x += twistStep;
		verticalPosition -= step / 2;
		CurrentFormVertices[VertexCount] = CurrentFormVertices[0];
		if (twistStep)
			CurrentFormVertices[0].x = CurrentFormVertices[1].x + twistStep / 4;
		else
			CurrentFormVertices[0].x = middle;
		CurrentFormVertices[0].y = verticalPosition;
		SelectedForm->vertexCount++;
		NewFormVertexCount++;
		FormVertexIndex++;
		StateMap.set(StateFlag::FORMSEL);
		fvars(FormIndex);
		frmout(FormIndex);
		flipv();
		StateMap.reset(StateFlag::FORMSEL);
		size.x          = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		size.y          = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		horizontalRatio = UnzoomedRect.x / 4 / size.x;
		if (horizontalRatio > 1)
			horizontalRatio = 1;
		verticalRatio = UnzoomedRect.y / 4 / size.y;
		if (verticalRatio < horizontalRatio)
			horizontalRatio = verticalRatio;
		if (horizontalRatio < 1) {
			for (size_t iVertex = 0u; iVertex < VertexCount; iVertex++) {
				CurrentFormVertices[iVertex].x
				    = (CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x) * horizontalRatio + CurrentFormVertices[0].x;
				CurrentFormVertices[iVertex].y
				    = (CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y) * horizontalRatio + CurrentFormVertices[0].y;
			}
		}
		frmout(FormIndex);
		for (size_t iVertex = 0u; iVertex < VertexCount; iVertex++) {
			CurrentFormVertices[iVertex].x -= SelectedForm->rectangle.left;
			CurrentFormVertices[iVertex].y -= SelectedForm->rectangle.bottom;
		}
	}
}

inline void wavinit(HWND hwndlg) {
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), fmt::format(L"{}", IniFile.wavePoints).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), fmt::format(L"{}", IniFile.waveStart).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), fmt::format(L"{}", IniFile.waveEnd).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVS), fmt::format(L"{}", IniFile.waveLobes).c_str());
}

bool CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	wchar_t buffer[HBUFSIZ] = { 0 };

	switch (umsg) {
	case WM_INITDIALOG:
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		wavinit(hwndlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDCANCEL:
			EndDialog(hwndlg, 0);
			return TRUE;
		case IDOK:
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buffer, HBUFSIZ);
			IniFile.wavePoints = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buffer, HBUFSIZ);
			IniFile.waveStart = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buffer, HBUFSIZ);
			IniFile.waveEnd = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buffer, HBUFSIZ);
			IniFile.waveLobes = std::stoi(buffer);
			if (IniFile.wavePoints > 100)
				IniFile.wavePoints = 100;
			if (IniFile.wavePoints < 3)
				IniFile.wavePoints = 3;
			if (IniFile.waveStart == IniFile.waveEnd)
				IniFile.waveEnd += (IniFile.wavePoints >> 2);
			IniFile.waveStart %= IniFile.wavePoints;
			IniFile.waveEnd %= IniFile.wavePoints;
			EndDialog(hwndlg, 1);
			break;
		case IDC_DEFWAV:
			IniFile.wavePoints = IWAVPNTS;
			IniFile.waveStart  = IWAVSTRT;
			IniFile.waveEnd    = IWAVEND;
			IniFile.waveLobes  = IWAVS;
			wavinit(hwndlg);
			break;
		}
	}
	return false;
}

void wavfrm() {
	unsigned iPoint = 0, iNextVertex = 0, iLobe = 0;
	size_t   iVertex = 0, vertexCount = 0;
	unsigned end             = 0;
	unsigned count           = 0;
	fPOINT   currentPosition = {};
	double   horizontalRatio = 0.0;
	double   verticalRatio   = 0.0;
	fPOINT   selectedSize    = {};

	unmsg();
	if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_WAV), ThrEdWindow, (DLGPROC)wavprc)) {
		iVertex = FormVertexIndex;
		end     = IniFile.waveEnd + 1;
		durpoli(IniFile.wavePoints);
		mdufrm();
		FormVertexIndex = iVertex;
		std::vector<fPOINT> points(IniFile.wavePoints);
		iPoint  = 0;
		iVertex = IniFile.waveStart;
		while (iVertex != IniFile.waveEnd && iPoint < IniFile.wavePoints) {
			iNextVertex      = (iVertex + 1) % IniFile.wavePoints;
			points[iPoint].x = -CurrentFormVertices[iNextVertex].x + CurrentFormVertices[iVertex].x;
			points[iPoint].y = -CurrentFormVertices[iNextVertex].y + CurrentFormVertices[iVertex].y;
			iPoint++;
			iVertex = iNextVertex;
		}
		count             = iPoint;
		iVertex           = 0;
		currentPosition.x = currentPosition.y = 0;
		for (iLobe = 0; iLobe < IniFile.waveLobes; iLobe++) {
			if (iLobe & 1) {
				for (iPoint = 0; iPoint < count; iPoint++) {
					CurrentFormVertices[iVertex] = currentPosition;
					iVertex++;
					currentPosition.x += points[iPoint].x;
					currentPosition.y += points[iPoint].y;
				}
			}
			else {
				for (iPoint = count; iPoint != 0; iPoint--) {
					CurrentFormVertices[iVertex] = currentPosition;
					iVertex++;
					currentPosition.x += points[iPoint - 1].x;
					currentPosition.y += points[iPoint - 1].y;
				}
			}
		}
		CurrentFormVertices[iVertex] = currentPosition;
		vertexCount                  = iVertex + 1;
		const double rotationAngle   = -atan2(CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y,
                                            CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x);
		for (iVertex = 0; iVertex < vertexCount; iVertex++)
			rotflt(CurrentFormVertices[iVertex], rotationAngle, { 0.0, 0.0 });
		SelectedForm->type        = FRMLINE;
		SelectedForm->vertexCount = gsl::narrow<unsigned short>(vertexCount);
		FormVertexIndex += vertexCount;
		frmout(FormIndex);
		StateMap.reset(StateFlag::FORMSEL);
		selectedSize.x  = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		selectedSize.y  = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		horizontalRatio = UnzoomedRect.x / 4 / selectedSize.x;
		if (horizontalRatio > 1)
			horizontalRatio = 1;
		verticalRatio = UnzoomedRect.y / 4 / selectedSize.y;
		if (verticalRatio < horizontalRatio)
			horizontalRatio = verticalRatio;
		if (horizontalRatio < 1) {
			for (iVertex = 0; iVertex < vertexCount; iVertex++) {
				CurrentFormVertices[iVertex].x
				    = (CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x) * horizontalRatio + CurrentFormVertices[0].x;
				CurrentFormVertices[iVertex].y
				    = (CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y) * horizontalRatio + CurrentFormVertices[0].y;
			}
		}
		frmout(FormIndex);
		for (iVertex = 0; iVertex < vertexCount; iVertex++) {
			CurrentFormVertices[iVertex].x -= SelectedForm->rectangle.left;
			CurrentFormVertices[iVertex].y -= SelectedForm->rectangle.bottom;
		}
		FormMoveDelta.x = FormMoveDelta.y = 0;
		NewFormVertexCount                = vertexCount + 1;
		setmfrm();
		mdufrm();
	}
}
