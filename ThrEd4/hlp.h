#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <string>

bool clpmsgs(unsigned code);
bool filmsgs(unsigned code);
void adbad(std::wstring& repairMessage, unsigned code, size_t count);
void alrotmsg();
void datmsg(unsigned code);
void frm1pnt();
void grpmsg();
void grpmsg1();
void sdmsg();
void shord();
void spltmsg();