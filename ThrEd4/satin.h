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
//#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace satin {

SATCON* adsatk(size_t count) noexcept;
void clearGuideSize();
void cpySat(const FRMHED& formHeader);
void cpyTmpGuides(std::vector<SATCONOUT> &inSatinGuides);
void cpyUndoGuides(const BAKHED& undoData);
void delcon(unsigned GuideIndex);
void delsac(size_t formIndex) noexcept;
void delspnt();
void drwsat();
void dusat() noexcept;
size_t getGuideSize();
void ribon();
void satadj();
void satbrd();
void satfil();
void satfix();
void satknkt();
void satout(double satinWidth);
void satpnt0();
void satpnt1();
void satsel();
void satzum();
void sbrd();
void setGuideSize(size_t newGuideSize);
void slbrd();
void spltsat(const SATCON& currentGuide);
}

namespace satinPriv {

bool     chkbak(const std::vector<dPOINT>& satinBackup, const dPOINT& pnt);
SATCON*  nusac(size_t formIndex, unsigned guideCount) noexcept;
void     outfn(unsigned start, unsigned finish, double satinWidth);
void     sacspac(const SATCON* const startGuide, unsigned guideCount) noexcept;
void     satclos();
void     satcpy(SATCON* const destination, const std::vector<SATCON> source, unsigned int size);
void     satends(unsigned isBlunt);
void     satfn(const std::vector<double>& lengths, size_t line1Start, size_t line1End, size_t line2Start, size_t line2End);
unsigned satind(const SATCON* const guide) noexcept;
void     satmf(const std::vector<double>& lengths);
void     satsbrd();
bool     satselfn() noexcept;
void     sbfn(const std::vector<fPOINT>& insidePoints, size_t start, size_t finish);
void     sfn(size_t startVertex);
void     unsat();
}