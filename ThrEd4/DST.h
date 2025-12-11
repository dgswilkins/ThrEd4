#ifndef THRED_DST_H
#define THRED_DST_H

// Local Headers
#include "point.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

#pragma pack(push, 1) // make sure that the DST data structures are aligned as per the standard
class DSTREC          // dst stitch record
{
  public:
  uint8_t led {};
  uint8_t mid {};
  uint8_t nd {};
};
#pragma pack(pop)

namespace DST { // NOLINT(readability-identifier-naming)
auto colmatch(COLORREF color) -> uint32_t;
void dstInit() noexcept;
auto readDSTFile(fs::path const& newFileName) -> bool;
auto saveDST(fs::path const& auxName, std::vector<F_POINT_ATTR> const& saveStitches) -> bool;
} // namespace DST

#endif // THRED_DST_H
