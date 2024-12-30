#pragma once

// Local Headers
#include "formHeader.h"

// Standard Libraries
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace tfc {
auto doPaste(std::vector<POINT> const& stretchBoxLine, bool& retflag) -> bool;
void duclip();
void fClipInit() noexcept;
void fpUnClip();
auto getClipFormCount() noexcept -> uint32_t;
auto getPCDClipMemory() noexcept -> HGLOBAL;
void lodclp(uint32_t iStitch);
void rtrclp();
void setClipOrigin(POINT source) noexcept;
void txtclp(FRM_HEAD& textureForm);
} // namespace tfc
