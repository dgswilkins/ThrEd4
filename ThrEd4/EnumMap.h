#pragma once

// Standard Libraries
#include <bitset>
#include <cstddef>

// struct for ensuring an enum has a count element.
// this does NOT validate that the EnumCount element is the last element
template <class T> struct has_enum_count {
	using yes = char;
	using no  = yes (&)[2];

	template <class U> static yes test(decltype(U::EnumCount)*);
	template <class U> static no  test(...);

	// SFINAE magic.
	static bool const value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

template <typename EnumType> class EnumMap
{
	static_assert(std::is_enum<EnumType>::value, "Type for EnumMap must be an Enum");
	static_assert(has_enum_count<EnumType>::value,
	              "Enum provided to EnumMap must have a \"EnumCount\" option as the last element in the enum.");

public:
	explicit constexpr EnumMap(unsigned i_val)
	    : mask_(i_val) {
	}
	inline bool test(const EnumType i_key) const {
		return mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	}
	inline void set() {
		mask_.set();
	}
	inline void set(const EnumType i_key, bool i_val = true) {
		mask_.set(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key), i_val);
	}
	inline bool testAndSet(const EnumType i_key, bool i_val = true) {
		const bool val = mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
		mask_.set(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key), i_val);
		return val;
	}
	inline void reset() noexcept {
		mask_.reset();
	}
	inline void reset(const EnumType i_key) {
		mask_.reset(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	}
	inline bool testAndReset(const EnumType i_key) {
		const bool val = mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
		mask_.reset(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
		return val;
	}
	inline void flip() {
		mask_.flip();
	}
	inline void flip(const EnumType i_key) {
		mask_.flip(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	}
	inline bool testAndFlip(const EnumType i_key) {
		const bool val = mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
		mask_.flip(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
		return val;
	}
	inline unsigned long to_ulong() {
		return mask_.to_ulong();
	}
	inline size_t count() const {
		return mask_.count();
	} // number of set bits
	inline size_t size() const {
		return mask_.size();
	} // number of bits in the entire set
	inline bool any() const {
		return mask_.any();
	}
	inline bool none() const {
		return mask_.none();
	}
	inline bool all() const {
		return mask_.all();
	}
	inline std::bitset<gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(EnumType::EnumCount)> mask() const {
		return mask_;
	}
	inline void mask(const std::bitset<gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(EnumType::EnumCount)>& i_mask) {
		mask_ = i_mask;
	}

	bool operator==(const EnumMap<EnumType>& i_other) const {
		return mask_ == i_other.mask_;
	}
	bool operator!=(const EnumMap<EnumType>& i_other) const {
		return mask_ != i_other.mask_;
	}

	EnumMap<EnumType> operator&=(const EnumMap<EnumType>& i_other) {
		mask_ &= i_other.mask_;
		return *this;
	}

	EnumMap<EnumType> operator|=(const EnumMap<EnumType>& i_other) {
		mask_ |= i_other.mask_;
		return *this;
	}

	EnumMap<EnumType> operator&(const EnumMap<EnumType>& i_other) const {
		EnumMap<EnumType> newMask;
		newMask.mask_ = mask_ & i_other.mask_;
		return newMask;
	}

	EnumMap<EnumType> operator|(const EnumMap<EnumType>& i_other) const {
		EnumMap<EnumType> newMask;
		newMask.mask_ = mask_ | i_other.mask_;
		return newMask;
	}

	EnumMap<EnumType> operator~() const {
		EnumMap<EnumType> newMask;
		newMask.mask_ = ~mask_;
		return newMask;
	}

private:
	std::bitset<gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(EnumType::EnumCount)> mask_;
};
