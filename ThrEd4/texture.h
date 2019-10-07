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
void redtx();
void rstxt();
void rtrtx();
void savtxt();
void setshft();
void setxt(std::vector<RNGCNT>& textureSegments);
void txdun();
void txof();
void txsnap();
void txtkey(uint32_t keyCode, FRMHED& textureForm);
void txtlbut(const FRMHED& textureForm);
void txtrbut();
void txtrmov(const FRMHED& textureForm);
void txtrup();
void writeScreenWidth(uint32_t position);

namespace internal {
	void altx();
	void angrct(fRECTANGLE& rectangle) noexcept;
	void butsid(uint32_t windowId);
	auto chkbut() -> bool;
	void chktx();
	auto chktxh(_In_ const TXHST* historyItem) -> bool;
	void chktxnum();
	void deorg(POINT& point) noexcept;
	void dutxfn(uint32_t textureType);
	void dutxlin(const fPOINT& point0in, const fPOINT& point1in);
	void dutxmir();
	void dutxrct(TXTRCT& textureRect);
	void dutxtlin() noexcept;
	void dutxtx(uint32_t index, uint16_t offsetPixels);
	void ed2px(const fPOINT& editPoint, POINT& point);
	auto ed2stch(const fPOINT& point) noexcept -> fPOINT;
	void ed2txp(const POINT& offset, TXPNT& textureRecord);
	auto inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch) noexcept -> bool;
	void nutx();
	void nxbak();
	void px2ed(const POINT& point, fPOINT& editPoint) noexcept;
	auto px2txt(const POINT& offset) -> bool;
	void redtbak();
	void ritxfrm(const FRMHED& textureForm);
	void ritxrct() noexcept;
	void setxclp(const FRMHED& textureForm);
	void setxfrm();
	void setxmov();
	void stxlin();
	void txang();
	void txbak();
	auto txbutfn() -> bool;
	auto tpComp(const TXPNT& texturePoint0, const TXPNT& texturePoint1) noexcept -> bool;
	void txcntrv(const FRMHED& textureForm);
	void txdelal();
	auto txdig(uint32_t keyCode, char& character) -> bool;
	void txgro(const FRMHED& textureForm);
	void txhor();
	auto txnam(wchar_t* name, int32_t sizeName) -> bool;
	void txnudg(int32_t deltaX, float deltaY);
	void txpar();
	void txrbak() noexcept;
	void txrct2rct(const TXTRCT& textureRect, RECT& rectangle);
	void txrfor() noexcept;
	void txshrnk(const FRMHED& textureForm);
	void txsiz(float ratio, const FRMHED& textureForm);
	void txt2pix(const TXPNT& texturePoint, POINT& screenPoint);
	auto txtclos(uint32_t& closestTexturePoint) -> bool;
	void txtclp(FRMHED& textureForm);
	void txtdel();
	void txtlin();
	void txtxfn(const POINT& reference, uint16_t offsetPixels) noexcept;
	void txvrt();
} // namespace internal
} // namespace texture
