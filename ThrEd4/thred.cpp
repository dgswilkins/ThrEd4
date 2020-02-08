#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

#ifdef ALLOCFAILURE
#include <new.h>
#endif

// Standard Libraries
#include <bitset>

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "EnumMap.h"
#include "utf8conv.h"
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "formForms.h"
#include "hlp.h"
#include "repair.h"
#include "satin.h"
#include "texture.h"
#include "trace.h"
#include "xt.h"
#include "thred.h"

namespace fs = std::filesystem;

namespace thi = thred::internal;

// select box
constexpr auto NERCNT = 4U; // number of entries in the near array;

// main variables
int32_t         ArgCount;                // command line argument count
RECT            ThredWindowRect;         // main window size
RECT            ColorBarRect;            // color bar rectangle
RECT            MinLenRect;              // minimum length rectangle
RECT            MaxLenRect;              // maximum length rectangle
uint32_t        SmallestStitchIndex;     // pointer to the smallest stitch in the selected range
uint32_t        LargestStitchIndex;      // pointer to the largest stitch in the selected range
uint32_t        CurrentStitchIndex;      // pointer to the current selection for length search
HDC             ThredDC;                 // main device context handle
HDC             ColorBarDC;              // color bar device context
HBITMAP         StitchWindowBmp;         // bitmap for the memory stitch device context
SIZE            TextSize;                // used for measuring sizes of text items
SIZE            ScreenSizePixels;        // screen size in pixels
SIZE            ScreenSizeMM;            // screen size in millimeters
RECT            StitchWindowAbsRect;     // stitch window size,absolute
POINT           NearestPixel[NERCNT];    // selected points
POINT           BoxCoordinate = {0, 0};  // single select box point
double          DistanceToClick;         // distance of closest point to a mouse click
uint32_t        ClosestPointIndexClone;  // copy of index of closest point
uint32_t        PrevGroupStartStitch;    // lower end of previous selection
uint32_t        PrevGroupEndStitch;      // higher end of previous selection
uint32_t        BufferDigitCount;        // number of decimal digits in the number of stitches
uint32_t        LineIndex;               // line index for display routine
float           StitchWindowAspectRatio; // aspect ratio of the stitch window
FORMVERTEXCLIP* ClipFormVerticesData;    // form points clipboard header
void*           ThrEdClipPointer;        // for memory allocation for thred format clipboard data
POINT           ClipOrigin;              // origin of clipboard box in stitch coordinates
SIZE            SelectBoxSize;           // size of the select box
POINT SelectBoxOffset; // offset of the spot the user selected from the lower left of the select box
float RotationHandleAngle;      // angle of the rotation handle
uint32_t BitmapColor  = BITCOL; // bitmap color
double   ThreadSize30 = TSIZ30; //#30 thread size
double   ThreadSize40 = TSIZ40; //#40 thread size
double   ThreadSize60 = TSIZ60; //#40 thread size
uint32_t RunPoint;              // point for animating stitchout
uint32_t StitchesPerFrame;      // number of stitches to draw in each frame
int32_t  MovieTimeStep;         // time delay for stitchout

// WARNING the size of the following array must be changed if the maximum movie speed is changed
POINT MovieLine[100]; // line for movie stitch draw

uint32_t LRUMenuId[] = {FM_ONAM0, FM_ONAM1, FM_ONAM2, FM_ONAM3}; // recently used file menu ID's

uint32_t UndoBufferWriteIndex = 0;      // undo storage pointer
uint32_t UndoBufferReadIndex  = 0;      // undo retrieval pointers
uint32_t LastKeyCode          = 0xffff; // last key code

std::vector<fs::path>* VersionNames; // temporary storage for old file version names

char          FileVersionIndex;   // points to old version to be read
uint32_t      LayerIndex;         // active layer code
uint32_t      ClipFormsCount;     // number of forms the on the clipboard
POINT         StitchArrow[3];     // arrow for selected stitch
RANGE         SelectedRange;      // first and last stitch for min/max stitch select
uint32_t      NameOrder[50];      // designer name order table
uint8_t       NameEncoder[128];   // designer name encoding
uint8_t       NameDecoder[256];   // designer name decode
HWND          FirstWin;           // first window not destroyed for exiting enumerate loop
FRMRANGE      SelectedFormsRange; // range of selected forms
uint32_t      TmpFormIndex;       // saved form index
float         ZoomMin;            // minimum allowed zoom value
fPOINT        BalaradOffset;      // balarad offset
uint32_t      SideWindowLocation; // side message window location
POINT         SideWindowSize;     // size of the side message window
std::wstring* SideWindowsStrings; // string array displayed in sidmsg
fPOINT        CellSize;           // size of an stitchMap cell for drawing stitch boxes
uint32_t      DraggedColor;       // color being dragged

std::vector<POINT>* FormVerticesAsLine; // form vertex clipboard paste into form line

uint32_t LastFormSelected; // end point of selected range of forms

std::vector<std::unique_ptr<uint32_t[]>>* UndoBuffer; // backup data

#if PESACT
uint8_t* PEScolors;             // pes colors
uint8_t* PESdata;               // pes card data buffer
fPOINT   PESstitchCenterOffset; // offset for writing pes files
PESTCH*  PESstitches;           // pes stitch buffer
uint8_t  PESequivColors[16];    // pes equivalent colors
uint8_t* PESstitch;             // pes stitches
uint32_t PEScolorIndex;         // pes color index
#endif

// cursors
HCURSOR FormCursor;            // form
HCURSOR DLineCursor;           // dline
HCURSOR NeedleUpCursor;        // upright needle
HCURSOR NeedleLeftUpCursor;    // left up needle
HCURSOR NeedleLeftDownCursor;  // left down needle
HCURSOR NeedleRightUpCursor;   // right up needle
HCURSOR NeedleRightDownCursor; // right down needle

HPEN LinePen;        // line pen for stitch move lines
HPEN BoxPen[4];      // box pens
HPEN CrossPen;       // pen for crosses in color windows
HPEN GroupSelectPen; // pen for group select
HPEN BackgroundPen;  // background color pen
HPEN BitmapPen;      // bitmap pen
HPEN ZoomMarkPen;    // zoom mark pen
HPEN KnotPen;        // knot pen

uint32_t BackgroundPenWidth; // width of the background pen

// brushes
HBRUSH DefaultColorBrush[16]; // default color brushes
HBRUSH UserColorBrush[16];    // user color brushes

// for the choose color dialog box
CHOOSECOLOR ColorStruct;
COLORREF    CustomColor[16];
// for the background color dialog box
CHOOSECOLOR BackgroundColorStruct;
COLORREF    CustomBackgroundColor[16];
// for the bitmap color dialog box
CHOOSECOLOR BitMapColorStruct;
COLORREF    BitmapBackgroundColors[16];

wchar_t  ThreadSize[16][2];    // user selected thread sizes
uint32_t ThreadSizePixels[16]; // thread sizes in pixels
uint32_t ThreadSizeIndex[16];  // thread size indices

HMENU FileMenu;               // file submenu
HMENU BorderFillMenu;         // border fill submenu
HMENU ViewMenu;               // view submenu
HMENU ViewSetMenu;            // view/set
HMENU EditMenu;               // edit submenu
HWND  ChangeThreadSizeWin[3]; // thread size change windows
HWND  ColorBar;               // color bar
HWND  SpeedScrollBar;         // speed scroll bar for movie
HWND  BackupViewer[OLDVER];   // handles of multiple file viewing windows

COLORREF       BackgroundColor; // stitch window background
COLORREF const BoxColor[] = {0x404040, 0x408040, 0x804040, 0x404080};
int32_t        ThreadWidthPixels[3];     // thread sizes in pixels
std::bitset<32> DisplayedColorBitmap(0); // Map of color numbers in design that are actually displayed
double GapToNearest[NERCNT];             // distances of the closest points
                                         // to a mouse click
int32_t    NearestPoint[NERCNT];         // indices of the closest points
fPOINT     StitchRangeSize;              // form check ranges
uint32_t   MoveAnchor;                   // for resequencing stitches
double     RotateAngle;                  // angle for pixel rotate
SIZE       PickColorMsgSize;             // size of the pick color message
POINT      InsertSize;                   // size of file insert window
fPOINT     InsertCenter;                 // center point in inserted file
uint32_t   NumericCode;                  // keyboard numerical input
float      LowestAngle;                  // low angle for angle from mark
float      OriginalAngle;                // original angle for angle from mark
float      HighestAngle;                 // hi angle for angle from mark
uint32_t   Knots[MAXKNOTS];              // pointers to knots
uint32_t   KnotCount;                    // number of knots in the design
uint32_t   KnotAttribute;                // knot stitch attribute
fPOINT     KnotStep;                     // knot stepSize
char const KnotAtStartOrder[] = {2, 3, 1, 4, 0};     // knot spacings
char       KnotAtEndOrder[]   = {-2, -3, -1, -4, 0}; // reverse knot spacings
char       KnotAtLastOrder[]  = {0, -4, -1, -3, -2}; // reverse knot spacings
HANDLE     BalaradFile;                              // balarad file handle

// graphics variables
float      AspectRatio = (LHUPX / LHUPY); // aspect ratio of the stitch window
SCROLLINFO ScrollInfo;                    // scroll bar i/o structure

COLORREF const DefaultColors[] = {0x00000000,
                                  0x00800000,
                                  0x00FF0000,
                                  0x00808000,
                                  0x00FFFF00,
                                  0x00800080,
                                  0x00FF00FF,
                                  0x00000080,
                                  0x000000FF,
                                  0x00008000,
                                  0x0000FF00,
                                  0x00008080,
                                  0x0000FFFF,
                                  0x00808080,
                                  0x00C0C0C0,
                                  0x00FFFFFF};

int32_t BoxOffset[4];

uint32_t VerticalIndex;      // vertical index of the color window, calculated from mouse click
uint32_t ThreadSizeSelected; // thread selected for size change

#if PESACT
TCHAR const AllFilter[_MAX_PATH + 1] =
    L"Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Brother (PES)\0*.pes\0Tajima (DST)\0*.dst\0";
#else
wchar_t const AllFilter[_MAX_PATH + 1] =
    L"Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Tajima (DST)\0*.dst\0";
#endif

fs::path* GeName;
fs::path* ColorFileName; //.thw file name
fs::path* RGBFileName;   //.rgb file name
fs::path* DefaultDirectory;
fs::path* SearchName;
fs::path* DefaultBMPDirectory;
fs::path* BalaradName0; // balarad semaphore file
fs::path* BalaradName1; // balarad data file
fs::path* BalaradName2;
fs::path* UserBMPFileName; // bitmap file name from user load
fs::path* IniFileName;     //.ini file name

wchar_t const BmpFilter[_MAX_PATH + 1]    = L"Microsoft (BMP)\0*.bmp\0";
wchar_t       CustomFilter[_MAX_PATH + 1] = L"Thredworks (THR)\0*.thr\0";
HANDLE        FileHandle                  = nullptr;
HANDLE        PCSFileHandle               = nullptr;
HANDLE        IniFileHandle               = nullptr;
HANDLE        InsertedFileHandle; // insert file handle
HANDLE        BitmapFileHandle;   // bitmap handle
uint32_t      FileSize;           // size of file
DWORD         BytesRead;          // bytes actually read from file
uint32_t      ColorChanges;       // number of color changes

OPENFILENAME OpenFileName = {
    sizeof(OpenFileName),     // lStructsize
    nullptr,                  // hwndOwner
    nullptr,                  // hInstance
    std::begin(AllFilter),    // lpstrFilter
    std::begin(CustomFilter), // lpstrCustomFilter
    _MAX_PATH,                // nMaxCustFilter
    0,                        // nFilterIndex
    nullptr,                  // lpstrFile
    _MAX_PATH,                // nMaxFile
    nullptr,                  // lpstrFileTitle
    0,                        // nMaxFileTitle
    nullptr,                  // lpstrInitialDir
    nullptr,                  // lpstrTitle
    OFN_OVERWRITEPROMPT,      // Flags
    0,                        // nFileOffset
    0,                        // nFileExtension
    L"thr",                   // lpstrDefExt
    0,                        // lCustData
    nullptr,                  // lpfnHook
    nullptr,                  // lpTemplateName
#ifdef _MAC
    nullptr, // lpEditInfo
    0U,      // lpstrPrompt
#endif
#if (_WIN32_WINNT >= 0x0500)
    nullptr, // *pvReserved
    0U,      // dwReserved
    0U       // FlagsEx
#endif
};

std::vector<fs::path>*     PreviousNames;
std::vector<std::wstring>* Thumbnails;            // vector of thumbnail names
uint32_t                   ThumbnailsSelected[4]; // indexes of thumbnails selected for display
uint32_t ThumbnailDisplayCount;             // number of thumbnail file names selected for display
uint32_t ThumbnailIndex;                    // index into the thumbnail filname table
wchar_t  ThumbnailSearchString[32];         // storage for the thumnail search string
wchar_t  InsertedFileName[_MAX_PATH] = {0}; // insert file name
uint32_t InsertedVertexIndex;               // saved float pointer for inserting files
uint32_t InsertedFormIndex;                 // saved form pointer for inserting files
uint32_t InsertedStitchIndex;               // saved stitch pointer for inserting files

OPENFILENAME OpenBitmapName = {
    sizeof(OpenBitmapName),   // lStructsize
    nullptr,                  // hwndOwner
    nullptr,                  // hInstance
    std::begin(BmpFilter),    // lpstrFilter
    std::begin(CustomFilter), // lpstrCustomFilter
    _MAX_PATH,                // nMaxCustFilter
    0,                        // nFilterIndex
    nullptr,                  // lpstrFile
    _MAX_PATH,                // nMaxFile
    nullptr,                  // lpstrFileTitle
    0,                        // nMaxFileTitle
    nullptr,                  // lpstrInitialDir
    nullptr,                  // lpstrTitle
    OFN_OVERWRITEPROMPT,      // Flags
    0,                        // nFileOffset
    0,                        // nFileExtension
    L"bmp",                   // lpstrDefExt
    0,                        // lCustData
    nullptr,                  // lpfnHook
    nullptr,                  // lpTemplateName
#ifdef _MAC
    nullptr, // lpEditInfo
    0U,      // lpstrPrompt
#endif
#if (_WIN32_WINNT >= 0x0500)
    nullptr, // *pvReserved
    0U,      // dwReserved
    0U       // FlagsEx
#endif
};

BITMAPFILEHEADER BitmapFileHeader;   // bitmap file header
BITMAPV4HEADER   BitmapFileHeaderV4; // bitmap version4 file header
BITMAPINFO       BitmapInfo;         // bitmap info
BITMAPINFOHEADER BitmapInfoHeader;   // bitmap info header

COLORREF const DefaultUserColors[] = {0x00000000,
                                      0x002dffff,
                                      0x003f87e9,
                                      0x000762f8,
                                      0x000000ff,
                                      0x002f03af,
                                      0x007248b7,
                                      0x00ff0080,
                                      0x00920166,
                                      0x00a00000,
                                      0x00ff2424,
                                      0x006a4d15,
                                      0x00f5800a,
                                      0x004b7807,
                                      0x00156a1e,
                                      0x00dbe6e3};

COLORREF const DefaultCustomColors[] = {0x00729674,
                                        0x001a1eb9,
                                        0x00427347,
                                        0x0000bfff,
                                        0x00d3c25f,
                                        0xc003ced0,
                                        0x004a8459,
                                        0x008cacd0,
                                        0x0081aeb6,
                                        0x007243a5,
                                        0x00bdadda,
                                        0x009976c5,
                                        0x0096d9f5,
                                        0x00e2ddd6,
                                        0x00245322,
                                        0x007b60ae};

COLORREF const DefaultCustomBackgroundColors[] = {0x00a3c5dc,
                                                  0x00adc7b6,
                                                  0x00d1fcfb,
                                                  0x00dcd7c0,
                                                  0x00ebddcd,
                                                  0x00c6b3b3,
                                                  0x00dec9ce,
                                                  0x00d2d1e9,
                                                  0x00dfdffd,
                                                  0x00bee6ef,
                                                  0x008fb8b1,
                                                  0x0085c2e0,
                                                  0x00abc1c9,
                                                  0x00d3d3c7,
                                                  0x007c9c84,
                                                  0x009acddc};

COLORREF const DefaultBitmapBackgroundColors[] = {0x00c0d5bf,
                                                  0x00c8dfee,
                                                  0x00708189,
                                                  0x00a5a97a,
                                                  0x00b8d6fe,
                                                  0x008a8371,
                                                  0x004b6cb8,
                                                  0x009cdcc2,
                                                  0x00366d39,
                                                  0x00dcfcfb,
                                                  0x003c4f75,
                                                  0x0095b086,
                                                  0x00c9dcba,
                                                  0x0043377b,
                                                  0x00b799ae,
                                                  0x0054667a};

POINT MoveLine0[2];              // move point line
POINT MoveLine1[2];              // move point line
POINT ClipInsertBoxLine[5];      // for displaying clipboard insert rectangle
POINT RotateBoxOutline[5];       // for drawing the rotate rectangle
POINT RotateBoxCrossVertLine[2]; // vertical part of the rotate cross
POINT RotateBoxCrossHorzLine[2]; // horizontal part of the rotate cross
POINT RotateBoxToCursorLine[2];  // line from the cursor to the center of the rotate cross

COLCHNG ColorChangeTable[MAXCHNG];

class DSTDAT
{
  public:
  char cor {0};
  char val {0};

  constexpr DSTDAT() noexcept = default;
  // DSTDAT(DSTDAT&&) = default;
  // DSTDAT& operator=(DSTDAT const& rhs) = default;
  // DSTDAT& operator=(DSTDAT&&) = default;
  //~DSTDAT() = default;
};

constexpr auto XCOR = char {0};
constexpr auto YCOR = char {1};

DSTDAT const DSTValues[] = {
    {XCOR, 1}, {XCOR, -1}, {XCOR, 9},  {XCOR, -9},  {YCOR, -9},  {YCOR, 9},  {YCOR, -1}, {YCOR, 1},
    {XCOR, 3}, {XCOR, -3}, {XCOR, 27}, {XCOR, -27}, {YCOR, -27}, {YCOR, 27}, {YCOR, -3}, {YCOR, 3},
    {XCOR, 0}, {XCOR, 0},  {XCOR, 81}, {XCOR, -81}, {YCOR, -81}, {YCOR, 81}};

uint32_t const Xdst[] = {
    0x090a0a, //-121
    0x090a08, //-120
    0x090a09, //-119
    0x09080a, //-118
    0x090808, //-117
    0x090809, //-116
    0x09090a, //-115
    0x090908, //-114
    0x090909, //-113
    0x090a02, //-112
    0x090a00, //-111
    0x090a01, //-110
    0x090802, //-109
    0x090800, //-108
    0x090801, //-107
    0x090902, //-106
    0x090900, //-105
    0x090901, //-104
    0x090a06, //-103
    0x090a04, //-102
    0x090a05, //-101
    0x090806, //-100
    0x090804, //-99
    0x090805, //-98
    0x090906, //-97
    0x090904, //-96
    0x090905, //-95
    0x09020a, //-94
    0x090208, //-93
    0x090209, //-92
    0x09000a, //-91
    0x090008, //-90
    0x090009, //-89
    0x09010a, //-88
    0x090108, //-87
    0x090109, //-86
    0x090202, //-85
    0x090200, //-84
    0x090201, //-83
    0x090002, //-82
    0x090000, //-81
    0x090001, //-80
    0x090102, //-79
    0x090100, //-78
    0x090101, //-77
    0x090206, //-76
    0x090204, //-75
    0x090205, //-74
    0x090006, //-73
    0x090004, //-72
    0x090005, //-71
    0x090106, //-70
    0x090104, //-69
    0x090105, //-68
    0x09060a, //-67
    0x090608, //-66
    0x090609, //-65
    0x09040a, //-64
    0x090408, //-63
    0x090409, //-62
    0x09050a, //-61
    0x090508, //-60
    0x090509, //-59
    0x090602, //-58
    0x090600, //-57
    0x090601, //-56
    0x090402, //-55
    0x090400, //-54
    0x090401, //-53
    0x090502, //-52
    0x090500, //-51
    0x090501, //-50
    0x090606, //-49
    0x090604, //-48
    0x090605, //-47
    0x090406, //-46
    0x090404, //-45
    0x090405, //-44
    0x090506, //-43
    0x090504, //-42
    0x090505, //-41
    0x010a0a, //-40
    0x010a08, //-39
    0x010a09, //-38
    0x01080a, //-37
    0x010808, //-36
    0x010809, //-35
    0x01090a, //-34
    0x010908, //-33
    0x010909, //-32
    0x010a02, //-31
    0x010a00, //-30
    0x010a01, //-29
    0x010802, //-28
    0x010800, //-27
    0x010801, //-26
    0x010902, //-25
    0x010900, //-24
    0x010901, //-23
    0x010a06, //-22
    0x010a04, //-21
    0x010a05, //-20
    0x010806, //-19
    0x010804, //-18
    0x010805, //-17
    0x010906, //-16
    0x010904, //-15
    0x010905, //-14
    0x01020a, //-13
    0x010208, //-12
    0x010209, //-11
    0x01000a, //-10
    0x010008, //-9
    0x010009, //-8
    0x01010a, //-7
    0x010108, //-6
    0x010109, //-5
    0x010202, //-4
    0x010200, //-3
    0x010201, //-2
    0x010002, //-1
    0x010000, // 0
    0x010001, // 1
    0x010102, // 2
    0x010100, // 3
    0x010101, // 4
    0x010206, // 5
    0x010204, // 6
    0x010205, // 7
    0x010006, // 8
    0x010004, // 9
    0x010005, // 10
    0x010106, // 11
    0x010104, // 12
    0x010105, // 13
    0x01060a, // 14
    0x010608, // 15
    0x010609, // 16
    0x01040a, // 17
    0x010408, // 18
    0x010409, // 19
    0x01050a, // 20
    0x010508, // 21
    0x010509, // 22
    0x010602, // 23
    0x010600, // 24
    0x010601, // 25
    0x010402, // 26
    0x010400, // 27
    0x010401, // 28
    0x010502, // 29
    0x010500, // 30
    0x010501, // 31
    0x010606, // 32
    0x010604, // 33
    0x010605, // 34
    0x010406, // 35
    0x010404, // 36
    0x010405, // 37
    0x010506, // 38
    0x010504, // 39
    0x010505, // 40
    0x050a0a, // 41
    0x050a08, // 42
    0x050a09, // 43
    0x05080a, // 44
    0x050808, // 45
    0x050809, // 46
    0x05090a, // 47
    0x050908, // 48
    0x050909, // 49
    0x050a02, // 50
    0x050a00, // 51
    0x050a01, // 52
    0x050802, // 53
    0x050800, // 54
    0x050801, // 55
    0x050902, // 56
    0x050900, // 57
    0x050901, // 58
    0x050a06, // 59
    0x050a04, // 60
    0x050a05, // 61
    0x050806, // 62
    0x050804, // 63
    0x050805, // 64
    0x050906, // 65
    0x050904, // 66
    0x050905, // 67
    0x05020a, // 68
    0x050208, // 69
    0x050209, // 70
    0x05000a, // 71
    0x050008, // 72
    0x050009, // 73
    0x05010a, // 74
    0x050108, // 75
    0x050109, // 76
    0x050202, // 77
    0x050200, // 78
    0x050201, // 79
    0x050002, // 80
    0x050000, // 81
    0x050001, // 82
    0x050102, // 83
    0x050100, // 84
    0x050101, // 85
    0x050206, // 86
    0x050204, // 87
    0x050205, // 88
    0x050006, // 89
    0x050004, // 90
    0x050005, // 91
    0x050106, // 92
    0x050104, // 93
    0x050105, // 94
    0x05060a, // 95
    0x050608, // 96
    0x050609, // 97
    0x05040a, // 98
    0x050408, // 99
    0x050409, // 100
    0x05050a, // 101
    0x050508, // 102
    0x050509, // 103
    0x050602, // 104
    0x050600, // 105
    0x050601, // 106
    0x050402, // 107
    0x050400, // 108
    0x050401, // 109
    0x050502, // 110
    0x050500, // 111
    0x050501, // 112
    0x050606, // 113
    0x050604, // 114
    0x050605, // 115
    0x050406, // 116
    0x050404, // 117
    0x050405, // 118
    0x050506, // 119
    0x050504, // 120
    0x050505  // 121
};

uint32_t const Ydst[] = {
    0x115050, //-121
    0x115010, //-120
    0x115090, //-119
    0x111050, //-118
    0x111010, //-117
    0x111090, //-116
    0x119050, //-115
    0x119010, //-114
    0x119090, //-113
    0x115040, //-112
    0x115000, //-111
    0x115080, //-110
    0x111040, //-109
    0x111000, //-108
    0x111080, //-107
    0x119040, //-106
    0x119000, //-105
    0x119080, //-104
    0x115060, //-103
    0x115020, //-102
    0x1150a0, //-101
    0x111060, //-100
    0x111020, //-99
    0x1110a0, //-98
    0x119060, //-97
    0x119020, //-96
    0x1190a0, //-95
    0x114050, //-94
    0x114010, //-93
    0x114090, //-92
    0x110050, //-91
    0x110010, //-90
    0x110090, //-89
    0x118050, //-88
    0x118010, //-87
    0x118090, //-86
    0x114040, //-85
    0x114000, //-84
    0x114080, //-83
    0x110040, //-82
    0x110000, //-81
    0x110080, //-80
    0x118040, //-79
    0x118000, //-78
    0x118080, //-77
    0x114060, //-76
    0x114020, //-75
    0x1140a0, //-74
    0x110060, //-73
    0x110020, //-72
    0x1100a0, //-71
    0x118060, //-70
    0x118020, //-69
    0x1180a0, //-68
    0x116050, //-67
    0x116010, //-66
    0x116090, //-65
    0x112050, //-64
    0x112010, //-63
    0x112090, //-62
    0x11a050, //-61
    0x11a010, //-60
    0x11a090, //-59
    0x116040, //-58
    0x116000, //-57
    0x116080, //-56
    0x112040, //-55
    0x112000, //-54
    0x112080, //-53
    0x11a040, //-52
    0x11a000, //-51
    0x11a080, //-50
    0x116060, //-49
    0x116020, //-48
    0x1160a0, //-47
    0x112060, //-46
    0x112020, //-45
    0x1120a0, //-44
    0x11a060, //-43
    0x11a020, //-42
    0x11a0a0, //-41
    0x015050, //-40
    0x015010, //-39
    0x015090, //-38
    0x011050, //-37
    0x011010, //-36
    0x011090, //-35
    0x019050, //-34
    0x019010, //-33
    0x019090, //-32
    0x015040, //-31
    0x015000, //-30
    0x015080, //-29
    0x011040, //-28
    0x011000, //-27
    0x011080, //-26
    0x019040, //-25
    0x019000, //-24
    0x019080, //-23
    0x015060, //-22
    0x015020, //-21
    0x0150a0, //-20
    0x011060, //-19
    0x011020, //-18
    0x0110a0, //-17
    0x019060, //-16
    0x019020, //-15
    0x0190a0, //-14
    0x014050, //-13
    0x014010, //-12
    0x014090, //-11
    0x010050, //-10
    0x010010, //-9
    0x010090, //-8
    0x018050, //-7
    0x018010, //-6
    0x018090, //-5
    0x014040, //-4
    0x014000, //-3
    0x014080, //-2
    0x010040, //-1
    0x010000, // 0
    0x010080, // 1
    0x018040, // 2
    0x018000, // 3
    0x018080, // 4
    0x014060, // 5
    0x014020, // 6
    0x0140a0, // 7
    0x010060, // 8
    0x010020, // 9
    0x0100a0, // 10
    0x018060, // 11
    0x018020, // 12
    0x0180a0, // 13
    0x016050, // 14
    0x016010, // 15
    0x016090, // 16
    0x012050, // 17
    0x012010, // 18
    0x012090, // 19
    0x01a050, // 20
    0x01a010, // 21
    0x01a090, // 22
    0x016040, // 23
    0x016000, // 24
    0x016080, // 25
    0x012040, // 26
    0x012000, // 27
    0x012080, // 28
    0x01a040, // 29
    0x01a000, // 30
    0x01a080, // 31
    0x016060, // 32
    0x016020, // 33
    0x0160a0, // 34
    0x012060, // 35
    0x012020, // 36
    0x0120a0, // 37
    0x01a060, // 38
    0x01a020, // 39
    0x01a0a0, // 40
    0x215050, // 41
    0x215010, // 42
    0x215090, // 43
    0x211050, // 44
    0x211010, // 45
    0x211090, // 46
    0x219050, // 47
    0x219010, // 48
    0x219090, // 49
    0x215040, // 50
    0x215000, // 51
    0x215080, // 52
    0x211040, // 53
    0x211000, // 54
    0x211080, // 55
    0x219040, // 56
    0x219000, // 57
    0x219080, // 58
    0x215060, // 59
    0x215020, // 60
    0x2150a0, // 61
    0x211060, // 62
    0x211020, // 63
    0x2110a0, // 64
    0x219060, // 65
    0x219020, // 66
    0x2190a0, // 67
    0x214050, // 68
    0x214010, // 69
    0x214090, // 70
    0x210050, // 71
    0x210010, // 72
    0x210090, // 73
    0x218050, // 74
    0x218010, // 75
    0x218090, // 76
    0x214040, // 77
    0x214000, // 78
    0x214080, // 79
    0x210040, // 80
    0x210000, // 81
    0x210080, // 82
    0x218040, // 83
    0x218000, // 84
    0x218080, // 85
    0x214060, // 86
    0x214020, // 87
    0x2140a0, // 88
    0x210060, // 89
    0x210020, // 90
    0x2100a0, // 91
    0x218060, // 92
    0x218020, // 93
    0x2180a0, // 94
    0x216050, // 95
    0x216010, // 96
    0x216090, // 97
    0x212050, // 98
    0x212010, // 99
    0x212090, // 100
    0x21a050, // 101
    0x21a010, // 102
    0x21a090, // 103
    0x216040, // 104
    0x216000, // 105
    0x216080, // 106
    0x212040, // 107
    0x212000, // 108
    0x212080, // 109
    0x21a040, // 110
    0x21a000, // 111
    0x21a080, // 112
    0x216060, // 113
    0x216020, // 114
    0x2160a0, // 115
    0x212060, // 116
    0x212020, // 117
    0x2120a0, // 118
    0x21a060, // 119
    0x21a020, // 120
    0x21a0a0  // 121
};

#if PESACT
THREAD const PESThread[] = {
    {{0x00, 0x00, 0x00}, "Unknown", ""},                // Index  00
    {{0x1a, 0x0a, 0x94}, "Prussian Blue", "ETP007"},    // Index 01
    {{0x0f, 0x75, 0xff}, "Blue", "ETP405"},             // Index 02
    {{0x00, 0x93, 0x4c}, "Teal Green", "ETP534"},       // Index 03
    {{0xba, 0xbd, 0xfe}, "Corn Flower Blue", "ETP070"}, // Index 04
    {{0xec, 0x00, 0x00}, "Red", "ETP800"},              // Index 05
    {{0xe4, 0x99, 0x5a}, "Reddish Brown", "ETP337"},    // Index 06
    {{0xcc, 0x48, 0xab}, "Magenta", "ETP620"},          // Index 07
    {{0xfd, 0xc4, 0xfa}, "Light Lilac", "ETP810"},      // Index 08
    {{0xdd, 0x84, 0xcd}, "Lilac", "ETP612"},            // Index 09
    {{0x6b, 0xd3, 0x8a}, "Mint Green", "ETP502"},       // Index 10
    {{0xe4, 0xa9, 0x45}, "Deep Gold", "ETP214"},        // Index 11
    {{0xff, 0xbd, 0x42}, "Orange", "ETP208"},           // Index 12
    {{0xff, 0xe6, 0x00}, "Yellow", "ETP205"},           // Index 13
    {{0x6c, 0xd9, 0x00}, "Lime Green", "ETP513"},       // Index 14
    {{0xc1, 0xa9, 0x41}, "Brass", "ETP328"},            // Index 15
    {{0xb5, 0xad, 0x97}, "Silver", "ETP005"},           // Index 16
    {{0xba, 0x9c, 0x5f}, "Russet Brown", "ETP330"},     // Index 17
    {{0xfa, 0xf5, 0x9e}, "Cream Brown", "ETP010"},      // Index 18
    {{0x80, 0x80, 0x80}, "Pewter", "ETP704"},           // Index 19
    {{0x00, 0x00, 0x00}, "Black", "ETP900"},            // Index 20
    {{0x00, 0x1c, 0xdf}, "Ultramarine", "ETP406"},      // Index 21
    {{0xdf, 0x00, 0xb8}, "Royal Purple", "ETP869"},     // Index 22
    {{0x62, 0x62, 0x62}, "Dark Gray", "ETP707"},        // Index 23
    {{0x69, 0x26, 0x0d}, "Dark Brown", "ETP058"},       // Index 24
    {{0xff, 0x00, 0x60}, "Deep Rose", "ETP086"},        // Index 25
    {{0xbf, 0x82, 0x00}, "Light Brown", "ETP323"},      // Index 26
    {{0xf3, 0x91, 0x78}, "Salmon Pink", "ETP079"},      // Index 27
    {{0xff, 0x68, 0x05}, "Vermilion", "ETP030"},        // Index 28
    {{0xf0, 0xf0, 0xf0}, "White", "ETP001"},            // Index 29
    {{0xc8, 0x32, 0xcd}, "Violet", "ETP613"},           // Index 30
    {{0xb0, 0xbf, 0x9b}, "Seacrest", "ETP542"},         // Index 31
    {{0x65, 0xbf, 0xeb}, "Sky Blue", "ETP019"},         // Index 32
    {{0xff, 0xba, 0x04}, "Pumpkin", "ETP126"},          // Index 33
    {{0xff, 0xf0, 0x6c}, "Cream Yellow", "ETP010"},     // Index 34
    {{0xfe, 0xca, 0x15}, "Khaki", "ETP348"},            // Index 35
    {{0xf3, 0x81, 0x01}, "Clay Brown", "ETP339"},       // Index 36
    {{0x37, 0xa9, 0x23}, "Leaf Green", "ETP509"},       // Index 37
    {{0x23, 0x46, 0x5f}, "Peacock Blue", "ETP405"},     // Index 38
    {{0xa6, 0xa6, 0x95}, "Gray", "ETP707"},             // Index 39
    {{0xce, 0xbf, 0xa6}, "Warm Gray", "ETP399"},        // Index 40
    {{0x96, 0xaa, 0x02}, "Dark Olive", "ETP517"},       // Index 41
    {{0xff, 0xe3, 0xc6}, "Linen", "ETP307"},            // Index 42
    {{0xff, 0x99, 0xd7}, "Pink", "ETP085"},             // Index 43
    {{0x00, 0x70, 0x04}, "Deep Green", "ETP808"},       // Index 44
    {{0xed, 0xcc, 0xfb}, "Lavender", "ETP804"},         // Index 45
    {{0xc0, 0x89, 0xd8}, "Wisteria Violet", "ETP607"},  // Index 46
    {{0xe7, 0xd9, 0xb4}, "Beige", "ETP843"},            // Index 47
    {{0xe9, 0x0e, 0x86}, "Carmine", "ETP807"},          // Index 48
    {{0xcf, 0x68, 0x29}, "Amber Red", "ETP333"},        // Index 49
    {{0x40, 0x86, 0x15}, "Olive Green", "ETP519"},      // Index 50
    {{0xdb, 0x17, 0x97}, "Dark Fuchsia", "ETP107"},     // Index 51
    {{0xff, 0xa7, 0x04}, "Tangerine", "ETP209"},        // Index 52
    {{0xb9, 0xff, 0xff}, "Light Blue", "ETP017"},       // Index 53
    {{0x22, 0x89, 0x27}, "Emerald Green", "ETP507"},    // Index 54
    {{0xb6, 0x12, 0xcd}, "Purple", "ETP614"},           // Index 55
    {{0x00, 0xaa, 0x00}, "Moss Green", "ETP515"},       // Index 56
    {{0xfe, 0xa9, 0xdc}, "Flesh Pink", "ETP124"},       // Index 57
    {{0xfe, 0xd5, 0x10}, "Harvest Gold", "ETP206"},     // Index 58
    {{0x00, 0x97, 0xdf}, "Electric Blue", "ETP420"},    // Index 59
    {{0xff, 0xff, 0x84}, "Lemon Yellow", "ETP205"},     // Index 60
    {{0xcf, 0xe7, 0x74}, "Fresh Green", "ETP027"},      // Index 61
    {{0xff, 0xc8, 0x64}, "Applique Material", ""},      // Index 62
    {{0xff, 0xc8, 0xc8}, "Applique Position", ""},      // Index 63
    {{0xff, 0xc8, 0xc8}, "Applique", ""}                // Index 64
};

// clang-format off
char const imageWithFrame[ThumbHeight][ThumbWidth]= { 
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } 
};
// clang-format on

#endif

uint32_t const FillTypes[] = // fill type array for side window display
    {0, VRTF, HORF, ANGF, SATF, CLPF, CONTF, VCLPF, HCLPF, ANGCLPF, FTHF, TXVRTF, TXHORF, TXANGF};
uint32_t const EdgeFillTypes[] = // edge fill type array for side window display
    {0, EDGELINE, EDGEBEAN, EDGECLIP, EDGEANGSAT, EDGEAPPL, EDGEPROPSAT, EDGEBHOL, EDGEPICOT, EDGEDOUBLE, EDGELCHAIN, EDGEOCHAIN, EDGECLIPX};
uint32_t const FeatherFillTypes[] = // feather fill types
    {FTHSIN, FTHSIN2, FTHLIN, FTHPSG, FTHRMP, FTHFAZ};

auto CALLBACK thred::internal::dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
  UNREFERENCED_PARAMETER(lparam);

  switch (umsg) {
	case WM_INITDIALOG: {
	  auto hwnd = GetDlgItem(hwndlg, IDC_DESED);
	  SetWindowText(hwnd, DesignerName->c_str());
	  SetFocus(hwnd);
	  SendMessage(hwnd, EM_SETSEL, 0, -1);
	  break;
	}
	case WM_COMMAND: {
	  switch (LOWORD(wparam)) { // NOLINT
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  auto    hwnd = GetDlgItem(hwndlg, IDC_DESED);
		  wchar_t designerBuffer[50];
		  GetWindowText(hwnd,
		                static_cast<LPTSTR>(designerBuffer),
		                sizeof(designerBuffer) / sizeof(designerBuffer[0]));
		  DesignerName->assign(std::begin(designerBuffer));
		  EndDialog(hwndlg, 0);
		  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
		  return TRUE;
		}
	  }
	}
	default: {
	}
  }
  return FALSE;
}

void thred::internal::getdes() noexcept {
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DESNAM), ThrEdWindow, reinterpret_cast<DLGPROC>(dnamproc)); // NOLINT
}

auto thred::internal::isfclp() noexcept -> bool {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  return clip::isclp(form) && form.fillType != CLPF;
}

auto thred::internal::stlen(uint32_t iStitch) noexcept -> float {
  return hypot(
      StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).x - StitchBuffer->operator[](iStitch).x,
      StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y - StitchBuffer->operator[](iStitch).y);
}

void thred::undat() noexcept {
  if (FormDataSheet != nullptr) {
	DestroyWindow(FormDataSheet);
	FormDataSheet = nullptr;
  }
}

void thred::internal::qchk() {
  if (UserFlagMap.test(UserFlag::MARQ)) {
	CheckMenuItem(MainMenu, ID_MARKESC, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_MARKQ, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_MARKESC, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_MARKQ, MF_CHECKED);
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::nedmen() {
  if (UserFlagMap.test(UserFlag::NEDOF)) {
	CheckMenuItem(MainMenu, ID_SETNEDL, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_RSTNEDL, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_SETNEDL, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_RSTNEDL, MF_UNCHECKED);
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::knotmen() {
  if (UserFlagMap.test(UserFlag::KNOTOF)) {
	CheckMenuItem(MainMenu, ID_KNOTON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_KNOTOF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_KNOTON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_KNOTOF, MF_UNCHECKED);
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::bsavmen() {
  if (UserFlagMap.test(UserFlag::BSAVOF)) {
	CheckMenuItem(MainMenu, ID_BSAVON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_BSAVOF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_BSAVON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_BSAVOF, MF_UNCHECKED);
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::linbmen() {
  if (UserFlagMap.test(UserFlag::LINSPAC)) {
	CheckMenuItem(MainMenu, ID_LINBEXACT, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_LINBEVEN, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_LINBEXACT, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_LINBEVEN, MF_UNCHECKED);
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::wrnmen() {
  auto code = UINT {MF_CHECKED};

  if (UserFlagMap.test(UserFlag::WRNOF)) {
	code = MF_UNCHECKED;
  }
  CheckMenuItem(MainMenu, ID_WARNOF, code);
}

int32_t datcod[] = {ID_CHKOF, ID_CHKON, ID_CHKREP, ID_CHKREPMSG};

void thred::chkmen() noexcept {
  auto lastCode = gsl::narrow_cast<uint32_t>(sizeof(datcod) / sizeof(datcod[0]));
  for (auto iCode = 0U; iCode < lastCode; iCode++) {
	auto const code = (iCode == IniFile.dataCheck) ? UINT {MF_CHECKED} : UINT {MF_UNCHECKED};
	CheckMenuItem(MainMenu, datcod[iCode], code);
  }
}

void thred::duzrat() noexcept {
  if (ZoomRect.right == 0.0F) {
	ZoomRect.right = LHUPX;
  }
  if (ZoomRect.top == 0.0F) {
	ZoomRect.top = LHUPY;
  }
  ZoomRatio.x = gsl::narrow_cast<float>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  ZoomRatio.y = gsl::narrow_cast<float>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
}

auto thred::internal::rsed() noexcept -> uint32_t {
  auto time = SYSTEMTIME {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};

  GetLocalTime(&time);
  return (gsl::narrow_cast<uint32_t>(time.wSecond) << 16U) | time.wMilliseconds;
}

void thred::internal::fnamtabs() {
  for (auto iName = 0U; iName < 50U; iName++) {
	NameOrder[iName] = iName;
  }
  PseudoRandomValue = NORDSED;
  for (auto iName = 0U; iName < 100U; iName++) {
	auto source      = form::psg() % 50U;
	auto destination = form::psg() % 50U;
	std::swap(NameOrder[destination], NameOrder[source]);
  }
  for (auto iName = 0U; iName < 128U; iName++) {
	NameEncoder[iName] = gsl::narrow<uint8_t>(iName) + NCODOF;
  }
  PseudoRandomValue = NCODSED;
  for (auto iName = 0U; iName < 256U; iName++) {
	auto source      = form::psg() & 0x7fU;
	auto destination = form::psg() & 0x7fU;
	std::swap(NameEncoder[destination], NameEncoder[source]);
  }
  auto const fillval = gsl::narrow_cast<uint8_t>(0);
  std::fill(std::begin(NameDecoder), std::end(NameDecoder), fillval);
  for (auto iName = 32U; iName < 127U; iName++) {
	NameDecoder[NameEncoder[iName]] = gsl::narrow<uint8_t>(iName);
  }
}

void thred::internal::ritfnam(std::wstring const& designerName) {
  auto    designer    = utf::Utf16ToUtf8(designerName);
  uint8_t tmpName[50] = {0};

  if (NameOrder[0] > 50U) {
	fnamtabs();
  }
  PseudoRandomValue = rsed();
  auto iName        = 0U;
  for (; iName < 50U; iName++) {
	tmpName[iName] = form::psg() & 0xffU;
  }
  for (iName = 0U; iName < 50U; iName++) {
	if (designer[iName] != 0) {
	  tmpName[iName] = NameEncoder[designer[iName]]; // NOLINT
	}
	else {
	  while (NameDecoder[tmpName[iName]] != 0U) {
		tmpName[iName] = form::psg() & 0xffU;
	  }
	  break;
	}
  }
  if (iName == 50U) {
	while (NameDecoder[tmpName[49]] != 0U) {
	  tmpName[49] = form::psg() & 0xffU;
	}
  }
  for (iName = 0U; iName < 50U; iName++) {
	if (NameOrder[iName] < 50U) {
	  ExtendedHeader->creatorName[NameOrder[iName]] = tmpName[iName];
	}
  }
}

void thred::internal::redfnam(std::wstring& designerName) {
  uint8_t tmpName[50] = {0};
  auto    designer    = std::string {};

  for (auto iName = 0; iName < 50; iName++) {
	if (NameOrder[iName] < 50) {
	  tmpName[iName] = ExtendedHeader->creatorName[NameOrder[iName]];
	}
	else {
	  tmpName[iName] = 111;
	}
  }
  designer.reserve(50U);
  for (auto const character : tmpName) {
	if (NameDecoder[character] != 0U) {
	  designer.push_back(NameDecoder[character]);
	}
	else {
	  break;
	}
  }
  auto decoded = utf::Utf8ToUtf16(designer);
  designerName.assign(decoded);
}

void thred::internal::dstin(uint32_t number, POINT& pout) noexcept {
  // ToDo - what is this code doing?
  auto shift = 1U;
  pout       = {};
  for (auto const& DSTValue : DSTValues) {
	if ((number & shift) != 0U) {
	  if (DSTValue.cor != 0) {
		pout.y += DSTValue.val;
	  }
	  else {
		pout.x += DSTValue.val;
	  }
	}
	shift <<= 1U;
  }
}

auto thred::adflt(uint32_t count) -> uint32_t {
  auto const startVertex = wrap::toUnsigned(FormVertices->size());
  auto const it          = FormVertices->end();
  auto const val         = fPOINT {};
  FormVertices->insert(it, count, val);
  return startVertex;
}

auto thred::adclp(uint32_t count) -> uint32_t {
  auto const iClipPoint = wrap::toUnsigned(ClipPoints->size());
  auto const it         = ClipPoints->end();
  auto const val        = fPOINT {};
  ClipPoints->insert(it, count, val);
  return iClipPoint;
}

auto thred::duthrsh(double threshold) noexcept -> uint32_t {
  auto iZoomLevel = 0U;

  if (threshold > 0.0) {
	auto zoom = 1.0;
	while (zoom > threshold) {
	  zoom *= ZUMFCT;
	  iZoomLevel++;
	}
  }
  else {
	return 0;
  }
  return iZoomLevel + 1U;
}

auto thred::internal::unthrsh(uint32_t level) noexcept -> double {
  auto zoom = 1.0;

  if (level != 0U) {
	level--;
  }
  else {
	return 0.0;
  }
  while (level != 0U) {
	zoom *= ZUMFCT;
	level--;
  }
  return zoom;
}

void thred::ritfcor(fPOINT const& point) {
  displayText::butxt(HCOR, fmt::format(L"x{:.0f} y{:.0f}", (point.x / PFGRAN), (point.y / PFGRAN)));
}

void thred::internal::ritcor(fPOINTATTR const& pointAttribute) {
  auto const point = fPOINT {pointAttribute.x, pointAttribute.y};

  thred::ritfcor(point);
}

void thred::internal::fndknt() {
  auto endStitch = wrap::toUnsigned(StitchBuffer->size());
  if (endStitch > 4) {
	endStitch -= 4;
	// ToDo - Is flag initialized and used correctly?
	auto flag = false;
	KnotCount = 0;
	for (auto iStitch = 0U; iStitch < endStitch; iStitch++) {
	  if ((StitchBuffer->operator[](iStitch).attribute & KNOTMSK) != 0U) {
		if (!flag) {
		  Knots[KnotCount++] = iStitch;
		}
	  }
	  else {
		flag = false;
	  }
	}
  }
  else {
	KnotCount = 0;
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void thred::coltab() {
  if (StitchBuffer->size() > 1) {
	ColorChanges = 0;
	if (!StitchBuffer->empty()) {
	  auto firstStitch = StitchBuffer->begin();
	  firstStitch->attribute &= NCOLMSK;
	  firstStitch->attribute |= (firstStitch + 1)->attribute & COLMSK;
	  auto lastStitch = StitchBuffer->rbegin();
	  lastStitch->attribute &= NCOLMSK;
	  lastStitch->attribute |= (lastStitch + 1)->attribute & COLMSK;
	  auto currentColor = StitchBuffer->front().attribute & COLMSK;
	  for (auto stitchIt = StitchBuffer->begin() + 1U; stitchIt < StitchBuffer->end() - 1U; stitchIt++) {
		if ((stitchIt->attribute & COLMSK) != currentColor) {
		  if (((stitchIt + 1)->attribute & COLMSK) == currentColor) {
			stitchIt->attribute &= NCOLMSK;
			stitchIt->attribute |= currentColor;
		  }
		  currentColor = stitchIt->attribute & COLMSK;
		}
	  }
	  auto iColor      = 0U;
	  auto iStitch     = 0U;
	  currentColor     = 0xffffffff;
	  auto const range = fRECTANGLE {
	      UnzoomedRect.x * -1.0F, UnzoomedRect.y * 2.0F, UnzoomedRect.x * 2.0F, UnzoomedRect.y * -1.0F};
	  for (auto& stitch : *StitchBuffer) {
		if (stitch.x < range.left) {
		  stitch.x = range.left;
		}
		if (stitch.x > range.right) {
		  stitch.x = range.right;
		}
		if (stitch.y > range.top) {
		  stitch.y = range.top;
		}
		if (stitch.y < range.bottom) {
		  stitch.y = range.bottom;
		}
		auto const nextColor = stitch.attribute & COLMSK;
		if (currentColor != nextColor) {
		  ColorChangeTable[iColor].colorIndex =
		      gsl::narrow<decltype(ColorChangeTable[iColor].colorIndex)>(nextColor);
		  ColorChangeTable[iColor++].stitchIndex =
		      gsl::narrow<decltype(ColorChangeTable[iColor].stitchIndex)>(iStitch);
		  currentColor = nextColor;
		}
		iStitch++;
	  }
	  ColorChanges = iColor;
	  ColorChangeTable[iColor].stitchIndex =
	      gsl::narrow<decltype(ColorChangeTable[iColor].stitchIndex)>(StitchBuffer->size());
	  if (ClosestPointIndex > gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U)) {
		ClosestPointIndex = gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U);
	  }
	  thi::fndknt();
	}
  }
}

void thred::internal::ladj() {
  for (auto iLayer = 0U; iLayer < 5U; iLayer++) {
	if (iLayer == ActiveLayer) {
	  EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_GRAYED); // NOLINT
	}
	else {
	  EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_ENABLED); // NOLINT
	}
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::deldu() {
  for (auto& undo : *UndoBuffer) {
	undo.reset(nullptr);
  }
  UndoBufferWriteIndex = 0;
  StateMap.reset(StateFlag::BAKWRAP);
  StateMap.reset(StateFlag::BAKACT);
}

void thred::internal::dudat() {
  auto& undoBuffer = *UndoBuffer;

  undoBuffer[UndoBufferWriteIndex].reset(nullptr);
  auto const&    formList  = *FormList;
  auto const     formCount = wrap::toUnsigned(formList.size());
  constexpr auto formSize  = sizeof(decltype(formList.back()));
  auto const     size      = sizeof(BAKHED) + formSize * formList.size() +
                    sizeof(decltype(StitchBuffer->back())) * StitchBuffer->size() +
                    sizeof(decltype(FormVertices->back())) * FormVertices->size() +
                    sizeof(decltype(ClipPoints->back())) * ClipPoints->size() +
                    sizeof(decltype(SatinGuides->back())) * SatinGuides->size() + sizeof(UserColor) +
                    sizeof(decltype(TexturePointsBuffer->back())) * TexturePointsBuffer->size();
  undoBuffer[UndoBufferWriteIndex] = std::make_unique<uint32_t[]>(size);
  auto backupData                  = convert_ptr<BAKHED*>(undoBuffer[UndoBufferWriteIndex].get());
  if (backupData != nullptr) {
	backupData->zoomRect  = UnzoomedRect;
	backupData->formCount = formCount;
	backupData->forms     = convert_ptr<FRMHED*>(&backupData[1]);
	if (formCount != 0) {
	  auto const dest = gsl::span<FRMHED>(backupData->forms, formList.size());
	  std::copy(formList.cbegin(), formList.cend(), dest.begin());
	}
	backupData->stitchCount = gsl::narrow<decltype(backupData->stitchCount)>(StitchBuffer->size());
	backupData->stitches    = convert_ptr<fPOINTATTR*>(&backupData->forms[formCount]);
	if (!StitchBuffer->empty()) {
	  auto const dest = gsl::span<fPOINTATTR>(backupData->stitches, StitchBuffer->size());
	  std::copy(StitchBuffer->begin(), StitchBuffer->end(), dest.begin());
	}
	backupData->vertexCount = gsl::narrow<decltype(backupData->vertexCount)>(FormVertices->size());
	backupData->vertices    = convert_ptr<fPOINT*>(&backupData->stitches[StitchBuffer->size()]);
	if (!FormVertices->empty()) {
	  auto const dest = gsl::span<fPOINT>(backupData->vertices, FormVertices->size());
	  std::copy(FormVertices->cbegin(), FormVertices->cend(), dest.begin());
	}
	backupData->guideCount = gsl::narrow<decltype(backupData->guideCount)>(SatinGuides->size());
	backupData->guide      = convert_ptr<SATCON*>(&backupData->vertices[FormVertices->size()]);
	if (!SatinGuides->empty()) {
	  auto const dest = gsl::span<SATCON>(backupData->guide, backupData->guideCount);
	  std::copy(SatinGuides->cbegin(), SatinGuides->cend(), dest.begin());
	}
	backupData->clipPointCount = gsl::narrow<decltype(backupData->clipPointCount)>(ClipPoints->size());
	backupData->clipPoints     = convert_ptr<fPOINT*>(&backupData->guide[SatinGuides->size()]);
	if (!ClipPoints->empty()) {
	  auto const dest = gsl::span<fPOINT>(backupData->clipPoints, backupData->clipPointCount);
	  std::copy(ClipPoints->cbegin(), ClipPoints->cend(), dest.begin());
	}
	backupData->colors = convert_ptr<COLORREF*>(&backupData->clipPoints[ClipPoints->size()]);
	{
	  auto const dest = gsl::span<COLORREF>(backupData->colors, COLOR_COUNT);
	  std::copy(std::begin(UserColor), std::end(UserColor), dest.begin());
	}
	backupData->texturePoints     = convert_ptr<TXPNT*>(&backupData->colors[16]);
	backupData->texturePointCount = wrap::toUnsigned(TexturePointsBuffer->size());
	if (!TexturePointsBuffer->empty()) {
	  auto const dest = gsl::span<TXPNT>(backupData->texturePoints, backupData->texturePointCount);
	  std::copy(TexturePointsBuffer->cbegin(), TexturePointsBuffer->cend(), dest.begin());
	}
  }
}

void thred::savdo() {
  StateMap.set(StateFlag::WASDO);
  StateMap.set(StateFlag::CMPDO);
  if (StateMap.testAndReset(StateFlag::SAVACT)) {
	if (StateMap.testAndReset(StateFlag::BAKING)) {
	  StateMap.reset(StateFlag::REDUSHO);
	  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED); // NOLINT
	}
	StateMap.set(StateFlag::BAKACT);
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED); // NOLINT
	StateMap.set(StateFlag::DUMEN);
	thi::dudat();
	UndoBufferWriteIndex++;
	UndoBufferWriteIndex &= 0xfU;
	if (UndoBufferWriteIndex == 0U) {
	  StateMap.set(StateFlag::BAKWRAP);
	}
  }
}

void thred::internal::redfils() {
  auto findData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};

  for (auto const iLRU : LRUMenuId) {
	if (GetMenuState(FileMenu, iLRU, MF_BYCOMMAND) != gsl::narrow_cast<UINT>(-1)) {
	  DeleteMenu(FileMenu, iLRU, MF_BYCOMMAND);
	}
  }
  auto& previousNames = *PreviousNames;
  for (auto iLRU = 0U; iLRU < OLDNUM; iLRU++) {
	if (!previousNames[iLRU].empty()) {
	  if (StateMap.test(StateFlag::SAVAS)) {
		AppendMenu(
		    FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], previousNames[iLRU].wstring().c_str()); // NOLINT
	  }
	  else {
		auto fileHandle = FindFirstFile(previousNames[iLRU].c_str(), &findData);
		if (fileHandle == INVALID_HANDLE_VALUE) { // NOLINT
		  previousNames[iLRU].clear();
		}
		else {
		  AppendMenu(
		      FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], previousNames[iLRU].wstring().c_str()); // NOLINT
		  FindClose(fileHandle);
		}
	  }
	}
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::nunams() {
  *AuxName = *WorkingFileName;
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  AuxName->replace_extension(L".dst");
	  break;
	}
#if PESACT
	case AUXPES: {
	  AuxName->replace_extension(L".pes");
	  break;
	}
#endif
	default: {
	  AuxName->replace_extension(L".pcs");
	  break;
	}
  }
  *ThrName = *WorkingFileName;
  ThrName->replace_extension(L".thr");
  *GeName = *WorkingFileName;
  GeName->replace_extension(L".th*");
  auto  flag          = true;
  auto& previousNames = *PreviousNames;
  for (auto iPrevious = 0U; iPrevious < OLDNUM; iPrevious++) {
	if (previousNames[iPrevious] == *ThrName) {
	  if (iPrevious != 0U) {
		std::swap(previousNames[0], previousNames[iPrevious]);
		flag = false;
		break;
	  }

	  return;
	}
  }
  if (flag) {
	for (auto iPrevious = 0U; iPrevious < OLDNUM; iPrevious++) {
	  if (previousNames[iPrevious].empty()) {
		previousNames[iPrevious].assign(*ThrName);
		flag = false;
		break;
	  }
	}
  }
  if (flag) {
	previousNames[3] = previousNames[2];
	previousNames[2] = previousNames[1];
	previousNames[1] = previousNames[0];
	previousNames[0].assign(*ThrName);
  }
  redfils();
}

void thred::rngadj() {
  auto lastStitch = wrap::toUnsigned(StitchBuffer->size());
  if (lastStitch != 0U) {
	lastStitch--;
	if (ClosestPointIndex > lastStitch) {
	  ClosestPointIndex = lastStitch;
	}
  }
  if (GroupStitchIndex > StitchBuffer->size()) {
	GroupStitchIndex = ClosestPointIndex;
  }
  if (GroupStitchIndex > ClosestPointIndex) {
	GroupStartStitch = ClosestPointIndex;
	GroupEndStitch   = GroupStitchIndex;
  }
  else {
	GroupStartStitch = GroupStitchIndex;
	GroupEndStitch   = ClosestPointIndex;
  }
}

void thred::internal::box(uint32_t iNearest, HDC dc) noexcept {
  auto const boxWidth = BoxOffset[iNearest];
  POINT      line[5]  = {};

  line[0] = {NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y - boxWidth};
  line[1] = {NearestPixel[iNearest].x + boxWidth, NearestPixel[iNearest].y - boxWidth};
  line[2] = {NearestPixel[iNearest].x + boxWidth, NearestPixel[iNearest].y + boxWidth};
  line[3] = {NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y + boxWidth};
  line[4] = {NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y - boxWidth};
  Polyline(dc, static_cast<POINT const*>(line), 5);
}

void thred::internal::boxs() noexcept {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  for (auto iNear = 0U; iNear < NearestCount; iNear++) {
	SelectObject(StitchWindowDC, BoxPen[iNear]);
	box(iNear, StitchWindowDC);
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unboxs() noexcept {
  if (NearestCount != 0U) {
	boxs();
	NearestCount = 0;
  }
}

void thred::internal::stchPars() {
  AspectRatio = gsl::narrow_cast<float>(UnzoomedRect.x) / gsl::narrow_cast<float>(UnzoomedRect.y);
  if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
	StitchWindowSize.x = wrap::round<int32_t>(
	    gsl::narrow_cast<float>(ThredWindowRect.bottom - ((*ScrollSize) * 2)) * AspectRatio);
  }
  else {
	StitchWindowSize.x =
	    wrap::round<int32_t>(gsl::narrow_cast<float>(ThredWindowRect.bottom - *ScrollSize) * AspectRatio);
  }

  if ((StitchWindowSize.x + ButtonWidthX3 + *ScrollSize + *ColorBarSize) < ThredWindowRect.right) {
	if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
	  StitchWindowSize.y = ThredWindowRect.bottom - (*ScrollSize * 2);
	}
	else {
	  StitchWindowSize.y = ThredWindowRect.bottom - *ScrollSize;
	}
  }
  else {
	StitchWindowSize = {ThredWindowRect.right - ButtonWidthX3 - *ColorBarSize,
	                    ThredWindowRect.bottom - ThredWindowRect.top};
	if ((gsl::narrow_cast<float>(StitchWindowSize.x) / gsl::narrow_cast<float>(StitchWindowSize.y)) > AspectRatio) {
	  StitchWindowSize.x = wrap::round<int32_t>(StitchWindowSize.y * AspectRatio);
	}
	else {
	  StitchWindowSize.y = wrap::round<int32_t>(StitchWindowSize.x / AspectRatio);
	}
  }
}

void thred::redraw(HWND window) noexcept {
  RedrawWindow(window, nullptr, nullptr, RDW_INVALIDATE);
  if (window == MainStitchWin) {
	for (auto& iWindow : *DefaultColorWin) {
	  if (iWindow != nullptr) {
		RedrawWindow(iWindow, nullptr, nullptr, RDW_INVALIDATE);
	  }
	}
	RedrawWindow(ColorBar, nullptr, nullptr, RDW_INVALIDATE);
  }
}

void thred::internal::nuRct() noexcept {
  GetClientRect(ThrEdWindow, &ThredWindowRect);
  GetWindowRect(ColorBar, &ColorBarRect);
  if (!ButtonWin->empty()) {
	GetWindowRect(ButtonWin->operator[](HMINLEN), &MinLenRect);
	GetWindowRect(ButtonWin->operator[](HMAXLEN), &MaxLenRect);
  }
  if (ColorBarDC != nullptr) {
	ReleaseDC(ColorBar, ColorBarDC);
	ColorBarDC = GetDC(ColorBar);
  }
  if (StitchWindowMemDC != nullptr) {
	DeleteDC(StitchWindowMemDC);
  }
  if (StitchWindowDC != nullptr) {
	ReleaseDC(MainStitchWin, StitchWindowDC);
	StitchWindowDC = GetDCEx(MainStitchWin, nullptr, DCX_PARENTCLIP | DCX_CLIPSIBLINGS); // NOLINT
	StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
	if (StitchWindowBmp != nullptr) {
	  DeleteObject(StitchWindowBmp);
	}
	GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
  }
  if (ThredDC != nullptr) {
	ReleaseDC(ThrEdWindow, ThredDC);
  }
  if (ThrEdWindow != nullptr) {
	ThredDC = GetDC(ThrEdWindow);
	SetStretchBltMode(ThredDC, COLORONCOLOR);
	GetDCOrgEx(ThredDC, &ThredWindowOrigin);
  }
  if (MainStitchWin != nullptr) {
	GetWindowRect(MainStitchWin, &StitchWindowAbsRect);
	GetClientRect(MainStitchWin, &StitchWindowClientRect);
  }
  if (StitchWindowDC != nullptr) {
	StitchWindowBmp =
	    CreateCompatibleBitmap(StitchWindowDC, StitchWindowClientRect.right, StitchWindowClientRect.bottom);
  }
  if ((StitchWindowMemDC != nullptr) && (StitchWindowBmp != nullptr)) {
	SelectObject(StitchWindowMemDC, StitchWindowBmp);
  }
}

void thred::movStch() {
  auto clientSize = POINT {(ThredWindowRect.right - ButtonWidthX3 - (*ScrollSize + *ColorBarSize)),
                           (ThredWindowRect.bottom)};
  auto verticalOffset = 0;

  thi::unboxs();
  if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
	verticalOffset = *ScrollSize;
	clientSize.y -= *ScrollSize;
  }
  if (StateMap.test(StateFlag::ZUMED)) {
	clientSize.y -= *ScrollSize;
	MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, clientSize.x, clientSize.y, FALSE);
	MoveWindow(VerticalScrollBar, ButtonWidthX3 + clientSize.x, 0, *ScrollSize, clientSize.y, TRUE);
	MoveWindow(HorizontalScrollBar, ButtonWidthX3, clientSize.y + verticalOffset, clientSize.x, *ScrollSize, TRUE);
	StitchWindowAspectRatio =
	    gsl::narrow_cast<float>(clientSize.x) / gsl::narrow_cast<float>(clientSize.y);
	if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
	  MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, clientSize.x, *ScrollSize, TRUE);
	}
	ShowWindow(VerticalScrollBar, TRUE);
	ShowWindow(HorizontalScrollBar, TRUE);
  }
  else {
	thi::stchPars();
	auto const actualWindowHeight = StitchWindowSize.y + *ScrollSize;
	MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, StitchWindowSize.x, actualWindowHeight, TRUE);
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
	StitchWindowAspectRatio =
	    gsl::narrow_cast<float>(StitchWindowSize.x) / gsl::narrow_cast<float>(actualWindowHeight);
	if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
	  MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, StitchWindowSize.x, *ScrollSize, TRUE);
	}
  }
  MoveWindow(ColorBar, ThredWindowRect.right - *ColorBarSize, 0, *ColorBarSize, ThredWindowRect.bottom, TRUE);
  thi::nuRct();
  thred::redraw(ColorBar);
}

void thred::unbsho() {
  if (StateMap.testAndReset(StateFlag::BAKSHO)) {
	for (auto& iBackup : BackupViewer) {
	  DestroyWindow(iBackup);
	}
  }
}

void thred::unsid() noexcept {
  FormMenuChoice = 0;
  if (SideMessageWindow != nullptr) {
	DestroyWindow(SideMessageWindow);
	SideMessageWindow = nullptr;
  }
}

void thred::internal::insadj() {
  StateMap.reset(StateFlag::PRELIN);
  form::refil();
  thred::coltab();
  form::frmout(ClosestFormToCursor);
}

void thred::internal::patdun() {
  StateMap.reset(StateFlag::RUNPAT);
  KillTimer(ThrEdWindow, 0);
  StateMap.set(StateFlag::WASPAT);
  thred::movStch();
  StateMap.set(StateFlag::RESTCH);
  displayText::tabmsg(IDS_END);
}

auto thred::internal::pxchk(double pixelSize) noexcept -> double {
  if (pixelSize < 0.2) {
	return 1.0;
  }
  if (pixelSize > 20) {
	return 20.0;
  }
  return pixelSize;
}

void thred::internal::sizstch(fRECTANGLE& rectangle, std::vector<fPOINTATTR>& stitches) noexcept {
  if (!stitches.empty()) {
	rectangle.bottom = rectangle.top = stitches[0].y;
	rectangle.left = rectangle.right = stitches[0].x;
	for (auto stitch : stitches) {
	  if (stitch.x < rectangle.left) {
		rectangle.left = stitch.x;
	  }
	  if (stitch.x > rectangle.right) {
		rectangle.right = stitch.x;
	  }
	  if (stitch.y < rectangle.bottom) {
		rectangle.bottom = stitch.y;
	  }
	  if (stitch.y > rectangle.top) {
		rectangle.top = stitch.y;
	  }
	}
  }
  else {
	rectangle = {};
  }
}

void thred::internal::zRctAdj() noexcept {
  if (ZoomRect.top > UnzoomedRect.y) {
	auto const adjustment = ZoomRect.top - UnzoomedRect.y;
	ZoomRect.top -= adjustment;
	ZoomRect.bottom -= adjustment;
  }
  if (ZoomRect.bottom < 0) {
	ZoomRect.top -= ZoomRect.bottom;
	ZoomRect.bottom = 0.0F;
  }
  if (ZoomRect.right > UnzoomedRect.x) {
	auto const adjustment = ZoomRect.right - UnzoomedRect.x;
	ZoomRect.right -= adjustment;
	ZoomRect.left -= adjustment;
  }
  if (ZoomRect.left < 0) {
	ZoomRect.right -= ZoomRect.left;
	ZoomRect.left = 0.0F;
  }
}

void thred::shft(fPOINT const& delta) noexcept {
  auto const halfZoomRect =
      fPOINT {((ZoomRect.right - ZoomRect.left) / 2.0F), ((ZoomRect.top - ZoomRect.bottom) / 2.0F)};
  auto const center = fPOINT {(ZoomRect.left + halfZoomRect.x), (ZoomRect.bottom + halfZoomRect.y)};
  auto const shift  = fPOINT {(center.x - delta.x), (center.y - delta.y)};

  ZoomRect.bottom -= shift.y;
  ZoomRect.top -= shift.y;
  ZoomRect.left -= shift.x;
  ZoomRect.right -= shift.x;

  thi::zRctAdj();
}

void thred::internal::stch2px1(uint32_t iStitch) {
  if (!StitchBuffer->empty()) {
	StitchCoordinatesPixels.x =
	    wrap::ceil<int32_t>((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x);
	StitchCoordinatesPixels.y =
	    wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
	                        (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y);
  }
  else {
	// ToDo - what should these values be without the redundant round?
	StitchCoordinatesPixels.x = wrap::round<int32_t>(0.01F);
	StitchCoordinatesPixels.y = wrap::round<int32_t>(StitchWindowClientRect.bottom + 0.01F);
  }
}

void thred::internal::shft2box() {
  SelectedPoint = StitchBuffer->operator[](ClosestPointIndex);
  thred::shft(SelectedPoint);
  stch2px1(ClosestPointIndex);
}

void thred::internal::unbox() {
  if (StateMap.testAndReset(StateFlag::SELBOX)) {
	SelectObject(StitchWindowDC, BoxPen[0]);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, static_cast<POINT const*>(StitchArrow), 3);
	SetROP2(StitchWindowDC, R2_COPYPEN);
  }
}

void thred::internal::ilin() noexcept {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  Polyline(StitchWindowDC, static_cast<POINT const*>(InsertLine), 2);
  SetROP2(StitchWindowDC, R2_XORPEN);
  Polyline(StitchWindowDC, &InsertLine[1], 2);
  SetROP2(StitchWindowDC, R2_COPYPEN);
  SelectObject(StitchWindowMemDC, LinePen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  Polyline(StitchWindowMemDC, static_cast<POINT const*>(InsertLine), 2);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  Polyline(StitchWindowMemDC, &InsertLine[1], 2);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::internal::xlin() {
  if (StateMap.testAndReset(StateFlag::ILIN)) {
	ilin();
  }
}

void thred::internal::ilin1() noexcept {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  Polyline(StitchWindowDC, static_cast<POINT const*>(InsertLine), 2);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::xlin1() {
  if (StateMap.testAndReset(StateFlag::ILIN1)) {
	ilin1();
  }
}

void thred::internal::endpnt() {
  unbox();
  xlin();
  xlin1();
  StateMap.set(StateFlag::LIN1);
  StateMap.set(StateFlag::INSRT);
  StateMap.reset(StateFlag::GRPSEL);
  InsertLine[0] = StitchCoordinatesPixels;
  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
}

void thred::internal::duIns() {
  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
  stch2px1(ClosestPointIndex);
  InsertLine[0] = StitchCoordinatesPixels;
  stch2px1(ClosestPointIndex + 1U);
  InsertLine[2] = StitchCoordinatesPixels;
  xlin();
  StateMap.set(StateFlag::ILIN);
  ilin();
}

void thred::internal::movins() {
  if (StateMap.test(StateFlag::INSRT)) {
	if (StateMap.test(StateFlag::LIN1)) {
	  if (StateMap.test(StateFlag::BAKEND)) {
		stch2px1(wrap::toUnsigned(StitchBuffer->size() - 1U));
	  }
	  else {
		stch2px1(0U);
	  }
	  endpnt();
	}
	else {
	  duIns();
	}
  }
}

void thred::zumhom() {
  ZoomRect = fRECTANGLE {
      0.0F, gsl::narrow_cast<float>(UnzoomedRect.y), gsl::narrow_cast<float>(UnzoomedRect.x), 0.0F};
  ZoomFactor = 1;
  StateMap.reset(StateFlag::ZUMED);
  thred::movStch();
  NearestCount = 0;
  if (StateMap.test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  StateMap.set(StateFlag::RESTCH);
  if (StateMap.test(StateFlag::SELBOX)) {
	thi::shft2box();
  }
  thred::duzrat();
  thi::movins();
}

void thred::internal::hupfn() {
  auto checkHoopRect = fRECTANGLE {}; // for checking the hoop size
  StateMap.reset(StateFlag::HUPCHNG);
  sizstch(checkHoopRect, *StitchBuffer);
  if (!FormList->empty()) {
	if (StitchBuffer->empty()) {
	  auto vertexIt        = FormVertices->cbegin();
	  checkHoopRect.top    = vertexIt->y;
	  checkHoopRect.bottom = vertexIt->y;
	  checkHoopRect.right  = vertexIt->x;
	  checkHoopRect.left   = vertexIt->x;
	}
	for (auto& FormVertice : *FormVertices) {
	  if (FormVertice.x < checkHoopRect.left) {
		checkHoopRect.left = FormVertice.x;
	  }
	  if (FormVertice.x > checkHoopRect.right) {
		checkHoopRect.right = FormVertice.x;
	  }
	  if (FormVertice.y < checkHoopRect.bottom) {
		checkHoopRect.bottom = FormVertice.y;
	  }
	  if (FormVertice.y > checkHoopRect.top) {
		checkHoopRect.top = FormVertice.y;
	  }
	}
  }
  if (!StitchBuffer->empty() || !FormVertices->empty() || StateMap.test(StateFlag::HUPEX)) {
	if (checkHoopRect.left < 0 || checkHoopRect.right > IniFile.hoopSizeX ||
	    checkHoopRect.bottom < 0 || checkHoopRect.top > IniFile.hoopSizeY) {
	  StateMap.set(StateFlag::HUPEX);
	}
	if (StateMap.test(StateFlag::HUPEX)) {
	  auto const hoopSize =
	      fPOINT {checkHoopRect.right - checkHoopRect.left, checkHoopRect.top - checkHoopRect.bottom};
	  if (hoopSize.x > IniFile.hoopSizeX) {
		IniFile.hoopSizeX = hoopSize.x;
		StateMap.set(StateFlag::HUPCHNG);
	  }
	  if (hoopSize.y > IniFile.hoopSizeY) {
		IniFile.hoopSizeY = hoopSize.y;
		StateMap.set(StateFlag::HUPCHNG);
	  }
	  auto const designCenter =
	      fPOINT {hoopSize.x / 2.0F + checkHoopRect.left, hoopSize.y / 2.0F + checkHoopRect.bottom};
	  auto const hoopCenter = fPOINT {IniFile.hoopSizeX / 2.0F, IniFile.hoopSizeY / 2.0F};
	  auto const delta      = fPOINT {hoopCenter.x - designCenter.x, hoopCenter.y - designCenter.y};
	  for (auto& stitch : *StitchBuffer) {
		stitch.x += delta.x;
		stitch.y += delta.y;
	  }
	  for (auto& FormVertice : *FormVertices) {
		FormVertice.x += delta.x;
		FormVertice.y += delta.y;
	  }
	  for (auto& form : *FormList) {
		form.rectangle.left += delta.x;
		form.rectangle.right += delta.x;
		form.rectangle.top += delta.y;
		form.rectangle.bottom += delta.y;
	  }
	  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
	  ZoomMin      = gsl::narrow_cast<float>(MINZUM) / gsl::narrow_cast<float>(UnzoomedRect.x);
	  thred::zumhom();
	}
  }
}

void thred::internal::chkhup() {
  StateMap.set(StateFlag::HUPEX);
  hupfn();
  if (StateMap.test(StateFlag::INIT)) {
	formForms::prfmsg();
  }
  xt::setfchk();
}

void thred::internal::chknum() {
  auto value = 0.0F;
  if (std::wcslen(std::begin(MsgBuffer)) != 0U) {
	value = wrap::bufToFloat(std::begin(MsgBuffer));
  }

  xt::clrstch();
  if (StateMap.testAndReset(StateFlag::NUROT)) {
	if (value != 0.0F) {
	  IniFile.rotationAngle = value * PI_F / 180.0F;
	}
	return;
  }
  if (StateMap.testAndReset(StateFlag::ENTRSEG)) {
	if (value != 0.0F) {
	  IniFile.rotationAngle = 2.0F * PI_F / value;
	}
	return;
  }
  if (MsgIndex != 0U) {
	if (FormMenuChoice != 0U) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  value      = wrap::bufToFloat(std::begin(SideWindowEntryBuffer)) * PFGRAN;
	  switch (FormMenuChoice) {
		case LTXOF: {
		  thred::savdo();
		  form.txof = value;
		  break;
		}
		case LUANG: {
		  thred::savdo();
		  form.underlayStitchAngle = value / 180.0F * PI_F / PFGRAN;
		  break;
		}
		case LUSPAC: {
		  thred::savdo();
		  form.underlaySpacing = value;
		  break;
		}
		case LWLKIND: {
		  thred::savdo();
		  form.underlayIndent = value;
		  break;
		}
		case LULEN: {
		  thred::savdo();
		  form.underlayStitchLen = value;
		  break;
		}
		case LDSTRT: {
		  thred::savdo();
		  form.fillStart = wrap::round<uint32_t>(value / PFGRAN);
		  form.fillStart %= form.vertexCount;
		  break;
		}
		case LDEND: {
		  thred::savdo();
		  form.fillEnd = wrap::round<uint32_t>(value / PFGRAN);
		  form.fillEnd %= form.vertexCount;
		  break;
		}
		case LFTHUPCNT: {
		  thred::savdo();
		  value /= PFGRAN;
		  if (value > 255.0F) {
			value = 255.0F;
		  }
		  form.fillInfo.feather.upCount = wrap::round<uint8_t>(value);
		  break;
		}
		case LFTHCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			form::nufthcol(gsl::narrow_cast<uint32_t>(
			                   (std::wcstol(std::begin(SideWindowEntryBuffer), nullptr, 10) - 1)) &
			               0xfU);
			wrap::setSideWinVal(LFTHCOL);
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LFRMCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			auto colVal =
			    gsl::narrow_cast<uint32_t>((std::wcstol(std::begin(SideWindowEntryBuffer), nullptr, 10) - 1)) & 0xfU;
			form::nufilcol(colVal);
			SetWindowText(ValueWindow->operator[](LFRMCOL), fmt::format(L"{}", colVal + 1U).c_str());
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LUNDCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			auto colVal =
			    gsl::narrow_cast<uint32_t>((std::wcstol(std::begin(SideWindowEntryBuffer), nullptr, 10) - 1)) & 0xfU;
			form.underlayColor = colVal;
			SetWindowText(ValueWindow->operator[](LUNDCOL), fmt::format(L"{}", colVal + 1U).c_str());
			form::refilfn();
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LBRDCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			auto colVal =
			    gsl::narrow_cast<uint32_t>((std::wcstol(std::begin(SideWindowEntryBuffer), nullptr, 10) - 1)) & 0xfU;
			form::nubrdcol(colVal);
			SetWindowText(ValueWindow->operator[](LBRDCOL), fmt::format(L"{}", colVal + 1U).c_str());
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LBRDPIC: {
		  thred::savdo();
		  form.edgeSpacing = value;
		  thred::unsid();
		  wrap::setSideWinVal(LBRDPIC);
		  form::refil();
		  return;
		}
		case LFRMFAZ: {
		  thred::savdo();
		  form.wordParam = wrap::floor<uint32_t>(value / PFGRAN);
		  thred::unsid();
		  wrap::setSideWinVal(LFRMFAZ);
		  form::refil();
		  return;
		}
		case LBRDPOS: {
		  thred::savdo();
		  form.edgeStitchLen = value / PFGRAN;
		  thred::unsid();
		  wrap::setSideWinVal(LBRDPOS);
		  form::refil();
		  return;
		}
		case LMAXFIL: {
		  thred::savdo();
		  form.maxFillStitchLen = value;
		  thred::unsid();
		  wrap::setSideWinVal(LMAXFIL);
		  form::refil();
		  return;
		}
		case LMINFIL: {
		  thred::savdo();
		  form.minFillStitchLen = value;
		  thred::unsid();
		  wrap::setSideWinVal(LMINFIL);
		  form::refil();
		  return;
		}
		case LMAXBRD: {
		  thred::savdo();
		  form.maxBorderStitchLen = value;
		  thred::unsid();
		  wrap::setSideWinVal(LMAXBRD);
		  form::refil();
		  return;
		}
		case LMINBRD: {
		  thred::savdo();
		  form.minBorderStitchLen = value;
		  thred::unsid();
		  wrap::setSideWinVal(LMINBRD);
		  form::refil();
		  return;
		}
		default: {
		}
	  }
	  if (FormMenuChoice == LBCSIZ) {
		thred::savdo();
		if (form.edgeType == EDGEBHOL) {
		  form::savblen(gsl::narrow_cast<float>(value));
		}
		else {
		  form::savplen(gsl::narrow_cast<float>(value));
		}
	  }
	  else {
		if (value != 0.0F) {
		  switch (FormMenuChoice) {
			case LFTHSIZ: {
			  thred::savdo();
			  form.fillInfo.feather.ratio = value / PFGRAN;
			  break;
			}
			case LFTHNUM: {
			  thred::savdo();
			  form.fillInfo.feather.count = wrap::round<uint16_t>(value / PFGRAN);
			  break;
			}
			case LFTHFLR: {
			  thred::savdo();
			  form.fillInfo.feather.minStitchSize = value;
			  break;
			}
			case LFTHDWNCNT: {
			  thred::savdo();
			  value /= PFGRAN;
			  if (value > 255.0F) {
				value = 255.0F;
			  }
			  form.fillInfo.feather.downCount = wrap::round<uint8_t>(value);
			  break;
			}
			case LFRMSPAC: {
			  thred::savdo();
			  form.fillSpacing = value;
			  break;
			}
			case LFRMLEN: {
			  thred::savdo();
			  form.lengthOrCount.stitchLength = value;
			  break;
			}
			case LBRDSPAC: {
			  thred::savdo();
			  auto const edgeType = form.edgeType & NEGUND;
			  switch (edgeType) {
				case EDGEPROPSAT:
				case EDGEOCHAIN:
				case EDGELCHAIN: {
				  form.edgeSpacing = value;
				  break;
				}
				default: {
				  form.edgeSpacing = value / 2.0F;
				}
			  }
			  break;
			}
			case LBRDLEN: {
			  thred::savdo();
			  form.edgeStitchLen = value;
			  break;
			}
			case LBRDSIZ: {
			  thred::savdo();
			  form.borderSize = value;
			  break;
			}
			case LFRMANG: {
			  thred::savdo();
			  form.angleOrClipData.angle = value / 180.0F * PI_F / PFGRAN;
			  break;
			}
			case LSACANG: {
			  thred::savdo();
			  form.satinOrAngle.angle = value / 180.0F * PI_F / PFGRAN;
			  break;
			}
			case LAPCOL: {
			  thred::savdo();
			  form.borderColor &= COLMSK;
			  auto borderColor = wrap::round<uint8_t>(value / PFGRAN);
			  if (borderColor != 0U) {
				borderColor--;
			  }
			  borderColor &= COLMSK;
			  form.borderColor |= (gsl::narrow_cast<uint32_t>(borderColor) << 4U);
			  break;
			}
			default: {
			}
		  }
		}
		else {
		  thred::savdo();
		  if (FormMenuChoice == LFRMSPAC && isfclp()) {
			form.fillSpacing = 0;
		  }
		}
	  }
	  thred::unsid();
	  form::refil();
	  formForms::refrm();
	}
	else {
	  if (PreferenceIndex != 0U) {
		value = wrap::bufToFloat(std::begin(SideWindowEntryBuffer));
		switch (PreferenceIndex - 1) {
		  case PEG: {
			IniFile.eggRatio = value;
			SetWindowText(ValueWindow->operator[](PEG), fmt::format(L"{:.2f}", value).c_str());
			break;
		  }
		  case PNUDG: {
			IniFile.cursorNudgeStep = value;
			IniFile.nudgePixels     = wrap::round<uint16_t>(pxchk(value));
			SetWindowText(ValueWindow->operator[](PNUDG), fmt::format(L"{:.2f}", value).c_str());
			break;
		  }
		  case PPIC: {
			PicotSpacing = value * PFGRAN;
			SetWindowText(ValueWindow->operator[](PPIC), fmt::format(L"{:.2f}", value).c_str());
			break;
		  }
		  case PCLPOF: {
			IniFile.clipOffset = value * PFGRAN;
			SetWindowText(ValueWindow->operator[](PCLPOF), fmt::format(L"{:.2f} mm", value).c_str());
			break;
		  }
		  case PFAZ: {
			IniFile.fillPhase = wrap::floor<uint32_t>(value);
			SetWindowText(ValueWindow->operator[](PFAZ), fmt::format(L"{}", IniFile.fillPhase).c_str());
			break;
		  }
		  case PCHRAT: {
			IniFile.chainRatio = value;
			SetWindowText(ValueWindow->operator[](PCHRAT), fmt::format(L"{:.2f}", value).c_str());
			break;
		  }
		  case PMIN: {
			MinStitchLength = value * PFGRAN;
			SetWindowText(ValueWindow->operator[](PMIN), fmt::format(L"{:.2f}", value).c_str());
			break;
		  }
		  default: {
			if (value != 0.0F) {
			  auto bufVal = fmt::format(L"{:.2f}", value);
			  switch (PreferenceIndex - 1) {
				case PSPAC: {
				  LineSpacing = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PSPAC), bufVal.c_str());
				  break;
				}
				case PANGL: {
				  IniFile.fillAngle = value / 180.0F * PI_F;
				  SetWindowText(ValueWindow->operator[](PANGL), bufVal.c_str());
				  break;
				}
				case PSAT: {
				  BorderWidth         = value * PFGRAN;
				  IniFile.borderWidth = BorderWidth;
				  SetWindowText(ValueWindow->operator[](PSAT), bufVal.c_str());
				  break;
				}
				case PMAX: {
				  IniFile.maxStitchLength = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PMAX), bufVal.c_str());
				  break;
				}
				case PUSE: {
				  UserStitchLength = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PUSE), bufVal.c_str());
				  break;
				}
				case PSMAL: {
				  SmallStitchLength = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PSMAL), bufVal.c_str());
				  break;
				}
				case PAP: {
				  AppliqueColor = wrap::round<uint32_t>(value - 1.0F) % 16U;
				  SetWindowText(ValueWindow->operator[](PAP),
				                fmt::format(L"{}", (AppliqueColor + 1U)).c_str());
				  break;
				}
				case PAPSL: {
				  IniFile.AppStitchLen = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PAPSL), bufVal.c_str());
				  break;
				}
				case PSNP: {
				  SnapLength = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PSNP), bufVal.c_str());
				  break;
				}
				case PSTAR: {
				  StarRatio = value;
				  if (StarRatio > 1.0F) {
					StarRatio = 1.0F;
				  }
				  if (StarRatio < 0.01F) {
					StarRatio = 0.01F;
				  }
				  SetWindowText(ValueWindow->operator[](PSTAR), fmt::format(L"{:.2f}", StarRatio).c_str());
				  break;
				}
				case PLRAT: {
				  IniFile.lensRatio = value;
				  if (IniFile.lensRatio > 10.0F) {
					IniFile.lensRatio = 10.0F;
				  }
				  if (IniFile.lensRatio < 0.1F) {
					IniFile.lensRatio = 0.1F;
				  }
				  SetWindowText(ValueWindow->operator[](PLRAT),
				                fmt::format(L"{:.2f}", IniFile.lensRatio).c_str());
				  break;
				}
				case PSPIR: {
				  SpiralWrap = value;
				  // ToDo - Are these limits correct?
				  if (SpiralWrap > 20.0F) {
					SpiralWrap = 20.0F;
				  }
				  if (SpiralWrap < 0.3F) {
					SpiralWrap = 0.3F;
				  }
				  SetWindowText(ValueWindow->operator[](PSPIR), fmt::format(L"{:.2f}", SpiralWrap).c_str());
				  break;
				}
				case PBUT: {
				  ButtonholeCornerLength = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PBUT), fmt::format(L"{:.2f}", value).c_str());
				  break;
				}
				case PHUPX: {
				  IniFile.hoopSizeX = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PHUPX), fmt::format(L"{:.0f} mm", value).c_str());
				  form::sethup();
				  formForms::prfmsg();
				  chkhup();
				  break;
				}
				case PHUPY: {
				  IniFile.hoopSizeY = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PHUPY), fmt::format(L"{:.0f} mm", value).c_str());
				  form::sethup();
				  formForms::prfmsg();
				  chkhup();
				  break;
				}
				case PGRD: {
				  IniFile.gridSize = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PGRD), fmt::format(L"{:.2f} mm", value).c_str());
				  break;
				}
				case PCHN: {
				  IniFile.chainSpace = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PCHN), fmt::format(L"{:.2f}", value).c_str());
				  break;
				}
			  }
			}
		  }
		}
		thred::unsid();
		PreferenceIndex = 0;
	  }
	  else {
		if (wcslen(std::begin(MsgBuffer)) != 0U) {
		  value = wrap::bufToFloat(std::begin(MsgBuffer));
		  do {
			if (StateMap.testAndReset(StateFlag::ENTRFNUM)) {
			  if (value < FormList->size()) {
				form::frmnumfn(wrap::round<uint32_t>(value));
			  }
			  else {
				displayText::tabmsg(IDS_FRMN1);
			  }
			  return;
			}
			auto uintValue = wrap::floor<uint32_t>(value);
			if (StateMap.testAndReset(StateFlag::ENTRPOL)) {
			  thred::savdo();
			  form::durpoli(uintValue);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTRSTAR)) {
			  thred::savdo();
			  form::dustar(uintValue,
			               250.0F / gsl::narrow<float>(value) * ZoomFactor *
			                   gsl::narrow_cast<float>(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY));
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTRSPIR)) {
			  thred::savdo();
			  form::duspir(uintValue);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTRHART)) {
			  thred::savdo();
			  form::duhart(uintValue);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTRLENS)) {
			  thred::savdo();
			  form::dulens(uintValue);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTREG)) {
			  thred::savdo();
			  form::dueg(uintValue);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTRZIG)) {
			  thred::savdo();
			  form::duzig(uintValue);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::PIXIN)) {
			  IniFile.nudgePixels = wrap::round<uint16_t>(pxchk(value));
			  break;
			}
			if (StateMap.testAndReset(StateFlag::STPXIN)) {
			  IniFile.stitchSizePixels = wrap::round<uint16_t>(pxchk(value));
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FRMPXIN)) {
			  IniFile.formVertexSizePixels = wrap::round<uint16_t>(value);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FRMBOXIN)) {
			  IniFile.formBoxSizePixels = wrap::round<uint16_t>(value);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::GETMIN)) {
			  SmallStitchLength = value * PFGRAN;
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTR30)) {
			  ThreadSize30 = value;
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTR40)) {
			  ThreadSize40 = value;
			  break;
			}
			if (StateMap.testAndReset(StateFlag::ENTR60)) {
			  ThreadSize60 = value;
			  break;
			}
			if (StateMap.testAndReset(StateFlag::SCLPSPAC)) {
			  IniFile.clipOffset = value * PFGRAN;
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFIND)) {
			  xt::dufind(value);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFHI)) {
			  if (value != 0.0) {
				xt::dufhi(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFWID)) {
			  if (value != 0.0) {
				xt::dufwid(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFMAX)) {
			  if (value != 0.0) {
				xt::dufmax(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFMIN)) {
			  xt::dufmin(value * PFGRAN);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETBMAX)) {
			  if (value != 0.0) {
				xt::dubmax(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETBMIN)) {
			  xt::dubmin(value * PFGRAN);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETBSPAC)) {
			  if (value != 0.0) {
				xt::dubspac(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFLEN)) {
			  if (value != 0.0) {
				xt::dublen(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETBCOL)) {
			  xt::dubcol(wrap::round<uint32_t>(value));
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFCOL)) {
			  xt::dufcol(wrap::round<uint32_t>(value));
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETUCOL)) {
			  xt::dundcol(wrap::round<uint32_t>(value));
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFANG)) {
			  xt::dufxang(value);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFSPAC)) {
			  if (value != 0.0) {
				xt::dufspac(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETUANG)) {
			  xt::dufang(value);
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETFLEN)) {
			  if (value != 0.0) {
				xt::duflen(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETUSPAC)) {
			  if (value != 0.0F) {
				xt::duspac(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::FSETULEN)) {
			  if (value != 0.0F) {
				xt::dusulen(value * PFGRAN);
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::GTUANG)) {
			  IniFile.underlayAngle = value / 180.0F * PI_F;
			  break;
			}
			if (StateMap.testAndReset(StateFlag::GTUSPAC)) {
			  if (value != 0.0) {
				IniFile.underlaySpacing = value * PFGRAN;
			  }
			  break;
			}
			if (StateMap.testAndReset(StateFlag::GTWLKIND)) {
			  IniFile.underlayIndent = value * PFGRAN;
			  break;
			}
			if (StateMap.testAndReset(StateFlag::GTWLKLEN)) {
			  if (value != 0.0) {
				IniFile.underlayStitchLen = value * PFGRAN;
			  }
			  break;
			}
		  } while (false);
		}
	  }
	}
  }
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::noMsg() {
  if (DestroyWindow(MsgWindow) != 0) {
	MsgWindow = nullptr;
  }
  DestroyWindow(OKButton);
  DestroyWindow(DiscardButton);
  DestroyWindow(CancelButton);
  DestroyWindow(DeleteStitchesDialog);
  if (GeneralNumberInputBox != nullptr) {
	if (DestroyWindow(GeneralNumberInputBox) != 0) {
	  GeneralNumberInputBox = nullptr;
	}
  }
  if (StateMap.testAndReset(StateFlag::NUMIN) || (FormMenuChoice != 0U) || (PreferenceIndex != 0U)) {
	chknum();
  }
  StateMap.reset(StateFlag::MOVMSG);
  StateMap.reset(StateFlag::DELFRM);
  StateMap.reset(StateFlag::FILMSG);
  StateMap.reset(StateFlag::FORMIN);
  StateMap.set(StateFlag::RESTCH);
}

void thred::unmsg() {
  if (MsgWindow != nullptr) {
	thi::noMsg();
  }
}

GSL_SUPPRESS(26461) auto thred::internal::oldwnd(HWND window) noexcept -> bool {
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	if (window == DefaultColorWin->operator[](iColor) ||
	    UserColorWin->operator[](iColor) == window || ThreadSizeWin[iColor] == window) {
	  return false;
	}
  }
  for (auto iWindow = 0U; iWindow < 9U; iWindow++) {
	if (ButtonWin->operator[](iWindow) == window) {
	  return false;
	}
  }
  if (MainStitchWin == window) {
	return false;
  }
  if (VerticalScrollBar == window) {
	return false;
  }
  if (HorizontalScrollBar == window) {
	return false;
  }
  if (ColorBar == window) {
	return false;
  }
  if (SpeedScrollBar == window) {
	return false;
  }
  return true;
}

auto CALLBACK thred::internal::EnumChildProc(HWND hwnd, LPARAM lParam) noexcept -> BOOL {
  UNREFERENCED_PARAMETER(lParam);

  if (oldwnd(hwnd)) {
	return DestroyWindow(hwnd);
  }
  if (FirstWin != nullptr) {
	if (FirstWin == hwnd) {
	  return FALSE;
	}
	return TRUE;
  }

  FirstWin = hwnd;
  return TRUE;
}

void thred::internal::rstAll() {
  StateMap.reset(StateFlag::WASFPNT);
  StateMap.reset(StateFlag::WASFRMFRM);
  StateMap.reset(StateFlag::FPUNCLP);
  StateMap.reset(StateFlag::FPSEL);
  StateMap.reset(StateFlag::CAPT);
  StateMap.reset(StateFlag::INSRT);
  StateMap.reset(StateFlag::SELBOX);
  StateMap.reset(StateFlag::GRPSEL);
  StateMap.reset(StateFlag::SCROS);
  StateMap.reset(StateFlag::ECROS);
  StateMap.reset(StateFlag::LIN1);
  StateMap.reset(StateFlag::CLPSHO);
  StateMap.reset(StateFlag::SELPNT);
  StateMap.reset(StateFlag::ROTAT);
  StateMap.reset(StateFlag::ROTCAPT);
  StateMap.reset(StateFlag::FRMPMOV);
  StateMap.reset(StateFlag::MOVFRM);
  StateMap.reset(StateFlag::SATCNKT);
  StateMap.reset(StateFlag::FRMPSEL);
  StateMap.reset(StateFlag::SHOINSF);
  StateMap.reset(StateFlag::STRTCH);
  StateMap.reset(StateFlag::SHOSTRTCH);
  StateMap.reset(StateFlag::EXPAND);
  StateMap.reset(StateFlag::POLIMOV);
  StateMap.reset(StateFlag::BZUMIN);
  StateMap.reset(StateFlag::OSAV);
  StateMap.reset(StateFlag::SAVEX);
  StateMap.reset(StateFlag::MOVFRMS);
  StateMap.reset(StateFlag::FRMROT);
  StateMap.reset(StateFlag::DELSFRMS);
  StateMap.reset(StateFlag::BIGBOX);
  StateMap.reset(StateFlag::UPTO);
  StateMap.reset(StateFlag::LENSRCH);
  StateMap.reset(StateFlag::WASGRP);
  StateMap.reset(StateFlag::BOXZUM);
  StateMap.reset(StateFlag::HIDSTCH);
  StateMap.reset(StateFlag::ENTRPOL);
  StateMap.reset(StateFlag::ENTRSTAR);
  StateMap.reset(StateFlag::ENTRSPIR);
  StateMap.reset(StateFlag::ENTRHART);
  StateMap.reset(StateFlag::FORMIN);
  StateMap.reset(StateFlag::GTUANG);
  StateMap.reset(StateFlag::GTUSPAC);
  StateMap.reset(StateFlag::GTWLKIND);
  StateMap.reset(StateFlag::GTWLKLEN);
  trace::untrace();
  StateMap.reset(StateFlag::WASEDG);
  displayText::butxt(HUPTO, StringTable->operator[](STR_UPOF));
  if (ZoomFactor == 1) {
	StateMap.reset(StateFlag::ZUMED);
  }
  thred::movStch();
  ShowWindow(MainStitchWin, TRUE);
  thred::unbsho();
  StateMap.reset(StateFlag::SIDACT);
  thred::unsid();
  StateMap.reset(StateFlag::PRFACT);
  StateMap.reset(StateFlag::WASRT);
  DestroyWindow(PreferencesWindow);
  thred::undat();
  if (StateMap.testAndReset(StateFlag::INSFRM)) {
	insadj();
  }
  StateMap.reset(StateFlag::FUNCLP);
  if (StateMap.testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (StateMap.testAndReset(StateFlag::RUNPAT)) {
	patdun();
  }
  StateMap.reset(StateFlag::FORMSEL);
  StateMap.reset(StateFlag::FRMPSEL);
  thred::unmsg();
  SearchLine->clear();
  SearchLine->shrink_to_fit();
  FirstWin = nullptr;
  while (EnumChildWindows(MainStitchWin, thi::EnumChildProc, 0) != 0) {
  }
}

void thred::ritot(uint32_t number) {
  auto txt         = fmt::format(StringTable->operator[](STR_TOT), number);
  BufferDigitCount = wrap::toUnsigned(txt.size());
  displayText::butxt(HTOT, txt);
}

void thred::internal::frmcalc() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	auto const code      = ClosestFormToCursor << FRMSHFT;
	auto const endStitch = StitchBuffer->size() - 1U;
	auto       maxLength = 0.0;
	auto       minLength = 1e99;
	if (!StitchBuffer->empty()) {
	  for (auto iStitch = 0U; iStitch < endStitch; iStitch++) {
		auto const stitch     = StitchBuffer->operator[](iStitch);
		auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
		if ((stitch.attribute & FRMSK) == code && ((stitch.attribute & NOTFRM) == 0U) &&
		    (stitchFwd1.attribute & FRMSK) == code && ((stitchFwd1.attribute & TYPMSK) != 0U)) {
		  auto const length = std::hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
		  if (length > maxLength) {
			maxLength          = length;
			LargestStitchIndex = iStitch;
		  }
		  if (length < minLength) {
			minLength           = length;
			SmallestStitchIndex = iStitch;
		  }
		}
	  }
	}
	if (fabs(maxLength) < 10000.0) {
	  auto strMax = std::wstring {};
	  displayText::loadString(strMax, IDS_LENMAX);
	  displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength / PFGRAN)));
	}
	if (fabs(minLength) < 10000.0) {
	  auto strMin = std::wstring {};
	  displayText::loadString(strMin, IDS_LENMIN);
	  displayText::butxt(HMINLEN, fmt::format(strMin, (minLength / PFGRAN)));
	}
  }
  else {
	auto const blank = std::wstring {};
	displayText::butxt(HMAXLEN, blank);
	displayText::butxt(HMINLEN, blank);
  }
}

void thred::internal::lenfn(uint32_t start, uint32_t end) {
  auto maxLength = 0.0;
  auto minLength = 1e99;

  SmallestStitchIndex = 0U;
  LargestStitchIndex  = 0U;
  for (auto iStitch = start; iStitch < end; iStitch++) {
	auto const stitch     = StitchBuffer->operator[](iStitch);
	auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	auto const length     = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
	if (length > maxLength) {
	  maxLength          = length;
	  LargestStitchIndex = iStitch;
	}
	if (length < minLength) {
	  minLength           = length;
	  SmallestStitchIndex = iStitch;
	}
  }
  auto strMax = std::wstring {};
  auto strMin = std::wstring {};
  displayText::loadString(strMax, IDS_LENMAX);
  displayText::loadString(strMin, IDS_LENMIN);
  displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength / PFGRAN)));
  displayText::butxt(HMINLEN, fmt::format(strMin, (minLength / PFGRAN)));
}

void thred::internal::lenCalc() {
  auto const blank = std::wstring {};

  if (StateMap.test(StateFlag::LENSRCH)) {
	auto       txt        = std::wstring {};
	auto const stitch     = StitchBuffer->operator[](ClosestPointIndex);
	auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
	auto const lenMax     = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y) / PFGRAN;
	displayText::butxt(HMINLEN, fmt::format(L"{:.2f}", lenMax));
	displayText::loadString(txt, IDS_SRCH);
	displayText::butxt(HMAXLEN, txt);
  }
  else {
	if (StitchBuffer->size() > 1U) {
	  if (StateMap.test(StateFlag::FORMSEL)) {
		frmcalc();
		displayText::butxt(HCOR, blank);
		return;
	  }
	  thred::rngadj();
	  if (StateMap.test(StateFlag::GRPSEL) && GroupStartStitch != GroupEndStitch) {
		lenfn(GroupStartStitch, GroupEndStitch - 1U);
	  }
	  else {
		lenfn(0, wrap::toUnsigned(StitchBuffer->size() - 2U));
	  }
	}
	else {
	  displayText::butxt(HMAXLEN, blank);
	  displayText::butxt(HMINLEN, blank);
	}
  }
}

void thred::internal::delsmal(uint32_t startStitch, uint32_t endStitch) {
  auto const codedAttribute = ClosestFormToCursor << FRMSHFT;
  auto       stitchSize     = 1e38F;

  thred::savdo();
  if (StateMap.test(StateFlag::FORMSEL)) {
	auto iPrevStitch = form::find1st();
	auto iStitch     = iPrevStitch + 1U;
	auto lastStitch  = StitchBuffer->size();

	if (!StitchBuffer->empty()) { // find the first small stitch in the selected form
	  lastStitch--;
	  while (iStitch < lastStitch && stitchSize > SmallStitchLength) {
		auto const stitch     = StitchBuffer->operator[](iStitch);
		auto const prevStitch = StitchBuffer->operator[](iPrevStitch);
		if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == codedAttribute) { // are we still in the selected form?
		  if ((stitch.attribute & KNOTMSK) == 0U) { // is this a knot?
			auto const delta = fPOINT {stitch.x - prevStitch.x, stitch.y - prevStitch.y};
			stitchSize       = hypot(delta.x, delta.y);
			if (stitchSize > SmallStitchLength) {
			  iPrevStitch++;
			  iStitch++;
			}
		  }
		}
		else {
		  return; // we reached the last stitch in the selected form without seeing a small stitch so don't do anything
		}
	  }
	}
	if (iStitch != lastStitch) {
	  iStitch--; // we found a small stich so back up to the start point
	}
	else {
	  return; // we reached the last stitch without seeing a small stitch so don't do anything
	}
	auto iOutputStitch = iStitch;
	auto prevPoint     = StitchBuffer->operator[](startStitch);
	lastStitch         = StitchBuffer->size();
	if (lastStitch != 0U) {
	  lastStitch--;
	  while (iStitch < lastStitch) {
		auto const stitch = StitchBuffer->operator[](iStitch);
		auto outStitch    = StitchBuffer->operator[](iOutputStitch);
		if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == codedAttribute) { // are we still in the selected form?
		  if ((stitch.attribute & KNOTMSK) != 0U) { // is this a knot?
			prevPoint = outStitch;
			outStitch = stitch;
			iOutputStitch++;
		  }
		  else {
			auto const delta = fPOINT {stitch.x - prevPoint.x, stitch.y - prevPoint.y};
			stitchSize       = hypot(delta.x, delta.y);
			if (stitchSize > SmallStitchLength) {
			  outStitch = stitch;
			  prevPoint = stitch;
			  iOutputStitch++;
			}
			else {
			  if (iOutputStitch > lastStitch) {
				break; // we are at the end of the buffer
			  }
			}
		  }
		}
		else {
		  break; // we are no longer in the form
		}
		iStitch++;
	  }
	}
	iOutputStitch++;
	StitchBuffer->erase(std::next(StitchBuffer->begin(), iOutputStitch),
	                    std::next(StitchBuffer->begin(), iStitch));
	thred::coltab();
  }
  else {
	auto iNextStitch = startStitch + 1U;
	auto prevPoint   = StitchBuffer->operator[](startStitch);
	for (auto iStitch = iNextStitch; iStitch < endStitch; iStitch++) {
	  auto const stitch = StitchBuffer->operator[](iStitch);
	  if ((StitchBuffer->operator[](iNextStitch).attribute & KNOTMSK) != 0U) {
		prevPoint = StitchBuffer->operator[](iNextStitch);

		StitchBuffer->operator[](iNextStitch++) = stitch;
	  }
	  else {
		auto const delta = fPOINT {stitch.x - prevPoint.x, stitch.y - prevPoint.y};
		stitchSize       = hypot(delta.x, delta.y);
		if (stitchSize > SmallStitchLength) {
		  StitchBuffer->operator[](iNextStitch++) = stitch;
		  prevPoint                               = stitch;
		}
	  }
	}
	StitchBuffer->erase(std::next(StitchBuffer->begin(), iNextStitch),
	                    std::next(StitchBuffer->begin(), endStitch));
	thred::coltab();
  }
  rstAll();
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  lenCalc();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::duzero() {
  if (!SelectedFormList->empty()) {
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto selectedForm : (*SelectedFormList)) {
	  formMap.set(selectedForm);
	}
	StateMap.reset(StateFlag::CONTIG);
	auto iDestination  = StitchBuffer->begin();
	auto currentStitch = StitchBuffer->front();
	for (auto& iStitch : *StitchBuffer) {
	  if (((iStitch.attribute & TYPMSK) != 0U) && formMap.test((iStitch.attribute & FRMSK) >> FRMSHFT)) {
		if (StateMap.testAndSet(StateFlag::CONTIG)) {
		  auto const stitchLength = hypot(iStitch.x - currentStitch.x, iStitch.y - currentStitch.y);
		  if (stitchLength > MinStitchLength) {
			currentStitch   = iStitch;
			(*iDestination) = iStitch;
			iDestination++;
		  }
		}
		else {
		  currentStitch = iStitch;
		}
	  }
	  else {
		(*iDestination) = iStitch;
		iDestination++;
		StateMap.reset(StateFlag::CONTIG);
	  }
	}
	StitchBuffer->erase(iDestination, StitchBuffer->end());
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (StateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	delsmal(GroupStartStitch, GroupEndStitch);
  }
  else {
	delsmal(0, wrap::toUnsigned(StitchBuffer->size()));
  }
}

void thred::internal::rshft(POINT const& shiftPoint) {
  ZoomRect.top -= shiftPoint.y;
  ZoomRect.left -= shiftPoint.x;
  ZoomRect.right -= shiftPoint.x;
  ZoomRect.bottom -= shiftPoint.y;
  zRctAdj();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::pgdwn() {
  if (StateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, wrap::round<int32_t>((ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	rshft(scrollPosition);
  }
}

void thred::internal::pgup() {
  if (StateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, wrap::round<int32_t>(-(ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	rshft(scrollPosition);
  }
}

void thred::internal::pglft() {
  if (StateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {wrap::round<int32_t>((ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
	rshft(scrollPosition);
  }
}

void thred::internal::pgrit() {
  if (StateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition =
	    POINT {wrap::round<int32_t>(-(ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
	rshft(scrollPosition);
  }
}

void thred::internal::selin(uint32_t start, uint32_t end, HDC dc) {
  SelectObject(dc, GroupSelectPen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (!SearchLine->empty()) {
	wrap::Polyline(dc, SearchLine->data(), wrap::toUnsigned(SearchLine->size()));
  }
  if (start > end) {
	std::swap(start, end);
  }
  SearchLine->clear();
  if (!StitchBuffer->empty()) {
	for (auto iStitch = start; iStitch <= end; iStitch++) {
	  SearchLine->push_back(POINT {
	      wrap::ceil<int32_t>(((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x)),
	      wrap::ceil<int32_t>((StitchWindowClientRect.bottom -
	                           (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y))});
	}
	wrap::Polyline(dc, SearchLine->data(), wrap::toUnsigned(SearchLine->size()));
  }
  SetROP2(dc, R2_COPYPEN);
}

void thred::internal::cros(uint32_t iStitch) {
  auto const armLength = BoxOffset[0];

  stch2px1(iStitch);
  InsertLine[0] = {StitchCoordinatesPixels.x - armLength, StitchCoordinatesPixels.y};
  InsertLine[1] = {StitchCoordinatesPixels.x + armLength, StitchCoordinatesPixels.y};
  SelectObject(StitchWindowDC, CrossPen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  Polyline(StitchWindowDC, static_cast<POINT const*>(InsertLine), 2);
  SelectObject(StitchWindowMemDC, CrossPen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  Polyline(StitchWindowMemDC, static_cast<POINT const*>(InsertLine), 2);
  InsertLine[0] = {StitchCoordinatesPixels.x, StitchCoordinatesPixels.y - armLength};
  InsertLine[1] = {StitchCoordinatesPixels.x, StitchCoordinatesPixels.y - 1};
  Polyline(StitchWindowDC, static_cast<POINT const*>(InsertLine), 2);
  Polyline(StitchWindowMemDC, static_cast<POINT const*>(InsertLine), 2);
  InsertLine[0].y = StitchCoordinatesPixels.y + 2;
  InsertLine[1].y = StitchCoordinatesPixels.y + armLength;
  Polyline(StitchWindowDC, static_cast<POINT const*>(InsertLine), 2);
  SetROP2(StitchWindowDC, R2_COPYPEN);
  Polyline(StitchWindowMemDC, static_cast<POINT const*>(InsertLine), 2);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::internal::uncros() {
  if (StateMap.testAndReset(StateFlag::SCROS)) {
	cros(GroupStartStitch);
  }
  if (StateMap.testAndReset(StateFlag::ECROS)) {
	if (GroupEndStitch != GroupStartStitch) {
	  cros(GroupEndStitch);
	}
	StateMap.set(StateFlag::ELIN);
  }
}

void thred::internal::ducros(HDC dc) {
  uncros();
  StateMap.set(StateFlag::SCROS);
  cros(GroupStartStitch);
  if (GroupStartStitch != GroupEndStitch) {
	cros(GroupEndStitch);
	StateMap.set(StateFlag::ECROS);
  }
  selin(GroupStartStitch, GroupEndStitch, dc);
}

void thred::selRct(fRECTANGLE& sourceRect) noexcept {
  if (!StitchBuffer->empty()) {
	sourceRect.left = sourceRect.right = StitchBuffer->operator[](GroupStartStitch).x;
	sourceRect.top = sourceRect.bottom = StitchBuffer->operator[](GroupStartStitch).y;
	for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; iStitch++) {
	  auto const stitch = StitchBuffer->operator[](iStitch);
	  if (stitch.x > sourceRect.right) {
		sourceRect.right = stitch.x;
	  }
	  if (stitch.x < sourceRect.left) {
		sourceRect.left = stitch.x;
	  }
	  if (stitch.y < sourceRect.bottom) {
		sourceRect.bottom = stitch.y;
	  }
	  if (stitch.y > sourceRect.top) {
		sourceRect.top = stitch.y;
	  }
	}
  }
  if (sourceRect.right - sourceRect.left == 0) {
	sourceRect.right++;
	sourceRect.left--;
  }
  if (sourceRect.top - sourceRect.bottom == 0) {
	sourceRect.top++;
	sourceRect.bottom--;
  }
}

void thred::internal::dusel(HDC dc) {
  SetROP2(dc, R2_NOTXORPEN);
  SelectObject(dc, LinePen);
  wrap::Polyline(dc, FormControlPoints->data(), wrap::toUnsigned(FormControlPoints->size() - 1U));
  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(FormControlPoints->size() - 1U); iPoint++) {
	form::selsqr(FormControlPoints->operator[](iPoint), dc);
  }
  SetROP2(dc, R2_COPYPEN);
}

void thred::internal::unsel() {
  if (StateMap.testAndReset(StateFlag::SELSHO)) {
	dusel(StitchWindowDC);
  }
}

void thred::internal::nuselrct() {
  auto outline = std::vector<fPOINT> {};
  outline.resize(FormControlPoints->size());

  unsel();
  outline[0].x = outline[6].x = outline[7].x = outline[8].x = StitchRangeRect.left;
  outline[1].x = outline[5].x = form::midl(StitchRangeRect.right, StitchRangeRect.left);
  outline[0].y = outline[1].y = outline[2].y = outline[8].y = StitchRangeRect.top;
  outline[3].y = outline[7].y = form::midl(StitchRangeRect.top, StitchRangeRect.bottom);
  outline[4].y = outline[5].y = outline[6].y = StitchRangeRect.bottom;
  outline[2].x = outline[3].x = outline[4].x = StitchRangeRect.right;

  auto iPoint = outline.begin();
  for (auto& controlPoint : *FormControlPoints) {
	form::sfCor2px(*iPoint, controlPoint);
	iPoint++;
  }
}

void thred::grpAdj() {
  thi::uncros();
  thi::unsel();
  thred::rngadj();
  thi::ducros(StitchWindowDC);
  thi::lenCalc();
  thred::selRct(StitchRangeRect);
  if (StateMap.test(StateFlag::ZUMED) && GroupEndStitch != GroupStartStitch) {
	if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1 ||
	    StitchRangeRect.left < ZoomRect.left + 1 || StitchRangeRect.right > ZoomRect.right - 1) {
	  auto newSize = POINT {wrap::round<int32_t>(StitchRangeRect.right - StitchRangeRect.left),
	                        wrap::round<int32_t>(StitchRangeRect.top - StitchRangeRect.bottom)};

	  auto coordinate = 0.0F;
	  if (newSize.x < MINZUM) {
		if (newSize.x < 1) {
		  newSize.x = 1;
		}
		coordinate = gsl::narrow_cast<decltype(coordinate)>(MINZUM) / newSize.x;
		newSize    = {MINZUM, wrap::round<int32_t>(coordinate * newSize.y)};
	  }
	  if (newSize.x > newSize.y) {
		coordinate = newSize.x * ZMARGIN;
		newSize.x += wrap::round<int32_t>(coordinate);
		coordinate = newSize.x / StitchWindowAspectRatio;
		newSize.y  = wrap::round<int32_t>(coordinate);
	  }
	  else {
		coordinate = newSize.y * ZMARGIN;
		newSize.y  = wrap::round<int32_t>(coordinate);
		coordinate = newSize.y * StitchWindowAspectRatio;
		newSize.x  = wrap::round<int32_t>(coordinate);
	  }
	  if (newSize.x > UnzoomedRect.x || newSize.y > UnzoomedRect.y) {
		ZoomRect.left = ZoomRect.bottom = 0.0F;
		ZoomRect.right                  = UnzoomedRect.x;
		ZoomRect.top                    = UnzoomedRect.y;
		StateMap.reset(StateFlag::ZUMED);
		ZoomFactor = 1.0;
		thred::movStch();
	  }
	  else {
		ZoomRect.right = ZoomRect.left + (newSize.x);
		ZoomFactor   = gsl::narrow_cast<float>(newSize.x) / gsl::narrow_cast<float>(UnzoomedRect.x);
		ZoomRect.top = ZoomRect.bottom + (newSize.y);
		SelectedPoint.x = ((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left;
		SelectedPoint.y = ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom;
		thred::shft(SelectedPoint);
	  }
	}
  }
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::nuAct(uint32_t iStitch) noexcept {
  auto const color = ActiveColor;
  if (!StitchBuffer->empty()) {
	ActiveColor = StitchBuffer->operator[](iStitch).attribute & COLMSK;
  }
  else {
	ActiveColor = 0;
  }
  thred::redraw(UserColorWin->operator[](color));
  thred::redraw(UserColorWin->operator[](ActiveColor));
}

void thred::internal::lensadj() {
  uncros();
  unsel();
  ClosestPointIndex = CurrentStitchIndex;
  GroupStitchIndex  = CurrentStitchIndex + 1U;
  thred::rngadj();
  ducros(StitchWindowDC);
  lenCalc();
  thred::selRct(StitchRangeRect);
  if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1 ||
      StitchRangeRect.left < ZoomRect.left + 1 || StitchRangeRect.right > ZoomRect.right - 1) {
	SelectedPoint.x = ((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left;
	SelectedPoint.y = ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom;
	thred::shft(SelectedPoint);
  }
  nuAct(GroupStartStitch);
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::ritlayr() {
  auto layer = 0xffffffffU;

  if (StateMap.test(StateFlag::SELBOX)) {
	if (!StitchBuffer->empty()) {
	  layer = (StitchBuffer->operator[](ClosestPointIndex).attribute & LAYMSK) >> LAYSHFT;
	}
  }
  else {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::FRMPSEL)) {
	  layer = gsl::narrow_cast<decltype(layer)>(FormList->operator[](ClosestFormToCursor).attribute & FRMLMSK) >> 1U;
	}
  }
  if ((layer & 0xffff0000U) != 0U) {
	auto const blank = std::wstring {};
	displayText::butxt(HLAYR, blank);
  }
  else {
	auto txt         = fmt::format(StringTable->operator[](STR_LAYR), layer);
	BufferDigitCount = wrap::toUnsigned(txt.size());
	displayText::butxt(HLAYR, txt);
  }
}

auto thred::internal::nuPen(HPEN pen, uint32_t width, COLORREF color) noexcept -> HPEN {
  DeleteObject(pen);
  return wrap::CreatePen(PS_SOLID, width, color);
}

void thred::internal::nuStchSiz(uint32_t iColor, uint32_t width) noexcept {
  if (width != ThreadSizePixels[iColor]) {
	UserPen[iColor]          = nuPen(UserPen[iColor], width, UserColor[iColor]);
	ThreadSizePixels[iColor] = width;
  }
}

auto thred::internal::nuBrush(HBRUSH brush, COLORREF color) noexcept -> HBRUSH {
  DeleteObject(brush);
  return CreateSolidBrush(color);
}

void thred::internal::rotpix(POINT const& unrotatedPoint, POINT& rotatedPoint, POINT const& rotationCenterPixels) {
  // won't handle vertical lines

  auto const dx               = unrotatedPoint.x - rotationCenterPixels.x;
  auto const dy               = unrotatedPoint.y - rotationCenterPixels.y;
  auto const distanceToCenter = hypot(dx, dy);
  auto const newAngle         = atan2(dy, dx) - RotateAngle;

  rotatedPoint = {wrap::round<int32_t>(rotationCenterPixels.x + distanceToCenter * cos(newAngle)),
                  wrap::round<int32_t>(rotationCenterPixels.y + distanceToCenter * sin(newAngle))};
}

void thred::internal::duar() {
  auto const offset = gsl::narrow<int32_t>(MulDiv(10, *screenDPI, 96));
  auto arrowCenter = POINT {(StitchCoordinatesPixels.x - offset), (StitchCoordinatesPixels.y + offset)};
  StitchArrow[1] = StitchCoordinatesPixels;
  rotpix(arrowCenter, StitchArrow[0], StitchCoordinatesPixels);
  arrowCenter.y = StitchCoordinatesPixels.y - offset;
  rotpix(arrowCenter, StitchArrow[2], StitchCoordinatesPixels);
  SelectObject(StitchWindowMemDC, BoxPen[0]);
  SelectObject(StitchWindowDC, BoxPen[0]);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  Polyline(StitchWindowMemDC, static_cast<POINT const*>(StitchArrow), 3);
  Polyline(StitchWindowDC, static_cast<POINT const*>(StitchArrow), 3);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::dubox() {
  if (!StitchBuffer->empty()) {
	auto const stitch = StitchBuffer->operator[](ClosestPointIndex);
	if (ClosestPointIndex != (StitchBuffer->size() - 1U)) {
	  // if the selected point is not at the end then aim at the next point
	  auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
	  RotateAngle           = atan2(stitchFwd1.y - stitch.y, stitchFwd1.x - stitch.x);
	}
	else { // otherwise aim in the same direction
	  auto const stitchBck1 = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) - 1U);
	  RotateAngle           = atan2(stitch.y - stitchBck1.y, stitch.x - stitchBck1.x);
	}
	duar();
	StateMap.reset(StateFlag::ELIN);
	StateMap.set(StateFlag::SELBOX);
	StateMap.reset(StateFlag::FRMPSEL);
	thred::redraw(ColorBar);
	displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
  }
}

auto thred::internal::stch2px(uint32_t iStitch) -> bool {
  StitchCoordinatesPixels = {
      wrap::ceil<int32_t>((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x),
      wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                          (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y)};
  return StitchCoordinatesPixels.x >= 0 && StitchCoordinatesPixels.x <= StitchWindowClientRect.right &&
         StitchCoordinatesPixels.y >= 0 && StitchCoordinatesPixels.y <= StitchWindowClientRect.bottom;
}

void thred::stch2pxr(fPOINT const& stitchCoordinate) {
  StitchCoordinatesPixels = {wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                             wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                                                 (stitchCoordinate.y - ZoomRect.bottom) * ZoomRatio.y)};
}

void thred::internal::getDocsFolder(fs::path* directory) {
  if (directory != nullptr) {
	auto       ppszPath = PWSTR {nullptr}; // variable to receive the path memory block pointer.
	auto const hr       = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &ppszPath);
	if (SUCCEEDED(hr)) {
	  directory->assign(ppszPath); // make a local copy of the path
	}
	CoTaskMemFree(ppszPath); // free up the path memory block
  }
}

void thred::internal::defNam(fs::path const& fileName) {
  if (fileName.empty()) {
	getDocsFolder(DefaultDirectory);
  }
  else {
	*DefaultDirectory = fileName.parent_path();
  }
}

void thred::internal::defbNam() {
  if (!UserBMPFileName->empty()) {
	*DefaultBMPDirectory = UserBMPFileName->parent_path();
  }
}

void thred::internal::ritini() {
  auto           directory        = utf::Utf16ToUtf8(DefaultDirectory->wstring());
  auto const     defaultDirectory = gsl::span<char> {IniFile.defaultDirectory};
  constexpr char fillchar         = '\0';
  std::fill(defaultDirectory.begin(), defaultDirectory.end(), fillchar);
  std::copy(directory.cbegin(), directory.cend(), defaultDirectory.begin());
  auto const& previousNames = *PreviousNames;
  {
	auto iVersion = 0U;
	for (auto& prevName : IniFile.prevNames) {
	  auto const name = gsl::span<char> {prevName};
	  std::fill(name.begin(), name.end(), fillchar);
	  if (!previousNames[iVersion].empty()) {
		auto previous = utf::Utf16ToUtf8(previousNames[iVersion]);
		std::copy(previous.cbegin(), previous.cend(), name.begin());
	  }
	  iVersion++;
	}
  }
  auto       designer     = utf::Utf16ToUtf8(*DesignerName);
  auto const designerName = gsl::span<char> {IniFile.designerName};
  std::fill(designerName.begin(), designerName.end(), fillchar);
  std::copy(designer.cbegin(), designer.cend(), designerName.begin());
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	IniFile.stitchColors[iColor]              = UserColor[iColor];
	IniFile.backgroundPreferredColors[iColor] = CustomBackgroundColor[iColor];
	IniFile.stitchPreferredColors[iColor]     = CustomColor[iColor];
	IniFile.bitmapBackgroundColors[iColor]    = BitmapBackgroundColors[iColor];
  }
  IniFile.backgroundColor = BackgroundColor;
  IniFile.bitmapColor     = BitmapColor;
  IniFile.minStitchLength = MinStitchLength;
  if (ShowStitchThreshold < 0.0) {
	ShowStitchThreshold = 0.0;
  }
  if (ShowStitchThreshold > 9.0) {
	ShowStitchThreshold = 9.0;
  }
  IniFile.showStitchThreshold    = ShowStitchThreshold;
  IniFile.threadSize30           = ThreadSize30;
  IniFile.threadSize40           = ThreadSize40;
  IniFile.threadSize60           = ThreadSize60;
  IniFile.userStitchLength       = UserStitchLength;
  IniFile.smallStitchLength      = SmallStitchLength;
  IniFile.stitchBoxesThreshold   = StitchBoxesThreshold;
  IniFile.stitchSpace            = LineSpacing;
  IniFile.userFlagMap            = UserFlagMap.to_ulong();
  IniFile.borderWidth            = BorderWidth;
  IniFile.appliqueColor          = AppliqueColor;
  IniFile.snapLength             = SnapLength;
  IniFile.starRatio              = StarRatio;
  IniFile.spiralWrap             = SpiralWrap;
  IniFile.buttonholeCornerLength = ButtonholeCornerLength;
  IniFile.picotSpace             = PicotSpacing;
  if (!UserFlagMap.test(UserFlag::SAVMAX)) {
	auto windowRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(ThrEdWindow, &windowRect);
	IniFile.initialWindowCoords.left   = windowRect.left;
	IniFile.initialWindowCoords.right  = windowRect.right;
	IniFile.initialWindowCoords.bottom = windowRect.bottom;
	IniFile.initialWindowCoords.top    = windowRect.top;
  }
  IniFileHandle = CreateFile(
      IniFileName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT
  if (IniFileHandle != INVALID_HANDLE_VALUE) { // NOLINT
	WriteFile(IniFileHandle, &IniFile, sizeof(IniFile), &BytesRead, nullptr);
  }
  CloseHandle(IniFileHandle);
}

auto thred::internal::savcmp() noexcept -> bool {
#ifdef _DEBUG

  return true;

#else

  return !StateMap.test(StateFlag::CMPDO);

#endif
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440)
void thred::internal::thr2bal(std::vector<BALSTCH>& balaradStitch, uint32_t destination, uint32_t source, uint32_t code) {
  constexpr auto BalaradRatio = 10.0F / 6.0F;

  balaradStitch[destination].flag = 0;
  balaradStitch[destination].code = gsl::narrow<uint8_t>(code);
  balaradStitch[destination].x = (StitchBuffer->operator[](source).x - BalaradOffset.x) * BalaradRatio;
  balaradStitch[destination].y = (StitchBuffer->operator[](source).y - BalaradOffset.y) * BalaradRatio;
}

auto thred::internal::coldis(COLORREF colorA, COLORREF colorB) -> DWORD {
  auto color1 = PECCOLOR {GetRValue(colorA), GetGValue(colorA), GetBValue(colorA)}; // NOLINT
  auto color2 = PECCOLOR {GetRValue(colorB), GetGValue(colorB), GetBValue(colorB)}; // NOLINT

  auto const meanR = (gsl::narrow_cast<int32_t>(color1.r) + gsl::narrow_cast<int32_t>(color2.r)) / 2;
  auto       deltaR = gsl::narrow_cast<int32_t>(color1.r) - gsl::narrow_cast<int32_t>(color2.r);
  auto       deltaG = gsl::narrow_cast<int32_t>(color1.g) - gsl::narrow_cast<int32_t>(color2.g);
  auto       deltaB = gsl::narrow_cast<int32_t>(color1.b) - gsl::narrow_cast<int32_t>(color2.b);
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula
  return wrap::round<DWORD>(std::sqrt((((512 + meanR) * deltaR * deltaR) / 256) + 4 * deltaG * deltaG +
                                      (((767 - meanR) * deltaB * deltaB) / 256)));
}

auto thred::internal::colmatch(COLORREF color) -> uint32_t {
  if (ColorChanges < 16U) {
	for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
	  if (color == UserColor[iColor]) {
		return iColor;
	  }
	}
	UserColor[ColorChanges++] = color;
	return ColorChanges - 1U;
  }

  auto minDistance = DWORD {0xffffffffU};
  auto iDistance   = DWORD {0x00000000U};
  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
	auto const distance = coldis(color, UserColor[iColor]);
	if (distance == 0U) {
	  return iColor;
	}
	iDistance = 0U;
	if (distance < minDistance) {
	  iDistance   = iColor;
	  minDistance = distance;
	}
  }
  return iDistance;
}

void thred::internal::redbal() {
  auto balaradHeader = BALHED {};

  StitchBuffer->clear();
  FormList->clear();

  auto balaradFile =
      CreateFile(BalaradName2->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (balaradFile != INVALID_HANDLE_VALUE) { // NOLINT
	auto bytesRead = DWORD {0U};
	ReadFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesRead, nullptr);
	if (bytesRead == sizeof(balaradHeader)) {
	  auto balaradStitch = std::vector<BALSTCH> {};
	  balaradStitch.resize(MAXITEMS);
	  ReadFile(balaradFile, balaradStitch.data(), MAXITEMS * sizeof(decltype(balaradStitch.back())), &bytesRead, nullptr);
	  auto const stitchCount  = bytesRead / sizeof(decltype(balaradStitch.back()));
	  BackgroundColor         = balaradHeader.backgroundColor;
	  IniFile.backgroundColor = balaradHeader.backgroundColor;
	  BackgroundPen           = nuPen(BackgroundPen, 1, BackgroundColor);
	  BackgroundPenWidth      = 1;
	  DeleteObject(BackgroundBrush);
	  BackgroundBrush        = CreateSolidBrush(BackgroundColor);
	  constexpr auto IBALRAT = 0.6F;
	  IniFile.hoopSizeX      = balaradHeader.hoopSizeX * IBALRAT;
	  IniFile.hoopSizeY      = balaradHeader.hoopSizeY * IBALRAT;
	  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
	  BalaradOffset.x    = IniFile.hoopSizeX / 2.0F;
	  BalaradOffset.y    = IniFile.hoopSizeY / 2.0F;
	  PCSHeader.hoopType = CUSTHUP;
	  IniFile.hoopType   = CUSTHUP;
	  UserColor[0]       = balaradHeader.color[0];
	  auto color         = 0U;
	  auto bColor        = 1U;
	  ColorChanges       = 1U;
	  for (auto iStitch = 0U; iStitch < stitchCount; iStitch++) {
		switch (balaradStitch[iStitch].code) {
		  case BALNORM: {
			StitchBuffer->push_back(fPOINTATTR {balaradStitch[iStitch].x * IBALRAT + BalaradOffset.x,
			                                    balaradStitch[iStitch].y * IBALRAT + BalaradOffset.y,
			                                    color});
			break;
		  }
		  case BALSTOP: {
			color = colmatch(balaradHeader.color[bColor++]);
			break;
		  }
		}
	  }
	  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
		UserPen[iColor]        = wrap::CreatePen(PS_SOLID, 1, UserColor[iColor]);
		UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	  }
	  thred::coltab();
	  thred::redraw(ColorBar);
	  StateMap.set(StateFlag::INIT);
	  StateMap.set(StateFlag::RESTCH);
	}
  }
  CloseHandle(balaradFile);
}

void thred::internal::ritbal() {
  auto           balaradHeader = BALHED {};
  constexpr auto balaradRatio  = 10.0F / 6.0F;

  if (!BalaradName0->empty() && !BalaradName1->empty() && (!StitchBuffer->empty())) {
	auto outputName = (WorkingFileName->empty()) ? (*DefaultDirectory / L"balfil.thr") : *WorkingFileName;
	outputName.replace_extension(L".thv");
	auto balaradFile =
	    CreateFile(outputName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (balaradFile == INVALID_HANDLE_VALUE) { // NOLINT
	  return;
	}
	auto color             = StitchBuffer->front().attribute & COLMSK;
	balaradHeader.color[0] = UserColor[color];
	auto iColor            = 1U;
	for (auto& stitch : *StitchBuffer) {
	  if (color != (stitch.attribute & COLMSK)) {
		color                         = stitch.attribute & COLMSK;
		balaradHeader.color[iColor++] = UserColor[color];
		if ((iColor & 0xffffff00) != 0U) {
		  break;
		}
	  }
	}
	balaradHeader.signature       = 'drbm';
	balaradHeader.backgroundColor = BackgroundColor;
	balaradHeader.hoopSizeX       = IniFile.hoopSizeX * balaradRatio;
	balaradHeader.hoopSizeY       = IniFile.hoopSizeY * balaradRatio;
	auto bytesWritten             = DWORD {0};
	WriteFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesWritten, nullptr);
	BalaradOffset.x    = IniFile.hoopSizeX / 2.0F;
	BalaradOffset.y    = IniFile.hoopSizeY / 2.0F;
	auto balaradStitch = std::vector<BALSTCH> {};
	balaradStitch.resize(StitchBuffer->size() + 2U);
	color = StitchBuffer->front().attribute & COLMSK;
	// ToDo - does this loop make sense? iOutput is > 2 after one iteration
	auto iOutput = 0U;
	thr2bal(balaradStitch, iOutput++, 0, BALJUMP);
	balaradStitch[iOutput].flag = gsl::narrow<uint8_t>(color);
	for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size()) && iOutput < 2U; iStitch++) {
	  thr2bal(balaradStitch, iOutput++, iStitch, BALNORM);
	  if ((StitchBuffer->operator[](iStitch).attribute & COLMSK) != color) {
		thr2bal(balaradStitch, iOutput, iStitch, BALSTOP);
		color                         = StitchBuffer->operator[](iStitch).attribute & COLMSK;
		balaradStitch[iOutput++].flag = gsl::narrow<uint8_t>(color);
	  }
	}
	WriteFile(balaradFile, balaradStitch.data(), iOutput * sizeof(decltype(balaradStitch.back())), &bytesWritten, nullptr);
	CloseHandle(balaradFile);
	balaradFile =
	    CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	wrap::WriteFile(balaradFile,
	                outputName.wstring().c_str(),
	                wrap::toUnsigned(outputName.wstring().size()) + 1U,
	                &bytesWritten,
	                nullptr);
	CloseHandle(balaradFile);
  }
  else {
	if (!BalaradName1->empty()) {
	  auto balaradFile =
	      CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	  CloseHandle(balaradFile);
	}
  }
  if (!BalaradName0->empty()) {
	fs::remove(*BalaradName0);
  }
}

void thred::internal::dstcurs() noexcept {
  DestroyCursor(FormCursor);
  DestroyCursor(DLineCursor);
  DestroyCursor(NeedleUpCursor);
  DestroyCursor(NeedleRightDownCursor);
  DestroyCursor(NeedleRightUpCursor);
  DestroyCursor(NeedleLeftDownCursor);
  DestroyCursor(NeedleLeftUpCursor);
}

void thred::internal::reldun() {
  texture::txdun();
  ritbal();
  ritini();
  dstcurs();
  PostQuitMessage(0);
}

auto thred::internal::chkattr(fs::path const& filename) -> bool {
  constexpr auto writeBits = (fs::perms::owner_write | fs::perms::others_write | fs::perms::group_write);

  if (fs::space(filename.parent_path()).available == 0U) {
	StateMap.set(StateFlag::NOTFREE);
	return true;
  }

  StateMap.reset(StateFlag::NOTFREE);
  auto const writePerms = fs::status(filename).permissions() & writeBits;
  if (writePerms == fs::perms::none) {
	auto buttonPressed =
	    MessageBox(ThrEdWindow,
	               fmt::format(StringTable->operator[](STR_OVRLOK), filename.wstring()).c_str(),
	               StringTable->operator[](STR_OVRIT).c_str(),
	               MB_YESNO);
	if (buttonPressed == IDYES) {
	  fs::permissions(filename, writeBits, fs::perm_options::add);
	}
	else {
	  return true;
	}
  }

  return false;
}

void thred::internal::duver(fs::path const& name) {
  auto version = tolower(name.extension().wstring().back()) - 'r';
  if (version >= 0 && version < gsl::narrow<decltype(version)>(VersionNames->size())) {
	VersionNames->operator[](version) = name;
  }
}

void thred::internal::durit(std::vector<char>& destination, const void* source, uint32_t count) {
  if (source != nullptr) {
	auto const src =
	    gsl::span<char const> {gsl::narrow_cast<char const*>(source), gsl::narrow<ptrdiff_t>(count)};
	destination.insert(destination.end(), src.begin(), src.end());
  }
}

void thred::internal::dubuf(std::vector<char>& buffer) {
  auto stitchHeader  = STRHED {};
  auto vertexCount   = 0U;
  auto guideCount    = 0U;
  auto clipDataCount = 0U;

  stitchHeader.headerType = 0x2746872;
  stitchHeader.fileLength = gsl::narrow<decltype(stitchHeader.fileLength)>(
      StitchBuffer->size() * sizeof(decltype(StitchBuffer->back())) + sizeof(stitchHeader) + sizeof(PCSBMPFileName));
  stitchHeader.stitchCount = gsl::narrow<decltype(stitchHeader.stitchCount)>(StitchBuffer->size());
  stitchHeader.hoopType    = IniFile.hoopType;
  auto       designer      = utf::Utf16ToUtf8(*DesignerName);
  auto const modifierName  = gsl::span<char> {ExtendedHeader->modifierName};
  std::copy(designer.cbegin(), designer.cend(), modifierName.begin());
  if (!FormList->empty()) {
	for (auto& form : (*FormList)) {
	  vertexCount += form.vertexCount;
	  if (form.type == SAT) {
		guideCount += form.satinGuideCount;
	  }
	  if (clip::isclp(form)) {
		clipDataCount += form.lengthOrCount.clipCount;
	  }
	  if (clip::iseclp(form)) {
		clipDataCount += form.clipEntries;
	  }
	}
  }
  stitchHeader.formCount      = gsl::narrow<uint16_t>(FormList->size());
  stitchHeader.vertexCount    = gsl::narrow<uint16_t>(vertexCount);
  stitchHeader.dlineCount     = gsl::narrow<uint16_t>(guideCount);
  stitchHeader.clipDataCount  = gsl::narrow<uint16_t>(clipDataCount);
  constexpr auto threadLength = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0])) /
                                2; // ThreadSize is defined as a 16 entry array of 2 bytes
  constexpr auto formDataOffset = sizeof(PCSBMPFileName) + sizeof(BackgroundColor) +
                                  sizeof(UserColor) + sizeof(CustomColor) + threadLength;
  auto       vtxLen        = sizeof(stitchHeader) + StitchBuffer->size() * sizeof(decltype(StitchBuffer->back())) + formDataOffset;
  auto const thredDataSize = FormList->size() * sizeof(decltype(FormList->back())) +
                             vertexCount * sizeof(decltype(FormVertices->back())) +
                             guideCount * sizeof(decltype(SatinGuides->back())) +
                             clipDataCount * sizeof(decltype(ClipPoints->back())) +
                             TexturePointsBuffer->size() * sizeof(decltype(TexturePointsBuffer->back()));
  buffer.reserve(vtxLen + thredDataSize);
  // ToDo - vertexLength overflows if there are more than 5446 stitches, so clamp it until version 3
  if (vtxLen > USHRT_MAX) {
	vtxLen = USHRT_MAX;
  }
  stitchHeader.vertexLen = gsl::narrow<uint16_t>(vtxLen);
  stitchHeader.dlineLen = gsl::narrow<uint16_t>(sizeof(decltype(FormVertices->back())) * vertexCount);
  stitchHeader.clipDataLen = gsl::narrow<uint16_t>(sizeof(decltype(ClipPoints->back())) * clipDataCount);
  durit(buffer, &stitchHeader, sizeof(stitchHeader));
  ExtendedHeader->auxFormat         = IniFile.auxFileType;
  ExtendedHeader->hoopSizeX         = IniFile.hoopSizeX;
  ExtendedHeader->hoopSizeY         = IniFile.hoopSizeY;
  ExtendedHeader->texturePointCount = wrap::toUnsigned(TexturePointsBuffer->size());
  durit(buffer, ExtendedHeader, sizeof(*ExtendedHeader));
  durit(buffer,
        StitchBuffer->data(),
        wrap::toUnsigned(StitchBuffer->size() * sizeof(decltype(StitchBuffer->back()))));
  if (PCSBMPFileName[0] == 0) {
	std::fill(std::begin(PCSBMPFileName), std::end(PCSBMPFileName), '\0');
  }
  durit(buffer, std::begin(PCSBMPFileName), sizeof(PCSBMPFileName));
  durit(buffer, &BackgroundColor, sizeof(BackgroundColor));
  durit(buffer, std::begin(UserColor), sizeof(UserColor));
  durit(buffer, std::begin(CustomColor), sizeof(CustomColor));
  auto threadSizeBuffer = std::string {};
  threadSizeBuffer.resize(threadLength);
  for (auto iThread = 0U; iThread < threadLength; iThread++) {
	threadSizeBuffer[iThread] = ThreadSize[iThread][0];
  }
  durit(buffer, threadSizeBuffer.c_str(), wrap::toUnsigned(threadSizeBuffer.size() * sizeof(threadSizeBuffer[0])));
  if (!FormList->empty()) {
	auto outForms = std::vector<FRMHEDOUT> {};
	outForms.reserve(FormList->size());
	auto vertices = std::vector<fPOINT> {};
	vertices.reserve(vertexCount);
	auto guides = std::vector<SATCONOUT> {};
	guides.reserve(guideCount);
	auto points = std::vector<fPOINT> {};
	points.reserve(clipDataCount);
	for (auto& srcForm : (*FormList)) {
	  outForms.emplace_back(srcForm);
	  auto vertexIt = std::next(FormVertices->cbegin(), srcForm.vertexIndex);
	  for (auto iVertex = 0U; iVertex < srcForm.vertexCount; iVertex++) {
		vertices.push_back(vertexIt[iVertex]);
	  }
	  if (srcForm.type == SAT) {
		outForms.back().satinGuideCount = gsl::narrow<uint16_t>(srcForm.satinGuideCount);
		auto guideIt = std::next(SatinGuides->cbegin(), srcForm.satinOrAngle.guide);
		for (auto iGuide = 0U; iGuide < srcForm.satinGuideCount; iGuide++) {
		  guides.emplace_back(guideIt[iGuide]);
		}
	  }
	  if (clip::isclp(srcForm)) {
		auto offsetStart = std::next(ClipPoints->cbegin(), srcForm.angleOrClipData.clip);
		for (auto iClip = 0U; iClip < srcForm.lengthOrCount.clipCount; iClip++) {
		  points.push_back(*offsetStart);
		  offsetStart++;
		}
	  }
	  if (clip::iseclpx(srcForm)) {
		auto       offsetStart = std::next(ClipPoints->cbegin(), srcForm.borderClipData);
		auto const clipCount   = srcForm.clipEntries;
		for (auto iClip = 0U; iClip < clipCount; iClip++) {
		  points.push_back(*offsetStart);
		  offsetStart++;
		}
	  }
	}
	if (!outForms.empty()) {
	  durit(buffer, outForms.data(), wrap::toUnsigned(outForms.size() * sizeof(decltype(outForms.back()))));
	}
	if (!vertices.empty()) {
	  durit(buffer, vertices.data(), wrap::toUnsigned(vertices.size() * sizeof(decltype(vertices.back()))));
	}
	if (!guides.empty()) {
	  durit(buffer, guides.data(), wrap::toUnsigned(guides.size() * sizeof(decltype(guides.back()))));
	}
	if (!points.empty()) {
	  durit(buffer, points.data(), wrap::toUnsigned(points.size() * sizeof(decltype(points.back()))));
	}
	if (!TexturePointsBuffer->empty()) {
	  durit(buffer,
	        TexturePointsBuffer->data(),
	        gsl::narrow<uint32_t>(TexturePointsBuffer->size() * sizeof(decltype(TexturePointsBuffer->back()))));
	}
  }
}

void thred::internal::thrsav() {
  if (chkattr(*WorkingFileName)) {
	return;
  }
  if (!StateMap.testAndReset(StateFlag::IGNAM)) {
	auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
	auto file     = FindFirstFile(GeName->wstring().c_str(), &fileData);
	if (file != INVALID_HANDLE_VALUE) { // NOLINT
	  StateMap.reset(StateFlag::CMPDO);
	  for (auto& version : *VersionNames) {
		version.clear();
	  }
	  duver(*DefaultDirectory / std::begin(fileData.cFileName));
	  while (FindNextFile(file, &fileData)) {
		duver(*DefaultDirectory / std::begin(fileData.cFileName));
	  }
	  FindClose(file);
	  fs::remove(VersionNames->back());
	  for (auto iBackup = VersionNames->size() - 1U; iBackup > 0; iBackup--) {
		if (!VersionNames->operator[](iBackup - 1U).empty()) {
		  auto newFileName = VersionNames->operator[](iBackup - 1U);

		  auto ext   = newFileName.extension().wstring();
		  ext.back() = gsl::narrow<wchar_t>(iBackup - 1) + 's';
		  newFileName.replace_extension(ext);
		  fs::rename(VersionNames->operator[](iBackup - 1U), newFileName);
		}
	  }
	}
  }
  FileHandle = CreateFile(ThrName->wstring().c_str(), (GENERIC_WRITE), 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (FileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	displayText::crmsg(*ThrName);
	FileHandle = nullptr;
  }
  else {
	auto output = std::vector<char> {};
	dubuf(output);
	auto bytesWritten = DWORD {0};
	WriteFile(FileHandle, output.data(), wrap::toUnsigned(output.size()), &bytesWritten, nullptr);
	if (bytesWritten != output.size()) {
	  auto fmtStr = std::wstring {};
	  displayText::loadString(fmtStr, IDS_FWERR);
	  displayText::shoMsg(fmt::format(fmtStr, ThrName->wstring()));
	}
	CloseHandle(FileHandle);
  }
}

void thred::internal::chk1col() {
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
  for (auto iColorChange = 0U; iColorChange < ColorChanges; iColorChange++) {
	if (ColorChangeTable[iColorChange + 1U].stitchIndex - ColorChangeTable[iColorChange].stitchIndex == 1) {
	  auto const iStitch = ColorChangeTable[iColorChange].stitchIndex;
	  auto       color   = 0U;
	  if (iStitch != 0U) {
		color = StitchBuffer->operator[](iStitch - 1U).attribute & COLMSK;
	  }
	  else {
		color = StitchBuffer->operator[](iStitch).attribute & COLMSK;
	  }
	  StitchBuffer->operator[](iStitch).attribute &= NCOLMSK;
	  StitchBuffer->operator[](iStitch).attribute |= color;
	}
  }
}

void thred::internal::savdst(std::vector<DSTREC>& DSTRecords, uint32_t data) {
  auto dstRecord = DSTREC {gsl::narrow_cast<uint8_t>(data & 0xFFU),
                           gsl::narrow_cast<uint8_t>((data & 0xFF00U) >> 8U),
                           gsl::narrow_cast<uint8_t>((data & 0xFF0000U) >> 16U)};
  DSTRecords.push_back(dstRecord);
}

constexpr auto thred::internal::dudbits(POINT const& dif) -> uint32_t {
  return Xdst[dif.x + 121] | Ydst[dif.y + 121];
}

auto thred::internal::colfil() -> bool {
  *ColorFileName = *WorkingFileName;
  *RGBFileName   = *WorkingFileName;
  auto flag      = false;
  if (WorkingFileName->has_extension()) {
	ColorFileName->replace_extension(L"thw");
	RGBFileName->replace_extension(L"rgb");
	flag = true;
  }
  return flag;
}

void thred::internal::ritdst(DSTOffsets&                    DSTOffsetData,
                             std::vector<DSTREC>&           DSTRecords,
                             std::vector<fPOINTATTR> const& stitches) {
  constexpr auto DSTMax          = 121;
  auto           dstStitchBuffer = std::vector<fPOINTATTR> {};
  dstStitchBuffer.resize(StitchBuffer->size());
  auto colorData = std::vector<uint32_t> {};

  // there could be as many colors as there are stitches
  // but we only need to reserve a reasonable number
  colorData.reserve(32);

  colorData.push_back(COLVER);
  colorData.push_back(BackgroundColor);
  colorData.push_back(UserColor[stitches[0].attribute & COLMSK]);
  auto destination = dstStitchBuffer.begin();
  for (auto& stitch : stitches) {
	*destination++ = fPOINTATTR {stitch.x * 5.0F / 3.0F, stitch.y * 5.0F / 3.0F, stitch.attribute};
  }
  auto boundingRect = fRECTANGLE {
      dstStitchBuffer[0].x, dstStitchBuffer[0].y, dstStitchBuffer[0].x, dstStitchBuffer[0].y};
  for (auto& stitch : dstStitchBuffer) {
	if (stitch.x > boundingRect.right) {
	  boundingRect.right = stitch.x + 0.5F;
	}
	if (stitch.x < boundingRect.left) {
	  boundingRect.left = stitch.x - 0.5F;
	}
	if (stitch.y > boundingRect.top) {
	  boundingRect.top = stitch.y + 0.5F;
	}
	if (stitch.y < boundingRect.bottom) {
	  boundingRect.bottom = stitch.y - 0.5F;
	}
  }
  auto centerCoordinate = POINT {wrap::round<int32_t>(form::midl(boundingRect.right, boundingRect.left)),
                                 wrap::round<int32_t>(form::midl(boundingRect.top, boundingRect.bottom))};
  DSTOffsetData.Positive.x = wrap::round<int32_t>(boundingRect.right - centerCoordinate.x + 1);
  DSTOffsetData.Positive.y = wrap::round<int32_t>(boundingRect.top - centerCoordinate.y + 1);
  DSTOffsetData.Negative.x = wrap::round<int32_t>(centerCoordinate.x - boundingRect.left - 1);
  DSTOffsetData.Negative.y = wrap::round<int32_t>(centerCoordinate.y - boundingRect.bottom - 1);
  auto color               = dstStitchBuffer[0].attribute & 0xfU;
  for (auto& stitch : dstStitchBuffer) {
	if (color != (stitch.attribute & 0xfU)) {
	  savdst(DSTRecords, 0xc30000);
	  color = stitch.attribute & 0xfU;
	  colorData.push_back(UserColor[color]);
	}
	auto lengths = POINT {wrap::round<int32_t>(gsl::narrow_cast<double>(stitch.x) - centerCoordinate.x),
	                      wrap::round<int32_t>(gsl::narrow_cast<double>(stitch.y) - centerCoordinate.y)};
	auto const absoluteLengths = POINT {abs(lengths.x), abs(lengths.y)};
	auto       count           = 0U;
	if (absoluteLengths.x > absoluteLengths.y) {
	  count = absoluteLengths.x / DSTMax + 1;
	}
	else {
	  count = absoluteLengths.y / DSTMax + 1;
	}
	auto const stepSize = POINT {gsl::narrow<int32_t>(absoluteLengths.x / count + 1U),
	                             gsl::narrow<int32_t>(absoluteLengths.y / count + 1U)};

	auto difference = POINT {0L, 0L};
	while ((lengths.x != 0) || (lengths.y != 0)) {
	  auto dstType = REGTYP;
	  if (abs(lengths.x) > stepSize.x) {
		dstType = JMPTYP;
		if (lengths.x > 0) {
		  difference.x = stepSize.x;
		}
		else {
		  difference.x = -stepSize.x;
		}
	  }
	  else {
		difference.x = lengths.x;
	  }
	  if (abs(lengths.y) > stepSize.y) {
		dstType = JMPTYP;
		if (lengths.y > 0) {
		  difference.y = stepSize.y;
		}
		else {
		  difference.y = -stepSize.y;
		}
	  }
	  else {
		difference.y = lengths.y;
	  }
	  savdst(DSTRecords, dudbits(difference) | dstType);
	  centerCoordinate.x += difference.x;
	  centerCoordinate.y += difference.y;
	  lengths.x -= difference.x;
	  lengths.y -= difference.y;
	}
  }
  DSTRecords.push_back(
      {gsl::narrow_cast<uint8_t>(0), gsl::narrow_cast<uint8_t>(0), gsl::narrow_cast<uint8_t>(0xf3)});

  if (colfil()) {
	auto bytesWritten = DWORD {0};
	auto colorFile =
	    CreateFile(ColorFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (colorFile != INVALID_HANDLE_VALUE) { // NOLINT
	  wrap::WriteFile(colorFile,
	                  &colorData[0],
	                  wrap::toUnsigned(colorData.size() * sizeof(decltype(colorData.data()))),
	                  &bytesWritten,
	                  nullptr);
	}
	CloseHandle(colorFile);
	colorFile = CreateFile(RGBFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (colorFile != INVALID_HANDLE_VALUE) { // NOLINT
	  wrap::WriteFile(colorFile,
	                  &colorData[2],
	                  wrap::toUnsigned((colorData.size() - 2U) * sizeof(decltype(colorData.data()))),
	                  &bytesWritten,
	                  nullptr);
	}
	CloseHandle(colorFile);
  }
}

auto thred::internal::pcshup(std::vector<fPOINTATTR>& stitches) -> bool {
  auto boundingRect = fRECTANGLE {stitches[0].y, stitches[0].x, stitches[0].x, stitches[0].y};

  for (auto stitch : stitches) {
	if (stitch.x < boundingRect.left) {
	  boundingRect.left = stitch.x;
	}
	if (stitch.x > boundingRect.right) {
	  boundingRect.right = stitch.x;
	}
	if (stitch.y < boundingRect.bottom) {
	  boundingRect.bottom = stitch.y;
	}
	if (stitch.y > boundingRect.top) {
	  boundingRect.top = stitch.y;
	}
  }
  fPOINT const boundingSize = {boundingRect.right - boundingRect.left,
                               boundingRect.top - boundingRect.bottom};
  if (boundingSize.x > LHUPX || boundingSize.y > LHUPY) {
	displayText::tabmsg(IDS_PFAF2L);
	return true;
  }
  auto hoopSize = fPOINT {};
  if (boundingSize.x > SHUPX || boundingSize.y > SHUPY) {
	PCSHeader.hoopType = LARGHUP;
	hoopSize.x         = LHUPX;
	hoopSize.y         = LHUPY;
  }
  else {
	if (IniFile.hoopSizeX == LHUPX && IniFile.hoopSizeY == LHUPY) {
	  PCSHeader.hoopType = LARGHUP;
	  hoopSize.x         = LHUPX;
	  hoopSize.y         = LHUPY;
	}
	else {
	  PCSHeader.hoopType = SMALHUP;
	  hoopSize.x         = SHUPX;
	  hoopSize.y         = SHUPY;
	}
  }
  auto delta = fPOINT {};
  if (boundingRect.right > hoopSize.x) {
	delta.x = hoopSize.x - boundingRect.right;
  }
  if (boundingRect.top > hoopSize.y) {
	delta.y = hoopSize.y - boundingRect.top;
  }
  if (boundingRect.left < 0) {
	delta.x = -boundingRect.left;
  }
  if (boundingRect.bottom < 0) {
	delta.y = -boundingRect.bottom;
  }
  if ((delta.x != 0.0F) || (delta.y != 0.0F)) {
	for (auto& offsetStitch : stitches) {
	  offsetStitch.x += delta.x;
	  offsetStitch.y += delta.y;
	}
  }
  return false;
}

#if PESACT

auto thred::internal::pesmtch(COLORREF const& referenceColor, uint8_t const& colorIndex) -> uint32_t {
  auto color           = PECCOLOR {GetRValue(referenceColor), GetGValue(referenceColor), GetBValue(referenceColor)}; // NOLINT
  auto translatedColor = PESThread[colorIndex].color;

  auto const meanR = (gsl::narrow_cast<int32_t>(color.r) + gsl::narrow_cast<int32_t>(translatedColor.r)) / 2;
  auto deltaR = gsl::narrow_cast<int32_t>(color.r) - gsl::narrow_cast<int32_t>(translatedColor.r);
  auto deltaG = gsl::narrow_cast<int32_t>(color.g) - gsl::narrow_cast<int32_t>(translatedColor.g);
  auto deltaB = gsl::narrow_cast<int32_t>(color.b) - gsl::narrow_cast<int32_t>(translatedColor.b);
  // From https://www.compuphase.com/cmetric.htm a more perceptually accurate color distance formula
  return wrap::round<uint32_t>(std::sqrt((((512 + meanR) * deltaR * deltaR) / 256) + 4 * deltaG * deltaG +
                                         (((767 - meanR) * deltaB * deltaB) / 256)));
}

void thred::internal::ritpes(std::vector<uint8_t>& buffer, fPOINTATTR const& stitch) {
  constexpr auto factor  = 3.0F / 5.0F;
  auto const     oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESTCH));
  auto*      pesStitch    = convert_ptr<PESTCH*>(&buffer[oldSize]);
  auto const scaledStitch = fPOINT {-stitch.x * factor + PESstitchCenterOffset.x,
                                    stitch.y * factor - PESstitchCenterOffset.y};
  pesStitch->x            = wrap::round<int16_t>(scaledStitch.x);
  pesStitch->y            = wrap::round<int16_t>(scaledStitch.y);
  OutputIndex++;
}

void thred::internal::ritpesCode(std::vector<uint8_t>& buffer) {
  constexpr auto blockEndCode = uint16_t {0x8003U};
  auto const     oldSize      = buffer.size();
  buffer.resize(oldSize + sizeof(uint16_t));
  auto* contCode = convert_ptr<uint16_t*>(&buffer[oldSize]);
  if (contCode != nullptr) {
	*contCode = blockEndCode;
  }
}

void thred::internal::ritpesBlock(std::vector<uint8_t>& buffer, PESSTCHLST newBlock) {
  auto const oldSize = buffer.size();
  buffer.resize(oldSize + sizeof(PESSTCHLST));
  auto* blockHeader = convert_ptr<PESSTCHLST*>(&buffer[oldSize]);
  *blockHeader      = newBlock;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void thred::internal::pecnam(gsl::span<char> label) {
  strncpy(label.data(), "LA:", 3); // NOLINT
  auto const lblSize  = wrap::toUnsigned(label.size() - 3U);
  auto       fileStem = utf::Utf16ToUtf8(AuxName->stem());
  if (fileStem.size() < lblSize) {
	fileStem += std::string(lblSize - fileStem.size(), ' ');
  }
  strncpy(&label[3], fileStem.c_str(), lblSize); // NOLINT
}
#pragma warning(pop)

void thred::internal::pecEncodeint32_t(std::vector<uint8_t>& buffer, int32_t delta) {
  auto outputVal = gsl::narrow_cast<uint32_t>(abs(delta)) & 0x7FFU;
  if (delta < 0) {
	outputVal = delta + 0x1000U & 0x7FFU;
	outputVal |= 0x800U;
  }
  auto upperByte = gsl::narrow_cast<uint8_t>(((outputVal >> 8U) & 0x0FU) | 0x80U);
  buffer.push_back(upperByte);
  auto lowerByte = gsl::narrow_cast<uint8_t>(outputVal & 0xffU);
  buffer.push_back(lowerByte);
}

void thred::internal::rpcrd(std::vector<uint8_t>& buffer, fPOINT& thisStitch, float srcX, float srcY) {
  auto deltaX = wrap::round<int32_t>(srcX * 5.0F / 3.0F);
  auto deltaY = -wrap::round<int32_t>(srcY * 5.0F / 3.0F);
  if (deltaX < 63 && deltaX > -64 && deltaY < 63 && deltaY > -64) {
	auto xVal = gsl::narrow<uint8_t>(deltaX & 0x7FU); // NOLINT
	auto yVal = gsl::narrow<uint8_t>(deltaY & 0x7FU); // NOLINT
	buffer.push_back(xVal);
	buffer.push_back(yVal);
  }
  else {
	pecEncodeint32_t(buffer, deltaX);
	pecEncodeint32_t(buffer, deltaY);
  }
  thisStitch.x += deltaX * 0.6F;
  thisStitch.y += -deltaY * 0.6F;
}

inline void thred::internal::pecEncodeStop(std::vector<uint8_t>& buffer, uint8_t val) {
  buffer.push_back(0xfe);
  buffer.push_back(0xb0);
  buffer.push_back(val);
}

void thred::internal::pecdat(std::vector<uint8_t>& buffer) {
  auto* pecHeader = convert_ptr<PECHDR*>(buffer.data());
  PESdata         = buffer.data();
  PEScolors       = std::begin(pecHeader->pad);
  auto thisStitch = fPOINT {};
  rpcrd(buffer, thisStitch, StitchBuffer->front().x, StitchBuffer->front().y);
  auto iColor  = 1U;
  auto color   = StitchBuffer->front().attribute & COLMSK;
  PEScolors[0] = PESequivColors[color];
  for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size() - 1U); iStitch++) {
	auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	if ((stitchFwd1.attribute & COLMSK) != color) {
	  color = stitchFwd1.attribute & COLMSK;
	  pecEncodeStop(buffer, (iColor % 2U) + 1U);
	  PEScolors[iColor] = PESequivColors[color];
	  iColor++;
	}
	auto const xDelta = stitchFwd1.x - thisStitch.x;
	auto const yDelta = stitchFwd1.y - thisStitch.y;
	rpcrd(buffer, thisStitch, xDelta, yDelta);
  }
  buffer.push_back(0xffU);
  buffer.push_back(0x0U);
}

void thred::internal::writeThumbnail(std::vector<uint8_t>& buffer,
                                     uint8_t const (*image)[ThumbHeight][ThumbWidth]) {
  if (image != nullptr) {
	for (auto i = 0U; i < ThumbHeight; i++) {
	  for (auto j = 0U; j < 6U; j++) {
		auto const offset = j * 8;
		auto       output = uint8_t {0U};
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset] != 0);
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + 1U] != 0U) << 1U;
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + 2U] != 0U) << 2U;
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + 3U] != 0U) << 3U;
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + 4U] != 0U) << 4U;
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + 5U] != 0U) << 5U;
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + 6U] != 0U) << 6U;
		output |= gsl::narrow_cast<uint32_t>((*image)[i][offset + 7U] != 0U) << 7U;
		buffer.push_back(output);
	  }
	}
  }
}

void thred::internal::pecImage(std::vector<uint8_t>& pecBuffer) {
  uint8_t thumbnail[ThumbHeight][ThumbWidth] = {};
  uint8_t const(*p_thumbnail)[ThumbHeight][ThumbWidth] = &thumbnail; // 2D arrays are painful to pass as parameters

  auto const yFactor = 31.0F / IniFile.hoopSizeY;
  auto const xFactor = 40.0F / IniFile.hoopSizeX;
  auto const dest    = gsl::span<uint8_t>(&thumbnail[0][0], sizeof(thumbnail));
  // write the overall thumbnail
  std::copy(&imageWithFrame[0][0], &imageWithFrame[0][0] + sizeof(imageWithFrame), dest.begin());
  for (auto& stitch : *StitchBuffer) {
	auto x          = wrap::floor<uint16_t>((stitch.x) * xFactor) + 4U;
	auto y          = wrap::floor<uint16_t>((stitch.y) * yFactor) + 5U;
	y               = ThumbHeight - y;
	thumbnail[y][x] = 1U;
  }
  thi::writeThumbnail(pecBuffer, p_thumbnail);
  // now write out the individual thread thumbnails
  std::copy(&imageWithFrame[0][0], &imageWithFrame[0][0] + sizeof(imageWithFrame), dest.begin());
  auto stitchColor = (StitchBuffer->front().attribute & COLMSK);
  // ToDo - check that this is still writing out the thumbnails correctly when starting from index 0 instead of 1
  for (auto& stitch : *StitchBuffer) {
	auto x = wrap::round<uint16_t>((stitch.x) * xFactor) + 3U;
	auto y = wrap::round<uint16_t>((stitch.y) * yFactor) + 3U;
	y      = ThumbHeight - y;
	if (stitchColor == (stitch.attribute & COLMSK)) {
	  thumbnail[y][x] = 1;
	}
	else {
	  thi::writeThumbnail(pecBuffer, p_thumbnail);
	  std::copy(&imageWithFrame[0][0], &imageWithFrame[0][0] + sizeof(imageWithFrame), dest.begin());
	  stitchColor     = (stitch.attribute & COLMSK);
	  thumbnail[y][x] = 1;
	}
  }
  thi::writeThumbnail(pecBuffer, p_thumbnail);
}

#endif

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void thred::internal::sav() {
  xt::duauxnam();
  if (chkattr(*AuxName)) {
	return;
  }
  if (StitchBuffer->empty()) {
	return;
  }
  chk1col();
  thred::coltab();
  auto saveStitches = std::vector<fPOINTATTR> {};
  saveStitches.resize(StitchBuffer->size());
  if (UserFlagMap.test(UserFlag::ROTAUX)) {
	auto iDest = saveStitches.begin();
	for (auto stitch : *StitchBuffer) {
	  *iDest++ = fPOINTATTR {stitch.y, stitch.x, stitch.attribute};
	}
  }
  else {
	std::copy(StitchBuffer->begin(), StitchBuffer->end(), saveStitches.begin());
  }
  PCSFileHandle = CreateFile(
      AuxName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT
  if (PCSFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	displayText::crmsg(*AuxName);
	PCSFileHandle = nullptr;
  }
  else {
	auto flag       = true;
	auto DSTRecords = std::vector<DSTREC> {};
	// There are always going to be more records in the DST format because color changes and jumps count as stitches
	DSTRecords.reserve(StitchBuffer->size() + 128U);
	auto        DSTOffset       = DSTOffsets {};
	auto        PCSStitchBuffer = std::vector<PCSTCH> {};
	auto        auxName         = utf::Utf16ToUtf8(*AuxName);
	auto const* desc            = strrchr(auxName.data(), '\\') + 1U;
	switch (IniFile.auxFileType) {
	  case AUXDST: {
		auto dstHeader = DSTHED {};
		ritdst(DSTOffset, DSTRecords, saveStitches);
		// dstHeader fields are fixed width, so use strncpy in its intended way.
		// Use sizeof to ensure no overrun if the format string is wrong length
		strncpy(dstHeader.desched, "LA:", sizeof(dstHeader.desched)); // NOLINT
		std::fill(std::begin(dstHeader.desc), std::end(dstHeader.desc), ' ');
		if (desc != nullptr) {
		  for (auto iHeader = 0U; iHeader < wrap::toUnsigned(sizeof(dstHeader.desc)); iHeader++) {
			if ((desc[iHeader] != 0) && desc[iHeader] != '.') {
			  dstHeader.desc[iHeader] = desc[iHeader];
			}
			else {
			  break;
			}
		  }
		}
		// clang-format off
		dstHeader.desc[16] = 0xd;
		strncpy(dstHeader.recshed,  "ST:",      sizeof(dstHeader.recshed));                                          // NOLINT
		strncpy(dstHeader.recs,    fmt::format("{:7d}\r", DSTRecords.size()).c_str(), sizeof(dstHeader.recs));       // NOLINT
		strncpy(dstHeader.cohed,    "CO:",      sizeof(dstHeader.cohed));                                            // NOLINT
		strncpy(dstHeader.co,       "  0\xd",   sizeof(dstHeader.co));                                               // NOLINT
		strncpy(dstHeader.xplushed, "+X:",      sizeof(dstHeader.xplushed));                                         // NOLINT
		strncpy(dstHeader.xplus,    fmt::format("{:5d}\xd", DSTOffset.Negative.x).c_str(), sizeof(dstHeader.xplus)); // NOLINT
		strncpy(dstHeader.xminhed,  "-X:",      sizeof(dstHeader.xminhed));                                          // NOLINT
		strncpy(dstHeader.xmin,     fmt::format("{:5d}\xd", DSTOffset.Positive.x).c_str(), sizeof(dstHeader.xmin));  // NOLINT
		strncpy(dstHeader.yplushed, "+Y:",      sizeof(dstHeader.yplushed));                                         // NOLINT
		strncpy(dstHeader.yplus,    fmt::format("{:5d}\xd", DSTOffset.Positive.y).c_str(), sizeof(dstHeader.yplus)); // NOLINT
		strncpy(dstHeader.yminhed,  "-Y:",      sizeof(dstHeader.yminhed));                                          // NOLINT
		strncpy(dstHeader.ymin,     fmt::format("{:5d}\xd", DSTOffset.Negative.y).c_str(), sizeof(dstHeader.ymin));  // NOLINT
		strncpy(dstHeader.axhed,    "AX:",      sizeof(dstHeader.axhed));                                            // NOLINT
		strncpy(dstHeader.ax,       "-    0\r", sizeof(dstHeader.ax));                                               // NOLINT
		strncpy(dstHeader.ayhed,    "AY:",      sizeof(dstHeader.ayhed));                                            // NOLINT
		strncpy(dstHeader.ay,       "+    0\r", sizeof(dstHeader.ay));                                               // NOLINT
		strncpy(dstHeader.mxhed,    "MX:",      sizeof(dstHeader.mxhed));                                            // NOLINT
		strncpy(dstHeader.mx,       "+    0\r", sizeof(dstHeader.mx));                                               // NOLINT
		strncpy(dstHeader.myhed,    "MY:",      sizeof(dstHeader.myhed));                                            // NOLINT
		strncpy(dstHeader.my,       "+    0\r", sizeof(dstHeader.my));                                               // NOLINT
		strncpy(dstHeader.pdhed,    "PD",       sizeof(dstHeader.pdhed));                                            // NOLINT
		strncpy(dstHeader.pd,       "******\r", sizeof(dstHeader.pd));                                               // NOLINT
		strncpy(dstHeader.eof,      "\x1a",     sizeof(dstHeader.eof));                                              // NOLINT
		// clang-format on
		std::fill(std::begin(dstHeader.res), std::end(dstHeader.res), ' ');
		auto bytesWritten = DWORD {0};
		WriteFile(PCSFileHandle, &dstHeader, sizeof(dstHeader), &bytesWritten, nullptr);
		wrap::WriteFile(PCSFileHandle,
		                DSTRecords.data(),
		                wrap::toUnsigned(sizeof(decltype(DSTRecords.back())) * DSTRecords.size()),
		                &bytesWritten,
		                nullptr);
		break;
	  }
#if PESACT
	  case AUXPES: {
		auto pesHeader = PESHED {};
		strncpy(pesHeader.led, "#PES0001", sizeof(pesHeader.led)); // NOLINT
		pesHeader.celn = 7U;
		strncpy(pesHeader.ce, "CEmbOne", sizeof(pesHeader.ce)); // NOLINT
		pesHeader.cslen = 7U;
		strncpy(pesHeader.cs, "CSewSeg", sizeof(pesHeader.cs)); // NOLINT
		auto iColor = 0;
		for (auto const color : UserColor) {
		  auto           matchIndex  = 0U;
		  auto           matchMin    = 0xffffffffU;
		  constexpr auto threadCount = sizeof(PESThread) / sizeof(PESThread[0]);
		  for (auto iColorMatch = 1U; iColorMatch < threadCount; iColorMatch++) {
			auto const match = pesmtch(color, iColorMatch);
			if (match < matchMin) {
			  matchIndex = iColorMatch;
			  matchMin   = match;
			}
		  }
		  PESequivColors[iColor++] = gsl::narrow<uint8_t>(matchIndex);
		}
		auto stitchColor  = StitchBuffer->front().attribute & COLMSK;
		auto boundingRect = fRECTANGLE {};
		sizstch(boundingRect, *StitchBuffer);
		PESstitchCenterOffset.x = form::midl(boundingRect.right, boundingRect.left);
		PESstitchCenterOffset.y = form::midl(boundingRect.top, boundingRect.bottom);
		pesHeader.xsiz = wrap::round<uint16_t>((boundingRect.right - boundingRect.left) * (5.0F / 3.0F));
		pesHeader.ysiz = wrap::round<uint16_t>((boundingRect.top - boundingRect.bottom) * (5.0F / 3.0F));
		OutputIndex    = 0;
		auto pesBuffer = std::vector<uint8_t> {};
		// ToDo - make a reasonable guess for the size of data in the PES buffer. err on the side of caution
		auto const pesSize = sizeof(PESSTCHLST) + StitchBuffer->size() * sizeof(PESTCH) + 1000U;
		pesBuffer.reserve(pesSize);
		auto threadList = std::vector<PESCOLORLIST> {};
		auto blockIndex = gsl::narrow_cast<uint16_t>(0U); // Index into the stitch blocks
		threadList.push_back(PESCOLORLIST {blockIndex, PESequivColors[stitchColor]});
		pesBuffer.resize(sizeof(PESSTCHLST));
		// first block is a jump in place
		ritpesBlock(pesBuffer, PESSTCHLST {1, PESequivColors[stitchColor], 2});
		blockIndex++;
		ritpes(pesBuffer, saveStitches[0]);
		ritpes(pesBuffer, saveStitches[0]);
		ritpesCode(pesBuffer);
		// then a normal stitch in place
		ritpesBlock(pesBuffer, PESSTCHLST {0, PESequivColors[stitchColor], 2});
		blockIndex++;
		ritpes(pesBuffer, saveStitches[0]);
		ritpes(pesBuffer, saveStitches[0]);
		ritpesCode(pesBuffer);
		// then a jump to the first location
		ritpesBlock(pesBuffer, PESSTCHLST {1, PESequivColors[stitchColor], 2});
		blockIndex++;
		ritpes(pesBuffer, saveStitches[0]);
		ritpes(pesBuffer, saveStitches[1]);
		ritpesCode(pesBuffer);
		// now stitch out.
		auto pesThreadCount = 0U;
		auto lastIndex      = pesBuffer.size();
		ritpesBlock(pesBuffer, PESSTCHLST {0, PESequivColors[stitchColor], 0});
		blockIndex++;
		OutputIndex = 0;
		for (auto iStitch = 1U; iStitch < wrap::toUnsigned(StitchBuffer->size()); iStitch++) {
		  if (stitchColor == (StitchBuffer->operator[](iStitch).attribute & COLMSK)) {
			// we are in the same color block, so write the stitch
			ritpes(pesBuffer, saveStitches[iStitch]);
		  }
		  else {
			// write stitch
			ritpesCode(pesBuffer);
			// close out the previous block
			auto blockHeader         = convert_ptr<PESSTCHLST*>(&pesBuffer[lastIndex]);
			blockHeader->stitchcount = OutputIndex;
			// save the thread/color information
			pesThreadCount++;
			stitchColor = StitchBuffer->operator[](iStitch).attribute & COLMSK;
			threadList.push_back(PESCOLORLIST {blockIndex, PESequivColors[stitchColor]});
			// then create the jump block
			OutputIndex = 0;
			ritpesBlock(pesBuffer, PESSTCHLST {1, PESequivColors[stitchColor], 2});
			blockIndex++;
			ritpes(pesBuffer, saveStitches[iStitch - 1U]);
			ritpes(pesBuffer, saveStitches[iStitch]);
			ritpesCode(pesBuffer);
			// and finally start the next block
			OutputIndex = 0;
			lastIndex   = pesBuffer.size();
			ritpesBlock(pesBuffer, PESSTCHLST {0, PESequivColors[stitchColor], 0});
			blockIndex++;
			ritpes(pesBuffer, saveStitches[iStitch]);
		  }
		}
		// finalize the last stitch block
		auto blockHeader         = convert_ptr<PESSTCHLST*>(&pesBuffer[lastIndex]);
		blockHeader->stitchcount = OutputIndex;
		// write the color/thread table
		lastIndex = pesBuffer.size();
		pesBuffer.resize(lastIndex + sizeof(uint16_t));
		auto* colorIndex = convert_ptr<uint16_t*>(&pesBuffer[lastIndex]);
		*colorIndex      = pesThreadCount;
		for (auto paletteIndex = 0U; paletteIndex < pesThreadCount; paletteIndex++) {
		  lastIndex = pesBuffer.size();
		  pesBuffer.resize(lastIndex + 2 * sizeof(uint16_t));
		  auto* colorEntry = convert_ptr<uint16_t*>(&pesBuffer[lastIndex]);
		  *colorEntry      = threadList[paletteIndex].blockIndex;
		  colorEntry++;
		  *colorEntry = threadList[paletteIndex].colorIndex;
		}
		pesHeader.off     = wrap::toUnsigned(pesBuffer.size() + sizeof(pesHeader));
		pesHeader.blct    = 1;
		pesHeader.bcnt    = pesThreadCount;
		pesHeader.hpsz    = 0;
		GroupStartStitch  = 0;
		GroupEndStitch    = gsl::narrow<decltype(GroupEndStitch)>(StitchBuffer->size() - 1U);
		auto bytesWritten = DWORD {0};
		WriteFile(PCSFileHandle, convert_ptr<PESHED*>(&pesHeader), sizeof(pesHeader), &bytesWritten, nullptr);
		WriteFile(PCSFileHandle, pesBuffer.data(), wrap::toUnsigned(pesBuffer.size()), &bytesWritten, nullptr);
		pesBuffer.clear();
		pesBuffer.shrink_to_fit();
		auto pecBuffer = std::vector<uint8_t> {};
		// make a reasonable guess for the size of data in the PEC buffer. Assume all stitch coordinates are 2 bytes
		// and pad by 1000 to account for jumps. Also reserve memory for thumbnails
		auto const pecSize = sizeof(PECHDR) + sizeof(PECHDR2) + StitchBuffer->size() * 2 + 1000 +
		                     (wrap::toSize(pesThreadCount) + 1U) * ThumbHeight * (ThumbWidth / 8);
		pecBuffer.reserve(pecSize);
		pecBuffer.resize(sizeof(PECHDR) + sizeof(PECHDR2));
		auto*      pecHeader = convert_ptr<PECHDR*>(pecBuffer.data());
		auto const label     = gsl::span<char>(pecHeader->label);
		pecnam(label);
		auto fstart = std::next(pecBuffer.begin(), sizeof(pecHeader->label));
		auto fend   = std::next(pecBuffer.begin(), sizeof(*pecHeader));
		std::fill(fstart, fend, ' ');
		pecHeader->labnd       = 13; // 13 = carriage return
		pecHeader->colorCount  = gsl::narrow<uint8_t>(pesThreadCount);
		pecHeader->thumbHeight = ThumbHeight;
		pecHeader->thumbWidth  = ThumbWidth / 8U;
		pecdat(pecBuffer);
		auto* pecHeader2            = convert_ptr<PECHDR2*>(&pecBuffer[sizeof(PECHDR)]);
		pecHeader2->unknown1        = 0;
		pecHeader2->thumbnailOffset = wrap::toUnsigned(pecBuffer.size() - sizeof(PECHDR));
		pecHeader2->unknown2        = 0x3100;
		pecHeader2->unknown3        = 0xf0ff;
		pecHeader2->width           = pesHeader.xsiz;
		pecHeader2->height          = pesHeader.ysiz;
		pecHeader2->unknown4        = 0x01e0;
		pecHeader2->unknown5        = 0x01b0;

		auto xInt16_le = wrap::round<uint16_t>(boundingRect.left * (5.0F / 3.0F));
		auto yInt16_le = wrap::round<uint16_t>(boundingRect.bottom * (5.0F / 3.0F));
		xInt16_le |= gsl::narrow_cast<uint16_t>(0x9000);
		yInt16_le |= gsl::narrow<uint16_t>(0x9000U);
		pecHeader2->xMin = ((xInt16_le & 0xff00U) >> 8U) | ((xInt16_le & 0x00ffU) << 8U);
		pecHeader2->yMin = ((yInt16_le & 0xff00U) >> 8U) | ((yInt16_le & 0x00ffU) << 8U);

		thi::pecImage(pecBuffer);
		WriteFile(PCSFileHandle, pecBuffer.data(), wrap::toUnsigned(pecBuffer.size()), &bytesWritten, nullptr);
		break;
	  }
#endif
	  default: {
		PCSHeader.stitchCount = gsl::narrow<decltype(PCSHeader.stitchCount)>(StitchBuffer->size());
		for (auto iColor = 0U; iColor < 16U; iColor++) {
		  PCSHeader.colors[iColor] = UserColor[iColor];
		}
		do {
		  if (pcshup(saveStitches)) {
			flag = false;
			break;
		  }
		  auto bytesWritten = DWORD {0};
		  if (WriteFile(PCSFileHandle, &PCSHeader, 0x46, &bytesWritten, nullptr) == 0) {
			displayText::riter();
			flag = false;
			break;
		  }
		  auto iPCSstitch = 0U;
		  auto savcol     = 0xffU;
		  PCSStitchBuffer.resize(StitchBuffer->size() + ColorChanges + 2U);
		  for (auto& stitch : saveStitches) {
			if ((stitch.attribute & COLMSK) != savcol) {
			  savcol                           = stitch.attribute & COLMSK;
			  PCSStitchBuffer[iPCSstitch].tag  = 3;
			  PCSStitchBuffer[iPCSstitch++].fx = gsl::narrow<uint8_t>(savcol);
			}
			auto integerPart                = 0.0;
			auto fractionalPart             = std::modf(stitch.x, &integerPart);
			PCSStitchBuffer[iPCSstitch].fx  = wrap::floor<uint8_t>(fractionalPart * 256.0);
			PCSStitchBuffer[iPCSstitch].x   = gsl::narrow<int16_t>(integerPart);
			fractionalPart                  = std::modf(stitch.y, &integerPart);
			PCSStitchBuffer[iPCSstitch].fy  = wrap::floor<uint8_t>(fractionalPart * 256.0);
			PCSStitchBuffer[iPCSstitch++].y = gsl::narrow<int16_t>(integerPart);
		  }
		  if (WriteFile(PCSFileHandle,
		                PCSStitchBuffer.data(),
		                iPCSstitch * sizeof(decltype(PCSStitchBuffer.back())),
		                &bytesWritten,
		                nullptr) == 0) {
			displayText::riter();
			flag = false;
			break;
		  }
		  if (UserFlagMap.test(UserFlag::BSAVOF)) {
			char const blank[16] = {};
			if (WriteFile(PCSFileHandle, static_cast<LPCVOID>(blank), sizeof(blank) - 1U, &bytesWritten, nullptr) == 0) {
			  displayText::riter();
			  flag = false;
			  break;
			}
		  }
		  else {
			if (WriteFile(PCSFileHandle, static_cast<LPCVOID>(PCSBMPFileName), sizeof(PCSBMPFileName) - 1U, &bytesWritten, nullptr) ==
			    0) {
			  displayText::riter();
			  flag = false;
			  break;
			}
		  }
		} while (false);
	  }
	}
	if (flag) {
	  defNam(*WorkingFileName);
	  CloseHandle(PCSFileHandle);
	  if (UserFlagMap.test(UserFlag::ROTAUX)) {
		displayText::filnopn(IDS_FILROT, *AuxName);
	  }
	}
  }
}

#pragma warning(pop)

void thred::internal::auxmen() {
  auto filinfo = MENUITEMINFO {sizeof(MENUITEMINFO),
                               MIIM_TYPE,
                               MFT_STRING,
                               0,
                               0,
                               nullptr,
                               nullptr,
                               nullptr,
                               0,
                               nullptr, // dwTypeData
                               13,
                               nullptr};

  auto auxMsg = std::wstring {};

  CheckMenuItem(MainMenu, ID_AUXPCS, MF_UNCHECKED);
#if PESACT
  CheckMenuItem(MainMenu, ID_AUXPES, MF_UNCHECKED);
#else
  EnableMenuItem(MainMenu, ID_AUXPES, MF_DISABLED | MF_GRAYED);
#endif
  CheckMenuItem(MainMenu, ID_AUXDST, MF_UNCHECKED);
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  auxMsg = fmt::format(StringTable->operator[](STR_AUXTXT), L"DST");
	  CheckMenuItem(MainMenu, ID_AUXDST, MF_CHECKED);
	  break;
	}
	case AUXPES:
#if PESACT
	{
	  auxMsg = fmt::format(StringTable->operator[](STR_AUXTXT), L"PES");
	  CheckMenuItem(MainMenu, ID_AUXPES, MF_CHECKED);
	  break;
	}
#else
	{
	  IniFile.auxFileType = AUXPCS;
	}
#endif
	default: {
	  auxMsg = fmt::format(StringTable->operator[](STR_AUXTXT), L"PCS");
	  CheckMenuItem(MainMenu, ID_AUXPCS, MF_CHECKED);
	}
  }
  GSL_SUPPRESS(26492) {
	filinfo.dwTypeData = const_cast<LPTSTR>(auxMsg.c_str()); // NOLINT
	SetMenuItemInfo(FileMenu, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
  }
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::savAs() {
  if (!StitchBuffer->empty() || !FormList->empty() || (PCSBMPFileName[0] != 0)) {
	auto fileName = std::vector<wchar_t> {};
	fileName.resize(_MAX_PATH);
	auto dirBuffer = std::vector<wchar_t> {};
	dirBuffer.resize(_MAX_PATH);

	auto workingFileStr = WorkingFileName->wstring();
	auto dirStr         = DefaultDirectory->wstring();
	std::copy(workingFileStr.cbegin(), workingFileStr.cend(), fileName.begin());
	std::copy(dirStr.cbegin(), dirStr.cend(), dirBuffer.begin());
	// ToDo - find a better way to do this
	OpenFileName.hwndOwner       = ThrEdWindow;
	OpenFileName.hInstance       = ThrEdInstance;
	OpenFileName.lpstrFile       = fileName.data();
	OpenFileName.lpstrInitialDir = dirBuffer.data();
	OpenFileName.nFilterIndex    = 0;
	if (GetSaveFileName(&OpenFileName)) {
	  fileName.resize(wcslen(fileName.data()));
	  WorkingFileName->assign(fileName.cbegin(), fileName.cend());
	  *DefaultDirectory = WorkingFileName->parent_path();
	  switch (OpenFileName.nFilterIndex) {
		case 1: {
		  WorkingFileName->replace_extension(L".thr");
		  break;
		}
		case 2: {
		  WorkingFileName->replace_extension(L".pcs");
		  IniFile.auxFileType = AUXPCS;
		  auxmen();
		  break;
		}
		case 3:
#if PESACT
		{
		  WorkingFileName->replace_extension(L".pes");
		  IniFile.auxFileType = AUXPES;
		  auxmen();
		  break;
		}
		case 4:
#endif
		{
		  WorkingFileName->replace_extension(L".dst");
		  IniFile.auxFileType = AUXDST;
		  auxmen();
		  break;
		}
	  }
	  StateMap.set(StateFlag::SAVAS);
	  nunams();
	  ritini();
	  StateMap.reset(StateFlag::SAVAS);
	  StateMap.reset(StateFlag::CMPDO);
	  thrsav();
	  sav();
	  SetWindowText(ThrEdWindow, ThrName->wstring().c_str());
	}
  }
}

void thred::save() {
  if (WorkingFileName->empty()) {
	thi::savAs();
  }
  else {
	if (WorkingFileName->extension().empty()) {
	  *WorkingFileName /= L".thr";
	}
	thi::thrsav();
	if (!StitchBuffer->empty()) {
	  thi::sav();
	}
  }
}

void thred::internal::dun() {
  texture::savtxt();
  texture::txof();
  texture::rstxt();
  if (StateMap.testAndReset(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	StateMap.reset(StateFlag::WASRT);
  }
  thred::unsid();
  thred::unbsho();
  rstAll();
  //	if(savcmp() || (*BalaradName0 && *BalaradName1 && PCSHeader.stitchCount && !FormIndex))
  if (savcmp() || (!BalaradName0->empty())) {
	reldun();
  }
  else {
	if (StitchWindowClientRect.right != 0) {
	  displayText::savdisc();
	  StateMap.set(StateFlag::SAVEX);
	}
	else {
	  auto fmtStr = std::wstring {};
	  displayText::loadString(fmtStr, IDS_SAVFIL);
	  if (MessageBox(ThrEdWindow,
	                 fmt::format(fmtStr, ThrName->wstring()).c_str(),
	                 StringTable->operator[](STR_CLOS).c_str(),
	                 MB_YESNO) == IDYES) {
		thred::save();
	  }
	  reldun();
	}
  }
}

void thred::internal::dusid(uint32_t entry) noexcept {
  SideWindow[entry] = CreateWindow(L"STATIC", // NOLINT
                                   SideWindowsStrings[entry].c_str(),
                                   SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                   3,
                                   SideWindowLocation * SideWindowSize.y + 3,
                                   SideWindowSize.x + 3,
                                   SideWindowSize.y,
                                   SideMessageWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  SideWindowLocation++;
}

void thred::internal::sidmsg(FRMHED const& form, HWND window, std::wstring* const strings, uint32_t entries) {
  if (strings != nullptr) {
	auto childListRect  = RECT {0L, 0L, 0L, 0L};
	auto parentListRect = RECT {0L, 0L, 0L, 0L};
	auto entryCount     = entries;

	std::fill(ValueWindow->begin(), ValueWindow->end(), nullptr);
	SideWindowSize     = {};
	SideWindowLocation = 0;
	SideWindowsStrings = strings;
	GetWindowRect(window, &childListRect);
	GetWindowRect(FormDataSheet, &parentListRect);
	form::ispcdclp();
	if (StateMap.test(StateFlag::FILTYP)) {
	  for (auto iEntry = 0U; iEntry < EDGETYPS + 1U; iEntry++) {
		if (gsl::narrow<uint32_t>(form.edgeType & NEGUND) == EdgeFillTypes[iEntry]) {
		  entryCount--;
		}
		else {
		  if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT ||
		      EdgeFillTypes[iEntry] == EDGECLIPX) {
			if (StateMap.test(StateFlag::WASPCDCLP)) {
			  formForms::maxtsiz(strings[iEntry], SideWindowSize);
			}
			else {
			  entryCount--;
			}
		  }
		  else {
			formForms::maxtsiz(strings[iEntry], SideWindowSize);
		  }
		}
	  }
	  SideMessageWindow = CreateWindow(L"STATIC", // NOLINT
	                                   nullptr,
	                                   WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                   parentListRect.right - ThredWindowOrigin.x + 3,
	                                   childListRect.top - ThredWindowOrigin.y - 3,
	                                   SideWindowSize.x + 12,
	                                   SideWindowSize.y * entryCount + 12,
	                                   ThrEdWindow,
	                                   nullptr,
	                                   ThrEdInstance,
	                                   nullptr);
	  for (auto iEntry = 0U; iEntry < entries; iEntry++) {
		if (gsl::narrow<uint32_t>(form.edgeType & NEGUND) != EdgeFillTypes[iEntry]) {
		  if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT ||
		      EdgeFillTypes[iEntry] == EDGECLIPX) {
			if (StateMap.test(StateFlag::WASPCDCLP)) {
			  dusid(iEntry);
			}
		  }
		  else {
			dusid(iEntry);
		  }
		}
	  }
	}
	else {
	  if (FormMenuChoice == LLAYR) {
		auto zero = std::wstring {L"0"};
		formForms::maxtsiz(zero, SideWindowSize);
	  }
	  else {
		if (FormMenuChoice == LFTHTYP) {
		  entryCount     = 5U;
		  SideWindowSize = {ButtonWidthX3, ButtonHeight};
		}
		else {
		  for (auto iEntry = 0U; iEntry < entries; iEntry++) {
			if (((1U << FillTypes[iEntry]) & ClipTypeMap) != 0U) {
			  if (StateMap.test(StateFlag::WASPCDCLP)) {
				formForms::maxtsiz(strings[iEntry], SideWindowSize);
			  }
			  else {
				entryCount--;
			  }
			}
			else {
			  if (FillTypes[iEntry] == form.fillType) {
				entryCount--;
			  }
			  else {
				formForms::maxtsiz(strings[iEntry], SideWindowSize);
			  }
			}
		  }
		}
	  }
	  SideMessageWindow = CreateWindow(L"STATIC", // NOLINT
	                                   nullptr,
	                                   WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                   parentListRect.right - ThredWindowOrigin.x + 3,
	                                   childListRect.top - ThredWindowOrigin.y - 3,
	                                   SideWindowSize.x + 12,
	                                   SideWindowSize.y * entryCount + 12,
	                                   ThrEdWindow,
	                                   nullptr,
	                                   ThrEdInstance,
	                                   nullptr);
	  if (FormMenuChoice == LLAYR) {
		for (auto iEntry = 0U; iEntry < entries; iEntry++) {
		  dusid(iEntry);
		}
	  }
	  else {
		if (FormMenuChoice == LFTHTYP) {
		  for (auto iEntry = 0U; iEntry < 6U; iEntry++) {
			if (FeatherFillTypes[iEntry] != form.fillInfo.feather.fillType) {
			  dusid(iEntry);
			}
		  }
		}
		else {
		  for (auto iEntry = 0U; iEntry < entries; iEntry++) {
			if (FillTypes[iEntry] != form.fillType) {
			  if (((1U << FillTypes[iEntry]) & ClipTypeMap) != 0U) {
				if (StateMap.test(StateFlag::WASPCDCLP)) {
				  dusid(iEntry);
				}
			  }
			  else {
				dusid(iEntry);
			  }
			}
		  }
		}
	  }
	}
	StateMap.set(StateFlag::SIDACT);
  }
}

void thred::internal::centr() {
  auto const center = POINT {wrap::round<int32_t>((ZoomRect.right - ZoomRect.left) / 2.0F),
                             wrap::round<int32_t>((ZoomRect.top - ZoomRect.bottom) / 2.0F)};

  SelectedPoint.x = ZoomRect.left + center.x;
  SelectedPoint.y = ZoomRect.bottom + center.y;
}

void thred::internal::stchWnd() {
  stchPars();

  MainStitchWin = nullptr;
  MainStitchWin = CreateWindow(L"STATIC", // NOLINT
                               nullptr,
                               SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
                               ButtonWidthX3,
                               0,
                               StitchWindowSize.x,
                               StitchWindowSize.y,
                               ThrEdWindow,
                               nullptr,
                               ThrEdInstance,
                               nullptr);
  auto hFont    = displayText::getThrEdFont(400);
  SelectObject(GetDC(MainStitchWin), hFont);

  if (MainStitchWin != nullptr) {
	GetWindowRect(MainStitchWin, &StitchWindowAbsRect);

	VerticalScrollBar = CreateWindow(L"SCROLLBAR", // NOLINT
	                                 nullptr,
	                                 SBS_VERT | WS_CHILD | WS_VISIBLE,
	                                 StitchWindowSize.x + ButtonWidthX3,
	                                 0,
	                                 *ScrollSize,
	                                 StitchWindowSize.y,
	                                 ThrEdWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);

	HorizontalScrollBar = CreateWindow(L"SCROLLBAR", // NOLINT
	                                   nullptr,
	                                   SBS_HORZ | WS_CHILD | WS_VISIBLE,
	                                   ButtonWidthX3,
	                                   StitchWindowSize.y,
	                                   StitchWindowSize.x,
	                                   *ScrollSize,
	                                   ThrEdWindow,
	                                   nullptr,
	                                   ThrEdInstance,
	                                   nullptr);
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
  }
}

// check if a click occurred in A vertical set of 16 windows
// and calculate which window had the click
auto thred::internal::chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) noexcept -> bool {
  auto flag       = false;
  auto topRect    = RECT {0L, 0L, 0L, 0L};
  auto bottomRect = RECT {0L, 0L, 0L, 0L};

  GetWindowRect(topWindow, &topRect);
  GetWindowRect(bottomWindow, &bottomRect);

  if (clickCoord.x > topRect.left && clickCoord.x < bottomRect.right &&
      clickCoord.y > topRect.top && clickCoord.y < bottomRect.bottom) {
	VerticalIndex = 15U - (bottomRect.bottom - clickCoord.y) / ButtonHeight;
	if (VerticalIndex > 15U) { // Something has broken so do something reasonable
	  VerticalIndex = 0;
	}
	else { // we have a valid Index
	  flag = true;
	}
  }
  return flag;
}

void thred::internal::delstch1(uint32_t iStitch) {
  if (!StitchBuffer->empty()) {
	StitchBuffer->erase(std::next(StitchBuffer->begin(), iStitch));
	if (ClosestPointIndex > wrap::toUnsigned(StitchBuffer->size()) - 1U) {
	  ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size()) - 1U;
	}
  }
}

void thred::internal::redbak() {
  auto const undoData = convert_ptr<BAKHED*>(UndoBuffer->operator[](UndoBufferWriteIndex).get());
  if (undoData != nullptr) {
	if (undoData->stitchCount != 0U) {
	  StitchBuffer->resize(undoData->stitchCount);
	  std::copy(&undoData->stitches[0], &undoData->stitches[undoData->stitchCount], StitchBuffer->begin());
	}
	else {
	  StitchBuffer->clear();
	  StateMap.reset(StateFlag::INIT);
	}
	UnzoomedRect = undoData->zoomRect;
	if (undoData->formCount != 0U) {
	  FormList->resize(undoData->formCount);
	  std::copy(undoData->forms, undoData->forms + undoData->formCount, FormList->begin());
	}
	else {
	  FormList->clear();
	}
	if (undoData->vertexCount != 0U) {
	  FormVertices->resize(undoData->vertexCount);
	  std::copy(undoData->vertices, undoData->vertices + undoData->vertexCount, FormVertices->begin());
	}
	else {
	  FormVertices->clear();
	}
	if (undoData->guideCount != 0U) {
	  SatinGuides->resize(undoData->guideCount);
	  std::copy(undoData->guide, undoData->guide + undoData->guideCount, SatinGuides->begin());
	}
	else {
	  SatinGuides->clear();
	}
	if (undoData->clipPointCount != 0U) {
	  ClipPoints->resize(undoData->clipPointCount);
	  std::copy(undoData->clipPoints, undoData->clipPoints + undoData->clipPointCount, ClipPoints->begin());
	}
	else {
	  ClipPoints->clear();
	}
	// ToDo - add field in BAKHED to keep track of number of colors
	constexpr auto sizeColors = (sizeof(UserColor) / sizeof(UserColor[0]));
	auto const undoColors = gsl::span<COLORREF> {undoData->colors, gsl::narrow<ptrdiff_t>(sizeColors)};
	auto const userColors = gsl::span<COLORREF> {UserColor};
	std::copy(undoColors.cbegin(), undoColors.cend(), userColors.begin());
	for (auto iColor = 0U; iColor < sizeColors; iColor++) {
	  UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
	  UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	}
	for (auto& iColor : *UserColorWin) {
	  thred::redraw(iColor);
	}
	TexturePointsBuffer->resize(undoData->texturePointCount);
	if (undoData->texturePointCount != 0U) {
	  std::copy(undoData->texturePoints,
	            undoData->texturePoints + undoData->texturePointCount,
	            TexturePointsBuffer->begin());
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
}

void thred::internal::redo() {
  if (StateMap.test(StateFlag::BAKACT) && StateMap.test(StateFlag::REDUSHO)) {
	UndoBufferWriteIndex++;
	UndoBufferWriteIndex &= 0xfU;
	auto const nextBufferIndex = (UndoBufferWriteIndex + 1U) & 0xfU;
	if (nextBufferIndex == UndoBufferReadIndex) {
	  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED); // NOLINT
	  StateMap.reset(StateFlag::REDUSHO);
	}
	else {
	  if (!StateMap.testAndSet(StateFlag::REDUSHO)) {
		EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED); // NOLINT
	  }
	}
	if (!StateMap.testAndSet(StateFlag::UNDUSHO)) {
	  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED); // NOLINT
	}
	redbak();
	StateMap.set(StateFlag::DUMEN);
  }
}

void thred::internal::bak() {
  thred::unmsg();
  StateMap.reset(StateFlag::FPSEL);
  StateMap.reset(StateFlag::FRMPSEL);
  StateMap.reset(StateFlag::BIGBOX);
  SelectedFormList->clear();
  thred::undat();
  if (StateMap.testAndReset(StateFlag::PRFACT)) {
	StateMap.reset(StateFlag::WASRT);
	DestroyWindow(PreferencesWindow);
	PreferenceIndex = 0;
	thred::unsid();
  }
  if (!StateMap.testAndSet(StateFlag::BAKING)) {
	dudat();
	UndoBufferReadIndex = UndoBufferWriteIndex + 1U;
  }
  if (StateMap.test(StateFlag::BAKWRAP)) {
	UndoBufferWriteIndex--;
	UndoBufferWriteIndex &= 0xfU;
	auto const previousBufferIndex = UndoBufferWriteIndex - 1U;
	if (previousBufferIndex == UndoBufferReadIndex) {
	  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED); // NOLINT
	  StateMap.set(StateFlag::DUMEN);
	  StateMap.reset(StateFlag::UNDUSHO);
	}
  }
  else {
	if (UndoBufferWriteIndex != 0U) {
	  UndoBufferWriteIndex--;
	}
	if (UndoBufferWriteIndex == 0U) {
	  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED); // NOLINT
	  StateMap.set(StateFlag::DUMEN);
	  StateMap.reset(StateFlag::UNDUSHO);
	}
  }
  if (!StateMap.testAndSet(StateFlag::REDUSHO)) {
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED); // NOLINT
	StateMap.set(StateFlag::DUMEN);
  }
  StateMap.reset(StateFlag::FORMSEL);
  StateMap.reset(StateFlag::GRPSEL);
  redbak();
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void thred::internal::bitsiz() {
  auto const screenAspectRatio =
      gsl::narrow<float>(UnzoomedRect.x) / gsl::narrow<float>(UnzoomedRect.y);
  auto const bitmapAspectRatio = gsl::narrow<float>(BitmapWidth) / gsl::narrow<float>(BitmapHeight);

  if (bitmapAspectRatio > screenAspectRatio) {
	BitmapSizeinStitches.x = gsl::narrow<float>(UnzoomedRect.x);
	BitmapSizeinStitches.y = gsl::narrow<float>(UnzoomedRect.x) / bitmapAspectRatio;
  }
  else {
	BitmapSizeinStitches.x = gsl::narrow<float>(UnzoomedRect.y) * bitmapAspectRatio;
	BitmapSizeinStitches.y = gsl::narrow<float>(UnzoomedRect.y);
  }
  BmpStitchRatio.x = BitmapWidth / BitmapSizeinStitches.x;
  BmpStitchRatio.y = BitmapHeight / BitmapSizeinStitches.y;
  StitchBmpRatio.x = BitmapSizeinStitches.x / BitmapWidth;
  StitchBmpRatio.y = BitmapSizeinStitches.y / BitmapHeight;
}

// Get a rough estimate of whether black or white
// is dominant in the monochrome bitmap
auto thred::internal::binv(std::vector<uint8_t> const& monoBitmapData, uint32_t bitmapWidthInBytes) -> bool {
  auto       whiteBits = 0U;
  auto       blackBits = 0U;
  auto const byteCount = BitmapWidth >> 3U;

  for (auto iHeight = 0U; iHeight < BitmapHeight; iHeight++) {
	GSL_SUPPRESS(26429) {
	  if ((wrap::toSize(bitmapWidthInBytes) * iHeight) < monoBitmapData.size()) {
		auto const bcpnt = &monoBitmapData[wrap::toSize(bitmapWidthInBytes) * iHeight];
		for (auto iBytes = 0U; iBytes < byteCount; iBytes++) {
		  if (bcpnt[iBytes] == 0U) {
			blackBits++;
		  }
		  else {
			if (bcpnt[iBytes] == 0xff) {
			  whiteBits++;
			}
		  }
		}
	  }
	  else {
		throw;
	  }
	}
  }
  return whiteBits > blackBits;
}

void thred::internal::bitlin(uint8_t const* source, uint32_t* destination, COLORREF foreground, COLORREF background) {
  if ((source != nullptr) && (destination != nullptr)) {
	for (auto i = 0U; i < (BitmapWidth >> 3U); i++) {
	  auto bits = std::bitset<8U>(source[i]);
	  for (auto bitOffset = 0U; bitOffset < 8U; bitOffset++) {
		*destination = bits[bitOffset ^ 7U] ? foreground : background;
		destination++;
	  }
	}
	if (auto const final = (BitmapWidth % 8)) {
	  auto bits = std::bitset<8U>(source[(BitmapWidth >> 3U)]);
	  for (auto bitOffset = final; bitOffset < 8U; bitOffset++) {
		*destination = bits[bitOffset ^ 7U] ? foreground : background;
		destination++;
	  }
	}
  }
}

auto thred::internal::fswap(COLORREF color) noexcept -> COLORREF {
  // ToDo - find a better option than _byteswap
  return _byteswap_ulong(color) >> 8U;
}

constexpr auto thred::internal::gudtyp(WORD bitCount) noexcept -> bool {
  auto flag = false;
  switch (bitCount) { // NOLINT
	case 1U:
	case 24U:
	case 32U:
	  flag = true;
  }
  return flag;
}

// Move unpacked 24BPP data into packed 24BPP data
void thred::internal::movmap(uint32_t cnt, uint8_t* buffer) {
  auto source = TraceBitmapData;
  if (source != nullptr) {
	auto destination = buffer;

	for (auto i = 0U; i < cnt; i++) {
	  *(convert_ptr<uint32_t*>(destination)) = *(source++);
	  destination += 3;
	}
  }
}

void thred::internal::savmap() {
  if (PCSBMPFileName[0] != 0) {
	if (StateMap.test(StateFlag::MONOMAP)) {
	  displayText::tabmsg(IDS_SAVMAP);
	  return;
	}
	if (!StateMap.test(StateFlag::WASTRAC)) {
	  displayText::tabmsg(IDS_MAPCHG);
	  return;
	}
	if (GetSaveFileName(&OpenBitmapName)) {
	  BitmapFileHandle =
	      CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	  if (IniFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
		displayText::crmsg(*UserBMPFileName);
		return;
	  }
	  auto bytesWritten = DWORD {0};
	  WriteFile(BitmapFileHandle, &BitmapFileHeader, 14U, &bytesWritten, nullptr);
	  WriteFile(BitmapFileHandle, &BitmapFileHeaderV4, BitmapFileHeader.bfOffBits - 14U, &bytesWritten, nullptr);
	  auto buffer = std::vector<uint8_t> {};
	  buffer.resize((wrap::toSize(BitmapWidth) * BitmapHeight * 3U) + 1U);
	  movmap(BitmapWidth * BitmapHeight, buffer.data());
	  WriteFile(BitmapFileHandle, buffer.data(), BitmapWidth * BitmapHeight * 3, &bytesWritten, nullptr);
	  CloseHandle(BitmapFileHandle);
	}
  }
  else {
	displayText::tabmsg(IDS_SHOMAP);
  }
}

auto thred::getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP {
  GSL_SUPPRESS(26490) {
	if (ppvBits != nullptr) {
	  auto bitmap =
	      CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, reinterpret_cast<void**>(ppvBits), nullptr, 0); // NOLINT
	  if (*ppvBits != nullptr) {
		return bitmap;
	  }

	  DeleteObject(bitmap);
	  throw;
	}

	throw;
  }
}

GSL_SUPPRESS(26490) void thred::internal::bfil() {
  auto const InverseBackgroundColor = fswap(BackgroundColor);
  BitmapFileHandle =
      CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (BitmapFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	auto fmtStr = std::wstring {};
	displayText::loadString(fmtStr, IDS_UNOPEN);
	displayText::shoMsg(fmt::format(fmtStr, UserBMPFileName->wstring()));
	CloseHandle(BitmapFileHandle);
	BitmapFileHandle  = nullptr;
	PCSBMPFileName[0] = 0;
	return;
  }
  ReadFile(BitmapFileHandle, &BitmapFileHeader, 14U, &BytesRead, nullptr);
  constexpr auto MB_Sig = 0x4D42; // check for 'BM' signature in the 1st 2 bytes. Use Big Endian order
  if (BitmapFileHeader.bfType == MB_Sig) {
	auto fileHeaderSize = BitmapFileHeader.bfOffBits - 14U;
	if (fileHeaderSize > sizeof(BITMAPV4HEADER)) {
	  fileHeaderSize = sizeof(BITMAPV4HEADER);
	}
	ReadFile(BitmapFileHandle, &BitmapFileHeaderV4, fileHeaderSize, &BytesRead, nullptr);
  }
  else {
	CloseHandle(BitmapFileHandle);
	BitmapFileHandle  = nullptr;
	PCSBMPFileName[0] = 0;
	return;
  }
  if (gudtyp(BitmapFileHeaderV4.bV4BitCount)) {
	if (!StateMap.testAndReset(StateFlag::WASESC)) {
	  StateMap.reset(StateFlag::TRSET);
	}
	BitmapWidth  = BitmapFileHeaderV4.bV4Width;
	BitmapHeight = BitmapFileHeaderV4.bV4Height;
	StateMap.set(StateFlag::INIT);
	ZoomRect.left = ZoomRect.bottom = 0.0F;
	ZoomRect.right                  = UnzoomedRect.x;
	ZoomRect.top                    = UnzoomedRect.y;
	BitmapDC                        = CreateCompatibleDC(StitchWindowDC);
	if (BitmapFileHeaderV4.bV4BitCount == 1) {
	  StateMap.set(StateFlag::MONOMAP);
	  auto       bitmapWidthBytes = (BitmapWidth >> 5U) << 2U;
	  auto const widthOverflow    = BitmapWidth % 32;
	  if (widthOverflow != 0U) {
		bitmapWidthBytes += 4;
	  }
	  auto const bitmapSizeBytes = bitmapWidthBytes * BitmapHeight;
	  auto       monoBitmapData  = std::vector<uint8_t> {};
	  monoBitmapData.resize(bitmapSizeBytes);
	  ReadFile(BitmapFileHandle, monoBitmapData.data(), bitmapSizeBytes, &BytesRead, nullptr);
	  CloseHandle(BitmapFileHandle);
	  auto const flag       = binv(monoBitmapData, bitmapWidthBytes);
	  auto const foreground = gsl::narrow_cast<COLORREF>(flag ? InverseBackgroundColor : BitmapColor);
	  auto const background = gsl::narrow_cast<COLORREF>(flag ? BitmapColor : InverseBackgroundColor);

	  BitmapInfoHeader               = {};
	  BitmapInfoHeader.biSize        = sizeof(BitmapInfoHeader);
	  BitmapInfoHeader.biWidth       = BitmapWidth;
	  BitmapInfoHeader.biHeight      = BitmapHeight;
	  BitmapInfoHeader.biPlanes      = 1U;
	  BitmapInfoHeader.biBitCount    = 32U;
	  BitmapInfoHeader.biCompression = BI_RGB;
	  BitmapInfo.bmiHeader           = BitmapInfoHeader;
	  HBITMAP bitmap                 = {};

	  GSL_SUPPRESS(26413) {
		auto* bits = gsl::narrow_cast<uint32_t*>(nullptr);
		bitmap     = thred::getBitmap(BitmapDC, &BitmapInfo, &bits);
		// Synchronize
		GdiFlush();
		if (bits != nullptr) {
		  for (auto iHeight = 0U; iHeight < BitmapHeight; iHeight++) {
			bitlin(&monoBitmapData[wrap::toSize(iHeight) * bitmapWidthBytes],
			       &bits[wrap::toSize(iHeight) * BitmapWidth],
			       background,
			       foreground);
		  }
		}
	  };
	  auto deviceContext = CreateCompatibleDC(StitchWindowDC);
	  if ((bitmap != nullptr) && (deviceContext != nullptr)) {
		SelectObject(deviceContext, bitmap);
		BitmapFileHandle = CreateCompatibleBitmap(StitchWindowDC, BitmapWidth, BitmapHeight);
		SelectObject(BitmapDC, BitmapFileHandle);
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, deviceContext, 0, 0, SRCCOPY);
		DeleteObject(bitmap);
		DeleteObject(deviceContext);
	  }
	}
	else {
	  CloseHandle(BitmapFileHandle);
	  StateMap.reset(StateFlag::MONOMAP);
	  BitmapFileHandle = LoadImage(
	      ThrEdInstance, UserBMPFileName->wstring().c_str(), IMAGE_BITMAP, BitmapWidth, BitmapHeight, LR_LOADFROMFILE);
	  SelectObject(BitmapDC, BitmapFileHandle);
	  StateMap.set(StateFlag::RESTCH);
	}
	bitsiz();
	trace::initColorRef();
	StateMap.reset(StateFlag::HIDMAP);
  }
  else {
	CloseHandle(BitmapFileHandle);
	BitmapFileHandle  = nullptr;
	PCSBMPFileName[0] = 0;
	displayText::tabmsg(IDS_BMAP);
  }
}

void thred::internal::prtred() {
  CloseHandle(FileHandle);
  StateMap.reset(StateFlag::INIT);
  FormList->clear();
  displayText::tabmsg(IDS_PRT);
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
}

auto thred::internal::dtrn(DSTREC* dpnt) -> uint32_t {
  return *(convert_ptr<uint32_t*>(dpnt));
}

void thred::internal::dstran(std::vector<DSTREC>& DSTData) {
  auto bytesRead = DWORD {0};
  auto colors    = std::vector<uint32_t> {};

  if (colfil()) {
	auto colorFile =
	    CreateFile(ColorFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (colorFile != INVALID_HANDLE_VALUE) { // NOLINT
	  auto colorFileSize = LARGE_INTEGER {};
	  GetFileSizeEx(colorFile, &colorFileSize);
	  // There can only be (64K + 3) colors, so even if HighPart is non-zero, we don't care
	  colors.resize(colorFileSize.u.LowPart / sizeof(decltype(colors.back())));
	  ReadFile(colorFile, colors.data(), colorFileSize.u.LowPart, &bytesRead, nullptr);
	  CloseHandle(colorFile);
	  if (bytesRead > (sizeof(decltype(colors.back())) * 2)) {
		if (colors[0] == COLVER) {
		  BackgroundColor = colors[1];
		  ColorChanges    = 0;
		}
	  }
	}
  }

  auto iColor = 2U;
  auto color  = 0U;
  if (bytesRead >= ((wrap::toSize(iColor) + 1U) * sizeof(decltype(colors.back())))) {
	color = colmatch(colors[iColor++]);
  }
  else {
	color = 0;
  }
  auto localStitch       = fPOINT {};
  auto maximumCoordinate = fPOINT {-1e12F, -1e12F};
  auto mimimumCoordinate = fPOINT {1e12F, 1e12F};
  for (auto& record : DSTData) {
	if ((record.nd & 0x40U) != 0) {
	  if (bytesRead >= ((wrap::toSize(iColor) + 1U) * sizeof(decltype(colors.back())))) {
		color = colmatch(colors[iColor++]);
	  }
	  else {
		color++;
		color &= 0xfU;
	  }
	}
	else {
	  auto dstStitch = POINT {0L, 0L};
	  dstin(thi::dtrn(&record), dstStitch);
	  localStitch.x += dstStitch.x;
	  localStitch.y += dstStitch.y;
	  if ((record.nd & 0x80U) == 0U) {
		StitchBuffer->push_back(fPOINTATTR {localStitch.x * 0.6F, localStitch.y * 0.6F, color | NOTFRM});
		auto& stitch = StitchBuffer->back();
		if (stitch.x > maximumCoordinate.x) {
		  maximumCoordinate.x = stitch.x;
		}
		if (stitch.y > maximumCoordinate.y) {
		  maximumCoordinate.y = stitch.y;
		}
		if (stitch.x < mimimumCoordinate.x) {
		  mimimumCoordinate.x = stitch.x;
		}
		if (stitch.y < mimimumCoordinate.y) {
		  mimimumCoordinate.y = stitch.y;
		}
	  }
	}
  }
  auto const dstSize =
      fPOINT {maximumCoordinate.x - mimimumCoordinate.x, maximumCoordinate.y - mimimumCoordinate.y};
  IniFile.hoopType = CUSTHUP;
  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
  if (dstSize.x > UnzoomedRect.x || dstSize.y > UnzoomedRect.y) {
	IniFile.hoopSizeX = dstSize.x * 1.1F;
	IniFile.hoopSizeY = dstSize.y * 1.1F;
	UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
	displayText::hsizmsg();
  }
  auto const delta =
      fPOINT {(gsl::narrow_cast<float>(UnzoomedRect.x) - dstSize.x) / 2.0F - mimimumCoordinate.x,
              (gsl::narrow_cast<float>(UnzoomedRect.y) - dstSize.y) / 2.0F - mimimumCoordinate.y};
  for (auto& iStitch : *StitchBuffer) {
	iStitch.x += delta.x;
	iStitch.y += delta.y;
  }
}

auto thred::internal::chkdst(DSTHED const* dstHeader) noexcept -> bool {
  return strncmp(dstHeader->desched, "LA:", 3) == 0; // NOLINT
}

#if PESACT

auto thred::internal::dupcol(uint32_t activeColor) -> uint32_t {
  constexpr auto threadSize  = sizeof(PESThread) / sizeof(PESThread[0]);
  auto const     threadColor = PESThread[PEScolors[PEScolorIndex++] % threadSize];
  auto const color = RGB(threadColor.color.r, threadColor.color.g, threadColor.color.b); // NOLINT
  for (auto iColor = 0U; iColor < activeColor; iColor++) {
	if (UserColor[iColor] == color) {
	  return iColor;
	}
  }
  auto minimumDistance = 0xffffffU;
  auto matchIndex      = 0U;
  for (auto iColor = 1U; iColor < activeColor; iColor++) {
	auto const matchDistance = pesmtch(color, PEScolors[iColor]);
	if (matchDistance < minimumDistance) {
	  minimumDistance = matchDistance;
	  matchIndex      = iColor;
	}
  }
  return matchIndex;
}

#endif

void thred::internal::unthum() {
  if (StateMap.testAndReset(StateFlag::THUMSHO)) {
	for (auto& iBackup : BackupViewer) {
	  DestroyWindow(iBackup);
	}
	if (StateMap.test(StateFlag::UPTO)) {
	  displayText::butxt(HUPTO, StringTable->operator[](STR_UPON));
	}
	else {
	  displayText::butxt(HUPTO, StringTable->operator[](STR_UPOF));
	}
	auto const blank = std::wstring {};
	displayText::butxt(HNUM, blank);
	thred::redraw(ButtonWin->operator[](HHID));
	displayText::butxt(HBOXSEL, StringTable->operator[](STR_BOXSEL));
  }
}

void thred::internal::rstdu() {
  deldu();
  StateMap.reset(StateFlag::REDUSHO);
  StateMap.reset(StateFlag::UNDUSHO);
  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED); // NOLINT
  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED); // NOLINT
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::nuFil() {
  auto fileName = std::vector<wchar_t> {};
  fileName.resize(_MAX_PATH);
  auto dirBuffer = std::vector<wchar_t> {};
  dirBuffer.resize(_MAX_PATH);
  auto workingFileStr = WorkingFileName->wstring();
  auto dirStr         = DefaultDirectory->wstring();
  std::copy(workingFileStr.cbegin(), workingFileStr.cend(), fileName.begin());
  std::copy(dirStr.cbegin(), dirStr.cend(), dirBuffer.begin());
  OpenFileName.hwndOwner       = ThrEdWindow;
  OpenFileName.hInstance       = ThrEdInstance;
  OpenFileName.lpstrFile       = fileName.data();
  OpenFileName.lpstrInitialDir = dirBuffer.data();
  if (StateMap.testAndReset(StateFlag::REDOLD) || GetOpenFileName(&OpenFileName)) {
	fileName.resize(wcslen(fileName.data()));
	WorkingFileName->assign(fileName.cbegin(), fileName.cend());
	fnamtabs();
	trace::untrace();
	if (!FormList->empty()) {
	  form::delfrms();
	}
	StateMap.reset(StateFlag::ZUMED);
	StateMap.reset(StateFlag::FRMOF);
	StateMap.reset(StateFlag::HID);
	StateMap.reset(StateFlag::BIGBOX);
	unthum();
	thred::unbsho();
	if (PCSBMPFileName[0] != 0) {
	  DeleteObject(BitmapFileHandle);
	  ReleaseDC(ThrEdWindow, BitmapDC);
	  PCSBMPFileName[0] = 0;
	}
	// ToDo - use ifstream?
	// ifstream file(WorkingFileName, ios::in | ios::binary | ios::ate);
	FileHandle =
	    CreateFile(WorkingFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (FileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	  if (GetLastError() == 32U) {
		displayText::filnopn(IDS_FNOPNA, *WorkingFileName);
	  }
	  else {
		displayText::filnopn(IDS_FNOPN, *WorkingFileName);
	  }
	  FileHandle = nullptr;
	}
	else {
	  StateMap.reset(StateFlag::CMPDO);
	  StateMap.reset(StateFlag::SAVACT);
	  StateMap.reset(StateFlag::BAKING);
	  StateMap.reset(StateFlag::REDUSHO);
	  TexturePointsBuffer->clear();
	  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED); // NOLINT
	  deldu();
	  DesignerName->assign(utf::Utf8ToUtf16(std::string(std::begin(IniFile.designerName))));
	  thred::unbsho();
	  StateMap.reset(StateFlag::MOVSET);
	  form::frmon();
	  SelectedFormList->clear();
	  SelectedFormList->shrink_to_fit();
	  if (StateMap.testAndReset(StateFlag::PRFACT)) {
		DestroyWindow(PreferencesWindow);
		StateMap.reset(StateFlag::WASRT);
		PreferenceIndex = 0;
	  }
	  PCSBMPFileName[0] = 0;
	  StateMap.reset(StateFlag::SCROS);
	  StateMap.reset(StateFlag::ECROS);
	  SearchLine->clear();
	  SearchLine->shrink_to_fit();
	  StateMap.reset(StateFlag::BZUMIN);
	  rstdu();
	  thred::unmsg();
	  UndoBufferWriteIndex = 0;
	  StateMap.reset(StateFlag::BAKWRAP);
	  ZoomFactor = 1;
	  StateMap.set(StateFlag::RESTCH);
	  defNam(*WorkingFileName);
	  NearestCount = 0;
	  if (StateMap.testAndReset(StateFlag::WASPAT)) {
		DestroyWindow(SpeedScrollBar);
	  }
	  auto textureHistoryFlag = false;
	  if (StateMap.test(StateFlag::WASTXBAK)) {
		textureHistoryFlag = true;
	  }
	  StateMap.reset();
	  if (textureHistoryFlag) {
		StateMap.set(StateFlag::WASTXBAK);
	  }
	  auto       fileSizeHigh   = DWORD {};
	  auto       fileSize       = GetFileSize(FileHandle, &fileSizeHigh);
	  auto       fileExt        = WorkingFileName->extension().wstring();
	  auto const firstCharacter = tolower(fileExt[1]);
	  if (firstCharacter == 't') {
		auto thredHeader = STRHED {};
		ReadFile(FileHandle, &thredHeader, sizeof(thredHeader), &BytesRead, nullptr);
		if ((thredHeader.headerType & 0xffffffU) == 0x746872U) {
		  if (BytesRead != sizeof(thredHeader)) {
			displayText::tabmsg(IDS_SHRTF);
			return;
		  }
		  auto const version = (thredHeader.headerType & 0xff000000) >> 24U;
		  DesignerName->assign(utf::Utf8ToUtf16(std::string(std::begin(IniFile.designerName))));
		  switch (version) {
			case 0: {
			  if (PCSHeader.hoopType == SMALHUP) {
				IniFile.hoopSizeX = SHUPX;
				IniFile.hoopSizeY = SHUPY;
				UnzoomedRect = {gsl::narrow_cast<int32_t>(SHUPX), gsl::narrow_cast<int32_t>(SHUPY)};
			  }
			  else {
				IniFile.hoopSizeX = LHUPX;
				IniFile.hoopSizeY = LHUPY;
				UnzoomedRect = {gsl::narrow_cast<int32_t>(LHUPX), gsl::narrow_cast<int32_t>(LHUPY)};
				PCSHeader.hoopType = LARGHUP;
			  }
			  ritfnam(*DesignerName);
			  auto const modifierName = gsl::span<char> {ExtendedHeader->modifierName};
			  std::copy(&IniFile.designerName[0],
			            &IniFile.designerName[strlen(std::begin(IniFile.designerName))],
			            modifierName.begin());
			  break;
			}
			case 1:
			case 2: {
			  ReadFile(FileHandle, ExtendedHeader, sizeof(*ExtendedHeader), &BytesRead, nullptr);
			  if (BytesRead != sizeof(*ExtendedHeader)) {
				displayText::tabmsg(IDS_SHRTF);
				return;
			  }
			  IniFile.hoopSizeX = ExtendedHeader->hoopSizeX;
			  IniFile.hoopSizeY = ExtendedHeader->hoopSizeY;

			  UnzoomedRect = {wrap::round<int32_t>(ExtendedHeader->hoopSizeX),
			                  wrap::round<int32_t>(ExtendedHeader->hoopSizeY)};
			  redfnam(*DesignerName);
			  break;
			}
			default: {
			  displayText::tabmsg(IDS_NOTVER);
			  return;
			}
		  }
		  ZoomRect = fRECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
		  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
		  if (thredHeader.stitchCount != 0U) {
			StitchBuffer->resize(thredHeader.stitchCount);
			auto bytesToRead =
			    gsl::narrow<DWORD>(thredHeader.stitchCount * sizeof(decltype(StitchBuffer->back())));
			ReadFile(FileHandle, StitchBuffer->data(), bytesToRead, &BytesRead, nullptr);
			if (BytesRead != bytesToRead) {
			  // StitchBuffer->resize(BytesRead / sizeof(decltype(StitchBuffer->back())));
			  // StateMap.set(StateFlag::BADFIL);
			  prtred();
			  return;
			}
		  }
		  StitchBuffer->shrink_to_fit();
		  ReadFile(FileHandle, static_cast<LPVOID>(PCSBMPFileName), sizeof(PCSBMPFileName), &BytesRead, nullptr);
		  if (BytesRead != sizeof(PCSBMPFileName)) {
			PCSBMPFileName[0] = 0;
			prtred();
			return;
		  }
		  ReadFile(FileHandle, &BackgroundColor, sizeof(BackgroundColor), &BytesRead, nullptr);
		  if (BytesRead != sizeof(BackgroundColor)) {
			BackgroundColor = IniFile.backgroundColor;
			prtred();
			return;
		  }
		  BackgroundBrush = CreateSolidBrush(BackgroundColor);
		  ReadFile(FileHandle, static_cast<LPVOID>(UserColor), sizeof(UserColor), &BytesRead, nullptr);
		  if (BytesRead != sizeof(UserColor)) {
			prtred();
			return;
		  }
		  ReadFile(FileHandle, static_cast<LPVOID>(CustomColor), sizeof(CustomColor), &BytesRead, nullptr);
		  if (BytesRead != sizeof(CustomColor)) {
			prtred();
			return;
		  }
		  constexpr auto threadLength = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0])) /
		                                2; // ThreadSize is defined as a 16 entry array of 2 characters
		  char msgBuffer[threadLength];
		  ReadFile(FileHandle, static_cast<LPVOID>(msgBuffer), threadLength, &BytesRead, nullptr);
		  if (BytesRead != threadLength) {
			prtred();
			return;
		  }
		  auto threadSizebuf  = std::string(std::begin(msgBuffer), sizeof(msgBuffer));
		  auto threadSizeBufW = utf::Utf8ToUtf16(threadSizebuf);
		  for (auto iThread = 0U; iThread < threadLength; iThread++) {
			ThreadSize[iThread][0] = threadSizeBufW[iThread];
		  }
		  if (thredHeader.formCount != 0) {
			StateMap.reset(StateFlag::BADFIL);
			MsgBuffer[0]     = 0;
			auto bytesToRead = DWORD {0};
			if (version < 2) {
			  auto formListOriginal = std::vector<FRMHEDO> {};
			  formListOriginal.resize(thredHeader.formCount);
			  bytesToRead =
			      gsl::narrow<DWORD>(thredHeader.formCount * sizeof(decltype(formListOriginal.back())));
			  wrap::ReadFile(FileHandle, formListOriginal.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != thredHeader.formCount * sizeof(decltype(formListOriginal.back()))) {
				thredHeader.formCount = gsl::narrow<decltype(thredHeader.formCount)>(
				    BytesRead / sizeof(decltype(formListOriginal.back())));
				formListOriginal.resize(thredHeader.formCount);
				StateMap.set(StateFlag::BADFIL);
			  }
			  FormList->reserve(formListOriginal.size());
			  FormList->insert(FormList->end(), formListOriginal.begin(), formListOriginal.end());
			}
			else {
			  auto inFormList = std::vector<FRMHEDOUT> {};
			  inFormList.resize(thredHeader.formCount);
			  bytesToRead = gsl::narrow<DWORD>(thredHeader.formCount * sizeof(decltype(inFormList.back())));
			  wrap::ReadFile(FileHandle, inFormList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				thredHeader.formCount = gsl::narrow<decltype(thredHeader.formCount)>(
				    BytesRead / sizeof(decltype(inFormList.back())));
				inFormList.resize(thredHeader.formCount);
				StateMap.set(StateFlag::BADFIL);
			  }
			  FormList->reserve(inFormList.size());
			  FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
			}
			FormList->shrink_to_fit();
			if (thredHeader.vertexCount != 0U) {
			  FormVertices->resize(thredHeader.vertexCount);
			  bytesToRead =
			      gsl::narrow<DWORD>(thredHeader.vertexCount * sizeof(decltype(FormVertices->back())));
			  ReadFile(FileHandle, FormVertices->data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				FormVertices->resize(BytesRead / sizeof(decltype(FormVertices->back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			}
			else {
			  // We have forms but no vertices - blow up the read
			  prtred();
			  return;
			}
			FormVertices->shrink_to_fit();
			if (thredHeader.dlineCount != 0U) {
			  auto inGuideList = std::vector<SATCONOUT>(thredHeader.dlineCount);
			  bytesToRead =
			      gsl::narrow<DWORD>(thredHeader.dlineCount * sizeof(decltype(inGuideList.back())));
			  ReadFile(FileHandle, inGuideList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				inGuideList.resize(BytesRead / sizeof(decltype(inGuideList.back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			  SatinGuides->reserve(inGuideList.size());
			  SatinGuides->insert(SatinGuides->end(), inGuideList.begin(), inGuideList.end());
			}
			SatinGuides->shrink_to_fit();
			if (thredHeader.clipDataCount != 0U) {
			  ClipPoints->resize(thredHeader.clipDataCount);
			  bytesToRead =
			      gsl::narrow<DWORD>(thredHeader.clipDataCount * sizeof(decltype(ClipPoints->back())));
			  ReadFile(FileHandle, ClipPoints->data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				ClipPoints->resize(BytesRead / sizeof(decltype(ClipPoints->back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			}
			ClipPoints->shrink_to_fit();
			if (ExtendedHeader->texturePointCount != 0U) {
			  TexturePointsBuffer->resize(ExtendedHeader->texturePointCount);
			  bytesToRead = gsl::narrow<DWORD>(ExtendedHeader->texturePointCount *
			                                   sizeof(decltype(TexturePointsBuffer->back())));
			  ReadFile(FileHandle, TexturePointsBuffer->data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				TexturePointsBuffer->resize(BytesRead / sizeof(decltype(TexturePointsBuffer->back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			}
			else {
			  TexturePointsBuffer->clear();
			}
			TexturePointsBuffer->shrink_to_fit();
			if (StateMap.testAndReset(StateFlag::BADFIL)) {
			  displayText::bfilmsg();
			}
			// now re-create all the pointers/indexes in the form data
			auto clipOffset   = 0U;
			auto vertexOffset = 0U;
			auto guideOffset  = 0U;
			for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
			  auto& form       = FormList->operator[](iForm);
			  form.vertexIndex = vertexOffset;
			  vertexOffset += form.vertexCount;
			  if (form.type == SAT) {
				if (form.satinGuideCount != 0U) {
				  form.satinOrAngle.guide = guideOffset;
				  guideOffset += form.satinGuideCount;
				}
			  }
			  // ToDo - do we still need to do this in v3? (we can store the offset safely in v3
			  // where we could not store the pointer in v2)
			  if (clip::isclp(form)) {
				form.angleOrClipData.clip = clipOffset;
				clipOffset += form.lengthOrCount.clipCount;
			  }
			  if (clip::iseclpx(iForm)) {
				form.borderClipData = clipOffset;
				clipOffset += form.clipEntries;
			  }
			}
			xt::setfchk();
		  }
		}
		else {
		  displayText::tabmsg(IDS_NOTHR);
		}
	  }
	  else {
		StateMap.set(StateFlag::NOTHRFIL);
		if (firstCharacter == 'p') {
		  if (tolower(fileExt[2]) == 'c') {
			ReadFile(FileHandle, &PCSHeader, sizeof(PCSHeader), &BytesRead, nullptr);
			if (fileSize == 0U) {
			  displayText::filnopn(IDS_ZEROL, *WorkingFileName);
			  return;
			}
			if (PCSHeader.leadIn == '2' && PCSHeader.colorCount == 16U) {
			  for (auto iColor = 0U; iColor < 16U; iColor++) {
				UserColor[iColor] = PCSHeader.colors[iColor];
			  }
			  fileSize -= sizeof(PCSHeader);
			  auto const pcsStitchCount = fileSize / sizeof(PCSTCH) + 2U;
			  auto       PCSDataBuffer  = std::vector<PCSTCH> {};
			  PCSDataBuffer.resize(pcsStitchCount);
			  ReadFile(FileHandle, PCSDataBuffer.data(), fileSize, &BytesRead, nullptr);
			  auto iStitch      = 0U;
			  auto iColorChange = 0U;
			  auto color        = 0U;
			  auto iPCSstitch   = 0U;
			  while (iStitch < PCSHeader.stitchCount && iPCSstitch < pcsStitchCount) {
				auto& stitch = PCSDataBuffer[iPCSstitch];
				if (stitch.tag == 3) {
				  ColorChangeTable[iColorChange].colorIndex    = stitch.fx;
				  ColorChangeTable[iColorChange++].stitchIndex = gsl::narrow<uint16_t>(iStitch);
				  color                                        = NOTFRM | stitch.fx;
				}
				else {
				  StitchBuffer->push_back(
				      fPOINTATTR {wrap::toFloat(stitch.x) + wrap::toFloat(stitch.fx) / 256.0F,
				                  wrap::toFloat(stitch.y) + wrap::toFloat(stitch.fy) / 256.0F,
				                  color});
				  iStitch++;
				}
				iPCSstitch++;
			  }
			  // Grab the bitmap filename
			  auto tnam = convert_ptr<char*>(&PCSDataBuffer[iPCSstitch]);
			  strcpy_s(PCSBMPFileName, tnam);
			  // wcscpy_s(fileExtention, sizeof(WorkingFileName) - (fileExtention - WorkingFileName), L"thr");
			  IniFile.auxFileType = AUXPCS;
			  if (PCSHeader.hoopType != LARGHUP && PCSHeader.hoopType != SMALHUP) {
				PCSHeader.hoopType = LARGHUP;
			  }
			  auto stitchRect = fRECTANGLE {};
			  sizstch(stitchRect, *StitchBuffer);
			  if (stitchRect.left < 0 || stitchRect.right > LHUPY || stitchRect.bottom < 0 ||
			      stitchRect.top > LHUPY) {
				IniFile.hoopSizeX = LHUPX;
				IniFile.hoopSizeY = LHUPY;
				chkhup();
			  }
			  else {
				if (PCSHeader.hoopType == LARGHUP) {
				  IniFile.hoopType  = LARGHUP;
				  IniFile.hoopSizeX = LHUPX;
				  IniFile.hoopSizeY = LHUPY;
				}
				else {
				  if (stitchRect.right > SHUPX || stitchRect.top > SHUPY || PCSHeader.hoopType == LARGHUP) {
					IniFile.hoopType  = LARGHUP;
					IniFile.hoopSizeX = SHUPX;
					IniFile.hoopSizeY = SHUPY;
				  }
				  else {
					IniFile.hoopType  = SMALHUP;
					IniFile.hoopSizeX = SHUPX;
					IniFile.hoopSizeY = SHUPY;
				  }
				}
			  }
			}
		  }
#if PESACT
		  else {
			auto       ec   = std::error_code {};
			auto const size = fs::file_size(*WorkingFileName, ec);
			if (ec != std::error_code {}) {
			  // ToDo - find better error message
			  displayText::filnopn(IDS_FNOPN, *WorkingFileName);
			  return;
			}
			auto fileBuf    = std::vector<uint8_t>(size);
			auto fileBuffer = fileBuf.data();
			ReadFile(FileHandle, fileBuffer, size, &BytesRead, nullptr);
			auto pesHeader = convert_ptr<PESHED*>(fileBuffer);
			if (strncmp(pesHeader->led, "#PES00", 6) != 0) { // NOLINT
			  auto fmtStr = std::wstring {};
			  displayText::loadString(fmtStr, IDS_NOTPES);
			  displayText::shoMsg(fmt::format(fmtStr, WorkingFileName->wstring()));
			  return;
			}
			auto pecHeader = convert_ptr<PECHDR*>(&fileBuffer[pesHeader->off]);
			// auto pecHeader2          = convert_ptr<PECHDR2*>(&fileBuffer[pesHeader->off + sizeof(PECHDR)]);
			auto pecOffset             = pesHeader->off + sizeof(PECHDR) + sizeof(PECHDR2);
			PESstitch                  = &fileBuffer[pecOffset];
			auto const pesColorCount   = pecHeader->colorCount + 1U;
			PEScolors                  = std::begin(pecHeader->pad);
			constexpr auto threadCount = sizeof(PESThread) / sizeof(PESThread[0]);
			auto           colorMap    = boost::dynamic_bitset<>(threadCount);
			auto           activeColor = 0U;
			for (auto iColor = 0U; iColor < pesColorCount; iColor++) {
			  if (PEScolors[iColor] < threadCount) {
				if (!colorMap.test_set(PEScolors[iColor])) {
				  auto const threadColor = PESThread[PEScolors[iColor]];
				  auto const color = RGB(threadColor.color.r, threadColor.color.g, threadColor.color.b); // NOLINT
				  UserColor[activeColor++] = color;
				  if (activeColor >= 16U) {
					break;
				  }
				}
			  }
			  else {
				auto const color         = RGB( // NOLINT
                    PESThread[0].color.r,
                    PESThread[0].color.g,
                    PESThread[0].color.b); // color unknown
				UserColor[activeColor++] = color;
			  }
			}
			PEScolorIndex = 1;
			auto loc      = fPOINT {};
			StateMap.reset(StateFlag::FILDIR);
			StitchBuffer->push_back(fPOINTATTR {});
			if (BytesRead > ((pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U)) {
			  auto color      = 0U;
			  auto iPESstitch = 0U;
			  auto const pecCount = BytesRead - (pesHeader->off + (sizeof(PECHDR) + sizeof(PECHDR2))) + 3U;
			  while (iPESstitch < pecCount) {
				if (PESstitch[iPESstitch] == 0xff && PESstitch[iPESstitch + 1U] == 0) {
				  break;
				}
				if (PESstitch[iPESstitch] == 0xfe && PESstitch[iPESstitch + 1U] == 0xb0) {
				  color = dupcol(activeColor);
				  iPESstitch += 2;
				}
				else {
				  auto locof = 0.0F;

				  if ((PESstitch[iPESstitch] & 0x80U) != 0U) {
					auto pesVal = (((PESstitch[iPESstitch] & 0x0FU) << 8U) | PESstitch[iPESstitch + 1U]) & 0xFFFU;
					if ((pesVal & 0x800U) != 0U) {
					  pesVal -= 0x1000;
					}
					auto sPesVal = gsl::narrow_cast<int32_t>(pesVal);
					locof        = gsl::narrow_cast<decltype(locof)>(sPesVal);
					iPESstitch++;
				  }
				  else {
					if (PESstitch[iPESstitch] > 0x3f) {
					  locof = wrap::toFloat(PESstitch[iPESstitch]) - 128.0F;
					}
					else {
					  locof = PESstitch[iPESstitch];
					}
				  }
				  locof *= 3.0F / 5.0F;
				  // ToDo - (PES) Use a new flag bit for this since FILDIR is not correct
				  if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					loc.y -= locof;
					StitchBuffer->push_back(fPOINTATTR {loc.x, loc.y, color});
				  }
				  else {
					loc.x += locof;
				  }
				}
				iPESstitch++;
			  }
			  // IniFile.auxFileType=AUXPES;
			  hupfn();
			}
			else {
			  auto fmtStr = std::wstring {};
			  displayText::loadString(fmtStr, IDS_NOTPES);
			  displayText::shoMsg(fmt::format(fmtStr, WorkingFileName->wstring()));
			  return;
			}
		  }
#endif
		}
		else {
		  auto dstHeader = DSTHED {};
		  ReadFile(FileHandle, &dstHeader, sizeof(dstHeader), &BytesRead, nullptr);
		  if (BytesRead == sizeof(dstHeader)) {
			if (chkdst(&dstHeader)) {
			  PCSBMPFileName[0] = 0;
			  fileSize          = GetFileSize(FileHandle, &BytesRead) - sizeof(dstHeader);
			  auto DSTData      = std::vector<DSTREC> {};
			  DSTData.resize(fileSize / sizeof(DSTREC));
			  wrap::ReadFile(FileHandle, DSTData.data(), fileSize, &BytesRead, nullptr);
			  dstran(DSTData);
			  IniFile.auxFileType = AUXDST;
			}
		  }
		  else {
			displayText::tabmsg(IDS_DST2S);
			CloseHandle(FileHandle);
			return;
		  }
		}
	  }
	  if (PCSBMPFileName[0] != 0) {
		fs::current_path(*DefaultDirectory);
		auto BMPfileName = utf::Utf8ToUtf16(std::string(std::begin(PCSBMPFileName)));
		UserBMPFileName->assign(BMPfileName);
		bfil();
	  }
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  // BufferIndex     = 0;
	  ZoomRect     = fRECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
	  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
	  thred::movStch();
	  thred::coltab();
	  StateMap.reset(StateFlag::ZUMED);
	  wchar_t buffer[3] = {0};
	  for (auto iColor = 0U; iColor < 16U; iColor++) {
		UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
		UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
		wcsncpy_s(buffer, ThreadSize[iColor], 2); // NOLINT
		SetWindowText(ThreadSizeWin[iColor], static_cast<LPCWSTR>(buffer));
	  }
	  for (auto& iColor : *UserColorWin) {
		thred::redraw(iColor);
	  }
	  thred::redraw(ColorBar);
	  if (!StitchBuffer->empty()) {
		StateMap.set(StateFlag::INIT);
	  }
	  nuAct(0);
	  StateMap.set(StateFlag::RESTCH);
	  nunams();
	  ritini();
	  auto const blank = std::wstring {};
	  displayText::butxt(HNUM, blank);
	  if (!StitchBuffer->empty()) {
		nuAct(StitchBuffer->front().attribute & COLMSK);
	  }
	  else {
		nuAct(0);
	  }
	  auxmen();
	}
	lenCalc();
	SetWindowText(
	    ThrEdWindow,
	    fmt::format(StringTable->operator[](STR_THRDBY), WorkingFileName->wstring(), *DesignerName).c_str());
	CloseHandle(FileHandle);
	StateMap.set(StateFlag::INIT);
	StateMap.reset(StateFlag::TRSET);
	if (StateMap.test(StateFlag::NOTHRFIL)) {
	  for (auto& stitch : *StitchBuffer) {
		stitch.attribute |= NOTFRM;
	  }
	}
	repair::lodchk();
  }
}

auto thred::internal::nuCol(COLORREF init) noexcept -> COLORREF {
  ColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT; // NOLINT
  ColorStruct.hwndOwner      = ThrEdWindow;
  ColorStruct.lCustData      = 0;
  ColorStruct.lpCustColors   = std::begin(CustomColor);
  ColorStruct.lpfnHook       = nullptr;
  ColorStruct.lpTemplateName = nullptr;
  ColorStruct.rgbResult      = init;
  ColorStruct.lStructSize    = sizeof(ColorStruct);
  return ChooseColor(&ColorStruct);
}

auto thred::internal::nuBak() noexcept -> COLORREF {
  BackgroundColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT; // NOLINT
  BackgroundColorStruct.hwndOwner      = ThrEdWindow;
  BackgroundColorStruct.lCustData      = 0;
  BackgroundColorStruct.lpCustColors   = std::begin(CustomBackgroundColor);
  BackgroundColorStruct.lpfnHook       = nullptr;
  BackgroundColorStruct.lpTemplateName = nullptr;
  BackgroundColorStruct.rgbResult      = BackgroundColor;
  BackgroundColorStruct.lStructSize    = sizeof(BackgroundColorStruct);
  return ChooseColor(&BackgroundColorStruct);
}

auto thred::internal::nuBit() noexcept -> COLORREF {
  BitMapColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT; // NOLINT
  BitMapColorStruct.hwndOwner      = ThrEdWindow;
  BitMapColorStruct.lCustData      = 0;
  BitMapColorStruct.lpCustColors   = std::begin(BitmapBackgroundColors);
  BitMapColorStruct.lpfnHook       = nullptr;
  BitMapColorStruct.lpTemplateName = nullptr;
  BitMapColorStruct.rgbResult      = BitmapColor;
  BitMapColorStruct.lStructSize    = sizeof(BackgroundColorStruct);
  return ChooseColor(&BitMapColorStruct);
}

void thred::pxCor2stch(POINT const& point) noexcept {
  auto ratio = gsl::narrow_cast<float>(point.x - StitchWindowAbsRect.left) /
               gsl::narrow_cast<float>(StitchWindowClientRect.right);
  SelectedPoint.x = ratio * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
  ratio = (gsl::narrow_cast<double>(StitchWindowAbsRect.bottom) - point.y) / StitchWindowClientRect.bottom;
  SelectedPoint.y = ratio * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

auto thred::px2stch() noexcept -> bool {
  if (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right &&
      Msg.pt.y >= StitchWindowAbsRect.top && Msg.pt.y <= StitchWindowAbsRect.bottom) {
	auto tdub = gsl::narrow_cast<float>(Msg.pt.x - StitchWindowAbsRect.left) /
	            gsl::narrow_cast<float>(StitchWindowClientRect.right);
	SelectedPoint.x = tdub * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	tdub =
	    (gsl::narrow_cast<double>(StitchWindowAbsRect.bottom) - Msg.pt.y) / StitchWindowClientRect.bottom;
	SelectedPoint.y = tdub * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
	return true;
  }
  return false;
}

void thred::internal::zumin() {
  if (!StateMap.testAndReset(StateFlag::ZUMACT)) {
	ZoomFactor *= ZUMFCT;
  }
  if (ZoomFactor < ZoomMin) {
	ZoomFactor = ZoomMin;
  }
  if (!StateMap.testAndSet(StateFlag::ZUMED)) {
	thred::movStch();
  }
  if (!StateMap.testAndReset(StateFlag::BZUMIN)) {
	do {
	  if (StateMap.test(StateFlag::GMRK)) {
		SelectedPoint = ZoomMarkPoint;
		break;
	  }
	  if (StateMap.test(StateFlag::FORMSEL)) {
		auto const& boundingRect = FormList->operator[](ClosestFormToCursor).rectangle;

		SelectedPoint = fPOINT {form::midl(boundingRect.right, boundingRect.left),
		                        form::midl(boundingRect.top, boundingRect.bottom)};
		break;
	  }
	  if (StateMap.test(StateFlag::FRMPSEL)) {
		auto vertexIt = std::next(FormVertices->cbegin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
		SelectedPoint = vertexIt[ClosestVertexToCursor];
		break;
	  }
	  if (StateMap.test(StateFlag::SELBOX)) {
		SelectedPoint = StitchBuffer->operator[](ClosestPointIndex);
		break;
	  }
	  if (StateMap.test(StateFlag::GRPSEL)) {
		auto groupBoundingRect = fRECTANGLE {};
		thred::selRct(groupBoundingRect);
		SelectedPoint = fPOINT {form::midl(groupBoundingRect.right, groupBoundingRect.left),
		                        form::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
		break;
	  }
	  if (StateMap.test(StateFlag::INSRT)) {
		if (StateMap.test(StateFlag::LIN1)) {
		  if (StateMap.test(StateFlag::BAKEND)) {
			SelectedPoint = StitchBuffer->back();
		  }
		  else {
			SelectedPoint = StitchBuffer->front();
		  }
		}
		else {
		  SelectedPoint =
		      fPOINT {form::midl(StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).x,
		                         StitchBuffer->operator[](ClosestPointIndex).x),
		              form::midl(StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).y,
		                         StitchBuffer->operator[](ClosestPointIndex).y)};
		}
		break;
	  }
	  if (!SelectedFormList->empty()) {
		auto        firstForm = SelectedFormList->front();
		auto const& firstRect = FormList->operator[](firstForm).rectangle;

		SelectedFormsRect = {wrap::round<int32_t>(firstRect.left),
		                     wrap::round<int32_t>(firstRect.top),
		                     wrap::round<int32_t>(firstRect.right),
		                     wrap::round<int32_t>(firstRect.bottom)};
		for (auto selectedForm : (*SelectedFormList)) {
		  auto const& rect = FormList->operator[](selectedForm).rectangle;
		  if (rect.bottom < SelectedFormsRect.bottom) {
			SelectedFormsRect.bottom = wrap::round<int32_t>(rect.bottom);
		  }
		  if (rect.top > SelectedFormsRect.top) {
			SelectedFormsRect.top = wrap::round<int32_t>(rect.top);
		  }
		  if (rect.left < SelectedFormsRect.left) {
			SelectedFormsRect.left = wrap::round<int32_t>(rect.left);
		  }
		  if (rect.right > SelectedFormsRect.right) {
			SelectedFormsRect.right = wrap::round<int32_t>(rect.right);
		  }
		}
		SelectedPoint = fPOINT {form::midl(SelectedFormsRect.right, SelectedFormsRect.left),
		                        form::midl(SelectedFormsRect.top, SelectedFormsRect.bottom)};
		break;
	  }
	  if (!thred::px2stch()) {
		centr();
	  }
	} while (false);
  }
  auto const zoomRight = UnzoomedRect.x * ZoomFactor;
  ZoomRect             = fRECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
  thred::shft(SelectedPoint);
  NearestCount = 0;
  if (!StateMap.test(StateFlag::GMRK) && StateMap.test(StateFlag::SELBOX)) {
	shft2box();
  }
  if (StateMap.test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  thred::duzrat();
  movins();
  StateMap.set(StateFlag::RESTCH);
  ilin();
}

void thred::internal::zumshft() {
  if (StateMap.test(StateFlag::ZUMED)) {
	unboxs();
	if (thred::px2stch()) {
	  NearestCount = 0;
	  thred::shft(SelectedPoint);
	  if (StateMap.test(StateFlag::RUNPAT)) {
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		RunPoint = 0;
	  }
	  StateMap.set(StateFlag::RESTCH);
	}
  }
  movins();
}

void thred::internal::zumout() {
  unboxs();
  if (StateMap.test(StateFlag::ZUMED)) {
	do {
	  if (StateMap.test(StateFlag::GMRK)) {
		SelectedPoint = ZoomMarkPoint;
		break;
	  }
	  if (StateMap.test(StateFlag::FORMSEL)) {
		auto const& boundingRect = FormList->operator[](ClosestFormToCursor).rectangle;

		SelectedPoint = fPOINT {form::midl(boundingRect.right, boundingRect.left),
		                        form::midl(boundingRect.top, boundingRect.bottom)};
		break;
	  }
	  if (StateMap.test(StateFlag::FRMPSEL)) {
		auto vertexIt = std::next(FormVertices->cbegin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
		SelectedPoint = vertexIt[ClosestVertexToCursor];
		break;
	  }
	  if (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::INSRT)) {
		SelectedPoint = StitchBuffer->operator[](ClosestPointIndex);
		break;
	  }
	  if (StateMap.test(StateFlag::GRPSEL)) {
		auto groupBoundingRect = fRECTANGLE {};
		thred::selRct(groupBoundingRect);
		SelectedPoint = fPOINT {form::midl(groupBoundingRect.right, groupBoundingRect.left),
		                        form::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
		break;
	  }
	  if (StateMap.test(StateFlag::SELBOX)) {
		shft2box();
		break;
	  }
	  if (!thred::px2stch()) {
		centr();
	  }
	} while (false);
	ZoomFactor /= ZUMFCT;
	if (ZoomFactor > 0.98F) {
	  ZoomFactor = 1;
	  StateMap.reset(StateFlag::ZUMED);
	  ZoomRect =
	      fRECTANGLE {0.0F, gsl::narrow<float>(UnzoomedRect.y), gsl::narrow<float>(UnzoomedRect.x), 0.0F};
	  thred::movStch();
	  NearestCount = 0;
	}
	else {
	  auto const zoomRight = UnzoomedRect.x * ZoomFactor;
	  ZoomRect = fRECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
	  thred::shft(SelectedPoint);
	}
	if (StateMap.test(StateFlag::RUNPAT)) {
	  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	  RunPoint = 0;
	}
	StateMap.set(StateFlag::RESTCH);
	thred::duzrat();
	movins();
  }
}

void thred::internal::duClos(uint32_t startStitch, uint32_t stitchCount) noexcept {
  for (auto iStitch = startStitch; iStitch < startStitch + stitchCount; iStitch++) {
	auto const stitch = StitchBuffer->operator[](iStitch);

	auto const cx =
	    ((stitch.x > SelectedPoint.x) ? (stitch.x - SelectedPoint.x) : (SelectedPoint.x - stitch.x));
	auto const cy =
	    ((stitch.y > SelectedPoint.y) ? (stitch.y - SelectedPoint.y) : (SelectedPoint.y - stitch.y));
	auto sum   = hypot(cx, cy);
	auto tind0 = iStitch;
	for (auto iNear = 0U; iNear < NERCNT; iNear++) {
	  if (sum < GapToNearest[iNear]) {
		auto const lowestSum = GapToNearest[iNear];
		auto const tind1     = NearestPoint[iNear];
		GapToNearest[iNear]  = sum;
		NearestPoint[iNear]  = tind0;
		sum                  = lowestSum;
		tind0                = tind1;
	  }
	}
  }
}

void thred::internal::closPnt() {
  unbox();
  unboxs();
  for (auto iNear = 0U; iNear < 4; iNear++) {
	GapToNearest[iNear] = 1e99;
	NearestPoint[iNear] = -1;
  }
  thred::px2stch();
  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
	auto const iStitch0 = ColorChangeTable[iColor].stitchIndex;
	auto const iStitch1 = ColorChangeTable[iColor + 1U].stitchIndex;
	if (StateMap.test(StateFlag::HID)) {
	  if (ColorChangeTable[iColor].colorIndex == ActiveColor) {
		duClos(iStitch0, iStitch1 - iStitch0);
	  }
	}
	else {
	  duClos(iStitch0, iStitch1 - iStitch0);
	}
  }
  GetClientRect(MainStitchWin, &StitchWindowClientRect);
  for (auto const iNear : NearestPoint) {
	if (stch2px(iNear)) {
	  NearestPoint[NearestCount]   = iNear;
	  NearestPixel[NearestCount++] = StitchCoordinatesPixels;
	}
  }
  boxs();
}

auto thred::internal::closPnt1(uint32_t* closestStitch) -> bool {
  if (closestStitch != nullptr) {
	auto       closestIndex = 0U;
	auto const pointToCheck =
	    POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};

	if (StateMap.test(StateFlag::SELBOX) && stch2px(ClosestPointIndex)) {
	  if (hypot(StitchCoordinatesPixels.x - pointToCheck.x, StitchCoordinatesPixels.y - pointToCheck.y) < CLOSENUF) {
		*closestStitch = ClosestPointIndex;
		return true;
	  }
	}
	for (auto iNear = 0U; iNear < NearestCount; iNear++) {
	  auto const offset = BoxOffset[iNear];
	  if (pointToCheck.x >= NearestPixel[iNear].x - offset && pointToCheck.x <= NearestPixel[iNear].x + offset &&
	      pointToCheck.y >= NearestPixel[iNear].y - offset && pointToCheck.y <= NearestPixel[iNear].y + offset) {
		*closestStitch = NearestPoint[iNear];
		return true;
	  }
	}
	thred::px2stch();
	DistanceToClick = 1e99;
	if (StateMap.test(StateFlag::HID)) {
	  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
		if (ColorChangeTable[iColor].colorIndex == ActiveColor) {
		  for (auto iStitch = ColorChangeTable[iColor].stitchIndex;
		       iStitch < ColorChangeTable[iColor + 1U].stitchIndex;
		       iStitch++) {
			auto const stitch = StitchBuffer->operator[](iStitch);
			if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right &&
			    stitch.y >= ZoomRect.bottom && stitch.y <= ZoomRect.top) {
			  auto const cx = ((stitch.x > SelectedPoint.x) ? (stitch.x - SelectedPoint.x)
			                                                : (SelectedPoint.x - stitch.x));
			  auto const cy = ((stitch.y > SelectedPoint.y) ? (stitch.y - SelectedPoint.y)
			                                                : (SelectedPoint.y - stitch.y));
			  auto const currentDistance = hypot(cx, cy);
			  if (currentDistance < DistanceToClick) {
				DistanceToClick = currentDistance;
				closestIndex    = iStitch;
			  }
			}
		  }
		}
	  }
	}
	else {
	  auto currentStitch = 0U;
	  for (auto& stitch : *StitchBuffer) {
		auto const layer = (stitch.attribute & LAYMSK) >> LAYSHFT;
		if ((ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
		  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right &&
		      stitch.y >= ZoomRect.bottom && stitch.y <= ZoomRect.top) {
			auto const cx              = stitch.x - SelectedPoint.x;
			auto const cy              = stitch.y - SelectedPoint.y;
			auto const currentDistance = hypot(cx, cy);
			if (currentDistance < DistanceToClick) {
			  DistanceToClick = currentDistance;
			  closestIndex    = currentStitch;
			}
		  }
		}
		currentStitch++;
	  }
	}
	if (DistanceToClick == 1e99) {
	  return false;
	}
	stch2px(closestIndex);
	if (StateMap.test(StateFlag::IGNTHR)) {
	  *closestStitch = closestIndex;
	  return true;
	}

	if (hypot(pointToCheck.x - StitchCoordinatesPixels.x, pointToCheck.y - StitchCoordinatesPixels.y) < CLOSENUF) {
	  *closestStitch = closestIndex;
	  return true;
	}

	return false;
  }
  return false;
}

auto thred::internal::pt2colInd(uint32_t iStitch) noexcept -> uint32_t {
  auto iColor = 0U;

  for (iColor = 0; iColor < ColorChanges; iColor++) {
	if (ColorChangeTable[iColor].stitchIndex > iStitch) {
	  break;
	}
  }
  return iColor;
}

void thred::internal::toglup() {
  if (StateMap.testAndFlip(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, StringTable->operator[](STR_UPOF));
  }
  else {
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  ClosestPointIndex = GroupStartStitch;
	  StateMap.set(StateFlag::SELBOX);
	  StateMap.reset(StateFlag::FRMPSEL);
	}
	else {
	  if (!StateMap.test(StateFlag::SELBOX)) {
		ClosestPointIndex = 0;
		if (StateMap.testAndReset(StateFlag::FORMSEL)) {
		  while (ClosestPointIndex < wrap::toUnsigned(StitchBuffer->size()) &&
		         form::notfstch(StitchBuffer->operator[](ClosestPointIndex).attribute)) {
			ClosestPointIndex++;
		  }
		  StateMap.set(StateFlag::SELBOX);
		  StateMap.reset(StateFlag::FRMPSEL);
		}
	  }
	}
	displayText::butxt(HUPTO, StringTable->operator[](STR_UPON));
  }
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::toglHid() {
  if (StateMap.testAndFlip(StateFlag::HID)) {
	StateMap.reset(StateFlag::FRMOF);
  }
  else {
	StateMap.set(StateFlag::FRMOF);
  }
  unthum();
  thred::redraw(ButtonWin->operator[](HHID));
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::dulin() {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (MoveLine0[0].x == MoveLine1[1].x && MoveLine0[0].y == MoveLine1[1].y) {
	if (StateMap.test(StateFlag::ISDWN)) {
	  Polyline(StitchWindowDC, static_cast<POINT const*>(MoveLine0), 2);
	}
	else {
	  Polyline(StitchWindowDC, static_cast<POINT const*>(MoveLine1), 2);
	}
  }
  else {
	if (StateMap.test(StateFlag::ISDWN)) {
	  Polyline(StitchWindowDC, static_cast<POINT const*>(MoveLine0), 2);
	}
	if (StateMap.test(StateFlag::ISUP)) {
	  Polyline(StitchWindowDC, static_cast<POINT const*>(MoveLine1), 2);
	}
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
  StateMap.flip(StateFlag::WASLIN);
}

void thred::internal::unlin() {
  if (StateMap.test(StateFlag::WASLIN)) {
	dulin();
  }
}

void thred::internal::movbox() {
  if (!StitchBuffer->empty()) {
	auto const stitch = StitchBuffer->operator[](ClosestPointIndex);
	if (stch2px(ClosestPointIndex)) {
	  unbox();
	  OutputDebugString(fmt::format(L"movbox:Stitch [{}] form [{}] type [{}] x [{}] y[{}]\n",
	                                ClosestPointIndex,
	                                ((stitch.attribute & FRMSK) >> FRMSHFT),
	                                ((stitch.attribute & TYPMSK) >> TYPSHFT),
	                                stitch.x,
	                                stitch.y)
	                        .c_str());
	  dubox();
	  if (StateMap.test(StateFlag::UPTO)) {
		StateMap.set(StateFlag::RESTCH);
	  }
	}
	else {
	  shft2box();
	  StateMap.set(StateFlag::SELBOX);
	  StateMap.reset(StateFlag::FRMPSEL);
	  StateMap.set(StateFlag::RESTCH);
	}
	nuAct(ClosestPointIndex);
	ritcor(StitchBuffer->operator[](ClosestPointIndex));
  }
}

auto thred::internal::chkhid(uint32_t colorToCheck) -> bool {
  if (StateMap.test(StateFlag::HID)) {
	return ColorChangeTable[colorToCheck].colorIndex == ActiveColor;
  }

  return true;
}

/*
This function attempts to find the stitch the user is trying to select.
A rectangle is created that is slightly larger than the stitch.
If the mouse position is outside this rectangle, the stitch is disqualified.
If the mouse position is inside the rectangle, the distance from the stitch
 to the select point (SelectedPoint) is calculated.

Find the equation for the line by solving the linear parametric eauations using
stitch = StitchBuffer->operator[](StitchIndex);
stitchFwd = StitchBuffer->operator[](StitchIndex + 1);

  stitch.x + Slope*stitch.y=offset
  stitchFwd.x + Slope*stitchFwd.y=offset

substituting:

  stitch.x + Slope*stitch.y=stitchFwd.x + Slope*stitchFwd.y

collecting terms:

  Slope*stitch.y - Slope*stitchFwd.y = stitchFwd.x - stitch.x
  Slope*(stitch.y - stitchFwd.y) = stitchFwd.x - stitch.x
  Slope = (stitchFwd.x - stitch.x)/(stitch.y - stitchFwd.y)

define xba = stitchFwd.x - stitch.x
define yab = stitch.y - stitchFwd.y

  Slope=xba/yab;

back substituting into stitch.x + Slope*stitch.y=offset

  offset=stitch.x + Slope*stitch.y

The equation for a point vertical to the equation for the line and running
 through SelectedPoint is:

  SelectedPoint.x - SelectedPoint.y/Slope = poff

If intersection is the intersections between the two lines then

  intersection.x - Slope*intersection.y = offset
  intersection.x + intersection.y/Slope = poff

Subtracting the two equations

  Slope*intersection.y + intersection.y/Slope = offset-poff

Multiply by Slope

  Slope*Slope*intersection.y + intersection.y = Slope(offset-poff)
  intersection.y(Slope*Slope + 1) = Slope(offset - poff)
  intersection.y = Slope*(offset - poff)/(Slope*Slope + 1)

back substitute into intersection.x+Slope*intersection.y=offset

  intersection.x = offset - Slope*intersection.y

if dx = intersection.x - spnt.x & dy = intersection.y-spnt.y

the distance from the point to the line is given by

  sqrt(dx*dx + dy*dy)
*/

auto thred::internal::closlin() -> uint32_t {
  // ToDo - This function needs to be thoroughly checked and compared to findDistanceToSide

  auto closestPoint = 0xffffffffU;
  auto sum          = 1e99;
  auto checkedPoint = fPOINT {};

  constexpr auto Tolerance = 20.0F;
  thi::unboxs();
  auto offset = gsl::narrow_cast<float>(Msg.pt.x - StitchWindowAbsRect.left) /
                gsl::narrow_cast<float>(StitchWindowAbsRect.right - StitchWindowAbsRect.left);
  checkedPoint.x = offset * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
  offset         = (gsl::narrow_cast<float>(StitchWindowAbsRect.bottom) - Msg.pt.y) /
           (gsl::narrow_cast<float>(StitchWindowAbsRect.bottom) - StitchWindowAbsRect.top);
  checkedPoint.y       = (offset * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom);
  offset               = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
  auto const tolerance = offset * Tolerance;
  for (auto iChange = 0U; iChange < ColorChanges; iChange++) {
	auto stitchCount = gsl::narrow<uint16_t>(
	    std::abs(ColorChangeTable[iChange + 1U].stitchIndex - ColorChangeTable[iChange].stitchIndex));
	if (ColorChangeTable[iChange + 1U].stitchIndex == StitchBuffer->size()) {
	  --stitchCount;
	}
	auto const stitches = std::next(StitchBuffer->begin(), ColorChangeTable[iChange].stitchIndex);
	if (thi::chkhid(iChange)) {
	  for (auto iStitch = 0U; iStitch < stitchCount; iStitch++) {
		auto const layer = (stitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
		if ((ActiveLayer == 0U) || (layer == 0U) || (layer == ActiveLayer)) {
		  auto       boundingRect = fRECTANGLE {};
		  auto const xba =
		      wrap::round<int32_t>(stitches[wrap::toSize(iStitch) + 1U].x - stitches[iStitch].x);
		  auto const yab =
		      wrap::round<int32_t>(stitches[iStitch].y - stitches[wrap::toSize(iStitch) + 1U].y);
		  if (xba > 0) {
			boundingRect.left  = stitches[iStitch].x - tolerance;
			boundingRect.right = stitches[wrap::toSize(iStitch) + 1U].x + tolerance;
		  }
		  else {
			boundingRect.left  = stitches[wrap::toSize(iStitch) + 1U].x - tolerance;
			boundingRect.right = stitches[iStitch].x + tolerance;
		  }
		  if (yab < 0) {
			boundingRect.bottom = stitches[iStitch].y - tolerance;
			boundingRect.top    = stitches[wrap::toSize(iStitch) + 1U].y + tolerance;
		  }
		  else {
			boundingRect.bottom = stitches[wrap::toSize(iStitch) + 1U].y - tolerance;
			boundingRect.top    = stitches[iStitch].y + tolerance;
		  }
		  if (checkedPoint.x > boundingRect.left && checkedPoint.x < boundingRect.right &&
		      checkedPoint.y > boundingRect.bottom && checkedPoint.y < boundingRect.top) {
			auto tsum         = 0.0;
			auto intersection = fPOINT {};
			auto dx           = 0.0F;
			auto dy           = 0.0F;
			do {
			  if (yab == 0) {
				// stitch is horizontal
				intersection.x = checkedPoint.x;
				intersection.y = stitches[iStitch].y;
			  }
			  else {
				if (xba == 0) {
				  // stitch is vertical
				  dx = stitches[iStitch].x - checkedPoint.x;
				  boundingRect.top -= tolerance;
				  boundingRect.bottom += tolerance;
				  if (checkedPoint.y > boundingRect.top) {
					dy   = checkedPoint.y - boundingRect.top;
					tsum = hypot(dx, dy);
					break;
				  }
				  if (checkedPoint.y < boundingRect.bottom) {
					dy   = checkedPoint.y - boundingRect.bottom;
					tsum = hypot(dx, dy);
					break;
				  }
				  tsum = fabs(dx);
				  break;
				}
				auto const slope = gsl::narrow_cast<float>(xba) / yab;
				offset           = stitches[iStitch].x + slope * stitches[iStitch].y;
				auto const poff  = checkedPoint.x - checkedPoint.y / slope;

				intersection = fPOINT {offset - slope * intersection.y,
				                       slope * (offset - poff) / (slope * slope + 1.0F)};
				dx           = intersection.x - checkedPoint.x;
				dy           = intersection.y - checkedPoint.y;
			  }
			  boundingRect.top -= tolerance;
			  boundingRect.bottom += tolerance;
			  boundingRect.left += tolerance;
			  boundingRect.right -= tolerance;
			  if (intersection.x < boundingRect.left) {
				if (intersection.y < boundingRect.bottom) {
				  dx = checkedPoint.x - boundingRect.left;
				  dy = checkedPoint.y - boundingRect.bottom;
				}
				else {
				  dx = checkedPoint.x - boundingRect.left;
				  dy = checkedPoint.y - boundingRect.top;
				}
			  }
			  else {
				if (intersection.x > boundingRect.right) {
				  if (intersection.y < boundingRect.bottom) {
					dx = checkedPoint.x - boundingRect.right;
					dy = checkedPoint.y - boundingRect.bottom;
				  }
				  else {
					dx = checkedPoint.x - boundingRect.right;
					dy = checkedPoint.y - boundingRect.top;
				  }
				}
			  }
			  tsum = sqrt(dx * dx + dy * dy);
			} while (false);
			if (tsum < sum) {
			  sum          = tsum;
			  closestPoint = iStitch + ColorChangeTable[iChange].stitchIndex;
			}
		  }
		}
	  }
	}
  }
  if (sum > tolerance) {
	return 0xffffffff;
  }
  return closestPoint;
}

void thred::internal::istch() {
  xlin();
  xlin1();
  if (StateMap.test(StateFlag::SELBOX)) {
	if ((ClosestPointIndex != 0U) &&
	    ClosestPointIndex != gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U)) {
	  thred::px2stch();
	  auto& stitch    = StitchBuffer->operator[](ClosestPointIndex);
	  auto& prvStitch = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) - 1U);
	  auto& nxtStitch = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
	  auto const angt = atan2(stitch.y - SelectedPoint.y, stitch.x - SelectedPoint.x);
	  auto const angb = atan2(stitch.y - prvStitch.y, stitch.x - prvStitch.x);
	  auto const angf = atan2(stitch.y - nxtStitch.y, stitch.x - nxtStitch.x);
	  if (fabs(angf - angt) > fabs(angb - angt)) {
		ClosestPointIndex--;
	  }
	}
	else {
	  if (ClosestPointIndex == gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U)) {
		ClosestPointIndex--;
	  }
	}
  }
  else {
	ClosestPointIndex = closlin();
  }
  if (ClosestPointIndex == gsl::narrow_cast<decltype(ClosestPointIndex)>(-1)) {
	StateMap.reset(StateFlag::INSRT);
  }
  else {
	StateMap.set(StateFlag::INSRT);
	duIns();
	SetCapture(ThrEdWindow);
	displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
	nuAct(ClosestPointIndex);
  }
}

void thred::internal::mark() {
  if (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::INSRT) || closPnt1(&ClosestPointIndex)) {
	unbox();
	xlin();
	StateMap.reset(StateFlag::CAPT);
	StateMap.reset(StateFlag::SELBOX);
	StateMap.reset(StateFlag::INSRT);
	StateMap.reset(StateFlag::SCROS);
	StateMap.reset(StateFlag::ECROS);
	GroupStitchIndex = ClosestPointIndex;
	StateMap.set(StateFlag::GRPSEL);
  }
}

void thred::internal::selCol() {
  if (!StitchBuffer->empty()) {
	auto iStitch = 0U;
	if (StateMap.test(StateFlag::SELBOX)) {
	  iStitch = ClosestPointIndex;
	}
	else {
	  if (StateMap.test(StateFlag::GRPSEL)) {
		iStitch = GroupStitchIndex;
	  }
	  else {
		iStitch = 0;
	  }
	}
	if (iStitch > gsl::narrow<decltype(iStitch)>(StitchBuffer->size() - 1U)) {
	  iStitch = gsl::narrow<decltype(iStitch)>(StitchBuffer->size() - 1U);
	}
	GroupStitchIndex  = iStitch;
	ClosestPointIndex = iStitch;
	auto const color  = StitchBuffer->operator[](iStitch).attribute & COLMSK;
	while ((ClosestPointIndex != 0U) && (StitchBuffer->operator[](ClosestPointIndex).attribute & COLMSK) == color) {
	  ClosestPointIndex--;
	}
	if ((StitchBuffer->operator[](ClosestPointIndex).attribute & COLMSK) != color) {
	  ClosestPointIndex++;
	}
	while (GroupStitchIndex < gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U) &&
	       (StitchBuffer->operator[](GroupStitchIndex).attribute & COLMSK) == color) {
	  GroupStitchIndex++;
	}
	if ((StitchBuffer->operator[](ClosestPointIndex).attribute & COLMSK) != color) {
	  ClosestPointIndex--;
	}
	if (GroupStitchIndex > gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U)) {
	  GroupStitchIndex = gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U);
	}
	StateMap.set(StateFlag::GRPSEL);
	unbox();
	thred::grpAdj();
	ActiveColor = color;
	StateMap.set(StateFlag::RESTCH);
  }
}

void thred::internal::newFil() {
  StateMap.reset(StateFlag::CMPDO);
  if (PCSBMPFileName[0] != 0) {
	PCSBMPFileName[0] = 0;
	DeleteObject(BitmapFileHandle);
	ReleaseDC(ThrEdWindow, BitmapDC);
  }
  deldu();
  DesignerName->assign(utf::Utf8ToUtf16(std::string(std::begin(IniFile.designerName))));
  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
  *ThrName = *DefaultDirectory / (StringTable->operator[](STR_NUFIL).c_str());
  ritfnam(*DesignerName);
  auto       designer     = utf::Utf16ToUtf8(*DesignerName);
  auto const modifierName = gsl::span<char> {ExtendedHeader->modifierName};
  std::copy(designer.cbegin(), designer.cend(), modifierName.begin());
  rstdu();
  rstAll();
  displayText::clrhbut(3);
  StateMap.reset(StateFlag::MOVSET);
  PCSHeader.leadIn     = 0x32;
  PCSHeader.colorCount = 16U;
  unbox();
  xlin();
  xlin1();
  StateMap.reset(StateFlag::INIT);
  StateMap.reset(StateFlag::INSRT);
  StateMap.reset(StateFlag::LIN1);
  StateMap.reset(StateFlag::FORMSEL);
  StateMap.reset(StateFlag::BAKACT);
  StateMap.reset(StateFlag::GMRK);
  StitchBuffer->clear();
  StitchBuffer->shrink_to_fit();
  DisplayedColorBitmap.reset();
  PCSBMPFileName[0] = 0;
  FormVertices->clear();
  FormVertices->shrink_to_fit();
  TexturePointsBuffer->clear();
  TexturePointsBuffer->shrink_to_fit();
  ClipPoints->clear();
  ClipPoints->shrink_to_fit();
  SatinGuides->clear();
  SatinGuides->shrink_to_fit();
  FormList->clear();
  FormList->shrink_to_fit();
  ColorChanges = 0;
  KnotCount    = 0;
  WorkingFileName->clear();
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	thred::redraw(DefaultColorWin->operator[](iColor));
	thred::redraw(UserColorWin->operator[](iColor));
	thred::redraw(ThreadSizeWin[iColor]);
  }
  thred::zumhom();
}

void thred::bBox() noexcept {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  Polyline(StitchWindowDC, static_cast<POINT const*>(ZoomBoxLine), 5);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unbBox() {
  if (StateMap.testAndReset(StateFlag::BZUM)) {
	thred::bBox();
  }
}

void thred::internal::rebox() {
  unbox();
  if (closPnt1(&ClosestPointIndex)) {
	nuAct(ClosestPointIndex);
	if (stch2px(ClosestPointIndex)) {
	  dubox();
	  OutputDebugString(
	      fmt::format(L"rebox:Stitch [{}] form [{}] type [{}]\n",
	                  ClosestPointIndex,
	                  ((StitchBuffer->operator[](ClosestPointIndex).attribute & FRMSK) >> FRMSHFT),
	                  ((StitchBuffer->operator[](ClosestPointIndex).attribute & TYPMSK) >> TYPSHFT))
	          .c_str());
	}
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
	  StateMap.reset(StateFlag::SCROS);
	  StateMap.reset(StateFlag::ECROS);
	  SearchLine->clear();
	  SearchLine->shrink_to_fit();
	  StateMap.set(StateFlag::RESTCH);
	  for (auto& window : *UserColorWin) {
		thred::redraw(window);
	  }
	}
	ritcor(StitchBuffer->operator[](ClosestPointIndex));
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void thred::delstchm() {
  thred::rngadj();
  auto start = std::next(StitchBuffer->begin(), GroupStartStitch);
  auto end   = std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch + 1U));
  StitchBuffer->erase(start, end);
}

void thred::internal::duclp() noexcept {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  Polyline(StitchWindowDC, static_cast<POINT const*>(ClipInsertBoxLine), 5);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unclp() {
  if (StateMap.testAndReset(StateFlag::CLPSHO)) {
	duclp();
  }
}

void thred::internal::clpbox() {
  auto const ratio =
      gsl::narrow_cast<float>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);

  thred::px2stch();
  if (SelectedPoint.x + ClipRectSize.cx > UnzoomedRect.x) {
	SelectedPoint.x = UnzoomedRect.x - ClipRectSize.cx;
  }
  if (SelectedPoint.y + ClipRectSize.cy > UnzoomedRect.y) {
	SelectedPoint.y = UnzoomedRect.y - ClipRectSize.cy;
  }
  ClipOrigin = {wrap::round<int32_t>(SelectedPoint.x), wrap::round<int32_t>(SelectedPoint.y)};
  auto const adjustedSize =
      SIZE {wrap::ceil<int32_t>(ClipRectSize.cx * ratio), wrap::ceil<int32_t>(ClipRectSize.cy * ratio)};
  StitchCoordinatesPixels = {wrap::ceil<int32_t>((SelectedPoint.x - ZoomRect.left) * ratio),
                             wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                                                 (SelectedPoint.y - ZoomRect.bottom) * ratio -
                                                 adjustedSize.cy)};
  ClipInsertBoxLine[0].x = ClipInsertBoxLine[3].x = ClipInsertBoxLine[4].x = StitchCoordinatesPixels.x;
  ClipInsertBoxLine[0].y = ClipInsertBoxLine[1].y = ClipInsertBoxLine[4].y = StitchCoordinatesPixels.y;
  ClipInsertBoxLine[1].x = ClipInsertBoxLine[2].x = ClipInsertBoxLine[0].x + adjustedSize.cx;
  ClipInsertBoxLine[2].y = ClipInsertBoxLine[3].y = ClipInsertBoxLine[0].y + adjustedSize.cy;
  StateMap.set(StateFlag::CLPSHO);
  duclp();
}

void thred::internal::lodclp(uint32_t iStitch) {
  StitchBuffer->insert(std::next(StitchBuffer->begin(), iStitch), ClipBuffer->size(), fPOINTATTR {});
  ClosestPointIndex = iStitch;
  for (auto& clip : *ClipBuffer) {
	StitchBuffer->operator[](iStitch++) = fPOINTATTR {
	    clip.x + ClipOrigin.x, clip.y + ClipOrigin.y, (clip.attribute & COLMSK) | LayerIndex | NOTFRM};
  }
  GroupStitchIndex = iStitch - 1U;
  StateMap.set(StateFlag::GRPSEL);
  if (!StitchBuffer->empty()) {
	StateMap.set(StateFlag::INIT);
  }
}

void thred::internal::rSelbox() {
  auto const ratio =
      gsl::narrow_cast<float>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  auto const adjustedSelectSize = SIZE {gsl::narrow_cast<int32_t>(lround(SelectBoxSize.cx * ratio)),
                                        gsl::narrow_cast<int32_t>(lround(SelectBoxSize.cy * ratio))};

  unsel();
  thred::px2stch();
  if (SelectedPoint.x - SelectBoxOffset.x + SelectBoxSize.cx >= UnzoomedRect.x) {
	SelectedPoint.x = gsl::narrow_cast<float>(UnzoomedRect.x - SelectBoxSize.cx + SelectBoxOffset.x);
  }
  if (SelectedPoint.y - SelectBoxOffset.y + SelectBoxSize.cy >= UnzoomedRect.y) {
	SelectedPoint.y = gsl::narrow_cast<float>(UnzoomedRect.y - SelectBoxSize.cy + SelectBoxOffset.y);
  }
  if (SelectedPoint.x - SelectBoxOffset.x < 0) {
	SelectedPoint.x = SelectBoxOffset.x;
  }
  if (SelectedPoint.y - SelectBoxOffset.y < 0) {
	SelectedPoint.y = SelectBoxOffset.y;
  }
  StitchCoordinatesPixels = {
      wrap::ceil<int32_t>((SelectedPoint.x - ZoomRect.left - SelectBoxOffset.x) * ratio),
      wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                          (SelectedPoint.y - ZoomRect.bottom - SelectBoxOffset.y) * ratio -
                          adjustedSelectSize.cy)};
  auto& formControls = *FormControlPoints;
  formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x =
      StitchCoordinatesPixels.x;
  formControls[1].x = formControls[5].x = StitchCoordinatesPixels.x + adjustedSelectSize.cx / 2;
  formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y =
      StitchCoordinatesPixels.y;
  formControls[3].y = formControls[7].y = StitchCoordinatesPixels.y + adjustedSelectSize.cy / 2;
  formControls[4].y = formControls[5].y = formControls[6].y =
      StitchCoordinatesPixels.y + adjustedSelectSize.cy;
  formControls[2].x = formControls[3].x = formControls[4].x =
      StitchCoordinatesPixels.x + adjustedSelectSize.cx;
  StateMap.set(StateFlag::SELSHO);
  dusel(StitchWindowDC);
}

void thred::internal::duSelbox() {
  thred::px2stch();
  SelectBoxSize = {wrap::round<int32_t>(StitchRangeRect.right - StitchRangeRect.left),
                   wrap::round<int32_t>(StitchRangeRect.top - StitchRangeRect.bottom)};

  SelectBoxOffset = {wrap::round<int32_t>(SelectedPoint.x - StitchRangeRect.left),
                     wrap::round<int32_t>(SelectedPoint.y - StitchRangeRect.bottom)};
}

void thred::internal::setbak(uint32_t penWidth) noexcept {
  if (BackgroundPenWidth != penWidth) {
	BackgroundPen = nuPen(BackgroundPen, penWidth, BackgroundColor);
  }
}

void thred::internal::stchbox(uint32_t iStitch, HDC dc) {
  POINT      line[5] = {};
  auto const layer   = (StitchBuffer->operator[](iStitch).attribute & LAYMSK) >> LAYSHFT;
  auto const offset  = MulDiv(IniFile.stitchSizePixels, *screenDPI, 96);

  if ((ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
	stch2px1(iStitch);
	line[0].x = line[3].x = line[4].x = StitchCoordinatesPixels.x - offset;
	line[0].y = line[1].y = StitchCoordinatesPixels.y - offset;
	line[1].x = line[2].x = StitchCoordinatesPixels.x + offset;
	line[2].y = line[3].y = StitchCoordinatesPixels.y + offset;
	line[4].y             = StitchCoordinatesPixels.y - offset;
	Polyline(dc, static_cast<POINT const*>(line), 5);
  }
}

void thred::sCor2px(fPOINT const& stitchCoordinate, POINT& pixelCoordinate) {
  pixelCoordinate = {wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                     wrap::ceil<int32_t>(StitchWindowClientRect.bottom +
                                         (ZoomRect.bottom - stitchCoordinate.y) * ZoomRatio.y)};
}

auto thred::internal::sdCor2px(fPOINTATTR const& stitchPoint) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(StitchWindowClientRect.bottom +
                                    (ZoomRect.bottom - stitchPoint.y) * ZoomRatio.y)};
}

void thred::internal::durot() noexcept {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  Polyline(StitchWindowDC, static_cast<POINT const*>(RotateBoxOutline), 5);
  Polyline(StitchWindowDC, static_cast<POINT const*>(RotateBoxCrossVertLine), 2);
  Polyline(StitchWindowDC, static_cast<POINT const*>(RotateBoxCrossHorzLine), 2);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unrot() {
  if (StateMap.testAndReset(StateFlag::ROTSHO)) {
	durot();
  }
}

void thred::internal::durotu() noexcept {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  Polyline(StitchWindowDC, static_cast<POINT const*>(RotateBoxToCursorLine), 2);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unrotu() {
  if (StateMap.testAndReset(StateFlag::ROTUSHO)) {
	durotu();
  }
}

void thred::internal::rotang(fPOINT unrotatedPoint, POINT& rotatedPoint, float rotationAngle, fPOINT const& rotationCenter) {
  auto       distanceToCenter = 0.0F;
  auto       newAngle         = 0.0F;
  auto const dx               = unrotatedPoint.x - rotationCenter.x;
  auto const dy               = unrotatedPoint.y - rotationCenter.y;

  if (dx != 0.0F) {
	distanceToCenter = hypot(dx, dy);
	newAngle         = atan2(dy, dx);
	newAngle += rotationAngle;
  }
  else {
	distanceToCenter = abs(dy);
	if (dy > 0) {
	  newAngle = PI_F / 2.0F + rotationAngle;
	}
	else {
	  newAngle = rotationAngle - PI_F / 2.0F;
	}
  }
  auto const point = fPOINT {rotationCenter.x + distanceToCenter * cos(newAngle),
                             rotationCenter.y + distanceToCenter * sin(newAngle)};
  thred::sCor2px(point, rotatedPoint);
}

void thred::rotang1(fPOINTATTR const& unrotatedPoint,
                    fPOINT&           rotatedPoint,
                    float             rotationAngle,
                    fPOINT const&     rotationCenter) noexcept {
  auto       distanceToCenter = 0.0F;
  auto       newAngle         = 0.0F;
  auto const dx               = unrotatedPoint.x - rotationCenter.x;
  auto const dy               = unrotatedPoint.y - rotationCenter.y;

  if (dx != 0.0) {
	distanceToCenter = hypot(dx, dy);
	newAngle         = atan2(dy, dx);
	newAngle += rotationAngle;
  }
  else {
	if (dy > 0.0) {
	  distanceToCenter = dy;
	  newAngle         = PI_F / 2.0F + rotationAngle;
	}
	else {
	  distanceToCenter = -dy;
	  newAngle         = rotationAngle - PI_F / 2.0F;
	}
  }
  rotatedPoint.x = rotationCenter.x + distanceToCenter * cos(newAngle);
  rotatedPoint.y = rotationCenter.y + distanceToCenter * sin(newAngle);
}

void thred::rotangf(fPOINT const& unrotatedPoint, fPOINT& rotatedPoint, float rotationAngle, fPOINT const& rotationCenter) noexcept {
  auto       distanceToCenter = 0.0F;
  auto       newAngle         = 0.0F;
  auto const dx               = unrotatedPoint.x - rotationCenter.x;
  auto const dy               = unrotatedPoint.y - rotationCenter.y;

  if (dx != 0.0) {
	distanceToCenter = hypot(dx, dy);
	newAngle         = atan2(dy, dx);
	newAngle += rotationAngle;
  }
  else {
	if (dy > 0) {
	  distanceToCenter = dy;
	  newAngle         = PI_F / 2.0F + rotationAngle;
	}
	else {
	  distanceToCenter = -dy;
	  newAngle         = rotationAngle - PI_F / 2.0F;
	}
  }
  rotatedPoint.x = rotationCenter.x + distanceToCenter * cos(newAngle);
  rotatedPoint.y = rotationCenter.y + distanceToCenter * sin(newAngle);
}

void thred::rotflt(fPOINT& point, float const rotationAngle, fPOINT const& rotationCenter) noexcept {
  auto       len      = 0.0F;
  auto       newAngle = 0.0F;
  auto const dx       = point.x - rotationCenter.x;
  auto const dy       = point.y - rotationCenter.y;

  if (dx != 0.0F) {
	len      = hypot(dx, dy);
	newAngle = atan2(dy, dx);
	newAngle += rotationAngle;
  }
  else {
	if (dy > 0) {
	  len      = dy;
	  newAngle = PI_F / 2.0F + rotationAngle;
	}
	else {
	  len      = -dy;
	  newAngle = rotationAngle - PI_F / 2.0F;
	}
  }
  point.x = rotationCenter.x + len * cos(newAngle);
  point.y = rotationCenter.y + len * sin(newAngle);
}

void thred::internal::rotstch(fPOINTATTR& stitch, float const rotationAngle, fPOINT const& rotationCenter) noexcept {
  auto       distanceToCenter = 0.0F;
  auto       newAngle         = 0.0F;
  auto const dx               = stitch.x - rotationCenter.x;
  auto const dy               = stitch.y - rotationCenter.y;

  if (dx != 0.0) {
	distanceToCenter = hypot(dx, dy);
	newAngle         = atan2(dy, dx);
	newAngle += rotationAngle;
  }
  else {
	if (dy > 0) {
	  distanceToCenter = dy;
	  newAngle         = PI_F / 2.0F + rotationAngle;
	}
	else {
	  distanceToCenter = -dy;
	  newAngle         = rotationAngle - PI_F / 2.0F;
	}
  }
  stitch.y = rotationCenter.y + distanceToCenter * sin(newAngle);
  stitch.x = rotationCenter.x + distanceToCenter * cos(newAngle);
}

void thred::internal::ritrot(float rotationAngle, fPOINT const& rotationCenter) {
  if (StateMap.test(StateFlag::MOVCNTR)) { // set rotation angle to 0 if we are moving the center point
	rotationAngle = 0.0F;
  }
  auto rotated           = POINT {0L, 0L};
  auto rotationReference = fPOINT {RotationRect.left, RotationRect.top};

  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[0] = RotateBoxOutline[4] = rotated;
  rotationReference.x                       = rotationCenter.x;
  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossVertLine[0] = rotated;
  rotationReference.x       = RotationRect.right;
  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[1] = rotated;
  rotationReference.y = rotationCenter.y;
  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossHorzLine[1] = rotated;
  rotationReference.y       = RotationRect.bottom;
  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[2] = rotated;
  rotationReference.x = rotationCenter.x;
  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossVertLine[1] = rotated;
  rotationReference.x       = RotationRect.left;
  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[3] = rotated;
  rotationReference.y = rotationCenter.y;
  rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossHorzLine[0] = rotated;
  thred::sCor2px(rotationCenter, RotateBoxToCursorLine[0]);
  StateMap.set(StateFlag::ROTSHO);
  durot();
}

void thred::internal::rot(fPOINT& rotationCenter) {
  do {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  break;
	}
	if (!SelectedFormList->empty()) {
	  break;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
	  break;
	}
	if (StateMap.test(StateFlag::GRPSEL)) {
	  break;
	}
	if (StateMap.test(StateFlag::FPSEL)) {
	  break;
	}
	// no rotatable selections found
	displayText::shoseln(IDS_FGRPF, IDS_ROT);
	return;
  } while (false);
  StateMap.set(StateFlag::ROTAT);
  rotationCenter = form::rotpar();
  ritrot(0, rotationCenter);
}

void thred::internal::savclp(uint32_t destination, std::vector<fPOINTATTR>& buffer, uint32_t source) {
  auto integer = 0.0;

  ClipStitchData[destination].led = buffer[source].attribute & COLMSK;
  auto fractional = modf(gsl::narrow_cast<double>(buffer[source].x) - LowerLeftStitch.x, &integer);
  ClipStitchData[destination].fx   = wrap::floor<uint8_t>(fractional * 256.0);
  ClipStitchData[destination].x    = gsl::narrow<uint16_t>(integer);
  ClipStitchData[destination].spcx = 0;
  fractional = modf(gsl::narrow_cast<double>(buffer[source].y) - LowerLeftStitch.y, &integer);
  ClipStitchData[destination].fy   = wrap::floor<uint8_t>(fractional * 256.0);
  ClipStitchData[destination].y    = gsl::narrow<uint16_t>(integer);
  ClipStitchData[destination].spcy = 0;
  // ToDo - Are these structure members needed?
  ClipStitchData[destination].myst = 1;
  ClipStitchData[destination].tag  = 0x14;
}

void thred::rtclpfn(uint32_t destination, uint32_t source) {
  auto        integer    = 0.0F;
  auto const& clipBuffer = *ClipBuffer;

  ClipStitchData[destination].led  = 0;
  auto fractional                  = modf(clipBuffer[source].x - LowerLeftStitch.x, &integer);
  ClipStitchData[destination].fx   = wrap::floor<uint8_t>(fractional * 256.0F);
  ClipStitchData[destination].x    = gsl::narrow<uint16_t>(integer);
  ClipStitchData[destination].spcx = 0;
  fractional                       = modf(clipBuffer[source].y - LowerLeftStitch.y, &integer);
  ClipStitchData[destination].fy   = wrap::floor<uint8_t>(fractional * 256.0F);
  ClipStitchData[destination].y    = gsl::narrow<uint16_t>(integer);
  ClipStitchData[destination].spcy = 0;
  // ToDo - Are these structure members needed?
  ClipStitchData[destination].myst = 1;
  ClipStitchData[destination].tag  = 0x14;
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) auto thred::internal::sizfclp(FRMHED const& form) -> uint32_t {
  auto clipSize =
      gsl::narrow<uint32_t>(sizeof(FORMCLIP) + form.vertexCount * sizeof(decltype(FormVertices->back())));
  if (form.type == SAT) {
	clipSize += form.satinGuideCount * sizeof(decltype(SatinGuides->back()));
  }
  if (clip::iseclp(form)) {
	clipSize += form.clipEntries * sizeof(decltype(ClipPoints->back()));
  }
  if (clip::isclpx(form)) {
	clipSize += form.lengthOrCount.clipCount * sizeof(decltype(ClipPoints->back()));
  }
  if (texture::istx(form)) {
	clipSize += form.fillInfo.texture.count * sizeof(decltype(TexturePointsBuffer->back()));
  }
  return clipSize;
}

auto thred::internal::frmcnt(uint32_t iForm, uint32_t& formFirstStitchIndex) -> uint32_t {
  auto const codedAttribute = iForm << FRMSHFT;

  auto iStitch     = StitchBuffer->begin();
  auto stitchCount = 0U;
  auto flag        = true;
  for (; iStitch != StitchBuffer->end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && ((iStitch->attribute & TYPMSK) != 0U)) {
	  flag = false;
	  break;
	}
	stitchCount++;
  }
  if (flag) {
	return 0;
  }

  formFirstStitchIndex = stitchCount;
  stitchCount          = 0U;
  LowerLeftStitch      = fPOINT {1e20F, 1e20F};
  for (; iStitch != StitchBuffer->end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && ((iStitch->attribute & TYPMSK) != 0U)) {
	  if (iStitch->x < LowerLeftStitch.x) {
		LowerLeftStitch.x = iStitch->x;
	  }
	  if (iStitch->y < LowerLeftStitch.y) {
		LowerLeftStitch.y = iStitch->y;
	  }
	  stitchCount++;
	}
  }
  return stitchCount;
}

auto thred::internal::sizclp(FRMHED const& form, uint32_t& formFirstStitchIndex, uint32_t& formStitchCount)
    -> uint32_t {
  FileSize    = sizeof(FORMCLIP) + form.vertexCount * sizeof(decltype(FormVertices->back()));
  auto length = FileSize;
  if (form.type == SAT) {
	FileSize += form.satinGuideCount * sizeof(decltype(SatinGuides->back()));
  }
  if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	formStitchCount = frmcnt(ClosestFormToCursor, formFirstStitchIndex);
	length += formStitchCount;
	FileSize += length * sizeof(StitchBuffer->front());
  }
  if (clip::iseclp(form)) {
	FileSize += form.clipEntries * sizeof(decltype(ClipPoints->back()));
  }
  if (clip::isclpx(form)) {
	FileSize += form.lengthOrCount.clipCount * sizeof(decltype(ClipPoints->back()));
  }
  if (texture::istx(form)) {
	FileSize += form.fillInfo.texture.count * sizeof(decltype(TexturePointsBuffer->back()));
  }

  return length;
}

void thred::internal::duclip() {
  if (StateMap.test(StateFlag::FPSEL)) {
	if (OpenClipboard(ThrEdWindow) != 0) {
	  EmptyClipboard();
	  ThrEdClip        = RegisterClipboardFormat(ThrEdClipFormat);
	  ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, // NOLINT
	                                 (wrap::toSize(SelectedFormVertices.vertexCount) + 1U) * sizeof(fPOINT) +
	                                     sizeof(FORMVERTEXCLIP));
	  GSL_SUPPRESS(26429) {
		if (ThrEdClipPointer != nullptr) {
		  auto* clipHeader        = *(gsl::narrow_cast<FORMVERTEXCLIP**>(ThrEdClipPointer));
		  clipHeader->clipType    = CLP_FRMPS;
		  clipHeader->vertexCount = SelectedFormVertices.vertexCount;
		  clipHeader->direction   = StateMap.test(StateFlag::PSELDIR);
		  // skip past the header
		  auto*       vertices = convert_ptr<fPOINT*>(&clipHeader[1]);
		  auto const& form     = FormList->operator[](ClosestFormToCursor);
		  auto vertexIt        = std::next(FormVertices->cbegin(), form.vertexIndex);
		  auto iSource         = SelectedFormVertices.start;
		  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertices[iVertex] = vertexIt[iSource];
			iSource           = form::pdir(form, iSource);
		  }
		  SetClipboardData(ThrEdClip, ThrEdClipPointer);
		}
		else {
		  throw;
		}
	  }
	  CloseClipboard();
	}
	return;
  }
  if (StateMap.test(StateFlag::BIGBOX)) {
	displayText::tabmsg(IDS_INSF);
  }
  else {
	if (OpenClipboard(ThrEdWindow) != 0) {
	  EmptyClipboard();
	  ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
	  if (!SelectedFormList->empty()) {
		auto msiz   = 0U;
		auto length = 0U;
		for (auto& selectedForm : (*SelectedFormList)) {
		  ClosestFormToCursor = selectedForm;
		  auto& currentForm   = FormList->operator[](selectedForm);
		  length += sizfclp(currentForm);
		  msiz += FileSize;
		}
		ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, wrap::toSize(msiz) + length); // NOLINT
		GSL_SUPPRESS(26429) {
		  if (ThrEdClipPointer != nullptr) {
			auto clipFormsHeader       = *(gsl::narrow_cast<FORMSCLIP**>(ThrEdClipPointer));
			clipFormsHeader->clipType  = CLP_FRMS;
			clipFormsHeader->formCount = gsl::narrow<uint16_t>(SelectedFormList->size());
			// Skip past the header
			auto forms = convert_ptr<FRMHED*>(&clipFormsHeader[1]);
			auto iForm = 0U;
			for (auto& selectedForm : (*SelectedFormList)) {
			  auto& currentForm = FormList->operator[](selectedForm);
			  forms[iForm++]    = currentForm;
			}
			// skip past the forms
			auto formVertices = convert_ptr<fPOINT*>(&forms[iForm]);
			auto iVertex      = 0U;
			for (auto& selectedForm : (*SelectedFormList)) {
			  auto& form                      = FormList->operator[](selectedForm);
			  auto                   vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
			  for (auto iSide = 0U; iSide < form.vertexCount; iSide++) {
				formVertices[iVertex++] = vertexIt[iSide];
			  }
			}
			// skip past the vertex list
			auto guides     = convert_ptr<SATCON*>(&formVertices[iVertex]);
			auto guideCount = 0U;
			for (auto& selectedForm : (*SelectedFormList)) {
			  auto& form = FormList->operator[](selectedForm);
			  if (form.type == SAT) {
				auto guideIt = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
				for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
				  guides[guideCount++] = guideIt[iGuide];
				}
			  }
			}
			// skip past the guides
			auto points     = convert_ptr<fPOINT*>(&guides[guideCount]);
			auto pointCount = 0;
			for (auto& selectedForm : (*SelectedFormList)) {
			  auto& form = FormList->operator[](selectedForm);
			  if (clip::isclpx(form)) {
				auto offsetStart = std::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
				for (auto iClip = 0U; iClip < form.lengthOrCount.clipCount; iClip++) {
				  points[pointCount++] = *offsetStart;
				  offsetStart++;
				}
			  }
			  if (clip::iseclp(form)) {
				auto offsetStart = std::next(ClipPoints->cbegin(), form.borderClipData);
				for (auto iClip = 0U; iClip < form.clipEntries; iClip++) {
				  points[pointCount++] = *offsetStart;
				  offsetStart++;
				}
			  }
			}
			// Skip past the points
			auto textures     = convert_ptr<TXPNT*>(&points[pointCount]);
			auto textureCount = 0;
			iForm             = 0;
			for (auto& selectedForm : (*SelectedFormList)) {
			  auto& form = FormList->operator[](selectedForm);
			  if (texture::istx(selectedForm)) {
				auto startPoint = std::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
				auto       endPoint = std::next(startPoint, form.fillInfo.texture.count);
				auto const dest = gsl::span<TXPNT>(&textures[textureCount], form.fillInfo.texture.count);
				std::copy(startPoint, endPoint, dest.begin());
				forms[iForm++].fillInfo.texture.index = gsl::narrow<uint16_t>(textureCount);
				textureCount += form.fillInfo.texture.count;
			  }
			}
			SetClipboardData(ThrEdClip, ThrEdClipPointer);
		  }
		}
		CloseClipboard();
		auto formMap = boost::dynamic_bitset<>(FormList->size());
		for (auto& selectedForm : (*SelectedFormList)) {
		  formMap.set(selectedForm);
		}
		auto astch = std::vector<fPOINTATTR> {};
		// Reserve a reasonable amount space, probably not enough though
		astch.reserve(StitchBuffer->size() / 16U);
		LowerLeftStitch.x = LowerLeftStitch.y = 1e30F;
		for (auto& stitch : *StitchBuffer) {
		  if (((stitch.attribute & NOTFRM) == 0U) && formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
			if (stitch.x < LowerLeftStitch.x) {
			  LowerLeftStitch.x = stitch.x;
			}
			if (stitch.y < LowerLeftStitch.y) {
			  LowerLeftStitch.y = stitch.y;
			}
			astch.push_back(stitch);
		  }
		}
		auto const stitchCount = wrap::toUnsigned(astch.size());
		if ((!StitchBuffer->empty()) && (stitchCount != 0)) {
		  Clip = RegisterClipboardFormat(PcdClipFormat);
		  ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2U); // NOLINT
		  if (ClipPointer != nullptr) {
			ClipStitchData    = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
			auto iStitch      = 0U;
			auto iDestination = 0U;
			savclp(0, astch, 0);
			iStitch++;
			ClipStitchData[0].led = stitchCount;
			iDestination++;
			while (iStitch < stitchCount) {
			  savclp(iDestination++, astch, iStitch++);
			}
			SetClipboardData(Clip, ClipPointer);
		  }
		  CloseClipboard();
		}
	  }
	  else {
		if (StateMap.test(StateFlag::FORMSEL)) {
		  auto  firstStitch             = 0U; // points to the first stitch in a form
		  auto  stitchCount             = 0U;
		  auto& form                    = FormList->operator[](ClosestFormToCursor);
		  auto const             length = sizclp(form, firstStitch, stitchCount);
		  FileSize += sizeof(FORMCLIP);
		  ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, FileSize); // NOLINT
		  GSL_SUPPRESS(26429) {
			if (ThrEdClipPointer != nullptr) {
			  auto clipFormHeader      = *(gsl::narrow_cast<FORMCLIP**>(ThrEdClipPointer));
			  clipFormHeader->clipType = CLP_FRM;
			  clipFormHeader->form     = form;
			  auto formVertices        = convert_ptr<fPOINT*>(&clipFormHeader[1]);
			  auto vertexIt            = std::next(FormVertices->cbegin(), form.vertexIndex);
			  for (auto iSide = 0U; iSide < form.vertexCount; iSide++) {
				formVertices[iSide] = vertexIt[iSide];
			  }
			  auto guides = convert_ptr<SATCON*>(&formVertices[form.vertexCount]);
			  auto iGuide = 0U;
			  if (form.type == SAT) {
				auto guideIt = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
				for (iGuide = 0; iGuide < form.satinGuideCount; iGuide++) {
				  guides[iGuide] = guideIt[iGuide];
				}
			  }
			  auto mclp  = convert_ptr<fPOINT*>(&guides[iGuide]);
			  auto iClip = 0U;
			  if (clip::isclpx(ClosestFormToCursor)) {
				auto offsetStart = std::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
				for (iClip = 0; iClip < form.lengthOrCount.clipCount; iClip++) {
				  mclp[iClip] = *offsetStart;
				  offsetStart++;
				}
			  }
			  auto points = convert_ptr<fPOINT*>(&mclp[iClip]);
			  if (clip::iseclpx(ClosestFormToCursor)) {
				auto offsetStart = std::next(ClipPoints->cbegin(), form.borderClipData);
				for (iClip = 0; iClip < form.clipEntries; iClip++) {
				  points[iClip] = *offsetStart;
				  offsetStart++;
				}
			  }
			  auto textures = convert_ptr<TXPNT*>(&points[iClip]);
			  if (texture::istx(ClosestFormToCursor)) {
				auto startPoint = std::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
				auto       endPoint = std::next(startPoint, form.fillInfo.texture.count);
				auto const dest     = gsl::span<TXPNT>(textures, form.fillInfo.texture.count);
				std::copy(startPoint, endPoint, dest.begin());
			  }
			  SetClipboardData(ThrEdClip, ThrEdClipPointer);
			}
			if (((form.fillType != 0U) || (form.edgeType != 0U))) {
			  Clip = RegisterClipboardFormat(PcdClipFormat);
			  ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2U); // NOLINT
			  if (ClipPointer != nullptr) {
				ClipStitchData = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
				auto iTexture  = firstStitch;
				savclp(0, *StitchBuffer, iTexture);
				ClipStitchData[0].led = length;
				iTexture++;
				auto iDestination   = 1U;
				auto codedAttribute = gsl::narrow<uint32_t>(ClosestFormToCursor << FRMSHFT);
				while (iTexture < StitchBuffer->size()) {
				  if ((StitchBuffer->operator[](iTexture).attribute & FRMSK) == codedAttribute &&
				      ((StitchBuffer->operator[](iTexture).attribute & NOTFRM) == 0U)) {
					savclp(iDestination++, *StitchBuffer, iTexture);
				  }
				  iTexture++;
				}
				SetClipboardData(Clip, ClipPointer);
			  }
			  form::ispcdclp();
			}
		  }
		  CloseClipboard();
		}
		else {
		  if ((!StitchBuffer->empty()) && StateMap.test(StateFlag::GRPSEL)) {
			Clip = RegisterClipboardFormat(PcdClipFormat);
			thred::rngadj();
			LowerLeftStitch.x = LowerLeftStitch.y = 1e30F;
			for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
			  auto const stitch = StitchBuffer->operator[](iStitch);
			  if (stitch.x < LowerLeftStitch.x) {
				LowerLeftStitch.x = stitch.x;
			  }
			  if (stitch.y < LowerLeftStitch.y) {
				LowerLeftStitch.y = stitch.y;
			  }
			}
			auto const length  = GroupEndStitch - GroupStartStitch + 1U;
			auto       iSource = GroupStartStitch;
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length * sizeof(CLPSTCH) + 2U); // NOLINT
			if (ClipPointer != nullptr) {
			  ClipStitchData = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
			  savclp(0, *StitchBuffer, iSource);
			  ClipStitchData[0].led = length;
			  iSource++;
			  for (auto iStitch = 1U; iStitch < length; iStitch++) {
				savclp(iStitch, *StitchBuffer, iSource++);
			  }
			  SetClipboardData(Clip, ClipPointer);
			}
			CloseClipboard();
		  }
		}
	  }
	  CloseClipboard();
	}
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void thred::delfstchs() {
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  // find the first stitch to delete
  auto firstStitch =
      std::find_if(StitchBuffer->begin(), StitchBuffer->end(), [codedForm](fPOINTATTR const& m) -> bool {
	    return ((m.attribute & (FRMSK | NOTFRM)) == codedForm);
      });
  if (firstStitch != StitchBuffer->end()) {
	// we found the first stitch, so now delete the stitches in the form
	StitchBuffer->erase(std::remove_if(firstStitch,
	                                   StitchBuffer->end(),
	                                   [codedForm](fPOINTATTR const& m) -> bool {
	                                     return (m.attribute & FRMSK) == codedForm;
	                                   }),
	                    StitchBuffer->end());
  }
}

void thred::internal::f1del(uint32_t formIndex) {
  if (StateMap.test(StateFlag::DELTO)) {
	auto const codedForm = formIndex << FRMSHFT;
	StitchBuffer->erase(std::remove_if(StitchBuffer->begin(),
	                                   StitchBuffer->end(),
	                                   [codedForm](fPOINTATTR const& m) -> bool {
	                                     return (((m.attribute & NOTFRM) == 0U) &&
	                                             ((m.attribute & FRMSK) == codedForm));
	                                   }),
	                    StitchBuffer->end());
  }
  clip::deleclp(formIndex);
  clip::delmclp(formIndex);
  satin::delsac(formIndex);
  form::delflt(formIndex);
  texture::deltx(formIndex);
}

void thred::frmdel() {
  thi::f1del(ClosestFormToCursor);
  auto firstForm = FormList->cbegin();
  FormList->erase(std::next(firstForm, ClosestFormToCursor));
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  if (StateMap.testAndReset(StateFlag::DELTO)) {
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & NOTFRM) == 0U) {
		auto const stitchForm = (stitch.attribute & FRMSK);
		if (stitchForm > codedForm) {
		  stitch.attribute &= NFRMSK;
		  stitch.attribute |= (stitchForm - (1U << FRMSHFT));
		}
	  }
	}
  }
  else {
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & NOTFRM) == 0U) {
		auto const stitchForm = (stitch.attribute & FRMSK);
		if (stitchForm == codedForm) {
		  stitch.attribute &= (NFRMSK & NTYPMSK);
		  stitch.attribute |= NOTFRM;
		}
		if (stitchForm > codedForm) {
		  stitch.attribute &= NFRMSK;
		  stitch.attribute |= (stitchForm - (1U << FRMSHFT));
		}
	  }
	}
  }
  StateMap.reset(StateFlag::FORMSEL);
}

void thred::internal::delsfrms(uint32_t code) {
  if (code != 0U) {
	if (!FormList->empty()) {
	  auto formIndices = std::vector<uint32_t> {};
	  formIndices.resize(FormList->size());
	  auto formMap = boost::dynamic_bitset<>(FormList->size());
	  for (auto selectedForm : (*SelectedFormList)) {
		ClosestFormToCursor = selectedForm;
		formMap.set(ClosestFormToCursor);
		f1del(selectedForm);
	  }
	  auto       deletedFormCount = 0U;
	  auto       firstForm        = FormList->cbegin();
	  auto const formCount        = FormList->size();
	  for (auto iForm = 0U; iForm < formCount; iForm++) {
		if (!formMap.test(iForm)) {
		  formIndices[iForm] = (iForm - deletedFormCount) << FRMSHFT;
		}
		else {
		  FormList->erase(std::next(firstForm, (iForm - deletedFormCount)));
		  firstForm = FormList->cbegin(); // the erase invalidates firstForm
		  deletedFormCount++;
		}
	  }
	  if (StateMap.test(StateFlag::DELTO)) {
		for (auto& stitch : *StitchBuffer) {
		  if ((stitch.attribute & ALTYPMSK) != 0U) {
			auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT;
			if (!formMap.test(iForm)) {
			  stitch.attribute &= NFRMSK;
			  stitch.attribute |= formIndices[iForm];
			}
		  }
		}
	  }
	  else {
		for (auto& stitch : *StitchBuffer) {
		  if ((stitch.attribute & NOTFRM) == 0U) {
			auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT;
			if (formMap.test(iForm)) {
			  stitch.attribute &= (NFRMSK & NTYPMSK);
			  stitch.attribute |= NOTFRM;
			}
			else {
			  stitch.attribute &= NFRMSK;
			  stitch.attribute |= formIndices[iForm];
			}
		  }
		}
	  }
	  SelectedFormList->clear();
	  StateMap.reset(StateFlag::FORMSEL);
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::cut() {
  thred::savdo();
  duclip();
  if (!SelectedFormList->empty()) {
	StateMap.set(StateFlag::DELTO);
	delsfrms('s');
  }
  else {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  thred::delfstchs();
	  thred::frmdel();
	}
	else {
	  if (StateMap.test(StateFlag::GRPSEL)) {
		thred::delstchm();
	  }
	}
  }
  thred::coltab();
  rstAll();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::unpat() {
  if (StateMap.testAndReset(StateFlag::WASPAT)) {
	ShowWindow(SpeedScrollBar, FALSE);
	DestroyWindow(SpeedScrollBar);
	thred::movStch();
	StateMap.set(StateFlag::RESTCH);
  }
}
auto thred::internal::cmpstch(uint32_t iStitchA, uint32_t iStitchB) noexcept -> bool {
  if (StitchBuffer->operator[](iStitchA).x != StitchBuffer->operator[](iStitchB).x) {
	return false;
  }

  return StitchBuffer->operator[](iStitchA).y == StitchBuffer->operator[](iStitchB).y;
}

void thred::internal::ofstch(std::vector<fPOINTATTR>& buffer, uint32_t iSource, char offset) {
  buffer.emplace_back(
      fPOINTATTR {StitchBuffer->operator[](iSource).x + KnotStep.x * gsl::narrow_cast<float>(offset),
                  StitchBuffer->operator[](iSource).y + KnotStep.y * gsl::narrow_cast<float>(offset),
                  KnotAttribute});
}

void thred::internal::endknt(std::vector<fPOINTATTR>& buffer, uint32_t finish) {
  auto length = 0.0F;
  auto iStart = finish - 1U;
  if (finish == 0U) {
	iStart++;
  }

  KnotAttribute = StitchBuffer->operator[](iStart).attribute | KNOTMSK;
  do {
	auto const delta = fPOINT {StitchBuffer->operator[](finish).x - StitchBuffer->operator[](iStart).x,
	                           StitchBuffer->operator[](finish).y - StitchBuffer->operator[](iStart).y};

	length = hypot(delta.x, delta.y);
	iStart--;
  } while (length == 0.0F);
  auto const* knots =
      (StateMap.test(StateFlag::FILDIR)) ? std::begin(KnotAtLastOrder) : std::begin(KnotAtEndOrder);
  if (knots != nullptr) {
	if ((iStart & 0x8000000U) == 0U) {
	  auto const delta =
	      fPOINT {StitchBuffer->operator[](finish).x - StitchBuffer->operator[](iStart).x,
	              StitchBuffer->operator[](finish).y - StitchBuffer->operator[](iStart).y};
	  KnotStep.x = 2.0F / length * delta.x;
	  KnotStep.y = 2.0F / length * delta.y;
	  for (auto iKnot = 0U; iKnot < 5U; iKnot++) {
		ofstch(buffer, finish, knots[iKnot]);
	  }
	  if (StateMap.test(StateFlag::FILDIR)) {
		ofstch(buffer, finish, 0);
	  }
	}
  }
}

void thred::internal::strtknt(std::vector<fPOINTATTR>& buffer, uint32_t start) {
  auto length = 0.0F;
  auto finish = start + 1U;

  do {
	auto const delta = fPOINT {StitchBuffer->operator[](finish).x - StitchBuffer->operator[](start).x,
	                           StitchBuffer->operator[](finish).y - StitchBuffer->operator[](start).y};

	length = hypot(delta.x, delta.y);
	finish++;
  } while (length < 2.0F && finish < wrap::toUnsigned(StitchBuffer->size()));
  if (finish < wrap::toUnsigned(StitchBuffer->size())) {
	auto const delta = fPOINT {StitchBuffer->operator[](finish).x - StitchBuffer->operator[](start).x,
	                           StitchBuffer->operator[](finish).y - StitchBuffer->operator[](start).y};

	KnotAttribute = StitchBuffer->operator[](start).attribute | KNOTMSK;
	KnotStep.x    = 2.0F / length * delta.x;
	KnotStep.y    = 2.0F / length * delta.y;
	for (char const iKnot : KnotAtStartOrder) {
	  ofstch(buffer, start, iKnot);
	}
  }
}

void thred::internal::delknt() {
  // delete the knot stitches
  StitchBuffer->erase(
      std::remove_if(StitchBuffer->begin(),
                     StitchBuffer->end(),
                     [](fPOINTATTR const& m) -> bool { return (m.attribute & KNOTMSK) != 0U; }),
      StitchBuffer->end());
}

void thred::internal::delknot() {
  // Find the first knot stitch, if any
  auto firstStitch = std::find_if(StitchBuffer->begin(), StitchBuffer->end(), [](fPOINTATTR const& m) -> bool {
	return ((m.attribute & KNOTMSK) != 0U);
  });
  if (firstStitch != StitchBuffer->end()) {
	thred::savdo();
	// delete the knot stitches
	StitchBuffer->erase(
	    std::remove_if(firstStitch,
	                   StitchBuffer->end(),
	                   [](fPOINTATTR const& m) -> bool { return (m.attribute & KNOTMSK) != 0U; }),
	    StitchBuffer->end());
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
}

auto thred::internal::kjmp(std::vector<fPOINTATTR>& buffer, uint32_t start) -> uint32_t {
  while (start < gsl::narrow<decltype(start)>(StitchBuffer->size() - 1U) && stlen(start) > KNOTLEN) {
	buffer.push_back(StitchBuffer->operator[](start++));
  }
  strtknt(buffer, start);
  return start;
}

void thred::internal::setknt() {
  auto iStitch = 0U;

  // ToDo - Use a temp buffer rather than the high buffer
  auto buffer = std::vector<fPOINTATTR> {};
  buffer.reserve(StitchBuffer->size());
  OutputIndex = 0;
  buffer.push_back(StitchBuffer->front());
  strtknt(buffer, 0);
  if (stlen(0) > KNOTLEN) {
	iStitch = kjmp(buffer, 1);
  }
  else {
	iStitch = 1;
  }
  StateMap.reset(StateFlag::FILDIR);
  while (iStitch < wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	buffer.push_back(StitchBuffer->operator[](iStitch));
	if (stlen(iStitch) > KNOTLEN) {
	  endknt(buffer, iStitch);
	  iStitch = kjmp(buffer, iStitch + 1U);
	  buffer.push_back(StitchBuffer->operator[](iStitch));
	}
	iStitch++;
  }
  StateMap.set(StateFlag::FILDIR);
  endknt(buffer, iStitch);
  buffer.back().attribute &= (~KNOTMSK);
  StitchBuffer->resize(buffer.size());
  *StitchBuffer = buffer;
}

auto thred::internal::srchknot(uint32_t source) noexcept -> uint32_t {
  auto knotIndex = 0U;
  while (knotIndex < KnotCount && Knots[knotIndex] < source) {
	knotIndex++;
  }
  if (knotIndex != 0U) {
	knotIndex--;
  }
  if (((Knots[knotIndex] > source) ? (Knots[knotIndex] - source) : (source - Knots[knotIndex])) < 5U) {
	knotIndex++;
	if (((Knots[knotIndex] > source) ? (Knots[knotIndex] - source) : (source - Knots[knotIndex])) < 5U) {
	  return 0;
	}
	return 2;
  }

  knotIndex++;
  if (((Knots[knotIndex] > source) ? (Knots[knotIndex] - source) : (source - Knots[knotIndex])) < 5U) {
	return 1;
  }
  return 3;
}

void thred::internal::chkncol() {
  auto initialColor = StitchBuffer->front().attribute & COLMSK;

  // ToDo - Use a temp buffer rather than the high buffer
  auto buffer = std::vector<fPOINTATTR> {};
  buffer.reserve(StitchBuffer->size());
  OutputIndex = 0;
  StateMap.reset(StateFlag::FILDIR);
  auto iStitch = 0U;
  for (auto& stitch : *StitchBuffer) {
	auto const color = stitch.attribute & COLMSK;
	if (color == initialColor) {
	  buffer.push_back(stitch);
	}
	else {
	  initialColor    = stitch.attribute & COLMSK;
	  auto const code = srchknot(iStitch);
	  if ((code & 1U) != 0U) {
		endknt(buffer, iStitch - 1U);
	  }
	  buffer.push_back(stitch);
	  if ((code & 2U) != 0U) {
		strtknt(buffer, iStitch);
	  }
	}
	iStitch++;
  }
  StitchBuffer->resize(buffer.size());
  *StitchBuffer = buffer;
}

void thred::internal::setknots() {
  if (!StitchBuffer->empty()) {
	thred::savdo();
	delknt();
	setknt();
	fndknt();
	chkncol();
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
	thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  }
}

void thred::internal::lodbmp() {
  if (PCSBMPFileName[0] != 0) {
	DeleteObject(BitmapFileHandle);
	ReleaseDC(ThrEdWindow, BitmapDC);
  }
  auto fileName = std::vector<wchar_t> {};
  fileName.resize(_MAX_PATH);
  auto dirBuffer = std::vector<wchar_t> {};
  dirBuffer.resize(_MAX_PATH);
  auto workingFileStr = UserBMPFileName->wstring();
  auto dirStr         = DefaultBMPDirectory->wstring();
  std::copy(workingFileStr.cbegin(), workingFileStr.cend(), fileName.begin());
  std::copy(dirStr.cbegin(), dirStr.cend(), dirBuffer.begin());
  OpenBitmapName.lpstrFile       = fileName.data();
  OpenBitmapName.lpstrInitialDir = dirBuffer.data();
  if (GetOpenFileName(&OpenBitmapName)) {
	fileName.resize(wcslen(fileName.data()));
	UserBMPFileName->assign(fileName.cbegin(), fileName.cend());
	trace::untrace();
	auto saveFile = utf::Utf16ToUtf8(UserBMPFileName->filename().wstring());
	// PCS file can only store a 16 character filename?
	// ToDo - give the user a little more info that the bitmap has not been loaded
	if (!saveFile.empty() && saveFile.size() < 16U) {
	  auto const bmpName = gsl::span<char> {PCSBMPFileName};
	  std::copy(saveFile.cbegin(), saveFile.cend(), bmpName.begin());
	  defbNam();
	  bfil();
	}
	else {
	}
	StateMap.set(StateFlag::RESTCH);
  }
}

void thred::internal::duhbit(uint32_t cod) noexcept {
  CheckMenuItem(MainMenu, ID_HIDBIT, MF_BYCOMMAND | cod);  // NOLINT
  CheckMenuItem(MainMenu, ID_HIDBITF, MF_BYCOMMAND | cod); // NOLINT
}

void thred::hidbit() {
  if (StateMap.testAndFlip(StateFlag::HIDMAP)) {
	thi::duhbit(MF_UNCHECKED);
  }
  else {
	thi::duhbit(MF_CHECKED);
  }
  StateMap.set(StateFlag::DUMEN);
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::drwlstch(uint32_t finish) {
  auto color = 0U;

  if (StateMap.test(StateFlag::HID)) {
	while (RunPoint < (finish - 1) && (StitchBuffer->operator[](RunPoint).attribute & COLMSK) != ActiveColor) {
	  RunPoint++;
	}
  }
  if (StateMap.test(StateFlag::ZUMED)) {
	auto iMovieFrame = 1U;
	while (RunPoint < StitchesPerFrame + 1 && RunPoint < finish - 2 && !stch2px(RunPoint)) {
	  RunPoint++;
	}
	auto const origin = RunPoint - 1U;

	color = StitchBuffer->operator[](RunPoint).attribute & COLMSK;

	auto flag = true;
	while (iMovieFrame < StitchesPerFrame + 1 && RunPoint < finish - 2 &&
	       (StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color) {
	  if (stch2px(RunPoint)) {
		MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
		if (flag) {
		  flag = false;
		  if (stch2px(RunPoint - 1)) {
			MovieLine[0] = MovieLine[1];
		  }
		  else {
			MovieLine[0] = StitchCoordinatesPixels;
		  }
		}
	  }
	  RunPoint++;
	}
	if (RunPoint == origin) {
	  RunPoint++;
	}
	if (!stch2px(RunPoint)) {
	  if ((StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color) {
		MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
		RunPoint++;
	  }
	}
	SelectObject(StitchWindowDC, UserPen[color]);
	wrap::Polyline(StitchWindowDC, static_cast<POINT const*>(MovieLine), iMovieFrame);
	if (!flag) {
	  RunPoint--;
	}
  }
  else {
	auto iMovieFrame = 0U;

	color = StitchBuffer->operator[](RunPoint).attribute & COLMSK;
	SelectObject(StitchWindowDC, UserPen[color]);
	while (iMovieFrame < StitchesPerFrame && (RunPoint + 1 < finish - 1) &&
	       ((StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color)) {
	  stch2px1(RunPoint++);
	  MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
	}
	RunPoint--;
	wrap::Polyline(StitchWindowDC, static_cast<POINT const*>(MovieLine), iMovieFrame);
  }
  if ((StitchBuffer->operator[](wrap::toSize(RunPoint) + 1U).attribute & 0xfU) != color) {
	RunPoint++;
  }
  displayText::ritnum(STR_NUMSEL, RunPoint);
  if (RunPoint + 3 > finish - 1) {
	patdun();
  }
}

void thred::internal::stchout() {
  if (StateMap.test(StateFlag::GRPSEL)) {
	drwlstch(GroupEndStitch);
  }
  else {
	drwlstch(wrap::toUnsigned(StitchBuffer->size() - 1U));
  }
}

void thred::internal::setsped() {
  auto elapsedTimePerFrame = 0U;

  if (MovieTimeStep == 0) {
	MovieTimeStep = 1;
  }
  auto const userTimePerFrame = gsl::narrow_cast<double>(MovieTimeStep) / 10.0;
  if (userTimePerFrame < 10.0) {
	elapsedTimePerFrame = 100U; // units are millseconds
	StitchesPerFrame    = wrap::round<uint32_t>(elapsedTimePerFrame / userTimePerFrame);
	if (StitchesPerFrame > 99U) {
	  StitchesPerFrame = 99U;
	}
  }
  else {
	elapsedTimePerFrame = wrap::round<uint32_t>(userTimePerFrame);
	StitchesPerFrame    = 2;
  }
  if (StitchesPerFrame < 2) {
	StitchesPerFrame = 2;
  }
  SetTimer(ThrEdWindow, 0, elapsedTimePerFrame, nullptr);
}

void thred::internal::deltot() {
  DesignerName->assign(utf::Utf8ToUtf16(std::string(std::begin(IniFile.designerName))));
  TexturePointsBuffer->clear();
  FormList->clear();
  StitchBuffer->clear();
  FormVertices->clear();
  SatinGuides->clear();
  StateMap.reset(StateFlag::GMRK);
  rstAll();
  thred::coltab();
  thred::zumhom();
  SetWindowText(ThrEdWindow,
                fmt::format(StringTable->operator[](STR_THRDBY), ThrName->wstring(), *DesignerName).c_str());
}

auto thred::internal::wastch() noexcept -> bool {
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
	  return true;
	}
  }
  return false;
}

auto thred::internal::frmstch() -> bool {
  auto formMap = boost::dynamic_bitset<>(FormList->size());

  for (auto form : (*SelectedFormList)) {
	formMap.set(form);
  }
  for (auto& stitch : *StitchBuffer) {
	if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
	  return true;
	}
  }
  return false;
}

void thred::internal::delet() {
  thred::undat();
  if (StateMap.testAndReset(StateFlag::FPSEL)) {
	thred::savdo();
	auto& form = FormList->operator[](ClosestFormToCursor);
	// dynamic bitset allows non-contiguous ranges of points to be deleted in later versions
	auto vertexMap         = boost::dynamic_bitset<>(form.vertexCount);
	auto currentFormVertex = SelectedFormVertices.start;
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
	  vertexMap.set(currentFormVertex);
	  currentFormVertex = form::pdir(form, currentFormVertex);
	}
	currentFormVertex = 0;
	auto vertexIt     = std::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	  if (!vertexMap.test(iVertex)) {
		vertexIt[currentFormVertex++] = vertexIt[iVertex];
	  }
	}
	auto eraseStart = std::next(FormVertices->cbegin(), wrap::toSize(form.vertexIndex) + currentFormVertex);
	auto eraseEnd = std::next(eraseStart, (form.vertexCount - currentFormVertex));
	FormVertices->erase(eraseStart, eraseEnd); // This invalidates iterators
	auto const nextForm = wrap::toSize(ClosestFormToCursor) + 1U;
	for (auto iForm = std::next(FormList->begin(), nextForm); iForm < FormList->end(); iForm++) {
	  iForm->vertexIndex -= SelectedFormVertices.vertexCount + 1U;
	}
	form.vertexCount -= (SelectedFormVertices.vertexCount + 1U);
	form::frmout(ClosestFormToCursor);
	if (form.type == SAT) {
	  // Make sure the end guides are still valid
	  if (vertexMap.test(0) || vertexMap.test(1)) {
		form.wordParam = 0; // Satin end guide
		form.attribute &= 0xfeU;
	  }
	  auto const iNext = form.wordParam + 1U;
	  if (vertexMap.test(form.wordParam) || vertexMap.test(iNext)) {
		form.wordParam = 0;
	  }

	  // ToDo - Is there a better way to do this than iterating through?
	  auto guideIt = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
		auto newGuideVal = 0U;
		for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		  if (vertexMap.test(iVertex)) {
			if (guideIt[iGuide].finish == iVertex) {
			  guideIt[iGuide].finish = guideIt[iGuide].start;
			}
			if (guideIt[iGuide].start == iVertex) {
			  guideIt[iGuide].start = guideIt[iGuide].finish;
			}
		  }
		  else {
			if (guideIt[iGuide].finish == iVertex) {
			  guideIt[iGuide].finish = newGuideVal;
			}
			if (guideIt[iGuide].start == iVertex) {
			  guideIt[iGuide].start = newGuideVal;
			}
			newGuideVal++;
		  }
		}
	  }
	  satin::satadj(form);
	}
	form::refil();
	fndknt();
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_SHIFT)) {
	deltot();
  }
  else {
	thred::savdo();
	auto satinFlag = false;
	if (!SelectedFormList->empty()) {
	  if (frmstch()) {
		StateMap.set(StateFlag::DELSFRMS);
		displayText::tabmsg(IDS_DELFRM);
		displayText::okcan();
		displayText::tomsg();
	  }
	  else {
		delsfrms(1);
	  }
	  return;
	}
	if (StateMap.test(StateFlag::FORMSEL) && !FormList->empty()) {
	  if (wastch()) {
		StateMap.set(StateFlag::DELFRM);
		displayText::tabmsg(IDS_FDEL);
		displayText::okcan();
		displayText::tomsg();
	  }
	  else {
		if (!FormList->empty()) {
		  StateMap.reset(StateFlag::DELTO);
		  thred::frmdel();
		  StateMap.set(StateFlag::RESTCH);
		}
	  }
	  fndknt();
	  return;
	}
	if (StateMap.test(StateFlag::SELBOX)) {
	  if (StitchBuffer->size() > 2U) {
		delstch1(ClosestPointIndex);
		if (!stch2px(ClosestPointIndex)) {
		  movbox();
		}
	  }
	  else {
		StitchBuffer->clear();
		StateMap.reset(StateFlag::SELBOX);
	  }
	  thred::coltab();
	  xt::setfchk();
	  fndknt();
	  StateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap.test(StateFlag::GRPSEL)) {
	  thred::delstchm();
	  thred::coltab();
	  rstAll();
	  xt::setfchk();
	  fndknt();
	  StateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap.test(StateFlag::FRMPSEL) || form::closfrm()) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  switch (form.type) {
		case FRMLINE: {
		  if (form.fillType == CONTF) {
			if (ClosestVertexToCursor == form.angleOrClipData.guide.start ||
			    ClosestVertexToCursor == form.angleOrClipData.guide.finish) {
			  form::delmfil(ClosestFormToCursor);
			  form.fillType = 0;
			  thred::coltab();
			  StateMap.set(StateFlag::RESTCH);
			  return;
			}
			if (form.angleOrClipData.guide.start > ClosestVertexToCursor) {
			  form.angleOrClipData.guide.start--;
			}
			if (form.angleOrClipData.guide.finish > ClosestVertexToCursor) {
			  form.angleOrClipData.guide.finish--;
			}
		  }
		  break;
		}
		case SAT: {
		  do {
			if (ClosestVertexToCursor <= 1) {
			  if ((form.attribute & FRMEND) != 0U) {
				if (form.wordParam != 0U) {
				  form.wordParam = 0;
				}
				else {
				  form.attribute &= 0xfeU;
				}
				satinFlag = true;
				break;
			  }
			}
			auto const& endGuide = form.wordParam;
			if (endGuide != 0U) {
			  if (ClosestVertexToCursor == gsl::narrow<uint32_t>(endGuide) ||
			      ClosestVertexToCursor == gsl::narrow<uint32_t>(endGuide) + 1U) {
				form.wordParam = 0;
				satinFlag      = true;
				break;
			  }
			}
			auto guideIt = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
			  if (guideIt[iGuide].start == ClosestVertexToCursor || guideIt[iGuide].finish == ClosestVertexToCursor) {
				satin::delcon(form, iGuide);
				satinFlag = true;
				break;
			  }
			}
		  } while (false);
		}
	  }
	  if (!satinFlag) {
		satin::delspnt();
	  }
	  if (ClosestFormToCursor > FormList->size() - 1U) {
		ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	  }
	  if (!FormList->empty()) {
		form::frmout(ClosestFormToCursor);
		form::refil();
	  }
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	}
	if (!satinFlag && closPnt1(&ClosestPointIndex)) {
	  if (StitchBuffer->size() > 2U) {
		delstch1(ClosestPointIndex);
		if (!stch2px(ClosestPointIndex)) {
		  movbox();
		}
	  }
	  else {
		StitchBuffer->clear();
		StateMap.reset(StateFlag::SELBOX);
	  }
	  thred::coltab();
	  xt::setfchk();
	  fndknt();
	  StateMap.set(StateFlag::RESTCH);
	  return;
	}
  }
  fndknt();
}

void thred::internal::movi() {
  if (StateMap.test(StateFlag::GRPSEL)) {
	rstAll();
	StateMap.set(StateFlag::GRPSEL);
  }
  else {
	rstAll();
  }
  if (!StitchBuffer->empty()) {
	if (MsgWindow != nullptr) {
	  DestroyWindow(MsgWindow);
	  MsgWindow = nullptr;
	}
	StateMap.set(StateFlag::RUNPAT);
	if (StateMap.test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  RunPoint = GroupStartStitch;
	}
	else {
	  RunPoint = 0;
	}
	thred::movStch();
	if (!StateMap.test(StateFlag::WASPAT)) {
	  SpeedScrollBar = CreateWindow(L"SCROLLBAR", // NOLINT
	                                nullptr,
	                                SBS_HORZ | WS_CHILD | WS_VISIBLE,
	                                ButtonWidthX3,
	                                0,
	                                StitchWindowSize.x,
	                                *ScrollSize,
	                                ThrEdWindow,
	                                nullptr,
	                                ThrEdInstance,
	                                nullptr);
	}
	auto stepCount = 0.0F;
	if (StateMap.test(StateFlag::ZUMED)) {
	  stepCount = gsl::narrow_cast<float>(StitchBuffer->size()) * ZoomFactor * ZoomFactor;
	}
	else {
	  stepCount = gsl::narrow_cast<float>(StitchBuffer->size());
	}
	if (!StateMap.test(StateFlag::WASPAT)) {
	  MovieTimeStep = wrap::round<decltype(MovieTimeStep)>(10000.0F * MOVITIM / stepCount);
	}
	if (MovieTimeStep < MINDELAY) {
	  MovieTimeStep = MINDELAY;
	}
	if (MovieTimeStep > MAXDELAY) {
	  MovieTimeStep = MAXDELAY;
	}
	ScrollInfo.cbSize = sizeof(ScrollInfo);
	ScrollInfo.fMask  = SIF_ALL; // NOLINT
	ScrollInfo.nMax   = MAXDELAY;
	ScrollInfo.nMin   = MINDELAY;
	ScrollInfo.nPage  = 1;
	ScrollInfo.nPos   = MAXDELAY - MovieTimeStep;
	SetScrollInfo(SpeedScrollBar, SB_CTL, &ScrollInfo, TRUE);
	FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
	setsped();
  }
}

void thred::redclp() {
  auto       clipRect   = fRECTANGLE {};
  auto const codedLayer = gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT);

  ClipPointer = GlobalLock(ClipMemory);
  if (ClipPointer != nullptr) {
	ClipStitchData        = gsl::narrow_cast<CLPSTCH*>(ClipPointer);
	auto const clipSize   = ClipStitchData[0].led;
	auto&      clipBuffer = *ClipBuffer;
	clipBuffer.clear();
	clipBuffer.reserve(clipSize);

	clipBuffer.emplace_back(
	    gsl::narrow_cast<float>(ClipStitchData[0].x) + gsl::narrow_cast<float>(ClipStitchData[0].fx) / 256.0F,
	    gsl::narrow_cast<float>(ClipStitchData[0].y) + gsl::narrow_cast<float>(ClipStitchData[0].fy) / 256.0F,
	    0U);

#if CLPBUG
	OutputDebugString(fmt::format(L"redclp:interator [0] x [{:6.2F}] y [{:6.2F}]\n",
	                              ClipBuffer[0].x,
	                              ClipBuffer[0].y)
	                      .c_str());
#endif
	clipRect.left = clipRect.right = clipBuffer[0].x;
	clipRect.bottom = clipRect.top = clipBuffer[0].y;
	for (auto iStitch = 1U; iStitch < clipSize; iStitch++) {
	  clipBuffer.emplace_back(gsl::narrow_cast<float>(ClipStitchData[iStitch].x) +
	                              gsl::narrow_cast<float>(ClipStitchData[iStitch].fx) / 256.0F,
	                          gsl::narrow_cast<float>(ClipStitchData[iStitch].y) +
	                              gsl::narrow_cast<float>(ClipStitchData[iStitch].fy) / 256.0F,
	                          (ClipStitchData[iStitch].led & 0xfU) | codedLayer);

#if CLPBUG
	  OutputDebugString(fmt::format(L"redclp:interator [{}] x [{:6.2F}] y [{:6.2F}]\n",
	                                iStitch,
	                                ClipBuffer[iStitch].x,
	                                ClipBuffer[iStitch].y)
	                        .c_str());
#endif
	  if (clipBuffer[iStitch].x < clipRect.left) {
		clipRect.left = clipBuffer[iStitch].x;
	  }
	  if (clipBuffer[iStitch].x > clipRect.right) {
		clipRect.right = clipBuffer[iStitch].x;
	  }
	  if (clipBuffer[iStitch].y < clipRect.bottom) {
		clipRect.bottom = clipBuffer[iStitch].y;
	  }
	  if (clipBuffer[iStitch].y > clipRect.top) {
		clipRect.top = clipBuffer[iStitch].y;
	  }
	}
	clipBuffer[0].attribute = ActiveColor | codedLayer;
	ClipRectSize            = {clipRect.right - clipRect.left, clipRect.top - clipRect.bottom};
	GlobalUnlock(ClipMemory);
	if ((clipRect.left != 0.0F) || (clipRect.bottom != 0.0F)) {
	  for (auto& clip : *ClipBuffer) {
		clip.x -= clipRect.left;
		clip.y -= clipRect.bottom;
	  }
	  clipRect.top -= clipRect.bottom;
	  clipRect.right -= clipRect.left;
	  clipRect.bottom = clipRect.left = 0;
	}
  }
}

constexpr auto thred::internal::nxtcrnr(uint32_t corner) -> uint32_t {
  return (corner + 1U) & 3U;
}

void thred::internal::drwmrk(HDC dc) {
  auto       markCoordinates = POINT {0L, 0L};
  POINT      markLine[2]     = {};
  auto const markOffset      = gsl::narrow<int32_t>(MulDiv(6, *screenDPI, 96));

  thred::sCor2px(fPOINT {ZoomMarkPoint}, markCoordinates);
  SelectObject(dc, ZoomMarkPen);
  SetROP2(dc, R2_XORPEN);
  markLine[0] = {markCoordinates.x - markOffset, markCoordinates.y - markOffset};
  markLine[1] = {markCoordinates.x + markOffset, markCoordinates.y + markOffset};
  Polyline(dc, static_cast<POINT const*>(markLine), 2);
  markLine[0] = {markCoordinates.x - markOffset, markCoordinates.y + markOffset};
  markLine[1] = {markCoordinates.x + markOffset, markCoordinates.y - markOffset};
  Polyline(dc, static_cast<POINT const*>(markLine), 2);
  SetROP2(dc, R2_COPYPEN);
}

void thred::internal::vubak() {
  if ((FileHandle != nullptr) || StateMap.test(StateFlag::THUMSHO)) {
	StateMap.set(StateFlag::ZUMED);
	thred::movStch();
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	auto const dx = StitchWindowClientRect.right / 2;
	auto const dy = StitchWindowClientRect.bottom / 2;
	for (auto iVersion = 0U; iVersion < 4; iVersion++) { // there are 4 quadrants
	  auto verticalLocation = 0L;
	  if ((iVersion & 2U) != 0U) {
		verticalLocation = dy;
	  }
	  BackupViewer[iVersion] = CreateWindow(L"STATIC", // NOLINT
	                                        L"",
	                                        SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                        dx * (iVersion & 1U) + ButtonWidthX3,
	                                        verticalLocation,
	                                        dx,
	                                        dy,
	                                        ThrEdWindow,
	                                        nullptr,
	                                        ThrEdInstance,
	                                        nullptr);
	}
	StateMap.set(StateFlag::BAKSHO);
  }
}

void thred::internal::insflin(POINT insertPoint) {
  auto const offset    = POINT {InsertSize.x / 2, InsertSize.y / 2};
  auto&      formLines = *FormLines;
  formLines.resize(5U);
  formLines[0].x = formLines[3].x = formLines[4].x = insertPoint.x - offset.x;
  formLines[1].x = formLines[2].x = insertPoint.x + offset.x;
  formLines[0].y = formLines[1].y = formLines[4].y = insertPoint.y - offset.y;
  formLines[2].y = formLines[3].y = insertPoint.y + offset.y;
}

auto thred::internal::isthr(wchar_t const* const filename) -> bool {
  auto thredPath = fs::path(filename);
  auto extention = thredPath.extension().wstring();

  return (extention.compare(0, 3, L".th") == 0);
}

auto thred::internal::gethand(std::vector<fPOINTATTR> const& stitch, uint32_t stitchCount) noexcept -> uint32_t {
  auto userStitchCount = 0U;
  for (auto iStitch = 0U; iStitch < stitchCount; iStitch++) {
	if ((stitch[iStitch].attribute & USMSK) != 0U) {
	  userStitchCount++;
	}
  }
  return userStitchCount;
}

void thred::internal::insfil() {
  auto const defDirectory = DefaultDirectory->wstring();
  // clang-format off
  auto file = OPENFILENAME {
	sizeof(OPENFILENAME),                             // lStructsize
	ThrEdWindow,                                      // hwndOwner
	ThrEdInstance,                                    // hInstance
	L"Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0", // lpstrFilter
	std::begin(CustomFilter),                         // lpstrCustomFilter
	_MAX_PATH,                                        // nMaxCustFilter
	0,                                                // nFilterIndex
	std::begin(InsertedFileName),                     // lpstrFile
	_MAX_PATH,                                        // nMaxFile
	nullptr,                                          // lpstrFileTitle
	0,                                                // nMaxFileTitle
	defDirectory.c_str(),                             // lpstrInitialDir
	nullptr,                                          // lpstrTitle
	OFN_EXPLORER | OFN_OVERWRITEPROMPT,               // NOLINT Flags
	0,                                                // nFileOffset
	0,                                                // nFileExtension
	L"thr",                                           // lpstrDefExt
	0,                                                // lCustData
	nullptr,                                          // lpfnHook
	nullptr,                                          // lpTemplateName
#ifdef _MAC
	nullptr,                                          // lpEditInfo
	0U,                                               // lpstrPrompt
#endif
#if (_WIN32_WINNT >= 0x0500)
	nullptr,                                          // *pvReserved
	0U,                                               // dwReserved
	0U                                                // FlagsEx
#endif
  //clang-format on
  };

  if (StateMap.test(StateFlag::IGNORINS) || GetOpenFileName(&file)) {
	InsertedFileHandle = CreateFile(
	    static_cast<LPTSTR>(InsertedFileName), (GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (InsertedFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	  displayText::filnopn(IDS_FNOPN, fs::path(std::begin(InsertedFileName)));
	  FileHandle = nullptr;
	  CloseHandle(InsertedFileHandle);
	}
	else {
	  InsertedStitchIndex = gsl::narrow<decltype(InsertedStitchIndex)>(StitchBuffer->size());
	  if (isthr(std::begin(InsertedFileName))) {
		auto fileHeader = STRHED {};
		ReadFile(InsertedFileHandle, &fileHeader, sizeof(fileHeader), &BytesRead, nullptr);
		if ((fileHeader.headerType & 0xffffffU) != 0x746872U) {
		  displayText::tabmsg(IDS_NOTHR);
		}
		else {
		  constexpr auto FRMW        = 5;
		  constexpr auto HANDW       = 4;
		  constexpr auto FRMPW       = 2;
		  constexpr auto STCHW       = 1;
		  auto           homscor     = 0U;
		  auto           thredHeader = STREX {};
		  auto const     version     = (fileHeader.headerType & 0xff000000) >> 24U;
		  if (version != 0U) {
			gethand(*StitchBuffer, wrap::toUnsigned(StitchBuffer->size()));
			// ToDo - replace constants with sizes of data structures?
			homscor = gsl::narrow<decltype(homscor)>(FormList->size()) * FRMW +
			          gethand(*StitchBuffer, wrap::toUnsigned(StitchBuffer->size())) * HANDW +
			          gsl::narrow<decltype(homscor)>(FormVertices->size()) * FRMPW +
			          gsl::narrow<decltype(homscor)>(StitchBuffer->size()) * STCHW;
			ReadFile(InsertedFileHandle, &thredHeader, sizeof(thredHeader), &BytesRead, nullptr);
		  }
		  thred::savdo();
		  auto fileStitchBuffer = std::vector<fPOINTATTR> {};
		  if (fileHeader.stitchCount != 0U) {
			fileStitchBuffer.resize(fileHeader.stitchCount);
			ReadFile(InsertedFileHandle,
			         fileStitchBuffer.data(),
			         fileHeader.stitchCount * sizeof(fileStitchBuffer.back()),
			         &BytesRead,
			         nullptr);
		  }
		  constexpr auto threadLength = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0])) /
		                                2U; // ThreadSize is defined as a 16 entry array of 2 bytes
		  constexpr auto formDataOffset = sizeof(PCSBMPFileName) + sizeof(BackgroundColor) +
		                                  sizeof(UserColor) + sizeof(CustomColor) + threadLength;
		  SetFilePointer(InsertedFileHandle, formDataOffset, nullptr, FILE_CURRENT);
		  auto insertedRectangle = fRECTANGLE {1e-9F, 1e9F, 1e-9F, 1e9F};
		  InsertedVertexIndex    = gsl::narrow<decltype(InsertedVertexIndex)>(FormVertices->size());
		  InsertedFormIndex      = gsl::narrow<decltype(InsertedFormIndex)>(FormList->size());
		  if (fileHeader.formCount != 0U) {
			auto const newFormVertexIndex = wrap::toUnsigned(FormVertices->size());
			auto       newSatinGuideIndex = wrap::toUnsigned(SatinGuides->size());
			auto       clipOffset         = wrap::toUnsigned(ClipPoints->size());
			auto       textureOffset      = wrap::toUnsigned(TexturePointsBuffer->size());
			if (version < 2) {
			  auto inFormList = std::vector<FRMHEDO> {};
			  inFormList.resize(fileHeader.formCount);
			  auto bytesToRead =
			      gsl::narrow<DWORD>(fileHeader.formCount * sizeof(decltype(inFormList.back())));
			  ReadFile(InsertedFileHandle, inFormList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != fileHeader.formCount * sizeof(decltype(inFormList.back()))) {
				inFormList.resize(BytesRead / sizeof(decltype(inFormList.back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			  FormList->reserve(FormList->size() + inFormList.size());
			  FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
			}
			else {
			  auto inFormList = std::vector<FRMHEDOUT> {};
			  inFormList.resize(fileHeader.formCount);
			  auto bytesToRead =
			      gsl::narrow<DWORD>(fileHeader.formCount * sizeof(decltype(inFormList.back())));
			  wrap::ReadFile(InsertedFileHandle, inFormList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				inFormList.resize(BytesRead / sizeof(decltype(inFormList.back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			  FormList->reserve(FormList->size() + inFormList.size());
			  FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
			}
			auto vertexOffset = wrap::toUnsigned(FormVertices->size());
			if (fileHeader.vertexCount != 0U) {
			  auto inVerticeList = std::vector<fPOINT> {};
			  inVerticeList.resize(fileHeader.vertexCount);
			  auto bytesToRead =
			      gsl::narrow<DWORD>(fileHeader.vertexCount * sizeof(decltype(inVerticeList.back())));
			  ReadFile(InsertedFileHandle, inVerticeList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				inVerticeList.resize(BytesRead / sizeof(decltype(inVerticeList.back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			  FormVertices->reserve(FormVertices->size() + inVerticeList.size());
			  FormVertices->insert(FormVertices->end(), inVerticeList.cbegin(), inVerticeList.cend());
			}
			else {
			  StateMap.set(StateFlag::BADFIL);
			}
			auto guideOffset = wrap::toUnsigned(SatinGuides->size());
			if (fileHeader.dlineCount != 0U) {
			  auto inGuideList = std::vector<SATCONOUT> {};
			  inGuideList.resize(fileHeader.dlineCount);
			  auto bytesToRead =
			      gsl::narrow<DWORD>(fileHeader.dlineCount * sizeof(decltype(inGuideList.back())));
			  ReadFile(InsertedFileHandle, inGuideList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				inGuideList.resize(BytesRead / sizeof(decltype(inGuideList.back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			  SatinGuides->reserve(SatinGuides->size() + inGuideList.size());
			  SatinGuides->insert(SatinGuides->end(), inGuideList.begin(), inGuideList.end());
			  newSatinGuideIndex += wrap::toUnsigned(inGuideList.size());
			}
			if (fileHeader.clipDataCount != 0U) {
			  auto inPointList = std::vector<fPOINT> {};
			  inPointList.resize(fileHeader.clipDataCount);
			  auto bytesToRead =
			      gsl::narrow<DWORD>(fileHeader.clipDataCount * sizeof(decltype(ClipPoints->back())));
			  ReadFile(InsertedFileHandle, inPointList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				inPointList.resize(BytesRead / sizeof(decltype(inPointList.back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			  ClipPoints->reserve(ClipPoints->size() + inPointList.size());
			  ClipPoints->insert(ClipPoints->end(), inPointList.begin(), inPointList.end());
			}
			if (thredHeader.texturePointCount != 0U) {
			  auto inTextureList = std::vector<TXPNT> {};
			  inTextureList.resize(thredHeader.texturePointCount);
			  auto bytesToRead = gsl::narrow<DWORD>(thredHeader.texturePointCount *
			                                        sizeof(decltype(inTextureList.back())));
			  ReadFile(InsertedFileHandle, inTextureList.data(), bytesToRead, &BytesRead, nullptr);
			  if (BytesRead != bytesToRead) {
				inTextureList.resize(BytesRead / sizeof(decltype(inTextureList.back())));
				StateMap.set(StateFlag::BADFIL);
			  }
			  TexturePointsBuffer->reserve(TexturePointsBuffer->size() + inTextureList.size());
			  TexturePointsBuffer->insert(
			      TexturePointsBuffer->end(), inTextureList.begin(), inTextureList.end());
			}
			CloseHandle(InsertedFileHandle);
			InsertedFileHandle = nullptr;
			// update the form pointer variables
			for (auto iFormList = InsertedFormIndex; iFormList < wrap::toUnsigned(FormList->size()); iFormList++) {
			  auto& formIter       = FormList->operator[](iFormList);
			  formIter.vertexIndex = vertexOffset;
			  vertexOffset += formIter.vertexCount;
			  if (formIter.type == SAT) {
				if (formIter.satinGuideCount != 0U) {
				  formIter.satinOrAngle.guide = guideOffset;
				  guideOffset += formIter.satinGuideCount;
				}
			  }
			  if (clip::isclp(formIter)) {
				formIter.angleOrClipData.clip = clipOffset;
				clipOffset += formIter.lengthOrCount.clipCount;
			  }
			  if (clip::iseclpx(iFormList)) {
				formIter.borderClipData = clipOffset;
				clipOffset += formIter.clipEntries;
			  }
			  if (texture::istx(iFormList)) {
				formIter.fillInfo.texture.index =
				    gsl::narrow<decltype(formIter.fillInfo.texture.index)>(textureOffset);
				textureOffset += formIter.fillInfo.texture.count;
			  }
			}
			if (newFormVertexIndex != FormVertices->size()) {
			  StateMap.set(StateFlag::BADFIL);
			}
			if (newSatinGuideIndex != SatinGuides->size()) {
			  StateMap.set(StateFlag::BADFIL);
			}
			if (clipOffset != ClipPoints->size()) {
			  StateMap.set(StateFlag::BADFIL);
			}

			if (fileHeader.formCount != 0U) {
			  auto const insertedVertex = FormVertices->operator[](InsertedVertexIndex);
			  insertedRectangle.left    = insertedVertex.x;
			  insertedRectangle.right   = insertedVertex.x;
			  insertedRectangle.bottom  = insertedVertex.y;
			  insertedRectangle.top     = insertedVertex.y;
			  for (auto iVertex = InsertedVertexIndex + 1U; iVertex < wrap::toUnsigned(FormVertices->size());
			       iVertex++) {
				auto const vertex = FormVertices->operator[](iVertex);
				if (vertex.x < insertedRectangle.left) {
				  insertedRectangle.left = vertex.x;
				}
				if (vertex.x > insertedRectangle.right) {
				  insertedRectangle.right = vertex.x;
				}
				if (vertex.y < insertedRectangle.bottom) {
				  insertedRectangle.bottom = vertex.y;
				}
				if (vertex.y > insertedRectangle.top) {
				  insertedRectangle.top = vertex.y;
				}
			  }
			}
		  }
		  if (fileHeader.stitchCount != 0U) {
			auto encodedFormIndex = gsl::narrow<uint32_t>(InsertedFormIndex << FRMSHFT);
			for (auto iStitch = 0; iStitch < fileHeader.stitchCount; iStitch++) {
			  if ((fileStitchBuffer[iStitch].attribute & ALTYPMSK) != 0U) {
				auto const newAttribute = (fileStitchBuffer[iStitch].attribute & FRMSK) + encodedFormIndex;
				fileStitchBuffer[iStitch].attribute &= NFRMSK;
				fileStitchBuffer[iStitch].attribute |= newAttribute;
			  }
			  if (fileStitchBuffer[iStitch].x < insertedRectangle.left) {
				insertedRectangle.left = fileStitchBuffer[iStitch].x;
			  }
			  if (fileStitchBuffer[iStitch].x > insertedRectangle.right) {
				insertedRectangle.right = fileStitchBuffer[iStitch].x;
			  }
			  if (fileStitchBuffer[iStitch].y < insertedRectangle.bottom) {
				insertedRectangle.bottom = fileStitchBuffer[iStitch].y;
			  }
			  if (fileStitchBuffer[iStitch].y > insertedRectangle.top) {
				insertedRectangle.top = fileStitchBuffer[iStitch].y;
			  }
			}
		  }
		  if ((fileHeader.headerType & 0x1000000U) != 0U) {
			// ToDo - Replace constants with sizes of data structures
			auto const filscor = (fileHeader.formCount) * FRMW +
			                     gethand(fileStitchBuffer, fileHeader.stitchCount) * HANDW +
			                     fileHeader.vertexLen * FRMPW + fileHeader.stitchCount * STCHW;
			if (filscor > homscor) {
			  for (auto iName = 0U; iName < 50U; iName++) {
				ExtendedHeader->creatorName[iName] = thredHeader.creatorName[iName];
			  }
			  redfnam(*DesignerName);
			  SetWindowText(ThrEdWindow,
			                fmt::format(StringTable->operator[](STR_THRDBY), ThrName->wstring(), *DesignerName)
			                    .c_str());
			}
		  }
		  InsertCenter = fPOINT {form::midl(insertedRectangle.right, insertedRectangle.left),
		                         form::midl(insertedRectangle.top, insertedRectangle.bottom)};
		  auto dest    = StitchBuffer->end();
		  StitchBuffer->insert(dest, fileStitchBuffer.cbegin(), fileStitchBuffer.cend());
		  auto const insertedSize = fPOINT {insertedRectangle.right - insertedRectangle.left,
		                                    insertedRectangle.top - insertedRectangle.bottom};
		  form::ratsr();
		  InsertSize.x = wrap::round<int32_t>(insertedSize.x * HorizontalRatio);
		  // ToDo - Should this be vertical ratio?
		  InsertSize.y = wrap::round<int32_t>(insertedSize.y * HorizontalRatio);
		  auto const initialInsertPoint =
		      POINT {StitchWindowClientRect.right / 2, StitchWindowClientRect.bottom / 2};
		  insflin(initialInsertPoint);
		  NewFormVertexCount = 5U;
		  StateMap.set(StateFlag::SHOFRM);
		  StateMap.set(StateFlag::INSFIL);
		  form::dufrm();
		}
	  }
	  else {
		auto pcsFileHeader = PCSHEADER {};
		ReadFile(InsertedFileHandle, &pcsFileHeader, 0x46, &BytesRead, nullptr);
		if (PCSHeader.leadIn == 0x32 && PCSHeader.colorCount == 16U) {
		  auto pcsStitchBuffer = std::vector<PCSTCH> {};
		  pcsStitchBuffer.resize(pcsFileHeader.stitchCount);
		  auto bytesToRead =
		      gsl::narrow<DWORD>(pcsFileHeader.stitchCount * sizeof(decltype(pcsStitchBuffer.back())));
		  ReadFile(InsertedFileHandle, pcsStitchBuffer.data(), bytesToRead, &BytesRead, nullptr);
		  if (BytesRead == bytesToRead) {
			thred::savdo();
			auto insertIndex = StitchBuffer->size();
			StitchBuffer->reserve(StitchBuffer->size() + pcsFileHeader.stitchCount);
			auto newAttribute = 0U;
			for (auto iPCSStitch = 0U; iPCSStitch < pcsFileHeader.stitchCount; iPCSStitch++) {
			  if (pcsStitchBuffer[iPCSStitch].tag == 3) {
				newAttribute = pcsStitchBuffer[iPCSStitch++].fx | NOTFRM;
			  }
			  else {
				(*StitchBuffer)
				    .emplace_back(
				        fPOINTATTR {gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].x) +
				                        gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].fx) / 256.0F,
				                    gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].y) +
				                        gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].fy) / 256.0F,
				                    newAttribute});
			  }
			}
			auto const newStitchCount = StitchBuffer->size();
			auto const startStitch    = StitchBuffer->operator[](insertIndex);

			auto insertedRectangle =
			    fRECTANGLE {startStitch.x, startStitch.y, startStitch.x, startStitch.y};

			for (; insertIndex < newStitchCount; insertIndex++) {
			  auto const stitch = StitchBuffer->operator[](insertIndex);
			  if (stitch.x < insertedRectangle.left) {
				insertedRectangle.left = stitch.x;
			  }
			  if (stitch.x > insertedRectangle.right) {
				insertedRectangle.right = stitch.x;
			  }
			  if (stitch.y < insertedRectangle.bottom) {
				insertedRectangle.bottom = stitch.y;
			  }
			  if (stitch.y > insertedRectangle.top) {
				insertedRectangle.top = stitch.y;
			  }
			}
			InsertCenter = fPOINT {form::midl(insertedRectangle.right, insertedRectangle.left),
			                       form::midl(insertedRectangle.top, insertedRectangle.bottom)};
			auto const insertedSize = fPOINT {insertedRectangle.right - insertedRectangle.left,
			                                  insertedRectangle.top - insertedRectangle.bottom};
			form::ratsr();
			InsertSize.x = wrap::round<int32_t>(insertedSize.x * HorizontalRatio);
			// ToDo - Should this be vertical ratio?
			InsertSize.y = wrap::round<int32_t>(insertedSize.y * HorizontalRatio);
			auto const initialInsertPoint =
			    POINT {StitchWindowClientRect.right / 2, StitchWindowClientRect.bottom / 2};
			insflin(initialInsertPoint);
			NewFormVertexCount = 5U;
			StateMap.set(StateFlag::SHOFRM);
			StateMap.set(StateFlag::INSFIL);
			form::dufrm();
			// We did not insert forms so insure that duinsfil does not move forms
			InsertedFormIndex   = wrap::toUnsigned(FormList->size());
			InsertedVertexIndex = wrap::toUnsigned(FormVertices->size());
		  }
		  else {
			// pcsStitchBuffer->resize(BytesRead / sizeof(decltype(StitchBuffer->back())));
			// StateMap.set(StateFlag::BADFIL);
			prtred();
			return;
		  }
		}
	  }
	  if (InsertedFileHandle != nullptr) {
		CloseHandle(InsertedFileHandle);
	  }
	}
  }
}

void thred::internal::getbak() {
  // auto thumbSpan = gsl::span<uint32_t, 4>{ ThumbnailsSelected };
  if (StateMap.test(StateFlag::THUMSHO)) {
	if (ThumbnailsSelected[FileVersionIndex] != 0U) { // NOLINT
	  if (StateMap.test(StateFlag::RBUT)) {
		wcscpy_s(InsertedFileName, Thumbnails->operator[](ThumbnailsSelected[FileVersionIndex]).data()); // NOLINT
		StateMap.set(StateFlag::IGNORINS);
		unthum();
		StateMap.set(StateFlag::FRMOF);
		insfil();
		if (wrap::pressed(VK_SHIFT)) {
		  StateMap.reset(StateFlag::INSFIL);
		  StateMap.reset(StateFlag::FRMOF);
		  StateMap.set(StateFlag::INIT);
		  thred::coltab();
		  StateMap.set(StateFlag::RESTCH);
		}
	  }
	  else {
		*WorkingFileName =
		    *DefaultDirectory / Thumbnails->operator[](ThumbnailsSelected[FileVersionIndex]); // NOLINT
		StateMap.set(StateFlag::REDOLD);
		nuFil();
	  }
	}
	else {
	  unthum();
	}
  }
}

void thred::internal::rebak() {
  for (auto& iVersion : BackupViewer) {
	DestroyWindow(iVersion);
  }
  auto newFileName    = *ThrName;
  auto safetyFileName = newFileName; // initialise from local variable
  auto ext            = newFileName.extension().wstring();
  ext.back()          = FileVersionIndex + 's';
  newFileName.replace_extension(ext);
  ext.back() = 'x';
  safetyFileName.replace_extension(ext);
  fs::rename(*ThrName, safetyFileName);
  if (fs::exists(newFileName)) {
	fs::rename(newFileName, *ThrName);
  }
  fs::rename(safetyFileName, newFileName);
  *WorkingFileName = *ThrName;
  StateMap.set(StateFlag::REDOLD);
  nuFil();
  fs::remove(safetyFileName);
}

void thred::internal::thumbak() {
  for (auto& iVersion : BackupViewer) {
	DestroyWindow(iVersion);
  }
  getbak();
}

void thred::internal::movbak(char source, char destination) {
  auto sourceFileName      = *ThrName;
  auto destinationFileName = sourceFileName; // initialise from local variable
  auto ext                 = ThrName->extension().wstring();

  ext.back() = source;
  sourceFileName.replace_extension(ext);
  ext.back() = destination;
  destinationFileName.replace_extension(ext);
  fs::remove(destinationFileName);
  fs::rename(sourceFileName, destinationFileName);
}

void thred::internal::purg() {
  if (FileHandle != nullptr) {
	auto fileName = *ThrName;
	auto ext      = ThrName->extension().wstring();
	for (auto iLast = 0U; iLast < OLDVER; iLast++) {
	  ext.back() = iLast + 's';
	  fileName.replace_extension(ext);
	  fs::remove(fileName);
	}
  }
}

void thred::internal::purgdir() {
  StateMap.set(StateFlag::PRGMSG);
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_DELBAK);
  displayText::shoMsg(fmt::format(fmtStr, DefaultDirectory->wstring()));
  displayText::okcan();
}

void thred::internal::deldir() {
  thred::unmsg();
  displayText::tabmsg(IDS_BAKDEL);
  auto backSpec = std::wstring {L".th0"};

  auto& backChar = backSpec.back();
  for (auto& p : fs::directory_iterator(*DefaultDirectory)) {
	if (!fs::is_directory(p)) {
	  auto fileExt = p.path().extension().wstring();
	  backChar     = 's';
	  for (char iLastChar = 0; iLastChar < OLDVER; iLastChar++) {
		if (fileExt == backSpec) {
		  fs::remove(p.path());
		  backChar++;
		}
	  }
	}
  }
  thred::unmsg();
  displayText::tabmsg(IDS_BAKDT);
}

auto thred::internal::chkwnd(HWND window) noexcept -> bool {
  auto windowRect = RECT {0L, 0L, 0L, 0L};

  GetWindowRect(window, &windowRect);
  return Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
         Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom;
}

auto thred::internal::chkok() noexcept -> bool {
  return chkwnd(OKButton);
}

void thred::internal::mv2f() {
  if (StateMap.testAndReset(StateFlag::FORMSEL)) {
	thred::savdo();
	std::vector<fPOINTATTR> tempStitchBuffer {};
	// since most of the stitches will be moved into the temp buffer, reserve the space
	tempStitchBuffer.reserve(StitchBuffer->size());
	auto       iLowBuffer = 0U;
	auto const attribute  = ClosestFormToCursor << FRMSHFT;
	for (auto& stitch : *StitchBuffer) {
	  if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == attribute) {
		StitchBuffer->operator[](iLowBuffer++) = stitch;
	  }
	  else {
		tempStitchBuffer.push_back(stitch);
	  }
	}
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), std::next(StitchBuffer->begin(), iLowBuffer));
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
	  thred::savdo();
	  std::vector<fPOINTATTR> tempStitchBuffer {};
	  auto const              grpSize = GroupEndStitch + 1U - GroupStartStitch;
	  tempStitchBuffer.resize(StitchBuffer->size() - grpSize);
	  thred::rngadj();
	  std::copy(StitchBuffer->begin(),
	            std::next(StitchBuffer->begin(), GroupStartStitch),
	            tempStitchBuffer.begin());
	  std::copy(std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch) + 1U),
	            StitchBuffer->end(),
	            std::next(tempStitchBuffer.begin(), GroupStartStitch));
	  std::copy(std::next(StitchBuffer->begin(), GroupStartStitch),
	            std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch) + 1U),
	            StitchBuffer->begin());
	  std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), std::next(StitchBuffer->begin(), grpSize));
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::mv2b() {
  if (StateMap.testAndReset(StateFlag::FORMSEL)) {
	thred::savdo();
	std::vector<fPOINTATTR> tempStitchBuffer {};
	auto                    iLowBuffer = 0U;
	auto const              attribute  = ClosestFormToCursor << FRMSHFT;
	for (auto& stitch : *StitchBuffer) {
	  if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == attribute) {
		tempStitchBuffer.push_back(stitch);
	  }
	  else {
		StitchBuffer->operator[](iLowBuffer++) = stitch;
	  }
	}
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), std::next(StitchBuffer->begin(), iLowBuffer));
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
	  thred::savdo();
	  thred::rngadj();
	  std::vector<fPOINTATTR> tempStitchBuffer {};
	  auto const              grpSize = GroupEndStitch + 1U - GroupStartStitch;
	  tempStitchBuffer.resize(grpSize);
	  std::copy(std::next(StitchBuffer->begin(), GroupStartStitch),
	            std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch) + 1U),
	            tempStitchBuffer.begin());
	  std::copy(std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch) + 1U),
	            StitchBuffer->end(),
	            std::next(StitchBuffer->begin(), GroupStartStitch));
	  std::copy(tempStitchBuffer.begin(),
	            tempStitchBuffer.end(),
	            std::next(StitchBuffer->begin(), StitchBuffer->size() - grpSize));
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::infadj(float& xCoordinate, float& yCoordinate) noexcept {
  if (isfinite(xCoordinate)) {
	if (xCoordinate > UnzoomedRect.x) {
	  xCoordinate = UnzoomedRect.x;
	}
	else {
	  if (xCoordinate < 0) {
		xCoordinate = 0;
	  }
	}
  }
  if (isfinite(yCoordinate)) {
	if (yCoordinate > UnzoomedRect.y) {
	  yCoordinate = UnzoomedRect.y;
	}
	else {
	  if (yCoordinate < 0) {
		yCoordinate = 0;
	  }
	}
  }
}

void thred::delinf() noexcept {
  for (auto& iStitch : *StitchBuffer) {
	thi::infadj(iStitch.x, iStitch.y);
  }
  for (auto& FormVertice : *FormVertices) {
	thi::infadj(FormVertice.x, FormVertice.y);
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void thred::chkrng(fPOINT& range) {
  thred::savdo();
  thred::delinf();
  range.x = gsl::narrow<double>(UnzoomedRect.x);
  range.y = gsl::narrow<double>(UnzoomedRect.y);
  if (!FormList->empty()) {
	// ToDo - Why do we treat the forms differently?
	auto tmpCount     = 0U;
	auto iDestination = StitchBuffer->begin();
	for (auto& stitch : *StitchBuffer) {
	  if (((stitch.attribute & NOTFRM) != 0U) ||
	      (((stitch.attribute & TYPMSK) != 0U) && ((stitch.attribute & FRMSK) >> FRMSHFT < FormList->size()))) {
		if (stitch.x > range.x) {
		  stitch.x = range.x - 1.0F;
		}
		if (stitch.x < 0) {
		  stitch.x = 0;
		}
		if (stitch.y > range.y) {
		  stitch.y = range.y - 1.0F;
		}
		if (stitch.y < 0) {
		  stitch.y = 0;
		}
		*iDestination++ = stitch;
	  }
	  else {
		OutputDebugString(fmt::format(L"chkrng:tmpCount [{}]\n", tmpCount).c_str());
	  }
	  tmpCount++;
	}
	StitchBuffer->resize(iDestination - StitchBuffer->begin());
	thred::coltab();
  }
  else {
	for (auto& stitch : *StitchBuffer) {
	  if (stitch.x > range.x) {
		stitch.x = range.x - 1.0F;
	  }
	  if (stitch.x < 0) {
		stitch.x = 0;
	  }
	  if (stitch.y > range.y) {
		stitch.y = range.y - 1.0F;
	  }
	  if (stitch.y < 0) {
		stitch.y = 0;
	  }
	}
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void thred::ritmov(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  if (ClosestVertexToCursor != 0U) {
	if (ClosestVertexToCursor == gsl::narrow<uint32_t>(form.vertexCount) - 1 && form.type == FRMLINE) {
	  Polyline(StitchWindowDC, RubberBandLine->data(), 2);
	}
	else {
	  Polyline(StitchWindowDC, RubberBandLine->data(), 3);
	}
  }
  else {
	RubberBandLine->operator[](2) = FormLines->operator[](1);
	if (form.type == FRMLINE) {
	  Polyline(StitchWindowDC, &RubberBandLine->operator[](1), 2);
	}
	else {
	  Polyline(StitchWindowDC, RubberBandLine->data(), 3);
	}
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unmov() {
  if (StateMap.testAndReset(StateFlag::SHOMOV)) {
	thred::ritmov(ClosestFormToCursor);
  }
}

void thred::internal::duprct(FRMHED const& form) {
  auto currentVertex        = SelectedFormVertices.start;
  auto vertexIt             = std::next(FormVertices->cbegin(), form.vertexIndex);
  SelectedVerticesRect.left = SelectedVerticesRect.right = vertexIt[currentVertex].x;
  SelectedVerticesRect.top = SelectedVerticesRect.bottom = vertexIt[currentVertex].y;
  currentVertex                                          = form::pdir(form, currentVertex);
  for (auto iVertex = 0U; iVertex < SelectedFormVertices.vertexCount; iVertex++) {
	if (vertexIt[currentVertex].x < SelectedVerticesRect.left) {
	  SelectedVerticesRect.left = vertexIt[currentVertex].x;
	}
	if (vertexIt[currentVertex].x > SelectedVerticesRect.right) {
	  SelectedVerticesRect.right = vertexIt[currentVertex].x;
	}
	if (vertexIt[currentVertex].y < SelectedVerticesRect.bottom) {
	  SelectedVerticesRect.bottom = vertexIt[currentVertex].y;
	}
	if (vertexIt[currentVertex].y > SelectedVerticesRect.top) {
	  SelectedVerticesRect.top = vertexIt[currentVertex].y;
	}
	currentVertex = form::pdir(form, currentVertex);
  }
}

void thred::setpsel() {
  form::unpsel();
  auto const& form = FormList->operator[](ClosestFormToCursor);
  thi::duprct(form);
  form::sRct2px(SelectedVerticesRect, SelectedPixelsRect);
  form::rct2sel(SelectedPixelsRect, *SelectedPointsLine);
  auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
  form::sfCor2px(vertexIt[SelectedFormVertices.finish], EndPointCross);
  StateMap.set(StateFlag::SHOPSEL);
  form::dupsel(StitchWindowDC);
  StateMap.set(StateFlag::FPSEL);
}

void thred::rotfn(float rotationAngle, fPOINT const& rotationCenter) {
  thred::savdo();
  if (StateMap.test(StateFlag::FPSEL)) {
	auto& form                           = FormList->operator[](ClosestFormToCursor);
	auto                   currentVertex = SelectedFormVertices.start;
	auto                   vertexIt      = std::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
	  thred::rotflt(vertexIt[currentVertex], rotationAngle, rotationCenter);
	  currentVertex = form::pdir(form, currentVertex);
	}
	form::frmout(ClosestFormToCursor);
	thred::setpsel();
	form::refil();
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (StateMap.test(StateFlag::BIGBOX)) {
	for (auto& FormVertice : *FormVertices) {
	  thred::rotflt(FormVertice, rotationAngle, rotationCenter);
	}
	for (auto& stitch : *StitchBuffer) {
	  thi::rotstch(stitch, rotationAngle, rotationCenter);
	}
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	  form::frmout(iForm);
	}
	form::selal();
	return;
  }
  if (StateMap.testAndReset(StateFlag::FRMSROT)) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor             = selectedForm;
	  auto& form                      = FormList->operator[](selectedForm);
	  auto                   vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		thred::rotflt(vertexIt[iVertex], rotationAngle, rotationCenter);
	  }
	  form::frmout(ClosestFormToCursor);
	  form::refilfn();
	}
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.testAndReset(StateFlag::FRMROT)) {
	  auto& form                      = FormList->operator[](ClosestFormToCursor);
	  auto                   vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		thred::rotflt(vertexIt[iVertex], rotationAngle, rotationCenter);
	  }
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	  StateMap.set(StateFlag::RESTCH);
	}
	else {
	  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
		thi::rotstch(StitchBuffer->operator[](iStitch), rotationAngle, rotationCenter);
	  }
	  thred::rngadj();
	  thi::selin(GroupStartStitch, GroupEndStitch, StitchWindowDC);
	  StateMap.set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::rotfns(float rotationAngle) {
  thred::savdo();
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
}

void thred::internal::nulayr(uint32_t play) {
  ActiveLayer = play;
  LayerIndex  = play / 2;
  ladj();
  if (ActiveLayer != 0U) {
	if (StateMap.test(StateFlag::FORMSEL) &&
	    ((gsl::narrow_cast<decltype(ActiveLayer)>(FormList->operator[](ClosestFormToCursor).attribute & FRMLMSK) >>
	      1U) != ActiveLayer)) {
	  StateMap.reset(StateFlag::FORMSEL);
	}
	StateMap.reset(StateFlag::GRPSEL);
	if (StateMap.test(StateFlag::SELBOX)) {
	  if (ActiveLayer != ((StitchBuffer->operator[](ClosestPointIndex).attribute & LAYMSK) >> LAYSHFT) + 1U) {
		StateMap.reset(StateFlag::SELBOX);
	  }
	}
	SelectedFormList->clear();
  }
  StateMap.set(StateFlag::RESTCH);
}

auto thred::internal::iselpnt() noexcept -> bool {
  auto const pointToTest = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto       closestControlPoint = 0U;
  auto       minimumLength       = 1e99;
  auto       iControlPoint       = 0U;
  for (auto controlPoint : *FormControlPoints) {
	auto const length = hypot(pointToTest.x - controlPoint.x, pointToTest.y - controlPoint.y);
	if (length < minimumLength) {
	  minimumLength       = length;
	  closestControlPoint = iControlPoint;
	}
	iControlPoint++;
  }
  if (minimumLength < CLOSENUF) {
	SelectedFormControlVertex = closestControlPoint;
	return true;
  }
  return false;
}

void thred::strtchbox(std::vector<POINT>& stretchBoxLine) noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  Polyline(StitchWindowDC, stretchBoxLine.data(), 5);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unstrtch(std::vector<POINT>& stretchBoxLine) {
  if (StateMap.testAndReset(StateFlag::SHOSTRTCH)) {
	thred::strtchbox(stretchBoxLine);
  }
}

auto thred::internal::chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  auto       minimumLength = 1e99;
  auto const pointToTest = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};

  for (auto iControlPoint = 0U; iControlPoint < 9U; iControlPoint++) {
	auto const length = hypot(pointToTest.x - SelectedFormsLine->operator[](iControlPoint).x,
	                          pointToTest.y - SelectedFormsLine->operator[](iControlPoint).y);
	if (length < minimumLength) {
	  minimumLength             = length;
	  SelectedFormControlVertex = iControlPoint;
	}
  }
  auto& formLines = *FormLines;
  formLines.resize(5U);
  for (auto iCorner = 0U; iCorner < 4; iCorner++) {
	formLines[iCorner] = SelectedFormsLine->operator[](wrap::toSize(iCorner) * 2U);
  }
  formLines[4] = formLines[0];
  if (minimumLength < CLOSENUF) {
	for (auto iCorner = 0U; iCorner < 4; iCorner++) {
	  stretchBoxLine[iCorner] = SelectedFormsLine->operator[](wrap::toSize(iCorner) * 2U);
	}
	stretchBoxLine[4] = stretchBoxLine[0];
	thred::strtchbox(stretchBoxLine);
	if ((SelectedFormControlVertex & 1U) != 0U) {
	  StateMap.set(StateFlag::STRTCH);
	}
	else {
	  StateMap.set(StateFlag::EXPAND);
	  xyRatio = gsl::narrow_cast<float>(SelectedFormsRect.right - SelectedFormsRect.left) /
	            gsl::narrow_cast<float>(SelectedFormsRect.bottom - SelectedFormsRect.top);
	}
	SelectedFormControlVertex >>= 1U;
	StateMap.set(StateFlag::SHOSTRTCH);
	return true;
  }
  SelectedFormControlVertex >>= 1U;
  if (pointToTest.x >= SelectedFormsRect.left && pointToTest.x <= SelectedFormsRect.right &&
      pointToTest.y >= SelectedFormsRect.top && pointToTest.y <= SelectedFormsRect.bottom) {
	SelectedFormsSize = fPOINT {SelectedFormsRect.right - SelectedFormsRect.left,
	                            SelectedFormsRect.bottom - SelectedFormsRect.top};
	StateMap.set(StateFlag::MOVFRMS);
	FormMoveDelta =
	    fPOINT {pointToTest.x - SelectedFormsRect.left, pointToTest.y - SelectedFormsRect.top};
	StateMap.set(StateFlag::SHOSTRTCH);
	thred::strtchbox(stretchBoxLine);
	return true;
  }
  return false;
}

void thred::internal::delfre() {
  auto currentStitchCount = 0U;

  thred::savdo();
  // ToDo - this loop does not delete all free stitches. look at frmdel as well
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & NOTFRM) == 0U) {
	  StitchBuffer->operator[](currentStitchCount++) = stitch;
	}
  }
  StitchBuffer->resize(currentStitchCount);
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::setmov() {
  if (StateMap.test(StateFlag::SELBOX)) {
	MoveAnchor = ClosestPointIndex;
	StateMap.set(StateFlag::MOVSET);
	StateMap.set(StateFlag::RESTCH);
  }
}

void thred::internal::dufsel() {
  auto start  = 0U;
  auto finish = 0U;

  if (LastFormSelected > ClosestFormToCursor) {
	start  = ClosestFormToCursor;
	finish = LastFormSelected;
  }
  else {
	start  = LastFormSelected;
	finish = ClosestFormToCursor;
  }
  SelectedFormList->clear();
  SelectedFormList->reserve(wrap::toSize(finish) - start + 1U);
  while (start <= finish) {
	SelectedFormList->push_back(start);
	start++;
  }
}

void thred::internal::selup() {
  if (wrap::pressed(VK_SHIFT)) {
	StateMap.reset(StateFlag::SELBOX);
	if (StateMap.testAndReset(StateFlag::FORMSEL)) {
	  if (ClosestFormToCursor < FormList->size() - 1U) {
		SelectedFormList->push_back(ClosestFormToCursor);
		LastFormSelected = ClosestFormToCursor + 1U;
		SelectedFormList->push_back(LastFormSelected);
	  }
	  else {
		return;
	  }
	}
	else {
	  if (!SelectedFormList->empty()) {
		if (LastFormSelected < FormList->size() - 1U) {
		  LastFormSelected++;
		  dufsel();
		}
	  }
	  else {
		StateMap.set(StateFlag::FORMSEL);
	  }
	}
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.test(StateFlag::SELBOX)) {
	  unbox();
	  auto const attribute = StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK;
	  while (ClosestPointIndex < gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U) &&
	         (StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK) == attribute) {
		ClosestPointIndex++;
	  }
	  stch2px(ClosestPointIndex);
	  dubox();
	}
	else {
	  if (!FormList->empty()) {
		if (StateMap.testAndSet(StateFlag::FORMSEL)) {
		  if (ClosestFormToCursor < gsl::narrow<uint32_t>(FormList->size()) - 1U) {
			ClosestFormToCursor++;
		  }
		}
		else {
		  ClosestFormToCursor = 0;
		}
		displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
		StateMap.set(StateFlag::RESTCH);
	  }
	}
  }
}

void thred::internal::seldwn() {
  if (wrap::pressed(VK_SHIFT)) {
	StateMap.reset(StateFlag::SELBOX);
	if (StateMap.testAndReset(StateFlag::FORMSEL)) {
	  if (ClosestFormToCursor != 0U) {
		SelectedFormList->push_back(ClosestFormToCursor);
		LastFormSelected = ClosestFormToCursor - 1U;
		SelectedFormList->push_back(LastFormSelected);
	  }
	  else {
		return;
	  }
	}
	else {
	  if (!SelectedFormList->empty()) {
		if (LastFormSelected != 0U) {
		  LastFormSelected--;
		  dufsel();
		}
	  }
	  else {
		StateMap.set(StateFlag::FORMSEL);
	  }
	}
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.test(StateFlag::SELBOX)) {
	  unbox();
	  auto const attribute = StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK;
	  while ((ClosestPointIndex != 0U) &&
	         (StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK) == attribute) {
		ClosestPointIndex--;
	  }
	  stch2px(ClosestPointIndex);
	  dubox();
	}
	else {
	  if (!FormList->empty()) {
		if (StateMap.testAndSet(StateFlag::FORMSEL)) {
		  if (ClosestFormToCursor != 0U) {
			ClosestFormToCursor--;
		  }
		}
		else {
		  ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
		}
		displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
		StateMap.set(StateFlag::RESTCH);
	  }
	}
  }
}

auto thred::internal::movstchs(uint32_t destination, uint32_t start, uint32_t finish) -> bool {
  std::vector<fPOINTATTR> tempStitchBuffer {};

  if ((destination + 1U) < wrap::toUnsigned(StitchBuffer->size())) {
	destination++;
  }
  if (start > finish) {
	std::swap(start, finish);
  }
  if (destination >= start && destination < finish) {
	displayText::tabmsg(IDS_DST1);
	return false;
  }
  if (destination < start) {
	tempStitchBuffer.resize(finish - destination);
	std::copy(std::next(StitchBuffer->begin(), start),
	          std::next(StitchBuffer->begin(), finish),
	          tempStitchBuffer.begin());
	std::copy(std::next(StitchBuffer->begin(), destination),
	          std::next(StitchBuffer->begin(), start),
	          std::next(tempStitchBuffer.begin(), finish - start));
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), std::next(StitchBuffer->begin(), destination));
  }
  else {
	tempStitchBuffer.resize(destination - start);
	std::copy(std::next(StitchBuffer->begin(), finish),
	          std::next(StitchBuffer->begin(), destination),
	          tempStitchBuffer.begin());
	std::copy(std::next(StitchBuffer->begin(), start),
	          std::next(StitchBuffer->begin(), finish),
	          std::next(tempStitchBuffer.begin(), destination - finish));
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), std::next(StitchBuffer->begin(), start));
  }
  return true;
}

void thred::internal::movmrk() {
  if (StateMap.test(StateFlag::MOVSET)) {
	if (StateMap.test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  if (movstchs(MoveAnchor, GroupStartStitch, GroupEndStitch)) {
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	  }
	}
	else {
	  if (StateMap.test(StateFlag::SELBOX)) {
		if (movstchs(MoveAnchor, ClosestPointIndex, ClosestPointIndex + 1U)) {
		  StateMap.set(StateFlag::RESTCH);
		  thred::coltab();
		}
	  }
	}
  }
}

void thred::internal::vuthrds() {
  if ((GetMenuState(ViewMenu, ID_VUTHRDS, MF_BYCOMMAND) & MF_CHECKED) != 0U) { // NOLINT
	CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);          // NOLINT
	StateMap.reset(StateFlag::THRDS);
  }
  else {
	CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_CHECKED); // NOLINT
	StateMap.set(StateFlag::THRDS);
  }
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::vuselthr() {
  if ((GetMenuState(ViewMenu, ID_VUSELTHRDS, MF_BYCOMMAND) & MF_CHECKED) != 0U) { // NOLINT
	CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);          // NOLINT
	StateMap.reset(StateFlag::COL);
  }
  else {
	CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_CHECKED); // NOLINT
	StateMap.set(StateFlag::COL);
  }
  StateMap.set(StateFlag::RESTCH);
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void thred::internal::colchk() {
  if (!StitchBuffer->empty()) {
	auto color         = StitchBuffer->front().attribute & COLMSK;
	auto currentStitch = 0U;

	// ToDo - Can this loop become a ranged for?
	for (auto iStitch = 0U; iStitch < gsl::narrow<decltype(iStitch)>(StitchBuffer->size()); iStitch++) {
	  auto const stitch = StitchBuffer->operator[](iStitch);
	  if (color != (stitch.attribute & COLMSK)) {
		if ((iStitch - currentStitch == 1) && ((currentStitch) != 0U)) {
		  auto stitchBck1      = StitchBuffer->operator[](iStitch - 1U);
		  stitchBck1.attribute = (stitch.attribute & NCOLMSK) | (stitchBck1.attribute & COLMSK);
		}
		color         = stitch.attribute & COLMSK;
		currentStitch = iStitch;
	  }
	}
  }
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) auto thred::internal::makbig(uint32_t start, uint32_t finish) -> uint32_t {
  auto newStitches = std::vector<fPOINTATTR> {};
  newStitches.reserve(finish - start); // we know that we will have at least this number of Stitches
  auto adcnt = 0U;

  finish--;
  auto stitchIt     = std::next(StitchBuffer->begin(), start);
  auto nextStitchIt = stitchIt + 1U;
  for (auto iSource = start; iSource < finish; iSource++) {
	auto const delta = fPOINT {(*nextStitchIt).x - (*stitchIt).x, (*nextStitchIt).y - (*stitchIt).y};
	auto const length = hypot(delta.x, delta.y);
	newStitches.push_back(*stitchIt);
	if (length > IniFile.maxStitchLength) {
	  auto const stitchCount = std::ceil(length / UserStitchLength);
	  auto const step        = fPOINT {delta.x / stitchCount, delta.y / stitchCount};
	  auto       point       = fPOINT {(*stitchIt).x + step.x, (*stitchIt).y + step.y};
	  auto       attribute   = (*stitchIt).attribute;
	  if (attribute != (*nextStitchIt).attribute) {
		if (((attribute & NOTFRM) == 0U) && (((*nextStitchIt).attribute & TYPMSK) != 0U)) {
		  if (!((attribute & FRMSK) == ((*nextStitchIt).attribute & FRMSK))) {
			attribute &= NTYPMSK;
			attribute |= NOTFRM;
		  }
		}
		else {
		  attribute &= NTYPMSK;
		  attribute |= NOTFRM;
		}
	  }
	  attribute &= (~KNOTMSK);
	  for (auto iStitch = 0U; iStitch < wrap::round<decltype(iStitch)>(stitchCount) - 1U; iStitch++) {
		newStitches.emplace_back(fPOINTATTR {point.x, point.y, attribute});
		point.x += step.x;
		point.y += step.y;
		adcnt++;
	  }
	}
	stitchIt++;
	nextStitchIt++;
  }
  // now copy stitches back up to the end of the original group
  std::copy(newStitches.begin(),
            std::next(newStitches.begin(), finish - start),
            std::next(StitchBuffer->begin(), start));
  // and then insert the remainder of the new stitches
  StitchBuffer->insert(std::next(StitchBuffer->begin(), finish),
                       std::next(newStitches.begin(), finish - start),
                       newStitches.end());
  return adcnt;
}

void thred::internal::rembig() {
  if (UserStitchLength < IniFile.maxStitchLength) {
	thred::savdo();
	do {
	  if (!SelectedFormList->empty()) {
		auto range = RANGE {};
		for (auto selectedForm : (*SelectedFormList)) {
		  form::frmrng(selectedForm, range);
		  thi::makbig(range.start, range.finish);
		}
		break;
	  }
	  if (StateMap.test(StateFlag::FORMSEL)) {
		auto range = RANGE {};
		form::frmrng(ClosestFormToCursor, range);
		thi::makbig(range.start, range.finish);
		break;
	  }
	  if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		if (GroupEndStitch < gsl::narrow<decltype(GroupEndStitch)>(StitchBuffer->size())) {
		  GroupEndStitch++;
		}
		if (ClosestPointIndex < GroupStitchIndex) {
		  GroupStitchIndex += thi::makbig(GroupStartStitch, GroupEndStitch);
		}
		else {
		  ClosestPointIndex += thi::makbig(GroupStartStitch, GroupEndStitch);
		}
		thred::grpAdj();
		break;
	  }
	  thi::makbig(0, wrap::toUnsigned(StitchBuffer->size()));
	} while (false);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_REM1);
  }
}

void thred::internal::duselrng() {
  SelectedRange.start  = 0U;
  SelectedRange.finish = gsl::narrow<decltype(SelectedRange.finish)>(StitchBuffer->size());
  if (StateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	SelectedRange.start  = GroupStartStitch;
	SelectedRange.finish = GroupEndStitch;
	return;
  }
}

void thred::internal::longer() {
  auto iStitch = 0U;
  auto flag    = true;

  if (ClosestPointIndex == LargestStitchIndex) {
	return;
  }
  auto const start         = StitchBuffer->operator[](ClosestPointIndex);
  auto const startFwd1     = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
  auto const currentLength = hypot(startFwd1.x - start.x, startFwd1.y - start.y);
  for (iStitch = ClosestPointIndex + 1U; iStitch < SelectedRange.finish; iStitch++) {
	auto const stitch     = StitchBuffer->operator[](iStitch);
	auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	auto const length     = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
	if (length == currentLength) {
	  flag = false;
	  break;
	}
  }
  if (flag) {
	auto       minimumLength = 1e99;
	auto const rangeEnd      = SelectedRange.finish - 1U;
	for (auto currentStitch = SelectedRange.start; currentStitch < rangeEnd; currentStitch++) {
	  auto const stitch     = StitchBuffer->operator[](currentStitch);
	  auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(currentStitch) + 1U);
	  auto const length     = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
	  if (length > currentLength && length < minimumLength) {
		minimumLength = length;
		iStitch       = currentStitch;
	  }
	}
	if (minimumLength == 1e99) {
	  return;
	}
  }
  CurrentStitchIndex = iStitch;
  lensadj();
  displayText::ritnum(STR_NUMSCH, ClosestPointIndex);
}

void thred::internal::shorter() {
  auto currentStitch = 0U;
  auto currentLength = 0.0;
  auto flag          = true;

  if (ClosestPointIndex == SmallestStitchIndex) {
	return;
  }
  auto const start     = StitchBuffer->operator[](ClosestPointIndex);
  auto const startFwd1 = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
  currentLength        = hypot(startFwd1.x - start.x, startFwd1.y - start.y);
  for (currentStitch = ClosestPointIndex; currentStitch != 0; currentStitch--) {
	auto const stitch     = StitchBuffer->operator[](currentStitch);
	auto const stitchBck1 = StitchBuffer->operator[](wrap::toSize(currentStitch) - 1U);
	auto const length     = hypot(stitch.x - stitchBck1.x, stitch.y - stitchBck1.y);
	if (length == currentLength) {
	  currentStitch--;
	  flag = false;
	  break;
	}
  }
  if (flag) {
	auto maximumLength = 0.0;
	auto iStitch       = 0U;
	for (iStitch = SelectedRange.start; iStitch < SelectedRange.finish - 1U; iStitch++) {
	  auto const stitch     = StitchBuffer->operator[](iStitch);
	  auto const stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	  auto const length     = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
	  if (length < currentLength && length > maximumLength) {
		maximumLength = length;
		currentStitch = iStitch;
	  }
	}
	// ToDo - Is this right?
	auto const minLength = hypot(
	    StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).x - StitchBuffer->operator[](iStitch).x,
	    StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y - StitchBuffer->operator[](iStitch).y);
	displayText::butxt(HMINLEN, fmt::format(L"{:.2f}", minLength));
  }
  CurrentStitchIndex = currentStitch;
  lensadj();
  displayText::ritnum(STR_NUMSCH, ClosestPointIndex);
}

void thred::internal::setsrch(uint32_t stitch) {
  CurrentStitchIndex = stitch;
  lensadj();
  displayText::ritnum(STR_NUMSCH, ClosestPointIndex);
}

auto thred::internal::inrng(uint32_t iStitch) noexcept -> bool {
  if (iStitch < StitchBuffer->size()) {
	auto const stitch = StitchBuffer->operator[](iStitch);
	return stitch.x >= StitchRangeRect.left && stitch.x <= StitchRangeRect.right &&
	       stitch.y >= StitchRangeRect.bottom && stitch.y <= StitchRangeRect.top;
  }
  return false;
}

auto thred::internal::finrng(uint32_t find) noexcept -> bool {
  auto const& rectFind = FormList->operator[](find).rectangle;
  if (rectFind.left >= StitchRangeRect.left && rectFind.right <= StitchRangeRect.right &&
      rectFind.bottom >= StitchRangeRect.bottom && rectFind.top <= StitchRangeRect.top) {
	if (ActiveLayer == 0U) {
	  return true;
	}
	auto const cod = gsl::narrow_cast<uint8_t>(
	    gsl::narrow_cast<uint8_t>(FormList->operator[](find).attribute & FRMLMSK) >> 1U);
	return (cod == 0U) || ActiveLayer == cod;
  }

  return false;
}

void thred::internal::ungrplo() {
  if (StateMap.testAndReset(StateFlag::GRPSEL)) {
	thred::rngadj();
	ClosestPointIndex = GroupStartStitch;
	StateMap.set(StateFlag::SELBOX);
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  auto flag    = true;
	  auto iStitch = 0U;
	  for (auto& stitch : *StitchBuffer) {
		if (((stitch.attribute & NOTFRM) == 0U) && ((stitch.attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
		  ClosestPointIndex = iStitch;
		  StateMap.set(StateFlag::SELBOX);
		  StateMap.set(StateFlag::RESTCH);
		  flag = false;
		  break;
		}
		iStitch++;
	  }
	  if (flag) {
		displayText::grpmsg1();
	  }
	}
	else {
	  displayText::grpmsg();
	}
  }
}

void thred::internal::ungrphi() {
  if (StateMap.testAndReset(StateFlag::GRPSEL)) {
	thred::rngadj();
	ClosestPointIndex = GroupEndStitch;
	StateMap.set(StateFlag::SELBOX);
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  auto flag = true;
	  for (auto iStitch = wrap::toUnsigned(StitchBuffer->size()); iStitch != 0; iStitch--) {
		if (((StitchBuffer->operator[](iStitch - 1U).attribute & NOTFRM) == 0U) &&
		    ((StitchBuffer->operator[](iStitch - 1U).attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
		  ClosestPointIndex = iStitch - 1U;
		  StateMap.set(StateFlag::SELBOX);
		  StateMap.set(StateFlag::RESTCH);
		  flag = false;
		  break;
		}
	  }
	  if (flag) {
		displayText::grpmsg1();
	  }
	}
	else {
	  displayText::grpmsg();
	}
  }
}

void thred::internal::barnam(HWND window, uint32_t iThumbnail) {
  if (iThumbnail < ThumbnailDisplayCount) {
	auto thumbPath = fs::path(Thumbnails->operator[](ThumbnailsSelected[iThumbnail]).data());
	auto name      = thumbPath.stem().wstring().substr(0U, 12U);
	SetWindowText(window, name.c_str());
  }
  else {
	SetWindowText(window, static_cast<LPCWSTR>(L""));
  }
}

void thred::internal::rthumnam(uint32_t iThumbnail) {
  switch (iThumbnail) {
	case 0: {
	  barnam(ButtonWin->operator[](HNUM), iThumbnail);
	  break;
	}
	case 1: {
	  barnam(ButtonWin->operator[](HTOT), iThumbnail);
	  break;
	}
	case 2: {
	  barnam(ButtonWin->operator[](HMINLEN), iThumbnail);
	  break;
	}
	case 3: {
	  barnam(ButtonWin->operator[](HMAXLEN), iThumbnail);
	  break;
	}
	default: {
	}
  }
}

void thred::internal::thumnail() {
  auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};

  thred::unbsho();
  thred::undat();
  trace::untrace();

  fs::current_path(*DefaultDirectory);
  *SearchName = *DefaultDirectory / L"*.thr";
  auto file   = FindFirstFile(SearchName->wstring().c_str(), &fileData);
  if (file == INVALID_HANDLE_VALUE) { // NOLINT
	auto const dwError = GetLastError();
	auto       fmtStr  = std::wstring {};
	displayText::loadString(fmtStr, IDS_FFINDERR);
	displayText::shoMsg(fmt::format(fmtStr, SearchName->wstring(), dwError));
	unthum();
  }
  else {
	Thumbnails->clear();
	Thumbnails->push_back(std::wstring(std::begin(fileData.cFileName)));
	while (FindNextFile(file, &fileData)) {
	  Thumbnails->push_back(std::wstring(std::begin(fileData.cFileName)));
	}
	FindClose(file);
	std::sort(Thumbnails->begin(), Thumbnails->end());
	auto       iThumbnail = 0U;
	auto const thumbSize  = Thumbnails->size();
	while (iThumbnail < 4 && iThumbnail < thumbSize) {
	  ThumbnailsSelected[iThumbnail] = iThumbnail;
	  iThumbnail++;
	}
	ThumbnailIndex = ThumbnailDisplayCount = iThumbnail;
	while (iThumbnail < 4 && iThumbnail < thumbSize) {
	  rthumnam(iThumbnail++);
	}
	StateMap.set(StateFlag::THUMSHO);
	ThumbnailSearchString[0] = 0;
	SetWindowText(ButtonWin->operator[](HBOXSEL), L"");
	auto const blank = std::wstring {};
	displayText::butxt(HBOXSEL, blank);
	vubak();
	StateMap.set(StateFlag::RESTCH);
  }
}

void thred::internal::nuthsel() {
  if (ThumbnailIndex < Thumbnails->size()) {
	auto const savedIndex = ThumbnailIndex;
	auto       iThumbnail = 0U;
	auto const length     = wcslen(std::begin(ThumbnailSearchString));
	StateMap.set(StateFlag::RESTCH);
	if (length != 0U) {
	  while (iThumbnail < 4 && ThumbnailIndex < Thumbnails->size()) {
		if (wcsncmp(std::begin(ThumbnailSearchString), Thumbnails->operator[](ThumbnailIndex).data(), length) == 0) {
		  ThumbnailsSelected[iThumbnail] = ThumbnailIndex;
		  thred::redraw(BackupViewer[iThumbnail++]);
		}
		ThumbnailIndex++;
	  }
	}
	else {
	  while (iThumbnail < 4 && ThumbnailIndex < Thumbnails->size()) {
		ThumbnailsSelected[iThumbnail] = ThumbnailIndex;
		thred::redraw(BackupViewer[iThumbnail++]);
		ThumbnailIndex++;
	  }
	}
	if (iThumbnail != 0U) {
	  ThumbnailDisplayCount = iThumbnail;
	  while (iThumbnail < 4) {
		rthumnam(iThumbnail++);
	  }
	}
	else {
	  ThumbnailIndex = savedIndex;
	}
  }
}

void thred::internal::nuthbak(uint32_t count) {
  if (ThumbnailIndex != 0U) {
	auto const length = wcslen(std::begin(ThumbnailSearchString));
	if (length != 0U) {
	  while ((count != 0U) && ThumbnailIndex < MAXFORMS) {
		if (ThumbnailIndex != 0U) {
		  ThumbnailIndex--;
		  if (wcsncmp(std::begin(ThumbnailSearchString), Thumbnails->operator[](ThumbnailIndex).data(), length) == 0) {
			count--;
		  }
		}
		else {
		  break;
		}
	  }
	}
	else {
	  ThumbnailIndex -= count;
	}
	if (ThumbnailIndex > MAXFORMS) {
	  ThumbnailIndex = 0;
	}
	nuthsel();
  }
}

void thred::internal::nuthum(char character) {
  auto length = wcslen(std::begin(ThumbnailSearchString));
  if (length < 16U) {
	StateMap.set(StateFlag::RESTCH);
	ThumbnailSearchString[length++] = character;
	ThumbnailSearchString[length]   = 0;
	auto txt                        = std::wstring(std::begin(ThumbnailSearchString));
	displayText::butxt(HBOXSEL, txt);
	ThumbnailIndex = 0;
	nuthsel();
  }
}

void thred::internal::bakthum() {
  auto searchStringLength = wcslen(std::begin(ThumbnailSearchString));
  if (searchStringLength != 0U) {
	StateMap.set(StateFlag::RESTCH);
	ThumbnailSearchString[--searchStringLength] = 0;
	ThumbnailIndex                              = 0;
	auto txt                                    = std::wstring(std::begin(ThumbnailSearchString));
	displayText::butxt(HBOXSEL, txt);
	nuthsel();
  }
}

void thred::internal::selalstch() {
  if (!StitchBuffer->empty()) {
	ClosestPointIndex = 0;
	GroupStitchIndex  = gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U);
	GroupStartStitch  = ClosestPointIndex;
	GroupEndStitch    = GroupStitchIndex;
	StateMap.set(StateFlag::GRPSEL);
	StateMap.set(StateFlag::SCROS);
	StateMap.set(StateFlag::ECROS);
	thred::grpAdj();
	StateMap.set(StateFlag::RESTCH);
  }
}

void thred::internal::duinsfil() {
  thred::px2stch();
  auto const offset = fPOINT {SelectedPoint.x - InsertCenter.x, SelectedPoint.y - InsertCenter.y};
  for (auto iForm = InsertedFormIndex; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	auto& formRectangle = FormList->operator[](iForm).rectangle;
	formRectangle.bottom += offset.y;
	formRectangle.top += offset.y;
	formRectangle.left += offset.x;
	formRectangle.right += offset.x;
  }
  for (auto iVertex = InsertedVertexIndex; iVertex < wrap::toUnsigned(FormVertices->size()); iVertex++) {
	FormVertices->operator[](iVertex).x += offset.x;
	FormVertices->operator[](iVertex).y += offset.y;
  }
  for (auto iStitch = InsertedStitchIndex; iStitch < wrap::toUnsigned(StitchBuffer->size()); iStitch++) {
	StitchBuffer->operator[](iStitch).x += offset.x;
	StitchBuffer->operator[](iStitch).y += offset.y;
  }
  StateMap.reset(StateFlag::FRMOF);
  StateMap.set(StateFlag::INIT);
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::gotbox() {
  StateMap.reset(StateFlag::BZUMIN);
  StateMap.reset(StateFlag::BOXSLCT);
  StateMap.reset(StateFlag::FRMPSEL);
  thred::grpAdj();
}

void thred::internal::rngal() {
  if (!StateMap.testAndReset(StateFlag::WASFPNT)) {
	StateMap.reset(StateFlag::GRPSEL);
	auto prng = std::vector<RANGE> {};
	StateMap.reset(StateFlag::GRPSEL);
	auto iStitch     = 0U;
	auto flagInRange = false;
	for (; iStitch < gsl::narrow<decltype(iStitch)>(StitchBuffer->size()); iStitch++) {
	  if (inrng(iStitch)) {
		if (!flagInRange) {
		  prng.push_back(RANGE {iStitch, 0U});
		  flagInRange = true;
		}
	  }
	  else {
		if (flagInRange) {
		  prng.back().finish = iStitch - 1U;
		  flagInRange        = false;
		}
	  }
	}
	if (flagInRange) {
	  prng.back().finish = iStitch - 1U;
	}
	if (!prng.empty()) {
	  auto maximumLength = 0U;
	  auto largestRange  = 0U;
	  for (auto index = 0U; index < prng.size(); index++) {
		auto const length = prng[index].finish - prng[index].start;
		if (length > maximumLength) {
		  maximumLength = length;
		  largestRange  = index;
		}
	  }
	  if (maximumLength != 0U) {
		ClosestPointIndex = prng[largestRange].start;
		GroupStitchIndex  = prng[largestRange].finish;
		StateMap.set(StateFlag::GRPSEL);
	  }
	  gotbox();
	}
  }
}

void thred::internal::nucols() {
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto selectedForm : (*SelectedFormList)) {
	formMap.set(selectedForm);
	auto& form = FormList->operator[](selectedForm);
	if (form.fillType != 0U) {
	  form.fillColor              = gsl::narrow<uint8_t>(ActiveColor);
	  form.fillInfo.feather.color = gsl::narrow<uint8_t>(ActiveColor);
	}
	if (form.edgeType != 0U) {
	  form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
	}
	if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	  form.underlayColor = gsl::narrow<uint8_t>(ActiveColor);
	}
  }
  for (auto& stitch : *StitchBuffer) {
	if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT) && (stitch.attribute & TYPMSK) != TYPMSK) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= ActiveColor;
	}
  }
}

auto thred::internal::dunum(uint32_t code) noexcept -> bool {
  if (code >= '0' && code <= '9') {
	NumericCode = code;
	return true;
  }
  if (code >= VK_NUMPAD0 && code <= VK_NUMPAD9) {
	NumericCode = code - '0';
	return true;
  }
  return false;
}

void thred::stchrct(fRECTANGLE& rectangle) noexcept {
  if (!StitchBuffer->empty()) {
	rectangle.bottom = rectangle.left = 1e10F;
	rectangle.top = rectangle.right = 0;
	for (auto& stitch : *StitchBuffer) {
	  if (stitch.x < rectangle.left) {
		rectangle.left = stitch.x;
	  }
	  if (stitch.x > rectangle.right) {
		rectangle.right = stitch.x;
	  }
	  if (stitch.y < rectangle.bottom) {
		rectangle.bottom = stitch.y;
	  }
	  if (stitch.y > rectangle.top) {
		rectangle.top = stitch.y;
	  }
	}
  }
}

void thred::frmrct(fRECTANGLE& rectangle) noexcept {
  rectangle.left = rectangle.right = FormVertices->front().x;
  rectangle.top = rectangle.bottom = FormVertices->front().y;
  for (auto& FormVertice : *FormVertices) {
	if (FormVertice.x < rectangle.left) {
	  rectangle.left = FormVertice.x;
	}
	if (FormVertice.x > rectangle.right) {
	  rectangle.right = FormVertice.x;
	}
	if (FormVertice.y > rectangle.top) {
	  rectangle.top = FormVertice.y;
	}
	if (FormVertice.y < rectangle.bottom) {
	  rectangle.bottom = FormVertice.y;
	}
  }
}

void thred::internal::desiz() {
  auto  rectangle   = fRECTANGLE {};
  auto  info        = std::wstring {};
  auto& stringTable = *StringTable;

  if (!StitchBuffer->empty()) {
	thred::stchrct(rectangle);
	auto const xSize = (rectangle.right - rectangle.left) / PFGRAN;
	auto const ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
	if ((rectangle.left < 0) || (rectangle.bottom < 0) || (rectangle.right > IniFile.hoopSizeX) ||
	    (rectangle.top > IniFile.hoopSizeY)) {
	  info += stringTable[STR_STCHOUT];
	}
	info += fmt::format(
	    stringTable[STR_STCHS], wrap::toUnsigned(StitchBuffer->size()), xSize, (xSize / 25.4F), ySize, (ySize / 25.4F));
  }
  if (!FormList->empty()) {
	thred::frmrct(rectangle);
	auto const xSize = (rectangle.right - rectangle.left) / PFGRAN;
	auto const ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
	info += fmt::format(stringTable[STR_FORMS], FormList->size(), xSize, (xSize / 25.4F), ySize, (ySize / 25.4F));
  }
  info += fmt::format(stringTable[STR_HUPWID], (IniFile.hoopSizeX / PFGRAN), (IniFile.hoopSizeY / PFGRAN));
  if (!StitchBuffer->empty()) {
	auto modifier = utf::Utf8ToUtf16(std::string(std::begin(ExtendedHeader->modifierName)));
	info += fmt::format(stringTable[STR_CREATBY], *DesignerName, modifier);
  }
  displayText::shoMsg(info);
}

void thred::internal::sidhup() {
  auto hoopRectangle        = RECT {0L, 0L, 0L, 0L};
  auto preferencesRectangle = RECT {0L, 0L, 0L, 0L};

  StateMap.set(StateFlag::HUPMSG);
  GetWindowRect(ValueWindow->operator[](PHUP), &hoopRectangle);
  GetWindowRect(PreferencesWindow, &preferencesRectangle);
  SideMessageWindow = CreateWindow(L"STATIC", // NOLINT
                                   nullptr,
                                   WS_BORDER | WS_CHILD | WS_VISIBLE,
                                   preferencesRectangle.right + 3 - ThredWindowOrigin.x,
                                   hoopRectangle.top - ThredWindowOrigin.y,
                                   ButtonWidthX3 + ButtonWidth * 2 + 6,
                                   ButtonHeight * HUPS + 6,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  for (auto iHoop = 0U; iHoop < HUPS; iHoop++) {
	SideWindow[iHoop] = CreateWindow(L"STATIC", // NOLINT
	                                 StringTable->operator[](wrap::toSize(iHoop) + STR_HUP0).c_str(),
	                                 SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                 3,
	                                 ButtonHeight * iHoop + 3,
	                                 ButtonWidthX3 + ButtonWidth * 2,
	                                 ButtonHeight,
	                                 SideMessageWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
  }
}

void thred::internal::setpcs() {
  IniFile.auxFileType = AUXPCS;
  auxmen();
}

#if PESACT
void thred::internal::setpes() {
  IniFile.auxFileType = AUXPES;
  auxmen();
}
#endif

void thred::internal::setdst() {
  IniFile.auxFileType = AUXDST;
  auxmen();
}

void thred::internal::fop() {
  trace::untrace();
  if (!FormList->empty() || (!StitchBuffer->empty())) {
	if (savcmp()) {
	  nuFil();
	  nulayr(0);
	}
	else {
	  displayText::savdisc();
	  StateMap.set(StateFlag::OSAV);
	}
  }
  else {
	nuFil();
	nulayr(0);
  }
}

void thred::internal::clpradj(fRECTANGLE& clipRectAdjusted, fPOINTATTR const& stitch) noexcept {
  if (stitch.x < clipRectAdjusted.left) {
	clipRectAdjusted.left = stitch.x;
  }
  if (stitch.x > clipRectAdjusted.right) {
	clipRectAdjusted.right = stitch.x;
  }
  if (stitch.y < clipRectAdjusted.bottom) {
	clipRectAdjusted.bottom = stitch.y;
  }
  if (stitch.y > clipRectAdjusted.top) {
	clipRectAdjusted.top = stitch.y;
  }
}

void thred::internal::clpadj() {
  if (StateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	auto iStitch          = GroupStartStitch;
	auto ClipRectAdjusted = fRECTANGLE {StitchBuffer->operator[](iStitch).x,
	                                    StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y,
	                                    StitchBuffer->operator[](iStitch).x,
	                                    StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y};
	iStitch++;
	while (iStitch < GroupEndStitch) {
	  clpradj(ClipRectAdjusted, StitchBuffer->operator[](iStitch++));
	}
	if (StitchBuffer->operator[](iStitch).x < ClipRectAdjusted.left) {
	  ClipRectAdjusted.left = StitchBuffer->operator[](iStitch).x;
	}
	if (StitchBuffer->operator[](iStitch).x > ClipRectAdjusted.right) {
	  ClipRectAdjusted.right = StitchBuffer->operator[](iStitch).x;
	}
	auto const clipMiddle = form::midl(ClipRectAdjusted.right, ClipRectAdjusted.left);
	StitchBuffer->operator[](GroupStartStitch).y = form::midl(ClipRectAdjusted.top, ClipRectAdjusted.bottom);
	StitchBuffer->operator[](GroupEndStitch).y = StitchBuffer->operator[](GroupStartStitch).y;
	if (StitchBuffer->operator[](GroupStartStitch).x < clipMiddle) {
	  StitchBuffer->operator[](GroupStartStitch).x = ClipRectAdjusted.left;
	  StitchBuffer->operator[](GroupEndStitch).x   = ClipRectAdjusted.right;
	}
	else {
	  StitchBuffer->operator[](GroupEndStitch).x   = ClipRectAdjusted.left;
	  StitchBuffer->operator[](GroupStartStitch).x = ClipRectAdjusted.right;
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_GRPMSG, IDS_RNGEND);
  }
}

void thred::internal::shftflt(fPOINT point) noexcept {
  if (point.x <= ZoomRect.left || point.x >= ZoomRect.right || point.y <= ZoomRect.bottom ||
      point.y >= ZoomRect.top) {
	thred::shft(point);
  }
}

void thred::internal::fil2men() {
  if (UserFlagMap.test(UserFlag::FIL2OF)) {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_UNCHECKED);
  }
}

void thred::internal::defpref() {
  UserFlagMap.reset();
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	UserColor[iColor]              = DefaultUserColors[iColor];
	CustomColor[iColor]            = DefaultCustomColors[iColor];
	CustomBackgroundColor[iColor]  = DefaultCustomBackgroundColors[iColor];
	BitmapBackgroundColors[iColor] = DefaultBitmapBackgroundColors[iColor];
  }
  formForms::dazdef();
  AppliqueColor          = 15U;
  IniFile.AppStitchLen   = APSPAC;
  BorderWidth            = BRDWID;
  ButtonholeCornerLength = IBFCLEN;
  IniFile.chainSpace     = CHSDEF;
  IniFile.chainRatio     = CHRDEF;
  IniFile.fillAngle      = DEFANG;
  UserFlagMap.reset(UserFlag::SQRFIL);
  LineSpacing             = DEFSPACE * PFGRAN;
  ShowStitchThreshold     = SHOPNTS;
  IniFile.gridSize        = 12.0F;
  IniFile.hoopType        = LARGHUP;
  IniFile.hoopSizeX       = LHUPX;
  IniFile.hoopSizeY       = LHUPY;
  IniFile.cursorNudgeStep = NUGINI;
  IniFile.nudgePixels     = DEFPIX;
  UserFlagMap.set(UserFlag::BLUNT);
  SmallStitchLength       = SMALSIZ * PFGRAN;
  SnapLength              = SNPLEN * PFGRAN;
  SpiralWrap              = SPIRWRAP;
  StarRatio               = STARAT;
  StitchBoxesThreshold    = STCHBOX;
  IniFile.maxStitchLength = MAXSIZ * PFGRAN;
  UserStitchLength        = USESIZ * PFGRAN;
  MinStitchLength         = MINSIZ * PFGRAN;
  IniFile.clipOffset      = 0;
  IniFile.fillPhase       = 0;
  BitmapColor             = 0xc8dfee;
  if (IniFile.customHoopX == 0.0F) {
	IniFile.customHoopX = LHUPX;
  }
  if (IniFile.customHoopY == 0.0F) {
	IniFile.customHoopY = LHUPY;
  }
  PicotSpacing = IPICSPAC;
  UserFlagMap.set(UserFlag::FIL2OF);
  fil2men();
  BackgroundColor              = 0xa8c4b1;
  UnzoomedRect.x               = wrap::round<int32_t>(IniFile.hoopSizeX);
  UnzoomedRect.y               = wrap::round<int32_t>(IniFile.hoopSizeY);
  IniFile.waveEnd              = IWAVEND;
  IniFile.wavePoints           = IWAVPNTS;
  IniFile.waveLobes            = IWAVS;
  IniFile.waveStart            = IWAVSTRT;
  IniFile.featherFillType      = FDEFTYP;
  IniFile.featherUpCount       = FDEFUP;
  IniFile.featherDownCount     = FDEFDWN;
  IniFile.featherRatio         = FDEFRAT;
  IniFile.featherMinStitchSize = FDEFFLR;
  IniFile.featherCount         = FDEFNUM;
  IniFile.underlayStitchLen    = DEFULEN;
  IniFile.underlaySpacing      = DEFUSPAC;
  IniFile.formBoxSizePixels    = DEFBPIX;
  IniFile.textureHeight        = ITXHI;
  IniFile.textureWidth         = ITXWID;
  IniFile.textureSpacing       = ITXSPAC;
  IniFile.lensRatio            = DEFLRAT;
}

void thred::internal::dumrk(float xCoord, float yCoord) {
  if (StateMap.testAndReset(StateFlag::GMRK)) {
	drwmrk(StitchWindowDC);
  }
  ZoomMarkPoint.x = xCoord;
  ZoomMarkPoint.y = yCoord;
  StateMap.set(StateFlag::INIT);
  StateMap.set(StateFlag::GMRK);
  drwmrk(StitchWindowDC);
  StateMap.set(StateFlag::WASMRK);
}

void thred::internal::gselrng() noexcept {
  auto const& selectedFormList = *SelectedFormList;

  SelectedFormsRange.start = SelectedFormsRange.finish = selectedFormList[0];
  for (auto selectedForm : selectedFormList) {
	if (selectedForm < SelectedFormsRange.start) {
	  SelectedFormsRange.start = selectedForm;
	}
	if (selectedForm > SelectedFormsRange.finish) {
	  SelectedFormsRange.finish = selectedForm;
	}
  }
}

auto thred::internal::nuang(float yDelta, float xDelta) noexcept -> float {
  auto const angle         = atan2(yDelta, xDelta);
  auto       relativeAngle = angle - OriginalAngle;
  if (fabs(relativeAngle) > PI_F) {
	if (relativeAngle > 0.0F) {
	  relativeAngle = 2.0F * PI_F - relativeAngle;
	}
	else {
	  relativeAngle += 2.0F * PI_F;
	}
  }
  return relativeAngle;
}

void thred::internal::angdif(float angle) noexcept {
  if (angle > HighestAngle) {
	HighestAngle = angle;
  }
  else {
	if (angle < LowestAngle) {
	  LowestAngle = angle;
	}
  }
}

void thred::internal::rotmrk() {
  if (StateMap.test(StateFlag::GMRK) && (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL))) {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  auto const codedFormIndex = ClosestFormToCursor << FRMSHFT;
	  LowestAngle = HighestAngle      = 0.0F;
	  auto& form                      = FormList->operator[](ClosestFormToCursor);
	  auto                   vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	  OriginalAngle = atan2(vertexIt[0].y - ZoomMarkPoint.y, vertexIt[0].x - ZoomMarkPoint.x);
	  for (auto iVertex = 1U; iVertex < form.vertexCount; iVertex++) {
		angdif(nuang(vertexIt[iVertex].y - ZoomMarkPoint.y, vertexIt[iVertex].x - ZoomMarkPoint.x));
	  }
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) == codedFormIndex) {
		  angdif(nuang(stitch.y - ZoomMarkPoint.y, stitch.x - ZoomMarkPoint.x));
		}
	  }
	}
	else {
	  thred::rngadj();
	  LowestAngle   = 0.0F;
	  HighestAngle  = 0.0F;
	  OriginalAngle = atan2(StitchBuffer->operator[](GroupStartStitch).y - ZoomMarkPoint.y,
	                        StitchBuffer->operator[](GroupStartStitch).x - ZoomMarkPoint.x);
	  for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; iStitch++) {
		angdif(nuang(StitchBuffer->operator[](iStitch).y - ZoomMarkPoint.y,
		             StitchBuffer->operator[](iStitch).x - ZoomMarkPoint.x));
	  }
	}
	auto const tAngle     = HighestAngle - LowestAngle;
	auto const segments   = std::round(2.0F * PI_F / tAngle);
	IniFile.rotationAngle = 2.0F * PI_F / segments;
	auto fmtStr           = std::wstring {};
	displayText::loadString(fmtStr, IDS_ROTMARK);
	// ToDo - should this be IniFile.rotationAngle?
	displayText::shoMsg(fmt::format(fmtStr, IniFile.fillAngle * 180.0F / PI_F, segments));
  }
  else {
	displayText::shoseln(IDS_FSZ, IDS_SETROTM);
  }
}

void thred::internal::segentr(float rotationAngle) {
  if (rotationAngle == 0.0F) {
	rotationAngle = PI_F / 180.0F;
  }
  displayText::shoMsg(fmt::format(StringTable->operator[](STR_ENTROT), (2.0F * PI_F / rotationAngle)));
  StateMap.set(StateFlag::NUMIN);
  displayText::numWnd();
}

void thred::internal::rotseg() {
  segentr(IniFile.rotationAngle);
  StateMap.set(StateFlag::ENTRSEG);
}

void thred::internal::pntmrk() {
  do {
	if (StateMap.test(StateFlag::SELBOX)) {
	  dumrk(StitchBuffer->operator[](ClosestPointIndex).x,
	        StitchBuffer->operator[](ClosestPointIndex).y);
	  break;
	}
	if (StateMap.test(StateFlag::FRMPSEL)) {
	  auto vertexIt = std::next(FormVertices->cbegin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
	  auto const& vertex = vertexIt[ClosestVertexToCursor];
	  dumrk(vertex.x, vertex.y);
	  break;
	}
	displayText::shoseln(IDS_STCH_FRM, IDS_SETMRK);
  } while (false);
}

void thred::internal::filfrms() {
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  form::refilfn();
	}
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  form::refil();
	  StateMap.set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::nuslst(uint32_t find) {
  gselrng();
  // ToDo - Check this code. Does it do what is intended?
  do {
	if (find < SelectedFormsRange.start) {
	  for (auto form = find; form < SelectedFormsRange.finish; form++) {
		SelectedFormList->push_back(form);
	  }
	  break;
	}
	if (find > SelectedFormsRange.finish) {
	  for (auto form = SelectedFormsRange.start; form <= find; form++) {
		SelectedFormList->push_back(form);
	  }
	  break;
	}
	for (auto form = SelectedFormsRange.start; form <= find; form++) {
	  SelectedFormList->push_back(form);
	}
  } while (false);
}

void thred::internal::srchk() {
  StateMap.reset(StateFlag::FORMSEL);
  SelectedFormList->clear();
  if (StateMap.testAndSet(StateFlag::LENSRCH)) {
	if (StateMap.test(StateFlag::WASGRP)) {
	  ClosestPointIndex = GroupStartStitch = PrevGroupStartStitch;
	  GroupStitchIndex = GroupEndStitch = PrevGroupEndStitch;
	}
	else {
	  StateMap.reset(StateFlag::GRPSEL);
	}
  }
  else {
	if (StateMap.test(StateFlag::GRPSEL)) {
	  StateMap.set(StateFlag::WASGRP);
	  thred::rngadj();
	  PrevGroupStartStitch = GroupStartStitch;
	  PrevGroupEndStitch   = GroupEndStitch;
	}
	else {
	  StateMap.reset(StateFlag::WASGRP);
	}
  }
  duselrng();
}

constexpr auto thred::internal::byteSwap(uint32_t data) noexcept -> uint32_t {
  auto const a = ((data & 0x000000FFU) << 24U) | ((data & 0x0000FF00U) << 8U) |
                 ((data & 0x00FF0000U) >> 8U) | ((data & 0xFF000000U) >> 24U);
  return a;
}

void thred::internal::ritcur() noexcept {
  auto currentCursor = GetCursor();
  if (currentCursor != nullptr) {
	auto iconInfo = ICONINFO {FALSE, 0U, 0U, nullptr, nullptr};
	GetIconInfo(currentCursor, &iconInfo);
	auto cursorPosition = POINT {0L, 0L};
	GetCursorPos(&cursorPosition);
	cursorPosition.x -= (StitchWindowOrigin.x + iconInfo.xHotspot);
	cursorPosition.y -= (StitchWindowOrigin.y + iconInfo.yHotspot);
	// ToDo - replace with GetDIBits
	uint8_t bitmapBits[64] = {};
	GetBitmapBits(iconInfo.hbmMask, 256, &bitmapBits);
	if (currentCursor == ArrowCursor) {
	  for (auto iRow = 0; iRow < 32; iRow++) {
		auto const mask          = byteSwap(bitmapBits[iRow]);
		auto const bitmapInverse = byteSwap(bitmapBits[iRow + 32]);
		auto       bitMask       = 0x80000000U;
		for (auto iPixel = 0; iPixel < 32; iPixel++) {
		  if ((bitMask & mask) == 0U) {
			auto pixelColor = 0U;
			if ((bitMask & bitmapInverse) != 0U) {
			  pixelColor = 0xffffff;
			}
			else {
			  pixelColor = 0x000000;
			}
			SetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow, pixelColor);
		  }
		  bitMask >>= 1U;
		}
	  }
	}
	else {
	  for (auto iRow = 0; iRow < 32; iRow++) {
		auto const bitmapInverse = byteSwap(bitmapBits[iRow + 32]);
		auto       bitMask       = 0x80000000U;
		for (auto iPixel = 0; iPixel < 32; iPixel++) {
		  if ((bitMask & bitmapInverse) != 0U) {
			SetPixel(StitchWindowDC,
			         cursorPosition.x + iPixel,
			         cursorPosition.y + iRow,
			         GetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow) ^ 0xffffffU);
		  }
		  bitMask >>= 1U;
		}
	  }
	}
  }
}

void thred::internal::nedon() {
  UserFlagMap.reset(UserFlag::NEDOF);
  nedmen();
}

void thred::internal::nedof() {
  UserFlagMap.set(UserFlag::NEDOF);
  nedmen();
}

void thred::internal::shoknot() {
  UserFlagMap.reset(UserFlag::KNOTOF);
  knotmen();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::hidknot() {
  UserFlagMap.set(UserFlag::KNOTOF);
  knotmen();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::pcsbsavon() {
  UserFlagMap.reset(UserFlag::BSAVOF);
  bsavmen();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::pcsbsavof() {
  UserFlagMap.set(UserFlag::BSAVOF);
  bsavmen();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::tglhid() {
  if (!StateMap.testAndFlip(StateFlag::HIDSTCH)) {
	StateMap.reset(StateFlag::FRMOF);
  }
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::respac(FRMHED& form) noexcept {
  if (clip::isclp(form)) {
	form.fillSpacing = LineSpacing;
	form::fsizpar(form);
  }
}

auto thred::internal::chkminus(uint32_t code) noexcept -> bool {
  if (code == 189 || code == 109) { // '-' key pressed
	if (PreferenceIndex == PFAZ) {  // Clipboard Offset in preferences
	  return true;
	}
	if (FormMenuChoice == LFRMSPAC && isfclp()) { // Fill Spacing for fan clip fill
	  return true;
	}
	if (FormMenuChoice == LWLKIND) { // Indent
	  return true;
	}
  }
  return false;
}

void thred::internal::retrac() {
  if (StateMap.test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	if (GroupStartStitch == 0U) {
	  GroupStartStitch++;
	}
	auto const count = GroupEndStitch - GroupStartStitch;
	auto insertPoint = std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch) + 1U);
	auto startPoint = std::next(StitchBuffer->rbegin(), StitchBuffer->size() - GroupEndStitch);
	auto endPoint   = std::next(startPoint, count);
	StitchBuffer->insert(insertPoint, startPoint, endPoint);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_GRPMSG, IDS_RETRAC);
  }
}

void thred::internal::setgrd(COLORREF color) {
  GRDCOD const gridCodes[] = {
      {ID_GRDHI, HIGRD},
      {ID_GRDMED, MEDGRD},
      {ID_GRDEF, DEFGRD},
      {ID_GRDRED, REDGRD},
      {ID_GRDBLU, BLUGRD},
      {ID_GRDGRN, GRNGRD},
  };

  for (auto const gridCode : gridCodes) {
	if (color == gridCode.col) {
	  CheckMenuItem(MainMenu, gridCode.id, MF_CHECKED);
	}
	else {
	  CheckMenuItem(MainMenu, gridCode.id, MF_UNCHECKED);
	}
  }
  GridPen           = nuPen(GridPen, 1, color);
  IniFile.gridColor = color;
  StateMap.set(StateFlag::RESTCH);
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::ovrlay() {
  StateMap.reset(StateFlag::IGNORINS);
  insfil();
  StateMap.reset(StateFlag::INSFIL);
  StateMap.reset(StateFlag::FRMOF);
  StateMap.set(StateFlag::INIT);
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::fil2sel(uint32_t stat) {
  UserFlagMap.set(UserFlag::FIL2OF);
  if (stat != 0U) {
	UserFlagMap.reset(UserFlag::FIL2OF);
  }
  fil2men();
}

void thred::internal::rotauxmen() {
  if (UserFlagMap.test(UserFlag::ROTAUX)) {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_CHECKED);
  }
}

void thred::internal::rotauxsel(uint32_t stat) {
  UserFlagMap.set(UserFlag::ROTAUX);
  if (stat == 0U) {
	UserFlagMap.reset(UserFlag::ROTAUX);
  }
  rotauxmen();
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::frmcurmen() {
  if (UserFlagMap.test(UserFlag::FRMX)) {
	CheckMenuItem(MainMenu, ID_FRMX, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FRMX, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_CHECKED);
  }
}

void thred::internal::frmcursel(uint32_t cursorType) {
  UserFlagMap.set(UserFlag::FRMX);
  if (cursorType == 0U) {
	UserFlagMap.reset(UserFlag::FRMX);
  }
  frmcurmen();
  StateMap.set(StateFlag::DUMEN);
}

void thred::internal::stchsnap(uint32_t start, uint32_t finish) {
  auto pnt = std::next(StitchBuffer->begin(), start);

  for (auto i = 0U; i < finish - start; i++) {
	pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
	pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;

	pnt++;
  }
}

void thred::internal::frmsnap(uint32_t start, uint32_t count) {
  auto vertexIt = std::next(FormVertices->begin(), start);

  for (auto i = 0U; i < count; i++) {
	vertexIt[i].x = rintf(vertexIt[i].x / IniFile.gridSize) * IniFile.gridSize;
	vertexIt[i].y = rintf(vertexIt[i].y / IniFile.gridSize) * IniFile.gridSize;
  }
}

void thred::internal::gsnap() {
  if (StateMap.test(StateFlag::TXTRED)) {
	texture::txsnap();
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& formIter      = FormList->operator[](ClosestFormToCursor);
	  frmsnap(formIter.vertexIndex, formIter.vertexCount);
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	}
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto& formIter = FormList->operator[](ClosestFormToCursor);
	  frmsnap(formIter.vertexIndex, formIter.vertexCount);
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	  StateMap.set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap.test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		stchsnap(GroupStartStitch, GroupEndStitch + 1U);
		StateMap.set(StateFlag::RESTCH);
	  }
	  else {
		displayText::shoseln(IDS_FGRPF, IDS_SNAP2GRD);
	  }
	}
  }
}

void thred::internal::ritlock(WIN32_FIND_DATA const* fileData, uint32_t fileIndex, HWND hwndlg) noexcept {
  if (fileData != nullptr) {
	SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_RESETCONTENT, 0, 0);
	SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_RESETCONTENT, 0, 0);
	for (auto iFile = 0U; iFile < fileIndex; iFile++) {
	  if ((fileData[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0U) { // NOLINT
		GSL_SUPPRESS(26490)
		SendMessage(GetDlgItem(hwndlg, IDC_LOCKED),
		            LB_ADDSTRING,
		            0,
		            reinterpret_cast<LPARAM>(fileData[iFile].cFileName)); // NOLINT
	  }
	  else {
		GSL_SUPPRESS(26490)
		SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED),
		            LB_ADDSTRING,
		            0,
		            reinterpret_cast<LPARAM>(fileData[iFile].cFileName)); // NOLINT
	  }
	}
  }
}

auto CALLBACK thred::internal::LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  auto fileInfo = gsl::narrow_cast<FINDINFO*>(nullptr);

  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  SetWindowLongPtr(hwndlg, DWLP_USER, lparam);
	  GSL_SUPPRESS(26490) fileInfo = reinterpret_cast<FINDINFO*>(lparam); // NOLINT
	  if (fileInfo != nullptr) {
		auto searchName   = *DefaultDirectory / L"*.thr";
		auto searchResult = FindFirstFile(searchName.wstring().c_str(), &(fileInfo->data[0]));
		if (searchResult == INVALID_HANDLE_VALUE) { // NOLINT
		  auto fmtStr = std::wstring {};
		  displayText::loadString(fmtStr, IDS_NOTHRFIL);
		  displayText::shoMsg(fmt::format(fmtStr, DefaultDirectory->wstring()));
		  EndDialog(hwndlg, wparam);
		  return TRUE;
		}
		fileInfo->count = 1;
		while (FindNextFile(searchResult, &fileInfo->data[fileInfo->count++])) {
		  ;
		}
		fileInfo->count--;
		ritlock(fileInfo->data, fileInfo->count, hwndlg);
	  }
	  break;
	}
	case WM_COMMAND: {
	  GSL_SUPPRESS(26490)
	  fileInfo = reinterpret_cast<FINDINFO*>(GetWindowLongPtr(hwndlg, DWLP_USER)); // NOLINT
	  if (fileInfo != nullptr) {
		switch (LOWORD(wparam)) { // NOLINT
		  case IDCANCEL: {
			EndDialog(hwndlg, wparam);
			return TRUE;
		  }
		  case IDC_LOCKAL: {
			for (auto iFile = 0U; iFile < fileInfo->count; iFile++) {
			  fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY; // NOLINT
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDC_UNLOCKAL: {
			for (auto iFile = 0U; iFile < fileInfo->count; iFile++) {
			  fileInfo->data[iFile].dwFileAttributes &= 0xffffffffU ^ FILE_ATTRIBUTE_READONLY; // NOLINT
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDC_LOCK: {
			auto fileError    = 0U;
			auto unlockHandle = GetDlgItem(hwndlg, IDC_UNLOCKED);
			for (auto iFile = 0U; iFile < fileInfo->count; iFile++) {
			  if ((fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0U) { // NOLINT
				if (SendMessage(unlockHandle, LB_GETSEL, fileError, 0)) {
				  fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY; // NOLINT
				}
				fileError++;
			  }
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDC_UNLOCK: {
			auto fileError  = 0U;
			auto lockHandle = GetDlgItem(hwndlg, IDC_LOCKED);
			for (auto iFile = 0U; iFile < fileInfo->count; iFile++) {
			  if ((fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0U) { // NOLINT
				if (SendMessage(lockHandle, LB_GETSEL, fileError, 0)) {
				  fileInfo->data[iFile].dwFileAttributes &= 0xffffffffU ^ FILE_ATTRIBUTE_READONLY; // NOLINT
				}
				fileError++;
			  }
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDOK: {
			auto fileError = 0U;
			for (auto iFile = 0U; iFile < fileInfo->count; iFile++) {
			  auto fileName = *DefaultDirectory / std::begin(fileInfo->data[iFile].cFileName);
			  if (!SetFileAttributes(fileName.wstring().c_str(), fileInfo->data[iFile].dwFileAttributes)) {
				fileError++;
			  }
			}
			if (fileError != 0U) {
			  auto fmtStr = std::wstring {};
			  displayText::loadString(fmtStr, IDS_LOCKNOT);
			  displayText::shoMsg(fmt::format(fmtStr, fileError));
			}
			EndDialog(hwndlg, wparam);
			return TRUE;
		  }
		}
	  }
	}
	default: {
	}
  }
  return FALSE;
}

void thred::internal::lock() {
  // ToDo - investigate C++17 option shown here: http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
  auto lockInfo = FINDINFO {};

  lockInfo.count = 0;
  // ToDo - Replace 512 with maximum files in subdirectory
  lockInfo.data = new WIN32_FIND_DATA[512]; // NOLINT

  GSL_SUPPRESS(26490)
  DialogBoxParam(
      ThrEdInstance, MAKEINTRESOURCE(IDD_DLOCK), ThrEdWindow, LockPrc, reinterpret_cast<LPARAM>(&lockInfo)); // NOLINT

  delete[] lockInfo.data; // NOLINT
}

void thred::internal::delstch() {
  thred::savdo();
  StitchBuffer->clear();
  StitchBuffer->shrink_to_fit();
  TexturePointsBuffer->clear();
  TexturePointsBuffer->shrink_to_fit();
  rstAll();
  form::clrfills();
  ColorChanges     = 0;
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  displayText::butxt(HTOT, blank);
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::chkbit() {
  if ((PCSBMPFileName[0] != 0) && (StateMap.test(StateFlag::WASDIF) || StateMap.test(StateFlag::WASDSEL) ||
                                   StateMap.test(StateFlag::WASBLAK))) {
	StateMap.set(StateFlag::WASESC);
	bfil();
  }
}

void thred::internal::delmap() {
  PCSBMPFileName[0] = 0;
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::closfn() {
  deltot();
  KnotCount = 0;
  WorkingFileName->clear();
  PCSBMPFileName[0] = 0;
  deldu();
  displayText::clrhbut(3);
  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
}

void thred::internal::filclos() {
  if (savcmp()) {
	closfn();
  }
  else {
	StateMap.set(StateFlag::FCLOS);
	displayText::savdisc();
  }
}

void thred::internal::frmpos(FRMHED& form, float deltaX, float deltaY) {
  auto vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	vertexIt[iVertex].x += deltaX;
	vertexIt[iVertex].y += deltaY;
  }
  form.rectangle.bottom += deltaY;
  form.rectangle.top += deltaY;
  form.rectangle.left += deltaX;
  form.rectangle.right += deltaX;
}

void thred::internal::nudgfn(float deltaX, float deltaY) {
  if (StateMap.test(StateFlag::BIGBOX) || !SelectedFormList->empty() || StateMap.test(StateFlag::FORMSEL) ||
      StateMap.test(StateFlag::GRPSEL) || StateMap.test(StateFlag::SELBOX)) {
	thred::savdo();
  }
  if (StateMap.test(StateFlag::BIGBOX)) {
	for (auto& formIt : *FormList) {
	  frmpos(formIt, deltaX, deltaY);
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.x += deltaX;
	  stitch.y += deltaY;
	}
	AllItemsRect.bottom += deltaY;
	AllItemsRect.top += deltaY;
	AllItemsRect.left += deltaX;
	AllItemsRect.right += deltaX;
	form::stchrct2px(AllItemsRect, SelectedFormsRect);
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (!SelectedFormList->empty()) {
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto selectedForm : (*SelectedFormList)) {
	  formMap.set(selectedForm);
	}
	for (auto& stitch : *StitchBuffer) {
	  if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
		stitch.x += deltaX;
		stitch.y += deltaY;
	  }
	}
	for (auto selectedForm : (*SelectedFormList)) {
	  auto& form = FormList->operator[](selectedForm);
	  frmpos(form, deltaX, deltaY);
	}
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (StateMap.test(StateFlag::FORMSEL)) {
	auto& form = FormList->operator[](ClosestFormToCursor);
	frmpos(form, deltaX, deltaY);
	if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
		  stitch.x += deltaX;
		  stitch.y += deltaY;
		}
	  }
	}
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (StateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
	  StitchBuffer->operator[](iStitch).x += deltaX;
	  StitchBuffer->operator[](iStitch).y += deltaY;
	}
	thred::grpAdj();
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (StateMap.test(StateFlag::SELBOX)) {
	StitchBuffer->operator[](ClosestPointIndex).x += deltaX;
	StitchBuffer->operator[](ClosestPointIndex).y += deltaY;
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  auto pixel = POINT {0L, 0L};
  if (deltaX != 0.0F) {
	if (deltaX > 0) {
	  pixel.x = IniFile.nudgePixels;
	}
	else {
	  pixel.x = -IniFile.nudgePixels;
	}
  }
  if (deltaY != 0.0F) {
	if (deltaY > 0) {
	  pixel.y = -IniFile.nudgePixels;
	}
	else {
	  pixel.y = +IniFile.nudgePixels;
	}
  }
  // ToDo - use SendInput instead
  mouse_event(MOUSEEVENTF_MOVE, pixel.x, pixel.y, 0, 0);
}

void thred::internal::pixmsg(uint32_t iString, uint32_t pixelCount) {
  displayText::shoMsg(fmt::format(StringTable->operator[](iString), pixelCount));
}

void thred::internal::getnpix() {
  pixmsg(STR_NUDG, IniFile.nudgePixels);
  StateMap.set(StateFlag::NUMIN);
  StateMap.set(StateFlag::PIXIN);
  displayText::numWnd();
}

void thred::internal::getstpix() {
  pixmsg(STR_STCHP, IniFile.stitchSizePixels);
  StateMap.set(StateFlag::NUMIN);
  StateMap.set(StateFlag::STPXIN);
  displayText::numWnd();
}

void thred::internal::getfrmpix() {
  pixmsg(STR_FRMP, IniFile.formVertexSizePixels);
  StateMap.set(StateFlag::NUMIN);
  StateMap.set(StateFlag::FRMPXIN);
  displayText::numWnd();
}

void thred::internal::getfrmbox() {
  pixmsg(STR_FRMBOX, IniFile.formBoxSizePixels);
  StateMap.set(StateFlag::NUMIN);
  StateMap.set(StateFlag::FRMBOXIN);
  displayText::numWnd();
}

void thred::internal::bakmrk() {
  if (StateMap.test(StateFlag::WASMRK)) {
	if (ZoomMarkPoint.x > IniFile.hoopSizeX) {
	  ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
	}
	if (ZoomMarkPoint.y > IniFile.hoopSizeY) {
	  ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
	}
	dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
	fPOINT const point(ZoomMarkPoint);
	thred::shft(point);
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_MRK);
  }
}

void thred::internal::nuscol(uint32_t iColor) noexcept {
  UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
  UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
  thred::redraw(UserColorWin->operator[](iColor));
}

void thred::internal::movchk() {
  auto& defaultColorWin = *DefaultColorWin;
  if ((Msg.wParam & MK_LBUTTON) != 0U) { // NOLINT
	if (!StateMap.testAndSet(StateFlag::WASMOV)) {
	  if (thi::chkMsgs(Msg.pt, defaultColorWin.front(), defaultColorWin.back())) {
		DraggedColor = VerticalIndex & 0xfU;
		StateMap.set(StateFlag::WASCOL);
	  }
	}
  }
  else {
	if (StateMap.testAndReset(StateFlag::WASMOV) && StateMap.testAndReset(StateFlag::WASCOL)) {
	  if (thi::chkMsgs(Msg.pt, defaultColorWin.front(), defaultColorWin.back())) {
		auto const key          = wrap::pressed(VK_SHIFT);
		auto const switchColors = wrap::pressed(VK_CONTROL);
		for (auto& stitch : *StitchBuffer) {
		  auto const color = stitch.attribute & COLMSK;
		  if (color == VerticalIndex) {
			stitch.attribute &= NCOLMSK;
			stitch.attribute |= DraggedColor;
		  }
		  else {
			if (key && color == DraggedColor) {
			  stitch.attribute &= NCOLMSK;
			  stitch.attribute |= VerticalIndex;
			}
		  }
		}
		for (auto& formIter : *FormList) {
		  if (formIter.fillType != 0U) {
			if (formIter.fillColor == VerticalIndex) {
			  formIter.fillColor = gsl::narrow<uint8_t>(DraggedColor);
			}
			else {
			  if (key && formIter.fillColor == DraggedColor) {
				formIter.fillColor = gsl::narrow<uint8_t>(VerticalIndex);
			  }
			}
			if (formIter.fillInfo.feather.color == VerticalIndex) {
			  formIter.fillInfo.feather.color = gsl::narrow<uint8_t>(DraggedColor);
			}
			else {
			  if (key && formIter.fillInfo.feather.color == DraggedColor) {
				formIter.fillInfo.feather.color = gsl::narrow<uint8_t>(VerticalIndex);
			  }
			}
		  }
		  if (formIter.edgeType != 0U) {
			if (formIter.borderColor == VerticalIndex) {
			  formIter.borderColor = gsl::narrow<uint8_t>(DraggedColor);
			}
			else {
			  if (key && formIter.borderColor == DraggedColor) {
				formIter.borderColor = gsl::narrow<uint8_t>(VerticalIndex);
			  }
			}
		  }
		}
		if (switchColors) {
		  auto const swapColor     = UserColor[VerticalIndex];
		  UserColor[VerticalIndex] = UserColor[DraggedColor];
		  if (key) {
			UserColor[DraggedColor] = swapColor;
			nuscol(DraggedColor);
		  }
		  nuscol(VerticalIndex);
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	  }
	}
  }
}

void thred::internal::inscol() {
  auto colorMap = boost::dynamic_bitset<>(16U);
  VerticalIndex &= COLMSK;
  for (auto& stitch : *StitchBuffer) {
	colorMap.set(stitch.attribute & COLMSK);
  }
  if (colorMap.all()) {
	displayText::tabmsg(IDS_COLAL);
  }
  else {
	auto nextColor = 15U;
	while (colorMap.test(nextColor)) {
	  nextColor--;
	}
	for (auto& stitch : *StitchBuffer) {
	  auto const color = stitch.attribute & COLMSK;
	  if (color >= VerticalIndex && color < nextColor) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color + 1U;
	  }
	}
	for (auto& formIter : *FormList) {
	  if (formIter.fillType != 0U) {
		if (formIter.fillColor >= VerticalIndex && formIter.fillColor < nextColor) {
		  formIter.fillColor++;
		}
		if (formIter.fillInfo.feather.color >= VerticalIndex && formIter.fillInfo.feather.color < nextColor) {
		  formIter.fillInfo.feather.color++;
		}
	  }
	  if (formIter.edgeType != 0U) {
		if (formIter.borderColor >= VerticalIndex && formIter.borderColor < nextColor) {
		  formIter.borderColor++;
		}
	  }
	}
	for (auto iColor = nextColor; iColor > VerticalIndex; iColor--) {
	  if (iColor != 0U) {
		UserColor[iColor] = UserColor[iColor - 1U];
	  }
	  nuscol(iColor);
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
}

auto thred::internal::usedcol() noexcept -> bool {
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & COLMSK) == VerticalIndex) {
	  return true;
	}
  }
  return false;
}

void thred::internal::delcol() {
  if (usedcol()) {
	displayText::tabmsg(IDS_COLU);
  }
  else {
	for (auto& stitch : *StitchBuffer) {
	  auto const color = stitch.attribute & COLMSK;
	  if (color > VerticalIndex && (color != 0U)) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color - 1U;
	  }
	}
	for (auto& formIter : *FormList) {
	  if (formIter.fillType != 0U) {
		if (formIter.fillColor > VerticalIndex) {
		  formIter.fillColor--;
		}
		if (formIter.fillInfo.feather.color > VerticalIndex) {
		  formIter.fillInfo.feather.color--;
		}
	  }
	  if (formIter.edgeType != 0U) {
		if (formIter.borderColor > VerticalIndex) {
		  formIter.borderColor--;
		}
	  }
	}
	for (auto iColor = VerticalIndex; iColor < 15U; iColor++) {
	  UserColor[iColor] = UserColor[iColor + 1U];
	  nuscol(iColor);
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
}

void thred::internal::set1knot() {
  if ((!StitchBuffer->empty()) && StateMap.test(StateFlag::SELBOX)) {
	thred::savdo();
	auto buffer = std::vector<fPOINTATTR> {};
	buffer.reserve(StitchBuffer->size() + 5U);
	buffer.insert(buffer.begin(),
	              StitchBuffer->begin(),
	              std::next(StitchBuffer->begin(), gsl::narrow<ptrdiff_t>(ClosestPointIndex + 1U)));
	if (ClosestPointIndex == wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	  StateMap.set(StateFlag::FILDIR);
	  OutputIndex = ClosestPointIndex + 1U;
	  endknt(buffer, ClosestPointIndex);
	}
	else {
	  OutputIndex = ClosestPointIndex + 1U;
	  strtknt(buffer, ClosestPointIndex);
	}
	buffer.insert(buffer.end(),
	              std::next(StitchBuffer->begin(), gsl::narrow<ptrdiff_t>(ClosestPointIndex) + 1U),
	              StitchBuffer->end());
	StitchBuffer->resize(buffer.size());
	*StitchBuffer = buffer;
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	displayText::msgstr(IDS_NOSTCHSEL);
	displayText::shoMsg(std::wstring(std::begin(MsgBuffer)));
  }
}

void thred::internal::selfrm0() {
  StateMap.reset(StateFlag::GRPSEL);
  if (StateMap.testAndReset(StateFlag::FORMSEL)) {
	StateMap.set(StateFlag::FRMPSEL);
	ClosestVertexToCursor = 0;
  }
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::selfrmx() {
  StateMap.reset(StateFlag::GRPSEL);
  if (StateMap.testAndReset(StateFlag::FORMSEL)) {
	StateMap.set(StateFlag::FRMPSEL);
	ClosestVertexToCursor = FormList->operator[](ClosestFormToCursor).vertexCount - 1U;
  }
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::setpclp() {
  auto point = POINT {0L, 0L};
  FormVerticesAsLine->clear();
  auto it = InterleaveSequence->begin();
  form::sfCor2px(*it, point);
  it++;
  FormVerticesAsLine->push_back(point);
  form::sfCor2px(*it, point);
  auto const offset =
      POINT {Msg.pt.x - StitchWindowOrigin.x - point.x, Msg.pt.y - StitchWindowOrigin.y - point.y};
  for (auto ine = 1U; ine < wrap::toUnsigned(InterleaveSequence->size()) - 1U; ine++) {
	form::sfCor2px(*it, point);
	it++;
	FormVerticesAsLine->push_back(POINT {point.x + offset.x, point.y + offset.y});
  }
  form::sfCor2px(InterleaveSequence->back(), point);
  FormVerticesAsLine->push_back(point);
}

void thred::internal::dupclp() {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::Polyline(StitchWindowDC, FormVerticesAsLine->data(), wrap::toUnsigned(FormVerticesAsLine->size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unpclp() {
  if (StateMap.testAndReset(StateFlag::SHOP)) {
	dupclp();
  }
}

void thred::internal::fixpclp(uint32_t closestFormToCursor) {
  auto const point = POINT {(Msg.pt.x + gsl::narrow_cast<decltype(Msg.pt.x)>(FormMoveDelta.x)),
                            (Msg.pt.y + gsl::narrow_cast<decltype(Msg.pt.y)>(FormMoveDelta.y))};
  auto       it    = std::next(InterleaveSequence->begin(), 1);
  thred::pxCor2stch(point);
  auto const offset                 = fPOINT {SelectedPoint.x - it->x, SelectedPoint.y - it->y};
  auto const count                  = wrap::toUnsigned(InterleaveSequence->size()) - 2U;
  auto& form                        = FormList->operator[](closestFormToCursor);
  auto const             nextVertex = form::nxt(form, ClosestVertexToCursor);
  form::fltspac(nextVertex, count);
  form.vertexCount += count;
  auto vertexIt = std::next(FormVertices->begin(), form.vertexIndex + nextVertex);
  for (auto iOutput = 1U; iOutput < wrap::toUnsigned(InterleaveSequence->size()) - 1U; iOutput++) {
	*vertexIt = fPOINT {it->x + offset.x, it->y + offset.y};
	vertexIt++;
	it++;
  }
  form::refil();
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
}

auto thred::internal::sidclp() -> bool {
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  return true;
	}
  }
  CloseClipboard();
  return false;
}

void thred::internal::selfpnt() {
  StateMap.reset(StateFlag::FORMSEL);
  StateMap.set(StateFlag::FRMPSEL);
  StateMap.reset(StateFlag::FPSEL);
  StateMap.reset(StateFlag::SELBOX);
  SelectedFormVertices.start = ClosestVertexToCursor;
  SelectedFormVertices.form  = ClosestFormToCursor;
  auto vertexIt = std::next(FormVertices->cbegin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
  thred::ritfcor(vertexIt[ClosestVertexToCursor]);
  StateMap.set(StateFlag::RESTCH);
}

void thred::internal::esccode() {
  chkbit();
  duhbit(MF_UNCHECKED);
  unthum();
  StateMap.reset(StateFlag::MOVSET);
  StateMap.reset(StateFlag::HID);
  StateMap.reset(StateFlag::FRMOF);
  StateMap.reset(StateFlag::THRDS);
  thred::redraw(ButtonWin->operator[](HHID));
  CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED); // NOLINT
  StateMap.reset(StateFlag::COL);
  CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED); // NOLINT
  StateMap.set(StateFlag::DUMEN);
  StateMap.reset(StateFlag::RUNPAT);
  StateMap.reset(StateFlag::WASPAT);
  StateMap.reset(StateFlag::WASBLAK);
  StateMap.reset(StateFlag::GTUANG);
  StateMap.reset(StateFlag::GTUSPAC);
  StateMap.reset(StateFlag::GTWLKIND);
  StateMap.reset(StateFlag::GTWLKLEN);
  DestroyWindow(SpeedScrollBar);
  StateMap.reset(StateFlag::GMRK);
  StateMap.reset(StateFlag::FORMSEL);
  SelectedFormList->clear();
}

void thred::internal::qcode() {
  trace::untrace();
  StateMap.reset(StateFlag::HIDMAP);
  StateMap.reset(StateFlag::FILDIR);
  ReleaseCapture();
  if (StitchBuffer->size() == 1) {
	StitchBuffer->clear();
	StateMap.reset(StateFlag::INIT);
  }
  if (StateMap.testAndReset(StateFlag::POLIMOV)) { // aborting form add
	auto& form = FormList->back();
	if (form.vertexCount != 0U) {
	  auto first = std::next(FormVertices->begin(), form.vertexIndex);
	  auto last  = std::next(first, form.vertexCount);
	  FormVertices->erase(first, last);
	}
	if (form.satinGuideCount != 0U) {
	  auto first = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
	  auto last  = std::next(first, form.satinGuideCount);
	  SatinGuides->erase(first, last);
	}
	FormList->pop_back();
	if (!FormList->empty()) {
	  ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	}
  }
  if (StateMap.testAndReset(StateFlag::FUNCLP)) { // aborting form paste
	thi::bak();
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
  }
  if (StateMap.testAndReset(StateFlag::FUNSCLP)) { // aborting forms paste
	thi::bak();
	SelectedFormList->clear();
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
  }
  if (!UserFlagMap.test(UserFlag::MARQ)) {
	StateMap.reset(StateFlag::GMRK);
  }
  if (StateMap.testAndReset(StateFlag::PRFACT)) {
	if (StateMap.testAndReset(StateFlag::WASRT)) {
	  StateMap.set(StateFlag::INSRT);
	  StateMap.set(StateFlag::RESTCH);
	  return;
	}

	rstAll();
  }
  else {
	rstAll();
  }
  StateMap.set(StateFlag::RESTCH);
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
}

void thred::internal::drwLin(std::vector<POINT>& linePoints, uint32_t currentStitch, uint32_t length, HPEN hPen) {
  if (!StitchBuffer->empty()) {
	auto const activeStitch = std::next(StitchBuffer->begin(), currentStitch);

	if (ActiveLayer != 0U) {
	  LineIndex = 0;
	}
	auto iOffset = 0U;
	for (iOffset = 0; iOffset < length; iOffset++) {
	  auto const layer = (activeStitch[iOffset].attribute & LAYMSK) >> LAYSHFT;
	  if ((ActiveLayer == 0U) || (layer == 0U) || (layer == ActiveLayer)) {
		linePoints[LineIndex++] = {
		    wrap::round<int32_t>((activeStitch[iOffset].x - ZoomRect.left) * ZoomRatio.x),
		    wrap::round<int32_t>(StitchWindowClientRect.bottom -
		                         (activeStitch[iOffset].y - ZoomRect.bottom) * ZoomRatio.y)};
	  }
	}
	SelectObject(StitchWindowMemDC, hPen);
	wrap::Polyline(StitchWindowMemDC, linePoints.data(), LineIndex);
	LineIndex        = 1;
	auto const layer = (activeStitch[iOffset - 1U].attribute & LAYMSK) >> LAYSHFT;
	if ((ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
	  if (iOffset != 0U) {
		linePoints[0] = {
		    wrap::round<int32_t>((activeStitch[iOffset - 1U].x - ZoomRect.left) * ZoomRatio.x),
		    wrap::round<int32_t>(gsl::narrow_cast<float>(StitchWindowClientRect.bottom) -
		                         (activeStitch[iOffset - 1U].y - ZoomRect.bottom) * ZoomRatio.y)};
	  }
	  else {
		linePoints[0] = {wrap::round<int32_t>((activeStitch[0].x - ZoomRect.left) * ZoomRatio.x),
		                 wrap::round<int32_t>(gsl::narrow_cast<float>(StitchWindowClientRect.bottom) -
		                                      (activeStitch[0].y - ZoomRect.bottom) * ZoomRatio.y)};
	  }
	}
  }
}

auto CALLBACK thred::internal::fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
  UNREFERENCED_PARAMETER(lparam);

  switch (umsg) {
	case WM_INITDIALOG: {
	  auto const featherType = IniFile.featherType;
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), fmt::format(L"{:.2f}", IniFile.featherRatio).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), fmt::format(L"{}", IniFile.featherUpCount).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), fmt::format(L"{}", IniFile.featherDownCount).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFLR),
	                fmt::format(L"{:.2f}", (IniFile.featherMinStitchSize / PFGRAN)).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), fmt::format(L"{}", IniFile.featherCount).c_str());
	  auto featherStyle = std::wstring {};
	  for (auto iFeatherStyle = 0U; iFeatherStyle < 6U; iFeatherStyle++) {
		displayText::loadString(featherStyle, (IDS_FTH0 + iFeatherStyle));
		GSL_SUPPRESS(26490)
		SendMessage(
		    GetDlgItem(hwndlg, IDC_FDTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(featherStyle.c_str())); // NOLINT
	  }
	  SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, IniFile.featherFillType - 1, 0);
	  auto state = 0U;
	  if ((featherType & AT_FTHBLND) != 0) {
		state = BST_CHECKED;
	  }
	  else {
		state = BST_UNCHECKED;
	  }
	  CheckDlgButton(hwndlg, IDC_FDBLND, state);
	  if ((featherType & AT_FTHUP) != 0) {
		state = BST_CHECKED;
	  }
	  else {
		state = BST_UNCHECKED;
	  }
	  CheckDlgButton(hwndlg, IDC_FDUP, state);
	  if ((featherType & AT_FTHBTH) != 0) {
		state = BST_CHECKED;
	  }
	  else {
		state = BST_UNCHECKED;
	  }
	  CheckDlgButton(hwndlg, IDC_FBTH, state);
	  break;
	}
	case WM_COMMAND: {
	  switch (LOWORD(wparam)) { // NOLINT
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  IniFile.featherType = 0;
		  if (IsDlgButtonChecked(hwndlg, IDC_FDBLND) != 0U) {
			IniFile.featherType = AT_FTHBLND;
		  }
		  if (IsDlgButtonChecked(hwndlg, IDC_FDUP) != 0U) {
			IniFile.featherType |= AT_FTHUP;
		  }
		  if (IsDlgButtonChecked(hwndlg, IDC_FBTH) != 0U) {
			IniFile.featherType |= AT_FTHBTH;
		  }
		  wchar_t buf[HBUFSIZ] = {0};
		  GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), static_cast<LPTSTR>(buf), HBUFSIZ);
		  IniFile.featherFillType = FDEFTYP;
		  wchar_t buf1[HBUFSIZ]   = {0};
		  for (auto iFeatherStyle = 0U; iFeatherStyle < 6U; iFeatherStyle++) {
			LoadString(ThrEdInstance, IDS_FTH0 + iFeatherStyle, static_cast<LPTSTR>(buf1), HBUFSIZ);
			if (wcscmp(std::begin(buf), std::begin(buf1)) == 0) {
			  IniFile.featherFillType = gsl::narrow<uint8_t>(iFeatherStyle + 1U);
			  break;
			}
		  }
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), static_cast<LPTSTR>(buf), HBUFSIZ);
		  IniFile.featherRatio = wrap::wcstof(buf);
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), static_cast<LPTSTR>(buf), HBUFSIZ);
		  IniFile.featherUpCount = gsl::narrow<uint8_t>(std::stoi(std::begin(buf)));
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), static_cast<LPTSTR>(buf), HBUFSIZ);
		  IniFile.featherDownCount = gsl::narrow<uint8_t>(std::stoi(std::begin(buf)));
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFLR), static_cast<LPTSTR>(buf), HBUFSIZ);
		  IniFile.featherMinStitchSize = std::wcstof(std::begin(buf), nullptr) * PFGRAN;
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), static_cast<LPTSTR>(buf), HBUFSIZ);
		  IniFile.featherCount = gsl::narrow<uint16_t>(std::stoi(std::begin(buf)));
		  if (IniFile.featherCount < 1) {
			IniFile.featherCount = 1;
		  }
		  EndDialog(hwndlg, 1);
		  break;
		}
	  }
	}
	default: {
	}
  }
  return FALSE;
}

void thred::internal::dufdef() noexcept {
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, reinterpret_cast<DLGPROC>(fthdefprc)); // NOLINT
}

auto thred::internal::handleMouseMove(std::vector<POINT>& stretchBoxLine,
                                      float               xyRatio,
                                      float&              rotationAngle,
                                      fPOINT const&       rotationCenter,
                                      FRMHED const&       textureForm) -> bool {
  if (StateMap.test(StateFlag::TXTMOV)) {
	texture::txtrmov(textureForm);
	return true;
  }
  movchk();
  if (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right &&
      Msg.pt.y >= StitchWindowAbsRect.top && Msg.pt.y <= StitchWindowAbsRect.bottom) {
	if (wrap::pressed(VK_SHIFT)) {
	  xt::mvshft();
	  return true;
	}
	do {
	  if ((wrap::pressed(VK_SHIFT)) && thred::px2stch()) {
		thred::ritfcor(SelectedPoint);
	  }
	  if ((StateMap.test(StateFlag::PRFACT) || StateMap.test(StateFlag::FORMIN) ||
	       StateMap.test(StateFlag::POLIMOV)) ||
	      (FormDataSheet != nullptr)) {
		wrap::setCursor(ArrowCursor);
		break;
	  }
	  if (!StateMap.test(StateFlag::INIT)) {
		if (UserFlagMap.test(UserFlag::NEDOF)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(NeedleUpCursor);
		}
		break;
	  }
	  if (StateMap.test(StateFlag::INSRT)) {
		if (UserFlagMap.test(UserFlag::NEDOF)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  if (StateMap.test(StateFlag::LIN1)) {
			wrap::setCursor(NeedleUpCursor);
		  }
		  else {
			if (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).x >
			    StitchBuffer->operator[](ClosestPointIndex).x) {
			  if (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).y >
			      StitchBuffer->operator[](ClosestPointIndex).y) {
				wrap::setCursor(NeedleLeftUpCursor);
			  }
			  else {
				wrap::setCursor(NeedleLeftDownCursor);
			  }
			}
			else {
			  if (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).y >
			      StitchBuffer->operator[](ClosestPointIndex).y) {
				wrap::setCursor(NeedleRightUpCursor);
			  }
			  else {
				wrap::setCursor(NeedleRightDownCursor);
			  }
			}
		  }
		}
		break;
	  }
	  if (StateMap.test(StateFlag::BZUMIN) || StateMap.test(StateFlag::BOXZUM) ||
	      StateMap.test(StateFlag::SELPNT)) {
		wrap::setCursor(CrossCursor);
		break;
	  }
	  if (StateMap.test(StateFlag::SATIN) || StateMap.test(StateFlag::SATPNT) ||
	      StateMap.test(StateFlag::INSFRM)) {
		if (UserFlagMap.test(UserFlag::FRMX)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(FormCursor);
		}
		break;
	  }
	  if (StateMap.test(StateFlag::SATCNKT)) {
		wrap::setCursor(DLineCursor);
	  }
	  else {
		wrap::setCursor(ArrowCursor);
	  }
	} while (false);
	if (StateMap.test(StateFlag::FPUNCLP)) {
	  unpclp();
	  setpclp();
	  StateMap.set(StateFlag::SHOP);
	  dupclp();
	}
	if (StateMap.test(StateFlag::INSFIL)) {
	  form::unfrm();
	  auto const point = POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  insflin(point);
	  StateMap.set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (StateMap.test(StateFlag::MOVFRMS)) {
	  unstrtch(stretchBoxLine);
	  stretchBoxLine[0].x = stretchBoxLine[3].x = stretchBoxLine[4].x =
	      Msg.pt.x - wrap::round<int32_t>(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[1].x = stretchBoxLine[2].x =
	      Msg.pt.x + wrap::round<int32_t>(SelectedFormsSize.x) -
	      wrap::round<int32_t>(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[0].y = stretchBoxLine[1].y = stretchBoxLine[4].y =
	      Msg.pt.y - wrap::round<int32_t>(FormMoveDelta.y) - StitchWindowOrigin.y;
	  stretchBoxLine[2].y = stretchBoxLine[3].y =
	      Msg.pt.y + wrap::round<int32_t>(SelectedFormsSize.y) -
	      wrap::round<int32_t>(FormMoveDelta.y) - StitchWindowOrigin.y;
	  StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap.test(StateFlag::POLIMOV)) {
	  form::munfrm();
	  form::setmfrm();
	  StateMap.set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (StateMap.test(StateFlag::EXPAND)) {
	  unstrtch(stretchBoxLine);
	  auto newSize = fPOINT {gsl::narrow<float>(Msg.pt.x - StitchWindowOrigin.x),
                             gsl::narrow<float>(Msg.pt.y - StitchWindowOrigin.y)};
	  auto       iSide   = (SelectedFormControlVertex + 2U) % 4U;
	  auto const ratio = std::fabs((gsl::narrow_cast<double>(newSize.x) - stretchBoxLine[iSide].x) /
	                               (gsl::narrow_cast<double>(newSize.y) - stretchBoxLine[iSide].y));
	  if ((iSide & 1U) != 0U) {
		if (ratio < xyRatio) {
		  newSize.x = (stretchBoxLine[iSide].y - newSize.y) * xyRatio + stretchBoxLine[iSide].x;
		}
		else {
		  newSize.y = (stretchBoxLine[iSide].x - newSize.x) / xyRatio + stretchBoxLine[iSide].y;
		}
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].y = wrap::round<int32_t>(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = wrap::round<int32_t>(newSize.x);
		stretchBoxLine[iSide].y = wrap::round<int32_t>(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = wrap::round<int32_t>(newSize.x);
	  }
	  else {
		if (ratio < xyRatio) {
		  newSize.x = (newSize.y - stretchBoxLine[iSide].y) * xyRatio + stretchBoxLine[iSide].x;
		}
		else {
		  newSize.y = (newSize.x - stretchBoxLine[iSide].x) / xyRatio + stretchBoxLine[iSide].y;
		}
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = wrap::round<int32_t>(newSize.x);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = wrap::round<int32_t>(newSize.x);
		stretchBoxLine[iSide].y = wrap::round<int32_t>(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].y = wrap::round<int32_t>(newSize.y);
	  }
	  stretchBoxLine[4] = stretchBoxLine[0];
	  StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap.test(StateFlag::STRTCH)) {
	  unstrtch(stretchBoxLine);
	  auto lineLength = 0L;
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		lineLength = Msg.pt.x - StitchWindowOrigin.x;
	  }
	  else {
		lineLength = Msg.pt.y - StitchWindowOrigin.y;
	  }
	  auto const dst  = (SelectedFormControlVertex + 2U) % 4U;
	  auto const code = nxtcrnr(dst);
	  for (auto iSide = 0U; iSide < 4; iSide++) {
		if (iSide != dst && iSide != code) {
		  if ((SelectedFormControlVertex & 1U) != 0U) {
			stretchBoxLine[iSide].x = lineLength;
		  }
		  else {
			stretchBoxLine[iSide].y = lineLength;
		  }
		}
	  }
	  stretchBoxLine[4] = stretchBoxLine[0];
	  StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap.test(StateFlag::INSFRM)) {
	  form::uninsf();
	  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  StateMap.set(StateFlag::SHOINSF);
	  form::duinsf();
	  return true;
	}
	if (StateMap.test(StateFlag::FUNCLP)) {
	  form::unfrm();
	  form::setmfrm();
	  StateMap.set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (StateMap.test(StateFlag::SATCNKT)) {
	  form::drwcon();
	  return true;
	}
	if (StateMap.test(StateFlag::SATPNT)) {
	  satin::drwsat();
	  return true;
	}
	if (StateMap.test(StateFlag::FRMOV)) {
	  form::munfrm();
	  form::setmfrm();
	  StateMap.set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (StateMap.test(StateFlag::FRMPMOV)) {
	  unmov();
	  RubberBandLine->operator[](1) = {Msg.pt.x - StitchWindowOrigin.x,
	                                   Msg.pt.y - StitchWindowOrigin.y};
	  StateMap.set(StateFlag::SHOMOV);
	  thred::ritmov(ClosestFormToCursor);
	  if (thred::px2stch()) {
		thred::ritfcor(SelectedPoint);
	  }
	  return true;
	}
	if (StateMap.test(StateFlag::MOVCNTR)) {
	  unrot();
	  thred::px2stch();
	  ritrot(rotationAngle, SelectedPoint);
	  return true;
	}
	if (StateMap.test(StateFlag::ROTCAPT)) {
	  unrotu();
	  unrot();
	  RotateBoxToCursorLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  auto const adjustedPoint =
	      fPOINT {gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
	              gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y)};
	  if (adjustedPoint.x != 0.0F) {
		rotationAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
	  }
	  else {
		if (adjustedPoint.y > 0) {
		  rotationAngle = PI_F / 2.0F;
		}
		else {
		  rotationAngle = -PI_F / 2.0F;
		}
	  }
	  rotationAngle -= RotationHandleAngle;
	  ritrot(rotationAngle, rotationCenter);
	  StateMap.set(StateFlag::ROTUSHO);
	  durotu();
	  return true;
	}
	if (StateMap.test(StateFlag::SELPNT)) {
	  if (StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  unsel();
	  rSelbox();
	  return true;
	}
	if (StateMap.test(StateFlag::CLPSHO)) {
	  unclp();
	  clpbox();
	  return true;
	}
	if (StateMap.test(StateFlag::CAPT)) {
	  if (thred::px2stch()) {
		thred::ritfcor(SelectedPoint);
	  }
	  unlin();
	  MoveLine0[1] = MoveLine1[0] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  dulin();
	  return true;
	}
	if (StateMap.test(StateFlag::INSRT)) {
	  if (thred::px2stch()) {
		thred::ritfcor(SelectedPoint);
	  }
	  if (StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  if (StateMap.test(StateFlag::LIN1)) {
		if (!StitchBuffer->empty()) {
		  xlin1();
		  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		  StateMap.set(StateFlag::ILIN1);
		  ilin1();
		}
		return true;
	  }
	  xlin();
	  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  StateMap.set(StateFlag::ILIN);
	  ilin();
	  return true;
	}
	if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT)) {
	  SetCapture(ThrEdWindow);
	}
	if (StateMap.test(StateFlag::BZUMIN) && ((Msg.wParam & MK_LBUTTON) != 0U)) { // NOLINT
	  if (StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  thred::unbBox();
	  ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
	  StateMap.set(StateFlag::BZUM);
	  thred::bBox();
	  return true;
	}
  }
  else {
	SetCursor(ArrowCursor);
	wrap::setCursor(ArrowCursor);
	if (StateMap.testAndReset(StateFlag::VCAPT)) {
	  ReleaseCapture();
	}
  }
  return true;
}

auto thred::internal::handleLeftButtonUp(float xyRatio, float rotationAngle, fPOINT& rotationCenter, bool& retflag)
    -> bool {
  retflag = true;
  if ((wrap::pressed(VK_SHIFT)) && thred::px2stch()) {
	texture::setshft();
	return true;
  }
  if (StateMap.test(StateFlag::TXTRED)) {
	texture::txtrup();
	return true;
  }
  ReleaseCapture();
  movchk();
  if (StateMap.testAndReset(StateFlag::MOVFRMS)) {
	thred::savdo();
	auto const point = POINT {(Msg.pt.x - wrap::round<int32_t>(FormMoveDelta.x) - StitchWindowOrigin.x) -
	                              SelectedFormsRect.left,
	                          (Msg.pt.y - wrap::round<int32_t>(FormMoveDelta.y) - StitchWindowOrigin.y) -
	                              SelectedFormsRect.top};
	form::ratsr();
	FormMoveDelta = fPOINT {point.x / HorizontalRatio, point.y / VerticalRatio};
	if (StateMap.test(StateFlag::FPSEL)) {
	  auto const& form = FormList->operator[](ClosestFormToCursor);
	  auto iSelectedVertex = SelectedFormVertices.start;
	  auto vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
		vertexIt[iSelectedVertex].x += FormMoveDelta.x;
		vertexIt[iSelectedVertex].y -= FormMoveDelta.y;
		iSelectedVertex = form::pdir(form, iSelectedVertex);
	  }
	  thred::setpsel();
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	  StateMap.set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap.test(StateFlag::BIGBOX)) {
		thred::savdo();
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		  form::frmadj(iForm);
		}
		for (auto& stitch : *StitchBuffer) {
		  stitch.x += FormMoveDelta.x;
		  stitch.y -= FormMoveDelta.y;
		}
		form::selal();
	  }
	  else {
		thred::savdo();
		for (auto selectedForm : (*SelectedFormList)) {
		  form::frmadj(selectedForm);
		}
		form::frmsadj();
		StateMap.set(StateFlag::RESTCH);
	  }
	}
	return true;
  }
  if (StateMap.testAndReset(StateFlag::EXPAND)) {
	form::setexpand(xyRatio);
	return true;
  }
  if (StateMap.testAndReset(StateFlag::STRTCH)) {
	form::setstrtch();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::FRMOV)) {
	thred::savdo();
	form::rstfrm();
	StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap.testAndReset(StateFlag::FRMPMOV)) {
	thred::savdo();
	form::setfpnt();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::MOVCNTR)) {
	rotationCenter = SelectedPoint;
	StateMap.set(StateFlag::ROTAT);
	return true;
  }
  if (StateMap.testAndReset(StateFlag::ROTCAPT)) {
	thred::rotfn(rotationAngle, rotationCenter);
	return true;
  }
  if (StateMap.testAndReset(StateFlag::SELPNT)) {
	thred::savdo();
	ReleaseCapture();
	unsel();
	auto const adjustedPoint = fPOINT {(StitchRangeRect.left + SelectBoxOffset.x) - SelectedPoint.x,
	                                   (StitchRangeRect.bottom + SelectBoxOffset.y) - SelectedPoint.y};
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
	  StitchBuffer->operator[](iStitch).x -= adjustedPoint.x;
	  StitchBuffer->operator[](iStitch).y -= adjustedPoint.y;
	}
	StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap.testAndReset(StateFlag::CAPT)) {
	unlin();
	ReleaseCapture();
	StateMap.reset(StateFlag::CAPT);
	thred::savdo();
	thred::px2stch();
	StitchBuffer->operator[](ClosestPointIndex).x = SelectedPoint.x;
	StitchBuffer->operator[](ClosestPointIndex).y = SelectedPoint.y;
	StitchBuffer->operator[](ClosestPointIndex).attribute |= USMSK;
	if (ZoomFactor < STCHBOX) {
	  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	  SelectObject(StitchWindowMemDC, LinePen);
	  stchbox(ClosestPointIndex - 1, StitchWindowDC);
	  stchbox(ClosestPointIndex, StitchWindowDC);
	  stchbox(ClosestPointIndex + 1, StitchWindowDC);
	  SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
	StateMap.set(StateFlag::SELBOX);
	StateMap.reset(StateFlag::FRMPSEL);
	StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap.test(StateFlag::BZUMIN)) {
	thred::px2stch();
	if (StateMap.testAndReset(StateFlag::BOXSLCT)) {
	  if (ZoomBoxOrigin.x > SelectedPoint.x) {
		StitchRangeRect.right = ZoomBoxOrigin.x;
		StitchRangeRect.left  = SelectedPoint.x;
	  }
	  else {
		StitchRangeRect.right = SelectedPoint.x;
		StitchRangeRect.left  = ZoomBoxOrigin.x;
	  }
	  if (ZoomBoxOrigin.y > SelectedPoint.y) {
		StitchRangeRect.top    = ZoomBoxOrigin.y;
		StitchRangeRect.bottom = SelectedPoint.y;
	  }
	  else {
		StitchRangeRect.top    = SelectedPoint.y;
		StitchRangeRect.bottom = ZoomBoxOrigin.y;
	  }
	  if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		thred::rngadj();
		for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
		  if (inrng(iStitch)) {
			ClosestPointIndex = iStitch;
			StateMap.set(StateFlag::SELBOX);
			break;
		  }
		}
	  }
	  if (StateMap.testAndReset(StateFlag::NOSEL)) {
		SelectedFormList->clear();
		// We potentially reserve too much memory, but the cost of reallocatioon is higher than the
		// small amount overallocated
		SelectedFormList->reserve(FormList->size());
		StateMap.reset(StateFlag::FORMSEL);
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		  if (finrng(iForm)) {
			SelectedFormList->push_back(iForm);
		  }
		}
		if (SelectedFormList->size() == 1) {
		  ReleaseCapture();
		  gotbox();
		  ClosestFormToCursor   = SelectedFormList->front();
		  ClosestVertexToCursor = 0;
		  SelectedFormList->clear();
		  displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
		  StateMap.set(StateFlag::RESTCH);
		  StateMap.set(StateFlag::FORMSEL);
		  return true;
		}
		if (!SelectedFormList->empty()) {
		  gotbox();
		  return true;
		}
		StateMap.reset(StateFlag::BZUMIN);
		StateMap.reset(StateFlag::BOXSLCT);
		StateMap.reset(StateFlag::FRMPSEL);
		StateMap.set(StateFlag::RESTCH);
	  }
	  if (StateMap.testAndReset(StateFlag::SELBOX)) {
		if (inrng(ClosestPointIndex)) {
		  GroupStitchIndex = ClosestPointIndex + 1U;
		  ClosestPointIndex--;
		  while (inrng(ClosestPointIndex)) {
			ClosestPointIndex--;
		  }
		  ClosestPointIndex++;
		  while (inrng(GroupStitchIndex)) {
			GroupStitchIndex++;
		  }
		  GroupStitchIndex--;
		  StateMap.set(StateFlag::GRPSEL);
		  gotbox();
		  return true;
		}
	  }
	  if (!StateMap.test(StateFlag::INSRT)) {
		rngal();
	  }
	  //				gotbox();
	  return true;
	}

	if (SelectedPoint.x < ZoomBoxOrigin.x) {
	  std::swap(ZoomBoxOrigin.x, SelectedPoint.x);
	}
	if (SelectedPoint.y < ZoomBoxOrigin.y) {
	  std::swap(ZoomBoxOrigin.y, SelectedPoint.y);
	}
	auto newSize    = fPOINT {SelectedPoint.x - ZoomBoxOrigin.x, SelectedPoint.y - ZoomBoxOrigin.y};
	SelectedPoint.x = ZoomBoxOrigin.x + newSize.x / 2.0F;
	SelectedPoint.y = ZoomBoxOrigin.y + newSize.y / 2.0F;
	auto const saveFactor = ZoomFactor;
	if (newSize.x > newSize.y) {
	  newSize.y  = newSize.x / StitchWindowAspectRatio;
	  ZoomFactor = newSize.x / UnzoomedRect.x;
	}
	else {
	  newSize.x  = newSize.y * StitchWindowAspectRatio;
	  ZoomFactor = newSize.y / UnzoomedRect.x;
	}
	if (ZoomFactor < ZoomMin) {
	  ZoomFactor = saveFactor;
	  zumin();
	  return true;
	}
	ZoomRect = fRECTANGLE {0.0F, newSize.y, newSize.x, 0.0F};
	thred::shft(SelectedPoint);
	StateMap.reset(StateFlag::BZUMIN);
	StateMap.set(StateFlag::RESTCH);
	if (!StateMap.testAndSet(StateFlag::ZUMED)) {
	  thred::movStch();
	}
	return true;

	//			StateMap.reset(StateFlag::BZUMIN);
  }
  retflag = false;
  return {};
}

auto thred::internal::handleEitherButtonDown(bool& retflag) -> bool {
  retflag = true;
  if (FormDataSheet != nullptr) {
	auto formDataRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(FormDataSheet, &formDataRect);
  }
  if (StateMap.testAndReset(StateFlag::THUMON)) {
	if (chkok()) {
	  thred::save();
	  thumbak();
	  thred::unmsg();
	  return true;
	}
	if (chkwnd(DiscardButton)) {
	  thumbak();
	  thred::unmsg();
	  return true;
	}
	StateMap.set(StateFlag::BAKSHO);
	thred::unbsho();
	StateMap.reset(StateFlag::THUMSHO);
	thred::unmsg();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::BAKSHO)) {
	if (Msg.message == WM_RBUTTONDOWN) {
	  StateMap.set(StateFlag::RBUT);
	}
	else {
	  StateMap.reset(StateFlag::RBUT);
	}
	{
	  auto iVersion = 0U;
	  for (auto& iBackup : BackupViewer) {
		if (Msg.hwnd == iBackup) {
		  FileVersionIndex = gsl::narrow<char>(iVersion);
		  if (StateMap.test(StateFlag::THUMSHO)) {
			if (savcmp()) {
			  thumbak();
			}
			else {
			  if (StateMap.test(StateFlag::RBUT)) {
				thumbak();
			  }
			  else {
				StateMap.set(StateFlag::THUMON);
				displayText::savdisc();
			  }
			  return true;
			}
		  }
		  else {
			rebak();
		  }
		  rstAll();
		  StateMap.set(StateFlag::RESTCH);
		  return true;
		}
		iVersion++;
	  }
	}
  }
  if (Msg.pt.x >= ColorBarRect.left && Msg.pt.x <= ColorBarRect.right &&
      Msg.pt.y >= ColorBarRect.top && Msg.pt.y <= ColorBarRect.bottom) {
	unpat();
	auto const colorBarPosition = (gsl::narrow_cast<double>(Msg.pt.y) - ColorBarRect.top) /
	                              (gsl::narrow_cast<double>(ColorBarRect.bottom) - ColorBarRect.top);
	if (Msg.message == WM_RBUTTONDOWN) {
	  if (((Msg.wParam & MK_SHIFT) != 0U) // NOLINT
	      && (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::GRPSEL))) {
		unbox();
		GroupStitchIndex = wrap::round<uint32_t>(colorBarPosition * StitchBuffer->size());
		StateMap.set(StateFlag::GRPSEL);
		thred::grpAdj();
		nuAct(GroupStitchIndex);
		StateMap.set(StateFlag::RESTCH);
	  }
	  else {
		ClosestPointIndex = wrap::round<uint32_t>(colorBarPosition * StitchBuffer->size());
		nuAct(ClosestPointIndex);
		movbox();
		if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		  StateMap.reset(StateFlag::SCROS);
		  StateMap.reset(StateFlag::ECROS);
		  StateMap.set(StateFlag::RESTCH);
		}
	  }
	}
	else {
	  ClosestPointIndex = wrap::round<uint32_t>(colorBarPosition * StitchBuffer->size());
	  nuAct(ClosestPointIndex);
	  rstAll();
	  StateMap.set(StateFlag::SELBOX);
	  selCol();
	}
	thred::redraw(ColorBar);
	return true;
  }
  retflag = false;
  return {};
}

auto thred::internal::handleRightButtonDown() -> bool {
  if (StateMap.test(StateFlag::TXTRED) && (MsgWindow == nullptr)) {
	texture::txtrbut();
	return true;
  }
  if (wrap::pressed(VK_SHIFT)) {
	if (form::closfrm()) {
	  if ((StateMap.test(StateFlag::FRMPSEL) || StateMap.test(StateFlag::FPSEL)) &&
	      SelectedFormVertices.form == ClosestFormToCursor) {
		auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
		StateMap.reset(StateFlag::FRMPSEL);
		StateMap.set(StateFlag::FPSEL);
		SelectedFormVertices.finish = ClosestVertexToCursor;
		auto const selectedVertexCount =
		    (SelectedFormVertices.finish - SelectedFormVertices.start + vertexCount) % vertexCount;
		if (selectedVertexCount < vertexCount / 2U) {
		  SelectedFormVertices.vertexCount = selectedVertexCount;
		  StateMap.set(StateFlag::PSELDIR);
		}
		else {
		  SelectedFormVertices.vertexCount = vertexCount - selectedVertexCount;
		  StateMap.reset(StateFlag::PSELDIR);
		}
		thred::setpsel();
		return true;
	  }
	  StateMap.reset(StateFlag::FPSEL);
	  StateMap.set(StateFlag::FRMPSEL);
	  SelectedFormVertices.start = ClosestVertexToCursor;
	  SelectedFormVertices.form  = ClosestFormToCursor;
	  selfpnt();
	  return true;
	}
  }
  if (StateMap.test(StateFlag::WASTRAC)) {
	trace::wasTrace1();
	return true;
  }
  thred::unmsg();
  unboxs();
  unpat();
  StateMap.reset(StateFlag::SATIN);
  if (StateMap.testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (StateMap.test(StateFlag::BAKSHO)) {
	thred::unbsho();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::BIGBOX)) {
	StateMap.set(StateFlag::RESTCH);
  }
  if (StateMap.test(StateFlag::PRFACT)) {
	if (StateMap.testAndReset(StateFlag::HUPMSG)) {
	  thred::unsid();
	}
	else {
	  if (PreferenceIndex != 0U) {
		chknum();
		thred::unsid();
		PreferenceIndex = 0;
	  }
	  else {
		DestroyWindow(PreferencesWindow);
		StateMap.reset(StateFlag::PRFACT);
		if (StateMap.testAndReset(StateFlag::WASRT)) {
		  StateMap.set(StateFlag::INSRT);
		}
	  }
	}
	StateMap.set(StateFlag::RESTCH);
	return true;
  }

  if (FormMenuChoice != 0U) {
	chknum();
	FormMenuChoice = 0;
	StateMap.set(StateFlag::RESTCH);
	return true;
  }

  if (StateMap.testAndReset(StateFlag::INSFRM)) {
	insadj();
	StateMap.reset(StateFlag::SHOINSF);
	StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (thred::px2stch() && !(StateMap.test(StateFlag::SIZSEL) &&
                            thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
	if (!FormList->empty() && !StateMap.test(StateFlag::FRMOF)) {
	  if ((Msg.wParam & MK_SHIFT) != 0U) { // NOLINT
		TmpFormIndex = ClosestFormToCursor;
		if (form::closfrm()) {
		  // ToDo - I don't think this can ever be hit with closfrm
		  if (!SelectedFormList->empty()) {
			nuslst(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
			return true;
		  }

		  if (StateMap.testAndReset(StateFlag::FORMSEL) && TmpFormIndex != ClosestFormToCursor) {
			if (TmpFormIndex > ClosestFormToCursor) {
			  std::swap(ClosestFormToCursor, TmpFormIndex);
			}
			for (auto iForm = TmpFormIndex; iForm <= ClosestFormToCursor; iForm++) {
			  SelectedFormList->push_back(iForm);
			}
			StateMap.set(StateFlag::RESTCH);
			return true;
		  }

		  form::nufsel();
		}
	  }
	  if ((Msg.wParam & MK_CONTROL) != 0U) { // NOLINT
		if (SelectedFormList->empty() && StateMap.test(StateFlag::FORMSEL)) {
		  StateMap.set(StateFlag::WASEL);
		  PreviousFormIndex = ClosestFormToCursor;
		}
		if (form::closfrm()) {
		  form::nufsel();
		}
		if (SelectedFormList->size() > 1) {
		  std::wstring const blank;
		  displayText::butxt(HNUM, blank);
		}
		return true;
	  }
	  if (StateMap.test(StateFlag::FORMSEL)) {
		do {
		  if (FormMenuChoice != 0U) {
			chknum();
			FormMenuChoice = 0;
			break;
		  }
		  if (FormDataSheet != nullptr) {
			thred::undat();
			thred::unsid();
			FormMenuChoice = 0;
			StateMap.set(StateFlag::RESTCH);
			break;
		  }
		  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		  lenCalc();
		  if (!StateMap.testAndReset(StateFlag::ENTROT)) {
			StateMap.reset(StateFlag::FORMSEL);
		  }
		  StateMap.set(StateFlag::DUMEN);
		  if (!closPnt1(&ClosestPointIndex)) {
			unbox();
		  }
		} while (false);
	  }
	  if (form::closfrm()) {
		StateMap.set(StateFlag::FORMSEL);
		StateMap.reset(StateFlag::FPSEL);
		form::unpsel();
		form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		if (StateMap.testAndReset(StateFlag::FRMPSEL) || !SelectedFormList->empty()) {
		  StateMap.set(StateFlag::RESTCH);
		  SelectedFormList->clear();
		}
		displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
		lenCalc();
		unrot();
		return true;
	  }
	  if (!SelectedFormList->empty()) {
		SelectedFormList->clear();
		StateMap.set(StateFlag::RESTCH);
	  }
	  if (StateMap.testAndReset(StateFlag::FRMPSEL)) {
		StateMap.set(StateFlag::RESTCH);
	  }
	}
	if (StateMap.test(StateFlag::INIT) || (FileHandle != nullptr)) {
	  if ((Msg.wParam & MK_SHIFT) != 0U) { // NOLINT
		if (StateMap.test(StateFlag::SELBOX)) {
		  auto const code = ClosestPointIndex;
		  closPnt1(&ClosestPointIndex);
		  if (ClosestPointIndex != code) {
			unbox();
			GroupStitchIndex  = ClosestPointIndex;
			ClosestPointIndex = code;
			StateMap.set(StateFlag::GRPSEL);
			thred::grpAdj();
		  }
		  nuAct(GroupStitchIndex);
		  thred::redraw(ColorBar);
		  return true;
		}
		if (StateMap.test(StateFlag::GRPSEL)) {
		  auto const code = ClosestPointIndex;
		  closPnt1(&ClosestPointIndex);
		  GroupStitchIndex  = ClosestPointIndex;
		  ClosestPointIndex = code;
		  thred::grpAdj();
		  nuAct(GroupStitchIndex);
		  thred::redraw(ColorBar);
		  return true;
		}
		rebox();
	  }
	  else {
		StateMap.reset(StateFlag::LENSRCH);
		StateMap.reset(StateFlag::WASGRP);
		StateMap.reset(StateFlag::FPSEL);
		StateMap.reset(StateFlag::WASFRMFRM);
		StateMap.reset(StateFlag::SIDACT);
		if (StateMap.testAndReset(StateFlag::INSRT)) {
		  ReleaseCapture();
		  rstAll();
		  StateMap.set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap.testAndReset(StateFlag::GRPSEL)) {
			StateMap.set(StateFlag::RESTCH);
		  }
		  if (!StitchBuffer->empty()) {
			rebox();
		  }
		}
	  }
	  xt::clrstch();
	  StateMap.set(StateFlag::RESTCH);
	}
	return true;
  }
  if (Msg.pt.x >= MinLenRect.left && Msg.pt.x <= MinLenRect.right && Msg.pt.y > MinLenRect.top &&
      Msg.pt.y <= MinLenRect.bottom) {
	srchk();
	setsrch(SmallestStitchIndex);
	lensadj();
	StateMap.set(StateFlag::GRPSEL);
	StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (Msg.pt.x >= MaxLenRect.left && Msg.pt.x <= MaxLenRect.right && Msg.pt.y > MaxLenRect.top &&
      Msg.pt.y <= MaxLenRect.bottom) {
	srchk();
	setsrch(LargestStitchIndex);
	lensadj();
	StateMap.set(StateFlag::GRPSEL);
	StateMap.set(StateFlag::RESTCH);
  }
  return true;
}

auto thred::internal::updateHoopSize() -> bool {
  for (auto iHoop = 0U; iHoop < HUPS; iHoop++) {
	if (Msg.hwnd == SideWindow[iHoop]) {
	  switch (iHoop + 1U) {
		case SETCUST: {
		  IniFile.customHoopX = IniFile.hoopSizeX;
		  IniFile.customHoopY = IniFile.hoopSizeY;
		  break;
		}
		case SMALHUP: {
		  IniFile.hoopSizeX = SHUPX;
		  IniFile.hoopSizeY = SHUPY;
		  IniFile.hoopType  = SMALHUP;
		  break;
		}
		case LARGHUP: {
		  IniFile.hoopSizeX = LHUPX;
		  IniFile.hoopSizeY = LHUPY;
		  IniFile.hoopType  = LARGHUP;
		  break;
		}
		case HUP100: {
		  IniFile.hoopSizeX = HUP100XY;
		  IniFile.hoopSizeY = HUP100XY;
		  IniFile.hoopType  = HUP100;
		  break;
		}
		case CUSTHUP: {
		  IniFile.hoopSizeX = IniFile.customHoopX;
		  IniFile.hoopSizeY = IniFile.customHoopY;
		  IniFile.hoopType  = CUSTHUP;
		  break;
		}
	  }
	  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
	  form::sethup();
	  chkhup();
	  break;
	}
  }
  thred::unsid();
  formForms::prfmsg();
  return true;
}

auto thred::internal::updateFillColor() -> bool {
  do {
	thred::savdo();
	if (StateMap.testAndReset(StateFlag::FSETFCOL)) {
	  xt::dufcol(VerticalIndex + 1U);
	  return true;
	}
	if (StateMap.testAndReset(StateFlag::BRDSID)) {
	  form::nubrdcol(VerticalIndex);
	  break;
	}
	if (StateMap.testAndReset(StateFlag::APSID)) {
	  form::nulapcol(VerticalIndex);
	  break;
	}
	if (StateMap.testAndReset(StateFlag::FTHSID)) {
	  form::nufthcol(VerticalIndex);
	  break;
	}
	if (StateMap.testAndReset(StateFlag::UNDCOL)) {
	  FormList->operator[](ClosestFormToCursor).underlayColor = VerticalIndex & 0xfU;
	  form::refilfn();
	  thred::coltab();
	  break;
	}
	if (StateMap.testAndReset(StateFlag::FSETUCOL)) {
	  xt::dundcol(VerticalIndex + 1U);
	  return true;
	}
	form::nufilcol(VerticalIndex);
  } while (false);
  MsgBuffer[0] = gsl::narrow<char>(VerticalIndex) + 0x30;
  MsgBuffer[1] = 0;
  SetWindowText(ValueWindow->operator[](LBRDCOL), static_cast<LPCWSTR>(MsgBuffer));
  thred::unsid();
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
  StateMap.reset(StateFlag::SIDCOL);
  FormMenuChoice = 0;
  return true;
}

auto thred::internal::updatePreferences() -> bool {
  chknum();
  if (Msg.hwnd == ValueWindow->operator[](PSQR)) {
	if (UserFlagMap.testAndFlip(UserFlag::SQRFIL)) {
	  SetWindowText(ValueWindow->operator[](PSQR), StringTable->operator[](STR_PNTD).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PSQR), StringTable->operator[](STR_SQR).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PBLNT)) {
	if (UserFlagMap.testAndFlip(UserFlag::BLUNT)) {
	  SetWindowText(ValueWindow->operator[](PBLNT), StringTable->operator[](STR_TAPR).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PBLNT), StringTable->operator[](STR_BLUNT).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PUND)) {
	if (UserFlagMap.testAndFlip(UserFlag::DUND)) {
	  SetWindowText(ValueWindow->operator[](PUND), StringTable->operator[](STR_OFF).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PUND), StringTable->operator[](STR_ON).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PHUP)) {
	sidhup();
	return true;
  }
  for (auto iPreference = 0U; iPreference < PRFLINS; iPreference++) {
	if (Msg.hwnd == ValueWindow->operator[](iPreference)) {
	  PreferenceIndex = iPreference + 1U;
	  OutputDebugString(fmt::format(L"handleLeftButtonDown:PreferenceIndex [{}]\n", PreferenceIndex).c_str());
	  formForms::prfsid(ValueWindow->operator[](iPreference));
	  break;
	}
  }
  return true;
}

auto thred::internal::handleSideWindowActive() -> bool {
  thred::savdo();
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (FormMenuChoice == LFTHTYP) {
	for (auto iFillType = 0U; iFillType < 6U; iFillType++) {
	  if (Msg.hwnd == SideWindow[iFillType]) {
		form.fillInfo.feather.fillType = gsl::narrow<uint8_t>(iFillType + 1U);
		thred::unsid();
		form::refil();
		formForms::refrm();
		break;
	  }
	}
	return true;
  }
  if (FormMenuChoice == LLAYR) {
	auto iLayer = 0U;
	for (; iLayer < 5U; iLayer++) {
	  if (Msg.hwnd == SideWindow[iLayer]) {
		break;
	  }
	}
	if (iLayer < 5U) {
	  form::movlayr(iLayer * 2U);
	  StateMap.set(StateFlag::FORMSEL);
	}
	thred::unsid();
	auto layerStr =
	    fmt::format(L"{}", (gsl::narrow_cast<decltype(form.attribute)>(form.attribute & FRMLMSK) >> 1U));
	SetWindowText(ValueWindow->operator[](LLAYR), layerStr.c_str());
	formForms::refrm();

	return true;
  }
  form.borderColor &= COLMSK;
  if (StateMap.testAndReset(StateFlag::BRDACT)) {
	if (clip::iseclp(form)) {
	  clip::deleclp(ClosestFormToCursor);
	}
	do {
	  if (Msg.hwnd == SideWindow[0]) {
		form.edgeType = 0;
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
		break;
	  }
	  if (Msg.hwnd == SideWindow[1]) {
		if (form.edgeType != 0U) {
		  auto const code = form.edgeType & NEGUND;
		  if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL) {
			form::bsizpar(form);
		  }
		  form.edgeType = EDGELINE;
		  break;
		}

		form::bord();
		break;
	  }
	  if (Msg.hwnd == SideWindow[2]) {
		if (form.edgeType != 0U) {
		  auto const code = form.edgeType & NEGUND;
		  if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL) {
			form::bsizpar(form);
		  }
		  form.edgeType = EDGEBEAN;
		  break;
		}

		form::dubold();
		break;
	  }
	  if (Msg.hwnd == SideWindow[3]) {
		form::fclp();
		break;
	  }
	  if (Msg.hwnd == SideWindow[4]) {
		if (form.edgeType != 0U) {
		  switch (form.edgeType) {
			case EDGECLIP: {
			  form::bsizpar(form);
			}
			case EDGELINE:
			case EDGEBEAN: {
			  form.borderSize  = BorderWidth;
			  form.edgeSpacing = LineSpacing;
			  break;
			}
			case EDGEPROPSAT: {
			  form.edgeSpacing /= 2;
			  break;
			}
		  }
		  form.edgeType = EDGEANGSAT;
		  if (UserFlagMap.test(UserFlag::DUND)) {
			form.edgeType |= EGUND;
		  }
		  break;
		}

		satin::satbrd();
		break;
	  }
	  if (Msg.hwnd == SideWindow[5]) {
		if (form.fillType != 0U) {
		  form::delmfil(ClosestFormToCursor);
		  form.fillType = 0;
		}
		if (form.edgeType != 0U) {
		  if (form.edgeType == EDGELINE || form.edgeType == EDGEBEAN || form.edgeType == EDGECLIP) {
			form.borderSize  = BorderWidth;
			form.edgeSpacing = LineSpacing;
			if (form.edgeType == EDGECLIP) {
			  form::bsizpar(form);
			}
		  }
		  form.edgeType = EDGEAPPL;
		  if (UserFlagMap.test(UserFlag::DUND)) {
			form.edgeType |= EGUND;
		  }
		  form.borderColor |= (AppliqueColor << 4U);
		  break;
		}

		form::apliq();
		break;
	  }
	  if (Msg.hwnd == SideWindow[6]) {
		if (form.edgeType != 0U) {
		  switch (form.edgeType) {
			case EDGECLIP: {
			  form::bsizpar(form);
			}
			case EDGELINE:
			case EDGEBEAN: {
			  form.borderSize  = BorderWidth;
			  form.edgeSpacing = LineSpacing;
			  break;
			}
			case EDGEANGSAT: {
			  form.edgeSpacing *= 2;
			}
		  }
		  form.edgeType = EDGEPROPSAT;
		  if (UserFlagMap.test(UserFlag::DUND)) {
			form.edgeType |= EGUND;
		  }
		  break;
		}

		form::prpbrd(LineSpacing);
		break;
	  }
	  if (Msg.hwnd == SideWindow[7]) {
		if (form.edgeType != 0U) {
		  if (form.edgeType == EDGELINE || form.edgeType == EDGEBEAN || form.edgeType == EDGECLIP) {
			form.borderSize  = BorderWidth;
			form.edgeSpacing = LineSpacing;
			if (form.edgeType == EDGECLIP) {
			  form::bsizpar(form);
			}
		  }
		  form.edgeType = EDGEBHOL;
		  break;
		}

		form::bhol();
		break;
	  }
	  if (Msg.hwnd == SideWindow[8]) {
		if (form.edgeType != 0U) {
		  if (form.edgeType == EDGELINE || form.edgeType == EDGEBEAN || form.edgeType == EDGECLIP) {
			form.borderSize  = BorderWidth;
			form.edgeSpacing = LineSpacing;
			if (form.edgeType == EDGECLIP) {
			  form::bsizpar(form);
			}
		  }
		  form::picot();
		  break;
		}

		form::picot();
		break;
	  }
	  if (Msg.hwnd == SideWindow[9]) {
		if (form.edgeType != 0U) {
		  auto const code = form.edgeType & NEGUND;
		  if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL) {
			form::bsizpar(form);
		  }
		  form.edgeType = EDGEDOUBLE;
		  break;
		}

		form::dubsfil(form);
		break;
	  }
	  if (Msg.hwnd == SideWindow[10]) {
		StateMap.set(StateFlag::LINCHN);
		form::chan();
		thred::coltab();
		break;
	  }
	  if (Msg.hwnd == SideWindow[11]) {
		StateMap.reset(StateFlag::LINCHN);
		form::chan();
		thred::coltab();
		break;
	  }
	  if (Msg.hwnd == SideWindow[12]) {
		form::filclpx();
		break;
	  }
	} while (false);
	formForms::refrm();
	form::refil();
	thred::unmsg();
	thred::unsid();
	StateMap.set(StateFlag::RESTCH);
	return true;
  }

  if (form.fillType == SAT && (form.satinGuideCount != 0U)) {
	satin::delsac(ClosestFormToCursor);
  }
  if ((form.edgeType & NEGUND) == EDGEAPPL) {
	form.edgeType = EDGEANGSAT;
	if (UserFlagMap.test(UserFlag::DUND)) {
	  form.edgeType |= EGUND;
	}
  }
  auto textureFlag = false;
  do {
	if (Msg.hwnd == SideWindow[0]) { // none
	  form.type = FRMFPOLY;
	  form::delmfil(ClosestFormToCursor);
	  form.fillType = 0;
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[1]) { // vertical fill
	  thred::savdo();
	  form.type = FRMFPOLY;
	  if (form.fillType != 0U) {
		respac(form);
		form.fillType = VRTF;
		form.type     = FRMFPOLY;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }

	  form::filvrt();
	  break;
	}
	if (Msg.hwnd == SideWindow[2]) { // horizontal fill
	  form.type = FRMFPOLY;
	  if (form.fillType != 0U) {
		respac(form);
		form.fillType = HORF;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }

	  form::filhor();
	  break;
	}
	if (Msg.hwnd == SideWindow[3]) { // angle fill
	  form.type = FRMFPOLY;
	  if (form.fillType != 0U) {
		if (form.satinGuideCount != 0U) {
		  satin::delsac(ClosestFormToCursor);
		}
		respac(form);
		// ToDo - should we be using the angle information already present
		form.fillType              = ANGF;
		form.angleOrClipData.angle = IniFile.fillAngle;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }

	  form::filangl();
	  break;
	}
	if (Msg.hwnd == SideWindow[4]) { // fan fill
	  form.type = SAT;
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinOrAngle.guide = 0;
	  }
	  if (form.fillType != 0U) {
		respac(form);
		form.fillType = SATF;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }

	  StateMap.reset(StateFlag::FTHR);
	  form::filsat();
	  break;
	}
	if (Msg.hwnd == SideWindow[5]) { // fan clip
	  form.type = SAT;
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinOrAngle.guide = 0;
	  }
	  form::clpfil();
	  break;
	}
	if (Msg.hwnd == SideWindow[6]) { // contour fill
	  if (form.fillType != 0U) {
		if (form.fillType == CLPF) {
		  form.fillSpacing = LineSpacing;
		}
		form::chkcont();
		clip::delclps(ClosestFormToCursor);
		break;
	  }

	  form::contfil();
	  break;
	}
	if (Msg.hwnd == SideWindow[7]) { // vertical clip
	  if (sidclp()) {
		form::vrtsclp();
	  }
	  StateMap.reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[8]) { // horizontal clip
	  if (sidclp()) {
		form::horsclp();
	  }
	  StateMap.reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[9]) { // angle clip
	  if (sidclp()) {
		if (form.satinGuideCount != 0U) {
		  satin::delsac(ClosestFormToCursor);
		}
		form::angsclp(form);
	  }
	  StateMap.reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[10]) { // feather fill
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinOrAngle.guide = 0;
	  }
	  xt::fethrf();
	  StateMap.set(StateFlag::INIT);
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[11]) // vertical texture
	{
	  if (texture::istx(ClosestFormToCursor)) {
		form.fillType = TXVRTF;
		break;
	  }
	  texture::dutxtfil();
	  textureFlag = true;
	  break;
	}
	if (Msg.hwnd == SideWindow[12]) // horizontal texture
	{
	  if (texture::istx(ClosestFormToCursor)) {
		form.fillType = TXHORF;
		break;
	  }
	  texture::dutxtfil();
	  textureFlag = true;
	  break;
	}
	if (Msg.hwnd == SideWindow[13]) // angle texture
	{
	  if (texture::istx(ClosestFormToCursor)) {
		form.fillType              = TXANGF;
		form.angleOrClipData.angle = IniFile.fillAngle;
		break;
	  }
	  texture::dutxtfil();
	  textureFlag = true;
	  break;
	}
  } while (false);
  if (textureFlag) {
	thred::unsid();
	DestroyWindow(FormDataSheet);
	FormDataSheet = nullptr;
	StateMap.set(StateFlag::WASFRMFRM);
  }
  else {
	formForms::refrm();
	form::refil();
	thred::unmsg();
	thred::unsid();
	StateMap.set(StateFlag::RESTCH);
  }
  return true;
}

auto thred::internal::handleFormDataSheet() -> bool {
  chknum();
  thred::unsid();
  auto& form = FormList->operator[](ClosestFormToCursor);

  do {
	if (Msg.hwnd == ValueWindow->operator[](LTXOF) || Msg.hwnd == LabelWindow->operator[](LTXOF)) {
	  FormMenuChoice = LTXOF;
	  formForms::sidwnd(ValueWindow->operator[](LTXOF));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LUSPAC) || Msg.hwnd == LabelWindow->operator[](LUSPAC)) {
	  FormMenuChoice = LUSPAC;
	  formForms::sidwnd(ValueWindow->operator[](LUSPAC));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LUANG) || Msg.hwnd == LabelWindow->operator[](LUANG)) {
	  FormMenuChoice = LUANG;
	  formForms::sidwnd(ValueWindow->operator[](LUANG));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LULEN) || Msg.hwnd == LabelWindow->operator[](LULEN)) {
	  FormMenuChoice = LULEN;
	  formForms::sidwnd(ValueWindow->operator[](LULEN));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LWLKIND) || Msg.hwnd == LabelWindow->operator[](LWLKIND)) {
	  FormMenuChoice = LWLKIND;
	  formForms::sidwnd(ValueWindow->operator[](LWLKIND));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHSIZ) || Msg.hwnd == LabelWindow->operator[](LFTHSIZ)) {
	  FormMenuChoice = LFTHSIZ;
	  formForms::sidwnd(ValueWindow->operator[](LFTHSIZ));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHNUM) || Msg.hwnd == LabelWindow->operator[](LFTHNUM)) {
	  FormMenuChoice = LFTHNUM;
	  formForms::sidwnd(ValueWindow->operator[](LFTHNUM));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHFLR) || Msg.hwnd == LabelWindow->operator[](LFTHFLR)) {
	  FormMenuChoice = LFTHFLR;
	  formForms::sidwnd(ValueWindow->operator[](LFTHFLR));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHUPCNT) || Msg.hwnd == LabelWindow->operator[](LFTHUPCNT)) {
	  FormMenuChoice = LFTHUPCNT;
	  formForms::sidwnd(ValueWindow->operator[](LFTHUPCNT));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHDWNCNT) || Msg.hwnd == LabelWindow->operator[](LFTHDWNCNT)) {
	  FormMenuChoice = LFTHDWNCNT;
	  formForms::sidwnd(ValueWindow->operator[](LFTHDWNCNT));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHBLND) || Msg.hwnd == LabelWindow->operator[](LFTHBLND)) {
	  form.extendedAttribute ^= AT_FTHBLND;
	  formForms::refrm();
	  form::refil();
	  thred::unsid();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHUP) || Msg.hwnd == LabelWindow->operator[](LFTHUP)) {
	  form.extendedAttribute ^= AT_FTHUP;
	  form::refil();
	  auto choice = StringTable->operator[](STR_OFF);
	  if ((form.extendedAttribute & AT_FTHUP) != 0U) {
		choice = StringTable->operator[](STR_ON);
	  }
	  SetWindowText(ValueWindow->operator[](LFTHUP), choice.c_str());
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHBTH) || Msg.hwnd == LabelWindow->operator[](LFTHBTH)) {
	  form.extendedAttribute ^= AT_FTHBTH;
	  formForms::refrm();
	  form::refil();
	  thred::unsid();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHTYP) || Msg.hwnd == LabelWindow->operator[](LFTHTYP)) {
	  FormMenuChoice = LFTHTYP;
	  sidmsg(form, ValueWindow->operator[](LFTHTYP), &StringTable->operator[](STR_FTH0), 6);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRM) || Msg.hwnd == LabelWindow->operator[](LFRM)) {
	  thred::savdo();
	  form::unfil();
	  if (form.type == FRMLINE) {
		form.type = FRMFPOLY;
	  }
	  else {
		form.type = FRMLINE;
	  }
	  thred::coltab();
	  satin::delsac(ClosestFormToCursor);
	  StateMap.set(StateFlag::RESTCH);
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LLAYR) || Msg.hwnd == LabelWindow->operator[](LLAYR)) {
	  std::wstring layerText[] = {L"0", L"1", L"2", L"3", L"4"};
	  FormMenuChoice           = LLAYR;
	  StateMap.reset(StateFlag::FILTYP);
	  sidmsg(form, ValueWindow->operator[](LLAYR), std::begin(layerText), 5U);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMFIL) || Msg.hwnd == LabelWindow->operator[](LFRMFIL)) {
	  StateMap.reset(StateFlag::FILTYP);
	  FormMenuChoice = LFRMFIL;
	  sidmsg(form, ValueWindow->operator[](LFRMFIL), &StringTable->operator[](STR_FIL0), 14U);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMCOL) || Msg.hwnd == LabelWindow->operator[](LFRMCOL)) {
	  FormMenuChoice = LFRMCOL;
	  formForms::sidwnd(ValueWindow->operator[](LFRMCOL));
	  StateMap.set(StateFlag::SIDCOL);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LUNDCOL) || Msg.hwnd == LabelWindow->operator[](LUNDCOL)) {
	  FormMenuChoice = LUNDCOL;
	  formForms::sidwnd(ValueWindow->operator[](LUNDCOL));
	  StateMap.set(StateFlag::SIDCOL);
	  StateMap.set(StateFlag::UNDCOL);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHCOL) || Msg.hwnd == LabelWindow->operator[](LFTHCOL)) {
	  FormMenuChoice = LFTHCOL;
	  formForms::sidwnd(ValueWindow->operator[](LFTHCOL));
	  StateMap.set(StateFlag::SIDCOL);
	  StateMap.set(StateFlag::FTHSID);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMSPAC) || Msg.hwnd == LabelWindow->operator[](LFRMSPAC)) {
	  FormMenuChoice = LFRMSPAC;
	  formForms::sidwnd(ValueWindow->operator[](LFRMSPAC));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMLEN) || Msg.hwnd == LabelWindow->operator[](LFRMLEN)) {
	  FormMenuChoice = LFRMLEN;
	  formForms::sidwnd(ValueWindow->operator[](LFRMLEN));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRD) || Msg.hwnd == LabelWindow->operator[](LBRD)) {
	  StateMap.set(StateFlag::FILTYP);
	  sidmsg(form, ValueWindow->operator[](LBRD), &StringTable->operator[](STR_EDG0), EDGETYPS + 1U);
	  StateMap.set(StateFlag::BRDACT);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDCOL) || Msg.hwnd == LabelWindow->operator[](LBRDCOL)) {
	  FormMenuChoice = LBRDCOL;
	  formForms::sidwnd(ValueWindow->operator[](LBRDCOL));
	  StateMap.set(StateFlag::SIDCOL);
	  StateMap.set(StateFlag::BRDSID);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDSPAC) || Msg.hwnd == LabelWindow->operator[](LBRDSPAC)) {
	  FormMenuChoice = LBRDSPAC;
	  formForms::sidwnd(ValueWindow->operator[](LBRDSPAC));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDLEN) || Msg.hwnd == LabelWindow->operator[](LBRDLEN)) {
	  FormMenuChoice = LBRDLEN;
	  formForms::sidwnd(ValueWindow->operator[](LBRDLEN));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDSIZ) || Msg.hwnd == LabelWindow->operator[](LBRDSIZ)) {
	  FormMenuChoice = LBRDSIZ;
	  formForms::sidwnd(ValueWindow->operator[](LBRDSIZ));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LAPCOL) || Msg.hwnd == LabelWindow->operator[](LAPCOL)) {
	  FormMenuChoice = LAPCOL;
	  StateMap.set(StateFlag::SIDCOL);
	  StateMap.set(StateFlag::APSID);
	  formForms::sidwnd(ValueWindow->operator[](LAPCOL));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBCSIZ) || Msg.hwnd == LabelWindow->operator[](LBCSIZ)) {
	  FormMenuChoice = LBCSIZ;
	  formForms::sidwnd(ValueWindow->operator[](LBCSIZ));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBSTRT) || Msg.hwnd == LabelWindow->operator[](LBSTRT)) {
	  auto const code = form.attribute & SBLNT;
	  if (code != 0U) {
		form.attribute &= NSBLNT;
		SetWindowText(ValueWindow->operator[](LBSTRT), StringTable->operator[](STR_TAPR).c_str());
	  }
	  else {
		form.attribute |= SBLNT;
		SetWindowText(ValueWindow->operator[](LBSTRT), StringTable->operator[](STR_BLUNT).c_str());
	  }
	  form::refil();
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBFIN) || Msg.hwnd == LabelWindow->operator[](LBFIN)) {
	  auto const code = form.attribute & FBLNT;
	  if (code != 0U) {
		form.attribute &= NFBLNT;
		SetWindowText(ValueWindow->operator[](LBFIN), StringTable->operator[](STR_TAPR).c_str());
	  }
	  else {
		form.attribute |= FBLNT;
		SetWindowText(ValueWindow->operator[](LBFIN), StringTable->operator[](STR_BLUNT).c_str());
	  }
	  form::refil();
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMANG) || Msg.hwnd == LabelWindow->operator[](LFRMANG)) {
	  FormMenuChoice = LFRMANG;
	  formForms::sidwnd(ValueWindow->operator[](LFRMANG));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDPIC) || Msg.hwnd == LabelWindow->operator[](LBRDPIC)) {
	  FormMenuChoice = LBRDPIC;
	  formForms::sidwnd(ValueWindow->operator[](LBRDPIC));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDUND) || Msg.hwnd == LabelWindow->operator[](LBRDUND)) {
	  form.edgeType ^= EGUND;
	  form::refil();
	  auto const code = form.edgeType & EGUND;
	  if (code != 0U) {
		SetWindowText(ValueWindow->operator[](LBRDUND), StringTable->operator[](STR_ON).c_str());
	  }
	  else {
		SetWindowText(ValueWindow->operator[](LBRDUND), StringTable->operator[](STR_OFF).c_str());
	  }
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LSACANG) || Msg.hwnd == LabelWindow->operator[](LSACANG)) {
	  FormMenuChoice = LSACANG;
	  formForms::sidwnd(ValueWindow->operator[](LSACANG));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMFAZ) || Msg.hwnd == LabelWindow->operator[](LFRMFAZ)) {
	  FormMenuChoice = LFRMFAZ;
	  formForms::sidwnd(ValueWindow->operator[](LFRMFAZ));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDPOS) || Msg.hwnd == LabelWindow->operator[](LBRDPOS)) {
	  FormMenuChoice = LBRDPOS;
	  formForms::sidwnd(ValueWindow->operator[](LBRDPOS));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBFILSQR) || Msg.hwnd == LabelWindow->operator[](LBFILSQR)) {
	  xt::dubit(form, AT_SQR);
	  auto choice = StringTable->operator[](STR_PNTD);
	  if ((form.extendedAttribute & AT_SQR) != 0U) {
		choice = StringTable->operator[](STR_SQR);
	  }
	  SetWindowText(ValueWindow->operator[](LBFILSQR), choice.c_str());
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFSTRT) || Msg.hwnd == LabelWindow->operator[](LFSTRT)) {
	  xt::dubit(form, AT_STRT);
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LDSTRT) || Msg.hwnd == LabelWindow->operator[](LDSTRT)) {
	  FormMenuChoice = LDSTRT;
	  formForms::sidwnd(ValueWindow->operator[](LDSTRT));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFEND) || Msg.hwnd == LabelWindow->operator[](LFEND)) {
	  xt::dubit(form, AT_END);
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LDEND) || Msg.hwnd == LabelWindow->operator[](LDEND)) {
	  FormMenuChoice = LDEND;
	  formForms::sidwnd(ValueWindow->operator[](LDEND));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LWALK) || Msg.hwnd == LabelWindow->operator[](LWALK)) {
	  xt::dubit(form, AT_WALK);
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LCWLK) || Msg.hwnd == LabelWindow->operator[](LCWLK)) {
	  xt::dubit(form, AT_CWLK);
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LUND) || Msg.hwnd == LabelWindow->operator[](LUND)) {
	  xt::dubit(form, AT_UND);
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LMAXFIL) || Msg.hwnd == LabelWindow->operator[](LMAXFIL)) {
	  FormMenuChoice = LMAXFIL;
	  formForms::sidwnd(ValueWindow->operator[](LMAXFIL));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LMINFIL) || Msg.hwnd == LabelWindow->operator[](LMINFIL)) {
	  FormMenuChoice = LMINFIL;
	  formForms::sidwnd(ValueWindow->operator[](LMINFIL));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LMAXBRD) || Msg.hwnd == LabelWindow->operator[](LMAXBRD)) {
	  FormMenuChoice = LMAXBRD;
	  formForms::sidwnd(ValueWindow->operator[](LMAXBRD));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LMINBRD) || Msg.hwnd == LabelWindow->operator[](LMINBRD)) {
	  FormMenuChoice = LMINBRD;
	  formForms::sidwnd(ValueWindow->operator[](LMINBRD));
	  break;
	}
  } while (false);
  OutputDebugString(fmt::format(L"handleLeftButtonDown:FormMenuChoice [{}]\n", FormMenuChoice).c_str());

  return true;
}

auto thred::internal::handleLeftButtonDown(std::vector<POINT>& stretchBoxLine,
                                           float&              xyRatio,
                                           FRMHED const&       textureForm,
                                           bool&               retflag) -> bool {
  retflag = true;
  if ((wrap::pressed(VK_SHIFT)) && thred::px2stch()) {
	xt::dushft();
	return true;
  }
  if (StateMap.test(StateFlag::TXTRED) && !StateMap.test(StateFlag::FORMIN)) {
	texture::txtlbut(textureForm);
	return true;
  }
  if (StateMap.testAndReset(StateFlag::FSETFCOL)) {
	thred::unsid();
	thred::unmsg();
	if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	  xt::dufcol(VerticalIndex + 1U);
	  return true;
	}
  }
  if (StateMap.testAndReset(StateFlag::FSETBCOL)) {
	thred::unsid();
	thred::unmsg();
	if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	  xt::dubcol(VerticalIndex + 1U);
	  return true;
	}
  }
  if (StateMap.testAndReset(StateFlag::FPUNCLP)) {
	thred::savdo();
	fixpclp(ClosestFormToCursor);
	form::frmout(ClosestFormToCursor);
	return true;
  }
  if (StateMap.test(StateFlag::FPSEL) && !StateMap.test(StateFlag::FUNCLP) && !StateMap.test(StateFlag::ROTAT)) {
	*SelectedFormsLine = *SelectedPointsLine;
	SelectedFormsRect  = SelectedPixelsRect;
	if (chkbig(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (StateMap.test(StateFlag::WASTRAC)) {
	trace::tracpar();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::HUPMSG)) {
	return thi::updateHoopSize();
  }
  if (StateMap.testAndReset(StateFlag::INSFIL)) {
	duinsfil();
	return true;
  }
  if (StateMap.test(StateFlag::BIGBOX) && chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (StateMap.testAndReset(StateFlag::DELSFRMS)) {
	auto code = 0U;
	if (chkok()) {
	  thred::savdo();
	  StateMap.reset(StateFlag::DELTO);
	  code = 1;
	}
	else {
	  auto windowRect = RECT {0L, 0L, 0L, 0L};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
	      Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom) {
		thred::savdo();
		StateMap.set(StateFlag::DELTO);
		code = 1;
	  }
	}
	delsfrms(code);
	thred::unmsg();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::MOVMSG)) {
	if (chkok()) {
	  thred::savdo();
	  form::refilfn();
	}
	thred::unmsg();
	if (StateMap.testAndReset(StateFlag::WASFRMFRM)) {
	  formForms::refrm();
	}
	return true;
  }
  if (StateMap.test(StateFlag::FUNSCLP)) {
	StateMap.reset(StateFlag::MOVFRMS);
	auto formsRect = fRECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, formsRect);
	thred::px2stch();
	FormMoveDelta = fPOINT {SelectedPoint.x - form::midl(formsRect.right, formsRect.left),
	                        SelectedPoint.y - form::midl(formsRect.top, formsRect.bottom)};
	for (auto iForm = 0U; iForm < ClipFormsCount; iForm++) {
	  ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - iForm - 1U);
	  auto& form          = FormList->operator[](ClosestFormToCursor);
	  auto vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		vertexIt[iVertex].x += FormMoveDelta.x;
		vertexIt[iVertex].y += FormMoveDelta.y;
	  }
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	}
	StateMap.reset(StateFlag::FUNSCLP);
	StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (!SelectedFormList->empty() && !StateMap.test(StateFlag::ROTAT) && chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (StateMap.test(StateFlag::SIDCOL) &&
      thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	return thi::updateFillColor();
  }
  if (StateMap.testAndReset(StateFlag::OSAV)) {
	if (chkok()) {
	  thred::save();
	  nuFil();
	  thred::unmsg();
	  return true;
	}
	if (chkwnd(DiscardButton)) {
	  nuFil();
	  thred::unmsg();
	  return true;
	}
	thred::unmsg();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::FCLOS)) {
	if (chkok()) {
	  thred::save();
	}
	else {
	  if (!chkwnd(CancelButton)) {
		closfn();
	  }
	}
	thred::unmsg();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::SAVEX)) {
	if (chkok()) {
	  thred::save();
	  reldun();
	}
	if (chkwnd(DiscardButton)) {
	  reldun();
	}
	thred::unmsg();
	return true;
  }
  if (StateMap.test(StateFlag::PRFACT)) {
	return thi::updatePreferences();
  }
  if (!StateMap.test(StateFlag::ROTAT) && StateMap.test(StateFlag::GRPSEL)) {
	auto& controlPoint = *FormControlPoints;
	if (iselpnt()) {
	  for (auto iSide = 0U; iSide < wrap::toUnsigned(stretchBoxLine.size()); iSide++) {
		stretchBoxLine[iSide] = controlPoint[wrap::toSize(iSide) * 2U];
	  }
	  stretchBoxLine.back() = stretchBoxLine.front();
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		StateMap.set(StateFlag::STRTCH);
	  }
	  else {
		StateMap.set(StateFlag::EXPAND);
		xyRatio = (StitchRangeRect.right - StitchRangeRect.left) /
		          (StitchRangeRect.top - StitchRangeRect.bottom);
	  }
	  SelectedFormControlVertex >>= 1U;
	  StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}

	StitchCoordinatesPixels.x = Msg.pt.x - StitchWindowOrigin.x;
	StitchCoordinatesPixels.y = Msg.pt.y - StitchWindowOrigin.y;
	if (StitchCoordinatesPixels.x >= controlPoint[0].x &&
	    StitchCoordinatesPixels.x <= controlPoint[2].x &&
	    StitchCoordinatesPixels.y >= controlPoint[0].y &&
	    StitchCoordinatesPixels.y <= controlPoint[4].y) {
	  duSelbox();
	  StateMap.set(StateFlag::SELPNT);
	  SetCapture(ThrEdWindow);
	  StateMap.set(StateFlag::VCAPT);
	  rSelbox();
	  return true;
	}
  }
  if (StateMap.testAndReset(StateFlag::NEWBAK)) {
	if (chkok()) {
	  thred::unmsg();
	  thred::save();
	  newFil();
	  return true;
	}
	if (chkwnd(DiscardButton)) {
	  thred::unmsg();
	  newFil();
	  return true;
	}
	thred::unmsg();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::PRGMSG)) {
	if (chkok()) {
	  deldir();
	  return true;
	}
  }
  if (StateMap.testAndReset(StateFlag::DUBAK)) {
	if (StateMap.test(StateFlag::THUMSHO)) {
	  if (chkok()) {
		thrsav();
	  }
	  getbak();
	  StateMap.reset(StateFlag::THUMSHO);
	  return true;
	}

	if (chkok()) {
	  auto  thrExt   = ThrName->extension().wstring();
	  auto& thrBack  = thrExt.back();
	  auto  workExt  = WorkingFileName->extension().wstring();
	  auto& workBack = workExt.back();
	  thrBack        = 't';
	  ThrName->replace_extension(thrExt);
	  StateMap.set(StateFlag::IGNAM);
	  thrsav();
	  thrBack = 'r';
	  ThrName->replace_extension(thrExt);
	  if (FileVersionIndex > 0) {
		workBack = (FileVersionIndex - 1) + '0';
		WorkingFileName->replace_extension(workExt);
	  }
	  StateMap.set(StateFlag::REDOLD);
	  nuFil();
	  workBack = 'r';
	  WorkingFileName->replace_extension(workExt);
	  switch (FileVersionIndex) {
		case 3: {
		  movbak('1', '2');
		}
		case 2: {
		  movbak('0', '1');
		}
		case 1: {
		  movbak('r', '0');
		}
		default: {
		}
	  }
	  movbak('t', 'r');
	  thrBack = 't';
	  ThrName->replace_extension(thrExt);
	  fs::remove(*ThrName);
	  thrBack = 'r';
	  ThrName->replace_extension(thrExt);
	  FileHandle = CreateFile(
	      ThrName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr); // NOLINT
	  if (FileHandle == INVALID_HANDLE_VALUE) { // NOLINT
		FileHandle = nullptr;
	  }
	  return true;
	}
	auto windowRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(CancelButton, &windowRect);
	if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top &&
	    Msg.pt.y <= windowRect.bottom) {
	  getbak();
	  return true;
	}
  }
  if (StateMap.test(StateFlag::DELFRM)) {
	auto code = 0;
	if (chkok()) {
	  thred::savdo();
	  StateMap.reset(StateFlag::DELTO);
	  code = 1;
	}
	else {
	  auto windowRect = RECT {0L, 0L, 0L, 0L};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
	      Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom) {
		thred::savdo();
		StateMap.set(StateFlag::DELTO);
		code = 1;
	  }
	}
	if (code != 0) {
	  thred::frmdel();
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	}
	thred::unmsg();
	return true;
  }
  if (StateMap.test(StateFlag::FILMSG)) {
	if (chkok()) {
	  thred::savdo();
	  form::unfil();
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  thred::unmsg();
	  return true;
	}
  }
  if (StateMap.testAndReset(StateFlag::SIZSEL)) {
	if (thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2])) {
	  VerticalIndex -= 13U;
	  char threadSizeMap[]                = {'3', '4', '6'};
	  ThreadSize[ThreadSizeSelected][0]   = threadSizeMap[VerticalIndex];
	  ThreadSizeIndex[ThreadSizeSelected] = VerticalIndex;
	  wchar_t buffer[3] = {0};
	  wcsncpy_s(buffer, ThreadSize[ThreadSizeSelected], 2); // NOLINT
	  buffer[2] = 0;
	  SetWindowText(ThreadSizeWin[ThreadSizeSelected], static_cast<LPTSTR>(buffer));
	  StateMap.set(StateFlag::RESTCH);
	  for (auto& iWindow : ChangeThreadSizeWin) {
		if (iWindow != nullptr) {
		  DestroyWindow(iWindow);
		}
	  }
	  return true;
	}
	for (auto& iWindow : ChangeThreadSizeWin) {
	  if (iWindow != nullptr) {
		DestroyWindow(iWindow);
	  }
	}
  }
  if (StateMap.testAndReset(StateFlag::POLIMOV)) {
	thred::savdo();
	form::setfrm();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::FORMIN)) {
	auto windowRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(MsgWindow, &windowRect);
	if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top &&
	    Msg.pt.y <= windowRect.bottom) {
	  auto const iFillType = (Msg.pt.y - windowRect.top - 1) / (ButtonHeight - 4);
	  if (StateMap.testAndReset(StateFlag::FENDIN)) {
		if (iFillType == 3) {
		  UserFlagMap.reset(UserFlag::SQRFIL);
		}
		if (iFillType == 4) {
		  UserFlagMap.set(UserFlag::SQRFIL);
		}
	  }
	  else {
		form::duform(iFillType);
	  }
	  switch (iFillType) {
		case FRMRPOLY - 1:
		case FRMSTAR - 1:
		case FRMSPIRAL - 1:
		case FRMHEART - 2:
		case FRMLENS - 2:
		case FRMEGG - 2:
		case FRMZIGZAG - 2:
		  return true;
		default: {
		}
	  }
	}
	thred::unmsg();
	return true;
  }
  if (PreferenceIndex == PAP + 1 && thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	AppliqueColor = VerticalIndex;
	SetWindowText(ValueWindow->operator[](PAP), fmt::format(L"{}", VerticalIndex).c_str());
	thred::unsid();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::SIDACT)) {
	return thi::handleSideWindowActive();
  }
  if (FormDataSheet != nullptr) {
	return thi::handleFormDataSheet();
  }
  if (StateMap.test(StateFlag::INSFRM)) {
	thred::savdo();
	form::setins();
	return true;
  }
  if (StateMap.test(StateFlag::FUNCLP)) {
	StateMap.set(StateFlag::INIT);
	form::rstfrm();
	form::refil();
	StateMap.reset(StateFlag::FUNCLP);
	if (StateMap.testAndReset(StateFlag::FPSEL)) {
	  form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	}
	StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap.test(StateFlag::SATCNKT)) {
	thred::savdo();
	satin::satknkt();
	return true;
  }
  if (StateMap.test(StateFlag::SATPNT)) {
	satin::satpnt1();
	return true;
  }
  if (StateMap.testAndReset(StateFlag::SATIN)) {
	satin::satpnt0();
	return true;
  }
  if (StateMap.test(StateFlag::FORMSEL)) {
	if (!StateMap.test(StateFlag::FRMROT) && form::chkfrm(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (StateMap.testAndReset(StateFlag::MOVFRM)) {
	thred::savdo();
	form::setfrm();
	return true;
  }
  thred::unmsg();
  if (thred::px2stch() && !(StateMap.test(StateFlag::SIZSEL) &&
                            thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
	unpat();
	if (StateMap.testAndReset(StateFlag::ROTAT)) {
	  RotateBoxToCursorLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  auto const adjustedPoint =
	      fPOINT {gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
	              gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y)};
	  if (hypot(adjustedPoint.x, adjustedPoint.y) < CLOSENUF) {
		thred::px2stch();
		StateMap.set(StateFlag::MOVCNTR);
		unrot();
		ritrot(0, SelectedPoint);
	  }
	  else {
		if (adjustedPoint.x != 0.0F) {
		  RotationHandleAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
		}
		else {
		  if (adjustedPoint.y > 0) {
			RotationHandleAngle = PI_F / 2.0F;
		  }
		  else {
			RotationHandleAngle = -PI_F / 2.0F;
		  }
		}
		StateMap.set(StateFlag::ROTUSHO);
		durotu();
		StateMap.set(StateFlag::ROTCAPT);
	  }
	  return true;
	}
	if (StateMap.testAndReset(StateFlag::CLPSHO)) {
	  thred::savdo();
	  if ((!StitchBuffer->empty()) &&
	      (StateMap.testAndReset(StateFlag::SELBOX) || StateMap.testAndReset(StateFlag::INSRT)) &&
	      ClosestPointIndex != gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U)) {
		lodclp(ClosestPointIndex);
	  }
	  else {
		lodclp(wrap::toUnsigned(StitchBuffer->size()));
	  }
	  thred::rngadj();
	  auto formsRect = fRECTANGLE {};
	  thred::selRct(formsRect);
	  auto& formControls = *FormControlPoints;
	  formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x =
	      wrap::round<int32_t>(formsRect.left);
	  formControls[1].x = formControls[5].x =
	      wrap::round<int32_t>(form::midl(formsRect.right, formsRect.left));
	  formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y =
	      wrap::round<int32_t>(formsRect.top);
	  formControls[3].y = formControls[7].y =
	      wrap::round<int32_t>(form::midl(formsRect.top, formsRect.bottom));
	  formControls[4].y = formControls[5].y = formControls[6].y = wrap::round<int32_t>(formsRect.bottom);
	  formControls[2].x = formControls[3].x = formControls[4].x = wrap::round<int32_t>(formsRect.right);
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  return true;
	}
	if (StateMap.testAndReset(StateFlag::BOXZUM)) {
	  StateMap.set(StateFlag::BZUMIN);
	  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	  thred::px2stch();
	  ZoomBoxOrigin = SelectedPoint;
	  StateMap.set(StateFlag::VCAPT);
	  return true;
	}
	if (!FormList->empty() && !StateMap.test(StateFlag::FRMOF)) {
	  if (!StateMap.test(StateFlag::INSRT)) {
		if (StateMap.testAndReset(StateFlag::FORMSEL)) {
		  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		}
		if (form::closfrm()) {
		  StateMap.set(StateFlag::FRMPMOV);
		  form::frmovlin();
		  return true;
		}
	  }
	}
	if (StateMap.test(StateFlag::INIT)) {
	  unlin();
	  if (StateMap.test(StateFlag::INSRT)) {
		thred::px2stch();
		auto code =
		    (ActiveColor | USMSK | gsl::narrow_cast<decltype(ActiveColor)>(ActiveLayer << LAYSHFT) | NOTFRM) & NKNOTMSK;
		if (StateMap.test(StateFlag::LIN1)) {
		  if (StateMap.test(StateFlag::BAKEND)) {
			xlin1();
			auto const iStitch = wrap::toUnsigned(StitchBuffer->size());
			StitchBuffer->push_back({SelectedPoint.x, SelectedPoint.y, code});
			thred::duzrat();
			stch2px1(iStitch);
			InsertLine[0] = StitchCoordinatesPixels;
			InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
			return true;
		  }

		  xlin1();
		  StitchBuffer->insert(StitchBuffer->begin(), {SelectedPoint.x, SelectedPoint.y, code});
		  StitchBuffer->front().attribute &= (~KNOTMSK);
		  stch2px1(0);
		  InsertLine[0] = StitchCoordinatesPixels;
		  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		  thred::coltab();
		  StateMap.set(StateFlag::RESTCH);
		  return true;
		}

		xlin();
		if (((StitchBuffer->operator[](ClosestPointIndex).attribute & ALTYPMSK) != 0U) &&
		    ((StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).attribute & ALTYPMSK) != 0U)) {
		  if ((StitchBuffer->operator[](ClosestPointIndex).attribute & FRMSK) ==
		      (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).attribute & FRMSK)) {
			code = StitchBuffer->operator[](ClosestPointIndex).attribute | USMSK;
		  }
		}
		ClosestPointIndex++;
		thred::savdo();
		StitchBuffer->insert(std::next(StitchBuffer->begin(), ClosestPointIndex),
		                     fPOINTATTR {SelectedPoint.x, SelectedPoint.y, code});
		xlin();
		InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		stch2px1(ClosestPointIndex);
		InsertLine[0] = StitchCoordinatesPixels;
		stch2px1(ClosestPointIndex + 1U);
		InsertLine[2] = StitchCoordinatesPixels;
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
		displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
		return true;
	  }
	  if ((!StateMap.test(StateFlag::HIDSTCH)) && closPnt1(&ClosestPointIndexClone)) {
		ClosestPointIndex = ClosestPointIndexClone;
		unbox();
		unboxs();
		setbak(ThreadSizePixels[StitchBuffer->operator[](ClosestPointIndex).attribute & 0xfU] + 3U);
		auto linePoints = std::vector<POINT> {};
		linePoints.resize(3);
		LineIndex = 0;
		SetROP2(StitchWindowDC, R2_NOTXORPEN);
		if (ClosestPointIndex == 0) {
		  if (ZoomFactor < STCHBOX) {
			SelectObject(StitchWindowDC, LinePen);
			stchbox(0, StitchWindowDC);
			stchbox(1, StitchWindowDC);
		  }
		  SetROP2(StitchWindowDC, R2_COPYPEN);
		  drwLin(linePoints, 0, 2, BackgroundPen);
		}
		else {
		  if (!StitchBuffer->empty()) {
			auto const lastStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
			if (ClosestPointIndex == lastStitch) {
			  if (ZoomFactor < STCHBOX) {
				SelectObject(StitchWindowDC, LinePen);
				stchbox(lastStitch, StitchWindowDC);
				stchbox(lastStitch - 1, StitchWindowDC);
			  }
			  SetROP2(StitchWindowDC, R2_COPYPEN);
			  drwLin(linePoints, lastStitch - 1, 2, BackgroundPen);
			}
			else {
			  if (ZoomFactor < STCHBOX) {
				SelectObject(StitchWindowDC, LinePen);
				stchbox(ClosestPointIndex - 1, StitchWindowDC);
				stchbox(ClosestPointIndex, StitchWindowDC);
				stchbox(ClosestPointIndex + 1, StitchWindowDC);
			  }
			  SetROP2(StitchWindowDC, R2_COPYPEN);
			  drwLin(linePoints, ClosestPointIndex - 1, 3, BackgroundPen);
			}
		  }
		}
		MoveLine0[1] =
		    MoveLine1[0] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		if (ClosestPointIndex == 0) {
		  StateMap.reset(StateFlag::ISDWN);
		}
		else {
		  StateMap.set(StateFlag::ISDWN);
		  stch2px1(ClosestPointIndex - 1);
		  MoveLine0[0] = StitchCoordinatesPixels;
		}
		auto iStitch = wrap::toUnsigned(StitchBuffer->size());
		if (iStitch != 0U) {
		  iStitch--;
		}
		if (ClosestPointIndex >= iStitch) {
		  StateMap.reset(StateFlag::ISUP);
		}
		else {
		  StateMap.set(StateFlag::ISUP);
		  stch2px1(ClosestPointIndex + 1U);
		  MoveLine1[1] = StitchCoordinatesPixels;
		}
		dulin();
		SetCapture(ThrEdWindow);
		StateMap.set(StateFlag::CAPT);
		displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
	  }
	}
	else {
	  if (thred::px2stch()) {
		thred::savdo();
		InsertLine[0] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		InsertLine[1] = InsertLine[0];
		StitchBuffer->push_back({SelectedPoint.x, SelectedPoint.y, USMSK | ActiveColor | LayerIndex | NOTFRM});
		ColorChanges                    = 1;
		ColorChangeTable[0].colorIndex  = gsl::narrow<uint16_t>(ActiveColor);
		ColorChangeTable[0].stitchIndex = 0;
		ColorChangeTable[1].stitchIndex = 1;
		StateMap.set(StateFlag::LIN1);
		StateMap.set(StateFlag::INSRT);
		SetCapture(ThrEdWindow);
		ClosestPointIndex = 1;
		StateMap.set(StateFlag::INIT);
		StateMap.set(StateFlag::BAKEND);
	  }
	}
	thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	StateMap.set(StateFlag::BOXSLCT);
	StateMap.set(StateFlag::BZUMIN);
	StateMap.set(StateFlag::NOSEL);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	thred::px2stch();
	ZoomBoxOrigin = SelectedPoint;
	StateMap.set(StateFlag::VCAPT);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HBOXSEL)) {
	form::boxsel();
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HUPTO)) {
	toglup();
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HHID)) {
	toglHid();
	return true;
  }
  if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	if (Msg.message == WM_LBUTTONDOWN) {
	  auto const code = ActiveColor;
	  ActiveColor     = VerticalIndex & 0xfU;
	  thred::redraw(UserColorWin->operator[](code));
	  thred::redraw(UserColorWin->operator[](ActiveColor));
	  if (StateMap.test(StateFlag::HID)) {
		StateMap.reset(StateFlag::SELBOX);
		StateMap.reset(StateFlag::GRPSEL);
		StateMap.reset(StateFlag::SCROS);
		StateMap.reset(StateFlag::ECROS);
		StateMap.set(StateFlag::RESTCH);
		thred::redraw(ButtonWin->operator[](HHID));
	  }
	  else {
		if (!SelectedFormList->empty()) {
		  thred::savdo();
		  nucols();
		  thred::coltab();
		  StateMap.set(StateFlag::RESTCH);
		  return true;
		}

		if (StateMap.test(StateFlag::FORMSEL)) {
		  auto& form = FormList->operator[](ClosestFormToCursor);
		  if ((form.fillType != 0U) || (form.edgeType != 0U) ||
		      ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U)) {
			thred::savdo();
			if (form.fillType != 0U) {
			  form.fillColor = gsl::narrow<uint8_t>(ActiveColor);
			  if (form.fillType == FTHF) {
				form.fillInfo.feather.color = gsl::narrow<uint8_t>(ActiveColor);
			  }
			}
			if (form.edgeType != 0U) {
			  if (form.edgeType == EDGEAPPL) {
				form.borderColor &= APCOLMSK;
				form.borderColor |= ActiveColor;
			  }
			  else {
				form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
			  }
			}
			if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
			  form.underlayColor = gsl::narrow<uint8_t>(ActiveColor);
			}
			auto const formCode = ClosestFormToCursor << FRMSHFT;
			for (auto& stitch : *StitchBuffer) {
			  if (((stitch.attribute & ALTYPMSK) != 0U) && (stitch.attribute & FRMSK) == formCode &&
			      (stitch.attribute & TYPMSK) != TYPMSK) {
				stitch.attribute &= NCOLMSK;
				stitch.attribute |= ActiveColor;
			  }
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		  }
		}
		else {
		  if (StateMap.test(StateFlag::GRPSEL)) {
			thred::savdo();
			for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; iStitch++) {
			  StitchBuffer->operator[](iStitch).attribute &= 0xfffffff0;
			  StitchBuffer->operator[](iStitch).attribute |= ActiveColor;
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		  }
		  else {
			if (StateMap.test(StateFlag::COL)) {
			  StateMap.set(StateFlag::RESTCH);
			}
		  }
		}
	  }
	}
	return true;
  }
  if (thi::chkMsgs(Msg.pt, UserColorWin->front(), UserColorWin->back())) {
	if (Msg.message == WM_LBUTTONDOWN && (nuCol(UserColor[VerticalIndex]) != 0U)) {
	  thred::savdo();
	  UserColor[VerticalIndex]      = ColorStruct.rgbResult;
	  UserPen[VerticalIndex]        = nuPen(UserPen[VerticalIndex], 1, UserColor[VerticalIndex]);
	  UserColorBrush[VerticalIndex] = nuBrush(UserColorBrush[VerticalIndex], UserColor[VerticalIndex]);
	  thred::redraw(UserColorWin->operator[](VerticalIndex));
	  StateMap.set(StateFlag::RESTCH);
	}
	return true;
  }
  if (thi::chkMsgs(Msg.pt, ThreadSizeWin[0], ThreadSizeWin[15])) {
	if (Msg.message == WM_LBUTTONDOWN) {
	  wchar_t const* const str[] = {L"30", L"40", L"60"};

	  thred::savdo();
	  ThreadSizeSelected = VerticalIndex;
	  for (auto iThreadSize = 0U; iThreadSize < 3; iThreadSize++) {
		ChangeThreadSizeWin[iThreadSize] = CreateWindow(L"STATIC", // NOLINT
		                                                str[iThreadSize],
		                                                WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                                ButtonWidthX3,
		                                                ButtonHeight * (iThreadSize + VerticalIndex),
		                                                ButtonWidth,
		                                                ButtonHeight,
		                                                ThrEdWindow,
		                                                nullptr,
		                                                ThrEdInstance,
		                                                nullptr);
	  }
	  StateMap.set(StateFlag::SIZSEL);
	}
	return true;
  }
  retflag = false;
  return {};
}

auto thred::internal::doPaste(std::vector<POINT>& stretchBoxLine, bool& retflag) -> bool {
  retflag = true;
  thred::savdo();
  ThrEdClip  = RegisterClipboardFormat(ThrEdClipFormat);
  ClipMemory = GetClipboardData(ThrEdClip);
  if (ClipMemory != nullptr) {
	ClipPointer = GlobalLock(ClipMemory);
	if (ClipPointer != nullptr) {
	  ClipFormVerticesData = gsl::narrow_cast<FORMVERTEXCLIP*>(ClipPointer);
	  if (ClipFormVerticesData->clipType == CLP_FRMPS) {
		thred::duzrat();
		auto byteCount =
		    sizeof(*ClipFormVerticesData) + (wrap::toSize(ClipFormVerticesData->vertexCount) + 1U) *
		                                        sizeof(decltype(FormVertices->back()));
		auto clipCopyBuffer = std::vector<uint8_t> {};
		clipCopyBuffer.resize(byteCount);
		auto clipPointer = gsl::narrow_cast<uint8_t*>(ClipPointer);
		std::copy(clipPointer, clipPointer + byteCount, clipCopyBuffer.begin());
		GlobalUnlock(ClipMemory);
		CloseClipboard();
		ClipFormVerticesData = convert_ptr<FORMVERTEXCLIP*>(clipCopyBuffer.data());
		if (StateMap.test(StateFlag::FRMPSEL)) {
		  auto& form                      = FormList->operator[](ClosestFormToCursor);
		  auto                   vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
		  InterleaveSequence->clear();
		  InterleaveSequence->reserve(wrap::toSize(ClipFormVerticesData->vertexCount) + 3U);
		  InterleaveSequence->push_back(vertexIt[ClosestVertexToCursor]);
		  auto clipData = convert_ptr<fPOINT*>(&ClipFormVerticesData[1]);
		  for (auto iVertex = 0U; iVertex <= ClipFormVerticesData->vertexCount; iVertex++) {
			InterleaveSequence->push_back(clipData[iVertex]);
		  }
		  auto const nextVertex = form::nxt(form, ClosestVertexToCursor);
		  InterleaveSequence->push_back(vertexIt[nextVertex]);
		  setpclp();
		  StateMap.set(StateFlag::FPUNCLP);
		  StateMap.set(StateFlag::SHOP);
		  dupclp();
		}
		else {
		  FormMoveDelta = fPOINT {};
		  StateMap.set(StateFlag::FUNCLP);
		  FormList->push_back(FRMHED {});
		  ClosestFormToCursor  = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
		  auto& formIter       = FormList->back();
		  formIter.type        = FRMLINE;
		  formIter.vertexCount = ClipFormVerticesData->vertexCount + 1U;
		  formIter.vertexIndex = thred::adflt(formIter.vertexCount);
		  auto vertices        = convert_ptr<fPOINT*>(&ClipFormVerticesData[1]);
		  auto destination     = std::next(FormVertices->begin(), formIter.vertexIndex);
		  std::copy(vertices, vertices + formIter.vertexCount, destination);
		  StateMap.set(StateFlag::INIT);
		  NewFormVertexCount = formIter.vertexCount;
		  form::unfrm();
		  form::setmfrm();
		  StateMap.set(StateFlag::SHOFRM);
		  form::dufrm();
		}
		return true;
	  }
	  // ToDo - Add more information to the clipboard so that memory can be allocated
	  auto clipFormsHeader = gsl::narrow_cast<FORMSCLIP*>(ClipPointer);
	  if (clipFormsHeader->clipType == CLP_FRMS) {
		auto iForm            = 0U;
		ClipFormsCount        = clipFormsHeader->formCount;
		auto       forms      = convert_ptr<FRMHED*>(&clipFormsHeader[1]);
		auto const formOffset = wrap::toUnsigned(FormList->size());
		for (iForm = 0; iForm < ClipFormsCount; iForm++) {
		  FormList->push_back(FRMHED {});
		  auto& formIter = FormList->back();
		  formIter       = forms[iForm];
		  formIter.attribute = (gsl::narrow_cast<decltype(formIter.attribute)>(formIter.attribute & NFRMLMSK) |
		                        gsl::narrow_cast<decltype(formIter.attribute)>(ActiveLayer << 1U));
		}
		auto formVertices  = convert_ptr<fPOINT*>(&forms[iForm]);
		auto currentVertex = 0U;
		for (iForm = 0; iForm < ClipFormsCount; iForm++) {
		  auto const offset = formOffset + iForm;
		  auto& form        = FormList->operator[](offset);
		  form.vertexIndex  = wrap::toUnsigned(FormVertices->size());
		  FormVertices->insert(FormVertices->end(),
		                       &formVertices[currentVertex],
		                       &formVertices[currentVertex + form.vertexCount]);
		  currentVertex += form.vertexCount;
		}
		auto guides       = convert_ptr<SATCON*>(&formVertices[currentVertex]);
		auto currentGuide = 0U;
		for (iForm = 0; iForm < ClipFormsCount; iForm++) {
		  auto const offset = formOffset + iForm;
		  auto& form        = FormList->operator[](offset);
		  if (form.type == SAT && (form.satinGuideCount != 0U)) {
			form.satinOrAngle.guide = satin::adsatk(form.satinGuideCount);
			auto guideIt            = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
			  guideIt[iGuide] = guides[currentGuide++];
			}
		  }
		}
		auto clipData    = convert_ptr<fPOINT*>(&guides[currentGuide]);
		auto currentClip = 0U;
		for (iForm = 0; iForm < ClipFormsCount; iForm++) {
		  auto const offset = formOffset + iForm;
		  auto& form        = FormList->operator[](offset);
		  if (clip::isclpx(offset)) {
			form.angleOrClipData.clip = thred::adclp(form.lengthOrCount.clipCount);
			auto offsetStart          = std::next(ClipPoints->begin(), form.angleOrClipData.clip);
			for (auto iClip = 0U; iClip < form.lengthOrCount.clipCount; iClip++) {
			  *offsetStart = clipData[currentClip++];
			  offsetStart++;
			}
		  }
		  if (clip::iseclpx(offset)) {
			form.borderClipData = thred::adclp(form.clipEntries);
			auto offsetStart    = std::next(ClipPoints->begin(), form.borderClipData);
			for (auto iClip = 0U; iClip < form.clipEntries; iClip++) {
			  *offsetStart = clipData[currentClip++];
			  offsetStart++;
			}
		  }
		}
		auto textureSource = convert_ptr<TXPNT*>(&clipData[currentClip]);
		auto textureCount  = 0;
		for (iForm = 0; iForm < ClipFormsCount; iForm++) {
		  if (texture::istx(formOffset + iForm)) {
			auto& form = FormList->operator[](wrap::toSize(formOffset) + iForm);
			textureCount += form.fillInfo.texture.count;
			form.fillInfo.texture.index += gsl::narrow<uint16_t>(TexturePointsBuffer->size());
		  }
		}
		auto const oldEnd = TexturePointsBuffer->size();
		TexturePointsBuffer->resize(TexturePointsBuffer->size() + textureCount);
		auto textureDestination = std::next(TexturePointsBuffer->begin(), oldEnd);
		std::copy(textureSource, textureSource + textureCount, textureDestination);
		GlobalUnlock(ClipMemory);
		SelectedFormsRect.top = SelectedFormsRect.left = 0x7fffffff;
		SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
		form::ratsr();
		SelectedFormList->clear();
		SelectedFormList->reserve(ClipFormsCount);
		for (OutputIndex = 0; OutputIndex < (ClipFormsCount); OutputIndex++) {
		  form::fselrct(OutputIndex + formOffset);
		  SelectedFormList->push_back(OutputIndex + formOffset);
		}
		SelectedFormsSize.x = gsl::narrow<float>(SelectedFormsRect.right - SelectedFormsRect.left);
		SelectedFormsSize.y = gsl::narrow<float>(SelectedFormsRect.bottom - SelectedFormsRect.top);
		StateMap.set(StateFlag::INIT);
		auto& formLines = *FormLines;
		formLines.resize(5U);
		formLines[0].x = formLines[3].x = formLines[4].x = SelectedFormsRect.left;
		formLines[1].x = formLines[2].x = SelectedFormsRect.right;
		formLines[0].y = formLines[1].y = formLines[4].y = SelectedFormsRect.top;
		formLines[2].y = formLines[3].y = SelectedFormsRect.bottom;
		StateMap.set(StateFlag::SHOSTRTCH);
		thred::strtchbox(stretchBoxLine);
		FormMoveDelta =
		    fPOINT {gsl::narrow<float>((SelectedFormsRect.right - SelectedFormsRect.left) / 2),
		            gsl::narrow<float>((SelectedFormsRect.bottom - SelectedFormsRect.top) / 2)};
		StateMap.set(StateFlag::MOVFRMS);
		StateMap.set(StateFlag::FUNSCLP);
	  }
	  else {
		auto clipFormHeader = gsl::narrow_cast<FORMCLIP*>(ClipPointer);
		if (clipFormHeader->clipType == CLP_FRM) {
		  FormMoveDelta = fPOINT {};
		  StateMap.set(StateFlag::FUNCLP);
		  FormList->push_back(clipFormHeader->form);
		  ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
		  auto& formIter      = FormList->back();
		  formIter.attribute = gsl::narrow_cast<decltype(formIter.attribute)>(formIter.attribute & NFRMLMSK) |
		                       gsl::narrow_cast<decltype(formIter.attribute)>(ActiveLayer << 1U);
		  formIter.vertexIndex = thred::adflt(formIter.vertexCount);
		  auto formVertices    = convert_ptr<fPOINT*>(&clipFormHeader[1]);
		  auto destIt          = std::next(FormVertices->begin(), formIter.vertexIndex);
		  std::copy(formVertices, formVertices + formIter.vertexCount, destIt);
		  auto guides = convert_ptr<SATCON*>(&formVertices[formIter.vertexCount]);
		  if (formIter.type == SAT && (formIter.satinGuideCount != 0U)) {
			formIter.satinOrAngle.guide = satin::adsatk(formIter.satinGuideCount);
			std::copy(guides,
			          guides + formIter.satinGuideCount,
			          std::next(SatinGuides->begin(), formIter.satinOrAngle.guide));
		  }
		  auto       clipData      = convert_ptr<fPOINT*>(guides);
		  auto       clipCount     = 0U;
		  auto const lastFormIndex = wrap::toUnsigned(FormList->size() - 1U);
		  if (clip::isclpx(lastFormIndex)) {
			formIter.angleOrClipData.clip = thred::adclp(formIter.lengthOrCount.clipCount);
			auto destination = std::next(ClipPoints->begin(), formIter.angleOrClipData.clip);
			std::copy(clipData, clipData + formIter.lengthOrCount.clipCount, destination);
			clipCount += formIter.lengthOrCount.clipCount;
		  }
		  if (clip::iseclpx(lastFormIndex)) {
			clipData                = convert_ptr<fPOINT*>(&clipData[clipCount]);
			formIter.borderClipData = thred::adclp(formIter.clipEntries);
			auto destination        = std::next(ClipPoints->begin(), formIter.borderClipData);
			std::copy(clipData, clipData + formIter.clipEntries, destination);
			clipCount += formIter.clipEntries;
		  }
		  auto textureSource = convert_ptr<TXPNT*>(&clipData[clipCount]);
		  if (texture::istx(lastFormIndex)) {
			formIter.fillInfo.texture.index = gsl::narrow<uint16_t>(TexturePointsBuffer->size());

			auto currentCount = formIter.fillInfo.texture.count;
			TexturePointsBuffer->resize(TexturePointsBuffer->size() + currentCount);
			auto iter = std::next(TexturePointsBuffer->begin(), formIter.fillInfo.texture.index);
			std::copy(textureSource, textureSource + currentCount, iter);
		  }
		  NewFormVertexCount = formIter.vertexCount;
		  if (formIter.type != FRMLINE) {
			NewFormVertexCount++;
		  }
		  StateMap.set(StateFlag::INIT);
		  form::unfrm();
		  thred::duzrat();
		  form::setmfrm();
		  StateMap.set(StateFlag::SHOFRM);
		  form::dufrm();
		}
		GlobalUnlock(ClipMemory);
	  }
	}
	CloseClipboard();
  }
  else {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  clpbox();
	  StateMap.set(StateFlag::CLPSHO);
	}
	CloseClipboard();
  }
  retflag = false;
  return {};
}

auto thred::internal::handleHomeKey(bool& retflag) -> bool {
  retflag = true;
  if ((wrap::pressed(VK_SHIFT)) && (wrap::pressed(VK_CONTROL))) {
	if (StateMap.testAndReset(StateFlag::SELBOX)) {
	  GroupStitchIndex = 0;
	  StateMap.set(StateFlag::GRPSEL);
	}
	else {
	  if (StateMap.test(StateFlag::GRPSEL)) {
		if (GroupStitchIndex > ClosestPointIndex) {
		  ClosestPointIndex = 0;
		}
		else {
		  GroupStitchIndex = 0;
		}
	  }
	  else {
		StateMap.set(StateFlag::SELBOX);
		StateMap.set(StateFlag::RESTCH);
	  }
	}
	thred::grpAdj();
	thred::redraw(ColorBar);
	return true;
  }
  if (wrap::pressed(VK_SHIFT)) {
	auto iColor = 0U;
	if (StateMap.testAndReset(StateFlag::SELBOX)) {
	  StateMap.set(StateFlag::GRPSEL);
	  iColor = pt2colInd(ClosestPointIndex);
	}
	else {
	  iColor = pt2colInd(GroupStitchIndex);
	}
	iColor--;
	GroupStitchIndex = ColorChangeTable[iColor].stitchIndex;
	thred::grpAdj();
	thred::redraw(ColorBar);
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  stch2px1(0);
	  endpnt();
	  StateMap.reset(StateFlag::BAKEND);
	  StateMap.set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap.test(StateFlag::SELBOX)) {
		auto iColor = pt2colInd(ClosestPointIndex);
		if (iColor != 0U) {
		  iColor--;
		}
		ClosestPointIndex = ColorChangeTable[iColor].stitchIndex;
	  }
	  else {
		if (StateMap.test(StateFlag::LENSRCH)) {
		  setsrch(SmallestStitchIndex);
		  return true;
		}
		ClosestPointIndex = 0;
	  }
	  movbox();
	}
  }
  retflag = false;
  return {};
}

auto thred::internal::handleEndKey(int32_t& retflag) -> bool {
  retflag = 1;
  if ((wrap::pressed(VK_SHIFT)) && (wrap::pressed(VK_CONTROL))) {
	if (!StitchBuffer->empty()) {
	  if (StateMap.testAndReset(StateFlag::SELBOX)) {
		GroupStitchIndex = gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U);
		StateMap.set(StateFlag::GRPSEL);
	  }
	  else {
		if (StateMap.test(StateFlag::GRPSEL)) {
		  if (GroupStitchIndex > ClosestPointIndex) {
			GroupStitchIndex = gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U);
		  }
		  else {
			ClosestPointIndex = gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U);
		  }
		}
		else {
		  StateMap.set(StateFlag::SELBOX);
		  ClosestPointIndex = gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U);
		  StateMap.set(StateFlag::RESTCH);
		  return true;
		}
	  }
	  thred::grpAdj();
	  thred::redraw(ColorBar);
	  retflag = 2;
	  return true;
	}
	return false;
  }
  if (wrap::pressed(VK_SHIFT)) {
	auto iColor = 0U;
	if (StateMap.testAndReset(StateFlag::SELBOX)) {
	  StateMap.set(StateFlag::GRPSEL);
	  iColor = pt2colInd(ClosestPointIndex);
	}
	else {
	  iColor = pt2colInd(GroupStitchIndex);
	}
	GroupStitchIndex = ColorChangeTable[iColor].stitchIndex - 1U;
	thred::grpAdj();
	thred::redraw(ColorBar);
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  if (!StitchBuffer->empty()) {
		stch2px1(wrap::toUnsigned(StitchBuffer->size() - 1U));
	  }
	  endpnt();
	  StateMap.set(StateFlag::BAKEND);
	  StateMap.set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap.test(StateFlag::SELBOX)) {
		auto const iColor = pt2colInd(ClosestPointIndex);
		ClosestPointIndex = ColorChangeTable[iColor].stitchIndex - 1U;
	  }
	  else {
		if (StateMap.test(StateFlag::LENSRCH)) {
		  setsrch(LargestStitchIndex);
		  return true;
		}
		ClosestPointIndex = gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U);
	  }
	  movbox();
	}
  }
  retflag = 0;
  return false;
}

auto thred::internal::handleRightKey(bool& retflag) -> bool {
  retflag                 = true;
  auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
  if (wrap::pressed(VK_SHIFT)) {
	if (StateMap.test(StateFlag::FPSEL)) {
	  if (StateMap.test(StateFlag::PSELDIR)) {
		++SelectedFormVertices.vertexCount %= vertexCount;
		SelectedFormVertices.finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % vertexCount;
	  }
	  else {
		if (SelectedFormVertices.vertexCount != 0U) {
		  SelectedFormVertices.vertexCount--;
		  SelectedFormVertices.finish =
		      (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
		}
		else {
		  SelectedFormVertices.vertexCount = 1;
		  StateMap.set(StateFlag::PSELDIR);
		  SelectedFormVertices.finish = (SelectedFormVertices.start + 1U) % vertexCount;
		}
	  }
	  thred::setpsel();
	}
	else {
	  if (StateMap.testAndReset(StateFlag::FRMPSEL)) {
		form::unpsel();
		SelectedFormVertices.start       = ClosestVertexToCursor;
		SelectedFormVertices.form        = ClosestFormToCursor;
		SelectedFormVertices.vertexCount = 1;
		SelectedFormVertices.finish      = (SelectedFormVertices.start + 1U) % vertexCount;
		StateMap.set(StateFlag::PSELDIR);
		thred::setpsel();
	  }
	  else {
		StateMap.reset(StateFlag::LENSRCH);
		StateMap.reset(StateFlag::FORMSEL);
		if (StateMap.testAndReset(StateFlag::SELBOX)) {
		  if (!StitchBuffer->empty()) {
			auto const lastStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
			if (ClosestPointIndex < lastStitch) {
			  StateMap.set(StateFlag::GRPSEL);
			  GroupStitchIndex = ClosestPointIndex + 1U;
			}
		  }
		}
		else {
		  if (!StitchBuffer->empty()) {
			auto const lastStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
			if (GroupStitchIndex < lastStitch) {
			  GroupStitchIndex++;
			  nuAct(GroupStitchIndex);
			}
		  }
		}
		thred::grpAdj();
		thred::redraw(ColorBar);
	  }
	}
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  nudgfn(IniFile.cursorNudgeStep, 0);
	}
	else {
	  if (StateMap.test(StateFlag::LENSRCH)) {
		longer();
		StateMap.set(StateFlag::RESTCH);
	  }
	  else {
		if (StateMap.test(StateFlag::FRMPSEL)) {
		  ClosestVertexToCursor = form::nxt(FormList->operator[](ClosestFormToCursor), ClosestVertexToCursor);
		  displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
		  auto vertexIt =
		      std::next(FormVertices->cbegin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
		  auto const& vertex = vertexIt[ClosestVertexToCursor];
		  thred::ritfcor(vertex);
		  shftflt(vertex);
		  StateMap.set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap.test(StateFlag::SELBOX)) {
			if (ClosestPointIndex < gsl::narrow<decltype(ClosestPointIndex)>(StitchBuffer->size() - 1U)) {
			  ClosestPointIndex++;
			}
			movbox();
			return true;
		  }
		  if (StateMap.test(StateFlag::GRPSEL)) {
			if (GroupStitchIndex < gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U)) {
			  GroupStitchIndex++;
			  thred::grpAdj();
			  thred::redraw(ColorBar);
			}
		  }
		}
	  }
	}
  }
  retflag = false;
  return {};
}

auto thred::internal::handleLeftKey(bool& retflag) -> bool {
  retflag = true;
  if (wrap::pressed(VK_SHIFT)) {
	if (StateMap.test(StateFlag::FPSEL)) {
	  auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
	  if (!StateMap.test(StateFlag::PSELDIR)) {
		++SelectedFormVertices.vertexCount %= vertexCount;
		SelectedFormVertices.finish =
		    (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
	  }
	  else {
		if (SelectedFormVertices.vertexCount != 0U) {
		  SelectedFormVertices.vertexCount--;
		  SelectedFormVertices.finish =
		      (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
		}
		else {
		  SelectedFormVertices.vertexCount = 1;
		  SelectedFormVertices.finish = (SelectedFormVertices.start + vertexCount - 1) % vertexCount;
		  StateMap.reset(StateFlag::PSELDIR);
		}
	  }
	  thred::setpsel();
	}
	else {
	  if (StateMap.testAndReset(StateFlag::FRMPSEL)) {
		form::unpsel();
		SelectedFormVertices.start       = ClosestVertexToCursor;
		SelectedFormVertices.vertexCount = 1;
		StateMap.reset(StateFlag::PSELDIR);
		thred::setpsel();
	  }
	  else {
		StateMap.reset(StateFlag::LENSRCH);
		StateMap.reset(StateFlag::FORMSEL);
		if (StateMap.testAndReset(StateFlag::SELBOX)) {
		  StateMap.set(StateFlag::GRPSEL);
		  GroupStitchIndex = ClosestPointIndex - 1U;
		}
		else if (GroupStitchIndex != 0U) {
		  GroupStitchIndex--;
		  nuAct(GroupStitchIndex);
		}
		thred::grpAdj();
		thred::redraw(ColorBar);
	  }
	}
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  nudgfn(-IniFile.cursorNudgeStep, 0);
	}
	else {
	  if (StateMap.test(StateFlag::LENSRCH)) {
		shorter();
		StateMap.set(StateFlag::RESTCH);
	  }
	  else {
		if (StateMap.test(StateFlag::FRMPSEL)) {
		  auto form             = FormList->operator[](ClosestFormToCursor);
		  ClosestVertexToCursor = form::prv(form, ClosestVertexToCursor);
		  displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
		  auto        vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
		  auto const& vertex   = vertexIt[ClosestVertexToCursor];
		  thred::ritfcor(vertex);
		  shftflt(vertex);
		  StateMap.set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap.test(StateFlag::SELBOX)) {
			if (ClosestPointIndex != 0U) {
			  ClosestPointIndex--;
			}
			movbox();
			return true;
		  }
		  if (StateMap.test(StateFlag::GRPSEL)) {
			if (GroupStitchIndex != 0U) {
			  GroupStitchIndex--;
			  thred::grpAdj();
			  thred::redraw(ColorBar);
			}
		  }
		}
	  }
	}
  }
  retflag = false;
  return {};
}

auto thred::internal::handleMainWinKeys(uint32_t const&     code,
                                        fPOINT&             rotationCenter,
                                        std::vector<POINT>& stretchBoxLine,
                                        bool&               retflag) -> bool {
  retflag = true;
  switch (code) {
	case VK_ESCAPE: {
	  esccode();
	}
	case 'Q': {
	  if (wrap::pressed(VK_SHIFT)) {
		ritcur();
		return true;
	  }
	  qcode();
	  return true;
	}
	case 0xc0: { //`
	  if ((wrap::pressed(VK_CONTROL)) && wrap::pressed(VK_MENU)) {
		BackgroundColor    = 0x505050;
		BackgroundPen      = nuPen(BackgroundPen, 1, BackgroundColor);
		BackgroundPenWidth = 1;
		DeleteObject(BackgroundBrush);
		BackgroundBrush = CreateSolidBrush(BackgroundColor);
		if (PCSBMPFileName[0] != 0) {
		  bfil();
		}
		StateMap.set(StateFlag::RESTCH);
	  }
	  else {
#if PESACT
		//				IniFile.auxFileType=AUXPES;
		//				strcpy_s(WorkingFileName,"u:\\mrd\\t.thr");
		//				StateMap.set(StateFlag::REDOLD);
		//				nuFil();
		//				lodpes();
		//				savpes();
#endif
		xt::tst();
	  }
	  break;
	}
	case 0x21: { // page up
	  if (wrap::pressed(VK_SHIFT)) {
		form::redup();
	  }
	  else {
		form::rotagain();
	  }
	  break;
	}
	case 0x22: { // page down
	  if (wrap::pressed(VK_SHIFT)) {
		form::bakdup();
	  }
	  else {
		form::bakagain();
	  }
	  break;
	}
	case 0x6d: { // keypad -
	  toglup();
	  break;
	}
	case 0xbd: { //-
	  StateMap.reset(StateFlag::CNTRH);
	  StateMap.reset(StateFlag::CNTRV);
	  if (wrap::pressed(VK_SHIFT)) {
		StateMap.set(StateFlag::CNTRV);
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  StateMap.set(StateFlag::CNTRH);
		}
	  }
	  form::cntrx();
	  break;
	}
	case 0xba: { // semicolon
	  movmrk();
	  break;
	}
	case 0xbb: { //=
	  form::shrnk();
	  break;
	}
	case 0xbc: { // comma
	  if (wrap::pressed(VK_SHIFT)) {
		xt::setfilstrt();
	  }
	  else {
		thred::savdo();
		form::join();
	  }
	  break;
	}
	case 0xbe: { // period
	  if (wrap::pressed(VK_SHIFT)) {
		xt::setfilend();
	  }
	  else {
		setmov();
	  }
	  break;
	}
	case 0xbf: { /// forward slash /
	  formForms::frmnum();
	  break;
	}
	case 0xdb: { //[
	  if (wrap::pressed(VK_SHIFT)) {
		selfrm0();
	  }
	  else {
		ungrplo();
	  }
	  break;
	}
	case 0xdd: { //]
	  if (wrap::pressed(VK_SHIFT)) {
		selfrmx();
	  }
	  else {
		ungrphi();
	  }
	  break;
	}
	case 0xde: { //'
	  desiz();
	  break;
	}
	case VK_F1: {
	  hlp::help();
	  break;
	}
	case VK_F2: {
	  form::snap();
	  break;
	}
	case VK_F3: {
	  if (wrap::pressed(VK_CONTROL)) {
		form::srtfrm();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  form::srtbyfrm();
		}
		else {
		  xt::fsort();
		}
	  }
	  break;
	}
	case VK_F4: {
	  xt::rtrclp();
	  break;
	}
	case VK_F5: {
	  filfrms();
	  break;
	}
	case VK_F6: {
	  vuthrds();
	  break;
	}
	case VK_F7: {
	  thred::save();
	  break;
	}
	case VK_F8: {
	  savAs();
	  break;
	}
	case VK_F9: {
	  selCol();
	  break;
	}
	case VK_F11: {
	  if (wrap::pressed(VK_SHIFT)) {
		rembig();
	  }
	  else {
		duzero();
	  }
	  break;
	}
	case VK_F12: {
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_MENU)) {
		reldun();
	  }
	  else {
#ifdef _DEBUG
		if (wrap::pressed(VK_SHIFT)) {
		  xt::dmpat();
		}
		else {
#endif
		  dun();
#ifdef _DEBUG
		}
#endif
	  }
	  break;
	}
	case VK_INSERT: {
	  if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), UserColorWin->back())) { // check if point is in any of the color windows
		inscol();
	  }
	  break;
	}
	case VK_DELETE: {
	  if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), UserColorWin->back())) {
		delcol();
	  }
	  else {
		delet();
	  }
	  break;
	}
	case VK_TAB: {
	  rot(rotationCenter);
	  break;
	}
	case VK_SPACE: {
	  if (StateMap.testAndFlip(StateFlag::INSRT)) {
		ReleaseCapture();
		StateMap.set(StateFlag::RESTCH);
	  }
	  else {
		istch();
	  }
	  unbox();
	  if (StateMap.testAndReset(StateFlag::GRPSEL) || StateMap.testAndReset(StateFlag::FORMSEL)) {
		StateMap.set(StateFlag::RESTCH);
	  }
	  break;
	}
	case VK_HOME: {
	  auto       homeFlag = true;
	  auto const retval   = handleHomeKey(homeFlag);
	  if (homeFlag) {
		return retval;
	  }
	  break;
	}
	case VK_END: {
	  auto       endFlag = 1;
	  auto const retval  = handleEndKey(endFlag);
	  if (endFlag == 2) {
		break;
	  }
	  if (endFlag == 1) {
		return retval;
	  }
	  break;
	}
	case VK_DOWN: {
	  if (wrap::pressed(VK_CONTROL)) {
		nudgfn(0, -IniFile.cursorNudgeStep);
	  }
	  else {
		seldwn();
	  }
	  break;
	}
	case VK_UP: {
	  if (wrap::pressed(VK_CONTROL)) {
		nudgfn(0, IniFile.cursorNudgeStep);
	  }
	  else {
		selup();
	  }
	  break;
	}
	case VK_RIGHT: {
	  auto       rightFlag = true;
	  auto const retval    = thi::handleRightKey(rightFlag);
	  if (rightFlag) {
		return retval;
	  }
	  break;
	}
	case VK_LEFT: {
	  auto       leftFlag = true;
	  auto const retval   = handleLeftKey(leftFlag);
	  if (leftFlag) {
		return retval;
	  }
	  break;
	}
	case 'A': {
	  if (wrap::pressed(VK_CONTROL)) {
		selalstch();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  form::selal();
		}
		else {
		  zumout();
		}
	  }
	  break;
	}
	case 'B': {
	  if (wrap::pressed(VK_CONTROL)) {
		redo();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  bakmrk();
		}
		else {
		  bak();
		}
	  }
	  break;
	}
	case 'C': {
	  if (wrap::pressed(VK_CONTROL)) {
		duclip();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  StateMap.reset(StateFlag::CNV2FTH);
		  satin::ribon();
		}
		else {
		  StateMap.reset(StateFlag::HIDSTCH);
		  StateMap.set(StateFlag::IGNTHR);
		  rebox();
		  StateMap.reset(StateFlag::IGNTHR);
		  StateMap.set(StateFlag::RESTCH);
		}
	  }
	  break;
	}
	case 'D': {
	  if (wrap::pressed(VK_SHIFT)) {
		if (StateMap.test(StateFlag::FORMSEL)) {
		  PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
		  keybd_event('F', 0, 0, 0);
		}
	  }
	  else {
		satin::satsel();
	  }
	  break;
	}
	case 'E': {
	  if (wrap::pressed(VK_SHIFT)) {
		texture::dutxtfil();
	  }
	  else {
		form::infrm();
	  }
	  break;
	}
	case 'F': {
	  if (!SelectedFormList->empty()) {
		PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
		keybd_event('F', 0, 0, 0);
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  StateMap.set(StateFlag::CNV2FTH);
		  satin::ribon();
		}
		else {
		  form::frmon();
		  if (StateMap.test(StateFlag::FORMSEL)) {
			formForms::refrm();
		  }
		  else {
			form::form();
		  }
		}
	  }
	  break;
	}
	case 'G': {
	  thi::mark();
	  break;
	}
	case 'H': {
	  if (wrap::pressed(VK_SHIFT)) {
		pglft();
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  trace::trcsel();
		}
		else {
		  trace::blak();
		}
	  }
	  break;
	}
	case 'J': {
	  if (wrap::pressed(VK_SHIFT)) {
		pgrit();
	  }
	  else {
		form::refilal();
	  }
	  break;
	}
	case 'K': {
	  if (wrap::pressed(VK_MENU) && wrap::pressed(VK_CONTROL)) {
		setknots();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  tglhid();
		}
		else {
		  if (wrap::pressed(VK_CONTROL)) {
			set1knot();
		  }
		  else {
			form::tglfrm();
		  }
		}
	  }
	  break;
	}
	case 'L': {
	  if (wrap::pressed(VK_SHIFT)) {
		delstch();
	  }
	  else {
		form::fcntr();
	  }
	  break;
	}
	case 'M': {
	  if (wrap::pressed(VK_CONTROL)) {
		pntmrk();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  dumrk(gsl::narrow_cast<float>(UnzoomedRect.x) / 2.0F, gsl::narrow_cast<float>(UnzoomedRect.y) / 2.0F);
		}
		else {
		  if (thred::px2stch()) {
			dumrk(SelectedPoint.x, SelectedPoint.y);
		  }
		}
		StateMap.set(StateFlag::RESTCH);
	  }
	  break;
	}
	case 'N': {
	  if (wrap::pressed(VK_CONTROL)) {
		xt::nudsiz();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  pgdwn();
		}
		else {
		  StateMap.set(StateFlag::TRCUP);
		  trace::trace();
		}
	  }
	  break;
	}
	case 'O': {
	  fop();
	  break;
	}
	case 'P': {
	  formForms::prfmsg();
	  break;
	}
	case 'R': {
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_SHIFT)) {
		thred::movStch();
		unbox();
		StateMap.set(StateFlag::RESTCH);
	  }
	  else {
		if (FormDataSheet == nullptr) {
		  if (wrap::pressed(VK_CONTROL)) {
			form::setrang();
			return true;
		  }
		  if (wrap::pressed(VK_SHIFT)) {
			rotmrk();
			return true;
		  }

		  rotseg();
		}
	  }
	  break;
	}
	case 'S': {
	  if (wrap::pressed(VK_CONTROL)) {
		colchk();
		thred::save();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  gsnap();
		}
		else {
		  zumshft();
		}
	  }
	  break;
	}
	case 'T': {
	  if (wrap::pressed(VK_SHIFT)) {
		retrac();
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  trace::trinit();
		}
		else {
		  thumnail();
		}
	  }
	  break;
	}
	case 'U': {
	  if (wrap::pressed(VK_SHIFT)) {
		pgup();
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  trace::trdif();
		}
		else {
		  StateMap.reset(StateFlag::TRCUP);
		  trace::trace();
		}
	  }
	  break;
	}
	case 'V': {
	  if ((wrap::pressed(VK_CONTROL)) && (OpenClipboard(ThrEdWindow) != 0)) {
		auto       pasteFlag = true;
		auto const retval    = doPaste(stretchBoxLine, pasteFlag);
		if (pasteFlag) {
		  return retval;
		}
	  }
	  else {
		closPnt();
	  }
	  break;
	}
	case 'W': {
	  if (wrap::pressed(VK_SHIFT)) {
		form::crop();
	  }
	  else {
		form::insat();
	  }
	  break;
	}
	case 'X': {
	  if (wrap::pressed(VK_CONTROL)) {
		cut();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  thred::hidbit();
		}
		else {
		  thred::zumhom();
		}
	  }
	  break;
	}
	case 'Y': {
	  if (form::closfrm()) {
		selfpnt();
	  }
	  break;
	}
	case 'Z': {
	  if (wrap::pressed(VK_SHIFT)) {
		ZoomFactor = ZoomMin;
		zumin();
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  bak();
		}
		else {
		  zumin();
		}
	  }
	  break;
	}
	default: {
	}
  }
  retflag = false;
  return {};
}

auto thred::internal::handleNumericInput(uint32_t const& code, bool& retflag) -> bool {
  retflag = true;
  if (StateMap.test(StateFlag::SCLPSPAC) && code == 0xbd && (MsgIndex == 0U)) {
	MsgBuffer[0] = '-';
	MsgBuffer[1] = 0;
	MsgIndex     = 1;
	SetWindowText(GeneralNumberInputBox, static_cast<LPTSTR>(MsgBuffer));
	return true;
  }
  if (dunum(code)) {
	if (StateMap.test(StateFlag::TRNIN0)) {
	  trace::traceNumberInput(NumericCode);
	}
	else {
	  MsgBuffer[MsgIndex++] = NumericCode;
	  MsgBuffer[MsgIndex]   = 0;
	  SetWindowText(GeneralNumberInputBox, static_cast<LPTSTR>(MsgBuffer));
	}
	return true;
  }
  switch (code) {
	case 0x6e:   // numpad period
	case 0xbe: { // period
	  MsgBuffer[MsgIndex++] = '.';
	  MsgBuffer[MsgIndex]   = 0;
	  SetWindowText(GeneralNumberInputBox, static_cast<LPTSTR>(MsgBuffer));
	  return true;
	}
	case 8: { // backspace
	  if (MsgIndex != 0U) {
		MsgIndex--;
		if (StateMap.test(StateFlag::TRNIN0)) {
		  trace::traceNumberReset();
		}
		else {
		  MsgBuffer[MsgIndex] = 0;
		  SetWindowText(GeneralNumberInputBox, static_cast<LPTSTR>(MsgBuffer));
		}
	  }
	  return true;
	}
	case VK_RETURN: {
	  if (StateMap.test(StateFlag::TRNIN0)) {
		trace::dutrnum2();
	  }
	  else {
		if (StateMap.test(StateFlag::TRNIN1)) {
		  trace::dutrnum1();
		}
	  }
	  break;
	}
	default: {
	}
  }
  retflag = false;
  return {};
}

auto thred::internal::handleEditMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_KNOTAT: { // edit / Set / Knot at Selected Stitch
	  set1knot();
	  flag = true;
	  break;
	}
	case ID_FRM2COL: { // edit / Set / Form Color to  stitch Color
	  form::col2frm();
	  flag = true;
	  break;
	}
	case ID_SNAP2GRD: { // edit / Snap / to Grid
	  gsnap();
	  flag = true;
	  break;
	}
	case ID_EDIT_SELECTCOLOR: { // edit / Select / Color
	  selCol();
	  flag = true;
	  break;
	}
	case ID_REPAIR: { // edit / Repair Data
	  repair::repar();
	  flag = true;
	  break;
	}
	case ID_FRMIND: { // edit / Form Update / Indent
	  xt::setfind();
	  flag = true;
	  break;
	}
	case ID_SETSIZ: { // edit / Set / Design Size
	  xt::nudsiz();
	  flag = true;
	  break;
	}
	case ID_FRMHI: { // edit / Form Update / Height
	  xt::setfhi();
	  flag = true;
	  break;
	}
	case ID_FRMWID: { // edit / Form Update / Width
	  xt::setfwid();
	  flag = true;
	  break;
	}
	case ID_MAXFLEN: { // edit / Form Update / Fill /  Maximum Stitch Length
	  xt::setfmax();
	  flag = true;
	  break;
	}
	case ID_MINFLEN: { // edit / Form Update / Fill /  Minimum Stitch Length
	  xt::setfmin();
	  flag = true;
	  break;
	}
	case ID_MAXBLEN: { // edit / Form Update / Border /  Maximum Stitch Length
	  xt::setbmax();
	  flag = true;
	  break;
	}
	case ID_MINBLEN: { // edit / Form Update / Border /  Minimum Stitch Length
	  xt::setbmin();
	  flag = true;
	  break;
	}
	case ID_SETBSPAC: { // edit / Form Update / Border /  Spacing
	  xt::setbspac();
	  flag = true;
	  break;
	}
	case ID_SETBLEN: { // edit / Form Update / Border /  Stitch Length
	  xt::setblen();
	  flag = true;
	  break;
	}
	case ID_SETBCOL: { // edit / Form Update / Border /  Color
	  xt::setbcol();
	  flag = true;
	  break;
	}
	case ID_SETFCOL: { // edit / Form Update / Fill /  Color
	  xt::setfcol();
	  flag = true;
	  break;
	}
	case ID_SETUCOL: { // edit / Form Update / Underlay /  Color
	  xt::setucol();
	  flag = true;
	  break;
	}
	case ID_SETFANG: { // edit / Form Update / Fill /  Angle
	  xt::setfang();
	  flag = true;
	  break;
	}
	case ID_SETFSPAC: { // edit / Form Update / Fill /  Spacing
	  xt::setfspac();
	  flag = true;
	  break;
	}
	case ID_SETFLEN: { // edit / Form Update / Fill /  Stitch Length
	  xt::setflen();
	  flag = true;
	  break;
	}
	case ID_SETUANG: { // edit / Form Update / Underlay /  Angle
	  xt::sfuang();
	  flag = true;
	  break;
	}
	case ID_SETUSPAC: { // edit / Form Update / Underlay /  Spacing
	  xt::uspac();
	  flag = true;
	  break;
	}
	case ID_UNDLEN: { // edit / Form Update / Underlay / Stitch Length
	  xt::undlen();
	  flag = true;
	  break;
	}
	case ID_SETCWLK: { // edit / Form Update / Center Walk / On
	  xt::setcwlk();
	  flag = true;
	  break;
	}
	case ID_SETWLK: { // edit / Form Update / Edge Walk /  On
	  xt::setwlk();
	  flag = true;
	  break;
	}
	case ID_SETUND: { // edit / Form Update / Underlay / On
	  xt::setund();
	  flag = true;
	  break;
	}
	case ID_NOTCWLK: { // edit / Form Update / Center Walk / Off
	  xt::notcwlk();
	  flag = true;
	  break;
	}
	case ID_NOTWLK: { // edit / Form Update / Edge Walk / Off
	  xt::notwlk();
	  flag = true;
	  break;
	}
	case ID_NOTUND: { // edit / Form Update / Underlay / Off
	  xt::notund();
	  flag = true;
	  break;
	}
	case ID_SELUND: { // edit / Select / Form Underlay Stitches
	  form::selfil(UNDMSK);
	  flag = true;
	  break;
	}
	case ID_SELWLK: { // edit / Select / Form Edge Walk Stitches
	  form::selfil(WLKMSK);
	  flag = true;
	  break;
	}
	case ID_ALFRM: { // edit / Select / All Forms
	  xt::selalfrm();
	  flag = true;
	  break;
	}
	case ID_FILSTRT: { // Edit / Set / Fill Start Point
	  xt::setfilstrt();
	  flag = true;
	  break;
	}
	case ID_FILEND: { // Edit / Set / Fill End Point
	  xt::setfilend();
	  flag = true;
	  break;
	}
	case ID_2FTHR: { // edit / Convert / to Feather Ribbon
	  StateMap.set(StateFlag::CNV2FTH);
	  satin::ribon();
	  flag = true;
	  break;
	}
	case ID_FTHDEF: { // edit / Set / Feather Defaults
	  dufdef();
	  flag = true;
	  break;
	}
	case ID_SRTF: { // edit / Sort / by Form
	  form::srtfrm();
	  flag = true;
	  break;
	}
	case ID_CROP: { // edit / Crop to Form
	  form::crop();
	  flag = true;
	  break;
	}
	case ID_BLAK: { // edit / Trace / Reset Form Pixels
	  trace::blak();
	  flag = true;
	  break;
	}
	case ID_TRDIF: { // edit / Trace / Find Edges
	  trace::trdif();
	  flag = true;
	  break;
	}
	case ID_TRACEDG: { // edit / Trace / Show Traced Edges
	  trace::tracedg();
	  flag = true;
	  break;
	}
	case ID_TRCSEL: { // edit / Trace / Select Colors
	  trace::trcsel();
	  flag = true;
	  break;
	}
	case ID_TRACE: { // edit / Trace / Trace Mode
	  trace::trinit();
	  flag = true;
	  break;
	}
	case ID_RETRACE: { // edit / Retrace
	  retrac();
	  flag = true;
	  break;
	}
	case ID_DELKNOT: { // edit / Delete / Knots
	  delknot();
	  flag = true;
	  break;
	}
	case ID_STCHS2FRM: { // edit / Convert / Stitches to Form
	  form::stchs2frm();
	  flag = true;
	  break;
	}
	case ID_SPLTFRM: { // edit / Split Form
	  form::spltfrm();
	  flag = true;
	  break;
	}
	case ID_UNBEAN: { // edit / Convert / from Bean to Line
	  form::debean();
	  flag = true;
	  break;
	}
	case ID_DUBEAN: { // edit / Convert / to Bean
	  form::dubean();
	  flag = true;
	  break;
	}
	case ID_SRTBF: { // edit / Sort / by Color then Form
	  form::srtbyfrm();
	  flag = true;
	  break;
	}
	case ID_CENTIRE: { // edit / Center / Entire Design
	  form::centir();
	  flag = true;
	  break;
	}
	case ID_CNTRX: { // edit / Center / Both
	  StateMap.reset(StateFlag::CNTRH);
	  StateMap.reset(StateFlag::CNTRV);
	  form::cntrx();
	  flag = true;
	  break;
	}
	case ID_CNTRH: { // edit / Center / Horizontal
	  StateMap.reset(StateFlag::CNTRH);
	  StateMap.set(StateFlag::CNTRV);
	  form::cntrx();
	  flag = true;
	  break;
	}
	case ID_CNTRV: { // edit / Center / Vertical
	  StateMap.set(StateFlag::CNTRH);
	  StateMap.reset(StateFlag::CNTRV);
	  form::cntrx();
	  flag = true;
	  break;
	}
	case ID_FRMNUM: { // edit / Set / Form Number
	  formForms::frmnum();
	  flag = true;
	  break;
	}
	case ID_MRKPNT: { // edit / Set / Zoom Mark at Selected Point
	  pntmrk();
	  flag = true;
	  break;
	}
	case ID_ROTSEG: { // edit / Set / Rotation / Segments
	  rotseg();
	  flag = true;
	  break;
	}
	case ID_ROTMRK: { // edit / Set / Rotation / Angle from Mark
	  rotmrk();
	  flag = true;
	  break;
	}
	case ID_MRKCNTR: { // edit / Set / Zoom Mark at Center
	  dumrk(gsl::narrow_cast<float>(UnzoomedRect.x) / 2.0F, gsl::narrow_cast<float>(UnzoomedRect.y) / 2.0F);
	  StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_SETROT: { // edit / Set / Rotation / Angle
	  form::setrang();
	  flag = true;
	  break;
	}
	case ID_REMBIG: { // edit / Delete / Large Stitches
	  rembig();
	  flag = true;
	  break;
	}
	case ID_SHRINK: { // edit / Shrink Clipboard Border
	  form::shrnk();
	  flag = true;
	  break;
	}
	case ID_DUPAGAIN: { // edit / Rotate / and Duplicate again
	  form::redup();
	  flag = true;
	  break;
	}
	case ID_CLPADJ: { // edit / Set / Range Ends for Clipboard Fills
	  clpadj();
	  flag = true;
	  break;
	}
	case ID_DELTOT: { // edit / Delete / All Forms and Stitches
	  deltot();
	  flag = true;
	  break;
	}
	case ID_RIBON: { // edit / Convert / to Satin Ribbon
	  StateMap.reset(StateFlag::CNV2FTH);
	  satin::ribon();
	  flag = true;
	  break;
	}
	case ID_CNTR: { // edit / Center / Forms
	  form::fcntr();
	  flag = true;
	  break;
	}
	case ID_SELALSTCH: { // edit / Select / All Stitches
	  selalstch();
	  flag = true;
	  break;
	}
	case ID_UNGRPLO: { // edit / Ungroup / First
	  ungrplo();
	  flag = true;
	  break;
	}
	case ID_UNGRPHI: { // edit / Ungroup / Last
	  ungrphi();
	  flag = true;
	  break;
	}
	case ID_MOVMRK: { // edit / Move / to Mark
	  movmrk();
	  flag = true;
	  break;
	}
	case ID_SELFIL: { // edit / Select / Form Fill Stitches
	  form::selfil(FRMFIL);
	  flag = true;
	  break;
	}
	case ID_SELBRD: { // edit / Select / Form Border Stitches
	  form::selfil(FRMBFIL);
	  flag = true;
	  break;
	}
	case ID_SELAP: { // edit / Select / Form Applique Stitches
	  form::selfil(FRMAPFIL);
	  flag = true;
	  break;
	}
	case ID_SELFSTCHS: { // edit / Select / Form Stitches
	  form::selalfil();
	  flag = true;
	  break;
	}
	case ID_SETMRK: { // edit / Set / Order Mark
	  setmov();
	  flag = true;
	  break;
	}
	case ID_DELFRE: { // edit / Delete / Free Stitches
	  delfre();
	  flag = true;
	  break;
	}
	case ID_SELAL: { // edit / Select / All Forms and Stitches
	  form::selal();
	  flag = true;
	  break;
	}
	case ID_REFILAL: { // edit / Refill All
	  form::refilal();
	  flag = true;
	  break;
	}
	case ID_CHK: { // edit / Check Range
	  thred::chkrng(StitchRangeSize);
	  StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_RTRVCLP: { // edit / Retrieve Clipboard Stitches
	  xt::rtrclp();
	  flag = true;
	  break;
	}
	case ID_SORT: { // edit / Sort / Auto
	  xt::fsort();
	  flag = true;
	  break;
	}
	case ID_LAYMOV0: { // edit / Move to Layer / 0
	  form::movlayr(0);
	  flag = true;
	  break;
	}
	case ID_LAYMOV1: { // edit / Move to Layer / 1
	  form::movlayr(2U);
	  flag = true;
	  break;
	}
	case ID_LAYMOV2: { // edit / Move to Layer / 2
	  form::movlayr(4U);
	  flag = true;
	  break;
	}
	case ID_LAYMOV3: { // edit / Move to Layer / 3
	  form::movlayr(6U);
	  break;
	}
	case ID_LAYMOV4: { // edit / Move to Layer / 4
	  form::movlayr(8U);
	  flag = true;
	  break;
	}
	case ID_LAYCPY0: { // edit / Copy to Layer / 0
	  form::cpylayr(0U);
	  flag = true;
	  break;
	}
	case ID_LAYCPY1: { // edit / Copy to Layer / 1
	  form::cpylayr(2U);
	  flag = true;
	  break;
	}
	case ID_LAYCPY2: { // edit / Copy to Layer / 2
	  form::cpylayr(4U);
	  flag = true;
	  break;
	}
	case ID_LAYCPY3: { // edit / Copy to Layer / 3
	  form::cpylayr(6U);
	  flag = true;
	  break;
	}
	case ID_LAYCPY4: { // edit / Copy to Layer / 4
	  form::cpylayr(8U);
	  flag = true;
	  break;
	}
	case ID_ROTDUP: { // edit / Rotate / and Duplicate
	  form::rotdup();
	  flag = true;
	  break;
	}
	case ID_ROTAGAIN: { // edit / Rotate / Again
	  form::rotagain();
	  flag = true;
	  break;
	}
	case ID_ROTCMD: { // edit / Rotate / Command
	  form::rotcmd();
	  flag = true;
	  break;
	}
	case ID_DELFRMS: { // edit / Delete / All Forms
	  form::delfrms();
	  rstAll();
	  StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_SNAP2: { // edit / Snap / Together
	  form::snap();
	  flag = true;
	  break;
	}
	case ID_FLPORD: { // edit / Flip / Order
	  form::flpord();
	  flag = true;
	  break;
	}
	case ID_MV2BAK: { // edit / move / to End
	  mv2b();
	  flag = true;
	  break;
	}
	case ID_MV2FRNT: { // edit / move / to Start
	  mv2f();
	  flag = true;
	  break;
	}
	case ID_DELETE: { // edit / Delete / Selected
	  delet();
	  flag = true;
	  break;
	}
	case ID_FLIPH: { // edit / Flip / Horizontal
	  form::fliph();
	  flag = true;
	  break;
	}
	case ID_FLIPV: { // edit / Flip / Vertical
	  thred::savdo();
	  form::flipv();
	  flag = true;
	  break;
	}
	case ID_FRM0: { // edit / Set / Form Zero Point
	  form::frm0();
	  flag = true;
	  break;
	}
	case ID_HIDBIT: { // edit / Trace / Hide Bitmap
	  thred::hidbit();
	  flag = true;
	  break;
	}
	case ID_DELSTCH: { // edit / Delete / All Stitches
	  delstch();
	  flag = true;
	  break;
	}
	case ID_KNOTS: { // edit / Set / Knots
	  setknots();
	  flag = true;
	  break;
	}
	case ID_REMZERO: { // edit / Delete / Small Stitches
	  duzero();
	  flag = true;
	  break;
	}
	case ID_EDIT_RESET_COL: { // edit / Reset Colors
	  for (auto iColor = 0U; iColor < 16U; iColor++) {
		UserColor[iColor]      = DefaultColors[iColor];
		UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
		UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
		thred::redraw(UserColorWin->operator[](iColor));
	  }
	  StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_DESNAM: { // edit / Set / Designer Name
	  getdes();
	  flag = true;
	  break;
	}
	default: {
	}
  }
  return flag;
}

auto thred::internal::handleViewMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_WARNOF: // view / set / Warn if edited
	  xt::chgwrn();
	  flag = true;
	  break;
	case ID_CLPSPAC: { // view / set / Clipboard Fill  Spacing
	  xt::setclpspac();
	  flag = true;
	  break;
	}
	case ID_FIL2SEL_ON: { // view / Set / Fill at Select / On
	  fil2sel(1);
	  flag = true;
	  break;
	}
	case ID_FIL2SEL_OFF: { // view / Set / Fill at Select / Off
	  fil2sel(0);
	  flag = true;
	  break;
	}
	case ID_USPAC: { // view / Set / Underlay / Spacing
	  xt::setuspac();
	  flag = true;
	  break;
	}
	case ID_UANG: { // view / Set / Underlay / Angle
	  xt::setuang();
	  flag = true;
	  break;
	}
	case ID_USTCH: { // view / Set / Underlay / Stitch Length
	  xt::setulen();
	  flag = true;
	  break;
	}
	case ID_WIND: { // view / Set / Underlay / Indent
	  xt::setwlkind();
	  flag = true;
	  break;
	}
	case ID_FRMX: { // view / Set / Form Cursor / Cross
	  frmcursel(1);
	  flag = true;
	  break;
	}
	case ID_FRMBOX: { // view / Set / Form Cursor / Box
	  frmcursel(0);
	  flag = true;
	  break;
	}
	case ID_ROTAUXON: { // view / Set / Rotate Machine File / On
	  rotauxsel(1);
	  flag = true;
	  break;
	}
	case ID_ROTAUXOFF: { // view / Set / Rotate Machine File / Off
	  rotauxsel(0);
	  flag = true;
	  break;
	}
	case ID_STCHPIX: { // view / Set / Point Size / Stitch Point Boxes
	  getstpix();
	  flag = true;
	  break;
	}
	case ID_FRMPIX: { // view / Set / Point Size / Form Point Triangles
	  getfrmpix();
	  flag = true;
	  break;
	}
	case ID_FRMPBOX: { // view / Set / Point Size / Form Box
	  getfrmbox();
	  flag = true;
	  break;
	}
	case ID_BAKMRK: { // view / Retrieve Mark
	  bakmrk();
	  flag = true;
	  break;
	}
	case ID_MARKESC: { // view / Set / Retrieve Mark / Escape
	  UserFlagMap.set(UserFlag::MARQ);
	  qchk();
	  flag = true;
	  break;
	}
	case ID_MARKQ: { // view / Set / Retrieve Mark / Q
	  UserFlagMap.reset(UserFlag::MARQ);
	  qchk();
	  flag = true;
	  break;
	}
	case ID_NUDGPIX: { // view / Set / Nudge Pixels
	  getnpix();
	  flag = true;
	  break;
	}
	case ID_GRDHI: { // view / Set / Grid Mask / High
	  setgrd(HIGRD);
	  flag = true;
	  break;
	}
	case ID_GRDMED: { // view / Set / Grid Mask / Medium
	  setgrd(MEDGRD);
	  flag = true;
	  break;
	}
	case ID_GRDEF: { // view / Set / Grid Mask / Default
	  setgrd(DEFGRD);
	  flag = true;
	  break;
	}
	case ID_GRDRED: { // view / Set / Grid Mask / UnRed
	  setgrd(REDGRD);
	  flag = true;
	  break;
	}
	case ID_GRDBLU: { // view / Set / Grid Mask / UnBlue
	  setgrd(BLUGRD);
	  flag = true;
	  break;
	}
	case ID_GRDGRN: { // view / Set / Grid Mask / UnGreen
	  setgrd(GRNGRD);
	  flag = true;
	  break;
	}
	case ID_LINBEXACT: { // view / Set / Line Border Spacing / Exact
	  UserFlagMap.reset(UserFlag::LINSPAC);
	  linbmen();
	  flag = true;
	  break;
	}
	case ID_LINBEVEN: { // view / Set / Line Border Spacing / Even
	  UserFlagMap.set(UserFlag::LINSPAC);
	  linbmen();
	  flag = true;
	  break;
	}
	case ID_BSAVON: { // view / Set / PCS Bitmap Save / On
	  pcsbsavon();
	  flag = true;
	  break;
	}
	case ID_BSAVOF: { // view / Set / PCS Bitmap Save / Off
	  pcsbsavof();
	  flag = true;
	  break;
	}
	case ID_KNOTON: { // view / Knots / On
	  shoknot();
	  flag = true;
	  break;
	}
	case ID_KNOTOF: { // view / Knots / Off
	  hidknot();
	  flag = true;
	  break;
	}
	case ID_RSTNEDL: { // view / Set / Needle Cursor / Off
	  nedof();
	  flag = true;
	  break;
	}
	case ID_SETNEDL: { // view / Set / Needle Cursor / On
	  nedon();
	  flag = true;
	  break;
	}
	case ID_SETPREF: { // view / Set / Default Preferences
	  defpref();
	  flag = true;
	  break;
	}
	case ID_AUXPCS: { // view / Set / Machine File Type / Pfaff PCS
	  setpcs();
	  flag = true;
	  break;
	}
#if PESACT
	case ID_AUXPES: { // view / Set / Machine File Type / Brother PES
	  setpes();
	  flag = true;
	  break;
	}
#endif
	case ID_AUXDST: { // view / Set / Machine File Type / Tajima DST
	  setdst();
	  flag = true;
	  break;
	}
	case ID_ABOUT: { // view / About ThrEd4
	  displayText::tabmsg(IDS_CPYRIT);
	  flag = true;
	  break;
	}
	case ID_DESIZ: { // view / Design Information
	  desiz();
	  flag = true;
	  break;
	}
	case ID_VUSELTHRDS: { // view / Show Threads for Selected Color
	  vuselthr();
	  flag = true;
	  break;
	}
	case ID_VUTHRDS: { // view / Show Threads
	  vuthrds();
	  flag = true;
	  break;
	}
	case ID_VUBAK: { // view / Backups
	  vubak();
	  flag = true;
	  break;
	}
	case ID_SETAP: { // view / Set / Applique Color
	  form::setap();
	  flag = true;
	  break;
	}
	case ID_RUNPAT: { // view / Movie
	  thi::movi();
	  flag = true;
	  break;
	}
	case ID_CHKOF: { // view / Set / Data check / Off
	  xt::chgchk(0);
	  flag = true;
	  break;
	}
	case ID_CHKON: { // view / Set / Data check / On
	  xt::chgchk(1);
	  flag = true;
	  break;
	}
	case ID_CHKREP: { // view / Set / Data Check / Auto Repair on
	  xt::chgchk(2);
	  flag = true;
	  break;
	}
	case ID_CHKREPMSG: { // view / Set / Data Check / Auto Repair with Message
	  xt::chgchk(3);
	  flag = true;
	  break;
	}
	case ID_LENDEF: { // view / Set / Default Stitch Lengths
	  IniFile.maxStitchLength = MAXSIZ * PFGRAN;
	  UserStitchLength        = USESIZ * PFGRAN;
	  MinStitchLength         = MINSIZ * PFGRAN;
	  flag                    = true;
	  break;
	}
	case ID_TSIZDEF: { // view / Thread Size / Set Defaults
	  ThreadSize30 = TSIZ30;
	  ThreadSize40 = TSIZ40;
	  ThreadSize60 = TSIZ60;
	  flag         = true;
	  break;
	}
	case ID_SIZ30: { // view / Thread Size / 30
	  displayText::tsizmsg(L"30", ThreadSize30);
	  StateMap.set(StateFlag::ENTR30);
	  flag = true;
	  break;
	}
	case ID_SIZ40: { // view / Thread Size / 40
	  displayText::tsizmsg(L"40", ThreadSize40);
	  StateMap.set(StateFlag::ENTR40);
	  flag = true;
	  break;
	}
	case ID_SIZ60: { // view / Thread Size / 60
	  displayText::tsizmsg(L"60", ThreadSize60);
	  StateMap.set(StateFlag::ENTR60);
	  flag = true;
	  break;
	}
	case ID_VIEW_STCHBAK: { // view / Set / Background Color
	  if (nuBak() != 0U) {
		BackgroundColor    = BackgroundColorStruct.rgbResult;
		BackgroundPen      = nuPen(BackgroundPen, 1, BackgroundColor);
		BackgroundPenWidth = 1;
		DeleteObject(BackgroundBrush);
		BackgroundBrush = CreateSolidBrush(BackgroundColor);
		if (PCSBMPFileName[0] != 0) {
		  bfil();
		}
		thred::zumhom();
	  }
	  flag = true;
	  break;
	}
	case ID_BITCOL: { // view / Set / Bitmap Color
	  if (nuBit() != 0U) {
		BitmapColor = fswap(BitMapColorStruct.rgbResult);
		if (PCSBMPFileName[0] != 0) {
		  bfil();
		}
		BitmapPen = nuPen(BitmapPen, 1, BitmapColor);
		thred::zumhom();
	  }
	  flag = true;
	  break;
	}
	case VU_ZUMFUL: { // view / Zoom Full
	  thred::zumhom();
	  flag = true;
	  break;
	}
	default: {
	}
  }
  return flag;
}

auto thred::internal::handleFileMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_CLOSE: { // file / Close
	  filclos();
	  flag = true;
	  break;
	}
	case ID_DELMAP: { // file / Remove Bitmap
	  delmap();
	  flag = true;
	  break;
	}
	case ID_SAVMAP: { // file / Save Bitmap
	  savmap();
	  flag = true;
	  break;
	}
	case ID_FLOK: { // file / Locking
	  lock();
	  flag = true;
	  break;
	}
	case ID_OVRLAY: { // file / Overlay
	  ovrlay();
	  flag = true;
	  break;
	}
	case ID_INSFIL: { // file / Insert
	  StateMap.reset(StateFlag::IGNORINS);
	  insfil();
	  flag = true;
	  break;
	}
	case ID_THUM: { // file / Thumbnails
	  thumnail();
	  flag = true;
	  break;
	}
	case ID_PURGDIR: { // file / Delete Backups / All backups in the selected directory
	  purgdir();
	  flag = true;
	  break;
	}
	case ID_PURG: { // file / Delete Backups / Backups for the selected file
	  purg();
	  flag = true;
	  break;
	}
	case ID_OPNPCD: { // file / Open Auxiliary file
	  switch (IniFile.auxFileType) {
		case AUXDST: {
		  OpenFileName.nFilterIndex = 3;
		  break;
		}
		default: {
		  OpenFileName.nFilterIndex = 2;
		}
	  }
	  nuFil();
	  nulayr(0);
	  flag = true;
	  break;
	}
	case ID_HIDBITF: { // file / Hide Bitmap
	  thred::hidbit();
	  flag = true;
	  break;
	}
	case ID_LODBIT: { // file / Load Bitmap
	  lodbmp();
	  flag = true;
	  break;
	}
	case ID_FILE_OPEN1: { // file / Open
	  fop();
	  flag = true;
	  break;
	}
	case ID_FILE_SAVE3: { // file / Save As
	  colchk();
	  savAs();
	  flag = true;
	  break;
	}
	case ID_FILE_SAVE2: { // file / Save
	  colchk();
	  thred::save();
	  flag = true;
	  break;
	}
	case ID_FILE_NEW1: { // file / New
	  if (!savcmp()) {
		displayText::savdisc();
		StateMap.set(StateFlag::NEWBAK);
		StateMap.reset(StateFlag::PRFACT);
		thred::undat();
	  }
	  else {
		newFil();
		nulayr(0);
	  }
	  flag = true;
	  break;
	}
	default: {
	}
  }
  return flag;
}

auto thred::internal::handleFillMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_BOLD: { // fill / Border / Bean
	  if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::dubold();
	  flag = true;
	  break;
	}
	case ID_TXFIL: { // Fill / Texture Editor
	  texture::dutxtfil();
	  flag = true;
	  break;
	}
	case ID_FETHR: { // fill / Feather
	  xt::fethr();
	  flag = true;
	  break;
	}
	case ID_FILCLPX: { // fill / Border / Clipboard, Even
	  form::filclpx();
	  flag = true;
	  break;
	}
	case ID_LINCHN: { // fill / Border / Line chain
	  StateMap.set(StateFlag::LINCHN);
	  form::chain();
	  flag = true;
	  break;
	}
	case ID_OPNCHN: { // fill / Border / Open chain
	  StateMap.reset(StateFlag::LINCHN);
	  form::chain();
	  flag = true;
	  break;
	}
	case ID_DUBFIL: { // fill / Border / Double
	  form::dubfil();
	  flag = true;
	  break;
	}
	case ID_HORCLP: { // fill / Clipboard / Horizontal
	  if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::horclp();
	  flag = true;
	  break;
	}
	case ID_ANGCLP: { // fill / Clipboard / Angle
	  if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::angclp();
	  flag = true;
	  break;
	}
	case ID_VRTCLP: { // fill / Clipboard / Vertical
	  if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::vrtclp();
	  flag = true;
	  break;
	}
	case ID_REFILF: { // fill / Refill
	  filfrms();
	  flag = true;
	  break;
	}
	case ID_CONTF: { // fill / Contour
	  form::contfil();
	  flag = true;
	  break;
	}
	case ID_PICOT: { // fill / Border / Picot
	  form::picot();
	  flag = true;
	  break;
	}
	case ID_FILBUT: { // fill / Border / Buttonhole
	  form::bhol();
	  flag = true;
	  break;
	}
	case ID_CLPFIL: { // fill / Clipboard / Fan
	  form::clpfil();
	  flag = true;
	  break;
	}
	case ID_PERP: { // fill / Border / Perpendicular Satin
	  if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::prpbrd(LineSpacing);
	  flag = true;
	  break;
	}
	case ID_FILANG: { // fill / Angle
	  if (StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::filangl();
	  flag = true;
	  break;
	}
	case ID_APLIQ: { // fill / Border / Applique
	  if (StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::apliq();
	  flag = true;
	  break;
	}
	case ID_SATBRD: { // fill / Border / Angle Satin
	  if (StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  satin::satbrd();
	  flag = true;
	  break;
	}
	case ID_FILCLP: { // fill / Border / Clipboard
	  form::fclp();
	  flag = true;
	  break;
	}
	case ID_FILIN: { // fill / Border / Line
	  if (StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::bord();
	  flag = true;
	  break;
	}
	case ID_UNFIL: { // fill / Unfill
	  thred::savdo();
	  form::unfil();
	  thred::coltab();
	  StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_FILSAT: { // fill / Fan
	  StateMap.reset(StateFlag::FTHR);
	  form::filsat();
	  flag = true;
	  break;
	}
	case ID_FILL_VERT: { // fill / Vertical
	  if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::filvrt();
	  flag = true;
	  break;
	}
	case ID_FILL_HOR: { // fill / Horizontal
	  if (StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::filhor();
	  flag = true;
	  break;
	}
	default: {
	}
  }
  return flag;
}

auto thred::internal::handleMainMenu(WORD const& wParameter, fPOINT& rotationCenter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_HLP: { // help
	  hlp::help();
	  flag = true;
	  break;
	}
	case ID_LA: { // all (Show all layers)
	  nulayr(0);
	  flag = true;
	  break;
	}
	case ID_L1: { // 1 (Show layer 1 only)
	  nulayr(1);
	  flag = true;
	  break;
	}
	case ID_L2: { // 2 (Show layer 2 only)
	  nulayr(2);
	  flag = true;
	  break;
	}
	case ID_L3: { // 3 (Show layer 3 only)
	  nulayr(3);
	  flag = true;
	  break;
	}
	case ID_L4: { // 4 (Show layer 4 only)
	  nulayr(4);
	  flag = true;
	  break;
	}
	case ID_FRMOF: { // frm+
	  if (wrap::pressed(VK_SHIFT)) {
		tglhid();
	  }
	  else {
		form::tglfrm();
	  }
	  flag = true;
	  break;
	}
	case ID_PREF: { // pref
	  formForms::prfmsg();
	  flag = true;
	  break;
	}
	case ID_ADEND: { // add
	  if (!StitchBuffer->empty()) {
		stch2px1(wrap::toUnsigned(StitchBuffer->size()) - 1U);
	  }
	  endpnt();
	  StateMap.set(StateFlag::BAKEND);
	  StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_REDO: { // redo
	  redo();
	  flag = true;
	  break;
	}
	case ID_FORM: { // Form
	  form::frmon();
	  if (StateMap.test(StateFlag::FORMSEL)) {
		formForms::refrm();
	  }
	  else {
		form::form();
	  }
	  flag = true;
	  break;
	}
	case ID_ROT: { // rot
	  rot(rotationCenter);
	  flag = true;
	  break;
	}
	case ZUMIN: { // in
	  if (StateMap.test(StateFlag::GMRK) || StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::INSRT) ||
	      StateMap.test(StateFlag::GRPSEL) || StateMap.test(StateFlag::FORMSEL)) {
		zumin();
	  }
	  else {
		StateMap.reset(StateFlag::BZUM);
		StateMap.set(StateFlag::BOXZUM);
		StateMap.reset(StateFlag::BZUMIN);
		StateMap.set(StateFlag::VCAPT);
		SetCapture(ThrEdWindow);
	  }
	  flag = true;
	  break;
	}
	case ID_ZUMOUT: { // out
	  zumout();
	  flag = true;
	  break;
	}
	case ID_BACK: { // undo
	  bak();
	  flag = true;
	  break;
	}
	default: {
	}
  }
  return flag;
}

auto thred::internal::chkMsg(std::vector<POINT>& stretchBoxLine,
                             float&              xyRatio,
                             float&              angle,
                             fPOINT&             rotationCenter,
                             FRMHED&             textureForm) -> bool {
  if (Msg.message == WM_MOUSEMOVE) {
	return thi::handleMouseMove(stretchBoxLine, xyRatio, angle, rotationCenter, textureForm);
  }
  if (Msg.message == WM_LBUTTONUP) {
	auto       retflag = true;
	auto const retval  = thi::handleLeftButtonUp(xyRatio, angle, rotationCenter, retflag);
	if (retflag) {
	  return retval;
	}
  }
  if (Msg.message == WM_RBUTTONDOWN || Msg.message == WM_LBUTTONDOWN) {
	auto       retflag = true;
	auto const retval  = thi::handleEitherButtonDown(retflag);
	if (retflag) {
	  return retval;
	}
  }
  if (Msg.message == WM_RBUTTONDOWN) {
	return thi::handleRightButtonDown();
  }
  if (Msg.message == WM_LBUTTONDOWN) {
	{
	  auto       retflag = true;
	  auto const retval  = thi::handleLeftButtonDown(stretchBoxLine, xyRatio, textureForm, retflag);
	  if (retflag) {
		return retval;
	  }
	}
  }
  switch (Msg.message) {
	case WM_TIMER: {
	  if (StateMap.test(StateFlag::RUNPAT) && (Msg.wParam == 0U)) {
		stchout();
	  }
	  break;
	}
	case WM_CHAR: {
	  if (isgraph(gsl::narrow<int32_t>(Msg.wParam)) != 0) {
		nuthum(gsl::narrow<char>(tolower(gsl::narrow<int32_t>(Msg.wParam))));
	  }
	  break;
	}
	case WM_KEYDOWN: {
	  auto const code = gsl::narrow<uint32_t>(Msg.wParam & 0xffffU);
	  if (StateMap.test(StateFlag::TXTRED)) {
		texture::txtkey(code, textureForm);
		return true;
	  }

	  // ToDo - value passed to duform is wierd because it is dependant on order of enumeration of the form types.
	  //        and value 'SAT' throws it off
	  if (StateMap.test(StateFlag::FORMIN)) {
		if (wrap::pressed(VK_CONTROL)) {
		  return true;
		}

		switch (code) {
		  case 'E': {
			StateMap.reset(StateFlag::FORMIN);
			thred::unmsg();
			form::duform(FRMLINE - 1);
			return true;
		  }
		  case 'F': {
			StateMap.reset(StateFlag::FORMIN);
			thred::unmsg();
			form::duform(FRMFPOLY - 1);
			return true;
		  }
		  case 'R': {
			form::duform(FRMRPOLY - 1);
			return true;
		  }
		  case 'S': {
			form::duform(FRMSTAR - 1);
			return true;
		  }
		  case 'A': {
			form::duform(FRMSPIRAL - 1);
			return true;
		  }
		  case 'H': {
			form::duform(FRMHEART - 2);
			return true;
		  }
		  case 'L': {
			form::duform(FRMLENS - 2);
			return true;
		  }
		  case 'G': {
			form::duform(FRMEGG - 2);
			return true;
		  }
		  case 'T': {
			form::duform(FRMTEAR - 2);
			return true;
		  }
		  case 'Z': {
			form::duform(FRMZIGZAG - 2);
			return true;
		  }
		  case 'W': {
			form::duform(FRMWAVE - 2);
			return true;
		  }
		  case 'D': {
			form::duform(FRMDAISY - 2);
			return true;
		  }
		  default: {
		  }
		}
	  }
	  if (StateMap.test(StateFlag::FILMSG)) {
		if (code == VK_RETURN || code == 0xc0) { // check for return or back tick '`'
		  thred::savdo();
		  form::unfil();
		  thred::coltab();
		  StateMap.set(StateFlag::RESTCH);
		  thred::unmsg();
		  return true;
		}
	  }
	  if (StateMap.testAndReset(StateFlag::MOVMSG)) {
		if (code == VK_RETURN || code == 0xc0) {
		  thred::savdo();
		  form::refilfn();
		  thred::unmsg();
		}
		else {
		  thred::unmsg();
		}
		if (StateMap.testAndReset(StateFlag::WASFRMFRM)) {
		  formForms::refrm();
		}
		return true;
	  }
	  if (StateMap.testAndReset(StateFlag::PRGMSG)) {
		if (code == VK_RETURN || code == 0xc0) {
		  deldir();
		  return true;
		}
	  }
	  if (StateMap.testAndReset(StateFlag::DELSFRMS)) {
		if (code == 'S' || code == VK_RETURN || code == 0xc0) {
		  thred::savdo();
		  if (code == 'S') {
			StateMap.set(StateFlag::DELTO);
		  }
		  else {
			StateMap.reset(StateFlag::DELTO);
		  }
		  delsfrms(code);
		  thred::coltab();
		  StateMap.set(StateFlag::RESTCH);
		  thred::unmsg();
		  return true;
		}
	  }
	  if (StateMap.testAndReset(StateFlag::DELFRM)) {
		if (code == 'S' || code == VK_RETURN || code == 0xc0) {
		  thred::savdo();
		  if (code == 'S') {
			StateMap.set(StateFlag::DELTO);
		  }
		  else {
			StateMap.reset(StateFlag::DELTO);
		  }
		  thred::frmdel();
		  fndknt();
		  thred::coltab();
		  StateMap.set(StateFlag::RESTCH);
		  thred::unmsg();
		  return true;
		}
	  }
	  if (StateMap.test(StateFlag::THUMSHO)) {
		switch (code) {
		  case VK_ESCAPE:
		  case 'Q': {
			unthum();
			StateMap.reset(StateFlag::BAKSHO);
			esccode();
			qcode();
			break;
		  }
		  case VK_DOWN:
		  case 0x22: { // page down
			nuthsel();
			break;
		  }
		  case VK_UP:
		  case 0x21: { // page up
			nuthbak(8U);
			break;
		  }
		  case VK_HOME: {
			ThumbnailIndex = 0;
			nuthsel();
			break;
		  }
		  case VK_END: {
			ThumbnailIndex = wrap::toUnsigned(Thumbnails->size());
			nuthbak(4);
			break;
		  }
		  case 8: // backspace
		  case VK_LEFT: {
			bakthum();
			break;
		  }
		  default: {
			TranslateMessage(&Msg);
		  }
		}
		return true;
	  }
	  if (StateMap.test(StateFlag::FSETFSPAC) || StateMap.test(StateFlag::GTWLKIND)) {
		// Check for keycode 'dash' and numpad 'subtract'
		if (code == 189 || code == 109) {
		  MsgBuffer[0] = '-';
		  MsgIndex     = 1;
		  MsgBuffer[1] = 0;
		  SetWindowText(GeneralNumberInputBox, std::begin(MsgBuffer));
		  return true;
		}
	  }
	  if ((FormMenuChoice != 0U) || (PreferenceIndex != 0U)) {
		if (chkminus(code)) {
		  MsgIndex                 = 1;
		  SideWindowEntryBuffer[0] = '-';
		  SetWindowText(SideMessageWindow, std::begin(SideWindowEntryBuffer));
		  return true;
		}
		if (dunum(code)) {
		  if (PreferenceIndex == PSHO + 1 || PreferenceIndex == PBOX + 1U) {
			MsgBuffer[0] = gsl::narrow<char>(NumericCode);
			MsgBuffer[1] = 0;
			if (PreferenceIndex == PSHO + 1U) {
			  ShowStitchThreshold = unthrsh(NumericCode - 0x30);
			  SetWindowText(ValueWindow->operator[](PSHO), std::begin(MsgBuffer));
			}
			else {
			  StitchBoxesThreshold = unthrsh(NumericCode - 0x30);
			  SetWindowText(ValueWindow->operator[](PBOX), std::begin(MsgBuffer));
			}
			thred::unsid();
		  }
		  else {
			if (MsgIndex < ((sizeof(SideWindowEntryBuffer) - 1) / sizeof(SideWindowEntryBuffer[0]))) {
			  SideWindowEntryBuffer[MsgIndex++] = NumericCode;
			  SideWindowEntryBuffer[MsgIndex]   = 0;
			  SetWindowText(SideMessageWindow, std::begin(SideWindowEntryBuffer));
			}
		  }
		  return true;
		}
		switch (code) {
		  case 0x6e:   // numpad period
		  case 0xbe: { // period
			SideWindowEntryBuffer[MsgIndex++] = '.';
			SideWindowEntryBuffer[MsgIndex]   = 0;
			SetWindowText(SideMessageWindow, std::begin(SideWindowEntryBuffer));
			return true;
		  }
		  case 8: { // backspace
			if (MsgIndex != 0U) {
			  SideWindowEntryBuffer[--MsgIndex] = 0;
			  SetWindowText(SideMessageWindow, std::begin(SideWindowEntryBuffer));
			}
			return true;
		  }
		  case VK_RETURN: {
			chknum();
			FormMenuChoice  = 0;
			PreferenceIndex = 0;
			return true;
		  }
		  default: {
		  }
		}
	  }
	  if (code == 'I') {
		thi::movi();
		LastKeyCode = 'I';
		return true;
	  }
	  if (code == 'Q' && LastKeyCode == 'Q') {
		unpat();
	  }
	  LastKeyCode = code;
	  if (StateMap.test(StateFlag::NUMIN)) {
		{
		  auto       retflag = true;
		  auto const retval  = handleNumericInput(code, retflag);
		  if (retflag) {
			return retval;
		  }
		}
	  }
	  if (StateMap.testAndReset(StateFlag::ENTRDUP)) {
		if (std::wcslen(std::begin(MsgBuffer)) != 0) {
		  auto const value = wrap::bufToFloat(std::begin(MsgBuffer));
		  if (value != 0.0F) {
			IniFile.rotationAngle = value * PI_F / 180.0F;
		  }
		}
		form::duprot(IniFile.rotationAngle);
	  }
	  if (StateMap.testAndReset(StateFlag::ENTROT)) {
		if (std::wcslen(std::begin(MsgBuffer)) != 0) {
		  auto const value = wrap::bufToFloat(std::begin(MsgBuffer));
		  if (value != 0.0) {
			IniFile.rotationAngle = value * PI_F / 180.0F;
		  }
		}
		rotfns(IniFile.rotationAngle);
	  }
	  thred::unmsg();
	  {
		auto       retflag = true;
		auto const retval  = handleMainWinKeys(code, rotationCenter, stretchBoxLine, retflag);
		if (retflag) {
		  return retval;
		}
	  }
	  break;
	}
	case WM_COMMAND: {
	  thred::unmsg();
	  {
		auto const& previousNames = *PreviousNames;
		auto        iVersion      = 0U;
		for (auto const iLRU : LRUMenuId) {
		  if (Msg.wParam == iLRU) {
			*WorkingFileName = previousNames[iVersion];
			StateMap.set(StateFlag::REDOLD);
			nuFil();
		  }
		  iVersion++;
		}
	  }
	  auto const wParameter = LOWORD(Msg.wParam); // NOLINT
	  if (wParameter > 40000U && wParameter < 40300U) {
		thred::undat();
	  }
	  if (thi::handleMainMenu(wParameter, rotationCenter)) {
		break;
	  }
	  if (thi::handleEditMenu(wParameter)) {
		break;
	  }
	  if (thi::handleFileMenu(wParameter)) {
		break;
	  }
	  if (thi::handleFillMenu(wParameter)) {
		break;
	  }
	  if (thi::handleViewMenu(wParameter)) {
		break;
	  }
	  break;
	}
  }
  return false;
}

// return the width of a text item
auto thred::txtWid(wchar_t const* string) noexcept -> int32_t {
  wrap::GetTextExtentPoint32(ThredDC, string, gsl::narrow<uint32_t>(wcslen(string)), &TextSize);
  return TextSize.cx;
}

void thred::internal::makCol() noexcept {
  wchar_t buffer[3] = {0};

  buffer[1] = L'0';
  buffer[2] = 0;

  auto hFont = displayText::getThrEdFont(400);

  for (auto iColor = 0U; iColor < 16U; iColor++) {
	DefaultColorWin->operator[](iColor) = CreateWindow(L"STATIC", // NOLINT
	                                                   nullptr,
	                                                   SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                                   0,
	                                                   ButtonHeight * iColor,
	                                                   ButtonWidth,
	                                                   ButtonHeight,
	                                                   ThrEdWindow,
	                                                   nullptr,
	                                                   ThrEdInstance,
	                                                   nullptr);
	displayText::setWindowFont(DefaultColorWin->operator[](iColor), hFont);
	UserColorWin->operator[](iColor) = CreateWindow(L"STATIC", // NOLINT
	                                                nullptr,
	                                                SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                                ButtonWidth,
	                                                ButtonHeight * iColor,
	                                                ButtonWidth,
	                                                ButtonHeight,
	                                                ThrEdWindow,
	                                                nullptr,
	                                                ThrEdInstance,
	                                                nullptr);
	buffer[0]                        = ThreadSize[iColor][0];
	ThreadSizeWin[iColor]            = CreateWindow(L"STATIC", // NOLINT
                                         buffer,
                                         SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                         ButtonWidth * 2U,
                                         ButtonHeight * iColor,
                                         ButtonWidth,
                                         ButtonHeight,
                                         ThrEdWindow,
                                         nullptr,
                                         ThrEdInstance,
                                         nullptr);
	displayText::setWindowFont(ThreadSizeWin[iColor], hFont);
  }
}

void thred::internal::ritloc() {
  auto       lockFilePath = fs::path {};
  auto       ppszPath = gsl::narrow_cast<PWSTR>(nullptr); // variable to receive the path memory block pointer.
  auto const hr = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, nullptr, &ppszPath);

  if (SUCCEEDED(hr)) {
	lockFilePath.assign(ppszPath); // make a local copy of the path
	lockFilePath /= L"ThrEd";
	fs::create_directory(lockFilePath);
	lockFilePath /= L"thredloc.txt";
	auto lockFile = CreateFile(lockFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (lockFile != INVALID_HANDLE_VALUE) { // NOLINT
	  auto bytesWritten = DWORD {0};
	  auto value        = utf::Utf16ToUtf8(*HomeDirectory);
	  wrap::WriteFile(lockFile, value.data(), wrap::toUnsigned(value.size()) + 1U, &bytesWritten, nullptr);
	  CloseHandle(lockFile);
	}
  }
  CoTaskMemFree(ppszPath); // free up the path memory block
}

void thred::internal::crtcurs() noexcept {
  FormCursor            = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Form));      // NOLINT
  DLineCursor           = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_DLIN));      // NOLINT
  NeedleUpCursor        = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Upright));   // NOLINT
  NeedleRightDownCursor = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightDown)); // NOLINT
  NeedleRightUpCursor   = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightUp));   // NOLINT
  NeedleLeftDownCursor  = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftDown));  // NOLINT
  NeedleLeftUpCursor    = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftUp));    // NOLINT
}

void thred::internal::duhom() {
  auto arg0 = fs::path {ArgList[0]};

  *HomeDirectory = arg0.parent_path();
}

void thred::internal::ducmd() {
  if (ArgCount > 1) {
	auto arg1 = std::wstring {ArgList[1]};
	if (arg1.compare(0, 4, L"/F1:") == 0) {
	  auto balaradFileName = *HomeDirectory / arg1.substr(4);
	  BalaradFile =
	      CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	  if (BalaradFile != INVALID_HANDLE_VALUE) { // NOLINT
		CloseHandle(BalaradFile);
		*BalaradName0 = balaradFileName;
		if (ArgCount > 2) {
		  auto arg2 = std::wstring {ArgList[2]};
		  if (arg2.compare(0, 4, L"/F2:") == 0) {
			balaradFileName = *HomeDirectory / arg2.substr(4);
			BalaradFile     = CreateFile(
                balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
			if (BalaradFile != INVALID_HANDLE_VALUE) { // NOLINT
			  wchar_t readBuffer[_MAX_PATH + 1] = {0};

			  *BalaradName1  = balaradFileName;
			  auto bytesRead = DWORD {0};
			  ReadFile(BalaradFile, &readBuffer, (_MAX_PATH + 1), &bytesRead, nullptr);
			  if (bytesRead != 0U) {
				BalaradName2->assign(static_cast<wchar_t const*>(readBuffer));
				redbal();
			  }
			  CloseHandle(BalaradFile);
			}
		  }
		}
		SetWindowText(ThrEdWindow, StringTable->operator[](STR_EMB).c_str());
	  }
	  fs::remove(*BalaradName1);
	}
	else {
	  WorkingFileName->assign(arg1);
	  StateMap.set(StateFlag::REDOLD);
	  nuFil();
	}
  }
}

void thred::internal::setPrefs() {
  defpref();
  getDocsFolder(DefaultDirectory);
  if (DesignerName->empty()) {
	wchar_t designerBuffer[50];
	LoadString(ThrEdInstance,
	           IDS_UNAM,
	           static_cast<LPTSTR>(designerBuffer),
	           sizeof(designerBuffer) / sizeof(designerBuffer[0]));
	DesignerName->assign(static_cast<wchar_t const*>(designerBuffer));
	getdes();
  }
}

void thred::internal::redini() {
  duhom();
  *IniFileName = *HomeDirectory;
  *IniFileName /= L"thred.ini";
  IniFileHandle =
      CreateFile(IniFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (IniFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	setPrefs();
  }
  else {
	auto bytesRead = DWORD {0};
	ReadFile(IniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, nullptr);
	CloseHandle(IniFileHandle);
	if (bytesRead < sizeof(IniFile)) {
	  auto newFileName = *IniFileName;
	  newFileName.replace_filename("thred-ini.bak");
	  fs::rename(*IniFileName, newFileName);
	  setPrefs();
	}
	else {
	  auto directory = utf::Utf8ToUtf16(std::string(static_cast<char const*>(IniFile.defaultDirectory)));
	  DefaultDirectory->assign(directory);
	  DefaultBMPDirectory->assign(directory);
	  {
		auto& previousNames = *PreviousNames;
		auto  iVersion      = 0U;
		for (auto const& prevName : IniFile.prevNames) {
		  if (strlen(std::begin(prevName)) != 0U) {
			previousNames[iVersion].assign(utf::Utf8ToUtf16(std::string(std::begin(prevName))));
		  }
		  else {
			previousNames[iVersion].clear();
		  }
		  iVersion++;
		}
	  }
	  DesignerName->assign(utf::Utf8ToUtf16(std::string(static_cast<char const*>(IniFile.designerName))));
	  for (auto iColor = 0U; iColor < 16U; iColor++) {
		UserColor[iColor]              = IniFile.stitchColors[iColor];
		CustomColor[iColor]            = IniFile.stitchPreferredColors[iColor];
		CustomBackgroundColor[iColor]  = IniFile.backgroundPreferredColors[iColor];
		BitmapBackgroundColors[iColor] = IniFile.bitmapBackgroundColors[iColor];
	  }
	  BackgroundColor = IniFile.backgroundColor;
	  BitmapColor     = IniFile.bitmapColor;
	  MinStitchLength = IniFile.minStitchLength;
	  if (IniFile.showStitchThreshold < 0) {
		IniFile.showStitchThreshold = 0;
	  }
	  if (IniFile.showStitchThreshold > 9.0) {
		IniFile.showStitchThreshold = 9.0;
	  }
	  ShowStitchThreshold = IniFile.showStitchThreshold;
	  if (IniFile.threadSize30 != 0.0) {
		ThreadSize30 = IniFile.threadSize30;
	  }
	  if (IniFile.threadSize40 != 0.0) {
		ThreadSize40 = IniFile.threadSize40;
	  }
	  if (IniFile.threadSize60 != 0.0) {
		ThreadSize60 = IniFile.threadSize60;
	  }
	  if (IniFile.userStitchLength != 0.0F) {
		UserStitchLength = IniFile.userStitchLength;
	  }
	  if (IniFile.maxStitchLength == 0.0) {
		IniFile.maxStitchLength = MAXSIZ * PFGRAN;
	  }
	  if (IniFile.smallStitchLength != 0.0F) {
		SmallStitchLength = IniFile.smallStitchLength;
	  }
	  StitchBoxesThreshold = IniFile.stitchBoxesThreshold;
	  if (IniFile.stitchSpace != 0.0F) {
		LineSpacing = IniFile.stitchSpace;
	  }
	  {
		auto const tmp = EnumMap<UserFlag>(IniFile.userFlagMap);
		UserFlagMap    = tmp;
	  }
	  if (IniFile.borderWidth != 0.0F) {
		BorderWidth = IniFile.borderWidth;
	  }
	  if (IniFile.appliqueColor != 0U) {
		AppliqueColor = IniFile.appliqueColor & 0xfU;
	  }
	  if (IniFile.snapLength != 0.0F) {
		SnapLength = IniFile.snapLength;
	  }
	  if (IniFile.starRatio != 0.0F) {
		StarRatio = IniFile.starRatio;
	  }
	  if (IniFile.spiralWrap != 0.0F) {
		SpiralWrap = IniFile.spiralWrap;
	  }
	  if (IniFile.buttonholeCornerLength != 0.0F) {
		ButtonholeCornerLength = IniFile.buttonholeCornerLength;
	  }
	  if (IniFile.gridSize == 0.0F) {
		IniFile.gridSize = 12.0F;
	  }
	  if (IniFile.wavePoints == 0U) {
		IniFile.wavePoints = IWAVPNTS;
	  }
	  if (IniFile.waveStart == 0U) {
		IniFile.waveStart = IWAVSTRT;
	  }
	  if (IniFile.waveEnd == 0U) {
		IniFile.waveEnd = IWAVEND;
	  }
	  if (IniFile.waveLobes == 0U) {
		IniFile.waveLobes = IWAVS;
	  }
	  if (IniFile.featherFillType == 0U) {
		IniFile.featherFillType = FDEFTYP;
	  }
	  if (IniFile.featherUpCount == 0U) {
		IniFile.featherUpCount = FDEFUP;
	  }
	  if (IniFile.featherDownCount == 0U) {
		IniFile.featherDownCount = FDEFDWN;
	  }
	  if (IniFile.featherRatio == 0.0F) {
		IniFile.featherRatio = FDEFRAT;
	  }
	  if (IniFile.featherMinStitchSize == 0.0F) {
		IniFile.featherMinStitchSize = FDEFFLR;
	  }
	  if (IniFile.featherCount == 0U) {
		IniFile.featherCount = FDEFNUM;
	  }
	  if (IniFile.daisyHoleDiameter == 0.0F) {
		IniFile.daisyHoleDiameter = DAZHLEN;
	  }
	  if (IniFile.daisyPetalPoints == 0U) {
		IniFile.daisyPetalPoints = DAZCNT;
	  }
	  if (IniFile.daisyInnerCount == 0U) {
		IniFile.daisyInnerCount = DAZICNT;
	  }
	  if (IniFile.daisyDiameter == 0.0F) {
		IniFile.daisyDiameter = DAZLEN;
	  }
	  if (IniFile.daisyPetalCount == 0U) {
		IniFile.daisyPetalCount = DAZPETS;
	  }
	  if (IniFile.daisyPetalLen == 0.0F) {
		IniFile.daisyPetalLen = DAZPLEN;
	  }

	  switch (IniFile.hoopType) {
		case SMALHUP: {
		  IniFile.hoopSizeX = SHUPX;
		  IniFile.hoopSizeY = SHUPY;
		  break;
		}
		case LARGHUP: {
		  IniFile.hoopSizeX = LHUPX;
		  IniFile.hoopSizeY = LHUPY;
		  break;
		}
		case CUSTHUP: {
		  if (IniFile.hoopSizeX == 0.0F) {
			IniFile.hoopSizeX = LHUPX;
		  }
		  if (IniFile.hoopSizeY == 0.0F) {
			IniFile.hoopSizeY = LHUPY;
		  }
		  break;
		}
		case HUP100: {
		  if (IniFile.hoopSizeX == 0.0F) {
			IniFile.hoopSizeX = HUP100XY;
		  }
		  if (IniFile.hoopSizeY == 0.0F) {
			IniFile.hoopSizeY = HUP100XY;
		  }
		  break;
		}
		default: {
		  IniFile.hoopType  = LARGHUP;
		  IniFile.hoopSizeX = LHUPX;
		  IniFile.hoopSizeY = LHUPY;
		}
	  }
	  UnzoomedRect = {wrap::round<int32_t>(IniFile.hoopSizeX), wrap::round<int32_t>(IniFile.hoopSizeY)};
	  PicotSpacing = IniFile.picotSpace;
	}
  }
  if (IniFile.gridColor == 0U) {
	IniFile.gridColor = DEFGRD;
  }
  if (IniFile.fillAngle == 0.0) {
	IniFile.fillAngle = PI_F / 6.0F;
  }
  auto deviceContext = GetDC(nullptr);
  ScreenSizePixels = {GetDeviceCaps(deviceContext, HORZRES), GetDeviceCaps(deviceContext, VERTRES)};
  if (IniFile.initialWindowCoords.left < 0) {
	IniFile.initialWindowCoords.left = 0;
  }
  if (IniFile.initialWindowCoords.top < 0) {
	IniFile.initialWindowCoords.top = 0;
  }
  auto const adjustedWidth = ScreenSizePixels.cx - 30;
  if (IniFile.initialWindowCoords.right > adjustedWidth) {
	IniFile.initialWindowCoords.right = adjustedWidth;
  }
  if (IniFile.initialWindowCoords.bottom > ScreenSizePixels.cy) {
	IniFile.initialWindowCoords.bottom = ScreenSizePixels.cy;
  }
}

void thred::internal::chkirct() noexcept {
  auto const screenLimits = POINT {ScreenSizePixels.cx - 1, ScreenSizePixels.cy - 1};
  if (IniFile.initialWindowCoords.left > screenLimits.x) {
	IniFile.initialWindowCoords.left = screenLimits.x;
  }
  if (IniFile.initialWindowCoords.right > screenLimits.x) {
	IniFile.initialWindowCoords.right = screenLimits.x;
  }
  if (IniFile.initialWindowCoords.left < 0) {
	IniFile.initialWindowCoords.left = 0;
  }
  if (IniFile.initialWindowCoords.right < 0) {
	IniFile.initialWindowCoords.right = 0;
  }
  if (IniFile.initialWindowCoords.top > screenLimits.y) {
	IniFile.initialWindowCoords.top = screenLimits.y;
  }
  if (IniFile.initialWindowCoords.bottom > screenLimits.y) {
	IniFile.initialWindowCoords.bottom = screenLimits.y;
  }
  if (IniFile.initialWindowCoords.top < 0) {
	IniFile.initialWindowCoords.top = 0;
  }
  if (IniFile.initialWindowCoords.bottom < 0) {
	IniFile.initialWindowCoords.bottom = 0;
  }
  if (IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left < 300) {
	IniFile.initialWindowCoords.left  = 0;
	IniFile.initialWindowCoords.right = screenLimits.x / 2;
  }
  if (IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top < 300) {
	IniFile.initialWindowCoords.top    = 0;
	IniFile.initialWindowCoords.bottom = screenLimits.y / 2;
  }
}

void thred::internal::init() {
  auto       deviceContext   = GetDC(nullptr);
  auto const screenHalfWidth = (GetDeviceCaps(deviceContext, HORZRES)) / 2;
  auto       blank           = std::wstring {};

  ReleaseDC(nullptr, deviceContext);
  TexturePointsBuffer->clear();
  LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1)); // NOLINT
  MainMenu   = GetMenu(ThrEdWindow);
  auto wRect = RECT {0L, 0L, 0L, 0L};
  GetWindowRect(ThrEdWindow, &wRect);
  auto mRect = RECT {0L, 0L, 0L, 0L};
  GetMenuItemRect(ThrEdWindow, MainMenu, 0, &mRect);
  wRect.left  = mRect.left;
  wRect.right = mRect.right;
  for (auto iMenu = 0U; iMenu <= M_HELP; iMenu++) {
	GetMenuItemRect(ThrEdWindow, MainMenu, iMenu, &mRect);
	wRect.right += (mRect.right - mRect.left);
  }
  wRect.right += 20;
  auto const sWidth = wRect.right - wRect.left;
  if (sWidth < screenHalfWidth) {
	wRect.right = wRect.left += screenHalfWidth;
  }
  FillMenu       = GetSubMenu(MainMenu, M_FILL);
  FileMenu       = GetSubMenu(MainMenu, M_FILE);
  EditMenu       = GetSubMenu(MainMenu, M_EDIT);
  BorderFillMenu = GetSubMenu(FillMenu, MFIL_BORD);
  ViewMenu       = GetSubMenu(MainMenu, M_VIEW);
  ViewSetMenu    = GetSubMenu(ViewMenu, MVW_SET);
  qchk();
  ArrowCursor = LoadCursor(nullptr, IDC_ARROW); // NOLINT
  CrossCursor = LoadCursor(nullptr, IDC_CROSS); // NOLINT
  crtcurs();
  redfils();
  StateMap.reset(); // clear the bitmap
  // set up the size variables
  ThredDC = GetDC(ThrEdWindow);
  SetStretchBltMode(ThredDC, COLORONCOLOR);
  StitchWindowDC    = GetDCEx(MainStitchWin, nullptr, DCX_PARENTCLIP | DCX_CLIPSIBLINGS); // NOLINT
  StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
  ScreenSizeMM.cx   = GetDeviceCaps(ThredDC, HORZSIZE);
  ScreenSizeMM.cy   = GetDeviceCaps(ThredDC, VERTSIZE);
  chkirct();
  if (!UserFlagMap.test(UserFlag::SAVMAX)) {
	MoveWindow(ThrEdWindow,
	           IniFile.initialWindowCoords.left,
	           IniFile.initialWindowCoords.top,
	           IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left,
	           IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top,
	           0);
  }
  auto hFont = displayText::getThrEdFont(400);
  SelectObject(ThredDC, hFont);
  SelectObject(StitchWindowDC, hFont);
  ButtonWidth               = thred::txtWid(L"MM") + TXTSIDS;
  ButtonWidthX3             = ButtonWidth * 3;
  ButtonHeight              = TextSize.cy + 4;
  auto const offsetStepSize = thred::txtWid(L"0");
  for (auto iOffset = 0U; iOffset < NERCNT; iOffset++) {
	BoxOffset[iOffset] = offsetStepSize * (iOffset + 1);
  }
  GetClientRect(ThrEdWindow, &ThredWindowRect);
  stchWnd();
  displayText::lodstr();
  if (IniFileHandle == nullptr) {
	// initialize the user color and thread size arrays
	for (auto iColor = 0U; iColor < 16U; iColor++) {
	  UserColor[iColor] = DefaultColors[iColor];
	}
  }
  for (auto& iSize : ThreadSize) {
	iSize[0] = '4';
	iSize[1] = '0';
  }
  if (IniFile.traceLength == 0.0F) {
	IniFile.traceLength = TRACLEN;
  }
  if (IniFile.traceRatio == 0.0F) {
	IniFile.traceRatio = TRACRAT;
  }
  if (IniFile.chainSpace == 0.0F) {
	IniFile.chainSpace = CHSDEF;
  }
  if (IniFile.chainRatio == 0.0F) {
	IniFile.chainRatio = CHRDEF;
  }
  if (IniFile.lensRatio == 0.0F) {
	IniFile.lensRatio = DEFLRAT;
  }
  if (IniFile.cursorNudgeStep == 0.0F) {
	IniFile.cursorNudgeStep = NUGINI;
  }
  if (IniFile.nudgePixels == 0U) {
	IniFile.nudgePixels = DEFPIX;
  }
  if (IniFile.eggRatio == 0.0F) {
	IniFile.eggRatio = DEFEGRAT;
  }
  if (IniFile.stitchSizePixels == 0U) {
	IniFile.stitchSizePixels = DEFPNTPIX;
  }
  if (IniFile.formVertexSizePixels == 0U) {
	IniFile.formVertexSizePixels = DEFPNTPIX;
  }
  if (IniFile.formSides == 0U) {
	IniFile.formSides = 24U;
  }
  if (IniFile.tearTailLength == 0.0F) {
	IniFile.tearTailLength = 1.4F;
  }
  if (IniFile.underlayStitchLen == 0.0F) {
	IniFile.underlayStitchLen = DEFULEN;
  }
  if (IniFile.underlaySpacing == 0.0F) {
	IniFile.underlaySpacing = DEFUSPAC;
  }
  setgrd(IniFile.gridColor);
  makCol(); // make the color change windows
  ButtonWin->resize(9U);
  for (auto iButton = 0U; iButton < 9U; iButton++) {
	DWORD windowFlags = 0U;
	auto  buttonTxt   = &blank;
	switch (iButton) {
	  case HBOXSEL: {
		windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER; // NOLINT
		buttonTxt   = &StringTable->operator[](STR_BOXSEL);
		break;
	  }
	  case HUPTO: {
		windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER; // NOLINT
		buttonTxt   = &StringTable->operator[](STR_UPOF);
		break;
	  }
	  case HHID: {
		windowFlags = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER; // NOLINT
		buttonTxt   = &StringTable->operator[](STR_PIKOL);
		break;
	  }
	  default: {
		windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER; // NOLINT
	  }
	}
	if (buttonTxt != nullptr) {
	  ButtonWin->operator[](iButton) = CreateWindow(L"STATIC",
	                                                buttonTxt->c_str(),
	                                                windowFlags,
	                                                0,
	                                                ButtonHeight * (16 + iButton),
	                                                ButtonWidthX3,
	                                                ButtonHeight,
	                                                ThrEdWindow,
	                                                nullptr,
	                                                ThrEdInstance,
	                                                nullptr);
	}
  }
  trace::initTraceWindows();
  ColorBar = CreateWindow(L"STATIC", // NOLINT
                          L"",
                          SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
                          ThredWindowRect.right - *ColorBarSize,
                          0,
                          *ColorBarSize,
                          ThredWindowRect.bottom,
                          ThrEdWindow,
                          nullptr,
                          ThrEdInstance,
                          nullptr);
  nuRct();
  // create pens

  for (auto iRGBK = 0U; iRGBK < 4; iRGBK++) {
	BoxPen[iRGBK] = wrap::CreatePen(PS_SOLID, 1, BoxColor[iRGBK]);
  }
  LinePen            = wrap::CreatePen(PS_SOLID, 1U, 0x404040U);
  CrossPen           = wrap::CreatePen(PS_SOLID, 5U, 0x804080U);
  GroupSelectPen     = wrap::CreatePen(PS_SOLID, 1U, 0x804080U);
  GridPen            = wrap::CreatePen(PS_SOLID, 1U, IniFile.gridColor);
  BackgroundPen      = wrap::CreatePen(PS_SOLID, 3U, BackgroundColor);
  BitmapPen          = wrap::CreatePen(PS_SOLID, 1U, BitmapColor);
  FormPen            = wrap::CreatePen(PS_SOLID, 1U, 0xc0c0c0U);
  MultiFormPen       = wrap::CreatePen(PS_SOLID, 1U, 0xc0c080U);
  FormPen3px         = wrap::CreatePen(PS_SOLID, 3U, 0xc0c0c0U);
  FormSelectedPen    = wrap::CreatePen(PS_SOLID, 1U, 0x80c080U);
  ZoomMarkPen        = wrap::CreatePen(PS_SOLID, 3U, 0x40c040U);
  SelectAllPen       = wrap::CreatePen(PS_SOLID, 1U, 0xc08080U);
  KnotPen            = wrap::CreatePen(PS_SOLID, 1U, 0xffffffU);
  LayerPen[0]        = wrap::CreatePen(PS_SOLID, 1U, 0xc0c0c0U);
  LayerPen[1]        = wrap::CreatePen(PS_SOLID, 1U, 0x80c0c0U);
  LayerPen[2]        = wrap::CreatePen(PS_SOLID, 1U, 0xc080c0U);
  LayerPen[3]        = wrap::CreatePen(PS_SOLID, 1U, 0xc0c080U);
  LayerPen[4]        = wrap::CreatePen(PS_SOLID, 1U, 0x40c0c0U);
  LayerPen[5]        = wrap::CreatePen(PS_SOLID, 1U, 0xc0c040U);
  BackgroundPenWidth = 1;
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	ThreadSizePixels[iColor] = 1;
	ThreadSizeIndex[iColor]  = 1;
	UserPen[iColor]          = wrap::CreatePen(PS_SOLID, 1, UserColor[iColor]);
  }
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  // create brushes
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	DefaultColorBrush[iColor] = CreateSolidBrush(DefaultColors[iColor]);
	UserColorBrush[iColor]    = CreateSolidBrush(UserColor[iColor]);
  }
  ZoomFactor           = 1;
  PCSHeader.leadIn     = 0x32;
  PCSHeader.colorCount = 16U;
  StitchBuffer->clear();
  GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
  ladj();
  wrap::GetTextExtentPoint32(ThredDC,
                             StringTable->operator[](STR_PIKOL).c_str(),
                             wrap::toUnsigned(StringTable->operator[](STR_PIKOL).size()),
                             &PickColorMsgSize);
  auxmen();
  fnamtabs();
  ritfnam(*DesignerName);
  auto       designer     = utf::Utf16ToUtf8(*DesignerName);
  auto const modifierName = gsl::span<char> {ExtendedHeader->modifierName};
  std::copy(designer.begin(), designer.end(), modifierName.begin());
  chkhup();
  nedmen();
  fil2men();
  knotmen();
  bsavmen();
  rotauxmen();
  linbmen();
  thred::wrnmen();
  ritloc();
  frmcurmen();
  texture::redtx();
  StateMap.set(StateFlag::HIDMAP);
  thred::hidbit();
  thred::chkmen();
  // check command line-should be last item in init
  ducmd();
  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
}

auto thred::internal::defTxt(uint32_t iColor) -> COLORREF {
  // bitmap for color number. Black or white bit chosen for contrast against the default background colors
  auto const     textColorMap = std::bitset<16>(0xbaf);
  constexpr auto white        = 0xffffffU;
  constexpr auto black        = 0U;
  return textColorMap.test(iColor) ? white : black;
}

void thred::internal::relin() {
  stch2px1(ClosestPointIndex - 1);
  MoveLine0[0] = StitchCoordinatesPixels;
  stch2px1(ClosestPointIndex + 1U);
  MoveLine1[1] = StitchCoordinatesPixels;
  StateMap.reset(StateFlag::WASLIN);
  dulin();
}

void thred::internal::dumov() {
  auto const anchorStitch = StitchBuffer->operator[](MoveAnchor);

  RotateAngle = atan2(StitchBuffer->    operator[](wrap::toSize(MoveAnchor) + 1U).y -
                          StitchBuffer->operator[](MoveAnchor).y,
                      StitchBuffer->    operator[](wrap::toSize(MoveAnchor) + 1U).x -
                          StitchBuffer->operator[](MoveAnchor).x);
  if (anchorStitch.x >= ZoomRect.left && anchorStitch.x <= ZoomRect.right &&
      anchorStitch.y >= ZoomRect.bottom && anchorStitch.y <= ZoomRect.top) {
	POINT      rotationOutline[8]   = {};
	auto const rotationCenterPixels = sdCor2px(StitchBuffer->operator[](MoveAnchor));
	rotationOutline[0]              = rotationCenterPixels;
	rotationOutline[6]              = rotationCenterPixels;
	auto OffsetFromCenter = POINT {rotationCenterPixels.x + 12, rotationCenterPixels.y + 2};
	rotpix(OffsetFromCenter, rotationOutline[1], rotationCenterPixels);
	OffsetFromCenter.y = rotationCenterPixels.y - 2;
	rotpix(OffsetFromCenter, rotationOutline[5], rotationCenterPixels);
	OffsetFromCenter.y = rotationCenterPixels.y + 6;
	rotpix(OffsetFromCenter, rotationOutline[2], rotationCenterPixels);
	OffsetFromCenter.y = rotationCenterPixels.y - 6;
	rotpix(OffsetFromCenter, rotationOutline[4], rotationCenterPixels);
	OffsetFromCenter.x = rotationCenterPixels.x + 20;
	OffsetFromCenter.y = rotationCenterPixels.y;
	rotpix(OffsetFromCenter, rotationOutline[3], rotationCenterPixels);
	SelectObject(StitchWindowMemDC, FormPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	Polyline(StitchWindowMemDC, static_cast<POINT const*>(rotationOutline), 7);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
}

auto thred::internal::chkup(uint32_t count, uint32_t iStitch) -> uint32_t {
  if (StateMap.test(StateFlag::UPTO) && (ClosestPointIndex != 0U)) {
	if (ColorChangeTable[iStitch].stitchIndex < ClosestPointIndex) {
	  if (ColorChangeTable[wrap::toSize(iStitch) + 1U].stitchIndex < ClosestPointIndex) {
		return count;
	  }
	  return ClosestPointIndex - ColorChangeTable[iStitch].stitchIndex + 1U;
	}

	return 0;
  }

  return count;
}

auto thred::internal::bitar() -> bool {
  auto const zoomedInRect = fRECTANGLE {
      ZoomRect.left, (UnzoomedRect.y - ZoomRect.top), ZoomRect.right, (UnzoomedRect.y - ZoomRect.bottom)};

  if (zoomedInRect.top > BitmapSizeinStitches.y || zoomedInRect.left > BitmapSizeinStitches.x) {
	return false;
  }
  BitmapSrcRect = {wrap::ceil<int32_t>(ZoomRect.left * BmpStitchRatio.x),
                   wrap::ceil<int32_t>(zoomedInRect.top * BmpStitchRatio.y),
                   wrap::floor<int32_t>(ZoomRect.right * BmpStitchRatio.x),
                   wrap::floor<int32_t>(zoomedInRect.bottom * BmpStitchRatio.y)};
  if (BitmapSrcRect.right > gsl::narrow<int32_t>(BitmapWidth)) {
	BitmapSrcRect.right = BitmapWidth;
	StateMap.reset(StateFlag::LANDSCAP);
  }
  if (BitmapSrcRect.bottom > gsl::narrow<int32_t>(BitmapHeight)) {
	BitmapSrcRect.bottom = BitmapHeight;
	StateMap.set(StateFlag::LANDSCAP);
  }
  auto const backingRect = fRECTANGLE {BitmapSrcRect.left * StitchBmpRatio.x,
                                       BitmapSrcRect.top * StitchBmpRatio.y,
                                       BitmapSrcRect.right * StitchBmpRatio.x,
                                       BitmapSrcRect.bottom * StitchBmpRatio.y};

  auto const differenceRect = fRECTANGLE {backingRect.left - zoomedInRect.left,
                                          backingRect.top - zoomedInRect.top,
                                          zoomedInRect.right - backingRect.right,
                                          zoomedInRect.bottom - backingRect.bottom};

  auto const bitmapStitchRatio =
      fPOINT {gsl::narrow_cast<float>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left),
              gsl::narrow_cast<float>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom)};

  BitmapDstRect = {
      wrap::round<int32_t>(differenceRect.left * bitmapStitchRatio.x),
      wrap::round<int32_t>(differenceRect.top * bitmapStitchRatio.y),
      wrap::round<int32_t>(StitchWindowClientRect.right - differenceRect.right * bitmapStitchRatio.x),
      wrap::round<int32_t>(StitchWindowClientRect.bottom - differenceRect.bottom * bitmapStitchRatio.y)};
  return true;
}

void thred::internal::stCor2px(fPOINTATTR const& stitch, POINT& point) {
  point = {
      wrap::ceil<int32_t>((stitch.x - ZoomRect.left) * ZoomRatio.x),
      wrap::ceil<int32_t>(StitchWindowClientRect.bottom - (stitch.y - ZoomRect.bottom) * ZoomRatio.y)};
}

void thred::internal::drwknot() {
  constexpr auto KnotBoxSize  = 5;  // offset of the knot box sides;
  constexpr auto KnotLineSize = 10; // length of the knot line;

  if (!UserFlagMap.test(UserFlag::KNOTOF) && (KnotCount != 0U) && (!StitchBuffer->empty())) {
	auto const kOffset = gsl::narrow<int32_t>(MulDiv(KnotBoxSize, *screenDPI, 96));
	auto const kLine   = gsl::narrow<int32_t>(MulDiv(KnotLineSize, *screenDPI, 96));

	auto  point   = POINT {0L, 0L};
	POINT tlin[5] = {};

	for (auto ind = 0U; ind < KnotCount; ind++) {
	  stCor2px(StitchBuffer->operator[](Knots[ind]), point);
	  SelectObject(StitchWindowMemDC, KnotPen);
	  SetROP2(StitchWindowMemDC, R2_XORPEN);
	  tlin[0].x = tlin[3].x = tlin[4].x = point.x - kOffset;
	  tlin[1].x = tlin[2].x = point.x + kOffset;
	  tlin[0].y = tlin[1].y = tlin[4].y = point.y + kOffset;
	  tlin[2].y = tlin[3].y = point.y - kOffset;
	  Polyline(StitchWindowMemDC, static_cast<POINT const*>(tlin), 5);
	  tlin[0].x = point.x - kLine;
	  tlin[1].x = point.x + kLine;
	  tlin[0].y = tlin[1].y = point.y;
	  Polyline(StitchWindowMemDC, static_cast<POINT const*>(tlin), 2);
	  SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
  }
}

void thred::internal::dugrid() {
  if (ZoomFactor < ShowStitchThreshold || (ShowStitchThreshold == 0.0)) {
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, GridPen);
	auto const gridRect    = RECT {wrap::ceil<int32_t>(ZoomRect.left / IniFile.gridSize),
                                wrap::floor<int32_t>(ZoomRect.top / IniFile.gridSize),
                                wrap::floor<int32_t>(ZoomRect.right / IniFile.gridSize),
                                wrap::ceil<int32_t>(ZoomRect.bottom / IniFile.gridSize)};
	POINT      gridLine[2] = {};
	gridLine[0].x          = 0;
	gridLine[1].x          = StitchWindowClientRect.right;
	for (auto iGrid = gridRect.bottom; iGrid <= gridRect.top; iGrid++) {
	  gridLine[0].y = gridLine[1].y = wrap::ceil<int32_t>(
	      StitchWindowClientRect.bottom - (iGrid * IniFile.gridSize - ZoomRect.bottom) * ZoomRatio.y);
	  Polyline(StitchWindowMemDC, static_cast<POINT const*>(gridLine), 2);
	}
	gridLine[0].y = 0;
	gridLine[1].y = StitchWindowClientRect.bottom;
	for (auto iGrid = gridRect.left; iGrid <= gridRect.right; iGrid++) {
	  gridLine[0].x = gridLine[1].x =
	      wrap::ceil<int32_t>((iGrid * IniFile.gridSize - ZoomRect.left) * ZoomRatio.x);
	  Polyline(StitchWindowMemDC, static_cast<POINT const*>(gridLine), 2);
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
}

void thred::internal::rint() noexcept {
  CellSize.x = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
  CellSize.y = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
}

auto thred::internal::setRmap(boost::dynamic_bitset<>& stitchMap, fPOINTATTR const& stitchPoint) -> bool {
  auto bitPoint = gsl::narrow<uint32_t>(floor((stitchPoint.x - ZoomRect.left) / CellSize.x) *
                                        floor((stitchPoint.y - ZoomRect.bottom) / CellSize.y));
  return !stitchMap.test_set(bitPoint);
}

void thred::internal::drawBackground() {
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
  thred::duzrat();
  if ((PCSBMPFileName[0] != 0) && !StateMap.test(StateFlag::HIDMAP) && !StateMap.test(StateFlag::UPTO)) {
	auto deviceContext = BitmapDC;
	if (StateMap.test(StateFlag::WASTRAC)) {
	  deviceContext = TraceDC;
	}
	if (bitar()) {
	  StretchBlt(StitchWindowMemDC,
	             BitmapDstRect.left,
	             BitmapDstRect.top,
	             BitmapDstRect.right - BitmapDstRect.left,
	             BitmapDstRect.bottom - BitmapDstRect.top,
	             deviceContext,
	             BitmapSrcRect.left,
	             BitmapSrcRect.top,
	             BitmapSrcRect.right - BitmapSrcRect.left,
	             BitmapSrcRect.bottom - BitmapSrcRect.top,
	             SRCCOPY);
	}
  }
  dugrid();
}

void thred::internal::drwStch() {
  auto stitchCount = 0U;

  StateMap.set(StateFlag::RELAYR);
  StateMap.reset(StateFlag::SELSHO);
  StateMap.reset(StateFlag::ILIN1);
  StateMap.reset(StateFlag::BZUM);
  unboxs();
  unrotu();
  unrot();
  StateMap.reset(StateFlag::ILIN);
  uncros();
  StateMap.reset(StateFlag::SHOFRM);
  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
	LineIndex = ColorChangeTable[iColor + 1U].stitchIndex - ColorChangeTable[iColor].stitchIndex;
	if (LineIndex > stitchCount) {
	  stitchCount = LineIndex;
	}
  }
  auto linePoints = std::vector<POINT> {};
  linePoints.resize(wrap::toSize(stitchCount) + 2U);
  thi::drawBackground();
  if (StateMap.test(StateFlag::INIT)) {
	if (StateMap.test(StateFlag::ZUMED)) {
	  ScrollInfo.cbSize = sizeof(ScrollInfo);
	  ScrollInfo.fMask  = SIF_ALL; // NOLINT
	  ScrollInfo.nMax   = UnzoomedRect.y;
	  ScrollInfo.nMin   = 0;
	  ScrollInfo.nPage  = wrap::round<int32_t>(ZoomRect.top - ZoomRect.bottom);
	  ScrollInfo.nPos   = wrap::round<decltype(ScrollInfo.nPos)>(UnzoomedRect.y - ZoomRect.top);
	  SetScrollInfo(VerticalScrollBar, SB_CTL, &ScrollInfo, TRUE);
	  ScrollInfo.nMax  = UnzoomedRect.x;
	  ScrollInfo.nPage = wrap::round<int32_t>(ZoomRect.right - ZoomRect.left);
	  ScrollInfo.nPos  = wrap::round<decltype(ScrollInfo.nPos)>(ZoomRect.left);
	  SetScrollInfo(HorizontalScrollBar, SB_CTL, &ScrollInfo, TRUE);
	  ShowWindow(HorizontalScrollBar, TRUE);
	  ShowWindow(VerticalScrollBar, TRUE);
	}
	else {
	  ShowWindow(VerticalScrollBar, FALSE);
	  ShowWindow(HorizontalScrollBar, FALSE);
	}
	thred::duzrat();
	auto const dub6      = ZoomRatio.x * 6.0F;
	ThreadWidthPixels[0] = wrap::round<int32_t>(dub6 * TSIZ30);
	ThreadWidthPixels[1] = wrap::round<int32_t>(dub6 * TSIZ40);
	ThreadWidthPixels[2] = wrap::round<int32_t>(dub6 * TSIZ60);
	for (auto iColor = 0U; iColor < 16U; iColor++) {
	  if (StateMap.test(StateFlag::THRDS)) {
		nuStchSiz(iColor, ThreadWidthPixels[ThreadSizeIndex[iColor]]);
	  }
	  else {
		if (StateMap.test(StateFlag::COL)) {
		  if (iColor == ActiveColor) {
			nuStchSiz(iColor, ThreadWidthPixels[ThreadSizeIndex[iColor]]);
		  }
		  else {
			nuStchSiz(iColor, 1);
		  }
		}
		else {
		  nuStchSiz(iColor, 1);
		}
	  }
	}
	LineIndex = 0;
	DisplayedColorBitmap.reset();
	if (StateMap.test(StateFlag::ZUMED)) {
	  LineIndex = 0;
	  StateMap.reset(StateFlag::LINED);
	  StateMap.reset(StateFlag::LININ);
	  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
		if (StateMap.test(StateFlag::HID)) {
		  if (ColorChangeTable[iColor].colorIndex != ActiveColor) {
			stitchCount = ColorChangeTable[iColor + 1U].stitchIndex - ColorChangeTable[iColor].stitchIndex;
			auto const stitchIt = std::next(StitchBuffer->begin(), ColorChangeTable[iColor].stitchIndex);
			for (auto iStitch = 0U; iStitch < stitchCount; iStitch++) {
			  if (stitchIt[iStitch].x >= ZoomRect.left && stitchIt[iStitch].x <= ZoomRect.right &&
			      stitchIt[iStitch].y >= ZoomRect.bottom && stitchIt[iStitch].y <= ZoomRect.top) {
				DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
				break;
			  }
			}
			continue;
		  }
		}
		auto wascol = 0U;
		SelectObject(StitchWindowMemDC, UserPen[ColorChangeTable[iColor].colorIndex]);
		stitchCount = ColorChangeTable[iColor + 1U].stitchIndex - ColorChangeTable[iColor].stitchIndex;
		if (!StitchBuffer->empty()) {
		  auto const stitchIt = std::next(StitchBuffer->begin(), ColorChangeTable[iColor].stitchIndex);
		  stitchCount         = chkup(stitchCount, iColor);
		  auto const maxYcoord = gsl::narrow_cast<float>(DrawItem->rcItem.bottom);

		  for (auto iStitch = 0U; iStitch < stitchCount; iStitch++) {
			auto const layer = (stitchIt[iStitch].attribute & LAYMSK) >> LAYSHFT;
			if ((ActiveLayer == 0U) || (layer == 0U) || (layer == ActiveLayer)) {
			  if (stitchIt[iStitch].x >= ZoomRect.left && stitchIt[iStitch].x <= ZoomRect.right &&
			      stitchIt[iStitch].y >= ZoomRect.bottom && stitchIt[iStitch].y <= ZoomRect.top) {
				wascol = 1;
				if (StateMap.testAndSet(StateFlag::LINED)) {
				  if (StateMap.testAndSet(StateFlag::LININ)) {
					linePoints[LineIndex++] =
					    POINT {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					           wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                               ZoomRatio.y)};
				  }
				  else {
					if (iStitch == 0) {
					  linePoints[LineIndex++] =
					      POINT {wrap::ceil<int32_t>((stitchIt->x - ZoomRect.left) * ZoomRatio.x),
					             wrap::ceil<int32_t>(maxYcoord -
					                                 (stitchIt->y - ZoomRect.bottom) * ZoomRatio.y)};
					}
					else {
					  linePoints[LineIndex++] = POINT {
					      wrap::ceil<int32_t>((stitchIt[iStitch - 1U].x - ZoomRect.left) * ZoomRatio.x),
					      wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1U].y - ZoomRect.bottom) *
					                                          ZoomRatio.y)};
					  linePoints[LineIndex++] = POINT {
					      wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					      wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                          ZoomRatio.y)};
					}
				  }
				}
				else {
				  if (iStitch == 0) {
					linePoints[0] =
					    POINT {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					           wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                               ZoomRatio.y)};
					LineIndex = 1;
				  }
				  else {
					linePoints[0] = POINT {
					    wrap::ceil<int32_t>((stitchIt[iStitch - 1U].x - ZoomRect.left) * ZoomRatio.x),
					    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1U].y - ZoomRect.bottom) *
					                                        ZoomRatio.y)};
					linePoints[1] =
					    POINT {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					           wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                               ZoomRatio.y)};
					LineIndex = 2;
				  }
				  StateMap.set(StateFlag::LININ);
				}
			  }
			  else {
				if (StateMap.testAndReset(StateFlag::LININ)) {
				  linePoints[LineIndex++] =
				      POINT {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
				             wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
				                                                 ZoomRatio.y)};
				  wrap::Polyline(StitchWindowMemDC, linePoints.data(), LineIndex);
				  LineIndex = 0;
				}
				else {
				  if (iStitch != 0U) {
					// write an equation for this line
					auto const xDelta        = stitchIt[iStitch].x - stitchIt[iStitch - 1U].x;
					auto const yDelta        = stitchIt[iStitch - 1U].y - stitchIt[iStitch].y;
					auto const slope         = gsl::narrow_cast<double>(xDelta) / yDelta;
					auto const offset        = stitchIt[iStitch].x + slope * stitchIt[iStitch].y;
					POINT      stitchLine[2] = {};
					do {
					  // does the line intersect with the top of the screen?
					  auto gapToEdge = offset - slope * ZoomRect.top;
					  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
						stitchLine[0] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch - 1U].x - ZoomRect.left) *
						                        ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1U].y - ZoomRect.bottom) *
						                                        ZoomRatio.x)};
						stitchLine[1] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
						                                        ZoomRatio.x)};
						Polyline(StitchWindowMemDC, static_cast<POINT const*>(stitchLine), 2);
						break;
					  }
					  // does the line intersect the bottom of the screen?
					  gapToEdge = offset - slope * ZoomRect.bottom;
					  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
						stitchLine[0] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch - 1U].x - ZoomRect.left) *
						                        ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1U].y - ZoomRect.bottom) *
						                                        ZoomRatio.y)};
						stitchLine[1] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
						                                        ZoomRatio.y)};
						Polyline(StitchWindowMemDC, static_cast<POINT const*>(stitchLine), 2);
						break;
					  }
					  // does the line intersect the left side of the screen?
					  if (slope != 0.0) {
						gapToEdge = (offset - ZoomRect.left) / slope;
						if (gapToEdge >= ZoomRect.bottom && gapToEdge <= ZoomRect.top) {
						  stitchLine[0] = POINT {
						      wrap::ceil<int32_t>((stitchIt[iStitch - 1U].x - ZoomRect.left) *
						                          ZoomRatio.x),
						      wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1U].y - ZoomRect.bottom) *
						                                          ZoomRatio.y)};
						  stitchLine[1] = POINT {
						      wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
						      wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
						                                          ZoomRatio.y)};
						  Polyline(StitchWindowMemDC, static_cast<POINT const*>(stitchLine), 2);
						}
					  }
					} while (false);
				  }
				}
			  }
			}
		  }
		}
		if (LineIndex != 0U) {
		  wrap::Polyline(StitchWindowMemDC, linePoints.data(), LineIndex);
		  linePoints[0] = linePoints[LineIndex - 1U];
		  LineIndex     = 1;
		}
		if (wascol != 0U) {
		  DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
		}
	  }
	}
	else {
	  auto const pwid = StateMap.test(StateFlag::HID);
	  for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
		DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
		stitchCount = ColorChangeTable[iColor + 1U].stitchIndex - ColorChangeTable[iColor].stitchIndex;
		stitchCount = chkup(stitchCount, iColor);
		if (!pwid || ColorChangeTable[iColor].colorIndex == ActiveColor) {
		  drwLin(linePoints,
		         ColorChangeTable[iColor].stitchIndex,
		         stitchCount,
		         UserPen[ColorChangeTable[iColor].colorIndex]);
		}
	  }
	}
	if (StateMap.test(StateFlag::SELBOX)) {
	  if (!StitchBuffer->empty()) {
		ritcor(StitchBuffer->operator[](ClosestPointIndex));
		if (stch2px(ClosestPointIndex)) {
		  dubox();
		}
	  }
	}
	if (StateMap.test(StateFlag::FRMPSEL)) {
	  auto vertexIt = std::next(FormVertices->cbegin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
	  thred::ritfcor(vertexIt[ClosestVertexToCursor]);
	}
	if (!StateMap.test(StateFlag::SELBOX) && !StateMap.test(StateFlag::FRMPSEL)) {
	  auto const blank = std::wstring {};
	  displayText::butxt(HCOR, blank);
	}
	if (StateMap.test(StateFlag::WASLIN)) {
	  relin();
	}
	if (StateMap.test(StateFlag::GRPSEL)) {
	  if (cmpstch(ClosestPointIndex, GroupStitchIndex)) {
		cros(ClosestPointIndex);
	  }
	  else {
		SearchLine->clear();
		ducros(StitchWindowMemDC);
	  }
	  thred::selRct(StitchRangeRect);
	  nuselrct();
	  StateMap.set(StateFlag::SELSHO);
	  dusel(StitchWindowMemDC);
	}
	if (ZoomFactor < StitchBoxesThreshold) {
	  rint();
	  auto const maxMapSize = StitchWindowClientRect.right * StitchWindowClientRect.bottom;

	  auto stitchMap = boost::dynamic_bitset<>(maxMapSize, 0U);
	  SelectObject(StitchWindowMemDC, LinePen);
	  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	  if (StateMap.test(StateFlag::HID)) {
		for (auto iColor = 0U; iColor < ColorChanges; iColor++) {
		  if (ColorChangeTable[iColor].colorIndex == ActiveColor) {
			for (auto iStitch = ColorChangeTable[iColor].stitchIndex;
			     iStitch < ColorChangeTable[iColor + 1U].stitchIndex;
			     iStitch++) {
			  auto const stitch = StitchBuffer->operator[](iStitch);
			  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right && stitch.y >= ZoomRect.bottom &&
			      stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch)) {
				stchbox(iStitch, StitchWindowMemDC);
			  }
			}
		  }
		}
	  }
	  else {
		auto iStitch = 0U;
		for (auto& stitch : *StitchBuffer) {
		  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right &&
		      stitch.y >= ZoomRect.bottom && stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch)) {
			stchbox(iStitch, StitchWindowMemDC);
		  }
		  iStitch++;
		}
	  }
	  SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
	if (StateMap.test(StateFlag::CLPSHO)) {
	  duclp();
	}
  }
  if (!FormList->empty() && !StateMap.test(StateFlag::FRMOF)) {
	form::drwfrm();
  }
  if (StateMap.test(StateFlag::INSFRM)) {
	form::rinfrm();
  }
  if (StateMap.test(StateFlag::BIGBOX)) {
	form::dubig();
  }
  if (StateMap.test(StateFlag::MOVFRM)) {
	StateMap.set(StateFlag::SHOFRM);
	form::dufrm();
  }
  if (StateMap.test(StateFlag::SATPNT)) {
	satin::satzum();
  }
  if (StateMap.test(StateFlag::GMRK)) {
	drwmrk(StitchWindowMemDC);
  }
  if (StateMap.test(StateFlag::PRFACT)) {
	thred::redraw(PreferencesWindow);
  }
  if (StateMap.test(StateFlag::SELBOX)) {
	displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
  }
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  if (StateMap.test(StateFlag::INIT)) {
	lenCalc();
  }
  if (StateMap.test(StateFlag::MOVSET)) {
	dumov();
  }
  drwknot();
}

void thred::internal::dubar() {
  auto colorBarRect = RECT {DrawItem->rcItem.left, 0, DrawItem->rcItem.right, DrawItem->rcItem.bottom};
  POINT indicatorLine[2] = {};

  for (auto iColorChange = 0U; iColorChange < ColorChanges; iColorChange++) {
	auto const barSectionHeight =
	    gsl::narrow_cast<double>(ColorChangeTable[iColorChange + 1U].stitchIndex) / StitchBuffer->size();
	colorBarRect.bottom = wrap::round<int32_t>(barSectionHeight * DrawItem->rcItem.bottom);
	FillRect(DrawItem->hDC, &colorBarRect, UserColorBrush[ColorChangeTable[iColorChange].colorIndex]);
	colorBarRect.top = colorBarRect.bottom;
  }
  if (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::GRPSEL)) {
	auto selectedIndicator = gsl::narrow_cast<double>(ClosestPointIndex) / StitchBuffer->size();
	indicatorLine[0].y = indicatorLine[1].y = wrap::ceil<int32_t>(colorBarRect.bottom * selectedIndicator);
	indicatorLine[0].x                      = colorBarRect.left;
	indicatorLine[1].x                      = colorBarRect.right;
	SelectObject(DrawItem->hDC, CrossPen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(DrawItem->hDC, static_cast<POINT const*>(indicatorLine), 2);
	if (StateMap.test(StateFlag::GRPSEL)) {
	  selectedIndicator = gsl::narrow_cast<double>(GroupStitchIndex) / StitchBuffer->size();
	  indicatorLine[0].y = indicatorLine[1].y = wrap::ceil<int32_t>(colorBarRect.bottom * selectedIndicator);
	  indicatorLine[0].x                      = colorBarRect.left;
	  indicatorLine[1].x                      = colorBarRect.right;
	  Polyline(DrawItem->hDC, static_cast<POINT const*>(indicatorLine), 2);
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
}

void thred::internal::ritbak(fs::path const& fileName, DRAWITEMSTRUCT* drawItem) {
  auto thrEdFile = CreateFile(fileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (thrEdFile != INVALID_HANDLE_VALUE) { // NOLINT
	auto stitchHeader = STRHED {};
	ReadFile(thrEdFile, &stitchHeader, sizeof(stitchHeader), &BytesRead, nullptr);
	if (BytesRead == sizeof(stitchHeader)) {
	  auto       stitchSourceSize = fPOINT {1.0F, 1.0F};
	  auto const fileTypeVersion  = (stitchHeader.headerType & 0xff000000) >> 24U;
	  if ((stitchHeader.headerType & 0xffffffU) == 0x746872U) {
		switch (fileTypeVersion) {
		  case 0: {
			if (PCSHeader.hoopType == SMALHUP) {
			  stitchSourceSize.x = IniFile.hoopSizeX = SHUPX;
			  stitchSourceSize.y = IniFile.hoopSizeY = SHUPY;
			}
			else {
			  stitchSourceSize.x = IniFile.hoopSizeX = LHUPX;
			  stitchSourceSize.y = IniFile.hoopSizeY = LHUPY;
			}
			break;
		  }
		  case 1:
		  case 2: {
			auto extendedHeader = STREX {};
			ReadFile(thrEdFile, &extendedHeader, sizeof(extendedHeader), &BytesRead, nullptr);
			if (BytesRead != sizeof(extendedHeader)) {
			  return;
			}
			stitchSourceSize.x = ExtendedHeader->hoopSizeX;
			stitchSourceSize.y = ExtendedHeader->hoopSizeY;
			break;
		  }
		  default: {
			return;
		  }
		}
	  }
	  auto const drawingDestinationSize = POINT {(drawItem->rcItem.right - drawItem->rcItem.left),
	                                             (drawItem->rcItem.bottom - drawItem->rcItem.top)};
	  auto const xRatio = gsl::narrow_cast<double>(drawingDestinationSize.x) / stitchSourceSize.x;
	  auto const yRatio = gsl::narrow_cast<double>(drawingDestinationSize.y) / stitchSourceSize.y;
	  auto       ratio  = 0.0;
	  if (xRatio < yRatio) {
		ratio = xRatio;
	  }
	  else {
		ratio = yRatio;
	  }
	  if (stitchHeader.stitchCount != 0U) {
		auto stitchesToDraw = std::vector<fPOINTATTR> {};
		stitchesToDraw.resize(stitchHeader.stitchCount);
		auto lines = std::vector<POINT> {};
		lines.resize(stitchHeader.stitchCount);
		auto const bytesToRead =
		    gsl::narrow_cast<DWORD>(stitchHeader.stitchCount * sizeof(StitchBuffer->front()));
		ReadFile(thrEdFile, stitchesToDraw.data(), bytesToRead, &BytesRead, nullptr);
		if (bytesToRead == BytesRead) {
		  SetFilePointer(thrEdFile, 16, nullptr, FILE_CURRENT);
		  auto brushColor = COLORREF {};
		  ReadFile(thrEdFile, &brushColor, sizeof(brushColor), &BytesRead, nullptr);
		  auto colors = std::vector<COLORREF> {};
		  colors.resize(16U);
		  wrap::ReadFile(thrEdFile,
		                 colors.data(),
		                 wrap::toUnsigned(colors.size() * sizeof(decltype(colors.back()))),
		                 &BytesRead,
		                 nullptr);
		  auto brush = CreateSolidBrush(brushColor);
		  SelectObject(drawItem->hDC, brush);
		  FillRect(drawItem->hDC, &drawItem->rcItem, brush);
		  auto iColor = stitchesToDraw[0].attribute & 0xfU;
		  auto pen    = wrap::CreatePen(PS_SOLID, 1, colors[iColor]);
		  auto iLine  = 0U;
		  for (auto iStitch = 0U; iStitch < stitchHeader.stitchCount; iStitch++) {
			if ((stitchesToDraw[iStitch].attribute & 0xfU) == iColor) {
			  lines[iLine++] = {
			      wrap::round<int32_t>(stitchesToDraw[iStitch].x * ratio),
			      wrap::round<int32_t>(drawingDestinationSize.y - stitchesToDraw[iStitch].y * ratio)};
			}
			else {
			  pen = nuPen(pen, 1, colors[iColor]);
			  SelectObject(drawItem->hDC, pen);
			  wrap::Polyline(drawItem->hDC, lines.data(), iLine);
			  iLine  = 0;
			  iColor = stitchesToDraw[iStitch].attribute & 0xfU;
			}
		  }
		  if (iLine != 0U) {
			pen = nuPen(pen, 1, colors[iColor]);
			SelectObject(drawItem->hDC, pen);
			wrap::Polyline(drawItem->hDC, lines.data(), iLine);
		  }
		  DeleteObject(brush);
		  DeleteObject(pen);
		}
		else {
		  CloseHandle(thrEdFile);
		  return;
		}
	  }
	  else {
		SetFilePointer(thrEdFile, 84, nullptr, FILE_CURRENT);
	  }
	  if (stitchHeader.formCount != 0U) {
		do {
		  SetFilePointer(thrEdFile, 80, nullptr, FILE_CURRENT);
		  auto formList = std::vector<FRMHED> {};
		  formList.resize(stitchHeader.formCount);
		  auto vertexList = std::vector<fPOINT> {};
		  vertexList.resize(stitchHeader.vertexCount);
		  if (fileTypeVersion < 2) {
			auto formListOriginal = std::vector<FRMHEDO> {};
			formListOriginal.resize(stitchHeader.formCount);
			auto const bytesToRead = stitchHeader.formCount *
			                         gsl::narrow<uint32_t>(sizeof(decltype(formListOriginal.back())));
			ReadFile(thrEdFile, formListOriginal.data(), bytesToRead, &BytesRead, nullptr);
			if (BytesRead != bytesToRead) {
			  break;
			}
			std::copy(formListOriginal.cbegin(), formListOriginal.cend(), formList.begin());
		  }
		  else {
			auto inFormList = std::vector<FRMHEDOUT> {};
			inFormList.resize(stitchHeader.formCount);
			auto const bytesToRead =
			    gsl::narrow<uint32_t>(stitchHeader.formCount * sizeof(decltype(inFormList.back())));
			wrap::ReadFile(thrEdFile, inFormList.data(), bytesToRead, &BytesRead, nullptr);
			if (BytesRead != bytesToRead) {
			  break;
			}
			std::copy(inFormList.cbegin(), inFormList.cend(), formList.begin());
		  }
		  auto const bytesToRead =
		      stitchHeader.vertexCount * gsl::narrow<uint32_t>(sizeof(decltype(vertexList.back())));
		  ReadFile(thrEdFile, vertexList.data(), bytesToRead, &BytesRead, nullptr);
		  if (BytesRead != bytesToRead) {
			break;
		  }
		  auto lines    = std::vector<POINT> {};
		  auto maxLines = 0U;
		  for (auto iForm = 0; iForm < stitchHeader.formCount; iForm++) {
			if (formList[iForm].vertexCount > maxLines) {
			  maxLines = formList[iForm].vertexCount;
			}
		  }
		  lines.resize(wrap::toSize(maxLines) + 1U);
		  auto iVertex = 0U;
		  for (auto iForm = 0; iForm < stitchHeader.formCount; iForm++) {
			auto const iLine = iVertex;
			for (auto iVertexInForm = 0U;
			     (iVertexInForm < formList[iForm].vertexCount) && (iVertex < stitchHeader.vertexCount);
			     iVertexInForm++) {
			  lines[iVertexInForm] = {
			      wrap::round<int32_t>(vertexList[iVertex].x * ratio),
			      wrap::round<int32_t>(drawingDestinationSize.y - vertexList[iVertex++].y * ratio)};
			}
			lines[formList[iForm].vertexCount] = {
			    wrap::round<int32_t>(vertexList[iLine].x * ratio),
			    wrap::round<int32_t>(drawingDestinationSize.y - vertexList[iLine].y * ratio)};
			SelectObject(drawItem->hDC, FormPen);
			SetROP2(drawItem->hDC, R2_XORPEN);
			if (formList[iForm].type == FRMLINE) {
			  wrap::Polyline(drawItem->hDC, lines.data(), formList[iForm].vertexCount);
			}
			else {
			  wrap::Polyline(drawItem->hDC, lines.data(), formList[iForm].vertexCount + 1U);
			}
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		  }
		} while (false);
	  }
	}
	CloseHandle(thrEdFile);
  }
}

struct CreateParams {
  BOOL bEnableNonClientDpiScaling;
};

auto CALLBACK thred::internal::WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
  switch (message) {
#if HIGHDPI
	case WM_NCCREATE: {
	  // Enable per-monitor DPI scaling for caption, menu, and top-level
	  // scroll bars.
	  //
	  // Non-client area (scroll bars, caption bar, etc.) does not DPI scale
	  // automatically on Windows 8.1. In Windows 10 (1607) support was added
	  // for this via a call to EnableNonClientDpiScaling. Windows 10 (1703)
	  // supports this automatically when the DPI_AWARENESS_CONTEXT is
	  // DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2.
	  //
	  EnableNonClientDpiScaling(p_hWnd);
	  break;
	}
#endif
	case WM_PAINT: {
	  displayText::updateWinFont(p_hWnd);
	  break;
	}
	case WM_INITMENU: {
	  if (StateMap.testAndReset(StateFlag::PRFACT)) {
		DestroyWindow(PreferencesWindow);
		StateMap.reset(StateFlag::WASRT);
	  }
	  thred::unmsg();
	  thred::undat();
	  StateMap.reset(StateFlag::FORMIN);
	  StateMap.set(StateFlag::RESTCH);
	  break;
	}
	case WM_HSCROLL: {
	  switch (gsl::narrow<int32_t>(LOWORD(wParam))) { // NOLINT
		case SB_LINELEFT: {
		  if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
			MovieTimeStep += SPEDLIN;
			if (MovieTimeStep > MAXDELAY) {
			  MovieTimeStep = MAXDELAY;
			}
			setsped();
			SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
		  }
		  else {
			auto scrollPoint = POINT {0L, 0L}; // for scroll bar functions
			scrollPoint.x    = wrap::round<int32_t>((ZoomRect.right - ZoomRect.left) * LINSCROL);
			if (scrollPoint.x == 0) {
			  scrollPoint.x = 1;
			}
			rshft(scrollPoint);
		  }
		  return 1;
		}
		case SB_LINERIGHT: {
		  if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
			MovieTimeStep -= SPEDLIN;
			if (MovieTimeStep < MINDELAY) {
			  MovieTimeStep = MINDELAY;
			}
			setsped();
			SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
		  }
		  else {
			auto scrollPoint = POINT {0L, 0L}; // for scroll bar functions
			scrollPoint.x    = wrap::round<int32_t>(-(ZoomRect.right - ZoomRect.left) * LINSCROL);
			if (scrollPoint.x == 0) {
			  scrollPoint.x = -1;
			}
			rshft(scrollPoint);
		  }
		  return 1;
		}
		case SB_PAGELEFT: {
		  if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
			MovieTimeStep += SPEDPAG;
			if (MovieTimeStep < MINDELAY) {
			  MovieTimeStep = MINDELAY;
			}
			setsped();
			SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
		  }
		  else {
			pglft();
		  }
		  return 1;
		}
		case SB_PAGERIGHT: {
		  if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
			MovieTimeStep -= SPEDPAG;
			if (MovieTimeStep < MINDELAY) {
			  MovieTimeStep = MINDELAY;
			}
			setsped();
			SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
		  }
		  else {
			pgrit();
		  }
		  return 1;
		}
		case SB_THUMBPOSITION: {
		  if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
			GSL_SUPPRESS(26490) {
			  if (reinterpret_cast<HWND>(lParam) == SpeedScrollBar) { // NOLINT
				auto const position = HIWORD(wParam);                 // NOLINT
				MovieTimeStep       = MAXDELAY - position;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, position, TRUE);
			  }
			}
		  }
		  else {
			GSL_SUPPRESS(26490) {
			  if (reinterpret_cast<HWND>(lParam) == HorizontalScrollBar) { // NOLINT
				auto const zoomWidth = ZoomRect.right - ZoomRect.left;
				ZoomRect.left        = gsl::narrow<int32_t>(HIWORD(wParam)); // NOLINT
				ZoomRect.right       = ZoomRect.left + zoomWidth;
				if (ZoomRect.right > UnzoomedRect.x) {
				  ZoomRect.right = UnzoomedRect.x;
				  ZoomRect.left  = UnzoomedRect.x - zoomWidth;
				}
				StateMap.set(StateFlag::RESTCH);
			  }
			}
		  }
		  return 1;
		}
	  }
	  break;
	}
	case WM_VSCROLL: {
	  switch (LOWORD(wParam)) { // NOLINT
		case SB_LINEDOWN: {
		  auto scrollPoint = POINT {0L, 0L};
		  scrollPoint.y    = wrap::round<int32_t>((ZoomRect.top - ZoomRect.bottom) * LINSCROL);
		  if (scrollPoint.y == 0) {
			scrollPoint.y = 1;
		  }
		  rshft(scrollPoint);
		  return 1;
		}
		case SB_LINEUP: {
		  auto scrollPoint = POINT {0L, 0L};
		  scrollPoint.y    = wrap::round<int32_t>(-(ZoomRect.top - ZoomRect.bottom) * LINSCROL);
		  if (scrollPoint.y == 0) {
			scrollPoint.y = -1;
		  }
		  rshft(scrollPoint);
		  return 1;
		}
		case SB_PAGEDOWN: {
		  pgdwn();
		  return 1;
		}
		case SB_PAGEUP: {
		  pgup();
		  return 1;
		}
		case SB_THUMBPOSITION: {
		  auto const zoomHeight = ZoomRect.top - ZoomRect.bottom;
		  ZoomRect.top = gsl::narrow_cast<float>(UnzoomedRect.y) - gsl::narrow<int32_t>(HIWORD(wParam)); // NOLINT
		  ZoomRect.bottom = ZoomRect.top - zoomHeight;
		  if (ZoomRect.bottom < 0) {
			ZoomRect.bottom = 0.0F;
			ZoomRect.top    = zoomHeight;
		  }
		  StateMap.set(StateFlag::RESTCH);
		  return 1;
		}
	  }
	  break;
	}
	case WM_DRAWITEM: {
	  // owner draw windows
	  GSL_SUPPRESS(26490) DrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam); // NOLINT
	  if (DrawItem->hwndItem == MainStitchWin && DrawItem->itemAction == ODA_DRAWENTIRE) {
		if (StateMap.test(StateFlag::TXTRED)) {
		  texture::drwtxtr();
		  return 1;
		}
		if (!StateMap.test(StateFlag::RUNPAT)) {
		  if (!StateMap.test(StateFlag::HIDSTCH) &&
		      ((FileHandle != nullptr) || StateMap.test(StateFlag::INIT) || !FormList->empty() ||
		       StateMap.test(StateFlag::SATPNT)) &&
		      !StateMap.test(StateFlag::BAKSHO)) {
			drwStch();
		  }
		  else {
			thi::drawBackground();
			if (StateMap.test(StateFlag::HIDSTCH)) {
			  form::drwfrm();
			  if (StateMap.test(StateFlag::SATPNT)) {
				satin::satzum();
			  }
			}
			if (StateMap.test(StateFlag::PRFACT)) {
			  thred::redraw(PreferencesWindow);
			}
			else {
			  if (StateMap.test(StateFlag::MOVFRM)) {
				form::unfrm();
				StateMap.set(StateFlag::SHOFRM);
				form::dufrm();
			  }
			  if (StateMap.test(StateFlag::POLIMOV)) {
				StateMap.set(StateFlag::SHOFRM);
				form::mdufrm();
			  }
			}
		  }
		  BitBlt(StitchWindowDC,                // handle to destination DC
		         0,                             // x-coord of destination upper-left corner
		         0,                             // y-coord of destination upper-left corner
		         StitchWindowClientRect.right,  // width of destination rectangle
		         StitchWindowClientRect.bottom, // height of destination rectangle
		         StitchWindowMemDC,             // handle to source DC
		         0,                             // x-coordinate of source upper-left corner
		         0,                             // y-coordinate of source upper-left corner
		         SRCCOPY                        // raster operation code
		  );
		  if (FormDataSheet != nullptr) {
			thred::redraw(FormDataSheet);
		  }
		  if (StateMap.test(StateFlag::ROTSHO)) {
			durot();
		  }
		  if (StateMap.test(StateFlag::SHOSAT)) {
			satin::dusat();
		  }
		  if (StateMap.test(StateFlag::SHOINSF)) {
			form::duinsf();
		  }
		}
		return 1;
	  }
	  if (DrawItem->hwndItem == ColorBar && DrawItem->itemAction == ODA_DRAWENTIRE) {
		if (!StitchBuffer->empty()) {
		  dubar();
		}
		else {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_WINDOW));
		}
		return 1;
	  }
	  if (!ButtonWin->empty() && DrawItem->hwndItem == ButtonWin->operator[](HHID) &&
	      DrawItem->itemAction == ODA_DRAWENTIRE) {
		auto const position = (ButtonWidthX3 - PickColorMsgSize.cx) / 2;
		if (StateMap.test(StateFlag::HID)) {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[ActiveColor]);
		  SetBkColor(DrawItem->hDC, UserColor[ActiveColor]);
		}
		else {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_BTNFACE));
		}
		if (StateMap.test(StateFlag::TXTRED)) {
		  texture::writeScreenWidth(position);
		}
		else {
		  wrap::TextOut(DrawItem->hDC,
		                position,
		                1,
		                StringTable->operator[](STR_PIKOL).c_str(),
		                wrap::toUnsigned(StringTable->operator[](STR_PIKOL).size()));
		}
		return 1;
	  }
	  if (StateMap.test(StateFlag::WASTRAC)) {
		trace::wasTrace();
		return 1;
	  }

	  for (auto iColor = 0U; iColor < 16U; iColor++) {
		if (DrawItem->hwndItem == DefaultColorWin->operator[](iColor)) {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, DefaultColorBrush[iColor]);
		  if (DisplayedColorBitmap.test(iColor)) {
			SetBkColor(DrawItem->hDC, DefaultColors[iColor]);
			SetTextColor(DrawItem->hDC, defTxt(iColor));
			auto colorNum = std::wstring(fmt::format(L"{}", iColor + 1U));
			auto textSize = SIZE {0L, 0L};
			wrap::GetTextExtentPoint32(
			    DrawItem->hDC, colorNum.c_str(), wrap::toUnsigned(colorNum.size()), &textSize);
			wrap::TextOut(DrawItem->hDC,
			              (ButtonWidth - textSize.cx) / 2,
			              0,
			              colorNum.c_str(),
			              wrap::toUnsigned(colorNum.size()));
		  }
		  return 1;
		}
		if (DrawItem->hwndItem == UserColorWin->operator[](iColor)) {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[iColor]);
		  if (iColor == ActiveColor) {
			SelectObject(DrawItem->hDC, CrossPen);
			SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
			POINT line[2] = {};
			line[0].x = line[1].x = DrawItem->rcItem.right / 2;
			line[0].y             = 0;
			line[1].y             = DrawItem->rcItem.bottom;
			Polyline(DrawItem->hDC, static_cast<POINT const*>(line), 2);
			line[0].y = line[1].y = DrawItem->rcItem.bottom / 2;
			line[0].x             = 0;
			line[1].x             = DrawItem->rcItem.right;
			Polyline(DrawItem->hDC, static_cast<POINT const*>(line), 2);
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		  }
		  return 1;
		}
	  }

	  if (StateMap.test(StateFlag::BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {
		if (StateMap.test(StateFlag::THUMSHO)) {
		  for (auto iThumb = 0U; iThumb < 4; iThumb++) {
			if (iThumb < ThumbnailDisplayCount && DrawItem->hwndItem == BackupViewer[iThumb]) {
			  ritbak(Thumbnails->operator[](ThumbnailsSelected[iThumb]).data(), DrawItem);
			  rthumnam(iThumb);
			  return 1;
			}
		  }
		}
		else {
		  for (auto iVersion = 0; iVersion < OLDVER; iVersion++) {
			if (DrawItem->hwndItem == BackupViewer[iVersion]) {
			  auto fileName = *ThrName;
			  auto ext      = fileName.extension().wstring();
			  ext.back()    = iVersion + 's';
			  fileName.replace_extension(ext);
			  ritbak(fileName, DrawItem);
			  return 1;
			}
		  }
		}
	  }
	  break;
	}
	case WM_SIZE: {
	  GetClientRect(p_hWnd, &ThredWindowRect);
	  switch (wParam) {
		case SIZE_MAXIMIZED: {
		  UserFlagMap.set(UserFlag::SAVMAX);
		  break;
		}
		case SIZE_MINIMIZED: {
		  UserFlagMap.reset(UserFlag::SAVMAX);
		  break;
		}
		case SIZE_RESTORED: {
		  UserFlagMap.reset(UserFlag::SAVMAX);
		  chkirct();
		  if (StateMap.testAndSet(StateFlag::SIZED)) {
			auto       screenCenterOffset = 0U;
			auto const maxWindowDimension = SIZE {ScreenSizePixels.cx - 30, ScreenSizePixels.cy - 30};
			if ((ThredWindowRect.right - ThredWindowRect.left) > maxWindowDimension.cx) {
			  screenCenterOffset    = (ScreenSizePixels.cx - maxWindowDimension.cx) / 2;
			  ThredWindowRect.left  = screenCenterOffset;
			  ThredWindowRect.right = ScreenSizePixels.cx - screenCenterOffset;
			}
			if ((ThredWindowRect.bottom - ThredWindowRect.top) > maxWindowDimension.cy) {
			  screenCenterOffset     = (ScreenSizePixels.cy - maxWindowDimension.cy) / 2;
			  ThredWindowRect.top    = screenCenterOffset;
			  ThredWindowRect.bottom = ScreenSizePixels.cy - screenCenterOffset;
			}
			if (screenCenterOffset != 0U) {
			  MoveWindow(p_hWnd,
			             ThredWindowRect.left,
			             ThredWindowRect.top,
			             ThredWindowRect.right - ThredWindowRect.left,
			             ThredWindowRect.bottom - ThredWindowRect.top,
			             TRUE);
			}
		  }
		  else {
			MoveWindow(p_hWnd,
			           IniFile.initialWindowCoords.left,
			           IniFile.initialWindowCoords.top,
			           IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left,
			           IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top,
			           TRUE);
		  }
		  ShowWindow(p_hWnd, SW_SHOW);
		  break;
		}
		default: {
		}
	  }
	  GetClientRect(p_hWnd, &ThredWindowRect);
	  thred::movStch();
	  if (StateMap.test(StateFlag::ZUMED)) {
		auto const bRatio        = StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
		auto const adjustedWidth = StitchWindowClientRect.right / bRatio;
		if (adjustedWidth + ZoomRect.left > UnzoomedRect.x) {
		  ZoomRect.right = UnzoomedRect.x;
		  ZoomRect.left  = UnzoomedRect.x - adjustedWidth;
		}
		else {
		  ZoomRect.right = adjustedWidth + ZoomRect.left;
		}
	  }
	  else {
		ZoomRect =
		    fRECTANGLE {0.0F, gsl::narrow<float>(UnzoomedRect.y), gsl::narrow<float>(UnzoomedRect.x), 0.0F};
	  }
	  NearestCount = 0;
	  StateMap.set(StateFlag::RESTCH);
	  if (StateMap.test(StateFlag::SELBOX)) {
		shft2box();
	  }
	  if (StateMap.test(StateFlag::RUNPAT)) {
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		if (StateMap.test(StateFlag::ZUMED)) {
		  RunPoint = GroupStartStitch;
		}
		else {
		  RunPoint = 0;
		}
	  }
	  return 1;
	}
	case WM_MOVE: {
	  GetClientRect(p_hWnd, &ThredWindowRect);
	  if ((ThredWindowRect.right - ThredWindowRect.left) < 20) {
		ThredWindowRect.left  = 0;
		ThredWindowRect.right = 300;
	  }
	  if ((ThredWindowRect.bottom - ThredWindowRect.top) < 20) {
		ThredWindowRect.top    = 0;
		ThredWindowRect.bottom = 300;
	  }
	  thred::movStch();
	  if (StateMap.test(StateFlag::RUNPAT)) {
		thred::duzrat();
		RunPoint = 0;
		FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
	  }
	  return 1;
	}
	case WM_CLOSE: {
	  dun();
	  if (StateMap.test(StateFlag::SAVEX)) {
		return 1;
	  }
	  break;
	}
	default: {
	}
  }
  return DefWindowProc(p_hWnd, message, wParam, lParam);
}

void thred::internal::sachk() {
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	auto const& form = FormList->operator[](iForm);
	if (form.type == SAT && (form.satinGuideCount != 0U)) {
	  auto guideIt = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
		if (guideIt[iGuide].start > form.vertexCount || guideIt[iGuide].finish > form.vertexCount) {
		  auto const bakclo   = ClosestFormToCursor;
		  ClosestFormToCursor = iForm;
		  satin::delsac(iForm);
		  ClosestFormToCursor = bakclo;
		}
	  }
	}
  }
}

#ifdef ALLOCFAILURE
auto handle_program_memory_depletion(uint32_t) -> int32_t {
  // ToDo - Make this handle the failure with more user notifiication
  displayText::shoMsg("Memory Allocation Failure");
  exit(EXIT_FAILURE);
}
#endif

#pragma warning(push)
#pragma warning(disable : 26461) // disable warning for hPrevInstance not being marked as a pointer to const
auto APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int32_t nShowCmd)
    -> int32_t { // NOLINT
  UNREFERENCED_PARAMETER(nShowCmd);

  ArgList = CommandLineToArgvW(GetCommandLine(), &ArgCount);

#ifdef ALLOCFAILURE
  _PNH old_handler = _set_new_handler(handle_program_memory_depletion);
  // char* testalloc = new char[(~uint32_t((int32_t)0) / 2) - 1U]();
  // testalloc[0] = 1;
#endif

  ThrEdInstance = hInstance;

  auto wc = WNDCLASSEX {0U, 0U, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
  wc.cbSize      = sizeof(WNDCLASSEX);
  wc.style       = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // NOLINT
  wc.lpfnWndProc = thi::WndProc;
  wc.cbClsExtra  = 0;
  wc.cbWndExtra  = 0;
  wc.hInstance   = ThrEdInstance;
  wc.hIcon       = gsl::narrow_cast<HICON>(
      LoadImage(ThrEdInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_SHARED)); // NOLINT
  wc.hCursor = nullptr; //  set the cursor to null as the cursor changes in the window:
                        //  https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setcursor
  wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1); // NOLINT
  wc.lpszClassName = L"thred";
  wc.hIconSm       = nullptr;

#if HIGHDPI
  auto previousDpiContext = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE); // NOLINT
#endif

  // to keep the compiler from complaining
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  if (RegisterClassEx(&wc)) {
	auto private_AngledFormVertices        = std::vector<fPOINT> {};
	auto private_AuxName                   = fs::path {};
	auto private_BSequence                 = std::vector<BSEQPNT> {};
	auto private_BalaradName0              = fs::path {};
	auto private_BalaradName1              = fs::path {};
	auto private_BalaradName2              = fs::path {};
	auto private_ButtonWin                 = std::vector<HWND> {};
	auto private_ClipBuffer                = std::vector<fPOINTATTR> {};
	auto private_ClipPoints                = std::vector<fPOINT> {};
	auto private_ColorFileName             = fs::path {};
	auto private_DefaultBMPDirectory       = fs::path {};
	auto private_DefaultColorWin           = std::vector<HWND> {};
	auto private_DefaultDirectory          = fs::path {};
	auto private_DesignerName              = std::wstring {};
	auto private_ExtendedHeader            = STREX {};
	auto private_FormAngles                = std::vector<float> {};
	auto private_FormControlPoints         = std::vector<POINT> {};
	auto private_FormLines                 = std::vector<POINT> {};
	auto private_FormList                  = std::vector<FRMHED> {};
	auto private_FormOnOff                 = std::wstring {};
	auto private_FormVertices              = std::vector<fPOINT> {};
	auto private_FormVerticesAsLine        = std::vector<POINT> {};
	auto private_GeName                    = fs::path {};
	auto private_HomeDirectory             = fs::path {};
	auto private_IniFileName               = fs::path {};
	auto private_InsidePointList           = std::vector<fPOINT> {};
	auto private_InterleaveSequence        = std::vector<fPOINT> {};
	auto private_InterleaveSequenceIndices = std::vector<INSREC> {};
	auto private_LabelWindow               = std::vector<HWND> {};
	auto formOnOff                         = private_FormOnOff.data();

	auto private_MenuInfo = MENUITEMINFO {
	  sizeof(MENUITEMINFO), // Size
	      MIIM_TYPE,        // Mask
	      MFT_STRING,       // Type
	      0,                // State
	      0,                // ID
	      nullptr,          // SubMenu
	      nullptr,          // bmpChecked
	      nullptr,          // bmpUnchecked
	      0,                // ItemData
	      formOnOff,        // TypeData
	      16,               // cch
#if (WINVER >= 0x0500)
	      nullptr // bmpItem
#endif            /* WINVER >= 0x0500 */
	};
	auto private_OSequence                 = std::vector<fPOINT> {};
	auto private_OutsidePointList          = std::vector<fPOINT> {};
	auto private_PreviousNames             = std::vector<fs::path> {};
	auto private_RGBFileName               = fs::path {};
	auto private_RubberBandLine            = std::vector<POINT> {};
	auto private_SatinGuides               = std::vector<SATCON> {};
	auto private_SearchLine                = std::vector<POINT> {};
	auto private_SearchName                = fs::path {};
	auto private_SelectedFormList          = std::vector<uint32_t> {};
	auto private_SelectedFormsLine         = std::vector<POINT> {};
	auto private_SelectedPointsLine        = std::vector<POINT> {};
	auto private_SelectedTexturePointsList = std::vector<uint32_t> {};
	auto private_StitchBuffer              = std::vector<fPOINTATTR> {};
	auto private_StringTable               = std::vector<std::wstring> {};
	auto private_TempPolygon               = std::vector<fPOINT> {};
	auto private_TempTexturePoints         = std::vector<TXPNT> {};
	auto private_TexturePointsBuffer       = std::vector<TXPNT> {};
	auto private_ThrName                   = fs::path {};
	auto private_Thumbnails                = std::vector<std::wstring> {};
	auto private_TracedEdges               = boost::dynamic_bitset<> {};
	auto private_TracedMap                 = boost::dynamic_bitset<> {};
	auto private_UndoBuffer                = std::vector<std::unique_ptr<uint32_t[]>> {};
	auto private_UserBMPFileName           = fs::path {};
	auto private_UserColorWin              = std::vector<HWND> {};
	auto private_ValueWindow               = std::vector<HWND> {};
	auto private_VersionNames              = std::vector<fs::path> {};
	auto private_WorkingFileName           = fs::path {};
	auto private_textureInputBuffer        = std::wstring {};

	private_DefaultColorWin.resize(16U);
	private_FormControlPoints.resize(9U);
	private_LabelWindow.resize(LASTLIN);
	private_RubberBandLine.resize(3U);
	private_SelectedFormsLine.resize(9U);
	private_SelectedPointsLine.resize(9U);
	private_StringTable.resize(STR_LEN);
	private_UndoBuffer.resize(16U);
	private_UserColorWin.resize(16U);
	private_ValueWindow.resize(LASTLIN);
	private_PreviousNames.reserve(OLDNUM);
	private_VersionNames.reserve(OLDVER);
	for (auto iVersion = 0U; iVersion < OLDNUM; iVersion++) {
	  private_PreviousNames.emplace_back(L"");
	}
	for (auto iVersion = 0; iVersion < OLDVER; iVersion++) {
	  private_VersionNames.emplace_back(L"");
	}

	AngledFormVertices        = &private_AngledFormVertices;
	AuxName                   = &private_AuxName;
	BSequence                 = &private_BSequence;
	BalaradName0              = &private_BalaradName0;
	BalaradName1              = &private_BalaradName1;
	BalaradName2              = &private_BalaradName2;
	ButtonWin                 = &private_ButtonWin;
	ClipBuffer                = &private_ClipBuffer;
	ClipPoints                = &private_ClipPoints;
	ColorFileName             = &private_ColorFileName;
	DefaultBMPDirectory       = &private_DefaultBMPDirectory;
	DefaultColorWin           = &private_DefaultColorWin;
	DefaultDirectory          = &private_DefaultDirectory;
	DesignerName              = &private_DesignerName;
	ExtendedHeader            = &private_ExtendedHeader;
	FormAngles                = &private_FormAngles;
	FormControlPoints         = &private_FormControlPoints;
	FormLines                 = &private_FormLines;
	FormList                  = &private_FormList;
	FormOnOff                 = &private_FormOnOff;
	FormVertices              = &private_FormVertices;
	FormVerticesAsLine        = &private_FormVerticesAsLine;
	GeName                    = &private_GeName;
	HomeDirectory             = &private_HomeDirectory;
	IniFileName               = &private_IniFileName;
	InsidePointList           = &private_InsidePointList;
	InterleaveSequence        = &private_InterleaveSequence;
	InterleaveSequenceIndices = &private_InterleaveSequenceIndices;
	LabelWindow               = &private_LabelWindow;
	MenuInfo                  = &private_MenuInfo;
	OSequence                 = &private_OSequence;
	OutsidePointList          = &private_OutsidePointList;
	PreviousNames             = &private_PreviousNames;
	RGBFileName               = &private_RGBFileName;
	RubberBandLine            = &private_RubberBandLine;
	SatinGuides               = &private_SatinGuides;
	SearchLine                = &private_SearchLine;
	SearchName                = &private_SearchName;
	SelectedFormList          = &private_SelectedFormList;
	SelectedFormsLine         = &private_SelectedFormsLine;
	SelectedPointsLine        = &private_SelectedPointsLine;
	StitchBuffer              = &private_StitchBuffer;
	StringTable               = &private_StringTable;
	TempPolygon               = &private_TempPolygon;
	TextureInputBuffer        = &private_textureInputBuffer;
	TexturePointsBuffer       = &private_TexturePointsBuffer;
	ThrName                   = &private_ThrName;
	Thumbnails                = &private_Thumbnails;
	TracedEdges               = &private_TracedEdges;
	TracedMap                 = &private_TracedMap;
	UndoBuffer                = &private_UndoBuffer;
	UserBMPFileName           = &private_UserBMPFileName;
	UserColorWin              = &private_UserColorWin;
	ValueWindow               = &private_ValueWindow;
	VersionNames              = &private_VersionNames;
	WorkingFileName           = &private_WorkingFileName;

	texture::initTextures(&private_TempTexturePoints, &private_SelectedTexturePointsList);

	thi::redini();

	CreateParams createParams {};
	createParams.bEnableNonClientDpiScaling = TRUE;

	auto private_ScrollSize   = SCROLSIZ;
	ScrollSize                = &private_ScrollSize;
	auto private_ColorBarSize = COLSIZ;
	ColorBarSize              = &private_ColorBarSize;

	if (IniFile.initialWindowCoords.right != 0) {
	  ThrEdWindow = CreateWindow(L"thred", // NOLINT
	                             L"",
	                             WS_OVERLAPPEDWINDOW,
	                             IniFile.initialWindowCoords.left,
	                             IniFile.initialWindowCoords.right,
	                             IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left,
	                             IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top,
	                             nullptr,
	                             nullptr,
	                             hInstance,
	                             &createParams);
	}
	else {
	  ThrEdWindow = CreateWindow(L"thred", // NOLINT
	                             L"",
	                             WS_OVERLAPPEDWINDOW,
	                             CW_USEDEFAULT,
	                             CW_USEDEFAULT,
	                             CW_USEDEFAULT,
	                             CW_USEDEFAULT,
	                             nullptr,
	                             nullptr,
	                             hInstance,
	                             &createParams);
	  GetClientRect(ThrEdWindow, &ThredWindowRect);
	  IniFile.initialWindowCoords = ThredWindowRect;
	}

	// Adjust the scroll width for the screen DPI now that we have a window handle
	auto private_DPI     = GetDpiForWindow(ThrEdWindow);
	screenDPI            = &private_DPI;
	private_ScrollSize   = MulDiv(private_ScrollSize, *screenDPI, 96);
	private_ColorBarSize = MulDiv(private_ColorBarSize, *screenDPI, 96);
	thi::init();
	if (UserFlagMap.test(UserFlag::SAVMAX)) {
	  ShowWindow(ThrEdWindow, SW_SHOWMAXIMIZED);
	}
	else {
	  ShowWindow(ThrEdWindow, SW_SHOW);
	}
	if (DesignerName->empty()) {
	  wchar_t designerBuffer[50];
	  LoadString(ThrEdInstance,
	             IDS_UNAM,
	             static_cast<LPTSTR>(designerBuffer),
	             sizeof(designerBuffer) / sizeof(designerBuffer[0]));
	  DesignerName->assign(static_cast<wchar_t const*>(designerBuffer));
	  thi::getdes();
	}

	auto xyRatio        = 1.0F; // expand form aspect ratio
	auto rotationAngle  = 0.0F;
	auto rotationCenter = fPOINT {};
	auto textureForm    = FRMHED {};

	auto stretchBoxLine = std::vector<POINT> {};
	stretchBoxLine.resize(5U); // stretch and expand

	while (GetMessage(&Msg, nullptr, 0, 0)) {
	  StateMap.set(StateFlag::SAVACT);
	  if (!thi::chkMsg(stretchBoxLine, xyRatio, rotationAngle, rotationCenter, textureForm)) {
		DispatchMessage(&Msg);
	  }
	  if (StateMap.testAndReset(StateFlag::FCHK)) {
		repair::frmchkx();
	  }
	  if (StateMap.testAndReset(StateFlag::RESTCH)) {
		thred::redraw(MainStitchWin);
	  }
	  if (StateMap.testAndReset(StateFlag::RELAYR)) {
		thi::ritlayr();
	  }
	  if (!StateMap.test(StateFlag::TXTRED)) {
		thi::sachk();
	  }
	  if (StateMap.testAndReset(StateFlag::DUMEN)) {
		DrawMenuBar(ThrEdWindow);
	  }
	}
#ifdef ALLOCFAILURE
	_set_new_handler(old_handler);
#endif
	return 0;
  }
#ifdef ALLOCFAILURE
  _set_new_handler(old_handler);
#endif

#if HIGHDPI
  // Restore the current thread's DPI awareness context
  SetThreadDpiAwarenessContext(previousDpiContext);
#endif

  LocalFree(ArgList);

  return -1;
}
#pragma warning(pop)
