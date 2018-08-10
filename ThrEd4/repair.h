#pragma once

// Standard Libraries
#include <string>

// Local Headers
#include "ThrEdTypes.h"

namespace repair {

void frmchkx();
void repar();
}

namespace repairPriv {

void     adbad(std::wstring& repairMessage, unsigned code, size_t count);
void     bcup(size_t find, BADCNTS& badData) noexcept;
void     chkclp(const FRMHED* const formHeader, BADCNTS& badData);
void     chkeclp(const FRMHED* const formHeader, BADCNTS& badData);
void     chkfstch() noexcept;
unsigned frmchkfn();
void     repclp(std::wstring& repairMessage);
void     repflt(std::wstring& repairMessage);
void     repsat();
void     reptx();
}