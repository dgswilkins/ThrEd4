#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "lang.h"
#include "resource.h"
#include "thred.h"
#include <htmlhelp.h>
#include <locale.h>

extern unsigned			chkMap(unsigned bPnt);
extern void				ispcdclp();
extern void				movStch();
extern void				okcan();
extern void				rstAll();
extern void				shoMsg(TCHAR* str);

extern	BSEQPNT			BSequence[BSEQLEN];
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
extern	void			numWnd();
extern	fPOINT			OSequence[OSEQLEN];
extern	PCSHEADER		PCSHeader;
extern	long			PreferenceWindowWidth;
extern	unsigned		rstMap(unsigned bPnt);
extern	RECT			scRct;
extern	FRMHED*			SelectedForm;
extern	unsigned		SelectedFormCount;
extern	unsigned		setMap(unsigned bPnt);
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
TCHAR*	StringData;			//string storage

TCHAR	HelpBuffer[HBUFSIZ];	//message formatting buffer

void adbad(unsigned cod, unsigned cnt)
{
	// ToDo - not sure that StringData has been initialized when this is called
	//what does this function do?
	LoadString(ThrEdInstance, cod, StringData, HBUFSIZ);
	StringData = &StringData[strlen(StringData)];
	LoadString(ThrEdInstance, IDS_NOTREP, HelpBuffer, HBUFSIZ);
	sprintf_s(StringData, strlen(StringData), HelpBuffer, cnt);
	StringData = &StringData[strlen(StringData)];
}

void hsizmsg()
{
	TCHAR buf[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_HSIZ, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buf, UnzoomedRect.x / PFGRAN, UnzoomedRect.y / PFGRAN);
	shoMsg(HelpBuffer);
}

void msgflt(unsigned msgid, float par)
{
	TCHAR	buf[HBUFSIZ];

	LoadString(ThrEdInstance, msgid, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buf, par);
	shoMsg(HelpBuffer);
	setMap(NUMIN);
	numWnd();
}

void tsizmsg(TCHAR* sizstr, double pd_Size) {

	TCHAR	buf[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_SIZ, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buf, sizstr, pd_Size);
	shoMsg(HelpBuffer);
	setMap(NUMIN);
	numWnd();
}

void bfilmsg() {

	TCHAR	buf[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_BADFIL, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buf, WorkingFileName);
	shoMsg(HelpBuffer);
}

void filnopn(unsigned cod, TCHAR* nam) {

	TCHAR	buf[HBUFSIZ];

	LoadString(ThrEdInstance, cod, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buf, nam);
	shoMsg(HelpBuffer);
}

void crmsg(TCHAR* nam) {

	TCHAR	buf[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_CREAT, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buf, nam);
	shoMsg(HelpBuffer);
}

void butxt(unsigned ind, TCHAR* str) {

	if (chkMap(WASTRAC) && ind > HNUM) {

		if (ind == 5) {

			if (chkMap(HIDMAP))
				SetWindowText(ButtonWin[ind], StringTable[STR_TRC1H]);
			else
				SetWindowText(ButtonWin[ind], StringTable[STR_TRC1S]);
		}
		else
			SetWindowText(ButtonWin[ind], StringTable[ind - 4 + STR_TRC0]);
	}
	else
		SetWindowText(ButtonWin[ind], str);
}

void ritnum(unsigned cod, unsigned num) {

	sprintf_s(HelpBuffer, sizeof(HelpBuffer), "%s %d", StringTable[cod], num);
	butxt(HNUM, HelpBuffer);
}

void msgstr(unsigned cod) {

	LoadString(ThrEdInstance, cod, MsgBuffer, MSGSIZ);
}

void lodstr() {

	unsigned	ind, ine, cnt;
	TCHAR*		pchr;

	StringTable[0] = pchr = (TCHAR*)BSequence;
	for (ind = 0; ind < STR_LEN; ind++) {

		cnt = LoadString(ThrEdInstance, LoadStringList[ind], pchr, 1000) + 1;
		pchr += cnt;
	}
	cnt = pchr - StringTable[0];
	StringData = new TCHAR[cnt];
	MoveMemory(StringData, StringTable[0], cnt);
	StringTable[0] = StringData;
	ine = 1;
	for (ind = 0; ind <= cnt; ind++) {

		if (!StringData[ind]) {

			ind++;
			StringTable[ine] = &StringData[ind];
			ine++;
		}
	}
}

