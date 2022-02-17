#pragma once

// Local Headers
#include "formHeader.h"

// Standard Libraries
#include <cstdint>
#include <filesystem>
#include <vector>

class FEATHER
{
  public:
  uint32_t fillType {};          // type of feather fill
  float    ratio {};             // feather ratio
  float    minStitch {};         // smallest stitch allowed
  float    ratioLocal {};        // local feather ratio
  float    formRatio {};         // feather ratio from form
  uint32_t extendedAttribute {}; // extended form attribute
  uint32_t upCount {};           // feather up count
  uint32_t downCount {};         // feather down count
  uint32_t totalCount {};        // up count plus down count
  uint32_t phaseIndex {};
  uint32_t phase {};
  uint32_t countUp {};
  uint32_t countDown {};
  float    globalStep {};
  float    globalPosition {};
  float    globalPhase {};
  float    globalUp {};
  float    globalDown {};
  float    globalRatio {};

  // constexpr FEATHER() noexcept = default;
  // FEATHER(FEATHER const&) = default;
  // FEATHER(FEATHER&&) = default;
  // FEATHER& operator=(FEATHER const& rhs) = default;
  // FEATHER& operator=(FEATHER&&) = default;
  //~FEATHER() = default;
};

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
  F_STARTS fillNamed;
  uint32_t fillArray[sizeof(F_STARTS) / sizeof(uint32_t)] {};

  // inline FILL_STARTS() noexcept;
  // FILL_STARTS(FILL_STARTS const&) = default;
  // FILL_STARTS(FILL_STARTS&&) = default;
  // FILL_STARTS& operator=(FILL_STARTS const& rhs) = default;
  // FILL_STARTS& operator=(FILL_STARTS&&) = default;
  //~FILL_STARTS() = default;
};

class INT_INFO
{
  public:
  uint32_t      pins {};
  uint32_t      coloc {};
  uint32_t      layerIndex {};
  uint32_t      start {};
  uint32_t      output {};
  F_POINT_ATTR* highStitchBuffer {};

  // constexpr INT_INFO() noexcept = default;
  // INT_INFO(INT_INFO const&) = default;
  // INT_INFO(INT_INFO&&) = default;
  // INT_INFO& operator=(INT_INFO const& rhs) = default;
  // INT_INFO& operator=(INT_INFO&&) = default;
  //~INT_INFO() = default;
};

class O_REC
{
  public:
  uint32_t start {};
  uint32_t finish {};
  uint32_t startStitch {};
  uint32_t endStitch {};
  uint32_t color {};
  uint32_t type {};
  uint32_t form {};
  uint32_t otyp {};

  // constexpr O_REC() noexcept = default;
  // O_REC(O_REC const&) = default;
  // O_REC(O_REC&&) = default;
  // O_REC& operator=(O_REC const& rhs) = default;
  // O_REC& operator=(O_REC&&) = default;
  //~O_REC() = default;
};

class SORT_REC
{
  public:
  uint32_t start {};          // start region index
  uint32_t finish {};         // finish region index
  uint32_t count {};          // number of regions in range
  uint32_t currentRegion {};  // current region index
  bool     direction = false; // direction of sort

  // constexpr SORT_REC() noexcept = default;
  // SORT_REC(SORT_REC const&) = default;
  // SORT_REC(SORT_REC&&) = default;
  // SORT_REC& operator=(SORT_REC const& rhs) = default;
  // SORT_REC& operator=(SORT_REC&&) = default;
  //~SORT_REC() = default;
};

namespace fs = std::filesystem;

