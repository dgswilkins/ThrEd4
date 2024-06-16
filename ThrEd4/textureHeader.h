#pragma once
// Local Headers
#include "point.h"

// Standard Libraries
#include <cstdint>
#include <vector>

#pragma pack(push, 1)
class TX_PNT // textured fill point
{
  public:
  float    y {};
  uint16_t line {};

  // constexpr TX_PNT() noexcept = default;
  // TX_PNT(TX_PNT const&) = default;
  // TX_PNT(TX_PNT&&) = default;
  // TX_PNT& operator=(TX_PNT const& rhs) = default;
  // TX_PNT& operator=(TX_PNT&&) = default;
  //~TX_PNT() = default;
};
#pragma pack(pop)

class TXTR_SCREEN
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  int32_t  top {};              // pixel top line
  int32_t  bottom {};           // pixel bottom line
  int32_t  height {};           // pixel height of area
  int32_t  halfHeight {};       // pixel middle of area
  float    xOffset {};          // edit x offset of area
  float    yOffset {};          // edit y offset of area
  float    areaHeight {};       // edit height of area
  float    screenHeight {};     // edit height of screen
  float    width {};            // edit width of area
  float    spacing {};          // edit space between lines
  uint16_t lines {};            // number of lines
  float    editToPixelRatio {}; // edit to pixel ratio
  F_POINT  formCenter;          // middle of the form
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  explicit constexpr TXTR_SCREEN() noexcept = default;
  // TXTR_SCREEN(TXTR_SCREEN const&) = default;
  // TXTR_SCREEN(TXTR_SCREEN&&) = default;
  // TXTR_SCREEN& operator=(TXTR_SCREEN const& rhs) = default;
  // TXTR_SCREEN& operator=(TXTR_SCREEN&&) = default;
  //~TXTR_SCREEN() = default;
};

class TX_HIST
{
  public:
  std::vector<TX_PNT> texturePoints;

  float height {};
  float width {};
  float spacing {};

  // constexpr TX_HIST() noexcept = default;
  // TX_HIST(TX_HIST const&) = default;
  // TX_HIST(TX_HIST&&) = default;
  // TX_HIST& operator=(TX_HIST const& rhs) = default;
  // TX_HIST& operator=(TX_HIST&&) = default;
  //~TX_HIST() = default;
};
