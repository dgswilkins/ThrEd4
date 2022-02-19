#pragma once

// Local Headers
#include "formHeader.h"

// Standard Libraries
#include <cstdint>

#pragma pack(push, 1)
class FORM_CLIP // form data clipboard header
{
  public:
  uint32_t clipType {};
  uint32_t reserved {};
  FRM_HEAD form {};

  // constexpr FORM_CLIP() noexcept = default;
  // FORM_CLIP(FORM_CLIP const&) = default;
  // FORM_CLIP(FORM_CLIP&&) = default;
  // FORM_CLIP& operator=(FORM_CLIP const& rhs) = default;
  // FORM_CLIP& operator=(FORM_CLIP&&) = default;
  //~FORM_CLIP() = default;
};
#pragma pack(pop)
