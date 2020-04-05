#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
//#include <Windows.h> // Win32 Platform SDK main header

#ifdef ALLOCFAILURE
//#include <new.h>
#endif

// Standard Libraries
//#include <bitset>

// Open Source headers
//#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
//#include <boost/dynamic_bitset.hpp>
//#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "thred.h"
#include "DST.h"

namespace di = DST::internal;

fs::path* ColorFileName; //.thw file name
fs::path* RGBFileName;   //.rgb file name

class DSTDAT
{
  public:
  char cor {0};
  char val {0};

  constexpr DSTDAT() noexcept = default;
  // DSTDAT(DSTDAT&&) = default;
  // DSTDAT& operator=(DSTDAT const& rhs) = default;
  // DSTDAT& operator=(DSTDAT&&) = default;
  //~DSTDAT() = default;
};

constexpr auto XCOR = char {0};
constexpr auto YCOR = char {1};

DSTDAT const DSTValues[] = {
    {XCOR, 1}, {XCOR, -1}, {XCOR, 9},  {XCOR, -9},  {YCOR, -9},  {YCOR, 9},  {YCOR, -1}, {YCOR, 1},
    {XCOR, 3}, {XCOR, -3}, {XCOR, 27}, {XCOR, -27}, {YCOR, -27}, {YCOR, 27}, {YCOR, -3}, {YCOR, 3},
    {XCOR, 0}, {XCOR, 0},  {XCOR, 81}, {XCOR, -81}, {YCOR, -81}, {YCOR, 81}};

