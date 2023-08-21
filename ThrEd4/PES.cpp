// Local Headers
#include "stdafx.h"
#include "switches.h"

#if PESACT
// Local Headers
#include "displayText.h"
#include "globals.h"
#include "PES.h"
#include "thred.h"
#include "utf8conv.h"

static constexpr uint8_t THUMBHGT = 38U;
static constexpr uint8_t THUMBWID = 48U;
using imgArray                    = std::array<std::array<uint8_t, THUMBWID>, THUMBHGT>;

#pragma pack(push, 1)
class PECHDR
{
  public:
  char     label[19] {};  // Label string prefixed with "LA:" and padded with space (0x20)
  int8_t   labnd {};      // carriage return character
  uint8_t  ukn1[11] {};   // Unknown (' ')
  uint8_t  ukn2 {};       // Unknown
  uint16_t hnd1 {};       // Unknown (0x00ff)
  uint8_t  thumbWidth {}; // Thumbnail image width in bytes (6) , with 8 bit pixels per byte
                          // Thus, 6 would mean 6x8 = 48 pixels per line
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
#pragma pack(pop)

#pragma pack(push, 1)
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

class PES_COLOR_LIST
{
  public:
  uint16_t blockIndex {};
  uint16_t colorIndex {};

  // constexpr PES_COLOR_LIST() noexcept = default;
  // PES_COLOR_LIST(PES_COLOR_LIST const&) = default;
  // PES_COLOR_LIST(PES_COLOR_LIST&&) = default;
  // PES_COLOR_LIST& operator=(PECCOLORLIST const& rhs) = default;
  // PES_COLOR_LIST& operator=(PES_COLOR_LIST&&) = default;
  //~PES_COLOR_LIST() = default;
};

#pragma pack(push, 1)
// clang-format off
class PESHED
{
  public:
  char     ledI[4] {}; //   0-3  Identification (#PES)         
  char     ledV[4] {}; //   4-7  version (0001)         
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
#pragma pack(pop)

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
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

#pragma pack(push, 1)
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
#pragma pack(pop)

class THREAD
{
  public:
  PEC_COLOR   color {};
  char const* description {};
  char const* catalogNumber {};

