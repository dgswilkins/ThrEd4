#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <filesystem>
#include <vector>

#include "ThrEdTypes.h"

namespace fs = std::experimental::filesystem;

void setfchk();

#ifdef _DEBUG
void prbug();
#endif

void     altx();
void     angrct(fRECTANGLE& rectangle);
void     bcolfn(size_t find, unsigned color);
void     bcup(size_t find, BADCNTS& badData);
void     blenfn(size_t find, float length);
void     bmaxfn(size_t find, float length);
void     bminfn(size_t find, float length);
unsigned bpsg();
void     bspacfn(size_t find, float length);
void     butsid(unsigned windowId);
void     bxtxt(unsigned iButton, unsigned iMessage);
void     chgchk(int code);
void     chgwrn();
bool     chkasp(fPOINT& point, float aspectRatio, HWND dialog);
bool     chkbut();
void     chkclp(const FRMHED* const formHeader, BADCNTS& badData);
void     chkcwlk(unsigned& interleaveSequenceIndex2);
void     chkdaz();
void     chkeclp(const FRMHED* const formHeader, BADCNTS& badData);
void     chkend(unsigned offset, unsigned code, INTINF& ilData);
void     chkfstch();
bool     chkp2cnam(const wchar_t* fileName);
bool     chkrdun(const std::vector<unsigned>& formFillCounter, const std::vector<OREC*>& pRecs, const SRTREC& stitchRecord);
void     chktx();
bool     chktxh(_In_ const TXHST* historyItem);
void     chktxnum();
void     chkund(const std::vector<RNGCNT>& textureSegments, unsigned& interleaveSequenceIndex2);
void     chkuseq(const unsigned interleaveSequenceIndex2);
void     chkwlk(unsigned& interleaveSequenceIndex2);
void     clrstch();
void     dasyfrm();

BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

void dazdef();
void deltx();
void delwlk(size_t code);
void deorg(POINT& point);

#ifdef _DEBUG
void dmpat();
#endif

#ifdef _DEBUG
void dmprec(const std::vector<OREC*>& stitchRegion, unsigned count);
#endif

void drwtxbut();
void drwtxtr();

#ifdef _DEBUG
void duatf(unsigned ind);
#endif

void     duauxnam();
void     dubcol(unsigned color);
void     dubit(unsigned bit);
void     dublen(float length);
void     dubmax(float length);
void     dubmin(float length);
void     dubspac(float length);
void     dufang(float angle);
void     dufcol(unsigned color);
void     dufhi(float length);
void     dufind(float indent);
void     duflen(float length);
void     dufmax(float length);
void     dufmin(float length);
void     dufspac(float spacing);
void     dufwid(float length);
void     dufxang(float angle);
void     duint(unsigned offset, unsigned code, INTINF& ilData);
void     dundcol(unsigned color);
unsigned duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord);

constexpr float durat(float start, float finish, float featherRatio);

void     durats(size_t iSequence, fPOINT& point, FEATHER& feather);
void     durec(OREC& record);
void     dushft();
void     duspac(float spacing);
void     dusulen(float length);
void     dutxfn(unsigned textureType);
void     dutxlin(const fPOINT& point0in, const fPOINT& point1in);
void     dutxmir();
void     dutxrct(TXTRCT& textureRect);
void     dutxtfil();
void     dutxtlin();
void     dutxtx(int index, int offsetPixels);
unsigned dutyp(unsigned attribute);

constexpr float duxrat(float strt, float fin, float featherRatioLocal);

void   duxrats(size_t start, size_t finish, fPOINT& point, float featherRatioLocal);
void   ed2px(const fPOINT& editPoint, POINT& point);
fPOINT ed2stch(const fPOINT& point);
void   ed2txp(const POINT& offset, TXPNT& textureRecord);

BOOL CALLBACK enumch(HWND hwnd, LPARAM lParam);

