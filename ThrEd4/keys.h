#pragma once

namespace keys {
void desiz();
auto createFormType(wchar_t code) -> bool;
auto getMoveAnchor() noexcept -> uint32_t;
auto handleMainWinKeys(wchar_t const& code, F_POINT& rotationCenter, std::vector<POINT> const& stretchBoxLine) -> bool;
void movmrk();
void navigateThumbnails(wchar_t code);
auto otherKeys(wchar_t code) -> bool;
void setmov();
void ungrphi();
void ungrplo();
} // namespace keys