  // constexpr THREAD() noexcept = default;
  // THREAD(THREAD const&) = default;
  // THREAD(THREAD&&) = default;
  // THREAD& operator=(THREAD const& rhs) = default;
  // THREAD& operator=(THREAD&&) = default;
  //~THREAD() = default;
};

constexpr auto BIT12    = uint32_t {0x800U}; // Set bit 12 if delta is negative
constexpr auto BIT8     = uint32_t {0x80U};  // Set bit 8 on the upper byte
constexpr auto BTSPBYTE = 8U;                // bits per byte
constexpr auto IPECFACT = 3.0F / 5.0F;       // inverse PEC format scale factor
constexpr auto PECFACT  = 5.0F / 3.0F;       // PEC format scale factor
constexpr auto POSOFF   = int32_t {0x1000};  // offset used to shift value positive

namespace pi {
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
} // namespace pi

namespace {
auto PESequivColors = std::array<uint8_t, COLORCNT> {}; // pes equivalent colors
} // namespace

static constexpr auto INDEX00 = THREAD {{0x00, 0x00, 0x00}, "Unknown", ""};
static constexpr auto INDEX01 = THREAD {{0x1a, 0x0a, 0x94}, "Prussian Blue", "ETP007"};
static constexpr auto INDEX02 = THREAD {{0x0f, 0x75, 0xff}, "Blue", "ETP405"};
static constexpr auto INDEX03 = THREAD {{0x00, 0x93, 0x4c}, "Teal Green", "ETP534"};
static constexpr auto INDEX04 = THREAD {{0xba, 0xbd, 0xfe}, "Corn Flower Blue", "ETP070"};
static constexpr auto INDEX05 = THREAD {{0xec, 0x00, 0x00}, "Red", "ETP800"};
static constexpr auto INDEX06 = THREAD {{0xe4, 0x99, 0x5a}, "Reddish Brown", "ETP337"};
static constexpr auto INDEX07 = THREAD {{0xcc, 0x48, 0xab}, "Magenta", "ETP620"};
static constexpr auto INDEX08 = THREAD {{0xfd, 0xc4, 0xfa}, "Light Lilac", "ETP810"};
static constexpr auto INDEX09 = THREAD {{0xdd, 0x84, 0xcd}, "Lilac", "ETP612"};
static constexpr auto INDEX10 = THREAD {{0x6b, 0xd3, 0x8a}, "Mint Green", "ETP502"};
static constexpr auto INDEX11 = THREAD {{0xe4, 0xa9, 0x45}, "Deep Gold", "ETP214"};
static constexpr auto INDEX12 = THREAD {{0xff, 0xbd, 0x42}, "Orange", "ETP208"};
static constexpr auto INDEX13 = THREAD {{0xff, 0xe6, 0x00}, "Yellow", "ETP205"};
static constexpr auto INDEX14 = THREAD {{0x6c, 0xd9, 0x00}, "Lime Green", "ETP513"};
static constexpr auto INDEX15 = THREAD {{0xc1, 0xa9, 0x41}, "Brass", "ETP328"};
static constexpr auto INDEX16 = THREAD {{0xb5, 0xad, 0x97}, "Silver", "ETP005"};
static constexpr auto INDEX17 = THREAD {{0xba, 0x9c, 0x5f}, "Russet Brown", "ETP330"};
static constexpr auto INDEX18 = THREAD {{0xfa, 0xf5, 0x9e}, "Cream Brown", "ETP010"};
static constexpr auto INDEX19 = THREAD {{0x80, 0x80, 0x80}, "Pewter", "ETP704"};
static constexpr auto INDEX20 = THREAD {{0x00, 0x00, 0x00}, "Black", "ETP900"};
static constexpr auto INDEX21 = THREAD {{0x00, 0x1c, 0xdf}, "Ultramarine", "ETP406"};
static constexpr auto INDEX22 = THREAD {{0xdf, 0x00, 0xb8}, "Royal Purple", "ETP869"};
static constexpr auto INDEX23 = THREAD {{0x62, 0x62, 0x62}, "Dark Gray", "ETP707"};
static constexpr auto INDEX24 = THREAD {{0x69, 0x26, 0x0d}, "Dark Brown", "ETP058"};
static constexpr auto INDEX25 = THREAD {{0xff, 0x00, 0x60}, "Deep Rose", "ETP086"};
static constexpr auto INDEX26 = THREAD {{0xbf, 0x82, 0x00}, "Light Brown", "ETP323"};
static constexpr auto INDEX27 = THREAD {{0xf3, 0x91, 0x78}, "Salmon Pink", "ETP079"};
static constexpr auto INDEX28 = THREAD {{0xff, 0x68, 0x05}, "Vermilion", "ETP030"};
static constexpr auto INDEX29 = THREAD {{0xf0, 0xf0, 0xf0}, "White", "ETP001"};
static constexpr auto INDEX30 = THREAD {{0xc8, 0x32, 0xcd}, "Violet", "ETP613"};
static constexpr auto INDEX31 = THREAD {{0xb0, 0xbf, 0x9b}, "Seacrest", "ETP542"};
static constexpr auto INDEX32 = THREAD {{0x65, 0xbf, 0xeb}, "Sky Blue", "ETP019"};
static constexpr auto INDEX33 = THREAD {{0xff, 0xba, 0x04}, "Pumpkin", "ETP126"};
static constexpr auto INDEX34 = THREAD {{0xff, 0xf0, 0x6c}, "Cream Yellow", "ETP010"};
static constexpr auto INDEX35 = THREAD {{0xfe, 0xca, 0x15}, "Khaki", "ETP348"};
static constexpr auto INDEX36 = THREAD {{0xf3, 0x81, 0x01}, "Clay Brown", "ETP339"};
static constexpr auto INDEX37 = THREAD {{0x37, 0xa9, 0x23}, "Leaf Green", "ETP509"};
static constexpr auto INDEX38 = THREAD {{0x23, 0x46, 0x5f}, "Peacock Blue", "ETP405"};
static constexpr auto INDEX39 = THREAD {{0xa6, 0xa6, 0x95}, "Gray", "ETP707"};
static constexpr auto INDEX40 = THREAD {{0xce, 0xbf, 0xa6}, "Warm Gray", "ETP399"};
static constexpr auto INDEX41 = THREAD {{0x96, 0xaa, 0x02}, "Dark Olive", "ETP517"};
static constexpr auto INDEX42 = THREAD {{0xff, 0xe3, 0xc6}, "Linen", "ETP307"};
static constexpr auto INDEX43 = THREAD {{0xff, 0x99, 0xd7}, "Pink", "ETP085"};
static constexpr auto INDEX44 = THREAD {{0x00, 0x70, 0x04}, "Deep Green", "ETP808"};
static constexpr auto INDEX45 = THREAD {{0xed, 0xcc, 0xfb}, "Lavender", "ETP804"};
static constexpr auto INDEX46 = THREAD {{0xc0, 0x89, 0xd8}, "Wisteria Violet", "ETP607"};
static constexpr auto INDEX47 = THREAD {{0xe7, 0xd9, 0xb4}, "Beige", "ETP843"};
static constexpr auto INDEX48 = THREAD {{0xe9, 0x0e, 0x86}, "Carmine", "ETP807"};
static constexpr auto INDEX49 = THREAD {{0xcf, 0x68, 0x29}, "Amber Red", "ETP333"};
static constexpr auto INDEX50 = THREAD {{0x40, 0x86, 0x15}, "Olive Green", "ETP519"};
static constexpr auto INDEX51 = THREAD {{0xdb, 0x17, 0x97}, "Dark Fuchsia", "ETP107"};
static constexpr auto INDEX52 = THREAD {{0xff, 0xa7, 0x04}, "Tangerine", "ETP209"};
static constexpr auto INDEX53 = THREAD {{0xb9, 0xff, 0xff}, "Light Blue", "ETP017"};
static constexpr auto INDEX54 = THREAD {{0x22, 0x89, 0x27}, "Emerald Green", "ETP507"};
static constexpr auto INDEX55 = THREAD {{0xb6, 0x12, 0xcd}, "Purple", "ETP614"};
static constexpr auto INDEX56 = THREAD {{0x00, 0xaa, 0x00}, "Moss Green", "ETP515"};
static constexpr auto INDEX57 = THREAD {{0xfe, 0xa9, 0xdc}, "Flesh Pink", "ETP124"};
static constexpr auto INDEX58 = THREAD {{0xfe, 0xd5, 0x10}, "Harvest Gold", "ETP206"};
static constexpr auto INDEX59 = THREAD {{0x00, 0x97, 0xdf}, "Electric Blue", "ETP420"};
static constexpr auto INDEX60 = THREAD {{0xff, 0xff, 0x84}, "Lemon Yellow", "ETP205"};
static constexpr auto INDEX61 = THREAD {{0xcf, 0xe7, 0x74}, "Fresh Green", "ETP027"};
static constexpr auto INDEX62 = THREAD {{0xff, 0xc8, 0x64}, "Applique Material", ""};
static constexpr auto INDEX63 = THREAD {{0xff, 0xc8, 0xc8}, "Applique Position", ""};
static constexpr auto INDEX64 = THREAD {{0xff, 0xc8, 0xc8}, "Applique", ""};

static constexpr auto PES_THREAD = std::array<THREAD, 65> {
    INDEX00, INDEX01, INDEX02, INDEX03, INDEX04, INDEX05, INDEX06, INDEX07, INDEX08, INDEX09,
    INDEX10, INDEX11, INDEX12, INDEX13, INDEX14, INDEX15, INDEX16, INDEX17, INDEX18, INDEX19,
    INDEX20, INDEX21, INDEX22, INDEX23, INDEX24, INDEX25, INDEX26, INDEX27, INDEX28, INDEX29,
    INDEX30, INDEX31, INDEX32, INDEX33, INDEX34, INDEX35, INDEX36, INDEX37, INDEX38, INDEX39,
    INDEX40, INDEX41, INDEX42, INDEX43, INDEX44, INDEX45, INDEX46, INDEX47, INDEX48, INDEX49,
    INDEX50, INDEX51, INDEX52, INDEX53, INDEX54, INDEX55, INDEX56, INDEX57, INDEX58, INDEX59,
    INDEX60, INDEX61, INDEX62, INDEX63, INDEX64};

constexpr auto THTYPCNT = PES_THREAD.size(); // Count of thread colors available in the PES format

// clang-format off
static constexpr auto IMAGE_WITH_FRAME = imgArray{{ 
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

auto pi::pesmtch(COLORREF const& referenceColor, uint8_t const& colorIndex) -> uint32_t {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  auto color = PEC_COLOR {GetRValue(referenceColor), GetGValue(referenceColor), GetBValue(referenceColor)};
  auto translatedColor = PES_THREAD[colorIndex].color;
  auto const meanR = (gsl::narrow_cast<int32_t>(color.r) + gsl::narrow_cast<int32_t>(translatedColor.r)) / 2;
  auto const deltaR = gsl::narrow_cast<int32_t>(color.r) - gsl::narrow_cast<int32_t>(translatedColor.r);
  auto const deltaG = gsl::narrow_cast<int32_t>(color.g) - gsl::narrow_cast<int32_t>(translatedColor.g);
  auto const deltaB = gsl::narrow_cast<int32_t>(color.b) - gsl::narrow_cast<int32_t>(translatedColor.b);
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula
  return wrap::round<uint32_t>(std::sqrt((((512 + meanR) * deltaR * deltaR) / 256) + 4 * deltaG * deltaG +
                                         (((767 - meanR) * deltaB * deltaB) / 256)));
}

void pi::ritpes(std::vector<uint8_t>& buffer, F_POINT_ATTR const& stitch, F_POINT const& offset) {
  auto const oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESTCH));
  auto* pesStitch = convertFromPtr<PESTCH*>(&buffer[oldSize]);
  auto const scaledStitch = F_POINT {-stitch.x * IPECFACT + offset.x, stitch.y * IPECFACT - offset.y};
  pesStitch->x = wrap::round<int16_t>(scaledStitch.x);
  pesStitch->y = wrap::round<int16_t>(scaledStitch.y);
}

void pi::ritpesCode(std::vector<uint8_t>& buffer) {
  constexpr auto ENDCODE = uint16_t {0x8003U}; // Block end code
  auto const     oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(uint16_t));
  auto* contCode = convertFromPtr<uint16_t*>(&buffer[oldSize]);
  if (contCode != nullptr) {
	*contCode = ENDCODE;
  }
}

void pi::ritpesBlock(std::vector<uint8_t>& buffer, PESSTCHLST newBlock) {
  auto const oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESSTCHLST));
  auto* blockHeader = convertFromPtr<PESSTCHLST*>(&buffer[oldSize]);
  *blockHeader      = newBlock;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void pi::pecnam(gsl::span<char> const& label) {
  // ReSharper disable once CppDeprecatedEntity
  strncpy(label.data(), "LA:", 3); // NOLINT(clang-diagnostic-deprecated-declarations)
  auto const lblSize  = wrap::toUnsigned(label.size() - 3U);
  auto       fileStem = utf::utf16ToUtf8(AuxName->stem());
  if (fileStem.size() < lblSize) {
	fileStem += std::string(lblSize - fileStem.size(), ' ');
  }
  auto* ptr = std::next(label.data(), 3U);
  // ReSharper disable once CppDeprecatedEntity
  strncpy(ptr, fileStem.c_str(), lblSize); // NOLINT(clang-diagnostic-deprecated-declarations)
}
#pragma warning(pop)

