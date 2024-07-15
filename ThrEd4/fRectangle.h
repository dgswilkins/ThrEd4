#pragma once

#pragma pack(push, 1)
class F_RECTANGLE
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  float left {};
  float top {};
  float right {};
  float bottom {};
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  explicit constexpr F_RECTANGLE() noexcept = default;
  explicit constexpr F_RECTANGLE(float rhsLeft, float rhsTop, float rhsRight, float rhsBottom) noexcept;
};
#pragma pack(pop)

constexpr F_RECTANGLE::F_RECTANGLE(float const rhsLeft, float const rhsTop, float const rhsRight, float const rhsBottom) noexcept :
    left(rhsLeft), top(rhsTop), right(rhsRight), bottom(rhsBottom) {
}
