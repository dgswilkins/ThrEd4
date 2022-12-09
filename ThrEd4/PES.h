#pragma once

// Local Headers
#include "switches.h"

#ifdef PESACT
// Local Headers
#include "point.h"

// Standard Libraries
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace PES { // NOLINT(readability-identifier-naming)
auto readPESFile(fs::path const& newFileName) -> bool;
auto savePES(fs::path const& auxName, std::vector<F_POINT_ATTR> const& saveStitches) -> bool;
} // namespace PES
#endif
