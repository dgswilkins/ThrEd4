#pragma once

// Local Headers
#include "textureHeader.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "fmt/xchar.h"
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
void okcan();
void pntmsg(uint32_t msgID);
void riter();
void ritnum(uint32_t code, uint32_t value);
void savdisc();
void setWindowFont(HWND hWnd, HFONT hFont) noexcept;
void shoMsg(std::wstring const& message, bool top);
void shord();
void shoseln(uint32_t code0, uint32_t code1);
void spltmsg();
void tabmsg(uint32_t code, bool top);
void tomsg();
void tsizmsg(wchar_t const* threadSizeText, float threadSize);
void updateWinFont(HWND hWnd) noexcept;

template <class inType> auto format(uint32_t const messageId, inType const& value) -> std::wstring {
  return fmt::format(fmt::runtime(loadStr(messageId)), value);
}

template <class inTypeA, class inTypeB>
auto format2(uint32_t const messageId, inTypeA const& valueA, inTypeB const& valueB) -> std::wstring {
  return fmt::format(fmt::runtime(loadStr(messageId)), valueA, valueB);
}

template <class inTypeA, class inTypeB, class inTypeC, class inTypeD, class inTypeE>
auto format5(uint32_t const messageId, inTypeA valueA, inTypeB valueB, inTypeC valueC, inTypeD valueD, inTypeE valueE)
    -> std::wstring {
  return fmt::format(fmt::runtime(loadStr(messageId)), valueA, valueB, valueC, valueD, valueE);
}

template <class inType> void showMessage(uint32_t messageId, inType const& value) {
  auto const fmtStr = format(messageId, value);
  displayText::shoMsg(fmtStr, true);
}

} // namespace displayText