uint32_t const Xdst[] = {
    0x090a0a, //-121
    0x090a08, //-120
    0x090a09, //-119
    0x09080a, //-118
    0x090808, //-117
    0x090809, //-116
    0x09090a, //-115
    0x090908, //-114
    0x090909, //-113
    0x090a02, //-112
    0x090a00, //-111
    0x090a01, //-110
    0x090802, //-109
    0x090800, //-108
    0x090801, //-107
    0x090902, //-106
    0x090900, //-105
    0x090901, //-104
    0x090a06, //-103
    0x090a04, //-102
    0x090a05, //-101
    0x090806, //-100
    0x090804, //-99
    0x090805, //-98
    0x090906, //-97
    0x090904, //-96
    0x090905, //-95
    0x09020a, //-94
    0x090208, //-93
    0x090209, //-92
    0x09000a, //-91
    0x090008, //-90
    0x090009, //-89
    0x09010a, //-88
    0x090108, //-87
    0x090109, //-86
    0x090202, //-85
    0x090200, //-84
    0x090201, //-83
    0x090002, //-82
    0x090000, //-81
    0x090001, //-80
    0x090102, //-79
    0x090100, //-78
    0x090101, //-77
    0x090206, //-76
    0x090204, //-75
    0x090205, //-74
    0x090006, //-73
    0x090004, //-72
    0x090005, //-71
    0x090106, //-70
    0x090104, //-69
    0x090105, //-68
    0x09060a, //-67
    0x090608, //-66
    0x090609, //-65
    0x09040a, //-64
    0x090408, //-63
    0x090409, //-62
    0x09050a, //-61
    0x090508, //-60
    0x090509, //-59
    0x090602, //-58
    0x090600, //-57
    0x090601, //-56
    0x090402, //-55
    0x090400, //-54
    0x090401, //-53
    0x090502, //-52
    0x090500, //-51
    0x090501, //-50
    0x090606, //-49
    0x090604, //-48
    0x090605, //-47
    0x090406, //-46
    0x090404, //-45
    0x090405, //-44
    0x090506, //-43
    0x090504, //-42
    0x090505, //-41
    0x010a0a, //-40
    0x010a08, //-39
    0x010a09, //-38
    0x01080a, //-37
    0x010808, //-36
    0x010809, //-35
    0x01090a, //-34
    0x010908, //-33
    0x010909, //-32
    0x010a02, //-31
    0x010a00, //-30
    0x010a01, //-29
    0x010802, //-28
    0x010800, //-27
    0x010801, //-26
    0x010902, //-25
    0x010900, //-24
    0x010901, //-23
    0x010a06, //-22
    0x010a04, //-21
    0x010a05, //-20
    0x010806, //-19
    0x010804, //-18
    0x010805, //-17
    0x010906, //-16
    0x010904, //-15
    0x010905, //-14
    0x01020a, //-13
    0x010208, //-12
    0x010209, //-11
    0x01000a, //-10
    0x010008, //-9
    0x010009, //-8
    0x01010a, //-7
    0x010108, //-6
    0x010109, //-5
    0x010202, //-4
    0x010200, //-3
    0x010201, //-2
    0x010002, //-1
    0x010000, // 0
    0x010001, // 1
    0x010102, // 2
    0x010100, // 3
    0x010101, // 4
    0x010206, // 5
    0x010204, // 6
    0x010205, // 7
    0x010006, // 8
    0x010004, // 9
    0x010005, // 10
    0x010106, // 11
    0x010104, // 12
    0x010105, // 13
    0x01060a, // 14
    0x010608, // 15
    0x010609, // 16
    0x01040a, // 17
    0x010408, // 18
    0x010409, // 19
    0x01050a, // 20
    0x010508, // 21
    0x010509, // 22
    0x010602, // 23
    0x010600, // 24
    0x010601, // 25
    0x010402, // 26
    0x010400, // 27
    0x010401, // 28
    0x010502, // 29
    0x010500, // 30
    0x010501, // 31
    0x010606, // 32
    0x010604, // 33
    0x010605, // 34
    0x010406, // 35
    0x010404, // 36
    0x010405, // 37
    0x010506, // 38
    0x010504, // 39
    0x010505, // 40
    0x050a0a, // 41
    0x050a08, // 42
    0x050a09, // 43
    0x05080a, // 44
    0x050808, // 45
    0x050809, // 46
    0x05090a, // 47
    0x050908, // 48
    0x050909, // 49
    0x050a02, // 50
    0x050a00, // 51
    0x050a01, // 52
    0x050802, // 53
    0x050800, // 54
    0x050801, // 55
    0x050902, // 56
    0x050900, // 57
    0x050901, // 58
    0x050a06, // 59
    0x050a04, // 60
    0x050a05, // 61
    0x050806, // 62
    0x050804, // 63
    0x050805, // 64
    0x050906, // 65
    0x050904, // 66
    0x050905, // 67
    0x05020a, // 68
    0x050208, // 69
    0x050209, // 70
    0x05000a, // 71
    0x050008, // 72
    0x050009, // 73
    0x05010a, // 74
    0x050108, // 75
    0x050109, // 76
    0x050202, // 77
    0x050200, // 78
    0x050201, // 79
    0x050002, // 80
    0x050000, // 81
    0x050001, // 82
    0x050102, // 83
    0x050100, // 84
    0x050101, // 85
    0x050206, // 86
    0x050204, // 87
    0x050205, // 88
    0x050006, // 89
    0x050004, // 90
    0x050005, // 91
    0x050106, // 92
    0x050104, // 93
    0x050105, // 94
    0x05060a, // 95
    0x050608, // 96
    0x050609, // 97
    0x05040a, // 98
    0x050408, // 99
    0x050409, // 100
    0x05050a, // 101
    0x050508, // 102
    0x050509, // 103
    0x050602, // 104
    0x050600, // 105
    0x050601, // 106
    0x050402, // 107
    0x050400, // 108
    0x050401, // 109
    0x050502, // 110
    0x050500, // 111
    0x050501, // 112
    0x050606, // 113
    0x050604, // 114
    0x050605, // 115
    0x050406, // 116
    0x050404, // 117
    0x050405, // 118
    0x050506, // 119
    0x050504, // 120
    0x050505  // 121
};

