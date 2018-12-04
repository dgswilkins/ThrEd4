﻿#include "stdafx.h"

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

namespace fs = std::experimental::filesystem;

namespace thi = thred::internal;

// select box
constexpr auto NERCNT = 4u; // number of entries in the near array;

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
unsigned int    BufferDigitCount;         // number of decimal digits in the number of stitches
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
unsigned int  TmpFormIndex;                  // saved form index
double        ZoomMin;                       // minimum allowed zoom value
fRECTANGLE    CheckHoopRect;                 // for checking the hoop size
fPOINT        BalaradOffset;                 // balarad offset
unsigned      SideWindowLocation;            // side message window location
POINT         SideWindowSize;                // size of the side message window
std::wstring* SideWindowsStrings;            // string array displayed in sidmsg
dPOINT        CellSize;                      // size of an stitchMap cell for drawing stitch boxes
unsigned      DraggedColor;                  // color being dragged
POINT*        FormVerticesAsLine;            // form vertex clipboard paste into form line
unsigned int  LastFormSelected;              // end point of selected range of forms

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
const TCHAR AllFilter[_MAX_PATH + 1]
    = L"Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Brother (PES)\0*.pes\0Tajima (DST)\0*.dst\0";
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
HANDLE        FileHandle                  = nullptr;
HANDLE        PCSFileHandle               = nullptr;
HANDLE        IniFileHandle               = nullptr;
HANDLE        InsertedFileHandle; // insert file handle
HANDLE        BitmapFileHandle;   // bitmap handle
unsigned int  FileSize;           // size of file
DWORD         BytesRead;          // bytes actually read from file
unsigned      ColorChanges;       // number of color changes

OPENFILENAME OpenFileName = {
	sizeof(OpenFileName), // lStructsize
	nullptr,              // hwndOwner
	nullptr,              // hInstance
	AllFilter,            // lpstrFilter
	CustomFilter,         // lpstrCustomFilter
	_MAX_PATH,            // nMaxCustFilter
	0,                    // nFilterIndex
	nullptr,              // lpstrFile
	_MAX_PATH,            // nMaxFile
	nullptr,              // lpstrFileTitle
	0,                    // nMaxFileTitle
	nullptr,              // lpstrInitialDir
	nullptr,              // lpstrTitle
	OFN_OVERWRITEPROMPT,  // Flags
	0,                    // nFileOffset
	0,                    // nFileExtension
	L"thr",               // lpstrDefExt
	0,                    // lCustData
	nullptr,              // lpfnHook
	nullptr               // lpTemplateName
};

std::vector<fs::path>*     PreviousNames;
std::vector<std::wstring>* Thumbnails;                          // vector of thumbnail names
unsigned int               ThumbnailsSelected[4];               // indexes of thumbnails selected for display
unsigned                   ThumbnailDisplayCount;               // number of thumbnail file names selected for display
unsigned int               ThumbnailIndex;                      // index into the thumbnail filname table
wchar_t                    ThumbnailSearchString[32];           // storage for the thumnail search string
wchar_t                    InsertedFileName[_MAX_PATH] = { 0 }; // insert file name
unsigned int               InsertedVertexIndex;                 // saved float pointer for inserting files
unsigned int               InsertedFormIndex;                   // saved form pointer for inserting files
unsigned                   InsertedStitchCount;                 // saved stitch pointer for inserting files

OPENFILENAME OpenBitmapName = {
	sizeof(OpenBitmapName), // lStructsize
	nullptr,                // hwndOwner
	nullptr,                // hInstance
	BmpFilter,              // lpstrFilter
	CustomFilter,           // lpstrCustomFilter
	_MAX_PATH,              // nMaxCustFilter
	0,                      // nFilterIndex
	nullptr,                // lpstrFile
	_MAX_PATH,              // nMaxFile
	nullptr,                // lpstrFileTitle
	0,                      // nMaxFileTitle
	nullptr,                // lpstrInitialDir
	nullptr,                // lpstrTitle
	OFN_OVERWRITEPROMPT,    // Flags
	0,                      // nFileOffset
	0,                      // nFileExtension
	L"bmp",                 // lpstrDefExt
	0,                      // lCustData
	nullptr,                // lpfnHook
	nullptr,                // lpTemplateName
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

struct _dstdat {
	char cor;
	char val;
};

using DSTDAT = struct _dstdat;

constexpr auto XCOR = 0;
constexpr auto YCOR = 1;

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

	switch (umsg) {
	case WM_INITDIALOG: {
		auto hwnd = GetDlgItem(hwndlg, IDC_DESED);
		SetWindowText(hwnd, DesignerName->c_str());
		SetFocus(hwnd);
		SendMessage(hwnd, EM_SETSEL, 0, -1);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case IDCANCEL: {
			EndDialog(hwndlg, 0);
			return TRUE;
		}
		case IDOK: {
			auto    hwnd = GetDlgItem(hwndlg, IDC_DESED);
			wchar_t designerBuffer[50];
			GetWindowText(hwnd, designerBuffer, sizeof(designerBuffer) / sizeof(designerBuffer[0]));
			DesignerName->assign(designerBuffer);
			EndDialog(hwndlg, 0);
			SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRED], *DesignerName).c_str());
			return TRUE;
		}
		}
	}
	}
	return false;
}

void thred::internal::getdes() noexcept {
	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DESNAM), ThrEdWindow, (DLGPROC)dnamproc);
}

bool thred::internal::isfclp() {
	return clip::isclp(ClosestFormToCursor) && (*FormList)[ClosestFormToCursor].fillType != CLPF;
}

double thred::internal::stlen(unsigned iStitch) noexcept {
	return hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
}

void thred::undat() noexcept {
	if (FormDataSheet) {
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
	auto code = UINT{ MF_CHECKED };

	if (UserFlagMap.test(UserFlag::WRNOF)) {
		code = MF_UNCHECKED;
	}
	CheckMenuItem(MainMenu, ID_WARNOF, code);
}

int datcod[] = { ID_CHKOF, ID_CHKON, ID_CHKREP, ID_CHKREPMSG };

void thred::chkmen() noexcept {
	auto code = UINT{ MF_CHECKED };

	for (auto iCode = 0; iCode < (sizeof(datcod) / sizeof(datcod[0])); iCode++) {
		code = MF_UNCHECKED;
		if (iCode == IniFile.dataCheck) {
			code = MF_CHECKED;
		}
		CheckMenuItem(MainMenu, datcod[iCode], code);
	}
}

void thred::duzrat() noexcept {
	if (!ZoomRect.right) {
		ZoomRect.right = LHUPX;
	}
	if (!ZoomRect.top) {
		ZoomRect.top = LHUPY;
	}
	ZoomRatio.x = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	ZoomRatio.y = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
}

unsigned thred::internal::rsed() noexcept {
	auto time = SYSTEMTIME{};

	GetLocalTime(&time);
	return (time.wSecond << 16) | time.wMilliseconds;
}

void thred::internal::fnamtabs() {
	for (auto iName = 0u; iName < 50; iName++) {
		NameOrder[iName] = iName;
	}
	PseudoRandomValue = NORDSED;
	for (auto iName = 0u; iName < 100; iName++) {
		auto source      = form::psg() % 50;
		auto destination = form::psg() % 50;
		std::swap(NameOrder[destination], NameOrder[source]);
	}
	for (auto iName = 0u; iName < 128; iName++) {
		NameEncoder[iName] = gsl::narrow<unsigned char>(iName) + NCODOF;
	}
	PseudoRandomValue = NCODSED;
	for (auto iName = 0u; iName < 256; iName++) {
		auto source      = form::psg() & 0x7f;
		auto destination = form::psg() & 0x7f;
		std::swap(NameEncoder[destination], NameEncoder[source]);
	}
	const auto fillval = gsl::narrow_cast<unsigned char>(0);
	std::fill_n(NameDecoder, sizeof(NameDecoder), fillval);
	for (auto iName = 32u; iName < 127; iName++) {
		NameDecoder[NameEncoder[iName]] = gsl::narrow<unsigned char>(iName);
	}
}

void thred::internal::ritfnam(const std::wstring& designerName) {
	auto          designer    = utf::Utf16ToUtf8(designerName);
	auto          iName       = 0u;
	unsigned char tmpName[50] = { 0 };

	if (NameOrder[0] > 50) {
		fnamtabs();
	}
	PseudoRandomValue = rsed();
	for (iName = 0; iName < 50; iName++) {
		tmpName[iName] = form::psg() & 0xff;
	}
	for (iName = 0; iName < 50; iName++) {
		if (designer[iName]) {
			tmpName[iName] = NameEncoder[designer[iName]];
		}
		else {
			while (NameDecoder[tmpName[iName]]) {
				tmpName[iName] = form::psg() & 0xff;
			}
			break;
		}
	}
	if (iName == 50) {
		while (NameDecoder[tmpName[49]]) {
			tmpName[49] = form::psg() & 0xff;
		}
	}
	for (iName = 0; iName < 50; iName++) {
		if (NameOrder[iName] < 50) {
			ExtendedHeader.creatorName[NameOrder[iName]] = tmpName[iName];
		}
	}
}

void thred::internal::redfnam(std::wstring& designerName) {
	unsigned char tmpName[50] = { 0 };
	auto          designer    = std::string{};

	for (auto iName = 0; iName < 50; iName++) {
		if (NameOrder[iName] < 50) {
			tmpName[iName] = ExtendedHeader.creatorName[NameOrder[iName]];
		}
		else {
			tmpName[iName] = 111;
		}
	}
	designer.reserve(50);
	for (const auto character : tmpName) {
		if (NameDecoder[character]) {
			designer.push_back(NameDecoder[character]);
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
	auto shift = 1u;
	pout       = {};
	for (const auto& DSTValue : DSTValues) {
		if (number & shift) {
			if (DSTValue.cor) {
				pout.y += DSTValue.val;
			}
			else {
				pout.x += DSTValue.val;
			}
		}
		shift <<= 1;
	}
}

fPOINT* thred::adflt(unsigned int count) {
	auto iFormVertex = FormVertexIndex;

	if (FormVertexIndex + count > MAXITEMS) {
		displayText::tabmsg(IDS_FRMOVR);
	}
	FormVertexIndex += count;
	return &FormVertices[iFormVertex];
}

unsigned int thred::adclp(unsigned int count) {
	const auto iClipPoint = ClipPoints->size();
	const auto it = ClipPoints->end();
	const auto val = fPOINT{};
	ClipPoints->insert(it, count, val);
	return iClipPoint;
}

unsigned thred::duthrsh(double threshold) noexcept {
	auto iZoomLevel = 0u;

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
	return iZoomLevel + 1;
}

double thred::internal::unthrsh(unsigned level) noexcept {
	auto zoom = 1.0;

	if (level) {
		level--;
	}
	else {
		return 0.0;
	}
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
	const auto point = fPOINT{ pointAttribute.x, pointAttribute.y };

	thred::ritfcor(point);
}

void thred::internal::fndknt() noexcept {
	auto endStitch = PCSHeader.stitchCount;
	if (PCSHeader.stitchCount > 4) {
		endStitch -= 4;
		// ToDo - Is flag initialized and used correctly?
		auto flag = false;
		KnotCount = 0;
		for (auto iStitch = 0u; iStitch < endStitch; iStitch++) {
			if (StitchBuffer[iStitch].attribute & KNOTMSK) {
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

void thred::coltab() {
	ColorChanges   = 0;
	auto endStitch = PCSHeader.stitchCount;
	if (endStitch) {
		endStitch--;
		StitchBuffer[0].attribute &= NCOLMSK;
		StitchBuffer[0].attribute |= (StitchBuffer[1].attribute & COLMSK);
		StitchBuffer[PCSHeader.stitchCount - 1].attribute &= NCOLMSK;
		StitchBuffer[PCSHeader.stitchCount - 1].attribute |= (StitchBuffer[PCSHeader.stitchCount - 2].attribute & COLMSK);
		auto currentColor = StitchBuffer[0].attribute & COLMSK;
		for (auto iStitch = 1u; iStitch < endStitch; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & COLMSK) != currentColor) {
				if ((StitchBuffer[iStitch + 1].attribute & COLMSK) == currentColor) {
					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= currentColor;
				}
				currentColor = StitchBuffer[iStitch].attribute & COLMSK;
			}
		}
		auto iColor      = 0u;
		currentColor     = 0xffffffff;
		const auto range = dRECTANGLE{ UnzoomedRect.y * 2.0, UnzoomedRect.y * -1.0, UnzoomedRect.x * -1.0, UnzoomedRect.x * 2.0 };
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			auto* Stitch = &StitchBuffer[iStitch];
			if (Stitch->x < range.left) {
				Stitch->x = static_cast<float>(range.left);
			}
			if (Stitch->x > range.right) {
				Stitch->x = static_cast<float>(range.right);
			}
			if (Stitch->y > range.top) {
				Stitch->y = static_cast<float>(range.top);
			}
			if (Stitch->y < range.bottom) {
				Stitch->y = static_cast<float>(range.bottom);
			}
			const auto nextColor = Stitch->attribute & COLMSK;
			if (currentColor != nextColor) {
				ColorChangeTable[iColor].colorIndex    = gsl::narrow<unsigned short>(nextColor);
				ColorChangeTable[iColor++].stitchIndex = gsl::narrow<unsigned short>(iStitch);
				currentColor                           = nextColor;
			}
		}
		ColorChanges                         = iColor;
		ColorChangeTable[iColor].stitchIndex = gsl::narrow<unsigned short>(PCSHeader.stitchCount);
		if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		}
		thi::fndknt();
	}
}

void thred::internal::ladj() {
	for (auto iLayer = 0u; iLayer < 5; iLayer++) {
		if (iLayer == ActiveLayer) {
			EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_GRAYED);
		}
		else {
			EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_ENABLED);
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
	const auto&    formList  = *FormList;
	const auto     formCount = formList.size();
	constexpr auto formSize  = sizeof(decltype(formList.back()));
	const auto     size      = sizeof(BAKHED) + formSize * formList.size() + sizeof(StitchBuffer[0]) * PCSHeader.stitchCount
	                  + sizeof(FormVertices[0]) * FormVertexIndex + sizeof((*ClipPoints)[0]) * ClipPoints->size()
	                  + sizeof(SatinGuides[0]) * satin::getGuideSize() + sizeof(UserColor)
	                  + sizeof((*TexturePointsBuffer)[0]) * TextureIndex;
	undoBuffer[UndoBufferWriteIndex] = std::make_unique<unsigned[]>(size);
	auto backupData                  = convert_ptr<BAKHED*>(undoBuffer[UndoBufferWriteIndex].get());
	if (backupData) {
		backupData->zoomRect  = UnzoomedRect;
		backupData->formCount = formCount;
		backupData->forms     = convert_ptr<FRMHED*>(&backupData[1]);
		std::copy(formList.cbegin(), formList.cend(), stdext::make_checked_array_iterator(backupData->forms, formList.size()));
		backupData->stitchCount = PCSHeader.stitchCount;
		backupData->stitches    = convert_ptr<fPOINTATTR*>(&backupData->forms[formCount]);
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
		backupData->clipPointCount = ClipPoints->size();
		backupData->clipPoints     = convert_ptr<fPOINT*>(&backupData->guide[satin::getGuideSize()]);
		if (!ClipPoints->empty()) {
			std::copy(ClipPoints->begin(),
			          ClipPoints->end(),
			          stdext::make_checked_array_iterator(backupData->clipPoints, backupData->clipPointCount));
		}
		backupData->colors = convert_ptr<COLORREF*>(&backupData->clipPoints[ClipPoints->size()]);
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
		if (!UndoBufferWriteIndex) {
			StateMap.set(StateFlag::BAKWRAP);
		}
	}
}

void thred::internal::redfils() {
	auto findData = WIN32_FIND_DATA{};

	for (const auto iLRU : LRUMenuId) {
		if (GetMenuState(FileMenu, iLRU, MF_BYCOMMAND) != -1) {
			DeleteMenu(FileMenu, iLRU, MF_BYCOMMAND);
		}
	}
	auto& previousNames = *PreviousNames;
	for (auto iLRU = 0; iLRU < OLDNUM; iLRU++) {
		if (!previousNames[iLRU].empty()) {
			if (StateMap.test(StateFlag::SAVAS)) {
				AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], previousNames[iLRU].wstring().c_str());
			}
			else {
				auto fileHandle = FindFirstFile(previousNames[iLRU].c_str(), &findData);
				if (fileHandle == INVALID_HANDLE_VALUE) {
					previousNames[iLRU].clear();
				}
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
	for (auto iPrevious = 0u; iPrevious < OLDNUM; iPrevious++) {
		if (previousNames[iPrevious] == *ThrName) {
			if (iPrevious) {
				std::swap(previousNames[0], previousNames[iPrevious]);
				flag = false;
				break;
			}

			return;
		}
	}
	if (flag) {
		for (auto iPrevious = 0u; iPrevious < OLDNUM; iPrevious++) {
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

void thred::rngadj() noexcept {
	auto lastStitch = PCSHeader.stitchCount;
	if (lastStitch) {
		lastStitch--;
		if (ClosestPointIndex > lastStitch) {
			ClosestPointIndex = lastStitch;
		}
	}
	if (GroupStitchIndex > PCSHeader.stitchCount) {
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

void thred::mvstch(unsigned int destination, unsigned int source) noexcept {
	StitchBuffer[destination] = StitchBuffer[source];
}

void thred::internal::box(unsigned iNearest, HDC dc) noexcept {
	const auto boxWidth = BoxOffset[iNearest];
	POINT      line[5]  = {};

	line[0] = { NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y - boxWidth };
	line[1] = { NearestPixel[iNearest].x + boxWidth, NearestPixel[iNearest].y - boxWidth };
	line[2] = { NearestPixel[iNearest].x + boxWidth, NearestPixel[iNearest].y + boxWidth };
	line[3] = { NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y + boxWidth };
	line[4] = { NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y - boxWidth };
	Polyline(dc, line, 5);
}

void thred::internal::boxs() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	for (auto iNear = 0u; iNear < NearestCount; iNear++) {
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
	if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
		StitchWindowSize.x
		    = dToL((static_cast<double>(ThredWindowRect.bottom) - (gsl::narrow<long long>(*ScrollSize) << 1)) * AspectRatio);
	}
	else {
		StitchWindowSize.x = dToL((static_cast<double>(ThredWindowRect.bottom) - *ScrollSize) * AspectRatio);
	}

	if ((StitchWindowSize.x + gsl::narrow<long>(ButtonWidthX3) + *ScrollSize + *ColorBarSize) < ThredWindowRect.right) {
		if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
			StitchWindowSize.y = ThredWindowRect.bottom - (*ScrollSize << 1);
		}
		else {
			StitchWindowSize.y = ThredWindowRect.bottom - *ScrollSize;
		}
	}
	else {
		StitchWindowSize = { gsl::narrow<LONG>(ThredWindowRect.right - ButtonWidthX3 - *ColorBarSize),
			                 ThredWindowRect.bottom - ThredWindowRect.top };
		if (static_cast<double>(StitchWindowSize.x) / static_cast<double>(StitchWindowSize.y) > AspectRatio) {
			StitchWindowSize.x = dToL(StitchWindowSize.y * AspectRatio);
		}
		else {
			StitchWindowSize.y = dToL(StitchWindowSize.x / AspectRatio);
		}
	}
}

void thred::redraw(HWND window) noexcept {
	RedrawWindow(window, nullptr, nullptr, RDW_INVALIDATE);
	if (window == MainStitchWin) {
		for (auto& iWindow : DefaultColorWin) {
			if (iWindow) {
				RedrawWindow(iWindow, nullptr, nullptr, RDW_INVALIDATE);
			}
		}
		RedrawWindow(ColorBar, nullptr, nullptr, RDW_INVALIDATE);
	}
}

void thred::internal::nuRct() {
	GetClientRect(ThrEdWindow, &ThredWindowRect);
	GetWindowRect(ColorBar, &ColorBarRect);
	if (!ButtonWin->empty()) {
		GetWindowRect((*ButtonWin)[HMINLEN], &MinLenRect);
		GetWindowRect((*ButtonWin)[HMAXLEN], &MaxLenRect);
	}
	if (ColorBarDC) {
		ReleaseDC(ColorBar, ColorBarDC);
		ColorBarDC = GetDC(ColorBar);
	}
	if (StitchWindowMemDC) {
		DeleteDC(StitchWindowMemDC);
	}
	if (StitchWindowDC) {
		ReleaseDC(MainStitchWin, StitchWindowDC);
		StitchWindowDC    = GetDCEx(MainStitchWin, nullptr, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
		StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
		if (StitchWindowBmp) {
			DeleteObject(StitchWindowBmp);
		}
		GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
	}
	if (ThredDC) {
		ReleaseDC(ThrEdWindow, ThredDC);
	}
	if (ThrEdWindow) {
		ThredDC = GetDC(ThrEdWindow);
		SetStretchBltMode(ThredDC, COLORONCOLOR);
		GetDCOrgEx(ThredDC, &ThredWindowOrigin);
	}
	if (MainStitchWin) {
		GetWindowRect(MainStitchWin, &StitchWindowAbsRect);
		GetClientRect(MainStitchWin, &StitchWindowClientRect);
	}
	if (StitchWindowDC) {
		StitchWindowBmp = CreateCompatibleBitmap(StitchWindowDC, StitchWindowClientRect.right, StitchWindowClientRect.bottom);
	}
	if (StitchWindowMemDC && StitchWindowBmp) {
		SelectObject(StitchWindowMemDC, StitchWindowBmp);
	}
}

void thred::movStch() {
	auto clientSize     = POINT{ (ThredWindowRect.right - gsl::narrow<LONG>(ButtonWidthX3) - (*ScrollSize + *ColorBarSize)),
                             (ThredWindowRect.bottom) };
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
		StitchWindowAspectRatio = static_cast<double>(clientSize.x) / clientSize.y;
		if (StateMap.test(StateFlag::RUNPAT) || StateMap.test(StateFlag::WASPAT)) {
			MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, clientSize.x, *ScrollSize, TRUE);
		}
		ShowWindow(VerticalScrollBar, TRUE);
		ShowWindow(HorizontalScrollBar, TRUE);
	}
	else {
		thi::stchPars();
		const auto actualWindowHeight = StitchWindowSize.y + *ScrollSize;
		MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, StitchWindowSize.x, actualWindowHeight, TRUE);
		ShowWindow(VerticalScrollBar, FALSE);
		ShowWindow(HorizontalScrollBar, FALSE);
		StitchWindowAspectRatio = static_cast<double>(StitchWindowSize.x) / actualWindowHeight;
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
	if (SideMessageWindow) {
		DestroyWindow(SideMessageWindow);
		SideMessageWindow = nullptr;
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
	if (pixelSize < 0.2) {
		return 1.0;
	}
	if (pixelSize > 20) {
		return 20.0;
	}
	return pixelSize;
}

void thred::internal::sizstch(fRECTANGLE& rectangle, const fPOINTATTR* const stitches) noexcept {
	if (PCSHeader.stitchCount && stitches) {
		rectangle.bottom = rectangle.top = stitches[0].y;
		rectangle.left = rectangle.right = stitches[0].x;
		for (auto iStitch = 1u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (stitches[iStitch].x < rectangle.left) {
				rectangle.left = stitches[iStitch].x;
			}
			if (stitches[iStitch].x > rectangle.right) {
				rectangle.right = stitches[iStitch].x;
			}
			if (stitches[iStitch].y < rectangle.bottom) {
				rectangle.bottom = stitches[iStitch].y;
			}
			if (stitches[iStitch].y > rectangle.top) {
				rectangle.top = stitches[iStitch].y;
			}
		}
	}
	else {
		rectangle = {};
	}
}

void thred::internal::zRctAdj() noexcept {
	if (ZoomRect.top > UnzoomedRect.y) {
		const auto adjustment = ZoomRect.top - UnzoomedRect.y;
		ZoomRect.top -= adjustment;
		ZoomRect.bottom -= adjustment;
	}
	if (ZoomRect.bottom < 0) {
		ZoomRect.top -= ZoomRect.bottom;
		ZoomRect.bottom = 0;
	}
	if (ZoomRect.right > UnzoomedRect.x) {
		const auto adjustment = ZoomRect.right - UnzoomedRect.x;
		ZoomRect.right -= adjustment;
		ZoomRect.left -= adjustment;
	}
	if (ZoomRect.left < 0) {
		ZoomRect.right -= ZoomRect.left;
		ZoomRect.left = 0;
	}
}

void thred::shft(const fPOINT& delta) noexcept {
	const auto halfZoomRect = dPOINT{ ((ZoomRect.right - ZoomRect.left) / 2), ((ZoomRect.top - ZoomRect.bottom) / 2) };
	const auto center       = dPOINT{ (ZoomRect.left + halfZoomRect.x), (ZoomRect.bottom + halfZoomRect.y) };
	const auto shift        = dPOINT{ (center.x - delta.x), (center.y - delta.y) };

	ZoomRect.bottom -= shift.y;
	ZoomRect.top -= shift.y;
	ZoomRect.left -= shift.x;
	ZoomRect.right -= shift.x;

	thi::zRctAdj();
}

void thred::internal::stch2px1(unsigned iStitch) {
	StitchCoordinatesPixels.x = dToL((StitchBuffer[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5);
	StitchCoordinatesPixels.y
	    = dToL(StitchWindowClientRect.bottom - (StitchBuffer[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5);
}

void thred::internal::shft2box() {
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
	if (StateMap.testAndReset(StateFlag::ILIN)) {
		ilin();
	}
}

void thred::internal::ilin1() noexcept {
	SelectObject(StitchWindowDC, LinePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
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
	InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
}

void thred::internal::duIns() {
	InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
	stch2px1(ClosestPointIndex);
	InsertLine[0] = StitchCoordinatesPixels;
	stch2px1(ClosestPointIndex + 1);
	InsertLine[2] = StitchCoordinatesPixels;
	xlin();
	StateMap.set(StateFlag::ILIN);
	ilin();
}

void thred::internal::movins() {
	if (StateMap.test(StateFlag::INSRT)) {
		if (StateMap.test(StateFlag::LIN1)) {
			if (StateMap.test(StateFlag::BAKEND)) {
				stch2px1(PCSHeader.stitchCount - 1);
			}
			else {
				stch2px1(0);
			}
			endpnt();
		}
		else {
			duIns();
		}
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
	if (StateMap.test(StateFlag::SELBOX)) {
		thi::shft2box();
	}
	thred::duzrat();
	thi::movins();
}

void thred::internal::hupfn() {
	StateMap.reset(StateFlag::HUPCHNG);
	sizstch(CheckHoopRect, StitchBuffer);
	if (!(*FormList).empty()) {
		if (!PCSHeader.stitchCount) {
			CheckHoopRect.bottom = CheckHoopRect.top = CurrentFormVertices[0].y;
			CheckHoopRect.left = CheckHoopRect.right = CurrentFormVertices[0].x;
		}
		for (auto iVertex = 0u; iVertex < FormVertexIndex; iVertex++) {
			if (FormVertices[iVertex].x < CheckHoopRect.left) {
				CheckHoopRect.left = FormVertices[iVertex].x;
			}
			if (FormVertices[iVertex].x > CheckHoopRect.right) {
				CheckHoopRect.right = FormVertices[iVertex].x;
			}
			if (FormVertices[iVertex].y < CheckHoopRect.bottom) {
				CheckHoopRect.bottom = FormVertices[iVertex].y;
			}
			if (FormVertices[iVertex].y > CheckHoopRect.top) {
				CheckHoopRect.top = FormVertices[iVertex].y;
			}
		}
	}
	if (PCSHeader.stitchCount || FormVertexIndex || StateMap.test(StateFlag::HUPEX)) {
		if (CheckHoopRect.left < 0 || CheckHoopRect.right > IniFile.hoopSizeX || CheckHoopRect.bottom < 0
		    || CheckHoopRect.top > IniFile.hoopSizeY) {
			StateMap.set(StateFlag::HUPEX);
		}
		if (StateMap.test(StateFlag::HUPEX)) {
			const auto hoopSize = dPOINT{ CheckHoopRect.right - CheckHoopRect.left, CheckHoopRect.top - CheckHoopRect.bottom };
			if (hoopSize.x > IniFile.hoopSizeX) {
				IniFile.hoopSizeX = hoopSize.x;
				StateMap.set(StateFlag::HUPCHNG);
			}
			if (hoopSize.y > IniFile.hoopSizeY) {
				IniFile.hoopSizeY = hoopSize.y;
				StateMap.set(StateFlag::HUPCHNG);
			}
			const auto designCenter = dPOINT{ hoopSize.x / 2.0 + CheckHoopRect.left, hoopSize.y / 2.0 + CheckHoopRect.bottom };
			const auto hoopCenter   = dPOINT{ IniFile.hoopSizeX / 2.0, IniFile.hoopSizeY / 2.0 };
			const auto delta        = dPOINT{ hoopCenter.x - designCenter.x, hoopCenter.y - designCenter.y };
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iStitch].x += delta.x;
				StitchBuffer[iStitch].y += delta.y;
			}
			for (auto iVertex = 0u; iVertex < FormVertexIndex; iVertex++) {
				FormVertices[iVertex].x += delta.x;
				FormVertices[iVertex].y += delta.y;
			}
			for (auto iForm = 0u; iForm < FormIndex; iForm++) {
				auto& form = (*FormList)[iForm];
				form.rectangle.left += delta.x;
				form.rectangle.right += delta.x;
				form.rectangle.top += delta.y;
				form.rectangle.bottom += delta.y;
			}
			UnzoomedRect = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
			ZoomMin      = static_cast<double>(MINZUM) / UnzoomedRect.x;
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

double thred::internal::bufToDouble(wchar_t* buffer) {
	auto value = 0.0;

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
	auto value = 0ull;

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
	auto value = 0.0;
	if (wcslen(MsgBuffer)) {
		value = bufToDouble(MsgBuffer);
	}

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
			case LTXOF: {
				thred::savdo();
				SelectedForm->txof = value;
				break;
			}
			case LUANG: {
				thred::savdo();
				SelectedForm->underlayStitchAngle = value / 180 * PI / PFGRAN;
				break;
			}
			case LUSPAC: {
				thred::savdo();
				SelectedForm->underlaySpacing = value;
				break;
			}
			case LWLKIND: {
				thred::savdo();
				SelectedForm->underlayIndent = value;
				break;
			}
			case LULEN: {
				thred::savdo();
				SelectedForm->underlayStitchLen = value;
				break;
			}
			case LDSTRT: {
				thred::savdo();
				SelectedForm->fillStart = gsl::narrow<unsigned int>(std::round(value / PFGRAN));
				SelectedForm->fillStart %= VertexCount;
				break;
			}
			case LDEND: {
				thred::savdo();
				SelectedForm->fillEnd = gsl::narrow<unsigned int>(std::round(value / PFGRAN));
				SelectedForm->fillEnd %= VertexCount;
				break;
			}
			case LFTHUPCNT: {
				thred::savdo();
				value /= PFGRAN;
				if (value > 255.0) {
					value = 255.0;
				}
				SelectedForm->fillInfo.feather.upCount = gsl::narrow<unsigned char>(std::round(value));
				break;
			}
			case LFTHCOL: {
				if (value) {
					thred::savdo();
					form::nufthcol((bufTou64(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText((*ValueWindow)[LFTHCOL], SideWindowEntryBuffer);
					thred::coltab();
				}
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return;
			}
			case LFRMCOL: {
				if (value) {
					thred::savdo();
					form::nufilcol((bufTou64(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText((*ValueWindow)[LFRMCOL], SideWindowEntryBuffer);
					thred::coltab();
				}
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return;
			}
			case LUNDCOL: {
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
			}
			case LBRDCOL: {
				if (value) {
					thred::savdo();
					form::nubrdcol((bufTou64(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText((*ValueWindow)[LFRMCOL], SideWindowEntryBuffer);
					thred::coltab();
				}
				thred::unsid();
				StateMap.set(StateFlag::RESTCH);
				return;
			}
			case LBRDPIC: {
				thred::savdo();
				SelectedForm->edgeSpacing = value;
				thred::unsid();
				SetWindowText((*ValueWindow)[LBRDPIC], SideWindowEntryBuffer);
				form::refil();
				return;
			}
			case LFRMFAZ: {
				thred::savdo();
				SelectedForm->wordParam = gsl::narrow<unsigned int>(std::floor(value / PFGRAN));
				thred::unsid();
				SetWindowText((*ValueWindow)[LFRMFAZ], SideWindowEntryBuffer);
				form::refil();
				return;
			}
			case LBRDPOS: {
				thred::savdo();
				SelectedForm->edgeStitchLen = value / PFGRAN;
				thred::unsid();
				SetWindowText((*ValueWindow)[LBRDPOS], SideWindowEntryBuffer);
				form::refil();
				return;
			}
			case LMAXFIL: {
				thred::savdo();
				SelectedForm->maxFillStitchLen = value;
				thred::unsid();
				SetWindowText((*ValueWindow)[LMAXFIL], SideWindowEntryBuffer);
				form::refil();
				return;
			}
			case LMINFIL: {
				thred::savdo();
				SelectedForm->minFillStitchLen = value;
				thred::unsid();
				SetWindowText((*ValueWindow)[LMINFIL], SideWindowEntryBuffer);
				form::refil();
				return;
			}
			case LMAXBRD: {
				thred::savdo();
				SelectedForm->maxBorderStitchLen = value;
				thred::unsid();
				SetWindowText((*ValueWindow)[LMAXBRD], SideWindowEntryBuffer);
				form::refil();
				return;
			}
			case LMINBRD: {
				thred::savdo();
				SelectedForm->minBorderStitchLen = value;
				thred::unsid();
				SetWindowText((*ValueWindow)[LMINBRD], SideWindowEntryBuffer);
				form::refil();
				return;
			}
			}
			if (FormMenuChoice == LBCSIZ) {
				thred::savdo();
				if (SelectedForm->edgeType == EDGEBHOL) {
					form::savblen(static_cast<float>(value));
				}
				else {
					form::savplen(static_cast<float>(value));
				}
			}
			else {
				if (value) {
					switch (FormMenuChoice) {
					case LFTHSIZ: {
						thred::savdo();
						SelectedForm->fillInfo.feather.ratio = value / PFGRAN;
						break;
					}
					case LFTHNUM: {
						thred::savdo();
						SelectedForm->fillInfo.feather.count = gsl::narrow<unsigned short>(std::round(value / PFGRAN));
						break;
					}
					case LFTHFLR: {
						thred::savdo();
						SelectedForm->fillInfo.feather.minStitchSize = value;
						break;
					}
					case LFTHDWNCNT: {
						thred::savdo();
						value /= PFGRAN;
						if (value > 255.0) {
							value = 255.0;
						}
						SelectedForm->fillInfo.feather.downCount = gsl::narrow<unsigned char>(std::round(value));
						break;
					}
					case LFRMSPAC: {
						thred::savdo();
						SelectedForm->fillSpacing = value;
						break;
					}
					case LFRMLEN: {
						thred::savdo();
						SelectedForm->lengthOrCount.stitchLength = value;
						break;
					}
					case LBRDSPAC: {
						thred::savdo();
						const auto edgeType = SelectedForm->edgeType & NEGUND;
						switch (edgeType) {
						case EDGEPROPSAT: {
							SelectedForm->edgeSpacing = value;
							break;
						}
						case EDGEOCHAIN:
						case EDGELCHAIN: {
							SelectedForm->edgeSpacing = value;
							break;
						}
						default: { SelectedForm->edgeSpacing = value / 2; }
						}
						break;
					}
					case LBRDLEN: {
						thred::savdo();
						SelectedForm->edgeStitchLen = value;
						break;
					}
					case LBRDSIZ: {
						thred::savdo();
						SelectedForm->borderSize = value;
						break;
					}
					case LFRMANG: {
						thred::savdo();
						SelectedForm->angleOrClipData.angle = value / 180 * PI / PFGRAN;
						break;
					}
					case LSACANG: {
						thred::savdo();
						SelectedForm->satinOrAngle.angle = value / 180 * PI / PFGRAN;
						break;
					}
					case LAPCOL: {
						thred::savdo();
						SelectedForm->borderColor &= COLMSK;
						auto borderColor = static_cast<unsigned char>(std::round(value / PFGRAN));
						if (borderColor) {
							borderColor--;
						}
						borderColor &= COLMSK;
						SelectedForm->borderColor |= (borderColor << 4);
						break;
					}
					}
				}
				else {
					thred::savdo();
					if (FormMenuChoice == LFRMSPAC && isfclp()) {
						SelectedForm->fillSpacing = 0;
					}
				}
			}
			thred::unsid();
			form::refil();
			formForms::refrm();
		}
		else {
			if (PreferenceIndex) {
				value = bufToDouble(SideWindowEntryBuffer);
				switch (PreferenceIndex - 1) {
				case PEG: {
					IniFile.eggRatio = value;
					SetWindowText((*ValueWindow)[PEG], fmt::format(L"{:.2f}", value).c_str());
					break;
				}
				case PNUDG: {
					IniFile.cursorNudgeStep = value;
					IniFile.nudgePixels     = gsl::narrow<unsigned short>(std::round(pxchk(value)));
					SetWindowText((*ValueWindow)[PNUDG], fmt::format(L"{:.2f}", value).c_str());
					break;
				}
				case PPIC: {
					PicotSpacing = value * PFGRAN;
					SetWindowText((*ValueWindow)[PPIC], fmt::format(L"{:.2f}", value).c_str());
					break;
				}
				case PCLPOF: {
					IniFile.clipOffset = value * PFGRAN;
					SetWindowText((*ValueWindow)[PCLPOF], fmt::format(L"{:.2f} mm", value).c_str());
					break;
				}
				case PFAZ: {
					IniFile.fillPhase = gsl::narrow<unsigned int>(std::floor(value));
					SetWindowText((*ValueWindow)[PFAZ], fmt::format(L"{}", IniFile.fillPhase).c_str());
					break;
				}
				case PCHRAT: {
					IniFile.chainRatio = value;
					SetWindowText((*ValueWindow)[PCHRAT], fmt::format(L"{:.2f}", value).c_str());
					break;
				}
				case PMIN: {
					MinStitchLength = value * PFGRAN;
					SetWindowText((*ValueWindow)[PMIN], fmt::format(L"{:.2f}", value).c_str());
					break;
				}
				default: {
					if (value) {
						auto bufVal = fmt::format(L"{:.2f}", value);
						switch (PreferenceIndex - 1) {
						case PSPAC: {
							LineSpacing = value * PFGRAN;
							SetWindowText((*ValueWindow)[PSPAC], bufVal.c_str());
							break;
						}
						case PANGL: {
							IniFile.fillAngle = value / 180 * PI;
							SetWindowText((*ValueWindow)[PANGL], bufVal.c_str());
							break;
						}
						case PSAT: {
							BorderWidth         = value * PFGRAN;
							IniFile.borderWidth = BorderWidth;
							SetWindowText((*ValueWindow)[PSAT], bufVal.c_str());
							break;
						}
						case PMAX: {
							IniFile.maxStitchLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PMAX], bufVal.c_str());
							break;
						}
						case PUSE: {
							UserStitchLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PUSE], bufVal.c_str());
							break;
						}
						case PSMAL: {
							SmallStitchLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PSMAL], bufVal.c_str());
							break;
						}
						case PAP: {
							AppliqueColor = gsl::narrow<unsigned>(std::round(value - 1)) % 16;
							SetWindowText((*ValueWindow)[PAP], fmt::format(L"{}", (AppliqueColor + 1)).c_str());
							break;
						}
						case PSNP: {
							SnapLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PSNP], bufVal.c_str());
							break;
						}
						case PSTAR: {
							StarRatio = value;
							if (StarRatio > 1) {
								StarRatio = 1;
							}
							if (StarRatio < 0.05) {
								StarRatio = 0.05;
							}
							SetWindowText((*ValueWindow)[PSTAR], fmt::format(L"{:.2f}", StarRatio).c_str());
							break;
						}
						case PSPIR: {
							SpiralWrap = value;
							if (StarRatio > 20) {
								StarRatio = 20.;
							}
							if (StarRatio < 0.3) {
								StarRatio = 0.3;
							}
							SetWindowText((*ValueWindow)[PSPIR], fmt::format(L"{:.2f}", SpiralWrap).c_str());
							break;
						}
						case PBUT: {
							ButtonholeCornerLength = value * PFGRAN;
							SetWindowText((*ValueWindow)[PBUT], fmt::format(L"{:.2f}", value).c_str());
							break;
						}
						case PHUPX: {
							IniFile.hoopSizeX = value * PFGRAN;
							SetWindowText((*ValueWindow)[PHUPX], fmt::format(L"{:.0f} mm", value).c_str());
							form::sethup();
							formForms::prfmsg();
							chkhup();
							break;
						}
						case PHUPY: {
							IniFile.hoopSizeY = value * PFGRAN;
							SetWindowText((*ValueWindow)[PHUPY], fmt::format(L"{:.0f} mm", value).c_str());
							form::sethup();
							formForms::prfmsg();
							chkhup();
							break;
						}
						case PGRD: {
							IniFile.gridSize = value * PFGRAN;
							SetWindowText((*ValueWindow)[PGRD], fmt::format(L"{:.2f} mm", value).c_str());
							break;
						}
						case PCHN: {
							IniFile.chainSpace = value * PFGRAN;
							SetWindowText((*ValueWindow)[PCHN], fmt::format(L"{:.2f}", value).c_str());
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
				if (wcslen(MsgBuffer)) {
					value = bufToDouble(MsgBuffer);
					// ToDo - figure out why MsgBuffer gets large value and crashes gsl::narrow below
					for (auto i = 0u; i < 64; i++) {
						MsgBuffer[i] = 0x0;
					}
					do {
						if (StateMap.testAndReset(StateFlag::ENTRFNUM)) {
							if (value < FormIndex) {
								form::frmnumfn(dToUI(value));
							}
							else {
								displayText::tabmsg(IDS_FRMN1);
							}
							return;
						}
						auto uintValue = gsl::narrow<unsigned int>(std::floor(value));
						if (StateMap.testAndReset(StateFlag::ENTRPOL)) {
							form::durpoli(uintValue);
							break;
						}
						if (StateMap.testAndReset(StateFlag::ENTRSTAR)) {
							form::dustar(uintValue,
							             250.0 / value * ZoomFactor * (static_cast<double>(UnzoomedRect.x) + UnzoomedRect.y)
							                 / (static_cast<double>(LHUPX) + LHUPY));
							break;
						}
						if (StateMap.testAndReset(StateFlag::ENTRSPIR)) {
							form::duspir(uintValue);
							break;
						}
						if (StateMap.testAndReset(StateFlag::ENTRHART)) {
							form::duhart(uintValue);
							break;
						}
						if (StateMap.testAndReset(StateFlag::ENTRLENS)) {
							form::dulens(uintValue);
							break;
						}
						if (StateMap.testAndReset(StateFlag::ENTREG)) {
							form::dueg(uintValue);
							break;
						}
						if (StateMap.testAndReset(StateFlag::ENTRZIG)) {
							form::duzig(uintValue);
							break;
						}
						if (StateMap.testAndReset(StateFlag::PIXIN)) {
							IniFile.nudgePixels = gsl::narrow<unsigned short>(std::round(pxchk(value)));
							break;
						}
						if (StateMap.testAndReset(StateFlag::STPXIN)) {
							IniFile.stitchSizePixels = gsl::narrow<unsigned short>(std::round(pxchk(value)));
							break;
						}
						if (StateMap.testAndReset(StateFlag::FRMPXIN)) {
							IniFile.formVertexSizePixels = gsl::narrow<unsigned short>(std::round(value));
							break;
						}
						if (StateMap.testAndReset(StateFlag::FRMBOXIN)) {
							IniFile.formBoxSizePixels = gsl::narrow<unsigned short>(std::round(value));
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
							if (value) {
								xt::dufhi(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFWID)) {
							if (value) {
								xt::dufwid(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFMAX)) {
							if (value) {
								xt::dufmax(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFMIN)) {
							xt::dufmin(value * PFGRAN);
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETBMAX)) {
							if (value) {
								xt::dubmax(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETBMIN)) {
							xt::dubmin(value * PFGRAN);
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETBSPAC)) {
							if (value) {
								xt::dubspac(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFLEN)) {
							if (value) {
								xt::dublen(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETBCOL)) {
							xt::dubcol(dToUI(value));
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFCOL)) {
							xt::dufcol(dToUI(value));
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETUCOL)) {
							xt::dundcol(dToUI(value));
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFANG)) {
							xt::dufxang(value);
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFSPAC)) {
							if (value) {
								xt::dufspac(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETUANG)) {
							xt::dufang(value);
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETFLEN)) {
							if (value) {
								xt::duflen(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETUSPAC)) {
							if (value) {
								xt::duspac(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::FSETULEN)) {
							if (value) {
								xt::dusulen(value * PFGRAN);
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::GTUANG)) {
							IniFile.underlayAngle = value / 180 * PI;
							break;
						}
						if (StateMap.testAndReset(StateFlag::GTUSPAC)) {
							if (value) {
								IniFile.underlaySpacing = value * PFGRAN;
							}
							break;
						}
						if (StateMap.testAndReset(StateFlag::GTWLKIND)) {
							IniFile.underlayIndent = value * PFGRAN;
							break;
						}
						if (StateMap.testAndReset(StateFlag::GTWLKLEN)) {
							if (value) {
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
	if (DestroyWindow(MsgWindow)) {
		MsgWindow = nullptr;
	}
	DestroyWindow(OKButton);
	DestroyWindow(DiscardButton);
	DestroyWindow(CancelButton);
	DestroyWindow(DeleteStitchesDialog);
	if (GeneralNumberInputBox) {
		if (DestroyWindow(GeneralNumberInputBox)) {
			GeneralNumberInputBox = nullptr;
		}
	}
	if (StateMap.testAndReset(StateFlag::NUMIN) || FormMenuChoice || PreferenceIndex) {
		chknum();
	}
	StateMap.reset(StateFlag::MOVMSG);
	StateMap.reset(StateFlag::DELFRM);
	StateMap.reset(StateFlag::FILMSG);
	StateMap.reset(StateFlag::FORMIN);
	StateMap.set(StateFlag::RESTCH);
}

void thred::unmsg() {
	if (MsgWindow) {
		thi::noMsg();
	}
}

GSL_SUPPRESS(26461) bool thred::internal::oldwnd(HWND window) {
	for (auto iColor = 0u; iColor < 16; iColor++) {
		if (DefaultColorWin[iColor] == window || UserColorWin[iColor] == window || ThreadSizeWin[iColor] == window) {
			return false;
		}
	}
	for (auto iWindow = 0u; iWindow < 9; iWindow++) {
		if ((*ButtonWin)[iWindow] == window) {
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

BOOL CALLBACK thred::internal::EnumChildProc(HWND hwnd, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	if (oldwnd(hwnd)) {
		return DestroyWindow(hwnd);
	}
	if (FirstWin) {
		if (FirstWin == hwnd) {
			return false;
		}
		return true;
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
	displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
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
	SearchLineIndex = 0;
	FirstWin        = nullptr;
	while (EnumChildWindows(MainStitchWin, thi::EnumChildProc, 0)) {
	}
}

void thred::ritot(unsigned number) {
	auto txt         = fmt::format((*StringTable)[STR_TOT], number);
	BufferDigitCount = gsl::narrow<unsigned int>(txt.size());
	displayText::butxt(HTOT, txt);
}

void thred::internal::frmcalc() {
	auto& form = (*FormList)[ClosestFormToCursor];
	if (form.fillType || form.edgeType) {
		const auto code      = ClosestFormToCursor << FRMSHFT;
		auto       endStitch = PCSHeader.stitchCount;
		auto       maxLength = 0.0;
		auto       minLength = 1e99;
		if (PCSHeader.stitchCount) {
			endStitch--;
			for (auto iStitch = 0u; iStitch < endStitch; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) == code && !(StitchBuffer[iStitch].attribute & NOTFRM)
				    && (StitchBuffer[iStitch + 1].attribute & FRMSK) == code && (StitchBuffer[iStitch + 1].attribute & TYPMSK)) {
					const auto length = std::hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x,
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
		}
		if (fabs(maxLength < 10000)) {
			auto strMax = std::wstring{};
			displayText::loadString(strMax, IDS_LENMAX);
			displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength / PFGRAN)));
		}
		if (fabs(minLength < 10000)) {
			auto strMin = std::wstring{};
			displayText::loadString(strMin, IDS_LENMIN);
			displayText::butxt(HMINLEN, fmt::format(strMin, (minLength / PFGRAN)));
		}
	}
	else {
		const auto blank = std::wstring{};
		displayText::butxt(HMAXLEN, blank);
		displayText::butxt(HMINLEN, blank);
	}
}

void thred::internal::lenfn(unsigned start, unsigned end) {
	auto maxLength = 0.0;
	auto minLength = 1e99;

	SmallestStitchIndex = 0u;
	LargestStitchIndex  = 0u;
	for (auto iStitch = start; iStitch < end; iStitch++) {
		const auto length
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
	auto strMax = std::wstring{};
	auto strMin = std::wstring{};
	displayText::loadString(strMax, IDS_LENMAX);
	displayText::loadString(strMin, IDS_LENMIN);
	displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength / PFGRAN)));
	displayText::butxt(HMINLEN, fmt::format(strMin, (minLength / PFGRAN)));
}

void thred::internal::lenCalc() {
	const auto blank = std::wstring{};

	if (StateMap.test(StateFlag::LENSRCH)) {
		auto       txt    = std::wstring{};
		const auto lenMax = (hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x,
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
			if (StateMap.test(StateFlag::GRPSEL) && GroupStartStitch != GroupEndStitch) {
				lenfn(GroupStartStitch, GroupEndStitch);
			}
			else {
				lenfn(0, PCSHeader.stitchCount - 1);
			}
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

	const auto codedAttribute = ClosestFormToCursor << 4;
	auto       stitchSize     = 1e99;

	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		auto iNextStitch = form::find1st();
		auto iStitch     = iNextStitch + 1;
		auto lastStitch  = PCSHeader.stitchCount;
		if (lastStitch) {
			lastStitch--;
			while (iStitch < lastStitch && stitchSize > SmallStitchLength) {
				if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute) {
					if (StitchBuffer[iStitch].attribute & KNOTMSK) {
						iNextStitch = iStitch;
					}
					else {
						const auto delta = POINT{ fToL(StitchBuffer[iStitch].x - StitchBuffer[iNextStitch].x),
							                      fToL(StitchBuffer[iStitch].y - StitchBuffer[iNextStitch].y) };
						stitchSize       = hypot(delta.x, delta.y);
					}
					iStitch++;
				}
			}
		}
		if (iStitch != endStitch - 2) {
			iStitch--;
			iNextStitch = iStitch + 2;
			lastStitch  = PCSHeader.stitchCount;
			if (lastStitch) {
				lastStitch--;
				while (iNextStitch < lastStitch) {
					do {
						if (!(StitchBuffer[iStitch].attribute & NOTFRM)
						    && (StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute
						    && !(StitchBuffer[iStitch].attribute & KNOTMSK)) {
							const auto delta = POINT{ fToL(StitchBuffer[iNextStitch].x - StitchBuffer[iStitch].x),
								                      fToL(StitchBuffer[iNextStitch++].y - StitchBuffer[iStitch].y) };
							stitchSize       = hypot(delta.x, delta.y);
						}
						else {
							iNextStitch++;
						}
					} while (stitchSize < SmallStitchLength && iNextStitch < PCSHeader.stitchCount);
					StitchBuffer[++iStitch] = StitchBuffer[--iNextStitch];
				}
			}
			iStitch++;
			while (iNextStitch < PCSHeader.stitchCount) {
				StitchBuffer[iStitch++] = StitchBuffer[iNextStitch++];
			}
			PCSHeader.stitchCount = gsl::narrow<unsigned short>(iStitch);
			thred::coltab();
		}
	}
	else {
		const auto iStitch     = startStitch;
		auto       iNextStitch = startStitch + 1;
		SelectedPoint          = StitchBuffer[iStitch];
		for (auto inf = iNextStitch; inf < endStitch; inf++) {
			if (StitchBuffer[iNextStitch].attribute & KNOTMSK) {
				SelectedPoint = StitchBuffer[iNextStitch];
				thred::mvstch(iNextStitch++, inf);
			}
			else {
				const auto delta
				    = POINT{ fToL(StitchBuffer[inf].x - SelectedPoint.x), fToL(StitchBuffer[inf].y - SelectedPoint.y) };
				stitchSize = hypot(delta.x, delta.y);
				if (stitchSize > SmallStitchLength) {
					thred::mvstch(iNextStitch++, inf);
					SelectedPoint = StitchBuffer[inf];
				}
			}
		}
		auto inf = endStitch;
		while (inf < PCSHeader.stitchCount) {
			thred::mvstch(iNextStitch++, inf++);
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(iNextStitch);
		thred::coltab();
	}
	rstAll();
	thred::ritot(PCSHeader.stitchCount);
	lenCalc();
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::duzero() {
	if (!SelectedFormList->empty()) {
		auto formMap = boost::dynamic_bitset<>(FormIndex);
		for (auto selectedForm : (*SelectedFormList)) {
			formMap.set(selectedForm);
		}
		StateMap.reset(StateFlag::CONTIG);
		auto iDestination  = 0u;
		auto currentStitch = StitchBuffer;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute & TYPMSK && formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
				if (StateMap.testAndSet(StateFlag::CONTIG)) {
					const auto stitchLength
					    = hypot(StitchBuffer[iStitch].x - currentStitch->x, StitchBuffer[iStitch].y - currentStitch->y);
					if (stitchLength > MinStitchLength) {
						currentStitch                = &StitchBuffer[iStitch];
						StitchBuffer[iDestination++] = StitchBuffer[iStitch];
					}
				}
				else {
					currentStitch = &StitchBuffer[iStitch];
				}
			}
			else {
				StitchBuffer[iDestination++] = StitchBuffer[iStitch];
				StateMap.reset(StateFlag::CONTIG);
			}
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(iDestination);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		delsmal(GroupStartStitch, GroupEndStitch);
	}
	else {
		delsmal(0, PCSHeader.stitchCount);
	}
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
	if (StateMap.test(StateFlag::ZUMED)) {
		const auto scrollPosition = POINT{ 0, dToL((ZoomRect.top - ZoomRect.bottom) * PAGSCROL) };
		rshft(scrollPosition);
	}
}

void thred::internal::pgup() {
	if (StateMap.test(StateFlag::ZUMED)) {
		const auto scrollPosition = POINT{ 0, dToL(-(ZoomRect.top - ZoomRect.bottom) * PAGSCROL) };
		rshft(scrollPosition);
	}
}

void thred::internal::pglft() {
	if (StateMap.test(StateFlag::ZUMED)) {
		const auto scrollPosition = POINT{ dToL((ZoomRect.right - ZoomRect.left) * PAGSCROL), 0 };
		rshft(scrollPosition);
	}
}

void thred::internal::pgrit() {
	if (StateMap.test(StateFlag::ZUMED)) {
		const auto scrollPosition = POINT{ dToL(-(ZoomRect.right - ZoomRect.left) * PAGSCROL), 0 };
		rshft(scrollPosition);
	}
}

void thred::internal::selin(unsigned start, unsigned end, HDC dc) {
	SelectObject(dc, GroupSelectPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	if (SearchLineIndex) {
		PolylineInt(dc, SearchLine, SearchLineIndex);
	}
	if (start > end) {
		std::swap(start, end);
	}
	SearchLineIndex = 0;
	for (auto iStitch = start; iStitch <= end; iStitch++) {
		auto coordinate               = ((StitchBuffer[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5);
		SearchLine[SearchLineIndex].x = dToL(coordinate);
		coordinate = (StitchWindowClientRect.bottom - (StitchBuffer[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5);
		SearchLine[SearchLineIndex++].y = dToL(coordinate);
	}
	PolylineInt(dc, SearchLine, SearchLineIndex);
	SetROP2(dc, R2_COPYPEN);
}

void thred::internal::cros(unsigned iStitch) {
	const auto armLength = BoxOffset[0];

	stch2px1(iStitch);
	InsertLine[0] = { StitchCoordinatesPixels.x - armLength, StitchCoordinatesPixels.y };
	InsertLine[1] = { StitchCoordinatesPixels.x + armLength, StitchCoordinatesPixels.y };
	SelectObject(StitchWindowDC, CrossPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
	SelectObject(StitchWindowMemDC, CrossPen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	InsertLine[0] = { StitchCoordinatesPixels.x, StitchCoordinatesPixels.y - armLength };
	InsertLine[1] = { StitchCoordinatesPixels.x, StitchCoordinatesPixels.y - 1 };
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
	sourceRect.left = sourceRect.right = StitchBuffer[GroupStartStitch].x;
	sourceRect.top = sourceRect.bottom = StitchBuffer[GroupStartStitch].y;
	for (auto iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
		if (StitchBuffer[iStitch].x > sourceRect.right) {
			sourceRect.right = StitchBuffer[iStitch].x;
		}
		if (StitchBuffer[iStitch].x < sourceRect.left) {
			sourceRect.left = StitchBuffer[iStitch].x;
		}
		if (StitchBuffer[iStitch].y < sourceRect.bottom) {
			sourceRect.bottom = StitchBuffer[iStitch].y;
		}
		if (StitchBuffer[iStitch].y > sourceRect.top) {
			sourceRect.top = StitchBuffer[iStitch].y;
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
	PolylineInt(dc, FormControlPoints->data(), gsl::narrow<unsigned int>(FormControlPoints->size() - 1));
	for (auto iPoint = 0u; iPoint < 8; iPoint++) {
		form::selsqr((*FormControlPoints)[iPoint], dc);
	}
	SetROP2(dc, R2_COPYPEN);
}

void thred::internal::unsel() {
	if (StateMap.testAndReset(StateFlag::SELSHO)) {
		dusel(StitchWindowDC);
	}
}

void thred::internal::nuselrct() {
	fPOINT outline[9] = {};

	unsel();
	outline[0].x = outline[6].x = outline[7].x = outline[8].x = StitchRangeRect.left;
	outline[1].x = outline[5].x = form::midl(StitchRangeRect.right, StitchRangeRect.left);
	outline[0].y = outline[1].y = outline[2].y = outline[8].y = StitchRangeRect.top;
	outline[3].y = outline[7].y = form::midl(StitchRangeRect.top, StitchRangeRect.bottom);
	outline[4].y = outline[5].y = outline[6].y = StitchRangeRect.bottom;
	outline[2].x = outline[3].x = outline[4].x = StitchRangeRect.right;
	for (auto iLine = 0u; iLine < 9; iLine++) {
		form::sfCor2px(outline[iLine], (*FormControlPoints)[iLine]);
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
		if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1
		    || StitchRangeRect.left < ZoomRect.left + 1 || StitchRangeRect.right > ZoomRect.right - 1) {
			auto newSize
			    = POINT{ fToL(StitchRangeRect.right - StitchRangeRect.left), fToL(StitchRangeRect.top - StitchRangeRect.bottom) };
			auto coordinate = 0.0;
			if (newSize.x < MINZUM) {
				// ToDo - we are comparing an integer to a small float here - does that make sense?
				if (newSize.x < TINY) {
					newSize.x = 1;
				}
				coordinate = MINZUM / newSize.x;
				newSize    = { MINZUM, dToL(coordinate * newSize.y) };
			}
			if (newSize.x > newSize.y) {
				coordinate = newSize.x * ZMARGIN;
				newSize.x += dToL(coordinate);
				coordinate = newSize.x / StitchWindowAspectRatio;
				newSize.y  = dToL(coordinate);
			}
			else {
				coordinate = newSize.y * ZMARGIN;
				newSize.y  = dToL(coordinate);
				coordinate = newSize.y * StitchWindowAspectRatio;
				newSize.x  = dToL(coordinate);
			}
			if (newSize.x > UnzoomedRect.x || newSize.y > UnzoomedRect.y) {
				ZoomRect.left = ZoomRect.bottom = 0.0;
				ZoomRect.right                  = UnzoomedRect.x;
				ZoomRect.top                    = UnzoomedRect.y;
				StateMap.reset(StateFlag::ZUMED);
				ZoomFactor = 1.0;
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
	const auto color = ActiveColor;
	ActiveColor      = StitchBuffer[iStitch].attribute & COLMSK;
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
	auto layer = 0xffffffffu;

	if (StateMap.test(StateFlag::SELBOX)) {
		layer = (StitchBuffer[ClosestPointIndex].attribute & LAYMSK) >> LAYSHFT;
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::FRMPSEL)) {
			layer = ((*FormList)[ClosestFormToCursor].attribute & FRMLMSK) >> 1;
		}
	}
	if (layer & 0xffff0000u) {
		const auto blank = std::wstring{};
		displayText::butxt(HLAYR, blank);
	}
	else {
		auto txt         = fmt::format((*StringTable)[STR_LAYR], layer);
		BufferDigitCount = gsl::narrow<unsigned int>(txt.size());
		displayText::butxt(HLAYR, txt);
	}
}

HPEN thred::internal::nuPen(HPEN pen, unsigned width, COLORREF color) noexcept {
	DeleteObject(pen);
	return CreatePenInt(PS_SOLID, width, color);
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

	const auto dx               = unrotatedPoint.x - rotationCenterPixels.x;
	const auto dy               = unrotatedPoint.y - rotationCenterPixels.y;
	const auto distanceToCenter = hypot(dx, dy);
	const auto newAngle         = atan2(dy, dx) - RotateAngle;

	rotatedPoint = { dToL(rotationCenterPixels.x + distanceToCenter * cos(newAngle)),
		             dToL(rotationCenterPixels.y + distanceToCenter * sin(newAngle)) };
}

void thred::internal::duar() {
	const auto offset = gsl::narrow<LONG>(MulDiv(10, *screenDPI, 96));

	auto arrowCenter = POINT{ (StitchCoordinatesPixels.x - offset), (StitchCoordinatesPixels.y + offset) };

	StitchArrow[1] = StitchCoordinatesPixels;
	rotpix(arrowCenter, StitchArrow[0], StitchCoordinatesPixels);
	arrowCenter.y = StitchCoordinatesPixels.y - offset;
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

bool thred::internal::stch2px(unsigned iStitch) {
	StitchCoordinatesPixels
	    = { dToL((StitchBuffer[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
		    dToL(StitchWindowClientRect.bottom - (StitchBuffer[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5) };
	return StitchCoordinatesPixels.x >= 0 && StitchCoordinatesPixels.x <= StitchWindowClientRect.right
	       && StitchCoordinatesPixels.y >= 0 && StitchCoordinatesPixels.y <= StitchWindowClientRect.bottom;
}

void thred::stch2pxr(const fPOINT& stitchCoordinate) {
	StitchCoordinatesPixels
	    = { dToL((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x + 0.5),
		    dToL(StitchWindowClientRect.bottom - (stitchCoordinate.y - ZoomRect.bottom) * ZoomRatio.y + 0.5) };
}

void thred::internal::getDocsFolder(fs::path* directory) {
	if (directory) {
		auto       ppszPath = PWSTR{ nullptr }; // variable to receive the path memory block pointer.
		const auto hr       = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &ppszPath);
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
	auto           directory = utf::Utf16ToUtf8(DefaultDirectory->wstring());
	constexpr char fillchar  = '\0';
	std::fill(IniFile.defaultDirectory, IniFile.defaultDirectory + sizeof(IniFile.defaultDirectory), fillchar);
	std::copy(directory.begin(), directory.end(), IniFile.defaultDirectory);
	const auto& previousNames = *PreviousNames;
	{
		auto iVersion = 0u;
		for (auto& prevName : IniFile.prevNames) {
			std::fill(prevName, prevName + sizeof(prevName), fillchar);
			if (!previousNames[iVersion].empty()) {
				auto previous = utf::Utf16ToUtf8(previousNames[iVersion]);
				std::copy(previous.begin(), previous.end(), prevName);
			}
			iVersion++;
		}
	}
	auto designer = utf::Utf16ToUtf8(*DesignerName);
	std::fill(IniFile.designerName, IniFile.designerName + sizeof(IniFile.designerName), fillchar);
	std::copy(designer.begin(), designer.end(), IniFile.designerName);
	for (auto iColor = 0u; iColor < 16; iColor++) {
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
		auto windowRect = RECT{};
		GetWindowRect(ThrEdWindow, &windowRect);
		IniFile.initialWindowCoords.left   = windowRect.left;
		IniFile.initialWindowCoords.right  = windowRect.right;
		IniFile.initialWindowCoords.bottom = windowRect.bottom;
		IniFile.initialWindowCoords.top    = windowRect.top;
	}
	IniFileHandle
	    = CreateFile(IniFileName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (IniFileHandle != INVALID_HANDLE_VALUE) {
		WriteFile(IniFileHandle, &IniFile, sizeof(IniFile), &BytesRead, nullptr);
	}
	CloseHandle(IniFileHandle);
}

bool thred::internal::savcmp() noexcept {
#ifdef _DEBUG

	return true;

#else

	return !StateMap.test(StateFlag::CMPDO);

#endif
}

void thred::internal::thr2bal(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code) {
	constexpr auto BalaradRatio = 10.0f / 6.0f;

	balaradStitch[destination].flag = 0;
	balaradStitch[destination].code = gsl::narrow<unsigned char>(code);
	balaradStitch[destination].x    = (StitchBuffer[source].x - BalaradOffset.x) * BalaradRatio;
	balaradStitch[destination].y    = (StitchBuffer[source].y - BalaradOffset.y) * BalaradRatio;
}

constexpr DWORD thred::internal::coldis(COLORREF colorA, COLORREF colorB) {
	auto distance
	    = ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	colorA >>= 8;
	colorB >>= 8;
	distance += ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	colorA >>= 8;
	colorB >>= 8;
	distance += ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	return distance;
}

void thred::internal::bal2thr(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code) {
	constexpr auto IBALRAT = 0.6f;

	StitchBuffer[destination]
	    = { balaradStitch[source].x * IBALRAT + BalaradOffset.x, balaradStitch[source].y * IBALRAT + BalaradOffset.y, code };
}

unsigned thred::internal::colmatch(COLORREF color) noexcept {
	if (ColorChanges < 16) {
		for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
			if (color == UserColor[iColor]) {
				return iColor;
			}
		}
		UserColor[ColorChanges++] = color;
		return ColorChanges - 1;
	}

	auto minDistance = DWORD{ 0xffffffffu };
	auto iDistance   = DWORD{ 0x00000000u };
	for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
		const auto distance = coldis(color, UserColor[iColor]);
		if (!distance) {
			return iColor;
		}
		iDistance = 0u;
		if (distance < minDistance) {
			iDistance   = iColor;
			minDistance = distance;
		}
	}
	return iDistance;
}

void thred::internal::redbal() {
	auto balaradHeader = BALHED{};

	PCSHeader.stitchCount = 0u;
	FormIndex             = 0u;

	auto balaradFile = CreateFile(BalaradName2->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (balaradFile != INVALID_HANDLE_VALUE) {
		auto bytesRead = DWORD{ 0u };
		ReadFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesRead, nullptr);
		if (bytesRead == sizeof(balaradHeader)) {
			auto balaradStitch = std::vector<BALSTCH>(MAXITEMS);
			ReadFile(balaradFile, balaradStitch.data(), MAXITEMS * sizeof(balaradStitch[0]), &bytesRead, nullptr);
			const auto stitchCount  = bytesRead / sizeof(balaradStitch[0]);
			BackgroundColor         = balaradHeader.backgroundColor;
			IniFile.backgroundColor = balaradHeader.backgroundColor;
			BackgroundPen           = nuPen(BackgroundPen, 1, BackgroundColor);
			BackgroundPenWidth      = 1;
			DeleteObject(BackgroundBrush);
			BackgroundBrush        = CreateSolidBrush(BackgroundColor);
			constexpr auto IBALRAT = 0.6f;
			IniFile.hoopSizeX      = balaradHeader.hoopSizeX * IBALRAT;
			IniFile.hoopSizeY      = balaradHeader.hoopSizeY * IBALRAT;
			UnzoomedRect           = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
			BalaradOffset.x        = IniFile.hoopSizeX / 2.0;
			BalaradOffset.y        = IniFile.hoopSizeY / 2.0;
			PCSHeader.hoopType     = CUSTHUP;
			IniFile.hoopType       = CUSTHUP;
			UserColor[0]           = balaradHeader.color[0];
			auto color             = 0u;
			auto bColor            = 1u;
			auto iBalaradStitch    = 0u;
			ColorChanges           = 1u;
			for (auto iStitch = 0u; iStitch < stitchCount; iStitch++) {
				switch (balaradStitch[iStitch].code) {
				case BALNORM: {
					bal2thr(balaradStitch, iBalaradStitch++, iStitch, color);
					break;
				}
				case BALSTOP: {
					color = colmatch(balaradHeader.color[bColor++]);
					break;
				}
				}
			}
			for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
				UserPen[iColor]        = CreatePenInt(PS_SOLID, 1, UserColor[iColor]);
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
			}
			PCSHeader.stitchCount = gsl::narrow<unsigned short>(iBalaradStitch);
			thred::coltab();
			thred::redraw(ColorBar);
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
	CloseHandle(balaradFile);
}

void thred::internal::ritbal() {
	auto           balaradHeader = BALHED{};
	auto           balaradFile   = HANDLE{};
	auto           outputName    = fs::path{};
	constexpr auto BalaradRatio  = 10.0f / 6.0f;

	if (!BalaradName0->empty() && !BalaradName1->empty() && PCSHeader.stitchCount) {
		if (WorkingFileName->empty()) {
			outputName = *DefaultDirectory / L"balfil.thr";
		}
		else {
			outputName = *WorkingFileName;
		}
		outputName.replace_extension(L".thv");
		balaradFile = CreateFile(outputName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		if (balaradFile == INVALID_HANDLE_VALUE) {
			return;
		}
		auto color             = StitchBuffer[0].attribute & COLMSK;
		balaradHeader.color[0] = UserColor[color];
		auto iColor            = 1u;
		for (auto iStitch = 1u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (color != (StitchBuffer[iStitch].attribute & COLMSK)) {
				color                         = StitchBuffer[iStitch].attribute & COLMSK;
				balaradHeader.color[iColor++] = UserColor[color];
				if (iColor & 0xffffff00) {
					break;
				}
			}
		}
		balaradHeader.signature       = 'drbm';
		balaradHeader.backgroundColor = BackgroundColor;
		balaradHeader.hoopSizeX       = IniFile.hoopSizeX * BalaradRatio;
		balaradHeader.hoopSizeY       = IniFile.hoopSizeY * BalaradRatio;
		auto bytesWritten             = DWORD{ 0 };
		WriteFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesWritten, nullptr);
		BalaradOffset.x    = IniFile.hoopSizeX / 2.0f;
		BalaradOffset.y    = IniFile.hoopSizeY / 2.0f;
		auto balaradStitch = std::vector<BALSTCH>(gsl::narrow<size_t>(PCSHeader.stitchCount) + 2u);
		color              = StitchBuffer[0].attribute & COLMSK;
		// ToDo - does this loop make sense? iOutput is > 2 after one iteration
		auto iOutput = 0u;
		thr2bal(balaradStitch, iOutput++, 0, BALJUMP);
		balaradStitch[iOutput].flag = gsl::narrow<unsigned char>(color);
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount && iOutput < 2; iStitch++) {
			thr2bal(balaradStitch, iOutput++, iStitch, BALNORM);
			if ((StitchBuffer[iStitch].attribute & COLMSK) != color) {
				thr2bal(balaradStitch, iOutput, iStitch, BALSTOP);
				color                         = StitchBuffer[iStitch].attribute & COLMSK;
				balaradStitch[iOutput++].flag = gsl::narrow<unsigned char>(color);
			}
		}
		WriteFile(balaradFile, balaradStitch.data(), iOutput * sizeof(balaradStitch[0]), &bytesWritten, nullptr);
		CloseHandle(balaradFile);
		balaradFile = CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		WriteFileInt(balaradFile,
		             outputName.wstring().c_str(),
		             gsl::narrow<unsigned int>(outputName.wstring().size() + 1),
		             &bytesWritten,
		             nullptr);
		CloseHandle(balaradFile);
	}
	else {
		if (!BalaradName1->empty()) {
			balaradFile = CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
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
		return true;
	}

	StateMap.reset(StateFlag::NOTFREE);
	const auto writePerms = fs::status(filename).permissions() & writeBits;
	if (writePerms == fs::perms::none) {
		auto buttonPressed = MessageBox(ThrEdWindow,
		                                fmt::format((*StringTable)[STR_OVRLOK], filename.wstring()).c_str(),
		                                (*StringTable)[STR_OVRIT].c_str(),
		                                MB_YESNO);
		if (buttonPressed == IDYES) {
			fs::permissions(filename, fs::perms::add_perms | writeBits);
		}
		else {
			return true;
		}
	}

	return false;
}

void thred::internal::duver(const fs::path& name) {
	auto version = tolower(name.extension().wstring().back()) - 'r';
	if (version >= 0 && version <= (OLDVER - 1)) {
		VersionNames[version] = name;
	}
}

void thred::internal::durit(char** destination, const void* const source, unsigned int count) noexcept {
	if (destination && source) {
		GSL_SUPPRESS(26474) memcpy(static_cast<void*>(*destination), source, count);
		*destination += count;
	}
}

void thred::internal::dubuf(char* const buffer, unsigned& count) {
	auto stitchHeader = STRHED{};
	auto vertexCount  = 0u;
	auto guideCount = 0u, clipDataCount = 0u;
	auto output = buffer;

	stitchHeader.headerType  = 0x2746872;
	stitchHeader.fileLength  = PCSHeader.stitchCount * sizeof(StitchBuffer[0]) + sizeof(stitchHeader) + sizeof(PCSBMPFileName);
	stitchHeader.stitchCount = PCSHeader.stitchCount;
	stitchHeader.hoopType    = IniFile.hoopType;
	auto designer            = utf::Utf16ToUtf8(*DesignerName);
	std::copy(designer.begin(), designer.end(), ExtendedHeader.modifierName);
	if (!(*FormList).empty()) {
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
	stitchHeader.formCount     = gsl::narrow<unsigned short>((*FormList).size());
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
	stitchHeader.clipDataLen = gsl::narrow<unsigned short>(sizeof((*ClipPoints)[0]) * clipDataCount);
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
	auto threadSizeBufW     = std::wstring(MsgBuffer);
	auto threadSizeBuf      = utf::Utf16ToUtf8(threadSizeBufW);
	durit(&output, threadSizeBuf.c_str(), gsl::narrow<unsigned int>(threadSizeBuf.size() * sizeof(threadSizeBuf[0])));
	if (!(*FormList).empty()) {
		auto outForms = std::vector<FRMHEDOUT>{};
		outForms.reserve((*FormList).size());
		auto vertices = std::vector<fPOINT>{};
		vertices.reserve(vertexCount);
		auto guides = std::vector<SATCONOUT>{};
		guides.reserve(guideCount);
		auto points = std::vector<fPOINT>{};
		points.reserve(clipDataCount);
		for (auto& srcForm : (*FormList)) {
			outForms.emplace_back(FRMHEDOUT{ srcForm });
			for (auto iVertex = 0u; iVertex < srcForm.vertexCount; iVertex++) {
				vertices.push_back(srcForm.vertices[iVertex]);
			}
			if (srcForm.type == SAT) {
				outForms.back().satinGuideCount = gsl::narrow<unsigned short>(srcForm.satinGuideCount);
				for (auto iGuide = 0u; iGuide < srcForm.satinGuideCount; iGuide++) {
					guides.emplace_back(SATCONOUT{ srcForm.satinOrAngle.guide[iGuide] });
				}
			}
			if (clip::isclp(srcForm)) {
				auto offsetStart = ClipPoints->begin() + srcForm.angleOrClipData.clip;
				for (auto iClip = 0u; iClip < srcForm.lengthOrCount.clipCount; iClip++) {
					points.push_back(*offsetStart);
					offsetStart++;
				}
			}
			if (clip::iseclpx(srcForm)) {
				auto       offsetStart = ClipPoints->begin() + srcForm.borderClipData;
				const auto clipCount   = srcForm.clipEntries;
				for (auto iClip = 0u; iClip < clipCount; iClip++) {
					points.push_back(*offsetStart);
					offsetStart++;
				}
			}
		}
		if (!outForms.empty()) {
			durit(&output, outForms.data(), gsl::narrow<unsigned int>(outForms.size() * sizeof(outForms[0])));
		}
		if (!vertices.empty()) {
			durit(&output, vertices.data(), gsl::narrow<unsigned int>(vertices.size() * sizeof(vertices[0])));
		}
		if (!guides.empty()) {
			durit(&output, guides.data(), gsl::narrow<unsigned int>(guides.size() * sizeof(guides[0])));
		}
		if (!points.empty()) {
			durit(&output, points.data(), gsl::narrow<unsigned int>(points.size() * sizeof(points[0])));
		}
		if (TextureIndex) {
			durit(&output,
			      TexturePointsBuffer->data(),
			      gsl::narrow<unsigned int>(TextureIndex * sizeof((*TexturePointsBuffer)[0])));
		}
	}
	// ToDo - find a better way than pointer arithmetic
	count = output - buffer;
}

void thred::internal::thrsav() {
	if (chkattr(*WorkingFileName)) {
		return;
	}
	if (!StateMap.testAndReset(StateFlag::IGNAM)) {
		auto fileData = WIN32_FIND_DATA{};
		auto file     = FindFirstFile(GeName->wstring().c_str(), &fileData);
		if (file != INVALID_HANDLE_VALUE) {
			StateMap.reset(StateFlag::CMPDO);
			for (auto& version : VersionNames) {
				version.clear();
			}
			duver(*DefaultDirectory / fileData.cFileName);
			while (FindNextFile(file, &fileData)) {
				duver(*DefaultDirectory / fileData.cFileName);
			}
			FindClose(file);
			fs::remove(VersionNames[OLDVER - 1]);
			for (auto iBackup = OLDVER - 2; iBackup >= 0; iBackup--) {
				if (!VersionNames[iBackup].empty()) {
					auto newFileName = VersionNames[iBackup];
					auto ext         = newFileName.extension().wstring();
					ext.back()       = iBackup + 's';
					newFileName.replace_extension(ext);
					fs::rename(VersionNames[iBackup], newFileName);
				}
			}
		}
	}
	FileHandle = CreateFile(ThrName->wstring().c_str(), (GENERIC_WRITE), 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (FileHandle == INVALID_HANDLE_VALUE) {
		displayText::crmsg(*ThrName);
		FileHandle = nullptr;
	}
	else {
		// ToDo - MAXITEMS * 8 is not the best option here. Need something better
		auto output = std::vector<char>(MAXITEMS * 8);
		auto count  = 0u;
		dubuf(output.data(), count);
		auto bytesWritten = DWORD{ 0 };
		WriteFile(FileHandle, output.data(), count, &bytesWritten, nullptr);
		if (bytesWritten != count) {
			auto fmtStr = std::wstring{};
			displayText::loadString(fmtStr, IDS_FWERR);
			displayText::shoMsg(fmt::format(fmtStr, ThrName->wstring()));
		}
		CloseHandle(FileHandle);
	}
}

void thred::internal::chk1col() {
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
	for (auto iColorChange = 0u; iColorChange < ColorChanges; iColorChange++) {
		if (ColorChangeTable[iColorChange + 1].stitchIndex - ColorChangeTable[iColorChange].stitchIndex == 1) {
			auto iStitch = ColorChangeTable[iColorChange].stitchIndex;
			auto color   = 0u;
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
	} x{};

	x.data = data;

	DSTRecords.push_back(x.dstRecord);
}

constexpr unsigned thred::internal::dudbits(const POINT& dif) {
	return Xdst[dif.x + 121] | Ydst[dif.y + 121];
}

bool thred::internal::colfil() {
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
                             const std::vector<fPOINTATTR>& stitches) {
	constexpr auto DSTMax          = 121;
	auto           dstStitchBuffer = std::vector<fPOINTATTR>(PCSHeader.stitchCount);
	auto           colorData       = std::vector<unsigned>{};

	// there could be as many colors as there are stitches
	// but we only need to reserve a reasonable number
	colorData.reserve(32);

	colorData.push_back(COLVER);
	colorData.push_back(BackgroundColor);
	colorData.push_back(UserColor[stitches[0].attribute & COLMSK]);
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		dstStitchBuffer[iStitch].x         = stitches[iStitch].x * 5 / 3;
		dstStitchBuffer[iStitch].y         = stitches[iStitch].y * 5 / 3;
		dstStitchBuffer[iStitch].attribute = stitches[iStitch].attribute;
	}
	auto boundingRect = fRECTANGLE{ dstStitchBuffer[0].y, dstStitchBuffer[0].x, dstStitchBuffer[0].x, dstStitchBuffer[0].y };
	for (auto iStitch = 1u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (dstStitchBuffer[iStitch].x > boundingRect.right) {
			boundingRect.right = dstStitchBuffer[iStitch].x + 0.5;
		}
		if (dstStitchBuffer[iStitch].x < boundingRect.left) {
			boundingRect.left = dstStitchBuffer[iStitch].x - 0.5;
		}
		if (dstStitchBuffer[iStitch].y > boundingRect.top) {
			boundingRect.top = dstStitchBuffer[iStitch].y + 0.5;
		}
		if (dstStitchBuffer[iStitch].y < boundingRect.bottom) {
			boundingRect.bottom = dstStitchBuffer[iStitch].y - 0.5;
		}
	}
	auto centerCoordinate
	    = POINT{ dToL((static_cast<double>(boundingRect.right) - boundingRect.left) / 2.0 + boundingRect.left),
		         dToL((static_cast<double>(boundingRect.top) - boundingRect.bottom) / 2.0 + boundingRect.bottom) };
	DSTOffsetData.Positive.x = fToL(boundingRect.right - centerCoordinate.x + 1);
	DSTOffsetData.Positive.y = fToL(boundingRect.top - centerCoordinate.y + 1);
	DSTOffsetData.Negative.x = fToL(centerCoordinate.x - boundingRect.left - 1);
	DSTOffsetData.Negative.y = fToL(centerCoordinate.y - boundingRect.bottom - 1);
	auto color               = dstStitchBuffer[0].attribute & 0xf;
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (color != (dstStitchBuffer[iStitch].attribute & 0xf)) {
			savdst(DSTRecords, 0xc30000);
			color = dstStitchBuffer[iStitch].attribute & 0xf;
			colorData.push_back(UserColor[color]);
		}
		auto       lengths         = POINT{ dToL(static_cast<double>(dstStitchBuffer[iStitch].x) - centerCoordinate.x),
                              dToL(static_cast<double>(dstStitchBuffer[iStitch].y) - centerCoordinate.y) };
		const auto absoluteLengths = POINT{ abs(lengths.x), abs(lengths.y) };
		auto       count           = 0u;
		if (absoluteLengths.x > absoluteLengths.y) {
			count = absoluteLengths.x / DSTMax + 1;
		}
		else {
			count = absoluteLengths.y / DSTMax + 1;
		}
		const auto stepSize
		    = POINT{ gsl::narrow<LONG>(absoluteLengths.x / count + 1), gsl::narrow<LONG>(absoluteLengths.y / count + 1) };
		auto difference = POINT{};
		while (lengths.x || lengths.y) {
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
	DSTRecords.push_back({ 0, 0, gsl::narrow<unsigned char>(0xf3) });

	if (colfil()) {
		auto bytesWritten = DWORD{ 0 };
		auto colorFile    = CreateFile(ColorFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		if (colorFile != INVALID_HANDLE_VALUE) {
			WriteFileInt(colorFile,
			             &colorData[0],
			             gsl::narrow<unsigned int>(colorData.size() * sizeof(colorData[0])),
			             &bytesWritten,
			             nullptr);
		}
		CloseHandle(colorFile);
		colorFile = CreateFile(RGBFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		if (colorFile != INVALID_HANDLE_VALUE) {
			WriteFileInt(colorFile,
			             &colorData[2],
			             gsl::narrow<unsigned int>((colorData.size() - 2) * sizeof(colorData[0])),
			             &bytesWritten,
			             nullptr);
		}
		CloseHandle(colorFile);
	}
}

bool thred::internal::pcshup(std::vector<fPOINTATTR>& stitches) {
	auto boundingRect = fRECTANGLE{ stitches[0].y, stitches[0].x, stitches[0].x, stitches[0].y };

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
	const fPOINT boundingSize = { boundingRect.right - boundingRect.left, boundingRect.top - boundingRect.bottom };
	if (boundingSize.x > LHUPX || boundingSize.y > LHUPY) {
		displayText::tabmsg(IDS_PFAF2L);
		return true;
	}
	auto hoopSize = fPOINT{};
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
	auto delta = fPOINT{};
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
	if (delta.x || delta.y) {
		for (auto offsetStitch : stitches) {
			offsetStitch.x += delta.x;
			offsetStitch.y += delta.y;
		}
	}
	return false;
}

#if PESACT

unsigned thred::internal::pesmtch(COLORREF referenceColor, unsigned char colorIndex) noexcept {
	auto color           = referenceColor;
	auto translatedColor = PESColorTranslate[colorIndex];
	auto colorDistance   = 0u;

	for (auto iRGB = 0u; iRGB < 3; iRGB++) {
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

unsigned thred::internal::pesnam() noexcept {
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

void thred::internal::rpcrd(float stitchDelta) {
	auto pesDelta = gsl::narrow<int>(std::round(stitchDelta * 5 / 3));
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
	OutputIndex = 532;
	PESdata     = buffer;
	PEScolors   = convert_ptr<unsigned char*>(&PESdata[49]);
	rpcrd(StitchBuffer[0].x);
	rpcrd(-StitchBuffer[0].y);
	auto iColor = 1u;
	auto color  = StitchBuffer[0].attribute & COLMSK;
	for (auto iStitch = 0u; iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1; iStitch++) {
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
	xt::duauxnam();
	if (chkattr(*AuxName)) {
		return;
	}
	if (!PCSHeader.stitchCount) {
		return;
	}
	chk1col();
	thred::coltab();
	auto saveStitches = std::vector<fPOINTATTR>(PCSHeader.stitchCount);
	if (UserFlagMap.test(UserFlag::ROTAUX)) {
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			saveStitches[iStitch] = StitchBuffer[iStitch];
		}
	}
	else {
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			saveStitches[iStitch] = StitchBuffer[iStitch];
		}
	}
	PCSFileHandle = CreateFile(AuxName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (PCSFileHandle == INVALID_HANDLE_VALUE) {
		displayText::crmsg(*AuxName);
		PCSFileHandle = nullptr;
	}
	else {
		auto flag       = true;
		auto DSTRecords = std::vector<DSTREC>{};
		// There are always going to be more records in the DST format because color changes and jumps count as stitches
		DSTRecords.reserve(gsl::narrow<size_t>(PCSHeader.stitchCount) + 128);
		auto DSTOffsetData   = DSTOffsets{};
		auto PCSStitchBuffer = std::vector<PCSTCH>{};
		auto auxName         = utf::Utf16ToUtf8(*AuxName);
		auto desc            = strrchr(auxName.data(), '\\') + 1;
		switch (IniFile.auxFileType) {
		case AUXDST: {
			auto dstHeader = DSTHED{};
			ritdst(DSTOffsetData, DSTRecords, saveStitches);
			// dstHeader fields are fixed width, so use strncpy in its intended way.
			// Use sizeof to ensure no overrun if the format string is wrong length
			strncpy(dstHeader.desched, "LA:", sizeof(dstHeader.desched));
			std::fill_n(dstHeader.desc, sizeof(dstHeader.desc), ' ');
			if (desc) {
				for (auto iHeader = 0u; iHeader < sizeof(dstHeader.desc); iHeader++) {
					if (desc[iHeader] && desc[iHeader] != '.') {
						dstHeader.desc[iHeader] = desc[iHeader];
					}
					else {
						break;
					}
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
			auto bytesWritten = DWORD{ 0 };
			WriteFile(PCSFileHandle, &dstHeader, sizeof(dstHeader), &bytesWritten, nullptr);
			WriteFileInt(PCSFileHandle,
			             DSTRecords.data(),
			             gsl::narrow<unsigned int>(sizeof(DSTRecords[0]) * DSTRecords.size()),
			             &bytesWritten,
			             nullptr);
			break;
		}
#if PESACT
		case AUXPES: {
			auto pesHeader = PESHED{};
			auto pchr      = convert_ptr<unsigned char*>(&pesHeader);
			strncpy(pesHeader.led, "#PES0001", sizeof(pesHeader.led));
			strncpy(pesHeader.ce, "CEmbOne", sizeof(pesHeader.ce));
			strncpy(pesHeader.cs, "CSewSeg", sizeof(pesHeader.cs));
			auto matchIndex = 0u;
			for (auto iColor = 0u; iColor < 16; iColor++) {
				auto matchMin = 0xffffffffu;
				for (auto iColorMatch = 0u; iColorMatch < (sizeof(PESColorTranslate) >> 2); iColorMatch++) {
					auto match = pesmtch(UserColor[iColor], iColorMatch);
					if (match < matchMin) {
						matchIndex = iColorMatch;
						matchMin   = match;
					}
				}
				PESequivColors[iColor] = gsl::narrow<unsigned char>(matchIndex);
			}
			auto color        = StitchBuffer[0].attribute & COLMSK;
			auto boundingRect = fRECTANGLE{};
			sizstch(boundingRect, StitchBuffer);
			PESstitchCenterOffset.x = form::midl(boundingRect.right, boundingRect.left);
			PESstitchCenterOffset.y = form::midl(boundingRect.top, boundingRect.bottom);
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
			auto pesColorCount = 0u;
			for (auto iStitch = 1u; iStitch < PCSHeader.stitchCount; iStitch++) {
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
			PESstitches[OutputIndex].x   = (sizeof(PESColorTranslate) >> 2);
			PESstitches[OutputIndex++].y = 0;
			auto pesOffset               = convert_ptr<unsigned*>(&pesHeader.off);
			*pesOffset                   = (OutputIndex << 2) + sizeof(pesHeader);
			*pesHeader.m1                = 0x20;
			GroupStartStitch             = 0;
			GroupEndStitch               = PCSHeader.stitchCount - 1;
			pesHeader.xsiz               = 10000;
			pesHeader.ysiz               = 10000;
			auto bytesWritten            = DWORD{ 0 };
			WriteFile(PCSFileHandle, convert_ptr<PESHED*>(&pesHeader), sizeof(pesHeader), &bytesWritten, nullptr);
			WriteFile(PCSFileHandle, PESstitches, OutputIndex * sizeof(PESstitches[0]), &bytesWritten, nullptr);
			delete[] PESstitches;
			// ToDo - (PES) is there a better estimate for data size?
			pchr = new unsigned char[MAXITEMS * 4];
			// ToDo - (PES) Add buffer parameter and remove use of BSequence in pesname
			auto iHeader = pesnam();
			while (iHeader < 512)
				pchr[iHeader++] = ' ';
			pchr[19] = 13;
			pchr[48] = gsl::narrow<unsigned char>(pesColorCount);
			pecdat(pchr);
			auto upnt = convert_ptr<unsigned*>(&pchr[514]);
			*upnt     = OutputIndex - 512;
			pchr[517] = 0x20;
			pchr[518] = 0xff;
			pchr[519] = 0xef;
			auto psiz = convert_ptr<short*>(&pchr[520]);
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
			WriteFile(PCSFileHandle, pchr, OutputIndex, &bytesWritten, nullptr);
			delete[] pchr;
			break;
		}
#endif
		default: {
			for (auto iColor = 0u; iColor < 16; iColor++) {
				PCSHeader.colors[iColor] = UserColor[iColor];
			}
			do {
				if (pcshup(saveStitches)) {
					flag = false;
					break;
				}
				auto bytesWritten = DWORD{ 0 };
				if (!WriteFile(PCSFileHandle, &PCSHeader, 0x46, &bytesWritten, nullptr)) {
					displayText::riter();
					flag = false;
					break;
				}
				auto iPCSstitch = 0u;
				auto savcol     = 0xffu;
				PCSStitchBuffer.resize(gsl::narrow<size_t>(PCSHeader.stitchCount) + ColorChanges + 2);
				for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
					if ((saveStitches[iStitch].attribute & COLMSK) != savcol) {
						savcol                           = saveStitches[iStitch].attribute & COLMSK;
						PCSStitchBuffer[iPCSstitch].tag  = 3;
						PCSStitchBuffer[iPCSstitch++].fx = gsl::narrow<unsigned char>(savcol);
					}
					auto integerPart                = 0.0;
					auto fractionalPart             = std::modf(saveStitches[iStitch].x, &integerPart);
					PCSStitchBuffer[iPCSstitch].fx  = gsl::narrow<unsigned char>(std::floor(fractionalPart * 256.0));
					PCSStitchBuffer[iPCSstitch].x   = gsl::narrow<short>(integerPart);
					fractionalPart                  = std::modf(saveStitches[iStitch].y, &integerPart);
					PCSStitchBuffer[iPCSstitch].fy  = gsl::narrow<unsigned char>(std::floor(fractionalPart * 256.0));
					PCSStitchBuffer[iPCSstitch++].y = gsl::narrow<short>(integerPart);
				}
				if (!WriteFile(
				        PCSFileHandle, PCSStitchBuffer.data(), iPCSstitch * sizeof(PCSStitchBuffer[0]), &bytesWritten, nullptr)) {
					displayText::riter();
					flag = false;
					break;
				}
				if (UserFlagMap.test(UserFlag::BSAVOF)) {
					*MsgBuffer = 0;
					if (!WriteFile(PCSFileHandle, MsgBuffer, 15, &bytesWritten, nullptr)) {
						displayText::riter();
						flag = false;
						break;
					}
				}
				else {
					if (!WriteFile(PCSFileHandle, PCSBMPFileName, 15, &bytesWritten, nullptr)) {
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
	auto filinfo = MENUITEMINFO{ sizeof(MENUITEMINFO),
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
		                         nullptr };

	auto auxMsg = std::wstring{};

	CheckMenuItem(MainMenu, ID_AUXPCS, MF_UNCHECKED);
#if PESACT
	CheckMenuItem(MainMenu, ID_AUXPES, MF_UNCHECKED);
#else
	EnableMenuItem(MainMenu, ID_AUXPES, MF_DISABLED | MF_GRAYED);
#endif
	CheckMenuItem(MainMenu, ID_AUXDST, MF_UNCHECKED);
	switch (IniFile.auxFileType) {
	case AUXDST: {
		auxMsg = fmt::format((*StringTable)[STR_AUXTXT], L"DST");
		CheckMenuItem(MainMenu, ID_AUXDST, MF_CHECKED);
		break;
	}
	case AUXPES:
#if PESACT
	{
		auxMsg = fmt::format((*StringTable)[STR_AUXTXT], L"PES");
		CheckMenuItem(MainMenu, ID_AUXPES, MF_CHECKED);
		break;
	}
#else
	{
		IniFile.auxFileType = AUXPCS;
	}
#endif
	default: {
		auxMsg = fmt::format((*StringTable)[STR_AUXTXT], L"PCS");
		CheckMenuItem(MainMenu, ID_AUXPCS, MF_CHECKED);
	}
	}
	GSL_SUPPRESS(26465) {
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
		if (PCSHeader.stitchCount) {
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
		if (StitchWindowClientRect.right) {
			displayText::savdisc();
			StateMap.set(StateFlag::SAVEX);
		}
		else {
			auto fmtStr = std::wstring{};
			displayText::loadString(fmtStr, IDS_SAVFIL);
			if (MessageBox(
			        ThrEdWindow, fmt::format(fmtStr, ThrName->wstring()).c_str(), (*StringTable)[STR_CLOS].c_str(), MB_YESNO)
			    == IDYES) {
				thred::save();
			}
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
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
	SideWindowLocation++;
}

void thred::internal::sidmsg(HWND window, std::wstring* const strings, unsigned entries) {
	if (strings) {
		auto childListRect  = RECT{};
		auto parentListRect = RECT{};
		auto entryCount     = entries;

		std::fill(ValueWindow->begin(), ValueWindow->end(), nullptr);
		SideWindowSize     = {};
		SideWindowLocation = 0;
		SideWindowsStrings = strings;
		GetWindowRect(window, &childListRect);
		GetWindowRect(FormDataSheet, &parentListRect);
		form::ispcdclp();
		if (StateMap.test(StateFlag::FILTYP)) {
			for (auto iEntry = 0u; iEntry < EDGETYPS + 1; iEntry++) {
				if (gsl::narrow<unsigned>(SelectedForm->edgeType & NEGUND) == EdgeFillTypes[iEntry]) {
					entryCount--;
				}
				else {
					if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT
					    || EdgeFillTypes[iEntry] == EDGECLIPX) {
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
			SideMessageWindow = CreateWindow(L"STATIC",
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
			for (auto iEntry = 0u; iEntry < entries; iEntry++) {
				if (gsl::narrow<unsigned>(SelectedForm->edgeType & NEGUND) != EdgeFillTypes[iEntry]) {
					if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT
					    || EdgeFillTypes[iEntry] == EDGECLIPX) {
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
				auto zero = std::wstring{ L"0" };
				formForms::maxtsiz(zero, SideWindowSize);
			}
			else {
				if (FormMenuChoice == LFTHTYP) {
					entryCount     = 5;
					SideWindowSize = { gsl::narrow<LONG>(ButtonWidthX3), gsl::narrow<LONG>(ButtonHeight) };
				}
				else {
					for (auto iEntry = 0u; iEntry < entries; iEntry++) {
						if ((1 << FillTypes[iEntry]) & ClipTypeMap) {
							if (StateMap.test(StateFlag::WASPCDCLP)) {
								formForms::maxtsiz(strings[iEntry], SideWindowSize);
							}
							else {
								entryCount--;
							}
						}
						else {
							if (FillTypes[iEntry] == SelectedForm->fillType) {
								entryCount--;
							}
							else {
								formForms::maxtsiz(strings[iEntry], SideWindowSize);
							}
						}
					}
				}
			}
			SideMessageWindow = CreateWindow(L"STATIC",
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
				for (auto iEntry = 0u; iEntry < entries; iEntry++) {
					dusid(iEntry);
				}
			}
			else {
				if (FormMenuChoice == LFTHTYP) {
					for (auto iEntry = 0u; iEntry < 6; iEntry++) {
						if (FeatherFillTypes[iEntry] != SelectedForm->fillInfo.feather.fillType) {
							dusid(iEntry);
						}
					}
				}
				else {
					for (auto iEntry = 0u; iEntry < entries; iEntry++) {
						if (FillTypes[iEntry] != SelectedForm->fillType) {
							if ((1 << FillTypes[iEntry]) & ClipTypeMap) {
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
	const auto center = POINT{ dToL((ZoomRect.right - ZoomRect.left) / 2.0), dToL((ZoomRect.top - ZoomRect.bottom) / 2.0) };

	SelectedPoint.x = ZoomRect.left + center.x;
	SelectedPoint.y = ZoomRect.bottom + center.y;
}

void thred::internal::stchWnd() {
	stchPars();

	MainStitchWin = nullptr;
	MainStitchWin = CreateWindow(L"STATIC",
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

		VerticalScrollBar = CreateWindow(L"SCROLLBAR",
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

		HorizontalScrollBar = CreateWindow(L"SCROLLBAR",
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
bool thred::internal::chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) noexcept {
	auto topRect    = RECT{};
	auto bottomRect = RECT{};

	GetWindowRect(topWindow, &topRect);
	GetWindowRect(bottomWindow, &bottomRect);

	if (clickCoord.x > topRect.left && clickCoord.x < bottomRect.right && clickCoord.y > topRect.top
	    && clickCoord.y < bottomRect.bottom) {
		VerticalIndex = 15 - (bottomRect.bottom - clickCoord.y) / ButtonHeight;
		return true;
	}
	return false;
}

void thred::internal::delstch1(unsigned iStitch) {
	if (PCSHeader.stitchCount) {
		while (iStitch <= PCSHeader.stitchCount) {
			StitchBuffer[iStitch] = StitchBuffer[iStitch + 1];
			iStitch++;
		}
		PCSHeader.stitchCount--;
		if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		}
	}
}

void thred::internal::redbak() {
	const auto undoData = convert_ptr<BAKHED*>((*UndoBuffer)[UndoBufferWriteIndex].get());
	if (undoData) {
		if (undoData->stitchCount) {
			std::copy(undoData->stitches, undoData->stitches + undoData->stitchCount, StitchBuffer);
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(undoData->stitchCount);
		UnzoomedRect          = undoData->zoomRect;
		if (undoData->formCount) {
			(*FormList).clear();
			for (auto iForm = 0u; iForm < undoData->formCount; iForm++) {
				const auto& form = undoData->forms[iForm];
				(*FormList).push_back(form);
			}
		}
		FormIndex = undoData->formCount;
		if (undoData->vertexCount) {
			std::copy(undoData->vertices, undoData->vertices + undoData->vertexCount, FormVertices);
		}
		FormVertexIndex = undoData->vertexCount;
		satin::cpyUndoGuides(*undoData);
		if (undoData->clipPointCount) {
			ClipPoints->resize(undoData->clipPointCount);
			std::copy(undoData->clipPoints, undoData->clipPoints + undoData->clipPointCount, ClipPoints->begin());
		}
		auto sizeColors = (sizeof(UserColor) / sizeof(UserColor[0]));
		std::copy(undoData->colors, undoData->colors + sizeColors, UserColor);
		for (auto iColor = 0u; iColor < sizeColors; iColor++) {
			UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
			UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
		}
		for (auto iColor = 0u; iColor < sizeColors; iColor++) {
			thred::redraw(UserColorWin[iColor]);
		}
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
	if (StateMap.test(StateFlag::BAKACT) && StateMap.test(StateFlag::REDUSHO)) {
		UndoBufferWriteIndex++;
		UndoBufferWriteIndex &= 0xf;
		const auto nextBufferIndex = (UndoBufferWriteIndex + 1) & 0xf;
		if (nextBufferIndex == UndoBufferReadIndex) {
			EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
			StateMap.reset(StateFlag::REDUSHO);
		}
		else {
			if (!StateMap.testAndSet(StateFlag::REDUSHO)) {
				EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
			}
		}
		if (!StateMap.testAndSet(StateFlag::UNDUSHO)) {
			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
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
	thred::undat();
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
		const auto previousBufferIndex = UndoBufferWriteIndex - 1;
		if (previousBufferIndex == UndoBufferReadIndex) {
			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			StateMap.set(StateFlag::DUMEN);
			StateMap.reset(StateFlag::UNDUSHO);
		}
	}
	else {
		if (UndoBufferWriteIndex) {
			UndoBufferWriteIndex--;
		}
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

void thred::internal::bitsiz() {
	const auto screenAspectRatio = gsl::narrow<double>(UnzoomedRect.x) / UnzoomedRect.y;
	const auto bitmapAspectRatio = gsl::narrow<double>(BitmapWidth) / BitmapHeight;

	if (bitmapAspectRatio > screenAspectRatio) {
		BitmapSizeinStitches.x = gsl::narrow<double>(UnzoomedRect.x);
		BitmapSizeinStitches.y = UnzoomedRect.x / bitmapAspectRatio;
	}
	else {
		BitmapSizeinStitches.x = UnzoomedRect.y * bitmapAspectRatio;
		BitmapSizeinStitches.y = gsl::narrow<double>(UnzoomedRect.y);
	}
	BmpStitchRatio.x = BitmapWidth / BitmapSizeinStitches.x;
	BmpStitchRatio.y = BitmapHeight / BitmapSizeinStitches.y;
	StitchBmpRatio.x = BitmapSizeinStitches.x / BitmapWidth;
	StitchBmpRatio.y = BitmapSizeinStitches.y / BitmapHeight;
}

// Get a rough estimate of whether black or white
// is dominant in the monochrome bitmap
bool thred::internal::binv(const std::vector<unsigned char>& monoBitmapData, unsigned bitmapWidthInBytes) {
	auto       whiteBits = 0u;
	auto       blackBits = 0u;
	const auto byteCount = BitmapWidth >> 3;

	for (auto iHeight = 0u; iHeight < BitmapHeight; iHeight++) {
		GSL_SUPPRESS(26429) {
			if ((gsl::narrow<size_t>(bitmapWidthInBytes) * iHeight) < monoBitmapData.size()) {
				const auto bcpnt = &monoBitmapData[gsl::narrow<size_t>(bitmapWidthInBytes) * iHeight];
				for (auto iBytes = 0u; iBytes < byteCount; iBytes++) {
					if (!bcpnt[iBytes]) {
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

void thred::internal::bitlin(const unsigned char* source, unsigned int* destination, COLORREF foreground, COLORREF background) {
	if (source && destination) {
		for (auto i = 0u; i < (BitmapWidth >> 3); i++) {
			auto bits = std::bitset<8>(source[i]);
			for (auto bitOffset = 0u; bitOffset < 8; bitOffset++) {
				*destination = bits[bitOffset ^ 7] ? foreground : background;
				destination++;
			}
		}
		if (const auto final = (BitmapWidth % 8)) {
			auto bits = std::bitset<8>(source[(BitmapWidth >> 3)]);
			for (auto bitOffset = final; bitOffset < 8; bitOffset++) {
				*destination = bits[bitOffset ^ 7] ? foreground : background;
				destination++;
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
		return true;
	case 24:
		return true;
	case 32:
		return true;
	}
	return false;
}

// Move unpacked 24BPP data into packed 24BPP data
void thred::internal::movmap(unsigned cnt, unsigned char* buffer) {
	auto source = TraceBitmapData;
	if (source) {
		auto destination = buffer;

		for (auto i = 0u; i < cnt; i++) {
			*(convert_ptr<unsigned*>(destination)) = *(source++);
			destination += 3;
		}
	}
}

void thred::internal::savmap() {
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
			BitmapFileHandle
			    = CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
			if (IniFileHandle == INVALID_HANDLE_VALUE) {
				displayText::crmsg(*UserBMPFileName);
				return;
			}
			auto bytesWritten = DWORD{ 0 };
			WriteFile(BitmapFileHandle, &BitmapFileHeader, 14, &bytesWritten, nullptr);
			WriteFile(BitmapFileHandle, &BitmapFileHeaderV4, BitmapFileHeader.bfOffBits - 14, &bytesWritten, nullptr);
			auto buffer = std::vector<unsigned char>((gsl::narrow<size_t>(BitmapWidth) * BitmapHeight * 3) + 1);
			movmap(BitmapWidth * BitmapHeight, buffer.data());
			WriteFile(BitmapFileHandle, buffer.data(), BitmapWidth * BitmapHeight * 3, &bytesWritten, nullptr);
			CloseHandle(BitmapFileHandle);
		}
	}
	else {
		displayText::tabmsg(IDS_SHOMAP);
	}
}

HBITMAP thred::getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ unsigned int** ppvBits) {
	GSL_SUPPRESS(26490) {
		if (ppvBits) {
			auto bitmap = CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, reinterpret_cast<void**>(ppvBits), nullptr, 0);
			if (*ppvBits != nullptr) {
				return bitmap;
			}

			DeleteObject(bitmap);
			throw;
		}

		throw;
	}
}

void thred::internal::bfil() {
	const auto InverseBackgroundColor = fswap(BackgroundColor);
	BitmapFileHandle = CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (BitmapFileHandle == INVALID_HANDLE_VALUE) {
		auto fmtStr = std::wstring{};
		displayText::loadString(fmtStr, IDS_UNOPEN);
		displayText::shoMsg(fmt::format(fmtStr, UserBMPFileName->wstring()));
		CloseHandle(BitmapFileHandle);
		BitmapFileHandle  = nullptr;
		PCSBMPFileName[0] = 0;
		return;
	}
	ReadFile(BitmapFileHandle, &BitmapFileHeader, 14, &BytesRead, nullptr);
	if (BitmapFileHeader.bfType == 'MB') {
		auto fileHeaderSize = BitmapFileHeader.bfOffBits - 14;
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
		ZoomRect.left = ZoomRect.bottom = 0;
		ZoomRect.right                  = UnzoomedRect.x;
		ZoomRect.top                    = UnzoomedRect.y;
		BitmapDC                        = CreateCompatibleDC(StitchWindowDC);
		if (BitmapFileHeaderV4.bV4BitCount == 1) {
			StateMap.set(StateFlag::MONOMAP);
			auto       bitmapWidthBytes = (BitmapWidth >> 5) << 2;
			const auto widthOverflow    = BitmapWidth % 32;
			if (widthOverflow) {
				bitmapWidthBytes += 4;
			}
			const auto bitmapSizeBytes = bitmapWidthBytes * BitmapHeight;
			auto       monoBitmapData  = std::vector<unsigned char>(bitmapSizeBytes);
			ReadFile(BitmapFileHandle, monoBitmapData.data(), bitmapSizeBytes, &BytesRead, nullptr);
			CloseHandle(BitmapFileHandle);
			auto foreground = COLORREF{};
			auto background = COLORREF{};
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
			HBITMAP bitmap                 = {};

			GSL_SUPPRESS(26413) {
				auto* bits = gsl::narrow_cast<unsigned int*>(nullptr);
				bitmap     = thred::getBitmap(BitmapDC, &BitmapInfo, &bits);
				// Synchronize
				GdiFlush();
				if (bits) {
					for (auto iHeight = 0u; iHeight < BitmapHeight; iHeight++) {
						bitlin(&monoBitmapData[gsl::narrow<size_t>(iHeight) * bitmapWidthBytes],
						       &bits[gsl::narrow<size_t>(iHeight) * BitmapWidth],
						       background,
						       foreground);
					}
				}
			};
			auto deviceContext = CreateCompatibleDC(StitchWindowDC);
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
		BitmapFileHandle  = nullptr;
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
	auto bytesRead = DWORD{ 0 };
	auto colors    = std::vector<unsigned>{};

	if (colfil()) {
		auto colorFile = CreateFile(ColorFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
		if (colorFile != INVALID_HANDLE_VALUE) {
			auto colorFileSize = LARGE_INTEGER{};
			GetFileSizeEx(colorFile, &colorFileSize);
			// There can only be (64K + 3) colors, so even if HighPart is non-zero, we don't care
			colors.resize(colorFileSize.u.LowPart / sizeof(colors[0]));
			ReadFile(colorFile, colors.data(), colorFileSize.u.LowPart, &bytesRead, nullptr);
			CloseHandle(colorFile);
			if (bytesRead > (sizeof(colors[0]) * 2)) {
				if (colors[0] == COLVER) {
					BackgroundColor = colors[1];
					ColorChanges    = 0;
				}
			}
		}
	}

	auto iColor = 2u;
	auto color  = 0u;
	if (bytesRead >= ((gsl::narrow<size_t>(iColor) + 1) * sizeof(colors[0]))) {
		color = colmatch(colors[iColor++]);
	}
	else {
		color = 0;
	}
	auto localStitch       = fPOINT{};
	auto maximumCoordinate = fPOINT{ -1e12f, -1e12f };
	auto mimimumCoordinate = fPOINT{ 1e12f, 1e12f };
	auto iStitch           = 0u;
	for (auto& record : DSTData) {
		if (record.nd & 0x40) {
			if (bytesRead >= ((gsl::narrow<size_t>(iColor) + 1) * sizeof(colors[0]))) {
				color = colmatch(colors[iColor++]);
			}
			else {
				color++;
				color &= 0xf;
			}
		}
		else {
			auto dstStitch = POINT{};
			dstin(thi::dtrn(&record), dstStitch);
			localStitch.x += dstStitch.x;
			localStitch.y += dstStitch.y;
			if (!(record.nd & 0x80)) {
				StitchBuffer[iStitch] = { localStitch.x * 0.6f, localStitch.y * 0.6f, color | NOTFRM };
				if (StitchBuffer[iStitch].x > maximumCoordinate.x) {
					maximumCoordinate.x = StitchBuffer[iStitch].x;
				}
				if (StitchBuffer[iStitch].y > maximumCoordinate.y) {
					maximumCoordinate.y = StitchBuffer[iStitch].y;
				}
				if (StitchBuffer[iStitch].x < mimimumCoordinate.x) {
					mimimumCoordinate.x = StitchBuffer[iStitch].x;
				}
				if (StitchBuffer[iStitch].y < mimimumCoordinate.y) {
					mimimumCoordinate.y = StitchBuffer[iStitch].y;
				}
				iStitch++;
			}
		}
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(iStitch);
	const auto dstSize    = fPOINT{ maximumCoordinate.x - mimimumCoordinate.x, maximumCoordinate.y - mimimumCoordinate.y };
	IniFile.hoopType      = CUSTHUP;
	UnzoomedRect          = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
	if (dstSize.x > UnzoomedRect.x || dstSize.y > UnzoomedRect.y) {
		IniFile.hoopSizeX = dstSize.x * 1.1f;
		IniFile.hoopSizeY = dstSize.y * 1.1f;
		UnzoomedRect      = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
		displayText::hsizmsg();
	}
	const auto delta = fPOINT{ (UnzoomedRect.x - dstSize.x) / 2 - mimimumCoordinate.x,
		                       (UnzoomedRect.y - dstSize.y) / 2 - mimimumCoordinate.y };
	for (iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].x += delta.x;
		StitchBuffer[iStitch].y += delta.y;
	}
}

bool thred::internal::chkdst(const DSTHED* dstHeader) noexcept {
	return strncmp(dstHeader->desched, "LA:", 3) == 0;
}

#if PESACT

unsigned thred::internal::tripl(char* dat) {
	return (*convert_ptr<unsigned*>(dat)) & 0xffffff;
}

unsigned thred::internal::dupcol() noexcept {
	const auto color = PESColorTranslate[PEScolors[PEScolorIndex++]];
	for (auto iColor = 0u; iColor < ActivePointIndex; iColor++) {
		if (UserColor[iColor] == color)
			return iColor;
	}
	auto minimumDistance = 0xffffffu;
	auto matchIndex      = 0u;
	for (auto iColor = 1u; iColor < ActivePointIndex; iColor++) {
		const auto matchDistance = pesmtch(color, PEScolors[iColor]);
		if (matchDistance < minimumDistance) {
			minimumDistance = matchDistance;
			matchIndex      = iColor;
		}
	}
	return matchIndex;
}

double thred::internal::dubl(unsigned char* pnt) noexcept {
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
	if (StateMap.testAndReset(StateFlag::THUMSHO)) {
		for (auto& iBackup : BackupViewer) {
			DestroyWindow(iBackup);
		}
		if (StateMap.test(StateFlag::UPTO)) {
			displayText::butxt(HUPTO, (*StringTable)[STR_UPON]);
		}
		else {
			displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
		}
		const auto blank = std::wstring{};
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
		if (!(*FormList).empty()) {
			form::delfrms();
		}
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
		FileHandle = CreateFile(WorkingFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
		if (FileHandle == INVALID_HANDLE_VALUE) {
			if (GetLastError() == 32) {
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
			if (SearchLineIndex) {
				SearchLineIndex = 0;
			}
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
			auto       fileSizeHigh   = DWORD{};
			auto       fileSize       = GetFileSize(FileHandle, &fileSizeHigh);
			auto       fileExt        = WorkingFileName->extension().wstring();
			const auto firstCharacter = tolower(fileExt[1]);
			if (firstCharacter == 't') {
				auto thredHeader = STRHED{};
				ReadFile(FileHandle, &thredHeader, sizeof(thredHeader), &BytesRead, nullptr);
				if ((thredHeader.headerType & 0xffffff) == 0x746872) {
					if (BytesRead != sizeof(thredHeader)) {
						displayText::tabmsg(IDS_SHRTF);
						return;
					}
					const auto version = (thredHeader.headerType & 0xff000000) >> 24;
					DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
					switch (version) {
					case 0: {
						if (PCSHeader.hoopType == SMALHUP) {
							IniFile.hoopSizeX = SHUPX;
							IniFile.hoopSizeY = SHUPY;
							UnzoomedRect      = { static_cast<LONG>(SHUPX), static_cast<LONG>(SHUPY) };
						}
						else {
							IniFile.hoopSizeX  = LHUPX;
							IniFile.hoopSizeY  = LHUPY;
							UnzoomedRect       = { static_cast<LONG>(LHUPX), static_cast<LONG>(LHUPY) };
							PCSHeader.hoopType = LARGHUP;
						}
						ritfnam(*DesignerName);
						std::copy(IniFile.designerName,
						          IniFile.designerName + strlen(IniFile.designerName),
						          ExtendedHeader.modifierName);
						break;
					}
					case 1:
					case 2: {
						ReadFile(FileHandle, &ExtendedHeader, sizeof(ExtendedHeader), &BytesRead, nullptr);
						if (BytesRead != sizeof(ExtendedHeader)) {
							displayText::tabmsg(IDS_SHRTF);
							return;
						}
						IniFile.hoopSizeX = ExtendedHeader.hoopSizeX;
						IniFile.hoopSizeY = ExtendedHeader.hoopSizeY;
						UnzoomedRect      = { fToL(ExtendedHeader.hoopSizeX), fToL(ExtendedHeader.hoopSizeY) };
						redfnam(*DesignerName);
						break;
					}
					default: {
						displayText::tabmsg(IDS_NOTVER);
						return;
					}
					}
					ZoomRect.bottom       = 0;
					ZoomRect.left         = 0;
					ZoomRect.right        = IniFile.hoopSizeX;
					ZoomRect.top          = IniFile.hoopSizeY;
					UnzoomedRect          = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
					PCSHeader.stitchCount = thredHeader.stitchCount;
					auto stitchesRead     = DWORD{ 0 };
					ReadFile(FileHandle, StitchBuffer, PCSHeader.stitchCount * sizeof(StitchBuffer[0]), &stitchesRead, nullptr);
					if (stitchesRead != PCSHeader.stitchCount * sizeof(StitchBuffer[0])) {
						PCSHeader.stitchCount = gsl::narrow<unsigned short>(stitchesRead / sizeof(StitchBuffer[0]));
						prtred();
						return;
					}
					ReadFile(FileHandle, PCSBMPFileName, sizeof(PCSBMPFileName), &BytesRead, nullptr);
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
					ReadFile(FileHandle, UserColor, sizeof(UserColor), &BytesRead, nullptr);
					if (BytesRead != sizeof(UserColor)) {
						prtred();
						return;
					}
					ReadFile(FileHandle, CustomColor, sizeof(CustomColor), &BytesRead, nullptr);
					if (BytesRead != sizeof(CustomColor)) {
						prtred();
						return;
					}
					constexpr auto threadLength = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0]))
					                              / 2; // ThreadSize is defined as a 16 entry array of 2 characters
					char msgBuffer[threadLength];
					ReadFile(FileHandle, msgBuffer, threadLength, &BytesRead, nullptr);
					if (BytesRead != threadLength) {
						prtred();
						return;
					}
					auto threadSizebuf  = std::string(msgBuffer, sizeof(msgBuffer));
					auto threadSizeBufW = utf::Utf8ToUtf16(threadSizebuf);
					for (auto iThread = 0; iThread < threadLength; iThread++) {
						ThreadSize[iThread][0] = threadSizeBufW[iThread];
					}
					FormIndex = thredHeader.formCount;
					if (thredHeader.formCount > MAXFORMS) {
						FormIndex             = MAXFORMS;
						thredHeader.formCount = MAXFORMS;
					}
					if (thredHeader.formCount != 0) {
						StateMap.reset(StateFlag::BADFIL);
						ClipPoints->clear();
						FormVertexIndex = 0;
						satin::clearGuideSize();
						MsgBuffer[0]     = 0;
						auto bytesToRead = DWORD{ 0 };
						if (version < 2) {
							auto formListOriginal = std::vector<FRMHEDO>(FormIndex);
							bytesToRead           = gsl::narrow<DWORD>(FormIndex * sizeof(formListOriginal[0]));
							ReadFileInt(FileHandle, formListOriginal.data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != FormIndex * sizeof(formListOriginal[0])) {
								FormIndex = BytesRead / sizeof(formListOriginal[0]);
								StateMap.set(StateFlag::BADFIL);
							}
							for (auto form : formListOriginal) {
								(*FormList).emplace_back(FRMHED{ form });
							}
						}
						else {
							auto inFormList = std::vector<FRMHEDOUT>(FormIndex);
							bytesToRead     = gsl::narrow<DWORD>(FormIndex * sizeof(inFormList[0]));
							ReadFileInt(FileHandle, inFormList.data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								FormIndex = BytesRead / sizeof(inFormList[0]);
								StateMap.set(StateFlag::BADFIL);
							}
							for (auto form : inFormList) {
								(*FormList).emplace_back(FRMHED{ form });
							}
						}
						if (thredHeader.vertexCount) {
							bytesToRead = gsl::narrow<DWORD>(thredHeader.vertexCount * sizeof(FormVertices[0]));
							ReadFile(FileHandle, FormVertices, bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								FormVertexIndex = BytesRead / sizeof(FormVertices[0]);
								for (auto iVertex = FormVertexIndex; iVertex < thredHeader.vertexCount; iVertex++) {
									FormVertices[iVertex].x = FormVertices[iVertex].y = 0;
								}
								StateMap.set(StateFlag::BADFIL);
							}
						}
						else {
							// We have forms but no vertices - blow up the read
							prtred();
							return;
						}
						if (thredHeader.dlineCount) {
							auto inSatinGuides = std::vector<SATCONOUT>(thredHeader.dlineCount);
							bytesToRead        = gsl::narrow<DWORD>(thredHeader.dlineCount * sizeof(inSatinGuides[0]));
							ReadFile(FileHandle, inSatinGuides.data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								StateMap.set(StateFlag::BADFIL);
							}
							std::copy(inSatinGuides.cbegin(), inSatinGuides.cend(), SatinGuides);
						}
						if (thredHeader.clipDataCount) {
							ClipPoints->resize(thredHeader.clipDataCount);
							bytesToRead = gsl::narrow<DWORD>(thredHeader.clipDataCount * sizeof((*ClipPoints)[0]));
							ReadFile(FileHandle, ClipPoints->data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								ClipPoints->resize(BytesRead / sizeof((*ClipPoints)[0]));
								StateMap.set(StateFlag::BADFIL);
							}
						}
						if (ExtendedHeader.texturePointCount) {
							TexturePointsBuffer->resize(ExtendedHeader.texturePointCount);
							bytesToRead
							    = gsl::narrow<DWORD>(ExtendedHeader.texturePointCount * sizeof((*TexturePointsBuffer)[0]));
							ReadFile(FileHandle, TexturePointsBuffer->data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								TexturePointsBuffer->resize(BytesRead / sizeof((*TexturePointsBuffer)[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							TextureIndex = gsl::narrow<unsigned int>(TexturePointsBuffer->size());
						}
						else {
							TextureIndex = 0;
						}
						if (StateMap.testAndReset(StateFlag::BADFIL)) {
							displayText::bfilmsg();
						}
						// now re-create all the pointers in the form data
						satin::clearGuideSize();
						FormVertexIndex = 0;
						for (auto iForm = 0u; iForm < FormIndex; iForm++) {
							auto& formIter    = (*FormList)[iForm];
							formIter.vertices = thred::adflt(formIter.vertexCount);
							if (formIter.type == SAT) {
								if (formIter.satinGuideCount) {
									formIter.satinOrAngle.guide = satin::adsatk(formIter.satinGuideCount);
								}
							}
							if (clip::isclp(iForm)) {
								formIter.angleOrClipData.clip = thred::adclp(formIter.lengthOrCount.clipCount);
							}
							if (clip::iseclpx(iForm)) {
								formIter.borderClipData = thred::adclp(formIter.clipEntries);
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
						if (!fileSize) {
							displayText::filnopn(IDS_ZEROL, *WorkingFileName);
							return;
						}
						if (PCSHeader.leadIn == '2' && PCSHeader.colorCount == 16) {
							for (auto iColor = 0u; iColor < 16; iColor++) {
								UserColor[iColor] = PCSHeader.colors[iColor];
							}
							fileSize -= sizeof(PCSHeader);
							const auto pcsStitchCount = fileSize / sizeof(PCSTCH) + 2;
							auto       PCSDataBuffer  = std::vector<PCSTCH>(pcsStitchCount);
							ReadFile(FileHandle, PCSDataBuffer.data(), fileSize, &BytesRead, nullptr);
							auto iStitch      = 0u;
							auto iColorChange = 0u;
							auto color        = 0u;
							auto iPCSstitch   = 0u;
							while (iStitch < PCSHeader.stitchCount && iPCSstitch < pcsStitchCount) {
								if (PCSDataBuffer[iPCSstitch].tag == 3) {
									ColorChangeTable[iColorChange].colorIndex    = PCSDataBuffer[iPCSstitch].fx;
									ColorChangeTable[iColorChange++].stitchIndex = gsl::narrow<unsigned short>(iStitch);
									color                                        = NOTFRM | PCSDataBuffer[iPCSstitch++].fx;
								}
								else {
									StitchBuffer[iStitch].x
									    = PCSDataBuffer[iPCSstitch].x + static_cast<float>(PCSDataBuffer[iPCSstitch].fx) / 256.0;
									StitchBuffer[iStitch].y
									    = PCSDataBuffer[iPCSstitch].y + static_cast<float>(PCSDataBuffer[iPCSstitch].fy) / 256.0;
									StitchBuffer[iStitch++].attribute = color;
									iPCSstitch++;
								}
							}
							PCSHeader.stitchCount = gsl::narrow<unsigned short>(iStitch);
							// Grab the bitmap filename
							auto tnam = convert_ptr<char*>(&PCSDataBuffer[iPCSstitch]);
							strcpy_s(PCSBMPFileName, tnam);
							// wcscpy_s(fileExtention, sizeof(WorkingFileName) - (fileExtention - WorkingFileName), L"thr");
							IniFile.auxFileType = AUXPCS;
							if (PCSHeader.hoopType != LARGHUP && PCSHeader.hoopType != SMALHUP) {
								PCSHeader.hoopType = LARGHUP;
							}
							auto stitchRect = fRECTANGLE{};
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
						auto fileBuffer = new unsigned char[MAXITEMS * 8];
						ReadFile(FileHandle, fileBuffer, MAXITEMS * 8, &BytesRead, nullptr);
						auto pesHeader = convert_ptr<PESHED*>(fileBuffer);
						auto l_peschr  = convert_ptr<char*>(pesHeader);
						if (strncmp(pesHeader->led, "#PES00", 6)) {
							auto fmtStr = std::wstring{};
							displayText::loadString(fmtStr, IDS_NOTPES);
							displayText::shoMsg(fmt::format(fmtStr, WorkingFileName->wstring()));
							return;
						}
						auto pecof         = tripl(pesHeader->off);
						PESstitch          = fileBuffer + (pecof + 532);
						ActivePointIndex   = 0;
						auto pesColorCount = convert_ptr<unsigned char*>(&l_peschr[pecof + 48]);
						PEScolors          = &pesColorCount[1];
						// allow all the colors described by a char
						auto colorMap    = boost::dynamic_bitset<>(256);
						ActivePointIndex = 0;
						for (auto iColor = 0u; iColor < gsl::narrow<unsigned>(*pesColorCount + 1); iColor++) {
							if (!colorMap.test_set(PEScolors[iColor])) {
								UserColor[ActivePointIndex++] = PESColorTranslate[PEScolors[iColor] & PESCMSK];
								if (ActivePointIndex >= 16)
									break;
							}
						}
						auto color    = 0u;
						PEScolorIndex = 1;
						auto loc      = dPOINT{};
						StateMap.reset(StateFlag::FILDIR);
						auto iPESstitch         = 0u;
						auto iActualPESstitches = 1u;
						StitchBuffer[0].x       = StitchBuffer[0].y;
						auto locof              = 0.0;
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
					auto dstHeader = DSTHED{};
					ReadFile(FileHandle, &dstHeader, sizeof(dstHeader), &BytesRead, nullptr);
					if (BytesRead == sizeof(dstHeader)) {
						if (chkdst(&dstHeader)) {
							PCSBMPFileName[0] = 0;
							fileSize          = GetFileSize(FileHandle, &BytesRead) - sizeof(dstHeader);
							auto DSTData      = std::vector<DSTREC>(fileSize / sizeof(DSTREC));
							ReadFileInt(FileHandle, DSTData.data(), fileSize, &BytesRead, nullptr);
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
			ZoomRect     = { IniFile.hoopSizeY, 0, 0, IniFile.hoopSizeX };
			UnzoomedRect = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
			thred::movStch();
			thred::coltab();
			StateMap.reset(StateFlag::ZUMED);
			wchar_t buffer[3] = { 0 };
			for (auto iColor = 0u; iColor < 16; iColor++) {
				UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
				wcsncpy_s(buffer, ThreadSize[iColor], 2);
				SetWindowText(ThreadSizeWin[iColor], buffer);
			}
			for (auto& iColor : UserColorWin) {
				thred::redraw(iColor);
			}
			thred::redraw(ColorBar);
			if (PCSHeader.stitchCount) {
				StateMap.set(StateFlag::INIT);
			}
			nuAct(0);
			StateMap.set(StateFlag::RESTCH);
			nunams();
			ritini();
			const auto blank = std::wstring{};
			displayText::butxt(HNUM, blank);
			if (PCSHeader.stitchCount) {
				nuAct(StitchBuffer[0].attribute & COLMSK);
			}
			else {
				nuAct(0);
			}
			auxmen();
		}
		lenCalc();
		SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRDBY], WorkingFileName->wstring(), *DesignerName).c_str());
		CloseHandle(FileHandle);
		StateMap.set(StateFlag::INIT);
		StateMap.reset(StateFlag::TRSET);
		if (StateMap.test(StateFlag::NOTHRFIL)) {
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iStitch].attribute |= NOTFRM;
			}
		}
		repair::lodchk();
	}
}

COLORREF thred::internal::nuCol(COLORREF init) noexcept {
	ColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
	ColorStruct.hwndOwner      = ThrEdWindow;
	ColorStruct.lCustData      = 0;
	ColorStruct.lpCustColors   = CustomColor;
	ColorStruct.lpfnHook       = nullptr;
	ColorStruct.lpTemplateName = nullptr;
	ColorStruct.rgbResult      = init;
	ColorStruct.lStructSize    = sizeof(ColorStruct);
	return ChooseColor(&ColorStruct);
}

COLORREF thred::internal::nuBak() noexcept {
	BackgroundColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
	BackgroundColorStruct.hwndOwner      = ThrEdWindow;
	BackgroundColorStruct.lCustData      = 0;
	BackgroundColorStruct.lpCustColors   = CustomBackgroundColor;
	BackgroundColorStruct.lpfnHook       = nullptr;
	BackgroundColorStruct.lpTemplateName = nullptr;
	BackgroundColorStruct.rgbResult      = BackgroundColor;
	BackgroundColorStruct.lStructSize    = sizeof(BackgroundColorStruct);
	return ChooseColor(&BackgroundColorStruct);
}

COLORREF thred::internal::nuBit() noexcept {
	BitMapColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
	BitMapColorStruct.hwndOwner      = ThrEdWindow;
	BitMapColorStruct.lCustData      = 0;
	BitMapColorStruct.lpCustColors   = BitmapBackgroundColors;
	BitMapColorStruct.lpfnHook       = nullptr;
	BitMapColorStruct.lpTemplateName = nullptr;
	BitMapColorStruct.rgbResult      = BitmapColor;
	BitMapColorStruct.lStructSize    = sizeof(BackgroundColorStruct);
	return ChooseColor(&BitMapColorStruct);
}

void thred::pxCor2stch(const POINT& point) noexcept {
	auto ratio      = (static_cast<double>(point.x) - StitchWindowAbsRect.left) / StitchWindowClientRect.right;
	SelectedPoint.x = ratio * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	ratio           = (static_cast<double>(StitchWindowAbsRect.bottom) - point.y) / StitchWindowClientRect.bottom;
	SelectedPoint.y = ratio * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

bool thred::px2stch() noexcept {
	if (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right && Msg.pt.y >= StitchWindowAbsRect.top
	    && Msg.pt.y <= StitchWindowAbsRect.bottom) {
		auto tdub       = (static_cast<double>(Msg.pt.x) - StitchWindowAbsRect.left) / StitchWindowClientRect.right;
		SelectedPoint.x = tdub * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
		tdub            = (static_cast<double>(StitchWindowAbsRect.bottom) - Msg.pt.y) / StitchWindowClientRect.bottom;
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
				const auto& boundingRect = (*FormList)[ClosestFormToCursor].rectangle;

				SelectedPoint.x = ((boundingRect.right - boundingRect.left) / 2.0f) + boundingRect.left;
				SelectedPoint.y = ((boundingRect.top - boundingRect.bottom) / 2.0f) + boundingRect.bottom;
				break;
			}
			if (StateMap.test(StateFlag::FRMPSEL)) {
				SelectedPoint = (*FormList)[ClosestFormToCursor].vertices[ClosestVertexToCursor];
				break;
			}
			if (StateMap.test(StateFlag::SELBOX)) {
				SelectedPoint = StitchBuffer[ClosestPointIndex];
				break;
			}
			if (StateMap.test(StateFlag::GRPSEL)) {
				auto groupBoundingRect = fRECTANGLE{};
				thred::selRct(groupBoundingRect);
				SelectedPoint.x = ((groupBoundingRect.right - groupBoundingRect.left) / 2.0f) + groupBoundingRect.left;
				SelectedPoint.y = ((groupBoundingRect.top - groupBoundingRect.bottom) / 2.0f) + groupBoundingRect.bottom;
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
					SelectedPoint.x = (StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x) / 2.0f
					                  + StitchBuffer[ClosestPointIndex].x;
					SelectedPoint.y = (StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y) / 2.0f
					                  + StitchBuffer[ClosestPointIndex].y;
				}
				break;
			}
			if (!SelectedFormList->empty()) {
				auto        firstForm = SelectedFormList->front();
				const auto& firstRect = (*FormList)[firstForm].rectangle;
				SelectedFormsRect = { fToL(firstRect.left), fToL(firstRect.top), fToL(firstRect.right), fToL(firstRect.bottom) };
				for (auto selectedForm : (*SelectedFormList)) {
					const auto& rect = (*FormList)[selectedForm].rectangle;
					if (rect.bottom < SelectedFormsRect.bottom) {
						SelectedFormsRect.bottom = fToL(rect.bottom);
					}
					if (rect.top > SelectedFormsRect.top) {
						SelectedFormsRect.top = fToL(rect.top);
					}
					if (rect.left < SelectedFormsRect.left) {
						SelectedFormsRect.left = fToL(rect.left);
					}
					if (rect.right > SelectedFormsRect.right) {
						SelectedFormsRect.right = fToL(rect.right);
					}
				}
				SelectedPoint.x
				    = (static_cast<double>(SelectedFormsRect.right) - SelectedFormsRect.left) / 2.0 + SelectedFormsRect.left;
				SelectedPoint.y
				    = (static_cast<double>(SelectedFormsRect.top) - SelectedFormsRect.bottom) / 2.0 + SelectedFormsRect.bottom;
				break;
			}
			if (!thred::px2stch()) {
				centr();
			}
		} while (false);
	}
	const auto zoomRight = UnzoomedRect.x * ZoomFactor;
	ZoomRect             = { zoomRight / StitchWindowAspectRatio, 0, 0, zoomRight };
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
				const auto& boundingRect = (*FormList)[ClosestFormToCursor].rectangle;

				SelectedPoint.x = ((boundingRect.right - boundingRect.left) / 2) + boundingRect.left;
				SelectedPoint.y = ((boundingRect.top - boundingRect.bottom) / 2) + boundingRect.bottom;
				break;
			}
			if (StateMap.test(StateFlag::FRMPSEL)) {
				SelectedPoint = (*FormList)[ClosestFormToCursor].vertices[ClosestVertexToCursor];
				break;
			}
			if (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::INSRT)) {
				SelectedPoint = StitchBuffer[ClosestPointIndex];
				break;
			}
			if (StateMap.test(StateFlag::GRPSEL)) {
				auto groupBoundingRect = fRECTANGLE{};
				thred::selRct(groupBoundingRect);
				SelectedPoint.x = ((groupBoundingRect.right - groupBoundingRect.left) / 2) + groupBoundingRect.left;
				SelectedPoint.y = ((groupBoundingRect.top - groupBoundingRect.bottom) / 2) + groupBoundingRect.bottom;
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
		if (ZoomFactor > 0.98) {
			ZoomFactor = 1;
			StateMap.reset(StateFlag::ZUMED);
			ZoomRect = { gsl::narrow<double>(UnzoomedRect.y), 0, 0, gsl::narrow<double>(UnzoomedRect.x) };
			thred::movStch();
			NearestCount = 0;
		}
		else {
			const auto zoomRight = UnzoomedRect.x * ZoomFactor;
			ZoomRect             = { zoomRight / StitchWindowAspectRatio, 0, 0, zoomRight };
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

void thred::internal::duClos(unsigned startStitch, unsigned stitchCount) noexcept {
	for (auto iStitch = startStitch; iStitch < startStitch + stitchCount; iStitch++) {
		const auto cx    = ((StitchBuffer[iStitch].x > SelectedPoint.x) ? (StitchBuffer[iStitch].x - SelectedPoint.x)
                                                                     : (SelectedPoint.x - StitchBuffer[iStitch].x));
		const auto cy    = ((StitchBuffer[iStitch].y > SelectedPoint.y) ? (StitchBuffer[iStitch].y - SelectedPoint.y)
                                                                     : (SelectedPoint.y - StitchBuffer[iStitch].y));
		auto       sum   = hypot(cx, cy);
		auto       tind0 = iStitch;
		for (auto iNear = 0; iNear < NERCNT; iNear++) {
			if (sum < GapToNearest[iNear]) {
				const auto lowestSum = GapToNearest[iNear];
				const auto tind1     = NearestPoint[iNear];
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
	for (auto iNear = 0u; iNear < 4; iNear++) {
		GapToNearest[iNear] = 1e99;
		NearestPoint[iNear] = -1;
	}
	thred::px2stch();
	for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
		const auto iStitch0 = ColorChangeTable[iColor].stitchIndex;
		const auto iStitch1 = ColorChangeTable[iColor + 1].stitchIndex;
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
	for (const auto iNear : NearestPoint) {
		if (stch2px(iNear)) {
			NearestPoint[NearestCount]   = iNear;
			NearestPixel[NearestCount++] = StitchCoordinatesPixels;
		}
	}
	boxs();
}

bool thred::internal::closPnt1(unsigned* closestStitch) {
	if (closestStitch) {
		auto       closestIndex    = 0u;
		double     currentDistance = 1e99;
		const auto pointToCheck    = POINT{ (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };

		if (StateMap.test(StateFlag::SELBOX) && stch2px(ClosestPointIndex)) {
			if (hypot(StitchCoordinatesPixels.x - pointToCheck.x, StitchCoordinatesPixels.y - pointToCheck.y) < CLOSENUF) {
				*closestStitch = ClosestPointIndex;
				return true;
			}
		}
		for (auto iNear = 0u; iNear < NearestCount; iNear++) {
			const auto offset = BoxOffset[iNear];
			if (pointToCheck.x >= NearestPixel[iNear].x - offset && pointToCheck.x <= NearestPixel[iNear].x + offset
			    && pointToCheck.y >= NearestPixel[iNear].y - offset && pointToCheck.y <= NearestPixel[iNear].y + offset) {
				*closestStitch = NearestPoint[iNear];
				return true;
			}
		}
		thred::px2stch();
		DistanceToClick = 1e99;
		if (StateMap.test(StateFlag::HID)) {
			for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
				if (ColorChangeTable[iColor].colorIndex == ActiveColor) {
					for (auto iStitch = ColorChangeTable[iColor].stitchIndex; iStitch < ColorChangeTable[iColor + 1].stitchIndex;
					     iStitch++) {
						if (StitchBuffer[iStitch].x >= ZoomRect.left && StitchBuffer[iStitch].x <= ZoomRect.right
						    && StitchBuffer[iStitch].y >= ZoomRect.bottom && StitchBuffer[iStitch].y <= ZoomRect.top) {
							const auto cx
							    = ((StitchBuffer[iStitch].x > SelectedPoint.x) ? (StitchBuffer[iStitch].x - SelectedPoint.x)
							                                                   : (SelectedPoint.x - StitchBuffer[iStitch].x));
							const auto cy
							    = ((StitchBuffer[iStitch].y > SelectedPoint.y) ? (StitchBuffer[iStitch].y - SelectedPoint.y)
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
			for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				const auto layer = (StitchBuffer[iStitch].attribute & LAYMSK) >> LAYSHFT;
				if (!ActiveLayer || !layer || layer == ActiveLayer) {
					if (StitchBuffer[iStitch].x >= ZoomRect.left && StitchBuffer[iStitch].x <= ZoomRect.right
					    && StitchBuffer[iStitch].y >= ZoomRect.bottom && StitchBuffer[iStitch].y <= ZoomRect.top) {
						const auto cx   = StitchBuffer[iStitch].x - SelectedPoint.x;
						const auto cy   = StitchBuffer[iStitch].y - SelectedPoint.y;
						currentDistance = hypot(cx, cy);
						if (currentDistance < DistanceToClick) {
							DistanceToClick = currentDistance;
							closestIndex    = iStitch;
						}
					}
				}
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

unsigned thred::internal::pt2colInd(unsigned iStitch) noexcept {
	auto iColor = 0u;

	for (iColor = 0; iColor < ColorChanges; iColor++) {
		if (ColorChangeTable[iColor].stitchIndex > iStitch) {
			break;
		}
	}
	return iColor;
}

void thred::internal::toglup() {
	if (StateMap.testAndFlip(StateFlag::UPTO)) {
		displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
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
					while (ClosestPointIndex < PCSHeader.stitchCount
					       && form::notfstch(StitchBuffer[ClosestPointIndex].attribute)) {
						ClosestPointIndex++;
					}
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
	if (StateMap.testAndFlip(StateFlag::HID)) {
		StateMap.reset(StateFlag::FRMOF);
	}
	else {
		StateMap.set(StateFlag::FRMOF);
	}
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
		if (StateMap.test(StateFlag::ISDWN)) {
			Polyline(StitchWindowDC, MoveLine0, 2);
		}
		if (StateMap.test(StateFlag::ISUP)) {
			Polyline(StitchWindowDC, MoveLine1, 2);
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
	if (stch2px(ClosestPointIndex)) {
		unbox();
		OutputDebugString(fmt::format(L"Stitch:{} form:{} type:{}\n",
		                              ClosestPointIndex,
		                              ((StitchBuffer[ClosestPointIndex].attribute & FRMSK) >> FRMSHFT),
		                              ((StitchBuffer[ClosestPointIndex].attribute & TYPMSK) >> TYPSHFT))
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
	ritcor(StitchBuffer[ClosestPointIndex]);
}

bool thred::internal::chkhid(unsigned colorToCheck) {
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

	auto closestPoint = 0xffffffffu;
	auto sum          = 1e99;
	auto checkedPoint = dPOINT{};

	constexpr auto Tolerance = 20.0f;
	thi::unboxs();
	auto offset = (static_cast<float>(Msg.pt.x) - StitchWindowAbsRect.left)
	              / (static_cast<float>(StitchWindowAbsRect.right) - StitchWindowAbsRect.left);
	checkedPoint.x = offset * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	offset         = (static_cast<float>(StitchWindowAbsRect.bottom) - Msg.pt.y)
	         / (static_cast<float>(StitchWindowAbsRect.bottom) - StitchWindowAbsRect.top);
	checkedPoint.y       = (offset * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom);
	offset               = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
	const auto tolerance = offset * Tolerance;
	for (auto iChange = 0u; iChange < ColorChanges; iChange++) {
		auto stitchCount = gsl::narrow<unsigned short>(
		    std::abs(ColorChangeTable[iChange + 1].stitchIndex - ColorChangeTable[iChange].stitchIndex));
		const auto* stitches = &StitchBuffer[ColorChangeTable[iChange].stitchIndex];
		if (stitches) {
			if (thi::chkhid(iChange)) {
				for (auto iStitch = 0u; iStitch < stitchCount; iStitch++) {
					const auto layer = (stitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
					if (!ActiveLayer || !layer || (layer == ActiveLayer)) {
						auto       boundingRect = dRECTANGLE{};
						const auto xba          = fToL(stitches[iStitch + 1].x - stitches[iStitch].x);
						const auto yab          = fToL(stitches[iStitch].y - stitches[iStitch + 1].y);
						if (xba > 0) {
							boundingRect.left  = static_cast<double>(stitches[iStitch].x) - tolerance;
							boundingRect.right = static_cast<double>(stitches[iStitch + 1].x) + tolerance;
						}
						else {
							boundingRect.left  = static_cast<double>(stitches[iStitch + 1].x) - tolerance;
							boundingRect.right = static_cast<double>(stitches[iStitch].x) + tolerance;
						}
						if (yab < 0) {
							boundingRect.bottom = static_cast<double>(stitches[iStitch].y) - tolerance;
							boundingRect.top    = static_cast<double>(stitches[iStitch + 1].y) + tolerance;
						}
						else {
							boundingRect.bottom = static_cast<double>(stitches[iStitch + 1].y) - tolerance;
							boundingRect.top    = static_cast<double>(stitches[iStitch].y) + tolerance;
						}
						if (checkedPoint.x > boundingRect.left && checkedPoint.x < boundingRect.right
						    && checkedPoint.y > boundingRect.bottom && checkedPoint.y < boundingRect.top) {
							auto tsum         = 0.0;
							auto intersection = dPOINT{};
							auto dx           = 0.0;
							auto dy           = 0.0;
							do {
								if (yab == 0) {
									// stitch is horizontal
									intersection.x = checkedPoint.x;
									intersection.y = static_cast<double>(stitches[iStitch].y);
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
									const auto slope = static_cast<double>(xba) / yab;
									offset           = stitches[iStitch].x + slope * stitches[iStitch].y;
									const auto poff  = checkedPoint.x - checkedPoint.y / slope;
									intersection.x   = offset - slope * intersection.y;
									intersection.y   = slope * (offset - poff) / (slope * slope + 1);
									dx               = intersection.x - checkedPoint.x;
									dy               = intersection.y - checkedPoint.y;
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
		if (ClosestPointIndex && ClosestPointIndex != gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			thred::px2stch();
			const auto angt
			    = atan2(StitchBuffer[ClosestPointIndex].y - SelectedPoint.y, StitchBuffer[ClosestPointIndex].x - SelectedPoint.x);
			const auto angb = atan2(StitchBuffer[ClosestPointIndex].y - StitchBuffer[ClosestPointIndex - 1].y,
			                        StitchBuffer[ClosestPointIndex].x - StitchBuffer[ClosestPointIndex - 1].x);
			const auto angf = atan2(StitchBuffer[ClosestPointIndex].y - StitchBuffer[ClosestPointIndex + 1].y,
			                        StitchBuffer[ClosestPointIndex].x - StitchBuffer[ClosestPointIndex + 1].x);
			if (fabs(angf - angt) > fabs(angb - angt)) {
				ClosestPointIndex--;
			}
		}
		else {
			if (ClosestPointIndex == gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
				ClosestPointIndex--;
			}
		}
	}
	else {
		ClosestPointIndex = closlin();
	}
	if (ClosestPointIndex == -1) {
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
	if (PCSHeader.stitchCount) {
		auto iStitch = 0u;
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
		if (iStitch > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			iStitch = PCSHeader.stitchCount - 1;
		}
		GroupStitchIndex  = iStitch;
		ClosestPointIndex = iStitch;
		const auto color  = StitchBuffer[iStitch].attribute & COLMSK;
		while (ClosestPointIndex && (StitchBuffer[ClosestPointIndex].attribute & COLMSK) == color) {
			ClosestPointIndex--;
		}
		if ((StitchBuffer[ClosestPointIndex].attribute & COLMSK) != color) {
			ClosestPointIndex++;
		}
		while (GroupStitchIndex < PCSHeader.stitchCount && (StitchBuffer[GroupStitchIndex].attribute & COLMSK) == color) {
			GroupStitchIndex++;
		}
		if ((StitchBuffer[ClosestPointIndex].attribute & COLMSK) != color) {
			ClosestPointIndex--;
		}
		if (GroupStitchIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			GroupStitchIndex = PCSHeader.stitchCount - 1;
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
	TextureIndex          = 0;
	satin::clearGuideSize();
	FormIndex    = 0;
	ColorChanges = 0;
	KnotCount    = 0;
	WorkingFileName->clear();
	for (auto iColor = 0u; iColor < 16; iColor++) {
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
			for (auto& window : UserColorWin) {
				thred::redraw(window);
			}
		}
		ritcor(StitchBuffer[ClosestPointIndex]);
	}
}

void thred::delstchm() {
	thred::rngadj();
	auto destination = GroupStartStitch;
	for (auto iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[destination++] = StitchBuffer[iStitch];
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(destination);
}

void thred::internal::duclp() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, ClipInsertBoxLine, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unclp() {
	if (StateMap.testAndReset(StateFlag::CLPSHO)) {
		duclp();
	}
}

void thred::internal::clpbox() {
	const auto ratio = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);

	thred::px2stch();
	if (SelectedPoint.x + ClipRectSize.cx > UnzoomedRect.x) {
		SelectedPoint.x = UnzoomedRect.x - ClipRectSize.cx;
	}
	if (SelectedPoint.y + ClipRectSize.cy > UnzoomedRect.y) {
		SelectedPoint.y = UnzoomedRect.y - ClipRectSize.cy;
	}
	ClipOrigin              = { fToL(SelectedPoint.x), fToL(SelectedPoint.y) };
	const auto adjustedSize = SIZE{ dToL(ClipRectSize.cx * ratio + 0.5), dToL(ClipRectSize.cy * ratio + 0.5) };
	StitchCoordinatesPixels
	    = { dToL((SelectedPoint.x - ZoomRect.left) * ratio + 0.5),
		    dToL(StitchWindowClientRect.bottom - (SelectedPoint.y - ZoomRect.bottom) * ratio + 0.5 - adjustedSize.cy) };
	ClipInsertBoxLine[0].x = ClipInsertBoxLine[3].x = ClipInsertBoxLine[4].x = StitchCoordinatesPixels.x;
	ClipInsertBoxLine[0].y = ClipInsertBoxLine[1].y = ClipInsertBoxLine[4].y = StitchCoordinatesPixels.y;
	ClipInsertBoxLine[1].x = ClipInsertBoxLine[2].x = ClipInsertBoxLine[0].x + adjustedSize.cx;
	ClipInsertBoxLine[2].y = ClipInsertBoxLine[3].y = ClipInsertBoxLine[0].y + adjustedSize.cy;
	StateMap.set(StateFlag::CLPSHO);
	duclp();
}

void thred::internal::lodclp(unsigned iStitch) {
	const auto adjustment = fPOINT{ (ClipOrigin.x - ClipRect.left), (ClipOrigin.y - ClipRect.bottom) };
	auto       source     = PCSHeader.stitchCount;
	if (PCSHeader.stitchCount) {
		source--;
	}
	const auto clipSize    = ClipBuffer->size();
	auto       destination = PCSHeader.stitchCount + gsl::narrow<unsigned int>(clipSize);
	if (PCSHeader.stitchCount || clipSize) {
		destination--;
	}

	if (iStitch != PCSHeader.stitchCount) {
		iStitch++;
	}
	if (iStitch < gsl::narrow<unsigned>(PCSHeader.stitchCount) && StateMap.test(StateFlag::INIT)) {
		while (source && (source >= iStitch)) {
			// ToDo - is source being decremented too soon?
			StitchBuffer[destination].x           = StitchBuffer[source].x;
			StitchBuffer[destination].y           = StitchBuffer[source--].y;
			StitchBuffer[destination--].attribute = StitchBuffer[source].attribute;
		}
	}
	ClosestPointIndex = iStitch;
	for (auto& clip : *ClipBuffer) {
		StitchBuffer[iStitch++] = { clip.x + adjustment.x, clip.y + adjustment.y, clip.attribute & COLMSK | LayerIndex | NOTFRM };
	}
	GroupStitchIndex = iStitch - 1;
	StateMap.set(StateFlag::GRPSEL);
	PCSHeader.stitchCount += gsl::narrow<unsigned short>(ClipBuffer->size());
	if (PCSHeader.stitchCount) {
		StateMap.set(StateFlag::INIT);
	}
}

void thred::internal::rSelbox() {
	const auto ratio = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	const auto adjustedSelectSize
	    = SIZE{ static_cast<LONG>(SelectBoxSize.cx * ratio + 0.5), static_cast<LONG>(SelectBoxSize.cy * ratio + 0.5) };

	unsel();
	thred::px2stch();
	if (SelectedPoint.x - SelectBoxOffset.x + SelectBoxSize.cx >= UnzoomedRect.x) {
		SelectedPoint.x = UnzoomedRect.x - SelectBoxSize.cx + SelectBoxOffset.x;
	}
	if (SelectedPoint.y - SelectBoxOffset.y + SelectBoxSize.cy >= UnzoomedRect.y) {
		SelectedPoint.y = UnzoomedRect.y - SelectBoxSize.cy + SelectBoxOffset.y;
	}
	if (SelectedPoint.x - SelectBoxOffset.x < 0) {
		SelectedPoint.x = SelectBoxOffset.x;
	}
	if (SelectedPoint.y - SelectBoxOffset.y < 0) {
		SelectedPoint.y = SelectBoxOffset.y;
	}
	StitchCoordinatesPixels
	    = { dToL((SelectedPoint.x - ZoomRect.left - SelectBoxOffset.x) * ratio + 0.5),
		    dToL(StitchWindowClientRect.bottom - (SelectedPoint.y - ZoomRect.bottom - SelectBoxOffset.y) * ratio + 0.5
		         - adjustedSelectSize.cy) };
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

void thred::internal::duSelbox() {
	thred::px2stch();
	SelectBoxSize   = { fToL(StitchRangeRect.right - StitchRangeRect.left), fToL(StitchRangeRect.top - StitchRangeRect.bottom) };
	SelectBoxOffset = { fToL(SelectedPoint.x - StitchRangeRect.left), fToL(SelectedPoint.y - StitchRangeRect.bottom) };
}

void thred::internal::setbak(unsigned penWidth) noexcept {
	if (BackgroundPenWidth != penWidth) {
		BackgroundPen = nuPen(BackgroundPen, penWidth, BackgroundColor);
	}
}

void thred::internal::stchbox(unsigned iStitch, HDC dc) {
	POINT      line[5] = {};
	const auto layer   = (StitchBuffer[iStitch].attribute & LAYMSK) >> LAYSHFT;
	const auto offset  = MulDiv(IniFile.stitchSizePixels, *screenDPI, 96);

	if (!ActiveLayer || !layer || layer == ActiveLayer) {
		stch2px1(iStitch);
		line[0].x = line[3].x = line[4].x = StitchCoordinatesPixels.x - offset;
		line[0].y = line[1].y = StitchCoordinatesPixels.y - offset;
		line[1].x = line[2].x = StitchCoordinatesPixels.x + offset;
		line[2].y = line[3].y = StitchCoordinatesPixels.y + offset;
		line[4].y             = StitchCoordinatesPixels.y - offset;
		Polyline(dc, line, 5);
	}
}

void thred::sCor2px(const dPOINT& stitchCoordinate, POINT& pixelCoordinate) {
	pixelCoordinate = { dToL((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x + 0.5),
		                dToL(StitchWindowClientRect.bottom + (ZoomRect.bottom - stitchCoordinate.y) * ZoomRatio.y + 0.5) };
}

void thred::internal::sdCor2px(const fPOINTATTR stitchPoint, POINT& pixelCoordinate) {
	pixelCoordinate = { dToL((stitchPoint.x - ZoomRect.left) * ZoomRatio.x + 0.5),
		                dToL(StitchWindowClientRect.bottom + (ZoomRect.bottom - stitchPoint.y) * ZoomRatio.y + 0.5) };
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
	if (StateMap.testAndReset(StateFlag::ROTSHO)) {
		durot();
	}
}

void thred::internal::durotu() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, RotateBoxToCursorLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unrotu() {
	if (StateMap.testAndReset(StateFlag::ROTUSHO)) {
		durotu();
	}
}

void thred::internal::rotang(dPOINT unrotatedPoint, POINT& rotatedPoint, double rotationAngle, const dPOINT& rotationCenter) {
	auto       distanceToCenter = 0.0;
	auto       newAngle         = 0.0;
	const auto dx               = unrotatedPoint.x - rotationCenter.x;
	const auto dy               = unrotatedPoint.y - rotationCenter.y;

	if (dx) {
		distanceToCenter = hypot(dx, dy);
		newAngle         = atan2(dy, dx);
		newAngle += rotationAngle;
	}
	else {
		distanceToCenter = abs(dy);
		if (dy > 0) {
			newAngle = PI / 2 + rotationAngle;
		}
		else {
			newAngle = rotationAngle - PI / 2;
		}
	}
	const auto point
	    = dPOINT{ rotationCenter.x + distanceToCenter * cos(newAngle), rotationCenter.y + distanceToCenter * sin(newAngle) };
	thred::sCor2px(point, rotatedPoint);
}

void thred::rotang1(const fPOINTATTR& unrotatedPoint,
                    fPOINT&           rotatedPoint,
                    double            rotationAngle,
                    const dPOINT&     rotationCenter) noexcept {
	auto       distanceToCenter = 0.0;
	auto       newAngle         = 0.0;
	const auto dx               = unrotatedPoint.x - rotationCenter.x;
	const auto dy               = unrotatedPoint.y - rotationCenter.y;

	if (dx) {
		distanceToCenter = hypot(dx, dy);
		newAngle         = atan2(dy, dx);
		newAngle += rotationAngle;
	}
	else {
		if (dy > 0.0) {
			distanceToCenter = dy;
			newAngle         = PI / 2 + rotationAngle;
		}
		else {
			distanceToCenter = -dy;
			newAngle         = rotationAngle - PI / 2;
		}
	}
	rotatedPoint.x = rotationCenter.x + distanceToCenter * cos(newAngle);
	rotatedPoint.y = rotationCenter.y + distanceToCenter * sin(newAngle);
}

void thred::rotangf(const fPOINT& unrotatedPoint,
                    fPOINT&       rotatedPoint,
                    double        rotationAngle,
                    const dPOINT& rotationCenter) noexcept {
	auto       distanceToCenter = 0.0;
	auto       newAngle         = 0.0;
	const auto dx               = unrotatedPoint.x - rotationCenter.x;
	const auto dy               = unrotatedPoint.y - rotationCenter.y;

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
	rotatedPoint.x = rotationCenter.x + distanceToCenter * cos(newAngle);
	rotatedPoint.y = rotationCenter.y + distanceToCenter * sin(newAngle);
}

void thred::rotflt(fPOINT& point, const double rotationAngle, const dPOINT& rotationCenter) noexcept {
	auto       len      = 0.0;
	auto       newAngle = 0.0;
	const auto dx       = point.x - rotationCenter.x;
	const auto dy       = point.y - rotationCenter.y;

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
	point.x = rotationCenter.x + len * cos(newAngle);
	point.y = rotationCenter.y + len * sin(newAngle);
}

void thred::internal::rotstch(fPOINTATTR* stitch, const double rotationAngle, const dPOINT& rotationCenter) noexcept {
	auto       distanceToCenter = 0.0;
	auto       newAngle         = 0.0;
	const auto dx               = static_cast<double>(stitch->x) - rotationCenter.x;
	const auto dy               = static_cast<double>(stitch->y) - rotationCenter.y;

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
	auto rotated           = POINT{};
	auto rotationReference = dPOINT{ (RotationRect.left), (RotationRect.top) };

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
		if (!SelectedFormList->empty()) {
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
	rotationCenter = form::rotpar();
	ritrot(0, rotationCenter);
}

void thred::internal::savclp(unsigned int destination, unsigned int source) {
	auto integer = 0.0;

	ClipStitchData[destination].led  = StitchBuffer[source].attribute & COLMSK;
	auto fractional                  = modf(static_cast<double>(StitchBuffer[source].x) - LowerLeftStitch.x, &integer);
	ClipStitchData[destination].fx   = gsl::narrow<unsigned char>(std::floor(fractional * 256.0));
	ClipStitchData[destination].x    = gsl::narrow<unsigned short>(integer);
	ClipStitchData[destination].spcx = 0;
	fractional                       = modf(static_cast<double>(StitchBuffer[source].y) - LowerLeftStitch.y, &integer);
	ClipStitchData[destination].fy   = gsl::narrow<unsigned char>(std::floor(fractional * 256.0));
	ClipStitchData[destination].y    = gsl::narrow<unsigned short>(integer);
	ClipStitchData[destination].spcy = 0;
	// ToDo - Are these structure members needed?
	ClipStitchData[destination].myst = 1;
	ClipStitchData[destination].tag  = 0x14;
}

void thred::rtclpfn(unsigned int destination, unsigned int source) {
	auto  integer    = 0.0;
	auto& clipBuffer = *ClipBuffer;

	ClipStitchData[destination].led  = 0;
	auto fractional                  = modf(static_cast<double>(clipBuffer[source].x) - LowerLeftStitch.x, &integer);
	ClipStitchData[destination].fx   = gsl::narrow<unsigned char>(std::floor(fractional * 256.0));
	ClipStitchData[destination].x    = gsl::narrow<unsigned short>(integer);
	ClipStitchData[destination].spcx = 0;
	fractional                       = modf(static_cast<double>(clipBuffer[source].y) - LowerLeftStitch.y, &integer);
	ClipStitchData[destination].fy   = gsl::narrow<unsigned char>(std::floor(fractional * 256.0));
	ClipStitchData[destination].y    = gsl::narrow<unsigned short>(integer);
	ClipStitchData[destination].spcy = 0;
	// ToDo - Are these structure members needed?
	ClipStitchData[destination].myst = 1;
	ClipStitchData[destination].tag  = 0x14;
}

unsigned int thred::internal::sizfclp() {
	auto clipSize = gsl::narrow<unsigned int>(sizeof(*ClipFormHeader) + VertexCount * sizeof(FormVertices[0]));
	if (SelectedForm->type == SAT) {
		clipSize += SelectedForm->satinGuideCount * sizeof(SatinGuides[0]);
	}
	if (clip::iseclp(ClosestFormToCursor)) {
		clipSize += SelectedForm->clipEntries * sizeof((*ClipPoints)[0]);
	}
	if (clip::isclpx(ClosestFormToCursor)) {
		clipSize += SelectedForm->lengthOrCount.clipCount * sizeof((*ClipPoints)[0]);
	}
	if (texture::istx(ClosestFormToCursor)) {
		clipSize += SelectedForm->fillInfo.texture.count * sizeof(TexturePointsBuffer[0]);
	}
	return clipSize;
}

unsigned thred::internal::frmcnt(unsigned int iForm, unsigned& formFirstStitchIndex) noexcept {
	const auto codedAttribute = iForm << FRMSHFT;
	auto       iStitch        = 0u;

	LowerLeftStitch = fPOINT{ 1e20f, 1e20f };
	auto flag       = true;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute && StitchBuffer[iStitch].attribute & TYPMSK) {
			flag = false;
			break;
		}
	}
	if (flag) {
		return 0;
	}

	formFirstStitchIndex = iStitch;
	auto stitchCount     = 0u;
	while (iStitch < PCSHeader.stitchCount) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) == codedAttribute && StitchBuffer[iStitch].attribute & TYPMSK) {
			if (StitchBuffer[iStitch].x < LowerLeftStitch.x) {
				LowerLeftStitch.x = StitchBuffer[iStitch].x;
			}
			if (StitchBuffer[iStitch].y < LowerLeftStitch.y) {
				LowerLeftStitch.y = StitchBuffer[iStitch].y;
			}
			stitchCount++;
		}
		iStitch++;
	}
	return stitchCount;
}

unsigned int thred::internal::sizclp(unsigned& formFirstStitchIndex, unsigned& formStitchCount) {
	FileSize    = sizeof(*ClipFormHeader) + VertexCount * sizeof(FormVertices[0]);
	auto length = FileSize;
	if (SelectedForm->type == SAT) {
		FileSize += SelectedForm->satinGuideCount * sizeof(SatinGuides[0]);
	}
	if (SelectedForm->fillType || SelectedForm->edgeType) {
		formStitchCount = frmcnt(ClosestFormToCursor, formFirstStitchIndex);
		length += formStitchCount;
		FileSize += length * sizeof(StitchBuffer[0]);
	}
	if (clip::iseclp(ClosestFormToCursor)) {
		FileSize += SelectedForm->clipEntries * sizeof((*ClipPoints)[0]);
	}
	if (clip::isclpx(ClosestFormToCursor)) {
		FileSize += SelectedForm->lengthOrCount.clipCount * sizeof((*ClipPoints)[0]);
	}
	if (texture::istx(ClosestFormToCursor)) {
		FileSize += SelectedForm->fillInfo.texture.count * sizeof(TexturePointsBuffer[0]);
	}

	return length;
}

void thred::internal::duclip() {
	if (StateMap.test(StateFlag::FPSEL)) {
		if (OpenClipboard(ThrEdWindow)) {
			EmptyClipboard();
			ThrEdClip        = RegisterClipboardFormat(ThrEdClipFormat);
			ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
			                               (gsl::narrow<size_t>(SelectedFormVertices.vertexCount) + 1) * sizeof(fPOINT)
			                                   + sizeof(FORMVERTEXCLIP));
			GSL_SUPPRESS(26429) {
				if (ThrEdClipPointer) {
					auto* clipHeader        = *(static_cast<FORMVERTEXCLIP**>(ThrEdClipPointer));
					clipHeader->clipType    = CLP_FRMPS;
					clipHeader->vertexCount = SelectedFormVertices.vertexCount;
					if (StateMap.test(StateFlag::PSELDIR)) {
						clipHeader->direction = true;
					}
					else {
						clipHeader->direction = false;
					}
					// skip past the header
					auto* vertices = convert_ptr<fPOINT*>(&clipHeader[1]);
					form::fvars(ClosestFormToCursor);
					auto iSource = SelectedFormVertices.start;
					for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
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
	if (StateMap.test(StateFlag::BIGBOX)) {
		displayText::tabmsg(IDS_INSF);
	}
	else {
		if (OpenClipboard(ThrEdWindow)) {
			EmptyClipboard();
			ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
			if (!SelectedFormList->empty()) {
				auto msiz   = 0u;
				auto length = 0u;
				for (auto selectedForm : (*SelectedFormList)) {
					ClosestFormToCursor = selectedForm;
					form::fvars(ClosestFormToCursor);
					length += sizfclp();
					msiz += FileSize;
				}
				ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, gsl::narrow<size_t>(msiz) + length);
				GSL_SUPPRESS(26429) {
					if (ThrEdClipPointer) {
						ClipFormsHeader            = *(static_cast<FORMSCLIP**>(ThrEdClipPointer));
						ClipFormsHeader->clipType  = CLP_FRMS;
						ClipFormsHeader->formCount = gsl::narrow<unsigned short>(SelectedFormList->size());
						// Skip past the header
						auto forms = convert_ptr<FRMHED*>(&ClipFormsHeader[1]);
						auto iForm = 0u;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm   = &((*FormList)[selectedForm]);
							forms[iForm++] = (*FormList)[selectedForm];
						}
						// skip past the forms
						CurrentFormVertices = convert_ptr<fPOINT*>(&forms[iForm]);
						auto iVertex        = 0u;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &((*FormList)[selectedForm]);
							for (auto iSide = 0u; iSide < SelectedForm->vertexCount; iSide++) {
								CurrentFormVertices[iVertex++] = SelectedForm->vertices[iSide];
							}
						}
						// skip past the vertex list
						auto guides     = convert_ptr<SATCON*>(&CurrentFormVertices[iVertex]);
						auto guideCount = 0u;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &((*FormList)[selectedForm]);
							if (SelectedForm->type == SAT) {
								for (auto iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
									guides[guideCount++] = SelectedForm->satinOrAngle.guide[iGuide];
								}
							}
						}
						// skip past the guides
						auto points     = convert_ptr<fPOINT*>(&guides[guideCount]);
						auto pointCount = 0;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &((*FormList)[selectedForm]);
							if (clip::isclpx(selectedForm)) {
								auto offsetStart = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
								for (auto iClip = 0u; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {
									points[pointCount++] = *offsetStart;
									offsetStart++;
								}
							}
							if (clip::iseclp(selectedForm)) {
								auto offsetStart = ClipPoints->begin() + SelectedForm->borderClipData;
								for (auto iClip = 0u; iClip < SelectedForm->clipEntries; iClip++) {
									points[pointCount++] = *offsetStart;
									offsetStart++;
								}
							}
						}
						// Skip past the points
						auto textures     = convert_ptr<TXPNT*>(&points[pointCount]);
						auto textureCount = 0;
						iForm             = 0;
						for (auto selectedForm : (*SelectedFormList)) {
							SelectedForm = &((*FormList)[selectedForm]);
							if (texture::istx(selectedForm)) {
								auto startPoint = TexturePointsBuffer->cbegin() + SelectedForm->fillInfo.texture.index;
								auto endPoint   = startPoint + SelectedForm->fillInfo.texture.count;
								std::copy(startPoint,
								          endPoint,
								          stdext::make_checked_array_iterator(&textures[textureCount],
								                                              SelectedForm->fillInfo.texture.count));
								forms[iForm++].fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
								textureCount += SelectedForm->fillInfo.texture.count;
							}
						}
						SetClipboardData(ThrEdClip, ThrEdClipPointer);
					}
				}
				CloseClipboard();
				auto formMap = boost::dynamic_bitset<>(FormIndex);
				for (auto selectedForm : (*SelectedFormList)) {
					formMap.set(selectedForm);
				}
				// ToDo - what is astch used for?
				auto astch        = &StitchBuffer[MAXITEMS];
				auto stitchCount  = 0;
				LowerLeftStitch.x = LowerLeftStitch.y = 1e30f;
				for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
					if (!(StitchBuffer[iStitch].attribute & NOTFRM)
					    && formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
						if (StitchBuffer[iStitch].x < LowerLeftStitch.x) {
							LowerLeftStitch.x = StitchBuffer[iStitch].x;
						}
						if (StitchBuffer[iStitch].y < LowerLeftStitch.y) {
							LowerLeftStitch.y = StitchBuffer[iStitch].y;
						}
						astch[stitchCount++] = StitchBuffer[iStitch];
					}
				}
				if (PCSHeader.stitchCount && stitchCount) {
					Clip        = RegisterClipboardFormat(PcdClipFormat);
					ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2);
					if (ClipPointer) {
						ClipStitchData    = *(static_cast<CLPSTCH**>(ClipPointer));
						auto iStitch      = MAXITEMS;
						auto iDestination = 0u;
						savclp(0, iStitch);
						iStitch++;
						ClipStitchData[0].led = stitchCount;
						iDestination++;
						while (iStitch < stitchCount + MAXITEMS) {
							savclp(iDestination++, iStitch++);
						}
						SetClipboardData(Clip, ClipPointer);
					}
					CloseClipboard();
				}
			}
			else {
				if (StateMap.test(StateFlag::FORMSEL)) {
					auto       firstStitch = 0u; // points to the first stitch in a form
					auto       stitchCount = 0u;
					const auto length      = sizclp(firstStitch, stitchCount);
					form::fvars(ClosestFormToCursor);
					FileSize += sizeof(*ClipFormHeader);
					ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, FileSize);
					GSL_SUPPRESS(26429) {
						if (ThrEdClipPointer) {
							ClipFormHeader           = *(static_cast<FORMCLIP**>(ThrEdClipPointer));
							ClipFormHeader->clipType = CLP_FRM;
							ClipFormHeader->form     = (*FormList)[ClosestFormToCursor];
							CurrentFormVertices      = convert_ptr<fPOINT*>(&ClipFormHeader[1]);
							for (auto iSide = 0u; iSide < SelectedForm->vertexCount; iSide++) {
								CurrentFormVertices[iSide] = SelectedForm->vertices[iSide];
							}
							auto guides = convert_ptr<SATCON*>(&CurrentFormVertices[VertexCount]);
							auto iGuide = 0u;
							if (SelectedForm->type == SAT) {
								for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
									guides[iGuide] = SelectedForm->satinOrAngle.guide[iGuide];
								}
							}
							auto mclp  = convert_ptr<fPOINT*>(&guides[iGuide]);
							auto iClip = 0u;
							if (clip::isclpx(ClosestFormToCursor)) {
								auto offsetStart = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
								for (iClip = 0; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {
									mclp[iClip] = *offsetStart;
									offsetStart++;
								}
							}
							auto points = convert_ptr<fPOINT*>(&mclp[iClip]);
							if (clip::iseclpx(ClosestFormToCursor)) {
								auto offsetStart = ClipPoints->begin() + SelectedForm->borderClipData;
								for (iClip = 0; iClip < SelectedForm->clipEntries; iClip++) {
									points[iClip] = *offsetStart;
									offsetStart++;
								}
							}
							auto textures = convert_ptr<TXPNT*>(&points[iClip]);
							if (texture::istx(ClosestFormToCursor)) {
								auto startPoint = TexturePointsBuffer->cbegin() + SelectedForm->fillInfo.texture.index;
								auto endPoint   = startPoint + SelectedForm->fillInfo.texture.count;
								std::copy(startPoint,
								          endPoint,
								          stdext::make_checked_array_iterator(textures, SelectedForm->fillInfo.texture.count));
							}
							SetClipboardData(ThrEdClip, ThrEdClipPointer);
						}
						if ((SelectedForm->fillType || SelectedForm->edgeType)) {
							Clip        = RegisterClipboardFormat(PcdClipFormat);
							ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2);
							if (ClipPointer) {
								ClipStitchData = *(static_cast<CLPSTCH**>(ClipPointer));
								auto iTexture  = firstStitch;
								savclp(0, iTexture);
								ClipStitchData[0].led = length;
								iTexture++;
								auto iDestination   = 1u;
								auto codedAttribute = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
								while (iTexture < PCSHeader.stitchCount) {
									if ((StitchBuffer[iTexture].attribute & FRMSK) == codedAttribute
									    && !(StitchBuffer[iTexture].attribute & NOTFRM)) {
										savclp(iDestination++, iTexture);
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
					if (PCSHeader.stitchCount && StateMap.test(StateFlag::GRPSEL)) {
						Clip = RegisterClipboardFormat(PcdClipFormat);
						thred::rngadj();
						LowerLeftStitch.x = LowerLeftStitch.y = 1e30f;
						for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
							if (StitchBuffer[iStitch].x < LowerLeftStitch.x) {
								LowerLeftStitch.x = StitchBuffer[iStitch].x;
							}
							if (StitchBuffer[iStitch].y < LowerLeftStitch.y) {
								LowerLeftStitch.y = StitchBuffer[iStitch].y;
							}
						}
						const auto length  = GroupEndStitch - GroupStartStitch + 1;
						auto       iSource = GroupStartStitch;
						ClipPointer        = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length * sizeof(CLPSTCH) + 2);
						if (ClipPointer) {
							ClipStitchData = *(static_cast<CLPSTCH**>(ClipPointer));
							savclp(0, iSource);
							ClipStitchData[0].led = length;
							iSource++;
							for (auto iStitch = 1u; iStitch < length; iStitch++) {
								savclp(iStitch, iSource++);
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

void thred::delfstchs() {
	auto iDestinationStitch = 0u;

	for (auto iSourceStitch = 0u; iSourceStitch < PCSHeader.stitchCount; iSourceStitch++) {
		if (StitchBuffer[iSourceStitch].attribute & NOTFRM) {
			StitchBuffer[iDestinationStitch++] = StitchBuffer[iSourceStitch];
		}
		else {
			if (((StitchBuffer[iSourceStitch].attribute & FRMSK) >> 4) != ClosestFormToCursor) {
				StitchBuffer[iDestinationStitch++] = StitchBuffer[iSourceStitch];
			}
		}
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(iDestinationStitch);
}

void thred::internal::f1del() {
	if (StateMap.test(StateFlag::DELTO)) {
		auto cod         = gsl::narrow<unsigned int>(ClosestFormToCursor << FRMSHFT);
		auto stitchCount = 0u;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (FRMSK | NOTFRM)) != cod) {
				StitchBuffer[stitchCount] = StitchBuffer[iStitch];
				stitchCount++;
			}
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(stitchCount);
	}
	clip::deleclp(ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	satin::delsac(ClosestFormToCursor);
	form::delflt(ClosestFormToCursor);
	texture::deltx();
}

void thred::frmdel() {
	form::fvars(ClosestFormToCursor);
	thi::f1del();
	auto firstForm = (*FormList).begin();
	(*FormList).erase(firstForm + ClosestFormToCursor);
	if (StateMap.testAndReset(StateFlag::DELTO)) {
		for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM)) {
				const auto stitchAttributeFormBits = (StitchBuffer[iStitch].attribute & FRMSK);
				const auto stitchForm              = stitchAttributeFormBits >> 4;
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
				const auto stitchAttributeFormBits = (StitchBuffer[iStitch].attribute & FRMSK);
				const auto stitchForm              = stitchAttributeFormBits >> FRMSHFT;
				if (stitchForm == ClosestFormToCursor) {
					StitchBuffer[iStitch].attribute &= (NFRMSK & NTYPMSK);
				}
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
	if (code) {
		if (!(*FormList).empty()) {
			auto formIndices = std::vector<unsigned>(FormIndex);
			auto formMap     = boost::dynamic_bitset<>(FormIndex);
			for (auto selectedForm : (*SelectedFormList)) {
				ClosestFormToCursor = selectedForm;
				formMap.set(ClosestFormToCursor);
				form::fvars(ClosestFormToCursor);
				f1del();
			}
			auto deletedFormCount = 0u;
			auto firstForm        = (*FormList).begin();
			for (auto iForm = 0u; iForm < FormIndex; iForm++) {
				if (!formMap.test(iForm)) {
					formIndices[iForm] = (iForm - deletedFormCount) << FRMSHFT;
				}
				else {
					(*FormList).erase(firstForm + iForm - deletedFormCount);
					firstForm = (*FormList).begin(); // the erase invalidates firstForm
					deletedFormCount++;
				}
			}
			FormIndex = (*FormList).size();
			if (StateMap.test(StateFlag::DELTO)) {
				auto validStitchCount = 0u;
				for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
					if (StitchBuffer[iStitch].attribute & ALTYPMSK) {
						const auto iForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
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
				PCSHeader.stitchCount = gsl::narrow<unsigned short>(validStitchCount);
			}
			else {
				for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
					if (!(StitchBuffer[iStitch].attribute & NOTFRM)) {
						const auto iForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
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
bool thred::internal::cmpstch(unsigned iStitchA, unsigned iStitchB) noexcept {
	if (StitchBuffer[iStitchA].x != StitchBuffer[iStitchB].x) {
		return false;
	}

	return StitchBuffer[iStitchA].y == StitchBuffer[iStitchB].y;
}

void thred::internal::ofstch(unsigned iSource, char offset) noexcept {
	StitchBuffer[OutputIndex++]
	    = { StitchBuffer[iSource].x + KnotStep.x * offset, StitchBuffer[iSource].y + KnotStep.y * offset, KnotAttribute };
}

void thred::internal::endknt(unsigned finish) {
	auto length = 0.0;
	auto delta  = dPOINT{};
	auto iStart = finish - 1;
	if (!finish) {
		iStart++;
	}

	auto knots = gsl::narrow_cast<char*>(nullptr);

	KnotAttribute = StitchBuffer[iStart].attribute | KNOTMSK;
	do {
		delta = dPOINT{ static_cast<double>(StitchBuffer[finish].x) - StitchBuffer[iStart].x,
			            static_cast<double>(StitchBuffer[finish].y) - StitchBuffer[iStart].y };

		length = hypot(delta.x, delta.y);
		iStart--;
	} while (!length);
	if (StateMap.test(StateFlag::FILDIR)) {
		knots = KnotAtLastOrder;
	}
	else {
		knots = KnotAtEndOrder;
	}
	if (knots) {
		if (!(iStart & 0x8000000)) {
			KnotStep.x = 2.0 / length * delta.x;
			KnotStep.y = 2.0 / length * delta.y;
			for (auto iKnot = 0u; iKnot < 5; iKnot++) {
				ofstch(finish, knots[iKnot]);
			}
			if (StateMap.test(StateFlag::FILDIR)) {
				ofstch(finish, 0);
			}
		}
	}
}

void thred::internal::strtknt(unsigned start) noexcept {
	auto length = 0.0;
	auto delta  = dPOINT{};
	auto finish = start + 1;

	do {
		delta = dPOINT{ static_cast<double>(StitchBuffer[finish].x) - StitchBuffer[start].x,
			            static_cast<double>(StitchBuffer[finish].y) - StitchBuffer[start].y };

		length = hypot(delta.x, delta.y);
		finish++;
	} while (length < 2 && finish < PCSHeader.stitchCount);
	if (finish < PCSHeader.stitchCount) {
		KnotAttribute = StitchBuffer[start].attribute | KNOTMSK;
		KnotStep.x    = 2.0 / length * delta.x;
		KnotStep.y    = 2.0 / length * delta.y;
		for (const char iKnot : KnotAtStartOrder) {
			ofstch(start, iKnot);
		}
	}
}

void thred::internal::delknt() {
	auto newStitchCount = 0u;

	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (!(StitchBuffer[iStitch].attribute & KNOTMSK)) {
			thred::mvstch(newStitchCount++, iStitch);
		}
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(newStitchCount);
}

bool thred::internal::isknots() noexcept {
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (StitchBuffer[iStitch].attribute & KNOTMSK) {
			return true;
		}
	}
	return false;
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
	while (start < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1 && stlen(start) > KNOTLEN) {
		thred::mvstch(OutputIndex++, start++);
	}
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
	auto iStitch = 0u;

	// ToDo - Use a temp buffer rather than the high buffer
	OutputIndex = MAXITEMS;
	thred::mvstch(OutputIndex++, 0);
	strtknt(0);
	if (stlen(0) > KNOTLEN) {
		iStitch = kjmp(1);
	}
	else {
		iStitch = 1;
	}
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
	while (ActivePointIndex < KnotCount && Knots[ActivePointIndex] < source) {
		ActivePointIndex++;
	}
	ActivePointIndex--;
	if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5) {
		ActivePointIndex++;
		if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5) {
			return 0;
		}
		return 2;
	}

	ActivePointIndex++;
	if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5) {
		return 1;
	}
	return 3;
}

void thred::internal::chkncol() {
	auto initialColor = StitchBuffer[0].attribute & COLMSK;

	// ToDo - Use a temp buffer rather than the high buffer
	OutputIndex = MAXITEMS;
	StateMap.reset(StateFlag::FILDIR);
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		const auto color = StitchBuffer[iStitch].attribute & COLMSK;
		if (color == initialColor) {
			thred::mvstch(OutputIndex++, iStitch);
		}
		else {
			initialColor    = StitchBuffer[iStitch].attribute & COLMSK;
			const auto code = srchknot(iStitch);
			if (code & 1) {
				endknt(iStitch - 1);
			}
			thred::mvstch(OutputIndex++, iStitch);
			if (code & 2) {
				strtknt(iStitch);
			}
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
		if (!filename.empty() && filename.size() < 16) {
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
	if (StateMap.testAndFlip(StateFlag::HIDMAP)) {
		thi::duhbit(MF_UNCHECKED);
	}
	else {
		thi::duhbit(MF_CHECKED);
	}
	StateMap.set(StateFlag::DUMEN);
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::drwlstch(unsigned finish) {
	auto color = 0u;

	if (StateMap.test(StateFlag::HID)) {
		while (RunPoint < (finish - 1) && (StitchBuffer[RunPoint].attribute & COLMSK) != ActiveColor) {
			RunPoint++;
		}
	}
	if (StateMap.test(StateFlag::ZUMED)) {
		auto iMovieFrame = 1u;
		while (RunPoint < StitchesPerFrame + 1 && RunPoint < finish - 2 && !stch2px(RunPoint)) {
			RunPoint++;
		}
		const auto origin = RunPoint - 1;
		color             = StitchBuffer[RunPoint].attribute & COLMSK;
		auto flag         = true;
		while (iMovieFrame < StitchesPerFrame + 1 && RunPoint < finish - 2
		       && (StitchBuffer[RunPoint].attribute & COLMSK) == color) {
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
			if ((StitchBuffer[RunPoint].attribute & COLMSK) == color) {
				MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
				RunPoint++;
			}
		}
		SelectObject(StitchWindowDC, UserPen[color]);
		PolylineInt(StitchWindowDC, MovieLine, iMovieFrame);
		if (!flag) {
			RunPoint--;
		}
	}
	else {
		auto iMovieFrame = 0u;
		color            = StitchBuffer[RunPoint].attribute & COLMSK;
		SelectObject(StitchWindowDC, UserPen[color]);
		while (iMovieFrame < StitchesPerFrame && (RunPoint + 1 < finish - 1)
		       && ((StitchBuffer[RunPoint].attribute & COLMSK) == color)) {
			stch2px1(RunPoint++);
			MovieLine[iMovieFrame++] = StitchCoordinatesPixels;
		}
		RunPoint--;
		PolylineInt(StitchWindowDC, MovieLine, iMovieFrame);
	}
	if ((StitchBuffer[RunPoint + 1].attribute & 0xf) != color) {
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
		drwlstch(PCSHeader.stitchCount - 1);
	}
}

void thred::internal::setsped() {
	auto elapsedTimePerFrame = 0u;

	if (!MovieTimeStep) {
		MovieTimeStep = 1;
	}
	const auto userTimePerFrame = static_cast<double>(MovieTimeStep) / 10;
	if (userTimePerFrame < 10) {
		elapsedTimePerFrame = 100; // units are millseconds
		StitchesPerFrame    = gsl::narrow<unsigned>(std::round(elapsedTimePerFrame / userTimePerFrame));
		if (StitchesPerFrame > 99) {
			StitchesPerFrame = 99;
		}
	}
	else {
		elapsedTimePerFrame = gsl::narrow<unsigned>(std::round(userTimePerFrame));
		StitchesPerFrame    = 2;
	}
	if (StitchesPerFrame < 2) {
		StitchesPerFrame = 2;
	}
	SetTimer(ThrEdWindow, 0, elapsedTimePerFrame, nullptr);
}

void thred::internal::deltot() {
	DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
	TextureIndex = FormIndex = FormVertexIndex = PCSHeader.stitchCount = 0;
	satin::clearGuideSize();
	StateMap.reset(StateFlag::GMRK);
	rstAll();
	thred::coltab();
	thred::zumhom();
	SetWindowText(ThrEdWindow, fmt::format((*StringTable)[STR_THRDBY], ThrName->wstring(), *DesignerName).c_str());
}

bool thred::internal::wastch() noexcept {
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
			return true;
		}
	}
	return false;
}

bool thred::internal::frmstch() {
	auto formMap = boost::dynamic_bitset<>(FormIndex);

	for (auto form : (*SelectedFormList)) {
		formMap.set(form);
	}
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		const auto formCode = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
		if (formMap.test(formCode)) {
			return true;
		}
	}
	return false;
}

void thred::internal::delet() {
	thred::undat();
	if (StateMap.testAndReset(StateFlag::FPSEL)) {
		thred::savdo();
		form::fvars(ClosestFormToCursor);
		auto vertexMap         = boost::dynamic_bitset<>(VertexCount);
		auto currentFormVertex = SelectedFormVertices.start;
		for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertexMap.set(currentFormVertex);
			currentFormVertex = form::pdir(currentFormVertex);
		}
		currentFormVertex = 0;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			if (!vertexMap.test(iVertex)) {
				CurrentFormVertices[currentFormVertex++] = CurrentFormVertices[iVertex];
			}
		}
		currentFormVertex = form::fltind(&CurrentFormVertices[currentFormVertex]);
		auto iVertex      = form::fltind(&CurrentFormVertices[VertexCount]);
		while (iVertex < FormVertexIndex) {
			FormVertices[currentFormVertex++] = FormVertices[iVertex++];
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			(*FormList)[iForm].vertices -= (gsl::narrow<size_t>(SelectedFormVertices.vertexCount) + 1);
		}
		FormVertexIndex -= (SelectedFormVertices.vertexCount + 1);
		SelectedForm->vertexCount -= (SelectedFormVertices.vertexCount + 1);
		form::frmout(ClosestFormToCursor);
		if (SelectedForm->type == SAT) {
			satin::satadj();
		}
		form::refil();
		fndknt();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (GetKeyState(VK_CONTROL) & GetKeyState(VK_SHIFT) & 0x8000) {
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
		if (StateMap.test(StateFlag::FORMSEL) && FormIndex) {
			if (wastch()) {
				StateMap.set(StateFlag::DELFRM);
				displayText::tabmsg(IDS_FDEL);
				displayText::okcan();
				displayText::tomsg();
			}
			else {
				if (!(*FormList).empty()) {
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
				if (!stch2px(ClosestPointIndex)) {
					movbox();
				}
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
			SelectedForm = &((*FormList)[ClosestFormToCursor]);
			switch (SelectedForm->type) {
			case FRMLINE: {
				if (SelectedForm->fillType == CONTF) {
					if (ClosestVertexToCursor == SelectedForm->angleOrClipData.guide.start
					    || ClosestVertexToCursor == SelectedForm->angleOrClipData.guide.finish) {
						form::delmfil();
						SelectedForm->fillType = 0;
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return;
					}
					if (SelectedForm->angleOrClipData.guide.start > ClosestVertexToCursor) {
						SelectedForm->angleOrClipData.guide.start--;
					}
					if (SelectedForm->angleOrClipData.guide.finish > ClosestVertexToCursor) {
						SelectedForm->angleOrClipData.guide.finish--;
					}
				}
				break;
			}
			case SAT: {
				do {
					if (ClosestVertexToCursor <= 1) {
						if (SelectedForm->attribute & FRMEND) {
							if (SelectedForm->wordParam) {
								SelectedForm->wordParam = 0;
							}
							else {
								SelectedForm->attribute &= 0xfe;
							}
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
					for (auto iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
						if (SelectedForm->satinOrAngle.guide[iGuide].start == ClosestVertexToCursor
						    || SelectedForm->satinOrAngle.guide[iGuide].finish == ClosestVertexToCursor) {
							satin::delcon(iGuide);
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
			if (ClosestFormToCursor > FormIndex - 1) {
				ClosestFormToCursor = FormIndex - 1;
			}
			if (!(*FormList).empty()) {
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
				if (!stch2px(ClosestPointIndex)) {
					movbox();
				}
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
	rstAll();
	if (PCSHeader.stitchCount) {
		if (MsgWindow) {
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
			SpeedScrollBar = CreateWindow(L"SCROLLBAR",
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
		auto stepCount = 0.0;
		if (StateMap.test(StateFlag::ZUMED)) {
			stepCount = PCSHeader.stitchCount * ZoomFactor * ZoomFactor;
		}
		else {
			stepCount = PCSHeader.stitchCount;
		}
		if (!StateMap.test(StateFlag::WASPAT)) {
			MovieTimeStep = gsl::narrow<decltype(MovieTimeStep)>(std::round(10000.0 * MOVITIM / stepCount));
		}
		if (MovieTimeStep < MINDELAY) {
			MovieTimeStep = MINDELAY;
		}
		if (MovieTimeStep > MAXDELAY) {
			MovieTimeStep = MAXDELAY;
		}
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
	const auto codedLayer = ActiveLayer << LAYSHFT;

	ClipPointer = GlobalLock(ClipMemory);
	if (ClipPointer) {
		ClipStitchData        = static_cast<CLPSTCH*>(ClipPointer);
		const auto clipSize   = ClipStitchData[0].led;
		auto&      clipBuffer = *ClipBuffer;
		clipBuffer.clear();
		clipBuffer.reserve(clipSize);

		clipBuffer.emplace_back(fPOINTATTR{ ClipStitchData[0].x + static_cast<float>(ClipStitchData[0].fx) / 256.0f,
		                                    ClipStitchData[0].y + static_cast<float>(ClipStitchData[0].fy) / 256.0f,
		                                    0u });

#if CLPBUG
		OutputDebugString(fmt::format("interator: 0 x: {:6.2f},y: {:6.2f}\n", ClipBuffer[0].x, ClipBuffer[0].y).c_str());
#endif
		ClipRect.left = ClipRect.right = clipBuffer[0].x;
		ClipRect.bottom = ClipRect.top = clipBuffer[0].y;
		for (auto iStitch = 1u; iStitch < clipSize; iStitch++) {
			clipBuffer.emplace_back(
			    fPOINTATTR{ ClipStitchData[iStitch].x + static_cast<float>(ClipStitchData[iStitch].fx) / 256.0f,
			                ClipStitchData[iStitch].y + static_cast<float>(ClipStitchData[iStitch].fy) / 256.0f,
			                (ClipStitchData[iStitch].led & 0xf) | codedLayer });

#if CLPBUG
			OutputDebugString(
			    fmt::format("interator: {} x: {:6.2f},y: {:6.2f}\n", iStitch, ClipBuffer[iStitch].x, ClipBuffer[iStitch].y)
			        .c_str());
#endif
			if (clipBuffer[iStitch].x < ClipRect.left) {
				ClipRect.left = clipBuffer[iStitch].x;
			}
			if (clipBuffer[iStitch].x > ClipRect.right) {
				ClipRect.right = clipBuffer[iStitch].x;
			}
			if (clipBuffer[iStitch].y < ClipRect.bottom) {
				ClipRect.bottom = clipBuffer[iStitch].y;
			}
			if (clipBuffer[iStitch].y > ClipRect.top) {
				ClipRect.top = clipBuffer[iStitch].y;
			}
		}
		clipBuffer[0].attribute = ActiveColor | codedLayer;
		ClipRectSize            = { ClipRect.right - ClipRect.left, ClipRect.top - ClipRect.bottom };
		GlobalUnlock(ClipMemory);
		if (ClipRect.left || ClipRect.bottom) {
			for (auto& clip : *ClipBuffer) {
				clip.x -= ClipRect.left;
				clip.y -= ClipRect.bottom;
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

void thred::internal::drwmrk(HDC dc) {
	auto       markCoordinates = POINT{};
	POINT      markLine[2]     = {};
	const auto markOffset      = gsl::narrow<long>(MulDiv(6, *screenDPI, 96));

	thred::sCor2px(ZoomMarkPoint, markCoordinates);
	SelectObject(dc, ZoomMarkPen);
	SetROP2(dc, R2_XORPEN);
	markLine[0] = { markCoordinates.x - markOffset, markCoordinates.y - markOffset };
	markLine[1] = { markCoordinates.x + markOffset, markCoordinates.y + markOffset };
	Polyline(dc, markLine, 2);
	markLine[0] = { markCoordinates.x - markOffset, markCoordinates.y + markOffset };
	markLine[1] = { markCoordinates.x + markOffset, markCoordinates.y - markOffset };
	Polyline(dc, markLine, 2);
	SetROP2(dc, R2_COPYPEN);
}

void thred::internal::vubak() {
	if (FileHandle || StateMap.test(StateFlag::THUMSHO)) {
		StateMap.set(StateFlag::ZUMED);
		thred::movStch();
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		const auto dx = (StitchWindowClientRect.right >> 1);
		const auto dy = (StitchWindowClientRect.bottom >> 1);
		for (auto iVersion = 0u; iVersion < 4; iVersion++) { // there are 4 quadrants
			auto verticalLocation = 0l;
			if (iVersion & 2) {
				verticalLocation = dy;
			}
			BackupViewer[iVersion] = CreateWindow(L"STATIC",
			                                      L"",
			                                      SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			                                      dx * (iVersion & 1) + ButtonWidthX3,
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
	const auto offset    = POINT{ InsertSize.x >> 1, InsertSize.y >> 1 };
	auto&      formLines = *FormLines;
	formLines.resize(5);
	formLines[0].x = formLines[3].x = formLines[4].x = insertPoint.x - offset.x;
	formLines[1].x = formLines[2].x = insertPoint.x + offset.x;
	formLines[0].y = formLines[1].y = formLines[4].y = insertPoint.y - offset.y;
	formLines[2].y = formLines[3].y = insertPoint.y + offset.y;
}

bool thred::internal::isthr(const wchar_t* const filename) {
	auto thredPath = fs::path(filename);
	auto extention = thredPath.extension().wstring();

	return (extention.compare(0, 3, L".th") == 0);
}

unsigned thred::internal::gethand(const fPOINTATTR* stitch, unsigned stitchCount) noexcept {
	auto userStitchCount = 0u;
	if (stitch) {
		for (auto iStitch = 0u; iStitch < stitchCount; iStitch++) {
			if (stitch[iStitch].attribute & USMSK) {
				userStitchCount++;
			}
		}
	}
	return userStitchCount;
}

void thred::internal::insfil() {
	auto file = OPENFILENAME{
		sizeof(OPENFILENAME),                // lStructsize
		ThrEdWindow,                         // hwndOwner
		ThrEdInstance,                       // hInstance
		L"THR files\0*.thr\0\0",             // lpstrFilter
		CustomFilter,                        // lpstrCustomFilter
		_MAX_PATH,                           // nMaxCustFilter
		0,                                   // nFilterIndex
		InsertedFileName,                    // lpstrFile
		_MAX_PATH,                           // nMaxFile
		nullptr,                             // lpstrFileTitle
		0,                                   // nMaxFileTitle
		DefaultDirectory->wstring().c_str(), // lpstr	ialDir
		nullptr,                             // lpstrTitle
		OFN_EXPLORER | OFN_OVERWRITEPROMPT,  // Flags
		0,                                   // nFileOffset
		0,                                   // nFileExtension
		L"thr",                              // lpstrDefExt
		0,                                   // lCustData
		nullptr,                             // lpfnHook
		nullptr,                             // lpTemplateName
	};

	if (StateMap.test(StateFlag::IGNORINS) || GetOpenFileName(&file)) {
		InsertedFileHandle = CreateFile(InsertedFileName, (GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr);
		if (InsertedFileHandle == INVALID_HANDLE_VALUE) {
			displayText::filnopn(IDS_FNOPN, InsertedFileName);
			FileHandle = nullptr;
			CloseHandle(InsertedFileHandle);
		}
		else {
			InsertedStitchCount = PCSHeader.stitchCount;
			if (isthr(InsertedFileName)) {
				auto fileHeader = STRHED{};
				ReadFile(InsertedFileHandle, &fileHeader, sizeof(fileHeader), &BytesRead, nullptr);
				if ((fileHeader.headerType & 0xffffff) != 0x746872) {
					displayText::tabmsg(IDS_NOTHR);
				}
				else {
					constexpr auto FRMW        = 5;
					constexpr auto HANDW       = 4;
					constexpr auto FRMPW       = 2;
					constexpr auto STCHW       = 1;
					auto           homscor     = 0u;
					auto           thredHeader = STREX{};
					const auto     version     = (fileHeader.headerType & 0xff000000) >> 24;
					if (version) {
						gethand(StitchBuffer, PCSHeader.stitchCount);
						// ToDo - replace constants with sizes of data structures?
						homscor = (FormIndex)*FRMW + gethand(StitchBuffer, PCSHeader.stitchCount) * HANDW
						          + FormVertexIndex * FRMPW + PCSHeader.stitchCount * STCHW;
						ReadFile(InsertedFileHandle, &thredHeader, sizeof(thredHeader), &BytesRead, nullptr);
					}
					thred::savdo();
					if (fileHeader.stitchCount) {
						ReadFile(InsertedFileHandle,
						         &StitchBuffer[PCSHeader.stitchCount],
						         fileHeader.stitchCount * sizeof(StitchBuffer[0]),
						         &BytesRead,
						         nullptr);
					}
					const auto threadLength = (sizeof(ThreadSize) / sizeof(ThreadSize[0][0]))
					                          / 2; // ThreadSize is defined as a 16 entry array of 2 bytes
					const auto formDataOffset = sizeof(PCSBMPFileName) + sizeof(BackgroundColor) + sizeof(UserColor)
					                            + sizeof(CustomColor) + threadLength;
					SetFilePointer(InsertedFileHandle, formDataOffset, nullptr, FILE_CURRENT);
					auto insertedRectangle = fRECTANGLE{ 1e-9f, 1e9f, 1e-9f, 1e9f };
					InsertedVertexIndex    = FormVertexIndex;
					InsertedFormIndex      = FormIndex;
					if (fileHeader.formCount) {
						auto newFormVertexIndex = FormVertexIndex;
						auto newSatinGuideIndex = satin::getGuideSize();
						auto clipOffset  = ClipPoints->size();
						auto textureOffset    = TexturePointsBuffer->size();
						if (version < 2) {
							auto inFormList = std::vector<FRMHEDO>(fileHeader.formCount);
							ReadFile(InsertedFileHandle,
							         inFormList.data(),
							         fileHeader.formCount * sizeof(inFormList[0]),
							         &BytesRead,
							         nullptr);
							if (BytesRead != fileHeader.formCount * sizeof(inFormList[0])) {
								inFormList.resize(BytesRead / sizeof(inFormList[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							if (FormIndex + fileHeader.formCount < MAXFORMS) {
								for (auto form : inFormList) {
									(*FormList).emplace_back(FRMHED{ form });
								}
							}
							FormIndex += gsl::narrow<unsigned int>(inFormList.size());
						}
						else {
							auto inFormList  = std::vector<FRMHEDOUT>(fileHeader.formCount);
							auto bytesToRead = gsl::narrow<DWORD>(fileHeader.formCount * sizeof(inFormList[0]));
							ReadFileInt(InsertedFileHandle, inFormList.data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								inFormList.resize(BytesRead / sizeof(inFormList[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							if (FormIndex + fileHeader.formCount < MAXFORMS) {
								for (auto form : inFormList) {
									(*FormList).emplace_back(FRMHED{ form });
								}
							}
							FormIndex += gsl::narrow<unsigned int>(inFormList.size());
						}
						if (fileHeader.vertexCount) {
							auto bytesToRead = gsl::narrow<DWORD>(fileHeader.vertexCount * sizeof(FormVertices[0]));
							ReadFile(InsertedFileHandle, &FormVertices[FormVertexIndex], bytesToRead, &BytesRead, nullptr);
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
							auto inSatinGuides = std::vector<SATCONOUT>(fileHeader.dlineCount);
							auto bytesToRead   = gsl::narrow<DWORD>(fileHeader.dlineCount * sizeof(inSatinGuides[0]));
							ReadFile(FileHandle, inSatinGuides.data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								inSatinGuides.resize(BytesRead / sizeof(inSatinGuides[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							satin::cpyTmpGuides(inSatinGuides);
							newSatinGuideIndex += gsl::narrow<unsigned int>(inSatinGuides.size());
						}
						if (fileHeader.clipDataCount) {
							auto tempClipPoints = std::vector<fPOINT>{};
							tempClipPoints.resize(fileHeader.clipDataCount);
							auto bytesToRead = gsl::narrow<DWORD>(fileHeader.clipDataCount * sizeof((*ClipPoints)[0]));
							ReadFile(InsertedFileHandle, tempClipPoints.data(), bytesToRead, &BytesRead, nullptr);
							if (BytesRead != bytesToRead) {
								tempClipPoints.resize(BytesRead / sizeof(tempClipPoints[0]));
								StateMap.set(StateFlag::BADFIL);
							}
							ClipPoints->insert(ClipPoints->end(), tempClipPoints.begin(), tempClipPoints.end());
							//clipOffset += BytesRead / sizeof((*ClipPoints)[0]);
						}
						if (thredHeader.texturePointCount) {
							auto tempTextureBuffer = std::vector<TXPNT>{};
							tempTextureBuffer.resize(thredHeader.texturePointCount);
							ReadFile(InsertedFileHandle,
							         tempTextureBuffer.data(),
							         thredHeader.texturePointCount * sizeof(tempTextureBuffer[0]),
							         &BytesRead,
							         nullptr);
							TexturePointsBuffer->insert(TexturePointsBuffer->end(), tempTextureBuffer.begin(), tempTextureBuffer.end());
						}
						CloseHandle(InsertedFileHandle);
						InsertedFileHandle = nullptr;
						// update the form pointer variables
						for (auto iFormList = InsertedFormIndex; iFormList < FormIndex; iFormList++) {
							auto& formIter    = (*FormList)[iFormList];
							formIter.vertices = thred::adflt(formIter.vertexCount);
							if (formIter.type == SAT) {
								if (formIter.satinGuideCount) {
									formIter.satinOrAngle.guide = satin::adsatk(formIter.satinGuideCount);
								}
							}
							if (clip::isclp(iFormList)) {
								formIter.angleOrClipData.clip = clipOffset;
								clipOffset += formIter.lengthOrCount.clipCount;
							}
							if (clip::iseclpx(iFormList)) {
								formIter.borderClipData = clipOffset;
								clipOffset += formIter.clipEntries;
							}
							if (texture::istx(iFormList)) {
								formIter.fillInfo.texture.index = gsl::narrow<decltype(formIter.fillInfo.texture.index)>(textureOffset);
								textureOffset += formIter.fillInfo.texture.count;
							}
						}
						if (newFormVertexIndex != FormVertexIndex) {
							StateMap.set(StateFlag::BADFIL);
						}
						if (newSatinGuideIndex != satin::getGuideSize()) {
							StateMap.set(StateFlag::BADFIL);
						}
						if (clipOffset != ClipPoints->size()) {
							StateMap.set(StateFlag::BADFIL);
						}

						TextureIndex = textureOffset;
						if (fileHeader.formCount) {
							insertedRectangle.left = insertedRectangle.right = FormVertices[InsertedVertexIndex].x;
							insertedRectangle.bottom = insertedRectangle.top = FormVertices[InsertedVertexIndex].y;
							for (auto iVertex = InsertedVertexIndex + 1; iVertex < FormVertexIndex; iVertex++) {
								if (FormVertices[iVertex].x < insertedRectangle.left) {
									insertedRectangle.left = FormVertices[iVertex].x;
								}
								if (FormVertices[iVertex].x > insertedRectangle.right) {
									insertedRectangle.right = FormVertices[iVertex].x;
								}
								if (FormVertices[iVertex].y < insertedRectangle.bottom) {
									insertedRectangle.bottom = FormVertices[iVertex].y;
								}
								if (FormVertices[iVertex].y > insertedRectangle.top) {
									insertedRectangle.top = FormVertices[iVertex].y;
								}
							}
						}
					}
					if (fileHeader.stitchCount) {
						auto encodedFormIndex = gsl::narrow<unsigned int>(InsertedFormIndex << FRMSHFT);
						for (auto iStitch = PCSHeader.stitchCount; iStitch < PCSHeader.stitchCount + fileHeader.stitchCount;
						     iStitch++) {
							if (StitchBuffer[iStitch].attribute & ALTYPMSK) {
								const auto newAttribute = (StitchBuffer[iStitch].attribute & FRMSK) + encodedFormIndex;
								StitchBuffer[iStitch].attribute &= NFRMSK;
								StitchBuffer[iStitch].attribute |= newAttribute;
							}
							if (StitchBuffer[iStitch].x < insertedRectangle.left) {
								insertedRectangle.left = StitchBuffer[iStitch].x;
							}
							if (StitchBuffer[iStitch].x > insertedRectangle.right) {
								insertedRectangle.right = StitchBuffer[iStitch].x;
							}
							if (StitchBuffer[iStitch].y < insertedRectangle.bottom) {
								insertedRectangle.bottom = StitchBuffer[iStitch].y;
							}
							if (StitchBuffer[iStitch].y > insertedRectangle.top) {
								insertedRectangle.top = StitchBuffer[iStitch].y;
							}
						}
					}
					if (fileHeader.headerType & 0x1000000) {
						// ToDo - Replace constants with sizes of data structures
						const auto filscor = (fileHeader.formCount) * FRMW
						                     + gethand(&StitchBuffer[PCSHeader.stitchCount], fileHeader.stitchCount) * HANDW
						                     + fileHeader.vertexLen * FRMPW + fileHeader.stitchCount * STCHW;
						if (filscor > homscor) {
							for (auto iName = 0u; iName < 50; iName++) {
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
					const auto insertedSize = fPOINT{ insertedRectangle.right - insertedRectangle.left,
						                              insertedRectangle.top - insertedRectangle.bottom };
					form::ratsr();
					InsertSize.x = dToL(insertedSize.x * HorizontalRatio);
					// ToDo - Should this be vertical ratio?
					InsertSize.y = dToL(insertedSize.y * HorizontalRatio);
					const auto initialInsertPoint
					    = POINT{ StitchWindowClientRect.right >> 1, StitchWindowClientRect.bottom >> 1 };
					insflin(initialInsertPoint);
					NewFormVertexCount = 5;
					StateMap.set(StateFlag::SHOFRM);
					StateMap.set(StateFlag::INSFIL);
					form::dufrm();
				}
			}
			else {
				// ToDo - inserting PCS files is broken and needs to be fixed
				auto pcsFileHeader = PCSHEADER{};
				ReadFile(InsertedFileHandle, &pcsFileHeader, 0x46, &BytesRead, nullptr);
				if (PCSHeader.leadIn == 0x32 && PCSHeader.colorCount == 16) {
					thred::savdo();
					auto pcsStitchBuffer = std::vector<PCSTCH>(pcsFileHeader.stitchCount);
					ReadFile(InsertedFileHandle,
					         pcsStitchBuffer.data(),
					         pcsFileHeader.stitchCount * sizeof(pcsStitchBuffer[0]),
					         &BytesRead,
					         nullptr);
					auto iStitch      = PCSHeader.stitchCount;
					auto newAttribute = 0u;
					auto iPCSStitch   = 0u;
					for (iPCSStitch = 0u; iPCSStitch < pcsFileHeader.stitchCount; iPCSStitch++) {
						if (pcsStitchBuffer[iPCSStitch].tag == 3) {
							newAttribute = pcsStitchBuffer[iPCSStitch++].fx;
						}
						else {
							StitchBuffer[iStitch]
							    = { pcsStitchBuffer[iPCSStitch].x + static_cast<float>(pcsStitchBuffer[iPCSStitch].fx) / 256.0f,
								    pcsStitchBuffer[iPCSStitch].y + static_cast<float>(pcsStitchBuffer[iPCSStitch].fy) / 256.0f };
							StitchBuffer[iStitch++].attribute = newAttribute;
						}
					}
					const auto newStitchCount = iStitch;
					iStitch                   = PCSHeader.stitchCount;
					auto insertedRectangle    = fRECTANGLE{ StitchBuffer[iPCSStitch].y,
                                                         StitchBuffer[iPCSStitch].x,
                                                         StitchBuffer[iPCSStitch].x,
                                                         StitchBuffer[iPCSStitch].y };
					iPCSStitch++;
					while (iStitch < newStitchCount) {
						if (StitchBuffer[iStitch].x < insertedRectangle.left) {
							insertedRectangle.left = StitchBuffer[iStitch].x;
						}
						if (StitchBuffer[iStitch].x > insertedRectangle.right) {
							insertedRectangle.right = StitchBuffer[iStitch].x;
						}
						if (StitchBuffer[iStitch].y < insertedRectangle.bottom) {
							insertedRectangle.bottom = StitchBuffer[iStitch].y;
						}
						if (StitchBuffer[iStitch].y > insertedRectangle.top) {
							insertedRectangle.top = StitchBuffer[iStitch].y;
						}
						iStitch++;
					}
					InsertCenter.x = (insertedRectangle.right - insertedRectangle.left) / 2 + insertedRectangle.left;
					InsertCenter.y = (insertedRectangle.top - insertedRectangle.bottom) / 2 + insertedRectangle.bottom;

					PCSHeader.stitchCount   = newStitchCount;
					const auto insertedSize = fPOINT{ insertedRectangle.right - insertedRectangle.left,
						                              insertedRectangle.top - insertedRectangle.bottom };
					form::ratsr();
					InsertSize.x = dToL(insertedSize.x * HorizontalRatio);
					// ToDo - Should this be vertical ratio?
					InsertSize.y = dToL(insertedSize.y * HorizontalRatio);
					const auto initialInsertPoint
					    = POINT{ StitchWindowClientRect.right >> 1, StitchWindowClientRect.bottom >> 1 };
					insflin(initialInsertPoint);
					NewFormVertexCount = 5;
					StateMap.set(StateFlag::SHOFRM);
					StateMap.set(StateFlag::INSFIL);
					form::dufrm();
				}
			}
			if (InsertedFileHandle) {
				CloseHandle(InsertedFileHandle);
			}
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
				if (GetKeyState(VK_SHIFT) & 0x8000) {
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
	auto safetyFileName = *ThrName;
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
	auto destinationFileName = *ThrName;
	auto ext                 = ThrName->extension().wstring();

	ext.back() = source;
	sourceFileName.replace_extension(ext);
	ext.back() = destination;
	destinationFileName.replace_extension(ext);
	fs::remove(destinationFileName);
	fs::rename(sourceFileName, destinationFileName);
}

void thred::internal::purg() {
	if (FileHandle) {
		auto fileName = *ThrName;
		auto ext      = ThrName->extension().wstring();
		for (auto iLast = 0u; iLast < OLDVER; iLast++) {
			ext.back() = iLast + 's';
			fileName.replace_extension(ext);
			fs::remove(fileName);
		}
	}
}

void thred::internal::purgdir() {
	StateMap.set(StateFlag::PRGMSG);
	auto fmtStr = std::wstring{};
	displayText::loadString(fmtStr, IDS_DELBAK);
	displayText::shoMsg(fmt::format(fmtStr, DefaultDirectory->wstring()));
	displayText::okcan();
}

void thred::internal::deldir() {
	thred::unmsg();
	displayText::tabmsg(IDS_BAKDEL);
	auto backSpec = std::wstring{ L".th0" };

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
	auto windowRect = RECT{};

	GetWindowRect(window, &windowRect);
	return Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
	       && Msg.pt.y <= windowRect.bottom;
}

bool thred::internal::chkok() noexcept {
	return chkwnd(OKButton);
}

void thred::internal::mv2f() {
	if (StateMap.testAndReset(StateFlag::FORMSEL)) {
		// ToDo - Use a temp buffer rather than the high buffer
		thred::savdo();
		auto       iLowBuffer  = 0u;
		auto       iHighBuffer = MAXITEMS;
		const auto attribute   = ClosestFormToCursor << FRMSHFT;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			else {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
		}
		auto iStitch = MAXITEMS;
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
			auto iHighBuffer = MAXITEMS;
			auto iLowBuffer  = 0u;
			for (auto iStitch = 0u; iStitch < GroupStartStitch; iStitch++) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			for (auto iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			for (auto iStitch = MAXITEMS; iStitch < iHighBuffer; iStitch++) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void thred::internal::mv2b() {
	if (StateMap.testAndReset(StateFlag::FORMSEL)) {
		// ToDo - Use a temp buffer rather than the high buffer
		thred::savdo();
		auto       iLowBuffer  = 0u;
		auto       iHighBuffer = MAXITEMS;
		const auto attribute   = ClosestFormToCursor << FRMSHFT;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute & NOTFRM) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			else {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
		}
		auto iStitch = MAXITEMS;
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
			auto iHighBuffer = MAXITEMS;
			for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
				StitchBuffer[iHighBuffer++] = StitchBuffer[iStitch];
			}
			auto iLowBuffer = GroupStartStitch;
			for (auto iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iLowBuffer++] = StitchBuffer[iStitch];
			}
			for (auto iStitch = MAXITEMS; iStitch < iHighBuffer; iStitch++) {
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
			if (*xCoordinate > UnzoomedRect.x) {
				*xCoordinate = UnzoomedRect.x;
			}
			else {
				if (*xCoordinate < 0) {
					*xCoordinate = 0;
				}
			}
		}
		if (!_finite(*yCoordinate)) {
			if (*yCoordinate > UnzoomedRect.y) {
				*yCoordinate = UnzoomedRect.y;
			}
			else {
				if (*yCoordinate < 0) {
					*yCoordinate = 0;
				}
			}
		}
	}
}

void thred::delinf() noexcept {
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		thi::infadj(&StitchBuffer[iStitch].x, &StitchBuffer[iStitch].y);
	}
	for (auto iVertex = 0u; iVertex < FormVertexIndex; iVertex++) {
		thi::infadj(&FormVertices[iVertex].x, &FormVertices[iVertex].y);
	}
}

void thred::chkrng(fPOINT& range) {
	thred::delinf();
	range.x = gsl::narrow<double>(UnzoomedRect.x);
	range.y = gsl::narrow<double>(UnzoomedRect.y);
	if (!(*FormList).empty()) {
		auto stitchCount = 0u;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute & NOTFRM
			    || ((StitchBuffer[iStitch].attribute & TYPMSK)
			        && ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT < FormIndex))) {
				StitchBuffer[stitchCount] = StitchBuffer[iStitch];
				if (StitchBuffer[stitchCount].x > range.x) {
					StitchBuffer[stitchCount].x = range.x - 1;
				}
				if (StitchBuffer[stitchCount].x < 0) {
					StitchBuffer[stitchCount].x = 0;
				}
				if (StitchBuffer[stitchCount].y > range.y) {
					StitchBuffer[stitchCount].y = range.y - 1;
				}
				if (StitchBuffer[stitchCount].y < 0) {
					StitchBuffer[stitchCount].y = 0;
				}
				stitchCount++;
			}
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(stitchCount);
	}
	else {
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].x > range.x) {
				StitchBuffer[iStitch].x = range.x - 1;
			}
			if (StitchBuffer[iStitch].x < 0) {
				StitchBuffer[iStitch].x = 0;
			}
			if (StitchBuffer[iStitch].y > range.y) {
				StitchBuffer[iStitch].y = range.y - 1;
			}
			if (StitchBuffer[iStitch].y < 0) {
				StitchBuffer[iStitch].y = 0;
			}
		}
	}
}

void thred::ritmov() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if (ClosestVertexToCursor) {
		if (ClosestVertexToCursor == gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1 && SelectedForm->type == FRMLINE) {
			Polyline(StitchWindowDC, RubberBandLine->data(), 2);
		}
		else {
			Polyline(StitchWindowDC, RubberBandLine->data(), 3);
		}
	}
	else {
		(*RubberBandLine)[2] = (*FormLines)[1];
		if (SelectedForm->type == FRMLINE) {
			Polyline(StitchWindowDC, &(*RubberBandLine)[1], 2);
		}
		else {
			Polyline(StitchWindowDC, RubberBandLine->data(), 3);
		}
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unmov() {
	if (StateMap.testAndReset(StateFlag::SHOMOV)) {
		thred::ritmov();
	}
}

void thred::internal::duprct() {
	auto currentVertex = SelectedFormVertices.start;

	SelectedVerticesRect.left = SelectedVerticesRect.right = CurrentFormVertices[currentVertex].x;
	SelectedVerticesRect.top = SelectedVerticesRect.bottom = CurrentFormVertices[currentVertex].y;
	currentVertex                                          = form::pdir(currentVertex);
	for (auto iVertex = 0u; iVertex < SelectedFormVertices.vertexCount; iVertex++) {
		if (CurrentFormVertices[currentVertex].x < SelectedVerticesRect.left) {
			SelectedVerticesRect.left = CurrentFormVertices[currentVertex].x;
		}
		if (CurrentFormVertices[currentVertex].x > SelectedVerticesRect.right) {
			SelectedVerticesRect.right = CurrentFormVertices[currentVertex].x;
		}
		if (CurrentFormVertices[currentVertex].y < SelectedVerticesRect.bottom) {
			SelectedVerticesRect.bottom = CurrentFormVertices[currentVertex].y;
		}
		if (CurrentFormVertices[currentVertex].y > SelectedVerticesRect.top) {
			SelectedVerticesRect.top = CurrentFormVertices[currentVertex].y;
		}
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
	thred::savdo();
	if (StateMap.test(StateFlag::FPSEL)) {
		form::fvars(ClosestFormToCursor);
		auto currentVertex = SelectedFormVertices.start;
		for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
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
		for (auto iVertex = 0u; iVertex < FormVertexIndex; iVertex++) {
			thred::rotflt(FormVertices[iVertex], rotationAngle, rotationCenter);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			thi::rotstch(&StitchBuffer[iStitch], rotationAngle, rotationCenter);
		}
		for (auto iForm = 0u; iForm < FormIndex; iForm++) {
			form::frmout(iForm);
		}
		form::selal();
		return;
	}
	if (StateMap.testAndReset(StateFlag::FRMSROT)) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
				thred::rotflt(CurrentFormVertices[iVertex], rotationAngle, rotationCenter);
			}
			form::frmout(ClosestFormToCursor);
			form::refilfn();
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.testAndReset(StateFlag::FRMROT)) {
			form::fvars(ClosestFormToCursor);
			for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
				thred::rotflt(CurrentFormVertices[iVertex], rotationAngle, rotationCenter);
			}
			form::frmout(ClosestFormToCursor);
			form::refil();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
				thi::rotstch(&StitchBuffer[iStitch], rotationAngle, rotationCenter);
			}
			thred::rngadj();
			thi::selin(GroupStartStitch, GroupEndStitch, StitchWindowDC);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void thred::internal::rotfns(double rotationAngle) {
	thred::savdo();
	const auto rotationCenter = form::rotpar();
	thred::rotfn(rotationAngle, rotationCenter);
}

void thred::internal::nulayr(unsigned play) {
	ActiveLayer = play;
	LayerIndex  = play >> 1;
	ladj();
	if (ActiveLayer) {
		if (StateMap.test(StateFlag::FORMSEL)
		    && (gsl::narrow<unsigned>(((*FormList)[ClosestFormToCursor].attribute & FRMLMSK) >> 1) != ActiveLayer)) {
			StateMap.reset(StateFlag::FORMSEL);
		}
		StateMap.reset(StateFlag::GRPSEL);
		if (StateMap.test(StateFlag::SELBOX)) {
			if (ActiveLayer != ((StitchBuffer[ClosestPointIndex].attribute & LAYMSK) >> LAYSHFT) + 1) {
				StateMap.reset(StateFlag::SELBOX);
			}
		}
		SelectedFormList->clear();
	}
	StateMap.set(StateFlag::RESTCH);
}

bool thred::internal::iselpnt() {
	auto       closestControlPoint = 0u;
	auto       minimumLength       = 1e99;
	const auto pointToTest         = POINT{ (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };

	for (auto iControlPoint = 0u; iControlPoint < 9; iControlPoint++) {
		const auto length
		    = hypot(pointToTest.x - (*FormControlPoints)[iControlPoint].x, pointToTest.y - (*FormControlPoints)[iControlPoint].y);
		if (length < minimumLength) {
			minimumLength       = length;
			closestControlPoint = iControlPoint;
		}
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

bool thred::internal::chkbig(std::vector<POINT>& stretchBoxLine, double& xyRatio) {
	auto       minimumLength = 1e99;
	const auto pointToTest   = POINT{ (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };

	for (auto iControlPoint = 0u; iControlPoint < 9; iControlPoint++) {
		const auto length
		    = hypot(pointToTest.x - (*SelectedFormsLine)[iControlPoint].x, pointToTest.y - (*SelectedFormsLine)[iControlPoint].y);
		if (length < minimumLength) {
			minimumLength             = length;
			SelectedFormControlVertex = iControlPoint;
		}
	}
	auto& formLines = *FormLines;
	formLines.resize(5);
	for (auto iCorner = 0u; iCorner < 4; iCorner++) {
		formLines[iCorner] = (*SelectedFormsLine)[gsl::narrow<size_t>(iCorner) << 1];
	}
	formLines[4] = formLines[0];
	if (minimumLength < CLOSENUF) {
		for (auto iCorner = 0u; iCorner < 4; iCorner++) {
			stretchBoxLine[iCorner] = (*SelectedFormsLine)[gsl::narrow<size_t>(iCorner) << 1];
		}
		stretchBoxLine[4] = stretchBoxLine[0];
		thred::strtchbox(stretchBoxLine);
		if (SelectedFormControlVertex & 1) {
			StateMap.set(StateFlag::STRTCH);
		}
		else {
			StateMap.set(StateFlag::EXPAND);
			xyRatio = (static_cast<double>(SelectedFormsRect.right) - SelectedFormsRect.left)
			          / (static_cast<double>(SelectedFormsRect.bottom) - SelectedFormsRect.top);
		}
		SelectedFormControlVertex >>= 1;
		StateMap.set(StateFlag::SHOSTRTCH);
		return true;
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
		return true;
	}
	return false;
}

void thred::internal::delfre() {
	auto currentStitchCount = 0u;

	thred::savdo();
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (!(StitchBuffer[iStitch].attribute & NOTFRM)) {
			StitchBuffer[currentStitchCount++] = StitchBuffer[iStitch];
		}
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(currentStitchCount);
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
	auto start  = 0u;
	auto finish = 0u;

	if (LastFormSelected > ClosestFormToCursor) {
		start  = ClosestFormToCursor;
		finish = LastFormSelected;
	}
	else {
		start  = LastFormSelected;
		finish = ClosestFormToCursor;
	}
	SelectedFormList->clear();
	SelectedFormList->reserve(gsl::narrow<size_t>(finish) - start + 1);
	while (start <= finish) {
		SelectedFormList->push_back(start);
		start++;
	}
}

void thred::internal::selup() {
	if (GetKeyState(VK_SHIFT) & 0x8000) {
		StateMap.reset(StateFlag::SELBOX);
		if (StateMap.testAndReset(StateFlag::FORMSEL)) {
			if (ClosestFormToCursor < FormIndex - 1) {
				SelectedFormList->push_back(ClosestFormToCursor);
				LastFormSelected = ClosestFormToCursor + 1;
				SelectedFormList->push_back(LastFormSelected);
			}
			else {
				return;
			}
		}
		else {
			if (!SelectedFormList->empty()) {
				if (LastFormSelected < FormIndex - 1) {
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
			const auto attribute = StitchBuffer[ClosestPointIndex].attribute & ATMSK;
			while (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1
			       && (StitchBuffer[ClosestPointIndex].attribute & ATMSK) == attribute) {
				ClosestPointIndex++;
			}
			stch2px(ClosestPointIndex);
			dubox();
		}
		else {
			if (!(*FormList).empty()) {
				if (StateMap.testAndSet(StateFlag::FORMSEL)) {
					if (ClosestFormToCursor < gsl::narrow<unsigned>(FormIndex) - 1) {
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
	if (GetKeyState(VK_SHIFT) & 0x8000) {
		StateMap.reset(StateFlag::SELBOX);
		if (StateMap.testAndReset(StateFlag::FORMSEL)) {
			if (ClosestFormToCursor) {
				SelectedFormList->push_back(ClosestFormToCursor);
				LastFormSelected = ClosestFormToCursor - 1;
				SelectedFormList->push_back(LastFormSelected);
			}
			else {
				return;
			}
		}
		else {
			if (!SelectedFormList->empty()) {
				if (LastFormSelected) {
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
			const auto attribute = StitchBuffer[ClosestPointIndex].attribute & ATMSK;
			while (ClosestPointIndex && (StitchBuffer[ClosestPointIndex].attribute & ATMSK) == attribute) {
				ClosestPointIndex--;
			}
			stch2px(ClosestPointIndex);
			dubox();
		}
		else {
			if (!(*FormList).empty()) {
				if (StateMap.testAndSet(StateFlag::FORMSEL)) {
					if (ClosestFormToCursor) {
						ClosestFormToCursor--;
					}
				}
				else {
					ClosestFormToCursor = FormIndex - 1;
				}
				displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

bool thred::internal::movstchs(unsigned destination, unsigned start, unsigned finish) {
	auto dind = MAXITEMS;

	// ToDo - Use a temp buffer rather than the high buffer
	if ((destination + 1) < PCSHeader.stitchCount) {
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
		auto count = finish - start;
		thred::mvstchs(dind, start, count);
		dind += count;
		count = start - destination;
		thred::mvstchs(dind, destination, count);
		dind += count;
		thred::mvstchs(destination, MAXITEMS, dind - (MAXITEMS));
	}
	else {
		auto count = destination - finish;
		thred::mvstchs(dind, finish, count);
		dind += count;
		count = finish - start;
		thred::mvstchs(dind, start, count);
		dind += count;
		thred::mvstchs(start, MAXITEMS, dind - (MAXITEMS));
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
	auto color         = StitchBuffer[0].attribute & COLMSK;
	auto currentStitch = 0u;

	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (color != (StitchBuffer[iStitch].attribute & COLMSK)) {
			if ((iStitch - currentStitch == 1) && (currentStitch)) {
				StitchBuffer[iStitch - 1].attribute
				    = StitchBuffer[iStitch].attribute & NCOLMSK | (StitchBuffer[currentStitch - 1].attribute & COLMSK);
			}
			color         = StitchBuffer[iStitch].attribute & COLMSK;
			currentStitch = iStitch;
		}
	}
}

unsigned thred::internal::makbig(unsigned start, unsigned finish) {
	auto destination = MAXITEMS;
	auto adcnt       = 0u;

	finish--;
	// ToDo - Use a temp buffer rather than the high buffer
	for (auto source = start; source < finish; source++) {
		const auto delta
		    = dPOINT{ StitchBuffer[source + 1].x - StitchBuffer[source].x, StitchBuffer[source + 1].y - StitchBuffer[source].y };
		const auto length = hypot(delta.x, delta.y);
		thred::mvstch(destination++, source);
		if (length > IniFile.maxStitchLength) {
			const auto stitchCount = std::ceil(length / UserStitchLength);
			const auto step        = dPOINT{ delta.x / stitchCount, delta.y / stitchCount };
			auto       point       = dPOINT{ StitchBuffer[source].x + step.x, StitchBuffer[source].y + step.y };
			auto       attribute   = StitchBuffer[source].attribute;
			if (attribute != StitchBuffer[source + 1].attribute) {
				if (!(attribute & NOTFRM) && StitchBuffer[source + 1].attribute & TYPMSK) {
					if (!((attribute & FRMSK) == (StitchBuffer[source + 1].attribute & FRMSK))) {
						attribute &= NTYPMSK;
					}
				}
				else {
					attribute &= NTYPMSK;
				}
			}
			attribute &= (~KNOTMSK);
			for (auto iStitch = 0; iStitch < stitchCount - 1; iStitch++) {
				StitchBuffer[destination++] = { gsl::narrow<float>(point.x), gsl::narrow<float>(point.y), attribute };
				point.x += step.x;
				point.y += step.y;
				adcnt++;
			}
		}
	}
	auto source = finish;
	while (source < PCSHeader.stitchCount) {
		thred::mvstch(destination++, source++);
	}
	const auto* sourceStitch      = &StitchBuffer[MAXITEMS];
	auto*       destinationStitch = &StitchBuffer[start];
	if (sourceStitch && destinationStitch) {
		const auto stitchCount = destination - (MAXITEMS);
		for (source = 0; source < stitchCount; source++) {
			destinationStitch[source] = sourceStitch[source];
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(start + stitchCount);
	}
	return adcnt;
}

void thred::internal::rembig() {
	if (UserStitchLength < IniFile.maxStitchLength) {
		thred::savdo();
		do {
			if (!SelectedFormList->empty()) {
				auto range = RANGE{};
				for (auto selectedForm : (*SelectedFormList)) {
					form::frmrng(selectedForm, range);
					thi::makbig(range.start, range.finish);
				}
				break;
			}
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto range = RANGE{};
				form::frmrng(ClosestFormToCursor, range);
				thi::makbig(range.start, range.finish);
				break;
			}
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::rngadj();
				if (GroupEndStitch < PCSHeader.stitchCount) {
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
			thi::makbig(0, PCSHeader.stitchCount);
		} while (false);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::tabmsg(IDS_REM1);
	}
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
	auto iStitch = 0u;
	auto flag    = true;

	if (ClosestPointIndex == LargestStitchIndex) {
		return;
	}
	const auto currentLength = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x,
	                                 StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (iStitch = ClosestPointIndex + 1; iStitch < SelectedRange.finish; iStitch++) {
		const auto length
		    = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
		if (length == currentLength) {
			flag = false;
			break;
		}
	}
	if (flag) {
		auto       minimumLength = 1e99;
		const auto rangeEnd      = SelectedRange.finish - 1;
		for (auto currentStitch = SelectedRange.start; currentStitch < rangeEnd; currentStitch++) {
			const auto length = hypot(StitchBuffer[currentStitch + 1].x - StitchBuffer[currentStitch].x,
			                          StitchBuffer[currentStitch + 1].y - StitchBuffer[currentStitch].y);
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
	auto currentStitch = 0u;
	auto currentLength = 0.0;
	auto flag          = true;

	if (ClosestPointIndex == SmallestStitchIndex) {
		return;
	}
	currentLength = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x,
	                      StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (currentStitch = ClosestPointIndex; currentStitch != 0; currentStitch--) {
		const auto length = hypot(StitchBuffer[currentStitch].x - StitchBuffer[currentStitch - 1].x,
		                          StitchBuffer[currentStitch].y - StitchBuffer[currentStitch - 1].y);
		if (length == currentLength) {
			currentStitch--;
			flag = false;
			break;
		}
	}
	if (flag) {
		auto maximumLength = 0.0;
		auto iStitch       = 0u;
		for (iStitch = SelectedRange.start; iStitch < SelectedRange.finish - 1; iStitch++) {
			const auto length = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x,
			                          StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
			if (length < currentLength && length > maximumLength) {
				maximumLength = length;
				currentStitch = iStitch;
			}
		}
		const auto minLength
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
	return StitchBuffer[stitch].x >= StitchRangeRect.left && StitchBuffer[stitch].x <= StitchRangeRect.right
	       && StitchBuffer[stitch].y >= StitchRangeRect.bottom && StitchBuffer[stitch].y <= StitchRangeRect.top;
}

bool thred::internal::finrng(unsigned int find) {
	const auto& rectFind = (*FormList)[find].rectangle;
	if (rectFind.left >= StitchRangeRect.left && rectFind.right <= StitchRangeRect.right
	    && rectFind.bottom >= StitchRangeRect.bottom && rectFind.top <= StitchRangeRect.top) {
		if (!ActiveLayer) {
			return true;
		}
		const auto cod = ((*FormList)[find].attribute & FRMLMSK) >> 1;
		return !cod || ActiveLayer == cod;
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
			auto flag = true;
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
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
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		thred::rngadj();
		ClosestPointIndex = GroupEndStitch;
		StateMap.set(StateFlag::SELBOX);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			auto flag = true;
			for (auto iStitch = PCSHeader.stitchCount; iStitch != 0; iStitch--) {
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

void thred::internal::barnam(HWND window, unsigned iThumbnail) {
	if (iThumbnail < ThumbnailDisplayCount) {
		auto thumbPath = fs::path((*Thumbnails)[ThumbnailsSelected[iThumbnail]].data());
		auto name      = thumbPath.stem().wstring().substr(0, 12);
		SetWindowText(window, name.c_str());
	}
	else {
		SetWindowText(window, L"");
	}
}

void thred::internal::rthumnam(unsigned iThumbnail) {
	switch (iThumbnail) {
	case 0: {
		barnam((*ButtonWin)[HNUM], iThumbnail);
		break;
	}
	case 1: {
		barnam((*ButtonWin)[HTOT], iThumbnail);
		break;
	}
	case 2: {
		barnam((*ButtonWin)[HMINLEN], iThumbnail);
		break;
	}
	case 3: {
		barnam((*ButtonWin)[HMAXLEN], iThumbnail);
		break;
	}
	}
}

void thred::internal::thumnail() {
	auto fileData = WIN32_FIND_DATA{};

	thred::unbsho();
	thred::undat();
	trace::untrace();

	fs::current_path(*DefaultDirectory);
	*SearchName = *DefaultDirectory / L"*.thr";
	auto file   = FindFirstFile(SearchName->wstring().c_str(), &fileData);
	if (file == INVALID_HANDLE_VALUE) {
		const auto dwError = GetLastError();
		auto       fmtStr  = std::wstring{};
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
		auto       iThumbnail = 0u;
		const auto thumbSize  = Thumbnails->size();
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
		SetWindowText((*ButtonWin)[HBOXSEL], L"");
		const auto blank = std::wstring{};
		displayText::butxt(HBOXSEL, blank);
		vubak();
		StateMap.set(StateFlag::RESTCH);
	}
}

void thred::internal::nuthsel() {
	if (ThumbnailIndex < Thumbnails->size()) {
		const auto savedIndex = ThumbnailIndex;
		auto       iThumbnail = 0u;
		const auto length     = wcslen(ThumbnailSearchString);
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
			while (iThumbnail < 4) {
				rthumnam(iThumbnail++);
			}
		}
		else {
			ThumbnailIndex = savedIndex;
		}
	}
}

void thred::internal::nuthbak(unsigned count) {
	if (ThumbnailIndex) {
		const auto length = wcslen(ThumbnailSearchString);
		if (length) {
			while (count && ThumbnailIndex < MAXFORMS) {
				if (ThumbnailIndex) {
					ThumbnailIndex--;
					if (!wcsncmp(ThumbnailSearchString, (*Thumbnails)[ThumbnailIndex].data(), length)) {
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
	auto length = wcslen(ThumbnailSearchString);
	if (length < 16) {
		StateMap.set(StateFlag::RESTCH);
		ThumbnailSearchString[length++] = character;
		ThumbnailSearchString[length]   = 0;
		auto txt                        = std::wstring(ThumbnailSearchString);
		displayText::butxt(HBOXSEL, txt);
		ThumbnailIndex = 0;
		nuthsel();
	}
}

void thred::internal::bakthum() {
	auto searchStringLength = wcslen(ThumbnailSearchString);
	if (searchStringLength) {
		StateMap.set(StateFlag::RESTCH);
		ThumbnailSearchString[--searchStringLength] = 0;
		ThumbnailIndex                              = 0;
		auto txt                                    = std::wstring(ThumbnailSearchString);
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
	thred::px2stch();
	const auto offset = fPOINT{ SelectedPoint.x - InsertCenter.x, SelectedPoint.y - InsertCenter.y };
	for (auto iForm = InsertedFormIndex; iForm < FormIndex; iForm++) {
		auto& formRectangle = (*FormList)[iForm].rectangle;
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
	if (!StateMap.testAndReset(StateFlag::WASFPNT)) {
		StateMap.reset(StateFlag::GRPSEL);
		// ToDo - use .reserve instead with .push_back
		auto prng = std::vector<RANGE>(MAXITEMS);
		StateMap.reset(StateFlag::GRPSEL);
		auto iStitch     = 0u;
		auto iRange      = 0u;
		auto flagInRange = false;
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
		const auto rangeCount = iRange;
		if (rangeCount) {
			auto maximumLength = 0u;
			auto largestRange  = 0u;
			for (auto index = 0u; index < rangeCount; index++) {
				const auto length = prng[index].finish - prng[index].start;
				if (length > maximumLength) {
					maximumLength = length;
					largestRange  = index;
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
	auto formMap = boost::dynamic_bitset<>(FormIndex);
	for (auto selectedForm : (*SelectedFormList)) {
		formMap.set(selectedForm);
		SelectedForm = &((*FormList)[selectedForm]);
		if (SelectedForm->fillType) {
			SelectedForm->fillColor              = gsl::narrow<unsigned char>(ActiveColor);
			SelectedForm->fillInfo.feather.color = gsl::narrow<unsigned char>(ActiveColor);
		}
		if (SelectedForm->edgeType) {
			SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
		}
		if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
			SelectedForm->underlayColor = gsl::narrow<unsigned char>(ActiveColor);
		}
	}
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
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
		return true;
	}
	if (code >= VK_NUMPAD0 && code <= VK_NUMPAD9) {
		NumericCode = code - '0';
		return true;
	}
	return false;
}

void thred::stchrct(fRECTANGLE& rectangle) noexcept {
	if (PCSHeader.stitchCount) {
		rectangle.bottom = rectangle.left = 1e10;
		rectangle.top = rectangle.right = 0;
		for (auto index = 0u; index < PCSHeader.stitchCount; index++) {
			if (StitchBuffer[index].x < rectangle.left) {
				rectangle.left = StitchBuffer[index].x;
			}
			if (StitchBuffer[index].x > rectangle.right) {
				rectangle.right = StitchBuffer[index].x;
			}
			if (StitchBuffer[index].y < rectangle.bottom) {
				rectangle.bottom = StitchBuffer[index].y;
			}
			if (StitchBuffer[index].y > rectangle.top) {
				rectangle.top = StitchBuffer[index].y;
			}
		}
	}
}

void thred::frmrct(fRECTANGLE& rectangle) noexcept {
	rectangle.left = rectangle.right = FormVertices[0].x;
	rectangle.top = rectangle.bottom = FormVertices[0].y;
	for (auto iVertex = 0u; iVertex < FormVertexIndex; iVertex++) {
		if (FormVertices[iVertex].x < rectangle.left) {
			rectangle.left = FormVertices[iVertex].x;
		}
		if (FormVertices[iVertex].x > rectangle.right) {
			rectangle.right = FormVertices[iVertex].x;
		}
		if (FormVertices[iVertex].y > rectangle.top) {
			rectangle.top = FormVertices[iVertex].y;
		}
		if (FormVertices[iVertex].y < rectangle.bottom) {
			rectangle.bottom = FormVertices[iVertex].y;
		}
	}
}

void thred::internal::desiz() {
	auto rectangle   = fRECTANGLE{};
	auto info        = std::wstring{};
	auto stringTable = *StringTable;

	if (PCSHeader.stitchCount) {
		thred::stchrct(rectangle);
		const auto xSize = (rectangle.right - rectangle.left) / PFGRAN;
		const auto ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
		if ((rectangle.left < 0) || (rectangle.bottom < 0) || (rectangle.right > IniFile.hoopSizeX)
		    || (rectangle.top > IniFile.hoopSizeY)) {
			info += stringTable[STR_STCHOUT];
		}
		info += fmt::format(stringTable[STR_STCHS], PCSHeader.stitchCount, xSize, (xSize / 25.4), ySize, (ySize / 25.4));
	}
	if (!(*FormList).empty()) {
		thred::frmrct(rectangle);
		const auto xSize = (rectangle.right - rectangle.left) / PFGRAN;
		const auto ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
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
	auto hoopRectangle        = RECT{};
	auto preferencesRectangle = RECT{};

	StateMap.set(StateFlag::HUPMSG);
	GetWindowRect((*ValueWindow)[PHUP], &hoopRectangle);
	GetWindowRect(PreferencesWindow, &preferencesRectangle);
	SideMessageWindow = CreateWindow(L"STATIC",
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
	for (auto iHoop = 0u; iHoop < HUPS; iHoop++) {
		SideWindow[iHoop] = CreateWindow(L"STATIC",
		                                 (*StringTable)[gsl::narrow<size_t>(iHoop) + STR_HUP0].c_str(),
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
	if (!(*FormList).empty() || PCSHeader.stitchCount) {
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
	if (stitch.x < ClipRectAdjusted.left) {
		ClipRectAdjusted.left = stitch.x;
	}
	if (stitch.x > ClipRectAdjusted.right) {
		ClipRectAdjusted.right = stitch.x;
	}
	if (stitch.y < ClipRectAdjusted.bottom) {
		ClipRectAdjusted.bottom = stitch.y;
	}
	if (stitch.y > ClipRectAdjusted.top) {
		ClipRectAdjusted.top = stitch.y;
	}
}

void thred::internal::clpadj() {
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		auto iStitch          = GroupStartStitch;
		ClipRectAdjusted.left = ClipRect.right = StitchBuffer[iStitch++].x;
		ClipRectAdjusted.top = ClipRectAdjusted.bottom = StitchBuffer[iStitch].y;
		clpradj(StitchBuffer[iStitch]);
		while (iStitch < GroupEndStitch) {
			clpradj(StitchBuffer[iStitch++]);
		}
		if (StitchBuffer[iStitch].x < ClipRectAdjusted.left) {
			ClipRectAdjusted.left = StitchBuffer[iStitch].x;
		}
		if (StitchBuffer[iStitch].x > ClipRectAdjusted.right) {
			ClipRectAdjusted.right = StitchBuffer[iStitch].x;
		}
		const auto clipMiddle            = (ClipRectAdjusted.right - ClipRectAdjusted.left) / 2 + ClipRectAdjusted.left;
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
	else {
		displayText::shoseln(IDS_GRPMSG, IDS_RNGEND);
	}
}

void thred::internal::shftflt(fPOINT point) noexcept {
	if (point.x <= ZoomRect.left || point.x >= ZoomRect.right || point.y <= ZoomRect.bottom || point.y >= ZoomRect.top) {
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
	for (auto iColor = 0u; iColor < 16; iColor++) {
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
	if (!IniFile.customHoopX) {
		IniFile.customHoopX = LHUPX;
	}
	if (!IniFile.customHoopY) {
		IniFile.customHoopY = LHUPY;
	}
	PicotSpacing = IPICSPAC;
	UserFlagMap.set(UserFlag::FIL2OF);
	fil2men();
	BackgroundColor              = 0xa8c4b1;
	UnzoomedRect.x               = fToL(IniFile.hoopSizeX);
	UnzoomedRect.y               = fToL(IniFile.hoopSizeY);
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

void thred::internal::gselrng() {
	auto selectedFormList = *SelectedFormList;

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

double thred::internal::nuang(double yDelta, double xDelta) noexcept {
	const auto angle         = atan2(yDelta, xDelta);
	auto       relativeAngle = angle - OriginalAngle;
	if (fabs(relativeAngle) > PI) {
		if (relativeAngle > 0) {
			relativeAngle = 2 * PI - relativeAngle;
		}
		else {
			relativeAngle += 2 * PI;
		}
	}
	return relativeAngle;
}

void thred::internal::angdif(double angle) noexcept {
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
			const auto codedFormIndex = ClosestFormToCursor << FRMSHFT;
			form::fvars(ClosestFormToCursor);
			LowestAngle = HighestAngle = 0;
			OriginalAngle = atan2(CurrentFormVertices[0].y - ZoomMarkPoint.y, CurrentFormVertices[0].x - ZoomMarkPoint.x);
			for (auto iVertex = 1u; iVertex < VertexCount; iVertex++) {
				angdif(nuang(CurrentFormVertices[iVertex].y - ZoomMarkPoint.y, CurrentFormVertices[iVertex].x - ZoomMarkPoint.x));
			}
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) == codedFormIndex) {
					angdif(nuang(StitchBuffer[iStitch].y - ZoomMarkPoint.y, StitchBuffer[iStitch].x - ZoomMarkPoint.x));
				}
			}
		}
		else {
			thred::rngadj();
			LowestAngle = HighestAngle = 0;
			OriginalAngle
			    = atan2(StitchBuffer[GroupStartStitch].y - ZoomMarkPoint.y, StitchBuffer[GroupStartStitch].x - ZoomMarkPoint.x);
			for (auto iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
				angdif(nuang(StitchBuffer[iStitch].y - ZoomMarkPoint.y, StitchBuffer[iStitch].x - ZoomMarkPoint.x));
			}
		}
		const auto tAngle     = HighestAngle - LowestAngle;
		const auto segments   = std::round(2 * PI / tAngle);
		IniFile.rotationAngle = 2 * PI / segments;
		auto fmtStr           = std::wstring{};
		displayText::loadString(fmtStr, IDS_ROTMARK);
		// ToDo - should this be IniFile.rotationAngle?
		displayText::shoMsg(fmt::format(fmtStr, IniFile.fillAngle * 180 / PI, segments));
	}
	else {
		displayText::shoseln(IDS_FSZ, IDS_SETROTM);
	}
}

void thred::internal::segentr(double rotationAngle) {
	if (!rotationAngle) {
		rotationAngle = PI / 180;
	}
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
			const auto& vertex = (*FormList)[ClosestFormToCursor].vertices[ClosestVertexToCursor];
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

void thred::internal::nuslst(unsigned int find) {
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

constexpr unsigned thred::internal::byteSwap(unsigned data) noexcept {
	const auto a
	    = ((data & 0x000000FF) << 24) | ((data & 0x0000FF00) << 8) | ((data & 0x00FF0000) >> 8) | ((data & 0xFF000000) >> 24);
	return a;
}

void thred::internal::ritcur() noexcept {
	auto currentCursor = GetCursor();
	if (currentCursor) {
		auto iconInfo = ICONINFO{};
		GetIconInfo(currentCursor, &iconInfo);
		auto cursorPosition = POINT{};
		GetCursorPos(&cursorPosition);
		cursorPosition.x -= (StitchWindowOrigin.x + iconInfo.xHotspot);
		cursorPosition.y -= (StitchWindowOrigin.y + iconInfo.yHotspot);
		// ToDo - replace with GetDIBits
		unsigned char bitmapBits[64] = {};
		GetBitmapBits(iconInfo.hbmMask, 256, &bitmapBits);
		if (currentCursor == ArrowCursor) {
			for (auto iRow = 0u; iRow < 32; iRow++) {
				const auto mask          = byteSwap(bitmapBits[iRow]);
				const auto bitmapInverse = byteSwap(bitmapBits[iRow + 32]);
				auto       bitMask       = 0x80000000u;
				for (auto iPixel = 0u; iPixel < 32; iPixel++) {
					if (!(bitMask & mask)) {
						auto pixelColor = 0u;
						if (bitMask & bitmapInverse) {
							pixelColor = 0xffffff;
						}
						else {
							pixelColor = 0x000000;
						}
						SetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow, pixelColor);
					}
					bitMask >>= 1;
				}
			}
		}
		else {
			for (auto iRow = 0u; iRow < 32; iRow++) {
				const auto bitmapInverse = byteSwap(bitmapBits[iRow + 32]);
				auto       bitMask       = 0x80000000u;
				for (auto iPixel = 0u; iPixel < 32; iPixel++) {
					if (bitMask & bitmapInverse) {
						SetPixel(StitchWindowDC,
						         cursorPosition.x + iPixel,
						         cursorPosition.y + iRow,
						         GetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow) ^ 0xffffff);
					}
					bitMask >>= 1;
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

void thred::internal::respac() {
	if (clip::isclp(ClosestFormToCursor)) {
		SelectedForm->fillSpacing = LineSpacing;
		form::fsizpar();
	}
}

bool thred::internal::chkminus(unsigned code) {
	if (code == 189 || code == 109) {  // '-' key pressed
		if (PreferenceIndex == PFAZ) { // Clipboard Offset in preferences
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
		if (!GroupStartStitch) {
			GroupStartStitch++;
		}
		form::makspac(GroupEndStitch + 1, GroupEndStitch - GroupStartStitch);
		auto source      = GroupEndStitch - 1;
		auto destination = GroupEndStitch + 1;
		while (source && (source >= GroupStartStitch)) {
			StitchBuffer[destination++] = StitchBuffer[source--];
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_GRPMSG, IDS_RETRAC);
	}
}

void thred::internal::setgrd(COLORREF color) {
	const GRDCOD gridCodes[] = {
		ID_GRDHI, HIGRD, ID_GRDMED, MEDGRD, ID_GRDEF, DEFGRD, ID_GRDRED, REDGRD, ID_GRDBLU, BLUGRD, ID_GRDGRN, GRNGRD,
	};

	for (const auto gridCode : gridCodes) {
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

void thred::internal::fil2sel(unsigned stat) {
	UserFlagMap.set(UserFlag::FIL2OF);
	if (stat) {
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

void thred::internal::rotauxsel(unsigned stat) {
	UserFlagMap.set(UserFlag::ROTAUX);
	if (!stat) {
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

void thred::internal::frmcursel(unsigned cursorType) {
	UserFlagMap.set(UserFlag::FRMX);
	if (!cursorType) {
		UserFlagMap.reset(UserFlag::FRMX);
	}
	frmcurmen();
	StateMap.set(StateFlag::DUMEN);
}

void thred::internal::stchsnap(unsigned start, unsigned finish) noexcept {
	auto pnt = &StitchBuffer[start];

	for (auto i = 0u; i < finish - start; i++) {
		pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
		pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;

		pnt++;
	}
}

void thred::internal::frmsnap(fPOINT* start, unsigned int count) noexcept {
	for (auto i = 0u; i < count; i++) {
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
	if (!SelectedFormList->empty()) {
		thred::savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			auto& formIter      = (*FormList)[ClosestFormToCursor];
			frmsnap(formIter.vertices, formIter.vertexCount);
			form::frmout(ClosestFormToCursor);
			form::refil();
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			auto& formIter = (*FormList)[ClosestFormToCursor];
			frmsnap(formIter.vertices, formIter.vertexCount);
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
			else {
				displayText::shoseln(IDS_FGRPF, IDS_SNAP2GRD);
			}
		}
	}
}

void thred::internal::ritlock(const WIN32_FIND_DATA* fileData, unsigned fileIndex, HWND hwndlg) noexcept {
	if (fileData) {
		SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_RESETCONTENT, 0, 0);
		SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_RESETCONTENT, 0, 0);
		for (auto iFile = 0u; iFile < fileIndex; iFile++) {
			if (fileData[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
				GSL_SUPPRESS(26490)
				SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(fileData[iFile].cFileName));
			}
			else {
				GSL_SUPPRESS(26490)
				SendMessage(
				    GetDlgItem(hwndlg, IDC_UNLOCKED), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(fileData[iFile].cFileName));
			}
		}
	}
}

INT_PTR CALLBACK thred::internal::LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	auto fileInfo = gsl::narrow_cast<FINDINFO*>(nullptr);

	switch (umsg) {
	case WM_INITDIALOG: {
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		SetWindowLongPtr(hwndlg, DWLP_USER, lparam);
		GSL_SUPPRESS(26490) fileInfo = reinterpret_cast<FINDINFO*>(lparam);
		if (fileInfo) {
			auto searchName   = *DefaultDirectory / L"*.thr";
			auto searchResult = FindFirstFile(searchName.wstring().c_str(), &(fileInfo->data[0]));
			if (searchResult == INVALID_HANDLE_VALUE) {
				auto fmtStr = std::wstring{};
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
		GSL_SUPPRESS(26490) fileInfo = reinterpret_cast<FINDINFO*>(GetWindowLongPtr(hwndlg, DWLP_USER));
		if (fileInfo) {
			switch (LOWORD(wparam)) {
			case IDCANCEL: {
				EndDialog(hwndlg, wparam);
				return TRUE;
			}
			case IDC_LOCKAL: {
				for (auto iFile = 0u; iFile < fileInfo->count; iFile++) {
					fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
				}
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			}
			case IDC_UNLOCKAL: {
				for (auto iFile = 0u; iFile < fileInfo->count; iFile++) {
					fileInfo->data[iFile].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
				}
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			}
			case IDC_LOCK: {
				auto fileError    = 0u;
				auto unlockHandle = GetDlgItem(hwndlg, IDC_UNLOCKED);
				for (auto iFile = 0u; iFile < fileInfo->count; iFile++) {
					if (!(fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
						if (SendMessage(unlockHandle, LB_GETSEL, fileError, 0)) {
							fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
						}
						fileError++;
					}
				}
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			}
			case IDC_UNLOCK: {
				auto fileError  = 0u;
				auto lockHandle = GetDlgItem(hwndlg, IDC_LOCKED);
				for (auto iFile = 0u; iFile < fileInfo->count; iFile++) {
					if ((fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
						if (SendMessage(lockHandle, LB_GETSEL, fileError, 0)) {
							fileInfo->data[iFile].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
						}
						fileError++;
					}
				}
				ritlock(fileInfo->data, fileInfo->count, hwndlg);
				break;
			}
			case IDOK: {
				auto fileError = 0u;
				for (auto iFile = 0u; iFile < fileInfo->count; iFile++) {
					auto fileName = *DefaultDirectory / fileInfo->data[iFile].cFileName;
					if (!SetFileAttributes(fileName.wstring().c_str(), fileInfo->data[iFile].dwFileAttributes)) {
						fileError++;
					}
				}
				if (fileError) {
					auto fmtStr = std::wstring{};
					displayText::loadString(fmtStr, IDS_LOCKNOT);
					displayText::shoMsg(fmt::format(fmtStr, fileError));
				}
				EndDialog(hwndlg, wparam);
				return TRUE;
			}
			}
		}
	}
	}
	return false;
}

void thred::internal::lock() {
	// ToDo - investigate C++17 option shown here: http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
	auto lockInfo = FINDINFO{};

	lockInfo.count = 0;
	// ToDo - Replace 512 with maximum files in subdirectory
	lockInfo.data = new WIN32_FIND_DATA[512];

	GSL_SUPPRESS(26490)
	DialogBoxParam(ThrEdInstance, MAKEINTRESOURCE(IDD_DLOCK), ThrEdWindow, LockPrc, reinterpret_cast<LPARAM>(&lockInfo));

	delete[] lockInfo.data;
}

void thred::internal::delstch() {
	thred::savdo();
	PCSHeader.stitchCount = 0;
	TextureIndex          = 0;
	rstAll();
	form::clrfills();
	ColorChanges     = 0;
	const auto blank = std::wstring{};
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
	if (savcmp()) {
		closfn();
	}
	else {
		StateMap.set(StateFlag::FCLOS);
		displayText::savdisc();
	}
}

void thred::internal::frmpos(float deltaX, float deltaY) noexcept {
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		SelectedForm->vertices[iVertex].x += deltaX;
		SelectedForm->vertices[iVertex].y += deltaY;
	}
	SelectedForm->rectangle.bottom += deltaY;
	SelectedForm->rectangle.top += deltaY;
	SelectedForm->rectangle.left += deltaX;
	SelectedForm->rectangle.right += deltaX;
}

void thred::internal::nudgfn(float deltaX, float deltaY) {
	if (StateMap.test(StateFlag::BIGBOX) || !SelectedFormList->empty() || StateMap.test(StateFlag::FORMSEL)
	    || StateMap.test(StateFlag::GRPSEL) || StateMap.test(StateFlag::SELBOX)) {
		thred::savdo();
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		for (auto iForm = 0u; iForm < FormIndex; iForm++) {
			SelectedForm = &((*FormList)[iForm]);
			frmpos(deltaX, deltaY);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
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
	if (!SelectedFormList->empty()) {
		auto formMap = boost::dynamic_bitset<>(FormIndex);
		for (auto selectedForm : (*SelectedFormList)) {
			formMap.set(selectedForm);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
				StitchBuffer[iStitch].x += deltaX;
				StitchBuffer[iStitch].y += deltaY;
			}
		}
		for (auto selectedForm : (*SelectedFormList)) {
			SelectedForm = &((*FormList)[selectedForm]);
			frmpos(deltaX, deltaY);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::FORMSEL)) {
		SelectedForm = &((*FormList)[ClosestFormToCursor]);
		frmpos(deltaX, deltaY);
		if (SelectedForm->fillType || SelectedForm->edgeType) {
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
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
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
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
	auto pixel = POINT{};
	if (deltaX) {
		if (deltaX > 0) {
			pixel.x = IniFile.nudgePixels;
		}
		else {
			pixel.x = -IniFile.nudgePixels;
		}
	}
	if (deltaY) {
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
	if (StateMap.test(StateFlag::WASMRK)) {
		if (ZoomMarkPoint.x > IniFile.hoopSizeX) {
			ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
		}
		if (ZoomMarkPoint.y > IniFile.hoopSizeY) {
			ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
		}
		dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
		const fPOINT point(ZoomMarkPoint);
		thred::shft(point);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::tabmsg(IDS_MRK);
	}
}

void thred::internal::nuscol(unsigned iColor) noexcept {
	UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
	UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	thred::redraw(UserColorWin[iColor]);
}

void thred::internal::movchk() {
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
				const auto key          = GetKeyState(VK_SHIFT) & 0x8000;
				const auto switchColors = GetKeyState(VK_CONTROL) & 0x8000;
				for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
					const auto color = StitchBuffer[iStitch].attribute & COLMSK;
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
				for (auto iForm = 0u; iForm < FormIndex; iForm++) {
					auto& formIter = (*FormList)[iForm];
					if (formIter.fillType) {
						if (formIter.fillColor == VerticalIndex) {
							formIter.fillColor = gsl::narrow<unsigned char>(DraggedColor);
						}
						else {
							if (!key && formIter.fillColor == DraggedColor) {
								formIter.fillColor = gsl::narrow<unsigned char>(VerticalIndex);
							}
						}
						if (formIter.fillInfo.feather.color == VerticalIndex) {
							formIter.fillInfo.feather.color = gsl::narrow<unsigned char>(DraggedColor);
						}
						else {
							if (!key && formIter.fillInfo.feather.color == DraggedColor) {
								formIter.fillInfo.feather.color = gsl::narrow<unsigned char>(VerticalIndex);
							}
						}
					}
					if (formIter.edgeType) {
						if (formIter.borderColor == VerticalIndex) {
							formIter.borderColor = gsl::narrow<unsigned char>(DraggedColor);
						}
						else {
							if (!key && formIter.borderColor == DraggedColor) {
								formIter.borderColor = gsl::narrow<unsigned char>(VerticalIndex);
							}
						}
					}
				}
				if (!switchColors) {
					const auto swapColor     = UserColor[VerticalIndex];
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
	auto colorMap = boost::dynamic_bitset<>(16);
	if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {
		VerticalIndex &= COLMSK;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			colorMap.set(StitchBuffer[iStitch].attribute & COLMSK);
		}
		if (colorMap.all()) {
			displayText::tabmsg(IDS_COLAL);
		}
		else {
			auto nextColor = 15u;
			while (colorMap.test(nextColor)) {
				nextColor--;
			}
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				const auto color = StitchBuffer[iStitch].attribute & COLMSK;
				if (color >= VerticalIndex && color < nextColor) {
					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= color + 1;
				}
			}
			for (auto iForm = 0u; iForm < FormIndex; iForm++) {
				auto& formIter = (*FormList)[iForm];
				if (formIter.fillType) {
					if (formIter.fillColor >= VerticalIndex && formIter.fillColor < nextColor) {
						formIter.fillColor++;
					}
					if (formIter.fillInfo.feather.color >= VerticalIndex && formIter.fillInfo.feather.color < nextColor) {
						formIter.fillInfo.feather.color++;
					}
				}
				if (formIter.edgeType) {
					if (formIter.borderColor >= VerticalIndex && formIter.borderColor < nextColor) {
						formIter.borderColor++;
					}
				}
			}
			for (auto iColor = nextColor; iColor > VerticalIndex; iColor--) {
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
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & COLMSK) == VerticalIndex) {
			return true;
		}
	}
	return false;
}

void thred::internal::delcol() {
	if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {
		VerticalIndex &= 0xf;
		if (usedcol()) {
			displayText::tabmsg(IDS_COLU);
		}
		else {
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				const auto color = StitchBuffer[iStitch].attribute & COLMSK;
				if (color > VerticalIndex && color) {
					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= color - 1;
				}
			}
			for (auto iForm = 0u; iForm < FormIndex; iForm++) {
				auto formIter = (*FormList)[iForm];
				if (formIter.fillType) {
					if (formIter.fillColor > VerticalIndex) {
						formIter.fillColor--;
					}
					if (formIter.fillInfo.feather.color > VerticalIndex) {
						formIter.fillInfo.feather.color--;
					}
				}
				if (formIter.edgeType) {
					if (formIter.borderColor > VerticalIndex) {
						formIter.borderColor--;
					}
				}
			}
			for (auto iColor = VerticalIndex; iColor < 15; iColor++) {
				UserColor[iColor] = UserColor[iColor + 1];
				nuscol(iColor);
			}
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void thred::internal::set1knot() {
	if (PCSHeader.stitchCount && StateMap.test(StateFlag::SELBOX)) {
		thred::savdo();
		if (ClosestPointIndex == gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			StateMap.set(StateFlag::FILDIR);
			OutputIndex = ClosestPointIndex + 1;
			endknt(ClosestPointIndex);
		}
		else {
			auto source = PCSHeader.stitchCount;
			if (PCSHeader.stitchCount) {
				source--;
			}
			auto destination = PCSHeader.stitchCount + 4;
			while (source && source >= ClosestPointIndex) {
				thred::mvstch(destination--, source--);
			}
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
		ClosestVertexToCursor = (*FormList)[ClosestFormToCursor].vertexCount - 1;
	}
	StateMap.set(StateFlag::RESTCH);
}

void thred::internal::setpclp() {
	auto point = POINT{};

	form::sfCor2px(InterleaveSequence[0], point);
	FormVerticesAsLine[0] = point;
	form::sfCor2px(InterleaveSequence[1], point);
	const auto offset = POINT{ Msg.pt.x - StitchWindowOrigin.x - point.x, Msg.pt.y - StitchWindowOrigin.y - point.y };
	for (auto ine = 1u; ine < OutputIndex - 1; ine++) {
		form::sfCor2px(InterleaveSequence[ine], point);
		FormVerticesAsLine[ine] = { point.x + offset.x, point.y + offset.y };
	}
	form::sfCor2px(InterleaveSequence[OutputIndex - 1], point);
	FormVerticesAsLine[OutputIndex - 1] = point;
}

void thred::internal::dupclp() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	PolylineInt(StitchWindowDC, FormVerticesAsLine, OutputIndex);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unpclp() {
	if (StateMap.testAndReset(StateFlag::SHOP)) {
		dupclp();
	}
}

void thred::internal::fixpclp() {
	const auto point = POINT{ (Msg.pt.x + static_cast<LONG>(FormMoveDelta.x)), (Msg.pt.y + static_cast<LONG>(FormMoveDelta.y)) };

	thred::pxCor2stch(point);
	const auto offset = fPOINT{ SelectedPoint.x - InterleaveSequence[1].x, SelectedPoint.y - InterleaveSequence[1].y };
	auto       iNext  = form::nxt(ClosestVertexToCursor);
	const auto count  = OutputIndex - 2;
	form::fltspac(&CurrentFormVertices[iNext], count);
	for (auto iOutput = 1u; iOutput < OutputIndex - 1; iOutput++) {
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
	thred::ritfcor((*FormList)[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
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
	if (PCSHeader.stitchCount == 1) {
		PCSHeader.stitchCount = 0;
	}
	// ToDo - do we need to erase vertices and textures when aborting?
	if (StateMap.testAndReset(StateFlag::POLIMOV)) { // aborting form add
		(*FormList).pop_back();
		ClosestFormToCursor = (*FormList).size() - 1;
	};
	if (StateMap.testAndReset(StateFlag::FUNCLP)) { // aborting form paste
		(*FormList).pop_back();
		ClosestFormToCursor = (*FormList).size() - 1;
	}
	if (StateMap.testAndReset(StateFlag::FUNSCLP)) { // aborting forms paste
		for (auto iForm = 0u; iForm < ClipFormsCount; iForm++) {
			(*FormList).pop_back();
		}
		SelectedFormList->clear();
		ClosestFormToCursor = (*FormList).size() - 1;
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
	const auto blank = std::wstring{};
	displayText::butxt(HNUM, blank);
}

void thred::internal::drwLin(std::vector<POINT>& linePoints, unsigned currentStitch, unsigned length, HPEN hPen) {
	const auto* activeStitch = &StitchBuffer[currentStitch];

	if (activeStitch) {
		if (ActiveLayer) {
			LineIndex = 0;
		}
		auto iOffset = 0u;
		for (iOffset = 0; iOffset < length; iOffset++) {
			const auto layer = (activeStitch[iOffset].attribute & LAYMSK) >> LAYSHFT;
			if (!ActiveLayer || !layer || (layer == ActiveLayer)) {
				linePoints[LineIndex++]
				    = { dToL((activeStitch[iOffset].x - ZoomRect.left) * ZoomRatio.x),
					    dToL(StitchWindowClientRect.bottom - (activeStitch[iOffset].y - ZoomRect.bottom) * ZoomRatio.y) };
			}
		}
		SelectObject(StitchWindowMemDC, hPen);
		// ToDo - where did 16000 come from?
		if (LineIndex < 16000) {
			PolylineInt(StitchWindowMemDC, linePoints.data(), LineIndex);
		}
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
		LineIndex        = 1;
		const auto layer = (activeStitch[iOffset].attribute & LAYMSK) >> LAYSHFT;
		if (!ActiveLayer || !layer || layer == ActiveLayer) {
			if (iOffset) {
				linePoints[0]
				    = { dToL((activeStitch[iOffset - 1].x - ZoomRect.left) * ZoomRatio.x),
					    dToL(StitchWindowClientRect.bottom - (activeStitch[iOffset - 1].y - ZoomRect.bottom) * ZoomRatio.y) };
			}
			else {
				linePoints[0] = { dToL((activeStitch[0].x - ZoomRect.left) * ZoomRatio.x),
					              dToL(StitchWindowClientRect.bottom - (activeStitch[0].y - ZoomRect.bottom) * ZoomRatio.y) };
			}
		}
	}
	else {
		throw;
	}
}

BOOL CALLBACK thred::internal::fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	switch (umsg) {
	case WM_INITDIALOG: {
		const auto featherType = IniFile.featherType;
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), fmt::format(L"{:.2f}", IniFile.featherRatio).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), fmt::format(L"{}", IniFile.featherUpCount).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), fmt::format(L"{}", IniFile.featherDownCount).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFLR), fmt::format(L"{:.2f}", (IniFile.featherMinStitchSize / PFGRAN)).c_str());
		SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), fmt::format(L"{}", IniFile.featherCount).c_str());
		auto featherStyle = std::wstring{};
		for (auto iFeatherStyle = 0u; iFeatherStyle < 6; iFeatherStyle++) {
			displayText::loadString(featherStyle, (IDS_FTH0 + iFeatherStyle));
			GSL_SUPPRESS(26490)
			SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(featherStyle.c_str()));
		}
		SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, IniFile.featherFillType - 1, 0);
		auto state = 0u;
		if (featherType & AT_FTHBLND) {
			state = BST_CHECKED;
		}
		else {
			state = BST_UNCHECKED;
		}
		CheckDlgButton(hwndlg, IDC_FDBLND, state);
		if (featherType & AT_FTHUP) {
			state = BST_CHECKED;
		}
		else {
			state = BST_UNCHECKED;
		}
		CheckDlgButton(hwndlg, IDC_FDUP, state);
		if (featherType & AT_FTHBTH) {
			state = BST_CHECKED;
		}
		else {
			state = BST_UNCHECKED;
		}
		CheckDlgButton(hwndlg, IDC_FBTH, state);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case IDCANCEL: {
			EndDialog(hwndlg, 0);
			return TRUE;
		}
		case IDOK: {
			IniFile.featherType = 0;
			if (IsDlgButtonChecked(hwndlg, IDC_FDBLND)) {
				IniFile.featherType = AT_FTHBLND;
			}
			if (IsDlgButtonChecked(hwndlg, IDC_FDUP)) {
				IniFile.featherType |= AT_FTHUP;
			}
			if (IsDlgButtonChecked(hwndlg, IDC_FBTH)) {
				IniFile.featherType |= AT_FTHBTH;
			}
			wchar_t buf[HBUFSIZ] = { 0 };
			GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), buf, HBUFSIZ);
			IniFile.featherFillType = FDEFTYP;
			wchar_t buf1[HBUFSIZ]   = { 0 };
			for (auto iFeatherStyle = 0u; iFeatherStyle < 6; iFeatherStyle++) {
				LoadString(ThrEdInstance, IDS_FTH0 + iFeatherStyle, buf1, HBUFSIZ);
				if (!wcscmp(buf, buf1)) {
					IniFile.featherFillType = gsl::narrow<unsigned char>(iFeatherStyle + 1);
					break;
				}
			}
			GetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf, HBUFSIZ);
			IniFile.featherRatio = std::stof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf, HBUFSIZ);
			IniFile.featherUpCount = gsl::narrow<unsigned char>(std::stoi(buf));
			GetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf, HBUFSIZ);
			IniFile.featherDownCount = gsl::narrow<unsigned char>(std::stoi(buf));
			GetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf, HBUFSIZ);
			IniFile.featherMinStitchSize = std::stof(buf) * PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf, HBUFSIZ);
			IniFile.featherCount = gsl::narrow<unsigned short>(std::stoi(buf));
			if (IniFile.featherCount < 1) {
				IniFile.featherCount = 1;
			}
			EndDialog(hwndlg, 1);
			break;
		}
		}
	}
	}
	return false;
}

void thred::internal::dufdef() noexcept {
	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, (DLGPROC)fthdefprc);
}

bool thred::internal::chkMsg(std::vector<POINT>& stretchBoxLine,
                             double&             xyRatio,
                             double&             rotationAngle,
                             dPOINT&             rotationCenter,
                             FRMHED&             textureForm) { // NOLINT
	if (Msg.message == WM_MOUSEMOVE) {
		if (StateMap.test(StateFlag::TXTMOV)) {
			texture::txtrmov(textureForm);
			return true;
		}
		movchk();
		if (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right && Msg.pt.y >= StitchWindowAbsRect.top
		    && Msg.pt.y <= StitchWindowAbsRect.bottom) {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				xt::mvshft();
				return true;
			}
			do {
				if (GetKeyState(VK_SHIFT) & 0x8000 && thred::px2stch()) {
					thred::ritfcor(SelectedPoint);
				}
				if ((StateMap.test(StateFlag::PRFACT) || StateMap.test(StateFlag::FORMIN) || StateMap.test(StateFlag::POLIMOV))
				    || FormDataSheet) {
					setCursorInt(ArrowCursor);
					break;
				}
				if (!StateMap.test(StateFlag::INIT)) {
					if (UserFlagMap.test(UserFlag::NEDOF)) {
						setCursorInt(CrossCursor);
					}
					else {
						setCursorInt(NeedleUpCursor);
					}
					break;
				}
				if (StateMap.test(StateFlag::INSRT)) {
					if (UserFlagMap.test(UserFlag::NEDOF)) {
						setCursorInt(CrossCursor);
					}
					else {
						if (StateMap.test(StateFlag::LIN1)) {
							setCursorInt(NeedleUpCursor);
						}
						else {
							if (StitchBuffer[ClosestPointIndex + 1].x > StitchBuffer[ClosestPointIndex].x) {
								if (StitchBuffer[ClosestPointIndex + 1].y > StitchBuffer[ClosestPointIndex].y) {
									setCursorInt(NeedleLeftUpCursor);
								}
								else {
									setCursorInt(NeedleLeftDownCursor);
								}
							}
							else {
								if (StitchBuffer[ClosestPointIndex + 1].y > StitchBuffer[ClosestPointIndex].y) {
									setCursorInt(NeedleRightUpCursor);
								}
								else {
									setCursorInt(NeedleRightDownCursor);
								}
							}
						}
					}
					break;
				}
				if (StateMap.test(StateFlag::BZUMIN) || StateMap.test(StateFlag::BOXZUM) || StateMap.test(StateFlag::SELPNT)) {
					setCursorInt(CrossCursor);
					break;
				}
				if (StateMap.test(StateFlag::SATIN) || StateMap.test(StateFlag::SATPNT) || StateMap.test(StateFlag::INSFRM)) {
					if (UserFlagMap.test(UserFlag::FRMX)) {
						setCursorInt(CrossCursor);
					}
					else {
						setCursorInt(FormCursor);
					}
					break;
				}
				if (StateMap.test(StateFlag::SATCNKT)) {
					setCursorInt(DLineCursor);
				}
				else {
					setCursorInt(ArrowCursor);
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
				const auto point = POINT{ Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
				insflin(point);
				StateMap.set(StateFlag::SHOFRM);
				form::dufrm();
				return true;
			}
			if (StateMap.test(StateFlag::MOVFRMS)) {
				unstrtch(stretchBoxLine);
				stretchBoxLine[0].x = stretchBoxLine[3].x = stretchBoxLine[4].x
				    = Msg.pt.x - fToL(FormMoveDelta.x) - StitchWindowOrigin.x;
				stretchBoxLine[1].x = stretchBoxLine[2].x
				    = Msg.pt.x + fToL(SelectedFormsSize.x) - fToL(FormMoveDelta.x) - StitchWindowOrigin.x;
				stretchBoxLine[0].y = stretchBoxLine[1].y = stretchBoxLine[4].y
				    = Msg.pt.y - fToL(FormMoveDelta.y) - StitchWindowOrigin.y;
				stretchBoxLine[2].y = stretchBoxLine[3].y
				    = Msg.pt.y + fToL(SelectedFormsSize.y) - fToL(FormMoveDelta.y) - StitchWindowOrigin.y;
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
				auto       newSize = fPOINT{ gsl::narrow<float>(Msg.pt.x - StitchWindowOrigin.x),
                                       gsl::narrow<float>(Msg.pt.y - StitchWindowOrigin.y) };
				auto       iSide   = (SelectedFormControlVertex + 2) % 4;
				const auto ratio   = std::fabs((static_cast<double>(newSize.x) - stretchBoxLine[iSide].x)
                                             / (static_cast<double>(newSize.y) - stretchBoxLine[iSide].y));
				if (iSide & 1) {
					if (ratio < xyRatio) {
						newSize.x = (stretchBoxLine[iSide].y - newSize.y) * xyRatio + stretchBoxLine[iSide].x;
					}
					else {
						newSize.y
						    = (static_cast<double>(stretchBoxLine[iSide].x) - newSize.x) / xyRatio + stretchBoxLine[iSide].y;
					}
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].y = dToL(newSize.y);
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = dToL(newSize.x);
					stretchBoxLine[iSide].y = dToL(newSize.y);
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = dToL(newSize.x);
				}
				else {
					if (ratio < xyRatio) {
						newSize.x = (newSize.y - stretchBoxLine[iSide].y) * xyRatio + stretchBoxLine[iSide].x;
					}
					else {
						newSize.y
						    = (static_cast<double>(newSize.x) - stretchBoxLine[iSide].x) / xyRatio + stretchBoxLine[iSide].y;
					}
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = dToL(newSize.x);
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].x = dToL(newSize.x);
					stretchBoxLine[iSide].y = dToL(newSize.y);
					iSide                   = nxtcrnr(iSide);
					stretchBoxLine[iSide].y = dToL(newSize.y);
				}
				stretchBoxLine[4] = stretchBoxLine[0];
				StateMap.set(StateFlag::SHOSTRTCH);
				thred::strtchbox(stretchBoxLine);
				return true;
			}
			if (StateMap.test(StateFlag::STRTCH)) {
				unstrtch(stretchBoxLine);
				auto lineLength = 0l;
				if (SelectedFormControlVertex & 1) {
					lineLength = Msg.pt.x - StitchWindowOrigin.x;
				}
				else {
					lineLength = Msg.pt.y - StitchWindowOrigin.y;
				}
				const auto dst  = (SelectedFormControlVertex + 2) % 4;
				const auto code = nxtcrnr(dst);
				for (auto iSide = 0u; iSide < 4; iSide++) {
					if (iSide != dst && iSide != code) {
						if (SelectedFormControlVertex & 1) {
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
				InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
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
				(*RubberBandLine)[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
				StateMap.set(StateFlag::SHOMOV);
				thred::ritmov();
				if (thred::px2stch()) {
					thred::ritfcor(SelectedPoint);
				}
				return true;
			}
			if (StateMap.test(StateFlag::MOVCNTR)) {
				unrot();
				thred::px2stch();
				ritrot(rotationAngle, dPOINT{ SelectedPoint });
				return true;
			}
			if (StateMap.test(StateFlag::ROTCAPT)) {
				unrotu();
				unrot();
				RotateBoxToCursorLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
				const auto adjustedPoint = fPOINT{ gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
					                               gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y) };
				if (adjustedPoint.x) {
					rotationAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
				}
				else {
					if (adjustedPoint.y > 0) {
						rotationAngle = PI / 2;
					}
					else {
						rotationAngle = -PI / 2;
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
				MoveLine0[1] = MoveLine1[0] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
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
					if (PCSHeader.stitchCount) {
						xlin1();
						InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
						StateMap.set(StateFlag::ILIN1);
						ilin1();
					}
					return true;
				}
				xlin();
				InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
				StateMap.set(StateFlag::ILIN);
				ilin();
				return true;
			}
			if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT)) {
				SetCapture(ThrEdWindow);
			}
			if (StateMap.test(StateFlag::BZUMIN) && (Msg.wParam & MK_LBUTTON)) {
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
			setCursorInt(ArrowCursor);
			if (StateMap.testAndReset(StateFlag::VCAPT)) {
				ReleaseCapture();
			}
		}
		return true;
	}
	if (Msg.message == WM_LBUTTONUP) {
		if (GetKeyState(VK_SHIFT) & 0x8000 && thred::px2stch()) {
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
			const auto point = POINT{ (Msg.pt.x - dToL(FormMoveDelta.x) - StitchWindowOrigin.x) - SelectedFormsRect.left,
				                      (Msg.pt.y - dToL(FormMoveDelta.y) - StitchWindowOrigin.y) - SelectedFormsRect.top };
			form::ratsr();
			FormMoveDelta.x = point.x / HorizontalRatio;
			FormMoveDelta.y = point.y / VerticalRatio;
			if (StateMap.test(StateFlag::FPSEL)) {
				form::fvars(ClosestFormToCursor);
				auto iSelectedVertex = SelectedFormVertices.start;
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
					for (auto iForm = 0u; iForm < FormIndex; iForm++) {
						form::frmadj(iForm);
					}
					for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
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
			const auto adjustedPoint = fPOINT{ (StitchRangeRect.left + SelectBoxOffset.x) - SelectedPoint.x,
				                               (StitchRangeRect.bottom + SelectBoxOffset.y) - SelectedPoint.y };
			for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
				StitchBuffer[iStitch].x -= adjustedPoint.x;
				StitchBuffer[iStitch].y -= adjustedPoint.y;
			}
			StateMap.set(StateFlag::RESTCH);
			return true;
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
					// We potentially reserve too much memory, but the cost of reallocatioon is higher than the small
					// amount overallocated
					SelectedFormList->reserve(FormIndex);
					StateMap.reset(StateFlag::FORMSEL);
					for (auto iForm = 0u; iForm < FormIndex; iForm++) {
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
						GroupStitchIndex = ClosestPointIndex + 1;
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
			auto newSize          = fPOINT{ SelectedPoint.x - ZoomBoxOrigin.x, SelectedPoint.y - ZoomBoxOrigin.y };
			SelectedPoint.x       = ZoomBoxOrigin.x + newSize.x / 2.0f;
			SelectedPoint.y       = ZoomBoxOrigin.y + newSize.y / 2.0f;
			const auto saveFactor = ZoomFactor;
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
			ZoomRect.left = ZoomRect.bottom = 0;
			ZoomRect.right                  = newSize.x;
			ZoomRect.top                    = newSize.y;
			thred::shft(SelectedPoint);
			StateMap.reset(StateFlag::BZUMIN);
			StateMap.set(StateFlag::RESTCH);
			if (!StateMap.testAndSet(StateFlag::ZUMED)) {
				thred::movStch();
			}
			return true;

			//			StateMap.reset(StateFlag::BZUMIN);
		}
	}
	if (Msg.message == WM_RBUTTONDOWN || Msg.message == WM_LBUTTONDOWN) {
		if (FormDataSheet) {
			auto formDataRect = RECT{};
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
				auto iVersion = 0u;
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
		if (Msg.pt.x >= ColorBarRect.left && Msg.pt.x <= ColorBarRect.right && Msg.pt.y >= ColorBarRect.top
		    && Msg.pt.y <= ColorBarRect.bottom) {
			unpat();
			const auto colorBarPosition = (static_cast<double>(Msg.pt.y) - ColorBarRect.top)
			                              / (static_cast<double>(ColorBarRect.bottom) - ColorBarRect.top);
			if (Msg.message == WM_RBUTTONDOWN) {
				if (Msg.wParam & MK_SHIFT && (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::GRPSEL))) {
					unbox();
					GroupStitchIndex = dToUI(colorBarPosition * PCSHeader.stitchCount);
					StateMap.set(StateFlag::GRPSEL);
					thred::grpAdj();
					nuAct(GroupStitchIndex);
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					ClosestPointIndex = dToUI(colorBarPosition * PCSHeader.stitchCount);
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
				ClosestPointIndex = dToUI(colorBarPosition * PCSHeader.stitchCount);
				nuAct(ClosestPointIndex);
				rstAll();
				StateMap.set(StateFlag::SELBOX);
				selCol();
			}
			thred::redraw(ColorBar);
			return true;
		}
	}
	if (Msg.message == WM_RBUTTONDOWN) {
		if (StateMap.test(StateFlag::TXTRED) && !MsgWindow) {
			texture::txtrbut();
			return true;
		}
		if (GetKeyState(VK_SHIFT) & 0x8000) {
			if (form::closfrm()) {
				if ((StateMap.test(StateFlag::FRMPSEL) || StateMap.test(StateFlag::FPSEL))
				    && SelectedFormVertices.form == ClosestFormToCursor) {
					StateMap.reset(StateFlag::FRMPSEL);
					StateMap.set(StateFlag::FPSEL);
					SelectedFormVertices.finish = ClosestVertexToCursor;
					const auto selectedVertexCount
					    = (SelectedFormVertices.finish - SelectedFormVertices.start + VertexCount) % VertexCount;
					if (selectedVertexCount<VertexCount>> 1) {
						SelectedFormVertices.vertexCount = selectedVertexCount;
						StateMap.set(StateFlag::PSELDIR);
					}
					else {
						SelectedFormVertices.vertexCount = VertexCount - selectedVertexCount;
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
				if (PreferenceIndex) {
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

		if (FormMenuChoice) {
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
		if (thred::px2stch()
		    && !(StateMap.test(StateFlag::SIZSEL) && thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
			if (!(*FormList).empty() && !StateMap.test(StateFlag::FRMOF)) {
				if (Msg.wParam & MK_SHIFT) {
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
				if (Msg.wParam & MK_CONTROL) {
					if (SelectedFormList->empty() && StateMap.test(StateFlag::FORMSEL)) {
						StateMap.set(StateFlag::WASEL);
						PreviousFormIndex = ClosestFormToCursor;
					}
					if (form::closfrm()) {
						form::nufsel();
					}
					if (SelectedFormList->size() > 1) {
						const std::wstring blank;
						displayText::butxt(HNUM, blank);
					}
					return true;
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
					form::fvars(ClosestFormToCursor);
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
			if (StateMap.test(StateFlag::INIT) || FileHandle) {
				if (Msg.wParam & MK_SHIFT) {
					if (StateMap.test(StateFlag::SELBOX)) {
						const auto code = ClosestPointIndex;
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
						const auto code = ClosestPointIndex;
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
						if (PCSHeader.stitchCount) {
							rebox();
						}
					}
				}
				xt::clrstch();
				StateMap.set(StateFlag::RESTCH);
			}
			return true;
		}
		if (Msg.pt.x >= MinLenRect.left && Msg.pt.x <= MinLenRect.right && Msg.pt.y > MinLenRect.top
		    && Msg.pt.y <= MinLenRect.bottom) {
			srchk();
			setsrch(SmallestStitchIndex);
			lensadj();
			StateMap.set(StateFlag::GRPSEL);
			StateMap.set(StateFlag::RESTCH);
			return true;
		}
		if (Msg.pt.x >= MaxLenRect.left && Msg.pt.x <= MaxLenRect.right && Msg.pt.y > MaxLenRect.top
		    && Msg.pt.y <= MaxLenRect.bottom) {
			srchk();
			setsrch(LargestStitchIndex);
			lensadj();
			StateMap.set(StateFlag::GRPSEL);
			StateMap.set(StateFlag::RESTCH);
		}
		return true;
	}
	if (Msg.message == WM_LBUTTONDOWN) {
		if (GetKeyState(VK_SHIFT) & 0x8000 && thred::px2stch()) {
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
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
				xt::dufcol(VerticalIndex + 1);
				return true;
			}
		}
		if (StateMap.testAndReset(StateFlag::FSETBCOL)) {
			thred::unsid();
			thred::unmsg();
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
				xt::dubcol(VerticalIndex + 1);
				return true;
			}
		}
		if (StateMap.testAndReset(StateFlag::FPUNCLP)) {
			thred::savdo();
			fixpclp();
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
			for (auto iHoop = 0u; iHoop < HUPS; iHoop++) {
				if (Msg.hwnd == SideWindow[iHoop]) {
					switch (iHoop + 1) {
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
					UnzoomedRect = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
					form::sethup();
					chkhup();
					break;
				}
			}
			thred::unsid();
			formForms::prfmsg();
			return true;
		}
		if (StateMap.testAndReset(StateFlag::INSFIL)) {
			duinsfil();
			return true;
		}
		if (StateMap.test(StateFlag::BIGBOX) && chkbig(stretchBoxLine, xyRatio)) {
			return true;
		}
		if (StateMap.testAndReset(StateFlag::DELSFRMS)) {
			auto code = 0u;
			if (chkok()) {
				thred::savdo();
				StateMap.reset(StateFlag::DELTO);
				code = 1;
			}
			else {
				auto windowRect = RECT{};
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
		if (StateMap.testAndReset(StateFlag::FUNSCLP)) {
			thred::savdo();
			StateMap.reset(StateFlag::MOVFRMS);
			auto formsRect = fRECTANGLE{};
			form::pxrct2stch(SelectedFormsRect, formsRect);
			thred::px2stch();
			FormMoveDelta.x = SelectedPoint.x - ((formsRect.right - formsRect.left) / 2.0f + formsRect.left);
			FormMoveDelta.y = SelectedPoint.y - ((formsRect.top - formsRect.bottom) / 2.0f + formsRect.bottom);
			for (auto iForm = 0u; iForm < ClipFormsCount; iForm++) {
				ClosestFormToCursor = FormIndex + iForm;
				form::fvars(ClosestFormToCursor);
				for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
					SelectedForm->vertices[iVertex].x += FormMoveDelta.x;
					SelectedForm->vertices[iVertex].y += FormMoveDelta.y;
				}
				form::frmout(ClosestFormToCursor);
				form::refil();
			}
			FormIndex += ClipFormsCount;
			StateMap.set(StateFlag::RESTCH);
			return true;
		}
		if (!SelectedFormList->empty() && !StateMap.test(StateFlag::ROTAT) && chkbig(stretchBoxLine, xyRatio)) {
			return true;
		}
		if (StateMap.test(StateFlag::SIDCOL) && thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
			do {
				thred::savdo();
				if (StateMap.testAndReset(StateFlag::FSETFCOL)) {
					xt::dufcol(VerticalIndex + 1);
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
					SelectedForm->underlayColor = VerticalIndex & 0xf;
					form::refilfn();
					thred::coltab();
					break;
				}
				if (StateMap.testAndReset(StateFlag::FSETUCOL)) {
					xt::dundcol(VerticalIndex + 1);
					return true;
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
			return true;
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
			chknum();
			if (Msg.hwnd == (*ValueWindow)[PSQR]) {
				if (UserFlagMap.testAndFlip(UserFlag::SQRFIL)) {
					SetWindowText((*ValueWindow)[PSQR], (*StringTable)[STR_PNTD].c_str());
				}
				else {
					SetWindowText((*ValueWindow)[PSQR], (*StringTable)[STR_SQR].c_str());
				}
				return true;
			}
			if (Msg.hwnd == (*ValueWindow)[PBLNT]) {
				if (UserFlagMap.testAndFlip(UserFlag::BLUNT)) {
					SetWindowText((*ValueWindow)[PBLNT], (*StringTable)[STR_TAPR].c_str());
				}
				else {
					SetWindowText((*ValueWindow)[PBLNT], (*StringTable)[STR_BLUNT].c_str());
				}
				return true;
			}
			if (Msg.hwnd == (*ValueWindow)[PUND]) {
				if (UserFlagMap.testAndFlip(UserFlag::DUND)) {
					SetWindowText((*ValueWindow)[PUND], (*StringTable)[STR_OFF].c_str());
				}
				else {
					SetWindowText((*ValueWindow)[PUND], (*StringTable)[STR_ON].c_str());
				}
				return true;
			}
			if (Msg.hwnd == (*ValueWindow)[PHUP]) {
				sidhup();
				return true;
			}
			for (auto iPreference = 0u; iPreference < PRFLINS; iPreference++) {
				if (Msg.hwnd == (*ValueWindow)[iPreference]) {
					PreferenceIndex = iPreference + 1;
					OutputDebugString(fmt::format(L"chkmsg:PreferenceIndex '{}'\n", PreferenceIndex).c_str());
					formForms::prfsid((*ValueWindow)[iPreference]);
					break;
				}
			}
			return true;
		}
		if (!StateMap.test(StateFlag::ROTAT) && StateMap.test(StateFlag::GRPSEL)) {
			auto controlPoint = *FormControlPoints;
			if (iselpnt()) {
				for (auto iSide = 0u; iSide < 4; iSide++) {
					stretchBoxLine[iSide] = controlPoint[gsl::narrow<size_t>(iSide) << 1];
				}
				stretchBoxLine[4] = stretchBoxLine[0];
				if (SelectedFormControlVertex & 1) {
					StateMap.set(StateFlag::STRTCH);
				}
				else {
					StateMap.set(StateFlag::EXPAND);
					xyRatio = (static_cast<double>(StitchRangeRect.right) - StitchRangeRect.left)
					          / (static_cast<double>(StitchRangeRect.top) - StitchRangeRect.bottom);
				}
				SelectedFormControlVertex >>= 1;
				StateMap.set(StateFlag::SHOSTRTCH);
				thred::strtchbox(stretchBoxLine);
				return true;
			}

			StitchCoordinatesPixels.x = Msg.pt.x - StitchWindowOrigin.x;
			StitchCoordinatesPixels.y = Msg.pt.y - StitchWindowOrigin.y;
			if (StitchCoordinatesPixels.x >= controlPoint[0].x && StitchCoordinatesPixels.x <= controlPoint[2].x
			    && StitchCoordinatesPixels.y >= controlPoint[0].y && StitchCoordinatesPixels.y <= controlPoint[4].y) {
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
				}
				movbak('t', 'r');
				thrBack = 't';
				ThrName->replace_extension(thrExt);
				fs::remove(*ThrName);
				thrBack = 'r';
				ThrName->replace_extension(thrExt);
				FileHandle = CreateFile(
				    ThrName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr);
				if (FileHandle == INVALID_HANDLE_VALUE) {
					FileHandle = nullptr;
				}
				return true;
			}
			auto windowRect = RECT{};
			GetWindowRect(CancelButton, &windowRect);
			if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
			    && Msg.pt.y <= windowRect.bottom) {
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
				auto windowRect = RECT{};
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
				VerticalIndex -= 13;
				char threadSizeMap[]                = { '3', '4', '6' };
				ThreadSize[ThreadSizeSelected][0]   = threadSizeMap[VerticalIndex];
				ThreadSizeIndex[ThreadSizeSelected] = VerticalIndex;
				wchar_t buffer[3]                   = { 0 };
				wcsncpy_s(buffer, ThreadSize[ThreadSizeSelected], 2);
				buffer[2] = 0;
				SetWindowText(ThreadSizeWin[ThreadSizeSelected], buffer);
				StateMap.set(StateFlag::RESTCH);
				for (auto& iWindow : ChangeThreadSizeWin) {
					if (iWindow) {
						DestroyWindow(iWindow);
					}
				}
				return true;
			}
			for (auto& iWindow : ChangeThreadSizeWin) {
				if (iWindow) {
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
			auto windowRect = RECT{};
			GetWindowRect(MsgWindow, &windowRect);
			if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top
			    && Msg.pt.y <= windowRect.bottom) {
				const auto iFillType = (Msg.pt.y - windowRect.top - 1) / (ButtonHeight - 4);
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
				}
			}
			thred::unmsg();
			return true;
		}
		if (PreferenceIndex == PAP + 1 && thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
			AppliqueColor = VerticalIndex;
			SetWindowText((*ValueWindow)[PAP], fmt::format(L"{}", VerticalIndex).c_str());
			thred::unsid();
			return true;
		}
		if (StateMap.testAndReset(StateFlag::SIDACT)) {
			thred::savdo();
			if (FormMenuChoice == LFTHTYP) {
				for (auto iFillType = 0u; iFillType < 6; iFillType++) {
					if (Msg.hwnd == SideWindow[iFillType]) {
						SelectedForm->fillInfo.feather.fillType = gsl::narrow<unsigned char>(iFillType + 1);
						thred::unsid();
						form::refil();
						formForms::refrm();
						break;
					}
				}
				return true;
			}
			if (FormMenuChoice == LLAYR) {
				auto iLayer = 0u;
				for (iLayer = 0; iLayer < 5; iLayer++) {
					if (Msg.hwnd == SideWindow[iLayer]) {
						break;
					}
				}
				if (iLayer < 5) {
					form::movlayr(iLayer << 1);
					StateMap.set(StateFlag::FORMSEL);
				}
				thred::unsid();
				auto layerStr = fmt::format(L"{}", ((SelectedForm->attribute & FRMLMSK) >> 1));
				SetWindowText((*ValueWindow)[LLAYR], layerStr.c_str());
				return true;
			}
			SelectedForm->borderColor &= COLMSK;
			if (StateMap.testAndReset(StateFlag::BRDACT)) {
				if (clip::iseclp(ClosestFormToCursor)) {
					clip::deleclp(ClosestFormToCursor);
				}
				do {
					if (Msg.hwnd == SideWindow[0]) {
						SelectedForm->edgeType = 0;
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						break;
					}
					if (Msg.hwnd == SideWindow[1]) {
						if (SelectedForm->edgeType) {
							const auto code = SelectedForm->edgeType & NEGUND;
							if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL) {
								form::bsizpar();
							}
							SelectedForm->edgeType = EDGELINE;
							break;
						}

						form::bord();
						break;
					}
					if (Msg.hwnd == SideWindow[2]) {
						if (SelectedForm->edgeType) {
							const auto code = SelectedForm->edgeType & NEGUND;
							if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL) {
								form::bsizpar();
							}
							SelectedForm->edgeType = EDGEBEAN;
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
						if (SelectedForm->edgeType) {
							switch (SelectedForm->edgeType) {
							case EDGECLIP: {
								form::bsizpar();
							}
							case EDGELINE:
							case EDGEBEAN: {
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								break;
							}
							case EDGEPROPSAT: {
								SelectedForm->edgeSpacing /= 2;
								break;
							}
							}
							SelectedForm->edgeType = EDGEANGSAT;
							if (UserFlagMap.test(UserFlag::DUND)) {
								SelectedForm->edgeType |= EGUND;
							}
							break;
						}

						satin::satbrd();
						break;
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
								if (SelectedForm->edgeType == EDGECLIP) {
									form::bsizpar();
								}
							}
							SelectedForm->edgeType = EDGEAPPL;
							if (UserFlagMap.test(UserFlag::DUND)) {
								SelectedForm->edgeType |= EGUND;
							}
							SelectedForm->borderColor |= (AppliqueColor << 4);
							break;
						}

						form::apliq();
						break;
					}
					if (Msg.hwnd == SideWindow[6]) {
						if (SelectedForm->edgeType) {
							switch (SelectedForm->edgeType) {
							case EDGECLIP: {
								form::bsizpar();
							}
							case EDGELINE:
							case EDGEBEAN: {
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								break;
							}
							case EDGEANGSAT: {
								SelectedForm->edgeSpacing *= 2;
							}
							}
							SelectedForm->edgeType = EDGEPROPSAT;
							if (UserFlagMap.test(UserFlag::DUND)) {
								SelectedForm->edgeType |= EGUND;
							}
							break;
						}

						form::prpbrd(LineSpacing);
						break;
					}
					if (Msg.hwnd == SideWindow[7]) {
						if (SelectedForm->edgeType) {
							if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN
							    || SelectedForm->edgeType == EDGECLIP) {
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								if (SelectedForm->edgeType == EDGECLIP) {
									form::bsizpar();
								}
							}
							SelectedForm->edgeType = EDGEBHOL;
							break;
						}

						form::bhol();
						break;
					}
					if (Msg.hwnd == SideWindow[8]) {
						if (SelectedForm->edgeType) {
							if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN
							    || SelectedForm->edgeType == EDGECLIP) {
								SelectedForm->borderSize  = BorderWidth;
								SelectedForm->edgeSpacing = LineSpacing;
								if (SelectedForm->edgeType == EDGECLIP) {
									form::bsizpar();
								}
							}
							form::picot();
							break;
						}

						form::picot();
						break;
					}
					if (Msg.hwnd == SideWindow[9]) {
						if (SelectedForm->edgeType) {
							const auto code = SelectedForm->edgeType & NEGUND;
							if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL) {
								form::bsizpar();
							}
							SelectedForm->edgeType = EDGEDOUBLE;
							break;
						}

						form::dubsfil();
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

			if (SelectedForm->fillType == SAT && SelectedForm->satinGuideCount) {
				satin::delsac(ClosestFormToCursor);
			}
			if ((SelectedForm->edgeType & NEGUND) == EDGEAPPL) {
				SelectedForm->edgeType = EDGEANGSAT;
				if (UserFlagMap.test(UserFlag::DUND)) {
					SelectedForm->edgeType |= EGUND;
				}
			}
			auto textureFlag = false;
			do {
				if (Msg.hwnd == SideWindow[0]) { // none
					SelectedForm->type = FRMFPOLY;
					form::delmfil();
					SelectedForm->fillType = 0;
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
					break;
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

					form::filvrt();
					break;
				}
				if (Msg.hwnd == SideWindow[2]) { // horizontal fill
					SelectedForm->type = FRMFPOLY;
					if (SelectedForm->fillType) {
						respac();
						SelectedForm->fillType = HORF;
						break;
					}

					form::filhor();
					break;
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

					form::filangl();
					break;
				}
				if (Msg.hwnd == SideWindow[4]) { // fan fill
					SelectedForm->type = SAT;
					if (SelectedForm->fillType) {
						respac();
						SelectedForm->fillType = SATF;
						break;
					}

					StateMap.reset(StateFlag::FTHR);
					form::filsat();
					break;
				}
				if (Msg.hwnd == SideWindow[5]) { // fan clip
					SelectedForm->type = SAT;
					form::clpfil();
					break;
				}
				if (Msg.hwnd == SideWindow[6]) { // contour fill
					if (SelectedForm->fillType) {
						if (SelectedForm->fillType == CLPF) {
							SelectedForm->fillSpacing = LineSpacing;
						}
						form::chkcont();
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
						form::angsclp();
					}
					StateMap.reset(StateFlag::CLPSHO);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == SideWindow[10]) { // feather fill
					xt::fethrf();
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == SideWindow[11]) // vertical texture
				{
					if (texture::istx(ClosestFormToCursor)) {
						SelectedForm->fillType = TXVRTF;
						break;
					}
					texture::dutxtfil();
					textureFlag = true;
					break;
				}
				if (Msg.hwnd == SideWindow[12]) // horizontal texture
				{
					if (texture::istx(ClosestFormToCursor)) {
						SelectedForm->fillType = TXHORF;
						break;
					}
					texture::dutxtfil();
					textureFlag = true;
					break;
				}
				if (Msg.hwnd == SideWindow[13]) // angle texture
				{
					if (texture::istx(ClosestFormToCursor)) {
						SelectedForm->fillType              = TXANGF;
						SelectedForm->angleOrClipData.angle = IniFile.fillAngle;
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
		if (FormDataSheet) {
			chknum();
			thred::unsid();
			do {
				if (Msg.hwnd == (*ValueWindow)[LTXOF] || Msg.hwnd == (*LabelWindow)[LTXOF]) {
					FormMenuChoice = LTXOF;
					formForms::sidwnd((*ValueWindow)[LTXOF]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUSPAC] || Msg.hwnd == (*LabelWindow)[LUSPAC]) {
					FormMenuChoice = LUSPAC;
					formForms::sidwnd((*ValueWindow)[LUSPAC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUANG] || Msg.hwnd == (*LabelWindow)[LUANG]) {
					FormMenuChoice = LUANG;
					formForms::sidwnd((*ValueWindow)[LUANG]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LULEN] || Msg.hwnd == (*LabelWindow)[LULEN]) {
					FormMenuChoice = LULEN;
					formForms::sidwnd((*ValueWindow)[LULEN]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LWLKIND] || Msg.hwnd == (*LabelWindow)[LWLKIND]) {
					FormMenuChoice = LWLKIND;
					formForms::sidwnd((*ValueWindow)[LWLKIND]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHSIZ] || Msg.hwnd == (*LabelWindow)[LFTHSIZ]) {
					FormMenuChoice = LFTHSIZ;
					formForms::sidwnd((*ValueWindow)[LFTHSIZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHNUM] || Msg.hwnd == (*LabelWindow)[LFTHNUM]) {
					FormMenuChoice = LFTHNUM;
					formForms::sidwnd((*ValueWindow)[LFTHNUM]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHFLR] || Msg.hwnd == (*LabelWindow)[LFTHFLR]) {
					FormMenuChoice = LFTHFLR;
					formForms::sidwnd((*ValueWindow)[LFTHFLR]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHUPCNT] || Msg.hwnd == (*LabelWindow)[LFTHUPCNT]) {
					FormMenuChoice = LFTHUPCNT;
					formForms::sidwnd((*ValueWindow)[LFTHUPCNT]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHDWNCNT] || Msg.hwnd == (*LabelWindow)[LFTHDWNCNT]) {
					FormMenuChoice = LFTHDWNCNT;
					formForms::sidwnd((*ValueWindow)[LFTHDWNCNT]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHBLND] || Msg.hwnd == (*LabelWindow)[LFTHBLND]) {
					SelectedForm->extendedAttribute ^= AT_FTHBLND;
					formForms::refrm();
					form::refil();
					thred::unsid();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHUP] || Msg.hwnd == (*LabelWindow)[LFTHUP]) {
					SelectedForm->extendedAttribute ^= AT_FTHUP;
					form::refil();
					auto choice = (*StringTable)[STR_OFF];
					if (SelectedForm->extendedAttribute & AT_FTHUP) {
						choice = (*StringTable)[STR_ON];
					}
					SetWindowText((*ValueWindow)[LFTHUP], choice.c_str());
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHBTH] || Msg.hwnd == (*LabelWindow)[LFTHBTH]) {
					SelectedForm->extendedAttribute ^= AT_FTHBTH;
					formForms::refrm();
					form::refil();
					thred::unsid();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHTYP] || Msg.hwnd == (*LabelWindow)[LFTHTYP]) {
					FormMenuChoice = LFTHTYP;
					sidmsg((*ValueWindow)[LFTHTYP], &(*StringTable)[STR_FTH0], 6);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRM] || Msg.hwnd == (*LabelWindow)[LFRM]) {
					thred::savdo();
					form::unfil();
					if (SelectedForm->type == FRMLINE) {
						SelectedForm->type = FRMFPOLY;
					}
					else {
						SelectedForm->type = FRMLINE;
					}
					thred::coltab();
					satin::delsac(ClosestFormToCursor);
					StateMap.set(StateFlag::RESTCH);
					formForms::refrm();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LLAYR] || Msg.hwnd == (*LabelWindow)[LLAYR]) {
					std::wstring layerText[] = { L"0", L"1", L"2", L"3", L"4" };
					FormMenuChoice           = LLAYR;
					StateMap.reset(StateFlag::FILTYP);
					sidmsg((*ValueWindow)[LLAYR], layerText, 5);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMFIL] || Msg.hwnd == (*LabelWindow)[LFRMFIL]) {
					StateMap.reset(StateFlag::FILTYP);
					FormMenuChoice = LFRMFIL;
					sidmsg((*ValueWindow)[LFRMFIL], &(*StringTable)[STR_FIL0], 14);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMCOL] || Msg.hwnd == (*LabelWindow)[LFRMCOL]) {
					FormMenuChoice = LFRMCOL;
					formForms::sidwnd((*ValueWindow)[LFRMCOL]);
					StateMap.set(StateFlag::SIDCOL);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUNDCOL] || Msg.hwnd == (*LabelWindow)[LUNDCOL]) {
					FormMenuChoice = LUNDCOL;
					formForms::sidwnd((*ValueWindow)[LUNDCOL]);
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::UNDCOL);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFTHCOL] || Msg.hwnd == (*LabelWindow)[LFTHCOL]) {
					FormMenuChoice = LFTHCOL;
					formForms::sidwnd((*ValueWindow)[LFTHCOL]);
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::FTHSID);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMSPAC] || Msg.hwnd == (*LabelWindow)[LFRMSPAC]) {
					FormMenuChoice = LFRMSPAC;
					formForms::sidwnd((*ValueWindow)[LFRMSPAC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMLEN] || Msg.hwnd == (*LabelWindow)[LFRMLEN]) {
					FormMenuChoice = LFRMLEN;
					formForms::sidwnd((*ValueWindow)[LFRMLEN]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRD] || Msg.hwnd == (*LabelWindow)[LBRD]) {
					StateMap.set(StateFlag::FILTYP);
					sidmsg((*ValueWindow)[LBRD], &(*StringTable)[STR_EDG0], EDGETYPS + 1);
					StateMap.set(StateFlag::BRDACT);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDCOL] || Msg.hwnd == (*LabelWindow)[LBRDCOL]) {
					FormMenuChoice = LBRDCOL;
					formForms::sidwnd((*ValueWindow)[LBRDCOL]);
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::BRDSID);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDSPAC] || Msg.hwnd == (*LabelWindow)[LBRDSPAC]) {
					FormMenuChoice = LBRDSPAC;
					formForms::sidwnd((*ValueWindow)[LBRDSPAC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDLEN] || Msg.hwnd == (*LabelWindow)[LBRDLEN]) {
					FormMenuChoice = LBRDLEN;
					formForms::sidwnd((*ValueWindow)[LBRDLEN]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDSIZ] || Msg.hwnd == (*LabelWindow)[LBRDSIZ]) {
					FormMenuChoice = LBRDSIZ;
					formForms::sidwnd((*ValueWindow)[LBRDSIZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LAPCOL] || Msg.hwnd == (*LabelWindow)[LAPCOL]) {
					FormMenuChoice = LAPCOL;
					StateMap.set(StateFlag::SIDCOL);
					StateMap.set(StateFlag::APSID);
					formForms::sidwnd((*ValueWindow)[LAPCOL]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBCSIZ] || Msg.hwnd == (*LabelWindow)[LBCSIZ]) {
					FormMenuChoice = LBCSIZ;
					formForms::sidwnd((*ValueWindow)[LBCSIZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBSTRT] || Msg.hwnd == (*LabelWindow)[LBSTRT]) {
					const auto code = SelectedForm->attribute & SBLNT;
					if (code) {
						SelectedForm->attribute &= NSBLNT;
					}
					else {
						SelectedForm->attribute |= SBLNT;
					}
					form::refil();
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBFIN] || Msg.hwnd == (*LabelWindow)[LBFIN]) {
					const auto code = SelectedForm->attribute & FBLNT;
					if (code) {
						SelectedForm->attribute &= NFBLNT;
					}
					else {
						SelectedForm->attribute |= FBLNT;
					}
					form::refil();
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMANG] || Msg.hwnd == (*LabelWindow)[LFRMANG]) {
					FormMenuChoice = LFRMANG;
					formForms::sidwnd((*ValueWindow)[LFRMANG]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDPIC] || Msg.hwnd == (*LabelWindow)[LBRDPIC]) {
					FormMenuChoice = LBRDPIC;
					formForms::sidwnd((*ValueWindow)[LBRDPIC]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDUND] || Msg.hwnd == (*LabelWindow)[LBRDUND]) {
					SelectedForm->edgeType ^= EGUND;
					form::refil();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LSACANG] || Msg.hwnd == (*LabelWindow)[LSACANG]) {
					FormMenuChoice = LSACANG;
					formForms::sidwnd((*ValueWindow)[LSACANG]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFRMFAZ] || Msg.hwnd == (*LabelWindow)[LFRMFAZ]) {
					FormMenuChoice = LFRMFAZ;
					formForms::sidwnd((*ValueWindow)[LFRMFAZ]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBRDPOS] || Msg.hwnd == (*LabelWindow)[LBRDPOS]) {
					FormMenuChoice = LBRDPOS;
					formForms::sidwnd((*ValueWindow)[LBRDPOS]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LBFILSQR] || Msg.hwnd == (*LabelWindow)[LBFILSQR]) {
					xt::dubit(AT_SQR);
					auto choice = (*StringTable)[STR_PNTD];
					if (SelectedForm->extendedAttribute & AT_SQR) {
						choice = (*StringTable)[STR_SQR];
					}
					SetWindowText((*ValueWindow)[LBFILSQR], choice.c_str());
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFSTRT] || Msg.hwnd == (*LabelWindow)[LFSTRT]) {
					xt::dubit(AT_STRT);
					formForms::refrm();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LDSTRT] || Msg.hwnd == (*LabelWindow)[LDSTRT]) {
					FormMenuChoice = LDSTRT;
					formForms::sidwnd((*ValueWindow)[LDSTRT]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LFEND] || Msg.hwnd == (*LabelWindow)[LFEND]) {
					xt::dubit(AT_END);
					formForms::refrm();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LDEND] || Msg.hwnd == (*LabelWindow)[LDEND]) {
					FormMenuChoice = LDEND;
					formForms::sidwnd((*ValueWindow)[LDEND]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LWALK] || Msg.hwnd == (*LabelWindow)[LWALK]) {
					xt::dubit(AT_WALK);
					formForms::refrm();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LCWLK] || Msg.hwnd == (*LabelWindow)[LCWLK]) {
					xt::dubit(AT_CWLK);
					formForms::refrm();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LUND] || Msg.hwnd == (*LabelWindow)[LUND]) {
					xt::dubit(AT_UND);
					formForms::refrm();
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMAXFIL] || Msg.hwnd == (*LabelWindow)[LMAXFIL]) {
					FormMenuChoice = LMAXFIL;
					formForms::sidwnd((*ValueWindow)[LMAXFIL]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMINFIL] || Msg.hwnd == (*LabelWindow)[LMINFIL]) {
					FormMenuChoice = LMINFIL;
					formForms::sidwnd((*ValueWindow)[LMINFIL]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMAXBRD] || Msg.hwnd == (*LabelWindow)[LMAXBRD]) {
					FormMenuChoice = LMAXBRD;
					formForms::sidwnd((*ValueWindow)[LMAXBRD]);
					break;
				}
				if (Msg.hwnd == (*ValueWindow)[LMINBRD] || Msg.hwnd == (*LabelWindow)[LMINBRD]) {
					FormMenuChoice = LMINBRD;
					formForms::sidwnd((*ValueWindow)[LMINBRD]);
					break;
				}
			} while (false);
			OutputDebugString(fmt::format(L"chkmsg:FormMenuChoice '{}'\n", FormMenuChoice).c_str());

			return true;
		}
		if (StateMap.test(StateFlag::INSFRM)) {
			thred::savdo();
			form::setins();
			return true;
		}
		if (StateMap.test(StateFlag::FUNCLP)) {
			thred::savdo();
			FormIndex++;
			StateMap.set(StateFlag::INIT);
			form::rstfrm();
			form::fvars(ClosestFormToCursor);
			form::refil();
			StateMap.reset(StateFlag::FUNCLP);
			if (StateMap.testAndReset(StateFlag::FPSEL)) {
				form::frmout(FormIndex - 1);
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
		if (thred::px2stch()
		    && !(StateMap.test(StateFlag::SIZSEL) && thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
			unpat();
			if (StateMap.testAndReset(StateFlag::ROTAT)) {
				RotateBoxToCursorLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
				const auto adjustedPoint = fPOINT{ gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
					                               gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y) };
				if (hypot(adjustedPoint.x, adjustedPoint.y) < CLOSENUF) {
					thred::px2stch();
					StateMap.set(StateFlag::MOVCNTR);
					unrot();
					ritrot(rotationAngle, dPOINT{ SelectedPoint });
				}
				else {
					if (adjustedPoint.x) {
						RotationHandleAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
					}
					else {
						if (adjustedPoint.y > 0) {
							RotationHandleAngle = PI / 2;
						}
						else {
							RotationHandleAngle = -PI / 2;
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
				if (PCSHeader.stitchCount && (StateMap.testAndReset(StateFlag::SELBOX) || StateMap.testAndReset(StateFlag::INSRT))
				    && ClosestPointIndex != gsl::narrow<unsigned int>(PCSHeader.stitchCount - 1)) {
					lodclp(ClosestPointIndex);
				}
				else {
					lodclp(PCSHeader.stitchCount);
				}
				thred::rngadj();
				auto formsRect = fRECTANGLE{};
				thred::selRct(formsRect);
				auto& formControls = *FormControlPoints;
				formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = fToL(formsRect.left);
				formControls[1].x = formControls[5].x = fToL(form::midl(formsRect.right, formsRect.left));
				formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = fToL(formsRect.top);
				formControls[3].y = formControls[7].y = fToL(form::midl(formsRect.top, formsRect.bottom));
				formControls[4].y = formControls[5].y = formControls[6].y = fToL(formsRect.bottom);
				formControls[2].x = formControls[3].x = formControls[4].x = fToL(formsRect.right);
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
			if (!(*FormList).empty() && !StateMap.test(StateFlag::FRMOF)) {
				if (!StateMap.test(StateFlag::INSRT)) {
					if (StateMap.testAndReset(StateFlag::FORMSEL)) {
						form::ritfrct(ClosestFormToCursor, StitchWindowDC);
					}
					if (form::closfrm()) {
						StateMap.set(StateFlag::FRMPMOV);
						form::fvars(ClosestFormToCursor);
						form::frmovlin();
						return true;
					}
				}
			}
			if (StateMap.test(StateFlag::INIT)) {
				unlin();
				thred::savdo();
				if (StateMap.test(StateFlag::INSRT) && PCSHeader.stitchCount < MAXITEMS) {
					thred::px2stch();
					auto code = (ActiveColor | USMSK | (ActiveLayer << LAYSHFT) | NOTFRM) & NKNOTMSK;
					if (StateMap.test(StateFlag::LIN1)) {
						if (StateMap.test(StateFlag::BAKEND)) {
							xlin1();
							auto iStitch          = PCSHeader.stitchCount;
							StitchBuffer[iStitch] = { SelectedPoint.x, SelectedPoint.y, code };
							thred::duzrat();
							stch2px1(iStitch);
							InsertLine[0] = StitchCoordinatesPixels;
							InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
							PCSHeader.stitchCount++;
							thred::coltab();
							StateMap.set(StateFlag::RESTCH);
							return true;
						}

						xlin1();
						for (auto iStitch = PCSHeader.stitchCount; iStitch; iStitch--) {
							StitchBuffer[iStitch] = StitchBuffer[iStitch - 1];
						}
						StitchBuffer[0] = { SelectedPoint.x, SelectedPoint.y, code };
						StitchBuffer[0].attribute &= (~KNOTMSK);
						stch2px1(0);
						InsertLine[0] = StitchCoordinatesPixels;
						InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
						PCSHeader.stitchCount++;
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return true;
					}

					xlin();
					if (StitchBuffer[ClosestPointIndex].attribute & ALTYPMSK
					    && StitchBuffer[ClosestPointIndex + 1].attribute & ALTYPMSK) {
						if ((StitchBuffer[ClosestPointIndex].attribute & FRMSK)
						    == (StitchBuffer[ClosestPointIndex + 1].attribute & FRMSK)) {
							code = StitchBuffer[ClosestPointIndex].attribute | USMSK;
						}
					}
					auto iStitch = PCSHeader.stitchCount;
					do {
						StitchBuffer[iStitch] = StitchBuffer[iStitch - 1];
						iStitch--;
					} while (iStitch > ClosestPointIndex);
					PCSHeader.stitchCount++;
					ClosestPointIndex++;
					StitchBuffer[ClosestPointIndex] = { SelectedPoint.x, SelectedPoint.y, code };
					xlin();
					InsertLine[1] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
					stch2px1(ClosestPointIndex);
					InsertLine[0] = StitchCoordinatesPixels;
					stch2px1(ClosestPointIndex + 1);
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
					setbak(ThreadSizePixels[StitchBuffer[ClosestPointIndex].attribute & 0xf] + 3);
					auto linePoints = std::vector<POINT>(3);
					LineIndex       = 0;
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
						auto lastStitch = PCSHeader.stitchCount;
						if (lastStitch) {
							lastStitch--;
							if (ClosestPointIndex == lastStitch) {
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
					}
					MoveLine0[1] = MoveLine1[0] = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
					if (ClosestPointIndex <= 0) {
						StateMap.reset(StateFlag::ISDWN);
					}
					else {
						StateMap.set(StateFlag::ISDWN);
						stch2px1(ClosestPointIndex - 1);
						MoveLine0[0] = StitchCoordinatesPixels;
					}
					auto iStitch = PCSHeader.stitchCount;
					if (iStitch) {
						iStitch--;
					}
					if (ClosestPointIndex >= iStitch) {
						StateMap.reset(StateFlag::ISUP);
					}
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
					PCSHeader.stitchCount = 1;
					InsertLine[0]         = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
					StitchBuffer[0]       = { SelectedPoint.x, SelectedPoint.y, USMSK | ActiveColor | LayerIndex | NOTFRM };
					ColorChanges          = 1;
					ColorChangeTable[0].colorIndex  = gsl::narrow<unsigned short>(ActiveColor);
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
			return true;
		}
		if (Msg.hwnd == (*ButtonWin)[HBOXSEL]) {
			form::boxsel();
			return true;
		}
		if (Msg.hwnd == (*ButtonWin)[HUPTO]) {
			toglup();
			return true;
		}
		if (Msg.hwnd == (*ButtonWin)[HHID]) {
			toglHid();
			return true;
		}
		if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
			if (Msg.message == WM_LBUTTONDOWN) {
				thred::savdo();
				auto code   = ActiveColor;
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
					if (!SelectedFormList->empty()) {
						nucols();
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
						return true;
					}

					if (StateMap.test(StateFlag::FORMSEL)) {
						SelectedForm = &((*FormList)[ClosestFormToCursor]);
						if (SelectedForm->fillType || SelectedForm->edgeType
						    || SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
							auto& formIter = (*FormList)[ClosestFormToCursor];
							if (formIter.fillType) {
								formIter.fillColor = gsl::narrow<unsigned char>(ActiveColor);
								if (formIter.fillType == FTHF) {
									formIter.fillInfo.feather.color = gsl::narrow<unsigned char>(ActiveColor);
								}
							}
							if (formIter.edgeType) {
								if (formIter.edgeType == EDGEAPPL) {
									formIter.borderColor &= APCOLMSK;
									formIter.borderColor |= ActiveColor;
								}
								else {
									formIter.borderColor = gsl::narrow<unsigned char>(ActiveColor);
								}
							}
							if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
								formIter.underlayColor = gsl::narrow<unsigned char>(ActiveColor);
							}
							const auto formCode = ClosestFormToCursor << FRMSHFT;
							for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
								auto stitchAttribute = StitchBuffer[iStitch].attribute;
								if (stitchAttribute & ALTYPMSK && (stitchAttribute & FRMSK) == formCode
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
							for (auto iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
								StitchBuffer[iStitch].attribute &= 0xfffffff0;
								StitchBuffer[iStitch].attribute |= ActiveColor;
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
		if (thi::chkMsgs(Msg.pt, UserColorWin[0], UserColorWin[15])) {
			if (Msg.message == WM_LBUTTONDOWN && nuCol(UserColor[VerticalIndex])) {
				thred::savdo();
				UserColor[VerticalIndex]      = ColorStruct.rgbResult;
				UserPen[VerticalIndex]        = nuPen(UserPen[VerticalIndex], 1, UserColor[VerticalIndex]);
				UserColorBrush[VerticalIndex] = nuBrush(UserColorBrush[VerticalIndex], UserColor[VerticalIndex]);
				thred::redraw(UserColorWin[VerticalIndex]);
				StateMap.set(StateFlag::RESTCH);
			}
			return true;
		}
		if (thi::chkMsgs(Msg.pt, ThreadSizeWin[0], ThreadSizeWin[15])) {
			if (Msg.message == WM_LBUTTONDOWN) {
				const wchar_t* const str[] = { L"30", L"40", L"60" };

				thred::savdo();
				ThreadSizeSelected = VerticalIndex;
				for (auto iThreadSize = 0u; iThreadSize < 3; iThreadSize++) {
					ChangeThreadSizeWin[iThreadSize] = CreateWindow(L"STATIC",
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
	}
	switch (Msg.message) {
	case WM_TIMER: {
		if (StateMap.test(StateFlag::RUNPAT) && !Msg.wParam) {
			stchout();
		}
		break;
	}
	case WM_CHAR: {
		if (isgraph(gsl::narrow<int>(Msg.wParam))) {
			nuthum(gsl::narrow<char>(tolower(gsl::narrow<int>(Msg.wParam))));
		}
		break;
	}
	case WM_KEYDOWN: {
		const auto code = gsl::narrow<unsigned int>(Msg.wParam & 0xffff);
		if (StateMap.test(StateFlag::TXTRED)) {
			texture::txtkey(code, textureForm);
			return true;
		}
		form::fvars(ClosestFormToCursor);

		// ToDo - value passed to duform is wierd because it is dependant on order of enumeration of the form types.
		//        and value 'SAT' throws it off
		if (StateMap.test(StateFlag::FORMIN)) {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
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
			}
		}
		if (StateMap.test(StateFlag::FILMSG)) {
			if (code == VK_RETURN || code == 0xc0) {
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
				nuthbak(8);
				break;
			}
			case VK_HOME: {
				ThumbnailIndex = 0;
				nuthsel();
				break;
			}
			case VK_END: {
				ThumbnailIndex = gsl::narrow<unsigned int>(Thumbnails->size());
				nuthbak(4);
				break;
			}
			case 8: // backspace
			case VK_LEFT: {
				bakthum();
				break;
			}
			default: { TranslateMessage(&Msg); }
			}
			return true;
		}
		if (StateMap.test(StateFlag::FSETFSPAC) || StateMap.test(StateFlag::GTWLKIND)) {
			// Check for keycode 'dash' and numpad 'subtract'
			if (code == 189 || code == 109) {
				*MsgBuffer   = '-';
				MsgIndex     = 1;
				MsgBuffer[1] = 0;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return true;
			}
		}
		if (FormMenuChoice || PreferenceIndex) {
			if (chkminus(code)) {
				MsgIndex                 = 1;
				SideWindowEntryBuffer[0] = '-';
				SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				return true;
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
				return true;
			}
			switch (code) {
			case 0x6e:   // numpad period
			case 0xbe: { // period
				SideWindowEntryBuffer[MsgIndex++] = '.';
				SideWindowEntryBuffer[MsgIndex]   = 0;
				SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				return true;
			}
			case 8: { // backspace
				if (MsgIndex) {
					SideWindowEntryBuffer[--MsgIndex] = 0;
					SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				}
				return true;
			}
			case VK_RETURN: {
				chknum();
				FormMenuChoice  = 0;
				PreferenceIndex = 0;
				return true;
			}
			}
		}
		if (code == 'I') {
			movi();
			LastKeyCode = 'I';
			return true;
		}
		if (code == 'Q' && LastKeyCode == 'Q') {
			unpat();
		}
		LastKeyCode = code;
		if (StateMap.test(StateFlag::NUMIN)) {
			if (StateMap.test(StateFlag::SCLPSPAC) && code == 0xbd && !MsgIndex) {
				MsgBuffer[0] = '-';
				MsgBuffer[1] = 0;
				MsgIndex     = 1;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return true;
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
				return true;
			}
			switch (code) {
			case 0x6e:   // numpad period
			case 0xbe: { // period
				MsgBuffer[MsgIndex++] = '.';
				MsgBuffer[MsgIndex]   = 0;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return true;
			}
			case 8: { // backspace
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
			}
		}
		if (StateMap.testAndReset(StateFlag::ENTRDUP)) {
			if (std::wcslen(MsgBuffer) != 0) {
				const auto value = std::stod(MsgBuffer);
				if (value != 0.0) {
					IniFile.rotationAngle = value * PI / 180;
				}
			}
			form::duprot(IniFile.rotationAngle);
		}
		if (StateMap.testAndReset(StateFlag::ENTROT)) {
			if (std::wcslen(MsgBuffer) != 0) {
				const auto value = std::stod(MsgBuffer);
				if (value != 0.0) {
					IniFile.rotationAngle = value * PI / 180;
				}
			}
			rotfns(IniFile.rotationAngle);
		}
		thred::unmsg();
		switch (code) {
		case VK_ESCAPE: {
			esccode();
		}
		case 'Q': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				ritcur();
				return true;
			}
			qcode();
			return true;
		}
		case 0xc0: { //`
			if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState(VK_MENU) & 0x8000) {
				BackgroundColor    = 0x505050;
				BackgroundPen      = nuPen(BackgroundPen, 1, BackgroundColor);
				BackgroundPenWidth = 1;
				DeleteObject(BackgroundBrush);
				BackgroundBrush = CreateSolidBrush(BackgroundColor);
				if (PCSBMPFileName[0]) {
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
		case VK_INSERT: {
			inscol();
			break;
		}
		case 0x6d: { // keypad -
			toglup();
			break;
		}
		case 0xbd: { //-
			StateMap.reset(StateFlag::CNTRH);
			StateMap.reset(StateFlag::CNTRV);
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				StateMap.set(StateFlag::CNTRV);
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					StateMap.set(StateFlag::CNTRH);
				}
			}
			form::cntrx();
			break;
		}
		case 0xbf: { /// forward slash /
			formForms::frmnum();
			break;
		}
		case 0xbb: { //=
			form::shrnk();
			break;
		}
		case 0x21: { // page up
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				form::redup();
			}
			else {
				form::rotagain();
			}
			break;
		}
		case 0x22: { // page down
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				form::bakdup();
			}
			else {
				form::bakagain();
			}
			break;
		}
		case 0xde: { //'
			desiz();
			break;
		}
		case 0xdb: { //[
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				selfrm0();
			}
			else {
				ungrplo();
			}
			break;
		}
		case 0xdd: { //]
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				selfrmx();
			}
			else {
				ungrphi();
			}
			break;
		}
		case 0xba: { // semicolon
			movmrk();
			break;
		}
		case VK_DOWN: {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				nudgfn(0, -IniFile.cursorNudgeStep);
			}
			else {
				seldwn();
			}
			break;
		}
		case VK_UP: {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				nudgfn(0, IniFile.cursorNudgeStep);
			}
			else {
				selup();
			}
			break;
		}
		case 0xbe: { // period
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				xt::setfilend();
			}
			else {
				setmov();
			}
			break;
		}
		case 'W': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				form::crop();
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					xt::pes2crd();
				}
				else {
					form::insat();
				}
			}
			break;
		}
		case 'E': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				texture::dutxtfil();
			}
			else {
				form::infrm();
			}
			break;
		}
		case VK_F2: {
			form::snap();
			break;
		}
		case VK_F1: {
			hlp::help();
			break;
		}
		case VK_F3: {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				form::srtfrm();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
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
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				rembig();
			}
			else {
				duzero();
			}
			break;
		}
		case VK_F12: {
			if (GetKeyState(VK_CONTROL) & GetKeyState(VK_MENU) & 0x8000) {
				reldun();
			}
			else {
#ifdef _DEBUG
				if (GetKeyState(VK_SHIFT) & 0x8000) {
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
		case 'Y': {
			if (form::closfrm()) {
				selfpnt();
			}
			break;
		}
		case 'O': {
			fop();
			break;
		}
		case 'Z': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				ZoomFactor = ZoomMin;
				zumin();
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					bak();
				}
				else {
					zumin();
				}
			}
			break;
		}
		case 'X': {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				cut();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					thred::hidbit();
				}
				else {
					thred::zumhom();
				}
			}
			break;
		}
		case 'P': {
			formForms::prfmsg();
			break;
		}
		case 'N': {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				xt::nudsiz();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					pgdwn();
				}
				else {
					StateMap.set(StateFlag::TRCUP);
					trace::trace();
				}
			}
			break;
		}
		case 'U': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				pgup();
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					trace::trdif();
				}
				else {
					StateMap.reset(StateFlag::TRCUP);
					trace::trace();
				}
			}
			break;
		}
		case 'H': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				pglft();
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					trace::trcsel();
				}
				else {
					trace::blak();
				}
			}
			break;
		}
		case 'J': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				pgrit();
			}
			else {
				form::refilal();
			}
			break;
		}
		case 0xbc: { // comma
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				xt::setfilstrt();
			}
			else {
				thred::savdo();
				form::join();
			}
			break;
		}
		case 'B': {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				redo();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					bakmrk();
				}
				else {
					bak();
				}
			}
			break;
		}
		case 'D': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
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
		case 'K': {
			if (GetKeyState(VK_MENU) & GetKeyState(VK_CONTROL) & 0x8000) {
				setknots();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					tglhid();
				}
				else {
					if (GetKeyState(VK_CONTROL) & 0x8000) {
						set1knot();
					}
					else {
						form::tglfrm();
					}
				}
			}
			break;
		}
		case 'A': {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				selalstch();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					form::selal();
				}
				else {
					zumout();
				}
			}
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
		case 'T': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				retrac();
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					trace::trinit();
				}
				else {
					thumnail();
				}
			}
			break;
		}
		case 'R': {
			if (GetKeyState(VK_CONTROL) & GetKeyState(VK_SHIFT) & 0x8000) {
				thred::movStch();
				unbox();
				StateMap.set(StateFlag::RESTCH);
			}
			else {
				if (!FormDataSheet) {
					if (GetKeyState(VK_CONTROL) & 0x8000) {
						form::setrang();
						return true;
					}
					if (GetKeyState(VK_SHIFT) & 0x8000) {
						rotmrk();
						return true;
					}

					rotseg();
				}
			}
			break;
		}
		case VK_TAB: {
			rot(rotationCenter);
			break;
		}
		case 'S': {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				colchk();
				thred::save();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					gsnap();
				}
				else {
					zumshft();
				}
			}
			break;
		}
		case 'F': {
			if (!SelectedFormList->empty()) {
				PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
				keybd_event('F', 0, 0, 0);
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
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
		case 'V': {
			if (GetKeyState(VK_CONTROL) & 0x8000 && OpenClipboard(ThrEdWindow)) {
				ThrEdClip  = RegisterClipboardFormat(ThrEdClipFormat);
				ClipMemory = GetClipboardData(ThrEdClip);
				if (ClipMemory) {
					ClipPointer = GlobalLock(ClipMemory);
					if (ClipPointer) {
						ClipFormVerticesData = static_cast<FORMVERTEXCLIP*>(ClipPointer);
						if (ClipFormVerticesData->clipType == CLP_FRMPS) {
							thred::duzrat();
							auto byteCount
							    = sizeof(*ClipFormVerticesData)
							      + (gsl::narrow<size_t>(ClipFormVerticesData->vertexCount) + 1) * sizeof(FormVertices[0]);
							auto clipCopyBuffer = std::vector<unsigned char>(byteCount);
							auto clipPointer    = static_cast<unsigned char*>(ClipPointer);
							auto _              = std::copy(clipPointer, clipPointer + byteCount, clipCopyBuffer.begin());
							GlobalUnlock(ClipMemory);
							CloseClipboard();
							ClipFormVerticesData = convert_ptr<FORMVERTEXCLIP*>(clipCopyBuffer.data());
							if (StateMap.test(StateFlag::FRMPSEL)) {
								form::fvars(ClosestFormToCursor);
								InterleaveSequence[0] = CurrentFormVertices[ClosestVertexToCursor];
								auto clipData         = convert_ptr<fPOINT*>(&ClipFormVerticesData[1]);
								// Todo - localize iVertex into the loop
								auto iVertex = 0u;
								for (iVertex = 0; iVertex <= ClipFormVerticesData->vertexCount; iVertex++) {
									InterleaveSequence[iVertex + 1] = clipData[iVertex];
								}
								const auto nextVertex = form::nxt(ClosestVertexToCursor);
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
								FormMoveDelta.x = 0.0f;
								FormMoveDelta.y = 0.0f;
								StateMap.set(StateFlag::FUNCLP);
								(*FormList).emplace_back(FRMHED{});
								ClosestFormToCursor  = (*FormList).size() - 1;
								auto& formIter       = (*FormList).back();
								formIter.type        = FRMLINE;
								formIter.vertexCount = ClipFormVerticesData->vertexCount + 1;
								formIter.vertices    = thred::adflt(SelectedForm->vertexCount);
								form::fvars(ClosestFormToCursor);
								auto vertices = convert_ptr<fPOINT*>(&ClipFormVerticesData[1]);
								std::copy(vertices,
								          vertices + formIter.vertexCount,
								          stdext::make_checked_array_iterator(formIter.vertices, formIter.vertexCount));
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
						ClipFormsHeader = static_cast<FORMSCLIP*>(ClipPointer);
						if (ClipFormsHeader->clipType == CLP_FRMS) {
							auto iForm     = 0u;
							ClipFormsCount = ClipFormsHeader->formCount;
							auto forms     = convert_ptr<FRMHED*>(&ClipFormsHeader[1]);
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								(*FormList).emplace_back(FRMHED{});
								auto& formIter = (*FormList).back();
								formIter       = forms[iForm];
								formIter.attribute
								    = gsl::narrow<unsigned char>((formIter.attribute & NFRMLMSK) | (ActiveLayer << 1));
							}
							// ToDo - use local variable for CurrentFormVertices
							CurrentFormVertices = convert_ptr<fPOINT*>(&forms[iForm]);
							auto currentVertex  = 0u;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								SelectedForm           = &((*FormList)[FormIndex + iForm]);
								SelectedForm->vertices = thred::adflt(SelectedForm->vertexCount);
								for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
									SelectedForm->vertices[iVertex] = CurrentFormVertices[currentVertex++];
								}
							}
							auto guides       = convert_ptr<SATCON*>(&CurrentFormVertices[currentVertex]);
							auto currentGuide = 0u;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								SelectedForm = &((*FormList)[FormIndex + iForm]);
								if (SelectedForm->type == SAT && SelectedForm->satinGuideCount) {
									SelectedForm->satinOrAngle.guide = satin::adsatk(SelectedForm->satinGuideCount);
									for (auto iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
										SelectedForm->satinOrAngle.guide[iGuide] = guides[currentGuide++];
									}
								}
							}
							auto clipData    = convert_ptr<fPOINT*>(&guides[currentGuide]);
							auto currentClip = 0u;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								SelectedForm = &((*FormList)[FormIndex + iForm]);
								if (clip::isclpx(FormIndex + iForm)) {
									SelectedForm->angleOrClipData.clip = thred::adclp(SelectedForm->lengthOrCount.clipCount);
									auto offsetStart                   = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
									for (auto iClip = 0u; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {
										*offsetStart = clipData[currentClip++];
										offsetStart++;
									}
								}
								if (clip::iseclpx(FormIndex + iForm)) {
									SelectedForm->borderClipData = thred::adclp(SelectedForm->clipEntries);
									auto offsetStart             = ClipPoints->begin() + SelectedForm->borderClipData;
									for (auto iClip = 0u; iClip < SelectedForm->clipEntries; iClip++) {
										*offsetStart = clipData[currentClip++];
										offsetStart++;
									}
								}
							}
							auto textureSource = convert_ptr<TXPNT*>(&clipData[currentClip]);
							auto textureCount  = 0;
							for (iForm = 0; iForm < ClipFormsCount; iForm++) {
								if (texture::istx(FormIndex + iForm)) {
									SelectedForm = &((*FormList)[FormIndex + iForm]);
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
							for (OutputIndex = 0; OutputIndex < (ClipFormsCount); OutputIndex++) {
								form::fselrct(OutputIndex + FormIndex);
								SelectedFormList->push_back(OutputIndex + FormIndex);
							}
							SelectedFormsSize.x = gsl::narrow<float>(SelectedFormsRect.right - SelectedFormsRect.left);
							SelectedFormsSize.y = gsl::narrow<float>(SelectedFormsRect.bottom - SelectedFormsRect.top);
							StateMap.set(StateFlag::INIT);
							auto& formLines = *FormLines;
							formLines.resize(5);
							formLines[0].x = formLines[3].x = formLines[4].x = SelectedFormsRect.left;
							formLines[1].x = formLines[2].x = SelectedFormsRect.right;
							formLines[0].y = formLines[1].y = formLines[4].y = SelectedFormsRect.top;
							formLines[2].y = formLines[3].y = SelectedFormsRect.bottom;
							StateMap.set(StateFlag::SHOSTRTCH);
							thred::strtchbox(stretchBoxLine);
							FormMoveDelta.x = gsl::narrow<float>((SelectedFormsRect.right - SelectedFormsRect.left) >> 1);
							FormMoveDelta.y = gsl::narrow<float>((SelectedFormsRect.bottom - SelectedFormsRect.top) >> 1);
							StateMap.set(StateFlag::MOVFRMS);
							StateMap.set(StateFlag::FUNSCLP);
						}
						else {
							ClipFormHeader = static_cast<FORMCLIP*>(ClipPointer);
							if (ClipFormHeader->clipType == CLP_FRM) {
								FormMoveDelta.x = 0.0f;
								FormMoveDelta.y = 0.0f;
								StateMap.set(StateFlag::FUNCLP);
								(*FormList).push_back(ClipFormHeader->form);
								ClosestFormToCursor = (*FormList).size() - 1;
								form::fvars(ClosestFormToCursor);
								auto& formIter = (*FormList).back();
								formIter.attribute
								    = gsl::narrow<unsigned char>((formIter.attribute & NFRMLMSK) | (ActiveLayer << 1));
								formIter.vertices   = thred::adflt(formIter.vertexCount);
								CurrentFormVertices = convert_ptr<fPOINT*>(&ClipFormHeader[1]);
								std::copy(CurrentFormVertices,
								          CurrentFormVertices + formIter.vertexCount,
								          stdext::make_checked_array_iterator(formIter.vertices, formIter.vertexCount));
								auto guides = convert_ptr<SATCON*>(&CurrentFormVertices[formIter.vertexCount]);
								if (formIter.type == SAT && formIter.satinGuideCount) {
									formIter.satinOrAngle.guide = satin::adsatk(formIter.satinGuideCount);
									std::copy(guides,
									          guides + formIter.satinGuideCount,
									          stdext::make_checked_array_iterator(formIter.satinOrAngle.guide,
									                                              formIter.satinGuideCount));
								}
								auto clipData  = convert_ptr<fPOINT*>(&guides[0]);
								auto clipCount = 0u;
								if (clip::isclpx(FormIndex)) {
									formIter.angleOrClipData.clip = thred::adclp(formIter.lengthOrCount.clipCount);
									auto destination              = ClipPoints->begin() + formIter.angleOrClipData.clip;
									std::copy(clipData, clipData + formIter.lengthOrCount.clipCount, destination);
									clipCount += formIter.lengthOrCount.clipCount;
								}
								if (clip::iseclpx(FormIndex)) {
									clipData                = convert_ptr<fPOINT*>(&clipData[clipCount]);
									formIter.borderClipData = thred::adclp(formIter.clipEntries);
									auto destination        = ClipPoints->begin() + formIter.borderClipData;
									std::copy(clipData, clipData + formIter.clipEntries, destination);
									clipCount += formIter.clipEntries;
								}
								auto textureSource = convert_ptr<TXPNT*>(&clipData[clipCount]);
								if (texture::istx(FormIndex)) {
									formIter.fillInfo.texture.index = gsl::narrow<unsigned short>(TextureIndex);

									auto currentCount = formIter.fillInfo.texture.count;
									TexturePointsBuffer->resize(TexturePointsBuffer->size() + currentCount);
									auto iter = TexturePointsBuffer->begin() + TextureIndex;
									TextureIndex += currentCount;
									const auto _ = std::copy(textureSource, textureSource + currentCount, iter);
								}
							}
							GlobalUnlock(ClipMemory);
							StateMap.set(StateFlag::INIT);
							NewFormVertexCount = SelectedForm->vertexCount;
							if (SelectedForm->type != FRMLINE) {
								NewFormVertexCount++;
							}
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
			else {
				closPnt();
			}
			break;
		}
		case 'C': {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				duclip();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
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
		case VK_HOME: {
			if (GetKeyState(VK_SHIFT) & 0x8000 && GetKeyState(VK_CONTROL) & 0x8000) {
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
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				auto iColor = 0u;
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
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					stch2px1(0);
					endpnt();
					StateMap.reset(StateFlag::BAKEND);
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::SELBOX)) {
						auto iColor = pt2colInd(ClosestPointIndex);
						if (iColor) {
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
			break;
		}
		case VK_END: {
			if (GetKeyState(VK_SHIFT) & 0x8000 && GetKeyState(VK_CONTROL) & 0x8000) {
				if (StateMap.testAndReset(StateFlag::SELBOX)) {
					GroupStitchIndex = PCSHeader.stitchCount - 1;
					StateMap.set(StateFlag::GRPSEL);
				}
				else {
					if (StateMap.test(StateFlag::GRPSEL)) {
						if (GroupStitchIndex > ClosestPointIndex) {
							GroupStitchIndex = PCSHeader.stitchCount - 1;
						}
						else {
							ClosestPointIndex = PCSHeader.stitchCount - 1;
						}
					}
					else {
						StateMap.set(StateFlag::SELBOX);
						ClosestPointIndex = PCSHeader.stitchCount - 1;
						StateMap.set(StateFlag::RESTCH);
						return true;
					}
				}
				thred::grpAdj();
				thred::redraw(ColorBar);
				break;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				auto iColor = 0u;
				if (StateMap.testAndReset(StateFlag::SELBOX)) {
					StateMap.set(StateFlag::GRPSEL);
					iColor = pt2colInd(ClosestPointIndex);
				}
				else {
					iColor = pt2colInd(GroupStitchIndex);
				}
				GroupStitchIndex = ColorChangeTable[iColor].stitchIndex - 1;
				thred::grpAdj();
				thred::redraw(ColorBar);
			}
			else {
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					stch2px1(PCSHeader.stitchCount - 1);
					endpnt();
					StateMap.set(StateFlag::BAKEND);
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::SELBOX)) {
						const auto iColor = pt2colInd(ClosestPointIndex);
						ClosestPointIndex = ColorChangeTable[iColor].stitchIndex - 1;
					}
					else {
						if (StateMap.test(StateFlag::LENSRCH)) {
							setsrch(LargestStitchIndex);
							return true;
						}
						ClosestPointIndex = PCSHeader.stitchCount - 1;
					}
					movbox();
				}
			}
			break;
		}
		case VK_RIGHT: {
			form::fvars(ClosestFormToCursor);
			if (GetKeyState(VK_SHIFT) & 0x8000) {
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
							auto lastStitch = PCSHeader.stitchCount;
							if (lastStitch) {
								lastStitch--;
								if (ClosestPointIndex < lastStitch) {
									StateMap.set(StateFlag::GRPSEL);
									GroupStitchIndex = ClosestPointIndex + 1;
								}
							}
						}
						else {
							auto lastStitch = PCSHeader.stitchCount;
							if (lastStitch) {
								lastStitch--;
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
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					nudgfn(IniFile.cursorNudgeStep, 0);
				}
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
							const auto& vertex = (*FormList)[ClosestFormToCursor].vertices[ClosestVertexToCursor];
							thred::ritfcor(vertex);
							shftflt(vertex);
							StateMap.set(StateFlag::RESTCH);
						}
						else {
							if (StateMap.test(StateFlag::SELBOX)) {
								if (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
									ClosestPointIndex++;
								}
								movbox();
								return true;
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
		}
		case VK_LEFT: {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
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
				if (GetKeyState(VK_CONTROL) & 0x8000) {
					nudgfn(-IniFile.cursorNudgeStep, 0);
				}
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
							const auto& vertex = (*FormList)[ClosestFormToCursor].vertices[ClosestVertexToCursor];
							thred::ritfcor(vertex);
							shftflt(vertex);
							StateMap.set(StateFlag::RESTCH);
						}
						else {
							if (StateMap.test(StateFlag::SELBOX)) {
								if (ClosestPointIndex) {
									ClosestPointIndex--;
								}
								movbox();
								return true;
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
		}
		case 'G': {
			mark();
			break;
		}
		case 'M': {
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				pntmrk();
			}
			else {
				if (GetKeyState(VK_SHIFT) & 0x8000) {
					dumrk(UnzoomedRect.x / 2, UnzoomedRect.y / 2);
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
		case 'L': {
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				delstch();
			}
			else {
				form::fcntr();
			}
			break;
		}
		case VK_DELETE: {
			if (thi::chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {
				delcol();
			}
			else {
				delet();
			}
			break;
		}
		}
		break;
	}
	case WM_COMMAND: {
		thred::unmsg();
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
		}
		{
			const auto& previousNames = *PreviousNames;
			auto        iVersion      = 0u;
			for (const auto iLRU : LRUMenuId) {
				if (Msg.wParam == iLRU) {
					*WorkingFileName = previousNames[iVersion];
					StateMap.set(StateFlag::REDOLD);
					nuFil();
				}
				iVersion++;
			}
		}
		const auto wParameter = LOWORD(Msg.wParam);
		if (wParameter > 40000 && wParameter < 40300) {
			thred::undat();
		}
		switch (wParameter) {
		case ID_CHKOF: { // view / Set / Data check / Off
			xt::chgchk(0);
			break;
		}
		case ID_CHKON: { // view / Set / Data check / On
			xt::chgchk(1);
			break;
		}
		case ID_CHKREP: { // view / Set / Data Check / Auto Repair on
			xt::chgchk(2);
			break;
		}
		case ID_CHKREPMSG: { // view / Set / Data Check / Auto Repair with Message
			xt::chgchk(3);
			break;
		}
		case ID_REPAIR: { // edit / Repair Data
			repair::repar();
			break;
		}
		case ID_WARNOF: // set / Warn if edited
			xt::chgwrn();
			break;
		case ID_CLPSPAC: { // set / Clipboard Fill  Spacing
			xt::setclpspac();
			break;
		}
		case ID_FRMIND: { // edit / Form Update / Indent
			xt::setfind();
			break;
		}
		case ID_SETSIZ: { // edit / Set / Design Size
			xt::nudsiz();
			break;
		}
		case ID_TXFIL: { // Fill / Texture Editor
			texture::dutxtfil();
			break;
		}
		case ID_FRMHI: { // edit / Form Update / Height
			xt::setfhi();
			break;
		}
		case ID_FRMWID: { // edit / Form Update / Width
			xt::setfwid();
			break;
		}
		case ID_MAXFLEN: { // edit / Form Update / Fill /  Maximum Stitch Length
			xt::setfmax();
			break;
		}
		case ID_MINFLEN: { // edit / Form Update / Fill /  Minimum Stitch Length
			xt::setfmin();
			break;
		}
		case ID_MAXBLEN: { // edit / Form Update / Border /  Maximum Stitch Length
			xt::setbmax();
			break;
		}
		case ID_MINBLEN: { // edit / Form Update / Border /  Minimum Stitch Length
			xt::setbmin();
			break;
		}
		case ID_SETBSPAC: { // edit / Form Update / Border /  Spacing
			xt::setbspac();
			break;
		}
		case ID_SETBLEN: { // edit / Form Update / Border /  Stitch Length
			xt::setblen();
			break;
		}
		case ID_SETBCOL: { // edit / Form Update / Border /  Color
			xt::setbcol();
			break;
		}
		case ID_SETFCOL: { // edit / Form Update / Fill /  Color
			xt::setfcol();
			break;
		}
		case ID_SETUCOL: { // edit / Form Update / Underlay /  Color
			xt::setucol();
			break;
		}
		case ID_SETFANG: { // edit / Form Update / Fill /  Angle
			xt::setfang();
			break;
		}
		case ID_SETFSPAC: { // edit / Form Update / Fill /  Spacing
			xt::setfspac();
			break;
		}
		case ID_SETFLEN: { // edit / Form Update / Fill /  Stitch Length
			xt::setflen();
			break;
		}
		case ID_SETUANG: { // edit / Form Update / Underlay /  Angle
			xt::sfuang();
			break;
		}
		case ID_SETUSPAC: { // edit / Form Update / Underlay /  Spacing
			xt::uspac();
			break;
		}
		case ID_UNDLEN: { // edit / Form Update / Underlay / Stitch Length
			xt::undlen();
			break;
		}
		case ID_SETCWLK: { // edit / Form Update / Center Walk / On
			xt::setcwlk();
			break;
		}
		case ID_SETWLK: { // edit / Form Update / Edge Walk /  On
			xt::setwlk();
			break;
		}
		case ID_SETUND: { // edit / Form Update / Underlay / On
			xt::setund();
			break;
		}
		case ID_NOTCWLK: { // edit / Form Update / Center Walk / Off
			xt::notcwlk();
			break;
		}
		case ID_NOTWLK: { // edit / Form Update / Edge Walk / Off
			xt::notwlk();
			break;
		}
		case ID_NOTUND: { // edit / Form Update / Underlay / Off
			xt::notund();
			break;
		}
		case ID_SELUND: { // edit / Select / Form Underlay Stitches
			form::selfil(UNDMSK);
			break;
		}
		case ID_SELWLK: { // edit / Select / Form Edge Walk Stitches
			form::selfil(WLKMSK);
			break;
		}
		case ID_ALFRM: { // edit / Select / All Forms
			xt::selalfrm();
			break;
		}
		case ID_USPAC: { // view / Set / Underlay / Spacing
			xt::setuspac();
			break;
		}
		case ID_UANG: { // view / Set / Underlay / Angle
			xt::setuang();
			break;
		}
		case ID_USTCH: { // view / Set / Underlay / Stitch Length
			xt::setulen();
			break;
		}
		case ID_WIND: { // view / Set / Underlay / Indent
			xt::setwlkind();
			break;
		}
		case ID_FILSTRT: { // Edit / Set / Fill Start Point
			xt::setfilstrt();
			break;
		}
		case ID_FILEND: { // Edit / Set / Fill End Point
			xt::setfilend();
			break;
		}
		case ID_PES2CRD: { // file / PES2Card
			xt::pes2crd();
			break;
		}
		case ID_2FTHR: { // edit / Convert / to Feather Ribbon
			StateMap.set(StateFlag::CNV2FTH);
			satin::ribon();
			break;
		}
		case ID_FETHR: { // fill / Feather
			xt::fethr();
			break;
		}
		case ID_FTHDEF: { // edit / Set / Feather Defaults
			dufdef();
			break;
		}
		case ID_SRTF: { // edit / Sort / by Form
			form::srtfrm();
			break;
		}
		case ID_FILCLPX: { // fill / Border / Clipboard, Even
			form::filclpx();
			break;
		}
		case ID_FRMX: { // view / Set / Form Cursor / Cross
			frmcursel(1);
			break;
		}
		case ID_FRMBOX: { // view / Set / Form Cursor / Box
			frmcursel(0);
			break;
		}
		case ID_KNOTAT: { // edit / Set / Knot at Selected Stitch
			set1knot();
			break;
		}
		case ID_STCHPIX: { // view / Set / Point Size / Stitch Point Boxes
			getstpix();
			break;
		}
		case ID_FRMPIX: { // view / Set / Point Size / Form Point Triangles
			getfrmpix();
			break;
		}
		case ID_FRMPBOX: { // view / Set / Point Size / Form Box
			getfrmbox();
			break;
		}
		case ID_CROP: { // edit / Crop to Form
			form::crop();
			break;
		}
		case ID_BAKMRK: { // view / Retrieve Mark
			bakmrk();
			break;
		}
		case ID_MARKESC: { // view / Set / Retrieve Mark / Escape
			UserFlagMap.set(UserFlag::MARQ);
			qchk();
			break;
		}
		case ID_MARKQ: { // view / Set / Retrieve Mark / Q
			UserFlagMap.reset(UserFlag::MARQ);
			qchk();
			break;
		}
		case ID_NUDGPIX: { // view / Set / Nudge Pixels
			getnpix();
			break;
		}
		case ID_LINCHN: { // fill / Border / Line chain
			StateMap.set(StateFlag::LINCHN);
			form::chain();
			break;
		}
		case ID_OPNCHN: { // fill / Border / Open chain
			StateMap.reset(StateFlag::LINCHN);
			form::chain();
			break;
		}
		case ID_CLOSE: { // file / Close
			filclos();
			break;
		}
		case ID_DELMAP: { // file / Remove Bitmap
			delmap();
			break;
		}
		case ID_BLAK: { // edit / Trace / Reset Form Pixels
			trace::blak();
			break;
		}
		case ID_SAVMAP: { // file / Save Bitmap
			savmap();
			break;
		}
		case ID_TRDIF: { // edit / Trace / Find Edges
			trace::trdif();
			break;
		}
		case ID_TRACEDG: { // edit / Trace / Show Traced Edges
			trace::tracedg();
			break;
		}
		case ID_TRCSEL: { // edit / Trace / Select Colors
			trace::trcsel();
			break;
		}
		case ID_TRACE: { // edit / Trace / Trace Mode
			trace::trinit();
			break;
		}
		case ID_FLOK: { // file / Locking
			lock();
			break;
		}
		case ID_ROTAUXON: { // view / Set / Rotate Machine File / On
			rotauxsel(1);
			break;
		}
		case ID_ROTAUXOFF: { // view / Set / Rotate Machine File / Off
			rotauxsel(0);
			break;
		}
		case ID_FRM2COL: { // edit / Set / Form Color to  stitch Color
			form::col2frm();
			break;
		}
		case ID_SNAP2GRD: { // edit / Snap / to Grid
			gsnap();
			break;
		}
		case ID_FIL2SEL_ON: { // view / Set / Fill at Select / On
			fil2sel(1);
			break;
		}
		case ID_FIL2SEL_OFF: { // view / Set / Fill at Select / Off
			fil2sel(0);
			break;
		}
		case ID_OVRLAY: { // file / Overlay
			ovrlay();
			break;
		}
		case ID_GRDHI: { // view / Set / Grid Mask / High
			setgrd(HIGRD);
			break;
		}
		case ID_GRDMED: { // view / Set / Grid Mask / Medium
			setgrd(MEDGRD);
			break;
		}
		case ID_GRDEF: { // view / Set / Grid Mask / Default
			setgrd(DEFGRD);
			break;
		}
		case ID_GRDRED: { // view / Set / Grid Mask / UnRed
			setgrd(REDGRD);
			break;
		}
		case ID_GRDBLU: { // view / Set / Grid Mask / UnBlue
			setgrd(BLUGRD);
			break;
		}
		case ID_GRDGRN: { // view / Set / Grid Mask / UnGreen
			setgrd(GRNGRD);
			break;
		}
		case ID_RETRACE: { // edit / Retrace
			retrac();
			break;
		}
		case ID_DUBFIL: { // fill / Border / Double
			form::dubfil();
			break;
		}
		case ID_HORCLP: { // fill / Clipboard / Horizontal
			if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
				thred::savdo();
			}
			form::horclp();
			break;
		}
		case ID_ANGCLP: { // fill / Clipboard / Angle
			if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
				thred::savdo();
			}
			form::angclp();
			break;
		}
		case ID_VRTCLP: { // fill / Clipboard / Vertical
			if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
				thred::savdo();
			}
			form::vrtclp();
			break;
		}
		case ID_LINBEXACT: { // view / Set / Line Border Spacing / Exact
			UserFlagMap.reset(UserFlag::LINSPAC);
			linbmen();
			break;
		}
		case ID_LINBEVEN: { // view / Set / Line Border Spacing / Even
			UserFlagMap.set(UserFlag::LINSPAC);
			linbmen();
			break;
		}
		case ID_BSAVON: { // view / Set / PCS Bitmap Save / On
			pcsbsavon();
			break;
		}
		case ID_BSAVOF: { // view / Set / PCS Bitmap Save / Off
			pcsbsavof();
			break;
		}
		case ID_KNOTON: { // view / Knots / On
			shoknot();
			break;
		}
		case ID_KNOTOF: { // view / Knots / Off
			hidknot();
			break;
		}
		case ID_DELKNOT: { // edit / Delete / Knots
			delknot();
			break;
		}
		case ID_RSTNEDL: { // view / Set / Needle Cursor / Off
			nedof();
			break;
		}
		case ID_SETNEDL: { // view / Set / Needle Cursor / On
			nedon();
			break;
		}
		case ID_STCHS2FRM: { // edit / Convert / Stitches to Form
			form::stchs2frm();
			break;
		}
		case ID_SPLTFRM: { // edit / Split Form
			form::spltfrm();
			break;
		}
		case ID_UNBEAN: { // edit / Convert / from Bean to Line
			form::debean();
			break;
		}
		case ID_DUBEAN: { // edit / Convert / to Bean
			form::dubean();
			break;
		}
		case ID_SRTBF: { // edit / Sort / by Color then Form
			form::srtbyfrm();
			break;
		}
		case ID_CENTIRE: { // edit / Center / Entire Design
			form::centir();
			break;
		}
		case ID_CNTRX: { // edit / Center / Both
			StateMap.reset(StateFlag::CNTRH);
			StateMap.reset(StateFlag::CNTRV);
			form::cntrx();
			break;
		}
		case ID_CNTRH: { // edit / Center / Horizontal
			StateMap.reset(StateFlag::CNTRH);
			StateMap.set(StateFlag::CNTRV);
			form::cntrx();
			break;
		}
		case ID_CNTRV: { // edit / Center / Vertical
			StateMap.set(StateFlag::CNTRH);
			StateMap.reset(StateFlag::CNTRV);
			form::cntrx();
			break;
		}
		case ID_FRMNUM: { // edit / Set / Form Number
			formForms::frmnum();
			break;
		}
		case ID_REFILF: { // fill / Refill
			filfrms();
			break;
		}
		case ID_MRKPNT: { // edit / Set / Zoom Mark at Selected Point
			pntmrk();
			break;
		}
		case ID_ROTSEG: { // edit / Set / Rotation / Segments
			rotseg();
			break;
		}
		case ID_ROTMRK: { // edit / Set / Rotation / Angle from Mark
			rotmrk();
			break;
		}
		case ID_MRKCNTR: { // edit / Set / Zoom Mark at Center
			dumrk(UnzoomedRect.x / 2, UnzoomedRect.y / 2);
			StateMap.set(StateFlag::RESTCH);
			break;
		}
		case ID_SETROT: { // edit / Set / Rotation / Angle
			form::setrang();
			break;
		}
		case ID_SETPREF: { // view / Set / Default Preferences
			defpref();
			break;
		}
		case ID_SHRINK: { // edit / Shrink Clipboard Border
			form::shrnk();
			break;
		}
		case ID_DUPAGAIN: { // edit / Rotate / and Duplicate again
			form::redup();
			break;
		}
		case ID_CLPADJ: { // edit / Set / Range Ends for Clipboard Fills
			clpadj();
			break;
		}
		case ID_DELTOT: { // edit / Delete / All Forms and Stitches
			deltot();
			break;
		}
		case ID_AUXPCS: { // view / Set / Machine File Type / Pfaff PCS
			setpcs();
			break;
		}
#if PESACT
		case ID_AUXPES: { // view / Set / Machine File Type / Brother PES
			setpes();
			break;
		}
#endif
		case ID_AUXDST: { // view / Set / Machine File Type / Tajima DST
			setdst();
			break;
		}
		case ID_ABOUT: { // view / About ThrEd4
			displayText::tabmsg(IDS_CPYRIT);
			break;
		}
		case ID_RIBON: { // edit / Convert / to Satin Ribbon
			StateMap.reset(StateFlag::CNV2FTH);
			satin::ribon();
			break;
		}
		case ID_DESIZ: { // view / Design Information
			desiz();
			break;
		}
		case ID_HLP: { // help
			hlp::help();
			break;
		}
		case ID_CNTR: { // edit / Center / Forms
			form::fcntr();
			break;
		}
		case ID_INSFIL: { // file / Insert
			StateMap.reset(StateFlag::IGNORINS);
			insfil();
			break;
		}
		case ID_SELALSTCH: { // edit / Select / All Stitches
			selalstch();
			break;
		}
		case ID_UNGRPLO: { // edit / Ungroup / First
			ungrplo();
			break;
		}
		case ID_UNGRPHI: { // edit / Ungroup / Last
			ungrphi();
			break;
		}
		case ID_CONTF: { // fill / Contour
			form::contfil();
			break;
		}
		case ID_THUM: { // file / Thumbnails
			thumnail();
			break;
		}
		case ID_PICOT: { // fill / Border / Picot
			form::picot();
			break;
		}
		case ID_FILBUT: { // fill / Border / Buttonhole
			form::bhol();
			break;
		}
		case ID_REMBIG: { // edit / Delete / Large Stitches
			rembig();
			break;
		}
		case ID_VUSELTHRDS: { // view / Show Threads for Selected Color
			vuselthr();
			break;
		}
		case ID_VUTHRDS: { // view / Show Threads
			vuthrds();
			break;
		}
		case ID_MOVMRK: { // edit / Move / to Mark
			movmrk();
			break;
		}
		case ID_SELFIL: { // edit / Select / Form Fill Stitches
			form::selfil(FRMFIL);
			break;
		}
		case ID_SELBRD: { // edit / Select / Form Border Stitches
			form::selfil(FRMBFIL);
			break;
		}
		case ID_SELAP: { // edit / Select / Form Applique Stitches
			form::selfil(FRMAPFIL);
			break;
		}
		case ID_SELFSTCHS: { // edit / Select / Form Stitches
			form::selalfil();
			break;
		}
		case ID_SETMRK: { // edit / Set / Order Mark
			setmov();
			break;
		}
		case ID_DELFRE: { // edit / Delete / Free Stitches
			delfre();
			break;
		}
		case ID_SELAL: { // edit / Select / All Forms and Stitches
			form::selal();
			break;
		}
		case ID_REFILAL: { // edit / Refill All
			form::refilal();
			break;
		}
		case ID_CHK: { // edit / Check Range
			thred::chkrng(StitchRangeSize);
			StateMap.set(StateFlag::RESTCH);
			break;
		}
		case ID_RTRVCLP: { // edit / Retrieve Clipboard Stitches
			xt::rtrclp();
			break;
		}
		case ID_SORT: { // edit / Sort / Auto
			xt::fsort();
			break;
		}
		case ID_LAYMOV0: { // edit / Move to Layer / 0
			form::movlayr(0);
			break;
		}
		case ID_LAYMOV1: { // edit / Move to Layer / 1
			form::movlayr(2);
			break;
		}
		case ID_LAYMOV2: { // edit / Move to Layer / 2
			form::movlayr(4);
			break;
		}
		case ID_LAYMOV3: { // edit / Move to Layer / 3
			form::movlayr(6);
			break;
		}
		case ID_LAYMOV4: { // edit / Move to Layer / 4
			form::movlayr(8);
			break;
		}
		case ID_LAYCPY0: { // edit / Copy to Layer / 0
			form::cpylayr(0);
			break;
		}
		case ID_LAYCPY1: { // edit / Copy to Layer / 1
			form::cpylayr(2);
			break;
		}
		case ID_LAYCPY2: { // edit / Copy to Layer / 2
			form::cpylayr(4);
			break;
		}
		case ID_LAYCPY3: { // edit / Copy to Layer / 3
			form::cpylayr(6);
			break;
		}
		case ID_LAYCPY4: { // edit / Copy to Layer / 4
			form::cpylayr(8);
			break;
		}
		case ID_LA: { // all (Show all layers)
			nulayr(0);
			break;
		}
		case ID_L1: { // 1 (Show layer 1 only)
			nulayr(1);
			break;
		}
		case ID_L2: { // 2 (Show layer 2 only)
			nulayr(2);
			break;
		}
		case ID_L3: { // 3 (Show layer 3 only)
			nulayr(3);
			break;
		}
		case ID_L4: { // 4 (Show layer 4 only)
			nulayr(4);
			break;
		}
		case ID_ROTDUP: { // edit / Rotate / and Duplicate
			form::rotdup();
			break;
		}
		case ID_ROTAGAIN: { // edit / Rotate / Again
			form::rotagain();
			break;
		}
		case ID_ROTCMD: { // edit / Rotate / Command
			form::rotcmd();
			break;
		}
		case ID_DELFRMS: { // edit / Delete / All Forms
			form::delfrms();
			rstAll();
			StateMap.set(StateFlag::RESTCH);
			break;
		}
		case ID_SNAP2: { // edit / Snap / Together
			form::snap();
			break;
		}
		case ID_CLPFIL: { // fill / Clipboard / Fan
			form::clpfil();
			break;
		}
		case ID_FLPORD: { // edit / Flip / Order
			form::flpord();
			break;
		}
		case ID_FRMOF: { // frm+
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				tglhid();
			}
			else {
				form::tglfrm();
			}
			break;
		}
		case ID_MV2BAK: { // edit / move / to End
			mv2b();
			break;
		}
		case ID_MV2FRNT: { // edit / move / to Start
			mv2f();
			break;
		}
		case ID_PERP: { // fill / Border / Perpendicular Satin
			if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
				thred::savdo();
			}
			form::prpbrd(LineSpacing);
			break;
		}
		case ID_PURGDIR: { // file / Delete Backups / All backups in the selected directory
			purgdir();
			break;
		}
		case ID_PURG: { // file / Delete Backups / Backups for the selected file
			purg();
			break;
		}
		case ID_VUBAK: { // view / Backups
			vubak();
			break;
		}
		case ID_DELETE: { // edit / Delete / Selected
			delet();
			break;
		}
		case ID_FLIPH: { // edit / Flip / Horizontal
			form::fliph();
			break;
		}
		case ID_FLIPV: { // edit / Flip / Vertical
			form::flipv();
			break;
		}
		case ID_FILANG: { // fill / Angle
			if (StateMap.test(StateFlag::FORMSEL)) {
				thred::savdo();
			}
			form::filangl();
			break;
		}
		case ID_PREF: { // pref
			formForms::prfmsg();
			break;
		}
		case ID_BOLD: { // fill / Border / Bean
			if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
				thred::savdo();
			}
			form::dubold();
			break;
		}
		case ID_ADEND: { // add
			stch2px1(PCSHeader.stitchCount - 1);
			endpnt();
			StateMap.set(StateFlag::BAKEND);
			StateMap.set(StateFlag::RESTCH);
			break;
		}
		case ID_SETAP: { // view / Set / Applique Color
			form::setap();
			break;
		}
		case ID_APLIQ: { // fill / Border / Applique
			if (StateMap.test(StateFlag::FORMSEL)) {
				thred::savdo();
			}
			form::apliq();
			break;
		}
		case ID_SATBRD: { // fill / Border / Angle Satin
			if (StateMap.test(StateFlag::FORMSEL)) {
				thred::savdo();
			}
			satin::satbrd();
			break;
		}
		case ID_FILCLP: { // fill / Border / Clipboard
			form::fclp();
			break;
		}
		case ID_FILIN: { // fill / Border / Line
			if (StateMap.test(StateFlag::FORMSEL)) {
				thred::savdo();
			}
			form::bord();
			break;
		}
		case ID_FRM0: { // edit / Set / Form Zero Point
			form::frm0();
			break;
		}
		case ID_REDO: { // redo
			redo();
			break;
		}
		case ID_UNFIL: { // fill / Unfill
			thred::savdo();
			form::unfil();
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
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
			break;
		}
		case ID_FILSAT: { // fill / Fan
			StateMap.reset(StateFlag::FTHR);
			form::filsat();
			break;
		}
		case ID_OPNPCD: { // file / Open Auxiliary file
			switch (IniFile.auxFileType) {
			case AUXDST: {
				OpenFileName.nFilterIndex = 3;
				break;
			}
			default: { OpenFileName.nFilterIndex = 2; }
			}
			nuFil();
			nulayr(0);
			break;
		}
		case ID_DELSTCH: { // edit / Delete / All Stitches
			delstch();
			break;
		}
		case ID_FILL_VERT: { // fill / Vertical
			if (StateMap.test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
				thred::savdo();
			}
			form::filvrt();
			break;
		}
		case ID_FILL_HOR: { // fill / Horizontal
			if (StateMap.test(StateFlag::FORMSEL)) {
				thred::savdo();
			}
			form::filhor();
			break;
		}
		case ID_RUNPAT: { // view / Movie
			movi();
			break;
		}
		case ID_LENDEF: { // Todo - No menu item in thred.rc for this option
			IniFile.maxStitchLength = MAXSIZ * PFGRAN;
			UserStitchLength        = USESIZ * PFGRAN;
			MinStitchLength         = MINSIZ * PFGRAN;
			break;
		}
		case ID_TSIZDEF: { // view / Thread Size / Set Defaults
			ThreadSize30 = TSIZ30;
			ThreadSize40 = TSIZ40;
			ThreadSize60 = TSIZ60;
			break;
		}
		case ID_SIZ30: { // view / Thread Size / 30
			displayText::tsizmsg(L"30", ThreadSize30);
			StateMap.set(StateFlag::ENTR30);
			break;
		}
		case ID_SIZ40: { // view / Thread Size / 40
			displayText::tsizmsg(L"40", ThreadSize40);
			StateMap.set(StateFlag::ENTR40);
			break;
		}
		case ID_SIZ60: { // view / Thread Size / 60
			displayText::tsizmsg(L"60", ThreadSize60);
			StateMap.set(StateFlag::ENTR60);
			break;
		}
		case ID_HIDBITF:  // file / Hide Bitmap
		case ID_HIDBIT: { // edit / Trace / Hide Bitmap
			thred::hidbit();
			break;
		}
		case ID_LODBIT: { // file / Load Bitmap
			lodbmp();
			break;
		}
		case ID_KNOTS: { // edit / Set / Knots
			setknots();
			break;
		}
		case ID_REMZERO: { // edit / Delete / Small Stitches
			duzero();
			break;
		}
		case ID_ROT: { // rot
			rot(rotationCenter);
			break;
		}
		case ZUMIN: { // in
			if (StateMap.test(StateFlag::GMRK) || StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::INSRT)
			    || StateMap.test(StateFlag::GRPSEL) || StateMap.test(StateFlag::FORMSEL)) {
				zumin();
			}
			else {
				StateMap.reset(StateFlag::BZUM);
				StateMap.set(StateFlag::BOXZUM);
				StateMap.reset(StateFlag::BZUMIN);
				StateMap.set(StateFlag::VCAPT);
				SetCapture(ThrEdWindow);
			}
			break;
		}
		case ID_ZUMOUT: { // out
			zumout();
			break;
		}
		case ID_FILE_OPEN1: { // file / Open
			fop();
			break;
		}
		case ID_VIEW_STCHBAK: { // view / Set / Background Color
			if (nuBak()) {
				BackgroundColor    = BackgroundColorStruct.rgbResult;
				BackgroundPen      = nuPen(BackgroundPen, 1, BackgroundColor);
				BackgroundPenWidth = 1;
				DeleteObject(BackgroundBrush);
				BackgroundBrush = CreateSolidBrush(BackgroundColor);
				if (PCSBMPFileName[0]) {
					bfil();
				}
				thred::zumhom();
			}
			break;
		}
		case ID_BITCOL: { // view / Set / Bitmap Color
			if (nuBit()) {
				BitmapColor = fswap(BitMapColorStruct.rgbResult);
				if (PCSBMPFileName[0]) {
					bfil();
				}
				BitmapPen = nuPen(BitmapPen, 1, BitmapColor);
				thred::zumhom();
			}
			break;
		}
		case ID_FILE_SAVE3: { // file / Save As
			colchk();
			savAs();
			break;
		}
		case ID_EDIT_RESET_COL: { // edit / Reset Colors
			for (auto iColor = 0u; iColor < 16; iColor++) {
				UserColor[iColor]      = DefaultColors[iColor];
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
				UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
				thred::redraw(UserColorWin[iColor]);
			}
			StateMap.set(StateFlag::RESTCH);
			break;
		}
		case ID_FILE_SAVE2: { // file / Save
			colchk();
			thred::save();
			break;
		}
		case VU_ZUMFUL: { // view / Zoom Full
			thred::zumhom();
			break;
		}
		case ID_EDIT_SELECTCOLOR: { // edit / Select / Color
			selCol();
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
			break;
		}
		case ID_BACK: { // undo
			bak();
			break;
		}
		case ID_DESNAM: { // edit / Set / Designer Name
			getdes();
			break;
		}
		}
		break;
	}
	}
	return false;
}

// return the width of a text item
long thred::txtWid(const wchar_t* string) {
	GetTextExtentPoint32Int(ThredDC, string, gsl::narrow<unsigned int>(wcslen(string)), &TextSize);
	return TextSize.cx;
}

void thred::internal::makCol() noexcept {
	wchar_t buffer[3] = { 0 };

	buffer[1] = L'0';
	buffer[2] = 0;

	auto hFont = displayText::getThrEdFont(400);

	for (auto iColor = 0u; iColor < 16; iColor++) {
		DefaultColorWin[iColor] = CreateWindow(L"STATIC",
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
		displayText::setWindowFont(DefaultColorWin[iColor], hFont);
		UserColorWin[iColor]  = CreateWindow(L"STATIC",
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
		buffer[0]             = ThreadSize[iColor][0];
		ThreadSizeWin[iColor] = CreateWindow(L"STATIC",
		                                     buffer,
		                                     SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                     ButtonWidth << 1,
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
	auto       lockFilePath = fs::path{};
	auto       ppszPath     = gsl::narrow_cast<PWSTR>(nullptr); // variable to receive the path memory block pointer.
	const auto hr           = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, nullptr, &ppszPath);

	if (SUCCEEDED(hr)) {
		lockFilePath.assign(ppszPath); // make a local copy of the path
		lockFilePath /= L"ThrEd";
		fs::create_directory(lockFilePath);
		lockFilePath /= L"thredloc.txt";
		auto lockFile = CreateFile(lockFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
		if (lockFile != INVALID_HANDLE_VALUE) {
			auto bytesWritten = DWORD{ 0 };
			auto value        = utf::Utf16ToUtf8(*HomeDirectory);
			WriteFileInt(lockFile, value.data(), gsl::narrow<unsigned int>(value.size() + 1), &bytesWritten, nullptr);
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
	auto arg0 = fs::path{ ArgList[0] };

	*HomeDirectory = arg0.parent_path();
}

void thred::internal::ducmd() {
	if (ArgCount > 1) {
		auto arg1 = std::wstring{ ArgList[1] };
		if (!arg1.compare(0, 4, L"/F1:")) {
			auto balaradFileName = *HomeDirectory / arg1.substr(4);
			BalaradFile = CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
			if (BalaradFile != INVALID_HANDLE_VALUE) {
				CloseHandle(BalaradFile);
				*BalaradName0 = balaradFileName;
				if (ArgCount > 2) {
					auto arg2 = std::wstring{ ArgList[2] };
					if (!arg2.compare(0, 4, L"/F2:")) {
						balaradFileName = *HomeDirectory / arg2.substr(4);
						BalaradFile
						    = CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
						if (BalaradFile != INVALID_HANDLE_VALUE) {
							wchar_t readBuffer[_MAX_PATH + 1] = { 0 };

							*BalaradName1  = balaradFileName;
							auto bytesRead = DWORD{ 0 };
							ReadFile(BalaradFile, &readBuffer, (_MAX_PATH + 1), &bytesRead, nullptr);
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
	duhom();
	*IniFileName = *HomeDirectory;
	*IniFileName /= L"thred.ini";
	IniFileHandle = CreateFile(IniFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (IniFileHandle == INVALID_HANDLE_VALUE) {
		defpref();
		getDocsFolder(DefaultDirectory);
		if (DesignerName->empty()) {
			wchar_t designerBuffer[50];
			LoadString(ThrEdInstance, IDS_UNAM, designerBuffer, sizeof(designerBuffer) / sizeof(designerBuffer[0]));
			DesignerName->assign(designerBuffer);
			getdes();
		}
	}
	else {
		auto bytesRead = DWORD{ 0 };
		ReadFile(IniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, nullptr);
		if (bytesRead < sizeof(IniFile)) {
			IniFile.formBoxSizePixels = DEFBPIX;
		}
		auto directory = utf::Utf8ToUtf16(IniFile.defaultDirectory);
		DefaultDirectory->assign(directory);
		DefaultBMPDirectory->assign(directory);
		{
			auto& previousNames = *PreviousNames;
			auto  iVersion      = 0u;
			for (const auto& prevName : IniFile.prevNames) {
				if (strlen(prevName)) {
					previousNames[iVersion].assign(utf::Utf8ToUtf16(std::string(prevName)));
				}
				else {
					previousNames[iVersion].clear();
				}
				iVersion++;
			}
		}
		DesignerName->assign(utf::Utf8ToUtf16(std::string(IniFile.designerName)));
		for (auto iColor = 0u; iColor < 16; iColor++) {
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
		if (IniFile.threadSize30) {
			ThreadSize30 = IniFile.threadSize30;
		}
		if (IniFile.threadSize40) {
			ThreadSize40 = IniFile.threadSize40;
		}
		if (IniFile.threadSize60) {
			ThreadSize60 = IniFile.threadSize60;
		}
		if (IniFile.userStitchLength) {
			UserStitchLength = IniFile.userStitchLength;
		}
		if (!IniFile.maxStitchLength) {
			IniFile.maxStitchLength = MAXSIZ * PFGRAN;
		}
		if (IniFile.smallStitchLength) {
			SmallStitchLength = IniFile.smallStitchLength;
		}
		StitchBoxesThreshold = IniFile.stitchBoxesThreshold;
		if (IniFile.stitchSpace) {
			LineSpacing = IniFile.stitchSpace;
		}
		{
			const auto tmp = EnumMap<UserFlag>(IniFile.userFlagMap);
			UserFlagMap    = tmp;
		}
		if (IniFile.borderWidth) {
			BorderWidth = IniFile.borderWidth;
		}
		if (IniFile.appliqueColor) {
			AppliqueColor = IniFile.appliqueColor & 0xf;
		}
		if (IniFile.snapLength) {
			SnapLength = IniFile.snapLength;
		}
		if (IniFile.starRatio) {
			StarRatio = IniFile.starRatio;
		}
		if (IniFile.spiralWrap) {
			SpiralWrap = IniFile.spiralWrap;
		}
		if (IniFile.buttonholeCornerLength) {
			ButtonholeCornerLength = IniFile.buttonholeCornerLength;
		}
		if (!IniFile.gridSize) {
			IniFile.gridSize = 12;
		}
		if (!IniFile.wavePoints) {
			IniFile.wavePoints = IWAVPNTS;
		}
		if (!IniFile.waveStart) {
			IniFile.waveStart = IWAVSTRT;
		}
		if (!IniFile.waveEnd) {
			IniFile.waveEnd = IWAVEND;
		}
		if (!IniFile.waveLobes) {
			IniFile.waveLobes = IWAVS;
		}
		if (!IniFile.featherFillType) {
			IniFile.featherFillType = FDEFTYP;
		}
		if (!IniFile.featherUpCount) {
			IniFile.featherUpCount = FDEFUP;
		}
		if (!IniFile.featherDownCount) {
			IniFile.featherDownCount = FDEFDWN;
		}
		if (!IniFile.featherRatio) {
			IniFile.featherRatio = FDEFRAT;
		}
		if (!IniFile.featherMinStitchSize) {
			IniFile.featherMinStitchSize = FDEFFLR;
		}
		if (!IniFile.featherCount) {
			IniFile.featherCount = FDEFNUM;
		}
		if (!IniFile.daisyHoleDiameter) {
			IniFile.daisyHoleDiameter = DAZHLEN;
		}
		if (!IniFile.daisyPetalPoints) {
			IniFile.daisyPetalPoints = DAZCNT;
		}
		if (!IniFile.daisyInnerCount) {
			IniFile.daisyInnerCount = DAZICNT;
		}
		if (!IniFile.daisyDiameter) {
			IniFile.daisyDiameter = DAZLEN;
		}
		if (!IniFile.daisyPetalCount) {
			IniFile.daisyPetalCount = DAZPETS;
		}
		if (!IniFile.daisyPetalLen) {
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
			if (!IniFile.hoopSizeX) {
				IniFile.hoopSizeX = LHUPX;
			}
			if (!IniFile.hoopSizeY) {
				IniFile.hoopSizeY = LHUPY;
			}
			break;
		}
		case HUP100: {
			if (!IniFile.hoopSizeX) {
				IniFile.hoopSizeX = HUP100XY;
			}
			if (!IniFile.hoopSizeY) {
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
		UnzoomedRect = { fToL(IniFile.hoopSizeX), fToL(IniFile.hoopSizeY) };
		PicotSpacing = IniFile.picotSpace;
	}
	if (!IniFile.gridColor) {
		IniFile.gridColor = DEFGRD;
	}
	CloseHandle(IniFileHandle);
	if (!IniFile.fillAngle) {
		IniFile.fillAngle = PI / 6;
	}
	auto deviceContext = GetDC(nullptr);
	ScreenSizePixels   = { GetDeviceCaps(deviceContext, HORZRES), GetDeviceCaps(deviceContext, VERTRES) };
	if (IniFile.initialWindowCoords.left < 0) {
		IniFile.initialWindowCoords.left = 0;
	}
	if (IniFile.initialWindowCoords.top < 0) {
		IniFile.initialWindowCoords.top = 0;
	}
	const auto adjustedWidth = ScreenSizePixels.cx - 30;
	if (IniFile.initialWindowCoords.right > adjustedWidth) {
		IniFile.initialWindowCoords.right = adjustedWidth;
	}
	if (IniFile.initialWindowCoords.bottom > ScreenSizePixels.cy) {
		IniFile.initialWindowCoords.bottom = ScreenSizePixels.cy;
	}
}

void thred::internal::chkirct() noexcept {
	const auto screenLimits = POINT{ ScreenSizePixels.cx - 1, ScreenSizePixels.cy - 1 };
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
		IniFile.initialWindowCoords.right = screenLimits.x >> 1;
	}
	if (IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top < 300) {
		IniFile.initialWindowCoords.top    = 0;
		IniFile.initialWindowCoords.bottom = screenLimits.y >> 1;
	}
}

void thred::internal::init() {
	auto       deviceContext   = GetDC(nullptr);
	const auto screenHalfWidth = (GetDeviceCaps(deviceContext, HORZRES)) >> 1;
	auto       blank           = std::wstring{};

	ReleaseDC(nullptr, deviceContext);
	TextureIndex = 0;
	LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1));
	MainMenu   = GetMenu(ThrEdWindow);
	auto wRect = RECT{};
	GetWindowRect(ThrEdWindow, &wRect);
	auto mRect = RECT{};
	GetMenuItemRect(ThrEdWindow, MainMenu, 0, &mRect);
	wRect.left  = mRect.left;
	wRect.right = mRect.right;
	for (auto iMenu = 0u; iMenu <= M_HELP; iMenu++) {
		GetMenuItemRect(ThrEdWindow, MainMenu, iMenu, &mRect);
		wRect.right += (mRect.right - mRect.left);
	}
	wRect.right += 20;
	const auto sWidth = wRect.right - wRect.left;
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
	ArrowCursor = LoadCursor(nullptr, IDC_ARROW);
	CrossCursor = LoadCursor(nullptr, IDC_CROSS);
	crtcurs();
	redfils();
	StateMap.reset(); // clear the bitmap
	// set up the size variables
	ThredDC = GetDC(ThrEdWindow);
	SetStretchBltMode(ThredDC, COLORONCOLOR);
	StitchWindowDC    = GetDCEx(MainStitchWin, nullptr, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
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
	const auto offsetStepSize = thred::txtWid(L"0");
	for (auto iOffset = 0; iOffset < NERCNT; iOffset++) {
		BoxOffset[iOffset] = offsetStepSize * (iOffset + 1);
	}
	GetClientRect(ThrEdWindow, &ThredWindowRect);
	stchWnd();
	displayText::lodstr();
	if (!IniFileHandle) {
		// initialize the user color and thread size arrays
		for (auto iColor = 0u; iColor < 16; iColor++) {
			UserColor[iColor] = DefaultColors[iColor];
		}
	}
	for (auto& iSize : ThreadSize) {
		iSize[0] = '4';
		iSize[1] = '0';
	}
	if (!IniFile.traceLength) {
		IniFile.traceLength = TRACLEN;
	}
	if (!IniFile.traceRatio) {
		IniFile.traceRatio = TRACRAT;
	}
	if (!IniFile.chainSpace) {
		IniFile.chainSpace = CHSDEF;
	}
	if (!IniFile.chainRatio) {
		IniFile.chainRatio = CHRDEF;
	}
	if (!IniFile.cursorNudgeStep) {
		IniFile.cursorNudgeStep = NUGINI;
	}
	if (!IniFile.nudgePixels) {
		IniFile.nudgePixels = DEFPIX;
	}
	if (!IniFile.eggRatio) {
		IniFile.eggRatio = DEFEGRAT;
	}
	if (!IniFile.stitchSizePixels) {
		IniFile.stitchSizePixels = DEFPNTPIX;
	}
	if (!IniFile.formVertexSizePixels) {
		IniFile.formVertexSizePixels = DEFPNTPIX;
	}
	if (!IniFile.formSides) {
		IniFile.formSides = 24;
	}
	if (!IniFile.tearTailLength) {
		IniFile.tearTailLength = 1.4f;
	}
	if (!IniFile.underlayStitchLen)
		IniFile.underlayStitchLen = DEFULEN;
	if (!IniFile.underlaySpacing)
		IniFile.underlaySpacing = DEFUSPAC;
	setgrd(IniFile.gridColor);
	makCol(); // make the color change windows
	ButtonWin->resize(9);
	for (auto iButton = 0u; iButton < 9; iButton++) {
		auto windowFlags = 0u;
		auto buttonTxt   = &blank;
		switch (iButton) {
		case HBOXSEL: {
			windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonTxt   = &(*StringTable)[STR_BOXSEL];
			break;
		}
		case HUPTO: {
			windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonTxt   = &(*StringTable)[STR_UPOF];
			break;
		}
		case HHID: {
			windowFlags = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonTxt   = &(*StringTable)[STR_PIKOL];
			break;
		}
		default: { windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER; }
		}
		if (buttonTxt) {
			(*ButtonWin)[iButton] = CreateWindow(L"STATIC",
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
	ColorBar = CreateWindow(L"STATIC",
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

	for (auto iRGBK = 0u; iRGBK < 4; iRGBK++) {
		BoxPen[iRGBK] = CreatePenInt(PS_SOLID, 1, BoxColor[iRGBK]);
	}
	LinePen            = CreatePenInt(PS_SOLID, 1, 0x404040);
	CrossPen           = CreatePenInt(PS_SOLID, 5, 0x804080);
	GroupSelectPen     = CreatePenInt(PS_SOLID, 1, 0x804080);
	GridPen            = CreatePenInt(PS_SOLID, 1, IniFile.gridColor);
	BackgroundPen      = CreatePenInt(PS_SOLID, 3, BackgroundColor);
	BitmapPen          = CreatePenInt(PS_SOLID, 1, BitmapColor);
	FormPen            = CreatePenInt(PS_SOLID, 1, 0xc0c0c0);
	MultiFormPen       = CreatePenInt(PS_SOLID, 1, 0xc0c080);
	FormPen3px         = CreatePenInt(PS_SOLID, 3, 0xc0c0c0);
	FormSelectedPen    = CreatePenInt(PS_SOLID, 1, 0x80c080);
	ZoomMarkPen        = CreatePenInt(PS_SOLID, 3, 0x40c040);
	SelectAllPen       = CreatePenInt(PS_SOLID, 1, 0xc08080);
	KnotPen            = CreatePenInt(PS_SOLID, 1, 0xffffff);
	LayerPen[0]        = CreatePenInt(PS_SOLID, 1, 0xc0c0c0);
	LayerPen[1]        = CreatePenInt(PS_SOLID, 1, 0x80c0c0);
	LayerPen[2]        = CreatePenInt(PS_SOLID, 1, 0xc080c0);
	LayerPen[3]        = CreatePenInt(PS_SOLID, 1, 0xc0c080);
	LayerPen[4]        = CreatePenInt(PS_SOLID, 1, 0x40c0c0);
	LayerPen[5]        = CreatePenInt(PS_SOLID, 1, 0xc0c040);
	BackgroundPenWidth = 1;
	for (auto iColor = 0u; iColor < 16; iColor++) {
		ThreadSizePixels[iColor] = 1;
		ThreadSizeIndex[iColor]  = 1;
		UserPen[iColor]          = CreatePenInt(PS_SOLID, 1, UserColor[iColor]);
	}
	BackgroundBrush = CreateSolidBrush(BackgroundColor);
	// create brushes
	for (auto iColor = 0u; iColor < 16; iColor++) {
		DefaultColorBrush[iColor] = CreateSolidBrush(DefaultColors[iColor]);
		UserColorBrush[iColor]    = CreateSolidBrush(UserColor[iColor]);
	}
	ZoomFactor            = 1;
	PCSHeader.leadIn      = 0x32;
	PCSHeader.colorCount  = 16;
	PCSHeader.stitchCount = 0;
	GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
	ladj();
	GetTextExtentPoint32Int(ThredDC,
	                        (*StringTable)[STR_PIKOL].c_str(),
	                        gsl::narrow<unsigned int>((*StringTable)[STR_PIKOL].size()),
	                        &PickColorMsgSize);
	auxmen();
	fnamtabs();
	ritfnam(*DesignerName);
	auto designer = utf::Utf16ToUtf8(*DesignerName);
	std::copy(designer.begin(), designer.end(), ExtendedHeader.modifierName);
	ExtendedHeader.stgran = 0;
	for (auto& reservedChar : ExtendedHeader.res) {
		reservedChar = 0;
	}
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
	// bitmap for color number. Black or white bit chosen for contrast against the default background colors
	const auto textColorMap = std::bitset<16>(0xbaf);
	const auto white        = 0xffffffu;
	const auto black        = 0u;
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
	const auto anchorStitch = StitchBuffer[MoveAnchor];

	RotateAngle = atan2(StitchBuffer[MoveAnchor + 1].y - StitchBuffer[MoveAnchor].y,
	                    StitchBuffer[MoveAnchor + 1].x - StitchBuffer[MoveAnchor].x);
	if (anchorStitch.x >= ZoomRect.left && anchorStitch.x <= ZoomRect.right && anchorStitch.y >= ZoomRect.bottom
	    && anchorStitch.y <= ZoomRect.top) {
		auto  rotationCenterPixels = POINT{};
		POINT rotationOutline[8]   = {};
		sdCor2px(StitchBuffer[MoveAnchor], rotationCenterPixels);
		rotationOutline[0]    = rotationCenterPixels;
		rotationOutline[6]    = rotationCenterPixels;
		auto OffsetFromCenter = POINT{ rotationCenterPixels.x + 12, rotationCenterPixels.y + 2 };
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
			if (ColorChangeTable[iStitch + 1].stitchIndex < ClosestPointIndex) {
				return count;
			}
			return ClosestPointIndex - ColorChangeTable[iStitch].stitchIndex + 1;
		}

		return 0;
	}

	return count;
}

bool thred::internal::bitar() {
	const auto zoomedInRect
	    = dRECTANGLE{ (UnzoomedRect.y - ZoomRect.top), (UnzoomedRect.y - ZoomRect.bottom), ZoomRect.left, ZoomRect.right };

	if (zoomedInRect.top > BitmapSizeinStitches.y || zoomedInRect.left > BitmapSizeinStitches.x) {
		return false;
	}
	BitmapSrcRect = { gsl::narrow<LONG>(std::ceil(ZoomRect.left * BmpStitchRatio.x)),
		              gsl::narrow<LONG>(std::ceil(zoomedInRect.top * BmpStitchRatio.y)),
		              gsl::narrow<LONG>(std::floor(ZoomRect.right * BmpStitchRatio.x)),
		              gsl::narrow<LONG>(std::floor(zoomedInRect.bottom * BmpStitchRatio.y)) };
	if (BitmapSrcRect.right > gsl::narrow<long>(BitmapWidth)) {
		BitmapSrcRect.right = BitmapWidth;
		StateMap.reset(StateFlag::LANDSCAP);
	}
	if (BitmapSrcRect.bottom > gsl::narrow<long>(BitmapHeight)) {
		BitmapSrcRect.bottom = BitmapHeight;
		StateMap.set(StateFlag::LANDSCAP);
	}
	const auto backingRect = dRECTANGLE{ BitmapSrcRect.top * StitchBmpRatio.y,
		                                 BitmapSrcRect.bottom * StitchBmpRatio.y,
		                                 BitmapSrcRect.left * StitchBmpRatio.x,
		                                 BitmapSrcRect.right * StitchBmpRatio.x };

	const auto differenceRect = dRECTANGLE{ backingRect.top - zoomedInRect.top,
		                                    zoomedInRect.bottom - backingRect.bottom,
		                                    backingRect.left - zoomedInRect.left,
		                                    zoomedInRect.right - backingRect.right };

	const auto bitmapStitchRatio
	    = dPOINT{ static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left),
		          static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom) };

	BitmapDstRect = { dToL(differenceRect.left * bitmapStitchRatio.x),
		              dToL(differenceRect.top * bitmapStitchRatio.y),
		              dToL(StitchWindowClientRect.right - differenceRect.right * bitmapStitchRatio.x),
		              dToL(StitchWindowClientRect.bottom - differenceRect.bottom * bitmapStitchRatio.y) };
	return true;
}

inline void thred::internal::stCor2px(const fPOINTATTR& stitch, POINT& point) {
	// ToDo - is 0.5 still required with dToL?
	point = { dToL((stitch.x - ZoomRect.left) * ZoomRatio.x + 0.5),
		      dToL(StitchWindowClientRect.bottom - (stitch.y - ZoomRect.bottom) * ZoomRatio.y + 0.5) };
}

void thred::internal::drwknot() {
	constexpr auto KnotBoxSize  = 5;  // offset of the knot box sides;
	constexpr auto KnotLineSize = 10; // length of the knot line;

	if (!UserFlagMap.test(UserFlag::KNOTOF) && KnotCount && PCSHeader.stitchCount) {
		const auto kOffset = gsl::narrow<LONG>(MulDiv(KnotBoxSize, *screenDPI, 96));
		const auto kLine   = gsl::narrow<LONG>(MulDiv(KnotLineSize, *screenDPI, 96));

		auto  point   = POINT{};
		POINT tlin[5] = {};

		for (auto ind = 0u; ind < KnotCount; ind++) {
			stCor2px(StitchBuffer[Knots[ind]], point);
			SelectObject(StitchWindowMemDC, KnotPen);
			SetROP2(StitchWindowMemDC, R2_XORPEN);
			tlin[0].x = tlin[3].x = tlin[4].x = point.x - kOffset;
			tlin[1].x = tlin[2].x = point.x + kOffset;
			tlin[0].y = tlin[1].y = tlin[4].y = point.y + kOffset;
			tlin[2].y = tlin[3].y = point.y - kOffset;
			Polyline(StitchWindowMemDC, tlin, 5);
			tlin[0].x = point.x - kLine;
			tlin[1].x = point.x + kLine;
			tlin[0].y = tlin[1].y = point.y;
			Polyline(StitchWindowMemDC, tlin, 2);
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		}
	}
}

void thred::internal::dugrid() {
	if (ZoomFactor < ShowStitchThreshold || !ShowStitchThreshold) {
		SetROP2(StitchWindowMemDC, R2_XORPEN);
		SelectObject(StitchWindowMemDC, GridPen);
		const auto gridRect    = RECT{ gsl::narrow<LONG>(std::ceil(ZoomRect.left / IniFile.gridSize)),
                                    gsl::narrow<LONG>(std::floor(ZoomRect.top / IniFile.gridSize)),
                                    gsl::narrow<LONG>(std::floor(ZoomRect.right / IniFile.gridSize)),
                                    gsl::narrow<LONG>(std::ceil(ZoomRect.bottom / IniFile.gridSize)) };
		POINT      gridLine[2] = {};
		gridLine[0].x          = 0;
		gridLine[1].x          = StitchWindowClientRect.right;
		for (auto iGrid = gridRect.bottom; iGrid <= gridRect.top; iGrid++) {
			gridLine[0].y = gridLine[1].y
			    = dToL(StitchWindowClientRect.bottom
			           - (static_cast<double>(iGrid) * IniFile.gridSize - ZoomRect.bottom) * ZoomRatio.y + 0.5);
			Polyline(StitchWindowMemDC, gridLine, 2);
		}
		gridLine[0].y = 0;
		gridLine[1].y = StitchWindowClientRect.bottom;
		for (auto iGrid = gridRect.left; iGrid <= gridRect.right; iGrid++) {
			gridLine[0].x = gridLine[1].x
			    = dToL((static_cast<double>(iGrid) * IniFile.gridSize - ZoomRect.left) * ZoomRatio.x + 0.5);
			Polyline(StitchWindowMemDC, gridLine, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void thred::internal::rint() noexcept {
	CellSize.x = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
	CellSize.y = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
}

bool thred::internal::setRmap(boost::dynamic_bitset<>& stitchMap, const fPOINTATTR& stitchPoint) {
	auto bitPoint = gsl::narrow<unsigned int>(floor((stitchPoint.x - ZoomRect.left) / CellSize.x)
	                                          * floor((stitchPoint.y - ZoomRect.bottom) / CellSize.y));
	return !stitchMap.test_set(bitPoint);
}

void thred::internal::drwStch() {
	auto stitchCount = 0u;

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
	for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
		LineIndex = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
		if (LineIndex > stitchCount) {
			stitchCount = LineIndex;
		}
	}
	auto linePoints = std::vector<POINT>(gsl::narrow<size_t>(stitchCount) + 2);
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	thred::duzrat();
	if (PCSBMPFileName[0] && !StateMap.test(StateFlag::HIDMAP) && !StateMap.test(StateFlag::UPTO)) {
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
	if (StateMap.test(StateFlag::INIT)) {
		if (StateMap.test(StateFlag::ZUMED)) {
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask  = SIF_ALL;
			ScrollInfo.nMax   = UnzoomedRect.y;
			ScrollInfo.nMin   = 0;
			ScrollInfo.nPage  = dToL(ZoomRect.top - ZoomRect.bottom);
			ScrollInfo.nPos   = gsl::narrow<decltype(ScrollInfo.nPos)>(std::round(UnzoomedRect.y - ZoomRect.top));
			SetScrollInfo(VerticalScrollBar, SB_CTL, &ScrollInfo, TRUE);
			ScrollInfo.nMax  = UnzoomedRect.x;
			ScrollInfo.nPage = dToL(ZoomRect.right - ZoomRect.left);
			ScrollInfo.nPos  = gsl::narrow<decltype(ScrollInfo.nPos)>(std::round(ZoomRect.left));
			SetScrollInfo(HorizontalScrollBar, SB_CTL, &ScrollInfo, TRUE);
			ShowWindow(HorizontalScrollBar, TRUE);
			ShowWindow(VerticalScrollBar, TRUE);
		}
		else {
			ShowWindow(VerticalScrollBar, FALSE);
			ShowWindow(HorizontalScrollBar, FALSE);
		}
		thred::duzrat();
		const auto dub6      = ZoomRatio.x * 6.0;
		ThreadWidthPixels[0] = gsl::narrow<int>(std::round(dub6 * TSIZ30));
		ThreadWidthPixels[1] = gsl::narrow<int>(std::round(dub6 * TSIZ40));
		ThreadWidthPixels[2] = gsl::narrow<int>(std::round(dub6 * TSIZ60));
		for (auto iColor = 0u; iColor < 16; iColor++) {
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
			auto wascol = 0u;
			for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
				if (StateMap.test(StateFlag::HID)) {
					if (ColorChangeTable[iColor].colorIndex != ActiveColor) {
						stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
						const auto* currentStitches = &StitchBuffer[ColorChangeTable[iColor].stitchIndex];
						if (currentStitches) {
							for (auto iStitch = 0u; iStitch < stitchCount; iStitch++) {
								if (currentStitches[iStitch].x >= ZoomRect.left && currentStitches[iStitch].x <= ZoomRect.right
								    && currentStitches[iStitch].y >= ZoomRect.bottom
								    && currentStitches[iStitch].y <= ZoomRect.top) {
									DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
									break;
								}
							}
							break;
						}

						throw;
					}

					wascol = 0;
				}
				else {
					wascol = 0;
				}
				SelectObject(StitchWindowMemDC, UserPen[ColorChangeTable[iColor].colorIndex]);
				stitchCount                 = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
				const auto* currentStitches = &StitchBuffer[ColorChangeTable[iColor].stitchIndex];
				if (currentStitches) {
					stitchCount          = chkup(stitchCount, iColor);
					const auto maxYcoord = DrawItem->rcItem.bottom;

					for (auto iStitch = 0u; iStitch < stitchCount; iStitch++) {
						const auto layer = (currentStitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
						if (!ActiveLayer || !layer || (layer == ActiveLayer)) {
							if (currentStitches[iStitch].x >= ZoomRect.left && currentStitches[iStitch].x <= ZoomRect.right
							    && currentStitches[iStitch].y >= ZoomRect.bottom && currentStitches[iStitch].y <= ZoomRect.top) {
								wascol = 1;
								if (StateMap.testAndSet(StateFlag::LINED)) {
									if (StateMap.testAndSet(StateFlag::LININ)) {
										linePoints[LineIndex++] = {
											dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
											dToL(maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5)
										};
									}
									else {
										if (iStitch == 0) {
											linePoints[LineIndex++]
											    = { dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
												    dToL(maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y
												         + 0.5) };
										}
										else {
											linePoints[LineIndex++]
											    = { dToL((currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5),
												    dToL(maxYcoord
												         - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y
												         + 0.5) };
											linePoints[LineIndex++]
											    = { dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
												    dToL(maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y
												         + 0.5) };
										}
									}
								}
								else {
									if (iStitch == 0) {
										linePoints[0] = {
											dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
											dToL(maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5)
										};
										LineIndex = 1;
									}
									else {
										linePoints[0]
										    = { dToL((currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5),
											    dToL(maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y
											         + 0.5) };
										linePoints[1] = {
											dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
											dToL(maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5)
										};
										LineIndex = 2;
									}
									StateMap.set(StateFlag::LININ);
								}
							}
							else {
								if (StateMap.testAndReset(StateFlag::LININ)) {
									linePoints[LineIndex++] = {
										dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
										dToL(maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5)
									};
									PolylineInt(StitchWindowMemDC, linePoints.data(), LineIndex);
									LineIndex = 0;
								}
								else {
									if (iStitch) {
										// write an equation for this line
										const auto xDelta = currentStitches[iStitch].x - currentStitches[iStitch - 1].x;
										const auto yDelta = currentStitches[iStitch - 1].y - currentStitches[iStitch].y;
										const auto slope  = static_cast<double>(xDelta) / yDelta;
										const auto offset = currentStitches[iStitch].x + slope * currentStitches[iStitch].y;
										POINT      stitchLine[2] = {};
										do {
											// does the line intersect with the top of the screen?
											auto gapToEdge = offset - slope * ZoomRect.top;
											if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
												stitchLine[0] = {
													dToL((currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5),
													dToL(maxYcoord
													     - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.x + 0.5)
												};
												stitchLine[1]
												    = { dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
													    dToL(maxYcoord
													         - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.x
													         + 0.5) };
												Polyline(StitchWindowMemDC, stitchLine, 2);
												break;
											}
											// does the line intersect the bottom of the screen?
											gapToEdge = offset - slope * ZoomRect.bottom;
											if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
												stitchLine[0] = {
													dToL((currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x + 0.5),
													dToL(maxYcoord
													     - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y + 0.5)
												};
												stitchLine[1]
												    = { dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
													    dToL(maxYcoord
													         - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y
													         + 0.5) };
												Polyline(StitchWindowMemDC, stitchLine, 2);
												break;
											}
											// does the line intersect the left side of the screen?
											if (slope) {
												gapToEdge = (offset - ZoomRect.left) / slope;
												if (gapToEdge >= ZoomRect.bottom && gapToEdge <= ZoomRect.top) {
													stitchLine[0] = {
														dToL((currentStitches[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x
														     + 0.5),
														dToL(maxYcoord
														     - (currentStitches[iStitch - 1].y - ZoomRect.bottom) * ZoomRatio.y
														     + 0.5)
													};
													stitchLine[1] = {
														dToL((currentStitches[iStitch].x - ZoomRect.left) * ZoomRatio.x + 0.5),
														dToL(maxYcoord
														     - (currentStitches[iStitch].y - ZoomRect.bottom) * ZoomRatio.y + 0.5)
													};
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
				if (wascol) {
					DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
				}
			}
		}
		else {
			const auto pwid = StateMap.test(StateFlag::HID);
			for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
				DisplayedColorBitmap.set(ColorChangeTable[iColor].colorIndex);
				stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
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
			ritcor(StitchBuffer[ClosestPointIndex]);
			if (stch2px(ClosestPointIndex)) {
				dubox();
			}
		}
		if (StateMap.test(StateFlag::FRMPSEL)) {
			thred::ritfcor((*FormList)[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
		}
		if (!StateMap.test(StateFlag::SELBOX) && !StateMap.test(StateFlag::FRMPSEL)) {
			const auto blank = std::wstring{};
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

			auto stitchMap = boost::dynamic_bitset<>(maxMapSize, false);
			SelectObject(StitchWindowMemDC, LinePen);
			SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
			if (StateMap.test(StateFlag::HID)) {
				for (auto iColor = 0u; iColor < ColorChanges; iColor++) {
					if (ColorChangeTable[iColor].colorIndex == ActiveColor) {
						for (auto iStitch = ColorChangeTable[iColor].stitchIndex;
						     iStitch < ColorChangeTable[iColor + 1].stitchIndex;
						     iStitch++) {
							if (StitchBuffer[iStitch].x >= ZoomRect.left && StitchBuffer[iStitch].x <= ZoomRect.right
							    && StitchBuffer[iStitch].y >= ZoomRect.bottom && StitchBuffer[iStitch].y <= ZoomRect.top
							    && setRmap(stitchMap, StitchBuffer[iStitch])) {
								stchbox(iStitch, StitchWindowMemDC);
							}
						}
					}
				}
			}
			else {
				for (auto iColor = 0; iColor < PCSHeader.stitchCount; iColor++) {
					if (StitchBuffer[iColor].x >= ZoomRect.left && StitchBuffer[iColor].x <= ZoomRect.right
					    && StitchBuffer[iColor].y >= ZoomRect.bottom && StitchBuffer[iColor].y <= ZoomRect.top
					    && setRmap(stitchMap, StitchBuffer[iColor])) {
						stchbox(iColor, StitchWindowMemDC);
					}
				}
			}
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		}
		if (StateMap.test(StateFlag::CLPSHO)) {
			duclp();
		}
	}
	if (!(*FormList).empty() && !StateMap.test(StateFlag::FRMOF)) {
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
	thred::ritot(PCSHeader.stitchCount);
	if (StateMap.test(StateFlag::INIT)) {
		lenCalc();
	}
	if (StateMap.test(StateFlag::MOVSET)) {
		dumov();
	}
	drwknot();
}

void thred::internal::dubar() {
	auto  colorBarRect     = RECT{ DrawItem->rcItem.left, 0, DrawItem->rcItem.right, DrawItem->rcItem.bottom };
	POINT indicatorLine[2] = {};

	for (auto iColorChange = 0u; iColorChange < ColorChanges; iColorChange++) {
		const auto barSectionHeight = static_cast<double>(ColorChangeTable[iColorChange + 1].stitchIndex) / PCSHeader.stitchCount;
		colorBarRect.bottom         = dToL(barSectionHeight * DrawItem->rcItem.bottom);
		FillRect(DrawItem->hDC, &colorBarRect, UserColorBrush[ColorChangeTable[iColorChange].colorIndex]);
		colorBarRect.top = colorBarRect.bottom;
	}
	if (StateMap.test(StateFlag::SELBOX) || StateMap.test(StateFlag::GRPSEL)) {
		auto selectedIndicator = static_cast<double>(ClosestPointIndex) / PCSHeader.stitchCount;
		indicatorLine[0].y = indicatorLine[1].y = dToL(colorBarRect.bottom * selectedIndicator + 0.5);
		indicatorLine[0].x                      = colorBarRect.left;
		indicatorLine[1].x                      = colorBarRect.right;
		SelectObject(DrawItem->hDC, CrossPen);
		SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
		Polyline(DrawItem->hDC, indicatorLine, 2);
		if (StateMap.test(StateFlag::GRPSEL)) {
			selectedIndicator  = static_cast<double>(GroupStitchIndex) / PCSHeader.stitchCount;
			indicatorLine[0].y = indicatorLine[1].y = dToL(colorBarRect.bottom * selectedIndicator + 0.5);
			indicatorLine[0].x                      = colorBarRect.left;
			indicatorLine[1].x                      = colorBarRect.right;
			Polyline(DrawItem->hDC, indicatorLine, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void thred::internal::ritbak(const fs::path& fileName, DRAWITEMSTRUCT* drawItem) {
	auto thrEdFile = CreateFile(fileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (thrEdFile != INVALID_HANDLE_VALUE) {
		auto stitchHeader = STRHED{};
		ReadFile(thrEdFile, &stitchHeader, sizeof(stitchHeader), &BytesRead, nullptr);
		if (BytesRead == sizeof(stitchHeader)) {
			auto       stitchSourceSize = fPOINT{ 1.0f, 1.0f };
			const auto fileTypeVersion  = (stitchHeader.headerType & 0xff000000) >> 24;
			if ((stitchHeader.headerType & 0xffffff) == 0x746872) {
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
					auto extendedHeader = STREX{};
					ReadFile(thrEdFile, &extendedHeader, sizeof(extendedHeader), &BytesRead, nullptr);
					if (BytesRead != sizeof(extendedHeader)) {
						return;
					}
					stitchSourceSize.x = extendedHeader.hoopSizeX;
					stitchSourceSize.y = extendedHeader.hoopSizeY;
					break;
				}
				default: { return; }
				}
			}
			const auto drawingDestinationSize
			    = POINT{ (drawItem->rcItem.right - drawItem->rcItem.left), (drawItem->rcItem.bottom - drawItem->rcItem.top) };
			const auto xRatio = static_cast<double>(drawingDestinationSize.x) / stitchSourceSize.x;
			const auto yRatio = static_cast<double>(drawingDestinationSize.y) / stitchSourceSize.y;
			auto       ratio  = 0.0;
			if (xRatio < yRatio) {
				ratio = xRatio;
			}
			else {
				ratio = yRatio;
			}
			if (stitchHeader.stitchCount) {
				auto       stitchesToDraw = std::vector<fPOINTATTR>(stitchHeader.stitchCount);
				auto       lines          = std::vector<POINT>(stitchHeader.stitchCount);
				const auto bytesToRead    = gsl::narrow_cast<DWORD>(stitchHeader.stitchCount * sizeof(StitchBuffer[0]));
				ReadFile(thrEdFile, stitchesToDraw.data(), bytesToRead, &BytesRead, nullptr);
				if (bytesToRead == BytesRead) {
					SetFilePointer(thrEdFile, 16, nullptr, FILE_CURRENT);
					auto brushColor = COLORREF{};
					ReadFile(thrEdFile, &brushColor, sizeof(brushColor), &BytesRead, nullptr);
					auto colors = std::vector<COLORREF>(16);
					ReadFileInt(thrEdFile,
					            colors.data(),
					            gsl::narrow<unsigned int>(colors.size() * sizeof(colors[0])),
					            &BytesRead,
					            nullptr);
					auto brush = CreateSolidBrush(brushColor);
					SelectObject(drawItem->hDC, brush);
					FillRect(drawItem->hDC, &drawItem->rcItem, brush);
					auto iColor = stitchesToDraw[0].attribute & 0xf;
					auto pen    = CreatePenInt(PS_SOLID, 1, colors[iColor]);
					auto iLine  = 0u;
					for (auto iStitch = 0u; iStitch < stitchHeader.stitchCount; iStitch++) {
						if ((stitchesToDraw[iStitch].attribute & 0xf) == iColor) {
							lines[iLine++] = { dToL(stitchesToDraw[iStitch].x * ratio),
								               dToL(drawingDestinationSize.y - stitchesToDraw[iStitch].y * ratio) };
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
			else {
				SetFilePointer(thrEdFile, 84, nullptr, FILE_CURRENT);
			}
			if (stitchHeader.formCount) {
				do {
					// Todo - find a better value than MAXFRMLINS
					auto lines = std::vector<POINT>(MAXFRMLINS);
					SetFilePointer(thrEdFile, 80, nullptr, FILE_CURRENT);
					auto formList   = std::vector<FRMHED>(stitchHeader.formCount);
					auto vertexList = std::vector<fPOINT>(stitchHeader.vertexCount);
					if (fileTypeVersion < 2) {
						auto       formListOriginal = std::vector<FRMHEDO>(stitchHeader.formCount);
						const auto bytesToRead = stitchHeader.formCount * gsl::narrow<unsigned int>(sizeof(formListOriginal[0]));
						ReadFile(thrEdFile, formListOriginal.data(), bytesToRead, &BytesRead, nullptr);
						if (BytesRead != bytesToRead) {
							break;
						}
						const auto _ = std::copy(formListOriginal.cbegin(), formListOriginal.cend(), formList.begin());
					}
					else {
						auto       inFormList  = std::vector<FRMHEDOUT>(stitchHeader.formCount);
						const auto bytesToRead = gsl::narrow<unsigned int>(stitchHeader.formCount * sizeof(inFormList[0]));
						ReadFileInt(thrEdFile, inFormList.data(), bytesToRead, &BytesRead, nullptr);
						if (BytesRead != bytesToRead) {
							break;
						}
						std::copy(inFormList.cbegin(), inFormList.cend(), formList.begin());
					}
					const auto bytesToRead = stitchHeader.vertexCount * gsl::narrow<unsigned int>(sizeof(vertexList[0]));
					ReadFile(thrEdFile, vertexList.data(), bytesToRead, &BytesRead, nullptr);
					if (BytesRead != bytesToRead) {
						break;
					}
					auto iVertex = 0u;
					for (auto iForm = 0; iForm < stitchHeader.formCount; iForm++) {
						const auto iLine = iVertex;
						for (auto iVertexInForm = 0u;
						     (iVertexInForm < formList[iForm].vertexCount) && (iVertex < stitchHeader.vertexCount);
						     iVertexInForm++) {
							lines[iVertexInForm] = { dToL(vertexList[iVertex].x * ratio),
								                     dToL(drawingDestinationSize.y - vertexList[iVertex++].y * ratio) };
						}
						lines[formList[iForm].vertexCount]
						    = { dToL(vertexList[iLine].x * ratio), dToL(drawingDestinationSize.y - vertexList[iLine].y * ratio) };
						SelectObject(drawItem->hDC, FormPen);
						SetROP2(drawItem->hDC, R2_XORPEN);
						if (formList[iForm].type == FRMLINE) {
							PolylineInt(drawItem->hDC, lines.data(), formList[iForm].vertexCount);
						}
						else {
							PolylineInt(drawItem->hDC, lines.data(), formList[iForm].vertexCount + 1);
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

LRESULT CALLBACK thred::internal::WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
		switch (gsl::narrow<long>(LOWORD(wParam))) {
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
				auto scrollPoint = POINT{}; // for scroll bar functions
				scrollPoint.x    = dToL((ZoomRect.right - ZoomRect.left) * LINSCROL);
				if (!scrollPoint.x) {
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
				auto scrollPoint = POINT{}; // for scroll bar functions
				scrollPoint.x    = dToL(-(ZoomRect.right - ZoomRect.left) * LINSCROL);
				if (!scrollPoint.x) {
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
					if (reinterpret_cast<HWND>(lParam) == SpeedScrollBar) {
						const auto position = HIWORD(wParam);
						MovieTimeStep       = MAXDELAY - position;
						setsped();
						SetScrollPos(SpeedScrollBar, SB_CTL, position, TRUE);
					}
				}
			}
			else {
				GSL_SUPPRESS(26490) {
					if (reinterpret_cast<HWND>(lParam) == HorizontalScrollBar) {
						const auto zoomWidth = ZoomRect.right - ZoomRect.left;
						ZoomRect.left        = gsl::narrow<long>(HIWORD(wParam));
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
		switch (LOWORD(wParam)) {
		case SB_LINEDOWN: {
			auto scrollPoint = POINT{};
			scrollPoint.y    = dToL((ZoomRect.top - ZoomRect.bottom) * LINSCROL);
			if (!scrollPoint.y) {
				scrollPoint.y = 1;
			}
			rshft(scrollPoint);
			return 1;
		}
		case SB_LINEUP: {
			auto scrollPoint = POINT{};
			scrollPoint.y    = dToL(-(ZoomRect.top - ZoomRect.bottom) * LINSCROL);
			if (!scrollPoint.y) {
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
			const auto zoomHeight = ZoomRect.top - ZoomRect.bottom;
			ZoomRect.top          = static_cast<double>(UnzoomedRect.y) - gsl::narrow<long>(HIWORD(wParam));
			ZoomRect.bottom       = ZoomRect.top - zoomHeight;
			if (ZoomRect.bottom < 0) {
				ZoomRect.bottom = 0;
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
		GSL_SUPPRESS(26490) DrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (DrawItem->hwndItem == MainStitchWin && DrawItem->itemAction == ODA_DRAWENTIRE) {
			if (StateMap.test(StateFlag::TXTRED)) {
				texture::drwtxtr();
				return 1;
			}
			if (!StateMap.test(StateFlag::RUNPAT)) {
				if (!StateMap.test(StateFlag::HIDSTCH)
				    && (FileHandle || StateMap.test(StateFlag::INIT) || !(*FormList).empty() || StateMap.test(StateFlag::SATPNT))
				    && !StateMap.test(StateFlag::BAKSHO)) {
					drwStch();
				}
				else {
					FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
					thred::duzrat();
					dugrid();
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
				if (FormDataSheet) {
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
			if (PCSHeader.stitchCount) {
				dubar();
			}
			else {
				FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_WINDOW));
			}
			return 1;
		}
		if (!ButtonWin->empty() && DrawItem->hwndItem == (*ButtonWin)[HHID] && DrawItem->itemAction == ODA_DRAWENTIRE) {
			const auto position = (ButtonWidthX3 - PickColorMsgSize.cx) >> 1;
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
				TextOutInt(DrawItem->hDC,
				           position,
				           1,
				           (*StringTable)[STR_PIKOL].c_str(),
				           gsl::narrow<unsigned int>((*StringTable)[STR_PIKOL].size()));
			}
			return 1;
		}
		if (StateMap.test(StateFlag::WASTRAC)) {
			trace::wasTrace();
			return 1;
		}

		for (auto iColor = 0u; iColor < 16; iColor++) {
			if (DrawItem->hwndItem == DefaultColorWin[iColor]) {
				FillRect(DrawItem->hDC, &DrawItem->rcItem, DefaultColorBrush[iColor]);
				if (DisplayedColorBitmap.test(iColor)) {
					SetBkColor(DrawItem->hDC, DefaultColors[iColor]);
					SetTextColor(DrawItem->hDC, defTxt(iColor));
					auto colorNum = std::wstring(fmt::format(L"{}", iColor + 1));
					auto textSize = SIZE{};
					GetTextExtentPoint32Int(
					    DrawItem->hDC, colorNum.c_str(), gsl::narrow<unsigned int>(colorNum.size()), &textSize);
					TextOutInt(DrawItem->hDC,
					           (ButtonWidth - textSize.cx) >> 1,
					           0,
					           colorNum.c_str(),
					           gsl::narrow<unsigned int>(colorNum.size()));
				}
				return 1;
			}
			if (DrawItem->hwndItem == UserColorWin[iColor]) {
				FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[iColor]);
				if (iColor == ActiveColor) {
					SelectObject(DrawItem->hDC, CrossPen);
					SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
					POINT line[2] = {};
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

		if (StateMap.test(StateFlag::BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {
			if (StateMap.test(StateFlag::THUMSHO)) {
				for (auto iThumb = 0u; iThumb < 4; iThumb++) {
					if (iThumb < ThumbnailDisplayCount && DrawItem->hwndItem == BackupViewer[iThumb]) {
						ritbak((*Thumbnails)[ThumbnailsSelected[iThumb]].data(), DrawItem);
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
				auto       screenCenterOffset = 0u;
				const auto maxWindowDimension = SIZE{ ScreenSizePixels.cx - 30, ScreenSizePixels.cy - 30 };
				if ((ThredWindowRect.right - ThredWindowRect.left) > maxWindowDimension.cx) {
					screenCenterOffset    = (ScreenSizePixels.cx - maxWindowDimension.cx) >> 1;
					ThredWindowRect.left  = screenCenterOffset;
					ThredWindowRect.right = ScreenSizePixels.cx - screenCenterOffset;
				}
				if ((ThredWindowRect.bottom - ThredWindowRect.top) > maxWindowDimension.cy) {
					screenCenterOffset     = (ScreenSizePixels.cy - maxWindowDimension.cy) >> 1;
					ThredWindowRect.top    = screenCenterOffset;
					ThredWindowRect.bottom = ScreenSizePixels.cy - screenCenterOffset;
				}
				if (screenCenterOffset) {
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
		}
		GetClientRect(p_hWnd, &ThredWindowRect);
		thred::movStch();
		if (StateMap.test(StateFlag::ZUMED)) {
			const auto bRatio        = StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
			const auto adjustedWidth = StitchWindowClientRect.right / bRatio;
			if (adjustedWidth + ZoomRect.left > UnzoomedRect.x) {
				ZoomRect.right = UnzoomedRect.x;
				ZoomRect.left  = UnzoomedRect.x - adjustedWidth;
			}
			else {
				ZoomRect.right = adjustedWidth + ZoomRect.left;
			}
		}
		else {
			ZoomRect = { gsl::narrow<double>(UnzoomedRect.y), 0.0, 0.0, gsl::narrow<double>(UnzoomedRect.x) };
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
	}
	return DefWindowProc(p_hWnd, message, wParam, lParam);
}

void thred::internal::sachk() {
	for (auto iForm = 0ul; iForm < FormIndex; iForm++) {
		const auto& form = (*FormList)[iForm];
		if (form.type == SAT && form.satinGuideCount) {
			const auto* guide = form.satinOrAngle.guide;
			if (guide) {
				for (auto iGuide = 0u; iGuide < form.satinGuideCount; iGuide++) {
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

#ifdef ALLOCFAILURE
int handle_program_memory_depletion(unsigned int) {
	// ToDo - Make this handle the failure with more user notifiication
	displayText::shoMsg("Memory Allocation Failure");
	exit(EXIT_FAILURE);
}
#endif

#pragma warning(push)
#pragma warning(disable : 26461) // disable warning for hPrevInstance not being marked as a pointer to const
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int    nShowCmd) { // NOLINT
	UNREFERENCED_PARAMETER(nShowCmd);

	ArgList = CommandLineToArgvW(GetCommandLine(), &ArgCount);

#ifdef ALLOCFAILURE
	_PNH old_handler = _set_new_handler(handle_program_memory_depletion);
	// char* testalloc = new char[(~unsigned int((int)0) / 2) - 1]();
	// testalloc[0] = 1;
#endif

	ThrEdInstance = hInstance;

	auto wc        = WNDCLASSEX{};
	wc.cbSize      = sizeof(WNDCLASSEX);
	wc.style       = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = thi::WndProc;
	wc.cbClsExtra  = 0;
	wc.cbWndExtra  = 0;
	wc.hInstance   = ThrEdInstance;
	wc.hIcon       = static_cast<HICON>(LoadImage(ThrEdInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_SHARED));
	wc.hCursor     = nullptr; //  set the cursor to null as the cursor changes in the window:
	                          //  https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setcursor
	wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = L"thred";
	wc.hIconSm       = nullptr;

#if HIGHDPI
	auto previousDpiContext = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
#endif

	// to keep the compiler from complaining
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (RegisterClassEx(&wc)) {
		auto private_TracedMap   = boost::dynamic_bitset<>{};
		TracedMap                = &private_TracedMap;
		auto private_TracedEdges = boost::dynamic_bitset<>{};
		TracedEdges              = &private_TracedEdges;
		auto private_Thumbnails  = std::vector<std::wstring>{};
		Thumbnails               = &private_Thumbnails;
		std::vector<std::unique_ptr<unsigned[]>> private_UndoBuffer(16);
		UndoBuffer                             = &private_UndoBuffer;
		auto private_TempTexturePoints         = std::vector<TXPNT>{};
		auto private_SelectedTexturePointsList = std::vector<unsigned int>{};
		texture::initTextures(&private_TempTexturePoints, &private_SelectedTexturePointsList);
		auto private_StringTable = std::vector<std::wstring>(STR_LEN);
		StringTable              = &private_StringTable;
		auto private_ButtonWin   = std::vector<HWND>{};
		ButtonWin                = &private_ButtonWin;
		auto private_FormOnOff   = std::wstring{};
		FormOnOff                = &private_FormOnOff;
#pragma warning(push)
#pragma warning(disable : 26465) // supress warning for casting away the const
		auto formOnOff = const_cast<LPWSTR>(private_FormOnOff.data());
#pragma warning(pop)

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
#endif                  /* WINVER >= 0x0500 */
		};
		MenuInfo                         = &private_MenuInfo;
		auto private_TempPolygon         = std::vector<fPOINT>{};
		TempPolygon                      = &private_TempPolygon;
		auto private_ClipBuffer          = std::vector<fPOINTATTR>{};
		ClipBuffer                       = &private_ClipBuffer;
		auto private_ClipPoints          = std::vector<fPOINT>{};
		ClipPoints                       = &private_ClipPoints;
		auto private_OutsidePointList    = std::vector<fPOINT>{};
		OutsidePointList                 = &private_OutsidePointList;
		auto private_InsidePointList     = std::vector<fPOINT>{};
		InsidePointList                  = &private_InsidePointList;
		auto private_ValueWindow         = std::vector<HWND>(LASTLIN);
		ValueWindow                      = &private_ValueWindow;
		auto private_LabelWindow         = std::vector<HWND>(LASTLIN);
		LabelWindow                      = &private_LabelWindow;
		auto private_SelectedFormList    = std::vector<unsigned int>{};
		SelectedFormList                 = &private_SelectedFormList;
		auto private_FormControlPoints   = std::vector<POINT>(10);
		FormControlPoints                = &private_FormControlPoints;
		auto private_FormLines           = std::vector<POINT>{};
		FormLines                        = &private_FormLines;
		auto private_FormList            = std::vector<FRMHED>{};
		FormList                         = &private_FormList;
		auto private_RubberBandLine      = std::vector<POINT>(3);
		RubberBandLine                   = &private_RubberBandLine;
		auto private_SelectedFormsLine   = std::vector<POINT>(9);
		SelectedFormsLine                = &private_SelectedFormsLine;
		auto private_SelectedPointsLine  = std::vector<POINT>(9);
		SelectedPointsLine               = &private_SelectedPointsLine;
		auto private_FormAngles          = std::vector<double>{};
		FormAngles                       = &private_FormAngles;
		auto private_AngledFormVertices  = std::vector<fPOINT>{};
		AngledFormVertices               = &private_AngledFormVertices;
		auto private_textureInputBuffer  = std::wstring{};
		TextureInputBuffer               = &private_textureInputBuffer;
		auto private_TexturePointsBuffer = std::vector<TXPNT>{};
		TexturePointsBuffer              = &private_TexturePointsBuffer;
		auto private_PreviousNames       = std::vector<fs::path>{};
		for (auto iVersion = 0; iVersion < OLDNUM; iVersion++) {
			private_PreviousNames.emplace_back(L"");
		}
		PreviousNames                    = &private_PreviousNames;
		auto private_DesignerName        = std::wstring{};
		DesignerName                     = &private_DesignerName;
		auto private_AuxName             = fs::path{};
		auto private_ThrName             = fs::path{};
		auto private_WorkingFileName     = fs::path{};
		auto private_GeName              = fs::path{};
		auto private_ColorFileName       = fs::path{};
		auto private_RGBFileName         = fs::path{};
		auto private_DefaultDirectory    = fs::path{};
		auto private_SearchName          = fs::path{};
		auto private_DefaultBMPDirectory = fs::path{};
		auto private_BalaradName0        = fs::path{};
		auto private_BalaradName1        = fs::path{};
		auto private_BalaradName2        = fs::path{};
		auto private_HomeDirectory       = fs::path{};
		auto private_UserBMPFileName     = fs::path{};
		auto private_IniFileName         = fs::path{};

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

		CreateParams createParams{};
		createParams.bEnableNonClientDpiScaling = true;

		auto private_ScrollSize   = SCROLSIZ;
		ScrollSize                = &private_ScrollSize;
		auto private_ColorBarSize = COLSIZ;
		ColorBarSize              = &private_ColorBarSize;

		if (IniFile.initialWindowCoords.right) {
			ThrEdWindow = CreateWindow(L"thred",
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
			ThrEdWindow = CreateWindow(L"thred",
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
			LoadString(ThrEdInstance, IDS_UNAM, designerBuffer, sizeof(designerBuffer) / sizeof(designerBuffer[0]));
			DesignerName->assign(designerBuffer);
			thi::getdes();
		}

		auto xyRatio        = 0.0; // expand form aspect ratio
		auto rotationAngle  = 0.0;
		auto rotationCenter = dPOINT{};
		auto textureForm    = FRMHED{};

		auto stretchBoxLine = std::vector<POINT>(5); // stretch and expand

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
