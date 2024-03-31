#pragma once

// Local Headers
#include "fRectangle.h"
#include "satCon.h"
#include "ThrEdTypes.h"
#include "wrappers.h"

// Standard Libraries
#include <cstdint>

#pragma pack(push, 1)
union FANGCLPOUT {
  private:
  float m_angle;
  DWORD m_clip {}; // clip pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
  SAT_CON_OUT m_guide;

  public:
  inline FANGCLPOUT() noexcept;
  // FANGCLPOUT(FANGCLPOUT const&) = default;
  // FANGCLPOUT(FANGCLPOUT&&) = default;
  // FANGCLPOUT& operator=(FANGCLPOUT const& rhs) = default;
  // FANGCLPOUT& operator=(FANGCLPOUT&&) = default;
  //~FANGCLPOUT() = default;

  // Getter and Setter for angle
  [[nodiscard]] auto getAngle() const noexcept -> float {
	return m_angle;
  }
  
  void setAngle(float value) noexcept {
	m_angle = value;
  }

  // Getter and Setter for clip
  [[nodiscard]] auto getClip() const noexcept -> DWORD {
	return m_clip;
  }
  
  void setClip(DWORD value) noexcept {
	m_clip = value;
  }

  // Getter and Setter for guide
  [[nodiscard]] auto getGuide() const noexcept -> SAT_CON_OUT {
	return m_guide;
  }

  void setGuide(SAT_CON_OUT value) noexcept {
	m_guide = value;
  }

  void setGuide(SAT_CON value) {
	m_guide = value;
  }
};
#pragma pack(pop)

inline FANGCLPOUT::FANGCLPOUT() noexcept {
  m_guide.start  = gsl::narrow_cast<decltype(m_guide.start)>(0U);
  m_guide.finish = gsl::narrow_cast<decltype(m_guide.finish)>(0U);
}

#pragma pack(push, 1)
union FLENCNTOUT {
  private:
  float    m_stitchLength {};
  uint32_t m_clipCount;

  public:
  FLENCNTOUT() noexcept = default;
  // FLENCNTOUT(FLENCNTOUT const&) = default;
  // FLENCNTOUT(FLENCNTOUT&&) = default;
  // FLENCNTOUT& operator=(FLENCNTOUT const& rhs) = default;
  // FLENCNTOUT& operator=(FLENCNTOUT&&) = default;
  //~FLENCNTOUT() = default;

  // Getter and Setter for stitchLength
  [[nodiscard]] auto getStitchLength() const noexcept -> float {
	return m_stitchLength;
  }
  void setStitchLength(float value) noexcept {
	m_stitchLength = value;
  }

  // Getter and Setter for clipCount
  [[nodiscard]] auto getClipCount() const noexcept -> uint32_t {
	return m_clipCount;
  }
  void setClipCount(uint32_t value) noexcept {
	m_clipCount = value;
  }
};
#pragma pack(pop)

constexpr auto MINRCT = 12.0F; // minimum dimension of a form select rectangle

#pragma pack(push, 1)
class FTHR_INFO
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint8_t  fillType {};      // feather fill type
  uint8_t  upCount {};       // feather up count
  uint8_t  downCount {};     // feather down count
  uint8_t  color {};         // feather blend color
  float    ratio {};         // feather ratio
  float    minStitchSize {}; // feather floor
  uint16_t count {};         // feather fill psg granularity
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  // FTHR_INFO() noexcept = default;
  // FTHR_INFO(FTHR_INFO const&) = default;
  // FTHR_INFO(FTHR_INFO&&) = default;
  // FTHR_INFO& operator=(FTHR_INFO const& rhs) = default;
  // FTHR_INFO& operator=(FTHR_INFO&&) = default;
  //~FTHR_INFO() = default;
};
#pragma pack(pop)

#pragma pack(push, 1)
class TXTR_INFO
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  int16_t  lines {};
  uint16_t index {};
  uint16_t count {};
  float    height {};
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  // TXTR_INFO() noexcept = default;
  // TXTR_INFO(TXTR_INFO const&) = default;
  // TXTR_INFO(TXTR_INFO&&) = default;
  // TXTR_INFO& operator=(TXTR_INFO const& rhs) = default;
  // TXTR_INFO& operator=(TXTR_INFO&&) = default;
  //~TXTR_INFO() = default;
};
#pragma pack(pop)

union TF_INFO_OUT;

#pragma pack(push, 1)
union TF_INFO_OUT {
  private:
  FTHR_INFO m_feather {};
  TXTR_INFO m_texture;

