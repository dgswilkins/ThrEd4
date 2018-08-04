#pragma once

// Local Headers
#include "ThrEdTypes.h"

void alrotmsg();
void bfilmsg();
void butxt(unsigned iButton, const std::wstring& buttonText);
void bxtxt(unsigned iButton, unsigned iMessage);
// bool        clpmsgs(unsigned code);
void        clrhbut(unsigned startButton);
void        crmsg(const fs::path& fileName);
void        datmsg(unsigned code);
void        drwtxbut(const TXTSCR& textureScreen);
bool        filmsgs(unsigned code);
void        filnopn(unsigned code, const fs::path& fileName);
void        frm1pnt();
void        grpmsg();
void        grpmsg1();
void        hlpflt(unsigned iButton, unsigned iMessage, float data);
void        hsizmsg();
inline void loadString(std::wstring& sDest, unsigned stringID);
void        lodstr();
void        msgflt(unsigned messageId, float value);
void        msgstr(unsigned code) noexcept;
void        numWnd() noexcept;
void        okcan();
void        pntmsg(unsigned msgID);
void        riter();
void        ritnum(unsigned code, size_t value);
void        savdisc();
void        sdmsg();
void        shoMsg(const std::wstring& message);
void        shord();
void        shoseln(unsigned code0, unsigned code1);
void        spltmsg();
void        tabmsg(unsigned code);
void        tomsg();
void        tsizmsg(const wchar_t* threadSizeText, double threadSize);
