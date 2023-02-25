// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "displayText.h"
#include "form.h"
#include "globals.h"
#include "thred.h"

// displayText internal namespace
namespace di {
void bxtxt(uint32_t iButton, uint32_t iMessage);
void hlpflt(uint32_t iButton, uint32_t iMessage, float data);
void sdmsg();
} // namespace di

auto displayText::loadStr(uint32_t stringID) -> std::wstring {
  auto* pBuf  = gsl::narrow_cast<wchar_t*>(nullptr);
  auto  sDest = std::wstring {};
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (auto const len = LoadString(ThrEdInstance, stringID, reinterpret_cast<LPTSTR>(&pBuf), 0)) {
	auto const span = gsl::span {pBuf, wrap::toSize(len)};
	sDest.insert(sDest.end(), span.begin(), span.end());
  }
  return sDest;
}

void displayText::shoMsg(std::wstring const& message, bool top) {
  if (!message.empty()) {
	auto       strings              = std::vector<std::wstring> {};
	auto       iString              = 0U;
	auto       previousStringLength = 0U;
	auto const sizeLim              = message.size();
	while (iString < sizeLim) {
	  if (constexpr auto NEWLINE = 10; message[iString] == NEWLINE) {
		strings.push_back(message.substr(previousStringLength, (iString++ - previousStringLength)));
		previousStringLength = iString;
	  }
	  else {
		++iString;
	  }
	}
	strings.push_back(message.substr(previousStringLength, (iString - previousStringLength)));
	auto textSize    = SIZE {};
	auto messageSize = SIZE {};
	for (auto& string : strings) {
	  wrap::getTextExtentPoint32(GetDC(ThrEdWindow), string.c_str(), wrap::toUnsigned(string.size()), &textSize);
	  if (textSize.cx > messageSize.cx) {
		messageSize.cx = textSize.cx;
	  }
	  if (textSize.cy > messageSize.cy) {
		messageSize.cy = textSize.cy;
	  }
	}
	messageSize.cy *= gsl::narrow<int32_t>(strings.size());
	auto mainRect = RECT {};
	GetWindowRect(MainStitchWin, &mainRect);
	auto thredRect = RECT {};
	GetWindowRect(ThrEdWindow, &thredRect);
	auto const xOffset = mainRect.left - thredRect.left;
	auto       yOffset = 3;
	if (!top) {
	  yOffset = (mainRect.bottom - mainRect.top) - 12 - messageSize.cy;
	}
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
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
	//		displayText::updateWinFont(MainStitchWin);
  }
}

void displayText::tabmsg(uint32_t code, bool top) {
  displayText::shoMsg(displayText::loadStr(code), top);
}

