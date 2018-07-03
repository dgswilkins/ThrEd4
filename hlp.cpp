#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <CppCoreCheck\warnings.h>
#include <htmlhelp.h>
#include <locale.h>
#include <stdio.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <fmt/format.h>
#pragma warning(pop)

#include "resource.h"
#include "thred.h"

extern void ispcdclp();
extern void movStch();
extern void numWnd();
extern void okcan();
extern void rstAll();
extern void shoMsg(const std::wstring& message);

extern unsigned               ButtonHeight;
extern unsigned               ButtonWidthX3;
extern std::vector<HWND>*     ButtonWin;
extern unsigned               ClosestFormToCursor;
extern DRAWITEMSTRUCT*        DrawItem;
extern unsigned               FormIndex;
extern FRMHED                 FormList[MAXFORMS];
extern wchar_t                HomeDirectory[_MAX_PATH];
extern INIFILE                IniFile;
extern HWND                   MainStitchWin;
extern MSG                    Msg;
extern wchar_t                MsgBuffer[MSGSIZ];
extern fPOINT                 OSequence[OSEQLEN];
extern PCSHEADER              PCSHeader;
extern long                   PreferenceWindowWidth;
extern RECT                   scRct;
extern FRMHED*                SelectedForm;
extern std::vector<unsigned>* SelectedFormList;
extern EnumMap<StateFlag>     StateMap;
extern HDC                    StitchWindowMemDC;
extern HINSTANCE              ThrEdInstance;
extern HWND                   ThrEdWindow;
extern wchar_t                ThrName[_MAX_PATH];
extern POINT                  UnzoomedRect;
extern wchar_t                WorkingFileName[_MAX_PATH];

HWND HelpWindow;    // help window
HWND MsgWindow = 0; // message window

unsigned short LoadStringList[] = {
	// strings to load into memory at init time
	IDS_PIKOL,    IDS_UPON,      IDS_UPOF,   IDS_AUXTXT,  IDS_HUP0,    IDS_HUP1,    IDS_HUP2,    IDS_HUP3,   IDS_HUP4,
	IDS_TRC0,     IDS_TRC1S,     IDS_TRC2,   IDS_TRC3,    IDS_TRC4,    IDS_TRC1H,   IDS_NUMPNT,  IDS_NUMFRM, IDS_NUMSCH,
	IDS_NUMFORM,  IDS_NUMSEL,    IDS_BOXSEL, IDS_CLOS,    IDS_TOT,     IDS_LAYR,    IDS_OVRLOK,  IDS_OVRIT,  IDS_THRED,
	IDS_NUFIL,    IDS_EMB,       IDS_ON,     IDS_OFF,     IDS_FMEN,    IDS_TXT0,    IDS_TXT1,    IDS_TXT2,   IDS_TXT3,
	IDS_TXT4,     IDS_TXT5,      IDS_TXT6,   IDS_TXT7,    IDS_TXT8,    IDS_TXT9,    IDS_TXT10,   IDS_TXT11,  IDS_TXT12,
	IDS_TXT13,    IDS_TXT14,     IDS_TXT15,  IDS_TXT16,   IDS_TXT17,   IDS_TXT18,   IDS_TXT19,   IDS_TXT20,  IDS_TXT21,
	IDS_TXT22,    IDS_TXT23,     IDS_FIL0,   IDS_FIL1,    IDS_FIL2,    IDS_FIL3,    IDS_FIL4,    IDS_FIL5,   IDS_FIL6,
	IDS_FIL7,     IDS_FIL8,      IDS_FIL9,   IDS_FIL10,   IDS_FIL11,   IDS_FIL12,   IDS_FIL13,   IDS_EDG0,   IDS_EDG1,
	IDS_EDG2,     IDS_EDG3,      IDS_EDG4,   IDS_EDG5,    IDS_EDG6,    IDS_EDG7,    IDS_EDG8,    IDS_EDG9,   IDS_EDG10,
	IDS_EDG11,    IDS_EDG12,     IDS_PRF0,   IDS_PRF1,    IDS_PRF2,    IDS_PRF3,    IDS_PRF4,    IDS_PRF5,   IDS_PRF6,
	IDS_PRF7,     IDS_PRF8,      IDS_PRF9,   IDS_PRF10,   IDS_PRF11,   IDS_PRF12,   IDS_PRF13,   IDS_PRF14,  IDS_PRF15,
	IDS_PRF16,    IDS_PRF17,     IDS_PRF18,  IDS_PRF19,   IDS_PRF20,   IDS_PRF21,   IDS_PRF22,   IDS_PRF23,  IDS_PRF24,
	IDS_PRF25,    IDS_PRF26,     IDS_PRF27,  IDS_FRMPLUS, IDS_FRMINUS, IDS_OKENT,   IDS_CANCEL,  IDS_FREH,   IDS_BLUNT,
	IDS_TAPR,     IDS_PNTD,      IDS_SQR,    IDS_DELST2,  IDS_THRDBY,  IDS_STCHOUT, IDS_STCHS,   IDS_FORMS,  IDS_HUPWID,
	IDS_CREATBY,  IDS_CUSTHUP,   IDS_STCHP,  IDS_FRMP,    IDS_ENTROT,  IDS_NUDG,    IDS_ALLX,    IDS_FTHCOL, IDS_FTHTYP,
	IDS_FTH0,     IDS_FTH1,      IDS_FTH2,   IDS_FTH3,    IDS_FTH4,    IDS_FTH5,    IDS_FTHBLND, IDS_FTHUP,  IDS_FTHBOTH,
	IDS_FTHUPCNT, IDS_FTHDWNCNT, IDS_FTHSIZ, IDS_FTHNUM,  IDS_FTHFLR,  IDS_FSTRT,   IDS_FEND,    IDS_WALK,   IDS_UWLKIND,
	IDS_UND,      IDS_ULEN,      IDS_FUANG,  IDS_FUSPAC,  IDS_CWLK,    IDS_UNDCOL,  IDS_FRMBOX,  IDS_TXOF,
};

