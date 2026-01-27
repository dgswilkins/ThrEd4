#ifndef THRED_ENUM_MAP_H
#define THRED_ENUM_MAP_H

// Open Source headers
#include "gsl/gsl"

// Standard Libraries
#include <type_traits>
#include <bitset>

// struct for ensuring an enum has a count element.
// this does NOT validate that the EnumCount element is the last element
// default template:
template <class, class = void> struct hasEnumCount : std::false_type {
};

// specialized as hasEnumCount< T , void > or sfinae
template <class T>
struct hasEnumCount<T, std::void_t<decltype(T::EnumCount)>> : std::is_same<T, decltype(T::EnumCount)> {
};

template <typename EnumType> class ENUM_MAP
{
  static_assert(std::is_enum_v<EnumType>, "Type for EnumMap must be an Enum");
  static_assert(hasEnumCount<EnumType>::value, "Enum provided to EnumMap must have a \"EnumCount\" option as the last element in the enum.");

  public:
  constexpr explicit ENUM_MAP(uint32_t i_val) : m_mask(i_val) {
  }
  [[nodiscard]] auto test(EnumType const i_key) const -> bool {
	return m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
  }
  void set() {
	m_mask.set();
  }
  void set(EnumType const i_key, bool i_val = true) {
	m_mask.set(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key), i_val);
  }
  auto testAndSet(EnumType const i_key, bool i_val = true) -> bool {
	bool const val = m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	m_mask.set(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key), i_val);
	return val;
  }
  void reset() noexcept {
	m_mask.reset();
  }
  void reset(EnumType const i_key) {
	m_mask.reset(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
  }
  auto testAndReset(EnumType const i_key) -> bool {
	bool const val = m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	m_mask.reset(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	return val;
  }
  void flip() {
	m_mask.flip();
  }
  void flip(EnumType const i_key) {
	m_mask.flip(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
  }
  auto testAndFlip(EnumType const i_key) -> bool {
	bool const val = m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	m_mask.flip(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	return val;
  }
  auto to_ulong() noexcept -> uint32_t { // NOLINT(readability-identifier-naming)
	return m_mask.to_ulong();
  }
  [[nodiscard]] auto count() const -> size_t {
	return m_mask.count();
  } // number of set bits
  [[nodiscard]] auto size() const -> size_t {
	return m_mask.size();
  } // number of bits in the entire set
  [[nodiscard]] auto any() const -> bool {
	return m_mask.any();
  }
  [[nodiscard]] auto none() const -> bool {
	return m_mask.none();
  }
  [[nodiscard]] auto all() const -> bool {
	return m_mask.all();
  }
  [[nodiscard]] auto mask() const
      -> std::bitset<gsl::narrow_cast<std::underlying_type_t<EnumType>>(EnumType::EnumCount)> {
	return m_mask;
  }
  void mask(std::bitset<gsl::narrow_cast<std::underlying_type_t<EnumType>>(EnumType::EnumCount)> const& i_mask) {
	m_mask = i_mask;
  }

  auto operator==(ENUM_MAP const& i_other) const -> bool {
	return m_mask == i_other.m_mask;
  }
  auto operator!=(ENUM_MAP const& i_other) const -> bool {
	return m_mask != i_other.m_mask;
  }

  auto operator&=(ENUM_MAP const& i_other) -> ENUM_MAP {
	m_mask &= i_other.m_mask;
	return *this;
  }

  auto operator|=(ENUM_MAP const& i_other) -> ENUM_MAP {
	m_mask |= i_other.m_mask;
	return *this;
  }

  auto operator&(ENUM_MAP const& i_other) const -> ENUM_MAP {
	ENUM_MAP newMask;
	newMask.m_mask = m_mask & i_other.m_mask;
	return newMask;
  }

  auto operator|(ENUM_MAP const& i_other) const -> ENUM_MAP {
	ENUM_MAP newMask;
	newMask.m_mask = m_mask | i_other.m_mask;
	return newMask;
  }

  auto operator~() const -> ENUM_MAP {
	ENUM_MAP newMask;
	newMask.m_mask = ~m_mask;
	return newMask;
  }

  private:
  std::bitset<gsl::narrow_cast<std::underlying_type_t<EnumType>>(EnumType::EnumCount)> m_mask;
};

#endif // THRED_ENUM_MAP_H
