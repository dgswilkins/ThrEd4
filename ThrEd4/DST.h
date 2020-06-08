#pragma once

#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace fs = std::filesystem;

// clang-format off
class DSTHED // dst file header
{
  public:
  char desched[3]  {0}; // 00  00	description  NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char desc[17]    {0}; // 03  03                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char recshed[3]  {0}; // 20  14	record count NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char recs[8]     {0}; // 23  17                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char cohed[3]    {0}; // 31  1F                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char co[4]       {0}; // 34  22                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char xplushed[3] {0}; // 38  26	x+ size      NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char xplus[6]    {0}; // 41  29                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char xminhed[3]  {0}; // 47  2F	x- size      NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char xmin[6]     {0}; // 50  32                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char yplushed[3] {0}; // 56  38                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char yplus[6]    {0}; // 59  3B	y+ size      NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char yminhed[3]  {0}; // 65  41                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char ymin[6]     {0}; // 68  44	y- size      NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char axhed[3]    {0}; // 74  4A                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char ax[7]       {0}; // 77  4D                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char ayhed[3]    {0}; // 84  54                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char ay[7]       {0}; // 87  57                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char mxhed[3]    {0}; // 94  5E                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char mx[7]       {0}; // 97  61                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char myhed[3]    {0}; // 104 68                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char my[7]       {0}; // 107 6B                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char pdhed[2]    {0}; // 114 72                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char pd[7]       {0}; // 116 74                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char eof[1]      {0}; // 123 7B                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  char res[388]    {0}; // 124 7C                NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)

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
  uint8_t led {0U};
  uint8_t mid {0U};
  uint8_t nd {0U};

  constexpr DSTREC() noexcept = default;
  // DSTREC(DSTREC&&) = default;
  // DSTREC& operator=(DSTREC const& rhs) = default;
  // DSTREC& operator=(DSTREC&&) = default;
  //~DSTREC() = default;
};

class DSTOffsets
{
  public:
  POINT Positive {0, 0}; // plus offset written into the destination file header
  POINT Negative {0, 0}; // minus offset written into the destination file header

  constexpr DSTOffsets() noexcept = default;
  // DSTOffsets(DSTOffsets&&) = default;
  // DSTOffsets& operator=(DSTOffsets const& rhs) = default;
  // DSTOffsets& operator=(DSTOffsets&&) = default;
  //~DSTOffsets() = default;
};

namespace DST {
auto colmatch(COLORREF color) -> uint32_t;
auto readDSTFile(std::filesystem::path const& newFileName) -> bool;
void ritdst(DSTOffsets& DSTOffsetData, std::vector<DSTREC>& DSTRecords, std::vector<fPOINTATTR> const& stitches);
auto saveDST(fs::path const* auxName, std::vector<fPOINTATTR> const& saveStitches) -> bool;
void setColFilename(fs::path* directory) noexcept;
void setRGBFilename(fs::path* directory) noexcept;

namespace internal {
  auto chkdst(DSTHED const* dstHeader) noexcept -> bool;
  auto coldis(COLORREF colorA, COLORREF colorB) -> DWORD;
  auto colfil() -> bool;
  void dstin(uint32_t number, POINT& pout) noexcept;
  void dstran(std::vector<DSTREC>& DSTData);
  auto dtrn(DSTREC* dpnt) -> uint32_t;

  constexpr auto dudbits(POINT const& dif) -> uint32_t;

  void savdst(std::vector<DSTREC>& DSTRecords, uint32_t data);
} // namespace internal
} // namespace DST
