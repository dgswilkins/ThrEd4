// Local Headers
#include "stdafx.h"
#include "switches.h"

#if PESACT
// Local Headers
#include "displayText.h"
#include "EnumMap.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "PES.h"
#include "point.h"
#include "Resources/resource.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "utf8conv.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "gsl/narrow"
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <fileapi.h>
#include <handleapi.h>
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <iterator>
// ReSharper disable CppUnusedIncludeDirective
#include <ranges>
// ReSharper restore CppUnusedIncludeDirective
#include <stdexcept>
#include <string>
#include <vector>

#pragma pack(push, 1)
// clang-format off
class PECHDR
{
  public:
// NOLINTBEGIN(readability-magic-numbers, misc-non-private-member-variables-in-classes)
  std::array<char, 19>     label {}; // Label string prefixed with "LA:" and padded with space (0x20)
  int8_t                   labnd {}; // carriage return character
  std::array<uint8_t, 11>  ukn1 {};  // Unknown (' ')
  uint8_t                  ukn2 {};  // Unknown
  uint16_t                 hnd1 {};  // Unknown (0x00ff)
  uint8_t                  thumbWidth {};  // Thumbnail image width in bytes (6) , with 8 bit pixels per byte
                                           // So, 6 would mean 6x8 = 48 pixels per line
  uint8_t                  thumbHeight {}; // Thumbnail image height in pixels (38)
  std::array<uint8_t, 12>  ukn3 {};        // Unknown, usually 20 20 20 20 64 20 00 20 00 20 20 20
  uint8_t                  colorCount {};  // Number of colors minus one, 0xFF means 0 colors
  std::array<uint8_t, 463> pad {};         // Pad bytes up to 512.
  // NOLINTEND(readability-magic-numbers, misc-non-private-member-variables-in-classes)

  explicit constexpr PECHDR() noexcept = default;
};
// clang-format on
#pragma pack(pop)

#pragma pack(push, 1)
class PECHDR2
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
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
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  explicit constexpr PECHDR2() noexcept = default;
};
#pragma pack(pop)

class PES_COLOR_LIST
{
  public:
  explicit constexpr PES_COLOR_LIST(uint16_t const bIndex, uint16_t const cIndex) noexcept :
      m_blockIndex(bIndex), m_colorIndex(cIndex) {
  }

  [[nodiscard]] constexpr auto getBlockIndex() const noexcept -> uint16_t {
	return m_blockIndex;
  }

  [[nodiscard]] constexpr auto getColorIndex() const noexcept -> uint16_t {
	return m_colorIndex;
  }

  private:
  uint16_t m_blockIndex {};
  uint16_t m_colorIndex {};
};

#pragma pack(push, 1)
class PESHED
{
  public:
  // NOLINTBEGIN(readability-magic-numbers, misc-non-private-member-variables-in-classes)
  std::array<char, 4>    ledI {};  //   0-3  Identification (#PES)
  std::array<char, 4>    ledV {};  //   4-7  version (0001)
  uint32_t               off {};   //   8-b  Absolute PEC section byte offset
  uint16_t               hpsz {};  //   c,d  Hoopsize (0), 0 = 100x100mm, 1 = 130x180mm
  uint16_t               usdn {};  //   e,f  Use existing design area (1)
  uint16_t               blct {};  // 10,11  CSewSeg segment block count (1)
  std::array<uint8_t, 4> hnd1 {};  // 12-15  header end (FF FF 00 00)
  uint16_t               celn {};  // 16,17  Length of following string (7)
  std::array<char, 7>    ce {};    // 18-1e  CEmbOne identification (CEmbOne)
  int16_t                xlft {};  // 1f,20  Extent left
  int16_t                xtop {};  // 21,22  Extent top
  int16_t                xrht {};  // 23,24  Extent right
  int16_t                xbot {};  // 25,26  Extent bottom
  int16_t                plft {};  // 27,28  Extent left position
  int16_t                ptop {};  // 29,2a  Extent top position
  int16_t                prht {};  // 2b,2c  Extent right position
  int16_t                pbot {};  // 2d,2e  Extent bottom position
  float                  atfm1 {}; // 2f-32  Affine transform Scale X (1.0F) (00 00 80 3f)
  float                  atfm2 {}; // 33-36  Affine transform Skew X (0.0F) (00 00 00 00)
  float                  atfm3 {}; // 37-3a  Affine transform Skew Y (0.0F) (00 00 00 00)
  float                  atfm4 {}; // 3b-3e  Affine transform Scale Y (1.0F) (00 00 80 3f)
  float                  atfm5 {}; // 3f-42  Affine transform Left_Pos
  float                  atfm6 {}; // 43-46  Affine transform Bottom_Pos
  uint16_t               ukn1 {};  // 47,48  unknown (1)
  int16_t                xtrn {};  // 49,4a  CSewSeg x coordinate translation (0)
  int16_t                ytrn {};  // 4b,4c  CSewSeg y coordinate translation (0)
  int16_t                xsiz {};  // 4d,4e  CSewSeg width
  int16_t                ysiz {};  // 4f,50  CSewSeg height
  std::array<int8_t, 8>  ukn2 {};  // 51,58  unknown (0)
  uint16_t               bcnt {};  // 59,5a  CSewSeg block count (segments + (2*colorChanges))
  std::array<uint8_t, 4> hnd2 {};  // 5b-5e  header end (FF FF 00 00)
  uint16_t               cslen {}; // 5f,60  CSewSeg length (7)
  std::array<char, 7>    cs {};    // 61-67  CSewSeg identification (CSewSeg)

  //  uint16_t               styp1;    // 68,69  Stitch type (0)
  //  uint16_t               scol;     // 6a,6b  Stitch Palette thread index

  // NOLINTEND(readability-magic-numbers, misc-non-private-member-variables-in-classes)

  constexpr PESHED() noexcept = default;
};
#pragma pack(pop)

#ifdef USE_PESLED
#pragma pack(push, 1) // make sure that the PES data structures are aligned on byte boundaries
// NOLINTBEGIN(readability-magic-numbers)
class PESLED
{
  private:
  std::array<int8_t, 8> m_ver {};
  uint32_t              m_pec {};

  public:
  explicit inline constexpr PESLED() noexcept = default;
};
// NOLINTEND(readability-magic-numbers)
#pragma pack(pop)
#endif

