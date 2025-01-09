#pragma once

// Local Headers
#include "formHeader.h"

// Standard Libraries
#include <vector>

namespace texture {
void deltx(uint32_t formIndex);
void drwtxtr();
auto dutxtfil() -> bool;
void redtx();
void rstxt();
void rtrtx(FRM_HEAD const& form);
void savtxt();
void setshft();
void setTxtCurLoc(POINT location) noexcept;
void setxfrm() noexcept;
void setxt(FRM_HEAD& form, std::vector<RNG_COUNT>& textureSegments);
void txdun();
void txof();
void txsnap();
void txtkey(wchar_t keyCode, FRM_HEAD& textureForm);
void txtlbut(FRM_HEAD const& textureForm);
void txtrbut();
void txtrmov(FRM_HEAD const& textureForm);
void txtrup();
auto textureInit() -> uint32_t;
void writeScreenWidth(DRAWITEMSTRUCT const& drawItem, int32_t position);

} // namespace texture
