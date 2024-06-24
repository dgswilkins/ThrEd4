#pragma once

// Standard Libraries
#include <cstdint>

class CLIP_STITCH
{
  public:
  uint32_t led {}; // stitchcount in the first record and color otherwise
  uint8_t  fx {};  // fractional part of x coord
  uint16_t x {};   // integer part of x coord
  uint8_t  fy {};  // fractional part of y coord
  uint16_t y {};   // integer part of y coord
};
