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

// Open Source headers

// Local Headers
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "bitmap.h"
#include "displayText.h"
#include "thred.h"
#include "DST.h"

namespace di = DST::internal;

static auto ColorFileName = static_cast<fs::path*>(nullptr); //.thw file name
static auto RGBFileName   = static_cast<fs::path*>(nullptr); //.rgb file name

class DSTDAT
{
  public:
  char cor {0};
  char val {0};

  constexpr DSTDAT() noexcept = default;
  constexpr DSTDAT(char rhsCor, char rhsVal);
  // DSTDAT(DSTDAT&&) = default;
  // DSTDAT& operator=(DSTDAT const& rhs) = default;
  // DSTDAT& operator=(DSTDAT&&) = default;
  //~DSTDAT() = default;
};

constexpr DSTDAT::DSTDAT(char rhsCor, char rhsVal) : cor(rhsCor), val(rhsVal) {
}

constexpr auto XCOR = char {0};
constexpr auto YCOR = char {1};

constexpr auto DSTMAX    = 121;         // maximum stitch/jump length of 121 in DST format
constexpr auto DSTSCALE  = 3.0F / 5.0F; // DST stitch scaling factor
constexpr auto IDSTSCALE = 5.0F / 3.0F; // Inverse DST stitch scaling factor