void shoMsg(TCHAR* str) {

	SIZE		tsiz, msgsiz;
	TCHAR**		strs;
	unsigned*	l_lens;
	unsigned	cnt = 0, ind = 0, ine = 0, bak = 0;
	long		off;

	while (str[ind]) {

		if (str[ind++] == 10)
			cnt++;
	}
	cnt++;
	strs = new TCHAR*[cnt + 1];
	l_lens = new unsigned[cnt + 1];
	ind = 0;
	strs[0] = str;
	while (str[ind]) {

		if (str[ind] == 10) {

			l_lens[ine] = ind - bak;
			strs[++ine] = &str[ind++];
			bak = ind;
		}
		else
			ind++;
	}
	l_lens[ine] = ind - bak;
	ine++;
	tsiz.cx = tsiz.cy = msgsiz.cy = msgsiz.cx = 0;
	for (ind = 0; ind < ine; ind++) {

		GetTextExtentPoint32(StitchWindowMemDC, strs[ind], l_lens[ind], &tsiz);
		if (tsiz.cx > msgsiz.cx)
			msgsiz.cx = tsiz.cx;
		if (tsiz.cy > msgsiz.cy)
			msgsiz.cy = tsiz.cy;
	}
	msgsiz.cy *= cnt;
	if (rstMap(MSGOF))
		off = PreferenceWindowWidth + 6;
	else
		off = 3;
	MsgWindow = CreateWindow(
		"STATIC",
		str,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		off,
		3,
		msgsiz.cx + 20,
		msgsiz.cy + 6,
		MainStitchWin,
		NULL,
		ThrEdInstance,
		NULL);
	delete[] strs;
	delete[] l_lens;
}

void tabmsg(unsigned cod) {

	LoadString(ThrEdInstance, cod, HelpBuffer, HBUFSIZ);
	shoMsg(HelpBuffer);
}

void riter() {

	tabmsg(IDS_RITER);
}

void pntmsg(unsigned cod) {

	TCHAR	temp[HBUFSIZ];
	TCHAR	buf[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_PNT, temp, HBUFSIZ);
	LoadString(ThrEdInstance, cod, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), temp, buf);
	shoMsg(HelpBuffer);
}

void shosel(TCHAR* seltxt, TCHAR* selfun) {

	sprintf_s(HelpBuffer, sizeof(HelpBuffer), "Select %s\n to use %s", seltxt, selfun);
	shoMsg(HelpBuffer);
}

void shoseln(unsigned cod0, unsigned cod1) {

	TCHAR	temp[HBUFSIZ];
	TCHAR	buf0[HBUFSIZ];
	TCHAR	buf1[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_SHOSEL, temp, HBUFSIZ);
	LoadString(ThrEdInstance, cod0, buf0, HBUFSIZ);
	LoadString(ThrEdInstance, cod1, buf1, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), temp, buf0, buf1);
	shoMsg(HelpBuffer);
}

BOOL clpmsgs(unsigned cod) {

	ispcdclp();
	if ((cod == FML_CLP || cod == FMM_CLP || cod == FML_PIC) && !chkMap(WASPCDCLP)) {

		tabmsg(IDS_CLPS);
		return 1;
	}
	return 0;
}

void frm1pnt() {

	if (FormIndex == 1) {

		setMap(FORMSEL);
		ClosestFormToCursor = 0;
	}
}

BOOL filmsgs(unsigned cod) {

	if (SelectedFormCount)
		return clpmsgs(cod);
	if (FormIndex) {

		frm1pnt();
		if (chkMap(FORMSEL)) {

			SelectedForm = &FormList[ClosestFormToCursor];
			if (SelectedForm->sides == 2) {

				if (cod < FML_LIN) {

					tabmsg(IDS_FRM3X);
					return 1;
				}
				else {

					if (cod == FML_PRPS) {

						tabmsg(IDS_ANGS);
						return 1;
					}
				}
			}
			return clpmsgs(cod);
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
#if LANG==ENG||LANG==HNG
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

	TCHAR	buf[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_SAVDISC, buf, HBUFSIZ);
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), buf, ThrName);
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

void datmsg(unsigned cod)
{
	TCHAR*	pchr;

	pchr = MsgBuffer;
	if (cod&BADFLT)
	{
		LoadString(ThrEdInstance, IDS_BADFLT, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	if (cod&BADCLP)
	{
		LoadString(ThrEdInstance, IDS_BADCLP, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	if (cod&BADSAT)
	{
		LoadString(ThrEdInstance, IDS_BADSAT, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	if (cod&BADTX)
	{
		LoadString(ThrEdInstance, IDS_BADTX, pchr, HBUFSIZ);
		pchr = &pchr[strlen(pchr)];
	}
	pchr--;
	*pchr = 0;
	shoMsg(MsgBuffer);

}
