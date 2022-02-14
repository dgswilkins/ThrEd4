#pragma once

// Local Headers
//  ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

// Standard Libraries
#include <type_traits>
#include <bitset>
#include <cstddef>

// struct for ensuring an enum has a count element.
// this does NOT validate that the EnumCount element is the last element
// default template:
template <class, class = void> struct hasEnumCount : std::false_type { };

// specialized as hasEnumCount< T , void > or sfinae
template <class T>
struct hasEnumCount<T, std::void_t<decltype(T::EnumCount)>> : std::is_same<T, decltype(T::EnumCount)> { };

template <typename EnumType> class ENUM_MAP
{
  static_assert(std::is_enum_v<EnumType>, "Type for EnumMap must be an Enum");
  static_assert(hasEnumCount<EnumType>::value, "Enum provided to EnumMap must have a \"EnumCount\" option as the last element in the enum.");

  public:
  constexpr explicit ENUM_MAP(uint32_t i_val) : m_mask(i_val) {
  }
  [[nodiscard]] inline auto test(EnumType const i_key) const -> bool {
	return m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
  }
  inline void set() {
	m_mask.set();
  }
  inline void set(EnumType const i_key, bool i_val = true) {
	m_mask.set(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key), i_val);
  }
  inline auto testAndSet(EnumType const i_key, bool i_val = true) -> bool {
	bool const val = m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	m_mask.set(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key), i_val);
	return val;
  }
  inline void reset() noexcept {
	m_mask.reset();
  }
  inline void reset(EnumType const i_key) {
	m_mask.reset(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
  }
  inline auto testAndReset(EnumType const i_key) -> bool {
	bool const val = m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	m_mask.reset(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	return val;
  }
  inline void flip() {
	m_mask.flip();
  }
  inline void flip(EnumType const i_key) {
	m_mask.flip(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
  }
  inline auto testAndFlip(EnumType const i_key) -> bool {
	bool const val = m_mask.test(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	m_mask.flip(gsl::narrow_cast<std::underlying_type_t<EnumType>>(i_key));
	return val;
  }
  inline auto to_ulong() -> uint32_t { // NOLINT(readability-identifier-naming)
	return m_mask.to_ulong();
  }
  [[nodiscard]] inline auto count() const -> size_t {
	return m_mask.count();
  } // number of set bits
  [[nodiscard]] inline auto size() const -> size_t {
	return m_mask.size();
  } // number of bits in the entire set
  [[nodiscard]] inline auto any() const -> bool {
	return m_mask.any();
  }
  [[nodiscard]] inline auto none() const -> bool {
	return m_mask.none();
  }
  [[nodiscard]] inline auto all() const -> bool {
	return m_mask.all();
  }
  [[nodiscard]] inline auto mask() const
      -> std::bitset<gsl::narrow_cast<std::underlying_type_t<EnumType>>(EnumType::EnumCount)> {
	return m_mask;
  }
  inline void mask(std::bitset<gsl::narrow_cast<std::underlying_type_t<EnumType>>(EnumType::EnumCount)> const& i_mask) {
	m_mask = i_mask;
  }

  auto operator==(ENUM_MAP<EnumType> const& i_other) const -> bool {
	return m_mask == i_other.m_mask;
  }
  auto operator!=(ENUM_MAP<EnumType> const& i_other) const -> bool {
	return m_mask != i_other.m_mask;
  }

  auto operator&=(ENUM_MAP<EnumType> const& i_other) -> ENUM_MAP<EnumType> {
	m_mask &= i_other.m_mask;
	return *this;
  }

  auto operator|=(ENUM_MAP<EnumType> const& i_other) -> ENUM_MAP<EnumType> {
	m_mask |= i_other.m_mask;
	return *this;
  }

  auto operator&(ENUM_MAP<EnumType> const& i_other) const -> ENUM_MAP<EnumType> {
	ENUM_MAP<EnumType> newMask;
	newMask.m_mask = m_mask & i_other.m_mask;
	return newMask;
  }

  auto operator|(ENUM_MAP<EnumType> const& i_other) const -> ENUM_MAP<EnumType> {
	ENUM_MAP<EnumType> newMask;
	newMask.m_mask = m_mask | i_other.m_mask;
	return newMask;
  }

  auto operator~() const -> ENUM_MAP<EnumType> {
	ENUM_MAP<EnumType> newMask;
	newMask.m_mask = ~m_mask;
	return newMask;
  }

  private:
  std::bitset<gsl::narrow_cast<std::underlying_type_t<EnumType>>(EnumType::EnumCount)> m_mask;
};
