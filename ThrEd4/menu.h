#pragma once

constexpr auto GRDHI  = uint32_t {0xffffffU}; // grid high color
constexpr auto GRDMED = uint32_t {0x404040U}; // grid medium color
constexpr auto GRDDEF = uint32_t {0x202020U}; // grid default color
constexpr auto GRDRED = uint32_t {0xff2020U}; // grid red color
constexpr auto GRDBLU = uint32_t {0x20ff20U}; // grid green color
constexpr auto GRDGRN = uint32_t {0x2020ffU}; // grid blue color

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
void vuselthr();
void vuthrds();
void wrnmen();
} // namespace menu