#pragma pack(push, 1)
class PESSTCHLST
{
  public:
  explicit constexpr PESSTCHLST(uint16_t const stype, uint16_t const tIndex, uint16_t const count) noexcept
      : m_stitchType(stype), m_threadIndex(tIndex), m_stitchCount(count) {
  }

  void setStitchType(uint32_t const stype) {
	m_stitchType = gsl::narrow<uint16_t>(stype);
  }

  private:
  uint16_t m_stitchType {};
  // NOLINTBEGIN(clang-diagnostic-unused-private-field)
  uint16_t m_threadIndex {};
  uint16_t m_stitchCount {};
  // NOLINTEND(clang-diagnostic-unused-private-field)
};
#pragma pack(pop)

#pragma pack(push, 1)
class PESTCH
{
  public:
  inline auto operator=(F_POINT const& rhs) -> PESTCH&;

  private:
  int16_t m_x {};
  int16_t m_y {};
};
#pragma pack(pop)

inline auto PESTCH::operator=(F_POINT const& rhs) -> PESTCH& {
  m_x = wrap::round<int16_t>(rhs.x);
  m_y = wrap::round<int16_t>(rhs.y);

  return *this;
}

class THREAD
{
  public:
  explicit constexpr THREAD(PEC_COLOR const& color, char const* description, char const* catalogNumber) noexcept
      : m_color(color), m_description(description), m_catalogNumber(catalogNumber) {
  }

  [[nodiscard]] constexpr auto getColor() const noexcept -> PEC_COLOR const& {
	return m_color;
  }

  [[nodiscard]] constexpr auto getRGB() const noexcept -> COLORREF {
	return RGB(m_color.r, m_color.g, m_color.b);
  }

  private:
  PEC_COLOR m_color {};
  // NOLINTBEGIN(clang-diagnostic-unused-private-field)
  char const* m_description {};
  char const* m_catalogNumber {};
  // NOLINTEND(clang-diagnostic-unused-private-field)
};

