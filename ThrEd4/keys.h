#pragma once

namespace keys {
void desiz();
auto getMoveAnchor() noexcept -> uint32_t;
auto handleMainWinKeys(wchar_t const& code, F_POINT& rotationCenter, std::vector<POINT> const& stretchBoxLine, bool& retflag)
    -> bool;
void movmrk();
void setmov();
void ungrphi();
void ungrplo();
} // namespace keys
