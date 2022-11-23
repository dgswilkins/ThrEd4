#pragma once

namespace menu {
void auxmen();
void bsavmen();
void disableRedo();
void disableUndo();
void enableRedo();
void enableUndo();
void flipFormOnOff();
void init() noexcept;
void knotmen();
void ladj();
void linbmen();
void nedmen();
void qchk();
void redfils(gsl::not_null<std::array<uint32_t, OLDNUM>*> lruMenuId,
             gsl::not_null<std::vector<fs::path>*>        previousNames);
void resetFormOnOff();
void vuselthr();
void vuthrds();
void wrnmen();
} // namespace menu
