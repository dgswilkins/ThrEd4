#pragma once

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

template <typename Block = uint32_t, typename Allocator = std::allocator<uint32_t>>
class ExtendedBitSet : public boost::dynamic_bitset<Block, Allocator>
{
	using super = boost::dynamic_bitset<Block, Allocator>;

public:
	GSL_SUPPRESS(26434)
	explicit ExtendedBitSet(std::size_t bits)
	    : super::dynamic_bitset(bits) {
	}

	/*
	bool testAndReset(std::size_t pos) const {
	    bool val = super::test(pos);
	    super::reset(pos);
	    return val;
	}
	*/

	size_t getFirst() {
		const size_t foundBit = super::find_first();
		if (foundBit != super::npos) {
			super::reset(foundBit);
		}
		else {
			return 0xFFFFFFFF;
		}
		return foundBit;
	}

	size_t getLast() {
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
