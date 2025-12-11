#ifndef THRED_PCS_H
#define THRED_PCS_H

// Local Headers
#include "fRectangle.h"
#include "point.h"

// Standard Libraries
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace PCS { // NOLINT(readability-identifier-naming)
auto insPCS(fs::path const& insertedFile, F_RECTANGLE& insertedRectangle) -> bool;
auto isPCS(fs::path const& path) -> bool;
auto readPCSFile(fs::path const& newFileName) -> bool;
auto savePCS(fs::path const& auxName, std::vector<F_POINT_ATTR>& saveStitches) -> bool;
} // namespace PCS

#endif // THRED_PCS_H