void     fangfn(size_t find, float angle);
void     fcolfn(size_t find, unsigned color);
void     fdelstch(FILLSTARTS& fillStartsData, unsigned& fillStartsMap);
void     fethr();
void     fethrf();
void     fhifn(size_t find, float length);
int      fil2crd(const fs::path& fileName);
void     findfn(size_t find, float indent);
void     flenfn(size_t find, float length);
void     fmaxfn(size_t find, float length);
void     fminfn(size_t find, float length);
void     fncwlk(unsigned& interleaveSequenceIndex2);
void     fnund(const std::vector<RNGCNT>& textureSegments, size_t find, unsigned& interleaveSequenceIndex2);
void     fnwlk(size_t find, unsigned& interleaveSequenceIndex2);
void     fritfil(std::vector<fPOINT>& featherSequence, unsigned& interleaveSequenceIndex2);
unsigned frmchkfn();
void     frmchkx();
void     fsort();
void     fspacfn(size_t find, float spacing);
void     fthdfn(size_t iSequence, FEATHER& feather);
void     fthfn(size_t iSequence, FEATHER& feather);
void     fthrbfn(size_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence);
void     fthrfn(unsigned& interleaveSequenceIndex2);
void     fthvars(FEATHER& feather);
void     fwidfn(size_t find, float length);
float    getstxt(unsigned stringIndex, HWND dialog);
unsigned gucon(const fPOINT& start, const fPOINT& finish, unsigned destination, unsigned code);
void     hlpflt(unsigned iButton, unsigned iMessage, float data);
void     initdaz(HWND hWinDialog);
bool     inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch);

std::vector<fPOINT>& insid();

void   intlv(const FILLSTARTS& fillStartsData, unsigned fillStartsMap, const unsigned interleaveSequenceIndex2);
bool   isfil();
bool   istx(size_t iForm);
bool   istxclp();
bool   lastcol(unsigned index, fPOINT& point);
void   lodchk();
fPOINT midpnt(const fPOINT& startPoint, const fPOINT& endPoint);
void   mvshft();
void   notcwlk();
void   notund();
void   notundfn(unsigned code);
void   notwlk();
void   nudfn(const fRECTANGLE& designSizeRect);
void   nudsiz();
void   nurat(FEATHER& feather);
void   nutx();
void   nxbak();
void   pes2crd();

double precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord);

void px2ed(const POINT& point, fPOINT& editPoint);
bool px2txt(const POINT& offset);
void ratpnt(size_t iPoint, size_t iNextPoint, fPOINT& point, float featherRatio);
bool recmp(const OREC* record1, const OREC* record2);
void redtbak();
void redtx();
bool refcmp(const OREC* record1, const OREC* record2);
void repar();
void repclp(std::wstring& repairMessage);
void repflt(std::wstring& repairMessage);
void repsat();
void reptx();
void ritcwlk(unsigned& interleaveSequenceIndex2);
void ritund(unsigned& interleaveSequenceIndex2);
void ritwlk(unsigned& interleaveSequenceIndex2);
void ritxfrm();
void ritxrct();
void rstxt();
void rtrclp();
void rtrtx();
void sadj(fPOINT& point, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect);
void sadj(fPOINTATTR& stitch, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect);
void savtxt();
void selalfrm();
void setbcol();
void setblen();
void setbmax();
void setbmin();
void setbspac();
void setclpspac();
void setcwlk();
void setfang();
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
void setshft();

BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

void setstxt(unsigned stringIndex, float value, HWND dialog);
void setuang();
void setucol();
void setulen();
void setund();
void setundfn(unsigned code);
void setuspac();
void setwlk();
void setwlkind();
void setxclp();
void setxfrm() noexcept;
void setxmov();
void setxt(std::vector<RNGCNT>& textureSegments);
void sfuang();
bool srtchk(const std::vector<OREC*>& stitchRegion, unsigned count, unsigned& badForm);
void srtcol();
void stxlin();

constexpr ULARGE_INTEGER tim2int(FILETIME time);

void tritx();
void tst();
void txang();
void txbak();
bool txbutfn();
bool txcmp(const TXPNT& texturePoint0, const TXPNT& texturePoint1);
void txcntrv();
void txdelal();
bool txdig(unsigned keyCode, char& character);
void txdun();
void txgro();
void txhor();
bool txnam(wchar_t* name, int sizeName);
void txnudg(int deltaX, float deltaY);
void txof();
void txpar();
void txrbak();
void txrct2rct(const TXTRCT& textureRect, RECT& rectangle);
void txrfor();
void txshrnk();
void txsiz(double ratio);
void txsnap();
void txt2pix(const TXPNT& texturePoint, POINT& screenPoint);
bool txtclos(unsigned& closestTexturePoint);
void txtclp();
void txtdel();
void txtkey(unsigned keyCode);
void txtlbut();
void txtlin();
void txtrbut();
void txtrmov();
void txtrup();
void txtxfn(const POINT& reference, int offsetPixels);
void txvrt();
void uangfn(size_t find, float angle);
void ucolfn(size_t find, unsigned color);
void ulenfn(size_t find, float length);
void undclp();
void undlen();
void uspac();
void uspacfn(size_t find, float spacing);
void xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal);
