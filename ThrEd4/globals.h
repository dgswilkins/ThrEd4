#pragma once

// We disable 26446 as operator[] is bounds checked for vectors in the debug version
#pragma warning(disable : 26446) // bounds.4     Prefer to use gsl::at() instead of unchecked subscript operator

// Local Headers
#include "clipStitch.h"
#include "EnumMap.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "iniFile.h"
#include "textureHeader.h"

// Open Source headers
#include "boost/dynamic_bitset.hpp"

// Standard Libraries
#include <filesystem>

constexpr auto OLDVER   = wchar_t {4}; // number of old file versions kept
constexpr auto RES_SIZE = 26;          // reserved for expansion in the ThrEd v1.0 header
constexpr auto UNDOLEN  = 16U;         // UndoBuffer length

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
  std::vector<F_POINT>      angledFormVertices;
  fs::path                  auxName;
  std::vector<B_SEQ_PNT>    bSequence;
  std::vector<HWND>         buttonWin;
  std::vector<F_POINT_ATTR> clipBuffer;
  std::vector<F_POINT>      clipPoints;
  int32_t                   DPI = 0;
  std::vector<float>        formAngles;
  std::vector<POINT>        formLines;
  std::vector<FRM_HEAD>     FormList;
  std::wstring              formOnOff;
  std::vector<F_POINT>      FormVertices;
  std::vector<F_POINT>      insidePointList;
  std::vector<F_POINT>      InterleaveSequence;
  std::vector<INS_REC>      interleaveSequenceIndices;

  std::vector<F_POINT>      OSequence;
  std::vector<F_POINT>      outsidePointList;
  std::vector<POINT>        rubberBandLine;
  std::vector<SAT_CON>      satinGuides;
  std::vector<POINT>        searchLine;
  std::vector<uint32_t>     selectedFormList;
  std::vector<POINT>        selectedFormsLine;
  std::vector<POINT>        selectedPointsLine;
  ENUM_MAP<StateFlag>       StateMap = ENUM_MAP<StateFlag> {0};
  std::vector<F_POINT_ATTR> StitchBuffer;
  std::vector<TX_HIST>      TextureHistory;
  std::vector<TX_PNT>       TexturePointsBuffer;
  fs::path                  thrName;
  boost::dynamic_bitset<>   tracedEdges;
  boost::dynamic_bitset<>   TracedMap;

  std::array<std::vector<uint32_t>, UNDOLEN> UndoBuffer;

  ENUM_MAP<UserFlag> userFlagMap = ENUM_MAP<UserFlag> {0};
  std::vector<HWND>  valueWindow;
  fs::path           WorkingFileName;
  std::wstring       textureInputBuffer;
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
extern uint8_t    ActiveColor;
extern uint8_t    ActiveLayer;

extern uint8_t AppliqueColor;

extern COLORREF BackgroundColor;
extern float    BorderWidth;
extern int32_t  ButtonHeight;
extern int32_t  ButtonWidth;
extern int32_t  ButtonWidthX3;

extern float    ButtonholeCornerLength;
extern HWND     CancelButton;
extern uint32_t Clip;

extern HGLOBAL ClipMemory;

extern F_LSIZ   ClipRectSize;
extern uint32_t ClosestFormToCursor;
extern uint32_t ClosestPointIndex;
extern uint32_t ClosestVertexToCursor;

extern HWND DeleteStitchesDialog;

extern HWND            DiscardButton;
extern DRAWITEMSTRUCT* DrawItem;
extern POINT           EndPointCross;

extern HWND FormDataSheet;

extern uint32_t FormMenuChoice;
extern F_POINT  FormMoveDelta;

extern HPEN FormPen3px;
extern HPEN FormPen;
extern HPEN FormSelectedPen;

extern HWND     GeneralNumberInputBox;
extern HPEN     GridPen;
extern uint32_t GroupEndStitch;
extern uint32_t GroupStartStitch;
extern uint32_t GroupStitchIndex;
extern float    HorizontalRatio;
extern HWND     HorizontalScrollBar;
extern INI_FILE IniFile;

extern std::array<POINT, 3>  InsertLine;
extern std::vector<F_POINT>* InsidePoints;

extern F_POINT LastPoint;
extern float   LineSpacing;
extern F_POINT LowerLeftStitch;

extern HMENU MainMenu;
extern HWND  MainStitchWin;
extern float MaxStitchLen;

extern float MinStitchLength;
extern MSG   WinMsg;

extern RECT     MsgRect;
extern HWND     MsgWindow;
extern HPEN     MultiFormPen;
extern uint32_t NearestCount;
extern uint32_t NewFormVertexCount;
extern HWND     OKButton;

extern bool OutLineEverySelectedForm;

extern std::vector<F_POINT>* OutsidePoints;

extern wchar_t const* PcdClipFormat;
extern float          PicotSpacing;
extern uint32_t       PreferenceIndex;
extern HWND           PreferencesWindow;
extern uint32_t       PreviousFormIndex;
extern uint32_t       PseudoRandomValue;
extern F_RECTANGLE    RotationRect;

extern HPEN     SelectAllPen;
extern uint32_t SelectedFormControlVertex;

extern FORM_VERTICES SelectedFormVertices;

extern RECT    SelectedFormsRect;
extern F_POINT SelectedFormsSize;
extern RECT    SelectedPixelsRect;

extern F_RECTANGLE SelectedVerticesRect;

extern float ShowStitchThreshold;
extern HWND  SideMessageWindow;
extern float SmallStitchLength;
extern float SnapLength;

extern uint32_t SortIndex;
extern float    SpiralWrap;
extern float    StarRatio;

extern F_POINT StitchBmpRatio;
extern float   StitchBoxesThreshold;

extern F_RECTANGLE StitchRangeRect;
extern RECT        StitchWindowClientRect;
extern HDC         StitchWindowDC;
extern HDC         StitchWindowMemDC;
extern POINT       StitchWindowOrigin;
extern SIZE        StitchWindowSize;

extern wchar_t const* ThrEdClipFormat;
extern HINSTANCE      ThrEdInstance;
extern HWND           ThrEdWindow;
extern POINT          ThredWindowOrigin;
extern uint32_t*      TraceBitmapData;

extern SIZE UnzoomedRect;

extern std::array<COLORREF, COLORCNT> UserColor;

extern float UserStitchLength;

extern float VerticalRatio;
extern HWND  VerticalScrollBar;

extern std::array<POINT, SQPNTS> ZoomBoxLine;

extern F_POINT     ZoomBoxOrigin;
extern float       ZoomFactor;
extern F_POINT     ZoomMarkPoint;
extern F_POINT     ZoomRatio;
extern F_RECTANGLE ZoomRect;
