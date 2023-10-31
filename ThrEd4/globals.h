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

constexpr auto UNDOLEN = 16U; // UndoBuffer length

class FORM_VERTICES
{
  public:
  uint32_t start {};
  uint32_t vertexCount {};
  uint32_t finish {};
  uint32_t form {};

  // constexpr FORM_VERTICES() noexcept = default;
  // FORM_VERTICES(FORM_VERTICES const&) = default;
  // FORM_VERTICES(FORM_VERTICES&&) = default;
  // FORM_VERTICES& operator=(FORM_VERTICES const& rhs) = default;
  // FORM_VERTICES& operator=(FORM_VERTICES&&) = default;
  //~FORM_VERTICES() = default;
};

class F_LSIZ
{
  public:
  float cx {};
  float cy {};

  // constexpr F_LSIZ() noexcept = default;
  // F_LSIZ(F_LSIZ const&) = default;
  // F_LSIZ(F_LSIZ&&) = default;
  // F_LSIZ& operator=(F_LSIZ const& rhs) = default;
  // F_LSIZ& operator=(F_LSIZ&&) = default;
  //~F_LSIZ() = default;
};

namespace fs = std::filesystem;

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
extern MSG   Msg;

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
extern float SpiralWrap;
extern float StarRatio;

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

extern std::vector<std::unique_ptr<uint32_t[]>>* UndoBuffer; // NOLINT(modernize-avoid-c-arrays)

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
