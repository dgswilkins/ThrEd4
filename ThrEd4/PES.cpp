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

constexpr auto PECFACT  = 5.0F / 3.0F;       // PEC format scale factor
constexpr auto IPECFACT = 3.0F / 5.0F;       // inverse PEC format scale factor
constexpr auto BTSPBYTE = 8U;                // bits per byte
constexpr auto BIT8     = uint32_t {0x80U};  // Set bit 8 on the upper byte
constexpr auto BIT12    = uint32_t {0x800U}; // Set bit 12 if delta is negative
constexpr auto POSOFF   = int32_t {0x1000};  // offset used to shift value positive

static auto PEScolors      = gsl::narrow_cast<uint8_t*>(nullptr); // pes colors
static auto PESequivColors = std::array<uint8_t, COLORCNT> {};    // pes equivalent colors

static constexpr auto index00 = THREAD {{0x00, 0x00, 0x00}, "Unknown", ""};
static constexpr auto index01 = THREAD {{0x1a, 0x0a, 0x94}, "Prussian Blue", "ETP007"};
static constexpr auto index02 = THREAD {{0x0f, 0x75, 0xff}, "Blue", "ETP405"};
static constexpr auto index03 = THREAD {{0x00, 0x93, 0x4c}, "Teal Green", "ETP534"};
static constexpr auto index04 = THREAD {{0xba, 0xbd, 0xfe}, "Corn Flower Blue", "ETP070"};
static constexpr auto index05 = THREAD {{0xec, 0x00, 0x00}, "Red", "ETP800"};
static constexpr auto index06 = THREAD {{0xe4, 0x99, 0x5a}, "Reddish Brown", "ETP337"};
static constexpr auto index07 = THREAD {{0xcc, 0x48, 0xab}, "Magenta", "ETP620"};
static constexpr auto index08 = THREAD {{0xfd, 0xc4, 0xfa}, "Light Lilac", "ETP810"};
static constexpr auto index09 = THREAD {{0xdd, 0x84, 0xcd}, "Lilac", "ETP612"};
static constexpr auto index10 = THREAD {{0x6b, 0xd3, 0x8a}, "Mint Green", "ETP502"};
static constexpr auto index11 = THREAD {{0xe4, 0xa9, 0x45}, "Deep Gold", "ETP214"};
static constexpr auto index12 = THREAD {{0xff, 0xbd, 0x42}, "Orange", "ETP208"};
static constexpr auto index13 = THREAD {{0xff, 0xe6, 0x00}, "Yellow", "ETP205"};
static constexpr auto index14 = THREAD {{0x6c, 0xd9, 0x00}, "Lime Green", "ETP513"};
static constexpr auto index15 = THREAD {{0xc1, 0xa9, 0x41}, "Brass", "ETP328"};
static constexpr auto index16 = THREAD {{0xb5, 0xad, 0x97}, "Silver", "ETP005"};
static constexpr auto index17 = THREAD {{0xba, 0x9c, 0x5f}, "Russet Brown", "ETP330"};
static constexpr auto index18 = THREAD {{0xfa, 0xf5, 0x9e}, "Cream Brown", "ETP010"};
static constexpr auto index19 = THREAD {{0x80, 0x80, 0x80}, "Pewter", "ETP704"};
static constexpr auto index20 = THREAD {{0x00, 0x00, 0x00}, "Black", "ETP900"};
static constexpr auto index21 = THREAD {{0x00, 0x1c, 0xdf}, "Ultramarine", "ETP406"};
static constexpr auto index22 = THREAD {{0xdf, 0x00, 0xb8}, "Royal Purple", "ETP869"};
static constexpr auto index23 = THREAD {{0x62, 0x62, 0x62}, "Dark Gray", "ETP707"};
static constexpr auto index24 = THREAD {{0x69, 0x26, 0x0d}, "Dark Brown", "ETP058"};
static constexpr auto index25 = THREAD {{0xff, 0x00, 0x60}, "Deep Rose", "ETP086"};
static constexpr auto index26 = THREAD {{0xbf, 0x82, 0x00}, "Light Brown", "ETP323"};
static constexpr auto index27 = THREAD {{0xf3, 0x91, 0x78}, "Salmon Pink", "ETP079"};
static constexpr auto index28 = THREAD {{0xff, 0x68, 0x05}, "Vermilion", "ETP030"};
static constexpr auto index29 = THREAD {{0xf0, 0xf0, 0xf0}, "White", "ETP001"};
static constexpr auto index30 = THREAD {{0xc8, 0x32, 0xcd}, "Violet", "ETP613"};
static constexpr auto index31 = THREAD {{0xb0, 0xbf, 0x9b}, "Seacrest", "ETP542"};
static constexpr auto index32 = THREAD {{0x65, 0xbf, 0xeb}, "Sky Blue", "ETP019"};
static constexpr auto index33 = THREAD {{0xff, 0xba, 0x04}, "Pumpkin", "ETP126"};
static constexpr auto index34 = THREAD {{0xff, 0xf0, 0x6c}, "Cream Yellow", "ETP010"};
static constexpr auto index35 = THREAD {{0xfe, 0xca, 0x15}, "Khaki", "ETP348"};
static constexpr auto index36 = THREAD {{0xf3, 0x81, 0x01}, "Clay Brown", "ETP339"};
static constexpr auto index37 = THREAD {{0x37, 0xa9, 0x23}, "Leaf Green", "ETP509"};
static constexpr auto index38 = THREAD {{0x23, 0x46, 0x5f}, "Peacock Blue", "ETP405"};
static constexpr auto index39 = THREAD {{0xa6, 0xa6, 0x95}, "Gray", "ETP707"};
static constexpr auto index40 = THREAD {{0xce, 0xbf, 0xa6}, "Warm Gray", "ETP399"};
static constexpr auto index41 = THREAD {{0x96, 0xaa, 0x02}, "Dark Olive", "ETP517"};
static constexpr auto index42 = THREAD {{0xff, 0xe3, 0xc6}, "Linen", "ETP307"};
static constexpr auto index43 = THREAD {{0xff, 0x99, 0xd7}, "Pink", "ETP085"};
static constexpr auto index44 = THREAD {{0x00, 0x70, 0x04}, "Deep Green", "ETP808"};
static constexpr auto index45 = THREAD {{0xed, 0xcc, 0xfb}, "Lavender", "ETP804"};
static constexpr auto index46 = THREAD {{0xc0, 0x89, 0xd8}, "Wisteria Violet", "ETP607"};
static constexpr auto index47 = THREAD {{0xe7, 0xd9, 0xb4}, "Beige", "ETP843"};
static constexpr auto index48 = THREAD {{0xe9, 0x0e, 0x86}, "Carmine", "ETP807"};
static constexpr auto index49 = THREAD {{0xcf, 0x68, 0x29}, "Amber Red", "ETP333"};
static constexpr auto index50 = THREAD {{0x40, 0x86, 0x15}, "Olive Green", "ETP519"};
static constexpr auto index51 = THREAD {{0xdb, 0x17, 0x97}, "Dark Fuchsia", "ETP107"};
static constexpr auto index52 = THREAD {{0xff, 0xa7, 0x04}, "Tangerine", "ETP209"};
static constexpr auto index53 = THREAD {{0xb9, 0xff, 0xff}, "Light Blue", "ETP017"};
static constexpr auto index54 = THREAD {{0x22, 0x89, 0x27}, "Emerald Green", "ETP507"};
static constexpr auto index55 = THREAD {{0xb6, 0x12, 0xcd}, "Purple", "ETP614"};
static constexpr auto index56 = THREAD {{0x00, 0xaa, 0x00}, "Moss Green", "ETP515"};
static constexpr auto index57 = THREAD {{0xfe, 0xa9, 0xdc}, "Flesh Pink", "ETP124"};
static constexpr auto index58 = THREAD {{0xfe, 0xd5, 0x10}, "Harvest Gold", "ETP206"};
static constexpr auto index59 = THREAD {{0x00, 0x97, 0xdf}, "Electric Blue", "ETP420"};
static constexpr auto index60 = THREAD {{0xff, 0xff, 0x84}, "Lemon Yellow", "ETP205"};
static constexpr auto index61 = THREAD {{0xcf, 0xe7, 0x74}, "Fresh Green", "ETP027"};
static constexpr auto index62 = THREAD {{0xff, 0xc8, 0x64}, "Applique Material", ""};
static constexpr auto index63 = THREAD {{0xff, 0xc8, 0xc8}, "Applique Position", ""};
static constexpr auto index64 = THREAD {{0xff, 0xc8, 0xc8}, "Applique", ""};