std::vector<std::wstring>* StringTable;

inline void loadString(std::wstring& sDest, unsigned stringID) {
	WCHAR  sBuf[HBUFSIZ] = { 0 };
	LPWSTR psBuf         = sBuf;
	sDest.clear();
	if (auto len = ::LoadStringW(ThrEdInstance, stringID, psBuf, 0)) {
		sDest.resize(len);
		auto _ = std::copy(sBuf, sBuf + len, sDest.begin());
	}
}

void adbad(std::wstring& repairMessage, unsigned code, unsigned count) {
	std::wstring fmtStr;

	loadString(fmtStr, code);
	repairMessage += fmtStr;
	loadString(fmtStr, IDS_NOTREP);
	//repairMessage += fmt::format(fmtStr, count);
}

void hsizmsg() {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_HSIZ);
	shoMsg(fmt::format(fmtStr, (UnzoomedRect.x / PFGRAN), (UnzoomedRect.y / PFGRAN)));
}

void msgflt(unsigned messageId, float value) {
	std::wstring fmtStr;

	loadString(fmtStr, messageId);
	shoMsg(fmt::format(fmtStr, value));
	StateMap.set(StateFlag::NUMIN);
	numWnd();
}

void tsizmsg(wchar_t* threadSizeText, double threadSize) {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_SIZ);
	//shoMsg(fmt::format(fmtStr, threadSizeText, threadSize));
	StateMap.set(StateFlag::NUMIN);
	numWnd();
}

void bfilmsg() {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_BADFIL);
	//shoMsg(fmt::format(fmtStr, WorkingFileName));
}

void filnopn(unsigned code, const wchar_t* fileName) {
	std::wstring fmtStr;

	loadString(fmtStr, code);
	//shoMsg(fmt::format(fmtStr, fileName));
}

void crmsg(const wchar_t* fileName) {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_CREAT);
	//shoMsg(fmt::format(fmtStr, fileName));
}

