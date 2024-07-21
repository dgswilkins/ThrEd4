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

constexpr auto RES_SIZE = 26;  // reserved for expansion in the ThrEd v1.0 header
constexpr auto UNDOLEN = 16U; // UndoBuffer length

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

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  F_RECTANGLE               privateAllItemsRect;
  std::vector<F_POINT>      privateAngledFormVertices;
  fs::path                  privateAuxName;
  std::vector<COLORREF>     privateBitmapBackgroundColor;
  std::vector<B_SEQ_PNT>    privateBSequence;
  fs::path                  privateBalaradName0;
  fs::path                  privateBalaradName1;
  fs::path                  privateBalaradName2;
  std::vector<HWND>         privateButtonWin;
  std::vector<F_POINT_ATTR> privateClipBuffer;
  std::vector<F_POINT>      privateClipPoints;
  int32_t                   privateColorBarSize = 0;
  fs::path                  privateColorFileName;
  std::vector<COL_CHANGE>   privateColorChangeTable;
  std::vector<HWND>         privateDefaultColorWin;
  fs::path                  privateDefaultDirectory;
  std::wstring              privateDesignerName;
  int32_t                   privateDPI = 0;
  THR_HEAD_EX               privateExtendedHeader;
  std::vector<float>        privateFormAngles;
  std::vector<POINT>        privateFormControlPoints;
  std::vector<POINT>        privateFormLines;
  std::vector<FRM_HEAD>     privateFormList;
  std::wstring              privateFormOnOff;
  std::vector<F_POINT>      privateFormVertices;
  std::vector<POINT>        privateFormVerticesAsLine;
  fs::path                  privateHomeDirectory;
  fs::path                  privateIniFileName;
  std::vector<F_POINT>      privateInsidePointList;
  std::vector<F_POINT>      privateInterleaveSequence;
  std::vector<INS_REC>      privateInterleaveSequenceIndices;
  std::vector<uint32_t>     privateKnots;
  std::vector<HWND>         privateLabelWindow;

  std::array<uint32_t, OLDNUM> privateLRUMenuId = {FM_ONAM0, FM_ONAM1, FM_ONAM2, FM_ONAM3}; // recently used file menu ID's

  MENUITEMINFO              privateMenuInfo = MENUITEMINFO {};
  std::vector<wchar_t>      privateMsgBuffer;
  std::vector<POINT>        privateNearestPixel;
  std::vector<uint32_t>     privateNearestPoint;
  std::vector<F_POINT>      privateOSequence;
  std::vector<F_POINT>      privateOutsidePointList;
  std::vector<fs::path>     privatePreviousNames;
  fs::path                  privateRGBFileName;
  std::vector<POINT>        privateRubberBandLine;
  std::vector<SAT_CON>      privateSatinGuides;
  int32_t                   privateScrollSize = 0;
  std::vector<POINT>        privateSearchLine;
  std::vector<uint32_t>     privateSelectedFormList;
  std::vector<POINT>        privateSelectedFormsLine;
  std::vector<POINT>        privateSelectedPointsLine;
  std::vector<uint32_t>     privateSelectedTexturePointsList;
  std::vector<HWND>         privateSideWindow;
  std::vector<wchar_t>      privateSideWindowEntryBuffer;
  std::vector<SEARCH_REC>   privateSortBuffer;
  ENUM_MAP<StateFlag>       privateStateMap = ENUM_MAP<StateFlag> {0};
  std::vector<F_POINT_ATTR> privateStitchBuffer;
  std::vector<F_POINT>      privateTempPolygon;
  std::vector<TX_PNT>       privateTempTexturePoints;
  std::vector<TX_HIST>      privateTextureHistory;
  std::vector<TX_PNT>       privateTexturePointsBuffer;
  fs::path                  privateThrName;
  std::vector<HWND>         privateThreadSizeWin;
  std::vector<wchar_t>      privateThumbnailSearchString;
  std::vector<std::wstring> privateThumbnails;
  boost::dynamic_bitset<>   privateTracedEdges;
  boost::dynamic_bitset<>   privateTracedMap;

  std::array<std::vector<uint32_t>, UNDOLEN> privateUndoBuffer;

  fs::path              privateUserBMPFileName;
  std::vector<HWND>     privateUserColorWin;
  ENUM_MAP<UserFlag>    privateUserFlagMap = ENUM_MAP<UserFlag> {0};
  std::vector<HPEN>     privateUserPen;
  std::vector<HWND>     privateValueWindow;
  std::vector<fs::path> privateVersionNames;
  fs::path              privateWorkingFileName;
  std::wstring          privatetextureInputBuffer;
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

extern MY_SINGLE*   Instance;
extern uint8_t      ActiveColor;
extern uint8_t      ActiveLayer;
extern F_RECTANGLE* AllItemsRect;

extern std::vector<F_POINT>* AngledFormVertices;

extern uint8_t   AppliqueColor;
extern fs::path* AuxName;

extern std::vector<B_SEQ_PNT>* BSequence;

extern COLORREF BackgroundColor;
extern float    BorderWidth;
extern int32_t  ButtonHeight;
extern int32_t  ButtonWidth;
extern int32_t  ButtonWidthX3;

extern std::vector<HWND>* ButtonWin;

extern float    ButtonholeCornerLength;
extern HWND     CancelButton;
extern uint32_t Clip;

extern std::vector<F_POINT_ATTR>* ClipBuffer;

extern HGLOBAL ClipMemory;
extern void*   ClipPointer;

extern std::vector<F_POINT>* ClipPoints;

