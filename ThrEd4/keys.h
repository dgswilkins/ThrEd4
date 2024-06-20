#pragma once

namespace keys {
void desiz();
auto createFormType(const wchar_t code) -> bool;
auto getMoveAnchor() noexcept -> uint32_t;
auto handleMainWinKeys(wchar_t const& code, F_POINT& rotationCenter, std::vector<POINT> const& stretchBoxLine)
    -> bool;
void movmrk();
void navigateThumbnails(const wchar_t code);
auto otherKeys(const wchar_t code) -> bool;
void setmov();
void ungrphi();
void ungrplo();
} // namespace keys
