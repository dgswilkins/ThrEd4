// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <bitset>
#include <commdlg.h>
#include <cstddef>
#include <filesystem>
#include <htmlhelp.h>
#include <limits> // For std::numeric_limits
#include <shellapi.h>
#include <Shlwapi.h>
#include <stdint.h>  // for uint32_t
#include <stdexcept> // for std::runtime_error
#include <string>    // for std::string
#include <crtdbg.h>  // For _ASSERTE()
#include <Windows.h> // Win32 Platform SDK main header

// TODO: reference additional headers your program requires here
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <boost/dynamic_bitset.hpp>
#include <boost/endian/conversion.hpp>
#include <fmt/format.h>
#include <gsl/gsl>
#pragma warning(pop)
