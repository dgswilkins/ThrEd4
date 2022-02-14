#pragma once

// Standard Libraries
#include <cstdint>

constexpr auto BADFLT = 1U;
constexpr auto BADCLP = 1U << 1U;
constexpr auto BADSAT = 1U << 2U;
constexpr auto BADTX  = 1U << 3U;

class BAD_COUNTS
{
  public:
  uint32_t attribute {};
  uint32_t flt {};
  uint32_t clip {};
  uint32_t guideCount {}; // ToDo - is this an accurate description?
  uint32_t tx {};

  // constexpr BAD_COUNTS() noexcept = default;
  // BAD_COUNTS(BAD_COUNTS const&) = default;
  // BAD_COUNTS(BAD_COUNTS&&) = default;
  // BAD_COUNTS& operator=(BAD_COUNTS const& rhs) = default;
  // BAD_COUNTS& operator=(BAD_COUNTS&&) = default;
  //~BAD_COUNTS() = default;
};
