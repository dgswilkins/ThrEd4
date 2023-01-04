// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "displayText.h"
#include "globals.h"
#include "menu.h"

// menus
static auto BorderFillMenu = gsl::narrow_cast<HMENU>(nullptr); // border fill submenu
static auto EditMenu       = gsl::narrow_cast<HMENU>(nullptr); // edit submenu
static auto FileMenu       = gsl::narrow_cast<HMENU>(nullptr); // file submenu
static auto FillMenu       = gsl::narrow_cast<HMENU>(nullptr); // fill submenu
static auto ViewMenu       = gsl::narrow_cast<HMENU>(nullptr); // view submenu
static auto ViewSetMenu    = gsl::narrow_cast<HMENU>(nullptr); // view/set

void menu::disableRedo() {
  if (StateMap->testAndReset(StateFlag::REDUSHO)) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
	StateMap->set(StateFlag::DUMEN);
  }
}

void menu::enableRedo() {
  if (!StateMap->testAndSet(StateFlag::REDUSHO)) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
	StateMap->set(StateFlag::DUMEN);
  }
}

void menu::disableUndo() {
  if (StateMap->testAndReset(StateFlag::UNDUSHO)) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	StateMap->set(StateFlag::DUMEN);
  }
}

void menu::enableUndo() {
  if (!StateMap->testAndSet(StateFlag::UNDUSHO)) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
	StateMap->set(StateFlag::DUMEN);
  }
}

void menu::ladj() {
  for (auto const& iLayer : LAYRLIST) {
	if (iLayer.value == ActiveLayer) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, iLayer.value + M_ALL, MF_BYPOSITION | MF_GRAYED);
	}
	else {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  EnableMenuItem(MainMenu, iLayer.value + M_ALL, MF_BYPOSITION | MF_ENABLED);
	}
  }
  StateMap->set(StateFlag::DUMEN);
}

void menu::flipFormOnOff() {
  if (StateMap->testAndFlip(StateFlag::FRMOF)) {
	FormOnOff->assign(displayText::loadStr(IDS_FRMPLUS));
  }
  else {
	FormOnOff->assign(displayText::loadStr(IDS_FRMINUS));
	StateMap->reset(StateFlag::FORMSEL);
	StateMap->reset(StateFlag::FORMIN);
	StateMap->reset(StateFlag::MOVFRM);
	StateMap->reset(StateFlag::FRMPMOV);
	StateMap->reset(StateFlag::FRMOV);
	StateMap->reset(StateFlag::SATIN);
	StateMap->reset(StateFlag::SATPNT);
	StateMap->reset(StateFlag::SATCNKT);
	StateMap->reset(StateFlag::FENDIN);
	StateMap->reset(StateFlag::DELFRM);
	StateMap->reset(StateFlag::FRMPSEL);
	StateMap->reset(StateFlag::INSFRM);
  }
  SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
  StateMap->set(StateFlag::DUMEN);
}

void menu::resetFormOnOff() {
  FormOnOff->assign(displayText::loadStr(IDS_FRMPLUS));
  SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
  StateMap->set(StateFlag::DUMEN);
}

