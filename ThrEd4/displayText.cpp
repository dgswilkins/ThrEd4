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

void displayText::loadString(std::wstring& sDest, uint32_t stringID) {
  auto* pBuf = gsl::narrow_cast<wchar_t*>(nullptr);
  sDest.clear();
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (auto const len = LoadString(ThrEdInstance, stringID, reinterpret_cast<LPTSTR>(&pBuf), 0)) {
	auto const span = gsl::span<wchar_t>(pBuf, wrap::toSize(len));
	sDest.insert(sDest.end(), span.begin(), span.end());
  }
}

void displayText::shoMsg(std::wstring const& message) {
  if (!message.empty()) {
	constexpr auto newline              = 10;
	auto           strings              = std::vector<std::wstring> {};
	auto           iString              = 0U;
	auto           previousStringLength = 0U;
	auto const     sizeLim              = message.size();
	while (iString < sizeLim) {
	  if (message[iString] == newline) {
		strings.push_back(message.substr(previousStringLength, (iString++ - previousStringLength)));
		previousStringLength = iString;
	  }
	  else {
		++iString;
	  }
	}
	strings.push_back(message.substr(previousStringLength, (iString++ - previousStringLength)));
	auto textSize    = SIZE {0L, 0L};
	auto messageSize = SIZE {0L, 0L};
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
	auto mainRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(MainStitchWin, &mainRect);
	auto xOffset = mainRect.left;
	GetWindowRect(ThrEdWindow, &mainRect);
	xOffset -= mainRect.left;
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
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

void displayText::tabmsg(uint32_t code) {
  auto message = std::wstring {};
  displayText::loadString(message, code);
  displayText::shoMsg(message);
}

void displayText::lodstr() {
  static constexpr auto loadStringList = std::array<uint16_t, STR_LEN> {
      // strings to load into memory at init time
      IDS_PIKOL,     IDS_UPON,    IDS_UPOF,    IDS_AUXTXT,  IDS_HUP0,    IDS_HUP1,    IDS_HUP2,
      IDS_HUP3,      IDS_HUP4,    IDS_TRC0,    IDS_TRC1S,   IDS_TRC2,    IDS_TRC3,    IDS_TRC4,
      IDS_TRC1H,     IDS_NUMPNT,  IDS_NUMFRM,  IDS_NUMSCH,  IDS_NUMFORM, IDS_NUMSEL,  IDS_BOXSEL,
      IDS_CLOS,      IDS_TOT,     IDS_LAYR,    IDS_OVRLOK,  IDS_OVRIT,   IDS_THRED,   IDS_NUFIL,
      IDS_EMB,       IDS_ON,      IDS_OFF,     IDS_FMEN,    IDS_TXT0,    IDS_TXT1,    IDS_TXT2,
      IDS_TXT3,      IDS_TXT4,    IDS_TXT5,    IDS_TXT6,    IDS_TXT7,    IDS_TXT8,    IDS_TXT9,
      IDS_TXT10,     IDS_TXT11,   IDS_TXT12,   IDS_TXT13,   IDS_TXT14,   IDS_TXT15,   IDS_TXT16,
      IDS_TXT17,     IDS_TXT18,   IDS_TXT19,   IDS_TXT20,   IDS_TXT21,   IDS_TXT22,   IDS_TXT23,
      IDS_FIL0,      IDS_FIL1,    IDS_FIL2,    IDS_FIL3,    IDS_FIL4,    IDS_FIL5,    IDS_FIL6,
      IDS_FIL7,      IDS_FIL8,    IDS_FIL9,    IDS_FIL10,   IDS_FIL11,   IDS_FIL12,   IDS_FIL13,
      IDS_EDG0,      IDS_EDG1,    IDS_EDG2,    IDS_EDG3,    IDS_EDG4,    IDS_EDG5,    IDS_EDG6,
      IDS_EDG7,      IDS_EDG8,    IDS_EDG9,    IDS_EDG10,   IDS_EDG11,   IDS_EDG12,   IDS_PRF0,
      IDS_PRF1,      IDS_PRF2,    IDS_PRF3,    IDS_PRF4,    IDS_PRF5,    IDS_PRF6,    IDS_PRF7,
      IDS_PRF8,      IDS_PRF9,    IDS_PRF10,   IDS_PRF11,   IDS_PRF12,   IDS_PRF13,   IDS_PRF14,
      IDS_PRF15,     IDS_PRF16,   IDS_PRF17,   IDS_PRF18,   IDS_PRF19,   IDS_PRF20,   IDS_PRF21,
      IDS_PRF22,     IDS_PRF23,   IDS_PRF24,   IDS_PRF25,   IDS_PRF26,   IDS_PRF27,   IDS_PRF28,
      IDS_PRF29,     IDS_FRMPLUS, IDS_FRMINUS, IDS_OKENT,   IDS_CANCEL,  IDS_FREH,    IDS_BLUNT,
      IDS_TAPR,      IDS_PNTD,    IDS_SQR,     IDS_DELST2,  IDS_THRDBY,  IDS_STCHOUT, IDS_STCHS,
      IDS_FORMS,     IDS_HUPWID,  IDS_CREATBY, IDS_CUSTHUP, IDS_STCHP,   IDS_FRMP,    IDS_ENTROT,
      IDS_NUDG,      IDS_ALLX,    IDS_FTHCOL,  IDS_FTHTYP,  IDS_FTH0,    IDS_FTH1,    IDS_FTH2,
      IDS_FTH3,      IDS_FTH4,    IDS_FTH5,    IDS_FTHBLND, IDS_FTHUP,   IDS_FTHBOTH, IDS_FTHUPCNT,
      IDS_FTHDWNCNT, IDS_FTHSIZ,  IDS_FTHNUM,  IDS_FTHFLR,  IDS_FSTRT,   IDS_FEND,    IDS_WALK,
      IDS_UWLKIND,   IDS_UND,     IDS_ULEN,    IDS_FUANG,   IDS_FUSPAC,  IDS_CWLK,    IDS_UNDCOL,
      IDS_FRMBOX,    IDS_TXOF};

  for (auto iString = 0U; iString < loadStringList.size(); ++iString) {
	displayText::loadString(StringTable->operator[](iString), loadStringList[iString]);
  }
}

void displayText::hsizmsg() {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_HSIZ);
  // NOLINTNEXTLINE
  displayText::shoMsg(fmt::format(fmtStr, (wrap::toFloat(UnzoomedRect.x) * IPFGRAN), (wrap::toFloat(UnzoomedRect.y) * IPFGRAN)));
}

void displayText::numWnd() {
  auto messageRect = RECT {0L, 0L, 0L, 0L};
  GetClientRect(MsgWindow, &messageRect);
  auto wRect = RECT {0L, 0L, 0L, 0L};
  GetWindowRect(MainStitchWin, &wRect);
  auto xOffset = wRect.left;
  GetWindowRect(ThrEdWindow, &wRect);
  xOffset -= wRect.left;
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
  MsgIndex              = 0;
  MsgBuffer.fill(0);
}

void displayText::msgflt(uint32_t messageId, float value) {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, messageId);
  displayText::shoMsg(fmt::format(fmtStr, value));
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void displayText::tsizmsg(wchar_t const* threadSizeText, float threadSize) {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_SIZ);
  displayText::shoMsg(fmt::format(fmtStr, threadSizeText, threadSize));
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void displayText::bfilmsg() {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_BADFIL);
  displayText::shoMsg(fmt::format(fmtStr, WorkingFileName->wstring()));
}

