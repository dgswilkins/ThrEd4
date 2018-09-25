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
#include <htmlhelp.h>
#include <shellapi.h>
#include <shlobj.h>

// Standard Libraries
#include <bitset>
#include <filesystem>
#include <limits> 
#include <string>
#include <vector>

// Open Source headers
#define ALL_CPPCORECHECK_WARNINGS 26400 26401 26402 26403 26404 26405 26406 26407 26408 26409 26410 26411 26414 26415 26416 26417 26418 26426 26427 26429 26430 26431 26432 26433 26434 26435 26436 26437 26438 26439 26440 26441 26443 26444 26445 26446 26447 26448 26449 26450 26451 26452 26453 26454 26459 26460 26461 26462 26463 26464 26465 26466 26471 26472 26473 26474 26475 26476 26477 26481 26482 26483 26485 26486 26487 26489 26490 26491 26492 26493 26494 26495 26496 26497 26498
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
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