static constexpr auto PESThread = std::array<THREAD, 65> {
    index00, index01, index02, index03, index04, index05, index06, index07, index08, index09,
    index10, index11, index12, index13, index14, index15, index16, index17, index18, index19,
    index20, index21, index22, index23, index24, index25, index26, index27, index28, index29,
    index30, index31, index32, index33, index34, index35, index36, index37, index38, index39,
    index40, index41, index42, index43, index44, index45, index46, index47, index48, index49,
    index50, index51, index52, index53, index54, index55, index56, index57, index58, index59,
    index60, index61, index62, index63, index64};

constexpr auto THTYPCNT = PESThread.size(); // Count of thread colors available in the PES format

// clang-format off
static constexpr auto imageWithFrame = imgArray{{ 
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

auto PES::internal::pesmtch(COLORREF const& referenceColor, uint8_t const& colorIndex) -> uint32_t {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  auto color = PECCOLOR {GetRValue(referenceColor), GetGValue(referenceColor), GetBValue(referenceColor)};
  auto translatedColor = PESThread[colorIndex].color;
  auto const meanR = (gsl::narrow_cast<int32_t>(color.r) + gsl::narrow_cast<int32_t>(translatedColor.r)) / 2;
  auto const deltaR = gsl::narrow_cast<int32_t>(color.r) - gsl::narrow_cast<int32_t>(translatedColor.r);
  auto const deltaG = gsl::narrow_cast<int32_t>(color.g) - gsl::narrow_cast<int32_t>(translatedColor.g);
  auto const deltaB = gsl::narrow_cast<int32_t>(color.b) - gsl::narrow_cast<int32_t>(translatedColor.b);
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula
  return wrap::round<uint32_t>(std::sqrt((((512 + meanR) * deltaR * deltaR) / 256) + 4 * deltaG * deltaG +
                                         (((767 - meanR) * deltaB * deltaB) / 256)));
}

void PES::internal::ritpes(std::vector<uint8_t>& buffer, fPOINTATTR const& stitch, fPOINT const& offset) {
  auto const oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESTCH));
  auto*      pesStitch    = convert_ptr<PESTCH*>(&buffer[oldSize]);
  auto const scaledStitch = fPOINT {-stitch.x * IPECFACT + offset.x, stitch.y * IPECFACT - offset.y};
  pesStitch->x            = wrap::round<int16_t>(scaledStitch.x);
  pesStitch->y            = wrap::round<int16_t>(scaledStitch.y);
}

