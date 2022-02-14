#pragma once

// Local Headers
#include "fRectangleHeader.h"
#include "pointHeader.h"

// Standard Libraries
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace PCS { // NOLINT(readability-identifier-naming)
auto readPCSFile(fs::path const& newFileName) -> bool;
auto savePCS(fs::path const* auxName, std::vector<F_POINT_ATTR>& saveStitches) -> bool;
auto insPCS(fs::path const& insertedFile, F_RECTANGLE& insertedRectangle) -> bool;
auto isPCS(fs::path const& path) -> bool;
namespace internal {
  auto pcshup(std::vector<F_POINT_ATTR>& stitches) -> bool;
} // namespace internal
} // namespace PCS
