#pragma once

// Local Headers
#include "pointHeader.h"
#include "switches.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <array>
#include <cstdint>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

#ifdef PESACT

static constexpr uint8_t THUMBHGT = 38U;
static constexpr uint8_t THUMBWID = 48U;
using imgArray                    = std::array<std::array<uint8_t, THUMBWID>, THUMBHGT>;

#pragma pack(push, 1) // make sure that the PES data structures are aligned on byte boundaries

class PESLED
{
  public:
  int8_t   ver[8] {};
  uint32_t pec {};

  constexpr PESLED() noexcept = default;
  // PESLED(PESLED&&) = default;
  // PESLED& operator=(PESLED const& rhs) = default;
  // PESLED& operator=(PESLED&&) = default;
  //~PESLED() = default;
};

// clang-format off
class PESHED
{
  public:
  char     led[8]  {}; //   0-7  Identification and version (#PES0001)         
  uint32_t off     {}; //   8-b  Absolute PEC section byte offset
  uint16_t hpsz    {}; //   c,d  Hoopsize (0), 0 = 100x100mm, 1 = 130x180mm
  uint16_t usdn    {}; //   e,f  Use existing design area (1)
  uint16_t blct    {}; // 10,11  CSewSeg segment block count (1)
  uint8_t  hnd1[4] {}; // 12-15  header end (FF FF 00 00)                      
  uint16_t celn    {}; // 16,17  Length of following string (7)
  char     ce[7]   {}; // 18-1e  CEmbOne identification (CEmbOne)              
  int16_t  xlft    {}; // 1f,20  Extent left
  int16_t  xtop    {}; // 21,22  Extent top
  int16_t  xrht    {}; // 23,24  Extent right
  int16_t  xbot    {}; // 25,26  Extent bottom
  int16_t  plft    {}; // 27,28  Extent left position
  int16_t  ptop    {}; // 29,2a  Extent top position
  int16_t  prht    {}; // 2b,2c  Extent right position
  int16_t  pbot    {}; // 2d,2e  Extent bottom position
  float    atfm1   {}; // 2f-32  Affine transform Scale X (1.0F) (00 00 80 3f)
  float    atfm2   {}; // 33-36  Affine transform Skew X (0.0F) (00 00 00 00)
  float    atfm3   {}; // 37-3a  Affine transform Skew Y (0.0F) (00 00 00 00)
  float    atfm4   {}; // 3b-3e  Affine transform Scale Y (1.0F) (00 00 80 3f)
  float    atfm5   {}; // 3f-42  Affine transform Left_Pos
  float    atfm6   {}; // 43-46  Affine transform Bottom_Pos
  uint16_t ukn1    {}; // 47,48  unknown (1)
  int16_t  xtrn    {}; // 49,4a  CSewSeg x coordinate translation (0)
  int16_t  ytrn    {}; // 4b,4c  CSewSeg y coordinate translation (0)
  int16_t  xsiz    {}; // 4d,4e  CSewSeg width
  int16_t  ysiz    {}; // 4f,50  CSewSeg height
  int8_t   ukn2[8] {}; // 51,58  unknown (0)                                   
  uint16_t bcnt    {}; // 59,5a  CSewSeg block count (segments + (2*colorChanges))
  uint8_t  hnd2[4] {}; // 5b-5e  header end (FF FF 00 00)                      
  uint16_t cslen   {}; // 5f,60  CSewSeg length (7)
  char     cs[7]   {}; // 61-67  CSewSeg identification (CSewSeg)              
// uint16_t styp1;     // 68,69  Stitch type (0)
// uint16_t scol;      // 6a,6b  Stitch Palette thread index

  constexpr PESHED() noexcept = default;
  // PESHED(PESHED&&) = default;
  // PESHED& operator=(PESHED const& rhs) = default;
  // PESHED& operator=(PESHED&&) = default;
  //~PESHED() = default;
};
// clang-format on

class PESTCH
{
  public:
  int16_t x {};
  int16_t y {};

