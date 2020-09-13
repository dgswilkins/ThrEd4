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
#include <vector>

// Local Headers
#include "ThrEdTypes.h"

namespace texture {
void deltx(uint32_t formIndex);
void drwtxtr();
void dutxtfil();
void initTextures(std::vector<TXPNT>* ptrTexturePoints, std::vector<uint32_t>* ptrTexturePointsList) noexcept;
auto istx(uint32_t iForm) noexcept -> bool;
auto istx(FRMHED const& form) noexcept -> bool;
void redtx();
void rstxt();
void rtrtx(FRMHED const& form);
void savtxt();
void setshft();
void setxt(FRMHED& form, std::vector<RNGCNT>& textureSegments);
void txdun();
void txof();
void txsnap();
void txtkey(int16_t keyCode, FRMHED& textureForm);
void txtlbut(FRMHED const& textureForm);
void txtrbut();
void txtrmov(FRMHED const& textureForm);
void txtrup();
void writeScreenWidth(int32_t position);

namespace internal {
  void altx();
  void angrct(fRECTANGLE& rectangle) noexcept;
  void butsid(uint32_t windowId);
  auto chkbut() -> bool;
  void chktx();
  auto chktxh(_In_ TXHST const& historyItem) -> bool;
  void chktxnum();
  void deorg(POINT& point) noexcept;
  void dutxfn(uint32_t textureType);
  void dutxlin(fPOINT const& point0in, fPOINT const& point1in);
  void dutxmir();
  void dutxrct(TXTRCT& textureRect);
  void dutxtlin() noexcept;
  void dutxtx(uint32_t index, uint16_t offsetPixels);
  void ed2px(fPOINT const& editPoint, POINT& point) noexcept;
  auto ed2stch(fPOINT const& point) noexcept -> fPOINT;
  void ed2txp(POINT const& offset, TXPNT& textureRecord);
  auto inrct(fRECTANGLE const& rectangle, fPOINTATTR const& stitch) noexcept -> bool;
  void nutx(FRMHED& form);
  void nxbak();
  void px2ed(POINT const& point, fPOINT& editPoint) noexcept;
  auto px2txt(POINT const& offset) -> bool;
  void redtbak();
  void ritxfrm(FRMHED const& textureForm);
  void ritxrct();
  void setxclp(FRMHED const& form);
  void setxfrm() noexcept;
  void setxmov();
  void stxlin();
  void txang(FRMHED& form);
  void txbak();
  auto txbutfn() -> bool;
  auto tpComp(TXPNT const& texturePoint0, TXPNT const& texturePoint1) noexcept -> bool;
  void txcntrv(FRMHED const& textureForm);
  void txdelal();
  auto txdig(int16_t keyCode, wchar_t& character) -> bool;
  void txgro(FRMHED const& textureForm);
  void txhor(FRMHED& form);
  auto txnam(wchar_t* name, int32_t sizeName) -> bool;
  void txnudg(int32_t deltaX, float deltaY);
  void txpar(FRMHED& form);
  void txrbak() noexcept;
  void txrct2rct(TXTRCT const& textureRect, RECT& rectangle) noexcept;
  void txrfor() noexcept;
  void txshrnk(FRMHED const& textureForm);
  void txsiz(float ratio, FRMHED const& textureForm);
  void txt2pix(TXPNT const& texturePoint, POINT& screenPoint) noexcept;
  auto txtclos(uint32_t& closestTexturePoint) -> bool;
  void txtclp(FRMHED& textureForm);
  void txtdel();
  void txtlin();
  void txtxfn(POINT const& reference, uint16_t offsetPixels);
  void txvrt(FRMHED& form);
} // namespace internal
} // namespace texture