// PES internal namespace
namespace {
constexpr auto BIT12    = uint32_t {0x800U}; // Set bit 12 if delta is negative
constexpr auto BIT8     = uint32_t {0x80U};  // Set bit 8 on the upper byte
constexpr auto BTSPBYTE = 8U;                // bits per byte
constexpr auto IPECFACT = 3.0F / 5.0F;       // inverse PEC format scale factor
constexpr auto PECFACT  = 5.0F / 3.0F;       // PEC format scale factor
constexpr auto POSOFF   = int32_t {0x1000};  // offset used to shift value positive

// thumbnail image size
constexpr uint8_t THUMBHGT = 38U;
constexpr uint8_t THUMBWID = 48U;
using imgArray             = std::array<std::array<uint8_t, THUMBWID>, THUMBHGT>;

constexpr auto INDEX00 = THREAD {{.r = 0x00, .g = 0x00, .b = 0x00}, "Unknown", ""};
constexpr auto INDEX01 = THREAD {{.r = 0x1a, .g = 0x0a, .b = 0x94}, "Prussian Blue", "ETP007"};
constexpr auto INDEX02 = THREAD {{.r = 0x0f, .g = 0x75, .b = 0xff}, "Blue", "ETP405"};
constexpr auto INDEX03 = THREAD {{.r = 0x00, .g = 0x93, .b = 0x4c}, "Teal Green", "ETP534"};
constexpr auto INDEX04 = THREAD {{.r = 0xba, .g = 0xbd, .b = 0xfe}, "Corn Flower Blue", "ETP070"};
constexpr auto INDEX05 = THREAD {{.r = 0xec, .g = 0x00, .b = 0x00}, "Red", "ETP800"};
constexpr auto INDEX06 = THREAD {{.r = 0xe4, .g = 0x99, .b = 0x5a}, "Reddish Brown", "ETP337"};
constexpr auto INDEX07 = THREAD {{.r = 0xcc, .g = 0x48, .b = 0xab}, "Magenta", "ETP620"};
constexpr auto INDEX08 = THREAD {{.r = 0xfd, .g = 0xc4, .b = 0xfa}, "Light Lilac", "ETP810"};
constexpr auto INDEX09 = THREAD {{.r = 0xdd, .g = 0x84, .b = 0xcd}, "Lilac", "ETP612"};
constexpr auto INDEX10 = THREAD {{.r = 0x6b, .g = 0xd3, .b = 0x8a}, "Mint Green", "ETP502"};
constexpr auto INDEX11 = THREAD {{.r = 0xe4, .g = 0xa9, .b = 0x45}, "Deep Gold", "ETP214"};
constexpr auto INDEX12 = THREAD {{.r = 0xff, .g = 0xbd, .b = 0x42}, "Orange", "ETP208"};
constexpr auto INDEX13 = THREAD {{.r = 0xff, .g = 0xe6, .b = 0x00}, "Yellow", "ETP205"};
constexpr auto INDEX14 = THREAD {{.r = 0x6c, .g = 0xd9, .b = 0x00}, "Lime Green", "ETP513"};
constexpr auto INDEX15 = THREAD {{.r = 0xc1, .g = 0xa9, .b = 0x41}, "Brass", "ETP328"};
constexpr auto INDEX16 = THREAD {{.r = 0xb5, .g = 0xad, .b = 0x97}, "Silver", "ETP005"};
constexpr auto INDEX17 = THREAD {{.r = 0xba, .g = 0x9c, .b = 0x5f}, "Russet Brown", "ETP330"};
constexpr auto INDEX18 = THREAD {{.r = 0xfa, .g = 0xf5, .b = 0x9e}, "Cream Brown", "ETP010"};
constexpr auto INDEX19 = THREAD {{.r = 0x80, .g = 0x80, .b = 0x80}, "Pewter", "ETP704"};
constexpr auto INDEX20 = THREAD {{.r = 0x00, .g = 0x00, .b = 0x00}, "Black", "ETP900"};
constexpr auto INDEX21 = THREAD {{.r = 0x00, .g = 0x1c, .b = 0xdf}, "Ultramarine", "ETP406"};
constexpr auto INDEX22 = THREAD {{.r = 0xdf, .g = 0x00, .b = 0xb8}, "Royal Purple", "ETP869"};
constexpr auto INDEX23 = THREAD {{.r = 0x62, .g = 0x62, .b = 0x62}, "Dark Gray", "ETP707"};
constexpr auto INDEX24 = THREAD {{.r = 0x69, .g = 0x26, .b = 0x0d}, "Dark Brown", "ETP058"};
constexpr auto INDEX25 = THREAD {{.r = 0xff, .g = 0x00, .b = 0x60}, "Deep Rose", "ETP086"};
constexpr auto INDEX26 = THREAD {{.r = 0xbf, .g = 0x82, .b = 0x00}, "Light Brown", "ETP323"};
constexpr auto INDEX27 = THREAD {{.r = 0xf3, .g = 0x91, .b = 0x78}, "Salmon Pink", "ETP079"};
constexpr auto INDEX28 = THREAD {{.r = 0xff, .g = 0x68, .b = 0x05}, "Vermilion", "ETP030"};
constexpr auto INDEX29 = THREAD {{.r = 0xf0, .g = 0xf0, .b = 0xf0}, "White", "ETP001"};
constexpr auto INDEX30 = THREAD {{.r = 0xc8, .g = 0x32, .b = 0xcd}, "Violet", "ETP613"};
constexpr auto INDEX31 = THREAD {{.r = 0xb0, .g = 0xbf, .b = 0x9b}, "Seacrest", "ETP542"};
constexpr auto INDEX32 = THREAD {{.r = 0x65, .g = 0xbf, .b = 0xeb}, "Sky Blue", "ETP019"};
constexpr auto INDEX33 = THREAD {{.r = 0xff, .g = 0xba, .b = 0x04}, "Pumpkin", "ETP126"};
constexpr auto INDEX34 = THREAD {{.r = 0xff, .g = 0xf0, .b = 0x6c}, "Cream Yellow", "ETP010"};
constexpr auto INDEX35 = THREAD {{.r = 0xfe, .g = 0xca, .b = 0x15}, "Khaki", "ETP348"};
constexpr auto INDEX36 = THREAD {{.r = 0xf3, .g = 0x81, .b = 0x01}, "Clay Brown", "ETP339"};
constexpr auto INDEX37 = THREAD {{.r = 0x37, .g = 0xa9, .b = 0x23}, "Leaf Green", "ETP509"};
constexpr auto INDEX38 = THREAD {{.r = 0x23, .g = 0x46, .b = 0x5f}, "Peacock Blue", "ETP405"};
constexpr auto INDEX39 = THREAD {{.r = 0xa6, .g = 0xa6, .b = 0x95}, "Gray", "ETP707"};
constexpr auto INDEX40 = THREAD {{.r = 0xce, .g = 0xbf, .b = 0xa6}, "Warm Gray", "ETP399"};
constexpr auto INDEX41 = THREAD {{.r = 0x96, .g = 0xaa, .b = 0x02}, "Dark Olive", "ETP517"};
constexpr auto INDEX42 = THREAD {{.r = 0xff, .g = 0xe3, .b = 0xc6}, "Linen", "ETP307"};
constexpr auto INDEX43 = THREAD {{.r = 0xff, .g = 0x99, .b = 0xd7}, "Pink", "ETP085"};
constexpr auto INDEX44 = THREAD {{.r = 0x00, .g = 0x70, .b = 0x04}, "Deep Green", "ETP808"};
constexpr auto INDEX45 = THREAD {{.r = 0xed, .g = 0xcc, .b = 0xfb}, "Lavender", "ETP804"};
constexpr auto INDEX46 = THREAD {{.r = 0xc0, .g = 0x89, .b = 0xd8}, "Wisteria Violet", "ETP607"};
constexpr auto INDEX47 = THREAD {{.r = 0xe7, .g = 0xd9, .b = 0xb4}, "Beige", "ETP843"};
constexpr auto INDEX48 = THREAD {{.r = 0xe9, .g = 0x0e, .b = 0x86}, "Carmine", "ETP807"};
constexpr auto INDEX49 = THREAD {{.r = 0xcf, .g = 0x68, .b = 0x29}, "Amber Red", "ETP333"};
constexpr auto INDEX50 = THREAD {{.r = 0x40, .g = 0x86, .b = 0x15}, "Olive Green", "ETP519"};
constexpr auto INDEX51 = THREAD {{.r = 0xdb, .g = 0x17, .b = 0x97}, "Dark Fuchsia", "ETP107"};
constexpr auto INDEX52 = THREAD {{.r = 0xff, .g = 0xa7, .b = 0x04}, "Tangerine", "ETP209"};
constexpr auto INDEX53 = THREAD {{.r = 0xb9, .g = 0xff, .b = 0xff}, "Light Blue", "ETP017"};
constexpr auto INDEX54 = THREAD {{.r = 0x22, .g = 0x89, .b = 0x27}, "Emerald Green", "ETP507"};
constexpr auto INDEX55 = THREAD {{.r = 0xb6, .g = 0x12, .b = 0xcd}, "Purple", "ETP614"};
constexpr auto INDEX56 = THREAD {{.r = 0x00, .g = 0xaa, .b = 0x00}, "Moss Green", "ETP515"};
constexpr auto INDEX57 = THREAD {{.r = 0xfe, .g = 0xa9, .b = 0xdc}, "Flesh Pink", "ETP124"};
constexpr auto INDEX58 = THREAD {{.r = 0xfe, .g = 0xd5, .b = 0x10}, "Harvest Gold", "ETP206"};
constexpr auto INDEX59 = THREAD {{.r = 0x00, .g = 0x97, .b = 0xdf}, "Electric Blue", "ETP420"};
constexpr auto INDEX60 = THREAD {{.r = 0xff, .g = 0xff, .b = 0x84}, "Lemon Yellow", "ETP205"};
constexpr auto INDEX61 = THREAD {{.r = 0xcf, .g = 0xe7, .b = 0x74}, "Fresh Green", "ETP027"};
constexpr auto INDEX62 = THREAD {{.r = 0xff, .g = 0xc8, .b = 0x64}, "Applique Material", ""};
constexpr auto INDEX63 = THREAD {{.r = 0xff, .g = 0xc8, .b = 0xc8}, "Applique Position", ""};
constexpr auto INDEX64 = THREAD {{.r = 0xff, .g = 0xc8, .b = 0xc8}, "Applique", ""};

constexpr auto PES_THREAD = std::array {
    INDEX00, INDEX01, INDEX02, INDEX03, INDEX04, INDEX05, INDEX06, INDEX07, INDEX08, INDEX09,
    INDEX10, INDEX11, INDEX12, INDEX13, INDEX14, INDEX15, INDEX16, INDEX17, INDEX18, INDEX19,
    INDEX20, INDEX21, INDEX22, INDEX23, INDEX24, INDEX25, INDEX26, INDEX27, INDEX28, INDEX29,
    INDEX30, INDEX31, INDEX32, INDEX33, INDEX34, INDEX35, INDEX36, INDEX37, INDEX38, INDEX39,
    INDEX40, INDEX41, INDEX42, INDEX43, INDEX44, INDEX45, INDEX46, INDEX47, INDEX48, INDEX49,
    INDEX50, INDEX51, INDEX52, INDEX53, INDEX54, INDEX55, INDEX56, INDEX57, INDEX58, INDEX59,
    INDEX60, INDEX61, INDEX62, INDEX63, INDEX64};

constexpr auto THTYPCNT = PES_THREAD.size(); // Count of thread colors available in the PES format

// clang-format off
constexpr auto IMAGE_WITH_FRAME = imgArray{{ 
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } 
}};
// clang-format on

