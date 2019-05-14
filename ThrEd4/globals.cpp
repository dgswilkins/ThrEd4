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
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "ThrEdTypes.h"
#include "EnumMap.h"

namespace fs = std::filesystem;

uint32_t   ActiveColor = 0; // active color selector
uint32_t   ActiveLayer = 0; // active layer
fRECTANGLE AllItemsRect{};  // rectangle enclosing all forms and stitches

std::vector<fPOINT>* AngledFormVertices; // form formOrigin data for angle fills

uint32_t  AppliqueColor = 15; // underlay color
LPTSTR*   ArgList;            // command line argument array
HCURSOR   ArrowCursor;        // arrow
fs::path* AuxName;            //

std::vector<BSEQPNT>* BSequence; //

HBRUSH   BackgroundBrush;      // background color brush
uint32_t BeanCount;            // number of stitches added by convert to bean
HDC      BitmapDC;             // bitmap device context
RECT     BitmapDstRect;        // stitch window destination rectangle for zooomed view
uint32_t BitmapHeight;         // bitmap height
POINT    BitmapPoint;          // a point on the bitmap
fPOINT   BitmapSizeinStitches; // bitmap end points in stitch points
RECT     BitmapSrcRect;        // bitmap source rectangle for zoomed view
fPOINT   BmpStitchRatio;       // bitmap to stitch hoop ratios
uint32_t BitmapWidth;          // bitmap width
float    BorderWidth = BRDWID; // border width for satin borders
int32_t  ButtonHeight;         // button height
int32_t  ButtonWidth;          // button width
int32_t  ButtonWidthX3;        // button width times 3

std::vector<HWND>* ButtonWin; // button windows

float    ButtonholeCornerLength = IBFCLEN; // buttonhole corner length
HWND     CancelButton;                     // cancel button
uint32_t Clip = 0;                         // pcs format clipboard handle

std::vector<fPOINTATTR>* ClipBuffer; //

FORMCLIP* ClipFormHeader; // for thred form clipboard data
HGLOBAL   ClipMemory;     // handle to the clipboard memory
void*     ClipPointer;    // for memory allocation for clipboard data

std::vector<fPOINT>* ClipPoints; // main clipboard fill points for forms

FLSIZ      ClipRectSize;   // clipboard rectangle size
CLPSTCH*   ClipStitchData; // for pcs clipboard data

uint32_t ClipTypeMap = MCLPF | MVCLPF | MHCLPF | MANGCLPF; // for checking if a fill is a clipboard fill

uint32_t ClosestFormToCursor;    // closest form to the cursor
uint32_t ClosestPointIndex;      // index of closest point
uint32_t ClosestVertexToCursor;  // formOrigin closest to the cursor
int32_t* ColorBarSize;           // Color bar width scaled for DPI
HCURSOR  CrossCursor;            // cross
uint32_t CurrentFormGuides;      // guide index in the currently selected form
uint32_t CurrentFormGuidesCount; // number of guides in the currently selected form
uint32_t CurrentVertexIndex;     // points in the currently selected form

std::vector<HWND>* DefaultColorWin; // default color windows

HWND            DeleteStitchesDialog; // asking user if they want to delete the sitches associated with a form
std::wstring*   DesignerName;         // designer name in clear
HWND            DiscardButton;        // discard button
DRAWITEMSTRUCT* DrawItem;             // for owner-draw windows
POINT           EndPointCross;        // point to draw at the end cross for form select points
HMENU           FillMenu;             // fill submenu

std::vector<float>* FormAngles;        // angles of a form for satin border fills
std::vector<POINT>* FormControlPoints; // form control rectangle in pixel coordinates

HWND                 FormDataSheet = nullptr; // form data sheet
std::vector<POINT>*  FormLines;               // used in the form drawing routines
std::vector<FRMHED>* FormList;                // a list of form headers
uint32_t             FormMenuChoice = 0u;     // data type for form data form numerical entry
fPOINT               FormMoveDelta;           // offset for moving forms

std::wstring* FormOnOff;

HPEN FormPen;         // form pen
HPEN FormPen3px;      // three-pixel form pen
HPEN FormSelectedPen; // form select pen

std::vector<fPOINT>* FormVertices; // form points

