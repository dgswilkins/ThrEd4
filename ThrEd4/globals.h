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
	constexpr auto COLSIZ   = int32_t {12};     // logical pixel width of the color bar
	constexpr auto MSGSIZ   = uint32_t {8192U}; // size of the message buffer
	constexpr auto NERCNT   = 4U;               // number of entries in the near array;
	constexpr auto SCROLSIZ = int32_t {12};     // logical pixel width of a scroll bar
	constexpr auto SWCOUNT  = 16U;              // number of side windows to create/track
	constexpr auto SWBLEN   = 11U; // Side Window buffer length including the zero terminator
	constexpr auto ITXBUFSZ = uint32_t {16U}; // texture buffer depth
	constexpr auto TSSSIZ   = size_t {32U};   // size of the message buffer
	ColorBarSize            = COLSIZ;
	FormControlPoints.resize(OUTPNTS);
	DefaultColorWin.resize(COLORCNT);
	LabelWindow.resize(LASTLIN);
	MsgBuffer.reserve(MSGSIZ);
	NearestPixel.resize(NERCNT);
	NearestPoint.resize(NERCNT);
	RubberBandLine.resize(3U);
	ScrollSize = SCROLSIZ;
	SelectedFormsLine.resize(OUTPNTS);
	SelectedPointsLine.resize(OUTPNTS);
	SideWindow.resize(SWCOUNT);
	SideWindowEntryBuffer.resize(SWBLEN);
	TextureHistory.resize(ITXBUFSZ);
	ThreadSizeWin.resize(COLORCNT);
	ThumbnailSearchString.reserve(TSSSIZ);
	UserColorWin.resize(COLORCNT);
	UserPen.resize(COLORCNT);
	ValueWindow.resize(LASTLIN);
	PreviousNames.reserve(OLDNUM);
	VersionNames.reserve(OLDVER);
	for (auto iVersion = 0U; iVersion < OLDNUM; ++iVersion) {
	  PreviousNames.emplace_back(L"");
	}
	for (auto iVersion = wchar_t {}; iVersion < OLDVER; ++iVersion) {
	  VersionNames.emplace_back(L"");
	}
  }

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  F_RECTANGLE               AllItemsRect;
  std::vector<F_POINT>      AngledFormVertices;
  fs::path                  AuxName;
  std::vector<COLORREF>     BitmapBackgroundColor;
  std::vector<B_SEQ_PNT>    BSequence;
  fs::path                  BalaradName0;
  fs::path                  BalaradName1;
  fs::path                  BalaradName2;
  std::vector<HWND>         ButtonWin;
  std::vector<F_POINT_ATTR> ClipBuffer;
  std::vector<F_POINT>      ClipPoints;
  int32_t                   ColorBarSize = 0;
  fs::path                  ColorFileName;
  std::vector<COL_CHANGE>   ColorChangeTable;
  std::vector<HWND>         DefaultColorWin;
  fs::path                  DefaultDirectory;
  std::wstring              DesignerName;
  int32_t                   DPI = 0;
  THR_HEAD_EX               ExtendedHeader;
  std::vector<float>        FormAngles;
  std::vector<POINT>        FormControlPoints;
  std::vector<POINT>        FormLines;
  std::vector<FRM_HEAD>     FormList;
  std::wstring              FormOnOff;
  std::vector<F_POINT>      FormVertices;
  std::vector<POINT>        FormVerticesAsLine;
  fs::path                  HomeDirectory;
  fs::path                  IniFileName;
  std::vector<F_POINT>      InsidePointList;
  std::vector<F_POINT>      InterleaveSequence;
  std::vector<INS_REC>      InterleaveSequenceIndices;
  std::vector<uint32_t>     Knots;
  std::vector<HWND>         LabelWindow;

  std::array<uint32_t, OLDNUM> LRUMenuId = {FM_ONAM0, FM_ONAM1, FM_ONAM2, FM_ONAM3}; // recently used file menu ID's

  MENUITEMINFO              MenuInfo = MENUITEMINFO {};
  std::vector<wchar_t>      MsgBuffer;
  std::vector<POINT>        NearestPixel;
  std::vector<uint32_t>     NearestPoint;
  std::vector<F_POINT>      OSequence;
  std::vector<F_POINT>      OutsidePointList;
  std::vector<fs::path>     PreviousNames;
  fs::path                  RGBFileName;
  std::vector<POINT>        RubberBandLine;
  std::vector<SAT_CON>      SatinGuides;
  int32_t                   ScrollSize = 0;
  std::vector<POINT>        SearchLine;
  std::vector<uint32_t>     SelectedFormList;
  std::vector<POINT>        SelectedFormsLine;
  std::vector<POINT>        SelectedPointsLine;
  std::vector<uint32_t>     SelectedTexturePointsList;
  std::vector<HWND>         SideWindow;
  std::vector<wchar_t>      SideWindowEntryBuffer;
  std::vector<SEARCH_REC>   SortBuffer;
  ENUM_MAP<StateFlag>       StateMap = ENUM_MAP<StateFlag> {0};
  std::vector<F_POINT_ATTR> StitchBuffer;
  std::vector<F_POINT>      TempPolygon;
  std::vector<TX_PNT>       TempTexturePoints;
  std::vector<TX_HIST>      TextureHistory;
  std::vector<TX_PNT>       TexturePointsBuffer;
  fs::path                  ThrName;
  std::vector<HWND>         ThreadSizeWin;
  std::vector<wchar_t>      ThumbnailSearchString;
  std::vector<std::wstring> Thumbnails;
  boost::dynamic_bitset<>   TracedEdges;
  boost::dynamic_bitset<>   TracedMap;

  std::array<std::vector<uint32_t>, UNDOLEN> UndoBuffer;

  fs::path              UserBMPFileName;
  std::vector<HWND>     UserColorWin;
  ENUM_MAP<UserFlag>    UserFlagMap = ENUM_MAP<UserFlag> {0};
  std::vector<HPEN>     UserPen;
  std::vector<HWND>     ValueWindow;
  std::vector<fs::path> VersionNames;
  fs::path              WorkingFileName;
  std::wstring          textureInputBuffer;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  MY_SINGLE(const MY_SINGLE&)                    = delete;
  auto operator=(const MY_SINGLE&) -> MY_SINGLE& = delete;
  MY_SINGLE(MY_SINGLE&&)                         = delete;
  auto operator=(MY_SINGLE&&) -> MY_SINGLE&      = delete;

  private:
  MY_SINGLE()  = default;
  ~MY_SINGLE() = default;

  // file menu items
  enum FileMenuItems : uint8_t {
	FM_NEW,
	FM_OPEN,
	FM_CLOS,
	FM_THUMB,
	FM_OPNPCS,
	FM_INSRT,
	FM_OVRLAY,
	FM_SAV,
	FM_SAVAS,
	FM_LODBIT,
	FM_SAVBIT,
	FM_HIDBIT,
	FM_RMVBIT,
	FM_PURG,
	FM_LOCK,
	FM_ONAM0,
	FM_ONAM1,
	FM_ONAM2,
	FM_ONAM3
  };
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
extern void*   ClipPointer;