extern F_LSIZ       ClipRectSize;
extern CLIP_STITCH* ClipStitchData;
extern uint32_t     ClosestFormToCursor;
extern uint32_t     ClosestPointIndex;
extern uint32_t     ClosestVertexToCursor;

extern HWND DeleteStitchesDialog;

extern HWND            DiscardButton;
extern DRAWITEMSTRUCT* DrawItem;
extern POINT           EndPointCross;

extern std::vector<float>* FormAngles;

extern HWND FormDataSheet;

extern std::vector<POINT>*    FormLines;
extern std::vector<FRM_HEAD>* FormList;

extern uint32_t FormMenuChoice;
extern F_POINT  FormMoveDelta;

extern std::wstring* FormOnOff;

extern HPEN FormPen3px;
extern HPEN FormPen;
extern HPEN FormSelectedPen;

extern std::vector<F_POINT>* FormVertices;

extern HWND     GeneralNumberInputBox;
extern HPEN     GridPen;
extern uint32_t GroupEndStitch;
extern uint32_t GroupStartStitch;
extern uint32_t GroupStitchIndex;
extern float    HorizontalRatio;
extern HWND     HorizontalScrollBar;
extern INI_FILE IniFile;

extern std::array<POINT, 3>  InsertLine;
extern std::vector<F_POINT>* InsidePointList;
extern std::vector<F_POINT>* InsidePoints;
extern std::vector<INS_REC>* InterleaveSequenceIndices;
extern std::vector<F_POINT>* InterleaveSequence;
extern std::vector<HWND>*    LabelWindow;

extern F_POINT LastPoint;
extern float   LineSpacing;
extern F_POINT LowerLeftStitch;

extern std::array<uint32_t, OLDNUM>* LRUPtr;

extern HMENU MainMenu;
extern HWND  MainStitchWin;
extern float MaxStitchLen;

extern MENUITEMINFO* MenuInfo;

extern float MinStitchLength;
extern MSG   WinMsg;

extern RECT     MsgRect;
extern HWND     MsgWindow;
extern HPEN     MultiFormPen;
extern uint32_t NearestCount;
extern uint32_t NewFormVertexCount;
extern HWND     OKButton;

extern std::vector<F_POINT>* OSequence;

extern bool OutLineEverySelectedForm;

extern std::vector<F_POINT>* OutsidePointList;
extern std::vector<F_POINT>* OutsidePoints;

extern wchar_t const* PcdClipFormat;
extern float          PicotSpacing;
extern uint32_t       PreferenceIndex;
extern int32_t        PreferenceWindowWidth;
extern HWND           PreferencesWindow;
extern uint32_t       PreviousFormIndex;
extern uint32_t       PseudoRandomValue;
extern F_RECTANGLE    RotationRect;

extern std::vector<POINT>* RubberBandLine;

extern std::vector<SAT_CON>* SatinGuides;

extern int32_t* ScreenDPI;
extern int32_t* ScrollSize;

extern std::vector<POINT>* SearchLine;

extern HPEN     SelectAllPen;
extern uint32_t SelectedFormControlVertex;

extern std::vector<uint32_t>* SelectedFormList;
extern FORM_VERTICES          SelectedFormVertices;
extern std::vector<POINT>*    SelectedFormsLine;

extern RECT    SelectedFormsRect;
extern F_POINT SelectedFormsSize;
extern RECT    SelectedPixelsRect;

extern std::vector<POINT>* SelectedPointsLine;

extern F_RECTANGLE SelectedVerticesRect;

extern float ShowStitchThreshold;
extern HWND  SideMessageWindow;
extern float SmallStitchLength;
extern float SnapLength;

extern std::vector<SEARCH_REC>* SortBuffer;

extern uint32_t SortIndex;
extern float    SpiralWrap;
extern float    StarRatio;

extern ENUM_MAP<StateFlag>* StateMap;

extern F_POINT StitchBmpRatio;
extern float   StitchBoxesThreshold;

extern std::vector<F_POINT_ATTR>* StitchBuffer;

extern F_RECTANGLE StitchRangeRect;
extern RECT        StitchWindowClientRect;
extern HDC         StitchWindowDC;
extern HDC         StitchWindowMemDC;
extern POINT       StitchWindowOrigin;
extern SIZE        StitchWindowSize;

extern std::vector<F_POINT>* TempPolygon;
extern std::wstring*         TextureInputBuffer;
extern std::vector<TX_PNT>*  TexturePointsBuffer;

extern wchar_t const* ThrEdClipFormat;
extern HINSTANCE      ThrEdInstance;
extern HWND           ThrEdWindow;
extern fs::path*      ThrName;
extern POINT          ThredWindowOrigin;
extern uint32_t*      TraceBitmapData;

extern boost::dynamic_bitset<>* TracedEdges;
extern boost::dynamic_bitset<>* TracedMap;

extern SIZE UnzoomedRect;

extern std::array<std::vector<uint32_t>, UNDOLEN>* UndoBuffer;

extern std::array<COLORREF, COLORCNT> UserColor;

extern ENUM_MAP<UserFlag>* UserFlagMap;

extern float UserStitchLength;

extern std::vector<HWND>* ValueWindow;

extern float     VerticalRatio;
extern HWND      VerticalScrollBar;
extern fs::path* WorkingFileName;

extern std::array<POINT, SQPNTS> ZoomBoxLine;

extern F_POINT     ZoomBoxOrigin;
extern float       ZoomFactor;
extern F_POINT     ZoomMarkPoint;
extern F_POINT     ZoomRatio;
extern F_RECTANGLE ZoomRect;
