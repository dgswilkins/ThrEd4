#pragma once

// Open Source headers
#include "boost/dynamic_bitset.hpp"

template <typename Block = uint32_t, typename Allocator = std::allocator<uint32_t>>
class BIT_SET_EX : public boost::dynamic_bitset<Block, Allocator>
{
  using super = boost::dynamic_bitset<Block, Allocator>;

  public:
#pragma warning(suppress : 26434) // Function hides a non-virtual function  (c.128)
  explicit BIT_SET_EX(std::size_t bits) : super::dynamic_bitset(bits) {
  }

  /*
  bool testAndReset(std::size_t pos) const {
      bool val = super::test(pos);
      super::reset(pos);
      return val;
  }
  */

  auto getFirst() -> size_t {
	size_t const foundBit = super::find_first();
	if (foundBit != super::npos) {
	  super::reset(foundBit);
	}
	else {
	  return BIGSIZE;
	}
	return foundBit;
  }

  auto getLast() -> size_t {
	size_t foundBit = super::find_first();
	if (foundBit != super::npos) {
	  auto nextFoundBit = super::find_next(foundBit);
	  while (nextFoundBit != super::npos) {
		foundBit     = nextFoundBit;
		nextFoundBit = super::find_next(foundBit);
	  }
	}
	if (foundBit != super::npos) {
	  super::reset(foundBit);
	}
	else {
	  return BIGSIZE;
	}
	return foundBit;
  }
};