extern F_LSIZ       ClipRectSize;
extern CLIP_STITCH* ClipStitchData;
extern uint32_t     ClosestFormToCursor;
extern uint32_t     ClosestPointIndex;
extern uint32_t     ClosestVertexToCursor;

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
extern std::vector<HWND>*    LabelWindow;

extern F_POINT LastPoint;
extern float   LineSpacing;
extern F_POINT LowerLeftStitch;

extern std::array<uint32_t, OLDNUM>* LRUPtr;

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
extern int32_t        PreferenceWindowWidth;
extern HWND           PreferencesWindow;
extern uint32_t       PreviousFormIndex;
extern uint32_t       PseudoRandomValue;
extern F_RECTANGLE    RotationRect;

extern int32_t* ScreenDPI;
extern int32_t* ScrollSize;

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

extern std::vector<SEARCH_REC>* SortBuffer;

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

extern ENUM_MAP<UserFlag>* UserFlagMap;

extern float UserStitchLength;

extern float VerticalRatio;
extern HWND  VerticalScrollBar;

extern std::array<POINT, SQPNTS> ZoomBoxLine;

extern F_POINT     ZoomBoxOrigin;
extern float       ZoomFactor;
extern F_POINT     ZoomMarkPoint;
extern F_POINT     ZoomRatio;
extern F_RECTANGLE ZoomRect;