  public:
  TF_INFO_OUT() noexcept = default;
  // TF_INFO_OUT(TF_INFO_OUT const&) = default;
  // TF_INFO_OUT(TF_INFO_OUT&&) = default;
  // TF_INFO_OUT& operator=(TF_INFO_OUT const& rhs) = default;
  // TF_INFO_OUT& operator=(TF_INFO_OUT&&) = default;
  //~TF_INFO_OUT() = default;

  // Getter and Setter for feather
  [[nodiscard]] auto getFeather() const noexcept -> FTHR_INFO {
    return m_feather;
  }
  
  void setFeather(FTHR_INFO const& value) noexcept {
    m_feather = value;
  }

  // Getter and Setter for texture
  [[nodiscard]] auto getTexture() const noexcept -> TXTR_INFO {
    return m_texture;
  }
  
  void setTexture(TXTR_INFO const& value) noexcept {
	m_texture = value;
  }
};
#pragma pack(pop)

#pragma pack(push, 1)
union SATINANGLEOUT {
  private:
  DWORD m_guide {};
  float m_angle;

  public:
  SATINANGLEOUT() noexcept = default;
  // SATINANGLEOUT(SATINANGLEOUT const&) = default;
  // SATINANGLEOUT(SATINANGLEOUT&&) = default;
  // SATINANGLEOUT& operator=(SATINANGLEOUT const& rhs) = default;
  // SATINANGLEOUT& operator=(SATINANGLEOUT&&) = default;
  //~SATINANGLEOUT() = default;

  // Getter and Setter for guide
  [[nodiscard]] auto getGuide() const noexcept -> DWORD {
    return m_guide;
  }
  
  void setGuide(DWORD value) noexcept {
    m_guide = value;
  }

  // Getter and Setter for angle
  [[nodiscard]] auto getAngle() const noexcept -> float {
    return m_angle;
  }
  
  void setAngle(float value) noexcept {
    m_angle = value;
  }
};
#pragma pack(pop)

#pragma pack(push, 1)
class FRM_HEAD_O // Original form header used prior to version 2
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint8_t  attribute {};   // attribute
  uint16_t vertexCount {}; // number of sides
  uint8_t  type {};        // type
  uint8_t  fillColor {};   // fill color
  uint8_t  borderColor {}; // border color
  uint16_t clipEntries {}; // number of border clipboard entries
  DWORD    vertices {};    // points

  SATINANGLEOUT satinOrAngle {}; // satin guidelines or angle clipboard fill angle

  DWORD       borderClipData {};  // border clipboard data
  uint16_t    satinGuideCount {}; // number of satin guidelines
  uint16_t    wordParam {};       // word parameter
  F_RECTANGLE rectangle {};       // rectangle
  uint8_t     fillType {};        // fill type
  uint8_t     edgeType {};        // edge type
  float       fillSpacing {};     // fill spacing
  FLENCNTOUT  lengthOrCount {};   // fill stitch length or clipboard count
  FANGCLPOUT  angleOrClipData {}; // fill angle or clipboard data pointer
  float       borderSize {};      // border size
  float       edgeSpacing {};     // edge spacing
  float       edgeStitchLen {};   // edge stitch length
  uint16_t    res {};             // pico length
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  FRM_HEAD_O() noexcept = default;
  // FRM_HEAD_O(FRM_HEAD_O const&) = default;
  // FRM_HEAD_O(FRM_HEAD_O&&) = default;
  // FRM_HEAD_O& operator=(FRM_HEAD_O const& rhs) = default;
  // FRM_HEAD_O& operator=(FRM_HEAD_O&&) = default;
  //~FRM_HEAD_O() = default;
};
#pragma pack(pop)

class FRM_HEAD_OUT;

