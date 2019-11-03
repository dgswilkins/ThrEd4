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

namespace fs = std::filesystem;

namespace xt {

void chgchk(uint8_t code);
void chgwrn();
void chkcwlk(FRMHED& form);
void chkund(FRMHED& form, const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices);
void chkwlk(FRMHED& form);
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
auto insid(const FRMHED& form) -> std::vector<fPOINT>&;
void intlv(const FRMHED& form, const FILLSTARTS& fillStartsData, uint32_t fillStartsMap);
void mvshft();
void notcwlk();
void notund();
void notwlk();
void nudsiz();
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

	void bcolfn(uint32_t find, uint32_t color);
	void blenfn(uint32_t find, float length);
	void bmaxfn(uint32_t find, float length);
	void bminfn(uint32_t find, float length);
	auto bpsg() noexcept -> uint32_t;
	void bspacfn(uint32_t find, float length);
	auto chkasp(fPOINT& point, float aspectRatio, HWND dialog) -> bool;
	void chkend(const FRMHED& form, std::vector<fPOINTATTR>& buffer, uint32_t code, INTINF& ilData);
	auto chkrdun(const std::vector<uint32_t>& formFillCounter,
	             const std::vector<OREC*>&    pRecs,
	             const SRTREC&                stitchRecord) noexcept -> bool;
	void chkuseq(FRMHED& form);
	void delwlk(uint32_t code);

#ifdef _DEBUG
	void dmprec(const std::vector<OREC*>& stitchRegion, uint32_t count);
	void duatf(uint32_t ind);
#endif

	void duint(const FRMHED& form, std::vector<fPOINTATTR>& buffer, uint32_t code, INTINF& ilData);
	auto duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord) -> uint32_t;

	constexpr float durat(float start, float finish, float featherRatio);

	void durats(uint32_t iSequence, std::vector<fPOINT>* sequence, FEATHER& feather);
	void durec(OREC& record) noexcept;
	auto dutyp(uint32_t attribute) noexcept -> uint32_t;
	void duxrats(uint32_t start, uint32_t finish, fPOINT& point, float featherRatioLocal) noexcept;

	auto CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept -> BOOL;

	void fangfn(uint32_t find, float angle);
	void fcolfn(uint32_t find, uint32_t color);
	void fhifn(uint32_t find, float length);
	void findfn(uint32_t find, float indent);
	void flenfn(uint32_t find, float length);
	void fmaxfn(uint32_t find, float length);
	void fminfn(uint32_t find, float length);
	void fncwlk(FRMHED& form);
	void fnund(FRMHED& form, const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices);
	void fnwlk(FRMHED& form);
	void fritfil(std::vector<fPOINT>& featherSequence);
	void fspacfn(uint32_t find, float spacing);
	void fthdfn(uint32_t iSequence, FEATHER& feather);
	void fthfn(uint32_t iSequence, FEATHER& feather);
	void fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence);
	void fthvars(FEATHER& feather);
	void fwidfn(uint32_t find, float length);
	auto getstxt(uint32_t stringIndex, HWND dialog) -> float;
	auto gucon(const FRMHED&            form,
	           std::vector<fPOINTATTR>& buffer,
	           const fPOINT&            start,
	           const fPOINT&            finish,
	           uint32_t                 destination,
	           uint32_t                 code) -> uint32_t;
	auto isfil() noexcept -> bool;
	auto lastcol(uint32_t index, fPOINT& point) noexcept -> bool;
	auto midpnt(const fPOINT& startPoint, const fPOINT& endPoint) noexcept -> fPOINT;
	void notundfn(uint32_t code);
	void nudfn(const fRECTANGLE& designSizeRect);
	void nurat(FEATHER& feather) noexcept;
	auto precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord) -> double;
	void ratpnt(uint32_t iPoint, uint32_t iNextPoint, fPOINT& point, float featherRatio) noexcept;
	auto orComp(const OREC* record1, const OREC* record2) noexcept -> bool;
	auto orfComp(const OREC* record1, const OREC* record2) noexcept -> bool;
	void rtrclpfn();
	void ritcwlk(FRMHED& form);
	void ritund(FRMHED& form);
	void ritwlk(FRMHED& form);
	void sadj(fPOINT& point, const fPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept;
	void sadj(fPOINTATTR& stitch, const fPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept;

	auto CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

	void setstxt(uint32_t stringIndex, float value, HWND dialog);
	void setundfn(uint32_t code);
	auto srtchk(const std::vector<OREC*>& stitchRegion, uint32_t count, uint32_t& badForm) noexcept -> bool;

	constexpr ULARGE_INTEGER tim2int(FILETIME time) noexcept;

	void uangfn(uint32_t find, float angle);
	void ucolfn(uint32_t find, uint32_t color);
	void ulenfn(uint32_t find, float length);
	void undclp(const FRMHED& form);
	void uspacfn(uint32_t find, float spacing);
	void xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal) noexcept;
} // namespace internal
} // namespace xt
