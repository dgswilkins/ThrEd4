#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "form.h"
#include "thred.h"
#include "displayText.h"

namespace di = displayText::internal;

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

inline void displayText::loadString(std::wstring& sDest, unsigned stringID) {
	auto pBuf = static_cast<wchar_t*>(nullptr);
	sDest.clear();
	GSL_SUPPRESS(26490) {
		if (auto len = LoadString(ThrEdInstance, stringID, reinterpret_cast<LPWSTR>(&pBuf), 0)) {
			sDest.resize(len);
			std::copy(pBuf, pBuf + len, sDest.begin());
		}
	}
}

void displayText::shoMsg(const std::wstring& message) {
	if (!message.empty()) {
		auto strings = std::vector<std::wstring>{};

		auto       iString              = 0u;
		auto       previousStringLength = 0u;
		const auto sizeLim              = message.size();
		while (iString < sizeLim) {
			if (message[iString] == 10) {
				strings.push_back(message.substr(previousStringLength, (iString++ - previousStringLength)));
				previousStringLength = iString;
			}
			else {
				iString++;
			}
		}
		strings.push_back(message.substr(previousStringLength, (iString++ - previousStringLength)));
		auto textSize    = SIZE{};
		auto messageSize = SIZE{};
		for (auto& string : strings) {
			GetTextExtentPoint32Int(GetDC(ThrEdWindow), string.c_str(), gsl::narrow<unsigned int>(string.size()), &textSize);
			if (textSize.cx > messageSize.cx) {
				messageSize.cx = textSize.cx;
			}
			if (textSize.cy > messageSize.cy) {
				messageSize.cy = textSize.cy;
			}
		}
		messageSize.cy *= gsl::narrow<LONG>(strings.size());
		auto mainRect = RECT{};
		GetWindowRect(MainStitchWin, &mainRect);
		auto xOffset = mainRect.left;
		GetWindowRect(ThrEdWindow, &mainRect);
		xOffset -= mainRect.left;
		MsgWindow = CreateWindow(L"STATIC",
		                         message.c_str(),
		                         SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                         xOffset,
		                         3,
		                         messageSize.cx + 20,
		                         messageSize.cy + 6,
		                         ThrEdWindow,
		                         nullptr,
		                         ThrEdInstance,
		                         nullptr);
		//		displayText::updateWinFont(MainStitchWin);
	}
}

void displayText::tabmsg(unsigned code) {
	auto message = std::wstring{};
	displayText::loadString(message, code);
	displayText::shoMsg(message);
}

void displayText::lodstr() {
	for (auto iString = 0; iString < STR_LEN; iString++) {
		displayText::loadString((*StringTable)[iString], LoadStringList[iString]);
	}
}

void displayText::hsizmsg() {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, IDS_HSIZ);
	displayText::shoMsg(fmt::format(fmtStr, (UnzoomedRect.x / PFGRAN), (UnzoomedRect.y / PFGRAN)));
}

void displayText::numWnd() noexcept {
	auto messageRect = RECT{};

	GetClientRect(MsgWindow, &messageRect);
	auto wRect = RECT{};
	GetWindowRect(MainStitchWin, &wRect);
	auto xOffset = wRect.left;
	GetWindowRect(ThrEdWindow, &wRect);
	xOffset -= wRect.left;
	GeneralNumberInputBox = CreateWindow(L"STATIC",
	                                     nullptr,
	                                     SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                     xOffset + 5,
	                                     messageRect.bottom + 15,
	                                     ButtonWidthX3,
	                                     ButtonHeight,
	                                     ThrEdWindow,
	                                     nullptr,
	                                     ThrEdInstance,
	                                     nullptr);
	MsgIndex              = 0;
	MsgBuffer[0]          = 0;
}

void displayText::msgflt(unsigned messageId, float value) {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, messageId);
	displayText::shoMsg(fmt::format(fmtStr, value));
	StateMap.set(StateFlag::NUMIN);
	displayText::numWnd();
}

void displayText::tsizmsg(const wchar_t* threadSizeText, double threadSize) {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, IDS_SIZ);
	displayText::shoMsg(fmt::format(fmtStr, threadSizeText, threadSize));
	StateMap.set(StateFlag::NUMIN);
	displayText::numWnd();
}

void displayText::bfilmsg() {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, IDS_BADFIL);
	displayText::shoMsg(fmt::format(fmtStr, WorkingFileName->wstring()));
}

void displayText::filnopn(unsigned code, const fs::path& fileName) {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, code);
	displayText::shoMsg(fmt::format(fmtStr, fileName.wstring()));
}

void displayText::crmsg(const fs::path& fileName) {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, IDS_CREAT);
	displayText::shoMsg(fmt::format(fmtStr, fileName.wstring()));
}

