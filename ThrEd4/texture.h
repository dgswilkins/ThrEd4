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

void deltx(unsigned int formIndex);
void drwtxtr();
void dutxtfil();
void initTextures(std::vector<TXPNT>* ptrTexturePoints, std::vector<unsigned int>* ptrTexturePointsList) noexcept;
bool istx(unsigned int iForm);
void redtx();
void rstxt();
void rtrtx();
void savtxt();
void setshft();
void setxt(std::vector<RNGCNT>& textureSegments);
void txdun();
void txof();
void txsnap();
void txtkey(unsigned keyCode, FRMHED& textureForm);
void txtlbut(const FRMHED& textureForm);
void txtrbut();
void txtrmov(const FRMHED& textureForm);
void txtrup();
void writeScreenWidth(unsigned position);

namespace internal {

	void   altx();
	void   angrct(fRECTANGLE& rectangle);
	void   butsid(unsigned windowId);
	bool   chkbut();
	void   chktx();
	bool   chktxh(_In_ const TXHST* historyItem);
	void   chktxnum();
	void   deorg(POINT& point) noexcept;
	void   dutxfn(unsigned textureType);
	void   dutxlin(const fPOINT& point0in, const fPOINT& point1in);
	void   dutxmir();
	void   dutxrct(TXTRCT& textureRect);
	void   dutxtlin() noexcept;
	void   dutxtx(unsigned int index, unsigned short offsetPixels);
	void   ed2px(const fPOINT& editPoint, POINT& point);
	fPOINT ed2stch(const fPOINT& point) noexcept;
	void   ed2txp(const POINT& offset, TXPNT& textureRecord);
	bool   inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch) noexcept;
	void   nutx();
	void   nxbak();
	void   px2ed(const POINT& point, fPOINT& editPoint) noexcept;
	bool   px2txt(const POINT& offset);
	void   redtbak();
	void   ritxfrm(const FRMHED& textureForm);
	void   ritxrct() noexcept;
	void   setxclp(const FRMHED& textureForm);
	void   setxfrm();
	void   setxmov();
	void   stxlin();
	void   txang();
	void   txbak();
	bool   txbutfn();
	bool   txcmp(const TXPNT& texturePoint0, const TXPNT& texturePoint1) noexcept;
	void   txcntrv(const FRMHED& textureForm);
	void   txdelal();
	bool   txdig(unsigned keyCode, char& character) noexcept;
	void   txgro(const FRMHED& textureForm);
	void   txhor();
	bool   txnam(wchar_t* name, int sizeName);
	void   txnudg(int deltaX, float deltaY);
	void   txpar();
	void   txrbak() noexcept;
	void   txrct2rct(const TXTRCT& textureRect, RECT& rectangle);
	void   txrfor() noexcept;
	void   txshrnk(const FRMHED& textureForm);
	void   txsiz(double ratio, const FRMHED& textureForm);
	void   txt2pix(const TXPNT& texturePoint, POINT& screenPoint);
	bool   txtclos(unsigned int& closestTexturePoint);
	void   txtclp(FRMHED& textureForm);
	void   txtdel();
	void   txtlin();
	void   txtxfn(const POINT& reference, unsigned short offsetPixels) noexcept;
	void   txvrt();
} // namespace internal
} // namespace texture