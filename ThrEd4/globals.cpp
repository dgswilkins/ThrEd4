#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <filesystem>

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "globals.h"
#include "ThrEdTypes.h"
#include "EnumMap.h"

namespace fs = std::filesystem;

// clang-format off
uint8_t                        ActiveColor               = 0U;      // active color selector
uint8_t                        ActiveLayer               = 0U;      // active layer
fRECTANGLE*                    AllItemsRect              = nullptr; // rectangle enclosing all forms and stitches
std::vector<fPOINT>*           AngledFormVertices        = nullptr; // form formOrigin data for angle fills
uint8_t                        AppliqueColor             = COLORCNT - 1U; // underlay color
fs::path*                      AuxName                   = nullptr; //
std::vector<BSEQPNT>*          BSequence                 = nullptr; //
COLORREF                       BackgroundColor           = 0U;      // stitch window background
float                          BorderWidth               = BRDWID;  // border width for satin borders
int32_t                        ButtonHeight              = 0;       // button height
int32_t                        ButtonWidth               = 0;       // button width
int32_t                        ButtonWidthX3             = 0;       // button width times 3
std::vector<HWND>*             ButtonWin                 = nullptr; // button windows
float                          ButtonholeCornerLength    = IBFCLEN; // buttonhole corner length
HWND                           CancelButton              = nullptr; // cancel button
uint32_t                       Clip                      = 0U;      // pcs format clipboard handle
std::vector<fPOINTATTR>*       ClipBuffer                = nullptr; //
HGLOBAL                        ClipMemory                = nullptr; // handle to the clipboard memory
void*                          ClipPointer               = nullptr; // for memory allocation for clipboard data
std::vector<fPOINT>*           ClipPoints                = nullptr; // main clipboard fill points for forms
FLSIZ                          ClipRectSize              = {};      // clipboard rectangle size
CLPSTCH*                       ClipStitchData            = nullptr; // for pcs clipboard data
uint32_t                       ClipTypeMap               = MCLPF | MVCLPF | MHCLPF | MANGCLPF; // for checking if a fill is a clipboard fill
uint32_t                       ClosestFormToCursor       = 0U;      // closest form to the cursor
uint32_t                       ClosestPointIndex         = 0U;      // index of closest point
uint32_t                       ClosestVertexToCursor     = 0U;      // formOrigin closest to the cursor
int32_t*                       ColorBarSize              = nullptr; // Color bar width scaled for DPI
HWND                           DeleteStitchesDialog      = nullptr; // asking user if they want to delete the sitches associated with a form
std::wstring*                  DesignerName              = nullptr; // designer name in clear
HWND                           DiscardButton             = nullptr; // discard button
DRAWITEMSTRUCT*                DrawItem                  = nullptr; // for owner-draw windows
POINT                          EndPointCross             = {};      // point to draw at the end cross for form select points
STREX*                         ExtendedHeader            = nullptr; // thred file header extension
HMENU                          FillMenu                  = nullptr; // fill submenu
std::vector<float>*            FormAngles                = nullptr; // angles of a form for satin border fills
std::vector<POINT>*            FormControlPoints         = nullptr; // form control rectangle in pixel coordinates
HWND                           FormDataSheet             = nullptr; // form data sheet
std::vector<POINT>*            FormLines                 = nullptr; // used in the form drawing routines
std::vector<FRMHED>*           FormList                  = nullptr; // a list of form headers
uint32_t                       FormMenuChoice            = 0U;      // data type for form data form numerical entry
fPOINT                         FormMoveDelta             = {};      // offset for moving forms
std::wstring*                  FormOnOff                 = nullptr;
HPEN                           FormPen                   = nullptr; // form pen
HPEN                           FormPen3px                = nullptr; // three-pixel form pen
HPEN                           FormSelectedPen           = nullptr; // form select pen
std::vector<fPOINT>*           FormVertices              = nullptr; // form points
HWND                           GeneralNumberInputBox     = nullptr; // general number input box
HPEN                           GridPen                   = nullptr; // pen for stitch grid
uint32_t                       GroupEndStitch            = 0U;      // higher end of selected stitches
uint32_t                       GroupStartStitch          = 0U;      // lower end of selected stitches
uint32_t                       GroupStitchIndex          = 0U;      // last point selected in group
float                          HorizontalRatio           = 0.0F;    // horizontal ratio between the zoom window and the entire stitch space
HWND                           HorizontalScrollBar       = nullptr; // horizontal scroll bar
INIFILE                        IniFile                   = {};      // initialization file
std::array<POINT, 3>           InsertLine                = {};      // the insert line
std::vector<fPOINT>*           InsidePointList           = nullptr; // list of inside outline points for satin or clipboard fills
std::vector<fPOINT>*           InsidePoints              = nullptr; // pointer to the list of inside outline points
std::vector<INSREC>*           InterleaveSequenceIndices = nullptr; // indices into interleave points
std::vector<fPOINT>*           InterleaveSequence        = nullptr; // storage for interleave points
std::vector<HWND>*             LabelWindow               = nullptr; // text handles for the form data sheet
fPOINT                         LastPoint                 = {};      // last formOrigin written by line connect routine
float                          LineSpacing               = DEFSPACE * PFGRAN; // stitch spacing in stitch units
fPOINT                         LowerLeftStitch           = {};      // lower left formOrigin in a form
HMENU                          MainMenu                  = nullptr; // main menu
HWND                           MainStitchWin             = nullptr; // stitch window
float                          MaxStitchLen              = 0.0F;    // maximum stitch length
MENUITEMINFO*                  MenuInfo                  = nullptr;
float                          MinStitchLength           = MINSIZ * PFGRAN; // minimum stitch size
MSG                            Msg                       = {};      // main message loop message
RECT                           MsgRect                   = {};      // rectangle containing the text message
HWND                           MsgWindow                 = nullptr; // message window
HPEN                           MultiFormPen              = nullptr; // multiple selected forms pen
uint32_t                       NearestCount              = 0U;      // number of boxes selected
uint32_t                       NewFormVertexCount        = 0U;      // points in the new form
HWND                           OKButton                  = nullptr; // ok button
std::vector<fPOINT>*           OSequence                 = nullptr; // temporary storage for sequencing
bool                           OutLineEverySelectedForm  = false;   // When selecting multiple forms, should we outline every form?
std::vector<fPOINT>*           OutsidePointList          = nullptr; // list of outside outline points for satin or clipboard fills
std::vector<fPOINT>*           OutsidePoints             = nullptr; // pointer to the list of outside outline points
wchar_t const*                 PcdClipFormat             = L"PMust_Format";   //
float                          PicotSpacing              = IPICSPAC;// space between border picots
uint32_t                       PreferenceIndex           = 0U;      // index to the active preference window
int32_t                        PreferenceWindowWidth     = {};      // width of the preference window
HWND                           PreferencesWindow         = nullptr; // preferences window
uint32_t                       PreviousFormIndex         = 0U;      // previously selected form
uint32_t                       PseudoRandomValue         = 0U;      // pseudo-random sequence register
fRECTANGLE                     RotationRect              = {};      // rotation rectangle
std::vector<POINT>*            RubberBandLine            = nullptr; // points to form points to be moved
std::vector<SATCON>*           SatinGuides               = nullptr; // satin form connects
RECT                           scRct                     = {};      //
int32_t*                       screenDPI                 = nullptr; // screen DPI
int32_t*                       ScrollSize                = nullptr; // Scroll bar width scaled for DPI
std::vector<POINT>*            SearchLine                = nullptr; // stitch select line
HPEN                           SelectAllPen              = nullptr; // pen for drawing large boxes
uint32_t                       SelectedFormControlVertex = 0U;      // user selected form control formOrigin
std::vector<uint32_t>*         SelectedFormList          = nullptr; // a list of selected forms
FORMVERTICES                   SelectedFormVertices      = {};      // selected form vertices
std::vector<POINT>*            SelectedFormsLine         = nullptr; // line derived from the big rectangle
RECT                           SelectedFormsRect         = {};      // for multiple selections;
fPOINT                         SelectedFormsSize         = {};      // size of multiple select rectangle
RECT                           SelectedPixelsRect        = {};      // display form vertex select rectangle
std::vector<POINT>*            SelectedPointsLine        = nullptr; // line derived from the formOrigin select rectangle
fRECTANGLE                     SelectedVerticesRect      = {};      // rectangle enclosing selected form verticess
float                          ShowStitchThreshold       = SHOPNTS; // show stitch grid below this zoom level
HWND                           SideMessageWindow         = nullptr; // main side message window
float                          SmallStitchLength         = SMALSIZ * PFGRAN; // user can remove stitches smaller than this
float                          SnapLength                = SNPLEN * PFGRAN;  // snap together length
float                          SpiralWrap                = SPIRWRAP;// number of revolutions in a spiral
float                          StarRatio                 = STARAT;  // star formOrigin to body ratio
EnumMap<StateFlag>*            StateMap                  = nullptr; // Flags indicating current run state
fPOINT                         StitchBmpRatio            = {};      // stitch hoop to bitmap ratios
float                          StitchBoxesThreshold      = STCHBOX; // threshold for drawing stitch boxes
std::vector<fPOINTATTR>*       StitchBuffer              = nullptr; // main stitch buffer
fRECTANGLE                     StitchRangeRect           = {};      // stitch range rectangle
RECT                           StitchWindowClientRect    = {};      // stitch window size,client
HDC                            StitchWindowDC            = nullptr; // stitch window device context
HDC                            StitchWindowMemDC         = nullptr; // stitch window memory device context
POINT                          StitchWindowOrigin        = {};      // offset origin of the stitch window
POINT                          StitchWindowSize          = {};      // size of the stitch window in pixels
std::vector<fPOINT>*           TempPolygon               = nullptr; // temporary storage when user is entering a polygon;
std::wstring*                  TextureInputBuffer        = nullptr; // texture fill number buffer
std::vector<TXPNT>*            TexturePointsBuffer       = nullptr; // buffer for textured fill points
std::array<HWND, COLORCNT>     ThreadSizeWin             = {};      // thread size windows
wchar_t const*                 ThrEdClipFormat           = L"threditor"; //
HINSTANCE                      ThrEdInstance             = {};      // main instance handle
HWND                           ThrEdWindow               = nullptr; // main window handle
fs::path*                      ThrName                   = nullptr; //
POINT                          ThredWindowOrigin         = {};      // offset origin of the main window
uint32_t*                      TraceBitmapData           = nullptr; // trace bitmap data
boost::dynamic_bitset<>*       TracedEdges               = nullptr; // detected edges of trace areas
boost::dynamic_bitset<>*       TracedMap                 = nullptr; // in/out state of trace areas
POINT                          UnzoomedRect              = {};      // size of the unzoomed stitch window
std::array<COLORREF, COLORCNT> UserColor                 = {};      // user colors
EnumMap<UserFlag>*             UserFlagMap               = nullptr; // for storage of persistent binary variables set by the user
float                          UserStitchLength          = USESIZ * PFGRAN; // user selected stitch size
std::vector<HWND>*             ValueWindow               = nullptr; // data handles for the form data sheet
float                          VerticalRatio             = 0.0F;    // vertical ratio between the zoom window and the entire stitch space
HWND                           VerticalScrollBar         = nullptr; // vertical scroll bar
fs::path*                      WorkingFileName           = nullptr; //
std::array<POINT, SQPNTS>      ZoomBoxLine               = {};      // the zoom box
fPOINT                         ZoomBoxOrigin             = {};      // zoom box origin
float                          ZoomFactor                = 1.0F;    // zoom factor
fPOINT                         ZoomMarkPoint             = {};      // stitch coordinates of the zoom mark
fPOINT                         ZoomRatio                 = {};      // zoom ratio used to draw stitch window
fRECTANGLE                     ZoomRect                  = {};      // zoom rectangle
// clang-format on
