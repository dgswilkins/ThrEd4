#pragma once

// Local Headers
#include "satConHeader.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cstdint>

union FANGCLPOUT;

union FANGCLP {
  public:
  float    angle;
  uint32_t clip; // pointer to start of fill clipboard data
  SAT_CON  guide {};

  inline FANGCLP() noexcept;
  // FANGCLP(FANGCLP const&) = default;
  // FANGCLP(FANGCLP&&) = default;
  // FANGCLP& operator=(FANGCLP const& rhs) = default;
  // FANGCLP& operator=(FANGCLP&&) = default;
  //~FANGCLP() = default;
};

inline FANGCLP::FANGCLP() noexcept {
  guide.start  = 0U;
  guide.finish = 0U;
}

#pragma pack(push, 1)
union FANGCLPOUT {
  public:
  float angle;
  DWORD clip {}; // clip pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
  SAT_CON_OUT guide;

  inline FANGCLPOUT() noexcept;
  // FANGCLPOUT(FANGCLPOUT const&) = default;
  // FANGCLPOUT(FANGCLPOUT&&) = default;
  // FANGCLPOUT& operator=(FANGCLPOUT const& rhs) = default;
  // FANGCLPOUT& operator=(FANGCLPOUT&&) = default;
  //~FANGCLPOUT() = default;
};
#pragma pack(pop)

inline FANGCLPOUT::FANGCLPOUT() noexcept {
  guide.start  = 0U;
  guide.finish = 0U;
}