void displayText::hsizmsg() {
  // NOLINTNEXTLINE(clang-diagnostic-sign-conversion)
  auto const fmtStr = fmt::format(fmt::runtime(displayText::loadStr(IDS_HSIZ)),
                                  (wrap::toFloat(UnzoomedRect.cx) * IPFGRAN),
                                  (wrap::toFloat(UnzoomedRect.cy) * IPFGRAN));
  displayText::shoMsg(fmtStr, false);
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
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
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

void displayText::msgflt(uint32_t messageId, float value) {
  displayText::showMessage(messageId, value);
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void displayText::tsizmsg(wchar_t const* threadSizeText, float threadSize) {
  auto const fmtStr = fmt::format(fmt::runtime(displayText::loadStr(IDS_SIZ)), threadSizeText, threadSize);
  displayText::shoMsg(fmtStr, true);
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void displayText::bfilmsg() {
  displayText::showMessage(IDS_BADFIL, WorkingFileName->wstring());
}

void displayText::filnopn(uint32_t code, fs::path const& fileName) {
  displayText::showMessage(code, fileName.wstring());
}

void displayText::crmsg(fs::path const& fileName) {
  displayText::showMessage(IDS_CREAT, fileName.wstring());
}

void displayText::butxt(uint32_t iButton, std::wstring const& buttonText) {
  if (StateMap->test(StateFlag::WASTRAC) && iButton > HNUM) {
	if (iButton == HMINLEN) {
	  if (StateMap->test(StateFlag::HIDMAP)) {
		auto const bText = displayText::loadStr(IDS_TRC1H);
		SetWindowText(ButtonWin->operator[](iButton), bText.c_str());
	  }
	  else {
		auto const bText = displayText::loadStr(IDS_TRC1S);
		SetWindowText(ButtonWin->operator[](iButton), bText.c_str());
	  }
	}
	else {
	  auto const bText = displayText::loadStr(iButton - 4U + IDS_TRC0);
	  SetWindowText(ButtonWin->operator[](iButton), bText.c_str());
	}
  }
  else {
	SetWindowText(ButtonWin->operator[](iButton), buttonText.c_str());
  }
}

void displayText::clrhbut(uint32_t startButton) {
  for (auto const spButtons =
           std::ranges::subrange(wrap::next(ButtonWin->begin(), startButton), ButtonWin->end());
       auto const iButton : spButtons) { // NOLINT(readability-qualified-auto)
	SetWindowText(iButton, L"");
  }
}

void displayText::ritnum(uint32_t code, uint32_t value) {
  auto const fmtStr = fmt::format(fmt::runtime(displayText::loadStr(code)), value);
  displayText::butxt(HNUM, fmtStr);
}

void displayText::riter() {
  displayText::tabmsg(IDS_RITER, false);
}

void displayText::pntmsg(uint32_t msgID) {
  auto const message = displayText::loadStr(msgID);
  displayText::showMessage(IDS_PNT, message);
}

void displayText::shoseln(uint32_t code0, uint32_t code1) {
  auto const msg0   = displayText::loadStr(code0);
  auto const msg1   = displayText::loadStr(code1);
  auto const fmtStr = fmt::format(fmt::runtime(displayText::loadStr(IDS_SHOSEL)), msg0, msg1);
  displayText::shoMsg(fmtStr, false);
}

auto displayText::clpmsgs(uint32_t code) -> bool {
  form::ispcdclp();
  if ((code == FML_CLP || code == FMM_CLP || code == FML_PIC) && !StateMap->test(StateFlag::WASPCDCLP)) {
	displayText::tabmsg(IDS_CLPS, false);
	return true;
  }
  return false;
}

void displayText::frm1pnt() {
  if (FormList->size() == 1) {
	StateMap->set(StateFlag::FORMSEL);
	ClosestFormToCursor = 0;
  }
}

auto displayText::filmsgs(uint32_t code) -> bool {
  if (!SelectedFormList->empty()) {
	return displayText::clpmsgs(code);
  }
  if (!FormList->empty()) {
	displayText::frm1pnt();
	if (StateMap->test(StateFlag::FORMSEL)) {
	  if (auto const& form = FormList->operator[](ClosestFormToCursor); form.vertexCount == 2) {
		if (code < FML_LIN) {
		  displayText::tabmsg(IDS_FRM3X, false);
		  return false;
		}

		if (code == FML_PRPS) {
		  displayText::tabmsg(IDS_ANGS, false);
		  return false;
		}
	  }
	  return displayText::clpmsgs(code);
	}

	displayText::tabmsg(IDS_FILSEL, false);
	return false;
  }
  displayText::tabmsg(IDS_FILCR, false);
  return false;
}

void displayText::grpmsg() {
  displayText::shoseln(IDS_FGRPS, IDS_UNGRP);
}

void displayText::grpmsg1() {
  displayText::tabmsg(IDS_NOGRP, false);
}

void di::sdmsg() {
  displayText::showMessage(IDS_SAVDISC, ThrName->wstring());
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

void displayText::okcan() {
  GetClientRect(MsgWindow, &MsgRect);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  OKButton = CreateWindow(L"STATIC",
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
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  StateMap->reset(StateFlag::BIGBOX);
  GetClientRect(MsgWindow, &MsgRect);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  OKButton = CreateWindow(L"STATIC",
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
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CancelButton = CreateWindow(L"STATIC",
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

static auto CALLBACK enumChildProc(HWND p_hWnd, LPARAM lParam) noexcept -> BOOL {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  SendMessage(p_hWnd, WM_SETFONT, gsl::narrow_cast<WPARAM>(lParam), MAKELPARAM(TRUE, 0));
  return TRUE;
}

void displayText::updateWinFont(HWND hWnd) noexcept {
  auto const* hFont = displayText::getThrEdFont(FONTSIZE);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  EnumChildWindows(hWnd, enumChildProc, reinterpret_cast<LPARAM>(hFont));
}

void displayText::tomsg() {
  auto okRect   = RECT {};
  auto textSize = SIZE {};
  GetWindowRect(OKButton, &okRect);
  auto const winName = displayText::loadStr(IDS_DELST2);
  wrap::getTextExtentPoint32(GetDC(ThrEdWindow), winName.c_str(), wrap::toUnsigned(winName.size()), &textSize);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  displayText::updateWinFont(MainStitchWin);
}

void di::bxtxt(uint32_t iButton, uint32_t iMessage) {
  SetWindowText(ButtonWin->operator[](iButton), displayText::loadStr(iMessage).c_str());
}

void di::hlpflt(uint32_t iButton, uint32_t iMessage, float data) {
  auto const fmtStr = fmt::format(fmt::runtime(displayText::loadStr(iMessage)), data);
  SetWindowText(ButtonWin->operator[](iButton), fmtStr.c_str());
}

void displayText::drwtxbut(TXTR_SCREEN const& textureScreen) {
  di::bxtxt(HTXCLR, IDS_CLEAR);
  di::hlpflt(HTXHI, IDS_TXHI, textureScreen.areaHeight * IPFGRAN);
  thred::redraw(ButtonWin->operator[](HTXWID));
  di::hlpflt(HTXSPAC, IDS_TXSPAC, textureScreen.spacing * IPFGRAN);
  di::bxtxt(HTXVRT, IDS_TXVRT);
  di::bxtxt(HTXHOR, IDS_TXHOR);
  di::bxtxt(HTXANG, IDS_TXANG);
  di::bxtxt(HTXMIR, IDS_TXMIR);
  SetWindowText(ButtonWin->operator[](HTXMIR + 1U), L"");
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

#pragma warning(suppress : 26461) // con.3 The pointer argument can be marked as a pointer to const
void displayText::setWindowFont(HWND hWnd, HFONT hFont) noexcept {
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
}