void displayText::filnopn(uint32_t code, fs::path const& fileName) {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, code);
  displayText::shoMsg(fmt::format(fmtStr, fileName.wstring()));
}

void displayText::crmsg(fs::path const& fileName) {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_CREAT);
  displayText::shoMsg(fmt::format(fmtStr, fileName.wstring()));
}

void displayText::butxt(uint32_t iButton, std::wstring const& buttonText) {
  if (StateMap->test(StateFlag::WASTRAC) && iButton > HNUM) {
	if (iButton == HMINLEN) {
	  if (StateMap->test(StateFlag::HIDMAP)) {
		SetWindowText(ButtonWin->operator[](iButton), StringTable->operator[](STR_TRC1H).c_str());
	  }
	  else {
		SetWindowText(ButtonWin->operator[](iButton), StringTable->operator[](STR_TRC1S).c_str());
	  }
	}
	else {
	  SetWindowText(ButtonWin->  operator[](iButton),
	                StringTable->operator[](wrap::toSize(iButton) - 4U + STR_TRC0).c_str());
	}
  }
  else {
	SetWindowText(ButtonWin->operator[](iButton), buttonText.c_str());
  }
}

void displayText::clrhbut(uint32_t startButton) noexcept {
  for (auto iButton = startButton; iButton < BTNCOUNT; ++iButton) {
	SetWindowText(ButtonWin->operator[](iButton), L"");
  }
}

void displayText::ritnum(uint32_t code, uint32_t value) {
  displayText::butxt(HNUM, fmt::format(StringTable->operator[](code), value));
}

