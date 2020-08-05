#pragma once
#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace fs = std::filesystem;

namespace PCS {
auto readPCSFile(fs::path const& newFileName) -> bool;
auto savePCS(fs::path const* auxName, std::vector<fPOINTATTR>& saveStitches) -> bool;
auto insPCS(fs::path const& insertedFile, fRECTANGLE& insertedRectangle) -> bool;
auto isPCS(fs::path const& path) -> bool;
namespace internal {
  auto pcshup(std::vector<fPOINTATTR>& stitches) -> bool;
} // namespace internal
} // namespace PCS
