#pragma once

// Local Headers
#include "formHeader.h"
#include "fRectangle.h"

// Standard Libraries
#include <cstdint>

namespace clip {
void chnfn(FRM_HEAD const& form);
void clpbrd(FRM_HEAD const& form, F_RECTANGLE const& clipRect, uint32_t startVertex);
void clpic(FRM_HEAD const& form, F_RECTANGLE const& clipRect);
void clpout(float width);
void delclps(uint32_t formIndex);
void deleclp(uint32_t formIndex);
void delmclp(uint32_t formIndex);
void duxclp(FRM_HEAD const& form);
auto nueclp(uint32_t currentForm, uint32_t count) -> uint32_t;
auto numclp(uint32_t formIndex) -> uint32_t;
void oclp(F_RECTANGLE& clipRect, uint32_t clipIndex, uint32_t clipEntries);
} // namespace clip
