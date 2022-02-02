#pragma once

// Standard Libraries
#include <cstdint>

// Open Source headers

// Local Headers
#include "ThrEdTypes.h"

#pragma pack(push, 1)
class FRM_HEAD_O // Original form header used prior to version 2
{
  public:
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
  uint8_t     attribute {};           // attribute
  uint32_t    vertexCount {};         // number of sides
  uint8_t     type {};                // type
  uint8_t     fillColor {};           // fill color
  uint8_t     borderColor {};         // border color
  uint32_t    clipEntries {};         // number of border clipboard entries
  uint32_t    vertexIndex {};         // index into FormVertices
  SATINANGLE  satinOrAngle {};        // satin guidelines or angle clipboard fill angle
  uint32_t    borderClipData {};      // pointer to start of border clipboard data
  uint32_t    satinGuideCount {};     // number of satin guidelines
  uint32_t    wordParam {};           // clipboard/textured fill phase or satin end guide
  F_RECTANGLE rectangle {};           // rectangle
  uint8_t     fillType {};            // fill type
  uint8_t     edgeType {};            // edge type
  float       fillSpacing {};         // fill spacing
  FLENCNT     lengthOrCount {};       // fill stitch length or clipboard count
  FANGCLP     angleOrClipData {};     // fill angle or clipboard data pointer
  float       borderSize {};          // border size
  float       edgeSpacing {};         // edge spacing
  float       edgeStitchLen {};       // edge stitch length
  uint16_t    picoLength {};          // pico length
  uint32_t    extendedAttribute {};   // attribute extension
  float       maxFillStitchLen {};    // maximum fill stitch length
  float       minFillStitchLen {};    // minimum fill stitch length
  float       maxBorderStitchLen {};  // maximum border stitch length
  float       minBorderStitchLen {};  // minimum border stitch length
  TF_INFO     fillInfo {};            // feather/texture info
  uint32_t    fillStart {};           // fill start point
  uint32_t    fillEnd {};             // fill end point
  float       underlaySpacing {};     // underlay spacing
  float       underlayStitchLen {};   // underlay stitch length
  float       underlayStitchAngle {}; // underlay stitch angle
  float       underlayIndent {};      // underlay/edge walk indent
  float       txof {};                // gradient end density
  uint8_t     underlayColor {};       // underlay color
  uint8_t     cres {};                // reserved

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
  [[nodiscard]] inline auto isclp() const noexcept -> bool;
  [[nodiscard]] inline auto iseclp() const noexcept -> bool;
  [[nodiscard]] inline auto isclpx() const noexcept -> bool;
  [[nodiscard]] inline auto iseclpx() const noexcept -> bool;
  [[nodiscard]] inline auto istx() const noexcept -> bool;
  [[nodiscard]] inline auto isfclp() const noexcept -> bool;
  inline void               outline();
  inline void               squareEnd(bool flag) noexcept;
};

#pragma pack(push, 1)
class FRM_HEAD_OUT
{
  public:
  uint8_t  attribute {};   // attribute
  uint16_t vertexCount {}; // number of sides
  uint8_t  type {};        // type
  uint8_t  fillColor {};   // fill color
  uint8_t  borderColor {}; // border color
  uint16_t clipEntries {}; // number of border clipboard entries
  DWORD    vertexIndex {}; // vertex index not saved in v1 or v2. size is to keep compatibility with v1 & v2 ThrEd files

  SATINANGLEOUT satinOrAngle {}; // satin guidelines or angle clipboard fill angle

  DWORD       borderClipData {}; // border clipboard data pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
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
  TF_INFO  fillInfo {};            // feather/texture info
  uint16_t fillStart {};           // fill start point
  uint16_t fillEnd {};             // fill end point
  float    underlaySpacing {};     // underlay spacing
  float    underlayStitchLen {};   // underlay stitch length
  float    underlayStitchAngle {}; // underlay stitch angle
  float    underlayIndent {};      // underlay/edge walk indent
  float    txof {};                // gradient end density
  uint8_t  underlayColor {};       // underlay color
  uint8_t  cres {};                // reserved

