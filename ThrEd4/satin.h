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
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
//#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace satin {

uint32_t adsatk(uint32_t count);
void         cpySat(const FRMHED& formHeader);
void         delcon(uint32_t GuideIndex);
void         delsac(uint32_t formIndex);
void         delspnt();
void         drwsat();
void         dusat();
void         ribon();
void         satadj();
void         satbrd();
void         satfil();
void         satfix();
void         satknkt();
void         satout(float satinWidth);
void         satpnt0();
void         satpnt1();
void         satsel();
void         satzum();
void         sbrd();
void         slbrd();
void         spltsat(uint32_t guideIndex);

namespace internal {

	bool chkbak(const std::vector<fPOINT>& satinBackup, const fPOINT& pnt);
	void filinsbw(std::vector<fPOINT>& satinBackup, const fPOINT& point, uint32_t& satinBackupIndex);

	uint32_t nusac(uint32_t formIndex, uint32_t guideCount);

	void     outfn(uint32_t start, uint32_t finish, double satinWidth);
	void     sacspac(uint32_t startGuide, uint32_t guideCount);
	void     satclos();
	void     satcpy(const std::vector<SATCON>& source, uint32_t size);
	void     satends(uint32_t isBlunt);
	void     satfn(const std::vector<double>& lengths,
	               uint32_t               line1Start,
	               uint32_t               line1End,
	               uint32_t               line2Start,
	               uint32_t               line2End);
	uint32_t satind(const SATCON* guide) noexcept;
	void     satmf(const std::vector<double>& lengths);
	void     satsbrd();
	bool     satselfn();
	void     sbfn(const std::vector<fPOINT>& insidePoints, uint32_t start, uint32_t finish);
	void     sfn(uint32_t startVertex);
	void     unsat();
} // namespace internal
} // namespace satin