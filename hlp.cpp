#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "lang.h"
#include "resource.h"
#include "thred.h"
#include <htmlhelp.h>
#include <locale.h>

extern void				ispcdclp();
extern void				movStch();
extern	void			numWnd();
extern void				okcan();
extern void				rstAll();
extern void				shoMsg(TCHAR* string);

extern	unsigned		ButtonHeight;
extern	unsigned		ButtonWidthX3;
extern	HWND			ButtonWin[9];
extern	unsigned		ClosestFormToCursor;
extern	DRAWITEMSTRUCT*	DrawItem;
extern	unsigned		FormIndex;
extern	FRMHED			FormList[MAXFORMS];
extern	TCHAR			HomeDirectory[_MAX_PATH];
extern	INIFILE			IniFile;
extern	HWND			MainStitchWin;
extern	MSG				Msg;
extern	TCHAR			MsgBuffer[MSGSIZ];
extern	fPOINT			OSequence[OSEQLEN];
extern	PCSHEADER		PCSHeader;
extern	long			PreferenceWindowWidth;
extern	RECT			scRct;
extern	FRMHED*			SelectedForm;
extern	unsigned		SelectedFormCount;
extern	EnumMap<StateFlag>	StateMap; 
extern	HDC				StitchWindowMemDC;
extern	HINSTANCE		ThrEdInstance;
extern	HWND			ThrEdWindow;
extern	TCHAR			ThrName[_MAX_PATH];
extern	POINT			UnzoomedRect;
extern	TCHAR			WorkingFileName[_MAX_PATH];

HANDLE					HelpFile;					//handle to the help file
TCHAR					HelpFileName[_MAX_PATH];	//help file name
unsigned				HelpFileLength;				//help file length
HWND					HelpWindow;				//help window
HWND					MsgWindow = 0;				//message window

unsigned short LoadStringList[] = {	//strings to load into memory at init time
	IDS_PIKOL,
	IDS_UPON,
	IDS_UPOF,
	IDS_AUXTXT,
	IDS_HUP0,
	IDS_HUP1,
	IDS_HUP2,
	IDS_HUP3,
	IDS_HUP4,
	IDS_TRC0,
	IDS_TRC1S,
	IDS_TRC2,
	IDS_TRC3,
	IDS_TRC4,
	IDS_TRC1H,
	IDS_NUMPNT,
	IDS_NUMFRM,
	IDS_NUMSCH,
	IDS_NUMFORM,
	IDS_NUMSEL,
	IDS_BOXSEL,
	IDS_CLOS,
	IDS_TOT,
	IDS_LAYR,
	IDS_OVRLOK,
	IDS_OVRIT,
	IDS_THRED,
	IDS_NUFIL,
	IDS_EMB,
	IDS_ON,
	IDS_OFF,
	IDS_FMEN,
	IDS_TXT0,
	IDS_TXT1,
	IDS_TXT2,
	IDS_TXT3,
	IDS_TXT4,
	IDS_TXT5,
	IDS_TXT6,
	IDS_TXT7,
	IDS_TXT8,
	IDS_TXT9,
	IDS_TXT10,
	IDS_TXT11,
	IDS_TXT12,
	IDS_TXT13,
	IDS_TXT14,
	IDS_TXT15,
	IDS_TXT16,
	IDS_TXT17,
	IDS_TXT18,
	IDS_TXT19,
	IDS_TXT20,
	IDS_TXT21,
	IDS_TXT22,
	IDS_TXT23,
	IDS_FIL0,
	IDS_FIL1,
	IDS_FIL2,
	IDS_FIL3,
	IDS_FIL4,
	IDS_FIL5,
	IDS_FIL6,
	IDS_FIL7,
	IDS_FIL8,
	IDS_FIL9,
	IDS_FIL10,
	IDS_FIL11,
	IDS_FIL12,
	IDS_FIL13,
	IDS_EDG0,
	IDS_EDG1,
	IDS_EDG2,
	IDS_EDG3,
	IDS_EDG4,
	IDS_EDG5,
	IDS_EDG6,
	IDS_EDG7,
	IDS_EDG8,
	IDS_EDG9,
	IDS_EDG10,
	IDS_EDG11,
	IDS_EDG12,
	IDS_PRF0,
	IDS_PRF1,
	IDS_PRF2,
	IDS_PRF3,
	IDS_PRF4,
	IDS_PRF5,
	IDS_PRF6,
	IDS_PRF7,
	IDS_PRF8,
	IDS_PRF9,
	IDS_PRF10,
	IDS_PRF11,
	IDS_PRF12,
	IDS_PRF13,
	IDS_PRF14,
	IDS_PRF15,
	IDS_PRF16,
	IDS_PRF17,
	IDS_PRF18,
	IDS_PRF19,
	IDS_PRF20,
	IDS_PRF21,
	IDS_PRF22,
	IDS_PRF23,
	IDS_PRF24,
	IDS_PRF25,
	IDS_PRF26,
	IDS_PRF27,
	IDS_FRMPLUS,
	IDS_FRMINUS,
	IDS_OKENT,
	IDS_CANCEL,
	IDS_FREH,
	IDS_BLUNT,
	IDS_TAPR,
	IDS_PNTD,
	IDS_SQR,
	IDS_DELST2,
	IDS_THRDBY,
	IDS_STCHOUT,
	IDS_STCHS,
	IDS_FORMS,
	IDS_HUPWID,
	IDS_CREATBY,
	IDS_CUSTHUP,
	IDS_STCHP,
	IDS_FRMP,
	IDS_ENTROT,
	IDS_NUDG,
	IDS_ALLX,
	IDS_FTHCOL,
	IDS_FTHTYP,
	IDS_FTH0,
	IDS_FTH1,
	IDS_FTH2,
	IDS_FTH3,
	IDS_FTH4,
	IDS_FTH5,
	IDS_FTHBLND,
	IDS_FTHUP,
	IDS_FTHBOTH,
	IDS_FTHUPCNT,
	IDS_FTHDWNCNT,
	IDS_FTHSIZ,
	IDS_FTHNUM,
	IDS_FTHFLR,
	IDS_FSTRT,
	IDS_FEND,
	IDS_WALK,
	IDS_UWLKIND,
	IDS_UND,
	IDS_ULEN,
	IDS_FUANG,
	IDS_FUSPAC,
	IDS_CWLK,
	IDS_UNDCOL,
	IDS_FRMBOX,
	IDS_TXOF,
};

