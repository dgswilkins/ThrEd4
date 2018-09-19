#pragma once

// Standard Libraries
#include <string>

// Local Headers
#include "ThrEdTypes.h"

namespace repair {

void frmchkx();
void lodchk();
void repar();

namespace internal {

	void     adbad(std::wstring& repairMessage, unsigned code, unsigned int count);
	void     bcup(unsigned int find, BADCNTS& badData) noexcept;
	void     chkclp(const FRMHED* const formHeader, BADCNTS& badData);
	void     chkeclp(const FRMHED* const formHeader, BADCNTS& badData);
	void     chkfstch() noexcept;
	unsigned frmchkfn();
	void     repclp(std::wstring& repairMessage);
	void     repflt(std::wstring& repairMessage);
	void     repsat();
	void     reptx();
} // namespace internal
} // namespace repair