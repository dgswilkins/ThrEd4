#pragma once

// Local Headers
//  ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/gsl"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cfloat>
#include <cstdint>

namespace util {
auto constexpr closeEnough(float first, float second) -> bool {
  // test if the floats are so close together that they can be considered equal
  auto const val = (first > second) ? (first - second) : (second - first);
  return (val < FLT_EPSILON);
}

auto constexpr closeEnough(double first, double second) -> bool {
  // test if the floats are so close together that they can be considered equal
  auto const val = (first > second) ? (first - second) : (second - first);
  return (val < DBL_EPSILON);
}
} // namespace util

class F_POINT;
class F_POINT_ATTR;
class SMAL_PNT_L;
class B_SEQ_PNT;

class D_POINT
{
  public:
  double x {};
  double y {};

  // constexpr D_POINT() noexcept = default;
  explicit inline D_POINT(F_POINT const& rhs) noexcept;
  // ToDo - Not sure why this suppression is required. CPPCheck bug?
  // cppcheck-suppress unknownMacro
  inline D_POINT(float rhsX, float rhsY) noexcept;
  // D_POINT(D_POINT const&) = default;
  // D_POINT(D_POINT&&) = default;
  // D_POINT& operator=(D_POINT const& rhs) = default;
  // D_POINT& operator=(D_POINT&&) = default;
  //~D_POINT() = default;
};

#pragma pack(push, 1)
class F_POINT
{
  public:
  float x {};
  float y {};

  constexpr F_POINT() noexcept = default;
  inline constexpr F_POINT(double rhsX, double rhsY) noexcept;
  inline constexpr F_POINT(float rhsX, float rhsY) noexcept;
  inline constexpr F_POINT(int32_t rhsX, int32_t rhsY) noexcept;
  inline constexpr F_POINT(LONG rhsX, LONG rhsY) noexcept;
  inline constexpr explicit F_POINT(D_POINT const& rhs) noexcept;
  inline constexpr auto operator==(F_POINT const& rhs) const noexcept -> bool;
  inline constexpr auto operator=(D_POINT const& rhs) noexcept -> F_POINT&;
  inline constexpr auto operator=(F_POINT_ATTR const& rhs) noexcept -> F_POINT&;
  inline constexpr auto operator=(SMAL_PNT_L const& rhs) noexcept -> F_POINT&;
  inline constexpr auto operator=(B_SEQ_PNT const& rhs) noexcept -> F_POINT&;
  // F_POINT(F_POINT const&) = default;
  // F_POINT(F_POINT&&) = default;
  // F_POINT& operator=(F_POINT const& rhs) = default;
  // F_POINT& operator=(F_POINT&&) = default;
  //~F_POINT() = default;
};
#pragma pack(pop)
#pragma pack(push, 1)
class F_POINT_ATTR
{
  public:
  float    x {};
  float    y {};
  uint32_t attribute {};

  constexpr F_POINT_ATTR() noexcept = default;
  inline F_POINT_ATTR(double rhsX, double rhsY, uint32_t rhsA) noexcept;
  inline F_POINT_ATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept;
  inline constexpr auto operator==(F_POINT_ATTR const& rhs) const noexcept -> bool;
  // F_POINT_ATTR(F_POINT_ATTR const&) = default;
  // F_POINT_ATTR(F_POINT_ATTR&&) = default;
  // F_POINT_ATTR& operator=(F_POINT_ATTR const& rhs) = default;
  // F_POINT_ATTR& operator=(F_POINT_ATTR&&) = default;
  //~F_POINT_ATTR() = default;
};
#pragma pack(pop)

class SMAL_PNT_L
{
  public:
  uint32_t line {}; // line and group must remain in this order for sort to work
  uint32_t group {};
  float    x {};
  float    y {};

  // constexpr SMAL_PNT_L() noexcept = default;
  // SMAL_PNT_L(SMAL_PNT_L const&) = default;
  // SMAL_PNT_L(SMAL_PNT_L&&) = default;
  // SMAL_PNT_L& operator=(SMAL_PNT_L const& rhs) = default;
  // SMAL_PNT_L& operator=(SMAL_PNT_L&&) = default;
  //~SMAL_PNT_L() = default;
};

class B_SEQ_PNT
{
  public:
  float  x {};
  float  y {};
  int8_t attribute {};