HWND      GeneralNumberInputBox; // general number input box
HPEN      GridPen;               // pen for stitch grid
uint32_t  GroupEndStitch;        // higher end of selected stitches
uint32_t  GroupStartStitch;      // lower end of selected stitches
uint32_t  GroupStitchIndex;      // last point selected in group
fs::path* HomeDirectory;         // directory from which thred was executed
double    HorizontalLength;      // horizontal length of a clipboard fill
float     HorizontalLength2;     // horizontal length of a clipboard fill/2
float     HorizontalRatio;       // horizontal ratio between the zoom window and the entire stitch space
HWND      HorizontalScrollBar;   // horizontal scroll bar
INIFILE   IniFile;               // initialization file
POINT     InsertLine[3];         // the insert line

std::vector<fPOINT>* InsidePointList;           // list of inside outline points for satin or clipboard fills
std::vector<fPOINT>* InsidePoints;              // pointer to the list of inside outline points
std::vector<INSREC>* InterleaveSequenceIndices; // indices into interleave points
std::vector<fPOINT>* InterleaveSequence;        // storage for interleave points
std::vector<HWND>*   LabelWindow;               // text handles for the form data sheet

fPOINT LastPoint;                       // last formOrigin written by line connect routine
HPEN   LayerPen[6];                     //
float  LineSpacing = DEFSPACE * PFGRAN; // stitch spacing in stitch units
fPOINT LowerLeftStitch;                 // lower left formOrigin in a form
HMENU  MainMenu;                        // main menu
HWND   MainStitchWin;                   // stitch window
float  MaxStitchLen;                    // maximum stitch length

MENUITEMINFO* MenuInfo;

float    MinStitchLength = MINSIZ * PFAFGRAN; // minimum stitch size
MSG      Msg;                                 // main message loop message
wchar_t  MsgBuffer[MSGSIZ];                   // for user messages
uint32_t MsgIndex;                            // pointer to the message buffer
RECT     MsgRect;                             // rectangle containing the text message
HWND     MsgWindow = nullptr;                 // message window
HPEN     MultiFormPen;                        // multiple selected forms pen
uint32_t NearestCount;                        // number of boxes selected
uint32_t NewFormVertexCount;                  // points in the new form
HWND     OKButton;                            // ok button

std::vector<fPOINT>* OSequence; // temporary storage for sequencing

bool     OutLineEverySelectedForm = false; // When selecting multiple forms, should we outline every form?
uint32_t OutputIndex;                      // output pointer for sequencing

std::vector<fPOINT>* OutsidePointList; // list of outside outline points for satin or clipboard fills
std::vector<fPOINT>* OutsidePoints;    // pointer to the list of outside outline points

char           PCSBMPFileName[16];                // bitmap file name from pcs file
PCSHEADER      PCSHeader;                         // pcs file header
const wchar_t* PcdClipFormat   = L"PMust_Format"; //
float          PicotSpacing    = IPICSPAC;        // space between border picots
uint32_t       PreferenceIndex = 0;               // index to the active preference window
int32_t        PreferenceWindowWidth;             // width of the preference window
HWND           PreferencesWindow;                 // preferences window
uint32_t       PreviousFormIndex;                 // previously selected form
uint32_t       PseudoRandomValue;                 // pseudo-random sequence register
fRECTANGLE     RotationRect{};                    // rotation rectangle

std::vector<POINT>* RubberBandLine; // points to form points to be moved

uint32_t SatinEndGuide; // satin end guide for the currently selected form

std::vector<SATCON>* SatinGuides; // satin form connects

RECT      scRct;      //
uint32_t* screenDPI;  // screen DPI
int32_t*  ScrollSize; // Scroll bar width scaled for DPI

std::vector<POINT>* SearchLine; // stitch select line

HPEN     SelectAllPen;              // pen for drawing large boxes
FRMHED*  SelectedForm;              // pointer to selected form
uint32_t SelectedFormControlVertex; // user selected form control formOrigin

std::vector<uint32_t>* SelectedFormList;     // a list of selected forms
FORMVERTICES           SelectedFormVertices; // selected form vertices
std::vector<POINT>*    SelectedFormsLine;    // line derived from the big rectangle

