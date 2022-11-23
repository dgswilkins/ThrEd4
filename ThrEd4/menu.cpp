// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "globals.h"
#include "menu.h"

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