auto PESequivColors = std::array<uint8_t, COLORCNT> {}; // pes equivalent colors

// Definitions
auto dupcol(gsl::span<uint8_t> const& pesColors, uint32_t activeColor, uint32_t& index) -> uint32_t;
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

// Functions
auto dupcol(gsl::span<uint8_t> const& pesColors, uint32_t const activeColor, uint32_t& index) -> uint32_t {
  auto const& threadColor = PES_THREAD.at(pesColors[index++] % THTYPCNT);
  auto const  color       = threadColor.getRGB();
  auto        iUserColor  = UserColor.cbegin();
  for (auto iColor = 0U; iColor < activeColor; ++iColor) {
	if (*iUserColor == color) {
	  return iColor;
	}
	++iUserColor;
  }
  auto minimumDistance = BIGUINT;
  auto matchIndex      = 0U;
  for (auto iColor = 1U; iColor < activeColor; ++iColor) {
	if (auto const matchDistance = pesmtch(color, pesColors[iColor]); matchDistance < minimumDistance) {
	  minimumDistance = matchDistance;
	  matchIndex      = iColor;
	}
  }
  return matchIndex;
}

void pecEncodeInt(std::vector<uint8_t>& buffer, int32_t const delta) {
  constexpr auto MSK11BIT  = uint32_t {0x7FFU}; // used to mask the value to 11 bits
  auto           outputVal = gsl::narrow_cast<uint32_t>(std::abs(delta)) & MSK11BIT;
  if (delta < 0) {
	outputVal = gsl::narrow_cast<uint32_t>(delta + POSOFF) & MSK11BIT;
	outputVal |= BIT12;
  }
  // upper byte has upper 4 bits of the encode value
  auto const upperByte = gsl::narrow_cast<uint8_t>(((outputVal >> BYTSHFT) & NIBMASK) | BIT8);
  buffer.push_back(upperByte);
  // lower byte has lower 8 bits of the encoded value
  auto const lowerByte = gsl::narrow_cast<uint8_t>(outputVal & BYTMASK);
  buffer.push_back(lowerByte);
}

void pecEncodeStop(std::vector<uint8_t>& buffer, uint8_t const val) {
  // NOLINTBEGIN(readability-magic-numbers)
  buffer.push_back(0xfe);
  buffer.push_back(0xb0);
  buffer.push_back(val);
  // NOLINTEND(readability-magic-numbers)
}

void pecImage(std::vector<uint8_t>& pecBuffer) {
  auto       thumbnail = IMAGE_WITH_FRAME;
  auto const yFactor   = 31.0F / IniFile.hoopSizeY;
  auto const xFactor   = 40.0F / IniFile.hoopSizeX;
  // write the overall thumbnail
  constexpr auto XOFFSET = uint8_t {4U}; // thumbnail x offset to place it in the frame correctly
  constexpr auto YOFFSET = uint8_t {5U}; // thumbnail y offset to place it in the frame correctly
  for (auto const& stitch : Instance->stitchBuffer) {
	auto const xCoord = wrap::toPtrdiff(wrap::floor<uint16_t>(stitch.x * xFactor) + XOFFSET);
	auto const yCoord = wrap::toPtrdiff(THUMBHGT - (wrap::floor<uint16_t>(stitch.y * yFactor) + YOFFSET));
	auto const iThumbnail = std::next(thumbnail.begin(), yCoord);
	auto const iRow       = std::next(iThumbnail->begin(), xCoord);
	*iRow                 = 1U;
  }
  writeThumbnail(pecBuffer, thumbnail);
  // now write out the individual thread thumbnails
  thumbnail        = IMAGE_WITH_FRAME;
  auto stitchColor = Instance->stitchBuffer.front().attribute & COLMSK;
  for (auto const& stitch : Instance->stitchBuffer) {
	auto const xCoord = wrap::toPtrdiff(wrap::floor<uint16_t>(stitch.x * xFactor) + XOFFSET);
	auto const yCoord = wrap::toPtrdiff(THUMBHGT - (wrap::floor<uint16_t>(stitch.y * yFactor) + YOFFSET));
	auto const iThumbnail = std::next(thumbnail.begin(), yCoord);
	auto const iRow       = std::next(iThumbnail->begin(), xCoord);
	if (stitchColor == (stitch.attribute & COLMSK)) {
	  *iRow = 1U;
	}
	else {
	  writeThumbnail(pecBuffer, thumbnail);
	  thumbnail   = IMAGE_WITH_FRAME;
	  stitchColor = stitch.attribute & COLMSK;
	  *iRow       = 1U;
	}
  }
  writeThumbnail(pecBuffer, thumbnail);
}

