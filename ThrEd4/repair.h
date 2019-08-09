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

	void     adbad(std::wstring& repairMessage, uint32_t code, uint32_t count);
	void     bcup(uint32_t find, BADCNTS& badData);
	void     chkclp(const FRMHED& formHeader, BADCNTS& badData) noexcept;
	void     chkeclp(const FRMHED& formHeader, BADCNTS& badData) noexcept;
	void     chkfstch() noexcept;
	void     chkSat(const FRMHED& form, BADCNTS& badData) noexcept;
	void     chkTxt(const FRMHED& form, BADCNTS& badData) noexcept;
	void     chkVrtx(const FRMHED& form, BADCNTS& badData) noexcept;
	uint32_t frmchkfn();
	void     repclp(std::wstring& repairMessage);
	void     repflt(std::wstring& repairMessage);
	void     repsat();
	void     reptx();
} // namespace internal
} // namespace repair
