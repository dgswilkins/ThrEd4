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
#include "switches.h"
#if PESACT

#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "thred.h"
#include "PES.h"

namespace pi = PES::internal;

constexpr auto PECScale    = 5.0F / 3.0F;
constexpr auto invPECScale = 3.0F / 5.0F;
constexpr auto bitsPerByte = 8U;

auto static PEScolors             = gsl::narrow_cast<uint8_t*>(nullptr); // pes colors
auto static PESequivColors        = std::array<uint8_t, COLOR_COUNT> {}; // pes equivalent colors

THREAD const PESThread[] = {
    {{0x00, 0x00, 0x00}, "Unknown", ""},                // Index  00
    {{0x1a, 0x0a, 0x94}, "Prussian Blue", "ETP007"},    // Index 01
    {{0x0f, 0x75, 0xff}, "Blue", "ETP405"},             // Index 02
    {{0x00, 0x93, 0x4c}, "Teal Green", "ETP534"},       // Index 03
    {{0xba, 0xbd, 0xfe}, "Corn Flower Blue", "ETP070"}, // Index 04
    {{0xec, 0x00, 0x00}, "Red", "ETP800"},              // Index 05
    {{0xe4, 0x99, 0x5a}, "Reddish Brown", "ETP337"},    // Index 06
    {{0xcc, 0x48, 0xab}, "Magenta", "ETP620"},          // Index 07
    {{0xfd, 0xc4, 0xfa}, "Light Lilac", "ETP810"},      // Index 08
    {{0xdd, 0x84, 0xcd}, "Lilac", "ETP612"},            // Index 09
    {{0x6b, 0xd3, 0x8a}, "Mint Green", "ETP502"},       // Index 10
    {{0xe4, 0xa9, 0x45}, "Deep Gold", "ETP214"},        // Index 11
    {{0xff, 0xbd, 0x42}, "Orange", "ETP208"},           // Index 12
    {{0xff, 0xe6, 0x00}, "Yellow", "ETP205"},           // Index 13
    {{0x6c, 0xd9, 0x00}, "Lime Green", "ETP513"},       // Index 14
    {{0xc1, 0xa9, 0x41}, "Brass", "ETP328"},            // Index 15
    {{0xb5, 0xad, 0x97}, "Silver", "ETP005"},           // Index 16
    {{0xba, 0x9c, 0x5f}, "Russet Brown", "ETP330"},     // Index 17
    {{0xfa, 0xf5, 0x9e}, "Cream Brown", "ETP010"},      // Index 18
    {{0x80, 0x80, 0x80}, "Pewter", "ETP704"},           // Index 19
    {{0x00, 0x00, 0x00}, "Black", "ETP900"},            // Index 20
    {{0x00, 0x1c, 0xdf}, "Ultramarine", "ETP406"},      // Index 21
    {{0xdf, 0x00, 0xb8}, "Royal Purple", "ETP869"},     // Index 22
    {{0x62, 0x62, 0x62}, "Dark Gray", "ETP707"},        // Index 23
    {{0x69, 0x26, 0x0d}, "Dark Brown", "ETP058"},       // Index 24
    {{0xff, 0x00, 0x60}, "Deep Rose", "ETP086"},        // Index 25
    {{0xbf, 0x82, 0x00}, "Light Brown", "ETP323"},      // Index 26
    {{0xf3, 0x91, 0x78}, "Salmon Pink", "ETP079"},      // Index 27
    {{0xff, 0x68, 0x05}, "Vermilion", "ETP030"},        // Index 28
    {{0xf0, 0xf0, 0xf0}, "White", "ETP001"},            // Index 29
    {{0xc8, 0x32, 0xcd}, "Violet", "ETP613"},           // Index 30
    {{0xb0, 0xbf, 0x9b}, "Seacrest", "ETP542"},         // Index 31
    {{0x65, 0xbf, 0xeb}, "Sky Blue", "ETP019"},         // Index 32
    {{0xff, 0xba, 0x04}, "Pumpkin", "ETP126"},          // Index 33
    {{0xff, 0xf0, 0x6c}, "Cream Yellow", "ETP010"},     // Index 34
    {{0xfe, 0xca, 0x15}, "Khaki", "ETP348"},            // Index 35
    {{0xf3, 0x81, 0x01}, "Clay Brown", "ETP339"},       // Index 36
    {{0x37, 0xa9, 0x23}, "Leaf Green", "ETP509"},       // Index 37
    {{0x23, 0x46, 0x5f}, "Peacock Blue", "ETP405"},     // Index 38
    {{0xa6, 0xa6, 0x95}, "Gray", "ETP707"},             // Index 39
    {{0xce, 0xbf, 0xa6}, "Warm Gray", "ETP399"},        // Index 40
    {{0x96, 0xaa, 0x02}, "Dark Olive", "ETP517"},       // Index 41
    {{0xff, 0xe3, 0xc6}, "Linen", "ETP307"},            // Index 42
    {{0xff, 0x99, 0xd7}, "Pink", "ETP085"},             // Index 43
    {{0x00, 0x70, 0x04}, "Deep Green", "ETP808"},       // Index 44
    {{0xed, 0xcc, 0xfb}, "Lavender", "ETP804"},         // Index 45
    {{0xc0, 0x89, 0xd8}, "Wisteria Violet", "ETP607"},  // Index 46
    {{0xe7, 0xd9, 0xb4}, "Beige", "ETP843"},            // Index 47
    {{0xe9, 0x0e, 0x86}, "Carmine", "ETP807"},          // Index 48
    {{0xcf, 0x68, 0x29}, "Amber Red", "ETP333"},        // Index 49
    {{0x40, 0x86, 0x15}, "Olive Green", "ETP519"},      // Index 50
    {{0xdb, 0x17, 0x97}, "Dark Fuchsia", "ETP107"},     // Index 51
    {{0xff, 0xa7, 0x04}, "Tangerine", "ETP209"},        // Index 52
    {{0xb9, 0xff, 0xff}, "Light Blue", "ETP017"},       // Index 53
    {{0x22, 0x89, 0x27}, "Emerald Green", "ETP507"},    // Index 54
    {{0xb6, 0x12, 0xcd}, "Purple", "ETP614"},           // Index 55
    {{0x00, 0xaa, 0x00}, "Moss Green", "ETP515"},       // Index 56
    {{0xfe, 0xa9, 0xdc}, "Flesh Pink", "ETP124"},       // Index 57
    {{0xfe, 0xd5, 0x10}, "Harvest Gold", "ETP206"},     // Index 58
    {{0x00, 0x97, 0xdf}, "Electric Blue", "ETP420"},    // Index 59
    {{0xff, 0xff, 0x84}, "Lemon Yellow", "ETP205"},     // Index 60
    {{0xcf, 0xe7, 0x74}, "Fresh Green", "ETP027"},      // Index 61
    {{0xff, 0xc8, 0x64}, "Applique Material", ""},      // Index 62
    {{0xff, 0xc8, 0xc8}, "Applique Position", ""},      // Index 63
    {{0xff, 0xc8, 0xc8}, "Applique", ""}                // Index 64
};