void DST::internal::dstin(uint32_t number, POINT& pout) noexcept {
  // ToDo - what is this code doing?
  static constexpr auto DSTvalues = std::array<DSTDAT, 22> {
      // DST offset values
      DSTDAT {XCOR, 1},   DSTDAT {XCOR, -1}, DSTDAT {XCOR, 9},  DSTDAT {XCOR, -9},
      DSTDAT {YCOR, -9},  DSTDAT {YCOR, 9},  DSTDAT {YCOR, -1}, DSTDAT {YCOR, 1},
      DSTDAT {XCOR, 3},   DSTDAT {XCOR, -3}, DSTDAT {XCOR, 27}, DSTDAT {XCOR, -27},
      DSTDAT {YCOR, -27}, DSTDAT {YCOR, 27}, DSTDAT {YCOR, -3}, DSTDAT {YCOR, 3},
      DSTDAT {XCOR, 0},   DSTDAT {XCOR, 0},  DSTDAT {XCOR, 81}, DSTDAT {XCOR, -81},
      DSTDAT {YCOR, -81}, DSTDAT {YCOR, 81}};

  auto shift = 1U;
  pout       = POINT {};
  for (auto const& DSTValue : DSTvalues) {
	if ((number & shift) != 0U) {
	  if (DSTValue.cor != XCOR) {
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
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto colorFile =
	    CreateFile(ColorFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (colorFile != INVALID_HANDLE_VALUE) {
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
		  UserColor.fill(0);
		}
	  }
	}
  }
  auto iColor = 2U;
  auto color  = 0U;
  if (bytesRead >= ((wrap::toSize(iColor) + 1U) * sizeof(decltype(colors.back())))) {
	UserColor[0] = colors[iColor];
	color        = colmatch(colors[iColor++]);
	thred::addColor(0, color);
  }
  else {
	color = 0;
  }
  auto localStitch       = fPOINT {};
  auto maximumCoordinate = fPOINT {-BIGFLOAT, -BIGFLOAT};
  auto mimimumCoordinate = fPOINT {BIGFLOAT, BIGFLOAT};
  StitchBuffer->clear();
  StitchBuffer->reserve(DSTData.size()); // we will be reserving a little more than we need
  constexpr auto C1MASK = 0x40U;
  constexpr auto C0MASK = 0x80U;
  for (auto& record : DSTData) {
	if ((record.nd & C1MASK) != 0) { // if c1 is set, assume a color change and not a sequin, which would have c0 set too
	  if (bytesRead >= ((wrap::toSize(iColor) + 1U) * sizeof(decltype(colors.back())))) {
		color                    = colmatch(colors[iColor++]);
		auto const currentStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
		thred::addColor(currentStitch, color);
	  }
	  else {
		++color;
		color &= COLORBTS;
	  }
	}
	else {
	  auto dstStitch = POINT {0L, 0L};
	  di::dstin(di::dtrn(&record), dstStitch);
	  localStitch.x += wrap::toFloat(dstStitch.x);
	  localStitch.y += wrap::toFloat(dstStitch.y);
	  if ((record.nd & C0MASK) == 0U) { // if c0 is not set, we assume a normal stitch and not a sequin, which would have c1 set
		StitchBuffer->push_back(fPOINTATTR {localStitch.x * DSTSCALE, localStitch.y * DSTSCALE, color | NOTFRM});
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
  UnzoomedRect     = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
  if (dstSize.x > wrap::toFloat(UnzoomedRect.x) || dstSize.y > wrap::toFloat(UnzoomedRect.y)) {
	constexpr auto EXPRATIO = 1.1F; // 10% expansion ratio
	IniFile.hoopSizeX       = dstSize.x * EXPRATIO;
	IniFile.hoopSizeY       = dstSize.y * EXPRATIO;
	UnzoomedRect            = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
	displayText::hsizmsg();
  }
  auto const delta = fPOINT {(wrap::toFloat(UnzoomedRect.x) - dstSize.x) / 2.0F - mimimumCoordinate.x,
                             (wrap::toFloat(UnzoomedRect.y) - dstSize.y) / 2.0F - mimimumCoordinate.y};
  for (auto& iStitch : *StitchBuffer) {
	iStitch.x += delta.x;
	iStitch.y += delta.y;
  }
}

auto DST::internal::dtrn(DSTREC* dpnt) -> uint32_t {
  return *(convert_ptr<uint32_t*>(dpnt));
}

void DST::ritdst(DSTOffsets& DSTOffsetData, std::vector<DSTREC>& DSTRecords, std::vector<fPOINTATTR> const& stitches) {
  auto dstStitchBuffer = std::vector<fPOINTATTR> {};
  dstStitchBuffer.resize(StitchBuffer->size());
  auto colorData = std::vector<uint32_t> {};
  // there could be as many colors as there are stitches
  // but we only need to reserve a reasonable number
  // NOLINTNEXTLINE(readability-magic-numbers)
  colorData.reserve(32);
  colorData.push_back(COLVER);
  colorData.push_back(BackgroundColor);
  colorData.push_back(UserColor[stitches[0].attribute & COLMSK]);
  auto destination = dstStitchBuffer.begin();
  for (auto const& stitch : stitches) {
	*destination++ = fPOINTATTR {stitch.x * IDSTSCALE, stitch.y * IDSTSCALE, stitch.attribute};
  }
  auto boundingRect = fRECTANGLE {
      dstStitchBuffer[0].x, dstStitchBuffer[0].y, dstStitchBuffer[0].x, dstStitchBuffer[0].y};
  constexpr auto MARGIN = 0.5F; // margin added on all sides to ensure bounding rectangle area is not zero
  for (auto& stitch : dstStitchBuffer) {
	if (stitch.x > boundingRect.right) {
	  boundingRect.right = stitch.x + MARGIN;
	}
	if (stitch.x < boundingRect.left) {
	  boundingRect.left = stitch.x - MARGIN;
	}
	if (stitch.y > boundingRect.top) {
	  boundingRect.top = stitch.y + MARGIN;
	}
	if (stitch.y < boundingRect.bottom) {
	  boundingRect.bottom = stitch.y - MARGIN;
	}
  }
  auto centerCoordinate    = POINT {std::lround(wrap::midl(boundingRect.right, boundingRect.left)),
                                 std::lround(wrap::midl(boundingRect.top, boundingRect.bottom))};
  DSTOffsetData.Positive.x = std::lround(boundingRect.right - wrap::toFloat(centerCoordinate.x + 1));
  DSTOffsetData.Positive.y = std::lround(boundingRect.top - wrap::toFloat(centerCoordinate.y + 1));
  DSTOffsetData.Negative.x = std::lround(wrap::toFloat(centerCoordinate.x - 1) - boundingRect.left);
  DSTOffsetData.Negative.y = std::lround(wrap::toFloat(centerCoordinate.y - 1) - boundingRect.bottom);
  auto color               = dstStitchBuffer[0].attribute & COLORBTS;
  for (auto& stitch : dstStitchBuffer) {
	if (color != (stitch.attribute & COLORBTS)) {
	  constexpr auto STOPCODE = uint8_t {0xC3}; // note that stop code is the same as the color change code
	  DSTRecords.push_back(DSTREC {0, 0, STOPCODE});
	  color = stitch.attribute & COLORBTS;
	  colorData.push_back(UserColor[color]);
	}
	auto       lengths         = POINT {std::lround(stitch.x - wrap::toFloat(centerCoordinate.x)),
                          std::lround(stitch.y - wrap::toFloat(centerCoordinate.y))};
	auto const absoluteLengths = POINT {abs(lengths.x), abs(lengths.y)};
	auto       count           = 0;
	if (absoluteLengths.x > absoluteLengths.y) {
	  count = absoluteLengths.x / DSTMAX + 1;
	}
	else {
	  count = absoluteLengths.y / DSTMAX + 1;
	}
	auto const stepSize = POINT {(absoluteLengths.x / count + 1), (absoluteLengths.y / count + 1)};

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
  constexpr auto ENDCODE = uint8_t {0xF3};
  DSTRecords.push_back(DSTREC {0, 0, ENDCODE});
  if (di::colfil()) {
	auto bytesWritten = DWORD {0};
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto colorFile =
	    CreateFile(ColorFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (colorFile != INVALID_HANDLE_VALUE) {
	  wrap::WriteFile(colorFile,
	                  &colorData[0],
	                  wrap::toUnsigned(colorData.size() * sizeof(decltype(colorData.data()))),
	                  &bytesWritten,
	                  nullptr);
	}
	CloseHandle(colorFile);
	colorFile = CreateFile(RGBFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (colorFile != INVALID_HANDLE_VALUE) {
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  auto color1 = PECCOLOR {GetRValue(colorA), GetGValue(colorA), GetBValue(colorA)};
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  auto       color2 = PECCOLOR {GetRValue(colorB), GetGValue(colorB), GetBValue(colorB)};
  auto const meanR = (gsl::narrow_cast<int32_t>(color1.r) + gsl::narrow_cast<int32_t>(color2.r)) / 2;
  auto       deltaR = gsl::narrow_cast<int32_t>(color1.r) - gsl::narrow_cast<int32_t>(color2.r);
  auto       deltaG = gsl::narrow_cast<int32_t>(color1.g) - gsl::narrow_cast<int32_t>(color2.g);
  auto       deltaB = gsl::narrow_cast<int32_t>(color1.b) - gsl::narrow_cast<int32_t>(color2.b);
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula
  // NOLINTNEXTLINE(readability-magic-numbers)
  return wrap::round<DWORD>(std::sqrtf(wrap::toFloat((((512 + meanR) * deltaR * deltaR) / 256) + 4 * deltaG * deltaG +
                                                     (((767 - meanR) * deltaB * deltaB) / 256)))); // NOLINT(readability-magic-numbers)
}

auto DST::colmatch(COLORREF color) -> uint32_t {
  auto const colorChanges = thred::maxColor() + 1U;
  if (colorChanges < COLORCNT) {
	for (auto iColor = size_t {}; iColor < colorChanges; ++iColor) {
	  if (color == UserColor[iColor]) {
		return wrap::toUnsigned(iColor);
	  }
	}
	UserColor[colorChanges] = color;
	return wrap::toUnsigned(colorChanges);
  }
  auto minDistance = std::numeric_limits<DWORD>::max();
  auto iDistance   = uint32_t {};
  for (auto iColor = size_t {}; iColor < COLORCNT; ++iColor) {
	auto const distance = di::coldis(color, UserColor[iColor]);
	if (distance == 0U) {
	  return wrap::toUnsigned(iColor);
	}
	iDistance = 0U;
	if (distance < minDistance) {
	  iDistance   = wrap::toUnsigned(iColor);
	  minDistance = distance;
	}
  }
  return iDistance;
}

auto DST::internal::dudbits(POINT const& dif) -> uint32_t {
  static constexpr auto DSTLEN = 243U; // -121 to 121
  static constexpr auto xDST   = std::array<uint32_t, DSTLEN> {
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

  static constexpr auto yDST = std::array<uint32_t, DSTLEN> {
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
  return xDST[wrap::toSize(dif.x) + DSTMAX] | yDST[wrap::toSize(dif.y) + DSTMAX];
}

void DST::internal::savdst(std::vector<DSTREC>& DSTRecords, uint32_t data) {
  DSTRecords.push_back(DSTREC {gsl::narrow_cast<uint8_t>(data & B1MASK),
                               gsl::narrow_cast<uint8_t>((data & B2MASK) >> BYTSHFT),
                               gsl::narrow_cast<uint8_t>((data & B3MASK) >> WRDSHFT)});
}

auto DST::internal::chkdst(DSTHED const* dstHeader) noexcept -> bool {
  return strncmp(static_cast<const char*>(dstHeader->desched), "LA:", 3) == 0;
}

auto DST::readDSTFile(std::filesystem::path const& newFileName) -> bool {
  auto fileSize = uintmax_t {0};
  if (!thred::getFileSize(newFileName, fileSize)) {
	return false;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	return false;
  }
  auto dstHeader = DSTHED {};
  auto bytesRead = DWORD {};
  ReadFile(fileHandle, &dstHeader, sizeof(dstHeader), &bytesRead, nullptr);
  if (bytesRead == sizeof(dstHeader)) {
	if (di::chkdst(&dstHeader)) {
	  bitmap::resetBmpFile(true);
	  fileSize -= sizeof(dstHeader);
	  auto DSTData = std::vector<DSTREC> {};
	  DSTData.resize(wrap::toSize(fileSize / sizeof(DSTREC)));
	  ReadFile(fileHandle, DSTData.data(), gsl::narrow<DWORD>(fileSize), &bytesRead, nullptr);
	  di::dstran(DSTData);
	  IniFile.auxFileType = AUXDST;
	}
  }
  else {
	displayText::tabmsg(IDS_DST2S);
	CloseHandle(fileHandle);
	return false;
  }
  CloseHandle(fileHandle);
  return true;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
auto DST::saveDST(fs::path const* auxName, std::vector<fPOINTATTR> const& saveStitches) -> bool {
  auto flag = true;
  if (nullptr != auxName) {
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto fileHandle = CreateFile(
	    auxName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT(hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (fileHandle == INVALID_HANDLE_VALUE) {
	  displayText::crmsg(*auxName);
	  flag = false;
	}
	else {
	  do {
		auto        convAuxName = utf::Utf16ToUtf8(*auxName);
		auto const* desc        = strrchr(convAuxName.data(), '\\') + 1U;
		auto        DSTRecords  = std::vector<DSTREC> {};
		// There are always going to be more records in the DST format because color changes and jumps count as stitches so reserve a little extra
		// NOLINTNEXTLINE(readability-magic-numbers)
		DSTRecords.reserve(StitchBuffer->size() + 128U);
		auto DSTOffset = DSTOffsets {};
		auto dstHeader = DSTHED {};
		DST::ritdst(DSTOffset, DSTRecords, saveStitches);
		// dstHeader fields are fixed width, so use strncpy in its intended way.
		// Use sizeof to ensure no overrun if the format string is wrong length
		strncpy(static_cast<char*>(dstHeader.desched), "LA:", sizeof(dstHeader.desched)); // NOLINT(clang-diagnostic-deprecated-declarations)
		auto& dstDesc = dstHeader.desc;
		std::fill(std::begin(dstDesc), std::end(dstDesc), ' ');
		if (desc != nullptr) {
		  for (auto iHeader = 0U; iHeader < wrap::toUnsigned(sizeof(dstHeader.desc)); ++iHeader) {
			if ((desc[iHeader] != 0) && desc[iHeader] != '.') {
			  dstHeader.desc[iHeader] = desc[iHeader];
			}
			else {
			  break;
			}
		  }
		}
		// clang-format off
        // NOLINTNEXTLINE(readability-magic-numbers)
        dstHeader.desc[16] = 0xd;
        strncpy(static_cast<char *>(dstHeader.recshed),    "ST:", sizeof(dstHeader.recshed));                                           // NOLINT(clang-diagnostic-deprecated-declarations)                                        
        strncpy(static_cast<char *>(dstHeader.recs),  fmt::format("{:7d}\r", DSTRecords.size()).c_str(), sizeof(dstHeader.recs));       // NOLINT(clang-diagnostic-deprecated-declarations)       
        strncpy(static_cast<char *>(dstHeader.cohed),      "CO:", sizeof(dstHeader.cohed));                                             // NOLINT(clang-diagnostic-deprecated-declarations)                                            
        strncpy(static_cast<char *>(dstHeader.co),         "  0\xd", sizeof(dstHeader.co));                                             // NOLINT(clang-diagnostic-deprecated-declarations)                                            
        strncpy(static_cast<char *>(dstHeader.xplushed),   "+X:", sizeof(dstHeader.xplushed));                                          // NOLINT(clang-diagnostic-deprecated-declarations)                                        
        strncpy(static_cast<char *>(dstHeader.xplus), fmt::format("{:5d}\xd", DSTOffset.Negative.x).c_str(), sizeof(dstHeader.xplus));  // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.xminhed),    "-X:", sizeof(dstHeader.xminhed));                                           // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.xmin),  fmt::format( "{:5d}\xd", DSTOffset.Positive.x).c_str(), sizeof(dstHeader.xmin));  // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.yplushed),   "+Y:", sizeof(dstHeader.yplushed));                                          // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.yplus), fmt::format("{:5d}\xd", DSTOffset.Positive.y).c_str(), sizeof(dstHeader.yplus));  // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.yminhed),    "-Y:", sizeof(dstHeader.yminhed));                                           // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.ymin),  fmt::format( "{:5d}\xd", DSTOffset.Negative.y).c_str(), sizeof(dstHeader.ymin));  // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.axhed),      "AX:", sizeof(dstHeader.axhed));                                             // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.ax),         "-    0\r", sizeof(dstHeader.ax));                                           // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.ayhed),      "AY:", sizeof(dstHeader.ayhed));                                             // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.ay),         "+    0\r", sizeof(dstHeader.ay));                                           // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.mxhed),      "MX:", sizeof(dstHeader.mxhed));                                             // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.mx),         "+    0\r", sizeof(dstHeader.mx));                                           // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.myhed),      "MY:", sizeof(dstHeader.myhed));                                             // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.my),         "+    0\r", sizeof(dstHeader.my));                                           // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.pdhed),      "PD", sizeof(dstHeader.pdhed));                                              // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.pd),         "******\r", sizeof(dstHeader.pd));                                           // NOLINT(clang-diagnostic-deprecated-declarations)
        strncpy(static_cast<char *>(dstHeader.eof),        "\x1a", sizeof(dstHeader.eof));                                              // NOLINT(clang-diagnostic-deprecated-declarations)
		// clang-format on
		auto& res = dstHeader.res;
		std::fill(std::begin(res), std::end(res), ' ');
		auto bytesWritten = DWORD {0};
		if (FALSE == WriteFile(fileHandle, &dstHeader, sizeof(dstHeader), &bytesWritten, nullptr)) {
		  displayText::riter();
		  flag = false;
		  break;
		}
		if (FALSE == WriteFile(fileHandle,
		                       DSTRecords.data(),
		                       wrap::toUnsigned(sizeof(decltype(DSTRecords.back())) * DSTRecords.size()),
		                       &bytesWritten,
		                       nullptr)) {
		  displayText::riter();
		  flag = false;
		  break;
		}
	  } while (false);
	}
	CloseHandle(fileHandle);
  }
  else {
	flag = false;
  }
  return flag;
}
#pragma warning(pop)
