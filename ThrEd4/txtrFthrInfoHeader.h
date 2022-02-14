#pragma once

// Standard Libraries
#include <cstdint>

#pragma pack(push, 1)
class FTHR_INFO
{
  public:
  uint8_t  fillType {};      // feather fill type
  uint8_t  upCount {};       // feather up count
  uint8_t  downCount {};     // feather down count
  uint8_t  color {};         // feather blend color
  float    ratio {};         // feather ratio
  float    minStitchSize {}; // feather floor
  uint16_t count {};         // feather fill psg granularity

  // FTHR_INFO() noexcept = default;
  // FTHR_INFO(FTHR_INFO const&) = default;
  // FTHR_INFO(FTHR_INFO&&) = default;
  // FTHR_INFO& operator=(FTHR_INFO const& rhs) = default;
  // FTHR_INFO& operator=(FTHR_INFO&&) = default;
  //~FTHR_INFO() = default;
};
#pragma pack(pop)
#pragma pack(push, 1)
class TXTR_INFO
{
  public:
  int16_t  lines {};
  uint16_t index {};
  uint16_t count {};
  float    height {};

  // TXTR_INFO() noexcept = default;
  // TXTR_INFO(TXTR_INFO const&) = default;
  // TXTR_INFO(TXTR_INFO&&) = default;
  // TXTR_INFO& operator=(TXTR_INFO const& rhs) = default;
  // TXTR_INFO& operator=(TXTR_INFO&&) = default;
  //~TXTR_INFO() = default;
};
#pragma pack(pop)
#pragma pack(push, 1)
union TF_INFO {
  public:
  FTHR_INFO feather;
  TXTR_INFO texture {};

  // TF_INFO() noexcept = default;
  // TF_INFO(TF_INFO const&) = default;
  // TF_INFO(TF_INFO&&) = default;
  // TF_INFO& operator=(TF_INFO const& rhs) = default;
  // TF_INFO& operator=(TF_INFO&&) = default;
  //~TF_INFO() = default;
};
#pragma pack(pop)