namespace xt {

void chgchk(uint8_t code);
void chgwrn();
void chkcwlk(FRM_HEAD& form);
void chkund(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
void chkwlk(FRM_HEAD& form);
void clrstch() noexcept;

#ifdef _DEBUG
void dmpat();
#endif

void duauxnam();
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
void fdelstch(FRM_HEAD const& form, FILL_STARTS& fillStartsData, uint32_t& fillStartsMap);
void fethr();
void fethrf();
void fsort();
void fthrfn(FRM_HEAD& form);
auto insid(FRM_HEAD const& form) -> std::vector<F_POINT>&;
void intlv(FRM_HEAD const& form, FILL_STARTS const& fillStartsData, uint32_t fillStartsMap);
void mvshft();
void notcwlk();
void notund();
void notwlk();
void nudsiz();
void rtrclp();
void selalfrm();
void setbcol();
void setblen();
void setbmax();
void setbmin();
void setbspac();
void setclpspac();
void setcwlk();
void setfang();
void setfchk();
void setfcol();
void setfhi();
void setfilend();
void setfilstrt();
void setfind();
void setflen();
void setfmax();
void setfmin();
void setfspac();
void setfwid();
void setuang();
void setucol();
void setulen();
void setund();
void setuspac();
void setwlk();
void setwlkind();
void sfuang();
void srtcol();
void tst();
void undlen();
void uspac();

namespace internal {

  void bcolfn(uint32_t formNumber, uint8_t color);
  void blenfn(FRM_HEAD& form, float length);
  void bmaxfn(uint32_t formNumber, float length);
  void bminfn(uint32_t formNumber, float length);
  auto bpsg() noexcept -> uint32_t;
  void bspacfn(uint32_t formNumber, float length);
  auto chkasp(F_POINT& point, float aspectRatio, HWND dialog) -> bool;
  void chkend(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t code, INT_INFO& ilData);
  auto chkrdun(std::vector<uint32_t> const& formFillCounter,
               std::vector<O_REC*> const&   pRecs,
               SORT_REC const&              stitchRecord) noexcept -> bool;
  void delwlk(uint32_t code);

#ifdef _DEBUG
  void dmprec(std::vector<O_REC*> const& stitchRegion, uint32_t count);
  void duatf(uint32_t ind);
#endif

  void duint(FRM_HEAD const& form, std::vector<F_POINT_ATTR>& buffer, uint32_t code, INT_INFO& ilData);
  auto duprecs(std::vector<F_POINT_ATTR>& tempStitchBuffer, std::vector<O_REC*> const& pRecs, SORT_REC& sortRecord)
      -> uint32_t;

  constexpr auto durat(float start, float finish, float featherRatio) -> float;

  void durats(uint32_t iSequence, std::vector<F_POINT>* sequence, FEATHER& feather);
  void durec(O_REC& record);
  auto dutyp(uint32_t attribute) noexcept -> uint32_t;
  void duxrats(uint32_t start, uint32_t finish, F_POINT& point, float featherRatioLocal) noexcept;

  auto CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept -> BOOL;

  void fangfn(uint32_t formNumber, float angle);
  void fcolfn(uint32_t formNumber, uint8_t color);
  void fhifn(uint32_t formNumber, float length);
  void findfn(uint32_t formNumber, float indent);
  void flenfn(uint32_t formNumber, float length);
  void fmaxfn(uint32_t formNumber, float length);
  void fminfn(uint32_t formNumber, float length);
  void fncwlk(FRM_HEAD& form);
  void fnund(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
  void fnwlk(FRM_HEAD& form);
  void fritfil(FRM_HEAD const& form, std::vector<F_POINT> const& featherSequence);
  void fspacfn(uint32_t formNumber, float spacing);
  void fthdfn(uint32_t iSequence, FEATHER& feather);
  void fthfn(uint32_t iSequence, FEATHER& feather);
  void fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<F_POINT>& featherSequence);
  void fthvars(FRM_HEAD const& form, FEATHER& feather);
  void fwidfn(uint32_t formNumber, float length);
  auto getstxt(int32_t stringIndex, HWND dialog) -> float;
  auto gucon(FRM_HEAD const&            form,
             std::vector<F_POINT_ATTR>& buffer,
             F_POINT const&             start,
             F_POINT const&             finish,
             uint32_t                   destination,
             uint32_t                   code) -> uint32_t;
  auto isfil(FRM_HEAD const& form) noexcept -> bool;
  auto lastcol(uint32_t index, F_POINT& point) noexcept -> bool;
  auto midpnt(F_POINT const& startPoint, F_POINT const& endPoint) noexcept -> F_POINT;
  void notundfn(uint32_t code);
  void nudfn(F_RECTANGLE const& designSizeRect) noexcept;
  void nurat(FEATHER& feather) noexcept;
  auto precjmps(std::vector<F_POINT_ATTR>& tempStitchBuffer,
                std::vector<O_REC*> const& pRecs,
                SORT_REC const&            sortRecord) -> double;
  void ratpnt(uint32_t iPoint, uint32_t iNextPoint, F_POINT& point, float featherRatio) noexcept;
  auto orComp(O_REC const* record1, O_REC const* record2) -> bool;
  auto orfComp(O_REC const* record1, O_REC const* record2) noexcept -> bool;
  void rtrclpfn(FRM_HEAD const& form);
  void ritwlk(FRM_HEAD& form, uint32_t walkMask);
  void sadj(F_POINT& point, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept;
  void sadj(F_POINT_ATTR& stitch, F_POINT const& designSizeRatio, F_RECTANGLE const& designSizeRect) noexcept;

  auto CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  void setstxt(int32_t stringIndex, float value, HWND dialog);
  void setundfn(uint32_t code);
  auto srtchk(std::vector<O_REC*> const& stitchRegion, uint32_t count, uint32_t& badForm) -> bool;

  constexpr auto tim2int(FILETIME time) noexcept -> ULARGE_INTEGER;

  void uangfn(uint32_t formNumber, float angle);
  void ucolfn(uint32_t formNumber, uint8_t color);
  void ulenfn(uint32_t formNumber, float length);
  void undclp(FRM_HEAD const& form);
  void uspacfn(uint32_t formNumber, float spacing);
  void xratf(F_POINT const& startPoint, F_POINT const& endPoint, F_POINT& point, float featherRatioLocal) noexcept;
} // namespace internal
} // namespace xt
