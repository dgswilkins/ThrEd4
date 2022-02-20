#pragma once

// Standard Libraries
#include <cstdint>

class CLIP_STITCH
{
  public:
  uint32_t led {};  // stitchcount in the first record and color otherwise
  uint8_t  fx {};   // fractional part of x coord
  uint16_t x {};    // integer part of x coord
  uint8_t  spcx {}; // ToDo - Is this structure member needed?
  uint8_t  fy {};   // fractional part of y coord
  uint16_t y {};    // integer part of y coord
  uint8_t  spcy {}; // ToDo - Is this structure member needed?
  uint8_t  myst {}; // ToDo - Is this structure member needed?
  uint8_t  tag {};  // ToDo - Is this structure member needed?

  // constexpr CLIP_STITCH() noexcept = default;
  // CLIP_STITCH(CLIP_STITCH const&) = default;
  // CLIP_STITCH(CLIP_STITCH&&) = default;
  // CLIP_STITCH& operator=(CLIP_STITCH const& rhs) = default;
  // CLIP_STITCH& operator=(CLIP_STITCH&&) = default;
  //~CLIP_STITCH() = default;
};