// clang-format off
char const imageWithFrame[ThumbHeight][ThumbWidth]= { 
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
};
// clang-format on

auto PES::internal::pesmtch(COLORREF const& referenceColor, uint8_t const& colorIndex) -> uint32_t {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  auto color = PECCOLOR {GetRValue(referenceColor), GetGValue(referenceColor), GetBValue(referenceColor)};
  auto translatedColor = PESThread[colorIndex].color;
  auto const meanR = (gsl::narrow_cast<int32_t>(color.r) + gsl::narrow_cast<int32_t>(translatedColor.r)) / 2;
  auto deltaR = gsl::narrow_cast<int32_t>(color.r) - gsl::narrow_cast<int32_t>(translatedColor.r);
  auto deltaG = gsl::narrow_cast<int32_t>(color.g) - gsl::narrow_cast<int32_t>(translatedColor.g);
  auto deltaB = gsl::narrow_cast<int32_t>(color.b) - gsl::narrow_cast<int32_t>(translatedColor.b);
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula
  return wrap::round<uint32_t>(std::sqrt((((512 + meanR) * deltaR * deltaR) / 256) + 4 * deltaG * deltaG +
                                         (((767 - meanR) * deltaB * deltaB) / 256)));
}

void PES::internal::ritpes(std::vector<uint8_t>& buffer, fPOINTATTR const& stitch, fPOINT const& offset) {
  constexpr auto factor  = 3.0F / 5.0F;
  auto const     oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESTCH));
  auto*      pesStitch    = convert_ptr<PESTCH*>(&buffer[oldSize]);
  auto const scaledStitch = fPOINT {-stitch.x * factor + offset.x,
                                    stitch.y * factor - offset.y};
  pesStitch->x            = wrap::round<int16_t>(scaledStitch.x);
  pesStitch->y            = wrap::round<int16_t>(scaledStitch.y);
}