void pi::pecEncodeInt(std::vector<uint8_t>& buffer, int32_t delta) {
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

void pi::rpcrd(std::vector<uint8_t>& buffer, F_POINT& thisStitch, float srcX, float srcY) {
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
  thisStitch.x += wrap::toFloat(deltaX) * IPECFACT;
  thisStitch.y += -wrap::toFloat(deltaY) * IPECFACT;
}

void pi::pecEncodeStop(std::vector<uint8_t>& buffer, uint8_t val) {
  buffer.push_back(0xfe);
  buffer.push_back(0xb0);
  buffer.push_back(val);
}

void pi::pecdat(std::vector<uint8_t>& buffer) {
  auto*      pecHeader  = convertFromPtr<PECHDR*>(buffer.data());
  auto&      pad        = pecHeader->pad;
  auto const pesColors  = gsl::span<uint8_t> {std::begin(pad), sizeof(pad)};
  auto       thisStitch = F_POINT {};
  rpcrd(buffer, thisStitch, StitchBuffer->front().x, StitchBuffer->front().y);
  auto iColor  = 1U;
  auto color   = StitchBuffer->front().attribute & COLMSK;
  auto iPEC    = wrap::next(PESequivColors.begin(), color);
  pesColors[0] = *iPEC;
  for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size() - 1U); ++iStitch) {
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	if ((stitchFwd1.attribute & COLMSK) != color) {
	  color = stitchFwd1.attribute & COLMSK;
	  pecEncodeStop(buffer, (iColor % 2U) + 1U);
	  iPEC              = wrap::next(PESequivColors.begin(), color);
	  pesColors[iColor] = *iPEC;
	  ++iColor;
	}
	auto const xDelta = stitchFwd1.x - thisStitch.x;
	auto const yDelta = stitchFwd1.y - thisStitch.y;
	rpcrd(buffer, thisStitch, xDelta, yDelta);
  }
  buffer.push_back(0xffU);
  buffer.push_back(0x0U);
}

