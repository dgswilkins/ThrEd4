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
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
//#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace satin {

SATCON*      adsatk(unsigned int count) noexcept;
void         clearGuideSize() noexcept;
void         cpySat(const FRMHED& formHeader);
void         cpyTmpGuides(const std::vector<SATCONOUT>& inSatinGuides);
void         cpyUndoGuides(const BAKHED& undoData);
void         delcon(unsigned GuideIndex);
void         delsac(unsigned int formIndex) noexcept;
void         delspnt();
void         drwsat();
void         dusat() noexcept;
unsigned int getGuideSize() noexcept;
void         ribon();
void         satadj();
void         satbrd();
void         satfil();
void         satfix();
void         satknkt();
void         satout(double satinWidth);
void         satpnt0();
void         satpnt1();
void         satsel();
void         satzum();
void         sbrd();
void         setGuideSize(unsigned int newGuideSize) noexcept;
void         slbrd();
void         spltsat(const SATCON& currentGuide);

namespace internal {

	bool     chkbak(const std::vector<dPOINT>& satinBackup, const dPOINT& pnt);
	SATCON*  nusac(unsigned int formIndex, unsigned guideCount) noexcept;
	void     outfn(unsigned start, unsigned finish, double satinWidth);
	void     sacspac(const SATCON* const startGuide, unsigned guideCount) noexcept;
	void     satclos();
	void     satcpy(SATCON* const destination, const std::vector<SATCON> source, unsigned int size);
	void     satends(unsigned isBlunt);
	void     satfn(const std::vector<double>& lengths,
	               unsigned int               line1Start,
	               unsigned int               line1End,
	               unsigned int               line2Start,
	               unsigned int               line2End);
	unsigned satind(const SATCON* const guide) noexcept;
	void     satmf(const std::vector<double>& lengths);
	void     satsbrd();
	bool     satselfn() noexcept;
	void     sbfn(const std::vector<fPOINT>& insidePoints, unsigned int start, unsigned int finish);
	void     sfn(unsigned int startVertex);
	void     unsat();
} // namespace internal
} // namespace satin