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

namespace fs = std::experimental::filesystem;

unsigned   ActiveColor = 0; // active color selector
unsigned   ActiveLayer = 0; // active layer
fRECTANGLE AllItemsRect;    // rectangle enclosing all forms and stitches

std::vector<fPOINT>* AngledFormVertices; // form formOrigin data for angle fills

unsigned  AppliqueColor = 15; // underlay color
LPWSTR*   ArgList;            // command line argument array
HCURSOR   ArrowCursor;        // arrow
fs::path* AuxName;            //

std::vector<BSEQPNT>* BSequence; //

HBRUSH   BackgroundBrush;      // background color brush
unsigned BeanCount;            // number of stitches added by convert to bean
HDC      BitmapDC;             // bitmap device context
RECT     BitmapDstRect;        // stitch window destination rectangle for zooomed view
unsigned BitmapHeight;         // bitmap height
POINT    BitmapPoint;          // a point on the bitmap
dPOINT   BitmapSizeinStitches; // bitmap end points in stitch points
RECT     BitmapSrcRect;        // bitmap source rectangle for zoomed view
dPOINT   BmpStitchRatio;       // bitmap to stitch hoop ratios
unsigned BitmapWidth;          // bitmap width
double   BorderWidth = BRDWID; // border width for satin borders
unsigned ButtonHeight;         // button height
unsigned ButtonWidth;          // button width
unsigned ButtonWidthX3;        // button width times 3

std::vector<HWND>* ButtonWin; // button windows

float    ButtonholeCornerLength = IBFCLEN; // buttonhole corner length
HWND     CancelButton;                     // cancel button
unsigned Clip = 0;                         // pcs format clipboard handle

std::vector<fPOINTATTR>* ClipBuffer; //

FORMCLIP* ClipFormHeader; // for thred form clipboard data
HGLOBAL   ClipMemory;     // handle to the clipboard memory
void*     ClipPointer;    // for memory allocation for clipboard data

std::vector<fPOINT>* ClipPoints; // main clipboard fill points for forms

fRECTANGLE ClipRect;       // clipboard rectangle
FLSIZ      ClipRectSize;   // clipboard rectangle size
CLPSTCH*   ClipStitchData; // for pcs clipboard data

unsigned ClipTypeMap = MCLPF | MVCLPF | MHCLPF | MANGCLPF; // for checking if a fill is a clipboard fill

unsigned int ClosestFormToCursor;    // closest form to the cursor
unsigned     ClosestPointIndex;      // index of closest point
unsigned int ClosestVertexToCursor;  // formOrigin closest to the cursor
int*         ColorBarSize;           // Color bar width scaled for DPI
HCURSOR      CrossCursor;            // cross
unsigned int CurrentFormGuides;      // guide index in the currently selected form
unsigned int CurrentFormGuidesCount; // number of guides in the currently selected form
unsigned int CurrentVertexIndex;     // points in the currently selected form

std::vector<HWND>* DefaultColorWin; // default color windows

HWND            DeleteStitchesDialog; // asking user if they want to delete the sitches associated with a form
std::wstring*   DesignerName;         // designer name in clear
HWND            DiscardButton;        // discard button
DRAWITEMSTRUCT* DrawItem;             // for owner-draw windows
POINT           EndPointCross;        // point to draw at the end cross for form select points
HMENU           FillMenu;             // fill submenu

std::vector<float>*  FormAngles;        // angles of a form for satin border fills
std::vector<POINT>*  FormControlPoints; // form control rectangle in pixel coordinates

HWND                 FormDataSheet = nullptr; // form data sheet
std::vector<POINT>*  FormLines;               // used in the form drawing routines
std::vector<FRMHED>* FormList;                // a list of form headers
unsigned int         FormMenuChoice = 0u;     // data type for form data form numerical entry
fPOINT               FormMoveDelta;           // offset for moving forms

std::wstring* FormOnOff;

HPEN FormPen;         // form pen
HPEN FormPen3px;      // three-pixel form pen
HPEN FormSelectedPen; // form select pen

std::vector<fPOINT>* FormVertices; // form points

HWND      GeneralNumberInputBox; // general number input box
HPEN      GridPen;               // pen for stitch grid
unsigned  GroupEndStitch;        // higher end of selected stitches
unsigned  GroupStartStitch;      // lower end of selected stitches
unsigned  GroupStitchIndex;      // last point selected in group
fs::path* HomeDirectory;         // directory from which thred was executed
double    HorizontalLength;      // horizontal length of a clipboard fill
double    HorizontalLength2;     // horizontal length of a clipboard fill/2
double    HorizontalRatio;       // horizontal ratio between the zoom window and the entire stitch space
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
double LineSpacing = DEFSPACE * PFGRAN; // stitch spacing in stitch units
fPOINT LowerLeftStitch;                 // lower left formOrigin in a form
HMENU  MainMenu;                        // main menu
HWND   MainStitchWin;                   // stitch window
float  MaxStitchLen;                    // maximum stitch length

MENUITEMINFO* MenuInfo;

float        MinStitchLength = MINSIZ * PFAFGRAN; // minimum stitch size
MSG          Msg;                                 // main message loop message
wchar_t      MsgBuffer[MSGSIZ];                   // for user messages
unsigned     MsgIndex;                            // pointer to the message buffer
RECT         MsgRect;                             // rectangle containing the text message
HWND         MsgWindow = nullptr;                 // message window
HPEN         MultiFormPen;                        // multiple selected forms pen
unsigned     NearestCount;                        // number of boxes selected
unsigned int NewFormVertexCount;                  // points in the new form
HWND         OKButton;                            // ok button