RECT   SelectedFormsRect;  // for multiple selections;
fPOINT SelectedFormsSize;  // size of multiple select rectangle
RECT   SelectedPixelsRect; // display form vertex select rectangle
fPOINT SelectedPoint;      // for converting stitch coordinates to metric cordinates (mm)

std::vector<POINT>* SelectedPointsLine; // line derived from the formOrigin select rectangle

fRECTANGLE SelectedVerticesRect{};                   // rectangle enclosing selected form verticess
double     ShowStitchThreshold = SHOPNTS;          // show stitch grid below this zoom level
HWND       SideMessageWindow   = nullptr;          // main side message window
wchar_t    SideWindowEntryBuffer[11];              // side window number for entering form data sheet numbers
HWND       SideWindow[16];                         // side message windows
float      SmallStitchLength = SMALSIZ * PFAFGRAN; // user can remove stitches smaller than this
float      SnapLength        = SNPLEN * PFGRAN;    // snap together length
float      SpiralWrap        = SPIRWRAP;           // number of revolutions in a spiral
float      StarRatio         = STARAT;             // star formOrigin to body ratio

EnumMap<StateFlag> StateMap(0); // Flags indicating current run state

fPOINT     StitchBmpRatio;                 // stitch hoop to bitmap ratios
double     StitchBoxesThreshold = STCHBOX; // threshold for drawing stitch boxes

std::vector<fPOINTATTR>* StitchBuffer;     // main stitch buffer

POINT      StitchCoordinatesPixels;        // converted from stitch to pixel
fRECTANGLE StitchRangeRect;                // stitch range rectangle
RECT       StitchWindowClientRect;         // stitch window size,client
HDC        StitchWindowDC;                 // stitch window device context
HDC        StitchWindowMemDC;              // stitch window memory device context
POINT      StitchWindowOrigin;             // offset origin of the stitch window
POINT      StitchWindowSize;               // size of the stitch window in pixels

std::vector<std::wstring>* StringTable; //
std::vector<fPOINT>*       TempPolygon; // temporary storage when user is entering a polygon;

uint32_t            TextureIndex;                   // next textured fill point index
std::wstring*       TextureInputBuffer;             // texture fill number buffer
std::vector<TXPNT>* TexturePointsBuffer;            // buffer for textured fill points
HWND                ThreadSizeWin[16];              // thread size windows
uint32_t            ThrEdClip       = 0;            // thred format
const wchar_t*      ThrEdClipFormat = L"threditor"; //
HINSTANCE           ThrEdInstance;                  // main instance handle
HWND                ThrEdWindow;                    // main window handle
fs::path*           ThrName;                        //
POINT               ThredWindowOrigin;              // offset origin of the main window
uint32_t*           TraceBitmapData;                // trace bitmap data
HDC                 TraceDC;                        // trace device context

boost::dynamic_bitset<>* TracedEdges; // detected edges of trace areas
boost::dynamic_bitset<>* TracedMap;   // in/out state of trace areas

POINT              UnzoomedRect;                         // size of the unzoomed stitch window
COLORREF           UserColor[16];                        // user colors
HWND               UserColorWin[16];                     // user color windows
EnumMap<UserFlag>  UserFlagMap(0);                       // for storage of persistent binary variables set by the user
HPEN               UserPen[16];                          // user color pens
float              UserStitchLength = USESIZ * PFAFGRAN; // user selected stitch size
std::vector<HWND>* ValueWindow;                          // data handles for the form data sheet
uint32_t           VertexCount;                          // sides of the selected form to fill
float              VerticalRatio;                        // vertical ratio between the zoom window and the entire stitch space
HWND               VerticalScrollBar;                    // vertical scroll bar
fs::path*          WorkingFileName;                      //
POINT              ZoomBoxLine[5];                       // the zoom box
fPOINT             ZoomBoxOrigin;                        // zoom box origin
float              ZoomFactor = 1.0f;                    // zoom factor
fPOINT             ZoomMarkPoint;                        // stitch coordinates of the zoom mark
fPOINT             ZoomRatio;                            // zoom ratio used to draw stitch window
fRECTANGLE         ZoomRect;                             // zoom rectangle
