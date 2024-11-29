// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "backup.h"
#include "balarad.h"
#include "bitmap.h"
#include "clip.h"
#include "clipStitch.h"
#include "displayText.h"
#include "DST.h"
#include "EnumMap.h"
#include "form.h"
#include "formClip.h"
#include "formForms.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "keys.h"
#include "menu.h"
#include "mouse.h"
#include "PCS.h"
#include "PES.h"
#include "point.h"
#include "repair.h"
#include "Resources/resource.h"
#include "satCon.h"
#include "satin.h"
#include "texture.h"
#include "textureHeader.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "trace.h"
#include "utf8conv.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "boost/range/algorithm_ext/iota.hpp"
#include "fmt/compile.h"
#include "gsl/narrow"
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <basetsd.h>
#include <combaseapi.h>
#include <commdlg.h>
#include <corecrt.h>
#include <corecrt_wstring.h>
#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>
#include <KnownFolders.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <objbase.h>
#include <processenv.h>
#include <sal.h>
#include <shellapi.h>
#include <ShlObj_core.h>
#include <ShObjIdl_core.h>
#include <shtypes.h>
#include <sysinfoapi.h>
#include <WinBase.h>
#include <windef.h>
#include <winerror.h>
#include <wingdi.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <exception>
#include <filesystem>
#include <iterator>
#include <limits>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>
#include <xutility>

#ifdef ALLOCFAILURE
#include <iostream>
#include <new>
#endif

// File open types
enum class FileStyles : uint8_t {
  ALL_FILES, // All file Types
  INS_FILES  // Insert file types
};

// File Indices
enum class FileIndices : uint8_t {
  THR, // Thredworks
  PCS, // Pfaff
  PES, // Brother
  DST  // Tajima
};

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

#pragma pack(push, 1)
class THR_HEAD // ThrEd file header
{
  public:
  uint32_t headerType {};
  uint32_t fileLength {};    // length of strhed + length of stitch data
  uint16_t stitchCount {};   // number of stitches
  uint16_t hoopType {};      // size of hoop
  uint16_t formCount {};     // number of forms
  uint16_t vertexLen {};     // points to form points
  uint16_t vertexCount {};   // number of form points
  uint16_t dlineLen {};      // points to dline data
  uint16_t dlineCount {};    // dline data count
  uint16_t clipDataLen {};   // points to clipboard data
  uint16_t clipDataCount {}; // clipboard data count
};
#pragma pack(pop)

class THR_SINGLE
{
  public:
  static auto getInstance() noexcept -> THR_SINGLE* {
	// NOLINTNEXTLINE(clang-diagnostic-exit-time-destructors)
	static THR_SINGLE instance;
	return &instance;
  }

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::vector<COL_CHANGE>   ColorChangeTable;      // stitch indices where color changes
  std::wstring              DesignerName;          // designer name in clear
  std::vector<POINT>        FormControlPoints;     // form control rectangle in pixel coordinates
  std::vector<uint32_t>     Knots;                 // indices of knot stitches
  std::vector<HWND>         LabelWindow;           // text handles for the form data sheet
  std::vector<wchar_t>      MsgBuffer;             // for user messages
  std::vector<POINT>        NearestPixel;          // selected points
  std::vector<uint32_t>     NearestPoint;          // indices of the closest points
  std::vector<wchar_t>      SideWindowEntryBuffer; // buffer for entering form data sheet numbers
  std::vector<SEARCH_REC>   SortBuffer;            // sort buffer for stitch search
  std::vector<wchar_t>      ThumbnailSearchString; // storage for the thumbnail search string
  std::vector<std::wstring> Thumbnails;            // vector of thumbnail names

  // Paths
  fs::path DefaultDirectory;
  fs::path HomeDirectory; // directory from which ThrEd was executed
  fs::path IniFileName;   //.ini file name
  fs::path ThrName;       // ThrEd file path

  std::vector<fs::path> PreviousNames; // files in the LRU list
  std::vector<fs::path> VersionNames;  // temporary storage for old file version names

  // Pens
  std::vector<HPEN> UserPen; // user color pens

  // Windows
  std::vector<HWND> DefaultColorWin; // default color windows
  std::vector<HWND> SideWindow;      // side message windows
  std::vector<HWND> ThreadSizeWin;   // thread size windows
  std::vector<HWND> UserColorWin;    // user color windows

  // NOLINTEND(misc-non-private-member-variables-in-classes)

  THR_SINGLE(const THR_SINGLE&)                    = delete;
  auto operator=(const THR_SINGLE&) -> THR_SINGLE& = delete;
  THR_SINGLE(THR_SINGLE&&)                         = delete;
  auto operator=(THR_SINGLE&&) -> THR_SINGLE&      = delete;

  private:
  THR_SINGLE() noexcept = default;
  ~THR_SINGLE()         = default;
};

// thred internal namespace
namespace {
constexpr auto ARROWPNT = 3U;            // points required to draw arrow
constexpr auto CHRDEF   = 0.25;          // default chain stitch ratio
constexpr auto CHSDEF   = 24.0F;         // default chain stitch length
constexpr auto COLSIZ   = int32_t {12};  // logical pixel width of the color bar
constexpr auto DEFPIX   = uint16_t {2U}; // default nudge pixels
constexpr auto DEFULEN  = 12.0F;         // default underlay stitch length
constexpr auto DNDLEN   = 256U;          // designer name decoding table length
constexpr auto DNELEN   = 128U;          // designer name encoding table length
constexpr auto DNLEN    = 50U;           // designer name order table length
constexpr auto FLTDST = COMDLG_FILTERSPEC {L"Tajima", L"*.dst"}; // Display only Tajima format files (.dst)
constexpr auto FLTPCS = COMDLG_FILTERSPEC {L"Pfaff", L"* .pcs"}; // Display only Pfaff format files (.pcs)
constexpr auto FLTPES = COMDLG_FILTERSPEC {L"Brother", L"*.pes"}; // Display only Brother format files (.pes)
constexpr auto FLTTHR = COMDLG_FILTERSPEC {L"Thredworks", L"*.thr"}; // Display only ThrEd format files (.thr)
constexpr auto FTYPMASK = uint32_t {0xff000000U}; // top byte mask used for file type verification
constexpr auto HUPS     = int32_t {5};            // number of hoops the user can select
constexpr auto KNOTLEN  = int32_t {54};           // set knots for stitches longer than this
constexpr auto KNOTSCNT = 5U;                     // length of knot pattern in stitches
constexpr auto LRU_MENU_ID = std::array<uint32_t, OLDNUM> {FM_ONAM0, FM_ONAM1, FM_ONAM2, FM_ONAM3}; // recently used file menu ID's
constexpr auto MAXDELAY = int32_t {600};         // maximum movie time step
constexpr auto MINDELAY = int32_t {1};           // minimum movie time step
constexpr auto MINZUM   = int32_t {5};           // minimum zoom in stitch points
constexpr auto MINZUMF  = float {MINZUM};        // minimum zoom in stitch points
constexpr auto MSGSIZ   = uint32_t {8192U};      // size of the message buffer
constexpr auto NERCNT   = 4U;                    // number of entries in the nearestxxx arrays;
constexpr auto NUGINI   = 2.0F;                  // default nudge step
constexpr auto PAGSCROL = 0.9F;                  // page scroll factor
constexpr auto SCROLSIZ = int32_t {12};          // logical pixel width of a scroll bar
constexpr auto SIGMASK = uint32_t {0x00ffffffU}; // three byte mask used for file signature verification
constexpr auto SWBLEN   = 11U; // Side Window buffer length including the zero terminator
constexpr auto SWCOUNT  = 16U; // number of side windows to create/track
constexpr auto THREDSIG = uint32_t {0x746872U}; // ThrEd format file signature
constexpr auto TSIZ30   = 0.3F;                 // #30 thread size in millimeters
constexpr auto TSIZ40   = 0.2F;                 // #40 thread size in millimeters
constexpr auto TSIZ60   = 0.05F;                // #60 thread size in millimeters
constexpr auto TSSSIZ   = size_t {32U};         // size of the thumbnail search buffer
constexpr auto ZUMFCT   = 0.65F;                // zoom factor

// graphics constants
constexpr auto DEFAULT_COLORS = std::array<COLORREF, COLORCNT> {0x00000000,
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

// Pens
auto BackgroundPen  = gsl::narrow_cast<HPEN>(nullptr); // background color pen
auto BoxPen         = std::array<HPEN, 4> {};          // box pens
auto CrossPen       = gsl::narrow_cast<HPEN>(nullptr); // pen for crosses in color windows
auto GridPen        = gsl::narrow_cast<HPEN>(nullptr); // pen for stitch grid
auto GroupSelectPen = gsl::narrow_cast<HPEN>(nullptr); // pen for group select
auto KnotPen        = gsl::narrow_cast<HPEN>(nullptr); // knot pen
auto LinePen        = gsl::narrow_cast<HPEN>(nullptr); // line pen for stitch move lines
auto MultiFormPen   = gsl::narrow_cast<HPEN>(nullptr); // multiple selected forms pen
auto SelectAllPen   = gsl::narrow_cast<HPEN>(nullptr); // pen for drawing large boxes
auto ZoomMarkPen    = gsl::narrow_cast<HPEN>(nullptr); // zoom mark pen

auto BackgroundPenWidth = int32_t {}; // width of the background pen

// brushes
auto BackgroundBrush   = gsl::narrow_cast<HBRUSH>(nullptr); // background color brush
auto DefaultColorBrush = std::array<HBRUSH, COLORCNT> {};   // default color brushes
auto UserColorBrush    = std::array<HBRUSH, COLORCNT> {};   // user color brushes

// for the choose color dialog box
auto ColorStruct = CHOOSECOLOR {};
auto CustomColor = std::array<COLORREF, COLORCNT> {};

// for the background color dialog box
auto BackgroundColorStruct = CHOOSECOLOR {};
auto CustomBackgroundColor = std::array<COLORREF, COLORCNT> {};

// threads
auto ThreadSize       = std::array<wchar_t, COLORCNT> {};  // user selected thread sizes
auto ThreadSize30     = TSIZ30;                            // #30 thread size
auto ThreadSize40     = TSIZ40;                            // #40 thread size
auto ThreadSize60     = TSIZ60;                            // #60 thread size
auto ThreadSizeIndex  = std::array<uint32_t, COLORCNT> {}; // thread size indices
auto ThreadSizePixels = std::array<int32_t, COLORCNT> {};  // thread sizes in pixels

constexpr auto TSSIZE = ThreadSize.size(); // size of the user selected thread size array

// windows
auto BackupViewer = std::array<HWND, QUADRT> {}; // handles of multiple file viewing windows in quadrants
auto ChangeThreadSizeWin = std::array<HWND, 3> {};          // thread size change windows
auto ColorBar            = gsl::narrow_cast<HWND>(nullptr); // color bar
auto FirstWin = gsl::narrow_cast<HWND>(nullptr); // first window not destroyed for exiting enumerate loop
auto GeneralNumberInputBox = gsl::narrow_cast<HWND>(nullptr); // general number input box
auto HorizontalScrollBar   = gsl::narrow_cast<HWND>(nullptr); // horizontal scroll bar
auto SpeedScrollBar        = gsl::narrow_cast<HWND>(nullptr); // speed scroll bar for movie
auto VerticalScrollBar     = gsl::narrow_cast<HWND>(nullptr); // vertical scroll bar

// General variables
auto ArgCount           = int32_t {};                         // command line argument count
auto ArgList            = gsl::narrow_cast<LPTSTR*>(nullptr); // command line argument array
auto BoxOffset          = std::array<int32_t, 4> {};
auto ClipInsertBoxLine  = std::array<POINT, SQPNTS> {}; // for displaying clipboard insert rectangle
auto ColorBarRect       = RECT {};                      // color bar rectangle
auto ColorBarSize       = COLSIZ;                       // Color bar width scaled for DPI
auto CurrentStitchIndex = uint32_t {}; // pointer to the current selection for length search
auto DisplayedColorBitmap = std::bitset<COLORCNT + COLORCNT> {}; // Map of color numbers in design that are displayed
auto ExtendedHeader   = THR_HEAD_EX {};                          // ThrEd file header extension
auto FileVersionIndex = uint8_t {};                              // points to old version to be read
auto HorizontalRatio = float {}; // horizontal ratio between the zoom window and the entire stitch space
auto InsertCenter           = F_POINT {};  // center point in inserted file
auto InsertSize             = SIZE {};     // size of file insert window
auto InsertedFormIndex      = uint32_t {}; // saved form pointer for inserting files
auto InsertedStitchIndex    = uint32_t {}; // saved stitch pointer for inserting files
auto InsertedVertexIndex    = uint32_t {}; // saved vertex pointer for inserting files
auto LargestStitchIndex     = uint32_t {}; // pointer to the largest stitch in the selected range
auto LastKeyCode            = std::numeric_limits<wchar_t>::max(); // last key code
auto MoveLine0              = std::array<POINT, LNPNTS> {};        // move point line
auto MoveLine1              = std::array<POINT, LNPNTS> {};        // move point line
auto MovieTimeStep          = int32_t {};                          // time delay for stitchout
auto NameDecoder            = std::array<uint8_t, DNDLEN> {};      // designer name decode
auto NameEncoder            = std::array<uint8_t, DNELEN> {};      // designer name encoding
auto NameOrder              = std::array<uint32_t, DNLEN> {};      // designer name order table
auto NearestCount           = uint32_t {};                         // number of boxes selected
auto NumericCode            = wchar_t {};                          // keyboard numerical input
auto PickColorMsgSize       = SIZE {};                             // size of the pick color message
auto PreferenceIndex        = uint32_t {};                  // index to the active preference window
auto PrevGroupEndStitch     = uint32_t {};                  // higher end of previous selection
auto PrevGroupStartStitch   = uint32_t {};                  // lower end of previous selection
auto PreviousFormIndex      = uint32_t {};                  // previously selected form
auto RotateAngle            = float {};                     // angle for pixel rotate
auto RotateBoxCrossHorzLine = std::array<POINT, LNPNTS> {}; // horizontal part of the rotate cross
auto RotateBoxCrossVertLine = std::array<POINT, LNPNTS> {}; // vertical part of the rotate cross
auto RotateBoxOutline       = std::array<POINT, SQPNTS> {}; // for drawing the rotate rectangle
auto RotateBoxToCursorLine = std::array<POINT, LNPNTS> {}; // line from the cursor to the center of the rotate cross
auto RotationHandleAngle = float {};                       // angle of the rotation handle
auto RunPoint            = uint32_t {};                    // point for animating stitchout
auto ScreenSizePixels    = SIZE {};                        // screen size in pixels
auto ScrollSize          = SCROLSIZ;                       // Scroll bar width scaled for DPI
auto SelectBoxOffset = POINT {}; // offset of the spot the user selected from the lower left of the select box
auto SelectBoxSize           = SIZE {};          // size of the select box
auto SelectedFormsRange      = RANGE {};         // range of selected forms
auto SelectedRange           = RANGE {};         // first and last stitch for min/max stitch select
auto ShowStitchThreshold     = SHOPNTS;          // show stitch grid below this zoom level
auto SideWinMsgIdx           = uint32_t {};      // track current position in SideWindowEntryBuffer
auto SmallStitchLength       = SMALSIZ * PFGRAN; // user can remove stitches smaller than this
auto SmallestStitchIndex     = uint32_t {}; // pointer to the smallest stitch in the selected range
auto SortIndex               = uint32_t {}; // index for stitch search
auto StitchArrow             = std::array<POINT, ARROWPNT> {}; // arrow for selected stitch
auto StitchBoxesThreshold    = STCHBOX;                        // threshold for drawing stitch boxes
auto StitchWindowAbsRect     = RECT {};                        // stitch window size,absolute
auto StitchWindowAspectRatio = float {};                       // aspect ratio of the stitch window
auto StitchWindowBmp = gsl::narrow_cast<HBITMAP>(nullptr); // bitmap for the memory stitch device context
auto StitchWindowSize      = SIZE {};                      // size of the stitch window in pixels
auto StitchesPerFrame      = uint32_t {}; // number of stitches to draw in each frame
auto ThredDC               = gsl::narrow_cast<HDC>(nullptr); // main device context handle
auto ThredWindowRect       = RECT {};                        // main window size
auto ThumbnailDisplayCount = uint32_t {}; // number of thumbnail file names selected for display
auto ThumbnailIndex        = uint32_t {}; // index into the thumbnail filname table
auto ThumbnailsSelected = std::array<uint32_t, 4> {}; // indexes of thumbnails selected for display
auto VerticalIndex = uint8_t {}; // vertical index of the color window, calculated from mouse click
auto VerticalRatio = float {}; // vertical ratio between the zoom window and the entire stitch space
auto ZoomMin       = float {}; // minimum allowed zoom value

THR_SINGLE* ThrSingle;

// Definitions
void angdif(float& lowestAngle, float& highestAngle, float angle) noexcept;
void barnam(HWND window, uint32_t iThumbnail);
void box(uint32_t iNearest, HDC hDC);
void boxs();
auto centr() noexcept -> F_POINT;
void chk1col();
auto chk2of() -> bool;
auto chkMsg(std::vector<POINT>& stretchBoxLine, float& xyRatio, float& angle, F_POINT& rotationCenter, FRM_HEAD& textureForm)
    -> bool;
auto chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) -> bool;
auto chkattr(fs::path const& filename) -> bool;
auto chkhid(size_t colorToCheck) -> bool;
void chkirct() noexcept;
auto chkminus(wchar_t code) noexcept -> bool;
void chkncol();
void chknum();
auto chkup(uint32_t count, size_t iStitch) -> uint32_t;
void clpradj(F_RECTANGLE& clipRectAdjusted, F_POINT_ATTR const& stitch) noexcept;
auto cmpstch(uint32_t iStitchA, uint32_t iStitchB) noexcept -> bool;
void createBrushes() noexcept;
void cros(uint32_t iStitch);
void defNam(fs::path const& fileName);
auto defTxt(uint32_t iColor) -> COLORREF;
void delcol();
void delfstchs();
void delknt();
void delsmal(uint32_t startStitch, uint32_t endStitch);
void delstch1(uint32_t iStitch);
void destroyBV() noexcept;

auto CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

void doDrwInit();
void doInitUnzoomed();
void doInitZoomed();
void doStretch(uint32_t start, uint32_t end);
void drawBackground();
void drawOffScreenLine(const F_POINT_ATTR& iStitch, const F_POINT_ATTR& prevStitch, const float& maxYcoord);
void drawStitchBoxes();
void drwLin(std::vector<POINT>& linePoints, uint32_t currentStitch, uint32_t length, HPEN hPen);
void drwStch();
void drwknot();
void drwlstch(uint32_t finish);
void drwmrk(HDC hDC);
void duClos(uint32_t            startStitch,
            uint32_t            stitchCount,
            F_POINT const&      stitchPoint,
            std::vector<float>& gapToNearest) noexcept(!std::is_same_v<ptrdiff_t, int>);
void duar(POINT const& stitchCoordsInPixels) noexcept(std::is_same_v<size_t, uint32_t>);
void dubar();
void dubuf(std::vector<char>& buffer);
void duclp() noexcept(std::is_same_v<size_t, uint32_t>);
void ducmd();
void ducros(HDC hDC);
void dugrid();
void duhom();
void dulin(std::array<POINT, 2> const& moveLine0, std::array<POINT, 2> const& moveLine1);
void dumov();
auto dunum(wchar_t code) noexcept -> bool;
void duprct(FRM_HEAD const& form);
void durit(std::vector<char>& destination, const void* source, uint32_t count);
void durot() noexcept(std::is_same_v<size_t, uint32_t>);
void dusel(HDC hDC);
void duselrng(RANGE& selectedRange);
void dusid(LIST_TYPE entry, int32_t& windowLocation, SIZE const& windowSize);

void duver(fs::path const& name);
void endknt(std::vector<F_POINT_ATTR>& buffer, uint32_t finish);

auto CALLBACK enumChildProc(HWND hwnd, LPARAM lParam) noexcept -> BOOL;

void f1del(uint32_t formIndex);
void fillSortBuffer();
auto find1st() -> uint32_t;
void fnamtabs();
void fndknt();
void formStretch(uint32_t form);
void frmcalc(uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex);
void frmpos(FRM_HEAD& form, float deltaX, float deltaY) noexcept(!std::is_same_v<ptrdiff_t, int>);
void frmsnap(uint32_t start, uint32_t count) noexcept(!std::is_same_v<ptrdiff_t, int>);
auto frmstch() -> bool;

auto CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

void getbak();
void getDocsFolder(fs::path& directory);
auto gethand(std::vector<F_POINT_ATTR> const& stitch, uint32_t stitchCount) noexcept -> uint32_t;
auto getMaxCount() -> uint32_t;
auto getNewFileName(fs::path& newFileName, FileStyles fileTypes, FileIndices fileIndex) -> bool;
void gselrng() noexcept;
void handleChkMsgWMCOMMAND(F_POINT& rotationCenter);
auto handleDeleteLineForm(FRM_HEAD& form) -> bool;
void handleDeleteSatinForm(FRM_HEAD& form, bool& satinFlag);
void handleFeatherIDOK(HWND hwndlg);
auto handleFeatherWMCOMMAND(WPARAM const& wparam, HWND hwndlg) -> bool;
void handleFeatherWMINITDIALOG(HWND hwndlg);
auto handleLockWMCOMMAND(HWND hwndlg, WPARAM const& wparam) -> bool;
auto handleLockWMINITDIALOG(HWND hwndlg, LPARAM lparam, WPARAM const& wparam) -> bool;
auto handleNumericInput(wchar_t const& code, bool& retflag) -> bool;
void handleSizeRestored(HWND p_hWnd);
auto handleWndMsgWMKEYDOWN(FRM_HEAD& textureForm, F_POINT& rotationCenter, std::vector<POINT> const& stretchBoxLine)
    -> bool;
auto handleWndProcWMDRAWITEM(LPARAM lParam) -> bool;
auto handleWndProcWMHSCROLL(WPARAM const& wParam, float LINSCROL, LPARAM lParam) -> bool;
void handleWndProcWMINITMENU();
void handleWndProcWMMOVE(HWND p_hWnd);
void handleWndProcWMSIZE(HWND p_hWnd, WPARAM wParam);
auto handleWndProcWMVSCROLL(WPARAM const& wParam, float LINSCROL) -> bool;
void hidwnd(HWND hwnd) noexcept;
void infadj(float& xCoordinate, float& yCoordinate) noexcept;
void init();
void insadj();
void inscol();
auto insTHR(fs::path const& insertedFile, F_RECTANGLE& insertedRectangle) -> bool;
auto isInBox(POINT const& point, RECT const& box) noexcept -> bool;
auto isthr(fs::path const& thredPath) -> bool;
auto kjmp(std::vector<F_POINT_ATTR>& buffer, uint32_t start) -> uint32_t;
void lenfn(uint32_t startStitch, uint32_t endStitch, uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex);
void lensadj();
void loadColors() noexcept;

auto CALLBACK lockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

void makCol() noexcept;
auto makbig(uint32_t start, uint32_t finish) -> uint32_t;
void movins();
auto getSaveName(fs::path& fileName, FileIndices& fileType) -> bool;
void noMsg();
auto nuBak() noexcept -> BOOL;
void nuBrush(HBRUSH& brush, COLORREF color) noexcept;
auto nuCol(COLORREF init) noexcept -> BOOL;
void nuFil(FileIndices fileIndex);
void nuRct() noexcept;
void nuStchSiz(uint32_t iColor, int32_t width) noexcept(!std::is_same_v<ptrdiff_t, int>);
auto nuang(float originalAngle, float xDelta, float yDelta) noexcept -> float;
void nunams();
void nuselrct();
void nuthum(wchar_t character);
void ofstch(std::vector<F_POINT_ATTR>& buffer, uint32_t iSource, char offset, F_POINT const& knotStep, uint32_t knotAttribute);
auto oldwnd(HWND window) noexcept -> bool;
void patdun();
void prtred(HANDLE fileHandle, uint32_t code);
auto pxchk(float pixelSize) -> uint16_t;
auto readTHRFile(std::filesystem::path const& newFileName) -> bool;
void rebak();
void redfnam(std::wstring& designerName);
void redini();
void relin();
void resetState();
void respac(FRM_HEAD& form) noexcept;
void ritbak(fs::path const& fileName, DRAWITEMSTRUCT const& drawItem);
void ritcor(F_POINT_ATTR const& pointAttribute);
void ritfnam(std::wstring const& designerName);
void ritini();
void ritlayr();
void ritloc();
void ritlock(std::vector<WIN32_FIND_DATA> const& fileInfo, HWND hwndlg) noexcept;
void rotang(F_POINT unrotatedPoint, POINT& rotatedPoint, float rotationAngle, F_POINT const& rotationCenter);
void rotfns(float rotationAngle);
void rotpix(POINT const& unrotatedPoint, POINT& rotatedPoint, POINT const& rotationCenterPixels) noexcept;
void rotstch(F_POINT_ATTR& stitch, float rotationAngle, F_POINT const& rotationCenter) noexcept;
auto rsed() noexcept -> uint32_t;
void rshft(POINT const& shiftPoint);
void rstdu();
void rthumnam(uint32_t iThumbnail);
void sachk();
void sav();
auto savcmp() noexcept -> bool;
auto sdCor2px(F_POINT_ATTR const& stitchPoint) -> POINT;
void segentr(float rotationAngle);
void selin(uint32_t start, uint32_t end, HDC hDC);
auto setRmap(boost::dynamic_bitset<>& stitchMap, F_POINT_ATTR const& stitchPoint, F_POINT const& cellSize) -> bool;
void setScrollVisibility();
void setSideWinVal(int index);
void setbak(int32_t penWidth) noexcept;
void setknt();
void setPrefs();
void setsped();
void setsrch(bool end);
void shft2box();
void shownd(HWND hwnd) noexcept;
auto sidclp() -> bool;
void sidhup();
void sidmsg(FRM_HEAD const& form, HWND window);
void srchk();
auto srchknot(uint32_t source) -> uint32_t;
void stCor2px(F_POINT_ATTR const& stitch, POINT& point);
auto stch2px2(uint32_t iStitch) -> bool;
void stchPars();
void stchWnd();
void stchbox(uint32_t iStitch, HDC hDC);
void stchout();
void stchsnap(uint32_t start, uint32_t finish) noexcept(!std::is_same_v<ptrdiff_t, int>);
auto stlen(uint32_t iStitch) -> float;
void strtknt(std::vector<F_POINT_ATTR>& buffer, uint32_t start);
void thrInit() noexcept;
void thrsav();
void unboxs();
void uncros();
auto unthrsh(wchar_t level) noexcept -> float;
void unthum();

auto CALLBACK wndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT;

auto wastch(uint32_t const& formIndex) -> bool;
void zRctAdj() noexcept;

#ifdef ALLOCFAILURE
void handle_program_memory_depletion();
#endif

// Functions
void angdif(float& lowestAngle, float& highestAngle, float const angle) noexcept {
  if (angle > highestAngle) {
	highestAngle = angle;
	return;
  }
  lowestAngle = std::min(angle, lowestAngle);
}

// ReSharper disable CppParameterMayBeConst
void barnam(HWND window, uint32_t const iThumbnail) {
  if (iThumbnail >= ThumbnailDisplayCount) {
	SetWindowText(window, L"");
	return;
  }
  auto const thumbPath =
      fs::path(ThrSingle->Thumbnails.operator[](ThumbnailsSelected.at(iThumbnail)).data());
  constexpr auto TNAMELEN = 12U; // how many characters to display with each thumbnail

  auto const name = thumbPath.stem().wstring().substr(0U, TNAMELEN);
  SetWindowText(window, name.c_str());
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void box(uint32_t const iNearest, HDC hDC) {
  auto const itBoxWidth     = wrap::next(BoxOffset.begin(), iNearest);
  auto const boxWidth       = *itBoxWidth;
  auto const itNearestPixel = wrap::next(ThrSingle->NearestPixel.begin(), iNearest);
  auto const npx            = itNearestPixel->x;
  auto const npy            = itNearestPixel->y;
  auto       line           = std::array<POINT, SQPNTS> {};

  line[0] = {.x = npx - boxWidth, .y = npy - boxWidth};
  line[1] = {.x = npx + boxWidth, .y = npy - boxWidth};
  line[2] = {.x = npx + boxWidth, .y = npy + boxWidth};
  line[3] = {.x = npx - boxWidth, .y = npy + boxWidth};
  line[4] = {.x = npx - boxWidth, .y = npy - boxWidth};
  wrap::polyline(hDC, line.data(), wrap::toUnsigned(line.size()));
}
// ReSharper restore CppParameterMayBeConst

void boxs() {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  auto itBoxPen = BoxPen.begin();
  for (auto iNear = 0U; iNear < NearestCount; ++iNear) {
	SelectObject(StitchWindowDC, *itBoxPen++);
	box(iNear, StitchWindowDC);
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

auto centr() noexcept -> F_POINT {
  auto const center = POINT {std::lround((ZoomRect.right - ZoomRect.left) * 0.5F),
                             std::lround((ZoomRect.top - ZoomRect.bottom) * 0.5F)};
  return F_POINT {ZoomRect.left + wrap::toFloat(center.x), ZoomRect.bottom + wrap::toFloat(center.y)};
}

void chk1col() {
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
  for (auto iColorChange = size_t {}; iColorChange < thred::maxColor(); ++iColorChange) {
	auto const ccTableIt = wrap::next(ThrSingle->ColorChangeTable.begin(), iColorChange);
	if (auto const ccTableNextIt = std::next(ccTableIt);
	    ccTableNextIt->stitchIndex - ccTableIt->stitchIndex != 1) {
	  continue;
	}
	auto const iStitch  = ccTableIt->stitchIndex;
	auto       stitchIt = iStitch != 0U ? wrap::next(Instance->stitchBuffer.begin(), iStitch - 1U)
	                                    : Instance->stitchBuffer.begin();
	auto const color    = stitchIt->attribute & COLMSK;
	++stitchIt;
	stitchIt->attribute &= NCOLMSK;
	stitchIt->attribute |= color;
  }
}

auto chk2of() -> bool {
  if (!Instance->stateMap.test(StateFlag::SELBOX)) {
	return false;
  }
  if (Instance->userFlagMap.test(UserFlag::FIL2OF)) {
	return false;
  }
  return true;
}

auto chkMsg(std::vector<POINT>& stretchBoxLine, float& xyRatio, float& angle, F_POINT& rotationCenter, FRM_HEAD& textureForm)
    -> bool {
  if (WinMsg.message == WM_MOUSEMOVE) {
	return mouse::handleMouseMove(stretchBoxLine, xyRatio, angle, rotationCenter, textureForm);
  }
  if (WinMsg.message == WM_LBUTTONUP) {
	auto       retflag = true;
	auto const retval  = mouse::handleLeftButtonUp(xyRatio, angle, rotationCenter, retflag);
	if (retflag) {
	  return retval;
	}
  }
  if (WinMsg.message == WM_RBUTTONDOWN || WinMsg.message == WM_LBUTTONDOWN) {
	if (mouse::handleEitherButtonDown()) {
	  return true;
	}
  }
  if (WinMsg.message == WM_RBUTTONDOWN) {
	return mouse::handleRightButtonDown();
  }
  if (WinMsg.message == WM_LBUTTONDOWN) {
	{
	  auto retflag = true;
	  auto const retval = mouse::handleLeftButtonDown(stretchBoxLine, xyRatio, textureForm, retflag);
	  if (retflag) {
		return retval;
	  }
	}
  }
  switch (WinMsg.message) {
	case WM_TIMER: {
	  if (Instance->stateMap.test(StateFlag::RUNPAT) && WinMsg.wParam == 0U) {
		stchout();
	  }
	  break;
	}
	case WM_CHAR: {
	  if (iswgraph(gsl::narrow<wint_t>(WinMsg.wParam)) != 0) {
		nuthum(towlower(gsl::narrow<wint_t>(WinMsg.wParam)));
	  }
	  break;
	}
	case WM_KEYDOWN: {
	  if (handleWndMsgWMKEYDOWN(textureForm, rotationCenter, stretchBoxLine)) {
		return true;
	  }
	  break;
	}
	case WM_COMMAND: {
	  thred::unmsg();
	  handleChkMsgWMCOMMAND(rotationCenter);
	  break;
	}
	default: {
	  break;
	}
  }
  return false;
}

// check if a click occurred in a vertical set of windows
// and calculate which window had the click
// ReSharper disable CppParameterMayBeConst
auto chkMsgs(POINT const clickCoord, HWND topWindow, HWND bottomWindow) -> bool {
  auto topRect    = RECT {};
  auto bottomRect = RECT {};
  GetWindowRect(topWindow, &topRect);
  GetWindowRect(bottomWindow, &bottomRect);
  if (clickCoord.x <= topRect.left || clickCoord.x >= bottomRect.right ||
      clickCoord.y <= topRect.top || clickCoord.y >= bottomRect.bottom) {
	return false;
  }
  auto const indexRange = gsl::narrow<uint8_t>(((bottomRect.bottom - topRect.top) / ButtonHeight) - 1);
  VerticalIndex = indexRange - gsl::narrow<uint8_t>((bottomRect.bottom - clickCoord.y) / ButtonHeight);
  if (VerticalIndex > COLORMAX) { // Something has broken so do something reasonable
	VerticalIndex &= COLMSK;
	return false;
  }
  // we have a valid Index
  return true;
}
// ReSharper restore CppParameterMayBeConst

auto chkattr(fs::path const& filename) -> bool {
  constexpr auto WRBITS = fs::perms::owner_write | fs::perms::others_write | fs::perms::group_write; // Write permissions bits
  if (space(filename.parent_path()).available == 0U) {
	Instance->stateMap.set(StateFlag::NOTFREE);
	return true;
  }
  Instance->stateMap.reset(StateFlag::NOTFREE);
  if (auto const writePerms = status(filename).permissions() & WRBITS; writePerms != fs::perms::none) {
	return false;
  }
  auto const fmtStr = displayText::format(IDS_OVRLOK, filename.wstring());
  if (auto const buttonPressed =
          MessageBox(ThrEdWindow, fmtStr.c_str(), displayText::loadStr(IDS_OVRIT).c_str(), MB_YESNO);
      buttonPressed != IDYES) {
	return true;
  }
  permissions(filename, WRBITS, fs::perm_options::add);
  return false;
}

auto chkhid(size_t const colorToCheck) -> bool {
  if (Instance->stateMap.test(StateFlag::HID)) {
	return ThrSingle->ColorChangeTable.operator[](colorToCheck).colorIndex == ActiveColor;
  }

  return true;
}

void chkirct() noexcept {
  auto const screenLimits = SIZE {ScreenSizePixels.cx - 1, ScreenSizePixels.cy - 1};
  IniFile.initialWindowCoords.left = std::clamp(IniFile.initialWindowCoords.left, 0L, screenLimits.cx);
  IniFile.initialWindowCoords.right = std::clamp(IniFile.initialWindowCoords.right, 0L, screenLimits.cx);
  IniFile.initialWindowCoords.top = std::clamp(IniFile.initialWindowCoords.top, 0L, screenLimits.cy);
  IniFile.initialWindowCoords.bottom =
      std::clamp(IniFile.initialWindowCoords.bottom, 0L, screenLimits.cy);
  constexpr auto MINWINSZ = LONG {300}; // if the minimum window dimension is less than this, resize
  if (IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left < MINWINSZ) {
	IniFile.initialWindowCoords.left  = 0;
	IniFile.initialWindowCoords.right = screenLimits.cx / 2;
  }
  if (IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top < MINWINSZ) {
	IniFile.initialWindowCoords.top    = 0;
	IniFile.initialWindowCoords.bottom = screenLimits.cy / 2;
  }
}

auto chkminus(wchar_t const code) noexcept -> bool {
  if (code != VK_OEM_MINUS && code != VK_SUBTRACT) { // is '-' key pressed?
	return false;
  }
  if (PreferenceIndex == PRFCLPPHS) { // Clipboard Offset in preferences
	return true;
  }
  if (auto const& form = Instance->formList.operator[](ClosestFormToCursor);
      FormMenuChoice == LFRMSPAC && form.isFanClip()) { // Fill Spacing for fan clip fill
	return true;
  }
  if (FormMenuChoice == LWLKIND) { // Indent
	return true;
  }
  return false;
}

void chkncol() {
  auto initialColor = Instance->stitchBuffer.front().attribute & COLMSK;
  auto buffer       = std::vector<F_POINT_ATTR> {};
  buffer.reserve(Instance->stitchBuffer.size());
  Instance->stateMap.reset(StateFlag::FILDIR);
  auto iStitch = 0U;
  for (auto& stitch : Instance->stitchBuffer) {
	if (auto const color = stitch.attribute & COLMSK; color == initialColor) {
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
	++iStitch;
  }
  Instance->stitchBuffer = std::move(buffer);
}

void chknum() {
  xt::clrstch();
  auto& formList = Instance->formList;

  if (SideWinMsgIdx != 0U) {
	auto& valueWindow           = Instance->valueWindow;
	auto& sideWindowEntryBuffer = ThrSingle->SideWindowEntryBuffer;
	if (FormMenuChoice != 0U) { // the form menu is open
	  auto& form = formList.operator[](ClosestFormToCursor);

	  auto const value = wrap::wcsToFloat(sideWindowEntryBuffer.data()) * PFGRAN;
	  switch (FormMenuChoice) {
		case LTXOF: { // update texture fill spacing
		  thred::savdo();
		  form.txof = value;
		  break;
		}
		case LUANG: { // update underlay angle
		  thred::savdo();
		  form.underlayStitchAngle = value * DEGRADF * IPFGRAN;
		  break;
		}
		case LUSPAC: { // update underlay spacing
		  thred::savdo();
		  form.underlaySpacing = value;
		  break;
		}
		case LWLKIND: { // update underlay indent spacing
		  thred::savdo();
		  form.underlayIndent = value;
		  break;
		}
		case LULEN: { // update underlay stitch length
		  thred::savdo();
		  form.underlayStitchLen = value;
		  break;
		}
		case LDSTRT: { // update fill start point
		  thred::savdo();
		  form.fillStart = wrap::round<uint32_t>(value * IPFGRAN);
		  form.fillStart %= form.vertexCount;
		  break;
		}
		case LDEND: { // update fill end point
		  thred::savdo();
		  form.fillEnd = wrap::round<uint32_t>(value * IPFGRAN);
		  form.fillEnd %= form.vertexCount;
		  break;
		}
		case LFTHUPCNT: { // update feather up count
		  thred::savdo();
		  auto           upcnt    = value * IPFGRAN;
		  constexpr auto FUPCLAMP = 255.0F; // clamp the feather up count

		  upcnt                = std::min(upcnt, FUPCLAMP);
		  form.feather.upCount = wrap::round<uint8_t>(upcnt);
		  break;
		}
		case LFTHCOL: { // update feather color
		  if (value != 0.0F) {
			thred::savdo();
			form::nufthcol((wrap::wcsToLong<uint32_t>(sideWindowEntryBuffer.data()) - 1U) & COLMSK);
			setSideWinVal(LFTHCOL);
			thred::coltab();
		  }
		  thred::unsid();
		  Instance->stateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LFRMCOL: { // update form color
		  if (value != 0.0F) {
			thred::savdo();
			auto const colVal = gsl::narrow_cast<uint8_t>(
			    (wrap::wcsToLong<uint32_t>(sideWindowEntryBuffer.data()) - 1U) & COLMSK);
			form::nufilcol(colVal);
			auto const fmtStr = format(FMT_COMPILE(L"{}"), colVal + 1U);
			SetWindowText(valueWindow.operator[](LFRMCOL), fmtStr.c_str());
			thred::coltab();
		  }
		  thred::unsid();
		  Instance->stateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LUNDCOL: { // update underlay color
		  if (value != 0.0F) {
			thred::savdo();
			auto const colVal = gsl::narrow_cast<uint8_t>(
			    (wrap::wcsToLong<uint32_t>(sideWindowEntryBuffer.data()) - 1U) & COLMSK);
			form.underlayColor = colVal;
			auto const fmtStr  = format(FMT_COMPILE(L"{}"), colVal + 1U);
			SetWindowText(valueWindow.operator[](LUNDCOL), fmtStr.c_str());
			form::refilfn(ClosestFormToCursor);
			thred::coltab();
		  }
		  thred::unsid();
		  Instance->stateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LBRDCOL: { // update border color
		  if (value != 0.0F) {
			thred::savdo();
			auto const colVal = gsl::narrow_cast<uint8_t>(
			    (wrap::wcsToLong<uint32_t>(sideWindowEntryBuffer.data()) - 1U) & COLMSK);
			form::nubrdcol(colVal);
			auto const fmtStr = format(FMT_COMPILE(L"{}"), colVal + 1U);
			SetWindowText(valueWindow.operator[](LBRDCOL), fmtStr.c_str());
			thred::coltab();
		  }
		  thred::unsid();
		  Instance->stateMap.set(StateFlag::RESTCH);
		  return;
		}
		case LBRDPIC: { // update picot spacing
		  thred::savdo();
		  form.edgeSpacing = value;
		  thred::unsid();
		  setSideWinVal(LBRDPIC);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LFRMFAZ: { // update clip phase angle
		  thred::savdo();
		  form.wordParam = wrap::floor<uint32_t>(value * IPFGRAN);
		  thred::unsid();
		  setSideWinVal(LFRMFAZ);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LBRDPOS: { // update chain position
		  thred::savdo();
		  form.edgeStitchLen = value * IPFGRAN;
		  thred::unsid();
		  setSideWinVal(LBRDPOS);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMAXFIL: { // update max fill stitch length
		  thred::savdo();
		  form.maxFillStitchLen = value;
		  thred::unsid();
		  setSideWinVal(LMAXFIL);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMINFIL: { // update min fill stitch length
		  thred::savdo();
		  form.minFillStitchLen = value;
		  thred::unsid();
		  setSideWinVal(LMINFIL);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMAXBRD: { // update max border stitch length
		  thred::savdo();
		  form.maxBorderStitchLen = value;
		  thred::unsid();
		  setSideWinVal(LMAXBRD);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMINBRD: { // update min border stitch length
		  thred::savdo();
		  form.minBorderStitchLen = value;
		  thred::unsid();
		  setSideWinVal(LMINBRD);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		default: {
		  outDebugString(L"default hit in chknum 1: FormMenuChoice [{}]\n", FormMenuChoice);
		  break;
		}
	  }
	  if (FormMenuChoice == LBCSIZ) { // update buttonhole corner size or picot length
		thred::savdo();
		if (form.edgeType == EDGEBHOL) {
		  form::savblen(value);
		}
		else {
		  form::savplen(value);
		}
	  }
	  else {
		if (value != 0.0F) {
		  switch (FormMenuChoice) {
			case LFTHSIZ: { // update feather size
			  thred::savdo();
			  form.feather.ratio = value * IPFGRAN;
			  break;
			}
			case LFTHNUM: { // update feather count
			  thred::savdo();
			  form.feather.count = wrap::round<uint16_t>(value * IPFGRAN);
			  break;
			}
			case LFTHFLR: { // update feather min stitch size
			  thred::savdo();
			  form.feather.minStitchSize = value;
			  break;
			}
			case LFTHDWNCNT: { // update feather down count
			  thred::savdo();
			  auto           dncnt    = value * IPFGRAN;
			  constexpr auto FDNCLAMP = 255.0F; // clamp the feather down count

			  dncnt                  = std::min(dncnt, FDNCLAMP);
			  form.feather.downCount = wrap::round<uint8_t>(dncnt);
			  break;
			}
			case LFRMSPAC: { // update fill spacing
			  thred::savdo();
			  form.fillSpacing = value;
			  break;
			}
			case LFRMLEN: { // update fill stitch length
			  thred::savdo();
			  form.stitchLength = value;
			  break;
			}
			case LBRDSPAC: { // update border spacing
			  thred::savdo();
			  switch (auto const edgeType = form.edgeType & NEGUND; edgeType) {
				case EDGEPROPSAT:
				case EDGEOCHAIN:
				case EDGELCHAIN: {
				  form.edgeSpacing = value;
				  break;
				}
				default: {
				  form.edgeSpacing = value * HALF;
				}
			  }
			  break;
			}
			case LBRDLEN: { // update border stitch length
			  thred::savdo();
			  form.edgeStitchLen = value;
			  break;
			}
			case LBRDSIZ: { // update border width
			  thred::savdo();
			  form.borderSize = value;
			  break;
			}
			case LFRMANG: { // update fill angle
			  thred::savdo();
			  form.fillAngle = value * DEGRADF * IPFGRAN;
			  break;
			}
			case LSACANG: { // update clip fill angle
			  thred::savdo();
			  form.clipFillAngle = value * DEGRADF * IPFGRAN;
			  break;
			}
			case LAPCOL: { // update applique color
			  thred::savdo();
			  form.borderColor &= COLMSK;
			  auto borderColor = wrap::round<uint8_t>(value * IPFGRAN);
			  if (borderColor != 0U) {
				--borderColor;
			  }
			  borderColor &= COLMSK;
			  form.borderColor |= gsl::narrow_cast<decltype(form.borderColor)>(borderColor << 4U);
			  break;
			}
			default: {
			  outDebugString(L"default hit in chknum 2: FormMenuChoice [{}]\n", FormMenuChoice);
			  break;
			}
		  }
		}
		else {
		  thred::savdo();
		  if (FormMenuChoice == LFRMSPAC && form.isFanClip()) { // update fan clip spacing
			form.fillSpacing = 0;
		  }
		}
	  }
	  thred::unsid();
	  form::refil(ClosestFormToCursor);
	  formForms::refrm();
	}
	else {
	  if (PreferenceIndex != 0U) { // the preference menu is open
		auto const value = wrap::wcsToFloat(sideWindowEntryBuffer.data());
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto hWnd   = HWND {nullptr};
		auto fmtStr = std::wstring {};
		switch (PreferenceIndex - 1) {
		  case PRFEGGRAT: {
			IniFile.eggRatio = value;
			fmtStr           = format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd             = valueWindow.operator[](PRFEGGRAT);
			break;
		  }
		  case PRFNUGSTP: {
			IniFile.cursorNudgeStep = value;
			IniFile.nudgePixels     = pxchk(value);
			fmtStr                  = format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd                    = valueWindow.operator[](PRFNUGSTP);
			break;
		  }
		  case PRFPCTSPC: {
			PicotSpacing = value * PFGRAN;
			fmtStr       = format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd         = valueWindow.operator[](PRFPCTSPC);
			break;
		  }
		  case PRFCLPOFF: {
			IniFile.clipOffset = value * PFGRAN;
			fmtStr             = format(FMT_COMPILE(L"{:.2f} mm"), value);
			hWnd               = valueWindow.operator[](PRFCLPOFF);
			break;
		  }
		  case PRFCLPPHS: {
			IniFile.fillPhase = wrap::floor<uint32_t>(value);
			fmtStr            = format(FMT_COMPILE(L"{}"), IniFile.fillPhase);
			hWnd              = valueWindow.operator[](PRFCLPPHS);
			break;
		  }
		  case PRFCHFPOS: {
			IniFile.chainRatio = value;
			fmtStr             = format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd               = valueWindow.operator[](PRFCHFPOS);
			break;
		  }
		  case PRFSTCMIN: {
			MinStitchLength = value * PFGRAN;
			fmtStr          = format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd            = valueWindow.operator[](PRFSTCMIN);
			break;
		  }
		  default: {
			if (value != 0.0F) {
			  fmtStr = format(FMT_COMPILE(L"{:.2f}"), value);
			  switch (PreferenceIndex - 1) {
				case PRFFILSPC: {
				  LineSpacing = value * PFGRAN;
				  hWnd        = valueWindow.operator[](PRFFILSPC);
				  break;
				}
				case PRFFILANG: {
				  IniFile.fillAngle = value * DEGRADF;
				  hWnd              = valueWindow.operator[](PRFFILANG);
				  break;
				}
				case PRFBRDWID: {
				  BorderWidth         = value * PFGRAN;
				  IniFile.borderWidth = BorderWidth;
				  hWnd                = valueWindow.operator[](PRFBRDWID);
				  break;
				}
				case PRFSTCMAX: {
				  IniFile.maxStitchLength = value * PFGRAN;
				  hWnd                    = valueWindow.operator[](PRFSTCMAX);
				  break;
				}
				case PRFSTCUSR: {
				  UserStitchLength = value * PFGRAN;
				  hWnd             = valueWindow.operator[](PRFSTCUSR);
				  break;
				}
				case PRFSMLSTH: {
				  SmallStitchLength = value * PFGRAN;
				  hWnd              = valueWindow.operator[](PRFSMLSTH);
				  break;
				}
				case PRFAPPCOL: {
				  AppliqueColor = wrap::round<uint32_t>(value - 1.0F) % COLORCNT;
				  fmtStr        = format(FMT_COMPILE(L"{}"), AppliqueColor + 1U);
				  hWnd          = valueWindow.operator[](PRFAPPCOL);
				  break;
				}
				case PRFAPSLEN: {
				  IniFile.AppStitchLen = value * PFGRAN;
				  hWnd                 = valueWindow.operator[](PRFAPSLEN);
				  break;
				}
				case PRFSNPSIZ: {
				  SnapLength = value * PFGRAN;
				  hWnd       = valueWindow.operator[](PRFSNPSIZ);
				  break;
				}
				case PRFSTRRAT: {
				  StarRatio = value;

				  constexpr auto SRMINLIM = 0.01F; // star ratio minimum limit
				  constexpr auto SRMAXLIM = 1.0F;  // star ratio maximum limit

				  StarRatio = std::clamp(StarRatio, SRMINLIM, SRMAXLIM);
				  fmtStr    = format(FMT_COMPILE(L"{:.2f}"), StarRatio);
				  hWnd      = valueWindow.operator[](PRFSTRRAT);
				  break;
				}
				case PRFLENRAT: {
				  IniFile.lensRatio = value;

				  constexpr auto LRMINLIM = 0.1F;  // lens ratio minimum limit
				  constexpr auto LRMAXLIM = 10.0F; // lens ratio maximum limit

				  IniFile.lensRatio = std::clamp(IniFile.lensRatio, LRMINLIM, LRMAXLIM);
				  fmtStr            = format(FMT_COMPILE(L"{:.2f}"), IniFile.lensRatio);
				  hWnd              = valueWindow.operator[](PRFLENRAT);
				  break;
				}
				case PRFSPLWRP: {
				  SpiralWrap = value;
				  // ToDo - Are these limits correct?
				  constexpr auto SRMINLIM = 0.3F;  // spiral wrap minimum limit
				  constexpr auto SRMAXLIM = 20.0F; // spiral wrap maximum limit

				  SpiralWrap = std::clamp(SpiralWrap, SRMINLIM, SRMAXLIM);
				  fmtStr     = format(FMT_COMPILE(L"{:.2f}"), SpiralWrap);
				  hWnd       = valueWindow.operator[](PRFSPLWRP);
				  break;
				}
				case PRFBCNLEN: {
				  ButtonholeCornerLength = value * PFGRAN;
				  fmtStr                 = format(FMT_COMPILE(L"{:.2f}"), value);
				  hWnd                   = valueWindow.operator[](PRFBCNLEN);
				  break;
				}
				case PRFHUPWID: {
				  IniFile.hoopSizeX = value * PFGRAN;
				  fmtStr            = format(FMT_COMPILE(L"{:.0f} mm"), value);
				  hWnd              = valueWindow.operator[](PRFHUPWID);
				  form::sethup();
				  formForms::prfmsg();
				  thred::chkhup();
				  break;
				}
				case PRFHUPHGT: {
				  IniFile.hoopSizeY = value * PFGRAN;
				  fmtStr            = format(FMT_COMPILE(L"{:.0f} mm"), value);
				  hWnd              = valueWindow.operator[](PRFHUPHGT);
				  form::sethup();
				  formForms::prfmsg();
				  thred::chkhup();
				  break;
				}
				case PRFGRDSIZ: {
				  IniFile.gridSize = value * PFGRAN;
				  fmtStr           = format(FMT_COMPILE(L"{:.2f} mm"), value);
				  hWnd             = valueWindow.operator[](PRFGRDSIZ);
				  break;
				}
				case PRFCHFLEN: {
				  IniFile.chainSpace = value * PFGRAN;
				  fmtStr             = format(FMT_COMPILE(L"{:.2f}"), value);
				  hWnd               = valueWindow.operator[](PRFCHFLEN);
				  break;
				}
				default: {
				  outDebugString(L"default hit in chknum 3: PreferenceIndex [{}]\n", PreferenceIndex - 1);
				  break;
				}
			  }
			}
		  }
		}
		if (nullptr != hWnd && !fmtStr.empty()) {
		  SetWindowText(hWnd, fmtStr.c_str());
		}
		thred::unsid();
		PreferenceIndex = 0;
	  }
	}
  }
  if (ThrSingle->MsgBuffer.size() > 1) {
	outDebugString(L"chknum: buffer length [{}] size [{}]\n",
	               wcslen(ThrSingle->MsgBuffer.data()),
	               ThrSingle->MsgBuffer.size());
	auto const value = thred::getMsgBufferValue();
	if (Instance->stateMap.testAndReset(StateFlag::NUROT)) { // rotate
	  if (value != 0.0F) {
		IniFile.rotationAngle = value * DEGRADF;
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTRSEG)) { // rotation segment
	  if (value != 0.0F) {
		IniFile.rotationAngle = PI_F2 / value;
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTRFNUM)) { // form number
	  if (wrap::round<uint32_t>(value) < formList.size()) {
		form::frmnumfn(ClosestFormToCursor, wrap::round<uint32_t>(value));
	  }
	  else {
		displayText::tabmsg(IDS_FRMN1, false);
	  }
	  return;
	}
	auto const uintValue = wrap::floor<uint32_t>(std::abs(value));
	if (Instance->stateMap.testAndReset(StateFlag::ENTRPOL)) { // number of sides
	  thred::savdo();
	  form::durpoli(uintValue);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTRSTAR)) { // number of points in star
	  thred::savdo();
	  constexpr auto STARSIZE = 250.0F; // star size factor
	  form::dustar(uintValue,
	               STARSIZE / value * ZoomFactor *
	                   wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY));
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTRSPIR)) { // number of spirals
	  thred::savdo();
	  form::duspir(uintValue);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTRHART)) { // number of heart facets
	  thred::savdo();
	  form::duhart(uintValue);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTRLENS)) { // number of lens facets
	  thred::savdo();
	  form::dulens(uintValue);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTREG)) { // number of egg facets
	  thred::savdo();
	  form::dueg(uintValue);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTRZIG)) { // number of zigzags
	  thred::savdo();
	  form::duzig(uintValue);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::PIXIN)) { // nudge pixel size
	  IniFile.nudgePixels = pxchk(value);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::STPXIN)) { // stitch point pixel size
	  IniFile.stitchSizePixels = pxchk(value);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FRMPXIN)) { // form vertex pixel size
	  IniFile.formVertexSizePixels = wrap::round<uint16_t>(value);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FRMBOXIN)) { // form box pixel size
	  IniFile.formBoxSizePixels = wrap::round<uint16_t>(value);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::GETMIN)) { // minimum stitch length
	  SmallStitchLength = value * PFGRAN;
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTR30)) { // thread size for 30 weight
	  ThreadSize30 = value;
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTR40)) { // thread size for 40 weight
	  ThreadSize40 = value;
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::ENTR60)) { // thread size for 60 weight
	  ThreadSize60 = value;
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::SCLPSPAC)) { // clipboard fill spacing
	  IniFile.clipOffset = value * PFGRAN;
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFIND)) { // underlay indent
	  xt::dufind(value);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFHI)) { // edit form height
	  if (value != 0.0F) {
		xt::dufhi(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFWID)) { // edit form width
	  if (value != 0.0F) {
		xt::dufwid(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFMAX)) { // edit form max stitch length
	  if (value != 0.0F) {
		xt::dufmax(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFMIN)) { // edit form min stitch length
	  xt::dufmin(value * PFGRAN);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETBMAX)) { // edit border max stitch length
	  if (value != 0.0F) {
		xt::dubmax(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETBMIN)) { // edit border min stitch length
	  xt::dubmin(value * PFGRAN);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETBSPAC)) { // edit border spacing
	  if (value != 0.0F) {
		xt::dubspac(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFLEN)) { // edit form stitch length
	  if (value != 0.0F) {
		xt::dublen(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETBCOL)) { // edit border color
	  xt::dubcol(wrap::round<uint8_t>(value));
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFCOL)) { // edit form fill color
	  xt::dufcol(wrap::round<uint8_t>(value));
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETUCOL)) { // edit form underlay color
	  xt::dundcol(wrap::round<uint8_t>(value));
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFANG)) { // edit form fill angle
	  xt::dufxang(value);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFSPAC)) { // edit form fill spacing
	  if (value != 0.0F) {
		xt::dufspac(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETUANG)) { // edit form underlay angle
	  xt::dufang(value);
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETFLEN)) { // edit form fill stitch length
	  if (value != 0.0F) {
		xt::duflen(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETUSPAC)) { // edit form underlay spacing
	  if (value != 0.0F) {
		xt::duspac(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETULEN)) { // edit form underlay stitch length
	  if (value != 0.0F) {
		xt::dusulen(value * PFGRAN);
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::GTUANG)) { // default underlay angle
	  IniFile.underlayAngle = value * DEGRADF;
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::GTUSPAC)) { // default underlay spacing
	  if (value != 0.0F) {
		IniFile.underlaySpacing = value * PFGRAN;
	  }
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::GTWLKIND)) { // default underlay indent
	  IniFile.underlayIndent = value * PFGRAN;
	  return;
	}
	if (Instance->stateMap.testAndReset(StateFlag::GTWLKLEN)) { // default underlay stitch length
	  if (value != 0.0F) {
		IniFile.underlayStitchLen = value * PFGRAN;
	  }
	  return;
	}
  }

  Instance->stateMap.set(StateFlag::RESTCH);
}

auto chkup(uint32_t const count, size_t const iStitch) -> uint32_t {
  if (!Instance->stateMap.test(StateFlag::UPTO) || ClosestPointIndex == 0U) {
	return count;
  }
  auto const& colorChangeTable = ThrSingle->ColorChangeTable;
  if (colorChangeTable.operator[](iStitch).stitchIndex >= ClosestPointIndex) {
	return 0U;
  }
  if (colorChangeTable.operator[](iStitch + 1U).stitchIndex >= ClosestPointIndex) {
	return ClosestPointIndex - colorChangeTable.operator[](iStitch).stitchIndex + 1U;
  }
  return count;
}

void clpradj(F_RECTANGLE& clipRectAdjusted, F_POINT_ATTR const& stitch) noexcept {
  clipRectAdjusted.left   = std::min(stitch.x, clipRectAdjusted.left);
  clipRectAdjusted.right  = std::max(stitch.x, clipRectAdjusted.right);
  clipRectAdjusted.bottom = std::min(stitch.y, clipRectAdjusted.bottom);
  clipRectAdjusted.top    = std::max(stitch.y, clipRectAdjusted.top);
}

auto cmpstch(uint32_t const iStitchA, uint32_t const iStitchB) noexcept -> bool {
  return util::closeEnough(Instance->stitchBuffer.operator[](iStitchA).x,
                           Instance->stitchBuffer.operator[](iStitchB).x)
             ? util::closeEnough(Instance->stitchBuffer.operator[](iStitchA).y,
                                 Instance->stitchBuffer.operator[](iStitchB).y)
             : false;
}

void createBrushes() noexcept {
  auto dcb         = DefaultColorBrush.begin();
  auto ucb         = UserColorBrush.begin();
  auto itUserColor = UserColor.begin();
  for (auto const& color : DEFAULT_COLORS) {
	*dcb++ = CreateSolidBrush(color);
	*ucb++ = CreateSolidBrush(*itUserColor++);
  }
}

void cros(uint32_t const iStitch) {
  auto const armLength            = BoxOffset[0];
  auto const stitchCoordsInPixels = thred::stch2px1(iStitch);
  InsertLine[0] = {.x = stitchCoordsInPixels.x - armLength, .y = stitchCoordsInPixels.y};
  InsertLine[1] = {.x = stitchCoordsInPixels.x + armLength, .y = stitchCoordsInPixels.y};
  SelectObject(StitchWindowDC, CrossPen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SelectObject(StitchWindowMemDC, CrossPen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  InsertLine[0] = {.x = stitchCoordsInPixels.x, .y = stitchCoordsInPixels.y - armLength};
  InsertLine[1] = {.x = stitchCoordsInPixels.x, .y = stitchCoordsInPixels.y - 1};
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  wrap::polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  InsertLine[0].y = stitchCoordsInPixels.y + 2;
  InsertLine[1].y = stitchCoordsInPixels.y + armLength;
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
  wrap::polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void defNam(fs::path const& fileName) {
  if (fileName.empty()) {
	getDocsFolder(ThrSingle->DefaultDirectory);
	return;
  }
  ThrSingle->DefaultDirectory = fileName.parent_path();
}

auto defTxt(uint32_t const iColor) -> COLORREF {
  // bitmap for color number. Black or white bit chosen for contrast against the default background colors
  constexpr auto TEXT_COLOR_MAP = std::bitset<16>(0xbaf);
  return TEXT_COLOR_MAP.test(iColor) ? PENWHITE : PENBLK;
}

void delcol() {
  if (thred::usedcol(VerticalIndex)) {
	displayText::tabmsg(IDS_COLU, false);
	return;
  }
  for (auto& stitch : Instance->stitchBuffer) {
	auto const color = stitch.attribute & COLMSK;
	if (color <= VerticalIndex) {
	  continue;
	}
	stitch.attribute &= NCOLMSK;
	stitch.attribute |= color - 1U;
  }
  for (auto& iForm : Instance->formList) {
	if (iForm.fillType != 0U && iForm.fillColor > VerticalIndex) {
	  --iForm.fillColor;
	}
	if (iForm.fillType == FTHF && iForm.feather.color > VerticalIndex) {
	  --iForm.feather.color;
	}
	if (iForm.edgeType != 0U && iForm.borderColor > VerticalIndex) {
	  --iForm.borderColor;
	}
  }
  for (auto iColor = VerticalIndex; iColor < COLORMAX; ++iColor) {
	UserColor.at(iColor) = UserColor.at(wrap::toSize(iColor) + 1U);
	thred::nuscol(iColor);
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void delfstchs() {
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  // find the first stitch to delete
  if (auto const firstStitch = std::ranges::find_if(Instance->stitchBuffer,
                                                    [codedForm](F_POINT_ATTR const& stitch) -> bool {
	                                                  return (stitch.attribute & (FRMSK | NOTFRM)) == codedForm;
                                                    });
      firstStitch != Instance->stitchBuffer.end()) {
	// we found the first stitch, so now delete the stitches in the form
	Instance->stitchBuffer.erase(std::remove_if(firstStitch,
	                                            Instance->stitchBuffer.end(),
	                                            [codedForm](F_POINT_ATTR const& stitch) -> bool {
	                                              return (stitch.attribute & FRMSK) == codedForm;
	                                            }),
	                             Instance->stitchBuffer.end());
  }
}

void delknt() {
  // delete the knot stitches
  std::erase_if(Instance->stitchBuffer,
                [](F_POINT_ATTR const& stitch) -> bool { return (stitch.attribute & KNOTMSK) != 0U; });
}

void delsmal(uint32_t const startStitch, uint32_t const endStitch) {
  auto const codedAttribute = ClosestFormToCursor << FRMSHFT;
  auto stitchSize = BIGFLOAT; // to ensure that it is larger than SmallStitchLength first time through
  auto const minStitchLength = SmallStitchLength * SmallStitchLength;
  thred::savdo();
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto iPrevStitch = find1st();
	auto iStitch     = iPrevStitch + 1U;
	auto lastStitch  = Instance->stitchBuffer.size();
	--lastStitch;
	while (iStitch < lastStitch && stitchSize > minStitchLength) {
	  auto const& stitch     = Instance->stitchBuffer.operator[](iStitch);
	  auto const& prevStitch = Instance->stitchBuffer.operator[](iPrevStitch);
	  if ((stitch.attribute & NOTFRM) != 0U ||
	      (stitch.attribute & FRMSK) != codedAttribute) { // are we still in the selected form?
		return; // we reached the last stitch in the selected form without seeing a small stitch so don't do anything
	  }
	  if ((stitch.attribute & KNOTMSK) != 0U) { // is this a normal stitch?
		continue;
	  }
	  auto const deltaX = stitch.x - prevStitch.x;
	  auto const deltaY = stitch.y - prevStitch.y;
	  stitchSize        = deltaX * deltaX + deltaY * deltaY;
	  if (stitchSize > minStitchLength) {
		++iPrevStitch;
		++iStitch;
	  }
	}
	if (iStitch != lastStitch) {
	  --iStitch; // we found a small stich so back up to the start point
	}
	else {
	  return; // we reached the last stitch without seeing a small stitch so don't do anything
	}
	auto iOutputStitch = iStitch;
	auto prevPoint     = Instance->stitchBuffer.operator[](startStitch); // intentional copy
	lastStitch         = Instance->stitchBuffer.size();
	if (lastStitch != 0U) {
	  --lastStitch;
	  while (iStitch < lastStitch) {
		auto const& stitch    = Instance->stitchBuffer.operator[](iStitch);
		auto&       outStitch = Instance->stitchBuffer.operator[](iOutputStitch);
		if ((stitch.attribute & NOTFRM) == 0U && (stitch.attribute & FRMSK) == codedAttribute) { // are we still in the selected form?
		  if ((stitch.attribute & KNOTMSK) != 0U) { // is this a knot?
			prevPoint = outStitch;
			outStitch = stitch;
			++iOutputStitch;
		  }
		  else {
			auto const deltaX = stitch.x - prevPoint.x;
			auto const deltaY = stitch.y - prevPoint.y;
			stitchSize        = deltaX * deltaX + deltaY * deltaY;
			if (stitchSize > minStitchLength) {
			  outStitch = stitch;
			  prevPoint = stitch;
			  ++iOutputStitch;
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
		++iStitch;
	  }
	}
	++iOutputStitch;
	Instance->stitchBuffer.erase(wrap::next(Instance->stitchBuffer.begin(), iOutputStitch),
	                             wrap::next(Instance->stitchBuffer.begin(), iStitch));
	thred::coltab();
  }
  else {
	auto iNextStitch = startStitch + 1U;
	auto prevPoint   = Instance->stitchBuffer.operator[](startStitch); // intentional copy
	for (auto iStitch = iNextStitch; iStitch < endStitch; ++iStitch) {
	  auto const& stitch = Instance->stitchBuffer.operator[](iStitch);
	  if ((Instance->stitchBuffer.operator[](iNextStitch).attribute & KNOTMSK) != 0U) {
		prevPoint = Instance->stitchBuffer.operator[](iNextStitch);

		Instance->stitchBuffer.operator[](iNextStitch++) = stitch;
	  }
	  else {
		auto const deltaX = stitch.x - prevPoint.x;
		auto const deltaY = stitch.y - prevPoint.y;
		stitchSize        = deltaX * deltaX + deltaY * deltaY;
		if (stitchSize > minStitchLength) {
		  Instance->stitchBuffer.operator[](iNextStitch++) = stitch;
		  prevPoint                                        = stitch;
		}
	  }
	}
	Instance->stitchBuffer.erase(wrap::next(Instance->stitchBuffer.begin(), iNextStitch),
	                             wrap::next(Instance->stitchBuffer.begin(), endStitch));
	thred::coltab();
  }
  thred::rstAll();
  thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
  thred::lenCalc();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void delstch1(uint32_t const iStitch) {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  Instance->stitchBuffer.erase(wrap::next(Instance->stitchBuffer.begin(), iStitch));
  ClosestPointIndex = std::min(ClosestPointIndex, wrap::toUnsigned(Instance->stitchBuffer.size()) - 1U);
}

void destroyBV() noexcept {
  for (auto& iBackup : BackupViewer) {
	DestroyWindow(iBackup);
	iBackup = nullptr;
  }
}

// ReSharper disable CppParameterMayBeConst
auto CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  UNREFERENCED_PARAMETER(lparam);
  auto& designerName = ThrSingle->DesignerName;
  switch (umsg) {
	case WM_INITDIALOG: {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const hwnd = GetDlgItem(hwndlg, IDC_DESED);
	  SetWindowText(hwnd, designerName.c_str());
	  SetFocus(hwnd);
	  SendMessage(hwnd, EM_SETSEL, 0, -1);
	  break;
	}
	case WM_COMMAND: {
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, FALSE);
		  return TRUE;
		}
		case IDOK: {
		  // NOLINTNEXTLINE(readability-qualified-auto)
		  auto const hwnd           = GetDlgItem(hwndlg, IDC_DESED);
		  auto       designerBuffer = std::array<wchar_t, NameOrder.size()> {};
		  GetWindowText(hwnd, designerBuffer.data(), gsl::narrow<int>(designerBuffer.size()));
		  designerName.assign(designerBuffer.data());
		  EndDialog(hwndlg, 0);
		  auto const fmtStr = displayText::format(IDS_THRED, designerName);
		  SetWindowText(ThrEdWindow, fmtStr.c_str());
		  EndDialog(hwndlg, TRUE);
		  break;
		}
		default: {
		  outDebugString(L"default hit in dnamproc 1: wparam [{}]\n", LOWORD(wparam));
		  break;
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in dnamproc 2: umsg [{}]\n", umsg);
	  break;
	}
  }
  return FALSE;
}
// ReSharper restore CppParameterMayBeConst

void doDrwInit() {
  setScrollVisibility();
  thred::duzrat();
  auto const dub6        = ZoomRatio.x * 6.0F;
  auto const threadWidth = std::array<int32_t, 3> {
      std::lround(dub6 * TSIZ30), std::lround(dub6 * TSIZ40), std::lround(dub6 * TSIZ60)}; // thread sizes in pixels
  auto tsi = ThreadSizeIndex.begin();
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	auto width = int32_t {1};
	if (Instance->stateMap.test(StateFlag::THRDS) ||
	    (iColor == ActiveColor && Instance->stateMap.test(StateFlag::COL))) {
	  width = *wrap::next(threadWidth.begin(), *tsi);
	}
	nuStchSiz(iColor, width);
	++tsi;
  }
  DisplayedColorBitmap.reset();
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	doInitZoomed();
  }
  else {
	doInitUnzoomed();
  }
  if (Instance->stateMap.test(StateFlag::SELBOX)) { // if we have selected a box which has stitches
	if (!Instance->stitchBuffer.empty()) {
	  ritcor(Instance->stitchBuffer.operator[](ClosestPointIndex));
	  if (auto stitchCoordsInPixels = POINT {}; thred::stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
		thred::dubox(stitchCoordsInPixels);
	  }
	}
  }
  if (Instance->stateMap.test(StateFlag::FRMPSEL)) { // if we have selected a point in a form
	auto const itVertex =
	    wrap::next(Instance->formVertices.cbegin(),
	               Instance->formList.operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	thred::ritfcor(*itVertex);
  }
  if (!Instance->stateMap.test(StateFlag::SELBOX) && !Instance->stateMap.test(StateFlag::FRMPSEL)) {
	auto const blank = std::wstring {};
	displayText::butxt(HCOR, blank);
  }
  if (Instance->stateMap.test(StateFlag::WASLIN)) {
	relin();
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) { // if we have selected a group of stitches
	if (cmpstch(ClosestPointIndex, GroupStitchIndex)) {
	  cros(ClosestPointIndex);
	}
	else {
	  Instance->searchLine.clear();
	  ducros(StitchWindowMemDC);
	}
	thred::selRct(StitchRangeRect);
	nuselrct();
	Instance->stateMap.set(StateFlag::SELSHO);
	dusel(StitchWindowMemDC);
  }
  if (ZoomFactor < StitchBoxesThreshold) { // do we need to draw the stitch boxes?
	drawStitchBoxes();
  }
  if (Instance->stateMap.test(StateFlag::CLPSHO)) {
	duclp();
  }
}

void doInitUnzoomed() {
  auto linePoints = std::vector<POINT> {};
  linePoints.reserve(wrap::toSize(getMaxCount()) + 2U);
  auto const  pwid             = Instance->stateMap.test(StateFlag::HID);
  auto const& colorChangeTable = ThrSingle->ColorChangeTable;
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	DisplayedColorBitmap.set(colorChangeTable.operator[](iColor).colorIndex);
	auto stitchCount = wrap::toUnsigned(colorChangeTable.operator[](iColor + 1U).stitchIndex) -
	                   colorChangeTable.operator[](iColor).stitchIndex;
	stitchCount = chkup(stitchCount, iColor);
	if (!pwid || colorChangeTable.operator[](iColor).colorIndex == ActiveColor) {
	  drwLin(linePoints,
	         colorChangeTable.operator[](iColor).stitchIndex,
	         stitchCount,
	         ThrSingle->UserPen.operator[](colorChangeTable.operator[](iColor).colorIndex));
	}
  }
}

void doInitZoomed() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  auto stitchCount = getMaxCount();
  auto linePoints  = std::vector<POINT> {};
  linePoints.reserve(wrap::toSize(stitchCount) + 2U);
  Instance->stateMap.reset(StateFlag::LINED);
  Instance->stateMap.reset(StateFlag::LININ);
  // intentional copy of first stitch to initialize previous stitch
  auto prevStitch = Instance->stitchBuffer.front();
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) { // iterate through all colors
	auto& colorChangeTable = ThrSingle->ColorChangeTable;
	if (Instance->stateMap.test(StateFlag::HID) && colorChangeTable.operator[](iColor).colorIndex != ActiveColor) {
	  stitchCount = colorChangeTable.operator[](iColor + 1U).stitchIndex -
	                colorChangeTable.operator[](iColor).stitchIndex;
	  auto const sStart =
	      wrap::next(Instance->stitchBuffer.begin(), colorChangeTable.operator[](iColor).stitchIndex);
	  auto const sEnd = wrap::next(sStart, stitchCount);
	  // and check if the color is to be displayed in the zoomed view
	  for (auto sRange = std::ranges::subrange(sStart, sEnd); auto const& iStitch : sRange) {
		if (iStitch.x >= ZoomRect.left && iStitch.x <= ZoomRect.right &&
		    iStitch.y >= ZoomRect.bottom && iStitch.y <= ZoomRect.top) {
		  DisplayedColorBitmap.set(colorChangeTable.operator[](iColor).colorIndex);
		  break;
		}
	  }
	  continue;
	}
	auto wascol = 0U;
	SelectObject(StitchWindowMemDC,
	             ThrSingle->UserPen.operator[](colorChangeTable.operator[](iColor).colorIndex));
	stitchCount = colorChangeTable.operator[](iColor + 1U).stitchIndex -
	              colorChangeTable.operator[](iColor).stitchIndex;
	auto const stIndex = colorChangeTable.operator[](iColor).stitchIndex;

	auto const stStart   = wrap::next(Instance->stitchBuffer.begin(), stIndex);
	stitchCount          = chkup(stitchCount, iColor);
	auto const stitches  = std::ranges::subrange(stStart, wrap::next(stStart, stitchCount));
	auto const maxYcoord = wrap::toFloat(DrawItem->rcItem.bottom);
	for (auto& iStitch : stitches) { // iterate through all stitches of the current color
	  if (auto const layer = (iStitch.attribute & LAYMSK) >> LAYSHFT;
	      ActiveLayer != 0U && layer != 0U && layer != ActiveLayer) {
		continue;
	  }
	  if (iStitch.x >= ZoomRect.left && iStitch.x <= ZoomRect.right && iStitch.y >= ZoomRect.bottom &&
	      iStitch.y <= ZoomRect.top) { // if the stitch is in the zoomed view
		wascol = 1;
		if (Instance->stateMap.testAndSet(StateFlag::LINED)) {
		  if (Instance->stateMap.testAndSet(StateFlag::LININ)) {
			linePoints.push_back(
			    {.x = wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			     .y = wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.y))});
		  }
		  else {
			if (iStitch == prevStitch) {
			  linePoints.push_back(
			      {.x = wrap::ceil<int32_t>((stStart->x - ZoomRect.left) * ZoomRatio.x),
			       .y = wrap::ceil<int32_t>(maxYcoord - ((stStart->y - ZoomRect.bottom) * ZoomRatio.y))});
			}
			else {
			  linePoints.push_back(
			      {.x = wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
			       .y =
			           wrap::ceil<int32_t>(maxYcoord - ((prevStitch.y - ZoomRect.bottom) * ZoomRatio.y))});
			  linePoints.push_back(
			      {.x = wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			       .y = wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.y))});
			}
		  }
		}
		else {
		  if (iStitch == prevStitch) {
			linePoints.push_back(
			    {.x = wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			     .y = wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.y))});
		  }
		  else {
			linePoints.push_back(
			    {.x = wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
			     .y = wrap::ceil<int32_t>(maxYcoord - ((prevStitch.y - ZoomRect.bottom) * ZoomRatio.y))});
			linePoints.push_back(
			    {.x = wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			     .y = wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.y))});
		  }
		  Instance->stateMap.set(StateFlag::LININ);
		}
	  }
	  else { // the stitch is not in the zoomed view
		if (Instance->stateMap.testAndReset(StateFlag::LININ)) {
		  linePoints.push_back(
		      {.x = wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
		       .y = wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.y))});
		  wrap::polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
		  linePoints.clear();
		}
		else {
		  if (iStitch == prevStitch) {
			continue;
		  }
		  drawOffScreenLine(iStitch, prevStitch, maxYcoord);
		}
	  }
	  prevStitch = iStitch;
	}
	if (!linePoints.empty()) { // if we have a set of stitches t0 draw
	  wrap::polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
	  // intentional copy being made here to save last point
	  auto const lastPoint = linePoints.back();
	  linePoints.clear();
	  linePoints.push_back(lastPoint);
	}
	if (wascol != 0U) {
	  DisplayedColorBitmap.set(colorChangeTable.operator[](iColor).colorIndex);
	}
  }
}

// https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
void doStretch(uint32_t const start, uint32_t const end) {
  auto       firstStitch  = wrap::next(Instance->stitchBuffer.begin(), start);
  auto       centerStitch = std::next(firstStitch);
  auto       lastStitch   = std::next(centerStitch);
  auto const endStitch    = wrap::next(Instance->stitchBuffer.begin(), end);
  thred::savdo();
  while (lastStitch != endStitch) {
	if ((firstStitch->attribute & COLMSK) == (centerStitch->attribute & COLMSK) &&
	    (centerStitch->attribute & COLMSK) == (lastStitch->attribute & COLMSK)) {
	  constexpr auto FACTOR = 0.1F;
	  auto const delta1 = F_POINT {centerStitch->x - firstStitch->x, centerStitch->y - firstStitch->y};
	  auto const point  = F_POINT {lastStitch->x - delta1.x, lastStitch->y - delta1.y};
	  auto const delta3 = F_POINT {centerStitch->x - point.x, centerStitch->y - point.y};
	  centerStitch->x += delta3.x * FACTOR;
	  centerStitch->y += delta3.y * FACTOR;
	}
	++firstStitch;
	++centerStitch;
	++lastStitch;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void drawBackground() {
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
  thred::duzrat();
  if (bitmap::ismap() && !Instance->stateMap.test(StateFlag::HIDMAP) &&
      !Instance->stateMap.test(StateFlag::UPTO)) {
	bitmap::drawBmpBackground();
  }
  dugrid();
}

void drawOffScreenLine(const F_POINT_ATTR& iStitch, const F_POINT_ATTR& prevStitch, const float& maxYcoord) {
  // write an equation for this line
  auto const xDelta     = iStitch.x - prevStitch.x;
  auto const yDelta     = prevStitch.y - iStitch.y;
  auto const slope      = xDelta / yDelta;
  auto const offset     = iStitch.x + (slope * iStitch.y);
  auto       stitchLine = std::array<POINT, LNPNTS> {};
  // does the line intersect with the top of the screen?
  auto gapToEdge = offset - (slope * ZoomRect.top);
  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
	stitchLine[0] =
	    POINT {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
	           wrap::ceil<int32_t>(maxYcoord - ((prevStitch.y - ZoomRect.bottom) * ZoomRatio.x))};
	stitchLine[1] =
	    POINT {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
	           wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.x))};
	wrap::polyline(StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
	return;
  }
  // does the line intersect the bottom of the screen?
  gapToEdge = offset - slope * ZoomRect.bottom;
  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
	stitchLine[0] =
	    POINT {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
	           wrap::ceil<int32_t>(maxYcoord - ((prevStitch.y - ZoomRect.bottom) * ZoomRatio.y))};
	stitchLine[1] =
	    POINT {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
	           wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.y))};
	wrap::polyline(StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
	return;
  }
  // does the line intersect the left side of the screen?
  if (slope != 0.0F) {
	gapToEdge = (offset - ZoomRect.left) / slope;
	if (gapToEdge >= ZoomRect.bottom && gapToEdge <= ZoomRect.top) {
	  stitchLine[0] =
	      POINT {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
	             wrap::ceil<int32_t>(maxYcoord - ((prevStitch.y - ZoomRect.bottom) * ZoomRatio.y))};
	  stitchLine[1] =
	      POINT {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
	             wrap::ceil<int32_t>(maxYcoord - ((iStitch.y - ZoomRect.bottom) * ZoomRatio.y))};
	  wrap::polyline(StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
	}
  }
}

void drawStitchBoxes() {
  auto const cellSize =
      F_POINT {(ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right),
               (ZoomRect.top - ZoomRect.bottom) / wrap::toFloat(StitchWindowClientRect.bottom)};
  auto const maxMapSize = wrap::toSize(StitchWindowClientRect.right * StitchWindowClientRect.bottom);
  auto stitchMap = boost::dynamic_bitset(maxMapSize, 0U); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  SelectObject(StitchWindowMemDC, LinePen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  if (Instance->stateMap.test(StateFlag::HID)) {
	for (auto iColor = 0U; iColor < thred::maxColor(); ++iColor) {
	  auto& colorChangeTable = ThrSingle->ColorChangeTable;
	  if (colorChangeTable.operator[](iColor).colorIndex != ActiveColor) {
		continue;
	  }
	  for (auto iStitch = colorChangeTable.operator[](iColor).stitchIndex;
	       iStitch < colorChangeTable.operator[](gsl::narrow_cast<size_t>(iColor) + 1U).stitchIndex;
	       ++iStitch) {
		if (auto const& stitch = Instance->stitchBuffer.operator[](iStitch);
		    stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right && stitch.y >= ZoomRect.bottom &&
		    stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch, cellSize)) {
		  stchbox(iStitch, StitchWindowMemDC);
		}
	  }
	}
  }
  else {
	auto iStitch = 0U;
	for (auto& stitch : Instance->stitchBuffer) {
	  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right && stitch.y >= ZoomRect.bottom &&
	      stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch, cellSize)) {
		stchbox(iStitch, StitchWindowMemDC);
	  }
	  ++iStitch;
	}
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

// ReSharper disable CppParameterMayBeConst
void drwLin(std::vector<POINT>& linePoints, uint32_t const currentStitch, uint32_t const length, HPEN hPen) {
  if (Instance->stitchBuffer.empty() || length == 0) {
	return;
  }
  if (ActiveLayer != 0U) {
	linePoints.clear();
  }
  auto const firstStitch = wrap::next(Instance->stitchBuffer.cbegin(), currentStitch);
  auto const lastStitch  = wrap::next(firstStitch, length);
  for (auto const rStitches = std::ranges::subrange(firstStitch, lastStitch); auto const& stitch : rStitches) {
	if (auto const layer = (stitch.attribute & LAYMSK) >> LAYSHFT;
	    ActiveLayer != 0U && layer != 0U && layer != ActiveLayer) {
	  continue;
	}
	linePoints.push_back({.x = std::lround((stitch.x - ZoomRect.left) * ZoomRatio.x),
	                      .y = std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                       ((stitch.y - ZoomRect.bottom) * ZoomRatio.y))});
  }
  SelectObject(StitchWindowMemDC, hPen);
  wrap::polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
  linePoints.clear();
  if (auto const layer = (firstStitch->attribute & LAYMSK) >> LAYSHFT;
      ActiveLayer != 0U && layer != 0U && layer != ActiveLayer) {
	return;
  }
  linePoints.push_back({.x = std::lround((firstStitch->x - ZoomRect.left) * ZoomRatio.x),
                        .y = std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
                                         ((firstStitch->y - ZoomRect.bottom) * ZoomRatio.y))});
}
// ReSharper restore CppParameterMayBeConst

void drwStch() {
  Instance->stateMap.set(StateFlag::RELAYR);
  Instance->stateMap.reset(StateFlag::SELSHO);
  Instance->stateMap.reset(StateFlag::ILIN1);
  Instance->stateMap.reset(StateFlag::BZUM);
  unboxs();
  thred::unrotu();
  thred::unrot();
  Instance->stateMap.reset(StateFlag::ILIN);
  uncros();
  Instance->stateMap.reset(StateFlag::SHOFRM);
  drawBackground();
  if (Instance->stateMap.test(StateFlag::INIT)) {
	doDrwInit();
  }
  if (Instance->stateMap.test(StateFlag::ROTAT) || Instance->stateMap.test(StateFlag::ROTCAPT) ||
      Instance->stateMap.test(StateFlag::MOVCNTR)) {
	thred::ritrot(0, form::rotpar());
  }
  if (!Instance->formList.empty() && !Instance->stateMap.test(StateFlag::FRMOF)) {
	form::drwfrm();
  }
  if (Instance->stateMap.test(StateFlag::INSFRM)) {
	form::rinfrm();
  }
  if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	form::dubig();
  }
  if (Instance->stateMap.test(StateFlag::MOVFRM)) {
	Instance->stateMap.set(StateFlag::SHOFRM);
	form::dufrm();
  }
  if (Instance->stateMap.test(StateFlag::SATPNT)) {
	satin::satzum();
  }
  if (Instance->stateMap.test(StateFlag::GMRK)) {
	drwmrk(StitchWindowMemDC);
  }
  if (Instance->stateMap.test(StateFlag::PRFACT)) {
	thred::redraw(PreferencesWindow);
  }
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
  }
  thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
  if (Instance->stateMap.test(StateFlag::INIT)) {
	thred::lenCalc();
  }
  if (Instance->stateMap.test(StateFlag::MOVSET)) {
	dumov();
  }
  drwknot();
}

void drwknot() {
  if (Instance->userFlagMap.test(UserFlag::KNOTOF) || ThrSingle->Knots.empty() ||
      Instance->stitchBuffer.empty()) {
	return;
  }
  constexpr auto KBOFFSET = 5; // offset of the knot box sides
  auto const     kOffset  = MulDiv(KBOFFSET, Instance->DPI, STDDPI);
  constexpr auto KLINELEN = 10; // length of the knot line
  auto const     kLine    = MulDiv(KLINELEN, Instance->DPI, STDDPI);
  auto           point    = POINT {};
  auto           kOutline = std::array<POINT, SQPNTS> {};
  auto           tLine    = std::array<POINT, LNPNTS> {};
  for (auto const knot : ThrSingle->Knots) {
	stCor2px(Instance->stitchBuffer.operator[](knot), point);
	SelectObject(StitchWindowMemDC, KnotPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	kOutline[0].x = kOutline[3].x = kOutline[4].x = point.x - kOffset;
	kOutline[1].x = kOutline[2].x = point.x + kOffset;
	kOutline[0].y = kOutline[1].y = kOutline[4].y = point.y + kOffset;
	kOutline[2].y = kOutline[3].y = point.y - kOffset;
	wrap::polyline(StitchWindowMemDC, kOutline.data(), wrap::toUnsigned(kOutline.size()));
	tLine[0].x = point.x - kLine;
	tLine[1].x = point.x + kLine;
	tLine[0].y = tLine[1].y = point.y;
	wrap::polyline(StitchWindowMemDC, tLine.data(), wrap::toUnsigned(tLine.size()));
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
}

void drwlstch(uint32_t const finish) {
  auto           movieLine = std::vector<POINT> {}; // line for movie stitch draw
  constexpr auto LNMAXPTS  = 100U;                  // maximum number of points in a line
  movieLine.reserve(LNMAXPTS);
  if (finish == 0) {
	patdun();
	return;
  }
  if (Instance->stateMap.test(StateFlag::HID)) {
	while (RunPoint < finish - 1 && (Instance->stitchBuffer.operator[](RunPoint).attribute & COLMSK) != ActiveColor) {
	  ++RunPoint;
	}
  }
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	constexpr auto STITCH_COORDS_IN_PIXELS = POINT {};
	movieLine.push_back(STITCH_COORDS_IN_PIXELS);
	while (RunPoint < StitchesPerFrame + 1 && RunPoint < finish - 2 && !stch2px2(RunPoint)) {
	  ++RunPoint;
	}
  }
  auto const color = Instance->stitchBuffer.operator[](RunPoint).attribute & COLMSK;
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	auto       stitchCoordsInPixels = POINT {};
	auto const origin               = RunPoint - 1U;
	auto       flag                 = true;
	while (wrap::toUnsigned(movieLine.size()) < StitchesPerFrame + 1U && RunPoint < finish - 2U &&
	       (Instance->stitchBuffer.operator[](RunPoint).attribute & COLMSK) == color) {
	  if (thred::stch2px(RunPoint, stitchCoordsInPixels)) {
		movieLine.push_back(stitchCoordsInPixels);
		if (flag) {
		  flag = false;
		  if (RunPoint != 0U && thred::stch2px(RunPoint - 1, stitchCoordsInPixels)) {
			movieLine.front() = movieLine.back();
		  }
		  else {
			movieLine.front() = stitchCoordsInPixels;
		  }
		}
	  }
	  ++RunPoint;
	}
	if (RunPoint == origin) {
	  ++RunPoint;
	}
	if (!thred::stch2px(RunPoint, stitchCoordsInPixels)) {
	  if ((Instance->stitchBuffer.operator[](RunPoint).attribute & COLMSK) == color) {
		movieLine.push_back(stitchCoordsInPixels);
		++RunPoint;
	  }
	}
	SelectObject(StitchWindowDC, ThrSingle->UserPen.operator[](color));
	wrap::polyline(StitchWindowDC, movieLine.data(), wrap::toUnsigned(movieLine.size()));
	if (!flag) {
	  --RunPoint;
	}
  }
  else {
	auto iMovieFrame = 0U;

	SelectObject(StitchWindowDC, ThrSingle->UserPen.operator[](color));
	while (iMovieFrame < StitchesPerFrame && RunPoint + 1 < finish - 1 &&
	       (Instance->stitchBuffer.operator[](RunPoint).attribute & COLMSK) == color) {
	  movieLine.push_back(thred::stch2px1(RunPoint++));
	  ++iMovieFrame;
	}
	--RunPoint;
	wrap::polyline(StitchWindowDC, movieLine.data(), wrap::toUnsigned(movieLine.size()));
  }
  if ((Instance->stitchBuffer.operator[](wrap::toSize(RunPoint) + 1U).attribute & COLMSK) != color) {
	++RunPoint;
  }
  displayText::ritnum(IDS_NUMSEL, RunPoint);
  if (RunPoint + 3U > finish - 1U) {
	patdun();
  }
}

// ReSharper disable CppParameterMayBeConst
void drwmrk(HDC hDC) {
  auto       markCoordinates = POINT {};
  auto       markLine        = std::array<POINT, 2> {};
  auto const markOffset      = MulDiv(6, Instance->DPI, STDDPI);
  thred::sCor2px(F_POINT {ZoomMarkPoint}, markCoordinates);
  SelectObject(hDC, ZoomMarkPen);
  SetROP2(hDC, R2_XORPEN);
  markLine[0] = {.x = markCoordinates.x - markOffset, .y = markCoordinates.y - markOffset};
  markLine[1] = {.x = markCoordinates.x + markOffset, .y = markCoordinates.y + markOffset};
  wrap::polyline(hDC, markLine.data(), wrap::toUnsigned(markLine.size()));
  markLine[0] = {.x = markCoordinates.x - markOffset, .y = markCoordinates.y + markOffset};
  markLine[1] = {.x = markCoordinates.x + markOffset, .y = markCoordinates.y - markOffset};
  wrap::polyline(hDC, markLine.data(), wrap::toUnsigned(markLine.size()));
  SetROP2(hDC, R2_COPYPEN);
}
// ReSharper restore CppParameterMayBeConst

void duClos(uint32_t const      startStitch,
            uint32_t const      stitchCount,
            F_POINT const&      stitchPoint,
            std::vector<float>& gapToNearest) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto stitch = wrap::next(Instance->stitchBuffer.begin(), startStitch);
  for (auto iStitch = startStitch; iStitch < startStitch + stitchCount; ++iStitch) {
	auto const deltaX = stitch->x - stitchPoint.x;
	auto const deltaY = stitch->y - stitchPoint.y;

	auto sum       = (deltaX * deltaX) + (deltaY * deltaY);
	auto tempPoint = iStitch;
	auto gap       = gapToNearest.begin();
	for (auto& point : ThrSingle->NearestPoint) {
	  if (sum < *gap) {
		std::swap(*gap, sum);
		std::swap(point, tempPoint);
	  }
	  ++gap;
	}
	++stitch;
  }
}

void duar(POINT const& stitchCoordsInPixels) noexcept(std::is_same_v<size_t, uint32_t>) {
  auto const offset = MulDiv(10, Instance->DPI, STDDPI);
  auto arrowCenter  = POINT {(stitchCoordsInPixels.x - offset), (stitchCoordsInPixels.y + offset)};
  StitchArrow[1]    = stitchCoordsInPixels;
  rotpix(arrowCenter, StitchArrow[0], stitchCoordsInPixels);
  arrowCenter.y = stitchCoordsInPixels.y - offset;
  rotpix(arrowCenter, StitchArrow[2], stitchCoordsInPixels);
  SelectObject(StitchWindowMemDC, BoxPen[0]);
  SelectObject(StitchWindowDC, BoxPen[0]);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowMemDC, StitchArrow.data(), wrap::toUnsigned(StitchArrow.size()));
  wrap::polyline(StitchWindowDC, StitchArrow.data(), wrap::toUnsigned(StitchArrow.size()));
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void dubar() {
  auto colorBarRect = RECT {DrawItem->rcItem.left, 0, DrawItem->rcItem.right, DrawItem->rcItem.bottom};
  auto       indicatorLine = std::array<POINT, 2> {};
  auto const buffSize      = wrap::toFloat(Instance->stitchBuffer.size());
  for (auto iColorChange = size_t {}; iColorChange < thred::maxColor(); ++iColorChange) {
	auto&      colorChangeTable = ThrSingle->ColorChangeTable;
	auto const barSectionHeight =
	    wrap::toFloat(colorChangeTable.operator[](iColorChange + 1U).stitchIndex) / buffSize;
	colorBarRect.bottom = std::lround(barSectionHeight * wrap::toFloat(DrawItem->rcItem.bottom));
	auto ucb = wrap::next(UserColorBrush.begin(), colorChangeTable.operator[](iColorChange).colorIndex);
	FillRect(DrawItem->hDC, &colorBarRect, *ucb);
	colorBarRect.top = colorBarRect.bottom;
  }
  if (!Instance->stateMap.test(StateFlag::SELBOX) && !Instance->stateMap.test(StateFlag::GRPSEL)) {
	return;
  }
  auto selectedIndicator = wrap::toFloat(ClosestPointIndex) / buffSize;
  indicatorLine[0].y     = indicatorLine[1].y =
      wrap::ceil<decltype(indicatorLine[0].y)>(wrap::toFloat(colorBarRect.bottom) * selectedIndicator);
  indicatorLine[0].x = colorBarRect.left;
  indicatorLine[1].x = colorBarRect.right;
  SelectObject(DrawItem->hDC, CrossPen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  wrap::polyline(DrawItem->hDC, indicatorLine.data(), wrap::toUnsigned(indicatorLine.size()));
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	selectedIndicator  = wrap::toFloat(GroupStitchIndex) / buffSize;
	indicatorLine[0].y = indicatorLine[1].y =
	    wrap::ceil<decltype(indicatorLine[0].y)>(wrap::toFloat(colorBarRect.bottom) * selectedIndicator);
	indicatorLine[0].x = colorBarRect.left;
	indicatorLine[1].x = colorBarRect.right;
	wrap::polyline(DrawItem->hDC, indicatorLine.data(), wrap::toUnsigned(indicatorLine.size()));
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

// fill in the header and write the data to the buffer
void dubuf(std::vector<char>& buffer) {
  auto stitchHeader  = THR_HEAD {};
  auto vertexCount   = 0U;
  auto guideCount    = 0U;
  auto clipDataCount = 0U;

  constexpr auto HDRVERS = 0x02U; // ThrEd file version signature

  stitchHeader.headerType = HDRVERS << TBYTSHFT | THREDSIG;
  stitchHeader.fileLength =
      wrap::sizeofVector(Instance->stitchBuffer) + sizeof(stitchHeader) + bitmap::getBmpNameLength();
  wrap::narrow(stitchHeader.stitchCount, Instance->stitchBuffer.size());
  wrap::narrow_cast(stitchHeader.hoopType, IniFile.hoopType);
  auto       designer       = utf::utf16ToUtf8(ThrSingle->DesignerName);
  auto const spModifierName = gsl::span {ExtendedHeader.modifierName};
  std::ranges::copy(designer, spModifierName.begin());
  spModifierName[designer.length()] = 0;
  auto& formList                    = Instance->formList;

  if (!formList.empty()) {
	// count the number of vertices, satin guides, and clip data points in all forms
	for (auto& iForm : formList) {
	  vertexCount += iForm.vertexCount;
	  if (iForm.type == SAT && iForm.satinGuideCount != 0U) {
		guideCount += iForm.satinGuideCount;
	  }
	  if (iForm.isClip()) {
		clipDataCount += iForm.clipCount;
	  }
	  if (iForm.isEdgeClip()) {
		clipDataCount += iForm.clipEntries;
	  }
	}
  }
  // fill in the header with the values
  wrap::narrow(stitchHeader.formCount, formList.size());
  wrap::narrow(stitchHeader.vertexCount, vertexCount);
  wrap::narrow(stitchHeader.dlineCount, guideCount);
  wrap::narrow(stitchHeader.clipDataCount, clipDataCount);
  // calculate the offset to the form data entry point
  auto const formDataOffset = bitmap::getBmpNameLength() + sizeof(BackgroundColor) +
                              sizeof(UserColor) + sizeof(CustomColor) + TSSIZE;
  auto vtxLen = sizeof(stitchHeader) + wrap::sizeofVector(Instance->stitchBuffer) + formDataOffset;
  // calculate the design data size
  auto const thredDataSize = wrap::sizeofVector(formList) +
                             (vertexCount * wrap::sizeofType(Instance->formVertices)) +
                             (guideCount * wrap::sizeofType(Instance->satinGuides)) +
                             (clipDataCount * wrap::sizeofType(Instance->clipPoints)) +
                             wrap::sizeofVector(Instance->texturePointsBuffer);
  buffer.reserve(vtxLen + thredDataSize);
  // ToDo - vertexLength overflows a 16 bit integer if there are more than 5446 stitches, so clamp it until version 3
  constexpr auto VTXCLAMP =
      gsl::narrow_cast<size_t>(std::numeric_limits<decltype(stitchHeader.vertexLen)>::max());
  vtxLen = std::min(vtxLen, VTXCLAMP);
  wrap::narrow(stitchHeader.vertexLen, vtxLen);
  wrap::narrow(stitchHeader.dlineLen, wrap::sizeofType(Instance->formVertices) * vertexCount);
  wrap::narrow(stitchHeader.clipDataLen, wrap::sizeofType(Instance->clipPoints) * clipDataCount);
  // write the header to the buffer
  durit(buffer, &stitchHeader, sizeof(stitchHeader));
  ExtendedHeader.auxFormat         = IniFile.auxFileType;
  ExtendedHeader.hoopSizeX         = IniFile.hoopSizeX;
  ExtendedHeader.hoopSizeY         = IniFile.hoopSizeY;
  ExtendedHeader.texturePointCount = wrap::toUnsigned(Instance->texturePointsBuffer.size());
  // write the rest of the data to the buffer
  durit(buffer, &ExtendedHeader, sizeof(ExtendedHeader));
  durit(buffer, Instance->stitchBuffer.data(), wrap::sizeofVector(Instance->stitchBuffer));
  durit(buffer, bitmap::getBmpNameData(), bitmap::getBmpNameLength());
  durit(buffer, &BackgroundColor, sizeof(BackgroundColor));
  durit(buffer, UserColor.data(), sizeof(UserColor));
  durit(buffer, CustomColor.data(), sizeof(CustomColor));
  // write the thread size data to the buffer
  auto threadSizeBuffer = std::string {};
  threadSizeBuffer.resize(TSSIZE);
  auto iBuffer = threadSizeBuffer.begin();
  for (auto const& iThread : ThreadSize) {
	*iBuffer++ = gsl::narrow<char>(iThread);
  }

  durit(buffer, threadSizeBuffer.c_str(), wrap::toUnsigned(threadSizeBuffer.size() * sizeof(threadSizeBuffer[0])));
  // if we have no forms then we are done
  if (formList.empty()) {
	return;
  }
  // reserve space for the form data
  auto outForms = std::vector<FRM_HEAD_OUT> {};
  outForms.reserve(formList.size());
  auto vertices = std::vector<F_POINT> {};
  vertices.reserve(vertexCount);
  auto guides = std::vector<SAT_CON_OUT> {};
  guides.reserve(guideCount);
  auto points = std::vector<F_POINT> {};
  points.reserve(clipDataCount);
  // write the form vertex data to the buffer
  for (auto& srcForm : formList) {
	outForms.emplace_back(srcForm);
	auto itVertex = wrap::next(Instance->formVertices.cbegin(), srcForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < srcForm.vertexCount; ++iVertex) {
	  vertices.push_back(*itVertex);
	  ++itVertex;
	}
	if (srcForm.type == SAT) { // write the satin guide data to the guide buffer
	  wrap::narrow(outForms.back().satinGuideCount, srcForm.satinGuideCount);
	  if (srcForm.satinGuideCount != 0U) {
		auto itGuide = wrap::next(Instance->satinGuides.cbegin(), srcForm.satinGuideIndex);
		for (auto iGuide = 0U; iGuide < srcForm.satinGuideCount; ++iGuide) {
		  guides.emplace_back(*itGuide);
		  ++itGuide;
		}
	  }
	}
	if (srcForm.isClip()) { // write the clip data to the points buffer
	  auto offsetStart = wrap::next(Instance->clipPoints.cbegin(), srcForm.clipIndex);
	  for (auto iClip = 0U; iClip < srcForm.clipCount; ++iClip) {
		points.push_back(*offsetStart);
		++offsetStart;
	  }
	}
	if (srcForm.isEdgeClipX()) { // write the edge clip data to the points buffer
	  auto       offsetStart = wrap::next(Instance->clipPoints.cbegin(), srcForm.borderClipData);
	  auto const clipCount   = srcForm.clipEntries;
	  for (auto iClip = 0U; iClip < clipCount; ++iClip) {
		points.push_back(*offsetStart);
		++offsetStart;
	  }
	}
  }
  // write the form data to the buffer
  if (!outForms.empty()) {
	durit(buffer, outForms.data(), wrap::sizeofVector(outForms));
  }
  if (!vertices.empty()) {
	durit(buffer, vertices.data(), wrap::sizeofVector(vertices));
  }
  if (!guides.empty()) {
	durit(buffer, guides.data(), wrap::sizeofVector(guides));
  }
  if (!points.empty()) {
	durit(buffer, points.data(), wrap::sizeofVector(points));
  }
  if (!Instance->texturePointsBuffer.empty()) {
	durit(buffer, Instance->texturePointsBuffer.data(), wrap::sizeofVector(Instance->texturePointsBuffer));
  }
}

void duclp() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::polyline(StitchWindowDC, ClipInsertBoxLine.data(), wrap::toUnsigned(ClipInsertBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void ducmd() {
  if (ArgCount <= 1) {
	return;
  }
  auto const spArgList = gsl::span {ArgList, wrap::toSize(ArgCount)};
  auto const arg1      = std::wstring {spArgList[1]};
  if (!arg1.starts_with(L"/F1:")) {
	Instance->workingFileName.assign(arg1);
	Instance->stateMap.set(StateFlag::REDOLD);
	nuFil(FileIndices::THR);
	return;
  }
  auto balaradFileName = ThrSingle->HomeDirectory / arg1.substr(4);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto balaradFile =
      CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (balaradFile == INVALID_HANDLE_VALUE) {
	return;
  }
  CloseHandle(balaradFile);
  auto& balaradName0 = bal::getBN0();

  balaradName0 = balaradFileName;
  if (ArgCount <= 2) {
	return;
  }
  auto const arg2 = std::wstring {spArgList[2]};
  if (!arg2.starts_with(L"/F2:")) {
	return;
  }
  balaradFileName = ThrSingle->HomeDirectory / arg2.substr(4);
  balaradFile =
      CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (balaradFile == INVALID_HANDLE_VALUE) {
	return;
  }
  auto readBuffer = std::vector<char> {};
  readBuffer.resize(MAX_PATH + 1);
  auto& balaradName1 = bal::getBN1();

  balaradName1   = balaradFileName;
  auto bytesRead = DWORD {};
  if (!wrap::readFile(balaradFile, readBuffer.data(), readBuffer.size(), &bytesRead, L"ReadFile for readBuffer in ducmd")) {
	return;
  }
  if (bytesRead != 0U) {
	readBuffer.resize(bytesRead);
	auto& balaradName2 = bal::getBN2();
	balaradName2.assign(readBuffer.data());
	bal::redbal();
  }
  CloseHandle(balaradFile);
  fs::remove(balaradName1);
}

// ReSharper disable CppParameterMayBeConst
void ducros(HDC hDC) {
  uncros();
  Instance->stateMap.set(StateFlag::SCROS);
  cros(GroupStartStitch);
  if (GroupStartStitch != GroupEndStitch) {
	cros(GroupEndStitch);
	Instance->stateMap.set(StateFlag::ECROS);
  }
  selin(GroupStartStitch, GroupEndStitch, hDC);
}
// ReSharper restore CppParameterMayBeConst

void dugrid() {
  if (ZoomFactor >= ShowStitchThreshold && ShowStitchThreshold != 0.0F) {
	return;
  }
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  thred::selectGridPen();
  auto const gridRect = RECT {wrap::ceil<int32_t>(ZoomRect.left / IniFile.gridSize),
                              wrap::floor<int32_t>(ZoomRect.top / IniFile.gridSize),
                              wrap::floor<int32_t>(ZoomRect.right / IniFile.gridSize),
                              wrap::ceil<int32_t>(ZoomRect.bottom / IniFile.gridSize)};

  auto gridLine = std::array<POINT, 2> {};
  gridLine[0].x = 0;
  gridLine[1].x = StitchWindowClientRect.right;
  for (auto iGrid = gridRect.bottom; iGrid <= gridRect.top; ++iGrid) {
	gridLine[0].y = gridLine[1].y =
	    wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
	                        ((wrap::toFloat(iGrid) * IniFile.gridSize - ZoomRect.bottom) * ZoomRatio.y));
	wrap::polyline(StitchWindowMemDC, gridLine.data(), wrap::toUnsigned(gridLine.size()));
  }
  gridLine[0].y = 0;
  gridLine[1].y = StitchWindowClientRect.bottom;
  for (auto iGrid = gridRect.left; iGrid <= gridRect.right; ++iGrid) {
	gridLine[0].x = gridLine[1].x =
	    wrap::ceil<int32_t>((wrap::toFloat(iGrid) * IniFile.gridSize - ZoomRect.left) * ZoomRatio.x);
	wrap::polyline(StitchWindowMemDC, gridLine.data(), wrap::toUnsigned(gridLine.size()));
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void duhom() {
  auto const arg0          = fs::path {*ArgList};
  ThrSingle->HomeDirectory = arg0.parent_path();
}

void dulin(std::array<POINT, 2> const& moveLine0, std::array<POINT, 2> const& moveLine1) {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (moveLine0[0].x == moveLine1[1].x && moveLine0[0].y == moveLine1[1].y) {
	if (Instance->stateMap.test(StateFlag::ISDWN)) {
	  wrap::polyline(StitchWindowDC, moveLine0.data(), wrap::toUnsigned(moveLine0.size()));
	}
	else {
	  wrap::polyline(StitchWindowDC, moveLine1.data(), wrap::toUnsigned(moveLine1.size()));
	}
  }
  else {
	if (Instance->stateMap.test(StateFlag::ISDWN)) {
	  wrap::polyline(StitchWindowDC, moveLine0.data(), wrap::toUnsigned(moveLine0.size()));
	}
	if (Instance->stateMap.test(StateFlag::ISUP)) {
	  wrap::polyline(StitchWindowDC, moveLine1.data(), wrap::toUnsigned(moveLine1.size()));
	}
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
  Instance->stateMap.flip(StateFlag::WASLIN);
}

/* Arrow box points
 *                      06
 *                     / \
 *				   2--1  5--4
 *                  \       /
 *                   \     /
 *                    \   /
 *                      3
 */
void dumov() {
  // Arrow Box Indices
  constexpr auto ABP0 = 0;
  constexpr auto ABP1 = 1;
  constexpr auto ABP2 = 2;
  constexpr auto ABP3 = 3;
  constexpr auto ABP4 = 4;
  constexpr auto ABP5 = 5;
  constexpr auto ABP6 = 6;

  constexpr auto ABPOINTS = 7U; // Number of arrow box points

  constexpr auto ABLX = 12; // Arrow Length
  constexpr auto ABSX = 2;  // Arrow shaft width
  constexpr auto ABWX = 6;  // Arrow Width
  constexpr auto ABHY = 20; // Arrow length

  auto const sCurr = wrap::next(Instance->stitchBuffer.begin(), keys::getMoveAnchor());
  if (sCurr->x < ZoomRect.left || sCurr->x > ZoomRect.right || sCurr->y < ZoomRect.bottom ||
      sCurr->y > ZoomRect.top) {
	return;
  }
  auto const sNext          = std::next(sCurr);
  RotateAngle               = std::atan2f(sNext->y - sCurr->y, sNext->x - sCurr->x);
  auto       arrowBox       = std::array<POINT, ABPOINTS> {};
  auto const abCenterPixels = sdCor2px(Instance->stitchBuffer.operator[](keys::getMoveAnchor()));
  arrowBox[ABP0]            = abCenterPixels;
  arrowBox[ABP6]            = abCenterPixels;
  auto offsetFromCenter     = POINT {abCenterPixels.x + ABLX, abCenterPixels.y + ABSX};
  rotpix(offsetFromCenter, arrowBox[ABP1], abCenterPixels);
  offsetFromCenter.y = abCenterPixels.y - ABSX;
  rotpix(offsetFromCenter, arrowBox[ABP5], abCenterPixels);
  offsetFromCenter.y = abCenterPixels.y + ABWX;
  rotpix(offsetFromCenter, arrowBox[ABP2], abCenterPixels);
  offsetFromCenter.y = abCenterPixels.y - ABWX;
  rotpix(offsetFromCenter, arrowBox[ABP4], abCenterPixels);
  offsetFromCenter = POINT {abCenterPixels.x + ABHY, abCenterPixels.y};
  rotpix(offsetFromCenter, arrowBox[ABP3], abCenterPixels);
  SelectObject(StitchWindowMemDC, FormPen);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  wrap::polyline(StitchWindowMemDC, arrowBox.data(), wrap::toUnsigned(arrowBox.size()));
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

auto dunum(wchar_t const code) noexcept -> bool {
  if (code >= L'0' && code <= L'9') {
	NumericCode = code;
	return true;
  }
  if (code >= VK_NUMPAD0 && code <= VK_NUMPAD9) {
	NumericCode = code - L'0';
	return true;
  }
  return false;
}

void duprct(FRM_HEAD const& form) {
  auto currentVertex = SelectedFormVertices.start;
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + currentVertex);
  SelectedVerticesRect.left = SelectedVerticesRect.right = itVertex->x;
  SelectedVerticesRect.top = SelectedVerticesRect.bottom = itVertex->y;
  currentVertex                                          = form::pdir(form, currentVertex);
  for (auto iVertex = 0U; iVertex < SelectedFormVertices.vertexCount; ++iVertex) {
	itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + currentVertex);
	SelectedVerticesRect.left   = std::min(itVertex->x, SelectedVerticesRect.left);
	SelectedVerticesRect.right  = std::max(itVertex->x, SelectedVerticesRect.right);
	SelectedVerticesRect.bottom = std::min(itVertex->y, SelectedVerticesRect.bottom);
	SelectedVerticesRect.top    = std::max(itVertex->y, SelectedVerticesRect.top);
	currentVertex               = form::pdir(form, currentVertex);
  }
}

void durit(std::vector<char>& destination, const void* source, uint32_t count) {
  if (source != nullptr) {
	auto const spSrc = gsl::span {gsl::narrow_cast<char const*>(source), gsl::narrow_cast<size_t>(count)};
	destination.insert(destination.end(), spSrc.begin(), spSrc.end());
  }
}

void durot() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::polyline(StitchWindowDC, RotateBoxOutline.data(), wrap::toUnsigned(RotateBoxOutline.size()));
  wrap::polyline(
      StitchWindowDC, RotateBoxCrossVertLine.data(), wrap::toUnsigned(RotateBoxCrossVertLine.size()));
  wrap::polyline(
      StitchWindowDC, RotateBoxCrossHorzLine.data(), wrap::toUnsigned(RotateBoxCrossHorzLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

// ReSharper disable CppParameterMayBeConst
void dusel(HDC hDC) {
  SetROP2(hDC, R2_NOTXORPEN);
  SelectObject(hDC, LinePen);
  auto const& formControlPoints = ThrSingle->FormControlPoints;
  wrap::polyline(hDC, formControlPoints.data(), wrap::toUnsigned(formControlPoints.size() - 1U));
  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(formControlPoints.size() - 1U); ++iPoint) {
	form::selsqr(formControlPoints.operator[](iPoint), hDC);
  }
  SetROP2(hDC, R2_COPYPEN);
}
// ReSharper restore CppParameterMayBeConst

void duselrng(RANGE& selectedRange) {
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	selectedRange.start  = GroupStartStitch;
	selectedRange.finish = GroupEndStitch;
	return;
  }
  selectedRange.start  = 0U;
  selectedRange.finish = wrap::toUnsigned(Instance->stitchBuffer.size());
}

void dusid(LIST_TYPE const entry, int32_t& windowLocation, SIZE const& windowSize) {
  ThrSingle->SideWindow.operator[](entry.value) =
      CreateWindow(L"STATIC",
                   displayText::loadStr(entry.stringID).c_str(),
                   SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
                   3,
                   (windowLocation * windowSize.cy) + 3,
                   windowSize.cx + 3,
                   windowSize.cy,
                   SideMessageWindow,
                   nullptr,
                   ThrEdInstance,
                   nullptr);
  ++windowLocation;
}

void duver(fs::path const& name) {
  if (auto const version = wrap::toSize(tolower(name.extension().wstring().back()) - 'r');
      version < ThrSingle->VersionNames.size()) {
	ThrSingle->VersionNames.operator[](version) = name;
  }
}

void endknt(std::vector<F_POINT_ATTR>& buffer, uint32_t const finish) {
  auto const finishIt      = wrap::next(Instance->stitchBuffer.begin(), finish);
  auto       startIt       = finish != 0 ? std::next(finishIt, -1) : finishIt;
  auto const knotAttribute = startIt->attribute | KNOTMSK;

  auto deltaX = finishIt->x - startIt->x;
  auto deltaY = finishIt->y - startIt->y;
  auto length = (deltaX * deltaX) + (deltaY * deltaY);
  while (length == 0.0F && startIt != Instance->stitchBuffer.begin()) {
	deltaX = finishIt->x - startIt->x;
	deltaY = finishIt->y - startIt->y;
	length = deltaX * deltaX + deltaY * deltaY;
	--startIt;
  }
  length = std::sqrt(length);
  deltaX = finishIt->x - startIt->x;
  deltaY = finishIt->y - startIt->y;

  auto const knotStep = F_POINT {2.0F / length * deltaX, 2.0F / length * deltaY};
  constexpr auto KNOT_AT_END_ORDER = std::array<char, KNOTSCNT> {-2, -3, -1, -4, 0}; // reverse knot spacings
  constexpr auto KNOT_AT_LAST_ORDER = std::array<char, KNOTSCNT> {0, -4, -1, -3, -2}; // reverse knot spacings
  for (auto const& knots = Instance->stateMap.test(StateFlag::FILDIR) ? KNOT_AT_LAST_ORDER : KNOT_AT_END_ORDER;
       auto const knot : knots) {
	ofstch(buffer, finish, knot, knotStep, knotAttribute);
  }
  if (Instance->stateMap.test(StateFlag::FILDIR)) {
	ofstch(buffer, finish, 0, knotStep, knotAttribute);
  }
}

// ReSharper disable CppParameterMayBeConst
auto CALLBACK enumChildProc(HWND hwnd, LPARAM lParam) noexcept -> BOOL {
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
// ReSharper restore CppParameterMayBeConst

void f1del(uint32_t const formIndex) {
  if (Instance->stateMap.test(StateFlag::DELTO)) {
	auto const codedForm = formIndex << FRMSHFT;
	std::erase_if(Instance->stitchBuffer, [codedForm](F_POINT_ATTR const& stitch) -> bool {
	  return (stitch.attribute & NOTFRM) == 0U && (stitch.attribute & FRMSK) == codedForm;
	});
  }
  clip::deleclp(formIndex);
  clip::delmclp(formIndex);
  satin::delsac(formIndex);
  form::delflt(formIndex);
  texture::deltx(formIndex);
}

void fillSortBuffer() {
  auto& sortBuffer = ThrSingle->SortBuffer;
  sortBuffer.clear();
  sortBuffer.reserve(Instance->stitchBuffer.size());
  auto stitch     = wrap::next(Instance->stitchBuffer.begin(), SelectedRange.start);
  auto nextStitch = wrap::next(stitch, 1);
  auto index      = SelectedRange.start;

  auto const endStitch = wrap::next(Instance->stitchBuffer.begin(), SelectedRange.finish);
  while (nextStitch != endStitch) {
	auto const deltaX = nextStitch->x - stitch->x;
	auto const deltaY = nextStitch->y - stitch->y;
	auto const length = (deltaX * deltaX) + (deltaY * deltaY);
	sortBuffer.push_back(SEARCH_REC {.index = index, .length = length});
	++index;
	++stitch;
	++nextStitch;
  }
  std::ranges::stable_sort(sortBuffer, {}, &SEARCH_REC::length);
}

auto find1st() -> uint32_t {
  if (chk2of()) {
	return ClosestPointIndex;
  }
  return thred::findFirstStitch(ClosestFormToCursor);
}

void fnamtabs() {
  constexpr auto NORDSED = uint32_t {0x5a5a5a5aU}; // name order seed
  constexpr auto NCODSED = uint32_t {0x73ef5a7eU}; // name encoding seed
  constexpr auto NCODOF  = uint8_t {80U};          // name encoding offset
  constexpr auto MAX8B   = uint8_t {127};          // max value of a signed byte
  constexpr auto NDXOFF  = uint8_t {32U};          // index offset
  constexpr auto NAMELEN = NameOrder.size();
  boost::range::iota(NameOrder, 0);
  PseudoRandomValue = NORDSED;
  for (auto iName = 0U; iName < 2 * NAMELEN; ++iName) {
	auto const source      = wrap::next(NameOrder.begin(), form::psg() % NAMELEN);
	auto const destination = wrap::next(NameOrder.begin(), form::psg() % NAMELEN);
	std::swap(*destination, *source);
  }
  boost::range::iota(NameEncoder, NCODOF);
  PseudoRandomValue = NCODSED;
  for (auto iName = 0U; iName < 2 * NameEncoder.size(); ++iName) {
	auto const source      = wrap::next(NameEncoder.begin(), form::psg() & MSK7BITS);
	auto const destination = wrap::next(NameEncoder.begin(), form::psg() & MSK7BITS);
	std::swap(*destination, *source);
  }
  NameDecoder.fill(0);
  auto index = wrap::next(NameEncoder.begin(), NDXOFF);
  for (auto iName = NDXOFF; iName < uint8_t {MAX8B}; ++iName) {
	auto iND = wrap::next(NameDecoder.begin(), *index++);
	*iND     = iName;
  }
}

void fndknt() {
  auto& knots     = ThrSingle->Knots;
  auto  endStitch = wrap::toUnsigned(Instance->stitchBuffer.size());
  if (endStitch <= 4U) {
	knots.clear();
	return;
  }
  endStitch -= 4U;
  knots.clear();
  auto const spStitch = std::ranges::subrange(Instance->stitchBuffer.begin(),
                                              wrap::next(Instance->stitchBuffer.begin(), endStitch));
  for (auto iStitch = 0; const auto& stitch : spStitch) {
	if ((stitch.attribute & KNOTMSK) != 0U) {
	  knots.emplace_back(iStitch);
	}
	iStitch++;
  }
}

void formStretch(uint32_t const form) {
  auto stitch  = Instance->stitchBuffer.begin();
  auto iStitch = 0U;
  for (; iStitch < wrap::toUnsigned(Instance->stitchBuffer.size()); ++iStitch) {
	if ((stitch->attribute & FRMSK) >> FRMSHFT == form && (stitch->attribute & TYPFRM) != 0U) {
	  break;
	}
	++stitch;
  }
  auto const firstStitch = iStitch;
  while (stitch != Instance->stitchBuffer.end() && (stitch->attribute & FRMSK) >> FRMSHFT == form) {
	++iStitch;
	++stitch;
  }
  doStretch(firstStitch, iStitch);
}

void frmcalc(uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex) {
  if (auto const& form = Instance->formList.operator[](ClosestFormToCursor);
      Instance->stitchBuffer.empty() || (form.fillType == 0U && form.edgeType == 0U)) {
	auto const blank = std::wstring {};
	displayText::butxt(HMAXLEN, blank);
	displayText::butxt(HMINLEN, blank);
	return;
  }
  auto const code = ClosestFormToCursor << FRMSHFT;

  auto maxLength      = LOWFLOAT;
  auto minLength      = BIGFLOAT;
  smallestStitchIndex = 0U;
  largestStitchIndex  = 0U;
  auto stitchFwd1     = std::next(Instance->stitchBuffer.begin());

  auto const spStitches =
      std::ranges::subrange(Instance->stitchBuffer.begin(), Instance->stitchBuffer.end() - 2U);
  for (auto index = 0U; auto const& stitch : spStitches) {
	if ((stitch.attribute & FRMSK) != code || (stitch.attribute & NOTFRM) != 0U ||
	    (stitchFwd1->attribute & FRMSK) != code || (stitchFwd1->attribute & TYPMSK) == 0U) {
	  ++stitchFwd1;
	  ++index;
	  continue;
	}
	auto const deltaX = stitchFwd1->x - stitch.x;
	auto const deltaY = stitchFwd1->y - stitch.y;
	auto const length = (deltaX * deltaX) + (deltaY * deltaY);
	if (length > maxLength) {
	  maxLength          = length;
	  largestStitchIndex = index;
	}
	if (length < minLength) {
	  minLength           = length;
	  smallestStitchIndex = index;
	}
	++stitchFwd1;
	++index;
  }
  maxLength = std::sqrt(maxLength) * IPFGRAN;
  minLength = std::sqrt(minLength) * IPFGRAN;

  constexpr auto DIGITLIM = 10000.0F; // value that represents the max width that can be displayed
  if (fabs(maxLength) < DIGITLIM) {
	auto const strMax = displayText::format(IDS_LENMAX, maxLength);
	displayText::butxt(HMAXLEN, strMax);
  }
  if (fabs(minLength) < DIGITLIM) {
	auto const strMin = displayText::format(IDS_LENMIN, minLength);
	displayText::butxt(HMINLEN, strMin);
  }
}

void frmpos(FRM_HEAD& form, float const deltaX, float const deltaY) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*itVertex += F_POINT {deltaX, deltaY};
	++itVertex;
  }
  form.rectangle.left += deltaX;
  form.rectangle.top += deltaY;
  form.rectangle.right += deltaX;
  form.rectangle.bottom += deltaY;
}

void frmsnap(uint32_t const start, uint32_t const count) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto itVertex = wrap::next(Instance->formVertices.begin(), start);
  for (auto i = 0U; i < count; ++i) {
	itVertex->x = rintf(itVertex->x / IniFile.gridSize) * IniFile.gridSize;
	itVertex->y = rintf(itVertex->y / IniFile.gridSize) * IniFile.gridSize;
	++itVertex;
  }
}

auto frmstch() -> bool {
  auto formMap = boost::dynamic_bitset(Instance->formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  for (auto const selectedForm : Instance->selectedFormList) {
	formMap.set(selectedForm);
  }
  return std::ranges::any_of(Instance->stitchBuffer, [&formMap](F_POINT_ATTR const& stitch) -> bool {
	return formMap.test((stitch.attribute & FRMSK) >> FRMSHFT);
  });
}

// ReSharper disable CppParameterMayBeConst
auto CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  handleFeatherWMINITDIALOG(hwndlg);
	  break;
	}
	case WM_COMMAND: {
	  if (handleFeatherWMCOMMAND(wparam, hwndlg)) {
		return TRUE;
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in fthdefprc 2: umsg [{}]\n", umsg);
	  break;
	}
  }
  return FALSE;
}
// ReSharper restore CppParameterMayBeConst

void getbak() {
  if (!Instance->stateMap.test(StateFlag::THUMSHO)) {
	Instance->stateMap.set(StateFlag::REDOLD);
	nuFil(FileIndices::THR);
	return;
  }
  if (!Instance->stateMap.test(StateFlag::RBUT)) {
	return;
  }
  unthum();
  Instance->stateMap.set(StateFlag::FRMOF);
  auto& workingFileName = Instance->workingFileName;
  workingFileName       = ThrSingle->DefaultDirectory /
                    ThrSingle->Thumbnails.operator[](ThumbnailsSelected.at(FileVersionIndex));
  thred::insfil(workingFileName);
  if (!wrap::pressed(VK_SHIFT)) {
	return;
  }
  Instance->stateMap.reset(StateFlag::INSFIL);
  Instance->stateMap.reset(StateFlag::FRMOF);
  Instance->stateMap.set(StateFlag::INIT);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void getDocsFolder(fs::path& directory) {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto ppszPath = PWSTR {nullptr}; // variable to receive the path memory block pointer.
  if (auto const hResult = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &ppszPath); SUCCEEDED(hResult)) {
	directory.assign(ppszPath); // make a local copy of the path
  }
  CoTaskMemFree(ppszPath); // free up the path memory block
}

auto gethand(std::vector<F_POINT_ATTR> const& stitch, uint32_t const stitchCount) noexcept -> uint32_t {
  auto userStitchCount = 0U;
  for (auto iStitch = 0U; iStitch < stitchCount; ++iStitch) {
	if ((stitch[iStitch].attribute & USMSK) != 0U) {
	  ++userStitchCount;
	}
  }
  return userStitchCount;
}

auto getMaxCount() -> uint32_t {
  auto maxCount = uint32_t {0U};
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	auto const deltaCount =
	    gsl::narrow<uint32_t>(ThrSingle->ColorChangeTable.operator[](iColor + 1U).stitchIndex -
	                          ThrSingle->ColorChangeTable.operator[](iColor).stitchIndex);
	maxCount = std::max(deltaCount, maxCount);
  }
  return maxCount;
}

auto getNewFileName(fs::path& newFileName, FileStyles const fileTypes, FileIndices const fileIndex) -> bool {
  auto* pFileOpen = gsl::narrow_cast<IFileOpenDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hResult = CoCreateInstance(
      CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  if (FAILED(hResult) || nullptr == pFileOpen) {
	return false;
  }
  auto dwOptions = DWORD {};
  hResult        = pFileOpen->GetOptions(&dwOptions);
  if (FAILED(hResult)) {
	return false;
  }
  hResult = pFileOpen->SetOptions(dwOptions | FOS_DONTADDTORECENT); // NOLINT(hicpp-signed-bitwise)
  switch (fileTypes) {
	case FileStyles::ALL_FILES: {
#if PESACT
	  static constexpr auto ALL_FILE_TYPES =
	      std::array {FLTTHR, FLTPCS, FLTDST, FLTPES}; // All possible file types for save
#else
	  static constexpr auto ALL_FILE_TYPES = std::array<COMDLG_FILTERSPEC, 3> {FLTTHR, FLTPCS, FLTDST}; // All possible file types for save
#endif
	  hResult += pFileOpen->SetFileTypes(wrap::toUnsigned(ALL_FILE_TYPES.size()), ALL_FILE_TYPES.data());
	  break;
	}
	case FileStyles::INS_FILES: {
	  static constexpr auto INSERT_FILE_TYPES =
	      std::array {FLTTHR, FLTPCS}; // All possible file types that can be inserted into current design
	  hResult +=
	      pFileOpen->SetFileTypes(wrap::toUnsigned(INSERT_FILE_TYPES.size()), INSERT_FILE_TYPES.data());
	  break;
	}
	// ReSharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  throw std::runtime_error("Unknown file style");
	}
  }
  switch (fileIndex) {
	case FileIndices::THR: {
	  hResult += pFileOpen->SetFileTypeIndex(1);
	  break;
	}
	case FileIndices::PCS: {
	  hResult += pFileOpen->SetFileTypeIndex(2);
	  break;
	}
	case FileIndices::DST: {
	  hResult += pFileOpen->SetFileTypeIndex(3);
	  break;
	}
#if PESACT
	case FileIndices::PES: {
	  hResult += pFileOpen->SetFileTypeIndex(4);
	  break;
	}
#endif
	// ReSharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  throw std::runtime_error("Unknown file index");
	}
  }
  hResult += pFileOpen->SetTitle(L"Open ThrEd File");
#if USE_DEFAULTDIR
  // If we want to, we can set the default directory rather than using the OS mechanism for last used
  auto* psiFrom = gsl::narrow_cast<IShellItem*>(nullptr);
  hResult += SHCreateItemFromParsingName(
      ThrSingle->DefaultDirectory.wstring().data(), nullptr, IID_PPV_ARGS(&psiFrom));
  hResult += pFileOpen->SetFolder(psiFrom);
  if (nullptr != psiFrom) {
	psiFrom->Release();
  }
#endif
  if (FAILED(hResult)) {
	return false;
  }
  hResult = pFileOpen->Show(nullptr);
  if (FAILED(hResult)) {
	return false;
  }
  auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
  hResult     = pFileOpen->GetResult(&pItem);
  if (FAILED(hResult) || nullptr == pItem) {
	return false;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto pszFilePath = PWSTR {nullptr};
  hResult          = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
  if (FAILED(hResult)) {
	return false;
  }
  newFileName.assign(pszFilePath);
  CoTaskMemFree(pszFilePath);
  return true;
}

void gselrng() noexcept {
  auto const& selectedFormList = Instance->selectedFormList;
  SelectedFormsRange.start = SelectedFormsRange.finish = selectedFormList[0];
  for (auto const selectedForm : selectedFormList) {
	SelectedFormsRange.start  = std::min(selectedForm, SelectedFormsRange.start);
	SelectedFormsRange.finish = std::max(selectedForm, SelectedFormsRange.finish);
  }
}

void handleChkMsgWMCOMMAND(F_POINT& rotationCenter) {
  {
	auto previousName = ThrSingle->PreviousNames.begin();
	for (auto const& iLRU : LRU_MENU_ID) {
	  if (WinMsg.wParam == iLRU) {
		Instance->workingFileName = *previousName;
		Instance->stateMap.set(StateFlag::REDOLD);
		nuFil(FileIndices::THR);
	  }
	  ++previousName;
	}
  }
  auto const wParameter = LOWORD(WinMsg.wParam);
  if (wParameter >= ID_FILE_OPEN1 && wParameter <= ID_AUXPES) {
	thred::undat();
  }
  if (menu::handleMainMenu(wParameter, rotationCenter)) {
	return;
  }
  if (menu::handleEditMenu(wParameter)) {
	return;
  }
  if (menu::handleFileMenu(wParameter)) {
	return;
  }
  if (menu::handleFillMenu(wParameter)) {
	return;
  }
  menu::handleViewMenu(wParameter);
}

auto handleDeleteLineForm(FRM_HEAD& form) -> bool {
  if (form.fillType == CONTF) {
	if (ClosestVertexToCursor == form.fillGuide.start || ClosestVertexToCursor == form.fillGuide.finish) {
	  form::delmfil(ClosestFormToCursor);
	  form.fillType = 0;
	  thred::coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return true;
	}
	if (form.fillGuide.start > ClosestVertexToCursor) {
	  --form.fillGuide.start;
	}
	if (form.fillGuide.finish > ClosestVertexToCursor) {
	  --form.fillGuide.finish;
	}
  }
  return false;
}

void handleDeleteSatinForm(FRM_HEAD& form, bool& satinFlag) {
  if (ClosestVertexToCursor <= 1) {
	if ((form.attribute & FRMEND) != 0U) {
	  if (form.wordParam != 0U) {
		form.wordParam = 0;
	  }
	  else {
		form.attribute &= NFRMEND;
	  }
	  satinFlag = true;
	  return;
	}
  }
  if (auto const& endGuide = form.wordParam; endGuide != 0U) {
	if (ClosestVertexToCursor == endGuide || ClosestVertexToCursor == endGuide + 1U) {
	  form.wordParam = 0;
	  satinFlag      = true;
	  return;
	}
  }
  if (form.satinGuideCount != 0U) {
	auto itGuide = wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex);
	for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
	  if (itGuide->start == ClosestVertexToCursor || itGuide->finish == ClosestVertexToCursor) {
		satin::delcon(form, iGuide);
		satinFlag = true;
		break;
	  }
	  ++itGuide;
	}
  }
}

// ReSharper disable CppParameterMayBeConst
void handleFeatherIDOK(HWND hwndlg) {
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
  auto buf = std::array<wchar_t, HBUFSIZ> {};
  GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), buf.data(), HBUFSIZ);
  IniFile.initFeatherType();
  auto buffer = std::wstring {};
  for (auto const& iFeatherStyle : FTHRLIST) {
	buffer.assign(displayText::loadStr(iFeatherStyle.stringID));
	if (wcscmp(buf.data(), buffer.c_str()) == 0) {
	  IniFile.featherFillType = iFeatherStyle.value;
	  break;
	}
  }
  GetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf.data(), HBUFSIZ);
  IniFile.featherRatio = wrap::wcsToFloat(buf.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.featherUpCount, buf.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.featherDownCount, buf.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf.data(), HBUFSIZ);
  IniFile.featherMinStitchSize = wrap::wcsToFloat(buf.data()) * PFGRAN;
  GetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.featherCount, buf.data());
  IniFile.featherCount = std::max(IniFile.featherCount, uint16_t {1});
  EndDialog(hwndlg, TRUE);
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
auto handleFeatherWMCOMMAND(WPARAM const& wparam, HWND hwndlg) -> bool {
  switch (LOWORD(wparam)) {
	case IDCANCEL: {
	  EndDialog(hwndlg, FALSE);
	  return true;
	}
	case IDOK: {
	  handleFeatherIDOK(hwndlg);
	  break;
	}
	default: {
	  outDebugString(L"default hit in fthdefprc 1: wparam [{}]\n", LOWORD(wparam));
	  break;
	}
  }
  return false;
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handleFeatherWMINITDIALOG(HWND hwndlg) {
  auto const featherType = IniFile.featherType;
  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
  SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), format(FMT_COMPILE(L"{:.2f}"), IniFile.featherRatio).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), format(FMT_COMPILE(L"{}"), IniFile.featherUpCount).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT),
                format(FMT_COMPILE(L"{}"), IniFile.featherDownCount).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_DFLR),
                format(FMT_COMPILE(L"{:.2f}"), IniFile.featherMinStitchSize * IPFGRAN).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), format(FMT_COMPILE(L"{}"), IniFile.featherCount).c_str());
  auto featherStyle = std::wstring {};
  for (auto const& iFeatherStyle : FTHRLIST) {
	featherStyle.assign(displayText::loadStr(iFeatherStyle.stringID));
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
	SendMessage(GetDlgItem(hwndlg, IDC_FDTYP),
	            CB_ADDSTRING,
	            0,
	            reinterpret_cast<LPARAM>(featherStyle.c_str())); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  }
  auto const wParam = IniFile.featherFillType - 1;
  SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, wParam, 0);
  auto state = wrap::toUnsigned((featherType & AT_FTHBLND) != 0 ? BST_CHECKED : BST_UNCHECKED);
  CheckDlgButton(hwndlg, IDC_FDBLND, state);
  state = (featherType & AT_FTHUP) != 0 ? BST_CHECKED : BST_UNCHECKED;
  CheckDlgButton(hwndlg, IDC_FDUP, state);
  state = (featherType & AT_FTHBTH) != 0 ? BST_CHECKED : BST_UNCHECKED;
  CheckDlgButton(hwndlg, IDC_FBTH, state);
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
auto handleLockWMCOMMAND(HWND hwndlg, WPARAM const& wparam) -> bool {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
  if (auto* fileInfo = reinterpret_cast<std::vector<WIN32_FIND_DATA>*>(GetWindowLongPtr(hwndlg, DWLP_USER));
      fileInfo != nullptr) {
	constexpr auto NROMASK = BIGDWORD ^ FILE_ATTRIBUTE_READONLY; // invert FILE_ATTRIBUTE_READONLY
	auto&          spFileInfo = *fileInfo;
	switch (LOWORD(wparam)) {
	  case IDCANCEL: {
		EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
		return true;
	  }
	  case IDC_LOCKAL: {
		for (auto& iFile : spFileInfo) {
		  iFile.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
		}
		ritlock(spFileInfo, hwndlg);
		break;
	  }
	  case IDC_UNLOCKAL: {
		for (auto& iFile : spFileInfo) {
		  iFile.dwFileAttributes &= NROMASK;
		}
		ritlock(spFileInfo, hwndlg);
		break;
	  }
	  case IDC_LOCK: {
		auto fileError = 0U;
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto const unlockHandle = GetDlgItem(hwndlg, IDC_UNLOCKED);
		for (auto& iFile : spFileInfo) {
		  if ((iFile.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0U) {
			if (SendMessage(unlockHandle, LB_GETSEL, fileError, 0)) {
			  iFile.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
			}
			++fileError;
		  }
		}
		ritlock(spFileInfo, hwndlg);
		break;
	  }
	  case IDC_UNLOCK: {
		auto fileError = 0U;
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto const lockHandle = GetDlgItem(hwndlg, IDC_LOCKED);
		for (auto& iFile : spFileInfo) {
		  if ((iFile.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0U) {
			if (SendMessage(lockHandle, LB_GETSEL, fileError, 0)) {
			  iFile.dwFileAttributes &= NROMASK;
			}
			++fileError;
		  }
		}
		ritlock(spFileInfo, hwndlg);
		break;
	  }
	  case IDOK: {
		auto fileError = 0U;
		for (auto& iFile : spFileInfo) {
		  auto& cFileName = iFile.cFileName;
		  if (auto fileName = ThrSingle->DefaultDirectory / std::begin(cFileName);
		      !SetFileAttributes(fileName.wstring().c_str(), iFile.dwFileAttributes)) {
			++fileError;
		  }
		}
		if (fileError != 0U) {
		  displayText::showMessage(IDS_LOCKNOT, fileError);
		}
		EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
		return true;
	  }
	  default: {
		outDebugString(L"default hit in lockPrc 1: wparam [{}]\n", LOWORD(wparam));
		break;
	  }
	}
  }
  return false;
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
auto handleLockWMINITDIALOG(HWND hwndlg, LPARAM lparam, WPARAM const& wparam) -> bool {
  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
  SetWindowLongPtr(hwndlg, DWLP_USER, lparam);
  if (lparam != 0U) {
	// ToDo - investigate C++17 option shown here: https://web.archive.org/web/20220812120940/https://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
	auto*      fileInfo   = reinterpret_cast<std::vector<WIN32_FIND_DATA>*>(lparam);
	auto const searchName = ThrSingle->DefaultDirectory / L"*.thr";
	auto       result     = WIN32_FIND_DATA {};
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const searchResult = FindFirstFile(searchName.wstring().c_str(), &result);
	if (searchResult == INVALID_HANDLE_VALUE) {
	  displayText::showMessage(IDS_NOTHRFIL, ThrSingle->DefaultDirectory.wstring());
	  EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
	  return true;
	}
	fileInfo->push_back(result);

	while (FindNextFile(searchResult, &result)) {
	  fileInfo->push_back(result);
	}
	ritlock(*fileInfo, hwndlg);
  }
  return false;
}
// ReSharper restore CppParameterMayBeConst

auto handleNumericInput(wchar_t const& code, bool& retflag) -> bool {
  retflag         = true;
  auto& msgBuffer = ThrSingle->MsgBuffer;
  if (Instance->stateMap.test(StateFlag::SCLPSPAC) && (code == VK_OEM_MINUS || code == VK_SUBTRACT)) {
	if (msgBuffer.front() != '-') {
	  thred::resetMsgBuffer();
	  msgBuffer.back() = '-';
	  msgBuffer.push_back(0);
	  SetWindowText(GeneralNumberInputBox, msgBuffer.data());
	}
	return true;
  }
  if (dunum(code)) {
	if (Instance->stateMap.test(StateFlag::TRNIN0)) {
	  trace::traceNumberInput(NumericCode);
	}
	else {
	  msgBuffer.back() = NumericCode;
	  msgBuffer.push_back(0);
	  SetWindowText(GeneralNumberInputBox, msgBuffer.data());
	}
	return true;
  }
  switch (code) {
	case VK_DECIMAL:      // numpad period
	case VK_OEM_PERIOD: { // period
	  // ToDo - only allow entry if there is not already a period in the buffer
	  msgBuffer.back() = '.';
	  msgBuffer.push_back(0);
	  SetWindowText(GeneralNumberInputBox, msgBuffer.data());
	  return true;
	}
	case VK_BACK: { // backspace
	  if (Instance->stateMap.test(StateFlag::TRNIN0)) {
		trace::traceNumberReset();
	  }
	  if (msgBuffer.size() > 1) {
		msgBuffer.pop_back();
		msgBuffer.back() = 0;
		SetWindowText(GeneralNumberInputBox, msgBuffer.data());
	  }
	  return true;
	}
	case VK_RETURN: {
	  if (Instance->stateMap.test(StateFlag::TRNIN0)) {
		trace::dutrnum2();
	  }
	  else {
		if (Instance->stateMap.test(StateFlag::TRNIN1)) {
		  trace::dutrnum1();
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in handleNumericInput: code [{}]\n", code);
	  break;
	}
  }
  retflag = false;
  return {};
}

// ReSharper disable CppParameterMayBeConst
void handleSizeRestored(HWND p_hWnd) {
  Instance->userFlagMap.reset(UserFlag::SAVMAX);
  chkirct();
  if (Instance->stateMap.testAndSet(StateFlag::SIZED)) {
	auto       screenCenterOffset = LONG {};
	auto const maxWindowDimension = SIZE {ScreenSizePixels.cx - 30, ScreenSizePixels.cy - 30};
	if (ThredWindowRect.right - ThredWindowRect.left > maxWindowDimension.cx) {
	  screenCenterOffset    = (ScreenSizePixels.cx - maxWindowDimension.cx) / 2;
	  ThredWindowRect.left  = screenCenterOffset;
	  ThredWindowRect.right = ScreenSizePixels.cx - screenCenterOffset;
	}
	if (ThredWindowRect.bottom - ThredWindowRect.top > maxWindowDimension.cy) {
	  screenCenterOffset     = (ScreenSizePixels.cy - maxWindowDimension.cy) / 2;
	  ThredWindowRect.top    = screenCenterOffset;
	  ThredWindowRect.bottom = ScreenSizePixels.cy - screenCenterOffset;
	}
	if (screenCenterOffset != 0) {
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
}
// ReSharper restore CppParameterMayBeConst

auto handleWndMsgWMKEYDOWN(FRM_HEAD& textureForm, F_POINT& rotationCenter, std::vector<POINT> const& stretchBoxLine)
    -> bool {
  auto const code = gsl::narrow<wchar_t>(WinMsg.wParam & 0xffffU);
  if (Instance->stateMap.test(StateFlag::TXTRED)) {
	texture::txtkey(code, textureForm);
	return true;
  }
  // ToDo - value passed to duform is weird because it is dependant on order of enumeration of the form types.
  //        and value 'SAT' throws it off
  if (Instance->stateMap.test(StateFlag::FORMIN)) {
	if (wrap::pressed(VK_CONTROL)) {
	  return true;
	}
	if (keys::createFormType(code)) {
	  return true;
	}
  }
  if (Instance->stateMap.test(StateFlag::FILMSG)) {
	if (code == VK_RETURN || code == VK_OEM_3) { // check for return or back tick '`'
	  thred::savdo();
	  form::unfil();
	  thred::coltab();
	  thred::unmsg();
	  return true;
	}
  }
  if (Instance->stateMap.testAndReset(StateFlag::MOVMSG)) {
	if (code == VK_RETURN || code == VK_OEM_3) {
	  thred::savdo();
	  if (!Instance->selectedFormList.empty()) {
		for (auto const selectedForm : Instance->selectedFormList) {
		  form::refilfn(selectedForm);
		}
	  }
	  else {
		form::refilfn(ClosestFormToCursor);
	  }
	  thred::unmsg();
	}
	else {
	  backup::bak();
	}
	if (Instance->stateMap.testAndReset(StateFlag::WASFRMFRM)) {
	  formForms::refrm();
	}
	return true;
  }
  if (Instance->stateMap.testAndReset(StateFlag::PRGMSG)) {
	if (code == VK_RETURN || code == VK_OEM_3) {
	  thred::deldir();
	  return true;
	}
  }
  if (Instance->stateMap.testAndReset(StateFlag::DELSFRMS)) {
	if (code == L'S' || code == VK_RETURN || code == VK_OEM_3) {
	  thred::savdo();
	  if (code == L'S') {
		Instance->stateMap.set(StateFlag::DELTO);
	  }
	  else {
		Instance->stateMap.reset(StateFlag::DELTO);
	  }
	  thred::delsfrms();
	  thred::coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  thred::unmsg();
	  return true;
	}
  }
  if (Instance->stateMap.testAndReset(StateFlag::DELFRM)) {
	if (code == L'S' || code == VK_RETURN || code == VK_OEM_3) {
	  thred::savdo();
	  if (code == L'S') {
		Instance->stateMap.set(StateFlag::DELTO);
	  }
	  else {
		Instance->stateMap.reset(StateFlag::DELTO);
	  }
	  thred::frmdel();
	  fndknt();
	  thred::coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  thred::unmsg();
	  return true;
	}
  }
  if (Instance->stateMap.test(StateFlag::THUMSHO)) {
	keys::navigateThumbnails(code);
	return true;
  }
  auto& msgBuffer             = ThrSingle->MsgBuffer;
  auto& sideWindowEntryBuffer = ThrSingle->SideWindowEntryBuffer;
  if (Instance->stateMap.test(StateFlag::FSETFSPAC) || Instance->stateMap.test(StateFlag::GTWLKIND)) {
	// Check for keycode 'dash' and numpad 'subtract'
	if (code == VK_OEM_MINUS || code == VK_SUBTRACT) {
	  if (msgBuffer.front() != '-') {
		thred::resetMsgBuffer();
		msgBuffer.back() = '-';
		msgBuffer.push_back(0);
		SetWindowText(GeneralNumberInputBox, msgBuffer.data());
	  }
	  return true;
	}
  }
  if (FormMenuChoice != 0U || PreferenceIndex != 0U) {
	if (chkminus(code)) {
	  if (sideWindowEntryBuffer.front() != '-') {
		thred::resetSideBuffer();
		sideWindowEntryBuffer.operator[](SideWinMsgIdx++) = '-';
		SetWindowText(SideMessageWindow, sideWindowEntryBuffer.data());
	  }
	  return true;
	}
	if (dunum(code)) {
	  if (PreferenceIndex == PRFGRDCUT + 1 || PreferenceIndex == PRFSBXCUT + 1U) {
		auto buffer             = std::array<wchar_t, 2> {};
		buffer[0]               = NumericCode;
		auto const& valueWindow = Instance->valueWindow;
		if (PreferenceIndex == PRFGRDCUT + 1U) {
		  ShowStitchThreshold = unthrsh(NumericCode - L'0');
		  SetWindowText(valueWindow.operator[](PRFGRDCUT), buffer.data());
		}
		else {
		  StitchBoxesThreshold = unthrsh(NumericCode - L'0');
		  SetWindowText(valueWindow.operator[](PRFSBXCUT), buffer.data());
		}
		thred::unsid();
	  }
	  else {
		if (SideWinMsgIdx < sideWindowEntryBuffer.size() - 1U) {
		  sideWindowEntryBuffer.operator[](SideWinMsgIdx++) = NumericCode;
		  sideWindowEntryBuffer.operator[](SideWinMsgIdx)   = 0;
		  SetWindowText(SideMessageWindow, sideWindowEntryBuffer.data());
		}
	  }
	  return true;
	}
	if (keys::otherKeys(code)) {
	  return true;
	}
  }
  if (code == L'I') {
	thred::movi();
	LastKeyCode = L'I';
	return true;
  }
  if (code == L'Q' && LastKeyCode == L'Q') {
	thred::unpat();
  }
  LastKeyCode = code;
  if (Instance->stateMap.test(StateFlag::NUMIN)) {
	{
	  auto       retflag = true;
	  auto const retval  = handleNumericInput(code, retflag);
	  if (retflag) {
		return retval;
	  }
	}
  }
  if (Instance->stateMap.testAndReset(StateFlag::ENTRDUP)) {
	if (msgBuffer.size() > 1) {
	  outDebugString(L"chknum: buffer length [{}] size [{}]\n", wcslen(msgBuffer.data()), msgBuffer.size());
	  if (auto const value = thred::getMsgBufferValue(); value != 0.0F) {
		IniFile.rotationAngle = value * DEGRADF;
	  }
	}
	if (IniFile.rotationAngle != 0.0F) {
	  form::dupfn(IniFile.rotationAngle);
	}
  }
  if (Instance->stateMap.testAndReset(StateFlag::ENTROT)) {
	if (msgBuffer.size() > 1) {
	  outDebugString(L"chknum: buffer length [{}] size [{}]\n", wcslen(msgBuffer.data()), msgBuffer.size());
	  if (auto const value = thred::getMsgBufferValue(); value != 0.0F) {
		IniFile.rotationAngle = value * DEGRADF;
	  }
	}
	rotfns(IniFile.rotationAngle);
  }
  thred::unmsg();
  return keys::handleMainWinKeys(code, rotationCenter, stretchBoxLine);
}

// ReSharper disable CppParameterMayBeConst
auto handleWndProcWMDRAWITEM(LPARAM lParam) -> bool {
// owner draw windows
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
  DrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
  if (DrawItem->hwndItem == MainStitchWin && DrawItem->itemAction == ODA_DRAWENTIRE) {
	if (Instance->stateMap.test(StateFlag::TXTRED)) {
	  texture::drwtxtr();
	  return true;
	}
	if (!Instance->stateMap.test(StateFlag::RUNPAT)) {
	  if (!Instance->stateMap.test(StateFlag::HIDSTCH) &&
	      (!ThrSingle->ThrName.empty() || Instance->stateMap.test(StateFlag::INIT) ||
	       !Instance->formList.empty() || Instance->stateMap.test(StateFlag::SATPNT)) &&
	      !Instance->stateMap.test(StateFlag::BAKSHO)) {
		drwStch();
	  }
	  else {
		drawBackground();
		if (Instance->stateMap.test(StateFlag::HIDSTCH)) {
		  form::drwfrm();
		  if (Instance->stateMap.test(StateFlag::SATPNT)) {
			satin::satzum();
		  }
		}
		if (Instance->stateMap.test(StateFlag::PRFACT)) {
		  thred::redraw(PreferencesWindow);
		}
		else {
		  if (Instance->stateMap.test(StateFlag::MOVFRM)) {
			form::unfrm();
			Instance->stateMap.set(StateFlag::SHOFRM);
			form::dufrm();
		  }
		  if (Instance->stateMap.test(StateFlag::POLIMOV)) {
			Instance->stateMap.set(StateFlag::SHOFRM);
			form::mdufrm();
		  }
		}
	  }
	  BitBlt(StitchWindowDC,                // handle to destination DC
	         0,                             // x coordinate of destination upper-left corner
	         0,                             // y coordinate of destination upper-left corner
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
	  if (Instance->stateMap.test(StateFlag::ROTSHO)) {
		durot();
	  }
	  if (Instance->stateMap.test(StateFlag::SHOSAT)) {
		satin::dusat();
	  }
	  if (Instance->stateMap.test(StateFlag::SHOINSF)) {
		form::duinsf();
	  }
	}
	return true;
  }
  if (DrawItem->hwndItem == ColorBar && DrawItem->itemAction == ODA_DRAWENTIRE) {
	if (!Instance->stitchBuffer.empty()) {
	  dubar();
	}
	else {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_WINDOW));
	}
	return true;
  }
  if (!Instance->buttonWin.empty() && DrawItem->hwndItem == Instance->buttonWin.operator[](HHID) &&
      DrawItem->itemAction == ODA_DRAWENTIRE) {
	auto const position = (ButtonWidthX3 - PickColorMsgSize.cx) / 2;
	if (Instance->stateMap.test(StateFlag::HID)) {
	  auto const ucb         = wrap::next(UserColorBrush.begin(), ActiveColor);
	  auto const itUserColor = wrap::next(UserColor.begin(), ActiveColor);
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, *ucb);
	  SetBkColor(DrawItem->hDC, *itUserColor);
	}
	else {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_BTNFACE));
	}
	if (Instance->stateMap.test(StateFlag::TXTRED)) {
	  texture::writeScreenWidth(position);
	}
	else {
	  auto const pikol = displayText::loadStr(IDS_PIKOL);
	  wrap::textOut(DrawItem->hDC, position, 1, pikol.c_str(), wrap::toUnsigned(pikol.size()));
	}
	return true;
  }
  if (Instance->stateMap.test(StateFlag::WASTRAC)) {
	trace::wasTrace();
	return true;
  }
  auto dcb = DefaultColorBrush.begin();
  auto ucb = UserColorBrush.begin();
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	if (DrawItem->hwndItem == ThrSingle->DefaultColorWin.operator[](iColor)) {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, *dcb);
	  if (DisplayedColorBitmap.test(iColor)) {
		SetBkColor(DrawItem->hDC, DEFAULT_COLORS.at(iColor));
		SetTextColor(DrawItem->hDC, defTxt(iColor));
		auto const colorNum = format(FMT_COMPILE(L"{}"), iColor + 1U);
		auto       textSize = SIZE {};
		wrap::getTextExtentPoint32(DrawItem->hDC, colorNum.c_str(), wrap::toUnsigned(colorNum.size()), &textSize);
		wrap::textOut(DrawItem->hDC,
		              (ButtonWidth - textSize.cx) / 2,
		              0,
		              colorNum.c_str(),
		              wrap::toUnsigned(colorNum.size()));
	  }
	  return true;
	}
	if (DrawItem->hwndItem == ThrSingle->UserColorWin.operator[](iColor)) {
	  FillRect(DrawItem->hDC, &DrawItem->rcItem, *ucb);
	  if (iColor == ActiveColor) {
		SelectObject(DrawItem->hDC, CrossPen);
		SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
		auto line = std::array<POINT, 2> {};
		line[0]   = POINT {DrawItem->rcItem.right / 2, 0};
		line[1]   = POINT {DrawItem->rcItem.right / 2, DrawItem->rcItem.bottom};
		wrap::polyline(DrawItem->hDC, line.data(), wrap::toUnsigned(line.size()));
		line[0] = POINT {0, DrawItem->rcItem.bottom / 2};
		line[1] = POINT {DrawItem->rcItem.right, DrawItem->rcItem.bottom / 2};
		wrap::polyline(DrawItem->hDC, line.data(), wrap::toUnsigned(line.size()));
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	  }
	  return true;
	}
	++dcb;
	++ucb;
  }
  if (Instance->stateMap.test(StateFlag::BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {
	if (Instance->stateMap.test(StateFlag::THUMSHO)) {
	  auto itHWndBV = BackupViewer.begin();
	  for (auto iThumb = uint32_t {}; iThumb < QUADRT; ++iThumb) {
		if (iThumb < ThumbnailDisplayCount && DrawItem->hwndItem == *itHWndBV) {
		  ritbak(ThrSingle->Thumbnails.operator[](ThumbnailsSelected.at(iThumb)).data(), *DrawItem);
		  rthumnam(iThumb);
		  return true;
		}
		++itHWndBV;
	  }
	}
	else {
	  auto itHWndBV = BackupViewer.begin();
	  for (auto iVersion = wchar_t {}; iVersion < OLDVER; ++iVersion) {
		if (DrawItem->hwndItem == *itHWndBV) {
		  auto fileName = ThrSingle->ThrName; // intentional copy
		  auto ext      = fileName.extension().wstring();
		  ext.back()    = iVersion + 's';
		  fileName.replace_extension(ext);
		  ritbak(fileName, *DrawItem);
		  return true;
		}
		++itHWndBV;
	  }
	}
  }
  return false;
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
auto handleWndProcWMHSCROLL(WPARAM const& wParam, float const LINSCROL, LPARAM lParam) -> bool {
  constexpr auto SPEDLIN = int32_t {30};  // speed change for line message on speed scroll bar
  constexpr auto SPEDPAG = int32_t {120}; // speed change for page message on speed scroll bar
  switch (gsl::narrow<int32_t>(LOWORD(wParam))) {
	case SB_LINELEFT: {
	  if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
		MovieTimeStep += SPEDLIN;
		MovieTimeStep = std::min(MovieTimeStep, MAXDELAY);
		setsped();
		SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
	  }
	  else {
		auto scrollPoint = POINT {}; // for scroll bar functions
		scrollPoint.x    = std::lround((ZoomRect.right - ZoomRect.left) * LINSCROL);
		if (scrollPoint.x == 0) {
		  scrollPoint.x = 1;
		}
		rshft(scrollPoint);
	  }
	  return true;
	}
	case SB_LINERIGHT: {
	  if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
		MovieTimeStep -= SPEDLIN;
		MovieTimeStep = std::max(MovieTimeStep, MINDELAY);
		setsped();
		SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
	  }
	  else {
		auto scrollPoint = POINT {}; // for scroll bar functions
		scrollPoint.x    = std::lround(-(ZoomRect.right - ZoomRect.left) * LINSCROL);
		if (scrollPoint.x == 0) {
		  scrollPoint.x = -1;
		}
		rshft(scrollPoint);
	  }
	  return true;
	}
	case SB_PAGELEFT: {
	  if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
		MovieTimeStep += SPEDPAG;
		MovieTimeStep = std::max(MovieTimeStep, MINDELAY);
		setsped();
		SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
	  }
	  else {
		thred::pglft();
	  }
	  return true;
	}
	case SB_PAGERIGHT: {
	  if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
		MovieTimeStep -= SPEDPAG;
		MovieTimeStep = std::max(MovieTimeStep, MINDELAY);
		setsped();
		SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
	  }
	  else {
		thred::pgrit();
	  }
	  return true;
	}
	case SB_THUMBPOSITION: {
	  if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
		if (reinterpret_cast<HWND>(lParam) == SpeedScrollBar) {
		  auto const position = HIWORD(wParam);
		  MovieTimeStep       = MAXDELAY - position;
		  setsped();
		  SetScrollPos(SpeedScrollBar, SB_CTL, position, TRUE);
		}
	  }
	  else {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
		if (reinterpret_cast<HWND>(lParam) == HorizontalScrollBar) {
		  auto const zoomWidth = ZoomRect.right - ZoomRect.left;
		  ZoomRect.left        = wrap::toFloat(HIWORD(wParam));
		  ZoomRect.right       = ZoomRect.left + zoomWidth;
		  if (auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx); ZoomRect.right > unzoomedX) {
			ZoomRect.right = unzoomedX;
			ZoomRect.left  = unzoomedX - zoomWidth;
		  }
		  Instance->stateMap.set(StateFlag::RESTCH);
		}
	  }
	  return true;
	}
	default: {
	  outDebugString(L"default hit in wndProc 1: wParam [{}]\n", LOWORD(wParam));
	  break;
	}
  }
  return false;
}
// ReSharper restore CppParameterMayBeConst

void handleWndProcWMINITMENU() {
  if (Instance->stateMap.testAndReset(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	Instance->stateMap.reset(StateFlag::WASRT);
  }
  thred::unmsg();
  thred::undat();
  Instance->stateMap.reset(StateFlag::FORMIN);
  Instance->stateMap.set(StateFlag::RESTCH);
}

// ReSharper disable CppParameterMayBeConst
void handleWndProcWMMOVE(HWND p_hWnd) {
  GetClientRect(p_hWnd, &ThredWindowRect);
  constexpr auto SMALLWIN  = LONG {20};  // Smallest ThrEd window dimension
  constexpr auto DEFWINDIM = LONG {300}; // default small Thred window dimension
  if (ThredWindowRect.right - ThredWindowRect.left < SMALLWIN) {
	ThredWindowRect.left  = 0;
	ThredWindowRect.right = DEFWINDIM;
  }
  if (ThredWindowRect.bottom - ThredWindowRect.top < SMALLWIN) {
	ThredWindowRect.top    = 0;
	ThredWindowRect.bottom = DEFWINDIM;
  }
  thred::movStch();
  if (Instance->stateMap.test(StateFlag::RUNPAT)) {
	thred::duzrat();
	RunPoint = 0;
	FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
  }
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handleWndProcWMSIZE(HWND p_hWnd, WPARAM const wParam) {
  GetClientRect(p_hWnd, &ThredWindowRect);
  switch (wParam) {
	case SIZE_MAXIMIZED: {
	  Instance->userFlagMap.set(UserFlag::SAVMAX);
	  break;
	}
	case SIZE_MINIMIZED: {
	  Instance->userFlagMap.reset(UserFlag::SAVMAX);
	  break;
	}
	case SIZE_RESTORED: {
	  handleSizeRestored(p_hWnd);
	  break;
	}
	default: {
	  outDebugString(L"default hit in wndProc 3: wParam [{}]\n", wParam);
	  break;
	}
  }
  GetClientRect(p_hWnd, &ThredWindowRect);
  thred::movStch();
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	auto const bRatio = wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
	auto const adjustedWidth = wrap::toFloat(StitchWindowClientRect.right) / bRatio;
	if (auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx); adjustedWidth + ZoomRect.left > unzoomedX) {
	  ZoomRect.right = unzoomedX;
	  ZoomRect.left  = unzoomedX - adjustedWidth;
	}
	else {
	  ZoomRect.right = adjustedWidth + ZoomRect.left;
	}
  }
  else {
	ZoomRect =
	    F_RECTANGLE {0.0F, gsl::narrow<float>(UnzoomedRect.cy), gsl::narrow<float>(UnzoomedRect.cx), 0.0F};
  }
  thred::resetNearest();
  Instance->stateMap.set(StateFlag::RESTCH);
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	shft2box();
  }
  if (Instance->stateMap.test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	if (Instance->stateMap.test(StateFlag::ZUMED)) {
	  RunPoint = GroupStartStitch;
	}
	else {
	  RunPoint = 0;
	}
  }
}
// ReSharper restore CppParameterMayBeConst

auto handleWndProcWMVSCROLL(WPARAM const& wParam, float const LINSCROL) -> bool {
  switch (LOWORD(wParam)) {
	case SB_LINEDOWN: {
	  auto scrollPoint = POINT {};
	  scrollPoint.y    = std::lround((ZoomRect.top - ZoomRect.bottom) * LINSCROL);
	  if (scrollPoint.y == 0) {
		scrollPoint.y = 1;
	  }
	  rshft(scrollPoint);
	  return true;
	}
	case SB_LINEUP: {
	  auto scrollPoint = POINT {};
	  scrollPoint.y    = std::lround(-(ZoomRect.top - ZoomRect.bottom) * LINSCROL);
	  if (scrollPoint.y == 0) {
		scrollPoint.y = -1;
	  }
	  rshft(scrollPoint);
	  return true;
	}
	case SB_PAGEDOWN: {
	  thred::pgdwn();
	  return true;
	}
	case SB_PAGEUP: {
	  thred::pgup();
	  return true;
	}
	case SB_THUMBPOSITION: {
	  auto const zoomHeight = ZoomRect.top - ZoomRect.bottom;

	  ZoomRect.top    = wrap::toFloat(UnzoomedRect.cy) - wrap::toFloat(HIWORD(wParam));
	  ZoomRect.bottom = ZoomRect.top - zoomHeight;
	  if (ZoomRect.bottom < 0) {
		ZoomRect.bottom = 0.0F;
		ZoomRect.top    = zoomHeight;
	  }
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return true;
	}
	default: {
	  outDebugString(L"default hit in wndProc 2: wParam [{}]\n", LOWORD(wParam));
	  break;
	}
  }
  return false;
}

// ReSharper disable CppParameterMayBeConst
void hidwnd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_HIDE);
}
// ReSharper restore CppParameterMayBeConst

void infadj(float& xCoordinate, float& yCoordinate) noexcept {
  if (isfinite(xCoordinate)) {
	auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx);
	xCoordinate          = std::clamp(xCoordinate, 0.0F, unzoomedX);
  }
  if (isfinite(yCoordinate)) {
	auto const unzoomedY = wrap::toFloat(UnzoomedRect.cy);
	yCoordinate          = std::clamp(yCoordinate, 0.0F, unzoomedY);
  }
}

void init() {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const deviceContext = GetDC(nullptr);
  ReleaseDC(nullptr, deviceContext);
  Instance->texturePointsBuffer.clear();
  LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1));
  menu::init();
  menu::qchk();
  mouse::crtcurs();
  menu::redfils(LRU_MENU_ID, ThrSingle->PreviousNames);
  Instance->stateMap.reset(); // clear the bitmap
  // set up the size variables
  ThredDC = GetDC(ThrEdWindow);
  SetStretchBltMode(ThredDC, COLORONCOLOR);
  StitchWindowDC    = GetDCEx(MainStitchWin, nullptr, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
  StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
  chkirct();
  if (!Instance->userFlagMap.test(UserFlag::SAVMAX)) {
	MoveWindow(ThrEdWindow,
	           IniFile.initialWindowCoords.left,
	           IniFile.initialWindowCoords.top,
	           IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left,
	           IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top,
	           0);
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const hFont = displayText::getThrEdFont(FONTSIZE);
  SelectObject(ThredDC, hFont);
  SelectObject(StitchWindowDC, hFont);
  constexpr auto TXTSIDS = int32_t {6}; // extra pixels in a text box
  auto const     size    = thred::txtWid(L"MM");

  ButtonWidth   = size.cx + TXTSIDS;
  ButtonWidthX3 = ButtonWidth * 3;
  ButtonHeight  = size.cy + 4;
  std::ranges::generate(BoxOffset, [step = int32_t {1}, offsetStepSize = thred::txtWid(L"0")]() mutable -> int32_t {
	return offsetStepSize.cx * step++;
  });
  GetClientRect(ThrEdWindow, &ThredWindowRect);
  stchWnd();
  ThreadSize.fill(L'4');
  constexpr auto DEFPNTPX = uint16_t {4U}; // default form and stitch point pixels
  if (IniFile.traceLength == 0.0F) {
	constexpr auto TRACLEN = 1.0F; // initial trace length
	IniFile.traceLength    = TRACLEN;
  }
  if (IniFile.traceRatio == 0.0F) {
	constexpr auto TRACRAT = 1.00001F; // initial trace ratio
	IniFile.traceRatio     = TRACRAT;
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
	constexpr auto DEFEGRAT = 1.5; // default egg ratio
	IniFile.eggRatio        = DEFEGRAT;
  }
  if (IniFile.stitchSizePixels == 0U) {
	IniFile.stitchSizePixels = DEFPNTPX;
  }
  if (IniFile.formVertexSizePixels == 0U) {
	IniFile.formVertexSizePixels = DEFPNTPX;
  }
  if (IniFile.formSides == 0U) {
	constexpr auto FSID = uint16_t {24U}; // initial number of sides
	IniFile.formSides   = FSID;
  }
  if (IniFile.tearTailLength == 0.0F) {
	constexpr auto TAILLEN = 1.4F; // initial tail length
	IniFile.tearTailLength = TAILLEN;
  }
  if (IniFile.underlayStitchLen == 0.0F) {
	IniFile.underlayStitchLen = DEFULEN;
  }
  if (IniFile.underlaySpacing == 0.0F) {
	IniFile.underlaySpacing = DEFUSPAC;
  }
  thred::setgrd(IniFile.gridColor);
  makCol(); // make the color change windows
  Instance->buttonWin.resize(BTNCOUNT);
  auto const blank = std::wstring {};
  for (auto iButton = 0U; iButton < BTNCOUNT; ++iButton) {
	auto windowFlags = gsl::narrow_cast<DWORD>(SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER);
	auto buttonTxt = std::wstring {};
	switch (iButton) {
	  case HBOXSEL: {
		windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		buttonTxt.assign(displayText::loadStr(IDS_BOXSEL));
		break;
	  }
	  case HUPTO: {
		windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		buttonTxt.assign(displayText::loadStr(IDS_UPOF));
		break;
	  }
	  case HHID: {
		windowFlags = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER;
		buttonTxt.assign(displayText::loadStr(IDS_PIKOL));
		break;
	  }
	  default: {
		buttonTxt.assign(blank);
	  }
	}
	Instance->buttonWin.operator[](iButton) =
	    CreateWindow(L"STATIC",
	                 buttonTxt.c_str(),
	                 windowFlags,
	                 0,
	                 ButtonHeight * (16 + gsl::narrow_cast<int32_t>(iButton)),
	                 ButtonWidthX3,
	                 ButtonHeight,
	                 ThrEdWindow,
	                 nullptr,
	                 ThrEdInstance,
	                 nullptr);
  }
  trace::initTraceWindows();
  ColorBar = CreateWindow(L"STATIC",
                          L"",
                          SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
                          ThredWindowRect.right - ColorBarSize,
                          0,
                          ColorBarSize,
                          ThredWindowRect.bottom,
                          ThrEdWindow,
                          nullptr,
                          ThrEdInstance,
                          nullptr);
  nuRct();
  // create pens
  static constexpr auto BOX_COLOR = std::array<COLORREF, 4> {0x404040, 0x408040, 0x804040, 0x404080};
  auto itBoxCOlor = BOX_COLOR.begin();
  std::ranges::generate(BoxPen, [&itBoxCOlor]() mutable noexcept -> HPEN {
	return wrap::createPen(PS_SOLID, PENNWID, *itBoxCOlor++);
  });
  LinePen        = wrap::createPen(PS_SOLID, PENNWID, PENCHCL);
  CrossPen       = wrap::createPen(PS_SOLID, PENWWID, PENPRPLE);
  GroupSelectPen = wrap::createPen(PS_SOLID, PENNWID, PENPRPLE);
  GridPen        = wrap::createPen(PS_SOLID, PENNWID, IniFile.gridColor);
  BackgroundPen  = wrap::createPen(PS_SOLID, PENMWID, BackgroundColor);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const bitmapPen = wrap::createPen(PS_SOLID, PENNWID, bitmap::getBmpColor());
  bitmap::setBitmapPen(bitmapPen);
  FormPen         = wrap::createPen(PS_SOLID, PENNWID, PENSILVR);
  MultiFormPen    = wrap::createPen(PS_SOLID, PENNWID, PENPOLIV);
  FormPen3px      = wrap::createPen(PS_SOLID, PENMWID, PENSILVR);
  FormSelectedPen = wrap::createPen(PS_SOLID, PENNWID, PENSEGRN);
  ZoomMarkPen     = wrap::createPen(PS_SOLID, PENMWID, PENLMGRN);
  SelectAllPen    = wrap::createPen(PS_SOLID, PENNWID, PENROSY);
  KnotPen         = wrap::createPen(PS_SOLID, PENNWID, PENWHITE);
  form::setLayerPens();
  BackgroundPenWidth = 1;
  auto tsp           = ThreadSizePixels.begin();
  auto tsi           = ThreadSizeIndex.begin();
  auto itUserPen     = ThrSingle->UserPen.begin();
  for (auto const& color : UserColor) {
	*tsp++       = 1;
	*tsi++       = 1;
	*itUserPen++ = wrap::createPen(PS_SOLID, PENNWID, color);
  }
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  // create brushes
  createBrushes();
  ZoomFactor = 1;
  Instance->stitchBuffer.clear();
  GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
  menu::ladj();
  auto const pikol = displayText::loadStr(IDS_PIKOL);
  wrap::getTextExtentPoint32(ThredDC, pikol.c_str(), wrap::toUnsigned(pikol.size()), &PickColorMsgSize);
  menu::auxmen();
  fnamtabs();
  ritfnam(ThrSingle->DesignerName);
  auto       designer       = utf::utf16ToUtf8(ThrSingle->DesignerName);
  auto const spModifierName = gsl::span {ExtendedHeader.modifierName};
  std::ranges::copy(designer, spModifierName.begin());
  spModifierName[designer.length()] = 0;
  thred::chkhup();
  menu::nedmen();
  menu::fil2men();
  menu::knotmen();
  menu::bsavmen();
  menu::rotauxmen();
  menu::linbmen();
  menu::wrnmen();
  ritloc();
  menu::frmcurmen();
  texture::redtx();
  Instance->stateMap.set(StateFlag::HIDMAP);
  thred::hidbit();
  menu::chkmen();
  // check command line-should be last item in init
  ducmd();
  auto const fmtStr = displayText::format(IDS_THRED, ThrSingle->DesignerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
}

void insadj() {
  Instance->stateMap.reset(StateFlag::PRELIN);
  form::refil(ClosestFormToCursor);
  thred::coltab();
  Instance->formList.operator[](ClosestFormToCursor).outline();
}

void inscol() {
  auto colorMap = boost::dynamic_bitset<>(COLORCNT);
  VerticalIndex &= COLMSK;
  for (auto const& stitch : Instance->stitchBuffer) {
	colorMap.set(stitch.attribute & COLMSK);
  }
  if (colorMap.all()) {
	displayText::tabmsg(IDS_COLAL, false);
	return;
  }
  auto nextColor = COLORMAX;
  while (colorMap.test(nextColor)) {
	--nextColor;
  }
  for (auto& stitch : Instance->stitchBuffer) {
	auto const color = gsl::narrow<uint8_t>(stitch.attribute & COLMSK);
	if (color < VerticalIndex || color >= nextColor) {
	  continue;
	}
	stitch.attribute &= NCOLMSK;
	stitch.attribute |= color + 1U;
  }
  for (auto& iForm : Instance->formList) {
	if (iForm.fillType != 0U && iForm.fillColor >= VerticalIndex && iForm.fillColor < nextColor) {
	  ++iForm.fillColor;
	}
	if (iForm.fillType == FTHF && iForm.feather.color >= VerticalIndex && iForm.feather.color < nextColor) {
	  ++iForm.feather.color;
	}
	if (iForm.edgeType != 0U && iForm.borderColor >= VerticalIndex && iForm.borderColor < nextColor) {
	  ++iForm.borderColor;
	}
  }
  for (auto iColor = nextColor; iColor > VerticalIndex; --iColor) {
	auto const prevIndex = iColor - 1U;
	UserColor.at(iColor) = UserColor.at(prevIndex);
	thred::nuscol(iColor);
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto insTHR(fs::path const& insertedFile, F_RECTANGLE& insertedRectangle) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle =
      CreateFile(insertedFile.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::filnopn(IDS_FNOPN, insertedFile);
	return false;
  }
  auto fileHeader = THR_HEAD {};
  auto bytesRead  = DWORD {};
  if (!wrap::readFile(fileHandle, &fileHeader, sizeof(fileHeader), &bytesRead, L"ReadFile for fileHeader in insTHR")) {
	return false;
  }
  if ((fileHeader.headerType & SIGMASK) != THREDSIG) {
	displayText::tabmsg(IDS_NOTHR, false);
	CloseHandle(fileHandle);
	return false;
  }
  constexpr auto FRMW        = 5;
  constexpr auto HANDW       = 4;
  constexpr auto FRMPW       = 2;
  constexpr auto STCHW       = 1;
  auto           homscor     = 0U;
  auto           thredHeader = THR_HEAD_EX {};
  auto const     version     = (fileHeader.headerType & FTYPMASK) >> TBYTSHFT;
  auto&          formList    = Instance->formList;

  if (version != 0U) {
	gethand(Instance->stitchBuffer, wrap::toUnsigned(Instance->stitchBuffer.size()));
	// ToDo - replace constants with sizes of data structures?
	homscor = wrap::toUnsigned(formList.size()) * FRMW +
	          gethand(Instance->stitchBuffer, wrap::toUnsigned(Instance->stitchBuffer.size())) * HANDW +
	          wrap::toUnsigned(Instance->formVertices.size()) * FRMPW +
	          wrap::toUnsigned(Instance->stitchBuffer.size()) * STCHW;
	if (!wrap::readFile(fileHandle, &thredHeader, sizeof(thredHeader), &bytesRead, L"ReadFile for ThrEd Header in insTHR")) {
	  return false;
	}
  }
  thred::savdo();
  auto fileStitchBuffer = std::vector<F_POINT_ATTR> {};
  if (fileHeader.stitchCount != 0U) {
	fileStitchBuffer.resize(fileHeader.stitchCount);
	if (auto const bytesToRead = fileHeader.stitchCount * wrap::sizeofType(fileStitchBuffer);
	    !wrap::readFile(fileHandle, fileStitchBuffer.data(), bytesToRead, &bytesRead, L"ReadFile for fileStitchBuffer in insTHR")) {
	  return false;
	}
  }
  auto const formDataOffset = gsl::narrow<LONG>(bitmap::getBmpNameLength() + sizeof(BackgroundColor) +
                                                sizeof(UserColor) + sizeof(CustomColor) + TSSIZE);
  SetFilePointer(fileHandle, formDataOffset, nullptr, FILE_CURRENT);
  InsertedVertexIndex = wrap::toUnsigned(Instance->formVertices.size());
  InsertedFormIndex   = wrap::toUnsigned(formList.size());
  if (fileHeader.formCount != 0U) {
	auto const newFormVertexIndex = wrap::toUnsigned(Instance->formVertices.size());
	auto       newSatinGuideIndex = wrap::toUnsigned(Instance->satinGuides.size());
	auto       clipOffset         = wrap::toUnsigned(Instance->clipPoints.size());
	auto       textureOffset      = wrap::toUnsigned(Instance->texturePointsBuffer.size());
	if (version < 2) {
	  auto inFormList = std::vector<FRM_HEAD_O> {};
	  inFormList.resize(fileHeader.formCount);
	  auto const bytesToRead = fileHeader.formCount * wrap::sizeofType(inFormList);
	  if (!wrap::readFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, L"ReadFile for inFormList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inFormList.resize(bytesRead / wrap::sizeofType(inFormList));
		Instance->stateMap.set(StateFlag::BADFIL);
	  }
	  formList.reserve(formList.size() + inFormList.size());
	  formList.insert(formList.end(), inFormList.begin(), inFormList.end());
	}
	else {
	  auto inFormList = std::vector<FRM_HEAD_OUT> {};
	  inFormList.resize(fileHeader.formCount);
	  auto const bytesToRead = fileHeader.formCount * wrap::sizeofType(inFormList);
	  if (!wrap::readFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, L"ReadFile for inFormList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inFormList.resize(bytesRead / wrap::sizeofType(inFormList));
		Instance->stateMap.set(StateFlag::BADFIL);
	  }
	  formList.reserve(formList.size() + inFormList.size());
	  formList.insert(formList.end(), inFormList.begin(), inFormList.end());
	}
	auto vertexOffset = wrap::toUnsigned(Instance->formVertices.size());
	if (fileHeader.vertexCount != 0U) {
	  auto inVertexList = std::vector<F_POINT> {};
	  inVertexList.resize(fileHeader.vertexCount);
	  auto const bytesToRead = fileHeader.vertexCount * wrap::sizeofType(inVertexList);
	  if (!wrap::readFile(fileHandle, inVertexList.data(), bytesToRead, &bytesRead, L"ReadFile for inVertexList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inVertexList.resize(bytesRead / wrap::sizeofType(inVertexList));
		Instance->stateMap.set(StateFlag::BADFIL);
	  }
	  Instance->formVertices.reserve(Instance->formVertices.size() + inVertexList.size());
	  Instance->formVertices.insert(
	      Instance->formVertices.end(), inVertexList.cbegin(), inVertexList.cend());
	}
	else {
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	auto guideOffset = wrap::toUnsigned(Instance->satinGuides.size());
	if (fileHeader.dlineCount != 0U) {
	  auto inGuideList = std::vector<SAT_CON_OUT> {};
	  inGuideList.resize(fileHeader.dlineCount);
	  auto const bytesToRead = fileHeader.dlineCount * wrap::sizeofType(inGuideList);
	  if (!wrap::readFile(fileHandle, inGuideList.data(), bytesToRead, &bytesRead, L"ReadFile for inGuideList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inGuideList.resize(bytesRead / wrap::sizeofType(inGuideList));
		Instance->stateMap.set(StateFlag::BADFIL);
	  }
	  Instance->satinGuides.reserve(Instance->satinGuides.size() + inGuideList.size());
	  Instance->satinGuides.insert(Instance->satinGuides.end(), inGuideList.begin(), inGuideList.end());
	  newSatinGuideIndex += wrap::toUnsigned(inGuideList.size());
	}
	if (fileHeader.clipDataCount != 0U) {
	  auto inPointList = std::vector<F_POINT> {};
	  inPointList.resize(fileHeader.clipDataCount);
	  auto const bytesToRead = fileHeader.clipDataCount * wrap::sizeofType(Instance->clipPoints);
	  if (!wrap::readFile(fileHandle, inPointList.data(), bytesToRead, &bytesRead, L"ReadFile for inPointList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inPointList.resize(bytesRead / wrap::sizeofType(inPointList));
		Instance->stateMap.set(StateFlag::BADFIL);
	  }
	  Instance->clipPoints.reserve(Instance->clipPoints.size() + inPointList.size());
	  Instance->clipPoints.insert(Instance->clipPoints.end(), inPointList.begin(), inPointList.end());
	}
	if (thredHeader.texturePointCount != 0U) {
	  auto inTextureList = std::vector<TX_PNT> {};
	  inTextureList.resize(thredHeader.texturePointCount);
	  auto const bytesToRead = thredHeader.texturePointCount * wrap::sizeofType(inTextureList);
	  if (!wrap::readFile(fileHandle, inTextureList.data(), bytesToRead, &bytesRead, L"ReadFile for inTextureList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inTextureList.resize(bytesRead / wrap::sizeofType(inTextureList));
		Instance->stateMap.set(StateFlag::BADFIL);
	  }
	  Instance->texturePointsBuffer.reserve(Instance->texturePointsBuffer.size() + inTextureList.size());
	  Instance->texturePointsBuffer.insert(
	      Instance->texturePointsBuffer.end(), inTextureList.begin(), inTextureList.end());
	}
	// update the form pointer variables
	for (auto iFormList = InsertedFormIndex; iFormList < wrap::toUnsigned(formList.size()); ++iFormList) {
	  auto& formIter       = formList.operator[](iFormList);
	  formIter.vertexIndex = vertexOffset;
	  vertexOffset += formIter.vertexCount;
	  if (formIter.type == SAT && formIter.satinGuideCount != 0U) {
		formIter.satinGuideIndex = guideOffset;
		guideOffset += formIter.satinGuideCount;
	  }
	  if (formIter.isClip()) {
		formIter.clipIndex = clipOffset;
		clipOffset += formIter.clipCount;
	  }
	  if (formIter.isEdgeClipX()) {
		formIter.borderClipData = clipOffset;
		clipOffset += formIter.clipEntries;
	  }
	  if (formIter.isTexture()) {
		wrap::narrow(formIter.texture.index, textureOffset);
		textureOffset += formIter.texture.count;
	  }
	}
	if (newFormVertexIndex != Instance->formVertices.size()) {
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	if (newSatinGuideIndex != Instance->satinGuides.size()) {
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	if (clipOffset != Instance->clipPoints.size()) {
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	if (fileHeader.formCount != 0U) {
	  auto spVertices = gsl::span(Instance->formVertices).subspan(InsertedVertexIndex, fileHeader.vertexCount);

	  auto minX = BIGFLOAT;
	  auto minY = BIGFLOAT;
	  auto maxX = LOWFLOAT;
	  auto maxY = LOWFLOAT;
	  for (auto const& vertex : spVertices) {
		minX = std::min(minX, vertex.x);
		maxX = std::max(maxX, vertex.x);
		maxY = std::max(maxY, vertex.y);
		minY = std::min(minY, vertex.y);
	  }
	  insertedRectangle = F_RECTANGLE {minX, maxY, maxX, minY};
	}
  }
  if (fileHeader.stitchCount != 0U) {
	auto const encodedFormIndex = InsertedFormIndex << FRMSHFT;

	auto minX = insertedRectangle.left;
	auto minY = insertedRectangle.top;
	auto maxX = insertedRectangle.right;
	auto maxY = insertedRectangle.bottom;
	for (auto& stitch : fileStitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) != 0U) {
		auto const newAttribute = (stitch.attribute & FRMSK) + encodedFormIndex;
		stitch.attribute &= NFRMSK;
		stitch.attribute |= newAttribute;
	  }
	  minX = std::min(minX, stitch.x);
	  maxX = std::max(maxX, stitch.x);
	  minY = std::min(minY, stitch.y);
	  maxY = std::max(maxY, stitch.y);
	}
	insertedRectangle = F_RECTANGLE {minX, maxY, maxX, minY};
  }
  // ThrEd version 1 signature
  if (constexpr auto VERSION1 = 0x01U << TBYTSHFT; (fileHeader.headerType & VERSION1) != 0U) {
	// ToDo - Replace constants with sizes of data structures
	if (auto const filscor = (fileHeader.formCount * FRMW) +
	                         (gethand(fileStitchBuffer, fileHeader.stitchCount) * HANDW) +
	                         (fileHeader.vertexLen * FRMPW) + (fileHeader.stitchCount * STCHW);
	    filscor > homscor) {
	  auto const spEHCN = gsl::span {ExtendedHeader.creatorName};
	  std::ranges::copy(thredHeader.creatorName, spEHCN.begin());
	  redfnam(ThrSingle->DesignerName);
	  auto fmtStr = displayText::format2(IDS_THRDBY, ThrSingle->ThrName.wstring(), ThrSingle->DesignerName);
	  SetWindowText(ThrEdWindow, fmtStr.c_str());
	}
  }
  auto dest = Instance->stitchBuffer.end();
  Instance->stitchBuffer.insert(dest, fileStitchBuffer.cbegin(), fileStitchBuffer.cend());
  CloseHandle(fileHandle);
  return true;
}

// Is the point inside the box?
auto isInBox(POINT const& point, RECT const& box) noexcept -> bool {
  return point.x >= box.left && point.x <= box.right && point.y <= box.top && point.y >= box.bottom;
}

auto isthr(fs::path const& thredPath) -> bool {
  auto const extention = thredPath.extension().wstring();
  return extention.starts_with(L".th");
}

auto kjmp(std::vector<F_POINT_ATTR>& buffer, uint32_t start) -> uint32_t {
  while (start < wrap::toUnsigned(Instance->stitchBuffer.size() - 1U) && stlen(start) > KNOTLEN) {
	buffer.push_back(Instance->stitchBuffer.operator[](start++));
  }
  strtknt(buffer, start);
  return start;
}

void lenfn(uint32_t const startStitch, uint32_t const endStitch, uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex) {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  auto maxLength      = LOWFLOAT;
  auto minLength      = BIGFLOAT;
  smallestStitchIndex = 0U;
  largestStitchIndex  = 0U;

  auto const firstStitch = std::next(Instance->stitchBuffer.begin(), wrap::toPtrdiff(startStitch));
  auto const spStitches =
      std::ranges::subrange(firstStitch, wrap::next(Instance->stitchBuffer.begin(), endStitch));
  auto stitchFwd1 = std::next(firstStitch);
  for (auto index = startStitch; auto const& stitch : spStitches) {
	auto const deltaX = stitchFwd1->x - stitch.x;
	auto const deltaY = stitchFwd1->y - stitch.y;
	auto const length = (deltaX * deltaX) + (deltaY * deltaY);
	if (length > maxLength) {
	  maxLength          = length;
	  largestStitchIndex = index;
	}
	if (length < minLength) {
	  minLength           = length;
	  smallestStitchIndex = index;
	}
	++index;
	++stitchFwd1;
  }
  maxLength = std::sqrt(maxLength) * IPFGRAN;
  minLength = std::sqrt(minLength) * IPFGRAN;

  constexpr auto DIGITLIM = 10000.0F; // value that represents the max width that can be displayed
  if (fabs(maxLength) < DIGITLIM) {
	auto const strMax = displayText::format(IDS_LENMAX, maxLength);
	displayText::butxt(HMAXLEN, strMax);
  }
  if (fabs(minLength) < DIGITLIM) {
	auto const strMin = displayText::format(IDS_LENMIN, minLength);
	displayText::butxt(HMINLEN, strMin);
  }
}

void lensadj() {
  uncros();
  thred::unsel();
  ClosestPointIndex = CurrentStitchIndex;
  GroupStitchIndex  = CurrentStitchIndex + 1U;
  thred::rngadj();
  ducros(StitchWindowDC);
  thred::lenCalc();
  thred::selRct(StitchRangeRect);
  if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1 ||
      StitchRangeRect.left < ZoomRect.left + 1 || StitchRangeRect.right > ZoomRect.right - 1) {
	auto const stitchPoint =
	    F_POINT {((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left,
	             ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom};
	thred::shft(stitchPoint);
  }
  thred::nuAct(GroupStartStitch);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void loadColors() noexcept {
  auto const spISC  = gsl::span {IniFile.stitchColors};
  auto       iISC   = spISC.begin();
  auto const spISPC = gsl::span {IniFile.stitchPreferredColors};
  auto       iISPC  = spISPC.begin();
  auto const spIBPC = gsl::span {IniFile.backgroundPreferredColors};
  auto       iIBPC  = spIBPC.begin();
  auto       iCC    = CustomColor.begin();
  auto       iCBC   = CustomBackgroundColor.begin();
  for (auto& color : UserColor) {
	color   = *iISC++;
	*iCC++  = *iISPC++;
	*iCBC++ = *iIBPC++;
  }
}

// ReSharper disable CppParameterMayBeConst
auto CALLBACK lockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  switch (umsg) {
	case WM_INITDIALOG: {
	  if (handleLockWMINITDIALOG(hwndlg, lparam, wparam)) {
		return TRUE;
	  }
	  break;
	}
	case WM_COMMAND: {
	  if (handleLockWMCOMMAND(hwndlg, wparam)) {
		return TRUE;
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in lockPrc 2: umsg [{}]\n", umsg);
	  break;
	}
  }
  return FALSE;
}
// ReSharper restore CppParameterMayBeConst

void makCol() noexcept {
  auto buffer = std::array<wchar_t, 3> {};
  buffer[1]   = L'0';
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const hFont        = displayText::getThrEdFont(FONTSIZE);
  auto       dcw          = ThrSingle->DefaultColorWin.begin();
  auto       ucw          = ThrSingle->UserColorWin.begin();
  auto       itThreadSize = ThreadSize.begin();
  auto       yOffset      = int32_t {};

  for (auto& tsw : ThrSingle->ThreadSizeWin) {
	*dcw = CreateWindow(L"STATIC",
	                    nullptr,
	                    SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                    0,
	                    yOffset,
	                    ButtonWidth,
	                    ButtonHeight,
	                    ThrEdWindow,
	                    nullptr,
	                    ThrEdInstance,
	                    nullptr);
	displayText::setWindowFont(*dcw++, hFont);
	*ucw++ = CreateWindow(L"STATIC",
	                      nullptr,
	                      SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                      ButtonWidth,
	                      yOffset,
	                      ButtonWidth,
	                      ButtonHeight,
	                      ThrEdWindow,
	                      nullptr,
	                      ThrEdInstance,
	                      nullptr);

	buffer[0] = *itThreadSize++;
	tsw       = CreateWindow(L"STATIC",
                       buffer.data(),
                       SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                       ButtonWidth * 2,
                       yOffset,
                       ButtonWidth,
                       ButtonHeight,
                       ThrEdWindow,
                       nullptr,
                       ThrEdInstance,
                       nullptr);
	displayText::setWindowFont(tsw, hFont);
	yOffset += ButtonHeight;
  }
}

auto makbig(uint32_t const start, uint32_t finish) -> uint32_t {
  auto       newStitches = std::vector<F_POINT_ATTR> {};
  auto const newSize     = finish - start;
  newStitches.reserve(newSize); // we know that we will have at least this number of Stitches
  auto adcnt = 0U;
  --finish;
  auto stitchIt     = wrap::next(Instance->stitchBuffer.begin(), start);
  auto nextStitchIt = stitchIt + 1U;
  for (auto iSource = start; iSource < finish; ++iSource) {
	auto const delta = F_POINT {nextStitchIt->x - nextStitchIt->x, nextStitchIt->y - nextStitchIt->y};
	auto const length = std::hypot(delta.x, delta.y);
	newStitches.push_back(*stitchIt);
	if (length > IniFile.maxStitchLength) {
	  auto const stitchCount = std::ceil(length / UserStitchLength);
	  auto const step        = F_POINT {delta.x / stitchCount, delta.y / stitchCount};
	  auto       point       = F_POINT {stitchIt->x + step.x, stitchIt->y + step.y};
	  auto       attribute   = stitchIt->attribute;
	  if (attribute != nextStitchIt->attribute) {
		if ((attribute & NOTFRM) == 0U && (nextStitchIt->attribute & TYPMSK) != 0U) {
		  if ((attribute & FRMSK) != (nextStitchIt->attribute & FRMSK)) {
			attribute &= NTYPMSK;
			attribute |= NOTFRM;
		  }
		}
		else {
		  attribute &= NTYPMSK;
		  attribute |= NOTFRM;
		}
	  }
	  attribute &= ~KNOTMSK;
	  for (auto iStitch = 0U; iStitch < wrap::round<decltype(iStitch)>(stitchCount) - 1U; ++iStitch) {
		newStitches.emplace_back(point.x, point.y, attribute);
		point += step;
		++adcnt;
	  }
	}
	++stitchIt;
	++nextStitchIt;
  }
  // now copy stitches back up to the end of the original group
  std::copy(newStitches.begin(),
            wrap::next(newStitches.begin(), finish - start),
            wrap::next(Instance->stitchBuffer.begin(), start));
  // and then insert the remainder of the new stitches
  Instance->stitchBuffer.insert(wrap::next(Instance->stitchBuffer.begin(), finish),
                                wrap::next(newStitches.begin(), finish - start),
                                newStitches.end());
  return adcnt;
}

void movins() {
  if (!Instance->stateMap.test(StateFlag::INSRT)) {
	return;
  }
  if (Instance->stateMap.test(StateFlag::LIN1)) {
	Instance->stateMap.test(StateFlag::BAKEND)
	    ? thred::endpnt(thred::stch2px1(wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)))
	    : thred::endpnt(thred::stch2px1(0U));
  }
  else {
	thred::duIns();
  }
}

auto getSaveName(fs::path& fileName, FileIndices& fileType) -> bool {
  auto* pFileSave = gsl::narrow_cast<IFileSaveDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hResult = CoCreateInstance(
      CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  if (FAILED(hResult) || nullptr == pFileSave) {
	return false;
  }
#if PESACT
  static constexpr auto ALL_FILE_TYPES =
      std::array {FLTTHR, FLTPCS, FLTDST, FLTPES}; // All possible file types for save
#else
  static constexpr auto ALL_FILE_TYPES = std::array<COMDLG_FILTERSPEC, 4> {FLTTHR, FLTPCS, FLTDST}; // All possible file types for save
#endif
  hResult = pFileSave->SetFileTypes(wrap::toUnsigned(ALL_FILE_TYPES.size()), ALL_FILE_TYPES.data());
  hResult += pFileSave->SetFileTypeIndex(1);
  hResult += pFileSave->SetTitle(L"Save As");
  hResult += pFileSave->SetFileName(fileName.filename().c_str());
  hResult += pFileSave->SetDefaultExtension(L"thr");
  if (FAILED(hResult)) {
	return false;
  }
  hResult = pFileSave->Show(nullptr);
  if (FAILED(hResult)) {
	return false;
  }
  auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
  hResult     = pFileSave->GetResult(&pItem);
  if (FAILED(hResult) || nullptr == pItem) {
	return false;
  }
  auto reply = 0U;
  hResult    = pFileSave->GetFileTypeIndex(&reply);
  if (FAILED(hResult)) {
	return false;
  }
  switch (reply) {
	case 1: {
	  fileType = FileIndices::THR;
	  break;
	}
	case 2: {
	  fileType = FileIndices::PCS;
	  break;
	}
	case 3: {
	  fileType = FileIndices::DST;
	  break;
	}
#if PESACT
	case 4: {
	  fileType = FileIndices::PES;
	  break;
	}
#endif
	default: {
	  fileType = FileIndices::THR;
	  break;
	}
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto pszFilePath = PWSTR {nullptr};
  hResult          = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
  if (FAILED(hResult)) {
	return false;
  }
  fileName.assign(pszFilePath);
  CoTaskMemFree(pszFilePath);
  return true;
}

void noMsg() {
  if (DestroyWindow(MsgWindow) != 0) {
	MsgWindow = nullptr;
  }
  displayText::destroyOKButton();
  DestroyWindow(DiscardButton);
  DestroyWindow(CancelButton);
  DestroyWindow(DeleteStitchesDialog);
  if (GeneralNumberInputBox != nullptr) {
	if (DestroyWindow(GeneralNumberInputBox) != 0) {
	  GeneralNumberInputBox = nullptr;
	}
  }
  if (Instance->stateMap.testAndReset(StateFlag::NUMIN) || FormMenuChoice != 0U || PreferenceIndex != 0U) {
	chknum();
  }
  Instance->stateMap.reset(StateFlag::MOVMSG);
  Instance->stateMap.reset(StateFlag::DELFRM);
  Instance->stateMap.reset(StateFlag::FILMSG);
  Instance->stateMap.reset(StateFlag::FORMIN);
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto nuBak() noexcept -> BOOL {
  BackgroundColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
  BackgroundColorStruct.hwndOwner      = ThrEdWindow;
  BackgroundColorStruct.lCustData      = 0;
  BackgroundColorStruct.lpCustColors   = CustomBackgroundColor.data();
  BackgroundColorStruct.lpfnHook       = nullptr;
  BackgroundColorStruct.lpTemplateName = nullptr;
  BackgroundColorStruct.rgbResult      = BackgroundColor;
  BackgroundColorStruct.lStructSize    = sizeof(BackgroundColorStruct);
  return ChooseColor(&BackgroundColorStruct);
}

void nuBrush(HBRUSH& brush, COLORREF const color) noexcept {
  DeleteObject(brush);
  brush = CreateSolidBrush(color);
}

auto nuCol(COLORREF const init) noexcept -> BOOL {
  ColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
  ColorStruct.hwndOwner      = ThrEdWindow;
  ColorStruct.lCustData      = 0;
  ColorStruct.lpCustColors   = CustomColor.data();
  ColorStruct.lpfnHook       = nullptr;
  ColorStruct.lpTemplateName = nullptr;
  ColorStruct.rgbResult      = init;
  ColorStruct.lStructSize    = sizeof(ColorStruct);
  return ChooseColor(&ColorStruct);
}

void nuFil(FileIndices const fileIndex) {
  auto& workingFileName = Instance->workingFileName;

  // Todo - check filename for validity before using it
  auto newFileName = workingFileName; // intentional copy
  if (!Instance->stateMap.testAndReset(StateFlag::REDOLD) &&
      !getNewFileName(newFileName, FileStyles::ALL_FILES, fileIndex)) {
	return;
  }
  workingFileName.assign(newFileName);
  defNam(newFileName);
  resetState();
  auto const fileExt = newFileName.extension().wstring();
  if (fileExt.empty()) {
	return;
  }
  if (auto const firstCharacter = tolower(fileExt[1]); firstCharacter == 't') {
	if (!readTHRFile(newFileName)) {
	  return;
	}
  }
  else {
	Instance->stateMap.set(StateFlag::NOTHRFIL);
	if (firstCharacter == 'p') {
	  if (tolower(fileExt[2]) == 'c') {
		if (!PCS::readPCSFile(newFileName)) {
		  return;
		}
	  }
#if PESACT
	  else {
		if (!PES::readPESFile(newFileName)) {
		  return;
		}
	  }
#endif
	}
	else {
	  if (!DST::readDSTFile(newFileName)) {
		return;
	  }
	}
  }
  if (bitmap::ismap()) {
	bitmap::assignUBFilename(ThrSingle->DefaultDirectory);
  }
  thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
  ZoomRect     = F_RECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
  UnzoomedRect = {.cx = std::lround(IniFile.hoopSizeX), .cy = std::lround(IniFile.hoopSizeY)};
  thred::movStch();
  thred::coltab();
  Instance->stateMap.reset(StateFlag::ZUMED);
  auto buffer       = std::array<wchar_t, 3> {};
  buffer[1]         = L'0';
  auto itUserPen    = ThrSingle->UserPen.begin();
  auto ucb          = UserColorBrush.begin();
  auto itThreadSize = ThreadSize.begin();
  auto tsw          = ThrSingle->ThreadSizeWin.begin();
  for (auto const& color : UserColor) {
	thred::nuPen(*itUserPen, 1, color);
	++itUserPen;
	nuBrush(*ucb, color);
	++ucb;
	buffer[0] = *itThreadSize++;
	SetWindowText(*tsw++, buffer.data());
  }
  for (auto const& iColor : ThrSingle->UserColorWin) {
	thred::redraw(iColor);
  }
  thred::redrawColorBar();
  if (!Instance->stitchBuffer.empty()) {
	Instance->stateMap.set(StateFlag::INIT);
  }
  thred::nuAct(0);
  Instance->stateMap.set(StateFlag::RESTCH);
  nunams();
  ritini();
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  if (!Instance->stitchBuffer.empty()) {
	thred::nuAct(Instance->stitchBuffer.front().attribute & COLMSK);
  }
  else {
	thred::nuAct(0);
  }
  menu::auxmen();
  thred::lenCalc();
  auto const fmtStr = displayText::format2(IDS_THRDBY, newFileName.wstring(), ThrSingle->DesignerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
  Instance->stateMap.set(StateFlag::INIT);
  Instance->stateMap.reset(StateFlag::TRSET);
  if (Instance->stateMap.test(StateFlag::NOTHRFIL)) {
	for (auto& stitch : Instance->stitchBuffer) {
	  stitch.attribute |= NOTFRM;
	}
  }
  repair::lodchk();
}

void nuRct() noexcept {
  GetClientRect(ThrEdWindow, &ThredWindowRect);
  GetWindowRect(ColorBar, &ColorBarRect);
  if (StitchWindowMemDC != nullptr) {
	DeleteDC(StitchWindowMemDC);
  }
  if (StitchWindowDC != nullptr) {
	ReleaseDC(MainStitchWin, StitchWindowDC);
	StitchWindowDC    = GetDCEx(MainStitchWin, nullptr, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
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
  if (StitchWindowMemDC != nullptr && StitchWindowBmp != nullptr) {
	SelectObject(StitchWindowMemDC, StitchWindowBmp);
  }
}

void nuStchSiz(uint32_t const iColor, int32_t const width) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  if (auto const tsp = wrap::next(ThreadSizePixels.begin(), iColor); width != *tsp) {
	thred::nuPen(ThrSingle->UserPen.operator[](iColor), width, UserColor.at(iColor));
	*tsp = width;
  }
}

auto nuang(float const originalAngle, float const xDelta, float const yDelta) noexcept -> float {
  auto const angle         = std::atan2(yDelta, xDelta);
  auto       relativeAngle = angle - originalAngle;
  if (fabs(relativeAngle) > PI_F) {
	if (relativeAngle > 0.0F) {
	  relativeAngle = PI_F2 - relativeAngle;
	}
	else {
	  relativeAngle += PI_F2;
	}
  }
  return relativeAngle;
}

void nunams() {
  auto const& workingFileName = Instance->workingFileName;
  Instance->auxName           = workingFileName;
  xt::duauxnam(Instance->auxName);
  auto& thrName = ThrSingle->ThrName;
  thrName       = workingFileName;
  thrName.replace_extension(L".thr");
  auto& previousNames = ThrSingle->PreviousNames;
  if (previousNames.front() == thrName) {
	return;
  }
  auto flag = true;
  for (auto spNames =
           std::ranges::subrange(std::next(previousNames.begin()), ThrSingle->PreviousNames.end());
       auto& previousName : spNames) {
	if (previousName != thrName) {
	  continue;
	}
	std::swap(ThrSingle->PreviousNames.front(), previousName);
	flag = false;
	break;
  }
  if (!flag) {
	menu::redfils(LRU_MENU_ID, ThrSingle->PreviousNames);
	return;
  }
  for (auto& previousName : ThrSingle->PreviousNames) {
	if (!previousName.empty()) {
	  continue;
	}
	previousName.assign(thrName);
	flag = false;
	break;
  }
  if (flag) {
	previousNames.insert(previousNames.begin(), thrName);
	previousNames.pop_back();
  }
  menu::redfils(LRU_MENU_ID, previousNames);
}

void nuselrct() {
  auto outline = std::vector<F_POINT> {};
  outline.resize(ThrSingle->FormControlPoints.size());
  thred::unsel();
  outline[PTL].x = outline[PBL].x = outline[PLM].x = outline[PTLE].x = StitchRangeRect.left;
  outline[PTL].y = outline[PTM].y = outline[PTR].y = outline[PTLE].y = StitchRangeRect.top;
  outline[PTR].x = outline[PRM].x = outline[PBR].x = StitchRangeRect.right;
  outline[PBR].y = outline[PBM].y = outline[PBL].y = StitchRangeRect.bottom;
  outline[PTM].x = outline[PBM].x = wrap::midl(StitchRangeRect.right, StitchRangeRect.left);
  outline[PRM].y = outline[PLM].y = wrap::midl(StitchRangeRect.top, StitchRangeRect.bottom);

  auto iPoint = outline.begin();
  for (auto& controlPoint : ThrSingle->FormControlPoints) {
	controlPoint = form::sfCor2px(*iPoint);
	++iPoint;
  }
}

void nuthum(wchar_t const character) {
  Instance->stateMap.set(StateFlag::RESTCH);
  auto& thumbnailSearchString  = ThrSingle->ThumbnailSearchString;
  thumbnailSearchString.back() = character;
  thumbnailSearchString.push_back(0);

  auto const txt = std::wstring(thumbnailSearchString.data());
  displayText::butxt(HBOXSEL, txt);
  ThumbnailIndex = 0;
  thred::nuthsel();
}

void ofstch(std::vector<F_POINT_ATTR>& buffer, uint32_t const iSource, char const offset, F_POINT const& knotStep, uint32_t knotAttribute) {
  buffer.emplace_back(Instance->stitchBuffer.operator[](iSource).x + (knotStep.x * wrap::toFloat(offset)),
                      Instance->stitchBuffer.operator[](iSource).y + (knotStep.y * wrap::toFloat(offset)),
                      knotAttribute);
}

// ReSharper disable CppParameterMayBeConst
#pragma warning(suppress : 26461) // The pointer argument can be marked as a pointer to const (con.3)
auto oldwnd(HWND window) noexcept -> bool {
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	if (ThrSingle->DefaultColorWin.operator[](iColor) == window ||
	    ThrSingle->UserColorWin.operator[](iColor) == window ||
	    ThrSingle->ThreadSizeWin.operator[](iColor) == window) {
	  return false;
	}
  }
  for (auto iButton = 0U; iButton < BTNCOUNT; ++iButton) {
	if (Instance->buttonWin.operator[](iButton) == window) {
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
// ReSharper restore CppParameterMayBeConst

void patdun() {
  Instance->stateMap.reset(StateFlag::RUNPAT);
  KillTimer(ThrEdWindow, 0);
  Instance->stateMap.set(StateFlag::WASPAT);
  thred::movStch();
  Instance->stateMap.set(StateFlag::RESTCH);
  displayText::tabmsg(IDS_END, false);
}

// ReSharper disable CppParameterMayBeConst
void prtred(HANDLE fileHandle, uint32_t const code) {
  CloseHandle(fileHandle);
  Instance->stateMap.reset(StateFlag::INIT);
  Instance->formList.clear();
  displayText::tabmsg(code, false);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}
// ReSharper restore CppParameterMayBeConst

auto pxchk(float pixelSize) -> uint16_t {
  constexpr auto MINLIMIT = 1.0F;
  constexpr auto MAXLIMIT = 20.0F;

  pixelSize = std::clamp(pixelSize, MINLIMIT, MAXLIMIT);
  return wrap::round<uint16_t>(pixelSize);
}

auto readTHRFile(std::filesystem::path const& newFileName) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto bytesRead   = DWORD {};
  auto thredHeader = THR_HEAD {};
  if (!wrap::readFile(fileHandle, &thredHeader, sizeof(thredHeader), &bytesRead, L"ReadFile for ThrEd Header in readTHRFile")) { // check is we even have a file
	return false;
  }
  if ((thredHeader.headerType & SIGMASK) != THREDSIG) { // does the file have the correct signature
	prtred(fileHandle, IDS_NOTHR);
	return false;
  }
  if (bytesRead != sizeof(thredHeader)) { // is there enough data in the file to read the header
	prtred(fileHandle, IDS_SHRTF);
	return false;
  }
  auto const version = (thredHeader.headerType & FTYPMASK) >> TBYTSHFT;
  auto const spIDN   = gsl::span {IniFile.designerName};
  ThrSingle->DesignerName.assign(utf::utf8ToUtf16(std::string(spIDN.data())));
  switch (version) { // handle the different versions of the file format
	case 0: {
	  if (thredHeader.hoopType == SMALHUP) {
		IniFile.hoopSizeX = SHUPX;
		IniFile.hoopSizeY = SHUPY;
		UnzoomedRect = SIZE {gsl::narrow_cast<int32_t>(SHUPX), gsl::narrow_cast<int32_t>(SHUPY)};
	  }
	  else {
		IniFile.hoopSizeX = LHUPX;
		IniFile.hoopSizeY = LHUPY;
		UnzoomedRect = SIZE {gsl::narrow_cast<int32_t>(LHUPX), gsl::narrow_cast<int32_t>(LHUPY)};
	  }
	  ritfnam(ThrSingle->DesignerName);
	  auto const spModifierName = gsl::span {ExtendedHeader.modifierName};
	  std::copy(spIDN.begin(), wrap::next(spIDN.begin(), strlen(spIDN.data()) + 1U), spModifierName.begin());
	  break;
	}
	case 1:
	case 2: {
	  if (!wrap::readFile(fileHandle, &ExtendedHeader, sizeof(ExtendedHeader), &bytesRead, L"ReadFile for ExtendedHeader in readTHRFile")) {
		return false;
	  }
	  if (bytesRead != sizeof(ExtendedHeader)) { // is there enough data in the file to read the extended header
		prtred(fileHandle, IDS_SHRTF);
		return false;
	  }
	  IniFile.hoopSizeX = ExtendedHeader.hoopSizeX;
	  IniFile.hoopSizeY = ExtendedHeader.hoopSizeY;

	  UnzoomedRect = {.cx = std::lround(ExtendedHeader.hoopSizeX), .cy = std::lround(ExtendedHeader.hoopSizeY)};
	  redfnam(ThrSingle->DesignerName);
	  break;
	}
	default: {
	  prtred(fileHandle, IDS_NOTVER);
	  return false;
	}
  }
  ZoomRect     = F_RECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
  UnzoomedRect = {.cx = std::lround(IniFile.hoopSizeX), .cy = std::lround(IniFile.hoopSizeY)};
  Instance->stitchBuffer.resize(thredHeader.stitchCount);
  if (thredHeader.stitchCount != 0U) { // read the stitch buffer if there are stitches
	auto const bytesToRead = thredHeader.stitchCount * wrap::sizeofType(Instance->stitchBuffer);
	if (!wrap::readFile(fileHandle, Instance->stitchBuffer.data(), bytesToRead, &bytesRead, L"ReadFile for stitchBuffer in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  prtred(fileHandle, IDS_PRT);
	  return false;
	}
  }
  Instance->stitchBuffer.shrink_to_fit();
  auto bytesToRead = bitmap::getBmpNameLength();
  if (!wrap::readFile(fileHandle, bitmap::getBmpNameData(), bytesToRead, &bytesRead, L"ReadFile for getBmpNameData in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the bitmap name, reset the bitmap and return
	bitmap::resetBmpFile(true);
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  bytesToRead = sizeof(BackgroundColor);
  if (!wrap::readFile(fileHandle, &BackgroundColor, bytesToRead, &bytesRead, L"ReadFile for BackgroundColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the background color, default it and return
	BackgroundColor = IniFile.backgroundColor;
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  bytesToRead     = sizeof(UserColor);
  if (!wrap::readFile(fileHandle, UserColor.data(), bytesToRead, &bytesRead, L"ReadFile for UserColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the user colors, default them and return
	UserColor = DEFAULT_COLORS;
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  bytesToRead = sizeof(CustomColor);
  if (!wrap::readFile(fileHandle, CustomColor.data(), bytesToRead, &bytesRead, L"ReadFile for CustomColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the custom colors, default them and return
	CustomColor = DEFAULT_COLORS;
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto msgBuffer = std::array<char, TSSIZE> {};
  if (!wrap::readFile(fileHandle, msgBuffer.data(), sizeof(msgBuffer), &bytesRead, L"ReadFile for msgBuffer in readTHRFile")) {
	return false;
  }
  if (bytesRead != TSSIZE) { // if we can't read the thread size buffer, bail
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto const threadSizebuf  = std::string(msgBuffer.data(), msgBuffer.size());
  auto       threadSizeBufW = utf::utf8ToUtf16(threadSizebuf);
  std::ranges::generate(ThreadSize, [tsBuffer = threadSizeBufW.begin()]() mutable noexcept -> wchar_t {
	return *tsBuffer++;
  });
  if (thredHeader.formCount == 0) {
	CloseHandle(fileHandle);
	return true;
  }
  Instance->stateMap.reset(StateFlag::BADFIL);
  auto& formList = Instance->formList;

  if (version < 2) {
	auto formListOriginal = std::vector<FRM_HEAD_O> {};
	formListOriginal.resize(thredHeader.formCount);
	bytesToRead = thredHeader.formCount * wrap::sizeofType(formListOriginal);
	if (!wrap::readFile(fileHandle, formListOriginal.data(), bytesToRead, &bytesRead, L"ReadFile for formListOriginal in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) { // if the header and available data don't match, resize the form list with the data we do have but set the bad file flag
	  wrap::narrow(thredHeader.formCount, bytesRead / wrap::sizeofType(formListOriginal));
	  formListOriginal.resize(thredHeader.formCount);
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	formList.reserve(formListOriginal.size());
	formList.insert(formList.end(), formListOriginal.begin(), formListOriginal.end());
  }
  else {
	auto inFormList = std::vector<FRM_HEAD_OUT> {};
	inFormList.resize(thredHeader.formCount);
	bytesToRead = thredHeader.formCount * wrap::sizeofType(inFormList);
	if (!wrap::readFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, L"ReadFile for inFormList in readTHRFile")) { // read the form list
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  wrap::narrow(thredHeader.formCount, bytesRead / wrap::sizeofType(inFormList));
	  inFormList.resize(thredHeader.formCount);
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	formList.reserve(inFormList.size());
	formList.insert(formList.end(), inFormList.begin(), inFormList.end());
  }
  formList.shrink_to_fit();
  if (thredHeader.vertexCount != 0U) { // read the form vertices
	Instance->formVertices.resize(thredHeader.vertexCount);
	bytesToRead = thredHeader.vertexCount * wrap::sizeofType(Instance->formVertices);
	if (!wrap::readFile(fileHandle, Instance->formVertices.data(), bytesToRead, &bytesRead, L"ReadFile for formVertices in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  Instance->formVertices.resize(bytesRead / wrap::sizeofType(Instance->formVertices));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
  }
  else {
	// We have forms but no vertices - blow up the read
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  Instance->formVertices.shrink_to_fit();
  if (thredHeader.dlineCount != 0U) { // read the satin guide list
	auto inGuideList = std::vector<SAT_CON_OUT>(thredHeader.dlineCount);
	bytesToRead      = thredHeader.dlineCount * wrap::sizeofType(inGuideList);
	if (!wrap::readFile(fileHandle, inGuideList.data(), bytesToRead, &bytesRead, L"ReadFile for inGuideList in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  inGuideList.resize(bytesRead / wrap::sizeofType(inGuideList));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	Instance->satinGuides.reserve(inGuideList.size());
	Instance->satinGuides.insert(Instance->satinGuides.end(), inGuideList.begin(), inGuideList.end());
  }
  Instance->satinGuides.shrink_to_fit();
  if (thredHeader.clipDataCount != 0U) { // read the clip points
	Instance->clipPoints.resize(thredHeader.clipDataCount);
	bytesToRead = thredHeader.clipDataCount * wrap::sizeofType(Instance->clipPoints);
	if (!wrap::readFile(fileHandle, Instance->clipPoints.data(), bytesToRead, &bytesRead, L"ReadFile for clipPoints in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  Instance->clipPoints.resize(bytesRead / wrap::sizeofType(Instance->clipPoints));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
  }
  Instance->clipPoints.shrink_to_fit();
  if (ExtendedHeader.texturePointCount != 0U) { // read the texture points
	Instance->texturePointsBuffer.resize(ExtendedHeader.texturePointCount);
	bytesToRead = ExtendedHeader.texturePointCount * wrap::sizeofType(Instance->texturePointsBuffer);
	if (!wrap::readFile(fileHandle, Instance->texturePointsBuffer.data(), bytesToRead, &bytesRead, L"ReadFile for texturePointsBuffer in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  Instance->texturePointsBuffer.resize(bytesRead / wrap::sizeofType(Instance->texturePointsBuffer));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
  }
  else {
	Instance->texturePointsBuffer.clear();
  }
  Instance->texturePointsBuffer.shrink_to_fit();
  if (Instance->stateMap.testAndReset(StateFlag::BADFIL)) {
	displayText::bfilmsg();
  }
  // now re-create all the pointers/indexes in the form data
  auto clipOffset   = 0U;
  auto vertexOffset = 0U;
  auto guideOffset  = 0U;
  for (auto& iForm : formList) {
	iForm.vertexIndex = vertexOffset;
	vertexOffset += iForm.vertexCount;
	if (iForm.type == SAT && iForm.satinGuideCount != 0U) {
	  iForm.satinGuideIndex = guideOffset;
	  guideOffset += iForm.satinGuideCount;
	}
	// ToDo - do we still need to do this in v3? (we can store the offset safely in v3
	// where we could not store the pointer in v2)
	if (iForm.isClip()) {
	  iForm.clipIndex = clipOffset;
	  clipOffset += iForm.clipCount;
	}
	if (iForm.isEdgeClipX()) {
	  iForm.borderClipData = clipOffset;
	  clipOffset += iForm.clipEntries;
	}
  }
  xt::setfchk();
  CloseHandle(fileHandle);
  return true;
}

void rebak() {
  destroyBV();
  auto const& thrName = ThrSingle->ThrName;

  auto newFileName    = thrName;     // intentional copy
  auto safetyFileName = newFileName; // initialise from local variable
  auto ext            = newFileName.extension().wstring();
  ext.back()          = FileVersionIndex + L's';
  newFileName.replace_extension(ext);
  ext.back() = 'x';
  safetyFileName.replace_extension(ext);
  fs::rename(thrName, safetyFileName);
  if (exists(newFileName)) {
	fs::rename(newFileName, thrName);
  }
  fs::rename(safetyFileName, newFileName);
  Instance->workingFileName = thrName;
  Instance->stateMap.set(StateFlag::REDOLD);
  nuFil(FileIndices::THR);
  fs::remove(safetyFileName);
}

void redfnam(std::wstring& designerName) {
  constexpr auto NNN           = uint8_t {111U}; // string substitute character
  auto           tmpName       = std::array<uint8_t, NameOrder.size()> {};
  auto           designer      = std::string {};
  auto           iNameOrder    = NameOrder.begin();
  auto const     spCreatorName = gsl::span {ExtendedHeader.creatorName};
  for (auto& iTmpName : tmpName) {
	if (auto const& index = *iNameOrder; index < spCreatorName.size()) {
	  iTmpName = gsl::narrow_cast<uint8_t>(spCreatorName[index]);
	}
	else {
	  iTmpName = NNN;
	}
	++iNameOrder;
  }
  designer.reserve(tmpName.size());
  for (auto const& character : tmpName) {
	if (auto iND = wrap::next(NameDecoder.begin(), character); *iND != 0U) {
	  designer.push_back(gsl::narrow<char>(*iND));
	}
	else {
	  break;
	}
  }
  auto const decoded = utf::utf8ToUtf16(designer);
  designerName.assign(decoded);
}

void redini() {
  duhom();
  auto& iniFileName = ThrSingle->IniFileName;
  iniFileName       = ThrSingle->HomeDirectory;
  iniFileName /= L"thred.ini";
  // NOLINTNEXTLINE(readability-qualified-auto)
  if (auto const iniFileHandle =
          CreateFile(iniFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
      iniFileHandle == INVALID_HANDLE_VALUE) {
	setPrefs();
  }
  else {
	auto bytesRead = DWORD {};
	if (!wrap::readFile(iniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, L"ReadFile for IniFile in redini")) {
	  return;
	}
	CloseHandle(iniFileHandle);
	if (bytesRead < sizeof(IniFile)) {
	  auto newFileName = iniFileName; // intentional copy
	  newFileName.replace_filename("thred-ini.bak");
	  fs::rename(iniFileName, newFileName);
	  setPrefs();
	}
	else {
	  auto const directory = utf::utf8ToUtf16(std::string(IniFile.defaultDirectory.data()));
	  ThrSingle->DefaultDirectory.assign(directory);
	  {
		auto previousName = ThrSingle->PreviousNames.begin();
		for (auto const& prevName : IniFile.prevNames) {
		  if (strlen(prevName.data()) != 0U) {
			previousName->assign(utf::utf8ToUtf16(std::string(prevName.data())));
		  }
		  else {
			previousName->clear();
		  }
		  ++previousName;
		}
	  }
	  ThrSingle->DesignerName.assign(utf::utf8ToUtf16(std::string(IniFile.designerName.data())));
	  loadColors();
	  bitmap::setBmpBackColor();
	  BackgroundColor = IniFile.backgroundColor;
	  bitmap::setBitmapColor(IniFile.bitmapColor);
	  MinStitchLength             = IniFile.minStitchLength;
	  IniFile.showStitchThreshold = std::max(IniFile.showStitchThreshold, 0.0F);
	  constexpr auto SSTCLAMP     = 9.0F; // clamp the show stitch threshold to this value

	  IniFile.showStitchThreshold = std::min(IniFile.showStitchThreshold, SSTCLAMP);
	  ShowStitchThreshold         = IniFile.showStitchThreshold;
	  if (IniFile.threadSize30 != 0.0F) {
		ThreadSize30 = IniFile.threadSize30;
	  }
	  if (IniFile.threadSize40 != 0.0F) {
		ThreadSize40 = IniFile.threadSize40;
	  }
	  if (IniFile.threadSize60 != 0.0F) {
		ThreadSize60 = IniFile.threadSize60;
	  }
	  if (IniFile.userStitchLength != 0.0F) {
		UserStitchLength = IniFile.userStitchLength;
	  }
	  if (IniFile.maxStitchLength == 0.0F) {
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
		auto const tmp        = ENUM_MAP<UserFlag>(IniFile.userFlagMap);
		Instance->userFlagMap = tmp;
	  }
	  if (IniFile.borderWidth != 0.0F) {
		BorderWidth = IniFile.borderWidth;
	  }
	  if (IniFile.appliqueColor != 0U) {
		AppliqueColor = IniFile.appliqueColor & COLMSK;
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
		IniFile.gridSize = 12.0F; // set this to a sensible value NOLINT(readability-magic-numbers)
	  }
	  IniFile.setWav();
	  IniFile.setFeather();
	  IniFile.setDaisy();
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
	  UnzoomedRect = {.cx = std::lround(IniFile.hoopSizeX), .cy = std::lround(IniFile.hoopSizeY)};
	  PicotSpacing = IniFile.picotSpace;
	}
  }
  if (IniFile.gridColor == 0U) {
	IniFile.gridColor = menu::getGridDefColor();
  }
  if (IniFile.fillAngle == 0.0F) {
	IniFile.fillAngle = PI_F / 6.0F; // set this to a sensible number NOLINT(readability-magic-numbers)
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const deviceContext = GetDC(nullptr);
  ScreenSizePixels = {.cx = GetDeviceCaps(deviceContext, HORZRES), .cy = GetDeviceCaps(deviceContext, VERTRES)};
  IniFile.initialWindowCoords.left  = std::max(IniFile.initialWindowCoords.left, 0L);
  IniFile.initialWindowCoords.top   = std::max(IniFile.initialWindowCoords.top, 0L);
  auto const adjustedWidth          = ScreenSizePixels.cx - 30;
  IniFile.initialWindowCoords.right = std::min(IniFile.initialWindowCoords.right, adjustedWidth);
  IniFile.initialWindowCoords.bottom = std::min(IniFile.initialWindowCoords.bottom, ScreenSizePixels.cy);
}

void relin() {
  if (ClosestPointIndex != 0U) {
	MoveLine0[0] = thred::stch2px1(ClosestPointIndex - 1);
  }
  MoveLine1[1] = thred::stch2px1(ClosestPointIndex + 1U);
  Instance->stateMap.reset(StateFlag::WASLIN);
  dulin(MoveLine0, MoveLine1);
}

void resetState() {
  fnamtabs();
  trace::untrace();
  if (!Instance->formList.empty()) {
	form::delfrms();
  }
  unthum();
  thred::unbsho();
  bitmap::resetBmpFile(true);
  Instance->texturePointsBuffer.clear();
  menu::disableRedo();
  thred::unbsho();
  form::frmon();
  Instance->selectedFormList.clear();
  Instance->selectedFormList.shrink_to_fit();
  if (Instance->stateMap.test(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	PreferenceIndex = 0;
  }
  bitmap::resetBmpFile(true);
  Instance->searchLine.clear();
  Instance->searchLine.shrink_to_fit();
  rstdu();
  thred::unmsg();
  ZoomFactor = 1;
  thred::resetNearest();
  if (Instance->stateMap.testAndReset(StateFlag::WASPAT)) {
	DestroyWindow(SpeedScrollBar);
  }
  auto textureHistoryFlag = false;
  if (Instance->stateMap.test(StateFlag::WASTXBAK)) {
	textureHistoryFlag = true;
  }
  Instance->stateMap.reset();
  if (textureHistoryFlag) {
	Instance->stateMap.set(StateFlag::WASTXBAK);
  }
}

void respac(FRM_HEAD& form) noexcept {
  if (!form.isClip()) {
	return;
  }
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
}

void ritbak(fs::path const& fileName, DRAWITEMSTRUCT const& drawItem) {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto thrEdFile = CreateFile(fileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (thrEdFile == INVALID_HANDLE_VALUE) {
	return;
  }
  auto bytesRead    = DWORD {};
  auto stitchHeader = THR_HEAD {};
  if (!wrap::readFile(thrEdFile, &stitchHeader, sizeof(stitchHeader), &bytesRead, L"ReadFile for stitchHeader in ritbak")) {
	return;
  }
  if (bytesRead != sizeof(stitchHeader)) {
	CloseHandle(thrEdFile);
	return;
  }
  auto       stitchSourceSize = F_POINT {1.0F, 1.0F};
  auto const fileTypeVersion  = (stitchHeader.headerType & FTYPMASK) >> TBYTSHFT;
  if ((stitchHeader.headerType & SIGMASK) == THREDSIG) {
	switch (fileTypeVersion) {
	  case 0: {
		if (stitchHeader.hoopType == SMALHUP) {
		  IniFile.hoopSizeX = SHUPX;
		  IniFile.hoopSizeY = SHUPY;
		  stitchSourceSize  = F_POINT {SHUPX, SHUPY};
		}
		else {
		  IniFile.hoopSizeX = LHUPX;
		  IniFile.hoopSizeY = LHUPY;
		  stitchSourceSize  = F_POINT {LHUPX, LHUPY};
		}
		break;
	  }
	  case 1:
	  case 2: {
		auto extendedHeader = THR_HEAD_EX {};
		if (!wrap::readFile(thrEdFile, &extendedHeader, sizeof(extendedHeader), &bytesRead, L"ReadFile for extendedHeader in ritbak")) {
		  return;
		}
		if (bytesRead != sizeof(extendedHeader)) {
		  return;
		}
		stitchSourceSize = F_POINT {extendedHeader.hoopSizeX, extendedHeader.hoopSizeY};
		break;
	  }
	  default: {
		return;
	  }
	}
  }
  auto const drawingDestinationSize = SIZE {(drawItem.rcItem.right - drawItem.rcItem.left),
                                            (drawItem.rcItem.bottom - drawItem.rcItem.top)};
  auto const xRatio                 = wrap::toFloat(drawingDestinationSize.cx) / stitchSourceSize.x;
  auto const yRatio                 = wrap::toFloat(drawingDestinationSize.cy) / stitchSourceSize.y;
  auto const ratio                  = xRatio < yRatio ? xRatio : yRatio;
  if (stitchHeader.stitchCount != 0U) {
	auto stitchesToDraw = std::vector<F_POINT_ATTR> {};
	stitchesToDraw.resize(stitchHeader.stitchCount);
	auto lines = std::vector<POINT> {};
	lines.resize(stitchHeader.stitchCount);
	auto const bytesToRead = stitchHeader.stitchCount * wrap::sizeofType(Instance->stitchBuffer);
	if (!wrap::readFile(thrEdFile, stitchesToDraw.data(), bytesToRead, &bytesRead, L"ReadFile for stitchesToDraw in ritbak")) {
	  return;
	}
	if (bytesToRead != bytesRead) {
	  CloseHandle(thrEdFile);
	  return;
	}
	// skip over BMP name
	SetFilePointer(thrEdFile, gsl::narrow<LONG>(bitmap::getBmpNameLength()), nullptr, FILE_CURRENT);
	auto brushColor = COLORREF {};
	if (!wrap::readFile(thrEdFile, &brushColor, sizeof(brushColor), &bytesRead, L"ReadFile for brushColor in ritbak")) {
	  return;
	}
	auto colors = std::vector<COLORREF> {};
	colors.resize(COLORCNT);
	if (!wrap::readFile(thrEdFile, colors.data(), wrap::sizeofVector(colors), &bytesRead, L"ReadFile for colors in ritbak")) {
	  return;
	}
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto brush = CreateSolidBrush(brushColor);
	SelectObject(drawItem.hDC, brush);
	FillRect(drawItem.hDC, &drawItem.rcItem, brush);
	auto iColor = stitchesToDraw[0].attribute & COLMSK;
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto pen   = wrap::createPen(PS_SOLID, PENNWID, colors[iColor]);
	auto iLine = 0U;
	for (auto iStitch = 0U; iStitch < stitchHeader.stitchCount; ++iStitch) {
	  if ((stitchesToDraw[iStitch].attribute & COLMSK) != iColor) {
		thred::nuPen(pen, 1, colors[iColor]);
		SelectObject(drawItem.hDC, pen);
		wrap::polyline(drawItem.hDC, lines.data(), iLine);
		iLine  = 0;
		iColor = stitchesToDraw[iStitch].attribute & COLMSK;
		continue;
	  }
	  lines[iLine++] = {.x = std::lround(stitchesToDraw[iStitch].x * ratio),
	                    .y = std::lround(wrap::toFloat(drawingDestinationSize.cy) -
	                                     (stitchesToDraw[iStitch].y * ratio))};
	}
	if (iLine != 0U) {
	  thred::nuPen(pen, 1, colors[iColor]);
	  SelectObject(drawItem.hDC, pen);
	  wrap::polyline(drawItem.hDC, lines.data(), iLine);
	}
	DeleteObject(brush);
	DeleteObject(pen);
  }
  else {
	// no stitches in file so skip over BMP name , background color & user colors
	constexpr auto SKIP1 = LONG {16 + sizeof(COLORREF) + (sizeof(COLORREF) * COLORCNT)};
	SetFilePointer(thrEdFile, SKIP1, nullptr, FILE_CURRENT);
  }
  if (stitchHeader.formCount == 0U) {
	CloseHandle(thrEdFile);
	return;
  }
  // skip over custom colors (64 bytes)  and thread sizes (16 bytes) to forms
  constexpr auto SKIP2 = LONG {(sizeof(COLORREF) * COLORCNT) + 16};
  SetFilePointer(thrEdFile, SKIP2, nullptr, FILE_CURRENT);
  auto formList = std::vector<FRM_HEAD> {};
  formList.resize(stitchHeader.formCount);
  auto vertexList = std::vector<F_POINT> {};
  vertexList.resize(stitchHeader.vertexCount);
  if (fileTypeVersion < 2) {
	auto formListOriginal = std::vector<FRM_HEAD_O> {};
	formListOriginal.resize(stitchHeader.formCount);
	auto const bytesToRead = stitchHeader.formCount * wrap::sizeofType(formListOriginal);
	if (!wrap::readFile(thrEdFile, formListOriginal.data(), bytesToRead, &bytesRead, L"ReadFile for formListOriginal in ritbak")) {
	  return;
	}
	if (bytesRead != bytesToRead) {
	  CloseHandle(thrEdFile);
	  return;
	}
	std::ranges::copy(formListOriginal, formList.begin());
  }
  else {
	auto inFormList = std::vector<FRM_HEAD_OUT> {};
	inFormList.resize(stitchHeader.formCount);
	auto const bytesToRead = stitchHeader.formCount * wrap::sizeofType(inFormList);
	if (!wrap::readFile(thrEdFile, inFormList.data(), bytesToRead, &bytesRead, L"ReadFile for inFormList in ritbak")) {
	  return;
	}
	if (bytesRead != bytesToRead) {
	  CloseHandle(thrEdFile);
	  return;
	}
	std::ranges::copy(inFormList, formList.begin());
  }
  auto const bytesToRead = stitchHeader.vertexCount * wrap::sizeofType(vertexList);
  if (!wrap::readFile(thrEdFile, vertexList.data(), bytesToRead, &bytesRead, L"ReadFile for vertexList in ritbak")) {
	return;
  }
  if (bytesRead != bytesToRead) {
	CloseHandle(thrEdFile);
	return;
  }
  auto lines    = std::vector<POINT> {};
  auto maxLines = 0U;
  for (auto iForm = 0U; iForm < stitchHeader.formCount; ++iForm) {
	maxLines = std::max(formList[iForm].vertexCount, maxLines);
  }
  lines.resize(wrap::toSize(maxLines) + 1U);
  auto iVertex = 0U;
  for (auto iForm = 0U; iForm < stitchHeader.formCount; ++iForm) {
	auto const iLine = iVertex;
	for (auto iVertexInForm = 0U;
	     iVertexInForm < formList[iForm].vertexCount && iVertex < stitchHeader.vertexCount;
	     ++iVertexInForm) {
	  lines[iVertexInForm] = {.x = std::lround(vertexList[iVertex].x * ratio),
	                          .y = std::lround(wrap::toFloat(drawingDestinationSize.cy) -
	                                           (vertexList[iVertex++].y * ratio))};
	}
	lines[formList[iForm].vertexCount] = {
	    .x = std::lround(vertexList[iLine].x * ratio),
	    .y = std::lround(wrap::toFloat(drawingDestinationSize.cy) - (vertexList[iLine].y * ratio))};
	SelectObject(drawItem.hDC, FormPen);
	SetROP2(drawItem.hDC, R2_XORPEN);
	if (formList[iForm].type == FRMLINE) {
	  wrap::polyline(drawItem.hDC, lines.data(), formList[iForm].vertexCount);
	}
	else {
	  wrap::polyline(drawItem.hDC, lines.data(), formList[iForm].vertexCount + 1U);
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
  CloseHandle(thrEdFile);
}

void ritcor(F_POINT_ATTR const& pointAttribute) {
  auto const point = F_POINT {pointAttribute.x, pointAttribute.y};
  thred::ritfcor(point);
}

void ritfnam(std::wstring const& designerName) {
  constexpr auto NAMELEN  = NameOrder.size();
  auto const     designer = utf::utf16ToUtf8(designerName);
  auto           tmpName  = std::array<uint8_t, NameOrder.size()> {};
  if (NameOrder[0] > NameOrder.size()) {
	fnamtabs();
  }
  PseudoRandomValue = rsed();
  auto iName        = 0U;
  std::ranges::generate(tmpName, []() noexcept -> uint8_t { return form::psg() & BYTMASK; });
  // encode the designer name
  for (auto& iTmpName : tmpName) {
	// encode the name
	if (designer[iName++] != 0) {
	  auto iNE = wrap::next(NameEncoder.begin(), designer[iName]);
	  iTmpName = *iNE;
	  continue;
	}
	auto iND = wrap::next(NameDecoder.begin(), iTmpName);
	// salt the name
	while (*iND != 0U) {
	  iTmpName = form::psg() & BYTMASK;
	  iND      = wrap::next(NameDecoder.begin(), iTmpName);
	}
	break;
  }
  if (iName == NAMELEN) {
	auto const spNameDecoder = gsl::span {NameDecoder};
	auto&      index         = tmpName.back();
	while (spNameDecoder[wrap::toSize(index)] != 0U) {
	  index = gsl::narrow_cast<uint8_t>(form::psg() & BYTMASK);
	}
  }
  auto iTmpName = tmpName.begin();
  // write the encoded name
  auto const spCreatorName = gsl::span {ExtendedHeader.creatorName};
  for (auto const& iNameOrder : NameOrder) {
	if (iNameOrder < NAMELEN) {
	  spCreatorName[iNameOrder] = gsl::narrow_cast<char>(*iTmpName++);
	}
  }
}

void ritini() {
  auto const     directory = utf::utf16ToUtf8(ThrSingle->DefaultDirectory.wstring());
  constexpr char FILLCHAR  = '\0';
  std::ranges::fill(IniFile.defaultDirectory, FILLCHAR);
  auto const spIDD = gsl::span {IniFile.defaultDirectory};
  std::ranges::copy(directory, spIDD.begin());
  auto previousName = ThrSingle->PreviousNames.begin();
  for (auto& prevName : IniFile.prevNames) {
	std::ranges::fill(prevName, FILLCHAR);
	if (!previousName->empty()) {
	  auto       previous = utf::utf16ToUtf8(*previousName);
	  auto const spName   = gsl::span {prevName};
	  std::ranges::copy(previous, spName.begin());
	}
	++previousName;
  }
  auto const designer = utf::utf16ToUtf8(ThrSingle->DesignerName);
  std::ranges::fill(IniFile.designerName, FILLCHAR);
  auto const spIDN = gsl::span {IniFile.designerName};
  std::ranges::copy(designer, spIDN.begin());
  auto const spISC = gsl::span {IniFile.stitchColors};
  std::ranges::copy(UserColor, spISC.begin());
  auto const spIBPC = gsl::span {IniFile.backgroundPreferredColors};
  std::ranges::copy(CustomBackgroundColor, spIBPC.begin());
  auto const spISPC = gsl::span {IniFile.stitchPreferredColors};
  std::ranges::copy(CustomColor, spISPC.begin());
  std::ranges::generate(IniFile.bitmapBackgroundColors, [bcIndex = 0U]() mutable noexcept -> COLORREF {
	return bitmap::getBmpBackColor(bcIndex++);
  });
  IniFile.backgroundColor = BackgroundColor;
  IniFile.bitmapColor     = bitmap::getBmpColor();
  IniFile.minStitchLength = MinStitchLength;

  constexpr auto SSMAX = 9.0F; // Show stitch threshold maximum
  constexpr auto SSMIN = 0.0F; // Show stitch threshold minimum

  ShowStitchThreshold            = std::clamp(ShowStitchThreshold, SSMIN, SSMAX);
  IniFile.showStitchThreshold    = ShowStitchThreshold;
  IniFile.threadSize30           = ThreadSize30;
  IniFile.threadSize40           = ThreadSize40;
  IniFile.threadSize60           = ThreadSize60;
  IniFile.userStitchLength       = UserStitchLength;
  IniFile.smallStitchLength      = SmallStitchLength;
  IniFile.stitchBoxesThreshold   = StitchBoxesThreshold;
  IniFile.stitchSpace            = LineSpacing;
  IniFile.userFlagMap            = Instance->userFlagMap.to_ulong();
  IniFile.borderWidth            = BorderWidth;
  IniFile.appliqueColor          = AppliqueColor;
  IniFile.snapLength             = SnapLength;
  IniFile.starRatio              = StarRatio;
  IniFile.spiralWrap             = SpiralWrap;
  IniFile.buttonholeCornerLength = ButtonholeCornerLength;
  IniFile.picotSpace             = PicotSpacing;
  if (!Instance->userFlagMap.test(UserFlag::SAVMAX)) {
	auto windowRect = RECT {};
	GetWindowRect(ThrEdWindow, &windowRect);
	IniFile.initialWindowCoords.left   = windowRect.left;
	IniFile.initialWindowCoords.right  = windowRect.right;
	IniFile.initialWindowCoords.bottom = windowRect.bottom;
	IniFile.initialWindowCoords.top    = windowRect.top;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const iniFileHandle = CreateFile(
      ThrSingle->IniFileName.wstring().c_str(), GENERIC_WRITE | GENERIC_READ, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (iniFileHandle != INVALID_HANDLE_VALUE) {
	auto bytesRead = DWORD {};
	WriteFile(iniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, nullptr);
	// ToDo - Check that file is written completely
  }
  CloseHandle(iniFileHandle);
}

void ritlayr() {
  auto layer = BIGUINT;

  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	if (!Instance->stitchBuffer.empty()) {
	  layer = (Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & LAYMSK) >> LAYSHFT;
	}
  }
  else {
	if (Instance->stateMap.test(StateFlag::FORMSEL) || Instance->stateMap.test(StateFlag::FRMPSEL)) {
	  layer = gsl::narrow_cast<decltype(layer)>(
	              Instance->formList.operator[](ClosestFormToCursor).attribute & FRMLMSK) >>
	          1U;
	}
  }
  if ((layer & FRMMMSK) != 0U) {
	auto const blank = std::wstring {};
	displayText::butxt(HLAYR, blank);
  }
  else {
	auto const txt = displayText::format(IDS_LAYR, layer);
	displayText::butxt(HLAYR, txt);
  }
}

void ritloc() {
  auto lockFilePath = fs::path {};
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto ppszPath = PWSTR {nullptr}; // variable to receive the path memory block pointer.
  if (auto const hResult = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, nullptr, &ppszPath);
      FAILED(hResult)) {
	CoTaskMemFree(ppszPath); // free up the path memory block
	return;
  }
  lockFilePath.assign(ppszPath); // make a local copy of the path
  CoTaskMemFree(ppszPath);       // free up the path memory block
  lockFilePath /= L"ThrEd";
  create_directory(lockFilePath);
  lockFilePath /= L"thredloc.txt";
  // NOLINTNEXTLINE(readability-qualified-auto)
  if (auto const lockFile = CreateFile(lockFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
      lockFile != INVALID_HANDLE_VALUE) {
	auto       bytesWritten = DWORD {};
	auto const value        = utf::utf16ToUtf8(ThrSingle->HomeDirectory);
	wrap::writeFile(lockFile, value.data(), wrap::toUnsigned(value.size()) + 1U, &bytesWritten, nullptr);
	CloseHandle(lockFile);
  }
}

// ReSharper disable CppParameterMayBeConst
void ritlock(std::vector<WIN32_FIND_DATA> const& fileInfo, HWND hwndlg) noexcept {
  SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_RESETCONTENT, 0, 0);
  SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_RESETCONTENT, 0, 0);
  for (auto const& iFile : fileInfo) {
	if ((iFile.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0U) {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
	  SendMessage(GetDlgItem(hwndlg, IDC_LOCKED),
	              LB_ADDSTRING,
	              0,
	              reinterpret_cast<LPARAM>(iFile.cFileName)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	}
	else {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
	  SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED),
	              LB_ADDSTRING,
	              0,
	              reinterpret_cast<LPARAM>(iFile.cFileName)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	}
  }
}
// ReSharper restore CppParameterMayBeConst

void rotang(F_POINT const unrotatedPoint, POINT& rotatedPoint, float const rotationAngle, F_POINT const& rotationCenter) {
  auto const point = thred::rotangf(unrotatedPoint, rotationAngle, rotationCenter);
  thred::sCor2px(point, rotatedPoint);
}

void rotfns(float const rotationAngle) {
  thred::savdo();
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
}

void rotpix(POINT const& unrotatedPoint, POINT& rotatedPoint, POINT const& rotationCenterPixels) noexcept {
  // won't handle vertical lines
  auto const deltaX           = wrap::toFloat(unrotatedPoint.x - rotationCenterPixels.x);
  auto const deltaY           = wrap::toFloat(unrotatedPoint.y - rotationCenterPixels.y);
  auto const distanceToCenter = std::hypotf(deltaX, deltaY);
  auto const newAngle         = std::atan2f(deltaY, deltaX) - RotateAngle;
  rotatedPoint                = {
                     .x = std::lround(wrap::toFloat(rotationCenterPixels.x) + (distanceToCenter * cos(newAngle))),
                     .y = std::lround(wrap::toFloat(rotationCenterPixels.y) + (distanceToCenter * sin(newAngle)))};
}

void rotstch(F_POINT_ATTR& stitch, float const rotationAngle, F_POINT const& rotationCenter) noexcept {
  auto const point = thred::rotang1(stitch, rotationAngle, rotationCenter);

  stitch = point;
}

auto rsed() noexcept -> uint32_t {
  auto time = SYSTEMTIME {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
  GetLocalTime(&time);
  return (gsl::narrow_cast<uint32_t>(time.wSecond) << WRDSHFT) | time.wMilliseconds;
}

void rshft(POINT const& shiftPoint) {
  auto const shiftX = wrap::toFloat(shiftPoint.x);
  auto const shiftY = wrap::toFloat(shiftPoint.y);
  ZoomRect.top -= shiftY;
  ZoomRect.left -= shiftX;
  ZoomRect.right -= shiftX;
  ZoomRect.bottom -= shiftY;
  zRctAdj();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void rstdu() {
  backup::deldu();
  menu::disableRedo();
  menu::disableUndo();
}

void rthumnam(uint32_t const iThumbnail) {
  switch (iThumbnail) {
	case 0: {
	  barnam(Instance->buttonWin.operator[](HNUM), iThumbnail);
	  break;
	}
	case 1: {
	  barnam(Instance->buttonWin.operator[](HTOT), iThumbnail);
	  break;
	}
	case 2: {
	  barnam(Instance->buttonWin.operator[](HMINLEN), iThumbnail);
	  break;
	}
	case 3: {
	  barnam(Instance->buttonWin.operator[](HMAXLEN), iThumbnail);
	  break;
	}
	default: {
	  outDebugString(L"default hit in rthumbnam: iThumbnail [{}]\n", iThumbnail);
	  break;
	}
  }
}

void sachk() {
  for (auto iForm = 0U; iForm < wrap::toUnsigned(Instance->formList.size()); ++iForm) {
	if (auto const& form = Instance->formList.operator[](iForm); form.type == SAT && form.satinGuideCount != 0U) {
	  auto itGuide = wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		if (itGuide->start > form.vertexCount || itGuide->finish > form.vertexCount) {
		  satin::delsac(iForm);
		}
		++itGuide;
	  }
	}
  }
}

void sav() {
  auto& auxName = Instance->auxName;
  auxName       = Instance->workingFileName;
  xt::duauxnam(auxName);
  if (chkattr(auxName)) {
	return;
  }
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  chk1col();
  thred::coltab();
  auto saveStitches = std::vector<F_POINT_ATTR> {};
  saveStitches.resize(Instance->stitchBuffer.size());
  if (Instance->userFlagMap.test(UserFlag::ROTAUX)) {
	auto iDest = saveStitches.begin();
	for (auto const& stitch : Instance->stitchBuffer) {
	  *iDest++ = F_POINT_ATTR {stitch.y, stitch.x, stitch.attribute};
	}
  }
  else {
	std::ranges::copy(Instance->stitchBuffer, saveStitches.begin());
  }
  // ReSharper disable once CppInitializedValueIsAlwaysRewritten
  auto flag = true;
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  flag = DST::saveDST(auxName, saveStitches);
	  break;
	}
#if PESACT
	case AUXPES: {
	  flag = PES::savePES(auxName, saveStitches);
	  break;
	}
#endif
	default: {
	  flag = PCS::savePCS(auxName, saveStitches);
	}
  }
  if (flag) {
	defNam(Instance->workingFileName);
	if (Instance->userFlagMap.test(UserFlag::ROTAUX)) {
	  displayText::filnopn(IDS_FILROT, auxName);
	}
  }
}

auto savcmp() noexcept -> bool {
#ifdef _DEBUG
  return true;
#else
  return !Instance->stateMap.test(StateFlag::CMPDO);
#endif
}

auto sdCor2px(F_POINT_ATTR const& stitchPoint) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) +
                                    ((ZoomRect.bottom - stitchPoint.y) * ZoomRatio.y))};
}

void segentr(float rotationAngle) {
  if (rotationAngle == 0.0F) {
	rotationAngle = 1 * DEGRADF;
  }
  displayText::showMessage(IDS_ENTROT, PI_F2 / rotationAngle);
  Instance->stateMap.set(StateFlag::NUMIN);
  thred::numWnd();
}

// ReSharper disable CppParameterMayBeConst
void selin(uint32_t start, uint32_t end, HDC hDC) {
  SelectObject(hDC, GroupSelectPen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (!Instance->searchLine.empty()) {
	wrap::polyline(hDC, Instance->searchLine.data(), wrap::toUnsigned(Instance->searchLine.size()));
  }
  if (start > end) {
	std::swap(start, end);
  }
  Instance->searchLine.clear();
  if (!Instance->stitchBuffer.empty()) {
	auto stitch = wrap::next(Instance->stitchBuffer.begin(), start);
	for (auto iStitch = start; iStitch <= end; ++iStitch) {
	  Instance->searchLine.push_back(
	      POINT {wrap::ceil<int32_t>((stitch->x - ZoomRect.left) * ZoomRatio.x),
	             wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                 ((stitch->y - ZoomRect.bottom) * ZoomRatio.y))});
	  ++stitch;
	}
	wrap::polyline(hDC, Instance->searchLine.data(), wrap::toUnsigned(Instance->searchLine.size()));
  }
  SetROP2(hDC, R2_COPYPEN);
}
// ReSharper restore CppParameterMayBeConst

auto setRmap(boost::dynamic_bitset<>& stitchMap, F_POINT_ATTR const& stitchPoint, F_POINT const& cellSize)
    -> bool {
  auto const bitPoint = gsl::narrow<uint32_t>(floor((stitchPoint.x - ZoomRect.left) / cellSize.x) *
                                              floor((stitchPoint.y - ZoomRect.bottom) / cellSize.y));
  return !stitchMap.test_set(bitPoint);
}

void setScrollVisibility() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) { // if we are zoomed
	// set up the scroll bars
	auto scrollInfo   = SCROLLINFO {}; // scroll bar i/o structure
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.fMask  = SIF_ALL;
	scrollInfo.nMax   = UnzoomedRect.cy;
	scrollInfo.nMin   = 0;
	scrollInfo.nPage  = wrap::round<UINT>(ZoomRect.top - ZoomRect.bottom);
	scrollInfo.nPos =
	    wrap::round<decltype(scrollInfo.nPos)>(wrap::toFloat(UnzoomedRect.cy) - ZoomRect.top);
	SetScrollInfo(VerticalScrollBar, SB_CTL, &scrollInfo, TRUE);
	scrollInfo.nMax  = UnzoomedRect.cx;
	scrollInfo.nPage = wrap::round<UINT>(ZoomRect.right - ZoomRect.left);
	scrollInfo.nPos  = wrap::round<decltype(scrollInfo.nPos)>(ZoomRect.left);
	SetScrollInfo(HorizontalScrollBar, SB_CTL, &scrollInfo, TRUE);
	// and show the scroll bars
	thred::showScrollBars(true);
  }
  else {
	thred::showScrollBars(false);
  }
}

void setSideWinVal(int const index) {
  SetWindowText(Instance->valueWindow.operator[](wrap::toSize(index)),
                ThrSingle->SideWindowEntryBuffer.data());
}

void setbak(int32_t const penWidth) noexcept {
  if (BackgroundPenWidth != penWidth) {
	BackgroundPenWidth = penWidth;
	thred::nuPen(BackgroundPen, penWidth, BackgroundColor);
  }
}

void setknt() {
  auto buffer = std::vector<F_POINT_ATTR> {};
  buffer.reserve(Instance->stitchBuffer.size());
  buffer.push_back(Instance->stitchBuffer.front());
  strtknt(buffer, 0);
  auto iStitch = stlen(0) > KNOTLEN ? kjmp(buffer, 1) : 1;
  Instance->stateMap.reset(StateFlag::FILDIR);
  while (iStitch < wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)) {
	buffer.push_back(Instance->stitchBuffer.operator[](iStitch));
	if (stlen(iStitch) > KNOTLEN) {
	  endknt(buffer, iStitch);
	  iStitch = kjmp(buffer, iStitch + 1U);
	  buffer.push_back(Instance->stitchBuffer.operator[](iStitch));
	}
	++iStitch;
  }
  Instance->stateMap.set(StateFlag::FILDIR);
  endknt(buffer, iStitch);
  buffer.back().attribute &= ~KNOTMSK;
  Instance->stitchBuffer = std::move(buffer);
}

void setPrefs() {
  thred::defpref();
  getDocsFolder(ThrSingle->DefaultDirectory);
  if (ThrSingle->DesignerName.empty()) {
	ThrSingle->DesignerName.assign(displayText::loadStr(IDS_UNAM));
	thred::getdes();
  }
}

void setsped() {
  constexpr auto DEFET = uint32_t {100U}; // default elapsed time per frame. Units are milliseconds
  constexpr auto DEFUT = 10.0;            // default user time per frame. Units are milliseconds

  auto elapsedTimePerFrame = DEFET;
  if (MovieTimeStep == 0) {
	MovieTimeStep = 1;
  }
  if (auto const userTimePerFrame = wrap::toDouble(MovieTimeStep) / DEFUT; userTimePerFrame < DEFUT) {
	StitchesPerFrame        = wrap::round<uint32_t>(elapsedTimePerFrame / userTimePerFrame);
	constexpr auto SPFCLAMP = 99U; // maximum stitches per frame
	StitchesPerFrame        = std::min(StitchesPerFrame, SPFCLAMP);
  }
  else {
	elapsedTimePerFrame = wrap::round<uint32_t>(userTimePerFrame);
	StitchesPerFrame    = 2U;
  }
  StitchesPerFrame = std::max(StitchesPerFrame, 2U);
  SetTimer(ThrEdWindow, 0, elapsedTimePerFrame, nullptr);
}

void setsrch(bool const end) {
  if (end) {
	CurrentStitchIndex = LargestStitchIndex;
	SortIndex          = wrap::toUnsigned(ThrSingle->SortBuffer.size() - 1U);
  }
  else {
	CurrentStitchIndex = SmallestStitchIndex;
	SortIndex          = 0U;
  }
  lensadj();
  displayText::ritnum(IDS_NUMSCH, ClosestPointIndex);
}

void shft2box() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  auto const current     = wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex);
  auto const stitchPoint = F_POINT {current->x, current->y};
  thred::shft(stitchPoint);
  thred::stch2px1(ClosestPointIndex);
}

// ReSharper disable CppParameterMayBeConst
void shownd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_SHOW);
}
// ReSharper restore CppParameterMayBeConst

auto sidclp() -> bool {
  if (OpenClipboard(ThrEdWindow) == 0) {
	return false;
  }
  Clip       = form::registerPCDFormat();
  ClipMemory = GetClipboardData(Clip);
  if (ClipMemory == nullptr) {
	CloseClipboard();
	return false;
  }
  thred::redclp();
  CloseClipboard();
  return true;
}

void sidhup() {
  auto hoopRectangle        = RECT {};
  auto preferencesRectangle = RECT {};
  Instance->stateMap.set(StateFlag::HUPMSG);
  GetWindowRect(Instance->valueWindow.operator[](PRFHUPTYP), &hoopRectangle);
  GetWindowRect(PreferencesWindow, &preferencesRectangle);
  constexpr auto SMW_FLAGS = DWORD {WS_BORDER | WS_CHILD | WS_VISIBLE};
  SideMessageWindow        = CreateWindow(L"STATIC",
                                   nullptr,
                                   SMW_FLAGS,
                                   preferencesRectangle.right + 3 - ThredWindowOrigin.x,
                                   hoopRectangle.top - ThredWindowOrigin.y,
                                   ButtonWidthX3 + (ButtonWidth * 2) + 6,
                                   (ButtonHeight * HUPS) + 6,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  constexpr auto SW_FLAGS  = DWORD {SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER};
  for (auto iHoop = size_t {}; iHoop < HUPS; ++iHoop) {
	auto const idx = gsl::narrow_cast<int32_t>(iHoop);
	ThrSingle->SideWindow.operator[](iHoop) =
	    CreateWindow(L"STATIC",
	                 displayText::loadStr(wrap::toUnsigned(iHoop) + IDS_HUP0).c_str(),
	                 SW_FLAGS,
	                 3,
	                 (ButtonHeight * idx) + 3,
	                 ButtonWidthX3 + (ButtonWidth * 2),
	                 ButtonHeight,
	                 SideMessageWindow,
	                 nullptr,
	                 ThrEdInstance,
	                 nullptr);
  }
}

// ReSharper disable CppParameterMayBeConst
void sidmsg(FRM_HEAD const& form, HWND window) {
  auto childListRect  = RECT {};
  auto parentListRect = RECT {};
  std::ranges::fill(Instance->valueWindow, nullptr);
  auto sideWindowSize     = SIZE {};
  auto sideWindowLocation = int32_t {};
  GetWindowRect(window, &childListRect);
  GetWindowRect(FormDataSheet, &parentListRect);
  form::ispcdclp();
  if (Instance->stateMap.test(StateFlag::FILTYP)) { // if we are choosing a border fill type
	auto entryCount = gsl::narrow<int32_t>(EDGELIST.size());
	for (auto const& iEntry : EDGELIST) {
	  // calculate how many entries we will have in the side window
	  if ((form.edgeType & NEGUND) == iEntry.value) {
		--entryCount;
	  }
	  else {
		if ((iEntry.value == EDGECLIP || iEntry.value == EDGEPICOT || iEntry.value == EDGECLIPX) &&
		    !Instance->stateMap.test(StateFlag::WASPCDCLP)) {
		  --entryCount;
		}
		else {
		  formForms::maxtsiz(displayText::loadStr(iEntry.stringID), sideWindowSize);
		}
	  }
	}
	// create the side window
	SideMessageWindow = CreateWindow(L"STATIC",
	                                 nullptr,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 parentListRect.right - ThredWindowOrigin.x + 3,
	                                 childListRect.top - ThredWindowOrigin.y - 3,
	                                 sideWindowSize.cx + 12,
	                                 (sideWindowSize.cy * entryCount) + 12,
	                                 ThrEdWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
	for (auto const& iEntry : EDGELIST) {
	  // set the side window entries
	  if ((form.edgeType & NEGUND) != iEntry.value) {
		if (iEntry.value == EDGECLIP || iEntry.value == EDGEPICOT || iEntry.value == EDGECLIPX) {
		  if (Instance->stateMap.test(StateFlag::WASPCDCLP)) {
			dusid(iEntry, sideWindowLocation, sideWindowSize);
		  }
		}
		else {
		  dusid(iEntry, sideWindowLocation, sideWindowSize);
		}
	  }
	}
	Instance->stateMap.set(StateFlag::SIDACT);
	return;
  }
  auto entryCount = int32_t {0};
  // choose the side window vertical size based on the form menu choice
  switch (FormMenuChoice) {
	case LLAYR: {
	  entryCount      = gsl::narrow<int32_t>(LAYRLIST.size());
	  auto const zero = displayText::loadStr(IDS_LAY03);
	  formForms::maxtsiz(zero, sideWindowSize);
	  break;
	}
	case LFTHTYP: {
	  entryCount         = gsl::narrow<int32_t>(FTHRLIST.size() - 1U);
	  auto const fthrWid = displayText::loadStr(IDS_FTH3);
	  formForms::maxtsiz(fthrWid, sideWindowSize);
	  break;
	}
	case LFRMFIL: {
	  entryCount = gsl::narrow<int32_t>(FILLLIST.size());
	  for (auto const& iEntry : FILLLIST) {
		if (iEntry.value == form.fillType) {
		  --entryCount;
		}
		else {
		  if ((1U << iEntry.value & CLIPTYPEMAP) != 0U && !Instance->stateMap.test(StateFlag::WASPCDCLP)) {
			--entryCount;
		  }
		  else {
			formForms::maxtsiz(displayText::loadStr(iEntry.stringID), sideWindowSize);
		  }
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in sidmsg: FormMenuChoice [{}]\n", FormMenuChoice);
	  break;
	}
  }
  SideMessageWindow = CreateWindow(L"STATIC",
                                   nullptr,
                                   WS_BORDER | WS_CHILD | WS_VISIBLE,
                                   parentListRect.right - ThredWindowOrigin.x + 3,
                                   childListRect.top - ThredWindowOrigin.y - 3,
                                   sideWindowSize.cx + 12,
                                   (sideWindowSize.cy * entryCount) + 12,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  switch (FormMenuChoice) {
	// fill the side window with the appropriate entries
	case LLAYR: { // if we are choosing a layer
	  for (auto const& iEntry : LAYRLIST) {
		dusid(iEntry, sideWindowLocation, sideWindowSize);
	  }
	  break;
	}
	case LFTHTYP: { // if we are choosing a feather fill type
	  for (auto const& iEntry : FTHRLIST) {
		if (iEntry.value != form.feather.fillType) {
		  dusid(iEntry, sideWindowLocation, sideWindowSize);
		}
	  }
	  break;
	}
	case LFRMFIL: { // if we are choosing a fill type
	  for (auto const& iEntry : FILLLIST) {
		if (iEntry.value == form.fillType) {
		  continue;
		}
		if ((1U << iEntry.value & CLIPTYPEMAP) == 0U) {
		  dusid(iEntry, sideWindowLocation, sideWindowSize);
		  continue;
		}
		if (Instance->stateMap.test(StateFlag::WASPCDCLP)) {
		  dusid(iEntry, sideWindowLocation, sideWindowSize);
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"default 2 hit in sidmsg: FormMenuChoice [{}]\n", FormMenuChoice);
	  break;
	}
  }
  Instance->stateMap.set(StateFlag::SIDACT);
}
// ReSharper restore CppParameterMayBeConst

void srchk() {
  Instance->stateMap.reset(StateFlag::FORMSEL);
  Instance->selectedFormList.clear();
  if (Instance->stateMap.testAndSet(StateFlag::LENSRCH)) {
	if (Instance->stateMap.test(StateFlag::WASGRP)) {
	  ClosestPointIndex = GroupStartStitch = PrevGroupStartStitch;
	  GroupStitchIndex = GroupEndStitch = PrevGroupEndStitch;
	}
	else {
	  Instance->stateMap.reset(StateFlag::GRPSEL);
	}
  }
  else {
	if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	  Instance->stateMap.set(StateFlag::WASGRP);
	  thred::rngadj();
	  PrevGroupStartStitch = GroupStartStitch;
	  PrevGroupEndStitch   = GroupEndStitch;
	}
	else {
	  Instance->stateMap.reset(StateFlag::WASGRP);
	}
  }
  duselrng(SelectedRange);
}

auto srchknot(uint32_t const source) -> uint32_t {
  auto upper = std::ranges::find(ThrSingle->Knots, source);
  if (upper == ThrSingle->Knots.end()) {
	return 3;
  }
  --upper;
  if ((*upper > source ? *upper - source : source - *upper) < KNOTSCNT) {
	++upper;
	if ((*upper > source ? *upper - source : source - *upper) < KNOTSCNT) {
	  return 0;
	}
	return 2;
  }
  ++upper;
  if ((*upper > source ? *upper - source : source - *upper) < KNOTSCNT) {
	return 1;
  }
  return 3;
}

void stCor2px(F_POINT_ATTR const& stitch, POINT& point) {
  point = {.x = wrap::ceil<int32_t>((stitch.x - ZoomRect.left) * ZoomRatio.x),
           .y = wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    ((stitch.y - ZoomRect.bottom) * ZoomRatio.y))};
}

auto stch2px2(uint32_t const iStitch) -> bool {
  auto const stitchCoordsInPixels = POINT {
      wrap::ceil<int32_t>((Instance->stitchBuffer.operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x),
      wrap::ceil<int32_t>(
          wrap::toFloat(StitchWindowClientRect.bottom) -
          ((Instance->stitchBuffer.operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y))};
  return stitchCoordsInPixels.x >= 0 && stitchCoordsInPixels.x <= StitchWindowClientRect.right &&
         stitchCoordsInPixels.y >= 0 && stitchCoordsInPixels.y <= StitchWindowClientRect.bottom;
}

void stchPars() {
  auto const aspectRatio = wrap::toFloat(UnzoomedRect.cx) / wrap::toFloat(UnzoomedRect.cy);
  StitchWindowSize.cx =
      Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)
          ? std::lround(wrap::toFloat(ThredWindowRect.bottom - (ScrollSize * 2)) * aspectRatio)
          : std::lround(wrap::toFloat(ThredWindowRect.bottom - ScrollSize) * aspectRatio);

  if (StitchWindowSize.cx + ButtonWidthX3 + ScrollSize + ColorBarSize < ThredWindowRect.right) {
	StitchWindowSize.cy =
	    Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)
	        ? ThredWindowRect.bottom - (ScrollSize * 2)
	        : ThredWindowRect.bottom - ScrollSize;
  }
  else {
	StitchWindowSize = {.cx = ThredWindowRect.right - ButtonWidthX3 - ColorBarSize,
	                    .cy = ThredWindowRect.bottom - ThredWindowRect.top};
	auto const sizeX = wrap::toFloat(StitchWindowSize.cx);
	auto const sizeY = wrap::toFloat(StitchWindowSize.cy);
	if (auto const ratio = sizeX / sizeY; ratio > aspectRatio) {
	  StitchWindowSize.cx = std::lround(sizeY * aspectRatio);
	}
	else {
	  StitchWindowSize.cy = std::lround(sizeX / aspectRatio);
	}
  }
}

void stchWnd() {
  stchPars();
  MainStitchWin = nullptr;
  MainStitchWin = CreateWindow(L"STATIC",
                               nullptr,
                               SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
                               ButtonWidthX3,
                               0,
                               StitchWindowSize.cx,
                               StitchWindowSize.cy,
                               ThrEdWindow,
                               nullptr,
                               ThrEdInstance,
                               nullptr);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const hFont = displayText::getThrEdFont(FONTSIZE);
  SelectObject(GetDC(MainStitchWin), hFont);
  if (MainStitchWin == nullptr) {
	return;
  }
  GetWindowRect(MainStitchWin, &StitchWindowAbsRect);
  VerticalScrollBar   = CreateWindow(L"SCROLLBAR",
                                   nullptr,
                                   SBS_VERT | WS_CHILD | WS_VISIBLE,
                                   StitchWindowSize.cx + ButtonWidthX3,
                                   0,
                                   ScrollSize,
                                   StitchWindowSize.cy,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  HorizontalScrollBar = CreateWindow(L"SCROLLBAR",
                                     nullptr,
                                     SBS_HORZ | WS_CHILD | WS_VISIBLE,
                                     ButtonWidthX3,
                                     StitchWindowSize.cy,
                                     StitchWindowSize.cx,
                                     ScrollSize,
                                     ThrEdWindow,
                                     nullptr,
                                     ThrEdInstance,
                                     nullptr);
  thred::showScrollBars(false);
}

// ReSharper disable CppParameterMayBeConst
void stchbox(uint32_t const iStitch, HDC hDC) {
  auto       line   = std::array<POINT, SQPNTS> {};
  auto const layer  = (Instance->stitchBuffer.operator[](iStitch).attribute & LAYMSK) >> LAYSHFT;
  auto const offset = MulDiv(IniFile.stitchSizePixels, Instance->DPI, STDDPI);
  if (ActiveLayer != 0U && layer != 0U && layer != ActiveLayer) {
	return;
  }
  auto const stitchCoordsInPixels = thred::stch2px1(iStitch);
  line[0].x = line[3].x = line[4].x = stitchCoordsInPixels.x - offset;
  line[0].y = line[1].y = stitchCoordsInPixels.y - offset;
  line[1].x = line[2].x = stitchCoordsInPixels.x + offset;
  line[2].y = line[3].y = stitchCoordsInPixels.y + offset;
  line[4].y             = stitchCoordsInPixels.y - offset;
  wrap::polyline(hDC, line.data(), wrap::toUnsigned(line.size()));
}
// ReSharper restore CppParameterMayBeConst

void stchout() {
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	drwlstch(GroupEndStitch);
	return;
  }
  drwlstch(wrap::toUnsigned(Instance->stitchBuffer.size() - 1U));
}

void stchsnap(uint32_t const start, uint32_t const finish) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto pnt = wrap::next(Instance->stitchBuffer.begin(), start);
  for (auto i = 0U; i < finish - start; ++i) {
	pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
	pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;
	++pnt;
  }
}

auto stlen(uint32_t const iStitch) -> float {
  auto const currStitch = wrap::next(Instance->stitchBuffer.begin(), iStitch);
  auto const nextStitch = std::next(currStitch);
  return std::hypot(nextStitch->x - currStitch->x, nextStitch->y - currStitch->y);
}

void strtknt(std::vector<F_POINT_ATTR>& buffer, uint32_t const start) {
  constexpr auto KNL      = 2.0F; // knot length
  auto const     startIt  = wrap::next(Instance->stitchBuffer.begin(), start);
  auto           finishIt = std::next(startIt);
  auto           deltaX   = finishIt->x - startIt->x;
  auto           deltaY   = finishIt->y - startIt->y;
  auto           length   = (deltaX * deltaX) + (deltaY * deltaY);
  ++finishIt;
  while (length < KNL && finishIt != Instance->stitchBuffer.end()) {
	deltaX = finishIt->x - startIt->x;
	deltaY = finishIt->y - startIt->y;
	length = deltaX * deltaX + deltaY * deltaY;
	++finishIt;
  }
  if (finishIt == Instance->stitchBuffer.end()) {
	return;
  }
  --finishIt;
  length = std::sqrt(length);
  deltaX = finishIt->x - startIt->x;
  deltaY = finishIt->y - startIt->y;

  auto const            knotAttribute = startIt->attribute | KNOTMSK;
  auto const            knotStep      = F_POINT {2.0F / length * deltaX, 2.0F / length * deltaY};
  static constexpr auto KNOT_AT_START_ORDER = std::array<char, 5> {2, 3, 1, 4, 0}; // knot spacings
  for (char const iKnot : KNOT_AT_START_ORDER) {
	ofstch(buffer, start, iKnot, knotStep, knotAttribute);
  }
}

void thrInit() noexcept {
  ThrSingle = THR_SINGLE::getInstance();
}

void thrsav() {
  auto const& workingFileName = Instance->workingFileName;

  if (chkattr(workingFileName)) {
	return;
  }
  if (!Instance->stateMap.testAndReset(StateFlag::IGNAM)) {
	auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
	auto geName   = workingFileName; // intentional copy
	geName.replace_extension(L".th*");
	// NOLINTNEXTLINE(readability-qualified-auto)
	if (auto const file = FindFirstFile(geName.wstring().c_str(), &fileData); file != INVALID_HANDLE_VALUE) {
	  auto& versionNames = ThrSingle->VersionNames;
	  Instance->stateMap.reset(StateFlag::CMPDO);
	  for (auto& version : versionNames) {
		version.clear();
	  }
	  auto& fileName = fileData.cFileName;
	  duver(ThrSingle->DefaultDirectory / std::begin(fileName));
	  while (FindNextFile(file, &fileData)) {
		duver(ThrSingle->DefaultDirectory / std::begin(fileName));
	  }
	  FindClose(file);
	  fs::remove(versionNames.back());
	  for (auto iBackup = versionNames.size() - 1U; iBackup > 0; --iBackup) {
		if (!versionNames.operator[](iBackup - 1U).empty()) {
		  auto newFileName = versionNames.operator[](iBackup - 1U); // copy intended

		  auto ext   = newFileName.extension().wstring();
		  ext.back() = gsl::narrow<wchar_t>(iBackup - 1) + 's';
		  newFileName.replace_extension(ext);
		  auto eCode = std::error_code {};
		  fs::rename(versionNames.operator[](iBackup - 1U), newFileName, eCode);
		  auto const msg = eCode.message();
		  if (eCode != std::error_code {}) {
			// ToDo - find better error message
			displayText::filnopn(IDS_FNOPN, msg);
			return;
		  }
		}
	  }
	}
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const fileHandle =
      CreateFile(ThrSingle->ThrName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::crmsg(ThrSingle->ThrName);
	return;
  }
  auto output = std::vector<char> {};
  dubuf(output);
  auto bytesWritten = DWORD {};
  WriteFile(fileHandle, output.data(), wrap::toUnsigned(output.size()), &bytesWritten, nullptr);
  if (bytesWritten != output.size()) {
	displayText::showMessage(IDS_FWERR, ThrSingle->ThrName.wstring());
  }
  CloseHandle(fileHandle);
}

void unboxs() {
  if (NearestCount != 0U) {
	boxs();
	thred::resetNearest();
  }
}

void uncros() {
  if (Instance->stateMap.testAndReset(StateFlag::SCROS)) {
	cros(GroupStartStitch);
  }
  if (Instance->stateMap.testAndReset(StateFlag::ECROS)) {
	if (GroupEndStitch != GroupStartStitch) {
	  cros(GroupEndStitch);
	}
	Instance->stateMap.set(StateFlag::ELIN);
  }
}

auto unthrsh(wchar_t level) noexcept -> float {
  auto zoom = 1.0F;
  if (level == 0) {
	return 0.0F;
  }
  --level;
  while (level != 0) {
	zoom *= ZUMFCT;
	--level;
  }
  return zoom;
}

void unthum() {
  if (!Instance->stateMap.testAndReset(StateFlag::THUMSHO)) {
	return;
  }
  destroyBV();
  if (Instance->stateMap.test(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPON));
  }
  else {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
  }
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  thred::redraw(Instance->buttonWin.operator[](HHID));
  displayText::butxt(HBOXSEL, displayText::loadStr(IDS_BOXSEL));
}

struct createParams {
  BOOL bEnableNonClientDpiScaling;
};

// ReSharper disable CppParameterMayBeConst
auto CALLBACK wndProc(HWND p_hWnd, UINT const message, WPARAM wParam, LPARAM lParam) -> LRESULT {
  switch (constexpr auto LINSCROL = 0.05F; message) { // LINSCROL is the line scroll factor
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
	  handleWndProcWMINITMENU();
	  break;
	}
	case WM_HSCROLL: {
	  if (handleWndProcWMHSCROLL(wParam, LINSCROL, lParam)) {
		return 1;
	  }
	  break;
	}
	case WM_VSCROLL: {
	  if (handleWndProcWMVSCROLL(wParam, LINSCROL)) {
		return 1;
	  }
	  break;
	}
	case WM_DRAWITEM: {
	  if (handleWndProcWMDRAWITEM(lParam)) {
		return 1;
	  }
	  break;
	}
	case WM_SIZE: {
	  handleWndProcWMSIZE(p_hWnd, wParam);
	  return 1;
	}
	case WM_MOVE: {
	  handleWndProcWMMOVE(p_hWnd);
	  return 1;
	}
	case WM_CLOSE: {
	  thred::dun();
	  if (Instance->stateMap.test(StateFlag::SAVEX)) {
		return 1;
	  }
	  break;
	}
	default: {
	  break;
	}
  }
  return DefWindowProc(p_hWnd, message, wParam, lParam);
}
// ReSharper restore CppParameterMayBeConst

auto wastch(uint32_t const& formIndex) -> bool {
  return std::ranges::any_of(Instance->stitchBuffer, [&formIndex](F_POINT_ATTR const& stitch) -> bool {
	return (stitch.attribute & FRMSK) >> FRMSHFT == formIndex;
  });
}

void zRctAdj() noexcept {
  if (auto const unzoomedY = wrap::toFloat(UnzoomedRect.cy); ZoomRect.top > unzoomedY) {
	auto const adjustment = ZoomRect.top - unzoomedY;
	ZoomRect.top -= adjustment;
	ZoomRect.bottom -= adjustment;
  }
  if (ZoomRect.bottom < 0) {
	ZoomRect.top -= ZoomRect.bottom;
	ZoomRect.bottom = 0.0F;
  }
  if (auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx); ZoomRect.right > unzoomedX) {
	auto const adjustment = ZoomRect.right - unzoomedX;
	ZoomRect.right -= adjustment;
	ZoomRect.left -= adjustment;
  }
  if (ZoomRect.left < 0) {
	ZoomRect.right -= ZoomRect.left;
	ZoomRect.left = 0.0F;
  }
}

#ifdef ALLOCFAILURE
void handle_program_memory_depletion() {
  // ToDo - Make this handle the failure with more user notifiication
  std::set_new_handler(nullptr);
  outDebugString(L"Memory allocation failure\n");
  // needs to be here, otherwise nothing will be printed
  std::exit(EXIT_FAILURE);
}
#endif

} // namespace

void thred::hideColorWin() noexcept {
  auto iDefaultColorWin = ThrSingle->DefaultColorWin.begin();
  auto iUserColorWin    = ThrSingle->UserColorWin.begin();
  for (auto const& iThreadSizeWin : ThrSingle->ThreadSizeWin) {
	hidwnd(*iDefaultColorWin++);
	hidwnd(*iUserColorWin++);
	hidwnd(iThreadSizeWin);
  }
}

void thred::showColorWin() noexcept {
  auto iDefaultColorWin = ThrSingle->DefaultColorWin.begin();
  auto iUserColorWin    = ThrSingle->UserColorWin.begin();
  for (auto const& iThreadSizeWin : ThrSingle->ThreadSizeWin) {
	shownd(*iDefaultColorWin++);
	shownd(*iUserColorWin++);
	shownd(iThreadSizeWin);
  }
}

void thred::showScrollBars(bool const Show) noexcept {
  if (Show) {
	ShowWindow(HorizontalScrollBar, SW_SHOWNORMAL);
	ShowWindow(VerticalScrollBar, SW_SHOWNORMAL);
  }
  else {
	ShowWindow(VerticalScrollBar, SW_HIDE);
	ShowWindow(HorizontalScrollBar, SW_HIDE);
  }
}

auto thred::getUserPen(uint32_t const iPen) noexcept -> HPEN {
  return ThrSingle->UserPen.operator[](iPen);
}

void thred::resetSideBuffer() {
  SideWinMsgIdx           = 0;
  constexpr auto FILLCHAR = gsl::narrow_cast<wchar_t>(0U);
  std::ranges::fill(ThrSingle->SideWindowEntryBuffer, FILLCHAR);
}

void thred::resetMsgBuffer() {
  ThrSingle->MsgBuffer.clear();
  ThrSingle->MsgBuffer.push_back(0);
}

auto thred::getMsgBufferValue() -> float {
  return wrap::wcsToFloat(ThrSingle->MsgBuffer.data());
}

void thred::getdes() noexcept {
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DESNAM), ThrEdWindow, &dnamproc);
}

void thred::undat() noexcept {
  if (FormDataSheet != nullptr) {
	DestroyWindow(FormDataSheet);
	FormDataSheet = nullptr;
  }
}

void thred::duzrat() noexcept {
  if (ZoomRect.right == 0.0F) {
	ZoomRect.right = LHUPX;
  }
  if (ZoomRect.top == 0.0F) {
	ZoomRect.top = LHUPY;
  }
  ZoomRatio = F_POINT {(wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left)),
                       (wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom))};
}

auto thred::adflt(uint32_t const count) -> uint32_t {
  auto const startVertex = wrap::toUnsigned(Instance->formVertices.size());
  auto const itPoint     = Instance->formVertices.end();
  auto constexpr VAL     = F_POINT {};
  Instance->formVertices.insert(itPoint, count, VAL);
  return startVertex;
}

auto thred::adclp(uint32_t const count) -> uint32_t {
  auto const iClipPoint = wrap::toUnsigned(Instance->clipPoints.size());
  auto const itPoint    = Instance->clipPoints.end();
  auto constexpr VAL    = F_POINT {};
  Instance->clipPoints.insert(itPoint, count, VAL);
  return iClipPoint;
}

auto thred::duthrsh(float const threshold) noexcept -> uint32_t {
  auto iZoomLevel = 0U;
  if (threshold <= 0.0F) {
	return 0;
  }
  auto zoom = 1.0F;
  while (zoom > threshold) {
	zoom *= ZUMFCT;
	++iZoomLevel;
  }
  return iZoomLevel + 1U;
}

void thred::ritfcor(F_POINT const& point) {
  auto const fmtStr = format(FMT_COMPILE(L"x{:.0f} y{:.0f}"), point.x * IPFGRAN, point.y * IPFGRAN);
  displayText::butxt(HCOR, fmtStr);
}

auto thred::maxColor() noexcept -> size_t {
  if (ThrSingle->ColorChangeTable.empty()) {
	return 0U;
  }
  return ThrSingle->ColorChangeTable.size() - 1U;
}

void thred::resetColorChanges() noexcept {
  ThrSingle->ColorChangeTable.clear();
}

void thred::addColor(uint32_t const stitch, uint32_t const color) {
  ThrSingle->ColorChangeTable.emplace_back(COL_CHANGE {
      .stitchIndex = gsl::narrow<decltype(ThrSingle->ColorChangeTable.back().stitchIndex)>(stitch),
      .colorIndex  = gsl::narrow<decltype(ThrSingle->ColorChangeTable.back().colorIndex)>(color)});
}

void thred::coltab() {
  resetColorChanges();
  if (Instance->stitchBuffer.size() <= 1) {
	return;
  }
  auto const firstStitch  = Instance->stitchBuffer.begin();
  auto       currentColor = (firstStitch + 1)->attribute & COLMSK;
  firstStitch->attribute &= NCOLMSK;
  firstStitch->attribute |= currentColor;
  auto const lastStitch = Instance->stitchBuffer.rbegin();
  lastStitch->attribute &= NCOLMSK;
  lastStitch->attribute |= (lastStitch + 1)->attribute & COLMSK;

  auto const pEnd = std::prev(Instance->stitchBuffer.end());
  for (auto stitchIt = std::next(firstStitch); stitchIt < pEnd; ++stitchIt) {
	if ((stitchIt->attribute & COLMSK) == currentColor) {
	  continue;
	}
	if ((std::next(stitchIt)->attribute & COLMSK) == currentColor) {
	  stitchIt->attribute &= NCOLMSK;
	  stitchIt->attribute |= currentColor;
	}
	currentColor = stitchIt->attribute & COLMSK;
  }
  currentColor    = std::numeric_limits<decltype(currentColor)>::max();
  auto const minX = wrap::toFloat(UnzoomedRect.cx) * -1.0F;
  auto const minY = wrap::toFloat(UnzoomedRect.cy) * -1.0F;
  auto const maxX = wrap::toFloat(UnzoomedRect.cx) * 2.0F;
  auto const maxY = wrap::toFloat(UnzoomedRect.cy) * 2.0F;
  for (auto iStitch = 0U; auto& stitch : Instance->stitchBuffer) {
	stitch.x = std::clamp(stitch.x, minX, maxX);
	stitch.y = std::clamp(stitch.y, minY, maxY);

	if (auto const nextColor = stitch.attribute & COLMSK; currentColor != nextColor) {
	  addColor(iStitch, nextColor);
	  currentColor = nextColor;
	}
	++iStitch;
  }
  addColor(wrap::toUnsigned(Instance->stitchBuffer.size()), 0);
  ClosestPointIndex = std::min(ClosestPointIndex, wrap::toUnsigned(Instance->stitchBuffer.size() - 1U));
  fndknt();
}

void thred::savdo() {
  Instance->stateMap.set(StateFlag::WASDO);
  Instance->stateMap.set(StateFlag::CMPDO);
  if (!Instance->stateMap.testAndReset(StateFlag::SAVACT)) {
	return;
  }
  if (Instance->stateMap.testAndReset(StateFlag::BAKING)) {
	menu::disableRedo();
  }
  Instance->stateMap.set(StateFlag::BAKACT);
  menu::enableUndo();
  backup::dudat();
  backup::updateWriteIndex();
}

void thred::rngadj() noexcept(std::is_same_v<size_t, uint32_t>) {
  auto lastStitch = wrap::toUnsigned(Instance->stitchBuffer.size());
  if (lastStitch != 0U) {
	--lastStitch;
	ClosestPointIndex = std::min(ClosestPointIndex, lastStitch);
  }
  if (GroupStitchIndex > lastStitch) {
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

void thred::redrawColorBar() noexcept {
  RedrawWindow(ColorBar, nullptr, nullptr, RDW_INVALIDATE);
}

// ReSharper disable CppParameterMayBeConst
void thred::redraw(HWND window) noexcept {
  RedrawWindow(window, nullptr, nullptr, RDW_INVALIDATE);
  if (window != MainStitchWin) {
	return;
  }
  for (auto const& iWindow : ThrSingle->DefaultColorWin) {
	if (iWindow != nullptr) {
	  RedrawWindow(iWindow, nullptr, nullptr, RDW_INVALIDATE);
	}
  }
  RedrawWindow(ColorBar, nullptr, nullptr, RDW_INVALIDATE);
}
// ReSharper restore CppParameterMayBeConst

void thred::movStch() {
  auto clientSize =
      SIZE {(ThredWindowRect.right - ButtonWidthX3 - (ScrollSize + ColorBarSize)), ThredWindowRect.bottom};
  auto verticalOffset = 0;
  unboxs();
  if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
	verticalOffset = ScrollSize;
	clientSize.cy -= ScrollSize;
  }
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	clientSize.cy -= ScrollSize;
	MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, clientSize.cx, clientSize.cy, FALSE);
	MoveWindow(VerticalScrollBar, ButtonWidthX3 + clientSize.cx, 0, ScrollSize, clientSize.cy, TRUE);
	MoveWindow(HorizontalScrollBar, ButtonWidthX3, clientSize.cy + verticalOffset, clientSize.cx, ScrollSize, TRUE);
	StitchWindowAspectRatio = wrap::toFloat(clientSize.cx) / wrap::toFloat(clientSize.cy);
	if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
	  MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, clientSize.cx, ScrollSize, TRUE);
	}
	showScrollBars(true);
  }
  else {
	stchPars();
	auto const actualWindowHeight = StitchWindowSize.cy + ScrollSize;
	MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, StitchWindowSize.cx, actualWindowHeight, TRUE);
	showScrollBars(false);
	StitchWindowAspectRatio = wrap::toFloat(StitchWindowSize.cx) / wrap::toFloat(actualWindowHeight);
	if (Instance->stateMap.test(StateFlag::RUNPAT) || Instance->stateMap.test(StateFlag::WASPAT)) {
	  MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, StitchWindowSize.cx, ScrollSize, TRUE);
	}
  }
  MoveWindow(ColorBar, ThredWindowRect.right - ColorBarSize, 0, ColorBarSize, ThredWindowRect.bottom, TRUE);
  nuRct();
  redrawColorBar();
}

void thred::unbsho() {
  if (!Instance->stateMap.testAndReset(StateFlag::BAKSHO)) {
	return;
  }
  destroyBV();
}

void thred::unsid() noexcept {
  FormMenuChoice = 0;
  if (SideMessageWindow == nullptr) {
	return;
  }
  DestroyWindow(SideMessageWindow);
  SideMessageWindow = nullptr;
}

void thred::shft(F_POINT const& delta) noexcept {
  auto const halfZoomRect =
      F_POINT {((ZoomRect.right - ZoomRect.left) * 0.5F), ((ZoomRect.top - ZoomRect.bottom) * 0.5F)};
  auto const center = F_POINT {(ZoomRect.left + halfZoomRect.x), (ZoomRect.bottom + halfZoomRect.y)};
  auto const shift = F_POINT {(center.x - delta.x), (center.y - delta.y)};
  ZoomRect.bottom -= shift.y;
  ZoomRect.top -= shift.y;
  ZoomRect.left -= shift.x;
  ZoomRect.right -= shift.x;
  zRctAdj();
}

auto thred::stch2px1(uint32_t const iStitch) -> POINT {
  if (Instance->stitchBuffer.empty()) {
	return POINT {0L, StitchWindowClientRect.bottom};
  }
  auto const current = wrap::next(Instance->stitchBuffer.begin(), iStitch);
  return POINT {wrap::ceil<int32_t>((current->x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    ((current->y - ZoomRect.bottom) * ZoomRatio.y))};
}

void thred::unbox() {
  if (!Instance->stateMap.testAndReset(StateFlag::SELBOX)) {
	return;
  }
  SelectObject(StitchWindowDC, BoxPen[0]);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, StitchArrow.data(), wrap::toUnsigned(StitchArrow.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::ilin() noexcept {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_XORPEN);
  wrap::polyline(StitchWindowDC, &InsertLine[1], LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
  SelectObject(StitchWindowMemDC, LinePen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  wrap::polyline(StitchWindowMemDC, &InsertLine[1], LNPNTS);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::xlin() {
  if (Instance->stateMap.testAndReset(StateFlag::ILIN)) {
	ilin();
  }
}

void thred::ilin1() noexcept {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::xlin1() {
  if (Instance->stateMap.testAndReset(StateFlag::ILIN1)) {
	ilin1();
  }
}

void thred::endpnt(POINT const& stitchCoordsInPixels) {
  unbox();
  xlin();
  xlin1();
  Instance->stateMap.set(StateFlag::LIN1);
  Instance->stateMap.set(StateFlag::INSRT);
  Instance->stateMap.reset(StateFlag::GRPSEL);
  InsertLine[0] = stitchCoordsInPixels;
  InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x, .y = WinMsg.pt.y - StitchWindowOrigin.y};
}

void thred::duIns() {
  InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x, .y = WinMsg.pt.y - StitchWindowOrigin.y};
  InsertLine[0] = stch2px1(ClosestPointIndex);
  InsertLine[2] = stch2px1(ClosestPointIndex + 1U);
  xlin();
  Instance->stateMap.set(StateFlag::ILIN);
  ilin();
}

void thred::zumhom() {
  ZoomRect = F_RECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.cy), wrap::toFloat(UnzoomedRect.cx), 0.0F};
  ZoomFactor = 1;
  Instance->stateMap.reset(StateFlag::ZUMED);
  movStch();
  resetNearest();
  if (Instance->stateMap.test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	shft2box();
  }
  duzrat();
  movins();
}

void thred::hupfn() {
  auto checkHoopRect = F_RECTANGLE {}; // for checking the hoop size
  Instance->stateMap.reset(StateFlag::HUPCHNG);
  stchrct(checkHoopRect);
  auto& formList = Instance->formList;

  if (!formList.empty()) {
	if (Instance->stitchBuffer.empty()) {
	  auto const itVertex  = Instance->formVertices.cbegin();
	  checkHoopRect.top    = itVertex->y;
	  checkHoopRect.bottom = itVertex->y;
	  checkHoopRect.right  = itVertex->x;
	  checkHoopRect.left   = itVertex->x;
	}
	for (auto const& formVertice : Instance->formVertices) {
	  checkHoopRect.left   = std::min(checkHoopRect.left, formVertice.x);
	  checkHoopRect.right  = std::max(checkHoopRect.right, formVertice.x);
	  checkHoopRect.bottom = std::min(checkHoopRect.bottom, formVertice.y);
	  checkHoopRect.top    = std::max(checkHoopRect.top, formVertice.y);
	}
  }
  if (Instance->stitchBuffer.empty() && Instance->formVertices.empty() &&
      !Instance->stateMap.test(StateFlag::HUPEX)) {
	return;
  }
  if (checkHoopRect.left < 0 || checkHoopRect.right > IniFile.hoopSizeX ||
      checkHoopRect.bottom < 0 || checkHoopRect.top > IniFile.hoopSizeY) {
	Instance->stateMap.set(StateFlag::HUPEX);
  }
  if (!Instance->stateMap.test(StateFlag::HUPEX)) {
	return;
  }
  auto const hoopSize =
      F_POINT {checkHoopRect.right - checkHoopRect.left, checkHoopRect.top - checkHoopRect.bottom};
  if (hoopSize.x > IniFile.hoopSizeX) {
	IniFile.hoopSizeX = hoopSize.x;
	Instance->stateMap.set(StateFlag::HUPCHNG);
  }
  if (hoopSize.y > IniFile.hoopSizeY) {
	IniFile.hoopSizeY = hoopSize.y;
	Instance->stateMap.set(StateFlag::HUPCHNG);
  }
  auto const designCenter =
      F_POINT {(hoopSize.x * 0.5F) + checkHoopRect.left, (hoopSize.y * 0.5F) + checkHoopRect.bottom};
  auto const hoopCenter = F_POINT {IniFile.hoopSizeX * 0.5F, IniFile.hoopSizeY * 0.5F};
  auto const delta      = F_POINT {hoopCenter.x - designCenter.x, hoopCenter.y - designCenter.y};
  for (auto& stitch : Instance->stitchBuffer) {
	stitch += delta;
  }
  for (auto& formVertice : Instance->formVertices) {
	formVertice += delta;
  }
  for (auto& iForm : formList) {
	iForm.rectangle.left += delta.x;
	iForm.rectangle.top += delta.y;
	iForm.rectangle.right += delta.x;
	iForm.rectangle.bottom += delta.y;
  }
  UnzoomedRect = {.cx = std::lround(IniFile.hoopSizeX), .cy = std::lround(IniFile.hoopSizeY)};
  ZoomMin      = MINZUMF / wrap::toFloat(UnzoomedRect.cx);
  zumhom();
}

void thred::chkhup() {
  Instance->stateMap.set(StateFlag::HUPEX);
  hupfn();
  if (Instance->stateMap.test(StateFlag::INIT)) {
	formForms::prfmsg();
  }
  xt::setfchk();
}

void thred::unmsg() {
  if (MsgWindow != nullptr) {
	noMsg();
  }
}

void thred::rstAll() {
  Instance->stateMap.reset(StateFlag::WASFPNT);
  Instance->stateMap.reset(StateFlag::WASFRMFRM);
  Instance->stateMap.reset(StateFlag::FPUNCLP);
  Instance->stateMap.reset(StateFlag::FPSEL);
  Instance->stateMap.reset(StateFlag::CAPT);
  Instance->stateMap.reset(StateFlag::INSRT);
  Instance->stateMap.reset(StateFlag::SELBOX);
  Instance->stateMap.reset(StateFlag::GRPSEL);
  Instance->stateMap.reset(StateFlag::SCROS);
  Instance->stateMap.reset(StateFlag::ECROS);
  Instance->stateMap.reset(StateFlag::LIN1);
  Instance->stateMap.reset(StateFlag::CLPSHO);
  Instance->stateMap.reset(StateFlag::SELPNT);
  Instance->stateMap.reset(StateFlag::ROTAT);
  Instance->stateMap.reset(StateFlag::ROTCAPT);
  Instance->stateMap.reset(StateFlag::FRMPMOV);
  Instance->stateMap.reset(StateFlag::MOVFRM);
  Instance->stateMap.reset(StateFlag::SATCNKT);
  Instance->stateMap.reset(StateFlag::FRMPSEL);
  Instance->stateMap.reset(StateFlag::SHOINSF);
  Instance->stateMap.reset(StateFlag::STRTCH);
  Instance->stateMap.reset(StateFlag::SHOSTRTCH);
  Instance->stateMap.reset(StateFlag::EXPAND);
  Instance->stateMap.reset(StateFlag::POLIMOV);
  Instance->stateMap.reset(StateFlag::BZUMIN);
  Instance->stateMap.reset(StateFlag::OSAV);
  Instance->stateMap.reset(StateFlag::SAVEX);
  Instance->stateMap.reset(StateFlag::MOVFRMS);
  Instance->stateMap.reset(StateFlag::FRMROT);
  Instance->stateMap.reset(StateFlag::DELSFRMS);
  Instance->stateMap.reset(StateFlag::BIGBOX);
  Instance->stateMap.reset(StateFlag::UPTO);
  Instance->stateMap.reset(StateFlag::LENSRCH);
  Instance->stateMap.reset(StateFlag::WASGRP);
  Instance->stateMap.reset(StateFlag::BOXZUM);
  Instance->stateMap.reset(StateFlag::HIDSTCH);
  Instance->stateMap.reset(StateFlag::ENTRPOL);
  Instance->stateMap.reset(StateFlag::ENTRSTAR);
  Instance->stateMap.reset(StateFlag::ENTRSPIR);
  Instance->stateMap.reset(StateFlag::ENTRHART);
  Instance->stateMap.reset(StateFlag::FORMIN);
  Instance->stateMap.reset(StateFlag::GTUANG);
  Instance->stateMap.reset(StateFlag::GTUSPAC);
  Instance->stateMap.reset(StateFlag::GTWLKIND);
  Instance->stateMap.reset(StateFlag::GTWLKLEN);
  trace::untrace();
  Instance->stateMap.reset(StateFlag::WASEDG);
  displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
  if (util::closeEnough(ZoomFactor, 1.0F)) {
	Instance->stateMap.reset(StateFlag::ZUMED);
  }
  movStch();
  ShowWindow(MainStitchWin, SW_SHOWNORMAL);
  unbsho();
  Instance->stateMap.reset(StateFlag::SIDACT);
  unsid();
  Instance->stateMap.reset(StateFlag::PRFACT);
  Instance->stateMap.reset(StateFlag::WASRT);
  DestroyWindow(PreferencesWindow);
  undat();
  if (Instance->stateMap.testAndReset(StateFlag::INSFRM)) {
	insadj();
  }
  Instance->stateMap.reset(StateFlag::FUNCLP);
  if (Instance->stateMap.testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (Instance->stateMap.testAndReset(StateFlag::RUNPAT)) {
	patdun();
  }
  Instance->stateMap.reset(StateFlag::FORMSEL);
  Instance->stateMap.reset(StateFlag::FRMPSEL);
  if (!Instance->selectedFormList.empty()) {
	Instance->selectedFormList.clear();
  }
  unmsg();
  Instance->searchLine.clear();
  Instance->searchLine.shrink_to_fit();
  FirstWin = nullptr;
  while (EnumChildWindows(MainStitchWin, enumChildProc, 0) != 0) { }
}

void thred::ritot(uint32_t const number) {
  auto const txt = displayText::format(IDS_TOT, number);
  displayText::butxt(HTOT, txt);
}

void thred::lenCalc() {
  auto static chkVal = 0.0F;
  auto const blank   = std::wstring {};
  if (Instance->stateMap.test(StateFlag::LENSRCH)) {
	auto const stitch     = wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex);
	auto const stitchFwd1 = std::next(stitch);
	if (auto const lenMax = std::hypot(stitchFwd1->x - stitch->x, stitchFwd1->y - stitch->y) * IPFGRAN;
	    !util::closeEnough(lenMax, chkVal)) {
	  chkVal = lenMax;
	  displayText::butxt(HMINLEN, format(FMT_COMPILE(L"{:.2f}"), lenMax));
	}
	displayText::butxt(HMAXLEN, displayText::loadStr(IDS_SRCH));
	return;
  }
  if (Instance->stitchBuffer.size() <= 1U) {
	displayText::butxt(HMAXLEN, blank);
	displayText::butxt(HMINLEN, blank);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	frmcalc(LargestStitchIndex, SmallestStitchIndex);
	displayText::butxt(HCOR, blank);
	return;
  }
  rngadj();
  if (Instance->stateMap.test(StateFlag::GRPSEL) && GroupStartStitch != GroupEndStitch) {
	lenfn(GroupStartStitch, GroupEndStitch - 1U, LargestStitchIndex, SmallestStitchIndex);
  }
  else {
	lenfn(0, wrap::toUnsigned(Instance->stitchBuffer.size() - 2U), LargestStitchIndex, SmallestStitchIndex);
  }
}

void thred::duzero() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  auto const minStitch = SmallStitchLength * SmallStitchLength;
  if (!Instance->selectedFormList.empty()) {
	auto formMap = boost::dynamic_bitset(Instance->formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
	for (auto const selectedForm : Instance->selectedFormList) {
	  formMap.set(selectedForm);
	}
	Instance->stateMap.reset(StateFlag::CONTIG);
	auto iDestination  = Instance->stitchBuffer.begin();
	auto currentStitch = Instance->stitchBuffer.front(); // intentional copy
	for (auto const& iStitch : Instance->stitchBuffer) {
	  if ((iStitch.attribute & TYPMSK) == 0U || !formMap.test((iStitch.attribute & FRMSK) >> FRMSHFT)) {
		*iDestination = iStitch;
		++iDestination;
		Instance->stateMap.reset(StateFlag::CONTIG);
		continue;
	  }
	  if (!Instance->stateMap.testAndSet(StateFlag::CONTIG)) {
		++iDestination;
		continue;
	  }
	  auto const deltaX = iStitch.x - currentStitch.x;
	  auto const deltaY = iStitch.y - currentStitch.y;
	  if (auto const stitchLength = (deltaX * deltaX) + (deltaY * deltaY); stitchLength <= minStitch) {
		continue;
	  }
	  currentStitch = iStitch;
	  *iDestination = iStitch;
	  ++iDestination;
	}
	Instance->stitchBuffer.erase(iDestination, Instance->stitchBuffer.end());
	coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	rngadj();
	delsmal(GroupStartStitch, GroupEndStitch);
  }
  else {
	delsmal(0, wrap::toUnsigned(Instance->stitchBuffer.size()));
  }
}

void thred::pgdwn() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, std::lround((ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	rshft(scrollPosition);
  }
}

void thred::pgup() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, std::lround(-(ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	rshft(scrollPosition);
  }
}

void thred::pglft() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {std::lround((ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
	rshft(scrollPosition);
  }
}

void thred::pgrit() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {std::lround(-(ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
	rshft(scrollPosition);
  }
}

void thred::selRct(F_RECTANGLE& sourceRect) noexcept(!std::is_same_v<size_t, uint32_t>) {
  if (!Instance->stitchBuffer.empty()) {
	auto minX = BIGFLOAT;
	auto minY = BIGFLOAT;
	auto maxX = LOWFLOAT;
	auto maxY = LOWFLOAT;

	if (GroupEndStitch != GroupStartStitch) {
	  for (auto const groupStitchRange =
	           std::ranges::subrange(wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch),
	                                 wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch));
	       auto& stitch : groupStitchRange) {
		minX = std::min(minX, stitch.x);
		minY = std::min(minY, stitch.y);
		maxX = std::max(maxX, stitch.x);
		maxY = std::max(maxY, stitch.y);
	  }
	  sourceRect = F_RECTANGLE {minX, maxY, maxX, minY};
	}
	else {
	  auto const& stitch = Instance->stitchBuffer.at(GroupStartStitch);
	  sourceRect         = F_RECTANGLE {stitch.x, stitch.y, stitch.x, stitch.y};
	}
  }
  if (util::closeEnough(sourceRect.right, sourceRect.left)) {
	++sourceRect.right;
	--sourceRect.left;
  }
  if (util::closeEnough(sourceRect.top, sourceRect.bottom)) {
	++sourceRect.top;
	--sourceRect.bottom;
  }
}

void thred::unsel() {
  if (Instance->stateMap.testAndReset(StateFlag::SELSHO)) {
	dusel(StitchWindowDC);
  }
}

void thred::grpAdj() {
  uncros();
  unsel();
  rngadj();
  ducros(StitchWindowDC);
  lenCalc();
  selRct(StitchRangeRect);
  if (!Instance->stateMap.test(StateFlag::ZUMED) || GroupEndStitch == GroupStartStitch) {
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (StitchRangeRect.top <= ZoomRect.top - 1 && StitchRangeRect.bottom >= ZoomRect.bottom - 1 &&
      StitchRangeRect.left >= ZoomRect.left + 1 && StitchRangeRect.right <= ZoomRect.right - 1) {
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  auto newSize = F_POINT {std::round(StitchRangeRect.right - StitchRangeRect.left),
                          std::round(StitchRangeRect.top - StitchRangeRect.bottom)};
  if (newSize.x < MINZUMF) {
	newSize.x = std::max(newSize.x, 1.0F);

	auto const coordinate = MINZUMF / newSize.x;
	newSize               = F_POINT {MINZUMF, std::round(coordinate * newSize.y)};
  }
  constexpr auto ZMARGIN = 1.25F; // zoom margin for select zooms
  if (newSize.x > newSize.y) {
	auto coordinate = newSize.x * ZMARGIN;
	newSize.x += std::round(coordinate);
	coordinate = newSize.x / StitchWindowAspectRatio;
	newSize.y  = std::round(coordinate);
  }
  else {
	auto coordinate = newSize.y * ZMARGIN;
	newSize.y       = std::round(coordinate);
	coordinate      = newSize.y * StitchWindowAspectRatio;
	newSize.x       = std::round(coordinate);
  }
  if (newSize.x > wrap::toFloat(UnzoomedRect.cx) || newSize.y > wrap::toFloat(UnzoomedRect.cy)) {
	ZoomRect.left = ZoomRect.bottom = 0.0F;
	ZoomRect.right                  = wrap::toFloat(UnzoomedRect.cx);
	ZoomRect.top                    = wrap::toFloat(UnzoomedRect.cy);
	Instance->stateMap.reset(StateFlag::ZUMED);
	ZoomFactor = 1.0;
	movStch();
  }
  else {
	ZoomRect.right = ZoomRect.left + newSize.x;
	ZoomFactor     = newSize.x / wrap::toFloat(UnzoomedRect.cx);
	ZoomRect.top   = ZoomRect.bottom + newSize.y;
	auto const stitchPoint =
	    F_POINT {((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left,
	             ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom};
	shft(stitchPoint);
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::nuAct(uint32_t const iStitch) noexcept {
  auto const color = ActiveColor;
  if (!Instance->stitchBuffer.empty()) {
	ActiveColor = Instance->stitchBuffer.operator[](iStitch).attribute & COLMSK;
  }
  else {
	ActiveColor = 0;
  }
  redraw(ThrSingle->UserColorWin.operator[](color));
  redraw(ThrSingle->UserColorWin.operator[](ActiveColor));
}

void thred::nuPen(HPEN& pen, int32_t const width, COLORREF const color) noexcept {
  DeleteObject(pen);
  pen = wrap::createPen(PS_SOLID, width, color);
}

void thred::dubox(POINT const& stitchCoordsInPixels) {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  auto const stitch = wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex);
  if (ClosestPointIndex != Instance->stitchBuffer.size() - 1U) {
	// if the selected point is not at the end then aim at the next point
	auto const stitchFwd1 = std::next(stitch);
	RotateAngle           = std::atan2f(stitchFwd1->y - stitch->y, stitchFwd1->x - stitch->x);
  }
  else { // otherwise aim in the same direction
	auto const stitchBck1 = std::next(stitch, -1);
	RotateAngle           = std::atan2f(stitch->y - stitchBck1->y, stitch->x - stitchBck1->x);
  }
  duar(stitchCoordsInPixels);
  Instance->stateMap.reset(StateFlag::ELIN);
  Instance->stateMap.set(StateFlag::SELBOX);
  Instance->stateMap.reset(StateFlag::FRMPSEL);
  redrawColorBar();
  displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
}

auto thred::stch2px(uint32_t const iStitch, POINT& stitchCoordsInPixels) -> bool {
  if (Instance->stitchBuffer.empty()) {
	return false;
  }
  stitchCoordsInPixels = {
      .x = wrap::ceil<int32_t>((Instance->stitchBuffer.operator[](iStitch).x - ZoomRect.left) *
                               ZoomRatio.x),
      .y = wrap::ceil<int32_t>(
          wrap::toFloat(StitchWindowClientRect.bottom) -
          ((Instance->stitchBuffer.operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y))};
  return stitchCoordsInPixels.x >= 0 && stitchCoordsInPixels.x <= StitchWindowClientRect.right &&
         stitchCoordsInPixels.y >= 0 && stitchCoordsInPixels.y <= StitchWindowClientRect.bottom;
}

auto thred::stch2pxr(F_POINT const& stitchCoordinate) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    ((stitchCoordinate.y - ZoomRect.bottom) * ZoomRatio.y))};
}

void thred::reldun() {
  texture::txdun();
  bal::ritbal();
  ritini();
  mouse::dstcurs();
  PostQuitMessage(0);
}

void thred::savAs() {
  if (Instance->stitchBuffer.empty() && Instance->formList.empty() && bitmap::ismap()) {
	return;
  }
  auto  index           = FileIndices {};
  auto& workingFileName = Instance->workingFileName;
  if (!getSaveName(workingFileName, index)) {
	return;
  }
  ThrSingle->DefaultDirectory = workingFileName.parent_path();
  switch (index) {
	case FileIndices::THR: {
	  workingFileName.replace_extension(L".thr");
	  break;
	}
	case FileIndices::PCS: {
	  workingFileName.replace_extension(L".pcs");
	  menu::setpcs();
	  break;
	}
#if PESACT
	case FileIndices::PES: {
	  workingFileName.replace_extension(L".pes");
	  menu::setpes();
	  break;
	}
#endif
	case FileIndices::DST: {
	  workingFileName.replace_extension(L".dst");
	  menu::setdst();
	  break;
	}
	// ReSharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  throw std::runtime_error("Unknown file type");
	}
  }
  Instance->stateMap.set(StateFlag::SAVAS);
  nunams();
  ritini();
  Instance->stateMap.reset(StateFlag::SAVAS);
  Instance->stateMap.reset(StateFlag::CMPDO);
  thrsav();
  sav();
  SetWindowText(ThrEdWindow, ThrSingle->ThrName.wstring().c_str());
}

void thred::save() {
  auto& workingFileName = Instance->workingFileName;
  if (workingFileName.empty()) {
	savAs();
	return;
  }
  if (workingFileName.extension().empty()) {
	workingFileName /= L".thr";
  }
  thrsav();
  sav();
}

void thred::dun() {
  texture::savtxt();
  texture::txof();
  texture::rstxt();
  if (Instance->stateMap.testAndReset(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	Instance->stateMap.reset(StateFlag::WASRT);
  }
  unsid();
  unbsho();
  rstAll();
  //	if(savcmp() || (*BalaradName0 && *BalaradName1 && PCSHeader.stitchCount && !FormIndex))
  if (auto const& balaradName0 = bal::getBN0(); savcmp() || !balaradName0.empty()) {
	reldun();
	return;
  }
  if (StitchWindowClientRect.right != 0) {
	displayText::savdisc();
	Instance->stateMap.set(StateFlag::SAVEX);
	return;
  }
  auto const fmtStr = displayText::format(IDS_SAVFIL, ThrSingle->ThrName.wstring());
  if (MessageBox(ThrEdWindow, fmtStr.c_str(), displayText::loadStr(IDS_CLOS).c_str(), MB_YESNO) == IDYES) {
	save();
  }
  reldun();
}

auto thred::inDefaultColorWindows() -> bool {
  return chkMsgs(WinMsg.pt, ThrSingle->DefaultColorWin.front(), ThrSingle->DefaultColorWin.back());
}

auto thred::inChangeThreadWindows() -> bool {
  return chkMsgs(WinMsg.pt, ChangeThreadSizeWin.front(), ChangeThreadSizeWin.back());
}

auto thred::inThreadWindows() -> bool {
  return chkMsgs(WinMsg.pt, ThrSingle->ThreadSizeWin.front(), ThrSingle->ThreadSizeWin.back());
}

auto thred::inUserColorWindows() -> bool {
  return chkMsgs(WinMsg.pt, ThrSingle->UserColorWin.front(), ThrSingle->UserColorWin.back());
}

auto thred::inThreadSizeWindows() -> bool {
  return chkMsgs(WinMsg.pt, ThrSingle->ThreadSizeWin.front(), ThrSingle->ThreadSizeWin.back());
}

auto thred::getFileSize(fs::path const& newFileName, uintmax_t& size) -> bool {
  auto eCode = std::error_code {};
  size       = file_size(newFileName, eCode); // NOLINT
  if (eCode != std::error_code {}) {
	// ToDo - find better error message
	displayText::filnopn(IDS_FNOPN, newFileName);
	return false;
  }
  return true;
}

auto thred::getFileHandle(fs::path const& newFileName, HANDLE& fileHandle) -> bool {
  // ToDo - use ifstream?
  // ifstream file(workingFileName, ios::in | ios::binary | ios::ate);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const handle =
      CreateFile(newFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (handle == INVALID_HANDLE_VALUE) {
	if (GetLastError() == ERROR_SHARING_VIOLATION) {
	  displayText::filnopn(IDS_FNOPNA, newFileName);
	}
	else {
	  displayText::filnopn(IDS_FNOPN, newFileName);
	}
	fileHandle = nullptr;
	return false;
  }
  fileHandle = handle;
  return true;
}

void thred::nuTHRfile() {
  nuFil(FileIndices::THR);
}

auto thred::pxCor2stch(POINT const& point) noexcept -> F_POINT {
  auto const ratioX =
      wrap::toFloat(point.x - StitchWindowAbsRect.left) / wrap::toFloat(StitchWindowClientRect.right);
  auto const ratioY =
      wrap::toFloat(StitchWindowAbsRect.bottom - point.y) / wrap::toFloat(StitchWindowClientRect.bottom);
  return F_POINT {(ratioX * (ZoomRect.right - ZoomRect.left)) + ZoomRect.left,
                  (ratioY * (ZoomRect.top - ZoomRect.bottom)) + ZoomRect.bottom};
}

auto thred::inStitchWin() noexcept -> bool {
  return WinMsg.pt.x >= StitchWindowAbsRect.left && WinMsg.pt.x <= StitchWindowAbsRect.right &&
         WinMsg.pt.y >= StitchWindowAbsRect.top && WinMsg.pt.y <= StitchWindowAbsRect.bottom;
}

void thred::zumin() {
  if (!Instance->stateMap.testAndReset(StateFlag::ZUMACT)) {
	ZoomFactor *= ZUMFCT;
  }
  ZoomFactor = std::max(ZoomFactor, ZoomMin);
  if (!Instance->stateMap.testAndSet(StateFlag::ZUMED)) {
	movStch();
  }
  auto stitchPoint = pxCor2stch(WinMsg.pt);
  if (!Instance->stateMap.testAndReset(StateFlag::BZUMIN)) { // zoom box is not active
	while (true) {
	  if (Instance->stateMap.test(StateFlag::GMRK)) { // zoom to the mark
		stitchPoint = ZoomMarkPoint;
		break;
	  }
	  auto const& formList = Instance->formList;

	  if (Instance->stateMap.test(StateFlag::FORMSEL)) { // zoom to the selected form
		auto const& boundingRect = formList.operator[](ClosestFormToCursor).rectangle;

		stitchPoint = F_POINT {wrap::midl(boundingRect.right, boundingRect.left),
		                       wrap::midl(boundingRect.top, boundingRect.bottom)};
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::FRMPSEL)) { // zoom to the selected form vertex
		auto const itVertex =
		    wrap::next(Instance->formVertices.cbegin(),
		               formList.operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
		stitchPoint = *itVertex;
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::SELBOX)) { // zoom to the selected box
		stitchPoint = Instance->stitchBuffer.operator[](ClosestPointIndex);
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::GRPSEL)) { // zoom to the selected group of stitches
		auto groupBoundingRect = F_RECTANGLE {};
		selRct(groupBoundingRect);
		stitchPoint = F_POINT {wrap::midl(groupBoundingRect.right, groupBoundingRect.left),
		                       wrap::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::INSRT)) { // zoom to the insert stitch point
		if (Instance->stateMap.test(StateFlag::LIN1)) {
		  stitchPoint = Instance->stateMap.test(StateFlag::BAKEND) ? Instance->stitchBuffer.back()
		                                                           : Instance->stitchBuffer.front();
		}
		else {
		  stitchPoint = F_POINT {
		      wrap::midl(Instance->stitchBuffer.operator[](wrap::toSize(ClosestPointIndex) + 1U).x,
		                 Instance->stitchBuffer.operator[](ClosestPointIndex).x),
		      wrap::midl(Instance->stitchBuffer.operator[](wrap::toSize(ClosestPointIndex) + 1U).y,
		                 Instance->stitchBuffer.operator[](ClosestPointIndex).y)};
		}
		break;
	  }
	  if (!Instance->selectedFormList.empty()) { // zoom to the selected forms
		auto const  firstForm = Instance->selectedFormList.front();
		auto const& firstRect = formList.operator[](firstForm).rectangle;

		SelectedFormsRect = {.left   = std::lround(firstRect.left),
		                     .top    = std::lround(firstRect.top),
		                     .right  = std::lround(firstRect.right),
		                     .bottom = std::lround(firstRect.bottom)};
		for (auto const selectedForm : Instance->selectedFormList) {
		  auto const& rect = formList.operator[](selectedForm).rectangle;
		  if (rect.bottom < wrap::toFloat(SelectedFormsRect.bottom)) {
			SelectedFormsRect.bottom = std::lround(rect.bottom);
		  }
		  if (rect.top > wrap::toFloat(SelectedFormsRect.top)) {
			SelectedFormsRect.top = std::lround(rect.top);
		  }
		  if (rect.left < wrap::toFloat(SelectedFormsRect.left)) {
			SelectedFormsRect.left = std::lround(rect.left);
		  }
		  if (rect.right > wrap::toFloat(SelectedFormsRect.right)) {
			SelectedFormsRect.right = std::lround(rect.right);
		  }
		}
		stitchPoint = F_POINT {wrap::midl(SelectedFormsRect.right, SelectedFormsRect.left),
		                       wrap::midl(SelectedFormsRect.top, SelectedFormsRect.bottom)};
		break;
	  }
	  if (!inStitchWin()) { // nothing is selected or active, so zoom to the center of the hoop
		stitchPoint = centr();
	  }
	  break;
	}
  }
  auto const zoomRight = wrap::toFloat(UnzoomedRect.cx) * ZoomFactor;
  ZoomRect             = F_RECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
  shft(stitchPoint);
  resetNearest();
  if (!Instance->stateMap.test(StateFlag::GMRK) && Instance->stateMap.test(StateFlag::SELBOX)) {
	shft2box();
  }
  if (Instance->stateMap.test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  duzrat();
  movins();
  Instance->stateMap.set(StateFlag::RESTCH);
  ilin();
}

void thred::zumshft() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	unboxs();
	if (inStitchWin()) {
	  resetNearest();
	  shft(pxCor2stch(WinMsg.pt));
	  if (Instance->stateMap.test(StateFlag::RUNPAT)) {
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		RunPoint = 0;
	  }
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
  }
  movins();
}

void thred::zumout() {
  unboxs();
  auto stitchPoint = pxCor2stch(WinMsg.pt);
  if (!Instance->stateMap.test(StateFlag::ZUMED)) {
	return;
  }
  while (true) {
	if (Instance->stateMap.test(StateFlag::GMRK)) {
	  stitchPoint = ZoomMarkPoint;
	  break;
	}
	auto const& formList = Instance->formList;

	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  auto const& boundingRect = formList.operator[](ClosestFormToCursor).rectangle;

	  stitchPoint = F_POINT {wrap::midl(boundingRect.right, boundingRect.left),
	                         wrap::midl(boundingRect.top, boundingRect.bottom)};
	  break;
	}
	if (Instance->stateMap.test(StateFlag::FRMPSEL)) {
	  auto const itVertex =
	      wrap::next(Instance->formVertices.cbegin(),
	                 formList.operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	  stitchPoint = *itVertex;
	  break;
	}
	if (Instance->stateMap.test(StateFlag::SELBOX) || Instance->stateMap.test(StateFlag::INSRT)) {
	  stitchPoint = Instance->stitchBuffer.operator[](ClosestPointIndex);
	  break;
	}
	if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	  auto groupBoundingRect = F_RECTANGLE {};
	  selRct(groupBoundingRect);
	  stitchPoint = F_POINT {wrap::midl(groupBoundingRect.right, groupBoundingRect.left),
	                         wrap::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
	  break;
	}
	if (Instance->stateMap.test(StateFlag::SELBOX)) {
	  shft2box();
	  break;
	}
	if (!inStitchWin()) {
	  stitchPoint = centr();
	}
	break;
  }
  ZoomFactor /= ZUMFCT;
  // clamp the zoom factor
  if (constexpr auto ZMCLAMP = 0.98F; ZoomFactor > ZMCLAMP) {
	ZoomFactor = 1;
	Instance->stateMap.reset(StateFlag::ZUMED);
	ZoomRect = F_RECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.cy), wrap::toFloat(UnzoomedRect.cx), 0.0F};
	movStch();
	resetNearest();
  }
  else {
	auto const zoomRight = wrap::toFloat(UnzoomedRect.cx) * ZoomFactor;
	ZoomRect             = F_RECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
	shft(stitchPoint);
  }
  if (Instance->stateMap.test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
  duzrat();
  movins();
}

void thred::closPnt() {
  unbox();
  unboxs();
  auto&       nearestPoint     = ThrSingle->NearestPoint;
  auto const& colorChangeTable = ThrSingle->ColorChangeTable;

  auto gapToNearest = std::vector<float> {};          // distances of the closest points
  gapToNearest.resize(nearestPoint.size(), BIGFLOAT); // to a mouse click
  nearestPoint.assign(nearestPoint.size(), BIGUINT);
  auto const stitchPoint = pxCor2stch(WinMsg.pt);
  for (auto iColor = size_t {}; iColor < maxColor(); ++iColor) {
	auto const iStitch0 = colorChangeTable.operator[](iColor).stitchIndex;

	auto const iStitch1 =
	    gsl::narrow_cast<decltype(iStitch0)>(colorChangeTable.operator[](iColor + 1U).stitchIndex - iStitch0);
	if (Instance->stateMap.test(StateFlag::HID)) {
	  if (colorChangeTable.operator[](iColor).colorIndex == ActiveColor) {
		duClos(iStitch0, iStitch1, stitchPoint, gapToNearest);
	  }
	}
	else {
	  duClos(iStitch0, iStitch1, stitchPoint, gapToNearest);
	}
  }
  GetClientRect(MainStitchWin, &StitchWindowClientRect);
  auto stitchCoordsInPixels = POINT {};
  for (auto const& iNear : nearestPoint) {
	if (stch2px(iNear, stitchCoordsInPixels)) {
	  ThrSingle->NearestPixel.operator[](NearestCount++) = stitchCoordsInPixels;
	  nearestPoint.operator[](NearestCount)              = iNear;
	}
  }
  boxs();
}

auto thred::closPnt1(uint32_t& closestStitch) -> bool {
  auto       closestIndex = 0U;
  auto const pointToCheck =
      POINT {(WinMsg.pt.x - StitchWindowOrigin.x), (WinMsg.pt.y - StitchWindowOrigin.y)};
  auto stitchCoordsInPixels = POINT {};
  if (Instance->stateMap.test(StateFlag::SELBOX) && stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	if (std::hypot(stitchCoordsInPixels.x - pointToCheck.x, stitchCoordsInPixels.y - pointToCheck.y) < CLOSENUF) {
	  closestStitch = ClosestPointIndex;
	  return true;
	}
  }
  if (NearestCount != 0) {
	auto itBoxOffset = BoxOffset.begin();
	auto npi         = ThrSingle->NearestPixel.begin();
	auto npo         = ThrSingle->NearestPoint.begin();
	for (auto iNear = 0U; iNear < NearestCount; ++iNear) {
	  auto const offset = *itBoxOffset++;
	  if (auto const& pixel = *(npi++);
	      pointToCheck.x >= pixel.x - offset && pointToCheck.x <= pixel.x + offset &&
	      pointToCheck.y >= pixel.y - offset && pointToCheck.y <= pixel.y + offset) {
		closestStitch = *npo;
		return true;
	  }
	  ++npo;
	}
  }
  auto const stitchPoint     = pxCor2stch(WinMsg.pt);
  auto       distanceToClick = BIGFLOAT;
  if (Instance->stateMap.test(StateFlag::HID)) {
	auto const maxIt = wrap::next(ThrSingle->ColorChangeTable.end(), -1);
	for (auto colorIt = ThrSingle->ColorChangeTable.begin(); colorIt != maxIt; ++colorIt) {
	  if (colorIt->colorIndex != ActiveColor) {
		continue;
	  }
	  auto       stitch   = std::next(Instance->stitchBuffer.begin(), colorIt->stitchIndex);
	  auto const maxColor = std::next(colorIt)->stitchIndex;
	  for (auto iStitch = colorIt->stitchIndex; iStitch < maxColor; ++iStitch) {
		if (stitch->x < ZoomRect.left || stitch->x > ZoomRect.right ||
		    stitch->y < ZoomRect.bottom || stitch->y > ZoomRect.top) {
		  ++stitch;
		  continue;
		}
		auto const deltaX = stitch->x - stitchPoint.x;
		auto const deltaY = stitch->y - stitchPoint.y;
		if (auto const distance = (deltaX * deltaX) + (deltaY * deltaY); distance < distanceToClick) {
		  distanceToClick = distance;
		  closestIndex    = iStitch;
		}
		++stitch;
	  }
	}
  }
  else {
	auto currentStitch = 0U;
	for (auto const& stitch : Instance->stitchBuffer) {
	  if (auto const layer = (stitch.attribute & LAYMSK) >> LAYSHFT;
	      ActiveLayer != 0U && layer != 0U && layer != ActiveLayer) {
		++currentStitch;
		continue;
	  }
	  if (stitch.x < ZoomRect.left || stitch.x > ZoomRect.right || stitch.y < ZoomRect.bottom ||
	      stitch.y > ZoomRect.top) {
		++currentStitch;
		continue;
	  }
	  auto const deltaX = stitch.x - stitchPoint.x;
	  auto const deltaY = stitch.y - stitchPoint.y;
	  if (auto const distance = (deltaX * deltaX) + (deltaY * deltaY); distance < distanceToClick) {
		distanceToClick = distance;
		closestIndex    = currentStitch;
	  }
	  ++currentStitch;
	}
  }
  if (util::closeEnough(distanceToClick, BIGFLOAT)) {
	return false;
  }
  stch2px(closestIndex, stitchCoordsInPixels);
  if (Instance->stateMap.test(StateFlag::IGNTHR)) {
	closestStitch = closestIndex;
	return true;
  }
  if (std::hypot(pointToCheck.x - stitchCoordsInPixels.x, pointToCheck.y - stitchCoordsInPixels.y) < CLOSENUF) {
	closestStitch = closestIndex;
	return true;
  }
  return false;
}

auto thred::pt2colInd(uint32_t const iStitch) noexcept -> uint32_t {
  auto iColor = 0U;
  for (; iColor < maxColor(); ++iColor) {
	if (ThrSingle->ColorChangeTable.operator[](iColor).stitchIndex > iStitch) {
	  break;
	}
  }
  return iColor;
}

auto thred::findFirstStitch(uint32_t form) -> uint32_t { // find the first stitch in the selected form
  if (auto const formFirstStitch = std::ranges::find_if(Instance->stitchBuffer,
                                                        [form](F_POINT_ATTR const& stitch) -> bool {
	                                                      if ((stitch.attribute & NOTFRM) != 0U) {
	                                                        return false;
	                                                      }
	                                                      return (stitch.attribute & FRMSK) >> FRMSHFT == form;
                                                        });
      formFirstStitch != Instance->stitchBuffer.end()) {
	return wrap::toUnsigned(std::distance(Instance->stitchBuffer.begin(), formFirstStitch));
  }
  return wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
}

auto thred::findLastStitch(uint32_t form) -> uint32_t { // find the first stitch in the selected form
  if (auto const formFirstStitch = std::ranges::find_if(std::ranges::reverse_view(Instance->stitchBuffer),
                                                        [form](F_POINT_ATTR const& stitch) -> bool {
	                                                      if ((stitch.attribute & NOTFRM) != 0U) {
	                                                        return false;
	                                                      }
	                                                      return (stitch.attribute & FRMSK) >> FRMSHFT == form;
                                                        });
      formFirstStitch != Instance->stitchBuffer.rend()) {
	return wrap::toUnsigned(std::distance(formFirstStitch, Instance->stitchBuffer.rend()) - 1);
  }
  return wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
}

void thred::toglup() {
  if (Instance->stateMap.testAndFlip(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL)) {
	rngadj();
	ClosestPointIndex = GroupStartStitch;
	Instance->stateMap.set(StateFlag::SELBOX);
	Instance->stateMap.reset(StateFlag::FRMPSEL);
  }
  else {
	if (!Instance->stateMap.test(StateFlag::SELBOX)) {
	  ClosestPointIndex = 0;
	  if (Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
		ClosestPointIndex = findFirstStitch(ClosestFormToCursor);
		Instance->stateMap.set(StateFlag::SELBOX);
		Instance->stateMap.reset(StateFlag::FRMPSEL);
	  }
	}
  }
  displayText::butxt(HUPTO, displayText::loadStr(IDS_UPON));
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::toglHid() {
  if (Instance->stateMap.testAndFlip(StateFlag::HID)) {
	Instance->stateMap.reset(StateFlag::FRMOF);
  }
  else {
	Instance->stateMap.set(StateFlag::FRMOF);
  }
  unthum();
  redraw(Instance->buttonWin.operator[](HHID));
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::unlin() {
  if (Instance->stateMap.test(StateFlag::WASLIN)) {
	dulin(MoveLine0, MoveLine1);
  }
}

void thred::movbox() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  if (auto stitchCoordsInPixels = POINT {}; stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	unbox();
#ifdef _DEBUG
	auto const& stitch = Instance->stitchBuffer.operator[](ClosestPointIndex);
	outDebugString(L"movbox:Stitch [{}] form [{}] type [{}] x [{}] y[{}]\n",
	               ClosestPointIndex,
	               (stitch.attribute & FRMSK) >> FRMSHFT,
	               (stitch.attribute & TYPMSK) >> TYPSHFT,
	               stitch.x,
	               stitch.y);
#endif
	dubox(stitchCoordsInPixels);
	if (Instance->stateMap.test(StateFlag::UPTO)) {
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
  }
  else {
	shft2box();
	Instance->stateMap.set(StateFlag::SELBOX);
	Instance->stateMap.reset(StateFlag::FRMPSEL);
	Instance->stateMap.set(StateFlag::RESTCH);
  }
  nuAct(ClosestPointIndex);
  ritcor(Instance->stitchBuffer.operator[](ClosestPointIndex));
}

/*
This function attempts to find the stitch the user is trying to select.
A rectangle is created that is slightly larger than the stitch.
If the mouse position is outside this rectangle, the stitch is disqualified.
If the mouse position is inside the rectangle, the distance from the stitch
 to the select point (SelectedPoint) is calculated.

Find the equation for the line by solving the linear parametric equations using
stitch = Instance->stitchBuffer.operator[](StitchIndex);
stitchFwd = Instance->stitchBuffer.operator[](StitchIndex + 1);

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

auto thred::closlin() -> uint32_t {
  // ToDo - This function needs to be thoroughly checked and compared to findDistanceToSide
  auto           closestPoint = BIGUINT;
  constexpr auto TLFACTOR     = 20.0F; // tolerance ratio
  unboxs();
  auto offset = wrap::toFloat(WinMsg.pt.x - StitchWindowAbsRect.left) /
                wrap::toFloat(StitchWindowAbsRect.right - StitchWindowAbsRect.left);
  auto const offsetX = (offset * (ZoomRect.right - ZoomRect.left)) + ZoomRect.left;

  offset = wrap::toFloat(StitchWindowAbsRect.bottom - WinMsg.pt.y) /
           wrap::toFloat(StitchWindowAbsRect.bottom - StitchWindowAbsRect.top);
  auto const offsetY = (offset * (ZoomRect.top - ZoomRect.bottom)) + ZoomRect.bottom;
  offset         = (ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right);
  auto       sum = BIGFLOAT;
  auto const tolerance = offset * TLFACTOR;
  for (auto iChange = size_t {}; iChange < maxColor(); ++iChange) {
	auto& colorChangeTable = ThrSingle->ColorChangeTable;
	auto  stitchCount =
	    gsl::narrow_cast<ptrdiff_t>(std::abs(colorChangeTable.operator[](iChange + 1U).stitchIndex -
	                                         colorChangeTable.operator[](iChange).stitchIndex));
	if (colorChangeTable.operator[](iChange + 1U).stitchIndex == Instance->stitchBuffer.size()) {
	  --stitchCount;
	}
	auto const stitches =
	    wrap::next(Instance->stitchBuffer.begin(), colorChangeTable.operator[](iChange).stitchIndex);
	if (!chkhid(iChange)) {
	  continue;
	}
	for (auto iStitch = ptrdiff_t {}; iStitch < stitchCount; ++iStitch) {
	  if (auto const layer = (stitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
	      ActiveLayer != 0U && layer != 0U && layer != ActiveLayer) {
		continue;
	  }
	  auto const xba = std::lround(stitches[iStitch + 1].x - stitches[iStitch].x);
	  auto const yab = std::lround(stitches[iStitch].y - stitches[iStitch + 1].y);
	  auto left   = xba > 0 ? stitches[iStitch].x - tolerance : stitches[iStitch + 1].x - tolerance;
	  auto right  = xba > 0 ? stitches[iStitch + 1].x + tolerance : stitches[iStitch].x + tolerance;
	  auto bottom = yab < 0 ? stitches[iStitch].y - tolerance : stitches[iStitch + 1].y - tolerance;
	  auto top    = yab < 0 ? stitches[iStitch + 1].y + tolerance : stitches[iStitch].y + tolerance;
	  if (offsetX <= left || offsetX >= right || offsetY <= bottom || offsetY >= top) {
		continue;
	  }
	  // ReSharper disable once CppInitializedValueIsAlwaysRewritten
	  auto tsum         = 0.0F;
	  auto intersection = F_POINT {};
	  auto deltaX       = 0.0F;
	  auto deltaY       = 0.0F;
	  while (true) {
		if (yab == 0) {
		  // stitch is horizontal
		  intersection = F_POINT {offsetX, stitches[iStitch].y};
		}
		else {
		  if (xba == 0) {
			// stitch is vertical
			deltaX = stitches[iStitch].x - offsetX;
			top -= tolerance;
			bottom += tolerance;
			if (offsetY > top) {
			  deltaY = offsetY - top;
			  tsum   = deltaX * deltaX + deltaY * deltaY;
			  break;
			}
			if (offsetY < bottom) {
			  deltaY = offsetY - bottom;
			  tsum   = deltaX * deltaX + deltaY * deltaY;
			  break;
			}
			tsum = deltaX * deltaX;
			break;
		  }
		  auto const slope = wrap::toFloat(xba) / wrap::toFloat(yab);
		  offset           = stitches[iStitch].x + slope * stitches[iStitch].y;
		  auto const poff  = offsetX - (offsetY / slope);

		  intersection = F_POINT {offset - (slope * intersection.y),
		                          slope * (offset - poff) / (slope * slope + 1.0F)};

		  deltaX = intersection.x - offsetX;
		  deltaY = intersection.y - offsetY;
		}
		top -= tolerance;
		bottom += tolerance;
		left += tolerance;
		right -= tolerance;
		if (intersection.x < left) {
		  if (intersection.y < bottom) {
			deltaX = offsetX - left;
			deltaY = offsetY - bottom;
		  }
		  else {
			deltaX = offsetX - left;
			deltaY = offsetY - top;
		  }
		}
		else {
		  if (intersection.x > right) {
			if (intersection.y < bottom) {
			  deltaX = offsetX - right;
			  deltaY = offsetY - bottom;
			}
			else {
			  deltaX = offsetX - right;
			  deltaY = offsetY - top;
			}
		  }
		}
		tsum = deltaX * deltaX + deltaY * deltaY;
		break;
	  }
	  if (tsum < sum) {
		sum          = tsum;
		closestPoint = wrap::toUnsigned(iStitch) + colorChangeTable.operator[](iChange).stitchIndex;
	  }
	}
  }
  if (sum > tolerance * tolerance) {
	return BIGUINT;
  }
  return closestPoint;
}

void thred::selCol() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  // NOLINTBEGIN(readability-avoid-nested-conditional-operator)
  auto iStitch = Instance->stateMap.test(StateFlag::SELBOX)   ? ClosestPointIndex
                 : Instance->stateMap.test(StateFlag::GRPSEL) ? GroupStitchIndex
                                                              : 0;
  // NOLINTEND(readability-avoid-nested-conditional-operator)
  iStitch             = std::min(iStitch, wrap::toUnsigned(Instance->stitchBuffer.size() - 1U));
  GroupStitchIndex    = iStitch;
  ClosestPointIndex   = iStitch;
  auto const stitchIt = wrap::next(Instance->stitchBuffer.begin(), iStitch);
  auto const color    = gsl::narrow_cast<uint8_t>(stitchIt->attribute & COLMSK);
  while (ClosestPointIndex != 0U &&
         gsl::narrow_cast<uint8_t>(Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & COLMSK) == color) {
	--ClosestPointIndex;
  }
  if ((Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & COLMSK) != color) {
	++ClosestPointIndex;
  }
  while (GroupStitchIndex < wrap::toUnsigned(Instance->stitchBuffer.size() - 1U) &&
         (Instance->stitchBuffer.operator[](GroupStitchIndex).attribute & COLMSK) == color) {
	++GroupStitchIndex;
  }
  if ((Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & COLMSK) != color) {
	--ClosestPointIndex;
  }
  GroupStitchIndex = std::min(GroupStitchIndex, wrap::toUnsigned(Instance->stitchBuffer.size() - 1U));
  Instance->stateMap.set(StateFlag::GRPSEL);
  unbox();
  grpAdj();
  ActiveColor = color;
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::newFil() {
  Instance->stateMap.reset(StateFlag::CMPDO);
  bitmap::resetBmpFile(true);
  backup::deldu();
  auto& designerName = ThrSingle->DesignerName;
  designerName.assign(utf::utf8ToUtf16(std::string(IniFile.designerName.data())));
  auto const fmtStr = displayText::format(IDS_THRED, designerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
  ThrSingle->ThrName = ThrSingle->DefaultDirectory / displayText::loadStr(IDS_NUFIL).c_str();
  ritfnam(designerName);
  auto const designer       = utf::utf16ToUtf8(designerName);
  auto const spModifierName = gsl::span {ExtendedHeader.modifierName};
  std::ranges::copy(designer, spModifierName.begin());
  spModifierName[designer.length()] = 0;
  rstdu();
  rstAll();
  displayText::clrhbut(3);
  Instance->stateMap.reset(StateFlag::MOVSET);
  unbox();
  xlin();
  xlin1();
  Instance->stateMap.reset(StateFlag::INIT);
  Instance->stateMap.reset(StateFlag::INSRT);
  Instance->stateMap.reset(StateFlag::LIN1);
  Instance->stateMap.reset(StateFlag::FORMSEL);
  Instance->stateMap.reset(StateFlag::BAKACT);
  Instance->stateMap.reset(StateFlag::GMRK);
  Instance->stitchBuffer.clear();
  Instance->stitchBuffer.shrink_to_fit();
  DisplayedColorBitmap.reset();
  bitmap::resetBmpFile(true);
  Instance->formVertices.clear();
  Instance->formVertices.shrink_to_fit();
  Instance->texturePointsBuffer.clear();
  Instance->texturePointsBuffer.shrink_to_fit();
  Instance->clipPoints.clear();
  Instance->clipPoints.shrink_to_fit();
  Instance->satinGuides.clear();
  Instance->satinGuides.shrink_to_fit();
  auto& formList = Instance->formList;

  formList.clear();
  formList.shrink_to_fit();
  resetColorChanges();
  ThrSingle->Knots.clear();
  Instance->workingFileName.clear();
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	redraw(ThrSingle->DefaultColorWin.operator[](iColor));
	redraw(ThrSingle->UserColorWin.operator[](iColor));
	redraw(ThrSingle->ThreadSizeWin.operator[](iColor));
  }
  zumhom();
}

void thred::bBox() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::polyline(StitchWindowDC, ZoomBoxLine.data(), wrap::toUnsigned(ZoomBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unbBox() {
  if (Instance->stateMap.testAndReset(StateFlag::BZUM)) {
	bBox();
  }
}

void thred::rebox() {
  unbox();
  if (!closPnt1(ClosestPointIndex)) {
	return;
  }
  nuAct(ClosestPointIndex);
  if (auto stitchCoordsInPixels = POINT {}; stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	dubox(stitchCoordsInPixels);
	outDebugString(L"rebox:Stitch [{}] form [{}] type [{}]\n",
	               ClosestPointIndex,
	               (Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & FRMSK) >> FRMSHFT,
	               (Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & TYPMSK) >> TYPSHFT);
  }
  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL)) {
	Instance->stateMap.reset(StateFlag::SCROS);
	Instance->stateMap.reset(StateFlag::ECROS);
	Instance->searchLine.clear();
	Instance->searchLine.shrink_to_fit();
	Instance->stateMap.set(StateFlag::RESTCH);
	for (auto const& window : ThrSingle->UserColorWin) {
	  redraw(window);
	}
  }
  ritcor(Instance->stitchBuffer.operator[](ClosestPointIndex));
}

void thred::delstchm() {
  rngadj();
  auto const start = wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch);
  auto const end   = wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch + 1U);
  Instance->stitchBuffer.erase(start, end);
}

void thred::unclp() {
  if (Instance->stateMap.testAndReset(StateFlag::CLPSHO)) {
	duclp();
  }
}

void thred::clpbox() {
  auto const ratio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  auto       stitchPoint = pxCor2stch(WinMsg.pt);
  if (auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx); stitchPoint.x + ClipRectSize.cx > unzoomedX) {
	stitchPoint.x = unzoomedX - ClipRectSize.cx;
  }
  if (auto const unzoomedY = wrap::toFloat(UnzoomedRect.cy); stitchPoint.y + ClipRectSize.cy > unzoomedY) {
	stitchPoint.y = unzoomedY - ClipRectSize.cy;
  }
  tfc::setClipOrigin({std::lround(stitchPoint.x), std::lround(stitchPoint.y)});
  auto const adjustedSize =
      SIZE {wrap::ceil<int32_t>(ClipRectSize.cx * ratio), wrap::ceil<int32_t>(ClipRectSize.cy * ratio)};
  auto const stitchCoordsInPixels = POINT {
      wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ratio),
      wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                          ((stitchPoint.y - ZoomRect.bottom) * ratio) - wrap::toFloat(adjustedSize.cy))};
  ClipInsertBoxLine[0].x = ClipInsertBoxLine[3].x = ClipInsertBoxLine[4].x = stitchCoordsInPixels.x;
  ClipInsertBoxLine[0].y = ClipInsertBoxLine[1].y = ClipInsertBoxLine[4].y = stitchCoordsInPixels.y;
  ClipInsertBoxLine[1].x = ClipInsertBoxLine[2].x = ClipInsertBoxLine[0].x + adjustedSize.cx;
  ClipInsertBoxLine[2].y = ClipInsertBoxLine[3].y = ClipInsertBoxLine[0].y + adjustedSize.cy;
  Instance->stateMap.set(StateFlag::CLPSHO);
  duclp();
}

void thred::rSelbox() {
  auto const ratio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  auto const adjustedSelectSize =
      SIZE {gsl::narrow_cast<int32_t>(lround(wrap::toFloat(SelectBoxSize.cx) * ratio)),
            gsl::narrow_cast<int32_t>(lround(wrap::toFloat(SelectBoxSize.cy) * ratio))};
  unsel();
  auto stitchPoint = pxCor2stch(WinMsg.pt);
  if (stitchPoint.x - wrap::toFloat(SelectBoxOffset.x + SelectBoxSize.cx) >=
      wrap::toFloat(UnzoomedRect.cx)) {
	stitchPoint.x = wrap::toFloat(UnzoomedRect.cx - SelectBoxSize.cx + SelectBoxOffset.x);
  }
  if (stitchPoint.y - wrap::toFloat(SelectBoxOffset.y + SelectBoxSize.cy) >=
      wrap::toFloat(UnzoomedRect.cy)) {
	stitchPoint.y = wrap::toFloat(UnzoomedRect.cy - SelectBoxSize.cy + SelectBoxOffset.y);
  }
  if (stitchPoint.x - wrap::toFloat(SelectBoxOffset.x) < 0.0F) {
	stitchPoint.x = wrap::toFloat(SelectBoxOffset.x);
  }
  if (stitchPoint.y - wrap::toFloat(SelectBoxOffset.y) < 0.0F) {
	stitchPoint.y = wrap::toFloat(SelectBoxOffset.y);
  }
  auto const stitchCoordsInPixels = POINT {
      wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left - wrap::toFloat(SelectBoxOffset.x)) * ratio),
      wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                          ((stitchPoint.y - ZoomRect.bottom - wrap::toFloat(SelectBoxOffset.y)) * ratio) -
                          wrap::toFloat(adjustedSelectSize.cy))};
  auto& frmCtrls  = ThrSingle->FormControlPoints;
  frmCtrls[PTL].x = frmCtrls[PBL].x = frmCtrls[PLM].x = frmCtrls[PTLE].x = stitchCoordsInPixels.x;
  frmCtrls[PTL].y = frmCtrls[PTM].y = frmCtrls[PTR].y = frmCtrls[PTLE].y = stitchCoordsInPixels.y;
  frmCtrls[PBR].y = frmCtrls[PBM].y = frmCtrls[PBL].y = stitchCoordsInPixels.y + adjustedSelectSize.cy;
  frmCtrls[PTR].x = frmCtrls[PRM].x = frmCtrls[PBR].x = stitchCoordsInPixels.x + adjustedSelectSize.cx;
  frmCtrls[PTM].x = frmCtrls[PBM].x = stitchCoordsInPixels.x + adjustedSelectSize.cx / 2;
  frmCtrls[PRM].y = frmCtrls[PLM].y = stitchCoordsInPixels.y + adjustedSelectSize.cy / 2;
  Instance->stateMap.set(StateFlag::SELSHO);
  dusel(StitchWindowDC);
}

void thred::duSelbox() noexcept {
  auto const stitchPoint = pxCor2stch(WinMsg.pt);

  SelectBoxSize = {.cx = std::lround(StitchRangeRect.right - StitchRangeRect.left),
                   .cy = std::lround(StitchRangeRect.top - StitchRangeRect.bottom)};

  SelectBoxOffset = {.x = std::lround(stitchPoint.x - StitchRangeRect.left),
                     .y = std::lround(stitchPoint.y - StitchRangeRect.bottom)};
}

void thred::drawCapturedStitchBox() {
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  SelectObject(StitchWindowMemDC, LinePen);
  if (ClosestPointIndex != 0U) {
	stchbox(ClosestPointIndex - 1, StitchWindowDC);
  }
  stchbox(ClosestPointIndex, StitchWindowDC);
  if (ClosestPointIndex < wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)) {
	stchbox(ClosestPointIndex + 1, StitchWindowDC);
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::sCor2px(F_POINT const& stitchCoordinate, POINT& pixelCoordinate) {
  pixelCoordinate = {.x = wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                     .y = wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) +
                                              ((ZoomRect.bottom - stitchCoordinate.y) * ZoomRatio.y))};
}

void thred::unrot() {
  if (Instance->stateMap.testAndReset(StateFlag::ROTSHO)) {
	durot();
  }
}

void thred::durotu() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::polyline(
      StitchWindowDC, RotateBoxToCursorLine.data(), wrap::toUnsigned(RotateBoxToCursorLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unrotu() {
  if (Instance->stateMap.testAndReset(StateFlag::ROTUSHO)) {
	durotu();
  }
}

auto thred::rotangf(F_POINT const& unrotatedPoint, float const rotationAngle, F_POINT const& rotationCenter) noexcept
    -> F_POINT {
  auto const deltaX           = unrotatedPoint.x - rotationCenter.x;
  auto const deltaY           = unrotatedPoint.y - rotationCenter.y;
  auto const distanceToCenter = deltaX != 0.0F ? std::hypot(deltaX, deltaY) : std::abs(deltaY);
  auto       newAngle         = rotationAngle;
  if (deltaX != 0.0F) {
	newAngle += std::atan2(deltaY, deltaX);
  }
  else {
	if (deltaY > 0.0F) {
	  newAngle += PI_FHALF;
	}
	else {
	  newAngle -= PI_FHALF;
	}
  }
  return F_POINT {rotationCenter.x + (distanceToCenter * cos(newAngle)),
                  rotationCenter.y + (distanceToCenter * sin(newAngle))};
}

auto thred::rotang1(F_POINT_ATTR const& unrotatedPoint, float const rotationAngle, F_POINT const& rotationCenter) noexcept
    -> F_POINT {
  auto const point = F_POINT {unrotatedPoint.x, unrotatedPoint.y};
  return rotangf(point, rotationAngle, rotationCenter);
}

void thred::rotflt(F_POINT& point, float const rotationAngle, F_POINT const& rotationCenter) noexcept {
  point = rotangf(point, rotationAngle, rotationCenter);
}

void thred::ritrot(float rotationAngle, F_POINT const& rotationCenter) {
  if (Instance->stateMap.test(StateFlag::MOVCNTR)) { // set rotation angle to 0 if we are moving the center point
	rotationAngle = 0.0F;
  }
  auto rotated           = POINT {};
  auto rotationReference = F_POINT {RotationRect.left, RotationRect.top};
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
  sCor2px(rotationCenter, RotateBoxToCursorLine[0]);
  Instance->stateMap.set(StateFlag::ROTSHO);
  durot();
}

void thred::rot(F_POINT& rotationCenter) {
  if (!Instance->stateMap.test(StateFlag::FORMSEL) && Instance->selectedFormList.empty() &&
      !Instance->stateMap.test(StateFlag::BIGBOX) && !Instance->stateMap.test(StateFlag::GRPSEL) &&
      !Instance->stateMap.test(StateFlag::FPSEL)) {
	// no rotatable selections found
	displayText::shoseln(IDS_FGRPF, IDS_ROT);
	return;
  }
  Instance->stateMap.set(StateFlag::ROTAT);
  rotationCenter = form::rotpar();
  ritrot(0, rotationCenter);
}

void thred::frmdel() {
  f1del(ClosestFormToCursor);
  auto& formList = Instance->formList;

  auto const firstForm = formList.cbegin();
  formList.erase(wrap::next(firstForm, ClosestFormToCursor));
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  if (Instance->stateMap.testAndReset(StateFlag::DELTO)) {
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & NOTFRM) != 0U) {
		continue;
	  }
	  if (auto const stitchForm = stitch.attribute & FRMSK; stitchForm > codedForm) {
		stitch.attribute &= NFRMSK;
		stitch.attribute |= stitchForm - (1U << FRMSHFT);
	  }
	}
  }
  else {
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & NOTFRM) != 0U) {
		continue;
	  }
	  auto const stitchForm = stitch.attribute & FRMSK;
	  if (stitchForm == codedForm) {
		stitch.attribute &= NFRMSK & NTYPMSK;
		stitch.attribute |= NOTFRM;
		continue;
	  }
	  if (stitchForm > codedForm) {
		stitch.attribute &= NFRMSK;
		stitch.attribute |= stitchForm - (1U << FRMSHFT);
	  }
	}
  }
  Instance->stateMap.reset(StateFlag::FORMSEL);
  if (ClosestFormToCursor > formList.size() - 1U) {
	ClosestFormToCursor = 0U;
  }
}

void thred::delsfrms() {
  auto& formList = Instance->formList;

  if (formList.empty()) {
	return;
  }
  auto formIndices = std::vector<uint32_t> {};
  formIndices.resize(formList.size());
  auto formMap = boost::dynamic_bitset(formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  for (auto const selectedForm : Instance->selectedFormList) {
	formMap.set(selectedForm);
	f1del(selectedForm);
  }
  auto       deletedFormCount = 0U;
  auto       firstForm        = formList.cbegin();
  auto const formCount        = formList.size();
  for (auto iForm = 0U; iForm < formCount; ++iForm) {
	if (formMap.test(iForm)) {
	  formList.erase(wrap::next(firstForm, iForm - deletedFormCount));
	  firstForm = formList.cbegin(); // the erase invalidates firstForm
	  ++deletedFormCount;
	  continue;
	}
	formIndices[iForm] = (iForm - deletedFormCount) << FRMSHFT;
  }
  if (Instance->stateMap.test(StateFlag::DELTO)) {
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) == 0U) {
		continue;
	  }
	  if (auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT; !formMap.test(iForm)) {
		stitch.attribute &= NFRMSK;
		stitch.attribute |= formIndices[iForm];
	  }
	}
  }
  else {
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & NOTFRM) != 0U) {
		continue;
	  }
	  if (auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT; formMap.test(iForm)) {
		stitch.attribute &= NFRMSK & NTYPMSK;
		stitch.attribute |= NOTFRM;
	  }
	  else {
		stitch.attribute &= NFRMSK;
		stitch.attribute |= formIndices[iForm];
	  }
	}
  }
  Instance->selectedFormList.clear();
  Instance->stateMap.reset(StateFlag::FORMSEL);
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::cut() {
  savdo();
  tfc::duclip();
  if (!Instance->selectedFormList.empty()) {
	Instance->stateMap.set(StateFlag::DELTO);
	delsfrms();
  }
  else {
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  delfstchs();
	  frmdel();
	}
	else {
	  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
		delstchm();
	  }
	}
  }
  coltab();
  rstAll();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::stretch() {
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  formStretch(selectedForm);
	}
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	formStretch(ClosestFormToCursor);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	rngadj();
	doStretch(GroupStartStitch, GroupEndStitch);
	return;
  }
  doStretch(0, wrap::toUnsigned(Instance->stitchBuffer.size()));
}

void thred::contract() {
  savdo();
}

void thred::unpat() {
  if (!Instance->stateMap.testAndReset(StateFlag::WASPAT)) {
	return;
  }
  ShowWindow(SpeedScrollBar, SW_HIDE);
  DestroyWindow(SpeedScrollBar);
  movStch();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::delknot() {
  // Find the first knot stitch, if any
  auto const firstStitch = std::ranges::find_if(Instance->stitchBuffer, [](F_POINT_ATTR const& stitch) -> bool {
	return (stitch.attribute & KNOTMSK) != 0U;
  });
  if (firstStitch == Instance->stitchBuffer.end()) {
	return;
  }
  savdo();
  // delete the knot stitches
  Instance->stitchBuffer.erase(std::remove_if(firstStitch,
                                              Instance->stitchBuffer.end(),
                                              [](F_POINT_ATTR const& stitch) -> bool {
	                                            return (stitch.attribute & KNOTMSK) != 0U;
                                              }),
                               Instance->stitchBuffer.end());
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::setknots() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  savdo();
  delknt();
  setknt();
  fndknt();
  chkncol();
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
  ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
}

void thred::hidbit() {
  menu::flipHideBitmap();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::deltot() {
  ThrSingle->DesignerName.assign(utf::utf8ToUtf16(std::string(IniFile.designerName.data())));
  Instance->texturePointsBuffer.clear();
  Instance->formList.clear();
  Instance->stitchBuffer.clear();
  Instance->formVertices.clear();
  Instance->satinGuides.clear();
  Instance->stateMap.reset(StateFlag::GMRK);
  rstAll();
  coltab();
  zumhom();
  auto const wTxt = displayText::format2(IDS_THRDBY, ThrSingle->ThrName.wstring(), ThrSingle->DesignerName);
  SetWindowText(ThrEdWindow, wTxt.c_str());
}

void thred::delet() {
  undat();
  auto& formList = Instance->formList;

  if (Instance->stateMap.testAndReset(StateFlag::FPSEL)) {
	savdo();
	auto& form = formList.operator[](ClosestFormToCursor);
	// dynamic bitset allows non-contiguous ranges of points to be deleted in later versions
	// ReSharper disable once CppTemplateArgumentsCanBeDeduced
	auto vertexMap         = boost::dynamic_bitset<>(form.vertexCount);
	auto currentFormVertex = SelectedFormVertices.start;
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  vertexMap.set(currentFormVertex);
	  currentFormVertex = form::pdir(form, currentFormVertex);
	}
	auto const vBegin   = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	auto       vCurr    = vBegin; // intentional copy
	auto       itVertex = vBegin; // intentional copy
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  if (!vertexMap.test(iVertex)) {
		*vCurr = *itVertex;
		++vCurr;
	  }
	  ++itVertex;
	}
	auto const eraseCount = wrap::toUnsigned(std::distance(vBegin, vCurr));
	auto const eraseStart =
	    wrap::next(Instance->formVertices.cbegin(), wrap::toSize(form.vertexIndex) + eraseCount);
	auto const eraseEnd = wrap::next(eraseStart, form.vertexCount - eraseCount);
	Instance->formVertices.erase(eraseStart, eraseEnd); // This invalidates iterators
	auto const nextForm = wrap::toSize(ClosestFormToCursor) + 1U;
	for (auto iForm = wrap::next(formList.begin(), nextForm); iForm < formList.end(); ++iForm) {
	  iForm->vertexIndex -= SelectedFormVertices.vertexCount + 1U;
	}
	form.vertexCount -= SelectedFormVertices.vertexCount + 1U;
	form.outline();
	if (form.type == SAT) {
	  // Make sure the end guides are still valid
	  if (vertexMap.test(0) || vertexMap.test(1)) {
		form.wordParam = 0; // Satin end guide
		form.attribute &= NFRMEND;
	  }
	  if (auto const iNext = form.wordParam + 1U; vertexMap.test(form.wordParam) || vertexMap.test(iNext)) {
		form.wordParam = 0;
	  }

	  if (form.satinGuideCount == 0U) {
		// ToDo - Is there a better way to do this than iterating through?
		auto itGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex);
		for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		  auto newGuideVal = 0U;
		  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
			if (vertexMap.test(iVertex)) {
			  if (itGuide->finish == iVertex) {
				itGuide->finish = itGuide->start;
			  }
			  if (itGuide->start == iVertex) {
				itGuide->start = itGuide->finish;
			  }
			}
			else {
			  if (itGuide->finish == iVertex) {
				itGuide->finish = newGuideVal;
			  }
			  if (itGuide->start == iVertex) {
				itGuide->start = newGuideVal;
			  }
			  ++newGuideVal;
			}
		  }
		  ++itGuide;
		}
		satin::satadj(form);
	  }
	}
	form::refil(ClosestFormToCursor);
	fndknt();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_SHIFT)) {
	deltot();
	fndknt();
	return;
  }
  savdo();
  auto satinFlag = false;
  if (!Instance->selectedFormList.empty()) {
	if (frmstch()) {
	  Instance->stateMap.set(StateFlag::DELSFRMS);
	  displayText::tabmsg(IDS_DELFRM, false);
	  displayText::okcan();
	  displayText::tomsg();
	}
	else {
	  delsfrms();
	}
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL) && !formList.empty()) {
	if (wastch(ClosestFormToCursor)) {
	  Instance->stateMap.set(StateFlag::DELFRM);
	  displayText::tabmsg(IDS_FDEL, false);
	  displayText::okcan();
	  displayText::tomsg();
	}
	else {
	  Instance->stateMap.reset(StateFlag::DELTO);
	  frmdel();
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
	fndknt();
	return;
  }
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	if (Instance->stitchBuffer.size() > 2U) {
	  delstch1(ClosestPointIndex);
	  if (!stch2px2(ClosestPointIndex)) {
		movbox();
	  }
	}
	else {
	  Instance->stitchBuffer.clear();
	  Instance->stateMap.reset(StateFlag::SELBOX);
	}
	coltab();
	xt::setfchk();
	fndknt();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	delstchm();
	coltab();
	rstAll();
	xt::setfchk();
	fndknt();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FRMPSEL) || form::closfrm(ClosestFormToCursor)) {
	auto& form = formList.operator[](ClosestFormToCursor);
	switch (form.type) {
	  case FRMLINE: {
		if (handleDeleteLineForm(form)) {
		  return;
		}
		break;
	  }
	  case SAT: {
		handleDeleteSatinForm(form, satinFlag);
		break;
	  }
	  default: {
		outDebugString(L"default hit in delet: type [{}]\n", form.type);
		break;
	  }
	}
	if (!satinFlag) {
	  satin::delspnt();
	}
	if (ClosestFormToCursor > formList.size() - 1U) {
	  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
	}
	form.outline();
	form::refil(ClosestFormToCursor);
	coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
  if (!satinFlag && closPnt1(ClosestPointIndex)) {
	if (Instance->stitchBuffer.size() > 2U) {
	  delstch1(ClosestPointIndex);
	  if (!stch2px2(ClosestPointIndex)) {
		movbox();
	  }
	}
	else {
	  Instance->stitchBuffer.clear();
	  Instance->stateMap.reset(StateFlag::SELBOX);
	}
	coltab();
	xt::setfchk();
	fndknt();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  fndknt();
}

void thred::movi() {
  constexpr auto MOVITIM = int32_t {12}; // default movie time
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	rstAll();
	Instance->stateMap.set(StateFlag::GRPSEL);
  }
  else {
	rstAll();
  }
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  if (MsgWindow != nullptr) {
	DestroyWindow(MsgWindow);
	MsgWindow = nullptr;
  }
  Instance->stateMap.set(StateFlag::RUNPAT);
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	rngadj();
	RunPoint = GroupStartStitch;
  }
  else {
	RunPoint = 0;
  }
  movStch();
  if (!Instance->stateMap.test(StateFlag::WASPAT)) {
	SpeedScrollBar = CreateWindow(L"SCROLLBAR",
	                              nullptr,
	                              SBS_HORZ | WS_CHILD | WS_VISIBLE,
	                              ButtonWidthX3,
	                              0,
	                              StitchWindowSize.cx,
	                              ScrollSize,
	                              ThrEdWindow,
	                              nullptr,
	                              ThrEdInstance,
	                              nullptr);
  }
  auto const stepCount = Instance->stateMap.test(StateFlag::ZUMED)
                             ? gsl::narrow_cast<float>(Instance->stitchBuffer.size()) * ZoomFactor * ZoomFactor
                             : gsl::narrow_cast<float>(Instance->stitchBuffer.size());
  if (!Instance->stateMap.test(StateFlag::WASPAT)) {
	constexpr auto MLEN = 10000.0F; // default movie length (milliseconds)
	MovieTimeStep       = wrap::round<decltype(MovieTimeStep)>(MLEN * MOVITIM / stepCount);
  }
  MovieTimeStep     = std::clamp(MovieTimeStep, MINDELAY, MAXDELAY);
  auto scrollInfo   = SCROLLINFO {}; // scroll bar i/o structure
  scrollInfo.cbSize = sizeof(scrollInfo);
  scrollInfo.fMask  = SIF_ALL;
  scrollInfo.nMax   = MAXDELAY;
  scrollInfo.nMin   = MINDELAY;
  scrollInfo.nPage  = 1;
  scrollInfo.nPos   = MAXDELAY - MovieTimeStep;
  SetScrollInfo(SpeedScrollBar, SB_CTL, &scrollInfo, TRUE);
  FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
  setsped();
}

void thred::redclp() {
  auto const codedLayer  = gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT);
  auto*      clipPointer = GlobalLock(ClipMemory);
  if (clipPointer == nullptr) {
	return;
  }
  auto const* const clipStitchPtr    = gsl::narrow_cast<CLIP_STITCH const*>(clipPointer);
  auto const        clipSize         = clipStitchPtr->led;
  auto const        spClipStitchData = gsl::span {clipStitchPtr, clipSize};

  auto& clipBuffer = Instance->clipBuffer;

  clipBuffer.clear();
  clipBuffer.reserve(clipSize);
  clipBuffer.emplace_back(wrap::toFloat(clipStitchPtr->x) + (wrap::toFloat(clipStitchPtr->fx) / FRACFACT),
                          wrap::toFloat(clipStitchPtr->y) + (wrap::toFloat(clipStitchPtr->fy) / FRACFACT),
                          0U);

#if CLPBUG
  OutputDebugString(format(FMT_COMPILE(L"redclp:interator [0] x [{:6.2F}] y [{:6.2F}]\n"),
                           clipBuffer.back().x,
                           clipBuffer.back().y)
                        .c_str());
#endif
  auto clipRect = F_RECTANGLE {
      clipBuffer.front().x, clipBuffer.front().y, clipBuffer.front().x, clipBuffer.front().y};
  auto iCSD = std::next(spClipStitchData.begin());
  for (auto iStitch = 1U; iStitch < clipSize; ++iStitch) {
	clipBuffer.emplace_back(wrap::toFloat(iCSD->x) + (wrap::toFloat(iCSD->fx) / FRACFACT),
	                        wrap::toFloat(iCSD->y) + (wrap::toFloat(iCSD->fy) / FRACFACT),
	                        (iCSD->led & COLMSK) | codedLayer);

#if CLPBUG
	OutputDebugString(format(FMT_COMPILE(L"redclp:interator [{}] x [{:6.2F}] y [{:6.2F}]\n"),
	                         iStitch,
	                         clipBuffer.back().x,
	                         clipBuffer.back().y)
	                      .c_str());
#endif
	clipRect.left   = std::min(clipBuffer.back().x, clipRect.left);
	clipRect.right  = std::max(clipBuffer.back().x, clipRect.right);
	clipRect.bottom = std::min(clipBuffer.back().y, clipRect.bottom);
	clipRect.top    = std::max(clipBuffer.back().y, clipRect.top);
	++iCSD;
  }
  clipBuffer.front().attribute = ActiveColor | codedLayer;
  ClipRectSize = {.cx = clipRect.right - clipRect.left, .cy = clipRect.top - clipRect.bottom};
  GlobalUnlock(ClipMemory);
  if (clipRect.left != 0.0F || clipRect.bottom != 0.0F) {
	for (auto& clip : clipBuffer) {
	  clip.x -= clipRect.left;
	  clip.y -= clipRect.bottom;
	}
  }
}

void thred::vubak() {
  if (Instance->workingFileName.empty() && !Instance->stateMap.test(StateFlag::THUMSHO)) {
	return;
  }
  Instance->stateMap.set(StateFlag::ZUMED);
  movStch();
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
  auto const deltaX   = StitchWindowClientRect.right / 2;
  auto const deltaY   = StitchWindowClientRect.bottom / 2;
  auto       itHWndBV = BackupViewer.begin();
  for (auto iPosition = uint32_t {}; iPosition < QUADRT; ++iPosition) { // there are 4 quadrants
	auto verticalLocation = 0L;
	if ((iPosition & 2U) != 0U) {
	  verticalLocation = deltaY;
	}
	*itHWndBV = CreateWindow(L"STATIC",
	                         L"",
	                         SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                         (deltaX * gsl::narrow_cast<int32_t>(iPosition & 1U)) + ButtonWidthX3,
	                         verticalLocation,
	                         deltaX,
	                         deltaY,
	                         ThrEdWindow,
	                         nullptr,
	                         ThrEdInstance,
	                         nullptr);
	++itHWndBV;
  }
  Instance->stateMap.set(StateFlag::BAKSHO);
}

void thred::insflin(POINT const insertPoint) {
  auto const offset    = POINT {InsertSize.cx / 2, InsertSize.cy / 2};
  auto&      formLines = Instance->formLines;
  formLines.resize(SQPNTS);
  formLines[0].x = formLines[3].x = formLines[4].x = insertPoint.x - offset.x;
  formLines[1].x = formLines[2].x = insertPoint.x + offset.x;
  formLines[0].y = formLines[1].y = formLines[4].y = insertPoint.y - offset.y;
  formLines[2].y = formLines[3].y = insertPoint.y + offset.y;
}

void thred::insfil(fs::path& insertedFile) {
  auto successFlag       = false;
  auto insertedRectangle = F_RECTANGLE {BIGFLOAT, LOWFLOAT, LOWFLOAT, BIGFLOAT};
  if (insertedFile.empty()) {
	getNewFileName(insertedFile, FileStyles::INS_FILES, FileIndices::THR);
  }
  InsertedStitchIndex = wrap::toUnsigned(Instance->stitchBuffer.size());
  if (isthr(insertedFile)) {
	if (insTHR(insertedFile, insertedRectangle)) {
	  successFlag = true;
	}
  }
  if (PCS::isPCS(insertedFile)) {
	if (PCS::insPCS(insertedFile, insertedRectangle)) {
	  // We did not insert forms so ensure that duinsfil does not move forms
	  InsertedFormIndex   = wrap::toUnsigned(Instance->formList.size());
	  InsertedVertexIndex = wrap::toUnsigned(Instance->formVertices.size());
	  successFlag         = true;
	}
  }
  if (!successFlag) {
	return;
  }
  InsertCenter = F_POINT {wrap::midl(insertedRectangle.right, insertedRectangle.left),
                          wrap::midl(insertedRectangle.top, insertedRectangle.bottom)};

  auto const insertedSize = F_POINT {insertedRectangle.right - insertedRectangle.left,
                                     insertedRectangle.top - insertedRectangle.bottom};
  ratsr();
  InsertSize.cx = std::lround(insertedSize.x * HorizontalRatio);
  // ToDo - Should this be vertical ratio?
  InsertSize.cy = std::lround(insertedSize.y * HorizontalRatio);
  auto const initialInsertPoint = POINT {StitchWindowClientRect.right / 2, StitchWindowClientRect.bottom / 2};
  insflin(initialInsertPoint);
  NewFormVertexCount = 5U; // inserted file is a rectangle NOLINT(readability-magic-numbers)
  Instance->stateMap.set(StateFlag::SHOFRM);
  Instance->stateMap.set(StateFlag::INSFIL);
  form::dufrm();
}

void thred::thumbak() {
  destroyBV();
  getbak();
}

void thred::purg() {
  if (ThrSingle->ThrName.empty()) {
	return;
  }
  auto fileName = ThrSingle->ThrName; // intentional copy
  auto ext      = ThrSingle->ThrName.extension().wstring();
  for (auto iLast = wchar_t {}; iLast < OLDVER; ++iLast) {
	ext.back() = iLast + 's';
	fileName.replace_extension(ext);
	fs::remove(fileName);
  }
}

void thred::purgdir() {
  Instance->stateMap.set(StateFlag::PRGMSG);
  displayText::showMessage(IDS_DELBAK, ThrSingle->DefaultDirectory.wstring());
  displayText::okcan();
}

void thred::deldir() {
  unmsg();
  displayText::tabmsg(IDS_BAKDEL, false);
  auto backSpec = std::wstring {L".th0"};
  for (auto const& filePath : fs::directory_iterator(ThrSingle->DefaultDirectory)) {
	if (!is_directory(filePath)) {
	  auto fileExt    = filePath.path().extension().wstring();
	  backSpec.back() = 's';
	  for (auto iLastChar = wchar_t {}; iLastChar < OLDVER; ++iLastChar) {
		if (fileExt == backSpec) {
		  fs::remove(filePath.path());
		  ++backSpec.back();
		}
	  }
	}
  }
  unmsg();
  displayText::tabmsg(IDS_BAKDT, false);
}

// ReSharper disable CppParameterMayBeConst
auto thred::chkwnd(HWND window) noexcept -> bool {
  auto windowRect = RECT {};
  GetWindowRect(window, &windowRect);
  return WinMsg.pt.x >= windowRect.left && WinMsg.pt.x <= windowRect.right &&
         WinMsg.pt.y >= windowRect.top && WinMsg.pt.y <= windowRect.bottom;
}
// ReSharper restore CppParameterMayBeConst

void thred::mv2f() {
  if (Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
	savdo();
	std::vector<F_POINT_ATTR> tempStitchBuffer {};
	// since most of the stitches will be moved into the temp buffer, reserve the space
	tempStitchBuffer.reserve(Instance->stitchBuffer.size());
	auto       iLowBuffer = 0U;
	auto const attribute  = ClosestFormToCursor << FRMSHFT;
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & NOTFRM) == 0U && (stitch.attribute & FRMSK) == attribute) {
		Instance->stitchBuffer.operator[](iLowBuffer++) = stitch;
	  }
	  else {
		tempStitchBuffer.push_back(stitch);
	  }
	}
	std::ranges::copy(tempStitchBuffer, wrap::next(Instance->stitchBuffer.begin(), iLowBuffer));
	coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL)) {
	savdo();
	std::vector<F_POINT_ATTR> tempStitchBuffer {};
	auto const                grpSize = GroupEndStitch + 1U - GroupStartStitch;
	tempStitchBuffer.resize(Instance->stitchBuffer.size() - grpSize);
	rngadj();
	std::copy(Instance->stitchBuffer.begin(),
	          wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch + 1U),
	          Instance->stitchBuffer.end(),
	          wrap::next(tempStitchBuffer.begin(), GroupStartStitch));
	std::copy(wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch),
	          wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch + 1U),
	          Instance->stitchBuffer.begin());
	std::ranges::copy(tempStitchBuffer, wrap::next(Instance->stitchBuffer.begin(), grpSize));
	coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void thred::mv2b() {
  if (Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
	savdo();
	auto       tempStitchBuffer = std::vector<F_POINT_ATTR> {};
	auto       iLowBuffer       = 0U;
	auto const attribute        = ClosestFormToCursor << FRMSHFT;
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & NOTFRM) == 0U && (stitch.attribute & FRMSK) == attribute) {
		tempStitchBuffer.push_back(stitch);
	  }
	  else {
		Instance->stitchBuffer.operator[](iLowBuffer++) = stitch;
	  }
	}
	std::ranges::copy(tempStitchBuffer, wrap::next(Instance->stitchBuffer.begin(), iLowBuffer));
	coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL)) {
	savdo();
	rngadj();
	std::vector<F_POINT_ATTR> tempStitchBuffer {};
	auto const                grpSize = GroupEndStitch + 1U - GroupStartStitch;
	tempStitchBuffer.resize(grpSize);
	std::copy(wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch),
	          wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch + 1U),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch + 1U),
	          Instance->stitchBuffer.end(),
	          wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch));
	std::ranges::copy(tempStitchBuffer,
	                  wrap::next(Instance->stitchBuffer.begin(), Instance->stitchBuffer.size() - grpSize));
	coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void thred::delinf() noexcept {
  for (auto& iStitch : Instance->stitchBuffer) {
	infadj(iStitch.x, iStitch.y);
  }
  for (auto& formVertice : Instance->formVertices) {
	infadj(formVertice.x, formVertice.y);
  }
}

void thred::chkrng(F_POINT& range) {
  savdo();
  delinf();
  wrap::narrow(range.x, UnzoomedRect.cx);
  wrap::narrow(range.y, UnzoomedRect.cy);

  if (auto const& formList = Instance->formList; !formList.empty()) {
	// ToDo - Why do we treat the forms differently?
	auto iDestination = Instance->stitchBuffer.begin();
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & NOTFRM) != 0U ||
	      ((stitch.attribute & TYPMSK) != 0U && (stitch.attribute & FRMSK) >> FRMSHFT < formList.size())) {
		if (stitch.x > range.x) {
		  stitch.x = range.x - 1.0F;
		}
		stitch.x = std::max(stitch.x, 0.0F);
		if (stitch.y > range.y) {
		  stitch.y = range.y - 1.0F;
		}
		stitch.y        = std::max(stitch.y, 0.0F);
		*iDestination++ = stitch;
	  }
	}
	Instance->stitchBuffer.resize(wrap::toSize(std::distance(Instance->stitchBuffer.begin(), iDestination)));
	coltab();
	return;
  }
  for (auto& stitch : Instance->stitchBuffer) {
	if (stitch.x > range.x) {
	  stitch.x = range.x - 1.0F;
	}
	stitch.x = std::max(stitch.x, 0.0F);
	if (stitch.y > range.y) {
	  stitch.y = range.y - 1.0F;
	}
	stitch.y = std::max(stitch.y, 0.0F);
  }
}

void thred::ritmov(uint32_t const formIndex) noexcept {
  auto const& form = Instance->formList.operator[](formIndex);
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  if (ClosestVertexToCursor != 0U) {
	if (ClosestVertexToCursor == form.vertexCount - 1U && form.type == FRMLINE) {
	  Polyline(StitchWindowDC, Instance->rubberBandLine.data(), 2);
	}
	else {
	  Polyline(StitchWindowDC, Instance->rubberBandLine.data(), 3);
	}
  }
  else {
	Instance->rubberBandLine.operator[](2) = Instance->formLines.operator[](1);
	if (form.type == FRMLINE) {
	  Polyline(StitchWindowDC, std::addressof(Instance->rubberBandLine.operator[](1)), 2);
	}
	else {
	  Polyline(StitchWindowDC, Instance->rubberBandLine.data(), 3);
	}
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::setpsel() {
  form::unpsel();
  auto const& form = Instance->formList.operator[](ClosestFormToCursor);
  duprct(form);
  SelectedPixelsRect = form::sRct2px(SelectedVerticesRect);
  form::rct2sel(SelectedPixelsRect, Instance->selectedPointsLine);
  auto const itVertex =
      wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + SelectedFormVertices.finish);
  EndPointCross = form::sfCor2px(*itVertex);
  Instance->stateMap.set(StateFlag::SHOPSEL);
  form::dupsel(StitchWindowDC);
  Instance->stateMap.set(StateFlag::FPSEL);
}

void thred::rotfn(float const rotationAngle, F_POINT const& rotationCenter) {
  savdo();
  auto& formList = Instance->formList;

  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	// clang-format off
	auto&       form          = formList.operator[](ClosestFormToCursor);
	auto        currentVertex = SelectedFormVertices.start;
	auto  const vBegin        = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	// clang-format on
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto itVertex = wrap::next(vBegin, currentVertex);
	  rotflt(*itVertex, rotationAngle, rotationCenter);
	  currentVertex = form::pdir(form, currentVertex);
	}
	form.outline();
	setpsel();
	form::refil(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	for (auto& formVertice : Instance->formVertices) {
	  rotflt(formVertice, rotationAngle, rotationCenter);
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  rotstch(stitch, rotationAngle, rotationCenter);
	}
	for (auto& iForm : formList) {
	  iForm.outline();
	}
	form::selal();
	return;
  }
  if (Instance->stateMap.testAndReset(StateFlag::FRMSROT)) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  // clang-format off
	  auto& form     = formList.operator[](selectedForm);
	  auto  itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		rotflt(*itVertex, rotationAngle, rotationCenter);
		++itVertex;
	  }
	  form.outline();
	  form::refilfn(selectedForm);
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.testAndReset(StateFlag::FRMROT)) {
	auto& form = formList.operator[](ClosestFormToCursor);

	auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  rotflt(*itVertex, rotationAngle, rotationCenter);
	  ++itVertex;
	}
	form.outline();
	form::refil(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	rotstch(Instance->stitchBuffer.operator[](iStitch), rotationAngle, rotationCenter);
  }
  rngadj();
  selin(GroupStartStitch, GroupEndStitch, StitchWindowDC);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::showOnlyLayer(uint8_t const play) {
  ActiveLayer = play;
  menu::ladj();
  if (ActiveLayer == 0U) {
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL) &&
      gsl::narrow_cast<decltype(ActiveLayer)>(Instance->formList.operator[](ClosestFormToCursor).attribute & FRMLMSK) >> 1U !=
          ActiveLayer) {
	Instance->stateMap.reset(StateFlag::FORMSEL);
  }
  Instance->stateMap.reset(StateFlag::GRPSEL);
  if (Instance->stateMap.test(StateFlag::SELBOX) &&
      ActiveLayer != ((Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & LAYMSK) >> LAYSHFT) + 1U) {
	Instance->stateMap.reset(StateFlag::SELBOX);
  }
  Instance->selectedFormList.clear();
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto thred::iselpnt() noexcept -> bool {
  auto const pointToTest =
      POINT {(WinMsg.pt.x - StitchWindowOrigin.x), (WinMsg.pt.y - StitchWindowOrigin.y)};
  auto closestControlPoint = 0U;
  auto minimumLength       = BIGDBL;
  auto iControlPoint       = 0U;
  for (auto const& controlPoint : ThrSingle->FormControlPoints) {
	auto const deltaX = pointToTest.x - controlPoint.x;
	auto const deltaY = pointToTest.y - controlPoint.y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
	  minimumLength       = length;
	  closestControlPoint = iControlPoint;
	}
	++iControlPoint;
  }
  if (std::sqrt(minimumLength) < CLOSENUF) {
	SelectedFormControlVertex = closestControlPoint;
	return true;
  }
  return false;
}

void thred::strtchbox(std::vector<POINT> const& stretchBoxLine) noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, stretchBoxLine.data(), wrap::toUnsigned(stretchBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unstrtch(std::vector<POINT> const& stretchBoxLine) {
  if (Instance->stateMap.testAndReset(StateFlag::SHOSTRTCH)) {
	strtchbox(stretchBoxLine);
  }
}

// Does the outline have zero width or zero height?
auto thred::isLine(std::vector<POINT> const& boxOutline) noexcept -> bool {
  return boxOutline[0].x == boxOutline[1].x || boxOutline[1].y == boxOutline[2].y;
}

auto thred::chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  auto       minLength = BIGDBL;
  auto const pointToTest =
      POINT {(WinMsg.pt.x - StitchWindowOrigin.x), (WinMsg.pt.y - StitchWindowOrigin.y)};
  auto controlPoint = Instance->selectedFormsLine.begin();

  auto const endPoint = Instance->selectedFormsLine.size();
  for (auto iControlPoint = 0U; iControlPoint < endPoint; ++iControlPoint) {
	auto const deltaX = pointToTest.x - controlPoint->x;
	auto const deltaY = pointToTest.y - controlPoint->y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minLength) {
	  minLength                 = length;
	  SelectedFormControlVertex = iControlPoint;
	}
	++controlPoint;
  }
  auto const minimumLength = std::sqrt(minLength);

  auto& formLines = Instance->formLines;
  formLines.resize(SQPNTS);
  for (auto iCorner = 0U; iCorner < 4; ++iCorner) {
	formLines[iCorner] = Instance->selectedFormsLine.operator[](wrap::toSize(iCorner) * 2U);
  }
  formLines[4] = formLines[0];
  if (minimumLength < CLOSENUF && !isLine(formLines)) {
	for (auto iCorner = 0U; iCorner < 4; ++iCorner) {
	  stretchBoxLine[iCorner] = Instance->selectedFormsLine.operator[](wrap::toSize(iCorner) * 2U);
	}
	stretchBoxLine[4] = stretchBoxLine[0];
	strtchbox(stretchBoxLine);
	if ((SelectedFormControlVertex & 1U) != 0U) {
	  Instance->stateMap.set(StateFlag::STRTCH);
	}
	else {
	  Instance->stateMap.set(StateFlag::EXPAND);
	  xyRatio = wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left) /
	            wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
	}
	SelectedFormControlVertex >>= 1U;
	Instance->stateMap.set(StateFlag::SHOSTRTCH);
	return true;
  }
  SelectedFormControlVertex >>= 1U;
  if (isInBox(pointToTest, SelectedFormsRect) || (minimumLength < CLOSENUF && isLine(formLines))) {
	SelectedFormsSize = F_POINT {SelectedFormsRect.right - SelectedFormsRect.left,
	                             SelectedFormsRect.bottom - SelectedFormsRect.top};
	Instance->stateMap.set(StateFlag::MOVFRMS);
	FormMoveDelta =
	    F_POINT {pointToTest.x - SelectedFormsRect.left, pointToTest.y - SelectedFormsRect.top};
	Instance->stateMap.set(StateFlag::SHOSTRTCH);
	strtchbox(stretchBoxLine);
	return true;
  }
  return false;
}

void thred::delfre() {
  auto currentStitchCount = 0U;
  savdo();
  // ToDo - this loop does not delete all free stitches. look at frmdel as well
  for (auto const& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & NOTFRM) == 0U) {
	  Instance->stitchBuffer.operator[](currentStitchCount++) = stitch;
	}
  }
  Instance->stitchBuffer.resize(currentStitchCount);
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::colchk() noexcept(std::is_same_v<size_t, uint32_t>) {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  auto color       = Instance->stitchBuffer.front().attribute & COLMSK;
  auto startStitch = 0U;
  // ToDo - Can this loop become a ranged for?
  for (auto iStitch = 0U; iStitch < wrap::toUnsigned(Instance->stitchBuffer.size()); ++iStitch) {
	auto const& stitch = Instance->stitchBuffer.operator[](iStitch);
	if (color == (stitch.attribute & COLMSK)) {
	  continue;
	}
	if (iStitch - startStitch == 1 && startStitch != 0U) {
	  auto const prevIndex  = iStitch - 1U;
	  auto&      prevStitch = Instance->stitchBuffer.operator[](prevIndex);
	  prevStitch.attribute  = (stitch.attribute & NCOLMSK) | (prevStitch.attribute & COLMSK);
	}
	color       = stitch.attribute & COLMSK;
	startStitch = iStitch;
  }
}

void thred::rembig() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  if (UserStitchLength >= IniFile.maxStitchLength) {
	displayText::tabmsg(IDS_REM1, false);
	return;
  }
  savdo();
  while (true) {
	if (!Instance->selectedFormList.empty()) {
	  auto range = RANGE {};
	  for (auto const selectedForm : Instance->selectedFormList) {
		if (form::frmrng(selectedForm, range)) {
		  makbig(range.start, range.finish);
		}
	  }
	  break;
	}
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  if (auto range = RANGE {}; form::frmrng(ClosestFormToCursor, range)) {
		makbig(range.start, range.finish);
	  }
	  break;
	}
	if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	  rngadj();
	  if (GroupEndStitch < wrap::toUnsigned(Instance->stitchBuffer.size())) {
		++GroupEndStitch;
	  }
	  if (ClosestPointIndex < GroupStitchIndex) {
		GroupStitchIndex += makbig(GroupStartStitch, GroupEndStitch);
	  }
	  else {
		ClosestPointIndex += makbig(GroupStartStitch, GroupEndStitch);
	  }
	  grpAdj();
	  break;
	}
	makbig(0, wrap::toUnsigned(Instance->stitchBuffer.size()));
	break;
  }
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::nextSortedStitch(bool const direction) {
  if (direction) {
	if (ClosestPointIndex == LargestStitchIndex) {
	  return;
	}
	++SortIndex;
  }
  else {
	if (ClosestPointIndex == SmallestStitchIndex) {
	  return;
	}
	--SortIndex;
  }
  auto const nextStitch = wrap::next(ThrSingle->SortBuffer.begin(), SortIndex);
  outDebugString(L"SortIndex [{}]\n", SortIndex);
  CurrentStitchIndex   = nextStitch->index;
  auto const minLength = std::sqrt(nextStitch->length) * IPFGRAN;
  displayText::butxt(HMINLEN, format(FMT_COMPILE(L"{:.2f}"), minLength));
  lensadj();
  displayText::ritnum(IDS_NUMSCH, ClosestPointIndex);
}

auto thred::inrng(uint32_t const iStitch) noexcept -> bool {
  if (iStitch >= Instance->stitchBuffer.size()) {
	return false;
  }
  auto const& stitch = Instance->stitchBuffer.operator[](iStitch);
  return stitch.x >= StitchRangeRect.left && stitch.x <= StitchRangeRect.right &&
         stitch.y >= StitchRangeRect.bottom && stitch.y <= StitchRangeRect.top;
}

void thred::thumnail() {
  auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
  unbsho();
  undat();
  trace::untrace();
  current_path(ThrSingle->DefaultDirectory);
  auto const searchName = ThrSingle->DefaultDirectory / L"*.thr";
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const file = FindFirstFile(searchName.wstring().c_str(), &fileData);
  if (file == INVALID_HANDLE_VALUE) {
	auto const dwError = GetLastError();
	auto const fmtStr  = displayText::format2(IDS_FFINDERR, searchName.wstring(), dwError);
	displayText::shoMsg(fmtStr, true);
	unthum();
	return;
  }
  auto& thumbnails = ThrSingle->Thumbnails;
  thumbnails.clear();
  auto& fileName = fileData.cFileName;
  thumbnails.emplace_back(std::begin(fileName));
  while (FindNextFile(file, &fileData)) {
	thumbnails.emplace_back(std::begin(fileName));
  }
  FindClose(file);
  std::ranges::sort(thumbnails);
  auto       iThumbnail = 0U;
  auto const thumbSize  = thumbnails.size();
  while (iThumbnail < 4 && iThumbnail < thumbSize) {
	ThumbnailsSelected.at(iThumbnail) = iThumbnail;
	++iThumbnail;
  }
  ThumbnailIndex = ThumbnailDisplayCount = iThumbnail;
  while (iThumbnail < 4 && iThumbnail < thumbSize) {
	rthumnam(iThumbnail++);
  }
  Instance->stateMap.set(StateFlag::THUMSHO);
  ThrSingle->ThumbnailSearchString.clear();
  ThrSingle->ThumbnailSearchString.push_back(0);
  SetWindowText(Instance->buttonWin.operator[](HBOXSEL), L"");
  auto const blank = std::wstring {};
  displayText::butxt(HBOXSEL, blank);
  vubak();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::nuthsel() {
  auto const& thumbnails = ThrSingle->Thumbnails;
  if (ThumbnailIndex >= thumbnails.size()) {
	return;
  }
  auto const savedIndex = ThumbnailIndex;
  auto       iThumbnail = uint32_t {};
  auto const length     = wcslen(ThrSingle->ThumbnailSearchString.data());
  Instance->stateMap.set(StateFlag::RESTCH);
  if (length != 0U) {
	auto itHWndBV = BackupViewer.begin();
	while (iThumbnail < QUADRT && ThumbnailIndex < thumbnails.size()) { // there are 4 quadrants
	  if (_wcsnicmp(ThrSingle->ThumbnailSearchString.data(), thumbnails.operator[](ThumbnailIndex).data(), length) == 0) {
		ThumbnailsSelected.at(iThumbnail) = ThumbnailIndex;
		redraw(*itHWndBV);
		++itHWndBV;
		++iThumbnail;
	  }
	  ++ThumbnailIndex;
	}
  }
  else {
	auto itHWndBV = BackupViewer.begin();
	while (iThumbnail < QUADRT && ThumbnailIndex < thumbnails.size()) { // there are 4 quadrants
	  ThumbnailsSelected.at(iThumbnail) = ThumbnailIndex;
	  redraw(*itHWndBV);
	  ++itHWndBV;
	  ++iThumbnail;
	  ++ThumbnailIndex;
	}
  }
  if (iThumbnail != 0U) {
	ThumbnailDisplayCount = iThumbnail;
	while (iThumbnail < OLDVER) {
	  rthumnam(iThumbnail++);
	}
  }
  else {
	ThumbnailIndex = savedIndex;
  }
}

void thred::nuthbak(uint32_t count) {
  if (ThumbnailIndex == 0U) {
	return;
  }
  constexpr auto MAXFORMS = uint32_t {1024U}; // maximum number of forms
  if (auto const length = wcslen(ThrSingle->ThumbnailSearchString.data()); length != 0U) {
	while (count != 0U && ThumbnailIndex < MAXFORMS) {
	  if (ThumbnailIndex != 0U) {
		--ThumbnailIndex;
		if (_wcsnicmp(ThrSingle->ThumbnailSearchString.data(),
		              ThrSingle->Thumbnails.operator[](ThumbnailIndex).data(),
		              length) == 0) {
		  --count;
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

void thred::thumbBack() {
  auto& thumbnailSearchString = ThrSingle->ThumbnailSearchString;
  if (thumbnailSearchString.size() <= 1) {
	return;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
  thumbnailSearchString.pop_back();
  thumbnailSearchString.back() = 0;
  ThumbnailIndex               = 0;
  auto const txt               = std::wstring(thumbnailSearchString.data());
  displayText::butxt(HBOXSEL, txt);
  nuthsel();
}

void thred::selalstch() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  ClosestPointIndex = 0;
  GroupStitchIndex  = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
  GroupStartStitch  = ClosestPointIndex;
  GroupEndStitch    = GroupStitchIndex;
  Instance->stateMap.set(StateFlag::GRPSEL);
  Instance->stateMap.set(StateFlag::SCROS);
  Instance->stateMap.set(StateFlag::ECROS);
  grpAdj();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::duinsfil() {
  auto& formList = Instance->formList;

  auto const stitchPoint = pxCor2stch(WinMsg.pt);
  auto const offset      = F_POINT {stitchPoint.x - InsertCenter.x, stitchPoint.y - InsertCenter.y};
  for (auto iForm = InsertedFormIndex; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
	auto& formRectangle = formList.operator[](iForm).rectangle;
	formRectangle.bottom += offset.y;
	formRectangle.top += offset.y;
	formRectangle.left += offset.x;
	formRectangle.right += offset.x;
  }
  for (auto iVertex = InsertedVertexIndex; iVertex < wrap::toUnsigned(Instance->formVertices.size()); ++iVertex) {
	Instance->formVertices.operator[](iVertex) += offset;
  }
  for (auto iStitch = InsertedStitchIndex; iStitch < wrap::toUnsigned(Instance->stitchBuffer.size()); ++iStitch) {
	Instance->stitchBuffer.operator[](iStitch) += offset;
  }
  Instance->stateMap.reset(StateFlag::FRMOF);
  Instance->stateMap.set(StateFlag::INIT);
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::gotbox() {
  Instance->stateMap.reset(StateFlag::BZUMIN);
  Instance->stateMap.reset(StateFlag::BOXSLCT);
  Instance->stateMap.reset(StateFlag::FRMPSEL);
  grpAdj();
}

void thred::nucols() {
  auto& formList = Instance->formList;

  auto formMap = boost::dynamic_bitset(formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  for (auto const selectedForm : Instance->selectedFormList) {
	formMap.set(selectedForm);
	auto& form = formList.operator[](selectedForm);
	if (form.fillType != 0U) {
	  form.fillColor = ActiveColor;
	  if (form.fillType == FTHF) {
		form.feather.color = ActiveColor;
	  }
	}
	if (form.edgeType != 0U) {
	  form.borderColor = ActiveColor;
	}
	if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	  form.underlayColor = ActiveColor;
	}
  }
  for (auto& stitch : Instance->stitchBuffer) {
	if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT) && (stitch.attribute & TYPMSK) != TYPMSK) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= ActiveColor;
	}
  }
}

void thred::stchrct(F_RECTANGLE& rectangle) noexcept {
  if (Instance->stitchBuffer.empty()) {
	rectangle = F_RECTANGLE {};
	return;
  }
  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;

  for (auto const& stitch : Instance->stitchBuffer) {
	minX = std::min(minX, stitch.x);
	maxX = std::max(maxX, stitch.x);
	minY = std::min(minY, stitch.y);
	maxY = std::max(maxY, stitch.y);
  }

  rectangle = F_RECTANGLE {minX, maxY, maxX, minY};
}

void thred::frmrct(F_RECTANGLE& rectangle) noexcept {
  if (Instance->formVertices.empty()) {
	rectangle = F_RECTANGLE {};
	return;
  }
  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;

  for (auto const& formVertice : Instance->formVertices) {
	minX = std::min(minX, formVertice.x);
	minY = std::min(minY, formVertice.y);
	maxX = std::max(maxX, formVertice.x);
	maxY = std::max(maxY, formVertice.y);
  }

  rectangle = F_RECTANGLE {minX, maxY, maxX, minY};
}

void thred::fop() {
  trace::untrace();
  if (!Instance->formList.empty() || !Instance->stitchBuffer.empty()) {
	if (savcmp()) {
	  nuFil(FileIndices::THR);
	  showOnlyLayer(0U);
	}
	else {
	  displayText::savdisc();
	  Instance->stateMap.set(StateFlag::OSAV);
	}
  }
  else {
	nuFil(FileIndices::THR);
	showOnlyLayer(0U);
  }
}

void thred::clpadj() {
  if (!Instance->stateMap.test(StateFlag::GRPSEL)) {
	displayText::shoseln(IDS_GRPMSG, IDS_RNGEND);
	return;
  }
  rngadj();
  auto iStitch          = GroupStartStitch;
  auto clipRectAdjusted = F_RECTANGLE {Instance->stitchBuffer.operator[](iStitch).x,
                                       Instance->stitchBuffer.operator[](wrap::toSize(iStitch) + 1U).y,
                                       Instance->stitchBuffer.operator[](iStitch).x,
                                       Instance->stitchBuffer.operator[](wrap::toSize(iStitch) + 1U).y};
  ++iStitch;
  while (iStitch < GroupEndStitch) {
	clpradj(clipRectAdjusted, Instance->stitchBuffer.operator[](iStitch++));
  }
  clipRectAdjusted.left = std::min(Instance->stitchBuffer.operator[](iStitch).x, clipRectAdjusted.left);
  clipRectAdjusted.right = std::max(Instance->stitchBuffer.operator[](iStitch).x, clipRectAdjusted.right);
  auto const clipMiddle = wrap::midl(clipRectAdjusted.right, clipRectAdjusted.left);
  Instance->stitchBuffer.operator[](GroupStartStitch).y =
      wrap::midl(clipRectAdjusted.top, clipRectAdjusted.bottom);
  Instance->stitchBuffer.operator[](GroupEndStitch).y =
      Instance->stitchBuffer.operator[](GroupStartStitch).y;
  if (Instance->stitchBuffer.operator[](GroupStartStitch).x < clipMiddle) {
	Instance->stitchBuffer.operator[](GroupStartStitch).x = clipRectAdjusted.left;
	Instance->stitchBuffer.operator[](GroupEndStitch).x   = clipRectAdjusted.right;
  }
  else {
	Instance->stitchBuffer.operator[](GroupEndStitch).x   = clipRectAdjusted.left;
	Instance->stitchBuffer.operator[](GroupStartStitch).x = clipRectAdjusted.right;
  }
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::shftflt(F_POINT const& point) noexcept {
  if (point.x <= ZoomRect.left || point.x >= ZoomRect.right || point.y <= ZoomRect.bottom ||
      point.y >= ZoomRect.top) {
	shft(point);
  }
}

void thred::defpref() {
  constexpr auto APSPAC  = 10.8F;         // applique border spacing
  constexpr auto DEFBPIX = uint16_t {4U}; // default form box pixels
  constexpr auto DEFANG  = 0.7853981F;    // default fill angle, 45 degrees

  Instance->userFlagMap.reset();
  // NOLINTBEGIN(readability-magic-numbers)
  UserColor = {0x00000000,
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

  CustomColor = {0x00729674,
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

  CustomBackgroundColor = {0x00a3c5dc,
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
  // NOLINTEND(readability-magic-numbers)

  bitmap::setBmpBackColor();
  IniFile.dazdef();
  Instance->userFlagMap.set(UserFlag::DAZHOL);
  Instance->userFlagMap.set(UserFlag::DAZD);
  AppliqueColor          = COLORCNT - 1U;
  IniFile.AppStitchLen   = APSPAC;
  BorderWidth            = BRDWID;
  ButtonholeCornerLength = IBFCLEN;
  IniFile.chainSpace     = CHSDEF;
  IniFile.chainRatio     = CHRDEF;
  IniFile.fillAngle      = DEFANG;
  Instance->userFlagMap.reset(UserFlag::SQRFIL);
  LineSpacing             = DEFSPACE * PFGRAN;
  ShowStitchThreshold     = SHOPNTS;
  IniFile.gridSize        = 2.0F * PFGRAN; // 2mm default spacing NOLINT(readability-magic-numbers)
  IniFile.hoopType        = LARGHUP;
  IniFile.hoopSizeX       = LHUPX;
  IniFile.hoopSizeY       = LHUPY;
  IniFile.cursorNudgeStep = NUGINI;
  IniFile.nudgePixels     = DEFPIX;
  Instance->userFlagMap.set(UserFlag::BLUNT);
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
  bitmap::setBitmapColor(0xc8dfee); // light cornflower blue NOLINT(readability-magic-numbers)
  if (IniFile.customHoopX == 0.0F) {
	IniFile.customHoopX = LHUPX;
  }
  if (IniFile.customHoopY == 0.0F) {
	IniFile.customHoopY = LHUPY;
  }
  PicotSpacing = IPICSPAC;
  Instance->userFlagMap.set(UserFlag::FIL2OF);
  menu::fil2men();
  BackgroundColor = 0xa8c4b1; // lighter Dark Sea green NOLINT(readability-magic-numbers)
  UnzoomedRect.cx = std::lround(IniFile.hoopSizeX);
  UnzoomedRect.cy = std::lround(IniFile.hoopSizeY);
  IniFile.defWav();
  IniFile.defFeather();
  IniFile.underlayStitchLen = DEFULEN;
  IniFile.underlaySpacing   = DEFUSPAC;
  IniFile.formBoxSizePixels = DEFBPIX;
  IniFile.textureHeight     = ITXHI;
  IniFile.textureWidth      = ITXWID;
  IniFile.textureSpacing    = ITXSPAC;
  IniFile.lensRatio         = DEFLRAT;
}

void thred::dumrk(float const xCoord, float const yCoord) {
  if (Instance->stateMap.testAndReset(StateFlag::GMRK)) {
	drwmrk(StitchWindowDC);
  }
  ZoomMarkPoint = F_POINT {xCoord, yCoord};
  Instance->stateMap.set(StateFlag::INIT);
  Instance->stateMap.set(StateFlag::GMRK);
  drwmrk(StitchWindowDC);
  Instance->stateMap.set(StateFlag::WASMRK);
}

void thred::rotmrk() {
  if (!Instance->stateMap.test(StateFlag::GMRK) ||
      !(Instance->stateMap.test(StateFlag::FORMSEL) || Instance->stateMap.test(StateFlag::GRPSEL))) {
	displayText::shoseln(IDS_FSZ, IDS_SETROTM);
	return;
  }
  auto lowestAngle  = 0.0F;
  auto highestAngle = 0.0F;
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto const codedFormIndex = ClosestFormToCursor << FRMSHFT;
	// clang-format off
	  auto const& form     = Instance->formList.operator[](ClosestFormToCursor);
	  auto        itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	// clang-format on
	auto const originalAngle = std::atan2(itVertex->y - ZoomMarkPoint.y, itVertex->x - ZoomMarkPoint.x);
	++itVertex;
	for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	  angdif(lowestAngle,
	         highestAngle,
	         nuang(originalAngle, itVertex->x - ZoomMarkPoint.x, itVertex->y - ZoomMarkPoint.y));
	  ++itVertex;
	}
	for (auto const& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & FRMSK) == codedFormIndex) {
		angdif(lowestAngle,
		       highestAngle,
		       nuang(originalAngle, stitch.x - ZoomMarkPoint.x, stitch.y - ZoomMarkPoint.y));
	  }
	}
  }
  else {
	rngadj();
	auto const originalAngle =
	    std::atan2(Instance->stitchBuffer.operator[](GroupStartStitch).y - ZoomMarkPoint.y,
	               Instance->stitchBuffer.operator[](GroupStartStitch).x - ZoomMarkPoint.x);
	for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
	  angdif(lowestAngle,
	         highestAngle,
	         nuang(originalAngle,
	               Instance->stitchBuffer.operator[](iStitch).x - ZoomMarkPoint.x,
	               Instance->stitchBuffer.operator[](iStitch).y - ZoomMarkPoint.y));
	}
  }
  auto const tAngle     = highestAngle - lowestAngle;
  auto const segments   = std::round(PI_F2 / tAngle);
  IniFile.rotationAngle = PI_F2 / segments;
  auto const fmtStr = displayText::format2(IDS_ROTMARK, IniFile.rotationAngle * RADDEGF, segments);
  displayText::shoMsg(fmtStr, false);
}

void thred::rotseg() {
  segentr(IniFile.rotationAngle);
  Instance->stateMap.set(StateFlag::ENTRSEG);
}

void thred::pntmrk() {
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	dumrk(Instance->stitchBuffer.operator[](ClosestPointIndex).x,
	      Instance->stitchBuffer.operator[](ClosestPointIndex).y);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FRMPSEL)) {
	auto const itVertex =
	    wrap::next(Instance->formVertices.cbegin(),
	               Instance->formList.operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	dumrk(itVertex->x, itVertex->y);
	return;
  }
  displayText::shoseln(IDS_STCH_FRM, IDS_SETMRK);
}

void thred::filfrms() {
  if (Instance->selectedFormList.empty()) {
	if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	  return;
	}
	savdo();
	form::refil(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  savdo();
  for (auto const selectedForm : Instance->selectedFormList) {
	form::refilfn(selectedForm);
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::nuslst(uint32_t const find) {
  gselrng();
  // ToDo - Check this code. Does it do what is intended?
  if (find < SelectedFormsRange.start) {
	for (auto form = find; form < SelectedFormsRange.finish; ++form) {
	  Instance->selectedFormList.push_back(form);
	}
	return;
  }
  if (find > SelectedFormsRange.finish) {
	for (auto form = SelectedFormsRange.start; form <= find; ++form) {
	  Instance->selectedFormList.push_back(form);
	}
	return;
  }
  for (auto form = SelectedFormsRange.start; form <= find; ++form) {
	Instance->selectedFormList.push_back(form);
  }
}

void thred::tglhid() {
  if (!Instance->stateMap.testAndFlip(StateFlag::HIDSTCH)) {
	Instance->stateMap.reset(StateFlag::FRMOF);
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::retrac() {
  if (!Instance->stateMap.test(StateFlag::GRPSEL)) {
	displayText::shoseln(IDS_GRPMSG, IDS_RETRAC);
	return;
  }
  savdo();
  rngadj();
  if (GroupStartStitch == 0U) {
	++GroupStartStitch;
  }
  auto const count       = GroupEndStitch - GroupStartStitch;
  auto const insertPoint = wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch + 1U);
  auto const startPoint =
      wrap::next(Instance->stitchBuffer.rbegin(), Instance->stitchBuffer.size() - GroupEndStitch);
  auto const endPoint = wrap::next(startPoint, count);
  Instance->stitchBuffer.insert(insertPoint, startPoint, endPoint);
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::setgrd(COLORREF const color) {
  menu::setGridCols(color);
  nuPen(GridPen, 1, color);
  IniFile.gridColor = color;
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::ovrlay() {
  auto fileName = fs::path {};
  insfil(fileName);
  Instance->stateMap.reset(StateFlag::INSFIL);
  Instance->stateMap.reset(StateFlag::FRMOF);
  Instance->stateMap.set(StateFlag::INIT);
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::gsnap() {
  if (Instance->stateMap.test(StateFlag::TXTRED)) {
	texture::txsnap();
	return;
  }
  auto& formList = Instance->formList;

  if (Instance->selectedFormList.empty()) {
	if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	  if (!Instance->stateMap.test(StateFlag::GRPSEL)) {
		displayText::shoseln(IDS_FGRPF, IDS_SNAP2GRD);
		return;
	  }
	  savdo();
	  rngadj();
	  stchsnap(GroupStartStitch, GroupEndStitch + 1U);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}
	savdo();
	auto& formIter = formList.operator[](ClosestFormToCursor);
	frmsnap(formIter.vertexIndex, formIter.vertexCount);
	formIter.outline();
	form::refil(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  savdo();
  for (auto const selectedForm : Instance->selectedFormList) {
	ClosestFormToCursor = selectedForm;
	auto& formIter      = formList.operator[](ClosestFormToCursor);
	frmsnap(formIter.vertexIndex, formIter.vertexCount);
	formIter.outline();
	form::refil(ClosestFormToCursor);
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::fileLock() noexcept {
  auto lockInfo = std::vector<WIN32_FIND_DATA> {};
#pragma warning(suppress : 26490)
  DialogBoxParam(ThrEdInstance,
                 MAKEINTRESOURCE(IDD_DLOCK),
                 ThrEdWindow,
                 lockPrc,
                 reinterpret_cast<LPARAM>(&lockInfo)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}

void thred::delstch() {
  savdo();
  Instance->stitchBuffer.clear();
  Instance->stitchBuffer.shrink_to_fit();
  Instance->texturePointsBuffer.clear();
  Instance->texturePointsBuffer.shrink_to_fit();
  rstAll();
  form::clrfills();
  resetColorChanges();
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  displayText::butxt(HTOT, blank);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::closfn() {
  deltot();
  ThrSingle->Knots.clear();
  Instance->workingFileName.clear();
  bitmap::delmap();
  backup::deldu();
  displayText::clrhbut(3);
  auto const fmtStr = displayText::format(IDS_THRED, ThrSingle->DesignerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
}

void thred::filclos() {
  if (savcmp()) {
	closfn();
	return;
  }
  Instance->stateMap.set(StateFlag::FCLOS);
  displayText::savdisc();
}

void thred::nudgfn(float const deltaX, float const deltaY) {
  if (Instance->stateMap.test(StateFlag::BIGBOX) || !Instance->selectedFormList.empty() ||
      Instance->stateMap.test(StateFlag::FORMSEL) || Instance->stateMap.test(StateFlag::GRPSEL) ||
      Instance->stateMap.test(StateFlag::SELBOX)) {
	savdo();
  }
  auto& formList = Instance->formList;

  if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	for (auto& iForm : formList) {
	  frmpos(iForm, deltaX, deltaY);
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  stitch += F_POINT {deltaX, deltaY};
	}
	auto& allItemsRect = form::getAllItemsRect();
	allItemsRect.left += deltaX;
	allItemsRect.top += deltaY;
	allItemsRect.right += deltaX;
	allItemsRect.bottom += deltaY;
	form::stchrct2px(allItemsRect, SelectedFormsRect);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	auto formMap = boost::dynamic_bitset(formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
	for (auto const selectedForm : Instance->selectedFormList) {
	  formMap.set(selectedForm);
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
		stitch += F_POINT {deltaX, deltaY};
	  }
	}
	for (auto const selectedForm : Instance->selectedFormList) {
	  auto& form = formList.operator[](selectedForm);
	  frmpos(form, deltaX, deltaY);
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto& form = formList.operator[](ClosestFormToCursor);
	frmpos(form, deltaX, deltaY);
	if (form.fillType != 0U || form.edgeType != 0U) {
	  for (auto& stitch : Instance->stitchBuffer) {
		if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
		  stitch += F_POINT {deltaX, deltaY};
		}
	  }
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	rngadj();
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  Instance->stitchBuffer.operator[](iStitch) += F_POINT {deltaX, deltaY};
	}
	grpAdj();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	Instance->stitchBuffer.operator[](ClosestPointIndex) += F_POINT {deltaX, deltaY};
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  auto pixel = POINT {};
  if (deltaX != 0.0F) {
	pixel.x = deltaX > 0 ? IniFile.nudgePixels : -IniFile.nudgePixels;
  }
  if (deltaY != 0.0F) {
	pixel.y = deltaY > 0 ? -IniFile.nudgePixels : IniFile.nudgePixels;
  }
  // NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
  auto input       = INPUT {};
  input.type       = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_MOVE;
  input.mi.dx      = pixel.x;
  input.mi.dy      = pixel.y;
  SendInput(1, &input, sizeof(INPUT));
  // NOLINTEND(cppcoreguidelines-pro-type-union-access)
}

void thred::bakmrk() {
  if (!Instance->stateMap.test(StateFlag::WASMRK)) {
	displayText::tabmsg(IDS_MRK, false);
	return;
  }
  if (ZoomMarkPoint.x > IniFile.hoopSizeX) {
	ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
  }
  if (ZoomMarkPoint.y > IniFile.hoopSizeY) {
	ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
  }
  dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
  F_POINT const point(ZoomMarkPoint);
  shft(point);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::nuscol(size_t const iColor) {
  auto const itUserColor = wrap::next(UserColor.begin(), iColor);
  auto const itUserPen   = wrap::next(ThrSingle->UserPen.begin(), iColor);
  nuPen(*itUserPen, 1, *itUserColor);
  auto const ucb = wrap::next(UserColorBrush.begin(), iColor);
  nuBrush(*ucb, *itUserColor);
  redraw(ThrSingle->UserColorWin.operator[](iColor));
}

void thred::movchk() {
  static auto draggedColor = uint8_t {};
  if ((WinMsg.wParam & MK_LBUTTON) != 0U) {
	if (!Instance->stateMap.testAndSet(StateFlag::WASMOV)) {
	  if (inDefaultColorWindows()) {
		draggedColor = VerticalIndex & COLORMAX;
		Instance->stateMap.set(StateFlag::WASCOL);
	  }
	}
	return;
  }
  if (!Instance->stateMap.testAndReset(StateFlag::WASMOV) || !Instance->stateMap.testAndReset(StateFlag::WASCOL)) {
	return;
  }
  if (!inDefaultColorWindows() || draggedColor == VerticalIndex) {
	return;
  }
  savdo();
  auto const key          = wrap::pressed(VK_SHIFT);
  auto const switchColors = wrap::pressed(VK_CONTROL);
  for (auto& stitch : Instance->stitchBuffer) {
	if (auto const color = gsl::narrow_cast<uint8_t>(stitch.attribute & COLMSK); color == VerticalIndex) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= draggedColor;
	}
	else {
	  if (!key && color == draggedColor) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= VerticalIndex;
	  }
	}
  }
  for (auto& iForm : Instance->formList) {
	if (iForm.fillType != 0U) {
	  if (iForm.fillColor == VerticalIndex) {
		iForm.fillColor = draggedColor;
	  }
	  else {
		if (!key && iForm.fillColor == draggedColor) {
		  iForm.fillColor = VerticalIndex;
		}
	  }
	}
	if (iForm.fillType == FTHF) {
	  if (iForm.feather.color == VerticalIndex) {
		iForm.feather.color = draggedColor;
	  }
	  else {
		if (!key && iForm.feather.color == draggedColor) {
		  iForm.feather.color = VerticalIndex;
		}
	  }
	}
	if (iForm.edgeType != 0U) {
	  if (iForm.borderColor == VerticalIndex) {
		iForm.borderColor = draggedColor;
	  }
	  else {
		if (!key && iForm.borderColor == draggedColor) {
		  iForm.borderColor = VerticalIndex;
		}
	  }
	}
  }
  if (!switchColors) {
	auto const swapColor                      = UserColor.at(wrap::toSize(VerticalIndex));
	UserColor.at(wrap::toSize(VerticalIndex)) = UserColor.at(wrap::toSize(draggedColor));
	if (!key) {
	  UserColor.at(wrap::toSize(draggedColor)) = swapColor;
	  nuscol(draggedColor);
	}
	nuscol(VerticalIndex);
  }
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto thred::usedcol(uint8_t index) -> bool {
  return std::ranges::any_of(Instance->stitchBuffer, [&index](F_POINT_ATTR const& stitch) -> bool {
	return (stitch.attribute & COLMSK) == index;
  });
}

void thred::set1knot() {
  constexpr auto KNTSTCH = size_t {5U}; // number of stitches in knot
  if (Instance->stitchBuffer.empty() || !Instance->stateMap.test(StateFlag::SELBOX)) {
	displayText::tabmsg(IDS_NOSTCHSEL, false);
	return;
  }
  savdo();
  auto buffer = std::vector<F_POINT_ATTR> {};
  buffer.reserve(Instance->stitchBuffer.size() + KNTSTCH);
  buffer.insert(buffer.begin(),
                Instance->stitchBuffer.begin(),
                wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex + 1U));
  if (ClosestPointIndex == wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)) {
	Instance->stateMap.set(StateFlag::FILDIR);
	endknt(buffer, ClosestPointIndex);
  }
  else {
	strtknt(buffer, ClosestPointIndex);
  }
  buffer.insert(buffer.end(),
                wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex + 1U),
                Instance->stitchBuffer.end());
  Instance->stitchBuffer = std::move(buffer);
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::fixpclp(uint32_t const closestFormToCursor) {
  auto& interleaveSequence = Instance->interleaveSequence;

  auto const point = POINT {(WinMsg.pt.x + gsl::narrow_cast<decltype(WinMsg.pt.x)>(FormMoveDelta.x)),
                            (WinMsg.pt.y + gsl::narrow_cast<decltype(WinMsg.pt.y)>(FormMoveDelta.y))};
  auto       itIntlvSeq  = std::next(interleaveSequence.begin());
  auto const stitchPoint = pxCor2stch(point);
  auto const offset      = F_POINT {stitchPoint.x - itIntlvSeq->x, stitchPoint.y - itIntlvSeq->y};
  auto const count       = wrap::toUnsigned(interleaveSequence.size()) - 2U;
  auto&      form        = Instance->formList.operator[](closestFormToCursor);
  auto const nextVertex  = form::nxt(form, ClosestVertexToCursor);
  form::fltspac(nextVertex, count);
  form.vertexCount += count;
  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex + nextVertex);
  for (auto iOutput = size_t {1U}; iOutput < interleaveSequence.size() - 1U; ++iOutput) {
	*itVertex = F_POINT {itIntlvSeq->x + offset.x, itIntlvSeq->y + offset.y};
	++itVertex;
	++itIntlvSeq;
  }
  form::refil(ClosestFormToCursor);
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::selfpnt() {
  SelectedFormVertices.start = ClosestVertexToCursor;
  SelectedFormVertices.form  = ClosestFormToCursor;
  auto const itVertex =
      wrap::next(Instance->formVertices.cbegin(),
                 Instance->formList.operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
  ritfcor(*itVertex);
  Instance->stateMap.set(StateFlag::FRMPSEL);
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto thred::unselectAll() -> bool {
  Instance->stateMap.reset(StateFlag::FORMSEL);
  Instance->stateMap.reset(StateFlag::FRMPSEL);
  Instance->stateMap.reset(StateFlag::FPSEL);
  Instance->stateMap.set(StateFlag::RESTCH);
  unmsg();
  unboxs();
  unpat();
  Instance->stateMap.reset(StateFlag::SATIN);
  if (Instance->stateMap.testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (Instance->stateMap.testAndReset(StateFlag::BIGBOX)) {
	Instance->stateMap.set(StateFlag::RESTCH);
  }
  if (Instance->stateMap.test(StateFlag::PRFACT)) {
	if (Instance->stateMap.testAndReset(StateFlag::HUPMSG)) {
	  unsid();
	  return true;
	}
	if (PreferenceIndex != 0U) {
	  chknum();
	  unsid();
	  PreferenceIndex = 0;
	  return true;
	}
	DestroyWindow(PreferencesWindow);
	Instance->stateMap.reset(StateFlag::PRFACT);
	if (Instance->stateMap.testAndReset(StateFlag::WASRT)) {
	  Instance->stateMap.set(StateFlag::INSRT);
	}
  }
  if (FormMenuChoice != 0U) {
	chknum();
	FormMenuChoice = 0;
	Instance->stateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (Instance->stateMap.testAndReset(StateFlag::INSFRM)) {
	insadj();
	Instance->stateMap.reset(StateFlag::SHOINSF);
	Instance->stateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (Instance->stateMap.test(StateFlag::BAKSHO)) {
	unbsho();
	return true;
  }
  if (!Instance->selectedFormList.empty()) {
	Instance->selectedFormList.clear();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
  return false;
}

void thred::esccode() {
  bitmap::chkbit();
  menu::duhbit(MF_UNCHECKED);
  unthum();
  Instance->stateMap.reset(StateFlag::MOVSET);
  Instance->stateMap.reset(StateFlag::HID);
  Instance->stateMap.reset(StateFlag::FRMOF);
  Instance->stateMap.reset(StateFlag::THRDS);
  redraw(Instance->buttonWin.operator[](HHID));
  menu::resetThreadView();
  Instance->stateMap.reset(StateFlag::RUNPAT);
  Instance->stateMap.reset(StateFlag::WASPAT);
  Instance->stateMap.reset(StateFlag::WASBLAK);
  Instance->stateMap.reset(StateFlag::GTUANG);
  Instance->stateMap.reset(StateFlag::GTUSPAC);
  Instance->stateMap.reset(StateFlag::GTWLKIND);
  Instance->stateMap.reset(StateFlag::GTWLKLEN);
  DestroyWindow(SpeedScrollBar);
  Instance->stateMap.reset(StateFlag::GMRK);
  Instance->stateMap.reset(StateFlag::FORMSEL);
  Instance->selectedFormList.clear();
}

void thred::qcode() {
  trace::untrace();
  Instance->stateMap.reset(StateFlag::HIDMAP);
  Instance->stateMap.reset(StateFlag::FILDIR);
  ReleaseCapture();
  if (Instance->stitchBuffer.size() == 1) {
	Instance->stitchBuffer.clear();
	Instance->stateMap.reset(StateFlag::INIT);
  }
  auto& formList = Instance->formList;

  if (Instance->stateMap.testAndReset(StateFlag::POLIMOV)) { // aborting form add
	if (formList.back().vertexCount != 0U) {
	  auto const first = wrap::next(Instance->formVertices.begin(), formList.back().vertexIndex);
	  auto const last  = wrap::next(first, formList.back().vertexCount);
	  Instance->formVertices.erase(first, last);
	}
	if (formList.back().satinGuideCount != 0U) {
	  auto const first = wrap::next(Instance->satinGuides.begin(), formList.back().satinGuideIndex);
	  auto const last  = wrap::next(first, formList.back().satinGuideCount);
	  Instance->satinGuides.erase(first, last);
	}
	formList.pop_back();
	if (!formList.empty()) {
	  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
	}
  }
  if (Instance->stateMap.testAndReset(StateFlag::FUNCLP)) { // aborting form paste
	backup::bak();
	ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  }
  if (Instance->stateMap.testAndReset(StateFlag::FUNSCLP)) { // aborting forms paste
	backup::bak();
	Instance->selectedFormList.clear();
	ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  }
  if (!Instance->userFlagMap.test(UserFlag::MARQ)) {
	Instance->stateMap.reset(StateFlag::GMRK);
  }
  if (Instance->stateMap.testAndReset(StateFlag::PRFACT)) {
	if (Instance->stateMap.testAndReset(StateFlag::WASRT)) {
	  Instance->stateMap.set(StateFlag::INSRT);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}

	rstAll();
  }
  else {
	rstAll();
  }
  Instance->stateMap.set(StateFlag::RESTCH);
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
}

void thred::dufdef() noexcept {
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, &fthdefprc);
}

auto thred::displayBackups() -> bool {
  auto iVersion = uint8_t {};
  for (auto const& iBackup : BackupViewer) {
	if (WinMsg.hwnd == iBackup) {
	  FileVersionIndex = iVersion;
	  if (Instance->stateMap.test(StateFlag::THUMSHO)) {
		if (savcmp()) {
		  thumbak();
		}
		else {
		  if (Instance->stateMap.test(StateFlag::RBUT)) {
			thumbak();
		  }
		  else {
			Instance->stateMap.set(StateFlag::THUMON);
			displayText::savdisc();
		  }
		  return true;
		}
	  }
	  else {
		rebak();
	  }
	  rstAll();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return true;
	}
	++iVersion;
  }
  return false;
}

auto thred::inColorbar() noexcept -> bool {
  return WinMsg.pt.x >= ColorBarRect.left && WinMsg.pt.x <= ColorBarRect.right &&
         WinMsg.pt.y >= ColorBarRect.top && WinMsg.pt.y <= ColorBarRect.bottom;
}

auto thred::getColorbarVertPosition() noexcept -> float {
  return wrap::toFloat(WinMsg.pt.y - ColorBarRect.top) /
         wrap::toFloat(ColorBarRect.bottom - ColorBarRect.top);
}

void thred::handleFormSelected() {
  if (FormMenuChoice != 0U) {
	chknum();
	FormMenuChoice = 0;
	return;
  }
  if (FormDataSheet != nullptr) {
	undat();
	unsid();
	FormMenuChoice = 0;
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
  lenCalc();
  if (!Instance->stateMap.testAndReset(StateFlag::ENTROT)) {
	Instance->stateMap.reset(StateFlag::FORMSEL);
  }
  Instance->stateMap.set(StateFlag::DUMEN);
  if (!closPnt1(ClosestPointIndex)) {
	unbox();
  }
}

void thred::updateHoopSize() {
  auto&      sideWindow = ThrSingle->SideWindow;
  auto const itHwnd     = std::ranges::find(sideWindow, WinMsg.hwnd);
  if (itHwnd == sideWindow.end()) {
	return;
  }
  switch (auto const option = std::distance(sideWindow.begin(), itHwnd) + 1; option) {
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
	default: {
	  outDebugString(L"default hit in updateHoopSize: option [{}]\n", option);
	  break;
	}
  }
  UnzoomedRect = {.cx = std::lround(IniFile.hoopSizeX), .cy = std::lround(IniFile.hoopSizeY)};
  form::sethup();
  chkhup();
  unsid();
  formForms::prfmsg();
}

auto thred::updateFillColor() -> bool {
  while (true) {
	savdo();
	if (Instance->stateMap.testAndReset(StateFlag::FSETFCOL)) {
	  xt::dufcol(VerticalIndex + 1U);
	  return true;
	}
	if (Instance->stateMap.testAndReset(StateFlag::BRDSID)) {
	  form::nubrdcol(VerticalIndex);
	  break;
	}
	if (Instance->stateMap.testAndReset(StateFlag::APSID)) {
	  form::nulapcol(VerticalIndex);
	  break;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FTHSID)) {
	  form::nufthcol(VerticalIndex);
	  break;
	}
	if (Instance->stateMap.testAndReset(StateFlag::UNDCOL)) {
	  Instance->formList.operator[](ClosestFormToCursor).underlayColor = VerticalIndex & COLMSK;
	  form::refilfn(ClosestFormToCursor);
	  coltab();
	  break;
	}
	if (Instance->stateMap.testAndReset(StateFlag::FSETUCOL)) {
	  xt::dundcol(VerticalIndex + 1U);
	  return true;
	}
	form::nufilcol(VerticalIndex);
	break;
  }
  auto buffer = std::array<wchar_t, 2> {};
  wrap::narrow(buffer[0], VerticalIndex + '0');
  SetWindowText(Instance->valueWindow.operator[](LBRDCOL), buffer.data());
  unsid();
  coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
  Instance->stateMap.reset(StateFlag::SIDCOL);
  FormMenuChoice = 0;
  return true;
}

auto thred::updatePreferences() -> bool {
  chknum();
  auto const& valueWindow = Instance->valueWindow;
  if (WinMsg.hwnd == valueWindow.operator[](PRFFILEND)) {
	if (Instance->userFlagMap.testAndFlip(UserFlag::SQRFIL)) {
	  SetWindowText(valueWindow.operator[](PRFFILEND), displayText::loadStr(IDS_PNTD).c_str());
	}
	else {
	  SetWindowText(valueWindow.operator[](PRFFILEND), displayText::loadStr(IDS_SQR).c_str());
	}
	return true;
  }
  if (WinMsg.hwnd == valueWindow.operator[](PRFSATEND)) {
	if (Instance->userFlagMap.testAndFlip(UserFlag::BLUNT)) {
	  SetWindowText(valueWindow.operator[](PRFSATEND), displayText::loadStr(IDS_TAPR).c_str());
	}
	else {
	  SetWindowText(valueWindow.operator[](PRFSATEND), displayText::loadStr(IDS_BLUNT).c_str());
	}
	return true;
  }
  if (WinMsg.hwnd == valueWindow.operator[](PRFSATUND)) {
	if (Instance->userFlagMap.testAndFlip(UserFlag::DUND)) {
	  SetWindowText(valueWindow.operator[](PRFSATUND), displayText::loadStr(IDS_OFF).c_str());
	}
	else {
	  SetWindowText(valueWindow.operator[](PRFSATUND), displayText::loadStr(IDS_ON).c_str());
	}
	return true;
  }
  if (WinMsg.hwnd == valueWindow.operator[](PRFHUPTYP)) {
	sidhup();
	return true;
  }
  for (auto iPreference = 0U; iPreference < PRFLINS; ++iPreference) {
	if (WinMsg.hwnd == valueWindow.operator[](iPreference)) {
	  PreferenceIndex = iPreference + 1U;
	  outDebugString(L"handleLeftButtonDown:PreferenceIndex [{}]\n", PreferenceIndex);
	  formForms::prfsid(valueWindow.operator[](iPreference));
	  break;
	}
  }
  return true;
}

auto thred::handleSideWindowActive() -> bool {
  savdo();
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  if (FormMenuChoice == LFTHTYP) {
	if (auto const iFeather = std::ranges::find_if(FTHRLIST,
	                                               [](LIST_TYPE const& feather) noexcept -> bool {
	                                                 return WinMsg.hwnd ==
	                                                        ThrSingle->SideWindow.operator[](feather.value);
	                                               });
	    iFeather != FTHRLIST.end()) {
	  form.feather.fillType = iFeather->value;
	  unsid();
	  form::refil(ClosestFormToCursor);
	  formForms::refrm();
	}
	return true;
  }
  if (FormMenuChoice == LLAYR) {
	if (auto const iLayer = std::ranges::find_if(LAYRLIST,
	                                             [](LIST_TYPE const& layer) noexcept -> bool {
	                                               return WinMsg.hwnd ==
	                                                      ThrSingle->SideWindow.operator[](layer.value);
	                                             });
	    iLayer != LAYRLIST.end()) {
	  form::movlayr(iLayer->value);
	  Instance->stateMap.set(StateFlag::FORMSEL);
	  auto const layerStr = displayText::loadStr(iLayer->stringID);
	  SetWindowText(Instance->valueWindow.operator[](LLAYR), layerStr.c_str());
	}
	unsid();
	formForms::refrm();
	return true;
  }
  form.borderColor &= COLMSK;
  auto const& sideWindow = ThrSingle->SideWindow;
  if (Instance->stateMap.testAndReset(StateFlag::BRDACT)) {
	while (true) {
	  if (WinMsg.hwnd == sideWindow.operator[](0)) {
		if (form.isEdgeClip()) {
		  clip::deleclp(ClosestFormToCursor);
		}
		form.edgeType = 0;
		coltab();
		Instance->stateMap.set(StateFlag::RESTCH);
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGELINE)) {
		form::bord();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEBEAN)) {
		form::dubold();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGECLIP)) {
		form::fclp();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEANGSAT)) {
		satin::satbrd();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEAPPL)) {
		if (form.fillType != 0U) {
		  form::delmfil(ClosestFormToCursor);
		}
		form::apliq();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEPROPSAT)) {
		form::prpbrd(LineSpacing);
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEBHOL)) {
		form::bhol();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEPICOT)) {
		form::picot();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEDOUBLE)) {
		form::dubsfil(form);
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGELCHAIN)) {
		Instance->stateMap.set(StateFlag::LINCHN);
		form::chain();
		coltab();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGEOCHAIN)) {
		Instance->stateMap.reset(StateFlag::LINCHN);
		form::chain();
		coltab();
		break;
	  }
	  if (WinMsg.hwnd == sideWindow.operator[](EDGECLIPX)) {
		form::filclpx();
	  }
	  break;
	}
	formForms::refrm();
	form::refil(ClosestFormToCursor);
	unmsg();
	unsid();
	Instance->stateMap.set(StateFlag::RESTCH);
	return true;
  }

  if (form.fillType == SAT && form.satinGuideCount != 0U) {
	satin::delsac(ClosestFormToCursor);
  }
  if ((form.edgeType & NEGUND) == EDGEAPPL) {
	form.edgeType = EDGEANGSAT;
	if (Instance->userFlagMap.test(UserFlag::DUND)) {
	  form.edgeType |= EGUND;
	}
  }
  auto textureFlag = false;
  while (true) {
	if (WinMsg.hwnd == sideWindow.operator[](0)) { // none
	  form.type = FRMFPOLY;
	  form::delmfil(ClosestFormToCursor);
	  form.fillType = 0;
	  coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](VRTF)) { // vertical fill
	  savdo();
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
	if (WinMsg.hwnd == sideWindow.operator[](HORF)) { // horizontal fill
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
	if (WinMsg.hwnd == sideWindow.operator[](ANGF)) { // angle fill
	  form.type = FRMFPOLY;
	  if (form.fillType != 0U) {
		if (form.satinGuideCount != 0U) {
		  satin::delsac(ClosestFormToCursor);
		}
		respac(form);
		// ToDo - should we be using the angle information already present
		form.fillType  = ANGF;
		form.fillAngle = IniFile.fillAngle;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }
	  form::filangl();
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](SATF)) { // fan fill
	  form.type = SAT;
	  if (form.fillType == ANGF || form.fillType == ANGCLPF || form.fillType == TXANGF) {
		form.satinGuideIndex = 0;
	  }
	  if (form.fillType != 0U) {
		respac(form);
		form.fillType = SATF;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }
	  Instance->stateMap.reset(StateFlag::FTHR);
	  form::filsat();
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](CLPF)) { // fan clip
	  form.type = SAT;
	  if (form.fillType == ANGF || form.fillType == ANGCLPF || form.fillType == TXANGF) {
		form.satinGuideIndex = 0;
	  }
	  form::clpfil();
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](CONTF)) { // contour fill
	  if (form.vertexCount > 4) {
		if (form.fillType != 0U) {
		  if (form.fillType == CLPF) {
			form.fillSpacing = LineSpacing;
		  }
		  form::chkcont();
		  clip::delclps(ClosestFormToCursor);
		  break;
		}
		form::contfil();
	  }
	  else {
		displayText::tabmsg(IDS_CONT, false);
	  }
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](VCLPF)) { // vertical clip
	  if (sidclp()) {
		form::vrtsclp(ClosestFormToCursor);
	  }
	  Instance->stateMap.reset(StateFlag::CLPSHO);
	  coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](HCLPF)) { // horizontal clip
	  if (sidclp()) {
		form::horsclp();
	  }
	  Instance->stateMap.reset(StateFlag::CLPSHO);
	  coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](ANGCLPF)) { // angle clip
	  if (sidclp()) {
		if (form.satinGuideCount != 0U) {
		  satin::delsac(ClosestFormToCursor);
		}
		form::angsclp(form);
	  }
	  Instance->stateMap.reset(StateFlag::CLPSHO);
	  coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](FTHF)) { // feather fill
	  if (form.fillType == ANGF || form.fillType == ANGCLPF || form.fillType == TXANGF) {
		form.satinGuideIndex = 0;
	  }
	  xt::fethrf(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::INIT);
	  coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](TXVRTF)) // vertical texture
	{
	  if (form.isTexture()) {
		form.fillType = TXVRTF;
		break;
	  }
	  if (texture::dutxtfil()) {
		textureFlag = true;
	  }
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](TXHORF)) // horizontal texture
	{
	  if (form.isTexture()) {
		form.fillType = TXHORF;
		break;
	  }
	  if (texture::dutxtfil()) {
		textureFlag = true;
	  }
	  break;
	}
	if (WinMsg.hwnd == sideWindow.operator[](TXANGF)) // angle texture
	{
	  if (form.isTexture()) {
		form.fillType  = TXANGF;
		form.fillAngle = IniFile.fillAngle;
		break;
	  }
	  if (texture::dutxtfil()) {
		textureFlag = true;
	  }
	}
	break;
  }
  if (textureFlag) {
	unsid();
	DestroyWindow(FormDataSheet);
	FormDataSheet = nullptr;
	Instance->stateMap.set(StateFlag::WASFRMFRM);
  }
  else {
	formForms::refrm();
	form::refil(ClosestFormToCursor);
	unmsg();
	unsid();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
  return true;
}

auto thred::handleFormDataSheet() -> bool {
  chknum();
  unsid();
  auto&       form        = Instance->formList.operator[](ClosestFormToCursor);
  auto const& valueWindow = Instance->valueWindow;
  auto const& labelWindow = ThrSingle->LabelWindow;
  while (true) {
	if (WinMsg.hwnd == valueWindow.operator[](LTXOF) || WinMsg.hwnd == labelWindow.operator[](LTXOF)) {
	  // draw the texture fill spacing window
	  FormMenuChoice = LTXOF;
	  formForms::sidwnd(valueWindow.operator[](LTXOF));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LUSPAC) || WinMsg.hwnd == labelWindow.operator[](LUSPAC)) {
	  // draw the underlay spacing window
	  FormMenuChoice = LUSPAC;
	  formForms::sidwnd(valueWindow.operator[](LUSPAC));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LUANG) || WinMsg.hwnd == labelWindow.operator[](LUANG)) {
	  // draw the underlay angle window
	  FormMenuChoice = LUANG;
	  formForms::sidwnd(valueWindow.operator[](LUANG));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LULEN) || WinMsg.hwnd == labelWindow.operator[](LULEN)) {
	  // draw the underlay length window
	  FormMenuChoice = LULEN;
	  formForms::sidwnd(valueWindow.operator[](LULEN));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LWLKIND) || WinMsg.hwnd == labelWindow.operator[](LWLKIND)) {
	  // draw the edge walk/underlay indent window
	  FormMenuChoice = LWLKIND;
	  formForms::sidwnd(valueWindow.operator[](LWLKIND));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHSIZ) || WinMsg.hwnd == labelWindow.operator[](LFTHSIZ)) {
	  FormMenuChoice = LFTHSIZ;
	  formForms::sidwnd(valueWindow.operator[](LFTHSIZ));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHNUM) || WinMsg.hwnd == labelWindow.operator[](LFTHNUM)) {
	  // draw the feather number window
	  FormMenuChoice = LFTHNUM;
	  formForms::sidwnd(valueWindow.operator[](LFTHNUM));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHFLR) || WinMsg.hwnd == labelWindow.operator[](LFTHFLR)) {
	  // draw the feather floor window
	  FormMenuChoice = LFTHFLR;
	  formForms::sidwnd(valueWindow.operator[](LFTHFLR));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHUPCNT) || WinMsg.hwnd == labelWindow.operator[](LFTHUPCNT)) {
	  // draw the feather up count window
	  FormMenuChoice = LFTHUPCNT;
	  formForms::sidwnd(valueWindow.operator[](LFTHUPCNT));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHDWNCNT) || WinMsg.hwnd == labelWindow.operator[](LFTHDWNCNT)) {
	  // draw the feather down count window
	  FormMenuChoice = LFTHDWNCNT;
	  formForms::sidwnd(valueWindow.operator[](LFTHDWNCNT));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHBLND) || WinMsg.hwnd == labelWindow.operator[](LFTHBLND)) {
	  // draw the feather blend window
	  form.extendedAttribute ^= AT_FTHBLND;
	  formForms::refrm();
	  form::refil(ClosestFormToCursor);
	  unsid();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHUP) || WinMsg.hwnd == labelWindow.operator[](LFTHUP)) {
	  // draw the feather up or down window
	  form.extendedAttribute ^= AT_FTHUP;
	  form::refil(ClosestFormToCursor);
	  auto const choice = displayText::loadStr((form.extendedAttribute & AT_FTHUP) == 0U ? IDS_OFF : IDS_ON);
	  SetWindowText(valueWindow.operator[](LFTHUP), choice.c_str());
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHBTH) || WinMsg.hwnd == labelWindow.operator[](LFTHBTH)) {
	  // draw the feather both window
	  form.extendedAttribute ^= AT_FTHBTH;
	  formForms::refrm();
	  form::refil(ClosestFormToCursor);
	  unsid();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHTYP) || WinMsg.hwnd == labelWindow.operator[](LFTHTYP)) {
	  // draw the feather fill type window
	  FormMenuChoice = LFTHTYP;
	  sidmsg(form, valueWindow.operator[](LFTHTYP));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFRM) || WinMsg.hwnd == labelWindow.operator[](LFRM)) {
	  // draw the form type window
	  savdo();
	  form::unfil();
	  if (form.type == FRMLINE) {
		form.type = FRMFPOLY;
	  }
	  else {
		form.type = FRMLINE;
	  }
	  coltab();
	  satin::delsac(ClosestFormToCursor);
	  formForms::refrm();
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LLAYR) || WinMsg.hwnd == labelWindow.operator[](LLAYR)) {
	  // draw the layer window
	  FormMenuChoice = LLAYR;
	  Instance->stateMap.reset(StateFlag::FILTYP);
	  sidmsg(form, valueWindow.operator[](LLAYR));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFRMFIL) || WinMsg.hwnd == labelWindow.operator[](LFRMFIL)) {
	  // draw the fill type window
	  Instance->stateMap.reset(StateFlag::FILTYP);
	  FormMenuChoice = LFRMFIL;
	  sidmsg(form, valueWindow.operator[](LFRMFIL));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFRMCOL) || WinMsg.hwnd == labelWindow.operator[](LFRMCOL)) {
	  // draw the fill color window
	  FormMenuChoice = LFRMCOL;
	  formForms::sidwnd(valueWindow.operator[](LFRMCOL));
	  Instance->stateMap.set(StateFlag::SIDCOL);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LUNDCOL) || WinMsg.hwnd == labelWindow.operator[](LUNDCOL)) {
	  // draw the underlay color window
	  FormMenuChoice = LUNDCOL;
	  formForms::sidwnd(valueWindow.operator[](LUNDCOL));
	  Instance->stateMap.set(StateFlag::SIDCOL);
	  Instance->stateMap.set(StateFlag::UNDCOL);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFTHCOL) || WinMsg.hwnd == labelWindow.operator[](LFTHCOL)) {
	  // draw the feather color window
	  FormMenuChoice = LFTHCOL;
	  formForms::sidwnd(valueWindow.operator[](LFTHCOL));
	  Instance->stateMap.set(StateFlag::SIDCOL);
	  Instance->stateMap.set(StateFlag::FTHSID);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFRMSPAC) || WinMsg.hwnd == labelWindow.operator[](LFRMSPAC)) {
	  // draw the form spacing window
	  FormMenuChoice = LFRMSPAC;
	  formForms::sidwnd(valueWindow.operator[](LFRMSPAC));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFRMLEN) || WinMsg.hwnd == labelWindow.operator[](LFRMLEN)) {
	  // draw the form stitch length window
	  FormMenuChoice = LFRMLEN;
	  formForms::sidwnd(valueWindow.operator[](LFRMLEN));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRD) || WinMsg.hwnd == labelWindow.operator[](LBRD)) {
	  // draw the border window
	  Instance->stateMap.set(StateFlag::FILTYP);
	  sidmsg(form, valueWindow.operator[](LBRD));
	  Instance->stateMap.set(StateFlag::BRDACT);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRDCOL) || WinMsg.hwnd == labelWindow.operator[](LBRDCOL)) {
	  // draw the border color window
	  FormMenuChoice = LBRDCOL;
	  formForms::sidwnd(valueWindow.operator[](LBRDCOL));
	  Instance->stateMap.set(StateFlag::SIDCOL);
	  Instance->stateMap.set(StateFlag::BRDSID);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRDSPAC) || WinMsg.hwnd == labelWindow.operator[](LBRDSPAC)) {
	  // draw the border stitch spacing window
	  FormMenuChoice = LBRDSPAC;
	  formForms::sidwnd(valueWindow.operator[](LBRDSPAC));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRDLEN) || WinMsg.hwnd == labelWindow.operator[](LBRDLEN)) {
	  // draw the border stitch length window
	  FormMenuChoice = LBRDLEN;
	  formForms::sidwnd(valueWindow.operator[](LBRDLEN));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRDSIZ) || WinMsg.hwnd == labelWindow.operator[](LBRDSIZ)) {
	  // draw the border stitch size window
	  FormMenuChoice = LBRDSIZ;
	  formForms::sidwnd(valueWindow.operator[](LBRDSIZ));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LAPCOL) || WinMsg.hwnd == labelWindow.operator[](LAPCOL)) {
	  // draw the applique color window
	  FormMenuChoice = LAPCOL;
	  Instance->stateMap.set(StateFlag::SIDCOL);
	  Instance->stateMap.set(StateFlag::APSID);
	  formForms::sidwnd(valueWindow.operator[](LAPCOL));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBCSIZ) || WinMsg.hwnd == labelWindow.operator[](LBCSIZ)) {
	  // draw the buttonhole corner size window
	  FormMenuChoice = LBCSIZ;
	  formForms::sidwnd(valueWindow.operator[](LBCSIZ));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBSTRT) || WinMsg.hwnd == labelWindow.operator[](LBSTRT)) {
	  // draw the satin start style window
	  auto const code     = form.attribute & SBLNT;
	  auto const bluntStr = displayText::loadStr(code != 0U ? IDS_TAPR : IDS_BLUNT);
	  SetWindowText(valueWindow.operator[](LBSTRT), bluntStr.c_str());
	  if (code != 0U) {
		form.attribute &= NSBLNT;
	  }
	  else {
		form.attribute |= SBLNT;
	  }
	  form::refil(ClosestFormToCursor);
	  coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBFIN) || WinMsg.hwnd == labelWindow.operator[](LBFIN)) {
	  // draw the satin finish style window
	  auto const code     = form.attribute & FBLNT;
	  auto const bluntStr = displayText::loadStr(code != 0U ? IDS_TAPR : IDS_BLUNT);
	  SetWindowText(valueWindow.operator[](LBFIN), bluntStr.c_str());
	  if (code != 0U) {
		form.attribute &= NFBLNT;
	  }
	  else {
		form.attribute |= FBLNT;
	  }
	  form::refil(ClosestFormToCursor);
	  coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFRMANG) || WinMsg.hwnd == labelWindow.operator[](LFRMANG)) {
	  // draw the form fill angle window
	  FormMenuChoice = LFRMANG;
	  formForms::sidwnd(valueWindow.operator[](LFRMANG));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRDPIC) || WinMsg.hwnd == labelWindow.operator[](LBRDPIC)) {
	  // draw the picot spacing window
	  FormMenuChoice = LBRDPIC;
	  formForms::sidwnd(valueWindow.operator[](LBRDPIC));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRDUND) || WinMsg.hwnd == labelWindow.operator[](LBRDUND)) {
	  // draw the border underlay window
	  form.edgeType ^= EGUND;
	  form::refil(ClosestFormToCursor);
	  auto const code    = form.edgeType & EGUND;
	  auto const bUndStr = displayText::loadStr(code != 0U ? IDS_ON : IDS_OFF);
	  SetWindowText(valueWindow.operator[](LBRDUND), bUndStr.c_str());
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LSACANG) || WinMsg.hwnd == labelWindow.operator[](LSACANG)) {
	  // draw the clipboard fill angle window
	  FormMenuChoice = LSACANG;
	  formForms::sidwnd(valueWindow.operator[](LSACANG));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFRMFAZ) || WinMsg.hwnd == labelWindow.operator[](LFRMFAZ)) {
	  // draw the form fill phase window
	  FormMenuChoice = LFRMFAZ;
	  formForms::sidwnd(valueWindow.operator[](LFRMFAZ));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBRDPOS) || WinMsg.hwnd == labelWindow.operator[](LBRDPOS)) {
	  // draw the chain position window
	  FormMenuChoice = LBRDPOS;
	  formForms::sidwnd(valueWindow.operator[](LBRDPOS));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LBFILSQR) || WinMsg.hwnd == labelWindow.operator[](LBFILSQR)) {
	  // draw the form fill ends square window
	  xt::dubit(form, AT_SQR);
	  auto const choice = displayText::loadStr((form.extendedAttribute & AT_SQR) == 0U ? IDS_PNTD : IDS_SQR);
	  SetWindowText(valueWindow.operator[](LBFILSQR), choice.c_str());
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFSTRT) || WinMsg.hwnd == labelWindow.operator[](LFSTRT)) {
	  // draw the form fill start on/off window
	  xt::dubit(form, AT_STRT);
	  formForms::refrm();
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LDSTRT) || WinMsg.hwnd == labelWindow.operator[](LDSTRT)) {
	  // draw the form fill start window
	  FormMenuChoice = LDSTRT;
	  formForms::sidwnd(valueWindow.operator[](LDSTRT));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LFEND) || WinMsg.hwnd == labelWindow.operator[](LFEND)) {
	  // draw the form fill end on/off window
	  xt::dubit(form, AT_END);
	  formForms::refrm();
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LDEND) || WinMsg.hwnd == labelWindow.operator[](LDEND)) {
	  // draw the form fill end window
	  FormMenuChoice = LDEND;
	  formForms::sidwnd(valueWindow.operator[](LDEND));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LWALK) || WinMsg.hwnd == labelWindow.operator[](LWALK)) {
	  // draw the edge walk window
	  xt::dubit(form, AT_WALK);
	  formForms::refrm();
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LCWLK) || WinMsg.hwnd == labelWindow.operator[](LCWLK)) {
	  // draw the center walk window
	  xt::dubit(form, AT_CWLK);
	  formForms::refrm();
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LUND) || WinMsg.hwnd == labelWindow.operator[](LUND)) {
	  // draw the underlay window
	  xt::dubit(form, AT_UND);
	  formForms::refrm();
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LMAXFIL) || WinMsg.hwnd == labelWindow.operator[](LMAXFIL)) {
	  // draw the maximum fill stitch length window
	  FormMenuChoice = LMAXFIL;
	  formForms::sidwnd(valueWindow.operator[](LMAXFIL));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LMINFIL) || WinMsg.hwnd == labelWindow.operator[](LMINFIL)) {
	  // draw the minimum fill stitch length window
	  FormMenuChoice = LMINFIL;
	  formForms::sidwnd(valueWindow.operator[](LMINFIL));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LMAXBRD) || WinMsg.hwnd == labelWindow.operator[](LMAXBRD)) {
	  // draw the maximum border stitch length window
	  FormMenuChoice = LMAXBRD;
	  formForms::sidwnd(valueWindow.operator[](LMAXBRD));
	  break;
	}
	if (WinMsg.hwnd == valueWindow.operator[](LMINBRD) || WinMsg.hwnd == labelWindow.operator[](LMINBRD)) {
	  // draw the minimum border stitch length window
	  FormMenuChoice = LMINBRD;
	  formForms::sidwnd(valueWindow.operator[](LMINBRD));
	  break;
	}
	break;
  }
  outDebugString(L"handleLeftButtonDown:FormMenuChoice [{}]\n", FormMenuChoice);

  return true;
}

void thred::updateBackground() {
  BackgroundColor = 0x505050; // darker DimGrey NOLINT(readability-magic-numbers)
  nuPen(BackgroundPen, 1, BackgroundColor);
  BackgroundPenWidth = 1;
  DeleteObject(BackgroundBrush);
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  if (bitmap::ismap()) {
	bitmap::bfil(BackgroundColor);
  }
}

void thred::thumbQuit() {
  unthum();
  Instance->stateMap.reset(StateFlag::BAKSHO);
  esccode();
  qcode();
}

void thred::thumbHome() {
  ThumbnailIndex = 0;
  nuthsel();
}

void thred::thumbEnd() {
  ThumbnailIndex = wrap::toUnsigned(ThrSingle->Thumbnails.size());
  nuthbak(QUADRT);
}

void thred::otherPeriod() noexcept {
  // ToDo - only allow entry if there is not already a period in the buffer
  if (auto& sideWindowEntryBuffer = ThrSingle->SideWindowEntryBuffer;
      SideWinMsgIdx < sideWindowEntryBuffer.size() - 1U) {
	sideWindowEntryBuffer.operator[](SideWinMsgIdx++) = '.';
	sideWindowEntryBuffer.operator[](SideWinMsgIdx)   = 0;
	SetWindowText(SideMessageWindow, ThrSingle->SideWindowEntryBuffer.data());
  }
}

void thred::otherBack() noexcept {
  if (SideWinMsgIdx != 0U) {
	ThrSingle->SideWindowEntryBuffer.operator[](--SideWinMsgIdx) = 0;
	SetWindowText(SideMessageWindow, ThrSingle->SideWindowEntryBuffer.data());
  }
}

void thred::otherReturn() {
  chknum();
  FormMenuChoice  = 0;
  PreferenceIndex = 0;
}

// return the width of a text item
auto thred::txtWid(wchar_t const* string) noexcept(std::is_same_v<size_t, uint32_t>) -> SIZE {
  auto textSize = SIZE {};
  wrap::getTextExtentPoint32(ThredDC, string, wrap::toUnsigned(wcslen(string)), &textSize);
  return textSize;
}

auto thred::getHomeDir() noexcept -> fs::path& {
  return ThrSingle->HomeDirectory;
}

auto thred::getBackGroundBrush() noexcept -> HBRUSH {
  return BackgroundBrush;
}

void thred::tst() {
  ThrSingle->DesignerName.assign(L"Coder");
  ThrSingle->ThrName.assign(ThrSingle->DesignerName);
  Instance->stateMap.set(StateFlag::RESTCH);
}

#pragma warning(suppress : 26461) // pointer to const con.3 NOLINTNEXTLINE(readability-non-const-parameter)
auto APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int32_t nShowCmd)
    -> int32_t {
  // to keep the compiler from complaining
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nShowCmd);

  auto flag = false;
  ArgList   = CommandLineToArgvW(GetCommandLine(), &ArgCount);

#ifdef ALLOCFAILURE
  auto old_handler = std::set_new_handler(handle_program_memory_depletion);
#ifdef TEST_FAIL
  char* testalloc = new char[(~size_t((int32_t)0) / 2) - 1U]();
  testalloc[0]    = 1;
#endif
#endif
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if (auto const hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
      SUCCEEDED(hResult)) {
	ThrEdInstance          = hInstance;
	constexpr auto ICONDIM = 32; // window icon x & y dimension in pixels
	auto           winClass =
	    WNDCLASSEX {0U, 0U, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	winClass.cbSize      = sizeof(WNDCLASSEX);
	winClass.style       = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc = wndProc;
	winClass.cbClsExtra  = 0;
	winClass.cbWndExtra  = 0;
	winClass.hInstance   = ThrEdInstance;
	winClass.hIcon       = gsl::narrow_cast<HICON>(
        LoadImage(ThrEdInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, ICONDIM, ICONDIM, LR_SHARED));
	winClass.hCursor = nullptr; //  set the cursor to null as the cursor changes in the window:
	                            //  https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setcursor
	winClass.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	winClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	winClass.lpszClassName = L"thred";
	winClass.hIconSm       = nullptr;

#if HIGHDPI
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto previousDpiContext = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
#endif

	if (RegisterClassEx(&winClass)) {
	  Instance = MY_SINGLE::getInstance();
	  try {
		Instance->initialize();
	  }
	  catch (std::bad_alloc const&) {
		outDebugString(L"Memory allocation failure\n");
		return EXIT_FAILURE;
	  }
	  catch (std::exception const& e) {
		outDebugString(L"Exception caught: {}\n", static_cast<const void*>(e.what()));
		return EXIT_FAILURE;
	  }
	  catch (...) {
		outDebugString(L"Unknown exception caught\n");
		return EXIT_FAILURE;
	  }

	  DST::dstInit();
	  bal::balInit();
	  bitmap::bmpInit();
	  satin::satinInit();
	  texture::textureInit();
	  tfc::fClipInit();
	  trace::traceInit();
	  thrInit();

	  ThrSingle->DefaultColorWin.resize(COLORCNT);
	  ThrSingle->FormControlPoints.resize(OUTPNTS);
	  ThrSingle->LabelWindow.resize(LASTLIN);
	  ThrSingle->MsgBuffer.reserve(MSGSIZ);
	  ThrSingle->NearestPixel.resize(NERCNT);
	  ThrSingle->NearestPoint.resize(NERCNT);
	  ThrSingle->PreviousNames.reserve(OLDNUM);
	  for (auto iVersion = 0U; iVersion < OLDNUM; ++iVersion) {
		ThrSingle->PreviousNames.emplace_back(L"");
	  }
	  ThrSingle->SideWindow.resize(SWCOUNT);
	  ThrSingle->SideWindowEntryBuffer.resize(SWBLEN);
	  ThrSingle->ThreadSizeWin.resize(COLORCNT);
	  ThrSingle->ThumbnailSearchString.reserve(TSSSIZ);
	  ThrSingle->UserColorWin.resize(COLORCNT);
	  ThrSingle->UserPen.resize(COLORCNT);
	  ThrSingle->VersionNames.reserve(OLDVER);
	  for (auto iVersion = wchar_t {}; iVersion < OLDVER; ++iVersion) {
		ThrSingle->VersionNames.emplace_back(L"");
	  }

	  texture::initTextureHistory();
	  redini();

	  createParams createParams {};
	  createParams.bEnableNonClientDpiScaling = TRUE;

	  if (IniFile.initialWindowCoords.right != 0) {
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
	  Instance->DPI = gsl::narrow<int32_t>(GetDpiForWindow(ThrEdWindow));
	  ScrollSize    = MulDiv(ScrollSize, Instance->DPI, STDDPI);
	  ColorBarSize  = MulDiv(ColorBarSize, Instance->DPI, STDDPI);
	  init();
	  if (Instance->userFlagMap.test(UserFlag::SAVMAX)) {
		ShowWindow(ThrEdWindow, SW_SHOWMAXIMIZED);
	  }
	  else {
		ShowWindow(ThrEdWindow, SW_SHOW);
	  }
	  if (ThrSingle->DesignerName.empty()) {
		ThrSingle->DesignerName.assign(displayText::loadStr(IDS_UNAM));
		thred::getdes();
	  }
	  auto xyRatio        = 1.0F; // expand form aspect ratio
	  auto rotationAngle  = 0.0F;
	  auto rotationCenter = F_POINT {};
	  auto textureForm    = FRM_HEAD {};
	  auto stretchBoxLine = std::vector<POINT> {};
	  stretchBoxLine.resize(SQPNTS); // stretch and expand
	  while (GetMessage(&WinMsg, nullptr, 0, 0)) {
		Instance->stateMap.set(StateFlag::SAVACT);
		if (!chkMsg(stretchBoxLine, xyRatio, rotationAngle, rotationCenter, textureForm)) {
		  DispatchMessage(&WinMsg);
		}
		if (Instance->stateMap.testAndReset(StateFlag::FCHK)) {
		  repair::frmchkx();
		}
		if (Instance->stateMap.testAndReset(StateFlag::RESTCH)) {
		  thred::redraw(MainStitchWin);
		}
		if (Instance->stateMap.testAndReset(StateFlag::RELAYR)) {
		  ritlayr();
		}
		if (!Instance->stateMap.test(StateFlag::TXTRED)) {
		  sachk();
		}
		if (Instance->stateMap.testAndReset(StateFlag::DUMEN)) {
		  DrawMenuBar(ThrEdWindow);
		}
	  }
#ifdef ALLOCFAILURE
	  std::set_new_handler(old_handler);
#endif
	  flag = true;
	}

#if HIGHDPI
	// Restore the current thread's DPI awareness context
	SetThreadDpiAwarenessContext(previousDpiContext);
#endif
  }
  CoUninitialize();
  LocalFree(ArgList); // NOLINT(bugprone-multi-level-implicit-pointer-conversion)
  if (flag) {
	return 0;
  }
  return -1;
}

void thred::refreshColors() noexcept {
  auto itUserPen = ThrSingle->UserPen.begin();
  auto ucb       = UserColorBrush.begin();
  for (auto const& color : UserColor) {
	nuPen(*itUserPen, 1, color);
	nuBrush(*ucb, color);
	++itUserPen;
	++ucb;
  }
  for (auto const& iColor : ThrSingle->UserColorWin) {
	redraw(iColor);
  }
}

void thred::initPenBrush() noexcept {
  auto itUserPen = ThrSingle->UserPen.begin();
  auto ucb       = UserColorBrush.begin();
  for (auto const& ucolor : UserColor) {
	*itUserPen++ = wrap::createPen(PS_SOLID, PENNWID, ucolor);
	nuBrush(*ucb, ucolor);
	++ucb;
  }
}

void thred::initBackPenBrush() noexcept {
  nuPen(BackgroundPen, 1, BackgroundColor);
  BackgroundPenWidth = 1;
  DeleteObject(BackgroundBrush);
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
}

auto thred::setFileName() -> fs::path {
  auto const& workingFileName = Instance->workingFileName;

  return workingFileName.empty() ? ThrSingle->DefaultDirectory / L"balfil.thr" : workingFileName;
}

auto thred::getDesigner() -> std::wstring {
  auto const modifier = utf::utf8ToUtf16(std::string(ExtendedHeader.modifierName.data()));
  return displayText::format2(IDS_CREATBY, ThrSingle->DesignerName, modifier);
}

auto thred::getVerticalIndex() noexcept -> uint8_t {
  return VerticalIndex;
}

auto thred::getColorChangeIndex(uint32_t const iColor) noexcept -> uint16_t {
  return ThrSingle->ColorChangeTable.operator[](iColor).stitchIndex;
}

auto thred::getFormControlPoints() noexcept -> std::vector<POINT>& {
  return ThrSingle->FormControlPoints;
}

void thred::chkInsCol() {
  if (chkMsgs(WinMsg.pt, ThrSingle->DefaultColorWin.front(), ThrSingle->UserColorWin.back())) { // check if point is in any of the color windows
	inscol();
  }
}

void thred::chkDelCol() {
  if (chkMsgs(WinMsg.pt, ThrSingle->DefaultColorWin.front(), ThrSingle->UserColorWin.back())) {
	delcol();
  }
  else {
	delet();
  }
}

void thred::setSrchLargest() {
  setsrch(true);
}

void thred::setSrchSmallest() {
  setsrch(false);
}

auto thred::getZoomMin() noexcept -> float {
  return ZoomMin;
}

void thred::resetColors() {
  auto ucb         = UserColorBrush.begin();
  auto itUserColor = UserColor.begin();
  auto itUserPen   = ThrSingle->UserPen.begin();
  auto ucw         = ThrSingle->UserColorWin.begin();
  for (auto const& color : DEFAULT_COLORS) {
	*itUserColor = color;
	nuBrush(*ucb, *itUserColor);
	nuPen(*itUserPen, 1, *itUserColor);
	redraw(*ucw);
	++ucb;
	++itUserColor;
	++itUserPen;
	++ucw;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void thred::setThreadSize() noexcept {
  ThreadSize30 = TSIZ30;
  ThreadSize40 = TSIZ40;
  ThreadSize60 = TSIZ60;
}

void thred::viewThread30() {
  displayText::tsizmsg(L"30", ThreadSize30);
  Instance->stateMap.set(StateFlag::ENTR30);
}

void thred::viewThread40() {
  displayText::tsizmsg(L"40", ThreadSize40);
  Instance->stateMap.set(StateFlag::ENTR40);
}

void thred::viewThread60() {
  displayText::tsizmsg(L"60", ThreadSize60);
  Instance->stateMap.set(StateFlag::ENTR60);
}

void thred::setBackGroundColor() {
  if (nuBak() != 0U) {
	BackgroundColor = BackgroundColorStruct.rgbResult;
	nuPen(BackgroundPen, 1, BackgroundColor);
	BackgroundPenWidth = 1;
	DeleteObject(BackgroundBrush);
	BackgroundBrush = CreateSolidBrush(BackgroundColor);
	if (bitmap::ismap()) {
	  bitmap::bfil(BackgroundColor);
	}
	zumhom();
  }
}

void thred::openAuxFile() {
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  nuFil(FileIndices::DST);
	  break;
	}
#if PESACT
	case AUXPES: {
	  nuFil(FileIndices::PES);
	  break;
	}
#endif
	default: {
	  nuFil(FileIndices::PCS);
	}
  }
  showOnlyLayer(0U);
}

void thred::openNewFile() {
  if (savcmp()) {
	newFil();
	showOnlyLayer(0U);
  }
  else {
	displayText::savdisc();
	Instance->stateMap.set(StateFlag::NEWBAK);
	Instance->stateMap.reset(StateFlag::PRFACT);
	undat();
  }
}

void thred::openBitMapFile() {
  bitmap::lodbmp(ThrSingle->DefaultDirectory);
}

void thred::destroyChangeThreadSizeWindows() noexcept {
  for (auto const& iWindow : ChangeThreadSizeWin) {
	if (iWindow != nullptr) {
	  DestroyWindow(iWindow);
	}
  }
}

void thred::updateThreadSize(uint32_t const threadSizeSelected) {
  static constexpr auto THREAD_SIZE_MAP = std::array {L'3', L'4', L'6'};

  auto const itThreadSize = wrap::next(ThreadSize.begin(), threadSizeSelected);
  auto const tsm          = wrap::next(THREAD_SIZE_MAP.begin(), VerticalIndex);
  *itThreadSize           = *tsm;
  auto const tsi          = wrap::next(ThreadSizeIndex.begin(), threadSizeSelected);
  *tsi                    = VerticalIndex;
  auto buffer             = std::array<wchar_t, 3> {};
  buffer[0]               = *itThreadSize;
  buffer[1]               = L'0';
  auto const tsw          = wrap::next(ThrSingle->ThreadSizeWin.begin(), threadSizeSelected);
  SetWindowText(*tsw, buffer.data());
  Instance->stateMap.set(StateFlag::RESTCH);
  destroyChangeThreadSizeWindows();
}

auto thred::createChangeThreadSizeWindows() -> uint32_t {
  static constexpr auto THREAD_SIZES = std::array {L"30", L"40", L"60"};
  savdo();
  auto const threadSizeSelected = VerticalIndex;
  auto       idx                = gsl::narrow_cast<int32_t>(VerticalIndex);
  auto       iStr               = THREAD_SIZES.begin();
  std::ranges::generate(ChangeThreadSizeWin, [&idx, &iStr]() mutable noexcept -> HWND {
	return CreateWindow(L"STATIC",
	                    *iStr++,
	                    WS_CHILD | WS_VISIBLE | WS_BORDER,
	                    ButtonWidthX3,
	                    ButtonHeight * idx++,
	                    ButtonWidth,
	                    ButtonHeight,
	                    ThrEdWindow,
	                    nullptr,
	                    ThrEdInstance,
	                    nullptr);
  });
  Instance->stateMap.set(StateFlag::SIZSEL);
  return threadSizeSelected;
}

void thred::updateUserColor() {
  if (WinMsg.message == WM_LBUTTONDOWN && nuCol(UserColor.at(VerticalIndex)) != FALSE) {
	savdo();
	auto const itUserColor = wrap::next(UserColor.begin(), VerticalIndex);
	*itUserColor           = ColorStruct.rgbResult;
	auto const itUserPen   = wrap::next(ThrSingle->UserPen.begin(), VerticalIndex);
	nuPen(*itUserPen, 1, *itUserColor);
	auto const ucb = wrap::next(UserColorBrush.begin(), VerticalIndex);
	nuBrush(*ucb, *itUserColor);
	redraw(ThrSingle->UserColorWin.operator[](VerticalIndex));
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void thred::switchUserColors() noexcept {
  auto const code = ActiveColor;
  ActiveColor     = VerticalIndex & COLMSK;
  redraw(ThrSingle->UserColorWin.operator[](code));
  redraw(ThrSingle->UserColorWin.operator[](ActiveColor));
}

// ToDo - rename this to something more appropriate
void thred::redrawCapturedStitch(uint32_t const closestPointIndexClone) {
  ClosestPointIndex = closestPointIndexClone;
  unbox();
  unboxs();
  auto const index = Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & COLMSK;
  auto const spTSP = wrap::next(ThreadSizePixels.begin(), index);
  setbak(*spTSP + 3);
  auto linePoints = std::vector<POINT> {};
  linePoints.resize(3);
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
	if (!Instance->stitchBuffer.empty()) {
	  if (auto const lastStitch = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
	      ClosestPointIndex == lastStitch) {
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
      MoveLine1[0] = {.x = WinMsg.pt.x - StitchWindowOrigin.x, .y = WinMsg.pt.y - StitchWindowOrigin.y};
  if (ClosestPointIndex == 0) {
	Instance->stateMap.reset(StateFlag::ISDWN);
  }
  else {
	Instance->stateMap.set(StateFlag::ISDWN);
	MoveLine0[0] = stch2px1(ClosestPointIndex - 1);
  }
  auto iStitch = wrap::toUnsigned(Instance->stitchBuffer.size());
  if (iStitch != 0U) {
	--iStitch;
  }
  if (ClosestPointIndex >= iStitch) {
	Instance->stateMap.reset(StateFlag::ISUP);
  }
  else {
	Instance->stateMap.set(StateFlag::ISUP);
	MoveLine1[1] = stch2px1(ClosestPointIndex + 1U);
  }
  dulin(MoveLine0, MoveLine1);
  SetCapture(ThrEdWindow);
  Instance->stateMap.set(StateFlag::CAPT);
  displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
}

void thred::setFormControls() noexcept(!std::is_same_v<size_t, uint32_t>) {
  auto formsRect = F_RECTANGLE {};
  selRct(formsRect);
  auto& frmCtrls = ThrSingle->FormControlPoints;
  frmCtrls[PTL].x = frmCtrls[PBL].x = frmCtrls[PLM].x = frmCtrls[PTLE].x = std::lround(formsRect.left);
  frmCtrls[PTL].y = frmCtrls[PTR].y = frmCtrls[PTM].y = frmCtrls[PTLE].y = std::lround(formsRect.top);
  frmCtrls[PTR].x = frmCtrls[PRM].x = frmCtrls[PBR].x = std::lround(formsRect.right);
  frmCtrls[PBR].y = frmCtrls[PBM].y = frmCtrls[PBL].y = std::lround(formsRect.bottom);
  frmCtrls[PRM].y = frmCtrls[PLM].y = std::lround(wrap::midl(formsRect.top, formsRect.bottom));
  frmCtrls[PTM].x = frmCtrls[PBM].x = std::lround(wrap::midl(formsRect.right, formsRect.left));
}

void thred::setRotateCapture() {
  RotateBoxToCursorLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
                              .y = WinMsg.pt.y - StitchWindowOrigin.y};
  if (auto const adjustedPoint =
          F_POINT {gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
                   gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y)};
      std::hypot(adjustedPoint.x, adjustedPoint.y) < FCLOSNUF) {
	Instance->stateMap.set(StateFlag::MOVCNTR);
	unrot();
	ritrot(0, pxCor2stch(WinMsg.pt));
  }
  else {
	if (adjustedPoint.x != 0.0F) {
	  RotationHandleAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
	}
	else {
	  if (adjustedPoint.y > 0) {
		RotationHandleAngle = PI_FHALF;
	  }
	  else {
		RotationHandleAngle = -PI_FHALF;
	  }
	}
	Instance->stateMap.set(StateFlag::ROTUSHO);
	durotu();
	Instance->stateMap.set(StateFlag::ROTCAPT);
  }
}

auto thred::chkForm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  return form::chkfrm(ThrSingle->FormControlPoints, stretchBoxLine, xyRatio);
}

auto thred::getAdjustedPoint(F_POINT const stitchPoint) noexcept -> F_POINT {
  return F_POINT {StitchRangeRect.left + wrap::toFloat(SelectBoxOffset.x) - stitchPoint.x,
                  StitchRangeRect.bottom + wrap::toFloat(SelectBoxOffset.y) - stitchPoint.y};
}

auto thred::updateZoomFactor(F_POINT& newSize) noexcept -> float {
  if (newSize.x > newSize.y) {
	newSize.y = newSize.x / StitchWindowAspectRatio;
	return newSize.x / wrap::toFloat(UnzoomedRect.cx);
  }
  newSize.x = newSize.y * StitchWindowAspectRatio;
  return newSize.y / wrap::toFloat(UnzoomedRect.cx);
}

auto thred::getAdjustedDelta() -> F_POINT {
  RotateBoxToCursorLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
                              .y = WinMsg.pt.y - StitchWindowOrigin.y};
  return F_POINT {gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
                  gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y)};
}

auto thred::getRotationHandleAngle() noexcept -> float {
  return RotationHandleAngle;
}

void thred::updateMoveLine() {
  MoveLine0[1] =
      MoveLine1[0] = {.x = WinMsg.pt.x - StitchWindowOrigin.x, .y = WinMsg.pt.y - StitchWindowOrigin.y};
  dulin(MoveLine0, MoveLine1);
}

void thred::setSmallestStitchVal() {
  srchk();
  fillSortBuffer();
  SortIndex = 0U;
  setSrchSmallest();
  lensadj();
}

void thred::setLargestStitchVal() {
  srchk();
  fillSortBuffer();
  SortIndex = wrap::toUnsigned(ThrSingle->SortBuffer.size() - 1U);
  setSrchLargest();
  lensadj();
}

auto thred::getLabelWindow() noexcept -> std::vector<HWND>& {
  return ThrSingle->LabelWindow;
}

auto thred::getThrEdName() noexcept -> fs::path const& {
  return ThrSingle->ThrName;
}

void thred::rats() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	HorizontalRatio = (ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right);
	VerticalRatio = (ZoomRect.top - ZoomRect.bottom) / wrap::toFloat(StitchWindowClientRect.bottom);
  }
  else {
	HorizontalRatio = wrap::toFloat(UnzoomedRect.cx) / wrap::toFloat(StitchWindowClientRect.right);
	VerticalRatio   = wrap::toFloat(UnzoomedRect.cy) / wrap::toFloat(StitchWindowClientRect.bottom);
  }
}

void thred::ratsr() {
  if (Instance->stateMap.test(StateFlag::ZUMED)) {
	HorizontalRatio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	VerticalRatio = wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
  }
  else {
	HorizontalRatio = wrap::toFloat(StitchWindowClientRect.right) / wrap::toFloat(UnzoomedRect.cx);
	VerticalRatio   = wrap::toFloat(StitchWindowClientRect.bottom) / wrap::toFloat(UnzoomedRect.cy);
  }
}

auto thred::getMoveDelta(const POINT& point) noexcept -> F_POINT {
  return F_POINT {wrap::toFloat(point.x) / HorizontalRatio, -wrap::toFloat(point.y) / VerticalRatio};
}

auto thred::scaleHorizontal(float const& value) noexcept -> int32_t {
  return std::lround(value * HorizontalRatio);
}

auto thred::scaleVertical(float const& value) noexcept -> int32_t {
  return std::lround(value * VerticalRatio);
}

auto thred::getStitchWindowX() noexcept -> LONG {
  return StitchWindowSize.cx;
}

auto thred::getStitchBoxesThreshold() noexcept -> float {
  return StitchBoxesThreshold;
}

auto thred::getSmallStitchLength() noexcept -> float {
  return SmallStitchLength;
}

auto thred::getShowStitchThreshold() noexcept -> float {
  return ShowStitchThreshold;
}

void thred::selectAllPen() noexcept {
  SelectObject(StitchWindowMemDC, SelectAllPen);
}

void thred::pushPreviousForm() {
  Instance->selectedFormList.push_back(PreviousFormIndex);
}

void thred::setPreviousForm(uint32_t const value) noexcept {
  PreviousFormIndex = value;
}

void thred::resetPrefIndex() noexcept {
  PreferenceIndex = 0;
}

auto thred::checkPreferenceIndex(uint32_t const value) noexcept -> bool {
  return PreferenceIndex == value;
}

void thred::resetNearest() noexcept {
  NearestCount = 0;
}

void thred::selectMultiFormPen() noexcept {
  SelectObject(StitchWindowMemDC, MultiFormPen);
}

void thred::selectGridPen() noexcept {
  SelectObject(StitchWindowMemDC, GridPen);
}

void thred::numWnd() {
  auto messageRect = RECT {0L, 0L, 0L, 0L};
  GetClientRect(MsgWindow, &messageRect);
  auto wRect = RECT {0L, 0L, 0L, 0L};
  GetWindowRect(MainStitchWin, &wRect);
  auto xOffset = wRect.left;
  GetWindowRect(ThrEdWindow, &wRect);
  xOffset -= wRect.left;
  if (nullptr == GeneralNumberInputBox) {
	GeneralNumberInputBox = CreateWindow(L"STATIC",
	                                     nullptr,
	                                     SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                     xOffset + 5,
	                                     messageRect.bottom + 15,
	                                     ButtonWidthX3,
	                                     ButtonHeight,
	                                     ThrEdWindow,
	                                     nullptr,
	                                     ThrEdInstance,
	                                     nullptr);
  }
  else {
	throw std::runtime_error("GeneralNumberInputBox is null"); // we should never reach this
  }
  resetMsgBuffer();
}

void thred::destroyGeneralNumberInputBox() noexcept {
  if (nullptr != GeneralNumberInputBox) {
	DestroyWindow(GeneralNumberInputBox);
	GeneralNumberInputBox = nullptr;
  }
}

void thred::createTraceNumWin(int32_t const position) noexcept {
  constexpr auto DW_STYLE = DWORD {WS_CHILD | WS_VISIBLE | WS_BORDER};
  GeneralNumberInputBox   = CreateWindowEx(
      0L, L"STATIC", nullptr, DW_STYLE, ButtonWidthX3, position, ButtonWidthX3, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);
}
