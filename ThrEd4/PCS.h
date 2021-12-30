#pragma once
#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

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