class FRM_HEAD
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint8_t     attribute {};           // attribute
  uint32_t    vertexCount {};         // number of sides
  uint8_t     type {};                // type
  uint8_t     fillColor {};           // fill color
  uint8_t     borderColor {};         // border color
  uint32_t    clipEntries {};         // number of border clipboard entries
  uint32_t    vertexIndex {};         // index into FormVertices
  uint32_t    satinGuideIndex {};     // satin guidelines
  float       clipFillAngle {};	      // clipboard fill angle
  uint32_t    borderClipData {};      // pointer to start of border clipboard data
  uint32_t    satinGuideCount {};     // number of satin guidelines
  uint32_t    wordParam {};           // clipboard/textured fill phase or satin end guide
  F_RECTANGLE rectangle {};           // rectangle
  uint8_t     fillType {};            // fill type
  uint8_t     edgeType {};            // edge type
  float       fillSpacing {};         // fill spacing
  float       stitchLength {};        // fill stitch length
  uint32_t    clipCount {};			  // number of points in fill clipboard data
  float       fillAngle {};           // fill angle
  uint32_t    clipIndex {};           // pointer to start of fill clipboard data
  SAT_CON     fillGuide {};           // fill guide
  float       borderSize {};          // border size
  float       edgeSpacing {};         // edge spacing
  float       edgeStitchLen {};       // edge stitch length
  uint16_t    picoLength {};          // pico length
  uint32_t    extendedAttribute {};   // attribute extension
  float       maxFillStitchLen {};    // maximum fill stitch length
  float       minFillStitchLen {};    // minimum fill stitch length
  float       maxBorderStitchLen {};  // maximum border stitch length
  float       minBorderStitchLen {};  // minimum border stitch length
  FTHR_INFO   feather {};             // feather attributes
  TXTR_INFO   texture {};             // texture attributes
  uint32_t    fillStart {};           // fill start point
  uint32_t    fillEnd {};             // fill end point
  float       underlaySpacing {};     // underlay spacing
  float       underlayStitchLen {};   // underlay stitch length
  float       underlayStitchAngle {}; // underlay stitch angle
  float       underlayIndent {};      // underlay/edge walk indent
  float       txof {};                // gradient end density
  uint8_t     underlayColor {};       // underlay color
  uint8_t     cres {};                // reserved
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  FRM_HEAD() noexcept = default;
  // FRM_HEAD(FRM_HEAD const&) = default;
  // FRM_HEAD(FRM_HEAD&&) = default;
  // FRM_HEAD& operator=(FRM_HEAD const& rhs) = default;
  // FRM_HEAD& operator=(FRM_HEAD&&) = default;
  //~FRM_HEAD() = default;

  explicit inline FRM_HEAD(FRM_HEAD_O const& rhs) noexcept;
  explicit inline FRM_HEAD(FRM_HEAD_OUT const& rhs) noexcept;

  inline auto               operator=(FRM_HEAD_O const& rhs) noexcept -> FRM_HEAD&;
  inline auto               operator=(FRM_HEAD_OUT const& rhs) noexcept -> FRM_HEAD&;
  [[nodiscard]] inline auto isClip() const noexcept -> bool;
  [[nodiscard]] inline auto isEdgeClip() const noexcept -> bool;
  [[nodiscard]] inline auto isClipX() const noexcept -> bool;
  [[nodiscard]] inline auto isEdgeClipX() const noexcept -> bool;
  [[nodiscard]] inline auto isTexture() const noexcept -> bool;
  [[nodiscard]] inline auto isFanClip() const noexcept -> bool;
  inline void               outline() noexcept(!(std::is_same_v<ptrdiff_t, int>));
  inline void               squareEnd(bool flag) noexcept;
};

#pragma pack(push, 1)
class FRM_HEAD_OUT
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint8_t  attribute {};   // attribute
  uint16_t vertexCount {}; // number of sides
  uint8_t  type {};        // type
  uint8_t  fillColor {};   // fill color
  uint8_t  borderColor {}; // border color
  uint16_t clipEntries {}; // number of border clipboard entries
  DWORD vertexIndex {}; // vertex index not saved in v1 or v2. size is to keep compatibility with v1 & v2 ThrEd files

  SATINANGLEOUT satinOrAngle {}; // satin guidelines or angle clipboard fill angle

  DWORD borderClipData {}; // border clipboard data pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
  uint16_t    satinGuideCount {}; // number of satin guidelines
  uint16_t    wordParam {};       // clipboard/textured fill phase or satin end guide
  F_RECTANGLE rectangle {};       // rectangle
  uint8_t     fillType {};        // fill type
  uint8_t     edgeType {};        // edge type
  float       fillSpacing {};     // fill spacing
  FLENCNTOUT  lengthOrCount {};   // fill stitch length or clipboard count
  FANGCLPOUT  angleOrClipData {}; // fill angle or clipboard data pointer

  float    borderSize {};          // border size
  float    edgeSpacing {};         // edge spacing
  float    edgeStitchLen {};       // edge stitch length
  uint16_t picoLength {};          // pico length
  uint32_t extendedAttribute {};   // attribute extension
  float    maxFillStitchLen {};    // maximum fill stitch length
  float    minFillStitchLen {};    // minimum fill stitch length
  float    maxBorderStitchLen {};  // maximum border stitch length
  float    minBorderStitchLen {};  // minimum border stitch length
  TF_INFO_OUT  fillInfo {};            // feather/texture info
  uint16_t fillStart {};           // fill start point
  uint16_t fillEnd {};             // fill end point
  float    underlaySpacing {};     // underlay spacing
  float    underlayStitchLen {};   // underlay stitch length
  float    underlayStitchAngle {}; // underlay stitch angle
  float    underlayIndent {};      // underlay/edge walk indent
  float    txof {};                // gradient end density
  uint8_t  underlayColor {};       // underlay color
  uint8_t  cres {};                // reserved
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  explicit constexpr FRM_HEAD_OUT() noexcept = default;
  // FRM_HEAD_OUT(FRM_HEAD_OUT const&) = default;
  // FRM_HEAD_OUT(FRM_HEAD_OUT&&) = default;
  // FRM_HEAD_OUT& operator=(FRM_HEAD_OUT const& rhs) = default;
  // FRM_HEAD_OUT& operator=(FRM_HEAD_OUT&&) = default;
  //~FRM_HEAD_OUT() = default;

  explicit inline FRM_HEAD_OUT(FRM_HEAD const& rhs);
};
#pragma pack(pop)