std::vector<fPOINT>* OSequence; // temporary storage for sequencing

bool         OutLineEverySelectedForm = false; // When selecting multiple forms, should we outline every form?
unsigned int OutputIndex;                      // output pointer for sequencing

std::vector<fPOINT>* OutsidePointList; // list of outside outline points for satin or clipboard fills
std::vector<fPOINT>* OutsidePoints;    // pointer to the list of outside outline points

char           PCSBMPFileName[16];                // bitmap file name from pcs file
PCSHEADER      PCSHeader;                         // pcs file header
const wchar_t* PcdClipFormat   = L"PMust_Format"; //
float          PicotSpacing    = IPICSPAC;        // space between border picots
unsigned       PreferenceIndex = 0;               // index to the active preference window
long           PreferenceWindowWidth;             // width of the preference window
HWND           PreferencesWindow;                 // preferences window
unsigned int   PreviousFormIndex;                 // previously selected form
unsigned       PseudoRandomValue;                 // pseudo-random sequence register
fRECTANGLE     RotationRect;                      // rotation rectangle

std::vector<POINT>* RubberBandLine; // points to form points to be moved

unsigned int SatinEndGuide; // satin end guide for the currently selected form

std::vector<SATCON>* SatinGuides; // satin form connects

RECT      scRct;      //
unsigned* screenDPI;  // screen DPI
int*      ScrollSize; // Scroll bar width scaled for DPI

std::vector<POINT>* SearchLine; // stitch select line

HPEN     SelectAllPen;              // pen for drawing large boxes
FRMHED*  SelectedForm;              // pointer to selected form
unsigned SelectedFormControlVertex; // user selected form control formOrigin

std::vector<unsigned int>* SelectedFormList;     // a list of selected forms
FORMVERTICES               SelectedFormVertices; // selected form vertices
std::vector<POINT>*        SelectedFormsLine;    // line derived from the big rectangle

RECT   SelectedFormsRect;  // for multiple selections;
fPOINT SelectedFormsSize;  // size of multiple select rectangle
RECT   SelectedPixelsRect; // display form vertex select rectangle
fPOINT SelectedPoint;      // for converting stitch coordinates to metric cordinates (mm)

std::vector<POINT>* SelectedPointsLine; // line derived from the formOrigin select rectangle

fRECTANGLE SelectedVerticesRect;                   // rectangle enclosing selected form verticess
double     ShowStitchThreshold = SHOPNTS;          // show stitch grid below this zoom level
HWND       SideMessageWindow   = nullptr;          // main side message window
wchar_t    SideWindowEntryBuffer[11];              // side window number for entering form data sheet numbers
HWND       SideWindow[16];                         // side message windows
double     SmallStitchLength = SMALSIZ * PFAFGRAN; // user can remove stitches smaller than this
double     SnapLength        = SNPLEN * PFGRAN;    // snap together length
double     SpiralWrap        = SPIRWRAP;           // number of revolutions in a spiral
double     StarRatio         = STARAT;             // star formOrigin to body ratio

EnumMap<StateFlag> StateMap(0); // Flags indicating current run state

dPOINT     StitchBmpRatio;                 // stitch hoop to bitmap ratios
double     StitchBoxesThreshold = STCHBOX; // threshold for drawing stitch boxes
fPOINTATTR StitchBuffer[MAXITEMS * 2];     // main stitch buffer
POINT      StitchCoordinatesPixels;        // converted from stitch to pixel
fRECTANGLE StitchRangeRect;                // stitch range rectangle
RECT       StitchWindowClientRect;         // stitch window size,client
HDC        StitchWindowDC;                 // stitch window device context
HDC        StitchWindowMemDC;              // stitch window memory device context
POINT      StitchWindowOrigin;             // offset origin of the stitch window
POINT      StitchWindowSize;               // size of the stitch window in pixels

std::vector<std::wstring>* StringTable; //
std::vector<fPOINT>*       TempPolygon; // temporary storage when user is entering a polygon;

unsigned int        TextureIndex;                   // next textured fill point index
std::wstring*       TextureInputBuffer;             // texture fill number buffer
std::vector<TXPNT>* TexturePointsBuffer;            // buffer for textured fill points
HWND                ThreadSizeWin[16];              // thread size windows
unsigned            ThrEdClip       = 0;            // thred format
const wchar_t*      ThrEdClipFormat = L"threditor"; //
HINSTANCE           ThrEdInstance;                  // main instance handle
HWND                ThrEdWindow;                    // main window handle
fs::path*           ThrName;                        //
POINT               ThredWindowOrigin;              // offset origin of the main window
unsigned*           TraceBitmapData;                // trace bitmap data
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
unsigned int       VertexCount;                          // sides of the selected form to fill
double             VerticalRatio;                        // vertical ratio between the zoom window and the entire stitch space
HWND               VerticalScrollBar;                    // vertical scroll bar
fs::path*          WorkingFileName;                      //
POINT              ZoomBoxLine[5];                       // the zoom box
fPOINT             ZoomBoxOrigin;                        // zoom box origin
double             ZoomFactor = 1;                       // zoom factor
dPOINT             ZoomMarkPoint;                        // stitch coordinates of the zoom mark
dPOINT             ZoomRatio;                            // zoom ratio used to draw stitch window
dRECTANGLE         ZoomRect;                             // zoom rectangle