void PES::internal::ritpesCode(std::vector<uint8_t>& buffer) {
  constexpr auto ENDCODE = uint16_t {0x8003U}; // Block end code
  auto const     oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(uint16_t));
  auto* contCode = convert_ptr<uint16_t*>(&buffer[oldSize]);
  if (contCode != nullptr) {
	*contCode = ENDCODE;
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
  // ReSharper disable once CppDeprecatedEntity
  strncpy(label.data(), "LA:", 3); // NOLINT(clang-diagnostic-deprecated-declarations)
  auto const lblSize  = wrap::toUnsigned(label.size() - 3U);
  auto       fileStem = utf::Utf16ToUtf8(AuxName->stem());
  if (fileStem.size() < lblSize) {
	fileStem += std::string(lblSize - fileStem.size(), ' ');
  }
  // ReSharper disable once CppDeprecatedEntity
  strncpy(&label[3], fileStem.c_str(), lblSize); // NOLINT(clang-diagnostic-deprecated-declarations)
}
#pragma warning(pop)

void PES::internal::pecEncodeint32_t(std::vector<uint8_t>& buffer, int32_t delta) {
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

void PES::internal::rpcrd(std::vector<uint8_t>& buffer, fPOINT& thisStitch, float srcX, float srcY) {
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
	pecEncodeint32_t(buffer, deltaX);
	pecEncodeint32_t(buffer, deltaY);
  }
  thisStitch.x += wrap::toFloat(deltaX) * IPECFACT;
  thisStitch.y += -wrap::toFloat(deltaY) * IPECFACT;
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
  auto       iColor           = 1U;
  auto       color            = StitchBuffer->front().attribute & COLMSK;
  auto const spPESequivColors = gsl::span<uint8_t> {PESequivColors};
  PEScolors[0]                = spPESequivColors[color];
  for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size() - 1U); ++iStitch) {
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	if ((stitchFwd1.attribute & COLMSK) != color) {
	  color = stitchFwd1.attribute & COLMSK;
	  pecEncodeStop(buffer, (iColor % 2U) + 1U);
	  PEScolors[iColor] = spPESequivColors[color];
	  ++iColor;
	}
	auto const xDelta = stitchFwd1.x - thisStitch.x;
	auto const yDelta = stitchFwd1.y - thisStitch.y;
	rpcrd(buffer, thisStitch, xDelta, yDelta);
  }
  buffer.push_back(0xffU);
  buffer.push_back(0x0U);
}