// Use of unions in the output conversion routines is unavoidable,
// but even using getters and setters, the warnings still occur, so....
// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)

inline FRM_HEAD::FRM_HEAD(FRM_HEAD_O const& rhs) noexcept :
    attribute(rhs.attribute), vertexCount(rhs.vertexCount), type(rhs.type), fillColor(rhs.fillColor),
    borderColor(rhs.borderColor), clipEntries(rhs.clipEntries),
    borderClipData(rhs.borderClipData), // Todo - Should we be copying this value?
    satinGuideCount(rhs.satinGuideCount), wordParam(rhs.wordParam), rectangle(rhs.rectangle),
    fillType(rhs.fillType), edgeType(rhs.edgeType), fillSpacing(rhs.fillSpacing),
    borderSize(rhs.borderSize), edgeSpacing(rhs.edgeSpacing), edgeStitchLen(rhs.edgeStitchLen),
    picoLength(rhs.res) {
  if (isClip()) {
	clipCount = rhs.lengthOrCount.getClipCount();
  }
  else {
	stitchLength = rhs.lengthOrCount.getStitchLength();
  }
  if (isEdgeClipX() || isClipX()) {
	clipIndex = rhs.angleOrClipData.getClip();
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		fillGuide = rhs.angleOrClipData.getGuide();
	  }
	}
	else {
	  fillAngle = rhs.angleOrClipData.getAngle();
	}
  }
  if (fillType == ANGCLPF) {
	clipFillAngle = rhs.satinOrAngle.getAngle();
  }
  else {
	satinGuideIndex = rhs.satinOrAngle.getGuide();
  }
}

inline auto FRM_HEAD::operator=(FRM_HEAD_O const& rhs) noexcept -> FRM_HEAD& {
  attribute       = rhs.attribute;
  vertexCount     = rhs.vertexCount;
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = rhs.clipEntries;
  vertexIndex     = 0U;
  borderClipData  = rhs.borderClipData; // Todo - Should we be copying this value?
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  if (fillType == ANGCLPF) {
	clipFillAngle = rhs.satinOrAngle.getAngle();
  }
  else {
	satinGuideIndex = rhs.satinOrAngle.getGuide();
  }
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  if (isClip()) {
	clipCount = rhs.lengthOrCount.getClipCount();
  }
  else {
	stitchLength = rhs.lengthOrCount.getStitchLength();
  }
  if (isEdgeClipX() || isClipX()) {
	clipIndex = rhs.angleOrClipData.getClip();
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		fillGuide = rhs.angleOrClipData.getGuide();
	  }
	}
	else {
	  fillAngle = rhs.angleOrClipData.getAngle();
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.res;
  return *this;
}

