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

void   altx();
void   angrct(fRECTANGLE& rectangle) noexcept;
void   butsid(unsigned windowId);
bool   chkbut();
void   chktx();
bool   chktxh(_In_ const TXHST* historyItem);
void   chktxnum();
void   deltx();
void   deorg(POINT& point) noexcept;
void   drwtxtr();
void   dutxfn(unsigned textureType);
void   dutxlin(const fPOINT& point0in, const fPOINT& point1in);
void   dutxmir();
void   dutxrct(TXTRCT& textureRect);
void   dutxtfil();
void   dutxtlin() noexcept;
void   dutxtx(size_t index, unsigned short offsetPixels);
void   ed2px(const fPOINT& editPoint, POINT& point) noexcept;
fPOINT ed2stch(const fPOINT& point) noexcept;
void   ed2txp(const POINT& offset, TXPNT& textureRecord) noexcept;
void   initTextures(std::vector<TXPNT>* ptrTexturePoints, std::vector<size_t>* ptrTexturePointsList) noexcept;
bool   inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch) noexcept;
bool   istx(size_t iForm) noexcept;
bool   istxclp();
void   nutx();
void   nxbak();
void   px2ed(const POINT& point, fPOINT& editPoint) noexcept;
bool   px2txt(const POINT& offset);
void   redtbak();
void   redtx();
void   ritxfrm();
void   ritxrct() noexcept;
void   rstxt();
void   rtrtx();
void   savtxt();
void   setshft();
void   setxclp();
void   setxfrm() noexcept;
void   setxmov();
void   setxt(std::vector<RNGCNT>& textureSegments);
void   stxlin();
void   tritx() noexcept;
void   txang();
void   txbak();
bool   txbutfn();
bool   txcmp(const TXPNT& texturePoint0, const TXPNT& texturePoint1) noexcept;
void   txcntrv();
void   txdelal();
bool   txdig(unsigned keyCode, char& character);
void   txdun();
void   txgro();
void   txhor();
bool   txnam(wchar_t* name, int sizeName);
void   txnudg(int deltaX, float deltaY);
void   txof();
void   txpar();
void   txrbak() noexcept;
void   txrct2rct(const TXTRCT& textureRect, RECT& rectangle) noexcept;
void   txrfor() noexcept;
void   txshrnk();
void   txsiz(double ratio);
void   txsnap();
void   txt2pix(const TXPNT& texturePoint, POINT& screenPoint) noexcept;
bool   txtclos(size_t& closestTexturePoint);
void   txtclp();
void   txtdel();
void   txtkey(unsigned keyCode);
void   txtlbut();
void   txtlin();
void   txtrbut();
void   txtrmov();
void   txtrup();
void   txtxfn(const POINT& reference, unsigned short offsetPixels) noexcept;
void   txvrt();
void   writeScreenWidth(unsigned position);
