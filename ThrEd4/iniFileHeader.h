#pragma once
// Local Headers
#include "thredFileHeader.h"


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cstdint>
#include <cstdlib>

constexpr auto     COLORCNT = uint8_t {16U};           // Number of colors in arrays
constexpr auto     COLORMAX = uint8_t {COLORCNT - 1U}; // max index in color arrays
constexpr auto     OLDNUM   = uint32_t {4U};           // number of old filenames saved on file menu

constexpr auto FDEFTYP = int32_t {FTHPSG}; // default feather type

#pragma pack(push, 1)
class INI_FILE // ini file structure
{
  public:
  char     defaultDirectory[180] {};               // default directory
  COLORREF stitchColors[COLORCNT] {};              // colors
  COLORREF stitchPreferredColors[COLORCNT] {};     // stitch preference colors
  COLORREF backgroundPreferredColors[COLORCNT] {}; // background preference colors
  COLORREF backgroundColor {};                     // background color
  COLORREF bitmapColor {};                         // bitmap color
  float    minStitchLength {};                     // minimum stitch length
  float    showStitchThreshold {};                 // show stitch points
  float    threadSize30 {};                        // millimeter size of 30 weight thread
  float    threadSize40 {};                        // millimeter size of 40 weight thread
  float    threadSize60 {};                        // millimeter size of 60 weight thread
  float    userStitchLength {};                    // user stitch length
  float    maxStitchLength {};                     // maximum stitch length
  float    smallStitchLength {};                   // small stitch size
  float    stitchBoxesThreshold {};                // show sitch box level
  float    stitchSpace {};                         // stitch spacing between lines of stitches
  float    fillAngle {};                           // fill angle
  uint32_t userFlagMap {};                         // bitmap for user variables
  float    borderWidth {};                         // border width
  uint32_t appliqueColor {};                       // applique color
  char     prevNames[OLDNUM][_MAX_PATH] {{}};      // last file names
  float    snapLength {};                          // snap together length
  float    starRatio {};                           // star ratio
  float    spiralWrap {};                          // spiral wrap
  COLORREF bitmapBackgroundColors[COLORCNT] {};    // bitmap background color preferences
  float    buttonholeCornerLength {};              // buttonhole fill corner length
  float    picotSpace {};                          // space between border picots
  int8_t   hoopType {};                            // hoop type
  int8_t   auxFileType {};                         // machine file type
  float    hoopSizeX {};                           // hoop x size
  float    hoopSizeY {};                           // hoop y size
  float    rotationAngle {};                       // rotation angle
  float    gridSize {};                            // grid size
  float    clipOffset {};                          // clipboard offset
  RECT     initialWindowCoords {};                 // initial window coordinates
  COLORREF gridColor {};                           // grid color
  uint32_t fillPhase {};                           // clipboard fill phase
  float    customHoopX {};                         // custom hoop width
  float    customHoopY {};                         // custom hoop height
  float    traceLength {};                         // lens points
  float    traceRatio {};                          // trace ratio
  float    chainSpace {};                          // chain space
  float    chainRatio {};                          // chain ratio
  float    lensRatio {};                           // lens aspect ratio
  float    AppStitchLen {};                        // Applique stitch length
  float    cursorNudgeStep {};                     // cursor nudge step
  uint16_t nudgePixels {};                         // nudge pixels
  float    eggRatio {};                            // egg ratio
  uint16_t stitchSizePixels {};                    // size of stitch points in pixels
  uint16_t formVertexSizePixels {};                // size of form points in pixels
  uint16_t formSides {};                           // sides of a created form
  float    tearTailLength {};                      // length of the tear tail
  float    tearTwistStep {};                       // tear twist step
  float    tearTwistRatio {};                      // tear twist ratio
  uint16_t wavePoints {};                          // wave points
  uint16_t waveStart {};                           // wave strting point
  uint16_t waveEnd {};                             // wave ending point;
  uint16_t waveLobes {};                           // wave lobes
  uint8_t  featherFillType {};                     // feather fill type
  uint8_t  featherUpCount {};                      // feather up count
  uint8_t  featherDownCount {};                    // feather down count
  uint8_t  featherType {};                         // feather bits
  float    featherRatio {};                        // feather ratio
  float    featherMinStitchSize {};                // feather floor
  uint16_t featherCount {};                        // feather fill psg granularity
  char     p2cName[MAX_PATH] {};                   // pes2card file
  float    underlayIndent {};                      // edge walk/underlay indent
  float    underlayAngle {};                       // underlay angle
  float    underlaySpacing {};                     // underlay spacing
  float    underlayStitchLen {};                   // underlay stitch length
  float    daisyDiameter {};                       // daisy diameter
  float    daisyPetalLen {};                       // daisy petal length
  float    daisyHoleDiameter {};                   // daisy hole diameter
  uint32_t daisyPetalCount {};                     // daisy petals
  uint32_t daisyPetalPoints {};                    // daisy petal points
  uint32_t daisyInnerCount {};                     // daisy inner count
  uint8_t  daisyBorderType {};                     // daisy border type
  uint8_t  dataCheck {};                           // data check
  float    textureHeight {};                       // textured fill height
  float    textureWidth {};                        // textured fill width
  float    textureSpacing {};                      // textured fill spacing
  uint16_t formBoxSizePixels {};                   // form box pixels
  uint16_t daisyHeartCount {};                     // daisy heart count
  uint16_t textureEditorSize {};                   // texture editor pixels
  float    clpspc {};                              // clipboard fill spacing
  char     designerName[NAME_LEN] {};              // designer name