void menu::qchk() {
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

void menu::nedmen() {
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

void menu::knotmen() {
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

void menu::bsavmen() {
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

void menu::linbmen() {
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

void menu::wrnmen() {
  auto code = UINT {MF_CHECKED};
  if (UserFlagMap->test(UserFlag::WRNOF)) {
	code = MF_UNCHECKED;
  }
  CheckMenuItem(MainMenu, ID_WARNOF, code);
  StateMap->set(StateFlag::DUMEN);
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
  auto const auxTxt = displayText::loadStr(IDS_AUXTXT);
  switch (IniFile.auxFileType) {
	case AUXDST: {
	  auxMsg.assign(fmt::format(fmt::runtime(auxTxt), L"DST"));
	  CheckMenuItem(MainMenu, ID_AUXDST, MF_CHECKED);
	  break;
	}
	case AUXPES:
#if PESACT
	{
	  auxMsg.assign(fmt::format(fmt::runtime(auxTxt), L"PES"));
	  CheckMenuItem(MainMenu, ID_AUXPES, MF_CHECKED);
	  break;
	}
#else
	{
	  IniFile.auxFileType = AUXPCS;
	}
#endif
	default: {
	  auxMsg.assign(fmt::format(fmt::runtime(auxTxt), L"PCS"));
	  CheckMenuItem(MainMenu, ID_AUXPCS, MF_CHECKED);
	}
  }
#pragma warning(suppress : 26492) // type.3 Don't use const_cast to cast away const or volatile NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
  filinfo.dwTypeData = const_cast<LPWSTR>(auxMsg.c_str());
  SetMenuItemInfo(FileMenu, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
  StateMap->set(StateFlag::DUMEN);
}

void menu::redfils(gsl::not_null<std::array<uint32_t, OLDNUM>*> lruMenuId,
                   gsl::not_null<std::vector<fs::path>*>        previousNames) {
  auto findData = WIN32_FIND_DATA {0, {0, 0}, {0, 0}, {0, 0}, 0, 0, 0, 0, L"", L""};
  for (auto const& iLRU : *lruMenuId) {
	if (GetMenuState(FileMenu, iLRU, MF_BYCOMMAND) != gsl::narrow_cast<UINT>(-1)) {
	  DeleteMenu(FileMenu, iLRU, MF_BYCOMMAND);
	}
  }
  auto previousName = previousNames->begin();
  for (auto const& iLRU : *lruMenuId) {
	if (!previousName->empty()) {
	  if (StateMap->test(StateFlag::SAVAS)) {
		AppendMenu(FileMenu,
		           MF_BYCOMMAND | MF_STRING,
		           iLRU,
		           previousName->wstring().c_str()); // NOLINT(hicpp-signed-bitwise)
	  }
	  else {
		// NOLINTNEXTLINE(readability-qualified-auto)
		auto const fileHandle = FindFirstFile(previousName->c_str(), &findData);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
		if (fileHandle == INVALID_HANDLE_VALUE) {
		  previousName->clear();
		}
		else {
		  AppendMenu(FileMenu,
		             MF_BYCOMMAND | MF_STRING,
		             iLRU,
		             previousName->wstring().c_str()); // NOLINT(hicpp-signed-bitwise)
		  FindClose(fileHandle);
		}
	  }
	}
	++previousName;
  }
  StateMap->set(StateFlag::DUMEN);
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
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
  StateMap->reset(StateFlag::COL);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
  StateMap->set(StateFlag::DUMEN);
}

void menu::vuthrds() {
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

void menu::vuselthr() {
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

void menu::setGridCols(const COLORREF& color) {
  class GRID_COL
  {
public:
	uint32_t id {};
	uint32_t col {};

	// constexpr GRID_COL() noexcept = default;
	// GRID_COL(GRID_COL const&) = default;
	// GRID_COL(GRID_COL&&) = default;
	// GRID_COL& operator=(GRID_COL const& rhs) = default;
	// GRID_COL& operator=(GRID_COL&&) = default;
	//~GRID_COL() = default;
  };

  static constexpr auto GC_HIGH    = GRID_COL {ID_GRDHI, GRDHI};
  static constexpr auto GC_MEDIUM  = GRID_COL {ID_GRDMED, GRDMED};
  static constexpr auto GC_DEFAULT = GRID_COL {ID_GRDEF, GRDDEF};
  static constexpr auto GC_RED     = GRID_COL {ID_GRDRED, GRDRED};
  static constexpr auto GC_BLUE    = GRID_COL {ID_GRDBLU, GRDBLU};
  static constexpr auto GC_GREEN   = GRID_COL {ID_GRDGRN, GRDGRN};
  static constexpr auto GRID_CODES =
      std::array<GRID_COL, 6> {GC_HIGH, GC_MEDIUM, GC_DEFAULT, GC_RED, GC_BLUE, GC_GREEN};

  for (auto const& gridCode : GRID_CODES) {
	if (color == gridCode.col) {
	  CheckMenuItem(MainMenu, gridCode.id, MF_CHECKED);
	}
	else {
	  CheckMenuItem(MainMenu, gridCode.id, MF_UNCHECKED);
	}
  }
  StateMap->set(StateFlag::DUMEN);
}

void menu::duhbit(uint32_t cod) noexcept {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_HIDBIT, MF_BYCOMMAND | cod);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CheckMenuItem(MainMenu, ID_HIDBITF, MF_BYCOMMAND | cod);
}

void menu::flipHideBitmap() {
  if (StateMap->testAndFlip(StateFlag::HIDMAP)) {
	menu::duhbit(MF_UNCHECKED);
  }
  else {
	menu::duhbit(MF_CHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void menu::rotauxmen() {
  if (UserFlagMap->test(UserFlag::ROTAUX)) {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_ROTAUXON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_CHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void menu::frmcurmen() {
  if (UserFlagMap->test(UserFlag::FRMX)) {
	CheckMenuItem(MainMenu, ID_FRMX, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_UNCHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FRMX, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FRMBOX, MF_CHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

void menu::fil2men() {
  if (UserFlagMap->test(UserFlag::FIL2OF)) {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_UNCHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_CHECKED);
  }
  else {
	CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_CHECKED);
	CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_UNCHECKED);
  }
  StateMap->set(StateFlag::DUMEN);
}

static auto DataCode = std::array<UINT, 4U> {ID_CHKOF, ID_CHKON, ID_CHKREP, ID_CHKREPMSG};

void menu::chkmen() {
  constexpr auto LASTCODE = DataCode.size();
  for (auto iCode = 0U; iCode < LASTCODE; ++iCode) {
	auto const code = (iCode == IniFile.dataCheck) ? gsl::narrow_cast<UINT>(MF_CHECKED)
	                                               : gsl::narrow_cast<UINT>(MF_UNCHECKED);
	CheckMenuItem(MainMenu, DataCode[iCode], code);
  }
  StateMap->set(StateFlag::DUMEN);
}
