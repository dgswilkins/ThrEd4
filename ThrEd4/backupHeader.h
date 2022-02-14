#pragma once

// Local Headers
#include "formHeader.h"
#include "textureHeader.h"

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
