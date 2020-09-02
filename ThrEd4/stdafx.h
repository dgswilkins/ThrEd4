// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

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
#include <bitset>
#include <filesystem>
#include <limits>
#include <string>
#include <vector>

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26812) // supress warning for fmt library header
#pragma warning(disable : 26814) // supress warning for boost library header
#include <boost/dynamic_bitset.hpp>
#include <fmt/format.h>
#include <gsl/gsl>
#pragma warning(pop)

// Local Headers
#include "EnumMap.h"
#include "ExtendedBitSet.h"
#include "utf8conv.h"
#include "wrappers.h"

#include "ThrEdTypes.h"
