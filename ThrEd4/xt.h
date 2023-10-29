#pragma once

// Local Headers
#include "formHeader.h"

// Standard Libraries
#include <vector>

class F_STARTS
{
  public:
  uint32_t applique {};
  uint32_t fill {};
  uint32_t feather {};
  uint32_t border {};
  uint32_t appliqueColor {};
  uint32_t fillColor {};
  uint32_t featherColor {};
  uint32_t borderColor {};

  // constexpr F_STARTS() noexcept = default;
  // F_STARTS(F_STARTS const&) = default;
  // F_STARTS(F_STARTS&&) = default;
  // F_STARTS& operator=(F_STARTS const& rhs) = default;
  // F_STARTS& operator=(F_STARTS&&) = default;
  //~F_STARTS() = default;
};

union FILL_STARTS {
  public:
  F_STARTS                                                    fillNamed;
  std::array<uint32_t, (sizeof(F_STARTS) / sizeof(uint32_t))> fillArray {};

  // inline FILL_STARTS() noexcept;
  // FILL_STARTS(FILL_STARTS const&) = default;
  // FILL_STARTS(FILL_STARTS&&) = default;
  // FILL_STARTS& operator=(FILL_STARTS const& rhs) = default;
  // FILL_STARTS& operator=(FILL_STARTS&&) = default;
  //~FILL_STARTS() = default;
};

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
void fdelstch(uint32_t formIndex, FILL_STARTS& fillStartsData, uint32_t& fillStartsMap);
void fethr();
void fethrf(uint32_t formIndex);
void fsort();
void fthrfn(FRM_HEAD& form);
auto insid(FRM_HEAD const& form) -> std::vector<F_POINT>&;
void intlv(uint32_t formIndex, FILL_STARTS const& fillStartsData, uint32_t fillStartsMap);
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