inline FRM_HEAD_OUT::FRM_HEAD_OUT(FRM_HEAD const& rhs) :
    attribute(rhs.attribute), vertexCount(gsl::narrow<uint16_t>(rhs.vertexCount)), type(rhs.type),
    fillColor(rhs.fillColor), borderColor(rhs.borderColor),
    clipEntries(gsl::narrow<uint16_t>(rhs.clipEntries)),
    satinGuideCount(gsl::narrow<uint16_t>(rhs.satinGuideCount)),
    wordParam(gsl::narrow<uint16_t>(rhs.wordParam)), rectangle(rhs.rectangle), fillType(rhs.fillType),
    edgeType(rhs.edgeType), fillSpacing(rhs.fillSpacing), borderSize(rhs.borderSize),
    edgeSpacing(rhs.edgeSpacing), edgeStitchLen(rhs.edgeStitchLen), picoLength(rhs.picoLength),
    extendedAttribute(rhs.extendedAttribute), maxFillStitchLen(rhs.maxFillStitchLen),
    minFillStitchLen(rhs.minFillStitchLen), maxBorderStitchLen(rhs.maxBorderStitchLen),
    minBorderStitchLen(rhs.minBorderStitchLen),
    fillStart(gsl::narrow<uint16_t>(rhs.fillStart)), fillEnd(gsl::narrow<uint16_t>(rhs.fillEnd)),
    underlaySpacing(rhs.underlaySpacing), underlayStitchLen(rhs.underlayStitchLen),
    underlayStitchAngle(rhs.underlayStitchAngle), underlayIndent(rhs.underlayIndent),
    txof(rhs.txof), underlayColor(rhs.underlayColor), cres(rhs.cres) {
  if (rhs.isClip()) {
	lengthOrCount.setClipCount(rhs.clipCount);
  }
  else {
	lengthOrCount.setStitchLength(rhs.stitchLength);
  }
  if (rhs.isEdgeClipX() || rhs.isClipX()) {
	angleOrClipData.setClip(rhs.clipIndex);
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.setGuide(rhs.fillGuide);
	  }
	}
	else {
	  angleOrClipData.setAngle(rhs.fillAngle);
	}
  }
  if (fillType == ANGCLPF) {
	satinOrAngle.setAngle(rhs.clipFillAngle);
  }
  else {
	satinOrAngle.setGuide(rhs.satinGuideIndex);
  }
  if (rhs.isTexture()) {
	fillInfo.setTexture(rhs.texture);
  }
  else {
	fillInfo.setFeather(rhs.feather);
  }
}

inline FRM_HEAD::FRM_HEAD(FRM_HEAD_OUT const& rhs) noexcept :
    attribute(rhs.attribute), vertexCount(rhs.vertexCount), type(rhs.type), fillColor(rhs.fillColor),
    borderColor(rhs.borderColor), clipEntries(rhs.clipEntries),
    satinGuideCount(rhs.satinGuideCount), wordParam(rhs.wordParam), rectangle(rhs.rectangle),
    fillType(rhs.fillType), edgeType(rhs.edgeType), fillSpacing(rhs.fillSpacing),
    borderSize(rhs.borderSize), edgeSpacing(rhs.edgeSpacing), edgeStitchLen(rhs.edgeStitchLen),
    picoLength(rhs.picoLength), extendedAttribute(rhs.extendedAttribute),
    maxFillStitchLen(rhs.maxFillStitchLen), minFillStitchLen(rhs.minFillStitchLen),
    maxBorderStitchLen(rhs.maxBorderStitchLen), minBorderStitchLen(rhs.minBorderStitchLen),
    fillStart(rhs.fillStart), fillEnd(rhs.fillEnd),
    underlaySpacing(rhs.underlaySpacing), underlayStitchLen(rhs.underlayStitchLen),
    underlayStitchAngle(rhs.underlayStitchAngle), underlayIndent(rhs.underlayIndent),
    txof(rhs.txof), underlayColor(rhs.underlayColor), cres(rhs.cres) {
  if (isClip()) {
	clipCount = rhs.lengthOrCount.getClipCount();
  }
  else {
	stitchLength = rhs.lengthOrCount.getStitchLength();
  }
  if (isEdgeClipX() || isClipX()) {
	clipIndex = rhs.angleOrClipData.getClip();
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		fillGuide = rhs.angleOrClipData.getGuide();
	  }
	}
	else {
	  fillAngle = rhs.angleOrClipData.getAngle();
	}
  }
  if (fillType == ANGCLPF) {
	clipFillAngle  = rhs.satinOrAngle.getAngle();
  }
  else {
	satinGuideIndex = rhs.satinOrAngle.getGuide();
  }
  if (isTexture()) {
	texture = rhs.fillInfo.getTexture();
  }
  else {
	feather = rhs.fillInfo.getFeather();
  }
}