void displayText::riter() {
  displayText::tabmsg(IDS_RITER);
}

void displayText::pntmsg(uint32_t msgID) {
  auto fmtStr  = std::wstring {};
  auto message = std::wstring {};
  displayText::loadString(fmtStr, IDS_PNT);
  displayText::loadString(message, msgID);
  displayText::shoMsg(fmt::format(fmtStr, message));
}

void displayText::shoseln(uint32_t code0, uint32_t code1) {
  auto fmtStr = std::wstring {};
  auto msg0   = std::wstring {};
  auto msg1   = std::wstring {};
  displayText::loadString(fmtStr, IDS_SHOSEL);
  displayText::loadString(msg0, code0);
  displayText::loadString(msg1, code1);
  displayText::shoMsg(fmt::format(fmtStr, msg0, msg1));
}

auto displayText::clpmsgs(uint32_t code) -> bool {
  form::ispcdclp();
  if ((code == FML_CLP || code == FMM_CLP || code == FML_PIC) && !StateMap->test(StateFlag::WASPCDCLP)) {
	displayText::tabmsg(IDS_CLPS);
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
	  auto const& form = FormList->operator[](ClosestFormToCursor);
	  if (form.vertexCount == 2) {
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
  auto fmtStr = std::wstring {};

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

void displayText::datmsg(uint32_t code) {
  auto dataErrorID = 0U;
  auto dataError   = std::wstring {};
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

void displayText::okcan() noexcept {
  GetClientRect(MsgWindow, &MsgRect);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  OKButton = CreateWindow(L"STATIC",
                          StringTable->operator[](STR_OKENT).c_str(),
                          SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                          5,
                          MsgRect.bottom + 15,
                          ButtonWidth << 2U,
                          ButtonHeight,
                          MainStitchWin,
                          nullptr,
                          ThrEdInstance,
                          nullptr);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CancelButton = CreateWindow(L"STATIC",
                              StringTable->operator[](STR_CANCEL).c_str(),
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
  StateMap->reset(StateFlag::BIGBOX);
  GetClientRect(MsgWindow, &MsgRect);
  LoadString(ThrEdInstance, IDS_SAV, std::begin(buffer), HBUFSIZ);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  OKButton = CreateWindow(L"STATIC",
                          std::begin(buffer),
                          SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                          5,
                          MsgRect.bottom + 15,
                          ButtonWidthX3,
                          ButtonHeight,
                          MainStitchWin,
                          nullptr,
                          ThrEdInstance,
                          nullptr);
  LoadString(ThrEdInstance, IDS_DISC, std::begin(buffer), HBUFSIZ);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  DiscardButton = CreateWindow(L"STATIC",
                               std::begin(buffer),
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
                              StringTable->operator[](STR_CANCEL).c_str(),
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

static auto CALLBACK EnumChildProc(HWND p_hWnd, LPARAM lParam) noexcept -> BOOL {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  SendMessage(p_hWnd, WM_SETFONT, gsl::narrow_cast<WPARAM>(lParam), MAKELPARAM(TRUE, 0));
  return TRUE;
}

void displayText::updateWinFont(HWND hWnd) noexcept {
  auto const* hFont = displayText::getThrEdFont(FONTSIZE);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  EnumChildWindows(hWnd, EnumChildProc, reinterpret_cast<LPARAM>(hFont));
}

void displayText::tomsg() {
  auto OKrect   = RECT {0L, 0L, 0L, 0L};
  auto textSize = SIZE {0L, 0L};
  GetWindowRect(OKButton, &OKrect);
  wrap::getTextExtentPoint32(GetDC(ThrEdWindow),
                             StringTable->operator[](STR_DELST2).c_str(),
                             wrap::toUnsigned(StringTable->operator[](STR_DELST2).size()),
                             &textSize);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  DeleteStitchesDialog = CreateWindow(L"STATIC",
                                      StringTable->operator[](STR_DELST2).c_str(),
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

void displayText::internal::bxtxt(uint32_t iButton, uint32_t iMessage) {
  auto message = std::wstring {};
  displayText::loadString(message, iMessage);
  SetWindowText(ButtonWin->operator[](iButton), message.c_str());
}

void displayText::internal::hlpflt(uint32_t iButton, uint32_t iMessage, float data) {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, iMessage);
  SetWindowText(ButtonWin->operator[](iButton), fmt::format(fmtStr, data).c_str());
}

void displayText::drwtxbut(TXTSCR const& textureScreen) {
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
