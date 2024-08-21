// Local Headers
#include "stdafx.h"
#include "clipStitch.h"
#include "EnumMap.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "satCon.h"
#include "textureHeader.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <minwindef.h>
#include <windef.h>
#include <WinUser.h>

// Standard Libraries
#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

// clang-format off
// ReSharper disable CppUseAuto
MY_SINGLE*                     Instance                  = nullptr; // main instance
uint8_t                        ActiveColor               = 0U;      // active color selector
uint8_t                        ActiveLayer               = 0U;      // active layer
uint8_t                        AppliqueColor             = COLORCNT - 1U; // underlay color
COLORREF                       BackgroundColor           = 0U;      // stitch window background
float                          BorderWidth               = BRDWID;  // border width for satin borders
int32_t                        ButtonHeight              = 0;       // button height
int32_t                        ButtonWidth               = 0;       // button width
int32_t                        ButtonWidthX3             = 0;       // button width times 3
float                          ButtonholeCornerLength    = IBFCLEN; // buttonhole corner length
HWND                           CancelButton              = nullptr; // cancel button
uint32_t                       Clip                      = 0U;      // pcs format clipboard handle
HGLOBAL                        ClipMemory                = nullptr; // handle to the clipboard memory
void*                          ClipPointer               = nullptr; // for memory allocation for clipboard data
F_LSIZ                         ClipRectSize              = {};      // clipboard rectangle size
CLIP_STITCH*                   ClipStitchData            = nullptr; // for pcs clipboard data
uint32_t                       ClosestFormToCursor       = 0U;      // closest form to the cursor
uint32_t                       ClosestPointIndex         = 0U;      // index of closest point
uint32_t                       ClosestVertexToCursor     = 0U;      // formOrigin closest to the cursor
HWND                           DeleteStitchesDialog      = nullptr; // asking user if they want to delete the stitches associated with a form
HWND                           DiscardButton             = nullptr; // discard button
DRAWITEMSTRUCT*                DrawItem                  = nullptr; // for owner-draw windows
POINT                          EndPointCross             = {};      // point to draw at the end cross for form select points
HWND                           FormDataSheet             = nullptr; // form data sheet
uint32_t                       FormMenuChoice            = 0U;      // data type for form data form numerical entry
F_POINT                        FormMoveDelta             = F_POINT {}; // offset for moving forms
HPEN                           FormPen                   = nullptr; // form pen
HPEN                           FormPen3px                = nullptr; // three-pixel form pen
HPEN                           FormSelectedPen           = nullptr; // form select pen
std::vector<F_POINT>*          FormVertices              = nullptr; // form points
HWND                           GeneralNumberInputBox     = nullptr; // general number input box
HPEN                           GridPen                   = nullptr; // pen for stitch grid
uint32_t                       GroupEndStitch            = 0U;      // higher end of selected stitches
uint32_t                       GroupStartStitch          = 0U;      // lower end of selected stitches
uint32_t                       GroupStitchIndex          = 0U;      // last point selected in group
float                          HorizontalRatio           = 0.0F;    // horizontal ratio between the zoom window and the entire stitch space
HWND                           HorizontalScrollBar       = nullptr; // horizontal scroll bar
INI_FILE                       IniFile                   = {};      // initialization file
std::array<POINT, 3>           InsertLine                = {};      // the insert line
std::vector<F_POINT>*          InsidePoints              = nullptr; // pointer to the list of inside outline points
std::vector<HWND>*             LabelWindow               = nullptr; // text handles for the form data sheet
F_POINT                        LastPoint                 = F_POINT {}; // last formOrigin written by line connect routine
float                          LineSpacing               = DEFSPACE * PFGRAN; // stitch spacing in stitch units
F_POINT                        LowerLeftStitch           = F_POINT {}; // lower left formOrigin in a form
std::array<uint32_t, OLDNUM>*  LRUPtr                    = nullptr; // pointer to LRU list
HMENU                          MainMenu                  = nullptr; // main menu
HWND                           MainStitchWin             = nullptr; // stitch window
float                          MaxStitchLen              = 0.0F;    // maximum stitch length
float                          MinStitchLength           = MINSIZ * PFGRAN; // minimum stitch size
MSG                            WinMsg                    = {};      // main message loop message
RECT                           MsgRect                   = {};      // rectangle containing the text message
HWND                           MsgWindow                 = nullptr; // message window
HPEN                           MultiFormPen              = nullptr; // multiple selected forms pen
uint32_t                       NearestCount              = 0U;      // number of boxes selected
uint32_t                       NewFormVertexCount        = 0U;      // points in the new form
HWND                           OKButton                  = nullptr; // ok button
std::vector<F_POINT>*          OSequence                 = nullptr; // temporary storage for sequencing
bool                           OutLineEverySelectedForm  = false;   // When selecting multiple forms, should we outline every form?
std::vector<F_POINT>*          OutsidePointList          = nullptr; // list of outside outline points for satin or clipboard fills
std::vector<F_POINT>*          OutsidePoints             = nullptr; // pointer to the list of outside outline points
wchar_t const*                 PcdClipFormat             = L"PMust_Format";   //
float                          PicotSpacing              = IPICSPAC;// space between border picots
uint32_t                       PreferenceIndex           = 0U;      // index to the active preference window
int32_t                        PreferenceWindowWidth     = {};      // width of the preference window
HWND                           PreferencesWindow         = nullptr; // preferences window
uint32_t                       PreviousFormIndex         = 0U;      // previously selected form
uint32_t                       PseudoRandomValue         = 0U;      // pseudo-random sequence register
F_RECTANGLE                    RotationRect              = F_RECTANGLE {}; // rotation rectangle
std::vector<POINT>*            RubberBandLine            = nullptr; // points to form points to be moved
std::vector<SAT_CON>*          SatinGuides               = nullptr; // satin form connects
int32_t*                       ScreenDPI                 = nullptr; // screen DPI
int32_t*                       ScrollSize                = nullptr; // Scroll bar width scaled for DPI
std::vector<POINT>*            SearchLine                = nullptr; // stitch select line
HPEN                           SelectAllPen              = nullptr; // pen for drawing large boxes
uint32_t                       SelectedFormControlVertex = 0U;      // user selected form control formOrigin
std::vector<uint32_t>*         SelectedFormList          = nullptr; // a list of selected forms
FORM_VERTICES                  SelectedFormVertices      = {};      // selected form vertices
std::vector<POINT>*            SelectedFormsLine         = nullptr; // line derived from the big rectangle
RECT                           SelectedFormsRect         = {};      // for multiple selections;
F_POINT                        SelectedFormsSize         = F_POINT {}; // size of multiple select rectangle
RECT                           SelectedPixelsRect        = {};      // display form vertex select rectangle
std::vector<POINT>*            SelectedPointsLine        = nullptr; // line derived from the formOrigin select rectangle
F_RECTANGLE                    SelectedVerticesRect      = F_RECTANGLE {}; // rectangle enclosing selected form verticess
float                          ShowStitchThreshold       = SHOPNTS; // show stitch grid below this zoom level
HWND                           SideMessageWindow         = nullptr; // main side message window
float                          SmallStitchLength         = SMALSIZ * PFGRAN; // user can remove stitches smaller than this
float                          SnapLength                = SNPLEN * PFGRAN;  // snap together length
std::vector<SEARCH_REC>*       SortBuffer                = nullptr; // sort buffer for stitch search
uint32_t                       SortIndex                 = 0U;      // index for stitch search
float                          SpiralWrap                = SPIRWRAP;// number of revolutions in a spiral
float                          StarRatio                 = STARAT;  // star formOrigin to body ratio
ENUM_MAP<StateFlag>*           StateMap                  = nullptr; // Flags indicating current run state
F_POINT                        StitchBmpRatio            = F_POINT {}; // stitch hoop to bitmap ratios
float                          StitchBoxesThreshold      = STCHBOX; // threshold for drawing stitch boxes
std::vector<F_POINT_ATTR>*     StitchBuffer              = nullptr; // main stitch buffer
F_RECTANGLE                    StitchRangeRect           = F_RECTANGLE {}; // stitch range rectangle
RECT                           StitchWindowClientRect    = {};      // stitch window size,client
HDC                            StitchWindowDC            = nullptr; // stitch window device context
HDC                            StitchWindowMemDC         = nullptr; // stitch window memory device context
POINT                          StitchWindowOrigin        = {};      // offset origin of the stitch window
SIZE                           StitchWindowSize          = {};      // size of the stitch window in pixels
std::vector<F_POINT>*          TempPolygon               = nullptr; // temporary storage when user is entering a polygon;
std::wstring*                  TextureInputBuffer        = nullptr; // texture fill number buffer
std::vector<TX_PNT>*           TexturePointsBuffer       = nullptr; // buffer for textured fill points
wchar_t const*                 ThrEdClipFormat           = L"threditor"; //
HINSTANCE                      ThrEdInstance             = {};      // main instance handle
HWND                           ThrEdWindow               = nullptr; // main window handle
fs::path*                      ThrName                   = nullptr; //
POINT                          ThredWindowOrigin         = {};      // offset origin of the main window
uint32_t*                      TraceBitmapData           = nullptr; // trace bitmap data
boost::dynamic_bitset<>*       TracedEdges               = nullptr; // detected edges of trace areas
boost::dynamic_bitset<>*       TracedMap                 = nullptr; // in/out state of trace areas

std::array<std::vector<uint32_t>, UNDOLEN>* UndoBuffer   = nullptr; // backup data

SIZE                           UnzoomedRect              = {};      // size of the unzoomed stitch window
std::array<COLORREF, COLORCNT> UserColor                 = {};      // user colors
ENUM_MAP<UserFlag>*            UserFlagMap               = nullptr; // for storage of persistent binary variables set by the user
float                          UserStitchLength          = USESIZ * PFGRAN; // user selected stitch size
float                          VerticalRatio             = 0.0F;    // vertical ratio between the zoom window and the entire stitch space
HWND                           VerticalScrollBar         = nullptr; // vertical scroll bar
std::array<POINT, SQPNTS>      ZoomBoxLine               = {};      // the zoom box
F_POINT                        ZoomBoxOrigin             = F_POINT {}; // zoom box origin
float                          ZoomFactor                = 1.0F;    // zoom factor
F_POINT                        ZoomMarkPoint             = F_POINT {}; // stitch coordinates of the zoom mark
F_POINT                        ZoomRatio                 = F_POINT {}; // zoom ratio used to draw stitch window
F_RECTANGLE                    ZoomRect                  = F_RECTANGLE {}; // zoom rectangle
// ReSharper restore CppUseAuto
// clang-format on