TCHAR*	StringTable[256];		//memory string pointers
TCHAR*	StringData;				//string storage
TCHAR*	RepairString;			//Repair Type

TCHAR	HelpBuffer[HBUFSIZ];	//message formatting buffer

void adbad(unsigned code, unsigned count) noexcept {

	LoadString(ThrEdInstance, code, RepairString, sizeof(MsgBuffer) - strlen(RepairString));
	RepairString = &RepairString[strlen(RepairString)];
	LoadString(ThrEdInstance, IDS_NOTREP, HelpBuffer, sizeof(HelpBuffer));
	sprintf_s(RepairString, sizeof(MsgBuffer) - strlen(RepairString), HelpBuffer, count);
	RepairString = &RepairString[strlen(RepairString)];
}

void hsizmsg() {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_HSIZ, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buffer, UnzoomedRect.x / PFGRAN, UnzoomedRect.y / PFGRAN);
	shoMsg(HelpBuffer);
}

void msgflt(unsigned messageId, float value) {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, messageId, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buffer, value);
	shoMsg(HelpBuffer);
	StateMap.set(StateFlag::NUMIN);
	numWnd();
}

void tsizmsg(TCHAR* threadSizeText, double threadSize) {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_SIZ, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buffer, threadSizeText, threadSize);
	shoMsg(HelpBuffer);
	StateMap.set(StateFlag::NUMIN);
	numWnd();
}

void bfilmsg() {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_BADFIL, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buffer, WorkingFileName);
	shoMsg(HelpBuffer);
}

void filnopn(unsigned code, TCHAR* fileName) {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, code, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buffer, fileName);
	shoMsg(HelpBuffer);
}

void crmsg(TCHAR* fileName) {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_CREAT, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buffer, fileName);
	shoMsg(HelpBuffer);
}

void butxt(unsigned iButton, const TCHAR* buttonText) {

	if (StateMap.test(StateFlag::WASTRAC) && iButton > HNUM) {

		if (iButton == 5) {

			if (StateMap.test(StateFlag::HIDMAP))
				SetWindowText(ButtonWin[iButton], StringTable[STR_TRC1H]);
			else
				SetWindowText(ButtonWin[iButton], StringTable[STR_TRC1S]);
		}
		else
			SetWindowText(ButtonWin[iButton], StringTable[iButton - 4 + STR_TRC0]);
	}
	else
		SetWindowText(ButtonWin[iButton], buttonText);
}

void ritnum(unsigned code, unsigned value) {

	sprintf_s(HelpBuffer, sizeof(HelpBuffer), "%s %d", StringTable[code], value);
	butxt(HNUM, HelpBuffer);
}

void msgstr(unsigned code) noexcept {

	LoadString(ThrEdInstance, code, MsgBuffer, MSGSIZ);
}

