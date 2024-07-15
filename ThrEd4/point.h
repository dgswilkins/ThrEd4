#pragma once

// Open Source headers
#include "gsl/gsl"

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
auto inline closeEnough(float first, float second) noexcept -> bool {
  // test if the floats are so close together that they can be considered equal
  return std::abs(first - second) < FLT_EPSILON;
}

auto inline closeEnough(double first, double second) noexcept -> bool {
  // test if the doubles are so close together that they can be considered equal
  return std::abs(first - second) < DBL_EPSILON;
}

auto constexpr doubleToFloat(double value) -> float {
  try {
	return gsl::narrow<float>(value);
  }
  catch (gsl::narrowing_error const& e) { // check if we are seeing a significant rounding error
	UNREFERENCED_PARAMETER(e);
	constexpr auto MINDBL = 4e-7; // small number for conversions
	auto const var  = gsl::narrow_cast<float>(value);
	if (auto const diff = abs(value - gsl::narrow_cast<double>(var)) / value; diff > MINDBL) {
	  throw std::runtime_error("conversion error above limit");
	}
	return var;
  }
  catch (...) { // otherwise throw
	throw std::runtime_error("gsl::narrow failed in wrap:toFloat");
  }
}

} // namespace util

class F_POINT;
class F_POINT_ATTR;
class SMAL_PNT_L;
class B_SEQ_PNT;

#ifdef NEED_DPOINT
class D_POINT
{
  public:
  // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
  double x {};
  double y {};
  // NOLINTEND

  explicit inline D_POINT(F_POINT const& rhs) noexcept;
  explicit inline D_POINT(float rhsX, float rhsY) noexcept;
  explicit D_POINT(double rhsX, double rhsY) noexcept : x(rhsX), y(rhsY) {
  }
};
#endif

#pragma pack(push, 1)
class F_POINT
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  float x {};
  float y {};
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  explicit constexpr F_POINT() noexcept = default;
  explicit constexpr F_POINT(double rhsX, double rhsY);
  explicit constexpr F_POINT(float rhsX, float rhsY) noexcept;
  explicit constexpr F_POINT(int32_t rhsX, int32_t rhsY) noexcept;
  explicit constexpr F_POINT(LONG rhsX, LONG rhsY) noexcept;
#ifdef NEED_DPOINT
  constexpr auto operator=(D_POINT const& rhs) -> F_POINT&;
  explicit constexpr F_POINT(D_POINT const& rhs);
#endif
  inline auto    operator==(F_POINT const& rhs) const noexcept -> bool;
  constexpr auto operator=(F_POINT_ATTR const& rhs) noexcept -> F_POINT&;
  constexpr auto operator=(SMAL_PNT_L const& rhs) noexcept -> F_POINT&;
  constexpr auto operator=(B_SEQ_PNT const& rhs) noexcept -> F_POINT&;

  inline auto operator+(F_POINT_ATTR const& rhs) const noexcept -> F_POINT;

  auto operator+(F_POINT const& rhs) const noexcept -> F_POINT {
	return F_POINT {x + rhs.x, y + rhs.y};
  }

  auto operator-(F_POINT const& rhs) const noexcept -> F_POINT {
	return F_POINT {x - rhs.x, y - rhs.y};
  }

  auto operator-() const noexcept -> F_POINT {
	return F_POINT {-x, -y};
  }

  constexpr auto operator+=(F_POINT const& rhs) noexcept -> F_POINT& {
	x += rhs.x;
	y += rhs.y;
	return *this;
  }

  constexpr auto operator-=(F_POINT const& rhs) noexcept -> F_POINT& {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
  }

  constexpr auto operator*=(float rhs) noexcept -> F_POINT& {
	x *= rhs;
	y *= rhs;
	return *this;
  }

  constexpr auto operator*(float rhs) const noexcept -> F_POINT {
	return F_POINT {x * rhs, y * rhs};
  }

  constexpr auto operator*(F_POINT rhs) const noexcept -> F_POINT {
	return F_POINT {x * rhs.x, y * rhs.y};
  }

  constexpr auto operator/(float rhs) const noexcept -> F_POINT {
	return F_POINT {x / rhs, y / rhs};
  }

  constexpr auto operator/(F_POINT rhs) const noexcept -> F_POINT {
	return F_POINT {x / rhs.x, y / rhs.y};
  }
};
#pragma pack(pop)

#pragma pack(push, 1)
class F_POINT_ATTR
{
  public:
  // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
  float    x {};
  float    y {};
  uint32_t attribute {};
  // NOLINTEND

  explicit constexpr F_POINT_ATTR() noexcept = default;
  explicit inline F_POINT_ATTR(double rhsX, double rhsY, uint32_t rhsA);
  explicit inline F_POINT_ATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept;

  inline auto operator==(F_POINT_ATTR const& rhs) const noexcept -> bool;

  constexpr auto operator=(F_POINT const& rhs) noexcept -> F_POINT_ATTR& {
	x = rhs.x;
	y = rhs.y;
	return *this;
  }