void PES::internal::writeThumbnail(std::vector<uint8_t>& buffer, imgArray& image) {
  for (auto imageRow : image) {
	auto& iRow = imageRow;
	for (auto iPixel = iRow.begin(); iPixel != iRow.end();) {
	  auto output = uint8_t {0U};
	  for (auto bitPosition = 0U; bitPosition < BTSPBYTE; ++bitPosition) {
		output |= gsl::narrow_cast<uint8_t>(*iPixel << bitPosition);
		++iPixel;
	  }
	  buffer.push_back(output);
	}
  }
}

void PES::internal::pecImage(std::vector<uint8_t>& pecBuffer) {
  auto       thumbnail = imageWithFrame;
  auto const yFactor   = 31.0F / IniFile.hoopSizeY;
  auto const xFactor   = 40.0F / IniFile.hoopSizeX;
  // write the overall thumbnail
  constexpr auto XOFFSET = uint8_t {4U}; // thumbnail x offset to place it in the frame correctly
  constexpr auto YOFFSET = uint8_t {5U}; // thumbnail y offset to place it in the frame correctly
  auto const     spThumbnail = gsl::span<std::array<uint8_t, THUMBWID>> {thumbnail};
  for (auto& stitch : *StitchBuffer) {
	auto const x = wrap::toSize(wrap::round<uint16_t>(stitch.x * xFactor) + XOFFSET);
	auto const y = wrap::toSize(THUMBHGT - (wrap::round<uint16_t>(stitch.y * yFactor) + YOFFSET));
	auto const spRow = gsl::span<uint8_t> {spThumbnail[y]};
	spRow[x]         = 1U;
  }
  pi::writeThumbnail(pecBuffer, thumbnail);
  // now write out the individual thread thumbnails
  thumbnail        = imageWithFrame;
  auto stitchColor = (StitchBuffer->front().attribute & COLMSK);
  for (auto& stitch : *StitchBuffer) {
	auto const x = wrap::toSize(wrap::round<uint16_t>(stitch.x * xFactor) + XOFFSET);
	auto const y = wrap::toSize(THUMBHGT - (wrap::round<uint16_t>(stitch.y * yFactor) + YOFFSET));
	auto const spRow = gsl::span<uint8_t> {spThumbnail[y]};
	if (stitchColor == (stitch.attribute & COLMSK)) {
	  spRow[x] = 1U;
	}
	else {
	  pi::writeThumbnail(pecBuffer, thumbnail);
	  thumbnail   = imageWithFrame;
	  stitchColor = (stitch.attribute & COLMSK);
	  spRow[x]    = 1U;
	}
  }
  pi::writeThumbnail(pecBuffer, thumbnail);
}