uint32_t const Ydst[] = {
    0x115050, //-121
    0x115010, //-120
    0x115090, //-119
    0x111050, //-118
    0x111010, //-117
    0x111090, //-116
    0x119050, //-115
    0x119010, //-114
    0x119090, //-113
    0x115040, //-112
    0x115000, //-111
    0x115080, //-110
    0x111040, //-109
    0x111000, //-108
    0x111080, //-107
    0x119040, //-106
    0x119000, //-105
    0x119080, //-104
    0x115060, //-103
    0x115020, //-102
    0x1150a0, //-101
    0x111060, //-100
    0x111020, //-99
    0x1110a0, //-98
    0x119060, //-97
    0x119020, //-96
    0x1190a0, //-95
    0x114050, //-94
    0x114010, //-93
    0x114090, //-92
    0x110050, //-91
    0x110010, //-90
    0x110090, //-89
    0x118050, //-88
    0x118010, //-87
    0x118090, //-86
    0x114040, //-85
    0x114000, //-84
    0x114080, //-83
    0x110040, //-82
    0x110000, //-81
    0x110080, //-80
    0x118040, //-79
    0x118000, //-78
    0x118080, //-77
    0x114060, //-76
    0x114020, //-75
    0x1140a0, //-74
    0x110060, //-73
    0x110020, //-72
    0x1100a0, //-71
    0x118060, //-70
    0x118020, //-69
    0x1180a0, //-68
    0x116050, //-67
    0x116010, //-66
    0x116090, //-65
    0x112050, //-64
    0x112010, //-63
    0x112090, //-62
    0x11a050, //-61
    0x11a010, //-60
    0x11a090, //-59
    0x116040, //-58
    0x116000, //-57
    0x116080, //-56
    0x112040, //-55
    0x112000, //-54
    0x112080, //-53
    0x11a040, //-52
    0x11a000, //-51
    0x11a080, //-50
    0x116060, //-49
    0x116020, //-48
    0x1160a0, //-47
    0x112060, //-46
    0x112020, //-45
    0x1120a0, //-44
    0x11a060, //-43
    0x11a020, //-42
    0x11a0a0, //-41
    0x015050, //-40
    0x015010, //-39
    0x015090, //-38
    0x011050, //-37
    0x011010, //-36
    0x011090, //-35
    0x019050, //-34
    0x019010, //-33
    0x019090, //-32
    0x015040, //-31
    0x015000, //-30
    0x015080, //-29
    0x011040, //-28
    0x011000, //-27
    0x011080, //-26
    0x019040, //-25
    0x019000, //-24
    0x019080, //-23
    0x015060, //-22
    0x015020, //-21
    0x0150a0, //-20
    0x011060, //-19
    0x011020, //-18
    0x0110a0, //-17
    0x019060, //-16
    0x019020, //-15
    0x0190a0, //-14
    0x014050, //-13
    0x014010, //-12
    0x014090, //-11
    0x010050, //-10
    0x010010, //-9
    0x010090, //-8
    0x018050, //-7
    0x018010, //-6
    0x018090, //-5
    0x014040, //-4
    0x014000, //-3
    0x014080, //-2
    0x010040, //-1
    0x010000, // 0
    0x010080, // 1
    0x018040, // 2
    0x018000, // 3
    0x018080, // 4
    0x014060, // 5
    0x014020, // 6
    0x0140a0, // 7
    0x010060, // 8
    0x010020, // 9
    0x0100a0, // 10
    0x018060, // 11
    0x018020, // 12
    0x0180a0, // 13
    0x016050, // 14
    0x016010, // 15
    0x016090, // 16
    0x012050, // 17
    0x012010, // 18
    0x012090, // 19
    0x01a050, // 20
    0x01a010, // 21
    0x01a090, // 22
    0x016040, // 23
    0x016000, // 24
    0x016080, // 25
    0x012040, // 26
    0x012000, // 27
    0x012080, // 28
    0x01a040, // 29
    0x01a000, // 30
    0x01a080, // 31
    0x016060, // 32
    0x016020, // 33
    0x0160a0, // 34
    0x012060, // 35
    0x012020, // 36
    0x0120a0, // 37
    0x01a060, // 38
    0x01a020, // 39
    0x01a0a0, // 40
    0x215050, // 41
    0x215010, // 42
    0x215090, // 43
    0x211050, // 44
    0x211010, // 45
    0x211090, // 46
    0x219050, // 47
    0x219010, // 48
    0x219090, // 49
    0x215040, // 50
    0x215000, // 51
    0x215080, // 52
    0x211040, // 53
    0x211000, // 54
    0x211080, // 55
    0x219040, // 56
    0x219000, // 57
    0x219080, // 58
    0x215060, // 59
    0x215020, // 60
    0x2150a0, // 61
    0x211060, // 62
    0x211020, // 63
    0x2110a0, // 64
    0x219060, // 65
    0x219020, // 66
    0x2190a0, // 67
    0x214050, // 68
    0x214010, // 69
    0x214090, // 70
    0x210050, // 71
    0x210010, // 72
    0x210090, // 73
    0x218050, // 74
    0x218010, // 75
    0x218090, // 76
    0x214040, // 77
    0x214000, // 78
    0x214080, // 79
    0x210040, // 80
    0x210000, // 81
    0x210080, // 82
    0x218040, // 83
    0x218000, // 84
    0x218080, // 85
    0x214060, // 86
    0x214020, // 87
    0x2140a0, // 88
    0x210060, // 89
    0x210020, // 90
    0x2100a0, // 91
    0x218060, // 92
    0x218020, // 93
    0x2180a0, // 94
    0x216050, // 95
    0x216010, // 96
    0x216090, // 97
    0x212050, // 98
    0x212010, // 99
    0x212090, // 100
    0x21a050, // 101
    0x21a010, // 102
    0x21a090, // 103
    0x216040, // 104
    0x216000, // 105
    0x216080, // 106
    0x212040, // 107
    0x212000, // 108
    0x212080, // 109
    0x21a040, // 110
    0x21a000, // 111
    0x21a080, // 112
    0x216060, // 113
    0x216020, // 114
    0x2160a0, // 115
    0x212060, // 116
    0x212020, // 117
    0x2120a0, // 118
    0x21a060, // 119
    0x21a020, // 120
    0x21a0a0  // 121
};

