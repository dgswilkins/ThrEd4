#pragma once
#include <CppCoreCheck\warnings.h>
#pragma warning( push )  
#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)
#include <boost/dynamic_bitset.hpp>
#pragma warning( pop )  

template <typename Block = unsigned, typename Allocator = std::allocator<unsigned>>
class ExtendedBitSet : public boost::dynamic_bitset<Block, Allocator> {
	typedef boost::dynamic_bitset<Block, Allocator> super;
public:
	ExtendedBitSet (std::size_t bits) : super::dynamic_bitset(bits) {}

	/*
	bool testAndReset(std::size_t pos) const {
		bool val = super::test(pos);
		super::reset(pos);
		return val;
	}
	*/

	unsigned getFirst() {
		const unsigned foundBit = super::find_first();
		if (foundBit != super::npos) {
			super::reset(foundBit);
		}
		else {
			return 0xFFFFFFFF;
		}
		return foundBit;

	}

	unsigned getLast() {
		unsigned foundBit = super::find_first();
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

