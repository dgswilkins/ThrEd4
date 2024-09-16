// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "displayText.h"
#include "form.h"
#include "globals.h"
#include "Resources/resource.h"
#include "textureHeader.h"
#include "thred.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/narrow"
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <libloaderapi.h>
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <cstdint>
#include <filesystem>
// ReSharper disable CppUnusedIncludeDirective
#include <ranges>
// ReSharper restore CppUnusedIncludeDirective
#include <stdexcept>
#include <string>
#include <vector>

// displayText internal namespace
namespace di {
void bxtxt(uint32_t iButton, uint32_t iMessage);
auto clpmsgs(uint32_t code) -> bool;
void hlpflt(uint32_t iButton, uint32_t iMessage, float data);
void sdmsg();
} // namespace di

auto displayText::loadStr(uint32_t const stringID) -> std::wstring {
  auto* pBuf  = gsl::narrow_cast<wchar_t*>(nullptr);
  auto  sDest = std::wstring {};
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (auto const len = LoadString(ThrEdInstance, stringID, reinterpret_cast<LPTSTR>(&pBuf), 0)) {
	auto const span = gsl::span {pBuf, wrap::toSize(len)};
	sDest.insert(sDest.end(), span.begin(), span.end());
  }
  return sDest;
}

void displayText::shoMsg(std::wstring const& message, bool const top) {
  if (message.empty()) {
	return;
  }
  auto       strings              = std::vector<std::wstring> {};
  auto       iString              = 0U;
  auto       previousStringLength = 0U;
  auto const sizeLim              = message.size();
  while (iString < sizeLim) {
	if (constexpr auto NEWLINE = 10; message[iString] == NEWLINE) {
	  auto const subLen = iString++ - previousStringLength;
	  strings.push_back(message.substr(previousStringLength, subLen));
	  previousStringLength = iString;
	}
	else {
	  ++iString;
	}
  }
  auto const subLen = iString - previousStringLength;
  strings.push_back(message.substr(previousStringLength, subLen));
  auto textSize    = SIZE {};
  auto messageSize = SIZE {};
  for (auto& string : strings) {
	wrap::getTextExtentPoint32(GetDC(ThrEdWindow), string.c_str(), wrap::toUnsigned(string.size()), &textSize);
	messageSize.cx = std::max(textSize.cx, messageSize.cx);
	messageSize.cy = std::max(textSize.cy, messageSize.cy);
  }
  messageSize.cy *= gsl::narrow<int32_t>(strings.size());
  auto mainRect = RECT {};
  GetWindowRect(MainStitchWin, &mainRect);
  auto thredRect = RECT {};
  GetWindowRect(ThrEdWindow, &thredRect);
  auto const xOffset = mainRect.left - thredRect.left;
  auto       yOffset = 3;
  if (!top) {
	constexpr auto PXOFFSET = 12; // pixel offset

	yOffset = mainRect.bottom - mainRect.top - PXOFFSET - messageSize.cy;
  }
  MsgWindow = CreateWindow(L"STATIC",
                           message.c_str(),
                           SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                           xOffset,
                           yOffset,
                           messageSize.cx + 20,
                           messageSize.cy + 6,
                           ThrEdWindow,
                           nullptr,
                           ThrEdInstance,
                           nullptr);
}

void displayText::tabmsg(uint32_t const code, bool const top) {
  shoMsg(loadStr(code), top);
}

void displayText::hsizmsg() {
  auto const fmtStr =
      format2(IDS_HSIZ, wrap::toFloat(UnzoomedRect.cx) * IPFGRAN, wrap::toFloat(UnzoomedRect.cy) * IPFGRAN);
  shoMsg(fmtStr, false);
}

void displayText::numWnd() {
  auto messageRect = RECT {0L, 0L, 0L, 0L};
  GetClientRect(MsgWindow, &messageRect);
  auto wRect = RECT {0L, 0L, 0L, 0L};
  GetWindowRect(MainStitchWin, &wRect);
  auto xOffset = wRect.left;
  GetWindowRect(ThrEdWindow, &wRect);
  xOffset -= wRect.left;
  if (nullptr == GeneralNumberInputBox) {
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
  }
  else {
	throw std::runtime_error("GeneralNumberInputBox is null"); // we should never reach this
  }
  thred::resetMsgBuffer();
}

void displayText::msgflt(uint32_t const messageId, float const value) {
  showMessage(messageId, value);
  Instance->StateMap.set(StateFlag::NUMIN);
  numWnd();
}

void displayText::tsizmsg(wchar_t const* threadSizeText, float const threadSize) {
  auto const fmtStr = format2(IDS_SIZ, threadSizeText, threadSize);
  shoMsg(fmtStr, true);
  Instance->StateMap.set(StateFlag::NUMIN);
  numWnd();
}