void DST::internal::dstin(uint32_t number, POINT& pout) noexcept {
  // ToDo - what is this code doing?
  auto shift = 1U;
  pout       = POINT {};
  for (auto const& DSTValue : DSTValues) {
	if ((number & shift) != 0U) {
	  if (DSTValue.cor != 0) {
		pout.y += DSTValue.val;
	  }
	  else {
		pout.x += DSTValue.val;
	  }
	}
	shift <<= 1U;
  }
}

void DST::internal::dstran(std::vector<DSTREC>& DSTData) {
  auto bytesRead = DWORD {0};
  auto colors    = std::vector<uint32_t> {};
  if (di::colfil()) {
	auto *colorFile =
	    CreateFile(ColorFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (colorFile != INVALID_HANDLE_VALUE) { // NOLINT
	  auto colorFileSize = LARGE_INTEGER {};
	  GetFileSizeEx(colorFile, &colorFileSize);
	  // There can only be (64K + 3) colors, so even if HighPart is non-zero, we don't care
	  colors.resize(colorFileSize.u.LowPart / sizeof(decltype(colors.back())));
	  ReadFile(colorFile, colors.data(), colorFileSize.u.LowPart, &bytesRead, nullptr);
	  CloseHandle(colorFile);
	  if (bytesRead > (sizeof(decltype(colors.back())) * 2)) {
		if (colors[0] == COLVER) {
		  BackgroundColor = colors[1];
		  thred::resetColorChanges();
		}
	  }
	}
  }
  auto iColor = 2U;
  auto color  = 0U;
  if (bytesRead >= ((wrap::toSize(iColor) + 1U) * sizeof(decltype(colors.back())))) {
	color = colmatch(colors[iColor++]);
  }
  else {
	color = 0;
  }
  auto localStitch       = fPOINT {};
  auto maximumCoordinate = fPOINT {-1e12F, -1e12F};
  auto mimimumCoordinate = fPOINT {1e12F, 1e12F};
  for (auto& record : DSTData) {
	if ((record.nd & 0x40U) != 0) {
	  if (bytesRead >= ((wrap::toSize(iColor) + 1U) * sizeof(decltype(colors.back())))) {
		color = colmatch(colors[iColor++]);
	  }
	  else {
		color++;
		color &= 0xfU;
	  }
	}
	else {
	  auto dstStitch = POINT {0L, 0L};
	  di::dstin(di::dtrn(&record), dstStitch);
	  localStitch.x += dstStitch.x;
	  localStitch.y += dstStitch.y;
	  if ((record.nd & 0x80U) == 0U) {
		StitchBuffer->push_back(fPOINTATTR {localStitch.x * 0.6F, localStitch.y * 0.6F, color | NOTFRM});
		auto& stitch = StitchBuffer->back();
		if (stitch.x > maximumCoordinate.x) {
		  maximumCoordinate.x = stitch.x;
		}
		if (stitch.y > maximumCoordinate.y) {
		  maximumCoordinate.y = stitch.y;
		}
		if (stitch.x < mimimumCoordinate.x) {
		  mimimumCoordinate.x = stitch.x;
		}
		if (stitch.y < mimimumCoordinate.y) {
		  mimimumCoordinate.y = stitch.y;
		}
	  }
	}
  }
  auto const dstSize =
      fPOINT {maximumCoordinate.x - mimimumCoordinate.x, maximumCoordinate.y - mimimumCoordinate.y};
  IniFile.hoopType = CUSTHUP;
  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
  if (dstSize.x > UnzoomedRect.x || dstSize.y > UnzoomedRect.y) {
	IniFile.hoopSizeX = dstSize.x * 1.1F;
	IniFile.hoopSizeY = dstSize.y * 1.1F;
	UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
	displayText::hsizmsg();
  }
  auto const delta =
      fPOINT {(gsl::narrow_cast<float>(UnzoomedRect.x) - dstSize.x) / 2.0F - mimimumCoordinate.x,
              (gsl::narrow_cast<float>(UnzoomedRect.y) - dstSize.y) / 2.0F - mimimumCoordinate.y};
  for (auto& iStitch : *StitchBuffer) {
	iStitch.x += delta.x;
	iStitch.y += delta.y;
  }
}

auto DST::internal::dtrn(DSTREC* dpnt) -> uint32_t {
  return *(convert_ptr<uint32_t*>(dpnt));
}


void DST::ritdst(DSTOffsets&                    DSTOffsetData,
                             std::vector<DSTREC>&           DSTRecords,
                             std::vector<fPOINTATTR> const& stitches) {
  constexpr auto DSTMax          = 121;
  auto           dstStitchBuffer = std::vector<fPOINTATTR> {};
  dstStitchBuffer.resize(StitchBuffer->size());
  auto colorData = std::vector<uint32_t> {};
  // there could be as many colors as there are stitches
  // but we only need to reserve a reasonable number
  colorData.reserve(32);
  colorData.push_back(COLVER);
  colorData.push_back(BackgroundColor);
  colorData.push_back(UserColor[stitches[0].attribute & COLMSK]);
  auto destination = dstStitchBuffer.begin();
  for (auto const& stitch : stitches) {
	*destination++ = fPOINTATTR {stitch.x * 5.0F / 3.0F, stitch.y * 5.0F / 3.0F, stitch.attribute};
  }
  auto boundingRect = fRECTANGLE {
      dstStitchBuffer[0].x, dstStitchBuffer[0].y, dstStitchBuffer[0].x, dstStitchBuffer[0].y};
  for (auto& stitch : dstStitchBuffer) {
	if (stitch.x > boundingRect.right) {
	  boundingRect.right = stitch.x + 0.5F;
	}
	if (stitch.x < boundingRect.left) {
	  boundingRect.left = stitch.x - 0.5F;
	}
	if (stitch.y > boundingRect.top) {
	  boundingRect.top = stitch.y + 0.5F;
	}
	if (stitch.y < boundingRect.bottom) {
	  boundingRect.bottom = stitch.y - 0.5F;
	}
  }
  auto centerCoordinate = POINT {wrap::round<int32_t>(wrap::midl(boundingRect.right, boundingRect.left)),
                                 wrap::round<int32_t>(wrap::midl(boundingRect.top, boundingRect.bottom))};
  DSTOffsetData.Positive.x = wrap::round<int32_t>(boundingRect.right - centerCoordinate.x + 1);
  DSTOffsetData.Positive.y = wrap::round<int32_t>(boundingRect.top - centerCoordinate.y + 1);
  DSTOffsetData.Negative.x = wrap::round<int32_t>(centerCoordinate.x - boundingRect.left - 1);
  DSTOffsetData.Negative.y = wrap::round<int32_t>(centerCoordinate.y - boundingRect.bottom - 1);
  auto color               = dstStitchBuffer[0].attribute & 0xfU;
  for (auto& stitch : dstStitchBuffer) {
	if (color != (stitch.attribute & 0xfU)) {
	  di::savdst(DSTRecords, 0xc30000);
	  color = stitch.attribute & 0xfU;
	  colorData.push_back(UserColor[color]);
	}
	auto lengths = POINT {wrap::round<int32_t>(gsl::narrow_cast<double>(stitch.x) - centerCoordinate.x),
	                      wrap::round<int32_t>(gsl::narrow_cast<double>(stitch.y) - centerCoordinate.y)};
	auto const absoluteLengths = POINT {abs(lengths.x), abs(lengths.y)};
	auto       count           = 0U;
	if (absoluteLengths.x > absoluteLengths.y) {
	  count = absoluteLengths.x / DSTMax + 1;
	}
	else {
	  count = absoluteLengths.y / DSTMax + 1;
	}
	auto const stepSize = POINT {gsl::narrow<int32_t>(absoluteLengths.x / count + 1U),
	                             gsl::narrow<int32_t>(absoluteLengths.y / count + 1U)};

	auto difference = POINT {0L, 0L};
	while ((lengths.x != 0) || (lengths.y != 0)) {
	  auto dstType = REGTYP;
	  if (abs(lengths.x) > stepSize.x) {
		dstType = JMPTYP;
		if (lengths.x > 0) {
		  difference.x = stepSize.x;
		}
		else {
		  difference.x = -stepSize.x;
		}
	  }
	  else {
		difference.x = lengths.x;
	  }
	  if (abs(lengths.y) > stepSize.y) {
		dstType = JMPTYP;
		if (lengths.y > 0) {
		  difference.y = stepSize.y;
		}
		else {
		  difference.y = -stepSize.y;
		}
	  }
	  else {
		difference.y = lengths.y;
	  }
	  di::savdst(DSTRecords, di::dudbits(difference) | dstType);
	  centerCoordinate.x += difference.x;
	  centerCoordinate.y += difference.y;
	  lengths.x -= difference.x;
	  lengths.y -= difference.y;
	}
  }
  DSTRecords.push_back(
      {gsl::narrow_cast<uint8_t>(0), gsl::narrow_cast<uint8_t>(0), gsl::narrow_cast<uint8_t>(0xf3)});
  if (di::colfil()) {
	auto bytesWritten = DWORD {0};
	auto *colorFile =
	    CreateFile(ColorFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (colorFile != INVALID_HANDLE_VALUE) { // NOLINT
	  wrap::WriteFile(colorFile,
	                  &colorData[0],
	                  wrap::toUnsigned(colorData.size() * sizeof(decltype(colorData.data()))),
	                  &bytesWritten,
	                  nullptr);
	}
	CloseHandle(colorFile);
	colorFile = CreateFile(RGBFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (colorFile != INVALID_HANDLE_VALUE) { // NOLINT
	  wrap::WriteFile(colorFile,
	                  &colorData[2],
	                  wrap::toUnsigned((colorData.size() - 2U) * sizeof(decltype(colorData.data()))),
	                  &bytesWritten,
	                  nullptr);
	}
	CloseHandle(colorFile);
  }
}

auto DST::internal::colfil() -> bool {
  *ColorFileName = *WorkingFileName;
  *RGBFileName   = *WorkingFileName;
  auto flag      = false;
  if (WorkingFileName->has_extension()) {
	ColorFileName->replace_extension(L"thw");
	RGBFileName->replace_extension(L"rgb");
	flag = true;
  }
  return flag;
}

void DST::setColFilename(fs::path* directory) noexcept {
  ColorFileName = directory;
}

void DST::setRGBFilename(fs::path* directory) noexcept {
  RGBFileName = directory;
}

auto DST::internal::coldis(COLORREF colorA, COLORREF colorB) -> DWORD {
  auto       color1 = PECCOLOR {GetRValue(colorA), GetGValue(colorA), GetBValue(colorA)}; // NOLINT
  auto       color2 = PECCOLOR {GetRValue(colorB), GetGValue(colorB), GetBValue(colorB)}; // NOLINT
  auto const meanR = (gsl::narrow_cast<int32_t>(color1.r) + gsl::narrow_cast<int32_t>(color2.r)) / 2;
  auto       deltaR = gsl::narrow_cast<int32_t>(color1.r) - gsl::narrow_cast<int32_t>(color2.r);
  auto       deltaG = gsl::narrow_cast<int32_t>(color1.g) - gsl::narrow_cast<int32_t>(color2.g);
  auto       deltaB = gsl::narrow_cast<int32_t>(color1.b) - gsl::narrow_cast<int32_t>(color2.b);
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula
  return wrap::round<DWORD>(std::sqrt((((512 + meanR) * deltaR * deltaR) / 256) + 4 * deltaG * deltaG +
                                      (((767 - meanR) * deltaB * deltaB) / 256)));
}

auto DST::colmatch(COLORREF color) -> uint32_t {
  if (ColorChanges < 16U) {
	for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
	  if (color == UserColor[iColor]) {
		return iColor;
	  }
	}
	UserColor[ColorChanges++] = color;
	return ColorChanges - 1U;
  }
  auto minDistance = DWORD {0xffffffffU};
  auto iDistance   = DWORD {0x00000000U};
  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
	auto const distance = di::coldis(color, UserColor[iColor]);
	if (distance == 0U) {
	  return iColor;
	}
	iDistance = 0U;
	if (distance < minDistance) {
	  iDistance   = iColor;
	  minDistance = distance;
	}
  }
  return iDistance;
}

constexpr auto DST::internal::dudbits(POINT const& dif) -> uint32_t {
  return Xdst[dif.x + 121] | Ydst[dif.y + 121];
}

void DST::internal::savdst(std::vector<DSTREC>& DSTRecords, uint32_t data) {
  DSTRecords.push_back(DSTREC {gsl::narrow_cast<uint8_t>(data & 0xFFU),
                               gsl::narrow_cast<uint8_t>((data & 0xFF00U) >> 8U),
                               gsl::narrow_cast<uint8_t>((data & 0xFF0000U) >> 16U)});
}

auto DST::internal::chkdst(DSTHED const* dstHeader) noexcept -> bool {
  return strncmp(dstHeader->desched, "LA:", 3) == 0; // NOLINT
}

auto DST::readDSTFile(HANDLE fileHandle, DWORD& fileSize) -> bool {
  auto dstHeader = DSTHED {};
  auto bytesRead = DWORD {};
  ReadFile(fileHandle, &dstHeader, sizeof(dstHeader), &bytesRead, nullptr);
  if (bytesRead == sizeof(dstHeader)) {
	if (di::chkdst(&dstHeader)) {
	  PCSBMPFileName[0] = 0;
	  fileSize = GetFileSize(fileHandle, &bytesRead) - sizeof(dstHeader);
	  auto DSTData = std::vector<DSTREC> {};
	  DSTData.resize(fileSize / sizeof(DSTREC));
	  wrap::ReadFile(fileHandle, DSTData.data(), fileSize, &bytesRead, nullptr);
	  di::dstran(DSTData);
	  IniFile.auxFileType = AUXDST;
	}
  }
  else {
	displayText::tabmsg(IDS_DST2S);
	CloseHandle(fileHandle);
	return true;
  }
  return false;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void DST::saveDST(std::vector<fPOINTATTR> const& saveStitches, const char* desc) {
  auto *PCSFileHandle = CreateFile(
      AuxName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT
  if (PCSFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	displayText::crmsg(*AuxName);
	PCSFileHandle = nullptr;
  }
  else {
	auto DSTRecords = std::vector<DSTREC> {};
	// There are always going to be more records in the DST format because color changes and jumps count as stitches
	DSTRecords.reserve(StitchBuffer->size() + 128U);
	auto DSTOffset = DSTOffsets {};
	auto dstHeader = DSTHED {};
	DST::ritdst(DSTOffset, DSTRecords, saveStitches);
	// dstHeader fields are fixed width, so use strncpy in its intended way.
	// Use sizeof to ensure no overrun if the format string is wrong length
	strncpy(dstHeader.desched, "LA:", sizeof(dstHeader.desched)); // NOLINT
	std::fill(std::begin(dstHeader.desc), std::end(dstHeader.desc), ' ');
	if (desc != nullptr) {
	  for (auto iHeader = 0U; iHeader < wrap::toUnsigned(sizeof(dstHeader.desc)); iHeader++) {
		if ((desc[iHeader] != 0) && desc[iHeader] != '.') {
		  dstHeader.desc[iHeader] = desc[iHeader];
		}
		else {
		  break;
		}
	  }
	}
	// clang-format off
    dstHeader.desc[16] = 0xd;
    strncpy(dstHeader.recshed,    "ST:", sizeof(dstHeader.recshed));                                          // NOLINT
    strncpy(dstHeader.recs, fmt::format("{:7d}\r", DSTRecords.size()).c_str(), sizeof(dstHeader.recs));       // NOLINT
    strncpy(dstHeader.cohed,      "CO:", sizeof(dstHeader.cohed));                                            // NOLINT
    strncpy(dstHeader.co,         "  0\xd", sizeof(dstHeader.co));                                            // NOLINT
    strncpy(dstHeader.xplushed,   "+X:", sizeof(dstHeader.xplushed));                                         // NOLINT
    strncpy(dstHeader.xplus, fmt::format("{:5d}\xd", DSTOffset.Negative.x).c_str(), sizeof(dstHeader.xplus)); // NOLINT
    strncpy(dstHeader.xminhed,    "-X:", sizeof(dstHeader.xminhed));                                          // NOLINT
    strncpy(dstHeader.xmin, fmt::format( "{:5d}\xd", DSTOffset.Positive.x).c_str(), sizeof(dstHeader.xmin));  // NOLINT
    strncpy(dstHeader.yplushed,   "+Y:", sizeof(dstHeader.yplushed));                                         // NOLINT
    strncpy(dstHeader.yplus, fmt::format("{:5d}\xd", DSTOffset.Positive.y).c_str(), sizeof(dstHeader.yplus)); // NOLINT
    strncpy(dstHeader.yminhed,    "-Y:", sizeof(dstHeader.yminhed));                                          // NOLINT
    strncpy(dstHeader.ymin, fmt::format( "{:5d}\xd", DSTOffset.Negative.y).c_str(), sizeof(dstHeader.ymin));  // NOLINT
    strncpy(dstHeader.axhed,      "AX:", sizeof(dstHeader.axhed));                                            // NOLINT
    strncpy(dstHeader.ax,         "-    0\r", sizeof(dstHeader.ax));                                          // NOLINT
    strncpy(dstHeader.ayhed,      "AY:", sizeof(dstHeader.ayhed));                                            // NOLINT
    strncpy(dstHeader.ay,         "+    0\r", sizeof(dstHeader.ay));                                          // NOLINT
    strncpy(dstHeader.mxhed,      "MX:", sizeof(dstHeader.mxhed));                                            // NOLINT
    strncpy(dstHeader.mx,         "+    0\r", sizeof(dstHeader.mx));                                          // NOLINT
    strncpy(dstHeader.myhed,      "MY:", sizeof(dstHeader.myhed));                                            // NOLINT
    strncpy(dstHeader.my,         "+    0\r", sizeof(dstHeader.my));                                          // NOLINT
    strncpy(dstHeader.pdhed,      "PD", sizeof(dstHeader.pdhed));                                             // NOLINT
    strncpy(dstHeader.pd,         "******\r", sizeof(dstHeader.pd));                                          // NOLINT
    strncpy(dstHeader.eof,        "\x1a", sizeof(dstHeader.eof));                                             // NOLINT
	                                                       // clang-format on
	std::fill(std::begin(dstHeader.res), std::end(dstHeader.res), ' ');
	auto bytesWritten = DWORD {0};
	WriteFile(PCSFileHandle, &dstHeader, sizeof(dstHeader), &bytesWritten, nullptr);
	wrap::WriteFile(PCSFileHandle,
	                DSTRecords.data(),
	                wrap::toUnsigned(sizeof(decltype(DSTRecords.back())) * DSTRecords.size()),
	                &bytesWritten,
	                nullptr);
  }
}
#pragma warning(pop)
