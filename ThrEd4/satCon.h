#ifndef THRED_SAT_CON_H
#define THRED_SAT_CON_H

// Open Source headers
#include "gsl/gsl"

// Standard Libraries
#include <cstdint>

class SAT_CON_OUT;

class SAT_CON
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint32_t start {};
  uint32_t finish {};
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  constexpr SAT_CON() noexcept = default;
  explicit constexpr SAT_CON(SAT_CON_OUT const& rhs) noexcept;
  explicit constexpr SAT_CON(uint32_t rStart, uint32_t rFinish) noexcept;

  inline auto operator=(SAT_CON_OUT const& rhs) noexcept -> SAT_CON&;
};

constexpr SAT_CON::SAT_CON(uint32_t const rStart, uint32_t const rFinish) noexcept :
    start(rStart), finish(rFinish) {
}

#pragma pack(push, 1)
class SAT_CON_OUT
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint16_t start {};
  uint16_t finish {};
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  explicit constexpr SAT_CON_OUT() noexcept = default;
  explicit constexpr SAT_CON_OUT(SAT_CON const& rhs);

  inline auto operator=(SAT_CON const& rhs) -> SAT_CON_OUT&;
};
#pragma pack(pop)

constexpr SAT_CON_OUT::SAT_CON_OUT(SAT_CON const& rhs) :
    start(gsl::narrow<decltype(start)>(rhs.start)), finish(gsl::narrow<decltype(finish)>(rhs.finish)) {
}

inline auto SAT_CON_OUT::operator=(SAT_CON const& rhs) -> SAT_CON_OUT& {
  start  = gsl::narrow<decltype(start)>(rhs.start);
  finish = gsl::narrow<decltype(finish)>(rhs.finish);

  return *this;
}

constexpr SAT_CON::SAT_CON(SAT_CON_OUT const& rhs) noexcept : start(rhs.start), finish(rhs.finish) {
}

inline auto SAT_CON::operator=(SAT_CON_OUT const& rhs) noexcept -> SAT_CON& {
  start  = rhs.start;
  finish = rhs.finish;

  return *this;
}

#endif // THRED_SAT_CON_H
