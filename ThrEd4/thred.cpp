// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
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
#include "bitmap.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "formForms.h"
#include "DST.h"
#include "hlp.h"
#include "PCS.h"
#include "PES.h"
#include "repair.h"
#include "satin.h"
#include "texture.h"
#include "trace.h"
#include "xt.h"
#include "thred.h"

namespace fs = std::filesystem;

namespace thi = thred::internal;

constexpr auto NERCNT   = 4U;                   // number of entries in the near array;
constexpr auto DNLEN    = 50U;                  // designer name order table length
constexpr auto DNELEN   = 128U;                 // designer name encoding table length
constexpr auto DNDLEN   = 256U;                 // designer name decoding table length
constexpr auto UNDOLEN  = 16U;                  // UndoBuffer length
constexpr auto THREDSIG = uint32_t {0x746872U}; // ThrEd format file signature
constexpr auto SIGMASK = uint32_t {0x00ffffffU}; // three byte mask used for file signature verification
constexpr auto FTYPMASK = uint32_t {0xff000000U}; // top byte mask used for file type verification
constexpr auto KNOTSCNT = 5U;                     // length of knot pattern in stitches
constexpr auto ARROWPNT = 3U;                     // points required to draw arrow
constexpr auto FILLTYPS = uint32_t {14U};         // 13 fill types plus 'none'
constexpr auto FSSIZE   = uint8_t {6U};           // count of feather styles
constexpr auto EDGETYPS = uint32_t {13U};         // number of border fill types
constexpr auto OLDVER   = wchar_t {4};            // number of old file versions kept
constexpr auto HUPS     = 5;                      // number of hoops the user can select
constexpr auto BALRATIO = 10.0F / 6.0F;           // Balarad stitch size ration

// main variables
static auto ArgCount        = int32_t {};                         // command line argument count
static auto ArgList         = gsl::narrow_cast<LPTSTR*>(nullptr); // command line argument array
static auto ThredWindowRect = RECT {};                            // main window size
static auto ColorBarRect    = RECT {};                            // color bar rectangle
static auto HomeDirectory = gsl::narrow_cast<fs::path*>(nullptr); // directory from which thred was executed
static auto SmallestStitchIndex = uint32_t {}; // pointer to the smallest stitch in the selected range
static auto LargestStitchIndex = uint32_t {}; // pointer to the largest stitch in the selected range
static auto CurrentStitchIndex = uint32_t {}; // pointer to the current selection for length search
static auto ThredDC            = gsl::narrow_cast<HDC>(nullptr);    // main device context handle
static auto ScreenSizePixels   = SIZE {};                           // screen size in pixels
static auto StitchWindowAbsRect     = RECT {};                      // stitch window size,absolute
static auto NearestPixel            = std::array<POINT, NERCNT> {}; // selected points
static auto PrevGroupStartStitch    = uint32_t {}; // lower end of previous selection
static auto PrevGroupEndStitch      = uint32_t {}; // higher end of previous selection
static auto StitchWindowAspectRatio = float {};    // aspect ratio of the stitch window
static auto ClipOrigin              = POINT {};    // origin of clipboard box in stitch coordinates
static auto SelectBoxSize           = SIZE {};     // size of the select box
static auto SelectBoxOffset         = POINT {}; // offset of the spot the user selected from the lower left of the select box
static auto RotationHandleAngle     = float {};    // angle of the rotation handle
static auto ThreadSize30            = TSIZ30;      //#30 thread size
static auto ThreadSize40            = TSIZ40;      //#40 thread size
static auto ThreadSize60            = TSIZ60;      //#60 thread size
static auto RunPoint                = uint32_t {}; // point for animating stitchout
static auto StitchesPerFrame        = uint32_t {}; // number of stitches to draw in each frame
static auto MovieTimeStep           = int32_t {};  // time delay for stitchout
static auto LRUMenuId =
    std::array<uint32_t, OLDNUM> {FM_ONAM0, FM_ONAM1, FM_ONAM2, FM_ONAM3}; // recently used file menu ID's
static auto UndoBufferWriteIndex = uint32_t {};                            // undo storage pointer
static auto UndoBufferReadIndex  = uint32_t {};                         // undo retrieval pointers
static auto LastKeyCode          = std::numeric_limits<wchar_t>::max(); // last key code
static auto VersionNames =
    gsl::narrow_cast<std::vector<fs::path>*>(nullptr); // temporary storage for old file version names
static auto FileVersionIndex   = uint8_t {};           // points to old version to be read
static auto ClipFormsCount     = uint32_t {};          // number of forms the on the clipboard
static auto StitchArrow        = std::array<POINT, ARROWPNT> {}; // arrow for selected stitch
static auto SelectedRange      = RANGE {}; // first and last stitch for min/max stitch select
static auto NameOrder          = std::array<uint32_t, DNLEN> {}; // designer name order table
static auto NameEncoder        = std::array<uint8_t, DNELEN> {}; // designer name encoding
static auto NameDecoder        = std::array<uint8_t, DNDLEN> {}; // designer name decode
static auto FirstWin           = gsl::narrow_cast<HWND>(nullptr); // first window not destroyed for exiting enumerate loop
static auto SelectedFormsRange = FRMRANGE {}; // range of selected forms
static auto ZoomMin            = float {};    // minimum allowed zoom value
static auto BalaradOffset      = fPOINT {};   // balarad offset
static auto FormVerticesAsLine =
    gsl::narrow_cast<std::vector<POINT>*>(nullptr); // form vertex clipboard paste into form line
static auto LastFormSelected = uint32_t {};         // end point of selected range of forms
static auto UndoBuffer = gsl::narrow_cast<std::vector<std::unique_ptr<uint32_t[]>>*>(nullptr); // backup data NOLINT (modernize-avoid-c-arrays)

// cursors
static auto FormCursor            = gsl::narrow_cast<HCURSOR>(nullptr); // form
static auto DLineCursor           = gsl::narrow_cast<HCURSOR>(nullptr); // dline
static auto NeedleUpCursor        = gsl::narrow_cast<HCURSOR>(nullptr); // upright needle
static auto NeedleLeftUpCursor    = gsl::narrow_cast<HCURSOR>(nullptr); // left up needle
static auto NeedleLeftDownCursor  = gsl::narrow_cast<HCURSOR>(nullptr); // left down needle
static auto NeedleRightUpCursor   = gsl::narrow_cast<HCURSOR>(nullptr); // right up needle
static auto NeedleRightDownCursor = gsl::narrow_cast<HCURSOR>(nullptr); // right down needle
static auto ArrowCursor           = gsl::narrow_cast<HCURSOR>(nullptr); // arrow
static auto CrossCursor           = gsl::narrow_cast<HCURSOR>(nullptr); // cross

// Pens
static auto LayerPen       = std::array<HPEN, MAXLAYER> {};   //
static auto LinePen        = gsl::narrow_cast<HPEN>(nullptr); // line pen for stitch move lines
static auto BoxPen         = std::array<HPEN, 4> {};          // box pens
static auto CrossPen       = gsl::narrow_cast<HPEN>(nullptr); // pen for crosses in color windows
static auto GroupSelectPen = gsl::narrow_cast<HPEN>(nullptr); // pen for group select
static auto BackgroundPen  = gsl::narrow_cast<HPEN>(nullptr); // background color pen
static auto ZoomMarkPen    = gsl::narrow_cast<HPEN>(nullptr); // zoom mark pen
static auto KnotPen        = gsl::narrow_cast<HPEN>(nullptr); // knot pen
static auto BackgroundPenWidth = int32_t {};                  // width of the background pen

// brushes
static auto BackgroundBrush   = gsl::narrow_cast<HBRUSH>(nullptr); // background color brush
static auto DefaultColorBrush = std::array<HBRUSH, COLORCNT> {};   // default color brushes
static auto UserColorBrush    = std::array<HBRUSH, COLORCNT> {};   // user color brushes

// for the choose color dialog box
static auto ColorStruct = CHOOSECOLOR {};
static auto CustomColor = std::array<COLORREF, COLORCNT> {};

// for the background color dialog box
static auto BackgroundColorStruct = CHOOSECOLOR {};
static auto CustomBackgroundColor = std::array<COLORREF, COLORCNT> {};

// threads
static auto    ThreadSize          = std::array<wchar_t, COLORCNT> {}; // user selected thread sizes
static auto    ThreadSizePixels    = std::array<int32_t, COLORCNT> {}; // thread sizes in pixels
static auto    ThreadSizeIndex     = std::array<uint32_t, COLORCNT> {}; // thread size indices
static auto    ChangeThreadSizeWin = std::array<HWND, 3> {}; // thread size change windows
constexpr auto TSSIZE = ThreadSize.size(); // size of the user selected thread size array

// menus
static auto FileMenu       = gsl::narrow_cast<HMENU>(nullptr); // file submenu
static auto BorderFillMenu = gsl::narrow_cast<HMENU>(nullptr); // border fill submenu
static auto ViewMenu       = gsl::narrow_cast<HMENU>(nullptr); // view submenu
static auto ViewSetMenu    = gsl::narrow_cast<HMENU>(nullptr); // view/set
static auto EditMenu       = gsl::narrow_cast<HMENU>(nullptr); // edit submenu

// windows
static auto ColorBar       = gsl::narrow_cast<HWND>(nullptr); // color bar
static auto SpeedScrollBar = gsl::narrow_cast<HWND>(nullptr); // speed scroll bar for movie
static auto BackupViewer = std::array<HWND, OLDVER> {}; // handles of multiple file viewing windows

static auto StitchWindowBmp = gsl::narrow_cast<HBITMAP>(nullptr); // bitmap for the memory stitch device context
static auto DisplayedColorBitmap =
    std::bitset<32> {}; // Map of color numbers in design that are actually displayed
static auto NearestPoint     = std::array<uint32_t, NERCNT> {};   // indices of the closest points
static auto MoveAnchor       = uint32_t {};                       // for resequencing stitches
static auto RotateAngle      = float {};                          // angle for pixel rotate
static auto PickColorMsgSize = SIZE {};                           // size of the pick color message
static auto InsertSize       = POINT {};                          // size of file insert window
static auto InsertCenter     = fPOINT {};                         // center point in inserted file
static auto NumericCode      = wchar_t {};                        // keyboard numerical input
static auto Knots            = std::array<uint32_t, MAXKNOTS> {}; // pointers to knots
static auto KnotCount        = uint32_t {};                       // number of knots in the design

// graphics variables

auto const DefaultColors = std::array<COLORREF, COLORCNT> {0x00000000,
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

static auto BoxOffset = std::array<int32_t, 4> {};

static auto VerticalIndex = uint8_t {}; // vertical index of the color window, calculated from mouse click
static auto DefaultDirectory = gsl::narrow_cast<fs::path*>(nullptr);
static auto BalaradName0     = gsl::narrow_cast<fs::path*>(nullptr); // balarad semaphore file
static auto BalaradName1     = gsl::narrow_cast<fs::path*>(nullptr); // balarad data file
static auto BalaradName2     = gsl::narrow_cast<fs::path*>(nullptr);
static auto IniFileName      = gsl::narrow_cast<fs::path*>(nullptr); //.ini file name
static auto PreviousNames    = gsl::narrow_cast<std::vector<fs::path>*>(nullptr);
static auto Thumbnails       = gsl::narrow_cast<std::vector<std::wstring>*>(nullptr); // vector of thumbnail names

static auto ThumbnailsSelected = std::array<uint32_t, 4> {}; // indexes of thumbnails selected for display

static auto ThumbnailDisplayCount = uint32_t {}; // number of thumbnail file names selected for display
static auto ThumbnailIndex = uint32_t {};        // index into the thumbnail filname table

static auto ThumbnailSearchString = std::array<wchar_t, 32> {}; // storage for the thumnail search string

static auto InsertedVertexIndex = uint32_t {}; // saved vertex pointer for inserting files
static auto InsertedFormIndex   = uint32_t {}; // saved form pointer for inserting files
static auto InsertedStitchIndex = uint32_t {}; // saved stitch pointer for inserting files
static auto MoveLine0           = std::array<POINT, LNPNTS> {}; // move point line
static auto MoveLine1           = std::array<POINT, LNPNTS> {}; // move point line
static auto ClipInsertBoxLine = std::array<POINT, SQPNTS> {}; // for displaying clipboard insert rectangle
static auto RotateBoxOutline = std::array<POINT, SQPNTS> {}; // for drawing the rotate rectangle
static auto RotateBoxCrossVertLine = std::array<POINT, LNPNTS> {}; // vertical part of the rotate cross
static auto RotateBoxCrossHorzLine = std::array<POINT, LNPNTS> {}; // horizontal part of the rotate cross
static auto RotateBoxToCursorLine =
    std::array<POINT, LNPNTS> {}; // line from the cursor to the center of the rotate cross
static auto ColorChangeTable = gsl::narrow_cast<std::vector<COLCHNG>*>(nullptr);

auto CALLBACK thred::internal::dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  // NOLINTNEXTLINE(readability-qualified-auto)
		  auto const hwnd = GetDlgItem(hwndlg, IDC_DESED);
		  auto       designerBuffer = std::array<wchar_t, NameOrder.size()> {};
		  GetWindowText(hwnd,
		                designerBuffer.data(),
		                wrap::toUnsigned(designerBuffer.size()));
		  DesignerName->assign(designerBuffer.data());
		  EndDialog(hwndlg, 0);
		  // NOLINTNEXTLINE
		  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
		  return TRUE;
		}
		default: {
		  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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

void thred::internal::getdes() noexcept {
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-type-cstyle-cast)
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DESNAM), ThrEdWindow, reinterpret_cast<DLGPROC>(dnamproc));
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
  if (UserFlagMap->test(UserFlag::MARQ)) {
	CheckMenuItem(MainMenu, ID_MARKESC, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_MARKQ, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_MARKESC, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_MARKQ, MF_CHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::nedmen() {
  if (UserFlagMap->test(UserFlag::NEDOF)) {
	CheckMenuItem(MainMenu, ID_SETNEDL, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_RSTNEDL, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_SETNEDL, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_RSTNEDL, MF_UNCHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::knotmen() {
  if (UserFlagMap->test(UserFlag::KNOTOF)) {
	CheckMenuItem(MainMenu, ID_KNOTON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_KNOTOF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_KNOTON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_KNOTOF, MF_UNCHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::bsavmen() {
  if (UserFlagMap->test(UserFlag::BSAVOF)) {
	CheckMenuItem(MainMenu, ID_BSAVON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_BSAVOF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_BSAVON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_BSAVOF, MF_UNCHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::linbmen() {
  if (UserFlagMap->test(UserFlag::LINSPAC)) {
	CheckMenuItem(MainMenu, ID_LINBEXACT, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_LINBEVEN, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_LINBEXACT, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_LINBEVEN, MF_UNCHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void thred::wrnmen() {
  auto code = UINT {MF_CHECKED};
  if (UserFlagMap->test(UserFlag::WRNOF)) {
	code = MF_UNCHECKED;
  }
  CheckMenuItem(MainMenu, ID_WARNOF, code);
}

static auto datcod = std::array<UINT, 4U> {ID_CHKOF, ID_CHKON, ID_CHKREP, ID_CHKREPMSG};

void thred::chkmen() noexcept {
  constexpr auto LASTCODE = datcod.size();
  for (auto iCode = 0U; iCode < LASTCODE; ++iCode) {
	auto const code = (iCode == IniFile.dataCheck) ? gsl::narrow_cast<UINT>(MF_CHECKED)
	                                               : gsl::narrow_cast<UINT>(MF_UNCHECKED);
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
  ZoomRatio.x = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  ZoomRatio.y = wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
}

auto thred::internal::rsed() noexcept -> uint32_t {
  auto time = SYSTEMTIME {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
  GetLocalTime(&time);
  return (gsl::narrow_cast<uint32_t>(time.wSecond) << WRDSHFT) | time.wMilliseconds;
}

void thred::internal::fnamtabs() {
  constexpr auto NAMELEN = NameOrder.size();
  for (auto iName = 0U; iName < NAMELEN; ++iName) {
	NameOrder[iName] = iName;
  }
  PseudoRandomValue = NORDSED;
  for (auto iName = 0U; iName < 2 * NAMELEN; ++iName) {
	auto const source      = form::psg() % NAMELEN;
	auto const destination = form::psg() % NAMELEN;
	std::swap(NameOrder[destination], NameOrder[source]);
  }
  for (auto iName = uint8_t {0U}; iName < gsl::narrow<uint8_t>(NameEncoder.size()); ++iName) {
	NameEncoder[iName] = iName + NCODOF;
  }
  PseudoRandomValue = NCODSED;
  for (auto iName = 0U; iName < 2 * NameEncoder.size(); ++iName) {
	auto const source      = form::psg() & MSK7BITS;
	auto const destination = form::psg() & MSK7BITS;
	std::swap(NameEncoder[destination], NameEncoder[source]);
  }
  NameDecoder.fill(0);
  for (auto iName = uint8_t {32U}; iName < uint8_t {127U}; ++iName) {
	NameDecoder[NameEncoder[iName]] = iName;
  }
}

void thred::internal::ritfnam(std::wstring const& designerName) {
  auto designer = utf::Utf16ToUtf8(designerName);
  auto tmpName  = std::array<uint8_t, NameOrder.size()> {0};
  if (NameOrder[0] > NameOrder.size()) {
	fnamtabs();
  }
  PseudoRandomValue = rsed();
  auto iName        = 0U;
  for (; iName < NameOrder.size(); ++iName) {
	tmpName[iName] = (form::psg() & BYTMASK);
  }
  for (iName = 0U; iName < NameOrder.size(); ++iName) {
	if (designer[iName] != 0) {
	  tmpName[iName] = (NameEncoder[wrap::toSize(designer[iName])]);
	}
	else {
	  while (NameDecoder[wrap::toSize(tmpName[iName])] != 0U) {
		tmpName[iName] = (form::psg() & BYTMASK);
	  }
	  break;
	}
  }
  if (iName == NameOrder.size()) {
	auto& index = tmpName[NameOrder.size() - 1U];
	while (NameDecoder[wrap::toSize(index)] != 0U) {
	  index = gsl::narrow_cast<uint8_t>(form::psg() & BYTMASK);
	}
  }
  for (iName = 0U; iName < NameOrder.size(); ++iName) {
	if (NameOrder[iName] < NameOrder.size()) {
	  ExtendedHeader->creatorName[NameOrder[iName]] = gsl::narrow_cast<char>(tmpName[iName]);
	}
  }
}

void thred::internal::redfnam(std::wstring& designerName) {
  auto tmpName  = std::array<uint8_t, NameOrder.size()> {};
  auto designer = std::string {};
  for (auto iName = 0U; iName < tmpName.size(); ++iName) {
	if (NameOrder[iName] < NameOrder.size()) {
	  tmpName[iName] = gsl::narrow_cast<uint8_t>(ExtendedHeader->creatorName[NameOrder[iName]]);
	}
	else {
	  tmpName[iName] = 111;
	}
  }
  designer.reserve(tmpName.size());
  for (auto const character : tmpName) {
	if (NameDecoder[character] != 0U) {
	  designer.push_back(gsl::narrow<char>(NameDecoder[character]));
	}
	else {
	  break;
	}
  }
  auto const decoded = utf::Utf8ToUtf16(designer);
  designerName.assign(decoded);
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

auto thred::duthrsh(float threshold) noexcept -> uint32_t {
  auto iZoomLevel = 0U;
  if (threshold > 0.0F) {
	auto zoom = 1.0F;
	while (zoom > threshold) {
	  zoom *= ZUMFCT;
	  ++iZoomLevel;
	}
  }
  else {
	return 0;
  }
  return iZoomLevel + 1U;
}

auto thred::internal::unthrsh(wchar_t level) noexcept -> float {
  auto zoom = 1.0F;
  if (level != 0) {
	--level;
  }
  else {
	return 0.0F;
  }
  while (level != 0) {
	zoom *= ZUMFCT;
	--level;
  }
  return zoom;
}

void thred::ritfcor(fPOINT const& point) {
  displayText::butxt(HCOR, fmt::format(L"x{:.0f} y{:.0f}", (point.x * IPFGRAN), (point.y * IPFGRAN)));
}

void thred::internal::ritcor(fPOINTATTR const& pointAttribute) {
  auto const point = fPOINT {pointAttribute.x, pointAttribute.y};
  thred::ritfcor(point);
}

void thred::internal::fndknt() {
  auto endStitch = wrap::toUnsigned(StitchBuffer->size());
  if (endStitch > 4U) {
	endStitch -= 4U;
	// ToDo - Is flag initialized and used correctly?
	auto flag = false;
	KnotCount = 0U;
	for (auto iStitch = 0U; iStitch < endStitch; ++iStitch) {
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
	KnotCount = 0U;
  }
}

auto thred::maxColor() noexcept -> size_t {
  if (!ColorChangeTable->empty()) {
	return ColorChangeTable->size() - 1U;
  }
  return 0U;
}

void thred::resetColorChanges() noexcept {
  ColorChangeTable->clear();
}

void thred::addColor(uint32_t stitch, uint32_t color) {
  ColorChangeTable->push_back(COLCHNG {gsl::narrow<decltype(ColorChangeTable->back().stitchIndex)>(stitch),
                                       gsl::narrow<decltype(ColorChangeTable->back().colorIndex)>(color)});
}

void thred::coltab() {
  if (StitchBuffer->size() > 1) {
	thred::resetColorChanges();
	if (!StitchBuffer->empty()) {
	  auto firstStitch = StitchBuffer->begin();
	  firstStitch->attribute &= NCOLMSK;
	  firstStitch->attribute |= (firstStitch + 1)->attribute & COLMSK;
	  auto lastStitch = StitchBuffer->rbegin();
	  lastStitch->attribute &= NCOLMSK;
	  lastStitch->attribute |= (lastStitch + 1)->attribute & COLMSK;
	  auto currentColor = StitchBuffer->front().attribute & COLMSK;
	  for (auto stitchIt = StitchBuffer->begin() + 1U; stitchIt < StitchBuffer->end() - 1U; ++stitchIt) {
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
	  currentColor     = std::numeric_limits<decltype(currentColor)>::max();
	  auto const range = fRECTANGLE {wrap::toFloat(UnzoomedRect.x) * -1.0F,
	                                 wrap::toFloat(UnzoomedRect.y) * 2.0F,
	                                 wrap::toFloat(UnzoomedRect.x) * 2.0F,
	                                 wrap::toFloat(UnzoomedRect.y) * -1.0F};
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
		  addColor(iStitch, nextColor);
		  ++iColor;
		  currentColor = nextColor;
		}
		++iStitch;
	  }
	  addColor(wrap::toUnsigned(StitchBuffer->size()), 0);
	  if (ClosestPointIndex > wrap::toUnsigned(StitchBuffer->size() - 1U)) {
		ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
	  }
	  thi::fndknt();
	}
  }
}

void thred::internal::ladj() {
  for (auto iLayer = 0U; iLayer < MAXLAYER; ++iLayer) {
	if (iLayer == ActiveLayer) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_GRAYED);
	}
	else {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_ENABLED);
	}
  }
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::deldu() {
  for (auto& undo : *UndoBuffer) {
	undo.reset(nullptr);
  }
  UndoBufferWriteIndex = 0;
  StateMap->reset(StateFlag::BAKWRAP);
  StateMap->reset(StateFlag::BAKACT);
}

void thred::internal::dudat() {
  auto& undoBuffer = *UndoBuffer;
  undoBuffer[UndoBufferWriteIndex].reset(nullptr);
  auto const& formList  = *FormList;
  auto const  formCount = wrap::toUnsigned(formList.size());
  auto const  size      = sizeof(BAKHED) + sizeof(decltype(formList.back())) * formList.size() +
                    sizeof(decltype(StitchBuffer->back())) * StitchBuffer->size() +
                    sizeof(decltype(FormVertices->back())) * FormVertices->size() +
                    sizeof(decltype(ClipPoints->back())) * ClipPoints->size() +
                    sizeof(decltype(SatinGuides->back())) * SatinGuides->size() + sizeof(UserColor) +
                    sizeof(decltype(TexturePointsBuffer->back())) * TexturePointsBuffer->size();
  undoBuffer[UndoBufferWriteIndex] = std::make_unique<uint32_t[]>(size);  // NOLINT (modernize-avoid-c-arrays)
  auto* backupData                 = convert_ptr<BAKHED*>(undoBuffer[UndoBufferWriteIndex].get());
  if (backupData != nullptr) {
	backupData->zoomRect  = UnzoomedRect;
	backupData->formCount = formCount;
	backupData->forms     = convert_ptr<FRMHED*>(&backupData[1]);
	if (formCount != 0) {
	  auto const dest = gsl::span<FRMHED>(backupData->forms, formList.size());
	  std::copy(formList.cbegin(), formList.cend(), dest.begin());
	}
	backupData->stitchCount = wrap::toUnsigned(StitchBuffer->size());
	backupData->stitches    = convert_ptr<fPOINTATTR*>(&backupData->forms[formCount]);
	if (!StitchBuffer->empty()) {
	  auto const dest = gsl::span<fPOINTATTR>(backupData->stitches, StitchBuffer->size());
	  std::copy(StitchBuffer->begin(), StitchBuffer->end(), dest.begin());
	}
	backupData->vertexCount = wrap::toUnsigned(FormVertices->size());
	backupData->vertices    = convert_ptr<fPOINT*>(&backupData->stitches[StitchBuffer->size()]);
	if (!FormVertices->empty()) {
	  auto const dest = gsl::span<fPOINT>(backupData->vertices, FormVertices->size());
	  std::copy(FormVertices->cbegin(), FormVertices->cend(), dest.begin());
	}
	backupData->guideCount = wrap::toUnsigned(SatinGuides->size());
	backupData->guide      = convert_ptr<SATCON*>(&backupData->vertices[FormVertices->size()]);
	if (!SatinGuides->empty()) {
	  auto const dest = gsl::span<SATCON>(backupData->guide, backupData->guideCount);
	  std::copy(SatinGuides->cbegin(), SatinGuides->cend(), dest.begin());
	}
	backupData->clipPointCount = wrap::toUnsigned(ClipPoints->size());
	backupData->clipPoints     = convert_ptr<fPOINT*>(&backupData->guide[SatinGuides->size()]);
	if (!ClipPoints->empty()) {
	  auto const dest = gsl::span<fPOINT>(backupData->clipPoints, backupData->clipPointCount);
	  std::copy(ClipPoints->cbegin(), ClipPoints->cend(), dest.begin());
	}
	backupData->colors = convert_ptr<COLORREF*>(&backupData->clipPoints[ClipPoints->size()]);
	{
	  auto const dest = gsl::span<COLORREF>(backupData->colors, COLORCNT);
	  std::copy(std::begin(UserColor), std::end(UserColor), dest.begin());
	}
	backupData->texturePoints     = convert_ptr<TXPNT*>(&backupData->colors[COLORCNT]);
	backupData->texturePointCount = wrap::toUnsigned(TexturePointsBuffer->size());
	if (!TexturePointsBuffer->empty()) {
	  auto const dest = gsl::span<TXPNT>(backupData->texturePoints, backupData->texturePointCount);
	  std::copy(TexturePointsBuffer->cbegin(), TexturePointsBuffer->cend(), dest.begin());
	}
  }
}

void thred::savdo() {
  StateMap->set(StateFlag::WASDO);
  StateMap->set(StateFlag::CMPDO);
  if (StateMap->testAndReset(StateFlag::SAVACT)) {
	if (StateMap->testAndReset(StateFlag::BAKING)) {
	  StateMap->reset(StateFlag::REDUSHO);
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
	}
	StateMap->set(StateFlag::BAKACT);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
	StateMap->set(StateFlag::DUMEN);
	thi::dudat();
	++UndoBufferWriteIndex;
	UndoBufferWriteIndex &= (UNDOLEN - 1U);
	if (UndoBufferWriteIndex == 0U) {
	  StateMap->set(StateFlag::BAKWRAP);
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
  auto previousName = PreviousNames->begin();
  for (auto iLRU = 0U; iLRU < PreviousNames->size(); ++iLRU) {
	if (!previousName->empty()) {
	  if (StateMap->test(StateFlag::SAVAS)) {
		AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], previousName->wstring().c_str()); // NOLINT(hicpp-signed-bitwise)
	  }
	  else {
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto const fileHandle = FindFirstFile(previousName->c_str(), &findData);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		if (fileHandle == INVALID_HANDLE_VALUE) {
		  previousName->clear();
		}
		else {
		  AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], previousName->wstring().c_str()); // NOLINT(hicpp-signed-bitwise)
		  FindClose(fileHandle);
		}
	  }
	}
	++previousName;
  }
  StateMap->set(StateFlag::DUMEN);
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
  auto flag         = true;
  auto previousName = PreviousNames->begin();
  for (auto iPrevious = 0U; iPrevious < PreviousNames->size(); ++iPrevious) {
	if (*previousName == *ThrName) {
	  if (iPrevious != 0U) {
		std::iter_swap(PreviousNames->begin(), previousName);
		flag = false;
		break;
	  }
	  return;
	}
	++previousName;
  }
  if (flag) {
	previousName = PreviousNames->begin();
	for (auto iPrevious = 0U; iPrevious < PreviousNames->size(); ++iPrevious) {
	  if (previousName->empty()) {
		previousName->assign(*ThrName);
		flag = false;
		break;
	  }
	  ++previousName;
	}
  }
  if (flag) {
	PreviousNames->insert(PreviousNames->begin(), *ThrName);
	PreviousNames->pop_back();
  }
  redfils();
}

void thred::rngadj() {
  auto lastStitch = wrap::toUnsigned(StitchBuffer->size());
  if (lastStitch != 0U) {
	--lastStitch;
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

void thred::internal::box(uint32_t iNearest, HDC dc) {
  auto const boxWidth = BoxOffset[iNearest];
  auto       line     = std::array<POINT, SQPNTS> {};

  line[0] = {NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y - boxWidth};
  line[1] = {NearestPixel[iNearest].x + boxWidth, NearestPixel[iNearest].y - boxWidth};
  line[2] = {NearestPixel[iNearest].x + boxWidth, NearestPixel[iNearest].y + boxWidth};
  line[3] = {NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y + boxWidth};
  line[4] = {NearestPixel[iNearest].x - boxWidth, NearestPixel[iNearest].y - boxWidth};
  wrap::Polyline(dc, line.data(), wrap::toUnsigned(line.size()));
}

void thred::internal::boxs() {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  for (auto iNear = 0U; iNear < NearestCount; ++iNear) {
	SelectObject(StitchWindowDC, BoxPen[iNear]);
	box(iNear, StitchWindowDC);
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unboxs() {
  if (NearestCount != 0U) {
	boxs();
	NearestCount = 0;
  }
}

void thred::internal::stchPars() {
  auto const aspectRatio = wrap::toFloat(UnzoomedRect.x) / wrap::toFloat(UnzoomedRect.y);
  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
	StitchWindowSize.x = std::lround(wrap::toFloat(ThredWindowRect.bottom - ((*ScrollSize) * 2)) * aspectRatio);
  }
  else {
	StitchWindowSize.x = std::lround(wrap::toFloat(ThredWindowRect.bottom - *ScrollSize) * aspectRatio);
  }

  if ((StitchWindowSize.x + ButtonWidthX3 + *ScrollSize + *ColorBarSize) < ThredWindowRect.right) {
	if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
	  StitchWindowSize.y = ThredWindowRect.bottom - (*ScrollSize * 2);
	}
	else {
	  StitchWindowSize.y = ThredWindowRect.bottom - *ScrollSize;
	}
  }
  else {
	StitchWindowSize = {ThredWindowRect.right - ButtonWidthX3 - *ColorBarSize,
	                    ThredWindowRect.bottom - ThredWindowRect.top};
	auto const sizeX = wrap::toFloat(StitchWindowSize.x);
	auto const sizeY = wrap::toFloat(StitchWindowSize.y);
	if ((sizeX / sizeY) > aspectRatio) {
	  StitchWindowSize.x = std::lround(sizeY * aspectRatio);
	}
	else {
	  StitchWindowSize.y = std::lround(sizeX / aspectRatio);
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
  if (StitchWindowMemDC != nullptr) {
	DeleteDC(StitchWindowMemDC);
  }
  if (StitchWindowDC != nullptr) {
	ReleaseDC(MainStitchWin, StitchWindowDC);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  auto clientSize = POINT {(ThredWindowRect.right - ButtonWidthX3 - (*ScrollSize + *ColorBarSize)),
                           (ThredWindowRect.bottom)};
  auto verticalOffset = 0;
  thi::unboxs();
  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
	verticalOffset = *ScrollSize;
	clientSize.y -= *ScrollSize;
  }
  if (StateMap->test(StateFlag::ZUMED)) {
	clientSize.y -= *ScrollSize;
	MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, clientSize.x, clientSize.y, FALSE);
	MoveWindow(VerticalScrollBar, ButtonWidthX3 + clientSize.x, 0, *ScrollSize, clientSize.y, TRUE);
	MoveWindow(HorizontalScrollBar, ButtonWidthX3, clientSize.y + verticalOffset, clientSize.x, *ScrollSize, TRUE);
	StitchWindowAspectRatio = wrap::toFloat(clientSize.x) / wrap::toFloat(clientSize.y);
	if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
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
	StitchWindowAspectRatio = wrap::toFloat(StitchWindowSize.x) / wrap::toFloat(actualWindowHeight);
	if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
	  MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, StitchWindowSize.x, *ScrollSize, TRUE);
	}
  }
  MoveWindow(ColorBar, ThredWindowRect.right - *ColorBarSize, 0, *ColorBarSize, ThredWindowRect.bottom, TRUE);
  thi::nuRct();
  thred::redraw(ColorBar);
}

void thred::unbsho() {
  if (StateMap->testAndReset(StateFlag::BAKSHO)) {
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
  StateMap->reset(StateFlag::PRELIN);
  form::refil();
  thred::coltab();
  form::frmout(ClosestFormToCursor);
}

void thred::internal::patdun() {
  StateMap->reset(StateFlag::RUNPAT);
  KillTimer(ThrEdWindow, 0);
  StateMap->set(StateFlag::WASPAT);
  thred::movStch();
  StateMap->set(StateFlag::RESTCH);
  displayText::tabmsg(IDS_END);
}

auto thred::internal::pxchk(float pixelSize) -> uint16_t {
  constexpr auto MINLIMIT = 1.0F;
  constexpr auto MAXLIMIT = 20.0F;
  if (pixelSize < MINLIMIT) {
	pixelSize = MINLIMIT;
  }
  else if (pixelSize > MAXLIMIT) {
	pixelSize = MAXLIMIT;
  }
  return wrap::round<uint16_t>(pixelSize);
}

void thred::sizstch(fRECTANGLE& rectangle, std::vector<fPOINTATTR>& stitches) noexcept {
  if (!stitches.empty()) {
	rectangle.bottom = rectangle.top = stitches[0].y;
	rectangle.left = rectangle.right = stitches[0].x;
	for (auto& stitch : stitches) {
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
  auto const unzoomedY = wrap::toFloat(UnzoomedRect.y);
  if (ZoomRect.top > unzoomedY) {
	auto const adjustment = ZoomRect.top - unzoomedY;
	ZoomRect.top -= adjustment;
	ZoomRect.bottom -= adjustment;
  }
  if (ZoomRect.bottom < 0) {
	ZoomRect.top -= ZoomRect.bottom;
	ZoomRect.bottom = 0.0F;
  }
  auto const unzoomedX = wrap::toFloat(UnzoomedRect.x);
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

void thred::shft(fPOINT const& delta) noexcept {
  auto const halfZoomRect =
      fPOINT {((ZoomRect.right - ZoomRect.left) * 0.5F), ((ZoomRect.top - ZoomRect.bottom) * 0.5F)};
  auto const center = fPOINT {(ZoomRect.left + halfZoomRect.x), (ZoomRect.bottom + halfZoomRect.y)};
  auto const shift  = fPOINT {(center.x - delta.x), (center.y - delta.y)};
  ZoomRect.bottom -= shift.y;
  ZoomRect.top -= shift.y;
  ZoomRect.left -= shift.x;
  ZoomRect.right -= shift.x;
  thi::zRctAdj();
}

auto thred::internal::stch2px1(uint32_t iStitch) -> POINT {
  if (!StitchBuffer->empty()) {
	return POINT {
	    wrap::ceil<int32_t>((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x),
	    wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
	                        (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y)};
  }
  return POINT {0L, StitchWindowClientRect.bottom};
}

void thred::internal::shft2box() {
  auto const stitchPoint = fPOINT {StitchBuffer->operator[](ClosestPointIndex).x,
                                   StitchBuffer->operator[](ClosestPointIndex).y};
  thred::shft(stitchPoint);
  stch2px1(ClosestPointIndex);
}

void thred::internal::unbox() {
  if (StateMap->testAndReset(StateFlag::SELBOX)) {
	SelectObject(StitchWindowDC, BoxPen[0]);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	wrap::Polyline(StitchWindowDC, StitchArrow.data(), wrap::toUnsigned(StitchArrow.size()));
	SetROP2(StitchWindowDC, R2_COPYPEN);
  }
}

void thred::internal::ilin() noexcept {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_XORPEN);
  wrap::Polyline(StitchWindowDC, &InsertLine[1], LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
  SelectObject(StitchWindowMemDC, LinePen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  wrap::Polyline(StitchWindowMemDC, &InsertLine[1], LNPNTS);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::internal::xlin() {
  if (StateMap->testAndReset(StateFlag::ILIN)) {
	ilin();
  }
}

void thred::internal::ilin1() noexcept {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::xlin1() {
  if (StateMap->testAndReset(StateFlag::ILIN1)) {
	ilin1();
  }
}

void thred::internal::endpnt(POINT const& stitchCoordsInPixels) {
  unbox();
  xlin();
  xlin1();
  StateMap->set(StateFlag::LIN1);
  StateMap->set(StateFlag::INSRT);
  StateMap->reset(StateFlag::GRPSEL);
  InsertLine[0] = stitchCoordsInPixels;
  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
}

void thred::internal::duIns() {
  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
  InsertLine[0] = stch2px1(ClosestPointIndex);
  InsertLine[2] = stch2px1(ClosestPointIndex + 1U);
  xlin();
  StateMap->set(StateFlag::ILIN);
  ilin();
}

void thred::internal::movins() {
  if (StateMap->test(StateFlag::INSRT)) {
	if (StateMap->test(StateFlag::LIN1)) {
	  (StateMap->test(StateFlag::BAKEND)) ? endpnt(stch2px1(wrap::toUnsigned(StitchBuffer->size() - 1U)))
	                                      : endpnt(stch2px1(0U));
	}
	else {
	  duIns();
	}
  }
}

void thred::zumhom() {
  ZoomRect = fRECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.y), wrap::toFloat(UnzoomedRect.x), 0.0F};
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
  auto checkHoopRect = fRECTANGLE {}; // for checking the hoop size
  StateMap->reset(StateFlag::HUPCHNG);
  sizstch(checkHoopRect, *StitchBuffer);
  if (!FormList->empty()) {
	if (StitchBuffer->empty()) {
	  auto const vertexIt  = FormVertices->cbegin();
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
  if (!StitchBuffer->empty() || !FormVertices->empty() || StateMap->test(StateFlag::HUPEX)) {
	if (checkHoopRect.left < 0 || checkHoopRect.right > IniFile.hoopSizeX ||
	    checkHoopRect.bottom < 0 || checkHoopRect.top > IniFile.hoopSizeY) {
	  StateMap->set(StateFlag::HUPEX);
	}
	if (StateMap->test(StateFlag::HUPEX)) {
	  auto const hoopSize =
	      fPOINT {checkHoopRect.right - checkHoopRect.left, checkHoopRect.top - checkHoopRect.bottom};
	  if (hoopSize.x > IniFile.hoopSizeX) {
		IniFile.hoopSizeX = hoopSize.x;
		StateMap->set(StateFlag::HUPCHNG);
	  }
	  if (hoopSize.y > IniFile.hoopSizeY) {
		IniFile.hoopSizeY = hoopSize.y;
		StateMap->set(StateFlag::HUPCHNG);
	  }
	  auto const designCenter =
	      fPOINT {hoopSize.x * 0.5F + checkHoopRect.left, hoopSize.y * 0.5F + checkHoopRect.bottom};
	  auto const hoopCenter = fPOINT {IniFile.hoopSizeX * 0.5F, IniFile.hoopSizeY * 0.5F};
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
	  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
	  ZoomMin      = wrap::toFloat(MINZUM) / wrap::toFloat(UnzoomedRect.x);
	  thred::zumhom();
	}
  }
}

void thred::chkhup() {
  StateMap->set(StateFlag::HUPEX);
  thred::hupfn();
  if (StateMap->test(StateFlag::INIT)) {
	formForms::prfmsg();
  }
  xt::setfchk();
}

void thred::internal::chknum() {
  auto value = 0.0F;
  if (std::wcslen(MsgBuffer.data()) != 0U) {
	value = wrap::wcstof(MsgBuffer.data());
	outDebugString(L"chknum: buffer length [{}]\n", wcslen(MsgBuffer.data()));
  }
  xt::clrstch();
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
  if (MsgIndex != 0U) {
	if (FormMenuChoice != 0U) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  value      = wrap::wcstof(SideWindowEntryBuffer.data()) * PFGRAN;
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
		  value *= IPFGRAN;
		  constexpr auto FUPCLAMP = 255.0F; // clamp the feather up count
		  if (value > FUPCLAMP) {
			value = FUPCLAMP;
		  }
		  form.fillInfo.feather.upCount = wrap::round<uint8_t>(value);
		  break;
		}
		case LFTHCOL: {
		  if (value != 0.0F) {
			thred::savdo();
			form::nufthcol((wrap::wcstol<uint32_t>(SideWindowEntryBuffer.data()) - 1U) & COLORBTS);
			wrap::setSideWinVal(LFTHCOL);
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
			    (wrap::wcstol<uint32_t>(SideWindowEntryBuffer.data()) - 1U) & COLORBTS);
			form::nufilcol(colVal);
			SetWindowText(ValueWindow->operator[](LFRMCOL), fmt::format(L"{}", colVal + 1U).c_str());
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
			    (wrap::wcstol<uint32_t>(SideWindowEntryBuffer.data()) - 1U) & COLORBTS);
			form.underlayColor = colVal;
			SetWindowText(ValueWindow->operator[](LUNDCOL), fmt::format(L"{}", colVal + 1U).c_str());
			form::refilfn();
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
			    (wrap::wcstol<uint32_t>(SideWindowEntryBuffer.data()) - 1U) & COLORBTS);
			form::nubrdcol(colVal);
			SetWindowText(ValueWindow->operator[](LBRDCOL), fmt::format(L"{}", colVal + 1U).c_str());
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
		  wrap::setSideWinVal(LBRDPIC);
		  form::refil();
		  return;
		}
		case LFRMFAZ: {
		  thred::savdo();
		  form.wordParam = wrap::floor<uint32_t>(value * IPFGRAN);
		  thred::unsid();
		  wrap::setSideWinVal(LFRMFAZ);
		  form::refil();
		  return;
		}
		case LBRDPOS: {
		  thred::savdo();
		  form.edgeStitchLen = value * IPFGRAN;
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
			  form.fillInfo.feather.ratio = value * IPFGRAN;
			  break;
			}
			case LFTHNUM: {
			  thred::savdo();
			  form.fillInfo.feather.count = wrap::round<uint16_t>(value * IPFGRAN);
			  break;
			}
			case LFTHFLR: {
			  thred::savdo();
			  form.fillInfo.feather.minStitchSize = value;
			  break;
			}
			case LFTHDWNCNT: {
			  thred::savdo();
			  value *= IPFGRAN;
			  constexpr auto FDNCLAMP = 255.0F; // clamp the feather down count
			  if (value > FDNCLAMP) {
				value = FDNCLAMP;
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
				  form.edgeSpacing = value * 0.5F;
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
			  form.angleOrClipData.angle = value * DEGRADF * IPFGRAN;
			  break;
			}
			case LSACANG: {
			  thred::savdo();
			  form.satinOrAngle.angle = value * DEGRADF * IPFGRAN;
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
		value = wrap::wcstof(SideWindowEntryBuffer.data());
		switch (PreferenceIndex - 1) {
		  case PEG: {
			IniFile.eggRatio = value;
			SetWindowText(ValueWindow->operator[](PEG), fmt::format(L"{:.2f}", value).c_str());
			break;
		  }
		  case PNUDG: {
			IniFile.cursorNudgeStep = value;
			IniFile.nudgePixels     = pxchk(value);
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
			  auto const bufVal = fmt::format(L"{:.2f}", value);
			  switch (PreferenceIndex - 1) {
				case PSPAC: {
				  LineSpacing = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PSPAC), bufVal.c_str());
				  break;
				}
				case PANGL: {
				  IniFile.fillAngle = value * DEGRADF;
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
				  AppliqueColor = wrap::round<uint32_t>(value - 1.0F) % COLORCNT;
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

				  constexpr auto SRMINLIM = 0.01F; // star ratio minimum limit
				  constexpr auto SRMAXLIM = 1.0F;  // star ratio maximum limit
				  if (StarRatio > SRMAXLIM) {
					StarRatio = SRMAXLIM;
				  }
				  if (StarRatio < SRMINLIM) {
					StarRatio = SRMINLIM;
				  }
				  SetWindowText(ValueWindow->operator[](PSTAR), fmt::format(L"{:.2f}", StarRatio).c_str());
				  break;
				}
				case PLRAT: {
				  IniFile.lensRatio = value;

				  constexpr auto LRMINLIM = 0.1F;  // lens ratio minimum limit
				  constexpr auto LRMAXLIM = 10.0F; // lens ratio maximum limit
				  if (IniFile.lensRatio > LRMAXLIM) {
					IniFile.lensRatio = LRMAXLIM;
				  }
				  if (IniFile.lensRatio < LRMINLIM) {
					IniFile.lensRatio = LRMINLIM;
				  }
				  SetWindowText(ValueWindow->operator[](PLRAT),
				                fmt::format(L"{:.2f}", IniFile.lensRatio).c_str());
				  break;
				}
				case PSPIR: {
				  SpiralWrap = value;
				  // ToDo - Are these limits correct?
				  constexpr auto SRMINLIM = 0.3F;  // spiral wrap minimum limit
				  constexpr auto SRMAXLIM = 20.0F; // spiral wrap maximum limit
				  if (SpiralWrap > SRMAXLIM) {
					SpiralWrap = SRMINLIM;
				  }
				  if (SpiralWrap < SRMINLIM) {
					SpiralWrap = SRMINLIM;
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
				  thred::chkhup();
				  break;
				}
				case PHUPY: {
				  IniFile.hoopSizeY = value * PFGRAN;
				  SetWindowText(ValueWindow->operator[](PHUPY), fmt::format(L"{:.0f} mm", value).c_str());
				  form::sethup();
				  formForms::prfmsg();
				  thred::chkhup();
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
				default: {
				  outDebugString(L"default hit in chknum 3: PreferenceIndex [{}]\n", PreferenceIndex - 1);
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
		if (wcslen(MsgBuffer.data()) != 0U) {
		  outDebugString(L"chknum: buffer length [{}]\n", wcslen(MsgBuffer.data()));
		  value = wrap::wcstof(MsgBuffer.data());
		  do {
			if (StateMap->testAndReset(StateFlag::ENTRFNUM)) {
			  if (wrap::round<uint32_t>(value) < FormList->size()) {
				form::frmnumfn(wrap::round<uint32_t>(value));
			  }
			  else {
				displayText::tabmsg(IDS_FRMN1);
			  }
			  return;
			}
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
			                   wrap::toFloat(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY));
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
		  } while (false);
		}
	  }
	}
  }
  StateMap->set(StateFlag::RESTCH);
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
auto thred::internal::oldwnd(HWND window) noexcept -> bool {
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	if (window == DefaultColorWin->operator[](iColor) ||
	    UserColorWin->operator[](iColor) == window || ThreadSizeWin[iColor] == window) {
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

auto CALLBACK thred::internal::EnumChildProc(HWND hwnd, LPARAM lParam) noexcept -> BOOL {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
  displayText::butxt(HUPTO, StringTable->operator[](STR_UPOF));
  if (ZoomFactor == 1) {
	StateMap->reset(StateFlag::ZUMED);
  }
  thred::movStch();
  ShowWindow(MainStitchWin, TRUE);
  thred::unbsho();
  StateMap->reset(StateFlag::SIDACT);
  thred::unsid();
  StateMap->reset(StateFlag::PRFACT);
  StateMap->reset(StateFlag::WASRT);
  DestroyWindow(PreferencesWindow);
  thred::undat();
  if (StateMap->testAndReset(StateFlag::INSFRM)) {
	insadj();
  }
  StateMap->reset(StateFlag::FUNCLP);
  if (StateMap->testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (StateMap->testAndReset(StateFlag::RUNPAT)) {
	patdun();
  }
  StateMap->reset(StateFlag::FORMSEL);
  StateMap->reset(StateFlag::FRMPSEL);
  thred::unmsg();
  SearchLine->clear();
  SearchLine->shrink_to_fit();
  FirstWin = nullptr;
  while (EnumChildWindows(MainStitchWin, thi::EnumChildProc, 0) != 0) { }
}

void thred::ritot(uint32_t number) {
  auto const txt = fmt::format(StringTable->operator[](STR_TOT), number);
  displayText::butxt(HTOT, txt);
}

void thred::internal::frmcalc(uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex) {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	auto const code      = ClosestFormToCursor << FRMSHFT;
	auto const endStitch = StitchBuffer->size() - 1U;
	auto       maxLength = 0.0F;
	auto       minLength = BIGFLOAT;
	if (!StitchBuffer->empty()) {
	  for (auto iStitch = 0U; iStitch < endStitch; ++iStitch) {
		auto const& stitch     = StitchBuffer->operator[](iStitch);
		auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
		if ((stitch.attribute & FRMSK) == code && ((stitch.attribute & NOTFRM) == 0U) &&
		    (stitchFwd1.attribute & FRMSK) == code && ((stitchFwd1.attribute & TYPMSK) != 0U)) {
		  auto const length = std::hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
		  if (length > maxLength) {
			maxLength          = length;
			largestStitchIndex = iStitch;
		  }
		  if (length < minLength) {
			minLength           = length;
			smallestStitchIndex = iStitch;
		  }
		}
	  }
	}
	constexpr auto DIGITLIM = 10000.0F; // value that represents the max width that can be displayed
	if (fabs(maxLength) < DIGITLIM) {
	  auto strMax = std::wstring {};
	  displayText::loadString(strMax, IDS_LENMAX);
	  displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength * IPFGRAN)));
	}
	if (fabs(minLength) < DIGITLIM) {
	  auto strMin = std::wstring {};
	  displayText::loadString(strMin, IDS_LENMIN);
	  displayText::butxt(HMINLEN, fmt::format(strMin, (minLength * IPFGRAN)));
	}
  }
  else {
	auto const blank = std::wstring {};
	displayText::butxt(HMAXLEN, blank);
	displayText::butxt(HMINLEN, blank);
  }
}

void thred::internal::lenfn(uint32_t start, uint32_t end, uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex) {
  auto maxLength      = 0.0F;
  auto minLength      = BIGFLOAT;
  smallestStitchIndex = 0U;
  largestStitchIndex  = 0U;
  for (auto iStitch = start; iStitch < end; ++iStitch) {
	auto const& stitch     = StitchBuffer->operator[](iStitch);
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	auto const length      = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
	if (length > maxLength) {
	  maxLength          = length;
	  largestStitchIndex = iStitch;
	}
	if (length < minLength) {
	  minLength           = length;
	  smallestStitchIndex = iStitch;
	}
  }
  auto strMax = std::wstring {};
  auto strMin = std::wstring {};
  displayText::loadString(strMax, IDS_LENMAX);
  displayText::loadString(strMin, IDS_LENMIN);
  displayText::butxt(HMAXLEN, fmt::format(strMax, (maxLength * IPFGRAN)));
  displayText::butxt(HMINLEN, fmt::format(strMin, (minLength * IPFGRAN)));
}

void thred::internal::lenCalc() {
  auto const blank = std::wstring {};
  if (StateMap->test(StateFlag::LENSRCH)) {
	auto        txt        = std::wstring {};
	auto const& stitch     = StitchBuffer->operator[](ClosestPointIndex);
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
	auto const lenMax      = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y) * IPFGRAN;
	displayText::butxt(HMINLEN, fmt::format(L"{:.2f}", lenMax));
	displayText::loadString(txt, IDS_SRCH);
	displayText::butxt(HMAXLEN, txt);
  }
  else {
	if (StitchBuffer->size() > 1U) {
	  if (StateMap->test(StateFlag::FORMSEL)) {
		frmcalc(LargestStitchIndex, SmallestStitchIndex);
		displayText::butxt(HCOR, blank);
		return;
	  }
	  thred::rngadj();
	  if (StateMap->test(StateFlag::GRPSEL) && GroupStartStitch != GroupEndStitch) {
		lenfn(GroupStartStitch, GroupEndStitch - 1U, LargestStitchIndex, SmallestStitchIndex);
	  }
	  else {
		lenfn(0, wrap::toUnsigned(StitchBuffer->size() - 2U), LargestStitchIndex, SmallestStitchIndex);
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
  auto       stitchSize = BIGFLOAT; // to ensure that it is larger than SmallStitchLength first time through
  thred::savdo();
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto iPrevStitch = form::find1st();
	auto iStitch     = iPrevStitch + 1U;
	auto lastStitch  = StitchBuffer->size();
	if (!StitchBuffer->empty()) { // find the first small stitch in the selected form
	  --lastStitch;
	  while (iStitch < lastStitch && stitchSize > SmallStitchLength) {
		auto const& stitch     = StitchBuffer->operator[](iStitch);
		auto const& prevStitch = StitchBuffer->operator[](iPrevStitch);
		if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == codedAttribute) { // are we still in the selected form?
		  if ((stitch.attribute & KNOTMSK) == 0U) { // is this a knot?
			auto const delta = fPOINT {stitch.x - prevStitch.x, stitch.y - prevStitch.y};
			stitchSize       = hypot(delta.x, delta.y);
			if (stitchSize > SmallStitchLength) {
			  ++iPrevStitch;
			  ++iStitch;
			}
		  }
		}
		else {
		  return; // we reached the last stitch in the selected form without seeing a small stitch so don't do anything
		}
	  }
	}
	if (iStitch != lastStitch) {
	  --iStitch; // we found a small stich so back up to the start point
	}
	else {
	  return; // we reached the last stitch without seeing a small stitch so don't do anything
	}
	auto iOutputStitch = iStitch;
	auto prevPoint     = StitchBuffer->operator[](startStitch);
	lastStitch         = StitchBuffer->size();
	if (lastStitch != 0U) {
	  --lastStitch;
	  while (iStitch < lastStitch) {
		auto const& stitch = StitchBuffer->operator[](iStitch);
		auto& outStitch    = StitchBuffer->operator[](iOutputStitch);
		if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == codedAttribute) { // are we still in the selected form?
		  if ((stitch.attribute & KNOTMSK) != 0U) { // is this a knot?
			prevPoint = outStitch;
			outStitch = stitch;
			++iOutputStitch;
		  }
		  else {
			auto const delta = fPOINT {stitch.x - prevPoint.x, stitch.y - prevPoint.y};
			stitchSize       = hypot(delta.x, delta.y);
			if (stitchSize > SmallStitchLength) {
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
	auto prevPoint   = StitchBuffer->operator[](startStitch);
	for (auto iStitch = iNextStitch; iStitch < endStitch; ++iStitch) {
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
	StitchBuffer->erase(wrap::next(StitchBuffer->begin(), iNextStitch),
	                    wrap::next(StitchBuffer->begin(), endStitch));
	thred::coltab();
  }
  rstAll();
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  lenCalc();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::duzero() {
  if (!SelectedFormList->empty()) {
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto selectedForm : (*SelectedFormList)) {
	  formMap.set(selectedForm);
	}
	StateMap->reset(StateFlag::CONTIG);
	auto iDestination  = StitchBuffer->begin();
	auto currentStitch = StitchBuffer->front();
	for (auto& iStitch : *StitchBuffer) {
	  if (((iStitch.attribute & TYPMSK) != 0U) && formMap.test((iStitch.attribute & FRMSK) >> FRMSHFT)) {
		if (StateMap->testAndSet(StateFlag::CONTIG)) {
		  auto const stitchLength = hypot(iStitch.x - currentStitch.x, iStitch.y - currentStitch.y);
		  if (stitchLength > MinStitchLength) {
			currentStitch   = iStitch;
			(*iDestination) = iStitch;
			++iDestination;
		  }
		}
		else {
		  currentStitch = iStitch;
		}
	  }
	  else {
		(*iDestination) = iStitch;
		++iDestination;
		StateMap->reset(StateFlag::CONTIG);
	  }
	}
	StitchBuffer->erase(iDestination, StitchBuffer->end());
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	delsmal(GroupStartStitch, GroupEndStitch);
  }
  else {
	delsmal(0, wrap::toUnsigned(StitchBuffer->size()));
  }
}

void thred::internal::rshft(POINT const& shiftPoint) {
  auto const shiftX = wrap::toFloat(shiftPoint.x);
  auto const shiftY = wrap::toFloat(shiftPoint.y);
  ZoomRect.top -= shiftY;
  ZoomRect.left -= shiftX;
  ZoomRect.right -= shiftX;
  ZoomRect.bottom -= shiftY;
  zRctAdj();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::pgdwn() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, std::lround((ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	rshft(scrollPosition);
  }
}

void thred::internal::pgup() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {0, std::lround(-(ZoomRect.top - ZoomRect.bottom) * PAGSCROL)};
	rshft(scrollPosition);
  }
}

void thred::internal::pglft() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {std::lround((ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
	rshft(scrollPosition);
  }
}

void thred::internal::pgrit() {
  if (StateMap->test(StateFlag::ZUMED)) {
	auto const scrollPosition = POINT {std::lround(-(ZoomRect.right - ZoomRect.left) * PAGSCROL), 0};
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
	for (auto iStitch = start; iStitch <= end; ++iStitch) {
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
  auto const armLength            = BoxOffset[0];
  auto const stitchCoordsInPixels = stch2px1(iStitch);
  InsertLine[0]                   = {stitchCoordsInPixels.x - armLength, stitchCoordsInPixels.y};
  InsertLine[1]                   = {stitchCoordsInPixels.x + armLength, stitchCoordsInPixels.y};
  SelectObject(StitchWindowDC, CrossPen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SelectObject(StitchWindowMemDC, CrossPen);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  InsertLine[0] = {stitchCoordsInPixels.x, stitchCoordsInPixels.y - armLength};
  InsertLine[1] = {stitchCoordsInPixels.x, stitchCoordsInPixels.y - 1};
  wrap::Polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  wrap::Polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  InsertLine[0].y = stitchCoordsInPixels.y + 2;
  InsertLine[1].y = stitchCoordsInPixels.y + armLength;
  wrap::Polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
  wrap::Polyline(StitchWindowMemDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void thred::internal::uncros() {
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

void thred::internal::ducros(HDC dc) {
  uncros();
  StateMap->set(StateFlag::SCROS);
  cros(GroupStartStitch);
  if (GroupStartStitch != GroupEndStitch) {
	cros(GroupEndStitch);
	StateMap->set(StateFlag::ECROS);
  }
  selin(GroupStartStitch, GroupEndStitch, dc);
}

void thred::selRct(fRECTANGLE& sourceRect) noexcept {
  if (!StitchBuffer->empty()) {
	sourceRect.left = sourceRect.right = StitchBuffer->operator[](GroupStartStitch).x;
	sourceRect.top = sourceRect.bottom = StitchBuffer->operator[](GroupStartStitch).y;
	for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
	  auto const& stitch = StitchBuffer->operator[](iStitch);
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
	++sourceRect.right;
	--sourceRect.left;
  }
  if (sourceRect.top - sourceRect.bottom == 0) {
	++sourceRect.top;
	--sourceRect.bottom;
  }
}

void thred::internal::dusel(HDC dc) {
  SetROP2(dc, R2_NOTXORPEN);
  SelectObject(dc, LinePen);
  wrap::Polyline(dc, FormControlPoints->data(), wrap::toUnsigned(FormControlPoints->size() - 1U));
  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(FormControlPoints->size() - 1U); ++iPoint) {
	form::selsqr(FormControlPoints->operator[](iPoint), dc);
  }
  SetROP2(dc, R2_COPYPEN);
}

void thred::internal::unsel() {
  if (StateMap->testAndReset(StateFlag::SELSHO)) {
	dusel(StitchWindowDC);
  }
}

void thred::internal::nuselrct() {
  auto outline = std::vector<fPOINT> {};
  outline.resize(FormControlPoints->size());
  unsel();
  outline[0].x = outline[6].x = outline[7].x = outline[8].x = StitchRangeRect.left;
  outline[1].x = outline[5].x = wrap::midl(StitchRangeRect.right, StitchRangeRect.left);
  outline[0].y = outline[1].y = outline[2].y = outline[8].y = StitchRangeRect.top;
  outline[3].y = outline[7].y = wrap::midl(StitchRangeRect.top, StitchRangeRect.bottom);
  outline[4].y = outline[5].y = outline[6].y = StitchRangeRect.bottom;
  outline[2].x = outline[3].x = outline[4].x = StitchRangeRect.right;

  auto iPoint = outline.begin();
  for (auto& controlPoint : *FormControlPoints) {
	controlPoint = form::sfCor2px(*iPoint);
	++iPoint;
  }
}

void thred::grpAdj() {
  thi::uncros();
  thi::unsel();
  thred::rngadj();
  thi::ducros(StitchWindowDC);
  thi::lenCalc();
  thred::selRct(StitchRangeRect);
  if (StateMap->test(StateFlag::ZUMED) && GroupEndStitch != GroupStartStitch) {
	if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1 ||
	    StitchRangeRect.left < ZoomRect.left + 1 || StitchRangeRect.right > ZoomRect.right - 1) {
	  auto newSize = fPOINT {std::round(StitchRangeRect.right - StitchRangeRect.left),
	                         std::round(StitchRangeRect.top - StitchRangeRect.bottom)};
	  if (newSize.x < MINZUM) {
		if (newSize.x < 1.0F) {
		  newSize.x = 1.0F;
		}
		auto const coordinate = wrap::toFloat(MINZUM) / newSize.x;
		newSize               = fPOINT {wrap::toFloat(MINZUM), std::round(coordinate * newSize.y)};
	  }
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
	  if (newSize.x > wrap::toFloat(UnzoomedRect.x) || newSize.y > wrap::toFloat(UnzoomedRect.y)) {
		ZoomRect.left = ZoomRect.bottom = 0.0F;
		ZoomRect.right                  = wrap::toFloat(UnzoomedRect.x);
		ZoomRect.top                    = wrap::toFloat(UnzoomedRect.y);
		StateMap->reset(StateFlag::ZUMED);
		ZoomFactor = 1.0;
		thred::movStch();
	  }
	  else {
		ZoomRect.right = ZoomRect.left + newSize.x;
		ZoomFactor     = newSize.x / wrap::toFloat(UnzoomedRect.x);
		ZoomRect.top   = ZoomRect.bottom + newSize.y;
		auto const stitchPoint =
		    fPOINT {((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left,
		            ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom};
		thred::shft(stitchPoint);
	  }
	}
  }
  StateMap->set(StateFlag::RESTCH);
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
	auto const stitchPoint =
	    fPOINT {((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left,
	            ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom};
	thred::shft(stitchPoint);
  }
  nuAct(GroupStartStitch);
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::ritlayr() {
  auto layer = std::numeric_limits<uint32_t>::max();

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
  if ((layer & MAXMSK) != 0U) {
	auto const blank = std::wstring {};
	displayText::butxt(HLAYR, blank);
  }
  else {
	auto const txt = fmt::format(StringTable->operator[](STR_LAYR), layer);
	displayText::butxt(HLAYR, txt);
  }
}

auto thred::nuPen(HPEN pen, int32_t width, COLORREF color) noexcept -> HPEN {
  DeleteObject(pen);
  return wrap::CreatePen(PS_SOLID, width, color);
}

void thred::internal::nuStchSiz(uint32_t iColor, int32_t width) noexcept {
  if (width != ThreadSizePixels[iColor]) {
	UserPen[iColor]          = nuPen(UserPen[iColor], width, UserColor[iColor]);
	ThreadSizePixels[iColor] = width;
  }
}

auto thred::internal::nuBrush(HBRUSH brush, COLORREF color) noexcept -> HBRUSH {
  DeleteObject(brush);
  return CreateSolidBrush(color);
}

void thred::internal::rotpix(POINT const& unrotatedPoint, POINT& rotatedPoint, POINT const& rotationCenterPixels) noexcept {
  // won't handle vertical lines
  auto const dx               = wrap::toFloat(unrotatedPoint.x - rotationCenterPixels.x);
  auto const dy               = wrap::toFloat(unrotatedPoint.y - rotationCenterPixels.y);
  auto const distanceToCenter = hypotf(dx, dy);
  auto const newAngle         = atan2f(dy, dx) - RotateAngle;
  rotatedPoint = {std::lround(wrap::toFloat(rotationCenterPixels.x) + distanceToCenter * cos(newAngle)),
                  std::lround(wrap::toFloat(rotationCenterPixels.y) + distanceToCenter * sin(newAngle))};
}

void thred::internal::duar(POINT const& stitchCoordsInPixels) {
  auto const offset = MulDiv(10, *screenDPI, STDDPI);
  auto arrowCenter  = POINT {(stitchCoordsInPixels.x - offset), (stitchCoordsInPixels.y + offset)};
  StitchArrow[1]    = stitchCoordsInPixels;
  rotpix(arrowCenter, StitchArrow[0], stitchCoordsInPixels);
  arrowCenter.y = stitchCoordsInPixels.y - offset;
  rotpix(arrowCenter, StitchArrow[2], stitchCoordsInPixels);
  SelectObject(StitchWindowMemDC, BoxPen[0]);
  SelectObject(StitchWindowDC, BoxPen[0]);
  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowMemDC, StitchArrow.data(), wrap::toUnsigned(StitchArrow.size()));
  wrap::Polyline(StitchWindowDC, StitchArrow.data(), wrap::toUnsigned(StitchArrow.size()));
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::dubox(POINT const& stitchCoordsInPixels) {
  if (!StitchBuffer->empty()) {
	auto const& stitch = StitchBuffer->operator[](ClosestPointIndex);
	if (ClosestPointIndex != (StitchBuffer->size() - 1U)) {
	  // if the selected point is not at the end then aim at the next point
	  auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
	  RotateAngle            = atan2f(stitchFwd1.y - stitch.y, stitchFwd1.x - stitch.x);
	}
	else { // otherwise aim in the same direction
	  auto const& stitchBck1 = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) - 1U);
	  RotateAngle            = atan2f(stitch.y - stitchBck1.y, stitch.x - stitchBck1.x);
	}
	duar(stitchCoordsInPixels);
	StateMap->reset(StateFlag::ELIN);
	StateMap->set(StateFlag::SELBOX);
	StateMap->reset(StateFlag::FRMPSEL);
	thred::redraw(ColorBar);
	displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
  }
}

auto thred::internal::stch2px(uint32_t iStitch, POINT& stitchCoordsInPixels) -> bool {
  stitchCoordsInPixels = {
      wrap::ceil<int32_t>((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x),
      wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                          (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y)};
  return stitchCoordsInPixels.x >= 0 && stitchCoordsInPixels.x <= StitchWindowClientRect.right &&
         stitchCoordsInPixels.y >= 0 && stitchCoordsInPixels.y <= StitchWindowClientRect.bottom;
}

auto thred::internal::stch2px2(uint32_t iStitch) -> bool {
  auto const stitchCoordsInPixels =
      POINT {wrap::ceil<int32_t>((StitchBuffer->operator[](iStitch).x - ZoomRect.left) * ZoomRatio.x),
             wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                                 (StitchBuffer->operator[](iStitch).y - ZoomRect.bottom) * ZoomRatio.y)};
  return stitchCoordsInPixels.x >= 0 && stitchCoordsInPixels.x <= StitchWindowClientRect.right &&
         stitchCoordsInPixels.y >= 0 && stitchCoordsInPixels.y <= StitchWindowClientRect.bottom;
}

auto thred::stch2pxr(fPOINT const& stitchCoordinate) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                                    (stitchCoordinate.y - ZoomRect.bottom) * ZoomRatio.y)};
}

void thred::internal::getDocsFolder(fs::path* directory) {
  if (directory != nullptr) {
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto       ppszPath = PWSTR {nullptr}; // variable to receive the path memory block pointer.
	auto const hr       = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &ppszPath);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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

void thred::internal::ritini() {
  auto const     directory        = utf::Utf16ToUtf8(DefaultDirectory->wstring());
  auto&          defDir           = IniFile.defaultDirectory;
  auto const     defaultDirectory = gsl::span<char> {defDir};
  constexpr char FILLCHAR         = '\0';
  std::fill(defaultDirectory.begin(), defaultDirectory.end(), FILLCHAR);
  std::copy(directory.cbegin(), directory.cend(), defaultDirectory.begin());
  auto previousName = PreviousNames->begin();
  for (auto& prevName : IniFile.prevNames) {
	auto const name = gsl::span<char> {prevName};
	std::fill(name.begin(), name.end(), FILLCHAR);
	if (!previousName->empty()) {
	  auto previous = utf::Utf16ToUtf8(*previousName);
	  std::copy(previous.cbegin(), previous.cend(), name.begin());
	}
	++previousName;
  }
  auto const designer     = utf::Utf16ToUtf8(*DesignerName);
  auto&      desName      = IniFile.designerName;
  auto const designerName = gsl::span<char> {desName};
  std::fill(designerName.begin(), designerName.end(), FILLCHAR);
  std::copy(designer.cbegin(), designer.cend(), designerName.begin());
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	IniFile.stitchColors[iColor]              = UserColor[iColor];
	IniFile.backgroundPreferredColors[iColor] = CustomBackgroundColor[iColor];
	IniFile.stitchPreferredColors[iColor]     = CustomColor[iColor];
	IniFile.bitmapBackgroundColors[iColor]    = bitmap::getBmpBackColor(iColor);
  }
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
	auto windowRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(ThrEdWindow, &windowRect);
	IniFile.initialWindowCoords.left   = windowRect.left;
	IniFile.initialWindowCoords.right  = windowRect.right;
	IniFile.initialWindowCoords.bottom = windowRect.bottom;
	IniFile.initialWindowCoords.top    = windowRect.top;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const iniFileHandle = CreateFile(
      IniFileName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT(hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (iniFileHandle != INVALID_HANDLE_VALUE) {
	auto bytesRead = DWORD {0};
	WriteFile(iniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, nullptr);
	// ToDo - Check that file is wriiten completely
  }
  CloseHandle(iniFileHandle);
}

auto thred::internal::savcmp() noexcept -> bool {
#ifdef _DEBUG
  return true;
#else
  return !StateMap->test(StateFlag::CMPDO);
#endif
}

void thred::internal::thr2bal(std::vector<BALSTCH>& balaradStitch, uint32_t source, uint8_t code, uint8_t flag) {
  balaradStitch.push_back(BALSTCH {code,
                                   flag,
                                   (StitchBuffer->operator[](source).x - BalaradOffset.x) * BALRATIO,
                                   (StitchBuffer->operator[](source).y - BalaradOffset.y) * BALRATIO});
}

void thred::internal::redbal() {
  auto fileSize = uintmax_t {0};
  if (getFileSize(*BalaradName2, fileSize)) {
	auto balaradHeader = BALHED {};
	StitchBuffer->clear();
	FormList->clear();
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const balaradFile =
	    CreateFile(BalaradName2->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (balaradFile != INVALID_HANDLE_VALUE) {
	  auto bytesRead = DWORD {0U};
	  ReadFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesRead, nullptr);
	  if (bytesRead == sizeof(balaradHeader)) {
		auto       balaradStitch = std::vector<BALSTCH> {};
		auto const newSize = (fileSize - sizeof(balaradHeader)) / sizeof(decltype(balaradStitch.back()));
		balaradStitch.resize(gsl::narrow<size_t>(newSize));
		ReadFile(balaradFile, balaradStitch.data(), gsl::narrow<DWORD>(fileSize - sizeof(balaradHeader)), &bytesRead, nullptr);
		auto const stitchCount  = bytesRead / sizeof(decltype(balaradStitch.back()));
		BackgroundColor         = balaradHeader.backgroundColor;
		IniFile.backgroundColor = balaradHeader.backgroundColor;
		BackgroundPen           = nuPen(BackgroundPen, 1, BackgroundColor);
		BackgroundPenWidth      = 1;
		DeleteObject(BackgroundBrush);
		BackgroundBrush        = CreateSolidBrush(BackgroundColor);
		constexpr auto IBALRAT = 6.0F / 10.0F; // Inverse balarad stitch size ratio
		IniFile.hoopSizeX      = balaradHeader.hoopSizeX * IBALRAT;
		IniFile.hoopSizeY      = balaradHeader.hoopSizeY * IBALRAT;
		UnzoomedRect           = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
		BalaradOffset.x        = IniFile.hoopSizeX * 0.5F;
		BalaradOffset.y        = IniFile.hoopSizeY * 0.5F;
		IniFile.hoopType       = CUSTHUP;
		UserColor.fill(0);
		UserColor[0] = balaradHeader.color[0];
		auto color   = 0U;
		auto bColor  = 1U;
		thred::addColor(0, color);
		for (auto iStitch = 0U; iStitch < stitchCount; ++iStitch) {
		  switch (balaradStitch[iStitch].code) {
			case BALNORM: {
			  StitchBuffer->push_back(fPOINTATTR {balaradStitch[iStitch].x * IBALRAT + BalaradOffset.x,
			                                      balaradStitch[iStitch].y * IBALRAT + BalaradOffset.y,
			                                      color});
			  break;
			}
			case BALSTOP: {
			  color                    = DST::colmatch(balaradHeader.color[bColor++]);
			  auto const currentStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
			  thred::addColor(currentStitch, color);
			  break;
			}
			default: {
			  outDebugString(L"default hit in redbal: code [{}]\n", balaradStitch[iStitch].code);
			  break;
			}
		  }
		}
		for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
		  UserPen[iColor]        = wrap::CreatePen(PS_SOLID, PENNWID, UserColor[iColor]);
		  UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
		}
		thred::coltab();
		thred::redraw(ColorBar);
		StateMap->set(StateFlag::INIT);
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	CloseHandle(balaradFile);
  }
}

void thred::internal::ritbal() {
  auto balaradHeader = BALHED {};
  if (!BalaradName0->empty() && !BalaradName1->empty() && (!StitchBuffer->empty())) {
	auto outputName = (WorkingFileName->empty()) ? (*DefaultDirectory / L"balfil.thr") : *WorkingFileName;
	outputName.replace_extension(L".thv");
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto balaradFile =
	    CreateFile(outputName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (balaradFile == INVALID_HANDLE_VALUE) {
	  return;
	}
	auto color             = gsl::narrow_cast<uint8_t>(StitchBuffer->front().attribute & COLMSK);
	balaradHeader.color[0] = UserColor[color];
	auto iColor            = 1U;
	for (auto& stitch : *StitchBuffer) {
	  if (color != (stitch.attribute & COLMSK)) {
		color                         = stitch.attribute & COLMSK;
		balaradHeader.color[iColor++] = UserColor[color];
		if ((iColor & NCOLMSK) != 0U) {
		  break;
		}
	  }
	}
	// NOLINTNEXTLINE(clang-diagnostic-four-char-constants)
	balaradHeader.signature       = 'drbm';
	balaradHeader.backgroundColor = BackgroundColor;
	balaradHeader.hoopSizeX       = IniFile.hoopSizeX * BALRATIO;
	balaradHeader.hoopSizeY       = IniFile.hoopSizeY * BALRATIO;
	auto bytesWritten             = DWORD {0};
	WriteFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesWritten, nullptr);
	BalaradOffset.x    = IniFile.hoopSizeX * 0.5F;
	BalaradOffset.y    = IniFile.hoopSizeY * 0.5F;
	auto balaradStitch = std::vector<BALSTCH> {};
	balaradStitch.reserve(StitchBuffer->size() + 2U);
	color        = StitchBuffer->front().attribute & COLMSK;
	auto iOutput = 0U;
	thr2bal(balaradStitch, 0, BALJUMP, 0);
	++iOutput;
	for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	  thr2bal(balaradStitch, iStitch, BALNORM, 0);
	  ++iOutput;
	  if ((StitchBuffer->operator[](iStitch).attribute & COLMSK) != color) {
		color = StitchBuffer->operator[](iStitch).attribute & COLMSK;
		thr2bal(balaradStitch, iStitch, BALSTOP, color);
		++iOutput;
	  }
	}
	WriteFile(balaradFile, balaradStitch.data(), iOutput * sizeof(decltype(balaradStitch.back())), &bytesWritten, nullptr);
	CloseHandle(balaradFile);
	balaradFile =
	    CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	auto const outString = utf::Utf16ToUtf8(outputName.wstring());
	wrap::WriteFile(balaradFile, outString.c_str(), wrap::toUnsigned(outputName.wstring().size()) + 1U, &bytesWritten, nullptr);
	CloseHandle(balaradFile);
  }
  else {
	if (!BalaradName1->empty()) {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const balaradFile =
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
  constexpr auto WRBITS = (fs::perms::owner_write | fs::perms::others_write | fs::perms::group_write); // Write permissions bits
  if (fs::space(filename.parent_path()).available == 0U) {
	StateMap->set(StateFlag::NOTFREE);
	return true;
  }
  StateMap->reset(StateFlag::NOTFREE);
  auto const writePerms = fs::status(filename).permissions() & WRBITS;
  if (writePerms == fs::perms::none) {
	auto const buttonPressed =
	    MessageBox(ThrEdWindow,
	               fmt::format(StringTable->operator[](STR_OVRLOK), filename.wstring()).c_str(),
	               StringTable->operator[](STR_OVRIT).c_str(),
	               MB_YESNO);
	if (buttonPressed == IDYES) {
	  fs::permissions(filename, WRBITS, fs::perm_options::add);
	}
	else {
	  return true;
	}
  }
  return false;
}

void thred::internal::duver(fs::path const& name) {
  auto const version = wrap::toSize(tolower(name.extension().wstring().back()) - 'r');
  if (version < VersionNames->size()) {
	VersionNames->operator[](version) = name;
  }
}

void thred::internal::durit(std::vector<char>& destination, const void* source, uint32_t count) {
  if (source != nullptr) {
	auto const src =
	    gsl::span<char const> {gsl::narrow_cast<char const*>(source), gsl::narrow_cast<size_t>(count)};
	destination.insert(destination.end(), src.begin(), src.end());
  }
}

void thred::internal::dubuf(std::vector<char>& buffer) {
  auto stitchHeader  = STRHED {};
  auto vertexCount   = 0U;
  auto guideCount    = 0U;
  auto clipDataCount = 0U;

  constexpr auto HDRVERS = 0x02U; // ThrEd file version signature

  stitchHeader.headerType = HDRVERS << TBYTSHFT | THREDSIG;
  stitchHeader.fileLength = wrap::toUnsigned(StitchBuffer->size() * sizeof(decltype(StitchBuffer->back())) +
                                             sizeof(stitchHeader) + bitmap::getBmpNameLength());
  wrap::narrow(stitchHeader.stitchCount, StitchBuffer->size());
  wrap::narrow_cast(stitchHeader.hoopType, IniFile.hoopType);
  auto       designer     = utf::Utf16ToUtf8(*DesignerName);
  auto&      modName      = ExtendedHeader->modifierName;
  auto const modifierName = gsl::span<char> {modName};
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
  wrap::narrow(stitchHeader.formCount, FormList->size());
  wrap::narrow(stitchHeader.vertexCount, vertexCount);
  wrap::narrow(stitchHeader.dlineCount, guideCount);
  wrap::narrow(stitchHeader.clipDataCount, clipDataCount);
  auto const formDataOffset = bitmap::getBmpNameLength() + sizeof(BackgroundColor) +
                              sizeof(UserColor) + sizeof(CustomColor) + TSSIZE;
  auto       vtxLen        = sizeof(stitchHeader) + StitchBuffer->size() * sizeof(decltype(StitchBuffer->back())) + formDataOffset;
  auto const thredDataSize = FormList->size() * sizeof(decltype(FormList->back())) +
                             vertexCount * sizeof(decltype(FormVertices->back())) +
                             guideCount * sizeof(decltype(SatinGuides->back())) +
                             clipDataCount * sizeof(decltype(ClipPoints->back())) +
                             TexturePointsBuffer->size() * sizeof(decltype(TexturePointsBuffer->back()));
  buffer.reserve(vtxLen + thredDataSize);
  // ToDo - vertexLength overflows a 16 bit integer if there are more than 5446 stitches, so clamp it until version 3
  constexpr auto VTXCLAMP = std::numeric_limits<decltype(stitchHeader.vertexLen)>::max();
  if (vtxLen > VTXCLAMP) {
	vtxLen = VTXCLAMP;
  }
  wrap::narrow(stitchHeader.vertexLen, vtxLen);
  wrap::narrow(stitchHeader.dlineLen, sizeof(decltype(FormVertices->back())) * vertexCount);
  wrap::narrow(stitchHeader.clipDataLen, sizeof(decltype(ClipPoints->back())) * clipDataCount);
  durit(buffer, &stitchHeader, sizeof(stitchHeader));
  ExtendedHeader->auxFormat         = IniFile.auxFileType;
  ExtendedHeader->hoopSizeX         = IniFile.hoopSizeX;
  ExtendedHeader->hoopSizeY         = IniFile.hoopSizeY;
  ExtendedHeader->texturePointCount = wrap::toUnsigned(TexturePointsBuffer->size());
  durit(buffer, ExtendedHeader, sizeof(*ExtendedHeader));
  durit(buffer,
        StitchBuffer->data(),
        wrap::toUnsigned(StitchBuffer->size() * sizeof(decltype(StitchBuffer->back()))));
  durit(buffer, bitmap::getBmpNameData(), bitmap::getBmpNameLength());
  durit(buffer, &BackgroundColor, sizeof(BackgroundColor));
  durit(buffer, UserColor.data(), wrap::toUnsigned(UserColor.size() * sizeof(decltype(UserColor.back()))));
  durit(buffer,
        CustomColor.data(),
        wrap::toUnsigned(CustomColor.size() * sizeof(decltype(CustomColor.back()))));
  auto threadSizeBuffer = std::string {};
  threadSizeBuffer.resize(TSSIZE);
  for (auto iThread = 0U; iThread < TSSIZE; ++iThread) {
	threadSizeBuffer[iThread] = gsl::narrow<char>(ThreadSize[iThread]);
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
	  auto vertexIt = wrap::next(FormVertices->cbegin(), srcForm.vertexIndex);
	  for (auto iVertex = 0U; iVertex < srcForm.vertexCount; ++iVertex) {
		vertices.push_back(*vertexIt);
		++vertexIt;
	  }
	  if (srcForm.type == SAT) {
		wrap::narrow(outForms.back().satinGuideCount, srcForm.satinGuideCount);
		auto guideIt = wrap::next(SatinGuides->cbegin(), srcForm.satinOrAngle.guide);
		for (auto iGuide = 0U; iGuide < srcForm.satinGuideCount; ++iGuide) {
		  guides.emplace_back(*guideIt);
		  ++guideIt;
		}
	  }
	  if (clip::isclp(srcForm)) {
		auto offsetStart = wrap::next(ClipPoints->cbegin(), srcForm.angleOrClipData.clip);
		for (auto iClip = 0U; iClip < srcForm.lengthOrCount.clipCount; ++iClip) {
		  points.push_back(*offsetStart);
		  ++offsetStart;
		}
	  }
	  if (clip::iseclpx(srcForm)) {
		auto       offsetStart = wrap::next(ClipPoints->cbegin(), srcForm.borderClipData);
		auto const clipCount   = srcForm.clipEntries;
		for (auto iClip = 0U; iClip < clipCount; ++iClip) {
		  points.push_back(*offsetStart);
		  ++offsetStart;
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
	        wrap::toUnsigned(TexturePointsBuffer->size() * sizeof(decltype(TexturePointsBuffer->back()))));
	}
  }
}

void thred::internal::thrsav() {
  if (chkattr(*WorkingFileName)) {
	return;
  }
  if (!StateMap->testAndReset(StateFlag::IGNAM)) {
	auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto geName = *WorkingFileName;
	geName.replace_extension(L".th*");
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const file = FindFirstFile(geName.wstring().c_str(), &fileData);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
		  auto newFileName = VersionNames->operator[](iBackup - 1U);

		  auto ext   = newFileName.extension().wstring();
		  ext.back() = gsl::narrow<wchar_t>(iBackup - 1) + 's';
		  newFileName.replace_extension(ext);
		  auto ec = std::error_code {};
		  fs::rename(VersionNames->operator[](iBackup - 1U), newFileName, ec);
		  auto const msg = ec.message();
		  if (ec != std::error_code {}) {
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::crmsg(*ThrName);
  }
  else {
	auto output = std::vector<char> {};
	dubuf(output);
	auto bytesWritten = DWORD {0};
	WriteFile(fileHandle, output.data(), wrap::toUnsigned(output.size()), &bytesWritten, nullptr);
	if (bytesWritten != output.size()) {
	  auto fmtStr = std::wstring {};
	  displayText::loadString(fmtStr, IDS_FWERR);
	  displayText::shoMsg(fmt::format(fmtStr, ThrName->wstring()));
	}
	CloseHandle(fileHandle);
  }
}

void thred::internal::chk1col() {
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
  for (auto iColorChange = size_t {}; iColorChange < thred::maxColor(); ++iColorChange) {
	auto const ccTableIt     = wrap::next(ColorChangeTable->begin(), iColorChange);
	auto const ccTableNextIt = std::next(ccTableIt);
	if (ccTableNextIt->stitchIndex - ccTableIt->stitchIndex == 1) {
	  auto const iStitch = ccTableIt->stitchIndex;
	  auto       stitchIt =
	      (iStitch != 0U) ? wrap::next(StitchBuffer->begin(), iStitch - 1U) : StitchBuffer->begin();
	  auto const color = stitchIt->attribute & COLMSK;
	  ++stitchIt;
	  stitchIt->attribute &= NCOLMSK;
	  stitchIt->attribute |= color;
	}
  }
}

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
  if (UserFlagMap->test(UserFlag::ROTAUX)) {
	auto iDest = saveStitches.begin();
	for (auto& stitch : *StitchBuffer) {
	  *iDest++ = fPOINTATTR {stitch.y, stitch.x, stitch.attribute};
	}
  }
  else {
	std::copy(StitchBuffer->begin(), StitchBuffer->end(), saveStitches.begin());
  }
  // Resharper disable once CppInitializedValueIsAlwaysRewritten
  auto flag = true;
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  flag = DST::saveDST(AuxName, saveStitches);
	  break;
	}
#if PESACT
	case AUXPES: {
	  flag = PES::savePES(AuxName, saveStitches);
	  break;
	}
#endif
	default: {
	  flag = PCS::savePCS(AuxName, saveStitches);
	}
  }
  if (flag) {
	defNam(*WorkingFileName);
	if (UserFlagMap->test(UserFlag::ROTAUX)) {
	  displayText::filnopn(IDS_FILROT, *AuxName);
	}
  }
}

void thred::internal::auxmen() {
  constexpr auto CCH = 13; // length of the menu item text???

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
                               CCH,
                               nullptr};
  auto auxMsg  = std::wstring {};
  CheckMenuItem(MainMenu, ID_AUXPCS, MF_UNCHECKED);
#if PESACT
  CheckMenuItem(MainMenu, ID_AUXPES, MF_UNCHECKED);
#else
  EnableMenuItem(MainMenu, ID_AUXPES, MF_DISABLED | MF_GRAYED);
#endif
  CheckMenuItem(MainMenu, ID_AUXDST, MF_UNCHECKED);
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  auxMsg.assign(fmt::format(StringTable->operator[](STR_AUXTXT), L"DST"));
	  CheckMenuItem(MainMenu, ID_AUXDST, MF_CHECKED);
	  break;
	}
	case AUXPES:
#if PESACT
	{
	  auxMsg.assign(fmt::format(StringTable->operator[](STR_AUXTXT), L"PES"));
	  CheckMenuItem(MainMenu, ID_AUXPES, MF_CHECKED);
	  break;
	}
#else
	{
	  IniFile.auxFileType = AUXPCS;
	}
#endif
	default: {
	  auxMsg.assign(fmt::format(StringTable->operator[](STR_AUXTXT), L"PCS"));
	  CheckMenuItem(MainMenu, ID_AUXPCS, MF_CHECKED);
	}
  }
#pragma warning(suppress : 26492) // type.3 Don't use const_cast to cast away const or volatile NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
  filinfo.dwTypeData = const_cast<LPWSTR>(auxMsg.c_str());
  SetMenuItemInfo(FileMenu, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
  StateMap->set(StateFlag::DUMEN);
}

auto thred::internal::getSaveName(fs::path* fileName, fileIndices& fileType) -> bool {
  if (nullptr != fileName) {
	auto* pFileSave = gsl::narrow_cast<IFileSaveDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
	auto hr = CoCreateInstance(
	    CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (SUCCEEDED(hr) && (nullptr != pFileSave)) {
#if PESACT
	  static constexpr auto aFileTypes = std::array<COMDLG_FILTERSPEC, 4> {FLTTHR, FLTPCS, FLTDST, FLTPES}; // All possible file types for save
#else
	  static constexpr auto aFileTypes = std::array<COMDLG_FILTERSPEC, 4> {FLTTHR, FLTPCS, FLTDST}; // All possible file types for save
#endif
	  hr = pFileSave->SetFileTypes(wrap::toUnsigned(aFileTypes.size()), aFileTypes.data());
	  hr += pFileSave->SetFileTypeIndex(1);
	  hr += pFileSave->SetTitle(L"Save As");
	  hr += pFileSave->SetFileName(fileName->filename().c_str());
	  hr += pFileSave->SetDefaultExtension(L"thr");
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (SUCCEEDED(hr)) {
		hr = pFileSave->Show(nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		if (SUCCEEDED(hr)) {
		  auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
		  hr          = pFileSave->GetResult(&pItem);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		  if (SUCCEEDED(hr) && (nullptr != pItem)) {
			auto reply = 0U;
			hr         = pFileSave->GetFileTypeIndex(&reply);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
			if (SUCCEEDED(hr)) {
			  switch (reply) {
				case 1: {
				  fileType = fileIndices::THR;
				  break;
				}
				case 2: {
				  fileType = fileIndices::PCS;
				  break;
				}
				case 3: {
				  fileType = fileIndices::DST;
				  break;
				}
#if PESACT
				case 4: {
				  fileType = fileIndices::PES;
				  break;
				}
#endif
				default: {
				  fileType = fileIndices::THR;
				  break;
				}
			  }
			  // NOLINTNEXTLINE(readability-qualified-auto)
			  auto pszFilePath = PWSTR {nullptr};
			  hr               = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
			  if (SUCCEEDED(hr)) {
				fileName->assign(pszFilePath);
				CoTaskMemFree(pszFilePath);
				return true;
			  }
			}
		  }
		}
	  }
	}
  }
  return false;
}

void thred::internal::savAs() {
  if (!StitchBuffer->empty() || !FormList->empty() || bitmap::ismap()) {
	auto index = fileIndices {};
	if (getSaveName(WorkingFileName, index)) {
	  *DefaultDirectory = WorkingFileName->parent_path();
	  switch (index) {
		case fileIndices::THR: {
		  WorkingFileName->replace_extension(L".thr");
		  break;
		}
		case fileIndices::PCS: {
		  WorkingFileName->replace_extension(L".pcs");
		  IniFile.auxFileType = AUXPCS;
		  auxmen();
		  break;
		}
#if PESACT
		case fileIndices::PES: {
		  WorkingFileName->replace_extension(L".pes");
		  IniFile.auxFileType = AUXPES;
		  auxmen();
		  break;
		}
#endif
		case fileIndices::DST: {
		  WorkingFileName->replace_extension(L".dst");
		  IniFile.auxFileType = AUXDST;
		  auxmen();
		  break;
		}
	  }
	  StateMap->set(StateFlag::SAVAS);
	  nunams();
	  ritini();
	  StateMap->reset(StateFlag::SAVAS);
	  StateMap->reset(StateFlag::CMPDO);
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
	thi::sav();
  }
}

void thred::internal::dun() {
  texture::savtxt();
  texture::txof();
  texture::rstxt();
  if (StateMap->testAndReset(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	StateMap->reset(StateFlag::WASRT);
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
	  StateMap->set(StateFlag::SAVEX);
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

void thred::internal::dusid(uint32_t                   entry,
                            int32_t&                   windowLocation,
                            POINT const&               windowSize,
                            std::vector<std::wstring> const& strings) noexcept {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  SideWindow[entry] = CreateWindow(L"STATIC",
                                   strings[entry].c_str(),
                                   SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                   3,
                                   windowLocation * windowSize.y + 3,
                                   windowSize.x + 3,
                                   windowSize.y,
                                   SideMessageWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  ++windowLocation;
}

void thred::internal::sidmsg(FRMHED const& form, HWND window, std::vector<std::wstring> const& strings) {
  // edge fill type array for side window display
  constexpr auto edgeFillTypes = std::array<uint8_t, EDGETYPS> {
      0, EDGELINE, EDGEBEAN, EDGECLIP, EDGEANGSAT, EDGEAPPL, EDGEPROPSAT, EDGEBHOL, EDGEPICOT, EDGEDOUBLE, EDGELCHAIN, EDGEOCHAIN, EDGECLIPX};
  // fill type array for side window display
  constexpr auto fillTypes = std::array<uint8_t, FILLTYPS> {
      0, VRTF, HORF, ANGF, SATF, CLPF, CONTF, VCLPF, HCLPF, ANGCLPF, FTHF, TXVRTF, TXHORF, TXANGF};
  constexpr auto featherFillTypes =
      std::array<uint8_t, FSSIZE> {FTHSIN, FTHSIN2, FTHLIN, FTHPSG, FTHRMP, FTHFAZ}; // feather fill types
  auto childListRect  = RECT {0L, 0L, 0L, 0L};
  auto parentListRect = RECT {0L, 0L, 0L, 0L};
  auto entryCount     = gsl::narrow<int32_t>(strings.size());
  std::fill(ValueWindow->begin(), ValueWindow->end(), nullptr);
  auto sideWindowSize     = POINT {};
  auto sideWindowLocation = int32_t {};
  GetWindowRect(window, &childListRect);
  GetWindowRect(FormDataSheet, &parentListRect);
  form::ispcdclp();
  if (StateMap->test(StateFlag::FILTYP)) {
	for (auto iEntry = 0U; iEntry < edgeFillTypes.size(); ++iEntry) {
	  if ((form.edgeType & NEGUND) == edgeFillTypes[iEntry]) {
		--entryCount;
	  }
	  else {
		if (edgeFillTypes[iEntry] == EDGECLIP || edgeFillTypes[iEntry] == EDGEPICOT ||
		    edgeFillTypes[iEntry] == EDGECLIPX) {
		  if (StateMap->test(StateFlag::WASPCDCLP)) {
			formForms::maxtsiz(strings[iEntry], sideWindowSize);
		  }
		  else {
			--entryCount;
		  }
		}
		else {
		  formForms::maxtsiz(strings[iEntry], sideWindowSize);
		}
	  }
	}
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	SideMessageWindow = CreateWindow(L"STATIC",
	                                 nullptr,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 parentListRect.right - ThredWindowOrigin.x + 3,
	                                 childListRect.top - ThredWindowOrigin.y - 3,
	                                 sideWindowSize.x + 12,
	                                 sideWindowSize.y * entryCount + 12,
	                                 ThrEdWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
	for (auto iEntry = 0U; iEntry < strings.size(); ++iEntry) {
	  if ((form.edgeType & NEGUND) != edgeFillTypes[iEntry]) {
		if (edgeFillTypes[iEntry] == EDGECLIP || edgeFillTypes[iEntry] == EDGEPICOT ||
		    edgeFillTypes[iEntry] == EDGECLIPX) {
		  if (StateMap->test(StateFlag::WASPCDCLP)) {
			dusid(iEntry, sideWindowLocation, sideWindowSize, strings);
		  }
		}
		else {
		  dusid(iEntry, sideWindowLocation, sideWindowSize, strings);
		}
	  }
	}
  }
  else {
	if (FormMenuChoice == LLAYR) {
	  auto const zero = std::wstring {L"0"};
	  formForms::maxtsiz(zero, sideWindowSize);
	}
	else {
	  if (FormMenuChoice == LFTHTYP) {
		entryCount     = gsl::narrow<int32_t>(featherFillTypes.size() - 1U);
		sideWindowSize = POINT {ButtonWidthX3, ButtonHeight};
	  }
	  else {
		for (auto iEntry = 0U; iEntry < strings.size(); ++iEntry) {
		  if (((1U << fillTypes[iEntry]) & ClipTypeMap) != 0U) {
			if (StateMap->test(StateFlag::WASPCDCLP)) {
			  formForms::maxtsiz(strings[iEntry], sideWindowSize);
			}
			else {
			  --entryCount;
			}
		  }
		  else {
			if (fillTypes[iEntry] == form.fillType) {
			  --entryCount;
			}
			else {
			  formForms::maxtsiz(strings[iEntry], sideWindowSize);
			}
		  }
		}
	  }
	}
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	SideMessageWindow = CreateWindow(L"STATIC",
	                                 nullptr,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 parentListRect.right - ThredWindowOrigin.x + 3,
	                                 childListRect.top - ThredWindowOrigin.y - 3,
	                                 sideWindowSize.x + 12,
	                                 sideWindowSize.y * entryCount + 12,
	                                 ThrEdWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
	if (FormMenuChoice == LLAYR) {
	  for (auto iEntry = 0U; iEntry < strings.size(); ++iEntry) {
		dusid(iEntry, sideWindowLocation, sideWindowSize, strings);
	  }
	}
	else {
	  if (FormMenuChoice == LFTHTYP) {
		for (auto iEntry = 0U; iEntry < featherFillTypes.size(); ++iEntry) {
		  if (featherFillTypes[iEntry] != form.fillInfo.feather.fillType) {
			dusid(iEntry, sideWindowLocation, sideWindowSize, strings);
		  }
		}
	  }
	  else {
		for (auto iEntry = 0U; iEntry < strings.size(); ++iEntry) {
		  if (fillTypes[iEntry] != form.fillType) {
			if (((1U << fillTypes[iEntry]) & ClipTypeMap) != 0U) {
			  if (StateMap->test(StateFlag::WASPCDCLP)) {
				dusid(iEntry, sideWindowLocation, sideWindowSize, strings);
			  }
			}
			else {
			  dusid(iEntry, sideWindowLocation, sideWindowSize, strings);
			}
		  }
		}
	  }
	}
  }
  StateMap->set(StateFlag::SIDACT);
}

auto thred::internal::centr() noexcept -> fPOINT {
  auto const center = POINT {std::lround((ZoomRect.right - ZoomRect.left) * 0.5F),
                             std::lround((ZoomRect.top - ZoomRect.bottom) * 0.5F)};
  return fPOINT {ZoomRect.left + wrap::toFloat(center.x), ZoomRect.bottom + wrap::toFloat(center.y)};
}

void thred::internal::stchWnd() {
  stchPars();
  MainStitchWin = nullptr;
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const hFont = displayText::getThrEdFont(FONTSIZE);
  SelectObject(GetDC(MainStitchWin), hFont);
  if (MainStitchWin != nullptr) {
	GetWindowRect(MainStitchWin, &StitchWindowAbsRect);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
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
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
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
auto thred::internal::chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) -> bool {
  auto flag       = false;
  auto topRect    = RECT {0L, 0L, 0L, 0L};
  auto bottomRect = RECT {0L, 0L, 0L, 0L};
  GetWindowRect(topWindow, &topRect);
  GetWindowRect(bottomWindow, &bottomRect);
  if (clickCoord.x > topRect.left && clickCoord.x < bottomRect.right &&
      clickCoord.y > topRect.top && clickCoord.y < bottomRect.bottom) {
	VerticalIndex = COLORMAX - gsl::narrow<uint8_t>((bottomRect.bottom - clickCoord.y) / ButtonHeight);
	if (VerticalIndex > COLORMAX) { // Something has broken so do something reasonable
	  VerticalIndex &= COLORBTS;
	}
	else { // we have a valid Index
	  flag = true;
	}
  }
  return flag;
}

void thred::internal::delstch1(uint32_t iStitch) {
  if (!StitchBuffer->empty()) {
	StitchBuffer->erase(wrap::next(StitchBuffer->begin(), iStitch));
	if (ClosestPointIndex > wrap::toUnsigned(StitchBuffer->size()) - 1U) {
	  ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size()) - 1U;
	}
  }
}

void thred::internal::redbak() {
  auto const* undoData = convert_ptr<BAKHED*>(UndoBuffer->operator[](UndoBufferWriteIndex).get());
  if (undoData != nullptr) {
	StitchBuffer->clear();
	if (undoData->stitchCount != 0U) {
	  auto const span = gsl::span<fPOINTATTR>(undoData->stitches, undoData->stitchCount);
	  StitchBuffer->insert(StitchBuffer->end(), span.begin(), span.end());
	}
	else {
	  StateMap->reset(StateFlag::INIT);
	}
	UnzoomedRect = undoData->zoomRect;
	FormList->clear();
	if (undoData->formCount != 0U) {
	  auto const span = gsl::span<FRMHED>(undoData->forms, undoData->formCount);
	  FormList->insert(FormList->end(), span.begin(), span.end());
	}
	FormVertices->clear();
	if (undoData->vertexCount != 0U) {
	  auto const span = gsl::span<fPOINT>(undoData->vertices, undoData->vertexCount);
	  FormVertices->insert(FormVertices->end(), span.begin(), span.end());
	}
	SatinGuides->clear();
	if (undoData->guideCount != 0U) {
	  auto const span = gsl::span<SATCON>(undoData->guide, undoData->guideCount);
	  SatinGuides->insert(SatinGuides->end(), span.begin(), span.end());
	}
	ClipPoints->clear();
	if (undoData->clipPointCount != 0U) {
	  auto const span = gsl::span<fPOINT>(undoData->clipPoints, undoData->clipPointCount);
	  ClipPoints->insert(ClipPoints->end(), span.begin(), span.end());
	}
	// ToDo - add field in BAKHED to keep track of number of colors
	constexpr auto UCOLSIZE = UserColor.size();
	auto const undoColors = gsl::span<COLORREF> {undoData->colors, gsl::narrow<ptrdiff_t>(UCOLSIZE)};
	auto const userColors = gsl::span<COLORREF> {UserColor};
	std::copy(undoColors.begin(), undoColors.end(), userColors.begin());
	for (auto iColor = 0U; iColor < UCOLSIZE; ++iColor) {
	  UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
	  UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	}
	for (auto& iColor : *UserColorWin) {
	  thred::redraw(iColor);
	}
	TexturePointsBuffer->clear();
	if (undoData->texturePointCount != 0U) {
	  auto const span = gsl::span<TXPNT>(undoData->texturePoints, undoData->texturePointCount);
	  TexturePointsBuffer->insert(TexturePointsBuffer->end(), span.begin(), span.end());
	}
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void thred::internal::redo() {
  if (StateMap->test(StateFlag::BAKACT) && StateMap->test(StateFlag::REDUSHO)) {
	++UndoBufferWriteIndex;
	UndoBufferWriteIndex &= (UNDOLEN - 1U);
	auto const nextBufferIndex = (UndoBufferWriteIndex + 1U) & (UNDOLEN - 1U);
	if (nextBufferIndex == UndoBufferReadIndex) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
	  StateMap->reset(StateFlag::REDUSHO);
	}
	else {
	  if (!StateMap->testAndSet(StateFlag::REDUSHO)) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
	  }
	}
	if (!StateMap->testAndSet(StateFlag::UNDUSHO)) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
	}
	redbak();
	StateMap->set(StateFlag::DUMEN);
  }
}

void thred::internal::bak() {
  thred::unmsg();
  StateMap->reset(StateFlag::FPSEL);
  StateMap->reset(StateFlag::FRMPSEL);
  StateMap->reset(StateFlag::BIGBOX);
  SelectedFormList->clear();
  thred::undat();
  if (StateMap->testAndReset(StateFlag::PRFACT)) {
	StateMap->reset(StateFlag::WASRT);
	DestroyWindow(PreferencesWindow);
	PreferenceIndex = 0;
	thred::unsid();
  }
  if (!StateMap->testAndSet(StateFlag::BAKING)) {
	dudat();
	UndoBufferReadIndex = UndoBufferWriteIndex + 1U;
  }
  if (StateMap->test(StateFlag::BAKWRAP)) {
	--UndoBufferWriteIndex;
	UndoBufferWriteIndex &= (UNDOLEN - 1U);
	auto const previousBufferIndex = UndoBufferWriteIndex - 1U;
	if (previousBufferIndex == UndoBufferReadIndex) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	  StateMap->set(StateFlag::DUMEN);
	  StateMap->reset(StateFlag::UNDUSHO);
	}
  }
  else {
	if (UndoBufferWriteIndex != 0U) {
	  --UndoBufferWriteIndex;
	}
	if (UndoBufferWriteIndex == 0U) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	  StateMap->set(StateFlag::DUMEN);
	  StateMap->reset(StateFlag::UNDUSHO);
	}
  }
  if (!StateMap->testAndSet(StateFlag::REDUSHO)) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
	StateMap->set(StateFlag::DUMEN);
  }
  StateMap->reset(StateFlag::FORMSEL);
  StateMap->reset(StateFlag::GRPSEL);
  StateMap->reset(StateFlag::SCROS);
  StateMap->reset(StateFlag::ECROS);
  redbak();
}

void thred::internal::prtred(HANDLE fileHandle, uint32_t code) {
  CloseHandle(fileHandle);
  StateMap->reset(StateFlag::INIT);
  FormList->clear();
  displayText::tabmsg(code);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::unthum() {
  if (StateMap->testAndReset(StateFlag::THUMSHO)) {
	for (auto& iBackup : BackupViewer) {
	  DestroyWindow(iBackup);
	}
	if (StateMap->test(StateFlag::UPTO)) {
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
  StateMap->reset(StateFlag::REDUSHO);
  StateMap->reset(StateFlag::UNDUSHO);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
  StateMap->set(StateFlag::DUMEN);
}

auto thred::internal::getNewFileName(fs::path& newFileName, fileStyles fileTypes, fileIndices fileIndex) -> bool {
  auto* pFileOpen = gsl::narrow_cast<IFileOpenDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hr = CoCreateInstance(
      CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
  if (SUCCEEDED(hr) && (nullptr != pFileOpen)) {
	auto dwOptions = DWORD {};
	hr             = pFileOpen->GetOptions(&dwOptions);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (SUCCEEDED(hr)) {
	  hr = pFileOpen->SetOptions(dwOptions | FOS_DONTADDTORECENT); // NOLINT(hicpp-signed-bitwise)
	  switch (fileTypes) {
		case fileStyles::ALL_FILES: {
#if PESACT
		  static constexpr auto aFileTypes =
		      std::array<COMDLG_FILTERSPEC, 4> {FLTTHR, FLTPCS, FLTDST, FLTPES}; // All possible file types for save
#else
		  static constexpr auto aFileTypes = std::array<COMDLG_FILTERSPEC, 3> {FLTTHR, FLTPCS, FLTDST}; // All possible file types for save
#endif
		  hr += pFileOpen->SetFileTypes(wrap::toUnsigned(aFileTypes.size()), aFileTypes.data());
		  break;
		}
		case fileStyles::INS_FILES: {
		  static constexpr auto iFileTypes = std::array<COMDLG_FILTERSPEC, 2> {FLTTHR, FLTPCS}; // All possible file types that can be inserted into current design
		  hr += pFileOpen->SetFileTypes(wrap::toUnsigned(iFileTypes.size()), iFileTypes.data());
		  break;
		}
	  }
	  switch (fileIndex) {
		case fileIndices::THR: {
		  hr += pFileOpen->SetFileTypeIndex(1);
		  break;
		}
		case fileIndices::PCS: {
		  hr += pFileOpen->SetFileTypeIndex(2);
		  break;
		}
		case fileIndices::DST: {
		  hr += pFileOpen->SetFileTypeIndex(3);
		  break;
		}
#if PESACT
		case fileIndices::PES: {
		  hr += pFileOpen->SetFileTypeIndex(4);
		  break;
		}
#endif
	  }
	  hr += pFileOpen->SetTitle(L"Open Thred File");
#if 0
		// If we want to, we can set the default directory rather than using the OS mechanism for last used
		auto* psiFrom = gsl::narrow_cast<IShellItem*>(nullptr);
		hr += SHCreateItemFromParsingName(DefaultDirectory->wstring().data(), nullptr, IID_PPV_ARGS(&psiFrom));
		hr += pFileOpen->SetFolder(psiFrom);
		if (nullptr != psiFrom) {
		  psiFrom->Release();
		}
#endif
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (SUCCEEDED(hr)) {
		hr = pFileOpen->Show(nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		if (SUCCEEDED(hr)) {
		  auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
		  hr          = pFileOpen->GetResult(&pItem);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		  if (SUCCEEDED(hr) && (nullptr != pItem)) {
			// NOLINTNEXTLINE(readability-qualified-auto)
			auto pszFilePath = PWSTR {nullptr};
			hr               = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
			if (SUCCEEDED(hr)) {
			  newFileName.assign(pszFilePath);
			  CoTaskMemFree(pszFilePath);
			  return true;
			}
		  }
		}
	  }
	}
  }
  return false;
}

auto thred::getFileSize(std::filesystem::path const& newFileName, uintmax_t& size) -> bool {
  auto ec = std::error_code {};
  size    = fs::file_size(newFileName, ec);
  if (ec != std::error_code {}) {
	// ToDo - find better error message
	displayText::filnopn(IDS_FNOPN, newFileName);
	return false;
  }
  return true;
}

auto thred::internal::readTHRFile(std::filesystem::path const& newFileName) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto bytesRead   = DWORD {0};
  auto thredHeader = STRHED {};
  ReadFile(fileHandle, &thredHeader, sizeof(thredHeader), &bytesRead, nullptr);
  if ((thredHeader.headerType & SIGMASK) == THREDSIG) {
	if (bytesRead != sizeof(thredHeader)) {
	  prtred(fileHandle, IDS_SHRTF);
	  return false;
	}
	auto const version = (thredHeader.headerType & FTYPMASK) >> TBYTSHFT;
	auto&      desName = IniFile.designerName;
	DesignerName->assign(utf::Utf8ToUtf16(std::string(std::begin(desName))));
	switch (version) {
	  case 0: {
		if (thredHeader.hoopType == SMALHUP) {
		  IniFile.hoopSizeX = SHUPX;
		  IniFile.hoopSizeY = SHUPY;
		  UnzoomedRect = POINT {gsl::narrow_cast<int32_t>(SHUPX), gsl::narrow_cast<int32_t>(SHUPY)};
		}
		else {
		  IniFile.hoopSizeX = LHUPX;
		  IniFile.hoopSizeY = LHUPY;
		  UnzoomedRect = POINT {gsl::narrow_cast<int32_t>(LHUPX), gsl::narrow_cast<int32_t>(LHUPY)};
		}
		ritfnam(*DesignerName);
		auto&      modName      = ExtendedHeader->modifierName;
		auto const modifierName = gsl::span<char> {modName};
		std::copy(&IniFile.designerName[0],
		          &IniFile.designerName[strlen(std::begin(desName))],
		          modifierName.begin());
		break;
	  }
	  case 1:
	  case 2: {
		ReadFile(fileHandle, ExtendedHeader, sizeof(*ExtendedHeader), &bytesRead, nullptr);
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
	ZoomRect     = fRECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
	UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
	StitchBuffer->resize(thredHeader.stitchCount);
	if (thredHeader.stitchCount != 0U) {
	  auto const bytesToRead =
	      wrap::toUnsigned(thredHeader.stitchCount * sizeof(decltype(StitchBuffer->back())));
	  ReadFile(fileHandle, StitchBuffer->data(), bytesToRead, &bytesRead, nullptr);
	  if (bytesRead != bytesToRead) {
		prtred(fileHandle, IDS_PRT);
		return false;
	  }
	}
	StitchBuffer->shrink_to_fit();
	auto bytesToRead = bitmap::getBmpNameLength();
	ReadFile(fileHandle, bitmap::getBmpNameData(), bytesToRead, &bytesRead, nullptr);
	if (bytesRead != bytesToRead) {
	  bitmap::resetBmpFile(true);
	  prtred(fileHandle, IDS_PRT);
	  return false;
	}
	bytesToRead = sizeof(BackgroundColor);
	ReadFile(fileHandle, &BackgroundColor, bytesToRead, &bytesRead, nullptr);
	if (bytesRead != bytesToRead) {
	  BackgroundColor = IniFile.backgroundColor;
	  prtred(fileHandle, IDS_PRT);
	  return false;
	}
	BackgroundBrush = CreateSolidBrush(BackgroundColor);
	bytesToRead     = wrap::toUnsigned(UserColor.size() * sizeof(decltype(UserColor.back())));
	ReadFile(fileHandle, UserColor.data(), bytesToRead, &bytesRead, nullptr);
	if (bytesRead != bytesToRead) {
	  UserColor = DefaultColors;
	  prtred(fileHandle, IDS_PRT);
	  return false;
	}
	bytesToRead = wrap::toUnsigned(CustomColor.size() * sizeof(decltype(UserColor.back())));
	ReadFile(fileHandle, CustomColor.data(), bytesToRead, &bytesRead, nullptr);
	if (bytesRead != bytesToRead) {
	  CustomColor = DefaultColors;
	  prtred(fileHandle, IDS_PRT);
	  return false;
	}
	auto msgBuffer = std::array<char, TSSIZE> {};
	ReadFile(fileHandle, msgBuffer.data(), wrap::toUnsigned(msgBuffer.size()), &bytesRead, nullptr);
	if (bytesRead != TSSIZE) {
	  prtred(fileHandle, IDS_PRT);
	  return false;
	}
	auto threadSizebuf  = std::string(msgBuffer.data(), msgBuffer.size());
	auto threadSizeBufW = utf::Utf8ToUtf16(threadSizebuf);
	for (auto iThread = 0U; iThread < TSSIZE; ++iThread) {
	  ThreadSize[iThread] = threadSizeBufW[iThread];
	}
	if (thredHeader.formCount != 0) {
	  StateMap->reset(StateFlag::BADFIL);
	  if (version < 2) {
		auto formListOriginal = std::vector<FRMHEDO> {};
		formListOriginal.resize(thredHeader.formCount);
		bytesToRead = wrap::toUnsigned(thredHeader.formCount * sizeof(decltype(formListOriginal.back())));
		wrap::ReadFile(fileHandle, formListOriginal.data(), bytesToRead, &bytesRead, nullptr);
		if (bytesRead != bytesToRead) {
		  wrap::narrow(thredHeader.formCount, bytesRead / sizeof(decltype(formListOriginal.back())));
		  formListOriginal.resize(thredHeader.formCount);
		  StateMap->set(StateFlag::BADFIL);
		}
		FormList->reserve(formListOriginal.size());
		FormList->insert(FormList->end(), formListOriginal.begin(), formListOriginal.end());
	  }
	  else {
		auto inFormList = std::vector<FRMHEDOUT> {};
		inFormList.resize(thredHeader.formCount);
		bytesToRead = wrap::toUnsigned(thredHeader.formCount * sizeof(decltype(inFormList.back())));
		wrap::ReadFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, nullptr);
		if (bytesRead != bytesToRead) {
		  wrap::narrow(thredHeader.formCount, bytesRead / sizeof(decltype(inFormList.back())));
		  inFormList.resize(thredHeader.formCount);
		  StateMap->set(StateFlag::BADFIL);
		}
		FormList->reserve(inFormList.size());
		FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
	  }
	  FormList->shrink_to_fit();
	  if (thredHeader.vertexCount != 0U) {
		FormVertices->resize(thredHeader.vertexCount);
		bytesToRead = wrap::toUnsigned(thredHeader.vertexCount * sizeof(decltype(FormVertices->back())));
		ReadFile(fileHandle, FormVertices->data(), bytesToRead, &bytesRead, nullptr);
		if (bytesRead != bytesToRead) {
		  FormVertices->resize(bytesRead / sizeof(decltype(FormVertices->back())));
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
		auto inGuideList = std::vector<SATCONOUT>(thredHeader.dlineCount);
		bytesToRead = wrap::toUnsigned(thredHeader.dlineCount * sizeof(decltype(inGuideList.back())));
		ReadFile(fileHandle, inGuideList.data(), bytesToRead, &bytesRead, nullptr);
		if (bytesRead != bytesToRead) {
		  inGuideList.resize(bytesRead / sizeof(decltype(inGuideList.back())));
		  StateMap->set(StateFlag::BADFIL);
		}
		SatinGuides->reserve(inGuideList.size());
		SatinGuides->insert(SatinGuides->end(), inGuideList.begin(), inGuideList.end());
	  }
	  SatinGuides->shrink_to_fit();
	  if (thredHeader.clipDataCount != 0U) {
		ClipPoints->resize(thredHeader.clipDataCount);
		bytesToRead = wrap::toUnsigned(thredHeader.clipDataCount * sizeof(decltype(ClipPoints->back())));
		ReadFile(fileHandle, ClipPoints->data(), bytesToRead, &bytesRead, nullptr);
		if (bytesRead != bytesToRead) {
		  ClipPoints->resize(bytesRead / sizeof(decltype(ClipPoints->back())));
		  StateMap->set(StateFlag::BADFIL);
		}
	  }
	  ClipPoints->shrink_to_fit();
	  if (ExtendedHeader->texturePointCount != 0U) {
		TexturePointsBuffer->resize(ExtendedHeader->texturePointCount);
		bytesToRead = wrap::toUnsigned(ExtendedHeader->texturePointCount *
		                               sizeof(decltype(TexturePointsBuffer->back())));
		ReadFile(fileHandle, TexturePointsBuffer->data(), bytesToRead, &bytesRead, nullptr);
		if (bytesRead != bytesToRead) {
		  TexturePointsBuffer->resize(bytesRead / sizeof(decltype(TexturePointsBuffer->back())));
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
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
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
	prtred(fileHandle, IDS_NOTHR);
	return false;
  }
  CloseHandle(fileHandle);
  return true;
}

auto thred::getFileHandle(std::filesystem::path const& newFileName, HANDLE& fileHandle) -> bool {
  // ToDo - use ifstream?
  // ifstream file(WorkingFileName, ios::in | ios::binary | ios::ate);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const handle =
      CreateFile(newFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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

void thred::internal::resetState() {
  fnamtabs();
  trace::untrace();
  if (!FormList->empty()) {
	form::delfrms();
  }
  unthum();
  thred::unbsho();
  bitmap::resetBmpFile(true);
  TexturePointsBuffer->clear();
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
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

void thred::internal::nuFil(fileIndices fileIndex) {
  auto newFileName = *WorkingFileName;
  if (StateMap->testAndReset(StateFlag::REDOLD) || getNewFileName(newFileName, fileStyles::ALL_FILES, fileIndex)) {
	WorkingFileName->assign(newFileName);
	defNam(newFileName);
	resetState();
	auto       fileExt        = newFileName.extension().wstring();
	auto const firstCharacter = tolower(fileExt[1]);
	if (firstCharacter == 't') {
	  {
		if (!readTHRFile(newFileName)) {
		  return;
		}
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
	ZoomRect     = fRECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
	UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
	thred::movStch();
	thred::coltab();
	StateMap->reset(StateFlag::ZUMED);
	auto buffer = std::array < wchar_t, 3>{};
	buffer[1]         = L'0';
	for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	  UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
	  UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	  buffer[0]              = ThreadSize[iColor];
	  SetWindowText(ThreadSizeWin[iColor], buffer.data());
	}
	for (auto& iColor : *UserColorWin) {
	  thred::redraw(iColor);
	}
	thred::redraw(ColorBar);
	if (!StitchBuffer->empty()) {
	  StateMap->set(StateFlag::INIT);
	}
	nuAct(0);
	StateMap->set(StateFlag::RESTCH);
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
	lenCalc();
	SetWindowText(
	    ThrEdWindow,
	    fmt::format(StringTable->operator[](STR_THRDBY), newFileName.wstring(), *DesignerName).c_str());
	StateMap->set(StateFlag::INIT);
	StateMap->reset(StateFlag::TRSET);
	if (StateMap->test(StateFlag::NOTHRFIL)) {
	  for (auto& stitch : *StitchBuffer) {
		stitch.attribute |= NOTFRM;
	  }
	}
	repair::lodchk();
  }
}

auto thred::internal::nuCol(COLORREF init) noexcept -> BOOL {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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

auto thred::internal::nuBak() noexcept -> BOOL {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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

auto thred::pxCor2stch(POINT const& point) noexcept -> fPOINT {
  auto const ratioX =
      wrap::toFloat(point.x - StitchWindowAbsRect.left) / wrap::toFloat(StitchWindowClientRect.right);
  auto const ratioY =
      wrap::toFloat(StitchWindowAbsRect.bottom - point.y) / wrap::toFloat(StitchWindowClientRect.bottom);
  return fPOINT {ratioX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left,
                 ratioY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom};
}

auto thred::inStitchWin() noexcept -> bool {
  return (Msg.pt.x >= StitchWindowAbsRect.left && Msg.pt.x <= StitchWindowAbsRect.right &&
          Msg.pt.y >= StitchWindowAbsRect.top && Msg.pt.y <= StitchWindowAbsRect.bottom);
}

void thred::internal::zumin() {
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
	do {
	  if (StateMap->test(StateFlag::GMRK)) {
		stitchPoint = ZoomMarkPoint;
		break;
	  }
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto const& boundingRect = FormList->operator[](ClosestFormToCursor).rectangle;

		stitchPoint = fPOINT {wrap::midl(boundingRect.right, boundingRect.left),
		                      wrap::midl(boundingRect.top, boundingRect.bottom)};
		break;
	  }
	  if (StateMap->test(StateFlag::FRMPSEL)) {
		auto const vertexIt =
		    wrap::next(FormVertices->cbegin(),
		               FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
		stitchPoint = *vertexIt;
		break;
	  }
	  if (StateMap->test(StateFlag::SELBOX)) {
		stitchPoint = StitchBuffer->operator[](ClosestPointIndex);
		break;
	  }
	  if (StateMap->test(StateFlag::GRPSEL)) {
		auto groupBoundingRect = fRECTANGLE {};
		thred::selRct(groupBoundingRect);
		stitchPoint = fPOINT {wrap::midl(groupBoundingRect.right, groupBoundingRect.left),
		                      wrap::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
		break;
	  }
	  if (StateMap->test(StateFlag::INSRT)) {
		if (StateMap->test(StateFlag::LIN1)) {
		  if (StateMap->test(StateFlag::BAKEND)) {
			stitchPoint = StitchBuffer->back();
		  }
		  else {
			stitchPoint = StitchBuffer->front();
		  }
		}
		else {
		  stitchPoint =
		      fPOINT {wrap::midl(StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).x,
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
		for (auto selectedForm : (*SelectedFormList)) {
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
		stitchPoint = fPOINT {wrap::midl(SelectedFormsRect.right, SelectedFormsRect.left),
		                      wrap::midl(SelectedFormsRect.top, SelectedFormsRect.bottom)};
		break;
	  }
	  if (!thred::inStitchWin()) {
		stitchPoint = centr();
	  }
	} while (false);
  }
  auto const zoomRight = wrap::toFloat(UnzoomedRect.x) * ZoomFactor;
  ZoomRect             = fRECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
  thred::shft(stitchPoint);
  NearestCount = 0;
  if (!StateMap->test(StateFlag::GMRK) && StateMap->test(StateFlag::SELBOX)) {
	shft2box();
  }
  if (StateMap->test(StateFlag::RUNPAT)) {
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	RunPoint = 0;
  }
  thred::duzrat();
  movins();
  StateMap->set(StateFlag::RESTCH);
  ilin();
}

void thred::internal::zumshft() {
  if (StateMap->test(StateFlag::ZUMED)) {
	unboxs();
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
  movins();
}

void thred::internal::zumout() {
  unboxs();
  auto stitchPoint = thred::pxCor2stch(Msg.pt);
  if (StateMap->test(StateFlag::ZUMED)) {
	do {
	  if (StateMap->test(StateFlag::GMRK)) {
		stitchPoint = ZoomMarkPoint;
		break;
	  }
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto const& boundingRect = FormList->operator[](ClosestFormToCursor).rectangle;

		stitchPoint = fPOINT {wrap::midl(boundingRect.right, boundingRect.left),
		                      wrap::midl(boundingRect.top, boundingRect.bottom)};
		break;
	  }
	  if (StateMap->test(StateFlag::FRMPSEL)) {
		auto const vertexIt =
		    wrap::next(FormVertices->cbegin(),
		               FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
		stitchPoint = *vertexIt;
		break;
	  }
	  if (StateMap->test(StateFlag::SELBOX) || StateMap->test(StateFlag::INSRT)) {
		stitchPoint = StitchBuffer->operator[](ClosestPointIndex);
		break;
	  }
	  if (StateMap->test(StateFlag::GRPSEL)) {
		auto groupBoundingRect = fRECTANGLE {};
		thred::selRct(groupBoundingRect);
		stitchPoint = fPOINT {wrap::midl(groupBoundingRect.right, groupBoundingRect.left),
		                      wrap::midl(groupBoundingRect.top, groupBoundingRect.bottom)};
		break;
	  }
	  if (StateMap->test(StateFlag::SELBOX)) {
		shft2box();
		break;
	  }
	  if (!thred::inStitchWin()) {
		stitchPoint = centr();
	  }
	} while (false);
	ZoomFactor /= ZUMFCT;
	constexpr auto ZMCLAMP = 0.98F; // clamp the zoom factor
	if (ZoomFactor > ZMCLAMP) {
	  ZoomFactor = 1;
	  StateMap->reset(StateFlag::ZUMED);
	  ZoomRect = fRECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.y), wrap::toFloat(UnzoomedRect.x), 0.0F};
	  thred::movStch();
	  NearestCount = 0;
	}
	else {
	  auto const zoomRight = wrap::toFloat(UnzoomedRect.x) * ZoomFactor;
	  ZoomRect = fRECTANGLE {0.0F, zoomRight / StitchWindowAspectRatio, zoomRight, 0.0F};
	  thred::shft(stitchPoint);
	}
	if (StateMap->test(StateFlag::RUNPAT)) {
	  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	  RunPoint = 0;
	}
	StateMap->set(StateFlag::RESTCH);
	thred::duzrat();
	movins();
  }
}

void thred::internal::duClos(uint32_t            startStitch,
                             uint32_t            stitchCount,
                             fPOINT const&       stitchPoint,
                             std::vector<float>& gapToNearest) noexcept {
  for (auto iStitch = startStitch; iStitch < startStitch + stitchCount; ++iStitch) {
	auto const& stitch = StitchBuffer->operator[](iStitch);

	auto const cx =
	    ((stitch.x > stitchPoint.x) ? (stitch.x - stitchPoint.x) : (stitchPoint.x - stitch.x));
	auto const cy =
	    ((stitch.y > stitchPoint.y) ? (stitch.y - stitchPoint.y) : (stitchPoint.y - stitch.y));
	auto sum   = hypot(cx, cy);
	auto tind0 = iStitch;
	for (auto iNear = 0U; iNear < NERCNT; ++iNear) {
	  if (sum < gapToNearest[iNear]) {
		auto const lowestSum = gapToNearest[iNear];
		auto const tind1     = NearestPoint[iNear];
		gapToNearest[iNear]  = sum;
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
  std::vector<float> gapToNearest;       // distances of the closest points
  gapToNearest.resize(NERCNT, BIGFLOAT); // to a mouse click
  NearestPoint.fill(std::numeric_limits<uint32_t>::max());
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	auto const iStitch0 = ColorChangeTable->operator[](iColor).stitchIndex;

	auto const iStitch1 =
	    gsl::narrow_cast<decltype(iStitch0)>(ColorChangeTable->operator[](iColor + 1U).stitchIndex - iStitch0);
	if (StateMap->test(StateFlag::HID)) {
	  if (ColorChangeTable->operator[](iColor).colorIndex == ActiveColor) {
		duClos(iStitch0, iStitch1, stitchPoint, gapToNearest);
	  }
	}
	else {
	  duClos(iStitch0, iStitch1, stitchPoint, gapToNearest);
	}
  }
  GetClientRect(MainStitchWin, &StitchWindowClientRect);
  auto stitchCoordsInPixels = POINT {};
  for (auto const iNear : NearestPoint) {
	if (stch2px(iNear, stitchCoordsInPixels)) {
	  NearestPoint[NearestCount]   = iNear;
	  NearestPixel[NearestCount++] = stitchCoordsInPixels;
	}
  }
  boxs();
}

auto thred::internal::closPnt1(uint32_t& closestStitch) -> bool {
  auto       closestIndex = 0U;
  auto const pointToCheck = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto       stitchCoordsInPixels = POINT {};
  if (StateMap->test(StateFlag::SELBOX) && stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	if (hypot(stitchCoordsInPixels.x - pointToCheck.x, stitchCoordsInPixels.y - pointToCheck.y) < CLOSENUF) {
	  closestStitch = ClosestPointIndex;
	  return true;
	}
  }
  for (auto iNear = 0U; iNear < NearestCount; ++iNear) {
	auto const offset = BoxOffset[iNear];
	if (pointToCheck.x >= NearestPixel[iNear].x - offset && pointToCheck.x <= NearestPixel[iNear].x + offset &&
	    pointToCheck.y >= NearestPixel[iNear].y - offset && pointToCheck.y <= NearestPixel[iNear].y + offset) {
	  closestStitch = NearestPoint[iNear];
	  return true;
	}
  }
  auto const stitchPoint     = thred::pxCor2stch(Msg.pt);
  auto       distanceToClick = BIGFLOAT;
  if (StateMap->test(StateFlag::HID)) {
	for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	  if (ColorChangeTable->operator[](iColor).colorIndex == ActiveColor) {
		for (auto iStitch = ColorChangeTable->operator[](iColor).stitchIndex;
		     iStitch < ColorChangeTable->     operator[](iColor + 1U).stitchIndex;
		     ++iStitch) {
		  auto const& stitch = StitchBuffer->operator[](iStitch);
		  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right &&
		      stitch.y >= ZoomRect.bottom && stitch.y <= ZoomRect.top) {
			auto const cx =
			    ((stitch.x > stitchPoint.x) ? (stitch.x - stitchPoint.x) : (stitchPoint.x - stitch.x));
			auto const cy =
			    ((stitch.y > stitchPoint.y) ? (stitch.y - stitchPoint.y) : (stitchPoint.y - stitch.y));
			auto const currentDistance = hypot(cx, cy);
			if (currentDistance < distanceToClick) {
			  distanceToClick = currentDistance;
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
		  auto const cx              = stitch.x - stitchPoint.x;
		  auto const cy              = stitch.y - stitchPoint.y;
		  auto const currentDistance = hypot(cx, cy);
		  if (currentDistance < distanceToClick) {
			distanceToClick = currentDistance;
			closestIndex    = currentStitch;
		  }
		}
	  }
	  ++currentStitch;
	}
  }
  if (distanceToClick == BIGFLOAT) {
	return false;
  }
  stch2px(closestIndex, stitchCoordsInPixels);
  if (StateMap->test(StateFlag::IGNTHR)) {
	closestStitch = closestIndex;
	return true;
  }
  if (hypot(pointToCheck.x - stitchCoordsInPixels.x, pointToCheck.y - stitchCoordsInPixels.y) < CLOSENUF) {
	closestStitch = closestIndex;
	return true;
  }
  return false;
}

auto thred::internal::pt2colInd(uint32_t iStitch) noexcept -> uint32_t {
  auto iColor = 0U;
  for (; iColor < thred::maxColor(); ++iColor) {
	if (ColorChangeTable->operator[](iColor).stitchIndex > iStitch) {
	  break;
	}
  }
  return iColor;
}

void thred::internal::toglup() {
  if (StateMap->testAndFlip(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, StringTable->operator[](STR_UPOF));
  }
  else {
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
		  while (ClosestPointIndex < wrap::toUnsigned(StitchBuffer->size()) &&
		         form::notfstch(StitchBuffer->operator[](ClosestPointIndex).attribute)) {
			++ClosestPointIndex;
		  }
		  if (ClosestPointIndex == StitchBuffer->size()) { // the selected form does not yet have any stitches
			--ClosestPointIndex;
		  }
		  StateMap->set(StateFlag::SELBOX);
		  StateMap->reset(StateFlag::FRMPSEL);
		}
	  }
	}
	displayText::butxt(HUPTO, StringTable->operator[](STR_UPON));
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::toglHid() {
  if (StateMap->testAndFlip(StateFlag::HID)) {
	StateMap->reset(StateFlag::FRMOF);
  }
  else {
	StateMap->set(StateFlag::FRMOF);
  }
  unthum();
  thred::redraw(ButtonWin->operator[](HHID));
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::dulin(std::array<POINT, 2>& moveLine0, std::array<POINT, 2>& moveLine1) {
  SelectObject(StitchWindowDC, LinePen);
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  if (moveLine0[0].x == moveLine1[1].x && moveLine0[0].y == moveLine1[1].y) {
	if (StateMap->test(StateFlag::ISDWN)) {
	  wrap::Polyline(StitchWindowDC, moveLine0.data(), wrap::toUnsigned(moveLine0.size()));
	}
	else {
	  wrap::Polyline(StitchWindowDC, moveLine1.data(), wrap::toUnsigned(moveLine1.size()));
	}
  }
  else {
	if (StateMap->test(StateFlag::ISDWN)) {
	  wrap::Polyline(StitchWindowDC, moveLine0.data(), wrap::toUnsigned(moveLine0.size()));
	}
	if (StateMap->test(StateFlag::ISUP)) {
	  wrap::Polyline(StitchWindowDC, moveLine1.data(), wrap::toUnsigned(moveLine1.size()));
	}
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
  StateMap->flip(StateFlag::WASLIN);
}

void thred::internal::unlin() {
  if (StateMap->test(StateFlag::WASLIN)) {
	dulin(MoveLine0, MoveLine1);
  }
}

void thred::internal::movbox() {
  if (!StitchBuffer->empty()) {
	auto stitchCoordsInPixels = POINT {};
	if (stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	  unbox();
#ifdef _DEBUG
	  auto const& stitch = StitchBuffer->operator[](ClosestPointIndex);
	  outDebugString(L"movbox:Stitch [{}] form [{}] type [{}] x [{}] y[{}]\n",
	                 ClosestPointIndex,
	                 ((stitch.attribute & FRMSK) >> FRMSHFT),
	                 ((stitch.attribute & TYPMSK) >> TYPSHFT),
	                 stitch.x,
	                 stitch.y);
#endif
	  dubox(stitchCoordsInPixels);
	  if (StateMap->test(StateFlag::UPTO)) {
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	else {
	  shft2box();
	  StateMap->set(StateFlag::SELBOX);
	  StateMap->reset(StateFlag::FRMPSEL);
	  StateMap->set(StateFlag::RESTCH);
	}
	nuAct(ClosestPointIndex);
	ritcor(StitchBuffer->operator[](ClosestPointIndex));
  }
}

auto thred::internal::chkhid(size_t colorToCheck) -> bool {
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

auto thred::internal::closlin() -> uint32_t {
  // ToDo - This function needs to be thoroughly checked and compared to findDistanceToSide
  auto           closestPoint = std::numeric_limits<uint32_t>::max();
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
	if (thi::chkhid(iChange)) {
	  for (auto iStitch = ptrdiff_t {}; iStitch < stitchCount; ++iStitch) {
		auto const layer = (stitches[iStitch].attribute & LAYMSK) >> LAYSHFT;
		if ((ActiveLayer == 0U) || (layer == 0U) || (layer == ActiveLayer)) {
		  auto const xba = std::lround(stitches[iStitch + 1].x - stitches[iStitch].x);
		  auto const yab = std::lround(stitches[iStitch].y - stitches[iStitch + 1].y);
		  auto left = (xba > 0) ? stitches[iStitch].x - tolerance : stitches[iStitch + 1].x - tolerance;
		  auto right = (xba > 0) ? stitches[iStitch + 1].x + tolerance : stitches[iStitch].x + tolerance;
		  auto bottom = (yab < 0) ? stitches[iStitch].y - tolerance : stitches[iStitch + 1].y - tolerance;
		  auto top = (yab < 0) ? stitches[iStitch + 1].y + tolerance : stitches[iStitch].y + tolerance;
		  if (offsetX > left && offsetX < right && offsetY > bottom && offsetY < top) {
			// ReSharper disable once CppInitializedValueIsAlwaysRewritten
			auto tsum         = 0.0F;
			auto intersection = fPOINT {};
			auto dx           = 0.0F;
			auto dy           = 0.0F;
			do {
			  if (yab == 0) {
				// stitch is horizontal
				intersection.x = offsetX;
				intersection.y = stitches[iStitch].y;
			  }
			  else {
				if (xba == 0) {
				  // stitch is vertical
				  dx = stitches[iStitch].x - offsetX;
				  top -= tolerance;
				  bottom += tolerance;
				  if (offsetY > top) {
					dy   = offsetY - top;
					tsum = hypot(dx, dy);
					break;
				  }
				  if (offsetY < bottom) {
					dy   = offsetY - bottom;
					tsum = hypot(dx, dy);
					break;
				  }
				  tsum = fabs(dx);
				  break;
				}
				auto const slope = wrap::toFloat(xba) / wrap::toFloat(yab);
				offset           = stitches[iStitch].x + slope * stitches[iStitch].y;
				auto const poff  = offsetX - offsetY / slope;

				intersection = fPOINT {offset - slope * intersection.y,
				                       slope * (offset - poff) / (slope * slope + 1.0F)};
				dx           = intersection.x - offsetX;
				dy           = intersection.y - offsetY;
			  }
			  top -= tolerance;
			  bottom += tolerance;
			  left += tolerance;
			  right -= tolerance;
			  if (intersection.x < left) {
				if (intersection.y < bottom) {
				  dx = offsetX - left;
				  dy = offsetY - bottom;
				}
				else {
				  dx = offsetX - left;
				  dy = offsetY - top;
				}
			  }
			  else {
				if (intersection.x > right) {
				  if (intersection.y < bottom) {
					dx = offsetX - right;
					dy = offsetY - bottom;
				  }
				  else {
					dx = offsetX - right;
					dy = offsetY - top;
				  }
				}
			  }
			  tsum = sqrt(dx * dx + dy * dy);
			} while (false);
			if (tsum < sum) {
			  sum          = tsum;
			  closestPoint = wrap::toUnsigned(iStitch) + ColorChangeTable->operator[](iChange).stitchIndex;
			}
		  }
		}
	  }
	}
  }
  if (sum > tolerance) {
	return std::numeric_limits<uint32_t>::max();
  }
  return closestPoint;
}

void thred::internal::istch() {
  xlin();
  xlin1();
  if (StateMap->test(StateFlag::SELBOX)) {
	if ((ClosestPointIndex != 0U) && ClosestPointIndex != wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
	  auto& stitch           = StitchBuffer->operator[](ClosestPointIndex);
	  auto& prvStitch        = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) - 1U);
	  auto& nxtStitch        = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
	  auto const angt        = atan2(stitch.y - stitchPoint.y, stitch.x - stitchPoint.x);
	  auto const angb        = atan2(stitch.y - prvStitch.y, stitch.x - prvStitch.x);
	  auto const angf        = atan2(stitch.y - nxtStitch.y, stitch.x - nxtStitch.x);
	  if (fabs(angf - angt) > fabs(angb - angt)) {
		--ClosestPointIndex;
	  }
	}
	else {
	  if (ClosestPointIndex == wrap::toUnsigned(StitchBuffer->size() - 1U)) {
		--ClosestPointIndex;
	  }
	}
  }
  else {
	ClosestPointIndex = closlin();
  }
  if (ClosestPointIndex == gsl::narrow_cast<decltype(ClosestPointIndex)>(-1)) {
	StateMap->reset(StateFlag::INSRT);
  }
  else {
	StateMap->set(StateFlag::INSRT);
	duIns();
	SetCapture(ThrEdWindow);
	displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
	nuAct(ClosestPointIndex);
  }
}

void thred::internal::mark() {
  if (StateMap->test(StateFlag::SELBOX) || StateMap->test(StateFlag::INSRT) || closPnt1(ClosestPointIndex)) {
	unbox();
	xlin();
	StateMap->reset(StateFlag::CAPT);
	StateMap->reset(StateFlag::SELBOX);
	StateMap->reset(StateFlag::INSRT);
	StateMap->reset(StateFlag::SCROS);
	StateMap->reset(StateFlag::ECROS);
	GroupStitchIndex = ClosestPointIndex;
	StateMap->set(StateFlag::GRPSEL);
  }
}

void thred::internal::selCol() {
  if (!StitchBuffer->empty()) {
	auto iStitch = (StateMap->test(StateFlag::SELBOX))
	                   ? ClosestPointIndex
	                   : (StateMap->test(StateFlag::GRPSEL)) ? GroupStitchIndex : 0;
	if (iStitch > wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	  iStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
	}
	GroupStitchIndex  = iStitch;
	ClosestPointIndex = iStitch;
	auto const color = gsl::narrow_cast<uint8_t>(StitchBuffer->operator[](iStitch).attribute & COLMSK);
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
	unbox();
	thred::grpAdj();
	ActiveColor = color;
	StateMap->set(StateFlag::RESTCH);
  }
}

void thred::internal::newFil() {
  StateMap->reset(StateFlag::CMPDO);
  bitmap::resetBmpFile(true);
  deldu();
  auto& desName = IniFile.designerName;
  DesignerName->assign(utf::Utf8ToUtf16(std::string(std::begin(desName))));
  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
  *ThrName = *DefaultDirectory / (StringTable->operator[](STR_NUFIL).c_str());
  ritfnam(*DesignerName);
  auto const designer     = utf::Utf16ToUtf8(*DesignerName);
  auto&      modName      = ExtendedHeader->modifierName;
  auto const modifierName = gsl::span<char> {modName};
  std::copy(designer.cbegin(), designer.cend(), modifierName.begin());
  rstdu();
  rstAll();
  displayText::clrhbut(3);
  StateMap->reset(StateFlag::MOVSET);
  unbox();
  xlin();
  xlin1();
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
  KnotCount = 0;
  WorkingFileName->clear();
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	thred::redraw(DefaultColorWin->operator[](iColor));
	thred::redraw(UserColorWin->operator[](iColor));
	thred::redraw(ThreadSizeWin[iColor]);
  }
  thred::zumhom();
}

void thred::bBox() {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::Polyline(StitchWindowDC, ZoomBoxLine.data(), wrap::toUnsigned(ZoomBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::unbBox() {
  if (StateMap->testAndReset(StateFlag::BZUM)) {
	thred::bBox();
  }
}

void thred::internal::rebox() {
  unbox();
  if (closPnt1(ClosestPointIndex)) {
	nuAct(ClosestPointIndex);
	auto stitchCoordsInPixels = POINT {};
	if (stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
	  dubox(stitchCoordsInPixels);
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
	  for (auto& window : *UserColorWin) {
		thred::redraw(window);
	  }
	}
	ritcor(StitchBuffer->operator[](ClosestPointIndex));
  }
}

void thred::delstchm() {
  thred::rngadj();
  auto const start = wrap::next(StitchBuffer->begin(), GroupStartStitch);
  auto const end   = wrap::next(StitchBuffer->begin(), gsl::narrow<ptrdiff_t>(GroupEndStitch + 1U));
  StitchBuffer->erase(start, end);
}

void thred::internal::duclp() {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::Polyline(StitchWindowDC, ClipInsertBoxLine.data(), wrap::toUnsigned(ClipInsertBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unclp() {
  if (StateMap->testAndReset(StateFlag::CLPSHO)) {
	duclp();
  }
}

void thred::internal::clpbox() {
  auto const ratio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  auto       stitchPoint = thred::pxCor2stch(Msg.pt);
  auto const unzoomedX   = wrap::toFloat(UnzoomedRect.x);
  if (stitchPoint.x + ClipRectSize.cx > unzoomedX) {
	stitchPoint.x = unzoomedX - ClipRectSize.cx;
  }
  auto const unzoomedY = wrap::toFloat(UnzoomedRect.y);
  if (stitchPoint.y + ClipRectSize.cy > unzoomedY) {
	stitchPoint.y = unzoomedY - ClipRectSize.cy;
  }
  ClipOrigin = {std::lround(stitchPoint.x), std::lround(stitchPoint.y)};
  auto const adjustedSize =
      SIZE {wrap::ceil<int32_t>(ClipRectSize.cx * ratio), wrap::ceil<int32_t>(ClipRectSize.cy * ratio)};
  auto const stitchCoordsInPixels =
      POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ratio),
             wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                                 (stitchPoint.y - ZoomRect.bottom) * ratio - adjustedSize.cy)};
  ClipInsertBoxLine[0].x = ClipInsertBoxLine[3].x = ClipInsertBoxLine[4].x = stitchCoordsInPixels.x;
  ClipInsertBoxLine[0].y = ClipInsertBoxLine[1].y = ClipInsertBoxLine[4].y = stitchCoordsInPixels.y;
  ClipInsertBoxLine[1].x = ClipInsertBoxLine[2].x = ClipInsertBoxLine[0].x + adjustedSize.cx;
  ClipInsertBoxLine[2].y = ClipInsertBoxLine[3].y = ClipInsertBoxLine[0].y + adjustedSize.cy;
  StateMap->set(StateFlag::CLPSHO);
  duclp();
}

void thred::internal::lodclp(uint32_t iStitch) {
  StitchBuffer->insert(wrap::next(StitchBuffer->begin(), iStitch), ClipBuffer->size(), fPOINTATTR {});
  ClosestPointIndex  = iStitch;
  auto const originX = wrap::toFloat(ClipOrigin.x);
  auto const originY = wrap::toFloat(ClipOrigin.y);
  for (auto& clip : *ClipBuffer) {
	StitchBuffer->operator[](iStitch++) =
	    fPOINTATTR {clip.x + originX,
	                clip.y + originY,
	                (clip.attribute & COLMSK) | gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT) | NOTFRM};
  }
  GroupStitchIndex = iStitch - 1U;
  StateMap->set(StateFlag::GRPSEL);
  if (!StitchBuffer->empty()) {
	StateMap->set(StateFlag::INIT);
  }
}

void thred::internal::rSelbox() {
  auto const ratio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
  auto const adjustedSelectSize = SIZE {gsl::narrow_cast<int32_t>(lround(SelectBoxSize.cx * ratio)),
                                        gsl::narrow_cast<int32_t>(lround(SelectBoxSize.cy * ratio))};
  unsel();
  auto stitchPoint = thred::pxCor2stch(Msg.pt);
  if (stitchPoint.x - wrap::toFloat(SelectBoxOffset.x + SelectBoxSize.cx) >=
      wrap::toFloat(UnzoomedRect.x)) {
	stitchPoint.x = wrap::toFloat(UnzoomedRect.x - SelectBoxSize.cx + SelectBoxOffset.x);
  }
  if (stitchPoint.y - wrap::toFloat(SelectBoxOffset.y + SelectBoxSize.cy) >=
      wrap::toFloat(UnzoomedRect.y)) {
	stitchPoint.y = wrap::toFloat(UnzoomedRect.y - SelectBoxSize.cy + SelectBoxOffset.y);
  }
  if (stitchPoint.x - wrap::toFloat(SelectBoxOffset.x) < 0.0F) {
	stitchPoint.x = wrap::toFloat(SelectBoxOffset.x);
  }
  if (stitchPoint.y - wrap::toFloat(SelectBoxOffset.y) < 0.0F) {
	stitchPoint.y = wrap::toFloat(SelectBoxOffset.y);
  }
  auto const stitchCoordsInPixels =
      POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left - SelectBoxOffset.x) * ratio),
             wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                                 (stitchPoint.y - ZoomRect.bottom - SelectBoxOffset.y) * ratio -
                                 adjustedSelectSize.cy)};
  auto& formControls = *FormControlPoints;
  formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x =
      stitchCoordsInPixels.x;
  formControls[1].x = formControls[5].x = stitchCoordsInPixels.x + adjustedSelectSize.cx / 2;
  formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y =
      stitchCoordsInPixels.y;
  formControls[3].y = formControls[7].y = stitchCoordsInPixels.y + adjustedSelectSize.cy / 2;
  formControls[4].y = formControls[5].y = formControls[6].y =
      stitchCoordsInPixels.y + adjustedSelectSize.cy;
  formControls[2].x = formControls[3].x = formControls[4].x =
      stitchCoordsInPixels.x + adjustedSelectSize.cx;
  StateMap->set(StateFlag::SELSHO);
  dusel(StitchWindowDC);
}

void thred::internal::duSelbox() noexcept {
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);

  SelectBoxSize = {std::lround(StitchRangeRect.right - StitchRangeRect.left),
                   std::lround(StitchRangeRect.top - StitchRangeRect.bottom)};

  SelectBoxOffset = {std::lround(stitchPoint.x - StitchRangeRect.left),
                     std::lround(stitchPoint.y - StitchRangeRect.bottom)};
}

void thred::internal::setbak(int32_t penWidth) noexcept {
  if (BackgroundPenWidth != penWidth) {
	BackgroundPenWidth = penWidth;
	BackgroundPen      = nuPen(BackgroundPen, penWidth, BackgroundColor);
  }
}

void thred::internal::stchbox(uint32_t iStitch, HDC dc) {
  auto       line   = std::array<POINT, SQPNTS> {};
  auto const layer  = (StitchBuffer->operator[](iStitch).attribute & LAYMSK) >> LAYSHFT;
  auto const offset = MulDiv(IniFile.stitchSizePixels, *screenDPI, STDDPI);
  if ((ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
	auto const stitchCoordsInPixels = stch2px1(iStitch);
	line[0].x = line[3].x = line[4].x = stitchCoordsInPixels.x - offset;
	line[0].y = line[1].y = stitchCoordsInPixels.y - offset;
	line[1].x = line[2].x = stitchCoordsInPixels.x + offset;
	line[2].y = line[3].y = stitchCoordsInPixels.y + offset;
	line[4].y             = stitchCoordsInPixels.y - offset;
	wrap::Polyline(dc, line.data(), wrap::toUnsigned(line.size()));
  }
}

void thred::sCor2px(fPOINT const& stitchCoordinate, POINT& pixelCoordinate) {
  pixelCoordinate = {wrap::ceil<int32_t>((stitchCoordinate.x - ZoomRect.left) * ZoomRatio.x),
                     wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) +
                                         (ZoomRect.bottom - stitchCoordinate.y) * ZoomRatio.y)};
}

auto thred::internal::sdCor2px(fPOINTATTR const& stitchPoint) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) +
                                    (ZoomRect.bottom - stitchPoint.y) * ZoomRatio.y)};
}

void thred::internal::durot() {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::Polyline(StitchWindowDC, RotateBoxOutline.data(), wrap::toUnsigned(RotateBoxOutline.size()));
  wrap::Polyline(
      StitchWindowDC, RotateBoxCrossVertLine.data(), wrap::toUnsigned(RotateBoxCrossVertLine.size()));
  wrap::Polyline(
      StitchWindowDC, RotateBoxCrossHorzLine.data(), wrap::toUnsigned(RotateBoxCrossHorzLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unrot() {
  if (StateMap->testAndReset(StateFlag::ROTSHO)) {
	durot();
  }
}

void thred::internal::durotu() {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  SelectObject(StitchWindowDC, LinePen);
  wrap::Polyline(
      StitchWindowDC, RotateBoxToCursorLine.data(), wrap::toUnsigned(RotateBoxToCursorLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unrotu() {
  if (StateMap->testAndReset(StateFlag::ROTUSHO)) {
	durotu();
  }
}

auto thred::rotangf(fPOINT const& unrotatedPoint, float const rotationAngle, fPOINT const& rotationCenter) noexcept
    -> fPOINT {
  auto const dx               = unrotatedPoint.x - rotationCenter.x;
  auto const dy               = unrotatedPoint.y - rotationCenter.y;
  auto const distanceToCenter = (dx != 0.0F) ? hypot(dx, dy) : std::abs(dy);
  auto       newAngle         = rotationAngle;
  if (dx != 0.0F) {
	newAngle += atan2(dy, dx);
  }
  else {
	if (dy > 0.0F) {
	  newAngle += PI_FHALF;
	}
	else {
	  newAngle -= PI_FHALF;
	}
  }
  return fPOINT {rotationCenter.x + distanceToCenter * cos(newAngle),
                 rotationCenter.y + distanceToCenter * sin(newAngle)};
}

void thred::internal::rotang(fPOINT unrotatedPoint, POINT& rotatedPoint, float rotationAngle, fPOINT const& rotationCenter) {
  auto const point = rotangf(unrotatedPoint, rotationAngle, rotationCenter);
  thred::sCor2px(point, rotatedPoint);
}

auto thred::rotang1(fPOINTATTR const& unrotatedPoint, float const rotationAngle, fPOINT const& rotationCenter) noexcept
    -> fPOINT {
  auto const point = fPOINT {unrotatedPoint.x, unrotatedPoint.y};
  return thred::rotangf(point, rotationAngle, rotationCenter);
}

void thred::rotflt(fPOINT& point, float const rotationAngle, fPOINT const& rotationCenter) noexcept {
  point = thred::rotangf(point, rotationAngle, rotationCenter);
}

void thred::internal::rotstch(fPOINTATTR& stitch, float const rotationAngle, fPOINT const& rotationCenter) noexcept {
  auto const point = thred::rotang1(stitch, rotationAngle, rotationCenter);
  stitch.x         = point.x;
  stitch.y         = point.y;
}

void thred::internal::ritrot(float rotationAngle, fPOINT const& rotationCenter) {
  if (StateMap->test(StateFlag::MOVCNTR)) { // set rotation angle to 0 if we are moving the center point
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
  StateMap->set(StateFlag::ROTSHO);
  durot();
}

void thred::internal::rot(fPOINT& rotationCenter) {
  do {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  break;
	}
	if (!SelectedFormList->empty()) {
	  break;
	}
	if (StateMap->test(StateFlag::BIGBOX)) {
	  break;
	}
	if (StateMap->test(StateFlag::GRPSEL)) {
	  break;
	}
	if (StateMap->test(StateFlag::FPSEL)) {
	  break;
	}
	// no rotatable selections found
	displayText::shoseln(IDS_FGRPF, IDS_ROT);
	return;
  } while (false);
  StateMap->set(StateFlag::ROTAT);
  rotationCenter = form::rotpar();
  ritrot(0, rotationCenter);
}

void thred::savclp(CLPSTCH& destination, fPOINTATTR const& source, uint32_t led) {
  auto integer    = 0.0F;
  destination.led = led;
  auto fractional = std::modf(source.x - LowerLeftStitch.x, &integer);
  destination.fx  = wrap::floor<decltype(destination.fx)>(fractional * FRACFACT);
  wrap::narrow(destination.x, integer);
  destination.spcx = 0;
  fractional       = std::modf(source.y - LowerLeftStitch.y, &integer);
  destination.fy   = wrap::floor<decltype(destination.fy)>(fractional * FRACFACT);
  wrap::narrow(destination.y, integer);
  destination.spcy = 0;
  // ToDo - Are these structure members needed?
  destination.myst = 1;
  destination.tag  = 0x14;
}

auto thred::internal::sizfclp(FRMHED const& form) -> uint32_t {
  auto clipSize = wrap::toUnsigned(sizeof(FORMCLIP)) +
                  form.vertexCount * wrap::toUnsigned(sizeof(decltype(FormVertices->back())));
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
  auto       iStitch        = StitchBuffer->begin();
  auto       stitchCount    = 0U;
  auto       flag           = true;
  for (; iStitch != StitchBuffer->end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && ((iStitch->attribute & TYPMSK) != 0U)) {
	  flag = false;
	  break;
	}
	++stitchCount;
  }
  if (flag) {
	return 0;
  }
  formFirstStitchIndex = stitchCount;
  stitchCount          = 0U;
  LowerLeftStitch      = fPOINT {BIGFLOAT, BIGFLOAT};
  for (; iStitch != StitchBuffer->end(); ++iStitch) {
	if ((iStitch->attribute & FRMSK) == codedAttribute && ((iStitch->attribute & TYPMSK) != 0U)) {
	  if (iStitch->x < LowerLeftStitch.x) {
		LowerLeftStitch.x = iStitch->x;
	  }
	  if (iStitch->y < LowerLeftStitch.y) {
		LowerLeftStitch.y = iStitch->y;
	  }
	  ++stitchCount;
	}
  }
  return stitchCount;
}

void thred::internal::sizclp(FRMHED const& form,
                             uint32_t&     formFirstStitchIndex,
                             uint32_t&     formStitchCount,
                             uint32_t&     length,
                             uint32_t&     fileSize) {
  fileSize = sizeof(FORMCLIP) + form.vertexCount * sizeof(decltype(FormVertices->back()));
  length   = fileSize;
  if (form.type == SAT) {
	fileSize += form.satinGuideCount * sizeof(decltype(SatinGuides->back()));
  }
  if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	formStitchCount = frmcnt(ClosestFormToCursor, formFirstStitchIndex);
	length += formStitchCount;
	fileSize += length * sizeof(StitchBuffer->front());
  }
  if (clip::iseclp(form)) {
	fileSize += form.clipEntries * sizeof(decltype(ClipPoints->back()));
  }
  if (clip::isclpx(form)) {
	fileSize += form.lengthOrCount.clipCount * sizeof(decltype(ClipPoints->back()));
  }
  if (texture::istx(form)) {
	fileSize += form.fillInfo.texture.count * sizeof(decltype(TexturePointsBuffer->back()));
  }
}

void thred::internal::duclip() {
  if (StateMap->test(StateFlag::FPSEL)) {
	if (OpenClipboard(ThrEdWindow) != 0) {
	  EmptyClipboard();
	  auto const thrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
	  // NOLINTNEXTLINE(hicpp-signed-bitwise, readability-qualified-auto)
	  auto clipHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
	                                (wrap::toSize(SelectedFormVertices.vertexCount) + 1U) * sizeof(fPOINT) +
	                                    sizeof(FORMVERTEXCLIP));
	  if (clipHandle != nullptr) {
		auto* clipHeader        = *(gsl::narrow_cast<FORMVERTEXCLIP**>(clipHandle));
		clipHeader->clipType    = CLP_FRMPS;
		clipHeader->vertexCount = SelectedFormVertices.vertexCount;
		clipHeader->direction   = StateMap->test(StateFlag::PSELDIR);
		// skip past the header
		auto*       vertices = convert_ptr<fPOINT*>(&clipHeader[1]);
		auto const& form     = FormList->operator[](ClosestFormToCursor);
		auto vertexIt        = wrap::next(FormVertices->cbegin(), form.vertexIndex);
		auto iSource         = SelectedFormVertices.start;
		for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		  auto sourceIt     = wrap::next(vertexIt, iSource);
		  vertices[iVertex] = *sourceIt;
		  iSource           = form::pdir(form, iSource);
		}
		SetClipboardData(thrEdClip, clipHandle);
	  }
	  else {
		throw;
	  }
	  CloseClipboard();
	}
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	displayText::tabmsg(IDS_INSF);
  }
  else {
	if (OpenClipboard(ThrEdWindow) != 0) {
	  EmptyClipboard();
	  auto const thrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
	  if (!SelectedFormList->empty()) {
		auto length = 0U;
		for (auto& selectedForm : (*SelectedFormList)) {
		  ClosestFormToCursor = selectedForm;
		  auto& currentForm   = FormList->operator[](selectedForm);
		  length += sizfclp(currentForm);
		}
		// NOLINTNEXTLINE(hicpp-signed-bitwise, readability-qualified-auto)
		auto clipHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length);
		if (clipHandle != nullptr) {
		  auto* clipFormsHeader     = *(gsl::narrow_cast<FORMSCLIP**>(clipHandle));
		  clipFormsHeader->clipType = CLP_FRMS;
		  wrap::narrow(clipFormsHeader->formCount, SelectedFormList->size());
		  // Skip past the header
		  auto* forms = convert_ptr<FRMHED*>(&clipFormsHeader[1]);
		  auto  iForm = 0U;
		  for (auto& selectedForm : (*SelectedFormList)) {
			auto& currentForm = FormList->operator[](selectedForm);
			forms[iForm++]    = currentForm;
		  }
		  // skip past the forms
		  auto* formVertices = convert_ptr<fPOINT*>(&forms[iForm]);
		  auto  iVertex      = 0U;
		  for (auto& selectedForm : (*SelectedFormList)) {
			// clang-format off
			  auto& form     = FormList->operator[](selectedForm);
			  auto  vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			// clang-format on
			for (auto iSide = 0U; iSide < form.vertexCount; ++iSide) {
			  formVertices[iVertex++] = *vertexIt;
			  ++vertexIt;
			}
		  }
		  // skip past the vertex list
		  auto* guides     = convert_ptr<SATCON*>(&formVertices[iVertex]);
		  auto  guideCount = 0U;
		  for (auto& selectedForm : (*SelectedFormList)) {
			auto& form = FormList->operator[](selectedForm);
			if (form.type == SAT) {
			  auto guideIt = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
			  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
				guides[guideCount++] = *guideIt;
				++guideIt;
			  }
			}
		  }
		  // skip past the guides
		  auto* points     = convert_ptr<fPOINT*>(&guides[guideCount]);
		  auto  pointCount = 0;
		  for (auto& selectedForm : (*SelectedFormList)) {
			auto& form = FormList->operator[](selectedForm);
			if (clip::isclpx(form)) {
			  auto offsetStart = wrap::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
			  for (auto iClip = 0U; iClip < form.lengthOrCount.clipCount; ++iClip) {
				points[pointCount++] = *offsetStart;
				++offsetStart;
			  }
			}
			if (clip::iseclp(form)) {
			  auto offsetStart = wrap::next(ClipPoints->cbegin(), form.borderClipData);
			  for (auto iClip = 0U; iClip < form.clipEntries; ++iClip) {
				points[pointCount++] = *offsetStart;
				++offsetStart;
			  }
			}
		  }
		  // Skip past the points
		  auto* textures     = convert_ptr<TXPNT*>(&points[pointCount]);
		  auto  textureCount = uint16_t {0U};
		  iForm              = 0;
		  for (auto& selectedForm : (*SelectedFormList)) {
			auto& form = FormList->operator[](selectedForm);
			if (texture::istx(selectedForm)) {
			  auto startPoint = wrap::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
			  auto       endPoint = wrap::next(startPoint, form.fillInfo.texture.count);
			  auto const dest = gsl::span<TXPNT>(&textures[textureCount], form.fillInfo.texture.count);
			  std::copy(startPoint, endPoint, dest.begin());
			  forms[iForm++].fillInfo.texture.index = textureCount;
			  textureCount += form.fillInfo.texture.count;
			}
		  }
		  SetClipboardData(thrEdClip, clipHandle);
		}
		CloseClipboard();
		auto formMap = boost::dynamic_bitset<>(FormList->size());
		for (auto& selectedForm : (*SelectedFormList)) {
		  formMap.set(selectedForm);
		}
		auto astch = std::vector<fPOINTATTR> {};
		// Reserve a reasonable amount space, probably not enough though
		constexpr auto RESRATIO = 16U; // reserve space factor
		astch.reserve(StitchBuffer->size() / RESRATIO);
		LowerLeftStitch.x = LowerLeftStitch.y = BIGFLOAT;
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
		  // NOLINTNEXTLINE(hicpp-signed-bitwise)
		  ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2U);
		  if (ClipPointer != nullptr) {
			ClipStitchData    = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
			auto iStitch      = 0U;
			auto iDestination = 0U;
			savclp(ClipStitchData[0], astch[0], stitchCount);
			++iStitch;
			++iDestination;
			while (iStitch < stitchCount) {
			  savclp(ClipStitchData[iDestination++], astch[iStitch], astch[iStitch].attribute & COLMSK);
			  ++iStitch;
			}
			SetClipboardData(Clip, ClipPointer);
		  }
		  CloseClipboard();
		}
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  // clang-format off
		  auto  firstStitch = 0U; // points to the first stitch in a form
		  auto  stitchCount = 0U;
		  auto& form        = FormList->operator[](ClosestFormToCursor);
		  auto  length      = 0U;
		  // clang-format on
		  auto clipSize = 0U;
		  sizclp(form, firstStitch, stitchCount, length, clipSize);
		  clipSize += sizeof(FORMCLIP);
		  // NOLINTNEXTLINE(hicpp-signed-bitwise, readability-qualified-auto)
		  auto clipHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, clipSize);
		  if (clipHandle != nullptr) {
			auto* clipFormHeader     = *(gsl::narrow_cast<FORMCLIP**>(clipHandle));
			clipFormHeader->clipType = CLP_FRM;
			clipFormHeader->form     = form;
			auto* formVertices       = convert_ptr<fPOINT*>(&clipFormHeader[1]);
			auto  vertexIt           = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			for (auto iSide = 0U; iSide < form.vertexCount; ++iSide) {
			  formVertices[iSide] = *vertexIt;
			  ++vertexIt;
			}
			auto* guides = convert_ptr<SATCON*>(&formVertices[form.vertexCount]);
			auto  iGuide = 0U;
			if (form.type == SAT) {
			  auto guideIt = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
			  for (iGuide = 0; iGuide < form.satinGuideCount; ++iGuide) {
				guides[iGuide] = *guideIt;
				++guideIt;
			  }
			}
			auto* mclp  = convert_ptr<fPOINT*>(&guides[iGuide]);
			auto  iClip = 0U;
			if (clip::isclpx(ClosestFormToCursor)) {
			  auto offsetStart = wrap::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
			  for (iClip = 0; iClip < form.lengthOrCount.clipCount; ++iClip) {
				mclp[iClip] = *offsetStart;
				++offsetStart;
			  }
			}
			auto* points = convert_ptr<fPOINT*>(&mclp[iClip]);
			if (clip::iseclpx(ClosestFormToCursor)) {
			  auto offsetStart = wrap::next(ClipPoints->cbegin(), form.borderClipData);
			  for (iClip = 0; iClip < form.clipEntries; ++iClip) {
				points[iClip] = *offsetStart;
				++offsetStart;
			  }
			}
			auto* textures = convert_ptr<TXPNT*>(&points[iClip]);
			if (texture::istx(ClosestFormToCursor)) {
			  auto startPoint = wrap::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
			  auto       endPoint = wrap::next(startPoint, form.fillInfo.texture.count);
			  auto const dest     = gsl::span<TXPNT>(textures, form.fillInfo.texture.count);
			  std::copy(startPoint, endPoint, dest.begin());
			}
			SetClipboardData(thrEdClip, clipHandle);
		  }
		  if (((form.fillType != 0U) || (form.edgeType != 0U))) {
			Clip = RegisterClipboardFormat(PcdClipFormat);
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2U);
			if (ClipPointer != nullptr) {
			  ClipStitchData = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
			  auto iTexture  = firstStitch;
			  savclp(ClipStitchData[0], StitchBuffer->operator[](iTexture), length);
			  ++iTexture;
			  auto       iDestination   = 1U;
			  auto const codedAttribute = ClosestFormToCursor << FRMSHFT;
			  while (iTexture < StitchBuffer->size()) {
				if ((StitchBuffer->operator[](iTexture).attribute & FRMSK) == codedAttribute &&
				    ((StitchBuffer->operator[](iTexture).attribute & NOTFRM) == 0U)) {
				  savclp(ClipStitchData[iDestination++],
				         StitchBuffer->operator[](iTexture),
				         (StitchBuffer->operator[](iTexture).attribute & COLMSK));
				}
				++iTexture;
			  }
			  SetClipboardData(Clip, ClipPointer);
			}
			form::ispcdclp();
		  }
		  CloseClipboard();
		}
		else {
		  if ((!StitchBuffer->empty()) && StateMap->test(StateFlag::GRPSEL)) {
			Clip = RegisterClipboardFormat(PcdClipFormat);
			thred::rngadj();
			LowerLeftStitch.x = LowerLeftStitch.y = BIGFLOAT;
			for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
			  auto const& stitch = StitchBuffer->operator[](iStitch);
			  if (stitch.x < LowerLeftStitch.x) {
				LowerLeftStitch.x = stitch.x;
			  }
			  if (stitch.y < LowerLeftStitch.y) {
				LowerLeftStitch.y = stitch.y;
			  }
			}
			auto const length  = GroupEndStitch - GroupStartStitch + 1U;
			auto       iSource = GroupStartStitch;
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length * sizeof(CLPSTCH) + 2U);
			if (ClipPointer != nullptr) {
			  ClipStitchData = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
			  savclp(ClipStitchData[0], StitchBuffer->operator[](iSource), length);
			  ++iSource;
			  for (auto iStitch = 1U; iStitch < length; ++iStitch) {
				savclp(ClipStitchData[iStitch],
				       StitchBuffer->operator[](iSource),
				       (StitchBuffer->operator[](iSource).attribute & COLMSK));
				++iSource;
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
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  // find the first stitch to delete
  auto const firstStitch =
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
  if (StateMap->test(StateFlag::DELTO)) {
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
  auto const firstForm = FormList->cbegin();
  FormList->erase(wrap::next(firstForm, ClosestFormToCursor));
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  if (StateMap->testAndReset(StateFlag::DELTO)) {
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
  StateMap->reset(StateFlag::FORMSEL);
  if (ClosestFormToCursor > (FormList->size() - 1U)) {
	ClosestFormToCursor = 0U;
  }
}

void thred::internal::delsfrms(wchar_t code) {
  if (code != 0) {
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
	  for (auto iForm = 0U; iForm < formCount; ++iForm) {
		if (!formMap.test(iForm)) {
		  formIndices[iForm] = (iForm - deletedFormCount) << FRMSHFT;
		}
		else {
		  FormList->erase(wrap::next(firstForm, (iForm - deletedFormCount)));
		  firstForm = FormList->cbegin(); // the erase invalidates firstForm
		  ++deletedFormCount;
		}
	  }
	  if (StateMap->test(StateFlag::DELTO)) {
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
	  StateMap->reset(StateFlag::FORMSEL);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::cut() {
  thred::savdo();
  duclip();
  if (!SelectedFormList->empty()) {
	StateMap->set(StateFlag::DELTO);
	delsfrms(L's');
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::delfstchs();
	  thred::frmdel();
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::delstchm();
	  }
	}
  }
  thred::coltab();
  rstAll();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::unpat() {
  if (StateMap->testAndReset(StateFlag::WASPAT)) {
	ShowWindow(SpeedScrollBar, FALSE);
	DestroyWindow(SpeedScrollBar);
	thred::movStch();
	StateMap->set(StateFlag::RESTCH);
  }
}

auto thred::internal::cmpstch(uint32_t iStitchA, uint32_t iStitchB) noexcept -> bool {
  return (StitchBuffer->operator[](iStitchA).x != StitchBuffer->operator[](iStitchB).x)
             ? false
             : StitchBuffer->operator[](iStitchA).y == StitchBuffer->operator[](iStitchB).y;
}

void thred::internal::ofstch(std::vector<fPOINTATTR>& buffer,
                             uint32_t                 iSource,
                             char                     offset,
                             fPOINT const&            knotStep,
                             uint32_t                 knotAttribute) {
  buffer.emplace_back(fPOINTATTR {StitchBuffer->operator[](iSource).x + knotStep.x * wrap::toFloat(offset),
                                  StitchBuffer->operator[](iSource).y + knotStep.y * wrap::toFloat(offset),
                                  knotAttribute});
}

void thred::internal::endknt(std::vector<fPOINTATTR>& buffer, uint32_t finish) {
  auto const finishIt      = wrap::next(StitchBuffer->begin(), finish);
  auto       startIt       = (finish != 0) ? std::next(finishIt, -1) : finishIt;
  auto const knotAttribute = startIt->attribute | KNOTMSK;
  auto       delta         = fPOINT {finishIt->x - startIt->x, finishIt->y - startIt->y};
  auto       length        = hypot(delta.x, delta.y);
  while (length == 0.0F && startIt != StitchBuffer->begin()) {
	delta  = fPOINT {finishIt->x - startIt->x, finishIt->y - startIt->y};
	length = hypot(delta.x, delta.y);
	--startIt;
  }
  delta               = fPOINT {finishIt->x - startIt->x, finishIt->y - startIt->y};
  auto const knotStep = fPOINT {2.0F / length * delta.x, 2.0F / length * delta.y};
  constexpr auto knotAtEndOrder = std::array<char, KNOTSCNT> {-2, -3, -1, -4, 0}; // reverse knot spacings
  constexpr auto knotAtLastOrder = std::array<char, KNOTSCNT> {0, -4, -1, -3, -2}; // reverse knot spacings
  auto const& knots = (StateMap->test(StateFlag::FILDIR)) ? knotAtLastOrder : knotAtEndOrder;
  for (auto knot : knots) {
	ofstch(buffer, finish, knot, knotStep, knotAttribute);
  }
  if (StateMap->test(StateFlag::FILDIR)) {
	ofstch(buffer, finish, 0, knotStep, knotAttribute);
  }
}

void thred::internal::strtknt(std::vector<fPOINTATTR>& buffer, uint32_t start) {
  auto const startIt  = wrap::next(StitchBuffer->begin(), start);
  auto       finishIt = std::next(startIt);
  auto       delta    = fPOINT {finishIt->x - startIt->x, finishIt->y - startIt->y};
  auto       length   = hypot(delta.x, delta.y);
  ++finishIt;
  while (length < 2.0F && finishIt != StitchBuffer->end()) {
	delta  = fPOINT {finishIt->x - startIt->x, finishIt->y - startIt->y};
	length = hypot(delta.x, delta.y);
	++finishIt;
  }
  if (finishIt != StitchBuffer->end()) {
	delta                         = fPOINT {finishIt->x - startIt->x, finishIt->y - startIt->y};
	auto const knotAttribute      = startIt->attribute | KNOTMSK;
	auto const knotStep           = fPOINT {2.0F / length * delta.x, 2.0F / length * delta.y};
	static constexpr auto knotAtStartOrder = std::array < char , 5>{2, 3, 1, 4, 0}; // knot spacings
	for (char const iKnot : knotAtStartOrder) {
	  ofstch(buffer, start, iKnot, knotStep, knotAttribute);
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
  auto const firstStitch =
      std::find_if(StitchBuffer->begin(), StitchBuffer->end(), [](fPOINTATTR const& m) -> bool {
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
	StateMap->set(StateFlag::RESTCH);
  }
}

auto thred::internal::kjmp(std::vector<fPOINTATTR>& buffer, uint32_t start) -> uint32_t {
  while (start < wrap::toUnsigned(StitchBuffer->size() - 1U) && stlen(start) > KNOTLEN) {
	buffer.push_back(StitchBuffer->operator[](start++));
  }
  strtknt(buffer, start);
  return start;
}

void thred::internal::setknt() {
  auto buffer = std::vector<fPOINTATTR> {};
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
  StitchBuffer->resize(buffer.size());
  *StitchBuffer = buffer;
}

auto thred::internal::srchknot(uint32_t source) noexcept -> uint32_t {
  auto knotIndex = 0U;
  while (knotIndex < KnotCount && Knots[knotIndex] < source) {
	++knotIndex;
  }
  if (knotIndex != 0U) {
	--knotIndex;
  }
  if (((Knots[knotIndex] > source) ? (Knots[knotIndex] - source) : (source - Knots[knotIndex])) < KNOTSCNT) {
	++knotIndex;
	if (((Knots[knotIndex] > source) ? (Knots[knotIndex] - source) : (source - Knots[knotIndex])) < KNOTSCNT) {
	  return 0;
	}
	return 2;
  }

  ++knotIndex;
  if (((Knots[knotIndex] > source) ? (Knots[knotIndex] - source) : (source - Knots[knotIndex])) < KNOTSCNT) {
	return 1;
  }
  return 3;
}

void thred::internal::chkncol() {
  auto initialColor = StitchBuffer->front().attribute & COLMSK;
  auto buffer       = std::vector<fPOINTATTR> {};
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
	StateMap->set(StateFlag::RESTCH);
	thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  }
}

void thred::internal::duhbit(uint32_t cod) noexcept {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_HIDBIT, MF_BYCOMMAND | cod);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_HIDBITF, MF_BYCOMMAND | cod);
}

void thred::hidbit() {
  if (StateMap->testAndFlip(StateFlag::HIDMAP)) {
	thi::duhbit(MF_UNCHECKED);
  }
  else {
	thi::duhbit(MF_CHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::drwlstch(uint32_t finish) {
  auto           movieLine = std::vector<POINT> {}; // line for movie stitch draw
  constexpr auto LNMAXPTS  = 100U;                  // maximum number of points in a line
  movieLine.reserve(LNMAXPTS);
  if (finish != 0) {
	if (StateMap->test(StateFlag::HID)) {
	  while (RunPoint < (finish - 1) && (StitchBuffer->operator[](RunPoint).attribute & COLMSK) != ActiveColor) {
		++RunPoint;
	  }
	}
	if (StateMap->test(StateFlag::ZUMED)) {
	  auto const stitchCoordsInPixels = POINT {};
	  movieLine.push_back(stitchCoordsInPixels);
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
		if (stch2px(RunPoint, stitchCoordsInPixels)) {
		  movieLine.push_back(stitchCoordsInPixels);
		  if (flag) {
			flag = false;
			if ((RunPoint != 0U) && stch2px(RunPoint - 1, stitchCoordsInPixels)) {
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
	  if (!stch2px(RunPoint, stitchCoordsInPixels)) {
		if ((StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color) {
		  movieLine.push_back(stitchCoordsInPixels);
		  ++RunPoint;
		}
	  }
	  SelectObject(StitchWindowDC, UserPen[color]);
	  wrap::Polyline(StitchWindowDC, movieLine.data(), wrap::toUnsigned(movieLine.size()));
	  if (!flag) {
		--RunPoint;
	  }
	}
	else {
	  auto iMovieFrame = 0U;

	  SelectObject(StitchWindowDC, UserPen[color]);
	  while (iMovieFrame < StitchesPerFrame && (RunPoint + 1 < finish - 1) &&
	         ((StitchBuffer->operator[](RunPoint).attribute & COLMSK) == color)) {
		movieLine.push_back(stch2px1(RunPoint++));
		++iMovieFrame;
	  }
	  --RunPoint;
	  wrap::Polyline(StitchWindowDC, movieLine.data(), wrap::toUnsigned(movieLine.size()));
	}
	if ((StitchBuffer->operator[](wrap::toSize(RunPoint) + 1U).attribute & COLMSK) != color) {
	  ++RunPoint;
	}
	displayText::ritnum(STR_NUMSEL, RunPoint);
	if (RunPoint + 3U > finish - 1U) {
	  patdun();
	}
  }
  else {
	patdun();
  }
}

void thred::internal::stchout() {
  if (StateMap->test(StateFlag::GRPSEL)) {
	drwlstch(GroupEndStitch);
  }
  else {
	drwlstch(wrap::toUnsigned(StitchBuffer->size() - 1U));
  }
}

void thred::internal::setsped() {
  auto elapsedTimePerFrame = 100U; // units are millseconds
  if (MovieTimeStep == 0) {
	MovieTimeStep = 1;
  }
  auto const userTimePerFrame = gsl::narrow_cast<double>(MovieTimeStep) / 10.0;
  if (userTimePerFrame < 10.0) {
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

void thred::internal::deltot() {
  auto& desName = IniFile.designerName;
  DesignerName->assign(utf::Utf8ToUtf16(std::string(std::begin(desName))));
  TexturePointsBuffer->clear();
  FormList->clear();
  StitchBuffer->clear();
  FormVertices->clear();
  SatinGuides->clear();
  StateMap->reset(StateFlag::GMRK);
  rstAll();
  thred::coltab();
  thred::zumhom();
  SetWindowText(ThrEdWindow,
                fmt::format(StringTable->operator[](STR_THRDBY), ThrName->wstring(), *DesignerName).c_str());
}

auto thred::internal::wastch(uint32_t const& formIndex) -> bool {
  return std::any_of(StitchBuffer->begin(), StitchBuffer->end(), [&formIndex](fPOINTATTR const& m) -> bool {
	return (m.attribute & FRMSK) >> FRMSHFT == formIndex;
  });
}

auto thred::internal::frmstch() -> bool {
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto form : (*SelectedFormList)) {
	formMap.set(form);
  }
  return std::any_of(StitchBuffer->begin(), StitchBuffer->end(), [&formMap](fPOINTATTR const& m) -> bool {
	return formMap.test((m.attribute & FRMSK) >> FRMSHFT);
  });
}

void thred::internal::delet() {
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
	auto       vCurr    = vBegin;
	auto       vertexIt = vBegin;
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  if (!vertexMap.test(iVertex)) {
		*vCurr = *vertexIt;
		++vCurr;
	  }
	  ++vertexIt;
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
	form::frmout(ClosestFormToCursor);
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

	  // ToDo - Is there a better way to do this than iterating through?
	  auto guideIt = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		auto newGuideVal = 0U;
		for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		  if (vertexMap.test(iVertex)) {
			if (guideIt->finish == iVertex) {
			  guideIt->finish = guideIt->start;
			}
			if (guideIt->start == iVertex) {
			  guideIt->start = guideIt->finish;
			}
		  }
		  else {
			if (guideIt->finish == iVertex) {
			  guideIt->finish = newGuideVal;
			}
			if (guideIt->start == iVertex) {
			  guideIt->start = newGuideVal;
			}
			++newGuideVal;
		  }
		}
		++guideIt;
	  }
	  satin::satadj(form);
	}
	form::refil();
	fndknt();
	StateMap->set(StateFlag::RESTCH);
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
		StateMap->set(StateFlag::DELSFRMS);
		displayText::tabmsg(IDS_DELFRM);
		displayText::okcan();
		displayText::tomsg();
	  }
	  else {
		delsfrms(1);
	  }
	  return;
	}
	if (StateMap->test(StateFlag::FORMSEL) && !FormList->empty()) {
	  if (wastch(ClosestFormToCursor)) {
		StateMap->set(StateFlag::DELFRM);
		displayText::tabmsg(IDS_FDEL);
		displayText::okcan();
		displayText::tomsg();
	  }
	  else {
		if (!FormList->empty()) {
		  StateMap->reset(StateFlag::DELTO);
		  thred::frmdel();
		  StateMap->set(StateFlag::RESTCH);
		}
	  }
	  fndknt();
	  return;
	}
	if (StateMap->test(StateFlag::SELBOX)) {
	  if (StitchBuffer->size() > 2U) {
		delstch1(ClosestPointIndex);
		if (!stch2px2(ClosestPointIndex)) {
		  movbox();
		}
	  }
	  else {
		StitchBuffer->clear();
		StateMap->reset(StateFlag::SELBOX);
	  }
	  thred::coltab();
	  xt::setfchk();
	  fndknt();
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap->test(StateFlag::GRPSEL)) {
	  thred::delstchm();
	  thred::coltab();
	  rstAll();
	  xt::setfchk();
	  fndknt();
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap->test(StateFlag::FRMPSEL) || form::closfrm()) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  switch (form.type) {
		case FRMLINE: {
		  if (form.fillType == CONTF) {
			if (ClosestVertexToCursor == form.angleOrClipData.guide.start ||
			    ClosestVertexToCursor == form.angleOrClipData.guide.finish) {
			  form::delmfil(ClosestFormToCursor);
			  form.fillType = 0;
			  thred::coltab();
			  StateMap->set(StateFlag::RESTCH);
			  return;
			}
			if (form.angleOrClipData.guide.start > ClosestVertexToCursor) {
			  --(form.angleOrClipData.guide.start);
			}
			if (form.angleOrClipData.guide.finish > ClosestVertexToCursor) {
			  --(form.angleOrClipData.guide.finish);
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
			auto guideIt = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
			  if (guideIt->start == ClosestVertexToCursor || guideIt->finish == ClosestVertexToCursor) {
				satin::delcon(form, iGuide);
				satinFlag = true;
				break;
			  }
			  ++guideIt;
			}
		  } while (false);
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
	  if (!FormList->empty()) {
		form::frmout(ClosestFormToCursor);
		form::refil();
	  }
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	if (!satinFlag && closPnt1(ClosestPointIndex)) {
	  if (StitchBuffer->size() > 2U) {
		delstch1(ClosestPointIndex);
		if (!stch2px2(ClosestPointIndex)) {
		  movbox();
		}
	  }
	  else {
		StitchBuffer->clear();
		StateMap->reset(StateFlag::SELBOX);
	  }
	  thred::coltab();
	  xt::setfchk();
	  fndknt();
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
  }
  fndknt();
}

void thred::internal::movi() {
  if (StateMap->test(StateFlag::GRPSEL)) {
	rstAll();
	StateMap->set(StateFlag::GRPSEL);
  }
  else {
	rstAll();
  }
  if (!StitchBuffer->empty()) {
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
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
	auto const stepCount = (StateMap->test(StateFlag::ZUMED))
	                           ? gsl::narrow_cast<float>(StitchBuffer->size()) * ZoomFactor * ZoomFactor
	                           : gsl::narrow_cast<float>(StitchBuffer->size());
	if (!StateMap->test(StateFlag::WASPAT)) {
	  MovieTimeStep = wrap::round<decltype(MovieTimeStep)>(10000.0F * MOVITIM / stepCount);
	}
	if (MovieTimeStep < MINDELAY) {
	  MovieTimeStep = MINDELAY;
	}
	if (MovieTimeStep > MAXDELAY) {
	  MovieTimeStep = MAXDELAY;
	}
	auto scrollInfo   = SCROLLINFO {}; // scroll bar i/o structure
	scrollInfo.cbSize = sizeof(scrollInfo);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMax  = MAXDELAY;
	scrollInfo.nMin  = MINDELAY;
	scrollInfo.nPage = 1;
	scrollInfo.nPos  = MAXDELAY - MovieTimeStep;
	SetScrollInfo(SpeedScrollBar, SB_CTL, &scrollInfo, TRUE);
	FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
	setsped();
  }
}

void thred::redclp() {
  auto const codedLayer = gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT);
  ClipPointer           = GlobalLock(ClipMemory);
  if (ClipPointer != nullptr) {
#pragma warning(suppress : 26429) // f.23 Symbol is never tested for nullness, it can be marked as not_null
	auto* const clipStitchPtr  = gsl::narrow_cast<CLPSTCH*>(ClipPointer);
	auto const  clipSize       = clipStitchPtr[0].led;
	auto const  clipStitchData = gsl::span<CLPSTCH>(clipStitchPtr, clipSize);
	auto&       clipBuffer     = *ClipBuffer;
	clipBuffer.clear();
	clipBuffer.reserve(clipSize);
	clipBuffer.emplace_back(wrap::toFloat(clipStitchData[0].x) + wrap::toFloat(clipStitchData[0].fx) / FRACFACT,
	                        wrap::toFloat(clipStitchData[0].y) + wrap::toFloat(clipStitchData[0].fy) / FRACFACT,
	                        0U);

#if CLPBUG
	OutputDebugString(fmt::format(L"redclp:interator [0] x [{:6.2F}] y [{:6.2F}]\n",
	                              clipBuffer.back().x,
	                              clipBuffer.back().y)
	                      .c_str());
#endif
	auto clipRect = fRECTANGLE {clipBuffer[0].x, clipBuffer[0].y, clipBuffer[0].x, clipBuffer[0].y};
	for (auto iStitch = 1U; iStitch < clipSize; ++iStitch) {
	  clipBuffer.emplace_back(
	      wrap::toFloat(clipStitchData[iStitch].x) + wrap::toFloat(clipStitchData[iStitch].fx) / FRACFACT,
	      wrap::toFloat(clipStitchData[iStitch].y) + wrap::toFloat(clipStitchData[iStitch].fy) / FRACFACT,
	      (clipStitchData[iStitch].led & COLMSK) | codedLayer);

#if CLPBUG
	  OutputDebugString(fmt::format(L"redclp:interator [{}] x [{:6.2F}] y [{:6.2F}]\n",
	                                iStitch,
	                                clipBuffer.back().x,
	                                clipBuffer.back().y)
	                        .c_str());
#endif
	  if (clipBuffer.back().x < clipRect.left) {
		clipRect.left = clipBuffer.back().x;
	  }
	  if (clipBuffer.back().x > clipRect.right) {
		clipRect.right = clipBuffer.back().x;
	  }
	  if (clipBuffer.back().y < clipRect.bottom) {
		clipRect.bottom = clipBuffer.back().y;
	  }
	  if (clipBuffer.back().y > clipRect.top) {
		clipRect.top = clipBuffer.back().y;
	  }
	}
	clipBuffer.front().attribute = ActiveColor | codedLayer;
	ClipRectSize                 = {clipRect.right - clipRect.left, clipRect.top - clipRect.bottom};
	GlobalUnlock(ClipMemory);
	if ((clipRect.left != 0.0F) || (clipRect.bottom != 0.0F)) {
	  for (auto& clip : clipBuffer) {
		clip.x -= clipRect.left;
		clip.y -= clipRect.bottom;
	  }
	}
  }
}

constexpr auto thred::internal::nxtcrnr(uint32_t corner) -> uint32_t {
  return (corner + 1U) & 3U;
}

void thred::internal::drwmrk(HDC dc) {
  auto       markCoordinates = POINT {0L, 0L};
  auto       markLine        = std::array<POINT, 2> {};
  auto const markOffset      = MulDiv(6, *screenDPI, STDDPI);
  thred::sCor2px(fPOINT {ZoomMarkPoint}, markCoordinates);
  SelectObject(dc, ZoomMarkPen);
  SetROP2(dc, R2_XORPEN);
  markLine[0] = {markCoordinates.x - markOffset, markCoordinates.y - markOffset};
  markLine[1] = {markCoordinates.x + markOffset, markCoordinates.y + markOffset};
  wrap::Polyline(dc, markLine.data(), wrap::toUnsigned(markLine.size()));
  markLine[0] = {markCoordinates.x - markOffset, markCoordinates.y + markOffset};
  markLine[1] = {markCoordinates.x + markOffset, markCoordinates.y - markOffset};
  wrap::Polyline(dc, markLine.data(), wrap::toUnsigned(markLine.size()));
  SetROP2(dc, R2_COPYPEN);
}

void thred::internal::vubak() {
  if (!WorkingFileName->empty() || StateMap->test(StateFlag::THUMSHO)) {
	StateMap->set(StateFlag::ZUMED);
	thred::movStch();
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	auto const dx = StitchWindowClientRect.right / 2;
	auto const dy = StitchWindowClientRect.bottom / 2;
	for (auto iVersion = 0U; iVersion < 4; ++iVersion) { // there are 4 quadrants
	  auto verticalLocation = 0L;
	  if ((iVersion & 2U) != 0U) {
		verticalLocation = dy;
	  }
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  BackupViewer[iVersion] = CreateWindow(L"STATIC",
	                                        L"",
	                                        SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                        dx * gsl::narrow_cast<int32_t>(iVersion & 1U) + ButtonWidthX3,
	                                        verticalLocation,
	                                        dx,
	                                        dy,
	                                        ThrEdWindow,
	                                        nullptr,
	                                        ThrEdInstance,
	                                        nullptr);
	}
	StateMap->set(StateFlag::BAKSHO);
  }
}

void thred::insflin(POINT insertPoint) {
  auto const offset    = POINT {InsertSize.x / 2, InsertSize.y / 2};
  auto&      formLines = *FormLines;
  formLines.resize(SQPNTS);
  formLines[0].x = formLines[3].x = formLines[4].x = insertPoint.x - offset.x;
  formLines[1].x = formLines[2].x = insertPoint.x + offset.x;
  formLines[0].y = formLines[1].y = formLines[4].y = insertPoint.y - offset.y;
  formLines[2].y = formLines[3].y = insertPoint.y + offset.y;
}

auto thred::internal::isthr(fs::path const& thredPath) -> bool {
  auto const extention = thredPath.extension().wstring();
  return (extention.compare(0, 3, L".th") == 0);
}

auto thred::internal::gethand(std::vector<fPOINTATTR> const& stitch, uint32_t stitchCount) noexcept -> uint32_t {
  auto userStitchCount = 0U;
  for (auto iStitch = 0U; iStitch < stitchCount; ++iStitch) {
	if ((stitch[iStitch].attribute & USMSK) != 0U) {
	  ++userStitchCount;
	}
  }
  return userStitchCount;
}

void thred::internal::insfil(fs::path& insertedFile) {
  auto successFlag       = false;
  auto insertedRectangle = fRECTANGLE {BIGFLOAT, TNYFLOAT, TNYFLOAT, BIGFLOAT};
  if (insertedFile.empty()) {
	getNewFileName(insertedFile, fileStyles::INS_FILES, fileIndices::THR);
  }
  InsertedStitchIndex = wrap::toUnsigned(StitchBuffer->size());
  if (isthr(insertedFile)) {
	if (insTHR(insertedFile, insertedRectangle)) {
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
  if (successFlag) {
	InsertCenter = fPOINT {wrap::midl(insertedRectangle.right, insertedRectangle.left),
	                       wrap::midl(insertedRectangle.top, insertedRectangle.bottom)};

	auto const insertedSize = fPOINT {insertedRectangle.right - insertedRectangle.left,
	                                  insertedRectangle.top - insertedRectangle.bottom};
	form::ratsr();
	InsertSize.x = std::lround(insertedSize.x * HorizontalRatio);
	// ToDo - Should this be vertical ratio?
	InsertSize.y = std::lround(insertedSize.y * HorizontalRatio);
	auto const initialInsertPoint =
	    POINT {StitchWindowClientRect.right / 2, StitchWindowClientRect.bottom / 2};
	thred::insflin(initialInsertPoint);
	NewFormVertexCount = 5U;
	StateMap->set(StateFlag::SHOFRM);
	StateMap->set(StateFlag::INSFIL);
	form::dufrm();
  }
}

auto thred::internal::insTHR(fs::path const& insertedFile, fRECTANGLE& insertedRectangle) -> bool {
  auto retflag = true;
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle =
      CreateFile(insertedFile.wstring().c_str(), (GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::filnopn(IDS_FNOPN, insertedFile);
	retflag = false;
  }
  else {
	auto fileHeader = STRHED {};
	auto bytesRead  = DWORD {0};
	ReadFile(fileHandle, &fileHeader, sizeof(fileHeader), &bytesRead, nullptr);
	if ((fileHeader.headerType & SIGMASK) != THREDSIG) {
	  displayText::tabmsg(IDS_NOTHR);
	  retflag = false;
	}
	else {
	  constexpr auto FRMW        = 5;
	  constexpr auto HANDW       = 4;
	  constexpr auto FRMPW       = 2;
	  constexpr auto STCHW       = 1;
	  auto           homscor     = 0U;
	  auto           thredHeader = STREX {};
	  auto const     version     = (fileHeader.headerType & FTYPMASK) >> TBYTSHFT;
	  if (version != 0U) {
		gethand(*StitchBuffer, wrap::toUnsigned(StitchBuffer->size()));
		// ToDo - replace constants with sizes of data structures?
		homscor = wrap::toUnsigned(FormList->size()) * FRMW +
		          gethand(*StitchBuffer, wrap::toUnsigned(StitchBuffer->size())) * HANDW +
		          wrap::toUnsigned(FormVertices->size()) * FRMPW +
		          wrap::toUnsigned(StitchBuffer->size()) * STCHW;
		ReadFile(fileHandle, &thredHeader, sizeof(thredHeader), &bytesRead, nullptr);
	  }
	  thred::savdo();
	  auto fileStitchBuffer = std::vector<fPOINTATTR> {};
	  if (fileHeader.stitchCount != 0U) {
		fileStitchBuffer.resize(fileHeader.stitchCount);
		ReadFile(fileHandle,
		         fileStitchBuffer.data(),
		         fileHeader.stitchCount * sizeof(fileStitchBuffer.back()),
		         &bytesRead,
		         nullptr);
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
		  auto inFormList = std::vector<FRMHEDO> {};
		  inFormList.resize(fileHeader.formCount);
		  auto const bytesToRead =
		      fileHeader.formCount * wrap::toUnsigned(sizeof(decltype(inFormList.back())));
		  ReadFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, nullptr);
		  if (bytesRead != fileHeader.formCount * sizeof(decltype(inFormList.back()))) {
			inFormList.resize(bytesRead / sizeof(decltype(inFormList.back())));
			StateMap->set(StateFlag::BADFIL);
		  }
		  FormList->reserve(FormList->size() + inFormList.size());
		  FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
		}
		else {
		  auto inFormList = std::vector<FRMHEDOUT> {};
		  inFormList.resize(fileHeader.formCount);
		  auto const bytesToRead =
		      fileHeader.formCount * wrap::toUnsigned(sizeof(decltype(inFormList.back())));
		  wrap::ReadFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, nullptr);
		  if (bytesRead != bytesToRead) {
			inFormList.resize(bytesRead / sizeof(decltype(inFormList.back())));
			StateMap->set(StateFlag::BADFIL);
		  }
		  FormList->reserve(FormList->size() + inFormList.size());
		  FormList->insert(FormList->end(), inFormList.begin(), inFormList.end());
		}
		auto vertexOffset = wrap::toUnsigned(FormVertices->size());
		if (fileHeader.vertexCount != 0U) {
		  auto inVerticeList = std::vector<fPOINT> {};
		  inVerticeList.resize(fileHeader.vertexCount);
		  auto const bytesToRead =
		      fileHeader.vertexCount * wrap::toUnsigned(sizeof(decltype(inVerticeList.back())));
		  ReadFile(fileHandle, inVerticeList.data(), bytesToRead, &bytesRead, nullptr);
		  if (bytesRead != bytesToRead) {
			inVerticeList.resize(bytesRead / sizeof(decltype(inVerticeList.back())));
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
		  auto inGuideList = std::vector<SATCONOUT> {};
		  inGuideList.resize(fileHeader.dlineCount);
		  auto const bytesToRead =
		      fileHeader.dlineCount * wrap::toUnsigned(sizeof(decltype(inGuideList.back())));
		  ReadFile(fileHandle, inGuideList.data(), bytesToRead, &bytesRead, nullptr);
		  if (bytesRead != bytesToRead) {
			inGuideList.resize(bytesRead / sizeof(decltype(inGuideList.back())));
			StateMap->set(StateFlag::BADFIL);
		  }
		  SatinGuides->reserve(SatinGuides->size() + inGuideList.size());
		  SatinGuides->insert(SatinGuides->end(), inGuideList.begin(), inGuideList.end());
		  newSatinGuideIndex += wrap::toUnsigned(inGuideList.size());
		}
		if (fileHeader.clipDataCount != 0U) {
		  auto inPointList = std::vector<fPOINT> {};
		  inPointList.resize(fileHeader.clipDataCount);
		  auto const bytesToRead =
		      fileHeader.clipDataCount * wrap::toUnsigned(sizeof(decltype(ClipPoints->back())));
		  ReadFile(fileHandle, inPointList.data(), bytesToRead, &bytesRead, nullptr);
		  if (bytesRead != bytesToRead) {
			inPointList.resize(bytesRead / sizeof(decltype(inPointList.back())));
			StateMap->set(StateFlag::BADFIL);
		  }
		  ClipPoints->reserve(ClipPoints->size() + inPointList.size());
		  ClipPoints->insert(ClipPoints->end(), inPointList.begin(), inPointList.end());
		}
		if (thredHeader.texturePointCount != 0U) {
		  auto inTextureList = std::vector<TXPNT> {};
		  inTextureList.resize(thredHeader.texturePointCount);
		  auto const bytesToRead =
		      thredHeader.texturePointCount * wrap::toUnsigned(sizeof(decltype(inTextureList.back())));
		  ReadFile(fileHandle, inTextureList.data(), bytesToRead, &bytesRead, nullptr);
		  if (bytesRead != bytesToRead) {
			inTextureList.resize(bytesRead / sizeof(decltype(inTextureList.back())));
			StateMap->set(StateFlag::BADFIL);
		  }
		  TexturePointsBuffer->reserve(TexturePointsBuffer->size() + inTextureList.size());
		  TexturePointsBuffer->insert(
		      TexturePointsBuffer->end(), inTextureList.begin(), inTextureList.end());
		}
		// update the form pointer variables
		for (auto iFormList = InsertedFormIndex; iFormList < wrap::toUnsigned(FormList->size()); ++iFormList) {
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
			wrap::narrow(formIter.fillInfo.texture.index, textureOffset);
			textureOffset += formIter.fillInfo.texture.count;
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
		  auto const insertedVertex = FormVertices->operator[](InsertedVertexIndex);
		  insertedRectangle.left    = insertedVertex.x;
		  insertedRectangle.right   = insertedVertex.x;
		  insertedRectangle.bottom  = insertedVertex.y;
		  insertedRectangle.top     = insertedVertex.y;
		  for (auto iVertex = InsertedVertexIndex + 1U; iVertex < wrap::toUnsigned(FormVertices->size());
		       ++iVertex) {
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
		auto const encodedFormIndex = (InsertedFormIndex << FRMSHFT);
		for (auto iStitch = uint16_t {}; iStitch < fileHeader.stitchCount; ++iStitch) {
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
	  constexpr auto VERSION1 = (0x01U << TBYTSHFT); // ThrEd version 1 signature
	  if ((fileHeader.headerType & VERSION1) != 0U) {
		// ToDo - Replace constants with sizes of data structures
		auto const filscor = (fileHeader.formCount) * FRMW +
		                     gethand(fileStitchBuffer, fileHeader.stitchCount) * HANDW +
		                     fileHeader.vertexLen * FRMPW + fileHeader.stitchCount * STCHW;
		if (filscor > homscor) {
		  for (auto iName = 0U; iName < DNLEN; ++iName) {
			ExtendedHeader->creatorName[iName] = thredHeader.creatorName[iName];
		  }
		  redfnam(*DesignerName);
		  SetWindowText(
		      ThrEdWindow,
		      fmt::format(StringTable->operator[](STR_THRDBY), ThrName->wstring(), *DesignerName).c_str());
		}
	  }
	  auto dest = StitchBuffer->end();
	  StitchBuffer->insert(dest, fileStitchBuffer.cbegin(), fileStitchBuffer.cend());
	  retflag = true;
	}
  }
  CloseHandle(fileHandle);
  return retflag;
}

void thred::internal::getbak() {
  if (StateMap->test(StateFlag::THUMSHO)) {
	if (ThumbnailsSelected[FileVersionIndex] != 0U) {
	  *WorkingFileName = *DefaultDirectory / Thumbnails->operator[](ThumbnailsSelected[FileVersionIndex]);
	  if (StateMap->test(StateFlag::RBUT)) {
		unthum();
		StateMap->set(StateFlag::FRMOF);
		insfil(*WorkingFileName);
		if (wrap::pressed(VK_SHIFT)) {
		  StateMap->reset(StateFlag::INSFIL);
		  StateMap->reset(StateFlag::FRMOF);
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
	  }
	  else {
		StateMap->set(StateFlag::REDOLD);
		nuFil(fileIndices::THR);
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
  nuFil(fileIndices::THR);
  fs::remove(safetyFileName);
}

void thred::internal::thumbak() {
  for (auto& iVersion : BackupViewer) {
	DestroyWindow(iVersion);
  }
  getbak();
}

void thred::internal::purg() {
  if (!ThrName->empty()) {
	auto fileName = *ThrName;
	auto ext      = ThrName->extension().wstring();
	for (auto iLast = wchar_t {}; iLast < OLDVER; ++iLast) {
	  ext.back() = iLast + 's';
	  fileName.replace_extension(ext);
	  fs::remove(fileName);
	}
  }
}

void thred::internal::purgdir() {
  StateMap->set(StateFlag::PRGMSG);
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_DELBAK);
  displayText::shoMsg(fmt::format(fmtStr, DefaultDirectory->wstring()));
  displayText::okcan();
}

void thred::internal::deldir() {
  thred::unmsg();
  displayText::tabmsg(IDS_BAKDEL);
  auto  backSpec = std::wstring {L".th0"};
  auto& backChar = backSpec.back();
  for (auto const& p : fs::directory_iterator(*DefaultDirectory)) {
	if (!fs::is_directory(p)) {
	  auto fileExt = p.path().extension().wstring();
	  backChar     = 's';
	  for (auto iLastChar = wchar_t {}; iLastChar < OLDVER; ++iLastChar) {
		if (fileExt == backSpec) {
		  fs::remove(p.path());
		  ++backChar;
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
  if (StateMap->testAndReset(StateFlag::FORMSEL)) {
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
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), wrap::next(StitchBuffer->begin(), iLowBuffer));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	  thred::savdo();
	  std::vector<fPOINTATTR> tempStitchBuffer {};
	  auto const              grpSize = GroupEndStitch + 1U - GroupStartStitch;
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
	  std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), wrap::next(StitchBuffer->begin(), grpSize));
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::mv2b() {
  if (StateMap->testAndReset(StateFlag::FORMSEL)) {
	thred::savdo();
	auto       tempStitchBuffer = std::vector<fPOINTATTR> {};
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
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), wrap::next(StitchBuffer->begin(), iLowBuffer));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	  thred::savdo();
	  thred::rngadj();
	  std::vector<fPOINTATTR> tempStitchBuffer {};
	  auto const              grpSize = GroupEndStitch + 1U - GroupStartStitch;
	  tempStitchBuffer.resize(grpSize);
	  std::copy(wrap::next(StitchBuffer->begin(), GroupStartStitch),
	            wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U),
	            tempStitchBuffer.begin());
	  std::copy(wrap::next(StitchBuffer->begin(), GroupEndStitch + 1U),
	            StitchBuffer->end(),
	            wrap::next(StitchBuffer->begin(), GroupStartStitch));
	  std::copy(tempStitchBuffer.begin(),
	            tempStitchBuffer.end(),
	            wrap::next(StitchBuffer->begin(), StitchBuffer->size() - grpSize));
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::infadj(float& xCoordinate, float& yCoordinate) noexcept {
  if (isfinite(xCoordinate)) {
	auto const unzoomedX = wrap::toFloat(UnzoomedRect.x);
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
	auto const unzoomedY = wrap::toFloat(UnzoomedRect.y);
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
  for (auto& FormVertice : *FormVertices) {
	thi::infadj(FormVertice.x, FormVertice.y);
  }
}

void thred::chkrng(fPOINT& range) {
  thred::savdo();
  thred::delinf();
  wrap::narrow(range.x, UnzoomedRect.x);
  wrap::narrow(range.y, UnzoomedRect.y);
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

void thred::internal::unmov() {
  if (StateMap->testAndReset(StateFlag::SHOMOV)) {
	thred::ritmov(ClosestFormToCursor);
  }
}

void thred::internal::duprct(FRMHED const& form) {
  auto currentVertex        = SelectedFormVertices.start;
  auto vertexIt             = wrap::next(FormVertices->cbegin(), form.vertexIndex + currentVertex);
  SelectedVerticesRect.left = SelectedVerticesRect.right = vertexIt->x;
  SelectedVerticesRect.top = SelectedVerticesRect.bottom = vertexIt->y;
  currentVertex                                          = form::pdir(form, currentVertex);
  for (auto iVertex = 0U; iVertex < SelectedFormVertices.vertexCount; ++iVertex) {
	vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex + currentVertex);
	if (vertexIt->x < SelectedVerticesRect.left) {
	  SelectedVerticesRect.left = vertexIt->x;
	}
	if (vertexIt->x > SelectedVerticesRect.right) {
	  SelectedVerticesRect.right = vertexIt->x;
	}
	if (vertexIt->y < SelectedVerticesRect.bottom) {
	  SelectedVerticesRect.bottom = vertexIt->y;
	}
	if (vertexIt->y > SelectedVerticesRect.top) {
	  SelectedVerticesRect.top = vertexIt->y;
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
  auto const vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex + SelectedFormVertices.finish);
  EndPointCross = form::sfCor2px(*vertexIt);
  StateMap->set(StateFlag::SHOPSEL);
  form::dupsel(StitchWindowDC);
  StateMap->set(StateFlag::FPSEL);
}

void thred::rotfn(float rotationAngle, fPOINT const& rotationCenter) {
  thred::savdo();
  if (StateMap->test(StateFlag::FPSEL)) {
	// clang-format off
	auto& form          = FormList->operator[](ClosestFormToCursor);
	auto  currentVertex = SelectedFormVertices.start;
	auto  const vBegin  = wrap::next(FormVertices->begin(), form.vertexIndex);
	// clang-format on
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto vertexIt = wrap::next(vBegin, currentVertex);
	  thred::rotflt(*vertexIt, rotationAngle, rotationCenter);
	  currentVertex = form::pdir(form, currentVertex);
	}
	form::frmout(ClosestFormToCursor);
	thred::setpsel();
	form::refil();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	for (auto& FormVertice : *FormVertices) {
	  thred::rotflt(FormVertice, rotationAngle, rotationCenter);
	}
	for (auto& stitch : *StitchBuffer) {
	  thi::rotstch(stitch, rotationAngle, rotationCenter);
	}
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  form::frmout(iForm);
	}
	form::selal();
	return;
  }
  if (StateMap->testAndReset(StateFlag::FRMSROT)) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  // clang-format off
	  auto& form     = FormList->operator[](selectedForm);
	  auto  vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		thred::rotflt(*vertexIt, rotationAngle, rotationCenter);
	  }
	  form::frmout(ClosestFormToCursor);
	  form::refilfn();
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->testAndReset(StateFlag::FRMROT)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);

	  auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		thred::rotflt(*vertexIt, rotationAngle, rotationCenter);
		++vertexIt;
	  }
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		thi::rotstch(StitchBuffer->operator[](iStitch), rotationAngle, rotationCenter);
	  }
	  thred::rngadj();
	  thi::selin(GroupStartStitch, GroupEndStitch, StitchWindowDC);
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::rotfns(float rotationAngle) {
  thred::savdo();
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
}

void thred::internal::nulayr(uint8_t play) {
  ActiveLayer = play;
  ladj();
  if (ActiveLayer != 0U) {
	if (StateMap->test(StateFlag::FORMSEL) &&
	    ((gsl::narrow_cast<decltype(ActiveLayer)>(FormList->operator[](ClosestFormToCursor).attribute & FRMLMSK) >>
	      1U) != ActiveLayer)) {
	  StateMap->reset(StateFlag::FORMSEL);
	}
	StateMap->reset(StateFlag::GRPSEL);
	if (StateMap->test(StateFlag::SELBOX)) {
	  if (ActiveLayer != ((StitchBuffer->operator[](ClosestPointIndex).attribute & LAYMSK) >> LAYSHFT) + 1U) {
		StateMap->reset(StateFlag::SELBOX);
	  }
	}
	SelectedFormList->clear();
  }
  StateMap->set(StateFlag::RESTCH);
}

auto thred::internal::iselpnt() noexcept -> bool {
  auto const pointToTest = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto       closestControlPoint = 0U;
  auto       minimumLength       = BIGDBL;
  auto       iControlPoint       = 0U;
  for (auto controlPoint : *FormControlPoints) {
	auto const length = hypot(pointToTest.x - controlPoint.x, pointToTest.y - controlPoint.y);
	if (length < minimumLength) {
	  minimumLength       = length;
	  closestControlPoint = iControlPoint;
	}
	++iControlPoint;
  }
  if (minimumLength < CLOSENUF) {
	SelectedFormControlVertex = closestControlPoint;
	return true;
  }
  return false;
}

void thred::strtchbox(std::vector<POINT>& stretchBoxLine) {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::Polyline(StitchWindowDC, stretchBoxLine.data(), wrap::toUnsigned(stretchBoxLine.size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unstrtch(std::vector<POINT>& stretchBoxLine) {
  if (StateMap->testAndReset(StateFlag::SHOSTRTCH)) {
	thred::strtchbox(stretchBoxLine);
  }
}

// Does the outline have zero width or zero height?
// cppcheck-suppress constParameter
auto thred::internal::isLine(std::vector<POINT>& boxOutline) noexcept -> bool {
  return (boxOutline[0].x == boxOutline[1].x) || (boxOutline[1].y == boxOutline[2].y);
}

// Is the point inside the box?
auto thred::internal::isInBox(POINT const& point, RECT const& box) noexcept -> bool {
  return (point.x >= box.left && point.x <= box.right && point.y >= box.top && point.y <= box.bottom);
}

auto thred::internal::chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  auto       minimumLength = BIGDBL;
  auto const pointToTest = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  for (auto iControlPoint = 0U; iControlPoint < SelectedFormsLine->size(); ++iControlPoint) {
	auto const length = hypot(pointToTest.x - SelectedFormsLine->operator[](iControlPoint).x,
	                          pointToTest.y - SelectedFormsLine->operator[](iControlPoint).y);
	if (length < minimumLength) {
	  minimumLength             = length;
	  SelectedFormControlVertex = iControlPoint;
	}
  }
  auto& formLines = *FormLines;
  formLines.resize(SQPNTS);
  for (auto iCorner = 0U; iCorner < 4; ++iCorner) {
	formLines[iCorner] = SelectedFormsLine->operator[](wrap::toSize(iCorner) * 2U);
  }
  formLines[4] = formLines[0];
  if (minimumLength < CLOSENUF && !isLine(formLines)) {
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
  if (isInBox(pointToTest, SelectedFormsRect) || (minimumLength < CLOSENUF && isLine(formLines))) {
	SelectedFormsSize = fPOINT {SelectedFormsRect.right - SelectedFormsRect.left,
	                            SelectedFormsRect.bottom - SelectedFormsRect.top};
	StateMap->set(StateFlag::MOVFRMS);
	FormMoveDelta =
	    fPOINT {pointToTest.x - SelectedFormsRect.left, pointToTest.y - SelectedFormsRect.top};
	StateMap->set(StateFlag::SHOSTRTCH);
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
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::setmov() {
  if (StateMap->test(StateFlag::SELBOX)) {
	MoveAnchor = ClosestPointIndex;
	StateMap->set(StateFlag::MOVSET);
	StateMap->set(StateFlag::RESTCH);
  }
}

void thred::internal::dufsel() {
  auto start = (LastFormSelected > ClosestFormToCursor) ? ClosestFormToCursor : LastFormSelected;
  auto const finish = (LastFormSelected > ClosestFormToCursor) ? LastFormSelected : ClosestFormToCursor;
  SelectedFormList->clear();
  SelectedFormList->reserve(wrap::toSize(finish) - start + 1U);
  while (start <= finish) {
	SelectedFormList->push_back(start);
	++start;
  }
}

void thred::internal::selup() {
  if (wrap::pressed(VK_SHIFT)) {
	StateMap->reset(StateFlag::SELBOX);
	if (StateMap->testAndReset(StateFlag::FORMSEL)) {
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
		  ++LastFormSelected;
		  dufsel();
		}
	  }
	  else {
		StateMap->set(StateFlag::FORMSEL);
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::SELBOX)) {
	  unbox();
	  auto const attribute = StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK;
	  while (ClosestPointIndex < wrap::toUnsigned(StitchBuffer->size() - 1U) &&
	         (StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK) == attribute) {
		++ClosestPointIndex;
	  }
	  auto stitchCoordsInPixels = POINT {};
	  stch2px(ClosestPointIndex, stitchCoordsInPixels);
	  dubox(stitchCoordsInPixels);
	}
	else {
	  if (!FormList->empty()) {
		if (StateMap->testAndSet(StateFlag::FORMSEL)) {
		  if (ClosestFormToCursor < wrap::toUnsigned(FormList->size()) - 1U) {
			++ClosestFormToCursor;
		  }
		}
		else {
		  ClosestFormToCursor = 0;
		}
		displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void thred::internal::seldwn() {
  if (wrap::pressed(VK_SHIFT)) {
	StateMap->reset(StateFlag::SELBOX);
	if (StateMap->testAndReset(StateFlag::FORMSEL)) {
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
		  --LastFormSelected;
		  dufsel();
		}
	  }
	  else {
		StateMap->set(StateFlag::FORMSEL);
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::SELBOX)) {
	  unbox();
	  auto const attribute = StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK;
	  while ((ClosestPointIndex != 0U) &&
	         (StitchBuffer->operator[](ClosestPointIndex).attribute & ATMSK) == attribute) {
		--ClosestPointIndex;
	  }
	  auto stitchCoordsInPixels = POINT {};
	  stch2px(ClosestPointIndex, stitchCoordsInPixels);
	  dubox(stitchCoordsInPixels);
	}
	else {
	  if (!FormList->empty()) {
		if (StateMap->testAndSet(StateFlag::FORMSEL)) {
		  if (ClosestFormToCursor != 0U) {
			--ClosestFormToCursor;
		  }
		}
		else {
		  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
		}
		displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

auto thred::internal::movstchs(uint32_t destination, uint32_t start, uint32_t finish) -> bool {
  auto tempStitchBuffer = std::vector<fPOINTATTR> {};
  if ((destination + 1U) < wrap::toUnsigned(StitchBuffer->size())) {
	++destination;
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
	std::copy(wrap::next(StitchBuffer->begin(), start),
	          wrap::next(StitchBuffer->begin(), finish),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(StitchBuffer->begin(), destination),
	          wrap::next(StitchBuffer->begin(), start),
	          wrap::next(tempStitchBuffer.begin(), finish - start));
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), wrap::next(StitchBuffer->begin(), destination));
  }
  else {
	tempStitchBuffer.resize(destination - start);
	std::copy(wrap::next(StitchBuffer->begin(), finish),
	          wrap::next(StitchBuffer->begin(), destination),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(StitchBuffer->begin(), start),
	          wrap::next(StitchBuffer->begin(), finish),
	          wrap::next(tempStitchBuffer.begin(), destination - finish));
	std::copy(tempStitchBuffer.begin(), tempStitchBuffer.end(), wrap::next(StitchBuffer->begin(), start));
  }
  return true;
}

void thred::internal::movmrk() {
  if (StateMap->test(StateFlag::MOVSET)) {
	if (StateMap->test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  if (movstchs(MoveAnchor, GroupStartStitch, GroupEndStitch)) {
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	else {
	  if (StateMap->test(StateFlag::SELBOX)) {
		if (movstchs(MoveAnchor, ClosestPointIndex, ClosestPointIndex + 1U)) {
		  StateMap->set(StateFlag::RESTCH);
		  thred::coltab();
		}
	  }
	}
  }
}

void thred::internal::vuthrds() {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if ((GetMenuState(ViewMenu, ID_VUTHRDS, MF_BYCOMMAND) & MF_CHECKED) != 0U) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
	StateMap->reset(StateFlag::THRDS);
  }
  else {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_CHECKED);
	StateMap->set(StateFlag::THRDS);
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::vuselthr() {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if ((GetMenuState(ViewMenu, ID_VUSELTHRDS, MF_BYCOMMAND) & MF_CHECKED) != 0U) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
	StateMap->reset(StateFlag::COL);
  }
  else {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_CHECKED);
	StateMap->set(StateFlag::COL);
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::colchk() {
  if (!StitchBuffer->empty()) {
	auto color       = StitchBuffer->front().attribute & COLMSK;
	auto startStitch = 0U;
	// ToDo - Can this loop become a ranged for?
	for (auto iStitch = 0U; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	  auto const& stitch = StitchBuffer->operator[](iStitch);
	  if (color != (stitch.attribute & COLMSK)) {
		if ((iStitch - startStitch == 1) && ((startStitch) != 0U)) {
		  auto& prevStitch     = StitchBuffer->operator[](iStitch - 1U);
		  prevStitch.attribute = (stitch.attribute & NCOLMSK) | (prevStitch.attribute & COLMSK);
		}
		color       = stitch.attribute & COLMSK;
		startStitch = iStitch;
	  }
	}
  }
}

auto thred::internal::makbig(uint32_t start, uint32_t finish) -> uint32_t {
  auto newStitches = std::vector<fPOINTATTR> {};
  newStitches.reserve(finish - start); // we know that we will have at least this number of Stitches
  auto adcnt = 0U;
  --finish;
  auto stitchIt     = wrap::next(StitchBuffer->begin(), start);
  auto nextStitchIt = stitchIt + 1U;
  for (auto iSource = start; iSource < finish; ++iSource) {
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
	  for (auto iStitch = 0U; iStitch < wrap::round<decltype(iStitch)>(stitchCount) - 1U; ++iStitch) {
		newStitches.emplace_back(fPOINTATTR {point.x, point.y, attribute});
		point.x += step.x;
		point.y += step.y;
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
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto range = RANGE {};
		form::frmrng(ClosestFormToCursor, range);
		thi::makbig(range.start, range.finish);
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
	} while (false);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_REM1);
  }
}

void thred::internal::duselrng(RANGE& selectedRange) {
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	selectedRange.start  = GroupStartStitch;
	selectedRange.finish = GroupEndStitch;
  }
  else {
	selectedRange.start  = 0U;
	selectedRange.finish = wrap::toUnsigned(StitchBuffer->size());
  }
}

void thred::internal::longer() {
  auto flag = true;
  if (ClosestPointIndex == LargestStitchIndex) {
	return;
  }
  auto const& start        = StitchBuffer->operator[](ClosestPointIndex);
  auto const& startFwd1    = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
  auto const currentLength = hypot(startFwd1.x - start.x, startFwd1.y - start.y);
  auto const rangeEnd      = ((wrap::toSize(SelectedRange.finish) + 1U) < StitchBuffer->size())
                            ? SelectedRange.finish
                            : SelectedRange.finish - 1U;
  auto iStitch = ClosestPointIndex + 1U;
  for (; iStitch < rangeEnd; ++iStitch) {
	auto const& stitch     = StitchBuffer->operator[](iStitch);
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	auto const length      = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
	if (length == currentLength) {
	  flag = false;
	  break;
	}
  }
  if (flag) {
	auto minimumLength = BIGFLOAT;
	for (auto currentStitch = SelectedRange.start; currentStitch < rangeEnd; ++currentStitch) {
	  auto const& stitch     = StitchBuffer->operator[](currentStitch);
	  auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(currentStitch) + 1U);
	  auto const length      = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
	  if (length > currentLength && length < minimumLength) {
		minimumLength = length;
		iStitch       = currentStitch;
	  }
	}
	if (minimumLength == BIGFLOAT) {
	  return;
	}
  }
  CurrentStitchIndex = iStitch;
  lensadj();
  displayText::ritnum(STR_NUMSCH, ClosestPointIndex);
}

void thred::internal::shorter() {
  auto flag = true;
  if (ClosestPointIndex == SmallestStitchIndex) {
	return;
  }
  auto const& start        = StitchBuffer->operator[](ClosestPointIndex);
  auto const& startFwd1    = StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U);
  auto const currentLength = hypot(startFwd1.x - start.x, startFwd1.y - start.y);
  auto       currentStitch = ClosestPointIndex;
  for (; currentStitch != 0; --currentStitch) {
	auto const& stitch     = StitchBuffer->operator[](currentStitch);
	auto const& stitchBck1 = StitchBuffer->operator[](wrap::toSize(currentStitch) - 1U);
	auto const length      = hypot(stitch.x - stitchBck1.x, stitch.y - stitchBck1.y);
	if (length == currentLength) {
	  --currentStitch;
	  flag = false;
	  break;
	}
  }
  if (flag) {
	auto maximumLength = 0.0F;
	auto iStitch       = SelectedRange.start;
	for (; iStitch < SelectedRange.finish - 1U; ++iStitch) {
	  auto const& stitch     = StitchBuffer->operator[](iStitch);
	  auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iStitch) + 1U);
	  auto const length      = hypot(stitchFwd1.x - stitch.x, stitchFwd1.y - stitch.y);
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
	auto const& stitch = StitchBuffer->operator[](iStitch);
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
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	thred::rngadj();
	ClosestPointIndex = GroupStartStitch;
	StateMap->set(StateFlag::SELBOX);
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto flag    = true;
	  auto iStitch = 0U;
	  for (auto& stitch : *StitchBuffer) {
		if (((stitch.attribute & NOTFRM) == 0U) && ((stitch.attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
		  ClosestPointIndex = iStitch;
		  StateMap->set(StateFlag::SELBOX);
		  StateMap->set(StateFlag::RESTCH);
		  flag = false;
		  break;
		}
		++iStitch;
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
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	thred::rngadj();
	ClosestPointIndex = GroupEndStitch;
	StateMap->set(StateFlag::SELBOX);
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto flag = true;
	  for (auto iStitch = wrap::toUnsigned(StitchBuffer->size()); iStitch != 0; --iStitch) {
		if (((StitchBuffer->operator[](iStitch - 1U).attribute & NOTFRM) == 0U) &&
		    ((StitchBuffer->operator[](iStitch - 1U).attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
		  ClosestPointIndex = iStitch - 1U;
		  StateMap->set(StateFlag::SELBOX);
		  StateMap->set(StateFlag::RESTCH);
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
	auto const thumbPath = fs::path(Thumbnails->operator[](ThumbnailsSelected[iThumbnail]).data());
	constexpr auto TNAMELEN = 12U; // how many characters to display with each thumbnail

	auto const name = thumbPath.stem().wstring().substr(0U, TNAMELEN);
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
	  outDebugString(L"default hit in rthumbnam: iThumbnail [{}]\n", iThumbnail);
	  break;
	}
  }
}

void thred::internal::thumnail() {
  auto fileData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
  thred::unbsho();
  thred::undat();
  trace::untrace();
  fs::current_path(*DefaultDirectory);
  auto const searchName = *DefaultDirectory / L"*.thr";
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const file = FindFirstFile(searchName.wstring().c_str(), &fileData);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (file == INVALID_HANDLE_VALUE) {
	auto const dwError = GetLastError();
	auto       fmtStr  = std::wstring {};
	displayText::loadString(fmtStr, IDS_FFINDERR);
	displayText::shoMsg(fmt::format(fmtStr, searchName.wstring(), dwError));
	unthum();
  }
  else {
	Thumbnails->clear();
	auto& fileName = fileData.cFileName;
	Thumbnails->push_back(std::wstring(std::begin(fileName)));
	while (FindNextFile(file, &fileData)) {
	  Thumbnails->push_back(std::wstring(std::begin(fileName)));
	}
	FindClose(file);
	std::sort(Thumbnails->begin(), Thumbnails->end());
	auto       iThumbnail = 0U;
	auto const thumbSize  = Thumbnails->size();
	while (iThumbnail < 4 && iThumbnail < thumbSize) {
	  ThumbnailsSelected[iThumbnail] = iThumbnail;
	  ++iThumbnail;
	}
	ThumbnailIndex = ThumbnailDisplayCount = iThumbnail;
	while (iThumbnail < 4 && iThumbnail < thumbSize) {
	  rthumnam(iThumbnail++);
	}
	StateMap->set(StateFlag::THUMSHO);
	ThumbnailSearchString.front() = 0;
	SetWindowText(ButtonWin->operator[](HBOXSEL), L"");
	auto const blank = std::wstring {};
	displayText::butxt(HBOXSEL, blank);
	vubak();
	StateMap->set(StateFlag::RESTCH);
  }
}

void thred::internal::nuthsel() {
  if (ThumbnailIndex < Thumbnails->size()) {
	auto const savedIndex = ThumbnailIndex;
	auto       iThumbnail = 0U;
	auto const length     = wcslen(ThumbnailSearchString.data());
	StateMap->set(StateFlag::RESTCH);
	if (length != 0U) {
	  while (iThumbnail < 4 && ThumbnailIndex < Thumbnails->size()) {
		if (wcsncmp(ThumbnailSearchString.data(), Thumbnails->operator[](ThumbnailIndex).data(), length) == 0) {
		  ThumbnailsSelected[iThumbnail] = ThumbnailIndex;
		  thred::redraw(BackupViewer[iThumbnail++]);
		}
		++ThumbnailIndex;
	  }
	}
	else {
	  while (iThumbnail < 4 && ThumbnailIndex < Thumbnails->size()) {
		ThumbnailsSelected[iThumbnail] = ThumbnailIndex;
		thred::redraw(BackupViewer[iThumbnail++]);
		++ThumbnailIndex;
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
	auto const length = wcslen(ThumbnailSearchString.data());
	if (length != 0U) {
	  while ((count != 0U) && ThumbnailIndex < MAXFORMS) {
		if (ThumbnailIndex != 0U) {
		  --ThumbnailIndex;
		  if (wcsncmp(ThumbnailSearchString.data(), Thumbnails->operator[](ThumbnailIndex).data(), length) == 0) {
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
}

void thred::internal::nuthum(wchar_t character) {
  auto length = wcslen(ThumbnailSearchString.data());
  if (length < 16U) {
	StateMap->set(StateFlag::RESTCH);
	ThumbnailSearchString[length++] = character;
	ThumbnailSearchString[length]   = 0;

	auto const txt = std::wstring(ThumbnailSearchString.data());
	displayText::butxt(HBOXSEL, txt);
	ThumbnailIndex = 0;
	nuthsel();
  }
}

void thred::internal::bakthum() {
  auto searchStringLength = wcslen(ThumbnailSearchString.data());
  if (searchStringLength != 0U) {
	StateMap->set(StateFlag::RESTCH);
	ThumbnailSearchString[--searchStringLength] = 0;
	ThumbnailIndex                              = 0;
	auto const txt                              = std::wstring(ThumbnailSearchString.data());
	displayText::butxt(HBOXSEL, txt);
	nuthsel();
  }
}

void thred::internal::selalstch() {
  if (!StitchBuffer->empty()) {
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
}

void thred::internal::duinsfil() {
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  auto const offset      = fPOINT {stitchPoint.x - InsertCenter.x, stitchPoint.y - InsertCenter.y};
  for (auto iForm = InsertedFormIndex; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& formRectangle = FormList->operator[](iForm).rectangle;
	formRectangle.bottom += offset.y;
	formRectangle.top += offset.y;
	formRectangle.left += offset.x;
	formRectangle.right += offset.x;
  }
  for (auto iVertex = InsertedVertexIndex; iVertex < wrap::toUnsigned(FormVertices->size()); ++iVertex) {
	FormVertices->operator[](iVertex).x += offset.x;
	FormVertices->operator[](iVertex).y += offset.y;
  }
  for (auto iStitch = InsertedStitchIndex; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	StitchBuffer->operator[](iStitch).x += offset.x;
	StitchBuffer->operator[](iStitch).y += offset.y;
  }
  StateMap->reset(StateFlag::FRMOF);
  StateMap->set(StateFlag::INIT);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::gotbox() {
  StateMap->reset(StateFlag::BZUMIN);
  StateMap->reset(StateFlag::BOXSLCT);
  StateMap->reset(StateFlag::FRMPSEL);
  thred::grpAdj();
}

void thred::internal::rngal() {
  if (!StateMap->testAndReset(StateFlag::WASFPNT)) {
	StateMap->reset(StateFlag::GRPSEL);
	auto prng = std::vector<RANGE> {};
	StateMap->reset(StateFlag::GRPSEL);
	auto iStitch     = 0U;
	auto flagInRange = false;
	for (; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
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
	  for (auto index = 0U; index < prng.size(); ++index) {
		auto const length = prng[index].finish - prng[index].start;
		if (length > maximumLength) {
		  maximumLength = length;
		  largestRange  = index;
		}
	  }
	  if (maximumLength != 0U) {
		ClosestPointIndex = prng[largestRange].start;
		GroupStitchIndex  = prng[largestRange].finish;
		StateMap->set(StateFlag::GRPSEL);
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
	  wrap::narrow(form.fillColor, ActiveColor);
	  if (form.fillType == FTHF) {
		wrap::narrow(form.fillInfo.feather.color, ActiveColor);
	  }
	}
	if (form.edgeType != 0U) {
	  wrap::narrow(form.borderColor, ActiveColor);
	}
	if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
	  wrap::narrow(form.underlayColor, ActiveColor);
	}
  }
  for (auto& stitch : *StitchBuffer) {
	if (formMap.test((stitch.attribute & FRMSK) >> FRMSHFT) && (stitch.attribute & TYPMSK) != TYPMSK) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= ActiveColor;
	}
  }
}

auto thred::internal::dunum(wchar_t code) noexcept -> bool {
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

void thred::stchrct(fRECTANGLE& rectangle) noexcept {
  if (!StitchBuffer->empty()) {
	rectangle.bottom = rectangle.left = BIGFLOAT;
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
  constexpr auto MMTOINCH = 1 / 25.4F; // conversion factor for millimeters to inches

  auto  rectangle   = fRECTANGLE {};
  auto  info        = std::wstring {};
  auto& stringTable = *StringTable;
  if (!StitchBuffer->empty()) {
	thred::stchrct(rectangle);
	auto const xSize = (rectangle.right - rectangle.left) * IPFGRAN;
	auto const ySize = (rectangle.top - rectangle.bottom) * IPFGRAN;
	if ((rectangle.left < 0) || (rectangle.bottom < 0) || (rectangle.right > IniFile.hoopSizeX) ||
	    (rectangle.top > IniFile.hoopSizeY)) {
	  info += stringTable[STR_STCHOUT];
	}
	info += fmt::format(
	    stringTable[STR_STCHS], wrap::toUnsigned(StitchBuffer->size()), xSize, (xSize * MMTOINCH), ySize, (ySize * MMTOINCH));
  }
  if (!FormList->empty()) {
	thred::frmrct(rectangle);
	auto const xSize = (rectangle.right - rectangle.left) * IPFGRAN;
	auto const ySize = (rectangle.top - rectangle.bottom) * IPFGRAN;
	info += fmt::format(
	    stringTable[STR_FORMS], FormList->size(), xSize, (xSize * MMTOINCH), ySize, (ySize * MMTOINCH));
  }
  info += fmt::format(stringTable[STR_HUPWID], (IniFile.hoopSizeX * IPFGRAN), (IniFile.hoopSizeY * IPFGRAN));
  if (!StitchBuffer->empty()) {
	auto& modifierName = ExtendedHeader->modifierName;
	auto  modifier     = utf::Utf8ToUtf16(std::string(std::begin(modifierName)));
	info += fmt::format(stringTable[STR_CREATBY], *DesignerName, modifier);
  }
  displayText::shoMsg(info);
}

void thred::internal::sidhup() {
  auto hoopRectangle        = RECT {0L, 0L, 0L, 0L};
  auto preferencesRectangle = RECT {0L, 0L, 0L, 0L};
  StateMap->set(StateFlag::HUPMSG);
  GetWindowRect(ValueWindow->operator[](PHUP), &hoopRectangle);
  GetWindowRect(PreferencesWindow, &preferencesRectangle);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  for (auto iHoop = size_t {}; iHoop < HUPS; ++iHoop) {
	auto const idx = gsl::narrow_cast<int32_t>(iHoop);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	SideWindow[iHoop] = CreateWindow(L"STATIC",
	                                 StringTable->operator[](iHoop + STR_HUP0).c_str(),
	                                 SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
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
	  nuFil(fileIndices::THR);
	  nulayr(0U);
	}
	else {
	  displayText::savdisc();
	  StateMap->set(StateFlag::OSAV);
	}
  }
  else {
	nuFil(fileIndices::THR);
	nulayr(0U);
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
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	auto iStitch          = GroupStartStitch;
	auto ClipRectAdjusted = fRECTANGLE {StitchBuffer->operator[](iStitch).x,
	                                    StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y,
	                                    StitchBuffer->operator[](iStitch).x,
	                                    StitchBuffer->operator[](wrap::toSize(iStitch) + 1U).y};
	++iStitch;
	while (iStitch < GroupEndStitch) {
	  clpradj(ClipRectAdjusted, StitchBuffer->operator[](iStitch++));
	}
	if (StitchBuffer->operator[](iStitch).x < ClipRectAdjusted.left) {
	  ClipRectAdjusted.left = StitchBuffer->operator[](iStitch).x;
	}
	if (StitchBuffer->operator[](iStitch).x > ClipRectAdjusted.right) {
	  ClipRectAdjusted.right = StitchBuffer->operator[](iStitch).x;
	}
	auto const clipMiddle = wrap::midl(ClipRectAdjusted.right, ClipRectAdjusted.left);
	StitchBuffer->operator[](GroupStartStitch).y = wrap::midl(ClipRectAdjusted.top, ClipRectAdjusted.bottom);
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
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_GRPMSG, IDS_RNGEND);
  }
}

void thred::internal::shftflt(fPOINT const& point) noexcept {
  if (point.x <= ZoomRect.left || point.x >= ZoomRect.right || point.y <= ZoomRect.bottom ||
      point.y >= ZoomRect.top) {
	thred::shft(point);
  }
}

void thred::internal::fil2men() {
  if (UserFlagMap->test(UserFlag::FIL2OF)) {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_UNCHECKED);
  }
}

void thred::internal::defpref() {
  UserFlagMap->reset();
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

  bitmap::setBmpBackColor();
  formForms::dazdef();
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
  IniFile.gridSize        = 2.0F * PFGRAN; // 2mm default spacing
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
  bitmap::setBitmapColor(0xc8dfee);
  if (IniFile.customHoopX == 0.0F) {
	IniFile.customHoopX = LHUPX;
  }
  if (IniFile.customHoopY == 0.0F) {
	IniFile.customHoopY = LHUPY;
  }
  PicotSpacing = IPICSPAC;
  UserFlagMap->set(UserFlag::FIL2OF);
  fil2men();
  BackgroundColor              = 0xa8c4b1;
  UnzoomedRect.x               = std::lround(IniFile.hoopSizeX);
  UnzoomedRect.y               = std::lround(IniFile.hoopSizeY);
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
  if (StateMap->testAndReset(StateFlag::GMRK)) {
	drwmrk(StitchWindowDC);
  }
  ZoomMarkPoint.x = xCoord;
  ZoomMarkPoint.y = yCoord;
  StateMap->set(StateFlag::INIT);
  StateMap->set(StateFlag::GMRK);
  drwmrk(StitchWindowDC);
  StateMap->set(StateFlag::WASMRK);
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

auto thred::internal::nuang(float originalAngle, float xDelta, float yDelta) noexcept -> float {
  auto const angle         = atan2(yDelta, xDelta);
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

void thred::internal::angdif(float& lowestAngle, float& highestAngle, float angle) noexcept {
  if (angle > highestAngle) {
	highestAngle = angle;
  }
  else {
	if (angle < lowestAngle) {
	  lowestAngle = angle;
	}
  }
}

void thred::internal::rotmrk() {
  if (StateMap->test(StateFlag::GMRK) &&
      (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL))) {
	auto lowestAngle  = 0.0F;
	auto highestAngle = 0.0F;
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto const codedFormIndex = ClosestFormToCursor << FRMSHFT;
	  // clang-format off
	  auto& form     = FormList->operator[](ClosestFormToCursor);
	  auto  vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	  // clang-format on
	  auto const originalAngle = atan2(vertexIt->y - ZoomMarkPoint.y, vertexIt->x - ZoomMarkPoint.x);
	  ++vertexIt;
	  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
		angdif(lowestAngle,
		       highestAngle,
		       nuang(originalAngle, vertexIt->x - ZoomMarkPoint.x, vertexIt->y - ZoomMarkPoint.y));
		++vertexIt;
	  }
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) == codedFormIndex) {
		  angdif(lowestAngle,
		         highestAngle,
		         nuang(originalAngle, stitch.x - ZoomMarkPoint.x, stitch.y - ZoomMarkPoint.y));
		}
	  }
	}
	else {
	  thred::rngadj();
	  auto const originalAngle = atan2(StitchBuffer->operator[](GroupStartStitch).y - ZoomMarkPoint.y,
	                                   StitchBuffer->operator[](GroupStartStitch).x - ZoomMarkPoint.x);
	  for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
		angdif(lowestAngle,
		       highestAngle,
		       nuang(originalAngle,
		             StitchBuffer->operator[](iStitch).x - ZoomMarkPoint.x,
		             StitchBuffer->operator[](iStitch).y - ZoomMarkPoint.y));
	  }
	}
	auto const tAngle     = highestAngle - lowestAngle;
	auto const segments   = std::round(PI_F2 / tAngle);
	IniFile.rotationAngle = PI_F2 / segments;
	auto fmtStr           = std::wstring {};
	displayText::loadString(fmtStr, IDS_ROTMARK);
	// ToDo - should this be IniFile.rotationAngle?
	displayText::shoMsg(fmt::format(fmtStr, IniFile.fillAngle * RADDEGF, segments));
  }
  else {
	displayText::shoseln(IDS_FSZ, IDS_SETROTM);
  }
}

void thred::internal::segentr(float rotationAngle) {
  if (rotationAngle == 0.0F) {
	rotationAngle = 1 * DEGRADF;
  }
  displayText::shoMsg(fmt::format(StringTable->operator[](STR_ENTROT), (PI_F2 / rotationAngle)));
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void thred::internal::rotseg() {
  segentr(IniFile.rotationAngle);
  StateMap->set(StateFlag::ENTRSEG);
}

void thred::internal::pntmrk() {
  do {
	if (StateMap->test(StateFlag::SELBOX)) {
	  dumrk(StitchBuffer->operator[](ClosestPointIndex).x,
	        StitchBuffer->operator[](ClosestPointIndex).y);
	  break;
	}
	if (StateMap->test(StateFlag::FRMPSEL)) {
	  auto const vertexIt =
	      wrap::next(FormVertices->cbegin(),
	                 FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	  dumrk(vertexIt->x, vertexIt->y);
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
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  form::refil();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void thred::internal::nuslst(uint32_t find) {
  gselrng();
  // ToDo - Check this code. Does it do what is intended?
  do {
	if (find < SelectedFormsRange.start) {
	  for (auto form = find; form < SelectedFormsRange.finish; ++form) {
		SelectedFormList->push_back(form);
	  }
	  break;
	}
	if (find > SelectedFormsRange.finish) {
	  for (auto form = SelectedFormsRange.start; form <= find; ++form) {
		SelectedFormList->push_back(form);
	  }
	  break;
	}
	for (auto form = SelectedFormsRange.start; form <= find; ++form) {
	  SelectedFormList->push_back(form);
	}
  } while (false);
}

void thred::internal::srchk() {
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

constexpr auto thred::internal::byteSwap(uint32_t data) noexcept -> uint32_t {
  auto const a = ((data & 0x000000FFU) << 24U) | ((data & 0x0000FF00U) << 8U) |
                 ((data & 0x00FF0000U) >> 8U) | ((data & 0xFF000000U) >> 24U);
  return a;
}

void thred::internal::ritcur() {
  constexpr auto ICONROWS = 32; // rows in the icon
  constexpr auto BPINT    = 32; // bits in an uint32_t
  constexpr auto ICOLMASK = 0xffffffU;
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const currentCursor = GetCursor();
  if (currentCursor != nullptr) {
	auto iconInfo = ICONINFO {FALSE, 0U, 0U, nullptr, nullptr};
	GetIconInfo(currentCursor, &iconInfo);
	auto cursorPosition = POINT {0L, 0L};
	GetCursorPos(&cursorPosition);
	cursorPosition.x -= (StitchWindowOrigin.x + gsl::narrow_cast<LONG>(iconInfo.xHotspot));
	cursorPosition.y -= (StitchWindowOrigin.y + gsl::narrow_cast<LONG>(iconInfo.yHotspot));
	// ToDo - replace with GetDIBits
	constexpr auto ICONSIZE = 64U; // size in bytes of an icon bitmap

	auto bitmapBits = std::array<uint8_t, ICONSIZE> {};
	GetBitmapBits(iconInfo.hbmMask, gsl::narrow<LONG>(bitmapBits.size()), bitmapBits.data());
	if (currentCursor == ArrowCursor) {
	  for (auto iRow = 0; iRow < ICONROWS; ++iRow) {
		auto const mask          = byteSwap(bitmapBits[wrap::toSize(iRow)]);
		auto const bitmapInverse = byteSwap(bitmapBits[wrap::toSize(iRow) + 32]);
		auto       bitMask       = uint32_t {1U} << HBSHFT;
		for (auto iPixel = 0; iPixel < BPINT; ++iPixel) {
		  if ((bitMask & mask) == 0U) {
			auto const pixelColor = ((bitMask & bitmapInverse) != 0U) ? PENWHITE : PENBLK;
			SetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow, pixelColor);
		  }
		  bitMask >>= 1U;
		}
	  }
	}
	else {
	  for (auto iRow = 0; iRow < ICONROWS; ++iRow) {
		auto const bitmapInverse = byteSwap(bitmapBits[wrap::toSize(iRow) + 32]);
		auto       bitMask       = uint32_t {1U} << HBSHFT;
		for (auto iPixel = 0; iPixel < BPINT; ++iPixel) {
		  if ((bitMask & bitmapInverse) != 0U) {
			SetPixel(StitchWindowDC,
			         cursorPosition.x + iPixel,
			         cursorPosition.y + iRow,
			         GetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow) ^ ICOLMASK);
		  }
		  bitMask >>= 1U;
		}
	  }
	}
  }
}

void thred::internal::nedon() {
  UserFlagMap->reset(UserFlag::NEDOF);
  nedmen();
}

void thred::internal::nedof() {
  UserFlagMap->set(UserFlag::NEDOF);
  nedmen();
}

void thred::internal::shoknot() {
  UserFlagMap->reset(UserFlag::KNOTOF);
  knotmen();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::hidknot() {
  UserFlagMap->set(UserFlag::KNOTOF);
  knotmen();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::pcsbsavon() {
  UserFlagMap->reset(UserFlag::BSAVOF);
  bsavmen();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::pcsbsavof() {
  UserFlagMap->set(UserFlag::BSAVOF);
  bsavmen();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::tglhid() {
  if (!StateMap->testAndFlip(StateFlag::HIDSTCH)) {
	StateMap->reset(StateFlag::FRMOF);
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::respac(FRMHED& form) noexcept {
  if (clip::isclp(form)) {
	form.fillSpacing = LineSpacing;
	form::fsizpar(form);
  }
}

auto thred::internal::chkminus(wchar_t code) noexcept -> bool {
  if (code == VK_OEM_MINUS || code == VK_SUBTRACT) { // '-' key pressed
	if (PreferenceIndex == PFAZ) {                   // Clipboard Offset in preferences
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
  if (StateMap->test(StateFlag::GRPSEL)) {
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
  else {
	displayText::shoseln(IDS_GRPMSG, IDS_RETRAC);
  }
}

void thred::internal::setgrd(COLORREF color) {
  static constexpr auto gcHigh    = GRDCOD {ID_GRDHI, HIGRD};
  static constexpr auto gcMedium  = GRDCOD {ID_GRDMED, MEDGRD};
  static constexpr auto gcDefault = GRDCOD {ID_GRDEF, DEFGRD};
  static constexpr auto gcRed     = GRDCOD {ID_GRDRED, REDGRD};
  static constexpr auto gcBlue    = GRDCOD {ID_GRDBLU, BLUGRD};
  static constexpr auto gcGreen   = GRDCOD {ID_GRDGRN, GRNGRD};
  static constexpr auto gridCodes = std::array<GRDCOD, 6> {gcHigh, gcMedium, gcDefault, gcRed, gcBlue, gcGreen};

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
  StateMap->set(StateFlag::RESTCH);
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::ovrlay() {
  auto fileName = fs::path {};
  insfil(fileName);
  StateMap->reset(StateFlag::INSFIL);
  StateMap->reset(StateFlag::FRMOF);
  StateMap->set(StateFlag::INIT);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::fil2sel(uint32_t stat) {
  UserFlagMap->set(UserFlag::FIL2OF);
  if (stat != 0U) {
	UserFlagMap->reset(UserFlag::FIL2OF);
  }
  fil2men();
}

void thred::internal::rotauxmen() {
  if (UserFlagMap->test(UserFlag::ROTAUX)) {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_CHECKED);
  }
}

void thred::internal::rotauxsel(uint32_t stat) {
  UserFlagMap->set(UserFlag::ROTAUX);
  if (stat == 0U) {
	UserFlagMap->reset(UserFlag::ROTAUX);
  }
  rotauxmen();
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::frmcurmen() {
  if (UserFlagMap->test(UserFlag::FRMX)) {
	CheckMenuItem(MainMenu, ID_FRMX, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FRMX, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_CHECKED);
  }
}

void thred::internal::frmcursel(uint32_t cursorType) {
  UserFlagMap->set(UserFlag::FRMX);
  if (cursorType == 0U) {
	UserFlagMap->reset(UserFlag::FRMX);
  }
  frmcurmen();
  StateMap->set(StateFlag::DUMEN);
}

void thred::internal::stchsnap(uint32_t start, uint32_t finish) {
  auto pnt = wrap::next(StitchBuffer->begin(), start);
  for (auto i = 0U; i < finish - start; ++i) {
	pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
	pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;
	++pnt;
  }
}

void thred::internal::frmsnap(uint32_t start, uint32_t count) {
  auto vertexIt = wrap::next(FormVertices->begin(), start);
  for (auto i = 0U; i < count; ++i) {
	vertexIt->x = rintf(vertexIt->x / IniFile.gridSize) * IniFile.gridSize;
	vertexIt->y = rintf(vertexIt->y / IniFile.gridSize) * IniFile.gridSize;
	++vertexIt;
  }
}

void thred::internal::gsnap() {
  if (StateMap->test(StateFlag::TXTRED)) {
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
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto& formIter = FormList->operator[](ClosestFormToCursor);
	  frmsnap(formIter.vertexIndex, formIter.vertexCount);
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		stchsnap(GroupStartStitch, GroupEndStitch + 1U);
		StateMap->set(StateFlag::RESTCH);
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
	for (auto iFile = 0U; iFile < fileIndex; ++iFile) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if ((fileData[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0U) {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
		SendMessage(GetDlgItem(hwndlg, IDC_LOCKED),
		            LB_ADDSTRING,
		            0,
		            reinterpret_cast<LPARAM>(fileData[iFile].cFileName)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	  }
	  else {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
		SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED),
		            LB_ADDSTRING,
		            0,
		            reinterpret_cast<LPARAM>(fileData[iFile].cFileName)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	  }
	}
  }
}

auto CALLBACK thred::internal::LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  SetWindowLongPtr(hwndlg, DWLP_USER, lparam);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	  auto* fileInfo = reinterpret_cast<FINDINFO*>(lparam);
	  if (fileInfo != nullptr) {
		auto const searchName = *DefaultDirectory / L"*.thr";
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto const searchResult = FindFirstFile(searchName.wstring().c_str(), &(fileInfo->data[0]));
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		if (searchResult == INVALID_HANDLE_VALUE) {
		  auto fmtStr = std::wstring {};
		  displayText::loadString(fmtStr, IDS_NOTHRFIL);
		  displayText::shoMsg(fmt::format(fmtStr, DefaultDirectory->wstring()));
		  EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
		  return TRUE;
		}
		fileInfo->count = 1;
		while (FindNextFile(searchResult, &fileInfo->data[fileInfo->count++])) { }
		--(fileInfo->count);
		ritlock(fileInfo->data, fileInfo->count, hwndlg);
	  }
	  break;
	}
	case WM_COMMAND: {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	  auto* fileInfo = reinterpret_cast<FINDINFO*>(GetWindowLongPtr(hwndlg, DWLP_USER));
	  if (fileInfo != nullptr) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		constexpr auto NROMASK = std::numeric_limits<DWORD>::max() ^
		                         FILE_ATTRIBUTE_READONLY; // invert FILE_ATTRIBUTE_READONLY
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
		switch (LOWORD(wparam)) {
		  case IDCANCEL: {
			EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
			return TRUE;
		  }
		  case IDC_LOCKAL: {
			for (auto iFile = 0U; iFile < fileInfo->count; ++iFile) {
			  // NOLINTNEXTLINE(hicpp-signed-bitwise)
			  fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDC_UNLOCKAL: {
			for (auto iFile = 0U; iFile < fileInfo->count; ++iFile) {
			  // NOLINTNEXTLINE(hicpp-signed-bitwise)
			  fileInfo->data[iFile].dwFileAttributes &= NROMASK;
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDC_LOCK: {
			auto fileError = 0U;
			// NOLINTNEXTLINE(readability-qualified-auto)
			auto const unlockHandle = GetDlgItem(hwndlg, IDC_UNLOCKED);
			for (auto iFile = 0U; iFile < fileInfo->count; ++iFile) {
			  // NOLINTNEXTLINE(hicpp-signed-bitwise)
			  if ((fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0U) {
				if (SendMessage(unlockHandle, LB_GETSEL, fileError, 0)) {
				  // NOLINTNEXTLINE(hicpp-signed-bitwise)
				  fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
				}
				++fileError;
			  }
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDC_UNLOCK: {
			auto fileError = 0U;
			// NOLINTNEXTLINE(readability-qualified-auto)
			auto const lockHandle = GetDlgItem(hwndlg, IDC_LOCKED);
			for (auto iFile = 0U; iFile < fileInfo->count; ++iFile) {
			  // NOLINTNEXTLINE(hicpp-signed-bitwise)
			  if ((fileInfo->data[iFile].dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0U) {
				if (SendMessage(lockHandle, LB_GETSEL, fileError, 0)) {
				  // NOLINTNEXTLINE(hicpp-signed-bitwise)
				  fileInfo->data[iFile].dwFileAttributes &= NROMASK;
				}
				++fileError;
			  }
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;
		  }
		  case IDOK: {
			auto fileError = 0U;
			for (auto iFile = 0U; iFile < fileInfo->count; ++iFile) {
			  auto& cFileName = fileInfo->data[iFile].cFileName;
			  auto  fileName  = *DefaultDirectory / std::begin(cFileName);
			  if (!SetFileAttributes(fileName.wstring().c_str(), fileInfo->data[iFile].dwFileAttributes)) {
				++fileError;
			  }
			}
			if (fileError != 0U) {
			  auto fmtStr = std::wstring {};
			  displayText::loadString(fmtStr, IDS_LOCKNOT);
			  displayText::shoMsg(fmt::format(fmtStr, fileError));
			}
			EndDialog(hwndlg, gsl::narrow_cast<INT_PTR>(wparam));
			return TRUE;
		  }
		  default: {
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			outDebugString(L"default hit in LockPrc 1: wparam [{}]\n", LOWORD(wparam));
			break;
		  }
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"default hit in LockPrc 2: umsg [{}]\n", umsg);
	  break;
	}
  }
  return FALSE;
}

void thred::internal::lock() {
  // ToDo - investigate C++17 option shown here: http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
  auto lockInfo = FINDINFO {};
  // ToDo - Replace 512 with maximum files in subdirectory
  lockInfo.data = new WIN32_FIND_DATA[512]; // NOLINT(cppcoreguidelines-owning-memory)
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-type-cstyle-cast)
  DialogBoxParam(ThrEdInstance, MAKEINTRESOURCE(IDD_DLOCK), ThrEdWindow, LockPrc, reinterpret_cast<LPARAM>(&lockInfo));
  delete[] lockInfo.data; // NOLINT(cppcoreguidelines-owning-memory)
}

void thred::internal::delstch() {
  thred::savdo();
  StitchBuffer->clear();
  StitchBuffer->shrink_to_fit();
  TexturePointsBuffer->clear();
  TexturePointsBuffer->shrink_to_fit();
  rstAll();
  form::clrfills();
  thred::resetColorChanges();
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
  displayText::butxt(HTOT, blank);
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::closfn() {
  deltot();
  KnotCount = 0;
  WorkingFileName->clear();
  bitmap::delmap();
  deldu();
  displayText::clrhbut(3);
  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
}

void thred::internal::filclos() {
  if (savcmp()) {
	closfn();
  }
  else {
	StateMap->set(StateFlag::FCLOS);
	displayText::savdisc();
  }
}

void thred::internal::frmpos(FRMHED& form, float deltaX, float deltaY) {
  auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	vertexIt->x += deltaX;
	vertexIt->y += deltaY;
	++vertexIt;
  }
  form.rectangle.bottom += deltaY;
  form.rectangle.top += deltaY;
  form.rectangle.left += deltaX;
  form.rectangle.right += deltaX;
}

void thred::internal::nudgfn(float deltaX, float deltaY) {
  if (StateMap->test(StateFlag::BIGBOX) || !SelectedFormList->empty() || StateMap->test(StateFlag::FORMSEL) ||
      StateMap->test(StateFlag::GRPSEL) || StateMap->test(StateFlag::SELBOX)) {
	thred::savdo();
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	for (auto& formIt : *FormList) {
	  frmpos(formIt, deltaX, deltaY);
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.x += deltaX;
	  stitch.y += deltaY;
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
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::FORMSEL)) {
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
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  StitchBuffer->operator[](iStitch).x += deltaX;
	  StitchBuffer->operator[](iStitch).y += deltaY;
	}
	thred::grpAdj();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::SELBOX)) {
	StitchBuffer->operator[](ClosestPointIndex).x += deltaX;
	StitchBuffer->operator[](ClosestPointIndex).y += deltaY;
	StateMap->set(StateFlag::RESTCH);
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
  auto input       = INPUT {};
  input.type       = INPUT_MOUSE;
  input.mi.dwFlags = MOUSEEVENTF_MOVE;
  input.mi.dx      = pixel.x;
  input.mi.dy      = pixel.y;
  SendInput(1, &input, sizeof(INPUT));
}

void thred::internal::pixmsg(uint32_t iString, uint32_t pixelCount) {
  displayText::shoMsg(fmt::format(StringTable->operator[](iString), pixelCount));
}

void thred::internal::getnpix() {
  pixmsg(STR_NUDG, IniFile.nudgePixels);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::PIXIN);
  displayText::numWnd();
}

void thred::internal::getstpix() {
  pixmsg(STR_STCHP, IniFile.stitchSizePixels);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::STPXIN);
  displayText::numWnd();
}

void thred::internal::getfrmpix() {
  pixmsg(STR_FRMP, IniFile.formVertexSizePixels);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::FRMPXIN);
  displayText::numWnd();
}

void thred::internal::getfrmbox() {
  pixmsg(STR_FRMBOX, IniFile.formBoxSizePixels);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::FRMBOXIN);
  displayText::numWnd();
}

void thred::internal::bakmrk() {
  if (StateMap->test(StateFlag::WASMRK)) {
	if (ZoomMarkPoint.x > IniFile.hoopSizeX) {
	  ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
	}
	if (ZoomMarkPoint.y > IniFile.hoopSizeY) {
	  ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
	}
	dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
	fPOINT const point(ZoomMarkPoint);
	thred::shft(point);
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_MRK);
  }
}

void thred::internal::nuscol(size_t iColor) noexcept {
  UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
  UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
  thred::redraw(UserColorWin->operator[](iColor));
}

void thred::internal::movchk() {
  static auto draggedColor    = uint8_t {};
  auto&       defaultColorWin = *DefaultColorWin;
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if ((Msg.wParam & MK_LBUTTON) != 0U) {
	if (!StateMap->testAndSet(StateFlag::WASMOV)) {
	  if (thi::chkMsgs(Msg.pt, defaultColorWin.front(), defaultColorWin.back())) {
		draggedColor = VerticalIndex & COLORMAX;
		StateMap->set(StateFlag::WASCOL);
	  }
	}
  }
  else {
	if (StateMap->testAndReset(StateFlag::WASMOV) && StateMap->testAndReset(StateFlag::WASCOL)) {
	  if (thi::chkMsgs(Msg.pt, defaultColorWin.front(), defaultColorWin.back()) && draggedColor != VerticalIndex) {
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
		for (auto& formIter : *FormList) {
		  if (formIter.fillType != 0U) {
			if (formIter.fillColor == VerticalIndex) {
			  formIter.fillColor = draggedColor;
			}
			else {
			  if (!key && formIter.fillColor == draggedColor) {
				formIter.fillColor = VerticalIndex;
			  }
			}
			if (formIter.fillType == FTHF) {
			  if (formIter.fillInfo.feather.color == VerticalIndex) {
				formIter.fillInfo.feather.color = draggedColor;
			  }
			  else {
				if (!key && formIter.fillInfo.feather.color == draggedColor) {
				  formIter.fillInfo.feather.color = VerticalIndex;
				}
			  }
			}
		  }
		  if (formIter.edgeType != 0U) {
			if (formIter.borderColor == VerticalIndex) {
			  formIter.borderColor = draggedColor;
			}
			else {
			  if (!key && formIter.borderColor == draggedColor) {
				formIter.borderColor = VerticalIndex;
			  }
			}
		  }
		}
		if (!switchColors) {
		  auto const swapColor                   = UserColor[wrap::toSize(VerticalIndex)];
		  UserColor[wrap::toSize(VerticalIndex)] = UserColor[wrap::toSize(draggedColor)];
		  if (!key) {
			UserColor[wrap::toSize(draggedColor)] = swapColor;
			nuscol(draggedColor);
		  }
		  nuscol(VerticalIndex);
		}
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void thred::internal::inscol() {
  auto colorMap = boost::dynamic_bitset<>(COLORCNT);
  VerticalIndex &= COLMSK;
  for (auto& stitch : *StitchBuffer) {
	colorMap.set(stitch.attribute & COLMSK);
  }
  if (colorMap.all()) {
	displayText::tabmsg(IDS_COLAL);
  }
  else {
	auto nextColor = COLORMAX;
	while (colorMap.test(nextColor)) {
	  --nextColor;
	}
	for (auto& stitch : *StitchBuffer) {
	  auto const color = gsl::narrow<uint8_t>(stitch.attribute & COLMSK);
	  if (color >= VerticalIndex && color < nextColor) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color + 1U;
	  }
	}
	for (auto& formIter : *FormList) {
	  if (formIter.fillType != 0U) {
		if (formIter.fillColor >= VerticalIndex && formIter.fillColor < nextColor) {
		  ++(formIter.fillColor);
		}
		if (formIter.fillType == FTHF && formIter.fillInfo.feather.color >= VerticalIndex &&
		    formIter.fillInfo.feather.color < nextColor) {
		  ++(formIter.fillInfo.feather.color);
		}
	  }
	  if (formIter.edgeType != 0U) {
		if (formIter.borderColor >= VerticalIndex && formIter.borderColor < nextColor) {
		  ++(formIter.borderColor);
		}
	  }
	}
	for (auto iColor = nextColor; iColor > VerticalIndex; --iColor) {
	  if (iColor != 0U) {
		UserColor[iColor] = UserColor[iColor - 1U];
	  }
	  nuscol(iColor);
	}
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

auto thred::internal::usedcol(uint8_t index) -> bool {
  return std::any_of(StitchBuffer->begin(), StitchBuffer->end(), [&index](fPOINTATTR const& m) -> bool {
	return (m.attribute & COLMSK) == index;
  });
}

void thred::internal::delcol() {
  if (usedcol(VerticalIndex)) {
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
		  --(formIter.fillColor);
		}
		if (formIter.fillType == FTHF && formIter.fillInfo.feather.color > VerticalIndex) {
		  --(formIter.fillInfo.feather.color);
		}
	  }
	  if (formIter.edgeType != 0U) {
		if (formIter.borderColor > VerticalIndex) {
		  --(formIter.borderColor);
		}
	  }
	}
	for (auto iColor = VerticalIndex; iColor < COLORMAX; ++iColor) {
	  UserColor[iColor] = UserColor[wrap::toSize(iColor) + 1U];
	  nuscol(iColor);
	}
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void thred::internal::set1knot() {
  if ((!StitchBuffer->empty()) && StateMap->test(StateFlag::SELBOX)) {
	thred::savdo();
	auto buffer = std::vector<fPOINTATTR> {};
	buffer.reserve(StitchBuffer->size() + 5U);
	buffer.insert(buffer.begin(), StitchBuffer->begin(), wrap::next(StitchBuffer->begin(), ClosestPointIndex + 1U));
	if (ClosestPointIndex == wrap::toUnsigned(StitchBuffer->size() - 1U)) {
	  StateMap->set(StateFlag::FILDIR);
	  endknt(buffer, ClosestPointIndex);
	}
	else {
	  strtknt(buffer, ClosestPointIndex);
	}
	buffer.insert(
	    buffer.end(), wrap::next(StitchBuffer->begin(), ClosestPointIndex + 1U), StitchBuffer->end());
	StitchBuffer->resize(buffer.size());
	*StitchBuffer = buffer;
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_NOSTCHSEL);
  }
}

void thred::internal::selfrm0() {
  StateMap->reset(StateFlag::GRPSEL);
  if (StateMap->testAndReset(StateFlag::FORMSEL)) {
	StateMap->set(StateFlag::FRMPSEL);
	ClosestVertexToCursor = 0;
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::selfrmx() {
  StateMap->reset(StateFlag::GRPSEL);
  if (StateMap->testAndReset(StateFlag::FORMSEL)) {
	StateMap->set(StateFlag::FRMPSEL);
	ClosestVertexToCursor = FormList->operator[](ClosestFormToCursor).vertexCount - 1U;
  }
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::setpclp() {
  FormVerticesAsLine->clear();
  auto it    = InterleaveSequence->begin();
  auto point = form::sfCor2px(*it);
  ++it;
  FormVerticesAsLine->push_back(point);
  point = form::sfCor2px(*it);
  auto const offset =
      POINT {Msg.pt.x - StitchWindowOrigin.x - point.x, Msg.pt.y - StitchWindowOrigin.y - point.y};
  for (auto ine = 1U; ine < wrap::toUnsigned(InterleaveSequence->size()) - 1U; ++ine) {
	point = form::sfCor2px(*it);
	++it;
	FormVerticesAsLine->push_back(POINT {point.x + offset.x, point.y + offset.y});
  }
  point = form::sfCor2px(InterleaveSequence->back());
  FormVerticesAsLine->push_back(point);
}

void thred::internal::dupclp() {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::Polyline(StitchWindowDC, FormVerticesAsLine->data(), wrap::toUnsigned(FormVerticesAsLine->size()));
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void thred::internal::unpclp() {
  if (StateMap->testAndReset(StateFlag::SHOP)) {
	dupclp();
  }
}

void thred::internal::fixpclp(uint32_t closestFormToCursor) {
  auto const point = POINT {(Msg.pt.x + gsl::narrow_cast<decltype(Msg.pt.x)>(FormMoveDelta.x)),
                            (Msg.pt.y + gsl::narrow_cast<decltype(Msg.pt.y)>(FormMoveDelta.y))};
  auto       it    = std::next(InterleaveSequence->begin());
  auto const stitchPoint            = thred::pxCor2stch(point);
  auto const offset                 = fPOINT {stitchPoint.x - it->x, stitchPoint.y - it->y};
  auto const count                  = wrap::toUnsigned(InterleaveSequence->size()) - 2U;
  auto& form                        = FormList->operator[](closestFormToCursor);
  auto const             nextVertex = form::nxt(form, ClosestVertexToCursor);
  form::fltspac(nextVertex, count);
  form.vertexCount += count;
  auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex + nextVertex);
  for (auto iOutput = size_t {1U}; iOutput < InterleaveSequence->size() - 1U; ++iOutput) {
	*vertexIt = fPOINT {it->x + offset.x, it->y + offset.y};
	++vertexIt;
	++it;
  }
  form::refil();
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
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
  StateMap->reset(StateFlag::FORMSEL);
  StateMap->set(StateFlag::FRMPSEL);
  StateMap->reset(StateFlag::FPSEL);
  StateMap->reset(StateFlag::SELBOX);
  SelectedFormVertices.start = ClosestVertexToCursor;
  SelectedFormVertices.form  = ClosestFormToCursor;
  auto const vertexIt =
      wrap::next(FormVertices->cbegin(),
                 FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
  thred::ritfcor(*vertexIt);
  StateMap->set(StateFlag::RESTCH);
}

void thred::internal::esccode() {
  bitmap::chkbit();
  duhbit(MF_UNCHECKED);
  unthum();
  StateMap->reset(StateFlag::MOVSET);
  StateMap->reset(StateFlag::HID);
  StateMap->reset(StateFlag::FRMOF);
  StateMap->reset(StateFlag::THRDS);
  thred::redraw(ButtonWin->operator[](HHID));
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
  StateMap->reset(StateFlag::COL);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
  StateMap->set(StateFlag::DUMEN);
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

void thred::internal::qcode() {
  trace::untrace();
  StateMap->reset(StateFlag::HIDMAP);
  StateMap->reset(StateFlag::FILDIR);
  ReleaseCapture();
  if (StitchBuffer->size() == 1) {
	StitchBuffer->clear();
	StateMap->reset(StateFlag::INIT);
  }
  if (StateMap->testAndReset(StateFlag::POLIMOV)) { // aborting form add
	auto& form = FormList->back();
	if (form.vertexCount != 0U) {
	  auto const first = wrap::next(FormVertices->begin(), form.vertexIndex);
	  auto const last  = wrap::next(first, form.vertexCount);
	  FormVertices->erase(first, last);
	}
	if (form.satinGuideCount != 0U) {
	  auto const first = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
	  auto const last  = wrap::next(first, form.satinGuideCount);
	  SatinGuides->erase(first, last);
	}
	FormList->pop_back();
	if (!FormList->empty()) {
	  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
	}
  }
  if (StateMap->testAndReset(StateFlag::FUNCLP)) { // aborting form paste
	thi::bak();
	ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  }
  if (StateMap->testAndReset(StateFlag::FUNSCLP)) { // aborting forms paste
	thi::bak();
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

	rstAll();
  }
  else {
	rstAll();
  }
  StateMap->set(StateFlag::RESTCH);
  auto const blank = std::wstring {};
  displayText::butxt(HNUM, blank);
}

void thred::internal::drwLin(std::vector<POINT>& linePoints, uint32_t currentStitch, uint32_t length, HPEN hPen) {
  if (!StitchBuffer->empty() && length != 0) {
	auto activeStitch = wrap::next(StitchBuffer->cbegin(), currentStitch);
	if (ActiveLayer != 0U) {
	  linePoints.clear();
	}
	auto iOffset = 0U;
	for (; iOffset < length; ++iOffset) {
	  auto const layer = (activeStitch->attribute & LAYMSK) >> LAYSHFT;
	  if ((ActiveLayer == 0U) || (layer == 0U) || (layer == ActiveLayer)) {
		linePoints.push_back({std::lround((activeStitch->x - ZoomRect.left) * ZoomRatio.x),
		                      std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
		                                  (activeStitch->y - ZoomRect.bottom) * ZoomRatio.y)});
	  }
	  ++activeStitch;
	}
	--activeStitch;
	SelectObject(StitchWindowMemDC, hPen);
	wrap::Polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
	linePoints.clear();
	auto const layer = (activeStitch->attribute & LAYMSK) >> LAYSHFT;
	if ((ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
	  if (iOffset != 0U) {
		linePoints.push_back({std::lround((activeStitch->x - ZoomRect.left) * ZoomRatio.x),
		                      std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
		                                  (activeStitch->y - ZoomRect.bottom) * ZoomRatio.y)});
	  }
	  else {
		activeStitch = wrap::next(StitchBuffer->cbegin(), currentStitch);
		linePoints.push_back({std::lround((activeStitch->x - ZoomRect.left) * ZoomRatio.x),
		                      std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
		                                  (activeStitch->y - ZoomRect.bottom) * ZoomRatio.y)});
		// Todo - I am not sure how we could ever reach here. Leave throw to test
		throw;
	  }
	}
  }
}

auto CALLBACK thred::internal::fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  auto const featherType = IniFile.featherType;
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), fmt::format(L"{:.2f}", IniFile.featherRatio).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), fmt::format(L"{}", IniFile.featherUpCount).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), fmt::format(L"{}", IniFile.featherDownCount).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFLR),
	                fmt::format(L"{:.2f}", (IniFile.featherMinStitchSize * IPFGRAN)).c_str());
	  SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), fmt::format(L"{}", IniFile.featherCount).c_str());
	  auto featherStyle = std::wstring {};
	  for (auto iFeatherStyle = 0U; iFeatherStyle < FSSIZE; ++iFeatherStyle) {
		displayText::loadString(featherStyle, (IDS_FTH0 + iFeatherStyle));
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
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
		  auto buf = std::array<wchar_t, HBUFSIZ> {0};
		  GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), buf.data(), HBUFSIZ);
		  IniFile.featherFillType = FDEFTYP;
		  auto buffer               = std::wstring {};
		  for (auto iFeatherStyle = uint8_t {}; iFeatherStyle < FSSIZE; ++iFeatherStyle) {
			displayText::loadString(buffer, IDS_FTH0 + iFeatherStyle);
			if (wcscmp(buf.data(), buffer.c_str()) == 0) {
			  IniFile.featherFillType = iFeatherStyle + 1U;
			  break;
			}
		  }
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf.data(), HBUFSIZ);
		  IniFile.featherRatio = wrap::wcstof(buf.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.featherUpCount, buf.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.featherDownCount, buf.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf.data(), HBUFSIZ);
		  IniFile.featherMinStitchSize = wrap::wcstof(buf.data()) * PFGRAN;
		  GetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.featherCount, buf.data());
		  if (IniFile.featherCount < 1) {
			IniFile.featherCount = 1;
		  }
		  EndDialog(hwndlg, 1);
		  break;
		}
		default: {
		  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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

void thred::internal::dufdef() noexcept {
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-type-cstyle-cast)
  DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, reinterpret_cast<DLGPROC>(fthdefprc));
}

auto thred::internal::handleMouseMove(std::vector<POINT>& stretchBoxLine,
                                      float               xyRatio,
                                      float&              rotationAngle,
                                      fPOINT const&       rotationCenter,
                                      FRMHED const&       textureForm) -> bool {
  if (StateMap->test(StateFlag::TXTMOV)) {
	texture::txtrmov(textureForm);
	return true;
  }
  movchk();
  if (thred::inStitchWin()) {
	if (wrap::pressed(VK_SHIFT)) {
	  xt::mvshft();
	  return true;
	}
	do {
	  if ((wrap::pressed(VK_SHIFT)) && thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  if ((StateMap->test(StateFlag::PRFACT) || StateMap->test(StateFlag::FORMIN) ||
	       StateMap->test(StateFlag::POLIMOV)) ||
	      (FormDataSheet != nullptr)) {
		wrap::setCursor(ArrowCursor);
		break;
	  }
	  if (!StateMap->test(StateFlag::INIT)) {
		if (UserFlagMap->test(UserFlag::NEDOF)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(NeedleUpCursor);
		}
		break;
	  }
	  if (StateMap->test(StateFlag::INSRT)) {
		if (UserFlagMap->test(UserFlag::NEDOF)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  if (StateMap->test(StateFlag::LIN1)) {
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
	  if (StateMap->test(StateFlag::BZUMIN) || StateMap->test(StateFlag::BOXZUM) ||
	      StateMap->test(StateFlag::SELPNT)) {
		wrap::setCursor(CrossCursor);
		break;
	  }
	  if (StateMap->test(StateFlag::SATIN) || StateMap->test(StateFlag::SATPNT) ||
	      StateMap->test(StateFlag::INSFRM)) {
		if (UserFlagMap->test(UserFlag::FRMX)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(FormCursor);
		}
		break;
	  }
	  if (StateMap->test(StateFlag::SATCNKT)) {
		wrap::setCursor(DLineCursor);
	  }
	  else {
		wrap::setCursor(ArrowCursor);
	  }
	} while (false);
	if (StateMap->test(StateFlag::FPUNCLP)) {
	  unpclp();
	  setpclp();
	  StateMap->set(StateFlag::SHOP);
	  dupclp();
	}
	if (StateMap->test(StateFlag::INSFIL)) {
	  form::unfrm();
	  auto const point = POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  thred::insflin(point);
	  StateMap->set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::MOVFRMS)) {
	  unstrtch(stretchBoxLine);
	  stretchBoxLine[0].x = stretchBoxLine[3].x = stretchBoxLine[4].x =
	      Msg.pt.x - std::lround(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[1].x = stretchBoxLine[2].x = Msg.pt.x + std::lround(SelectedFormsSize.x) -
	                                              std::lround(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[0].y = stretchBoxLine[1].y = stretchBoxLine[4].y =
	      Msg.pt.y - std::lround(FormMoveDelta.y) - StitchWindowOrigin.y;
	  stretchBoxLine[2].y = stretchBoxLine[3].y = Msg.pt.y + std::lround(SelectedFormsSize.y) -
	                                              std::lround(FormMoveDelta.y) - StitchWindowOrigin.y;
	  if (isLine(stretchBoxLine)) {
		stretchBoxLine[0].x -= 1;
		stretchBoxLine[0].y -= 1;
		stretchBoxLine[1].x += 1;
		stretchBoxLine[1].y -= 1;
		stretchBoxLine[2].x += 1;
		stretchBoxLine[2].y += 1;
		stretchBoxLine[3].x -= 1;
		stretchBoxLine[3].y += 1;
		stretchBoxLine[4].x -= 1;
		stretchBoxLine[4].y -= 1;
	  }
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap->test(StateFlag::POLIMOV)) {
	  form::munfrm();
	  form::setmfrm();
	  StateMap->set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::EXPAND)) {
	  unstrtch(stretchBoxLine);
	  auto newSize = fPOINT {gsl::narrow<float>(Msg.pt.x - StitchWindowOrigin.x),
	                         gsl::narrow<float>(Msg.pt.y - StitchWindowOrigin.y)};

	  auto       iSide = (SelectedFormControlVertex + 2U) % 4U;
	  auto const ratio = std::fabs((newSize.x - wrap::toFloat(stretchBoxLine[iSide].x)) /
	                               (newSize.y - wrap::toFloat(stretchBoxLine[iSide].y)));
	  if ((iSide & 1U) != 0U) {
		if (ratio < xyRatio) {
		  newSize.x = (wrap::toFloat(stretchBoxLine[iSide].y) - newSize.y) * xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].x);
		}
		else {
		  newSize.y = (wrap::toFloat(stretchBoxLine[iSide].x) - newSize.x) / xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].y);
		}
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
	  }
	  else {
		if (ratio < xyRatio) {
		  newSize.x = (newSize.y - wrap::toFloat(stretchBoxLine[iSide].y)) * xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].x);
		}
		else {
		  newSize.y = (newSize.x - wrap::toFloat(stretchBoxLine[iSide].x)) / xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].y);
		}
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
	  }
	  stretchBoxLine[4] = stretchBoxLine[0];
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap->test(StateFlag::STRTCH)) {
	  unstrtch(stretchBoxLine);
	  auto const lineLength = ((SelectedFormControlVertex & 1U) != 0U)
	                              ? Msg.pt.x - StitchWindowOrigin.x
	                              : Msg.pt.y - StitchWindowOrigin.y;
	  auto const dst  = (SelectedFormControlVertex + 2U) % 4U;
	  auto const code = nxtcrnr(dst);
	  for (auto iSide = 0U; iSide < 4; ++iSide) {
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
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap->test(StateFlag::INSFRM)) {
	  form::uninsf();
	  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  StateMap->set(StateFlag::SHOINSF);
	  form::duinsf();
	  return true;
	}
	if (StateMap->test(StateFlag::FUNCLP)) {
	  form::unfrm();
	  form::setmfrm();
	  StateMap->set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::SATCNKT)) {
	  form::drwcon();
	  return true;
	}
	if (StateMap->test(StateFlag::SATPNT)) {
	  satin::drwsat();
	  return true;
	}
	if (StateMap->test(StateFlag::FRMOV)) {
	  form::munfrm();
	  form::setmfrm();
	  StateMap->set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::FRMPMOV)) {
	  unmov();
	  RubberBandLine->operator[](1) = {Msg.pt.x - StitchWindowOrigin.x,
	                                   Msg.pt.y - StitchWindowOrigin.y};
	  StateMap->set(StateFlag::SHOMOV);
	  thred::ritmov(ClosestFormToCursor);
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  return true;
	}
	if (StateMap->test(StateFlag::MOVCNTR)) {
	  unrot();
	  ritrot(rotationAngle, thred::pxCor2stch(Msg.pt));
	  return true;
	}
	if (StateMap->test(StateFlag::ROTCAPT)) {
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
		  rotationAngle = PI_FHALF;
		}
		else {
		  rotationAngle = -PI_FHALF;
		}
	  }
	  rotationAngle -= RotationHandleAngle;
	  ritrot(rotationAngle, rotationCenter);
	  StateMap->set(StateFlag::ROTUSHO);
	  durotu();
	  return true;
	}
	if (StateMap->test(StateFlag::SELPNT)) {
	  if (StateMap->testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  unsel();
	  rSelbox();
	  return true;
	}
	if (StateMap->test(StateFlag::CLPSHO)) {
	  unclp();
	  clpbox();
	  return true;
	}
	if (StateMap->test(StateFlag::CAPT)) {
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  unlin();
	  MoveLine0[1] = MoveLine1[0] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  dulin(MoveLine0, MoveLine1);
	  return true;
	}
	if (StateMap->test(StateFlag::INSRT)) {
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  if (StateMap->testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  if (StateMap->test(StateFlag::LIN1)) {
		if (!StitchBuffer->empty()) {
		  xlin1();
		  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		  StateMap->set(StateFlag::ILIN1);
		  ilin1();
		}
		return true;
	  }
	  xlin();
	  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  StateMap->set(StateFlag::ILIN);
	  ilin();
	  return true;
	}
	if (StateMap->test(StateFlag::BOXZUM) && StateMap->testAndSet(StateFlag::VCAPT)) {
	  SetCapture(ThrEdWindow);
	}
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	if (StateMap->test(StateFlag::BZUMIN) && ((Msg.wParam & MK_LBUTTON) != 0U)) {
	  if (StateMap->testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  thred::unbBox();
	  ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
	  StateMap->set(StateFlag::BZUM);
	  thred::bBox();
	  return true;
	}
  }
  else {
	SetCursor(ArrowCursor);
	wrap::setCursor(ArrowCursor);
	if (StateMap->testAndReset(StateFlag::VCAPT)) {
	  ReleaseCapture();
	}
  }
  return true;
}

auto thred::internal::handleLeftButtonUp(float xyRatio, float rotationAngle, fPOINT& rotationCenter, bool& retflag)
    -> bool {
  retflag = true;
  if ((wrap::pressed(VK_SHIFT)) && thred::inStitchWin()) {
	texture::setshft();
	return true;
  }
  if (StateMap->test(StateFlag::TXTRED)) {
	texture::txtrup();
	return true;
  }
  ReleaseCapture();
  movchk();
  if (StateMap->testAndReset(StateFlag::MOVFRMS)) {
	thred::savdo();
	auto const point =
	    POINT {(Msg.pt.x - std::lround(FormMoveDelta.x) - StitchWindowOrigin.x) - SelectedFormsRect.left,
	           (Msg.pt.y - std::lround(FormMoveDelta.y) - StitchWindowOrigin.y) - SelectedFormsRect.top};
	form::ratsr();
	FormMoveDelta = fPOINT {wrap::toFloat(point.x) / HorizontalRatio, wrap::toFloat(point.y) / VerticalRatio};
	if (StateMap->test(StateFlag::FPSEL)) {
	  // clang-format off
	  auto const& form            = FormList->operator[](ClosestFormToCursor);
	  auto        iSelectedVertex = SelectedFormVertices.start;
	  auto const vertexIt        = wrap::next(FormVertices->begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto thisIt = wrap::next(vertexIt, iSelectedVertex);
		thisIt->x += FormMoveDelta.x;
		thisIt->y -= FormMoveDelta.y;
		iSelectedVertex = form::pdir(form, iSelectedVertex);
	  }
	  thred::setpsel();
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::BIGBOX)) {
		thred::savdo();
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
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
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	return true;
  }
  if (StateMap->testAndReset(StateFlag::EXPAND)) {
	form::setexpand(xyRatio);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::STRTCH)) {
	form::setstrtch();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FRMOV)) {
	thred::savdo();
	form::rstfrm();
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FRMPMOV)) {
	thred::savdo();
	form::setfpnt();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::MOVCNTR)) {
	rotationCenter = thred::pxCor2stch(Msg.pt);
	StateMap->set(StateFlag::ROTAT);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::ROTCAPT)) {
	thred::rotfn(rotationAngle, rotationCenter);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::SELPNT)) {
	thred::savdo();
	ReleaseCapture();
	unsel();
	auto const stitchPoint = thred::pxCor2stch(Msg.pt);
	auto const adjustedPoint =
	    fPOINT {(StitchRangeRect.left + wrap::toFloat(SelectBoxOffset.x)) - stitchPoint.x,
	            (StitchRangeRect.bottom + wrap::toFloat(SelectBoxOffset.y)) - stitchPoint.y};
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  StitchBuffer->operator[](iStitch).x -= adjustedPoint.x;
	  StitchBuffer->operator[](iStitch).y -= adjustedPoint.y;
	}
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::CAPT)) {
	unlin();
	ReleaseCapture();
	StateMap->reset(StateFlag::CAPT);
	thred::savdo();
	auto const stitchPoint = thred::pxCor2stch(Msg.pt);

	StitchBuffer->operator[](ClosestPointIndex).x = stitchPoint.x;
	StitchBuffer->operator[](ClosestPointIndex).y = stitchPoint.y;
	StitchBuffer->operator[](ClosestPointIndex).attribute |= USMSK;
	if (ZoomFactor < STCHBOX) {
	  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	  SelectObject(StitchWindowMemDC, LinePen);
	  stchbox(ClosestPointIndex - 1, StitchWindowDC);
	  stchbox(ClosestPointIndex, StitchWindowDC);
	  if (ClosestPointIndex < wrap::toUnsigned(StitchBuffer->size() - 1U)) {
		stchbox(ClosestPointIndex + 1, StitchWindowDC);
	  }
	  SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
	StateMap->set(StateFlag::SELBOX);
	StateMap->reset(StateFlag::FRMPSEL);
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->test(StateFlag::BZUMIN)) {
	auto stitchPoint = thred::pxCor2stch(Msg.pt);
	if (StateMap->testAndReset(StateFlag::BOXSLCT)) {
	  if (ZoomBoxOrigin.x > stitchPoint.x) {
		StitchRangeRect.right = ZoomBoxOrigin.x;
		StitchRangeRect.left  = stitchPoint.x;
	  }
	  else {
		StitchRangeRect.right = stitchPoint.x;
		StitchRangeRect.left  = ZoomBoxOrigin.x;
	  }
	  if (ZoomBoxOrigin.y > stitchPoint.y) {
		StitchRangeRect.top    = ZoomBoxOrigin.y;
		StitchRangeRect.bottom = stitchPoint.y;
	  }
	  else {
		StitchRangeRect.top    = stitchPoint.y;
		StitchRangeRect.bottom = ZoomBoxOrigin.y;
	  }
	  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
		thred::rngadj();
		for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; ++iStitch) {
		  if (inrng(iStitch)) {
			ClosestPointIndex = iStitch;
			StateMap->set(StateFlag::SELBOX);
			break;
		  }
		}
	  }
	  if (StateMap->testAndReset(StateFlag::NOSEL)) {
		SelectedFormList->clear();
		// We potentially reserve too much memory, but the cost of reallocatioon is higher than the
		// small amount overallocated
		SelectedFormList->reserve(FormList->size());
		StateMap->reset(StateFlag::FORMSEL);
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
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
		  StateMap->set(StateFlag::RESTCH);
		  StateMap->set(StateFlag::FORMSEL);
		  return true;
		}
		if (!SelectedFormList->empty()) {
		  gotbox();
		  return true;
		}
		StateMap->reset(StateFlag::BZUMIN);
		StateMap->reset(StateFlag::BOXSLCT);
		StateMap->reset(StateFlag::FRMPSEL);
		StateMap->set(StateFlag::RESTCH);
	  }
	  if (StateMap->testAndReset(StateFlag::SELBOX)) {
		if (inrng(ClosestPointIndex)) {
		  GroupStitchIndex = ClosestPointIndex + 1U;
		  --ClosestPointIndex;
		  while (inrng(ClosestPointIndex)) {
			--ClosestPointIndex;
		  }
		  ++ClosestPointIndex;
		  while (inrng(GroupStitchIndex)) {
			++GroupStitchIndex;
		  }
		  --GroupStitchIndex;
		  StateMap->set(StateFlag::GRPSEL);
		  gotbox();
		  return true;
		}
	  }
	  if (!StateMap->test(StateFlag::INSRT)) {
		rngal();
	  }
	  //				gotbox();
	  return true;
	}

	if (stitchPoint.x < ZoomBoxOrigin.x) {
	  std::swap(ZoomBoxOrigin.x, stitchPoint.x);
	}
	if (stitchPoint.y < ZoomBoxOrigin.y) {
	  std::swap(ZoomBoxOrigin.y, stitchPoint.y);
	}
	auto newSize = fPOINT {stitchPoint.x - ZoomBoxOrigin.x, stitchPoint.y - ZoomBoxOrigin.y};
	stitchPoint  = fPOINT {ZoomBoxOrigin.x + newSize.x * 0.5F, ZoomBoxOrigin.y + newSize.y * 0.5F};
	auto const saveFactor = ZoomFactor;
	if (newSize.x > newSize.y) {
	  newSize.y  = newSize.x / StitchWindowAspectRatio;
	  ZoomFactor = newSize.x / wrap::toFloat(UnzoomedRect.x);
	}
	else {
	  newSize.x  = newSize.y * StitchWindowAspectRatio;
	  ZoomFactor = newSize.y / wrap::toFloat(UnzoomedRect.x);
	}
	if (ZoomFactor < ZoomMin) {
	  ZoomFactor = saveFactor;
	  zumin();
	  return true;
	}
	ZoomRect = fRECTANGLE {0.0F, newSize.y, newSize.x, 0.0F};
	thred::shft(stitchPoint);
	StateMap->reset(StateFlag::BZUMIN);
	StateMap->set(StateFlag::RESTCH);
	if (!StateMap->testAndSet(StateFlag::ZUMED)) {
	  thred::movStch();
	}
	return true;
	//			StateMap->reset(StateFlag::BZUMIN);
  }
  retflag = false;
  return false;
}

auto thred::internal::handleEitherButtonDown(bool& retflag) -> bool {
  retflag = true;
  if (FormDataSheet != nullptr) {
	auto formDataRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(FormDataSheet, &formDataRect);
  }
  if (StateMap->testAndReset(StateFlag::THUMON)) {
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
	StateMap->set(StateFlag::BAKSHO);
	thred::unbsho();
	StateMap->reset(StateFlag::THUMSHO);
	thred::unmsg();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::BAKSHO)) {
	if (Msg.message == WM_RBUTTONDOWN) {
	  StateMap->set(StateFlag::RBUT);
	}
	else {
	  StateMap->reset(StateFlag::RBUT);
	}
	{
	  auto iVersion = uint8_t {};
	  for (auto& iBackup : BackupViewer) {
		if (Msg.hwnd == iBackup) {
		  FileVersionIndex = iVersion;
		  if (StateMap->test(StateFlag::THUMSHO)) {
			if (savcmp()) {
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
			rebak();
		  }
		  rstAll();
		  StateMap->set(StateFlag::RESTCH);
		  return true;
		}
		++iVersion;
	  }
	}
  }
  if (Msg.pt.x >= ColorBarRect.left && Msg.pt.x <= ColorBarRect.right &&
      Msg.pt.y >= ColorBarRect.top && Msg.pt.y <= ColorBarRect.bottom) {
	unpat();
	auto const colorBarPosition = wrap::toFloat(Msg.pt.y - ColorBarRect.top) /
	                              wrap::toFloat(ColorBarRect.bottom - ColorBarRect.top);
	if (Msg.message == WM_RBUTTONDOWN) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if (((Msg.wParam & MK_SHIFT) != 0U) &&
	      (StateMap->test(StateFlag::SELBOX) || StateMap->test(StateFlag::GRPSEL))) {
		unbox();
		GroupStitchIndex = wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(StitchBuffer->size()));
		StateMap->set(StateFlag::GRPSEL);
		thred::grpAdj();
		nuAct(GroupStitchIndex);
		StateMap->set(StateFlag::RESTCH);
	  }
	  else {
		ClosestPointIndex = wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(StitchBuffer->size()));
		nuAct(ClosestPointIndex);
		movbox();
		if (StateMap->testAndReset(StateFlag::GRPSEL)) {
		  StateMap->reset(StateFlag::SCROS);
		  StateMap->reset(StateFlag::ECROS);
		  StateMap->set(StateFlag::RESTCH);
		}
	  }
	}
	else {
	  ClosestPointIndex = wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(StitchBuffer->size()));
	  nuAct(ClosestPointIndex);
	  rstAll();
	  StateMap->set(StateFlag::SELBOX);
	  selCol();
	}
	thred::redraw(ColorBar);
	return true;
  }
  retflag = false;
  return false;
}

auto thred::internal::handleRightButtonDown() -> bool {
  if (StateMap->test(StateFlag::TXTRED) && (MsgWindow == nullptr)) {
	texture::txtrbut();
	return true;
  }
  if (wrap::pressed(VK_SHIFT)) {
	if (form::closfrm()) {
	  if ((StateMap->test(StateFlag::FRMPSEL) || StateMap->test(StateFlag::FPSEL)) &&
	      SelectedFormVertices.form == ClosestFormToCursor) {
		auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
		StateMap->reset(StateFlag::FRMPSEL);
		StateMap->set(StateFlag::FPSEL);
		SelectedFormVertices.finish = ClosestVertexToCursor;
		auto const selectedVertexCount =
		    (SelectedFormVertices.finish - SelectedFormVertices.start + vertexCount) % vertexCount;
		if (selectedVertexCount < vertexCount / 2U) {
		  SelectedFormVertices.vertexCount = selectedVertexCount;
		  StateMap->set(StateFlag::PSELDIR);
		}
		else {
		  SelectedFormVertices.vertexCount = vertexCount - selectedVertexCount;
		  StateMap->reset(StateFlag::PSELDIR);
		}
		thred::setpsel();
		return true;
	  }
	  StateMap->reset(StateFlag::FPSEL);
	  StateMap->set(StateFlag::FRMPSEL);
	  SelectedFormVertices.start = ClosestVertexToCursor;
	  SelectedFormVertices.form  = ClosestFormToCursor;
	  selfpnt();
	  return true;
	}
  }
  if (StateMap->test(StateFlag::WASTRAC)) {
	trace::wasTrace1();
	return true;
  }
  thred::unmsg();
  unboxs();
  unpat();
  StateMap->reset(StateFlag::SATIN);
  if (StateMap->testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  if (StateMap->test(StateFlag::BAKSHO)) {
	thred::unbsho();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::BIGBOX)) {
	StateMap->set(StateFlag::RESTCH);
  }
  if (StateMap->test(StateFlag::PRFACT)) {
	if (StateMap->testAndReset(StateFlag::HUPMSG)) {
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
		StateMap->reset(StateFlag::PRFACT);
		if (StateMap->testAndReset(StateFlag::WASRT)) {
		  StateMap->set(StateFlag::INSRT);
		}
	  }
	}
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (FormMenuChoice != 0U) {
	chknum();
	FormMenuChoice = 0;
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::INSFRM)) {
	insadj();
	StateMap->reset(StateFlag::SHOINSF);
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (thred::inStitchWin() && !(StateMap->test(StateFlag::SIZSEL) &&
                                thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
	if (!FormList->empty() && !StateMap->test(StateFlag::FRMOF)) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if ((Msg.wParam & MK_SHIFT) != 0U) {
		auto tempIndex = ClosestFormToCursor;
		if (form::closfrm()) {
		  // ToDo - I don't think this can ever be hit with closfrm
		  if (!SelectedFormList->empty()) {
			nuslst(ClosestFormToCursor);
			StateMap->set(StateFlag::RESTCH);
			return true;
		  }
		  if (StateMap->testAndReset(StateFlag::FORMSEL) && tempIndex != ClosestFormToCursor) {
			if (tempIndex > ClosestFormToCursor) {
			  std::swap(ClosestFormToCursor, tempIndex);
			}
			for (auto iForm = tempIndex; iForm <= ClosestFormToCursor; ++iForm) {
			  SelectedFormList->push_back(iForm);
			}
			StateMap->set(StateFlag::RESTCH);
			return true;
		  }

		  form::nufsel();
		}
	  }
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if ((Msg.wParam & MK_CONTROL) != 0U) {
		if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		  StateMap->set(StateFlag::WASEL);
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
	  if (StateMap->test(StateFlag::FORMSEL)) {
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
			StateMap->set(StateFlag::RESTCH);
			break;
		  }
		  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		  lenCalc();
		  if (!StateMap->testAndReset(StateFlag::ENTROT)) {
			StateMap->reset(StateFlag::FORMSEL);
		  }
		  StateMap->set(StateFlag::DUMEN);
		  if (!closPnt1(ClosestPointIndex)) {
			unbox();
		  }
		} while (false);
	  }
	  if (form::closfrm()) {
		StateMap->set(StateFlag::FORMSEL);
		StateMap->reset(StateFlag::FPSEL);
		form::unpsel();
		form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		if (StateMap->testAndReset(StateFlag::FRMPSEL) || !SelectedFormList->empty()) {
		  StateMap->set(StateFlag::RESTCH);
		  SelectedFormList->clear();
		}
		displayText::ritnum(STR_NUMFORM, ClosestFormToCursor);
		lenCalc();
		unrot();
		return true;
	  }
	  if (!SelectedFormList->empty()) {
		SelectedFormList->clear();
		StateMap->set(StateFlag::RESTCH);
	  }
	  if (StateMap->testAndReset(StateFlag::FRMPSEL)) {
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	if (StateMap->test(StateFlag::INIT) || (!ThrName->empty())) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if ((Msg.wParam & MK_SHIFT) != 0U) {
		if (StateMap->test(StateFlag::SELBOX)) {
		  auto const code = ClosestPointIndex;
		  closPnt1(ClosestPointIndex);
		  if (ClosestPointIndex != code) {
			unbox();
			GroupStitchIndex  = ClosestPointIndex;
			ClosestPointIndex = code;
			StateMap->set(StateFlag::GRPSEL);
			thred::grpAdj();
		  }
		  nuAct(GroupStitchIndex);
		  thred::redraw(ColorBar);
		  return true;
		}
		if (StateMap->test(StateFlag::GRPSEL)) {
		  auto const code = ClosestPointIndex;
		  closPnt1(ClosestPointIndex);
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
		StateMap->reset(StateFlag::LENSRCH);
		StateMap->reset(StateFlag::WASGRP);
		StateMap->reset(StateFlag::FPSEL);
		StateMap->reset(StateFlag::WASFRMFRM);
		StateMap->reset(StateFlag::SIDACT);
		if (StateMap->testAndReset(StateFlag::INSRT)) {
		  ReleaseCapture();
		  rstAll();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
			StateMap->set(StateFlag::RESTCH);
		  }
		  if (!StitchBuffer->empty()) {
			rebox();
		  }
		}
	  }
	  xt::clrstch();
	  StateMap->set(StateFlag::RESTCH);
	}
	return true;
  }
  if (!ButtonWin->empty()) {
	auto minLenRect = RECT {};
	if (GetWindowRect(ButtonWin->operator[](HMINLEN), &minLenRect) != 0) {
	  if (Msg.pt.x >= minLenRect.left && Msg.pt.x <= minLenRect.right &&
	      Msg.pt.y > minLenRect.top && Msg.pt.y <= minLenRect.bottom) {
		srchk();
		setsrch(SmallestStitchIndex);
		lensadj();
		StateMap->set(StateFlag::GRPSEL);
		StateMap->set(StateFlag::RESTCH);
		return true;
	  }
	}
	auto maxLenRect = RECT {};
	if (GetWindowRect(ButtonWin->operator[](HMAXLEN), &maxLenRect) != 0) {
	  if (Msg.pt.x >= maxLenRect.left && Msg.pt.x <= maxLenRect.right &&
	      Msg.pt.y > maxLenRect.top && Msg.pt.y <= maxLenRect.bottom) {
		srchk();
		setsrch(LargestStitchIndex);
		lensadj();
		StateMap->set(StateFlag::GRPSEL);
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
  return true;
}

auto thred::internal::updateHoopSize() -> bool {
  for (auto iHoop = 0; iHoop < HUPS; ++iHoop) {
	if (Msg.hwnd == SideWindow[wrap::toSize(iHoop)]) {
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
		default: {
		  outDebugString(L"default hit in updateHoopSize: iHoop [{}]\n", iHoop);
		  break;
		}
	  }
	  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
	  form::sethup();
	  thred::chkhup();
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
	  FormList->operator[](ClosestFormToCursor).underlayColor = VerticalIndex & COLORBTS;
	  form::refilfn();
	  thred::coltab();
	  break;
	}
	if (StateMap->testAndReset(StateFlag::FSETUCOL)) {
	  xt::dundcol(VerticalIndex + 1U);
	  return true;
	}
	form::nufilcol(VerticalIndex);
  } while (false);
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

auto thred::internal::updatePreferences() -> bool {
  chknum();
  if (Msg.hwnd == ValueWindow->operator[](PSQR)) {
	if (UserFlagMap->testAndFlip(UserFlag::SQRFIL)) {
	  SetWindowText(ValueWindow->operator[](PSQR), StringTable->operator[](STR_PNTD).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PSQR), StringTable->operator[](STR_SQR).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PBLNT)) {
	if (UserFlagMap->testAndFlip(UserFlag::BLUNT)) {
	  SetWindowText(ValueWindow->operator[](PBLNT), StringTable->operator[](STR_TAPR).c_str());
	}
	else {
	  SetWindowText(ValueWindow->operator[](PBLNT), StringTable->operator[](STR_BLUNT).c_str());
	}
	return true;
  }
  if (Msg.hwnd == ValueWindow->operator[](PUND)) {
	if (UserFlagMap->testAndFlip(UserFlag::DUND)) {
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

auto thred::internal::handleSideWindowActive() -> bool {
  thred::savdo();
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (FormMenuChoice == LFTHTYP) {
	for (auto iFillType = uint8_t {0U}; iFillType < FSSIZE; ++iFillType) {
	  if (Msg.hwnd == SideWindow[iFillType]) {
		form.fillInfo.feather.fillType = iFillType + 1U;
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
	for (; iLayer < MAXLAYER; ++iLayer) {
	  if (Msg.hwnd == SideWindow[iLayer]) {
		break;
	  }
	}
	if (iLayer < MAXLAYER) {
	  form::movlayr(iLayer * 2U);
	  StateMap->set(StateFlag::FORMSEL);
	}
	thred::unsid();
	auto const layerStr =
	    fmt::format(L"{}", (gsl::narrow_cast<decltype(form.attribute)>(form.attribute & FRMLMSK) >> 1U));
	SetWindowText(ValueWindow->operator[](LLAYR), layerStr.c_str());
	formForms::refrm();
	return true;
  }
  form.borderColor &= COLMSK;
  if (StateMap->testAndReset(StateFlag::BRDACT)) {
	if (clip::iseclp(form)) {
	  clip::deleclp(ClosestFormToCursor);
	}
	do {
	  if (Msg.hwnd == SideWindow[0]) {
		form.edgeType = 0;
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
		break;
	  }
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGELINE]) {
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
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEBEAN]) {
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
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGECLIP]) {
		form::fclp();
		break;
	  }
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEANGSAT]) {
		if (form.edgeType != 0U) {
		  switch (form.edgeType) {
			case EDGECLIP: {
			  form::bsizpar(form);
			  [[fallthrough]];
			}
			case EDGELINE: {
			  [[fallthrough]];
			}
			case EDGEBEAN: {
			  form.borderSize  = BorderWidth;
			  form.edgeSpacing = LineSpacing;
			  break;
			}
			case EDGEPROPSAT: {
			  form.edgeSpacing /= 2;
			  break;
			}
			default: {
			  outDebugString(L"default hit in handleSideWindowActive 1: edgeType [{}]\n", form.edgeType);
			  break;
			}
		  }
		  form.edgeType = EDGEANGSAT;
		  if (UserFlagMap->test(UserFlag::DUND)) {
			form.edgeType |= EGUND;
		  }
		  break;
		}
		satin::satbrd();
		break;
	  }
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEAPPL]) {
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
		  if (UserFlagMap->test(UserFlag::DUND)) {
			form.edgeType |= EGUND;
		  }
		  form.borderColor |= gsl::narrow_cast<uint8_t>(AppliqueColor << 4U);
		  break;
		}
		form::apliq();
		break;
	  }
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEPROPSAT]) {
		if (form.edgeType != 0U) {
		  switch (form.edgeType) {
			case EDGECLIP: {
			  form::bsizpar(form);
			  [[fallthrough]];
			}
			case EDGELINE: {
			  [[fallthrough]];
			}
			case EDGEBEAN: {
			  form.borderSize  = BorderWidth;
			  form.edgeSpacing = LineSpacing;
			  break;
			}
			case EDGEANGSAT: {
			  form.edgeSpacing *= 2;
			  break;
			}
			default: {
			  outDebugString(L"default hit in handleSideWindowActive 2: edgeType [{}]\n", form.edgeType);
			  break;
			}
		  }
		  form.edgeType = EDGEPROPSAT;
		  if (UserFlagMap->test(UserFlag::DUND)) {
			form.edgeType |= EGUND;
		  }
		  break;
		}
		form::prpbrd(LineSpacing);
		break;
	  }
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEBHOL]) {
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
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEPICOT]) {
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
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEDOUBLE]) {
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
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGELCHAIN]) {
		StateMap->set(StateFlag::LINCHN);
		form::chan();
		thred::coltab();
		break;
	  }
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGEOCHAIN]) {
		StateMap->reset(StateFlag::LINCHN);
		form::chan();
		thred::coltab();
		break;
	  }
	  if (Msg.hwnd == SideWindow[edgeFillStyles::EDGECLIPX]) {
		form::filclpx();
		break;
	  }
	} while (false);
	formForms::refrm();
	form::refil();
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
  do {
	if (Msg.hwnd == SideWindow[0]) { // none
	  form.type = FRMFPOLY;
	  form::delmfil(ClosestFormToCursor);
	  form.fillType = 0;
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::VRTF]) { // vertical fill
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
	if (Msg.hwnd == SideWindow[fillStyles::HORF]) { // horizontal fill
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
	if (Msg.hwnd == SideWindow[fillStyles::ANGF]) { // angle fill
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
	if (Msg.hwnd == SideWindow[fillStyles::SATF]) { // fan fill
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
	  StateMap->reset(StateFlag::FTHR);
	  form::filsat();
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::CLPF]) { // fan clip
	  form.type = SAT;
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinOrAngle.guide = 0;
	  }
	  form::clpfil();
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::CONTF]) { // contour fill
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
	if (Msg.hwnd == SideWindow[fillStyles::VCLPF]) { // vertical clip
	  if (sidclp()) {
		form::vrtsclp();
	  }
	  StateMap->reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::HCLPF]) { // horizontal clip
	  if (sidclp()) {
		form::horsclp();
	  }
	  StateMap->reset(StateFlag::CLPSHO);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::ANGCLPF]) { // angle clip
	  if (sidclp()) {
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
	if (Msg.hwnd == SideWindow[fillStyles::FTHF]) { // feather fill
	  if ((form.fillType == ANGF) || (form.fillType == ANGCLPF) || (form.fillType == TXANGF)) {
		form.satinOrAngle.guide = 0;
	  }
	  xt::fethrf();
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::TXVRTF]) // vertical texture
	{
	  if (texture::istx(ClosestFormToCursor)) {
		form.fillType = TXVRTF;
		break;
	  }
	  texture::dutxtfil();
	  textureFlag = true;
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::TXHORF]) // horizontal texture
	{
	  if (texture::istx(ClosestFormToCursor)) {
		form.fillType = TXHORF;
		break;
	  }
	  texture::dutxtfil();
	  textureFlag = true;
	  break;
	}
	if (Msg.hwnd == SideWindow[fillStyles::TXANGF]) // angle texture
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
	StateMap->set(StateFlag::WASFRMFRM);
  }
  else {
	formForms::refrm();
	form::refil();
	thred::unmsg();
	thred::unsid();
	StateMap->set(StateFlag::RESTCH);
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
	  StateMap->set(StateFlag::RESTCH);
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
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHBTH) || Msg.hwnd == LabelWindow->operator[](LFTHBTH)) {
	  form.extendedAttribute ^= AT_FTHBTH;
	  formForms::refrm();
	  form::refil();
	  thred::unsid();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFTHTYP) || Msg.hwnd == LabelWindow->operator[](LFTHTYP)) {
	  FormMenuChoice = LFTHTYP;
	  auto start = wrap::next(StringTable->begin(), STR_FTH0);
	  auto end = wrap::next(start, FSSIZE);
	  auto fthStr    = std::vector<std::wstring>(start,end);
	  sidmsg(form, ValueWindow->operator[](LFTHTYP), fthStr);
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
	  StateMap->set(StateFlag::RESTCH);
	  formForms::refrm();
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LLAYR) || Msg.hwnd == LabelWindow->operator[](LLAYR)) {
	  auto const layerText = std::vector<std::wstring> {L"0", L"1", L"2", L"3", L"4"};
	  FormMenuChoice           = LLAYR;
	  StateMap->reset(StateFlag::FILTYP);
	  sidmsg(form, ValueWindow->operator[](LLAYR), layerText);
	  break;
	}
	if (Msg.hwnd == ValueWindow->operator[](LFRMFIL) || Msg.hwnd == LabelWindow->operator[](LFRMFIL)) {
	  StateMap->reset(StateFlag::FILTYP);
	  FormMenuChoice = LFRMFIL;
	  auto start     = wrap::next(StringTable->begin(), STR_FIL0);
	  auto end       = wrap::next(start, FILLTYPS);
	  auto fillStr    = std::vector<std::wstring>(start, end);
	  sidmsg(form, ValueWindow->operator[](LFRMFIL), fillStr);
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
	  auto start   = wrap::next(StringTable->begin(), STR_EDG0);
	  auto end     = wrap::next(start, EDGETYPS);
	  auto edgeStr = std::vector<std::wstring>(start, end);
	  sidmsg(form, ValueWindow->operator[](LBRD), edgeStr);
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
	  StateMap->set(StateFlag::RESTCH);
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
  outDebugString(L"handleLeftButtonDown:FormMenuChoice [{}]\n", FormMenuChoice);

  return true;
}

auto thred::internal::handleLeftButtonDown(std::vector<POINT>& stretchBoxLine,
                                           float&              xyRatio,
                                           FRMHED const&       textureForm,
                                           bool&               retflag) -> bool {
  static auto threadSizeSelected = uint32_t {}; // thread selected for size change

  retflag = true;
  if ((wrap::pressed(VK_SHIFT)) && thred::inStitchWin()) {
	xt::dushft();
	return true;
  }
  if (StateMap->test(StateFlag::TXTRED) && !StateMap->test(StateFlag::FORMIN)) {
	texture::txtlbut(textureForm);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FSETFCOL)) {
	thred::unsid();
	thred::unmsg();
	if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	  xt::dufcol(VerticalIndex + 1U);
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::FSETBCOL)) {
	thred::unsid();
	thred::unmsg();
	if (thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	  xt::dubcol(VerticalIndex + 1U);
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::FPUNCLP)) {
	thred::savdo();
	fixpclp(ClosestFormToCursor);
	form::frmout(ClosestFormToCursor);
	return true;
  }
  if (StateMap->test(StateFlag::FPSEL) && !StateMap->test(StateFlag::FUNCLP) &&
      !StateMap->test(StateFlag::ROTAT)) {
	*SelectedFormsLine = *SelectedPointsLine;
	SelectedFormsRect  = SelectedPixelsRect;
	if (chkbig(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (StateMap->test(StateFlag::WASTRAC)) {
	trace::tracpar();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::HUPMSG)) {
	return thi::updateHoopSize();
  }
  if (StateMap->testAndReset(StateFlag::INSFIL)) {
	duinsfil();
	return true;
  }
  if (StateMap->test(StateFlag::BIGBOX) && chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (StateMap->testAndReset(StateFlag::DELSFRMS)) {
	auto code = wchar_t {};
	if (chkok()) {
	  thred::savdo();
	  StateMap->reset(StateFlag::DELTO);
	  code = 1;
	}
	else {
	  auto windowRect = RECT {0L, 0L, 0L, 0L};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
	      Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom) {
		thred::savdo();
		StateMap->set(StateFlag::DELTO);
		code = 1;
	  }
	}
	delsfrms(code);
	thred::unmsg();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::MOVMSG)) {
	if (chkok()) {
	  thred::savdo();
	  form::refilfn();
	}
	thred::unmsg();
	if (StateMap->testAndReset(StateFlag::WASFRMFRM)) {
	  formForms::refrm();
	}
	return true;
  }
  if (StateMap->test(StateFlag::FUNSCLP)) {
	StateMap->reset(StateFlag::MOVFRMS);
	auto formsRect = fRECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, formsRect);
	auto const stitchPoint = thred::pxCor2stch(Msg.pt);

	FormMoveDelta = fPOINT {stitchPoint.x - wrap::midl(formsRect.right, formsRect.left),
	                        stitchPoint.y - wrap::midl(formsRect.top, formsRect.bottom)};
	for (auto iForm = 0U; iForm < ClipFormsCount; ++iForm) {
	  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - iForm - 1U);
	  // clang-format off
	  auto& form     = FormList->operator[](ClosestFormToCursor);
	  auto  vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		vertexIt->x += FormMoveDelta.x;
		vertexIt->y += FormMoveDelta.y;
		++vertexIt;
	  }
	  form::frmout(ClosestFormToCursor);
	  form::refil();
	}
	StateMap->reset(StateFlag::FUNSCLP);
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (!SelectedFormList->empty() && !StateMap->test(StateFlag::ROTAT) && chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (StateMap->test(StateFlag::SIDCOL) &&
      thi::chkMsgs(Msg.pt, DefaultColorWin->front(), DefaultColorWin->back())) {
	return thi::updateFillColor();
  }
  if (StateMap->testAndReset(StateFlag::OSAV)) {
	if (chkok()) {
	  thred::save();
	  nuFil(fileIndices::THR);
	  thred::unmsg();
	  return true;
	}
	if (chkwnd(DiscardButton)) {
	  nuFil(fileIndices::THR);
	  thred::unmsg();
	  return true;
	}
	thred::unmsg();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FCLOS)) {
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
  if (StateMap->testAndReset(StateFlag::SAVEX)) {
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
  if (StateMap->test(StateFlag::PRFACT)) {
	return thi::updatePreferences();
  }
  if (!StateMap->test(StateFlag::ROTAT) && StateMap->test(StateFlag::GRPSEL)) {
	auto& controlPoint = *FormControlPoints;
	if (iselpnt()) {
	  for (auto iSide = 0U; iSide < wrap::toUnsigned(stretchBoxLine.size()); ++iSide) {
		stretchBoxLine[iSide] = controlPoint[wrap::toSize(iSide) * 2U];
	  }
	  stretchBoxLine.back() = stretchBoxLine.front();
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		StateMap->set(StateFlag::STRTCH);
	  }
	  else {
		StateMap->set(StateFlag::EXPAND);
		xyRatio = (StitchRangeRect.right - StitchRangeRect.left) /
		          (StitchRangeRect.top - StitchRangeRect.bottom);
	  }
	  SelectedFormControlVertex >>= 1U;
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	auto const relativePoint = POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	if (relativePoint.x >= controlPoint[0].x && relativePoint.x <= controlPoint[2].x &&
	    relativePoint.y >= controlPoint[0].y && relativePoint.y <= controlPoint[4].y) {
	  duSelbox();
	  StateMap->set(StateFlag::SELPNT);
	  SetCapture(ThrEdWindow);
	  StateMap->set(StateFlag::VCAPT);
	  rSelbox();
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::NEWBAK)) {
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
  if (StateMap->testAndReset(StateFlag::PRGMSG)) {
	if (chkok()) {
	  deldir();
	  return true;
	}
  }
  if (StateMap->test(StateFlag::DELFRM)) {
	auto code = 0;
	if (chkok()) {
	  thred::savdo();
	  StateMap->reset(StateFlag::DELTO);
	  code = 1;
	}
	else {
	  auto windowRect = RECT {0L, 0L, 0L, 0L};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
	      Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom) {
		thred::savdo();
		StateMap->set(StateFlag::DELTO);
		code = 1;
	  }
	}
	if (code != 0) {
	  thred::frmdel();
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	thred::unmsg();
	return true;
  }
  if (StateMap->test(StateFlag::FILMSG)) {
	if (chkok()) {
	  thred::savdo();
	  form::unfil();
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  thred::unmsg();
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::SIZSEL)) {
	if (thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2])) {
	  VerticalIndex -= 13U;
	  static constexpr auto threadSizeMap = std::array<wchar_t, 3> {L'3', L'4', L'6'};
	  ThreadSize[threadSizeSelected]      = threadSizeMap[VerticalIndex];
	  ThreadSizeIndex[threadSizeSelected] = VerticalIndex;
	  auto buffer = std::array<wchar_t, 3> {};
	  buffer[0]                           = ThreadSize[threadSizeSelected];
	  buffer[1]                           = L'0';
	  SetWindowText(ThreadSizeWin[threadSizeSelected], buffer.data());
	  StateMap->set(StateFlag::RESTCH);
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
  if (StateMap->testAndReset(StateFlag::POLIMOV)) {
	thred::savdo();
	form::setfrm();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FORMIN)) {
	auto windowRect = RECT {0L, 0L, 0L, 0L};
	GetWindowRect(MsgWindow, &windowRect);
	if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top &&
	    Msg.pt.y <= windowRect.bottom) {
	  auto const iFillType = (Msg.pt.y - windowRect.top - 1) / (ButtonHeight - 4);
	  if (StateMap->testAndReset(StateFlag::FENDIN)) {
		if (iFillType == 3) {
		  UserFlagMap->reset(UserFlag::SQRFIL);
		}
		if (iFillType == 4) {
		  UserFlagMap->set(UserFlag::SQRFIL);
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
  if (StateMap->testAndReset(StateFlag::SIDACT)) {
	return thi::handleSideWindowActive();
  }
  if (FormDataSheet != nullptr) {
	return thi::handleFormDataSheet();
  }
  if (StateMap->test(StateFlag::INSFRM)) {
	thred::savdo();
	form::setins();
	return true;
  }
  if (StateMap->test(StateFlag::FUNCLP)) {
	StateMap->set(StateFlag::INIT);
	form::rstfrm();
	form::refil();
	StateMap->reset(StateFlag::FUNCLP);
	if (StateMap->testAndReset(StateFlag::FPSEL)) {
	  form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	}
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->test(StateFlag::SATCNKT)) {
	thred::savdo();
	satin::satknkt();
	return true;
  }
  if (StateMap->test(StateFlag::SATPNT)) {
	satin::satpnt1();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::SATIN)) {
	satin::satpnt0();
	return true;
  }
  if (StateMap->test(StateFlag::FORMSEL)) {
	if (!StateMap->test(StateFlag::FRMROT) && form::chkfrm(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::MOVFRM)) {
	thred::savdo();
	form::setfrm();
	return true;
  }
  thred::unmsg();
  if (thred::inStitchWin() && !(StateMap->test(StateFlag::SIZSEL) &&
                                thi::chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {
	unpat();
	if (StateMap->testAndReset(StateFlag::ROTAT)) {
	  RotateBoxToCursorLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  auto const adjustedPoint =
	      fPOINT {gsl::narrow<float>(RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x),
	              gsl::narrow<float>(RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y)};
	  if (hypot(adjustedPoint.x, adjustedPoint.y) < FCLOSNUF) {
		StateMap->set(StateFlag::MOVCNTR);
		unrot();
		ritrot(0, thred::pxCor2stch(Msg.pt));
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
		durotu();
		StateMap->set(StateFlag::ROTCAPT);
	  }
	  return true;
	}
	if (StateMap->testAndReset(StateFlag::CLPSHO)) {
	  thred::savdo();
	  if ((!StitchBuffer->empty()) &&
	      (StateMap->testAndReset(StateFlag::SELBOX) || StateMap->testAndReset(StateFlag::INSRT)) &&
	      ClosestPointIndex != wrap::toUnsigned(StitchBuffer->size() - 1U)) {
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
	      std::lround(formsRect.left);
	  formControls[1].x = formControls[5].x = std::lround(wrap::midl(formsRect.right, formsRect.left));
	  formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y =
	      std::lround(formsRect.top);
	  formControls[3].y = formControls[7].y = std::lround(wrap::midl(formsRect.top, formsRect.bottom));
	  formControls[4].y = formControls[5].y = formControls[6].y = std::lround(formsRect.bottom);
	  formControls[2].x = formControls[3].x = formControls[4].x = std::lround(formsRect.right);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  return true;
	}
	if (StateMap->testAndReset(StateFlag::BOXZUM)) {
	  StateMap->set(StateFlag::BZUMIN);
	  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	  ZoomBoxOrigin                       = thred::pxCor2stch(Msg.pt);
	  StateMap->set(StateFlag::VCAPT);
	  return true;
	}
	if (!FormList->empty() && !StateMap->test(StateFlag::FRMOF)) {
	  if (!StateMap->test(StateFlag::INSRT)) {
		if (StateMap->testAndReset(StateFlag::FORMSEL)) {
		  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		}
		if (form::closfrm()) {
		  StateMap->set(StateFlag::FRMPMOV);
		  form::frmovlin();
		  return true;
		}
	  }
	}
	if (StateMap->test(StateFlag::INIT)) {
	  unlin();
	  if (StateMap->test(StateFlag::INSRT)) {
		auto const stitchPoint = thred::pxCor2stch(Msg.pt);
		auto       code =
		    (ActiveColor | USMSK | gsl::narrow_cast<decltype(ActiveColor)>(ActiveLayer << LAYSHFT) | NOTFRM) & NKNOTMSK;
		if (StateMap->test(StateFlag::LIN1)) {
		  if (StateMap->test(StateFlag::BAKEND)) {
			xlin1();
			auto const iStitch = wrap::toUnsigned(StitchBuffer->size());
			StitchBuffer->push_back({stitchPoint.x, stitchPoint.y, code});
			thred::duzrat();
			InsertLine[0] = stch2px1(iStitch);
			InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
			return true;
		  }

		  xlin1();
		  StitchBuffer->insert(StitchBuffer->begin(), fPOINTATTR {stitchPoint.x, stitchPoint.y, code});
		  StitchBuffer->front().attribute &= (~KNOTMSK);
		  InsertLine[0] = stch2px1(0);
		  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
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
		++ClosestPointIndex;
		thred::savdo();
		StitchBuffer->insert(wrap::next(StitchBuffer->begin(), ClosestPointIndex),
		                     fPOINTATTR {stitchPoint.x, stitchPoint.y, code});
		xlin();
		InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		InsertLine[0] = stch2px1(ClosestPointIndex);
		InsertLine[2] = stch2px1(ClosestPointIndex + 1U);
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
		displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
		return true;
	  }
	  auto closestPointIndexClone = uint32_t {};
	  if ((!StateMap->test(StateFlag::HIDSTCH)) && closPnt1(closestPointIndexClone)) {
		ClosestPointIndex = closestPointIndexClone;
		unbox();
		unboxs();
		setbak(ThreadSizePixels[StitchBuffer->operator[](ClosestPointIndex).attribute & COLORBTS] + 3);
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
		  StateMap->reset(StateFlag::ISDWN);
		}
		else {
		  StateMap->set(StateFlag::ISDWN);
		  MoveLine0[0] = stch2px1(ClosestPointIndex - 1);
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
		  MoveLine1[1] = stch2px1(ClosestPointIndex + 1U);
		}
		dulin(MoveLine0, MoveLine1);
		SetCapture(ThrEdWindow);
		StateMap->set(StateFlag::CAPT);
		displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
	  }
	}
	else {
	  if (thred::inStitchWin()) {
		thred::savdo();
		InsertLine[0]          = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		InsertLine[1]          = InsertLine[0];
		auto const stitchPoint = thred::pxCor2stch(Msg.pt);
		StitchBuffer->push_back(
		    {stitchPoint.x,
		     stitchPoint.y,
		     USMSK | ActiveColor | gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT) | NOTFRM});
		thred::addColor(0, ActiveColor);
		StateMap->set(StateFlag::LIN1);
		StateMap->set(StateFlag::INSRT);
		SetCapture(ThrEdWindow);
		ClosestPointIndex = 1;
		StateMap->set(StateFlag::INIT);
		StateMap->set(StateFlag::BAKEND);
	  }
	}
	thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	StateMap->set(StateFlag::BOXSLCT);
	StateMap->set(StateFlag::BZUMIN);
	StateMap->set(StateFlag::NOSEL);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	ZoomBoxOrigin                       = thred::pxCor2stch(Msg.pt);
	StateMap->set(StateFlag::VCAPT);
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
	  ActiveColor     = VerticalIndex & COLORBTS;
	  thred::redraw(UserColorWin->operator[](code));
	  thred::redraw(UserColorWin->operator[](ActiveColor));
	  if (StateMap->test(StateFlag::HID)) {
		StateMap->reset(StateFlag::SELBOX);
		StateMap->reset(StateFlag::GRPSEL);
		StateMap->reset(StateFlag::SCROS);
		StateMap->reset(StateFlag::ECROS);
		StateMap->set(StateFlag::RESTCH);
		thred::redraw(ButtonWin->operator[](HHID));
	  }
	  else {
		if (!SelectedFormList->empty()) {
		  thred::savdo();
		  nucols();
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		  return true;
		}
		if (StateMap->test(StateFlag::FORMSEL)) {
		  auto& form = FormList->operator[](ClosestFormToCursor);
		  if ((form.fillType != 0U) || (form.edgeType != 0U) ||
		      ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U)) {
			thred::savdo();
			if (form.fillType != 0U) {
			  wrap::narrow(form.fillColor, ActiveColor);
			  if (form.fillType == FTHF) {
				wrap::narrow(form.fillInfo.feather.color, ActiveColor);
			  }
			}
			if (form.edgeType != 0U) {
			  if (form.edgeType == EDGEAPPL) {
				form.borderColor &= APCOLMSK;
				form.borderColor |= ActiveColor;
			  }
			  else {
				wrap::narrow(form.borderColor, ActiveColor);
			  }
			}
			if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
			  wrap::narrow(form.underlayColor, ActiveColor);
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
			StateMap->set(StateFlag::RESTCH);
		  }
		}
		else {
		  if (StateMap->test(StateFlag::GRPSEL)) {
			thred::savdo();
			for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
			  StitchBuffer->operator[](iStitch).attribute &= NCOLMSK;
			  StitchBuffer->operator[](iStitch).attribute |= ActiveColor;
			}
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		  else {
			if (StateMap->test(StateFlag::COL)) {
			  StateMap->set(StateFlag::RESTCH);
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
	  StateMap->set(StateFlag::RESTCH);
	}
	return true;
  }
  if (thi::chkMsgs(Msg.pt, ThreadSizeWin.front(), ThreadSizeWin.back())) {
	if (Msg.message == WM_LBUTTONDOWN) {
	  static constexpr auto str = std::array<wchar_t const*, 3> {L"30", L"40", L"60"};
	  thred::savdo();
	  threadSizeSelected = VerticalIndex;
	  for (auto iThrSize = 0U; iThrSize < 3; ++iThrSize) {
		auto const idx = gsl::narrow_cast<int32_t>(VerticalIndex + iThrSize);
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		ChangeThreadSizeWin[iThrSize] = CreateWindow(L"STATIC",
		                                             str[iThrSize],
		                                             WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                             ButtonWidthX3,
		                                             ButtonHeight * idx,
		                                             ButtonWidth,
		                                             ButtonHeight,
		                                             ThrEdWindow,
		                                             nullptr,
		                                             ThrEdInstance,
		                                             nullptr);
	  }
	  StateMap->set(StateFlag::SIZSEL);
	}
	return true;
  }
  retflag = false;
  return false;
}

auto thred::internal::doPaste(std::vector<POINT>& stretchBoxLine, bool& retflag) -> bool {
  retflag = true;
  thred::savdo();
  auto const thrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
  ClipMemory           = GetClipboardData(thrEdClip);
  if (ClipMemory != nullptr) {
	ClipPointer = GlobalLock(ClipMemory);
	if (ClipPointer != nullptr) {
	  auto* formVerticesData = convert_ptr<FORMVERTEXCLIP*>(ClipPointer);
	  if (formVerticesData->clipType == CLP_FRMPS) {
		thred::duzrat();
		auto const byteCount =
		    sizeof(*formVerticesData) + (wrap::toSize(formVerticesData->vertexCount) + 1U) *
		                                    sizeof(decltype(FormVertices->back()));
		auto        clipCopyBuffer = std::vector<uint8_t> {};
		auto* const clipP          = convert_ptr<uint8_t*>(ClipPointer);
		auto const  clipSpan       = gsl::span<uint8_t>(clipP, byteCount);
		clipCopyBuffer.insert(clipCopyBuffer.end(), clipSpan.begin(), clipSpan.end());
		GlobalUnlock(ClipMemory);
		CloseClipboard();
		formVerticesData = convert_ptr<FORMVERTEXCLIP*>(clipCopyBuffer.data());
		if (StateMap->test(StateFlag::FRMPSEL)) {
		  // clang-format off
		  auto& form     = FormList->operator[](ClosestFormToCursor);
		  auto  vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
		  // clang-format on
		  InterleaveSequence->clear();
		  InterleaveSequence->reserve(wrap::toSize(formVerticesData->vertexCount) + 3U);
		  auto const closestIt = wrap::next(vertexIt, ClosestVertexToCursor);
		  InterleaveSequence->push_back(*closestIt);
		  auto*      clipData = convert_ptr<fPOINT*>(&formVerticesData[1]);
		  auto const vertSpan = gsl::span<fPOINT>(clipData, formVerticesData->vertexCount);
		  InterleaveSequence->insert(InterleaveSequence->end(), vertSpan.begin(), vertSpan.end());
		  auto const nextVertex = form::nxt(form, ClosestVertexToCursor);
		  auto const nextIt     = wrap::next(vertexIt, nextVertex);
		  InterleaveSequence->push_back(*nextIt);
		  setpclp();
		  StateMap->set(StateFlag::FPUNCLP);
		  StateMap->set(StateFlag::SHOP);
		  dupclp();
		}
		else {
		  FormMoveDelta = fPOINT {};
		  StateMap->set(StateFlag::FUNCLP);
		  FormList->push_back(FRMHED {});
		  ClosestFormToCursor  = wrap::toUnsigned(FormList->size() - 1U);
		  auto& formIter       = FormList->back();
		  formIter.type        = FRMLINE;
		  formIter.vertexCount = formVerticesData->vertexCount + 1U;
		  formIter.vertexIndex = wrap::toUnsigned(FormVertices->size());
		  auto*      vertices  = convert_ptr<fPOINT*>(&formVerticesData[1]);
		  auto const vertSpan  = gsl::span<fPOINT>(vertices, formIter.vertexCount);
		  FormVertices->insert(FormVertices->end(), vertSpan.begin(), vertSpan.end());
		  StateMap->set(StateFlag::INIT);
		  NewFormVertexCount = formIter.vertexCount;
		  form::unfrm();
		  form::setmfrm();
		  StateMap->set(StateFlag::SHOFRM);
		  form::dufrm();
		}
		return true;
	  }
	  // ToDo - Add more information to the clipboard so that memory can be allocated
	  auto* clipFormsHeader = gsl::narrow_cast<FORMSCLIP*>(ClipPointer);
	  if (clipFormsHeader->clipType == CLP_FRMS) {
		auto iForm            = 0U;
		ClipFormsCount        = clipFormsHeader->formCount;
		auto*      forms      = convert_ptr<FRMHED*>(&clipFormsHeader[1]);
		auto const formSpan   = gsl::span<FRMHED>(forms, clipFormsHeader->formCount);
		auto const formOffset = wrap::toUnsigned(FormList->size());
		for (auto& form : formSpan) {
		  FormList->push_back(form);
		  auto& formIter = FormList->back();
		  formIter.attribute = (gsl::narrow_cast<decltype(formIter.attribute)>(formIter.attribute & NFRMLMSK) |
		                        gsl::narrow_cast<decltype(formIter.attribute)>(ActiveLayer << 1U));
		}
		auto* formVertices  = convert_ptr<fPOINT*>(&forms[clipFormsHeader->formCount]);
		auto  currentVertex = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  auto& form        = FormList->operator[](offset);
		  form.vertexIndex  = wrap::toUnsigned(FormVertices->size());
		  auto const currentVertices =
		      gsl::span<fPOINT>(formVertices, wrap::toSize(currentVertex + form.vertexCount));
		  FormVertices->insert(FormVertices->end(),
		                       wrap::next(currentVertices.begin(), currentVertex),
		                       currentVertices.end());
		  currentVertex += form.vertexCount;
		}
		auto* guides       = convert_ptr<SATCON*>(&formVertices[currentVertex]);
		auto  currentGuide = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  auto const offset = formOffset + iForm;
		  auto& form        = FormList->operator[](offset);
		  if (form.type == SAT && (form.satinGuideCount != 0U)) {
			form.satinOrAngle.guide = satin::adsatk(form.satinGuideCount);
			auto guideIt            = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
			  *guideIt = guides[currentGuide++];
			  ++guideIt;
			}
		  }
		}
		auto* clipData    = convert_ptr<fPOINT*>(&guides[currentGuide]);
		auto  currentClip = 0U;
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  // clang-format off
		  auto const offset = formOffset + iForm;
		  auto&      form   = FormList->operator[](offset);
		  // clang-format on
		  if (clip::isclpx(offset)) {
			form.angleOrClipData.clip = thred::adclp(form.lengthOrCount.clipCount);
			auto offsetStart          = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
			for (auto iClip = 0U; iClip < form.lengthOrCount.clipCount; ++iClip) {
			  *offsetStart = clipData[currentClip++];
			  ++offsetStart;
			}
		  }
		  if (clip::iseclpx(offset)) {
			form.borderClipData = thred::adclp(form.clipEntries);
			auto offsetStart    = wrap::next(ClipPoints->begin(), form.borderClipData);
			for (auto iClip = 0U; iClip < form.clipEntries; ++iClip) {
			  *offsetStart = clipData[currentClip++];
			  ++offsetStart;
			}
		  }
		}
		auto* textureSource = convert_ptr<TXPNT*>(&clipData[currentClip]);
		auto  textureCount  = size_t {};
		for (iForm = 0; iForm < ClipFormsCount; ++iForm) {
		  if (texture::istx(formOffset + iForm)) {
			auto& form = FormList->operator[](wrap::toSize(formOffset) + iForm);
			textureCount += form.fillInfo.texture.count;
			form.fillInfo.texture.index +=
			    gsl::narrow<decltype(form.fillInfo.texture.index)>(TexturePointsBuffer->size());
		  }
		}
		auto const textureSpan = gsl::span<TXPNT>(textureSource, textureCount);
		TexturePointsBuffer->insert(TexturePointsBuffer->end(), textureSpan.begin(), textureSpan.end());
		GlobalUnlock(ClipMemory);
		SelectedFormsRect.top = SelectedFormsRect.left = std::numeric_limits<LONG>::max();
		SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
		form::ratsr();
		SelectedFormList->clear();
		SelectedFormList->reserve(ClipFormsCount);
		for (auto index = 0U; index < (ClipFormsCount); ++index) {
		  form::fselrct(formOffset + index);
		  SelectedFormList->push_back(formOffset + index);
		}
		wrap::narrow_cast(SelectedFormsSize.x, SelectedFormsRect.right - SelectedFormsRect.left);
		wrap::narrow_cast(SelectedFormsSize.y, SelectedFormsRect.bottom - SelectedFormsRect.top);
		StateMap->set(StateFlag::INIT);
		auto& formLines = *FormLines;
		formLines.resize(SQPNTS);
		formLines[0].x = formLines[3].x = formLines[4].x = SelectedFormsRect.left;
		formLines[1].x = formLines[2].x = SelectedFormsRect.right;
		formLines[0].y = formLines[1].y = formLines[4].y = SelectedFormsRect.top;
		formLines[2].y = formLines[3].y = SelectedFormsRect.bottom;
		StateMap->set(StateFlag::SHOSTRTCH);
		thred::strtchbox(stretchBoxLine);
		FormMoveDelta =
		    fPOINT {gsl::narrow<float>((SelectedFormsRect.right - SelectedFormsRect.left) / 2),
		            gsl::narrow<float>((SelectedFormsRect.bottom - SelectedFormsRect.top) / 2)};
		StateMap->set(StateFlag::MOVFRMS);
		StateMap->set(StateFlag::FUNSCLP);
	  }
	  else {
		auto* clipFormHeader = gsl::narrow_cast<FORMCLIP*>(ClipPointer);
		if (clipFormHeader->clipType == CLP_FRM) {
		  FormMoveDelta = fPOINT {};
		  StateMap->set(StateFlag::FUNCLP);
		  FormList->push_back(clipFormHeader->form);
		  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
		  auto& formIter      = FormList->back();
		  formIter.attribute = gsl::narrow_cast<decltype(formIter.attribute)>(formIter.attribute & NFRMLMSK) |
		                       gsl::narrow_cast<decltype(formIter.attribute)>(ActiveLayer << 1U);
		  formIter.vertexIndex    = wrap::toUnsigned(FormVertices->size());
		  auto*      formVertices = convert_ptr<fPOINT*>(&clipFormHeader[1]);
		  auto const verticesSpan = gsl::span<fPOINT>(formVertices, formIter.vertexCount);
		  FormVertices->insert(FormVertices->end(), verticesSpan.begin(), verticesSpan.end());
		  auto* guides = convert_ptr<SATCON*>(&formVertices[formIter.vertexCount]);
		  if (formIter.type == SAT && (formIter.satinGuideCount != 0U)) {
			auto const guideSpan        = gsl::span<SATCON>(guides, formIter.satinGuideCount);
			formIter.satinOrAngle.guide = wrap::toUnsigned(SatinGuides->size());
			SatinGuides->insert(SatinGuides->end(), guideSpan.begin(), guideSpan.end());
		  }
		  auto*      clipData      = convert_ptr<fPOINT*>(&guides[formIter.satinGuideCount]);
		  auto       clipCount     = 0U;
		  auto const lastFormIndex = wrap::toUnsigned(FormList->size() - 1U);
		  if (clip::isclpx(lastFormIndex)) {
			auto const clipSpan = gsl::span<fPOINT>(clipData, formIter.lengthOrCount.clipCount);
			formIter.angleOrClipData.clip = wrap::toUnsigned(ClipPoints->size());
			ClipPoints->insert(ClipPoints->end(), clipSpan.begin(), clipSpan.end());
			clipCount += formIter.lengthOrCount.clipCount;
		  }
		  if (clip::iseclpx(lastFormIndex)) {
			clipData                = convert_ptr<fPOINT*>(&clipData[clipCount]);
			auto const clipSpan     = gsl::span<fPOINT>(clipData, formIter.clipEntries);
			formIter.borderClipData = wrap::toUnsigned(ClipPoints->size());
			ClipPoints->insert(ClipPoints->end(), clipSpan.begin(), clipSpan.end());
			clipCount += formIter.clipEntries;
		  }
		  if (texture::istx(lastFormIndex)) {
			auto*      textureSource = convert_ptr<TXPNT*>(&clipData[clipCount]);
			auto const textureSpan = gsl::span<TXPNT>(textureSource, formIter.fillInfo.texture.count);
			wrap::narrow(formIter.fillInfo.texture.index, TexturePointsBuffer->size());
			TexturePointsBuffer->insert(TexturePointsBuffer->end(), textureSpan.begin(), textureSpan.end());
		  }
		  NewFormVertexCount = formIter.vertexCount;
		  if (formIter.type != FRMLINE) {
			++NewFormVertexCount;
		  }
		  StateMap->set(StateFlag::INIT);
		  form::unfrm();
		  thred::duzrat();
		  form::setmfrm();
		  StateMap->set(StateFlag::SHOFRM);
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
	  StateMap->set(StateFlag::CLPSHO);
	}
	CloseClipboard();
  }
  retflag = false;
  return {};
}

auto thred::internal::handleHomeKey(bool& retflag) -> bool {
  retflag = true;
  if ((wrap::pressed(VK_SHIFT)) && (wrap::pressed(VK_CONTROL))) {
	if (StateMap->testAndReset(StateFlag::SELBOX)) {
	  GroupStitchIndex = 0;
	  StateMap->set(StateFlag::GRPSEL);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		if (GroupStitchIndex > ClosestPointIndex) {
		  ClosestPointIndex = 0;
		}
		else {
		  GroupStitchIndex = 0;
		}
	  }
	  else {
		StateMap->set(StateFlag::SELBOX);
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	thred::grpAdj();
	thred::redraw(ColorBar);
	return true;
  }
  if (wrap::pressed(VK_SHIFT)) {
	auto iColor = (StateMap->testAndReset(StateFlag::SELBOX)) ? pt2colInd(ClosestPointIndex)
	                                                          : pt2colInd(GroupStitchIndex);
	if (StateMap->testAndReset(StateFlag::SELBOX)) {
	  StateMap->set(StateFlag::GRPSEL);
	}
	if (iColor != 0U) {
	  --iColor;
	}
	GroupStitchIndex = ColorChangeTable->operator[](iColor).stitchIndex;
	thred::grpAdj();
	thred::redraw(ColorBar);
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  auto const stitchCoordsInPixels = stch2px1(0);
	  endpnt(stitchCoordsInPixels);
	  StateMap->reset(StateFlag::BAKEND);
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::SELBOX)) {
		auto iColor = pt2colInd(ClosestPointIndex);
		if (iColor != 0U) {
		  --iColor;
		}
		ClosestPointIndex = ColorChangeTable->operator[](iColor).stitchIndex;
	  }
	  else {
		if (StateMap->test(StateFlag::LENSRCH)) {
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
	  if (StateMap->testAndReset(StateFlag::SELBOX)) {
		GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
		StateMap->set(StateFlag::GRPSEL);
	  }
	  else {
		if (StateMap->test(StateFlag::GRPSEL)) {
		  if (GroupStitchIndex > ClosestPointIndex) {
			GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
		  }
		  else {
			ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
		  }
		}
		else {
		  StateMap->set(StateFlag::SELBOX);
		  ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
		  StateMap->set(StateFlag::RESTCH);
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
	auto const iColor = (StateMap->testAndReset(StateFlag::SELBOX)) ? pt2colInd(ClosestPointIndex)
	                                                                : pt2colInd(GroupStitchIndex);
	if (StateMap->testAndReset(StateFlag::SELBOX)) {
	  StateMap->set(StateFlag::GRPSEL);
	}
	GroupStitchIndex = ColorChangeTable->operator[](iColor).stitchIndex - 1U;
	thred::grpAdj();
	thred::redraw(ColorBar);
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  auto stitchCoordsInPixels = POINT {0L, StitchWindowClientRect.bottom};
	  if (!StitchBuffer->empty()) {
		stitchCoordsInPixels = stch2px1(wrap::toUnsigned(StitchBuffer->size() - 1U));
	  }
	  endpnt(stitchCoordsInPixels);
	  StateMap->set(StateFlag::BAKEND);
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::SELBOX)) {
		auto const                            iColor = pt2colInd(ClosestPointIndex);
		ClosestPointIndex = ColorChangeTable->operator[](iColor).stitchIndex - 1U;
	  }
	  else {
		if (StateMap->test(StateFlag::LENSRCH)) {
		  setsrch(LargestStitchIndex);
		  return true;
		}
		ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
	  }
	  movbox();
	}
  }
  retflag = 0;
  return false;
}

auto thred::internal::handleRightKey(bool& retflag) -> bool {
  retflag = true;
  if (wrap::pressed(VK_SHIFT)) {
	if (StateMap->test(StateFlag::FPSEL)) {
	  auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
	  if (StateMap->test(StateFlag::PSELDIR)) {
		++SelectedFormVertices.vertexCount %= vertexCount;
		SelectedFormVertices.finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % vertexCount;
	  }
	  else {
		if (SelectedFormVertices.vertexCount != 0U) {
		  --(SelectedFormVertices.vertexCount);
		  SelectedFormVertices.finish =
		      (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
		}
		else {
		  SelectedFormVertices.vertexCount = 1;
		  StateMap->set(StateFlag::PSELDIR);
		  SelectedFormVertices.finish = (SelectedFormVertices.start + 1U) % vertexCount;
		}
	  }
	  thred::setpsel();
	}
	else {
	  if (StateMap->testAndReset(StateFlag::FRMPSEL)) {
		auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
		form::unpsel();
		SelectedFormVertices.start       = ClosestVertexToCursor;
		SelectedFormVertices.form        = ClosestFormToCursor;
		SelectedFormVertices.vertexCount = 1;
		SelectedFormVertices.finish      = (SelectedFormVertices.start + 1U) % vertexCount;
		StateMap->set(StateFlag::PSELDIR);
		thred::setpsel();
	  }
	  else {
		StateMap->reset(StateFlag::LENSRCH);
		StateMap->reset(StateFlag::FORMSEL);
		if (StateMap->testAndReset(StateFlag::SELBOX)) {
		  if (!StitchBuffer->empty()) {
			auto const lastStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
			if (ClosestPointIndex < lastStitch) {
			  StateMap->set(StateFlag::GRPSEL);
			  GroupStitchIndex = ClosestPointIndex + 1U;
			}
		  }
		}
		else {
		  if (!StitchBuffer->empty()) {
			auto const lastStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
			if (GroupStitchIndex < lastStitch) {
			  ++GroupStitchIndex;
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
	  if (StateMap->test(StateFlag::LENSRCH)) {
		longer();
		StateMap->set(StateFlag::RESTCH);
	  }
	  else {
		if (StateMap->test(StateFlag::FRMPSEL)) {
		  ClosestVertexToCursor = form::nxt(FormList->operator[](ClosestFormToCursor), ClosestVertexToCursor);
		  displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
		  auto const vertexIt =
		      wrap::next(FormVertices->cbegin(),
		                 FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
		  thred::ritfcor(*vertexIt);
		  shftflt(*vertexIt);
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::SELBOX)) {
			if (ClosestPointIndex < wrap::toUnsigned(StitchBuffer->size() - 1U)) {
			  ++ClosestPointIndex;
			}
			movbox();
			return true;
		  }
		  if (StateMap->test(StateFlag::GRPSEL)) {
			if (GroupStitchIndex < wrap::toUnsigned(StitchBuffer->size() - 1U)) {
			  ++GroupStitchIndex;
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
	if (StateMap->test(StateFlag::FPSEL)) {
	  auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
	  if (!StateMap->test(StateFlag::PSELDIR)) {
		++SelectedFormVertices.vertexCount %= vertexCount;
		SelectedFormVertices.finish =
		    (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
	  }
	  else {
		if (SelectedFormVertices.vertexCount != 0U) {
		  --(SelectedFormVertices.vertexCount);
		  SelectedFormVertices.finish =
		      (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
		}
		else {
		  SelectedFormVertices.vertexCount = 1;
		  SelectedFormVertices.finish = (SelectedFormVertices.start + vertexCount - 1) % vertexCount;
		  StateMap->reset(StateFlag::PSELDIR);
		}
	  }
	  thred::setpsel();
	}
	else {
	  if (StateMap->testAndReset(StateFlag::FRMPSEL)) {
		form::unpsel();
		SelectedFormVertices.start       = ClosestVertexToCursor;
		SelectedFormVertices.vertexCount = 1;
		StateMap->reset(StateFlag::PSELDIR);
		thred::setpsel();
	  }
	  else {
		StateMap->reset(StateFlag::LENSRCH);
		StateMap->reset(StateFlag::FORMSEL);
		if (StateMap->testAndReset(StateFlag::SELBOX)) {
		  StateMap->set(StateFlag::GRPSEL);
		  GroupStitchIndex = ClosestPointIndex - 1U;
		}
		else if (GroupStitchIndex != 0U) {
		  --GroupStitchIndex;
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
	  if (StateMap->test(StateFlag::LENSRCH)) {
		shorter();
		StateMap->set(StateFlag::RESTCH);
	  }
	  else {
		if (StateMap->test(StateFlag::FRMPSEL)) {
		  auto const& form      = FormList->operator[](ClosestFormToCursor);
		  ClosestVertexToCursor = form::prv(form, ClosestVertexToCursor);
		  displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
		  auto const vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex + ClosestVertexToCursor);
		  thred::ritfcor(*vertexIt);
		  shftflt(*vertexIt);
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::SELBOX)) {
			if (ClosestPointIndex != 0U) {
			  --ClosestPointIndex;
			}
			movbox();
			return true;
		  }
		  if (StateMap->test(StateFlag::GRPSEL)) {
			if (GroupStitchIndex != 0U) {
			  --GroupStitchIndex;
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

auto thred::internal::handleMainWinKeys(wchar_t const&      code,
                                        fPOINT&             rotationCenter,
                                        std::vector<POINT>& stretchBoxLine,
                                        bool&               retflag) -> bool {
  retflag = true;
  switch (code) {
	case VK_ESCAPE: {
	  esccode();
	  [[fallthrough]];
	}
	case L'Q': {
	  if (wrap::pressed(VK_SHIFT)) {
		ritcur();
		return true;
	  }
	  qcode();
	  return true;
	}
	case VK_OEM_3: {                                             // '`~' for US
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_MENU)) { // CTRL + ALT
		BackgroundColor    = 0x505050;
		BackgroundPen      = nuPen(BackgroundPen, 1, BackgroundColor);
		BackgroundPenWidth = 1;
		DeleteObject(BackgroundBrush);
		BackgroundBrush = CreateSolidBrush(BackgroundColor);
		if (bitmap::ismap()) {
		  bitmap::internal::bfil(BackgroundColor);
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	  else {
#if PESACT
		//				IniFile.auxFileType=AUXPES;
		//				strcpy_s(WorkingFileName,"u:\\mrd\\t.thr");
		//				StateMap->set(StateFlag::REDOLD);
		//				nuFil();
		//				lodpes();
		//				savpes();
#endif
		xt::tst();
	  }
	  break;
	}
	case VK_PRIOR: { // page up
	  if (wrap::pressed(VK_SHIFT)) {
		form::redup();
	  }
	  else {
		form::rotagain();
	  }
	  break;
	}
	case VK_NEXT: { // page down
	  if (wrap::pressed(VK_SHIFT)) {
		form::bakdup();
	  }
	  else {
		form::bakagain();
	  }
	  break;
	}
	case VK_SUBTRACT: { // keypad -
	  toglup();
	  break;
	}
	case VK_OEM_MINUS: { //-
	  StateMap->reset(StateFlag::CNTRH);
	  StateMap->reset(StateFlag::CNTRV);
	  if (wrap::pressed(VK_SHIFT)) {
		StateMap->set(StateFlag::CNTRV);
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  StateMap->set(StateFlag::CNTRH);
		}
	  }
	  form::cntrx();
	  break;
	}
	case VK_OEM_1: { // ';:' for US
	  movmrk();
	  break;
	}
	case VK_OEM_PLUS: { // '+' any country
	  form::shrnk();
	  break;
	}
	case VK_OEM_COMMA: { // ',' any country
	  if (wrap::pressed(VK_SHIFT)) {
		xt::setfilstrt();
	  }
	  else {
		thred::savdo();
		form::join();
	  }
	  break;
	}
	case VK_OEM_PERIOD: { // '.' any country
	  if (wrap::pressed(VK_SHIFT)) {
		xt::setfilend();
	  }
	  else {
		setmov();
	  }
	  break;
	}
	case VK_OEM_2: { // '/?' for US
	  formForms::frmnum();
	  break;
	}
	case VK_OEM_4: { //  '[{' for US
	  if (wrap::pressed(VK_SHIFT)) {
		selfrm0();
	  }
	  else {
		ungrplo();
	  }
	  break;
	}
	case VK_OEM_6: { //  ']}' for US
	  if (wrap::pressed(VK_SHIFT)) {
		selfrmx();
	  }
	  else {
		ungrphi();
	  }
	  break;
	}
	case VK_OEM_7: { //  ''"' for US
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
	  if (StateMap->testAndFlip(StateFlag::INSRT)) {
		ReleaseCapture();
		StateMap->set(StateFlag::RESTCH);
	  }
	  else {
		istch();
	  }
	  unbox();
	  if (StateMap->testAndReset(StateFlag::GRPSEL) || StateMap->testAndReset(StateFlag::FORMSEL)) {
		StateMap->set(StateFlag::RESTCH);
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
	case L'A': {
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
	case L'B': {
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
	case L'C': {
	  if (wrap::pressed(VK_CONTROL)) {
		duclip();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  StateMap->reset(StateFlag::CNV2FTH);
		  satin::ribon();
		}
		else {
		  StateMap->reset(StateFlag::HIDSTCH);
		  StateMap->set(StateFlag::IGNTHR);
		  rebox();
		  StateMap->reset(StateFlag::IGNTHR);
		  StateMap->set(StateFlag::RESTCH);
		}
	  }
	  break;
	}
	case L'D': {
	  if (wrap::pressed(VK_SHIFT)) {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
		  keybd_event('F', 0, 0, 0);
		}
	  }
	  else {
		satin::satsel();
	  }
	  break;
	}
	case L'E': {
	  if (wrap::pressed(VK_SHIFT)) {
		texture::dutxtfil();
	  }
	  else {
		form::infrm();
	  }
	  break;
	}
	case L'F': {
	  if (!SelectedFormList->empty()) {
		PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
		keybd_event('F', 0, 0, 0);
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  StateMap->set(StateFlag::CNV2FTH);
		  satin::ribon();
		}
		else {
		  form::frmon();
		  if (StateMap->test(StateFlag::FORMSEL)) {
			formForms::refrm();
		  }
		  else {
			form::form();
		  }
		}
	  }
	  break;
	}
	case L'G': {
	  thi::mark();
	  break;
	}
	case L'H': {
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
	case L'J': {
	  if (wrap::pressed(VK_SHIFT)) {
		pgrit();
	  }
	  else {
		form::refilal();
	  }
	  break;
	}
	case L'K': {
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_MENU)) { // CTRL + ALT
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
	case L'L': {
	  if (wrap::pressed(VK_SHIFT)) {
		delstch();
	  }
	  else {
		form::fcntr();
	  }
	  break;
	}
	case L'M': {
	  if (wrap::pressed(VK_CONTROL)) {
		pntmrk();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  dumrk(wrap::toFloat(UnzoomedRect.x) * 0.5F, wrap::toFloat(UnzoomedRect.y) * 0.5F);
		}
		else {
		  if (thred::inStitchWin()) {
			auto const stitchPoint = thred::pxCor2stch(Msg.pt);
			dumrk(stitchPoint.x, stitchPoint.y);
		  }
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	  break;
	}
	case L'N': {
	  if (wrap::pressed(VK_CONTROL)) {
		xt::nudsiz();
	  }
	  else {
		if (wrap::pressed(VK_SHIFT)) {
		  pgdwn();
		}
		else {
		  StateMap->set(StateFlag::TRCUP);
		  trace::trace();
		}
	  }
	  break;
	}
	case L'O': {
	  fop();
	  break;
	}
	case L'P': {
	  formForms::prfmsg();
	  break;
	}
	case L'R': {
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_SHIFT)) {
		thred::movStch();
		unbox();
		StateMap->set(StateFlag::RESTCH);
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
	case L'S': {
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
	case L'T': {
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
	case L'U': {
	  if (wrap::pressed(VK_SHIFT)) {
		pgup();
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  trace::trdif();
		}
		else {
		  StateMap->reset(StateFlag::TRCUP);
		  trace::trace();
		}
	  }
	  break;
	}
	case L'V': {
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
	case L'W': {
	  if (wrap::pressed(VK_SHIFT)) {
		form::crop();
	  }
	  else {
		form::insat();
	  }
	  break;
	}
	case L'X': {
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
	case L'Y': {
	  if (form::closfrm()) {
		selfpnt();
	  }
	  break;
	}
	case L'Z': {
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
	  outDebugString(L"default hit in handleMainWinKeys: code [{}]\n", code);
	  break;
	}
  }
  retflag = false;
  return {};
}

auto thred::internal::handleNumericInput(wchar_t const& code, bool& retflag) -> bool {
  retflag = true;
  if (StateMap->test(StateFlag::SCLPSPAC) && code == VK_OEM_MINUS && (MsgIndex == 0U)) {
	MsgBuffer.fill(0);
	MsgBuffer[0] = '-';
	MsgIndex     = 1;
	SetWindowText(GeneralNumberInputBox, MsgBuffer.data());
	return true;
  }
  if (dunum(code)) {
	if (StateMap->test(StateFlag::TRNIN0)) {
	  trace::traceNumberInput(NumericCode);
	}
	else {
	  MsgBuffer[MsgIndex++] = NumericCode;
	  MsgBuffer[MsgIndex]   = 0;
	  SetWindowText(GeneralNumberInputBox, MsgBuffer.data());
	}
	return true;
  }
  switch (code) {
	case VK_DECIMAL:      // numpad period
	case VK_OEM_PERIOD: { // period
	  MsgBuffer[MsgIndex++] = '.';
	  MsgBuffer[MsgIndex]   = 0;
	  SetWindowText(GeneralNumberInputBox, MsgBuffer.data());
	  return true;
	}
	case VK_BACK: { // backspace
	  if (MsgIndex != 0U) {
		--MsgIndex;
		if (StateMap->test(StateFlag::TRNIN0)) {
		  trace::traceNumberReset();
		}
		else {
		  MsgBuffer[MsgIndex] = 0;
		  SetWindowText(GeneralNumberInputBox, MsgBuffer.data());
		}
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

auto thred::internal::handleEditMenu(WORD const& wParameter) -> bool {
  constexpr auto TOLAYER0 = 0U;
  constexpr auto TOLAYER1 = 1U;
  constexpr auto TOLAYER2 = 2U;
  constexpr auto TOLAYER3 = 3U;
  constexpr auto TOLAYER4 = 4U;
  auto           flag     = false;
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
	  StateMap->set(StateFlag::CNV2FTH);
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
	  StateMap->reset(StateFlag::CNTRH);
	  StateMap->reset(StateFlag::CNTRV);
	  form::cntrx();
	  flag = true;
	  break;
	}
	case ID_CNTRH: { // edit / Center / Horizontal
	  StateMap->reset(StateFlag::CNTRH);
	  StateMap->set(StateFlag::CNTRV);
	  form::cntrx();
	  flag = true;
	  break;
	}
	case ID_CNTRV: { // edit / Center / Vertical
	  StateMap->set(StateFlag::CNTRH);
	  StateMap->reset(StateFlag::CNTRV);
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
	  dumrk(wrap::toFloat(UnzoomedRect.x) * 0.5F, wrap::toFloat(UnzoomedRect.y) * 0.5F);
	  StateMap->set(StateFlag::RESTCH);
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
	  StateMap->reset(StateFlag::CNV2FTH);
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
	  auto stitchRange = fPOINT {};
	  thred::chkrng(stitchRange);
	  StateMap->set(StateFlag::RESTCH);
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
	  form::movlayr(TOLAYER0);
	  flag = true;
	  break;
	}
	case ID_LAYMOV1: { // edit / Move to Layer / 1
	  form::movlayr(TOLAYER1);
	  flag = true;
	  break;
	}
	case ID_LAYMOV2: { // edit / Move to Layer / 2
	  form::movlayr(TOLAYER2);
	  flag = true;
	  break;
	}
	case ID_LAYMOV3: { // edit / Move to Layer / 3
	  form::movlayr(TOLAYER3);
	  break;
	}
	case ID_LAYMOV4: { // edit / Move to Layer / 4
	  form::movlayr(TOLAYER4);
	  flag = true;
	  break;
	}
	case ID_LAYCPY0: { // edit / Copy to Layer / 0
	  form::cpylayr(TOLAYER0);
	  flag = true;
	  break;
	}
	case ID_LAYCPY1: { // edit / Copy to Layer / 1
	  form::cpylayr(TOLAYER1);
	  flag = true;
	  break;
	}
	case ID_LAYCPY2: { // edit / Copy to Layer / 2
	  form::cpylayr(TOLAYER2);
	  flag = true;
	  break;
	}
	case ID_LAYCPY3: { // edit / Copy to Layer / 3
	  form::cpylayr(TOLAYER3);
	  flag = true;
	  break;
	}
	case ID_LAYCPY4: { // edit / Copy to Layer / 4
	  form::cpylayr(TOLAYER4);
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
	  StateMap->set(StateFlag::RESTCH);
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
	  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
		UserColor[iColor]      = DefaultColors[iColor];
		UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
		UserPen[iColor]        = nuPen(UserPen[iColor], 1, UserColor[iColor]);
		thred::redraw(UserColorWin->operator[](iColor));
	  }
	  StateMap->set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_DESNAM: { // edit / Set / Designer Name
	  getdes();
	  flag = true;
	  break;
	}
	default: {
	  outDebugString(L"default hit in handleEditMenu: wParameter [{}]\n", wParameter);
	  break;
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
	  UserFlagMap->set(UserFlag::MARQ);
	  qchk();
	  flag = true;
	  break;
	}
	case ID_MARKQ: { // view / Set / Retrieve Mark / Q
	  UserFlagMap->reset(UserFlag::MARQ);
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
	  UserFlagMap->reset(UserFlag::LINSPAC);
	  linbmen();
	  flag = true;
	  break;
	}
	case ID_LINBEVEN: { // view / Set / Line Border Spacing / Even
	  UserFlagMap->set(UserFlag::LINSPAC);
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
	  StateMap->set(StateFlag::ENTR30);
	  flag = true;
	  break;
	}
	case ID_SIZ40: { // view / Thread Size / 40
	  displayText::tsizmsg(L"40", ThreadSize40);
	  StateMap->set(StateFlag::ENTR40);
	  flag = true;
	  break;
	}
	case ID_SIZ60: { // view / Thread Size / 60
	  displayText::tsizmsg(L"60", ThreadSize60);
	  StateMap->set(StateFlag::ENTR60);
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
		if (bitmap::ismap()) {
		  bitmap::internal::bfil(BackgroundColor);
		}
		thred::zumhom();
	  }
	  flag = true;
	  break;
	}
	case ID_BITCOL: { // view / Set / Bitmap Color
	  bitmap::setBmpColor();
	  flag = true;
	  break;
	}
	case VU_ZUMFUL: { // view / Zoom Full
	  thred::zumhom();
	  flag = true;
	  break;
	}
	default: {
	  outDebugString(L"default hit in handleViewMenu: wParameter [{}]\n", wParameter);
	  break;
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
	  bitmap::delmap();
	  flag = true;
	  break;
	}
	case ID_SAVMAP: { // file / Save Bitmap
	  bitmap::savmap();
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
	  auto fileName = fs::path {};
	  insfil(fileName);
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
		  nuFil(fileIndices::DST);
		  break;
		}
#if PESACT
		case AUXPES: {
		  nuFil(fileIndices::PES);
		  break;
		}
#endif
		default: {
		  nuFil(fileIndices::PCS);
		}
	  }
	  nulayr(0U);
	  flag = true;
	  break;
	}
	case ID_HIDBITF: { // file / Hide Bitmap
	  thred::hidbit();
	  flag = true;
	  break;
	}
	case ID_LODBIT: { // file / Load Bitmap
	  bitmap::lodbmp(DefaultDirectory);
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
	  if (savcmp()) {
		newFil();
		nulayr(0U);
	  }
	  else {
		displayText::savdisc();
		StateMap->set(StateFlag::NEWBAK);
		StateMap->reset(StateFlag::PRFACT);
		thred::undat();
	  }
	  flag = true;
	  break;
	}
	default: {
	  outDebugString(L"default hit in handleFileMenu: wParameter [{}]\n", wParameter);
	  break;
	}
  }
  return flag;
}

auto thred::internal::handleFillMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_BOLD: { // fill / Border / Bean
	  if (StateMap->test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
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
	  StateMap->set(StateFlag::LINCHN);
	  form::chain();
	  flag = true;
	  break;
	}
	case ID_OPNCHN: { // fill / Border / Open chain
	  StateMap->reset(StateFlag::LINCHN);
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
	  if (StateMap->test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::horclp();
	  flag = true;
	  break;
	}
	case ID_ANGCLP: { // fill / Clipboard / Angle
	  if (StateMap->test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::angclp();
	  flag = true;
	  break;
	}
	case ID_VRTCLP: { // fill / Clipboard / Vertical
	  if (StateMap->test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
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
	  if (StateMap->test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::prpbrd(LineSpacing);
	  flag = true;
	  break;
	}
	case ID_FILANG: { // fill / Angle
	  if (StateMap->test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::filangl();
	  flag = true;
	  break;
	}
	case ID_APLIQ: { // fill / Border / Applique
	  if (StateMap->test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::apliq();
	  flag = true;
	  break;
	}
	case ID_SATBRD: { // fill / Border / Angle Satin
	  if (StateMap->test(StateFlag::FORMSEL)) {
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
	  if (StateMap->test(StateFlag::FORMSEL)) {
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
	  StateMap->set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_FILSAT: { // fill / Fan
	  StateMap->reset(StateFlag::FTHR);
	  form::filsat();
	  flag = true;
	  break;
	}
	case ID_FILL_VERT: { // fill / Vertical
	  if (StateMap->test(StateFlag::FORMSEL) || !SelectedFormList->empty()) {
		thred::savdo();
	  }
	  form::filvrt();
	  flag = true;
	  break;
	}
	case ID_FILL_HOR: { // fill / Horizontal
	  if (StateMap->test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::filhor();
	  flag = true;
	  break;
	}
	default: {
	  outDebugString(L"default hit in handleFillMenu: wParameter [{}]\n", wParameter);
	  break;
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
	  nulayr(0U);
	  flag = true;
	  break;
	}
	case ID_L1: { // 1 (Show layer 1 only)
	  nulayr(1U);
	  flag = true;
	  break;
	}
	case ID_L2: { // 2 (Show layer 2 only)
	  nulayr(2U);
	  flag = true;
	  break;
	}
	case ID_L3: { // 3 (Show layer 3 only)
	  nulayr(3U);
	  flag = true;
	  break;
	}
	case ID_L4: { // 4 (Show layer 4 only)
	  nulayr(4U);
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
	  auto stitchCoordsInPixels = POINT {0L, StitchWindowClientRect.bottom};
	  if (!StitchBuffer->empty()) {
		stitchCoordsInPixels = stch2px1(wrap::toUnsigned(StitchBuffer->size()) - 1U);
	  }
	  endpnt(stitchCoordsInPixels);
	  StateMap->set(StateFlag::BAKEND);
	  StateMap->set(StateFlag::RESTCH);
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
	  if (StateMap->test(StateFlag::FORMSEL)) {
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
	  if (StateMap->test(StateFlag::GMRK) || StateMap->test(StateFlag::SELBOX) ||
	      StateMap->test(StateFlag::INSRT) || StateMap->test(StateFlag::GRPSEL) ||
	      StateMap->test(StateFlag::FORMSEL)) {
		zumin();
	  }
	  else {
		StateMap->reset(StateFlag::BZUM);
		StateMap->set(StateFlag::BOXZUM);
		StateMap->reset(StateFlag::BZUMIN);
		StateMap->set(StateFlag::VCAPT);
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
	  outDebugString(L"default hit in handleMainMenu: wParameter [{}]\n", wParameter);
	  break;
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
		  StateMap->set(StateFlag::RESTCH);
		  thred::unmsg();
		  return true;
		}
	  }
	  if (StateMap->testAndReset(StateFlag::MOVMSG)) {
		if (code == VK_RETURN || code == VK_OEM_3) {
		  thred::savdo();
		  form::refilfn();
		  thred::unmsg();
		}
		else {
		  thred::unmsg();
		}
		if (StateMap->testAndReset(StateFlag::WASFRMFRM)) {
		  formForms::refrm();
		}
		return true;
	  }
	  if (StateMap->testAndReset(StateFlag::PRGMSG)) {
		if (code == VK_RETURN || code == VK_OEM_3) {
		  deldir();
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
		  delsfrms(code);
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
			esccode();
			qcode();
			break;
		  }
		  case VK_DOWN:
		  case VK_NEXT: { // page down
			nuthsel();
			break;
		  }
		  case VK_UP:
		  case VK_PRIOR: { // page up
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
		  MsgBuffer.fill(0);
		  MsgBuffer[0] = '-';
		  MsgIndex     = 1;
		  SetWindowText(GeneralNumberInputBox, MsgBuffer.data());
		  return true;
		}
	  }
	  if ((FormMenuChoice != 0U) || (PreferenceIndex != 0U)) {
		if (chkminus(code)) {
		  MsgIndex                      = 1;
		  SideWindowEntryBuffer.front() = '-';
		  SetWindowText(SideMessageWindow, SideWindowEntryBuffer.data());
		  return true;
		}
		if (dunum(code)) {
		  if (PreferenceIndex == PSHO + 1 || PreferenceIndex == PBOX + 1U) {
			auto buffer = std::array<wchar_t, 2> {};
			buffer[0]   = NumericCode;
			if (PreferenceIndex == PSHO + 1U) {
			  ShowStitchThreshold = unthrsh(NumericCode - L'0');
			  SetWindowText(ValueWindow->operator[](PSHO), buffer.data());
			}
			else {
			  StitchBoxesThreshold = unthrsh(NumericCode - L'0');
			  SetWindowText(ValueWindow->operator[](PBOX), buffer.data());
			}
			thred::unsid();
		  }
		  else {
			if (MsgIndex < (SideWindowEntryBuffer.size() - 1U)) {
			  SideWindowEntryBuffer[MsgIndex++] = NumericCode;
			  SideWindowEntryBuffer[MsgIndex]   = 0;
			  SetWindowText(SideMessageWindow, SideWindowEntryBuffer.data());
			}
		  }
		  return true;
		}
		switch (code) {
		  case VK_DECIMAL:      // numpad period
		  case VK_OEM_PERIOD: { // period
			SideWindowEntryBuffer[MsgIndex++] = '.';
			SideWindowEntryBuffer[MsgIndex]   = 0;
			SetWindowText(SideMessageWindow, SideWindowEntryBuffer.data());
			return true;
		  }
		  case VK_BACK: { // backspace
			if (MsgIndex != 0U) {
			  SideWindowEntryBuffer[--MsgIndex] = 0;
			  SetWindowText(SideMessageWindow, SideWindowEntryBuffer.data());
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
		thi::movi();
		LastKeyCode = L'I';
		return true;
	  }
	  if (code == L'Q' && LastKeyCode == L'Q') {
		unpat();
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
		if (std::wcslen(MsgBuffer.data()) != 0) {
		  auto const value = wrap::wcstof(MsgBuffer.data());
		  if (value != 0.0F) {
			IniFile.rotationAngle = value * DEGRADF;
		  }
		}
		form::duprot(IniFile.rotationAngle);
	  }
	  if (StateMap->testAndReset(StateFlag::ENTROT)) {
		if (std::wcslen(MsgBuffer.data()) != 0) {
		  auto const value = wrap::wcstof(MsgBuffer.data());
		  if (value != 0.0F) {
			IniFile.rotationAngle = value * DEGRADF;
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
		auto previousName = PreviousNames->begin();
		for (auto const iLRU : LRUMenuId) {
		  if (Msg.wParam == iLRU) {
			*WorkingFileName = *previousName;
			StateMap->set(StateFlag::REDOLD);
			nuFil(fileIndices::THR);
		  }
		  ++previousName;
		}
	  }
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  auto const wParameter = LOWORD(Msg.wParam);
	  if (!(wParameter < ID_FILE_OPEN1 || wParameter > ID_AUXPES)) {
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
	default: {
	  // outDebugString(L"default hit in chkMsg: message [{}]\n", Msg.message);
	  break;
	}
  }
  return false;
}

// return the width of a text item
auto thred::txtWid(wchar_t const* string) -> SIZE {
  auto textSize = SIZE {};
  wrap::getTextExtentPoint32(ThredDC, string, wrap::toUnsigned(wcslen(string)), &textSize);
  return textSize;
}

void thred::internal::makCol() noexcept {
  auto buffer = std::array<wchar_t, 3> {};
  buffer[1]         = L'0';
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const hFont = displayText::getThrEdFont(FONTSIZE);
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	DefaultColorWin->operator[](iColor) = CreateWindow(L"STATIC",
	                                                   nullptr,
	                                                   SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                                   0,
	                                                   ButtonHeight * gsl::narrow_cast<int32_t>(iColor),
	                                                   ButtonWidth,
	                                                   ButtonHeight,
	                                                   ThrEdWindow,
	                                                   nullptr,
	                                                   ThrEdInstance,
	                                                   nullptr);
	displayText::setWindowFont(DefaultColorWin->operator[](iColor), hFont);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	UserColorWin->operator[](iColor) = CreateWindow(L"STATIC",
	                                                nullptr,
	                                                SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                                ButtonWidth,
	                                                ButtonHeight * gsl::narrow_cast<int32_t>(iColor),
	                                                ButtonWidth,
	                                                ButtonHeight,
	                                                ThrEdWindow,
	                                                nullptr,
	                                                ThrEdInstance,
	                                                nullptr);
	buffer[0]                        = ThreadSize[iColor];
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	ThreadSizeWin[iColor] = CreateWindow(L"STATIC",
	                                     buffer.data(),
	                                     SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                     ButtonWidth * 2,
	                                     ButtonHeight * gsl::narrow_cast<int32_t>(iColor),
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
  auto lockFilePath = fs::path {};
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto       ppszPath = PWSTR {nullptr}; // variable to receive the path memory block pointer.
  auto const hr       = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, nullptr, &ppszPath);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (SUCCEEDED(hr)) {
	lockFilePath.assign(ppszPath); // make a local copy of the path
	lockFilePath /= L"ThrEd";
	fs::create_directory(lockFilePath);
	lockFilePath /= L"thredloc.txt";
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const lockFile =
	    CreateFile(lockFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (lockFile != INVALID_HANDLE_VALUE) {
	  auto bytesWritten = DWORD {0};
	  auto value        = utf::Utf16ToUtf8(*HomeDirectory);
	  wrap::WriteFile(lockFile, value.data(), wrap::toUnsigned(value.size()) + 1U, &bytesWritten, nullptr);
	  CloseHandle(lockFile);
	}
  }
  CoTaskMemFree(ppszPath); // free up the path memory block
}

#pragma warning(push)
#pragma warning(disable : 26493) // Don't use C-style casts (type.4)
void thred::internal::crtcurs() noexcept {
  // clang-format off
  FormCursor            = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Form));      // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  DLineCursor           = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_DLIN));      // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  NeedleUpCursor        = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Upright));   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  NeedleRightDownCursor = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightDown)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  NeedleRightUpCursor   = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightUp));   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  NeedleLeftDownCursor  = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftDown));  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  NeedleLeftUpCursor    = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftUp));    // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
  // clang-format on
}
#pragma warning(pop)

void thred::internal::duhom() {
  auto const arg0 = fs::path {ArgList[0]};
  *HomeDirectory  = arg0.parent_path();
}

auto thred::getHomeDir() noexcept -> fs::path* {
  return HomeDirectory;
}

auto thred::getBackGroundBrush() noexcept -> HBRUSH {
  return BackgroundBrush;
}

void thred::internal::ducmd() {
  if (ArgCount > 1) {
	auto const arg1 = std::wstring {ArgList[1]};
	if (arg1.compare(0, 4, L"/F1:") == 0) {
	  auto balaradFileName = *HomeDirectory / arg1.substr(4);
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto BalaradFile =
	      CreateFile(balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (BalaradFile != INVALID_HANDLE_VALUE) {
		CloseHandle(BalaradFile);
		*BalaradName0 = balaradFileName;
		if (ArgCount > 2) {
		  auto const arg2 = std::wstring {ArgList[2]};
		  if (arg2.compare(0, 4, L"/F2:") == 0) {
			balaradFileName = *HomeDirectory / arg2.substr(4);
			BalaradFile     = CreateFile(
                balaradFileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
			if (BalaradFile != INVALID_HANDLE_VALUE) {
			  auto readBuffer = std::vector<char> {};
			  readBuffer.resize(_MAX_PATH + 1);
			  *BalaradName1  = balaradFileName;
			  auto bytesRead = DWORD {0};

			  ReadFile(BalaradFile, readBuffer.data(), wrap::toUnsigned(readBuffer.size()), &bytesRead, nullptr);
			  if (bytesRead != 0U) {
				readBuffer.resize(bytesRead);
				BalaradName2->assign(readBuffer.data());
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
	  StateMap->set(StateFlag::REDOLD);
	  nuFil(fileIndices::THR);
	}
  }
}

void thred::internal::setPrefs() {
  defpref();
  getDocsFolder(DefaultDirectory);
  if (DesignerName->empty()) {
	displayText::loadString(*DesignerName, IDS_UNAM);
	getdes();
  }
}

void thred::internal::redini() {
  duhom();
  *IniFileName = *HomeDirectory;
  *IniFileName /= L"thred.ini";
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const iniFileHandle =
      CreateFile(IniFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (iniFileHandle == INVALID_HANDLE_VALUE) {
	setPrefs();
  }
  else {
	auto bytesRead = DWORD {0};
	ReadFile(iniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, nullptr);
	CloseHandle(iniFileHandle);
	if (bytesRead < sizeof(IniFile)) {
	  auto newFileName = *IniFileName;
	  newFileName.replace_filename("thred-ini.bak");
	  fs::rename(*IniFileName, newFileName);
	  setPrefs();
	}
	else {
	  auto const directory =
	      utf::Utf8ToUtf16(std::string(static_cast<char const*>(IniFile.defaultDirectory)));
	  DefaultDirectory->assign(directory);
	  {
		auto previousName = PreviousNames->begin();
		for (auto const& prevName : IniFile.prevNames) {
		  if (strlen(std::begin(prevName)) != 0U) {
			previousName->assign(utf::Utf8ToUtf16(std::string(std::begin(prevName))));
		  }
		  else {
			previousName->clear();
		  }
		  ++previousName;
		}
	  }
	  DesignerName->assign(utf::Utf8ToUtf16(std::string(static_cast<char const*>(IniFile.designerName))));
	  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
		UserColor[iColor]             = IniFile.stitchColors[iColor];
		CustomColor[iColor]           = IniFile.stitchPreferredColors[iColor];
		CustomBackgroundColor[iColor] = IniFile.backgroundPreferredColors[iColor];
	  }
	  bitmap::setBmpBackColor();
	  BackgroundColor = IniFile.backgroundColor;
	  bitmap::setBitmapColor(IniFile.bitmapColor);
	  MinStitchLength = IniFile.minStitchLength;
	  if (IniFile.showStitchThreshold < 0) {
		IniFile.showStitchThreshold = 0;
	  }
	  if (IniFile.showStitchThreshold > 9.0F) {
		IniFile.showStitchThreshold = 9.0F;
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
		auto const tmp = EnumMap<UserFlag>(IniFile.userFlagMap);
		*UserFlagMap   = tmp;
	  }
	  if (IniFile.borderWidth != 0.0F) {
		BorderWidth = IniFile.borderWidth;
	  }
	  if (IniFile.appliqueColor != 0U) {
		AppliqueColor = IniFile.appliqueColor & COLORBTS;
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
	  UnzoomedRect = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
	  PicotSpacing = IniFile.picotSpace;
	}
  }
  if (IniFile.gridColor == 0U) {
	IniFile.gridColor = DEFGRD;
  }
  if (IniFile.fillAngle == 0.0F) {
	IniFile.fillAngle = PI_F / 6.0F;
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

auto thred::getLayerPen(uint32_t layer) noexcept -> HPEN {
  return LayerPen[layer];
}

void thred::internal::setLayerPens() noexcept {
  LayerPen[0] = wrap::CreatePen(PS_SOLID, PENNWID, PENSILVR);
  LayerPen[1] = wrap::CreatePen(PS_SOLID, PENNWID, PENTRQSE);
  LayerPen[2] = wrap::CreatePen(PS_SOLID, PENNWID, PENLILAC);
  LayerPen[3] = wrap::CreatePen(PS_SOLID, PENNWID, PENPOLIV);
  LayerPen[4] = wrap::CreatePen(PS_SOLID, PENNWID, PENTEAL);
}

void thred::internal::init() {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const deviceContext   = GetDC(nullptr);
  auto const screenHalfWidth = (GetDeviceCaps(deviceContext, HORZRES)) / 2;
  ReleaseDC(nullptr, deviceContext);
  TexturePointsBuffer->clear();
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1));
  MainMenu   = GetMenu(ThrEdWindow);
  auto wRect = RECT {0L, 0L, 0L, 0L};
  GetWindowRect(ThrEdWindow, &wRect);
  auto mRect = RECT {0L, 0L, 0L, 0L};
  GetMenuItemRect(ThrEdWindow, MainMenu, 0, &mRect);
  wRect.left  = mRect.left;
  wRect.right = mRect.right;
  for (auto iMenu = 0U; iMenu <= M_HELP; ++iMenu) {
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  ArrowCursor = LoadCursor(nullptr, IDC_ARROW);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  CrossCursor = LoadCursor(nullptr, IDC_CROSS);
  crtcurs();
  redfils();
  StateMap->reset(); // clear the bitmap
  // set up the size variables
  ThredDC = GetDC(ThrEdWindow);
  SetStretchBltMode(ThredDC, COLORONCOLOR);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  constexpr int32_t TXTSIDS = 6; // extra pixels in a text box
  auto const        size    = thred::txtWid(L"MM");
  ButtonWidth               = size.cx + TXTSIDS;
  ButtonWidthX3             = ButtonWidth * 3;
  ButtonHeight              = size.cy + 4;
  auto const offsetStepSize = thred::txtWid(L"0");
  for (auto iOffset = 0U; iOffset < NERCNT; ++iOffset) {
	BoxOffset[iOffset] = offsetStepSize.cx * (gsl::narrow_cast<int32_t>(iOffset) + 1);
  }
  GetClientRect(ThrEdWindow, &ThredWindowRect);
  stchWnd();
  displayText::lodstr();
  ThreadSize.fill(L'4');
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
	IniFile.stitchSizePixels = DEFPNTPX;
  }
  if (IniFile.formVertexSizePixels == 0U) {
	IniFile.formVertexSizePixels = DEFPNTPX;
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
  ButtonWin->resize(BTNCOUNT);
  auto const blank = std::wstring {};
  for (auto iButton = 0U; iButton < BTNCOUNT; ++iButton) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	auto windowFlags = gsl::narrow_cast<DWORD>(SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER);
	auto buttonTxt = std::wstring {};
	switch (iButton) {
	  case HBOXSEL: {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		buttonTxt.assign(StringTable->operator[](STR_BOXSEL));
		break;
	  }
	  case HUPTO: {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		windowFlags = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		buttonTxt.assign(StringTable->operator[](STR_UPOF));
		break;
	  }
	  case HHID: {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		windowFlags = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER;
		buttonTxt.assign(StringTable->operator[](STR_PIKOL));
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
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
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
  static constexpr auto boxColor = std::array<COLORREF, 4> {0x404040, 0x408040, 0x804040, 0x404080};
  for (auto iRGBK = 0U; iRGBK < 4; ++iRGBK) {
	BoxPen[iRGBK] = wrap::CreatePen(PS_SOLID, PENNWID, boxColor[iRGBK]);
  }
  LinePen        = wrap::CreatePen(PS_SOLID, PENNWID, PENCHCL);
  CrossPen       = wrap::CreatePen(PS_SOLID, PENWWID, PENPRPLE);
  GroupSelectPen = wrap::CreatePen(PS_SOLID, PENNWID, PENPRPLE);
  GridPen        = wrap::CreatePen(PS_SOLID, PENNWID, IniFile.gridColor);
  BackgroundPen  = wrap::CreatePen(PS_SOLID, PENMWID, BackgroundColor);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const bitmapPen = wrap::CreatePen(PS_SOLID, PENNWID, bitmap::getBmpColor());
  bitmap::setBitmapPen(bitmapPen);
  FormPen         = wrap::CreatePen(PS_SOLID, PENNWID, PENSILVR);
  MultiFormPen    = wrap::CreatePen(PS_SOLID, PENNWID, PENPOLIV);
  FormPen3px      = wrap::CreatePen(PS_SOLID, PENMWID, PENSILVR);
  FormSelectedPen = wrap::CreatePen(PS_SOLID, PENNWID, PENSEGRN);
  ZoomMarkPen     = wrap::CreatePen(PS_SOLID, PENMWID, PENLMGRN);
  SelectAllPen    = wrap::CreatePen(PS_SOLID, PENNWID, PENROSY);
  KnotPen         = wrap::CreatePen(PS_SOLID, PENNWID, PENWHITE);
  setLayerPens();
  BackgroundPenWidth = 1;
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	ThreadSizePixels[iColor] = 1;
	ThreadSizeIndex[iColor]  = 1;
	UserPen[iColor]          = wrap::CreatePen(PS_SOLID, PENNWID, UserColor[iColor]);
  }
  BackgroundBrush = CreateSolidBrush(BackgroundColor);
  // create brushes
  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	DefaultColorBrush[iColor] = CreateSolidBrush(DefaultColors[iColor]);
	UserColorBrush[iColor]    = CreateSolidBrush(UserColor[iColor]);
  }
  ZoomFactor = 1;
  StitchBuffer->clear();
  GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
  ladj();
  wrap::getTextExtentPoint32(ThredDC,
                             StringTable->operator[](STR_PIKOL).c_str(),
                             wrap::toUnsigned(StringTable->operator[](STR_PIKOL).size()),
                             &PickColorMsgSize);
  auxmen();
  fnamtabs();
  ritfnam(*DesignerName);
  auto       designer     = utf::Utf16ToUtf8(*DesignerName);
  auto&      modName      = ExtendedHeader->modifierName;
  auto const modifierName = gsl::span<char> {modName};
  std::copy(designer.begin(), designer.end(), modifierName.begin());
  thred::chkhup();
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
  StateMap->set(StateFlag::HIDMAP);
  thred::hidbit();
  thred::chkmen();
  // check command line-should be last item in init
  ducmd();
  SetWindowText(ThrEdWindow, fmt::format(StringTable->operator[](STR_THRED), *DesignerName).c_str());
}

auto thred::internal::defTxt(uint32_t iColor) -> COLORREF {
  // bitmap for color number. Black or white bit chosen for contrast against the default background colors
  constexpr auto textColorMap = std::bitset<16>(0xbaf);
  return textColorMap.test(iColor) ? PENWHITE : PENBLK;
}

void thred::internal::relin() {
  MoveLine0[0] = stch2px1(ClosestPointIndex - 1);
  MoveLine1[1] = stch2px1(ClosestPointIndex + 1U);
  StateMap->reset(StateFlag::WASLIN);
  dulin(MoveLine0, MoveLine1);
}

void thred::internal::dumov() {
  constexpr auto ABPOINTS = 7U; // Number of arrow box points
  auto const     sCurr    = wrap::next(StitchBuffer->begin(), MoveAnchor);
  auto const     sNext    = std::next(sCurr);
  RotateAngle             = atan2f(sNext->y - sCurr->y, sNext->x - sCurr->x);
  if (sCurr->x >= ZoomRect.left && sCurr->x <= ZoomRect.right && sCurr->y >= ZoomRect.bottom &&
      sCurr->y <= ZoomRect.top) {
	auto       arrowBox       = std::array<POINT, ABPOINTS> {};
	auto const abCenterPixels = sdCor2px(StitchBuffer->operator[](MoveAnchor));
	arrowBox[0]               = abCenterPixels;
	arrowBox[6]               = abCenterPixels;
	auto OffsetFromCenter     = POINT {abCenterPixels.x + 12, abCenterPixels.y + 2};
	rotpix(OffsetFromCenter, arrowBox[1], abCenterPixels);
	OffsetFromCenter.y = abCenterPixels.y - 2;
	rotpix(OffsetFromCenter, arrowBox[5], abCenterPixels);
	OffsetFromCenter.y = abCenterPixels.y + 6;
	rotpix(OffsetFromCenter, arrowBox[2], abCenterPixels);
	OffsetFromCenter.y = abCenterPixels.y - 6;
	rotpix(OffsetFromCenter, arrowBox[4], abCenterPixels);
	OffsetFromCenter.x = abCenterPixels.x + 20;
	OffsetFromCenter.y = abCenterPixels.y;
	rotpix(OffsetFromCenter, arrowBox[3], abCenterPixels);
	SelectObject(StitchWindowMemDC, FormPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	wrap::Polyline(StitchWindowMemDC, arrowBox.data(), wrap::toUnsigned(arrowBox.size()));
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
}

auto thred::internal::chkup(uint32_t count, size_t iStitch) -> uint32_t {
  if (StateMap->test(StateFlag::UPTO) && (ClosestPointIndex != 0U)) {
	if (ColorChangeTable->operator[](iStitch).stitchIndex < ClosestPointIndex) {
	  if (ColorChangeTable->operator[](iStitch + 1U).stitchIndex < ClosestPointIndex) {
		return count;
	  }
	  return ClosestPointIndex - ColorChangeTable->operator[](iStitch).stitchIndex + 1U;
	}
	return 0;
  }
  return count;
}

void thred::internal::stCor2px(fPOINTATTR const& stitch, POINT& point) {
  point = {wrap::ceil<int32_t>((stitch.x - ZoomRect.left) * ZoomRatio.x),
           wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                               (stitch.y - ZoomRect.bottom) * ZoomRatio.y)};
}

void thred::internal::drwknot() {
  constexpr auto KBOFFSET = 5;  // offset of the knot box sides;
  constexpr auto KLINELEN = 10; // length of the knot line;
  if (!UserFlagMap->test(UserFlag::KNOTOF) && (KnotCount != 0U) && (!StitchBuffer->empty())) {
	auto const kOffset  = MulDiv(KBOFFSET, *screenDPI, STDDPI);
	auto const kLine    = MulDiv(KLINELEN, *screenDPI, STDDPI);
	auto       point    = POINT {0L, 0L};
	auto       kOutline = std::array<POINT, SQPNTS> {};
	auto       tLine    = std::array<POINT, LNPNTS> {};
	for (auto ind = 0U; ind < KnotCount; ++ind) {
	  stCor2px(StitchBuffer->operator[](Knots[ind]), point);
	  SelectObject(StitchWindowMemDC, KnotPen);
	  SetROP2(StitchWindowMemDC, R2_XORPEN);
	  kOutline[0].x = kOutline[3].x = kOutline[4].x = point.x - kOffset;
	  kOutline[1].x = kOutline[2].x = point.x + kOffset;
	  kOutline[0].y = kOutline[1].y = kOutline[4].y = point.y + kOffset;
	  kOutline[2].y = kOutline[3].y = point.y - kOffset;
	  wrap::Polyline(StitchWindowMemDC, kOutline.data(), wrap::toUnsigned(kOutline.size()));
	  tLine[0].x = point.x - kLine;
	  tLine[1].x = point.x + kLine;
	  tLine[0].y = tLine[1].y = point.y;
	  wrap::Polyline(StitchWindowMemDC, tLine.data(), wrap::toUnsigned(tLine.size()));
	  SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
  }
}

void thred::internal::dugrid() {
  if (ZoomFactor < ShowStitchThreshold || (ShowStitchThreshold == 0.0F)) {
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
	  gridLine[0].y = gridLine[1].y = wrap::ceil<int32_t>(
	      wrap::toFloat(StitchWindowClientRect.bottom) -
	      (wrap::toFloat(iGrid) * IniFile.gridSize - ZoomRect.bottom) * ZoomRatio.y);
	  wrap::Polyline(StitchWindowMemDC, gridLine.data(), wrap::toUnsigned(gridLine.size()));
	}
	gridLine[0].y = 0;
	gridLine[1].y = StitchWindowClientRect.bottom;
	for (auto iGrid = gridRect.left; iGrid <= gridRect.right; ++iGrid) {
	  gridLine[0].x = gridLine[1].x =
	      wrap::ceil<int32_t>((wrap::toFloat(iGrid) * IniFile.gridSize - ZoomRect.left) * ZoomRatio.x);
	  wrap::Polyline(StitchWindowMemDC, gridLine.data(), wrap::toUnsigned(gridLine.size()));
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
}

auto thred::internal::setRmap(boost::dynamic_bitset<>& stitchMap,
                              fPOINTATTR const&        stitchPoint,
                              fPOINT const&            cellSize) -> bool {
  auto const bitPoint = gsl::narrow<uint32_t>(floor((stitchPoint.x - ZoomRect.left) / cellSize.x) *
                                              floor((stitchPoint.y - ZoomRect.bottom) / cellSize.y));
  return !stitchMap.test_set(bitPoint);
}

void thred::internal::drawBackground() {
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
  thred::duzrat();
  if (bitmap::ismap() && !StateMap->test(StateFlag::HIDMAP) && !StateMap->test(StateFlag::UPTO)) {
	bitmap::drawBmpBackground();
  }
  dugrid();
}

void thred::internal::drwStch() {
  StateMap->set(StateFlag::RELAYR);
  StateMap->reset(StateFlag::SELSHO);
  StateMap->reset(StateFlag::ILIN1);
  StateMap->reset(StateFlag::BZUM);
  unboxs();
  unrotu();
  unrot();
  StateMap->reset(StateFlag::ILIN);
  uncros();
  StateMap->reset(StateFlag::SHOFRM);
  auto stitchCount = 0U;
  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
	auto const deltaCount = gsl::narrow<uint32_t>(ColorChangeTable->operator[](iColor + 1U).stitchIndex -
	                                              ColorChangeTable->operator[](iColor).stitchIndex);
	if (deltaCount > stitchCount) {
	  stitchCount = deltaCount;
	}
  }
  auto linePoints = std::vector<POINT> {};
  linePoints.reserve(wrap::toSize(stitchCount) + 2U);
  thi::drawBackground();
  if (StateMap->test(StateFlag::INIT)) {
	if (StateMap->test(StateFlag::ZUMED)) {
	  auto scrollInfo   = SCROLLINFO {}; // scroll bar i/o structure
	  scrollInfo.cbSize = sizeof(scrollInfo);
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  scrollInfo.fMask = SIF_ALL;
	  scrollInfo.nMax  = UnzoomedRect.y;
	  scrollInfo.nMin  = 0;
	  scrollInfo.nPage = wrap::round<UINT>(ZoomRect.top - ZoomRect.bottom);
	  scrollInfo.nPos =
	      wrap::round<decltype(scrollInfo.nPos)>(wrap::toFloat(UnzoomedRect.y) - ZoomRect.top);
	  SetScrollInfo(VerticalScrollBar, SB_CTL, &scrollInfo, TRUE);
	  scrollInfo.nMax  = UnzoomedRect.x;
	  scrollInfo.nPage = wrap::round<UINT>(ZoomRect.right - ZoomRect.left);
	  scrollInfo.nPos  = wrap::round<decltype(scrollInfo.nPos)>(ZoomRect.left);
	  SetScrollInfo(HorizontalScrollBar, SB_CTL, &scrollInfo, TRUE);
	  ShowWindow(HorizontalScrollBar, TRUE);
	  ShowWindow(VerticalScrollBar, TRUE);
	}
	else {
	  ShowWindow(VerticalScrollBar, FALSE);
	  ShowWindow(HorizontalScrollBar, FALSE);
	}
	thred::duzrat();
	auto const    dub6           = ZoomRatio.x * 6.0F;
	const auto threadWidth = std::array<int32_t, 3> {
	    std::lround(dub6 * TSIZ30), std::lround(dub6 * TSIZ40), std::lround(dub6 * TSIZ60)}; // thread sizes in pixels
	for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
	  if (StateMap->test(StateFlag::THRDS)) {
		nuStchSiz(iColor, threadWidth[ThreadSizeIndex[iColor]]);
	  }
	  else {
		if (StateMap->test(StateFlag::COL)) {
		  if (iColor == ActiveColor) {
			nuStchSiz(iColor, threadWidth[ThreadSizeIndex[iColor]]);
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
	DisplayedColorBitmap.reset();
	if (StateMap->test(StateFlag::ZUMED)) {
	  StateMap->reset(StateFlag::LINED);
	  StateMap->reset(StateFlag::LININ);
	  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
		if (StateMap->test(StateFlag::HID)) {
		  if (ColorChangeTable->operator[](iColor).colorIndex != ActiveColor) {
			stitchCount = ColorChangeTable->operator[](iColor + 1U).stitchIndex -
			              ColorChangeTable->operator[](iColor).stitchIndex;
			auto const                      stitchIt =
			    wrap::next(StitchBuffer->begin(), ColorChangeTable->operator[](iColor).stitchIndex);
			for (auto iStitch = ptrdiff_t {}; iStitch < gsl::narrow<ptrdiff_t>(stitchCount); ++iStitch) {
			  if (stitchIt[iStitch].x >= ZoomRect.left && stitchIt[iStitch].x <= ZoomRect.right &&
			      stitchIt[iStitch].y >= ZoomRect.bottom && stitchIt[iStitch].y <= ZoomRect.top) {
				DisplayedColorBitmap.set(ColorChangeTable->operator[](iColor).colorIndex);
				break;
			  }
			}
			continue;
		  }
		}
		auto wascol = 0U;
		SelectObject(StitchWindowMemDC, UserPen[ColorChangeTable->operator[](iColor).colorIndex]);
		stitchCount = ColorChangeTable->operator[](iColor + 1U).stitchIndex -
		              ColorChangeTable->operator[](iColor).stitchIndex;
		if (!StitchBuffer->empty()) {
		  auto const stitchIt =
		      wrap::next(StitchBuffer->begin(), ColorChangeTable->operator[](iColor).stitchIndex);
		  stitchCount          = chkup(stitchCount, iColor);
		  auto const maxYcoord = wrap::toFloat(DrawItem->rcItem.bottom);
		  for (auto iStitch = ptrdiff_t {}; iStitch < gsl::narrow<ptrdiff_t>(stitchCount); ++iStitch) {
			auto const layer = (stitchIt[iStitch].attribute & LAYMSK) >> LAYSHFT;
			if ((ActiveLayer == 0U) || (layer == 0U) || (layer == ActiveLayer)) {
			  if (stitchIt[iStitch].x >= ZoomRect.left && stitchIt[iStitch].x <= ZoomRect.right &&
			      stitchIt[iStitch].y >= ZoomRect.bottom && stitchIt[iStitch].y <= ZoomRect.top) {
				wascol = 1;
				if (StateMap->testAndSet(StateFlag::LINED)) {
				  if (StateMap->testAndSet(StateFlag::LININ)) {
					linePoints.push_back(
					    {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					     wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                         ZoomRatio.y)});
				  }
				  else {
					if (iStitch == 0) {
					  linePoints.push_back(
					      {wrap::ceil<int32_t>((stitchIt->x - ZoomRect.left) * ZoomRatio.x),
					       wrap::ceil<int32_t>(maxYcoord -
					                           (stitchIt->y - ZoomRect.bottom) * ZoomRatio.y)});
					}
					else {
					  linePoints.push_back(
					      {wrap::ceil<int32_t>((stitchIt[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x),
					       wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1].y - ZoomRect.bottom) *
					                                           ZoomRatio.y)});
					  linePoints.push_back(
					      {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					       wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                           ZoomRatio.y)});
					}
				  }
				}
				else {
				  if (iStitch == 0) {
					linePoints.push_back(
					    {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					     wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                         ZoomRatio.y)});
				  }
				  else {
					linePoints.push_back(
					    {wrap::ceil<int32_t>((stitchIt[iStitch - 1].x - ZoomRect.left) * ZoomRatio.x),
					     wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1].y - ZoomRect.bottom) *
					                                         ZoomRatio.y)});
					linePoints.push_back(
					    {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
					     wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
					                                         ZoomRatio.y)});
				  }
				  StateMap->set(StateFlag::LININ);
				}
			  }
			  else {
				if (StateMap->testAndReset(StateFlag::LININ)) {
				  linePoints.push_back(
				      {wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
				       wrap::ceil<int32_t>(maxYcoord -
				                           (stitchIt[iStitch].y - ZoomRect.bottom) * ZoomRatio.y)});
				  wrap::Polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
				  linePoints.clear();
				}
				else {
				  if (iStitch != 0U) {
					// write an equation for this line
					auto const xDelta     = stitchIt[iStitch].x - stitchIt[iStitch - 1].x;
					auto const yDelta     = stitchIt[iStitch - 1].y - stitchIt[iStitch].y;
					auto const slope      = xDelta / yDelta;
					auto const offset     = stitchIt[iStitch].x + slope * stitchIt[iStitch].y;
					auto       stitchLine = std::array<POINT, LNPNTS> {};
					do {
					  // does the line intersect with the top of the screen?
					  auto gapToEdge = offset - slope * ZoomRect.top;
					  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
						stitchLine[0] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch - 1].x - ZoomRect.left) *
						                        ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1].y - ZoomRect.bottom) *
						                                        ZoomRatio.x)};
						stitchLine[1] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
						                                        ZoomRatio.x)};
						wrap::Polyline(
						    StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
						break;
					  }
					  // does the line intersect the bottom of the screen?
					  gapToEdge = offset - slope * ZoomRect.bottom;
					  if (gapToEdge >= ZoomRect.left && gapToEdge <= ZoomRect.right) {
						stitchLine[0] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch - 1].x - ZoomRect.left) *
						                        ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1].y - ZoomRect.bottom) *
						                                        ZoomRatio.y)};
						stitchLine[1] = POINT {
						    wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
						    wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
						                                        ZoomRatio.y)};
						wrap::Polyline(
						    StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
						break;
					  }
					  // does the line intersect the left side of the screen?
					  if (slope != 0.0F) {
						gapToEdge = (offset - ZoomRect.left) / slope;
						if (gapToEdge >= ZoomRect.bottom && gapToEdge <= ZoomRect.top) {
						  stitchLine[0] = POINT {
						      wrap::ceil<int32_t>((stitchIt[iStitch - 1].x - ZoomRect.left) *
						                          ZoomRatio.x),
						      wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch - 1].y - ZoomRect.bottom) *
						                                          ZoomRatio.y)};
						  stitchLine[1] = POINT {
						      wrap::ceil<int32_t>((stitchIt[iStitch].x - ZoomRect.left) * ZoomRatio.x),
						      wrap::ceil<int32_t>(maxYcoord - (stitchIt[iStitch].y - ZoomRect.bottom) *
						                                          ZoomRatio.y)};
						  wrap::Polyline(
						      StitchWindowMemDC, stitchLine.data(), wrap::toUnsigned(stitchLine.size()));
						}
					  }
					} while (false);
				  }
				}
			  }
			}
		  }
		}
		if (!linePoints.empty()) {
		  wrap::Polyline(StitchWindowMemDC, linePoints.data(), wrap::toUnsigned(linePoints.size()));
		  auto lastPoint = linePoints.back();
		  linePoints.clear();
		  linePoints.push_back(lastPoint);
		}
		if (wascol != 0U) {
		  DisplayedColorBitmap.set(ColorChangeTable->operator[](iColor).colorIndex);
		}
	  }
	}
	else {
	  auto const pwid = StateMap->test(StateFlag::HID);
	  for (auto iColor = size_t {}; iColor < thred::maxColor(); ++iColor) {
		DisplayedColorBitmap.set(ColorChangeTable->operator[](iColor).colorIndex);
		stitchCount = ColorChangeTable->operator[](iColor + 1U).stitchIndex -
		              ColorChangeTable->operator[](iColor).stitchIndex;
		stitchCount = chkup(stitchCount, iColor);
		if (!pwid || ColorChangeTable->operator[](iColor).colorIndex == ActiveColor) {
		  drwLin(linePoints,
		         ColorChangeTable->operator[](iColor).stitchIndex,
		         stitchCount,
		         UserPen[ColorChangeTable->operator[](iColor).colorIndex]);
		}
	  }
	}
	if (StateMap->test(StateFlag::SELBOX)) {
	  if (!StitchBuffer->empty()) {
		ritcor(StitchBuffer->operator[](ClosestPointIndex));
		auto stitchCoordsInPixels = POINT {};
		if (stch2px(ClosestPointIndex, stitchCoordsInPixels)) {
		  dubox(stitchCoordsInPixels);
		}
	  }
	}
	if (StateMap->test(StateFlag::FRMPSEL)) {
	  auto const vertexIt =
	      wrap::next(FormVertices->cbegin(),
	                 FormList->operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
	  thred::ritfcor(*vertexIt);
	}
	if (!StateMap->test(StateFlag::SELBOX) && !StateMap->test(StateFlag::FRMPSEL)) {
	  auto const blank = std::wstring {};
	  displayText::butxt(HCOR, blank);
	}
	if (StateMap->test(StateFlag::WASLIN)) {
	  relin();
	  throw; // this is here for testing I don't think this code is reachable
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
	      fPOINT {(ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right),
	              (ZoomRect.top - ZoomRect.bottom) / wrap::toFloat(StitchWindowClientRect.bottom)};
	  auto const maxMapSize = wrap::toSize(StitchWindowClientRect.right * StitchWindowClientRect.bottom);
	  auto       stitchMap = boost::dynamic_bitset<>(maxMapSize, 0U);
	  SelectObject(StitchWindowMemDC, LinePen);
	  SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	  if (StateMap->test(StateFlag::HID)) {
		for (auto iColor = 0U; iColor < thred::maxColor(); ++iColor) {
		  if (ColorChangeTable->operator[](iColor).colorIndex == ActiveColor) {
			for (auto iStitch = ColorChangeTable->operator[](iColor).stitchIndex;
			     iStitch < ColorChangeTable->operator[](gsl::narrow_cast<size_t>(iColor) + 1U).stitchIndex;
			     ++iStitch) {
			  auto const& stitch = StitchBuffer->operator[](iStitch);
			  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right && stitch.y >= ZoomRect.bottom &&
			      stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch, cellSize)) {
				stchbox(iStitch, StitchWindowMemDC);
			  }
			}
		  }
		}
	  }
	  else {
		auto iStitch = 0U;
		for (auto& stitch : *StitchBuffer) {
		  if (stitch.x >= ZoomRect.left && stitch.x <= ZoomRect.right && stitch.y >= ZoomRect.bottom &&
		      stitch.y <= ZoomRect.top && setRmap(stitchMap, stitch, cellSize)) {
			stchbox(iStitch, StitchWindowMemDC);
		  }
		  ++iStitch;
		}
	  }
	  SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
	if (StateMap->test(StateFlag::CLPSHO)) {
	  duclp();
	}
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
	drwmrk(StitchWindowMemDC);
  }
  if (StateMap->test(StateFlag::PRFACT)) {
	thred::redraw(PreferencesWindow);
  }
  if (StateMap->test(StateFlag::SELBOX)) {
	displayText::ritnum(STR_NUMSEL, ClosestPointIndex);
  }
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  if (StateMap->test(StateFlag::INIT)) {
	lenCalc();
  }
  if (StateMap->test(StateFlag::MOVSET)) {
	dumov();
  }
  drwknot();
}

void thred::internal::dubar() {
  auto colorBarRect = RECT {DrawItem->rcItem.left, 0, DrawItem->rcItem.right, DrawItem->rcItem.bottom};
  auto       indicatorLine = std::array<POINT, 2> {};
  auto const buffSize      = wrap::toFloat(StitchBuffer->size());
  for (auto iColorChange = size_t {}; iColorChange < thred::maxColor(); ++iColorChange) {
	auto const barSectionHeight =
	    wrap::toFloat(ColorChangeTable->operator[](iColorChange + 1U).stitchIndex) / buffSize;
	colorBarRect.bottom = std::lround(barSectionHeight * wrap::toFloat(DrawItem->rcItem.bottom));
	FillRect(DrawItem->hDC, &colorBarRect, UserColorBrush[ColorChangeTable->operator[](iColorChange).colorIndex]);
	colorBarRect.top = colorBarRect.bottom;
  }
  if (StateMap->test(StateFlag::SELBOX) || StateMap->test(StateFlag::GRPSEL)) {
	auto selectedIndicator = wrap::toFloat(ClosestPointIndex) / buffSize;
	indicatorLine[0].y     = indicatorLine[1].y =
	    wrap::ceil<decltype(indicatorLine[0].y)>(wrap::toFloat(colorBarRect.bottom) * selectedIndicator);
	indicatorLine[0].x = colorBarRect.left;
	indicatorLine[1].x = colorBarRect.right;
	SelectObject(DrawItem->hDC, CrossPen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	wrap::Polyline(DrawItem->hDC, indicatorLine.data(), wrap::toUnsigned(indicatorLine.size()));
	if (StateMap->test(StateFlag::GRPSEL)) {
	  selectedIndicator  = wrap::toFloat(GroupStitchIndex) / buffSize;
	  indicatorLine[0].y = indicatorLine[1].y =
	      wrap::ceil<decltype(indicatorLine[0].y)>(wrap::toFloat(colorBarRect.bottom) * selectedIndicator);
	  indicatorLine[0].x = colorBarRect.left;
	  indicatorLine[1].x = colorBarRect.right;
	  wrap::Polyline(DrawItem->hDC, indicatorLine.data(), wrap::toUnsigned(indicatorLine.size()));
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
  }
}

void thred::internal::ritbak(fs::path const& fileName, DRAWITEMSTRUCT* drawItem) {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto thrEdFile = CreateFile(fileName.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (thrEdFile != INVALID_HANDLE_VALUE) {
	auto bytesRead    = DWORD {0};
	auto stitchHeader = STRHED {};
	ReadFile(thrEdFile, &stitchHeader, sizeof(stitchHeader), &bytesRead, nullptr);
	if (bytesRead == sizeof(stitchHeader)) {
	  auto       stitchSourceSize = fPOINT {1.0F, 1.0F};
	  auto const fileTypeVersion  = (stitchHeader.headerType & FTYPMASK) >> TBYTSHFT;
	  if ((stitchHeader.headerType & SIGMASK) == THREDSIG) {
		switch (fileTypeVersion) {
		  case 0: {
			if (stitchHeader.hoopType == SMALHUP) {
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
			ReadFile(thrEdFile, &extendedHeader, sizeof(extendedHeader), &bytesRead, nullptr);
			if (bytesRead != sizeof(extendedHeader)) {
			  return;
			}
			stitchSourceSize.x = extendedHeader.hoopSizeX;
			stitchSourceSize.y = extendedHeader.hoopSizeY;
			break;
		  }
		  default: {
			return;
		  }
		}
	  }
	  auto const drawingDestinationSize = POINT {(drawItem->rcItem.right - drawItem->rcItem.left),
	                                             (drawItem->rcItem.bottom - drawItem->rcItem.top)};
	  auto const xRatio = wrap::toFloat(drawingDestinationSize.x) / stitchSourceSize.x;
	  auto const yRatio = wrap::toFloat(drawingDestinationSize.y) / stitchSourceSize.y;
	  auto const ratio  = (xRatio < yRatio) ? xRatio : yRatio;
	  if (stitchHeader.stitchCount != 0U) {
		auto stitchesToDraw = std::vector<fPOINTATTR> {};
		stitchesToDraw.resize(stitchHeader.stitchCount);
		auto lines = std::vector<POINT> {};
		lines.resize(stitchHeader.stitchCount);
		auto const bytesToRead =
		    gsl::narrow_cast<DWORD>(stitchHeader.stitchCount * sizeof(StitchBuffer->front()));
		ReadFile(thrEdFile, stitchesToDraw.data(), bytesToRead, &bytesRead, nullptr);
		if (bytesToRead == bytesRead) {
		  SetFilePointer(thrEdFile, 16, nullptr, FILE_CURRENT);
		  auto brushColor = COLORREF {};
		  ReadFile(thrEdFile, &brushColor, sizeof(brushColor), &bytesRead, nullptr);
		  auto colors = std::vector<COLORREF> {};
		  colors.resize(COLORCNT);
		  wrap::ReadFile(thrEdFile,
		                 colors.data(),
		                 wrap::toUnsigned(colors.size() * sizeof(decltype(colors.back()))),
		                 &bytesRead,
		                 nullptr);
		  // NOLINTNEXTLINE(readability-qualified-auto)
		  auto brush = CreateSolidBrush(brushColor);
		  SelectObject(drawItem->hDC, brush);
		  FillRect(drawItem->hDC, &drawItem->rcItem, brush);
		  auto iColor = stitchesToDraw[0].attribute & COLORBTS;
		  // NOLINTNEXTLINE(readability-qualified-auto)
		  auto pen   = wrap::CreatePen(PS_SOLID, PENNWID, colors[iColor]);
		  auto iLine = 0U;
		  for (auto iStitch = 0U; iStitch < stitchHeader.stitchCount; ++iStitch) {
			if ((stitchesToDraw[iStitch].attribute & COLORBTS) == iColor) {
			  lines[iLine++] = {std::lround(stitchesToDraw[iStitch].x * ratio),
			                    std::lround(wrap::toFloat(drawingDestinationSize.y) -
			                                stitchesToDraw[iStitch].y * ratio)};
			}
			else {
			  pen = nuPen(pen, 1, colors[iColor]);
			  SelectObject(drawItem->hDC, pen);
			  wrap::Polyline(drawItem->hDC, lines.data(), iLine);
			  iLine  = 0;
			  iColor = stitchesToDraw[iStitch].attribute & COLORBTS;
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
			auto const bytesToRead =
			    stitchHeader.formCount * wrap::toUnsigned(sizeof(decltype(formListOriginal.back())));
			ReadFile(thrEdFile, formListOriginal.data(), bytesToRead, &bytesRead, nullptr);
			if (bytesRead != bytesToRead) {
			  break;
			}
			std::copy(formListOriginal.cbegin(), formListOriginal.cend(), formList.begin());
		  }
		  else {
			auto inFormList = std::vector<FRMHEDOUT> {};
			inFormList.resize(stitchHeader.formCount);
			auto const bytesToRead =
			    stitchHeader.formCount * wrap::toUnsigned(sizeof(decltype(inFormList.back())));
			wrap::ReadFile(thrEdFile, inFormList.data(), bytesToRead, &bytesRead, nullptr);
			if (bytesRead != bytesToRead) {
			  break;
			}
			std::copy(inFormList.cbegin(), inFormList.cend(), formList.begin());
		  }
		  auto const bytesToRead =
		      stitchHeader.vertexCount * wrap::toUnsigned(sizeof(decltype(vertexList.back())));
		  ReadFile(thrEdFile, vertexList.data(), bytesToRead, &bytesRead, nullptr);
		  if (bytesRead != bytesToRead) {
			break;
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
			  lines[iVertexInForm] = {std::lround(vertexList[iVertex].x * ratio),
			                          std::lround(wrap::toFloat(drawingDestinationSize.y) -
			                                      vertexList[iVertex++].y * ratio)};
			}
			lines[formList[iForm].vertexCount] = {
			    std::lround(vertexList[iLine].x * ratio),
			    std::lround(wrap::toFloat(drawingDestinationSize.y) - vertexList[iLine].y * ratio)};
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
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
			auto scrollPoint = POINT {0L, 0L}; // for scroll bar functions
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
			auto scrollPoint = POINT {0L, 0L}; // for scroll bar functions
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
			pglft();
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
			pgrit();
		  }
		  return 1;
		}
		case SB_THUMBPOSITION: {
		  if (StateMap->test(StateFlag::RUNPAT) || StateMap->test(StateFlag::WASPAT)) {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
			if (reinterpret_cast<HWND>(lParam) == SpeedScrollBar) {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
			  auto const position = HIWORD(wParam);
			  MovieTimeStep       = MAXDELAY - position;
			  setsped();
			  SetScrollPos(SpeedScrollBar, SB_CTL, position, TRUE);
			}
		  }
		  else {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
			if (reinterpret_cast<HWND>(lParam) == HorizontalScrollBar) {
			  auto const zoomWidth = ZoomRect.right - ZoomRect.left;
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
			  ZoomRect.left        = wrap::toFloat(HIWORD(wParam));
			  ZoomRect.right       = ZoomRect.left + zoomWidth;
			  auto const unzoomedX = wrap::toFloat(UnzoomedRect.x);
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
		  // NOLINTNEXTLINE(hicpp-signed-bitwise)
		  outDebugString(L"default hit in WndProc 1: wParam [{}]\n", LOWORD(wParam));
		  break;
		}
	  }
	  break;
	}
	case WM_VSCROLL: {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  switch (LOWORD(wParam)) {
		case SB_LINEDOWN: {
		  auto scrollPoint = POINT {0L, 0L};
		  scrollPoint.y    = std::lround((ZoomRect.top - ZoomRect.bottom) * LINSCROL);
		  if (scrollPoint.y == 0) {
			scrollPoint.y = 1;
		  }
		  rshft(scrollPoint);
		  return 1;
		}
		case SB_LINEUP: {
		  auto scrollPoint = POINT {0L, 0L};
		  scrollPoint.y    = std::lround(-(ZoomRect.top - ZoomRect.bottom) * LINSCROL);
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
		  ZoomRect.top    = wrap::toFloat(UnzoomedRect.y) - wrap::toFloat(HIWORD(wParam));
		  ZoomRect.bottom = ZoomRect.top - zoomHeight;
		  if (ZoomRect.bottom < 0) {
			ZoomRect.bottom = 0.0F;
			ZoomRect.top    = zoomHeight;
		  }
		  StateMap->set(StateFlag::RESTCH);
		  return 1;
		}
		default: {
		  // NOLINTNEXTLINE(hicpp-signed-bitwise)
		  outDebugString(L"default hit in WndProc 2: wParam [{}]\n", LOWORD(wParam));
		  break;
		}
	  }
	  break;
	}
	case WM_DRAWITEM: {
// owner draw windows
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[ActiveColor]);
		  SetBkColor(DrawItem->hDC, UserColor[ActiveColor]);
		}
		else {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, GetSysColorBrush(COLOR_BTNFACE));
		}
		if (StateMap->test(StateFlag::TXTRED)) {
		  texture::writeScreenWidth(position);
		}
		else {
		  wrap::textOut(DrawItem->hDC,
		                position,
		                1,
		                StringTable->operator[](STR_PIKOL).c_str(),
		                wrap::toUnsigned(StringTable->operator[](STR_PIKOL).size()));
		}
		return 1;
	  }
	  if (StateMap->test(StateFlag::WASTRAC)) {
		trace::wasTrace();
		return 1;
	  }
	  for (auto iColor = 0U; iColor < COLORCNT; ++iColor) {
		if (DrawItem->hwndItem == DefaultColorWin->operator[](iColor)) {
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, DefaultColorBrush[iColor]);
		  if (DisplayedColorBitmap.test(iColor)) {
			SetBkColor(DrawItem->hDC, DefaultColors[iColor]);
			SetTextColor(DrawItem->hDC, defTxt(iColor));
			auto const colorNum = std::wstring(fmt::format(L"{}", iColor + 1U));
			auto       textSize = SIZE {0L, 0L};
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
		  FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[iColor]);
		  if (iColor == ActiveColor) {
			SelectObject(DrawItem->hDC, CrossPen);
			SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
			auto line = std::array<POINT, 2> {};
			line[0].x = line[1].x = DrawItem->rcItem.right / 2;
			line[0].y             = 0;
			line[1].y             = DrawItem->rcItem.bottom;
			wrap::Polyline(DrawItem->hDC, line.data(), wrap::toUnsigned(line.size()));
			line[0].y = line[1].y = DrawItem->rcItem.bottom / 2;
			line[0].x             = 0;
			line[1].x             = DrawItem->rcItem.right;
			wrap::Polyline(DrawItem->hDC, line.data(), wrap::toUnsigned(line.size()));
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		  }
		  return 1;
		}
	  }
	  if (StateMap->test(StateFlag::BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {
		if (StateMap->test(StateFlag::THUMSHO)) {
		  for (auto iThumb = 0U; iThumb < 4; ++iThumb) {
			if (iThumb < ThumbnailDisplayCount && DrawItem->hwndItem == BackupViewer[iThumb]) {
			  ritbak(Thumbnails->operator[](ThumbnailsSelected[iThumb]).data(), DrawItem);
			  rthumnam(iThumb);
			  return 1;
			}
		  }
		}
		else {
		  for (auto iVersion = wchar_t {}; iVersion < OLDVER; ++iVersion) {
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
			auto       screenCenterOffset = LONG {};
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
		  outDebugString(L"default hit in WndProc 3: wParam [{}]\n", wParam);
		  break;
		}
	  }
	  GetClientRect(p_hWnd, &ThredWindowRect);
	  thred::movStch();
	  if (StateMap->test(StateFlag::ZUMED)) {
		auto const bRatio = wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
		auto const adjustedWidth = wrap::toFloat(StitchWindowClientRect.right) / bRatio;
		auto const unzoomedX     = wrap::toFloat(UnzoomedRect.x);
		if (adjustedWidth + ZoomRect.left > unzoomedX) {
		  ZoomRect.right = unzoomedX;
		  ZoomRect.left  = unzoomedX - adjustedWidth;
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
	  if ((ThredWindowRect.right - ThredWindowRect.left) < 20) {
		ThredWindowRect.left  = 0;
		ThredWindowRect.right = 300;
	  }
	  if ((ThredWindowRect.bottom - ThredWindowRect.top) < 20) {
		ThredWindowRect.top    = 0;
		ThredWindowRect.bottom = 300;
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
	  dun();
	  if (StateMap->test(StateFlag::SAVEX)) {
		return 1;
	  }
	  break;
	}
	default: {
	  // outDebugString(L"default hit in WndProc 4: message [{}]\n", message);
	  break;
	}
  }
  return DefWindowProc(p_hWnd, message, wParam, lParam);
}

void thred::internal::sachk() {
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto const& form = FormList->operator[](iForm);
	if (form.type == SAT && (form.satinGuideCount != 0U)) {
	  auto guideIt = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		if (guideIt->start > form.vertexCount || guideIt->finish > form.vertexCount) {
		  auto const bakclo   = ClosestFormToCursor;
		  ClosestFormToCursor = iForm;
		  satin::delsac(iForm);
		  ClosestFormToCursor = bakclo;
		}
		++guideIt;
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
  auto const hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if (SUCCEEDED(hr)) {
	ThrEdInstance = hInstance;
	auto wc = WNDCLASSEX {0U, 0U, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	wc.cbSize = sizeof(WNDCLASSEX);
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	wc.style       = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = thi::WndProc;
	wc.cbClsExtra  = 0;
	wc.cbWndExtra  = 0;
	wc.hInstance   = ThrEdInstance;
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts
	wc.hIcon = gsl::narrow_cast<HICON>(
	    LoadImage(ThrEdInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_SHARED)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
	wc.hCursor = nullptr; //  set the cursor to null as the cursor changes in the window:
	                      //  https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-setcursor
	wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = L"thred";
	wc.hIconSm       = nullptr;

#if HIGHDPI
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,readability-qualified-auto)
	auto previousDpiContext = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
#endif

	if (RegisterClassEx(&wc)) {
	  auto  private_AllItemsRect              = fRECTANGLE {};
	  auto  private_AngledFormVertices        = std::vector<fPOINT> {};
	  auto  private_AuxName                   = fs::path {};
	  auto  private_BSequence                 = std::vector<BSEQPNT> {};
	  auto  private_BalaradName0              = fs::path {};
	  auto  private_BalaradName1              = fs::path {};
	  auto  private_BalaradName2              = fs::path {};
	  auto  private_ButtonWin                 = std::vector<HWND> {};
	  auto  private_ClipBuffer                = std::vector<fPOINTATTR> {};
	  auto  private_ClipPoints                = std::vector<fPOINT> {};
	  auto  private_ColorFileName             = fs::path {};
	  auto  private_ColorChangeTable          = std::vector<COLCHNG> {};
	  auto  private_DefaultColorWin           = std::vector<HWND> {};
	  auto  private_DefaultDirectory          = fs::path {};
	  auto  private_DesignerName              = std::wstring {};
	  auto  private_ExtendedHeader            = STREX {};
	  auto  private_FormAngles                = std::vector<float> {};
	  auto  private_FormControlPoints         = std::vector<POINT> {};
	  auto  private_FormLines                 = std::vector<POINT> {};
	  auto  private_FormList                  = std::vector<FRMHED> {};
	  auto  private_FormOnOff                 = std::wstring {};
	  auto  private_FormVertices              = std::vector<fPOINT> {};
	  auto  private_FormVerticesAsLine        = std::vector<POINT> {};
	  auto  private_HomeDirectory             = fs::path {};
	  auto  private_IniFileName               = fs::path {};
	  auto  private_InsidePointList           = std::vector<fPOINT> {};
	  auto  private_InterleaveSequence        = std::vector<fPOINT> {};
	  auto  private_InterleaveSequenceIndices = std::vector<INSREC> {};
	  auto  private_LabelWindow               = std::vector<HWND> {};
	  auto* formOnOff                         = private_FormOnOff.data();

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
#endif              /* WINVER >= 0x0500 */
	  };
	  auto private_OSequence                 = std::vector<fPOINT> {};
	  auto private_OutsidePointList          = std::vector<fPOINT> {};
	  auto private_PreviousNames             = std::vector<fs::path> {};
	  auto private_RGBFileName               = fs::path {};
	  auto private_RubberBandLine            = std::vector<POINT> {};
	  auto private_SatinGuides               = std::vector<SATCON> {};
	  auto private_SearchLine                = std::vector<POINT> {};
	  auto private_SelectedFormList          = std::vector<uint32_t> {};
	  auto private_SelectedFormsLine         = std::vector<POINT> {};
	  auto private_SelectedPointsLine        = std::vector<POINT> {};
	  auto private_SelectedTexturePointsList = std::vector<uint32_t> {};
	  auto private_StateMap                  = EnumMap<StateFlag> {0};
	  auto private_StitchBuffer              = std::vector<fPOINTATTR> {};
	  auto private_StringTable               = std::vector<std::wstring> {};
	  auto private_TempPolygon               = std::vector<fPOINT> {};
	  auto private_TempTexturePoints         = std::vector<TXPNT> {};
	  auto private_TextureHistory            = std::array<TXHST, ITXBUFSZ> {};
	  auto private_TexturePointsBuffer       = std::vector<TXPNT> {};
	  auto private_ThrName                   = fs::path {};
	  auto private_Thumbnails                = std::vector<std::wstring> {};
	  auto private_TracedEdges               = boost::dynamic_bitset<> {};
	  auto private_TracedMap                 = boost::dynamic_bitset<> {};
	  auto private_UndoBuffer                = std::vector<std::unique_ptr<uint32_t[]>> {};
	  auto private_UserBMPFileName           = fs::path {};
	  auto private_UserColorWin              = std::vector<HWND> {};
	  auto private_UserFlagMap               = EnumMap<UserFlag> {0};
	  auto private_ValueWindow               = std::vector<HWND> {};
	  auto private_VersionNames              = std::vector<fs::path> {};
	  auto private_WorkingFileName           = fs::path {};
	  auto private_textureInputBuffer        = std::wstring {};

	  private_DefaultColorWin.resize(COLORCNT);
	  private_FormControlPoints.resize(OUTPNTS);
	  private_LabelWindow.resize(LASTLIN);
	  private_RubberBandLine.resize(3U);
	  private_SelectedFormsLine.resize(OUTPNTS);
	  private_SelectedPointsLine.resize(OUTPNTS);
	  private_StringTable.resize(STR_LEN);
	  private_UndoBuffer.resize(UNDOLEN);
	  private_UserColorWin.resize(COLORCNT);
	  private_ValueWindow.resize(LASTLIN);
	  private_PreviousNames.reserve(OLDNUM);
	  private_VersionNames.reserve(OLDVER);
	  for (auto iVersion = 0U; iVersion < OLDNUM; ++iVersion) {
		private_PreviousNames.emplace_back(L"");
	  }
	  for (auto iVersion = wchar_t {}; iVersion < OLDVER; ++iVersion) {
		private_VersionNames.emplace_back(L"");
	  }
	  AllItemsRect              = &private_AllItemsRect;
	  AngledFormVertices        = &private_AngledFormVertices;
	  AuxName                   = &private_AuxName;
	  BSequence                 = &private_BSequence;
	  BalaradName0              = &private_BalaradName0;
	  BalaradName1              = &private_BalaradName1;
	  BalaradName2              = &private_BalaradName2;
	  ButtonWin                 = &private_ButtonWin;
	  ClipBuffer                = &private_ClipBuffer;
	  ClipPoints                = &private_ClipPoints;
	  ColorChangeTable          = &private_ColorChangeTable;
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
	  RubberBandLine            = &private_RubberBandLine;
	  SatinGuides               = &private_SatinGuides;
	  SearchLine                = &private_SearchLine;
	  SelectedFormList          = &private_SelectedFormList;
	  SelectedFormsLine         = &private_SelectedFormsLine;
	  SelectedPointsLine        = &private_SelectedPointsLine;
	  StateMap                  = &private_StateMap;
	  StitchBuffer              = &private_StitchBuffer;
	  StringTable               = &private_StringTable;
	  TempPolygon               = &private_TempPolygon;
	  TextureHistory            = &private_TextureHistory;
	  TextureInputBuffer        = &private_textureInputBuffer;
	  TexturePointsBuffer       = &private_TexturePointsBuffer;
	  ThrName                   = &private_ThrName;
	  Thumbnails                = &private_Thumbnails;
	  TracedEdges               = &private_TracedEdges;
	  TracedMap                 = &private_TracedMap;
	  UndoBuffer                = &private_UndoBuffer;
	  UserColorWin              = &private_UserColorWin;
	  UserFlagMap               = &private_UserFlagMap;
	  ValueWindow               = &private_ValueWindow;
	  VersionNames              = &private_VersionNames;
	  WorkingFileName           = &private_WorkingFileName;
	  bitmap::setUBfilename(&private_UserBMPFileName);
	  DST::setColFilename(&private_ColorFileName);
	  DST::setRGBFilename(&private_RGBFileName);
	  texture::initTextures(&private_TempTexturePoints, &private_SelectedTexturePointsList);
	  thi::redini();
	  CreateParams createParams {};
	  createParams.bEnableNonClientDpiScaling = TRUE;
	  auto private_ScrollSize                 = SCROLSIZ;
	  ScrollSize                              = &private_ScrollSize;
	  auto private_ColorBarSize               = COLSIZ;
	  ColorBarSize                            = &private_ColorBarSize;
	  if (IniFile.initialWindowCoords.right != 0) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
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
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
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
	  auto private_DPI     = gsl::narrow<int32_t>(GetDpiForWindow(ThrEdWindow));
	  screenDPI            = &private_DPI;
	  private_ScrollSize   = MulDiv(private_ScrollSize, *screenDPI, STDDPI);
	  private_ColorBarSize = MulDiv(private_ColorBarSize, *screenDPI, STDDPI);
	  thi::init();
	  if (UserFlagMap->test(UserFlag::SAVMAX)) {
		ShowWindow(ThrEdWindow, SW_SHOWMAXIMIZED);
	  }
	  else {
		ShowWindow(ThrEdWindow, SW_SHOW);
	  }
	  if (DesignerName->empty()) {
		auto designerBuffer = std::wstring {};
		displayText::loadString(*DesignerName, IDS_UNAM);
		thi::getdes();
	  }
	  auto xyRatio        = 1.0F; // expand form aspect ratio
	  auto rotationAngle  = 0.0F;
	  auto rotationCenter = fPOINT {};
	  auto textureForm    = FRMHED {};
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
  LocalFree(ArgList);
  if (flag) {
	return 0;
  }
  return -1;
}
