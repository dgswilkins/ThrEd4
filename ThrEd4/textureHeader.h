#pragma once
// Local Headers
#include "pointHeader.h"

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

#pragma pack(push, 1)
class TX_OFF // textured fill offset
{
  public:
  float   y {};
  int32_t line {};

  // constexpr TX_OFF() noexcept = default;
  // TX_OFF(TX_OFF const&) = default;
  // TX_OFF(TX_OFF&&) = default;
  // TX_OFF& operator=(TX_OFF const& rhs) = default;
  // TX_OFF& operator=(TX_OFF&&) = default;
  //~TX_OFF() = default;
};
#pragma pack(pop)

class TXTR_SCREEN
{
  public:
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

  // constexpr TXTR_SCREEN() noexcept = default;
  // TXTR_SCREEN(TXTR_SCREEN const&) = default;
  // TXTR_SCREEN(TXTR_SCREEN&&) = default;
  // TXTR_SCREEN& operator=(TXTR_SCREEN const& rhs) = default;
  // TXTR_SCREEN& operator=(TXTR_SCREEN&&) = default;
  //~TXTR_SCREEN() = default;
};

class TXTR_RECT
{
  public:
  uint16_t left {};
  uint16_t right {};
  float    top {};
  float    bottom {};

  // constexpr TXTR_RECT() noexcept = default;
  // TXTR_RECT(TXTR_RECT const&) = default;
  // TXTR_RECT(TXTR_RECT&&) = default;
  // TXTR_RECT& operator=(TXTR_RECT const& rhs) = default;
  // TXTR_RECT& operator=(TXTR_RECT&&) = default;
  //~TXTR_RECT() = default;
};

class TX_HIST
{
  public:
  std::vector<TX_PNT> texturePoints {};

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

class TX_HIST_BUFF
{
  public:
  uint32_t* placeholder {};
  uint32_t  count {};

  float height {};
  float width {};
  float spacing {};

  // constexpr TX_HIST_BUFF() noexcept = default;
  // TX_HIST_BUFF(TX_HIST_BUFF const&) = default;
  // TX_HIST_BUFF(TX_HIST_BUFF&&) = default;
  // TX_HIST_BUFF& operator=(TX_HIST_BUFF const& rhs) = default;
  // TX_HIST_BUFF& operator=(TX_HIST_BUFF&&) = default;
  //~TX_HIST_BUFF() = default;
};
