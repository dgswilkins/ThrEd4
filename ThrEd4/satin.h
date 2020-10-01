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
void satout(FRMHED const& form, float satinWidth);
void satpnt0();
void satpnt1();
void satsel();
void satzum();
void sbrd(FRMHED const& form);
auto scomp(SATCON const& arg1, SATCON const& arg2) noexcept -> bool;
void slbrd(FRMHED const& form);
void spltsat(uint32_t guideIndex);

namespace internal {
  auto chkbak(std::vector<fPOINT> const& satinBackup, fPOINT const& pnt) noexcept -> bool;
  void filinsbw(std::vector<fPOINT>& satinBackup, fPOINT const& point, uint32_t& satinBackupIndex, fPOINT& stitchPoint);
  auto nusac(uint32_t formIndex, uint32_t guideCount) -> uint32_t;
  void outfn(FRMHED const& form, uint32_t start, uint32_t finish, float satinWidth);
  void sacspac(uint32_t startGuide, uint32_t guideCount);
  void satclos();
  void satcpy(FRMHED const& form, std::vector<SATCON> const& source, uint32_t size);
  void satends(FRMHED const& form, uint32_t isBlunt, float width);
  void satfn(FRMHED const&             form,
             std::vector<float> const& lengths,
             uint32_t                  line1Start,
             uint32_t                  line1End,
             uint32_t                  line2Start,
             uint32_t                  line2End);
  void satmf(FRMHED const& form, std::vector<float> const& lengths);
  void satsbrd(uint32_t formIndex);
  auto satselfn() -> bool;
  void sbfn(std::vector<fPOINT> const& insidePoints, uint32_t start, uint32_t finish, fPOINT& stitchPoint);
  void sfn(FRMHED const& form, uint32_t startVertex, fPOINT& stitchPoint);
  void unsat();
} // namespace internal
} // namespace satin
