#pragma once

// Local Headers
#include "ThrEdTypes.h"

namespace displayText {

void        alrotmsg();
void        bfilmsg();
void        butxt(uint32_t iButton, const std::wstring& buttonText);
void        clrhbut(uint32_t startButton);
bool        clpmsgs(uint32_t code);
void        crmsg(const fs::path& fileName);
void        datmsg(uint32_t code);
void        drwtxbut(const TXTSCR& textureScreen);
bool        filmsgs(uint32_t code);
void        filnopn(uint32_t code, const fs::path& fileName);
void        frm1pnt();
HFONT       getThrEdFont(int32_t weight) noexcept;
void        grpmsg();
void        grpmsg1();
void        hsizmsg();
inline void loadString(std::wstring& sDest, uint32_t stringID);
void        lodstr();
void        msgflt(uint32_t messageId, float value);
void        msgstr(uint32_t code) noexcept;
void        numWnd() noexcept;
void        okcan();
void        pntmsg(uint32_t msgID);
void        riter();
void        ritnum(uint32_t code, uint32_t value);
void        tabmsg(uint32_t code);
void        savdisc();
void        setWindowFont(HWND hWnd, HFONT hFont) noexcept;
void        shoMsg(const std::wstring& message);
void        shord();
void        shoseln(uint32_t code0, uint32_t code1);
void        spltmsg();
void        tomsg();
void        tsizmsg(const wchar_t* threadSizeText, double threadSize);
void        updateWinFont(HWND hWnd) noexcept;

namespace internal {
	void bxtxt(uint32_t iButton, uint32_t iMessage);
	void hlpflt(uint32_t iButton, uint32_t iMessage, float data);
	void sdmsg();
} // namespace internal
} // namespace displayText