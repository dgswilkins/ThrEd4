#pragma once

// Local Headers
#include "clipStitch.h"
#include "formHeader.h"
#include "point.h"

// Standard Libraries
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace tfc {
void clipSelectedForm();
void clipSelectedForms();
void clipSelectedPoints();
void clipSelectedStitches();
auto doPaste(std::vector<POINT> const& stretchBoxLine, bool& retflag) -> bool;
void duclip();
void fpUnClip();
auto getClipForm(LPVOID clipMemory) noexcept -> FRM_HEAD*;
auto getClipFormCount() noexcept -> uint32_t;
void lodclp(uint32_t iStitch);
void rtrclp();
void rtrclpfn(FRM_HEAD const& form);
void savclp(CLIP_STITCH& destination, F_POINT_ATTR const& source, uint32_t led);
void setClipOrigin(POINT source) noexcept;
void setFVAS(std::vector<POINT>* source) noexcept;
void sizclp(FRM_HEAD const& form,
            uint32_t&       formFirstStitchIndex,
            uint32_t&       formStitchCount,
            uint32_t&       length,
            uint32_t&       fileSize) noexcept(std::is_same_v<size_t, uint32_t>);
auto sizfclp(FRM_HEAD const& form) noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t;
void txtclp(FRM_HEAD& textureForm);
} // namespace tfc
