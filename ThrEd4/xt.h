#ifndef THRED_XT_H
#define THRED_XT_H

// Local Headers
#include "formHeader.h"

// Standard Libraries
#include <vector>

enum FSI : uint8_t { // fill starts index values
  applique,
  fill,
  feather,
  border,
  appliqueColor,
  fillColor,
  featherColor,
  borderColor,
  fsi_size
};

using FillStartsDataType = std::array<uint32_t, fsi_size>;

namespace xt {

void chgchk(uint8_t code);
void chgwrn();
void chkcwlk(uint32_t formIndex);
void chkund(uint32_t formIndex, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
void chkwlk(uint32_t formIndex);
void clrstch() noexcept;

#ifdef _DEBUG
void dmpat();
#endif

void duauxnam(fs::path& auxName);
void dubcol(uint8_t color);
void dubit(FRM_HEAD& form, uint32_t bit);
void dublen(float length);
void dubmax(float length);
void dubmin(float length);
void dubspac(float length);
void dufang(float angle);
void dufcol(uint8_t color);
void dufhi(float length);
void dufind(float indent);
void duflen(float length);
void dufmax(float length);
void dufmin(float length);
void dufspac(float spacing);
void dufwid(float length);
void dufxang(float angle);
void dundcol(uint8_t color);
void dushft();
void duspac(float spacing);
void dusulen(float length);
void fdelstch(uint32_t formIndex, FillStartsDataType& fillStartsData, uint32_t& fillStartsMap);
void fethr();
void fethrf(uint32_t formIndex);
void fsort();
void fthrfn(FRM_HEAD& form);
auto insid(FRM_HEAD const& form) -> std::vector<F_POINT>&;
void intlv(uint32_t formIndex, FillStartsDataType const& fillStartsData, uint32_t fillStartsMap);
void mvshft();
void notcwlk();
void notund();
void notwlk();
void nudsiz();
void selalfrm();
void setclpspac();
void setcwlk();
void setEdit(uint32_t code, StateFlag flag);
void setfchk();
void setfilend();
void setfilstrt();
void setuang();
void setulen();
void setund();
void setuspac();
void setwlk();
void setwlkind();
void srtcol();
} // namespace xt

#endif // THRED_XT_H