void lodstr() {

	unsigned	iString = 0, iStringData = 0, iStringTable = 0, count = 0;
	// Over allocate storage to ensure no overflow
	TCHAR*		stringStorage = new TCHAR[65535];
	TCHAR*		strings = stringStorage;

	for (iString = 0; iString < STR_LEN; iString++) {

		count = LoadString(ThrEdInstance, LoadStringList[iString], strings, 1000) + 1;
		strings += count;
	}
	count = strings - stringStorage;
	// Now allocate based on actual count
	StringData = new TCHAR[count];
	MoveMemory(StringData, stringStorage, count);
	delete[] stringStorage;
	StringTable[0] = StringData;
	iStringTable = 1;
	for (iStringData = 0; iStringData <= count; iStringData++) {

		if (!StringData[iStringData]) {

			iStringData++;
			StringTable[iStringTable] = &StringData[iStringData];
			iStringTable++;
		}
	}
}

void shoMsg(TCHAR* string) {

	SIZE		textSize = {}, messageSize = {};
	TCHAR**		strings = nullptr;
	unsigned*	lengths = nullptr;
	unsigned	count = 0, iString = 0, index = 0, iLength = 0, previousStringLength = 0;
	long		offset = 0;

	while (string[iString]) {

		if (string[iString++] == 10)
			count++;
	}
	count++;
	strings = new TCHAR*[count + 1];
	lengths = new unsigned[count + 1];
	iString = 0;
	strings[0] = string;
	while (string[iString]) {

		if (string[iString] == 10) {

			lengths[iLength] = iString - previousStringLength;
			strings[++iLength] = &string[iString++];
			previousStringLength = iString;
		}
		else
			iString++;
	}
	lengths[iLength] = iString - previousStringLength;
	iLength++;
	textSize.cx = textSize.cy = messageSize.cy = messageSize.cx = 0;
	for (index = 0; index < iLength; index++) {

		GetTextExtentPoint32(StitchWindowMemDC, strings[index], lengths[index], &textSize);
		if (textSize.cx > messageSize.cx)
			messageSize.cx = textSize.cx;
		if (textSize.cy > messageSize.cy)
			messageSize.cy = textSize.cy;
	}
	messageSize.cy *= count;
	if (StateMap.testAndReset(StateFlag::MSGOF))
		offset = PreferenceWindowWidth + 6;
	else
		offset = 3;
	MsgWindow = CreateWindow(
		"STATIC",
		string,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		offset,
		3,
		messageSize.cx + 20,
		messageSize.cy + 6,
		MainStitchWin,
		NULL,
		ThrEdInstance,
		NULL);
	delete[] strings;
	delete[] lengths;
}

void tabmsg(unsigned code) {

	LoadString(ThrEdInstance, code, HelpBuffer, HBUFSIZ);
	shoMsg(HelpBuffer);
}

void riter() {

	tabmsg(IDS_RITER);
}

void pntmsg(unsigned count) {

	TCHAR	temp[HBUFSIZ];
	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_PNT, temp, HBUFSIZ);
	LoadString(ThrEdInstance, count, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), temp, buffer);
	shoMsg(HelpBuffer);
}

void shoseln(unsigned code0, unsigned code1) {

	TCHAR	temp[HBUFSIZ];
	TCHAR	buffer0[HBUFSIZ];
	TCHAR	buffer1[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_SHOSEL, temp, HBUFSIZ);
	LoadString(ThrEdInstance, code0, buffer0, HBUFSIZ);
	LoadString(ThrEdInstance, code1, buffer1, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), temp, buffer0, buffer1);
	shoMsg(HelpBuffer);
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

	if (SelectedFormCount)
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
	strcpy_s(HelpFileName, HomeDirectory);
#if LANG==ENG || LANG==HNG
	strcat_s(HelpFileName, "thred.chm");
#endif
#if LANG==GRM
	strcat_s(HelpFileName, "aladin.chm");
#endif
	HelpWindow = HtmlHelp(ThrEdWindow, HelpFileName, HH_DISPLAY_TOPIC, 0);
	if (!HelpWindow)
		tabmsg(IDS_NOHLP);
}

void sdmsg() {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_SAVDISC, buffer, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buffer, ThrName);
	shoMsg(HelpBuffer);
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
	TCHAR*	pchr;

	pchr = MsgBuffer;
	if (code&BADFLT) {
		LoadString(ThrEdInstance, IDS_BADFLT, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	if (code&BADCLP) {
		LoadString(ThrEdInstance, IDS_BADCLP, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	if (code&BADSAT) {
		LoadString(ThrEdInstance, IDS_BADSAT, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	if (code&BADTX) {
		LoadString(ThrEdInstance, IDS_BADTX, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	pchr--;
	*pchr = 0;
	shoMsg(MsgBuffer);

}
