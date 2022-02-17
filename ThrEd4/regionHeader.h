#pragma once

// Standard Libraries
#include <cstdint>

class REGION // region for sequencing vertical fills
{
  public:
  uint32_t start {};       // start line of region
  uint32_t end {};         // end line of region
  uint32_t regionBreak {}; // ToDo - Is this member needed?
  uint32_t breakCount {};

  // constexpr REGION() noexcept = default;
  inline REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept;
  // REGION(REGION const&) = default;
  // REGION(REGION&&) = default;
  // REGION& operator=(REGION const& rhs) = default;
  // REGION& operator=(REGION&&) = default;
  //~REGION() = default;
};

inline REGION::REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept :
    start(rhsStart), end(rhsEnd), regionBreak(rhsBreak), breakCount(rhsCount) {
}

class R_CON // PathMap: path map for sequencing
{
  public:
  uint32_t node {};
  bool     isConnected {};
  uint32_t nextGroup {};

  // constexpr R_CON() noexcept = default;
  // R_CON(R_CON const&) = default;
  // R_CON(R_CON&&) = default;
  // R_CON& operator=(R_CON const& rhs) = default;
  // R_CON& operator=(R_CON&&) = default;
  //~R_CON() = default;
};

class RG_SEQ // TempPath: temporary path connections
{
  public:
  uint32_t pcon {}; // pointer to PathMap entry
  int32_t  count {};
  bool     skp {}; // path not found

  // constexpr RG_SEQ() noexcept = default;
  // RG_SEQ(RG_SEQ const&) = default;
  // RG_SEQ(RG_SEQ&&) = default;
  // RG_SEQ& operator=(RG_SEQ const& rhs) = default;
  // RG_SEQ& operator=(RG_SEQ&&) = default;
  //~RG_SEQ() = default;
};

class F_SEQ // Forward SequencePath: path of sequenced regions
{
  public:
  uint16_t node {};
  uint16_t nextGroup {};
  bool     skp {}; // path not found

  // constexpr F_SEQ() noexcept = default;
  // F_SEQ(F_SEQ const&) = default;
  // F_SEQ(F_SEQ&&) = default;
  // F_SEQ& operator=(F_SEQ const& rhs) = default;
  // F_SEQ& operator=(F_SEQ&&) = default;
  //~F_SEQ() = default;
};