  constexpr PESTCH() noexcept = default;
  // PESTCH(PESTCH&&) = default;
  // PESTCH& operator=(PESTCH const& rhs) = default;
  // PESTCH& operator=(PESTCH&&) = default;
  //~PESTCH() = default;
};

class PESSTCHLST
{
  public:
  uint16_t stitchtype {};
  uint16_t threadIndex {};
  uint16_t stitchcount {};

  // constexpr PESSTCHLST() noexcept = default;
  // PESSTCHLST(PESSTCHLST&&) = default;
  // PESSTCHLST& operator=(PESSTCHLST const& rhs) = default;
  // PESSTCHLST& operator=(PESSTCHLST&&) = default;
  //~PESSTCHLST() = default;
};

class PECHDR
{
  public:
  char     label[19] {};  // Label string prefixed with "LA:" and padded with space (0x20)
  int8_t   labnd {};      // carriage return character
  uint8_t  ukn1[11] {};   // Unknown (' ')
  uint8_t  ukn2 {};       // Unknown
  uint16_t hnd1 {};       // Unknown (0x00ff)
  uint8_t  thumbWidth {}; // Thumbnail image width in bytes (6) , with 8 bit pixels per byte
                          // Thus, 6 would mean 6×8 = 48 pixels per line
  uint8_t thumbHeight {}; // Thumbnail image height in pixels (38)
  uint8_t ukn3[12] {};    // Unknown, usually 20 20 20 20 64 20 00 20 00 20 20 20
  uint8_t colorCount {};  // Number of colors minus one, 0xFF means 0 colors
  uint8_t pad[463] {};    // Pad bytes up to 512.

  constexpr PECHDR() noexcept = default;
  // PECHDR(PECHDR&&) = default;
  // PECHDR& operator=(PECHDR const& rhs) = default;
  // PECHDR& operator=(PECHDR&&) = default;
  //~PECHDR() = default;
};

class PECHDR2
{
  public:
  uint16_t unknown1 {}; // typical 0x0000
  uint16_t thumbnailOffset {}; // Offset to thumbnail image subsection relative to the PEC section offset plus 512 bytes
  uint16_t unknown2 {}; // typical 0x3100
  uint16_t unknown3 {}; // typical 0xf0ff
  int16_t  width {};    // Width
  int16_t  height {};   // height
  uint16_t unknown4 {}; // typical 0x01e0
  uint16_t unknown5 {}; // typical 0x01b0
  uint16_t xMin {};     // starting X val (Bigendian)
  uint16_t yMin {};     // starting Y val (Bigendian)

  constexpr PECHDR2() noexcept = default;
  // PECHDR2(PECHDR2&&) = default;
  // PECHDR2& operator=(PECHDR2 const& rhs) = default;
  // PECHDR2& operator=(PECHDR2&&) = default;
  //~PECHDR2() = default;
};

#pragma pack(pop)

namespace PES { // NOLINT(readability-identifier-naming)
auto readPESFile(fs::path const& newFileName) -> bool;
auto savePES(fs::path const* auxName, std::vector<F_POINT_ATTR> const& saveStitches) -> bool;
namespace internal {
  auto dupcol(uint32_t activeColor, uint32_t& index) -> uint32_t;
  void pecEncodeInt(std::vector<uint8_t>& buffer, int32_t delta);
  void pecEncodeStop(std::vector<uint8_t>& buffer, uint8_t val);
  void pecImage(std::vector<uint8_t>& pecBuffer);
  void pecdat(std::vector<uint8_t>& buffer);
  void pecnam(gsl::span<char> const& label);
  auto pesmtch(COLORREF const& referenceColor, uint8_t const& colorIndex) -> uint32_t;
  void ritpes(std::vector<uint8_t>& buffer, F_POINT_ATTR const& stitch, F_POINT const& offset);
  void ritpesBlock(std::vector<uint8_t>& buffer, PESSTCHLST newBlock);
  void ritpesCode(std::vector<uint8_t>& buffer);
  void rpcrd(std::vector<uint8_t>& buffer, F_POINT& thisStitch, float srcX, float srcY);
  void writeThumbnail(std::vector<uint8_t>& buffer, imgArray const& image);
} // namespace internal
} // namespace PES
#endif
