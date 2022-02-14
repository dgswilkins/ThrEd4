#pragma once

// Local Headers
#include "formHeader.h"

namespace satin {
auto adsatk(uint32_t count) -> uint32_t;
void delcon(FRM_HEAD& form, uint32_t GuideIndex);
void delsac(uint32_t formIndex);
void delspnt();
void drwsat();
void dusat() noexcept;
void ribon();
void satadj(FRM_HEAD& form);
void satbrd();
void satfil(FRM_HEAD& form);
void satfix();
void satknkt();
void satout(FRM_HEAD const& form, float satinWidth);
void satpnt0();
void satpnt1();
void satsel();
void satzum();
void sbrd(FRM_HEAD const& form);
auto scomp(SAT_CON const& arg1, SAT_CON const& arg2) noexcept -> bool;
void slbrd(FRM_HEAD const& form);
void spltsat(uint32_t guideIndex);

namespace internal {
  auto chkbak(std::vector<F_POINT> const& satinBackup, F_POINT const& pnt) noexcept -> bool;
  void filinsbw(std::vector<F_POINT>& satinBackup, F_POINT const& point, uint32_t& satinBackupIndex, F_POINT& stitchPoint);
  auto nusac(uint32_t formIndex, uint32_t guideCount) -> uint32_t;
  void outfn(FRM_HEAD const& form, uint32_t start, uint32_t finish, float satinWidth);
  void sacspac(uint32_t startGuide, uint32_t guideCount);
  void satclos();
  void satcpy(FRM_HEAD const& form, std::vector<SAT_CON> const& source, uint32_t size);
  void satends(FRM_HEAD const& form, uint32_t isBlunt, float width);
  void satfn(FRM_HEAD const&           form,
             std::vector<float> const& lengths,
             uint32_t                  line1Start,
             uint32_t                  line1End,
             uint32_t                  line2Start,
             uint32_t                  line2End);
  void satmf(FRM_HEAD const& form, std::vector<float> const& lengths);
  auto satOffset(const uint32_t& finish, const uint32_t& start, float satinWidth) noexcept -> F_POINT;
  void satsbrd(uint32_t formIndex);
  auto satselfn() -> bool;
  void sbfn(std::vector<F_POINT> const& insidePoints, uint32_t start, uint32_t finish, F_POINT& stitchPoint);
  void sfn(FRM_HEAD const& form, uint32_t startVertex, F_POINT& stitchPoint);
  void unsat();
} // namespace internal
} // namespace satin