void PES::internal::ritpesCode(std::vector<uint8_t>& buffer) {
  constexpr auto blockEndCode = uint16_t {0x8003U};
  auto const     oldSize      = buffer.size();
  buffer.resize(oldSize + sizeof(uint16_t));
  auto* contCode = convert_ptr<uint16_t*>(&buffer[oldSize]);
  if (contCode != nullptr) {
	*contCode = blockEndCode;
  }
}

void PES::internal::ritpesBlock(std::vector<uint8_t>& buffer, PESSTCHLST newBlock) {
  auto const oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESSTCHLST));
  auto* blockHeader = convert_ptr<PESSTCHLST*>(&buffer[oldSize]);
  *blockHeader      = newBlock;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void PES::internal::pecnam(gsl::span<char> label) {
  // NOLINTNEXTLINE(clang-diagnostic-deprecated-declarations)
  strncpy(label.data(), "LA:", 3);
  auto const lblSize  = wrap::toUnsigned(label.size() - 3U);
  auto       fileStem = utf::Utf16ToUtf8(AuxName->stem());
  if (fileStem.size() < lblSize) {
	fileStem += std::string(lblSize - fileStem.size(), ' ');
  }
  // NOLINTNEXTLINE(clang-diagnostic-deprecated-declarations)
  strncpy(&label[3], fileStem.c_str(), lblSize);
}
#pragma warning(pop)

void PES::internal::pecEncodeint32_t(std::vector<uint8_t>& buffer, int32_t delta) {
  constexpr auto mask11bits = uint32_t {0x7FFU};  // used to mask the value to 11 bits
  constexpr auto bit8       = uint32_t {0x80U};   // Set bit 8 on the upper byte
  constexpr auto bit12      = uint32_t {0x800U};  // Set bit 12 if delta is negative
  constexpr auto offset     = uint32_t {0x1000U}; // offset used to shift value positive
  auto           outputVal  = gsl::narrow_cast<uint32_t>(std::abs(delta)) & mask11bits;
  if (delta < 0) {
	outputVal = (delta + offset) & mask11bits;
	outputVal |= bit12;
  }
  // upper byte has upper 4 bits of the encode value
  auto upperByte = gsl::narrow_cast<uint8_t>(((outputVal >> BYTSHFT) & NIBMASK) | bit8);
  buffer.push_back(upperByte);
  // lower byte has lower 8 bits of the encoded value
  auto lowerByte = gsl::narrow_cast<uint8_t>(outputVal & BYTMASK);
  buffer.push_back(lowerByte);
}

void PES::internal::rpcrd(std::vector<uint8_t>& buffer, fPOINT& thisStitch, float srcX, float srcY) {
  constexpr auto maxDeltaVal = int32_t {63};
  constexpr auto minDeltaVal = int32_t {-64};

  auto const deltaX = std::lround(srcX * PECScale);
  auto const deltaY = -std::lround(srcY * PECScale);
  if (deltaX < maxDeltaVal && deltaX > minDeltaVal && deltaY < maxDeltaVal && deltaY > minDeltaVal) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	auto xVal = gsl::narrow<uint8_t>(deltaX & mask7bits);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	auto yVal = gsl::narrow<uint8_t>(deltaY & mask7bits);
	buffer.push_back(xVal);
	buffer.push_back(yVal);
  }
  else {
	pecEncodeint32_t(buffer, deltaX);
	pecEncodeint32_t(buffer, deltaY);
  }
  thisStitch.x += deltaX * invPECScale;
  thisStitch.y += -deltaY * invPECScale;
}

void PES::internal::pecEncodeStop(std::vector<uint8_t>& buffer, uint8_t val) {
  buffer.push_back(0xfe);
  buffer.push_back(0xb0);
  buffer.push_back(val);
}