void pecdat(std::vector<uint8_t>& buffer) {
  auto* pecHeader  = convertFromPtr<PECHDR*>(buffer.data());
  auto  thisStitch = F_POINT {};
  rpcrd(buffer, thisStitch, Instance->stitchBuffer.front().x, Instance->stitchBuffer.front().y);
  auto iColor     = 1U;
  auto color      = Instance->stitchBuffer.front().attribute & COLMSK;
  auto iPEC       = wrap::next(PESequivColors.begin(), color);
  auto iPesColors = pecHeader->pad.begin();
  *iPesColors++   = *iPEC;
  for (auto const  stitchRange = std::ranges::subrange(std::next(Instance->stitchBuffer.begin()),
                                                      Instance->stitchBuffer.end());
       auto const& stitch : stitchRange) {
	if ((stitch.attribute & COLMSK) != color) {
	  color = stitch.attribute & COLMSK;
	  pecEncodeStop(buffer, (iColor % 2U) + 1U);
	  iPEC          = wrap::next(PESequivColors.begin(), color);
	  *iPesColors++ = *iPEC;
	  ++iColor;
	}
	auto const xDelta = stitch.x - thisStitch.x;
	auto const yDelta = stitch.y - thisStitch.y;
	rpcrd(buffer, thisStitch, xDelta, yDelta);
  }
  // add the end marker
  // NOLINTBEGIN(readability-magic-numbers)
  buffer.push_back(0xffU);
  buffer.push_back(0x00U);
  // NOLINTEND(readability-magic-numbers)
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
// ReSharper disable CppDeprecatedEntity
void pecnam(gsl::span<char> const& label) {
  strncpy(label.data(), "LA:", 3);
  auto const lblSize  = wrap::toUnsigned(label.size() - 3U);
  auto       fileStem = utf::utf16ToUtf8(Instance->auxName.stem());
  if (fileStem.size() < lblSize) {
	fileStem += std::string(lblSize - fileStem.size(), ' ');
  }
  auto* ptr = std::next(label.data(), 3U);
  strncpy(ptr, fileStem.c_str(), lblSize);
}
// ReSharper restore CppDeprecatedEntity
#pragma warning(pop)

auto pesmtch(COLORREF const& referenceColor, uint8_t const& colorIndex) -> uint32_t {
  auto color = PEC_COLOR {
      .r = GetRValue(referenceColor), .g = GetGValue(referenceColor), .b = GetBValue(referenceColor)};
  auto const& translatedColor = PES_THREAD.at(colorIndex).getColor();
  auto const meanR = (gsl::narrow_cast<int32_t>(color.r) + gsl::narrow_cast<int32_t>(translatedColor.r)) / 2;
  auto const deltaR = gsl::narrow_cast<int32_t>(color.r) - gsl::narrow_cast<int32_t>(translatedColor.r);
  auto const deltaG = gsl::narrow_cast<int32_t>(color.g) - gsl::narrow_cast<int32_t>(translatedColor.g);
  auto const deltaB = gsl::narrow_cast<int32_t>(color.b) - gsl::narrow_cast<int32_t>(translatedColor.b);
  // NOLINTBEGIN(readability-magic-numbers)
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula NO LINTNEXTLINE(readability-magic-numbers)
  return wrap::round<uint32_t>(std::sqrt((((512 + meanR) * deltaR * deltaR) / 256) + (4 * deltaG * deltaG) +
                                         (((767 - meanR) * deltaB * deltaB) / 256)));
  // NOLINTEND(readability-magic-numbers)
}

void ritpes(std::vector<uint8_t>& buffer, F_POINT_ATTR const& stitch, F_POINT const& offset) {
  auto const oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESTCH));
  auto* pesStitch = convertFromPtr<PESTCH*>(&buffer[oldSize]);
  if (nullptr == pesStitch) {
	throw std::runtime_error("Failed to convert PESTCH pointer");
  }
  auto const scaledStitch =
      F_POINT {(-stitch.x * IPECFACT) + offset.x, (stitch.y * IPECFACT) - offset.y};
  *pesStitch = scaledStitch;
}

void ritpesBlock(std::vector<uint8_t>& buffer, PESSTCHLST const newBlock) {
  auto const oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESSTCHLST));
  auto* blockHeader = convertFromPtr<PESSTCHLST*>(&buffer[oldSize]);
  *blockHeader      = newBlock;
}

void ritpesCode(std::vector<uint8_t>& buffer) {
  constexpr auto ENDCODE = uint16_t {0x8003U}; // Block end code
  auto const     oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(uint16_t));
  if (auto* contCode = convertFromPtr<uint16_t*>(&buffer[oldSize]); contCode != nullptr) {
	*contCode = ENDCODE;
  }
}

void rpcrd(std::vector<uint8_t>& buffer, F_POINT& thisStitch, float const srcX, float const srcY) {
  constexpr auto DELTAMAX = int32_t {63};  // maximum value of the delta
  constexpr auto DELTAMIN = int32_t {-64}; // minimum value of the delta

  auto const deltaX = std::lround(srcX * PECFACT);
  auto const deltaY = -std::lround(srcY * PECFACT);
  if (deltaX < DELTAMAX && deltaX > DELTAMIN && deltaY < DELTAMAX && deltaY > DELTAMIN) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	auto const xVal = gsl::narrow<uint8_t>(deltaX & MSK7BITS);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	auto const yVal = gsl::narrow<uint8_t>(deltaY & MSK7BITS);
	buffer.push_back(xVal);
	buffer.push_back(yVal);
  }
  else {
	pecEncodeInt(buffer, deltaX);
	pecEncodeInt(buffer, deltaY);
  }
  thisStitch += F_POINT {deltaX, -deltaY} * IPECFACT;
}

void writeThumbnail(std::vector<uint8_t>& buffer, imgArray const& image) {
  for (auto const& imageRow : image) {
	auto const& iRow = imageRow;
	for (auto iPixel = iRow.begin(); iPixel != iRow.end();) {
	  auto output = uint8_t {};
	  for (auto bitPosition = 0U; bitPosition < BTSPBYTE; ++bitPosition) {
		output |= gsl::narrow_cast<uint8_t>(*iPixel << bitPosition);
		++iPixel;
	  }
	  buffer.push_back(output);
	}
  }
}

} // namespace