  constexpr auto operator+=(F_POINT const& rhs) noexcept -> F_POINT_ATTR& {
	x += rhs.x;
	y += rhs.y;
	return *this;
  }

  constexpr auto operator-=(F_POINT const& rhs) noexcept -> F_POINT_ATTR& {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
  }
};
#pragma pack(pop)

inline auto F_POINT::operator+(F_POINT_ATTR const& rhs) const noexcept -> F_POINT {
  return F_POINT {x + rhs.x, y + rhs.y};
}

class SMAL_PNT_L
{
  public:
  // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
  uint32_t line {}; // line and group must remain in this order for sort to work
  uint32_t group {};
  float    x {};
  float    y {};
  // NOLINTEND

  explicit constexpr SMAL_PNT_L() noexcept = default;
  explicit constexpr SMAL_PNT_L(uint32_t rhsLine, uint32_t rhsGroup, float rhsX, float rhsY) noexcept;
};

constexpr SMAL_PNT_L::SMAL_PNT_L(uint32_t rhsLine, uint32_t rhsGroup, float rhsX, float rhsY) noexcept :
    line(rhsLine), group(rhsGroup), x(rhsX), y(rhsY) {
}

class B_SEQ_PNT
{
  public:
  // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
  float  x {};
  float  y {};
  int8_t attribute {};
  // NOLINTEND

  explicit constexpr B_SEQ_PNT() noexcept = default;
  explicit constexpr B_SEQ_PNT(double rhsX, double rhsY, int32_t rhsAttr);
  explicit constexpr B_SEQ_PNT(float rhsX, float rhsY, int32_t rhsAttr);
};

// ToDo - This should use wrap::toFloat but there would be a circular dependency
constexpr B_SEQ_PNT::B_SEQ_PNT(double rhsX, double rhsY, int32_t rhsAttr) :
    x(util::doubleToFloat(rhsX)), y(util::doubleToFloat(rhsY)), attribute(gsl::narrow<int8_t>(rhsAttr)) {
}

constexpr B_SEQ_PNT::B_SEQ_PNT(float rhsX, float rhsY, int32_t rhsAttr) :
    x(rhsX), y(rhsY), attribute(gsl::narrow<int8_t>(rhsAttr)) {
}

inline auto F_POINT::operator==(F_POINT const& rhs) const noexcept -> bool {
  return util::closeEnough(x, rhs.x) && util::closeEnough(y, rhs.y);
}

constexpr F_POINT::F_POINT(float rhsX, float rhsY) noexcept : x(rhsX), y(rhsY) {
}

constexpr F_POINT::F_POINT(int32_t rhsX, int32_t rhsY) noexcept :
    x(gsl::narrow_cast<float>(rhsX)), y(gsl::narrow_cast<float>(rhsY)) {
}

constexpr F_POINT::F_POINT(LONG rhsX, LONG rhsY) noexcept :
    x(gsl::narrow_cast<float>(rhsX)), y(gsl::narrow_cast<float>(rhsY)) {
}

constexpr F_POINT::F_POINT(double rhsX, double rhsY) :
    x(util::doubleToFloat(rhsX)), y(util::doubleToFloat(rhsY)) {
}

#ifdef NEED_DPOINT
constexpr F_POINT::F_POINT(D_POINT const& rhs) :
    x(util::doubleToFloat(rhs.x)), y(util::doubleToFloat(rhs.y)) {
}

constexpr auto F_POINT::operator=(D_POINT const& rhs) -> F_POINT& {
  x = util::doubleToFloat(rhs.x);
  y = util::doubleToFloat(rhs.y);
  return *this;
}
#endif

constexpr auto F_POINT::operator=(SMAL_PNT_L const& rhs) noexcept -> F_POINT& {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

constexpr auto F_POINT::operator=(F_POINT_ATTR const& rhs) noexcept -> F_POINT& {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

constexpr auto F_POINT::operator=(B_SEQ_PNT const& rhs) noexcept -> F_POINT& {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

inline F_POINT_ATTR::F_POINT_ATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept :
    x(rhsX), y(rhsY), attribute(rhsA) {
}

inline F_POINT_ATTR::F_POINT_ATTR(double rhsX, double rhsY, uint32_t rhsA) :
    x(util::doubleToFloat(rhsX)), y(util::doubleToFloat(rhsY)), attribute(rhsA) {
}

inline auto F_POINT_ATTR::operator==(F_POINT_ATTR const& rhs) const noexcept -> bool {
  return util::closeEnough(x, rhs.x) && util::closeEnough(y, rhs.y) && attribute == rhs.attribute;
}

#ifdef NEED_DPOINT
inline D_POINT::D_POINT(F_POINT const& rhs) noexcept :
    x(gsl::narrow_cast<double>(rhs.x)), y(gsl::narrow_cast<double>(rhs.y)) {
}

inline D_POINT::D_POINT(float rhsX, float rhsY) noexcept :
    x(gsl::narrow_cast<double>(rhsX)), y(gsl::narrow_cast<double>(rhsY)) {
}
#endif
