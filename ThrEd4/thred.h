#pragma once

// Local Headers
#include "clipStitch.h"
#include "fRectangle.h"
#include "point.h"

// Standard Libraries
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace thred {
auto adclp(uint32_t count) -> uint32_t;
auto adflt(uint32_t count) -> uint32_t;
void bBox() noexcept(std::is_same_v<size_t, uint32_t>);
void chkhup();
void chkrng(F_POINT& range);
void clpbox();
void addColor(uint32_t stitch, uint32_t color);
void coltab();
void delfstchs();
void delinf() noexcept;
void delstchm();
auto duthrsh(float threshold) noexcept -> uint32_t;
void duzrat() noexcept;
auto findFirstStitch(uint32_t form) -> uint32_t;
auto findLastStitch(uint32_t form) -> uint32_t;
void frmdel();
void frmrct(F_RECTANGLE& rectangle) noexcept;
auto getBackGroundBrush() noexcept -> HBRUSH;
auto getFileHandle(fs::path const& newFileName, HANDLE& fileHandle) -> bool;
auto getFileSize(fs::path const& newFileName, uintmax_t& size) -> bool;
auto getHomeDir() noexcept -> fs::path*;
auto getLayerPen(uint32_t layer) noexcept(!(std::is_same_v<ptrdiff_t, int>)) -> HPEN;
auto getMsgBufferValue() -> float;
auto getUserPen(uint32_t iPen) noexcept -> HPEN;
void grpAdj();
void hidbit();
void hideColorWin() noexcept;
void hupfn();
void initPenBrush() noexcept;
void initBackPenBrush() noexcept;
void insflin(POINT insertPoint);
auto inStitchWin() noexcept -> bool;
auto maxColor() noexcept -> size_t;
void movStch();
void nuPen(HPEN& pen, int32_t width, COLORREF color) noexcept;
auto pxCor2stch(POINT const& point) noexcept -> F_POINT;
void redclp();
void redraw(HWND window) noexcept;
void redrawColorBar() noexcept;
void refreshColors() noexcept;
void resetMsgBuffer();
void resetSideBuffer();
void resetColorChanges() noexcept;
void ritfcor(F_POINT const& point);
void ritmov(uint32_t formIndex) noexcept;
void ritot(uint32_t number);
void rngadj() noexcept(std::is_same_v<size_t, uint32_t>);
auto rotang1(F_POINT_ATTR const& unrotatedPoint, float rotationAngle, F_POINT const& rotationCenter) noexcept
    -> F_POINT;
auto rotangf(F_POINT const& unrotatedPoint, float rotationAngle, F_POINT const& rotationCenter) noexcept -> F_POINT;
void rotflt(F_POINT& point, float rotationAngle, F_POINT const& rotationCenter) noexcept;
void rotfn(float rotationAngle, F_POINT const& rotationCenter);
void sCor2px(F_POINT const& stitchCoordinate, POINT& pixelCoordinate);
void savdo();
void save();
void selRct(F_RECTANGLE& sourceRect);
auto setFileName() -> fs::path;
void setpsel();
void shft(F_POINT const& delta) noexcept;
void showColorWin() noexcept;
void sizstch(F_RECTANGLE& rectangle, std::vector<F_POINT_ATTR> const& stitches) noexcept;
auto stch2pxr(F_POINT const& stitchCoordinate) -> POINT;
void stchrct(F_RECTANGLE& rectangle) noexcept;
void strtchbox(std::vector<POINT> const& stretchBoxLine) noexcept(std::is_same_v<size_t, uint32_t>);
auto txtWid(wchar_t const* string) noexcept(std::is_same_v<size_t, uint32_t>) -> SIZE;
void tst();
void unbBox();
void unbsho();
void undat() noexcept;
void unmsg();
void unsid() noexcept;
void zumhom();

} // namespace thred
