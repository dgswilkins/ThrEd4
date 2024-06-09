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
// resharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "fmt/compile.h"
#include "gsl/narrow"
#include "gsl/pointers"
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
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <cwctype>
#include <filesystem>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>
#include <xutility>

#ifdef ALLOCFAILURE
// #include <new.h>
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

namespace thi {
void angdif(float& lowestAngle, float& highestAngle, float angle) noexcept;
void bakthum();
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
void cros(uint32_t iStitch);
void defNam(fs::path const& fileName);
auto defTxt(uint32_t iColor) -> COLORREF;
void delcol();
void delfstchs();
void delknt();
void delsmal(uint32_t startStitch, uint32_t endStitch);
void delstch1(uint32_t iStitch);
void destroyBV() noexcept;

auto CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

void doDrwInit();
void doInitUnzoomed();
void doInitZoomed();
void doStretch(uint32_t start, uint32_t end);
void drawBackground();
void drawOffScreenLine(const F_POINT_ATTR& iStitch, const F_POINT_ATTR& prevStitch, const float& maxYcoord);
void drwLin(std::vector<POINT>& linePoints, uint32_t currentStitch, uint32_t length, HPEN hPen);
void drwStch();
void drwknot();
void drwlstch(uint32_t finish);
void drwmrk(HDC hDC);
void duClos(uint32_t            startStitch,
            uint32_t            stitchCount,
            F_POINT const&      stitchPoint,
            std::vector<float>& gapToNearest) noexcept(!(std::is_same_v<ptrdiff_t, int>));
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
void frmpos(FRM_HEAD& form, float deltaX, float deltaY) noexcept(!(std::is_same_v<ptrdiff_t, int>));
void frmsnap(uint32_t start, uint32_t count) noexcept(!(std::is_same_v<ptrdiff_t, int>));
auto frmstch() -> bool;

auto CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

void getbak();
void getDocsFolder(fs::path& directory);
auto gethand(std::vector<F_POINT_ATTR> const& stitch, uint32_t stitchCount) noexcept -> uint32_t;
auto getMaxCount() -> uint32_t;
auto getNewFileName(fs::path& newFileName, FileStyles fileTypes, FileIndices fileIndex) -> bool;
void gselrng() noexcept;
auto handleNumericInput(wchar_t const& code, bool& retflag) -> bool;
void hidwnd(HWND hwnd) noexcept;
void infadj(float& xCoordinate, float& yCoordinate) noexcept;
void createBrushes() noexcept;
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
void resetState();
void nuFil(FileIndices fileIndex);
void nuRct() noexcept;
void nuStchSiz(uint32_t iColor, int32_t width) noexcept(!(std::is_same_v<ptrdiff_t, int>));
auto nuang(float OriginalAngle, float xDelta, float yDelta) noexcept -> float;
void nunams();
void nuselrct();
void nuthbak(uint32_t count);
void nuthsel();
void nuthum(wchar_t character);
void ofstch(std::vector<F_POINT_ATTR>& buffer, uint32_t iSource, char offset, F_POINT const& KnotStep, uint32_t KnotAttribute);
auto oldwnd(HWND window) noexcept -> bool;
void patdun();
void prtred(HANDLE fileHandle, uint32_t code);
auto pxchk(float pixelSize) -> uint16_t;
auto readTHRFile(std::filesystem::path const& newFileName) -> bool;
void rebak();
void redfnam(std::wstring& designerName);
void redini();
void relin();
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
void stchsnap(uint32_t start, uint32_t finish) noexcept(!(std::is_same_v<ptrdiff_t, int>));
auto stlen(uint32_t iStitch) -> float;
void strtknt(std::vector<F_POINT_ATTR>& buffer, uint32_t start);
void thrsav();
void unboxs();
void uncros();
auto unthrsh(wchar_t level) noexcept -> float;
void unthum();

auto CALLBACK wndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT;

auto wastch(uint32_t const& formIndex) -> bool;
void zRctAdj() noexcept;

#ifdef ALLOCFAILURE
auto handle_program_memory_depletion(uint32_t) -> int32_t;
#endif
} // namespace thi

namespace {
constexpr auto ARROWPNT = 3U;            // points required to draw arrow
constexpr auto CHSDEF   = 24.0F;         // default chain stitch length
constexpr auto CHRDEF   = 0.25;          // default chain stitch ratio
constexpr auto DEFPIX   = uint16_t {2U}; // default nudge pixels
constexpr auto DEFULEN  = 12.0F;         // default underlay stitch length
constexpr auto DNDLEN   = 256U;          // designer name decoding table length
constexpr auto DNELEN   = 128U;          // designer name encoding table length
constexpr auto DNLEN    = 50U;           // designer name order table length
constexpr auto FLTTHR   = COMDLG_FILTERSPEC {L"Thredworks", L"*.thr"}; // Filter specifications
constexpr auto FLTPCS   = COMDLG_FILTERSPEC {L"Pfaff", L"* .pcs"};
constexpr auto FLTDST   = COMDLG_FILTERSPEC {L"Tajima", L"*.dst"};
constexpr auto FLTPES   = COMDLG_FILTERSPEC {L"Brother", L"*.pes"};
constexpr auto FTYPMASK = uint32_t {0xff000000U}; // top byte mask used for file type verification
constexpr auto HUPS     = int32_t {5};            // number of hoops the user can select
constexpr auto ITXBUFSZ = uint32_t {16U};         // texture buffer depth
constexpr auto KNOTSCNT = 5U;                     // length of knot pattern in stitches
constexpr auto MAXDELAY = int32_t {600};          // maximum movie time step
constexpr auto MINDELAY = int32_t {1};            // minimum movie time step
constexpr auto MINZUM   = int32_t {5};            // minimum zoom in stitch points
constexpr auto MINZUMF  = float {MINZUM};         // minimum zoom in stitch points
constexpr auto NERCNT   = 4U;                     // number of entries in the near array;
constexpr auto NUGINI   = 2.0F;                   // default nudge step
constexpr auto OLDVER   = wchar_t {4};            // number of old file versions kept
constexpr auto PAGSCROL = 0.9F;                   // page scroll factor
constexpr auto QUADRT   = uint32_t {4U};          // split display into quadrants
constexpr auto RES_SIZE = 26;                     // reserved for expansion in the ThrEd v1.0 header
constexpr auto SIGMASK = uint32_t {0x00ffffffU}; // three byte mask used for file signature verification
constexpr auto THREDSIG = uint32_t {0x746872U}; // ThrEd format file signature
constexpr auto TSIZ30   = 0.3F;                 // #30 thread size in millimeters
constexpr auto TSIZ40   = 0.2F;                 // #40 thread size in millimeters
constexpr auto TSIZ60   = 0.05F;                // #60 thread size in millimeters
constexpr auto ZUMFCT   = 0.65F;                // zoom factor
} // namespace

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

class COL_CHANGE
{
  public:
  uint16_t stitchIndex {}; // stitch index
  uint16_t colorIndex {};  // color index

  // constexpr COL_CHANGE() noexcept = default;
  // COL_CHANGE(COL_CHANGE const&) = default;
  // COL_CHANGE(COL_CHANGE&&) = default;
  // COL_CHANGE& operator=(COL_CHANGE const& rhs) = default;
  // COL_CHANGE& operator=(COL_CHANGE&&) = default;
  //~COL_CHANGE() = default;
};

#pragma pack(push, 1)
class THR_HEAD // thred file header
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

  // constexpr THR_HEAD() noexcept = default;
  // THR_HEAD(THR_HEAD const&) = default;
  // THR_HEAD(THR_HEAD&&) = default;
  // THR_HEAD& operator=(THR_HEAD const& rhs) = default;
  // THR_HEAD& operator=(THR_HEAD&&) = default;
  //~THR_HEAD() = default;
};
#pragma pack(pop)

#pragma pack(push, 1)
class THR_HEAD_EX // thred v1.0 file header extension
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

  // constexpr THR_HEAD_EX() noexcept = default;
  // THR_HEAD_EX(THR_HEAD_EX const&) = default;
  // THR_HEAD_EX(THR_HEAD_EX&&) = default;
  // THR_HEAD_EX& operator=(THR_HEAD_EX const& rhs) = default;
  // THR_HEAD_EX& operator=(THR_HEAD_EX&&) = default;
  //~THR_HEAD_EX() = default;
};
#pragma pack(pop)

// main variables
namespace {
auto FormControlPoints =
    gsl::narrow_cast<std::vector<POINT>*>(nullptr); // form control rectangle in pixel coordinates
auto ExtendedHeader  = gsl::narrow_cast<THR_HEAD_EX*>(nullptr);  // thred file header extension
auto DesignerName    = gsl::narrow_cast<std::wstring*>(nullptr); // designer name in clear
auto ArgCount        = int32_t {};                               // command line argument count
auto ArgList         = gsl::narrow_cast<LPTSTR*>(nullptr);       // command line argument array
auto ThredWindowRect = RECT {};                                  // main window size
auto ColorBarSize    = gsl::narrow_cast<int32_t*>(nullptr);      // Color bar width scaled for DPI
auto ColorBarRect    = RECT {};                                  // color bar rectangle
auto HomeDirectory = gsl::narrow_cast<fs::path*>(nullptr); // directory from which thred was executed
auto SmallestStitchIndex = uint32_t {}; // pointer to the smallest stitch in the selected range
auto LargestStitchIndex  = uint32_t {}; // pointer to the largest stitch in the selected range
auto CurrentStitchIndex  = uint32_t {}; // pointer to the current selection for length search
auto ThredDC             = gsl::narrow_cast<HDC>(nullptr); // main device context handle
auto ScreenSizePixels    = SIZE {};                        // screen size in pixels
auto StitchWindowAbsRect = RECT {};                        // stitch window size,absolute
auto NearestPixel        = gsl::narrow_cast<std::vector<POINT>*>(nullptr); // selected points
auto NearestPoint = gsl::narrow_cast<std::vector<uint32_t>*>(nullptr); // indices of the closest points
auto PrevGroupStartStitch    = uint32_t {}; // lower end of previous selection
auto PrevGroupEndStitch      = uint32_t {}; // higher end of previous selection
auto StitchWindowAspectRatio = float {};    // aspect ratio of the stitch window
auto SelectBoxSize           = SIZE {};     // size of the select box
auto SelectBoxOffset = POINT {}; // offset of the spot the user selected from the lower left of the select box
auto RotationHandleAngle = float {};    // angle of the rotation handle
auto ThreadSize30        = TSIZ30;      // #30 thread size
auto ThreadSize40        = TSIZ40;      // #40 thread size
auto ThreadSize60        = TSIZ60;      // #60 thread size
auto RunPoint            = uint32_t {}; // point for animating stitchout
auto StitchesPerFrame    = uint32_t {}; // number of stitches to draw in each frame
auto MovieTimeStep       = int32_t {};  // time delay for stitchout
auto LRUMenuId = std::array<uint32_t, OLDNUM> {FM_ONAM0, FM_ONAM1, FM_ONAM2, FM_ONAM3}; // recently used file menu ID's
auto LastKeyCode = std::numeric_limits<wchar_t>::max(); // last key code
auto VersionNames = gsl::narrow_cast<std::vector<fs::path>*>(nullptr); // temporary storage for old file version names
auto FileVersionIndex = uint8_t {};                     // points to old version to be read
auto StitchArrow      = std::array<POINT, ARROWPNT> {}; // arrow for selected stitch
auto SelectedRange    = RANGE {}; // first and last stitch for min/max stitch select
auto NameOrder        = std::array<uint32_t, DNLEN> {}; // designer name order table
auto NameEncoder      = std::array<uint8_t, DNELEN> {}; // designer name encoding
auto NameDecoder      = std::array<uint8_t, DNDLEN> {}; // designer name decode
auto FirstWin = gsl::narrow_cast<HWND>(nullptr); // first window not destroyed for exiting enumerate loop
auto SelectedFormsRange = RANGE {};              // range of selected forms
auto ZoomMin            = float {};              // minimum allowed zoom value

// Pens
auto LinePen            = gsl::narrow_cast<HPEN>(nullptr); // line pen for stitch move lines
auto BoxPen             = std::array<HPEN, 4> {};          // box pens
auto CrossPen           = gsl::narrow_cast<HPEN>(nullptr); // pen for crosses in color windows
auto GroupSelectPen     = gsl::narrow_cast<HPEN>(nullptr); // pen for group select
auto BackgroundPen      = gsl::narrow_cast<HPEN>(nullptr); // background color pen
auto ZoomMarkPen        = gsl::narrow_cast<HPEN>(nullptr); // zoom mark pen
auto KnotPen            = gsl::narrow_cast<HPEN>(nullptr); // knot pen
auto BackgroundPenWidth = int32_t {};                      // width of the background pen
auto UserPen            = gsl::narrow_cast<std::vector<HPEN>*>(nullptr); // user color pens

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
auto           ThreadSize          = std::array<wchar_t, COLORCNT> {}; // user selected thread sizes
auto           ThreadSizePixels    = std::array<int32_t, COLORCNT> {}; // thread sizes in pixels
auto           ThreadSizeIndex     = std::array<uint32_t, COLORCNT> {}; // thread size indices
auto           ChangeThreadSizeWin = std::array<HWND, 3> {}; // thread size change windows
constexpr auto TSSIZE = ThreadSize.size(); // size of the user selected thread size array

// windows
auto ColorBar       = gsl::narrow_cast<HWND>(nullptr); // color bar
auto SpeedScrollBar = gsl::narrow_cast<HWND>(nullptr); // speed scroll bar for movie
auto BackupViewer = std::array<HWND, QUADRT> {}; // handles of multiple file viewing windows in quadrants
auto DefaultColorWin = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // default color windows
auto UserColorWin    = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // user color windows
auto SideWindow      = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // side message windows
auto ThreadSizeWin   = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // thread size windows

auto StitchWindowBmp = gsl::narrow_cast<HBITMAP>(nullptr); // bitmap for the memory stitch device context
auto DisplayedColorBitmap =
    std::bitset<COLORCNT + COLORCNT> {}; // Map of color numbers in design that are actually displayed
auto RotateAngle      = float {};        // angle for pixel rotate
auto PickColorMsgSize = SIZE {};         // size of the pick color message
auto InsertSize       = SIZE {};         // size of file insert window
auto InsertCenter     = F_POINT {};      // center point in inserted file
auto NumericCode      = wchar_t {};      // keyboard numerical input
auto Knots = gsl::narrow_cast<std::vector<uint32_t>*>(nullptr); // indices of knot stitches

auto SideWindowEntryBuffer =
    gsl::narrow_cast<std::vector<wchar_t>*>(nullptr); // buffer for entering form data sheet numbers
auto SideWinMsgIdx = uint32_t {}; // track current position in SideWindowEntryBuffer
auto MsgBuffer     = gsl::narrow_cast<std::vector<wchar_t>*>(nullptr); // for user messages

// graphics variables

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

auto BoxOffset = std::array<int32_t, 4> {};

auto VerticalIndex = uint8_t {}; // vertical index of the color window, calculated from mouse click
auto DefaultDirectory = gsl::narrow_cast<fs::path*>(nullptr);
auto IniFileName      = gsl::narrow_cast<fs::path*>(nullptr); //.ini file name
auto PreviousNames    = gsl::narrow_cast<std::vector<fs::path>*>(nullptr);
auto Thumbnails = gsl::narrow_cast<std::vector<std::wstring>*>(nullptr); // vector of thumbnail names

auto ThumbnailsSelected = std::array<uint32_t, 4> {}; // indexes of thumbnails selected for display

auto ThumbnailDisplayCount = uint32_t {}; // number of thumbnail file names selected for display
auto ThumbnailIndex        = uint32_t {}; // index into the thumbnail filname table

auto ThumbnailSearchString =
    gsl::narrow_cast<std::vector<wchar_t>*>(nullptr); // storage for the thumbnail search string

auto InsertedVertexIndex = uint32_t {};                  // saved vertex pointer for inserting files
auto InsertedFormIndex   = uint32_t {};                  // saved form pointer for inserting files
auto InsertedStitchIndex = uint32_t {};                  // saved stitch pointer for inserting files
auto MoveLine0           = std::array<POINT, LNPNTS> {}; // move point line
auto MoveLine1           = std::array<POINT, LNPNTS> {}; // move point line
auto ClipInsertBoxLine = std::array<POINT, SQPNTS> {}; // for displaying clipboard insert rectangle
auto RotateBoxOutline  = std::array<POINT, SQPNTS> {}; // for drawing the rotate rectangle
auto RotateBoxCrossVertLine = std::array<POINT, LNPNTS> {}; // vertical part of the rotate cross
auto RotateBoxCrossHorzLine = std::array<POINT, LNPNTS> {}; // horizontal part of the rotate cross
auto RotateBoxToCursorLine = std::array<POINT, LNPNTS> {}; // line from the cursor to the center of the rotate cross
auto ColorChangeTable = gsl::narrow_cast<std::vector<COL_CHANGE>*>(nullptr);
} // namespace

auto CALLBACK thi::dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const hwnd = GetDlgItem(hwndlg, IDC_DESED);
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
		  // NOLINTNEXTLINE(readability-qualified-auto)
		  auto const hwnd           = GetDlgItem(hwndlg, IDC_DESED);
		  auto       designerBuffer = std::array<wchar_t, NameOrder.size()> {};
		  GetWindowText(hwnd, designerBuffer.data(), gsl::narrow<int>(designerBuffer.size()));
		  DesignerName->assign(designerBuffer.data());
		  EndDialog(hwndlg, 0);
		  auto const fmtStr = displayText::format(IDS_THRED, *DesignerName);
		  SetWindowText(ThrEdWindow, fmtStr.c_str());
		  return TRUE;
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

void thi::shownd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_SHOW);
}

void thi::hidwnd(HWND hwnd) noexcept {
  ShowWindow(hwnd, SW_HIDE);
}

void thred::hideColorWin() noexcept {
  auto iDefaultColorWin = DefaultColorWin->begin();
  auto iUserColorWin    = UserColorWin->begin();
  for (auto const& iThreadSizeWin : *ThreadSizeWin) {
	thi::hidwnd(*(iDefaultColorWin++));
	thi::hidwnd(*(iUserColorWin++));
	thi::hidwnd(iThreadSizeWin);
  }
}

void thred::showColorWin() noexcept {
  auto iDefaultColorWin = DefaultColorWin->begin();
  auto iUserColorWin    = UserColorWin->begin();
  for (auto const& iThreadSizeWin : *ThreadSizeWin) {
	thi::shownd(*(iDefaultColorWin++));
	thi::shownd(*(iUserColorWin++));
	thi::shownd(iThreadSizeWin);
  }
}

auto thred::getUserPen(uint32_t iPen) noexcept -> HPEN {
  return UserPen->operator[](iPen);
}

void thred::resetSideBuffer() {
  SideWinMsgIdx           = 0;
  constexpr auto FILLCHAR = gsl::narrow_cast<wchar_t>(0U);
  std::ranges::fill(*SideWindowEntryBuffer, FILLCHAR);
}

void thred::resetMsgBuffer() {
  MsgBuffer->clear();
  MsgBuffer->push_back(0);
}

auto thred::getMsgBufferValue() -> float {
  return wrap::wcsToFloat(MsgBuffer->data());
}

void thred::getdes() noexcept {
  // ToDo - don't update values in DialogBox as then 'cancel' does not work
  // ReSharper disable CppClangTidyClangDiagnosticCastFunctionTypeStrict
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DESNAM), ThrEdWindow, reinterpret_cast<DLGPROC>(thi::dnamproc)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, clang-diagnostic-cast-function-type-strict)
  // ReSharper restore CppClangTidyClangDiagnosticCastFunctionTypeStrict
}

auto thi::stlen(uint32_t iStitch) -> float {
  auto const currStitch = wrap::next(StitchBuffer->begin(), iStitch);
  auto const nextStitch = std::next(currStitch);
  return std::hypot(nextStitch->x - currStitch->x, nextStitch->y - currStitch->y);
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

auto thi::rsed() noexcept -> uint32_t {
  auto time = SYSTEMTIME {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
  GetLocalTime(&time);
  return (gsl::narrow_cast<uint32_t>(time.wSecond) << WRDSHFT) | time.wMilliseconds;
}

void thi::fnamtabs() {
  constexpr auto NORDSED = uint32_t {0x5a5a5a5aU}; // name order seed
  constexpr auto NCODSED = uint32_t {0x73ef5a7eU}; // name encoding seed
  constexpr auto NCODOF  = uint8_t {80U};          // name encoding offset
  constexpr auto MAX8B   = uint8_t {127};          // max value of a signed byte
  constexpr auto NDXOFF  = uint8_t {32U};          // index offset
  constexpr auto NAMELEN = NameOrder.size();
  std::iota(NameOrder.begin(), NameOrder.end(), 0);
  PseudoRandomValue = NORDSED;
  for (auto iName = 0U; iName < 2 * NAMELEN; ++iName) {
	auto const source      = wrap::next(NameOrder.begin(), form::psg() % NAMELEN);
	auto const destination = wrap::next(NameOrder.begin(), form::psg() % NAMELEN);
	std::swap(*destination, *source);
  }
  std::iota(NameEncoder.begin(), NameEncoder.end(), NCODOF);
  PseudoRandomValue = NCODSED;
  for (auto iName = 0U; iName < 2 * NameEncoder.size(); ++iName) {
	auto const source      = wrap::next(NameEncoder.begin(), form::psg() & MSK7BITS);
	auto const destination = wrap::next(NameEncoder.begin(), form::psg() & MSK7BITS);
	std::swap(*destination, *source);
  }
  NameDecoder.fill(0);
  auto index = wrap::next(NameEncoder.begin(), NDXOFF);
  for (auto iName = NDXOFF; iName < uint8_t {MAX8B}; ++iName) {
	auto iND = wrap::next(NameDecoder.begin(), *(index++));
	*iND     = iName;
  }
}

void thi::ritfnam(std::wstring const& designerName) {
  constexpr auto NAMELEN  = NameOrder.size();
  auto const     designer = utf::utf16ToUtf8(designerName);
  auto           tmpName  = std::array<uint8_t, NameOrder.size()> {};
  if (NameOrder[0] > NameOrder.size()) {
	fnamtabs();
  }
  PseudoRandomValue = rsed();
  auto iName        = 0U;
  std::ranges::generate(tmpName, []() noexcept -> uint8_t { return (form::psg() & BYTMASK); });
  for (auto& iTmpName : tmpName) {
	if (designer[iName++] != 0) {
	  auto iNE = wrap::next(NameEncoder.begin(), designer[iName]);
	  iTmpName = *iNE;
	  continue;
	}
	auto iND = wrap::next(NameDecoder.begin(), iTmpName);
	while (*iND != 0U) {
	  iTmpName = (form::psg() & BYTMASK);
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
  auto       iTmpName      = tmpName.begin();
  auto const spCreatorName = gsl::span {ExtendedHeader->creatorName};
  for (auto const& iNameOrder : NameOrder) {
	if (iNameOrder < NAMELEN) {
	  spCreatorName[iNameOrder] = gsl::narrow_cast<char>(*(iTmpName++));
	}
  }
}

void thi::redfnam(std::wstring& designerName) {
  constexpr auto NNN           = uint8_t {111U}; // string substitute character
  auto           tmpName       = std::array<uint8_t, NameOrder.size()> {};
  auto           designer      = std::string {};
  auto           iNameOrder    = NameOrder.begin();
  auto const     spCreatorName = gsl::span {ExtendedHeader->creatorName};
  for (auto& iTmpName : tmpName) {
	auto const& index = *iNameOrder;
	if (index < spCreatorName.size()) {
	  iTmpName = gsl::narrow_cast<uint8_t>(spCreatorName[index]);
	}
	else {
	  iTmpName = NNN;
	}
	++iNameOrder;
  }
  designer.reserve(tmpName.size());
  for (auto const& character : tmpName) {
	auto iND = wrap::next(NameDecoder.begin(), character);
	if (*iND != 0U) {
	  designer.push_back(gsl::narrow<char>(*iND));
	}
	else {
	  break;
	}
  }
  auto const decoded = utf::utf8ToUtf16(designer);
  designerName.assign(decoded);
}

auto thred::adflt(uint32_t count) -> uint32_t {
  auto const startVertex = wrap::toUnsigned(FormVertices->size());
  auto const itPoint     = FormVertices->end();
  auto constexpr VAL     = F_POINT {};
  FormVertices->insert(itPoint, count, VAL);
  return startVertex;
}

auto thred::adclp(uint32_t count) -> uint32_t {
  auto const iClipPoint = wrap::toUnsigned(ClipPoints->size());
  auto const itPoint    = ClipPoints->end();
  auto constexpr VAL    = F_POINT {};
  ClipPoints->insert(itPoint, count, VAL);
  return iClipPoint;
}

auto thred::duthrsh(float threshold) noexcept -> uint32_t {
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

auto thi::unthrsh(wchar_t level) noexcept -> float {
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

void thred::ritfcor(F_POINT const& point) {
  auto const fmtStr = fmt::format(FMT_COMPILE(L"x{:.0f} y{:.0f}"), (point.x * IPFGRAN), (point.y * IPFGRAN));
  displayText::butxt(HCOR, fmtStr);
}

void thi::ritcor(F_POINT_ATTR const& pointAttribute) {
  auto const point = F_POINT {pointAttribute.x, pointAttribute.y};
  thred::ritfcor(point);
}

void thi::fndknt() {
  auto endStitch = wrap::toUnsigned(StitchBuffer->size());
  if (endStitch <= 4U) {
	Knots->clear();
	return;
  }
  endStitch -= 4U;
  Knots->clear();
  auto spStitch = std::ranges::subrange(StitchBuffer->begin(), wrap::next(StitchBuffer->begin(), endStitch));
  for (auto iStitch = 0; const auto& stitch : spStitch) {
	if ((stitch.attribute & KNOTMSK) != 0U) {
	  Knots->emplace_back(iStitch);
	}
	iStitch++;
  }
}

auto thred::maxColor() noexcept -> size_t {
  if (ColorChangeTable->empty()) {
	return 0U;
  }
  return ColorChangeTable->size() - 1U;
}

void thred::resetColorChanges() noexcept {
  ColorChangeTable->clear();
}

void thred::addColor(uint32_t stitch, uint32_t color) {
  ColorChangeTable->emplace_back(
      COL_CHANGE {gsl::narrow<decltype(ColorChangeTable->back().stitchIndex)>(stitch),
                  gsl::narrow<decltype(ColorChangeTable->back().colorIndex)>(color)});
}

void thred::coltab() {
  thred::resetColorChanges();
  if (StitchBuffer->size() <= 1) {
	return;
  }
  auto const firstStitch  = StitchBuffer->begin();
  auto       currentColor = (firstStitch + 1)->attribute & COLMSK;
  firstStitch->attribute &= NCOLMSK;
  firstStitch->attribute |= currentColor;
  auto const lastStitch = StitchBuffer->rbegin();
  lastStitch->attribute &= NCOLMSK;
  lastStitch->attribute |= (lastStitch + 1)->attribute & COLMSK;

  auto const pEnd = std::prev(StitchBuffer->end());
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
  for (auto iStitch = 0U; auto& stitch : *StitchBuffer) {
	stitch.x = std::clamp(stitch.x, minX, maxX);
	stitch.y = std::clamp(stitch.y, minY, maxY);

	auto const nextColor = stitch.attribute & COLMSK;
	if (currentColor != nextColor) {
	  addColor(iStitch, nextColor);
	  currentColor = nextColor;
	}
	++iStitch;
  }
  addColor(wrap::toUnsigned(StitchBuffer->size()), 0);
  ClosestPointIndex = std::min(ClosestPointIndex, wrap::toUnsigned(StitchBuffer->size() - 1U));
  thi::fndknt();
}

void thred::savdo() {
  StateMap->set(StateFlag::WASDO);
  StateMap->set(StateFlag::CMPDO);
  if (!StateMap->testAndReset(StateFlag::SAVACT)) {
	return;
  }
  if (StateMap->testAndReset(StateFlag::BAKING)) {
	menu::disableRedo();
  }
  StateMap->set(StateFlag::BAKACT);
  menu::enableUndo();
  backup::dudat();
  backup::updateWriteIndex();
}

void thi::nunams() {
  *AuxName = *WorkingFileName;
  xt::duauxnam(*AuxName);
  *ThrName = *WorkingFileName;
  ThrName->replace_extension(L".thr");
  if (PreviousNames->front() == *ThrName) {
	return;
  }
  auto flag = true;
  for (auto spNames = std::ranges::subrange(std::next(PreviousNames->begin()), PreviousNames->end());
       auto& previousName : spNames) {
	if (previousName != *ThrName) {
	  continue;
	}
	std::swap(PreviousNames->front(), previousName);
	flag = false;
	break;
  }
  if (!flag) {
	menu::redfils(LRUPtr, PreviousNames);
	return;
  }
  for (auto& previousName : *PreviousNames) {
	if (!previousName.empty()) {
	  continue;
	}
	previousName.assign(*ThrName);
	flag = false;
	break;
  }
  if (flag) {
	PreviousNames->insert(PreviousNames->begin(), *ThrName);
	PreviousNames->pop_back();
  }
  menu::redfils(LRUPtr, PreviousNames);
}

void thred::rngadj() noexcept(std::is_same_v<size_t, uint32_t>) {
  auto lastStitch = wrap::toUnsigned(StitchBuffer->size());
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

void thi::box(uint32_t iNearest, HDC hDC) {
  auto const itBoxWidth     = wrap::next(BoxOffset.begin(), iNearest);
  auto const boxWidth       = *itBoxWidth;
  auto const itNearestPixel = wrap::next(NearestPixel->begin(), iNearest);
  auto const npx            = itNearestPixel->x;
  auto const npy            = itNearestPixel->y;
  auto       line           = std::array<POINT, SQPNTS> {};

  line[0] = {npx - boxWidth, npy - boxWidth};
  line[1] = {npx + boxWidth, npy - boxWidth};
  line[2] = {npx + boxWidth, npy + boxWidth};
  line[3] = {npx - boxWidth, npy + boxWidth};
  line[4] = {npx - boxWidth, npy - boxWidth};
  wrap::polyline(hDC, line.data(), wrap::toUnsigned(line.size()));
}

void thi::boxs() {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  auto itBoxPen = BoxPen.begin();
  for (auto iNear = 0U; iNear < NearestCount; ++iNear) {
	SelectObject(StitchWindowDC, *(itBoxPen++));
	box(iNear, StitchWindowDC);
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thi::unboxs() {
  if (NearestCount != 0U) {
	boxs();
	NearestCount = 0;
  }
}

void thi::stchPars() {
  auto const aspectRatio = wrap::toFloat(UnzoomedRect.cx) / wrap::toFloat(UnzoomedRect.cy);
  StitchWindowSize.cx =
      (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT))
          ? std::lround(wrap::toFloat(ThredWindowRect.bottom - ((*ScrollSize) * 2)) * aspectRatio)
          : std::lround(wrap::toFloat(ThredWindowRect.bottom - *ScrollSize) * aspectRatio);

  if ((StitchWindowSize.cx + ButtonWidthX3 + *ScrollSize + *ColorBarSize) < ThredWindowRect.right) {
	StitchWindowSize.cy = (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT))
	                          ? ThredWindowRect.bottom - (*ScrollSize * 2)
	                          : ThredWindowRect.bottom - *ScrollSize;
  }
  else {
	StitchWindowSize = {ThredWindowRect.right - ButtonWidthX3 - *ColorBarSize,
	                    ThredWindowRect.bottom - ThredWindowRect.top};
	auto const sizeX = wrap::toFloat(StitchWindowSize.cx);
	auto const sizeY = wrap::toFloat(StitchWindowSize.cy);
	if ((sizeX / sizeY) > aspectRatio) {
	  StitchWindowSize.cx = std::lround(sizeY * aspectRatio);
	}
	else {
	  StitchWindowSize.cy = std::lround(sizeX / aspectRatio);
	}
  }
}

void thred::redrawColorBar() noexcept {
  RedrawWindow(ColorBar, nullptr, nullptr, RDW_INVALIDATE);
}

void thred::redraw(HWND window) noexcept {
  RedrawWindow(window, nullptr, nullptr, RDW_INVALIDATE);
  if (window != MainStitchWin) {
	return;
  }
  for (auto const& iWindow : *DefaultColorWin) {
	if (iWindow != nullptr) {
	  RedrawWindow(iWindow, nullptr, nullptr, RDW_INVALIDATE);
	}
  }
  RedrawWindow(ColorBar, nullptr, nullptr, RDW_INVALIDATE);
}

void thi::nuRct() noexcept {
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
  if ((StitchWindowMemDC != nullptr) && (StitchWindowBmp != nullptr)) {
	SelectObject(StitchWindowMemDC, StitchWindowBmp);
  }
}

void thred::movStch() {
  auto clientSize = SIZE {(ThredWindowRect.right - ButtonWidthX3 - (*ScrollSize + *ColorBarSize)),
                          (ThredWindowRect.bottom)};
  auto verticalOffset = 0;
  thi::unboxs();
  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
	verticalOffset = *ScrollSize;
	clientSize.cy -= *ScrollSize;
  }
  if (StateMap->test(StateFlag::ZUMED)) {
	clientSize.cy -= *ScrollSize;
	MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, clientSize.cx, clientSize.cy, FALSE);
	MoveWindow(VerticalScrollBar, ButtonWidthX3 + clientSize.cx, 0, *ScrollSize, clientSize.cy, TRUE);
	MoveWindow(HorizontalScrollBar, ButtonWidthX3, clientSize.cy + verticalOffset, clientSize.cx, *ScrollSize, TRUE);
	StitchWindowAspectRatio = wrap::toFloat(clientSize.cx) / wrap::toFloat(clientSize.cy);
	if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
	  MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, clientSize.cx, *ScrollSize, TRUE);
	}
	ShowWindow(VerticalScrollBar, SW_SHOWNORMAL);
	ShowWindow(HorizontalScrollBar, SW_SHOWNORMAL);
  }
  else {
	thi::stchPars();
	auto const actualWindowHeight = StitchWindowSize.cy + *ScrollSize;
	MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, StitchWindowSize.cx, actualWindowHeight, TRUE);
	ShowWindow(VerticalScrollBar, SW_HIDE);
	ShowWindow(HorizontalScrollBar, SW_HIDE);
	StitchWindowAspectRatio = wrap::toFloat(StitchWindowSize.cx) / wrap::toFloat(actualWindowHeight);
	if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
	  MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, StitchWindowSize.cx, *ScrollSize, TRUE);
	}
  }
  MoveWindow(ColorBar, ThredWindowRect.right - *ColorBarSize, 0, *ColorBarSize, ThredWindowRect.bottom, TRUE);
  thi::nuRct();
  thred::redrawColorBar();
}

void thi::destroyBV() noexcept {
  for (auto& iBackup : BackupViewer) {
	DestroyWindow(iBackup);
	iBackup = nullptr;
  }
}

void thred::unbsho() {
  if (!StateMap->testAndReset(StateFlag::BAKSHO)) {
	return;
  }
  thi::destroyBV();
}

void thred::unsid() noexcept {
  FormMenuChoice = 0;
  if (SideMessageWindow == nullptr) {
	return;
  }
  DestroyWindow(SideMessageWindow);
  SideMessageWindow = nullptr;
}

void thi::insadj() {
  StateMap->reset(StateFlag::PRELIN);
  form::refil(ClosestFormToCursor);
  thred::coltab();
  FormList->operator[](ClosestFormToCursor).outline();
}

void thi::patdun() {
  StateMap->reset(StateFlag::RUNPAT);
  KillTimer(ThrEdWindow, 0);
  StateMap->set(StateFlag::WASPAT);
  thred::movStch();
  StateMap->set(StateFlag::RESTCH);
  displayText::tabmsg(IDS_END, false);
}

auto thi::pxchk(float pixelSize) -> uint16_t {
  constexpr auto MINLIMIT = 1.0F;
  constexpr auto MAXLIMIT = 20.0F;

  pixelSize = std::clamp(pixelSize, MINLIMIT, MAXLIMIT);
  return wrap::round<uint16_t>(pixelSize);
}

void thi::zRctAdj() noexcept {
  auto const unzoomedY = wrap::toFloat(UnzoomedRect.cy);
  if (ZoomRect.top > unzoomedY) {
	auto const adjustment = ZoomRect.top - unzoomedY;
	ZoomRect.top -= adjustment;
	ZoomRect.bottom -= adjustment;
  }
  if (ZoomRect.bottom < 0) {
	ZoomRect.top -= ZoomRect.bottom;
	ZoomRect.bottom = 0.0F;
  }
  auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx);
  if (ZoomRect.right > unzoomedX) {
	auto const adjustment = ZoomRect.right - unzoomedX;
	ZoomRect.right -= adjustment;
	ZoomRect.left -= adjustment;
  }
  if (ZoomRect.left < 0) {
	ZoomRect.right -= ZoomRect.left;
	ZoomRect.left = 0.0F;
  }
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
  thi::zRctAdj();
}

auto thred::stch2px1(uint32_t iStitch) -> POINT {
  if (StitchBuffer->empty()) {
	return POINT {0L, StitchWindowClientRect.bottom};
  }
  auto const current = wrap::next(StitchBuffer->begin(), iStitch);
  return POINT {wrap::ceil<int32_t>((current->x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    (current->y - ZoomRect.bottom) * ZoomRatio.y)};
}

void thi::shft2box() {
  if (StitchBuffer->empty()) {
	return;
  }
  auto const current     = wrap::next(StitchBuffer->begin(), ClosestPointIndex);
  auto const stitchPoint = F_POINT {current->x, current->y};
  thred::shft(stitchPoint);
  thred::stch2px1(ClosestPointIndex);
}

void thred::unbox() {
  if (!StateMap->testAndReset(StateFlag::SELBOX)) {
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
  if (StateMap->testAndReset(StateFlag::ILIN)) {
	thred::ilin();
  }
}

void thred::ilin1() noexcept {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::xlin1() {
  if (StateMap->testAndReset(StateFlag::ILIN1)) {
	thred::ilin1();
  }
}

void thred::endpnt(POINT const& stitchCoordsInPixels) {
  thred::unbox();
  thred::xlin();
  thred::xlin1();
  StateMap->set(StateFlag::LIN1);
  StateMap->set(StateFlag::INSRT);
  StateMap->reset(StateFlag::GRPSEL);
  InsertLine[0] = stitchCoordsInPixels;
  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
}

void thred::duIns() {
  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
  InsertLine[0] = thred::stch2px1(ClosestPointIndex);
  InsertLine[2] = thred::stch2px1(ClosestPointIndex + 1U);
  thred::xlin();
  StateMap->set(StateFlag::ILIN);
  thred::ilin();
}

void thi::movins() {
  if (!StateMap->test(StateFlag::INSRT)) {
	return;
  }
  if (StateMap->test(StateFlag::LIN1)) {
	(StateMap->test(StateFlag::BAKEND))
	    ? thred::endpnt(thred::stch2px1(wrap::toUnsigned(StitchBuffer->size() - 1U)))
	    : thred::endpnt(thred::stch2px1(0U));
  }
  else {
	thred::duIns();
  }
}

void thred::zumhom() {
  ZoomRect = F_RECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.cy), wrap::toFloat(UnzoomedRect.cx), 0.0F};
  ZoomFactor = 1;
  StateMap->reset(StateFlag::ZUMED);
  thred::movStch();
  NearestCount = 0;
  if (StateMap->test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  StateMap->set(StateFlag::RESTCH);
  if (StateMap->test(StateFlag::SELBOX)) {
	thi::shft2box();
  }
  thred::duzrat();
  thi::movins();
}

void thred::hupfn() {
  auto checkHoopRect = F_RECTANGLE {}; // for checking the hoop size
  StateMap->reset(StateFlag::HUPCHNG);
  thred::stchrct(checkHoopRect);
  if (!FormList->empty()) {
	if (StitchBuffer->empty()) {
	  auto const itVertex  = FormVertices->cbegin();
	  checkHoopRect.top    = itVertex->y;
	  checkHoopRect.bottom = itVertex->y;
	  checkHoopRect.right  = itVertex->x;
	  checkHoopRect.left   = itVertex->x;
	}
	for (auto const& formVertice : *FormVertices) {
	  checkHoopRect.left   = std::min(checkHoopRect.left, formVertice.x);
	  checkHoopRect.right  = std::max(checkHoopRect.right, formVertice.x);
	  checkHoopRect.bottom = std::min(checkHoopRect.bottom, formVertice.y);
	  checkHoopRect.top    = std::max(checkHoopRect.top, formVertice.y);
	}
  }
  if (StitchBuffer->empty() && FormVertices->empty() && !StateMap->test(StateFlag::HUPEX)) {
	return;
  }
  if (checkHoopRect.left < 0 || checkHoopRect.right > IniFile.hoopSizeX ||
      checkHoopRect.bottom < 0 || checkHoopRect.top > IniFile.hoopSizeY) {
	StateMap->set(StateFlag::HUPEX);
  }
  if (!StateMap->test(StateFlag::HUPEX)) {
	return;
  }
  auto const hoopSize =
      F_POINT {checkHoopRect.right - checkHoopRect.left, checkHoopRect.top - checkHoopRect.bottom};
  if (hoopSize.x > IniFile.hoopSizeX) {
	IniFile.hoopSizeX = hoopSize.x;
	StateMap->set(StateFlag::HUPCHNG);
  }
  if (hoopSize.y > IniFile.hoopSizeY) {
	IniFile.hoopSizeY = hoopSize.y;
	StateMap->set(StateFlag::HUPCHNG);
  }
  auto const designCenter =
      F_POINT {hoopSize.x * 0.5F + checkHoopRect.left, hoopSize.y * 0.5F + checkHoopRect.bottom};
  auto const hoopCenter = F_POINT {IniFile.hoopSizeX * 0.5F, IniFile.hoopSizeY * 0.5F};
  auto const delta      = F_POINT {hoopCenter.x - designCenter.x, hoopCenter.y - designCenter.y};
  for (auto& stitch : *StitchBuffer) {
	stitch += delta;
  }
  for (auto& formVertice : *FormVertices) {
	formVertice += delta;
  }
  for (auto& iForm : *FormList) {
	iForm.rectangle.left += delta.x;
	iForm.rectangle.top += delta.y;
	iForm.rectangle.right += delta.x;
	iForm.rectangle.bottom += delta.y;
  }
  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
  ZoomMin      = MINZUMF / wrap::toFloat(UnzoomedRect.cx);
  thred::zumhom();
}

void thred::chkhup() {
  StateMap->set(StateFlag::HUPEX);
  thred::hupfn();
  if (StateMap->test(StateFlag::INIT)) {
	formForms::prfmsg();
  }
  xt::setfchk();
}

void thi::setSideWinVal(int index) {
  SetWindowText(ValueWindow->operator[](wrap::toSize(index)), SideWindowEntryBuffer->data());
}

void thi::chknum() {
  xt::clrstch();
  if (SideWinMsgIdx != 0U) {
	if (FormMenuChoice != 0U) {
	  auto& form = FormList->operator[](ClosestFormToCursor);

	  auto const value = wrap::wcsToFloat(SideWindowEntryBuffer->data()) * PFGRAN;
	  switch (FormMenuChoice) {
		case LTXOF: {
		  thred::savdo();
		  form.txof = value;
		  break;
		}
		case LUANG: {
		  thred::savdo();
		  form.underlayStitchAngle = value * DEGRADF * IPFGRAN;
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
		  form.fillStart = wrap::round<uint32_t>(value * IPFGRAN);
		  form.fillStart %= form.vertexCount;
		  break;
		}
		case LDEND: {
		  thred::savdo();
		  form.fillEnd = wrap::round<uint32_t>(value * IPFGRAN);
		  form.fillEnd %= form.vertexCount;
		  break;
		}
		case LFTHUPCNT: {
		  thred::savdo();
		  auto           upcnt    = value * IPFGRAN;
		  constexpr auto FUPCLAMP = 255.0F; // clamp the feather up count

		  upcnt                = std::min(upcnt, FUPCLAMP);
		  form.feather.upCount = wrap::round<uint8_t>(upcnt);
		  break;
		}
		case LFTHCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			form::nufthcol((wrap::wcsToLong<uint32_t>(SideWindowEntryBuffer->data()) - 1U) & COLMSK);
			thi::setSideWinVal(LFTHCOL);
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap->set(StateFlag::RESTCH);
		  return;
		}
		case LFRMCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			auto const colVal = gsl::narrow_cast<uint8_t>(
			    (wrap::wcsToLong<uint32_t>(SideWindowEntryBuffer->data()) - 1U) & COLMSK);
			form::nufilcol(colVal);
			auto const fmtStr = fmt::format(FMT_COMPILE(L"{}"), colVal + 1U);
			SetWindowText(ValueWindow->operator[](LFRMCOL), fmtStr.c_str());
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap->set(StateFlag::RESTCH);
		  return;
		}
		case LUNDCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			auto const colVal = gsl::narrow_cast<uint8_t>(
			    (wrap::wcsToLong<uint32_t>(SideWindowEntryBuffer->data()) - 1U) & COLMSK);
			form.underlayColor = colVal;
			auto const fmtStr  = fmt::format(FMT_COMPILE(L"{}"), colVal + 1U);
			SetWindowText(ValueWindow->operator[](LUNDCOL), fmtStr.c_str());
			form::refilfn(ClosestFormToCursor);
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap->set(StateFlag::RESTCH);
		  return;
		}
		case LBRDCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			auto const colVal = gsl::narrow_cast<uint8_t>(
			    (wrap::wcsToLong<uint32_t>(SideWindowEntryBuffer->data()) - 1U) & COLMSK);
			form::nubrdcol(colVal);
			auto const fmtStr = fmt::format(FMT_COMPILE(L"{}"), colVal + 1U);
			SetWindowText(ValueWindow->operator[](LBRDCOL), fmtStr.c_str());
			thred::coltab();
		  }
		  thred::unsid();
		  StateMap->set(StateFlag::RESTCH);
		  return;
		}
		case LBRDPIC: {
		  thred::savdo();
		  form.edgeSpacing = value;
		  thred::unsid();
		  thi::setSideWinVal(LBRDPIC);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LFRMFAZ: {
		  thred::savdo();
		  form.wordParam = wrap::floor<uint32_t>(value * IPFGRAN);
		  thred::unsid();
		  thi::setSideWinVal(LFRMFAZ);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LBRDPOS: {
		  thred::savdo();
		  form.edgeStitchLen = value * IPFGRAN;
		  thred::unsid();
		  thi::setSideWinVal(LBRDPOS);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMAXFIL: {
		  thred::savdo();
		  form.maxFillStitchLen = value;
		  thred::unsid();
		  thi::setSideWinVal(LMAXFIL);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMINFIL: {
		  thred::savdo();
		  form.minFillStitchLen = value;
		  thred::unsid();
		  thi::setSideWinVal(LMINFIL);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMAXBRD: {
		  thred::savdo();
		  form.maxBorderStitchLen = value;
		  thred::unsid();
		  thi::setSideWinVal(LMAXBRD);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		case LMINBRD: {
		  thred::savdo();
		  form.minBorderStitchLen = value;
		  thred::unsid();
		  thi::setSideWinVal(LMINBRD);
		  form::refil(ClosestFormToCursor);
		  return;
		}
		default: {
		  outDebugString(L"default hit in chknum 1: FormMenuChoice [{}]\n", FormMenuChoice);
		  break;
		}
	  }
	  if (FormMenuChoice == LBCSIZ) {
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
			case LFTHSIZ: {
			  thred::savdo();
			  form.feather.ratio = value * IPFGRAN;
			  break;
			}
			case LFTHNUM: {
			  thred::savdo();
			  form.feather.count = wrap::round<uint16_t>(value * IPFGRAN);
			  break;
			}
			case LFTHFLR: {
			  thred::savdo();
			  form.feather.minStitchSize = value;
			  break;
			}
			case LFTHDWNCNT: {
			  thred::savdo();
			  auto           dncnt    = value * IPFGRAN;
			  constexpr auto FDNCLAMP = 255.0F; // clamp the feather down count

			  dncnt                  = std::min(dncnt, FDNCLAMP);
			  form.feather.downCount = wrap::round<uint8_t>(dncnt);
			  break;
			}
			case LFRMSPAC: {
			  thred::savdo();
			  form.fillSpacing = value;
			  break;
			}
			case LFRMLEN: {
			  thred::savdo();
			  form.stitchLength = value;
			  break;
			}
			case LBRDSPAC: {
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
			  form.fillAngle = value * DEGRADF * IPFGRAN;
			  break;
			}
			case LSACANG: {
			  thred::savdo();
			  form.clipFillAngle = value * DEGRADF * IPFGRAN;
			  break;
			}
			case LAPCOL: {
			  thred::savdo();
			  form.borderColor &= COLMSK;
			  auto borderColor = wrap::round<uint8_t>(value * IPFGRAN);
			  if (borderColor != 0U) {
				--borderColor;
			  }
			  borderColor &= COLMSK;
			  form.borderColor |= gsl::narrow_cast<decltype(form.borderColor)>((borderColor) << 4U);
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
		  if (FormMenuChoice == LFRMSPAC && form.isFanClip()) {
			form.fillSpacing = 0;
		  }
		}
	  }
	  thred::unsid();
	  form::refil(ClosestFormToCursor);
	  formForms::refrm();
	}
	else {
	  if (PreferenceIndex != 0U) {
		auto const value = wrap::wcsToFloat(SideWindowEntryBuffer->data());
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto hWnd   = HWND {nullptr};
		auto fmtStr = std::wstring {};
		switch (PreferenceIndex - 1) {
		  case PRFEGGRAT: {
			IniFile.eggRatio = value;
			fmtStr           = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd             = ValueWindow->operator[](PRFEGGRAT);
			break;
		  }
		  case PRFNUGSTP: {
			IniFile.cursorNudgeStep = value;
			IniFile.nudgePixels     = pxchk(value);
			fmtStr                  = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd                    = ValueWindow->operator[](PRFNUGSTP);
			break;
		  }
		  case PRFPCTSPC: {
			PicotSpacing = value * PFGRAN;
			fmtStr       = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd         = ValueWindow->operator[](PRFPCTSPC);
			break;
		  }
		  case PRFCLPOFF: {
			IniFile.clipOffset = value * PFGRAN;
			fmtStr             = fmt::format(FMT_COMPILE(L"{:.2f} mm"), value);
			hWnd               = ValueWindow->operator[](PRFCLPOFF);
			break;
		  }
		  case PRFCLPPHS: {
			IniFile.fillPhase = wrap::floor<uint32_t>(value);
			fmtStr            = fmt::format(FMT_COMPILE(L"{}"), IniFile.fillPhase);
			hWnd              = ValueWindow->operator[](PRFCLPPHS);
			break;
		  }
		  case PRFCHFPOS: {
			IniFile.chainRatio = value;
			fmtStr             = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd               = ValueWindow->operator[](PRFCHFPOS);
			break;
		  }
		  case PRFSTCMIN: {
			MinStitchLength = value * PFGRAN;
			fmtStr          = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
			hWnd            = ValueWindow->operator[](PRFSTCMIN);
			break;
		  }
		  default: {
			if (value != 0.0F) {
			  fmtStr = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
			  switch (PreferenceIndex - 1) {
				case PRFFILSPC: {
				  LineSpacing = value * PFGRAN;
				  hWnd        = ValueWindow->operator[](PRFFILSPC);
				  break;
				}
				case PRFFILANG: {
				  IniFile.fillAngle = value * DEGRADF;
				  hWnd              = ValueWindow->operator[](PRFFILANG);
				  break;
				}
				case PRFBRDWID: {
				  BorderWidth         = value * PFGRAN;
				  IniFile.borderWidth = BorderWidth;
				  hWnd                = ValueWindow->operator[](PRFBRDWID);
				  break;
				}
				case PRFSTCMAX: {
				  IniFile.maxStitchLength = value * PFGRAN;
				  hWnd                    = ValueWindow->operator[](PRFSTCMAX);
				  break;
				}
				case PRFSTCUSR: {
				  UserStitchLength = value * PFGRAN;
				  hWnd             = ValueWindow->operator[](PRFSTCUSR);
				  break;
				}
				case PRFSMLSTH: {
				  SmallStitchLength = value * PFGRAN;
				  hWnd              = ValueWindow->operator[](PRFSMLSTH);
				  break;
				}
				case PRFAPPCOL: {
				  AppliqueColor = wrap::round<uint32_t>(value - 1.0F) % COLORCNT;
				  fmtStr        = fmt::format(FMT_COMPILE(L"{}"), (AppliqueColor + 1U));
				  hWnd          = ValueWindow->operator[](PRFAPPCOL);
				  break;
				}
				case PRFAPSLEN: {
				  IniFile.AppStitchLen = value * PFGRAN;
				  hWnd                 = ValueWindow->operator[](PRFAPSLEN);
				  break;
				}
				case PRFSNPSIZ: {
				  SnapLength = value * PFGRAN;
				  hWnd       = ValueWindow->operator[](PRFSNPSIZ);
				  break;
				}
				case PRFSTRRAT: {
				  StarRatio = value;

				  constexpr auto SRMINLIM = 0.01F; // star ratio minimum limit
				  constexpr auto SRMAXLIM = 1.0F;  // star ratio maximum limit

				  StarRatio = std::clamp(StarRatio, SRMINLIM, SRMAXLIM);
				  fmtStr    = fmt::format(FMT_COMPILE(L"{:.2f}"), StarRatio);
				  hWnd      = ValueWindow->operator[](PRFSTRRAT);
				  break;
				}
				case PRFLENRAT: {
				  IniFile.lensRatio = value;

				  constexpr auto LRMINLIM = 0.1F;  // lens ratio minimum limit
				  constexpr auto LRMAXLIM = 10.0F; // lens ratio maximum limit

				  IniFile.lensRatio = std::clamp(IniFile.lensRatio, LRMINLIM, LRMAXLIM);
				  fmtStr            = fmt::format(FMT_COMPILE(L"{:.2f}"), IniFile.lensRatio);
				  hWnd              = ValueWindow->operator[](PRFLENRAT);
				  break;
				}
				case PRFSPLWRP: {
				  SpiralWrap = value;
				  // ToDo - Are these limits correct?
				  constexpr auto SRMINLIM = 0.3F;  // spiral wrap minimum limit
				  constexpr auto SRMAXLIM = 20.0F; // spiral wrap maximum limit

				  SpiralWrap = std::clamp(SpiralWrap, SRMINLIM, SRMAXLIM);
				  fmtStr     = fmt::format(FMT_COMPILE(L"{:.2f}"), SpiralWrap);
				  hWnd       = ValueWindow->operator[](PRFSPLWRP);
				  break;
				}
				case PRFBCNLEN: {
				  ButtonholeCornerLength = value * PFGRAN;
				  fmtStr                 = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
				  hWnd                   = ValueWindow->operator[](PRFBCNLEN);
				  break;
				}
				case PRFHUPWID: {
				  IniFile.hoopSizeX = value * PFGRAN;
				  fmtStr            = fmt::format(FMT_COMPILE(L"{:.0f} mm"), value);
				  hWnd              = ValueWindow->operator[](PRFHUPWID);
				  form::sethup();
				  formForms::prfmsg();
				  thred::chkhup();
				  break;
				}
				case PRFHUPHGT: {
				  IniFile.hoopSizeY = value * PFGRAN;
				  fmtStr            = fmt::format(FMT_COMPILE(L"{:.0f} mm"), value);
				  hWnd              = ValueWindow->operator[](PRFHUPHGT);
				  form::sethup();
				  formForms::prfmsg();
				  thred::chkhup();
				  break;
				}
				case PRFGRDSIZ: {
				  IniFile.gridSize = value * PFGRAN;
				  fmtStr           = fmt::format(FMT_COMPILE(L"{:.2f} mm"), value);
				  hWnd             = ValueWindow->operator[](PRFGRDSIZ);
				  break;
				}
				case PRFCHFLEN: {
				  IniFile.chainSpace = value * PFGRAN;
				  fmtStr             = fmt::format(FMT_COMPILE(L"{:.2f}"), value);
				  hWnd               = ValueWindow->operator[](PRFCHFLEN);
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
		if ((nullptr != hWnd) && (!fmtStr.empty())) {
		  SetWindowText(hWnd, fmtStr.c_str());
		}
		thred::unsid();
		PreferenceIndex = 0;
	  }
	}
  }
  if (MsgBuffer->size() > 1) {
	outDebugString(L"chknum: buffer length [{}] size [{}]\n", wcslen(MsgBuffer->data()), MsgBuffer->size());
	auto const value = thred::getMsgBufferValue();
	if (StateMap->testAndReset(StateFlag::NUROT)) {
	  if (value != 0.0F) {
		IniFile.rotationAngle = value * DEGRADF;
	  }
	  return;
	}
	if (StateMap->testAndReset(StateFlag::ENTRSEG)) {
	  if (value != 0.0F) {
		IniFile.rotationAngle = PI_F2 / value;
	  }
	  return;
	}
	if (StateMap->testAndReset(StateFlag::ENTRFNUM)) {
	  if (wrap::round<uint32_t>(value) < FormList->size()) {
		form::frmnumfn(ClosestFormToCursor, wrap::round<uint32_t>(value));
	  }
	  else {
		displayText::tabmsg(IDS_FRMN1, false);
	  }
	  return;
	}
	while (true) {
	  auto const uintValue = wrap::floor<uint32_t>(std::abs(value));
	  if (StateMap->testAndReset(StateFlag::ENTRPOL)) {
		thred::savdo();
		form::durpoli(uintValue);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTRSTAR)) {
		thred::savdo();
		constexpr auto STARSIZE = 250.0F; // star size factor
		form::dustar(uintValue,
		             STARSIZE / value * ZoomFactor *
		                 wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY));
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTRSPIR)) {
		thred::savdo();
		form::duspir(uintValue);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTRHART)) {
		thred::savdo();
		form::duhart(uintValue);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTRLENS)) {
		thred::savdo();
		form::dulens(uintValue);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTREG)) {
		thred::savdo();
		form::dueg(uintValue);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTRZIG)) {
		thred::savdo();
		form::duzig(uintValue);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::PIXIN)) {
		IniFile.nudgePixels = pxchk(value);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::STPXIN)) {
		IniFile.stitchSizePixels = pxchk(value);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FRMPXIN)) {
		IniFile.formVertexSizePixels = wrap::round<uint16_t>(value);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FRMBOXIN)) {
		IniFile.formBoxSizePixels = wrap::round<uint16_t>(value);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::GETMIN)) {
		SmallStitchLength = value * PFGRAN;
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTR30)) {
		ThreadSize30 = value;
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTR40)) {
		ThreadSize40 = value;
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::ENTR60)) {
		ThreadSize60 = value;
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::SCLPSPAC)) {
		IniFile.clipOffset = value * PFGRAN;
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFIND)) {
		xt::dufind(value);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFHI)) {
		if (value != 0.0F) {
		  xt::dufhi(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFWID)) {
		if (value != 0.0F) {
		  xt::dufwid(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFMAX)) {
		if (value != 0.0F) {
		  xt::dufmax(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFMIN)) {
		xt::dufmin(value * PFGRAN);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETBMAX)) {
		if (value != 0.0F) {
		  xt::dubmax(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETBMIN)) {
		xt::dubmin(value * PFGRAN);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETBSPAC)) {
		if (value != 0.0F) {
		  xt::dubspac(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFLEN)) {
		if (value != 0.0F) {
		  xt::dublen(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETBCOL)) {
		xt::dubcol(wrap::round<uint8_t>(value));
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFCOL)) {
		xt::dufcol(wrap::round<uint8_t>(value));
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETUCOL)) {
		xt::dundcol(wrap::round<uint8_t>(value));
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFANG)) {
		xt::dufxang(value);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFSPAC)) {
		if (value != 0.0F) {
		  xt::dufspac(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETUANG)) {
		xt::dufang(value);
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETFLEN)) {
		if (value != 0.0F) {
		  xt::duflen(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETUSPAC)) {
		if (value != 0.0F) {
		  xt::duspac(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::FSETULEN)) {
		if (value != 0.0F) {
		  xt::dusulen(value * PFGRAN);
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::GTUANG)) {
		IniFile.underlayAngle = value * DEGRADF;
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::GTUSPAC)) {
		if (value != 0.0F) {
		  IniFile.underlaySpacing = value * PFGRAN;
		}
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::GTWLKIND)) {
		IniFile.underlayIndent = value * PFGRAN;
		break;
	  }
	  if (StateMap->testAndReset(StateFlag::GTWLKLEN)) {
		if (value != 0.0F) {
		  IniFile.underlayStitchLen = value * PFGRAN;
		}
		break;
	  }
	  break;
	}
  }

  StateMap->set(StateFlag::RESTCH);
}

void thi::noMsg() {
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
  if (StateMap->testAndReset(StateFlag::NUMIN) || (FormMenuChoice != 0U) || (PreferenceIndex != 0U)) {
	chknum();
  }
  StateMap->reset(StateFlag::MOVMSG);
  StateMap->reset(StateFlag::DELFRM);
  StateMap->reset(StateFlag::FILMSG);
  StateMap->reset(StateFlag::FORMIN);
  StateMap->set(StateFlag::RESTCH);
}

void thred::unmsg() {
  if (MsgWindow != nullptr) {
	thi::noMsg();
  }
}

#pragma warning(suppress : 26461) // The pointer argument can be marked as a pointer to const (con.3)
auto thi::oldwnd(HWND window) noexcept -> bool {
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	if (DefaultColorWin->operator[](iColor) == window ||
	    UserColorWin->operator[](iColor) == window || ThreadSizeWin->operator[](iColor) == window) {
	  return false;
	}
  }
  for (auto iButton = 0U; iButton < BTNCOUNT; ++iButton) {
	if (ButtonWin->operator[](iButton) == window) {
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

auto CALLBACK thi::enumChildProc(HWND hwnd, LPARAM lParam) noexcept -> BOOL {
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

void thred::rstAll() {
  StateMap->reset(StateFlag::WASFPNT);
  StateMap->reset(StateFlag::WASFRMFRM);
  StateMap->reset(StateFlag::FPUNCLP);
  StateMap->reset(StateFlag::FPSEL);
  StateMap->reset(StateFlag::CAPT);
  StateMap->reset(StateFlag::INSRT);
  StateMap->reset(StateFlag::SELBOX);
  StateMap->reset(StateFlag::GRPSEL);
  StateMap->reset(StateFlag::SCROS);
  StateMap->reset(StateFlag::ECROS);
  StateMap->reset(StateFlag::LIN1);
  StateMap->reset(StateFlag::CLPSHO);
  StateMap->reset(StateFlag::SELPNT);
  StateMap->reset(StateFlag::ROTAT);
  StateMap->reset(StateFlag::ROTCAPT);
  StateMap->reset(StateFlag::FRMPMOV);
  StateMap->reset(StateFlag::MOVFRM);
  StateMap->reset(StateFlag::SATCNKT);
  StateMap->reset(StateFlag::FRMPSEL);
  StateMap->reset(StateFlag::SHOINSF);
  StateMap->reset(StateFlag::STRTCH);
  StateMap->reset(StateFlag::SHOSTRTCH);
  StateMap->reset(StateFlag::EXPAND);
  StateMap->reset(StateFlag::POLIMOV);
  StateMap->reset(StateFlag::BZUMIN);
  StateMap->reset(StateFlag::OSAV);
  StateMap->reset(StateFlag::SAVEX);
  StateMap->reset(StateFlag::MOVFRMS);
  StateMap->reset(StateFlag::FRMROT);
  StateMap->reset(StateFlag::DELSFRMS);
  StateMap->reset(StateFlag::BIGBOX);
  StateMap->reset(StateFlag::UPTO);
  StateMap->reset(StateFlag::LENSRCH);
  StateMap->reset(StateFlag::WASGRP);
  StateMap->reset(StateFlag::BOXZUM);
  StateMap->reset(StateFlag::HIDSTCH);
  StateMap->reset(StateFlag::ENTRPOL);
  StateMap->reset(StateFlag::ENTRSTAR);
  StateMap->reset(StateFlag::ENTRSPIR);
  StateMap->reset(StateFlag::ENTRHART);
  StateMap->reset(StateFlag::FORMIN);
  StateMap->reset(StateFlag::GTUANG);
  StateMap->reset(StateFlag::GTUSPAC);
  StateMap->reset(StateFlag::GTWLKIND);
  StateMap->reset(StateFlag::GTWLKLEN);
  trace::untrace();
  StateMap->reset(StateFlag::WASEDG);
  displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
  if (util::closeEnough(ZoomFactor, 1.0F)) {
	StateMap->reset(StateFlag::ZUMED);
  }
  thred::movStch();
  ShowWindow(MainStitchWin, SW_SHOWNORMAL);
  thred::unbsho();
  StateMap->reset(StateFlag::SIDACT);
  thred::unsid();
  StateMap->reset(StateFlag::PRFACT);
  StateMap->reset(StateFlag::WASRT);
  DestroyWindow(PreferencesWindow);
  thred::undat();
  if (StateMap->testAndReset(StateFlag::INSFRM)) {
	thi::insadj();
  }
  StateMap->reset(StateFlag::FUNCLP);
  if (StateMap->testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (StateMap->testAndReset(StateFlag::RUNPAT)) {
	thi::patdun();
  }
  StateMap->reset(StateFlag::FORMSEL);
  StateMap->reset(StateFlag::FRMPSEL);
  if (!SelectedFormList->empty()) {
	SelectedFormList->clear();
  }
  thred::unmsg();
  SearchLine->clear();
  SearchLine->shrink_to_fit();
  FirstWin = nullptr;
  while (EnumChildWindows(MainStitchWin, thi::enumChildProc, 0) != 0) { }
}

void thred::ritot(uint32_t number) {
  auto const txt = displayText::format(IDS_TOT, number);
  displayText::butxt(HTOT, txt);
}

void thi::frmcalc(uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex) {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (StitchBuffer->empty() || ((form.fillType == 0U) && (form.edgeType == 0U))) {
	auto const blank = std::wstring {};
	displayText::butxt(HMAXLEN, blank);
	displayText::butxt(HMINLEN, blank);
	return;
  }
	auto const     code      = ClosestFormToCursor << FRMSHFT;

	auto maxLength      = LOWFLOAT;
	auto minLength      = BIGFLOAT;
	smallestStitchIndex = 0U;
	largestStitchIndex  = 0U;
	auto stitchFwd1     = std::next(StitchBuffer->begin());
	auto spStitches = std::ranges::subrange(StitchBuffer->begin(), StitchBuffer->end() - 2U);
	for (auto index = 0U; auto const& stitch:spStitches) {
	  if ((stitch.attribute & FRMSK) != code || ((stitch.attribute & NOTFRM) != 0U) ||
	      (stitchFwd1->attribute & FRMSK) != code || ((stitchFwd1->attribute & TYPMSK) == 0U)) {
		++stitchFwd1;
		++index;
		continue;
	  }
	  auto const deltaX = stitchFwd1->x - stitch.x;
	  auto const deltaY = stitchFwd1->y - stitch.y;
	  auto const length = deltaX * deltaX + deltaY * deltaY;
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

void thi::lenfn(uint32_t startStitch, uint32_t endStitch, uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex) {
  if (StitchBuffer->empty()) {
	return;
  }
	auto maxLength      = LOWFLOAT;
	auto minLength      = BIGFLOAT;
	smallestStitchIndex = 0U;
	largestStitchIndex  = 0U;
	auto stitch         = wrap::next(StitchBuffer->begin(), startStitch);
	auto stitchFwd1     = std::next(stitch);
	for (auto iStitch = startStitch; iStitch < endStitch; ++iStitch) {
	  auto const deltaX = stitchFwd1->x - stitch->x;
	  auto const deltaY = stitchFwd1->y - stitch->y;
	  auto const length = deltaX * deltaX + deltaY * deltaY;
	  if (length > maxLength) {
		maxLength          = length;
		largestStitchIndex = iStitch;
	  }
	  if (length < minLength) {
		minLength           = length;
		smallestStitchIndex = iStitch;
	  }
	  ++stitch;
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

void thred::lenCalc() {
  auto static chkVal = 0.0F;
  auto const blank   = std::wstring {};
  if (StateMap->test(StateFlag::LENSRCH)) {
	auto const stitch     = wrap::next(StitchBuffer->begin(), ClosestPointIndex);
	auto const stitchFwd1 = std::next(stitch);
	auto const lenMax = std::hypot(stitchFwd1->x - stitch->x, stitchFwd1->y - stitch->y) * IPFGRAN;
	if (!util::closeEnough(lenMax, chkVal)) {
	  chkVal = lenMax;
	  displayText::butxt(HMINLEN, fmt::format(FMT_COMPILE(L"{:.2f}"), lenMax));
	}
	displayText::butxt(HMAXLEN, displayText::loadStr(IDS_SRCH));
	return;
  }
  if (StitchBuffer->size() <= 1U) {
	displayText::butxt(HMAXLEN, blank);
	displayText::butxt(HMINLEN, blank);
	return;
  }
  if (StateMap->test(StateFlag::FORMSEL)) {
	thi::frmcalc(LargestStitchIndex, SmallestStitchIndex);
	displayText::butxt(HCOR, blank);
	return;
  }
  thred::rngadj();
  if (StateMap->test(StateFlag::GRPSEL) && GroupStartStitch != GroupEndStitch) {
	thi::lenfn(GroupStartStitch, GroupEndStitch - 1U, LargestStitchIndex, SmallestStitchIndex);
  }
  else {
	thi::lenfn(0, wrap::toUnsigned(StitchBuffer->size() - 2U), LargestStitchIndex, SmallestStitchIndex);
  }
}

auto thi::chk2of() -> bool {
  if (!StateMap->test(StateFlag::SELBOX)) {
	return false;
  }
  if (UserFlagMap->test(UserFlag::FIL2OF)) {
	return false;
  }
  return true;
}

auto thi::find1st() -> uint32_t {
  if (thi::chk2of()) {
	return ClosestPointIndex;
  }
  return thred::findFirstStitch(ClosestFormToCursor);
}

void thi::delsmal(uint32_t startStitch, uint32_t endStitch) {
  auto const codedAttribute = ClosestFormToCursor << FRMSHFT;
  auto stitchSize = BIGFLOAT; // to ensure that it is larger than SmallStitchLength first time through
  auto const minStitchLength = SmallStitchLength * SmallStitchLength;
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto iPrevStitch = thi::find1st();
	auto iStitch     = iPrevStitch + 1U;
	auto lastStitch  = StitchBuffer->size();
	--lastStitch;
	while (iStitch < lastStitch && stitchSize > minStitchLength) {
	  auto const& stitch     = StitchBuffer->operator[](iStitch);
	  auto const& prevStitch = StitchBuffer->operator[](iPrevStitch);
	  if (((stitch.attribute & NOTFRM) != 0U) || (stitch.attribute & FRMSK) != codedAttribute) { // are we still in the selected form?
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
	auto prevPoint     = StitchBuffer->operator[](startStitch); // intentional copy
	lastStitch         = StitchBuffer->size();
	if (lastStitch != 0U) {
	  --lastStitch;
	  while (iStitch < lastStitch) {
		auto const& stitch    = StitchBuffer->operator[](iStitch);
		auto&       outStitch = StitchBuffer->operator[](iOutputStitch);
		if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == codedAttribute) { // are we still in the selected form?
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
	StitchBuffer->erase(wrap::next(StitchBuffer->begin(), iOutputStitch),
	                    wrap::next(StitchBuffer->begin(), iStitch));
	thred::coltab();
  }
  else {
	auto iNextStitch = startStitch + 1U;
	auto prevPoint   = StitchBuffer->operator[](startStitch); // intentional copy
	for (auto iStitch = iNextStitch; iStitch < endStitch; ++iStitch) {
	  auto const& stitch = StitchBuffer->operator[](iStitch);
	  if ((StitchBuffer->operator[](iNextStitch).attribute & KNOTMSK) != 0U) {
		prevPoint = StitchBuffer->operator[](iNextStitch);

		StitchBuffer->operator[](iNextStitch++) = stitch;
	  }
	  else {
		auto const deltaX = stitch.x - prevPoint.x;
		auto const deltaY = stitch.y - prevPoint.y;
		stitchSize        = deltaX * deltaX + deltaY * deltaY;
		if (stitchSize > minStitchLength) {
		  StitchBuffer->operator[](iNextStitch++) = stitch;
		  prevPoint                               = stitch;
		}
	  }
	}
	StitchBuffer->erase(wrap::next(StitchBuffer->begin(), iNextStitch),
	                    wrap::next(StitchBuffer->begin(), endStitch));
	thred::coltab();
  }
  thred::rstAll();
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  thred::lenCalc();
  StateMap->set(StateFlag::RESTCH);
}

void thred::duzero() {
  if (StitchBuffer->empty()) {
	return;
  }
  auto const minStitch = SmallStitchLength * SmallStitchLength;
  if (!SelectedFormList->empty()) {
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto const selectedForm : (*SelectedFormList)) {
	  formMap.set(selectedForm);
	}
	StateMap->reset(StateFlag::CONTIG);
	auto iDestination  = StitchBuffer->begin();
	auto currentStitch = StitchBuffer->front(); // intentional copy
	for (auto const& iStitch : *StitchBuffer) {
	  if (((iStitch.attribute & TYPMSK) == 0U) || !formMap.test((iStitch.attribute & FRMSK) >> FRMSHFT)) {
		(*iDestination) = iStitch;
		++iDestination;
		StateMap->reset(StateFlag::CONTIG);
		continue;
	  }
	  if (!StateMap->testAndSet(StateFlag::CONTIG)) {
		++iDestination;
		continue;
	  }
	  auto const deltaX       = iStitch.x - currentStitch.x;
	  auto const deltaY       = iStitch.y - currentStitch.y;
	  auto const stitchLength = deltaX * deltaX + deltaY * deltaY;
	  if (stitchLength <= minStitch) {
		continue;
	  }
	  currentStitch   = iStitch;
	  (*iDestination) = iStitch;
	  ++iDestination;
	}
	StitchBuffer->erase(iDestination, StitchBuffer->end());
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	thi::delsmal(GroupStartStitch, GroupEndStitch);
  }
  else {
	thi::delsmal(0, wrap::toUnsigned(StitchBuffer->size()));
  }
}

void thi::rshft(POINT const& shiftPoint) {
  auto const shiftX = wrap::toFloat(shiftPoint.x);
  auto const shiftY = wrap::toFloat(shiftPoint.y);
  ZoomRect.top -= shiftY;
  ZoomRect.left -= shiftX;
  ZoomRect.right -= shiftX;
  ZoomRect.bottom -= shiftY;
  zRctAdj();
  StateMap->set(StateFlag::RESTCH);
}

void thred::pgdwn() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, std::lround((ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	thi::rshft(scrollPosition);
  }
}

void thred::pgup() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, std::lround(-(ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	thi::rshft(scrollPosition);
  }
}

void thred::pglft() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {std::lround((ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
	thi::rshft(scrollPosition);
  }
}

void thred::pgrit() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {std::lround(-(ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
	thi::rshft(scrollPosition);
  }
}

void thi::selin(uint32_t start, uint32_t end, HDC hDC) {
  SelectObject(hDC, GroupSelectPen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (!SearchLine->empty()) {
	wrap::polyline(hDC, SearchLine->data(), wrap::toUnsigned(SearchLine->size()));
  }
  if (start > end) {
	std::swap(start, end);
  }
  SearchLine->clear();
  if (!StitchBuffer->empty()) {
	auto stitch = wrap::next(StitchBuffer->begin(), start);
	for (auto iStitch = start; iStitch <= end; ++iStitch) {
	  SearchLine->push_back(POINT {wrap::ceil<int32_t>(((stitch->x - ZoomRect.left) * ZoomRatio.x)),
	                               wrap::ceil<int32_t>((wrap::toFloat(StitchWindowClientRect.bottom) -
	                                                    (stitch->y - ZoomRect.bottom) * ZoomRatio.y))});
	  ++stitch;
	}
	wrap::polyline(hDC, SearchLine->data(), wrap::toUnsigned(SearchLine->size()));
  }
  SetROP2(hDC, R2_COPYPEN);
}

void thi::cros(uint32_t iStitch) {
  auto const armLength            = BoxOffset[0];
  auto const stitchCoordsInPixels = thred::stch2px1(iStitch);
  InsertLine[0]                   = {stitchCoordsInPixels.x - armLength, stitchCoordsInPixels.y};
  InsertLine[1]                   = {stitchCoordsInPixels.x + armLength, stitchCoordsInPixels.y};
  SelectObject(StitchWindowDC, CrossPen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SelectObject(StitchWindowMemDC, CrossPen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  InsertLine[0] = {stitchCoordsInPixels.x, stitchCoordsInPixels.y - armLength};
  InsertLine[1] = {stitchCoordsInPixels.x, stitchCoordsInPixels.y - 1};
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  wrap::polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  InsertLine[0].y = stitchCoordsInPixels.y + 2;
  InsertLine[1].y = stitchCoordsInPixels.y + armLength;
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
  wrap::polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thi::uncros() {
  if (StateMap->testAndReset(StateFlag::SCROS)) {
	cros(GroupStartStitch);
  }
  if (StateMap->testAndReset(StateFlag::ECROS)) {
	if (GroupEndStitch != GroupStartStitch) {
	  cros(GroupEndStitch);
	}
	StateMap->set(StateFlag::ELIN);
  }
}

void thi::ducros(HDC hDC) {
  uncros();
  StateMap->set(StateFlag::SCROS);
  cros(GroupStartStitch);
  if (GroupStartStitch != GroupEndStitch) {
	cros(GroupEndStitch);
	StateMap->set(StateFlag::ECROS);
  }
  selin(GroupStartStitch, GroupEndStitch, hDC);
}

void thred::selRct(F_RECTANGLE& sourceRect) noexcept(!std::is_same_v<size_t, uint32_t>) {
  if (!StitchBuffer->empty()) {
	auto minX = BIGFLOAT;
	auto minY = BIGFLOAT;
	auto maxX = LOWFLOAT;
	auto maxY = LOWFLOAT;

	if (GroupEndStitch != GroupStartStitch) {
	  auto const groupStitchRange =
	      std::ranges::subrange(wrap::next(StitchBuffer->begin(), GroupStartStitch),
	                            wrap::next(StitchBuffer->begin(), GroupEndStitch));
	  for (auto& stitch : groupStitchRange) {
		minX = std::min(minX, stitch.x);
		minY = std::min(minY, stitch.y);
		maxX = std::max(maxX, stitch.x);
		maxY = std::max(maxY, stitch.y);
	  }
	  sourceRect = F_RECTANGLE {minX, maxY, maxX, minY};
	}
	else {
	  auto const& stitch = StitchBuffer->at(GroupStartStitch);
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

void thi::dusel(HDC hDC) {
  SetROP2(hDC, R2_NOTXORPEN);
  SelectObject(hDC, LinePen);
  wrap::polyline(hDC, FormControlPoints->data(), wrap::toUnsigned(FormControlPoints->size() - 1U));
  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(FormControlPoints->size() - 1U); ++iPoint) {
	form::selsqr(FormControlPoints->operator[](iPoint), hDC);
  }
  SetROP2(hDC, R2_COPYPEN);
}

void thred::unsel() {
  if (StateMap->testAndReset(StateFlag::SELSHO)) {
	thi::dusel(StitchWindowDC);
  }
}

void thi::nuselrct() {
  auto outline = std::vector<F_POINT> {};
  outline.resize(FormControlPoints->size());
  thred::unsel();
  outline[PTL].x = outline[PBL].x = outline[PLM].x = outline[PTLE].x = StitchRangeRect.left;
  outline[PTL].y = outline[PTM].y = outline[PTR].y = outline[PTLE].y = StitchRangeRect.top;
  outline[PTR].x = outline[PRM].x = outline[PBR].x = StitchRangeRect.right;
  outline[PBR].y = outline[PBM].y = outline[PBL].y = StitchRangeRect.bottom;
  outline[PTM].x = outline[PBM].x = wrap::midl(StitchRangeRect.right, StitchRangeRect.left);
  outline[PRM].y = outline[PLM].y = wrap::midl(StitchRangeRect.top, StitchRangeRect.bottom);

  auto iPoint = outline.begin();
  for (auto& controlPoint : *FormControlPoints) {
	controlPoint = form::sfCor2px(*iPoint);
	++iPoint;
  }
}

void thred::grpAdj() {
  thi::uncros();
  thred::unsel();
  thred::rngadj();
  thi::ducros(StitchWindowDC);
  thred::lenCalc();
  thred::selRct(StitchRangeRect);
  if (!StateMap->test(StateFlag::ZUMED) || GroupEndStitch == GroupStartStitch) {
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StitchRangeRect.top <= (ZoomRect.top - 1) && StitchRangeRect.bottom >= (ZoomRect.bottom - 1) &&
      StitchRangeRect.left >= (ZoomRect.left + 1) && StitchRangeRect.right <= (ZoomRect.right - 1)) {
	StateMap->set(StateFlag::RESTCH);
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
	StateMap->reset(StateFlag::ZUMED);
	ZoomFactor = 1.0;
	thred::movStch();
  }
  else {
	ZoomRect.right = ZoomRect.left + newSize.x;
	ZoomFactor     = newSize.x / wrap::toFloat(UnzoomedRect.cx);
	ZoomRect.top   = ZoomRect.bottom + newSize.y;
	auto const stitchPoint =
	    F_POINT {((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left,
	             ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom};
	thred::shft(stitchPoint);
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::nuAct(uint32_t iStitch) noexcept {
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

void thi::lensadj() {
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
  StateMap->set(StateFlag::RESTCH);
}

void thi::ritlayr() {
  auto layer = BIGUINT;

  if (StateMap->test(StateFlag::SELBOX)) {
	if (!StitchBuffer->empty()) {
	  layer = (StitchBuffer->operator[](ClosestPointIndex).attribute & LAYMSK) >> LAYSHFT;
	}
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::FRMPSEL)) {
	  layer = gsl::narrow_cast<decltype(layer)>(FormList->operator[](ClosestFormToCursor).attribute & FRMLMSK) >> 1U;
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

void thred::nuPen(HPEN& pen, int32_t width, COLORREF color) noexcept {
  DeleteObject(pen);
  pen = wrap::createPen(PS_SOLID, width, color);
}

void thi::nuStchSiz(uint32_t iColor, int32_t width) noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  auto const tsp = wrap::next(ThreadSizePixels.begin(), iColor);
  if (width != *tsp) {
	thred::nuPen(UserPen->operator[](iColor), width, UserColor.at(iColor));
	*tsp = width;
  }
}

void thi::nuBrush(HBRUSH& brush, COLORREF color) noexcept {
  DeleteObject(brush);
  brush = CreateSolidBrush(color);
}

void thi::rotpix(POINT const& unrotatedPoint, POINT& rotatedPoint, POINT const& rotationCenterPixels) noexcept {
  // won't handle vertical lines
  auto const deltaX           = wrap::toFloat(unrotatedPoint.x - rotationCenterPixels.x);
  auto const deltaY           = wrap::toFloat(unrotatedPoint.y - rotationCenterPixels.y);
  auto const distanceToCenter = std::hypotf(deltaX, deltaY);
  auto const newAngle         = std::atan2f(deltaY, deltaX) - RotateAngle;
  rotatedPoint = {std::lround(wrap::toFloat(rotationCenterPixels.x) + distanceToCenter * cos(newAngle)),
                  std::lround(wrap::toFloat(rotationCenterPixels.y) + distanceToCenter * sin(newAngle))};
}

void thi::duar(POINT const& stitchCoordsInPixels) noexcept(std::is_same_v<size_t, uint32_t>) {
  auto const offset = MulDiv(10, *ScreenDPI, STDDPI);
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

void thred::dubox(POINT const& stitchCoordsInPixels) {
  if (StitchBuffer->empty()) {
	return;
  }
  auto const stitch = wrap::next(StitchBuffer->begin(), ClosestPointIndex);
  if (ClosestPointIndex != (StitchBuffer->size() - 1U)) {
	// if the selected point is not at the end then aim at the next point
	auto const stitchFwd1 = std::next(stitch);
	RotateAngle           = std::atan2f(stitchFwd1->y - stitch->y, stitchFwd1->x - stitch->x);
  }
  else { // otherwise aim in the same direction
	auto const stitchBck1 = std::next(stitch, -1);
	RotateAngle           = std::atan2f(stitch->y - stitchBck1->y, stitch->x - stitchBck1->x);
  }
  thi::duar(stitchCoordsInPixels);
  StateMap->reset(StateFlag::ELIN);
  StateMap->set(StateFlag::SELBOX);
  StateMap->reset(StateFlag::FRMPSEL);
  thred::redrawColorBar();
  displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
}

auto thred::stch2px(uint32_t iStitch, POINT& stitchCoordsInPixels) -> bool {
  if (StitchBuffer->empty()) {
	return false;
  }
  stitchCoordsInPixels = {
      wrap::ceil<int32_t>((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x),
      wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                          (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y)};
  return stitchCoordsInPixels.x >= 0 && stitchCoordsInPixels.x <= StitchWindowClientRect.right &&
         stitchCoordsInPixels.y >= 0 && stitchCoordsInPixels.y <= StitchWindowClientRect.bottom;
}

auto thi::stch2px2(uint32_t iStitch) -> bool {
  auto const stitchCoordsInPixels =
      POINT {wrap::ceil<int32_t>((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x),
             wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                 (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y)};
  return stitchCoordsInPixels.x >= 0 && stitchCoordsInPixels.x <= StitchWindowClientRect.right &&
         stitchCoordsInPixels.y >= 0 && stitchCoordsInPixels.y <= StitchWindowClientRect.bottom;
}

auto thred::stch2pxr(F_POINT const& stitchCoordinate) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    (stitchCoordinate.y - ZoomRect.bottom) * ZoomRatio.y)};
}

void thi::getDocsFolder(fs::path& directory) {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto       ppszPath = PWSTR {nullptr}; // variable to receive the path memory block pointer.
  auto const hResult  = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &ppszPath);
  if (SUCCEEDED(hResult)) {
	directory.assign(ppszPath); // make a local copy of the path
  }
  CoTaskMemFree(ppszPath); // free up the path memory block
}

void thi::defNam(fs::path const& fileName) {
  if (fileName.empty()) {
	getDocsFolder(*DefaultDirectory);
	return;
  }
  *DefaultDirectory = fileName.parent_path();
}

void thi::ritini() {
  auto const     directory = utf::utf16ToUtf8(DefaultDirectory->wstring());
  constexpr char FILLCHAR  = '\0';
  std::ranges::fill(IniFile.defaultDirectory, FILLCHAR);
  auto const spIDD = gsl::span {IniFile.defaultDirectory};
  std::ranges::copy(directory, spIDD.begin());
  auto previousName = PreviousNames->begin();
  for (auto& prevName : IniFile.prevNames) {
	std::ranges::fill(prevName, FILLCHAR);
	if (!previousName->empty()) {
	  auto       previous = utf::utf16ToUtf8(*previousName);
	  auto const spName   = gsl::span {prevName};
	  std::ranges::copy(previous, spName.begin());
	}
	++previousName;
  }
  auto const designer = utf::utf16ToUtf8(*DesignerName);
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
  if (ShowStitchThreshold < SSMIN) {
	ShowStitchThreshold = SSMIN;
  }
  if (ShowStitchThreshold > SSMAX) {
	ShowStitchThreshold = SSMAX;
  }
  IniFile.showStitchThreshold    = ShowStitchThreshold;
  IniFile.threadSize30           = ThreadSize30;
  IniFile.threadSize40           = ThreadSize40;
  IniFile.threadSize60           = ThreadSize60;
  IniFile.userStitchLength       = UserStitchLength;
  IniFile.smallStitchLength      = SmallStitchLength;
  IniFile.stitchBoxesThreshold   = StitchBoxesThreshold;
  IniFile.stitchSpace            = LineSpacing;
  IniFile.userFlagMap            = UserFlagMap->to_ulong();
  IniFile.borderWidth            = BorderWidth;
  IniFile.appliqueColor          = AppliqueColor;
  IniFile.snapLength             = SnapLength;
  IniFile.starRatio              = StarRatio;
  IniFile.spiralWrap             = SpiralWrap;
  IniFile.buttonholeCornerLength = ButtonholeCornerLength;
  IniFile.picotSpace             = PicotSpacing;
  if (!UserFlagMap->test(UserFlag::SAVMAX)) {
	auto windowRect = RECT {};
	GetWindowRect(ThrEdWindow, &windowRect);
	IniFile.initialWindowCoords.left   = windowRect.left;
	IniFile.initialWindowCoords.right  = windowRect.right;
	IniFile.initialWindowCoords.bottom = windowRect.bottom;
	IniFile.initialWindowCoords.top    = windowRect.top;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const iniFileHandle = CreateFile(
      IniFileName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (iniFileHandle != INVALID_HANDLE_VALUE) {
	auto bytesRead = DWORD {};
	WriteFile(iniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, nullptr);
	// ToDo - Check that file is wriiten completely
  }
  CloseHandle(iniFileHandle);
}

auto thi::savcmp() noexcept -> bool {
#ifdef _DEBUG
  return true;
#else
  return !StateMap->test(StateFlag::CMPDO);
#endif
}

void thred::reldun() {
  texture::txdun();
  bal::ritbal();
  thi::ritini();
  mouse::dstcurs();
  PostQuitMessage(0);
}

auto thi::chkattr(fs::path const& filename) -> bool {
  constexpr auto WRBITS = (fs::perms::owner_write | fs::perms::others_write | fs::perms::group_write); // Write permissions bits
  if (fs::space(filename.parent_path()).available == 0U) {
	StateMap->set(StateFlag::NOTFREE);
	return true;
  }
  StateMap->reset(StateFlag::NOTFREE);
  auto const writePerms = fs::status(filename).permissions() & WRBITS;
  if (writePerms != fs::perms::none) {
	return false;
  }
  auto const fmtStr = displayText::format(IDS_OVRLOK, filename.wstring());
  auto const buttonPressed =
      MessageBox(ThrEdWindow, fmtStr.c_str(), displayText::loadStr(IDS_OVRIT).c_str(), MB_YESNO);
  if (buttonPressed != IDYES) {
	return true;
  }
  fs::permissions(filename, WRBITS, fs::perm_options::add);
  return false;
}

void thi::duver(fs::path const& name) {
  auto const version = wrap::toSize(tolower(name.extension().wstring().back()) - 'r');
  if (version < VersionNames->size()) {
	VersionNames->operator[](version) = name;
  }
}

void thi::durit(std::vector<char>& destination, const void* source, uint32_t count) {
  if (source != nullptr) {
	auto const spSrc =
	    gsl::span<const char> {gsl::narrow_cast<char const*>(source), gsl::narrow_cast<size_t>(count)};
	destination.insert(destination.end(), spSrc.begin(), spSrc.end());
  }
}

void thi::dubuf(std::vector<char>& buffer) {
  auto stitchHeader  = THR_HEAD {};
  auto vertexCount   = 0U;
  auto guideCount    = 0U;
  auto clipDataCount = 0U;

  constexpr auto HDRVERS = 0x02U; // ThrEd file version signature

  stitchHeader.headerType = HDRVERS << TBYTSHFT | THREDSIG;
  stitchHeader.fileLength =
      wrap::sizeofVector(StitchBuffer) + sizeof(stitchHeader) + bitmap::getBmpNameLength();
  wrap::narrow(stitchHeader.stitchCount, StitchBuffer->size());
  wrap::narrow_cast(stitchHeader.hoopType, IniFile.hoopType);
  auto       designer       = utf::utf16ToUtf8(*DesignerName);
  auto const spModifierName = gsl::span {ExtendedHeader->modifierName};
  std::ranges::copy(designer, spModifierName.begin());
  spModifierName[designer.length()] = 0;
  if (!FormList->empty()) {
	for (auto& iForm : (*FormList)) {
	  vertexCount += iForm.vertexCount;
	  if (iForm.type == SAT && (iForm.satinGuideCount != 0U)) {
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
  wrap::narrow(stitchHeader.formCount, FormList->size());
  wrap::narrow(stitchHeader.vertexCount, vertexCount);
  wrap::narrow(stitchHeader.dlineCount, guideCount);
  wrap::narrow(stitchHeader.clipDataCount, clipDataCount);
  auto const formDataOffset = bitmap::getBmpNameLength() + sizeof(BackgroundColor) +
                              sizeof(UserColor) + sizeof(CustomColor) + TSSIZE;
  auto       vtxLen = sizeof(stitchHeader) + wrap::sizeofVector(StitchBuffer) + formDataOffset;
  auto const thredDataSize =
      wrap::sizeofVector(FormList) + vertexCount * wrap::sizeofType(FormVertices) +
      guideCount * wrap::sizeofType(SatinGuides) + clipDataCount * wrap::sizeofType(ClipPoints) +
      wrap::sizeofVector(TexturePointsBuffer);
  buffer.reserve(vtxLen + thredDataSize);
  // ToDo - vertexLength overflows a 16 bit integer if there are more than 5446 stitches, so clamp it until version 3
  constexpr auto VTXCLAMP = std::numeric_limits<decltype(stitchHeader.vertexLen)>::max();
  if (vtxLen > VTXCLAMP) {
	vtxLen = VTXCLAMP;
  }
  wrap::narrow(stitchHeader.vertexLen, vtxLen);
  wrap::narrow(stitchHeader.dlineLen, wrap::sizeofType(FormVertices) * vertexCount);
  wrap::narrow(stitchHeader.clipDataLen, wrap::sizeofType(ClipPoints) * clipDataCount);
  durit(buffer, &stitchHeader, sizeof(stitchHeader));
  ExtendedHeader->auxFormat         = IniFile.auxFileType;
  ExtendedHeader->hoopSizeX         = IniFile.hoopSizeX;
  ExtendedHeader->hoopSizeY         = IniFile.hoopSizeY;
  ExtendedHeader->texturePointCount = wrap::toUnsigned(TexturePointsBuffer->size());
  durit(buffer, ExtendedHeader, sizeof(*ExtendedHeader));
  durit(buffer, StitchBuffer->data(), wrap::sizeofVector(StitchBuffer));
  durit(buffer, bitmap::getBmpNameData(), bitmap::getBmpNameLength());
  durit(buffer, &BackgroundColor, sizeof(BackgroundColor));
  durit(buffer, UserColor.data(), sizeof(UserColor));
  durit(buffer, CustomColor.data(), sizeof(CustomColor));
  auto threadSizeBuffer = std::string {};
  threadSizeBuffer.resize(TSSIZE);
  auto iBuffer = threadSizeBuffer.begin();
  for (auto const& iThread : ThreadSize) {
	*(iBuffer++) = gsl::narrow<char>(iThread);
  }
  durit(buffer, threadSizeBuffer.c_str(), wrap::toUnsigned(threadSizeBuffer.size() * sizeof(threadSizeBuffer[0])));
  if (FormList->empty()) {
	return;
  }
  auto outForms = std::vector<FRM_HEAD_OUT> {};
  outForms.reserve(FormList->size());
  auto vertices = std::vector<F_POINT> {};
  vertices.reserve(vertexCount);
  auto guides = std::vector<SAT_CON_OUT> {};
  guides.reserve(guideCount);
  auto points = std::vector<F_POINT> {};
  points.reserve(clipDataCount);
  for (auto& srcForm : (*FormList)) {
	outForms.emplace_back(srcForm);
	auto itVertex = wrap::next(FormVertices->cbegin(), srcForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < srcForm.vertexCount; ++iVertex) {
	  vertices.push_back(*itVertex);
	  ++itVertex;
	}
	if (srcForm.type == SAT) {
	  wrap::narrow(outForms.back().satinGuideCount, srcForm.satinGuideCount);
	  if (srcForm.satinGuideCount != 0U) {
		auto itGuide = wrap::next(SatinGuides->cbegin(), srcForm.satinGuideIndex);
		for (auto iGuide = 0U; iGuide < srcForm.satinGuideCount; ++iGuide) {
		  guides.emplace_back(*itGuide);
		  ++itGuide;
		}
	  }
	}
	if (srcForm.isClip()) {
	  auto offsetStart = wrap::next(ClipPoints->cbegin(), srcForm.clipIndex);
	  for (auto iClip = 0U; iClip < srcForm.clipCount; ++iClip) {
		points.push_back(*offsetStart);
		++offsetStart;
	  }
	}
	if (srcForm.isEdgeClipX()) {
	  auto       offsetStart = wrap::next(ClipPoints->cbegin(), srcForm.borderClipData);
	  auto const clipCount   = srcForm.clipEntries;
	  for (auto iClip = 0U; iClip < clipCount; ++iClip) {
		points.push_back(*offsetStart);
		++offsetStart;
	  }
	}
  }
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
  if (!TexturePointsBuffer->empty()) {
	durit(buffer, TexturePointsBuffer->data(), wrap::sizeofVector(TexturePointsBuffer));
  }
}

void thi::thrsav() {
  if (chkattr(*WorkingFileName)) {
	return;
  }
  if (!StateMap->testAndReset(StateFlag::IGNAM)) {
	auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
	auto geName   = *WorkingFileName; // intentional copy
	geName.replace_extension(L".th*");
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const file = FindFirstFile(geName.wstring().c_str(), &fileData);
	if (file != INVALID_HANDLE_VALUE) {
	  StateMap->reset(StateFlag::CMPDO);
	  for (auto& version : *VersionNames) {
		version.clear();
	  }
	  auto& fileName = fileData.cFileName;
	  duver(*DefaultDirectory / std::begin(fileName));
	  while (FindNextFile(file, &fileData)) {
		duver(*DefaultDirectory / std::begin(fileName));
	  }
	  FindClose(file);
	  fs::remove(VersionNames->back());
	  for (auto iBackup = VersionNames->size() - 1U; iBackup > 0; --iBackup) {
		if (!VersionNames->operator[](iBackup - 1U).empty()) {
		  auto newFileName = VersionNames->operator[](iBackup - 1U); // copy intended

		  auto ext   = newFileName.extension().wstring();
		  ext.back() = gsl::narrow<wchar_t>(iBackup - 1) + 's';
		  newFileName.replace_extension(ext);
		  auto eCode = std::error_code {};
		  fs::rename(VersionNames->operator[](iBackup - 1U), newFileName, eCode);
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
      CreateFile(ThrName->wstring().c_str(), (GENERIC_WRITE), 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::crmsg(*ThrName);
	return;
  }
  auto output = std::vector<char> {};
  dubuf(output);
  auto bytesWritten = DWORD {};
  WriteFile(fileHandle, output.data(), wrap::toUnsigned(output.size()), &bytesWritten, nullptr);
  if (bytesWritten != output.size()) {
	displayText::showMessage(IDS_FWERR, ThrName->wstring());
  }
  CloseHandle(fileHandle);
}

void thi::chk1col() {
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
  for (auto iColorChange = size_t {}; iColorChange < thred::maxColor(); ++iColorChange) {
	auto const ccTableIt     = wrap::next(ColorChangeTable->begin(), iColorChange);
	auto const ccTableNextIt = std::next(ccTableIt);
	if (ccTableNextIt->stitchIndex - ccTableIt->stitchIndex != 1) {
	  continue;
	}
	auto const iStitch = ccTableIt->stitchIndex;
	auto stitchIt = (iStitch != 0U) ? wrap::next(StitchBuffer->begin(), iStitch - 1U) : StitchBuffer->begin();
	auto const color = stitchIt->attribute & COLMSK;
	++stitchIt;
	stitchIt->attribute &= NCOLMSK;
	stitchIt->attribute |= color;
  }
}

void thi::sav() {
  *AuxName = *WorkingFileName;
  xt::duauxnam(*AuxName);
  if (chkattr(*AuxName)) {
	return;
  }
  if (StitchBuffer->empty()) {
	return;
  }
  chk1col();
  thred::coltab();
  auto saveStitches = std::vector<F_POINT_ATTR> {};
  saveStitches.resize(StitchBuffer->size());
  if (UserFlagMap->test(UserFlag::ROTAUX)) {
	auto iDest = saveStitches.begin();
	for (auto const& stitch : *StitchBuffer) {
	  *iDest++ = F_POINT_ATTR {stitch.y, stitch.x, stitch.attribute};
	}
  }
  else {
	std::ranges::copy(*StitchBuffer, saveStitches.begin());
  }
  // ReSharper disable once CppInitializedValueIsAlwaysRewritten
  auto flag = true;
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  flag = DST::saveDST(*AuxName, saveStitches);
	  break;
	}
#if PESACT
	case AUXPES: {
	  flag = PES::savePES(*AuxName, saveStitches);
	  break;
	}
#endif
	default: {
	  flag = PCS::savePCS(*AuxName, saveStitches);
	}
  }
  if (flag) {
	defNam(*WorkingFileName);
	if (UserFlagMap->test(UserFlag::ROTAUX)) {
	  displayText::filnopn(IDS_FILROT, *AuxName);
	}
  }
}

auto thi::getSaveName(fs::path& fileName, FileIndices& fileType) -> bool {
  auto* pFileSave = gsl::narrow_cast<IFileSaveDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hResult = CoCreateInstance(
      CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  if (FAILED(hResult) || (nullptr == pFileSave)) {
	return false;
  }
#if PESACT
  static constexpr auto ALL_FILE_TYPES =
      std::array<COMDLG_FILTERSPEC, 4> {FLTTHR, FLTPCS, FLTDST, FLTPES}; // All possible file types for save
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
  if (FAILED(hResult) || (nullptr == pItem)) {
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

void thred::savAs() {
  if (StitchBuffer->empty() && FormList->empty() && bitmap::ismap()) {
	return;
  }
  auto index = FileIndices {};
  if (!thi::getSaveName(*WorkingFileName, index)) {
	return;
  }
  *DefaultDirectory = WorkingFileName->parent_path();
  switch (index) {
	case FileIndices::THR: {
	  WorkingFileName->replace_extension(L".thr");
	  break;
	}
	case FileIndices::PCS: {
	  WorkingFileName->replace_extension(L".pcs");
	  menu::setpcs();
	  break;
	}
#if PESACT
	case FileIndices::PES: {
	  WorkingFileName->replace_extension(L".pes");
	  menu::setpes();
	  break;
	}
#endif
	case FileIndices::DST: {
	  WorkingFileName->replace_extension(L".dst");
	  menu::setdst();
	  break;
	}
	// resharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  outDebugString(L"Unknown file type\n");
	  throw;
	}
  }
  StateMap->set(StateFlag::SAVAS);
  thi::nunams();
  thi::ritini();
  StateMap->reset(StateFlag::SAVAS);
  StateMap->reset(StateFlag::CMPDO);
  thi::thrsav();
  thi::sav();
  SetWindowText(ThrEdWindow, ThrName->wstring().c_str());
}

void thred::save() {
  if (WorkingFileName->empty()) {
	thred::savAs();
	return;
  }
  if (WorkingFileName->extension().empty()) {
	*WorkingFileName /= L".thr";
  }
  thi::thrsav();
  thi::sav();
}

void thred::dun() {
  texture::savtxt();
  texture::txof();
  texture::rstxt();
  if (StateMap->testAndReset(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	StateMap->reset(StateFlag::WASRT);
  }
  thred::unsid();
  thred::unbsho();
  thred::rstAll();
  gsl::not_null<fs::path*> const balaradName0 = bal::getBN0();
  //	if(savcmp() || (*BalaradName0 && *BalaradName1 && PCSHeader.stitchCount && !FormIndex))
  if (thi::savcmp() || (!balaradName0->empty())) {
	thred::reldun();
	return;
  }
  if (StitchWindowClientRect.right != 0) {
	displayText::savdisc();
	StateMap->set(StateFlag::SAVEX);
	return;
  }
  auto const fmtStr = displayText::format(IDS_SAVFIL, ThrName->wstring());
  if (MessageBox(ThrEdWindow, fmtStr.c_str(), displayText::loadStr(IDS_CLOS).c_str(), MB_YESNO) == IDYES) {
	thred::save();
  }
  thred::reldun();
}

void thi::dusid(LIST_TYPE entry, int32_t& windowLocation, SIZE const& windowSize) {
  SideWindow->operator[](entry.value) = CreateWindow(L"STATIC",
                                                     displayText::loadStr(entry.stringID).c_str(),
                                                     SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                                     3,
                                                     windowLocation * windowSize.cy + 3,
                                                     windowSize.cx + 3,
                                                     windowSize.cy,
                                                     SideMessageWindow,
                                                     nullptr,
                                                     ThrEdInstance,
                                                     nullptr);
  ++windowLocation;
}

void thi::sidmsg(FRM_HEAD const& form, HWND window) {
  auto childListRect  = RECT {};
  auto parentListRect = RECT {};
  std::ranges::fill(*ValueWindow, nullptr);
  auto sideWindowSize     = SIZE {};
  auto sideWindowLocation = int32_t {};
  GetWindowRect(window, &childListRect);
  GetWindowRect(FormDataSheet, &parentListRect);
  form::ispcdclp();
  if (StateMap->test(StateFlag::FILTYP)) {
	auto entryCount = gsl::narrow<int32_t>(EDGELIST.size());
	for (auto const& iEntry : EDGELIST) {
	  if ((form.edgeType & NEGUND) == iEntry.value) {
		--entryCount;
	  }
	  else {
		if ((iEntry.value == EDGECLIP || iEntry.value == EDGEPICOT || iEntry.value == EDGECLIPX) &&
		    (!StateMap->test(StateFlag::WASPCDCLP))) {
		  --entryCount;
		}
		else {
		  formForms::maxtsiz(displayText::loadStr(iEntry.stringID), sideWindowSize);
		}
	  }
	}
	SideMessageWindow = CreateWindow(L"STATIC",
	                                 nullptr,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 parentListRect.right - ThredWindowOrigin.x + 3,
	                                 childListRect.top - ThredWindowOrigin.y - 3,
	                                 sideWindowSize.cx + 12,
	                                 sideWindowSize.cy * entryCount + 12,
	                                 ThrEdWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
	for (auto const& iEntry : EDGELIST) {
	  if ((form.edgeType & NEGUND) != iEntry.value) {
		if (iEntry.value == EDGECLIP || iEntry.value == EDGEPICOT || iEntry.value == EDGECLIPX) {
		  if (StateMap->test(StateFlag::WASPCDCLP)) {
			dusid(iEntry, sideWindowLocation, sideWindowSize);
		  }
		}
		else {
		  dusid(iEntry, sideWindowLocation, sideWindowSize);
		}
	  }
	}
	StateMap->set(StateFlag::SIDACT);
	return;
  }
  auto entryCount = int32_t {0};
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
		  if ((((1U << iEntry.value) & CLIPTYPEMAP) != 0U) && (!StateMap->test(StateFlag::WASPCDCLP))) {
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
                                   sideWindowSize.cy * entryCount + 12,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  switch (FormMenuChoice) {
	case LLAYR: {
	  for (auto const& iEntry : LAYRLIST) {
		dusid(iEntry, sideWindowLocation, sideWindowSize);
	  }
	  break;
	}
	case LFTHTYP: {
	  for (auto const& iEntry : FTHRLIST) {
		if (iEntry.value != form.feather.fillType) {
		  dusid(iEntry, sideWindowLocation, sideWindowSize);
		}
	  }
	  break;
	}
	case LFRMFIL: {
	  for (auto const& iEntry : FILLLIST) {
		if (iEntry.value == form.fillType) {
		  continue;
		}
		if (((1U << iEntry.value) & CLIPTYPEMAP) == 0U) {
		  dusid(iEntry, sideWindowLocation, sideWindowSize);
		  continue;
		}
		if (StateMap->test(StateFlag::WASPCDCLP)) {
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
  StateMap->set(StateFlag::SIDACT);
}

auto thi::centr() noexcept -> F_POINT {
  auto const center = POINT {std::lround((ZoomRect.right - ZoomRect.left) * 0.5F),
                             std::lround((ZoomRect.top - ZoomRect.bottom) * 0.5F)};
  return F_POINT {ZoomRect.left + wrap::toFloat(center.x), ZoomRect.bottom + wrap::toFloat(center.y)};
}

void thi::stchWnd() {
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
                                   *ScrollSize,
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
                                     *ScrollSize,
                                     ThrEdWindow,
                                     nullptr,
                                     ThrEdInstance,
                                     nullptr);
  ShowWindow(VerticalScrollBar, SW_HIDE);
  ShowWindow(HorizontalScrollBar, SW_HIDE);
}

// check if a click occurred in a vertical set of windows
// and calculate which window had the click
auto thi::chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) -> bool {
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

auto thred::inDefaultColorWindows() -> bool {
  return thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back());
}

auto thred::inChangeThreadWindows() -> bool {
  return thi::chkMsgs(Msg.pt, ChangeThreadSizeWin.front(), ChangeThreadSizeWin.back());
}

auto thred::inThreadWindows() -> bool {
  return thi::chkMsgs(Msg.pt, ThreadSizeWin->front(), ThreadSizeWin->back());
}

auto thred::inUserColorWindows() -> bool {
  return thi::chkMsgs(Msg.pt, UserColorWin->front(), UserColorWin->back());
}

auto thred::inThreadSizeWindows() -> bool {
  return thi::chkMsgs(Msg.pt, ThreadSizeWin->front(), ThreadSizeWin->back());
}

void thi::delstch1(uint32_t iStitch) {
  if (StitchBuffer->empty()) {
	return;
  }
  StitchBuffer->erase(wrap::next(StitchBuffer->begin(), iStitch));
  if (ClosestPointIndex > wrap::toUnsigned(StitchBuffer->size()) - 1U) {
	ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size()) - 1U;
  }
}

void thi::prtred(HANDLE fileHandle, uint32_t code) {
  CloseHandle(fileHandle);
  StateMap->reset(StateFlag::INIT);
  FormList->clear();
  displayText::tabmsg(code, false);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thi::unthum() {
  if (!StateMap->testAndReset(StateFlag::THUMSHO)) {
	return;
  }
  thi::destroyBV();
  if (StateMap->test(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPON));
  }
  else {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
  }
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  thred::redraw(ButtonWin->operator[](HHID));
  displayText::butxt(HBOXSEL, displayText::loadStr(IDS_BOXSEL));
}

void thi::rstdu() {
  backup::deldu();
  menu::disableRedo();
  menu::disableUndo();
}

auto thi::getNewFileName(fs::path& newFileName, FileStyles fileTypes, FileIndices fileIndex) -> bool {
  auto* pFileOpen = gsl::narrow_cast<IFileOpenDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hResult = CoCreateInstance(
      CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  if (FAILED(hResult) || (nullptr == pFileOpen)) {
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
	      std::array<COMDLG_FILTERSPEC, 4> {FLTTHR, FLTPCS, FLTDST, FLTPES}; // All possible file types for save
#else
	  static constexpr auto ALL_FILE_TYPES = std::array<COMDLG_FILTERSPEC, 3> {FLTTHR, FLTPCS, FLTDST}; // All possible file types for save
#endif
	  hResult += pFileOpen->SetFileTypes(wrap::toUnsigned(ALL_FILE_TYPES.size()), ALL_FILE_TYPES.data());
	  break;
	}
	case FileStyles::INS_FILES: {
	  static constexpr auto INSERT_FILE_TYPES = std::array<COMDLG_FILTERSPEC, 2> {FLTTHR, FLTPCS}; // All possible file types that can be inserted into current design
	  hResult +=
	      pFileOpen->SetFileTypes(wrap::toUnsigned(INSERT_FILE_TYPES.size()), INSERT_FILE_TYPES.data());
	  break;
	}
	// resharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  outDebugString(L"Unknown file style\n");
	  throw;
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
	// resharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  outDebugString(L"Unknown file index\n");
	  throw;
	}
  }
  hResult += pFileOpen->SetTitle(L"Open Thred File");
#if USE_DEFAULTDIR
  // If we want to, we can set the default directory rather than using the OS mechanism for last used
  auto* psiFrom = gsl::narrow_cast<IShellItem*>(nullptr);
  hResult += SHCreateItemFromParsingName(DefaultDirectory->wstring().data(), nullptr, IID_PPV_ARGS(&psiFrom));
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
  if (FAILED(hResult) || (nullptr == pItem)) {
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

auto thred::getFileSize(fs::path const& newFileName, uintmax_t& size) -> bool {
  auto eCode = std::error_code {};
  size       = fs::file_size(newFileName, eCode);
  if (eCode != std::error_code {}) {
	// ToDo - find better error message
	displayText::filnopn(IDS_FNOPN, newFileName);
	return false;
  }
  return true;
}

auto thi::readTHRFile(std::filesystem::path const& newFileName) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto bytesRead   = DWORD {};
  auto thredHeader = THR_HEAD {};
  if (!wrap::readFile(fileHandle, &thredHeader, sizeof(thredHeader), &bytesRead, L"ReadFile for thredHeader in readTHRFile")) {
	return false;
  }
  if ((thredHeader.headerType & SIGMASK) != THREDSIG) {
	prtred(fileHandle, IDS_NOTHR);
	return false;
  }
  if (bytesRead != sizeof(thredHeader)) {
	prtred(fileHandle, IDS_SHRTF);
	return false;
  }
  auto const version = (thredHeader.headerType & FTYPMASK) >> TBYTSHFT;
  auto const spIDN   = gsl::span {IniFile.designerName};
  DesignerName->assign(utf::utf8ToUtf16(std::string(spIDN.data())));
  switch (version) {
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
	  ritfnam(*DesignerName);
	  auto const spModifierName = gsl::span {ExtendedHeader->modifierName};
	  std::copy(spIDN.begin(), wrap::next(spIDN.begin(), strlen(spIDN.data()) + 1U), spModifierName.begin());
	  break;
	}
	case 1:
	case 2: {
	  if (!wrap::readFile(fileHandle, ExtendedHeader, sizeof(*ExtendedHeader), &bytesRead, L"ReadFile for ExtendedHeader in readTHRFile")) {
		return false;
	  }
	  if (bytesRead != sizeof(*ExtendedHeader)) {
		prtred(fileHandle, IDS_SHRTF);
		return false;
	  }
	  IniFile.hoopSizeX = ExtendedHeader->hoopSizeX;
	  IniFile.hoopSizeY = ExtendedHeader->hoopSizeY;

	  UnzoomedRect = {std::lround(ExtendedHeader->hoopSizeX), std::lround(ExtendedHeader->hoopSizeY)};
	  redfnam(*DesignerName);
	  break;
	}
	default: {
	  prtred(fileHandle, IDS_NOTVER);
	  return false;
	}
  }
  ZoomRect     = F_RECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
  StitchBuffer->resize(thredHeader.stitchCount);
  if (thredHeader.stitchCount != 0U) {
	auto const bytesToRead = thredHeader.stitchCount * wrap::sizeofType(StitchBuffer);
	if (!wrap::readFile(fileHandle, StitchBuffer->data(), bytesToRead, &bytesRead, L"ReadFile for StitchBuffer in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  prtred(fileHandle, IDS_PRT);
	  return false;
	}
  }
  StitchBuffer->shrink_to_fit();
  auto bytesToRead = bitmap::getBmpNameLength();
  if (!wrap::readFile(fileHandle, bitmap::getBmpNameData(), bytesToRead, &bytesRead, L"ReadFile for getBmpNameData in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) {
	bitmap::resetBmpFile(true);
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  bytesToRead = sizeof(BackgroundColor);
  if (!wrap::readFile(fileHandle, &BackgroundColor, bytesToRead, &bytesRead, L"ReadFile for BackgroundColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) {
	BackgroundColor = IniFile.backgroundColor;
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  bytesToRead     = sizeof(UserColor);
  if (!wrap::readFile(fileHandle, UserColor.data(), bytesToRead, &bytesRead, L"ReadFile for UserColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) {
	UserColor = DEFAULT_COLORS;
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  bytesToRead = sizeof(CustomColor);
  if (!wrap::readFile(fileHandle, CustomColor.data(), bytesToRead, &bytesRead, L"ReadFile for CustomColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) {
	CustomColor = DEFAULT_COLORS;
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto msgBuffer = std::array<char, TSSIZE> {};
  if (!wrap::readFile(fileHandle, msgBuffer.data(), sizeof(msgBuffer), &bytesRead, L"ReadFile for msgBuffer in readTHRFile")) {
	return false;
  }
  if (bytesRead != TSSIZE) {
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto const threadSizebuf  = std::string(msgBuffer.data(), msgBuffer.size());
  auto       threadSizeBufW = utf::utf8ToUtf16(threadSizebuf);
  std::ranges::generate(ThreadSize, [tsBuffer = threadSizeBufW.begin()]() mutable noexcept -> wchar_t {
	return *(tsBuffer++);
  });
  if (thredHeader.formCount == 0) {
	CloseHandle(fileHandle);
	return true;
  }
  StateMap->reset(StateFlag::BADFIL);
  if (version < 2) {
	auto formListOriginal = std::vector<FRM_HEAD_O> {};
	formListOriginal.resize(thredHeader.formCount);
	bytesToRead = thredHeader.formCount * wrap::sizeofType(formListOriginal);
	if (!wrap::readFile(fileHandle, formListOriginal.data(), bytesToRead, &bytesRead, L"ReadFile for formListOriginal in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  wrap::narrow(thredHeader.formCount, bytesRead / wrap::sizeofType(formListOriginal));
	  formListOriginal.resize(thredHeader.formCount);
	  StateMap->set(StateFlag::BADFIL);
	}
	FormList->reserve(formListOriginal.size());
	FormList->insert(FormList->end(), formListOriginal.begin(), formListOriginal.end());
  }
  else {
	auto inFormList = std::vector<FRM_HEAD_OUT> {};
	inFormList.resize(thredHeader.formCount);
	bytesToRead = thredHeader.formCount * wrap::sizeofType(inFormList);
	if (!wrap::readFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, L"ReadFile for inFormList in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  wrap::narrow(thredHeader.formCount, bytesRead / wrap::sizeofType(inFormList));
	  inFormList.resize(thredHeader.formCount);
	  StateMap->set(StateFlag::BADFIL);
	}
	FormList->reserve(inFormList.size());
	FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
  }
  FormList->shrink_to_fit();
  if (thredHeader.vertexCount != 0U) {
	FormVertices->resize(thredHeader.vertexCount);
	bytesToRead = thredHeader.vertexCount * wrap::sizeofType(FormVertices);
	if (!wrap::readFile(fileHandle, FormVertices->data(), bytesToRead, &bytesRead, L"ReadFile for FormVertices in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  FormVertices->resize(bytesRead / wrap::sizeofType(FormVertices));
	  StateMap->set(StateFlag::BADFIL);
	}
  }
  else {
	// We have forms but no vertices - blow up the read
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  FormVertices->shrink_to_fit();
  if (thredHeader.dlineCount != 0U) {
	auto inGuideList = std::vector<SAT_CON_OUT>(thredHeader.dlineCount);
	bytesToRead      = thredHeader.dlineCount * wrap::sizeofType(inGuideList);
	if (!wrap::readFile(fileHandle, inGuideList.data(), bytesToRead, &bytesRead, L"ReadFile for inGuideList in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  inGuideList.resize(bytesRead / wrap::sizeofType(inGuideList));
	  StateMap->set(StateFlag::BADFIL);
	}
	SatinGuides->reserve(inGuideList.size());
	SatinGuides->insert(SatinGuides->end(), inGuideList.begin(), inGuideList.end());
  }
  SatinGuides->shrink_to_fit();
  if (thredHeader.clipDataCount != 0U) {
	ClipPoints->resize(thredHeader.clipDataCount);
	bytesToRead = thredHeader.clipDataCount * wrap::sizeofType(ClipPoints);
	if (!wrap::readFile(fileHandle, ClipPoints->data(), bytesToRead, &bytesRead, L"ReadFile for ClipPoints in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  ClipPoints->resize(bytesRead / wrap::sizeofType(ClipPoints));
	  StateMap->set(StateFlag::BADFIL);
	}
  }
  ClipPoints->shrink_to_fit();
  if (ExtendedHeader->texturePointCount != 0U) {
	TexturePointsBuffer->resize(ExtendedHeader->texturePointCount);
	bytesToRead = ExtendedHeader->texturePointCount * wrap::sizeofType(TexturePointsBuffer);
	if (!wrap::readFile(fileHandle, TexturePointsBuffer->data(), bytesToRead, &bytesRead, L"ReadFile for TexturePointsBuffer in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  TexturePointsBuffer->resize(bytesRead / wrap::sizeofType(TexturePointsBuffer));
	  StateMap->set(StateFlag::BADFIL);
	}
  }
  else {
	TexturePointsBuffer->clear();
  }
  TexturePointsBuffer->shrink_to_fit();
  if (StateMap->testAndReset(StateFlag::BADFIL)) {
	displayText::bfilmsg();
  }
  // now re-create all the pointers/indexes in the form data
  auto clipOffset   = 0U;
  auto vertexOffset = 0U;
  auto guideOffset  = 0U;
  for (auto& iForm : *FormList) {
	iForm.vertexIndex = vertexOffset;
	vertexOffset += iForm.vertexCount;
	if (iForm.type == SAT && (iForm.satinGuideCount != 0U)) {
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

auto thred::getFileHandle(fs::path const& newFileName, HANDLE& fileHandle) -> bool {
  // ToDo - use ifstream?
  // ifstream file(WorkingFileName, ios::in | ios::binary | ios::ate);
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

void thi::resetState() {
  fnamtabs();
  trace::untrace();
  if (!FormList->empty()) {
	form::delfrms();
  }
  unthum();
  thred::unbsho();
  bitmap::resetBmpFile(true);
  TexturePointsBuffer->clear();
  menu::disableRedo();
  thred::unbsho();
  form::frmon();
  SelectedFormList->clear();
  SelectedFormList->shrink_to_fit();
  if (StateMap->test(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	PreferenceIndex = 0;
  }
  bitmap::resetBmpFile(true);
  SearchLine->clear();
  SearchLine->shrink_to_fit();
  rstdu();
  thred::unmsg();
  ZoomFactor   = 1;
  NearestCount = 0;
  if (StateMap->testAndReset(StateFlag::WASPAT)) {
	DestroyWindow(SpeedScrollBar);
  }
  auto textureHistoryFlag = false;
  if (StateMap->test(StateFlag::WASTXBAK)) {
	textureHistoryFlag = true;
  }
  StateMap->reset();
  if (textureHistoryFlag) {
	StateMap->set(StateFlag::WASTXBAK);
  }
}

void thi::nuFil(FileIndices fileIndex) {
  // Todo - check filename for validity before using it
  auto newFileName = *WorkingFileName; // intentional copy
  if (!StateMap->testAndReset(StateFlag::REDOLD) &&
      !getNewFileName(newFileName, FileStyles::ALL_FILES, fileIndex)) {
	return;
  }
  WorkingFileName->assign(newFileName);
  defNam(newFileName);
  resetState();
  auto const fileExt = newFileName.extension().wstring();
  if (fileExt.empty()) {
	return;
  }
  auto const firstCharacter = tolower(fileExt[1]);
  if (firstCharacter == 't') {
	if (!readTHRFile(newFileName)) {
	  return;
	}
  }
  else {
	StateMap->set(StateFlag::NOTHRFIL);
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
	bitmap::assignUBFilename(*DefaultDirectory);
  }
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  ZoomRect     = F_RECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
  thred::movStch();
  thred::coltab();
  StateMap->reset(StateFlag::ZUMED);
  auto buffer       = std::array<wchar_t, 3> {};
  buffer[1]         = L'0';
  auto itUserPen    = UserPen->begin();
  auto ucb          = UserColorBrush.begin();
  auto itThreadSize = ThreadSize.begin();
  auto tsw          = ThreadSizeWin->begin();
  for (auto const& color : UserColor) {
	thred::nuPen(*itUserPen, 1, color);
	++itUserPen;
	thi::nuBrush(*ucb, color);
	++ucb;
	buffer[0] = *(itThreadSize++);
	SetWindowText(*(tsw++), buffer.data());
  }
  for (auto const& iColor : *UserColorWin) {
	thred::redraw(iColor);
  }
  thred::redrawColorBar();
  if (!StitchBuffer->empty()) {
	StateMap->set(StateFlag::INIT);
  }
  thred::nuAct(0);
  StateMap->set(StateFlag::RESTCH);
  nunams();
  ritini();
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  if (!StitchBuffer->empty()) {
	thred::nuAct(StitchBuffer->front().attribute & COLMSK);
  }
  else {
	thred::nuAct(0);
  }
  menu::auxmen();
  thred::lenCalc();
  auto const fmtStr = displayText::format2(IDS_THRDBY, newFileName.wstring(), *DesignerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
  StateMap->set(StateFlag::INIT);
  StateMap->reset(StateFlag::TRSET);
  if (StateMap->test(StateFlag::NOTHRFIL)) {
	for (auto& stitch : *StitchBuffer) {
	  stitch.attribute |= NOTFRM;
	}
  }
  repair::lodchk();
}

void thred::nuTHRfile() {
  thi::nuFil(FileIndices::THR);
}

auto thi::nuCol(COLORREF init) noexcept -> BOOL {
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

auto thi::nuBak() noexcept -> BOOL {
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

auto thred::pxCor2stch(POINT const& point) noexcept -> F_POINT {
  auto const ratioX =
      wrap::toFloat(point.x - StitchWindowAbsRect.left) / wrap::toFloat(StitchWindowClientRect.right);
  auto const ratioY =
      wrap::toFloat(StitchWindowAbsRect.bottom - point.y) / wrap::toFloat(StitchWindowClientRect.bottom);
  return F_POINT {ratioX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left,
                  ratioY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom};
}

auto thred::inStitchWin() noexcept -> bool {
  return (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right &&
          Msg.pt.y >= StitchWindowAbsRect.top && Msg.pt.y <= StitchWindowAbsRect.bottom);
}

void thred::zumin() {
  if (!StateMap->testAndReset(StateFlag::ZUMACT)) {
	ZoomFactor *= ZUMFCT;
  }
  if (ZoomFactor < ZoomMin) {
	ZoomFactor = ZoomMin;
  }
  if (!StateMap->testAndSet(StateFlag::ZUMED)) {
	thred::movStch();
  }
  auto stitchPoint = thred::pxCor2stch(Msg.pt);
  if (!StateMap->testAndReset(StateFlag::BZUMIN)) {
	while (true) {
	  if (StateMap->test(StateFlag::GMRK)) {
		stitchPoint = ZoomMarkPoint;
		break;
	  }
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto const& boundingRect = FormList->operator[](ClosestFormToCursor).rectangle;

		stitchPoint = F_POINT {wrap::midl(boundingRect.right, boundingRect.left),
		                       wrap::midl(boundingRect.top, boundingRect.bottom)};
		break;
	  }
	  if (StateMap->test(StateFlag::FRMPSEL)) {
		auto const itVertex =
		    wrap::next(FormVertices->cbegin(),
		               FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
		stitchPoint = *itVertex;
		break;
	  }
	  if (StateMap->test(StateFlag::SELBOX)) {
		stitchPoint = StitchBuffer->operator[](ClosestPointIndex);
		break;
	  }
	  if (StateMap->test(StateFlag::GRPSEL)) {
		auto groupBoundingRect = F_RECTANGLE {};
		thred::selRct(groupBoundingRect);
		stitchPoint = F_POINT {wrap::midl(groupBoundingRect.right, groupBoundingRect.left),
		                       wrap::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
		break;
	  }
	  if (StateMap->test(StateFlag::INSRT)) {
		if (StateMap->test(StateFlag::LIN1)) {
		  stitchPoint = StateMap->test(StateFlag::BAKEND) ? StitchBuffer->back() : StitchBuffer->front();
		}
		else {
		  stitchPoint =
		      F_POINT {wrap::midl(StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).x,
		                          StitchBuffer->operator[](ClosestPointIndex).x),
		               wrap::midl(StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).y,
		                          StitchBuffer->operator[](ClosestPointIndex).y)};
		}
		break;
	  }
	  if (!SelectedFormList->empty()) {
		auto const  firstForm = SelectedFormList->front();
		auto const& firstRect = FormList->operator[](firstForm).rectangle;

		SelectedFormsRect = {std::lround(firstRect.left),
		                     std::lround(firstRect.top),
		                     std::lround(firstRect.right),
		                     std::lround(firstRect.bottom)};
		for (auto const selectedForm : (*SelectedFormList)) {
		  auto const& rect = FormList->operator[](selectedForm).rectangle;
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
	  if (!thred::inStitchWin()) {
		stitchPoint = thi::centr();
	  }
	  break;
	}
  }
  auto const zoomRight = wrap::toFloat(UnzoomedRect.cx) * ZoomFactor;
  ZoomRect             = F_RECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
  thred::shft(stitchPoint);
  NearestCount = 0;
  if (!StateMap->test(StateFlag::GMRK) && StateMap->test(StateFlag::SELBOX)) {
	thi::shft2box();
  }
  if (StateMap->test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  thred::duzrat();
  thi::movins();
  StateMap->set(StateFlag::RESTCH);
  thred::ilin();
}

void thred::zumshft() {
  if (StateMap->test(StateFlag::ZUMED)) {
	thi::unboxs();
	if (thred::inStitchWin()) {
	  NearestCount = 0;
	  thred::shft(thred::pxCor2stch(Msg.pt));
	  if (StateMap->test(StateFlag::RUNPAT)) {
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		RunPoint = 0;
	  }
	  StateMap->set(StateFlag::RESTCH);
	}
  }
  thi::movins();
}

void thred::zumout() {
  thi::unboxs();
  auto stitchPoint = thred::pxCor2stch(Msg.pt);
  if (!StateMap->test(StateFlag::ZUMED)) {
	return;
  }
  while (true) {
	if (StateMap->test(StateFlag::GMRK)) {
	  stitchPoint = ZoomMarkPoint;
	  break;
	}
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto const& boundingRect = FormList->operator[](ClosestFormToCursor).rectangle;

	  stitchPoint = F_POINT {wrap::midl(boundingRect.right, boundingRect.left),
	                         wrap::midl(boundingRect.top, boundingRect.bottom)};
	  break;
	}
	if (StateMap->test(StateFlag::FRMPSEL)) {
	  auto const itVertex =
	      wrap::next(FormVertices->cbegin(),
	                 FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	  stitchPoint = *itVertex;
	  break;
	}
	if (StateMap->test(StateFlag::SELBOX) || StateMap->test(StateFlag::INSRT)) {
	  stitchPoint = StitchBuffer->operator[](ClosestPointIndex);
	  break;
	}
	if (StateMap->test(StateFlag::GRPSEL)) {
	  auto groupBoundingRect = F_RECTANGLE {};
	  thred::selRct(groupBoundingRect);
	  stitchPoint = F_POINT {wrap::midl(groupBoundingRect.right, groupBoundingRect.left),
	                         wrap::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
	  break;
	}
	if (StateMap->test(StateFlag::SELBOX)) {
	  thi::shft2box();
	  break;
	}
	if (!thred::inStitchWin()) {
	  stitchPoint = thi::centr();
	}
	break;
  }
  ZoomFactor /= ZUMFCT;
  constexpr auto ZMCLAMP = 0.98F; // clamp the zoom factor
  if (ZoomFactor > ZMCLAMP) {
	ZoomFactor = 1;
	StateMap->reset(StateFlag::ZUMED);
	ZoomRect = F_RECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.cy), wrap::toFloat(UnzoomedRect.cx), 0.0F};
	thred::movStch();
	NearestCount = 0;
  }
  else {
	auto const zoomRight = wrap::toFloat(UnzoomedRect.cx) * ZoomFactor;
	ZoomRect             = F_RECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
	thred::shft(stitchPoint);
  }
  if (StateMap->test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  StateMap->set(StateFlag::RESTCH);
  thred::duzrat();
  thi::movins();
}

void thi::duClos(uint32_t            startStitch,
                 uint32_t            stitchCount,
                 F_POINT const&      stitchPoint,
                 std::vector<float>& gapToNearest) noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  auto stitch = wrap::next(StitchBuffer->begin(), startStitch);
  for (auto iStitch = startStitch; iStitch < startStitch + stitchCount; ++iStitch) {
	auto const deltaX = stitch->x - stitchPoint.x;
	auto const deltaY = stitch->y - stitchPoint.y;

	auto sum       = deltaX * deltaX + deltaY * deltaY;
	auto tempPoint = iStitch;
	auto gap       = gapToNearest.begin();
	for (auto& point : *NearestPoint) {
	  if (sum < *gap) {
		std::swap(*gap, sum);
		std::swap(point, tempPoint);
	  }
	  ++gap;
	}
	++stitch;
  }
}

void thred::closPnt() {
  unbox();
  thi::unboxs();
  auto gapToNearest = std::vector<float> {};           // distances of the closest points
  gapToNearest.resize(NearestPoint->size(), BIGFLOAT); // to a mouse click
  NearestPoint->assign(NearestPoint->size(), BIGUINT);
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	auto const iStitch0 = ColorChangeTable->operator[](iColor).stitchIndex;

	auto const iStitch1 =
	    gsl::narrow_cast<decltype(iStitch0)>(ColorChangeTable->operator[](iColor + 1U).stitchIndex - iStitch0);
	if (StateMap->test(StateFlag::HID)) {
	  if (ColorChangeTable->operator[](iColor).colorIndex == ActiveColor) {
		thi::duClos(iStitch0, iStitch1, stitchPoint, gapToNearest);
	  }
	}
	else {
	  thi::duClos(iStitch0, iStitch1, stitchPoint, gapToNearest);
	}
  }
  GetClientRect(MainStitchWin, &StitchWindowClientRect);
  auto stitchCoordsInPixels = POINT {};
  for (auto const& iNear : *NearestPoint) {
	if (stch2px(iNear, stitchCoordsInPixels)) {
	  NearestPoint->operator[](NearestCount)   = iNear;
	  NearestPixel->operator[](NearestCount++) = stitchCoordsInPixels;
	}
  }
  thi::boxs();
}

auto thred::closPnt1(uint32_t& closestStitch) -> bool {
  auto closestIndex = 0U;
  auto const pointToCheck = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto stitchCoordsInPixels = POINT {};
  if (StateMap->test(StateFlag::SELBOX) && thred::stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	if (std::hypot(stitchCoordsInPixels.x - pointToCheck.x, stitchCoordsInPixels.y - pointToCheck.y) < CLOSENUF) {
	  closestStitch = ClosestPointIndex;
	  return true;
	}
  }
  if (NearestCount != 0) {
	auto itBoxOffset = BoxOffset.begin();
	auto npi         = NearestPixel->begin();
	auto npo         = NearestPoint->begin();
	for (auto iNear = 0U; iNear < NearestCount; ++iNear) {
	  auto const  offset = *(itBoxOffset++);
	  auto const& pixel  = *(npi++);
	  if (pointToCheck.x >= pixel.x - offset && pointToCheck.x <= pixel.x + offset &&
	      pointToCheck.y >= pixel.y - offset && pointToCheck.y <= pixel.y + offset) {
		closestStitch = *npo;
		return true;
	  }
	  ++npo;
	}
  }
  auto const stitchPoint     = thred::pxCor2stch(Msg.pt);
  auto       distanceToClick = BIGFLOAT;
  if (StateMap->test(StateFlag::HID)) {
	auto const maxIt = wrap::next(ColorChangeTable->end(), -1);
	for (auto colorIt = ColorChangeTable->begin(); colorIt != maxIt; ++colorIt) {
	  if (colorIt->colorIndex != ActiveColor) {
		continue;
	  }
	  auto       stitch   = std::next(StitchBuffer->begin(), colorIt->stitchIndex);
	  auto const maxColor = std::next(colorIt)->stitchIndex;
	  for (auto iStitch = colorIt->stitchIndex; iStitch < maxColor; ++iStitch) {
		if (stitch->x < ZoomRect.left || stitch->x > ZoomRect.right ||
		    stitch->y < ZoomRect.bottom || stitch->y > ZoomRect.top) {
		  ++stitch;
		  continue;
		}
		auto const deltaX   = stitch->x - stitchPoint.x;
		auto const deltaY   = stitch->y - stitchPoint.y;
		auto const distance = deltaX * deltaX + deltaY * deltaY;
		if (distance < distanceToClick) {
		  distanceToClick = distance;
		  closestIndex    = iStitch;
		}
		++stitch;
	  }
	}
  }
  else {
	auto currentStitch = 0U;
	for (auto const& stitch : *StitchBuffer) {
	  auto const layer = (stitch.attribute & LAYMSK) >> LAYSHFT;
	  if ((ActiveLayer != 0U) && (layer != 0U) && (layer != ActiveLayer)) {
		++currentStitch;
		continue;
	  }
	  if (stitch.x < ZoomRect.left || stitch.x > ZoomRect.right || stitch.y < ZoomRect.bottom ||
	      stitch.y > ZoomRect.top) {
		++currentStitch;
		continue;
	  }
	  auto const deltaX   = stitch.x - stitchPoint.x;
	  auto const deltaY   = stitch.y - stitchPoint.y;
	  auto const distance = deltaX * deltaX + deltaY * deltaY;
	  if (distance < distanceToClick) {
		distanceToClick = distance;
		closestIndex    = currentStitch;
	  }
	  ++currentStitch;
	}
  }
  if (util::closeEnough(distanceToClick, BIGFLOAT)) {
	return false;
  }
  thred::stch2px(closestIndex, stitchCoordsInPixels);
  if (StateMap->test(StateFlag::IGNTHR)) {
	closestStitch = closestIndex;
	return true;
  }
  if (std::hypot(pointToCheck.x - stitchCoordsInPixels.x, pointToCheck.y - stitchCoordsInPixels.y) < CLOSENUF) {
	closestStitch = closestIndex;
	return true;
  }
  return false;
}

auto thred::pt2colInd(uint32_t iStitch) noexcept -> uint32_t {
  auto iColor = 0U;
  for (; iColor < thred::maxColor(); ++iColor) {
	if (ColorChangeTable->operator[](iColor).stitchIndex > iStitch) {
	  break;
	}
  }
  return iColor;
}

auto thred::findFirstStitch(uint32_t form) -> uint32_t { // find the first stitch in the selected form
  auto const formFirstStitch = std::ranges::find_if(*StitchBuffer, [form](F_POINT_ATTR const& stitch) -> bool {
	if ((stitch.attribute & NOTFRM) != 0U) {
	  return false;
	}
	return ((stitch.attribute & FRMSK) >> FRMSHFT) == form;
  });
  if (formFirstStitch != StitchBuffer->end()) {
	return wrap::toUnsigned(std::distance(StitchBuffer->begin(), formFirstStitch));
  }
  return wrap::toUnsigned(StitchBuffer->size() - 1U);
}

auto thred::findLastStitch(uint32_t form) -> uint32_t { // find the first stitch in the selected form
  auto const formFirstStitch =
      std::find_if(StitchBuffer->rbegin(), StitchBuffer->rend(), [form](F_POINT_ATTR const& stitch) -> bool {
	    if ((stitch.attribute & NOTFRM) != 0U) {
	      return false;
	    }
	    return ((stitch.attribute & FRMSK) >> FRMSHFT) == form;
      });
  if (formFirstStitch != StitchBuffer->rend()) {
	return wrap::toUnsigned(std::distance(formFirstStitch, StitchBuffer->rend()) - 1);
  }
  return wrap::toUnsigned(StitchBuffer->size() - 1U);
}

void thred::toglup() {
  if (StateMap->testAndFlip(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	thred::rngadj();
	ClosestPointIndex = GroupStartStitch;
	StateMap->set(StateFlag::SELBOX);
	StateMap->reset(StateFlag::FRMPSEL);
  }
  else {
	if (!StateMap->test(StateFlag::SELBOX)) {
	  ClosestPointIndex = 0;
	  if (StateMap->testAndReset(StateFlag::FORMSEL)) {
		ClosestPointIndex = thred::findFirstStitch(ClosestFormToCursor);
		StateMap->set(StateFlag::SELBOX);
		StateMap->reset(StateFlag::FRMPSEL);
	  }
	}
  }
  displayText::butxt(HUPTO, displayText::loadStr(IDS_UPON));
  StateMap->set(StateFlag::RESTCH);
}

void thred::toglHid() {
  if (StateMap->testAndFlip(StateFlag::HID)) {
	StateMap->reset(StateFlag::FRMOF);
  }
  else {
	StateMap->set(StateFlag::FRMOF);
  }
  thi::unthum();
  thred::redraw(ButtonWin->operator[](HHID));
  StateMap->set(StateFlag::RESTCH);
}

void thi::dulin(std::array<POINT, 2> const& moveLine0, std::array<POINT, 2> const& moveLine1) {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (moveLine0[0].x == moveLine1[1].x && moveLine0[0].y == moveLine1[1].y) {
	if (StateMap->test(StateFlag::ISDWN)) {
	  wrap::polyline(StitchWindowDC, moveLine0.data(), wrap::toUnsigned(moveLine0.size()));
	}
	else {
	  wrap::polyline(StitchWindowDC, moveLine1.data(), wrap::toUnsigned(moveLine1.size()));
	}
  }
  else {
	if (StateMap->test(StateFlag::ISDWN)) {
	  wrap::polyline(StitchWindowDC, moveLine0.data(), wrap::toUnsigned(moveLine0.size()));
	}
	if (StateMap->test(StateFlag::ISUP)) {
	  wrap::polyline(StitchWindowDC, moveLine1.data(), wrap::toUnsigned(moveLine1.size()));
	}
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
  StateMap->flip(StateFlag::WASLIN);
}

void thred::unlin() {
  if (StateMap->test(StateFlag::WASLIN)) {
	thi::dulin(MoveLine0, MoveLine1);
  }
}

void thred::movbox() {
  if (StitchBuffer->empty()) {
	return;
  }
  auto stitchCoordsInPixels = POINT {};
  if (thred::stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	thred::unbox();
#ifdef _DEBUG
	auto const& stitch = StitchBuffer->operator[](ClosestPointIndex);
	outDebugString(L"movbox:Stitch [{}] form [{}] type [{}] x [{}] y[{}]\n",
	               ClosestPointIndex,
	               ((stitch.attribute & FRMSK) >> FRMSHFT),
	               ((stitch.attribute & TYPMSK) >> TYPSHFT),
	               stitch.x,
	               stitch.y);
#endif
	thred::dubox(stitchCoordsInPixels);
	if (StateMap->test(StateFlag::UPTO)) {
	  StateMap->set(StateFlag::RESTCH);
	}
  }
  else {
	thi::shft2box();
	StateMap->set(StateFlag::SELBOX);
	StateMap->reset(StateFlag::FRMPSEL);
	StateMap->set(StateFlag::RESTCH);
  }
  thred::nuAct(ClosestPointIndex);
  thi::ritcor(StitchBuffer->operator[](ClosestPointIndex));
}

auto thi::chkhid(size_t colorToCheck) -> bool {
  if (StateMap->test(StateFlag::HID)) {
	return ColorChangeTable->operator[](colorToCheck).colorIndex == ActiveColor;
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

auto thred::closlin() -> uint32_t {
  // ToDo - This function needs to be thoroughly checked and compared to findDistanceToSide
  auto           closestPoint = BIGUINT;
  constexpr auto TLFACTOR     = 20.0F; // tolerance ratio
  thi::unboxs();
  auto offset = wrap::toFloat(Msg.pt.x - StitchWindowAbsRect.left) /
                wrap::toFloat(StitchWindowAbsRect.right - StitchWindowAbsRect.left);
  auto const offsetX = offset * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;

  offset = wrap::toFloat(StitchWindowAbsRect.bottom - Msg.pt.y) /
           wrap::toFloat(StitchWindowAbsRect.bottom - StitchWindowAbsRect.top);
  auto const offsetY = (offset * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom);
  offset         = (ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right);
  auto       sum = BIGFLOAT;
  auto const tolerance = offset * TLFACTOR;
  for (auto iChange = size_t {}; iChange < thred::maxColor(); ++iChange) {
	auto stitchCount =
	    gsl::narrow_cast<ptrdiff_t>(std::abs(ColorChangeTable->operator[](iChange + 1U).stitchIndex -
	                                         ColorChangeTable->operator[](iChange).stitchIndex));
	if (ColorChangeTable->operator[](iChange + 1U).stitchIndex == StitchBuffer->size()) {
	  --stitchCount;
	}
	auto const stitches = wrap::next(StitchBuffer->begin(), ColorChangeTable->operator[](iChange).stitchIndex);
	if (!thi::chkhid(iChange)) {
	  continue;
	}
	for (auto iStitch = ptrdiff_t {}; iStitch < stitchCount; ++iStitch) {
	  auto const layer = (stitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
	  if ((ActiveLayer != 0U) && (layer != 0U) && (layer != ActiveLayer)) {
		continue;
	  }
	  auto const xba = std::lround(stitches[iStitch + 1].x - stitches[iStitch].x);
	  auto const yab = std::lround(stitches[iStitch].y - stitches[iStitch + 1].y);
	  auto left = (xba > 0) ? stitches[iStitch].x - tolerance : stitches[iStitch + 1].x - tolerance;
	  auto right = (xba > 0) ? stitches[iStitch + 1].x + tolerance : stitches[iStitch].x + tolerance;
	  auto bottom = (yab < 0) ? stitches[iStitch].y - tolerance : stitches[iStitch + 1].y - tolerance;
	  auto top = (yab < 0) ? stitches[iStitch + 1].y + tolerance : stitches[iStitch].y + tolerance;
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
		  auto const poff  = offsetX - offsetY / slope;

		  intersection =
		      F_POINT {offset - slope * intersection.y, slope * (offset - poff) / (slope * slope + 1.0F)};
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
		sum = tsum;
		closestPoint = wrap::toUnsigned(iStitch) + ColorChangeTable->operator[](iChange).stitchIndex;
	  }
	}
  }
  if (sum > (tolerance * tolerance)) {
	return BIGUINT;
  }
  return closestPoint;
}

void thred::selCol() {
  if (StitchBuffer->empty()) {
	return;
  }
  // NOLINTBEGIN(readability-avoid-nested-conditional-operator)
  auto iStitch = (StateMap->test(StateFlag::SELBOX))   ? ClosestPointIndex
                 : (StateMap->test(StateFlag::GRPSEL)) ? GroupStitchIndex
                                                       : 0;
  // NOLINTEND(readability-avoid-nested-conditional-operator)
  if (iStitch > wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	iStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
  }
  GroupStitchIndex    = iStitch;
  ClosestPointIndex   = iStitch;
  auto const stitchIt = wrap::next(StitchBuffer->begin(), iStitch);
  auto const color    = gsl::narrow_cast<uint8_t>(stitchIt->attribute & COLMSK);
  while ((ClosestPointIndex != 0U) &&
         gsl::narrow_cast<uint8_t>(StitchBuffer->operator[](ClosestPointIndex).attribute & COLMSK) == color) {
	--ClosestPointIndex;
  }
  if ((StitchBuffer->operator[](ClosestPointIndex).attribute & COLMSK) != color) {
	++ClosestPointIndex;
  }
  while (GroupStitchIndex < wrap::toUnsigned(StitchBuffer->size() - 1U) &&
         (StitchBuffer->operator[](GroupStitchIndex).attribute & COLMSK) == color) {
	++GroupStitchIndex;
  }
  if ((StitchBuffer->operator[](ClosestPointIndex).attribute & COLMSK) != color) {
	--ClosestPointIndex;
  }
  if (GroupStitchIndex > wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
  }
  StateMap->set(StateFlag::GRPSEL);
  thred::unbox();
  thred::grpAdj();
  ActiveColor = color;
  StateMap->set(StateFlag::RESTCH);
}

void thred::newFil() {
  StateMap->reset(StateFlag::CMPDO);
  bitmap::resetBmpFile(true);
  backup::deldu();
  DesignerName->assign(utf::utf8ToUtf16(std::string(IniFile.designerName.data())));
  auto const fmtStr = displayText::format(IDS_THRED, *DesignerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
  *ThrName = *DefaultDirectory / (displayText::loadStr(IDS_NUFIL).c_str());
  thi::ritfnam(*DesignerName);
  auto const designer       = utf::utf16ToUtf8(*DesignerName);
  auto const spModifierName = gsl::span {ExtendedHeader->modifierName};
  std::ranges::copy(designer, spModifierName.begin());
  spModifierName[designer.length()] = 0;
  thi::rstdu();
  thred::rstAll();
  displayText::clrhbut(3);
  StateMap->reset(StateFlag::MOVSET);
  thred::unbox();
  thred::xlin();
  thred::xlin1();
  StateMap->reset(StateFlag::INIT);
  StateMap->reset(StateFlag::INSRT);
  StateMap->reset(StateFlag::LIN1);
  StateMap->reset(StateFlag::FORMSEL);
  StateMap->reset(StateFlag::BAKACT);
  StateMap->reset(StateFlag::GMRK);
  StitchBuffer->clear();
  StitchBuffer->shrink_to_fit();
  DisplayedColorBitmap.reset();
  bitmap::resetBmpFile(true);
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
  thred::resetColorChanges();
  Knots->clear();
  WorkingFileName->clear();
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	thred::redraw(DefaultColorWin->operator[](iColor));
	thred::redraw(UserColorWin->operator[](iColor));
	thred::redraw(ThreadSizeWin->operator[](iColor));
  }
  thred::zumhom();
}

void thred::bBox() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::polyline(StitchWindowDC, ZoomBoxLine.data(), wrap::toUnsigned(ZoomBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unbBox() {
  if (StateMap->testAndReset(StateFlag::BZUM)) {
	thred::bBox();
  }
}

void thred::rebox() {
  thred::unbox();
  if (!thred::closPnt1(ClosestPointIndex)) {
	return;
  }
  thred::nuAct(ClosestPointIndex);
  auto stitchCoordsInPixels = POINT {};
  if (thred::stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	thred::dubox(stitchCoordsInPixels);
	outDebugString(L"rebox:Stitch [{}] form [{}] type [{}]\n",
	               ClosestPointIndex,
	               ((StitchBuffer->operator[](ClosestPointIndex).attribute & FRMSK) >> FRMSHFT),
	               ((StitchBuffer->operator[](ClosestPointIndex).attribute & TYPMSK) >> TYPSHFT));
  }
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	StateMap->reset(StateFlag::SCROS);
	StateMap->reset(StateFlag::ECROS);
	SearchLine->clear();
	SearchLine->shrink_to_fit();
	StateMap->set(StateFlag::RESTCH);
	for (auto const& window : *UserColorWin) {
	  thred::redraw(window);
	}
  }
  thi::ritcor(StitchBuffer->operator[](ClosestPointIndex));
}

void thred::delstchm() {
  thred::rngadj();
  auto const start = wrap::next(StitchBuffer->begin(), GroupStartStitch);
  auto const end   = wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U);
  StitchBuffer->erase(start, end);
}

void thi::duclp() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::polyline(StitchWindowDC, ClipInsertBoxLine.data(), wrap::toUnsigned(ClipInsertBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unclp() {
  if (StateMap->testAndReset(StateFlag::CLPSHO)) {
	thi::duclp();
  }
}

void thred::clpbox() {
  auto const ratio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  auto       stitchPoint = thred::pxCor2stch(Msg.pt);
  auto const unzoomedX   = wrap::toFloat(UnzoomedRect.cx);
  if (stitchPoint.x + ClipRectSize.cx > unzoomedX) {
	stitchPoint.x = unzoomedX - ClipRectSize.cx;
  }
  auto const unzoomedY = wrap::toFloat(UnzoomedRect.cy);
  if (stitchPoint.y + ClipRectSize.cy > unzoomedY) {
	stitchPoint.y = unzoomedY - ClipRectSize.cy;
  }
  tfc::setClipOrigin({std::lround(stitchPoint.x), std::lround(stitchPoint.y)});
  auto const adjustedSize =
      SIZE {wrap::ceil<int32_t>(ClipRectSize.cx * ratio), wrap::ceil<int32_t>(ClipRectSize.cy * ratio)};
  auto const stitchCoordsInPixels = POINT {
      wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ratio),
      wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                          (stitchPoint.y - ZoomRect.bottom) * ratio - wrap::toFloat(adjustedSize.cy))};
  ClipInsertBoxLine[0].x = ClipInsertBoxLine[3].x = ClipInsertBoxLine[4].x = stitchCoordsInPixels.x;
  ClipInsertBoxLine[0].y = ClipInsertBoxLine[1].y = ClipInsertBoxLine[4].y = stitchCoordsInPixels.y;
  ClipInsertBoxLine[1].x = ClipInsertBoxLine[2].x = ClipInsertBoxLine[0].x + adjustedSize.cx;
  ClipInsertBoxLine[2].y = ClipInsertBoxLine[3].y = ClipInsertBoxLine[0].y + adjustedSize.cy;
  StateMap->set(StateFlag::CLPSHO);
  thi::duclp();
}

void thred::rSelbox() {
  auto const ratio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  auto const adjustedSelectSize =
      SIZE {gsl::narrow_cast<int32_t>(lround(wrap::toFloat(SelectBoxSize.cx) * ratio)),
            gsl::narrow_cast<int32_t>(lround(wrap::toFloat(SelectBoxSize.cy) * ratio))};
  unsel();
  auto stitchPoint = thred::pxCor2stch(Msg.pt);
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
                          (stitchPoint.y - ZoomRect.bottom - wrap::toFloat(SelectBoxOffset.y)) * ratio -
                          wrap::toFloat(adjustedSelectSize.cy))};
  auto& frmCtrls  = *FormControlPoints;
  frmCtrls[PTL].x = frmCtrls[PBL].x = frmCtrls[PLM].x = frmCtrls[PTLE].x = stitchCoordsInPixels.x;
  frmCtrls[PTL].y = frmCtrls[PTM].y = frmCtrls[PTR].y = frmCtrls[PTLE].y = stitchCoordsInPixels.y;
  frmCtrls[PBR].y = frmCtrls[PBM].y = frmCtrls[PBL].y = stitchCoordsInPixels.y + adjustedSelectSize.cy;
  frmCtrls[PTR].x = frmCtrls[PRM].x = frmCtrls[PBR].x = stitchCoordsInPixels.x + adjustedSelectSize.cx;
  frmCtrls[PTM].x = frmCtrls[PBM].x = stitchCoordsInPixels.x + adjustedSelectSize.cx / 2;
  frmCtrls[PRM].y = frmCtrls[PLM].y = stitchCoordsInPixels.y + adjustedSelectSize.cy / 2;
  StateMap->set(StateFlag::SELSHO);
  thi::dusel(StitchWindowDC);
}

void thred::duSelbox() noexcept {
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);

  SelectBoxSize = {std::lround(StitchRangeRect.right - StitchRangeRect.left),
                   std::lround(StitchRangeRect.top - StitchRangeRect.bottom)};

  SelectBoxOffset = {std::lround(stitchPoint.x - StitchRangeRect.left),
                     std::lround(stitchPoint.y - StitchRangeRect.bottom)};
}

void thi::setbak(int32_t penWidth) noexcept {
  if (BackgroundPenWidth != penWidth) {
	BackgroundPenWidth = penWidth;
	thred::nuPen(BackgroundPen, penWidth, BackgroundColor);
  }
}

void thi::stchbox(uint32_t iStitch, HDC hDC) {
  auto       line   = std::array<POINT, SQPNTS> {};
  auto const layer  = (StitchBuffer->operator[](iStitch).attribute & LAYMSK) >> LAYSHFT;
  auto const offset = MulDiv(IniFile.stitchSizePixels, *ScreenDPI, STDDPI);
  if ((ActiveLayer != 0U) && (layer != 0U) && layer != ActiveLayer) {
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

void thred::drawCapturedStitchBox() {
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  SelectObject(StitchWindowMemDC, LinePen);
  thi::stchbox(ClosestPointIndex - 1, StitchWindowDC);
  thi::stchbox(ClosestPointIndex, StitchWindowDC);
  if (ClosestPointIndex < wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	thi::stchbox(ClosestPointIndex + 1, StitchWindowDC);
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::sCor2px(F_POINT const& stitchCoordinate, POINT& pixelCoordinate) {
  pixelCoordinate = {wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                     wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) +
                                         (ZoomRect.bottom - stitchCoordinate.y) * ZoomRatio.y)};
}

auto thi::sdCor2px(F_POINT_ATTR const& stitchPoint) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) +
                                    (ZoomRect.bottom - stitchPoint.y) * ZoomRatio.y)};
}

void thi::durot() noexcept(std::is_same_v<size_t, uint32_t>) {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::polyline(StitchWindowDC, RotateBoxOutline.data(), wrap::toUnsigned(RotateBoxOutline.size()));
  wrap::polyline(
      StitchWindowDC, RotateBoxCrossVertLine.data(), wrap::toUnsigned(RotateBoxCrossVertLine.size()));
  wrap::polyline(
      StitchWindowDC, RotateBoxCrossHorzLine.data(), wrap::toUnsigned(RotateBoxCrossHorzLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unrot() {
  if (StateMap->testAndReset(StateFlag::ROTSHO)) {
	thi::durot();
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
  if (StateMap->testAndReset(StateFlag::ROTUSHO)) {
	thred::durotu();
  }
}

auto thred::rotangf(F_POINT const& unrotatedPoint, float const rotationAngle, F_POINT const& rotationCenter) noexcept
    -> F_POINT {
  auto const deltaX           = unrotatedPoint.x - rotationCenter.x;
  auto const deltaY           = unrotatedPoint.y - rotationCenter.y;
  auto const distanceToCenter = (deltaX != 0.0F) ? std::hypot(deltaX, deltaY) : std::abs(deltaY);
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
  return F_POINT {rotationCenter.x + distanceToCenter * cos(newAngle),
                  rotationCenter.y + distanceToCenter * sin(newAngle)};
}

void thi::rotang(F_POINT unrotatedPoint, POINT& rotatedPoint, float rotationAngle, F_POINT const& rotationCenter) {
  auto const point = thred::rotangf(unrotatedPoint, rotationAngle, rotationCenter);
  thred::sCor2px(point, rotatedPoint);
}

auto thred::rotang1(F_POINT_ATTR const& unrotatedPoint, float const rotationAngle, F_POINT const& rotationCenter) noexcept
    -> F_POINT {
  auto const point = F_POINT {unrotatedPoint.x, unrotatedPoint.y};
  return thred::rotangf(point, rotationAngle, rotationCenter);
}

void thred::rotflt(F_POINT& point, float const rotationAngle, F_POINT const& rotationCenter) noexcept {
  point = thred::rotangf(point, rotationAngle, rotationCenter);
}

void thi::rotstch(F_POINT_ATTR& stitch, float const rotationAngle, F_POINT const& rotationCenter) noexcept {
  auto const point = thred::rotang1(stitch, rotationAngle, rotationCenter);

  stitch = point;
}

void thred::ritrot(float rotationAngle, F_POINT const& rotationCenter) {
  if (StateMap->test(StateFlag::MOVCNTR)) { // set rotation angle to 0 if we are moving the center point
	rotationAngle = 0.0F;
  }
  auto rotated           = POINT {};
  auto rotationReference = F_POINT {RotationRect.left, RotationRect.top};
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[0] = RotateBoxOutline[4] = rotated;
  rotationReference.x                       = rotationCenter.x;
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossVertLine[0] = rotated;
  rotationReference.x       = RotationRect.right;
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[1] = rotated;
  rotationReference.y = rotationCenter.y;
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossHorzLine[1] = rotated;
  rotationReference.y       = RotationRect.bottom;
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[2] = rotated;
  rotationReference.x = rotationCenter.x;
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossVertLine[1] = rotated;
  rotationReference.x       = RotationRect.left;
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxOutline[3] = rotated;
  rotationReference.y = rotationCenter.y;
  thi::rotang(rotationReference, rotated, rotationAngle, rotationCenter);
  RotateBoxCrossHorzLine[0] = rotated;
  thred::sCor2px(rotationCenter, RotateBoxToCursorLine[0]);
  StateMap->set(StateFlag::ROTSHO);
  thi::durot();
}

void thred::rot(F_POINT& rotationCenter) {
  if (!StateMap->test(StateFlag::FORMSEL) && SelectedFormList->empty() && !StateMap->test(StateFlag::BIGBOX) &&
      !StateMap->test(StateFlag::GRPSEL) && !StateMap->test(StateFlag::FPSEL)) {
	// no rotatable selections found
	displayText::shoseln(IDS_FGRPF, IDS_ROT);
	return;
  }
  StateMap->set(StateFlag::ROTAT);
  rotationCenter = form::rotpar();
  thred::ritrot(0, rotationCenter);
}

void thi::delfstchs() {
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  // find the first stitch to delete
  auto const firstStitch = std::ranges::find_if(*StitchBuffer, [codedForm](F_POINT_ATTR const& stitch) -> bool {
	return ((stitch.attribute & (FRMSK | NOTFRM)) == codedForm);
  });
  if (firstStitch != StitchBuffer->end()) {
	// we found the first stitch, so now delete the stitches in the form
	StitchBuffer->erase(std::remove_if(firstStitch,
	                                   StitchBuffer->end(),
	                                   [codedForm](F_POINT_ATTR const& stitch) -> bool {
	                                     return (stitch.attribute & FRMSK) == codedForm;
	                                   }),
	                    StitchBuffer->end());
  }
}

void thi::f1del(uint32_t formIndex) {
  if (StateMap->test(StateFlag::DELTO)) {
	auto const codedForm = formIndex << FRMSHFT;
	std::erase_if(*StitchBuffer, [codedForm](F_POINT_ATTR const& stitch) -> bool {
	  return (((stitch.attribute & NOTFRM) == 0U) && ((stitch.attribute & FRMSK) == codedForm));
	});
  }
  clip::deleclp(formIndex);
  clip::delmclp(formIndex);
  satin::delsac(formIndex);
  form::delflt(formIndex);
  texture::deltx(formIndex);
}

void thred::frmdel() {
  thi::f1del(ClosestFormToCursor);
  auto const firstForm = FormList->cbegin();
  FormList->erase(wrap::next(firstForm, ClosestFormToCursor));
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  if (StateMap->testAndReset(StateFlag::DELTO)) {
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & NOTFRM) != 0U) {
		continue;
	  }
	  auto const stitchForm = (stitch.attribute & FRMSK);
	  if (stitchForm > codedForm) {
		stitch.attribute &= NFRMSK;
		stitch.attribute |= (stitchForm - (1U << FRMSHFT));
	  }
	}
  }
  else {
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & NOTFRM) != 0U) {
		continue;
	  }
	  auto const stitchForm = (stitch.attribute & FRMSK);
	  if (stitchForm == codedForm) {
		stitch.attribute &= (NFRMSK & NTYPMSK);
		stitch.attribute |= NOTFRM;
		continue;
	  }
	  if (stitchForm > codedForm) {
		stitch.attribute &= NFRMSK;
		stitch.attribute |= (stitchForm - (1U << FRMSHFT));
	  }
	}
  }
  StateMap->reset(StateFlag::FORMSEL);
  if (ClosestFormToCursor > (FormList->size() - 1U)) {
	ClosestFormToCursor = 0U;
  }
}

void thred::delsfrms() {
  if (FormList->empty()) {
	return;
  }
  auto formIndices = std::vector<uint32_t> {};
  formIndices.resize(FormList->size());
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto const selectedForm : (*SelectedFormList)) {
	formMap.set(selectedForm);
	thi::f1del(selectedForm);
  }
  auto       deletedFormCount = 0U;
  auto       firstForm        = FormList->cbegin();
  auto const formCount        = FormList->size();
  for (auto iForm = 0U; iForm < formCount; ++iForm) {
	if (formMap.test(iForm)) {
	  FormList->erase(wrap::next(firstForm, (iForm - deletedFormCount)));
	  firstForm = FormList->cbegin(); // the erase invalidates firstForm
	  ++deletedFormCount;
	  continue;
	}
	formIndices[iForm] = (iForm - deletedFormCount) << FRMSHFT;
  }
  if (StateMap->test(StateFlag::DELTO)) {
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) == 0U) {
		continue;
	  }
	  auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	  if (!formMap.test(iForm)) {
		stitch.attribute &= NFRMSK;
		stitch.attribute |= formIndices[iForm];
	  }
	}
  }
  else {
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & NOTFRM) != 0U) {
		continue;
	  }
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
  SelectedFormList->clear();
  StateMap->reset(StateFlag::FORMSEL);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::cut() {
  thred::savdo();
  tfc::duclip();
  if (!SelectedFormList->empty()) {
	StateMap->set(StateFlag::DELTO);
	thred::delsfrms();
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thi::delfstchs();
	  thred::frmdel();
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::delstchm();
	  }
	}
  }
  thred::coltab();
  thred::rstAll();
  StateMap->set(StateFlag::RESTCH);
}

// https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
void thi::doStretch(uint32_t start, uint32_t end) {
  auto       firstStitch  = wrap::next(StitchBuffer->begin(), start);
  auto       centerStitch = std::next(firstStitch);
  auto       lastStitch   = std::next(centerStitch);
  auto const endStitch    = wrap::next(StitchBuffer->begin(), end);
  thred::savdo();
  while (lastStitch != endStitch) {
	if (((firstStitch->attribute & COLMSK) == (centerStitch->attribute & COLMSK)) &&
	    ((centerStitch->attribute & COLMSK) == (lastStitch->attribute & COLMSK))) {
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
  StateMap->set(StateFlag::RESTCH);
}

void thi::formStretch(uint32_t form) {
  auto stitch  = StitchBuffer->begin();
  auto iStitch = 0U;
  for (; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	if ((stitch->attribute & FRMSK) >> FRMSHFT == form && ((stitch->attribute & TYPFRM) != 0U)) {
	  break;
	}
	++stitch;
  }
  auto const firstStitch = iStitch;
  while ((stitch != StitchBuffer->end()) && ((stitch->attribute & FRMSK) >> FRMSHFT == form)) {
	++iStitch;
	++stitch;
  }
  doStretch(firstStitch, iStitch);
}

void thred::stretch() {
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  thi::formStretch(selectedForm);
	}
	return;
  }
  if (StateMap->test(StateFlag::FORMSEL)) {
	thi::formStretch(ClosestFormToCursor);
	return;
  }
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	thi::doStretch(GroupStartStitch, GroupEndStitch);
	return;
  }
  thi::doStretch(0, wrap::toUnsigned(StitchBuffer->size()));
}

void thred::contract() {
  thred::savdo();
}

void thred::unpat() {
  if (!StateMap->testAndReset(StateFlag::WASPAT)) {
	return;
  }
  ShowWindow(SpeedScrollBar, SW_HIDE);
  DestroyWindow(SpeedScrollBar);
  thred::movStch();
  StateMap->set(StateFlag::RESTCH);
}

auto thi::cmpstch(uint32_t iStitchA, uint32_t iStitchB) noexcept -> bool {
  return util::closeEnough(StitchBuffer->operator[](iStitchA).x, StitchBuffer->operator[](iStitchB).x)
             ? util::closeEnough(StitchBuffer->operator[](iStitchA).y,
                                 StitchBuffer->operator[](iStitchB).y)
             : false;
}

void thi::ofstch(std::vector<F_POINT_ATTR>& buffer, uint32_t iSource, char offset, F_POINT const& knotStep, uint32_t knotAttribute) {
  buffer.emplace_back(StitchBuffer->operator[](iSource).x + knotStep.x * wrap::toFloat(offset),
                      StitchBuffer->operator[](iSource).y + knotStep.y * wrap::toFloat(offset),
                      knotAttribute);
}

void thi::endknt(std::vector<F_POINT_ATTR>& buffer, uint32_t finish) {
  auto const finishIt      = wrap::next(StitchBuffer->begin(), finish);
  auto       startIt       = (finish != 0) ? std::next(finishIt, -1) : finishIt;
  auto const knotAttribute = startIt->attribute | KNOTMSK;

  auto deltaX = finishIt->x - startIt->x;
  auto deltaY = finishIt->y - startIt->y;
  auto length = deltaX * deltaX + deltaY * deltaY;
  while (length == 0.0F && startIt != StitchBuffer->begin()) {
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
  auto const& knots = (StateMap->test(StateFlag::FILDIR)) ? KNOT_AT_LAST_ORDER : KNOT_AT_END_ORDER;
  for (auto const knot : knots) {
	ofstch(buffer, finish, knot, knotStep, knotAttribute);
  }
  if (StateMap->test(StateFlag::FILDIR)) {
	ofstch(buffer, finish, 0, knotStep, knotAttribute);
  }
}

void thi::strtknt(std::vector<F_POINT_ATTR>& buffer, uint32_t start) {
  constexpr auto KNL      = 2.0F; // knot length
  auto const     startIt  = wrap::next(StitchBuffer->begin(), start);
  auto           finishIt = std::next(startIt);
  auto           deltaX   = finishIt->x - startIt->x;
  auto           deltaY   = finishIt->y - startIt->y;
  auto           length   = deltaX * deltaX + deltaY * deltaY;
  ++finishIt;
  while (length < KNL && finishIt != StitchBuffer->end()) {
	deltaX = finishIt->x - startIt->x;
	deltaY = finishIt->y - startIt->y;
	length = deltaX * deltaX + deltaY * deltaY;
	++finishIt;
  }
  if (finishIt == StitchBuffer->end()) {
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

void thi::delknt() {
  // delete the knot stitches
  std::erase_if(*StitchBuffer,
                [](F_POINT_ATTR const& stitch) -> bool { return (stitch.attribute & KNOTMSK) != 0U; });
}

void thred::delknot() {
  // Find the first knot stitch, if any
  auto const firstStitch = std::ranges::find_if(*StitchBuffer, [](F_POINT_ATTR const& stitch) -> bool {
	return ((stitch.attribute & KNOTMSK) != 0U);
  });
  if (firstStitch == StitchBuffer->end()) {
	return;
  }
  thred::savdo();
  // delete the knot stitches
  StitchBuffer->erase(std::remove_if(firstStitch,
                                     StitchBuffer->end(),
                                     [](F_POINT_ATTR const& stitch) -> bool {
	                                   return (stitch.attribute & KNOTMSK) != 0U;
                                     }),
                      StitchBuffer->end());
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

constexpr auto KNOTLEN = int32_t {54}; // set knots for stitches longer than this

auto thi::kjmp(std::vector<F_POINT_ATTR>& buffer, uint32_t start) -> uint32_t {
  while (start < wrap::toUnsigned(StitchBuffer->size() - 1U) && stlen(start) > KNOTLEN) {
	buffer.push_back(StitchBuffer->operator[](start++));
  }
  strtknt(buffer, start);
  return start;
}

void thi::setknt() {
  auto buffer = std::vector<F_POINT_ATTR> {};
  buffer.reserve(StitchBuffer->size());
  buffer.push_back(StitchBuffer->front());
  strtknt(buffer, 0);
  auto iStitch = (stlen(0) > KNOTLEN) ? kjmp(buffer, 1) : 1;
  StateMap->reset(StateFlag::FILDIR);
  while (iStitch < wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	buffer.push_back(StitchBuffer->operator[](iStitch));
	if (stlen(iStitch) > KNOTLEN) {
	  endknt(buffer, iStitch);
	  iStitch = kjmp(buffer, iStitch + 1U);
	  buffer.push_back(StitchBuffer->operator[](iStitch));
	}
	++iStitch;
  }
  StateMap->set(StateFlag::FILDIR);
  endknt(buffer, iStitch);
  buffer.back().attribute &= (~KNOTMSK);
  *StitchBuffer = std::move(buffer);
}

auto thi::srchknot(uint32_t source) -> uint32_t {
  auto upper = std::ranges::find(*Knots, source);
  if (upper == Knots->end()) {
	return 3;
  }
  --upper;
  if (((*upper > source) ? (*upper - source) : (source - *upper)) < KNOTSCNT) {
	++upper;
	if (((*upper > source) ? (*upper - source) : (source - *upper)) < KNOTSCNT) {
	  return 0;
	}
	return 2;
  }
  ++upper;
  if (((*upper > source) ? (*upper - source) : (source - *upper)) < KNOTSCNT) {
	return 1;
  }
  return 3;
}

void thi::chkncol() {
  auto initialColor = StitchBuffer->front().attribute & COLMSK;
  auto buffer       = std::vector<F_POINT_ATTR> {};
  buffer.reserve(StitchBuffer->size());
  StateMap->reset(StateFlag::FILDIR);
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
	++iStitch;
  }
  *StitchBuffer = std::move(buffer);
}

void thred::setknots() {
  if (StitchBuffer->empty()) {
	return;
  }
  thred::savdo();
  thi::delknt();
  thi::setknt();
  thi::fndknt();
  thi::chkncol();
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
}

void thred::hidbit() {
  menu::flipHideBitmap();
  StateMap->set(StateFlag::RESTCH);
}

void thi::drwlstch(uint32_t finish) {
  auto           movieLine = std::vector<POINT> {}; // line for movie stitch draw
  constexpr auto LNMAXPTS  = 100U;                  // maximum number of points in a line
  movieLine.reserve(LNMAXPTS);
  if (finish == 0) {
	patdun();
	return;
  }
  if (StateMap->test(StateFlag::HID)) {
	while (RunPoint < (finish - 1) && (StitchBuffer->operator[](RunPoint).attribute & COLMSK) != ActiveColor) {
	  ++RunPoint;
	}
  }
  if (StateMap->test(StateFlag::ZUMED)) {
	constexpr auto STITCH_COORDS_IN_PIXELS = POINT {};
	movieLine.push_back(STITCH_COORDS_IN_PIXELS);
	while (RunPoint < StitchesPerFrame + 1 && RunPoint < finish - 2 && !stch2px2(RunPoint)) {
	  ++RunPoint;
	}
  }
  auto const color = StitchBuffer->operator[](RunPoint).attribute & COLMSK;
  if (StateMap->test(StateFlag::ZUMED)) {
	auto       stitchCoordsInPixels = POINT {};
	auto const origin               = RunPoint - 1U;
	auto       flag                 = true;
	while (wrap::toUnsigned(movieLine.size()) < (StitchesPerFrame + 1U) && RunPoint < (finish - 2U) &&
	       (StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color) {
	  if (thred::stch2px(RunPoint, stitchCoordsInPixels)) {
		movieLine.push_back(stitchCoordsInPixels);
		if (flag) {
		  flag = false;
		  if ((RunPoint != 0U) && thred::stch2px(RunPoint - 1, stitchCoordsInPixels)) {
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
	  if ((StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color) {
		movieLine.push_back(stitchCoordsInPixels);
		++RunPoint;
	  }
	}
	SelectObject(StitchWindowDC, UserPen->operator[](color));
	wrap::polyline(StitchWindowDC, movieLine.data(), wrap::toUnsigned(movieLine.size()));
	if (!flag) {
	  --RunPoint;
	}
  }
  else {
	auto iMovieFrame = 0U;

	SelectObject(StitchWindowDC, UserPen->operator[](color));
	while (iMovieFrame < StitchesPerFrame && (RunPoint + 1 < finish - 1) &&
	       ((StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color)) {
	  movieLine.push_back(thred::stch2px1(RunPoint++));
	  ++iMovieFrame;
	}
	--RunPoint;
	wrap::polyline(StitchWindowDC, movieLine.data(), wrap::toUnsigned(movieLine.size()));
  }
  if ((StitchBuffer->operator[](wrap::toSize(RunPoint) + 1U).attribute & COLMSK) != color) {
	++RunPoint;
  }
  displayText::ritnum(IDS_NUMSEL, RunPoint);
  if (RunPoint + 3U > finish - 1U) {
	patdun();
  }
}

void thi::stchout() {
  if (StateMap->test(StateFlag::GRPSEL)) {
	drwlstch(GroupEndStitch);
	return;
  }
  drwlstch(wrap::toUnsigned(StitchBuffer->size() - 1U));
}

void thi::setsped() {
  constexpr auto DEFET = uint32_t {100U}; // default elapsed time per frame. Units are millseconds
  constexpr auto DEFUT = 10.0;            // default user time per frame. Units are millseconds

  auto elapsedTimePerFrame = DEFET;
  if (MovieTimeStep == 0) {
	MovieTimeStep = 1;
  }
  auto const userTimePerFrame = wrap::toDouble(MovieTimeStep) / DEFUT;
  if (userTimePerFrame < DEFUT) {
	StitchesPerFrame        = wrap::round<uint32_t>(elapsedTimePerFrame / userTimePerFrame);
	constexpr auto SPFCLAMP = 99U; // maximum stitches per frame
	if (StitchesPerFrame > SPFCLAMP) {
	  StitchesPerFrame = SPFCLAMP;
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

void thred::deltot() {
  DesignerName->assign(utf::utf8ToUtf16(std::string(IniFile.designerName.data())));
  TexturePointsBuffer->clear();
  FormList->clear();
  StitchBuffer->clear();
  FormVertices->clear();
  SatinGuides->clear();
  StateMap->reset(StateFlag::GMRK);
  thred::rstAll();
  thred::coltab();
  thred::zumhom();
  auto const wTxt = displayText::format2(IDS_THRDBY, ThrName->wstring(), *DesignerName);
  SetWindowText(ThrEdWindow, wTxt.c_str());
}

auto thi::wastch(uint32_t const& formIndex) -> bool {
  return std::ranges::any_of(*StitchBuffer, [&formIndex](F_POINT_ATTR const& stitch) -> bool {
	return (stitch.attribute & FRMSK) >> FRMSHFT == formIndex;
  });
}

auto thi::frmstch() -> bool {
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto const selectedForm : (*SelectedFormList)) {
	formMap.set(selectedForm);
  }
  return std::ranges::any_of(*StitchBuffer, [&formMap](F_POINT_ATTR const& stitch) -> bool {
	return formMap.test((stitch.attribute & FRMSK) >> FRMSHFT);
  });
}

void thred::delet() {
  thred::undat();
  if (StateMap->testAndReset(StateFlag::FPSEL)) {
	thred::savdo();
	auto& form = FormList->operator[](ClosestFormToCursor);
	// dynamic bitset allows non-contiguous ranges of points to be deleted in later versions
	auto vertexMap         = boost::dynamic_bitset<>(form.vertexCount);
	auto currentFormVertex = SelectedFormVertices.start;
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  vertexMap.set(currentFormVertex);
	  currentFormVertex = form::pdir(form, currentFormVertex);
	}
	auto const vBegin   = wrap::next(FormVertices->begin(), form.vertexIndex);
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
	auto const eraseStart = wrap::next(FormVertices->cbegin(), wrap::toSize(form.vertexIndex) + eraseCount);
	auto const eraseEnd = wrap::next(eraseStart, (form.vertexCount - eraseCount));
	FormVertices->erase(eraseStart, eraseEnd); // This invalidates iterators
	auto const nextForm = wrap::toSize(ClosestFormToCursor) + 1U;
	for (auto iForm = wrap::next(FormList->begin(), nextForm); iForm < FormList->end(); ++iForm) {
	  iForm->vertexIndex -= SelectedFormVertices.vertexCount + 1U;
	}
	form.vertexCount -= (SelectedFormVertices.vertexCount + 1U);
	form.outline();
	if (form.type == SAT) {
	  // Make sure the end guides are still valid
	  if (vertexMap.test(0) || vertexMap.test(1)) {
		form.wordParam = 0; // Satin end guide
		form.attribute &= NFRMEND;
	  }
	  auto const iNext = form.wordParam + 1U;
	  if (vertexMap.test(form.wordParam) || vertexMap.test(iNext)) {
		form.wordParam = 0;
	  }

	  if (form.satinGuideCount == 0U) {
		// ToDo - Is there a better way to do this than iterating through?
		auto itGuide = wrap::next(SatinGuides->begin(), form.satinGuideIndex);
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
	thi::fndknt();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_SHIFT)) {
	thred::deltot();
	thi::fndknt();
	return;
  }
  thred::savdo();
  auto satinFlag = false;
  if (!SelectedFormList->empty()) {
	if (thi::frmstch()) {
	  StateMap->set(StateFlag::DELSFRMS);
	  displayText::tabmsg(IDS_DELFRM, false);
	  displayText::okcan();
	  displayText::tomsg();
	}
	else {
	  thred::delsfrms();
	}
	return;
  }
  if (StateMap->test(StateFlag::FORMSEL) && !FormList->empty()) {
	if (thi::wastch(ClosestFormToCursor)) {
	  StateMap->set(StateFlag::DELFRM);
	  displayText::tabmsg(IDS_FDEL, false);
	  displayText::okcan();
	  displayText::tomsg();
	}
	else {
	  StateMap->reset(StateFlag::DELTO);
	  thred::frmdel();
	  StateMap->set(StateFlag::RESTCH);
	}
	thi::fndknt();
	return;
  }
  if (StateMap->test(StateFlag::SELBOX)) {
	if (StitchBuffer->size() > 2U) {
	  thi::delstch1(ClosestPointIndex);
	  if (!thi::stch2px2(ClosestPointIndex)) {
		thred::movbox();
	  }
	}
	else {
	  StitchBuffer->clear();
	  StateMap->reset(StateFlag::SELBOX);
	}
	thred::coltab();
	xt::setfchk();
	thi::fndknt();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::delstchm();
	thred::coltab();
	thred::rstAll();
	xt::setfchk();
	thi::fndknt();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::FRMPSEL) || form::closfrm(ClosestFormToCursor)) {
	auto& form = FormList->operator[](ClosestFormToCursor);
	switch (form.type) {
	  case FRMLINE: {
		if (form.fillType == CONTF) {
		  if (ClosestVertexToCursor == form.fillGuide.start ||
		      ClosestVertexToCursor == form.fillGuide.finish) {
			form::delmfil(ClosestFormToCursor);
			form.fillType = 0;
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
			return;
		  }
		  if (form.fillGuide.start > ClosestVertexToCursor) {
			--form.fillGuide.start;
		  }
		  if (form.fillGuide.finish > ClosestVertexToCursor) {
			--form.fillGuide.finish;
		  }
		}
		break;
	  }
	  case SAT: {
		while (true) {
		  if (ClosestVertexToCursor <= 1) {
			if ((form.attribute & FRMEND) != 0U) {
			  if (form.wordParam != 0U) {
				form.wordParam = 0;
			  }
			  else {
				form.attribute &= NFRMEND;
			  }
			  satinFlag = true;
			  break;
			}
		  }
		  auto const& endGuide = form.wordParam;
		  if (endGuide != 0U) {
			if (ClosestVertexToCursor == endGuide || ClosestVertexToCursor == endGuide + 1U) {
			  form.wordParam = 0;
			  satinFlag      = true;
			  break;
			}
		  }
		  if (form.satinGuideCount != 0U) {
			auto itGuide = wrap::next(SatinGuides->cbegin(), form.satinGuideIndex);
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
			  if (itGuide->start == ClosestVertexToCursor || itGuide->finish == ClosestVertexToCursor) {
				satin::delcon(form, iGuide);
				satinFlag = true;
				break;
			  }
			  ++itGuide;
			}
		  }
		  break;
		}
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
	if (ClosestFormToCursor > FormList->size() - 1U) {
	  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
	}
	form.outline();
	form::refil(ClosestFormToCursor);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  if (!satinFlag && thred::closPnt1(ClosestPointIndex)) {
	if (StitchBuffer->size() > 2U) {
	  thi::delstch1(ClosestPointIndex);
	  if (!thi::stch2px2(ClosestPointIndex)) {
		thred::movbox();
	  }
	}
	else {
	  StitchBuffer->clear();
	  StateMap->reset(StateFlag::SELBOX);
	}
	thred::coltab();
	xt::setfchk();
	thi::fndknt();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  thi::fndknt();
}

void thred::movi() {
  constexpr auto MOVITIM = int32_t {12}; // default movie time
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rstAll();
	StateMap->set(StateFlag::GRPSEL);
  }
  else {
	thred::rstAll();
  }
  if (StitchBuffer->empty()) {
	return;
  }
  if (MsgWindow != nullptr) {
	DestroyWindow(MsgWindow);
	MsgWindow = nullptr;
  }
  StateMap->set(StateFlag::RUNPAT);
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	RunPoint = GroupStartStitch;
  }
  else {
	RunPoint = 0;
  }
  thred::movStch();
  if (!StateMap->test(StateFlag::WASPAT)) {
	SpeedScrollBar = CreateWindow(L"SCROLLBAR",
	                              nullptr,
	                              SBS_HORZ | WS_CHILD | WS_VISIBLE,
	                              ButtonWidthX3,
	                              0,
	                              StitchWindowSize.cx,
	                              *ScrollSize,
	                              ThrEdWindow,
	                              nullptr,
	                              ThrEdInstance,
	                              nullptr);
  }
  auto const stepCount = (StateMap->test(StateFlag::ZUMED))
                             ? gsl::narrow_cast<float>(StitchBuffer->size()) * ZoomFactor * ZoomFactor
                             : gsl::narrow_cast<float>(StitchBuffer->size());
  if (!StateMap->test(StateFlag::WASPAT)) {
	constexpr auto MLEN = 10000.0F; // default movie length (millisecs)
	MovieTimeStep       = wrap::round<decltype(MovieTimeStep)>(MLEN * MOVITIM / stepCount);
  }
  if (MovieTimeStep < MINDELAY) {
	MovieTimeStep = MINDELAY;
  }
  if (MovieTimeStep > MAXDELAY) {
	MovieTimeStep = MAXDELAY;
  }
  auto scrollInfo   = SCROLLINFO {}; // scroll bar i/o structure
  scrollInfo.cbSize = sizeof(scrollInfo);
  scrollInfo.fMask  = SIF_ALL;
  scrollInfo.nMax   = MAXDELAY;
  scrollInfo.nMin   = MINDELAY;
  scrollInfo.nPage  = 1;
  scrollInfo.nPos   = MAXDELAY - MovieTimeStep;
  SetScrollInfo(SpeedScrollBar, SB_CTL, &scrollInfo, TRUE);
  FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
  thi::setsped();
}

void thred::redclp() {
  auto const codedLayer  = gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT);
  auto*      clipPointer = GlobalLock(ClipMemory);
  if (clipPointer == nullptr) {
	return;
  }
  auto const* const clipStitchPtr    = gsl::narrow_cast<CLIP_STITCH const*>(clipPointer);
  auto const        clipSize         = clipStitchPtr->led;
  auto const        spClipStitchData = gsl::span<const CLIP_STITCH> {clipStitchPtr, clipSize};
  ClipBuffer->clear();
  ClipBuffer->reserve(clipSize);
  ClipBuffer->emplace_back(wrap::toFloat(clipStitchPtr->x) + wrap::toFloat(clipStitchPtr->fx) / FRACFACT,
                           wrap::toFloat(clipStitchPtr->y) + wrap::toFloat(clipStitchPtr->fy) / FRACFACT,
                           0U);

#if CLPBUG
  OutputDebugString(fmt::format(FMT_COMPILE(L"redclp:interator [0] x [{:6.2F}] y [{:6.2F}]\n"),
                                ClipBuffer->back().x,
                                ClipBuffer->back().y)
                        .c_str());
#endif
  auto clipRect = F_RECTANGLE {
      ClipBuffer->front().x, ClipBuffer->front().y, ClipBuffer->front().x, ClipBuffer->front().y};
  auto iCSD = std::next(spClipStitchData.begin());
  for (auto iStitch = 1U; iStitch < clipSize; ++iStitch) {
	ClipBuffer->emplace_back(wrap::toFloat(iCSD->x) + wrap::toFloat(iCSD->fx) / FRACFACT,
	                         wrap::toFloat(iCSD->y) + wrap::toFloat(iCSD->fy) / FRACFACT,
	                         (iCSD->led & COLMSK) | codedLayer);

#if CLPBUG
	OutputDebugString(fmt::format(FMT_COMPILE(L"redclp:interator [{}] x [{:6.2F}] y [{:6.2F}]\n"),
	                              iStitch,
	                              ClipBuffer->back().x,
	                              ClipBuffer->back().y)
	                      .c_str());
#endif
	if (ClipBuffer->back().x < clipRect.left) {
	  clipRect.left = ClipBuffer->back().x;
	}
	if (ClipBuffer->back().x > clipRect.right) {
	  clipRect.right = ClipBuffer->back().x;
	}
	if (ClipBuffer->back().y < clipRect.bottom) {
	  clipRect.bottom = ClipBuffer->back().y;
	}
	if (ClipBuffer->back().y > clipRect.top) {
	  clipRect.top = ClipBuffer->back().y;
	}
	++iCSD;
  }
  ClipBuffer->front().attribute = ActiveColor | codedLayer;
  ClipRectSize                  = {clipRect.right - clipRect.left, clipRect.top - clipRect.bottom};
  GlobalUnlock(ClipMemory);
  if ((clipRect.left != 0.0F) || (clipRect.bottom != 0.0F)) {
	for (auto& clip : *ClipBuffer) {
	  clip.x -= clipRect.left;
	  clip.y -= clipRect.bottom;
	}
  }
}

void thi::drwmrk(HDC hDC) {
  auto       markCoordinates = POINT {};
  auto       markLine        = std::array<POINT, 2> {};
  auto const markOffset      = MulDiv(6, *ScreenDPI, STDDPI);
  thred::sCor2px(F_POINT {ZoomMarkPoint}, markCoordinates);
  SelectObject(hDC, ZoomMarkPen);
  SetROP2(hDC, R2_XORPEN);
  markLine[0] = {markCoordinates.x - markOffset, markCoordinates.y - markOffset};
  markLine[1] = {markCoordinates.x + markOffset, markCoordinates.y + markOffset};
  wrap::polyline(hDC, markLine.data(), wrap::toUnsigned(markLine.size()));
  markLine[0] = {markCoordinates.x - markOffset, markCoordinates.y + markOffset};
  markLine[1] = {markCoordinates.x + markOffset, markCoordinates.y - markOffset};
  wrap::polyline(hDC, markLine.data(), wrap::toUnsigned(markLine.size()));
  SetROP2(hDC, R2_COPYPEN);
}

void thred::vubak() {
  if (WorkingFileName->empty() && !StateMap->test(StateFlag::THUMSHO)) {
	return;
  }
  StateMap->set(StateFlag::ZUMED);
  thred::movStch();
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
	                         deltaX * gsl::narrow_cast<int32_t>(iPosition & 1U) + ButtonWidthX3,
	                         verticalLocation,
	                         deltaX,
	                         deltaY,
	                         ThrEdWindow,
	                         nullptr,
	                         ThrEdInstance,
	                         nullptr);
	++itHWndBV;
  }
  StateMap->set(StateFlag::BAKSHO);
}

void thred::insflin(POINT insertPoint) {
  auto const offset    = POINT {InsertSize.cx / 2, InsertSize.cy / 2};
  auto&      formLines = *FormLines;
  formLines.resize(SQPNTS);
  formLines[0].x = formLines[3].x = formLines[4].x = insertPoint.x - offset.x;
  formLines[1].x = formLines[2].x = insertPoint.x + offset.x;
  formLines[0].y = formLines[1].y = formLines[4].y = insertPoint.y - offset.y;
  formLines[2].y = formLines[3].y = insertPoint.y + offset.y;
}

auto thi::isthr(fs::path const& thredPath) -> bool {
  auto const extention = thredPath.extension().wstring();
  return (extention.compare(0, 3, L".th") == 0);
}

auto thi::gethand(std::vector<F_POINT_ATTR> const& stitch, uint32_t stitchCount) noexcept -> uint32_t {
  auto userStitchCount = 0U;
  for (auto iStitch = 0U; iStitch < stitchCount; ++iStitch) {
	if ((stitch[iStitch].attribute & USMSK) != 0U) {
	  ++userStitchCount;
	}
  }
  return userStitchCount;
}

void thred::insfil(fs::path& insertedFile) {
  auto successFlag       = false;
  auto insertedRectangle = F_RECTANGLE {BIGFLOAT, LOWFLOAT, LOWFLOAT, BIGFLOAT};
  if (insertedFile.empty()) {
	thi::getNewFileName(insertedFile, FileStyles::INS_FILES, FileIndices::THR);
  }
  InsertedStitchIndex = wrap::toUnsigned(StitchBuffer->size());
  if (thi::isthr(insertedFile)) {
	if (thi::insTHR(insertedFile, insertedRectangle)) {
	  successFlag = true;
	}
  }
  if (PCS::isPCS(insertedFile)) {
	if (PCS::insPCS(insertedFile, insertedRectangle)) {
	  // We did not insert forms so ensure that duinsfil does not move forms
	  InsertedFormIndex   = wrap::toUnsigned(FormList->size());
	  InsertedVertexIndex = wrap::toUnsigned(FormVertices->size());
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
  form::ratsr();
  InsertSize.cx = std::lround(insertedSize.x * HorizontalRatio);
  // ToDo - Should this be vertical ratio?
  InsertSize.cy = std::lround(insertedSize.y * HorizontalRatio);
  auto const initialInsertPoint = POINT {StitchWindowClientRect.right / 2, StitchWindowClientRect.bottom / 2};
  thred::insflin(initialInsertPoint);
  NewFormVertexCount = 5U; // inserted file is a rectangle NOLINT(readability-magic-numbers)
  StateMap->set(StateFlag::SHOFRM);
  StateMap->set(StateFlag::INSFIL);
  form::dufrm();
}

auto thi::insTHR(fs::path const& insertedFile, F_RECTANGLE& insertedRectangle) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle =
      CreateFile(insertedFile.wstring().c_str(), (GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr);
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
  if (version != 0U) {
	gethand(*StitchBuffer, wrap::toUnsigned(StitchBuffer->size()));
	// ToDo - replace constants with sizes of data structures?
	homscor = wrap::toUnsigned(FormList->size()) * FRMW +
	          gethand(*StitchBuffer, wrap::toUnsigned(StitchBuffer->size())) * HANDW +
	          wrap::toUnsigned(FormVertices->size()) * FRMPW + wrap::toUnsigned(StitchBuffer->size()) * STCHW;
	if (!wrap::readFile(fileHandle, &thredHeader, sizeof(thredHeader), &bytesRead, L"ReadFile for thredHeader in insTHR")) {
	  return false;
	}
  }
  thred::savdo();
  auto fileStitchBuffer = std::vector<F_POINT_ATTR> {};
  if (fileHeader.stitchCount != 0U) {
	fileStitchBuffer.resize(fileHeader.stitchCount);
	auto const bytesToRead = fileHeader.stitchCount * wrap::sizeofType(fileStitchBuffer);
	if (!wrap::readFile(fileHandle, fileStitchBuffer.data(), bytesToRead, &bytesRead, L"ReadFile for fileStitchBuffer in insTHR")) {
	  return false;
	}
  }
  auto const formDataOffset = gsl::narrow<LONG>(bitmap::getBmpNameLength() + sizeof(BackgroundColor) +
                                                sizeof(UserColor) + sizeof(CustomColor) + TSSIZE);
  SetFilePointer(fileHandle, formDataOffset, nullptr, FILE_CURRENT);
  InsertedVertexIndex = wrap::toUnsigned(FormVertices->size());
  InsertedFormIndex   = wrap::toUnsigned(FormList->size());
  if (fileHeader.formCount != 0U) {
	auto const newFormVertexIndex = wrap::toUnsigned(FormVertices->size());
	auto       newSatinGuideIndex = wrap::toUnsigned(SatinGuides->size());
	auto       clipOffset         = wrap::toUnsigned(ClipPoints->size());
	auto       textureOffset      = wrap::toUnsigned(TexturePointsBuffer->size());
	if (version < 2) {
	  auto inFormList = std::vector<FRM_HEAD_O> {};
	  inFormList.resize(fileHeader.formCount);
	  auto const bytesToRead = fileHeader.formCount * wrap::sizeofType(inFormList);
	  if (!wrap::readFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, L"ReadFile for inFormList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inFormList.resize(bytesRead / wrap::sizeofType(inFormList));
		StateMap->set(StateFlag::BADFIL);
	  }
	  FormList->reserve(FormList->size() + inFormList.size());
	  FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
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
		StateMap->set(StateFlag::BADFIL);
	  }
	  FormList->reserve(FormList->size() + inFormList.size());
	  FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
	}
	auto vertexOffset = wrap::toUnsigned(FormVertices->size());
	if (fileHeader.vertexCount != 0U) {
	  auto inVerticeList = std::vector<F_POINT> {};
	  inVerticeList.resize(fileHeader.vertexCount);
	  auto const bytesToRead = fileHeader.vertexCount * wrap::sizeofType(inVerticeList);
	  if (!wrap::readFile(fileHandle, inVerticeList.data(), bytesToRead, &bytesRead, L"ReadFile for inVerticeList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inVerticeList.resize(bytesRead / wrap::sizeofType(inVerticeList));
		StateMap->set(StateFlag::BADFIL);
	  }
	  FormVertices->reserve(FormVertices->size() + inVerticeList.size());
	  FormVertices->insert(FormVertices->end(), inVerticeList.cbegin(), inVerticeList.cend());
	}
	else {
	  StateMap->set(StateFlag::BADFIL);
	}
	auto guideOffset = wrap::toUnsigned(SatinGuides->size());
	if (fileHeader.dlineCount != 0U) {
	  auto inGuideList = std::vector<SAT_CON_OUT> {};
	  inGuideList.resize(fileHeader.dlineCount);
	  auto const bytesToRead = fileHeader.dlineCount * wrap::sizeofType(inGuideList);
	  if (!wrap::readFile(fileHandle, inGuideList.data(), bytesToRead, &bytesRead, L"ReadFile for inGuideList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inGuideList.resize(bytesRead / wrap::sizeofType(inGuideList));
		StateMap->set(StateFlag::BADFIL);
	  }
	  SatinGuides->reserve(SatinGuides->size() + inGuideList.size());
	  SatinGuides->insert(SatinGuides->end(), inGuideList.begin(), inGuideList.end());
	  newSatinGuideIndex += wrap::toUnsigned(inGuideList.size());
	}
	if (fileHeader.clipDataCount != 0U) {
	  auto inPointList = std::vector<F_POINT> {};
	  inPointList.resize(fileHeader.clipDataCount);
	  auto const bytesToRead = fileHeader.clipDataCount * wrap::sizeofType(ClipPoints);
	  if (!wrap::readFile(fileHandle, inPointList.data(), bytesToRead, &bytesRead, L"ReadFile for inPointList in insTHR")) {
		return false;
	  }
	  if (bytesRead != bytesToRead) {
		inPointList.resize(bytesRead / wrap::sizeofType(inPointList));
		StateMap->set(StateFlag::BADFIL);
	  }
	  ClipPoints->reserve(ClipPoints->size() + inPointList.size());
	  ClipPoints->insert(ClipPoints->end(), inPointList.begin(), inPointList.end());
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
		StateMap->set(StateFlag::BADFIL);
	  }
	  TexturePointsBuffer->reserve(TexturePointsBuffer->size() + inTextureList.size());
	  TexturePointsBuffer->insert(TexturePointsBuffer->end(), inTextureList.begin(), inTextureList.end());
	}
	// update the form pointer variables
	for (auto iFormList = InsertedFormIndex; iFormList < wrap::toUnsigned(FormList->size()); ++iFormList) {
	  auto& formIter       = FormList->operator[](iFormList);
	  formIter.vertexIndex = vertexOffset;
	  vertexOffset += formIter.vertexCount;
	  if (formIter.type == SAT && (formIter.satinGuideCount != 0U)) {
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
	if (newFormVertexIndex != FormVertices->size()) {
	  StateMap->set(StateFlag::BADFIL);
	}
	if (newSatinGuideIndex != SatinGuides->size()) {
	  StateMap->set(StateFlag::BADFIL);
	}
	if (clipOffset != ClipPoints->size()) {
	  StateMap->set(StateFlag::BADFIL);
	}
	if (fileHeader.formCount != 0U) {
	  auto spVertices = gsl::span(*FormVertices).subspan(InsertedVertexIndex, fileHeader.vertexCount);

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
	auto const encodedFormIndex = (InsertedFormIndex << FRMSHFT);

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
  constexpr auto VERSION1 = (0x01U << TBYTSHFT); // ThrEd version 1 signature
  if ((fileHeader.headerType & VERSION1) != 0U) {
	// ToDo - Replace constants with sizes of data structures
	auto const filscor = (fileHeader.formCount) * FRMW +
	                     gethand(fileStitchBuffer, fileHeader.stitchCount) * HANDW +
	                     fileHeader.vertexLen * FRMPW + fileHeader.stitchCount * STCHW;
	if (filscor > homscor) {
	  auto const spEHCN = gsl::span {ExtendedHeader->creatorName};
	  std::ranges::copy(thredHeader.creatorName, spEHCN.begin());
	  redfnam(*DesignerName);
	  auto fmtStr = displayText::format2(IDS_THRDBY, ThrName->wstring(), *DesignerName);
	  SetWindowText(ThrEdWindow, fmtStr.c_str());
	}
  }
  auto dest = StitchBuffer->end();
  StitchBuffer->insert(dest, fileStitchBuffer.cbegin(), fileStitchBuffer.cend());
  CloseHandle(fileHandle);
  return true;
}

void thi::getbak() {
  if (!StateMap->test(StateFlag::THUMSHO)) {
	StateMap->set(StateFlag::REDOLD);
	nuFil(FileIndices::THR);
	return;
  }
  if (!StateMap->test(StateFlag::RBUT)) {
	return;
  }
  unthum();
  StateMap->set(StateFlag::FRMOF);
  *WorkingFileName = *DefaultDirectory / Thumbnails->operator[](ThumbnailsSelected.at(FileVersionIndex));
  thred::insfil(*WorkingFileName);
  if (!wrap::pressed(VK_SHIFT)) {
	return;
  }
  StateMap->reset(StateFlag::INSFIL);
  StateMap->reset(StateFlag::FRMOF);
  StateMap->set(StateFlag::INIT);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thi::rebak() {
  thi::destroyBV();
  auto newFileName    = *ThrName;    // intentional copy
  auto safetyFileName = newFileName; // initialise from local variable
  auto ext            = newFileName.extension().wstring();
  ext.back()          = FileVersionIndex + L's';
  newFileName.replace_extension(ext);
  ext.back() = 'x';
  safetyFileName.replace_extension(ext);
  fs::rename(*ThrName, safetyFileName);
  if (fs::exists(newFileName)) {
	fs::rename(newFileName, *ThrName);
  }
  fs::rename(safetyFileName, newFileName);
  *WorkingFileName = *ThrName;
  StateMap->set(StateFlag::REDOLD);
  nuFil(FileIndices::THR);
  fs::remove(safetyFileName);
}

void thred::thumbak() {
  thi::destroyBV();
  thi::getbak();
}

void thred::purg() {
  if (ThrName->empty()) {
	return;
  }
  auto fileName = *ThrName; // intentional copy
  auto ext      = ThrName->extension().wstring();
  for (auto iLast = wchar_t {}; iLast < OLDVER; ++iLast) {
	ext.back() = iLast + 's';
	fileName.replace_extension(ext);
	fs::remove(fileName);
  }
}

void thred::purgdir() {
  StateMap->set(StateFlag::PRGMSG);
  displayText::showMessage(IDS_DELBAK, DefaultDirectory->wstring());
  displayText::okcan();
}

void thred::deldir() {
  thred::unmsg();
  displayText::tabmsg(IDS_BAKDEL, false);
  auto backSpec = std::wstring {L".th0"};
  for (auto const& filePath : fs::directory_iterator(*DefaultDirectory)) {
	if (!fs::is_directory(filePath)) {
	  auto fileExt    = filePath.path().extension().wstring();
	  backSpec.back() = 's';
	  for (auto iLastChar = wchar_t {}; iLastChar < OLDVER; ++iLastChar) {
		if (fileExt == backSpec) {
		  fs::remove(filePath.path());
		  ++(backSpec.back());
		}
	  }
	}
  }
  thred::unmsg();
  displayText::tabmsg(IDS_BAKDT, false);
}

auto thred::chkwnd(HWND window) noexcept -> bool {
  auto windowRect = RECT {};
  GetWindowRect(window, &windowRect);
  return Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
         Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom;
}

void thred::mv2f() {
  if (StateMap->testAndReset(StateFlag::FORMSEL)) {
	thred::savdo();
	std::vector<F_POINT_ATTR> tempStitchBuffer {};
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
	std::ranges::copy(tempStitchBuffer, wrap::next(StitchBuffer->begin(), iLowBuffer));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	thred::savdo();
	std::vector<F_POINT_ATTR> tempStitchBuffer {};
	auto const                grpSize = GroupEndStitch + 1U - GroupStartStitch;
	tempStitchBuffer.resize(StitchBuffer->size() - grpSize);
	thred::rngadj();
	std::copy(StitchBuffer->begin(),
	          wrap::next(StitchBuffer->begin(), GroupStartStitch),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U),
	          StitchBuffer->end(),
	          wrap::next(tempStitchBuffer.begin(), GroupStartStitch));
	std::copy(wrap::next(StitchBuffer->begin(), GroupStartStitch),
	          wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U),
	          StitchBuffer->begin());
	std::ranges::copy(tempStitchBuffer, wrap::next(StitchBuffer->begin(), grpSize));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void thred::mv2b() {
  if (StateMap->testAndReset(StateFlag::FORMSEL)) {
	thred::savdo();
	auto       tempStitchBuffer = std::vector<F_POINT_ATTR> {};
	auto       iLowBuffer       = 0U;
	auto const attribute        = ClosestFormToCursor << FRMSHFT;
	for (auto& stitch : *StitchBuffer) {
	  if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == attribute) {
		tempStitchBuffer.push_back(stitch);
	  }
	  else {
		StitchBuffer->operator[](iLowBuffer++) = stitch;
	  }
	}
	std::ranges::copy(tempStitchBuffer, wrap::next(StitchBuffer->begin(), iLowBuffer));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	std::vector<F_POINT_ATTR> tempStitchBuffer {};
	auto const                grpSize = GroupEndStitch + 1U - GroupStartStitch;
	tempStitchBuffer.resize(grpSize);
	std::copy(wrap::next(StitchBuffer->begin(), GroupStartStitch),
	          wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U),
	          StitchBuffer->end(),
	          wrap::next(StitchBuffer->begin(), GroupStartStitch));
	std::ranges::copy(tempStitchBuffer, wrap::next(StitchBuffer->begin(), StitchBuffer->size() - grpSize));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void thi::infadj(float& xCoordinate, float& yCoordinate) noexcept {
  if (isfinite(xCoordinate)) {
	auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx);
	if (xCoordinate > unzoomedX) {
	  xCoordinate = unzoomedX;
	}
	else {
	  if (xCoordinate < 0.0F) {
		xCoordinate = 0.0F;
	  }
	}
  }
  if (isfinite(yCoordinate)) {
	auto const unzoomedY = wrap::toFloat(UnzoomedRect.cy);
	if (yCoordinate > unzoomedY) {
	  yCoordinate = unzoomedY;
	}
	else {
	  if (yCoordinate < 0.0F) {
		yCoordinate = 0.0F;
	  }
	}
  }
}

void thred::delinf() noexcept {
  for (auto& iStitch : *StitchBuffer) {
	thi::infadj(iStitch.x, iStitch.y);
  }
  for (auto& formVertice : *FormVertices) {
	thi::infadj(formVertice.x, formVertice.y);
  }
}

void thred::chkrng(F_POINT& range) {
  thred::savdo();
  thred::delinf();
  wrap::narrow(range.x, UnzoomedRect.cx);
  wrap::narrow(range.y, UnzoomedRect.cy);
  if (!FormList->empty()) {
	// ToDo - Why do we treat the forms differently?
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
	}
	StitchBuffer->resize(wrap::toSize(std::distance(StitchBuffer->begin(), iDestination)));
	thred::coltab();
	return;
  }
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

void thred::ritmov(uint32_t formIndex) noexcept {
  auto const& form = FormList->operator[](formIndex);
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  if (ClosestVertexToCursor != 0U) {
	if (ClosestVertexToCursor == (form.vertexCount - 1U) && form.type == FRMLINE) {
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

void thi::duprct(FRM_HEAD const& form) {
  auto currentVertex        = SelectedFormVertices.start;
  auto itVertex             = wrap::next(FormVertices->cbegin(), form.vertexIndex + currentVertex);
  SelectedVerticesRect.left = SelectedVerticesRect.right = itVertex->x;
  SelectedVerticesRect.top = SelectedVerticesRect.bottom = itVertex->y;
  currentVertex                                          = form::pdir(form, currentVertex);
  for (auto iVertex = 0U; iVertex < SelectedFormVertices.vertexCount; ++iVertex) {
	itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + currentVertex);
	if (itVertex->x < SelectedVerticesRect.left) {
	  SelectedVerticesRect.left = itVertex->x;
	}
	if (itVertex->x > SelectedVerticesRect.right) {
	  SelectedVerticesRect.right = itVertex->x;
	}
	if (itVertex->y < SelectedVerticesRect.bottom) {
	  SelectedVerticesRect.bottom = itVertex->y;
	}
	if (itVertex->y > SelectedVerticesRect.top) {
	  SelectedVerticesRect.top = itVertex->y;
	}
	currentVertex = form::pdir(form, currentVertex);
  }
}

void thred::setpsel() {
  form::unpsel();
  auto const& form = FormList->operator[](ClosestFormToCursor);
  thi::duprct(form);
  SelectedPixelsRect = form::sRct2px(SelectedVerticesRect);
  form::rct2sel(SelectedPixelsRect, *SelectedPointsLine);
  auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + SelectedFormVertices.finish);
  EndPointCross = form::sfCor2px(*itVertex);
  StateMap->set(StateFlag::SHOPSEL);
  form::dupsel(StitchWindowDC);
  StateMap->set(StateFlag::FPSEL);
}

void thred::rotfn(float rotationAngle, F_POINT const& rotationCenter) {
  thred::savdo();
  if (StateMap->test(StateFlag::FPSEL)) {
	// clang-format off
	auto&       form          = FormList->operator[](ClosestFormToCursor);
	auto        currentVertex = SelectedFormVertices.start;
	auto  const vBegin        = wrap::next(FormVertices->begin(), form.vertexIndex);
	// clang-format on
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto itVertex = wrap::next(vBegin, currentVertex);
	  thred::rotflt(*itVertex, rotationAngle, rotationCenter);
	  currentVertex = form::pdir(form, currentVertex);
	}
	form.outline();
	thred::setpsel();
	form::refil(ClosestFormToCursor);
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	for (auto& formVertice : *FormVertices) {
	  thred::rotflt(formVertice, rotationAngle, rotationCenter);
	}
	for (auto& stitch : *StitchBuffer) {
	  thi::rotstch(stitch, rotationAngle, rotationCenter);
	}
	for (auto& iForm : *FormList) {
	  iForm.outline();
	}
	form::selal();
	return;
  }
  if (StateMap->testAndReset(StateFlag::FRMSROT)) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  // clang-format off
	  auto& form     = FormList->operator[](selectedForm);
	  auto  itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		thred::rotflt(*itVertex, rotationAngle, rotationCenter);
		++itVertex;
	  }
	  form.outline();
	  form::refilfn(selectedForm);
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->testAndReset(StateFlag::FRMROT)) {
	auto& form = FormList->operator[](ClosestFormToCursor);

	auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  thred::rotflt(*itVertex, rotationAngle, rotationCenter);
	  ++itVertex;
	}
	form.outline();
	form::refil(ClosestFormToCursor);
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	thi::rotstch(StitchBuffer->operator[](iStitch), rotationAngle, rotationCenter);
  }
  thred::rngadj();
  thi::selin(GroupStartStitch, GroupEndStitch, StitchWindowDC);
  StateMap->set(StateFlag::RESTCH);
}

void thi::rotfns(float rotationAngle) {
  thred::savdo();
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
}

void thred::showOnlyLayer(uint8_t play) {
  ActiveLayer = play;
  menu::ladj();
  if (ActiveLayer == 0U) {
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::FORMSEL) &&
      ((gsl::narrow_cast<decltype(ActiveLayer)>(FormList->operator[](ClosestFormToCursor).attribute & FRMLMSK) >>
        1U) != ActiveLayer)) {
	StateMap->reset(StateFlag::FORMSEL);
  }
  StateMap->reset(StateFlag::GRPSEL);
  if (StateMap->test(StateFlag::SELBOX) &&
      (ActiveLayer != ((StitchBuffer->operator[](ClosestPointIndex).attribute & LAYMSK) >> LAYSHFT) + 1U)) {
	StateMap->reset(StateFlag::SELBOX);
  }
  SelectedFormList->clear();
  StateMap->set(StateFlag::RESTCH);
}

auto thred::iselpnt() noexcept -> bool {
  auto const pointToTest = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto closestControlPoint = 0U;
  auto minimumLength       = BIGDBL;
  auto iControlPoint       = 0U;
  for (auto const& controlPoint : *FormControlPoints) {
	auto const deltaX = pointToTest.x - controlPoint.x;
	auto const deltaY = pointToTest.y - controlPoint.y;
	auto const length = deltaX * deltaX + deltaY * deltaY;
	if (length < minimumLength) {
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
  if (StateMap->testAndReset(StateFlag::SHOSTRTCH)) {
	thred::strtchbox(stretchBoxLine);
  }
}

// Does the outline have zero width or zero height?
auto thred::isLine(std::vector<POINT> const& boxOutline) noexcept -> bool {
  return (boxOutline[0].x == boxOutline[1].x) || (boxOutline[1].y == boxOutline[2].y);
}

// Is the point inside the box?
auto thi::isInBox(POINT const& point, RECT const& box) noexcept -> bool {
  return (point.x >= box.left && point.x <= box.right && point.y <= box.top && point.y >= box.bottom);
}

auto thred::chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  auto minLength = BIGDBL;
  auto const pointToTest = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto controlPoint = SelectedFormsLine->begin();

  auto const endPoint = SelectedFormsLine->size();
  for (auto iControlPoint = 0U; iControlPoint < endPoint; ++iControlPoint) {
	auto const deltaX = pointToTest.x - controlPoint->x;
	auto const deltaY = pointToTest.y - controlPoint->y;
	auto const length = deltaX * deltaX + deltaY * deltaY;
	if (length < minLength) {
	  minLength                 = length;
	  SelectedFormControlVertex = iControlPoint;
	}
	++controlPoint;
  }
  auto const minimumLength = std::sqrt(minLength);

  auto& formLines = *FormLines;
  formLines.resize(SQPNTS);
  for (auto iCorner = 0U; iCorner < 4; ++iCorner) {
	formLines[iCorner] = SelectedFormsLine->operator[](wrap::toSize(iCorner) * 2U);
  }
  formLines[4] = formLines[0];
  if (minimumLength < CLOSENUF && !thred::isLine(formLines)) {
	for (auto iCorner = 0U; iCorner < 4; ++iCorner) {
	  stretchBoxLine[iCorner] = SelectedFormsLine->operator[](wrap::toSize(iCorner) * 2U);
	}
	stretchBoxLine[4] = stretchBoxLine[0];
	thred::strtchbox(stretchBoxLine);
	if ((SelectedFormControlVertex & 1U) != 0U) {
	  StateMap->set(StateFlag::STRTCH);
	}
	else {
	  StateMap->set(StateFlag::EXPAND);
	  xyRatio = wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left) /
	            wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
	}
	SelectedFormControlVertex >>= 1U;
	StateMap->set(StateFlag::SHOSTRTCH);
	return true;
  }
  SelectedFormControlVertex >>= 1U;
  if (thi::isInBox(pointToTest, SelectedFormsRect) || (minimumLength < CLOSENUF && thred::isLine(formLines))) {
	SelectedFormsSize = F_POINT {SelectedFormsRect.right - SelectedFormsRect.left,
	                             SelectedFormsRect.bottom - SelectedFormsRect.top};
	StateMap->set(StateFlag::MOVFRMS);
	FormMoveDelta =
	    F_POINT {pointToTest.x - SelectedFormsRect.left, pointToTest.y - SelectedFormsRect.top};
	StateMap->set(StateFlag::SHOSTRTCH);
	thred::strtchbox(stretchBoxLine);
	return true;
  }
  return false;
}

void thred::delfre() {
  auto currentStitchCount = 0U;
  thred::savdo();
  // ToDo - this loop does not delete all free stitches. look at frmdel as well
  for (auto const& stitch : *StitchBuffer) {
	if ((stitch.attribute & NOTFRM) == 0U) {
	  StitchBuffer->operator[](currentStitchCount++) = stitch;
	}
  }
  StitchBuffer->resize(currentStitchCount);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::colchk() noexcept(std::is_same_v<size_t, uint32_t>) {
  if (StitchBuffer->empty()) {
	return;
  }
  auto color       = StitchBuffer->front().attribute & COLMSK;
  auto startStitch = 0U;
  // ToDo - Can this loop become a ranged for?
  for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	auto const& stitch = StitchBuffer->operator[](iStitch);
	if (color == (stitch.attribute & COLMSK)) {
	  continue;
	}
	if ((iStitch - startStitch == 1) && ((startStitch) != 0U)) {
	  auto& prevStitch     = StitchBuffer->operator[](iStitch - 1U);
	  prevStitch.attribute = (stitch.attribute & NCOLMSK) | (prevStitch.attribute & COLMSK);
	}
	color       = stitch.attribute & COLMSK;
	startStitch = iStitch;
  }
}

auto thi::makbig(uint32_t start, uint32_t finish) -> uint32_t {
  auto newStitches = std::vector<F_POINT_ATTR> {};
  newStitches.reserve(finish - start); // we know that we will have at least this number of Stitches
  auto adcnt = 0U;
  --finish;
  auto stitchIt     = wrap::next(StitchBuffer->begin(), start);
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
		if (((attribute & NOTFRM) == 0U) && ((nextStitchIt->attribute & TYPMSK) != 0U)) {
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
	  attribute &= (~KNOTMSK);
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
            wrap::next(StitchBuffer->begin(), start));
  // and then insert the remainder of the new stitches
  StitchBuffer->insert(wrap::next(StitchBuffer->begin(), finish),
                       wrap::next(newStitches.begin(), finish - start),
                       newStitches.end());
  return adcnt;
}

void thred::rembig() {
  if (StitchBuffer->empty()) {
	return;
  }
  if (UserStitchLength >= IniFile.maxStitchLength) {
	displayText::tabmsg(IDS_REM1, false);
	return;
  }
  thred::savdo();
  while (true) {
	if (!SelectedFormList->empty()) {
	  auto range = RANGE {};
	  for (auto const selectedForm : (*SelectedFormList)) {
		if (form::frmrng(selectedForm, range)) {
		  thi::makbig(range.start, range.finish);
		}
	  }
	  break;
	}
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto range = RANGE {};
	  if (form::frmrng(ClosestFormToCursor, range)) {
		thi::makbig(range.start, range.finish);
	  }
	  break;
	}
	if (StateMap->test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  if (GroupEndStitch < wrap::toUnsigned(StitchBuffer->size())) {
		++GroupEndStitch;
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
	break;
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thi::duselrng(RANGE& selectedRange) {
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	selectedRange.start  = GroupStartStitch;
	selectedRange.finish = GroupEndStitch;
	return;
  }
  selectedRange.start  = 0U;
  selectedRange.finish = wrap::toUnsigned(StitchBuffer->size());
}

void thred::nextSortedStitch(bool direction) {
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
  auto const nextStitch = wrap::next(SortBuffer->begin(), SortIndex);
  outDebugString(L"SortIndex [{}]\n", SortIndex);
  CurrentStitchIndex   = nextStitch->index;
  auto const minLength = std::sqrt(nextStitch->length) * IPFGRAN;
  displayText::butxt(HMINLEN, fmt::format(FMT_COMPILE(L"{:.2f}"), minLength));
  thi::lensadj();
  displayText::ritnum(IDS_NUMSCH, ClosestPointIndex);
}

void thi::setsrch(bool end) {
  if (end) {
	CurrentStitchIndex = LargestStitchIndex;
	SortIndex          = wrap::toUnsigned(SortBuffer->size() - 1U);
  }
  else {
	CurrentStitchIndex = SmallestStitchIndex;
	SortIndex          = 0U;
  }
  thi::lensadj();
  displayText::ritnum(IDS_NUMSCH, ClosestPointIndex);
}

auto thred::inrng(uint32_t iStitch) noexcept -> bool {
  if (iStitch >= StitchBuffer->size()) {
	return false;
  }
  auto const& stitch = StitchBuffer->operator[](iStitch);
  return stitch.x >= StitchRangeRect.left && stitch.x <= StitchRangeRect.right &&
         stitch.y >= StitchRangeRect.bottom && stitch.y <= StitchRangeRect.top;
}

void thi::barnam(HWND window, uint32_t iThumbnail) {
  if (iThumbnail >= ThumbnailDisplayCount) {
	SetWindowText(window, static_cast<LPCWSTR>(L""));
	return;
  }
  auto const thumbPath = fs::path(Thumbnails->operator[](ThumbnailsSelected.at(iThumbnail)).data());
  constexpr auto TNAMELEN = 12U; // how many characters to display with each thumbnail

  auto const name = thumbPath.stem().wstring().substr(0U, TNAMELEN);
  SetWindowText(window, name.c_str());
}

void thi::rthumnam(uint32_t iThumbnail) {
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
	  outDebugString(L"default hit in rthumbnam: iThumbnail [{}]\n", iThumbnail);
	  break;
	}
  }
}

void thred::thumnail() {
  auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
  thred::unbsho();
  thred::undat();
  trace::untrace();
  fs::current_path(*DefaultDirectory);
  auto const searchName = *DefaultDirectory / L"*.thr";
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const file = FindFirstFile(searchName.wstring().c_str(), &fileData);
  if (file == INVALID_HANDLE_VALUE) {
	auto const dwError = GetLastError();
	auto const fmtStr  = displayText::format2(IDS_FFINDERR, searchName.wstring(), dwError);
	displayText::shoMsg(fmtStr, true);
	thi::unthum();
	return;
  }
  Thumbnails->clear();
  auto& fileName = fileData.cFileName;
  Thumbnails->push_back(std::wstring(std::begin(fileName)));
  while (FindNextFile(file, &fileData)) {
	Thumbnails->push_back(std::wstring(std::begin(fileName)));
  }
  FindClose(file);
  std::ranges::sort(*Thumbnails);
  auto       iThumbnail = 0U;
  auto const thumbSize  = Thumbnails->size();
  while (iThumbnail < 4 && iThumbnail < thumbSize) {
	ThumbnailsSelected.at(iThumbnail) = iThumbnail;
	++iThumbnail;
  }
  ThumbnailIndex = ThumbnailDisplayCount = iThumbnail;
  while (iThumbnail < 4 && iThumbnail < thumbSize) {
	thi::rthumnam(iThumbnail++);
  }
  StateMap->set(StateFlag::THUMSHO);
  ThumbnailSearchString->clear();
  ThumbnailSearchString->push_back(0);
  SetWindowText(ButtonWin->operator[](HBOXSEL), L"");
  auto const blank = std::wstring {};
  displayText::butxt(HBOXSEL, blank);
  thred::vubak();
  StateMap->set(StateFlag::RESTCH);
}

void thi::nuthsel() {
  if (ThumbnailIndex >= Thumbnails->size()) {
	return;
  }
  auto const savedIndex = ThumbnailIndex;
  auto       iThumbnail = uint32_t {};
  auto const length     = wcslen(ThumbnailSearchString->data());
  StateMap->set(StateFlag::RESTCH);
  if (length != 0U) {
	auto itHWndBV = BackupViewer.begin();
	while (iThumbnail < QUADRT && ThumbnailIndex < Thumbnails->size()) { // there are 4 quadrants
	  if (_wcsnicmp(ThumbnailSearchString->data(), Thumbnails->operator[](ThumbnailIndex).data(), length) == 0) {
		ThumbnailsSelected.at(iThumbnail) = ThumbnailIndex;
		thred::redraw(*itHWndBV);
		++itHWndBV;
		++iThumbnail;
	  }
	  ++ThumbnailIndex;
	}
  }
  else {
	auto itHWndBV = BackupViewer.begin();
	while (iThumbnail < QUADRT && ThumbnailIndex < Thumbnails->size()) { // there are 4 quadrants
	  ThumbnailsSelected.at(iThumbnail) = ThumbnailIndex;
	  thred::redraw(*itHWndBV);
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

void thi::nuthbak(uint32_t count) {
  if (ThumbnailIndex == 0U) {
	return;
  }
  constexpr auto MAXFORMS = uint32_t {1024U}; // maximum number of forms
  auto const     length   = wcslen(ThumbnailSearchString->data());
  if (length != 0U) {
	while ((count != 0U) && ThumbnailIndex < MAXFORMS) {
	  if (ThumbnailIndex != 0U) {
		--ThumbnailIndex;
		if (_wcsnicmp(ThumbnailSearchString->data(), Thumbnails->operator[](ThumbnailIndex).data(), length) == 0) {
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

void thi::nuthum(wchar_t character) {
  StateMap->set(StateFlag::RESTCH);
  ThumbnailSearchString->back() = character;
  ThumbnailSearchString->push_back(0);

  auto const txt = std::wstring(ThumbnailSearchString->data());
  displayText::butxt(HBOXSEL, txt);
  ThumbnailIndex = 0;
  nuthsel();
}

void thi::bakthum() {
  if (ThumbnailSearchString->size() <= 1) {
	return;
  }
  StateMap->set(StateFlag::RESTCH);
  ThumbnailSearchString->pop_back();
  ThumbnailSearchString->back() = 0;
  ThumbnailIndex                = 0;
  auto const txt                = std::wstring(ThumbnailSearchString->data());
  displayText::butxt(HBOXSEL, txt);
  nuthsel();
}

void thred::selalstch() {
  if (StitchBuffer->empty()) {
	return;
  }
  ClosestPointIndex = 0;
  GroupStitchIndex  = wrap::toUnsigned(StitchBuffer->size() - 1U);
  GroupStartStitch  = ClosestPointIndex;
  GroupEndStitch    = GroupStitchIndex;
  StateMap->set(StateFlag::GRPSEL);
  StateMap->set(StateFlag::SCROS);
  StateMap->set(StateFlag::ECROS);
  thred::grpAdj();
  StateMap->set(StateFlag::RESTCH);
}

void thred::duinsfil() {
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  auto const offset      = F_POINT {stitchPoint.x - InsertCenter.x, stitchPoint.y - InsertCenter.y};
  for (auto iForm = InsertedFormIndex; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& formRectangle = FormList->operator[](iForm).rectangle;
	formRectangle.bottom += offset.y;
	formRectangle.top += offset.y;
	formRectangle.left += offset.x;
	formRectangle.right += offset.x;
  }
  for (auto iVertex = InsertedVertexIndex; iVertex < wrap::toUnsigned(FormVertices->size()); ++iVertex) {
	FormVertices->operator[](iVertex) += offset;
  }
  for (auto iStitch = InsertedStitchIndex; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	StitchBuffer->operator[](iStitch) += offset;
  }
  StateMap->reset(StateFlag::FRMOF);
  StateMap->set(StateFlag::INIT);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::gotbox() {
  StateMap->reset(StateFlag::BZUMIN);
  StateMap->reset(StateFlag::BOXSLCT);
  StateMap->reset(StateFlag::FRMPSEL);
  thred::grpAdj();
}

void thred::nucols() {
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto const selectedForm : (*SelectedFormList)) {
	formMap.set(selectedForm);
	auto& form = FormList->operator[](selectedForm);
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
  for (auto& stitch : *StitchBuffer) {
	if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT) && (stitch.attribute & TYPMSK) != TYPMSK) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= ActiveColor;
	}
  }
}

auto thi::dunum(wchar_t code) noexcept -> bool {
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

void thred::stchrct(F_RECTANGLE& rectangle) noexcept {
  if (StitchBuffer->empty()) {
	rectangle = F_RECTANGLE {};
	return;
  }
  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;

  for (auto const& stitch : *StitchBuffer) {
	minX = std::min(minX, stitch.x);
	maxX = std::max(maxX, stitch.x);
	minY = std::min(minY, stitch.y);
	maxY = std::max(maxY, stitch.y);
  }

  rectangle = F_RECTANGLE {minX, maxY, maxX, minY};
}

void thred::frmrct(F_RECTANGLE& rectangle) noexcept {
  if (FormVertices->empty()) {
	rectangle = F_RECTANGLE {};
	return;
  }
  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;

  for (auto const& formVertice : *FormVertices) {
	minX = std::min(minX, formVertice.x);
	minY = std::min(minY, formVertice.y);
	maxX = std::max(maxX, formVertice.x);
	maxY = std::max(maxY, formVertice.y);
  }

  rectangle = F_RECTANGLE {minX, maxY, maxX, minY};
}

void thi::sidhup() {
  auto hoopRectangle        = RECT {};
  auto preferencesRectangle = RECT {};
  StateMap->set(StateFlag::HUPMSG);
  GetWindowRect(ValueWindow->operator[](PRFHUPTYP), &hoopRectangle);
  GetWindowRect(PreferencesWindow, &preferencesRectangle);
  constexpr auto SMW_FLAGS = DWORD {WS_BORDER | WS_CHILD | WS_VISIBLE};
  SideMessageWindow        = CreateWindow(L"STATIC",
                                   nullptr,
                                   SMW_FLAGS,
                                   preferencesRectangle.right + 3 - ThredWindowOrigin.x,
                                   hoopRectangle.top - ThredWindowOrigin.y,
                                   ButtonWidthX3 + ButtonWidth * 2 + 6,
                                   ButtonHeight * HUPS + 6,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  constexpr auto SW_FLAGS  = DWORD {SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER};
  for (auto iHoop = size_t {}; iHoop < HUPS; ++iHoop) {
	auto const idx = gsl::narrow_cast<int32_t>(iHoop);
	SideWindow->operator[](iHoop) =
	    CreateWindow(L"STATIC",
	                 displayText::loadStr(wrap::toUnsigned(iHoop) + IDS_HUP0).c_str(),
	                 SW_FLAGS,
	                 3,
	                 ButtonHeight * idx + 3,
	                 ButtonWidthX3 + ButtonWidth * 2,
	                 ButtonHeight,
	                 SideMessageWindow,
	                 nullptr,
	                 ThrEdInstance,
	                 nullptr);
  }
}

void thred::fop() {
  trace::untrace();
  if (!FormList->empty() || (!StitchBuffer->empty())) {
	if (thi::savcmp()) {
	  thi::nuFil(FileIndices::THR);
	  thred::showOnlyLayer(0U);
	}
	else {
	  displayText::savdisc();
	  StateMap->set(StateFlag::OSAV);
	}
  }
  else {
	thi::nuFil(FileIndices::THR);
	thred::showOnlyLayer(0U);
  }
}

void thi::clpradj(F_RECTANGLE& clipRectAdjusted, F_POINT_ATTR const& stitch) noexcept {
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

void thred::clpadj() {
  if (!StateMap->test(StateFlag::GRPSEL)) {
	displayText::shoseln(IDS_GRPMSG, IDS_RNGEND);
	return;
  }
  thred::rngadj();
  auto iStitch          = GroupStartStitch;
  auto clipRectAdjusted = F_RECTANGLE {StitchBuffer->operator[](iStitch).x,
                                       StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y,
                                       StitchBuffer->operator[](iStitch).x,
                                       StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y};
  ++iStitch;
  while (iStitch < GroupEndStitch) {
	thi::clpradj(clipRectAdjusted, StitchBuffer->operator[](iStitch++));
  }
  if (StitchBuffer->operator[](iStitch).x < clipRectAdjusted.left) {
	clipRectAdjusted.left = StitchBuffer->operator[](iStitch).x;
  }
  if (StitchBuffer->operator[](iStitch).x > clipRectAdjusted.right) {
	clipRectAdjusted.right = StitchBuffer->operator[](iStitch).x;
  }
  auto const clipMiddle = wrap::midl(clipRectAdjusted.right, clipRectAdjusted.left);
  StitchBuffer->operator[](GroupStartStitch).y = wrap::midl(clipRectAdjusted.top, clipRectAdjusted.bottom);
  StitchBuffer->operator[](GroupEndStitch).y = StitchBuffer->operator[](GroupStartStitch).y;
  if (StitchBuffer->operator[](GroupStartStitch).x < clipMiddle) {
	StitchBuffer->operator[](GroupStartStitch).x = clipRectAdjusted.left;
	StitchBuffer->operator[](GroupEndStitch).x   = clipRectAdjusted.right;
  }
  else {
	StitchBuffer->operator[](GroupEndStitch).x   = clipRectAdjusted.left;
	StitchBuffer->operator[](GroupStartStitch).x = clipRectAdjusted.right;
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::shftflt(F_POINT const& point) noexcept {
  if (point.x <= ZoomRect.left || point.x >= ZoomRect.right || point.y <= ZoomRect.bottom ||
      point.y >= ZoomRect.top) {
	thred::shft(point);
  }
}

void thred::defpref() {
  constexpr auto APSPAC  = 10.8F;         // applique border spacing
  constexpr auto DEFBPIX = uint16_t {4U}; // default form box pixels
  constexpr auto DEFANG  = 0.7853981F;    // default fill angle, 45 degrees

  UserFlagMap->reset();
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
  UserFlagMap->set(UserFlag::DAZHOL);
  UserFlagMap->set(UserFlag::DAZD);
  AppliqueColor          = COLORCNT - 1U;
  IniFile.AppStitchLen   = APSPAC;
  BorderWidth            = BRDWID;
  ButtonholeCornerLength = IBFCLEN;
  IniFile.chainSpace     = CHSDEF;
  IniFile.chainRatio     = CHRDEF;
  IniFile.fillAngle      = DEFANG;
  UserFlagMap->reset(UserFlag::SQRFIL);
  LineSpacing             = DEFSPACE * PFGRAN;
  ShowStitchThreshold     = SHOPNTS;
  IniFile.gridSize        = 2.0F * PFGRAN; // 2mm default spacing NOLINT(readability-magic-numbers)
  IniFile.hoopType        = LARGHUP;
  IniFile.hoopSizeX       = LHUPX;
  IniFile.hoopSizeY       = LHUPY;
  IniFile.cursorNudgeStep = NUGINI;
  IniFile.nudgePixels     = DEFPIX;
  UserFlagMap->set(UserFlag::BLUNT);
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
  UserFlagMap->set(UserFlag::FIL2OF);
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

void thred::dumrk(float xCoord, float yCoord) {
  if (StateMap->testAndReset(StateFlag::GMRK)) {
	thi::drwmrk(StitchWindowDC);
  }
  ZoomMarkPoint = F_POINT {xCoord, yCoord};
  StateMap->set(StateFlag::INIT);
  StateMap->set(StateFlag::GMRK);
  thi::drwmrk(StitchWindowDC);
  StateMap->set(StateFlag::WASMRK);
}

void thi::gselrng() noexcept {
  auto const& selectedFormList = *SelectedFormList;
  SelectedFormsRange.start = SelectedFormsRange.finish = selectedFormList[0];
  for (auto const selectedForm : selectedFormList) {
	if (selectedForm < SelectedFormsRange.start) {
	  SelectedFormsRange.start = selectedForm;
	}
	if (selectedForm > SelectedFormsRange.finish) {
	  SelectedFormsRange.finish = selectedForm;
	}
  }
}

auto thi::nuang(float originalAngle, float xDelta, float yDelta) noexcept -> float {
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

void thi::angdif(float& lowestAngle, float& highestAngle, float angle) noexcept {
  if (angle > highestAngle) {
	highestAngle = angle;
	return;
  }
  if (angle < lowestAngle) {
	lowestAngle = angle;
  }
}

void thred::rotmrk() {
  if (!StateMap->test(StateFlag::GMRK) ||
      !(StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL))) {
	displayText::shoseln(IDS_FSZ, IDS_SETROTM);
	return;
  }
  auto lowestAngle  = 0.0F;
  auto highestAngle = 0.0F;
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto const codedFormIndex = ClosestFormToCursor << FRMSHFT;
	// clang-format off
	  auto const& form     = FormList->operator[](ClosestFormToCursor);
	  auto        itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	// clang-format on
	auto const originalAngle = std::atan2(itVertex->y - ZoomMarkPoint.y, itVertex->x - ZoomMarkPoint.x);
	++itVertex;
	for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	  thi::angdif(
	      lowestAngle,
	      highestAngle,
	      thi::nuang(originalAngle, itVertex->x - ZoomMarkPoint.x, itVertex->y - ZoomMarkPoint.y));
	  ++itVertex;
	}
	for (auto const& stitch : *StitchBuffer) {
	  if ((stitch.attribute & FRMSK) == codedFormIndex) {
		thi::angdif(lowestAngle,
		            highestAngle,
		            thi::nuang(originalAngle, stitch.x - ZoomMarkPoint.x, stitch.y - ZoomMarkPoint.y));
	  }
	}
  }
  else {
	thred::rngadj();
	auto const originalAngle =
	    std::atan2(StitchBuffer->operator[](GroupStartStitch).y - ZoomMarkPoint.y,
	               StitchBuffer->operator[](GroupStartStitch).x - ZoomMarkPoint.x);
	for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
	  thi::angdif(lowestAngle,
	              highestAngle,
	              thi::nuang(originalAngle,
	                         StitchBuffer->operator[](iStitch).x - ZoomMarkPoint.x,
	                         StitchBuffer->operator[](iStitch).y - ZoomMarkPoint.y));
	}
  }
  auto const tAngle     = highestAngle - lowestAngle;
  auto const segments   = std::round(PI_F2 / tAngle);
  IniFile.rotationAngle = PI_F2 / segments;
  auto const fmtStr = displayText::format2(IDS_ROTMARK, IniFile.rotationAngle * RADDEGF, segments);
  displayText::shoMsg(fmtStr, false);
}

void thi::segentr(float rotationAngle) {
  if (rotationAngle == 0.0F) {
	rotationAngle = 1 * DEGRADF;
  }
  displayText::showMessage(IDS_ENTROT, (PI_F2 / rotationAngle));
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void thred::rotseg() {
  thi::segentr(IniFile.rotationAngle);
  StateMap->set(StateFlag::ENTRSEG);
}

void thred::pntmrk() {
  if (StateMap->test(StateFlag::SELBOX)) {
	thred::dumrk(StitchBuffer->operator[](ClosestPointIndex).x,
	             StitchBuffer->operator[](ClosestPointIndex).y);
	return;
  }
  if (StateMap->test(StateFlag::FRMPSEL)) {
	auto const itVertex =
	    wrap::next(FormVertices->cbegin(),
	               FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	thred::dumrk(itVertex->x, itVertex->y);
	return;
  }
  displayText::shoseln(IDS_STCH_FRM, IDS_SETMRK);
}

void thred::filfrms() {
  if (SelectedFormList->empty()) {
	if (!StateMap->test(StateFlag::FORMSEL)) {
	  return;
	}
	thred::savdo();
	form::refil(ClosestFormToCursor);
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  thred::savdo();
  for (auto const selectedForm : (*SelectedFormList)) {
	form::refilfn(selectedForm);
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::nuslst(uint32_t find) {
  thi::gselrng();
  // ToDo - Check this code. Does it do what is intended?
  if (find < SelectedFormsRange.start) {
	for (auto form = find; form < SelectedFormsRange.finish; ++form) {
	  SelectedFormList->push_back(form);
	}
	return;
  }
  if (find > SelectedFormsRange.finish) {
	for (auto form = SelectedFormsRange.start; form <= find; ++form) {
	  SelectedFormList->push_back(form);
	}
	return;
  }
  for (auto form = SelectedFormsRange.start; form <= find; ++form) {
	SelectedFormList->push_back(form);
  }
}

void thi::srchk() {
  StateMap->reset(StateFlag::FORMSEL);
  SelectedFormList->clear();
  if (StateMap->testAndSet(StateFlag::LENSRCH)) {
	if (StateMap->test(StateFlag::WASGRP)) {
	  ClosestPointIndex = GroupStartStitch = PrevGroupStartStitch;
	  GroupStitchIndex = GroupEndStitch = PrevGroupEndStitch;
	}
	else {
	  StateMap->reset(StateFlag::GRPSEL);
	}
  }
  else {
	if (StateMap->test(StateFlag::GRPSEL)) {
	  StateMap->set(StateFlag::WASGRP);
	  thred::rngadj();
	  PrevGroupStartStitch = GroupStartStitch;
	  PrevGroupEndStitch   = GroupEndStitch;
	}
	else {
	  StateMap->reset(StateFlag::WASGRP);
	}
  }
  duselrng(SelectedRange);
}

void thred::tglhid() {
  if (!StateMap->testAndFlip(StateFlag::HIDSTCH)) {
	StateMap->reset(StateFlag::FRMOF);
  }
  StateMap->set(StateFlag::RESTCH);
}

void thi::respac(FRM_HEAD& form) noexcept {
  if (!form.isClip()) {
	return;
  }
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
}

auto thi::chkminus(wchar_t code) noexcept -> bool {
  if (code != VK_OEM_MINUS && code != VK_SUBTRACT) { // is '-' key pressed?
	return false;
  }
  if (PreferenceIndex == PRFCLPPHS) { // Clipboard Offset in preferences
	return true;
  }
  if (auto const& form = FormList->operator[](ClosestFormToCursor);
      FormMenuChoice == LFRMSPAC && form.isFanClip()) { // Fill Spacing for fan clip fill
	return true;
  }
  if (FormMenuChoice == LWLKIND) { // Indent
	return true;
  }
  return false;
}

void thred::retrac() {
  if (!StateMap->test(StateFlag::GRPSEL)) {
	displayText::shoseln(IDS_GRPMSG, IDS_RETRAC);
	return;
  }
  thred::savdo();
  thred::rngadj();
  if (GroupStartStitch == 0U) {
	++GroupStartStitch;
  }
  auto const count       = GroupEndStitch - GroupStartStitch;
  auto const insertPoint = wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U);
  auto const startPoint = wrap::next(StitchBuffer->rbegin(), StitchBuffer->size() - GroupEndStitch);
  auto const endPoint   = wrap::next(startPoint, count);
  StitchBuffer->insert(insertPoint, startPoint, endPoint);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::setgrd(COLORREF color) {
  menu::setGridCols(color);
  thred::nuPen(GridPen, 1, color);
  IniFile.gridColor = color;
  StateMap->set(StateFlag::RESTCH);
}

void thred::ovrlay() {
  auto fileName = fs::path {};
  thred::insfil(fileName);
  StateMap->reset(StateFlag::INSFIL);
  StateMap->reset(StateFlag::FRMOF);
  StateMap->set(StateFlag::INIT);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thi::stchsnap(uint32_t start, uint32_t finish) noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  auto pnt = wrap::next(StitchBuffer->begin(), start);
  for (auto i = 0U; i < finish - start; ++i) {
	pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
	pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;
	++pnt;
  }
}

void thi::frmsnap(uint32_t start, uint32_t count) noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  auto itVertex = wrap::next(FormVertices->begin(), start);
  for (auto i = 0U; i < count; ++i) {
	itVertex->x = rintf(itVertex->x / IniFile.gridSize) * IniFile.gridSize;
	itVertex->y = rintf(itVertex->y / IniFile.gridSize) * IniFile.gridSize;
	++itVertex;
  }
}

void thred::gsnap() {
  if (StateMap->test(StateFlag::TXTRED)) {
	texture::txsnap();
	return;
  }
  if (SelectedFormList->empty()) {
	if (!StateMap->test(StateFlag::FORMSEL)) {
	  if (!StateMap->test(StateFlag::GRPSEL)) {
		displayText::shoseln(IDS_FGRPF, IDS_SNAP2GRD);
		return;
	  }
	  thred::savdo();
	  thred::rngadj();
	  thi::stchsnap(GroupStartStitch, GroupEndStitch + 1U);
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
	thred::savdo();
	auto& formIter = FormList->operator[](ClosestFormToCursor);
	thi::frmsnap(formIter.vertexIndex, formIter.vertexCount);
	formIter.outline();
	form::refil(ClosestFormToCursor);
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  thred::savdo();
  for (auto const selectedForm : (*SelectedFormList)) {
	ClosestFormToCursor = selectedForm;
	auto& formIter      = FormList->operator[](ClosestFormToCursor);
	thi::frmsnap(formIter.vertexIndex, formIter.vertexCount);
	formIter.outline();
	form::refil(ClosestFormToCursor);
  }
  StateMap->set(StateFlag::RESTCH);
}

void thi::ritlock(std::vector<WIN32_FIND_DATA> const& fileInfo, HWND hwndlg) noexcept {
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

auto CALLBACK thi::lockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  SetWindowLongPtr(hwndlg, DWLP_USER, lparam);
	  if (lparam != 0U) {
		// ToDo - investigate C++17 option shown here: https://web.archive.org/web/20220812120940/https://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
		auto*      fileInfo   = reinterpret_cast<std::vector<WIN32_FIND_DATA>*>(lparam);
		auto const searchName = *DefaultDirectory / L"*.thr";
		auto       result     = WIN32_FIND_DATA {};
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto const searchResult = FindFirstFile(searchName.wstring().c_str(), &result);
		if (searchResult == INVALID_HANDLE_VALUE) {
		  displayText::showMessage(IDS_NOTHRFIL, DefaultDirectory->wstring());
		  EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
		  return TRUE;
		}
		fileInfo->push_back(result);

		while (FindNextFile(searchResult, &result)) {
		  fileInfo->push_back(result);
		}
		ritlock(*fileInfo, hwndlg);
	  }
	  break;
	}
	case WM_COMMAND: {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
	  auto* fileInfo = reinterpret_cast<std::vector<WIN32_FIND_DATA>*>(GetWindowLongPtr(hwndlg, DWLP_USER));
	  if (fileInfo != nullptr) {
		constexpr auto NROMASK = BIGDWORD ^ FILE_ATTRIBUTE_READONLY; // invert FILE_ATTRIBUTE_READONLY
		auto& spFileInfo = *fileInfo;
		switch (LOWORD(wparam)) {
		  case IDCANCEL: {
			EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
			return TRUE;
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
			  auto  fileName  = *DefaultDirectory / std::begin(cFileName);
			  if (!SetFileAttributes(fileName.wstring().c_str(), iFile.dwFileAttributes)) {
				++fileError;
			  }
			}
			if (fileError != 0U) {
			  displayText::showMessage(IDS_LOCKNOT, fileError);
			}
			EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
			return TRUE;
		  }
		  default: {
			outDebugString(L"default hit in lockPrc 1: wparam [{}]\n", LOWORD(wparam));
			break;
		  }
		}
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

void thred::fileLock() noexcept {
  auto lockInfo = std::vector<WIN32_FIND_DATA> {};
#pragma warning(suppress : 26490)
  DialogBoxParam(ThrEdInstance,
                 MAKEINTRESOURCE(IDD_DLOCK),
                 ThrEdWindow,
                 thi::lockPrc,
                 reinterpret_cast<LPARAM>(&lockInfo)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}

void thred::delstch() {
  thred::savdo();
  StitchBuffer->clear();
  StitchBuffer->shrink_to_fit();
  TexturePointsBuffer->clear();
  TexturePointsBuffer->shrink_to_fit();
  thred::rstAll();
  form::clrfills();
  thred::resetColorChanges();
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  displayText::butxt(HTOT, blank);
  StateMap->set(StateFlag::RESTCH);
}

void thred::closfn() {
  thred::deltot();
  Knots->clear();
  WorkingFileName->clear();
  bitmap::delmap();
  backup::deldu();
  displayText::clrhbut(3);
  auto const fmtStr = displayText::format(IDS_THRED, *DesignerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
}

void thred::filclos() {
  if (thi::savcmp()) {
	thred::closfn();
	return;
  }
  StateMap->set(StateFlag::FCLOS);
  displayText::savdisc();
}

void thi::frmpos(FRM_HEAD& form, float deltaX, float deltaY) noexcept(!(std::is_same_v<ptrdiff_t, int>)) {
  auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*itVertex += F_POINT {deltaX, deltaY};
	++itVertex;
  }
  form.rectangle.left += deltaX;
  form.rectangle.top += deltaY;
  form.rectangle.right += deltaX;
  form.rectangle.bottom += deltaY;
}

void thred::nudgfn(float deltaX, float deltaY) {
  if (StateMap->test(StateFlag::BIGBOX) || !SelectedFormList->empty() || StateMap->test(StateFlag::FORMSEL) ||
      StateMap->test(StateFlag::GRPSEL) || StateMap->test(StateFlag::SELBOX)) {
	thred::savdo();
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	for (auto& iForm : *FormList) {
	  thi::frmpos(iForm, deltaX, deltaY);
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch += F_POINT {deltaX, deltaY};
	}
	AllItemsRect->bottom += deltaY;
	AllItemsRect->top += deltaY;
	AllItemsRect->left += deltaX;
	AllItemsRect->right += deltaX;
	form::stchrct2px(AllItemsRect, SelectedFormsRect);
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (!SelectedFormList->empty()) {
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto const selectedForm : (*SelectedFormList)) {
	  formMap.set(selectedForm);
	}
	for (auto& stitch : *StitchBuffer) {
	  if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
		stitch += F_POINT {deltaX, deltaY};
	  }
	}
	for (auto const selectedForm : (*SelectedFormList)) {
	  auto& form = FormList->operator[](selectedForm);
	  thi::frmpos(form, deltaX, deltaY);
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto& form = FormList->operator[](ClosestFormToCursor);
	thi::frmpos(form, deltaX, deltaY);
	if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
		  stitch += F_POINT {deltaX, deltaY};
		}
	  }
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  StitchBuffer->operator[](iStitch) += F_POINT {deltaX, deltaY};
	}
	thred::grpAdj();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::SELBOX)) {
	StitchBuffer->operator[](ClosestPointIndex) += F_POINT {deltaX, deltaY};
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  auto pixel = POINT {};
  if (deltaX != 0.0F) {
	pixel.x = (deltaX > 0) ? IniFile.nudgePixels : -IniFile.nudgePixels;
  }
  if (deltaY != 0.0F) {
	pixel.y = (deltaY > 0) ? -IniFile.nudgePixels : IniFile.nudgePixels;
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
  if (!StateMap->test(StateFlag::WASMRK)) {
	displayText::tabmsg(IDS_MRK, false);
	return;
  }
  if (ZoomMarkPoint.x > IniFile.hoopSizeX) {
	ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
  }
  if (ZoomMarkPoint.y > IniFile.hoopSizeY) {
	ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
  }
  thred::dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
  F_POINT const point(ZoomMarkPoint);
  thred::shft(point);
  StateMap->set(StateFlag::RESTCH);
}

void thred::nuscol(size_t iColor) {
  auto const itUserColor = wrap::next(UserColor.begin(), iColor);
  auto const itUserPen   = wrap::next(UserPen->begin(), iColor);
  thred::nuPen(*itUserPen, 1, *itUserColor);
  auto const ucb = wrap::next(UserColorBrush.begin(), iColor);
  thi::nuBrush(*ucb, *itUserColor);
  thred::redraw(UserColorWin->operator[](iColor));
}

void thred::movchk() {
  static auto draggedColor = uint8_t {};
  if ((Msg.wParam & MK_LBUTTON) != 0U) {
	if (!StateMap->testAndSet(StateFlag::WASMOV)) {
	  if (thred::inDefaultColorWindows()) {
		draggedColor = VerticalIndex & COLORMAX;
		StateMap->set(StateFlag::WASCOL);
	  }
	}
	return;
  }
  if (!StateMap->testAndReset(StateFlag::WASMOV) || !StateMap->testAndReset(StateFlag::WASCOL)) {
	return;
  }
  if (!thred::inDefaultColorWindows() || draggedColor == VerticalIndex) {
	return;
  }
  thred::savdo();
  auto const key          = wrap::pressed(VK_SHIFT);
  auto const switchColors = wrap::pressed(VK_CONTROL);
  for (auto& stitch : *StitchBuffer) {
	auto const color = gsl::narrow_cast<uint8_t>(stitch.attribute & COLMSK);
	if (color == VerticalIndex) {
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
  for (auto& iForm : *FormList) {
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
	  thred::nuscol(draggedColor);
	}
	thred::nuscol(VerticalIndex);
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thi::inscol() {
  auto colorMap = boost::dynamic_bitset<>(COLORCNT);
  VerticalIndex &= COLMSK;
  for (auto const& stitch : *StitchBuffer) {
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
  for (auto& stitch : *StitchBuffer) {
	auto const color = gsl::narrow<uint8_t>(stitch.attribute & COLMSK);
	if (color < VerticalIndex || color >= nextColor) {
	  continue;
	}
	stitch.attribute &= NCOLMSK;
	stitch.attribute |= color + 1U;
  }
  for (auto& iForm : *FormList) {
	if ((iForm.fillType != 0U) && (iForm.fillColor >= VerticalIndex) && (iForm.fillColor < nextColor)) {
	  ++(iForm.fillColor);
	}
	if ((iForm.fillType == FTHF) && (iForm.feather.color >= VerticalIndex) && (iForm.feather.color < nextColor)) {
	  ++(iForm.feather.color);
	}
	if ((iForm.edgeType != 0U) && (iForm.borderColor >= VerticalIndex) && (iForm.borderColor < nextColor)) {
	  ++(iForm.borderColor);
	}
  }
  for (auto iColor = nextColor; iColor > VerticalIndex; --iColor) {
	if (iColor != 0U) {
	  UserColor.at(iColor) = UserColor.at(iColor - 1U);
	}
	thred::nuscol(iColor);
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

auto thred::usedcol(uint8_t index) -> bool {
  return std::ranges::any_of(*StitchBuffer, [&index](F_POINT_ATTR const& stitch) -> bool {
	return (stitch.attribute & COLMSK) == index;
  });
}

void thi::delcol() {
  if (thred::usedcol(VerticalIndex)) {
	displayText::tabmsg(IDS_COLU, false);
	return;
  }
  for (auto& stitch : *StitchBuffer) {
	auto const color = stitch.attribute & COLMSK;
	if (color <= VerticalIndex) {
	  continue;
	}
	stitch.attribute &= NCOLMSK;
	stitch.attribute |= color - 1U;
  }
  for (auto& iForm : *FormList) {
	if ((iForm.fillType != 0U) && (iForm.fillColor > VerticalIndex)) {
	  --(iForm.fillColor);
	}
	if (iForm.fillType == FTHF && iForm.feather.color > VerticalIndex) {
	  --(iForm.feather.color);
	}
	if ((iForm.edgeType != 0U) && (iForm.borderColor > VerticalIndex)) {
	  --(iForm.borderColor);
	}
  }
  for (auto iColor = VerticalIndex; iColor < COLORMAX; ++iColor) {
	UserColor.at(iColor) = UserColor.at(wrap::toSize(iColor) + 1U);
	thred::nuscol(iColor);
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::set1knot() {
  constexpr auto KNTSTCH = size_t {5U}; // number of stitches in knot
  if (StitchBuffer->empty() || (!StateMap->test(StateFlag::SELBOX))) {
	displayText::tabmsg(IDS_NOSTCHSEL, false);
	return;
  }
  thred::savdo();
  auto buffer = std::vector<F_POINT_ATTR> {};
  buffer.reserve(StitchBuffer->size() + KNTSTCH);
  buffer.insert(buffer.begin(), StitchBuffer->begin(), wrap::next(StitchBuffer->begin(), ClosestPointIndex + 1U));
  if (ClosestPointIndex == wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	StateMap->set(StateFlag::FILDIR);
	thi::endknt(buffer, ClosestPointIndex);
  }
  else {
	thi::strtknt(buffer, ClosestPointIndex);
  }
  buffer.insert(
      buffer.end(), wrap::next(StitchBuffer->begin(), ClosestPointIndex + 1U), StitchBuffer->end());
  *StitchBuffer = std::move(buffer);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::fixpclp(uint32_t closestFormToCursor) {
  auto const point      = POINT {(Msg.pt.x + gsl::narrow_cast<decltype(Msg.pt.x)>(FormMoveDelta.x)),
                            (Msg.pt.y + gsl::narrow_cast<decltype(Msg.pt.y)>(FormMoveDelta.y))};
  auto       itIntlvSeq = std::next(InterleaveSequence->begin());
  auto const stitchPoint = thred::pxCor2stch(point);
  auto const offset      = F_POINT {stitchPoint.x - itIntlvSeq->x, stitchPoint.y - itIntlvSeq->y};
  auto const count       = wrap::toUnsigned(InterleaveSequence->size()) - 2U;
  auto&      form        = FormList->operator[](closestFormToCursor);
  auto const nextVertex  = form::nxt(form, ClosestVertexToCursor);
  form::fltspac(nextVertex, count);
  form.vertexCount += count;
  auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex + nextVertex);
  for (auto iOutput = size_t {1U}; iOutput < InterleaveSequence->size() - 1U; ++iOutput) {
	*itVertex = F_POINT {itIntlvSeq->x + offset.x, itIntlvSeq->y + offset.y};
	++itVertex;
	++itIntlvSeq;
  }
  form::refil(ClosestFormToCursor);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

auto thi::sidclp() -> bool {
  if (OpenClipboard(ThrEdWindow) == 0) {
	return false;
  }
  Clip       = RegisterClipboardFormat(PcdClipFormat);
  ClipMemory = GetClipboardData(Clip);
  if (ClipMemory == nullptr) {
	CloseClipboard();
	return false;
  }
  thred::redclp();
  CloseClipboard();
  return true;
}

void thred::selfpnt() {
  SelectedFormVertices.start = ClosestVertexToCursor;
  SelectedFormVertices.form  = ClosestFormToCursor;
  auto const itVertex =
      wrap::next(FormVertices->cbegin(),
                 FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
  thred::ritfcor(*itVertex);
  StateMap->set(StateFlag::FRMPSEL);
  StateMap->set(StateFlag::RESTCH);
}

auto thred::unselectAll() -> bool {
  StateMap->reset(StateFlag::FORMSEL);
  StateMap->reset(StateFlag::FRMPSEL);
  StateMap->reset(StateFlag::FPSEL);
  StateMap->reset(StateFlag::SELBOX);
  StateMap->set(StateFlag::RESTCH);
  thred::unmsg();
  thi::unboxs();
  thred::unpat();
  StateMap->reset(StateFlag::SATIN);
  if (StateMap->testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (StateMap->testAndReset(StateFlag::BIGBOX)) {
	StateMap->set(StateFlag::RESTCH);
  }
  if (StateMap->test(StateFlag::PRFACT)) {
	if (StateMap->testAndReset(StateFlag::HUPMSG)) {
	  thred::unsid();
	  return true;
	}
	if (PreferenceIndex != 0U) {
	  thi::chknum();
	  thred::unsid();
	  PreferenceIndex = 0;
	  return true;
	}
	DestroyWindow(PreferencesWindow);
	StateMap->reset(StateFlag::PRFACT);
	if (StateMap->testAndReset(StateFlag::WASRT)) {
	  StateMap->set(StateFlag::INSRT);
	}
  }
  if (FormMenuChoice != 0U) {
	thi::chknum();
	FormMenuChoice = 0;
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::INSFRM)) {
	thi::insadj();
	StateMap->reset(StateFlag::SHOINSF);
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->test(StateFlag::BAKSHO)) {
	thred::unbsho();
	return true;
  }
  if (!SelectedFormList->empty()) {
	SelectedFormList->clear();
	StateMap->set(StateFlag::RESTCH);
  }
  return false;
}

void thred::esccode() {
  bitmap::chkbit();
  menu::duhbit(MF_UNCHECKED);
  thi::unthum();
  StateMap->reset(StateFlag::MOVSET);
  StateMap->reset(StateFlag::HID);
  StateMap->reset(StateFlag::FRMOF);
  StateMap->reset(StateFlag::THRDS);
  thred::redraw(ButtonWin->operator[](HHID));
  menu::resetThreadView();
  StateMap->reset(StateFlag::RUNPAT);
  StateMap->reset(StateFlag::WASPAT);
  StateMap->reset(StateFlag::WASBLAK);
  StateMap->reset(StateFlag::GTUANG);
  StateMap->reset(StateFlag::GTUSPAC);
  StateMap->reset(StateFlag::GTWLKIND);
  StateMap->reset(StateFlag::GTWLKLEN);
  DestroyWindow(SpeedScrollBar);
  StateMap->reset(StateFlag::GMRK);
  StateMap->reset(StateFlag::FORMSEL);
  SelectedFormList->clear();
}

void thred::qcode() {
  trace::untrace();
  StateMap->reset(StateFlag::HIDMAP);
  StateMap->reset(StateFlag::FILDIR);
  ReleaseCapture();
  if (StitchBuffer->size() == 1) {
	StitchBuffer->clear();
	StateMap->reset(StateFlag::INIT);
  }
  if (StateMap->testAndReset(StateFlag::POLIMOV)) { // aborting form add
	if (FormList->back().vertexCount != 0U) {
	  auto const first = wrap::next(FormVertices->begin(), FormList->back().vertexIndex);
	  auto const last  = wrap::next(first, FormList->back().vertexCount);
	  FormVertices->erase(first, last);
	}
	if (FormList->back().satinGuideCount != 0U) {
	  auto const first = wrap::next(SatinGuides->begin(), FormList->back().satinGuideIndex);
	  auto const last  = wrap::next(first, FormList->back().satinGuideCount);
	  SatinGuides->erase(first, last);
	}
	FormList->pop_back();
	if (!FormList->empty()) {
	  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
	}
  }
  if (StateMap->testAndReset(StateFlag::FUNCLP)) { // aborting form paste
	backup::bak();
	ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  }
  if (StateMap->testAndReset(StateFlag::FUNSCLP)) { // aborting forms paste
	backup::bak();
	SelectedFormList->clear();
	ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  }
  if (!UserFlagMap->test(UserFlag::MARQ)) {
	StateMap->reset(StateFlag::GMRK);
  }
  if (StateMap->testAndReset(StateFlag::PRFACT)) {
	if (StateMap->testAndReset(StateFlag::WASRT)) {
	  StateMap->set(StateFlag::INSRT);
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}

	thred::rstAll();
  }
  else {
	thred::rstAll();
  }
  StateMap->set(StateFlag::RESTCH);
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
}

void thi::drwLin(std::vector<POINT>& linePoints, uint32_t currentStitch, uint32_t length, HPEN hPen) {
  if (StitchBuffer->empty() || length == 0) {
	return;
  }
  if (ActiveLayer != 0U) {
	linePoints.clear();
  }
  auto const firstStitch = wrap::next(StitchBuffer->cbegin(), currentStitch);
  auto const lastStitch  = wrap::next(firstStitch, length);
  auto const rStitches   = std::ranges::subrange(firstStitch, lastStitch);
  for (auto const& stitch : rStitches) {
	auto const layer = (stitch.attribute & LAYMSK) >> LAYSHFT;
	if ((ActiveLayer != 0U) && (layer != 0U) && (layer != ActiveLayer)) {
	  continue;
	}
	linePoints.push_back({std::lround((stitch.x - ZoomRect.left) * ZoomRatio.x),
	                      std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                  (stitch.y - ZoomRect.bottom) * ZoomRatio.y)});
  }
  SelectObject(StitchWindowMemDC, hPen);
  wrap::polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
  linePoints.clear();
  auto const layer = (firstStitch->attribute & LAYMSK) >> LAYSHFT;
  if ((ActiveLayer != 0U) && (layer != 0U) && layer != ActiveLayer) {
	return;
  }
  linePoints.push_back({std::lround((firstStitch->x - ZoomRect.left) * ZoomRatio.x),
                        std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    (firstStitch->y - ZoomRect.bottom) * ZoomRatio.y)});
}

auto CALLBACK thi::fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  auto const featherType = IniFile.featherType;
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT),
	                fmt::format(FMT_COMPILE(L"{:.2f}"), IniFile.featherRatio).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT),
	                fmt::format(FMT_COMPILE(L"{}"), IniFile.featherUpCount).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT),
	                fmt::format(FMT_COMPILE(L"{}"), IniFile.featherDownCount).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFLR),
	                fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.featherMinStitchSize * IPFGRAN)).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM),
	                fmt::format(FMT_COMPILE(L"{}"), IniFile.featherCount).c_str());
	  auto featherStyle = std::wstring {};
	  for (auto const& iFeatherStyle : FTHRLIST) {
		featherStyle.assign(displayText::loadStr(iFeatherStyle.stringID));
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
		SendMessage(GetDlgItem(hwndlg, IDC_FDTYP),
		            CB_ADDSTRING,
		            0,
		            reinterpret_cast<LPARAM>(featherStyle.c_str())); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	  }
	  SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, IniFile.featherFillType - 1, 0);
	  auto state = wrap::toUnsigned(((featherType & AT_FTHBLND) != 0) ? BST_CHECKED : BST_UNCHECKED);
	  CheckDlgButton(hwndlg, IDC_FDBLND, state);
	  state = ((featherType & AT_FTHUP) != 0) ? BST_CHECKED : BST_UNCHECKED;
	  CheckDlgButton(hwndlg, IDC_FDUP, state);
	  state = ((featherType & AT_FTHBTH) != 0) ? BST_CHECKED : BST_UNCHECKED;
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
		  if (IniFile.featherCount < 1) {
			IniFile.featherCount = 1;
		  }
		  EndDialog(hwndlg, 1);
		  break;
		}
		default: {
		  outDebugString(L"default hit in fthdefprc 1: wparam [{}]\n", LOWORD(wparam));
		  break;
		}
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

void thred::dufdef() noexcept {
  // ToDo - don't update values in DialogBox as then 'cancel' does not work
  // ReSharper disable CppClangTidyClangDiagnosticCastFunctionTypeStrict
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, reinterpret_cast<DLGPROC>(thi::fthdefprc)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, clang-diagnostic-cast-function-type-strict)
  // ReSharper restore CppClangTidyClangDiagnosticCastFunctionTypeStrict
}

auto thred::displayBackups() -> bool {
  auto iVersion = uint8_t {};
  for (auto const& iBackup : BackupViewer) {
	if (Msg.hwnd == iBackup) {
	  FileVersionIndex = iVersion;
	  if (StateMap->test(StateFlag::THUMSHO)) {
		if (thi::savcmp()) {
		  thumbak();
		}
		else {
		  if (StateMap->test(StateFlag::RBUT)) {
			thumbak();
		  }
		  else {
			StateMap->set(StateFlag::THUMON);
			displayText::savdisc();
		  }
		  return true;
		}
	  }
	  else {
		thi::rebak();
	  }
	  thred::rstAll();
	  StateMap->set(StateFlag::RESTCH);
	  return true;
	}
	++iVersion;
  }
  return false;
}

auto thred::inColorbar() noexcept -> bool {
  return (Msg.pt.x >= ColorBarRect.left && Msg.pt.x <= ColorBarRect.right &&
          Msg.pt.y >= ColorBarRect.top && Msg.pt.y <= ColorBarRect.bottom);
}

auto thred::getColorbarVertPosition() noexcept -> float {
  return (wrap::toFloat(Msg.pt.y - ColorBarRect.top) /
          wrap::toFloat(ColorBarRect.bottom - ColorBarRect.top));
}

void thred::handleFormSelected() {
  if (FormMenuChoice != 0U) {
	thi::chknum();
	FormMenuChoice = 0;
	return;
  }
  if (FormDataSheet != nullptr) {
	thred::undat();
	thred::unsid();
	FormMenuChoice = 0;
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
  lenCalc();
  if (!StateMap->testAndReset(StateFlag::ENTROT)) {
	StateMap->reset(StateFlag::FORMSEL);
  }
  StateMap->set(StateFlag::DUMEN);
  if (!thred::closPnt1(ClosestPointIndex)) {
	thred::unbox();
  }
}

void thred::updateHoopSize() {
  auto const itHwnd = std::ranges::find(*SideWindow, Msg.hwnd);
  if (itHwnd == SideWindow->end()) {
	return;
  }
  switch (auto const option = std::distance(SideWindow->begin(), itHwnd) + 1; option) {
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
  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
  form::sethup();
  thred::chkhup();
  thred::unsid();
  formForms::prfmsg();
}

auto thred::updateFillColor() -> bool {
  while (true) {
	thred::savdo();
	if (StateMap->testAndReset(StateFlag::FSETFCOL)) {
	  xt::dufcol(VerticalIndex + 1U);
	  return true;
	}
	if (StateMap->testAndReset(StateFlag::BRDSID)) {
	  form::nubrdcol(VerticalIndex);
	  break;
	}
	if (StateMap->testAndReset(StateFlag::APSID)) {
	  form::nulapcol(VerticalIndex);
	  break;
	}
	if (StateMap->testAndReset(StateFlag::FTHSID)) {
	  form::nufthcol(VerticalIndex);
	  break;
	}
	if (StateMap->testAndReset(StateFlag::UNDCOL)) {
	  FormList->operator[](ClosestFormToCursor).underlayColor = VerticalIndex & COLMSK;
	  form::refilfn(ClosestFormToCursor);
	  thred::coltab();
	  break;
	}
	if (StateMap->testAndReset(StateFlag::FSETUCOL)) {
	  xt::dundcol(VerticalIndex + 1U);
	  return true;
	}
	form::nufilcol(VerticalIndex);
	break;
  }
  auto buffer = std::array<wchar_t, 2> {};
  wrap::narrow(buffer[0], VerticalIndex + '0');
  SetWindowText(ValueWindow->operator[](LBRDCOL), buffer.data());
  thred::unsid();
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
  StateMap->reset(StateFlag::SIDCOL);
  FormMenuChoice = 0;
  return true;
}

auto thred::updatePreferences() -> bool {
  thi::chknum();
  if (Msg.hwnd == ValueWindow->operator[](PRFFILEND)) {
	if (UserFlagMap->testAndFlip(UserFlag::SQRFIL)) {
	  SetWindowText(ValueWindow->operator[](PRFFILEND), displayText::loadStr(IDS_PNTD).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PRFFILEND), displayText::loadStr(IDS_SQR).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PRFSATEND)) {
	if (UserFlagMap->testAndFlip(UserFlag::BLUNT)) {
	  SetWindowText(ValueWindow->operator[](PRFSATEND), displayText::loadStr(IDS_TAPR).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PRFSATEND), displayText::loadStr(IDS_BLUNT).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PRFSATUND)) {
	if (UserFlagMap->testAndFlip(UserFlag::DUND)) {
	  SetWindowText(ValueWindow->operator[](PRFSATUND), displayText::loadStr(IDS_OFF).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PRFSATUND), displayText::loadStr(IDS_ON).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PRFHUPTYP)) {
	thi::sidhup();
	return true;
  }
  for (auto iPreference = 0U; iPreference < PRFLINS; ++iPreference) {
	if (Msg.hwnd == ValueWindow->operator[](iPreference)) {
	  PreferenceIndex = iPreference + 1U;
	  outDebugString(L"handleLeftButtonDown:PreferenceIndex [{}]\n", PreferenceIndex);
	  formForms::prfsid(ValueWindow->operator[](iPreference));
	  break;
	}
  }
  return true;
}

auto thred::handleSideWindowActive() -> bool {
  thred::savdo();
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (FormMenuChoice == LFTHTYP) {
	auto const iFeather = std::ranges::find_if(FTHRLIST, [](LIST_TYPE const& feather) noexcept -> bool {
	  return Msg.hwnd == SideWindow->operator[](feather.value);
	});
	if (iFeather != FTHRLIST.end()) {
	  form.feather.fillType = iFeather->value;
	  thred::unsid();
	  form::refil(ClosestFormToCursor);
	  formForms::refrm();
	}
	return true;
  }
  if (FormMenuChoice == LLAYR) {
	auto const iLayer = std::ranges::find_if(LAYRLIST, [](LIST_TYPE const& layer) noexcept -> bool {
	  return Msg.hwnd == SideWindow->operator[](layer.value);
	});
	if (iLayer != LAYRLIST.end()) {
	  form::movlayr(iLayer->value);
	  StateMap->set(StateFlag::FORMSEL);
	  auto const layerStr = displayText::loadStr(iLayer->stringID);
	  SetWindowText(ValueWindow->operator[](LLAYR), layerStr.c_str());
	}
	thred::unsid();
	formForms::refrm();
	return true;
  }
  form.borderColor &= COLMSK;
  if (StateMap->testAndReset(StateFlag::BRDACT)) {
	while (true) {
	  if (Msg.hwnd == SideWindow->operator[](0)) {
		if (form.isEdgeClip()) {
		  clip::deleclp(ClosestFormToCursor);
		}
		form.edgeType = 0;
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGELINE)) {
		form::bord();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEBEAN)) {
		form::dubold();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGECLIP)) {
		form::fclp();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEANGSAT)) {
		satin::satbrd();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEAPPL)) {
		if (form.fillType != 0U) {
		  form::delmfil(ClosestFormToCursor);
		}
		form::apliq();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEPROPSAT)) {
		form::prpbrd(LineSpacing);
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEBHOL)) {
		form::bhol();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEPICOT)) {
		form::picot();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEDOUBLE)) {
		form::dubsfil(form);
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGELCHAIN)) {
		StateMap->set(StateFlag::LINCHN);
		form::chain();
		thred::coltab();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGEOCHAIN)) {
		StateMap->reset(StateFlag::LINCHN);
		form::chain();
		thred::coltab();
		break;
	  }
	  if (Msg.hwnd == SideWindow->operator[](EDGECLIPX)) {
		form::filclpx();
	  }
	  break;
	}
	formForms::refrm();
	form::refil(ClosestFormToCursor);
	thred::unmsg();
	thred::unsid();
	StateMap->set(StateFlag::RESTCH);
	return true;
  }

  if (form.fillType == SAT && (form.satinGuideCount != 0U)) {
	satin::delsac(ClosestFormToCursor);
  }
  if ((form.edgeType & NEGUND) == EDGEAPPL) {
	form.edgeType = EDGEANGSAT;
	if (UserFlagMap->test(UserFlag::DUND)) {
	  form.edgeType |= EGUND;
	}
  }
  auto textureFlag = false;
  while (true) {
	if (Msg.hwnd == SideWindow->operator[](0)) { // none
	  form.type = FRMFPOLY;
	  form::delmfil(ClosestFormToCursor);
	  form.fillType = 0;
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::VRTF)) { // vertical fill
	  thred::savdo();
	  form.type = FRMFPOLY;
	  if (form.fillType != 0U) {
		thi::respac(form);
		form.fillType = VRTF;
		form.type     = FRMFPOLY;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }
	  form::filvrt();
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::HORF)) { // horizontal fill
	  form.type = FRMFPOLY;
	  if (form.fillType != 0U) {
		thi::respac(form);
		form.fillType = HORF;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }
	  form::filhor();
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::ANGF)) { // angle fill
	  form.type = FRMFPOLY;
	  if (form.fillType != 0U) {
		if (form.satinGuideCount != 0U) {
		  satin::delsac(ClosestFormToCursor);
		}
		thi::respac(form);
		// ToDo - should we be using the angle information already present
		form.fillType  = ANGF;
		form.fillAngle = IniFile.fillAngle;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }
	  form::filangl();
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::SATF)) { // fan fill
	  form.type = SAT;
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinGuideIndex = 0;
	  }
	  if (form.fillType != 0U) {
		thi::respac(form);
		form.fillType = SATF;
		clip::delmclp(ClosestFormToCursor);
		break;
	  }
	  StateMap->reset(StateFlag::FTHR);
	  form::filsat();
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::CLPF)) { // fan clip
	  form.type = SAT;
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinGuideIndex = 0;
	  }
	  form::clpfil();
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::CONTF)) { // contour fill
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
	if (Msg.hwnd == SideWindow->operator[](FillStyles::VCLPF)) { // vertical clip
	  if (thi::sidclp()) {
		form::vrtsclp(ClosestFormToCursor);
	  }
	  StateMap->reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::HCLPF)) { // horizontal clip
	  if (thi::sidclp()) {
		form::horsclp();
	  }
	  StateMap->reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::ANGCLPF)) { // angle clip
	  if (thi::sidclp()) {
		if (form.satinGuideCount != 0U) {
		  satin::delsac(ClosestFormToCursor);
		}
		form::angsclp(form);
	  }
	  StateMap->reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::FTHF)) { // feather fill
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinGuideIndex = 0;
	  }
	  xt::fethrf(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow->operator[](FillStyles::TXVRTF)) // vertical texture
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
	if (Msg.hwnd == SideWindow->operator[](FillStyles::TXHORF)) // horizontal texture
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
	if (Msg.hwnd == SideWindow->operator[](FillStyles::TXANGF)) // angle texture
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
	thred::unsid();
	DestroyWindow(FormDataSheet);
	FormDataSheet = nullptr;
	StateMap->set(StateFlag::WASFRMFRM);
  }
  else {
	formForms::refrm();
	form::refil(ClosestFormToCursor);
	thred::unmsg();
	thred::unsid();
	StateMap->set(StateFlag::RESTCH);
  }
  return true;
}

auto thred::handleFormDataSheet() -> bool {
  thi::chknum();
  thred::unsid();
  auto& form = FormList->operator[](ClosestFormToCursor);
  while (true) {
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
	  form::refil(ClosestFormToCursor);
	  thred::unsid();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHUP) || Msg.hwnd == LabelWindow->operator[](LFTHUP)) {
	  form.extendedAttribute ^= AT_FTHUP;
	  form::refil(ClosestFormToCursor);
	  auto const choice = displayText::loadStr(((form.extendedAttribute & AT_FTHUP) == 0U) ? IDS_OFF : IDS_ON);
	  SetWindowText(ValueWindow->operator[](LFTHUP), choice.c_str());
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHBTH) || Msg.hwnd == LabelWindow->operator[](LFTHBTH)) {
	  form.extendedAttribute ^= AT_FTHBTH;
	  formForms::refrm();
	  form::refil(ClosestFormToCursor);
	  thred::unsid();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHTYP) || Msg.hwnd == LabelWindow->operator[](LFTHTYP)) {
	  FormMenuChoice = LFTHTYP;
	  thi::sidmsg(form, ValueWindow->operator[](LFTHTYP));
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
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LLAYR) || Msg.hwnd == LabelWindow->operator[](LLAYR)) {
	  FormMenuChoice = LLAYR;
	  StateMap->reset(StateFlag::FILTYP);
	  thi::sidmsg(form, ValueWindow->operator[](LLAYR));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMFIL) || Msg.hwnd == LabelWindow->operator[](LFRMFIL)) {
	  StateMap->reset(StateFlag::FILTYP);
	  FormMenuChoice = LFRMFIL;
	  thi::sidmsg(form, ValueWindow->operator[](LFRMFIL));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMCOL) || Msg.hwnd == LabelWindow->operator[](LFRMCOL)) {
	  FormMenuChoice = LFRMCOL;
	  formForms::sidwnd(ValueWindow->operator[](LFRMCOL));
	  StateMap->set(StateFlag::SIDCOL);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LUNDCOL) || Msg.hwnd == LabelWindow->operator[](LUNDCOL)) {
	  FormMenuChoice = LUNDCOL;
	  formForms::sidwnd(ValueWindow->operator[](LUNDCOL));
	  StateMap->set(StateFlag::SIDCOL);
	  StateMap->set(StateFlag::UNDCOL);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHCOL) || Msg.hwnd == LabelWindow->operator[](LFTHCOL)) {
	  FormMenuChoice = LFTHCOL;
	  formForms::sidwnd(ValueWindow->operator[](LFTHCOL));
	  StateMap->set(StateFlag::SIDCOL);
	  StateMap->set(StateFlag::FTHSID);
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
	  StateMap->set(StateFlag::FILTYP);
	  thi::sidmsg(form, ValueWindow->operator[](LBRD));
	  StateMap->set(StateFlag::BRDACT);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBRDCOL) || Msg.hwnd == LabelWindow->operator[](LBRDCOL)) {
	  FormMenuChoice = LBRDCOL;
	  formForms::sidwnd(ValueWindow->operator[](LBRDCOL));
	  StateMap->set(StateFlag::SIDCOL);
	  StateMap->set(StateFlag::BRDSID);
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
	  StateMap->set(StateFlag::SIDCOL);
	  StateMap->set(StateFlag::APSID);
	  formForms::sidwnd(ValueWindow->operator[](LAPCOL));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBCSIZ) || Msg.hwnd == LabelWindow->operator[](LBCSIZ)) {
	  FormMenuChoice = LBCSIZ;
	  formForms::sidwnd(ValueWindow->operator[](LBCSIZ));
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBSTRT) || Msg.hwnd == LabelWindow->operator[](LBSTRT)) {
	  auto const code     = form.attribute & SBLNT;
	  auto const bluntStr = displayText::loadStr((code != 0U) ? IDS_TAPR : IDS_BLUNT);
	  SetWindowText(ValueWindow->operator[](LBSTRT), bluntStr.c_str());
	  if (code != 0U) {
		form.attribute &= NSBLNT;
	  }
	  else {
		form.attribute |= SBLNT;
	  }
	  form::refil(ClosestFormToCursor);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LBFIN) || Msg.hwnd == LabelWindow->operator[](LBFIN)) {
	  auto const code     = form.attribute & FBLNT;
	  auto const bluntStr = displayText::loadStr((code != 0U) ? IDS_TAPR : IDS_BLUNT);
	  SetWindowText(ValueWindow->operator[](LBFIN), bluntStr.c_str());
	  if (code != 0U) {
		form.attribute &= NFBLNT;
	  }
	  else {
		form.attribute |= FBLNT;
	  }
	  form::refil(ClosestFormToCursor);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
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
	  form::refil(ClosestFormToCursor);
	  auto const code    = form.edgeType & EGUND;
	  auto const bUndStr = displayText::loadStr((code != 0U) ? IDS_ON : IDS_OFF);
	  SetWindowText(ValueWindow->operator[](LBRDUND), bUndStr.c_str());
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
	  auto const choice = displayText::loadStr(((form.extendedAttribute & AT_SQR) == 0U) ? IDS_PNTD : IDS_SQR);
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
	break;
  }
  outDebugString(L"handleLeftButtonDown:FormMenuChoice [{}]\n", FormMenuChoice);

  return true;
}

void thred::updateBackground() {
  BackgroundColor = 0x505050; // darker DimGrey NOLINT(readability-magic-numbers)
  thred::nuPen(BackgroundPen, 1, BackgroundColor);
  BackgroundPenWidth = 1;
  DeleteObject(BackgroundBrush);
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  if (bitmap::ismap()) {
	bitmap::bfil(BackgroundColor);
  }
}

auto thi::handleNumericInput(wchar_t const& code, bool& retflag) -> bool {
  retflag = true;
  if (StateMap->test(StateFlag::SCLPSPAC) && (code == VK_OEM_MINUS || code == VK_SUBTRACT)) {
	if (MsgBuffer->front() != '-') {
	  thred::resetMsgBuffer();
	  MsgBuffer->back() = '-';
	  MsgBuffer->push_back(0);
	  SetWindowText(GeneralNumberInputBox, MsgBuffer->data());
	}
	return true;
  }
  if (dunum(code)) {
	if (StateMap->test(StateFlag::TRNIN0)) {
	  trace::traceNumberInput(NumericCode);
	}
	else {
	  MsgBuffer->back() = NumericCode;
	  MsgBuffer->push_back(0);
	  SetWindowText(GeneralNumberInputBox, MsgBuffer->data());
	}
	return true;
  }
  switch (code) {
	case VK_DECIMAL:      // numpad period
	case VK_OEM_PERIOD: { // period
	  // ToDo - only allow entry if there is not already a period in the buffer
	  MsgBuffer->back() = '.';
	  MsgBuffer->push_back(0);
	  SetWindowText(GeneralNumberInputBox, MsgBuffer->data());
	  return true;
	}
	case VK_BACK: { // backspace
	  if (StateMap->test(StateFlag::TRNIN0)) {
		trace::traceNumberReset();
	  }
	  if (MsgBuffer->size() > 1) {
		MsgBuffer->pop_back();
		MsgBuffer->back() = 0;
		SetWindowText(GeneralNumberInputBox, MsgBuffer->data());
	  }
	  return true;
	}
	case VK_RETURN: {
	  if (StateMap->test(StateFlag::TRNIN0)) {
		trace::dutrnum2();
	  }
	  else {
		if (StateMap->test(StateFlag::TRNIN1)) {
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

auto thi::chkMsg(std::vector<POINT>& stretchBoxLine, float& xyRatio, float& angle, F_POINT& rotationCenter, FRM_HEAD& textureForm)
    -> bool {
  if (Msg.message == WM_MOUSEMOVE) {
	return mouse::handleMouseMove(stretchBoxLine, xyRatio, angle, rotationCenter, textureForm);
  }
  if (Msg.message == WM_LBUTTONUP) {
	auto       retflag = true;
	auto const retval  = mouse::handleLeftButtonUp(xyRatio, angle, rotationCenter, retflag);
	if (retflag) {
	  return retval;
	}
  }
  if (Msg.message == WM_RBUTTONDOWN || Msg.message == WM_LBUTTONDOWN) {
	if (mouse::handleEitherButtonDown()) {
	  return true;
	}
  }
  if (Msg.message == WM_RBUTTONDOWN) {
	return mouse::handleRightButtonDown();
  }
  if (Msg.message == WM_LBUTTONDOWN) {
	{
	  auto retflag = true;
	  auto const retval = mouse::handleLeftButtonDown(stretchBoxLine, xyRatio, textureForm, retflag);
	  if (retflag) {
		return retval;
	  }
	}
  }
  switch (Msg.message) {
	case WM_TIMER: {
	  if (StateMap->test(StateFlag::RUNPAT) && (Msg.wParam == 0U)) {
		stchout();
	  }
	  break;
	}
	case WM_CHAR: {
	  if (iswgraph(gsl::narrow<wint_t>(Msg.wParam)) != 0) {
		nuthum(towlower(gsl::narrow<wint_t>(Msg.wParam)));
	  }
	  break;
	}
	case WM_KEYDOWN: {
	  auto const code = gsl::narrow<wchar_t>(Msg.wParam & 0xffffU);
	  if (StateMap->test(StateFlag::TXTRED)) {
		texture::txtkey(code, textureForm);
		return true;
	  }
	  // ToDo - value passed to duform is wierd because it is dependant on order of enumeration of the form types.
	  //        and value 'SAT' throws it off
	  if (StateMap->test(StateFlag::FORMIN)) {
		if (wrap::pressed(VK_CONTROL)) {
		  return true;
		}
		switch (code) {
		  case L'E': {
			StateMap->reset(StateFlag::FORMIN);
			thred::unmsg();
			form::duform(FRMLINE - 1);
			return true;
		  }
		  case L'F': {
			StateMap->reset(StateFlag::FORMIN);
			thred::unmsg();
			form::duform(FRMFPOLY - 1);
			return true;
		  }
		  case L'R': {
			form::duform(FRMRPOLY - 1);
			return true;
		  }
		  case L'S': {
			form::duform(FRMSTAR - 1);
			return true;
		  }
		  case L'A': {
			form::duform(FRMSPIRAL - 1);
			return true;
		  }
		  case L'H': {
			form::duform(FRMHEART - 2);
			return true;
		  }
		  case L'L': {
			form::duform(FRMLENS - 2);
			return true;
		  }
		  case L'G': {
			form::duform(FRMEGG - 2);
			return true;
		  }
		  case L'T': {
			form::duform(FRMTEAR - 2);
			return true;
		  }
		  case L'Z': {
			form::duform(FRMZIGZAG - 2);
			return true;
		  }
		  case L'W': {
			form::duform(FRMWAVE - 2);
			return true;
		  }
		  case L'D': {
			form::duform(FRMDAISY - 2);
			return true;
		  }
		  default: {
		  }
		}
	  }
	  if (StateMap->test(StateFlag::FILMSG)) {
		if (code == VK_RETURN || code == VK_OEM_3) { // check for return or back tick '`'
		  thred::savdo();
		  form::unfil();
		  thred::coltab();
		  thred::unmsg();
		  return true;
		}
	  }
	  if (StateMap->testAndReset(StateFlag::MOVMSG)) {
		if (code == VK_RETURN || code == VK_OEM_3) {
		  thred::savdo();
		  if (!SelectedFormList->empty()) {
			for (auto const selectedForm : (*SelectedFormList)) {
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
		if (StateMap->testAndReset(StateFlag::WASFRMFRM)) {
		  formForms::refrm();
		}
		return true;
	  }
	  if (StateMap->testAndReset(StateFlag::PRGMSG)) {
		if (code == VK_RETURN || code == VK_OEM_3) {
		  thred::deldir();
		  return true;
		}
	  }
	  if (StateMap->testAndReset(StateFlag::DELSFRMS)) {
		if (code == L'S' || code == VK_RETURN || code == VK_OEM_3) {
		  thred::savdo();
		  if (code == L'S') {
			StateMap->set(StateFlag::DELTO);
		  }
		  else {
			StateMap->reset(StateFlag::DELTO);
		  }
		  thred::delsfrms();
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		  thred::unmsg();
		  return true;
		}
	  }
	  if (StateMap->testAndReset(StateFlag::DELFRM)) {
		if (code == L'S' || code == VK_RETURN || code == VK_OEM_3) {
		  thred::savdo();
		  if (code == L'S') {
			StateMap->set(StateFlag::DELTO);
		  }
		  else {
			StateMap->reset(StateFlag::DELTO);
		  }
		  thred::frmdel();
		  fndknt();
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		  thred::unmsg();
		  return true;
		}
	  }
	  if (StateMap->test(StateFlag::THUMSHO)) {
		switch (code) {
		  case VK_ESCAPE:
		  case L'Q': {
			unthum();
			StateMap->reset(StateFlag::BAKSHO);
			thred::esccode();
			thred::qcode();
			break;
		  }
		  case VK_DOWN:
		  case VK_NEXT: { // page down
			nuthsel();
			break;
		  }
		  case VK_UP:
		  case VK_PRIOR: { // page up
			nuthbak(QUADRT + QUADRT);
			break;
		  }
		  case VK_HOME: {
			ThumbnailIndex = 0;
			nuthsel();
			break;
		  }
		  case VK_END: {
			ThumbnailIndex = wrap::toUnsigned(Thumbnails->size());
			nuthbak(QUADRT);
			break;
		  }
		  case VK_BACK: // backspace
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
	  if (StateMap->test(StateFlag::FSETFSPAC) || StateMap->test(StateFlag::GTWLKIND)) {
		// Check for keycode 'dash' and numpad 'subtract'
		if (code == VK_OEM_MINUS || code == VK_SUBTRACT) {
		  if (MsgBuffer->front() != '-') {
			thred::resetMsgBuffer();
			MsgBuffer->back() = '-';
			MsgBuffer->push_back(0);
			SetWindowText(GeneralNumberInputBox, MsgBuffer->data());
		  }
		  return true;
		}
	  }
	  if ((FormMenuChoice != 0U) || (PreferenceIndex != 0U)) {
		if (chkminus(code)) {
		  if (SideWindowEntryBuffer->front() != '-') {
			thred::resetSideBuffer();
			SideWindowEntryBuffer->operator[](SideWinMsgIdx++) = '-';
			SetWindowText(SideMessageWindow, SideWindowEntryBuffer->data());
		  }
		  return true;
		}
		if (dunum(code)) {
		  if (PreferenceIndex == PRFGRDCUT + 1 || PreferenceIndex == PRFSBXCUT + 1U) {
			auto buffer = std::array<wchar_t, 2> {};
			buffer[0]   = NumericCode;
			if (PreferenceIndex == PRFGRDCUT + 1U) {
			  ShowStitchThreshold = unthrsh(NumericCode - L'0');
			  SetWindowText(ValueWindow->operator[](PRFGRDCUT), buffer.data());
			}
			else {
			  StitchBoxesThreshold = unthrsh(NumericCode - L'0');
			  SetWindowText(ValueWindow->operator[](PRFSBXCUT), buffer.data());
			}
			thred::unsid();
		  }
		  else {
			if (SideWinMsgIdx < (SideWindowEntryBuffer->size() - 1U)) {
			  SideWindowEntryBuffer->operator[](SideWinMsgIdx++) = NumericCode;
			  SideWindowEntryBuffer->operator[](SideWinMsgIdx)   = 0;
			  SetWindowText(SideMessageWindow, SideWindowEntryBuffer->data());
			}
		  }
		  return true;
		}
		switch (code) {
		  case VK_DECIMAL:      // numpad period
		  case VK_OEM_PERIOD: { // period
			// ToDo - only allow entry if there is not already a period in the buffer
			if (SideWinMsgIdx < (SideWindowEntryBuffer->size() - 1U)) {
			  SideWindowEntryBuffer->operator[](SideWinMsgIdx++) = '.';
			  SideWindowEntryBuffer->operator[](SideWinMsgIdx)   = 0;
			  SetWindowText(SideMessageWindow, SideWindowEntryBuffer->data());
			}
			return true;
		  }
		  case VK_BACK: { // backspace
			if (SideWinMsgIdx != 0U) {
			  SideWindowEntryBuffer->operator[](--SideWinMsgIdx) = 0;
			  SetWindowText(SideMessageWindow, SideWindowEntryBuffer->data());
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
	  if (code == L'I') {
		thred::movi();
		LastKeyCode = L'I';
		return true;
	  }
	  if (code == L'Q' && LastKeyCode == L'Q') {
		thred::unpat();
	  }
	  LastKeyCode = code;
	  if (StateMap->test(StateFlag::NUMIN)) {
		{
		  auto       retflag = true;
		  auto const retval  = handleNumericInput(code, retflag);
		  if (retflag) {
			return retval;
		  }
		}
	  }
	  if (StateMap->testAndReset(StateFlag::ENTRDUP)) {
		if (MsgBuffer->size() > 1) {
		  outDebugString(
		      L"chknum: buffer length [{}] size [{}]\n", wcslen(MsgBuffer->data()), MsgBuffer->size());
		  auto const value = thred::getMsgBufferValue();
		  if (value != 0.0F) {
			IniFile.rotationAngle = value * DEGRADF;
		  }
		}
		if (IniFile.rotationAngle != 0.0F) {
		  form::dupfn(IniFile.rotationAngle);
		}
	  }
	  if (StateMap->testAndReset(StateFlag::ENTROT)) {
		if (MsgBuffer->size() > 1) {
		  outDebugString(
		      L"chknum: buffer length [{}] size [{}]\n", wcslen(MsgBuffer->data()), MsgBuffer->size());
		  auto const value = thred::getMsgBufferValue();
		  if (value != 0.0F) {
			IniFile.rotationAngle = value * DEGRADF;
		  }
		}
		rotfns(IniFile.rotationAngle);
	  }
	  thred::unmsg();
	  {
		auto       retflag = true;
		auto const retval  = keys::handleMainWinKeys(code, rotationCenter, stretchBoxLine, retflag);
		if (retflag) {
		  return retval;
		}
	  }
	  break;
	}
	case WM_COMMAND: {
	  thred::unmsg();
	  {
		auto previousName = PreviousNames->begin();
		for (auto const& iLRU : LRUMenuId) {
		  if (Msg.wParam == iLRU) {
			*WorkingFileName = *previousName;
			StateMap->set(StateFlag::REDOLD);
			nuFil(FileIndices::THR);
		  }
		  ++previousName;
		}
	  }
	  auto const wParameter = LOWORD(Msg.wParam);
	  if (wParameter >= ID_FILE_OPEN1 && wParameter <= ID_AUXPES) {
		thred::undat();
	  }
	  if (menu::handleMainMenu(wParameter, rotationCenter)) {
		break;
	  }
	  if (menu::handleEditMenu(wParameter)) {
		break;
	  }
	  if (menu::handleFileMenu(wParameter)) {
		break;
	  }
	  if (menu::handleFillMenu(wParameter)) {
		break;
	  }
	  if (menu::handleViewMenu(wParameter)) {
		break;
	  }
	  break;
	}
	default: {
	  // outDebugString(L"default hit in chkMsg: message [{}]\n", Msg.message);
	  break;
	}
  }
  return false;
}

// return the width of a text item
auto thred::txtWid(wchar_t const* string) noexcept(std::is_same_v<size_t, uint32_t>) -> SIZE {
  auto textSize = SIZE {};
  wrap::getTextExtentPoint32(ThredDC, string, wrap::toUnsigned(wcslen(string)), &textSize);
  return textSize;
}

void thi::makCol() noexcept {
  auto buffer = std::array<wchar_t, 3> {};
  buffer[1]   = L'0';
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const hFont        = displayText::getThrEdFont(FONTSIZE);
  auto       dcw          = DefaultColorWin->begin();
  auto       ucw          = UserColorWin->begin();
  auto       itThreadSize = ThreadSize.begin();
  auto       yOffset      = int32_t {};

  for (auto& tsw : *ThreadSizeWin) {
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
	displayText::setWindowFont(*(dcw++), hFont);
	*(ucw++)  = CreateWindow(L"STATIC",
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
	buffer[0] = *(itThreadSize++);
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

void thi::ritloc() {
  auto lockFilePath = fs::path {};
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto       ppszPath = PWSTR {nullptr}; // variable to receive the path memory block pointer.
  auto const hResult  = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, nullptr, &ppszPath);
  if (FAILED(hResult)) {
	CoTaskMemFree(ppszPath); // free up the path memory block
	return;
  }
  lockFilePath.assign(ppszPath); // make a local copy of the path
  CoTaskMemFree(ppszPath);       // free up the path memory block
  lockFilePath /= L"ThrEd";
  fs::create_directory(lockFilePath);
  lockFilePath /= L"thredloc.txt";
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const lockFile = CreateFile(lockFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (lockFile != INVALID_HANDLE_VALUE) {
	auto       bytesWritten = DWORD {};
	auto const value        = utf::utf16ToUtf8(*HomeDirectory);
	wrap::writeFile(lockFile, value.data(), wrap::toUnsigned(value.size()) + 1U, &bytesWritten, nullptr);
	CloseHandle(lockFile);
  }
}

void thi::duhom() {
  auto const arg0 = fs::path {*ArgList};
  *HomeDirectory  = arg0.parent_path();
}

auto thred::getHomeDir() noexcept -> fs::path* {
  return HomeDirectory;
}

auto thred::getBackGroundBrush() noexcept -> HBRUSH {
  return BackgroundBrush;
}

void thi::ducmd() {
  if (ArgCount <= 1) {
	return;
  }
  auto const spArgList = gsl::span<LPTSTR> {ArgList, wrap::toSize(ArgCount)};
  auto const arg1      = std::wstring {spArgList[1]};
  if (arg1.compare(0, 4, L"/F1:") != 0) {
	WorkingFileName->assign(arg1);
	StateMap->set(StateFlag::REDOLD);
	nuFil(FileIndices::THR);
	return;
  }
  auto balaradFileName = *HomeDirectory / arg1.substr(4);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto balaradFile =
      CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (balaradFile == INVALID_HANDLE_VALUE) {
	return;
  }
  CloseHandle(balaradFile);
  gsl::not_null<fs::path*> const balaradName0 = bal::getBN0();
  *balaradName0                               = balaradFileName;
  if (ArgCount <= 2) {
	return;
  }
  auto const arg2 = std::wstring {spArgList[2]};
  if (arg2.compare(0, 4, L"/F2:") != 0) {
	return;
  }
  balaradFileName = *HomeDirectory / arg2.substr(4);
  balaradFile =
      CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (balaradFile == INVALID_HANDLE_VALUE) {
	return;
  }
  auto readBuffer = std::vector<char> {};
  readBuffer.resize(MAX_PATH + 1);
  gsl::not_null<fs::path*> const balaradName1 = bal::getBN1();
  *balaradName1                               = balaradFileName;
  auto bytesRead                              = DWORD {};
  if (!wrap::readFile(balaradFile, readBuffer.data(), readBuffer.size(), &bytesRead, L"ReadFile for readBuffer in ducmd")) {
	return;
  }
  if (bytesRead != 0U) {
	readBuffer.resize(bytesRead);
	gsl::not_null<fs::path*> const balaradName2 = bal::getBN2();
	balaradName2->assign(readBuffer.data());
	bal::redbal();
  }
  CloseHandle(balaradFile);
  fs::remove(*balaradName1);
}

void thi::setPrefs() {
  thred::defpref();
  getDocsFolder(*DefaultDirectory);
  if (DesignerName->empty()) {
	DesignerName->assign(displayText::loadStr(IDS_UNAM));
	thred::getdes();
  }
}

void thi::loadColors() noexcept {
  auto const spISC  = gsl::span {IniFile.stitchColors};
  auto       iISC   = spISC.begin();
  auto const spISPC = gsl::span {IniFile.stitchPreferredColors};
  auto       iISPC  = spISPC.begin();
  auto const spIBPC = gsl::span {IniFile.backgroundPreferredColors};
  auto       iIBPC  = spIBPC.begin();
  auto       iCC    = CustomColor.begin();
  auto       iCBC   = CustomBackgroundColor.begin();
  for (auto& color : UserColor) {
	color     = *(iISC++);
	*(iCC++)  = *(iISPC++);
	*(iCBC++) = *(iIBPC++);
  }
}

void thi::redini() {
  duhom();
  *IniFileName = *HomeDirectory;
  *IniFileName /= L"thred.ini";
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const iniFileHandle =
      CreateFile(IniFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (iniFileHandle == INVALID_HANDLE_VALUE) {
	setPrefs();
  }
  else {
	auto bytesRead = DWORD {};
	if (!wrap::readFile(iniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, L"ReadFile for IniFile in redini")) {
	  return;
	}
	CloseHandle(iniFileHandle);
	if (bytesRead < sizeof(IniFile)) {
	  auto newFileName = *IniFileName; // intentional copy
	  newFileName.replace_filename("thred-ini.bak");
	  fs::rename(*IniFileName, newFileName);
	  setPrefs();
	}
	else {
	  auto const directory = utf::utf8ToUtf16(std::string(IniFile.defaultDirectory.data()));
	  DefaultDirectory->assign(directory);
	  {
		auto previousName = PreviousNames->begin();
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
	  DesignerName->assign(utf::utf8ToUtf16(std::string(IniFile.designerName.data())));
	  thi::loadColors();
	  bitmap::setBmpBackColor();
	  BackgroundColor = IniFile.backgroundColor;
	  bitmap::setBitmapColor(IniFile.bitmapColor);
	  MinStitchLength = IniFile.minStitchLength;
	  if (IniFile.showStitchThreshold < 0) {
		IniFile.showStitchThreshold = 0;
	  }
	  constexpr auto SSTCLAMP = 9.0F; // clamp the show stitch threshold to this value
	  if (IniFile.showStitchThreshold > SSTCLAMP) {
		IniFile.showStitchThreshold = SSTCLAMP;
	  }
	  ShowStitchThreshold = IniFile.showStitchThreshold;
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
		auto const tmp = ENUM_MAP<UserFlag>(IniFile.userFlagMap);
		*UserFlagMap   = tmp;
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
	  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
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

void thi::chkirct() noexcept {
  auto const screenLimits = SIZE {ScreenSizePixels.cx - 1, ScreenSizePixels.cy - 1};
  if (IniFile.initialWindowCoords.left > screenLimits.cx) {
	IniFile.initialWindowCoords.left = screenLimits.cx;
  }
  if (IniFile.initialWindowCoords.right > screenLimits.cx) {
	IniFile.initialWindowCoords.right = screenLimits.cx;
  }
  if (IniFile.initialWindowCoords.left < 0) {
	IniFile.initialWindowCoords.left = 0;
  }
  if (IniFile.initialWindowCoords.right < 0) {
	IniFile.initialWindowCoords.right = 0;
  }
  if (IniFile.initialWindowCoords.top > screenLimits.cy) {
	IniFile.initialWindowCoords.top = screenLimits.cy;
  }
  if (IniFile.initialWindowCoords.bottom > screenLimits.cy) {
	IniFile.initialWindowCoords.bottom = screenLimits.cy;
  }
  if (IniFile.initialWindowCoords.top < 0) {
	IniFile.initialWindowCoords.top = 0;
  }
  if (IniFile.initialWindowCoords.bottom < 0) {
	IniFile.initialWindowCoords.bottom = 0;
  }
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

void thi::createBrushes() noexcept {
  auto dcb         = DefaultColorBrush.begin();
  auto ucb         = UserColorBrush.begin();
  auto itUserColor = UserColor.begin();
  for (auto const& color : DEFAULT_COLORS) {
	*(dcb++) = CreateSolidBrush(color);
	*(ucb++) = CreateSolidBrush(*(itUserColor++));
  }
}

void thi::init() {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const deviceContext   = GetDC(nullptr);
  auto const screenHalfWidth = (GetDeviceCaps(deviceContext, HORZRES)) / 2;
  ReleaseDC(nullptr, deviceContext);
  TexturePointsBuffer->clear();
  LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1));
  MainMenu   = GetMenu(ThrEdWindow);
  auto wRect = RECT {};
  GetWindowRect(ThrEdWindow, &wRect);
  auto mRect = RECT {};
  GetMenuItemRect(ThrEdWindow, MainMenu, 0, &mRect);
  wRect.left  = mRect.left;
  wRect.right = mRect.right;
  for (auto iMenu = 0U; iMenu <= M_HELP; ++iMenu) {
	GetMenuItemRect(ThrEdWindow, MainMenu, iMenu, &mRect);
	wRect.right += (mRect.right - mRect.left);
  }
  constexpr auto MENUWS = LONG {20}; // menu righthand whitespace
  wRect.right += MENUWS;
  auto const sWidth = wRect.right - wRect.left;
  if (sWidth < screenHalfWidth) {
	wRect.right = wRect.left += screenHalfWidth;
  }
  menu::init();
  menu::qchk();
  mouse::crtcurs();
  menu::redfils(LRUPtr, PreviousNames);
  StateMap->reset(); // clear the bitmap
  // set up the size variables
  ThredDC = GetDC(ThrEdWindow);
  SetStretchBltMode(ThredDC, COLORONCOLOR);
  StitchWindowDC    = GetDCEx(MainStitchWin, nullptr, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
  StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
  chkirct();
  if (!UserFlagMap->test(UserFlag::SAVMAX)) {
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
  ButtonWin->resize(BTNCOUNT);
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
	ButtonWin->operator[](iButton) = CreateWindow(L"STATIC",
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
  static constexpr auto BOX_COLOR = std::array<COLORREF, 4> {0x404040, 0x408040, 0x804040, 0x404080};
  auto itBoxCOlor = BOX_COLOR.begin();
  std::ranges::generate(BoxPen, [&itBoxCOlor]() mutable noexcept -> HPEN {
	return wrap::createPen(PS_SOLID, PENNWID, *(itBoxCOlor++));
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
  auto itUserPen     = UserPen->begin();
  for (auto const& color : UserColor) {
	*(tsp++)       = 1;
	*(tsi++)       = 1;
	*(itUserPen++) = wrap::createPen(PS_SOLID, PENNWID, color);
  }
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  // create brushes
  createBrushes();
  ZoomFactor = 1;
  StitchBuffer->clear();
  GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
  menu::ladj();
  auto const pikol = displayText::loadStr(IDS_PIKOL);
  wrap::getTextExtentPoint32(ThredDC, pikol.c_str(), wrap::toUnsigned(pikol.size()), &PickColorMsgSize);
  menu::auxmen();
  fnamtabs();
  ritfnam(*DesignerName);
  auto       designer       = utf::utf16ToUtf8(*DesignerName);
  auto const spModifierName = gsl::span {ExtendedHeader->modifierName};
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
  StateMap->set(StateFlag::HIDMAP);
  thred::hidbit();
  menu::chkmen();
  // check command line-should be last item in init
  ducmd();
  auto const fmtStr = displayText::format(IDS_THRED, *DesignerName);
  SetWindowText(ThrEdWindow, fmtStr.c_str());
}

auto thi::defTxt(uint32_t iColor) -> COLORREF {
  // bitmap for color number. Black or white bit chosen for contrast against the default background colors
  constexpr auto TEXT_COLOR_MAP = std::bitset<16>(0xbaf);
  return TEXT_COLOR_MAP.test(iColor) ? PENWHITE : PENBLK;
}

void thi::relin() {
  MoveLine0[0] = thred::stch2px1(ClosestPointIndex - 1);
  MoveLine1[1] = thred::stch2px1(ClosestPointIndex + 1U);
  StateMap->reset(StateFlag::WASLIN);
  thi::dulin(MoveLine0, MoveLine1);
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
void thi::dumov() {
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

  auto const sCurr = wrap::next(StitchBuffer->begin(), keys::getMoveAnchor());
  if (sCurr->x < ZoomRect.left || sCurr->x > ZoomRect.right || sCurr->y < ZoomRect.bottom ||
      sCurr->y > ZoomRect.top) {
	return;
  }
  auto const sNext          = std::next(sCurr);
  RotateAngle               = std::atan2f(sNext->y - sCurr->y, sNext->x - sCurr->x);
  auto       arrowBox       = std::array<POINT, ABPOINTS> {};
  auto const abCenterPixels = sdCor2px(StitchBuffer->operator[](keys::getMoveAnchor()));
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

auto thi::chkup(uint32_t count, size_t iStitch) -> uint32_t {
  if (!StateMap->test(StateFlag::UPTO) || (ClosestPointIndex == 0U)) {
	return count;
  }
  if (ColorChangeTable->operator[](iStitch).stitchIndex >= ClosestPointIndex) {
	return 0U;
  }
  if (ColorChangeTable->operator[](iStitch + 1U).stitchIndex >= ClosestPointIndex) {
	return ClosestPointIndex - ColorChangeTable->operator[](iStitch).stitchIndex + 1U;
  }
  return count;
}

void thi::stCor2px(F_POINT_ATTR const& stitch, POINT& point) {
  point = {wrap::ceil<int32_t>((stitch.x - ZoomRect.left) * ZoomRatio.x),
           wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                               (stitch.y - ZoomRect.bottom) * ZoomRatio.y)};
}

void thi::drwknot() {
  if (UserFlagMap->test(UserFlag::KNOTOF) || Knots->empty() || StitchBuffer->empty()) {
	return;
  }
  constexpr auto KBOFFSET = 5; // offset of the knot box sides;
  auto const     kOffset  = MulDiv(KBOFFSET, *ScreenDPI, STDDPI);
  constexpr auto KLINELEN = 10; // length of the knot line;
  auto const     kLine    = MulDiv(KLINELEN, *ScreenDPI, STDDPI);
  auto           point    = POINT {};
  auto           kOutline = std::array<POINT, SQPNTS> {};
  auto           tLine    = std::array<POINT, LNPNTS> {};
  for (auto const knot : *Knots) {
	stCor2px(StitchBuffer->operator[](knot), point);
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

void thi::dugrid() {
  if (ZoomFactor >= ShowStitchThreshold && (ShowStitchThreshold != 0.0F)) {
	return;
  }
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  SelectObject(StitchWindowMemDC, GridPen);
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
	                        (wrap::toFloat(iGrid) * IniFile.gridSize - ZoomRect.bottom) * ZoomRatio.y);
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

auto thi::setRmap(boost::dynamic_bitset<>& stitchMap, F_POINT_ATTR const& stitchPoint, F_POINT const& cellSize)
    -> bool {
  auto const bitPoint = gsl::narrow<uint32_t>(floor((stitchPoint.x - ZoomRect.left) / cellSize.x) *
                                              floor((stitchPoint.y - ZoomRect.bottom) / cellSize.y));
  return !stitchMap.test_set(bitPoint);
}

void thi::drawBackground() {
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
  thred::duzrat();
  if (bitmap::ismap() && !StateMap->test(StateFlag::HIDMAP) && !StateMap->test(StateFlag::UPTO)) {
	bitmap::drawBmpBackground();
  }
  dugrid();
}

auto thi::getMaxCount() -> uint32_t {
  auto maxCount = uint32_t {0U};
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	auto const deltaCount = gsl::narrow<uint32_t>(ColorChangeTable->operator[](iColor + 1U).stitchIndex -
	                                              ColorChangeTable->operator[](iColor).stitchIndex);
	if (deltaCount > maxCount) {
	  maxCount = deltaCount;
	}
  }
  return maxCount;
}

void thi::drawOffScreenLine(const F_POINT_ATTR& iStitch, const F_POINT_ATTR& prevStitch, const float& maxYcoord) {
  // write an equation for this line
  auto const xDelta     = iStitch.x - prevStitch.x;
  auto const yDelta     = prevStitch.y - iStitch.y;
  auto const slope      = xDelta / yDelta;
  auto const offset     = iStitch.x + slope * iStitch.y;
  auto       stitchLine = std::array<POINT, LNPNTS> {};
  // does the line intersect with the top of the screen?
  auto gapToEdge = offset - slope * ZoomRect.top;
  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
	stitchLine[0] =
	    POINT {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
	           wrap::ceil<int32_t>(maxYcoord - (prevStitch.y - ZoomRect.bottom) * ZoomRatio.x)};
	stitchLine[1] = POINT {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
	                       wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.x)};
	wrap::polyline(StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
	return;
  }
  // does the line intersect the bottom of the screen?
  gapToEdge = offset - slope * ZoomRect.bottom;
  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
	stitchLine[0] =
	    POINT {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
	           wrap::ceil<int32_t>(maxYcoord - (prevStitch.y - ZoomRect.bottom) * ZoomRatio.y)};
	stitchLine[1] = POINT {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
	                       wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.y)};
	wrap::polyline(StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
	return;
  }
  // does the line intersect the left side of the screen?
  if (slope != 0.0F) {
	gapToEdge = (offset - ZoomRect.left) / slope;
	if (gapToEdge >= ZoomRect.bottom && gapToEdge <= ZoomRect.top) {
	  stitchLine[0] =
	      POINT {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
	             wrap::ceil<int32_t>(maxYcoord - (prevStitch.y - ZoomRect.bottom) * ZoomRatio.y)};
	  stitchLine[1] =
	      POINT {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
	             wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.y)};
	  wrap::polyline(StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
	}
  }
}

void thi::doInitZoomed() {
  if (StitchBuffer->empty()) {
	return;
  }
  auto stitchCount = getMaxCount();
  auto linePoints  = std::vector<POINT> {};
  linePoints.reserve(wrap::toSize(stitchCount) + 2U);
  StateMap->reset(StateFlag::LINED);
  StateMap->reset(StateFlag::LININ);
  // intentional copy of first stitch to initialize previous stitch
  auto prevStitch = StitchBuffer->front();
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	if (StateMap->test(StateFlag::HID) && (ColorChangeTable->operator[](iColor).colorIndex != ActiveColor)) {
	  stitchCount = ColorChangeTable->operator[](iColor + 1U).stitchIndex -
	                ColorChangeTable->operator[](iColor).stitchIndex;
	  auto const sStart = wrap::next(StitchBuffer->begin(), ColorChangeTable->operator[](iColor).stitchIndex);
	  auto const sEnd   = wrap::next(sStart, stitchCount);
	  auto       sRange = std::ranges::subrange(sStart, sEnd);
	  for (auto const& iStitch : sRange) {
		if (iStitch.x >= ZoomRect.left && iStitch.x <= ZoomRect.right &&
		    iStitch.y >= ZoomRect.bottom && iStitch.y <= ZoomRect.top) {
		  DisplayedColorBitmap.set(ColorChangeTable->operator[](iColor).colorIndex);
		  break;
		}
	  }
	  continue;
	}
	auto wascol = 0U;
	SelectObject(StitchWindowMemDC, UserPen->operator[](ColorChangeTable->operator[](iColor).colorIndex));
	stitchCount = ColorChangeTable->operator[](iColor + 1U).stitchIndex -
	              ColorChangeTable->operator[](iColor).stitchIndex;
	auto const stIndex = ColorChangeTable->operator[](iColor).stitchIndex;

	auto const stStart   = wrap::next(StitchBuffer->begin(), stIndex);
	stitchCount          = chkup(stitchCount, iColor);
	auto const stitches  = std::ranges::subrange(stStart, wrap::next(stStart, stitchCount));
	auto const maxYcoord = wrap::toFloat(DrawItem->rcItem.bottom);
	for (auto& iStitch : stitches) {
	  auto const layer = (iStitch.attribute & LAYMSK) >> LAYSHFT;
	  if ((ActiveLayer != 0U) && (layer != 0U) && (layer != ActiveLayer)) {
		continue;
	  }
	  if (iStitch.x >= ZoomRect.left && iStitch.x <= ZoomRect.right &&
	      iStitch.y >= ZoomRect.bottom && iStitch.y <= ZoomRect.top) {
		wascol = 1;
		if (StateMap->testAndSet(StateFlag::LINED)) {
		  if (StateMap->testAndSet(StateFlag::LININ)) {
			linePoints.push_back(
			    {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			     wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.y)});
		  }
		  else {
			if (iStitch == prevStitch) {
			  linePoints.push_back(
			      {wrap::ceil<int32_t>((stStart->x - ZoomRect.left) * ZoomRatio.x),
			       wrap::ceil<int32_t>(maxYcoord - (stStart->y - ZoomRect.bottom) * ZoomRatio.y)});
			}
			else {
			  linePoints.push_back(
			      {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
			       wrap::ceil<int32_t>(maxYcoord - (prevStitch.y - ZoomRect.bottom) * ZoomRatio.y)});
			  linePoints.push_back(
			      {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			       wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.y)});
			}
		  }
		}
		else {
		  if (iStitch == prevStitch) {
			linePoints.push_back(
			    {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			     wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.y)});
		  }
		  else {
			linePoints.push_back(
			    {wrap::ceil<int32_t>((prevStitch.x - ZoomRect.left) * ZoomRatio.x),
			     wrap::ceil<int32_t>(maxYcoord - (prevStitch.y - ZoomRect.bottom) * ZoomRatio.y)});
			linePoints.push_back(
			    {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
			     wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.y)});
		  }
		  StateMap->set(StateFlag::LININ);
		}
	  }
	  else {
		if (StateMap->testAndReset(StateFlag::LININ)) {
		  linePoints.push_back(
		      {wrap::ceil<int32_t>((iStitch.x - ZoomRect.left) * ZoomRatio.x),
		       wrap::ceil<int32_t>(maxYcoord - (iStitch.y - ZoomRect.bottom) * ZoomRatio.y)});
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
	if (!linePoints.empty()) {
	  wrap::polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
	  // intentional copy being made here to save last point
	  auto const lastPoint = linePoints.back();
	  linePoints.clear();
	  linePoints.push_back(lastPoint);
	}
	if (wascol != 0U) {
	  DisplayedColorBitmap.set(ColorChangeTable->operator[](iColor).colorIndex);
	}
  }
}

void thi::doInitUnzoomed() {
  auto linePoints = std::vector<POINT> {};
  linePoints.reserve(wrap::toSize(thi::getMaxCount()) + 2U);
  auto const pwid = StateMap->test(StateFlag::HID);
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	DisplayedColorBitmap.set(ColorChangeTable->operator[](iColor).colorIndex);
	auto stitchCount = wrap::toUnsigned(ColorChangeTable->operator[](iColor + 1U).stitchIndex) -
	                   ColorChangeTable->operator[](iColor).stitchIndex;
	stitchCount = chkup(stitchCount, iColor);
	if (!pwid || ColorChangeTable->operator[](iColor).colorIndex == ActiveColor) {
	  drwLin(linePoints,
	         ColorChangeTable->operator[](iColor).stitchIndex,
	         stitchCount,
	         UserPen->operator[](ColorChangeTable->operator[](iColor).colorIndex));
	}
  }
}

void thi::doDrwInit() {
  if (StateMap->test(StateFlag::ZUMED)) {
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
	ShowWindow(HorizontalScrollBar, SW_SHOWNORMAL);
	ShowWindow(VerticalScrollBar, SW_SHOWNORMAL);
  }
  else {
	ShowWindow(VerticalScrollBar, SW_HIDE);
	ShowWindow(HorizontalScrollBar, SW_HIDE);
  }
  thred::duzrat();
  auto const dub6        = ZoomRatio.x * 6.0F;
  auto const threadWidth = std::array<int32_t, 3> {
      std::lround(dub6 * TSIZ30), std::lround(dub6 * TSIZ40), std::lround(dub6 * TSIZ60)}; // thread sizes in pixels
  auto tsi = ThreadSizeIndex.begin();
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	auto width = int32_t {1};
	if (StateMap->test(StateFlag::THRDS) || ((iColor == ActiveColor) && StateMap->test(StateFlag::COL))) {
	  width = *(wrap::next(threadWidth.begin(), *tsi));
	}
	nuStchSiz(iColor, width);
	++tsi;
  }
  DisplayedColorBitmap.reset();
  if (StateMap->test(StateFlag::ZUMED)) {
	thi::doInitZoomed();
  }
  else {
	thi::doInitUnzoomed();
  }
  if (StateMap->test(StateFlag::SELBOX)) {
	if (!StitchBuffer->empty()) {
	  ritcor(StitchBuffer->operator[](ClosestPointIndex));
	  auto stitchCoordsInPixels = POINT {};
	  if (thred::stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
		thred::dubox(stitchCoordsInPixels);
	  }
	}
  }
  if (StateMap->test(StateFlag::FRMPSEL)) {
	auto const itVertex =
	    wrap::next(FormVertices->cbegin(),
	               FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	thred::ritfcor(*itVertex);
  }
  if (!StateMap->test(StateFlag::SELBOX) && !StateMap->test(StateFlag::FRMPSEL)) {
	auto const blank = std::wstring {};
	displayText::butxt(HCOR, blank);
  }
  if (StateMap->test(StateFlag::WASLIN)) {
	relin();
  }
  if (StateMap->test(StateFlag::GRPSEL)) {
	if (cmpstch(ClosestPointIndex, GroupStitchIndex)) {
	  cros(ClosestPointIndex);
	}
	else {
	  SearchLine->clear();
	  ducros(StitchWindowMemDC);
	}
	thred::selRct(StitchRangeRect);
	nuselrct();
	StateMap->set(StateFlag::SELSHO);
	dusel(StitchWindowMemDC);
  }
  if (ZoomFactor < StitchBoxesThreshold) {
	auto const cellSize =
	    F_POINT {(ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right),
	             (ZoomRect.top - ZoomRect.bottom) / wrap::toFloat(StitchWindowClientRect.bottom)};
	auto const maxMapSize = wrap::toSize(StitchWindowClientRect.right * StitchWindowClientRect.bottom);
	auto stitchMap = boost::dynamic_bitset<>(maxMapSize, 0U);
	SelectObject(StitchWindowMemDC, LinePen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	if (StateMap->test(StateFlag::HID)) {
	  for (auto iColor = 0U; iColor < thred::maxColor(); ++iColor) {
		if (ColorChangeTable->operator[](iColor).colorIndex != ActiveColor) {
		  continue;
		}
		for (auto iStitch = ColorChangeTable->operator[](iColor).stitchIndex;
		     iStitch < ColorChangeTable->operator[](gsl::narrow_cast<size_t>(iColor) + 1U).stitchIndex;
		     ++iStitch) {
		  auto const& stitch = StitchBuffer->operator[](iStitch);
		  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right && stitch.y >= ZoomRect.bottom &&
		      stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch, cellSize)) {
			thi::stchbox(iStitch, StitchWindowMemDC);
		  }
		}
	  }
	}
	else {
	  auto iStitch = 0U;
	  for (auto& stitch : *StitchBuffer) {
		if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right && stitch.y >= ZoomRect.bottom &&
		    stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch, cellSize)) {
		  thi::stchbox(iStitch, StitchWindowMemDC);
		}
		++iStitch;
	  }
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
  if (StateMap->test(StateFlag::CLPSHO)) {
	thi::duclp();
  }
}

void thi::drwStch() {
  StateMap->set(StateFlag::RELAYR);
  StateMap->reset(StateFlag::SELSHO);
  StateMap->reset(StateFlag::ILIN1);
  StateMap->reset(StateFlag::BZUM);
  unboxs();
  thred::unrotu();
  thred::unrot();
  StateMap->reset(StateFlag::ILIN);
  uncros();
  StateMap->reset(StateFlag::SHOFRM);
  thi::drawBackground();
  if (StateMap->test(StateFlag::INIT)) {
	thi::doDrwInit();
  }
  if (StateMap->test(StateFlag::ROTAT) || StateMap->test(StateFlag::ROTCAPT) ||
      StateMap->test(StateFlag::MOVCNTR)) {
	thred::ritrot(0, form::rotpar());
  }
  if (!FormList->empty() && !StateMap->test(StateFlag::FRMOF)) {
	form::drwfrm();
  }
  if (StateMap->test(StateFlag::INSFRM)) {
	form::rinfrm();
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	form::dubig();
  }
  if (StateMap->test(StateFlag::MOVFRM)) {
	StateMap->set(StateFlag::SHOFRM);
	form::dufrm();
  }
  if (StateMap->test(StateFlag::SATPNT)) {
	satin::satzum();
  }
  if (StateMap->test(StateFlag::GMRK)) {
	thi::drwmrk(StitchWindowMemDC);
  }
  if (StateMap->test(StateFlag::PRFACT)) {
	thred::redraw(PreferencesWindow);
  }
  if (StateMap->test(StateFlag::SELBOX)) {
	displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
  }
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  if (StateMap->test(StateFlag::INIT)) {
	thred::lenCalc();
  }
  if (StateMap->test(StateFlag::MOVSET)) {
	dumov();
  }
  drwknot();
}

void thi::dubar() {
  auto colorBarRect = RECT {DrawItem->rcItem.left, 0, DrawItem->rcItem.right, DrawItem->rcItem.bottom};
  auto       indicatorLine = std::array<POINT, 2> {};
  auto const buffSize      = wrap::toFloat(StitchBuffer->size());
  for (auto iColorChange = size_t {}; iColorChange < thred::maxColor(); ++iColorChange) {
	auto const barSectionHeight =
	    wrap::toFloat(ColorChangeTable->operator[](iColorChange + 1U).stitchIndex) / buffSize;
	colorBarRect.bottom = std::lround(barSectionHeight * wrap::toFloat(DrawItem->rcItem.bottom));
	auto ucb = wrap::next(UserColorBrush.begin(), ColorChangeTable->operator[](iColorChange).colorIndex);
	FillRect(DrawItem->hDC, &colorBarRect, *ucb);
	colorBarRect.top = colorBarRect.bottom;
  }
  if (!StateMap->test(StateFlag::SELBOX) && !StateMap->test(StateFlag::GRPSEL)) {
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
  if (StateMap->test(StateFlag::GRPSEL)) {
	selectedIndicator  = wrap::toFloat(GroupStitchIndex) / buffSize;
	indicatorLine[0].y = indicatorLine[1].y =
	    wrap::ceil<decltype(indicatorLine[0].y)>(wrap::toFloat(colorBarRect.bottom) * selectedIndicator);
	indicatorLine[0].x = colorBarRect.left;
	indicatorLine[1].x = colorBarRect.right;
	wrap::polyline(DrawItem->hDC, indicatorLine.data(), wrap::toUnsigned(indicatorLine.size()));
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thi::ritbak(fs::path const& fileName, DRAWITEMSTRUCT const& drawItem) {
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
  auto const ratio                  = (xRatio < yRatio) ? xRatio : yRatio;
  if (stitchHeader.stitchCount != 0U) {
	auto stitchesToDraw = std::vector<F_POINT_ATTR> {};
	stitchesToDraw.resize(stitchHeader.stitchCount);
	auto lines = std::vector<POINT> {};
	lines.resize(stitchHeader.stitchCount);
	auto const bytesToRead = stitchHeader.stitchCount * wrap::sizeofType(StitchBuffer);
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
	  lines[iLine++] = {
	      std::lround(stitchesToDraw[iStitch].x * ratio),
	      std::lround(wrap::toFloat(drawingDestinationSize.cy) - stitchesToDraw[iStitch].y * ratio)};
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
	constexpr auto SKIP1 = LONG {16 + sizeof(COLORREF) + sizeof(COLORREF) * COLORCNT};
	SetFilePointer(thrEdFile, SKIP1, nullptr, FILE_CURRENT);
  }
  if (stitchHeader.formCount == 0U) {
	CloseHandle(thrEdFile);
	return;
  }
  // skip over custom colors (64 bytes)  and thread sizes (16 bytes) to forms
  constexpr auto SKIP2 = LONG {sizeof(COLORREF) * COLORCNT + 16};
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
	if (formList[iForm].vertexCount > maxLines) {
	  maxLines = formList[iForm].vertexCount;
	}
  }
  lines.resize(wrap::toSize(maxLines) + 1U);
  auto iVertex = 0U;
  for (auto iForm = 0U; iForm < stitchHeader.formCount; ++iForm) {
	auto const iLine = iVertex;
	for (auto iVertexInForm = 0U;
	     (iVertexInForm < formList[iForm].vertexCount) && (iVertex < stitchHeader.vertexCount);
	     ++iVertexInForm) {
	  lines[iVertexInForm] = {
	      std::lround(vertexList[iVertex].x * ratio),
	      std::lround(wrap::toFloat(drawingDestinationSize.cy) - vertexList[iVertex++].y * ratio)};
	}
	lines[formList[iForm].vertexCount] = {
	    std::lround(vertexList[iLine].x * ratio),
	    std::lround(wrap::toFloat(drawingDestinationSize.cy) - vertexList[iLine].y * ratio)};
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

struct createParams {
  BOOL bEnableNonClientDpiScaling;
};

auto CALLBACK thi::wndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
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
	  if (StateMap->testAndReset(StateFlag::PRFACT)) {
		DestroyWindow(PreferencesWindow);
		StateMap->reset(StateFlag::WASRT);
	  }
	  thred::unmsg();
	  thred::undat();
	  StateMap->reset(StateFlag::FORMIN);
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	case WM_HSCROLL: {
	  constexpr auto SPEDLIN = int32_t {30};  // speed change for line message on speed scroll bar
	  constexpr auto SPEDPAG = int32_t {120}; // speed change for page message on speed scroll bar
	  switch (gsl::narrow<int32_t>(LOWORD(wParam))) {
		case SB_LINELEFT: {
		  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
			MovieTimeStep += SPEDLIN;
			if (MovieTimeStep > MAXDELAY) {
			  MovieTimeStep = MAXDELAY;
			}
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
		  return 1;
		}
		case SB_LINERIGHT: {
		  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
			MovieTimeStep -= SPEDLIN;
			if (MovieTimeStep < MINDELAY) {
			  MovieTimeStep = MINDELAY;
			}
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
		  return 1;
		}
		case SB_PAGELEFT: {
		  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
			MovieTimeStep += SPEDPAG;
			if (MovieTimeStep < MINDELAY) {
			  MovieTimeStep = MINDELAY;
			}
			setsped();
			SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
		  }
		  else {
			thred::pglft();
		  }
		  return 1;
		}
		case SB_PAGERIGHT: {
		  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
			MovieTimeStep -= SPEDPAG;
			if (MovieTimeStep < MINDELAY) {
			  MovieTimeStep = MINDELAY;
			}
			setsped();
			SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
		  }
		  else {
			thred::pgrit();
		  }
		  return 1;
		}
		case SB_THUMBPOSITION: {
		  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
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
			  auto const unzoomedX = wrap::toFloat(UnzoomedRect.cx);
			  if (ZoomRect.right > unzoomedX) {
				ZoomRect.right = unzoomedX;
				ZoomRect.left  = unzoomedX - zoomWidth;
			  }
			  StateMap->set(StateFlag::RESTCH);
			}
		  }
		  return 1;
		}
		default: {
		  outDebugString(L"default hit in wndProc 1: wParam [{}]\n", LOWORD(wParam));
		  break;
		}
	  }
	  break;
	}
	case WM_VSCROLL: {
	  switch (LOWORD(wParam)) {
		case SB_LINEDOWN: {
		  auto scrollPoint = POINT {};
		  scrollPoint.y    = std::lround((ZoomRect.top - ZoomRect.bottom) * LINSCROL);
		  if (scrollPoint.y == 0) {
			scrollPoint.y = 1;
		  }
		  rshft(scrollPoint);
		  return 1;
		}
		case SB_LINEUP: {
		  auto scrollPoint = POINT {};
		  scrollPoint.y    = std::lround(-(ZoomRect.top - ZoomRect.bottom) * LINSCROL);
		  if (scrollPoint.y == 0) {
			scrollPoint.y = -1;
		  }
		  rshft(scrollPoint);
		  return 1;
		}
		case SB_PAGEDOWN: {
		  thred::pgdwn();
		  return 1;
		}
		case SB_PAGEUP: {
		  thred::pgup();
		  return 1;
		}
		case SB_THUMBPOSITION: {
		  auto const zoomHeight = ZoomRect.top - ZoomRect.bottom;

		  ZoomRect.top    = wrap::toFloat(UnzoomedRect.cy) - wrap::toFloat(HIWORD(wParam));
		  ZoomRect.bottom = ZoomRect.top - zoomHeight;
		  if (ZoomRect.bottom < 0) {
			ZoomRect.bottom = 0.0F;
			ZoomRect.top    = zoomHeight;
		  }
		  StateMap->set(StateFlag::RESTCH);
		  return 1;
		}
		default: {
		  outDebugString(L"default hit in wndProc 2: wParam [{}]\n", LOWORD(wParam));
		  break;
		}
	  }
	  break;
	}
	case WM_DRAWITEM: {
// owner draw windows
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, performance-no-int-to-ptr)
	  DrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
	  if (DrawItem->hwndItem == MainStitchWin && DrawItem->itemAction == ODA_DRAWENTIRE) {
		if (StateMap->test(StateFlag::TXTRED)) {
		  texture::drwtxtr();
		  return 1;
		}
		if (!StateMap->test(StateFlag::RUNPAT)) {
		  if (!StateMap->test(StateFlag::HIDSTCH) &&
		      ((!ThrName->empty()) || StateMap->test(StateFlag::INIT) || !FormList->empty() ||
		       StateMap->test(StateFlag::SATPNT)) &&
		      !StateMap->test(StateFlag::BAKSHO)) {
			drwStch();
		  }
		  else {
			thi::drawBackground();
			if (StateMap->test(StateFlag::HIDSTCH)) {
			  form::drwfrm();
			  if (StateMap->test(StateFlag::SATPNT)) {
				satin::satzum();
			  }
			}
			if (StateMap->test(StateFlag::PRFACT)) {
			  thred::redraw(PreferencesWindow);
			}
			else {
			  if (StateMap->test(StateFlag::MOVFRM)) {
				form::unfrm();
				StateMap->set(StateFlag::SHOFRM);
				form::dufrm();
			  }
			  if (StateMap->test(StateFlag::POLIMOV)) {
				StateMap->set(StateFlag::SHOFRM);
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
		  if (StateMap->test(StateFlag::ROTSHO)) {
			durot();
		  }
		  if (StateMap->test(StateFlag::SHOSAT)) {
			satin::dusat();
		  }
		  if (StateMap->test(StateFlag::SHOINSF)) {
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
		if (StateMap->test(StateFlag::HID)) {
		  auto const ucb         = wrap::next(UserColorBrush.begin(), ActiveColor);
		  auto const itUserColor = wrap::next(UserColor.begin(), ActiveColor);
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, *ucb);
		  SetBkColor(DrawItem->hDC, *itUserColor);
		}
		else {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_BTNFACE));
		}
		if (StateMap->test(StateFlag::TXTRED)) {
		  texture::writeScreenWidth(position);
		}
		else {
		  auto const pikol = displayText::loadStr(IDS_PIKOL);
		  wrap::textOut(DrawItem->hDC, position, 1, pikol.c_str(), wrap::toUnsigned(pikol.size()));
		}
		return 1;
	  }
	  if (StateMap->test(StateFlag::WASTRAC)) {
		trace::wasTrace();
		return 1;
	  }
	  auto dcb = DefaultColorBrush.begin();
	  auto ucb = UserColorBrush.begin();
	  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
		if (DrawItem->hwndItem == DefaultColorWin->operator[](iColor)) {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, *dcb);
		  if (DisplayedColorBitmap.test(iColor)) {
			SetBkColor(DrawItem->hDC, DEFAULT_COLORS.at(iColor));
			SetTextColor(DrawItem->hDC, defTxt(iColor));
			auto const colorNum = fmt::format(FMT_COMPILE(L"{}"), iColor + 1U);
			auto       textSize = SIZE {};
			wrap::getTextExtentPoint32(
			    DrawItem->hDC, colorNum.c_str(), wrap::toUnsigned(colorNum.size()), &textSize);
			wrap::textOut(DrawItem->hDC,
			              (ButtonWidth - textSize.cx) / 2,
			              0,
			              colorNum.c_str(),
			              wrap::toUnsigned(colorNum.size()));
		  }
		  return 1;
		}
		if (DrawItem->hwndItem == UserColorWin->operator[](iColor)) {
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
		  return 1;
		}
		++dcb;
		++ucb;
	  }
	  if (StateMap->test(StateFlag::BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {
		if (StateMap->test(StateFlag::THUMSHO)) {
		  auto itHWndBV = BackupViewer.begin();
		  for (auto iThumb = uint32_t {}; iThumb < QUADRT; ++iThumb) {
			if (iThumb < ThumbnailDisplayCount && DrawItem->hwndItem == *itHWndBV) {
			  ritbak(Thumbnails->operator[](ThumbnailsSelected.at(iThumb)).data(), *DrawItem);
			  rthumnam(iThumb);
			  return 1;
			}
			++itHWndBV;
		  }
		}
		else {
		  auto itHWndBV = BackupViewer.begin();
		  for (auto iVersion = wchar_t {}; iVersion < OLDVER; ++iVersion) {
			if (DrawItem->hwndItem == *itHWndBV) {
			  auto fileName = *ThrName; // intentional copy
			  auto ext      = fileName.extension().wstring();
			  ext.back()    = iVersion + 's';
			  fileName.replace_extension(ext);
			  ritbak(fileName, *DrawItem);
			  return 1;
			}
			++itHWndBV;
		  }
		}
	  }
	  break;
	}
	case WM_SIZE: {
	  GetClientRect(p_hWnd, &ThredWindowRect);
	  switch (wParam) {
		case SIZE_MAXIMIZED: {
		  UserFlagMap->set(UserFlag::SAVMAX);
		  break;
		}
		case SIZE_MINIMIZED: {
		  UserFlagMap->reset(UserFlag::SAVMAX);
		  break;
		}
		case SIZE_RESTORED: {
		  UserFlagMap->reset(UserFlag::SAVMAX);
		  chkirct();
		  if (StateMap->testAndSet(StateFlag::SIZED)) {
			auto screenCenterOffset = LONG {};
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
		  break;
		}
		default: {
		  outDebugString(L"default hit in wndProc 3: wParam [{}]\n", wParam);
		  break;
		}
	  }
	  GetClientRect(p_hWnd, &ThredWindowRect);
	  thred::movStch();
	  if (StateMap->test(StateFlag::ZUMED)) {
		auto const bRatio = wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
		auto const adjustedWidth = wrap::toFloat(StitchWindowClientRect.right) / bRatio;
		auto const unzoomedX     = wrap::toFloat(UnzoomedRect.cx);
		if (adjustedWidth + ZoomRect.left > unzoomedX) {
		  ZoomRect.right = unzoomedX;
		  ZoomRect.left  = unzoomedX - adjustedWidth;
		}
		else {
		  ZoomRect.right = adjustedWidth + ZoomRect.left;
		}
	  }
	  else {
		ZoomRect = F_RECTANGLE {
		    0.0F, gsl::narrow<float>(UnzoomedRect.cy), gsl::narrow<float>(UnzoomedRect.cx), 0.0F};
	  }
	  NearestCount = 0;
	  StateMap->set(StateFlag::RESTCH);
	  if (StateMap->test(StateFlag::SELBOX)) {
		shft2box();
	  }
	  if (StateMap->test(StateFlag::RUNPAT)) {
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		if (StateMap->test(StateFlag::ZUMED)) {
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
	  constexpr auto SMALLWIN  = LONG {20};  // Smallest ThrEd window dimension
	  constexpr auto DEFWINDIM = LONG {300}; // default small Thred window dimension
	  if ((ThredWindowRect.right - ThredWindowRect.left) < SMALLWIN) {
		ThredWindowRect.left  = 0;
		ThredWindowRect.right = DEFWINDIM;
	  }
	  if ((ThredWindowRect.bottom - ThredWindowRect.top) < SMALLWIN) {
		ThredWindowRect.top    = 0;
		ThredWindowRect.bottom = DEFWINDIM;
	  }
	  thred::movStch();
	  if (StateMap->test(StateFlag::RUNPAT)) {
		thred::duzrat();
		RunPoint = 0;
		FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
	  }
	  return 1;
	}
	case WM_CLOSE: {
	  thred::dun();
	  if (StateMap->test(StateFlag::SAVEX)) {
		return 1;
	  }
	  break;
	}
	default: {
	  // outDebugString(L"default hit in wndProc 4: message [{}]\n", message);
	  break;
	}
  }
  return DefWindowProc(p_hWnd, message, wParam, lParam);
}

void thi::sachk() {
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto const& form = FormList->operator[](iForm);
	if (form.type == SAT && (form.satinGuideCount != 0U)) {
	  auto itGuide = wrap::next(SatinGuides->cbegin(), form.satinGuideIndex);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		if (itGuide->start > form.vertexCount || itGuide->finish > form.vertexCount) {
		  satin::delsac(iForm);
		}
		++itGuide;
	  }
	}
  }
}

void thred::tst() {
  DesignerName->assign(L"Coder");
  ThrName->assign(*DesignerName);
  StateMap->set(StateFlag::RESTCH);
}

#ifdef ALLOCFAILURE
auto handle_program_memory_depletion(uint32_t) -> int32_t {
  // ToDo - Make this handle the failure with more user notifiication
  displayText::shoMsg("Memory Allocation Failure");
  exit(EXIT_FAILURE);
}
#endif

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
  _PNH old_handler = _set_new_handler(handle_program_memory_depletion);
  // char* testalloc = new char[(~uint32_t((int32_t)0) / 2) - 1U]();
  // testalloc[0] = 1;
#endif
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  auto const hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if (SUCCEEDED(hResult)) {
	ThrEdInstance          = hInstance;
	constexpr auto ICONDIM = 32; // window icon x & y dimension in pixels
	auto           winClass =
	    WNDCLASSEX {0U, 0U, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	winClass.cbSize      = sizeof(WNDCLASSEX);
	winClass.style       = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc = thi::wndProc;
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
	  auto privateAllItemsRect              = F_RECTANGLE {};
	  auto privateAngledFormVertices        = std::vector<F_POINT> {};
	  auto privateAuxName                   = fs::path {};
	  auto privateBitmapBackgroundColor     = std::vector<COLORREF> {};
	  auto privateBSequence                 = std::vector<B_SEQ_PNT> {};
	  auto privateBalaradName0              = fs::path {};
	  auto privateBalaradName1              = fs::path {};
	  auto privateBalaradName2              = fs::path {};
	  auto privateButtonWin                 = std::vector<HWND> {};
	  auto privateClipBuffer                = std::vector<F_POINT_ATTR> {};
	  auto privateClipPoints                = std::vector<F_POINT> {};
	  auto privateColorFileName             = fs::path {};
	  auto privateColorChangeTable          = std::vector<COL_CHANGE> {};
	  auto privateDefaultColorWin           = std::vector<HWND> {};
	  auto privateDefaultDirectory          = fs::path {};
	  auto privateDesignerName              = std::wstring {};
	  auto privateExtendedHeader            = THR_HEAD_EX {};
	  auto privateFormAngles                = std::vector<float> {};
	  auto privateFormControlPoints         = std::vector<POINT> {};
	  auto privateFormLines                 = std::vector<POINT> {};
	  auto privateFormList                  = std::vector<FRM_HEAD> {};
	  auto privateFormOnOff                 = std::wstring {};
	  auto privateFormVertices              = std::vector<F_POINT> {};
	  auto privateFormVerticesAsLine        = std::vector<POINT> {};
	  auto privateHomeDirectory             = fs::path {};
	  auto privateIniFileName               = fs::path {};
	  auto privateInsidePointList           = std::vector<F_POINT> {};
	  auto privateInterleaveSequence        = std::vector<F_POINT> {};
	  auto privateInterleaveSequenceIndices = std::vector<INS_REC> {};
	  auto privateKnots                     = std::vector<uint32_t> {};
	  auto privateLabelWindow               = std::vector<HWND> {};
	  FormOnOff                             = &privateFormOnOff;

	  auto privateMenuInfo = MENUITEMINFO {
	      sizeof(MENUITEMINFO), // Size
	      MIIM_TYPE,            // Mask
	      MFT_STRING,           // Type
	      0,                    // State
	      0,                    // ID
	      nullptr,              // SubMenu
	      nullptr,              // bmpChecked
	      nullptr,              // bmpUnchecked
	      0,                    // ItemData
	      FormOnOff->data(),    // TypeData
	      1,                    // cch (dummy value since we use SetMenuItemInfo)
	      nullptr               // bmpItem (available only on Windows 2000 and higher)
	  };

	  auto privateMsgBuffer                 = std::vector<wchar_t> {};
	  auto privateNearestPixel              = std::vector<POINT> {};
	  auto privateNearestPoint              = std::vector<uint32_t> {};
	  auto privateOSequence                 = std::vector<F_POINT> {};
	  auto privateOutsidePointList          = std::vector<F_POINT> {};
	  auto privatePreviousNames             = std::vector<fs::path> {};
	  auto privateRGBFileName               = fs::path {};
	  auto privateRubberBandLine            = std::vector<POINT> {};
	  auto privateSatinGuides               = std::vector<SAT_CON> {};
	  auto privateSearchLine                = std::vector<POINT> {};
	  auto privateSelectedFormList          = std::vector<uint32_t> {};
	  auto privateSelectedFormsLine         = std::vector<POINT> {};
	  auto privateSelectedPointsLine        = std::vector<POINT> {};
	  auto privateSelectedTexturePointsList = std::vector<uint32_t> {};
	  auto privateSideWindow                = std::vector<HWND> {};
	  auto privateSideWindowEntryBuffer     = std::vector<wchar_t> {};
	  auto privateSortBuffer                = std::vector<SEARCH_REC> {};
	  auto privateStateMap                  = ENUM_MAP<StateFlag> {0};
	  auto privateStitchBuffer              = std::vector<F_POINT_ATTR> {};
	  auto privateTempPolygon               = std::vector<F_POINT> {};
	  auto privateTempTexturePoints         = std::vector<TX_PNT> {};
	  auto privateTextureHistory            = std::vector<TX_HIST> {};
	  auto privateTexturePointsBuffer       = std::vector<TX_PNT> {};
	  auto privateThrName                   = fs::path {};
	  auto privateThreadSizeWin             = std::vector<HWND> {};
	  auto privateThumbnailSearchString     = std::vector<wchar_t> {};
	  auto privateThumbnails                = std::vector<std::wstring> {};
	  auto privateTracedEdges               = boost::dynamic_bitset<> {};
	  auto privateTracedMap                 = boost::dynamic_bitset<> {};
	  auto privateUndoBuffer                = std::vector<std::unique_ptr<uint32_t[]>> {};
	  auto privateUserBMPFileName           = fs::path {};
	  auto privateUserColorWin              = std::vector<HWND> {};
	  auto privateUserFlagMap               = ENUM_MAP<UserFlag> {0};
	  auto privateUserPen                   = std::vector<HPEN> {};
	  auto privateValueWindow               = std::vector<HWND> {};
	  auto privateVersionNames              = std::vector<fs::path> {};
	  auto privateWorkingFileName           = fs::path {};
	  auto privatetextureInputBuffer        = std::wstring {};

	  privateDefaultColorWin.resize(COLORCNT);
	  privateFormControlPoints.resize(OUTPNTS);
	  privateLabelWindow.resize(LASTLIN);
	  constexpr auto MSGSIZ = uint32_t {8192U}; // size of the message buffer
	  privateMsgBuffer.reserve(MSGSIZ);
	  privateNearestPixel.resize(NERCNT);
	  privateNearestPoint.resize(NERCNT);
	  privateRubberBandLine.resize(3U);
	  privateSelectedFormsLine.resize(OUTPNTS);
	  privateSelectedPointsLine.resize(OUTPNTS);
	  constexpr auto SWCOUNT = 16U; // number of side windows to create/track
	  privateSideWindow.resize(SWCOUNT);
	  constexpr auto SWBLEN = 11U; // Side Window buffer length including the zero terminator
	  privateSideWindowEntryBuffer.resize(SWBLEN);
	  privateTextureHistory.resize(ITXBUFSZ);
	  privateThreadSizeWin.resize(COLORCNT);
	  constexpr auto TSSSIZ = size_t {32U}; // size of the message buffer
	  privateThumbnailSearchString.reserve(TSSSIZ);
	  privateUndoBuffer.resize(UNDOLEN);
	  privateUserColorWin.resize(COLORCNT);
	  privateUserPen.resize(COLORCNT);
	  privateValueWindow.resize(LASTLIN);
	  privatePreviousNames.reserve(OLDNUM);
	  privateVersionNames.reserve(OLDVER);
	  for (auto iVersion = 0U; iVersion < OLDNUM; ++iVersion) {
		privatePreviousNames.emplace_back(L"");
	  }
	  for (auto iVersion = wchar_t {}; iVersion < OLDVER; ++iVersion) {
		privateVersionNames.emplace_back(L"");
	  }
	  AllItemsRect              = &privateAllItemsRect;
	  AngledFormVertices        = &privateAngledFormVertices;
	  AuxName                   = &privateAuxName;
	  BSequence                 = &privateBSequence;
	  ButtonWin                 = &privateButtonWin;
	  ClipBuffer                = &privateClipBuffer;
	  ClipPoints                = &privateClipPoints;
	  ColorChangeTable          = &privateColorChangeTable;
	  DefaultColorWin           = &privateDefaultColorWin;
	  DefaultDirectory          = &privateDefaultDirectory;
	  DesignerName              = &privateDesignerName;
	  ExtendedHeader            = &privateExtendedHeader;
	  FormAngles                = &privateFormAngles;
	  FormControlPoints         = &privateFormControlPoints;
	  FormLines                 = &privateFormLines;
	  FormList                  = &privateFormList;
	  FormVertices              = &privateFormVertices;
	  HomeDirectory             = &privateHomeDirectory;
	  IniFileName               = &privateIniFileName;
	  InsidePointList           = &privateInsidePointList;
	  InterleaveSequence        = &privateInterleaveSequence;
	  InterleaveSequenceIndices = &privateInterleaveSequenceIndices;
	  Knots                     = &privateKnots;
	  LabelWindow               = &privateLabelWindow;
	  LRUPtr                    = &LRUMenuId;
	  MenuInfo                  = &privateMenuInfo;
	  MsgBuffer                 = &privateMsgBuffer;
	  NearestPixel              = &privateNearestPixel;
	  NearestPoint              = &privateNearestPoint;
	  OSequence                 = &privateOSequence;
	  OutsidePointList          = &privateOutsidePointList;
	  PreviousNames             = &privatePreviousNames;
	  RubberBandLine            = &privateRubberBandLine;
	  SatinGuides               = &privateSatinGuides;
	  SearchLine                = &privateSearchLine;
	  SelectedFormList          = &privateSelectedFormList;
	  SelectedFormsLine         = &privateSelectedFormsLine;
	  SelectedPointsLine        = &privateSelectedPointsLine;
	  SideWindow                = &privateSideWindow;
	  SideWindowEntryBuffer     = &privateSideWindowEntryBuffer;
	  SortBuffer                = &privateSortBuffer;
	  StateMap                  = &privateStateMap;
	  StitchBuffer              = &privateStitchBuffer;
	  TempPolygon               = &privateTempPolygon;
	  TextureInputBuffer        = &privatetextureInputBuffer;
	  TexturePointsBuffer       = &privateTexturePointsBuffer;
	  ThrName                   = &privateThrName;
	  ThreadSizeWin             = &privateThreadSizeWin;
	  ThumbnailSearchString     = &privateThumbnailSearchString;
	  Thumbnails                = &privateThumbnails;
	  TracedEdges               = &privateTracedEdges;
	  TracedMap                 = &privateTracedMap;
	  UndoBuffer                = &privateUndoBuffer;
	  UserColorWin              = &privateUserColorWin;
	  UserFlagMap               = &privateUserFlagMap;
	  UserPen                   = &privateUserPen;
	  ValueWindow               = &privateValueWindow;
	  VersionNames              = &privateVersionNames;
	  WorkingFileName           = &privateWorkingFileName;
	  bal::setBN0(&privateBalaradName0);
	  bal::setBN1(&privateBalaradName1);
	  bal::setBN2(&privateBalaradName2);
	  bitmap::setBBCV(&privateBitmapBackgroundColor);
	  bitmap::setUBfilename(&privateUserBMPFileName);
	  DST::setColFilename(&privateColorFileName);
	  DST::setRGBFilename(&privateRGBFileName);
	  texture::initTextures(&privateTempTexturePoints, &privateSelectedTexturePointsList, &privateTextureHistory);
	  tfc::setFVAS(&privateFormVerticesAsLine);
	  thi::redini();
	  createParams createParams {};
	  createParams.bEnableNonClientDpiScaling = TRUE;

	  constexpr auto SCROLSIZ            = int32_t {12}; // logical pixel width of a scroll bar
	  auto           privateScrollSize   = SCROLSIZ;
	  ScrollSize                         = &privateScrollSize;
	  constexpr auto COLSIZ              = int32_t {12}; // logical pixel width of the color bar
	  auto           privateColorBarSize = COLSIZ;
	  ColorBarSize                       = &privateColorBarSize;
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
	  auto privateDPI     = gsl::narrow<int32_t>(GetDpiForWindow(ThrEdWindow));
	  ScreenDPI           = &privateDPI;
	  privateScrollSize   = MulDiv(privateScrollSize, *ScreenDPI, STDDPI);
	  privateColorBarSize = MulDiv(privateColorBarSize, *ScreenDPI, STDDPI);
	  thi::init();
	  if (UserFlagMap->test(UserFlag::SAVMAX)) {
		ShowWindow(ThrEdWindow, SW_SHOWMAXIMIZED);
	  }
	  else {
		ShowWindow(ThrEdWindow, SW_SHOW);
	  }
	  if (DesignerName->empty()) {
		DesignerName->assign(displayText::loadStr(IDS_UNAM));
		thred::getdes();
	  }
	  auto xyRatio        = 1.0F; // expand form aspect ratio
	  auto rotationAngle  = 0.0F;
	  auto rotationCenter = F_POINT {};
	  auto textureForm    = FRM_HEAD {};
	  auto stretchBoxLine = std::vector<POINT> {};
	  stretchBoxLine.resize(SQPNTS); // stretch and expand
	  while (GetMessage(&Msg, nullptr, 0, 0)) {
		StateMap->set(StateFlag::SAVACT);
		if (!thi::chkMsg(stretchBoxLine, xyRatio, rotationAngle, rotationCenter, textureForm)) {
		  DispatchMessage(&Msg);
		}
		if (StateMap->testAndReset(StateFlag::FCHK)) {
		  repair::frmchkx();
		}
		if (StateMap->testAndReset(StateFlag::RESTCH)) {
		  thred::redraw(MainStitchWin);
		}
		if (StateMap->testAndReset(StateFlag::RELAYR)) {
		  thi::ritlayr();
		}
		if (!StateMap->test(StateFlag::TXTRED)) {
		  thi::sachk();
		}
		if (StateMap->testAndReset(StateFlag::DUMEN)) {
		  DrawMenuBar(ThrEdWindow);
		}
	  }
#ifdef ALLOCFAILURE
	  _set_new_handler(old_handler);
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
  auto itUserPen = UserPen->begin();
  auto ucb       = UserColorBrush.begin();
  for (auto const& color : UserColor) {
	thred::nuPen(*itUserPen, 1, color);
	thi::nuBrush(*ucb, color);
	++itUserPen;
	++ucb;
  }
  for (auto const& iColor : *UserColorWin) {
	thred::redraw(iColor);
  }
}

void thred::initPenBrush() noexcept {
  auto itUserPen = UserPen->begin();
  auto ucb       = UserColorBrush.begin();
  for (auto const& ucolor : UserColor) {
	*(itUserPen++) = wrap::createPen(PS_SOLID, PENNWID, ucolor);
	thi::nuBrush(*ucb, ucolor);
	++ucb;
  }
}

void thred::initBackPenBrush() noexcept {
  thred::nuPen(BackgroundPen, 1, BackgroundColor);
  BackgroundPenWidth = 1;
  DeleteObject(BackgroundBrush);
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
}

auto thred::setFileName() -> fs::path {
  return (WorkingFileName->empty()) ? (*DefaultDirectory / L"balfil.thr") : *WorkingFileName;
}

auto thred::getDesigner() -> std::wstring {
  auto const modifier = utf::utf8ToUtf16(std::string(ExtendedHeader->modifierName.data()));
  return displayText::format2(IDS_CREATBY, *DesignerName, modifier);
}

auto thred::getVerticalIndex() noexcept -> uint8_t {
  return VerticalIndex;
}

auto thred::getColorChangeIndex(uint32_t iColor) noexcept -> uint16_t {
  return ColorChangeTable->operator[](iColor).stitchIndex;
}

auto thred::getFormControlPoints() noexcept -> std::vector<POINT>* {
  return FormControlPoints;
}

void thred::chkInsCol() {
  if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), UserColorWin->back())) { // check if point is in any of the color windows
	thi::inscol();
  }
}

void thred::chkDelCol() {
  if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), UserColorWin->back())) {
	thi::delcol();
  }
  else {
	thred::delet();
  }
}

void thred::setSrchLargest() {
  thi::setsrch(true);
}

void thred::setSrchSmallest() {
  thi::setsrch(false);
}

auto thred::getZoomMin() noexcept -> float {
  return ZoomMin;
}

void thred::resetColors() {
  auto ucb         = UserColorBrush.begin();
  auto itUserColor = UserColor.begin();
  auto itUserPen   = UserPen->begin();
  auto ucw         = UserColorWin->begin();
  for (auto const& color : DEFAULT_COLORS) {
	*itUserColor = color;
	thi::nuBrush(*ucb, *itUserColor);
	thred::nuPen(*itUserPen, 1, *itUserColor);
	thred::redraw(*ucw);
	++ucb;
	++itUserColor;
	++itUserPen;
	++ucw;
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::setThreadSize() noexcept {
  ThreadSize30 = TSIZ30;
  ThreadSize40 = TSIZ40;
  ThreadSize60 = TSIZ60;
}

void thred::viewThread30() {
  displayText::tsizmsg(L"30", ThreadSize30);
  StateMap->set(StateFlag::ENTR30);
}

void thred::viewThread40() {
  displayText::tsizmsg(L"40", ThreadSize40);
  StateMap->set(StateFlag::ENTR40);
}

void thred::viewThread60() {
  displayText::tsizmsg(L"60", ThreadSize60);
  StateMap->set(StateFlag::ENTR60);
}

void thred::setBackGroundColor() {
  if (thi::nuBak() != 0U) {
	BackgroundColor = BackgroundColorStruct.rgbResult;
	thred::nuPen(BackgroundPen, 1, BackgroundColor);
	BackgroundPenWidth = 1;
	DeleteObject(BackgroundBrush);
	BackgroundBrush = CreateSolidBrush(BackgroundColor);
	if (bitmap::ismap()) {
	  bitmap::bfil(BackgroundColor);
	}
	thred::zumhom();
  }
}

void thred::openAuxFile() {
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  thi::nuFil(FileIndices::DST);
	  break;
	}
#if PESACT
	case AUXPES: {
	  thi::nuFil(FileIndices::PES);
	  break;
	}
#endif
	default: {
	  thi::nuFil(FileIndices::PCS);
	}
  }
  thred::showOnlyLayer(0U);
}

void thred::openNewFile() {
  if (thi::savcmp()) {
	thred::newFil();
	thred::showOnlyLayer(0U);
  }
  else {
	displayText::savdisc();
	StateMap->set(StateFlag::NEWBAK);
	StateMap->reset(StateFlag::PRFACT);
	thred::undat();
  }
}

void thred::openBitMapFile() {
  bitmap::lodbmp(*DefaultDirectory);
}

void thred::destroyChangeThreadSizeWindows() noexcept {
  for (auto const& iWindow : ChangeThreadSizeWin) {
	if (iWindow != nullptr) {
	  DestroyWindow(iWindow);
	}
  }
}

void thred::updateThreadSize(uint32_t threadSizeSelected) {
  static constexpr auto THREAD_SIZE_MAP = std::array<wchar_t, 3> {L'3', L'4', L'6'};

  auto const itThreadSize = wrap::next(ThreadSize.begin(), threadSizeSelected);
  auto const tsm          = wrap::next(THREAD_SIZE_MAP.begin(), VerticalIndex);
  *itThreadSize           = *tsm;
  auto const tsi          = wrap::next(ThreadSizeIndex.begin(), threadSizeSelected);
  *tsi                    = VerticalIndex;
  auto buffer             = std::array<wchar_t, 3> {};
  buffer[0]               = *itThreadSize;
  buffer[1]               = L'0';
  auto const tsw          = wrap::next(ThreadSizeWin->begin(), threadSizeSelected);
  SetWindowText(*tsw, buffer.data());
  StateMap->set(StateFlag::RESTCH);
  thred::destroyChangeThreadSizeWindows();
}

auto thred::createChangeThreadSizeWindows() -> uint32_t {
  static constexpr auto THREAD_SIZES = std::array<wchar_t const*, 3> {L"30", L"40", L"60"};
  thred::savdo();
  auto const threadSizeSelected = VerticalIndex;
  auto       idx                = gsl::narrow_cast<int32_t>(VerticalIndex);
  auto       iStr               = THREAD_SIZES.begin();
  std::ranges::generate(ChangeThreadSizeWin, [&idx, &iStr]() mutable noexcept -> HWND {
	return CreateWindow(L"STATIC",
	                    *(iStr++),
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
  StateMap->set(StateFlag::SIZSEL);
  return threadSizeSelected;
}

void thred::updateUserColor() {
  if (Msg.message == WM_LBUTTONDOWN && (thi::nuCol(UserColor.at(VerticalIndex)) != 0U)) {
	thred::savdo();
	auto const itUserColor = wrap::next(UserColor.begin(), VerticalIndex);
	*itUserColor           = ColorStruct.rgbResult;
	auto const itUserPen   = wrap::next(UserPen->begin(), VerticalIndex);
	thred::nuPen(*itUserPen, 1, *itUserColor);
	auto const ucb = wrap::next(UserColorBrush.begin(), VerticalIndex);
	thi::nuBrush(*ucb, *itUserColor);
	thred::redraw(UserColorWin->operator[](VerticalIndex));
	StateMap->set(StateFlag::RESTCH);
  }
}

void thred::switchUserColors() noexcept {
  auto const code = ActiveColor;
  ActiveColor     = VerticalIndex & COLMSK;
  thred::redraw(UserColorWin->operator[](code));
  thred::redraw(UserColorWin->operator[](ActiveColor));
}

// ToDo - rename this to something more appropriate
void thred::redrawCapturedStitch(uint32_t closestPointIndexClone) {
  ClosestPointIndex = closestPointIndexClone;
  thred::unbox();
  thi::unboxs();
  auto const index = StitchBuffer->operator[](ClosestPointIndex).attribute & COLMSK;
  auto const spTSP = wrap::next(ThreadSizePixels.begin(), index);
  thi::setbak(*spTSP + 3);
  auto linePoints = std::vector<POINT> {};
  linePoints.resize(3);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (ClosestPointIndex == 0) {
	if (ZoomFactor < STCHBOX) {
	  SelectObject(StitchWindowDC, LinePen);
	  thi::stchbox(0, StitchWindowDC);
	  thi::stchbox(1, StitchWindowDC);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
	thi::drwLin(linePoints, 0, 2, BackgroundPen);
  }
  else {
	if (!StitchBuffer->empty()) {
	  auto const lastStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
	  if (ClosestPointIndex == lastStitch) {
		if (ZoomFactor < STCHBOX) {
		  SelectObject(StitchWindowDC, LinePen);
		  thi::stchbox(lastStitch, StitchWindowDC);
		  thi::stchbox(lastStitch - 1, StitchWindowDC);
		}
		SetROP2(StitchWindowDC, R2_COPYPEN);
		thi::drwLin(linePoints, lastStitch - 1, 2, BackgroundPen);
	  }
	  else {
		if (ZoomFactor < STCHBOX) {
		  SelectObject(StitchWindowDC, LinePen);
		  thi::stchbox(ClosestPointIndex - 1, StitchWindowDC);
		  thi::stchbox(ClosestPointIndex, StitchWindowDC);
		  thi::stchbox(ClosestPointIndex + 1, StitchWindowDC);
		}
		SetROP2(StitchWindowDC, R2_COPYPEN);
		thi::drwLin(linePoints, ClosestPointIndex - 1, 3, BackgroundPen);
	  }
	}
  }
  MoveLine0[1] = MoveLine1[0] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
  if (ClosestPointIndex == 0) {
	StateMap->reset(StateFlag::ISDWN);
  }
  else {
	StateMap->set(StateFlag::ISDWN);
	MoveLine0[0] = thred::stch2px1(ClosestPointIndex - 1);
  }
  auto iStitch = wrap::toUnsigned(StitchBuffer->size());
  if (iStitch != 0U) {
	--iStitch;
  }
  if (ClosestPointIndex >= iStitch) {
	StateMap->reset(StateFlag::ISUP);
  }
  else {
	StateMap->set(StateFlag::ISUP);
	MoveLine1[1] = thred::stch2px1(ClosestPointIndex + 1U);
  }
  thi::dulin(MoveLine0, MoveLine1);
  SetCapture(ThrEdWindow);
  StateMap->set(StateFlag::CAPT);
  displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
}

void thred::setFormControls() noexcept(!std::is_same_v<size_t, uint32_t>) {
  auto formsRect = F_RECTANGLE {};
  thred::selRct(formsRect);
  auto& frmCtrls = *FormControlPoints;
  frmCtrls[PTL].x = frmCtrls[PBL].x = frmCtrls[PLM].x = frmCtrls[PTLE].x = std::lround(formsRect.left);
  frmCtrls[PTL].y = frmCtrls[PTR].y = frmCtrls[PTM].y = frmCtrls[PTLE].y = std::lround(formsRect.top);
  frmCtrls[PTR].x = frmCtrls[PRM].x = frmCtrls[PBR].x = std::lround(formsRect.right);
  frmCtrls[PBR].y = frmCtrls[PBM].y = frmCtrls[PBL].y = std::lround(formsRect.bottom);
  frmCtrls[PRM].y = frmCtrls[PLM].y = std::lround(wrap::midl(formsRect.top, formsRect.bottom));
  frmCtrls[PTM].x = frmCtrls[PBM].x = std::lround(wrap::midl(formsRect.right, formsRect.left));
}

void thred::setRotateCapture() {
  RotateBoxToCursorLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
  auto const adjustedPoint =
      F_POINT {gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
               gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y)};
  if (std::hypot(adjustedPoint.x, adjustedPoint.y) < FCLOSNUF) {
	StateMap->set(StateFlag::MOVCNTR);
	thred::unrot();
	thred::ritrot(0, thred::pxCor2stch(Msg.pt));
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
	StateMap->set(StateFlag::ROTUSHO);
	thred::durotu();
	StateMap->set(StateFlag::ROTCAPT);
  }
}

auto thred::chkForm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  return form::chkfrm(FormControlPoints, stretchBoxLine, xyRatio);
}

auto thred::getAdjustedPoint(F_POINT stitchPoint) noexcept -> F_POINT {
  return F_POINT {(StitchRangeRect.left + wrap::toFloat(SelectBoxOffset.x)) - stitchPoint.x,
                  (StitchRangeRect.bottom + wrap::toFloat(SelectBoxOffset.y)) - stitchPoint.y};
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
  RotateBoxToCursorLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
  return F_POINT {gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
                  gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y)};
}

auto thred::getRotationHandleAngle() noexcept -> float {
  return RotationHandleAngle;
}

void thred::updateMoveLine() {
  MoveLine0[1] = MoveLine1[0] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
  thi::dulin(MoveLine0, MoveLine1);
}

void thi::fillSortBuffer() {
  SortBuffer->clear();
  SortBuffer->reserve(StitchBuffer->size());
  auto stitch     = wrap::next(StitchBuffer->begin(), SelectedRange.start);
  auto nextStitch = wrap::next(stitch, 1);
  auto index      = SelectedRange.start;

  auto const endStitch = wrap::next(StitchBuffer->begin(), SelectedRange.finish);
  while (nextStitch != endStitch) {
	auto const deltaX = nextStitch->x - stitch->x;
	auto const deltaY = nextStitch->y - stitch->y;
	auto const length = deltaX * deltaX + deltaY * deltaY;
	SortBuffer->push_back(SEARCH_REC {index, length});
	++index;
	++stitch;
	++nextStitch;
  }
  std::ranges::stable_sort(*SortBuffer, {}, &SEARCH_REC::length);
}

void thred::setSmallestStitchVal() {
  thi::srchk();
  thi::fillSortBuffer();
  SortIndex = 0U;
  thred::setSrchSmallest();
  thi::lensadj();
}

void thred::setLargestStitchVal() {
  thi::srchk();
  thi::fillSortBuffer();
  SortIndex = wrap::toUnsigned(SortBuffer->size() - 1U);
  thred::setSrchLargest();
  thi::lensadj();
}
