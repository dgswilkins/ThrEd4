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
} // namespace satin
