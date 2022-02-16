#pragma once

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
constexpr uint32_t OLDNUM   = 4U;                      // number of old filenames saved on file menu

// wave initial values
constexpr uint16_t IWAVPNTS = 36U; // default wave points
constexpr uint16_t IWAVSTRT = 10U; // default wave start
constexpr uint16_t IWAVEND  = 26U; // default wave end
constexpr uint16_t IWAVS    = 5U;  // default wave lobes
// end of wave initial values

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
};
#pragma pack(pop)

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
