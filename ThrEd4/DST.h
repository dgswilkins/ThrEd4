#pragma once

#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace fs = std::filesystem;

#pragma pack(push, 1) // make sure that the DST data structures are aligned as per the standard

// clang-format off
class DSTHED // dst file header
{
  public:
  char desched[3]  {}; // 00  00	description
  char desc[17]    {}; // 03  03
  char recshed[3]  {}; // 20  14	record count
  char recs[8]     {}; // 23  17
  char cohed[3]    {}; // 31  1F
  char co[4]       {}; // 34  22
  char xplushed[3] {}; // 38  26	x+ size
  char xplus[6]    {}; // 41  29
  char xminhed[3]  {}; // 47  2F	x- size
  char xmin[6]     {}; // 50  32
  char yplushed[3] {}; // 56  38
  char yplus[6]    {}; // 59  3B	y+ size
  char yminhed[3]  {}; // 65  41
  char ymin[6]     {}; // 68  44	y- size
  char axhed[3]    {}; // 74  4A
  char ax[7]       {}; // 77  4D
  char ayhed[3]    {}; // 84  54
  char ay[7]       {}; // 87  57
  char mxhed[3]    {}; // 94  5E
  char mx[7]       {}; // 97  61
  char myhed[3]    {}; // 104 68
  char my[7]       {}; // 107 6B
  char pdhed[2]    {}; // 114 72
  char pd[7]       {}; // 116 74
  char eof[1]      {}; // 123 7B
  char res[388]    {}; // 124 7C

  constexpr DSTHED() noexcept = default;
  // DSTHED(DSTHED&&) = default;
  // DSTHED& operator=(DSTHED const& rhs) = default;
  // DSTHED& operator=(DSTHED&&) = default;
  //~DSTHED() = default;
};
// clang-format on

// dst type masks

constexpr auto JMPTYP = 0x830000U;
constexpr auto COLTYP = 0x630000U;
constexpr auto REGTYP = 0x030000U;

class DSTREC // dst stitch record
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
auto readDSTFile(std::filesystem::path const& newFileName) -> bool;
void ritdst(DST_OFFSETS& DSTOffsetData, std::vector<DSTREC>& DSTRecords, std::vector<F_POINT_ATTR> const& stitches);
auto saveDST(fs::path const* auxName, std::vector<F_POINT_ATTR> const& saveStitches) -> bool;
void setColFilename(fs::path* directory) noexcept;
void setRGBFilename(fs::path* directory) noexcept;

namespace internal {
  auto chkdst(DSTHED const* dstHeader) noexcept -> bool;
  auto coldis(COLORREF colorA, COLORREF colorB) -> DWORD;
  auto colfil() -> bool;
  void dstin(uint32_t number, POINT& pout) noexcept;
  void dstran(std::vector<DSTREC>& DSTData);
  auto dtrn(DSTREC* dpnt) -> uint32_t;

  auto dudbits(SIZE const& dif) -> uint32_t;

  void savdst(std::vector<DSTREC>& DSTRecords, uint32_t data);
} // namespace internal
} // namespace DST
