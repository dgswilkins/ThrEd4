#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <filesystem>
#include <vector>

// Local Headers
#include "ThrEdTypes.h"

namespace fs = std::filesystem;

namespace xt {

void chgchk(uint8_t code);
void chgwrn();
void chkcwlk(FRMHED& form);
void chkund(FRMHED& form, std::vector<RNGCNT> const& textureSegments, std::vector<fPOINT>& angledFormVertices);
void chkwlk(FRMHED& form);
void clrstch() noexcept;

#ifdef _DEBUG
void dmpat();
#endif

void duauxnam();
void dubcol(uint32_t color);
void dubit(FRMHED& form, uint32_t bit);
void dublen(float length);
void dubmax(float length);
void dubmin(float length);
void dubspac(float length);
void dufang(float angle);
void dufcol(uint32_t color);
void dufhi(float length);
void dufind(float indent);
void duflen(float length);
void dufmax(float length);
void dufmin(float length);
void dufspac(float spacing);
void dufwid(float length);
void dufxang(float angle);
void dundcol(uint32_t color);
void dushft();
void duspac(float spacing);
void dusulen(float length);
void fdelstch(FRMHED const& form, FILLSTARTS& fillStartsData, uint32_t& fillStartsMap);
void fethr();
void fethrf();
void fsort();
void fthrfn(FRMHED& form);
auto insid(FRMHED const& form) -> std::vector<fPOINT>&;
void intlv(FRMHED const& form, FILLSTARTS const& fillStartsData, uint32_t fillStartsMap);
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

  void bcolfn(uint32_t formNumber, uint32_t color);
  void blenfn(FRMHED& form, float length);
  void bmaxfn(uint32_t formNumber, float length);
  void bminfn(uint32_t formNumber, float length);
  auto bpsg() noexcept -> uint32_t;
  void bspacfn(uint32_t formNumber, float length);
  auto chkasp(fPOINT& point, float aspectRatio, HWND dialog) -> bool;
  void chkend(FRMHED const& form, std::vector<fPOINTATTR>& buffer, uint32_t code, INTINF& ilData);
  auto chkrdun(std::vector<uint32_t> const& formFillCounter,
               std::vector<OREC*> const&    pRecs,
               SRTREC const&                stitchRecord) noexcept -> bool;
  void delwlk(uint32_t code);

#ifdef _DEBUG
  void dmprec(std::vector<OREC*> const& stitchRegion, uint32_t count);
  void duatf(uint32_t ind);
#endif

  void duint(FRMHED const& form, std::vector<fPOINTATTR>& buffer, uint32_t code, INTINF& ilData);
  auto duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, std::vector<OREC*> const& pRecs, SRTREC& sortRecord)
      -> uint32_t;

  constexpr auto durat(float start, float finish, float featherRatio) -> float;

  void durats(uint32_t iSequence, std::vector<fPOINT>* sequence, FEATHER& feather);
  void durec(OREC& record);
  auto dutyp(uint32_t attribute) noexcept -> uint32_t;
  void duxrats(uint32_t start, uint32_t finish, fPOINT& point, float featherRatioLocal) noexcept;

  auto CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept -> BOOL;

  void fangfn(uint32_t formNumber, float angle);
  void fcolfn(uint32_t formNumber, uint32_t color);
  void fhifn(uint32_t formNumber, float length);
  void findfn(uint32_t formNumber, float indent);
  void flenfn(uint32_t formNumber, float length);
  void fmaxfn(uint32_t formNumber, float length);
  void fminfn(uint32_t formNumber, float length);
  void fncwlk(FRMHED& form);
  void fnund(FRMHED& form, std::vector<RNGCNT> const& textureSegments, std::vector<fPOINT>& angledFormVertices);
  void fnwlk(FRMHED& form);
  void fritfil(FRMHED const& form, std::vector<fPOINT>& featherSequence);
  void fspacfn(uint32_t formNumber, float spacing);
  void fthdfn(uint32_t iSequence, FEATHER& feather);
  void fthfn(uint32_t iSequence, FEATHER& feather);
  void fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence);
  void fthvars(FRMHED const& form, FEATHER& feather);
  void fwidfn(uint32_t formNumber, float length);
  auto getstxt(uint32_t stringIndex, HWND dialog) -> float;
  auto gucon(FRMHED const&            form,
             std::vector<fPOINTATTR>& buffer,
             fPOINT const&            start,
             fPOINT const&            finish,
             uint32_t                 destination,
             uint32_t                 code) -> uint32_t;
  auto isfil(FRMHED const& form) noexcept -> bool;
  auto lastcol(uint32_t index, fPOINT& point) noexcept -> bool;
  auto midpnt(fPOINT const& startPoint, fPOINT const& endPoint) noexcept -> fPOINT;
  void notundfn(uint32_t code);
  void nudfn(fRECTANGLE const& designSizeRect);
  void nurat(FEATHER& feather) noexcept;
  auto precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, std::vector<OREC*> const& pRecs, SRTREC const& sortRecord)
      -> double;
  void ratpnt(uint32_t iPoint, uint32_t iNextPoint, fPOINT& point, float featherRatio) noexcept;
  auto orComp(OREC const* record1, OREC const* record2) noexcept -> bool;
  auto orfComp(OREC const* record1, OREC const* record2) noexcept -> bool;
  void rtrclpfn(FRMHED const& form);
  void ritwlk(FRMHED& form, uint32_t walkMask);
  void sadj(fPOINT& point, fPOINT const& designSizeRatio, fRECTANGLE const& designSizeRect) noexcept;
  void sadj(fPOINTATTR& stitch, fPOINT const& designSizeRatio, fRECTANGLE const& designSizeRect) noexcept;

  auto CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  void setstxt(uint32_t stringIndex, float value, HWND dialog);
  void setundfn(uint32_t code);
  auto srtchk(std::vector<OREC*> const& stitchRegion, uint32_t count, uint32_t& badForm) noexcept -> bool;

  constexpr auto tim2int(FILETIME time) noexcept -> ULARGE_INTEGER;

  void uangfn(uint32_t formNumber, float angle);
  void ucolfn(uint32_t formNumber, uint32_t color);
  void ulenfn(uint32_t formNumber, float length);
  void undclp(FRMHED const& form);
  void uspacfn(uint32_t formNumber, float spacing);
  void xratf(fPOINT const& startPoint, fPOINT const& endPoint, fPOINT& point, float featherRatioLocal) noexcept;
} // namespace internal
} // namespace xt