  // constexpr INI_FILE() noexcept = default;
  // INI_FILE(INI_FILE const&) = default;
  // INI_FILE(INI_FILE&&) = default;
  // INI_FILE& operator=(INI_FILE const& rhs) = default;
  // INI_FILE& operator=(INI_FILE&&) = default;
  //~INI_FILE() = default;
  inline void setWav() noexcept;
  inline void defWav() noexcept;
  inline void defFeather() noexcept;
  inline void setFeather() noexcept;
  inline void initFeatherType() noexcept;
  inline void dazdef() noexcept;
  inline void setDaisy() noexcept;
};
#pragma pack(pop)

// wave initial values
constexpr auto IWAVPNTS = uint16_t {36U}; // default wave points
constexpr auto IWAVSTRT = uint16_t {10U}; // default wave start
constexpr auto IWAVEND  = uint16_t {26U}; // default wave end
constexpr auto IWAVS    = uint16_t {5U};  // default wave lobes
// end of wave initial values

inline void INI_FILE::setWav() noexcept {
  if (wavePoints == 0U) {
	wavePoints = IWAVPNTS;
  }
  if (waveStart == 0U) {
	waveStart = IWAVSTRT;
  }
  if (waveEnd == 0U) {
	waveEnd = IWAVEND;
  }
  if (waveLobes == 0U) {
	waveLobes = IWAVS;
  }
}

inline void INI_FILE::defWav() noexcept {
  waveEnd    = IWAVEND;
  wavePoints = IWAVPNTS;
  waveLobes  = IWAVS;
  waveStart  = IWAVSTRT;
}

inline void INI_FILE::setFeather() noexcept {
  if (featherFillType == 0U) {
	featherFillType = FDEFTYP;
  }
  if (featherUpCount == 0U) {
	featherUpCount = FDEFUP;
  }
  if (featherDownCount == 0U) {
	featherDownCount = FDEFDWN;
  }
  if (featherRatio == 0.0F) {
	featherRatio = FDEFRAT;
  }
  if (featherMinStitchSize == 0.0F) {
	featherMinStitchSize = FDEFFLR;
  }
  if (featherCount == 0U) {
	featherCount = FDEFNUM;
  }
}

inline void INI_FILE::defFeather() noexcept {
  featherFillType      = FDEFTYP;
  featherUpCount       = FDEFUP;
  featherDownCount     = FDEFDWN;
  featherRatio         = FDEFRAT;
  featherMinStitchSize = FDEFFLR;
  featherCount         = FDEFNUM;
}

inline void INI_FILE::initFeatherType() noexcept {
  featherFillType = FDEFTYP;
}

// daisy initial values
constexpr auto DAZPETS = 5U;  // petals
constexpr auto DAZCNT  = 10U; // petal points
constexpr auto DAZICNT = 2U;  // petal inner points
constexpr auto DAZLEN  = 15U; // diameter
constexpr auto DAZPLEN = 20U; // petal length
constexpr auto DAZHLEN = 5U;  // hole size
constexpr auto DAZTYP  = 5U;  // border type
constexpr auto DAZMCNT = 7U;  // mirror count
// end of daisy initial values

inline void INI_FILE::dazdef() noexcept {
  daisyPetalPoints  = DAZCNT;
  daisyHoleDiameter = DAZHLEN;
  daisyInnerCount   = DAZICNT;
  daisyDiameter     = DAZLEN;
  daisyPetalCount   = DAZPETS;
  daisyPetalLen     = DAZPLEN;
  daisyHeartCount   = DAZMCNT;
  daisyBorderType   = DAZTYP;
}

inline void INI_FILE::setDaisy() noexcept {
  if (daisyHoleDiameter == 0.0F) {
	daisyHoleDiameter = DAZHLEN;
  }
  if (daisyPetalPoints == 0U) {
	daisyPetalPoints = DAZCNT;
  }
  if (daisyInnerCount == 0U) {
	daisyInnerCount = DAZICNT;
  }
  if (daisyDiameter == 0.0F) {
	daisyDiameter = DAZLEN;
  }
  if (daisyPetalCount == 0U) {
	daisyPetalCount = DAZPETS;
  }
  if (daisyPetalLen == 0.0F) {
	daisyPetalLen = DAZPLEN;
  }
}