void displayText::butxt(unsigned iButton, const std::wstring& buttonText) {
	if (StateMap.test(StateFlag::WASTRAC) && iButton > HNUM) {
		if (iButton == 5) {
			if (StateMap.test(StateFlag::HIDMAP)) {
				SetWindowText((*ButtonWin)[iButton], (*StringTable)[STR_TRC1H].c_str());
			}
			else {
				SetWindowText((*ButtonWin)[iButton], (*StringTable)[STR_TRC1S].c_str());
			}
		}
		else {
			SetWindowText((*ButtonWin)[iButton], (*StringTable)[gsl::narrow<size_t>(iButton) - 4u + STR_TRC0].c_str());
		}
	}
	else {
		SetWindowText((*ButtonWin)[iButton], buttonText.c_str());
	}
}

void displayText::clrhbut(unsigned startButton) {
	for (auto iButton = startButton; iButton < 9u; iButton++) {
		SetWindowText((*ButtonWin)[iButton], L"");
	}
}

void displayText::ritnum(unsigned code, unsigned int value) {
	displayText::butxt(HNUM, fmt::format((*StringTable)[code], value));
}

void displayText::msgstr(unsigned code) noexcept {
	LoadString(ThrEdInstance, code, &MsgBuffer[0], MSGSIZ);
}

void displayText::riter() {
	displayText::tabmsg(IDS_RITER);
}

void displayText::pntmsg(unsigned msgID) {
	auto fmtStr  = std::wstring{};
	auto message = std::wstring{};

	displayText::loadString(fmtStr, IDS_PNT);
	displayText::loadString(message, msgID);
	displayText::shoMsg(fmt::format(fmtStr, message));
}

void displayText::shoseln(unsigned code0, unsigned code1) {
	auto fmtStr = std::wstring{};
	auto msg0   = std::wstring{};
	auto msg1   = std::wstring{};

	displayText::loadString(fmtStr, IDS_SHOSEL);
	displayText::loadString(msg0, code0);
	displayText::loadString(msg1, code1);
	displayText::shoMsg(fmt::format(fmtStr, msg0, msg1));
}

bool displayText::clpmsgs(unsigned code) {
	form::ispcdclp();
	if ((code == FML_CLP || code == FMM_CLP || code == FML_PIC) && !StateMap.test(StateFlag::WASPCDCLP)) {
		displayText::tabmsg(IDS_CLPS);
		return true;
	}
	return false;
}

void displayText::frm1pnt() {
	if (FormList->size() == 1) {
		StateMap.set(StateFlag::FORMSEL);
		ClosestFormToCursor = 0;
	}
}

bool displayText::filmsgs(unsigned code) {
	if (!SelectedFormList->empty()) {
		return displayText::clpmsgs(code);
	}
	if (!FormList->empty()) {
		displayText::frm1pnt();
		if (StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm = &((*FormList)[ClosestFormToCursor]);
			if (SelectedForm->vertexCount == 2) {
				if (code < FML_LIN) {
					displayText::tabmsg(IDS_FRM3X);
					return true;
				}

				if (code == FML_PRPS) {
					displayText::tabmsg(IDS_ANGS);
					return true;
				}
			}
			return displayText::clpmsgs(code);
		}

		displayText::tabmsg(IDS_FILSEL);
		return true;
	}

	displayText::tabmsg(IDS_FILCR);
	return true;
}

void displayText::grpmsg() {
	displayText::shoseln(IDS_FGRPS, IDS_UNGRP);
}

void displayText::grpmsg1() {
	displayText::tabmsg(IDS_NOGRP);
}

void displayText::internal::sdmsg() {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, IDS_SAVDISC);
	displayText::shoMsg(fmt::format(fmtStr, ThrName->wstring()));
}

void displayText::alrotmsg() {
	displayText::shoseln(IDS_ALLX, IDS_ROTAGIN);
}

void displayText::shord() {
	displayText::shoseln(IDS_FGRPF, IDS_ROTDUP);
}

void displayText::spltmsg() {
	displayText::shoseln(IDS_FRMGUID, IDS_SPLT);
}

void displayText::datmsg(unsigned code) {
	auto dataErrorID = 0u;
	auto dataError   = std::wstring{};

	switch (code) {
	case BADFLT:
		dataErrorID = IDS_BADFLT;
		break;
	case BADCLP:
		dataErrorID = IDS_BADCLP;
		break;
	case BADSAT:
		dataErrorID = IDS_BADSAT;
		break;
	case BADTX:
		dataErrorID = IDS_BADTX;
		break;
	default:
		dataErrorID = IDS_BADUKN;
		break;
	}
	displayText::loadString(dataError, dataErrorID);
	displayText::shoMsg(dataError);
}

void displayText::okcan() {
	GetClientRect(MsgWindow, &MsgRect);

	OKButton = CreateWindow(L"STATIC",
	                        (*StringTable)[STR_OKENT].c_str(),
	                        SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                        5,
	                        MsgRect.bottom + 15,
	                        ButtonWidth << 2,
	                        ButtonHeight,
	                        MainStitchWin,
	                        nullptr,
	                        ThrEdInstance,
	                        nullptr);

	CancelButton = CreateWindow(L"STATIC",
	                            (*StringTable)[STR_CANCEL].c_str(),
	                            SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                            ButtonWidth * 5,
	                            MsgRect.bottom + 15,
	                            ButtonWidthX3,
	                            ButtonHeight,
	                            MainStitchWin,
	                            nullptr,
	                            ThrEdInstance,
	                            nullptr);
}

