// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef DEBUG_PCH

// Local Headers
#include "clipStitch.h"
#include "EnumMap.h"
#include "ExtendedBitSet.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "point.h"
#include "satCon.h"
#include "textureHeader.h"
#include "ThrEdTypes.h"
#include "utf8conv.h"
#include "warnings.h"
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset.hpp"
#include "fmt/format.h"
#include "gsl/gsl"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// C RunTime Header Files
#include <commdlg.h>
#include <HtmlHelp.h>
#include <shellapi.h>
#include <ShlObj.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <climits>
#include <cstring>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <cwctype>
#include <filesystem>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include <xutility>

#endif // DEBUG_PCH
