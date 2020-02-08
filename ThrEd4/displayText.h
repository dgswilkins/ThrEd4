#pragma once

// Local Headers
#include "ThrEdTypes.h"

namespace displayText {

void  alrotmsg();
void  bfilmsg();
void  butxt(uint32_t iButton, std::wstring const& buttonText);
void  clrhbut(uint32_t startButton) noexcept;
auto  clpmsgs(uint32_t code) -> bool;
void  crmsg(fs::path const& fileName);
void  datmsg(uint32_t code);
void  drwtxbut(TXTSCR const& textureScreen);
auto  filmsgs(uint32_t code) -> bool;
void  filnopn(uint32_t code, fs::path const& fileName);
void  frm1pnt();
HFONT getThrEdFont(int32_t weight) noexcept;
void  grpmsg();
void  grpmsg1();
void  hsizmsg();
void  loadString(std::wstring& sDest, uint32_t stringID);
void  lodstr();
void  msgflt(uint32_t messageId, float value);
void  msgstr(uint32_t code) noexcept;
void  numWnd() noexcept;
void  okcan() noexcept;
void  pntmsg(uint32_t msgID);
void  riter();
void  ritnum(uint32_t code, uint32_t value);
void  tabmsg(uint32_t code);
void  savdisc();
void  setWindowFont(HWND hWnd, HFONT hFont) noexcept;
void  shoMsg(std::wstring const& message);
void  shord();
void  shoseln(uint32_t code0, uint32_t code1);
void  spltmsg();
void  tomsg();
void  tsizmsg(wchar_t const* threadSizeText, double threadSize);
void  updateWinFont(HWND hWnd) noexcept;

namespace internal {
  void bxtxt(uint32_t iButton, uint32_t iMessage);
  void hlpflt(uint32_t iButton, uint32_t iMessage, float data);
  void sdmsg();
} // namespace internal
} // namespace displayText