inline auto FRM_HEAD::operator=(FRM_HEAD_OUT const& rhs) noexcept -> FRM_HEAD& {
  attribute       = rhs.attribute;
  vertexCount     = rhs.vertexCount;
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = rhs.clipEntries;
  vertexIndex     = 0U; // do not read the index from a v2 file
  borderClipData  = 0U; // do not read the index from a v2 file
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  if (fillType == ANGCLPF) {
	clipFillAngle = rhs.satinOrAngle.getAngle();
  }
  else {
	satinGuideIndex = rhs.satinOrAngle.getGuide();
  }
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  if (isClip()) {
	clipCount = rhs.lengthOrCount.getClipCount();
  }
  else {
	stitchLength = rhs.lengthOrCount.getStitchLength();
  }
  if (isEdgeClipX() || isClipX()) {
	clipIndex = rhs.angleOrClipData.getClip();
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		fillGuide = rhs.angleOrClipData.getGuide();
	  }
	}
	else {
	  fillAngle = rhs.angleOrClipData.getAngle();
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.picoLength;

  extendedAttribute   = rhs.extendedAttribute;
  maxFillStitchLen    = rhs.maxFillStitchLen;
  minFillStitchLen    = rhs.minFillStitchLen;
  maxBorderStitchLen  = rhs.maxBorderStitchLen;
  minBorderStitchLen  = rhs.minBorderStitchLen;
  if (isTexture()) {
	texture = rhs.fillInfo.getTexture();
  }
  else {
	feather = rhs.fillInfo.getFeather();
  }
  fillStart           = rhs.fillStart;
  fillEnd             = rhs.fillEnd;
  underlaySpacing     = rhs.underlaySpacing;
  underlayStitchLen   = rhs.underlayStitchLen;
  underlayStitchAngle = rhs.underlayStitchAngle;
  underlayIndent      = rhs.underlayIndent;
  txof                = rhs.txof;
  underlayColor       = rhs.underlayColor;
  cres                = rhs.cres;

  return *this;
}

// NOLINTEND(cppcoreguidelines-pro-type-union-access)

inline auto FRM_HEAD::isClip() const noexcept -> bool {
  return ((1U << fillType) & CLIPTYPEMAP) != 0;
}

inline auto FRM_HEAD::isEdgeClip() const noexcept -> bool {
  return edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX;
}

inline auto FRM_HEAD::isClipX() const noexcept -> bool {
  return isClip() && (clipCount != 0U);
}

inline auto FRM_HEAD::isEdgeClipX() const noexcept -> bool {
  return isEdgeClip() && (clipEntries != 0U);
}

inline auto FRM_HEAD::isFanClip() const noexcept -> bool {
  return isClip() && fillType != CLPF;
}

inline auto FRM_HEAD::isTexture() const noexcept -> bool {
  if (fillType == TXVRTF) {
	return true;
  }
  if (fillType == TXHORF) {
	return true;
  }
  if (fillType == TXANGF) {
	return true;
  }
  return false;
}

inline void FRM_HEAD::outline() noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  if (vertexCount != 0U) {
	if (const auto* formVertices = wrap::getFormVertices(); nullptr != formVertices) {
	auto        itVertex     = wrap::next(formVertices->cbegin(), vertexIndex);
	rectangle                = F_RECTANGLE {itVertex->x, itVertex->y, itVertex->x, itVertex->y};
	for (auto iVertex = 1U; iVertex < vertexCount; ++iVertex) {
	  ++itVertex;
	  if (itVertex->x < rectangle.left) {
		rectangle.left = itVertex->x;
	  }
	  if (itVertex->y > rectangle.top) {
		rectangle.top = itVertex->y;
	  }
	  if (itVertex->x > rectangle.right) {
		rectangle.right = itVertex->x;
	  }
	  if (itVertex->y < rectangle.bottom) {
		rectangle.bottom = itVertex->y;
	  }
	}
	if (rectangle.top - rectangle.bottom < MINRCT) {
	  auto const offset = (MINRCT - (rectangle.top - rectangle.bottom)) / 2;
	  rectangle.top += offset;
	  rectangle.bottom -= offset;
	}
	if (rectangle.right - rectangle.left < MINRCT) {
	  auto const offset = (MINRCT - (rectangle.right - rectangle.left)) / 2;
	  rectangle.left -= offset;
	  rectangle.right += offset;
	}
  }
  }
}

inline void FRM_HEAD::squareEnd(bool flag) noexcept {
  if (flag) {
	extendedAttribute |= AT_SQR;
  }
  else {
	extendedAttribute &= ~(AT_SQR);
  }
}
