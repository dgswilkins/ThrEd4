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

void chgchk(uint8_t code);
void chgwrn();
void chkcwlk();
void chkund(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices);
void chkwlk();
void clrstch() noexcept;

#ifdef _DEBUG
void dmpat();
#endif

void duauxnam();
void dubcol(uint32_t color);
void dubit(uint32_t bit);
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
void fdelstch(FILLSTARTS& fillStartsData, uint32_t& fillStartsMap);
void fethr();
void fethrf();
void fsort();
void fthrfn();

std::vector<fPOINT>& insid();

void intlv(const FILLSTARTS& fillStartsData, uint32_t fillStartsMap);
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

	void     bcolfn(uint32_t find, uint32_t color);
	void     blenfn(uint32_t find, float length);
	void     bmaxfn(uint32_t find, float length);
	void     bminfn(uint32_t find, float length);
	uint32_t bpsg() noexcept;
	void     bspacfn(uint32_t find, float length);
	bool     chkasp(fPOINT& point, float aspectRatio, HWND dialog);
	void     chkend(uint32_t offset, uint32_t code, INTINF& ilData);
	bool     chkp2cnam(const wchar_t* fileName) noexcept;
	bool     chkrdun(const std::vector<uint32_t>& formFillCounter, const std::vector<OREC*>& pRecs, const SRTREC& stitchRecord);
	void     chkuseq();
	void     delwlk(uint32_t code);

#ifdef _DEBUG
	void dmprec(const std::vector<OREC*>& stitchRegion, uint32_t count);
	void duatf(uint32_t ind);
#endif

	void     duint(uint32_t offset, uint32_t code, INTINF& ilData);
	uint32_t duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord);

	constexpr float durat(float start, float finish, float featherRatio);

	void     durats(uint32_t iSequence, std::vector<fPOINT>* sequence, FEATHER& feather);
	void     durec(OREC& record) noexcept;
	uint32_t dutyp(uint32_t attribute) noexcept;

	void duxrats(uint32_t start, uint32_t finish, fPOINT& point, float featherRatioLocal);

	BOOL CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept;

	void  fangfn(uint32_t find, float angle);
	void  fcolfn(uint32_t find, uint32_t color);
	void  fhifn(uint32_t find, float length);
	int32_t   fil2crd(const fs::path& fileName);
	void  findfn(uint32_t find, float indent);
	void  flenfn(uint32_t find, float length);
	void  fmaxfn(uint32_t find, float length);
	void  fminfn(uint32_t find, float length);
	void  fncwlk();
	void  fnund(const std::vector<RNGCNT>& textureSegments, uint32_t find, std::vector<fPOINT>& angledFormVertices);
	void  fnwlk(uint32_t find);
	void  fritfil(std::vector<fPOINT>& featherSequence);
	void  fspacfn(uint32_t find, float spacing);
	void  fthdfn(uint32_t iSequence, FEATHER& feather);
	void  fthfn(uint32_t iSequence, FEATHER& feather);
	void  fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence);
	void  fthvars(FEATHER& feather);
	void  fwidfn(uint32_t find, float length);
	float getstxt(uint32_t stringIndex, HWND dialog);

	uint32_t gucon(const fPOINT& start, const fPOINT& finish, uint32_t destination, uint32_t code);

	bool   isfil() noexcept;
	bool   lastcol(uint32_t index, fPOINT& point);
	fPOINT midpnt(const fPOINT& startPoint, const fPOINT& endPoint) noexcept;
	void   notundfn(uint32_t code);
	void   nudfn(const fRECTANGLE& designSizeRect);
	void   nurat(FEATHER& feather);

	double precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord);

	void ratpnt(uint32_t iPoint, uint32_t iNextPoint, fPOINT& point, float featherRatio);
	bool recmp(const OREC* record1, const OREC* record2) noexcept;
	bool refcmp(const OREC* record1, const OREC* record2) noexcept;
	void rtrclpfn();
	void ritcwlk();
	void ritund();
	void ritwlk();
	void sadj(fPOINT& point, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept;
	void sadj(fPOINTATTR& stitch, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept;

	BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void setstxt(uint32_t stringIndex, float value, HWND dialog);
	void setundfn(uint32_t code);
	bool srtchk(const std::vector<OREC*>& stitchRegion, uint32_t count, uint32_t& badForm);

	constexpr ULARGE_INTEGER tim2int(FILETIME time) noexcept;

	void uangfn(uint32_t find, float angle);
	void ucolfn(uint32_t find, uint32_t color);
	void ulenfn(uint32_t find, float length);
	void undclp();
	void uspacfn(uint32_t find, float spacing);
	void xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal) noexcept;
} // namespace internal
} // namespace xt