void PES::internal::pecdat(std::vector<uint8_t>& buffer) {
  auto* pecHeader = convert_ptr<PECHDR*>(buffer.data());
  auto& pad       = pecHeader->pad;
  PEScolors       = std::begin(pad);
  auto thisStitch = fPOINT {};
  rpcrd(buffer, thisStitch, StitchBuffer->front().x, StitchBuffer->front().y);
  auto iColor  = 1U;
  auto color   = StitchBuffer->front().attribute & COLMSK;
  PEScolors[0] = PESequivColors[color];
  for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size() - 1U); ++iStitch) {
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	if ((stitchFwd1.attribute & COLMSK) != color) {
	  color = stitchFwd1.attribute & COLMSK;
	  pecEncodeStop(buffer, (iColor % 2U) + 1U);
	  PEScolors[iColor] = PESequivColors[color];
	  ++iColor;
	}
	auto const xDelta = stitchFwd1.x - thisStitch.x;
	auto const yDelta = stitchFwd1.y - thisStitch.y;
	rpcrd(buffer, thisStitch, xDelta, yDelta);
  }
  buffer.push_back(0xffU);
  buffer.push_back(0x0U);
}

void PES::internal::writeThumbnail(std::vector<uint8_t>& buffer, uint8_t const (*image)[ThumbHeight][ThumbWidth]) {
  constexpr auto byteWidth = ThumbWidth / 8U;
  if (image != nullptr) {
	for (auto i = 0U; i < ThumbHeight; ++i) {
	  for (auto j = 0U; j < byteWidth; ++j) {
		auto const offset = j * bitsPerByte;
		auto       output = uint8_t {0U};
		for (auto bitPosition = 0U; bitPosition < bitsPerByte; ++bitPosition) {
		  output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + bitPosition] != 0U) << bitPosition;
		}
		buffer.push_back(output);
	  }
	}
  }
}

void PES::internal::pecImage(std::vector<uint8_t>& pecBuffer) {
  uint8_t thumbnail[ThumbHeight][ThumbWidth] = {};
#pragma warning(suppress : 26485) // bounds.3 No array to pointer decay
  uint8_t const(*p_thumbnail)[ThumbHeight][ThumbWidth] = &thumbnail; // 2D arrays are painful to pass as parameters

  auto const yFactor = 31.0F / IniFile.hoopSizeY;
  auto const xFactor = 40.0F / IniFile.hoopSizeX;
  auto const dest    = gsl::span<uint8_t>(&thumbnail[0][0], sizeof(thumbnail));
  // write the overall thumbnail
  std::copy(&imageWithFrame[0][0], &imageWithFrame[0][0] + sizeof(imageWithFrame), dest.begin());
  constexpr auto xOffset = uint16_t {4U}; // thumbnail x offset to place it in the frame correctly
  constexpr auto yOffset = uint16_t {5U}; // thumbnail y offset to place it in the frame correctly
  for (auto& stitch : *StitchBuffer) {
	auto x          = wrap::floor<uint16_t>((stitch.x) * xFactor) + xOffset;
	auto y          = wrap::floor<uint16_t>((stitch.y) * yFactor) + yOffset;
	y               = ThumbHeight - y;
	thumbnail[y][x] = 1U;
  }
  pi::writeThumbnail(pecBuffer, p_thumbnail);
  // now write out the individual thread thumbnails
  std::copy(&imageWithFrame[0][0], &imageWithFrame[0][0] + sizeof(imageWithFrame), dest.begin());
  auto stitchColor = (StitchBuffer->front().attribute & COLMSK);
  // ToDo - check that this is still writing out the thumbnails correctly when starting from index 0 instead of 1
  for (auto& stitch : *StitchBuffer) {
	auto x = wrap::round<uint16_t>((stitch.x) * xFactor) + 3U;
	auto y = wrap::round<uint16_t>((stitch.y) * yFactor) + 3U;
	y      = ThumbHeight - y;
	if (stitchColor == (stitch.attribute & COLMSK)) {
	  thumbnail[y][x] = 1;
	}
	else {
	  pi::writeThumbnail(pecBuffer, p_thumbnail);
	  std::copy(&imageWithFrame[0][0], &imageWithFrame[0][0] + sizeof(imageWithFrame), dest.begin());
	  stitchColor     = (stitch.attribute & COLMSK);
	  thumbnail[y][x] = 1;
	}
  }
  pi::writeThumbnail(pecBuffer, p_thumbnail);
}

