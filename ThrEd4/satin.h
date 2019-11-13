#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
//#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
//#include <vector>

// Open Source headers
#include "warnings.h"

// Local Headers
#include "ThrEdTypes.h"

namespace satin {
auto adsatk(uint32_t count) -> uint32_t;
void delcon(FRMHED& form, uint32_t GuideIndex);
void delsac(uint32_t formIndex);
void delspnt();
void drwsat();
void dusat() noexcept;
void ribon();
void satadj(FRMHED& form);
void satbrd();
void satfil(FRMHED& form);
void satfix();
void satknkt();
void satout(const FRMHED& form, float satinWidth);
void satpnt0();
void satpnt1();
void satsel();
void satzum();
void sbrd(const FRMHED& form);
auto scomp(const SATCON& arg1, const SATCON& arg2) noexcept -> bool;
void slbrd(const FRMHED& form);
void spltsat(uint32_t guideIndex);

namespace internal {
	auto chkbak(const std::vector<fPOINT>& satinBackup, const fPOINT& pnt) noexcept -> bool;
	void filinsbw(std::vector<fPOINT>& satinBackup, const fPOINT& point, uint32_t& satinBackupIndex);
	auto nusac(uint32_t formIndex, uint32_t guideCount) -> uint32_t;
	void outfn(const FRMHED& form, uint32_t start, uint32_t finish, float satinWidth);
	void sacspac(uint32_t startGuide, uint32_t guideCount);
	void satclos();
	void satcpy(const FRMHED& form, const std::vector<SATCON>& source, uint32_t size);
	void satends(const FRMHED& form, uint32_t isBlunt, float width);
	void satfn(const std::vector<float>& lengths, uint32_t line1Start, uint32_t line1End, uint32_t line2Start, uint32_t line2End);
	void satmf(const FRMHED& form, const std::vector<float>& lengths);
	void satsbrd(uint32_t formIndex);
	auto satselfn() -> bool;
	void sbfn(const std::vector<fPOINT>& insidePoints, uint32_t start, uint32_t finish);
	void sfn(const FRMHED& form, uint32_t startVertex);
	void unsat();
} // namespace internal
} // namespace satin