void displayText::bfilmsg() {
  showMessage(IDS_BADFIL, Instance->WorkingFileName.wstring());
}

void displayText::filnopn(uint32_t const code, fs::path const& fileName) {
  showMessage(code, fileName.wstring());
}

void displayText::crmsg(fs::path const& fileName) {
  showMessage(IDS_CREAT, fileName.wstring());
}

void displayText::butxt(uint32_t const iButton, std::wstring const& buttonText) {
  if (!Instance->StateMap.test(StateFlag::WASTRAC) || iButton <= HNUM) {
	SetWindowText(Instance->ButtonWin.operator[](iButton), buttonText.c_str());
	return;
  }
  if (iButton != HMINLEN) {
	auto const bText = loadStr(iButton - 4U + IDS_TRC0);
	SetWindowText(Instance->ButtonWin.operator[](iButton), bText.c_str());
	return;
  }
  if (!Instance->StateMap.test(StateFlag::HIDMAP)) {
	auto const bText = loadStr(IDS_TRC1S);
	SetWindowText(Instance->ButtonWin.operator[](iButton), bText.c_str());
	return;
  }
  auto const bText = loadStr(IDS_TRC1H);
  SetWindowText(Instance->ButtonWin.operator[](iButton), bText.c_str());
}

void displayText::clrhbut(uint32_t const startButton) {
  for (auto const spButtons =
           std::ranges::subrange(wrap::next(Instance->ButtonWin.begin(), startButton), Instance->ButtonWin.end());
       auto const iButton : spButtons) { // NOLINT(readability-qualified-auto)
	SetWindowText(iButton, L"");
  }
}

void displayText::ritnum(uint32_t const code, uint32_t const value) {
  butxt(HNUM, format(code, value));
}

void displayText::riter() {
  tabmsg(IDS_RITER, false);
}

void displayText::pntmsg(uint32_t const msgID) {
  auto const message = loadStr(msgID);
  showMessage(IDS_PNT, message);
}

void displayText::shoseln(uint32_t const code0, uint32_t const code1) {
  auto const msg0 = loadStr(code0);
  auto const msg1 = loadStr(code1);
  shoMsg(format2(IDS_SHOSEL, msg0, msg1), false);
}

auto di::clpmsgs(uint32_t const code) -> bool {
  form::ispcdclp();
  if ((code == FML_CLP || code == FMM_CLP || code == FML_PIC) && !Instance->StateMap.test(StateFlag::WASPCDCLP)) {
	displayText::tabmsg(IDS_CLPS, false);
	return false;
  }
  return true;
}

void displayText::frm1pnt() {
  if (Instance->FormList.size() == 1) {
	Instance->StateMap.set(StateFlag::FORMSEL);
	ClosestFormToCursor = 0;
  }
}

auto displayText::filmsgs(uint32_t const code) -> bool {
  if (!Instance->SelectedFormList.empty()) {
	return di::clpmsgs(code);
  }

  if (auto const& formList = Instance->FormList; !formList.empty()) {
	frm1pnt();
	if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	  if (auto const& form = formList.operator[](ClosestFormToCursor); form.vertexCount == 2) {
		if (code < FML_LIN) {
		  tabmsg(IDS_FRM3X, false);
		  return false;
		}

		if (code == FML_PRPS) {
		  tabmsg(IDS_ANGS, false);
		  return false;
		}
	  }
	  return di::clpmsgs(code);
	}

	tabmsg(IDS_FILSEL, false);
	return false;
  }
  tabmsg(IDS_FILCR, false);
  return false;
}

void displayText::grpmsg() {
  shoseln(IDS_FGRPS, IDS_UNGRP);
}

void displayText::grpmsg1() {
  tabmsg(IDS_NOGRP, false);
}

void di::sdmsg() {
  displayText::showMessage(IDS_SAVDISC, ThrName->wstring());
}

void displayText::alrotmsg() {
  shoseln(IDS_ALLX, IDS_ROTAGIN);
}

void displayText::shord() {
  shoseln(IDS_FGRPF, IDS_ROTDUP);
}

void displayText::spltmsg() {
  shoseln(IDS_FRMGUID, IDS_SPLT);
}