void butxt(unsigned iButton, const std::wstring& buttonText) {
	if (StateMap.test(StateFlag::WASTRAC) && iButton > HNUM) {
		if (iButton == 5) {
			if (StateMap.test(StateFlag::HIDMAP))
				SetWindowText((*ButtonWin)[iButton], (*StringTable)[STR_TRC1H].c_str());
			else
				SetWindowText((*ButtonWin)[iButton], (*StringTable)[STR_TRC1S].c_str());
		}
		else
			SetWindowText((*ButtonWin)[iButton], (*StringTable)[iButton - 4 + STR_TRC0].c_str());
	}
	else
		SetWindowText((*ButtonWin)[iButton], buttonText.c_str());
}

void ritnum(unsigned code, unsigned value) {
	butxt(HNUM, fmt::format((*StringTable)[code], value));
}

void msgstr(unsigned code) noexcept {
	LoadString(ThrEdInstance, code, MsgBuffer, MSGSIZ);
}

void lodstr() {
	for (auto iString = 0; iString < STR_LEN; iString++) {
		loadString((*StringTable)[iString], LoadStringList[iString]);
	}
}

void shoMsg(const std::wstring& message) {
	if (message.size()) {
		SIZE                      textSize = {}, messageSize = {};
		unsigned                  iString = 0, index = 0, previousStringLength = 0;
		long                      offset = 0;
		std::vector<std::wstring> strings;

		iString            = 0;
		const auto sizeLim = message.size();
		while (iString < sizeLim) {
			if (message[iString] == 10) {
				strings.push_back(message.substr(previousStringLength, (iString++ - previousStringLength)));
				previousStringLength = iString;
			}
			else
				iString++;
		}
		strings.push_back(message.substr(previousStringLength, (iString++ - previousStringLength)));
		textSize.cx = textSize.cy = messageSize.cy = messageSize.cx = 0;
		for (index = 0; index < strings.size(); index++) {
			GetTextExtentPoint32(StitchWindowMemDC, strings[index].c_str(), strings[index].size(), &textSize);
			if (textSize.cx > messageSize.cx)
				messageSize.cx = textSize.cx;
			if (textSize.cy > messageSize.cy)
				messageSize.cy = textSize.cy;
		}
		messageSize.cy *= strings.size();
		if (StateMap.testAndReset(StateFlag::MSGOF))
			offset = PreferenceWindowWidth + 6;
		else
			offset = 3;
		MsgWindow = CreateWindow(L"STATIC",
		                         message.c_str(),
		                         SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                         offset,
		                         3,
		                         messageSize.cx + 20,
		                         messageSize.cy + 6,
		                         MainStitchWin,
		                         NULL,
		                         ThrEdInstance,
		                         NULL);
	}
}

void tabmsg(unsigned code) {
	std::wstring message;
	loadString(message, code);
	shoMsg(message);
}

void riter() {
	tabmsg(IDS_RITER);
}

void pntmsg(unsigned msgID) {
	std::wstring fmtStr, message;

	loadString(fmtStr, IDS_PNT);
	loadString(message, msgID);
	shoMsg(fmt::format(fmtStr, message));
}

void shoseln(unsigned code0, unsigned code1) {
	std::wstring fmtStr, msg0, msg1;

	loadString(fmtStr, IDS_SHOSEL);
	loadString(msg0, code0);
	loadString(msg1, code1);
	shoMsg(fmt::format(fmtStr, msg0, msg1));
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

void help() {
	std::wstring helpFileName;

	loadString(helpFileName, IDS_HELPFN);
	HelpWindow = HtmlHelp(ThrEdWindow, fmt::format(L"{}{}", HomeDirectory, helpFileName).c_str(), HH_DISPLAY_TOPIC, 0);
	if (!HelpWindow)
		tabmsg(IDS_NOHLP);
}

void sdmsg() {
	std::wstring fmtStr;

	loadString(fmtStr, IDS_SAVDISC);
	shoMsg(fmt::format(fmtStr, ThrName));
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