  inline FRM_HEAD_OUT() noexcept = default;
  // FRM_HEAD_OUT(FRM_HEAD_OUT const&) = default;
  // FRM_HEAD_OUT(FRM_HEAD_OUT&&) = default;
  // FRM_HEAD_OUT& operator=(FRM_HEAD_OUT const& rhs) = default;
  // FRM_HEAD_OUT& operator=(FRM_HEAD_OUT&&) = default;
  //~FRM_HEAD_OUT() = default;

  explicit inline FRM_HEAD_OUT(FRM_HEAD const& rhs);
  inline auto operator=(FRM_HEAD const& rhs) -> FRM_HEAD_OUT&;
};
#pragma pack(pop)

inline FRM_HEAD::FRM_HEAD(FRM_HEAD_O const& rhs) noexcept :
    attribute(rhs.attribute), vertexCount(rhs.vertexCount), type(rhs.type), fillColor(rhs.fillColor),
    borderColor(rhs.borderColor), clipEntries(rhs.clipEntries), satinOrAngle(rhs.satinOrAngle),
    borderClipData(rhs.borderClipData), // Todo - Should we be copying this value?
    satinGuideCount(rhs.satinGuideCount), wordParam(rhs.wordParam), rectangle(rhs.rectangle),
    fillType(rhs.fillType), edgeType(rhs.edgeType), fillSpacing(rhs.fillSpacing),
    lengthOrCount(rhs.lengthOrCount), borderSize(rhs.borderSize), edgeSpacing(rhs.edgeSpacing),
    edgeStitchLen(rhs.edgeStitchLen), picoLength(rhs.res), maxFillStitchLen(0.0F),
    minFillStitchLen(0.0F), maxBorderStitchLen(0.0F), minBorderStitchLen(0.0F), underlaySpacing(0.0F),
    underlayStitchLen(0.0F), underlayStitchAngle(0.0F), underlayIndent(0.0F), txof(0.0F) {
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
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
  satinOrAngle    = rhs.satinOrAngle;
  borderClipData  = rhs.borderClipData; // Todo - Should we be copying this value?
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  lengthOrCount   = rhs.lengthOrCount;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
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
    clipEntries(gsl::narrow<uint16_t>(rhs.clipEntries)), satinOrAngle(rhs.satinOrAngle),
    satinGuideCount(gsl::narrow<uint16_t>(rhs.satinGuideCount)),
    wordParam(gsl::narrow<uint16_t>(rhs.wordParam)), rectangle(rhs.rectangle), fillType(rhs.fillType),
    edgeType(rhs.edgeType), fillSpacing(rhs.fillSpacing), lengthOrCount(rhs.lengthOrCount),
    borderSize(rhs.borderSize), edgeSpacing(rhs.edgeSpacing), edgeStitchLen(rhs.edgeStitchLen),
    picoLength(rhs.picoLength), extendedAttribute(rhs.extendedAttribute),
    maxFillStitchLen(rhs.maxFillStitchLen), minFillStitchLen(rhs.minFillStitchLen),
    maxBorderStitchLen(rhs.maxBorderStitchLen), minBorderStitchLen(rhs.minBorderStitchLen),
    fillInfo(rhs.fillInfo), fillStart(gsl::narrow<uint16_t>(rhs.fillStart)),
    fillEnd(gsl::narrow<uint16_t>(rhs.fillEnd)), underlaySpacing(rhs.underlaySpacing),
    underlayStitchLen(rhs.underlayStitchLen), underlayStitchAngle(rhs.underlayStitchAngle),
    underlayIndent(rhs.underlayIndent), txof(rhs.txof), underlayColor(rhs.underlayColor), cres(rhs.cres) {
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start =
		    gsl::narrow<decltype(angleOrClipData.guide.start)>(rhs.angleOrClipData.guide.start);
		angleOrClipData.guide.finish =
		    gsl::narrow<decltype(angleOrClipData.guide.finish)>(rhs.angleOrClipData.guide.finish);
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
  }
}