auto PES::readPESFile(fs::path const& newFileName) -> bool {
  auto fileSize = uintmax_t {};
  if (!thred::getFileSize(newFileName, fileSize)) {
	return false;
  }
  if (fileSize == 0U) {
	return false;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	return false;
  }
  auto fileBuf = std::vector<uint8_t> {};
  fileBuf.resize(wrap::toSize(fileSize));
  auto* fileBuffer = fileBuf.data();
  if (fileBuffer == nullptr) {
	CloseHandle(fileHandle);
	return false;
  }
  auto bytesRead = DWORD {};
  if (!wrap::readFile(fileHandle, fileBuffer, fileSize, &bytesRead, L"ReadFile for fileBuffer in readPESFile")) {
	return false;
  }
  auto const* pesHeader = convertFromPtr<PESHED*>(fileBuf.data());

  // Check for the PES lead in value
  if (constexpr auto PESSTR = "#PES"; strncmp(pesHeader->ledI.data(), PESSTR, strlen(PESSTR)) != 0) {
	displayText::showMessage(IDS_NOTPES, newFileName.wstring());
	CloseHandle(fileHandle);
	return false;
  }

  constexpr std::array VER_STRINGS = {
      "0001", "0020", "0022", "0030", "0040", "0050", "0055", "0056", "0060", "0070", "0080", "0090", "0100"};

  auto contFlag = false;
  for (auto const& version : VER_STRINGS) {
	if (strncmp(pesHeader->ledV.data(), version, pesHeader->ledV.size()) == 0) {
	  contFlag = true;
	  break;
	}
  }
  if (!contFlag) {
	displayText::showMessage(IDS_NOTPES, newFileName.wstring());
	CloseHandle(fileHandle);
	return false;
  }
  auto*       pecHeader = convertFromPtr<PECHDR*>(&fileBuf[pesHeader->off]);
  auto const  pecOffset = pesHeader->off + sizeof(PECHDR) + sizeof(PECHDR2);
  auto const* pesStitch = &fileBuf[pecOffset];
  if (pesStitch == nullptr) {
	return false;
  }
  auto const pesColorCount = pecHeader->colorCount + 1U;
  auto const pesColors     = gsl::span {pecHeader->pad.data(), pesColorCount};
  auto colorMap = boost::dynamic_bitset(THTYPCNT); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  auto iUserColor = UserColor.begin();
  for (auto iColor = 0U; iColor < pesColorCount; ++iColor) {
	if (pesColors[iColor] < THTYPCNT) {
	  if (colorMap.test_set(pesColors[iColor])) {
		continue;
	  }
	  auto const& threadColor = PES_THREAD.at(pesColors[iColor]);
	  auto const  color       = threadColor.getRGB();
	  *iUserColor             = color;
	  ++iUserColor;
	  if (iUserColor == UserColor.end()) {
		break;
	  }
	  continue;
	}
	auto constexpr COLOR = PES_THREAD[0].getRGB(); // color unknown

	*iUserColor = COLOR;
	++iUserColor;
  }
  auto loc = F_POINT {};
  Instance->stateMap.reset(StateFlag::FILDIR);
  if (bytesRead <= pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2)) + 3U) {
	displayText::showMessage(IDS_NOTPES, newFileName.wstring());
	CloseHandle(fileHandle);
	return false;
  }
  auto       color      = 0U;
  auto       iPESstitch = size_t {0U};
  auto const pecCount   = bytesRead - (pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U;
  Instance->stitchBuffer.clear();
  Instance->stitchBuffer.reserve(pecCount / 2); // we are still reserving a bit more than necessary
  Instance->stitchBuffer.emplace_back();
  constexpr auto MSK12BIT = uint32_t {0xFFFU}; // used to mask the value to 12 bits

  auto       pesColorIndex = uint32_t {1U};
  auto const pesStitches   = gsl::span {pesStitch, pecCount};
  while (iPESstitch < pecCount) {
	// check for end marker NOLINTNEXTLINE(readability-magic-numbers)
	if (pesStitches[iPESstitch] == 0xff && pesStitches[iPESstitch + 1U] == 0) {
	  break;
	}
	// check for color change NOLINTNEXTLINE(readability-magic-numbers)
	if (pesStitches[iPESstitch] == 0xfe && pesStitches[iPESstitch + 1U] == 0xb0) {
	  color = dupcol(pesColors, wrap::toUnsigned(std::distance(UserColor.begin(), iUserColor)), pesColorIndex);
	  iPESstitch += 2;
	}
	else {
	  auto locof = 0.0F;
	  if ((pesStitches[iPESstitch] & BIT8) != 0U) {
		// combine the 4 bits from the first byte with the 8 bits from the next byte
		auto pesVal =
		    (gsl::narrow_cast<uint32_t>(pesStitches[iPESstitch] << BYTSHFT) | pesStitches[iPESstitch + 1U]) & MSK12BIT;
		if ((pesVal & BIT12) != 0U) {
		  pesVal -= POSOFF;
		}
		auto const sPesVal = gsl::narrow_cast<int32_t>(pesVal);
		wrap::narrow_cast(locof, sPesVal);
		++iPESstitch;
	  }
	  else {
		// NOLINTBEGIN(readability-magic-numbers)
		if (pesStitches[iPESstitch] > 0x3f) { // if the value is more than 63 flip sign
		  locof = wrap::toFloat(pesStitches[iPESstitch]) - 128.0F;
		}
		else {
		  locof = pesStitches[iPESstitch];
		}
		// NOLINTEND(readability-magic-numbers)
	  }
	  locof *= IPECFACT;
	  // ToDo - (PES) Use a new flag bit for this since FILDIR is not correct
	  if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
		loc.y -= locof;
		Instance->stitchBuffer.emplace_back(loc.x, loc.y, color);
	  }
	  else {
		loc.x += locof;
	  }
	}
	++iPESstitch;
  }
  thred::hupfn();
  CloseHandle(fileHandle);
  return true;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
