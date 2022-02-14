#pragma once

// Standard Libraries
#include <string>

// Local Headers
#include "ThrEdTypes.h"
#include "repairHeader.h"

namespace repair {
void frmchkx();
void lodchk();
void repar();

namespace internal {
  void adbad(std::wstring& repairMessage, uint32_t code, uint32_t count);
  void bcup(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
  void chkclp(FRM_HEAD const& formHeader, BAD_COUNTS& badData) noexcept;
  void chkeclp(FRM_HEAD const& formHeader, BAD_COUNTS& badData) noexcept;
  void chkfstch() noexcept;
  void chkSat(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
  void chkTxt(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
  void chkVrtx(FRM_HEAD const& form, BAD_COUNTS& badData) noexcept;
  void datmsg(uint32_t code);
  auto frmchkfn() -> uint32_t;
  void repclp(std::wstring& repairMessage);
  void repflt(std::wstring& repairMessage);
  void repsat();
  void reptx();
} // namespace internal
} // namespace repair
