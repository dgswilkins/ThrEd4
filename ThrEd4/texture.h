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

//void   altx();
//void   angrct(fRECTANGLE& rectangle);
//void   butsid(unsigned windowId);
//bool   chkbut();
//void   chktx();
//bool   chktxh(_In_ const TXHST* historyItem);
//void   chktxnum();
void   deltx();
//void   deorg(POINT& point);
void   drwtxtr();
//void   dutxfn(unsigned textureType);
//void   dutxlin(const fPOINT& point0in, const fPOINT& point1in);
//void   dutxmir();
//void   dutxrct(TXTRCT& textureRect);
void   dutxtfil();
//void   dutxtlin();
//void   dutxtx(size_t index, unsigned short offsetPixels);
//void   ed2px(const fPOINT& editPoint, POINT& point);
//fPOINT ed2stch(const fPOINT& point);
//void   ed2txp(const POINT& offset, TXPNT& textureRecord);
void   initTextures(std::vector<TXPNT>* ptrTexturePoints, std::vector<size_t>* ptrTexturePointsList) noexcept;
//bool   inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch);
bool   istx(size_t iForm);
//bool   istxclp();
//void   nutx();
//void   nxbak();
//void   px2ed(const POINT& point, fPOINT& editPoint);
//bool   px2txt(const POINT& offset);
//void   redtbak();
void   redtx();
//void   ritxfrm();
//void   ritxrct();
void   rstxt();
void   rtrtx();
void   savtxt();
void   setshft();
//void   setxclp();
//void   setxfrm();
//void   setxmov();
void   setxt(std::vector<RNGCNT>& textureSegments);
//void   stxlin();
//void   tritx();
//void   txang();
//void   txbak();
//bool   txbutfn();
//bool   txcmp(const TXPNT& texturePoint0, const TXPNT& texturePoint1);
//void   txcntrv();
//void   txdelal();
//bool   txdig(unsigned keyCode, char& character);
void   txdun();
//void   txgro();
//void   txhor();
//bool   txnam(wchar_t* name, int sizeName);
//void   txnudg(int deltaX, float deltaY);
void   txof();
//void   txpar();
//void   txrbak();
//void   txrct2rct(const TXTRCT& textureRect, RECT& rectangle);
//void   txrfor();
//void   txshrnk();
//void   txsiz(double ratio);
void   txsnap();
//void   txt2pix(const TXPNT& texturePoint, POINT& screenPoint);
//bool   txtclos(size_t& closestTexturePoint);
//void   txtclp();
//void   txtdel();
void   txtkey(unsigned keyCode);
void   txtlbut();
//void   txtlin();
void   txtrbut();
void   txtrmov();
void   txtrup();
//void   txtxfn(const POINT& reference, unsigned short offsetPixels);
//void   txvrt();
void writeScreenWidth(unsigned position);