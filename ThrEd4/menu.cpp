// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "backup.h"
#include "bitmap.h"
#include "displayText.h"
#include "EnumMap.h"
#include "form.h"
#include "formClip.h"
#include "formForms.h"
#include "globals.h"
#include "hlp.h"
#include "iniFile.h"
#include "keys.h"
#include "menu.h"
#include "point.h"
#include "repair.h"
#include "Resources/resource.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "trace.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/pointers"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <fileapi.h>
#include <handleapi.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <windef.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// menu internal namespace
namespace {
auto BorderFillMenu = gsl::narrow_cast<HMENU>(nullptr); // border fill submenu
auto EditMenu       = gsl::narrow_cast<HMENU>(nullptr); // edit submenu
auto FileMenu       = gsl::narrow_cast<HMENU>(nullptr); // file submenu
auto FillMenu       = gsl::narrow_cast<HMENU>(nullptr); // fill submenu
auto ViewMenu       = gsl::narrow_cast<HMENU>(nullptr); // view submenu
auto ViewSetMenu    = gsl::narrow_cast<HMENU>(nullptr); // view/set

constexpr auto GRDHI  = uint32_t {0xffffffU}; // grid high color
constexpr auto GRDMED = uint32_t {0x404040U}; // grid medium color
constexpr auto GRDDEF = uint32_t {0x202020U}; // grid default color
constexpr auto GRDRED = uint32_t {0xff2020U}; // grid red color
constexpr auto GRDBLU = uint32_t {0x20ff20U}; // grid green color
constexpr auto GRDGRN = uint32_t {0x2020ffU}; // grid blue color

// Definitions
void fil2sel(uint32_t stat);
void frmcursel(uint32_t cursorType);
void getfrmbox();
void getfrmpix();
void getnpix();
void getstpix();
void hidknot();
void nedof();
void nedon();
void pcsbsavof();
void pcsbsavon();
void rotauxsel(uint32_t stat);
void shoknot();
void vuselthr();

// Functions
void fil2sel(uint32_t const stat) {
  Instance->UserFlagMap.set(UserFlag::FIL2OF);
  if (stat != 0U) {
	Instance->UserFlagMap.reset(UserFlag::FIL2OF);
  }
  menu::fil2men();
}

void frmcursel(uint32_t const cursorType) {
  Instance->UserFlagMap.set(UserFlag::FRMX);
  if (cursorType == 0U) {
	Instance->UserFlagMap.reset(UserFlag::FRMX);
  }
  menu::frmcurmen();
}

void getfrmbox() {
  displayText::showMessage(IDS_FRMBOX, IniFile.formBoxSizePixels);
  Instance->StateMap.set(StateFlag::NUMIN);
  Instance->StateMap.set(StateFlag::FRMBOXIN);
  displayText::numWnd();
}

void getfrmpix() {
  displayText::showMessage(IDS_FRMP, IniFile.formVertexSizePixels);
  Instance->StateMap.set(StateFlag::NUMIN);
  Instance->StateMap.set(StateFlag::FRMPXIN);
  displayText::numWnd();
}

void getnpix() {
  displayText::showMessage(IDS_NUDG, IniFile.nudgePixels);
  Instance->StateMap.set(StateFlag::NUMIN);
  Instance->StateMap.set(StateFlag::PIXIN);
  displayText::numWnd();
}

void getstpix() {
  displayText::showMessage(IDS_STCHP, IniFile.stitchSizePixels);
  Instance->StateMap.set(StateFlag::NUMIN);
  Instance->StateMap.set(StateFlag::STPXIN);
  displayText::numWnd();
}

void hidknot() {
  Instance->UserFlagMap.set(UserFlag::KNOTOF);
  menu::knotmen();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void nedof() {
  Instance->UserFlagMap.set(UserFlag::NEDOF);
  menu::nedmen();
}

void nedon() {
  Instance->UserFlagMap.reset(UserFlag::NEDOF);
  menu::nedmen();
}

void pcsbsavof() {
  Instance->UserFlagMap.set(UserFlag::BSAVOF);
  menu::bsavmen();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void pcsbsavon() {
  Instance->UserFlagMap.reset(UserFlag::BSAVOF);
  menu::bsavmen();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void rotauxsel(uint32_t const stat) {
  Instance->UserFlagMap.set(UserFlag::ROTAUX);
  if (stat == 0U) {
	Instance->UserFlagMap.reset(UserFlag::ROTAUX);
  }
  menu::rotauxmen();
}

void shoknot() {
  Instance->UserFlagMap.reset(UserFlag::KNOTOF);
  menu::knotmen();
  Instance->StateMap.set(StateFlag::RESTCH);
}

void vuselthr() {
  if ((GetMenuState(ViewMenu, ID_VUSELTHRDS, MF_BYCOMMAND) & MF_CHECKED) != 0U) {
	CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
	Instance->StateMap.reset(StateFlag::COL);
  }
  else {
	CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_CHECKED);
	Instance->StateMap.set(StateFlag::COL);
  }
  Instance->StateMap.set(StateFlag::RESTCH);
}
} // namespace

void menu::setpcs() {
  IniFile.auxFileType = AUXPCS;
  auxmen();
}

#if PESACT
void menu::setpes() {
  IniFile.auxFileType = AUXPES;
  auxmen();
}
#endif

void menu::setdst() {
  IniFile.auxFileType = AUXDST;
  auxmen();
}

void menu::disableRedo() {
  if (Instance->StateMap.testAndReset(StateFlag::REDUSHO)) {
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
	Instance->StateMap.set(StateFlag::DUMEN);
  }
}

void menu::enableRedo() {
  if (!Instance->StateMap.testAndSet(StateFlag::REDUSHO)) {
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
	Instance->StateMap.set(StateFlag::DUMEN);
  }
}

void menu::disableUndo() {
  if (Instance->StateMap.testAndReset(StateFlag::UNDUSHO)) {
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	Instance->StateMap.set(StateFlag::DUMEN);
  }
}

void menu::enableUndo() {
  if (!Instance->StateMap.testAndSet(StateFlag::UNDUSHO)) {
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
	Instance->StateMap.set(StateFlag::DUMEN);
  }
}

void menu::ladj() {
  for (auto const& iLayer : LAYRLIST) {
	if (iLayer.value == ActiveLayer) {
	  EnableMenuItem(MainMenu, iLayer.value + M_ALL, MF_BYPOSITION | MF_GRAYED);
	}
	else {
	  EnableMenuItem(MainMenu, iLayer.value + M_ALL, MF_BYPOSITION | MF_ENABLED);
	}
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::flipFormOnOff() {
  if (Instance->StateMap.testAndFlip(StateFlag::FRMOF)) {
	Instance->FormOnOff.assign(displayText::loadStr(IDS_FRMPLUS));
  }
  else {
	Instance->FormOnOff.assign(displayText::loadStr(IDS_FRMINUS));
	Instance->StateMap.reset(StateFlag::FORMSEL);
	Instance->StateMap.reset(StateFlag::FORMIN);
	Instance->StateMap.reset(StateFlag::MOVFRM);
	Instance->StateMap.reset(StateFlag::FRMPMOV);
	Instance->StateMap.reset(StateFlag::FRMOV);
	Instance->StateMap.reset(StateFlag::SATIN);
	Instance->StateMap.reset(StateFlag::SATPNT);
	Instance->StateMap.reset(StateFlag::SATCNKT);
	Instance->StateMap.reset(StateFlag::FENDIN);
	Instance->StateMap.reset(StateFlag::DELFRM);
	Instance->StateMap.reset(StateFlag::FRMPSEL);
	Instance->StateMap.reset(StateFlag::INSFRM);
  }
  SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, std::addressof(Instance->MenuInfo));
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::resetFormOnOff() {
  Instance->FormOnOff.assign(displayText::loadStr(IDS_FRMPLUS));
  SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, std::addressof(Instance->MenuInfo));
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::qchk() {
  if (Instance->UserFlagMap.test(UserFlag::MARQ)) {
	CheckMenuItem(MainMenu, ID_MARKESC, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_MARKQ, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_MARKESC, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_MARKQ, MF_CHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::nedmen() {
  if (Instance->UserFlagMap.test(UserFlag::NEDOF)) {
	CheckMenuItem(MainMenu, ID_SETNEDL, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_RSTNEDL, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_SETNEDL, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_RSTNEDL, MF_UNCHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::knotmen() {
  if (Instance->UserFlagMap.test(UserFlag::KNOTOF)) {
	CheckMenuItem(MainMenu, ID_KNOTON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_KNOTOF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_KNOTON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_KNOTOF, MF_UNCHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::bsavmen() {
  if (Instance->UserFlagMap.test(UserFlag::BSAVOF)) {
	CheckMenuItem(MainMenu, ID_BSAVON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_BSAVOF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_BSAVON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_BSAVOF, MF_UNCHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::linbmen() {
  if (Instance->UserFlagMap.test(UserFlag::LINSPAC)) {
	CheckMenuItem(MainMenu, ID_LINBEXACT, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_LINBEVEN, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_LINBEXACT, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_LINBEVEN, MF_UNCHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::wrnmen() {
  auto code = UINT {MF_CHECKED};
  if (Instance->UserFlagMap.test(UserFlag::WRNOF)) {
	code = MF_UNCHECKED;
  }
  CheckMenuItem(MainMenu, ID_WARNOF, code);
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::auxmen() {
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
	  auxMsg.assign(displayText::format(IDS_AUXTXT, L"DST"));
	  CheckMenuItem(MainMenu, ID_AUXDST, MF_CHECKED);
	  break;
	}
	case AUXPES:
#if PESACT
	{
	  auxMsg.assign(displayText::format(IDS_AUXTXT, L"PES"));
	  CheckMenuItem(MainMenu, ID_AUXPES, MF_CHECKED);
	  break;
	}
#else
	{
	  IniFile.auxFileType = AUXPCS;
	}
#endif
	default: {
	  auxMsg.assign(displayText::format(IDS_AUXTXT, L"PCS"));
	  CheckMenuItem(MainMenu, ID_AUXPCS, MF_CHECKED);
	}
  }
#pragma warning(suppress : 26492) // type.3 Don't use const_cast to cast away const or volatile NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
  filinfo.dwTypeData = const_cast<LPWSTR>(auxMsg.c_str());
  SetMenuItemInfo(FileMenu, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::redfils(std::array<uint32_t, OLDNUM>& lruMenuId,
                   gsl::not_null<std::vector<fs::path>*> const        previousNames) {
  auto findData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
  for (auto const& iLRU : lruMenuId) {
	if (GetMenuState(FileMenu, iLRU, MF_BYCOMMAND) != gsl::narrow_cast<UINT>(-1)) {
	  DeleteMenu(FileMenu, iLRU, MF_BYCOMMAND);
	}
  }
  auto previousName = previousNames->begin();
  for (auto const& iLRU : lruMenuId) {
	if (!previousName->empty()) {
	  if (Instance->StateMap.test(StateFlag::SAVAS)) {
		AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, iLRU, previousName->wstring().c_str());
	  }
	  else {
		// NOLINTNEXTLINE(readability-qualified-auto)
		if (auto const fileHandle = FindFirstFile(previousName->c_str(), &findData);
		    fileHandle == INVALID_HANDLE_VALUE) {
		  previousName->clear();
		}
		else {
		  AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, iLRU, previousName->wstring().c_str());
		  FindClose(fileHandle);
		}
	  }
	}
	++previousName;
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::init() noexcept {
  FillMenu       = GetSubMenu(MainMenu, M_FILL);
  FileMenu       = GetSubMenu(MainMenu, M_FILE);
  EditMenu       = GetSubMenu(MainMenu, M_EDIT);
  BorderFillMenu = GetSubMenu(FillMenu, MFIL_BORD);
  ViewMenu       = GetSubMenu(MainMenu, M_VIEW);
  ViewSetMenu    = GetSubMenu(ViewMenu, MVW_SET);
}

void menu::resetThreadView() {
  CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
  Instance->StateMap.reset(StateFlag::COL);
  CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::vuthrds() {
  if ((GetMenuState(ViewMenu, ID_VUTHRDS, MF_BYCOMMAND) & MF_CHECKED) != 0U) {
	CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
	Instance->StateMap.reset(StateFlag::THRDS);
  }
  else {
	CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_CHECKED);
	Instance->StateMap.set(StateFlag::THRDS);
  }
  Instance->StateMap.set(StateFlag::RESTCH);
}

void menu::setGridCols(const COLORREF& color) {
  class GRID_COL
  {
public:
	uint32_t id {};
	uint32_t col {};
  };

  static constexpr auto GC_HIGH    = GRID_COL {.id = ID_GRDHI, .col = GRDHI};
  static constexpr auto GC_MEDIUM  = GRID_COL {.id = ID_GRDMED, .col = GRDMED};
  static constexpr auto GC_DEFAULT = GRID_COL {.id = ID_GRDEF, .col = GRDDEF};
  static constexpr auto GC_RED     = GRID_COL {.id = ID_GRDRED, .col = GRDRED};
  static constexpr auto GC_BLUE    = GRID_COL {.id = ID_GRDBLU, .col = GRDBLU};
  static constexpr auto GC_GREEN   = GRID_COL {.id = ID_GRDGRN, .col = GRDGRN};
  static constexpr auto GRID_CODES = std::array {GC_HIGH, GC_MEDIUM, GC_DEFAULT, GC_RED, GC_BLUE, GC_GREEN};

  for (auto const& gridCode : GRID_CODES) {
	if (color == gridCode.col) {
	  CheckMenuItem(MainMenu, gridCode.id, MF_CHECKED);
	}
	else {
	  CheckMenuItem(MainMenu, gridCode.id, MF_UNCHECKED);
	}
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::duhbit(uint32_t const cod) noexcept {
  CheckMenuItem(MainMenu, ID_HIDBIT, MF_BYCOMMAND | cod);
  CheckMenuItem(MainMenu, ID_HIDBITF, MF_BYCOMMAND | cod);
}

void menu::flipHideBitmap() {
  if (Instance->StateMap.testAndFlip(StateFlag::HIDMAP)) {
	duhbit(MF_UNCHECKED);
  }
  else {
	duhbit(MF_CHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::rotauxmen() {
  if (Instance->UserFlagMap.test(UserFlag::ROTAUX)) {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_CHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::frmcurmen() {
  if (Instance->UserFlagMap.test(UserFlag::FRMX)) {
	CheckMenuItem(MainMenu, ID_FRMX, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FRMX, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_CHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

void menu::fil2men() {
  if (Instance->UserFlagMap.test(UserFlag::FIL2OF)) {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_UNCHECKED);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

namespace {
auto DataCode = std::array<UINT, 4U> {ID_CHKOF, ID_CHKON, ID_CHKREP, ID_CHKREPMSG};
} // namespace

void menu::chkmen() {
  constexpr auto LASTCODE = DataCode.size();
  for (auto iCode = 0U; iCode < LASTCODE; ++iCode) {
	auto const code = iCode == IniFile.dataCheck ? gsl::narrow_cast<UINT>(MF_CHECKED)
	                                             : gsl::narrow_cast<UINT>(MF_UNCHECKED);
	CheckMenuItem(MainMenu, DataCode.at(iCode), code);
  }
  Instance->StateMap.set(StateFlag::DUMEN);
}

auto menu::handleEditMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_KNOTAT: { // edit / Set / Knot at Selected Stitch
	  thred::set1knot();
	  flag = true;
	  break;
	}
	case ID_FRM2COL: { // edit / Set / Form Color to  stitch Color
	  form::col2frm();
	  flag = true;
	  break;
	}
	case ID_SNAP2GRD: { // edit / Snap / to Grid
	  thred::gsnap();
	  flag = true;
	  break;
	}
	case ID_EDIT_SELECTCOLOR: { // edit / Select / Color
	  thred::selCol();
	  flag = true;
	  break;
	}
	case ID_REPAIR: { // edit / Repair Data
	  repair::repar();
	  flag = true;
	  break;
	}
	case ID_FRMIND: { // edit / Form Update / Indent
	  xt::setEdit(IDS_UWLKIND, StateFlag::FSETFIND);
	  flag = true;
	  break;
	}
	case ID_SETSIZ: { // edit / Set / Design Size
	  xt::nudsiz();
	  flag = true;
	  break;
	}
	case ID_FRMHI: { // edit / Form Update / Height
	  xt::setEdit(IDS_HI, StateFlag::FSETFHI);
	  flag = true;
	  break;
	}
	case ID_FRMWID: { // edit / Form Update / Width
	  xt::setEdit(IDS_WID, StateFlag::FSETFWID);
	  flag = true;
	  break;
	}
	case ID_MAXFLEN: { // edit / Form Update / Fill /  Maximum Stitch Length
	  xt::setEdit(IDS_TXT20, StateFlag::FSETFMAX);
	  flag = true;
	  break;
	}
	case ID_MINFLEN: { // edit / Form Update / Fill /  Minimum Stitch Length
	  xt::setEdit(IDS_TXT21, StateFlag::FSETFMIN);
	  flag = true;
	  break;
	}
	case ID_MAXBLEN: { // edit / Form Update / Border /  Maximum Stitch Length
	  xt::setEdit(IDS_TXT22, StateFlag::FSETBMAX);
	  flag = true;
	  break;
	}
	case ID_MINBLEN: { // edit / Form Update / Border /  Minimum Stitch Length
	  xt::setEdit(IDS_TXT23, StateFlag::FSETBMIN);
	  flag = true;
	  break;
	}
	case ID_SETBSPAC: { // edit / Form Update / Border /  Spacing
	  xt::setEdit(IDS_SETFSPAC, StateFlag::FSETBSPAC);
	  flag = true;
	  break;
	}
	case ID_SETBLEN: { // edit / Form Update / Border /  Stitch Length
	  xt::setEdit(IDS_SETFLEN, StateFlag::FSETBLEN);
	  flag = true;
	  break;
	}
	case ID_SETBCOL: { // edit / Form Update / Border /  Color
	  xt::setEdit(IDS_COL, StateFlag::FSETBCOL);
	  flag = true;
	  break;
	}
	case ID_SETFCOL: { // edit / Form Update / Fill /  Color
	  xt::setEdit(IDS_COL, StateFlag::FSETFCOL);
	  flag = true;
	  break;
	}
	case ID_SETUCOL: { // edit / Form Update / Underlay /  Color
	  xt::setEdit(IDS_COL, StateFlag::FSETUCOL);
	  flag = true;
	  break;
	}
	case ID_SETFANG: { // edit / Form Update / Fill /  Angle
	  xt::setEdit(IDS_SETFANG, StateFlag::FSETFANG);
	  flag = true;
	  break;
	}
	case ID_SETFSPAC: { // edit / Form Update / Fill /  Spacing
	  xt::setEdit(IDS_SETFSPAC, StateFlag::FSETFSPAC);
	  flag = true;
	  break;
	}
	case ID_SETFLEN: { // edit / Form Update / Fill /  Stitch Length
	  xt::setEdit(IDS_SETFLEN, StateFlag::FSETFLEN);
	  flag = true;
	  break;
	}
	case ID_SETUANG: { // edit / Form Update / Underlay /  Angle
	  xt::setEdit(IDS_SETUANG, StateFlag::FSETUANG);
	  flag = true;
	  break;
	}
	case ID_SETUSPAC: { // edit / Form Update / Underlay /  Spacing
	  xt::setEdit(IDS_SETUSPAC, StateFlag::FSETUSPAC);
	  flag = true;
	  break;
	}
	case ID_UNDLEN: { // edit / Form Update / Underlay / Stitch Length
	  xt::setEdit(IDS_SETULEN, StateFlag::FSETULEN);
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
	  Instance->StateMap.set(StateFlag::CNV2FTH);
	  satin::ribon();
	  flag = true;
	  break;
	}
	case ID_FTHDEF: { // edit / Set / Feather Defaults
	  thred::dufdef();
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
	  thred::retrac();
	  flag = true;
	  break;
	}
	case ID_DELKNOT: { // edit / Delete / Knots
	  thred::delknot();
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
	  Instance->StateMap.reset(StateFlag::CNTRH);
	  Instance->StateMap.reset(StateFlag::CNTRV);
	  form::cntrx();
	  flag = true;
	  break;
	}
	case ID_CNTRH: { // edit / Center / Horizontal
	  Instance->StateMap.reset(StateFlag::CNTRH);
	  Instance->StateMap.set(StateFlag::CNTRV);
	  form::cntrx();
	  flag = true;
	  break;
	}
	case ID_CNTRV: { // edit / Center / Vertical
	  Instance->StateMap.set(StateFlag::CNTRH);
	  Instance->StateMap.reset(StateFlag::CNTRV);
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
	  thred::pntmrk();
	  flag = true;
	  break;
	}
	case ID_ROTSEG: { // edit / Set / Rotation / Segments
	  thred::rotseg();
	  flag = true;
	  break;
	}
	case ID_ROTMRK: { // edit / Set / Rotation / Angle from Mark
	  thred::rotmrk();
	  flag = true;
	  break;
	}
	case ID_MRKCNTR: { // edit / Set / Zoom Mark at Center
	  thred::dumrk(wrap::toFloat(UnzoomedRect.cx) * HALF, wrap::toFloat(UnzoomedRect.cy) * HALF);
	  Instance->StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_SETROT: { // edit / Set / Rotation / Angle
	  form::setrang();
	  flag = true;
	  break;
	}
	case ID_REMBIG: { // edit / Delete / Large Stitches
	  thred::rembig();
	  flag = true;
	  break;
	}
	case ID_SHRINK: { // edit / Shrink Clipboard Border
	  form::shrnk();
	  flag = true;
	  break;
	}
	case ID_DUPAGAIN: { // edit / Rotate / and Duplicate again
	  Instance->StateMap.reset(StateFlag::ROTCLOCK);
	  form::dupfn(IniFile.rotationAngle);
	  flag = true;
	  break;
	}
	case ID_CLPADJ: { // edit / Set / Range Ends for Clipboard Fills
	  thred::clpadj();
	  flag = true;
	  break;
	}
	case ID_DELTOT: { // edit / Delete / All Forms and Stitches
	  thred::deltot();
	  flag = true;
	  break;
	}
	case ID_RIBON: { // edit / Convert / to Satin Ribbon
	  Instance->StateMap.reset(StateFlag::CNV2FTH);
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
	  thred::selalstch();
	  flag = true;
	  break;
	}
	case ID_UNGRPLO: { // edit / Ungroup / First
	  keys::ungrplo();
	  flag = true;
	  break;
	}
	case ID_UNGRPHI: { // edit / Ungroup / Last
	  keys::ungrphi();
	  flag = true;
	  break;
	}
	case ID_MOVMRK: { // edit / Move / to Mark
	  keys::movmrk();
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
	  keys::setmov();
	  flag = true;
	  break;
	}
	case ID_DELFRE: { // edit / Delete / Free Stitches
	  thred::delfre();
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
	  auto stitchRange = F_POINT {};
	  thred::chkrng(stitchRange);
	  Instance->StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_RTRVCLP: { // edit / Retrieve Clipboard Stitches
	  tfc::rtrclp();
	  flag = true;
	  break;
	}
	case ID_SORT: { // edit / Sort / Auto
	  xt::fsort();
	  flag = true;
	  break;
	}
	case ID_LAYMOV0: { // edit / Move to Layer / 0
	  constexpr auto TOLAYER0 = 0U;
	  form::movlayr(TOLAYER0);
	  flag = true;
	  break;
	}
	case ID_LAYMOV1: { // edit / Move to Layer / 1
	  constexpr auto TOLAYER1 = 1U;
	  form::movlayr(TOLAYER1);
	  flag = true;
	  break;
	}
	case ID_LAYMOV2: { // edit / Move to Layer / 2
	  constexpr auto TOLAYER2 = 2U;
	  form::movlayr(TOLAYER2);
	  flag = true;
	  break;
	}
	case ID_LAYMOV3: { // edit / Move to Layer / 3
	  constexpr auto TOLAYER3 = 3U;
	  form::movlayr(TOLAYER3);
	  break;
	}
	case ID_LAYMOV4: { // edit / Move to Layer / 4
	  constexpr auto TOLAYER4 = 4U;
	  form::movlayr(TOLAYER4);
	  flag = true;
	  break;
	}
	case ID_LAYCPY0: { // edit / Copy to Layer / 0
	  constexpr auto TOLAYER0 = 0U;
	  form::cpylayr(TOLAYER0);
	  flag = true;
	  break;
	}
	case ID_LAYCPY1: { // edit / Copy to Layer / 1
	  constexpr auto TOLAYER1 = 1U;
	  form::cpylayr(TOLAYER1);
	  flag = true;
	  break;
	}
	case ID_LAYCPY2: { // edit / Copy to Layer / 2
	  constexpr auto TOLAYER2 = 2U;
	  form::cpylayr(TOLAYER2);
	  flag = true;
	  break;
	}
	case ID_LAYCPY3: { // edit / Copy to Layer / 3
	  constexpr auto TOLAYER3 = 3U;
	  form::cpylayr(TOLAYER3);
	  flag = true;
	  break;
	}
	case ID_LAYCPY4: { // edit / Copy to Layer / 4
	  constexpr auto TOLAYER4 = 4U;
	  form::cpylayr(TOLAYER4);
	  flag = true;
	  break;
	}
	case ID_ROTDUP: { // edit / Rotate / and Duplicate
	  Instance->StateMap.reset(StateFlag::ROTCLOCK);
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
	  thred::rstAll();
	  Instance->StateMap.set(StateFlag::RESTCH);
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
	  thred::mv2b();
	  flag = true;
	  break;
	}
	case ID_MV2FRNT: { // edit / move / to Start
	  thred::mv2f();
	  flag = true;
	  break;
	}
	case ID_DELETE: { // edit / Delete / Selected
	  thred::delet();
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
	  thred::delstch();
	  flag = true;
	  break;
	}
	case ID_KNOTS: { // edit / Set / Knots
	  thred::setknots();
	  flag = true;
	  break;
	}
	case ID_REMZERO: { // edit / Delete / Small Stitches
	  thred::duzero();
	  flag = true;
	  break;
	}
	case ID_EDIT_RESET_COL: { // edit / Reset Colors
	  thred::resetColors();
	  flag = true;
	  break;
	}
	case ID_DESNAM: { // edit / Set / Designer Name
	  thred::getdes();
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

auto menu::handleViewMenu(WORD const& wParameter) -> bool {
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
	  thred::bakmrk();
	  flag = true;
	  break;
	}
	case ID_MARKESC: { // view / Set / Retrieve Mark / Escape
	  Instance->UserFlagMap.set(UserFlag::MARQ);
	  qchk();
	  flag = true;
	  break;
	}
	case ID_MARKQ: { // view / Set / Retrieve Mark / Q
	  Instance->UserFlagMap.reset(UserFlag::MARQ);
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
	  thred::setgrd(GRDHI);
	  flag = true;
	  break;
	}
	case ID_GRDMED: { // view / Set / Grid Mask / Medium
	  thred::setgrd(GRDMED);
	  flag = true;
	  break;
	}
	case ID_GRDEF: { // view / Set / Grid Mask / Default
	  thred::setgrd(GRDDEF);
	  flag = true;
	  break;
	}
	case ID_GRDRED: { // view / Set / Grid Mask / UnRed
	  thred::setgrd(GRDRED);
	  flag = true;
	  break;
	}
	case ID_GRDBLU: { // view / Set / Grid Mask / UnBlue
	  thred::setgrd(GRDBLU);
	  flag = true;
	  break;
	}
	case ID_GRDGRN: { // view / Set / Grid Mask / UnGreen
	  thred::setgrd(GRDGRN);
	  flag = true;
	  break;
	}
	case ID_LINBEXACT: { // view / Set / Line Border Spacing / Exact
	  Instance->UserFlagMap.reset(UserFlag::LINSPAC);
	  linbmen();
	  flag = true;
	  break;
	}
	case ID_LINBEVEN: { // view / Set / Line Border Spacing / Even
	  Instance->UserFlagMap.set(UserFlag::LINSPAC);
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
	  thred::defpref();
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
	  displayText::tabmsg(IDS_CPYRIT, false);
	  flag = true;
	  break;
	}
	case ID_DESIZ: { // view / Design Information
	  keys::desiz();
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
	  thred::vubak();
	  flag = true;
	  break;
	}
	case ID_SETAP: { // view / Set / Applique Color
	  form::setap();
	  flag = true;
	  break;
	}
	case ID_RUNPAT: { // view / Movie
	  thred::movi();
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
	  thred::setThreadSize();
	  flag = true;
	  break;
	}
	case ID_SIZ30: { // view / Thread Size / 30
	  thred::viewThread30();
	  flag = true;
	  break;
	}
	case ID_SIZ40: { // view / Thread Size / 40
	  thred::viewThread40();
	  flag = true;
	  break;
	}
	case ID_SIZ60: { // view / Thread Size / 60
	  thred::viewThread60();
	  flag = true;
	  break;
	}
	case ID_VIEW_STCHBAK: { // view / Set / Background Color
	  thred::setBackGroundColor();
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

auto menu::handleFileMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_CLOSE: { // file / Close
	  thred::filclos();
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
	  thred::fileLock();
	  flag = true;
	  break;
	}
	case ID_OVRLAY: { // file / Overlay
	  thred::ovrlay();
	  flag = true;
	  break;
	}
	case ID_INSFIL: { // file / Insert
	  auto fileName = fs::path {};
	  thred::insfil(fileName);
	  flag = true;
	  break;
	}
	case ID_THUM: { // file / Thumbnails
	  thred::thumnail();
	  flag = true;
	  break;
	}
	case ID_PURGDIR: { // file / Delete Backups / All backups in the selected directory
	  thred::purgdir();
	  flag = true;
	  break;
	}
	case ID_PURG: { // file / Delete Backups / Backups for the selected file
	  thred::purg();
	  flag = true;
	  break;
	}
	case ID_OPNPCD: { // file / Open Auxiliary file
	  thred::openAuxFile();
	  flag = true;
	  break;
	}
	case ID_HIDBITF: { // file / Hide Bitmap
	  thred::hidbit();
	  flag = true;
	  break;
	}
	case ID_LODBIT: { // file / Load Bitmap
	  thred::openBitMapFile();
	  flag = true;
	  break;
	}
	case ID_FILE_OPEN1: { // file / Open
	  thred::fop();
	  flag = true;
	  break;
	}
	case ID_FILE_SAVE3: { // file / Save As
	  thred::colchk();
	  thred::savAs();
	  flag = true;
	  break;
	}
	case ID_FILE_SAVE2: { // file / Save
	  thred::colchk();
	  thred::save();
	  flag = true;
	  break;
	}
	case ID_FILE_NEW1: { // file / New
	  thred::openNewFile();
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

auto menu::handleFillMenu(WORD const& wParameter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_BOLD: { // fill / Border / Bean
	  if (Instance->StateMap.test(StateFlag::FORMSEL) || !Instance->SelectedFormList.empty()) {
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
	  Instance->StateMap.set(StateFlag::LINCHN);
	  form::chain();
	  flag = true;
	  break;
	}
	case ID_OPNCHN: { // fill / Border / Open chain
	  Instance->StateMap.reset(StateFlag::LINCHN);
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
	  if (Instance->StateMap.test(StateFlag::FORMSEL) || !Instance->SelectedFormList.empty()) {
		thred::savdo();
	  }
	  form::horclp();
	  flag = true;
	  break;
	}
	case ID_ANGCLP: { // fill / Clipboard / Angle
	  if (Instance->StateMap.test(StateFlag::FORMSEL) || !Instance->SelectedFormList.empty()) {
		thred::savdo();
	  }
	  form::angclp();
	  flag = true;
	  break;
	}
	case ID_VRTCLP: { // fill / Clipboard / Vertical
	  if (Instance->StateMap.test(StateFlag::FORMSEL) || !Instance->SelectedFormList.empty()) {
		thred::savdo();
	  }
	  form::vrtclp();
	  flag = true;
	  break;
	}
	case ID_REFILF: { // fill / Refill
	  thred::filfrms();
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
	  if (Instance->StateMap.test(StateFlag::FORMSEL) || !Instance->SelectedFormList.empty()) {
		thred::savdo();
	  }
	  form::prpbrd(LineSpacing);
	  flag = true;
	  break;
	}
	case ID_FILANG: { // fill / Angle
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::filangl();
	  flag = true;
	  break;
	}
	case ID_APLIQ: { // fill / Border / Applique
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
	  }
	  form::apliq();
	  flag = true;
	  break;
	}
	case ID_SATBRD: { // fill / Border / Angle Satin
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
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
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
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
	  flag = true;
	  break;
	}
	case ID_FILSAT: { // fill / Fan
	  Instance->StateMap.reset(StateFlag::FTHR);
	  form::filsat();
	  flag = true;
	  break;
	}
	case ID_FILL_VERT: { // fill / Vertical
	  if (Instance->StateMap.test(StateFlag::FORMSEL) || !Instance->SelectedFormList.empty()) {
		thred::savdo();
	  }
	  form::filvrt();
	  flag = true;
	  break;
	}
	case ID_FILL_HOR: { // fill / Horizontal
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
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

auto menu::handleMainMenu(WORD const& wParameter, F_POINT& rotationCenter) -> bool {
  auto flag = false;
  switch (wParameter) {
	case ID_HLP: { // help
	  hlp::help();
	  flag = true;
	  break;
	}
	case ID_LA: { // all (Show all layers)
	  thred::showOnlyLayer(0U);
	  flag = true;
	  break;
	}
	case ID_L1: { // 1 (Show layer 1 only)
	  thred::showOnlyLayer(1U);
	  flag = true;
	  break;
	}
	case ID_L2: { // 2 (Show layer 2 only)
	  thred::showOnlyLayer(2U);
	  flag = true;
	  break;
	}
	case ID_L3: { // 3 (Show layer 3 only)
	  thred::showOnlyLayer(3U);
	  flag = true;
	  break;
	}
	case ID_L4: { // 4 (Show layer 4 only)
	  thred::showOnlyLayer(4U);
	  flag = true;
	  break;
	}
	case ID_FRMOF: { // frm+
	  if (wrap::pressed(VK_SHIFT)) {
		thred::tglhid();
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
	  if (!Instance->StitchBuffer.empty()) {
		stitchCoordsInPixels = thred::stch2px1(wrap::toUnsigned(Instance->StitchBuffer.size()) - 1U);
	  }
	  thred::endpnt(stitchCoordsInPixels);
	  Instance->StateMap.set(StateFlag::BAKEND);
	  Instance->StateMap.set(StateFlag::RESTCH);
	  flag = true;
	  break;
	}
	case ID_REDO: { // redo
	  if (Instance->StateMap.test(StateFlag::BAKACT) && Instance->StateMap.test(StateFlag::REDUSHO)) {
		backup::redo();
	  }
	  flag = true;
	  break;
	}
	case ID_FORM: { // Form
	  form::frmon();
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
		formForms::refrm();
	  }
	  else {
		form::form();
	  }
	  flag = true;
	  break;
	}
	case ID_ROT: { // rot
	  thred::rot(rotationCenter);
	  flag = true;
	  break;
	}
	case ZUMIN: { // in
	  if (Instance->StateMap.test(StateFlag::GMRK) || Instance->StateMap.test(StateFlag::SELBOX) ||
	      Instance->StateMap.test(StateFlag::INSRT) || Instance->StateMap.test(StateFlag::GRPSEL) ||
	      Instance->StateMap.test(StateFlag::FORMSEL)) {
		thred::zumin();
	  }
	  else {
		Instance->StateMap.reset(StateFlag::BZUM);
		Instance->StateMap.set(StateFlag::BOXZUM);
		Instance->StateMap.reset(StateFlag::BZUMIN);
		Instance->StateMap.set(StateFlag::VCAPT);
		SetCapture(ThrEdWindow);
	  }
	  flag = true;
	  break;
	}
	case ID_ZUMOUT: { // out
	  thred::zumout();
	  flag = true;
	  break;
	}
	case ID_BACK: { // undo
	  backup::bak();
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

auto menu::getGridDefColor() noexcept -> uint32_t {
  return GRDDEF;
}
