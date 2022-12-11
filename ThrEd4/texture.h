#pragma once

// Local Headers
#include "formHeader.h"
#include "textureHeader.h"

// Standard Libraries
#include <vector>

namespace texture {
void deltx(uint32_t formIndex);
void drwtxtr();
auto dutxtfil() -> bool;
void initTextures(std::vector<TX_PNT>*   ptrTexturePoints,
                  std::vector<uint32_t>* ptrTexturePointsList,
                  std::vector<TX_HIST>*  ptrTextureHistory) noexcept;
auto istx(uint32_t iForm) noexcept -> bool;
void redtx();
void rstxt();
void rtrtx(FRM_HEAD const& form);
void savtxt();
void setshft();
void setxt(FRM_HEAD& form, std::vector<RNG_COUNT>& textureSegments);
void txdun();
void txof();
void txsnap();
void txtkey(wchar_t keyCode, FRM_HEAD& textureForm);
void txtlbut(FRM_HEAD const& textureForm);
void txtrbut();
void txtrmov(FRM_HEAD const& textureForm);
void txtrup();
void writeScreenWidth(int32_t position);

} // namespace texture
