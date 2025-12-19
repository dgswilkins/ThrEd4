#ifndef THRED_XT_H
#define THRED_XT_H

// Local Headers
#include "formHeader.h"

// Standard Libraries
#include <array>
#include <vector>

enum class FSI : uint8_t { // fill starts index values
  kApplique = 0,
  kFill = 1,
  kFeather = 2,
  kBorder = 3,
  kAppliqueColor = 4,
  kFillColor = 5,
  kFeatherColor = 6,
  kBorderColor = 7,
  kMaxValue = kBorderColor
};

template <class IndexType, class ValueType> class ENUM_ARRAY
{
  public:
  auto operator[](IndexType idx) noexcept -> ValueType& {
	return m_Array.at(static_cast<size_t>(idx));
  }

  auto operator[](IndexType idx) const noexcept -> const ValueType& {
	return m_Array.at(static_cast<size_t>(idx));
  }

  auto operator[](uint32_t idx) noexcept -> ValueType& {
	return m_Array.at(idx);
  }

  auto operator[](uint32_t idx) const noexcept -> const ValueType& {
	return m_Array.at(idx);
  }

  [[nodiscard]] auto size() const -> auto {
	return m_Size;
  }

  auto begin() noexcept -> auto {
	return m_Array.begin();
  }

  auto end() noexcept -> auto {
	return m_Array.end();
  }

  private:
  std::array<ValueType, static_cast<size_t>(IndexType::kMaxValue) + 1> m_Array = {};

  size_t m_Size = static_cast<size_t>(IndexType::kMaxValue) + 1;
}; 

using FillStartsDataType = ENUM_ARRAY<FSI, uint32_t>;

namespace xt {

void chgchk(uint8_t code);
void chgwrn();
void chkcwlk(uint32_t formIndex);
void chkund(uint32_t formIndex, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
void chkwlk(uint32_t formIndex);
void clrstch() noexcept;

#ifdef _DEBUG
void dmpat();
#endif

void duauxnam(fs::path& auxName);
void dubcol(uint8_t color);
void dubit(FRM_HEAD& form, uint32_t bit);
void dublen(float length);
void dubmax(float length);
void dubmin(float length);
void dubspac(float length);
void dufang(float angle);
void dufcol(uint8_t color);
void dufhi(float length);
void dufind(float indent);
void duflen(float length);
void dufmax(float length);
void dufmin(float length);
void dufspac(float spacing);
void dufwid(float length);
void dufxang(float angle);
void dundcol(uint8_t color);
void dushft();
void duspac(float spacing);
void dusulen(float length);
void fdelstch(uint32_t formIndex, FillStartsDataType& fillStartsData, uint32_t& fillStartsMap);
void fethr();
void fethrf(uint32_t formIndex);
void fsort();
void fthrfn(FRM_HEAD& form);
auto insid(FRM_HEAD const& form) -> std::vector<F_POINT>&;
void intlv(uint32_t formIndex, FillStartsDataType const& fillStartsData, uint32_t fillStartsMap);
void mvshft();
void notcwlk();
void notund();
void notwlk();
void nudsiz();
void selalfrm();
void setclpspac();
void setcwlk();
void setEdit(uint32_t code, StateFlag flag);
void setfchk();
void setfilend();
void setfilstrt();
void setuang();
void setulen();
void setund();
void setuspac();
void setwlk();
void setwlkind();
void srtcol();
} // namespace xt

#endif // THRED_XT_H
