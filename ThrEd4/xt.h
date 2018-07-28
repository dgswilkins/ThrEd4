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

namespace fs = std::experimental::filesystem;


#ifdef _DEBUG
void prbug();
#endif

//void     bcolfn(size_t find, unsigned color);
//void     blenfn(size_t find, float length);
//void     bmaxfn(size_t find, float length);
//void     bminfn(size_t find, float length);
//unsigned bpsg();
//void     bspacfn(size_t find, float length);
void     chgchk(int code);
void     chgwrn();
//bool     chkasp(fPOINT& point, float aspectRatio, HWND dialog);
void     chkcwlk(unsigned& interleaveSequenceIndex2);
//void     chkdaz();
//void     chkend(unsigned offset, unsigned code, INTINF& ilData);
//bool     chkp2cnam(const wchar_t* fileName);
//bool     chkrdun(const std::vector<unsigned>& formFillCounter, const std::vector<OREC*>& pRecs, const SRTREC& stitchRecord);
void     chkund(const std::vector<RNGCNT>& textureSegments, unsigned& interleaveSequenceIndex2);
//void     chkuseq(const unsigned interleaveSequenceIndex2);
void     chkwlk(unsigned& interleaveSequenceIndex2);
void     clrstch();
void     dasyfrm();

//BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

void dazdef();
//void delwlk(size_t code);

#ifdef _DEBUG
void dmpat();
#endif

#ifdef _DEBUG
void dmprec(const std::vector<OREC*>& stitchRegion, unsigned count);
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
//void     duint(unsigned offset, unsigned code, INTINF& ilData);
void     dundcol(unsigned color);
//unsigned duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord);

//constexpr float durat(float start, float finish, float featherRatio);

//void     durats(size_t iSequence, fPOINT& point, FEATHER& feather);
//void     durec(OREC& record);
void     dushft();
void     duspac(float spacing);
void     dusulen(float length);
//unsigned dutyp(unsigned attribute);

//constexpr float duxrat(float strt, float fin, float featherRatioLocal);

//void   duxrats(size_t start, size_t finish, fPOINT& point, float featherRatioLocal);

//BOOL CALLBACK enumch(HWND hwnd, LPARAM lParam);

//void     fangfn(size_t find, float angle);
//void     fcolfn(size_t find, unsigned color);
void     fdelstch(FILLSTARTS& fillStartsData, unsigned& fillStartsMap);
void     fethr();
void     fethrf();
//void     fhifn(size_t find, float length);
//int      fil2crd(const fs::path& fileName);
//void     findfn(size_t find, float indent);
//void     flenfn(size_t find, float length);
//void     fmaxfn(size_t find, float length);
//void     fminfn(size_t find, float length);
//void     fncwlk(unsigned& interleaveSequenceIndex2);
//void     fnund(const std::vector<RNGCNT>& textureSegments, size_t find, unsigned& interleaveSequenceIndex2);
//void     fnwlk(size_t find, unsigned& interleaveSequenceIndex2);
//void     fritfil(std::vector<fPOINT>& featherSequence, unsigned& interleaveSequenceIndex2);
void     fsort();
//void     fspacfn(size_t find, float spacing);
//void     fthdfn(size_t iSequence, FEATHER& feather);
//void     fthfn(size_t iSequence, FEATHER& feather);
//void     fthrbfn(size_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence);
void     fthrfn(unsigned& interleaveSequenceIndex2);
//void     fthvars(FEATHER& feather);
//void     fwidfn(size_t find, float length);
//float    getstxt(unsigned stringIndex, HWND dialog);
//unsigned gucon(const fPOINT& start, const fPOINT& finish, unsigned destination, unsigned code);
//void     initdaz(HWND hWinDialog);

std::vector<fPOINT>& insid();

void   intlv(const FILLSTARTS& fillStartsData, unsigned fillStartsMap, const unsigned interleaveSequenceIndex2);
//bool   isfil();
//bool   lastcol(unsigned index, fPOINT& point);
void   lodchk();
//fPOINT midpnt(const fPOINT& startPoint, const fPOINT& endPoint);
void   mvshft();
void   notcwlk();
void   notund();
//void   notundfn(unsigned code);
void   notwlk();
//void   nudfn(const fRECTANGLE& designSizeRect);
void   nudsiz();
//void   nurat(FEATHER& feather);
void   pes2crd();

//double precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord);

//void ratpnt(size_t iPoint, size_t iNextPoint, fPOINT& point, float featherRatio);
//bool recmp(const OREC* record1, const OREC* record2);
//bool refcmp(const OREC* record1, const OREC* record2);
//void ritcwlk(unsigned& interleaveSequenceIndex2);
//void ritund(unsigned& interleaveSequenceIndex2);
//void ritwlk(unsigned& interleaveSequenceIndex2);
void rtrclp();
//void sadj(fPOINT& point, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect);
//void sadj(fPOINTATTR& stitch, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect);
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

//BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

//void setstxt(unsigned stringIndex, float value, HWND dialog);
void setuang();
void setucol();
void setulen();
void setund();
//void setundfn(unsigned code);
void setuspac();
void setwlk();
void setwlkind();
void sfuang();
//bool srtchk(const std::vector<OREC*>& stitchRegion, unsigned count, unsigned& badForm);
void srtcol();
//void stxlin();

//constexpr ULARGE_INTEGER tim2int(FILETIME time);

void tst();
//void uangfn(size_t find, float angle);
//void ucolfn(size_t find, unsigned color);
//void ulenfn(size_t find, float length);
//void undclp();
void undlen();
void uspac();
//void uspacfn(size_t find, float spacing);
//void xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal);
