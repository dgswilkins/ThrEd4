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

namespace xt {

void chgchk(unsigned char code);
void chgwrn();
void chkcwlk(unsigned& interleaveSequenceIndex2);
void chkund(const std::vector<RNGCNT>& textureSegments, unsigned& interleaveSequenceIndex2);
void chkwlk(unsigned& interleaveSequenceIndex2);
void clrstch() noexcept;

#ifdef _DEBUG
void dmpat();
#endif

void duauxnam();
void dubcol(unsigned color);
void dubit(unsigned bit);
void dublen(float length);
void dubmax(float length);
void dubmin(float length);
void dubspac(float length);
void dufang(float angle);
void dufcol(unsigned color);
void dufhi(float length);
void dufind(float indent);
void duflen(float length);
void dufmax(float length);
void dufmin(float length);
void dufspac(float spacing);
void dufwid(float length);
void dufxang(float angle);
void dundcol(unsigned color);
void dushft();
void duspac(float spacing);
void dusulen(float length);
void fdelstch(FILLSTARTS& fillStartsData, unsigned& fillStartsMap);
void fethr();
void fethrf();
void fsort();
void fthrfn(unsigned& interleaveSequenceIndex2);

std::vector<fPOINT>& insid();

void intlv(const FILLSTARTS& fillStartsData, unsigned fillStartsMap, unsigned interleaveSequenceIndex2);
void mvshft();
void notcwlk();
void notund();
void notwlk();
void nudsiz();
void pes2crd();
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

#ifdef _DEBUG
	void prbug() noexcept;
#endif

	void     bcolfn(unsigned int find, unsigned color);
	void     blenfn(unsigned int find, float length);
	void     bmaxfn(unsigned int find, float length);
	void     bminfn(unsigned int find, float length);
	unsigned bpsg() noexcept;
	void     bspacfn(unsigned int find, float length);
	bool     chkasp(fPOINT& point, float aspectRatio, HWND dialog);
	void     chkend(unsigned offset, unsigned code, INTINF& ilData);
	bool     chkp2cnam(const wchar_t* fileName) noexcept;
	bool     chkrdun(const std::vector<unsigned>& formFillCounter, const std::vector<OREC*>& pRecs, const SRTREC& stitchRecord);
	void     chkuseq(unsigned interleaveSequenceIndex2);
	void     delwlk(unsigned int code);

#ifdef _DEBUG
	void dmprec(const std::vector<OREC*>& stitchRegion, unsigned count);
	void duatf(unsigned ind);
#endif

	void     duint(unsigned offset, unsigned code, INTINF& ilData);
	unsigned duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord);

	constexpr float durat(float start, float finish, float featherRatio);

	void     durats(unsigned int iSequence, fPOINT& point, FEATHER& feather);
	void     durec(OREC& record) noexcept;
	unsigned dutyp(unsigned attribute) noexcept;

	void duxrats(unsigned int start, unsigned int finish, fPOINT& point, float featherRatioLocal);

	BOOL CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept;

	void  fangfn(unsigned int find, float angle);
	void  fcolfn(unsigned int find, unsigned color);
	void  fhifn(unsigned int find, float length);
	int   fil2crd(const fs::path& fileName);
	void  findfn(unsigned int find, float indent);
	void  flenfn(unsigned int find, float length);
	void  fmaxfn(unsigned int find, float length);
	void  fminfn(unsigned int find, float length);
	void  fncwlk(unsigned& interleaveSequenceIndex2);
	void  fnund(const std::vector<RNGCNT>& textureSegments, unsigned int find, unsigned& interleaveSequenceIndex2);
	void  fnwlk(unsigned int find, unsigned& interleaveSequenceIndex2);
	void  fritfil(std::vector<fPOINT>& featherSequence, unsigned& interleaveSequenceIndex2);
	void  fspacfn(unsigned int find, float spacing);
	void  fthdfn(unsigned int iSequence, FEATHER& feather);
	void  fthfn(unsigned int iSequence, FEATHER& feather);
	void  fthrbfn(unsigned int iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence);
	void  fthvars(FEATHER& feather);
	void  fwidfn(unsigned int find, float length);
	float getstxt(unsigned stringIndex, HWND dialog);

	unsigned gucon(const fPOINT& start, const fPOINT& finish, unsigned destination, unsigned code);

	bool   isfil() noexcept;
	bool   lastcol(unsigned index, fPOINT& point);
	fPOINT midpnt(const fPOINT& startPoint, const fPOINT& endPoint) noexcept;
	void   notundfn(unsigned code);
	void   nudfn(const fRECTANGLE& designSizeRect);
	void   nurat(FEATHER& feather);

	double precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord);

	void ratpnt(unsigned int iPoint, unsigned int iNextPoint, fPOINT& point, float featherRatio);
	bool recmp(const OREC* record1, const OREC* record2) noexcept;
	bool refcmp(const OREC* record1, const OREC* record2) noexcept;
	void rtrclpfn();
	void ritcwlk(unsigned& interleaveSequenceIndex2);
	void ritund(unsigned& interleaveSequenceIndex2);
	void ritwlk(unsigned& interleaveSequenceIndex2);
	void sadj(fPOINT& point, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept;
	void sadj(fPOINTATTR& stitch, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept;

	BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void setstxt(unsigned stringIndex, float value, HWND dialog);
	void setundfn(unsigned code);
	bool srtchk(const std::vector<OREC*>& stitchRegion, unsigned count, unsigned& badForm);

	constexpr ULARGE_INTEGER tim2int(FILETIME time) noexcept;

	void uangfn(unsigned int find, float angle);
	void ucolfn(unsigned int find, unsigned color);
	void ulenfn(unsigned int find, float length);
	void undclp();
	void uspacfn(unsigned int find, float spacing);
	void xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal) noexcept;
} // namespace internal
} // namespace xt