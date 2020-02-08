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
  void adbad(std::wstring& repairMessage, uint32_t code, uint32_t count);
  void bcup(FRMHED const& form, BADCNTS& badData) noexcept;
  void chkclp(FRMHED const& formHeader, BADCNTS& badData) noexcept;
  void chkeclp(FRMHED const& formHeader, BADCNTS& badData) noexcept;
  void chkfstch() noexcept;
  void chkSat(FRMHED const& form, BADCNTS& badData) noexcept;
  void chkTxt(FRMHED const& form, BADCNTS& badData) noexcept;
  void chkVrtx(FRMHED const& form, BADCNTS& badData) noexcept;
  auto frmchkfn() -> uint32_t;
  void repclp(std::wstring& repairMessage);
  void repflt(std::wstring& repairMessage);
  void repsat();
  void reptx();
} // namespace internal
} // namespace repair
