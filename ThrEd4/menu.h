#pragma once

// Local Headers
#include "stdafx.h"
#include "switches.h"

namespace fs = std::filesystem;

namespace menu {
void auxmen();
void bsavmen();
void chkmen();
void disableRedo();
void disableUndo();
void duhbit(uint32_t cod) noexcept;
void enableRedo();
void enableUndo();
void fil2men();
void flipFormOnOff();
void flipHideBitmap();
void frmcurmen();
auto getGridDefColor() -> uint32_t;
auto handleEditMenu(WORD const& wParameter) -> bool;
auto handleFileMenu(WORD const& wParameter) -> bool;
auto handleFillMenu(WORD const& wParameter) -> bool;
auto handleMainMenu(WORD const& wParameter, F_POINT& rotationCenter) -> bool;
auto handleViewMenu(WORD const& wParameter) -> bool;
void init() noexcept;
void knotmen();
void ladj();
void linbmen();
void nedmen();
void qchk();
void redfils(gsl::not_null<std::array<uint32_t, OLDNUM>*> lruMenuId,
             gsl::not_null<std::vector<fs::path>*>        previousNames);
void resetFormOnOff();
void resetThreadView();
void rotauxmen();
void setGridCols(const COLORREF& color);
void setdst();
#if PESACT
void setpes();
#endif
void setpcs();
void vuselthr();
void vuthrds();
void wrnmen();
} // namespace menu