auto PES::savePES(fs::path const& auxName, std::vector<F_POINT_ATTR> const& saveStitches) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = CreateFile(
      auxName.wstring().c_str(), GENERIC_WRITE | GENERIC_READ, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::crmsg(auxName);
	return false;
  }
  auto pesHeader = PESHED {};
  if (IniFile.hoopSizeX <= HUP100XY && IniFile.hoopSizeY <= HUP100XY) {
	pesHeader.hpsz = 0;
  }
  else {
	if (IniFile.hoopSizeX <= LHUPX && IniFile.hoopSizeY <= LHUPY) {
	  pesHeader.hpsz = 1;
	}
	else {
	  // ToDo - find better error message
	  displayText::tabmsg(IDS_STCHOUT, false);
	  CloseHandle(fileHandle);
	  return false;
	}
  }
  constexpr auto PESISTR = "#PES";    // PES lead in
  constexpr auto PESVSTR = "0001";    // PES version
  constexpr auto EMBSTR  = "CEmbOne"; // emb section lead in
  constexpr auto SEWSTR  = "CSewSeg"; // sewing segment lead in
  constexpr auto AT5OFF  = 350.0F;    // Affine transform offset 5
  constexpr auto AT6OFF  = 100.0F;    // Affine transform offset 6

  // ReSharper disable CppDeprecatedEntity
  strncpy(pesHeader.ledI.data(), PESISTR, strlen(PESISTR)); // NO LINT(clang-diagnostic-deprecated-declarations)
  strncpy(pesHeader.ledV.data(), PESVSTR, strlen(PESVSTR)); // NO LINT(clang-diagnostic-deprecated-declarations)
  wrap::narrow(pesHeader.celn, strlen(EMBSTR));
  strncpy(pesHeader.ce.data(), EMBSTR, pesHeader.celn); // NO LINT(clang-diagnostic-deprecated-declarations)
  wrap::narrow(pesHeader.cslen, strlen(SEWSTR));
  strncpy(pesHeader.cs.data(), SEWSTR, pesHeader.cslen); // NO LINT(clang-diagnostic-deprecated-declarations)
  // ReSharper restore CppDeprecatedEntity
  {
	auto iPEC = PESequivColors.begin();
	for (auto const& color : UserColor) {
	  auto matchIndex = 0U;
	  auto matchMin   = BIGUINT;
	  for (auto iColorMatch = 1U; iColorMatch < THTYPCNT; ++iColorMatch) {
		if (auto const match = pesmtch(color, gsl::narrow_cast<uint8_t>(iColorMatch)); match < matchMin) {
		  matchIndex = iColorMatch;
		  matchMin   = match;
		}
	  }
	  wrap::narrow(*iPEC, matchIndex);
	  ++iPEC;
	}
  }
  auto stitchColor  = Instance->stitchBuffer.front().attribute & COLMSK;
  auto boundingRect = F_RECTANGLE {};
  thred::stchrct(boundingRect);
  auto const offset      = F_POINT {wrap::midl(boundingRect.right, boundingRect.left),
                               wrap::midl(boundingRect.top, boundingRect.bottom)};
  pesHeader.atfm1        = 1.0F;
  pesHeader.atfm4        = 1.0F;
  auto const designSizeX = (boundingRect.right - boundingRect.left) * PECFACT;
  auto const designSizeY = (boundingRect.top - boundingRect.bottom) * PECFACT;
  pesHeader.xsiz         = wrap::round<int16_t>(designSizeX);
  pesHeader.ysiz         = wrap::round<int16_t>(designSizeY);
  auto const hoopSizeX   = IniFile.hoopSizeX * PECFACT;
  auto const hoopSizeY   = IniFile.hoopSizeY * PECFACT;
  pesHeader.atfm5        = AT5OFF + hoopSizeX * HALF - designSizeX * HALF;
  pesHeader.atfm6        = AT6OFF + designSizeY + hoopSizeY * HALF - designSizeY * HALF;
  auto pesBuffer         = std::vector<uint8_t> {};
  // ToDo - make a reasonable guess for the size of data in the PES buffer. err on the side of caution
  auto const pesSize = sizeof(PESSTCHLST) + (Instance->stitchBuffer.size() * sizeof(PESTCH)) + 1000U;
  pesBuffer.reserve(pesSize);
  auto threadList      = std::vector<PES_COLOR_LIST> {};
  auto blockIndex      = uint16_t {}; // Index into the stitch blocks
  auto iPESequivColors = wrap::next(PESequivColors.begin(), stitchColor);
  auto currentColor    = *iPESequivColors;
  threadList.emplace_back(blockIndex, currentColor);
  pesBuffer.resize(sizeof(PESSTCHLST));
  // first block is a jump in place
  ritpesBlock(pesBuffer, PESSTCHLST {1, currentColor, 2});
  ++blockIndex;
  ritpes(pesBuffer, saveStitches[0], offset);
  ritpes(pesBuffer, saveStitches[0], offset);
  ritpesCode(pesBuffer);
  // then a normal stitch in place
  ritpesBlock(pesBuffer, PESSTCHLST {0, currentColor, 2});
  ++blockIndex;
  ritpes(pesBuffer, saveStitches[0], offset);
  ritpes(pesBuffer, saveStitches[0], offset);
  ritpesCode(pesBuffer);
  // then a jump to the first location
  ritpesBlock(pesBuffer, PESSTCHLST {1, currentColor, 2});
  ++blockIndex;
  ritpes(pesBuffer, saveStitches[0], offset);
  ritpes(pesBuffer, saveStitches[1], offset);
  ritpesCode(pesBuffer);
  // now stitch out.
  auto pesThreadCount = 0U;
  auto lastIndex      = pesBuffer.size();
  ritpesBlock(pesBuffer, PESSTCHLST {0, currentColor, 0});
  ++blockIndex;
  auto stitchCount = 0U;
  for (auto iStitch = 1U; iStitch < wrap::toUnsigned(Instance->stitchBuffer.size()); ++iStitch) {
	if (stitchColor == (Instance->stitchBuffer.operator[](iStitch).attribute & COLMSK)) {
	  // we are in the same color block, so write the stitch
	  ritpes(pesBuffer, saveStitches[iStitch], offset);
	  ++stitchCount;
	}
	else {
	  // write stitch
	  ritpesCode(pesBuffer);
	  // close out the previous block
	  auto* blockHeader = convertFromPtr<PESSTCHLST*>(&pesBuffer[lastIndex]);
	  if (nullptr == blockHeader) {
		throw std::runtime_error("blockHeader is null");
	  }
	  blockHeader->setStitchType(stitchCount);
	  // save the thread/color information
	  ++pesThreadCount;
	  stitchColor     = Instance->stitchBuffer.operator[](iStitch).attribute & COLMSK;
	  iPESequivColors = wrap::next(PESequivColors.begin(), stitchColor);
	  currentColor    = *iPESequivColors;
	  threadList.emplace_back(blockIndex, currentColor);
	  // then create the jump block
	  ritpesBlock(pesBuffer, PESSTCHLST {1, currentColor, 2});
	  ++blockIndex;
	  auto const prevStitch = iStitch - 1U;
	  ritpes(pesBuffer, saveStitches[prevStitch], offset);
	  ritpes(pesBuffer, saveStitches[iStitch], offset);
	  ritpesCode(pesBuffer);
	  // and finally start the next block
	  stitchCount = 0U;
	  lastIndex   = pesBuffer.size();
	  ritpesBlock(pesBuffer, PESSTCHLST {0, currentColor, 0});
	  ++blockIndex;
	  ritpes(pesBuffer, saveStitches[iStitch], offset);
	  ++stitchCount;
	}
  }
  // finalize the last stitch block
  auto* blockHeader = convertFromPtr<PESSTCHLST*>(&pesBuffer[lastIndex]);
  if (nullptr == blockHeader) {
	throw std::runtime_error("blockHeader is null");
  }
  blockHeader->setStitchType(stitchCount);
  // write the color/thread table
  lastIndex = pesBuffer.size();
  pesBuffer.resize(lastIndex + sizeof(uint16_t));
  auto* colorIndex = convertFromPtr<uint16_t*>(&pesBuffer[lastIndex]);
  if (colorIndex == nullptr) {
	CloseHandle(fileHandle);
	return false;
  }
  *colorIndex = gsl::narrow_cast<uint16_t>(pesThreadCount);
  for (auto paletteIndex = 0U; paletteIndex < pesThreadCount; ++paletteIndex) {
	lastIndex = pesBuffer.size();
	pesBuffer.resize(lastIndex + (2 * sizeof(uint16_t)));
	auto* colorEntry = convertFromPtr<uint16_t*>(&pesBuffer[lastIndex]);
	if (colorEntry == nullptr) {
	  CloseHandle(fileHandle);
	  return false;
	}
	auto const colorEntries = gsl::span {colorEntry, 2};
	colorEntries[0]         = threadList[paletteIndex].getBlockIndex();
	colorEntries[1]         = threadList[paletteIndex].getColorIndex();
  }
  pesHeader.off  = wrap::toUnsigned(pesBuffer.size() + sizeof(pesHeader));
  pesHeader.blct = 1;
  // NOLINTBEGIN(readability-magic-numbers)
  // write the header end markers
  pesHeader.hnd1[0] = 0xff;
  pesHeader.hnd1[1] = 0xff;
  pesHeader.hnd1[2] = 0x00;
  pesHeader.hnd1[3] = 0x00;
  pesHeader.bcnt    = gsl::narrow_cast<uint16_t>(pesThreadCount);
  pesHeader.hnd2[0] = 0xff;
  pesHeader.hnd2[1] = 0xff;
  pesHeader.hnd2[2] = 0x00;
  pesHeader.hnd2[3] = 0x00;
  // NOLINTEND(readability-magic-numbers)
  GroupStartStitch  = 0;
  GroupEndStitch    = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
  auto bytesWritten = DWORD {};
  if (FALSE == WriteFile(fileHandle, convertFromPtr<PESHED*>(&pesHeader), sizeof(pesHeader), &bytesWritten, nullptr)) {
	displayText::riter();
	CloseHandle(fileHandle);
	return false;
  }
  if (FALSE == WriteFile(fileHandle, pesBuffer.data(), wrap::toUnsigned(pesBuffer.size()), &bytesWritten, nullptr)) {
	displayText::riter();
	CloseHandle(fileHandle);
	return false;
  }
  pesBuffer.clear();
  pesBuffer.shrink_to_fit();
  auto pecBuffer = std::vector<uint8_t> {};
  // make a reasonable guess for the size of data in the PEC buffer. Assume all stitch coordinates
  // are 2 bytes and pad by 1000 to account for jumps. Also reserve memory for thumbnails
  auto const pecSize = sizeof(PECHDR) + sizeof(PECHDR2) + (Instance->stitchBuffer.size() * 2) +
                       1000 + ((wrap::toSize(pesThreadCount) + 1U) * THUMBHGT * (THUMBWID / 8));
  pecBuffer.reserve(pecSize);
  pecBuffer.resize(sizeof(PECHDR) + sizeof(PECHDR2));
  auto*      pecHeader = convertFromPtr<PECHDR*>(pecBuffer.data());
  auto const spPHL     = gsl::span {pecHeader->label};
  pecnam(spPHL);
  auto fstart = std::next(pecBuffer.begin(), sizeof(pecHeader->label));
  auto fend   = std::next(pecBuffer.begin(), sizeof(*pecHeader));
  std::fill(fstart, fend, ' ');
  pecHeader->labnd = '\r'; // 13 = carriage return
  wrap::narrow(pecHeader->colorCount, pesThreadCount);
  // write the remainder of the header with magic numbers
  // NOLINTBEGIN(readability-magic-numbers)
  pecHeader->hnd1        = 0x00ff;
  pecHeader->thumbHeight = THUMBHGT;
  pecHeader->thumbWidth  = THUMBWID / BTSPBYTE;
  pecdat(pecBuffer);
  auto* pecHeader2            = convertFromPtr<PECHDR2*>(&pecBuffer[sizeof(PECHDR)]);
  pecHeader2->unknown1        = 0;
  pecHeader2->thumbnailOffset = gsl::narrow<uint16_t>(pecBuffer.size() - sizeof(PECHDR));
  pecHeader2->unknown2        = 0x3100;
  pecHeader2->unknown3        = 0xf0ff;
  pecHeader2->width           = pesHeader.xsiz;
  pecHeader2->height          = pesHeader.ysiz;
  pecHeader2->unknown4        = 0x01e0;
  pecHeader2->unknown5        = 0x01b0;
  // NOLINTEND(readability-magic-numbers)

  constexpr auto PECMASK1 = uint16_t {0x9000U}; //
  constexpr auto LOWBMASK = uint16_t {0x00ffU}; // low byte mask
  constexpr auto HIGHBMSK = uint16_t {0xff00U}; // high byte mask
  constexpr auto PECSHIFT = uint16_t {8U};      // byte shift

  auto xInt = wrap::round<uint16_t>(boundingRect.left * PECFACT);
  auto yInt = wrap::round<uint16_t>(boundingRect.bottom * PECFACT);
  xInt |= PECMASK1;
  yInt |= PECMASK1;
  pecHeader2->xMin = gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(xInt & HIGHBMSK) >> PECSHIFT) |
                     gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(xInt & LOWBMASK) << PECSHIFT);
  pecHeader2->yMin = gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(yInt & HIGHBMSK) >> PECSHIFT) |
                     gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(yInt & LOWBMASK) << PECSHIFT);
  pecImage(pecBuffer);
  if (FALSE == WriteFile(fileHandle, pecBuffer.data(), wrap::toUnsigned(pecBuffer.size()), &bytesWritten, nullptr)) {
	displayText::riter();
	CloseHandle(fileHandle);
	return false;
  }
  CloseHandle(fileHandle);
  return true;
}
#pragma warning(pop)

#endif