void displayText::okcan() {
  GetClientRect(MsgWindow, &MsgRect);
  OKButton     = CreateWindow(L"STATIC",
                          displayText::loadStr(IDS_OKENT).c_str(),
                          SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                          5,
                          MsgRect.bottom + 15,
                          ButtonWidth * 4,
                          ButtonHeight,
                          MainStitchWin,
                          nullptr,
                          ThrEdInstance,
                          nullptr);
  CancelButton = CreateWindow(L"STATIC",
                              displayText::loadStr(IDS_CANCEL).c_str(),
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
  di::sdmsg();
  Instance->StateMap.reset(StateFlag::BIGBOX);
  GetClientRect(MsgWindow, &MsgRect);
  OKButton      = CreateWindow(L"STATIC",
                          displayText::loadStr(IDS_SAV).c_str(),
                          SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                          5,
                          MsgRect.bottom + 15,
                          ButtonWidthX3,
                          ButtonHeight,
                          MainStitchWin,
                          nullptr,
                          ThrEdInstance,
                          nullptr);
  DiscardButton = CreateWindow(L"STATIC",
                               displayText::loadStr(IDS_DISC).c_str(),
                               SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                               ButtonWidthX3 + 15,
                               MsgRect.bottom + 15,
                               ButtonWidthX3,
                               ButtonHeight,
                               MainStitchWin,
                               nullptr,
                               ThrEdInstance,
                               nullptr);
  CancelButton  = CreateWindow(L"STATIC",
                              displayText::loadStr(IDS_CANCEL).c_str(),
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

namespace {
// ReSharper disable CppParameterMayBeConst
auto CALLBACK enumChildProc(HWND p_hWnd, LPARAM lParam) noexcept -> BOOL {
  SendMessage(p_hWnd, WM_SETFONT, gsl::narrow_cast<WPARAM>(lParam), MAKELPARAM(TRUE, 0));
  return TRUE;
}
// ReSharper restore CppParameterMayBeConst
} // namespace

// ReSharper disable CppParameterMayBeConst
void displayText::updateWinFont(HWND hWnd) noexcept {
  auto const* hFont = getThrEdFont(FONTSIZE);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  EnumChildWindows(hWnd, enumChildProc, reinterpret_cast<LPARAM>(hFont));
}
// ReSharper restore CppParameterMayBeConst

void displayText::tomsg() {
  auto okRect   = RECT {};
  auto textSize = SIZE {};
  GetWindowRect(OKButton, &okRect);
  auto const winName = loadStr(IDS_DELST2);
  wrap::getTextExtentPoint32(GetDC(ThrEdWindow), winName.c_str(), wrap::toUnsigned(winName.size()), &textSize);
  DeleteStitchesDialog = CreateWindow(L"STATIC",
                                      winName.c_str(),
                                      SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                      3,
                                      okRect.bottom - StitchWindowOrigin.y + 6 + textSize.cy,
                                      textSize.cx + 6,
                                      textSize.cy + 6,
                                      MainStitchWin,
                                      nullptr,
                                      ThrEdInstance,
                                      nullptr);
  updateWinFont(MainStitchWin);
}

void di::bxtxt(uint32_t const iButton, uint32_t const iMessage) {
  SetWindowText(Instance->ButtonWin.operator[](iButton), displayText::loadStr(iMessage).c_str());
}

void di::hlpflt(uint32_t const iButton, uint32_t const iMessage, float const data) {
  auto const fmtStr = displayText::format(iMessage, data);
  SetWindowText(Instance->ButtonWin.operator[](iButton), fmtStr.c_str());
}

void displayText::drwtxbut(TXTR_SCREEN const& textureScreen) {
  di::bxtxt(HTXCLR, IDS_CLEAR);
  di::hlpflt(HTXHI, IDS_TXHI, textureScreen.areaHeight * IPFGRAN);
  thred::redraw(Instance->ButtonWin.operator[](HTXWID));
  di::hlpflt(HTXSPAC, IDS_TXSPAC, textureScreen.spacing * IPFGRAN);
  di::bxtxt(HTXVRT, IDS_TXVRT);
  di::bxtxt(HTXHOR, IDS_TXHOR);
  di::bxtxt(HTXANG, IDS_TXANG);
  di::bxtxt(HTXMIR, IDS_TXMIR);
  auto constexpr INDEX = HTXMIR + 1U;
  SetWindowText(Instance->ButtonWin.operator[](INDEX), L"");
}

auto displayText::getThrEdFont(int32_t weight) noexcept -> HFONT {
  auto lfText = LOGFONT {0L, 0L, 0L, 0L, 0L, 0, 0, 0, 0, 0, 0, 0, 0, L""};
#if HIGHDPI
  auto const uDpi = GetDpiForWindow(ThrEdWindow);
  SystemParametersInfoForDpi(SPI_GETICONTITLELOGFONT, sizeof(lfText), &lfText, FALSE, uDpi);
  lfText.lfWeight = weight;
  return CreateFontIndirectW(&lfText);
#else
  SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lfText), &lfText, FALSE);
  lfText.lfWeight = weight;
  return CreateFontIndirectW(&lfText);

#endif
}

// ReSharper disable CppParameterMayBeConst
#pragma warning(suppress : 26461) // con.3 The pointer argument can be marked as a pointer to const
void displayText::setWindowFont(HWND hWnd, HFONT hFont) noexcept {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
}
// ReSharper restore CppParameterMayBeConst
