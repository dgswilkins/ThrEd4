#pragma once

#pragma pack(push, 1)
class F_RECTANGLE
{
  public:
  float left {};
  float top {};
  float right {};
  float bottom {};

  constexpr F_RECTANGLE() noexcept = default;
  // F_RECTANGLE(F_RECTANGLE const&) = default;
  // F_RECTANGLE(F_RECTANGLE&&) = default;
  // F_RECTANGLE& operator=(F_RECTANGLE const& rhs) = default;
  // F_RECTANGLE& operator=(F_RECTANGLE&&) = default;
  //~F_RECTANGLE() = default;

  inline F_RECTANGLE(float rhsLeft, float rhsTop, float rhsRight, float rhsBottom) noexcept;
};
#pragma pack(pop)

inline F_RECTANGLE::F_RECTANGLE(float rhsLeft, float rhsTop, float rhsRight, float rhsBottom) noexcept :
    left(rhsLeft), top(rhsTop), right(rhsRight), bottom(rhsBottom) {
}
