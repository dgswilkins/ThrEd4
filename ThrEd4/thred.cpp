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
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <boost/dynamic_bitset.hpp>
#pragma warning(disable : 4127) // supress warning for fmt library header
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

namespace fs = std::experimental::filesystem;

namespace thi = thred::internal;

// select box
#define NERCNT 4 // number of entries in the near array

// main variables
int             ArgCount;                 // command line argument count
RECT            ThredWindowRect;          // main window size
RECT            ColorBarRect;             // color bar rectangle
RECT            MinLenRect;               // minimum length rectangle
RECT            MaxLenRect;               // maximum length rectangle
unsigned        SmallestStitchIndex;      // pointer to the smallest stitch in the selected range
unsigned        LargestStitchIndex;       // pointer to the largest stitch in the selected range
unsigned        CurrentStitchIndex;       // pointer to the current selection for length search
HDC             ThredDC;                  // main device context handle
HDC             ColorBarDC;               // color bar device context
HBITMAP         StitchWindowBmp;          // bitmap for the memory stitch device context
SIZE            TextSize;                 // used for measuring sizes of text items
SIZE            ScreenSizePixels;         // screen size in pixels
SIZE            ScreenSizeMM;             // screen size in millimeters
RECT            StitchWindowAbsRect;      // stitch window size,absolute
POINT           NearestPixel[NERCNT];     // selected points
POINT           BoxCoordinate = { 0, 0 }; // single select box point
double          DistanceToClick;          // distance of closest point to a mouse click
unsigned        ClosestPointIndexClone;   // copy of index of closest point
unsigned        PrevGroupStartStitch;     // lower end of previous selection
unsigned        PrevGroupEndStitch;       // higher end of previous selection
size_t          BufferDigitCount;         // number of decimal digits in the number of stitches
unsigned        LineIndex;                // line index for display routine
double          StitchWindowAspectRatio;  // aspect ratio of the stitch window
FORMSCLIP*      ClipFormsHeader;          // multiple form clipboard header
FORMVERTEXCLIP* ClipFormVerticesData;     // form points clipboard header
void*           ThrEdClipPointer;         // for memory allocation for thred format clipboard data
POINT           ClipOrigin;               // origin of clipboard box in stitch coordinates
SIZE            SelectBoxSize;            // size of the select box
POINT           SelectBoxOffset;          // offset of the spot the user selected from the lower left of the select box
double          RotationHandleAngle;      // angle of the rotation handle
unsigned        BitmapColor  = BITCOL;    // bitmap color
double          ThreadSize30 = TSIZ30;    //#30 thread size
double          ThreadSize40 = TSIZ40;    //#40 thread size
double          ThreadSize60 = TSIZ60;    //#40 thread size
unsigned        RunPoint;                 // point for animating stitchout
unsigned        StitchesPerFrame;         // number of stitches to draw in each frame
int             MovieTimeStep;            // time delay for stitchout

// WARNING the size of the following array must be changed if the maximum movie speed is changed
POINT MovieLine[100]; // line for movie stitch draw

unsigned LRUMenuId[] = { FM_ONAM0, FM_ONAM1, FM_ONAM2, FM_ONAM3 }; // recently used file menu ID's

unsigned      UndoBufferWriteIndex = 0;      // undo storage pointer
unsigned      UndoBufferReadIndex  = 0;      // undo retrieval pointers
unsigned      LastKeyCode          = 0xffff; // last key code
fs::path      VersionNames[OLDVER];          // temporary storage for old file version names
char          FileVersionIndex;              // points to old version to be read
unsigned      LayerIndex;                    // active layer code
unsigned      ClipFormsCount;                // number of forms the on the clipboard
POINT         StitchArrow[3];                // arrow for selected stitch
RANGE         SelectedRange;                 // first and last stitch for min/max stitch select
unsigned      NameOrder[50];                 // designer name order table
unsigned char NameEncoder[128];              // designer name encoding
unsigned char NameDecoder[256];              // designer name decode
HWND          FirstWin;                      // first window not destroyed for exiting enumerate loop
FRMRANGE      SelectedFormsRange;            // range of selected forms
size_t        TmpFormIndex;                  // saved form index
double        ZoomMin;                       // minimum allowed zoom value
fRECTANGLE    CheckHoopRect;                 // for checking the hoop size
fPOINT        BalaradOffset;                 // balarad offset
unsigned      SideWindowLocation;            // side message window location
POINT         SideWindowSize;                // size of the side message window
std::wstring* SideWindowsStrings;            // string array displayed in sidmsg
dPOINT        CellSize;                      // size of an stitchMap cell for drawing stitch boxes
unsigned      DraggedColor;                  // color being dragged
POINT*        FormVerticesAsLine;            // form vertex clipboard paste into form line
size_t        LastFormSelected;              // end point of selected range of forms

std::vector<std::unique_ptr<unsigned[]>>* UndoBuffer; // backup data

#if PESACT
unsigned char* PEScolors;             // pes colors
unsigned char* PESdata;               // pes card data buffer
fPOINT         PESstitchCenterOffset; // offset for writing pes files
PESTCH*        PESstitches;           // pes stitch buffer
unsigned char  PESequivColors[16];    // pes equivalent colors
unsigned char* PESstitch;             // pes stitches
unsigned       PEScolorIndex;         // pes color index
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

unsigned BackgroundPenWidth; // width of the background pen

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
unsigned ThreadSizePixels[16]; // thread sizes in pixels
unsigned ThreadSizeIndex[16];  // thread size indices

HMENU FileMenu;               // file submenu
HMENU BorderFillMenu;         // border fill submenu
HMENU ViewMenu;               // view submenu
HMENU ViewSetMenu;            // view/set
HMENU EditMenu;               // edit submenu
HWND  ChangeThreadSizeWin[3]; // thread size change windows
HWND  ColorBar;               // color bar
HWND  SpeedScrollBar;         // speed scroll bar for movie
HWND  BackupViewer[OLDVER];   // handles of multiple file viewing windows

COLORREF        BackgroundColor; // stitch window background
COLORREF        BoxColor[] = { 0x404040, 0x408040, 0x804040, 0x404080 };
int             ThreadWidthPixels[3];                  // thread sizes in pixels
std::bitset<32> DisplayedColorBitmap(0);               // Map of color numbers in design that are actually displayed
double          GapToNearest[NERCNT];                  // distances of the closest points
                                                       // to a mouse click
long       NearestPoint[NERCNT];                       // indices of the closest points
POINT      SearchLine[MAXITEMS];                       // stitch select line
fPOINT     StitchRangeSize;                            // form check ranges
unsigned   MoveAnchor;                                 // for resequencing stitches
double     RotateAngle;                                // angle for pixel rotate
SIZE       PickColorMsgSize;                           // size of the pick color message
POINT      InsertSize;                                 // size of file insert window
fPOINT     InsertCenter;                               // center point in inserted file
unsigned   NumericCode;                                // keyboard numerical input
double     LowestAngle;                                // low angle for angle from mark
double     OriginalAngle;                              // original angle for angle from mark
double     HighestAngle;                               // hi angle for angle from mark
unsigned   Knots[MAXKNOTS];                            // pointers to knots
unsigned   KnotCount;                                  // number of knots in the design
unsigned   KnotAttribute;                              // knot stitch attribute
fPOINT     KnotStep;                                   // knot stepSize
char       KnotAtStartOrder[] = { 2, 3, 1, 4, 0 };     // knot spacings
char       KnotAtEndOrder[]   = { -2, -3, -1, -4, 0 }; // reverse knot spacings
char       KnotAtLastOrder[]  = { 0, -4, -1, -3, -2 }; // reverse knot spacings
fRECTANGLE ClipRectAdjusted;                           // rectangle for adjust range ends for clipboard fills
HANDLE     BalaradFile;                                // balarad file handle

// graphics variables
double     AspectRatio = (LHUPX / LHUPY); // aspect ratio of the stitch window
SCROLLINFO ScrollInfo;                    // scroll bar i/o structure

COLORREF DefaultColors[] = { 0x000000, 0x800000, 0xFF0000, 0x808000, 0xFFFF00, 0x800080, 0xFF00FF, 0x000080,
	                         0x0000FF, 0x008000, 0x00FF00, 0x008080, 0x00FFFF, 0x808080, 0xC0C0C0, 0xFFFFFF };

long BoxOffset[4];

unsigned VerticalIndex;      // vertical index of the color window, calculated from mouse click
unsigned ThreadSizeSelected; // thread selected for size change

#if PESACT
const TCHAR AllFilter[_MAX_PATH + 1] = "Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Brother (PES)\0*.pes\0Tajima (DST)\0*.dst\0";
#else
const wchar_t AllFilter[_MAX_PATH + 1] = L"Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Tajima (DST)\0*.dst\0";
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

const wchar_t BmpFilter[_MAX_PATH + 1]    = L"Microsoft (BMP)\0*.bmp\0";
wchar_t       CustomFilter[_MAX_PATH + 1] = L"Thredworks (THR)\0*.thr\0";
HANDLE        FileHandle                  = 0;
HANDLE        PCSFileHandle               = 0;
HANDLE        IniFileHandle               = 0;
HANDLE        InsertedFileHandle; // insert file handle
HANDLE        BitmapFileHandle;   // bitmap handle
size_t        FileSize;           // size of file
unsigned long BytesRead;          // bytes actually read from file
unsigned      ColorChanges;       // number of color changes

OPENFILENAME OpenFileName = {
	sizeof(OpenFileName), // lStructsize
	0,                    // hwndOwner
	0,                    // hInstance
	AllFilter,            // lpstrFilter
	CustomFilter,         // lpstrCustomFilter
	_MAX_PATH,            // nMaxCustFilter
	0,                    // nFilterIndex
	0,                    // lpstrFile
	_MAX_PATH,            // nMaxFile
	0,                    // lpstrFileTitle
	0,                    // nMaxFileTitle
	0,                    // lpstrInitialDir
	0,                    // lpstrTitle
	OFN_OVERWRITEPROMPT,  // Flags
	0,                    // nFileOffset
	0,                    // nFileExtension
	L"thr",               // lpstrDefExt
	0,                    // lCustData
	0,                    // lpfnHook
	0,                    // lpTemplateName
};

std::vector<fs::path>*     PreviousNames;
std::vector<std::wstring>* Thumbnails;                          // vector of thumbnail names
size_t                     ThumbnailsSelected[4];               // indexes of thumbnails selected for display
unsigned                   ThumbnailDisplayCount;               // number of thumbnail file names selected for display
size_t                     ThumbnailIndex;                      // index into the thumbnail filname table
wchar_t                    ThumbnailSearchString[32];           // storage for the thumnail search string
wchar_t                    InsertedFileName[_MAX_PATH] = { 0 }; // insert file name
size_t                     InsertedVertexIndex;                 // saved float pointer for inserting files
size_t                     InsertedFormIndex;                   // saved form pointer for inserting files
unsigned                   InsertedStitchCount;                 // saved stitch pointer for inserting files

OPENFILENAME OpenBitmapName = {
	sizeof(OpenBitmapName), // lStructsize
	0,                      // hwndOwner
	0,                      // hInstance
	BmpFilter,              // lpstrFilter
	CustomFilter,           // lpstrCustomFilter
	_MAX_PATH,              // nMaxCustFilter
	0,                      // nFilterIndex
	0,                      // lpstrFile
	_MAX_PATH,              // nMaxFile
	0,                      // lpstrFileTitle
	0,                      // nMaxFileTitle
	0,                      // lpstrInitialDir
	0,                      // lpstrTitle
	OFN_OVERWRITEPROMPT,    // Flags
	0,                      // nFileOffset
	0,                      // nFileExtension
	L"bmp",                 // lpstrDefExt
	0,                      // lCustData
	0,                      // lpfnHook
	0,                      // lpTemplateName
};

BITMAPFILEHEADER BitmapFileHeader;   // bitmap file header
BITMAPV4HEADER   BitmapFileHeaderV4; // bitmap version4 file header
BITMAPINFO       BitmapInfo;         // bitmap info
BITMAPINFOHEADER BitmapInfoHeader;   // bitmap info header

COLORREF DefaultUserColors[] = {
	0x00000000, 0x002dffff, 0x003f87e9, 0x000762f8, 0x000000ff, 0x002f03af, 0x007248b7, 0x00ff0080,
	0x00920166, 0x00a00000, 0x00ff2424, 0x006a4d15, 0x00f5800a, 0x004b7807, 0x00156a1e, 0x00dbe6e3,
};

COLORREF DefaultCustomColors[] = { 0x729674, 0x1a1eb9, 0x427347, 0xbfff,   0xd3c25f, 0xc3ced0, 0x4a8459, 0x8cacd0,
	                               0x81aeb6, 0x7243a5, 0xbdadda, 0x9976c5, 0x96d9f5, 0xe2ddd6, 0x245322, 0x7b60ae };

COLORREF DefaultCustomBackgroundColors[] = { 0xa3c5dc, 0xadc7b6, 0xd1fcfb, 0xdcd7c0, 0xebddcd, 0xc6b3b3, 0xdec9ce, 0xd2d1e9,
	                                         0xdfdffd, 0xbee6ef, 0x8fb8b1, 0x85c2e0, 0xabc1c9, 0xd3d3c7, 0x7c9c84, 0x9acddc };

COLORREF DefaultBitmapBackgroundColors[] = { 0xc0d5bf, 0xc8dfee, 0x708189, 0xa5a97a, 0xb8d6fe, 0x8a8371, 0x4b6cb8, 0x9cdcc2,
	                                         0x366d39, 0xdcfcfb, 0x3c4f75, 0x95b086, 0xc9dcba, 0x43377b, 0xb799ae, 0x54667a };

POINT MoveLine0[2];              // move point line
POINT MoveLine1[2];              // move point line
POINT ClipInsertBoxLine[5];      // for displaying clipboard insert rectangle
POINT RotateBoxOutline[5];       // for drawing the rotate rectangle
POINT RotateBoxCrossVertLine[2]; // vertical part of the rotate cross
POINT RotateBoxCrossHorzLine[2]; // horizontal part of the rotate cross
POINT RotateBoxToCursorLine[2];  // line from the cursor to the center of the rotate cross

COLCHNG ColorChangeTable[MAXCHNG];
STREX   ExtendedHeader; // thred file header extension

typedef struct _dstdat {
	char cor;
	char val;
} DSTDAT;

#define XCOR 0
#define YCOR 1

DSTDAT DSTValues[] = {
	{ XCOR, 1 }, { XCOR, -1 }, { XCOR, 9 },  { XCOR, -9 },  { YCOR, -9 },  { YCOR, 9 },  { YCOR, -1 }, { YCOR, 1 },
	{ XCOR, 3 }, { XCOR, -3 }, { XCOR, 27 }, { XCOR, -27 }, { YCOR, -27 }, { YCOR, 27 }, { YCOR, -3 }, { YCOR, 3 },
	{ XCOR, 0 }, { XCOR, 0 },  { XCOR, 81 }, { XCOR, -81 }, { YCOR, -81 }, { YCOR, 81 },
};

unsigned Xdst[] = {
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
	0x050505, // 121
};

unsigned Ydst[] = {
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
	0x21a0a0, // 121
};

#if PESACT

unsigned PESColorTranslate[] = {
	0xf0f0f0, // 00
	0x940a1a, // 01
	0xff750f, // 02
	0x4c9300, // 03
	0xfebdba, // 04
	0x0000ec, // 05
	0x5a99e4, // 06
	0xab48cc, // 07
	0xfac4fd, // 08
	0xcd84dd, // 09
	0x8ad36b, // 10
	0x45a9e4, // 11
	0x42bdff, // 12
	0x00e6ff, // 13
	0x00d96c, // 14
	0x41a9c1, // 15
	0x97adb5, // 16
	0x5f9cba, // 17
	0x9ef5fa, // 18
	0x808080, // 19
	0x000000, // 20
	0xdf1c00, // 21
	0xb800df, // 22
	0x626262, // 23
	0x0d2669, // 24
	0x6000ff, // 25
	0x0082bf, // 26
	0x7891f3, // 27
	0x0568ff, // 28
	0xf0f0f0, // 29
	0xcd32c8, // 30
	0x9bbfb0, // 31
	0xebbf65, // 32
	0x04baff, // 33
	0x6cf0ff, // 34
	0x15cafe, // 35
	0x0181f3, // 36
	0x23a937, // 37
	0x5f4623, // 38
	0x95a6a6, // 39
	0xa6bfce, // 40
	0x02aa96, // 41
	0xc6e3ff, // 42
	0xd799ff, // 43
	0x047000, // 44
	0xfbcced, // 45
	0xd889c0, // 46
	0xb4d9e7, // 47
	0x860ee9, // 48
	0x2968cf, // 49
	0x158640, // 50
	0x9717db, // 51
	0x04a7ff, // 52
	0xffffb9, // 53
	0x278922, // 54
	0xcd12b6, // 55
	0x00aa00, // 56
	0xdca9fe, // 57
	0x10d5fe, // 58
	0xdf9700, // 59
	0x84ffff, // 60
	0x74e7cf, // 61
	0x42bdff, // 62
	0xb4d9e7  // 63
};
#endif

unsigned FillTypes[] = // fill type array for side window display
    { 0, VRTF, HORF, ANGF, SATF, CLPF, CONTF, VCLPF, HCLPF, ANGCLPF, FTHF, TXVRTF, TXHORF, TXANGF };
unsigned EdgeFillTypes[] = // edge fill type array for side window display
    { 0,        EDGELINE,  EDGEBEAN,   EDGECLIP,   EDGEANGSAT, EDGEAPPL, EDGEPROPSAT,
	  EDGEBHOL, EDGEPICOT, EDGEDOUBLE, EDGELCHAIN, EDGEOCHAIN, EDGECLIPX };
unsigned FeatherFillTypes[] = // feather fill types
    { FTHSIN, FTHSIN2, FTHLIN, FTHPSG, FTHRMP, FTHFAZ };

BOOL CALLBACK thred::internal::dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	HWND hwnd = {};
	switch (umsg) {
	case WM_INITDIALOG:
		hwnd = GetDlgItem(hwndlg, IDC_DESED);
		SetWindowText(hwnd, DesignerName->c_str());
		SetFocus(hwnd);
		SendMessage(hwnd, EM_SETSEL, 0, -1);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDCANCEL:
			EndDialog(hwndlg, 0);
			return TRUE;
		case IDOK:
			hwnd = GetDlgItem(hwndlg, IDC_DESED);
			wchar_t designerBuffer[50];
			GetWindowText(hwnd, designerBuffer, sizeof(designerBuffer) / sizeof(designerBuffer[0]));
			DesignerName->assign(designerBuffer);
			EndDialog(hwndlg, 0);
			SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRED], *DesignerName).c_str());
			return TRUE;
		}
	}
	return 0;
}

void thred::internal::getdes() noexcept {
	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DESNAM), ThrEdWindow, (DLGPROC)dnamproc);
}

bool thred::internal::isfclp() noexcept {
	if (clip::isclp(ClosestFormToCursor) && FormList[ClosestFormToCursor].fillType != CLPF)
		return 1;
	return 0;
}

double thred::internal::stlen(unsigned iStitch) noexcept {
	return hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
}

void thred::undat() noexcept {
	if (FormDataSheet) {
		DestroyWindow(FormDataSheet);
		FormDataSheet = 0;
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
	unsigned code = MF_CHECKED;

	if (UserFlagMap.test(UserFlag::WRNOF))
		code = MF_UNCHECKED;
	CheckMenuItem(MainMenu, ID_WARNOF, code);
}

int datcod[] = { ID_CHKOF, ID_CHKON, ID_CHKREP, ID_CHKREPMSG };

void thred::chkmen() noexcept {
	int iCode = 0, code = MF_UNCHECKED;

	for (iCode = 0; iCode < (sizeof(datcod) / sizeof(datcod[0])); iCode++) {
		code = MF_UNCHECKED;
		if (iCode == IniFile.dataCheck)
			code = MF_CHECKED;
		CheckMenuItem(MainMenu, datcod[iCode], code);
	}
}

void thred::duzrat() noexcept {
	if (!ZoomRect.right)
		ZoomRect.right = LHUPX;
	if (!ZoomRect.top)
		ZoomRect.top = LHUPY;
	ZoomRatio.x = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	ZoomRatio.y = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
}

unsigned thred::internal::rsed() noexcept {
	SYSTEMTIME time = {};

	GetLocalTime(&time);
	return (time.wSecond << 16) | time.wMilliseconds;
}

void thred::internal::fnamtabs() {
	unsigned iName = 0, source = 0, destination = 0;

	for (iName = 0; iName < 50; iName++)
		NameOrder[iName] = iName;
	PseudoRandomValue = NORDSED;
	for (iName = 0; iName < 100; iName++) {
		source      = form::psg() % 50;
		destination = form::psg() % 50;
		std::swap(NameOrder[destination], NameOrder[source]);
	}
	for (iName = 0; iName < 128; iName++)
		NameEncoder[iName] = gsl::narrow<unsigned char>(iName) + NCODOF;
	PseudoRandomValue = NCODSED;
	for (iName = 0; iName < 256; iName++) {
		source      = form::psg() & 0x7f;
		destination = form::psg() & 0x7f;
		std::swap(NameEncoder[destination], NameEncoder[source]);
	}
	const unsigned char fillval = {};
	std::fill_n(NameDecoder, sizeof(NameDecoder), fillval);
	for (iName = 32; iName < 127; iName++)
		NameDecoder[NameEncoder[iName]] = gsl::narrow<unsigned char>(iName);
}

void thred::internal::ritfnam(const std::wstring& designerName) {
	auto          designer    = utf::Utf16ToUtf8(designerName);
	unsigned      iName       = 0;
	unsigned char tmpName[50] = { 0 };

	if (NameOrder[0] > 50)
		fnamtabs();
	PseudoRandomValue = rsed();
	for (iName = 0; iName < 50; iName++)
		tmpName[iName] = form::psg() & 0xff;
	for (iName = 0; iName < 50; iName++) {
		if (designer[iName]) {
			tmpName[iName] = NameEncoder[designer[iName]];
		}
		else {
			while (NameDecoder[tmpName[iName]])
				tmpName[iName] = form::psg() & 0xff;
			break;
		}
	}
	if (iName == 50) {
		while (NameDecoder[tmpName[49]])
			tmpName[49] = form::psg() & 0xff;
	}
	for (iName = 0; iName < 50; iName++) {
		if (NameOrder[iName] < 50) {
			ExtendedHeader.creatorName[NameOrder[iName]] = tmpName[iName];
		}
	}
}

void thred::internal::redfnam(std::wstring& designerName) {
	unsigned      iName       = 0;
	unsigned char tmpName[50] = { 0 };
	std::string   designer;

	for (iName = 0; iName < 50; iName++) {
		if (NameOrder[iName] < 50)
			tmpName[iName] = ExtendedHeader.creatorName[NameOrder[iName]];
		else
			tmpName[iName] = 111;
	}
	designer.reserve(50);
	for (iName = 0; iName < 50; iName++) {
		if (NameDecoder[tmpName[iName]]) {
			designer.push_back(NameDecoder[tmpName[iName]]);
		}
		else {
			break;
		}
	}
	auto decoded = utf::Utf8ToUtf16(designer);
	designerName.assign(decoded);
}

void thred::internal::dstin(unsigned number, POINT& pout) noexcept {
	// ToDo - what is this code doing?
	unsigned index = 0, shift = 1;
	pout = {};
	for (index = 0; index < 22; index++) {
		if (number & shift) {
			if (DSTValues[index].cor)
				pout.y += DSTValues[index].val;
			else
				pout.x += DSTValues[index].val;
		}
		shift <<= 1;
	}
}

fPOINT* thred::adflt(size_t count) {
	size_t iFormVertex = FormVertexIndex;

	if (FormVertexIndex + count > MAXITEMS)
		displayText::tabmsg(IDS_FRMOVR);
	FormVertexIndex += count;
	return &FormVertices[iFormVertex];
}

fPOINT* thred::adclp(size_t count) noexcept {
	size_t iClipPoint = ClipPointIndex;

	ClipPointIndex += count;
	return &ClipPoints[iClipPoint];
}

unsigned thred::duthrsh(double threshold) noexcept {
	unsigned iZoomLevel = 0;
	double   zoom       = 1;

	if (threshold > 0.0) {
		while (zoom > threshold) {
			zoom *= ZUMFCT;
			iZoomLevel++;
		}
	}
	else
		return 0;
	return iZoomLevel + 1;
}

double thred::internal::unthrsh(unsigned level) noexcept {
	double zoom = 1;

	if (level)
		level--;
	else
		return 0;
	while (level) {
		zoom *= ZUMFCT;
		level--;
	}
	return zoom;
}

void thred::ritfcor(const fPOINT& point) {
	displayText::butxt(HCOR, fmt::format(L"x{:.0f} y{:.0f}", (point.x / PFGRAN), (point.y / PFGRAN)));
}

void thred::internal::ritcor(const fPOINTATTR& pointAttribute) {
	const fPOINT point = { pointAttribute.x, pointAttribute.y };

	thred::ritfcor(point);
}

void thred::internal::fndknt() {
	unsigned iStitch = 0;
	bool     flag    = false;

	if (PCSHeader.stitchCount > 4) {
		// ToDo - Is flag initialized and used correctly?
		flag      = 0;
		KnotCount = 0;
		for (iStitch = 0; iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 4; iStitch++) {
			if (StitchBuffer[iStitch].attribute & KNOTMSK) {
				if (!flag)
					Knots[KnotCount++] = iStitch;
			}
			else
				flag = 0;
		}
	}
	else
		KnotCount = 0;
}

void thred::coltab() {
	unsigned    iStitch = 0, iColor = 0, nextColor = 0;
	unsigned    currentColor = 0;
	dRECTANGLE  range        = {};
	fPOINTATTR* Stitch       = nullptr;

	ColorChanges = 0;
	if (PCSHeader.stitchCount) {
		StitchBuffer[0].attribute &= NCOLMSK;
		StitchBuffer[0].attribute |= (StitchBuffer[1].attribute & COLMSK);
		StitchBuffer[PCSHeader.stitchCount - 1].attribute &= NCOLMSK;
		StitchBuffer[PCSHeader.stitchCount - 1].attribute |= (StitchBuffer[PCSHeader.stitchCount - 2].attribute & COLMSK);
		currentColor = StitchBuffer[0].attribute & COLMSK;
		for (iStitch = 1; iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & COLMSK) != currentColor) {
				if ((StitchBuffer[iStitch + 1].attribute & COLMSK) == currentColor) {
					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= currentColor;
				}
				currentColor = StitchBuffer[iStitch].attribute & COLMSK;
			}
		}
		iColor       = 0;
		currentColor = 0xffffffff;
		range.left   = -UnzoomedRect.x;
		range.right  = UnzoomedRect.x * 2;
		range.bottom = -UnzoomedRect.y;
		range.top    = UnzoomedRect.y * 2;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			Stitch = &StitchBuffer[iStitch];
			if (Stitch->x < range.left)
				Stitch->x = static_cast<float>(range.left);
			if (Stitch->x > range.right)
				Stitch->x = static_cast<float>(range.right);
			if (Stitch->y > range.top)
				Stitch->y = static_cast<float>(range.top);
			if (Stitch->y < range.bottom)
				Stitch->y = static_cast<float>(range.bottom);
			nextColor = Stitch->attribute & COLMSK;
			if (currentColor != nextColor) {
				ColorChangeTable[iColor].colorIndex    = gsl::narrow<unsigned short>(nextColor);
				ColorChangeTable[iColor++].stitchIndex = gsl::narrow<unsigned short>(iStitch);
				currentColor                           = nextColor;
			}
		}
		ColorChanges                         = iColor;
		ColorChangeTable[iColor].stitchIndex = gsl::narrow<unsigned short>(iStitch);
		if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		thi::fndknt();
	}
}

void thred::internal::ladj() {
	unsigned iLayer = 0;

	for (iLayer = 0; iLayer < 5; iLayer++) {
		if (iLayer == ActiveLayer)
			EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_GRAYED);
		else
			EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_ENABLED);
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
	BAKHED* backupData = nullptr;
	auto&   undoBuffer = *UndoBuffer;

	undoBuffer[UndoBufferWriteIndex].reset(nullptr);
	const auto size = sizeof(BAKHED) + sizeof(FormList[0]) * FormIndex + sizeof(StitchBuffer[0]) * PCSHeader.stitchCount
	                  + sizeof(FormVertices[0]) * FormVertexIndex + sizeof(ClipPoints[0]) * ClipPointIndex
	                  + sizeof(SatinGuides[0]) * satin::getGuideSize() + sizeof(UserColor)
	                  + sizeof((*TexturePointsBuffer)[0]) * TextureIndex;
	undoBuffer[UndoBufferWriteIndex] = std::make_unique<unsigned[]>(size);
	backupData                       = convert_ptr<BAKHED*>(undoBuffer[UndoBufferWriteIndex].get());
	if (backupData) {
		backupData->zoomRect  = UnzoomedRect;
		backupData->formCount = FormIndex;
		backupData->forms     = convert_ptr<FRMHED*>(&backupData[1]);
		std::copy(FormList, FormList + FormIndex, stdext::make_checked_array_iterator(backupData->forms, FormIndex));
		backupData->stitchCount = PCSHeader.stitchCount;
		backupData->stitches    = convert_ptr<fPOINTATTR*>(&backupData->forms[FormIndex]);
		if (PCSHeader.stitchCount) {
			std::copy(StitchBuffer,
			          StitchBuffer + PCSHeader.stitchCount,
			          stdext::make_checked_array_iterator(backupData->stitches, PCSHeader.stitchCount));
		}
		backupData->vertexCount = FormVertexIndex;
		backupData->vertices    = convert_ptr<fPOINT*>(&backupData->stitches[PCSHeader.stitchCount]);
		if (FormVertexIndex) {
			std::copy(FormVertices,
			          FormVertices + FormVertexIndex,
			          stdext::make_checked_array_iterator(backupData->vertices, backupData->vertexCount));
		}
		backupData->guideCount = satin::getGuideSize();
		backupData->guide      = convert_ptr<SATCON*>(&backupData->vertices[FormVertexIndex]);
		if (satin::getGuideSize()) {
			std::copy(SatinGuides,
			          SatinGuides + satin::getGuideSize(),
			          stdext::make_checked_array_iterator(backupData->guide, backupData->guideCount));
		}
		backupData->clipPointCount = ClipPointIndex;
		backupData->clipPoints     = convert_ptr<fPOINT*>(&backupData->guide[satin::getGuideSize()]);
		if (ClipPointIndex) {
			if (ClipPointIndex > MAXITEMS)
				ClipPointIndex = MAXITEMS;
			std::copy(ClipPoints,
			          ClipPoints + ClipPointIndex,
			          stdext::make_checked_array_iterator(backupData->clipPoints, backupData->clipPointCount));
		}
		backupData->colors = convert_ptr<COLORREF*>(&backupData->clipPoints[ClipPointIndex]);
		auto sizeColors    = (sizeof(UserColor) / sizeof(UserColor[0]));
		std::copy(UserColor, UserColor + sizeColors, stdext::make_checked_array_iterator(backupData->colors, sizeColors));
		backupData->texturePoints     = convert_ptr<TXPNT*>(&backupData->colors[16]);
		backupData->texturePointCount = TextureIndex;
		if (TextureIndex) {
			std::copy(TexturePointsBuffer->cbegin(),
			          TexturePointsBuffer->cend(),
			          stdext::make_checked_array_iterator(backupData->texturePoints, backupData->texturePointCount));
		}
	}
}

void thred::savdo() {
	StateMap.set(StateFlag::WASDO);
	StateMap.set(StateFlag::CMPDO);
	if (StateMap.testAndReset(StateFlag::SAVACT)) {
		if (StateMap.testAndReset(StateFlag::BAKING)) {
			StateMap.reset(StateFlag::REDUSHO);
			EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
		}
		StateMap.set(StateFlag::BAKACT);
		EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
		StateMap.set(StateFlag::DUMEN);
		thi::dudat();
		UndoBufferWriteIndex++;
		UndoBufferWriteIndex &= 0xf;
		if (!UndoBufferWriteIndex)
			StateMap.set(StateFlag::BAKWRAP);
	}
}

void thred::internal::redfils() {
	WIN32_FIND_DATA findData   = {};
	HANDLE          fileHandle = nullptr;

	for (auto iLRU = 0; iLRU < OLDNUM; iLRU++) {
		if (GetMenuState(FileMenu, LRUMenuId[iLRU], MF_BYCOMMAND) != -1)
			DeleteMenu(FileMenu, LRUMenuId[iLRU], MF_BYCOMMAND);
	}
	auto& previousNames = *PreviousNames;
	for (auto iLRU = 0; iLRU < OLDNUM; iLRU++) {
		if (!previousNames[iLRU].empty()) {
			if (StateMap.test(StateFlag::SAVAS))
				AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], previousNames[iLRU].wstring().c_str());
			else {
				fileHandle = FindFirstFile(previousNames[iLRU].c_str(), &findData);
				if (fileHandle == INVALID_HANDLE_VALUE)
					previousNames[iLRU].clear();
				else {
					AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], previousNames[iLRU].wstring().c_str());
					FindClose(fileHandle);
				}
			}
		}
	}
	StateMap.set(StateFlag::DUMEN);
}

void thred::internal::nunams() {
	unsigned iPrevious = 0;

	*AuxName = *WorkingFileName;
	switch (IniFile.auxFileType) {
	case AUXDST:
		AuxName->replace_extension(L".dst");
		break;
#if PESACT
	case AUXPES:
		AuxName.replace_extension(L".pes");
		break;
#endif
	default:
		AuxName->replace_extension(L".pcs");
	}
	*ThrName = *WorkingFileName;
	ThrName->replace_extension(L".thr");
	*GeName = *WorkingFileName;
	GeName->replace_extension(L".th*");
	bool  flag          = true;
	auto& previousNames = *PreviousNames;
	for (iPrevious = 0; iPrevious < OLDNUM; iPrevious++) {
		if (previousNames[iPrevious] == *ThrName) {
			if (iPrevious) {
				std::swap(previousNames[0], previousNames[iPrevious]);
				flag = false;
				break;
			}
			else {
				return;
			}
		}
	}
	if (flag) {
		for (iPrevious = 0; iPrevious < OLDNUM; iPrevious++) {
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
	if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
		ClosestPointIndex = PCSHeader.stitchCount - 1;
	if (GroupStitchIndex > PCSHeader.stitchCount)
		GroupStitchIndex = ClosestPointIndex;
	if (GroupStitchIndex > ClosestPointIndex) {
		GroupStartStitch = ClosestPointIndex;
		GroupEndStitch   = GroupStitchIndex;
	}
	else {
		GroupStartStitch = GroupStitchIndex;
		GroupEndStitch   = ClosestPointIndex;
	}
}

void thred::mvstch(size_t destination, size_t source) noexcept {
	StitchBuffer[destination] = StitchBuffer[source];
}

void thred::internal::box(unsigned iNearest, HDC dc) noexcept {
	const long boxWidth = BoxOffset[iNearest];
	POINT      line[5]  = {};

	line[0].x = NearestPixel[iNearest].x - boxWidth;
	line[0].y = NearestPixel[iNearest].y - boxWidth;
	line[1].x = NearestPixel[iNearest].x + boxWidth;
	line[1].y = NearestPixel[iNearest].y - boxWidth;
	line[2].x = NearestPixel[iNearest].x + boxWidth;
	line[2].y = NearestPixel[iNearest].y + boxWidth;
	line[3].x = NearestPixel[iNearest].x - boxWidth;
	line[3].y = NearestPixel[iNearest].y + boxWidth;
	line[4].x = NearestPixel[iNearest].x - boxWidth;
	line[4].y = NearestPixel[iNearest].y - boxWidth;
	Polyline(dc, line, 5);
}

void thred::internal::boxs() noexcept {
	unsigned iNear = 0;

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	for (iNear = 0; iNear < NearestCount; iNear++) {
		SelectObject(StitchWindowDC, BoxPen[iNear]);
		box(iNear, StitchWindowDC);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unboxs() noexcept {
	if (NearestCount) {
		boxs();
		NearestCount = 0;
	}
}

void thred::internal::stchPars() {
	AspectRatio = static_cast<double>(UnzoomedRect.x) / static_cast<double>(UnzoomedRect.y);
	if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT))
		StitchWindowSize.x = gsl::narrow<long>(round((ThredWindowRect.bottom - (SCROLSIZ << 1)) * AspectRatio));
	else
		StitchWindowSize.x = gsl::narrow<long>(round((ThredWindowRect.bottom - SCROLSIZ) * AspectRatio));

	if ((StitchWindowSize.x + gsl::narrow<long>(ButtonWidthX3) + RIGHTSIZ) < ThredWindowRect.right) {
		if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT))
			StitchWindowSize.y = ThredWindowRect.bottom - (SCROLSIZ << 1);
		else
			StitchWindowSize.y = ThredWindowRect.bottom - SCROLSIZ;
	}
	else {
		StitchWindowSize.x = (ThredWindowRect.right - ButtonWidthX3 - COLSIZ);
		StitchWindowSize.y = ThredWindowRect.bottom - ThredWindowRect.top;
		if (static_cast<double>(StitchWindowSize.x) / static_cast<double>(StitchWindowSize.y) > AspectRatio)
			StitchWindowSize.x = StitchWindowSize.y * AspectRatio;
		else
			StitchWindowSize.y = StitchWindowSize.x / AspectRatio;
	}
}

void thred::redraw(HWND window) noexcept {
	unsigned iWindow = 0;

	RedrawWindow(window, NULL, NULL, RDW_INVALIDATE);
	if (window == MainStitchWin) {
		for (iWindow = 0; iWindow < 16; iWindow++) {
			if (DefaultColorWin[iWindow])
				RedrawWindow(DefaultColorWin[iWindow], NULL, NULL, RDW_INVALIDATE);
		}
		RedrawWindow(ColorBar, NULL, NULL, RDW_INVALIDATE);
	}
}

void thred::internal::nuRct() {
	GetClientRect(ThrEdWindow, &ThredWindowRect);
	GetWindowRect(ColorBar, &ColorBarRect);
	if (ButtonWin->size() != 0) {
		GetWindowRect((*ButtonWin)[HMINLEN], &MinLenRect);
		GetWindowRect((*ButtonWin)[HMAXLEN], &MaxLenRect);
	}
	ReleaseDC(ColorBar, ColorBarDC);
	ColorBarDC = GetDC(ColorBar);
	DeleteDC(StitchWindowMemDC);
	ReleaseDC(MainStitchWin, StitchWindowDC);
	DeleteObject(StitchWindowBmp);
	ReleaseDC(MainStitchWin, StitchWindowDC);
	StitchWindowDC = GetDCEx(MainStitchWin, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	DeleteDC(StitchWindowMemDC);
	StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
	GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
	ReleaseDC(ThrEdWindow, ThredDC);
	ThredDC = GetDC(ThrEdWindow);
	SetStretchBltMode(ThredDC, COLORONCOLOR);
	GetDCOrgEx(ThredDC, &ThredWindowOrigin);
	GetWindowRect(MainStitchWin, &StitchWindowAbsRect);
	GetClientRect(MainStitchWin, &StitchWindowClientRect);
	StitchWindowBmp = CreateCompatibleBitmap(StitchWindowDC, StitchWindowClientRect.right, StitchWindowClientRect.bottom);
	SelectObject(StitchWindowMemDC, StitchWindowBmp);
}

void thred::movStch() {
	POINT clientSize     = { (ThredWindowRect.right - gsl::narrow<LONG>(ButtonWidthX3) - RIGHTSIZ), (ThredWindowRect.bottom) };
	long  verticalOffset = 0;
	long  actualWindowHeight = StitchWindowSize.y;

	thi::unboxs();
	if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
		verticalOffset = SCROLSIZ;
		clientSize.y -= SCROLSIZ;
		actualWindowHeight -= SCROLSIZ;
	}
	if (StateMap.test(StateFlag::ZUMED)) {
		clientSize.y -= SCROLSIZ;
		MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, clientSize.x, clientSize.y, FALSE);
		MoveWindow(VerticalScrollBar, ButtonWidthX3 + clientSize.x, 0, SCROLSIZ, clientSize.y, TRUE);
		MoveWindow(HorizontalScrollBar, ButtonWidthX3, clientSize.y + verticalOffset, clientSize.x, SCROLSIZ, TRUE);
		StitchWindowAspectRatio = static_cast<double>(clientSize.x) / clientSize.y;
		if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT))
			MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, clientSize.x, SCROLSIZ, TRUE);
		ShowWindow(VerticalScrollBar, TRUE);
		ShowWindow(HorizontalScrollBar, TRUE);
	}
	else {
		thi::stchPars();
		actualWindowHeight = StitchWindowSize.y + SCROLSIZ;
		MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, StitchWindowSize.x, actualWindowHeight, TRUE);
		ShowWindow(VerticalScrollBar, FALSE);
		ShowWindow(HorizontalScrollBar, FALSE);
		StitchWindowAspectRatio = static_cast<double>(StitchWindowSize.x) / actualWindowHeight;
		if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT))
			MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, StitchWindowSize.x, SCROLSIZ, TRUE);
	}
	MoveWindow(ColorBar, ThredWindowRect.right - COLSIZ, 0, COLSIZ, ThredWindowRect.bottom, TRUE);
	thi::nuRct();
	thred::redraw(ColorBar);
}

void thred::unbsho() {
	unsigned iBackup = 0;

	if (StateMap.testAndReset(StateFlag::BAKSHO)) {
		for (iBackup = 0; iBackup < OLDVER; iBackup++)
			DestroyWindow(BackupViewer[iBackup]);
	}
}

void thred::unsid() noexcept {
	FormMenuChoice = 0;
	if (SideMessageWindow) {
		DestroyWindow(SideMessageWindow);
		SideMessageWindow = 0;
	}
}

void thred::internal::insadj() {
	StateMap.reset(StateFlag::PRELIN);
	form::fvars(ClosestFormToCursor);
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

double thred::internal::pxchk(double pixelSize) noexcept {
	if (pixelSize < 0.2)
		return 1;
	if (pixelSize > 20)
		return 20;
	return pixelSize;
}

void thred::internal::sizstch(fRECTANGLE& rectangle, const fPOINTATTR* const stitches) noexcept {
	unsigned iStitch = 0;

	if (PCSHeader.stitchCount && stitches) {
		rectangle.bottom = rectangle.top = stitches[0].y;
		rectangle.left = rectangle.right = stitches[0].x;
		for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (stitches[iStitch].x < rectangle.left)
				rectangle.left = stitches[iStitch].x;
			if (stitches[iStitch].x > rectangle.right)
				rectangle.right = stitches[iStitch].x;
			if (stitches[iStitch].y < rectangle.bottom)
				rectangle.bottom = stitches[iStitch].y;
			if (stitches[iStitch].y > rectangle.top)
				rectangle.top = stitches[iStitch].y;
		}
	}
	else
		rectangle = {};
}

void thred::internal::zRctAdj() noexcept {
	double adjustment = 0.0;

	if (ZoomRect.top > UnzoomedRect.y) {
		adjustment = ZoomRect.top - UnzoomedRect.y;
		ZoomRect.top -= adjustment;
		ZoomRect.bottom -= adjustment;
	}
	if (ZoomRect.bottom < 0) {
		ZoomRect.top -= ZoomRect.bottom;
		ZoomRect.bottom = 0;
	}
	if (ZoomRect.right > UnzoomedRect.x) {
		adjustment = ZoomRect.right - UnzoomedRect.x;
		ZoomRect.right -= adjustment;
		ZoomRect.left -= adjustment;
	}
	if (ZoomRect.left < 0) {
		ZoomRect.right -= ZoomRect.left;
		ZoomRect.left = 0;
	}
}

void thred::shft(const fPOINT& delta) noexcept {
	dPOINT halfZoomRect = { ((ZoomRect.right - ZoomRect.left) / 2), ((ZoomRect.top - ZoomRect.bottom) / 2) };
	dPOINT center       = { (ZoomRect.left + halfZoomRect.x), (ZoomRect.bottom + halfZoomRect.y) };
	dPOINT shift        = { (center.x - delta.x), (center.y - delta.y) };

	ZoomRect.bottom -= shift.y;
	ZoomRect.top -= shift.y;
	ZoomRect.left -= shift.x;
	ZoomRect.right -= shift.x;

	thi::zRctAdj();
}

void thred::internal::stch2px1(unsigned iStitch) noexcept {
	StitchCoordinatesPixels.x = (StitchBuffer[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (StitchBuffer[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
}

void thred::internal::shft2box() noexcept {
	SelectedPoint = StitchBuffer[ClosestPointIndex];
	thred::shft(SelectedPoint);
	stch2px1(ClosestPointIndex);
}

void thred::internal::unbox() {
	if (StateMap.testAndReset(StateFlag::SELBOX)) {
		SelectObject(StitchWindowDC, BoxPen[0]);
		SetROP2(StitchWindowDC, R2_NOTXORPEN);
		Polyline(StitchWindowDC, StitchArrow, 3);
		SetROP2(StitchWindowDC, R2_COPYPEN);
	}
}

void thred::internal::ilin() noexcept {
	SelectObject(StitchWindowDC, LinePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_XORPEN);
	Polyline(StitchWindowDC, &InsertLine[1], 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	SelectObject(StitchWindowMemDC, LinePen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	Polyline(StitchWindowMemDC, &InsertLine[1], 2);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::internal::xlin() {
	if (StateMap.testAndReset(StateFlag::ILIN))
		ilin();
}

void thred::internal::ilin1() noexcept {
	SelectObject(StitchWindowDC, LinePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::xlin1() {
	if (StateMap.testAndReset(StateFlag::ILIN1))
		ilin1();
}

void thred::internal::endpnt() {
	unbox();
	xlin();
	xlin1();
	StateMap.set(StateFlag::LIN1);
	StateMap.set(StateFlag::INSRT);
	StateMap.reset(StateFlag::GRPSEL);
	InsertLine[0]   = StitchCoordinatesPixels;
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
}

void thred::internal::duIns() {
	POINT tlin[2] = {};

	// ToDo - what is tlin being used for?
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	stch2px1(ClosestPointIndex);
	tlin[0] = InsertLine[0] = StitchCoordinatesPixels;
	stch2px1(ClosestPointIndex + 1);
	tlin[1] = InsertLine[2] = StitchCoordinatesPixels;
	xlin();
	StateMap.set(StateFlag::ILIN);
	ilin();
}

void thred::internal::movins() {
	if (StateMap.test(StateFlag::INSRT)) {
		if (StateMap.test(StateFlag::LIN1)) {
			if (StateMap.test(StateFlag::BAKEND))
				stch2px1(PCSHeader.stitchCount - 1);
			else
				stch2px1(0);
			endpnt();
		}
		else
			duIns();
	}
}

void thred::zumhom() {
	ZoomRect.bottom = 0;
	ZoomRect.left   = 0;
	ZoomRect.right  = UnzoomedRect.x;
	ZoomRect.top    = UnzoomedRect.y;
	ZoomFactor      = 1;
	StateMap.reset(StateFlag::ZUMED);
	thred::movStch();
	NearestCount = 0;
	if (StateMap.test(StateFlag::RUNPAT)) {
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		RunPoint = 0;
	}
	StateMap.set(StateFlag::RESTCH);
	if (StateMap.test(StateFlag::SELBOX))
		thi::shft2box();
	thred::duzrat();
	thi::movins();
}

void thred::internal::hupfn() {
	unsigned iForm = 0, iVertex = 0, iStitch = 0;
	dPOINT   hoopCenter   = {};
	dPOINT   designCenter = {};
	dPOINT   hoopSize     = {};
	dPOINT   delta        = {};

	StateMap.reset(StateFlag::HUPCHNG);
	sizstch(CheckHoopRect, StitchBuffer);
	if (FormIndex) {
		if (!PCSHeader.stitchCount) {
			CheckHoopRect.bottom = CheckHoopRect.top = CurrentFormVertices[0].y;
			CheckHoopRect.left = CheckHoopRect.right = CurrentFormVertices[0].x;
		}
		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {
			if (FormVertices[iVertex].x < CheckHoopRect.left)
				CheckHoopRect.left = FormVertices[iVertex].x;
			if (FormVertices[iVertex].x > CheckHoopRect.right)
				CheckHoopRect.right = FormVertices[iVertex].x;
			if (FormVertices[iVertex].y < CheckHoopRect.bottom)
				CheckHoopRect.bottom = FormVertices[iVertex].y;
			if (FormVertices[iVertex].y > CheckHoopRect.top)
				CheckHoopRect.top = FormVertices[iVertex].y;
		}
	}
	if (PCSHeader.stitchCount || FormVertexIndex || StateMap.test(StateFlag::HUPEX)) {
		if (CheckHoopRect.left < 0 || CheckHoopRect.right > IniFile.hoopSizeX || CheckHoopRect.bottom < 0
		    || CheckHoopRect.top > IniFile.hoopSizeY) {
			StateMap.set(StateFlag::HUPEX);
		}
		if (StateMap.test(StateFlag::HUPEX)) {
			hoopSize.x = CheckHoopRect.right - CheckHoopRect.left;
			hoopSize.y = CheckHoopRect.top - CheckHoopRect.bottom;
			if (hoopSize.x > IniFile.hoopSizeX) {
				IniFile.hoopSizeX = hoopSize.x;
				StateMap.set(StateFlag::HUPCHNG);
			}
			if (hoopSize.y > IniFile.hoopSizeY) {
				IniFile.hoopSizeY = hoopSize.y;
				StateMap.set(StateFlag::HUPCHNG);
			}
			designCenter.x = hoopSize.x / 2 + CheckHoopRect.left;
			designCenter.y = hoopSize.y / 2 + CheckHoopRect.bottom;
			hoopCenter.x   = IniFile.hoopSizeX / 2;
			hoopCenter.y   = IniFile.hoopSizeY / 2;
			delta.x        = hoopCenter.x - designCenter.x;
			delta.y        = hoopCenter.y - designCenter.y;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iStitch].x += delta.x;
				StitchBuffer[iStitch].y += delta.y;
			}
			for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {
				FormVertices[iVertex].x += delta.x;
				FormVertices[iVertex].y += delta.y;
			}
			for (iForm = 0; iForm < FormIndex; iForm++) {
				FormList[iForm].rectangle.left += delta.x;
				FormList[iForm].rectangle.right += delta.x;
				FormList[iForm].rectangle.top += delta.y;
				FormList[iForm].rectangle.bottom += delta.y;
			}
			UnzoomedRect.x = IniFile.hoopSizeX;
			UnzoomedRect.y = IniFile.hoopSizeY;
			ZoomMin        = static_cast<double>(MINZUM) / UnzoomedRect.x;
			thred::zumhom();
		}
	}
}

void thred::internal::chkhup() {
	StateMap.set(StateFlag::HUPEX);
	hupfn();
	if (StateMap.test(StateFlag::INIT))
		formForms::prfmsg();
	xt::setfchk();
}

float thred::internal::bufToDouble(wchar_t* buffer) {
	float value;

	try {
		value = std::stod(buffer);
	}
	catch (...) {
		OutputDebugString(fmt::format(L"stod failed trying to convert '{}'\n", buffer).c_str());
		value = 0.0;
	}
	return value;
}

unsigned long long thred::internal::bufTou64(wchar_t* buffer) {
	unsigned long long value;

	try {
		value = std::stoull(buffer);
	}
	catch (...) {
		OutputDebugString(fmt::format(L"stoull failed trying to convert '{}'\n", buffer).c_str());
		value = 0;
	}
	return value;
}

void thred::internal::chknum() {
	double value = 0.0;
	if (wcslen(MsgBuffer)) {
		value = bufToDouble(MsgBuffer);
	}
	unsigned edgeType = 0, borderColor = 0;

	xt::clrstch();
	if (StateMap.testAndReset(StateFlag::NUROT)) {
		if (value) {
			IniFile.rotationAngle = value * PI / 180;
		}
		return;
	}
	if (StateMap.testAndReset(StateFlag::ENTRSEG)) {
		if (value) {
			IniFile.rotationAngle = 2 * PI / value;
		}
		return;
	}
	if (MsgIndex) {
		if (FormMenuChoice) {
			value = bufToDouble(SideWindowEntryBuffer) * PFGRAN;
			switch (FormMenuChoice) {
			case LTXOF:
				thred::savdo();
				SelectedForm->txof = value;
				break;
			case LUANG:
				thred::savdo();
				SelectedForm->underlayStitchAngle = value / 180 * PI / PFGRAN;
				break;
			case LUSPAC:
				thred::savdo();
				SelectedForm->underlaySpacing = value;
				break;
			case LWLKIND:
				thred::savdo();
				SelectedForm->underlayIndent = value;
				break;
			case LULEN:
				thred::savdo();
				SelectedForm->underlayStitchLen = value;
				break;
			case LDSTRT:
				thred::savdo();
				SelectedForm->fillStart = value / PFGRAN;
				SelectedForm->fillStart %= VertexCount;
				break;
			case LDEND:
				thred::savdo();
				SelectedForm->fillEnd = value / PFGRAN;
				SelectedForm->fillEnd %= VertexCount;
				break;
			case LFTHUPCNT:
				thred::savdo();
				value /= PFGRAN;
				if (value > 255)
					value = 255;
				SelectedForm->fillInfo.feather.upCount = value;
				break;
			case LFTHCOL:
				if (value) {
					thred::savdo();
					form::nufthcol((bufTou64(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText((*ValueWindow)[LFRMCOL], SideWindowEntryBuffer);
					thred::coltab();
				}
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return;
			case LFRMCOL:
				if (value) {
					thred::savdo();
					form::nufilcol((bufTou64(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText((*ValueWindow)[LFRMCOL], SideWindowEntryBuffer);
					thred::coltab();
				}
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return;
			case LUNDCOL:
				if (value) {
					thred::savdo();
					SelectedForm->underlayColor = (bufTou64(SideWindowEntryBuffer) - 1) & 0xf;
					SetWindowText((*ValueWindow)[LUNDCOL], SideWindowEntryBuffer);
					form::refilfn();
					thred::coltab();
				}
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return;
			case LBRDCOL:
				if (value) {
					thred::savdo();
					form::nubrdcol((bufTou64(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText((*ValueWindow)[LFRMCOL], SideWindowEntryBuffer);
					thred::coltab();
				}
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return;
			case LBRDPIC:
				thred::savdo();
				SelectedForm->edgeSpacing = value;
				form::refil();
				return;
			case LFRMFAZ:
				thred::savdo();
				SelectedForm->wordParam = floor(value) / PFGRAN;
				thred::unsid();
				form::refil();
				return;
			case LBRDPOS:
				thred::savdo();
				SelectedForm->edgeStitchLen = value / PFGRAN;
				thred::unsid();
				form::refil();
				return;
			case LMAXFIL:
				thred::savdo();
				SelectedForm->maxFillStitchLen = value;
				thred::unsid();
				form::refil();
				return;
			case LMINFIL:
				thred::savdo();
				SelectedForm->minFillStitchLen = value;
				thred::unsid();
				form::refil();
				return;
			case LMAXBRD:
				thred::savdo();
				SelectedForm->maxBorderStitchLen = value;
				thred::unsid();
				form::refil();
				return;
			case LMINBRD:
				thred::savdo();
				SelectedForm->minBorderStitchLen = value;
				thred::unsid();
				form::refil();
				return;
			}
			if (FormMenuChoice == LBCSIZ) {
				thred::savdo();
				if (SelectedForm->edgeType == EDGEBHOL)
					form::savblen(static_cast<float>(value));
				else
					form::savplen(static_cast<float>(value));
			}
			else {
				if (value) {
					switch (FormMenuChoice) {
					case LFTHSIZ:
						thred::savdo();
						SelectedForm->fillInfo.feather.ratio = value / PFGRAN;
						break;
					case LFTHNUM:
						thred::savdo();
						SelectedForm->fillInfo.feather.count = value / PFGRAN;
						break;
					case LFTHFLR:
						thred::savdo();
						SelectedForm->fillInfo.feather.minStitchSize = value;
						break;
					case LFTHDWNCNT:
						thred::savdo();
						value /= PFGRAN;
						if (value > 255)
							value = 255;
						SelectedForm->fillInfo.feather.downCount = value;
						break;
					case LFRMSPAC:
						thred::savdo();
						SelectedForm->fillSpacing = value;
						break;
					case LFRMLEN:
						thred::savdo();
						SelectedForm->lengthOrCount.stitchLength = value;
						break;
					case LBRDSPAC:
						thred::savdo();
						edgeType = SelectedForm->edgeType & NEGUND;
						switch (edgeType) {
						case EDGEPROPSAT:
							SelectedForm->edgeSpacing = value;
							break;
						case EDGEOCHAIN:
						case EDGELCHAIN:
							SelectedForm->edgeSpacing = value;
							break;
						default:
							SelectedForm->edgeSpacing = value / 2;
						}
						break;
					case LBRDLEN:
						thred::savdo();
						SelectedForm->edgeStitchLen = value;
						break;
					case LBRDSIZ:
						thred::savdo();
						SelectedForm->borderSize = value;
						break;
					case LFRMANG:
						thred::savdo();
						SelectedForm->angleOrClipData.angle = value / 180 * PI / PFGRAN;
						break;
					case LSACANG:
						thred::savdo();
						SelectedForm->satinOrAngle.angle = value / 180 * PI / PFGRAN;
						break;
					case LAPCOL:
						thred::savdo();
						SelectedForm->borderColor &= COLMSK;
						borderColor = value / PFGRAN;
						if (borderColor)
							borderColor--;
						borderColor &= COLMSK;
						SelectedForm->borderColor |= (borderColor << 4);
						break;
					}
				}
				else {
					thred::savdo();
					if (FormMenuChoice == LFRMSPAC && isfclp())
						SelectedForm->fillSpacing = 0;
				}
			}
			thred::unsid();
			form::refil();
		}
		else {
			if (PreferenceIndex) {
				value = bufToDouble(SideWindowEntryBuffer);
				switch (PreferenceIndex - 1) {
				case PEG:
					IniFile.eggRatio = value;
					SetWindowText((*ValueWindow)[PEG], fmt::format(L"{:.2f}", value).c_str());
					break;
				case PNUDG:
					IniFile.cursorNudgeStep = value;
					IniFile.nudgePixels     = pxchk(value);
					SetWindowText((*ValueWindow)[PNUDG], fmt::format(L"{:.2f}", value).c_str());
					break;
				case PPIC:
					PicotSpacing = value * PFGRAN;
					SetWindowText((*ValueWindow)[PPIC], fmt::format(L"{:.2f}", value).c_str());
					break;
				case PCLPOF:
					IniFile.clipOffset = value * PFGRAN;
					SetWindowText((*ValueWindow)[PCLPOF], fmt::format(L"{:.2f} mm", value).c_str());
					break;
				case PFAZ:
					IniFile.fillPhase = floor(value);
					SetWindowText((*ValueWindow)[PFAZ], fmt::format(L"{}", IniFile.fillPhase).c_str());
					break;
				case PCHRAT:
					IniFile.chainRatio = value;
					SetWindowText((*ValueWindow)[PCHRAT], fmt::format(L"{:.2f}", value).c_str());
					break;
				case PMIN:
					MinStitchLength = value * PFGRAN;
					SetWindowText((*ValueWindow)[PMIN], fmt::format(L"{:.2f}", value).c_str());
					break;
				default:
					if (value) {
						std::wstring bufVal(fmt::format(L"{:.2f}", value));
						switch (PreferenceIndex - 1) {
						case PSPAC:
							LineSpacing = value * PFGRAN;
							SetWindowText((*ValueWindow)[PSPAC], bufVal.c_str());
							break;
						case PANGL:
							IniFile.fillAngle = value / 180 * PI;
							SetWindowText((*ValueWindow)[PANGL], bufVal.c_str());
							break;
						case PSAT:
							BorderWidth         = value * PFGRAN;
							IniFile.borderWidth = BorderWidth;
							SetWindowText((*ValueWindow)[PSAT], bufVal.c_str());
							break;
						case PMAX:
							IniFile.maxStitchLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PMAX], bufVal.c_str());
							break;
						case PUSE:
							UserStitchLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PUSE], bufVal.c_str());
							break;
						case PSMAL:
							SmallStitchLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PSMAL], bufVal.c_str());
							break;
						case PAP:
							AppliqueColor = gsl::narrow<unsigned>(round(value - 1)) % 16;
							SetWindowText((*ValueWindow)[PAP], fmt::format(L"{}", (AppliqueColor + 1)).c_str());
							break;
						case PSNP:
							SnapLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PSNP], bufVal.c_str());
							break;
						case PSTAR:
							StarRatio = value;
							if (StarRatio > 1)
								StarRatio = 1;
							if (StarRatio < 0.05)
								StarRatio = 0.05;
							SetWindowText((*ValueWindow)[PSTAR], fmt::format(L"{:.2f}", StarRatio).c_str());
							break;
						case PSPIR:
							SpiralWrap = value;
							if (StarRatio > 20)
								StarRatio = 20.;
							if (StarRatio < 0.3)
								StarRatio = 0.3;
							SetWindowText((*ValueWindow)[PSPIR], fmt::format(L"{:.2f}", SpiralWrap).c_str());
							break;
						case PBUT:
							ButtonholeCornerLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PBUT], fmt::format(L"{:.2f}", value).c_str());
							break;
						case PHUPX:
							IniFile.hoopSizeX = value * PFGRAN;
							SetWindowText((*ValueWindow)[PHUPX], fmt::format(L"{:.0f} mm", value).c_str());
							form::sethup();
							formForms::prfmsg();
							chkhup();
							break;
						case PHUPY:
							IniFile.hoopSizeY = value * PFGRAN;
							SetWindowText((*ValueWindow)[PHUPY], fmt::format(L"{:.0f} mm", value).c_str());
							form::sethup();
							formForms::prfmsg();
							chkhup();
							break;
						case PGRD:
							IniFile.gridSize = value * PFGRAN;
							SetWindowText((*ValueWindow)[PGRD], fmt::format(L"{:.2f} mm", value).c_str());
							break;
						case PCHN:
							IniFile.chainSpace = value * PFGRAN;
							SetWindowText((*ValueWindow)[PCHN], fmt::format(L"{:.2f}", value).c_str());
							break;
						}
					}
				}
				thred::unsid();
				PreferenceIndex = 0;
			}
			else {
				if (wcslen(MsgBuffer)) {
					value = bufToDouble(MsgBuffer);
				}
				if (StateMap.testAndReset(StateFlag::SCLPSPAC))
					IniFile.clipOffset = value * PFGRAN;
				if (StateMap.testAndReset(StateFlag::FSETFIND))
					xt::dufind(value);
				if (StateMap.testAndReset(StateFlag::FSETFHI)) {
					if (value)
						xt::dufhi(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETFWID)) {
					if (value)
						xt::dufwid(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETFMAX)) {
					if (value)
						xt::dufmax(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETFMIN))
					xt::dufmin(value * PFGRAN);
				if (StateMap.testAndReset(StateFlag::FSETBMAX)) {
					if (value)
						xt::dubmax(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETBMIN))
					xt::dubmin(value * PFGRAN);
				if (StateMap.testAndReset(StateFlag::FSETBSPAC)) {
					if (value)
						xt::dubspac(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETFLEN)) {
					if (value)
						xt::dublen(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETBCOL))
					xt::dubcol(value);
				if (StateMap.testAndReset(StateFlag::FSETFCOL))
					xt::dufcol(value);
				if (StateMap.testAndReset(StateFlag::FSETUCOL))
					xt::dundcol(value);
				if (StateMap.testAndReset(StateFlag::FSETFANG))
					xt::dufxang(value);
				if (StateMap.testAndReset(StateFlag::FSETFSPAC)) {
					if (value)
						xt::dufspac(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETUANG))
					xt::dufang(value);
				if (StateMap.testAndReset(StateFlag::FSETFLEN)) {
					if (value)
						xt::duflen(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETUSPAC)) {
					if (value)
						xt::duspac(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::FSETULEN)) {
					if (value)
						xt::dusulen(value * PFGRAN);
				}
				if (StateMap.testAndReset(StateFlag::GTUANG))
					IniFile.underlayAngle = value / 180 * PI;
				if (StateMap.testAndReset(StateFlag::GTUSPAC)) {
					if (value)
						IniFile.underlaySpacing = value * PFGRAN;
				}
				if (StateMap.testAndReset(StateFlag::GTWLKIND))
					IniFile.underlayIndent = value * PFGRAN;
				if (StateMap.testAndReset(StateFlag::GTWLKLEN)) {
					if (value)
						IniFile.underlayStitchLen = value * PFGRAN;
				}
				if (StateMap.testAndReset(StateFlag::PIXIN))
					IniFile.nudgePixels = pxchk(value);
				if (StateMap.testAndReset(StateFlag::STPXIN))
					IniFile.stitchSizePixels = pxchk(value);
				if (StateMap.testAndReset(StateFlag::FRMPXIN))
					IniFile.formVertexSizePixels = value;
				if (StateMap.testAndReset(StateFlag::FRMBOXIN))
					IniFile.formBoxSizePixels = value;
				if (StateMap.testAndReset(StateFlag::GETMIN))
					SmallStitchLength = value * PFGRAN;
				if (StateMap.testAndReset(StateFlag::ENTR30))
					ThreadSize30 = value;
				if (StateMap.testAndReset(StateFlag::ENTR40))
					ThreadSize40 = value;
				if (StateMap.testAndReset(StateFlag::ENTR60))
					ThreadSize60 = value;
				if (StateMap.testAndReset(StateFlag::ENTRFNUM)) {
					if (value < FormIndex)
						form::frmnumfn(value);
					else
						displayText::tabmsg(IDS_FRMN1);
					return;
				}
				value = floor(value);
				if (StateMap.testAndReset(StateFlag::ENTRPOL))
					form::durpoli(value);
				if (StateMap.testAndReset(StateFlag::ENTRSTAR))
					form::dustar(value, 250 / value * ZoomFactor * (UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY));
				if (StateMap.testAndReset(StateFlag::ENTRSPIR))
					form::duspir(value);
				if (StateMap.testAndReset(StateFlag::ENTRHART))
					form::duhart(value);
				if (StateMap.testAndReset(StateFlag::ENTRLENS))
					form::dulens(value);
				if (StateMap.testAndReset(StateFlag::ENTREG))
					form::dueg(value);
				if (StateMap.testAndReset(StateFlag::ENTRZIG))
					form::duzig(value);
			}
		}
	}
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::noMsg() {
	if (DestroyWindow(MsgWindow))
		MsgWindow = 0;
	DestroyWindow(OKButton);
	DestroyWindow(DiscardButton);
	DestroyWindow(CancelButton);
	DestroyWindow(DeleteStitchesDialog);
	if (StateMap.testAndReset(StateFlag::NUMIN) || FormMenuChoice || PreferenceIndex)
		chknum();
	StateMap.reset(StateFlag::MOVMSG);
	StateMap.reset(StateFlag::DELFRM);
	StateMap.reset(StateFlag::FILMSG);
	StateMap.set(StateFlag::RESTCH);
}

void thred::unmsg() {
	if (MsgWindow)
		thi::noMsg();
}

[[gsl::suppress(con .3)]] bool thred::internal::oldwnd(HWND window) {
	unsigned iWindow = 0, iColor = 0;

	for (iColor = 0; iColor < 16; iColor++) {
		if (DefaultColorWin[iColor] == window || UserColorWin[iColor] == window || ThreadSizeWin[iColor] == window)
			return 0;
	}
	for (iWindow = 0; iWindow < 9; iWindow++) {
		if ((*ButtonWin)[iWindow] == window)
			return 0;
	}
	if (MainStitchWin == window)
		return 0;
	if (VerticalScrollBar == window)
		return 0;
	if (HorizontalScrollBar == window)
		return 0;
	if (ColorBar == window)
		return 0;
	if (SpeedScrollBar == window)
		return 0;
	return 1;
}

BOOL CALLBACK thred::internal::EnumChildProc(HWND hwnd, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	if (oldwnd(hwnd))
		return DestroyWindow(hwnd);
	else {
		if (FirstWin) {
			if (FirstWin == hwnd)
				return 0;
			else
				return TRUE;
		}
		else {
			FirstWin = hwnd;
			return TRUE;
		}
	}
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
	displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
	if (ZoomFactor == 1)
		StateMap.reset(StateFlag::ZUMED);
	thred::movStch();
	ShowWindow(MainStitchWin, TRUE);
	thred::unbsho();
	StateMap.reset(StateFlag::SIDACT);
	thred::unsid();
	StateMap.reset(StateFlag::PRFACT);
	StateMap.reset(StateFlag::WASRT);
	DestroyWindow(PreferencesWindow);
	thred::undat();
	if (StateMap.testAndReset(StateFlag::INSFRM))
		insadj();
	StateMap.reset(StateFlag::FUNCLP);
	if (StateMap.testAndReset(StateFlag::SATPNT))
		satin::satfix();
	if (StateMap.testAndReset(StateFlag::RUNPAT))
		patdun();
	StateMap.reset(StateFlag::FORMSEL);
	StateMap.reset(StateFlag::FRMPSEL);
	thred::unmsg();
	SearchLineIndex = 0;
	FirstWin        = 0;
	while (EnumChildWindows(MainStitchWin, EnumChildProc, 0)) {
	}
}

void thred::ritot(unsigned number) {
	std::wstring txt;
	txt              = fmt::format((*StringTable)[STR_TOT], number);
	BufferDigitCount = txt.size();
	displayText::butxt(HTOT, txt);
}

void thred::internal::frmcalc() {
	unsigned iStitch   = 0;
	size_t   code      = 0;
	double   maxLength = 0;
	double   minLength = 1e99;
	double   length    = 0.0;

	if (FormList[ClosestFormToCursor].fillType || FormList[ClosestFormToCursor].edgeType) {
		code = ClosestFormToCursor << FRMSHFT;
		for (iStitch = 0; iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & FRMSK) == code && !(StitchBuffer[iStitch].attribute & NOTFRM)
			    && (StitchBuffer[iStitch + 1].attribute & FRMSK) == code && (StitchBuffer[iStitch + 1].attribute & TYPMSK)) {
				length = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x,
				               StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
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
		if (fabs(maxLength < 10000)) {
			std::wstring strMax;
			displayText::loadString(strMax, IDS_LENMAX);
			displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength / PFGRAN)));
		}
		if (fabs(minLength < 10000)) {
			std::wstring strMin;
			displayText::loadString(strMin, IDS_LENMIN);
			displayText::butxt(HMINLEN, fmt::format(strMin, (minLength / PFGRAN)));
		}
	}
	else {
		const std::wstring blank(L"");
		displayText::butxt(HMAXLEN, blank);
		displayText::butxt(HMINLEN, blank);
	}
}

void thred::internal::lenfn(unsigned start, unsigned end) {
	unsigned iStitch   = 0;
	double   maxLength = 0;
	double   minLength = 1e99;
	double   length    = 0.0;

	SmallestStitchIndex = 0;
	LargestStitchIndex  = 0;
	for (iStitch = start; iStitch < end; iStitch++) {
		length
		    = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
		if (length > maxLength) {
			maxLength          = length;
			LargestStitchIndex = iStitch;
		}
		if (length < minLength) {
			minLength           = length;
			SmallestStitchIndex = iStitch;
		}
	}
	std::wstring strMax, strMin;
	displayText::loadString(strMax, IDS_LENMAX);
	displayText::loadString(strMin, IDS_LENMIN);
	displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength / PFGRAN)));
	displayText::butxt(HMINLEN, fmt::format(strMin, (minLength / PFGRAN)));
}

void thred::internal::lenCalc() {
	const std::wstring blank(L"");

	if (StateMap.test(StateFlag::LENSRCH)) {
		std::wstring txt;
		const float  lenMax = (hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x,
                                    StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y)
                              / PFGRAN);
		displayText::butxt(HMINLEN, fmt::format(L"{:.2f}", lenMax));
		displayText::loadString(txt, IDS_SRCH);
		displayText::butxt(HMAXLEN, txt);
	}
	else {
		if (PCSHeader.stitchCount > 1) {
			if (StateMap.test(StateFlag::FORMSEL)) {
				frmcalc();
				displayText::butxt(HCOR, blank);
				return;
			}
			thred::rngadj();
			if (StateMap.test(StateFlag::GRPSEL) && GroupStartStitch != GroupEndStitch)
				lenfn(GroupStartStitch, GroupEndStitch);
			else
				lenfn(0, PCSHeader.stitchCount - 1);
		}
		else {
			displayText::butxt(HMAXLEN, blank);
			displayText::butxt(HMINLEN, blank);
		}
	}
}

void thred::internal::delsmal(unsigned startStitch, unsigned endStitch) {
	// ToDo - does this function work correctly?
	// ToDo - rename inf

	unsigned     iStitch = 0, iNextStitch = 0;
	const size_t codedAttribute = ClosestFormToCursor << 4;
	unsigned     inf            = 0;
	long         dx = 0, dy = 0;
	double       stitchSize = 1e99;

	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		iNextStitch = form::find1st();
		iStitch     = iNextStitch + 1;
		while (iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1 && stitchSize > SmallStitchLength) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute) {
				if (StitchBuffer[iStitch].attribute & KNOTMSK)
					iNextStitch = iStitch;
				else {
					dx         = StitchBuffer[iStitch].x - StitchBuffer[iNextStitch].x;
					dy         = StitchBuffer[iStitch].y - StitchBuffer[iNextStitch].y;
					stitchSize = hypot(dx, dy);
				}
				iStitch++;
			}
		}
		if (iStitch != endStitch - 2) {
			iStitch--;
			iNextStitch = iStitch + 2;
			while (iNextStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
				do {
					if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute
					    && !(StitchBuffer[iStitch].attribute & KNOTMSK)) {
						dx         = StitchBuffer[iNextStitch].x - StitchBuffer[iStitch].x;
						dy         = StitchBuffer[iNextStitch++].y - StitchBuffer[iStitch].y;
						stitchSize = hypot(dx, dy);
					}
					else
						iNextStitch++;
				} while (stitchSize < SmallStitchLength && iNextStitch < PCSHeader.stitchCount);
				StitchBuffer[++iStitch] = StitchBuffer[--iNextStitch];
			}
			iStitch++;
			while (iNextStitch < PCSHeader.stitchCount) {
				StitchBuffer[iStitch++] = StitchBuffer[iNextStitch++];
			}
			PCSHeader.stitchCount = iStitch;
			thred::coltab();
		}
	}
	else {
		iStitch       = startStitch;
		iNextStitch   = startStitch + 1;
		SelectedPoint = StitchBuffer[iStitch];
		for (inf = iNextStitch; inf < endStitch; inf++) {
			if (StitchBuffer[iNextStitch].attribute & KNOTMSK) {
				SelectedPoint = StitchBuffer[iNextStitch];
				thred::mvstch(iNextStitch++, inf);
			}
			else {
				dx         = StitchBuffer[inf].x - SelectedPoint.x;
				dy         = StitchBuffer[inf].y - SelectedPoint.y;
				stitchSize = hypot(dx, dy);
				if (stitchSize > SmallStitchLength) {
					thred::mvstch(iNextStitch++, inf);
					SelectedPoint = StitchBuffer[inf];
				}
			}
		}
		while (inf < PCSHeader.stitchCount)
			thred::mvstch(iNextStitch++, inf++);
		PCSHeader.stitchCount = iNextStitch;
		thred::coltab();
	}
	rstAll();
	thred::ritot(PCSHeader.stitchCount);
	lenCalc();
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::duzero() {
	unsigned       iStitch       = 0;
	unsigned short iDestination  = 0;
	double         stitchLength  = 0.0;
	fPOINTATTR*    currentStitch = nullptr;

	if (SelectedFormList->size()) {
		boost::dynamic_bitset<> formMap(FormIndex);
		for (auto selectedForm : (*SelectedFormList)) {
			formMap.set(selectedForm);
		}
		StateMap.reset(StateFlag::CONTIG);
		iDestination  = 0;
		currentStitch = StitchBuffer;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute & TYPMSK && formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
				if (StateMap.testAndSet(StateFlag::CONTIG)) {
					stitchLength = hypot(StitchBuffer[iStitch].x - currentStitch->x, StitchBuffer[iStitch].y - currentStitch->y);
					if (stitchLength > MinStitchLength) {
						currentStitch                = &StitchBuffer[iStitch];
						StitchBuffer[iDestination++] = StitchBuffer[iStitch];
					}
				}
				else
					currentStitch = &StitchBuffer[iStitch];
			}
			else {
				StitchBuffer[iDestination++] = StitchBuffer[iStitch];
				StateMap.reset(StateFlag::CONTIG);
			}
		}
		PCSHeader.stitchCount = iDestination;
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		delsmal(GroupStartStitch, GroupEndStitch);
	}
	else
		delsmal(0, PCSHeader.stitchCount);
}

void thred::internal::rshft(const POINT& shiftPoint) {
	ZoomRect.right -= shiftPoint.x;
	ZoomRect.left -= shiftPoint.x;
	ZoomRect.top -= shiftPoint.y;
	ZoomRect.bottom -= shiftPoint.y;
	zRctAdj();
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::pgdwn() {
	POINT scrollPosition = {};

	if (StateMap.test(StateFlag::ZUMED)) {
		scrollPosition.y = (ZoomRect.top - ZoomRect.bottom) * PAGSCROL;
		scrollPosition.x = 0;
		rshft(scrollPosition);
	}
}

void thred::internal::pgup() {
	POINT scrollPosition = {};

	if (StateMap.test(StateFlag::ZUMED)) {
		scrollPosition.y = -(ZoomRect.top - ZoomRect.bottom) * PAGSCROL;
		scrollPosition.x = 0;
		rshft(scrollPosition);
	}
}

void thred::internal::pglft() {
	POINT scrollPosition = {};

	if (StateMap.test(StateFlag::ZUMED)) {
		scrollPosition.x = (ZoomRect.right - ZoomRect.left) * PAGSCROL;
		scrollPosition.y = 0;
		rshft(scrollPosition);
	}
}

void thred::internal::pgrit() {
	POINT scrollPosition = {};

	if (StateMap.test(StateFlag::ZUMED)) {
		scrollPosition.x = -(ZoomRect.right - ZoomRect.left) * PAGSCROL;
		scrollPosition.y = 0;
		rshft(scrollPosition);
	}
}

void thred::internal::selin(unsigned start, unsigned end, HDC dc) {
	unsigned   iStitch = 0, swap = 0;
	double     coordinate = 0.0;
	const long hi         = StitchWindowClientRect.bottom;

	SelectObject(dc, GroupSelectPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	if (SearchLineIndex)
		PolylineInt(dc, SearchLine, SearchLineIndex);
	if (start > end) {
		swap  = start;
		start = end;
		end   = swap;
	}
	SearchLineIndex = 0;
	for (iStitch = start; iStitch <= end; iStitch++) {
		coordinate                      = ((StitchBuffer[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5);
		SearchLine[SearchLineIndex].x   = gsl::narrow<long>(round(coordinate));
		coordinate                      = (hi - (StitchBuffer[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5);
		SearchLine[SearchLineIndex++].y = gsl::narrow<long>(round(coordinate));
	}
	PolylineInt(dc, SearchLine, SearchLineIndex);
	SetROP2(dc, R2_COPYPEN);
}

void thred::internal::cros(unsigned iStitch) noexcept {
	const long armLength = BoxOffset[0];

	stch2px1(iStitch);
	InsertLine[0].x = StitchCoordinatesPixels.x - armLength;
	InsertLine[1].y = InsertLine[0].y = StitchCoordinatesPixels.y;
	InsertLine[1].x                   = StitchCoordinatesPixels.x + armLength;
	SelectObject(StitchWindowDC, CrossPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
	SelectObject(StitchWindowMemDC, CrossPen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	InsertLine[0].y = StitchCoordinatesPixels.y - armLength;
	InsertLine[0].x = InsertLine[1].x = StitchCoordinatesPixels.x;
	InsertLine[1].y                   = StitchCoordinatesPixels.y - 1;
	Polyline(StitchWindowDC, InsertLine, 2);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	InsertLine[0].y = StitchCoordinatesPixels.y + 2;
	InsertLine[1].y = StitchCoordinatesPixels.y + armLength;
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::internal::uncros() {
	if (StateMap.testAndReset(StateFlag::SCROS))
		cros(GroupStartStitch);
	if (StateMap.testAndReset(StateFlag::ECROS)) {
		if (GroupEndStitch != GroupStartStitch)
			cros(GroupEndStitch);
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
	unsigned iStitch = 0;

	sourceRect.left = sourceRect.right = StitchBuffer[GroupStartStitch].x;
	sourceRect.top = sourceRect.bottom = StitchBuffer[GroupStartStitch].y;
	for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
		if (StitchBuffer[iStitch].x > sourceRect.right)
			sourceRect.right = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].x < sourceRect.left)
			sourceRect.left = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].y < sourceRect.bottom)
			sourceRect.bottom = StitchBuffer[iStitch].y;
		if (StitchBuffer[iStitch].y > sourceRect.top)
			sourceRect.top = StitchBuffer[iStitch].y;
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
	unsigned ind;

	SetROP2(dc, R2_NOTXORPEN);
	SelectObject(dc, LinePen);
	PolylineInt(dc, FormControlPoints->data(), (FormControlPoints->size() - 1));
	for (ind = 0; ind < 8; ind++)
		form::selsqr((*FormControlPoints)[ind], dc);
	SetROP2(dc, R2_COPYPEN);
}

void thred::internal::unsel() {
	if (StateMap.testAndReset(StateFlag::SELSHO))
		dusel(StitchWindowDC);
}

void thred::internal::nuselrct() {
	fPOINT   outline[9] = {};
	unsigned iLine      = 0;

	unsel();
	outline[0].x = outline[6].x = outline[7].x = outline[8].x = StitchRangeRect.left;
	outline[1].x = outline[5].x = form::midl(StitchRangeRect.right, StitchRangeRect.left);
	outline[0].y = outline[1].y = outline[2].y = outline[8].y = StitchRangeRect.top;
	outline[3].y = outline[7].y = form::midl(StitchRangeRect.top, StitchRangeRect.bottom);
	outline[4].y = outline[5].y = outline[6].y = StitchRangeRect.bottom;
	outline[2].x = outline[3].x = outline[4].x = StitchRangeRect.right;
	for (iLine = 0; iLine < 9; iLine++)
		form::sfCor2px(outline[iLine], (*FormControlPoints)[iLine]);
}

void thred::grpAdj() {
	POINT  newSize    = {};
	double coordinate = 0.0;

	thi::uncros();
	thi::unsel();
	thred::rngadj();
	thi::ducros(StitchWindowDC);
	thi::lenCalc();
	thred::selRct(StitchRangeRect);
	if (StateMap.test(StateFlag::ZUMED) && GroupEndStitch != GroupStartStitch) {
		if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1
		    || StitchRangeRect.left < ZoomRect.left + 1 || StitchRangeRect.right > ZoomRect.right - 1) {
			newSize.x = StitchRangeRect.right - StitchRangeRect.left;
			newSize.y = StitchRangeRect.top - StitchRangeRect.bottom;
			if (newSize.x < MINZUM) {
				if (newSize.x < TINY)
					newSize.x = 1;
				coordinate = MINZUM / newSize.x;
				newSize.x  = MINZUM;
				newSize.y  = coordinate * newSize.y;
			}
			if (newSize.x > newSize.y) {
				coordinate = newSize.x * ZMARGIN;
				newSize.x += gsl::narrow<long>(round(coordinate));
				coordinate = newSize.x / StitchWindowAspectRatio;
				newSize.y  = gsl::narrow<long>(round(coordinate));
			}
			else {
				coordinate = newSize.y * ZMARGIN;
				newSize.y  = gsl::narrow<long>(round(coordinate));
				coordinate = newSize.y * StitchWindowAspectRatio;
				newSize.x  = gsl::narrow<long>(round(coordinate));
			}
			if (newSize.x > UnzoomedRect.x || newSize.y > UnzoomedRect.y) {
				ZoomRect.left = ZoomRect.bottom = 0;
				ZoomRect.right                  = UnzoomedRect.x;
				ZoomRect.top                    = UnzoomedRect.y;
				StateMap.reset(StateFlag::ZUMED);
				ZoomFactor = 1;
				thred::movStch();
			}
			else {
				ZoomRect.right  = ZoomRect.left + (newSize.x);
				ZoomFactor      = static_cast<double>(newSize.x) / static_cast<double>(UnzoomedRect.x);
				ZoomRect.top    = ZoomRect.bottom + (newSize.y);
				SelectedPoint.x = ((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left;
				SelectedPoint.y = ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom;
				thred::shft(SelectedPoint);
			}
		}
	}
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::nuAct(unsigned iStitch) noexcept {
	unsigned color;

	color       = ActiveColor;
	ActiveColor = StitchBuffer[iStitch].attribute & COLMSK;
	thred::redraw(UserColorWin[color]);
	thred::redraw(UserColorWin[ActiveColor]);
}

void thred::internal::lensadj() {
	uncros();
	unsel();
	ClosestPointIndex = CurrentStitchIndex;
	GroupStitchIndex  = CurrentStitchIndex + 1;
	thred::rngadj();
	ducros(StitchWindowDC);
	lenCalc();
	thred::selRct(StitchRangeRect);
	if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1
	    || StitchRangeRect.left < ZoomRect.left + 1 || StitchRangeRect.right > ZoomRect.right - 1) {
		SelectedPoint.x = ((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left;
		SelectedPoint.y = ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom;
		thred::shft(SelectedPoint);
	}
	nuAct(GroupStartStitch);
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::ritlayr() {
	unsigned layer = 0xffffffff;

	if (StateMap.test(StateFlag::SELBOX))
		layer = (StitchBuffer[ClosestPointIndex].attribute & LAYMSK) >> LAYSHFT;
	else {
		if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::FRMPSEL))
			layer = (FormList[ClosestFormToCursor].attribute & FRMLMSK) >> 1;
	}
	if (layer & 0xffff0000) {
		const std::wstring blank(L"");
		displayText::butxt(HLAYR, blank);
	}
	else {
		std::wstring txt;
		txt              = fmt::format((*StringTable)[STR_LAYR], layer);
		BufferDigitCount = txt.size();
		displayText::butxt(HLAYR, txt);
	}
}

HPEN thred::internal::nuPen(HPEN pen, unsigned width, COLORREF color) noexcept {
	DeleteObject(pen);
	return CreatePen(PS_SOLID, width, color);
}

void thred::internal::nuStchSiz(unsigned iColor, unsigned width) noexcept {
	if (width != ThreadSizePixels[iColor]) {
		UserPen[iColor]          = nuPen(UserPen[iColor], width, UserColor[iColor]);
		ThreadSizePixels[iColor] = width;
	}
}

HBRUSH thred::internal::nuBrush(HBRUSH brush, COLORREF color) noexcept {
	DeleteObject(brush);
	return CreateSolidBrush(color);
}

void thred::internal::rotpix(const POINT& unrotatedPoint, POINT& rotatedPoint, const POINT& rotationCenterPixels) {
	// won't handle vertical lines

	double     distanceToCenter = 0.0, newAngle = 0.0;
	const long dx = unrotatedPoint.x - rotationCenterPixels.x;
	const long dy = unrotatedPoint.y - rotationCenterPixels.y;

	distanceToCenter = hypot(dx, dy);
	newAngle         = atan2(dy, dx);
	newAngle -= RotateAngle;
	rotatedPoint.y = rotationCenterPixels.y + distanceToCenter * sin(newAngle);
	rotatedPoint.x = rotationCenterPixels.x + distanceToCenter * cos(newAngle);
}

void thred::internal::duar() {
	POINT arrowCenter = { (StitchCoordinatesPixels.x - 10), (StitchCoordinatesPixels.y + 10) };

	StitchArrow[1] = StitchCoordinatesPixels;
	rotpix(arrowCenter, StitchArrow[0], StitchCoordinatesPixels);
	arrowCenter.y = StitchCoordinatesPixels.y - 10;
	rotpix(arrowCenter, StitchArrow[2], StitchCoordinatesPixels);
	SelectObject(StitchWindowMemDC, BoxPen[0]);
	SelectObject(StitchWindowDC, BoxPen[0]);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, StitchArrow, 3);
	Polyline(StitchWindowDC, StitchArrow, 3);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::dubox() {
	RotateAngle = atan2(StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y,
	                    StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x);
	duar();
	StateMap.reset(StateFlag::ELIN);
	StateMap.set(StateFlag::SELBOX);
	StateMap.reset(StateFlag::FRMPSEL);
	thred::redraw(ColorBar);
	// if (!BufferIndex)
	displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
}

unsigned thred::internal::stch2px(unsigned iStitch) noexcept {
	StitchCoordinatesPixels.x = (StitchBuffer[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (StitchBuffer[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
	if (StitchCoordinatesPixels.x >= 0 && StitchCoordinatesPixels.x <= StitchWindowClientRect.right
	    && StitchCoordinatesPixels.y >= 0 && StitchCoordinatesPixels.y <= StitchWindowClientRect.bottom)

		return 1;
	else
		return 0;
}

void thred::stch2pxr(const fPOINT& stitchCoordinate) noexcept {
	StitchCoordinatesPixels.x = (stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (stitchCoordinate.y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
}

void thred::internal::getDocsFolder(fs::path* directory) {
	if (directory) {
		PWSTR         ppszPath = nullptr; // variable to receive the path memory block pointer.
		const HRESULT hr       = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &ppszPath);

		if (SUCCEEDED(hr)) {
			directory->assign(ppszPath); // make a local copy of the path
		}

		CoTaskMemFree(ppszPath); // free up the path memory block
	}
}

void thred::internal::defNam(const fs::path& fileName) {
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
	unsigned    iColor     = 0;
	RECT        windowRect = {};
	std::string previous;

	auto           directory = utf::Utf16ToUtf8(DefaultDirectory->wstring());
	constexpr char fillchar  = '\0';
	std::fill(IniFile.defaultDirectory, IniFile.defaultDirectory + sizeof(IniFile.defaultDirectory), fillchar);
	std::copy(directory.begin(), directory.end(), IniFile.defaultDirectory);
	const auto& previousNames = *PreviousNames;
	for (auto iVersion = 0; iVersion < OLDNUM; iVersion++) {
		auto& prevNam = IniFile.prevNames[iVersion];
		std::fill(prevNam, prevNam + sizeof(prevNam), fillchar);
		if (!previousNames[iVersion].empty()) {
			previous.assign(utf::Utf16ToUtf8(previousNames[iVersion]));
			std::copy(previous.begin(), previous.end(), IniFile.prevNames[iVersion]);
		}
		else {
			previous.clear();
		}
	}
	auto designer = utf::Utf16ToUtf8(*DesignerName);
	std::fill(IniFile.designerName, IniFile.designerName + sizeof(IniFile.designerName), fillchar);
	std::copy(designer.begin(), designer.end(), IniFile.designerName);
	for (iColor = 0; iColor < 16; iColor++) {
		IniFile.stitchColors[iColor]              = UserColor[iColor];
		IniFile.backgroundPreferredColors[iColor] = CustomBackgroundColor[iColor];
		IniFile.stitchPreferredColors[iColor]     = CustomColor[iColor];
		IniFile.bitmapBackgroundColors[iColor]    = BitmapBackgroundColors[iColor];
	}
	IniFile.backgroundColor = BackgroundColor;
	IniFile.bitmapColor     = BitmapColor;
	IniFile.minStitchLength = MinStitchLength;
	if (ShowStitchThreshold < 0) {
		ShowStitchThreshold = 0;
	}
	if (ShowStitchThreshold > 9) {
		ShowStitchThreshold = 9;
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
		GetWindowRect(ThrEdWindow, &windowRect);
		IniFile.initialWindowCoords.left   = windowRect.left;
		IniFile.initialWindowCoords.right  = windowRect.right;
		IniFile.initialWindowCoords.bottom = windowRect.bottom;
		IniFile.initialWindowCoords.top    = windowRect.top;
	}
	IniFileHandle = CreateFile(IniFileName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (IniFileHandle != INVALID_HANDLE_VALUE)
		WriteFile(IniFileHandle, &IniFile, sizeof(IniFile), &BytesRead, NULL);
	CloseHandle(IniFileHandle);
}

bool thred::internal::savcmp() noexcept {
#ifdef _DEBUG

	return 1;

#else

	return !StateMap.test(StateFlag::CMPDO);

#endif
}

void thred::internal::thr2bal(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code) {
#define BALRAT 1.6666666666667

	balaradStitch[destination].flag = 0;
	balaradStitch[destination].code = gsl::narrow<unsigned char>(code);
	balaradStitch[destination].x    = (StitchBuffer[source].x - BalaradOffset.x) * BALRAT;
	balaradStitch[destination].y    = (StitchBuffer[source].y - BalaradOffset.y) * BALRAT;
}

constexpr unsigned thred::internal::coldis(COLORREF colorA, COLORREF colorB) {
	unsigned distance = 0;

	distance = ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	colorA >>= 8;
	colorB >>= 8;
	distance += ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	colorA >>= 8;
	colorB >>= 8;
	distance += ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	return distance;
}

void thred::internal::bal2thr(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code) {
#define IBALRAT 0.6

	StitchBuffer[destination].attribute = code;
	StitchBuffer[destination].x         = balaradStitch[source].x * IBALRAT + BalaradOffset.x;
	StitchBuffer[destination].y         = balaradStitch[source].y * IBALRAT + BalaradOffset.y;
}

unsigned thred::internal::colmatch(COLORREF color) noexcept {
	unsigned iColor = 0, distance = 0, minDistance = 0, iDistance = 0;

	if (ColorChanges < 16) {
		for (iColor = 0; iColor < ColorChanges; iColor++) {
			if (color == UserColor[iColor])
				return iColor;
		}
		UserColor[ColorChanges++] = color;
		return ColorChanges - 1;
	}
	else {
		minDistance = 0xffffffff;
		for (iColor = 0; iColor < ColorChanges; iColor++) {
			distance = coldis(color, UserColor[iColor]);
			if (!distance)
				return iColor;
			iDistance = 0;
			if (distance < minDistance) {
				iDistance   = iColor;
				minDistance = distance;
			}
		}
		return iDistance;
	}
}

void thred::internal::redbal() {
	HANDLE        balaradFile   = {};
	BALHED        balaradHeader = {};
	unsigned long bytesRead     = 0;
	unsigned      iStitch = 0, iColor = 0, iBalaradStitch = 0, stitchCount = 0, color = 0;

	PCSHeader.stitchCount = 0;
	FormIndex             = 0;
	balaradFile           = CreateFile(BalaradName2->wstring().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (balaradFile != INVALID_HANDLE_VALUE) {
		ReadFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesRead, 0);
		if (bytesRead == sizeof(balaradHeader)) {
			std::vector<BALSTCH> balaradStitch(MAXITEMS);
			ReadFile(balaradFile, balaradStitch.data(), MAXITEMS * sizeof(balaradStitch[0]), &bytesRead, 0);
			stitchCount             = bytesRead / sizeof(balaradStitch[0]);
			IniFile.backgroundColor = BackgroundColor = balaradHeader.backgroundColor;
			BackgroundPen                             = nuPen(BackgroundPen, 1, BackgroundColor);
			BackgroundPenWidth                        = 1;
			DeleteObject(BackgroundBrush);
			BackgroundBrush = CreateSolidBrush(BackgroundColor);
			UnzoomedRect.x = IniFile.hoopSizeX = balaradHeader.hoopSizeX * IBALRAT;
			UnzoomedRect.y = IniFile.hoopSizeY = balaradHeader.hoopSizeY * IBALRAT;
			BalaradOffset.x                    = IniFile.hoopSizeX / 2;
			BalaradOffset.y                    = IniFile.hoopSizeY / 2;
			IniFile.hoopType = PCSHeader.hoopType = CUSTHUP;
			UserColor[0]                          = balaradHeader.color[0];
			color                                 = 0;
			iColor                                = 1;
			iBalaradStitch                        = 0;
			ColorChanges                          = 1;
			for (iStitch = 0; iStitch < stitchCount; iStitch++) {
				switch (balaradStitch[iStitch].code) {
				case BALNORM:
					bal2thr(balaradStitch, iBalaradStitch++, iStitch, color);
					break;
				case BALSTOP:
					color = colmatch(balaradHeader.color[iColor++]);
					break;
				}
			}
			for (iColor = 0; iColor < ColorChanges; iColor++) {
				UserPen[iColor]        = CreatePen(PS_SOLID, 1, UserColor[iColor]);
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
			}
			PCSHeader.stitchCount = iBalaradStitch;
			thred::coltab();
			thred::redraw(ColorBar);
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
	CloseHandle(balaradFile);
}

void thred::internal::ritbal() {
	BALHED        balaradHeader = {};
	unsigned      iStitch = 0, iColor = 0, iOutput = 0, color = 0;
	HANDLE        balaradFile = {};
	fs::path      outputName;
	unsigned long bytesWritten = 0;

	if (!BalaradName0->empty() && !BalaradName1->empty() && PCSHeader.stitchCount) {
		if (WorkingFileName->empty()) {
			outputName = *DefaultDirectory / L"balfil.thr";
		}
		else {
			outputName = *WorkingFileName;
		}
		outputName.replace_extension(L".thv");
		balaradFile = CreateFile(outputName.wstring().c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (balaradFile == INVALID_HANDLE_VALUE)
			return;
		color                  = StitchBuffer[0].attribute & COLMSK;
		balaradHeader.color[0] = UserColor[color];
		iColor                 = 1;
		for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (color != (StitchBuffer[iStitch].attribute & COLMSK)) {
				color                         = StitchBuffer[iStitch].attribute & COLMSK;
				balaradHeader.color[iColor++] = UserColor[color];
				if (iColor & 0xffffff00)
					break;
			}
		}
		balaradHeader.signature       = 'drbm';
		balaradHeader.backgroundColor = BackgroundColor;
		balaradHeader.hoopSizeX       = IniFile.hoopSizeX * BALRAT;
		balaradHeader.hoopSizeY       = IniFile.hoopSizeY * BALRAT;
		WriteFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesWritten, 0);
		BalaradOffset.x = IniFile.hoopSizeX / 2;
		BalaradOffset.y = IniFile.hoopSizeY / 2;
		std::vector<BALSTCH> balaradStitch(PCSHeader.stitchCount + 2);
		color   = StitchBuffer[0].attribute & COLMSK;
		iOutput = 0;
		thr2bal(balaradStitch, iOutput++, 0, BALJUMP);
		balaradStitch[iOutput].flag = gsl::narrow<unsigned char>(color);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount && iOutput < 2; iStitch++) {
			thr2bal(balaradStitch, iOutput++, iStitch, BALNORM);
			if ((StitchBuffer[iStitch].attribute & COLMSK) != color) {
				thr2bal(balaradStitch, iOutput, iStitch, BALSTOP);
				color                         = StitchBuffer[iStitch].attribute & COLMSK;
				balaradStitch[iOutput++].flag = gsl::narrow<unsigned char>(color);
			}
		}
		WriteFile(balaradFile, balaradStitch.data(), iOutput * sizeof(balaradStitch[0]), &bytesWritten, 0);
		CloseHandle(balaradFile);
		balaradFile = CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		WriteFileInt(balaradFile, outputName.wstring().c_str(), outputName.wstring().size() + 1, &bytesWritten, 0);
		CloseHandle(balaradFile);
	}
	else {
		if (!BalaradName1->empty()) {
			balaradFile = CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
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

bool thred::internal::chkattr(const fs::path& filename) {
	constexpr auto writeBits = (fs::perms::owner_write | fs::perms::others_write | fs::perms::group_write);

	if (!fs::space(filename.parent_path()).available) {
		StateMap.set(StateFlag::NOTFREE);
		return 1;
	}
	else {
		StateMap.reset(StateFlag::NOTFREE);
		const auto writePerms = fs::status(filename).permissions() & writeBits;
		if (writePerms == fs::perms::none) {
			auto buttonPressed = MessageBox(ThrEdWindow,
			                                fmt::format((*StringTable)[STR_OVRLOK], filename.wstring()).c_str(),
			                                (*StringTable)[STR_OVRIT].c_str(),
			                                MB_YESNO);
			if (buttonPressed == IDYES)
				fs::permissions(filename, fs::perms::add_perms | writeBits);
			else
				return 1;
		}
	}
	return 0;
}

void thred::internal::duver(const fs::path& name) {
	auto version = tolower(name.extension().wstring().back()) - 'r';
	if (version >= 0 && version <= (OLDVER - 1)) {
		VersionNames[version] = name;
	}
}

void thred::internal::durit(char** destination, const void* const source, size_t count) noexcept {
	if (destination && source) {
		[[gsl::suppress(26474)]] memcpy(static_cast<void*>(*destination), source, count);
		*destination += count;
	}
}

void thred::internal::dubuf(char* const buffer, unsigned& count) {
	STRHED   stitchHeader = {};
	unsigned iVertex = 0, iGuide = 0, iClip = 0;
	size_t   vertexCount = 0;
	size_t   guideCount = 0, clipDataCount = 0;
	char*    output = buffer;

	stitchHeader.headerType  = 0x2746872;
	stitchHeader.fileLength  = PCSHeader.stitchCount * sizeof(StitchBuffer[0]) + sizeof(stitchHeader) + sizeof(PCSBMPFileName);
	stitchHeader.stitchCount = PCSHeader.stitchCount;
	stitchHeader.hoopType    = IniFile.hoopType;
	auto designer            = utf::Utf16ToUtf8(*DesignerName);
	std::copy(designer.begin(), designer.end(), ExtendedHeader.modifierName);
	if (FormIndex) {
		for (size_t iForm = 0; iForm < FormIndex; iForm++) {
			vertexCount += FormList[iForm].vertexCount;
			if (FormList[iForm].type == SAT)
				guideCount += FormList[iForm].satinGuideCount;
			if (clip::isclp(iForm))
				clipDataCount += FormList[iForm].lengthOrCount.clipCount;
			if (clip::iseclp(iForm))
				clipDataCount += FormList[iForm].clipEntries;
		}
	}
	stitchHeader.formCount     = gsl::narrow<unsigned short>(FormIndex);
	stitchHeader.vertexCount   = gsl::narrow<unsigned short>(vertexCount);
	stitchHeader.dlineCount    = gsl::narrow<unsigned short>(guideCount);
	stitchHeader.clipDataCount = gsl::narrow<unsigned short>(clipDataCount);
	constexpr auto threadLength
	    = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0])) / 2; // ThreadSize is defined as a 16 entry array of 2 bytes
	constexpr auto formDataOffset
	    = sizeof(PCSBMPFileName) + sizeof(BackgroundColor) + sizeof(UserColor) + sizeof(CustomColor) + threadLength;
	// ToDo - vertexLength overflows if there are more than 5446 stitches, so clamp it until version 3
	auto vtxLen = sizeof(stitchHeader) + PCSHeader.stitchCount * sizeof(StitchBuffer[0]) + formDataOffset;
	if (vtxLen > USHRT_MAX) {
		vtxLen = USHRT_MAX;
	}
	stitchHeader.vertexLen   = gsl::narrow<unsigned short>(vtxLen);
	stitchHeader.dlineLen    = gsl::narrow<unsigned short>(sizeof(FormVertices[0]) * vertexCount);
	stitchHeader.clipDataLen = gsl::narrow<unsigned short>(sizeof(ClipPoints[0]) * clipDataCount);
	durit(&output, &stitchHeader, sizeof(stitchHeader));
	ExtendedHeader.auxFormat         = IniFile.auxFileType;
	ExtendedHeader.hoopSizeX         = IniFile.hoopSizeX;
	ExtendedHeader.hoopSizeY         = IniFile.hoopSizeY;
	ExtendedHeader.texturePointCount = gsl::narrow<unsigned int>(TextureIndex);
	durit(&output, &ExtendedHeader, sizeof(ExtendedHeader));
	durit(&output, StitchBuffer, PCSHeader.stitchCount * sizeof(StitchBuffer[0]));
	if (!PCSBMPFileName[0]) {
		std::fill_n(PCSBMPFileName, sizeof(PCSBMPFileName), '\0');
	}
	durit(&output, PCSBMPFileName, sizeof(PCSBMPFileName));
	durit(&output, &BackgroundColor, sizeof(BackgroundColor));
	durit(&output, UserColor, sizeof(UserColor));
	durit(&output, CustomColor, sizeof(CustomColor));
	for (auto iThread = 0; iThread < threadLength; iThread++) {
		MsgBuffer[iThread] = ThreadSize[iThread][0];
	}
	MsgBuffer[threadLength] = 0;
	std::wstring threadSizeBufW(MsgBuffer);
	auto         threadSizeBuf = utf::Utf16ToUtf8(threadSizeBufW);
	durit(&output, threadSizeBuf.c_str(), threadSizeBuf.size() * sizeof(threadSizeBuf[0]));
	if (FormIndex) {
		std::vector<FRMHEDOUT> forms;
		forms.reserve(FormIndex);
		std::vector<fPOINT> vertices;
		vertices.reserve(vertexCount);
		std::vector<SATCONOUT> guides;
		guides.reserve(guideCount);
		std::vector<fPOINT> points;
		points.reserve(clipDataCount);
		for (size_t iForm = 0; iForm < FormIndex; iForm++) {
			forms.push_back(FormList[iForm]);
			forms[iForm].vertices = 0;
			for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++) {
				vertices.push_back(FormList[iForm].vertices[iVertex]);
			}
			if (FormList[iForm].type == SAT) {
				forms[iForm].satinGuideCount = gsl::narrow<unsigned short>(FormList[iForm].satinGuideCount);
				for (iGuide = 0; iGuide < FormList[iForm].satinGuideCount; iGuide++) {
					guides.push_back(FormList[iForm].satinOrAngle.guide[iGuide]);
				}
			}
			if (clip::isclp(iForm)) {
				for (iClip = 0; iClip < FormList[iForm].lengthOrCount.clipCount; iClip++) {
					points.push_back(FormList[iForm].angleOrClipData.clip[iClip]);
				}
			}
			if (clip::iseclpx(iForm)) {
				for (iClip = 0; iClip < FormList[iForm].clipEntries; iClip++) {
					points.push_back(FormList[iForm].borderClipData[iClip]);
				}
			}
		}
		if (forms.size()) {
			durit(&output, forms.data(), forms.size() * sizeof(forms[0]));
		}
		if (vertices.size()) {
			durit(&output, vertices.data(), vertices.size() * sizeof(vertices[0]));
		}
		if (guides.size()) {
			durit(&output, guides.data(), guides.size() * sizeof(guides[0]));
		}
		if (points.size()) {
			durit(&output, points.data(), points.size() * sizeof(points[0]));
		}
		if (TextureIndex) {
			durit(&output, TexturePointsBuffer->data(), TextureIndex * sizeof((*TexturePointsBuffer)[0]));
		}
	}
	count = output - buffer;
}

void thred::internal::thrsav() {
	unsigned        iVersion     = 0;
	int             iBackup      = 0;
	unsigned long   bytesWritten = 0;
	WIN32_FIND_DATA fileData     = {};
	HANDLE          file         = {};
	fs::path        newFileName;
	unsigned        count = 0;

	if (chkattr(*WorkingFileName))
		return;
	if (!StateMap.testAndReset(StateFlag::IGNAM)) {
		file     = FindFirstFile(GeName->wstring().c_str(), &fileData);
		iVersion = 0;
		if (file != INVALID_HANDLE_VALUE) {
			StateMap.reset(StateFlag::CMPDO);
			for (auto& version : VersionNames) {
				version.clear();
			}
			duver(*DefaultDirectory / fileData.cFileName);
			while (FindNextFile(file, &fileData))
				duver(*DefaultDirectory / fileData.cFileName);
			FindClose(file);
			fs::remove(VersionNames[OLDVER - 1]);
			for (iBackup = OLDVER - 2; iBackup >= 0; iBackup--) {
				if (!VersionNames[iBackup].empty()) {
					newFileName = VersionNames[iBackup];
					auto ext    = newFileName.extension().wstring();
					ext.back()  = iBackup + 's';
					newFileName.replace_extension(ext);
					fs::rename(VersionNames[iBackup], newFileName);
				}
			}
		}
	}
	FileHandle = CreateFile(ThrName->wstring().c_str(), (GENERIC_WRITE), 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE) {
		displayText::crmsg(*ThrName);
		FileHandle = 0;
	}
	else {
		// ToDo - MAXITEMS * 8 is not the best option here. Need something better
		auto output = std::vector<char>(MAXITEMS * 8);
		dubuf(output.data(), count);
		WriteFile(FileHandle, output.data(), count, &bytesWritten, 0);
		if (bytesWritten != count) {
			std::wstring fmtStr;
			displayText::loadString(fmtStr, IDS_FWERR);
			displayText::shoMsg(fmt::format(fmtStr, ThrName->wstring()));
		}
		CloseHandle(FileHandle);
	}
}

void thred::internal::chk1col() {
	unsigned iColorChange = 0, iStitch = 0, color = 0;

	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
	for (iColorChange = 0; iColorChange < ColorChanges; iColorChange++) {
		if (ColorChangeTable[iColorChange + 1].stitchIndex - ColorChangeTable[iColorChange].stitchIndex == 1) {
			iStitch = ColorChangeTable[iColorChange].stitchIndex;
			if (iStitch) {
				color = StitchBuffer[iStitch - 1].attribute & COLMSK;
			}
			else {
				color = StitchBuffer[iStitch].attribute & COLMSK;
			}
			StitchBuffer[iStitch].attribute &= NCOLMSK;
			StitchBuffer[iStitch].attribute |= color;
		}
	}
}

void thred::internal::savdst(std::vector<DSTREC>& DSTRecords, unsigned data) {
	union {
		unsigned data;
		DSTREC   dstRecord;
	} x;

	x.data = data;

	DSTRecords.push_back(x.dstRecord);
}

constexpr unsigned thred::internal::dudbits(const POINT& dif) {
	return Xdst[dif.x + 121] | Ydst[dif.y + 121];
}

bool thred::internal::colfil() {
	*ColorFileName = *WorkingFileName;
	*RGBFileName   = *WorkingFileName;
	if (WorkingFileName->has_extension()) {
		ColorFileName->replace_extension(L"thw");
		RGBFileName->replace_extension(L"rgb");
		return 1;
	}
	else
		return 0;
}

void thred::internal::ritdst(DSTOffsets& DSTOffsetData, std::vector<DSTREC>& DSTRecords, const std::vector<fPOINTATTR>& stitches) {
#define DSTMAX 121
	unsigned                iStitch = 0, dstType = 0, color = 0, count = 0;
	POINT                   centerCoordinate = {}, lengths = {}, absoluteLengths = {}, difference = {}, stepSize = {};
	fRECTANGLE              boundingRect = {};
	std::vector<fPOINTATTR> dstStitchBuffer(PCSHeader.stitchCount);
	std::vector<unsigned>   colorData;
	// there could be as many colors as there are stitches
	// but we only need to reserve a reasonable number
	colorData.reserve(32);
	HANDLE        colorFile    = {};
	unsigned long bytesWritten = 0;

	colorData.push_back(COLVER);
	colorData.push_back(BackgroundColor);
	colorData.push_back(UserColor[stitches[0].attribute & COLMSK]);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		dstStitchBuffer[iStitch].x         = stitches[iStitch].x * 5 / 3;
		dstStitchBuffer[iStitch].y         = stitches[iStitch].y * 5 / 3;
		dstStitchBuffer[iStitch].attribute = stitches[iStitch].attribute;
	}
	boundingRect.left = boundingRect.right = dstStitchBuffer[0].x;
	boundingRect.bottom = boundingRect.top = dstStitchBuffer[0].y;
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (dstStitchBuffer[iStitch].x > boundingRect.right)
			boundingRect.right = dstStitchBuffer[iStitch].x + 0.5;
		if (dstStitchBuffer[iStitch].x < boundingRect.left)
			boundingRect.left = dstStitchBuffer[iStitch].x - 0.5;
		if (dstStitchBuffer[iStitch].y > boundingRect.top)
			boundingRect.top = dstStitchBuffer[iStitch].y + 0.5;
		if (dstStitchBuffer[iStitch].y < boundingRect.bottom)
			boundingRect.bottom = dstStitchBuffer[iStitch].y - 0.5;
	}
	centerCoordinate.x       = (boundingRect.right - boundingRect.left) / 2 + boundingRect.left;
	centerCoordinate.y       = (boundingRect.top - boundingRect.bottom) / 2 + boundingRect.bottom;
	DSTOffsetData.Positive.x = boundingRect.right - centerCoordinate.x + 1;
	DSTOffsetData.Positive.y = boundingRect.top - centerCoordinate.y + 1;
	DSTOffsetData.Negative.x = centerCoordinate.x - boundingRect.left - 1;
	DSTOffsetData.Negative.y = centerCoordinate.y - boundingRect.bottom - 1;
	color                    = dstStitchBuffer[0].attribute & 0xf;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (color != (dstStitchBuffer[iStitch].attribute & 0xf)) {
			savdst(DSTRecords, 0xc30000);
			color = dstStitchBuffer[iStitch].attribute & 0xf;
			colorData.push_back(UserColor[color]);
		}
		lengths.x         = dstStitchBuffer[iStitch].x - centerCoordinate.x;
		lengths.y         = dstStitchBuffer[iStitch].y - centerCoordinate.y;
		absoluteLengths.x = abs(lengths.x);
		absoluteLengths.y = abs(lengths.y);
		if (absoluteLengths.x > absoluteLengths.y)
			count = absoluteLengths.x / DSTMAX + 1;
		else
			count = absoluteLengths.y / DSTMAX + 1;
		stepSize.x = absoluteLengths.x / count + 1;
		stepSize.y = absoluteLengths.y / count + 1;
		while (lengths.x || lengths.y) {
			dstType = REGTYP;
			if (abs(lengths.x) > stepSize.x) {
				dstType = JMPTYP;
				if (lengths.x > 0)
					difference.x = stepSize.x;
				else
					difference.x = -stepSize.x;
			}
			else
				difference.x = lengths.x;
			if (abs(lengths.y) > stepSize.y) {
				dstType = JMPTYP;
				if (lengths.y > 0)
					difference.y = stepSize.y;
				else
					difference.y = -stepSize.y;
			}
			else
				difference.y = lengths.y;
			savdst(DSTRecords, dudbits(difference) | dstType);
			centerCoordinate.x += difference.x;
			centerCoordinate.y += difference.y;
			lengths.x -= difference.x;
			lengths.y -= difference.y;
		}
	}
	DSTRecords.push_back({ 0, 0, gsl::narrow<unsigned char>(0xf3) });

	if (colfil()) {
		colorFile = CreateFile(ColorFileName->wstring().c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (colorFile != INVALID_HANDLE_VALUE)
			WriteFileInt(colorFile, &colorData[0], colorData.size() * sizeof(colorData[0]), &bytesWritten, 0);
		CloseHandle(colorFile);
		colorFile = CreateFile(RGBFileName->wstring().c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (colorFile != INVALID_HANDLE_VALUE)
			WriteFileInt(colorFile, &colorData[2], (colorData.size() - 2) * sizeof(colorData[0]), &bytesWritten, 0);
		CloseHandle(colorFile);
	}
}

bool thred::internal::pcshup(std::vector<fPOINTATTR>& stitches) {
	fRECTANGLE boundingRect = { stitches[0].y, stitches[0].x, stitches[0].x, stitches[0].y };
	fPOINT     boundingSize = {};
	fPOINT     hoopSize     = {};
	fPOINT     delta        = {};

	for (auto stitch : stitches) {
		if (stitch.x < boundingRect.left)
			boundingRect.left = stitch.x;
		if (stitch.x > boundingRect.right)
			boundingRect.right = stitch.x;
		if (stitch.y < boundingRect.bottom)
			boundingRect.bottom = stitch.y;
		if (stitch.y > boundingRect.top)
			boundingRect.top = stitch.y;
	}
	boundingSize.x = boundingRect.right - boundingRect.left;
	boundingSize.y = boundingRect.top - boundingRect.bottom;
	if (boundingSize.x > LHUPX || boundingSize.y > LHUPY) {
		displayText::tabmsg(IDS_PFAF2L);
		return true;
	}
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
	delta = {};
	if (boundingRect.right > hoopSize.x)
		delta.x = hoopSize.x - boundingRect.right;
	if (boundingRect.top > hoopSize.y)
		delta.y = hoopSize.y - boundingRect.top;
	if (boundingRect.left < 0)
		delta.x = -boundingRect.left;
	if (boundingRect.bottom < 0)
		delta.y = -boundingRect.bottom;
	if (delta.x || delta.y) {
		for (auto offsetStitch : stitches) {
			offsetStitch.x += delta.x;
			offsetStitch.y += delta.y;
		}
	}
	return false;
}

#if PESACT

unsigned pesmtch(COLORREF referenceColor, unsigned char colorIndex) noexcept {
	unsigned colorDistance = 0, iRGB = 0;
	COLORREF color = {}, translatedColor = {};

	color           = referenceColor;
	translatedColor = PESColorTranslate[colorIndex];
	colorDistance   = 0;
	for (iRGB = 0; iRGB < 3; iRGB++) {
		colorDistance += ((color & 0xff) > (translatedColor & 0xff)) ? ((color & 0xff) - (translatedColor & 0xff))
		                                                             : ((translatedColor & 0xff) - (color & 0xff));
		translatedColor >>= 8;
		color >>= 8;
	}
	return colorDistance;
}

void thred::internal::ritpes(unsigned iStitch, const std::vector<fPOINTATTR>& stitches) {
	PESstitches[OutputIndex].x   = -stitches[iStitch].x * 3 / 5 + PESstitchCenterOffset.x;
	PESstitches[OutputIndex++].y = stitches[iStitch].y * 3 / 5 - PESstitchCenterOffset.y;
}

void thred::internal::ritpcol(unsigned char colorIndex) noexcept {
	// ToDo - (PES) Complete translation from assembler
	_asm {
		mov		ebx, OutputIndex
		mov		eax, ebx
		inc		eax
		mov		OutputIndex, eax
		shl		ebx, 2
		add		ebx, PESstitches
		xor		eax, eax
		mov		al, colorIndex
		mov[ebx], eax
	}
}

unsigned pesnam() noexcept {
	// ToDo - (PES) Complete translation from assembler
	_asm {
		mov		ebx, offset AuxName
		mov		ecx, _MAX_PATH
		mov		edx, ebx
peslup0:
		mov		al, [ebx]
		or		al, al
		je		short peslup1
		cmp		al, '\\'
		jne		short peslup0a
		mov		edx, ebx
peslup0a:
		inc		ebx
		loop	peslup0
peslup1:
		mov		ebx, edx
		cmp		byte ptr[ebx], '\\'
		jne		short peslup1a
		inc		ebx
peslup1a:
		xor		ecx, ecx
		mov		cl, 17
		mov		edi, offset BSequence
		mov		dword ptr[edi], ':AL'
		add		edi, 3
peslup:
		mov		al, [ebx]
		inc		ebx
		cmp		al, '.'
		je		pesnamx
		mov		[edi], al
		inc		edi
		loop	peslup
pesnamx:
		mov		eax, edi
		sub		eax, offset BSequence
	}
}

void thred::internal::rpcrd(float stitchDelta) noexcept {
	int pesDelta = 0;

	pesDelta = stitchDelta * 5 / 3;
	if (pesDelta < 0) {
		if (pesDelta > -64) {
			PESdata[OutputIndex] = pesDelta - 128;
			OutputIndex++;
		}
		else {
			PESdata[OutputIndex] = pesDelta >> 8;
			OutputIndex++;
			PESdata[OutputIndex] = pesDelta & 0x8f;
			OutputIndex++;
		}
	}
	else {
		if (pesDelta < 64) {
			PESdata[OutputIndex] = pesDelta;
			OutputIndex++;
		}
		else {
			PESdata[OutputIndex] = (pesDelta >> 8) | 0x80;
			OutputIndex++;
			PESdata[OutputIndex] = pesDelta & 0xff;
			OutputIndex++;
		}
	}
}

void thred::internal::pecdat(unsigned char* buffer) {
	unsigned iStitch = 0;
	unsigned color   = StitchBuffer[0].attribute & COLMSK;
	unsigned iColor  = 1;

	OutputIndex = 532;
	PESdata     = buffer;
	PEScolors   = convert_ptr<unsigned char*>(&PESdata[49]);
	rpcrd(StitchBuffer[0].x);
	rpcrd(-StitchBuffer[0].y);
	for (iStitch = 0; iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & COLMSK) != color) {
			color                = StitchBuffer[iStitch].attribute & COLMSK;
			PESdata[OutputIndex] = 254;
			OutputIndex++;
			PESdata[OutputIndex] = 176;
			OutputIndex++;
			PESdata[OutputIndex] = iColor;
			OutputIndex++;
			PEScolors[iColor] = PESequivColors[color];
			iColor++;
		}
		rpcrd(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x);
		rpcrd(-StitchBuffer[iStitch + 1].y + StitchBuffer[iStitch].y);
	}
	PESdata[OutputIndex++] = 0xff;
	PESdata[OutputIndex++] = 0;
}

#endif

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void thred::internal::sav() {
	unsigned      iStitch = 0, iColor = 0, iHeader = 0, iPCSstitch = 0;
	unsigned long bytesWritten   = 0;
	double        fractionalPart = 0.0, integerPart = 0.0;
	DSTHED        dstHeader = {};
#if PESACT
	unsigned char* pchr = nullptr;
#endif
	unsigned savcol = 0;

#if PESACT

	PESHED        pesHeader    = {};
	unsigned      match        = 0;
	unsigned      matchIndex   = 0;
	unsigned      matchMin     = 0;
	unsigned char pesColor     = 0;
	fRECTANGLE    boundingRect = {};
	unsigned      color = 0, iColorMatch = 0;
	unsigned      pesColorCount = 0;
	unsigned*     pesOffset     = nullptr;
	unsigned*     upnt          = nullptr;
	short*        psiz          = nullptr;
#endif

	xt::duauxnam();
	if (chkattr(*AuxName))
		return;
	if (!PCSHeader.stitchCount)
		return;
	chk1col();
	thred::coltab();
	std::vector<fPOINTATTR> saveStitches(PCSHeader.stitchCount);
	if (UserFlagMap.test(UserFlag::ROTAUX)) {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			saveStitches[iStitch] = StitchBuffer[iStitch];
		}
	}
	else {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			saveStitches[iStitch] = StitchBuffer[iStitch];
		}
	}
	PCSFileHandle = CreateFile(AuxName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (PCSFileHandle == INVALID_HANDLE_VALUE) {
		displayText::crmsg(*AuxName);
		PCSFileHandle = 0;
	}
	else {
		bool                flag = true;
		std::vector<DSTREC> DSTRecords;
		// There are always going to be more records in the DST format because color changes and jumps count as stitches
		DSTRecords.reserve(PCSHeader.stitchCount + 128);
		DSTOffsets          DSTOffsetData = {};
		std::vector<PCSTCH> PCSStitchBuffer;
		auto                auxName = utf::Utf16ToUtf8(*AuxName);
		const char*         desc    = strrchr(auxName.data(), '\\') + 1;
		switch (IniFile.auxFileType) {
		case AUXDST:
			ritdst(DSTOffsetData, DSTRecords, saveStitches);
			// dstHeader fields are fixed width, so use strncpy in its intended way.
			// Use sizeof to ensure no overrun if the format string is wrong length
			strncpy(dstHeader.desched, "LA:", sizeof(dstHeader.desched));
			std::fill_n(dstHeader.desc, sizeof(dstHeader.desc), ' ');
			if (desc) {
				for (iHeader = 0; iHeader < sizeof(dstHeader.desc); iHeader++) {
					if (desc[iHeader] && desc[iHeader] != '.')
						dstHeader.desc[iHeader] = desc[iHeader];
					else
						break;
				}
			}
			dstHeader.desc[16] = 0xd;
			strncpy(dstHeader.recshed, "ST:", sizeof(dstHeader.recshed));
			strncpy(dstHeader.recs, fmt::format("{:7d}\r", DSTRecords.size()).c_str(), sizeof(dstHeader.recs));
			strncpy(dstHeader.cohed, "CO:", sizeof(dstHeader.cohed));
			strncpy(dstHeader.co, "  0\xd", sizeof(dstHeader.co));
			strncpy(dstHeader.xplushed, "+X:", sizeof(dstHeader.xplushed));
			strncpy(dstHeader.xplus, fmt::format("{:5d}\xd", DSTOffsetData.Negative.x).c_str(), sizeof(dstHeader.xplus));
			strncpy(dstHeader.xminhed, "-X:", sizeof(dstHeader.xminhed));
			strncpy(dstHeader.xmin, fmt::format("{:5d}\xd", DSTOffsetData.Positive.x).c_str(), sizeof(dstHeader.xmin));
			strncpy(dstHeader.yplushed, "+Y:", sizeof(dstHeader.yplushed));
			strncpy(dstHeader.yplus, fmt::format("{:5d}\xd", DSTOffsetData.Positive.y).c_str(), sizeof(dstHeader.yplus));
			strncpy(dstHeader.yminhed, "-Y:", sizeof(dstHeader.yminhed));
			strncpy(dstHeader.ymin, fmt::format("{:5d}\xd", DSTOffsetData.Negative.y).c_str(), sizeof(dstHeader.ymin));
			strncpy(dstHeader.axhed, "AX:", sizeof(dstHeader.axhed));
			strncpy(dstHeader.ax, "-    0\r", sizeof(dstHeader.ax));
			strncpy(dstHeader.ayhed, "AY:", sizeof(dstHeader.ayhed));
			strncpy(dstHeader.ay, "+    0\r", sizeof(dstHeader.ay));
			strncpy(dstHeader.mxhed, "MX:", sizeof(dstHeader.mxhed));
			strncpy(dstHeader.mx, "+    0\r", sizeof(dstHeader.mx));
			strncpy(dstHeader.myhed, "MY:", sizeof(dstHeader.myhed));
			strncpy(dstHeader.my, "+    0\r", sizeof(dstHeader.my));
			strncpy(dstHeader.pdhed, "PD", sizeof(dstHeader.pdhed));
			strncpy(dstHeader.pd, "******\r", sizeof(dstHeader.pd));
			strncpy(dstHeader.eof, "\x1a", sizeof(dstHeader.eof));
			std::fill_n(dstHeader.res, sizeof(dstHeader.res), ' ');
			WriteFile(PCSFileHandle, &dstHeader, sizeof(dstHeader), &bytesWritten, 0);
			WriteFileInt(PCSFileHandle, DSTRecords.data(), sizeof(DSTRecords[0]) * DSTRecords.size(), &bytesWritten, 0);
			break;
#if PESACT
		case AUXPES:
			pchr = convert_ptr<unsigned char*>(&pesHeader);
			for (iHeader = 0; iHeader < sizeof(pesHeader); iHeader++)
				pchr[iHeader] = 0;
			strncpy(pesHeader.led, "#PES0001", sizeof(pesHeader.led));
			strncpy(pesHeader.ce, "CEmbOne", sizeof(pesHeader.ce));
			strncpy(pesHeader.cs, "CSewSeg", sizeof(pesHeader.cs));
			for (iColor = 0; iColor < 16; iColor++) {
				matchMin = 0xffffffff;
				for (iColorMatch = 0; iColorMatch<sizeof(PESColorTranslate)>> 2; iColorMatch++) {
					match = pesmtch(UserColor[iColor], iColorMatch);
					if (match < matchMin) {
						matchIndex = iColorMatch;
						matchMin   = match;
					}
				}
				PESequivColors[iColor] = gsl::narrow<unsigned char>(matchIndex);
			}
			color    = StitchBuffer[0].attribute & COLMSK;
			pesColor = pesHeader.scol = PESequivColors[StitchBuffer[0].attribute & COLMSK];
			sizstch(boundingRect, StitchBuffer);
			PESstitchCenterOffset.x = midl(boundingRect.right, boundingRect.left);
			PESstitchCenterOffset.y = midl(boundingRect.top, boundingRect.bottom);
			pesHeader.xsiz          = boundingRect.right - boundingRect.left;
			pesHeader.ysiz          = boundingRect.top - boundingRect.bottom;
			OutputIndex             = 0;
			// There cannot be more color changes than stitches
			PESstitches = new PESTCH[PCSHeader.stitchCount * 2];
			ritpes(0, saveStitches);
			PESstitches[OutputIndex].x   = -32765; // 0x8003
			PESstitches[OutputIndex++].y = 0;
			ritpcol(PESequivColors[color]);
			ritpes(0, saveStitches);
			pesColorCount = 0;
			for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (color == (StitchBuffer[iStitch].attribute & COLMSK))
					ritpes(iStitch, saveStitches);
				else {
					ritpes(iStitch, saveStitches);
					PESstitches[OutputIndex].x   = -32767; // 0x8001
					PESstitches[OutputIndex++].y = 0;
					ritpcol(PESequivColors[color]);
					color = StitchBuffer[iStitch].attribute & COLMSK;
					ritpes(iStitch++, saveStitches);
					ritpes(iStitch, saveStitches);
					PESstitches[OutputIndex].x   = -32765; // 0x8003
					PESstitches[OutputIndex++].y = 0;
					ritpcol(PESequivColors[color]);
					ritpes(iStitch, saveStitches);
					pesColorCount++;
				}
			}
			PESstitches[OutputIndex].x   = iColorMatch;
			PESstitches[OutputIndex++].y = 0;
			pesOffset                    = convert_ptr<unsigned*>(&pesHeader.off);
			*pesOffset                   = (OutputIndex << 2) + sizeof(pesHeader);
			*pesHeader.m1                = 0x20;
			GroupStartStitch             = 0;
			GroupEndStitch               = PCSHeader.stitchCount - 1;
			pesHeader.xsiz               = 10000;
			pesHeader.ysiz               = 10000;
			WriteFile(PCSFileHandle, convert_ptr<PESHED*>(&pesHeader), sizeof(pesHeader), &bytesWritten, 0);
			WriteFile(PCSFileHandle, PESstitches, OutputIndex * sizeof(PESstitches[0]), &bytesWritten, 0);
			delete[] PESstitches;
			// ToDo - (PES) is there a better estimate for data size?
			pchr = new unsigned char[MAXITEMS * 4];
			// ToDo - (PES) Add buffer parameter and remove use of BSequence in pesname
			iHeader = pesnam();
			while (iHeader < 512)
				pchr[iHeader++] = ' ';
			pchr[19] = 13;
			pchr[48] = gsl::narrow<unsigned char>(pesColorCount);
			pecdat(pchr);
			upnt      = convert_ptr<unsigned*>(&pchr[514]);
			*upnt     = OutputIndex - 512;
			pchr[517] = 0x20;
			pchr[518] = 0xff;
			pchr[519] = 0xef;
			psiz      = convert_ptr<short*>(&pchr[520]);
			*psiz     = pesHeader.xsiz;
			psiz++;
			*psiz = pesHeader.ysiz;
			psiz++;
			*psiz      = 480;
			pesOffset  = convert_ptr<unsigned*>(psiz);
			*pesOffset = 11534816;
			//			pchr[527]=(char)0x0;
			//			pchr[528]=(char)0x90;
			//			pchr[529]=(char)0x0;
			//			pchr[530]=(char)0x8f;
			pchr[527] = 0x00;
			pchr[528] = 0x80; // hor msb
			pchr[529] = 0x80; // hor lsb
			pchr[530] = 0x82; // vert msb
			pchr[531] = 0xff; // vert lsb
			WriteFile(PCSFileHandle, pchr, OutputIndex, &bytesWritten, 0);
			delete[] pchr;
			break;
#endif
		default:
			for (iColor = 0; iColor < 16; iColor++) {
				PCSHeader.colors[iColor] = UserColor[iColor];
			}
			do {
				if (pcshup(saveStitches)) {
					flag = false;
					break;
				}
				if (!WriteFile(PCSFileHandle, &PCSHeader, 0x46, &bytesWritten, 0)) {
					displayText::riter();
					flag = false;
					break;
				}
				iPCSstitch = 0;
				savcol     = 0xff;
				PCSStitchBuffer.resize(PCSHeader.stitchCount + ColorChanges + 2);
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
					if ((saveStitches[iStitch].attribute & COLMSK) != savcol) {
						savcol                           = saveStitches[iStitch].attribute & COLMSK;
						PCSStitchBuffer[iPCSstitch].tag  = 3;
						PCSStitchBuffer[iPCSstitch++].fx = savcol;
					}
					fractionalPart                  = modf(saveStitches[iStitch].x, &integerPart);
					PCSStitchBuffer[iPCSstitch].fx  = fractionalPart * 256;
					PCSStitchBuffer[iPCSstitch].x   = integerPart;
					fractionalPart                  = modf(saveStitches[iStitch].y, &integerPart);
					PCSStitchBuffer[iPCSstitch].fy  = fractionalPart * 256;
					PCSStitchBuffer[iPCSstitch++].y = integerPart;
				}
				if (!WriteFile(
				        PCSFileHandle, PCSStitchBuffer.data(), iPCSstitch * sizeof(PCSStitchBuffer[0]), &bytesWritten, 0)) {
					displayText::riter();
					flag = false;
					break;
				}
				if (UserFlagMap.test(UserFlag::BSAVOF)) {
					*MsgBuffer = 0;
					if (!WriteFile(PCSFileHandle, MsgBuffer, 15, &bytesWritten, 0)) {
						displayText::riter();
						flag = false;
						break;
					}
				}
				else {
					if (!WriteFile(PCSFileHandle, PCSBMPFileName, 15, &bytesWritten, 0)) {
						displayText::riter();
						flag = false;
						break;
					}
				}
			} while (false);
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
	MENUITEMINFO filinfo = { sizeof(filinfo),
		                     MIIM_TYPE,
		                     MFT_STRING,
		                     0,
		                     0,
		                     0,
		                     0,
		                     0,
		                     0,
		                     nullptr, // dwTypeData
		                     13,
		                     0 };

	std::wstring auxMsg;

	CheckMenuItem(MainMenu, ID_AUXPCS, MF_UNCHECKED);
#if PESACT
	CheckMenuItem(MainMenu, ID_AUXPES, MF_UNCHECKED);
#else
	EnableMenuItem(MainMenu, ID_AUXPES, MF_DISABLED | MF_GRAYED);
#endif
	CheckMenuItem(MainMenu, ID_AUXDST, MF_UNCHECKED);
	switch (IniFile.auxFileType) {
	case AUXDST:
		auxMsg = fmt::format((*StringTable)[STR_AUXTXT], L"DST");
		CheckMenuItem(MainMenu, ID_AUXDST, MF_CHECKED);
		break;
	case AUXPES:
#if PESACT
		auxMsg = fmt::format((*StringTable)[STR_AUXTXT], L"PES");
		CheckMenuItem(MainMenu, ID_AUXPES, MF_CHECKED);
		break;
#else
		IniFile.auxFileType = AUXPCS;
#endif
	default:
		auxMsg = fmt::format((*StringTable)[STR_AUXTXT], L"PCS");
		CheckMenuItem(MainMenu, ID_AUXPCS, MF_CHECKED);
	}
	[[gsl::suppress(type .3)]] {
		filinfo.dwTypeData = const_cast<LPWSTR>(auxMsg.c_str());
		SetMenuItemInfo(FileMenu, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
	}
	StateMap.set(StateFlag::DUMEN);
}

void thred::internal::savAs() {
	if (PCSHeader.stitchCount || FormIndex || *PCSBMPFileName) {
		wchar_t szFileName[_MAX_PATH] = { 0 };
		wchar_t lpszBuffer[_MAX_PATH] = { 0 };
		auto    workingFileStr        = WorkingFileName->wstring();
		auto    dirStr                = DefaultDirectory->wstring();
		std::copy(workingFileStr.begin(), workingFileStr.end(), szFileName);
		std::copy(dirStr.begin(), dirStr.end(), lpszBuffer);
		OpenFileName.hwndOwner       = ThrEdWindow;
		OpenFileName.hInstance       = ThrEdInstance;
		OpenFileName.lpstrFile       = szFileName;
		OpenFileName.lpstrInitialDir = lpszBuffer;
		OpenFileName.nFilterIndex    = 0;
		if (GetSaveFileName(&OpenFileName)) {
			WorkingFileName->assign(szFileName);
			switch (OpenFileName.nFilterIndex) {
			case 1:
				WorkingFileName->replace_extension(L".thr");
				break;
			case 2:
				WorkingFileName->replace_extension(L".pcs");
				IniFile.auxFileType = AUXPCS;
				auxmen();
				break;
			case 3:
#if PESACT
				WorkingFileName->replace_extension(L".pes");
				IniFile.auxFileType = AUXPES;
				auxmen();
				break;
			case 4:
#endif
				WorkingFileName->replace_extension(L".dst");
				IniFile.auxFileType = AUXDST;
				auxmen();
				break;
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
		if (PCSHeader.stitchCount)
			thi::sav();
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
	if (savcmp() || (!BalaradName0->empty()))
		reldun();
	else {
		if (StitchWindowClientRect.right) {
			displayText::savdisc();
			StateMap.set(StateFlag::SAVEX);
		}
		else {
			std::wstring fmtStr;
			displayText::loadString(fmtStr, IDS_SAVFIL);
			if (MessageBox(
			        ThrEdWindow, fmt::format(fmtStr, ThrName->wstring()).c_str(), (*StringTable)[STR_CLOS].c_str(), MB_YESNO)
			    == IDYES)
				thred::save();
			reldun();
		}
	}
}

void thred::internal::dusid(unsigned entry) noexcept {
	SideWindow[entry] = CreateWindow(L"STATIC",
	                                 SideWindowsStrings[entry].c_str(),
	                                 SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                 3,
	                                 SideWindowLocation * SideWindowSize.y + 3,
	                                 SideWindowSize.x + 3,
	                                 SideWindowSize.y,
	                                 SideMessageWindow,
	                                 NULL,
	                                 ThrEdInstance,
	                                 NULL);
	SideWindowLocation++;
}

void thred::internal::sidmsg(HWND window, std::wstring* const strings, unsigned entries) {
	if (strings) {
		RECT     childListRect  = {};
		RECT     parentListRect = {};
		unsigned iEntry = 0, entryCount = entries;

		std::fill(ValueWindow->begin(), ValueWindow->end(), nullptr);
		SideWindowSize     = {};
		SideWindowLocation = 0;
		SideWindowsStrings = strings;
		GetWindowRect(window, &childListRect);
		GetWindowRect(FormDataSheet, &parentListRect);
		form::ispcdclp();
		if (StateMap.test(StateFlag::FILTYP)) {
			for (iEntry = 0; iEntry < EDGETYPS + 1; iEntry++) {
				if (gsl::narrow<unsigned>(SelectedForm->edgeType & NEGUND) == EdgeFillTypes[iEntry])
					entryCount--;
				else {
					if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT
					    || EdgeFillTypes[iEntry] == EDGECLIPX) {
						if (StateMap.test(StateFlag::WASPCDCLP))
							formForms::maxtsiz(strings[iEntry], SideWindowSize);
						else
							entryCount--;
					}
					else
						formForms::maxtsiz(strings[iEntry], SideWindowSize);
				}
			}
			SideMessageWindow = CreateWindow(L"STATIC",
			                                 0,
			                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
			                                 parentListRect.right - ThredWindowOrigin.x + 3,
			                                 childListRect.top - ThredWindowOrigin.y - 3,
			                                 SideWindowSize.x + 12,
			                                 SideWindowSize.y * entryCount + 12,
			                                 ThrEdWindow,
			                                 NULL,
			                                 ThrEdInstance,
			                                 NULL);
			for (iEntry = 0; iEntry < entries; iEntry++) {
				if (gsl::narrow<unsigned>(SelectedForm->edgeType & NEGUND) != EdgeFillTypes[iEntry]) {
					if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT
					    || EdgeFillTypes[iEntry] == EDGECLIPX) {
						if (StateMap.test(StateFlag::WASPCDCLP))
							dusid(iEntry);
					}
					else
						dusid(iEntry);
				}
			}
		}
		else {
			if (FormMenuChoice == LLAYR) {
				std::wstring zero = L"0";
				formForms::maxtsiz(zero, SideWindowSize);
			}
			else {
				if (FormMenuChoice == LFTHTYP) {
					entryCount       = 5;
					SideWindowSize.x = ButtonWidthX3;
					SideWindowSize.y = ButtonHeight;
				}
				else {
					for (iEntry = 0; iEntry < entries; iEntry++) {
						if ((1 << FillTypes[iEntry]) & ClipTypeMap) {
							if (StateMap.test(StateFlag::WASPCDCLP))
								formForms::maxtsiz(strings[iEntry], SideWindowSize);
							else
								entryCount--;
						}
						else {
							if (FillTypes[iEntry] == SelectedForm->fillType)
								entryCount--;
							else
								formForms::maxtsiz(strings[iEntry], SideWindowSize);
						}
					}
				}
			}
			SideMessageWindow = CreateWindow(L"STATIC",
			                                 0,
			                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
			                                 parentListRect.right - ThredWindowOrigin.x + 3,
			                                 childListRect.top - ThredWindowOrigin.y - 3,
			                                 SideWindowSize.x + 12,
			                                 SideWindowSize.y * entryCount + 12,
			                                 ThrEdWindow,
			                                 NULL,
			                                 ThrEdInstance,
			                                 NULL);
			if (FormMenuChoice == LLAYR) {
				for (iEntry = 0; iEntry < entries; iEntry++)
					dusid(iEntry);
			}
			else {
				if (FormMenuChoice == LFTHTYP) {
					for (iEntry = 0; iEntry < 6; iEntry++) {
						if (FeatherFillTypes[iEntry] != SelectedForm->fillInfo.feather.fillType)
							dusid(iEntry);
					}
				}
				else {
					for (iEntry = 0; iEntry < entries; iEntry++) {
						if (FillTypes[iEntry] != SelectedForm->fillType) {
							if ((1 << FillTypes[iEntry]) & ClipTypeMap) {
								if (StateMap.test(StateFlag::WASPCDCLP))
									dusid(iEntry);
							}
							else
								dusid(iEntry);
						}
					}
				}
			}
		}
		StateMap.set(StateFlag::SIDACT);
	}
}

void thred::internal::centr() noexcept {
	POINT center
	    = { static_cast<LONG>((ZoomRect.right - ZoomRect.left) / 2), static_cast<LONG>((ZoomRect.top - ZoomRect.bottom) / 2) };

	SelectedPoint.x = ZoomRect.left + center.x;
	SelectedPoint.y = ZoomRect.bottom + center.y;
}

void thred::internal::stchWnd() {
	stchPars();

	MainStitchWin = nullptr;
	MainStitchWin = CreateWindow(L"STATIC",
	                             0,
	                             SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                             ButtonWidthX3,
	                             0,
	                             StitchWindowSize.x,
	                             StitchWindowSize.y,
	                             ThrEdWindow,
	                             NULL,
	                             ThrEdInstance,
	                             NULL);

	if (MainStitchWin != nullptr) {
		GetWindowRect(MainStitchWin, &StitchWindowAbsRect);

		VerticalScrollBar = CreateWindow(L"SCROLLBAR",
		                                 0,
		                                 SBS_VERT | WS_CHILD | WS_VISIBLE,
		                                 StitchWindowSize.x + ButtonWidthX3,
		                                 0,
		                                 SCROLSIZ,
		                                 StitchWindowSize.y,
		                                 ThrEdWindow,
		                                 NULL,
		                                 ThrEdInstance,
		                                 NULL);

		HorizontalScrollBar = CreateWindow(L"SCROLLBAR",
		                                   0,
		                                   SBS_HORZ | WS_CHILD | WS_VISIBLE,
		                                   ButtonWidthX3,
		                                   StitchWindowSize.y,
		                                   StitchWindowSize.x,
		                                   SCROLSIZ,
		                                   ThrEdWindow,
		                                   NULL,
		                                   ThrEdInstance,
		                                   NULL);
		ShowWindow(VerticalScrollBar, FALSE);
		ShowWindow(HorizontalScrollBar, FALSE);
	}
}

// check if a click occurred in A vertical set of 16 windows
// and calculate which window had the click
unsigned thred::internal::chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) noexcept {
	RECT topRect    = {};
	RECT bottomRect = {};

	GetWindowRect(topWindow, &topRect);
	GetWindowRect(bottomWindow, &bottomRect);

	if (clickCoord.x > topRect.left && clickCoord.x < bottomRect.right && clickCoord.y > topRect.top
	    && clickCoord.y < bottomRect.bottom) {
		VerticalIndex = 15 - (bottomRect.bottom - clickCoord.y) / ButtonHeight;
		return 1;
	}
	else
		return 0;
}

void thred::internal::delstch1(unsigned iStitch) {
	if (PCSHeader.stitchCount) {
		while (iStitch <= PCSHeader.stitchCount) {
			StitchBuffer[iStitch] = StitchBuffer[iStitch + 1];
			iStitch++;
		}
		PCSHeader.stitchCount--;
		if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			ClosestPointIndex = PCSHeader.stitchCount - 1;
	}
}

void thred::internal::redbak() {
	const BAKHED* undoData = convert_ptr<BAKHED*>((*UndoBuffer)[UndoBufferWriteIndex].get());
	if (undoData) {
		unsigned iColor = 0;

		if (undoData->stitchCount) {
			std::copy(undoData->stitches, undoData->stitches + undoData->stitchCount, StitchBuffer);
		}
		PCSHeader.stitchCount = undoData->stitchCount;
		UnzoomedRect          = undoData->zoomRect;
		if (undoData->formCount) {
			std::copy(undoData->forms, undoData->forms + undoData->formCount, FormList);
		}
		FormIndex = undoData->formCount;
		if (undoData->vertexCount) {
			std::copy(undoData->vertices, undoData->vertices + undoData->vertexCount, FormVertices);
		}
		FormVertexIndex = undoData->vertexCount;
		satin::cpyUndoGuides(*undoData);
		if (undoData->clipPointCount) {
			std::copy(undoData->clipPoints, undoData->clipPoints + undoData->clipPointCount, ClipPoints);
		}
		ClipPointIndex  = undoData->clipPointCount;
		auto sizeColors = (sizeof(UserColor) / sizeof(UserColor[0]));
		std::copy(undoData->colors, undoData->colors + sizeColors, UserColor);
		for (iColor = 0; iColor < sizeColors; iColor++) {
			UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
			UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
		}
		for (iColor = 0; iColor < sizeColors; iColor++)
			thred::redraw(UserColorWin[iColor]);
		TexturePointsBuffer->resize(undoData->texturePointCount);
		if (undoData->texturePointCount) {
			const auto _ = std::copy(
			    undoData->texturePoints, undoData->texturePoints + undoData->texturePointCount, TexturePointsBuffer->begin());
		}
		TextureIndex = undoData->texturePointCount;
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void thred::internal::redo() {
	unsigned nextBufferIndex = 0;

	if (StateMap.test(StateFlag::BAKACT) && StateMap.test(StateFlag::REDUSHO)) {
		UndoBufferWriteIndex++;
		UndoBufferWriteIndex &= 0xf;
		nextBufferIndex = (UndoBufferWriteIndex + 1) & 0xf;
		if (nextBufferIndex == UndoBufferReadIndex) {
			EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
			StateMap.reset(StateFlag::REDUSHO);
		}
		else {
			if (!StateMap.testAndSet(StateFlag::REDUSHO))
				EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
		}
		if (!StateMap.testAndSet(StateFlag::UNDUSHO)) {
			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
		}
		redbak();
		StateMap.set(StateFlag::DUMEN);
	}
}

void thred::internal::bak() {
	unsigned previousBufferIndex = 0;

	thred::unmsg();
	StateMap.reset(StateFlag::FPSEL);
	StateMap.reset(StateFlag::FRMPSEL);
	StateMap.reset(StateFlag::BIGBOX);
	if (StateMap.testAndReset(StateFlag::PRFACT)) {
		StateMap.reset(StateFlag::WASRT);
		DestroyWindow(PreferencesWindow);
		PreferenceIndex = 0;
		thred::unsid();
	}
	if (!StateMap.testAndSet(StateFlag::BAKING)) {
		dudat();
		UndoBufferReadIndex = UndoBufferWriteIndex + 1;
	}
	if (StateMap.test(StateFlag::BAKWRAP)) {
		UndoBufferWriteIndex--;
		UndoBufferWriteIndex &= 0xf;
		previousBufferIndex = UndoBufferWriteIndex - 1;
		if (previousBufferIndex == UndoBufferReadIndex) {
			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			StateMap.set(StateFlag::DUMEN);
			StateMap.reset(StateFlag::UNDUSHO);
		}
	}
	else {
		if (UndoBufferWriteIndex)
			UndoBufferWriteIndex--;
		if (!UndoBufferWriteIndex) {
			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			StateMap.set(StateFlag::DUMEN);
			StateMap.reset(StateFlag::UNDUSHO);
		}
	}
	if (!StateMap.testAndSet(StateFlag::REDUSHO)) {
		EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
		StateMap.set(StateFlag::DUMEN);
	}
	StateMap.reset(StateFlag::FORMSEL);
	StateMap.reset(StateFlag::GRPSEL);
	redbak();
}

void thred::internal::bitsiz() noexcept {
	const double screenAspectRatio = static_cast<double>(UnzoomedRect.x) / UnzoomedRect.y;
	const double bitmapAspectRatio = static_cast<double>(BitmapWidth) / BitmapHeight;

	if (bitmapAspectRatio > screenAspectRatio) {
		BitmapSizeinStitches.x = UnzoomedRect.x;
		BitmapSizeinStitches.y = UnzoomedRect.x / bitmapAspectRatio;
	}
	else {
		BitmapSizeinStitches.y = UnzoomedRect.y;
		BitmapSizeinStitches.x = UnzoomedRect.y * bitmapAspectRatio;
	}
	BmpStitchRatio.x = static_cast<double>(BitmapWidth) / BitmapSizeinStitches.x;
	BmpStitchRatio.y = static_cast<double>(BitmapHeight) / BitmapSizeinStitches.y;
	StitchBmpRatio.x = static_cast<double>(BitmapSizeinStitches.x) / BitmapWidth;
	StitchBmpRatio.y = static_cast<double>(BitmapSizeinStitches.y) / BitmapHeight;
}

// Get a rough estimate of whether black or white
// is dominant in the monochrome bitmap
bool thred::internal::binv(const std::vector<unsigned char>& monoBitmapData, unsigned bitmapWidthInBytes) {
	unsigned       iHeight = 0, iBytes = 0, whiteBits = 0, blackBits = 0;
	const unsigned byteCount = BitmapWidth >> 3;

	for (iHeight = 0; iHeight < BitmapHeight; iHeight++) {
		[[gsl::suppress(26429)]] {
			if ((bitmapWidthInBytes * iHeight) < monoBitmapData.size()) {
				const unsigned char* bcpnt = &monoBitmapData[bitmapWidthInBytes * iHeight];
				for (iBytes = 0; iBytes < byteCount; iBytes++) {
					if (!bcpnt[iBytes])
						blackBits++;
					else {
						if (bcpnt[iBytes] == 0xff)
							whiteBits++;
					}
				}
			}
			else {
				throw;
			}
		}
	}
	if (whiteBits > blackBits)
		return true;
	return false;
}

void thred::internal::bitlin(const unsigned char* source, unsigned* destination, COLORREF foreground, COLORREF background) {
	if (source && destination) {
		unsigned i = 0;
		for (i = 0; i < (BitmapWidth >> 3); i++) {
			std::bitset<8> bits(source[i]);
			for (unsigned bitOffset = 0; bitOffset < 8; bitOffset++) {
				*destination++ = bits[bitOffset ^ 7] ? foreground : background;
			}
		}
		if (const unsigned int final = (BitmapWidth % 8)) {
			std::bitset<8> bits(source[i]);
			for (unsigned bitOffset = final; bitOffset < 8; bitOffset++) {
				*destination++ = bits[bitOffset ^ 7] ? foreground : background;
			}
		}
	}
}

COLORREF thred::internal::fswap(COLORREF color) noexcept {
	// ToDo - find a better option than _byteswap
	return _byteswap_ulong(color) >> 8;
}

bool thred::internal::gudtyp(WORD bitCount) noexcept {
	switch (bitCount) {
	case 1:
		return 1;
	case 24:
		return 1;
	case 32:
		return 1;
	}
	return 0;
}

// Move unpacked 24BPP data into packed 24BPP data
void thred::internal::movmap(unsigned cnt, unsigned char* buffer) {
	unsigned* source = TraceBitmapData;
	if (source) {
		unsigned char* destination = buffer;

		for (unsigned i = 0; i < cnt; i++) {
			*(convert_ptr<unsigned*>(destination)) = *(source++);
			destination += 3;
		}
	}
}

void thred::internal::savmap() {
	unsigned long bytesWritten = 0;

	if (PCSBMPFileName[0]) {
		if (StateMap.test(StateFlag::MONOMAP)) {
			displayText::tabmsg(IDS_SAVMAP);
			return;
		}
		if (!StateMap.test(StateFlag::WASTRAC)) {
			displayText::tabmsg(IDS_MAPCHG);
			return;
		}
		if (GetSaveFileName(&OpenBitmapName)) {
			BitmapFileHandle = CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if (IniFileHandle == INVALID_HANDLE_VALUE) {
				displayText::crmsg(*UserBMPFileName);
				return;
			}
			WriteFile(BitmapFileHandle, &BitmapFileHeader, 14, &bytesWritten, NULL);
			WriteFile(BitmapFileHandle, &BitmapFileHeaderV4, BitmapFileHeader.bfOffBits - 14, &bytesWritten, NULL);
			auto buffer = std::vector<unsigned char>((BitmapWidth * BitmapHeight * 3) + 1);
			movmap(BitmapWidth * BitmapHeight, buffer.data());
			WriteFile(BitmapFileHandle, buffer.data(), BitmapWidth * BitmapHeight * 3, &bytesWritten, NULL);
			CloseHandle(BitmapFileHandle);
		}
	}
	else
		displayText::tabmsg(IDS_SHOMAP);
}

HBITMAP thred::getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ unsigned int** ppvBits) {
	[[gsl::suppress(26490)]] {
		if (ppvBits) {
			HBITMAP bitmap = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, reinterpret_cast<void**>(ppvBits), 0, 0);
			if (*ppvBits != nullptr) {
				return bitmap;
			}
			else {
				throw;
			}
		}
		else {
			throw;
		}
	}
}

void thred::internal::bfil() {
	unsigned      bitmapWidthBytes = 0, widthOverflow = 0, fileHeaderSize = 0, bitmapSizeBytes = 0, iHeight = 0;
	unsigned int* bits                   = nullptr;
	HBITMAP       bitmap                 = {};
	HDC           deviceContext          = {};
	COLORREF      foreground             = {};
	COLORREF      background             = {};
	COLORREF      InverseBackgroundColor = {};

	InverseBackgroundColor = fswap(BackgroundColor);
	BitmapFileHandle       = CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (BitmapFileHandle == INVALID_HANDLE_VALUE) {
		std::wstring fmtStr;
		displayText::loadString(fmtStr, IDS_UNOPEN);
		displayText::shoMsg(fmt::format(fmtStr, UserBMPFileName->wstring()));
		CloseHandle(BitmapFileHandle);
		BitmapFileHandle  = 0;
		PCSBMPFileName[0] = 0;
		return;
	}
	ReadFile(BitmapFileHandle, &BitmapFileHeader, 14, &BytesRead, NULL);
	if (BitmapFileHeader.bfType == 'MB') {
		fileHeaderSize = BitmapFileHeader.bfOffBits - 14;
		if (fileHeaderSize > sizeof(BITMAPV4HEADER))
			fileHeaderSize = sizeof(BITMAPV4HEADER);
		ReadFile(BitmapFileHandle, &BitmapFileHeaderV4, fileHeaderSize, &BytesRead, NULL);
	}
	else {
		CloseHandle(BitmapFileHandle);
		BitmapFileHandle  = 0;
		PCSBMPFileName[0] = 0;
		return;
	}
	if (gudtyp(BitmapFileHeaderV4.bV4BitCount)) {
		if (!StateMap.testAndReset(StateFlag::WASESC))
			StateMap.reset(StateFlag::TRSET);
		BitmapWidth  = BitmapFileHeaderV4.bV4Width;
		BitmapHeight = BitmapFileHeaderV4.bV4Height;
		StateMap.set(StateFlag::INIT);
		ZoomRect.left = ZoomRect.bottom = 0;
		ZoomRect.right                  = UnzoomedRect.x;
		ZoomRect.top                    = UnzoomedRect.y;
		BitmapDC                        = CreateCompatibleDC(StitchWindowDC);
		if (BitmapFileHeaderV4.bV4BitCount == 1) {
			StateMap.set(StateFlag::MONOMAP);
			bitmapWidthBytes = (BitmapWidth >> 5) << 2;
			widthOverflow    = BitmapWidth % 32;
			if (widthOverflow)
				bitmapWidthBytes += 4;
			bitmapSizeBytes = bitmapWidthBytes * BitmapHeight;
			std::vector<unsigned char> monoBitmapData(bitmapSizeBytes);
			ReadFile(BitmapFileHandle, monoBitmapData.data(), bitmapSizeBytes, &BytesRead, NULL);
			CloseHandle(BitmapFileHandle);
			if (binv(monoBitmapData, bitmapWidthBytes)) {
				background = BitmapColor;
				foreground = InverseBackgroundColor;
			}
			else {
				foreground = BitmapColor;
				background = InverseBackgroundColor;
			}
			BitmapInfoHeader               = {};
			BitmapInfoHeader.biSize        = sizeof(BitmapInfoHeader);
			BitmapInfoHeader.biWidth       = BitmapWidth;
			BitmapInfoHeader.biHeight      = BitmapHeight;
			BitmapInfoHeader.biPlanes      = 1;
			BitmapInfoHeader.biBitCount    = 32;
			BitmapInfoHeader.biCompression = BI_RGB;
			BitmapInfo.bmiHeader           = BitmapInfoHeader;
			[[gsl::suppress(26413)]] {
				bitmap = thred::getBitmap(BitmapDC, &BitmapInfo, &bits);
				// Synchronize
				GdiFlush();
				if (bits) {
					for (iHeight = 0; iHeight < BitmapHeight; iHeight++) {
						bitlin(&monoBitmapData[iHeight * bitmapWidthBytes], &bits[iHeight * BitmapWidth], background, foreground);
					}
				}
			}
			deviceContext = CreateCompatibleDC(StitchWindowDC);
			if (bitmap && deviceContext) {
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
		BitmapFileHandle  = 0;
		PCSBMPFileName[0] = 0;
		displayText::tabmsg(IDS_BMAP);
	}
}

void thred::internal::prtred() {
	CloseHandle(FileHandle);
	StateMap.reset(StateFlag::INIT);
	FormIndex = 0;
	displayText::tabmsg(IDS_PRT);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

inline unsigned thred::internal::dtrn(DSTREC* dpnt) {
	return *(convert_ptr<unsigned int*>(dpnt));
}

void thred::internal::dstran(std::vector<DSTREC>& DSTData) {
	unsigned              iRecord = 0, iStitch = 0, color = 0;
	POINT                 localStitch       = {};
	POINT                 dstStitch         = {};
	fPOINT                maximumCoordinate = {};
	fPOINT                mimimumCoordinate = {};
	fPOINT                dstSize           = {};
	fPOINT                delta             = {};
	HANDLE                colorFile         = {};
	std::vector<unsigned> colors;
	unsigned              iColor        = 0;
	DWORD                 bytesRead     = 0;
	LARGE_INTEGER         colorFileSize = {};
	bool                  retval        = false;

	if (colfil()) {
		colorFile = CreateFile(ColorFileName->wstring().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (colorFile != INVALID_HANDLE_VALUE) {
			retval = GetFileSizeEx(colorFile, &colorFileSize);
			// There can only be (64K + 3) colors, so even if HighPart is non-zero, we don't care
			colors.resize(colorFileSize.u.LowPart / sizeof(colors[0]));
			ReadFile(colorFile, colors.data(), colorFileSize.u.LowPart, &bytesRead, 0);
			CloseHandle(colorFile);
			if (bytesRead > (sizeof(colors[0]) * 2)) {
				if (colors[0] == COLVER) {
					BackgroundColor = colors[1];
					ColorChanges    = 0;
				}
			}
		}
	}

	iColor = 2;
	if (bytesRead >= ((iColor + 1) * sizeof(colors[0])))
		color = colmatch(colors[iColor++]);
	else
		color = 0;
	localStitch       = {};
	maximumCoordinate = { -1e12f, -1e12f };
	mimimumCoordinate = { 1e12f, 1e12f };
	for (iRecord = 0; iRecord < DSTData.size(); iRecord++) {
		if (DSTData[iRecord].nd & 0x40) {
			if (bytesRead >= ((iColor + 1) * sizeof(colors[0])))
				color = colmatch(colors[iColor++]);
			else {
				color++;
				color &= 0xf;
			}
		}
		else {
			dstin(thi::dtrn(&DSTData[iRecord]), dstStitch);
			localStitch.x += dstStitch.x;
			localStitch.y += dstStitch.y;
			if (!(DSTData[iRecord].nd & 0x80)) {
				StitchBuffer[iStitch].attribute = color | NOTFRM;
				StitchBuffer[iStitch].x         = localStitch.x * 0.6;
				StitchBuffer[iStitch].y         = localStitch.y * 0.6;
				if (StitchBuffer[iStitch].x > maximumCoordinate.x)
					maximumCoordinate.x = StitchBuffer[iStitch].x;
				if (StitchBuffer[iStitch].y > maximumCoordinate.y)
					maximumCoordinate.y = StitchBuffer[iStitch].y;
				if (StitchBuffer[iStitch].x < mimimumCoordinate.x)
					mimimumCoordinate.x = StitchBuffer[iStitch].x;
				if (StitchBuffer[iStitch].y < mimimumCoordinate.y)
					mimimumCoordinate.y = StitchBuffer[iStitch].y;
				iStitch++;
			}
		}
	}
	PCSHeader.stitchCount = iStitch;
	dstSize.x             = maximumCoordinate.x - mimimumCoordinate.x;
	dstSize.y             = maximumCoordinate.y - mimimumCoordinate.y;
	IniFile.hoopType      = CUSTHUP;
	UnzoomedRect.x        = IniFile.hoopSizeX;
	UnzoomedRect.y        = IniFile.hoopSizeY;
	if (dstSize.x > UnzoomedRect.x || dstSize.y > UnzoomedRect.y) {
		IniFile.hoopSizeX = UnzoomedRect.x = dstSize.x * 1.1;
		IniFile.hoopSizeY = UnzoomedRect.y = dstSize.y * 1.1;
		displayText::hsizmsg();
	}
	delta.x = (UnzoomedRect.x - dstSize.x) / 2 - mimimumCoordinate.x;
	delta.y = (UnzoomedRect.y - dstSize.y) / 2 - mimimumCoordinate.y;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].x += delta.x;
		StitchBuffer[iStitch].y += delta.y;
	}
}

bool thred::internal::chkdst(const DSTHED* dstHeader) noexcept {
	if (strncmp(dstHeader->desched, "LA:", 3))
		return 0;
	return 1;
}

#if PESACT

unsigned tripl(char* dat) {
	return (*convert_ptr<unsigned*>(dat)) & 0xffffff;
}

unsigned dupcol() noexcept {
	unsigned iColor        = 0;
	COLORREF color         = {};
	unsigned matchDistance = 0, minimumDistance = 0, matchIndex = 0;

	color = PESColorTranslate[PEScolors[PEScolorIndex++]];
	for (iColor = 0; iColor < ActivePointIndex; iColor++) {
		if (UserColor[iColor] == color)
			return iColor;
	}
	minimumDistance = 0xffffff;
	for (iColor = 1; iColor < ActivePointIndex; iColor++) {
		matchDistance = pesmtch(color, PEScolors[iColor]);
		if (matchDistance < minimumDistance) {
			minimumDistance = matchDistance;
			matchIndex      = iColor;
		}
	}
	return matchIndex;
}

double dubl(unsigned char* pnt) noexcept {
	unsigned tdat = 0;

	_asm {
		mov		ecx, pnt
		mov		ecx, [ecx]
		movzx	ebx, ch
		test	cl, 8
		je		short dubl1
		mov		ch, 15
		and		cl, ch
		sub		ch, cl
		movzx	ecx, ch
		shl		ecx, 8
		mov		eax, 256
		sub		eax, ebx
		add		ecx, eax
		neg		ecx
		jmp		short dubl2
dubl1 :
		and		ecx, 0x7
		shl		ecx, 8
		add		ecx, ebx
dubl2 :
		mov		tdat, ecx
		fild	tdat
	}
}
#endif

void thred::internal::unthum() {
	unsigned iBackup = 0;

	if (StateMap.testAndReset(StateFlag::THUMSHO)) {
		for (iBackup = 0; iBackup < 4; iBackup++)
			DestroyWindow(BackupViewer[iBackup]);
		if (StateMap.test(StateFlag::UPTO))
			displayText::butxt(HUPTO, (*StringTable)[STR_UPON]);
		else
			displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
		const std::wstring blank(L"");
		displayText::butxt(HNUM, blank);
		thred::redraw((*ButtonWin)[HHID]);
		displayText::butxt(HBOXSEL, (*StringTable)[STR_BOXSEL]);
	}
}

void thred::internal::rstdu() {
	deldu();
	StateMap.reset(StateFlag::REDUSHO);
	StateMap.reset(StateFlag::UNDUSHO);
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
	StateMap.set(StateFlag::DUMEN);
}

void thred::internal::nuFil() {
	unsigned      fileSize = 0, iStitch = 0;
	unsigned      version            = 0;
	DWORD         fileSizeHigh       = {};
	unsigned      textureHistoryFlag = 0, pcsStitchCount = 0;
	unsigned      iPCSstitch = 0, color = 0, iColor = 0;
	unsigned      iColorChange   = 0;
	STRHED        thredHeader    = {};
	wchar_t       buffer[3]      = { 0 };
	char*         tnam           = nullptr;
	DSTHED        dstHeader      = {};
	long          totalBytesRead = 0;
	fRECTANGLE    stitchRect     = {};
	unsigned long stitchesRead   = 0;

#if PESACT

	unsigned char* fileBuffer         = nullptr;
	unsigned       iActualPESstitches = 0, iPESstitch = 0;
	PESHED*        pesHeader     = nullptr;
	char*          l_peschr      = nullptr;
	unsigned       pecof         = 0;
	unsigned char* pesColorCount = nullptr;
	dPOINT         loc           = {};
	PESTCH*        pabstch       = nullptr;
	unsigned       pabind        = 0;
	double         locof         = 0.0;
#endif

	wchar_t szFileName[_MAX_PATH] = { 0 };
	wchar_t lpszBuffer[_MAX_PATH] = { 0 };
	auto    workingFileStr        = WorkingFileName->wstring();
	auto    dirStr                = DefaultDirectory->wstring();
	std::copy(workingFileStr.begin(), workingFileStr.end(), szFileName);
	std::copy(dirStr.begin(), dirStr.end(), lpszBuffer);
	OpenFileName.hwndOwner       = ThrEdWindow;
	OpenFileName.hInstance       = ThrEdInstance;
	OpenFileName.lpstrFile       = szFileName;
	OpenFileName.lpstrInitialDir = lpszBuffer;
	if (StateMap.testAndReset(StateFlag::REDOLD) || GetOpenFileName(&OpenFileName)) {
		WorkingFileName->assign(szFileName);
		fnamtabs();
		trace::untrace();
		if (FormIndex)
			form::delfrms();
		StateMap.reset(StateFlag::ZUMED);
		StateMap.reset(StateFlag::FRMOF);
		StateMap.reset(StateFlag::HID);
		StateMap.reset(StateFlag::BIGBOX);
		unthum();
		thred::unbsho();
		if (PCSBMPFileName[0]) {
			DeleteObject(BitmapFileHandle);
			ReleaseDC(ThrEdWindow, BitmapDC);
			PCSBMPFileName[0] = 0;
		}
		// ToDo - use ifstream?
		// ifstream file(WorkingFileName, ios::in | ios::binary | ios::ate);
		FileHandle = CreateFile(WorkingFileName->wstring().c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (FileHandle == INVALID_HANDLE_VALUE) {
			if (GetLastError() == 32)
				displayText::filnopn(IDS_FNOPNA, *WorkingFileName);
			else
				displayText::filnopn(IDS_FNOPN, *WorkingFileName);
			FileHandle = 0;
		}
		else {
			StateMap.reset(StateFlag::CMPDO);
			StateMap.reset(StateFlag::SAVACT);
			StateMap.reset(StateFlag::BAKING);
			StateMap.reset(StateFlag::REDUSHO);
			TextureIndex = 0;
			EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
			deldu();
			DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
			thred::unbsho();
			StateMap.reset(StateFlag::MOVSET);
			form::frmon();
			SelectedFormList->clear();
			if (StateMap.testAndReset(StateFlag::PRFACT)) {
				DestroyWindow(PreferencesWindow);
				StateMap.reset(StateFlag::WASRT);
				PreferenceIndex = 0;
			}
			PCSBMPFileName[0] = 0;
			if (SearchLineIndex)
				SearchLineIndex = 0;
			StateMap.reset(StateFlag::SCROS);
			StateMap.reset(StateFlag::ECROS);
			StateMap.reset(StateFlag::BZUMIN);
			rstdu();
			thred::unmsg();
			UndoBufferWriteIndex = 0;
			StateMap.reset(StateFlag::BAKWRAP);
			ZoomFactor = 1;
			StateMap.set(StateFlag::RESTCH);
			defNam(*WorkingFileName);
			NearestCount = 0;
			if (StateMap.testAndReset(StateFlag::WASPAT))
				DestroyWindow(SpeedScrollBar);
			textureHistoryFlag = 0;
			if (StateMap.test(StateFlag::WASTXBAK))
				textureHistoryFlag = 1;
			StateMap.reset();
			if (textureHistoryFlag)
				StateMap.set(StateFlag::WASTXBAK);
			fileSize                  = GetFileSize(FileHandle, &fileSizeHigh);
			auto       fileExt        = WorkingFileName->extension().wstring();
			const auto firstCharacter = tolower(fileExt[1]);
			if (firstCharacter == 't') {
				ReadFile(FileHandle, &thredHeader, sizeof(thredHeader), &BytesRead, NULL);
				if ((thredHeader.headerType & 0xffffff) == 0x746872) {
					if (BytesRead != sizeof(thredHeader)) {
						displayText::tabmsg(IDS_SHRTF);
						return;
					}
					version = (thredHeader.headerType & 0xff000000) >> 24;
					DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
					switch (version) {
					case 0:
						if (PCSHeader.hoopType == SMALHUP) {
							UnzoomedRect.x = IniFile.hoopSizeX = SHUPX;
							UnzoomedRect.y = IniFile.hoopSizeY = SHUPY;
						}
						else {
							UnzoomedRect.x = IniFile.hoopSizeX = LHUPX;
							UnzoomedRect.y = IniFile.hoopSizeY = LHUPY;
							PCSHeader.hoopType                 = LARGHUP;
						}
						ritfnam(*DesignerName);
						std::copy(IniFile.designerName,
						          IniFile.designerName + strlen(IniFile.designerName),
						          ExtendedHeader.modifierName);
						break;
					case 1:
					case 2:
						ReadFile(FileHandle, &ExtendedHeader, sizeof(ExtendedHeader), &BytesRead, NULL);
						if (BytesRead != sizeof(ExtendedHeader)) {
							displayText::tabmsg(IDS_SHRTF);
							return;
						}
						IniFile.hoopSizeX = UnzoomedRect.x = ExtendedHeader.hoopSizeX;
						IniFile.hoopSizeY = UnzoomedRect.y = ExtendedHeader.hoopSizeY;
						redfnam(*DesignerName);
						break;
					default:
						displayText::tabmsg(IDS_NOTVER);
						return;
					}
					ZoomRect.bottom = ZoomRect.left = 0;
					ZoomRect.right = UnzoomedRect.x = IniFile.hoopSizeX;
					ZoomRect.top = UnzoomedRect.y = IniFile.hoopSizeY;
					PCSHeader.stitchCount         = thredHeader.stitchCount;
					ReadFile(FileHandle, StitchBuffer, PCSHeader.stitchCount * sizeof(StitchBuffer[0]), &stitchesRead, NULL);
					if (stitchesRead != PCSHeader.stitchCount * sizeof(StitchBuffer[0])) {
						PCSHeader.stitchCount = gsl::narrow<unsigned short>(stitchesRead / sizeof(StitchBuffer[0]));
						prtred();
						return;
					}
					ReadFile(FileHandle, PCSBMPFileName, sizeof(PCSBMPFileName), &BytesRead, 0);
					totalBytesRead = BytesRead;
					if (BytesRead != sizeof(PCSBMPFileName)) {
						PCSBMPFileName[0] = 0;
						prtred();
						return;
					}
					ReadFile(FileHandle, &BackgroundColor, sizeof(BackgroundColor), &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != sizeof(BackgroundColor)) {
						BackgroundColor = IniFile.backgroundColor;
						prtred();
						return;
					}
					BackgroundBrush = CreateSolidBrush(BackgroundColor);
					ReadFile(FileHandle, UserColor, sizeof(UserColor), &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != sizeof(UserColor)) {
						prtred();
						return;
					}
					ReadFile(FileHandle, CustomColor, sizeof(CustomColor), &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != sizeof(CustomColor)) {
						prtred();
						return;
					}
					constexpr auto threadLength = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0]))
					                              / 2; // ThreadSize is defined as a 16 entry array of 2 characters
					char msgBuffer[threadLength];
					ReadFile(FileHandle, msgBuffer, threadLength, &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != threadLength) {
						prtred();
						return;
					}
					std::string threadSizebuf(msgBuffer, sizeof(msgBuffer));
					auto        threadSizeBufW = utf::Utf8ToUtf16(threadSizebuf);
					for (int iThread = 0; iThread < threadLength; iThread++)
						ThreadSize[iThread][0] = threadSizeBufW[iThread];
					FormIndex = thredHeader.formCount;
					if (FormIndex > MAXFORMS) {
						FormIndex = MAXFORMS;
					}
					pcsStitchCount = 0;
					if (FormIndex) {
						StateMap.reset(StateFlag::BADFIL);
						FormVertexIndex = ClipPointIndex = 0;
						satin::clearGuideSize();
						MsgBuffer[0]      = 0;
						DWORD bytesToRead = 0u;
						if (version < 2) {
							std::vector<FRMHEDO> formListOriginal(FormIndex);
							bytesToRead = gsl::narrow<DWORD>(FormIndex * sizeof(formListOriginal[0]));
							ReadFileInt(FileHandle, formListOriginal.data(), bytesToRead, &BytesRead, 0);
							if (BytesRead != FormIndex * sizeof(formListOriginal[0])) {
								FormIndex = BytesRead / sizeof(formListOriginal[0]);
								StateMap.set(StateFlag::BADFIL);
							}
							std::copy(formListOriginal.cbegin(), formListOriginal.cend(), FormList);
						}
						else {
							std::vector<FRMHEDOUT> inFormList(FormIndex);
							bytesToRead = gsl::narrow<DWORD>(FormIndex * sizeof(inFormList[0]));
							ReadFileInt(FileHandle, inFormList.data(), bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								FormIndex = BytesRead / sizeof(inFormList[0]);
								StateMap.set(StateFlag::BADFIL);
							}
							std::copy(inFormList.cbegin(), inFormList.cend(), FormList);
						}
						if (thredHeader.vertexCount) {
							bytesToRead = gsl::narrow<DWORD>(thredHeader.vertexCount * sizeof(FormVertices[0]));
							ReadFile(FileHandle, FormVertices, bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								FormVertexIndex = BytesRead / sizeof(FormVertices[0]);
								for (size_t iVertex = FormVertexIndex; iVertex < thredHeader.vertexCount; iVertex++)
									FormVertices[iVertex].x = FormVertices[iVertex].y = 0;
								StateMap.set(StateFlag::BADFIL);
							}
						}
						else {
							// We have forms but no vertices - blow up the read
							prtred();
							return;
						}
						if (thredHeader.dlineCount) {
							std::vector<SATCONOUT> inSatinGuides(thredHeader.dlineCount);
							bytesToRead = gsl::narrow<DWORD>(thredHeader.dlineCount * sizeof(inSatinGuides[0]));
							ReadFile(FileHandle, inSatinGuides.data(), bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								StateMap.set(StateFlag::BADFIL);
							}
							std::copy(inSatinGuides.cbegin(), inSatinGuides.cend(), SatinGuides);
						}
						if (thredHeader.clipDataCount) {
							bytesToRead = gsl::narrow<DWORD>(thredHeader.clipDataCount * sizeof(ClipPoints[0]));
							ReadFile(FileHandle, ClipPoints, bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								ClipPointIndex = BytesRead / sizeof(ClipPoints[0]);
								StateMap.set(StateFlag::BADFIL);
							}
						}
						if (ExtendedHeader.texturePointCount) {
							TexturePointsBuffer->resize(ExtendedHeader.texturePointCount);
							bytesToRead
							    = gsl::narrow<DWORD>(ExtendedHeader.texturePointCount * sizeof((*TexturePointsBuffer)[0]));
							ReadFile(FileHandle, TexturePointsBuffer->data(), bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								TexturePointsBuffer->resize(BytesRead / sizeof((*TexturePointsBuffer)[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							TextureIndex = TexturePointsBuffer->size();
						}
						else {
							TextureIndex = 0;
						}
						if (StateMap.testAndReset(StateFlag::BADFIL)) {
							displayText::bfilmsg();
						}
						// now re-create all the pointers in the form data
						satin::clearGuideSize();
						ClipPointIndex  = 0;
						FormVertexIndex = 0;
						for (unsigned iForm = 0; iForm < FormIndex; iForm++) {
							FormList[iForm].vertices = thred::adflt(FormList[iForm].vertexCount);
							if (FormList[iForm].type == SAT) {
								if (FormList[iForm].satinGuideCount)
									FormList[iForm].satinOrAngle.guide = satin::adsatk(FormList[iForm].satinGuideCount);
							}
							if (clip::isclp(iForm))
								FormList[iForm].angleOrClipData.clip = thred::adclp(FormList[iForm].lengthOrCount.clipCount);
							if (clip::iseclpx(iForm))
								FormList[iForm].borderClipData = thred::adclp(FormList[iForm].clipEntries);
						}
						xt::setfchk();
					}
				}
				else
					displayText::tabmsg(IDS_NOTHR);
			}
			else {
				StateMap.set(StateFlag::NOTHRFIL);
				if (firstCharacter == 'p') {
					if (tolower(fileExt[2]) == 'c') {
						ReadFile(FileHandle, &PCSHeader, sizeof(PCSHeader), &BytesRead, NULL);
						if (!fileSize) {
							displayText::filnopn(IDS_ZEROL, *WorkingFileName);
							return;
						}
						if (PCSHeader.leadIn == '2' && PCSHeader.colorCount == 16) {
							for (iColor = 0; iColor < 16; iColor++)
								UserColor[iColor] = PCSHeader.colors[iColor];
							fileSize -= sizeof(PCSHeader);
							pcsStitchCount = fileSize / sizeof(PCSTCH) + 2;
							std::vector<PCSTCH> PCSDataBuffer(pcsStitchCount);
							ReadFile(FileHandle, PCSDataBuffer.data(), fileSize, &BytesRead, NULL);
							iStitch      = 0;
							iColorChange = 0;
							color        = 0;
							iPCSstitch   = 0;
							while (iStitch < PCSHeader.stitchCount && iPCSstitch < pcsStitchCount) {
								if (PCSDataBuffer[iPCSstitch].tag == 3) {
									ColorChangeTable[iColorChange].colorIndex    = PCSDataBuffer[iPCSstitch].fx;
									ColorChangeTable[iColorChange++].stitchIndex = iStitch;
									color                                        = NOTFRM | PCSDataBuffer[iPCSstitch++].fx;
								}
								else {
									StitchBuffer[iStitch].x
									    = PCSDataBuffer[iPCSstitch].x + static_cast<float>(PCSDataBuffer[iPCSstitch].fx) / 256;
									StitchBuffer[iStitch].y
									    = PCSDataBuffer[iPCSstitch].y + static_cast<float>(PCSDataBuffer[iPCSstitch].fy) / 256;
									StitchBuffer[iStitch++].attribute = color;
									iPCSstitch++;
								}
							}
							PCSHeader.stitchCount = iStitch;
							// Grab the bitmap filename
							tnam = convert_ptr<char*>(&PCSDataBuffer[iPCSstitch]);
							strcpy_s(PCSBMPFileName, tnam);
							// wcscpy_s(fileExtention, sizeof(WorkingFileName) - (fileExtention - WorkingFileName), L"thr");
							IniFile.auxFileType = AUXPCS;
							if (PCSHeader.hoopType != LARGHUP && PCSHeader.hoopType != SMALHUP)
								PCSHeader.hoopType = LARGHUP;
							sizstch(stitchRect, StitchBuffer);
							if (stitchRect.left < 0 || stitchRect.right > LHUPY || stitchRect.bottom < 0
							    || stitchRect.top > LHUPY) {
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
						// ToDo - (PES) is there a better estimate for data size?
						fileBuffer = new unsigned char[MAXITEMS * 8];
						ReadFile(FileHandle, fileBuffer, MAXITEMS * 8, &BytesRead, 0);
						pesHeader = convert_ptr<PESHED*>(fileBuffer);
						l_peschr  = convert_ptr<char*>(pesHeader);
						if (strncmp(pesHeader->led, "#PES00", 6)) {
							std::string fmtStr;
							displayText::displayText::loadString(fmtStr, IDS_NOTPES);
							displayText::shoMsg(fmt::format(fmtStr, WorkingFileName));
							return;
						}
						pecof            = tripl(pesHeader->off);
						PESstitch        = fileBuffer + (pecof + 532);
						ActivePointIndex = 0;
						pesColorCount    = convert_ptr<unsigned char*>(&l_peschr[pecof + 48]);
						PEScolors        = &pesColorCount[1];
						// allow all the colors described by a char
						boost::dynamic_bitset<> colorMap(256);
						ActivePointIndex = 0;
						for (iColor = 0; iColor < gsl::narrow<unsigned>(*pesColorCount + 1); iColor++) {
							if (!colorMap.test_set(PEScolors[iColor])) {
								UserColor[ActivePointIndex++] = PESColorTranslate[PEScolors[iColor] & PESCMSK];
								if (ActivePointIndex >= 16)
									break;
							}
						}
						color         = 0;
						PEScolorIndex = 1;
						loc.x = loc.y = 0;
						pabind        = 0;
						StateMap.reset(StateFlag::FILDIR);
						pabstch            = convert_ptr<PESTCH*>(&l_peschr[sizeof(pesHeader) + 4]);
						iPESstitch         = 0;
						iActualPESstitches = 1;
						StitchBuffer[0].x  = StitchBuffer[0].y;
						while (iPESstitch < BytesRead - pecof - 529) {
							if (PESstitch[iPESstitch] == 0xff && PESstitch[iPESstitch + 1] == 0)
								break;
							if (PESstitch[iPESstitch] == 0xfe && PESstitch[iPESstitch + 1] == 0xb0) {
								color = dupcol();
								iPESstitch += 2;
							}
							else {
								if (PESstitch[iPESstitch] & 0x80) {
									locof = dubl(&PESstitch[iPESstitch]);
									iPESstitch++;
								}
								else {
									if (PESstitch[iPESstitch] & 0x40)
										locof = PESstitch[iPESstitch] - 128;
									else
										locof = PESstitch[iPESstitch];
								}
								locof *= 0.6;
								// ToDo - (PES) Use a new flag bit for this since FILDIR is not correct
								if (StateMap.testAndFlip(StateFlag::FILDIR)) {
									loc.y -= locof;
									StitchBuffer[iActualPESstitches].x         = loc.x;
									StitchBuffer[iActualPESstitches].y         = loc.y;
									StitchBuffer[iActualPESstitches].attribute = color;
									iActualPESstitches++;
								}
								else
									loc.x += locof;
							}
							iPESstitch++;
						}
						PCSHeader.stitchCount = iActualPESstitches;
						// IniFile.auxFileType=AUXPES;
						hupfn();
						delete[] fileBuffer;
					}
#endif
				}
				else {
					ReadFile(FileHandle, &dstHeader, sizeof(dstHeader), &BytesRead, 0);
					if (BytesRead == sizeof(dstHeader)) {
						if (chkdst(&dstHeader)) {
							PCSBMPFileName[0] = 0;
							fileSize          = GetFileSize(FileHandle, &BytesRead) - sizeof(dstHeader);
							std::vector<DSTREC> DSTData(fileSize / sizeof(DSTREC));
							ReadFileInt(FileHandle, DSTData.data(), fileSize, &BytesRead, 0);
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
			if (PCSBMPFileName[0]) {
				fs::current_path(*DefaultDirectory);
				auto BMPfileName = utf::Utf8ToUtf16(PCSBMPFileName);
				UserBMPFileName->assign(BMPfileName);
				bfil();
			}
			thred::ritot(PCSHeader.stitchCount);
			// BufferIndex     = 0;
			ZoomRect.left   = 0;
			ZoomRect.bottom = 0;
			ZoomRect.right = UnzoomedRect.x = IniFile.hoopSizeX;
			ZoomRect.top = UnzoomedRect.y = IniFile.hoopSizeY;
			thred::movStch();
			thred::coltab();
			StateMap.reset(StateFlag::ZUMED);
			buffer[2] = 0;
			for (iColor = 0; iColor < 16; iColor++) {
				UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
				wcsncpy_s(buffer, ThreadSize[iColor], 2);
				SetWindowText(ThreadSizeWin[iColor], buffer);
			}
			for (iColor = 0; iColor < 16; iColor++)
				thred::redraw(UserColorWin[iColor]);
			thred::redraw(ColorBar);
			if (PCSHeader.stitchCount)
				StateMap.set(StateFlag::INIT);
			nuAct(0);
			StateMap.set(StateFlag::RESTCH);
			nunams();
			ritini();
			const std::wstring blank(L"");
			displayText::butxt(HNUM, blank);
			if (PCSHeader.stitchCount)
				nuAct(StitchBuffer[0].attribute & COLMSK);
			else
				nuAct(0);
			auxmen();
		}
		lenCalc();
		SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRDBY], WorkingFileName->wstring(), *DesignerName).c_str());
		CloseHandle(FileHandle);
		StateMap.set(StateFlag::INIT);
		StateMap.reset(StateFlag::TRSET);
		if (StateMap.test(StateFlag::NOTHRFIL)) {
			for (iStitch = 0; iStitch < (stitchesRead / sizeof(StitchBuffer[0])); iStitch++)
				StitchBuffer[iStitch].attribute |= NOTFRM;
		}
		repair::lodchk();
	}
}

COLORREF thred::internal::nuCol(COLORREF init) noexcept {
	ColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
	ColorStruct.hwndOwner      = ThrEdWindow;
	ColorStruct.lCustData      = 0;
	ColorStruct.lpCustColors   = CustomColor;
	ColorStruct.lpfnHook       = 0;
	ColorStruct.lpTemplateName = 0;
	ColorStruct.rgbResult      = init;
	ColorStruct.lStructSize    = sizeof(ColorStruct);
	return ChooseColor(&ColorStruct);
}

COLORREF thred::internal::nuBak() noexcept {
	BackgroundColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
	BackgroundColorStruct.hwndOwner      = ThrEdWindow;
	BackgroundColorStruct.lCustData      = 0;
	BackgroundColorStruct.lpCustColors   = CustomBackgroundColor;
	BackgroundColorStruct.lpfnHook       = 0;
	BackgroundColorStruct.lpTemplateName = 0;
	BackgroundColorStruct.rgbResult      = BackgroundColor;
	BackgroundColorStruct.lStructSize    = sizeof(BackgroundColorStruct);
	return ChooseColor(&BackgroundColorStruct);
}

COLORREF thred::internal::nuBit() noexcept {
	BitMapColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
	BitMapColorStruct.hwndOwner      = ThrEdWindow;
	BitMapColorStruct.lCustData      = 0;
	BitMapColorStruct.lpCustColors   = BitmapBackgroundColors;
	BitMapColorStruct.lpfnHook       = 0;
	BitMapColorStruct.lpTemplateName = 0;
	BitMapColorStruct.rgbResult      = BitmapColor;
	BitMapColorStruct.lStructSize    = sizeof(BackgroundColorStruct);
	return ChooseColor(&BitMapColorStruct);
}

void thred::pxCor2stch(const POINT& point) noexcept {
	double ratio;

	ratio           = static_cast<double>(point.x - StitchWindowAbsRect.left) / StitchWindowClientRect.right;
	SelectedPoint.x = ratio * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	ratio           = static_cast<double>(StitchWindowAbsRect.bottom - point.y) / StitchWindowClientRect.bottom;
	SelectedPoint.y = ratio * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

unsigned thred::px2stch() noexcept {
	double tdub = 0;

	if (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right && Msg.pt.y >= StitchWindowAbsRect.top
	    && Msg.pt.y <= StitchWindowAbsRect.bottom) {
		tdub            = static_cast<double>(Msg.pt.x - StitchWindowAbsRect.left) / StitchWindowClientRect.right;
		SelectedPoint.x = tdub * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
		tdub            = static_cast<double>(StitchWindowAbsRect.bottom - Msg.pt.y) / StitchWindowClientRect.bottom;
		SelectedPoint.y = tdub * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
		return 1;
	}
	else
		return 0;
}

void thred::internal::zumin() {
	dPOINT     newSize           = {};
	fRECTANGLE groupBoundingRect = {};

	if (!StateMap.testAndReset(StateFlag::ZUMACT))
		ZoomFactor *= ZUMFCT;
	if (ZoomFactor < ZoomMin)
		ZoomFactor = ZoomMin;
	if (!StateMap.testAndSet(StateFlag::ZUMED))
		thred::movStch();
	if (!StateMap.testAndReset(StateFlag::BZUMIN)) {
		do {
			if (StateMap.test(StateFlag::GMRK)) {
				SelectedPoint = ZoomMarkPoint;
				break;
			}
			if (StateMap.test(StateFlag::FORMSEL)) {
				const auto& boundingRect = FormList[ClosestFormToCursor].rectangle;
				SelectedPoint.x          = ((boundingRect.right - boundingRect.left) / 2) + boundingRect.left;
				SelectedPoint.y          = ((boundingRect.top - boundingRect.bottom) / 2) + boundingRect.bottom;
				break;
			}
			if (StateMap.test(StateFlag::FRMPSEL)) {
				SelectedPoint = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor];
				break;
			}
			if (StateMap.test(StateFlag::SELBOX)) {
				SelectedPoint = StitchBuffer[ClosestPointIndex];
				break;
			}
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::selRct(groupBoundingRect);
				SelectedPoint.x = ((groupBoundingRect.right - groupBoundingRect.left) / 2) + groupBoundingRect.left;
				SelectedPoint.y = ((groupBoundingRect.top - groupBoundingRect.bottom) / 2) + groupBoundingRect.bottom;
				break;
			}
			if (StateMap.test(StateFlag::INSRT)) {
				if (StateMap.test(StateFlag::LIN1)) {
					if (StateMap.test(StateFlag::BAKEND)) {
						SelectedPoint = StitchBuffer[PCSHeader.stitchCount - 1];
					}
					else {
						SelectedPoint = StitchBuffer[0];
					}
				}
				else {
					SelectedPoint.x = (StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x) / 2
					                  + StitchBuffer[ClosestPointIndex].x;
					SelectedPoint.y = (StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y) / 2
					                  + StitchBuffer[ClosestPointIndex].y;
				}
				break;
			}
			if (SelectedFormList->size()) {
				auto firstForm           = SelectedFormList->front();
				SelectedFormsRect.bottom = FormList[firstForm].rectangle.bottom;
				SelectedFormsRect.top    = FormList[firstForm].rectangle.top;
				SelectedFormsRect.left   = FormList[firstForm].rectangle.left;
				SelectedFormsRect.right  = FormList[firstForm].rectangle.right;
				for (auto selectedForm : (*SelectedFormList)) {
					if (FormList[selectedForm].rectangle.bottom < SelectedFormsRect.bottom)
						SelectedFormsRect.bottom = FormList[selectedForm].rectangle.bottom;
					if (FormList[selectedForm].rectangle.top > SelectedFormsRect.top)
						SelectedFormsRect.top = FormList[selectedForm].rectangle.top;
					if (FormList[selectedForm].rectangle.left < SelectedFormsRect.left)
						SelectedFormsRect.left = FormList[selectedForm].rectangle.left;
					if (FormList[selectedForm].rectangle.right > SelectedFormsRect.right)
						SelectedFormsRect.right = FormList[selectedForm].rectangle.right;
				}
				SelectedPoint.x = (SelectedFormsRect.right - SelectedFormsRect.left) / 2 + SelectedFormsRect.left;
				SelectedPoint.y = (SelectedFormsRect.top - SelectedFormsRect.bottom) / 2 + SelectedFormsRect.bottom;
				break;
			}
			if (!thred::px2stch())
				centr();
		} while (false);
	}
	newSize.x     = UnzoomedRect.x * ZoomFactor;
	newSize.y     = newSize.x / StitchWindowAspectRatio;
	ZoomRect.left = ZoomRect.bottom = 0;
	ZoomRect.right                  = newSize.x;
	ZoomRect.top                    = newSize.y;
	thred::shft(SelectedPoint);
	NearestCount = 0;
	if (!StateMap.test(StateFlag::GMRK) && StateMap.test(StateFlag::SELBOX))
		shft2box();
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
	POINT      newSize           = {};
	fRECTANGLE groupBoundingRect = {};

	unboxs();
	if (StateMap.test(StateFlag::ZUMED)) {
		do {
			if (StateMap.test(StateFlag::GMRK)) {
				SelectedPoint = ZoomMarkPoint;
				break;
			}
			if (StateMap.test(StateFlag::FORMSEL)) {
				const auto& boundingRect = FormList[ClosestFormToCursor].rectangle;
				SelectedPoint.x          = ((boundingRect.right - boundingRect.left) / 2) + boundingRect.left;
				SelectedPoint.y          = ((boundingRect.top - boundingRect.bottom) / 2) + boundingRect.bottom;
				break;
			}
			if (StateMap.test(StateFlag::FRMPSEL)) {
				SelectedPoint = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor];
				break;
			}
			if (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::INSRT)) {
				SelectedPoint = StitchBuffer[ClosestPointIndex];
				break;
			}
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::selRct(groupBoundingRect);
				SelectedPoint.x = ((groupBoundingRect.right - groupBoundingRect.left) / 2) + groupBoundingRect.left;
				SelectedPoint.y = ((groupBoundingRect.top - groupBoundingRect.bottom) / 2) + groupBoundingRect.bottom;
				break;
			}
			if (StateMap.test(StateFlag::SELBOX)) {
				shft2box();
				break;
			}
			if (!thred::px2stch())
				centr();
		} while (false);
		ZoomFactor /= ZUMFCT;
		if (ZoomFactor > 0.98) {
			ZoomFactor = 1;
			StateMap.reset(StateFlag::ZUMED);
			ZoomRect.bottom = 0;
			ZoomRect.left   = 0;
			ZoomRect.right  = UnzoomedRect.x;
			ZoomRect.top    = UnzoomedRect.y;
			thred::movStch();
			NearestCount = 0;
		}
		else {
			newSize.x     = UnzoomedRect.x * ZoomFactor;
			newSize.y     = newSize.x / StitchWindowAspectRatio;
			ZoomRect.left = ZoomRect.bottom = 0;
			ZoomRect.top                    = newSize.y;
			ZoomRect.right                  = newSize.x;
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

void thred::internal::duClos(unsigned startStitch, unsigned stitchCount) {
	unsigned iStitch = 0, iNear = 0, tind0 = 0, tind1 = 0;
	double   sum = 0.0, lowestSum = 0.0;
	long     cx = 0, cy = 0;

	for (iStitch = startStitch; iStitch < startStitch + stitchCount; iStitch++) {
		cx    = ((StitchBuffer[iStitch].x > SelectedPoint.x) ? (StitchBuffer[iStitch].x - SelectedPoint.x)
                                                          : (SelectedPoint.x - StitchBuffer[iStitch].x));
		cy    = ((StitchBuffer[iStitch].y > SelectedPoint.y) ? (StitchBuffer[iStitch].y - SelectedPoint.y)
                                                          : (SelectedPoint.y - StitchBuffer[iStitch].y));
		sum   = hypot(cx, cy);
		tind0 = iStitch;
		for (iNear = 0; iNear < NERCNT; iNear++) {
			if (sum < GapToNearest[iNear]) {
				lowestSum           = GapToNearest[iNear];
				tind1               = NearestPoint[iNear];
				GapToNearest[iNear] = sum;
				NearestPoint[iNear] = tind0;
				sum                 = lowestSum;
				tind0               = tind1;
			}
		}
	}
}

void thred::internal::closPnt() {
	unsigned iColor = 0, iNear = 0, iStitch0 = 0, iStitch1 = 0;

	unbox();
	unboxs();
	for (iNear = 0; iNear < 4; iNear++) {
		GapToNearest[iNear] = 1e99;
		NearestPoint[iNear] = -1;
	}
	thred::px2stch();
	for (iColor = 0; iColor < ColorChanges; iColor++) {
		iStitch0 = ColorChangeTable[iColor].stitchIndex;
		iStitch1 = ColorChangeTable[iColor + 1].stitchIndex;
		if (StateMap.test(StateFlag::HID)) {
			if (ColorChangeTable[iColor].colorIndex == ActiveColor)
				duClos(iStitch0, iStitch1 - iStitch0);
		}
		else
			duClos(iStitch0, iStitch1 - iStitch0);
	}
	GetClientRect(MainStitchWin, &StitchWindowClientRect);
	for (iNear = 0; iNear < NERCNT; iNear++) {
		if (stch2px(NearestPoint[iNear])) {
			NearestPoint[NearestCount]   = NearestPoint[iNear];
			NearestPixel[NearestCount++] = StitchCoordinatesPixels;
		}
	}
	boxs();
}

bool thred::internal::closPnt1(unsigned* closestStitch) {
	if (closestStitch) {
		unsigned iColor = 0, iNear = 0, iStitch = 0, closestIndex = 0;
		unsigned layer  = 0;
		long     offset = 0;
		double   cx = 0.0, cy = 0.0;
		POINT    pointToCheck    = { (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };
		double   currentDistance = 1e99;

		if (StateMap.test(StateFlag::SELBOX) && stch2px(ClosestPointIndex)) {
			if (hypot(StitchCoordinatesPixels.x - pointToCheck.x, StitchCoordinatesPixels.y - pointToCheck.y) < CLOSENUF) {
				*closestStitch = ClosestPointIndex;
				return true;
			}
		}
		for (iNear = 0; iNear < NearestCount; iNear++) {
			offset = BoxOffset[iNear];
			if (pointToCheck.x >= NearestPixel[iNear].x - offset && pointToCheck.x <= NearestPixel[iNear].x + offset
			    && pointToCheck.y >= NearestPixel[iNear].y - offset && pointToCheck.y <= NearestPixel[iNear].y + offset) {
				*closestStitch = NearestPoint[iNear];
				return true;
			}
		}
		thred::px2stch();
		DistanceToClick = 1e99;
		if (StateMap.test(StateFlag::HID)) {
			for (iColor = 0; iColor < ColorChanges; iColor++) {
				if (ColorChangeTable[iColor].colorIndex == ActiveColor) {
					for (iStitch = ColorChangeTable[iColor].stitchIndex; iStitch < ColorChangeTable[iColor + 1].stitchIndex;
					     iStitch++) {
						if (StitchBuffer[iStitch].x >= ZoomRect.left && StitchBuffer[iStitch].x <= ZoomRect.right
						    && StitchBuffer[iStitch].y >= ZoomRect.bottom && StitchBuffer[iStitch].y <= ZoomRect.top) {
							cx = ((StitchBuffer[iStitch].x > SelectedPoint.x) ? (StitchBuffer[iStitch].x - SelectedPoint.x)
							                                                  : (SelectedPoint.x - StitchBuffer[iStitch].x));
							cy = ((StitchBuffer[iStitch].y > SelectedPoint.y) ? (StitchBuffer[iStitch].y - SelectedPoint.y)
							                                                  : (SelectedPoint.y - StitchBuffer[iStitch].y));
							currentDistance = hypot(cx, cy);
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
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				layer = (StitchBuffer[iStitch].attribute & LAYMSK) >> LAYSHFT;
				if (!ActiveLayer || !layer || layer == ActiveLayer) {
					if (StitchBuffer[iStitch].x >= ZoomRect.left && StitchBuffer[iStitch].x <= ZoomRect.right
					    && StitchBuffer[iStitch].y >= ZoomRect.bottom && StitchBuffer[iStitch].y <= ZoomRect.top) {
						cx              = StitchBuffer[iStitch].x - SelectedPoint.x;
						cy              = StitchBuffer[iStitch].y - SelectedPoint.y;
						currentDistance = hypot(cx, cy);
						if (currentDistance < DistanceToClick) {
							DistanceToClick = currentDistance;
							closestIndex    = iStitch;
						}
					}
				}
			}
		}
		if (DistanceToClick == 1e99)
			return false;
		else {
			stch2px(closestIndex);
			if (StateMap.test(StateFlag::IGNTHR)) {
				*closestStitch = closestIndex;
				return true;
			}
			else {
				if (hypot(pointToCheck.x - StitchCoordinatesPixels.x, pointToCheck.y - StitchCoordinatesPixels.y) < CLOSENUF) {
					*closestStitch = closestIndex;
					return true;
				}
				else
					return false;
			}
		}
	}
	return false;
}

constexpr unsigned thred::internal::pt2colInd(unsigned iStitch) {
	unsigned iColor = 0;

	for (iColor = 0; iColor < ColorChanges; iColor++) {
		if (ColorChangeTable[iColor].stitchIndex > iStitch)
			break;
	}
	return iColor;
}

void thred::internal::toglup() {
	if (StateMap.testAndFlip(StateFlag::UPTO))
		displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
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
					while (ClosestPointIndex < PCSHeader.stitchCount && form::notfstch(StitchBuffer[ClosestPointIndex].attribute))
						ClosestPointIndex++;
					StateMap.set(StateFlag::SELBOX);
					StateMap.reset(StateFlag::FRMPSEL);
				}
			}
		}
		displayText::butxt(HUPTO, (*StringTable)[STR_UPON]);
	}
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::toglHid() {
	if (StateMap.testAndFlip(StateFlag::HID))
		StateMap.reset(StateFlag::FRMOF);
	else
		StateMap.set(StateFlag::FRMOF);
	unthum();
	thred::redraw((*ButtonWin)[HHID]);
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::dulin() {
	SelectObject(StitchWindowDC, LinePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	if (MoveLine0[0].x == MoveLine1[1].x && MoveLine0[0].y == MoveLine1[1].y) {
		if (StateMap.test(StateFlag::ISDWN)) {
			Polyline(StitchWindowDC, MoveLine0, 2);
		}
		else {
			Polyline(StitchWindowDC, MoveLine1, 2);
		}
	}
	else {
		if (StateMap.test(StateFlag::ISDWN))
			Polyline(StitchWindowDC, MoveLine0, 2);
		if (StateMap.test(StateFlag::ISUP))
			Polyline(StitchWindowDC, MoveLine1, 2);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
	StateMap.flip(StateFlag::WASLIN);
}

void thred::internal::unlin() {
	if (StateMap.test(StateFlag::WASLIN))
		dulin();
}

void thred::internal::movbox() {
	if (stch2px(ClosestPointIndex)) {
		unbox();
		OutputDebugString(fmt::format(L"Stitch:{} form:{} type:{}\n",
		                              ClosestPointIndex,
		                              ((StitchBuffer[ClosestPointIndex].attribute & FRMSK) >> FRMSHFT),
		                              ((StitchBuffer[ClosestPointIndex].attribute & TYPMSK) >> TYPSHFT))
		                      .c_str());
		dubox();
		if (StateMap.test(StateFlag::UPTO))
			StateMap.set(StateFlag::RESTCH);
	}
	else {
		shft2box();
		StateMap.set(StateFlag::SELBOX);
		StateMap.reset(StateFlag::FRMPSEL);
		StateMap.set(StateFlag::RESTCH);
	}
	nuAct(ClosestPointIndex);
	ritcor(StitchBuffer[ClosestPointIndex]);
}

bool thred::internal::chkhid(unsigned colorToCheck) {
	if (StateMap.test(StateFlag::HID)) {
		if (ColorChangeTable[colorToCheck].colorIndex == ActiveColor)
			return 1;
		else
			return 0;
	}
	else
		return 1;
}

/*
This function attempts to find the stitch the user is trying to select.
A rectangle is created that is slightly larger than the stitch.
If the mouse position is outside this rectangle, the stitch is disqualified.
If the mouse position is inside the rectangle, the distance from the stitch
 to the select point (SelectedPoint) is calculated.

Find the equation for the line by solving the linear parametric eauations

  StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y=offset
  StitchBuffer[StitchIndex+1].x+Slope*StitchBuffer[StitchIndex+1].y=offset

substituting:

  StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y=StitchBuffer[StitchIndex+1].x+Slope*StitchBuffer[StitchIndex+1].y

collecting terms:

  Slope*StitchBuffer[StitchIndex].y-Slope*StitchBuffer[StitchIndex+1].y=StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x
  Slope*(StitchBuffer[StitchIndex].y-StitchBuffer[StitchIndex+1].y)=StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x
  Slope=(StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x)/(StitchBuffer[StitchIndex].y-StitchBuffer[StitchIndex+1].y)

define xba=StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x
define yab=StitchBuffer[StitchIndex].y-StitchBuffer[StitchIndex+1].y

  Slope=xba/yab;

back substituting into StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y=offset

  offset=StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y

The equation for a point vertical to the equation for the line and running
 through SelectedPoint is:

  SelectedPoint.x-SelectedPoint.y/Slope=poff

If intersection is the intersections between the two lines then

  intersection.x-Slope*intersection.y=offset
  intersection.x+intersection.y/Slope=poff

Subtracting the two equations

  Slope*intersection.y+intersection.y/Slope=offset-poff

Multiply by Slope

  Slope*Slope*intersection.y+intersection.y=Slope(offset-poff)
  intersection.y(Slope*Slope+1)=Slope(offset-poff)
  intersection.y=Slope*(offset-poff)/(Slope*Slope+1)

back substitute into intersection.x+Slope*intersection.y=offset

  intersection.x=offset-Slope*intersection.y

if dx=intersection.x-spnt.x & dy=intersection.y-spnt.y

the distance from the point to the line is given by

  sqrt(dx*dx+dy*dy)
*/

unsigned thred::internal::closlin() {
	// ToDo - This function needs to be thoroughly checked and compared to findDistanceToSide

	unsigned   iStitch = 0, iChange = 0, stitchCount = 0, closestPoint = 0xffffffff;
	double     sum = 1e99;
	long       xba = 0, yab = 0;
	double     tsum         = 0.0;
	dRECTANGLE boundingRect = {};
	double     slope = 0.0, offset = 0.0, poff = 0.0, tolerance = 0.0, dx = 0.0, dy = 0.0;
	dPOINT     intersection = {};
	dPOINT     checkedPoint = {};
	unsigned   layer        = 0;

#define TOL 20
	thi::unboxs();
	offset = static_cast<float>(Msg.pt.x - StitchWindowAbsRect.left) / (StitchWindowAbsRect.right - StitchWindowAbsRect.left);
	checkedPoint.x = offset * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	offset = static_cast<float>(StitchWindowAbsRect.bottom - Msg.pt.y) / (StitchWindowAbsRect.bottom - StitchWindowAbsRect.top);
	checkedPoint.y = (offset * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom);
	offset         = static_cast<double>(ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
	tolerance      = offset * TOL;
	for (iChange = 0; iChange < ColorChanges; iChange++) {
		stitchCount                = ColorChangeTable[iChange + 1].stitchIndex - ColorChangeTable[iChange].stitchIndex;
		const fPOINTATTR* stitches = &StitchBuffer[ColorChangeTable[iChange].stitchIndex];
		if (stitches) {
			if (thi::chkhid(iChange)) {
				for (iStitch = 0; iStitch < stitchCount; iStitch++) {
					layer = (stitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
					if (!ActiveLayer || !layer || (layer == ActiveLayer)) {
						xba = stitches[iStitch + 1].x - stitches[iStitch].x;
						yab = stitches[iStitch].y - stitches[iStitch + 1].y;
						if (xba > 0) {
							boundingRect.left  = stitches[iStitch].x - tolerance;
							boundingRect.right = stitches[iStitch + 1].x + tolerance;
						}
						else {
							boundingRect.left  = stitches[iStitch + 1].x - tolerance;
							boundingRect.right = stitches[iStitch].x + tolerance;
						}
						if (yab < 0) {
							boundingRect.bottom = stitches[iStitch].y - tolerance;
							boundingRect.top    = stitches[iStitch + 1].y + tolerance;
						}
						else {
							boundingRect.bottom = stitches[iStitch + 1].y - tolerance;
							boundingRect.top    = stitches[iStitch].y + tolerance;
						}
						if (checkedPoint.x > boundingRect.left && checkedPoint.x < boundingRect.right
						    && checkedPoint.y > boundingRect.bottom && checkedPoint.y < boundingRect.top) {
							do {
								if (yab == 0) {
									// stitch is horizontal
									intersection.x = checkedPoint.x;
									intersection.y = stitches[iStitch].y;
									tsum = ((stitches[iStitch].y > checkedPoint.y) ? (stitches[iStitch].y - checkedPoint.y)
									                                               : (checkedPoint.y - stitches[iStitch].y));
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
									slope          = static_cast<double>(xba) / yab;
									offset         = stitches[iStitch].x + slope * stitches[iStitch].y;
									poff           = checkedPoint.x - checkedPoint.y / slope;
									intersection.y = slope * (offset - poff) / (slope * slope + 1);
									intersection.x = offset - slope * intersection.y;
									dx             = intersection.x - checkedPoint.x;
									dy             = intersection.y - checkedPoint.y;
									tsum           = hypot(dx, dy);
								}
								boundingRect.bottom += tolerance;
								boundingRect.top -= tolerance;
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
	}
	if (sum > tolerance)
		return 0xffffffff;
	else
		return closestPoint;
}

void thred::internal::istch() {
	double angb = 0.0, angf = 0.0, angt = 0.0;

	xlin();
	xlin1();
	if (StateMap.test(StateFlag::SELBOX)) {
		if (ClosestPointIndex && ClosestPointIndex != gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			thred::px2stch();
			angt
			    = atan2(StitchBuffer[ClosestPointIndex].y - SelectedPoint.y, StitchBuffer[ClosestPointIndex].x - SelectedPoint.x);
			angb = atan2(StitchBuffer[ClosestPointIndex].y - StitchBuffer[ClosestPointIndex - 1].y,
			             StitchBuffer[ClosestPointIndex].x - StitchBuffer[ClosestPointIndex - 1].x);
			angf = atan2(StitchBuffer[ClosestPointIndex].y - StitchBuffer[ClosestPointIndex + 1].y,
			             StitchBuffer[ClosestPointIndex].x - StitchBuffer[ClosestPointIndex + 1].x);
			if (fabs(angf - angt) > fabs(angb - angt))
				ClosestPointIndex--;
		}
		else {
			if (ClosestPointIndex == gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
				ClosestPointIndex--;
		}
	}
	else
		ClosestPointIndex = closlin();
	if (ClosestPointIndex == -1)
		StateMap.reset(StateFlag::INSRT);
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
	unsigned iStitch = 0, color = 0;

	if (PCSHeader.stitchCount) {
		if (StateMap.test(StateFlag::SELBOX))
			iStitch = ClosestPointIndex;
		else {
			if (StateMap.test(StateFlag::GRPSEL))
				iStitch = GroupStitchIndex;
			else
				iStitch = 0;
		}
		if (iStitch > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			iStitch = PCSHeader.stitchCount - 1;
		ClosestPointIndex = GroupStitchIndex = iStitch;
		color                                = StitchBuffer[iStitch].attribute & COLMSK;
		while (ClosestPointIndex && (StitchBuffer[ClosestPointIndex].attribute & COLMSK) == color)
			ClosestPointIndex--;
		if ((StitchBuffer[ClosestPointIndex].attribute & COLMSK) != color)
			ClosestPointIndex++;
		while (GroupStitchIndex < PCSHeader.stitchCount && (StitchBuffer[GroupStitchIndex].attribute & COLMSK) == color)
			GroupStitchIndex++;
		if ((StitchBuffer[ClosestPointIndex].attribute & COLMSK) != color)
			ClosestPointIndex--;
		if (GroupStitchIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			GroupStitchIndex = PCSHeader.stitchCount - 1;
		StateMap.set(StateFlag::GRPSEL);
		unbox();
		thred::grpAdj();
		ActiveColor = color;
		StateMap.set(StateFlag::RESTCH);
	}
}

void thred::internal::newFil() {
	unsigned iColor = 0;

	StateMap.reset(StateFlag::CMPDO);
	if (PCSBMPFileName[0]) {
		PCSBMPFileName[0] = 0;
		DeleteObject(BitmapFileHandle);
		ReleaseDC(ThrEdWindow, BitmapDC);
	}
	deldu();
	DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
	SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRED], *DesignerName).c_str());
	*ThrName = *DefaultDirectory / ((*StringTable)[STR_NUFIL].c_str());
	ritfnam(*DesignerName);
	auto designer = utf::Utf16ToUtf8(*DesignerName);
	std::copy(designer.begin(), designer.end(), ExtendedHeader.modifierName);
	rstdu();
	rstAll();
	displayText::clrhbut(3);
	StateMap.reset(StateFlag::MOVSET);
	PCSHeader.leadIn     = 0x32;
	PCSHeader.colorCount = 16;
	unbox();
	xlin();
	xlin1();
	StateMap.reset(StateFlag::INIT);
	StateMap.reset(StateFlag::INSRT);
	StateMap.reset(StateFlag::LIN1);
	StateMap.reset(StateFlag::FORMSEL);
	StateMap.reset(StateFlag::BAKACT);
	StateMap.reset(StateFlag::GMRK);
	PCSHeader.stitchCount = 0;
	DisplayedColorBitmap.reset();
	PCSBMPFileName[0]     = 0;
	PCSHeader.stitchCount = 0;
	FormVertexIndex       = 0;
	ClipPointIndex        = 0;
	TextureIndex          = 0;
	satin::clearGuideSize();
	FormIndex    = 0;
	ColorChanges = 0;
	KnotCount    = 0;
	WorkingFileName->clear();
	for (iColor = 0; iColor < 16; iColor++) {
		thred::redraw(DefaultColorWin[iColor]);
		thred::redraw(UserColorWin[iColor]);
		thred::redraw(ThreadSizeWin[iColor]);
	}
	thred::zumhom();
}

void thred::bBox() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, ZoomBoxLine, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unbBox() {
	if (StateMap.testAndReset(StateFlag::BZUM))
		thred::bBox();
}

void thred::internal::rebox() {
	unsigned iColor = 0;

	unbox();
	if (closPnt1(&ClosestPointIndex)) {
		nuAct(ClosestPointIndex);
		if (stch2px(ClosestPointIndex)) {
			dubox();
			OutputDebugString(fmt::format(L"Stitch:{} form:{} type:{}\n",
			                              ClosestPointIndex,
			                              ((StitchBuffer[ClosestPointIndex].attribute & FRMSK) >> FRMSHFT),
			                              ((StitchBuffer[ClosestPointIndex].attribute & TYPMSK) >> TYPSHFT))
			                      .c_str());
		}
		if (StateMap.testAndReset(StateFlag::GRPSEL)) {
			StateMap.reset(StateFlag::SCROS);
			StateMap.reset(StateFlag::ECROS);
			SearchLineIndex = 0;
			StateMap.set(StateFlag::RESTCH);
			for (iColor = 0; iColor < 16; iColor++)
				thred::redraw(UserColorWin[iColor]);
		}
		ritcor(StitchBuffer[ClosestPointIndex]);
	}
}

void thred::delstchm() {
	unsigned iStitch = 0, destination = 0;

	thred::rngadj();
	destination = GroupStartStitch;
	for (iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[destination++] = StitchBuffer[iStitch];
	}
	PCSHeader.stitchCount = destination;
}

void thred::internal::duclp() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, ClipInsertBoxLine, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unclp() {
	if (StateMap.testAndReset(StateFlag::CLPSHO))
		duclp();
}

void thred::internal::clpbox() {
	SIZE         adjustedSize = {};
	const double ratio        = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);

	thred::px2stch();
	if (SelectedPoint.x + ClipRectSize.cx > UnzoomedRect.x)
		SelectedPoint.x = UnzoomedRect.x - ClipRectSize.cx;
	if (SelectedPoint.y + ClipRectSize.cy > UnzoomedRect.y)
		SelectedPoint.y = UnzoomedRect.y - ClipRectSize.cy;
	ClipOrigin.x              = SelectedPoint.x;
	ClipOrigin.y              = SelectedPoint.y;
	adjustedSize.cx           = ClipRectSize.cx * ratio + 0.5;
	adjustedSize.cy           = ClipRectSize.cy * ratio + 0.5;
	StitchCoordinatesPixels.x = (SelectedPoint.x - ZoomRect.left) * ratio + 0.5;
	StitchCoordinatesPixels.y
	    = StitchWindowClientRect.bottom - (SelectedPoint.y - ZoomRect.bottom) * ratio + 0.5 - adjustedSize.cy;
	ClipInsertBoxLine[0].x = ClipInsertBoxLine[3].x = ClipInsertBoxLine[4].x = StitchCoordinatesPixels.x;
	ClipInsertBoxLine[0].y = ClipInsertBoxLine[1].y = ClipInsertBoxLine[4].y = StitchCoordinatesPixels.y;
	ClipInsertBoxLine[1].x = ClipInsertBoxLine[2].x = ClipInsertBoxLine[0].x + adjustedSize.cx;
	ClipInsertBoxLine[2].y = ClipInsertBoxLine[3].y = ClipInsertBoxLine[0].y + adjustedSize.cy;
	StateMap.set(StateFlag::CLPSHO);
	duclp();
}

void thred::internal::lodclp(unsigned iStitch) {
	const fPOINT adjustment  = { (ClipOrigin.x - ClipRect.left), (ClipOrigin.y - ClipRect.bottom) };
	unsigned     source      = PCSHeader.stitchCount - 1;
	size_t       destination = PCSHeader.stitchCount + ClipStitchCount - 1;

	if (iStitch != PCSHeader.stitchCount)
		iStitch++;
	if (iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) && StateMap.test(StateFlag::INIT)) {
		while (source >= iStitch) {
			StitchBuffer[destination].x           = StitchBuffer[source].x;
			StitchBuffer[destination].y           = StitchBuffer[source--].y;
			StitchBuffer[destination--].attribute = StitchBuffer[source].attribute;
		}
	}
	ClosestPointIndex = iStitch;
	for (source = 0; source < ClipStitchCount; source++) {
		StitchBuffer[iStitch].x           = ClipBuffer[source].x + adjustment.x;
		StitchBuffer[iStitch].y           = ClipBuffer[source].y + adjustment.y;
		StitchBuffer[iStitch++].attribute = ClipBuffer[source].attribute & COLMSK | LayerIndex | NOTFRM;
	}
	GroupStitchIndex = iStitch - 1;
	StateMap.set(StateFlag::GRPSEL);
	PCSHeader.stitchCount += gsl::narrow<unsigned short>(ClipStitchCount);
	if (PCSHeader.stitchCount)
		StateMap.set(StateFlag::INIT);
}

void thred::internal::rSelbox() {
	const double ratio = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	SIZE         adjustedSelectSize
	    = { static_cast<LONG>(SelectBoxSize.cx * ratio + 0.5), static_cast<LONG>(SelectBoxSize.cy * ratio + 0.5) };

	unsel();
	thred::px2stch();
	if (SelectedPoint.x - SelectBoxOffset.x + SelectBoxSize.cx >= UnzoomedRect.x)
		SelectedPoint.x = UnzoomedRect.x - SelectBoxSize.cx + SelectBoxOffset.x;
	if (SelectedPoint.y - SelectBoxOffset.y + SelectBoxSize.cy >= UnzoomedRect.y)
		SelectedPoint.y = UnzoomedRect.y - SelectBoxSize.cy + SelectBoxOffset.y;
	if (SelectedPoint.x - SelectBoxOffset.x < 0)
		SelectedPoint.x = SelectBoxOffset.x;
	if (SelectedPoint.y - SelectBoxOffset.y < 0)
		SelectedPoint.y = SelectBoxOffset.y;
	StitchCoordinatesPixels.x = (SelectedPoint.x - ZoomRect.left - SelectBoxOffset.x) * ratio + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (SelectedPoint.y - ZoomRect.bottom - SelectBoxOffset.y) * ratio
	                            + 0.5 - adjustedSelectSize.cy;
	auto& formControls = *FormControlPoints;
	formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = StitchCoordinatesPixels.x;
	formControls[1].x = formControls[5].x = StitchCoordinatesPixels.x + adjustedSelectSize.cx / 2;
	formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = StitchCoordinatesPixels.y;
	formControls[3].y = formControls[7].y = StitchCoordinatesPixels.y + adjustedSelectSize.cy / 2;
	formControls[4].y = formControls[5].y = formControls[6].y = StitchCoordinatesPixels.y + adjustedSelectSize.cy;
	formControls[2].x = formControls[3].x = formControls[4].x = StitchCoordinatesPixels.x + adjustedSelectSize.cx;
	StateMap.set(StateFlag::SELSHO);
	dusel(StitchWindowDC);
}

void thred::internal::duSelbox() noexcept {
	thred::px2stch();
	SelectBoxSize.cx  = StitchRangeRect.right - StitchRangeRect.left;
	SelectBoxSize.cy  = StitchRangeRect.top - StitchRangeRect.bottom;
	SelectBoxOffset.x = SelectedPoint.x - StitchRangeRect.left;
	SelectBoxOffset.y = SelectedPoint.y - StitchRangeRect.bottom;
}

void thred::internal::setbak(unsigned penWidth) noexcept {
	if (BackgroundPenWidth != penWidth)
		BackgroundPen = nuPen(BackgroundPen, penWidth, BackgroundColor);
}

void thred::internal::stchbox(unsigned iStitch, HDC dc) noexcept {
	POINT          line[5] = {};
	const unsigned layer   = (StitchBuffer[iStitch].attribute & LAYMSK) >> LAYSHFT;

	if (!ActiveLayer || !layer || layer == ActiveLayer) {
		stch2px1(iStitch);
		line[0].x = line[3].x = line[4].x = StitchCoordinatesPixels.x - IniFile.stitchSizePixels;
		line[0].y = line[1].y = StitchCoordinatesPixels.y - IniFile.stitchSizePixels;
		line[1].x = line[2].x = StitchCoordinatesPixels.x + IniFile.stitchSizePixels;
		line[2].y = line[3].y = StitchCoordinatesPixels.y + IniFile.stitchSizePixels;
		line[4].y             = StitchCoordinatesPixels.y - IniFile.stitchSizePixels;
		Polyline(dc, line, 5);
	}
}

void thred::sCor2px(const dPOINT& stitchCoordinate, POINT& pixelCoordinate) noexcept {
	pixelCoordinate.x = (stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x + 0.5;
	pixelCoordinate.y = StitchWindowClientRect.bottom + (ZoomRect.bottom - stitchCoordinate.y) * ZoomRatio.y + 0.5;
}

void thred::internal::sdCor2px(const fPOINTATTR stitchPoint, POINT& pixelCoordinate) noexcept {
	pixelCoordinate.x = (stitchPoint.x - ZoomRect.left) * ZoomRatio.x + 0.5;
	pixelCoordinate.y = StitchWindowClientRect.bottom + (ZoomRect.bottom - stitchPoint.y) * ZoomRatio.y + 0.5;
}

void thred::internal::durot() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, RotateBoxOutline, 5);
	Polyline(StitchWindowDC, RotateBoxCrossVertLine, 2);
	Polyline(StitchWindowDC, RotateBoxCrossHorzLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unrot() {
	if (StateMap.testAndReset(StateFlag::ROTSHO))
		durot();
}

void thred::internal::durotu() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, RotateBoxToCursorLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unrotu() {
	if (StateMap.testAndReset(StateFlag::ROTUSHO))
		durotu();
}

void thred::internal::rotang(dPOINT unrotatedPoint, POINT& rotatedPoint, double rotationAngle, const dPOINT& rotationCenter) {
	double     distanceToCenter = 0.0, newAngle = 0.0;
	dPOINT     point = {};
	const long dx    = unrotatedPoint.x - rotationCenter.x;
	const long dy    = unrotatedPoint.y - rotationCenter.y;

	if (dx) {
		distanceToCenter = hypot(dx, dy);
		newAngle         = atan2(dy, dx);
		newAngle += rotationAngle;
	}
	else {
		distanceToCenter = abs(dy);
		if (dy > 0)
			newAngle = PI / 2 + rotationAngle;
		else
			newAngle = rotationAngle - PI / 2;
	}
	point.y = rotationCenter.y + distanceToCenter * sin(newAngle);
	point.x = rotationCenter.x + distanceToCenter * cos(newAngle);
	thred::sCor2px(point, rotatedPoint);
}

void thred::rotang1(const fPOINTATTR& unrotatedPoint,
             fPOINT&           rotatedPoint,
             const double&     rotationAngle,
             const dPOINT&     rotationCenter) noexcept {
	double       distanceToCenter = 0.0, newAngle = 0.0;
	const double dx = unrotatedPoint.x - rotationCenter.x;
	const double dy = unrotatedPoint.y - rotationCenter.y;

	if (dx) {
		distanceToCenter = hypot(dx, dy);
		newAngle         = atan2(dy, dx);
		newAngle += rotationAngle;
	}
	else {
		if (dy > 0) {
			distanceToCenter = dy;
			newAngle         = PI / 2 + rotationAngle;
		}
		else {
			distanceToCenter = -dy;
			newAngle         = rotationAngle - PI / 2;
		}
	}
	rotatedPoint.y = rotationCenter.y + distanceToCenter * sin(newAngle);
	rotatedPoint.x = rotationCenter.x + distanceToCenter * cos(newAngle);
}

void thred::rotangf(const fPOINT& unrotatedPoint,
             fPOINT&       rotatedPoint,
             const double  rotationAngle,
             const dPOINT& rotationCenter) noexcept {
	double       distanceToCenter = 0.0, newAngle = 0.0;
	const double dx = unrotatedPoint.x - rotationCenter.x;
	const double dy = unrotatedPoint.y - rotationCenter.y;

	if (dx) {
		distanceToCenter = hypot(dx, dy);
		newAngle         = atan2(dy, dx);
		newAngle += rotationAngle;
	}
	else {
		if (dy > 0) {
			distanceToCenter = dy;
			newAngle         = PI / 2 + rotationAngle;
		}
		else {
			distanceToCenter = -dy;
			newAngle         = rotationAngle - PI / 2;
		}
	}
	rotatedPoint.y = rotationCenter.y + distanceToCenter * sin(newAngle);
	rotatedPoint.x = rotationCenter.x + distanceToCenter * cos(newAngle);
}

void thred::rotflt(fPOINT& point, const double rotationAngle, const dPOINT& rotationCenter) noexcept {
	double       len = 0.0, newAngle = 0.0;
	const double dx = point.x - rotationCenter.x;
	const double dy = point.y - rotationCenter.y;

	if (dx) {
		len      = hypot(dx, dy);
		newAngle = atan2(dy, dx);
		newAngle += rotationAngle;
	}
	else {
		if (dy > 0) {
			len      = dy;
			newAngle = PI / 2 + rotationAngle;
		}
		else {
			len      = -dy;
			newAngle = rotationAngle - PI / 2;
		}
	}
	point.y = rotationCenter.y + len * sin(newAngle);
	point.x = rotationCenter.x + len * cos(newAngle);
}

void thred::internal::rotstch(fPOINTATTR* stitch, const double rotationAngle, const dPOINT& rotationCenter) noexcept {
	double       distanceToCenter = 0.0, newAngle = 0.0;
	const double dx = stitch->x - rotationCenter.x;
	const double dy = stitch->y - rotationCenter.y;

	if (dx) {
		distanceToCenter = hypot(dx, dy);
		newAngle         = atan2(dy, dx);
		newAngle += rotationAngle;
	}
	else {
		if (dy > 0) {
			distanceToCenter = dy;
			newAngle         = PI / 2 + rotationAngle;
		}
		else {
			distanceToCenter = -dy;
			newAngle         = rotationAngle - PI / 2;
		}
	}
	stitch->y = rotationCenter.y + distanceToCenter * sin(newAngle);
	stitch->x = rotationCenter.x + distanceToCenter * cos(newAngle);
}

void thred::internal::ritrot(double rotationAngle, const dPOINT& rotationCenter) {
	POINT  rotated           = {};
	dPOINT rotationReference = { (RotationRect.left), (RotationRect.top) };

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

void thred::internal::durcntr(dPOINT& rotationCenter) noexcept {
	rotationCenter.x = form::midl(RotationRect.right, RotationRect.left);
	rotationCenter.y = form::midl(RotationRect.top, RotationRect.bottom);
}

void thred::internal::rot(dPOINT& rotationCenter) {
	do {
		if (StateMap.test(StateFlag::FPSEL)) {
			RotationRect = SelectedVerticesRect;
			break;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			RotationRect = AllItemsRect;
			break;
		}
		if (SelectedFormList->size()) {
			form::pxrct2stch(SelectedFormsRect, RotationRect);
			StateMap.set(StateFlag::FRMSROT);
			break;
		}
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			StateMap.set(StateFlag::FRMROT);
			RotationRect = SelectedForm->rectangle;
			break;
		}
		if (StateMap.test(StateFlag::GRPSEL)) {
			thred::rngadj();
			thred::selRct(RotationRect);
			break;
		}
		displayText::shoseln(IDS_FGRPF, IDS_ROT);
		return;
	} while (false);
	StateMap.set(StateFlag::ROTAT);
	durcntr(rotationCenter);
	ritrot(0, rotationCenter);
}

void thred::internal::savclp(size_t destination, size_t source) noexcept {
	double frct = 0.0, intg = 0.0;

	ClipStitchData[destination].led  = StitchBuffer[source].attribute & COLMSK;
	frct                             = modf(static_cast<double>(StitchBuffer[source].x) - LowerLeftStitch.x, &intg);
	ClipStitchData[destination].fx   = frct * 256;
	ClipStitchData[destination].x    = intg;
	ClipStitchData[destination].spcx = 0;
	frct                             = modf(static_cast<double>(StitchBuffer[source].y) - LowerLeftStitch.y, &intg);
	ClipStitchData[destination].fy   = frct * 256;
	ClipStitchData[destination].y    = intg;
	ClipStitchData[destination].spcy = 0;
	// ToDo - Are these structure members needed?
	ClipStitchData[destination].myst = 1;
	ClipStitchData[destination].tag  = 0x14;
}

void thred::rtclpfn(size_t destination, size_t source) noexcept {
	double fractional = 0.0, integer = 0.0;

	ClipStitchData[destination].led  = 0;
	fractional                       = modf(ClipBuffer[source].x - LowerLeftStitch.x, &integer);
	ClipStitchData[destination].fx   = fractional;
	ClipStitchData[destination].x    = integer;
	ClipStitchData[destination].spcx = 0;
	fractional                       = modf(ClipBuffer[source].y - LowerLeftStitch.y, &integer);
	ClipStitchData[destination].fy   = fractional;
	ClipStitchData[destination].y    = integer;
	ClipStitchData[destination].spcy = 0;
	// ToDo - Are these structure members needed?
	ClipStitchData[destination].myst = 1;
	ClipStitchData[destination].tag  = 0x14;
}

size_t thred::internal::sizfclp() noexcept {
	size_t clipSize;

	clipSize = sizeof(*ClipFormHeader) + VertexCount * sizeof(FormVertices[0]);
	if (SelectedForm->type == SAT)
		clipSize += SelectedForm->satinGuideCount * sizeof(SatinGuides[0]);
	if (clip::iseclp(ClosestFormToCursor))
		clipSize += SelectedForm->clipEntries * sizeof(ClipPoints[0]);
	if (clip::isclpx(ClosestFormToCursor))
		clipSize += SelectedForm->lengthOrCount.clipCount * sizeof(ClipPoints[0]);
	if (texture::istx(ClosestFormToCursor))
		clipSize += SelectedForm->fillInfo.texture.count * sizeof(TexturePointsBuffer[0]);
	return clipSize;
}

unsigned thred::internal::frmcnt(size_t iForm, unsigned formFirstStitchIndex) noexcept {
	const size_t codedAttribute = iForm << FRMSHFT;
	unsigned     stitchCount = 0, iStitch = 0;

	LowerLeftStitch.x = LowerLeftStitch.y = 1e20f;
	bool flag                             = true;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute && StitchBuffer[iStitch].attribute & TYPMSK) {
			flag = false;
			break;
		}
	}
	if (flag) {
		return 0;
	}
	else {
		formFirstStitchIndex = iStitch;
		while (iStitch < PCSHeader.stitchCount) {
			if ((StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute && StitchBuffer[iStitch].attribute & TYPMSK) {
				if (StitchBuffer[iStitch].x < LowerLeftStitch.x)
					LowerLeftStitch.x = StitchBuffer[iStitch].x;
				if (StitchBuffer[iStitch].y < LowerLeftStitch.y)
					LowerLeftStitch.y = StitchBuffer[iStitch].y;
				stitchCount++;
			}
			iStitch++;
		}
		return stitchCount;
	}
}

size_t thred::internal::sizclp(unsigned formFirstStitchIndex) noexcept {
	size_t length = 0;

	length = FileSize = sizeof(*ClipFormHeader) + VertexCount * sizeof(FormVertices[0]);
	if (SelectedForm->type == SAT)
		FileSize += SelectedForm->satinGuideCount * sizeof(SatinGuides[0]);
	if (SelectedForm->fillType || SelectedForm->edgeType) {
		length += frmcnt(ClosestFormToCursor, formFirstStitchIndex);
		FileSize += length * sizeof(StitchBuffer[0]);
	}
	if (clip::iseclp(ClosestFormToCursor))
		FileSize += SelectedForm->clipEntries * sizeof(ClipPoints[0]);
	if (clip::isclpx(ClosestFormToCursor))
		FileSize += SelectedForm->lengthOrCount.clipCount * sizeof(ClipPoints[0]);
	if (texture::istx(ClosestFormToCursor))
		FileSize += SelectedForm->fillInfo.texture.count * sizeof(TexturePointsBuffer[0]);

	return length;
}

unsigned thred::internal::lenclp() {
	unsigned codedAttribute = 0, iStitch = 0, stitchCount = 0;

	stitchCount    = 0;
	codedAttribute = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & (FRMSK | NOTFRM)) == codedAttribute)
			stitchCount++;
	}
	return stitchCount;
}

void thred::internal::duclip() {
	unsigned       iClip = 0, iDestination = 0, iForm = 0, iGuide = 0;
	unsigned       iSide = 0, iStitch = 0, iTexture = 0, iVertex = 0;
	size_t         iSource    = 0;
	unsigned       guideCount = 0, pointCount = 0, textureCount = 0, stitchCount = 0;
	unsigned       codedAttribute = 0;
	size_t         msiz           = 0;
	size_t         length         = 0;
	const unsigned firstStitch    = 0; // points to the first stitch in a form

	if (StateMap.test(StateFlag::FPSEL)) {
		if (OpenClipboard(ThrEdWindow)) {
			EmptyClipboard();
			ThrEdClip        = RegisterClipboardFormat(ThrEdClipFormat);
			ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
			                               (SelectedFormVertices.vertexCount + 1) * sizeof(fPOINT) + sizeof(FORMVERTEXCLIP));
			[[gsl::suppress(26429)]] {
				if (ThrEdClipPointer) {
					FORMVERTEXCLIP* clipHeader = *(static_cast<FORMVERTEXCLIP**>(ThrEdClipPointer));
					clipHeader->clipType       = CLP_FRMPS;
					clipHeader->vertexCount    = SelectedFormVertices.vertexCount;
					if (StateMap.test(StateFlag::PSELDIR))
						clipHeader->direction = 1;
					else
						clipHeader->direction = 0;
					// skip past the header
					fPOINT* vertices = convert_ptr<fPOINT*>(&clipHeader[1]);
					form::fvars(ClosestFormToCursor);
					iSource = SelectedFormVertices.start;
					for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
						vertices[iVertex] = CurrentFormVertices[iSource];
						iSource           = form::pdir(iSource);
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
	if (StateMap.test(StateFlag::BIGBOX))
		displayText::tabmsg(IDS_INSF);
	else {
		if (OpenClipboard(ThrEdWindow)) {
			EmptyClipboard();
			ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
			if (SelectedFormList->size()) {
				for (auto selectedForm : (*SelectedFormList)) {
					ClosestFormToCursor = selectedForm;
					form::fvars(ClosestFormToCursor);
					length += sizfclp();
					msiz += FileSize;
				}
				ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, msiz + length);
				[[gsl::suppress(26429)]] {
					if (ThrEdClipPointer) {
						ClipFormsHeader            = *(static_cast<FORMSCLIP**>(ThrEdClipPointer));
						ClipFormsHeader->clipType  = CLP_FRMS;
						ClipFormsHeader->formCount = gsl::narrow<unsigned short>(SelectedFormList->size());
						// Skip past the header
						FRMHED* forms = convert_ptr<FRMHED*>(&ClipFormsHeader[1]);
						iForm         = 0;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm   = &FormList[selectedForm];
							forms[iForm++] = FormList[selectedForm];
						}
						// skip past the forms
						CurrentFormVertices = convert_ptr<fPOINT*>(&forms[iForm]);
						iVertex             = 0;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &FormList[selectedForm];
							for (iSide = 0; iSide < SelectedForm->vertexCount; iSide++) {
								CurrentFormVertices[iVertex++] = SelectedForm->vertices[iSide];
							}
						}
						// skip past the vertex list
						SATCON* guides = convert_ptr<SATCON*>(&CurrentFormVertices[iVertex]);
						guideCount     = 0;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &FormList[selectedForm];
							if (SelectedForm->type == SAT) {
								for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
									guides[guideCount++] = SelectedForm->satinOrAngle.guide[iGuide];
								}
							}
						}
						// skip past the guides
						fPOINT* points = convert_ptr<fPOINT*>(&guides[guideCount]);
						pointCount     = 0;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &FormList[selectedForm];
							if (clip::isclpx(selectedForm)) {
								for (iClip = 0; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {
									points[pointCount++] = SelectedForm->angleOrClipData.clip[iClip];
								}
							}
							if (clip::iseclp(selectedForm)) {
								for (iClip = 0; iClip < SelectedForm->clipEntries; iClip++) {
									points[pointCount++] = SelectedForm->borderClipData[iClip];
								}
							}
						}
						// Skip past the points
						TXPNT* textures = convert_ptr<TXPNT*>(&points[pointCount]);
						textureCount    = 0;
						iForm           = 0;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &FormList[selectedForm];
							if (texture::istx(selectedForm)) {
								auto startPoint = TexturePointsBuffer->cbegin() + SelectedForm->fillInfo.texture.index;
								auto endPoint   = startPoint + SelectedForm->fillInfo.texture.count;
								std::copy(startPoint,
								          endPoint,
								          stdext::make_checked_array_iterator(&textures[textureCount],
								                                              SelectedForm->fillInfo.texture.count));
								forms[iForm++].fillInfo.texture.index = textureCount;
								textureCount += SelectedForm->fillInfo.texture.count;
							}
						}
						SetClipboardData(ThrEdClip, ThrEdClipPointer);
					}
				}
				CloseClipboard();
				boost::dynamic_bitset<> formMap(FormIndex);
				for (auto selectedForm : (*SelectedFormList)) {
					formMap.set(selectedForm);
				}
				// ToDo - what is astch used for?
				fPOINTATTR* astch = &StitchBuffer[MAXITEMS];
				stitchCount       = 0;
				LowerLeftStitch.x = LowerLeftStitch.y = 1e30f;
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
					if (!(StitchBuffer[iStitch].attribute & NOTFRM)
					    && formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
						if (StitchBuffer[iStitch].x < LowerLeftStitch.x)
							LowerLeftStitch.x = StitchBuffer[iStitch].x;
						if (StitchBuffer[iStitch].y < LowerLeftStitch.y)
							LowerLeftStitch.y = StitchBuffer[iStitch].y;
						astch[stitchCount++] = StitchBuffer[iStitch];
					}
				}
				if (PCSHeader.stitchCount && stitchCount) {
					Clip        = RegisterClipboardFormat(PcdClipFormat);
					ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2);
					if (ClipPointer) {
						ClipStitchData = *(static_cast<CLPSTCH**>(ClipPointer));
						iStitch        = MAXITEMS;
						iDestination   = 0;
						savclp(0, iStitch);
						iStitch++;
						ClipStitchData[0].led = stitchCount;
						iDestination++;
						while (iStitch < stitchCount + MAXITEMS)
							savclp(iDestination++, iStitch++);
						SetClipboardData(Clip, ClipPointer);
					}
					CloseClipboard();
				}
			}
			else {
				if (StateMap.test(StateFlag::FORMSEL)) {
					length = sizclp(firstStitch);
					form::fvars(ClosestFormToCursor);
					codedAttribute = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
					FileSize += sizeof(*ClipFormHeader);
					ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, FileSize);
					[[gsl::suppress(26429)]] {
						if (ThrEdClipPointer) {
							ClipFormHeader           = *(static_cast<FORMCLIP**>(ThrEdClipPointer));
							ClipFormHeader->clipType = CLP_FRM;
							ClipFormHeader->form     = FormList[ClosestFormToCursor];
							CurrentFormVertices      = convert_ptr<fPOINT*>(&ClipFormHeader[1]);
							for (iSide = 0; iSide < SelectedForm->vertexCount; iSide++) {
								CurrentFormVertices[iSide] = SelectedForm->vertices[iSide];
							}
							SATCON* guides = convert_ptr<SATCON*>(&CurrentFormVertices[VertexCount]);
							iGuide         = 0;
							if (SelectedForm->type == SAT) {
								for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
									guides[iGuide] = SelectedForm->satinOrAngle.guide[iGuide];
								}
							}
							fPOINT* mclp = convert_ptr<fPOINT*>(&guides[iGuide]);
							iClip        = 0;
							if (clip::isclpx(ClosestFormToCursor)) {
								for (iClip = 0; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {
									mclp[iClip] = SelectedForm->angleOrClipData.clip[iClip];
								}
							}
							fPOINT* points = convert_ptr<fPOINT*>(&mclp[iClip]);
							if (clip::iseclpx(ClosestFormToCursor)) {
								for (iClip = 0; iClip < SelectedForm->clipEntries; iClip++) {
									points[iClip] = SelectedForm->borderClipData[iClip];
								}
							}
							TXPNT* textures = convert_ptr<TXPNT*>(&points[iClip]);
							if (texture::istx(ClosestFormToCursor)) {
								auto startPoint = TexturePointsBuffer->cbegin() + SelectedForm->fillInfo.texture.index;
								auto endPoint   = startPoint + SelectedForm->fillInfo.texture.count;
								std::copy(startPoint,
								          endPoint,
								          stdext::make_checked_array_iterator(textures, SelectedForm->fillInfo.texture.count));
							}
							SetClipboardData(ThrEdClip, ThrEdClipPointer);
						}
						iTexture = 0;
						if ((SelectedForm->fillType || SelectedForm->edgeType)) {
							length      = lenclp();
							Clip        = RegisterClipboardFormat(PcdClipFormat);
							ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length * sizeof(CLPSTCH) + 2);
							if (ClipPointer) {
								ClipStitchData = *(static_cast<CLPSTCH**>(ClipPointer));
								iStitch        = firstStitch;
								savclp(0, iTexture);
								ClipStitchData[0].led = length;
								iTexture++;
								iDestination   = 1;
								codedAttribute = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
								while (iTexture < PCSHeader.stitchCount) {
									if ((StitchBuffer[iTexture].attribute & FRMSK) == codedAttribute
									    && !(StitchBuffer[iTexture].attribute & NOTFRM))
										savclp(iDestination++, iTexture);
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
					if (PCSHeader.stitchCount && StateMap.test(StateFlag::GRPSEL)) {
						Clip = RegisterClipboardFormat(PcdClipFormat);
						thred::rngadj();
						LowerLeftStitch.x = LowerLeftStitch.y = 1e30f;
						for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
							if (StitchBuffer[iStitch].x < LowerLeftStitch.x)
								LowerLeftStitch.x = StitchBuffer[iStitch].x;
							if (StitchBuffer[iStitch].y < LowerLeftStitch.y)
								LowerLeftStitch.y = StitchBuffer[iStitch].y;
						}
						length      = GroupEndStitch - GroupStartStitch + 1;
						iSource     = GroupStartStitch;
						ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length * sizeof(CLPSTCH) + 2);
						if (ClipPointer) {
							ClipStitchData = *(static_cast<CLPSTCH**>(ClipPointer));
							savclp(0, iSource);
							ClipStitchData[0].led = length;
							iSource++;
							for (iStitch = 1; iStitch < length; iStitch++)
								savclp(iStitch, iSource++);
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

void thred::delfstchs() noexcept {
	unsigned iSourceStitch = 0, iDestinationStitch = 0;

	for (iSourceStitch = 0; iSourceStitch < PCSHeader.stitchCount; iSourceStitch++) {
		if (StitchBuffer[iSourceStitch].attribute & NOTFRM) {
			StitchBuffer[iDestinationStitch++] = StitchBuffer[iSourceStitch];
		}
		else {
			if (((StitchBuffer[iSourceStitch].attribute & FRMSK) >> 4) != ClosestFormToCursor) {
				StitchBuffer[iDestinationStitch++] = StitchBuffer[iSourceStitch];
			}
		}
	}
	PCSHeader.stitchCount = iDestinationStitch;
}

void thred::internal::f1del() {
	unsigned cod = 0, iStitch = 0, stitchCount = 0;

	if (StateMap.test(StateFlag::DELTO)) {
		cod         = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
		stitchCount = 0;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (FRMSK | NOTFRM)) != cod) {
				StitchBuffer[stitchCount] = StitchBuffer[iStitch];
				stitchCount++;
			}
		}
		PCSHeader.stitchCount = stitchCount;
	}
	clip::deleclp(ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	satin::delsac(ClosestFormToCursor);
	form::delflt(ClosestFormToCursor);
	texture::deltx();
}

void thred::frmdel() {
	unsigned stitchForm = 0, stitchAttributeFormBits = 0;

	form::fvars(ClosestFormToCursor);
	thi::f1del();
	for (auto iForm = ClosestFormToCursor; iForm < FormIndex; iForm++) {
		FormList[iForm] = FormList[iForm + 1];
	}
	if (StateMap.testAndReset(StateFlag::DELTO)) {
		for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM)) {
				stitchAttributeFormBits = (StitchBuffer[iStitch].attribute & FRMSK);
				stitchForm              = stitchAttributeFormBits >> 4;
				if (stitchForm > ClosestFormToCursor) {
					StitchBuffer[iStitch].attribute &= NFRMSK;
					StitchBuffer[iStitch].attribute |= (stitchAttributeFormBits - 0x10);
				}
			}
		}
	}
	else {
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM)) {
				stitchAttributeFormBits = (StitchBuffer[iStitch].attribute & FRMSK);
				stitchForm              = stitchAttributeFormBits >> FRMSHFT;
				if (stitchForm == ClosestFormToCursor)
					StitchBuffer[iStitch].attribute &= (NFRMSK & NTYPMSK);
				if (stitchForm > ClosestFormToCursor) {
					StitchBuffer[iStitch].attribute &= NFRMSK;
					StitchBuffer[iStitch].attribute |= (stitchForm - 1) << FRMSHFT;
				}
			}
		}
	}
	FormIndex--;
	StateMap.reset(StateFlag::FORMSEL);
}

void thred::internal::delsfrms(unsigned code) {
	unsigned iForm          = 0;
	unsigned validFormCount = 0, deletedFormCount = 0, iStitch = 0, validStitchCount = 0;

	if (code) {
		if (FormIndex) {
			std::vector<unsigned>   formIndices(FormIndex);
			boost::dynamic_bitset<> formMap(FormIndex);
			for (auto selectedForm : (*SelectedFormList)) {
				ClosestFormToCursor = selectedForm;
				formMap.set(ClosestFormToCursor);
				form::fvars(ClosestFormToCursor);
				f1del();
			}
			validFormCount   = 0;
			deletedFormCount = 0;
			for (iForm = 0; iForm < FormIndex; iForm++) {
				if (!formMap.test(iForm)) {
					if (validFormCount != iForm) {
						FormList[validFormCount] = FormList[iForm];
					}
					formIndices[iForm] = (iForm - deletedFormCount) << FRMSHFT;
					validFormCount++;
				}
				else
					deletedFormCount++;
			}
			FormIndex        = validFormCount;
			validStitchCount = 0;
			if (StateMap.test(StateFlag::DELTO)) {
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
					if (StitchBuffer[iStitch].attribute & ALTYPMSK) {
						iForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
						if (!formMap.test(iForm)) {
							StitchBuffer[validStitchCount].attribute = StitchBuffer[iStitch].attribute &= NFRMSK;
							StitchBuffer[validStitchCount].attribute |= formIndices[iForm];
							StitchBuffer[validStitchCount].x   = StitchBuffer[iStitch].x;
							StitchBuffer[validStitchCount++].y = StitchBuffer[iStitch].y;
						}
					}
					else {
						StitchBuffer[validStitchCount++] = StitchBuffer[iStitch];
					}
				}
				PCSHeader.stitchCount = validStitchCount;
			}
			else {
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
					if (!(StitchBuffer[iStitch].attribute & NOTFRM)) {
						iForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
						if (formMap.test(iForm)) {
							StitchBuffer[iStitch].attribute &= (NFRMSK & NTYPMSK);
						}
						else {
							StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute &= NFRMSK;
							StitchBuffer[iStitch].attribute |= formIndices[iForm];
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
	duclip();
	if (SelectedFormList->size()) {
		StateMap.set(StateFlag::DELTO);
		delsfrms('s');
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::delfstchs();
			thred::frmdel();
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL))
				thred::delstchm();
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
bool thred::internal::cmpstch(unsigned iStitchA, unsigned iStitchB) noexcept {
	if (StitchBuffer[iStitchA].x != StitchBuffer[iStitchB].x)
		return 0;

	return StitchBuffer[iStitchA].y == StitchBuffer[iStitchB].y;
}

void thred::internal::ofstch(unsigned iSource, char offset) noexcept {
	StitchBuffer[OutputIndex].x           = StitchBuffer[iSource].x + KnotStep.x * offset;
	StitchBuffer[OutputIndex].y           = StitchBuffer[iSource].y + KnotStep.y * offset;
	StitchBuffer[OutputIndex++].attribute = KnotAttribute;
}

void thred::internal::endknt(unsigned finish) {
	double   length = 0.0;
	dPOINT   delta  = {};
	unsigned iStart = finish - 1, iKnot = 0;
	char*    knots = nullptr;

	KnotAttribute = StitchBuffer[iStart].attribute | KNOTMSK;
	do {
		delta.x = StitchBuffer[finish].x - StitchBuffer[iStart].x;
		delta.y = StitchBuffer[finish].y - StitchBuffer[iStart].y;
		length  = hypot(delta.x, delta.y);
		iStart--;
	} while (!length);
	if (StateMap.test(StateFlag::FILDIR))
		knots = KnotAtLastOrder;
	else
		knots = KnotAtEndOrder;
	if (knots) {
		if (!(iStart & 0x8000000)) {
			KnotStep.x = 2 / length * delta.x;
			KnotStep.y = 2 / length * delta.y;
			for (iKnot = 0; iKnot < 5; iKnot++)
				ofstch(finish, knots[iKnot]);
			if (StateMap.test(StateFlag::FILDIR))
				ofstch(finish, 0);
		}
	}
}

void thred::internal::strtknt(unsigned start) noexcept {
	double   length = 0.0;
	dPOINT   delta  = {};
	unsigned finish = start + 1;
	unsigned iKnot  = 0;

	do {
		delta.x = StitchBuffer[finish].x - StitchBuffer[start].x;
		delta.y = StitchBuffer[finish].y - StitchBuffer[start].y;
		length  = hypot(delta.x, delta.y);
		finish++;
	} while (length < 2 && finish < PCSHeader.stitchCount);
	if (finish < PCSHeader.stitchCount) {
		KnotAttribute = StitchBuffer[start].attribute | KNOTMSK;
		KnotStep.x    = 2 / length * delta.x;
		KnotStep.y    = 2 / length * delta.y;
		for (iKnot = 0; iKnot < 5; iKnot++)
			ofstch(start, KnotAtStartOrder[iKnot]);
	}
}

void thred::internal::delknt() noexcept {
	unsigned iStitch = 0, newStitchCount = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (!(StitchBuffer[iStitch].attribute & KNOTMSK))
			thred::mvstch(newStitchCount++, iStitch);
	}
	PCSHeader.stitchCount = newStitchCount;
}

bool thred::internal::isknots() noexcept {
	unsigned iStitch;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (StitchBuffer[iStitch].attribute & KNOTMSK)
			return 1;
	}
	return 0;
}

void thred::internal::delknot() {
	if (isknots()) {
		thred::savdo();
		delknt();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

unsigned thred::internal::kjmp(unsigned start) {
	while (start < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1 && stlen(start) > KNOTLEN)
		thred::mvstch(OutputIndex++, start++);
	strtknt(start);
	return start;
}

void thred::mvstchs(unsigned destination, unsigned source, unsigned count) {
	auto       sourceStart     = StitchBuffer + source;
	auto       sourceEnd       = sourceStart + count;
	const auto destinationSpan = stdext::make_checked_array_iterator(StitchBuffer + destination, (MAXITEMS * 2) - destination);
	std::copy(sourceStart, sourceEnd, destinationSpan);
}

void thred::internal::setknt() {
	unsigned iStitch = 0;

	// ToDo - Use a temp buffer rather than the high buffer
	OutputIndex = MAXITEMS;
	thred::mvstch(OutputIndex++, 0);
	strtknt(0);
	if (stlen(0) > KNOTLEN)
		iStitch = kjmp(1);
	else
		iStitch = 1;
	StateMap.reset(StateFlag::FILDIR);
	while (iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
		thred::mvstch(OutputIndex++, iStitch);
		if (stlen(iStitch) > KNOTLEN) {
			endknt(iStitch);
			iStitch = kjmp(iStitch + 1);
			thred::mvstch(OutputIndex++, iStitch);
		}
		iStitch++;
	}
	StateMap.set(StateFlag::FILDIR);
	endknt(iStitch);
	StitchBuffer[OutputIndex - 1].attribute &= (~KNOTMSK);
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(OutputIndex - MAXITEMS);
	thred::mvstchs(0, MAXITEMS, PCSHeader.stitchCount);
}

unsigned thred::internal::srchknot(unsigned source) noexcept {
	while (Knots[ActivePointIndex] < source && ActivePointIndex < KnotCount)
		ActivePointIndex++;
	ActivePointIndex--;
	if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5) {
		ActivePointIndex++;
		if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5)
			return 0;
		else
			return 2;
	}
	else {
		ActivePointIndex++;
		if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5)
			return 1;
		else
			return 3;
	}
}

void thred::internal::chkncol() {
	unsigned iStitch = 0, color = 0, code = 0;
	unsigned initialColor = StitchBuffer[0].attribute & COLMSK;

	// ToDo - Use a temp buffer rather than the high buffer
	OutputIndex = MAXITEMS;
	StateMap.reset(StateFlag::FILDIR);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		color = StitchBuffer[iStitch].attribute & COLMSK;
		if (color == initialColor)
			thred::mvstch(OutputIndex++, iStitch);
		else {
			initialColor = StitchBuffer[iStitch].attribute & COLMSK;
			code         = srchknot(iStitch);
			if (code & 1) {
				endknt(iStitch - 1);
			}
			thred::mvstch(OutputIndex++, iStitch);
			if (code & 2)
				strtknt(iStitch);
		}
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(OutputIndex - MAXITEMS);
	thred::mvstchs(0, MAXITEMS, PCSHeader.stitchCount);
}

void thred::internal::setknots() {
	if (PCSHeader.stitchCount) {
		thred::savdo();
		delknt();
		setknt();
		fndknt();
		chkncol();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
		thred::ritot(PCSHeader.stitchCount);
	}
}

void thred::internal::lodbmp() {
	if (PCSBMPFileName[0]) {
		DeleteObject(BitmapFileHandle);
		ReleaseDC(ThrEdWindow, BitmapDC);
	}
	wchar_t szFileName[_MAX_PATH] = { 0 };
	wchar_t lpszBuffer[_MAX_PATH] = { 0 };
	auto    workingFileStr        = UserBMPFileName->wstring();
	auto    dirStr                = DefaultBMPDirectory->wstring();
	std::copy(workingFileStr.begin(), workingFileStr.end(), szFileName);
	std::copy(dirStr.begin(), dirStr.end(), lpszBuffer);
	OpenBitmapName.lpstrFile       = szFileName;
	OpenBitmapName.lpstrInitialDir = lpszBuffer;
	if (GetOpenFileName(&OpenBitmapName)) {
		UserBMPFileName->assign(szFileName);
		trace::untrace();
		auto filename = utf::Utf16ToUtf8(UserBMPFileName->filename().wstring());
		// PCS file can only store a 16 character filename?
		// ToDo - give the user a little more info that the bitmap has not been loaded
		if (filename.size() && filename.size() < 16) {
			std::copy(filename.begin(), filename.end(), PCSBMPFileName);
			defbNam();
			bfil();
		}
		else {
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void thred::internal::duhbit(unsigned cod) noexcept {
	CheckMenuItem(MainMenu, ID_HIDBIT, MF_BYCOMMAND | cod);
	CheckMenuItem(MainMenu, ID_HIDBITF, MF_BYCOMMAND | cod);
}

void thred::hidbit() {
	if (StateMap.testAndFlip(StateFlag::HIDMAP))
		thi::duhbit(MF_UNCHECKED);
	else
		thi::duhbit(MF_CHECKED);
	StateMap.set(StateFlag::DUMEN);
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::drwlstch(unsigned finish) {
	unsigned color = 0, iMovieFrame = 0;
	unsigned origin = RunPoint;
	unsigned flag   = 1;

	if (StateMap.test(StateFlag::HID)) {
		while ((StitchBuffer[RunPoint].attribute & COLMSK) != ActiveColor && RunPoint < finish - 1)
			RunPoint++;
	}
	if (StateMap.test(StateFlag::ZUMED)) {
		iMovieFrame = 1;
		while (RunPoint < StitchesPerFrame + 1 && RunPoint < finish - 2 && !stch2px(RunPoint))
			RunPoint++;
		origin = RunPoint - 1;
		color  = StitchBuffer[RunPoint].attribute & COLMSK;
		flag   = 1;
		while (iMovieFrame < StitchesPerFrame + 1 && RunPoint < finish - 2
		       && (StitchBuffer[RunPoint].attribute & COLMSK) == color) {
			if (stch2px(RunPoint)) {
				MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
				if (flag) {
					flag = 0;
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
		if (RunPoint == origin)
			RunPoint++;
		if (!stch2px(RunPoint)) {
			if ((StitchBuffer[RunPoint].attribute & COLMSK) == color) {
				MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
				RunPoint++;
			}
		}
		SelectObject(StitchWindowDC, UserPen[color]);
		PolylineInt(StitchWindowDC, MovieLine, iMovieFrame);
		if (!flag)
			RunPoint--;
	}
	else {
		iMovieFrame = 0;
		color       = StitchBuffer[RunPoint].attribute & COLMSK;
		SelectObject(StitchWindowDC, UserPen[color]);
		while (iMovieFrame < StitchesPerFrame && (RunPoint + 1 < finish - 1)
		       && ((StitchBuffer[RunPoint].attribute & COLMSK) == color)) {
			stch2px1(RunPoint++);
			MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
		}
		RunPoint--;
		PolylineInt(StitchWindowDC, MovieLine, iMovieFrame);
	}
	if ((StitchBuffer[RunPoint + 1].attribute & 0xf) != color)
		RunPoint++;
	displayText::ritnum(STR_NUMSEL, RunPoint);
	if (RunPoint + 3 > finish - 1)
		patdun();
}

void thred::internal::stchout() {
	if (StateMap.test(StateFlag::GRPSEL))
		drwlstch(GroupEndStitch);
	else
		drwlstch(PCSHeader.stitchCount - 1);
}

void thred::internal::setsped() {
	unsigned elapsedTimePerFrame = 0;
	double   userTimePerFrame    = 0.0;

	if (!MovieTimeStep)
		MovieTimeStep = 1;
	userTimePerFrame = static_cast<double>(MovieTimeStep) / 10;
	if (userTimePerFrame < 10) {
		elapsedTimePerFrame = 100; // units are millseconds
		StitchesPerFrame    = gsl::narrow<unsigned>(round(elapsedTimePerFrame / userTimePerFrame));
		if (StitchesPerFrame > 99)
			StitchesPerFrame = 99;
	}
	else {
		elapsedTimePerFrame = gsl::narrow<unsigned>(round(userTimePerFrame));
		StitchesPerFrame    = 2;
	}
	if (StitchesPerFrame < 2)
		StitchesPerFrame = 2;
	SetTimer(ThrEdWindow, 0, elapsedTimePerFrame, 0);
}

void thred::internal::deltot() {
	DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
	TextureIndex = FormIndex = FormVertexIndex = ClipPointIndex = PCSHeader.stitchCount = 0;
	satin::clearGuideSize();
	StateMap.reset(StateFlag::GMRK);
	rstAll();
	thred::coltab();
	thred::zumhom();
	// wcscpy_s(DesignerName, IniFile.designerName);
	SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRDBY], ThrName->wstring(), *DesignerName).c_str());
}

bool thred::internal::wastch() noexcept {
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor)
			return 1;
	}
	return 0;
}

bool thred::internal::frmstch() {
	unsigned                iStitch = 0, formCode = 0;
	boost::dynamic_bitset<> formMap(FormIndex);

	for (auto form : (*SelectedFormList)) {
		formMap.set(form);
	}
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		formCode = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
		if (formMap.test(formCode))
			return true;
	}
	return false;
}

void thred::internal::delet() {
	unsigned iVertex = 0, iGuide = 0;
	size_t   currentFormVertex = 0;
	// ToDo - check satinFlag
	bool satinFlag = false;

	thred::undat();
	if (StateMap.testAndReset(StateFlag::FPSEL)) {
		thred::savdo();
		form::fvars(ClosestFormToCursor);
		boost::dynamic_bitset<> vertexMap(VertexCount);
		currentFormVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertexMap.set(currentFormVertex);
			currentFormVertex = form::pdir(currentFormVertex);
		}
		currentFormVertex = 0;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			if (!vertexMap.test(iVertex)) {
				CurrentFormVertices[currentFormVertex++] = CurrentFormVertices[iVertex];
			}
		}
		currentFormVertex = form::fltind(&CurrentFormVertices[currentFormVertex]);
		iVertex           = form::fltind(&CurrentFormVertices[iVertex]);
		while (iVertex < FormVertexIndex) {
			FormVertices[currentFormVertex++] = FormVertices[iVertex++];
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
			FormList[iForm].vertices -= (SelectedFormVertices.vertexCount + 1);
		FormVertexIndex -= (SelectedFormVertices.vertexCount + 1);
		SelectedForm->vertexCount -= (SelectedFormVertices.vertexCount + 1);
		form::frmout(ClosestFormToCursor);
		if (SelectedForm->type == SAT)
			satin::satadj();
		form::refil();
		fndknt();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (GetKeyState(VK_CONTROL) & GetKeyState(VK_SHIFT) & 0X8000) {
		deltot();
	}
	else {
		thred::savdo();
		if (SelectedFormList->size()) {
			if (frmstch()) {
				StateMap.set(StateFlag::DELSFRMS);
				displayText::tabmsg(IDS_DELFRM);
				displayText::okcan();
				displayText::tomsg();
			}
			else
				delsfrms(1);
			return;
		}
		if (StateMap.test(StateFlag::FORMSEL) && FormIndex) {
			if (wastch()) {
				StateMap.set(StateFlag::DELFRM);
				displayText::tabmsg(IDS_FDEL);
				displayText::okcan();
				displayText::tomsg();
			}
			else {
				if (FormIndex) {
					StateMap.reset(StateFlag::DELTO);
					thred::frmdel();
					StateMap.set(StateFlag::RESTCH);
				}
			}
			fndknt();
			return;
		}
		if (StateMap.test(StateFlag::SELBOX)) {
			if (PCSHeader.stitchCount > 2) {
				delstch1(ClosestPointIndex);
				if (!stch2px(ClosestPointIndex))
					movbox();
			}
			else {
				PCSHeader.stitchCount = 0;
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
			SelectedForm = &FormList[ClosestFormToCursor];
			switch (SelectedForm->type) {
			case FRMLINE:
				if (SelectedForm->fillType == CONTF) {
					if (ClosestVertexToCursor == SelectedForm->angleOrClipData.guide.start
					    || ClosestVertexToCursor == SelectedForm->angleOrClipData.guide.finish) {
						form::delmfil();
						SelectedForm->fillType = 0;
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return;
					}
					if (SelectedForm->angleOrClipData.guide.start > ClosestVertexToCursor)
						SelectedForm->angleOrClipData.guide.start--;
					if (SelectedForm->angleOrClipData.guide.finish > ClosestVertexToCursor)
						SelectedForm->angleOrClipData.guide.finish--;
				}
				break;
			case SAT:
				do {
					if (ClosestVertexToCursor <= 1) {
						if (SelectedForm->attribute & FRMEND) {
							if (SelectedForm->wordParam)
								SelectedForm->wordParam = 0;
							else
								SelectedForm->attribute &= 0xfe;
							satinFlag = true;
							break;
						}
					}
					if (SatinEndGuide) {
						if (ClosestVertexToCursor == gsl::narrow<unsigned>(SatinEndGuide)
						    || ClosestVertexToCursor == gsl::narrow<unsigned>(SatinEndGuide) + 1) {
							SelectedForm->wordParam = 0;
							satinFlag               = true;
							break;
						}
					}
					for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
						if (SelectedForm->satinOrAngle.guide[iGuide].start == ClosestVertexToCursor
						    || SelectedForm->satinOrAngle.guide[iGuide].finish == ClosestVertexToCursor) {
							satin::delcon(iGuide);
							satinFlag = true;
							break;
						}
					}
				} while (false);
			}
			if (!satinFlag) {
				satin::delspnt();
			}
			if (ClosestFormToCursor > FormIndex - 1)
				ClosestFormToCursor = FormIndex - 1;
			if (FormIndex) {
				form::frmout(ClosestFormToCursor);
				form::fvars(ClosestFormToCursor);
				form::refil();
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		if (!satinFlag && closPnt1(&ClosestPointIndex)) {
			if (PCSHeader.stitchCount > 2) {
				delstch1(ClosestPointIndex);
				if (!stch2px(ClosestPointIndex))
					movbox();
			}
			else {
				PCSHeader.stitchCount = 0;
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
	double stepCount = 0.0;

	rstAll();
	if (PCSHeader.stitchCount) {
		if (MsgWindow) {
			DestroyWindow(MsgWindow);
			MsgWindow = 0;
		}
		StateMap.set(StateFlag::RUNPAT);
		if (StateMap.test(StateFlag::GRPSEL)) {
			thred::rngadj();
			RunPoint = GroupStartStitch;
		}
		else
			RunPoint = 0;
		thred::movStch();
		if (!StateMap.test(StateFlag::WASPAT)) {
			SpeedScrollBar = CreateWindow(L"SCROLLBAR",
			                              0,
			                              SBS_HORZ | WS_CHILD | WS_VISIBLE,
			                              ButtonWidthX3,
			                              0,
			                              StitchWindowSize.x,
			                              SCROLSIZ,
			                              ThrEdWindow,
			                              NULL,
			                              ThrEdInstance,
			                              NULL);
		}
		if (StateMap.test(StateFlag::ZUMED))
			stepCount = PCSHeader.stitchCount * ZoomFactor * ZoomFactor;
		else
			stepCount = PCSHeader.stitchCount;
		if (!StateMap.test(StateFlag::WASPAT))
			MovieTimeStep = MOVITIM * 10000 / stepCount;
		if (MovieTimeStep < MINDELAY)
			MovieTimeStep = MINDELAY;
		if (MovieTimeStep > MAXDELAY)
			MovieTimeStep = MAXDELAY;
		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask  = SIF_ALL;
		ScrollInfo.nMax   = MAXDELAY;
		ScrollInfo.nMin   = MINDELAY;
		ScrollInfo.nPage  = 1;
		ScrollInfo.nPos   = MAXDELAY - MovieTimeStep;
		SetScrollInfo(SpeedScrollBar, SB_CTL, &ScrollInfo, TRUE);
		FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
		setsped();
	}
}

#define CLPBUG 0

void thred::redclp() {
	int       iStitch    = 0;
	const int codedLayer = ActiveLayer << LAYSHFT;

	ClipPointer = GlobalLock(ClipMemory);
	if (ClipPointer) {
		ClipStitchData          = static_cast<CLPSTCH*>(ClipPointer);
		ClipStitchCount         = ClipStitchData[0].led;
		ClipBuffer[0].x         = ClipStitchData[0].x + static_cast<float>(ClipStitchData[0].fx) / 256;
		ClipBuffer[0].y         = ClipStitchData[0].y + static_cast<float>(ClipStitchData[0].fy) / 256;
		ClipBuffer[0].attribute = 0;

#if CLPBUG
		OutputDebugString(fmt::format("interator: 0 x: {:6.2f},y: {:6.2f}\n", ClipBuffer[0].x, ClipBuffer[0].y).c_str());
#endif
		ClipRect.left = ClipRect.right = ClipBuffer[0].x;
		ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
		for (iStitch = 1; iStitch < gsl::narrow<long>(ClipStitchCount); iStitch++) {
			ClipBuffer[iStitch].x         = ClipStitchData[iStitch].x + static_cast<float>(ClipStitchData[iStitch].fx) / 256;
			ClipBuffer[iStitch].y         = ClipStitchData[iStitch].y + static_cast<float>(ClipStitchData[iStitch].fy) / 256;
			ClipBuffer[iStitch].attribute = (ClipStitchData[iStitch].led & 0xf) | codedLayer;

#if CLPBUG
			OutputDebugString(
			    fmt::format("interator: {} x: {:6.2f},y: {:6.2f}\n", iStitch, ClipBuffer[iStitch].x, ClipBuffer[iStitch].y)
			        .c_str());
#endif
			if (ClipBuffer[iStitch].x < ClipRect.left)
				ClipRect.left = ClipBuffer[iStitch].x;
			if (ClipBuffer[iStitch].x > ClipRect.right)
				ClipRect.right = ClipBuffer[iStitch].x;
			if (ClipBuffer[iStitch].y < ClipRect.bottom)
				ClipRect.bottom = ClipBuffer[iStitch].y;
			if (ClipBuffer[iStitch].y > ClipRect.top)
				ClipRect.top = ClipBuffer[iStitch].y;
		}
		ClipBuffer[0].attribute = ActiveColor | codedLayer;
		ClipRectSize.cx         = ClipRect.right - ClipRect.left;
		ClipRectSize.cy         = ClipRect.top - ClipRect.bottom;
		GlobalUnlock(ClipMemory);
		if (ClipRect.left || ClipRect.bottom) {
			for (iStitch = 0; iStitch < gsl::narrow<int>(ClipStitchCount); iStitch++) {
				ClipBuffer[iStitch].x -= ClipRect.left;
				ClipBuffer[iStitch].y -= ClipRect.bottom;
			}
			ClipRect.top -= ClipRect.bottom;
			ClipRect.right -= ClipRect.left;
			ClipRect.bottom = ClipRect.left = 0;
		}
	}
}

constexpr unsigned thred::internal::nxtcrnr(unsigned corner) {
	return (corner + 1) & 3;
}

void thred::internal::drwmrk(HDC dc) noexcept {
	POINT      markCoordinates = {};
	POINT      markLine[2]     = {};
	const long markOffset      = 6;

	thred::sCor2px(ZoomMarkPoint, markCoordinates);
	SelectObject(dc, ZoomMarkPen);
	SetROP2(dc, R2_XORPEN);
	markLine[0].x = markCoordinates.x - markOffset;
	markLine[0].y = markCoordinates.y - markOffset;
	markLine[1].x = markCoordinates.x + markOffset;
	markLine[1].y = markCoordinates.y + markOffset;
	Polyline(dc, markLine, 2);
	markLine[0].x = markCoordinates.x - markOffset;
	markLine[0].y = markCoordinates.y + markOffset;
	markLine[1].x = markCoordinates.x + markOffset;
	markLine[1].y = markCoordinates.y - markOffset;
	Polyline(dc, markLine, 2);
	SetROP2(dc, R2_COPYPEN);
}

void thred::internal::vubak() {
	unsigned iVersion = 0;
	long     dx = 0, dy = 0, verticalLocation = 0;

	if (FileHandle || StateMap.test(StateFlag::THUMSHO)) {
		StateMap.set(StateFlag::ZUMED);
		thred::movStch();
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		dx = (StitchWindowClientRect.right >> 1);
		dy = (StitchWindowClientRect.bottom >> 1);
		for (iVersion = 0; iVersion < 4; iVersion++) { // there are 4 quadrants
			if (iVersion & 2)
				verticalLocation = dy;
			else
				verticalLocation = 0;
			BackupViewer[iVersion] = CreateWindow(L"STATIC",
			                                      L"",
			                                      SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			                                      dx * (iVersion & 1) + ButtonWidthX3,
			                                      verticalLocation,
			                                      dx,
			                                      dy,
			                                      ThrEdWindow,
			                                      NULL,
			                                      ThrEdInstance,
			                                      NULL);
		}
		StateMap.set(StateFlag::BAKSHO);
	}
}

void thred::internal::insflin(POINT insertPoint) noexcept {
	POINT offset;

	offset.x = InsertSize.x >> 1;
	offset.y = InsertSize.y >> 1;

	FormLines[0].x = FormLines[3].x = FormLines[4].x = insertPoint.x - offset.x;
	FormLines[1].x = FormLines[2].x = insertPoint.x + offset.x;
	FormLines[0].y = FormLines[1].y = FormLines[4].y = insertPoint.y - offset.y;
	FormLines[2].y = FormLines[3].y = insertPoint.y + offset.y;
}

bool thred::internal::isthr(const wchar_t* const filename) {
	fs::path thredPath(filename);

	auto extention = thredPath.extension().wstring();

	if ((extention.compare(0, 3, L".th")) == 0) {
		return true;
	}
	else {
		return false;
	}
}

unsigned thred::internal::gethand(const fPOINTATTR* stitch, unsigned stitchCount) noexcept {
	unsigned userStitchCount = 0;
	if (stitch) {
		for (unsigned iStitch = 0; iStitch < stitchCount; iStitch++) {
			if (stitch[iStitch].attribute & USMSK)
				userStitchCount++;
		}
	}
	return userStitchCount;
}

void thred::internal::insfil() {
	OPENFILENAME file = {
		sizeof(file),                        // lStructsize
		ThrEdWindow,                         // hwndOwner
		ThrEdInstance,                       // hInstance
		L"THR files\0*.thr\0\0",             // lpstrFilter
		CustomFilter,                        // lpstrCustomFilter
		_MAX_PATH,                           // nMaxCustFilter
		0,                                   // nFilterIndex
		InsertedFileName,                    // lpstrFile
		_MAX_PATH,                           // nMaxFile
		0,                                   // lpstrFileTitle
		0,                                   // nMaxFileTitle
		DefaultDirectory->wstring().c_str(), // lpstr	ialDir
		0,                                   // lpstrTitle
		OFN_EXPLORER | OFN_OVERWRITEPROMPT,  // Flags
		0,                                   // nFileOffset
		0,                                   // nFileExtension
		L"thr",                              // lpstrDefExt
		0,                                   // lCustData
		0,                                   // lpfnHook
		0,                                   // lpTemplateName
	};
	STRHED     fileHeader    = {};
	STREX      thredHeader   = {};
	PCSHEADER  pcsFileHeader = {};
	unsigned   iStitch = 0, iName = 0, iPCSStitch = 0;
	unsigned   newStitchCount = 0, newAttribute = 0;
	fRECTANGLE insertedRectangle  = {};
	fPOINT     insertedSize       = {};
	POINT      initialInsertPoint = {};
	double     homscor            = 0.0;
	double     filscor            = 0.0;
	unsigned   version            = 0;
	size_t     newFormVertexIndex = FormVertexIndex;
	size_t     newSatinGuideIndex = satin::getGuideSize();
	size_t     newClipPointIndex  = ClipPointIndex;
	size_t     newTextureIndex    = TextureIndex;

	if (StateMap.test(StateFlag::IGNORINS) || GetOpenFileName(&file)) {
		InsertedFileHandle = CreateFile(InsertedFileName, (GENERIC_READ), 0, NULL, OPEN_EXISTING, 0, NULL);
		if (InsertedFileHandle == INVALID_HANDLE_VALUE) {
			displayText::filnopn(IDS_FNOPN, InsertedFileName);
			FileHandle = 0;
			CloseHandle(InsertedFileHandle);
		}
		else {
			InsertedStitchCount = PCSHeader.stitchCount;
			if (isthr(InsertedFileName)) {
				ReadFile(InsertedFileHandle, &fileHeader, sizeof(fileHeader), &BytesRead, NULL);
				if ((fileHeader.headerType & 0xffffff) != 0x746872)
					displayText::tabmsg(IDS_NOTHR);
				else {
					version = (fileHeader.headerType & 0xff000000) >> 24;
					if (version) {
#define FRMW 5
#define HANDW 4
#define FRMPW 2
#define STCHW 1
						gethand(StitchBuffer, PCSHeader.stitchCount);
						// ToDo - replace constants with sizes of data structures?
						homscor = static_cast<double>(FormIndex) * FRMW + gethand(StitchBuffer, PCSHeader.stitchCount) * HANDW
						          + FormVertexIndex * FRMPW + PCSHeader.stitchCount * STCHW;
						ReadFile(InsertedFileHandle, &thredHeader, sizeof(thredHeader), &BytesRead, 0);
					}
					thred::savdo();
					if (fileHeader.stitchCount) {
						ReadFile(InsertedFileHandle,
						         &StitchBuffer[PCSHeader.stitchCount],
						         fileHeader.stitchCount * sizeof(StitchBuffer[0]),
						         &BytesRead,
						         NULL);
					}
					const auto threadLength = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0]))
					                          / 2; // ThreadSize is defined as a 16 entry array of 2 bytes
					const auto formDataOffset = sizeof(PCSBMPFileName) + sizeof(BackgroundColor) + sizeof(UserColor)
					                            + sizeof(CustomColor) + threadLength;
					SetFilePointer(InsertedFileHandle, formDataOffset, 0, FILE_CURRENT);
					insertedRectangle.left   = 1e9f;
					insertedRectangle.bottom = 1e9f;
					insertedRectangle.top    = 1e-9f;
					insertedRectangle.right  = 1e-9f;
					InsertedVertexIndex      = FormVertexIndex;
					InsertedFormIndex        = FormIndex;
					if (fileHeader.formCount) {
						if (version < 2) {
							std::vector<FRMHEDO> formHeader(fileHeader.formCount);
							ReadFile(InsertedFileHandle,
							         formHeader.data(),
							         fileHeader.formCount * sizeof(formHeader[0]),
							         &BytesRead,
							         0);
							if (BytesRead != fileHeader.formCount * sizeof(formHeader[0])) {
								formHeader.resize(BytesRead / sizeof(formHeader[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							if (FormIndex + fileHeader.formCount < MAXFORMS) {
								FRMHED fillval = {};
								std::fill_n(FormList + FormIndex, fileHeader.formCount, fillval);
								std::copy(formHeader.cbegin(),
								          formHeader.cend(),
								          stdext::make_checked_array_iterator(FormList + FormIndex, MAXITEMS - FormIndex));
							}
							FormIndex += formHeader.size();
						}
						else {
							// ToDo - Use FRMHEDOUT here
							// ReadFile(
							//    InsertedFileHandle, &FormList[FormIndex], fileHeader.formCount * sizeof(FRMHED), &BytesRead, 0);
							std::vector<FRMHEDOUT> inFormList(fileHeader.formCount);
							auto                   bytesToRead = gsl::narrow<DWORD>(fileHeader.formCount * sizeof(inFormList[0]));
							ReadFileInt(InsertedFileHandle, inFormList.data(), bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								inFormList.resize(BytesRead / sizeof(inFormList[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							const auto destination = stdext::make_checked_array_iterator(
							    FormList + FormIndex, (sizeof(FormList) / sizeof(FormList[0])) - FormIndex);
							std::copy(inFormList.cbegin(), inFormList.cend(), destination);
							FormIndex += inFormList.size();
						}
						if (fileHeader.vertexCount) {
							auto bytesToRead = gsl::narrow<DWORD>(fileHeader.vertexCount * sizeof(FormVertices[0]));
							ReadFile(InsertedFileHandle, &FormVertices[FormVertexIndex], bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								newFormVertexIndex += BytesRead / sizeof(FormVertices[0]);
								StateMap.set(StateFlag::BADFIL);
							}
							else {
								newFormVertexIndex += fileHeader.vertexCount;
							}
						}
						else {
							StateMap.set(StateFlag::BADFIL);
						}
						if (fileHeader.dlineCount) {
							// ToDo - Use SATCONOUT here
							// ReadFile(InsertedFileHandle,
							//         &SatinGuides[SatinGuideIndex],
							//         fileHeader.dlineCount * sizeof(SatinGuides[0]),
							//         &BytesRead,
							//         0);
							std::vector<SATCONOUT> inSatinGuides(fileHeader.dlineCount);
							auto bytesToRead = gsl::narrow<DWORD>(fileHeader.dlineCount * sizeof(inSatinGuides[0]));
							ReadFile(FileHandle, inSatinGuides.data(), bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								inSatinGuides.resize(BytesRead / sizeof(inSatinGuides[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							satin::cpyTmpGuides(inSatinGuides);
							newSatinGuideIndex += inSatinGuides.size();
						}
						if (fileHeader.clipDataCount) {
							auto bytesToRead = gsl::narrow<DWORD>(fileHeader.clipDataCount * sizeof(ClipPoints[0]));
							ReadFile(InsertedFileHandle, &ClipPoints[ClipPointIndex], bytesToRead, &BytesRead, 0);
							if (BytesRead != bytesToRead) {
								StateMap.set(StateFlag::BADFIL);
							}
							newClipPointIndex += BytesRead / sizeof(ClipPoints[0]);
						}
						if (ExtendedHeader.texturePointCount) {
							TexturePointsBuffer->resize(TexturePointsBuffer->size() + ExtendedHeader.texturePointCount);
							ReadFile(InsertedFileHandle,
							         &TexturePointsBuffer[TextureIndex],
							         ExtendedHeader.texturePointCount * sizeof(TexturePointsBuffer[0]),
							         &BytesRead,
							         0);
						}
						CloseHandle(InsertedFileHandle);
						InsertedFileHandle = 0;
						// update the form pointer variables
						for (auto iFormList = InsertedFormIndex; iFormList < FormIndex; iFormList++) {
							FormList[iFormList].vertices = thred::adflt(FormList[iFormList].vertexCount);
							if (FormList[iFormList].type == SAT) {
								if (FormList[iFormList].satinGuideCount)
									FormList[iFormList].satinOrAngle.guide = satin::adsatk(FormList[iFormList].satinGuideCount);
								if (clip::isclpx(iFormList))
									FormList[iFormList].angleOrClipData.clip = thred::adclp(FormList[iFormList].lengthOrCount.clipCount);
							}
							if (clip::isclp(iFormList))
								FormList[iFormList].angleOrClipData.clip = thred::adclp(FormList[iFormList].lengthOrCount.clipCount);
							if (clip::iseclpx(iFormList))
								FormList[iFormList].borderClipData = thred::adclp(FormList[iFormList].clipEntries);
							if (texture::istx(iFormList)) {
								FormList[iFormList].fillInfo.texture.index += gsl::narrow<unsigned short>(TextureIndex);
								newTextureIndex += FormList[iFormList].fillInfo.texture.count;
							}
						}
						if (newFormVertexIndex != FormVertexIndex) {
							StateMap.set(StateFlag::BADFIL);
						}
						if (newSatinGuideIndex != satin::getGuideSize()) {
							StateMap.set(StateFlag::BADFIL);
						}
						if (newClipPointIndex != ClipPointIndex) {
							StateMap.set(StateFlag::BADFIL);
						}

						TextureIndex = newTextureIndex;
						if (fileHeader.formCount) {
							insertedRectangle.left = insertedRectangle.right = FormVertices[InsertedVertexIndex].x;
							insertedRectangle.bottom = insertedRectangle.top = FormVertices[InsertedVertexIndex].y;
							for (auto iVertex = InsertedVertexIndex + 1; iVertex < FormVertexIndex; iVertex++) {
								if (FormVertices[iVertex].x < insertedRectangle.left)
									insertedRectangle.left = FormVertices[iVertex].x;
								if (FormVertices[iVertex].x > insertedRectangle.right)
									insertedRectangle.right = FormVertices[iVertex].x;
								if (FormVertices[iVertex].y < insertedRectangle.bottom)
									insertedRectangle.bottom = FormVertices[iVertex].y;
								if (FormVertices[iVertex].y > insertedRectangle.top)
									insertedRectangle.top = FormVertices[iVertex].y;
							}
						}
					}
					if (fileHeader.stitchCount) {
						auto encodedFormIndex = gsl::narrow<unsigned int>(InsertedFormIndex << FRMSHFT);
						for (iStitch = PCSHeader.stitchCount;
						     iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) + fileHeader.stitchCount;
						     iStitch++) {
							if (StitchBuffer[iStitch].attribute & ALTYPMSK) {
								newAttribute = (StitchBuffer[iStitch].attribute & FRMSK) + encodedFormIndex;
								StitchBuffer[iStitch].attribute &= NFRMSK;
								StitchBuffer[iStitch].attribute |= newAttribute;
							}
							if (StitchBuffer[iStitch].x < insertedRectangle.left)
								insertedRectangle.left = StitchBuffer[iStitch].x;
							if (StitchBuffer[iStitch].x > insertedRectangle.right)
								insertedRectangle.right = StitchBuffer[iStitch].x;
							if (StitchBuffer[iStitch].y < insertedRectangle.bottom)
								insertedRectangle.bottom = StitchBuffer[iStitch].y;
							if (StitchBuffer[iStitch].y > insertedRectangle.top)
								insertedRectangle.top = StitchBuffer[iStitch].y;
						}
					}
					if (fileHeader.headerType & 0x1000000) {
						// ToDo - Replace constants with sizes of data structures
						filscor = static_cast<double>(fileHeader.formCount) * FRMW
						          + gethand(&StitchBuffer[PCSHeader.stitchCount], fileHeader.stitchCount) * HANDW
						          + fileHeader.vertexLen * FRMPW + fileHeader.stitchCount * STCHW;
						if (filscor > homscor) {
							for (iName = 0; iName < 50; iName++) {
								ExtendedHeader.creatorName[iName] = thredHeader.creatorName[iName];
							}
							redfnam(*DesignerName);
							SetWindowText(ThrEdWindow,
							              fmt::format((*StringTable)[STR_THRDBY], ThrName->wstring(), *DesignerName).c_str());
						}
					}
					InsertCenter.x = (insertedRectangle.right - insertedRectangle.left) / 2 + insertedRectangle.left;
					InsertCenter.y = (insertedRectangle.top - insertedRectangle.bottom) / 2 + insertedRectangle.bottom;
					PCSHeader.stitchCount += fileHeader.stitchCount;
					insertedSize.x = insertedRectangle.right - insertedRectangle.left;
					insertedSize.y = insertedRectangle.top - insertedRectangle.bottom;
					form::ratsr();
					InsertSize.x = insertedSize.x * HorizontalRatio;
					// ToDo - Should this be vertical ratio?
					InsertSize.y         = insertedSize.y * HorizontalRatio;
					initialInsertPoint.x = StitchWindowClientRect.right >> 1;
					initialInsertPoint.y = StitchWindowClientRect.bottom >> 1;
					insflin(initialInsertPoint);
					NewFormVertexCount = 5;
					StateMap.set(StateFlag::SHOFRM);
					StateMap.set(StateFlag::INSFIL);
					form::dufrm();
				}
			}
			else {
				// ToDo - inserting PCS files is broken and needs to be fixed
				ReadFile(InsertedFileHandle, &pcsFileHeader, 0x46, &BytesRead, NULL);
				if (PCSHeader.leadIn == 0x32 && PCSHeader.colorCount == 16) {
					thred::savdo();
					std::vector<PCSTCH> pcsStitchBuffer(pcsFileHeader.stitchCount);
					ReadFile(InsertedFileHandle,
					         pcsStitchBuffer.data(),
					         pcsFileHeader.stitchCount * sizeof(pcsStitchBuffer[0]),
					         &BytesRead,
					         NULL);
					iStitch      = PCSHeader.stitchCount;
					newAttribute = 0;
					for (iPCSStitch = 0; iPCSStitch < pcsFileHeader.stitchCount; iPCSStitch++) {
						if (pcsStitchBuffer[iPCSStitch].tag == 3)
							newAttribute = pcsStitchBuffer[iPCSStitch++].fx;
						else {
							StitchBuffer[iStitch].x
							    = pcsStitchBuffer[iPCSStitch].x + static_cast<float>(pcsStitchBuffer[iPCSStitch].fx) / 256;
							StitchBuffer[iStitch].y
							    = pcsStitchBuffer[iPCSStitch].y + static_cast<float>(pcsStitchBuffer[iPCSStitch].fy) / 256;
							StitchBuffer[iStitch++].attribute = newAttribute;
						}
					}
					newStitchCount         = iStitch;
					iStitch                = PCSHeader.stitchCount;
					insertedRectangle.left = insertedRectangle.right = StitchBuffer[iPCSStitch].x;
					insertedRectangle.top = insertedRectangle.bottom = StitchBuffer[iPCSStitch++].y;
					while (iStitch < gsl::narrow<unsigned>(newStitchCount)) {
						if (StitchBuffer[iStitch].x < insertedRectangle.left)
							insertedRectangle.left = StitchBuffer[iStitch].x;
						if (StitchBuffer[iStitch].x > insertedRectangle.right)
							insertedRectangle.right = StitchBuffer[iStitch].x;
						if (StitchBuffer[iStitch].y < insertedRectangle.bottom)
							insertedRectangle.bottom = StitchBuffer[iStitch].y;
						if (StitchBuffer[iStitch].y > insertedRectangle.top)
							insertedRectangle.top = StitchBuffer[iStitch].y;
						iStitch++;
					}
					InsertCenter.x        = (insertedRectangle.right - insertedRectangle.left) / 2 + insertedRectangle.left;
					InsertCenter.y        = (insertedRectangle.top - insertedRectangle.bottom) / 2 + insertedRectangle.bottom;
					PCSHeader.stitchCount = newStitchCount;
					insertedSize.x        = insertedRectangle.right - insertedRectangle.left;
					insertedSize.y        = insertedRectangle.top - insertedRectangle.bottom;
					form::ratsr();
					InsertSize.x = insertedSize.x * HorizontalRatio;
					// ToDo - Should this be vertical ratio?
					InsertSize.y         = insertedSize.y * HorizontalRatio;
					initialInsertPoint.x = StitchWindowClientRect.right >> 1;
					initialInsertPoint.y = StitchWindowClientRect.bottom >> 1;
					insflin(initialInsertPoint);
					NewFormVertexCount = 5;
					StateMap.set(StateFlag::SHOFRM);
					StateMap.set(StateFlag::INSFIL);
					form::dufrm();
				}
			}
			if (InsertedFileHandle)
				CloseHandle(InsertedFileHandle);
		}
	}
}

void thred::internal::getbak() {
	if (StateMap.test(StateFlag::THUMSHO)) {
		if (ThumbnailsSelected[FileVersionIndex]) {
			if (StateMap.test(StateFlag::RBUT)) {
				wcscpy_s(InsertedFileName, (*Thumbnails)[ThumbnailsSelected[FileVersionIndex]].data());
				StateMap.set(StateFlag::IGNORINS);
				unthum();
				StateMap.set(StateFlag::FRMOF);
				insfil();
				if (GetKeyState(VK_SHIFT) & 0X8000) {
					StateMap.reset(StateFlag::INSFIL);
					StateMap.reset(StateFlag::FRMOF);
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
			}
			else {
				*WorkingFileName = *DefaultDirectory / (*Thumbnails)[ThumbnailsSelected[FileVersionIndex]];
				StateMap.set(StateFlag::REDOLD);
				nuFil();
			}
		}
		else
			unthum();
	}
}

void thred::internal::rebak() {
	unsigned iVersion = 0;
	fs::path newFileName;
	fs::path safetyFileName;

	for (iVersion = 0; iVersion < OLDVER; iVersion++)
		DestroyWindow(BackupViewer[iVersion]);
	newFileName    = *ThrName;
	safetyFileName = *ThrName;
	auto ext       = newFileName.extension().wstring();
	ext.back()     = FileVersionIndex + 's';
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
	unsigned iVersion;

	for (iVersion = 0; iVersion < OLDVER; iVersion++)
		DestroyWindow(BackupViewer[iVersion]);
	getbak();
}

void thred::internal::movbak(char source, char destination) {
	auto sourceFileName      = *ThrName;
	auto destinationFileName = *ThrName;

	auto ext   = ThrName->extension().wstring();
	ext.back() = source;
	sourceFileName.replace_extension(ext);
	ext.back() = destination;
	destinationFileName.replace_extension(ext);

	fs::remove(destinationFileName);
	fs::rename(sourceFileName, destinationFileName);
}

void thred::internal::purg() {
	fs::path fileName;
	char     iLast = 0;

	if (FileHandle) {
		fileName = *ThrName;
		auto ext = ThrName->extension().wstring();
		for (iLast = 0; iLast < OLDVER; iLast++) {
			ext.back() = iLast + 's';
			fileName.replace_extension(ext);
			fs::remove(fileName);
		}
	}
}

void thred::internal::purgdir() {
	StateMap.set(StateFlag::PRGMSG);
	std::wstring fmtStr;
	displayText::loadString(fmtStr, IDS_DELBAK);
	displayText::shoMsg(fmt::format(fmtStr, DefaultDirectory->wstring()));
	displayText::okcan();
}

void thred::internal::deldir() {
	thred::unmsg();
	displayText::tabmsg(IDS_BAKDEL);
	std::wstring backSpec = { L".th0" };

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

bool thred::internal::chkwnd(HWND window) noexcept {
	RECT windowRect;

	GetWindowRect(window, &windowRect);
	if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
	    && Msg.pt.y <= windowRect.bottom)
		return 1;
	else
		return 0;
}

bool thred::internal::chkok() noexcept {
	return chkwnd(OKButton);
}

void thred::internal::mv2f() {
	unsigned attribute = 0, iLowBuffer = 0, iHighBuffer = 0, iStitch = 0;

	if (StateMap.testAndReset(StateFlag::FORMSEL)) {
		// ToDo - Use a temp buffer rather than the high buffer
		thred::savdo();
		iLowBuffer  = 0;
		iHighBuffer = MAXITEMS;
		attribute   = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			else {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
		}
		iStitch = MAXITEMS;
		while (iStitch < iHighBuffer) {
			StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch++];
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.testAndReset(StateFlag::GRPSEL)) {
			thred::savdo();
			thred::rngadj();
			iHighBuffer = MAXITEMS;
			iLowBuffer  = 0;
			for (iStitch = 0; iStitch < GroupStartStitch; iStitch++) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			for (iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			for (iStitch = MAXITEMS; iStitch < iHighBuffer; iStitch++) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void thred::internal::mv2b() {
	unsigned attribute = 0, iLowBuffer = 0, iHighBuffer = 0, iStitch = 0;

	if (StateMap.testAndReset(StateFlag::FORMSEL)) {
		// ToDo - Use a temp buffer rather than the high buffer
		thred::savdo();
		iLowBuffer  = 0;
		iHighBuffer = MAXITEMS;
		attribute   = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			else {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
		}
		iStitch = MAXITEMS;
		while (iStitch < iHighBuffer) {
			StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch++];
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.testAndReset(StateFlag::GRPSEL)) {
			thred::savdo();
			thred::rngadj();
			iHighBuffer = MAXITEMS;
			for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			iLowBuffer = GroupStartStitch;
			for (iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			for (iStitch = MAXITEMS; iStitch < iHighBuffer; iStitch++) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void thred::internal::infadj(float* xCoordinate, float* yCoordinate) noexcept {
	if (xCoordinate && yCoordinate) {
		if (!_finite(*xCoordinate)) {
			if (*xCoordinate > UnzoomedRect.x)
				*xCoordinate = UnzoomedRect.x;
			else {
				if (*xCoordinate < 0)
					*xCoordinate = 0;
			}
		}
		if (!_finite(*yCoordinate)) {
			if (*yCoordinate > UnzoomedRect.y)
				*yCoordinate = UnzoomedRect.y;
			else {
				if (*yCoordinate < 0)
					*yCoordinate = 0;
			}
		}
	}
}

void thred::delinf() noexcept {
	unsigned iStitch = 0, iVertex = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		thi::infadj(&StitchBuffer[iStitch].x, &StitchBuffer[iStitch].y);
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
		thi::infadj(&FormVertices[iVertex].x, &FormVertices[iVertex].y);
}

void thred::chkrng(fPOINT& range) noexcept {
	unsigned iStitch = 0, stitchCount = 0;

	thred::delinf();
	range.x = UnzoomedRect.x;
	range.y = UnzoomedRect.y;
	if (FormIndex) {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute & NOTFRM
			    || ((StitchBuffer[iStitch].attribute & TYPMSK)
			        && ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT < FormIndex))) {
				StitchBuffer[stitchCount] = StitchBuffer[iStitch];
				if (StitchBuffer[stitchCount].x > range.x)
					StitchBuffer[stitchCount].x = range.x - 1;
				if (StitchBuffer[stitchCount].x < 0)
					StitchBuffer[stitchCount].x = 0;
				if (StitchBuffer[stitchCount].y > range.y)
					StitchBuffer[stitchCount].y = range.y - 1;
				if (StitchBuffer[stitchCount].y < 0)
					StitchBuffer[stitchCount].y = 0;
				stitchCount++;
			}
		}
		PCSHeader.stitchCount = stitchCount;
	}
	else {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].x > range.x)
				StitchBuffer[iStitch].x = range.x - 1;
			if (StitchBuffer[iStitch].x < 0)
				StitchBuffer[iStitch].x = 0;
			if (StitchBuffer[iStitch].y > range.y)
				StitchBuffer[iStitch].y = range.y - 1;
			if (StitchBuffer[iStitch].y < 0)
				StitchBuffer[iStitch].y = 0;
		}
	}
}

void thred::ritmov() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if (ClosestVertexToCursor) {
		if (ClosestVertexToCursor == gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1 && SelectedForm->type == FRMLINE)
			Polyline(StitchWindowDC, RubberBandLine->data(), 2);
		else
			Polyline(StitchWindowDC, RubberBandLine->data(), 3);
	}
	else {
		(*RubberBandLine)[2] = FormLines[1];
		if (SelectedForm->type == FRMLINE)
			Polyline(StitchWindowDC, &(*RubberBandLine)[1], 2);
		else
			Polyline(StitchWindowDC, RubberBandLine->data(), 3);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unmov() {
	if (StateMap.testAndReset(StateFlag::SHOMOV))
		thred::ritmov();
}

void thred::internal::duprct() {
	unsigned iVertex       = 0;
	size_t   currentVertex = SelectedFormVertices.start;

	SelectedVerticesRect.left = SelectedVerticesRect.right = CurrentFormVertices[currentVertex].x;
	SelectedVerticesRect.top = SelectedVerticesRect.bottom = CurrentFormVertices[currentVertex].y;
	currentVertex                                          = form::pdir(currentVertex);
	for (iVertex = 0; iVertex < SelectedFormVertices.vertexCount; iVertex++) {
		if (CurrentFormVertices[currentVertex].x < SelectedVerticesRect.left)
			SelectedVerticesRect.left = CurrentFormVertices[currentVertex].x;
		if (CurrentFormVertices[currentVertex].x > SelectedVerticesRect.right)
			SelectedVerticesRect.right = CurrentFormVertices[currentVertex].x;
		if (CurrentFormVertices[currentVertex].y < SelectedVerticesRect.bottom)
			SelectedVerticesRect.bottom = CurrentFormVertices[currentVertex].y;
		if (CurrentFormVertices[currentVertex].y > SelectedVerticesRect.top)
			SelectedVerticesRect.top = CurrentFormVertices[currentVertex].y;
		currentVertex = form::pdir(currentVertex);
	}
}

void thred::setpsel() {
	form::unpsel();
	thi::duprct();
	form::sRct2px(SelectedVerticesRect, SelectedPixelsRect);
	form::rct2sel(SelectedPixelsRect, *SelectedPointsLine);
	form::sfCor2px(CurrentFormVertices[SelectedFormVertices.finish], EndPointCross);
	StateMap.set(StateFlag::SHOPSEL);
	form::dupsel(StitchWindowDC);
	StateMap.set(StateFlag::FPSEL);
}

void thred::rotfn(double rotationAngle, const dPOINT& rotationCenter) {
	unsigned iVertex = 0, iStitch = 0, iForm = 0;
	size_t   currentVertex = 0;
	double   length        = 0.0;
	dPOINT   center        = {};

	thred::savdo();
	if (StateMap.test(StateFlag::FPSEL)) {
		form::fvars(ClosestFormToCursor);
		currentVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			thred::rotflt(CurrentFormVertices[currentVertex], rotationAngle, rotationCenter);
			currentVertex = form::pdir(currentVertex);
		}
		form::frmout(ClosestFormToCursor);
		thred::setpsel();
		form::refil();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
			thred::rotflt(FormVertices[iVertex], rotationAngle, rotationCenter);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			thi::rotstch(&StitchBuffer[iStitch], rotationAngle, rotationCenter);
		for (iForm = 0; iForm < FormIndex; iForm++)
			form::frmout(iForm);
		form::selal();
		return;
	}
	if (StateMap.testAndReset(StateFlag::FRMSROT)) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
				thred::rotflt(CurrentFormVertices[iVertex], rotationAngle, rotationCenter);
			form::frmout(ClosestFormToCursor);
			form::refilfn();
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.testAndReset(StateFlag::FRMROT)) {
			form::fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
				thred::rotflt(CurrentFormVertices[iVertex], rotationAngle, rotationCenter);
			form::frmout(ClosestFormToCursor);
			form::refil();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			length = GroupEndStitch - GroupStartStitch + 1;
			for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
				thi::rotstch(&StitchBuffer[iStitch], rotationAngle, rotationCenter);
			thred::rngadj();
			thi::selin(GroupStartStitch, GroupEndStitch, StitchWindowDC);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void thred::internal::rotfns(double rotationAngle, const dPOINT& rotationCenter) {
	thred::savdo();
	thred::rotfn(rotationAngle, rotationCenter);
}

void thred::internal::nulayr(unsigned play) {
	ActiveLayer = play;
	LayerIndex  = play >> 1;
	ladj();
	if (ActiveLayer) {
		if (StateMap.test(StateFlag::FORMSEL)
		    && (gsl::narrow<unsigned>((FormList[ClosestFormToCursor].attribute & FRMLMSK) >> 1) != ActiveLayer))
			StateMap.reset(StateFlag::FORMSEL);
		StateMap.reset(StateFlag::GRPSEL);
		if (StateMap.test(StateFlag::SELBOX)) {
			if (ActiveLayer != ((StitchBuffer[ClosestPointIndex].attribute & LAYMSK) >> LAYSHFT) + 1)
				StateMap.reset(StateFlag::SELBOX);
		}
		SelectedFormList->clear();
	}
	StateMap.set(StateFlag::RESTCH);
}

bool thred::internal::iselpnt() {
	unsigned iControlPoint = 0, closestControlPoint = 0;
	double   length = 0.0, minimumLength = 1e99;
	POINT    pointToTest = { (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };

	for (iControlPoint = 0; iControlPoint < 9; iControlPoint++) {
		length
		    = hypot(pointToTest.x - (*FormControlPoints)[iControlPoint].x, pointToTest.y - (*FormControlPoints)[iControlPoint].y);
		if (length < minimumLength) {
			minimumLength       = length;
			closestControlPoint = iControlPoint;
		}
	}
	if (minimumLength < CLOSENUF) {
		SelectedFormControlVertex = closestControlPoint;
		return 1;
	}
	else
		return 0;
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

bool thred::internal::chkbig(std::vector<POINT>& stretchBoxLine, double& xyRatio) {
	unsigned iControlPoint = 0;
	double   length = 0.0, minimumLength = 1e99;
	POINT    pointToTest = { (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };

	for (iControlPoint = 0; iControlPoint < 9; iControlPoint++) {
		length
		    = hypot(pointToTest.x - (*SelectedFormsLine)[iControlPoint].x, pointToTest.y - (*SelectedFormsLine)[iControlPoint].y);
		if (length < minimumLength) {
			minimumLength             = length;
			SelectedFormControlVertex = iControlPoint;
		}
	}
	for (size_t iCorner = 0; iCorner < 4; iCorner++) {
		FormLines[iCorner] = (*SelectedFormsLine)[iCorner << 1];
	}
	FormLines[4] = FormLines[0];
	if (minimumLength < CLOSENUF) {
		for (size_t iCorner = 0; iCorner < 4; iCorner++) {
			stretchBoxLine[iCorner] = (*SelectedFormsLine)[iCorner << 1];
		}
		stretchBoxLine[4] = stretchBoxLine[0];
		thred::strtchbox(stretchBoxLine);
		if (SelectedFormControlVertex & 1)
			StateMap.set(StateFlag::STRTCH);
		else {
			StateMap.set(StateFlag::EXPAND);
			xyRatio = static_cast<double>(SelectedFormsRect.right - SelectedFormsRect.left)
			          / (SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		SelectedFormControlVertex >>= 1;
		StateMap.set(StateFlag::SHOSTRTCH);
		return 1;
	}
	if (pointToTest.x >= SelectedFormsRect.left && pointToTest.x <= SelectedFormsRect.right
	    && pointToTest.y >= SelectedFormsRect.top && pointToTest.y <= SelectedFormsRect.bottom) {
		SelectedFormsSize.x = SelectedFormsRect.right - SelectedFormsRect.left;
		SelectedFormsSize.y = SelectedFormsRect.bottom - SelectedFormsRect.top;
		StateMap.set(StateFlag::MOVFRMS);
		FormMoveDelta.x = pointToTest.x - SelectedFormsRect.left;
		FormMoveDelta.y = pointToTest.y - SelectedFormsRect.top;
		StateMap.set(StateFlag::SHOSTRTCH);
		thred::strtchbox(stretchBoxLine);
		return 1;
	}
	return 0;
}

void thred::internal::delfre() {
	unsigned iStitch = 0, currentStitchCount = 0;

	thred::savdo();
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (!(StitchBuffer[iStitch].attribute & NOTFRM)) {
			StitchBuffer[currentStitchCount++] = StitchBuffer[iStitch];
		}
	}
	PCSHeader.stitchCount = currentStitchCount;
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
	size_t start = 0, finish = 0;

	if (LastFormSelected > ClosestFormToCursor) {
		start  = ClosestFormToCursor;
		finish = LastFormSelected;
	}
	else {
		start  = LastFormSelected;
		finish = ClosestFormToCursor;
	}
	SelectedFormList->clear();
	SelectedFormList->reserve(finish - start + 1);
	while (start <= finish) {
		SelectedFormList->push_back(start);
		start++;
	}
}

void thred::internal::selup() {
	unsigned attribute = 0;

	if (GetKeyState(VK_SHIFT) & 0X8000) {
		StateMap.reset(StateFlag::SELBOX);
		if (StateMap.testAndReset(StateFlag::FORMSEL)) {
			if (ClosestFormToCursor < FormIndex - 1) {
				SelectedFormList->push_back(ClosestFormToCursor);
				LastFormSelected = ClosestFormToCursor + 1;
				SelectedFormList->push_back(LastFormSelected);
			}
			else
				return;
		}
		else {
			if (SelectedFormList->size()) {
				if (LastFormSelected < FormIndex - 1) {
					LastFormSelected++;
					dufsel();
				}
			}
			else
				StateMap.set(StateFlag::FORMSEL);
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::SELBOX)) {
			unbox();
			attribute = StitchBuffer[ClosestPointIndex].attribute & ATMSK;
			while (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1
			       && (StitchBuffer[ClosestPointIndex].attribute & ATMSK) == attribute)
				ClosestPointIndex++;
			stch2px(ClosestPointIndex);
			dubox();
		}
		else {
			if (FormIndex) {
				if (StateMap.testAndSet(StateFlag::FORMSEL)) {
					if (ClosestFormToCursor < gsl::narrow<unsigned>(FormIndex) - 1)
						ClosestFormToCursor++;
				}
				else
					ClosestFormToCursor = 0;
				displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void thred::internal::seldwn() {
	unsigned attribute = 0;

	if (GetKeyState(VK_SHIFT) & 0X8000) {
		StateMap.reset(StateFlag::SELBOX);
		if (StateMap.testAndReset(StateFlag::FORMSEL)) {
			if (ClosestFormToCursor) {
				SelectedFormList->push_back(ClosestFormToCursor);
				LastFormSelected = ClosestFormToCursor - 1;
				SelectedFormList->push_back(LastFormSelected);
			}
			else
				return;
		}
		else {
			if (SelectedFormList->size()) {
				if (LastFormSelected) {
					LastFormSelected--;
					dufsel();
				}
			}
			else
				StateMap.set(StateFlag::FORMSEL);
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::SELBOX)) {
			unbox();
			attribute = StitchBuffer[ClosestPointIndex].attribute & ATMSK;
			while (ClosestPointIndex && (StitchBuffer[ClosestPointIndex].attribute & ATMSK) == attribute)
				ClosestPointIndex--;
			stch2px(ClosestPointIndex);
			dubox();
		}
		else {
			if (FormIndex) {
				if (StateMap.testAndSet(StateFlag::FORMSEL)) {
					if (ClosestFormToCursor)
						ClosestFormToCursor--;
				}
				else
					ClosestFormToCursor = FormIndex - 1;
				displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

bool thred::internal::movstchs(unsigned destination, unsigned start, unsigned finish) {
	unsigned count = 0, swap = 0;
	unsigned dind = MAXITEMS;

	// ToDo - Use a temp buffer rather than the high buffer
	if (destination < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
		destination++;
	if (start > finish) {
		swap   = start;
		start  = finish;
		finish = swap;
	}
	if (destination >= start && destination < finish) {
		displayText::tabmsg(IDS_DST1);
		return 0;
	}
	if (destination < start) {
		count = finish - start;
		thred::mvstchs(dind, start, count);
		dind += count;
		count = start - destination;
		thred::mvstchs(dind, destination, count);
		dind += count;
		thred::mvstchs(destination, MAXITEMS, dind - (MAXITEMS));
	}
	else {
		count = destination - finish;
		thred::mvstchs(dind, finish, count);
		dind += count;
		count = finish - start;
		thred::mvstchs(dind, start, count);
		dind += count;
		thred::mvstchs(start, MAXITEMS, dind - (MAXITEMS));
	}
	return 1;
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
				if (movstchs(MoveAnchor, ClosestPointIndex, ClosestPointIndex + 1)) {
					StateMap.set(StateFlag::RESTCH);
					thred::coltab();
				}
			}
		}
	}
}

void thred::internal::vuthrds() {
	if (GetMenuState(ViewMenu, ID_VUTHRDS, MF_BYCOMMAND) & MF_CHECKED) {
		CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		StateMap.reset(StateFlag::THRDS);
	}
	else {
		CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_CHECKED);
		StateMap.set(StateFlag::THRDS);
	}
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::vuselthr() {
	if (GetMenuState(ViewMenu, ID_VUSELTHRDS, MF_BYCOMMAND) & MF_CHECKED) {
		CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		StateMap.reset(StateFlag::COL);
	}
	else {
		CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_CHECKED);
		StateMap.set(StateFlag::COL);
	}
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::colchk() noexcept {
	unsigned color         = StitchBuffer[0].attribute & COLMSK;
	unsigned currentStitch = 0, iStitch = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (color != (StitchBuffer[iStitch].attribute & COLMSK)) {
			if ((iStitch - currentStitch == 1) && (currentStitch))
				StitchBuffer[iStitch - 1].attribute
				    = StitchBuffer[iStitch].attribute & NCOLMSK | (StitchBuffer[currentStitch - 1].attribute & COLMSK);
			color         = StitchBuffer[iStitch].attribute & COLMSK;
			currentStitch = iStitch;
		}
	}
}

unsigned thred::internal::makbig(unsigned start, unsigned finish) {
	unsigned destination = MAXITEMS, source = 0, stitchCount = 0, iStitch = 0, adcnt = 0, attribute = 0;
	double   length = 0.0;
	dPOINT   delta = {}, step = {}, point = {};

	// ToDo - Use a temp buffer rather than the high buffer
	for (source = start; source < gsl::narrow<unsigned>(finish) - 1; source++) {
		delta.x = StitchBuffer[source + 1].x - StitchBuffer[source].x;
		delta.y = StitchBuffer[source + 1].y - StitchBuffer[source].y;
		length  = hypot(delta.x, delta.y);
		thred::mvstch(destination++, source);
		if (length > IniFile.maxStitchLength) {
			stitchCount = ceil(length / UserStitchLength);
			step.x      = delta.x / stitchCount;
			step.y      = delta.y / stitchCount;
			point.x     = StitchBuffer[source].x + step.x;
			point.y     = StitchBuffer[source].y + step.y;
			attribute   = StitchBuffer[source].attribute;
			if (attribute != StitchBuffer[source + 1].attribute) {
				if (!(attribute & NOTFRM) && StitchBuffer[source + 1].attribute & TYPMSK) {
					if (!((attribute & FRMSK) == (StitchBuffer[source + 1].attribute & FRMSK)))
						attribute &= NTYPMSK;
				}
				else
					attribute &= NTYPMSK;
			}
			attribute &= (~KNOTMSK);
			for (iStitch = 0; iStitch < stitchCount - 1; iStitch++) {
				StitchBuffer[destination].attribute = attribute;
				StitchBuffer[destination].x         = point.x;
				StitchBuffer[destination++].y       = point.y;
				point.x += step.x;
				point.y += step.y;
				adcnt++;
			}
		}
	}
	while (source < PCSHeader.stitchCount)
		thred::mvstch(destination++, source++);
	const fPOINTATTR* sourceStitch      = &StitchBuffer[MAXITEMS];
	fPOINTATTR*       destinationStitch = &StitchBuffer[start];
	if (sourceStitch && destinationStitch) {
		stitchCount = destination - (MAXITEMS);
		for (source = 0; source < stitchCount; source++)
			destinationStitch[source] = sourceStitch[source];
		PCSHeader.stitchCount = start + stitchCount;
	}
	return adcnt;
}

void thred::internal::rembig() {
	RANGE range = {};

	if (UserStitchLength < IniFile.maxStitchLength) {
		thred::savdo();
		do {
			if (SelectedFormList->size()) {
				for (auto selectedForm : (*SelectedFormList)) {
					form::frmrng(selectedForm, range);
					makbig(range.start, range.finish);
				}
				break;
			}
			if (StateMap.test(StateFlag::FORMSEL)) {
				form::frmrng(ClosestFormToCursor, range);
				makbig(range.start, range.finish);
				break;
			}
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::rngadj();
				if (GroupEndStitch < PCSHeader.stitchCount)
					GroupEndStitch++;
				if (ClosestPointIndex < GroupStitchIndex)
					GroupStitchIndex += makbig(GroupStartStitch, GroupEndStitch);
				else
					ClosestPointIndex += makbig(GroupStartStitch, GroupEndStitch);
				thred::grpAdj();
				break;
			}
			makbig(0, PCSHeader.stitchCount);
		} while (false);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		displayText::tabmsg(IDS_REM1);
}

void thred::internal::duselrng() {
	SelectedRange.start  = 0;
	SelectedRange.finish = PCSHeader.stitchCount;
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		SelectedRange.start  = GroupStartStitch;
		SelectedRange.finish = GroupEndStitch;
		return;
	}
}

void thred::internal::longer() {
	unsigned iStitch = 0, currentStitch = 0;
	double   length = 0.0, minimumLength = 1e99;
	double   currentLength = 0.0;
	bool     flag          = true;

	if (ClosestPointIndex == LargestStitchIndex)
		return;
	currentLength = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x,
	                      StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (iStitch = ClosestPointIndex + 1; iStitch < SelectedRange.finish; iStitch++) {
		length
		    = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
		if (length == currentLength) {
			flag = false;
			break;
		}
	}
	if (flag) {
		for (currentStitch = SelectedRange.start; currentStitch < SelectedRange.finish - 1; currentStitch++) {
			length = hypot(StitchBuffer[currentStitch + 1].x - StitchBuffer[currentStitch].x,
			               StitchBuffer[currentStitch + 1].y - StitchBuffer[currentStitch].y);
			if (length > currentLength && length < minimumLength) {
				minimumLength = length;
				iStitch       = currentStitch;
			}
		}
		if (minimumLength == 1e99)
			return;
	}
	CurrentStitchIndex = iStitch;
	lensadj();
	displayText::ritnum(STR_NUMSCH, ClosestPointIndex);
}

void thred::internal::shorter() {
	unsigned iStitch = 0, currentStitch = 0;
	double   length = 0.0, maximumLength = 0.0;
	double   currentLength = 0.0;
	bool     flag          = true;

	if (ClosestPointIndex == SmallestStitchIndex)
		return;
	currentLength = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x,
	                      StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (currentStitch = ClosestPointIndex; currentStitch != 0; currentStitch--) {
		length = hypot(StitchBuffer[currentStitch].x - StitchBuffer[currentStitch - 1].x,
		               StitchBuffer[currentStitch].y - StitchBuffer[currentStitch - 1].y);
		if (length == currentLength) {
			currentStitch--;
			flag = false;
			break;
		}
	}
	if (flag) {
		for (iStitch = SelectedRange.start; iStitch < SelectedRange.finish - 1; iStitch++) {
			length = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x,
			               StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
			if (length < currentLength && length > maximumLength) {
				maximumLength = length;
				currentStitch = iStitch;
			}
		}
		const float minLength
		    = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
		displayText::butxt(HMINLEN, fmt::format(L"{:.2f}", minLength));
	}
	CurrentStitchIndex = currentStitch;
	lensadj();
	displayText::ritnum(STR_NUMSCH, ClosestPointIndex);
}

void thred::internal::setsrch(unsigned stitch) {
	CurrentStitchIndex = stitch;
	lensadj();
	displayText::ritnum(STR_NUMSCH, ClosestPointIndex);
}

bool thred::internal::inrng(unsigned stitch) noexcept {
	if (StitchBuffer[stitch].x >= StitchRangeRect.left && StitchBuffer[stitch].x <= StitchRangeRect.right
	    && StitchBuffer[stitch].y >= StitchRangeRect.bottom && StitchBuffer[stitch].y <= StitchRangeRect.top)

		return 1;
	else
		return 0;
}

bool thred::internal::finrng(size_t find) noexcept {
	unsigned cod = 0;

	if (FormList[find].rectangle.left >= StitchRangeRect.left && FormList[find].rectangle.right <= StitchRangeRect.right
	    && FormList[find].rectangle.bottom >= StitchRangeRect.bottom && FormList[find].rectangle.top <= StitchRangeRect.top) {
		if (!ActiveLayer)
			return 1;
		cod = (FormList[find].attribute & FRMLMSK) >> 1;
		if (!cod || ActiveLayer == cod)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void thred::internal::ungrplo() {
	unsigned iStitch = 0;

	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		thred::rngadj();
		ClosestPointIndex = GroupStartStitch;
		StateMap.set(StateFlag::SELBOX);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			bool flag = true;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (!(StitchBuffer[iStitch].attribute & NOTFRM)
				    && ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
					ClosestPointIndex = iStitch;
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

void thred::internal::ungrphi() {
	unsigned iStitch = 0;

	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		thred::rngadj();
		ClosestPointIndex = GroupEndStitch;
		StateMap.set(StateFlag::SELBOX);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			bool flag = true;
			for (iStitch = PCSHeader.stitchCount; iStitch != 0; iStitch--) {
				if (!(StitchBuffer[iStitch - 1].attribute & NOTFRM)
				    && ((StitchBuffer[iStitch - 1].attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
					ClosestPointIndex = iStitch - 1;
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

int strcomp(const void* arg1, const void* arg2) noexcept {
	if (arg1 && arg2) {
		return _stricmp(*static_cast<char* const*>(arg1), *static_cast<char* const*>(arg2));
	}
	return 0;
}

void thred::internal::barnam(HWND window, unsigned iThumbnail) {
	if (iThumbnail < ThumbnailDisplayCount) {
		fs::path thumbPath((*Thumbnails)[ThumbnailsSelected[iThumbnail]].data());
		auto     name = thumbPath.stem().wstring().substr(0, 12);
		SetWindowText(window, name.c_str());
	}
	else
		SetWindowText(window, L"");
}

void thred::internal::rthumnam(unsigned iThumbnail) {
	switch (iThumbnail) {
	case 0:
		barnam((*ButtonWin)[HNUM], iThumbnail);
		break;
	case 1:
		barnam((*ButtonWin)[HTOT], iThumbnail);
		break;
	case 2:
		barnam((*ButtonWin)[HMINLEN], iThumbnail);
		break;
	case 3:
		barnam((*ButtonWin)[HMAXLEN], iThumbnail);
		break;
	}
}

void thred::internal::thumnail() {
	WIN32_FIND_DATA fileData   = {};
	unsigned        iThumbnail = 0;
	HANDLE          file       = {};

	thred::unbsho();
	thred::undat();
	trace::untrace();

	fs::current_path(*DefaultDirectory);
	*SearchName = *DefaultDirectory / L"*.thr";
	file        = FindFirstFile(SearchName->wstring().c_str(), &fileData);
	if (file == INVALID_HANDLE_VALUE) {
		const DWORD  dwError = GetLastError();
		std::wstring fmtStr;
		displayText::loadString(fmtStr, IDS_FFINDERR);
		displayText::shoMsg(fmt::format(fmtStr, SearchName->wstring(), dwError));
		unthum();
	}
	else {
		Thumbnails->clear();
		Thumbnails->push_back(fileData.cFileName);
		while (FindNextFile(file, &fileData)) {
			Thumbnails->push_back(fileData.cFileName);
		}
		FindClose(file);
		std::sort(Thumbnails->begin(), Thumbnails->end());
		iThumbnail           = 0;
		const auto thumbSize = Thumbnails->size();
		while (iThumbnail < 4 && iThumbnail < thumbSize) {
			ThumbnailsSelected[iThumbnail] = iThumbnail;
			iThumbnail++;
		}
		ThumbnailIndex = ThumbnailDisplayCount = iThumbnail;
		while (iThumbnail < 4 && iThumbnail < thumbSize)
			rthumnam(iThumbnail++);
		StateMap.set(StateFlag::THUMSHO);
		ThumbnailSearchString[0] = 0;
		SetWindowText((*ButtonWin)[HBOXSEL], L"");
		const std::wstring blank(L"");
		displayText::butxt(HBOXSEL, blank);
		vubak();
		StateMap.set(StateFlag::RESTCH);
	}
}

void thred::internal::nuthsel() {
	unsigned iThumbnail = 0;
	size_t   length = 0, savedIndex = 0;

	if (ThumbnailIndex < Thumbnails->size()) {
		savedIndex = ThumbnailIndex;
		iThumbnail = 0;
		length     = wcslen(ThumbnailSearchString);
		StateMap.set(StateFlag::RESTCH);
		if (length) {
			while (iThumbnail < 4 && ThumbnailIndex < Thumbnails->size()) {
				if (!wcsncmp(ThumbnailSearchString, (*Thumbnails)[ThumbnailIndex].data(), length)) {
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
		if (iThumbnail) {
			ThumbnailDisplayCount = iThumbnail;
			while (iThumbnail < 4)
				rthumnam(iThumbnail++);
		}
		else
			ThumbnailIndex = savedIndex;
	}
}

void thred::internal::nuthbak(unsigned count) {
	size_t length = 0;

	if (ThumbnailIndex) {
		length = wcslen(ThumbnailSearchString);
		if (length) {
			while (count && ThumbnailIndex < MAXFORMS) {
				if (ThumbnailIndex) {
					ThumbnailIndex--;
					if (!wcsncmp(ThumbnailSearchString, (*Thumbnails)[ThumbnailIndex].data(), length))
						count--;
				}
				else
					break;
			}
		}
		else
			ThumbnailIndex -= count;
		if (ThumbnailIndex > MAXFORMS)
			ThumbnailIndex = 0;
		nuthsel();
	}
}

void thred::internal::nuthum(char character) {
	size_t iString;

	iString = wcslen(ThumbnailSearchString);
	if (iString < 16) {
		StateMap.set(StateFlag::RESTCH);
		ThumbnailSearchString[iString++] = character;
		ThumbnailSearchString[iString]   = 0;
		std::wstring txt(ThumbnailSearchString);
		displayText::butxt(HBOXSEL, txt);
		ThumbnailIndex = 0;
		nuthsel();
	}
}

void thred::internal::bakthum() {
	size_t searchStringLength;

	searchStringLength = wcslen(ThumbnailSearchString);
	if (searchStringLength) {
		StateMap.set(StateFlag::RESTCH);
		ThumbnailSearchString[--searchStringLength] = 0;
		ThumbnailIndex                              = 0;
		std::wstring txt(ThumbnailSearchString);
		displayText::butxt(HBOXSEL, txt);
		nuthsel();
	}
}

void thred::internal::selalstch() {
	if (PCSHeader.stitchCount) {
		ClosestPointIndex = 0;
		GroupStitchIndex  = PCSHeader.stitchCount - 1;
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
	fPOINT offset = {};

	thred::px2stch();
	offset.x = SelectedPoint.x - InsertCenter.x;
	offset.y = SelectedPoint.y - InsertCenter.y;
	for (auto iForm = InsertedFormIndex; iForm < FormIndex; iForm++) {
		auto& formRectangle = FormList[iForm].rectangle;
		formRectangle.bottom += offset.y;
		formRectangle.top += offset.y;
		formRectangle.left += offset.x;
		formRectangle.right += offset.x;
	}
	for (auto iVertex = InsertedVertexIndex; iVertex < FormVertexIndex; iVertex++) {
		FormVertices[iVertex].x += offset.x;
		FormVertices[iVertex].y += offset.y;
	}
	for (auto iStitch = InsertedStitchCount; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].x += offset.x;
		StitchBuffer[iStitch].y += offset.y;
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
	unsigned iStitch = 0, iRange = 0, rangeCount = 0;
	bool     flagInRange  = false;
	unsigned largestRange = 0, maximumLength = 0, length = 0;

	if (!StateMap.testAndReset(StateFlag::WASFPNT)) {
		StateMap.reset(StateFlag::GRPSEL);
		std::vector<RANGE> prng(MAXITEMS);
		StateMap.reset(StateFlag::GRPSEL);
		while (iStitch < PCSHeader.stitchCount) {
			if (inrng(iStitch)) {
				if (!flagInRange) {
					prng[iRange].start = iStitch;
					flagInRange        = true;
				}
			}
			else {
				if (flagInRange) {
					prng[iRange].finish = iStitch - 1;
					iRange++;
					flagInRange = false;
				}
			}
			iStitch++;
		}
		if (flagInRange) {
			prng[iRange].finish = iStitch - 1;
			iRange++;
		}
		rangeCount = iRange;
		if (rangeCount) {
			maximumLength = 0;
			for (iRange = 0; iRange < rangeCount; iRange++) {
				length = prng[iRange].finish - prng[iRange].start;
				if (length > maximumLength) {
					maximumLength = length;
					largestRange  = iRange;
				}
			}
			if (maximumLength) {
				ClosestPointIndex = prng[largestRange].start;
				GroupStitchIndex  = prng[largestRange].finish;
				StateMap.set(StateFlag::GRPSEL);
			}
			gotbox();
		}
	}
}

void thred::internal::nucols() {
	unsigned iStitch = 0;

	boost::dynamic_bitset<> formMap(FormIndex);
	for (auto selectedForm : (*SelectedFormList)) {
		formMap.set(selectedForm);
		SelectedForm = &FormList[selectedForm];
		if (SelectedForm->fillType) {
			SelectedForm->fillColor              = ActiveColor;
			SelectedForm->fillInfo.feather.color = ActiveColor;
		}
		if (SelectedForm->edgeType)
			SelectedForm->borderColor = ActiveColor;
		if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK))
			SelectedForm->underlayColor = ActiveColor;
	}
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)
		    && (StitchBuffer[iStitch].attribute & TYPMSK) != TYPMSK) {
			StitchBuffer[iStitch].attribute &= NCOLMSK;
			StitchBuffer[iStitch].attribute |= ActiveColor;
		}
	}
}

bool thred::internal::dunum(unsigned code) noexcept {
	if (code >= '0' && code <= '9') {
		NumericCode = code;
		return 1;
	}
	if (code >= VK_NUMPAD0 && code <= VK_NUMPAD9) {
		NumericCode = code - '0';
		return 1;
	}
	return 0;
}

void thred::stchrct(fRECTANGLE& rectangle) noexcept {
	unsigned ind = 0;

	if (PCSHeader.stitchCount) {
		rectangle.bottom = rectangle.left = 1e10;
		rectangle.top = rectangle.right = 0;
		if (PCSHeader.stitchCount) {
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if (StitchBuffer[ind].x < rectangle.left)
					rectangle.left = StitchBuffer[ind].x;
				if (StitchBuffer[ind].x > rectangle.right)
					rectangle.right = StitchBuffer[ind].x;
				if (StitchBuffer[ind].y < rectangle.bottom)
					rectangle.bottom = StitchBuffer[ind].y;
				if (StitchBuffer[ind].y > rectangle.top)
					rectangle.top = StitchBuffer[ind].y;
			}
		}
	}
}

void thred::frmrct(fRECTANGLE& rectangle) noexcept {
	unsigned iVertex = 0;

	rectangle.left = rectangle.right = FormVertices[0].x;
	rectangle.top = rectangle.bottom = FormVertices[0].y;
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {
		if (FormVertices[iVertex].x < rectangle.left)
			rectangle.left = FormVertices[iVertex].x;
		if (FormVertices[iVertex].x > rectangle.right)
			rectangle.right = FormVertices[iVertex].x;
		if (FormVertices[iVertex].y > rectangle.top)
			rectangle.top = FormVertices[iVertex].y;
		if (FormVertices[iVertex].y < rectangle.bottom)
			rectangle.bottom = FormVertices[iVertex].y;
	}
}

void thred::internal::desiz() {
	fRECTANGLE   rectangle = {};
	FLOAT        xSize = 0.0, ySize = 0.0;
	std::wstring info;
	auto         stringTable = *StringTable;

	if (PCSHeader.stitchCount) {
		thred::stchrct(rectangle);
		xSize = (rectangle.right - rectangle.left) / PFGRAN;
		ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
		if ((rectangle.left < 0) || (rectangle.bottom < 0) || (rectangle.right > IniFile.hoopSizeX)
		    || (rectangle.top > IniFile.hoopSizeY)) {
			info += stringTable[STR_STCHOUT];
		}
		info += fmt::format(stringTable[STR_STCHS], PCSHeader.stitchCount, xSize, (xSize / 25.4), ySize, (ySize / 25.4));
	}
	if (FormIndex) {
		thred::frmrct(rectangle);
		xSize = (rectangle.right - rectangle.left) / PFGRAN;
		ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
		info += fmt::format(stringTable[STR_FORMS], FormIndex, xSize, (xSize / 25.4), ySize, (ySize / 25.4));
	}
	info += fmt::format(stringTable[STR_HUPWID], (IniFile.hoopSizeX / PFGRAN), (IniFile.hoopSizeY / PFGRAN));
	if (PCSHeader.stitchCount) {
		auto modifier = utf::Utf8ToUtf16(ExtendedHeader.modifierName);
		info += fmt::format(stringTable[STR_CREATBY], *DesignerName, modifier);
	}
	displayText::shoMsg(info);
}

void thred::internal::sidhup() {
	RECT     hoopRectangle        = {};
	RECT     preferencesRectangle = {};
	unsigned iHoop                = 0;

	StateMap.set(StateFlag::HUPMSG);
	GetWindowRect((*ValueWindow)[PHUP], &hoopRectangle);
	GetWindowRect(PreferencesWindow, &preferencesRectangle);
	SideMessageWindow = CreateWindow(L"STATIC",
	                                 0,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 preferencesRectangle.right + 3 - ThredWindowOrigin.x,
	                                 hoopRectangle.top - ThredWindowOrigin.y,
	                                 ButtonWidthX3 + ButtonWidth * 2 + 6,
	                                 ButtonHeight * HUPS + 6,
	                                 ThrEdWindow,
	                                 NULL,
	                                 ThrEdInstance,
	                                 NULL);
	for (iHoop = 0; iHoop < HUPS; iHoop++) {
		SideWindow[iHoop] = CreateWindow(L"STATIC",
		                                 (*StringTable)[iHoop + STR_HUP0].c_str(),
		                                 SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                 3,
		                                 ButtonHeight * iHoop + 3,
		                                 ButtonWidthX3 + ButtonWidth * 2,
		                                 ButtonHeight,
		                                 SideMessageWindow,
		                                 NULL,
		                                 ThrEdInstance,
		                                 NULL);
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
	if (FormIndex || PCSHeader.stitchCount) {
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

void thred::internal::clpradj(fPOINTATTR stitch) noexcept {
	if (stitch.x < ClipRectAdjusted.left)
		ClipRectAdjusted.left = stitch.x;
	if (stitch.x > ClipRectAdjusted.right)
		ClipRectAdjusted.right = stitch.x;
	if (stitch.y < ClipRectAdjusted.bottom)
		ClipRectAdjusted.bottom = stitch.y;
	if (stitch.y > ClipRectAdjusted.top)
		ClipRectAdjusted.top = stitch.y;
}

void thred::internal::clpadj() {
	double   clipMiddle = 0.0;
	unsigned iStitch    = 0;

	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		iStitch               = GroupStartStitch;
		ClipRectAdjusted.left = ClipRect.right = StitchBuffer[iStitch++].x;
		ClipRectAdjusted.top = ClipRectAdjusted.bottom = StitchBuffer[iStitch].y;
		clpradj(StitchBuffer[iStitch]);
		while (iStitch < GroupEndStitch)
			clpradj(StitchBuffer[iStitch++]);
		if (StitchBuffer[iStitch].x < ClipRectAdjusted.left)
			ClipRectAdjusted.left = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].x > ClipRectAdjusted.right)
			ClipRectAdjusted.right = StitchBuffer[iStitch].x;
		clipMiddle                       = (ClipRectAdjusted.right - ClipRectAdjusted.left) / 2 + ClipRectAdjusted.left;
		StitchBuffer[GroupStartStitch].y = StitchBuffer[GroupEndStitch].y
		    = (ClipRectAdjusted.top - ClipRectAdjusted.bottom) / 2 + ClipRectAdjusted.bottom;
		if (StitchBuffer[GroupStartStitch].x < clipMiddle) {
			StitchBuffer[GroupStartStitch].x = ClipRectAdjusted.left;
			StitchBuffer[GroupEndStitch].x   = ClipRectAdjusted.right;
		}
		else {
			StitchBuffer[GroupEndStitch].x   = ClipRectAdjusted.left;
			StitchBuffer[GroupStartStitch].x = ClipRectAdjusted.right;
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		displayText::shoseln(IDS_GRPMSG, IDS_RNGEND);
}

void thred::internal::shftflt(fPOINT point) noexcept {
	if (point.x <= ZoomRect.left || point.x >= ZoomRect.right || point.y <= ZoomRect.bottom || point.y >= ZoomRect.top)
		thred::shft(point);
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
	unsigned iColor = 0;

	UserFlagMap.reset();
	for (iColor = 0; iColor < 16; iColor++) {
		UserColor[iColor]              = DefaultUserColors[iColor];
		CustomColor[iColor]            = DefaultCustomColors[iColor];
		CustomBackgroundColor[iColor]  = DefaultCustomBackgroundColors[iColor];
		BitmapBackgroundColors[iColor] = DefaultBitmapBackgroundColors[iColor];
	}
	formForms::dazdef();
	AppliqueColor          = 15;
	BorderWidth            = BRDWID;
	ButtonholeCornerLength = IBFCLEN;
	IniFile.chainSpace     = CHSDEF;
	IniFile.chainRatio     = CHRDEF;
	IniFile.fillAngle      = DEFANG;
	UserFlagMap.reset(UserFlag::SQRFIL);
	LineSpacing             = DEFSPACE * PFGRAN;
	ShowStitchThreshold     = SHOPNTS;
	IniFile.gridSize        = 12;
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
	if (!IniFile.customHoopX)
		IniFile.customHoopX = LHUPX;
	if (!IniFile.customHoopY)
		IniFile.customHoopY = LHUPY;
	PicotSpacing = IPICSPAC;
	UserFlagMap.set(UserFlag::FIL2OF);
	fil2men();
	BackgroundColor              = 0xa8c4b1;
	UnzoomedRect.x               = IniFile.hoopSizeX;
	UnzoomedRect.y               = IniFile.hoopSizeY;
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
	IniFile.textureSpacing       = static_cast<float>(ITXSPAC);
}

void thred::internal::dumrk(double xCoord, double yCoord) {
	if (StateMap.testAndReset(StateFlag::GMRK))
		drwmrk(StitchWindowDC);
	ZoomMarkPoint = { xCoord, yCoord };
	StateMap.set(StateFlag::INIT);
	StateMap.set(StateFlag::GMRK);
	drwmrk(StitchWindowDC);
	StateMap.set(StateFlag::WASMRK);
}

void thred::internal::gselrng() {
	auto selectedFormList = *SelectedFormList;

	SelectedFormsRange.start = SelectedFormsRange.finish = selectedFormList[0];
	for (auto selectedForm : selectedFormList) {
		if (selectedForm < SelectedFormsRange.start)
			SelectedFormsRange.start = selectedForm;
		if (selectedForm > SelectedFormsRange.finish)
			SelectedFormsRange.finish = selectedForm;
	}
}

double thred::internal::nuang(double yDelta, double xDelta) noexcept {
	double angle = 0.0, relativeAngle = 0.0;

	angle         = atan2(yDelta, xDelta);
	relativeAngle = angle - OriginalAngle;
	if (fabs(relativeAngle) > PI) {
		if (relativeAngle > 0)
			relativeAngle = 2 * PI - relativeAngle;
		else
			relativeAngle += 2 * PI;
	}
	return relativeAngle;
}

void thred::internal::angdif(double angle) noexcept {
	if (angle > HighestAngle)
		HighestAngle = angle;
	else {
		if (angle < LowestAngle)
			LowestAngle = angle;
	}
}

void thred::internal::rotmrk() {
	unsigned iVertex = 0, iStitch = 0, segments = 0, codedFormIndex = 0;
	double   tang = 0.0;

	if (StateMap.test(StateFlag::GMRK) && (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL))) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			codedFormIndex = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
			form::fvars(ClosestFormToCursor);
			LowestAngle = HighestAngle = 0;
			OriginalAngle = atan2(CurrentFormVertices[0].y - ZoomMarkPoint.y, CurrentFormVertices[0].x - ZoomMarkPoint.x);
			for (iVertex = 1; iVertex < VertexCount; iVertex++)
				angdif(nuang(CurrentFormVertices[iVertex].y - ZoomMarkPoint.y, CurrentFormVertices[iVertex].x - ZoomMarkPoint.x));
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) == codedFormIndex)
					angdif(nuang(StitchBuffer[iStitch].y - ZoomMarkPoint.y, StitchBuffer[iStitch].x - ZoomMarkPoint.x));
			}
		}
		else {
			thred::rngadj();
			LowestAngle = HighestAngle = 0;
			OriginalAngle
			    = atan2(StitchBuffer[GroupStartStitch].y - ZoomMarkPoint.y, StitchBuffer[GroupStartStitch].x - ZoomMarkPoint.x);
			for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++)
				angdif(nuang(StitchBuffer[iStitch].y - ZoomMarkPoint.y, StitchBuffer[iStitch].x - ZoomMarkPoint.x));
		}
		tang                  = HighestAngle - LowestAngle;
		segments              = 2 * PI / tang;
		IniFile.rotationAngle = 2 * PI / segments;
		std::wstring fmtStr;
		displayText::loadString(fmtStr, IDS_ROTMARK);
		// ToDo - should this be IniFile.rotationAngle?
		displayText::shoMsg(fmt::format(fmtStr, IniFile.fillAngle * 180 / PI, segments));
	}
	else
		displayText::shoseln(IDS_FSZ, IDS_SETROTM);
}

void thred::internal::segentr(double rotationAngle) {
	if (!rotationAngle)
		rotationAngle = PI / 180;
	displayText::shoMsg(fmt::format((*StringTable)[STR_ENTROT], (2 * PI / rotationAngle)));
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
			dumrk(StitchBuffer[ClosestPointIndex].x, StitchBuffer[ClosestPointIndex].y);
			break;
		}
		if (StateMap.test(StateFlag::FRMPSEL)) {
			dumrk(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].x,
			      FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].y);
			break;
		}
		displayText::shoseln(IDS_STCH_FRM, IDS_SETMRK);
	} while (false);
}

void thred::internal::filfrms() {
	if (SelectedFormList->size()) {
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

void thred::internal::nuslst(size_t find) {
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
		else
			StateMap.reset(StateFlag::GRPSEL);
	}
	else {
		if (StateMap.test(StateFlag::GRPSEL)) {
			StateMap.set(StateFlag::WASGRP);
			thred::rngadj();
			PrevGroupStartStitch = GroupStartStitch;
			PrevGroupEndStitch   = GroupEndStitch;
		}
		else
			StateMap.reset(StateFlag::WASGRP);
	}
	duselrng();
}

unsigned thred::internal::duswap(unsigned data) noexcept {
	return _byteswap_ulong(data);
}

void thred::internal::ritcur() noexcept {
	ICONINFO      iconInfo       = {};
	HCURSOR       currentCursor  = GetCursor();
	POINT         cursorPosition = {};
	unsigned      iRow = 0, iPixel = 0;
	unsigned      mask = 0, bitmapInverse = 0;
	unsigned      bitMask        = 0;
	unsigned      pixelColor     = 0;
	unsigned char bitmapBits[64] = {};

	GetIconInfo(currentCursor, &iconInfo);
	GetCursorPos(&cursorPosition);
	cursorPosition.x -= (StitchWindowOrigin.x + iconInfo.xHotspot);
	cursorPosition.y -= (StitchWindowOrigin.y + iconInfo.yHotspot);
	// ToDo - replace with GetDIBits
	GetBitmapBits(iconInfo.hbmMask, 256, &bitmapBits);
	if (currentCursor == ArrowCursor) {
		for (iRow = 0; iRow < 32; iRow++) {
			mask          = duswap(bitmapBits[iRow]);
			bitmapInverse = duswap(bitmapBits[iRow + 32]);
			bitMask       = 0x80000000;
			for (iPixel = 0; iPixel < 32; iPixel++) {
				if (!(bitMask & mask)) {
					if (bitmapInverse & bitMask)
						pixelColor = 0xffffff;
					else
						pixelColor = 0x000000;
					SetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow, pixelColor);
				}
				bitMask >>= 1;
			}
		}
	}
	else {
		for (iRow = 0; iRow < 32; iRow++) {
			bitmapInverse = duswap(bitmapBits[iRow + 32]);
			bitMask       = 0x80000000;
			for (iPixel = 0; iPixel < 32; iPixel++) {
				if (bitMask & bitmapInverse)
					SetPixel(StitchWindowDC,
					         cursorPosition.x + iPixel,
					         cursorPosition.y + iRow,
					         GetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow) ^ 0xffffff);
				bitMask >>= 1;
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
	if (!StateMap.testAndFlip(StateFlag::HIDSTCH))
		StateMap.reset(StateFlag::FRMOF);
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::respac() noexcept {
	if (clip::isclp(ClosestFormToCursor)) {
		SelectedForm->fillSpacing = LineSpacing;
		form::fsizpar();
	}
}

bool thred::internal::chkminus(unsigned code) noexcept {
	if (code == 189 || code == 109) { // '-' key pressed
		if (PreferenceIndex == PFAZ)  // Clipboard Offset in preferences
			return 1;
		if (FormMenuChoice == LFRMSPAC && isfclp()) // Fill Spacing for fan clip fill
			return 1;
		if (FormMenuChoice == LWLKIND) // Indent
			return 1;
	}
	return 0;
}

void thred::internal::retrac() {
	unsigned source = 0, destination = 0;

	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		if (!GroupStartStitch)
			GroupStartStitch++;
		form::makspac(GroupEndStitch + 1, GroupEndStitch - GroupStartStitch);
		source      = GroupEndStitch - 1;
		destination = GroupEndStitch + 1;
		while (source >= GroupStartStitch) {
			StitchBuffer[destination++] = StitchBuffer[source--];
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		displayText::shoseln(IDS_GRPMSG, IDS_RETRAC);
}

void thred::internal::setgrd(COLORREF color) {
	unsigned     iCode       = 0;
	const GRDCOD gridCodes[] = {
		ID_GRDHI, HIGRD, ID_GRDMED, MEDGRD, ID_GRDEF, DEFGRD, ID_GRDRED, REDGRD, ID_GRDBLU, BLUGRD, ID_GRDGRN, GRNGRD,
	};

	for (iCode = 0; iCode < 6; iCode++) {
		if (color == gridCodes[iCode].col)
			CheckMenuItem(MainMenu, gridCodes[iCode].id, MF_CHECKED);
		else
			CheckMenuItem(MainMenu, gridCodes[iCode].id, MF_UNCHECKED);
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

void thred::internal::fil2sel(unsigned stat) {
	UserFlagMap.set(UserFlag::FIL2OF);
	if (stat)
		UserFlagMap.reset(UserFlag::FIL2OF);
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

void thred::internal::rotauxsel(unsigned stat) {
	UserFlagMap.set(UserFlag::ROTAUX);
	if (!stat)
		UserFlagMap.reset(UserFlag::ROTAUX);
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

void thred::internal::frmcursel(unsigned cursorType) {
	UserFlagMap.set(UserFlag::FRMX);
	if (!cursorType)
		UserFlagMap.reset(UserFlag::FRMX);
	frmcurmen();
	StateMap.set(StateFlag::DUMEN);
}

void thred::internal::stchsnap(unsigned start, unsigned finish) noexcept {
	fPOINTATTR* pnt = &StitchBuffer[start];

	for (unsigned i = 0; i < finish - start; i++) {
		pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
		pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;

		pnt++;
	}
}

void thred::internal::frmsnap(fPOINT* start, size_t count) noexcept {
	for (unsigned i = 0; i < count; i++) {
		start->x = rintf(start->x / IniFile.gridSize) * IniFile.gridSize;
		start->y = rintf(start->y / IniFile.gridSize) * IniFile.gridSize;

		start++;
	}
}

void thred::internal::gsnap() {
	if (StateMap.test(StateFlag::TXTRED)) {
		texture::txsnap();
		return;
	}
	if (SelectedFormList->size()) {
		thred::savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			frmsnap(FormList[ClosestFormToCursor].vertices, FormList[ClosestFormToCursor].vertexCount);
			form::frmout(ClosestFormToCursor);
			form::refil();
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			frmsnap(FormList[ClosestFormToCursor].vertices, FormList[ClosestFormToCursor].vertexCount);
			form::frmout(ClosestFormToCursor);
			form::refil();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				stchsnap(GroupStartStitch, GroupEndStitch + 1);
				StateMap.set(StateFlag::RESTCH);
			}
			else
				displayText::shoseln(IDS_FGRPF, IDS_SNAP2GRD);
		}
	}
}

void thred::internal::ritlock(const WIN32_FIND_DATA* fileData, unsigned fileIndex, HWND hwndlg) noexcept {
	if (fileData) {
		unsigned iFile;

		SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_RESETCONTENT, 0, 0);
		SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_RESETCONTENT, 0, 0);
		for (iFile = 0; iFile < fileIndex; iFile++) {
			if (fileData[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				[[gsl::suppress(type .1)]] SendMessage(
				    GetDlgItem(hwndlg, IDC_LOCKED), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(fileData[iFile].cFileName));
			else
				[[gsl::suppress(type .1)]] SendMessage(
				    GetDlgItem(hwndlg, IDC_UNLOCKED), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(fileData[iFile].cFileName));
		}
	}
}

INT_PTR CALLBACK thred::internal::LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	FINDINFO* fileInfo     = nullptr;
	HANDLE    searchResult = {};
	fs::path  searchName;
	fs::path  fileName;
	unsigned  iFile = 0, fileError = 0;
	HWND      lockHandle = {}, unlockHandle = {};

	switch (umsg) {
	case WM_INITDIALOG:
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		SetWindowLongPtr(hwndlg, DWLP_USER, lparam);
		[[gsl::suppress(type .1)]] fileInfo = reinterpret_cast<FINDINFO*>(lparam);
		if (fileInfo) {
			searchName   = *DefaultDirectory / L"*.thr";
			searchResult = FindFirstFile(searchName.wstring().c_str(), &(fileInfo->data[0]));
			if (searchResult == INVALID_HANDLE_VALUE) {
				std::wstring fmtStr;
				displayText::loadString(fmtStr, IDS_NOTHRFIL);
				displayText::shoMsg(fmt::format(fmtStr, DefaultDirectory->wstring()));
				EndDialog(hwndlg, wparam);
				return TRUE;
			}
			fileInfo->count = 1;
			while (FindNextFile(searchResult, &fileInfo->data[fileInfo->count++]))
				;
			fileInfo->count--;
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
		}
		break;
	case WM_COMMAND:
		[[gsl::suppress(type .1)]] fileInfo = reinterpret_cast<FINDINFO*>(GetWindowLongPtr(hwndlg, DWLP_USER));
		if (fileInfo) {
			switch (LOWORD(wparam)) {
			case IDCANCEL:
				EndDialog(hwndlg, wparam);
				return TRUE;
			case IDC_LOCKAL:
				for (iFile = 0; iFile < fileInfo->count; iFile++)
					fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			case IDC_UNLOCKAL:
				for (iFile = 0; iFile < fileInfo->count; iFile++)
					fileInfo->data[iFile].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			case IDC_LOCK:
				fileError    = 0;
				unlockHandle = GetDlgItem(hwndlg, IDC_UNLOCKED);
				for (iFile = 0; iFile < fileInfo->count; iFile++) {
					if (!(fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
						if (SendMessage(unlockHandle, LB_GETSEL, fileError, 0))
							fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
						fileError++;
					}
				}
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			case IDC_UNLOCK:
				fileError  = 0;
				lockHandle = GetDlgItem(hwndlg, IDC_LOCKED);
				for (iFile = 0; iFile < fileInfo->count; iFile++) {
					if ((fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
						if (SendMessage(lockHandle, LB_GETSEL, fileError, 0))
							fileInfo->data[iFile].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
						fileError++;
					}
				}
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			case IDOK:
				fileError = 0;
				for (iFile = 0; iFile < fileInfo->count; iFile++) {
					fileName = *DefaultDirectory / fileInfo->data[iFile].cFileName;
					if (!SetFileAttributes(fileName.wstring().c_str(), fileInfo->data[iFile].dwFileAttributes))
						fileError++;
				}
				if (fileError) {
					std::wstring fmtStr;
					displayText::loadString(fmtStr, IDS_LOCKNOT);
					displayText::shoMsg(fmt::format(fmtStr, fileError));
				}
				EndDialog(hwndlg, wparam);
				return TRUE;
				break;
			}
		}
	}
	return 0;
}

void thred::internal::lock() {
	// ToDo - investigate C++17 option shown here: http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
	FINDINFO lockInfo;

	lockInfo.count = 0;
	// ToDo - Replace 512 with maximum files in subdirectory
	lockInfo.data = new WIN32_FIND_DATA[512];

	[[gsl::suppress(type .1)]] DialogBoxParam(
	    ThrEdInstance, MAKEINTRESOURCE(IDD_DLOCK), ThrEdWindow, LockPrc, reinterpret_cast<LPARAM>(&lockInfo));

	delete[] lockInfo.data;
}

void thred::internal::delstch() {
	thred::savdo();
	PCSHeader.stitchCount = 0;
	ClipPointIndex        = 0;
	TextureIndex          = 0;
	rstAll();
	form::clrfills();
	ColorChanges = 0;
	const std::wstring blank(L"");
	displayText::butxt(HNUM, blank);
	displayText::butxt(HTOT, blank);
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::chkbit() {
	if (PCSBMPFileName[0]
	    && (StateMap.test(StateFlag::WASDIF) || StateMap.test(StateFlag::WASDSEL) || StateMap.test(StateFlag::WASBLAK))) {
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
	SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRED], *DesignerName).c_str());
}

void thred::internal::filclos() {
	if (savcmp())
		closfn();
	else {
		StateMap.set(StateFlag::FCLOS);
		displayText::savdisc();
	}
}

void thred::internal::frmpos(float deltaX, float deltaY) noexcept {
	unsigned iVertex;

	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		SelectedForm->vertices[iVertex].x += deltaX;
		SelectedForm->vertices[iVertex].y += deltaY;
	}
	SelectedForm->rectangle.bottom += deltaY;
	SelectedForm->rectangle.top += deltaY;
	SelectedForm->rectangle.left += deltaX;
	SelectedForm->rectangle.right += deltaX;
}

void thred::internal::nudgfn(float deltaX, float deltaY) {
	unsigned iForm = 0, iStitch = 0;
	POINT    pixel = {};

	if (StateMap.test(StateFlag::BIGBOX) || SelectedFormList->size() || StateMap.test(StateFlag::FORMSEL)
	    || StateMap.test(StateFlag::GRPSEL) || StateMap.test(StateFlag::SELBOX))
		thred::savdo();
	if (StateMap.test(StateFlag::BIGBOX)) {
		for (iForm = 0; iForm < FormIndex; iForm++) {
			SelectedForm = &FormList[iForm];
			frmpos(deltaX, deltaY);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			StitchBuffer[iStitch].x += deltaX;
			StitchBuffer[iStitch].y += deltaY;
		}
		AllItemsRect.bottom += deltaY;
		AllItemsRect.top += deltaY;
		AllItemsRect.left += deltaX;
		AllItemsRect.right += deltaX;
		form::stchrct2px(AllItemsRect, SelectedFormsRect);
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedFormList->size()) {
		boost::dynamic_bitset<> formMap(FormIndex);
		for (auto selectedForm : (*SelectedFormList)) {
			formMap.set(selectedForm);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
				StitchBuffer[iStitch].x += deltaX;
				StitchBuffer[iStitch].y += deltaY;
			}
		}
		for (auto selectedForm : (*SelectedFormList)) {
			SelectedForm = &FormList[selectedForm];
			frmpos(deltaX, deltaY);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::FORMSEL)) {
		SelectedForm = &FormList[ClosestFormToCursor];
		frmpos(deltaX, deltaY);
		if (SelectedForm->fillType || SelectedForm->edgeType) {
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
					StitchBuffer[iStitch].x += deltaX;
					StitchBuffer[iStitch].y += deltaY;
				}
			}
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
			StitchBuffer[iStitch].x += deltaX;
			StitchBuffer[iStitch].y += deltaY;
		}
		thred::grpAdj();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::SELBOX)) {
		StitchBuffer[ClosestPointIndex].x += deltaX;
		StitchBuffer[ClosestPointIndex].y += deltaY;
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	pixel = {};
	if (deltaX) {
		if (deltaX > 0)
			pixel.x = IniFile.nudgePixels;
		else
			pixel.x = -IniFile.nudgePixels;
	}
	if (deltaY) {
		if (deltaY > 0)
			pixel.y = -IniFile.nudgePixels;
		else
			pixel.y = +IniFile.nudgePixels;
	}
	// ToDo - use SendInput instead
	mouse_event(MOUSEEVENTF_MOVE, pixel.x, pixel.y, 0, 0);
}

void thred::internal::pixmsg(unsigned iString, unsigned pixelCount) {
	displayText::shoMsg(fmt::format((*StringTable)[iString], pixelCount));
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
	fPOINT point = {};

	if (StateMap.test(StateFlag::WASMRK)) {
		if (ZoomMarkPoint.x > IniFile.hoopSizeX)
			ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
		if (ZoomMarkPoint.y > IniFile.hoopSizeY)
			ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
		dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
		point = ZoomMarkPoint;
		thred::shft(point);
		StateMap.set(StateFlag::RESTCH);
	}
	else
		displayText::tabmsg(IDS_MRK);
}

void thred::internal::nuscol(unsigned iColor) noexcept {
	UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
	UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	thred::redraw(UserColorWin[iColor]);
}

void thred::internal::movchk() {
	unsigned iStitch = 0, iForm = 0, color = 0, key = 0, swapColor = 0, switchColors = 0;

	if (Msg.wParam & MK_LBUTTON) {
		if (!StateMap.testAndSet(StateFlag::WASMOV)) {
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
				DraggedColor = VerticalIndex & 0xf;
				StateMap.set(StateFlag::WASCOL);
			}
		}
	}
	else {
		if (StateMap.testAndReset(StateFlag::WASMOV) && StateMap.testAndReset(StateFlag::WASCOL)) {
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
				key          = GetKeyState(VK_SHIFT) & 0X8000;
				switchColors = GetKeyState(VK_CONTROL) & 0X8000;
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
					color = StitchBuffer[iStitch].attribute & COLMSK;
					if (color == VerticalIndex) {
						StitchBuffer[iStitch].attribute &= NCOLMSK;
						StitchBuffer[iStitch].attribute |= DraggedColor;
					}
					else {
						if (!key && color == DraggedColor) {
							StitchBuffer[iStitch].attribute &= NCOLMSK;
							StitchBuffer[iStitch].attribute |= VerticalIndex;
						}
					}
				}
				for (iForm = 0; iForm < FormIndex; iForm++) {
					const auto pfrm = &FormList[iForm];
					if (pfrm->fillType) {
						if (pfrm->fillColor == VerticalIndex)
							pfrm->fillColor = DraggedColor;
						else {
							if (!key && pfrm->fillColor == DraggedColor)
								pfrm->fillColor = VerticalIndex;
						}
						if (pfrm->fillInfo.feather.color == VerticalIndex)
							pfrm->fillInfo.feather.color = DraggedColor;
						else {
							if (!key && pfrm->fillInfo.feather.color == DraggedColor)
								pfrm->fillInfo.feather.color = VerticalIndex;
						}
					}
					if (pfrm->edgeType) {
						if (pfrm->borderColor == VerticalIndex)
							pfrm->borderColor = DraggedColor;
						else {
							if (!key && pfrm->borderColor == DraggedColor)
								pfrm->borderColor = VerticalIndex;
						}
					}
				}
				if (!switchColors) {
					swapColor                = UserColor[VerticalIndex];
					UserColor[VerticalIndex] = UserColor[DraggedColor];
					if (!key) {
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
	unsigned iStitch = 0, iForm = 0, iColor = 0, nextColor = 0, color = 0;

	boost::dynamic_bitset<> colorMap(16);
	if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {
		VerticalIndex &= COLMSK;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			colorMap.set(StitchBuffer[iStitch].attribute & COLMSK);
		if (colorMap.all())
			displayText::tabmsg(IDS_COLAL);
		else {
			nextColor = 15;
			while (colorMap.test(nextColor))
				nextColor--;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				color = StitchBuffer[iStitch].attribute & COLMSK;
				if (color >= VerticalIndex && color < nextColor) {
					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= color + 1;
				}
			}
			for (iForm = 0; iForm < FormIndex; iForm++) {
				const auto form = &FormList[iForm];
				if (form->fillType) {
					if (form->fillColor >= VerticalIndex && form->fillColor < nextColor)
						form->fillColor++;
					if (form->fillInfo.feather.color >= VerticalIndex && form->fillInfo.feather.color < nextColor)
						form->fillInfo.feather.color++;
				}
				if (form->edgeType) {
					if (form->borderColor >= VerticalIndex && form->borderColor < nextColor)
						form->borderColor++;
				}
			}
			for (iColor = nextColor; iColor > VerticalIndex; iColor--) {
				if (iColor) {
					UserColor[iColor] = UserColor[iColor - 1];
				}
				nuscol(iColor);
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

bool thred::internal::usedcol() noexcept {
	unsigned iStitch;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & COLMSK) == VerticalIndex)
			return 1;
	}
	return 0;
}

void thred::internal::delcol() {
	unsigned iStitch = 0, iForm = 0, iColor = 0, color = 0;

	if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {
		VerticalIndex &= 0xf;
		if (usedcol())
			displayText::tabmsg(IDS_COLU);
		else {
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				color = StitchBuffer[iStitch].attribute & COLMSK;
				if (color > VerticalIndex && color) {
					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= color - 1;
				}
			}
			for (iForm = 0; iForm < FormIndex; iForm++) {
				const auto form = &FormList[iForm];
				if (form->fillType) {
					if (form->fillColor > VerticalIndex)
						form->fillColor--;
					if (form->fillInfo.feather.color > VerticalIndex)
						form->fillInfo.feather.color--;
				}
				if (form->edgeType) {
					if (form->borderColor > VerticalIndex)
						form->borderColor--;
				}
			}
			for (iColor = VerticalIndex; iColor < 15; iColor++) {
				UserColor[iColor] = UserColor[iColor + 1];
				nuscol(iColor);
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void thred::internal::set1knot() {
	unsigned source = 0, destination = 0;

	if (PCSHeader.stitchCount && StateMap.test(StateFlag::SELBOX)) {
		thred::savdo();
		if (ClosestPointIndex == gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			StateMap.set(StateFlag::FILDIR);
			OutputIndex = ClosestPointIndex + 1;
			endknt(ClosestPointIndex);
		}
		else {
			source      = PCSHeader.stitchCount - 1;
			destination = PCSHeader.stitchCount + 4;
			while (source && source >= ClosestPointIndex)
				thred::mvstch(destination--, source--);
			OutputIndex = ClosestPointIndex + 1;
			strtknt(ClosestPointIndex);
		}
		PCSHeader.stitchCount += 5;
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::msgstr(IDS_NOSTCHSEL);
		displayText::shoMsg(MsgBuffer);
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
		ClosestVertexToCursor = FormList[ClosestFormToCursor].vertexCount - 1;
	}
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::setpclp() noexcept {
	POINT    offset = {};
	POINT    point  = {};
	unsigned ind = 0, ine = 0;

	form::sfCor2px(InterleaveSequence[0], point);
	FormVerticesAsLine[0] = point;
	form::sfCor2px(InterleaveSequence[1], point);
	offset.x = Msg.pt.x - StitchWindowOrigin.x - point.x;
	offset.y = Msg.pt.y - StitchWindowOrigin.y - point.y;
	// ToDo - Why use ine instead of ind
	for (ind = 0; ind < OutputIndex - 2; ind++) {
		ine = ind + 1;
		form::sfCor2px(InterleaveSequence[ine], point);
		FormVerticesAsLine[ine].x = point.x + offset.x;
		FormVerticesAsLine[ine].y = point.y + offset.y;
	}
	ind++;
	form::sfCor2px(InterleaveSequence[ind], point);
	FormVerticesAsLine[ind] = point;
}

void thred::internal::dupclp() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	PolylineInt(StitchWindowDC, FormVerticesAsLine, OutputIndex);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unpclp() {
	if (StateMap.testAndReset(StateFlag::SHOP))
		dupclp();
}

void thred::internal::fixpclp() {
	POINT  point  = { (Msg.pt.x + static_cast<LONG>(FormMoveDelta.x)), (Msg.pt.y + static_cast<LONG>(FormMoveDelta.y)) };
	fPOINT offset = {};
	size_t iNext  = 0;
	size_t count  = 0;

	thred::pxCor2stch(point);
	offset.x = SelectedPoint.x - InterleaveSequence[1].x;
	offset.y = SelectedPoint.y - InterleaveSequence[1].y;
	iNext    = form::nxt(ClosestVertexToCursor);
	count    = OutputIndex - 2;
	form::fltspac(&CurrentFormVertices[iNext], count);
	for (size_t iOutput = 1; iOutput < OutputIndex - 1; iOutput++) {
		CurrentFormVertices[iNext].x = InterleaveSequence[iOutput].x + offset.x;
		CurrentFormVertices[iNext].y = InterleaveSequence[iOutput].y + offset.y;
		iNext++;
	}
	SelectedForm->vertexCount += gsl::narrow<unsigned short>(count);
	form::refil();
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

bool thred::internal::sidclp() {
	form::fvars(ClosestFormToCursor);
	if (OpenClipboard(ThrEdWindow)) {
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			thred::redclp();
			CloseClipboard();
			return 1;
		}
	}
	CloseClipboard();
	return 0;
}

void thred::internal::selfpnt() {
	StateMap.reset(StateFlag::FORMSEL);
	StateMap.set(StateFlag::FRMPSEL);
	StateMap.reset(StateFlag::FPSEL);
	StateMap.reset(StateFlag::SELBOX);
	SelectedFormVertices.start = ClosestVertexToCursor;
	SelectedFormVertices.form  = ClosestFormToCursor;
	thred::ritfcor(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
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
	thred::redraw((*ButtonWin)[HHID]);
	CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
	StateMap.reset(StateFlag::COL);
	CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
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
}

void thred::internal::qcode() {
	trace::untrace();
	StateMap.reset(StateFlag::HIDMAP);
	StateMap.reset(StateFlag::FILDIR);
	ReleaseCapture();
	if (PCSHeader.stitchCount == 1)
		PCSHeader.stitchCount = 0;
	if (!UserFlagMap.test(UserFlag::MARQ))
		StateMap.reset(StateFlag::GMRK);
	if (StateMap.testAndReset(StateFlag::PRFACT)) {
		if (StateMap.testAndReset(StateFlag::WASRT)) {
			StateMap.set(StateFlag::INSRT);
			StateMap.set(StateFlag::RESTCH);
			return;
		}
		else
			rstAll();
	}
	else
		rstAll();
	StateMap.set(StateFlag::RESTCH);
	/*
	if (BufferIndex) {
	        BufferIndex          = 0;
	        StitchEntryBuffer[0] = 0;
	        ClosestPointIndex    = 0;
	        movbox();
	    }
	*/
	const std::wstring blank(L"");
	displayText::butxt(HNUM, blank);
	return;
}

void thred::internal::drwLin(std::vector<POINT>& linePoints, unsigned currentStitch, unsigned length, HPEN hPen) {
	unsigned          iOffset = 0, layer = 0;
	const fPOINTATTR* activeStitch = &StitchBuffer[currentStitch];

	if (activeStitch) {
		if (ActiveLayer)
			LineIndex = 0;
		for (iOffset = 0; iOffset < length; iOffset++) {
			layer = (activeStitch[iOffset].attribute & LAYMSK) >> LAYSHFT;
			if (!ActiveLayer || !layer || (layer == ActiveLayer)) {
				linePoints[LineIndex].x = (activeStitch[iOffset].x - ZoomRect.left) * ZoomRatio.x;
				linePoints[LineIndex++].y
				    = StitchWindowClientRect.bottom - (activeStitch[iOffset].y - ZoomRect.bottom) * ZoomRatio.y;
			}
		}
		SelectObject(StitchWindowMemDC, hPen);
		// ToDo - where did 16000 come from?
		if (LineIndex < 16000)
			PolylineInt(StitchWindowMemDC, linePoints.data(), LineIndex);
		else {
			iOffset = 0;
			while (LineIndex) {
				if (LineIndex > 16000) {
					Polyline(StitchWindowMemDC, &linePoints[iOffset], 16000);
					iOffset += 15999;
					LineIndex -= 15999;
				}
				else {
					PolylineInt(StitchWindowMemDC, &linePoints[iOffset], LineIndex);
					break;
				}
			}
		}
		LineIndex = 1;
		layer     = (activeStitch[iOffset].attribute & LAYMSK) >> LAYSHFT;
		if (!ActiveLayer || !layer || layer == ActiveLayer) {
			if (iOffset) {
				linePoints[0].x = (activeStitch[iOffset - 1].x - ZoomRect.left) * ZoomRatio.x;
				linePoints[0].y = StitchWindowClientRect.bottom - (activeStitch[iOffset - 1].y - ZoomRect.bottom) * ZoomRatio.y;
			}
			else {
				linePoints[0].x = (activeStitch[0].x - ZoomRect.left) * ZoomRatio.x;
				linePoints[0].y = StitchWindowClientRect.bottom - (activeStitch[0].y - ZoomRect.bottom) * ZoomRatio.y;
			}
		}
	}
	else {
		throw;
	}
}

BOOL CALLBACK thred::internal::fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	wchar_t      buf[HBUFSIZ]  = { 0 };
	wchar_t      buf1[HBUFSIZ] = { 0 };
	unsigned     iFeatherStyle = 0, state = 0, featherType = 0;
	std::wstring featherStyle;

	switch (umsg) {
	case WM_INITDIALOG:
		featherType = IniFile.featherType;
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), fmt::format(L"{:.2f}", IniFile.featherRatio).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), fmt::format(L"{}", IniFile.featherUpCount).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), fmt::format(L"{}", IniFile.featherDownCount).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFLR), fmt::format(L"{:.2f}", (IniFile.featherMinStitchSize / PFGRAN)).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), fmt::format(L"{}", IniFile.featherCount).c_str());
		for (iFeatherStyle = 0; iFeatherStyle < 6; iFeatherStyle++) {
			displayText::loadString(featherStyle, (IDS_FTH0 + iFeatherStyle));
			[[gsl::suppress(type .1)]] SendMessage(
			    GetDlgItem(hwndlg, IDC_FDTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(featherStyle.c_str()));
		}
		SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, IniFile.featherFillType - 1, 0);
		if (featherType & AT_FTHBLND)
			state = BST_CHECKED;
		else
			state = BST_UNCHECKED;
		CheckDlgButton(hwndlg, IDC_FDBLND, state);
		if (featherType & AT_FTHUP)
			state = BST_CHECKED;
		else
			state = BST_UNCHECKED;
		CheckDlgButton(hwndlg, IDC_FDUP, state);
		if (featherType & AT_FTHBTH)
			state = BST_CHECKED;
		else
			state = BST_UNCHECKED;
		CheckDlgButton(hwndlg, IDC_FBTH, state);
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDCANCEL:
			EndDialog(hwndlg, 0);
			return TRUE;
		case IDOK:
			IniFile.featherType = 0;
			if (IsDlgButtonChecked(hwndlg, IDC_FDBLND))
				IniFile.featherType = AT_FTHBLND;
			if (IsDlgButtonChecked(hwndlg, IDC_FDUP))
				IniFile.featherType |= AT_FTHUP;
			if (IsDlgButtonChecked(hwndlg, IDC_FBTH))
				IniFile.featherType |= AT_FTHBTH;
			GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), buf, HBUFSIZ);
			IniFile.featherFillType = FDEFTYP;
			for (iFeatherStyle = 0; iFeatherStyle < 6; iFeatherStyle++) {
				LoadString(ThrEdInstance, IDS_FTH0 + iFeatherStyle, buf1, HBUFSIZ);
				if (!wcscmp(buf, buf1)) {
					IniFile.featherFillType = iFeatherStyle + 1;
					break;
				}
			}
			GetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf, HBUFSIZ);
			IniFile.featherRatio = std::stof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf, HBUFSIZ);
			IniFile.featherUpCount = std::stoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf, HBUFSIZ);
			IniFile.featherDownCount = std::stoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf, HBUFSIZ);
			IniFile.featherMinStitchSize = std::stof(buf) * PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf, HBUFSIZ);
			IniFile.featherCount = std::stoi(buf);
			if (IniFile.featherCount < 1)
				IniFile.featherCount = 1;
			EndDialog(hwndlg, 1);
			break;
		}
	}
	return 0;
}

void thred::internal::dufdef() noexcept {
	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, (DLGPROC)fthdefprc);
}

unsigned thred::internal::chkMsg(std::vector<POINT>& stretchBoxLine, double& xyRatio, double& rotationAngle, dPOINT& rotationCenter) {
	double     colorBarPosition = 0.0, ratio = 0.0, swapCoordinate = 0.0, swapFactor = 0.0;
	FRMHED*    forms           = nullptr;
	fPOINT     adjustedPoint   = {};
	fPOINT*    clipData        = nullptr;
	fPOINT     newSize         = {};
	fRECTANGLE formsRect       = {};
	long       lineLength      = 0;
	POINT      point           = {};
	RECT       windowRect      = {};
	SATCON*    guides          = nullptr;
	wchar_t    buffer[20]      = { 0 };
	char       threadSizeMap[] = { '3', '4', '6' };
	TXPNT*     textureSource   = nullptr;
	unsigned   code = 0, currentClip = 0, currentGuide = 0, currentVertex = 0;
	size_t     byteCount = 0, clipCount = 0;
	unsigned   dst = 0, iClip = 0, iColor = 0, iFillType = 0, iGuide = 0, iHoop = 0;
	unsigned   iLayer = 0, iPreference = 0, iStitch = 0;
	size_t     iSelectedVertex = 0, selectedVertexCount = 0, nextVertex = 0;
	unsigned   iThreadSize = 0, iVersion = 0, iWindow = 0;
	unsigned   stitchAttribute = 0, textureCount = 0;
	WPARAM     wParameter = {};

	if (Msg.message == WM_MOUSEMOVE) {
		if (StateMap.test(StateFlag::TXTMOV)) {
			texture::txtrmov();
			return 1;
		}
		movchk();
		if (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right && Msg.pt.y >= StitchWindowAbsRect.top
		    && Msg.pt.y <= StitchWindowAbsRect.bottom) {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				xt::mvshft();
				return 1;
			}
			do {
				if (GetKeyState(VK_SHIFT) & 0x8000 && thred::px2stch())
					thred::ritfcor(SelectedPoint);
				if ((StateMap.test(StateFlag::PRFACT) || StateMap.test(StateFlag::FORMIN) || StateMap.test(StateFlag::POLIMOV))
				    || FormDataSheet) {
					SetCursor(ArrowCursor);
					break;
				}
				if (!StateMap.test(StateFlag::INIT)) {
					if (UserFlagMap.test(UserFlag::NEDOF))
						SetCursor(CrossCursor);
					else
						SetCursor(NeedleUpCursor);
					break;
				}
				if (StateMap.test(StateFlag::INSRT)) {
					if (UserFlagMap.test(UserFlag::NEDOF))
						SetCursor(CrossCursor);
					else {
						if (StateMap.test(StateFlag::LIN1))
							SetCursor(NeedleUpCursor);
						else {
							if (StitchBuffer[ClosestPointIndex + 1].x > StitchBuffer[ClosestPointIndex].x) {
								if (StitchBuffer[ClosestPointIndex + 1].y > StitchBuffer[ClosestPointIndex].y)
									SetCursor(NeedleLeftUpCursor);
								else
									SetCursor(NeedleLeftDownCursor);
							}
							else {
								if (StitchBuffer[ClosestPointIndex + 1].y > StitchBuffer[ClosestPointIndex].y)
									SetCursor(NeedleRightUpCursor);
								else
									SetCursor(NeedleRightDownCursor);
							}
						}
					}
					break;
				}
				if (StateMap.test(StateFlag::BZUMIN) || StateMap.test(StateFlag::BOXZUM) || StateMap.test(StateFlag::SELPNT)) {
					SetCursor(CrossCursor);
					break;
				}
				if (StateMap.test(StateFlag::SATIN) || StateMap.test(StateFlag::SATPNT) || StateMap.test(StateFlag::INSFRM)) {
					if (UserFlagMap.test(UserFlag::FRMX))
						SetCursor(CrossCursor);
					else
						SetCursor(FormCursor);
					break;
				}
				if (StateMap.test(StateFlag::SATCNKT)) {
					SetCursor(DLineCursor);
				}
				else {
					SetCursor(ArrowCursor);
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
				point.x = Msg.pt.x - StitchWindowOrigin.x;
				point.y = Msg.pt.y - StitchWindowOrigin.y;
				insflin(point);
				StateMap.set(StateFlag::SHOFRM);
				form::dufrm();
				return 1;
			}
			if (StateMap.test(StateFlag::MOVFRMS)) {
				unstrtch(stretchBoxLine);
				stretchBoxLine[0].x = stretchBoxLine[3].x = stretchBoxLine[4].x
				    = Msg.pt.x - FormMoveDelta.x - StitchWindowOrigin.x;
				stretchBoxLine[1].x = stretchBoxLine[2].x
				    = Msg.pt.x + SelectedFormsSize.x - FormMoveDelta.x - StitchWindowOrigin.x;
				stretchBoxLine[0].y = stretchBoxLine[1].y = stretchBoxLine[4].y
				    = Msg.pt.y - FormMoveDelta.y - StitchWindowOrigin.y;
				stretchBoxLine[2].y = stretchBoxLine[3].y
				    = Msg.pt.y + SelectedFormsSize.y - FormMoveDelta.y - StitchWindowOrigin.y;
				StateMap.set(StateFlag::SHOSTRTCH);
				thred::strtchbox(stretchBoxLine);
				return 1;
			}
			if (StateMap.test(StateFlag::POLIMOV)) {
				form::munfrm();
				form::setmfrm();
				StateMap.set(StateFlag::SHOFRM);
				form::mdufrm();
				return 1;
			}
			if (StateMap.test(StateFlag::EXPAND)) {
				unstrtch(stretchBoxLine);
				newSize.x  = Msg.pt.x - StitchWindowOrigin.x;
				newSize.y  = Msg.pt.y - StitchWindowOrigin.y;
				auto iSide = (SelectedFormControlVertex + 2) % 4;
				ratio = fabs(static_cast<double>(newSize.x - stretchBoxLine[iSide].x) / (newSize.y - stretchBoxLine[iSide].y));
				if (iSide & 1) {
					if (ratio < xyRatio)
						newSize.x = (stretchBoxLine[iSide].y - newSize.y) * xyRatio + stretchBoxLine[iSide].x;
					else
						newSize.y = (stretchBoxLine[iSide].x - newSize.x) / xyRatio + stretchBoxLine[iSide].y;
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].y = gsl::narrow<long>(round(newSize.y));
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = gsl::narrow<long>(round(newSize.x));
					stretchBoxLine[iSide].y = gsl::narrow<long>(round(newSize.y));
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = gsl::narrow<long>(round(newSize.x));
				}
				else {
					if (ratio < xyRatio)
						newSize.x = (newSize.y - stretchBoxLine[iSide].y) * xyRatio + stretchBoxLine[iSide].x;
					else
						newSize.y = (newSize.x - stretchBoxLine[iSide].x) / xyRatio + stretchBoxLine[iSide].y;
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = gsl::narrow<long>(round(newSize.x));
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = gsl::narrow<long>(round(newSize.x));
					stretchBoxLine[iSide].y = gsl::narrow<long>(round(newSize.y));
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].y = gsl::narrow<long>(round(newSize.y));
				}
				stretchBoxLine[4] = stretchBoxLine[0];
				StateMap.set(StateFlag::SHOSTRTCH);
				thred::strtchbox(stretchBoxLine);
				return 1;
			}
			if (StateMap.test(StateFlag::STRTCH)) {
				unstrtch(stretchBoxLine);
				if (SelectedFormControlVertex & 1)
					lineLength = Msg.pt.x - StitchWindowOrigin.x;
				else
					lineLength = Msg.pt.y - StitchWindowOrigin.y;
				dst  = (SelectedFormControlVertex + 2) % 4;
				code = nxtcrnr(dst);
				for (size_t iSide = 0u; iSide < 4; iSide++) {
					if (iSide != dst && iSide != code) {
						if (SelectedFormControlVertex & 1)
							stretchBoxLine[iSide].x = lineLength;
						else
							stretchBoxLine[iSide].y = lineLength;
					}
				}
				stretchBoxLine[4] = stretchBoxLine[0];
				StateMap.set(StateFlag::SHOSTRTCH);
				thred::strtchbox(stretchBoxLine);
				return 1;
			}
			if (StateMap.test(StateFlag::INSFRM)) {
				form::uninsf();
				InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				StateMap.set(StateFlag::SHOINSF);
				form::duinsf();
				return 1;
			}
			if (StateMap.test(StateFlag::FUNCLP)) {
				form::unfrm();
				form::setmfrm();
				StateMap.set(StateFlag::SHOFRM);
				form::dufrm();
				return 1;
			}
			if (StateMap.test(StateFlag::SATCNKT)) {
				form::drwcon();
				return 1;
			}
			if (StateMap.test(StateFlag::SATPNT)) {
				satin::drwsat();
				return 1;
			}
			if (StateMap.test(StateFlag::FRMOV)) {
				form::munfrm();
				form::setmfrm();
				StateMap.set(StateFlag::SHOFRM);
				form::mdufrm();
				return 1;
			}
			if (StateMap.test(StateFlag::FRMPMOV)) {
				unmov();
				(*RubberBandLine)[1].x = Msg.pt.x - StitchWindowOrigin.x;
				(*RubberBandLine)[1].y = Msg.pt.y - StitchWindowOrigin.y;
				StateMap.set(StateFlag::SHOMOV);
				thred::ritmov();
				if (thred::px2stch())
					thred::ritfcor(SelectedPoint);
				return 1;
			}
			if (StateMap.test(StateFlag::MOVCNTR)) {
				unrot();
				thred::px2stch();
				rotationCenter = SelectedPoint;
				ritrot(rotationAngle, rotationCenter);
				return 1;
			}
			if (StateMap.test(StateFlag::ROTCAPT)) {
				unrotu();
				unrot();
				RotateBoxToCursorLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				RotateBoxToCursorLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				adjustedPoint.x            = RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x;
				adjustedPoint.y            = RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y;
				if (adjustedPoint.x) {
					rotationAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
				}
				else {
					if (adjustedPoint.y > 0)
						rotationAngle = PI / 2;
					else
						rotationAngle = -PI / 2;
				}
				rotationAngle -= RotationHandleAngle;
				ritrot(rotationAngle, rotationCenter);
				StateMap.set(StateFlag::ROTUSHO);
				durotu();
				return 1;
			}
			if (StateMap.test(StateFlag::SELPNT)) {
				if (StateMap.testAndSet(StateFlag::VCAPT))
					SetCapture(ThrEdWindow);
				unsel();
				rSelbox();
				return 1;
			}
			if (StateMap.test(StateFlag::CLPSHO)) {
				unclp();
				clpbox();
				return 1;
			}
			if (StateMap.test(StateFlag::CAPT)) {
				if (thred::px2stch())
					thred::ritfcor(SelectedPoint);
				unlin();
				MoveLine0[1].x = MoveLine1[0].x = Msg.pt.x - StitchWindowOrigin.x;
				MoveLine0[1].y = MoveLine1[0].y = Msg.pt.y - StitchWindowOrigin.y;
				dulin();
				return 1;
			}
			if (StateMap.test(StateFlag::INSRT)) {
				if (thred::px2stch())
					thred::ritfcor(SelectedPoint);
				if (StateMap.testAndSet(StateFlag::VCAPT))
					SetCapture(ThrEdWindow);
				if (StateMap.test(StateFlag::LIN1)) {
					if (PCSHeader.stitchCount) {
						xlin1();
						InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
						InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
						StateMap.set(StateFlag::ILIN1);
						ilin1();
					}
					return 1;
				}
				xlin();
				InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				StateMap.set(StateFlag::ILIN);
				ilin();
				return 1;
			}
			if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT))
				SetCapture(ThrEdWindow);
			if (StateMap.test(StateFlag::BZUMIN) && (Msg.wParam & MK_LBUTTON)) {
				if (StateMap.testAndSet(StateFlag::VCAPT))
					SetCapture(ThrEdWindow);
				thred::unbBox();
				ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
				ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
				StateMap.set(StateFlag::BZUM);
				thred::bBox();
				return 1;
			}
		}
		else {
			if (StateMap.testAndReset(StateFlag::VCAPT))
				ReleaseCapture();
		}
		return 1;
	}
	if (Msg.message == WM_LBUTTONUP) {
		if (GetKeyState(VK_SHIFT) & 0X8000 && thred::px2stch()) {
			texture::setshft();
			return 1;
		}
		if (StateMap.test(StateFlag::TXTRED)) {
			texture::txtrup();
			return 1;
		}
		ReleaseCapture();
		movchk();
		if (StateMap.testAndReset(StateFlag::MOVFRMS)) {
			thred::savdo();
			point.x = (Msg.pt.x - FormMoveDelta.x - StitchWindowOrigin.x) - SelectedFormsRect.left;
			point.y = (Msg.pt.y - FormMoveDelta.y - StitchWindowOrigin.y) - SelectedFormsRect.top;
			form::ratsr();
			FormMoveDelta.x = point.x / HorizontalRatio;
			FormMoveDelta.y = point.y / VerticalRatio;
			if (StateMap.test(StateFlag::FPSEL)) {
				form::fvars(ClosestFormToCursor);
				iSelectedVertex = SelectedFormVertices.start;
				for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
					CurrentFormVertices[iSelectedVertex].x += FormMoveDelta.x;
					CurrentFormVertices[iSelectedVertex].y -= FormMoveDelta.y;
					iSelectedVertex = form::pdir(iSelectedVertex);
				}
				thred::setpsel();
				form::frmout(ClosestFormToCursor);
				form::refil();
				StateMap.set(StateFlag::RESTCH);
			}
			else {
				if (StateMap.test(StateFlag::BIGBOX)) {
					thred::savdo();
					for (size_t iForm = 0; iForm < FormIndex; iForm++)
						form::frmadj(iForm);
					for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
						StitchBuffer[iStitch].x += FormMoveDelta.x;
						StitchBuffer[iStitch].y -= FormMoveDelta.y;
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
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::EXPAND)) {
			form::setexpand(xyRatio);
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::STRTCH)) {
			form::setstrtch();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::FRMOV)) {
			thred::savdo();
			form::rstfrm();
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::FRMPMOV)) {
			thred::savdo();
			form::setfpnt();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::MOVCNTR)) {
			StateMap.set(StateFlag::ROTAT);
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::ROTCAPT)) {
			thred::rotfn(rotationAngle, rotationCenter);
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::SELPNT)) {
			thred::savdo();
			ReleaseCapture();
			unsel();
			adjustedPoint.x = (StitchRangeRect.left + SelectBoxOffset.x) - SelectedPoint.x;
			adjustedPoint.y = (StitchRangeRect.bottom + SelectBoxOffset.y) - SelectedPoint.y;
			for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
				StitchBuffer[iStitch].x -= adjustedPoint.x;
				StitchBuffer[iStitch].y -= adjustedPoint.y;
			}
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::CAPT)) {
			unlin();
			ReleaseCapture();
			StateMap.reset(StateFlag::CAPT);
			thred::px2stch();
			StitchBuffer[ClosestPointIndex].x = SelectedPoint.x;
			StitchBuffer[ClosestPointIndex].y = SelectedPoint.y;
			StitchBuffer[ClosestPointIndex].attribute |= USMSK;
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
			return 1;
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
					for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
						if (inrng(iStitch)) {
							ClosestPointIndex = iStitch;
							StateMap.set(StateFlag::SELBOX);
							break;
						}
					}
				}
				if (StateMap.testAndReset(StateFlag::NOSEL)) {
					SelectedFormList->clear();
					// We potentially reserve too much memory, but the cost of reallocatioon is higher than the small
					// amount overallocated
					SelectedFormList->reserve(FormIndex);
					StateMap.reset(StateFlag::FORMSEL);
					for (size_t iForm = 0; iForm < FormIndex; iForm++) {
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
						return 1;
					}
					if (SelectedFormList->size()) {
						gotbox();
						return 1;
					}
					StateMap.reset(StateFlag::BZUMIN);
					StateMap.reset(StateFlag::BOXSLCT);
					StateMap.reset(StateFlag::FRMPSEL);
					StateMap.set(StateFlag::RESTCH);
				}
				if (StateMap.testAndReset(StateFlag::SELBOX)) {
					if (inrng(ClosestPointIndex)) {
						GroupStitchIndex = ClosestPointIndex + 1;
						ClosestPointIndex--;
						while (inrng(ClosestPointIndex))
							ClosestPointIndex--;
						ClosestPointIndex++;
						while (inrng(GroupStitchIndex))
							GroupStitchIndex++;
						GroupStitchIndex--;
						StateMap.set(StateFlag::GRPSEL);
						gotbox();
						return 1;
					}
				}
				if (!StateMap.test(StateFlag::INSRT))
					rngal();
				//				gotbox();
				return 1;
			}
			else {
				if (SelectedPoint.x < ZoomBoxOrigin.x) {
					swapCoordinate  = ZoomBoxOrigin.x;
					ZoomBoxOrigin.x = SelectedPoint.x;
					SelectedPoint.x = swapCoordinate;
				}
				if (SelectedPoint.y < ZoomBoxOrigin.y) {
					swapCoordinate  = ZoomBoxOrigin.y;
					ZoomBoxOrigin.y = SelectedPoint.y;
					SelectedPoint.y = swapCoordinate;
				}
				newSize.x       = SelectedPoint.x - ZoomBoxOrigin.x;
				newSize.y       = SelectedPoint.y - ZoomBoxOrigin.y;
				SelectedPoint.x = ZoomBoxOrigin.x + newSize.x / 2;
				SelectedPoint.y = ZoomBoxOrigin.y + newSize.y / 2;
				swapFactor      = ZoomFactor;
				if (newSize.x > newSize.y) {
					newSize.y  = newSize.x / StitchWindowAspectRatio;
					ZoomFactor = newSize.x / UnzoomedRect.x;
				}
				else {
					newSize.x  = newSize.y * StitchWindowAspectRatio;
					ZoomFactor = newSize.y / UnzoomedRect.x;
				}
				if (ZoomFactor < ZoomMin) {
					ZoomFactor = swapFactor;
					zumin();
					return 1;
				}
				ZoomRect.left = ZoomRect.bottom = 0;
				ZoomRect.right                  = newSize.x;
				ZoomRect.top                    = newSize.y;
				thred::shft(SelectedPoint);
				StateMap.reset(StateFlag::BZUMIN);
				StateMap.set(StateFlag::RESTCH);
				if (!StateMap.testAndSet(StateFlag::ZUMED))
					thred::movStch();
				return 1;
			}
			//			StateMap.reset(StateFlag::BZUMIN);
		}
	}
	if (Msg.message == WM_RBUTTONDOWN || Msg.message == WM_LBUTTONDOWN) {
		if (StateMap.testAndReset(StateFlag::THUMON)) {
			if (chkok()) {
				thred::save();
				thumbak();
				thred::unmsg();
				return 1;
			}
			if (chkwnd(DiscardButton)) {
				thumbak();
				thred::unmsg();
				return 1;
			}
			StateMap.set(StateFlag::BAKSHO);
			thred::unbsho();
			StateMap.reset(StateFlag::THUMSHO);
			thred::unmsg();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::BAKSHO)) {
			if (Msg.message == WM_RBUTTONDOWN)
				StateMap.set(StateFlag::RBUT);
			else
				StateMap.reset(StateFlag::RBUT);
			for (iVersion = 0; iVersion < OLDVER; iVersion++) {
				if (Msg.hwnd == BackupViewer[iVersion]) {
					FileVersionIndex = iVersion;
					if (StateMap.test(StateFlag::THUMSHO)) {
						if (savcmp())
							thumbak();
						else {
							if (StateMap.test(StateFlag::RBUT))
								thumbak();
							else {
								StateMap.set(StateFlag::THUMON);
								displayText::savdisc();
							}
							return 1;
						}
					}
					else
						rebak();
					rstAll();
					StateMap.set(StateFlag::RESTCH);
					return 1;
				}
			}
		}
		if (Msg.pt.x >= ColorBarRect.left && Msg.pt.x <= ColorBarRect.right && Msg.pt.y >= ColorBarRect.top
		    && Msg.pt.y <= ColorBarRect.bottom) {
			unpat();
			colorBarPosition = static_cast<double>(Msg.pt.y - ColorBarRect.top) / (ColorBarRect.bottom - ColorBarRect.top);
			if (Msg.message == WM_RBUTTONDOWN) {
				if (Msg.wParam & MK_SHIFT && (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::GRPSEL))) {
					unbox();
					GroupStitchIndex = colorBarPosition * PCSHeader.stitchCount;
					StateMap.set(StateFlag::GRPSEL);
					thred::grpAdj();
					nuAct(GroupStitchIndex);
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					ClosestPointIndex = colorBarPosition * PCSHeader.stitchCount;
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
				ClosestPointIndex = colorBarPosition * PCSHeader.stitchCount;
				nuAct(ClosestPointIndex);
				rstAll();
				StateMap.set(StateFlag::SELBOX);
				selCol();
			}
			thred::redraw(ColorBar);
			return 1;
		}
	}
	if (Msg.message == WM_RBUTTONDOWN) {
		if (StateMap.test(StateFlag::TXTRED) && !MsgWindow) {
			texture::txtrbut();
			return 1;
		}
		if (GetKeyState(VK_SHIFT) & 0X8000) {
			if (form::closfrm()) {
				if ((StateMap.test(StateFlag::FRMPSEL) || StateMap.test(StateFlag::FPSEL))
				    && SelectedFormVertices.form == ClosestFormToCursor) {
					StateMap.reset(StateFlag::FRMPSEL);
					StateMap.set(StateFlag::FPSEL);
					SelectedFormVertices.finish = ClosestVertexToCursor;
					selectedVertexCount = (SelectedFormVertices.finish - SelectedFormVertices.start + VertexCount) % VertexCount;
					if (selectedVertexCount<VertexCount>> 1) {
						SelectedFormVertices.vertexCount = selectedVertexCount;
						StateMap.set(StateFlag::PSELDIR);
					}
					else {
						SelectedFormVertices.vertexCount = VertexCount - selectedVertexCount;
						StateMap.reset(StateFlag::PSELDIR);
					}
					thred::setpsel();
					return 1;
				}
				StateMap.reset(StateFlag::FPSEL);
				StateMap.set(StateFlag::FRMPSEL);
				SelectedFormVertices.start = ClosestVertexToCursor;
				SelectedFormVertices.form  = ClosestFormToCursor;
				selfpnt();
				return 1;
			}
		}
		if (StateMap.test(StateFlag::WASTRAC)) {
			trace::wasTrace1();
			return 1;
		}
		thred::unmsg();
		unboxs();
		unpat();
		// BufferIndex = 0;
		StateMap.reset(StateFlag::SATIN);
		if (StateMap.testAndReset(StateFlag::SATPNT))
			satin::satfix();
		if (StateMap.test(StateFlag::BAKSHO)) {
			thred::unbsho();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::BIGBOX))
			StateMap.set(StateFlag::RESTCH);
		if (StateMap.test(StateFlag::PRFACT)) {
			if (StateMap.testAndReset(StateFlag::HUPMSG))
				thred::unsid();
			else {
				if (PreferenceIndex) {
					chknum();
					thred::unsid();
					PreferenceIndex = 0;
				}
				else {
					DestroyWindow(PreferencesWindow);
					StateMap.reset(StateFlag::PRFACT);
					if (StateMap.testAndReset(StateFlag::WASRT))
						StateMap.set(StateFlag::INSRT);
				}
			}
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		else {
			if (FormMenuChoice) {
				chknum();
				FormMenuChoice = 0;
				StateMap.set(StateFlag::RESTCH);
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::INSFRM)) {
			insadj();
			StateMap.reset(StateFlag::SHOINSF);
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		if (thred::px2stch() && !(StateMap.test(StateFlag::SIZSEL) && thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
			if (FormIndex && !StateMap.test(StateFlag::FRMOF)) {
				if (Msg.wParam & MK_SHIFT) {
					TmpFormIndex = ClosestFormToCursor;
					if (form::closfrm()) {
						// ToDo - I don't think this can ever be hit with closfrm
						if (SelectedFormList->size()) {
							nuslst(ClosestFormToCursor);
							StateMap.set(StateFlag::RESTCH);
							return 1;
						}
						else {
							if (StateMap.testAndReset(StateFlag::FORMSEL) && TmpFormIndex != ClosestFormToCursor) {
								if (TmpFormIndex > ClosestFormToCursor) {
									std::swap(ClosestFormToCursor, TmpFormIndex);
								}
								for (auto iForm = TmpFormIndex; iForm <= ClosestFormToCursor; iForm++) {
									SelectedFormList->push_back(iForm);
								}
								StateMap.set(StateFlag::RESTCH);
								return 1;
							}
							else {
								form::nufsel();
							}
						}
					}
				}
				if (Msg.wParam & MK_CONTROL) {
					if (!SelectedFormList->size() && StateMap.test(StateFlag::FORMSEL)) {
						StateMap.set(StateFlag::WASEL);
						PreviousFormIndex = ClosestFormToCursor;
					}
					if (form::closfrm())
						form::nufsel();
					if (SelectedFormList->size() > 1) {
						const std::wstring blank(L"");
						displayText::butxt(HNUM, blank);
					}
					return 1;
				}
				if (StateMap.test(StateFlag::FORMSEL)) {
					do {
						if (FormMenuChoice) {
							chknum();
							FormMenuChoice = 0;
							break;
						}
						if (FormDataSheet) {
							thred::undat();
							thred::unsid();
							FormMenuChoice = 0;
							StateMap.set(StateFlag::RESTCH);
							break;
						}
						form::ritfrct(ClosestFormToCursor, StitchWindowDC);
						lenCalc();
						if (!StateMap.testAndReset(StateFlag::ENTROT))
							StateMap.reset(StateFlag::FORMSEL);
						StateMap.set(StateFlag::DUMEN);
						if (!closPnt1(&ClosestPointIndex))
							unbox();
					} while (false);
				}
				if (form::closfrm()) {
					StateMap.set(StateFlag::FORMSEL);
					StateMap.reset(StateFlag::FPSEL);
					form::unpsel();
					form::fvars(ClosestFormToCursor);
					form::ritfrct(ClosestFormToCursor, StitchWindowDC);
					if (StateMap.testAndReset(StateFlag::FRMPSEL) || SelectedFormList->size()) {
						StateMap.set(StateFlag::RESTCH);
						SelectedFormList->clear();
					}
					displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
					code = (FormList[ClosestFormToCursor].attribute & FRMLMSK) >> 1;
					lenCalc();
					unrot();
					return 1;
				}
				if (SelectedFormList->size()) {
					SelectedFormList->clear();
					StateMap.set(StateFlag::RESTCH);
				}
				if (StateMap.testAndReset(StateFlag::FRMPSEL))
					StateMap.set(StateFlag::RESTCH);
			}
			if (StateMap.test(StateFlag::INIT) || FileHandle) {
				if (Msg.wParam & MK_SHIFT) {
					if (StateMap.test(StateFlag::SELBOX)) {
						code = ClosestPointIndex;
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
						return 1;
					}
					if (StateMap.test(StateFlag::GRPSEL)) {
						code = ClosestPointIndex;
						closPnt1(&ClosestPointIndex);
						GroupStitchIndex  = ClosestPointIndex;
						ClosestPointIndex = code;
						thred::grpAdj();
						nuAct(GroupStitchIndex);
						thred::redraw(ColorBar);
						return 1;
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
						if (StateMap.testAndReset(StateFlag::GRPSEL))
							StateMap.set(StateFlag::RESTCH);
						if (PCSHeader.stitchCount)
							rebox();
					}
				}
				xt::clrstch();
				StateMap.set(StateFlag::RESTCH);
			}
			return 1;
		}
		if (Msg.pt.x >= MinLenRect.left && Msg.pt.x <= MinLenRect.right && Msg.pt.y > MinLenRect.top
		    && Msg.pt.y <= MinLenRect.bottom) {
			srchk();
			setsrch(SmallestStitchIndex);
			lensadj();
			StateMap.set(StateFlag::GRPSEL);
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		if (Msg.pt.x >= MaxLenRect.left && Msg.pt.x <= MaxLenRect.right && Msg.pt.y > MaxLenRect.top
		    && Msg.pt.y <= MaxLenRect.bottom) {
			srchk();
			setsrch(LargestStitchIndex);
			lensadj();
			StateMap.set(StateFlag::GRPSEL);
			StateMap.set(StateFlag::RESTCH);
		}
		return 1;
	}
	if (Msg.message == WM_LBUTTONDOWN) {
		if (GetKeyState(VK_SHIFT) & 0X8000 && thred::px2stch()) {
			xt::dushft();
			return 1;
		}
		if (StateMap.test(StateFlag::TXTRED) && !StateMap.test(StateFlag::FORMIN)) {
			texture::txtlbut();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::FSETFCOL)) {
			thred::unsid();
			thred::unmsg();
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
				xt::dufcol(VerticalIndex + 1);
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::FSETBCOL)) {
			thred::unsid();
			thred::unmsg();
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
				xt::dubcol(VerticalIndex + 1);
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::FPUNCLP)) {
			thred::savdo();
			fixpclp();
			form::frmout(ClosestFormToCursor);
			return 1;
		}
		if (StateMap.test(StateFlag::FPSEL) && !StateMap.test(StateFlag::FUNCLP) && !StateMap.test(StateFlag::ROTAT)) {
			*SelectedFormsLine = *SelectedPointsLine;
			SelectedFormsRect  = SelectedPixelsRect;
			if (chkbig(stretchBoxLine, xyRatio))
				return 1;
		}
		if (StateMap.test(StateFlag::WASTRAC)) {
			trace::tracpar();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::HUPMSG)) {
			for (iHoop = 0; iHoop < HUPS; iHoop++) {
				if (Msg.hwnd == SideWindow[iHoop]) {
					switch (iHoop + 1) {
					case SETCUST:
						IniFile.customHoopX = IniFile.hoopSizeX;
						IniFile.customHoopY = IniFile.hoopSizeY;
						StateMap.set(StateFlag::MSGOF);
						displayText::shoMsg(
						    fmt::format((*StringTable)[STR_CUSTHUP], IniFile.hoopSizeX / PFGRAN, IniFile.hoopSizeY / PFGRAN));
						break;
					case SMALHUP:
						IniFile.hoopSizeX = SHUPX;
						IniFile.hoopSizeY = SHUPY;
						IniFile.hoopType  = SMALHUP;
						break;
					case LARGHUP:
						IniFile.hoopSizeX = LHUPX;
						IniFile.hoopSizeY = LHUPY;
						IniFile.hoopType  = LARGHUP;
						break;
					case HUP100:
						IniFile.hoopSizeX = HUP100XY;
						IniFile.hoopSizeY = HUP100XY;
						IniFile.hoopType  = HUP100;
						break;
					case CUSTHUP:
						IniFile.hoopSizeX = IniFile.customHoopX;
						IniFile.hoopSizeY = IniFile.customHoopY;
						IniFile.hoopType  = CUSTHUP;
						break;
					}
					UnzoomedRect.x = IniFile.hoopSizeX;
					UnzoomedRect.y = IniFile.hoopSizeY;
					form::sethup();
					chkhup();
					break;
				}
			}
			thred::unsid();
			formForms::prfmsg();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::INSFIL)) {
			duinsfil();
			return 1;
		}
		if (StateMap.test(StateFlag::BIGBOX) && chkbig(stretchBoxLine, xyRatio)) {
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::DELSFRMS)) {
			code = 0;
			if (chkok()) {
				thred::savdo();
				StateMap.reset(StateFlag::DELTO);
				code = 1;
			}
			else {
				GetWindowRect(DeleteStitchesDialog, &windowRect);
				if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
				    && Msg.pt.y <= windowRect.bottom) {
					thred::savdo();
					StateMap.set(StateFlag::DELTO);
					code = 1;
				}
			}
			delsfrms(code);
			thred::unmsg();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::MOVMSG)) {
			if (chkok()) {
				thred::savdo();
				form::refilfn();
			}
			thred::unmsg();
			if (StateMap.testAndReset(StateFlag::WASFRMFRM))
				formForms::refrm();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::FUNSCLP)) {
			thred::savdo();
			StateMap.reset(StateFlag::MOVFRMS);
			form::pxrct2stch(SelectedFormsRect, formsRect);
			thred::px2stch();
			FormMoveDelta.x = SelectedPoint.x - ((formsRect.right - formsRect.left) / 2 + formsRect.left);
			FormMoveDelta.y = SelectedPoint.y - ((formsRect.top - formsRect.bottom) / 2 + formsRect.bottom);
			for (size_t iForm = 0; iForm < ClipFormsCount; iForm++) {
				ClosestFormToCursor = FormIndex + iForm;
				form::fvars(ClosestFormToCursor);
				for (size_t iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
					SelectedForm->vertices[iVertex].x += FormMoveDelta.x;
					SelectedForm->vertices[iVertex].y += FormMoveDelta.y;
				}
				form::frmout(ClosestFormToCursor);
				form::refil();
			}
			FormIndex += ClipFormsCount;
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		if (SelectedFormList->size() && !StateMap.test(StateFlag::ROTAT) && chkbig(stretchBoxLine, xyRatio)) {
			return 1;
		}
		if (StateMap.test(StateFlag::SIDCOL) && thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
			do {
				thred::savdo();
				if (StateMap.testAndReset(StateFlag::FSETFCOL)) {
					xt::dufcol(VerticalIndex + 1);
					return 1;
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
					SelectedForm->underlayColor = VerticalIndex & 0xf;
					form::refilfn();
					thred::coltab();
					break;
				}
				if (StateMap.testAndReset(StateFlag::FSETUCOL)) {
					xt::dundcol(VerticalIndex + 1);
					return 1;
				}
				form::nufilcol(VerticalIndex);
			} while (false);
			MsgBuffer[0] = gsl::narrow<char>(VerticalIndex) + 0x30;
			MsgBuffer[1] = 0;
			SetWindowText((*ValueWindow)[LBRDCOL], MsgBuffer);
			thred::unsid();
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
			StateMap.reset(StateFlag::SIDCOL);
			FormMenuChoice = 0;
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::OSAV)) {
			if (chkok()) {
				thred::save();
				nuFil();
				thred::unmsg();
				return 1;
			}
			if (chkwnd(DiscardButton)) {
				nuFil();
				thred::unmsg();
				return 1;
			}
			thred::unmsg();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::FCLOS)) {
			if (chkok())
				thred::save();
			else {
				if (!chkwnd(CancelButton))
					closfn();
			}
			thred::unmsg();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::SAVEX)) {
			if (chkok()) {
				thred::save();
				reldun();
			}
			if (chkwnd(DiscardButton))
				reldun();
			thred::unmsg();
			return 1;
		}
		if (StateMap.test(StateFlag::PRFACT)) {
			chknum();
			if (Msg.hwnd == (*ValueWindow)[PSQR]) {
				if (UserFlagMap.testAndFlip(UserFlag::SQRFIL))
					SetWindowText((*ValueWindow)[PSQR], (*StringTable)[STR_PNTD].c_str());
				else
					SetWindowText((*ValueWindow)[PSQR], (*StringTable)[STR_SQR].c_str());
				return 1;
			}
			if (Msg.hwnd == (*ValueWindow)[PBLNT]) {
				if (UserFlagMap.testAndFlip(UserFlag::BLUNT))
					SetWindowText((*ValueWindow)[PBLNT], (*StringTable)[STR_TAPR].c_str());
				else
					SetWindowText((*ValueWindow)[PBLNT], (*StringTable)[STR_BLUNT].c_str());
				return 1;
			}
			if (Msg.hwnd == (*ValueWindow)[PUND]) {
				if (UserFlagMap.testAndFlip(UserFlag::DUND))
					SetWindowText((*ValueWindow)[PUND], (*StringTable)[STR_OFF].c_str());
				else
					SetWindowText((*ValueWindow)[PUND], (*StringTable)[STR_ON].c_str());
				return 1;
			}
			if (Msg.hwnd == (*ValueWindow)[PHUP]) {
				sidhup();
				return 1;
			}
			for (iPreference = 0; iPreference < PRFLINS; iPreference++) {
				if (Msg.hwnd == (*ValueWindow)[iPreference]) {
					PreferenceIndex = iPreference + 1;
					formForms::prfsid((*ValueWindow)[iPreference]);
					break;
				}
			}
			return 1;
		}
		if (!StateMap.test(StateFlag::ROTAT) && StateMap.test(StateFlag::GRPSEL)) {
			auto controlPoint = *FormControlPoints;
			if (iselpnt()) {
				for (size_t iSide = 0; iSide < 4; iSide++) {
					stretchBoxLine[iSide] = controlPoint[iSide << 1];
				}
				stretchBoxLine[4] = stretchBoxLine[0];
				if (SelectedFormControlVertex & 1)
					StateMap.set(StateFlag::STRTCH);
				else {
					StateMap.set(StateFlag::EXPAND);
					xyRatio = static_cast<double>(StitchRangeRect.right - StitchRangeRect.left)
					          / (StitchRangeRect.top - StitchRangeRect.bottom);
				}
				SelectedFormControlVertex >>= 1;
				StateMap.set(StateFlag::SHOSTRTCH);
				thred::strtchbox(stretchBoxLine);
				return 1;
			}
			else {
				StitchCoordinatesPixels.x = Msg.pt.x - StitchWindowOrigin.x;
				StitchCoordinatesPixels.y = Msg.pt.y - StitchWindowOrigin.y;
				if (StitchCoordinatesPixels.x >= controlPoint[0].x && StitchCoordinatesPixels.x <= controlPoint[2].x
				    && StitchCoordinatesPixels.y >= controlPoint[0].y && StitchCoordinatesPixels.y <= controlPoint[4].y) {
					duSelbox();
					StateMap.set(StateFlag::SELPNT);
					SetCapture(ThrEdWindow);
					StateMap.set(StateFlag::VCAPT);
					rSelbox();
					return 1;
				}
			}
		}
		if (StateMap.testAndReset(StateFlag::NEWBAK)) {
			if (chkok()) {
				thred::unmsg();
				thred::save();
				newFil();
				return 1;
			}
			if (chkwnd(DiscardButton)) {
				thred::unmsg();
				newFil();
				return 1;
			}
			thred::unmsg();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::PRGMSG)) {
			if (chkok()) {
				deldir();
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::DUBAK)) {
			if (StateMap.test(StateFlag::THUMSHO)) {
				if (chkok())
					thrsav();
				getbak();
				StateMap.reset(StateFlag::THUMSHO);
				return 1;
			}
			else {
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
					case 3:
						movbak('1', '2');
					case 2:
						movbak('0', '1');
					case 1:
						movbak('r', '0');
					}
					movbak('t', 'r');
					thrBack = 't';
					ThrName->replace_extension(thrExt);
					fs::remove(*ThrName);
					thrBack = 'r';
					ThrName->replace_extension(thrExt);
					FileHandle
					    = CreateFile(ThrName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, NULL, OPEN_EXISTING, 0, NULL);
					if (FileHandle == INVALID_HANDLE_VALUE)
						FileHandle = 0;
					return 1;
				}
				GetWindowRect(CancelButton, &windowRect);
				if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
				    && Msg.pt.y <= windowRect.bottom) {
					getbak();
					return 1;
				}
			}
		}
		if (StateMap.test(StateFlag::DELFRM)) {
			code = 0;
			if (chkok()) {
				thred::savdo();
				StateMap.reset(StateFlag::DELTO);
				code = 1;
			}
			else {
				GetWindowRect(DeleteStitchesDialog, &windowRect);
				if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
				    && Msg.pt.y <= windowRect.bottom) {
					thred::savdo();
					StateMap.set(StateFlag::DELTO);
					code = 1;
				}
			}
			if (code) {
				thred::frmdel();
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
			}
			thred::unmsg();
			return 1;
		}
		if (StateMap.test(StateFlag::FILMSG)) {
			if (chkok()) {
				thred::savdo();
				form::unfil();
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
				thred::unmsg();
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::SIZSEL)) {
			if (thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2])) {
				VerticalIndex -= 13;
				ThreadSize[ThreadSizeSelected][0]   = threadSizeMap[VerticalIndex];
				ThreadSizeIndex[ThreadSizeSelected] = VerticalIndex;
				wcsncpy_s(buffer, ThreadSize[ThreadSizeSelected], 2);
				buffer[2] = 0;
				SetWindowText(ThreadSizeWin[ThreadSizeSelected], buffer);
				StateMap.set(StateFlag::RESTCH);
				for (iWindow = 0; iWindow < 3; iWindow++) {
					if (ChangeThreadSizeWin[iWindow]) {
						DestroyWindow(ChangeThreadSizeWin[iWindow]);
					}
				}
				return 1;
			}
			else {
				for (iWindow = 0; iWindow < 3; iWindow++) {
					if (ChangeThreadSizeWin[iWindow]) {
						DestroyWindow(ChangeThreadSizeWin[iWindow]);
					}
				}
			}
		}
		if (StateMap.testAndReset(StateFlag::POLIMOV)) {
			thred::savdo();
			form::setfrm();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::FORMIN)) {
			GetWindowRect(MsgWindow, &windowRect);
			if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
			    && Msg.pt.y <= windowRect.bottom) {
				iFillType = (Msg.pt.y - windowRect.top - 1) / (ButtonHeight - 4);
				if (StateMap.testAndReset(StateFlag::FENDIN)) {
					if (iFillType == 3)
						UserFlagMap.reset(UserFlag::SQRFIL);
					if (iFillType == 4)
						UserFlagMap.set(UserFlag::SQRFIL);
				}
				else
					form::duform(iFillType);
				switch (iFillType) {
				case FRMRPOLY - 1:
				case FRMSTAR - 1:
				case FRMSPIRAL - 1:
				case FRMHEART - 2:
				case FRMLENS - 2:
				case FRMEGG - 2:
				case FRMZIGZAG - 2:
					return 1;
				}
			}
			thred::unmsg();
			return 1;
		}
		if (PreferenceIndex == PAP + 1 && thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
			AppliqueColor = VerticalIndex;
			SetWindowText((*ValueWindow)[PAP], fmt::format(L"{}", VerticalIndex).c_str());
			thred::unsid();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::SIDACT)) {
			thred::savdo();
			if (FormMenuChoice == LFTHTYP) {
				for (iFillType = 0; iFillType < 6; iFillType++) {
					if (Msg.hwnd == SideWindow[iFillType]) {
						SelectedForm->fillInfo.feather.fillType = iFillType + 1;
						form::refil();
						formForms::refrm();
						break;
					}
				}
				return 1;
			}
			if (FormMenuChoice == LLAYR) {
				for (iLayer = 0; iLayer < 5; iLayer++) {
					if (Msg.hwnd == SideWindow[iLayer])
						break;
				}
				if (iLayer < 5) {
					form::movlayr(iLayer << 1);
					StateMap.set(StateFlag::FORMSEL);
				}
				return 1;
			}
			SelectedForm->borderColor &= COLMSK;
			if (StateMap.testAndReset(StateFlag::BRDACT)) {
				if (clip::iseclp(ClosestFormToCursor))
					clip::deleclp(ClosestFormToCursor);
				do {
					if (Msg.hwnd == SideWindow[0]) {
						SelectedForm->edgeType = 0;
						form::refilfn();
						thred::coltab();
						thred::unsid();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[1]) {
						if (SelectedForm->edgeType) {
							code = SelectedForm->edgeType & NEGUND;
							if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL)
								form::bsizpar();
							SelectedForm->edgeType = EDGELINE;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::bord();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[2]) {
						if (SelectedForm->edgeType) {
							code = SelectedForm->edgeType & NEGUND;
							if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL)
								form::bsizpar();
							SelectedForm->edgeType = EDGEBEAN;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::dubold();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[3]) {
						thred::unmsg();
						thred::unsid();
						form::fclp();
						return 1;
					}
					if (Msg.hwnd == SideWindow[4]) {
						if (SelectedForm->edgeType) {
							switch (SelectedForm->edgeType) {
							case EDGECLIP:
								form::bsizpar();
							case EDGELINE:
							case EDGEBEAN:
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								break;
							case EDGEPROPSAT:
								SelectedForm->edgeSpacing /= 2;
								break;
							}
							SelectedForm->edgeType = EDGEANGSAT;
							if (UserFlagMap.test(UserFlag::DUND))
								SelectedForm->edgeType |= EGUND;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							satin::satbrd();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[5]) {
						if (SelectedForm->fillType) {
							form::delmfil();
							SelectedForm->fillType = 0;
						}
						if (SelectedForm->edgeType) {
							if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN
							    || SelectedForm->edgeType == EDGECLIP) {
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								if (SelectedForm->edgeType == EDGECLIP)
									form::bsizpar();
							}
							SelectedForm->edgeType = EDGEAPPL;
							if (UserFlagMap.test(UserFlag::DUND))
								SelectedForm->edgeType |= EGUND;
							SelectedForm->borderColor |= (AppliqueColor << 4);
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::apliq();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[6]) {
						if (SelectedForm->edgeType) {
							switch (SelectedForm->edgeType) {
							case EDGECLIP:
								form::bsizpar();
							case EDGELINE:
							case EDGEBEAN:
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								break;
							case EDGEANGSAT:
								SelectedForm->edgeSpacing *= 2;
							}
							SelectedForm->edgeType = EDGEPROPSAT;
							if (UserFlagMap.test(UserFlag::DUND))
								SelectedForm->edgeType |= EGUND;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::prpbrd(LineSpacing);
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[7]) {
						if (SelectedForm->edgeType) {
							if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN
							    || SelectedForm->edgeType == EDGECLIP) {
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								if (SelectedForm->edgeType == EDGECLIP)
									form::bsizpar();
							}
							SelectedForm->edgeType = EDGEBHOL;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::bhol();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[8]) {
						if (SelectedForm->edgeType) {
							if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN
							    || SelectedForm->edgeType == EDGECLIP) {
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								if (SelectedForm->edgeType == EDGECLIP)
									form::bsizpar();
							}
							form::picot();
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::picot();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[9]) {
						if (SelectedForm->edgeType) {
							code = SelectedForm->edgeType & NEGUND;
							if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL)
								form::bsizpar();
							SelectedForm->edgeType = EDGEDOUBLE;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::dubsfil();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[10]) {
						StateMap.set(StateFlag::LINCHN);
						thred::unmsg();
						thred::unsid();
						form::chan();
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[11]) {
						StateMap.reset(StateFlag::LINCHN);
						thred::unmsg();
						thred::unsid();
						form::chan();
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[12]) {
						thred::unmsg();
						thred::unsid();
						form::filclpx();
						return 1;
					}
				} while (false);
				formForms::refrm();
				form::refil();
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return 1;
			}
			else {
				if (SelectedForm->fillType == SAT && SelectedForm->satinGuideCount)
					satin::delsac(ClosestFormToCursor);
				if ((SelectedForm->edgeType & NEGUND) == EDGEAPPL) {
					SelectedForm->edgeType = EDGEANGSAT;
					if (UserFlagMap.test(UserFlag::DUND))
						SelectedForm->edgeType |= EGUND;
				}
				do {
					if (Msg.hwnd == SideWindow[0]) { // none
						SelectedForm->type = FRMFPOLY;
						form::delmfil();
						SelectedForm->fillType = 0;
						thred::coltab();
						thred::unsid();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[1]) { // vertical fill
						thred::savdo();
						SelectedForm->type = FRMFPOLY;
						if (SelectedForm->fillType) {
							respac();
							SelectedForm->fillType = VRTF;
							SelectedForm->type     = FRMFPOLY;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::filvrt();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[2]) { // horizontal fill
						SelectedForm->type = FRMFPOLY;
						if (SelectedForm->fillType) {
							respac();
							SelectedForm->fillType = HORF;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::filhor();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[3]) { // angle fill
						SelectedForm->type = FRMFPOLY;
						if (SelectedForm->fillType) {
							respac();
							// ToDo - should we be using the angle information already present
							SelectedForm->fillType              = ANGF;
							SelectedForm->angleOrClipData.angle = IniFile.fillAngle;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::filangl();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[4]) { // fan fill
						SelectedForm->type = SAT;
						if (SelectedForm->fillType) {
							respac();
							SelectedForm->fillType = SATF;
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							StateMap.reset(StateFlag::FTHR);
							form::filsat();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[5]) { // fan clip
						SelectedForm->type = SAT;
						thred::unmsg();
						thred::unsid();
						form::clpfil();
						return 1;
					}
					if (Msg.hwnd == SideWindow[6]) { // contour fill
						if (SelectedForm->fillType) {
							if (SelectedForm->fillType == CLPF)
								SelectedForm->fillSpacing = LineSpacing;
							form::chkcont();
							break;
						}
						else {
							thred::unmsg();
							thred::unsid();
							form::contfil();
							return 1;
						}
					}
					if (Msg.hwnd == SideWindow[7]) { // vertical clip
						thred::unmsg();
						thred::unsid();
						if (sidclp())
							form::vrtsclp();
						StateMap.reset(StateFlag::CLPSHO);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[8]) { // horizontal clip
						thred::unmsg();
						thred::unsid();
						if (sidclp())
							form::horsclp();
						StateMap.reset(StateFlag::CLPSHO);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[9]) { // angle clip
						thred::unmsg();
						thred::unsid();
						if (sidclp())
							form::angsclp();
						StateMap.reset(StateFlag::CLPSHO);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[10]) { // feather fill
						thred::unmsg();
						thred::unsid();
						xt::fethrf();
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					if (Msg.hwnd == SideWindow[11]) // vertical texture
					{
						if (texture::istx(ClosestFormToCursor)) {
							SelectedForm->fillType = TXVRTF;
							break;
						}
						texture::dutxtfil();
					}
					if (Msg.hwnd == SideWindow[12]) // horizontal texture
					{
						if (texture::istx(ClosestFormToCursor)) {
							SelectedForm->fillType = TXHORF;
							break;
						}
						texture::dutxtfil();
					}
					if (Msg.hwnd == SideWindow[13]) // angle texture
					{
						if (texture::istx(ClosestFormToCursor)) {
							SelectedForm->fillType              = TXANGF;
							SelectedForm->angleOrClipData.angle = IniFile.fillAngle;
							break;
						}
						texture::dutxtfil();
					}
				} while (false);
				formForms::refrm();
				form::refil();
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return 1;
			}
		}
		if (FormDataSheet) {
			chknum();
			thred::unsid();
			do {
				if (Msg.hwnd == (*ValueWindow)[LTXOF]) {
					FormMenuChoice = LTXOF;
					formForms::sidwnd((*ValueWindow)[LTXOF]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUSPAC]) {
					FormMenuChoice = LUSPAC;
					formForms::sidwnd((*ValueWindow)[LUSPAC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUANG]) {
					FormMenuChoice = LUANG;
					formForms::sidwnd((*ValueWindow)[LUANG]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LULEN]) {
					FormMenuChoice = LULEN;
					formForms::sidwnd((*ValueWindow)[LULEN]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LWLKIND]) {
					FormMenuChoice = LWLKIND;
					formForms::sidwnd((*ValueWindow)[LWLKIND]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHSIZ]) {
					FormMenuChoice = LFTHSIZ;
					formForms::sidwnd((*ValueWindow)[LFTHSIZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHNUM]) {
					FormMenuChoice = LFTHNUM;
					formForms::sidwnd((*ValueWindow)[LFTHNUM]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHFLR]) {
					FormMenuChoice = LFTHFLR;
					formForms::sidwnd((*ValueWindow)[LFTHFLR]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHUPCNT]) {
					FormMenuChoice = LFTHUPCNT;
					formForms::sidwnd((*ValueWindow)[LFTHUPCNT]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHDWNCNT]) {
					FormMenuChoice = LFTHDWNCNT;
					formForms::sidwnd((*ValueWindow)[LFTHDWNCNT]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHBLND]) {
					StateMap.set(StateFlag::FLPBLND);
					formForms::refrm();
					form::refil();
					thred::unsid();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHUP]) {
					SelectedForm->extendedAttribute ^= AT_FTHUP;
					formForms::refrm();
					form::refil();
					thred::unsid();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHBTH]) {
					SelectedForm->extendedAttribute ^= AT_FTHBTH;
					formForms::refrm();
					form::refil();
					thred::unsid();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHTYP]) {
					FormMenuChoice = LFTHTYP;
					sidmsg((*ValueWindow)[LFTHTYP], &(*StringTable)[STR_FTH0], 6);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRM]) {
					thred::savdo();
					form::unfil();
					if (SelectedForm->type == FRMLINE)
						SelectedForm->type = FRMFPOLY;
					else
						SelectedForm->type = FRMLINE;
					thred::coltab();
					satin::delsac(ClosestFormToCursor);
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LLAYR]) {
					std::wstring LayerText[] = { L"0", L"1", L"2", L"3", L"4" };
					FormMenuChoice           = LLAYR;
					StateMap.reset(StateFlag::FILTYP);
					sidmsg((*ValueWindow)[LLAYR], LayerText, 5);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMFIL]) {
					StateMap.reset(StateFlag::FILTYP);
					FormMenuChoice = LFRMFIL;
					sidmsg((*ValueWindow)[LFRMFIL], &(*StringTable)[STR_FIL0], 14);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMCOL]) {
					FormMenuChoice = LFRMCOL;
					formForms::sidwnd((*ValueWindow)[LFRMCOL]);
					StateMap.set(StateFlag::SIDCOL);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUNDCOL]) {
					FormMenuChoice = LUNDCOL;
					formForms::sidwnd((*ValueWindow)[LUNDCOL]);
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::UNDCOL);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHCOL]) {
					FormMenuChoice = LFTHCOL;
					formForms::sidwnd((*ValueWindow)[LFTHCOL]);
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::FTHSID);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMSPAC]) {
					FormMenuChoice = LFRMSPAC;
					formForms::sidwnd((*ValueWindow)[LFRMSPAC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMLEN]) {
					FormMenuChoice = LFRMLEN;
					formForms::sidwnd((*ValueWindow)[LFRMLEN]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRD]) {
					StateMap.set(StateFlag::FILTYP);
					sidmsg((*ValueWindow)[LBRD], &(*StringTable)[STR_EDG0], EDGETYPS + 1);
					StateMap.set(StateFlag::BRDACT);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDCOL]) {
					FormMenuChoice = LBRDCOL;
					formForms::sidwnd((*ValueWindow)[LBRDCOL]);
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::BRDSID);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDSPAC]) {
					FormMenuChoice = LBRDSPAC;
					formForms::sidwnd((*ValueWindow)[LBRDSPAC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDLEN]) {
					FormMenuChoice = LBRDLEN;
					formForms::sidwnd((*ValueWindow)[LBRDLEN]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDSIZ]) {
					FormMenuChoice = LBRDSIZ;
					formForms::sidwnd((*ValueWindow)[LBRDSIZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LAPCOL]) {
					FormMenuChoice = LAPCOL;
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::APSID);
					formForms::sidwnd((*ValueWindow)[LAPCOL]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBCSIZ]) {
					FormMenuChoice = LBCSIZ;
					formForms::sidwnd((*ValueWindow)[LBCSIZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBSTRT]) {
					code = SelectedForm->attribute & SBLNT;
					if (code)
						SelectedForm->attribute &= NSBLNT;
					else
						SelectedForm->attribute |= SBLNT;
					form::refil();
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBFIN]) {
					code = SelectedForm->attribute & FBLNT;
					if (code)
						SelectedForm->attribute &= NFBLNT;
					else
						SelectedForm->attribute |= FBLNT;
					form::refil();
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMANG]) {
					FormMenuChoice = LFRMANG;
					formForms::sidwnd((*ValueWindow)[LFRMANG]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDPIC]) {
					FormMenuChoice = LBRDPIC;
					formForms::sidwnd((*ValueWindow)[LBRDPIC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDUND]) {
					SelectedForm->edgeType ^= EGUND;
					form::refil();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LSACANG]) {
					FormMenuChoice = LSACANG;
					formForms::sidwnd((*ValueWindow)[LSACANG]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMFAZ]) {
					FormMenuChoice = LFRMFAZ;
					formForms::sidwnd((*ValueWindow)[LFRMFAZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDPOS]) {
					FormMenuChoice = LBRDPOS;
					formForms::sidwnd((*ValueWindow)[LBRDPOS]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBFILSQR]) {
					xt::dubit(AT_SQR);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFSTRT]) {
					xt::dubit(AT_STRT);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LDSTRT]) {
					FormMenuChoice = LDSTRT;
					formForms::sidwnd((*ValueWindow)[LDSTRT]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFEND]) {
					xt::dubit(AT_END);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LDEND]) {
					FormMenuChoice = LDEND;
					formForms::sidwnd((*ValueWindow)[LDEND]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LWALK]) {
					xt::dubit(AT_WALK);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LCWLK]) {
					xt::dubit(AT_CWLK);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUND]) {
					xt::dubit(AT_UND);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMAXFIL]) {
					FormMenuChoice = LMAXFIL;
					formForms::sidwnd((*ValueWindow)[LMAXFIL]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMINFIL]) {
					FormMenuChoice = LMINFIL;
					formForms::sidwnd((*ValueWindow)[LMINFIL]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMAXBRD]) {
					FormMenuChoice = LMAXBRD;
					formForms::sidwnd((*ValueWindow)[LMAXBRD]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMINBRD]) {
					FormMenuChoice = LMINBRD;
					formForms::sidwnd((*ValueWindow)[LMINBRD]);
					break;
				}
			} while (false);
			return 1;
		}
		if (StateMap.test(StateFlag::INSFRM)) {
			thred::savdo();
			form::setins();
			return 1;
		}
		if (StateMap.test(StateFlag::FUNCLP)) {
			thred::savdo();
			FormIndex++;
			StateMap.set(StateFlag::INIT);
			form::rstfrm();
			form::fvars(ClosestFormToCursor);
			form::refil();
			StateMap.reset(StateFlag::FUNCLP);
			if (StateMap.testAndReset(StateFlag::FPSEL))
				form::frmout(FormIndex - 1);
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		if (StateMap.test(StateFlag::SATCNKT)) {
			thred::savdo();
			satin::satknkt();
			return 1;
		}
		if (StateMap.test(StateFlag::SATPNT)) {
			satin::satpnt1();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::SATIN)) {
			satin::satpnt0();
			return 1;
		}
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (!StateMap.test(StateFlag::FRMROT) && form::chkfrm(stretchBoxLine, xyRatio))
				return 1;
		}
		if (StateMap.testAndReset(StateFlag::MOVFRM)) {
			thred::savdo();
			form::setfrm();
			return 1;
		}
		thred::unmsg();
		if (thred::px2stch() && !(StateMap.test(StateFlag::SIZSEL) && thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
			unpat();
			if (StateMap.testAndReset(StateFlag::ROTAT)) {
				RotateBoxToCursorLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				RotateBoxToCursorLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				adjustedPoint.x            = RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x;
				adjustedPoint.y            = RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y;
				if (hypot(adjustedPoint.x, adjustedPoint.y) < CLOSENUF) {
					thred::px2stch();
					rotationCenter = SelectedPoint;
					StateMap.set(StateFlag::MOVCNTR);
					unrot();
					ritrot(rotationAngle, rotationCenter);
				}
				else {
					if (adjustedPoint.x)
						RotationHandleAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
					else {
						if (adjustedPoint.y > 0)
							RotationHandleAngle = PI / 2;
						else
							RotationHandleAngle = -PI / 2;
					}
					StateMap.set(StateFlag::ROTUSHO);
					durotu();
					StateMap.set(StateFlag::ROTCAPT);
				}
				return 1;
			}
			if (StateMap.testAndReset(StateFlag::CLPSHO)) {
				thred::savdo();
				if ((StateMap.testAndReset(StateFlag::SELBOX) || StateMap.testAndReset(StateFlag::INSRT))
				    && ClosestPointIndex != gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
					lodclp(ClosestPointIndex);
				else
					lodclp(PCSHeader.stitchCount);
				thred::rngadj();
				thred::selRct(formsRect);
				// ToDo - windowRect should be be formsRect? windowRect is not initialized before use here
				auto& formControls = *FormControlPoints;
				formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = windowRect.left;
				formControls[1].x = formControls[5].x = form::midl(windowRect.right, windowRect.left);
				formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = windowRect.top;
				formControls[3].y = formControls[7].y = form::midl(windowRect.top, windowRect.bottom);
				formControls[4].y = formControls[5].y = formControls[6].y = windowRect.bottom;
				formControls[2].x = formControls[3].x = formControls[4].x = windowRect.right;
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
				return 1;
			}
			if (StateMap.testAndReset(StateFlag::BOXZUM)) {
				StateMap.set(StateFlag::BZUMIN);
				ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
				ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
				thred::px2stch();
				ZoomBoxOrigin = SelectedPoint;
				StateMap.set(StateFlag::VCAPT);
				return 1;
			}
			if (FormIndex && !StateMap.test(StateFlag::FRMOF)) {
				if (!StateMap.test(StateFlag::INSRT)) {
					if (StateMap.testAndReset(StateFlag::FORMSEL))
						form::ritfrct(ClosestFormToCursor, StitchWindowDC);
					if (form::closfrm()) {
						StateMap.set(StateFlag::FRMPMOV);
						form::fvars(ClosestFormToCursor);
						form::frmovlin();
						return 1;
					}
				}
			}
			if (StateMap.test(StateFlag::INIT)) {
				unlin();
				thred::savdo();
				if (StateMap.test(StateFlag::INSRT) && PCSHeader.stitchCount < MAXITEMS) {
					thred::px2stch();
					code = (ActiveColor | USMSK | (ActiveLayer << LAYSHFT) | NOTFRM) & NKNOTMSK;
					if (StateMap.test(StateFlag::LIN1)) {
						if (StateMap.test(StateFlag::BAKEND)) {
							xlin1();
							iStitch                         = PCSHeader.stitchCount;
							StitchBuffer[iStitch].x         = SelectedPoint.x;
							StitchBuffer[iStitch].y         = SelectedPoint.y;
							StitchBuffer[iStitch].attribute = code;
							thred::duzrat();
							stch2px1(iStitch);
							InsertLine[0]   = StitchCoordinatesPixels;
							InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
							InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
							PCSHeader.stitchCount++;
							thred::coltab();
							StateMap.set(StateFlag::RESTCH);
							return 1;
						}
						else {
							xlin1();
							for (iStitch = PCSHeader.stitchCount; iStitch; iStitch--) {
								StitchBuffer[iStitch] = StitchBuffer[iStitch - 1];
							}
							StitchBuffer[0].attribute = code;
							StitchBuffer[iStitch].attribute &= (~KNOTMSK);
							StitchBuffer[0].x = SelectedPoint.x;
							StitchBuffer[0].y = SelectedPoint.y;
							stch2px1(iStitch);
							InsertLine[0]   = StitchCoordinatesPixels;
							InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
							InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
							PCSHeader.stitchCount++;
							thred::coltab();
							StateMap.set(StateFlag::RESTCH);
							return 1;
						}
					}
					else {
						xlin();
						if (StitchBuffer[ClosestPointIndex].attribute & ALTYPMSK
						    && StitchBuffer[ClosestPointIndex + 1].attribute & ALTYPMSK) {
							if ((StitchBuffer[ClosestPointIndex].attribute & FRMSK)
							    == (StitchBuffer[ClosestPointIndex + 1].attribute & FRMSK))
								code = StitchBuffer[ClosestPointIndex].attribute | USMSK;
						}
						iStitch = PCSHeader.stitchCount;
						do {
							StitchBuffer[iStitch] = StitchBuffer[iStitch - 1];
							iStitch--;
						} while (iStitch > ClosestPointIndex);
						PCSHeader.stitchCount++;
						ClosestPointIndex++;
						StitchBuffer[ClosestPointIndex].x         = SelectedPoint.x;
						StitchBuffer[ClosestPointIndex].y         = SelectedPoint.y;
						StitchBuffer[ClosestPointIndex].attribute = code;
						xlin();
						InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
						InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
						stch2px1(ClosestPointIndex);
						InsertLine[0] = StitchCoordinatesPixels;
						stch2px1(ClosestPointIndex + 1);
						InsertLine[2] = StitchCoordinatesPixels;
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
						return 1;
					}
				}
				if ((!StateMap.test(StateFlag::HIDSTCH)) && closPnt1(&ClosestPointIndexClone)) {
					ClosestPointIndex = ClosestPointIndexClone;
					unbox();
					unboxs();
					setbak(ThreadSizePixels[StitchBuffer[ClosestPointIndex].attribute & 0xf] + 3);
					std::vector<POINT> linePoints(3);
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
						if (ClosestPointIndex == gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
							if (ZoomFactor < STCHBOX) {
								SelectObject(StitchWindowDC, LinePen);
								stchbox(PCSHeader.stitchCount - 1, StitchWindowDC);
								stchbox(PCSHeader.stitchCount - 2, StitchWindowDC);
							}
							SetROP2(StitchWindowDC, R2_COPYPEN);
							drwLin(linePoints, PCSHeader.stitchCount - 2, 2, BackgroundPen);
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
					MoveLine0[1].x = MoveLine1[0].x = Msg.pt.x - StitchWindowOrigin.x;
					MoveLine0[1].y = MoveLine1[0].y = Msg.pt.y - StitchWindowOrigin.y;
					if (ClosestPointIndex <= 0)
						StateMap.reset(StateFlag::ISDWN);
					else {
						StateMap.set(StateFlag::ISDWN);
						stch2px1(ClosestPointIndex - 1);
						MoveLine0[0] = StitchCoordinatesPixels;
					}
					iStitch = PCSHeader.stitchCount - 1;
					if (ClosestPointIndex >= iStitch)
						StateMap.reset(StateFlag::ISUP);
					else {
						StateMap.set(StateFlag::ISUP);
						stch2px1(ClosestPointIndex + 1);
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
					PCSHeader.stitchCount           = 1;
					InsertLine[0].x                 = Msg.pt.x - StitchWindowOrigin.x;
					InsertLine[0].y                 = Msg.pt.y - StitchWindowOrigin.y;
					StitchBuffer[0].attribute       = USMSK | ActiveColor | LayerIndex | NOTFRM;
					StitchBuffer[0].x               = SelectedPoint.x;
					StitchBuffer[0].y               = SelectedPoint.y;
					ColorChanges                    = 1;
					ColorChangeTable[0].colorIndex  = ActiveColor;
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
			thred::ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::BOXSLCT);
			StateMap.set(StateFlag::BZUMIN);
			StateMap.set(StateFlag::NOSEL);
			ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
			ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
			ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
			thred::px2stch();
			ZoomBoxOrigin = SelectedPoint;
			StateMap.set(StateFlag::VCAPT);
			return 1;
		}
		if (Msg.hwnd == (*ButtonWin)[HBOXSEL]) {
			form::boxsel();
			return 1;
		}
		if (Msg.hwnd == (*ButtonWin)[HUPTO]) {
			toglup();
			return 1;
		}
		if (Msg.hwnd == (*ButtonWin)[HHID]) {
			toglHid();
			return 1;
		}
		if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
			if (Msg.message == WM_LBUTTONDOWN) {
				thred::savdo();
				code        = ActiveColor;
				ActiveColor = VerticalIndex & 0xf;
				thred::redraw(UserColorWin[code]);
				thred::redraw(UserColorWin[ActiveColor]);
				if (StateMap.test(StateFlag::HID)) {
					StateMap.reset(StateFlag::SELBOX);
					StateMap.reset(StateFlag::GRPSEL);
					StateMap.reset(StateFlag::SCROS);
					StateMap.reset(StateFlag::ECROS);
					StateMap.set(StateFlag::RESTCH);
					thred::redraw((*ButtonWin)[HHID]);
				}
				else {
					if (SelectedFormList->size()) {
						nucols();
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
					else {
						if (StateMap.test(StateFlag::FORMSEL)) {
							SelectedForm = &FormList[ClosestFormToCursor];
							if (SelectedForm->fillType || SelectedForm->edgeType
							    || SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
								if (FormList[ClosestFormToCursor].fillType) {
									FormList[ClosestFormToCursor].fillColor = ActiveColor;
									if (FormList[ClosestFormToCursor].fillType == FTHF)
										FormList[ClosestFormToCursor].fillInfo.feather.color = ActiveColor;
								}
								if (FormList[ClosestFormToCursor].edgeType) {
									if (FormList[ClosestFormToCursor].edgeType == EDGEAPPL) {
										FormList[ClosestFormToCursor].borderColor &= APCOLMSK;
										FormList[ClosestFormToCursor].borderColor |= ActiveColor;
									}
									else
										FormList[ClosestFormToCursor].borderColor = ActiveColor;
								}
								if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK))
									FormList[ClosestFormToCursor].underlayColor = ActiveColor;
								code = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
								for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
									stitchAttribute = StitchBuffer[iStitch].attribute;
									if (stitchAttribute & ALTYPMSK && (stitchAttribute & FRMSK) == code
									    && (stitchAttribute & TYPMSK) != TYPMSK) {
										stitchAttribute &= NCOLMSK;
										stitchAttribute |= ActiveColor;
										StitchBuffer[iStitch].attribute = stitchAttribute;
									}
								}
								thred::coltab();
								StateMap.set(StateFlag::RESTCH);
							}
						}
						else {
							if (StateMap.test(StateFlag::GRPSEL)) {
								for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
									StitchBuffer[iStitch].attribute &= 0xfffffff0;
									StitchBuffer[iStitch].attribute |= ActiveColor;
								}
								thred::coltab();
								StateMap.set(StateFlag::RESTCH);
							}
							else {
								if (StateMap.test(StateFlag::COL))
									StateMap.set(StateFlag::RESTCH);
							}
						}
					}
				}
			}
			return 1;
		}
		if (thi::chkMsgs(Msg.pt, UserColorWin[0], UserColorWin[15])) {
			if (Msg.message == WM_LBUTTONDOWN && nuCol(UserColor[VerticalIndex])) {
				thred::savdo();
				UserColor[VerticalIndex]      = ColorStruct.rgbResult;
				UserPen[VerticalIndex]        = nuPen(UserPen[VerticalIndex], 1, UserColor[VerticalIndex]);
				UserColorBrush[VerticalIndex] = nuBrush(UserColorBrush[VerticalIndex], UserColor[VerticalIndex]);
				thred::redraw(UserColorWin[VerticalIndex]);
				StateMap.set(StateFlag::RESTCH);
			}
			return 1;
		}
		if (thi::chkMsgs(Msg.pt, ThreadSizeWin[0], ThreadSizeWin[15])) {
			if (Msg.message == WM_LBUTTONDOWN) {
				const wchar_t* const str[] = { L"30", L"40", L"60" };

				thred::savdo();
				ThreadSizeSelected = VerticalIndex;
				for (iThreadSize = 0; iThreadSize < 3; iThreadSize++) {
					ChangeThreadSizeWin[iThreadSize] = CreateWindow(L"STATIC",
					                                                str[iThreadSize],
					                                                WS_CHILD | WS_VISIBLE | WS_BORDER,
					                                                ButtonWidthX3,
					                                                ButtonHeight * (iThreadSize + VerticalIndex),
					                                                ButtonWidth,
					                                                ButtonHeight,
					                                                ThrEdWindow,
					                                                NULL,
					                                                ThrEdInstance,
					                                                NULL);
				}
				StateMap.set(StateFlag::SIZSEL);
			}
			return 1;
		}
	}
	switch (Msg.message) {
	case WM_TIMER:
		if (StateMap.test(StateFlag::RUNPAT) && !Msg.wParam)
			stchout();
		break;
	case WM_CHAR:
		if (isgraph(Msg.wParam))
			nuthum(tolower(Msg.wParam));
		break;
	case WM_KEYDOWN:
		code = Msg.wParam & 0xffff;
		if (StateMap.test(StateFlag::TXTRED)) {
			texture::txtkey(code);
			return 1;
		}
		form::fvars(ClosestFormToCursor);

#if LANG == GRM
		if (code >= 0x30 && code <= 0x39) {
			if (GetKeyState(VK_SHIFT) & 0X8000) {
				switch (code) {
				case 0x38:
					selfrm0();
					break;
				case 0x39:
					selfrmx();
					break;
				}
			}
		}
#endif
		// ToDo - value passed to duform is wierd because it is dependant on order of enumeration of the form types.
		//        and value 'SAT' throws it off
		if (StateMap.test(StateFlag::FORMIN)) {
			if (GetKeyState(VK_CONTROL) & 0X8000)
				return 1;

			switch (code) {
			case 'E':
				StateMap.reset(StateFlag::FORMIN);
				thred::unmsg();
				form::duform(FRMLINE - 1);
				return 1;
			case 'F':
				StateMap.reset(StateFlag::FORMIN);
				thred::unmsg();
				form::duform(FRMFPOLY - 1);
				return 1;
			case 'R':
				form::duform(FRMRPOLY - 1);
				return 1;
			case 'S':
				form::duform(FRMSTAR - 1);
				return 1;
			case 'A':
				form::duform(FRMSPIRAL - 1);
				return 1;
			case 'H':
				form::duform(FRMHEART - 2);
				return 1;
			case 'L':
				form::duform(FRMLENS - 2);
				return 1;
			case 'G':
				form::duform(FRMEGG - 2);
				return 1;
			case 'T':
				form::duform(FRMTEAR - 2);
				return 1;
			case 'Z':
				form::duform(FRMZIGZAG - 2);
				return 1;
			case 'W':
				form::duform(FRMWAVE - 2);
				return 1;
			case 'D':
				form::duform(FRMDAISY - 2);
				return 1;
			}
		}
		if (StateMap.test(StateFlag::FILMSG)) {
			if (code == VK_RETURN || code == 0xc0) {
				thred::savdo();
				form::unfil();
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
				thred::unmsg();
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::MOVMSG)) {
			if (code == VK_RETURN || code == 0xc0) {
				thred::savdo();
				form::refilfn();
				thred::unmsg();
			}
			else
				thred::unmsg();
			if (StateMap.testAndReset(StateFlag::WASFRMFRM))
				formForms::refrm();
			return 1;
		}
		if (StateMap.testAndReset(StateFlag::PRGMSG)) {
			if (code == VK_RETURN || code == 0xc0) {
				deldir();
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::DELSFRMS)) {
			if (code == 'S' || code == VK_RETURN || code == 0xc0) {
				thred::savdo();
				if (code == 'S')
					StateMap.set(StateFlag::DELTO);
				else
					StateMap.reset(StateFlag::DELTO);
				delsfrms(code);
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
				thred::unmsg();
				return 1;
			}
		}
		if (StateMap.testAndReset(StateFlag::DELFRM)) {
			if (code == 'S' || code == VK_RETURN || code == 0xc0) {
				thred::savdo();
				if (code == 'S')
					StateMap.set(StateFlag::DELTO);
				else
					StateMap.reset(StateFlag::DELTO);
				thred::frmdel();
				fndknt();
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
				thred::unmsg();
				return 1;
			}
		}
		if (StateMap.test(StateFlag::THUMSHO)) {
			switch (code) {
			case VK_ESCAPE:
			case 'Q':
				unthum();
				StateMap.reset(StateFlag::BAKSHO);
				esccode();
				qcode();
				break;
			case VK_DOWN:
			case 0x22: // page down
				nuthsel();
				break;
			case VK_UP:
			case 0x21: // page up
				nuthbak(8);
				break;
			case VK_HOME:
				ThumbnailIndex = 0;
				nuthsel();
				break;
			case VK_END:
				ThumbnailIndex = Thumbnails->size();
				nuthbak(4);
				break;
			case 8: // backspace
			case VK_LEFT:
				bakthum();
				break;
			default:
				TranslateMessage(&Msg);
			}
			return 1;
		}
		if (StateMap.test(StateFlag::FSETFSPAC) || StateMap.test(StateFlag::GTWLKIND)) {
			// Check for keycode 'dash' and numpad 'subtract'
			if (code == 189 || code == 109) {
				*MsgBuffer   = '-';
				MsgIndex     = 1;
				MsgBuffer[1] = 0;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;
			}
		}
		if (FormMenuChoice || PreferenceIndex) {
			if (chkminus(code)) {
				MsgIndex                 = 1;
				SideWindowEntryBuffer[0] = '-';
				SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				return 1;
			}
			if (dunum(code)) {
				if (PreferenceIndex == PSHO + 1 || PreferenceIndex == PBOX + 1) {
					MsgBuffer[0] = gsl::narrow<char>(NumericCode);
					MsgBuffer[1] = 0;
					if (PreferenceIndex == PSHO + 1) {
						ShowStitchThreshold = unthrsh(NumericCode - 0x30);
						SetWindowText((*ValueWindow)[PSHO], MsgBuffer);
					}
					else {
						StitchBoxesThreshold = unthrsh(NumericCode - 0x30);
						SetWindowText((*ValueWindow)[PBOX], MsgBuffer);
					}
					thred::unsid();
				}
				else {
					if (MsgIndex < ((sizeof(SideWindowEntryBuffer) - 1) / sizeof(SideWindowEntryBuffer[0]))) {
						SideWindowEntryBuffer[MsgIndex++] = NumericCode;
						SideWindowEntryBuffer[MsgIndex]   = 0;
						SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
					}
				}
				return 1;
			}
			switch (code) {
			case 0x6e: // numpad period
			case 0xbe: // period
				SideWindowEntryBuffer[MsgIndex++] = '.';
				SideWindowEntryBuffer[MsgIndex]   = 0;
				SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				return 1;
			case 8: // backspace
				if (MsgIndex) {
					SideWindowEntryBuffer[--MsgIndex] = 0;
					SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				}
				return 1;
			case VK_RETURN:
				chknum();
				FormMenuChoice  = 0;
				PreferenceIndex = 0;
				return 1;
			}
		}
		if (code == 'I') {
			movi();
			LastKeyCode = 'I';
			return 1;
		}
		if (code == 'Q' && LastKeyCode == 'Q')
			unpat();
		LastKeyCode = code;
		if (StateMap.test(StateFlag::NUMIN)) {
			if (StateMap.test(StateFlag::SCLPSPAC) && code == 0xbd && !MsgIndex) {
				MsgBuffer[0] = '-';
				MsgBuffer[1] = 0;
				MsgIndex     = 1;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;
			}
			if (dunum(code)) {
				if (StateMap.test(StateFlag::TRNIN0)) {
					trace::traceNumberInput(NumericCode);
				}
				else {
					MsgBuffer[MsgIndex++] = NumericCode;
					MsgBuffer[MsgIndex]   = 0;
					SetWindowText(GeneralNumberInputBox, MsgBuffer);
				}
				return 1;
			}
			switch (code) {
			case 0x6e: // numpad period
			case 0xbe: // period
				MsgBuffer[MsgIndex++] = '.';
				MsgBuffer[MsgIndex]   = 0;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;
			case 8: // backspace
				if (MsgIndex) {
					MsgIndex--;
					if (StateMap.test(StateFlag::TRNIN0)) {
						trace::traceNumberReset();
					}
					else {
						MsgBuffer[MsgIndex] = 0;
						SetWindowText(GeneralNumberInputBox, MsgBuffer);
					}
				}
				return 1;
			case VK_RETURN:
				if (StateMap.test(StateFlag::TRNIN0))
					trace::dutrnum2();
				else {
					if (StateMap.test(StateFlag::TRNIN1))
						trace::dutrnum1();
				}
				break;
			}
		}
		/*
		if (code == 8 && BufferIndex) {
		    StitchEntryBuffer[--BufferIndex] = 0;
		    std::wstring txt(StitchEntryBuffer);
		    displayText::butxt(HNUM, txt);
		    ClosestPointIndex = std::stoi(StitchEntryBuffer);
		    movbox();
		    return 1;
		}
		if (!StateMap.test(StateFlag::WASTRAC) && dunum(code)) {
		    if (BufferIndex > BufferDigitCount - 1)
		        BufferIndex = 0;
		    if (StitchEntryBuffer[0] == '0' && BufferIndex)
		        BufferIndex--;
		    StitchEntryBuffer[BufferIndex++] = NumericCode;
		    StitchEntryBuffer[BufferIndex]   = 0;
		    ClosestPointIndex                = std::stoi(StitchEntryBuffer);
		    if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
		        // ToDo - replace use of swprintf_s
		        swprintf_s(StitchEntryBuffer,
		                   sizeof(StitchEntryBuffer) / sizeof(StitchEntryBuffer[0]),
		                   L"%d",
		                   PCSHeader.stitchCount - 1);
		        ClosestPointIndex = PCSHeader.stitchCount - 1;
		    }
		    std::wstring txt(StitchEntryBuffer);
		    displayText::butxt(HNUM, txt);
		    movbox();
		    StateMap.reset(StateFlag::NUMIN);
		    return 1;
		}
		*/
		// BufferIndex = 0;
		if (StateMap.testAndReset(StateFlag::ENTRDUP)) {
			const double value    = std::stof(MsgBuffer);
			double       newAngle = 0;
			if (value) {
				rotationAngle         = value * PI / 180;
				IniFile.rotationAngle = newAngle;
			}
			else
				newAngle = IniFile.rotationAngle;
			form::duprot(newAngle, rotationCenter);
		}
		if (StateMap.testAndReset(StateFlag::ENTROT)) {
			const double value    = std::stof(MsgBuffer);
			double       newAngle = 0;
			if (value) {
				newAngle              = value * PI / 180;
				IniFile.rotationAngle = newAngle;
			}
			else
				newAngle = IniFile.rotationAngle;
			rotfns(newAngle, rotationCenter);
		}
		thred::unmsg();
		switch (code) {
		case VK_ESCAPE:
			esccode();
		case 'Q':

			if (GetKeyState(VK_SHIFT) & 0x8000) {
				ritcur();
				return 1;
			}
			qcode();
			return 1;
		case 0xc0: //`
			if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState(VK_MENU) & 0x8000) {
				BackgroundColor    = 0x505050;
				BackgroundPen      = nuPen(BackgroundPen, 1, BackgroundColor);
				BackgroundPenWidth = 1;
				DeleteObject(BackgroundBrush);
				BackgroundBrush = CreateSolidBrush(BackgroundColor);
				if (PCSBMPFileName[0])
					bfil();
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
		case VK_INSERT:
			inscol();
			break;
		case 0x6d: // keypad -
			toglup();
			break;
		case 0xbd: //-
			StateMap.reset(StateFlag::CNTRH);
			StateMap.reset(StateFlag::CNTRV);
			if (GetKeyState(VK_SHIFT) & 0X8000)
				StateMap.set(StateFlag::CNTRV);
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000)
					StateMap.set(StateFlag::CNTRH);
			}
			form::cntrx();
			break;
		case 0xbf: /// forward slash /
			formForms::frmnum();
			break;
		case 0xbb: //=
			form::shrnk();
			break;
		case 0x21: // page up
			if (GetKeyState(VK_SHIFT) & 0X8000)
				form::redup();
			else
				form::rotagain();
			break;
		case 0x22: // page down
			if (GetKeyState(VK_SHIFT) & 0X8000)
				form::bakdup();
			else
				form::bakagain();
			break;
		case 0xde: //'
			desiz();
			break;
		case 0xdb: //[
			if (GetKeyState(VK_SHIFT) & 0X8000)
				selfrm0();
			else
				ungrplo();
			break;
		case 0xdd: //]
			if (GetKeyState(VK_SHIFT) & 0X8000)
				selfrmx();
			else
				ungrphi();
			break;
		case 0xba: // semicolon
			movmrk();
			break;
		case VK_DOWN:
			if (GetKeyState(VK_CONTROL) & 0X8000)
				nudgfn(0, -IniFile.cursorNudgeStep);
			else
				seldwn();
			break;
		case VK_UP:
			if (GetKeyState(VK_CONTROL) & 0X8000)
				nudgfn(0, IniFile.cursorNudgeStep);
			else
				selup();
			break;
		case 0xbe: // period
			if (GetKeyState(VK_SHIFT) & 0X8000)
				xt::setfilend();
			else
				setmov();
			break;
		case 'W':
			if (GetKeyState(VK_SHIFT) & 0X8000)
				form::crop();
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000)
					xt::pes2crd();
				else
					form::insat();
			}
			break;
		case 'E':
			if (GetKeyState(VK_SHIFT) & 0X8000)
				texture::dutxtfil();
			else
				form::infrm();
			break;
		case VK_F2:
			form::snap();
			break;
		case VK_F1:
			hlp::help();
			break;
		case VK_F3:
			if (GetKeyState(VK_CONTROL) & 0X8000)
				form::srtfrm();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					form::srtbyfrm();
				else
					xt::fsort();
			}
			break;
		case VK_F4:
			xt::rtrclp();
			break;
		case VK_F5:
			filfrms();
			break;
		case VK_F6:
			vuthrds();
			break;
		case VK_F7:
			thred::save();
			break;
		case VK_F8:
			savAs();
			break;
		case VK_F9:
			selCol();
			break;
		case VK_F11:
			if (GetKeyState(VK_SHIFT) & 0X8000)
				rembig();
			else
				duzero();
			break;
		case VK_F12:
			if (GetKeyState(VK_CONTROL) & GetKeyState(VK_MENU) & 0X8000)
				reldun();
			else {
#ifdef _DEBUG
				if (GetKeyState(VK_SHIFT) & 0X8000)
					xt::dmpat();
				else
#endif
					dun();
			}
			break;
		case 'Y':
			if (form::closfrm())
				selfpnt();
			break;
		case 'O':
			fop();
			break;
		case 'Z':
			if (GetKeyState(VK_SHIFT) & 0X8000) {
				ZoomFactor = ZoomMin;
				zumin();
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000)
					bak();
				else
					zumin();
			}
			break;
		case 'X':
			if (GetKeyState(VK_CONTROL) & 0X8000)
				cut();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					thred::hidbit();
				else
					thred::zumhom();
			}
			break;
		case 'P':
			formForms::prfmsg();
			break;
		case 'N':
			if (GetKeyState(VK_CONTROL) & 0X8000)
				xt::nudsiz();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					pgdwn();
				else {
					StateMap.set(StateFlag::TRCUP);
					trace::trace();
				}
			}
			break;
		case 'U':
			if (GetKeyState(VK_SHIFT) & 0X8000)
				pgup();
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000)
					trace::trdif();
				else {
					StateMap.reset(StateFlag::TRCUP);
					trace::trace();
				}
			}
			break;
		case 'H':
			if (GetKeyState(VK_SHIFT) & 0X8000)
				pglft();
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000)
					trace::trcsel();
				else
					trace::blak();
			}
			break;
		case 'J':
			if (GetKeyState(VK_SHIFT) & 0X8000)
				pgrit();
			else
				form::refilal();
			break;
		case 0xbc: // comma
			if (GetKeyState(VK_SHIFT) & 0X8000)
				xt::setfilstrt();
			else {
				thred::savdo();
				form::join();
			}
			break;
		case 'B':
			if (GetKeyState(VK_CONTROL) & 0X8000)
				redo();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					bakmrk();
				else
					bak();
			}
			break;
		case 'D':
			if (GetKeyState(VK_SHIFT) & 0X8000) {
				if (StateMap.test(StateFlag::FORMSEL)) {
					PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
					keybd_event('F', 0, 0, 0);
				}
			}
			else
				satin::satpnt1();
			break;
		case 'K':
			if (GetKeyState(VK_MENU) & GetKeyState(VK_CONTROL) & 0X8000)
				setknots();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					tglhid();
				else {
					if (GetKeyState(VK_CONTROL) & 0X8000)
						set1knot();
					else
						form::tglfrm();
				}
			}
			break;
		case 'A':
			if (GetKeyState(VK_CONTROL) & 0X8000)
				selalstch();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					form::selal();
				else
					zumout();
			}
			break;
		case VK_SPACE:
			if (StateMap.testAndFlip(StateFlag::INSRT)) {
				ReleaseCapture();
				StateMap.set(StateFlag::RESTCH);
			}
			else
				istch();
			unbox();
			if (StateMap.testAndReset(StateFlag::GRPSEL) || StateMap.testAndReset(StateFlag::FORMSEL))
				StateMap.set(StateFlag::RESTCH);
			break;
		case 'T':
			if (GetKeyState(VK_SHIFT) & 0x8000)
				retrac();
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000)
					trace::trinit();
				else
					thumnail();
			}
			break;
		case 'R':
			if (GetKeyState(VK_CONTROL) & GetKeyState(VK_SHIFT) & 0x8000) {
				thred::movStch();
				unbox();
				StateMap.set(StateFlag::RESTCH);
			}
			else {
				if (!FormDataSheet) {
					if (GetKeyState(VK_CONTROL) & 0x8000) {
						form::setrang();
						return 1;
					}
					if (GetKeyState(VK_SHIFT) & 0x8000) {
						rotmrk();
						return 1;
					}
					else {
						rotseg();
					}
				}
			}
			break;
		case VK_TAB:
			rot(rotationCenter);
			break;
		case 'S':
			if (GetKeyState(VK_CONTROL) & 0X8000) {
				colchk();
				thred::save();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					gsnap();
				else
					zumshft();
			}
			break;
		case 'F':
			if (SelectedFormList->size()) {
				PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
				keybd_event('F', 0, 0, 0);
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000) {
					StateMap.set(StateFlag::CNV2FTH);
					satin::ribon();
				}
				else {
					form::frmon();
					if (StateMap.test(StateFlag::FORMSEL))
						formForms::refrm();
					else
						form::form();
				}
			}
			break;
		case 'V':
			if (GetKeyState(VK_CONTROL) & 0X8000 && OpenClipboard(ThrEdWindow)) {
				ThrEdClip  = RegisterClipboardFormat(ThrEdClipFormat);
				ClipMemory = GetClipboardData(ThrEdClip);
				if (ClipMemory) {
					ClipPointer = GlobalLock(ClipMemory);
					if (ClipPointer) {
						ClipFormVerticesData = static_cast<FORMVERTEXCLIP*>(ClipPointer);
						if (ClipFormVerticesData->clipType == CLP_FRMPS) {
							thred::duzrat();
							byteCount = sizeof(*ClipFormVerticesData)
							            + (ClipFormVerticesData->vertexCount + 1) * sizeof(FormVertices[0]);
							auto clipCopyBuffer = std::vector<unsigned char>(byteCount);
							auto clipPointer    = static_cast<unsigned char*>(ClipPointer);
							auto _              = std::copy(clipPointer, clipPointer + byteCount, clipCopyBuffer.begin());
							GlobalUnlock(ClipMemory);
							CloseClipboard();
							ClipFormVerticesData = convert_ptr<FORMVERTEXCLIP*>(clipCopyBuffer.data());
							if (StateMap.test(StateFlag::FRMPSEL)) {
								form::fvars(ClosestFormToCursor);
								InterleaveSequence[0] = CurrentFormVertices[ClosestVertexToCursor];
								clipData              = convert_ptr<fPOINT*>(&ClipFormVerticesData[1]);
								size_t iVertex        = 0u;
								for (iVertex = 0; iVertex <= ClipFormVerticesData->vertexCount; iVertex++) {
									InterleaveSequence[iVertex + 1] = clipData[iVertex];
								}
								nextVertex = form::nxt(ClosestVertexToCursor);
								iVertex++;
								InterleaveSequence[iVertex] = CurrentFormVertices[nextVertex];
								OutputIndex                 = iVertex + 1;
								FormVerticesAsLine          = convert_ptr<POINT*>(&InterleaveSequence[OutputIndex]);
								setpclp();
								StateMap.set(StateFlag::FPUNCLP);
								StateMap.set(StateFlag::SHOP);
								dupclp();
							}
							else {
								FormMoveDelta = {};
								StateMap.set(StateFlag::FUNCLP);
								ClosestFormToCursor = FormIndex;
								SelectedForm        = &FormList[FormIndex];
								// ToDo - This is now dangerous. Need to define clear in FRMHED
								FillMemory(SelectedForm, sizeof((*SelectedForm)), 0);
								SelectedForm->type        = FRMLINE;
								SelectedForm->vertexCount = ClipFormVerticesData->vertexCount + 1;
								SelectedForm->vertices    = thred::adflt(SelectedForm->vertexCount);
								form::fvars(ClosestFormToCursor);
								auto vertices = convert_ptr<fPOINT*>(&ClipFormVerticesData[1]);
								std::copy(vertices,
								          vertices + SelectedForm->vertexCount,
								          stdext::make_checked_array_iterator(SelectedForm->vertices, SelectedForm->vertexCount));
								StateMap.set(StateFlag::INIT);
								NewFormVertexCount = SelectedForm->vertexCount;
								form::unfrm();
								form::setmfrm();
								StateMap.set(StateFlag::SHOFRM);
								form::dufrm();
							}
							return 1;
						}
						// ToDo - Add more information to the clipboard so that memory can be allocated
						ClipFormsHeader = static_cast<FORMSCLIP*>(ClipPointer);
						if (ClipFormsHeader->clipType == CLP_FRMS) {
							size_t iForm   = 0;
							ClipFormsCount = ClipFormsHeader->formCount;
							forms          = convert_ptr<FRMHED*>(&ClipFormsHeader[1]);
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								FormList[FormIndex + iForm] = forms[iForm];
								FormList[FormIndex + iForm].attribute
								    = (FormList[FormIndex + iForm].attribute & NFRMLMSK) | (ActiveLayer << 1);
							}
							// ToDo - use local variable for CurrentFormVertices
							CurrentFormVertices = convert_ptr<fPOINT*>(&forms[iForm]);
							currentVertex       = 0;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								SelectedForm           = &FormList[FormIndex + iForm];
								SelectedForm->vertices = thred::adflt(SelectedForm->vertexCount);
								for (size_t iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
									SelectedForm->vertices[iVertex] = CurrentFormVertices[currentVertex++];
								}
							}
							guides       = convert_ptr<SATCON*>(&CurrentFormVertices[currentVertex]);
							currentGuide = 0;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								SelectedForm = &FormList[FormIndex + iForm];
								if (SelectedForm->type == SAT && SelectedForm->satinGuideCount) {
									SelectedForm->satinOrAngle.guide = satin::adsatk(SelectedForm->satinGuideCount);
									for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
										SelectedForm->satinOrAngle.guide[iGuide] = guides[currentGuide++];
									}
								}
							}
							clipData    = convert_ptr<fPOINT*>(&guides[currentGuide]);
							currentClip = 0;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								SelectedForm = &FormList[FormIndex + iForm];
								if (clip::isclpx(FormIndex + iForm)) {
									SelectedForm->angleOrClipData.clip = thred::adclp(SelectedForm->lengthOrCount.clipCount);
									for (iClip = 0; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {
										SelectedForm->angleOrClipData.clip[iClip] = clipData[currentClip++];
									}
								}
								if (clip::iseclpx(FormIndex + iForm)) {
									SelectedForm->borderClipData = thred::adclp(SelectedForm->clipEntries);
									for (iClip = 0; iClip < SelectedForm->clipEntries; iClip++) {
										SelectedForm->borderClipData[iClip] = clipData[currentClip++];
									}
								}
							}
							textureSource = convert_ptr<TXPNT*>(&clipData[currentClip]);
							textureCount  = 0;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								if (texture::istx(FormIndex + iForm)) {
									SelectedForm = &FormList[FormIndex + iForm];
									textureCount += SelectedForm->fillInfo.texture.count;
									SelectedForm->fillInfo.texture.index += gsl::narrow<unsigned short>(TextureIndex);
								}
							}
							TexturePointsBuffer->resize(TexturePointsBuffer->size() + textureCount);
							auto textureDestination = TexturePointsBuffer->begin() + TextureIndex;
							auto _                  = std::copy(textureSource, textureSource + textureCount, textureDestination);
							TextureIndex += textureCount;
							GlobalUnlock(ClipMemory);
							SelectedFormsRect.top = SelectedFormsRect.left = 0x7fffffff;
							SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
							form::ratsr();
							SelectedFormList->reserve(ClipFormsCount);
							for (OutputIndex = 0; OutputIndex < gsl::narrow<unsigned>(ClipFormsCount); OutputIndex++) {
								form::fselrct(OutputIndex + FormIndex);
								SelectedFormList->push_back(OutputIndex + FormIndex);
							}
							SelectedFormsSize.x = SelectedFormsRect.right - SelectedFormsRect.left;
							SelectedFormsSize.y = SelectedFormsRect.bottom - SelectedFormsRect.top;
							StateMap.set(StateFlag::INIT);
							FormLines[0].x = FormLines[3].x = FormLines[4].x = SelectedFormsRect.left;
							FormLines[1].x = FormLines[2].x = SelectedFormsRect.right;
							FormLines[0].y = FormLines[1].y = FormLines[4].y = SelectedFormsRect.top;
							FormLines[2].y = FormLines[3].y = SelectedFormsRect.bottom;
							StateMap.set(StateFlag::SHOSTRTCH);
							thred::strtchbox(stretchBoxLine);
							FormMoveDelta.x = ((SelectedFormsRect.right - SelectedFormsRect.left) >> 1);
							FormMoveDelta.y = ((SelectedFormsRect.bottom - SelectedFormsRect.top) >> 1);
							StateMap.set(StateFlag::MOVFRMS);
							StateMap.set(StateFlag::FUNSCLP);
						}
						else {
							ClipFormHeader = static_cast<FORMCLIP*>(ClipPointer);
							if (ClipFormHeader->clipType == CLP_FRM) {
								FormMoveDelta = {};
								StateMap.set(StateFlag::FUNCLP);
								ClosestFormToCursor = FormIndex;
								form::fvars(FormIndex);
								FormList[FormIndex]           = ClipFormHeader->form;
								FormList[FormIndex].attribute = (FormList[FormIndex].attribute & NFRMLMSK) | (ActiveLayer << 1);
								SelectedForm->vertices        = thred::adflt(FormList[FormIndex].vertexCount);
								CurrentFormVertices           = convert_ptr<fPOINT*>(&ClipFormHeader[1]);
								std::copy(CurrentFormVertices,
								          CurrentFormVertices + SelectedForm->vertexCount,
								          stdext::make_checked_array_iterator(SelectedForm->vertices, SelectedForm->vertexCount));
								guides = convert_ptr<SATCON*>(&CurrentFormVertices[SelectedForm->vertexCount]);
								if (SelectedForm->type == SAT && SelectedForm->satinGuideCount) {
									SelectedForm->satinOrAngle.guide = satin::adsatk(SelectedForm->satinGuideCount);
									std::copy(guides,
									          guides + SelectedForm->satinGuideCount,
									          stdext::make_checked_array_iterator(SelectedForm->satinOrAngle.guide,
									                                              SelectedForm->satinGuideCount));
								}
								clipData  = convert_ptr<fPOINT*>(&guides[0]);
								clipCount = 0;
								if (clip::isclpx(FormIndex)) {
									SelectedForm->angleOrClipData.clip = thred::adclp(SelectedForm->lengthOrCount.clipCount);
									std::copy(clipData,
									          clipData + SelectedForm->lengthOrCount.clipCount,
									          stdext::make_checked_array_iterator(SelectedForm->angleOrClipData.clip,
									                                              SelectedForm->lengthOrCount.clipCount));
									clipCount += SelectedForm->lengthOrCount.clipCount;
								}
								if (clip::iseclpx(FormIndex)) {
									clipData                     = convert_ptr<fPOINT*>(&clipData[clipCount]);
									SelectedForm->borderClipData = thred::adclp(SelectedForm->clipEntries);
									std::copy(clipData,
									          clipData + SelectedForm->clipEntries,
									          stdext::make_checked_array_iterator(SelectedForm->borderClipData,
									                                              SelectedForm->clipEntries));
									clipCount += SelectedForm->clipEntries;
								}
								textureSource = convert_ptr<TXPNT*>(&clipData[clipCount]);
								if (texture::istx(FormIndex)) {
									SelectedForm->fillInfo.texture.index = gsl::narrow<unsigned short>(TextureIndex);

									auto currentCount = SelectedForm->fillInfo.texture.count;
									TexturePointsBuffer->resize(TexturePointsBuffer->size() + currentCount);
									auto iter = TexturePointsBuffer->begin() + TextureIndex;
									TextureIndex += currentCount;
									const auto _ = std::copy(textureSource, textureSource + currentCount, iter);
								}
							}
							GlobalUnlock(ClipMemory);
							StateMap.set(StateFlag::INIT);
							NewFormVertexCount = SelectedForm->vertexCount;
							if (SelectedForm->type != FRMLINE)
								NewFormVertexCount++;
							form::unfrm();
							thred::duzrat();
							form::setmfrm();
							StateMap.set(StateFlag::SHOFRM);
							form::dufrm();
						}
					}
					CloseClipboard();
				}
				else {
					Clip       = RegisterClipboardFormat(PcdClipFormat);
					ClipMemory = GetClipboardData(Clip);
					if (ClipMemory) {
						thred::redclp();
						clpbox();
						StateMap.set(StateFlag::CLPSHO);
					}
					CloseClipboard();
				}
			}
			else
				closPnt();
			break;
		case 'C':
			if (GetKeyState(VK_CONTROL) & 0X8000)
				duclip();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000) {
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
		case VK_HOME:
			if (GetKeyState(VK_SHIFT) & 0X8000 && GetKeyState(VK_CONTROL) & 0X8000) {
				if (StateMap.testAndReset(StateFlag::SELBOX)) {
					GroupStitchIndex = 0;
					StateMap.set(StateFlag::GRPSEL);
				}
				else {
					if (StateMap.test(StateFlag::GRPSEL)) {
						if (GroupStitchIndex > ClosestPointIndex)
							ClosestPointIndex = 0;
						else
							GroupStitchIndex = 0;
					}
					else {
						StateMap.set(StateFlag::SELBOX);
						StateMap.set(StateFlag::RESTCH);
					}
				}
				thred::grpAdj();
				thred::redraw(ColorBar);
				return 1;
			}
			if (GetKeyState(VK_SHIFT) & 0X8000) {
				if (StateMap.testAndReset(StateFlag::SELBOX)) {
					StateMap.set(StateFlag::GRPSEL);
					iColor = pt2colInd(ClosestPointIndex);
				}
				else
					iColor = pt2colInd(GroupStitchIndex);
				iColor--;
				GroupStitchIndex = ColorChangeTable[iColor].stitchIndex;
				thred::grpAdj();
				thred::redraw(ColorBar);
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000) {
					stch2px1(0);
					endpnt();
					StateMap.reset(StateFlag::BAKEND);
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::SELBOX)) {
						iColor = pt2colInd(ClosestPointIndex);
						if (iColor)
							iColor--;
						ClosestPointIndex = ColorChangeTable[iColor].stitchIndex;
					}
					else {
						if (StateMap.test(StateFlag::LENSRCH)) {
							setsrch(SmallestStitchIndex);
							return 1;
						}
						else
							ClosestPointIndex = 0;
					}
					movbox();
				}
			}
			break;
		case VK_END:
			if (GetKeyState(VK_SHIFT) & 0X8000 && GetKeyState(VK_CONTROL) & 0X8000) {
				if (StateMap.testAndReset(StateFlag::SELBOX)) {
					GroupStitchIndex = PCSHeader.stitchCount - 1;
					StateMap.set(StateFlag::GRPSEL);
				}
				else {
					if (StateMap.test(StateFlag::GRPSEL)) {
						if (GroupStitchIndex > ClosestPointIndex)
							GroupStitchIndex = PCSHeader.stitchCount - 1;
						else
							ClosestPointIndex = PCSHeader.stitchCount - 1;
					}
					else {
						StateMap.set(StateFlag::SELBOX);
						ClosestPointIndex = PCSHeader.stitchCount - 1;
						StateMap.set(StateFlag::RESTCH);
						return 1;
					}
				}
				thred::grpAdj();
				thred::redraw(ColorBar);
				break;
			}
			if (GetKeyState(VK_SHIFT) & 0X8000) {
				if (StateMap.testAndReset(StateFlag::SELBOX)) {
					StateMap.set(StateFlag::GRPSEL);
					iColor = pt2colInd(ClosestPointIndex);
				}
				else
					iColor = pt2colInd(GroupStitchIndex);
				GroupStitchIndex = ColorChangeTable[iColor].stitchIndex - 1;
				thred::grpAdj();
				thred::redraw(ColorBar);
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000) {
					stch2px1(PCSHeader.stitchCount - 1);
					endpnt();
					StateMap.set(StateFlag::BAKEND);
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::SELBOX)) {
						iColor            = pt2colInd(ClosestPointIndex);
						ClosestPointIndex = ColorChangeTable[iColor].stitchIndex - 1;
					}
					else {
						if (StateMap.test(StateFlag::LENSRCH)) {
							setsrch(LargestStitchIndex);
							return 1;
						}
						else
							ClosestPointIndex = PCSHeader.stitchCount - 1;
					}
					movbox();
				}
			}
			break;
		case VK_RIGHT:
			form::fvars(ClosestFormToCursor);
			if (GetKeyState(VK_SHIFT) & 0X8000) {
				if (StateMap.test(StateFlag::FPSEL)) {
					if (StateMap.test(StateFlag::PSELDIR)) {
						++SelectedFormVertices.vertexCount %= VertexCount;
						SelectedFormVertices.finish
						    = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % VertexCount;
					}
					else {
						if (SelectedFormVertices.vertexCount) {
							SelectedFormVertices.vertexCount--;
							SelectedFormVertices.finish
							    = (SelectedFormVertices.start + VertexCount - SelectedFormVertices.vertexCount) % VertexCount;
						}
						else {
							SelectedFormVertices.vertexCount = 1;
							StateMap.set(StateFlag::PSELDIR);
							SelectedFormVertices.finish = (SelectedFormVertices.start + 1) % VertexCount;
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
						SelectedFormVertices.finish      = (SelectedFormVertices.start + 1) % VertexCount;
						StateMap.set(StateFlag::PSELDIR);
						thred::setpsel();
					}
					else {
						StateMap.reset(StateFlag::LENSRCH);
						StateMap.reset(StateFlag::FORMSEL);
						if (StateMap.testAndReset(StateFlag::SELBOX)) {
							if (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
								StateMap.set(StateFlag::GRPSEL);
								GroupStitchIndex = ClosestPointIndex + 1;
							}
						}
						else {
							if (GroupStitchIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
								GroupStitchIndex++;
								nuAct(GroupStitchIndex);
							}
						}
						thred::grpAdj();
						thred::redraw(ColorBar);
					}
				}
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(IniFile.cursorNudgeStep, 0);
				else {
					if (StateMap.test(StateFlag::LENSRCH)) {
						longer();
						StateMap.set(StateFlag::RESTCH);
					}
					else {
						if (StateMap.test(StateFlag::FRMPSEL)) {
							form::fvars(ClosestFormToCursor);
							ClosestVertexToCursor = form::nxt(ClosestVertexToCursor);
							displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
							thred::ritfcor(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							shftflt(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							StateMap.set(StateFlag::RESTCH);
						}
						else {
							if (StateMap.test(StateFlag::SELBOX)) {
								if (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
									ClosestPointIndex++;
								movbox();
								return 1;
							}
							if (StateMap.test(StateFlag::GRPSEL)) {
								if (GroupStitchIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
									GroupStitchIndex++;
									thred::grpAdj();
									thred::redraw(ColorBar);
								}
							}
						}
					}
				}
			}
			break;
		case VK_LEFT:
			if (GetKeyState(VK_SHIFT) & 0X8000) {
				if (StateMap.test(StateFlag::FPSEL)) {
					if (!StateMap.test(StateFlag::PSELDIR)) {
						++SelectedFormVertices.vertexCount %= VertexCount;
						SelectedFormVertices.finish
						    = (SelectedFormVertices.start + VertexCount - SelectedFormVertices.vertexCount) % VertexCount;
					}
					else {
						if (SelectedFormVertices.vertexCount) {
							SelectedFormVertices.vertexCount--;
							SelectedFormVertices.finish
							    = (SelectedFormVertices.start + VertexCount - SelectedFormVertices.vertexCount) % VertexCount;
						}
						else {
							SelectedFormVertices.vertexCount = 1;
							SelectedFormVertices.finish      = (SelectedFormVertices.start + VertexCount - 1) % VertexCount;
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
							GroupStitchIndex = ClosestPointIndex - 1;
						}
						else if (GroupStitchIndex) {
							GroupStitchIndex--;
							nuAct(GroupStitchIndex);
						}
						thred::grpAdj();
						thred::redraw(ColorBar);
					}
				}
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(-IniFile.cursorNudgeStep, 0);
				else {
					if (StateMap.test(StateFlag::LENSRCH)) {
						shorter();
						StateMap.set(StateFlag::RESTCH);
					}
					else {
						if (StateMap.test(StateFlag::FRMPSEL)) {
							form::fvars(ClosestFormToCursor);
							ClosestVertexToCursor = form::prv(ClosestVertexToCursor);
							displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
							thred::ritfcor(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							shftflt(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							StateMap.set(StateFlag::RESTCH);
						}
						else {
							if (StateMap.test(StateFlag::SELBOX)) {
								if (ClosestPointIndex)
									ClosestPointIndex--;
								movbox();
								return 1;
							}
							if (StateMap.test(StateFlag::GRPSEL)) {
								if (GroupStitchIndex) {
									GroupStitchIndex--;
									thred::grpAdj();
									thred::redraw(ColorBar);
								}
							}
						}
					}
				}
			}
			break;
		case 'G':
			mark();
			break;
		case 'M':
			if (GetKeyState(VK_CONTROL) & 0x8000)
				pntmrk();
			else {
				if (GetKeyState(VK_SHIFT) & 0X8000)
					dumrk(UnzoomedRect.x / 2, UnzoomedRect.y / 2);
				else {
					if (thred::px2stch())
						dumrk(SelectedPoint.x, SelectedPoint.y);
				}
				StateMap.set(StateFlag::RESTCH);
			}
			break;
		case 'L':
			if (GetKeyState(VK_SHIFT) & 0x8000)
				delstch();
			else
				form::fcntr();
			break;
		case VK_DELETE:
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15]))
				delcol();
			else
				delet();
			break;
		}
		break;
	case WM_COMMAND:
		thred::unmsg();
		if (StateMap.test(StateFlag::FORMSEL))
			form::fvars(ClosestFormToCursor);
		{
			const auto& previousNames = *PreviousNames;
			for (iVersion = 0; iVersion < OLDNUM; iVersion++) {
				if (Msg.wParam == LRUMenuId[iVersion]) {
					*WorkingFileName = previousNames[iVersion];
					StateMap.set(StateFlag::REDOLD);
					nuFil();
				}
			}
		}
		wParameter = LOWORD(Msg.wParam);
		if (wParameter > 40000 && wParameter < 40400)
			thred::undat();
		switch (wParameter) {
		case ID_CHKOF: // view / Set / Data check / Off
			xt::chgchk(0);
			break;
		case ID_CHKON: // view / Set / Data check / On
			xt::chgchk(1);
			break;
		case ID_CHKREP: // view / Set / Data Check / Auto Repair on
			xt::chgchk(2);
			break;
		case ID_CHKREPMSG: // view / Set / Data Check / Auto Repair with Message
			xt::chgchk(3);
			break;
		case ID_REPAIR: // edit / Repair Data
			repair::repar();
			break;
		case ID_WARNOF: // set / Warn if edited
			xt::chgwrn();
			break;
		case ID_CLPSPAC: // set / Clipboard Fill  Spacing
			xt::setclpspac();
			break;
		case ID_FRMIND: // edit / Form Update / Indent
			xt::setfind();
			break;
		case ID_SETSIZ: // edit / Set / Design Size
			xt::nudsiz();
			break;
		case ID_TXFIL: // Fill / Texture Editor
			texture::dutxtfil();
			break;
		case ID_FRMHI: // edit / Form Update / Height
			xt::setfhi();
			break;
		case ID_FRMWID: // edit / Form Update / Width
			xt::setfwid();
			break;
		case ID_MAXFLEN: // edit / Form Update / Fill /  Maximum Stitch Length
			xt::setfmax();
			break;
		case ID_MINFLEN: // edit / Form Update / Fill /  Minimum Stitch Length
			xt::setfmin();
			break;
		case ID_MAXBLEN: // edit / Form Update / Border /  Maximum Stitch Length
			xt::setbmax();
			break;
		case ID_MINBLEN: // edit / Form Update / Border /  Minimum Stitch Length
			xt::setbmin();
			break;
		case ID_SETBSPAC: // edit / Form Update / Border /  Spacing
			xt::setbspac();
			break;
		case ID_SETBLEN: // edit / Form Update / Border /  Stitch Length
			xt::setblen();
			break;
		case ID_SETBCOL: // edit / Form Update / Border /  Color
			xt::setbcol();
			break;
		case ID_SETFCOL: // edit / Form Update / Fill /  Color
			xt::setfcol();
			break;
		case ID_SETUCOL: // edit / Form Update / Underlay /  Color
			xt::setucol();
			break;
		case ID_SETFANG: // edit / Form Update / Fill /  Angle
			xt::setfang();
			break;
		case ID_SETFSPAC: // edit / Form Update / Fill /  Spacing
			xt::setfspac();
			break;
		case ID_SETFLEN: // edit / Form Update / Fill /  Stitch Length
			xt::setflen();
			break;
		case ID_SETUANG: // edit / Form Update / Underlay /  Angle
			xt::sfuang();
			break;
		case ID_SETUSPAC: // edit / Form Update / Underlay /  Spacing
			xt::uspac();
			break;
		case ID_UNDLEN: // edit / Form Update / Underlay / Stitch Length
			xt::undlen();
			break;
		case ID_SETCWLK: // edit / Form Update / Center Walk / On
			xt::setcwlk();
			break;
		case ID_SETWLK: // edit / Form Update / Edge Walk /  On
			xt::setwlk();
			break;
		case ID_SETUND: // edit / Form Update / Underlay / On
			xt::setund();
			break;
		case ID_NOTCWLK: // edit / Form Update / Center Walk / Off
			xt::notcwlk();
			break;
		case ID_NOTWLK: // edit / Form Update / Edge Walk / Off
			xt::notwlk();
			break;
		case ID_NOTUND: // edit / Form Update / Underlay / Off
			xt::notund();
			break;
		case ID_SELUND: // edit / Select / Form Underlay Stitches
			form::selfil(UNDMSK);
			break;
		case ID_SELWLK: // edit / Select / Form Edge Walk Stitches
			form::selfil(WLKMSK);
			break;
		case ID_ALFRM: // edit / Select / All Forms
			xt::selalfrm();
			break;
		case ID_USPAC: // view / Set / Underlay / Spacing
			xt::setuspac();
			break;
		case ID_UANG: // view / Set / Underlay / Angle
			xt::setuang();
			break;
		case ID_USTCH: // view / Set / Underlay / Stitch Length
			xt::setulen();
			break;
		case ID_WIND: // view / Set / Underlay / Indent
			xt::setwlkind();
			break;
		case ID_FILSTRT: // Edit / Set / Fill Start Point
			xt::setfilstrt();
			break;
		case ID_FILEND: // Edit / Set / Fill End Point
			xt::setfilend();
			break;
		case ID_PES2CRD: // file / PES2Card
			xt::pes2crd();
			break;
		case ID_2FTHR: // edit / Convert / to Feather Ribbon
			StateMap.set(StateFlag::CNV2FTH);
			satin::ribon();
			break;
		case ID_FETHR: // fill / Feather
			xt::fethr();
			break;
		case ID_FTHDEF: // edit / Set / Feather Defaults
			dufdef();
			break;
		case ID_SRTF: // edit / Sort / by Form
			form::srtfrm();
			break;
		case ID_FILCLPX: // fill / Border / Clipboard, Even
			form::filclpx();
			break;
		case ID_FRMX: // view / Set / Form Cursor / Cross
			frmcursel(1);
			break;
		case ID_FRMBOX: // view / Set / Form Cursor / Box
			frmcursel(0);
			break;
		case ID_KNOTAT: // edit / Set / Knot at Selected Stitch
			set1knot();
			break;
		case ID_STCHPIX: // view / Set / Point Size / Stitch Point Boxes
			getstpix();
			;
			break;
		case ID_FRMPIX: // view / Set / Point Size / Form Point Triangles
			getfrmpix();
			break;
		case ID_FRMPBOX: // view / Set / Point Size / Form Box
			getfrmbox();
			break;
		case ID_CROP: // edit / Crop to Form
			form::crop();
			break;
		case ID_BAKMRK: // view / Retrieve Mark
			bakmrk();
			break;
		case ID_MARKESC: // view / Set / Retrieve Mark / Escape
			UserFlagMap.set(UserFlag::MARQ);
			qchk();
			break;
		case ID_MARKQ: // view / Set / Retrieve Mark / Q
			UserFlagMap.reset(UserFlag::MARQ);
			qchk();
			break;
		case ID_NUDGPIX: // view / Set / Nudge Pixels
			getnpix();
			break;
		case ID_LINCHN: // fill / Border / Line chain
			StateMap.set(StateFlag::LINCHN);
			form::chain();
			break;
		case ID_OPNCHN: // fill / Border / Open chain
			StateMap.reset(StateFlag::LINCHN);
			form::chain();
			break;
		case ID_CLOSE: // file / Close
			filclos();
			break;
		case ID_DELMAP: // file / Remove Bitmap
			delmap();
			break;
		case ID_BLAK: // edit / Trace / Reset Form Pixels
			trace::blak();
			break;
		case ID_SAVMAP: // file / Save Bitmap
			savmap();
			break;
		case ID_TRDIF: // edit / Trace / Find Edges
			trace::trdif();
			break;
		case ID_TRACEDG: // edit / Trace / Show Traced Edges
			trace::tracedg();
			break;
		case ID_TRCSEL: // edit / Trace / Select Colors
			trace::trcsel();
			break;
		case ID_TRACE: // edit / Trace / Trace Mode
			trace::trinit();
			break;
		case ID_FLOK: // file / Locking
			lock();
			break;
		case ID_ROTAUXON: // view / Set / Rotate Machine File / On
			rotauxsel(1);
			break;
		case ID_ROTAUXOFF: // view / Set / Rotate Machine File / Off
			rotauxsel(0);
			break;
		case ID_FRM2COL: // edit / Set / Form Color to  stitch Color
			form::col2frm();
			break;
		case ID_SNAP2GRD: // edit / Snap / to Grid
			gsnap();
			break;
		case ID_FIL2SEL_ON: // view / Set / Fill at Select / On
			fil2sel(1);
			break;
		case ID_FIL2SEL_OFF: // view / Set / Fill at Select / Off
			fil2sel(0);
			break;
		case ID_OVRLAY: // file / Overlay
			ovrlay();
			break;
		case ID_GRDHI: // view / Set / Grid Mask / High
			setgrd(HIGRD);
			break;
		case ID_GRDMED: // view / Set / Grid Mask / Medium
			setgrd(MEDGRD);
			break;
		case ID_GRDEF: // view / Set / Grid Mask / Default
			setgrd(DEFGRD);
			break;
		case ID_GRDRED: // view / Set / Grid Mask / UnRed
			setgrd(REDGRD);
			break;
		case ID_GRDBLU: // view / Set / Grid Mask / UnBlue
			setgrd(BLUGRD);
			break;
		case ID_GRDGRN: // view / Set / Grid Mask / UnGreen
			setgrd(GRNGRD);
			break;
		case ID_RETRACE: // edit / Retrace
			retrac();
			break;
		case ID_DUBFIL: // fill / Border / Double
			form::dubfil();
			break;
		case ID_HORCLP: // fill / Clipboard / Horizontal
			if (StateMap.test(StateFlag::FORMSEL) || SelectedFormList->size())
				thred::savdo();
			form::horclp();
			break;
		case ID_ANGCLP: // fill / Clipboard / Angle
			if (StateMap.test(StateFlag::FORMSEL) || SelectedFormList->size())
				thred::savdo();
			form::angclp();
			break;
		case ID_VRTCLP: // fill / Clipboard / Vertical
			if (StateMap.test(StateFlag::FORMSEL) || SelectedFormList->size())
				thred::savdo();
			form::vrtclp();
			break;
		case ID_LINBEXACT: // view / Set / Line Border Spacing / Exact
			UserFlagMap.reset(UserFlag::LINSPAC);
			linbmen();
			break;
		case ID_LINBEVEN: // view / Set / Line Border Spacing / Even
			UserFlagMap.set(UserFlag::LINSPAC);
			linbmen();
			break;
		case ID_BSAVON: // view / Set / PCS Bitmap Save / On
			pcsbsavon();
			break;
		case ID_BSAVOF: // view / Set / PCS Bitmap Save / Off
			pcsbsavof();
			break;
		case ID_KNOTON: // view / Knots / On
			shoknot();
			break;
		case ID_KNOTOF: // view / Knots / Off
			hidknot();
			break;
		case ID_DELKNOT: // edit / Delete / Knots
			delknot();
			break;
		case ID_RSTNEDL: // view / Set / Needle Cursor / Off
			nedof();
			break;
		case ID_SETNEDL: // view / Set / Needle Cursor / On
			nedon();
			break;
		case ID_STCHS2FRM: // edit / Convert / Stitches to Form
			form::stchs2frm();
			break;
		case ID_SPLTFRM: // edit / Split Form
			form::spltfrm();
			break;
		case ID_UNBEAN: // edit / Convert / from Bean to Line
			form::debean();
			break;
		case ID_DUBEAN: // edit / Convert / to Bean
			form::dubean();
			break;
		case ID_SRTBF: // edit / Sort / by Color then Form
			form::srtbyfrm();
			break;
		case ID_CENTIRE: // edit / Center / Entire Design
			form::centir();
			break;
		case ID_CNTRX: // edit / Center / Both
			StateMap.reset(StateFlag::CNTRH);
			StateMap.reset(StateFlag::CNTRV);
			form::cntrx();
			break;
		case ID_CNTRH: // edit / Center / Horizontal
			StateMap.reset(StateFlag::CNTRH);
			StateMap.set(StateFlag::CNTRV);
			form::cntrx();
			break;
		case ID_CNTRV: // edit / Center / Vertical
			StateMap.set(StateFlag::CNTRH);
			StateMap.reset(StateFlag::CNTRV);
			form::cntrx();
			break;
		case ID_FRMNUM: // edit / Set / Form Number
			formForms::frmnum();
			break;
		case ID_REFILF: // fill / Refill
			filfrms();
			break;
		case ID_MRKPNT: // edit / Set / Zoom Mark at Selected Point
			pntmrk();
			break;
		case ID_ROTSEG: // edit / Set / Rotation / Segments
			rotseg();
			break;
		case ID_ROTMRK: // edit / Set / Rotation / Angle from Mark
			rotmrk();
			break;
		case ID_MRKCNTR: // edit / Set / Zoom Mark at Center
			dumrk(UnzoomedRect.x / 2, UnzoomedRect.y / 2);
			StateMap.set(StateFlag::RESTCH);
			break;
		case ID_SETROT: // edit / Set / Rotation / Angle
			form::setrang();
			break;
		case ID_SETPREF: // view / Set / Default Preferences
			defpref();
			break;
		case ID_SHRINK: // edit / Shrink Clipboard Border
			form::shrnk();
			break;
		case ID_DUPAGAIN: // edit / Rotate / and Duplicate again
			form::redup();
			break;
		case ID_CLPADJ: // edit / Set / Range Ends for Clipboard Fills
			clpadj();
			break;
		case ID_DELTOT: // edit / Delete / All Forms and Stitches
			deltot();
			break;
		case ID_AUXPCS: // view / Set / Machine File Type / Pfaff PCS
			setpcs();
			break;
#if PESACT
		case ID_AUXPES: // view / Set / Machine File Type / Brother PES
			setpes();
			break;
#endif
		case ID_AUXDST: // view / Set / Machine File Type / Tajima DST
			setdst();
			break;
		case ID_ABOUT: // view / About ThrEd4
			displayText::tabmsg(IDS_CPYRIT);
			break;
		case ID_RIBON: // edit / Convert / to Satin Ribbon
			StateMap.reset(StateFlag::CNV2FTH);
			satin::ribon();
			break;
		case ID_DESIZ: // view / Design Information
			desiz();
			break;
		case ID_HLP: // help
			hlp::help();
			break;
		case ID_CNTR: // edit / Center / Forms
			form::fcntr();
			break;
		case ID_INSFIL: // file / Insert
			StateMap.reset(StateFlag::IGNORINS);
			insfil();
			break;
		case ID_SELALSTCH: // edit / Select / All Stitches
			selalstch();
			break;
		case ID_UNGRPLO: // edit / Ungroup / First
			ungrplo();
			break;
		case ID_UNGRPHI: // edit / Ungroup / Last
			ungrphi();
			break;
		case ID_CONTF: // fill / Contour
			form::contfil();
			break;
		case ID_THUM: // file / Thumbnails
			thumnail();
			break;
		case ID_PICOT: // fill / Border / Picot
			form::picot();
			break;
		case ID_FILBUT: // fill / Border / Buttonhole
			form::bhol();
			break;
		case ID_REMBIG: // edit / Delete / Large Stitches
			rembig();
			break;
		case ID_VUSELTHRDS: // view / Show Threads for Selected Color
			vuselthr();
			break;
		case ID_VUTHRDS: // view / Show Threads
			vuthrds();
			break;
		case ID_MOVMRK: // edit / Move / to Mark
			movmrk();
			break;
		case ID_SELFIL: // edit / Select / Form Fill Stitches
			form::selfil(FRMFIL);
			break;
		case ID_SELBRD: // edit / Select / Form Border Stitches
			form::selfil(FRMBFIL);
			break;
		case ID_SELAP: // edit / Select / Form Applique Stitches
			form::selfil(FRMAPFIL);
			break;
		case ID_SELFSTCHS: // edit / Select / Form Stitches
			form::selalfil();
			break;
		case ID_SETMRK: // edit / Set / Order Mark
			setmov();
			break;
		case ID_DELFRE: // edit / Delete / Free Stitches
			delfre();
			break;
		case ID_SELAL: // edit / Select / All Forms and Stitches
			form::selal();
			break;
		case ID_REFILAL: // edit / Refill All
			form::refilal();
			break;
		case ID_CHK: // edit / Check Range
			thred::chkrng(StitchRangeSize);
			StateMap.set(StateFlag::RESTCH);
			break;
		case ID_RTRVCLP: // edit / Retrieve Clipboard Stitches
			xt::rtrclp();
			break;
		case ID_SORT: // edit / Sort / Auto
			xt::fsort();
			break;
		case ID_LAYMOV0: // edit / Move to Layer / 0
			form::movlayr(0);
			break;
		case ID_LAYMOV1: // edit / Move to Layer / 1
			form::movlayr(2);
			break;
		case ID_LAYMOV2: // edit / Move to Layer / 2
			form::movlayr(4);
			break;
		case ID_LAYMOV3: // edit / Move to Layer / 3
			form::movlayr(6);
			break;
		case ID_LAYMOV4: // edit / Move to Layer / 4
			form::movlayr(8);
			break;
		case ID_LAYCPY0: // edit / Copy to Layer / 0
			form::cpylayr(0);
			break;
		case ID_LAYCPY1: // edit / Copy to Layer / 1
			form::cpylayr(2);
			break;
		case ID_LAYCPY2: // edit / Copy to Layer / 2
			form::cpylayr(4);
			break;
		case ID_LAYCPY3: // edit / Copy to Layer / 3
			form::cpylayr(6);
			break;
		case ID_LAYCPY4: // edit / Copy to Layer / 4
			form::cpylayr(8);
			break;
		case ID_LA: // all (Show all layers)
			nulayr(0);
			break;
		case ID_L1: // 1 (Show layer 1 only)
			nulayr(1);
			break;
		case ID_L2: // 2 (Show layer 2 only)
			nulayr(2);
			break;
		case ID_L3: // 3 (Show layer 3 only)
			nulayr(3);
			break;
		case ID_L4: // 4 (Show layer 4 only)
			nulayr(4);
			break;
		case ID_ROTDUP: // edit / Rotate / and Duplicate
			form::rotdup();
			break;
		case ID_ROTAGAIN: // edit / Rotate / Again
			form::rotagain();
			break;
		case ID_ROTCMD: // edit / Rotate / Command
			form::rotcmd(rotationCenter);
			break;
		case ID_DELFRMS: // edit / Delete / All Forms
			form::delfrms();
			rstAll();
			StateMap.set(StateFlag::RESTCH);
			break;
		case ID_SNAP2: // edit / Snap / Together
			form::snap();
			break;
		case ID_CLPFIL: // fill / Clipboard / Fan
			form::clpfil();
			break;
		case ID_FLPORD: // edit / Flip / Order
			form::flpord();
			break;
		case ID_FRMOF: // frm+
			if (GetKeyState(VK_SHIFT) & 0X8000)
				tglhid();
			else
				form::tglfrm();
			break;
		case ID_MV2BAK: // edit / move / to End
			mv2b();
			break;
		case ID_MV2FRNT: // edit / move / to Start
			mv2f();
			break;
		case ID_PERP: // fill / Border / Perpendicular Satin
			if (StateMap.test(StateFlag::FORMSEL) || SelectedFormList->size())
				thred::savdo();
			form::prpbrd(LineSpacing);
			break;
		case ID_PURGDIR: // file / Delete Backups / All backups in the selected directory
			purgdir();
			break;
		case ID_PURG: // file / Delete Backups / Backups for the selected file
			purg();
			break;
		case ID_VUBAK: // view / Backups
			vubak();
			break;
		case ID_DELETE: // edit / Delete / Selected
			delet();
			break;
		case ID_FLIPH: // edit / Flip / Horizontal
			form::fliph();
			break;
		case ID_FLIPV: // edit / Flip / Vertical
			form::flipv();
			break;
		case ID_FILANG: // fill / Angle
			if (StateMap.test(StateFlag::FORMSEL))
				thred::savdo();
			form::filangl();
			break;
		case ID_PREF: // pref
			formForms::prfmsg();
			break;
		case ID_BOLD: // fill / Border / Bean
			if (StateMap.test(StateFlag::FORMSEL) || SelectedFormList->size())
				thred::savdo();
			form::dubold();
			break;
		case ID_ADEND: // add
			stch2px1(PCSHeader.stitchCount - 1);
			endpnt();
			StateMap.set(StateFlag::BAKEND);
			StateMap.set(StateFlag::RESTCH);
			break;
		case ID_SETAP: // view / Set / Applique Color
			form::setap();
			break;
		case ID_APLIQ: // fill / Border / Applique
			if (StateMap.test(StateFlag::FORMSEL))
				thred::savdo();
			form::apliq();
			break;
		case ID_SATBRD: // fill / Border / Angle Satin
			if (StateMap.test(StateFlag::FORMSEL))
				thred::savdo();
			satin::satbrd();
			break;
		case ID_FILCLP: // fill / Border / Clipboard
			form::fclp();
			break;
		case ID_FILIN: // fill / Border / Line
			if (StateMap.test(StateFlag::FORMSEL))
				thred::savdo();
			form::bord();
			break;
		case ID_FRM0: // edit / Set / Form Zero Point
			form::frm0();
			break;
		case ID_REDO: // redo
			redo();
			break;
		case ID_UNFIL: // fill / Unfill
			thred::savdo();
			form::unfil();
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
			break;
		case ID_FORM: // Form
			form::frmon();
			if (StateMap.test(StateFlag::FORMSEL))
				formForms::refrm();
			else
				form::form();
			break;
		case ID_FILSAT: // fill / Fan
			StateMap.reset(StateFlag::FTHR);
			form::filsat();
			break;

		case ID_OPNPCD: // file / Open Auxiliary file
			switch (IniFile.auxFileType) {
			case AUXDST:
				OpenFileName.nFilterIndex = 3;
				break;
			default:
				OpenFileName.nFilterIndex = 2;
			}
			nuFil();
			nulayr(0);
			break;
		case ID_DELSTCH: // edit / Delete / All Stitches
			delstch();
			break;
		case ID_FILL_VERT: // fill / Vertical
			if (StateMap.test(StateFlag::FORMSEL) || SelectedFormList->size())
				thred::savdo();
			form::filvrt();
			break;
		case ID_FILL_HOR: // fill / Horizontal
			if (StateMap.test(StateFlag::FORMSEL))
				thred::savdo();
			form::filhor();
			break;
		case ID_RUNPAT: // view / Movie
			movi();
			break;
		case ID_LENDEF: // Todo - No menu item in thred.rc for this option
			IniFile.maxStitchLength = MAXSIZ * PFGRAN;
			UserStitchLength        = USESIZ * PFGRAN;
			MinStitchLength         = MINSIZ * PFGRAN;
			break;
		case ID_TSIZDEF: // view / Thread Size / Set Defaults
			ThreadSize30 = TSIZ30;
			ThreadSize40 = TSIZ40;
			ThreadSize60 = TSIZ60;
			break;
		case ID_SIZ30: // view / Thread Size / 30
			displayText::tsizmsg(L"30", ThreadSize30);
			StateMap.set(StateFlag::ENTR30);
			break;
		case ID_SIZ40: // view / Thread Size / 40
			displayText::tsizmsg(L"40", ThreadSize40);
			StateMap.set(StateFlag::ENTR40);
			break;
		case ID_SIZ60: // view / Thread Size / 60
			displayText::tsizmsg(L"60", ThreadSize60);
			StateMap.set(StateFlag::ENTR60);
			break;
		case ID_HIDBITF: // file / Hide Bitmap
		case ID_HIDBIT:  // edit / Trace / Hide Bitmap
			thred::hidbit();
			break;
		case ID_LODBIT: // file / Load Bitmap
			lodbmp();
			break;
		case ID_KNOTS: // edit / Set / Knots
			setknots();
			break;
		case ID_REMZERO: // edit / Delete / Small Stitches
			duzero();
			break;
		case ID_ROT: // rot
			rot(rotationCenter);
			break;
		case ZUMIN: // in
			if (StateMap.test(StateFlag::GMRK) || StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::INSRT)
			    || StateMap.test(StateFlag::GRPSEL) || StateMap.test(StateFlag::FORMSEL))
				zumin();
			else {
				StateMap.reset(StateFlag::BZUM);
				StateMap.set(StateFlag::BOXZUM);
				StateMap.reset(StateFlag::BZUMIN);
				StateMap.set(StateFlag::VCAPT);
				SetCapture(ThrEdWindow);
			}
			break;
		case ID_ZUMOUT: // out
			zumout();
			break;
		case ID_FILE_OPEN1: // file / Open
			fop();
			break;
		case ID_VIEW_STCHBAK: // view / Set / Background Color
			if (nuBak()) {
				BackgroundColor    = BackgroundColorStruct.rgbResult;
				BackgroundPen      = nuPen(BackgroundPen, 1, BackgroundColor);
				BackgroundPenWidth = 1;
				DeleteObject(BackgroundBrush);
				BackgroundBrush = CreateSolidBrush(BackgroundColor);
				if (PCSBMPFileName[0])
					bfil();
				thred::zumhom();
			}
			break;
		case ID_BITCOL: // view / Set / Bitmap Color
			if (nuBit()) {
				BitmapColor = fswap(BitMapColorStruct.rgbResult);
				if (PCSBMPFileName[0])
					bfil();
				BitmapPen = nuPen(BitmapPen, 1, BitmapColor);
				thred::zumhom();
			}
			break;
		case ID_FILE_SAVE3: // file / Save As
			colchk();
			savAs();
			break;
		case ID_EDIT_RESET_COL: // edit / Reset Colors
			for (iColor = 0; iColor < 16; iColor++) {
				UserColor[iColor]      = DefaultColors[iColor];
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
				UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
				thred::redraw(UserColorWin[iColor]);
			}
			StateMap.set(StateFlag::RESTCH);
			break;
		case ID_FILE_SAVE2: // file / Save
			colchk();
			thred::save();
			break;
		case VU_ZUMFUL: // view / Zoom Full
			thred::zumhom();
			break;
		case ID_EDIT_SELECTCOLOR: // edit / Select / Color
			selCol();
			break;
		case ID_FILE_NEW1: // file / New
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
			break;
		case ID_BACK: // undo
			bak();
			break;
		case ID_DESNAM: // edit / Set / Designer Name
			getdes();
			break;
		}
		break;
	}
	return 0;
}

// return the width of a text item
int thred::txtWid(const wchar_t* string) {
	GetTextExtentPoint32Int(ThredDC, string, wcslen(string), &TextSize);
	return TextSize.cx;
}

void thred::internal::makCol() noexcept {
	unsigned iColor    = 0;
	wchar_t  buffer[3] = { 0 };

	buffer[1] = L'0';
	buffer[2] = 0;
	for (iColor = 0; iColor < 16; iColor++) {
		DefaultColorWin[iColor] = CreateWindow(L"STATIC",
		                                       0,
		                                       SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                       0,
		                                       ButtonHeight * iColor,
		                                       ButtonWidth,
		                                       ButtonHeight,
		                                       ThrEdWindow,
		                                       NULL,
		                                       ThrEdInstance,
		                                       NULL);

		UserColorWin[iColor] = CreateWindow(L"STATIC",
		                                    0,
		                                    SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                    ButtonWidth,
		                                    ButtonHeight * iColor,
		                                    ButtonWidth,
		                                    ButtonHeight,
		                                    ThrEdWindow,
		                                    NULL,
		                                    ThrEdInstance,
		                                    NULL);

		buffer[0]             = ThreadSize[iColor][0];
		ThreadSizeWin[iColor] = CreateWindow(L"STATIC",
		                                     buffer,
		                                     SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                     ButtonWidth << 1,
		                                     ButtonHeight * iColor,
		                                     ButtonWidth,
		                                     ButtonHeight,
		                                     ThrEdWindow,
		                                     NULL,
		                                     ThrEdInstance,
		                                     NULL);
	}
}

void thred::internal::ritloc() {
	fs::path   lockFilePath;
	PWSTR      ppszPath = nullptr; // variable to receive the path memory block pointer.
	const auto hr       = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, NULL, &ppszPath);

	if (SUCCEEDED(hr)) {
		lockFilePath.assign(ppszPath); // make a local copy of the path
		lockFilePath /= L"ThrEd";
		fs::create_directory(lockFilePath);
		lockFilePath /= L"thredloc.txt";
		auto lockFile = CreateFile(lockFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (lockFile != INVALID_HANDLE_VALUE) {
			DWORD bytesWritten = 0;
			auto  value        = utf::Utf16ToUtf8(*HomeDirectory);
			WriteFileInt(lockFile, value.data(), value.size() + 1, &bytesWritten, 0);
			CloseHandle(lockFile);
		}
	}
	CoTaskMemFree(ppszPath); // free up the path memory block
}

void thred::internal::crtcurs() noexcept {
	FormCursor            = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Form));
	DLineCursor           = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_DLIN));
	NeedleUpCursor        = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Upright));
	NeedleRightDownCursor = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightDown));
	NeedleRightUpCursor   = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightUp));
	NeedleLeftDownCursor  = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftDown));
	NeedleLeftUpCursor    = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftUp));
}

void thred::internal::duhom() {
	fs::path arg0 = { ArgList[0] };

	*HomeDirectory = arg0.parent_path();
}

void thred::internal::ducmd() {
	unsigned long bytesRead = 0;
	fs::path      balaradFileName;

	if (ArgCount > 1) {
		std::wstring arg1 = { ArgList[1] };
		if (!arg1.compare(0, 4, L"/F1:")) {
			balaradFileName = *HomeDirectory / arg1.substr(4);
			BalaradFile     = CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			if (BalaradFile != INVALID_HANDLE_VALUE) {
				CloseHandle(BalaradFile);
				*BalaradName0 = balaradFileName;
				if (ArgCount > 2) {
					std::wstring arg2 = { ArgList[2] };
					if (!arg2.compare(0, 4, L"/F2:")) {
						balaradFileName = *HomeDirectory / arg2.substr(4);
						BalaradFile     = CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
						if (BalaradFile != INVALID_HANDLE_VALUE) {
							*BalaradName1                     = balaradFileName;
							wchar_t readBuffer[_MAX_PATH + 1] = { 0 };
							ReadFile(BalaradFile, &readBuffer, (_MAX_PATH + 1), &bytesRead, 0);
							if (bytesRead) {
								BalaradName2->assign(readBuffer);
								redbal();
							}
							CloseHandle(BalaradFile);
						}
					}
				}
				SetWindowText(ThrEdWindow, (*StringTable)[STR_EMB].c_str());
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

void thred::internal::redini() {
	unsigned      iColor = 0, adjustedWidth = 0;
	unsigned long bytesRead     = 0;
	HDC           deviceContext = {};

	duhom();
	*IniFileName = *HomeDirectory;
	*IniFileName /= L"thred.ini";
	IniFileHandle = CreateFile(IniFileName->wstring().c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (IniFileHandle == INVALID_HANDLE_VALUE) {
		defpref();
		getDocsFolder(DefaultDirectory);
		if (!DesignerName->size()) {
			wchar_t designerBuffer[50];
			LoadString(ThrEdInstance, IDS_UNAM, designerBuffer, sizeof(designerBuffer) / sizeof(designerBuffer[0]));
			DesignerName->assign(designerBuffer);
			getdes();
		}
	}
	else {
		ReadFile(IniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, 0);
		if (bytesRead < sizeof(IniFile))
			IniFile.formBoxSizePixels = DEFBPIX;
		auto directory = utf::Utf8ToUtf16(IniFile.defaultDirectory);
		DefaultDirectory->assign(directory);
		DefaultBMPDirectory->assign(directory);
		auto& previousNames = *PreviousNames;
		for (auto iVersion = 0; iVersion < OLDNUM; iVersion++) {
			if (strlen(IniFile.prevNames[iVersion])) {
				previousNames[iVersion].assign(utf::Utf8ToUtf16(std::string(IniFile.prevNames[iVersion])));
			}
			else {
				previousNames[iVersion].clear();
			}
		}
		DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
		for (iColor = 0; iColor < 16; iColor++) {
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
		if (IniFile.showStitchThreshold > 9) {
			IniFile.showStitchThreshold = 9;
		}
		ShowStitchThreshold = IniFile.showStitchThreshold;
		if (IniFile.threadSize30)
			ThreadSize30 = IniFile.threadSize30;
		if (IniFile.threadSize40)
			ThreadSize40 = IniFile.threadSize40;
		if (IniFile.threadSize60)
			ThreadSize60 = IniFile.threadSize60;
		if (IniFile.userStitchLength)
			UserStitchLength = IniFile.userStitchLength;
		if (!IniFile.maxStitchLength)
			IniFile.maxStitchLength = MAXSIZ * PFGRAN;
		if (IniFile.smallStitchLength)
			SmallStitchLength = IniFile.smallStitchLength;
		StitchBoxesThreshold = IniFile.stitchBoxesThreshold;
		if (IniFile.stitchSpace)
			LineSpacing = IniFile.stitchSpace;
		{
			const EnumMap<UserFlag> tmp(IniFile.userFlagMap);
			UserFlagMap = tmp;
		}
		if (IniFile.borderWidth)
			BorderWidth = IniFile.borderWidth;
		if (IniFile.appliqueColor)
			AppliqueColor = IniFile.appliqueColor & 0xf;
		if (IniFile.snapLength)
			SnapLength = IniFile.snapLength;
		if (IniFile.starRatio)
			StarRatio = IniFile.starRatio;
		if (IniFile.spiralWrap)
			SpiralWrap = IniFile.spiralWrap;
		if (IniFile.buttonholeCornerLength)
			ButtonholeCornerLength = IniFile.buttonholeCornerLength;
		if (!IniFile.gridSize)
			IniFile.gridSize = 12;
		if (!IniFile.wavePoints)
			IniFile.wavePoints = IWAVPNTS;
		if (!IniFile.waveStart)
			IniFile.waveStart = IWAVSTRT;
		if (!IniFile.waveEnd)
			IniFile.waveEnd = IWAVEND;
		if (!IniFile.waveLobes)
			IniFile.waveLobes = IWAVS;
		if (!IniFile.featherFillType)
			IniFile.featherFillType = FDEFTYP;
		if (!IniFile.featherUpCount)
			IniFile.featherUpCount = FDEFUP;
		if (!IniFile.featherDownCount)
			IniFile.featherDownCount = FDEFDWN;
		if (!IniFile.featherRatio)
			IniFile.featherRatio = FDEFRAT;
		if (!IniFile.featherMinStitchSize)
			IniFile.featherMinStitchSize = FDEFFLR;
		if (!IniFile.featherCount)
			IniFile.featherCount = FDEFNUM;
		if (!IniFile.daisyHoleDiameter)
			IniFile.daisyHoleDiameter = DAZHLEN;
		if (!IniFile.daisyPetalPoints)
			IniFile.daisyPetalPoints = DAZCNT;
		if (!IniFile.daisyInnerCount)
			IniFile.daisyInnerCount = DAZICNT;
		if (!IniFile.daisyDiameter)
			IniFile.daisyDiameter = DAZLEN;
		if (!IniFile.daisyPetalCount)
			IniFile.daisyPetalCount = DAZPETS;
		if (!IniFile.daisyPetalLen)
			IniFile.daisyPetalLen = DAZPLEN;

		switch (IniFile.hoopType) {
		case SMALHUP:
			IniFile.hoopSizeX = SHUPX;
			IniFile.hoopSizeY = SHUPY;
			break;
		case LARGHUP:
			IniFile.hoopSizeX = LHUPX;
			IniFile.hoopSizeY = LHUPY;
			break;
		case CUSTHUP:
			if (!IniFile.hoopSizeX)
				IniFile.hoopSizeX = LHUPX;
			if (!IniFile.hoopSizeY)
				IniFile.hoopSizeY = LHUPY;
			break;
		case HUP100:
			if (!IniFile.hoopSizeX)
				IniFile.hoopSizeX = HUP100XY;
			if (!IniFile.hoopSizeY)
				IniFile.hoopSizeY = HUP100XY;
			break;
		default:
			IniFile.hoopType  = LARGHUP;
			IniFile.hoopSizeX = LHUPX;
			IniFile.hoopSizeY = LHUPY;
		}
		UnzoomedRect.x = IniFile.hoopSizeX;
		UnzoomedRect.y = IniFile.hoopSizeY;
		PicotSpacing   = IniFile.picotSpace;
	}
	if (!IniFile.gridColor)
		IniFile.gridColor = DEFGRD;
	CloseHandle(IniFileHandle);
	if (!IniFile.fillAngle)
		IniFile.fillAngle = PI / 6;
	deviceContext       = GetDC(0);
	ScreenSizePixels.cx = GetDeviceCaps(deviceContext, HORZRES);
	ScreenSizePixels.cy = GetDeviceCaps(deviceContext, VERTRES);
	if (IniFile.initialWindowCoords.left < 0)
		IniFile.initialWindowCoords.left = 0;
	if (IniFile.initialWindowCoords.top < 0)
		IniFile.initialWindowCoords.top = 0;
	adjustedWidth = ScreenSizePixels.cx - 30;
	if (IniFile.initialWindowCoords.right > gsl::narrow<int>(adjustedWidth))
		IniFile.initialWindowCoords.right = adjustedWidth;
	if (IniFile.initialWindowCoords.bottom > ScreenSizePixels.cy)
		IniFile.initialWindowCoords.bottom = ScreenSizePixels.cy;
}

void thred::internal::chkirct() noexcept {
	POINT screenLimits;

	screenLimits.x = ScreenSizePixels.cx - 1;
	screenLimits.y = ScreenSizePixels.cy - 1;
	if (IniFile.initialWindowCoords.left > screenLimits.x)
		IniFile.initialWindowCoords.left = screenLimits.x;
	if (IniFile.initialWindowCoords.right > screenLimits.x)
		IniFile.initialWindowCoords.right = screenLimits.x;
	if (IniFile.initialWindowCoords.left < 0)
		IniFile.initialWindowCoords.left = 0;
	if (IniFile.initialWindowCoords.right < 0)
		IniFile.initialWindowCoords.right = 0;
	if (IniFile.initialWindowCoords.top > screenLimits.y)
		IniFile.initialWindowCoords.top = screenLimits.y;
	if (IniFile.initialWindowCoords.bottom > screenLimits.y)
		IniFile.initialWindowCoords.bottom = screenLimits.y;
	if (IniFile.initialWindowCoords.top < 0)
		IniFile.initialWindowCoords.top = 0;
	if (IniFile.initialWindowCoords.bottom < 0)
		IniFile.initialWindowCoords.bottom = 0;
	if (IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left < 300) {
		IniFile.initialWindowCoords.left  = 0;
		IniFile.initialWindowCoords.right = screenLimits.x >> 1;
	}
	if (IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top < 300) {
		IniFile.initialWindowCoords.top    = 0;
		IniFile.initialWindowCoords.bottom = screenLimits.y >> 1;
	}
}

void thred::internal::init() {
	unsigned            iButton = 0, iRGBK = 0, iByte = 0, flag = 0, iColor = 0, iOffset = 0, iMenu = 0;
	unsigned long       thwid           = 0;
	long                offsetStepSize  = 0;
	RECT                tRct            = {};
	RECT                wrct            = {};
	HDC                 deviceContext   = GetDC(NULL);
	const unsigned long screenHalfWidth = (GetDeviceCaps(deviceContext, HORZRES)) >> 1;
	std::wstring        blank(L"");
	std::wstring*       buttonTxt = &blank;

	ReleaseDC(NULL, deviceContext);
	TextureIndex = 0;
	LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1));
	MainMenu = GetMenu(ThrEdWindow);
	GetWindowRect(ThrEdWindow, &wrct);
	GetMenuItemRect(ThrEdWindow, MainMenu, 0, &tRct);
	wrct.left  = tRct.left;
	wrct.right = tRct.right;
	for (iMenu = 0; iMenu <= M_HELP; iMenu++) {
		GetMenuItemRect(ThrEdWindow, MainMenu, iMenu, &tRct);
		wrct.right += (tRct.right - tRct.left);
	}
	wrct.right += 20;
	thwid = wrct.right - wrct.left;
	if (thwid < screenHalfWidth)
		wrct.right = wrct.left += screenHalfWidth;
	FillMenu       = GetSubMenu(MainMenu, M_FILL);
	FileMenu       = GetSubMenu(MainMenu, M_FILE);
	EditMenu       = GetSubMenu(MainMenu, M_EDIT);
	BorderFillMenu = GetSubMenu(FillMenu, MFIL_BORD);
	ViewMenu       = GetSubMenu(MainMenu, M_VIEW);
	ViewSetMenu    = GetSubMenu(ViewMenu, MVW_SET);
	qchk();
	ArrowCursor = LoadCursor(0, IDC_ARROW);
	CrossCursor = LoadCursor(0, IDC_CROSS);
	crtcurs();
	redfils();
	StateMap.reset(); // clear the bitmap
	// set up the size variables
	ThredDC = GetDC(ThrEdWindow);
	SetStretchBltMode(ThredDC, COLORONCOLOR);
	StitchWindowDC    = GetDCEx(MainStitchWin, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
	ScreenSizeMM.cx   = GetDeviceCaps(ThredDC, HORZSIZE);
	ScreenSizeMM.cy   = GetDeviceCaps(ThredDC, VERTSIZE);
	chkirct();
	if (!UserFlagMap.test(UserFlag::SAVMAX))
		MoveWindow(ThrEdWindow,
		           IniFile.initialWindowCoords.left,
		           IniFile.initialWindowCoords.top,
		           IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left,
		           IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top,
		           0);
	ButtonWidth    = thred::txtWid(L"MM") + TXTSIDS;
	ButtonWidthX3  = ButtonWidth * 3;
	ButtonHeight   = TextSize.cy + 4;
	offsetStepSize = thred::txtWid(L"0");
	for (iOffset = 0; iOffset < NERCNT; iOffset++)
		BoxOffset[iOffset] = offsetStepSize * (iOffset + 1);
	GetClientRect(ThrEdWindow, &ThredWindowRect);
	stchWnd();
	displayText::lodstr();
	formForms::maxwid(STR_PRF0, STR_PRF27);
	if (!IniFileHandle) {
		// initialize the user color and thread size arrays
		for (iColor = 0; iColor < 16; iColor++)
			UserColor[iColor] = DefaultColors[iColor];
	}
	for (iColor = 0; iColor < 16; iColor++) {
		ThreadSize[iColor][0] = '4';
		ThreadSize[iColor][1] = '0';
	}
	if (!IniFile.traceLength)
		IniFile.traceLength = TRACLEN;
	if (!IniFile.traceRatio)
		IniFile.traceRatio = TRACRAT;
	if (!IniFile.chainSpace)
		IniFile.chainSpace = CHSDEF;
	if (!IniFile.chainRatio)
		IniFile.chainRatio = CHRDEF;
	if (!IniFile.cursorNudgeStep)
		IniFile.cursorNudgeStep = NUGINI;
	if (!IniFile.nudgePixels)
		IniFile.nudgePixels = DEFPIX;
	if (!IniFile.eggRatio)
		IniFile.eggRatio = DEFEGRAT;
	if (!IniFile.stitchSizePixels)
		IniFile.stitchSizePixels = DEFPNTPIX;
	if (!IniFile.formVertexSizePixels)
		IniFile.formVertexSizePixels = DEFPNTPIX;
	if (!IniFile.formSides)
		IniFile.formSides = 24;
	if (!IniFile.tearTailLength)
		IniFile.tearTailLength = 1.4f;
	if (!IniFile.underlayStitchLen)
		IniFile.underlayStitchLen = DEFULEN;
	if (!IniFile.underlaySpacing)
		IniFile.underlaySpacing = DEFUSPAC;
	setgrd(IniFile.gridColor);
	makCol(); // make the color change windows
	ButtonWin->resize(9);
	for (iButton = 0; iButton < 9; iButton++) {
		switch (iButton) {
		case HBOXSEL:
			flag      = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonTxt = &(*StringTable)[STR_BOXSEL];
			break;
		case HUPTO:
			flag      = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonTxt = &(*StringTable)[STR_UPOF];
			break;
		case HHID:
			flag      = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonTxt = &(*StringTable)[STR_PIKOL];
			break;
		default:
			buttonTxt = &blank;
			flag      = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		}
		if (buttonTxt) {
			(*ButtonWin)[iButton] = CreateWindow(L"STATIC",
			                                     buttonTxt->c_str(),
			                                     flag,
			                                     0,
			                                     ButtonHeight * (16 + iButton),
			                                     ButtonWidthX3,
			                                     ButtonHeight,
			                                     ThrEdWindow,
			                                     NULL,
			                                     ThrEdInstance,
			                                     NULL);
		}
	}
	trace::initTraceWindows();
	ColorBar = CreateWindow(L"STATIC",
	                        L"",
	                        SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                        ThredWindowRect.right - COLSIZ,
	                        0,
	                        COLSIZ,
	                        ThredWindowRect.bottom,
	                        ThrEdWindow,
	                        NULL,
	                        ThrEdInstance,
	                        NULL);
	nuRct();
	// create pens
	for (iRGBK = 0; iRGBK < 4; iRGBK++)
		BoxPen[iRGBK] = CreatePen(PS_SOLID, 1, BoxColor[iRGBK]);
	LinePen            = CreatePen(PS_SOLID, 1, 0x404040);
	CrossPen           = CreatePen(PS_SOLID, 5, 0x804080);
	GroupSelectPen     = CreatePen(PS_SOLID, 1, 0x804080);
	GridPen            = CreatePen(PS_SOLID, 1, IniFile.gridColor);
	BackgroundPen      = CreatePen(PS_SOLID, 3, BackgroundColor);
	BitmapPen          = CreatePen(PS_SOLID, 1, BitmapColor);
	FormPen            = CreatePen(PS_SOLID, 1, 0xc0c0c0);
	MultiFormPen       = CreatePen(PS_SOLID, 1, 0xc0c080);
	FormPen3px         = CreatePen(PS_SOLID, 3, 0xc0c0c0);
	FormSelectedPen    = CreatePen(PS_SOLID, 1, 0x80c080);
	ZoomMarkPen        = CreatePen(PS_SOLID, 3, 0x40c040);
	SelectAllPen       = CreatePen(PS_SOLID, 1, 0xc08080);
	KnotPen            = CreatePen(PS_SOLID, 1, 0xffffff);
	LayerPen[0]        = CreatePen(PS_SOLID, 1, 0xc0c0c0);
	LayerPen[1]        = CreatePen(PS_SOLID, 1, 0x80c0c0);
	LayerPen[2]        = CreatePen(PS_SOLID, 1, 0xc080c0);
	LayerPen[3]        = CreatePen(PS_SOLID, 1, 0xc0c080);
	LayerPen[4]        = CreatePen(PS_SOLID, 1, 0x40c0c0);
	LayerPen[5]        = CreatePen(PS_SOLID, 1, 0xc0c040);
	BackgroundPenWidth = 1;
	for (iColor = 0; iColor < 16; iColor++) {
		ThreadSizePixels[iColor] = 1;
		ThreadSizeIndex[iColor]  = 1;
		UserPen[iColor]          = CreatePen(PS_SOLID, 1, UserColor[iColor]);
	}
	BackgroundBrush = CreateSolidBrush(BackgroundColor);
	// create brushes
	for (iColor = 0; iColor < 16; iColor++) {
		DefaultColorBrush[iColor] = CreateSolidBrush(DefaultColors[iColor]);
		UserColorBrush[iColor]    = CreateSolidBrush(UserColor[iColor]);
	}
	ZoomFactor            = 1;
	PCSHeader.leadIn      = 0x32;
	PCSHeader.colorCount  = 16;
	PCSHeader.stitchCount = 0;
	GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
	ladj();
	GetTextExtentPointInt(
	    StitchWindowMemDC, (*StringTable)[STR_PIKOL].c_str(), (*StringTable)[STR_PIKOL].size(), &PickColorMsgSize);
	auxmen();
	fnamtabs();
	ritfnam(*DesignerName);
	auto designer = utf::Utf16ToUtf8(*DesignerName);
	std::copy(designer.begin(), designer.end(), ExtendedHeader.modifierName);
	ExtendedHeader.stgran = 0;
	for (iByte = 0; iByte < RES_SIZE; iByte++)
		ExtendedHeader.res[iByte] = 0;
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
	SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRED], *DesignerName).c_str());
}

COLORREF thred::internal::defTxt(unsigned iColor) {
	const std::bitset<16> textColorMap(
	    0xbaf); // bitmap for color number colors chosen for contrast against the default background colors
	const unsigned int white = 0xffffff;
	const unsigned int black = 0;
	return textColorMap.test(iColor) ? white : black;
}

void thred::internal::relin() {
	stch2px1(ClosestPointIndex - 1);
	MoveLine0[0] = StitchCoordinatesPixels;
	stch2px1(ClosestPointIndex + 1);
	MoveLine1[1] = StitchCoordinatesPixels;
	StateMap.reset(StateFlag::WASLIN);
	dulin();
}

void thred::internal::dumov() {
	const fPOINTATTR* anchorStitch         = &StitchBuffer[MoveAnchor];
	POINT             OffsetFromCenter     = {};
	POINT             rotationCenterPixels = {};
	POINT             rotationOutline[8]   = {};

	RotateAngle = atan2(StitchBuffer[MoveAnchor + 1].y - StitchBuffer[MoveAnchor].y,
	                    StitchBuffer[MoveAnchor + 1].x - StitchBuffer[MoveAnchor].x);
	if (anchorStitch->x >= ZoomRect.left && anchorStitch->x <= ZoomRect.right && anchorStitch->y >= ZoomRect.bottom
	    && anchorStitch->y <= ZoomRect.top) {
		sdCor2px(StitchBuffer[MoveAnchor], rotationCenterPixels);
		rotationOutline[0] = rotationOutline[6] = rotationCenterPixels;
		OffsetFromCenter.x                      = rotationCenterPixels.x + 12;
		OffsetFromCenter.y                      = rotationCenterPixels.y + 2;
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
		Polyline(StitchWindowMemDC, rotationOutline, 7);
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

unsigned thred::internal::chkup(unsigned count, unsigned iStitch) {
	if (StateMap.test(StateFlag::UPTO) && ClosestPointIndex) {
		if (ColorChangeTable[iStitch].stitchIndex < ClosestPointIndex) {
			if (ColorChangeTable[iStitch + 1].stitchIndex < ClosestPointIndex)
				return count;
			else
				return ClosestPointIndex - ColorChangeTable[iStitch].stitchIndex + 1;
		}
		else
			return 0;
	}
	else
		return count;
}

bool thred::internal::bitar() {
	dRECTANGLE backingRect       = {};
	dRECTANGLE differenceRect    = {};
	dPOINT     bitmapStitchRatio = {};
	dRECTANGLE zoomedInRect
	    = { (UnzoomedRect.y - ZoomRect.top), (UnzoomedRect.y - ZoomRect.bottom), ZoomRect.left, ZoomRect.right };

	if (zoomedInRect.top > BitmapSizeinStitches.y || zoomedInRect.left > BitmapSizeinStitches.x)
		return 0;
	else {
		BitmapSrcRect.top    = ceil(zoomedInRect.top * BmpStitchRatio.y);
		BitmapSrcRect.left   = ceil(ZoomRect.left * BmpStitchRatio.x);
		BitmapSrcRect.right  = floor(ZoomRect.right * BmpStitchRatio.x);
		BitmapSrcRect.bottom = floor(zoomedInRect.bottom * BmpStitchRatio.y);
		if (BitmapSrcRect.right > gsl::narrow<long>(BitmapWidth)) {
			BitmapSrcRect.right = BitmapWidth;
			StateMap.reset(StateFlag::LANDSCAP);
		}
		if (BitmapSrcRect.bottom > gsl::narrow<long>(BitmapHeight)) {
			BitmapSrcRect.bottom = BitmapHeight;
			StateMap.set(StateFlag::LANDSCAP);
		}
		backingRect.top       = BitmapSrcRect.top * StitchBmpRatio.y;
		backingRect.left      = BitmapSrcRect.left * StitchBmpRatio.x;
		backingRect.right     = BitmapSrcRect.right * StitchBmpRatio.x;
		backingRect.bottom    = BitmapSrcRect.bottom * StitchBmpRatio.y;
		differenceRect.top    = backingRect.top - zoomedInRect.top;
		differenceRect.left   = backingRect.left - zoomedInRect.left;
		differenceRect.right  = zoomedInRect.right - backingRect.right;
		differenceRect.bottom = zoomedInRect.bottom - backingRect.bottom;
		bitmapStitchRatio.x   = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
		bitmapStitchRatio.y   = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
		BitmapDstRect.top     = differenceRect.top * bitmapStitchRatio.y;
		BitmapDstRect.left    = differenceRect.left * bitmapStitchRatio.x;
		BitmapDstRect.right   = StitchWindowClientRect.right - differenceRect.right * bitmapStitchRatio.x;
		BitmapDstRect.bottom  = StitchWindowClientRect.bottom - differenceRect.bottom * bitmapStitchRatio.y;
		return 1;
	}
}

inline void thred::internal::stCor2px(const fPOINTATTR& stitch, POINT& point) noexcept {
	point.x = (stitch.x - ZoomRect.left) * ZoomRatio.x + 0.5;
	point.y = StitchWindowClientRect.bottom - (stitch.y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
}

void thred::internal::drwknot() {
#define KSIZ 5
#define KLSIZ 10

	unsigned ind     = 0;
	POINT    point   = {};
	POINT    tlin[5] = {};

	if (!UserFlagMap.test(UserFlag::KNOTOF) && KnotCount && PCSHeader.stitchCount) {
		for (ind = 0; ind < KnotCount; ind++) {
			stCor2px(StitchBuffer[Knots[ind]], point);
			SelectObject(StitchWindowMemDC, KnotPen);
			SetROP2(StitchWindowMemDC, R2_XORPEN);
			tlin[0].x = tlin[3].x = tlin[4].x = point.x - KSIZ;
			tlin[1].x = tlin[2].x = point.x + KSIZ;
			tlin[0].y = tlin[1].y = tlin[4].y = point.y + KSIZ;
			tlin[2].y = tlin[3].y = point.y - KSIZ;
			Polyline(StitchWindowMemDC, tlin, 5);
			tlin[0].x = point.x - KLSIZ;
			tlin[1].x = point.x + KLSIZ;
			tlin[0].y = tlin[1].y = point.y;
			Polyline(StitchWindowMemDC, tlin, 2);
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		}
	}
}

void thred::internal::dugrid() {
	POINT    gridLine[2] = {};
	RECT     gridRect    = {};
	unsigned iGrid       = 0;

	if (ZoomFactor < ShowStitchThreshold || !ShowStitchThreshold) {
		SetROP2(StitchWindowMemDC, R2_XORPEN);
		SelectObject(StitchWindowMemDC, GridPen);
		gridRect.left   = ceil(ZoomRect.left / IniFile.gridSize);
		gridRect.right  = floor(ZoomRect.right / IniFile.gridSize);
		gridRect.bottom = ceil(ZoomRect.bottom / IniFile.gridSize);
		gridRect.top    = floor(ZoomRect.top / IniFile.gridSize);
		gridLine[0].x   = 0;
		gridLine[1].x   = StitchWindowClientRect.right;
		for (iGrid = gridRect.bottom; iGrid <= gsl::narrow<unsigned>(gridRect.top); iGrid++) {
			gridLine[0].y = gridLine[1].y
			    = StitchWindowClientRect.bottom - (iGrid * IniFile.gridSize - ZoomRect.bottom) * ZoomRatio.y + 0.5;
			Polyline(StitchWindowMemDC, gridLine, 2);
		}
		gridLine[0].y = 0;
		gridLine[1].y = StitchWindowClientRect.bottom;
		for (iGrid = gridRect.left; iGrid <= gsl::narrow<unsigned>(gridRect.right); iGrid++) {
			gridLine[0].x = gridLine[1].x = (iGrid * IniFile.gridSize - ZoomRect.left) * ZoomRatio.x + 0.5;
			Polyline(StitchWindowMemDC, gridLine, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void thred::internal::rint() noexcept {
	const unsigned size = StitchWindowClientRect.right * StitchWindowClientRect.bottom;

	CellSize.x = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
	CellSize.y = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
}

bool thred::internal::setRmap(boost::dynamic_bitset<>& stitchMap, const fPOINTATTR& stitchPoint) {
	unsigned bitPoint;

	bitPoint = floor((stitchPoint.x - ZoomRect.left) / CellSize.x) * floor((stitchPoint.y - ZoomRect.bottom) / CellSize.y);
	return !stitchMap.test_set(bitPoint);
}

void thred::internal::drwStch() {
	unsigned   iColor = 0, iStitch = 0, stitchCount = 0, pwid = 0, wascol = 0;
	unsigned   layer = 0;
	double     slope = 0.0, offset = 0.0;
	double     dub6          = 0.0;
	POINT      stitchLine[2] = {};
	const long maxYcoord     = DrawItem->rcItem.bottom;
	long       xDelta = 0, yDelta = 0, gapToEdge = 0;
	HDC        deviceContext = {};

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
	for (iColor = 0; iColor < ColorChanges; iColor++) {
		LineIndex = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
		if (LineIndex > stitchCount)
			stitchCount = LineIndex;
	}
	std::vector<POINT> linePoints(stitchCount + 2);
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	thred::duzrat();
	if (PCSBMPFileName[0] && !StateMap.test(StateFlag::HIDMAP) && !StateMap.test(StateFlag::UPTO)) {
		if (StateMap.test(StateFlag::WASTRAC))
			deviceContext = TraceDC;
		else
			deviceContext = BitmapDC;
		if (bitar())
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
	dugrid();
	if (StateMap.test(StateFlag::INIT)) {
		if (StateMap.test(StateFlag::ZUMED)) {
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask  = SIF_ALL;
			ScrollInfo.nMax   = UnzoomedRect.y;
			ScrollInfo.nMin   = 0;
			ScrollInfo.nPage  = ZoomRect.top - ZoomRect.bottom;
			ScrollInfo.nPos   = UnzoomedRect.y - ZoomRect.top;
			SetScrollInfo(VerticalScrollBar, SB_CTL, &ScrollInfo, TRUE);
			ScrollInfo.nMax  = UnzoomedRect.x;
			ScrollInfo.nPage = ZoomRect.right - ZoomRect.left;
			ScrollInfo.nPos  = ZoomRect.left;
			SetScrollInfo(HorizontalScrollBar, SB_CTL, &ScrollInfo, TRUE);
			ShowWindow(HorizontalScrollBar, TRUE);
			ShowWindow(VerticalScrollBar, TRUE);
		}
		else {
			ShowWindow(VerticalScrollBar, FALSE);
			ShowWindow(HorizontalScrollBar, FALSE);
		}
		thred::duzrat();
		dub6                 = ZoomRatio.x * 6;
		ThreadWidthPixels[0] = dub6 * TSIZ30;
		ThreadWidthPixels[1] = dub6 * TSIZ40;
		ThreadWidthPixels[2] = dub6 * TSIZ60;
		for (iColor = 0; iColor < 16; iColor++) {
			if (StateMap.test(StateFlag::THRDS))
				nuStchSiz(iColor, ThreadWidthPixels[ThreadSizeIndex[iColor]]);
			else {
				if (StateMap.test(StateFlag::COL)) {
					if (iColor == ActiveColor)
						nuStchSiz(iColor, ThreadWidthPixels[ThreadSizeIndex[iColor]]);
					else
						nuStchSiz(iColor, 1);
				}
				else
					nuStchSiz(iColor, 1);
			}
		}
		LineIndex = 0;
		DisplayedColorBitmap.reset();
		if (StateMap.test(StateFlag::ZUMED)) {
			LineIndex = 0;
			StateMap.reset(StateFlag::LINED);
			StateMap.reset(StateFlag::LININ);
			for (iColor = 0; iColor < ColorChanges; iColor++) {
				if (StateMap.test(StateFlag::HID)) {
					if (ColorChangeTable[iColor].colorIndex != ActiveColor) {
						stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
						const fPOINTATTR* currentStitches = &StitchBuffer[ColorChangeTable[iColor].stitchIndex];
						if (currentStitches) {
							for (iStitch = 0; iStitch < stitchCount; iStitch++) {
								if (currentStitches[iStitch].x >= ZoomRect.left && currentStitches[iStitch].x <= ZoomRect.right
								    && currentStitches[iStitch].y >= ZoomRect.bottom
								    && currentStitches[iStitch].y <= ZoomRect.top) {
									DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
									break;
								}
							}
							break;
						}
						else {
							throw;
						}
					}
					else {
						wascol = 0;
					}
				}
				else {
					wascol = 0;
				}
				SelectObject(StitchWindowMemDC, UserPen[ColorChangeTable[iColor].colorIndex]);
				stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
				const fPOINTATTR* currentStitches = &StitchBuffer[ColorChangeTable[iColor].stitchIndex];
				if (currentStitches) {
					stitchCount = chkup(stitchCount, iColor);
					for (iStitch = 0; iStitch < stitchCount; iStitch++) {
						layer = (currentStitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
						if (!ActiveLayer || !layer || (layer == ActiveLayer)) {
							if (currentStitches[iStitch].x >= ZoomRect.left && currentStitches[iStitch].x <= ZoomRect.right
							    && currentStitches[iStitch].y >= ZoomRect.bottom && currentStitches[iStitch].y <= ZoomRect.top) {
								wascol = 1;
								if (StateMap.testAndSet(StateFlag::LINED)) {
									if (StateMap.testAndSet(StateFlag::LININ)) {
										linePoints[LineIndex].x
										    = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
										linePoints[LineIndex++].y
										    = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
									}
									else {
										if (iStitch == 0) {
											linePoints[LineIndex].x
											    = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
											linePoints[LineIndex++].y
											    = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
										}
										else {
											linePoints[LineIndex].x
											    = (currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5;
											linePoints[LineIndex++].y
											    = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y
											      + 0.5;
											linePoints[LineIndex].x
											    = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
											linePoints[LineIndex++].y
											    = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
										}
									}
								}
								else {
									if (iStitch == 0) {
										linePoints[0].x = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
										linePoints[0].y
										    = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
										LineIndex = 1;
									}
									else {
										linePoints[0].x = (currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5;
										linePoints[0].y
										    = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
										linePoints[1].x = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
										linePoints[1].y
										    = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
										LineIndex = 2;
									}
									StateMap.set(StateFlag::LININ);
								}
							}
							else {
								if (StateMap.testAndReset(StateFlag::LININ)) {
									linePoints[LineIndex].x = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
									linePoints[LineIndex++].y
									    = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5;
									PolylineInt(StitchWindowMemDC, linePoints.data(), LineIndex);
									LineIndex = 0;
								}
								else {
									if (iStitch) {
										// write an equation for this line
										xDelta = currentStitches[iStitch].x - currentStitches[iStitch - 1].x;
										yDelta = currentStitches[iStitch - 1].y - currentStitches[iStitch].y;
										slope  = static_cast<double>(xDelta) / yDelta;
										offset = currentStitches[iStitch].x + slope * currentStitches[iStitch].y;
										do {
											// does the line intersect with the top of the screen?
											gapToEdge = offset - slope * ZoomRect.top;
											if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
												stitchLine[0].x
												    = (currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5;
												stitchLine[0].y
												    = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.x
												      + 0.5;
												stitchLine[1].x
												    = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
												stitchLine[1].y = maxYcoord
												                  - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.x
												                  + 0.5;
												Polyline(StitchWindowMemDC, stitchLine, 2);
												break;
											}
											// does the line intersect the bottom of the screen?
											gapToEdge = offset - slope * ZoomRect.bottom;
											if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
												stitchLine[0].x
												    = (currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5;
												stitchLine[0].y
												    = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y
												      + 0.5;
												stitchLine[1].x
												    = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
												stitchLine[1].y = maxYcoord
												                  - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y
												                  + 0.5;
												Polyline(StitchWindowMemDC, stitchLine, 2);
												break;
											}
											// does the line intersect the left side of the screen?
											if (slope) {
												gapToEdge = (offset - ZoomRect.left) / slope;
												if (gapToEdge >= ZoomRect.bottom && gapToEdge <= ZoomRect.top) {
													stitchLine[0].x
													    = (currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5;
													stitchLine[0].y
													    = maxYcoord
													      - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y
													      + 0.5;
													stitchLine[1].x
													    = (currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5;
													stitchLine[1].y
													    = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y
													      + 0.5;
													Polyline(StitchWindowMemDC, stitchLine, 2);
												}
											}
										} while (false);
									}
								}
							}
						}
					}
				}
				else {
					throw;
				}
				if (LineIndex) {
					PolylineInt(StitchWindowMemDC, linePoints.data(), LineIndex);
					linePoints[0] = linePoints[LineIndex - 1];
					LineIndex     = 1;
				}
				if (wascol)
					DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
			}
		}
		else {
			pwid = StateMap.test(StateFlag::HID);
			for (iColor = 0; iColor < ColorChanges; iColor++) {
				DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
				stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
				stitchCount = chkup(stitchCount, iColor);
				if (!pwid || (pwid && ColorChangeTable[iColor].colorIndex == ActiveColor))
					drwLin(linePoints,
					       ColorChangeTable[iColor].stitchIndex,
					       stitchCount,
					       UserPen[ColorChangeTable[iColor].colorIndex]);
			}
		}
		if (StateMap.test(StateFlag::SELBOX)) {
			ritcor(StitchBuffer[ClosestPointIndex]);
			if (stch2px(ClosestPointIndex)) {
				dubox();
			}
		}
		if (StateMap.test(StateFlag::FRMPSEL))
			thred::ritfcor(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
		if (!StateMap.test(StateFlag::SELBOX) && !StateMap.test(StateFlag::FRMPSEL)) {
			const std::wstring blank(L"");
			displayText::butxt(HCOR, blank);
		}
		if (StateMap.test(StateFlag::WASLIN))
			relin();
		if (StateMap.test(StateFlag::GRPSEL)) {
			if (cmpstch(ClosestPointIndex, GroupStitchIndex))
				cros(ClosestPointIndex);
			else {
				SearchLineIndex = 0;
				ducros(StitchWindowMemDC);
			}
			thred::selRct(StitchRangeRect);
			nuselrct();
			StateMap.set(StateFlag::SELSHO);
			dusel(StitchWindowMemDC);
		}
		if (ZoomFactor < StitchBoxesThreshold) {
			rint();
			const auto maxMapSize = StitchWindowClientRect.right * StitchWindowClientRect.bottom;

			boost::dynamic_bitset<> stitchMap(0);
			stitchMap.resize(maxMapSize, false);
			SelectObject(StitchWindowMemDC, LinePen);
			SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
			if (StateMap.test(StateFlag::HID)) {
				for (iColor = 0; iColor < ColorChanges; iColor++) {
					if (ColorChangeTable[iColor].colorIndex == ActiveColor) {
						for (iStitch = ColorChangeTable[iColor].stitchIndex; iStitch < ColorChangeTable[iColor + 1].stitchIndex;
						     iStitch++) {
							if (StitchBuffer[iStitch].x >= ZoomRect.left && StitchBuffer[iStitch].x <= ZoomRect.right
							    && StitchBuffer[iStitch].y >= ZoomRect.bottom && StitchBuffer[iStitch].y <= ZoomRect.top
							    && setRmap(stitchMap, StitchBuffer[iStitch]))
								stchbox(iStitch, StitchWindowMemDC);
						}
					}
				}
			}
			else {
				for (iColor = 0; iColor < PCSHeader.stitchCount; iColor++) {
					if (StitchBuffer[iColor].x >= ZoomRect.left && StitchBuffer[iColor].x <= ZoomRect.right
					    && StitchBuffer[iColor].y >= ZoomRect.bottom && StitchBuffer[iColor].y <= ZoomRect.top
					    && setRmap(stitchMap, StitchBuffer[iColor]))
						stchbox(iColor, StitchWindowMemDC);
				}
			}
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		}
		if (StateMap.test(StateFlag::CLPSHO))
			duclp();
	}
	if (FormIndex && !StateMap.test(StateFlag::FRMOF))
		form::drwfrm();
	if (StateMap.test(StateFlag::INSFRM))
		form::rinfrm();
	if (StateMap.test(StateFlag::BIGBOX))
		form::dubig();
	if (StateMap.test(StateFlag::MOVFRM)) {
		StateMap.set(StateFlag::SHOFRM);
		form::dufrm();
	}
	if (StateMap.test(StateFlag::SATPNT))
		satin::satzum();
	if (FormDataSheet)
		formForms::refrm();
	if (StateMap.test(StateFlag::GMRK))
		drwmrk(StitchWindowMemDC);
	if (StateMap.test(StateFlag::PRFACT))
		thred::redraw(PreferencesWindow);
	if (StateMap.test(StateFlag::SELBOX))
		displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
	thred::ritot(PCSHeader.stitchCount);
	if (StateMap.test(StateFlag::INIT))
		lenCalc();
	if (StateMap.test(StateFlag::MOVSET))
		dumov();
	drwknot();
}

void thred::internal::dubar() {
	unsigned iColorChange     = 0;
	RECT     colorBarRect     = { DrawItem->rcItem.left, 0, DrawItem->rcItem.right, DrawItem->rcItem.bottom };
	double   barSectionHeight = 0.0, selectedIndicator = 0.0;
	POINT    indicatorLine[2] = {};

	for (iColorChange = 0; iColorChange < ColorChanges; iColorChange++) {
		barSectionHeight    = static_cast<double>(ColorChangeTable[iColorChange + 1].stitchIndex) / PCSHeader.stitchCount;
		colorBarRect.bottom = barSectionHeight * DrawItem->rcItem.bottom;
		FillRect(DrawItem->hDC, &colorBarRect, UserColorBrush[ColorChangeTable[iColorChange].colorIndex]);
		colorBarRect.top = colorBarRect.bottom;
	}
	if (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::GRPSEL)) {
		selectedIndicator  = static_cast<double>(ClosestPointIndex) / PCSHeader.stitchCount;
		indicatorLine[0].y = indicatorLine[1].y = colorBarRect.bottom * selectedIndicator + 0.5;
		indicatorLine[0].x                      = colorBarRect.left;
		indicatorLine[1].x                      = colorBarRect.right;
		SelectObject(DrawItem->hDC, CrossPen);
		SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
		Polyline(DrawItem->hDC, indicatorLine, 2);
		if (StateMap.test(StateFlag::GRPSEL)) {
			selectedIndicator  = static_cast<double>(GroupStitchIndex) / PCSHeader.stitchCount;
			indicatorLine[0].y = indicatorLine[1].y = colorBarRect.bottom * selectedIndicator + 0.5;
			indicatorLine[0].x                      = colorBarRect.left;
			indicatorLine[1].x                      = colorBarRect.right;
			Polyline(DrawItem->hDC, indicatorLine, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void thred::internal::ritbak(const fs::path& fileName, DRAWITEMSTRUCT* drawItem) {
	unsigned iStitch = 0, iVertexInForm = 0, iVertex = 0, iColor = 0, iLine = 0, bytesToRead = 0;
	POINT    drawingDestinationSize
	    = { (drawItem->rcItem.right - drawItem->rcItem.left), (drawItem->rcItem.bottom - drawItem->rcItem.top) };
	STRHED                stitchHeader     = {};
	fPOINT                stitchSourceSize = { 1, 1 };
	double                ratio            = 0.0;
	double                xRatio = 0.0, yRatio = 0.0;
	COLORREF              brushColor = {};
	std::vector<COLORREF> colors(16);
	HBRUSH                brush           = {};
	HPEN                  pen             = {};
	STREX                 extendedHeader  = {};
	unsigned              fileTypeVersion = 0;

	HANDLE thrEdFile = CreateFile(fileName.wstring().c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (thrEdFile != INVALID_HANDLE_VALUE) {
		ReadFile(thrEdFile, &stitchHeader, sizeof(stitchHeader), &BytesRead, NULL);
		if (BytesRead == sizeof(stitchHeader)) {
			if ((stitchHeader.headerType & 0xffffff) == 0x746872) {
				fileTypeVersion = (stitchHeader.headerType & 0xff000000) >> 24;
				switch (fileTypeVersion) {
				case 0:
					if (PCSHeader.hoopType == SMALHUP) {
						stitchSourceSize.x = IniFile.hoopSizeX = SHUPX;
						stitchSourceSize.y = IniFile.hoopSizeY = SHUPY;
					}
					else {
						stitchSourceSize.x = IniFile.hoopSizeX = LHUPX;
						stitchSourceSize.y = IniFile.hoopSizeY = LHUPY;
					}
					break;
				case 1:
				case 2:
					ReadFile(thrEdFile, &extendedHeader, sizeof(extendedHeader), &BytesRead, NULL);
					if (BytesRead != sizeof(extendedHeader))
						return;
					stitchSourceSize.x = extendedHeader.hoopSizeX;
					stitchSourceSize.y = extendedHeader.hoopSizeY;
					break;
				default:
					return;
				}
			}
			xRatio = static_cast<double>(drawingDestinationSize.x) / stitchSourceSize.x;
			yRatio = static_cast<double>(drawingDestinationSize.y) / stitchSourceSize.y;
			if (xRatio < yRatio)
				ratio = xRatio;
			else
				ratio = yRatio;
			if (stitchHeader.stitchCount) {
				std::vector<fPOINTATTR> stitchesToDraw(stitchHeader.stitchCount);
				std::vector<POINT>      lines(stitchHeader.stitchCount);
				bytesToRead = stitchHeader.stitchCount * sizeof(StitchBuffer[0]);
				ReadFile(thrEdFile, stitchesToDraw.data(), bytesToRead, &BytesRead, 0);
				if (bytesToRead == BytesRead) {
					SetFilePointer(thrEdFile, 16, 0, FILE_CURRENT);
					ReadFile(thrEdFile, &brushColor, sizeof(brushColor), &BytesRead, 0);
					ReadFileInt(thrEdFile, colors.data(), colors.size() * sizeof(colors[0]), &BytesRead, 0);
					brush = CreateSolidBrush(brushColor);
					SelectObject(drawItem->hDC, brush);
					FillRect(drawItem->hDC, &drawItem->rcItem, brush);
					iColor = stitchesToDraw[0].attribute & 0xf;
					pen    = CreatePen(PS_SOLID, 1, colors[iColor]);
					iLine  = 0;
					for (iStitch = 0; iStitch < stitchHeader.stitchCount; iStitch++) {
						if ((stitchesToDraw[iStitch].attribute & 0xf) == iColor) {
							lines[iLine].x   = stitchesToDraw[iStitch].x * ratio;
							lines[iLine++].y = drawingDestinationSize.y - stitchesToDraw[iStitch].y * ratio;
						}
						else {
							pen = nuPen(pen, 1, colors[iColor]);
							SelectObject(drawItem->hDC, pen);
							PolylineInt(drawItem->hDC, lines.data(), iLine);
							iLine  = 0;
							iColor = stitchesToDraw[iStitch].attribute & 0xf;
						}
					}
					if (iLine) {
						pen = nuPen(pen, 1, colors[iColor]);
						SelectObject(drawItem->hDC, pen);
						PolylineInt(drawItem->hDC, lines.data(), iLine);
					}
					DeleteObject(brush);
					DeleteObject(pen);
				}
				else {
					CloseHandle(thrEdFile);
					return;
				}
			}
			else
				SetFilePointer(thrEdFile, 84, 0, FILE_CURRENT);
			if (stitchHeader.formCount) {
				do {
					std::vector<POINT> lines(MAXFRMLINS);
					SetFilePointer(thrEdFile, 80, 0, FILE_CURRENT);
					std::vector<FRMHED> formList(stitchHeader.formCount);
					std::vector<fPOINT> vertexList(stitchHeader.vertexCount);
					if (fileTypeVersion < 2) {
						std::vector<FRMHEDO> formListOriginal(stitchHeader.formCount);
						bytesToRead = stitchHeader.formCount * sizeof(formListOriginal[0]);
						ReadFile(thrEdFile, formListOriginal.data(), bytesToRead, &BytesRead, 0);
						if (BytesRead != bytesToRead)
							break;
						const auto _ = std::copy(formListOriginal.cbegin(), formListOriginal.cend(), formList.begin());
					}
					else {
						bytesToRead = stitchHeader.formCount * sizeof(formList[0]);
						ReadFile(thrEdFile, formList.data(), bytesToRead, &BytesRead, 0);
						if (BytesRead != bytesToRead)
							break;
					}
					bytesToRead = stitchHeader.vertexCount * sizeof(vertexList[0]);
					ReadFile(thrEdFile, vertexList.data(), bytesToRead, &BytesRead, 0);
					if (BytesRead != bytesToRead)
						break;
					iVertex = 0;
					for (iStitch = 0; iStitch < stitchHeader.formCount; iStitch++) {
						iLine = iVertex;
						for (iVertexInForm = 0;
						     (iVertexInForm < formList[iStitch].vertexCount) && (iVertex < stitchHeader.vertexCount);
						     iVertexInForm++) {
							lines[iVertexInForm].x = vertexList[iVertex].x * ratio;
							lines[iVertexInForm].y = drawingDestinationSize.y - vertexList[iVertex++].y * ratio;
						}
						lines[iVertexInForm].x = vertexList[iLine].x * ratio;
						lines[iVertexInForm].y = drawingDestinationSize.y - vertexList[iLine].y * ratio;
						SelectObject(drawItem->hDC, FormPen);
						SetROP2(drawItem->hDC, R2_XORPEN);
						if (FormList[iStitch].type == FRMLINE)
							PolylineInt(drawItem->hDC, lines.data(), formList[iStitch].vertexCount);
						else
							PolylineInt(drawItem->hDC, lines.data(), formList[iStitch].vertexCount + 1);
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

LRESULT CALLBACK thred::internal::WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	unsigned position = 0, iColor = 0, iThumb = 0;
	unsigned screenCenterOffset = 0;
	SIZE     maxWindowDimension = {};
	SIZE     textSize           = {}; // for measuring text items
	POINT    scrollPoint        = {}; // for scroll bar functions
	POINT    line[2]            = {};
	long     adjustedWidth      = 0;
	double   tdub               = 0.0;
	fs::path fileName;

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
		// Here we are detecting if a BOOL was set to enable non-client DPI scaling
		// via the call to CreateWindow that resulted in this window. Doing this
		// detection is only necessary in the context of this sample.
		auto createStruct = reinterpret_cast<const CREATESTRUCT*>(lParam);
		auto createParams = static_cast<const CreateParams*>(createStruct->lpCreateParams);
		if (createParams->bEnableNonClientDpiScaling) {
			EnableNonClientDpiScaling(p_hWnd);
		}

		return DefWindowProc(p_hWnd, message, wParam, lParam);
	}
#endif
	case WM_INITMENU:
		if (StateMap.testAndReset(StateFlag::PRFACT)) {
			DestroyWindow(PreferencesWindow);
			StateMap.reset(StateFlag::WASRT);
		}
		thred::unmsg();
		thred::undat();
		StateMap.reset(StateFlag::FORMIN);
		StateMap.set(StateFlag::RESTCH);
		break;
	case WM_HSCROLL:
		switch (gsl::narrow<long>(LOWORD(wParam))) {
		case SB_LINELEFT:
			if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
				MovieTimeStep += SPEDLIN;
				if (MovieTimeStep > MAXDELAY)
					MovieTimeStep = MAXDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			}
			else {
				scrollPoint.x = (ZoomRect.right - ZoomRect.left) * LINSCROL;
				if (!scrollPoint.x)
					scrollPoint.x = 1;
				scrollPoint.y = 0;
				rshft(scrollPoint);
			}
			return 1;
		case SB_LINERIGHT:
			if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
				MovieTimeStep -= SPEDLIN;
				if (MovieTimeStep < MINDELAY)
					MovieTimeStep = MINDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			}
			else {
				scrollPoint.x = -(ZoomRect.right - ZoomRect.left) * LINSCROL;
				if (!scrollPoint.x)
					scrollPoint.x = -1;
				scrollPoint.y = 0;
				rshft(scrollPoint);
			}
			return 1;
		case SB_PAGELEFT:
			if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
				MovieTimeStep += SPEDPAG;
				if (MovieTimeStep < MINDELAY)
					MovieTimeStep = MINDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			}
			else
				pglft();
			return 1;
		case SB_PAGERIGHT:
			if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
				MovieTimeStep -= SPEDPAG;
				if (MovieTimeStep < MINDELAY)
					MovieTimeStep = MINDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			}
			else
				pgrit();
			return 1;
		case SB_THUMBPOSITION:
			if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
				[[gsl::suppress(type .1)]] {
					if (reinterpret_cast<HWND>(lParam) == SpeedScrollBar) {
						position      = HIWORD(wParam);
						MovieTimeStep = MAXDELAY - position;
						setsped();
						SetScrollPos(SpeedScrollBar, SB_CTL, position, TRUE);
					}
				}
			}
			else {
				[[gsl::suppress(type .1)]] {
					if (reinterpret_cast<HWND>(lParam) == HorizontalScrollBar) {
						scrollPoint.x  = ZoomRect.right - ZoomRect.left;
						ZoomRect.left  = gsl::narrow<long>(HIWORD(wParam));
						ZoomRect.right = ZoomRect.left + scrollPoint.x;
						if (ZoomRect.right > UnzoomedRect.x) {
							ZoomRect.right = UnzoomedRect.x;
							ZoomRect.left  = UnzoomedRect.x - scrollPoint.x;
						}
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			return 1;
		}
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam)) {
			//		unpat();
		case SB_LINEDOWN:
			scrollPoint.y = (ZoomRect.top - ZoomRect.bottom) * LINSCROL;
			if (!scrollPoint.y)
				scrollPoint.y = 1;
			scrollPoint.x = 0;
			rshft(scrollPoint);
			return 1;
		case SB_LINEUP:
			scrollPoint.y = -(ZoomRect.top - ZoomRect.bottom) * LINSCROL;
			if (!scrollPoint.y)
				scrollPoint.y = -1;
			scrollPoint.x = 0;
			rshft(scrollPoint);
			return 1;
		case SB_PAGEDOWN:
			pgdwn();
			return 1;
		case SB_PAGEUP:
			pgup();
			return 1;
		case SB_THUMBPOSITION:
			scrollPoint.y   = ZoomRect.top - ZoomRect.bottom;
			ZoomRect.top    = UnzoomedRect.y - gsl::narrow<long>(HIWORD(wParam));
			ZoomRect.bottom = ZoomRect.top - scrollPoint.y;
			if (ZoomRect.bottom < 0) {
				ZoomRect.bottom = 0;
				ZoomRect.top    = scrollPoint.y;
			}
			StateMap.set(StateFlag::RESTCH);
			return 1;
		}
		break;
	case WM_DRAWITEM:
		// owner draw windows
		[[gsl::suppress(type .1)]] DrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (DrawItem->hwndItem == MainStitchWin && DrawItem->itemAction == ODA_DRAWENTIRE) {
			if (StateMap.test(StateFlag::TXTRED)) {
				texture::drwtxtr();
				return 1;
			}
			if (!StateMap.test(StateFlag::RUNPAT)) {
				if (!StateMap.test(StateFlag::HIDSTCH)
				    && (FileHandle || StateMap.test(StateFlag::INIT) || FormIndex || StateMap.test(StateFlag::SATPNT))
				    && !StateMap.test(StateFlag::BAKSHO))
					drwStch();
				else {
					FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
					thred::duzrat();
					dugrid();
					if (StateMap.test(StateFlag::HIDSTCH)) {
						form::drwfrm();
						if (StateMap.test(StateFlag::SATPNT))
							satin::satzum();
					}
					if (StateMap.test(StateFlag::PRFACT))
						thred::redraw(PreferencesWindow);
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
				if (StateMap.test(StateFlag::ROTSHO))
					durot();
				if (StateMap.test(StateFlag::SHOSAT))
					satin::dusat();
				if (StateMap.test(StateFlag::SHOINSF))
					form::duinsf();
			}
			return 1;
		}
		if (DrawItem->hwndItem == ColorBar && DrawItem->itemAction == ODA_DRAWENTIRE) {
			if (PCSHeader.stitchCount)
				dubar();
			else
				FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_WINDOW));
			return 1;
		}
		if (DrawItem->hwndItem == (*ButtonWin)[HHID] && DrawItem->itemAction == ODA_DRAWENTIRE) {
			position = (ButtonWidthX3 - PickColorMsgSize.cx) >> 1;
			if (StateMap.test(StateFlag::HID)) {
				FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[ActiveColor]);
				SetBkColor(DrawItem->hDC, UserColor[ActiveColor]);
			}
			else
				FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_BTNFACE));
			if (StateMap.test(StateFlag::TXTRED)) {
				texture::writeScreenWidth(position);
			}
			else
				TextOutInt(DrawItem->hDC, position, 1, (*StringTable)[STR_PIKOL].c_str(), (*StringTable)[STR_PIKOL].size());
			return 1;
		}
		if (StateMap.test(StateFlag::WASTRAC)) {
			trace::wasTrace();
			return 1;
		}
		else {
			for (iColor = 0; iColor < 16; iColor++) {
				if (DrawItem->hwndItem == DefaultColorWin[iColor]) {
					FillRect(DrawItem->hDC, &DrawItem->rcItem, DefaultColorBrush[iColor]);
					if (DisplayedColorBitmap.test(iColor)) {
						SetBkColor(DrawItem->hDC, DefaultColors[iColor]);
						SetTextColor(DrawItem->hDC, defTxt(iColor));
						std::wstring colorNum(fmt::format(L"{}", iColor + 1));
						GetTextExtentPoint32Int(DrawItem->hDC, colorNum.c_str(), colorNum.size(), &textSize);
						TextOutInt(DrawItem->hDC, (ButtonWidth - textSize.cx) >> 1, 0, colorNum.c_str(), colorNum.size());
					}
					return 1;
				}
				if (DrawItem->hwndItem == UserColorWin[iColor]) {
					FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[iColor]);
					if (iColor == ActiveColor) {
						SelectObject(DrawItem->hDC, CrossPen);
						SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
						line[0].x = line[1].x = DrawItem->rcItem.right >> 1;
						line[0].y             = 0;
						line[1].y             = DrawItem->rcItem.bottom;
						Polyline(DrawItem->hDC, line, 2);
						line[0].y = line[1].y = DrawItem->rcItem.bottom >> 1;
						line[0].x             = 0;
						line[1].x             = DrawItem->rcItem.right;
						Polyline(DrawItem->hDC, line, 2);
						SetROP2(StitchWindowMemDC, R2_COPYPEN);
					}
					return 1;
				}
			}
		}
		if (StateMap.test(StateFlag::BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {
			if (StateMap.test(StateFlag::THUMSHO)) {
				for (iThumb = 0; iThumb < 4; iThumb++) {
					if (DrawItem->hwndItem == BackupViewer[iThumb] && iThumb < ThumbnailDisplayCount) {
						ritbak((*Thumbnails)[ThumbnailsSelected[iThumb]].data(), DrawItem);
						rthumnam(iThumb);
						return 1;
					}
				}
			}
			else {
				for (char iVersion = 0; iVersion < OLDVER; iVersion++) {
					if (DrawItem->hwndItem == BackupViewer[iVersion]) {
						fileName   = *ThrName;
						auto ext   = fileName.extension().wstring();
						ext.back() = iVersion + 's';
						fileName.replace_extension(ext);
						ritbak(fileName, DrawItem);
						return 1;
					}
				}
			}
		}
		break;
	case WM_SIZE:
		GetClientRect(p_hWnd, &ThredWindowRect);
		switch (wParam) {
		case SIZE_MAXIMIZED:
			UserFlagMap.set(UserFlag::SAVMAX);
			break;
		case SIZE_MINIMIZED:
			UserFlagMap.reset(UserFlag::SAVMAX);
			break;
		case SIZE_RESTORED:
			UserFlagMap.reset(UserFlag::SAVMAX);
			chkirct();
			if (StateMap.testAndSet(StateFlag::SIZED)) {
				screenCenterOffset    = 0;
				maxWindowDimension.cx = ScreenSizePixels.cx - 30;
				if ((ThredWindowRect.right - ThredWindowRect.left) > maxWindowDimension.cx) {
					screenCenterOffset    = (ScreenSizePixels.cx - maxWindowDimension.cx) >> 1;
					ThredWindowRect.left  = screenCenterOffset;
					ThredWindowRect.right = ScreenSizePixels.cx - screenCenterOffset;
				}
				maxWindowDimension.cy = ScreenSizePixels.cy - 30;
				if ((ThredWindowRect.bottom - ThredWindowRect.top) > maxWindowDimension.cy) {
					screenCenterOffset     = (ScreenSizePixels.cy - maxWindowDimension.cy) >> 1;
					ThredWindowRect.top    = screenCenterOffset;
					ThredWindowRect.bottom = ScreenSizePixels.cy - screenCenterOffset;
				}
				if (screenCenterOffset)
					MoveWindow(p_hWnd,
					           ThredWindowRect.left,
					           ThredWindowRect.top,
					           ThredWindowRect.right - ThredWindowRect.left,
					           ThredWindowRect.bottom - ThredWindowRect.top,
					           TRUE);
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
		GetClientRect(p_hWnd, &ThredWindowRect);
		thred::movStch();
		if (StateMap.test(StateFlag::ZUMED)) {
			tdub          = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
			adjustedWidth = StitchWindowClientRect.right / tdub;
			if (adjustedWidth + ZoomRect.left > UnzoomedRect.x) {
				ZoomRect.right = UnzoomedRect.x;
				ZoomRect.left  = UnzoomedRect.x - adjustedWidth;
			}
			else
				ZoomRect.right = adjustedWidth + ZoomRect.left;
		}
		else {
			ZoomRect.left = ZoomRect.bottom = 0;
			ZoomRect.right                  = UnzoomedRect.x;
			ZoomRect.top                    = UnzoomedRect.y;
		}
		NearestCount = 0;
		StateMap.set(StateFlag::RESTCH);
		if (StateMap.test(StateFlag::SELBOX))
			shft2box();
		if (StateMap.test(StateFlag::RUNPAT)) {
			FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
			if (StateMap.test(StateFlag::ZUMED))
				RunPoint = GroupStartStitch;
			else
				RunPoint = 0;
		}
		return 1;
	case WM_MOVE:
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
	case WM_CLOSE:
		dun();
		if (StateMap.test(StateFlag::SAVEX))
			return 1;
		break;
	}
	return DefWindowProc(p_hWnd, message, wParam, lParam);
}

void thred::internal::sachk() noexcept {
	for (auto iForm = 0ul; iForm < FormIndex; iForm++) {
		const auto& form = FormList[iForm];
		if (form.type == SAT && form.satinGuideCount) {
			const SATCON* guide = form.satinOrAngle.guide;
			if (guide) {
				for (size_t iGuide = 0; iGuide < form.satinGuideCount; iGuide++) {
					if (guide[iGuide].start > form.vertexCount || guide[iGuide].finish > form.vertexCount) {
						const auto bakclo   = ClosestFormToCursor;
						ClosestFormToCursor = iForm;
						satin::delsac(iForm);
						ClosestFormToCursor = bakclo;
					}
				}
			}
		}
	}
}

#define BAD_FPU_EX (_EM_OVERFLOW | _EM_ZERODIVIDE | _EM_INVALID)
#define COMMON_FPU_EX (_EM_INEXACT | _EM_UNDERFLOW | _EM_DENORMAL)
#define ALL_FPU_EX (BAD_FPU_EX | COMMON_FPU_EX)

/*
int fltex(int code) noexcept {
	if (code != 0x10)
		return 0;
	unsigned int current_word = 0;
	_controlfp_s(&current_word, ALL_FPU_EX, _MCW_EM);
	_controlfp_s(&current_word, _RC_NEAR, _MCW_RC);

	return -1;
}
*/

#ifdef ALLOCFAILURE
int handle_program_memory_depletion(size_t) {
	// ToDo - Make this handle the failure with more user notifiication
	displayText::shoMsg("Memory Allocation Failure");
	exit(EXIT_FAILURE);
}
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(nCmdShow);

	ArgList = CommandLineToArgvW(GetCommandLine(), &ArgCount);

	WNDCLASSEX wc = {};
	LOGBRUSH   br = { BS_SOLID, (COLOR_BACKGROUND + 1), 0 };

	std::vector<POINT> stretchBoxLine(5);   // stretch and expand
	double             xyRatio        = {}; // expand form aspect ratio
	double             rotationAngle  = {};
	dPOINT             rotationCenter = {};

#ifdef ALLOCFAILURE
	_PNH old_handler = _set_new_handler(handle_program_memory_depletion);
	// char* testalloc = new char[(~unsigned int((int)0) / 2) - 1]();
	// testalloc[0] = 1;
#endif

	ThrEdInstance  = hInstance;
	wc.cbSize      = sizeof(wc);
	wc.style       = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)thi::WndProc;
	wc.cbClsExtra  = 0;
	wc.cbWndExtra  = 0;
	wc.hInstance   = ThrEdInstance;
	wc.hIcon       = static_cast<HICON>(LoadImage(ThrEdInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_SHARED));
	wc.hCursor     = 0; //  set the cursor to null as the cursor changes in the window:
	                    //  https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setcursor
	wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = L"thred";
	wc.hIconSm       = NULL;

#if HIGHDPI
	DPI_AWARENESS_CONTEXT previousDpiContext = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
#endif

	// to keep the compiler from complaining
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (RegisterClassEx(&wc)) {
		boost::dynamic_bitset<> private_TracedMap(0);
		TracedMap = &private_TracedMap;
		boost::dynamic_bitset<> private_TracedEdges(0);
		TracedEdges = &private_TracedEdges;
		std::vector<std::wstring> private_Thumbnails;
		Thumbnails = &private_Thumbnails;
		std::vector<std::unique_ptr<unsigned[]>> private_UndoBuffer(16);
		UndoBuffer = &private_UndoBuffer;
		std::vector<TXPNT>  private_TempTexturePoints;
		std::vector<size_t> private_SelectedTexturePointsList;
		texture::initTextures(&private_TempTexturePoints, &private_SelectedTexturePointsList);
		std::vector<std::wstring> private_StringTable(STR_LEN);
		StringTable = &private_StringTable;
		std::vector<HWND> private_ButtonWin;
		ButtonWin = &private_ButtonWin;
		std::wstring private_FormOnOff;
		FormOnOff = &private_FormOnOff;
		[[gsl::suppress(type .3)]] {
			LPWSTR formOnOff = const_cast<LPWSTR>(private_FormOnOff.data());

			MENUITEMINFO private_MenuInfo
			    = { sizeof(private_MenuInfo), // Size
				    MIIM_TYPE,                // Mask
				    MFT_STRING,               // Type
				    0,                        // State
				    0,                        // ID
				    0,                        // SubMenu
				    0,                        // bmpChecked
				    0,                        // bmpUnchecked
				    0,                        // ItemData
				    formOnOff,                // TypeData
				    16,                       // cch
#if (WINVER >= 0x0500)
				    0 // bmpItem
#endif                /* WINVER >= 0x0500 */
			      };
			MenuInfo = &private_MenuInfo;
		}
		std::vector<fPOINT> private_TempPolygon;
		TempPolygon = &private_TempPolygon;
		std::vector<fPOINT> private_OutsidePointList;
		OutsidePointList = &private_OutsidePointList;
		std::vector<fPOINT> private_InsidePointList;
		InsidePointList = &private_InsidePointList;
		std::vector<HWND> private_ValueWindow(LASTLIN);
		ValueWindow = &private_ValueWindow;
		std::vector<HWND> private_LabelWindow(LASTLIN);
		LabelWindow = &private_LabelWindow;
		std::vector<size_t> private_SelectedFormList;
		SelectedFormList = &private_SelectedFormList;
		std::vector<POINT> private_FormControlPoints(10);
		FormControlPoints = &private_FormControlPoints;
		std::vector<POINT> private_RubberBandLine(3);
		RubberBandLine = &private_RubberBandLine;
		std::vector<POINT> private_SelectedFormsLine(9);
		SelectedFormsLine = &private_SelectedFormsLine;
		std::vector<POINT> private_SelectedPointsLine(9);
		SelectedPointsLine = &private_SelectedPointsLine;
		std::vector<double> private_FormAngles;
		FormAngles = &private_FormAngles;
		std::wstring private_textureInputBuffer;
		TextureInputBuffer = &private_textureInputBuffer;
		std::vector<TXPNT> private_TexturePointsBuffer;
		TexturePointsBuffer = &private_TexturePointsBuffer;
		std::vector<fs::path> private_PreviousNames;
		for (auto iVersion = 0; iVersion < OLDNUM; iVersion++) {
			private_PreviousNames.push_back(L"");
		}
		PreviousNames = &private_PreviousNames;
		std::wstring private_DesignerName;
		DesignerName = &private_DesignerName;
		fs::path private_AuxName;
		fs::path private_WorkingFileName;
		fs::path private_ThrName;
		fs::path private_GeName;
		fs::path private_ColorFileName;
		fs::path private_RGBFileName;
		fs::path private_DefaultDirectory;
		fs::path private_SearchName;
		fs::path private_DefaultBMPDirectory;
		fs::path private_BalaradName0;
		fs::path private_BalaradName1;
		fs::path private_BalaradName2;
		fs::path private_HomeDirectory;
		fs::path private_UserBMPFileName;
		fs::path private_IniFileName;

		AuxName             = &private_AuxName;
		WorkingFileName     = &private_WorkingFileName;
		ThrName             = &private_ThrName;
		GeName              = &private_GeName;
		ColorFileName       = &private_ColorFileName;
		RGBFileName         = &private_RGBFileName;
		DefaultDirectory    = &private_DefaultDirectory;
		SearchName          = &private_SearchName;
		DefaultBMPDirectory = &private_DefaultBMPDirectory;
		BalaradName0        = &private_BalaradName0;
		BalaradName1        = &private_BalaradName1;
		BalaradName2        = &private_BalaradName2;
		HomeDirectory       = &private_HomeDirectory;
		UserBMPFileName     = &private_UserBMPFileName;
		IniFileName         = &private_IniFileName;

		thi::redini();

		CreateParams createParams;
		createParams.bEnableNonClientDpiScaling = true;

		if (IniFile.initialWindowCoords.right) {
			ThrEdWindow = CreateWindow(L"thred",
			                           L"",
			                           WS_OVERLAPPEDWINDOW,
			                           IniFile.initialWindowCoords.left,
			                           IniFile.initialWindowCoords.right,
			                           IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left,
			                           IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top,
			                           0,
			                           0,
			                           hInstance,
			                           &createParams);
		}
		else {
			ThrEdWindow = CreateWindow(L"thred",
			                           L"",
			                           WS_OVERLAPPEDWINDOW,
			                           CW_USEDEFAULT,
			                           CW_USEDEFAULT,
			                           CW_USEDEFAULT,
			                           CW_USEDEFAULT,
			                           0,
			                           0,
			                           hInstance,
			                           &createParams);
			GetClientRect(ThrEdWindow, &ThredWindowRect);
			IniFile.initialWindowCoords.left   = ThredWindowRect.left;
			IniFile.initialWindowCoords.right  = ThredWindowRect.right;
			IniFile.initialWindowCoords.top    = ThredWindowRect.top;
			IniFile.initialWindowCoords.bottom = ThredWindowRect.bottom;
		}
		thi::init();
		if (UserFlagMap.test(UserFlag::SAVMAX))
			ShowWindow(ThrEdWindow, SW_SHOWMAXIMIZED);
		else
			ShowWindow(ThrEdWindow, SW_SHOW);
		if (!DesignerName->size()) {
			wchar_t designerBuffer[50];
			LoadString(ThrEdInstance, IDS_UNAM, designerBuffer, sizeof(designerBuffer) / sizeof(designerBuffer[0]));
			DesignerName->assign(designerBuffer);
			thi::getdes();
		}
		while (GetMessage(&Msg, NULL, 0, 0)) {
			StateMap.set(StateFlag::SAVACT);
			if (!thi::chkMsg(stretchBoxLine, xyRatio, rotationAngle, rotationCenter))
				DispatchMessage(&Msg);
			if (StateMap.testAndReset(StateFlag::FCHK))
				repair::frmchkx();
			if (StateMap.testAndReset(StateFlag::RESTCH))
				thred::redraw(MainStitchWin);
			if (StateMap.testAndReset(StateFlag::RELAYR))
				thi::ritlayr();
			if (!StateMap.test(StateFlag::TXTRED))
				thi::sachk();
			if (StateMap.testAndReset(StateFlag::DUMEN))
				DrawMenuBar(ThrEdWindow);
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