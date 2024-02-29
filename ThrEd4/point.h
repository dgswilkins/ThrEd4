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
auto constexpr closeEnough(float first, float second) -> bool {
  // test if the floats are so close together that they can be considered equal
  auto const val = (first > second) ? (first - second) : (second - first);
  return (val < FLT_EPSILON);
}

auto constexpr closeEnough(double first, double second) -> bool {
  // test if the doubles are so close together that they can be considered equal
  auto const val = (first > second) ? (first - second) : (second - first);
  return (val < DBL_EPSILON);
}

auto constexpr doubleToFloat(double value) -> float {
  constexpr auto MINDBL = 4e-5; // small number for conversions

  try {
	return gsl::narrow<float>(value);
  }
  catch (gsl::narrowing_error const& e) { // check if we are seeing a significant rounding error
	UNREFERENCED_PARAMETER(e);
	auto const var  = gsl::narrow_cast<float>(value);
	auto const diff = abs(value - gsl::narrow_cast<double>(var));
	if (diff > MINDBL) {
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

class D_POINT
{
  private:
  double m_xCoordinate {};
  double m_yCoordinate {};

  public:
  // constexpr D_POINT() noexcept = default;
  explicit inline D_POINT(F_POINT const& rhs) noexcept;
  inline D_POINT(double rhsX, double rhsY) noexcept : m_xCoordinate(rhsX), m_yCoordinate(rhsY) {}
  // ToDo - Not sure why this suppression is required. CPPCheck bug?
  // cppcheck-suppress unknownMacro
  inline D_POINT(float rhsX, float rhsY) noexcept;
  // D_POINT(D_POINT const&) = default;
  // D_POINT(D_POINT&&) = default;
  // D_POINT& operator=(D_POINT const& rhs) = default;
  // D_POINT& operator=(D_POINT&&) = default;
  //~D_POINT() = default;

  // Getter for x
  [[nodiscard]] auto getX() const noexcept -> double {
	return m_xCoordinate;
  }

  // Getter for y
  [[nodiscard]] auto getY() const noexcept -> double {
	return m_yCoordinate;
  }
};

#pragma pack(push, 1)
class F_POINT
{
  public:
  float x {};
  float y {};

  constexpr F_POINT() noexcept = default;
  inline constexpr F_POINT(double rhsX, double rhsY);
  inline constexpr F_POINT(float rhsX, float rhsY) noexcept;
  inline constexpr F_POINT(int32_t rhsX, int32_t rhsY) noexcept;
  inline constexpr F_POINT(LONG rhsX, LONG rhsY) noexcept;
  inline constexpr explicit F_POINT(D_POINT const& rhs);
  inline constexpr auto operator==(F_POINT const& rhs) const noexcept -> bool;
  inline constexpr auto operator=(D_POINT const& rhs) -> F_POINT&;
  inline constexpr auto operator=(F_POINT_ATTR const& rhs) noexcept -> F_POINT&;
  inline constexpr auto operator=(SMAL_PNT_L const& rhs) noexcept -> F_POINT&;
  inline constexpr auto operator=(B_SEQ_PNT const& rhs) noexcept -> F_POINT&;
  // F_POINT(F_POINT const&) = default;
  // F_POINT(F_POINT&&) = default;
  // F_POINT& operator=(F_POINT const& rhs) = default;
  // F_POINT& operator=(F_POINT&&) = default;
  //~F_POINT() = default;

  inline auto operator+(F_POINT_ATTR const& rhs) const noexcept -> F_POINT;

  inline auto operator+(F_POINT const& rhs) const noexcept -> F_POINT {
	return F_POINT {x + rhs.x, y + rhs.y};
  }

  inline auto operator-(F_POINT const& rhs) const noexcept -> F_POINT {
	return F_POINT {x - rhs.x, y - rhs.y};
  }

  inline auto operator-() const noexcept -> F_POINT {
	return F_POINT {-x, -y};
  }

  inline constexpr auto operator+=(F_POINT const& rhs) noexcept -> F_POINT& {
	x += rhs.x;
	y += rhs.y;
	return *this;
  }

  inline constexpr auto operator-=(F_POINT const& rhs) noexcept -> F_POINT& {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
  }

  inline constexpr auto operator*=(float rhs) noexcept -> F_POINT& {
	x *= rhs;
	y *= rhs;
	return *this;
  }
  
  inline constexpr auto operator*(float rhs) const noexcept -> F_POINT {
	return F_POINT {x * rhs, y * rhs};
  }

  inline constexpr auto operator*(F_POINT rhs) const noexcept -> F_POINT {
	return F_POINT {x * rhs.x, y * rhs.y};
  }

  inline constexpr auto operator/(float rhs) const noexcept -> F_POINT {
	return F_POINT {x / rhs, y / rhs};
  }

  inline constexpr auto operator/(F_POINT rhs) const noexcept -> F_POINT {
	return F_POINT {x / rhs.x, y / rhs.y};
  }
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
  inline F_POINT_ATTR(double rhsX, double rhsY, uint32_t rhsA);
  inline F_POINT_ATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept;
  // F_POINT_ATTR(F_POINT_ATTR const&) = default;
  // F_POINT_ATTR(F_POINT_ATTR&&) = default;
  // F_POINT_ATTR& operator=(F_POINT_ATTR const& rhs) = default;
  // F_POINT_ATTR& operator=(F_POINT_ATTR&&) = default;
  //~F_POINT_ATTR() = default;
  inline constexpr auto operator==(F_POINT_ATTR const& rhs) const noexcept -> bool;
};
#pragma pack(pop)

inline auto F_POINT::operator+(F_POINT_ATTR const& rhs) const noexcept -> F_POINT {
  return F_POINT {x + rhs.x, y + rhs.y};
}

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

// ToDo - This should use wrap::toFloat but there would be a circular dependency
inline B_SEQ_PNT::B_SEQ_PNT(double rhsX, double rhsY, int32_t rhsAttr) :
    x(util::doubleToFloat(rhsX)), y(util::doubleToFloat(rhsY)), attribute(gsl::narrow<int8_t>(rhsAttr)) {
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

inline constexpr F_POINT::F_POINT(double rhsX, double rhsY) :
    x(util::doubleToFloat(rhsX)), y(util::doubleToFloat(rhsY)) {
}

inline constexpr F_POINT::F_POINT(D_POINT const& rhs) :
    x(util::doubleToFloat(rhs.getX())), y(util::doubleToFloat(rhs.getY())) {
}

inline constexpr auto F_POINT::operator=(D_POINT const& rhs) -> F_POINT& {
  x = util::doubleToFloat(rhs.getX());
  y = util::doubleToFloat(rhs.getY());
  return *this;
}

inline constexpr auto F_POINT::operator=(SMAL_PNT_L const& rhs) noexcept -> F_POINT& {
  x = rhs.x;
  y = rhs.y;
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

inline F_POINT_ATTR::F_POINT_ATTR(double rhsX, double rhsY, uint32_t rhsA) :
    x(util::doubleToFloat(rhsX)), y(util::doubleToFloat(rhsY)), attribute(rhsA) {
}

inline constexpr auto F_POINT_ATTR::operator==(F_POINT_ATTR const& rhs) const noexcept -> bool {
  return util::closeEnough(x, rhs.x) && util::closeEnough(y, rhs.y) && attribute == rhs.attribute;
}

inline D_POINT::D_POINT(F_POINT const& rhs) noexcept :
    m_xCoordinate(gsl::narrow_cast<double>(rhs.x)), m_yCoordinate(gsl::narrow_cast<double>(rhs.y)) {
}

inline D_POINT::D_POINT(float rhsX, float rhsY) noexcept :
    m_xCoordinate(gsl::narrow_cast<double>(rhsX)), m_yCoordinate(gsl::narrow_cast<double>(rhsY)) {
}