void pi::writeThumbnail(std::vector<uint8_t>& buffer, imgArray const& image) {
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

void pi::pecImage(std::vector<uint8_t>& pecBuffer) {
  auto       thumbnail = IMAGE_WITH_FRAME;
  auto const yFactor   = 31.0F / IniFile.hoopSizeY;
  auto const xFactor   = 40.0F / IniFile.hoopSizeX;
  // write the overall thumbnail
  constexpr auto XOFFSET = uint8_t {4U}; // thumbnail x offset to place it in the frame correctly
  constexpr auto YOFFSET = uint8_t {5U}; // thumbnail y offset to place it in the frame correctly
  for (auto const& stitch : *StitchBuffer) {
	auto const xCoord = wrap::toPtrdiff(wrap::floor<uint16_t>(stitch.x * xFactor) + XOFFSET);
	auto const yCoord = wrap::toPtrdiff(THUMBHGT - (wrap::floor<uint16_t>(stitch.y * yFactor) + YOFFSET));
	auto const iThumbnail = std::next(thumbnail.begin(), yCoord);
	auto const iRow       = std::next((*iThumbnail).begin(), xCoord);
	*iRow                 = 1U;
  }
  pi::writeThumbnail(pecBuffer, thumbnail);
  // now write out the individual thread thumbnails
  thumbnail        = IMAGE_WITH_FRAME;
  auto stitchColor = (StitchBuffer->front().attribute & COLMSK);
  for (auto const& stitch : *StitchBuffer) {
	auto const xCoord = wrap::toPtrdiff(wrap::floor<uint16_t>(stitch.x * xFactor) + XOFFSET);
	auto const yCoord = wrap::toPtrdiff(THUMBHGT - (wrap::floor<uint16_t>(stitch.y * yFactor) + YOFFSET));
	auto const iThumbnail = std::next(thumbnail.begin(), yCoord);
	auto const iRow       = std::next((*iThumbnail).begin(), xCoord);
	if (stitchColor == (stitch.attribute & COLMSK)) {
	  *iRow = 1U;
	}
	else {
	  pi::writeThumbnail(pecBuffer, thumbnail);
	  thumbnail   = IMAGE_WITH_FRAME;
	  stitchColor = (stitch.attribute & COLMSK);
	  *iRow       = 1U;
	}
  }
  pi::writeThumbnail(pecBuffer, thumbnail);
}

auto pi::dupcol(gsl::span<uint8_t> const& pesColors, uint32_t activeColor, uint32_t& index) -> uint32_t {
  auto const& threadColor = PES_THREAD[pesColors[index++] % THTYPCNT];
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  auto const color      = RGB(threadColor.color.r, threadColor.color.g, threadColor.color.b);
  auto       iUserColor = UserColor.cbegin();
  for (auto iColor = 0U; iColor < activeColor; ++iColor) {
	if (*iUserColor == color) {
	  return iColor;
	}
	++iUserColor;
  }
  auto minimumDistance = std::numeric_limits<uint32_t>::max();
  auto matchIndex      = 0U;
  for (auto iColor = 1U; iColor < activeColor; ++iColor) {
	auto const matchDistance = pesmtch(color, pesColors[iColor]);
	if (matchDistance < minimumDistance) {
	  minimumDistance = matchDistance;
	  matchIndex      = iColor;
	}
  }
  return matchIndex;
}

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

  constexpr auto PESSTR = "#PES"; // PES lead in value
  if (strncmp(static_cast<const char*>(pesHeader->ledI), PESSTR, strlen(PESSTR)) != 0) {
	displayText::showMessage(IDS_NOTPES, newFileName.wstring());
	CloseHandle(fileHandle);
	return false;
  }
  auto        contFlag      = false;
  const char* verStrArray[] = {
      "0001", "0020", "0022", "0030", "0040", "0050", "0055", "0056", "0060", "0070", "0080", "0090", "0100"};
  for (auto const& version : verStrArray) {
	if (strncmp(static_cast<const char*>(pesHeader->ledV), version, strlen(version)) == 0) {
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
  auto&      pad           = pecHeader->pad;
  auto const pesColors     = gsl::span<uint8_t> {std::begin(pad), pesColorCount};
  auto       colorMap      = boost::dynamic_bitset<>(THTYPCNT);
  auto       iUserColor    = UserColor.begin();
  for (auto iColor = 0U; iColor < pesColorCount; ++iColor) {
	if (pesColors[iColor] < THTYPCNT) {
	  if (colorMap.test_set(pesColors[iColor])) {
		continue;
	  }
	  auto const& threadColor = PES_THREAD[pesColors[iColor]];
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  auto const color = RGB(threadColor.color.r, threadColor.color.g, threadColor.color.b);
	  *iUserColor      = color;
	  ++iUserColor;
	  if (iUserColor == UserColor.end()) {
		break;
	  }
	  continue;
	}
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	auto const color = RGB(PES_THREAD[0].color.r,
	                       PES_THREAD[0].color.g,
	                       PES_THREAD[0].color.b); // color unknown

	*iUserColor = color;
	++iUserColor;
  }
  auto loc = F_POINT {};
  StateMap->reset(StateFlag::FILDIR);
  if (bytesRead <= ((pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U)) {
	displayText::showMessage(IDS_NOTPES, newFileName.wstring());
	CloseHandle(fileHandle);
	return false;
  }
  auto       color      = 0U;
  auto       iPESstitch = size_t {0U};
  auto const pecCount   = bytesRead - (pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U;
  StitchBuffer->clear();
  StitchBuffer->reserve(pecCount / 2); // we are still reserving a bit more than necessary
  StitchBuffer->push_back(F_POINT_ATTR {});
  constexpr auto MSK12BIT = uint32_t {0xFFFU}; // used to mask the value to 12 bits

  auto       pesColorIndex = uint32_t {1U};
  auto const pesStitches   = gsl::span<const uint8_t> {pesStitch, pecCount};
  while (iPESstitch < pecCount) {
	if (pesStitches[iPESstitch] == 0xff && pesStitches[iPESstitch + 1U] == 0) {
	  break;
	}
	if (pesStitches[iPESstitch] == 0xfe && pesStitches[iPESstitch + 1U] == 0xb0) {
	  color = pi::dupcol(pesColors, wrap::toUnsigned(std::distance(UserColor.begin(), iUserColor)), pesColorIndex);
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
		if (pesStitches[iPESstitch] > 0x3f) {
		  locof = wrap::toFloat(pesStitches[iPESstitch]) - 128.0F;
		}
		else {
		  locof = pesStitches[iPESstitch];
		}
	  }
	  locof *= IPECFACT;
	  // ToDo - (PES) Use a new flag bit for this since FILDIR is not correct
	  if (StateMap->testAndFlip(StateFlag::FILDIR)) {
		loc.y -= locof;
		StitchBuffer->push_back(F_POINT_ATTR {loc.x, loc.y, color});
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
      auxName.wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT(hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::crmsg(auxName);
	return false;
  }
  auto pesHeader = PESHED {};
  if ((IniFile.hoopSizeX <= HUP100XY) && (IniFile.hoopSizeY <= HUP100XY)) {
	pesHeader.hpsz = 0;
  }
  else {
	if ((IniFile.hoopSizeX <= LHUPX) && (IniFile.hoopSizeY <= LHUPY)) {
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
  constexpr auto SEWSTR  = "CSewSeg"; // sewing segment leadin
  // ReSharper disable once CppDeprecatedEntity
  strncpy(static_cast<char*>(pesHeader.ledI), PESISTR, strlen(PESISTR)); // NOLINT(clang-diagnostic-deprecated-declarations)
  // ReSharper disable once CppDeprecatedEntity
  strncpy(static_cast<char*>(pesHeader.ledV), PESVSTR, strlen(PESVSTR)); // NOLINT(clang-diagnostic-deprecated-declarations)
  wrap::narrow(pesHeader.celn, strlen(EMBSTR));
  // ReSharper disable once CppDeprecatedEntity
  strncpy(static_cast<char*>(pesHeader.ce), EMBSTR, pesHeader.celn); // NOLINT(clang-diagnostic-deprecated-declarations)
  wrap::narrow(pesHeader.cslen, strlen(SEWSTR));
  // ReSharper disable once CppDeprecatedEntity
  strncpy(static_cast<char*>(pesHeader.cs), SEWSTR, pesHeader.cslen); // NOLINT(clang-diagnostic-deprecated-declarations)
  {
	auto iPEC = PESequivColors.begin();
	for (auto const& color : UserColor) {
	  auto matchIndex = 0U;
	  auto matchMin   = std::numeric_limits<uint32_t>::max();
	  for (auto iColorMatch = 1U; iColorMatch < THTYPCNT; ++iColorMatch) {
		auto const match = pi::pesmtch(color, gsl::narrow_cast<uint8_t>(iColorMatch));
		if (match < matchMin) {
		  matchIndex = iColorMatch;
		  matchMin   = match;
		}
	  }
	  wrap::narrow(*iPEC, matchIndex);
	  ++iPEC;
	}
  }
  auto stitchColor  = StitchBuffer->front().attribute & COLMSK;
  auto boundingRect = F_RECTANGLE {};
  thred::sizstch(boundingRect, *StitchBuffer);
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
  pesHeader.atfm5        = 350.0F + (hoopSizeX / 2.0F) - (designSizeX / 2.0F);
  pesHeader.atfm6        = 100.0F + (designSizeY) + (hoopSizeY / 2.0F) - (designSizeY / 2.0F);
  auto pesBuffer         = std::vector<uint8_t> {};
  // ToDo - make a reasonable guess for the size of data in the PES buffer. err on the side of caution
  auto const pesSize = sizeof(PESSTCHLST) + StitchBuffer->size() * sizeof(PESTCH) + 1000U;
  pesBuffer.reserve(pesSize);
  auto threadList      = std::vector<PES_COLOR_LIST> {};
  auto blockIndex      = uint16_t {}; // Index into the stitch blocks
  auto iPESequivColors = wrap::next(PESequivColors.begin(), stitchColor);
  auto currentColor    = *iPESequivColors;
  threadList.push_back(PES_COLOR_LIST {blockIndex, currentColor});
  pesBuffer.resize(sizeof(PESSTCHLST));
  // first block is a jump in place
  pi::ritpesBlock(pesBuffer, PESSTCHLST {1, currentColor, 2});
  ++blockIndex;
  pi::ritpes(pesBuffer, saveStitches[0], offset);
  pi::ritpes(pesBuffer, saveStitches[0], offset);
  pi::ritpesCode(pesBuffer);
  // then a normal stitch in place
  pi::ritpesBlock(pesBuffer, PESSTCHLST {0, currentColor, 2});
  ++blockIndex;
  pi::ritpes(pesBuffer, saveStitches[0], offset);
  pi::ritpes(pesBuffer, saveStitches[0], offset);
  pi::ritpesCode(pesBuffer);
  // then a jump to the first location
  pi::ritpesBlock(pesBuffer, PESSTCHLST {1, currentColor, 2});
  ++blockIndex;
  pi::ritpes(pesBuffer, saveStitches[0], offset);
  pi::ritpes(pesBuffer, saveStitches[1], offset);
  pi::ritpesCode(pesBuffer);
  // now stitch out.
  auto pesThreadCount = 0U;
  auto lastIndex      = pesBuffer.size();
  pi::ritpesBlock(pesBuffer, PESSTCHLST {0, currentColor, 0});
  ++blockIndex;
  auto stitchCount = 0U;
  for (auto iStitch = 1U; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	if (stitchColor == (StitchBuffer->operator[](iStitch).attribute & COLMSK)) {
	  // we are in the same color block, so write the stitch
	  pi::ritpes(pesBuffer, saveStitches[iStitch], offset);
	  ++stitchCount;
	}
	else {
	  // write stitch
	  pi::ritpesCode(pesBuffer);
	  // close out the previous block
	  auto* blockHeader        = convertFromPtr<PESSTCHLST*>(&pesBuffer[lastIndex]);
	  blockHeader->stitchcount = gsl::narrow<uint16_t>(stitchCount);
	  // save the thread/color information
	  ++pesThreadCount;
	  stitchColor     = StitchBuffer->operator[](iStitch).attribute & COLMSK;
	  iPESequivColors = wrap::next(PESequivColors.begin(), stitchColor);
	  currentColor    = *iPESequivColors;
	  threadList.push_back(PES_COLOR_LIST {blockIndex, currentColor});
	  // then create the jump block
	  pi::ritpesBlock(pesBuffer, PESSTCHLST {1, currentColor, 2});
	  ++blockIndex;
	  pi::ritpes(pesBuffer, saveStitches[iStitch - 1U], offset);
	  pi::ritpes(pesBuffer, saveStitches[iStitch], offset);
	  pi::ritpesCode(pesBuffer);
	  // and finally start the next block
	  stitchCount = 0U;
	  lastIndex   = pesBuffer.size();
	  pi::ritpesBlock(pesBuffer, PESSTCHLST {0, currentColor, 0});
	  ++blockIndex;
	  pi::ritpes(pesBuffer, saveStitches[iStitch], offset);
	  ++stitchCount;
	}
  }
  // finalize the last stitch block
  auto* blockHeader        = convertFromPtr<PESSTCHLST*>(&pesBuffer[lastIndex]);
  blockHeader->stitchcount = gsl::narrow_cast<uint16_t>(stitchCount);
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
	pesBuffer.resize(lastIndex + 2 * sizeof(uint16_t));
	auto* colorEntry = convertFromPtr<uint16_t*>(&pesBuffer[lastIndex]);
	if (colorEntry == nullptr) {
	  CloseHandle(fileHandle);
	  return false;
	}
	auto const colorEntries = gsl::span<uint16_t> {colorEntry, 2};
	colorEntries[0]         = threadList[paletteIndex].blockIndex;
	colorEntries[1]         = threadList[paletteIndex].colorIndex;
  }
  pesHeader.off     = wrap::toUnsigned(pesBuffer.size() + sizeof(pesHeader));
  pesHeader.blct    = 1;
  pesHeader.hnd1[0] = 0xff;
  pesHeader.hnd1[1] = 0xff;
  pesHeader.hnd1[2] = 0x00;
  pesHeader.hnd1[3] = 0x00;
  pesHeader.bcnt    = gsl::narrow_cast<uint16_t>(pesThreadCount);
  pesHeader.hnd2[0] = 0xff;
  pesHeader.hnd2[1] = 0xff;
  pesHeader.hnd2[2] = 0x00;
  pesHeader.hnd2[3] = 0x00;
  GroupStartStitch  = 0;
  GroupEndStitch    = wrap::toUnsigned(StitchBuffer->size() - 1U);
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
  auto const pecSize = sizeof(PECHDR) + sizeof(PECHDR2) + StitchBuffer->size() * 2 + 1000 +
                       (wrap::toSize(pesThreadCount) + 1U) * THUMBHGT * (THUMBWID / 8);
  pecBuffer.reserve(pecSize);
  pecBuffer.resize(sizeof(PECHDR) + sizeof(PECHDR2));
  auto*      pecHeader = convertFromPtr<PECHDR*>(pecBuffer.data());
  auto const spPHL     = gsl::span {pecHeader->label};
  pi::pecnam(spPHL);
  auto fstart = std::next(pecBuffer.begin(), sizeof(pecHeader->label));
  auto fend   = std::next(pecBuffer.begin(), sizeof(*pecHeader));
  std::fill(fstart, fend, ' ');
  pecHeader->labnd = '\r'; // 13 = carriage return
  wrap::narrow(pecHeader->colorCount, pesThreadCount);
  pecHeader->hnd1        = 0x00ff;
  pecHeader->thumbHeight = THUMBHGT;
  pecHeader->thumbWidth  = THUMBWID / BTSPBYTE;
  pi::pecdat(pecBuffer);
  auto* pecHeader2            = convertFromPtr<PECHDR2*>(&pecBuffer[sizeof(PECHDR)]);
  pecHeader2->unknown1        = 0;
  pecHeader2->thumbnailOffset = gsl::narrow<uint16_t>(pecBuffer.size() - sizeof(PECHDR));
  pecHeader2->unknown2        = 0x3100;
  pecHeader2->unknown3        = 0xf0ff;
  pecHeader2->width           = pesHeader.xsiz;
  pecHeader2->height          = pesHeader.ysiz;
  pecHeader2->unknown4        = 0x01e0;
  pecHeader2->unknown5        = 0x01b0;

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
  pi::pecImage(pecBuffer);
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