void displayText::savdisc() {
	wchar_t buffer[HBUFSIZ];

	di::sdmsg();
	StateMap.reset(StateFlag::BIGBOX);

	GetClientRect(MsgWindow, &MsgRect);

	LoadString(ThrEdInstance, IDS_SAV, &buffer[0], HBUFSIZ);
	OKButton = CreateWindow(L"STATIC",
	                        &buffer[0],
	                        SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                        5,
	                        MsgRect.bottom + 15,
	                        ButtonWidthX3,
	                        ButtonHeight,
	                        MainStitchWin,
	                        nullptr,
	                        ThrEdInstance,
	                        nullptr);

	LoadString(ThrEdInstance, IDS_DISC, &buffer[0], HBUFSIZ);
	DiscardButton = CreateWindow(L"STATIC",
	                             &buffer[0],
	                             SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                             ButtonWidthX3 + 15,
	                             MsgRect.bottom + 15,
	                             ButtonWidthX3,
	                             ButtonHeight,
	                             MainStitchWin,
	                             nullptr,
	                             ThrEdInstance,
	                             nullptr);

	CancelButton = CreateWindow(L"STATIC",
	                            (*StringTable)[STR_CANCEL].c_str(),
	                            SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                            2 * ButtonWidthX3 + 25,
	                            MsgRect.bottom + 15,
	                            ButtonWidthX3,
	                            ButtonHeight,
	                            MainStitchWin,
	                            nullptr,
	                            ThrEdInstance,
	                            nullptr);
}

#pragma warning(push)
#pragma warning(disable : 26493) // we use c style casts as this is a C API
BOOL CALLBACK EnumChildProc(HWND p_hWnd, LPARAM lParam) {
	SendMessage(p_hWnd, WM_SETFONT, gsl::narrow_cast<WPARAM>(lParam), MAKELPARAM(TRUE, 0));
	return TRUE;
}

void displayText::updateWinFont(HWND hWnd) noexcept {
	const auto* hFont = displayText::getThrEdFont(400);
	EnumChildWindows(hWnd, EnumChildProc, (LPARAM)hFont);
}
#pragma warning(pop)

void displayText::tomsg() {
	auto OKrect   = RECT{};
	auto textSize = SIZE{};

	GetWindowRect(OKButton, &OKrect);
	GetTextExtentPoint32Int(GetDC(ThrEdWindow),
	                        (*StringTable)[STR_DELST2].c_str(),
	                        gsl::narrow<unsigned int>((*StringTable)[STR_DELST2].size()),
	                        &textSize);
	DeleteStitchesDialog = CreateWindow(L"STATIC",
	                                    (*StringTable)[STR_DELST2].c_str(),
	                                    SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                    3,
	                                    OKrect.bottom - StitchWindowOrigin.y + 6 + textSize.cy,
	                                    textSize.cx + 6,
	                                    textSize.cy + 6,
	                                    MainStitchWin,
	                                    nullptr,
	                                    ThrEdInstance,
	                                    nullptr);
	displayText::updateWinFont(MainStitchWin);
}

void displayText::internal::bxtxt(unsigned iButton, unsigned iMessage) {
	auto message = std::wstring{};
	displayText::loadString(message, iMessage);
	SetWindowText((*ButtonWin)[iButton], message.c_str());
}

void displayText::internal::hlpflt(unsigned iButton, unsigned iMessage, float data) {
	auto fmtStr = std::wstring{};
	displayText::loadString(fmtStr, iMessage);
	SetWindowText((*ButtonWin)[iButton], fmt::format(fmtStr, data).c_str());
}

void displayText::drwtxbut(const TXTSCR& textureScreen) {
	di::bxtxt(HTXCLR, IDS_CLEAR);
	di::hlpflt(HTXHI, IDS_TXHI, textureScreen.areaHeight / PFGRAN);
	thred::redraw((*ButtonWin)[HTXWID]);
	di::hlpflt(HTXSPAC, IDS_TXSPAC, textureScreen.spacing / PFGRAN);
	di::bxtxt(HTXVRT, IDS_TXVRT);
	di::bxtxt(HTXHOR, IDS_TXHOR);
	di::bxtxt(HTXANG, IDS_TXANG);
	di::bxtxt(HTXMIR, IDS_TXMIR);
	SetWindowText((*ButtonWin)[HTXMIR + 1], L"");
}

HFONT displayText::getThrEdFont(LONG weight) noexcept {
	auto lfText = LOGFONT{};
#if HIGHDPI
	const auto uDpi = GetDpiForWindow(ThrEdWindow);
	SystemParametersInfoForDpi(SPI_GETICONTITLELOGFONT, sizeof(lfText), &lfText, FALSE, uDpi);
	lfText.lfWeight = weight;
	return CreateFontIndirectW(&lfText);
#else
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lfText), &lfText, FALSE);
	lfText.lfWeight = weight;
	return CreateFontIndirectW(&lfText);

#endif
}

GSL_SUPPRESS(26490) GSL_SUPPRESS(26461) void displayText::setWindowFont(HWND hWnd, HFONT hFont) noexcept {
	SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
}