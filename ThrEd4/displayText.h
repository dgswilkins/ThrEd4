#pragma once

void        bfilmsg();
void        butxt(unsigned iButton, const std::wstring& buttonText);
void        crmsg(const fs::path& fileName);
void        filnopn(unsigned code, const fs::path& fileName);
void        help();
void        hsizmsg();
inline void loadString(std::wstring& sDest, unsigned stringID);
void        lodstr();
void        msgflt(unsigned messageId, float value);
void        msgstr(unsigned code) noexcept;
void        numWnd() noexcept;
void        pntmsg(unsigned msgID);
void        riter();
void        ritnum(unsigned code, size_t value);
void        shoMsg(const std::wstring& message);
void        shoseln(unsigned code0, unsigned code1);
void        tabmsg(unsigned code);
void        tsizmsg(const wchar_t* threadSizeText, double threadSize);
