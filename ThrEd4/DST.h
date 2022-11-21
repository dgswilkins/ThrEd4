#pragma once

// Local Headers
#include "point.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <windows.h> // Win32 Platform SDK main header

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

  // constexpr DSTREC() noexcept = default;
  // DSTREC(DSTREC&&) = default;
  // DSTREC& operator=(DSTREC const& rhs) = default;
  // DSTREC& operator=(DSTREC&&) = default;
  //~DSTREC() = default;
};
#pragma pack(pop)

#pragma pack(push, 1) // make sure that the DST data structures are aligned as per the standard
class DST_OFFSETS
{
  public:
  POINT Positive {}; // plus offset written into the destination file header
  POINT Negative {}; // minus offset written into the destination file header

  constexpr DST_OFFSETS() noexcept = default;
  // DST_OFFSETS(DST_OFFSETS&&) = default;
  // DST_OFFSETS& operator=(DST_OFFSETS const& rhs) = default;
  // DST_OFFSETS& operator=(DST_OFFSETS&&) = default;
  //~DST_OFFSETS() = default;
};
#pragma pack(pop)

namespace DST { // NOLINT(readability-identifier-naming)
auto colmatch(COLORREF color) -> uint32_t;
auto readDSTFile(fs::path const& newFileName) -> bool;
void ritdst(DST_OFFSETS& DSTOffsetData, std::vector<DSTREC>& DSTRecords, std::vector<F_POINT_ATTR> const& stitches);
auto saveDST(fs::path const* auxName, std::vector<F_POINT_ATTR> const& saveStitches) -> bool;
void setColFilename(fs::path* directory) noexcept;
void setRGBFilename(fs::path* directory) noexcept;
} // namespace DST
