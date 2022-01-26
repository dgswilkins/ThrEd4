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

namespace internal {
  void altx();
  void angrct(F_RECTANGLE& rectangle) noexcept;
  void butsid(uint32_t windowId);
  auto chkbut() -> bool;
  void chktx();
  auto chktxh(_In_ TX_HIST const& historyItem) -> bool;
  void chktxnum();
  void deorg(POINT& point) noexcept;
  void dutxfn(uint32_t textureType);
  void dutxlin(F_POINT const& point0in, F_POINT const& point1in);
  void dutxmir();
  void dutxrct(TXTR_RECT& textureRect);
  void dutxtlin() noexcept;
  void dutxtx(uint32_t index, uint16_t offsetPixels);
  void ed2px(F_POINT const& editPoint, POINT& point) noexcept;
  auto ed2stch(F_POINT const& point) noexcept -> F_POINT;
  void ed2txp(POINT const& offset, TX_PNT& textureRecord);
  auto inrct(F_RECTANGLE const& rectangle, F_POINT_ATTR const& stitch) noexcept -> bool;
  void nutx(FRM_HEAD& form);
  void nxbak();
  void px2ed(POINT const& point, F_POINT& editPoint) noexcept;
  auto px2txt(POINT const& offset) -> bool;
  void redtbak();
  void ritxfrm(FRM_HEAD const& textureForm);
  void ritxrct();
  void setxclp(FRM_HEAD const& form);
  void setxfrm() noexcept;
  void setxmov();
  void stxlin();
  void txang(FRM_HEAD& form);
  void txbak();
  auto txbutfn() -> bool;
  auto tpComp(TX_PNT const& texturePoint0, TX_PNT const& texturePoint1) noexcept -> bool;
  void txcntrv(FRM_HEAD const& textureForm);
  void txdelal();
  auto txdig(wchar_t keyCode, wchar_t& character) noexcept -> bool;
  void txgro(FRM_HEAD const& textureForm);
  void txhor(FRM_HEAD& form);
  auto txnam(std::wstring& name) -> bool;
  void txnudg(int32_t deltaX, float deltaY);
  void txpar(FRM_HEAD& form);
  void txrbak() noexcept;
  void txrct2rct(TXTR_RECT const& textureRect, RECT& rectangle) noexcept;
  void txrfor() noexcept;
  void txshrnk(FRM_HEAD const& textureForm);
  void txsiz(float ratio, FRM_HEAD const& textureForm);
  void txt2pix(TX_PNT const& texturePoint, POINT& screenPoint) noexcept;
  auto txtclos(uint32_t& closestTexturePoint) -> bool;
  void txtclp(FRM_HEAD& textureForm);
  void txtdel();
  void txtlin();
  void txtxfn(POINT const& reference, uint16_t offsetPixels);
  void txvrt(FRM_HEAD& form);
} // namespace internal
} // namespace texture