  inline B_SEQ_PNT() noexcept = default;
  inline B_SEQ_PNT(double rhsX, double rhsY, int32_t rhsAttr);
  inline B_SEQ_PNT(float rhsX, float rhsY, int32_t rhsAttr);
  // B_SEQ_PNT(B_SEQ_PNT const&) = default;
  // B_SEQ_PNT(B_SEQ_PNT&&) = default;
  // B_SEQ_PNT& operator=(B_SEQ_PNT const& rhs) = default;
  // B_SEQ_PNT& operator=(B_SEQ_PNT&&) = default;
  //~B_SEQ_PNT() = default;
};

inline B_SEQ_PNT::B_SEQ_PNT(double rhsX, double rhsY, int32_t rhsAttr) :
    x(gsl::narrow<float>(rhsX)), y(gsl::narrow<float>(rhsY)), attribute(gsl::narrow<int8_t>(rhsAttr)) {
}

inline B_SEQ_PNT::B_SEQ_PNT(float rhsX, float rhsY, int32_t rhsAttr) :
    x(rhsX), y(rhsY), attribute(gsl::narrow<int8_t>(rhsAttr)) {
}

inline constexpr auto F_POINT::operator==(F_POINT const& rhs) const noexcept -> bool {
  return util::closeEnough(x, rhs.x) && util::closeEnough(y, rhs.y);
}

inline constexpr F_POINT::F_POINT(float rhsX, float rhsY) noexcept : x(rhsX), y(rhsY) {
}

inline constexpr F_POINT::F_POINT(int32_t rhsX, int32_t rhsY) noexcept :
    x(gsl::narrow_cast<float>(rhsX)), y(gsl::narrow_cast<float>(rhsY)) {
}

inline constexpr F_POINT::F_POINT(LONG rhsX, LONG rhsY) noexcept :
    x(gsl::narrow_cast<float>(rhsX)), y(gsl::narrow_cast<float>(rhsY)) {
}

inline constexpr F_POINT::F_POINT(double rhsX, double rhsY) noexcept :
    x(gsl::narrow_cast<float>(rhsX)), y(gsl::narrow_cast<float>(rhsY)) {
}

inline constexpr F_POINT::F_POINT(D_POINT const& rhs) noexcept :
    x(gsl::narrow_cast<float>(rhs.x)), y(gsl::narrow_cast<float>(rhs.y)) {
}

inline constexpr auto F_POINT::operator=(D_POINT const& rhs) noexcept -> F_POINT& {
  x = gsl::narrow_cast<float>(rhs.x);
  y = gsl::narrow_cast<float>(rhs.y);
  return *this;
}

inline constexpr auto F_POINT::operator=(SMAL_PNT_L const& rhs) noexcept -> F_POINT& {
  x = gsl::narrow_cast<float>(rhs.x);
  y = gsl::narrow_cast<float>(rhs.y);
  return *this;
}

inline constexpr auto F_POINT::operator=(F_POINT_ATTR const& rhs) noexcept -> F_POINT& {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

inline constexpr auto F_POINT::operator=(B_SEQ_PNT const& rhs) noexcept -> F_POINT& {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

inline F_POINT_ATTR::F_POINT_ATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept :
    x(rhsX), y(rhsY), attribute(rhsA) {
}

inline F_POINT_ATTR::F_POINT_ATTR(double rhsX, double rhsY, uint32_t rhsA) noexcept :
    x(gsl::narrow_cast<float>(rhsX)), y(gsl::narrow_cast<float>(rhsY)), attribute(rhsA) {
}

inline constexpr auto F_POINT_ATTR::operator==(F_POINT_ATTR const& rhs) const noexcept -> bool {
  return util::closeEnough(x, rhs.x) && util::closeEnough(y, rhs.y) && attribute==rhs.attribute;
}

inline D_POINT::D_POINT(F_POINT const& rhs) noexcept :
    x(gsl::narrow_cast<double>(rhs.x)), y(gsl::narrow_cast<double>(rhs.y)) {
}

inline D_POINT::D_POINT(float rhsX, float rhsY) noexcept :
    x(gsl::narrow_cast<double>(rhsX)), y(gsl::narrow_cast<double>(rhsY)) {
}
