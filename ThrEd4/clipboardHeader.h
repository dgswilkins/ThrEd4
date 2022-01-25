#pragma once

// Standard Libraries
#include <cstdint>

// Open Source headers

// Local Headers
#include "formHeader.h"

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

class FORMS_CLIP // multiple forms clipboard header
{
  public:
  uint32_t clipType {};
  uint16_t formCount {};
  uint16_t reserved {};

  // constexpr FORMS_CLIP() noexcept = default;
  // FORMS_CLIP(FORMS_CLIP const&) = default;
  // FORMS_CLIP(FORMS_CLIP&&) = default;
  // FORMS_CLIP& operator=(FORMS_CLIP const& rhs) = default;
  // FORMS_CLIP& operator=(FORMS_CLIP&&) = default;
  //~FORMS_CLIP() = default;
};

class FORM_VERTEX_CLIP // form points clipboard header
{
  public:
  uint32_t clipType {};
  uint32_t vertexCount {};
  bool     direction {};

  // constexpr FORM_VERTEX_CLIP() noexcept = default;
  // FORM_VERTEX_CLIP(FORM_VERTEX_CLIP const&) = default;
  // FORM_VERTEX_CLIP(FORM_VERTEX_CLIP&&) = default;
  // FORM_VERTEX_CLIP& operator=(FORM_VERTEX_CLIP const& rhs) = default;
  // FORM_VERTEX_CLIP& operator=(FORM_VERTEX_CLIP&&) = default;
  //~FORM_VERTEX_CLIP() = default;
};