inline auto FRM_HEAD_OUT::operator=(FRM_HEAD const& rhs) -> FRM_HEAD_OUT& {
  attribute       = rhs.attribute;
  vertexCount     = gsl::narrow<uint16_t>(rhs.vertexCount);
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = gsl::narrow<uint16_t>(rhs.clipEntries);
  vertexIndex     = 0; // do not write the pointer value to file
  satinOrAngle    = rhs.satinOrAngle;
  borderClipData  = 0; // do not write the pointer value to file
  satinGuideCount = gsl::narrow<uint16_t>(rhs.satinGuideCount);
  wordParam       = gsl::narrow<uint16_t>(rhs.wordParam);
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  lengthOrCount   = rhs.lengthOrCount;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start =
		    gsl::narrow<decltype(angleOrClipData.guide.start)>(rhs.angleOrClipData.guide.start);
		angleOrClipData.guide.finish =
		    gsl::narrow<decltype(angleOrClipData.guide.finish)>(rhs.angleOrClipData.guide.finish);
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
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
  fillInfo            = rhs.fillInfo;
  fillStart           = gsl::narrow<uint16_t>(rhs.fillStart);
  fillEnd             = gsl::narrow<uint16_t>(rhs.fillEnd);
  underlaySpacing     = rhs.underlaySpacing;
  underlayStitchLen   = rhs.underlayStitchLen;
  underlayStitchAngle = rhs.underlayStitchAngle;
  underlayIndent      = rhs.underlayIndent;
  txof                = rhs.txof;
  underlayColor       = rhs.underlayColor;
  cres                = rhs.cres;

  return *this;
}

inline FRM_HEAD::FRM_HEAD(FRM_HEAD_OUT const& rhs) noexcept :
    attribute(rhs.attribute), vertexCount(rhs.vertexCount), type(rhs.type),
    fillColor(rhs.fillColor), borderColor(rhs.borderColor), clipEntries(rhs.clipEntries),
    satinOrAngle(rhs.satinOrAngle), satinGuideCount(rhs.satinGuideCount), wordParam(rhs.wordParam),
    rectangle(rhs.rectangle), fillType(rhs.fillType), edgeType(rhs.edgeType),
    fillSpacing(rhs.fillSpacing), lengthOrCount(rhs.lengthOrCount), borderSize(rhs.borderSize),
    edgeSpacing(rhs.edgeSpacing), edgeStitchLen(rhs.edgeStitchLen), picoLength(rhs.picoLength),
    extendedAttribute(rhs.extendedAttribute), maxFillStitchLen(rhs.maxFillStitchLen),
    minFillStitchLen(rhs.minFillStitchLen), maxBorderStitchLen(rhs.maxBorderStitchLen),
    minBorderStitchLen(rhs.minBorderStitchLen), fillInfo(rhs.fillInfo), fillStart(rhs.fillStart),
    fillEnd(rhs.fillEnd), underlaySpacing(rhs.underlaySpacing), underlayStitchLen(rhs.underlayStitchLen),
    underlayStitchAngle(rhs.underlayStitchAngle), underlayIndent(rhs.underlayIndent),
    txof(rhs.txof), underlayColor(rhs.underlayColor), cres(rhs.cres) {
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
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
  satinOrAngle    = rhs.satinOrAngle;
  borderClipData  = 0U; // do not read the index from a v2 file
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  lengthOrCount   = rhs.lengthOrCount;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
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
  fillInfo            = rhs.fillInfo;
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

inline auto FRM_HEAD::isclp() const noexcept -> bool {
  return ((1U << fillType) & CLIPTYPEMAP) != 0;
}

inline auto FRM_HEAD::iseclp() const noexcept -> bool {
  return edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX;
}

inline auto FRM_HEAD::isclpx() const noexcept -> bool {
  return isclp() && (lengthOrCount.clipCount != 0U);
}

inline auto FRM_HEAD::iseclpx() const noexcept -> bool {
  return iseclp() && (clipEntries != 0U);
}

inline auto FRM_HEAD::isfclp() const noexcept -> bool {
  return isclp() && fillType != CLPF;
}

inline auto FRM_HEAD::istx() const noexcept -> bool {
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

inline void FRM_HEAD::outline() {
  if (vertexCount != 0U) {
	const auto* formVertices = wrap::getFormVertices();
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

inline void FRM_HEAD::squareEnd(bool flag) noexcept {
  if (flag) {
	extendedAttribute |= AT_SQR;
  }
  else {
	extendedAttribute &= ~(AT_SQR);
  }
}
