#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cstdint>

union SATINANGLEOUT;

union SATINANGLE {
  public:
  uint32_t guide {};
  float    angle;

  SATINANGLE() noexcept = default;
  // SATINANGLE(SATINANGLE const&) = default;
  // SATINANGLE(SATINANGLE&&) = default;
  // SATINANGLE& operator=(SATINANGLE const& rhs) = default;
  // SATINANGLE& operator=(SATINANGLE&&) = default;
  //~SATINANGLE() = default;

  explicit SATINANGLE(SATINANGLEOUT const& rhs) noexcept;
  inline auto operator=(SATINANGLEOUT const& rhs) noexcept -> SATINANGLE&;
};

#pragma pack(push, 1)
union SATINANGLEOUT {
  public:
  DWORD guide {};
  float angle;

  SATINANGLEOUT() noexcept = default;
  // SATINANGLEOUT(SATINANGLEOUT const&) = default;
  // SATINANGLEOUT(SATINANGLEOUT&&) = default;
  // SATINANGLEOUT& operator=(SATINANGLEOUT const& rhs) = default;
  // SATINANGLEOUT& operator=(SATINANGLEOUT&&) = default;
  //~SATINANGLEOUT() = default;

  explicit SATINANGLEOUT(SATINANGLE const& rhs) noexcept;
  inline auto operator=(SATINANGLE const& rhs) noexcept -> SATINANGLEOUT&;
};
#pragma pack(pop)

inline SATINANGLEOUT::SATINANGLEOUT(SATINANGLE const& rhs) noexcept : angle(rhs.angle) {
}

inline auto SATINANGLEOUT::operator=(SATINANGLE const& rhs) noexcept -> SATINANGLEOUT& {
  angle = rhs.angle;

  return *this;
}

inline SATINANGLE::SATINANGLE(SATINANGLEOUT const& rhs) noexcept : angle(rhs.angle) {
}

inline auto SATINANGLE::operator=(SATINANGLEOUT const& rhs) noexcept -> SATINANGLE& {
  angle = rhs.angle;

  return *this;
}