auto PES::internal::dupcol(uint32_t activeColor, uint32_t& index) -> uint32_t {
  auto const& threadColor = PESThread[PEScolors[index++] % THTYPCNT];
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
  auto fileBuf = std::vector<uint8_t> {};
  fileBuf.reserve(wrap::toSize(fileSize));
  auto* fileBuffer = fileBuf.data();
  auto  bytesRead  = DWORD {0};
  wrap::ReadFile(fileHandle, fileBuffer, gsl::narrow<DWORD>(fileSize), &bytesRead, nullptr);
  auto* pesHeader = convert_ptr<PESHED*>(fileBuffer);

  constexpr auto PESSTR = "#PES00"; // PES lead in value
  if (strncmp(static_cast<char*>(pesHeader->led), PESSTR, strlen(PESSTR)) != 0) {
	displayText::showMessage(IDS_NOTPES, newFileName.wstring());
	CloseHandle(fileHandle);
	return false;
  }
  auto*      pecHeader     = convert_ptr<PECHDR*>(&fileBuffer[pesHeader->off]);
  auto const pecOffset     = pesHeader->off + sizeof(PECHDR) + sizeof(PECHDR2);
  auto*      PESstitch     = &fileBuffer[pecOffset];
  auto const pesColorCount = pecHeader->colorCount + 1U;
  auto&      pad           = pecHeader->pad;
  PEScolors                = std::begin(pad);
  auto colorMap            = boost::dynamic_bitset<>(THTYPCNT);
  auto iUserColor          = UserColor.begin();
  for (auto iColor = 0U; iColor < pesColorCount; ++iColor) {
	if (PEScolors[iColor] < THTYPCNT) {
	  if (!colorMap.test_set(PEScolors[iColor])) {
		auto const& threadColor = PESThread[PEScolors[iColor]];
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
		auto const color = RGB(threadColor.color.r, threadColor.color.g, threadColor.color.b);
		*iUserColor      = color;
		++iUserColor;
		if (iUserColor == UserColor.end()) {
		  break;
		}
	  }
	}
	else {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  auto const color = RGB(PESThread[0].color.r,
	                         PESThread[0].color.g,
	                         PESThread[0].color.b); // color unknown

	  *iUserColor = color;
	  ++iUserColor;
	}
  }
  auto loc = fPOINT {};
  StateMap->reset(StateFlag::FILDIR);
  if (bytesRead > ((pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U)) {
	auto       color      = 0U;
	auto       iPESstitch = 0U;
	auto const pecCount   = bytesRead - (pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U;
	StitchBuffer->clear();
	StitchBuffer->reserve(pecCount / 2); // we are still reserving a bit more than necessary
	StitchBuffer->push_back(fPOINTATTR {});
	constexpr auto MSK12BIT = uint32_t {0xFFFU}; // used to mask the value to 12 bits

	auto PEScolorIndex = uint32_t {1U};
	while (iPESstitch < pecCount) {
	  if (PESstitch[iPESstitch] == 0xff && PESstitch[iPESstitch + 1U] == 0) {
		break;
	  }
	  if (PESstitch[iPESstitch] == 0xfe && PESstitch[iPESstitch + 1U] == 0xb0) {
		color = pi::dupcol(wrap::toUnsigned(std::distance(UserColor.begin(), iUserColor)), PEScolorIndex);
		iPESstitch += 2;
	  }
	  else {
		auto locof = 0.0F;
		if ((PESstitch[iPESstitch] & BIT8) != 0U) {
		  // combine the 4 bits from the first byte with the 8 bits from the next byte
		  auto pesVal =
		      (gsl::narrow_cast<uint32_t>(PESstitch[iPESstitch] << BYTSHFT) | PESstitch[iPESstitch + 1U]) & MSK12BIT;
		  if ((pesVal & BIT12) != 0U) {
			pesVal -= POSOFF;
		  }
		  auto const sPesVal = gsl::narrow_cast<int32_t>(pesVal);
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
		locof *= IPECFACT;
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
	thred::hupfn();
  }
  else {
	displayText::showMessage(IDS_NOTPES, newFileName.wstring());
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
		auto           pesHeader        = PESHED {};
		auto const     spPESequivColors = gsl::span<uint8_t> {PESequivColors};
		constexpr auto PESSTR           = "#PES0001"; // PES lead in
		constexpr auto EMBSTR           = "CEmbOne";  // emb section lead in
		constexpr auto SEWSTR           = "CSewSeg";  // sewing segment leadin
		// ReSharper disable once CppDeprecatedEntity
		strncpy(static_cast<char*>(pesHeader.led), PESSTR, strlen(PESSTR)); // NOLINT(clang-diagnostic-deprecated-declarations)
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
		auto boundingRect = fRECTANGLE {};
		thred::sizstch(boundingRect, *StitchBuffer);
		auto const offset = fPOINT {wrap::midl(boundingRect.right, boundingRect.left),
		                            wrap::midl(boundingRect.top, boundingRect.bottom)};

		pesHeader.xsiz = wrap::round<int16_t>((boundingRect.right - boundingRect.left) * PECFACT);
		pesHeader.ysiz = wrap::round<int16_t>((boundingRect.top - boundingRect.bottom) * PECFACT);
		auto pesBuffer = std::vector<uint8_t> {};
		// ToDo - make a reasonable guess for the size of data in the PES buffer. err on the side of caution
		auto const pesSize = sizeof(PESSTCHLST) + StitchBuffer->size() * sizeof(PESTCH) + 1000U;
		pesBuffer.reserve(pesSize);
		auto threadList   = std::vector<PESCOLORLIST> {};
		auto blockIndex   = uint16_t {0U}; // Index into the stitch blocks
		auto currentColor = spPESequivColors[stitchColor];
		threadList.push_back(PESCOLORLIST {blockIndex, currentColor});
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
			auto* blockHeader        = convert_ptr<PESSTCHLST*>(&pesBuffer[lastIndex]);
			blockHeader->stitchcount = gsl::narrow<uint16_t>(stitchCount);
			// save the thread/color information
			++pesThreadCount;
			stitchColor  = StitchBuffer->operator[](iStitch).attribute & COLMSK;
			currentColor = spPESequivColors[stitchColor];
			threadList.push_back(PESCOLORLIST {blockIndex, currentColor});
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
		auto* blockHeader        = convert_ptr<PESSTCHLST*>(&pesBuffer[lastIndex]);
		blockHeader->stitchcount = gsl::narrow_cast<uint16_t>(stitchCount);
		// write the color/thread table
		lastIndex = pesBuffer.size();
		pesBuffer.resize(lastIndex + sizeof(uint16_t));
		auto* colorIndex = convert_ptr<uint16_t*>(&pesBuffer[lastIndex]);
		*colorIndex      = gsl::narrow_cast<uint16_t>(pesThreadCount);
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
		pesHeader.bcnt    = gsl::narrow_cast<uint16_t>(pesThreadCount);
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
		                     (wrap::toSize(pesThreadCount) + 1U) * THUMBHGT * (THUMBWID / 8);
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
		pecHeader->thumbHeight = THUMBHGT;
		pecHeader->thumbWidth  = THUMBWID / BTSPBYTE;
		pi::pecdat(pecBuffer);
		auto* pecHeader2            = convert_ptr<PECHDR2*>(&pecBuffer[sizeof(PECHDR)]);
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

		auto xInt16_le = wrap::round<uint16_t>(boundingRect.left * PECFACT);
		auto yInt16_le = wrap::round<uint16_t>(boundingRect.bottom * PECFACT);
		xInt16_le |= PECMASK1;
		yInt16_le |= PECMASK1;
		pecHeader2->xMin =
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(xInt16_le & HIGHBMSK) >> PECSHIFT) |
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(xInt16_le & LOWBMASK) << PECSHIFT);
		pecHeader2->yMin =
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(yInt16_le & HIGHBMSK) >> PECSHIFT) |
		    gsl::narrow_cast<uint16_t>(gsl::narrow_cast<uint16_t>(yInt16_le & LOWBMASK) << PECSHIFT);
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
