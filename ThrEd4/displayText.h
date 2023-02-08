#pragma once

// Local Headers
#include "textureHeader.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26812) // supress warning for fmt library header
#include "fmt/xchar.h"
#include "fmt/compile.h"
#pragma warning(pop)

// Standard Libraries
#include <cstdint>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace displayText {
void alrotmsg();
void bfilmsg();
void butxt(uint32_t iButton, std::wstring const& buttonText);
void clrhbut(uint32_t startButton);
auto clpmsgs(uint32_t code) -> bool;
void crmsg(fs::path const& fileName);
void drwtxbut(TXTR_SCREEN const& textureScreen);
auto filmsgs(uint32_t code) -> bool;
void filnopn(uint32_t code, fs::path const& fileName);
void frm1pnt();
auto getThrEdFont(int32_t weight) noexcept -> HFONT;
void grpmsg();
void grpmsg1();
void hsizmsg();
auto loadStr(uint32_t stringID) -> std::wstring;
void msgflt(uint32_t messageId, float value);
void numWnd();
void okcan();
void pntmsg(uint32_t msgID);
void riter();
void ritnum(uint32_t code, uint32_t value);
void savdisc();
void setWindowFont(HWND hWnd, HFONT hFont) noexcept;
void shoMsg(std::wstring const& message, bool top);

template <class inType> void showMessage(uint32_t messageId, inType value) {
  auto const fmtStr = fmt::format(fmt::runtime(displayText::loadStr(messageId)), value);
  displayText::shoMsg(fmtStr, true);
}

void shord();
void shoseln(uint32_t code0, uint32_t code1);
void spltmsg();
void tabmsg(uint32_t code, bool top);
void tomsg();
void tsizmsg(wchar_t const* threadSizeText, float threadSize);
void updateWinFont(HWND hWnd) noexcept;
} // namespace displayText
