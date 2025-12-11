#ifndef THRED_INIFILE_H
#define THRED_INIFILE_H

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

constexpr auto COLORCNT = uint8_t {16U};           // Number of colors in arrays
constexpr auto COLORMAX = uint8_t {COLORCNT - 1U}; // max index in color arrays
constexpr auto NAME_LEN = 50;                      // Length of the name fields in ThrEd headers
constexpr auto OLDNUM   = uint32_t {4U};           // number of old filenames saved on file menu
constexpr auto DPLEN    = 180;                     // default directory path length

constexpr auto FDEFTYP = int32_t {FTHPSG}; // default feather type

#pragma pack(push, 1)
class INI_FILE // ini file structure
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::array<char, DPLEN>        defaultDirectory {};          // default directory
  std::array<COLORREF, COLORCNT> stitchColors {};              // colors
  std::array<COLORREF, COLORCNT> stitchPreferredColors {};     // stitch preference colors
  std::array<COLORREF, COLORCNT> backgroundPreferredColors {}; // background preference colors

  COLORREF backgroundColor {};      // background color
  COLORREF bitmapColor {};          // bitmap color
  float    minStitchLength {};      // minimum stitch length
  float    showStitchThreshold {};  // show stitch points
  float    threadSize30 {};         // millimeter size of 30 weight thread
  float    threadSize40 {};         // millimeter size of 40 weight thread
  float    threadSize60 {};         // millimeter size of 60 weight thread
  float    userStitchLength {};     // user stitch length
  float    maxStitchLength {};      // maximum stitch length
  float    smallStitchLength {};    // small stitch size
  float    stitchBoxesThreshold {}; // show sitch box level
  float    stitchSpace {};          // stitch spacing between lines of stitches
  float    fillAngle {};            // fill angle
  uint32_t userFlagMap {};          // bitmap for user variables
  float    borderWidth {};          // border width
  uint32_t appliqueColor {};        // applique color

  std::array<std::array<char, _MAX_PATH>, OLDNUM> prevNames {{}}; // last file names

  float snapLength {}; // snap together length
  float starRatio {};  // star ratio
  float spiralWrap {}; // spiral wrap

  std::array<COLORREF, COLORCNT> bitmapBackgroundColors {}; // bitmap background color preferences

  float    buttonholeCornerLength {}; // buttonhole fill corner length
  float    picotSpace {};             // space between border picots
  int8_t   hoopType {};               // hoop type
  int8_t   auxFileType {};            // machine file type
  float    hoopSizeX {};              // hoop x size
  float    hoopSizeY {};              // hoop y size
  float    rotationAngle {};          // rotation angle
  float    gridSize {};               // grid size
  float    clipOffset {};             // clipboard offset
  RECT     initialWindowCoords {};    // initial window coordinates
  COLORREF gridColor {};              // grid color
  uint32_t fillPhase {};              // clipboard fill phase
  float    customHoopX {};            // custom hoop width
  float    customHoopY {};            // custom hoop height
  float    traceLength {};            // lens points
  float    traceRatio {};             // trace ratio
  float    chainSpace {};             // chain space
  float    chainRatio {};             // chain ratio
  float    lensRatio {};              // lens aspect ratio
  float    AppStitchLen {};           // Applique stitch length
  float    cursorNudgeStep {};        // cursor nudge step
  uint16_t nudgePixels {};            // nudge pixels
  float    eggRatio {};               // egg ratio
  uint16_t stitchSizePixels {};       // size of stitch points in pixels
  uint16_t formVertexSizePixels {};   // size of form points in pixels
  uint16_t formSides {};              // sides of a created form
  float    tearTailLength {};         // length of the tear tail
  float    tearTwistStep {};          // tear twist step
  float    tearTwistRatio {};         // tear twist ratio
  uint16_t wavePoints {};             // wave points
  uint16_t waveStart {};              // wave strting point
  uint16_t waveEnd {};                // wave ending point;
  uint16_t waveLobes {};              // wave lobes
  uint8_t  featherFillType {};        // feather fill type
  uint8_t  featherUpCount {};         // feather up count
  uint8_t  featherDownCount {};       // feather down count
  uint8_t  featherType {};            // feather bits
  float    featherRatio {};           // feather ratio
  float    featherMinStitchSize {};   // feather floor
  uint16_t featherCount {};           // feather fill psg granularity

  std::array<char, MAX_PATH> p2cName {}; // pes2card file

  float    underlayIndent {};    // edge walk/underlay indent
  float    underlayAngle {};     // underlay angle
  float    underlaySpacing {};   // underlay spacing
  float    underlayStitchLen {}; // underlay stitch length
  float    daisyDiameter {};     // daisy diameter
  float    daisyPetalLen {};     // daisy petal length
  float    daisyHoleDiameter {}; // daisy hole diameter
  uint32_t daisyPetalCount {};   // daisy petals
  uint32_t daisyPetalPoints {};  // daisy petal points
  uint32_t daisyInnerCount {};   // daisy inner count
  uint8_t  daisyBorderType {};   // daisy border type
  uint8_t  dataCheck {};         // data check
  float    textureHeight {};     // textured fill height
  float    textureWidth {};      // textured fill width
  float    textureSpacing {};    // textured fill spacing
  uint16_t formBoxSizePixels {}; // form box pixels
  uint16_t daisyHeartCount {};   // daisy heart count
  uint16_t textureEditorSize {}; // texture editor pixels
  float    clpspc {};            // clipboard fill spacing

  std::array<char, NAME_LEN> designerName {}; // designer name
  // NOLINTEND(misc-non-private-member-variables-in-classes)

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

constexpr auto FDEFRAT = 0.6F;           // default feather ratio
constexpr auto FDEFUP  = uint8_t {10U};  // default feather up count
constexpr auto FDEFDWN = uint8_t {5U};   // default feather down count
constexpr auto FDEFFLR = 9.0F;           // default feather floor
constexpr auto FDEFNUM = uint16_t {10U}; // default feather number

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

#endif // THRED_INIFILE_H
