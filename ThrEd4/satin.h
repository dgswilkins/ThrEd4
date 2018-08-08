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

bool     chkbak(const std::vector<dPOINT>& satinBackup, const dPOINT& pnt);
void     delcon(unsigned GuideIndex);
void     delsac(size_t formIndex) noexcept;
void     delspnt();
void     drwsat();
void     dusat() noexcept;
SATCON*  nusac(size_t formIndex, unsigned guideCount) noexcept;
void     outfn(unsigned start, unsigned finish, double satinWidth);
void     ribon();
void     sacspac(const SATCON* const startGuide, unsigned guideCount) noexcept;
void     satadj();
void     satbrd();
void     satclos();
void     satcpy(SATCON* const destination, const std::vector<SATCON> source, unsigned int size);
void     satends(unsigned isBlunt);
void     satfil();
void     satfix();
void     satfn(const std::vector<double>& lengths, size_t line1Start, size_t line1End, size_t line2Start, size_t line2End);
unsigned satind(const SATCON* const guide) noexcept;
void     satknkt();
void     satmf(const std::vector<double>& lengths);
void     satout(double satinWidth);
void     satpnt0();
void     satpnt1();
void     satsbrd();
void     satsel();
bool     satselfn() noexcept;
void     satzum();
void     sbfn(const std::vector<fPOINT>& insidePoints, size_t start, size_t finish);
void     sbrd();
void     sfn(size_t startVertex);
void     slbrd();
void     spltsat(const SATCON& currentGuide);
void     unsat();
