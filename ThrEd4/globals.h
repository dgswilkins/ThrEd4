#pragma once

// We disable 26446 as operator[] is bounds checked for vectors in the debug version
#pragma warning(disable : 26446) // bounds.4     Prefer to use gsl::at() instead of unchecked subscript operator

// Local Headers
#include "EnumMap.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "iniFile.h"
#include "textureHeader.h"

// Standard Libraries
#include <filesystem>

constexpr auto OLDVER   = wchar_t {4}; // number of old file versions kept
constexpr auto RES_SIZE = 26;          // reserved for expansion in the ThrEd v1.0 header
constexpr auto UNDOLEN  = 16U;         // undoBuffer length

class FORM_VERTICES
{
  public:
  uint32_t start {};
  uint32_t vertexCount {};
  uint32_t finish {};
  uint32_t form {};
};

class F_LSIZ
{
  public:
  float cx {};
  float cy {};
};

class COL_CHANGE
{
  public:
  uint16_t stitchIndex {}; // stitch index
  uint16_t colorIndex {};  // color index
};

#pragma pack(push, 1)
class THR_HEAD_EX // ThrEd v1.0 file header extension
{
  public:
  float                        hoopSizeX {};         // hoop size x dimension
  float                        hoopSizeY {};         // hoop size y dimension
  float                        stgran {};            // stitches per millimeter
  std::array<char, NAME_LEN>   creatorName {};       // name of the file creator
  std::array<char, NAME_LEN>   modifierName {};      // name of last file modifier
  int8_t                       auxFormat {};         // auxillary file format
  int8_t                       stres {};             // reserved
  uint32_t                     texturePointCount {}; // textured fill point count
  std::array<int8_t, RES_SIZE> res {};               // reserved for expansion
};
#pragma pack(pop)

namespace fs = std::filesystem;

class MY_SINGLE
{
  public:
  static auto getInstance() -> MY_SINGLE* {
	// NOLINTNEXTLINE(clang-diagnostic-exit-time-destructors)
	static MY_SINGLE instance;

	return &instance;
  }

  void initialize() {
	rubberBandLine.resize(3U);
	selectedFormsLine.resize(OUTPNTS);
	selectedPointsLine.resize(OUTPNTS);
	valueWindow.resize(LASTLIN);
  }

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  int32_t                   DPI = 0;
  std::vector<F_POINT>      angledFormVertices;
  fs::path                  auxName;
  float                     borderWidth = BRDWID; // border width for satin borders
  std::vector<B_SEQ_PNT>    bSequence;
  std::vector<HWND>         buttonWin;
  std::vector<F_POINT_ATTR> clipBuffer;
  std::vector<F_POINT>      clipPoints;
  std::vector<float>        formAngles;
  std::vector<POINT>        formLines;
  std::vector<FRM_HEAD>     formList;
  std::wstring              formOnOff;
  std::vector<F_POINT>      formVertices;
  std::vector<F_POINT>      insidePointList;
  std::vector<F_POINT>      interleaveSequence;
  std::vector<INS_REC>      interleaveSequenceIndices;
  std::vector<F_POINT>      oSequence;
  std::vector<F_POINT>      outsidePointList;
  std::vector<POINT>        rubberBandLine;
  std::vector<SAT_CON>      satinGuides;
  std::vector<POINT>        searchLine;
  std::vector<uint32_t>     selectedFormList;
  std::vector<POINT>        selectedFormsLine;
  std::vector<POINT>        selectedPointsLine;
  ENUM_MAP<StateFlag>       stateMap = ENUM_MAP<StateFlag> {0};
  std::vector<F_POINT_ATTR> stitchBuffer;
  std::wstring              textureInputBuffer;
  std::vector<TX_PNT>       texturePointsBuffer;

  std::array<std::vector<uint32_t>, UNDOLEN> undoBuffer;

  ENUM_MAP<UserFlag> userFlagMap = ENUM_MAP<UserFlag> {0};
  std::vector<HWND>  valueWindow;
  fs::path           workingFileName;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  MY_SINGLE(const MY_SINGLE&)                    = delete;
  auto operator=(const MY_SINGLE&) -> MY_SINGLE& = delete;
  MY_SINGLE(MY_SINGLE&&)                         = delete;
  auto operator=(MY_SINGLE&&) -> MY_SINGLE&      = delete;

  private:
  MY_SINGLE()  = default;
  ~MY_SINGLE() = default;
};

extern MY_SINGLE* Instance;

extern uint8_t  ActiveColor;
extern uint8_t  ActiveLayer;
extern uint8_t  AppliqueColor;
extern int32_t  ButtonHeight;
extern int32_t  ButtonWidth;
extern int32_t  ButtonWidthX3;
extern float    ButtonholeCornerLength;
extern F_LSIZ   ClipRectSize;
extern uint32_t ClosestFormToCursor;
extern uint32_t ClosestPointIndex;
extern uint32_t ClosestVertexToCursor;
extern POINT    EndPointCross;
extern F_POINT  FormMoveDelta;
extern HPEN     FormPen3px;
extern HPEN     FormPen;
extern uint32_t GroupEndStitch;
extern uint32_t GroupStartStitch;
extern uint32_t GroupStitchIndex;
extern INI_FILE IniFile;

extern std::array<POINT, 3>  InsertLine;
extern std::vector<F_POINT>* InsidePoints;

extern F_POINT  LastPoint;
extern float    LineSpacing;
extern HWND     MainStitchWin;
extern float    MinStitchLength;
extern MSG      WinMsg;
extern RECT     MsgRect;
extern HWND     MsgWindow;
extern uint32_t NewFormVertexCount;

extern std::vector<F_POINT>* OutsidePoints;

extern float       PicotSpacing;
extern uint32_t    PseudoRandomValue;
extern F_RECTANGLE RotationRect;

extern uint32_t SelectedFormControlVertex;

extern FORM_VERTICES SelectedFormVertices;

extern RECT    SelectedFormsRect;
extern F_POINT SelectedFormsSize;
extern RECT    SelectedPixelsRect;

extern F_RECTANGLE SelectedVerticesRect;

extern HWND  SideMessageWindow;
extern float SnapLength;
extern float SpiralWrap;
extern float StarRatio;
extern F_POINT StitchBmpRatio;

extern F_RECTANGLE StitchRangeRect;

extern RECT      StitchWindowClientRect;
extern HDC       StitchWindowDC;
extern HDC       StitchWindowMemDC;
extern POINT     StitchWindowOrigin;
extern HINSTANCE ThrEdInstance;
extern HWND      ThrEdWindow;
extern POINT     ThredWindowOrigin;
extern uint32_t* TraceBitmapData;
extern SIZE      UnzoomedRect;

extern std::array<COLORREF, COLORCNT> UserColor;

extern float UserStitchLength;

extern std::array<POINT, SQPNTS> ZoomBoxLine;

extern F_POINT     ZoomBoxOrigin;
extern float       ZoomFactor;
extern F_POINT     ZoomMarkPoint;
extern F_POINT     ZoomRatio;
extern F_RECTANGLE ZoomRect;
