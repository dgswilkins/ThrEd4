#pragma once

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26814)
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

template <typename Block = uint32_t, typename Allocator = std::allocator<uint32_t>>
class ExtendedBitSet : public boost::dynamic_bitset<Block, Allocator>
{
  using super = boost::dynamic_bitset<Block, Allocator>;

  public:
#pragma warning(suppress : 26434) // Function hides a non-virtual function  (c.128)
  explicit ExtendedBitSet(std::size_t bits) : super::dynamic_bitset(bits) {
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
	  return 0xFFFFFFFF;
	}
	return foundBit;
  }

  auto getLast() -> size_t {
	size_t foundBit = super::find_first();
	if (foundBit != super::npos) {
	  do {
		if (super::find_next(foundBit) != super::npos) {
		  foundBit = super::find_next(foundBit);
		}
	  } while (super::find_next(foundBit) != super::npos);
	}
	if (foundBit != super::npos) {
	  super::reset(foundBit);
	}
	else {
	  return 0xFFFFFFFF;
	}
	return foundBit;
  }
};