auto PES::internal::dupcol(uint32_t activeColor, uint32_t& index) -> uint32_t {
  constexpr auto threadSize  = sizeof(PESThread) / sizeof(PESThread[0]);
  auto const     threadColor = PESThread[PEScolors[index++] % threadSize];
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  auto const color = RGB(threadColor.color.r, threadColor.color.g, threadColor.color.b);
  for (auto iColor = 0U; iColor < activeColor; ++iColor) {
	if (UserColor[iColor] == color) {
	  return iColor;
	}
  }
  auto minimumDistance = std::numeric_limits<uint32_t>::max();
  auto matchIndex      = 0U;
  for (auto iColor = 1U; iColor < activeColor; ++iColor) {
	auto const matchDistance = pesmtch(color, PEScolors[iColor]);
	if (matchDistance < minimumDistance) {
	  minimumDistance = matchDistance;
	  matchIndex      = iColor;
	}
  }
  return matchIndex;
}

auto PES::readPESFile(std::filesystem::path const& newFileName) -> bool {
  auto fileSize = uintmax_t {0};
  if (!thred::getFileSize(newFileName, fileSize)) {
	return false;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	return false;
  }
  auto  fileBuf    = std::vector<uint8_t>(fileSize);
  auto* fileBuffer = fileBuf.data();
  auto  bytesRead  = DWORD {0};
  ReadFile(fileHandle, fileBuffer, fileSize, &bytesRead, nullptr);
  auto* pesHeader = convert_ptr<PESHED*>(fileBuffer);

  constexpr auto PESStr = "#PES00";
  if (strncmp(static_cast<char*>(pesHeader->led), PESStr, strlen(PESStr)) != 0) {
	auto fmtStr = std::wstring {};
	displayText::loadString(fmtStr, IDS_NOTPES);
	displayText::shoMsg(fmt::format(fmtStr, newFileName.wstring()));
	CloseHandle(fileHandle);
	return false;
  }
  auto* pecHeader = convert_ptr<PECHDR*>(&fileBuffer[pesHeader->off]);
  // auto pecHeader2          = convert_ptr<PECHDR2*>(&fileBuffer[pesHeader->off + sizeof(PECHDR)]);
  auto pecOffset             = pesHeader->off + sizeof(PECHDR) + sizeof(PECHDR2);
  auto       PESstitch       = &fileBuffer[pecOffset];
  auto const pesColorCount   = pecHeader->colorCount + 1U;
  auto&      pad             = pecHeader->pad;
  PEScolors                  = std::begin(pad);
  constexpr auto threadCount = sizeof(PESThread) / sizeof(PESThread[0]);
  auto           colorMap    = boost::dynamic_bitset<>(threadCount);
  auto           activeColor = 0U;
  for (auto iColor = 0U; iColor < pesColorCount; ++iColor) {
	if (PEScolors[iColor] < threadCount) {
	  if (!colorMap.test_set(PEScolors[iColor])) {
		auto const threadColor = PESThread[PEScolors[iColor]];
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
		auto const color = RGB(threadColor.color.r, threadColor.color.g, threadColor.color.b);
		UserColor[activeColor++] = color;
		if (activeColor >= COLOR_COUNT) {
		  break;
		}
	  }
	}
	else {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  auto const color = RGB(PESThread[0].color.r,
	                         PESThread[0].color.g,
	                         PESThread[0].color.b); // color unknown

	  UserColor[activeColor++] = color;
	}
  }
  auto PEScolorIndex = 1U;
  auto loc      = fPOINT {};
  StateMap->reset(StateFlag::FILDIR);
  if (bytesRead > ((pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U)) {
	auto       color      = 0U;
	auto       iPESstitch = 0U;
	auto const pecCount   = bytesRead - (pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U;
	StitchBuffer->clear();
	StitchBuffer->reserve(pecCount / 2); // we are still reserving a bit more than necessary
	StitchBuffer->push_back(fPOINTATTR {});
	constexpr auto bit8       = uint32_t {0x80U};   // Is bit 8 set on the upper byte
	constexpr auto bit12      = uint32_t {0x800U};  // Is bit 12 set?
	constexpr auto offset     = uint32_t {0x1000U}; // offset used to shift value negative
	constexpr auto mask12bits = uint32_t {0xFFFU};  // used to mask the value to 12 bits
	while (iPESstitch < pecCount) {
	  if (PESstitch[iPESstitch] == 0xff && PESstitch[iPESstitch + 1U] == 0) {
		break;
	  }
	  if (PESstitch[iPESstitch] == 0xfe && PESstitch[iPESstitch + 1U] == 0xb0) {
		color = pi::dupcol(activeColor, PEScolorIndex);
		iPESstitch += 2;
	  }
	  else {
		auto locof = 0.0F;
		if ((PESstitch[iPESstitch] & bit8) != 0U) {
		  // combine the 4 bits from the first byte with the 8 bits from the next byte
		  auto pesVal =
		      (gsl::narrow_cast<uint32_t>(PESstitch[iPESstitch] << BYTSHFT) | PESstitch[iPESstitch + 1U]) & mask12bits;
		  if ((pesVal & bit12) != 0U) {
			pesVal -= offset;
		  }
		  auto sPesVal = gsl::narrow_cast<int32_t>(pesVal);
		  wrap::narrow_cast(locof, sPesVal);
		  ++iPESstitch;
		}
		else {
		  if (PESstitch[iPESstitch] > 0x3f) {
			locof = wrap::toFloat(PESstitch[iPESstitch]) - 128.0F;
		  }
		  else {
			locof = PESstitch[iPESstitch];
		  }
		}
		locof *= invPECScale;
		// ToDo - (PES) Use a new flag bit for this since FILDIR is not correct
		if (StateMap->testAndFlip(StateFlag::FILDIR)) {
		  loc.y -= locof;
		  StitchBuffer->push_back(fPOINTATTR {loc.x, loc.y, color});
		}
		else {
		  loc.x += locof;
		}
	  }
	  ++iPESstitch;
	}
	// IniFile.auxFileType=AUXPES;
	thred::hupfn();
  }
  else {
	auto fmtStr = std::wstring {};
	displayText::loadString(fmtStr, IDS_NOTPES);
	displayText::shoMsg(fmt::format(fmtStr, newFileName.wstring()));
	CloseHandle(fileHandle);
	return false;
  }
  CloseHandle(fileHandle);
  return true;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
auto PES::savePES(fs::path const* auxName, std::vector<fPOINTATTR> const& saveStitches) -> bool {
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
		auto pesHeader = PESHED {};

		constexpr auto PESStr = "#PES0001";
		constexpr auto EmbStr = "CEmbOne";
		constexpr auto SewStr = "CSewSeg";
		// NOLINTNEXTLINE(clang-diagnostic-deprecated-declarations)
		strncpy(static_cast<char*>(pesHeader.led), PESStr, strlen(PESStr));
		wrap::narrow(pesHeader.celn, strlen(EmbStr));
		// NOLINTNEXTLINE(clang-diagnostic-deprecated-declarations)
		strncpy(static_cast<char*>(pesHeader.ce), EmbStr, pesHeader.celn);
		wrap::narrow(pesHeader.cslen, strlen(SewStr));
		// NOLINTNEXTLINE(clang-diagnostic-deprecated-declarations)
		strncpy(static_cast<char*>(pesHeader.cs), SewStr, pesHeader.cslen);
		auto iColor = 0;
		for (auto const color : UserColor) {
		  auto           matchIndex  = 0U;
		  auto           matchMin    = std::numeric_limits<uint32_t>::max();
		  constexpr auto threadCount = sizeof(PESThread) / sizeof(PESThread[0]);
		  for (auto iColorMatch = 1U; iColorMatch < threadCount; ++iColorMatch) {
			auto const match = pi::pesmtch(color, iColorMatch);
			if (match < matchMin) {
			  matchIndex = iColorMatch;
			  matchMin   = match;
			}
		  }
		  wrap::narrow(PESequivColors[iColor++], matchIndex);
		}
		auto stitchColor  = StitchBuffer->front().attribute & COLMSK;
		auto boundingRect = fRECTANGLE {};
		thred::sizstch(boundingRect, *StitchBuffer);
		auto const offset = fPOINT {wrap::midl(boundingRect.right, boundingRect.left),
		                            wrap::midl(boundingRect.top, boundingRect.bottom)};

		pesHeader.xsiz = wrap::round<uint16_t>((boundingRect.right - boundingRect.left) * PECScale);
		pesHeader.ysiz = wrap::round<uint16_t>((boundingRect.top - boundingRect.bottom) * PECScale);
		auto pesBuffer = std::vector<uint8_t> {};
		// ToDo - make a reasonable guess for the size of data in the PES buffer. err on the side of caution
		auto const pesSize = sizeof(PESSTCHLST) + StitchBuffer->size() * sizeof(PESTCH) + 1000U;
		pesBuffer.reserve(pesSize);
		auto threadList = std::vector<PESCOLORLIST> {};
		auto blockIndex = uint16_t {0U}; // Index into the stitch blocks
		threadList.push_back(PESCOLORLIST {blockIndex, PESequivColors[stitchColor]});
		pesBuffer.resize(sizeof(PESSTCHLST));
		// first block is a jump in place
		pi::ritpesBlock(pesBuffer, PESSTCHLST {1, PESequivColors[stitchColor], 2});
		++blockIndex;
		pi::ritpes(pesBuffer, saveStitches[0], offset);
		pi::ritpes(pesBuffer, saveStitches[0], offset);
		pi::ritpesCode(pesBuffer);
		// then a normal stitch in place
		pi::ritpesBlock(pesBuffer, PESSTCHLST {0, PESequivColors[stitchColor], 2});
		++blockIndex;
		pi::ritpes(pesBuffer, saveStitches[0], offset);
		pi::ritpes(pesBuffer, saveStitches[0], offset);
		pi::ritpesCode(pesBuffer);
		// then a jump to the first location
		pi::ritpesBlock(pesBuffer, PESSTCHLST {1, PESequivColors[stitchColor], 2});
		++blockIndex;
		pi::ritpes(pesBuffer, saveStitches[0], offset);
		pi::ritpes(pesBuffer, saveStitches[1], offset);
		pi::ritpesCode(pesBuffer);
		// now stitch out.
		auto pesThreadCount = 0U;
		auto lastIndex      = pesBuffer.size();
		pi::ritpesBlock(pesBuffer, PESSTCHLST {0, PESequivColors[stitchColor], 0});
		++blockIndex;
		auto stitchCount = 0;
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
			auto* blockHeader        = convert_ptr<PESSTCHLST*>(&pesBuffer[lastIndex]);
			blockHeader->stitchcount = stitchCount;
			// save the thread/color information
			++pesThreadCount;
			stitchColor = StitchBuffer->operator[](iStitch).attribute & COLMSK;
			threadList.push_back(PESCOLORLIST {blockIndex, PESequivColors[stitchColor]});
			// then create the jump block
			pi::ritpesBlock(pesBuffer, PESSTCHLST {1, PESequivColors[stitchColor], 2});
			++blockIndex;
			pi::ritpes(pesBuffer, saveStitches[iStitch - 1U], offset);
			pi::ritpes(pesBuffer, saveStitches[iStitch], offset);
			pi::ritpesCode(pesBuffer);
			// and finally start the next block
			stitchCount = 0;
			lastIndex   = pesBuffer.size();
			pi::ritpesBlock(pesBuffer, PESSTCHLST {0, PESequivColors[stitchColor], 0});
			++blockIndex;
			pi::ritpes(pesBuffer, saveStitches[iStitch], offset);
			++stitchCount;
		  }
		}
		// finalize the last stitch block
		auto* blockHeader        = convert_ptr<PESSTCHLST*>(&pesBuffer[lastIndex]);
		blockHeader->stitchcount = stitchCount;
		// write the color/thread table
		lastIndex = pesBuffer.size();
		pesBuffer.resize(lastIndex + sizeof(uint16_t));
		auto* colorIndex = convert_ptr<uint16_t*>(&pesBuffer[lastIndex]);
		*colorIndex      = pesThreadCount;
		for (auto paletteIndex = 0U; paletteIndex < pesThreadCount; ++paletteIndex) {
		  lastIndex = pesBuffer.size();
		  pesBuffer.resize(lastIndex + 2 * sizeof(uint16_t));
		  auto* colorEntry = convert_ptr<uint16_t*>(&pesBuffer[lastIndex]);
		  *colorEntry      = threadList[paletteIndex].blockIndex;
		  ++colorEntry;
		  *colorEntry = threadList[paletteIndex].colorIndex;
		}
		pesHeader.off     = wrap::toUnsigned(pesBuffer.size() + sizeof(pesHeader));
		pesHeader.blct    = 1;
		pesHeader.hnd1[0] = 0xff;
		pesHeader.hnd1[1] = 0xff;
		pesHeader.hnd1[2] = 0x00;
		pesHeader.hnd1[3] = 0x00;
		pesHeader.bcnt    = pesThreadCount;
		pesHeader.hnd2[0] = 0xff;
		pesHeader.hnd2[1] = 0xff;
		pesHeader.hnd2[2] = 0x00;
		pesHeader.hnd2[3] = 0x00;
		pesHeader.hpsz    = 0;
		GroupStartStitch  = 0;
		GroupEndStitch    = wrap::toUnsigned(StitchBuffer->size() - 1U);
		auto bytesWritten = DWORD {0};
		if (FALSE == WriteFile(fileHandle, convert_ptr<PESHED*>(&pesHeader), sizeof(pesHeader), &bytesWritten, nullptr)) {
		  displayText::riter();
		  flag = false;
		  break;
		}
		if (FALSE == WriteFile(fileHandle, pesBuffer.data(), wrap::toUnsigned(pesBuffer.size()), &bytesWritten, nullptr)) {
		  displayText::riter();
		  flag = false;
		  break;
		}
		pesBuffer.clear();
		pesBuffer.shrink_to_fit();
		auto pecBuffer = std::vector<uint8_t> {};
		// make a reasonable guess for the size of data in the PEC buffer. Assume all stitch coordinates
		// are 2 bytes and pad by 1000 to account for jumps. Also reserve memory for thumbnails
		auto const pecSize = sizeof(PECHDR) + sizeof(PECHDR2) + StitchBuffer->size() * 2 + 1000 +
		                     (wrap::toSize(pesThreadCount) + 1U) * ThumbHeight * (ThumbWidth / 8);
		pecBuffer.reserve(pecSize);
		pecBuffer.resize(sizeof(PECHDR) + sizeof(PECHDR2));
		auto*      pecHeader = convert_ptr<PECHDR*>(pecBuffer.data());
		auto&      label     = pecHeader->label;
		auto const pecLabel  = gsl::span<char>(label);
		pi::pecnam(pecLabel);
		auto fstart = std::next(pecBuffer.begin(), sizeof(pecHeader->label));
		auto fend   = std::next(pecBuffer.begin(), sizeof(*pecHeader));
		std::fill(fstart, fend, ' ');
		pecHeader->labnd = '\r'; // 13 = carriage return
		wrap::narrow(pecHeader->colorCount, pesThreadCount);
		pecHeader->hnd1        = 0x00ff;
		pecHeader->thumbHeight = ThumbHeight;
		pecHeader->thumbWidth  = ThumbWidth / bitsPerByte;
		pi::pecdat(pecBuffer);
		auto* pecHeader2            = convert_ptr<PECHDR2*>(&pecBuffer[sizeof(PECHDR)]);
		pecHeader2->unknown1        = 0;
		pecHeader2->thumbnailOffset = wrap::toUnsigned(pecBuffer.size() - sizeof(PECHDR));
		pecHeader2->unknown2        = 0x3100;
		pecHeader2->unknown3        = 0xf0ff;
		pecHeader2->width           = pesHeader.xsiz;
		pecHeader2->height          = pesHeader.ysiz;
		pecHeader2->unknown4        = 0x01e0;
		pecHeader2->unknown5        = 0x01b0;

		constexpr auto PECMask1    = uint16_t {0x9000U};
		constexpr auto PECLowMask  = uint16_t {0x00ffU};
		constexpr auto PECHighMask = uint16_t {0xff00U};
		constexpr auto PECShift    = uint16_t {8U};

		auto xInt16_le = wrap::round<uint16_t>(boundingRect.left * PECScale);
		auto yInt16_le = wrap::round<uint16_t>(boundingRect.bottom * PECScale);
		xInt16_le |= PECMask1;
		yInt16_le |= PECMask1;
		pecHeader2->xMin =
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(xInt16_le & PECHighMask) >> PECShift) |
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(xInt16_le & PECLowMask) << PECShift);
		pecHeader2->yMin =
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(yInt16_le & PECHighMask) >> PECShift) |
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(yInt16_le & PECLowMask) << PECShift);
		pi::pecImage(pecBuffer);
		if (FALSE == WriteFile(fileHandle, pecBuffer.data(), wrap::toUnsigned(pecBuffer.size()), &bytesWritten, nullptr)) {
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

#endif