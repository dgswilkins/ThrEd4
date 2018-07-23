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
#include <bitset>
#include <commdlg.h>
#include <filesystem>
#include <htmlhelp.h>
#include <limits> 
#include <shellapi.h>
#include <shlobj.h>
#include <Shlwapi.h>
#include <string>
#include <vector>

// TODO: reference additional headers your program requires here
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header in x86-windows triplet
#include <boost/dynamic_bitset.hpp>
#include <fmt/format.h>
#include <gsl/gsl>
#pragma warning(pop)

#include "EnumMap.h"
#include "ExtendedBitSet.h"
#include "utf8conv.h"
#include "wrappers.h"
