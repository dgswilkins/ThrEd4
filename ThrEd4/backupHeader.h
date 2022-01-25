#pragma once

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

// Open Source headers

// Local Headers
#include "formHeader.h"
#include "ThrEdTypes.h"

#pragma pack(push, 1)

class BACK_HEAD // Backup header
{
  public:
  uint32_t      formCount {};
  FRM_HEAD*     forms {};
  uint32_t      stitchCount {};
  F_POINT_ATTR* stitches {};
  uint32_t      vertexCount {};
  F_POINT*      vertices {};
  uint32_t      guideCount {};
  SAT_CON*      guide {};
  uint32_t      clipPointCount {};
  F_POINT*      clipPoints {};
  COLORREF*     colors {};
  TX_PNT*       texturePoints {};
  uint32_t      texturePointCount {};
  SIZE          zoomRect {};

  // constexpr BACK_HEAD() noexcept = default;
  // BACK_HEAD(CLIP_STITCH const&) = default;
  // BACK_HEAD(CLIP_STITCH&&) = default;
  // BACK_HEAD& operator=(BACK_HEAD const& rhs) = default;
  // BACK_HEAD& operator=(BACK_HEAD&&) = default;
  //~BACK_HEAD() = default;
};

#pragma pack(pop)
