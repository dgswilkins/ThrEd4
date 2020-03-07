#pragma once

// Standard Libraries
#include <bitset>
#include <cstddef>

// struct for ensuring an enum has a count element.
// this does NOT validate that the EnumCount element is the last element
template <class T> struct has_enum_count {
  using yes = char;
  using no  = yes (&)[2]; // NOLINT(modernize-avoid-c-arrays)

  template <class U> static auto test(decltype(U::EnumCount)*) -> yes;
  template <class U> static auto  test(...) -> no;

  // SFINAE magic.
  static bool const value = sizeof(test<T>(nullptr)) == sizeof(yes); // NOLINT
};

template <typename EnumType> class EnumMap
{
  static_assert(std::is_enum<EnumType>::value, "Type for EnumMap must be an Enum");
  static_assert(has_enum_count<EnumType>::value, "Enum provided to EnumMap must have a \"EnumCount\" option as the last element in the enum.");

  public:
  explicit constexpr EnumMap(uint32_t i_val) : mask_(i_val) {
  }
  [[nodiscard]] inline auto test(EnumType const i_key) const -> bool {
	return mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
  }
  inline void set() {
	mask_.set();
  }
  inline void set(EnumType const i_key, bool i_val = true) {
	mask_.set(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key), i_val);
  }
  inline auto testAndSet(EnumType const i_key, bool i_val = true) -> bool {
	bool const val = mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	mask_.set(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key), i_val);
	return val;
  }
  inline void reset() noexcept {
	mask_.reset();
  }
  inline void reset(EnumType const i_key) {
	mask_.reset(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
  }
  inline auto testAndReset(EnumType const i_key) -> bool {
	bool const val = mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	mask_.reset(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	return val;
  }
  inline void flip() {
	mask_.flip();
  }
  inline void flip(EnumType const i_key) {
	mask_.flip(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
  }
  inline auto testAndFlip(EnumType const i_key) -> bool {
	bool const val = mask_.test(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	mask_.flip(gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(i_key));
	return val;
  }
  inline auto to_ulong() -> uint32_t {
	return mask_.to_ulong();
  }
  [[nodiscard]] inline auto count() const -> size_t {
	return mask_.count();
  } // number of set bits
  [[nodiscard]] inline auto size() const -> size_t {
	return mask_.size();
  } // number of bits in the entire set
  [[nodiscard]] inline auto any() const -> bool {
	return mask_.any();
  }
  [[nodiscard]] inline auto none() const -> bool {
	return mask_.none();
  }
  [[nodiscard]] inline auto all() const -> bool {
	return mask_.all();
  }
  [[nodiscard]] inline auto mask() const -> std::bitset<gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(EnumType::EnumCount)> {
	return mask_;
  }
  inline void
  mask(std::bitset<gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(EnumType::EnumCount)> const& i_mask) {
	mask_ = i_mask;
  }

  auto operator==(EnumMap<EnumType> const& i_other) const -> bool {
	return mask_ == i_other.mask_;
  }
  auto operator!=(EnumMap<EnumType> const& i_other) const -> bool {
	return mask_ != i_other.mask_;
  }

  auto operator&=(EnumMap<EnumType> const& i_other) -> EnumMap<EnumType> {
	mask_ &= i_other.mask_;
	return *this;
  }

  auto operator|=(EnumMap<EnumType> const& i_other) -> EnumMap<EnumType> {
	mask_ |= i_other.mask_;
	return *this;
  }

  auto operator&(EnumMap<EnumType> const& i_other) const -> EnumMap<EnumType> {
	EnumMap<EnumType> newMask;
	newMask.mask_ = mask_ & i_other.mask_;
	return newMask;
  }

  auto operator|(EnumMap<EnumType> const& i_other) const -> EnumMap<EnumType> {
	EnumMap<EnumType> newMask;
	newMask.mask_ = mask_ | i_other.mask_;
	return newMask;
  }

  auto operator~() const -> EnumMap<EnumType> {
	EnumMap<EnumType> newMask;
	newMask.mask_ = ~mask_;
	return newMask;
  }

  private:
  std::bitset<gsl::narrow_cast<typename std::underlying_type<EnumType>::type>(EnumType::EnumCount)> mask_;
};
