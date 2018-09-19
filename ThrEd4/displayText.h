#pragma once

// Local Headers
#include "ThrEdTypes.h"

namespace displayText {

void        alrotmsg();
void        bfilmsg();
void        butxt(unsigned iButton, const std::wstring& buttonText);
void        clrhbut(unsigned startButton);
bool        clpmsgs(unsigned code);
void        crmsg(const fs::path& fileName);
void        datmsg(unsigned code);
void        drwtxbut(const TXTSCR& textureScreen);
bool        filmsgs(unsigned code);
void        filnopn(unsigned code, const fs::path& fileName);
void        frm1pnt();
HFONT       getThrEdFont(LONG weight) noexcept;
void        grpmsg();
void        grpmsg1();
void        hsizmsg();
inline void loadString(std::wstring& sDest, unsigned stringID);
void        lodstr();
void        msgflt(unsigned messageId, float value);
void        msgstr(unsigned code) noexcept;
void        numWnd() noexcept;
void        okcan();
void        pntmsg(unsigned msgID);
void        riter();
void        ritnum(unsigned code, unsigned int value);
void        tabmsg(unsigned code);
void        savdisc();
void        setWindowFont(HWND hWnd, HFONT hFont) noexcept;
void        shoMsg(const std::wstring& message);
void        shord();
void        shoseln(unsigned code0, unsigned code1);
void        spltmsg();
void        tomsg();
void        tsizmsg(const wchar_t* threadSizeText, double threadSize);
void        updateWinFont(HWND hWnd) noexcept;

namespace internal {
	void bxtxt(unsigned iButton, unsigned iMessage);
	void hlpflt(unsigned iButton, unsigned iMessage, float data);
	void sdmsg();
} // namespace internal
} // namespace displayText