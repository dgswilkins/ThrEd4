#pragma once

// Standard Libraries
#include <cstdint>

union FLENCNTOUT;

union FLENCNT {
  public:
  float    stitchLength;
  uint32_t clipCount {}; // number of points in fill clipboard data

  FLENCNT() noexcept = default;
  // FLENCNT(FLENCNT const&) = default;
  // FLENCNT(FLENCNT&&) = default;
  // FLENCNT& operator=(FLENCNT const& rhs) = default;
  // FLENCNT& operator=(FLENCNT&&) = default;
  //~FLENCNT() = default;

  explicit FLENCNT(FLENCNTOUT const& rhs) noexcept;
  inline auto operator=(FLENCNTOUT const& rhs) noexcept -> FLENCNT&;
};

#pragma pack(push, 1)
union FLENCNTOUT {
  public:
  float    stitchLength {};
  uint32_t clipCount;

  FLENCNTOUT() noexcept = default;
  // FLENCNTOUT(FLENCNTOUT const&) = default;
  // FLENCNTOUT(FLENCNTOUT&&) = default;
  // FLENCNTOUT& operator=(FLENCNTOUT const& rhs) = default;
  // FLENCNTOUT& operator=(FLENCNTOUT&&) = default;
  //~FLENCNTOUT() = default;

  explicit FLENCNTOUT(FLENCNT const& rhs) noexcept;
  inline auto operator=(FLENCNT const& rhs) noexcept -> FLENCNTOUT&;
};
#pragma pack(pop)

inline FLENCNTOUT::FLENCNTOUT(FLENCNT const& rhs) noexcept : clipCount(rhs.clipCount) {
}

inline auto FLENCNTOUT::operator=(FLENCNT const& rhs) noexcept -> FLENCNTOUT& {
  clipCount = rhs.clipCount;

  return *this;
}

inline FLENCNT::FLENCNT(FLENCNTOUT const& rhs) noexcept : clipCount(rhs.clipCount) {
}

inline auto FLENCNT::operator=(FLENCNTOUT const& rhs) noexcept -> FLENCNT& {
  clipCount = rhs.clipCount;

  return *this;
}
