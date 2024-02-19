#pragma once

// Open Source headers
#include "gsl/gsl"

// Standard Libraries
#include <cstdint>

class SAT_CON_OUT;

class SAT_CON
{
  public:
  uint32_t start {};
  uint32_t finish {};

  constexpr SAT_CON() noexcept = default;
  // SAT_CON(SAT_CON const&) = default;
  // SAT_CON(SAT_CON&&) = default;
  // SAT_CON& operator=(SAT_CON const& rhs) = default;
  // SAT_CON& operator=(SAT_CON&&) = default;
  //~SAT_CON() = default;

  explicit SAT_CON(SAT_CON_OUT const& rhs) noexcept;
  inline SAT_CON(uint32_t rStart, uint32_t rFinish) noexcept;
  inline auto operator=(SAT_CON_OUT const& rhs) noexcept -> SAT_CON&;
};

inline SAT_CON::SAT_CON(uint32_t rStart, uint32_t rFinish) noexcept :
    start(rStart), finish(rFinish) {
}

#pragma pack(push, 1)
class SAT_CON_OUT
{
  public:
  uint16_t start {};
  uint16_t finish {};

  constexpr SAT_CON_OUT() noexcept = default;
  // SAT_CON_OUT(SAT_CON_OUT const&) = default;
  // SAT_CON_OUT(SAT_CON_OUT&&) = default;
  // SAT_CON_OUT& operator=(SAT_CON_OUT const& rhs) = default;
  // SAT_CON_OUT& operator=(SAT_CON_OUT&&) = default;
  //~SAT_CON_OUT() = default;

  explicit SAT_CON_OUT(SAT_CON const& rhs);
  inline auto operator=(SAT_CON const& rhs) -> SAT_CON_OUT&;
};
#pragma pack(pop)

inline SAT_CON_OUT::SAT_CON_OUT(SAT_CON const& rhs) :
    start(gsl::narrow<decltype(start)>(rhs.start)), finish(gsl::narrow<decltype(finish)>(rhs.finish)) {
}

inline auto SAT_CON_OUT::operator=(SAT_CON const& rhs) -> SAT_CON_OUT& {
  start  = gsl::narrow<decltype(start)>(rhs.start);
  finish = gsl::narrow<decltype(finish)>(rhs.finish);

  return *this;
}

inline SAT_CON::SAT_CON(SAT_CON_OUT const& rhs) noexcept : start(rhs.start), finish(rhs.finish) {
}

inline auto SAT_CON::operator=(SAT_CON_OUT const& rhs) noexcept -> SAT_CON& {
  start  = rhs.start;
  finish = rhs.finish;

  return *this;
}
