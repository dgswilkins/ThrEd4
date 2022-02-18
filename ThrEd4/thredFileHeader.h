#pragma once

// Standard Libraries
#include <cstdint>

constexpr auto RES_SIZE = 26; // reserved for expansion in the ThrEd v1.0 header
constexpr auto NAME_LEN = 50; // Length of the name fields in ThrEd headers

#pragma pack(push, 1)
class THR_HEAD // thred file header
{
  public:
  uint32_t headerType {};
  uint32_t fileLength {};    // length of strhed + length of stitch data
  uint16_t stitchCount {};   // number of stitches
  uint16_t hoopType {};      // size of hoop
  uint16_t formCount {};     // number of forms
  uint16_t vertexLen {};     // points to form points
  uint16_t vertexCount {};   // number of form points
  uint16_t dlineLen {};      // points to dline data
  uint16_t dlineCount {};    // dline data count
  uint16_t clipDataLen {};   // points to clipboard data
  uint16_t clipDataCount {}; // clipboard data count

  // constexpr THR_HEAD() noexcept = default;
  // THR_HEAD(THR_HEAD const&) = default;
  // THR_HEAD(THR_HEAD&&) = default;
  // THR_HEAD& operator=(THR_HEAD const& rhs) = default;
  // THR_HEAD& operator=(THR_HEAD&&) = default;
  //~THR_HEAD() = default;
};
#pragma pack(pop)
#pragma pack(push, 1)
class THR_HEAD_EX // thred v1.0 file header extension
{
  public:
  float    hoopSizeX {};              // hoop size x dimension
  float    hoopSizeY {};              // hoop size y dimension
  float    stgran {};                 // stitches per millimeter
  char     creatorName[NAME_LEN] {};  // name of the file creator
  char     modifierName[NAME_LEN] {}; // name of last file modifier
  int8_t   auxFormat {};              // auxillary file format
  int8_t   stres {};                  // reserved
  uint32_t texturePointCount {};      // textured fill point count
  int8_t   res[RES_SIZE] {};          // reserved for expansion

  // constexpr THR_HEAD_EX() noexcept = default;
  // THR_HEAD_EX(THR_HEAD_EX const&) = default;
  // THR_HEAD_EX(THR_HEAD_EX&&) = default;
  // THR_HEAD_EX& operator=(THR_HEAD_EX const& rhs) = default;
  // THR_HEAD_EX& operator=(THR_HEAD_EX&&) = default;
  //~THR_HEAD_EX() = default;
};
#pragma pack(pop)
