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
void addColor(uint32_t stitch, uint32_t color);
auto adflt(uint32_t count) -> uint32_t;
void bakmrk();
void bBox() noexcept(std::is_same_v<size_t, uint32_t>);
void chkDelCol();
void chkhup();
void chkInsCol();
auto chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) -> bool;
void chkrng(F_POINT& range);
auto closlin() -> uint32_t;
void closPnt();
auto closPnt1(uint32_t& closestStitch) -> bool;
void clpbox();
void colchk() noexcept(std::is_same_v<size_t, uint32_t>);
void coltab();
void contract();
void cut();
void delet();
void delfstchs();
void delinf() noexcept;
void delstch();
void delstchm();
void dubox(POINT const& stitchCoordsInPixels);
void duIns();
void dumrk(float xCoord, float yCoord);
void dun();
auto duthrsh(float threshold) noexcept -> uint32_t;
void duzero();
void duzrat() noexcept;
void endpnt(POINT const& stitchCoordsInPixels);
void esccode();
void filfrms();
auto findFirstStitch(uint32_t form) -> uint32_t;
auto findLastStitch(uint32_t form) -> uint32_t;
void fop();
void frmdel();
void frmrct(F_RECTANGLE& rectangle) noexcept;
auto getArrowCursor() -> HCURSOR;
auto getBackGroundBrush() noexcept -> HBRUSH;
auto getColorChangeIndex(uint32_t iColor) -> uint16_t;
auto getDesigner() -> std::wstring;
auto getFileHandle(fs::path const& newFileName, HANDLE& fileHandle) -> bool;
auto getFileSize(fs::path const& newFileName, uintmax_t& size) -> bool;
auto getHomeDir() noexcept -> fs::path*;
auto getLayerPen(uint32_t layer) noexcept(!(std::is_same_v<ptrdiff_t, int>)) -> HPEN;
auto getMsgBufferValue() -> float;
auto getUserPen(uint32_t iPen) noexcept -> HPEN;
auto getVerticalIndex() -> uint8_t;
auto getZoomMin() -> float;
void grpAdj();
void gsnap();
void hidbit();
void hideColorWin() noexcept;
void hupfn();
void initPenBrush() noexcept;
void initBackPenBrush() noexcept;
void insflin(POINT insertPoint);
auto inStitchWin() noexcept -> bool;
void longer();
auto maxColor() noexcept -> size_t;
void movbox();
void movStch();
void nuAct(uint32_t iStitch) noexcept;
void nudgfn(float deltaX, float deltaY);
void nuPen(HPEN& pen, int32_t width, COLORREF color) noexcept;
void nuscol(size_t iColor);
void pgdwn();
void pglft();
void pgrit();
void pgup();
void pntmrk();
auto pt2colInd(uint32_t iStitch) noexcept -> uint32_t;
auto pxCor2stch(POINT const& point) noexcept -> F_POINT;
void qcode();
void rebox();
void redclp();
void redraw(HWND window) noexcept;
void redrawColorBar() noexcept;
void refreshColors() noexcept;
void reldun();
void rembig();
void resetMsgBuffer();
void resetSideBuffer();
void resetColorChanges() noexcept;
void retrac();
void ritfcor(F_POINT const& point);
void ritmov(uint32_t formIndex) noexcept;
void ritot(uint32_t number);
void rngadj() noexcept(std::is_same_v<size_t, uint32_t>);
void rot(F_POINT& rotationCenter);
auto rotang1(F_POINT_ATTR const& unrotatedPoint, float rotationAngle, F_POINT const& rotationCenter) noexcept
    -> F_POINT;
auto rotangf(F_POINT const& unrotatedPoint, float rotationAngle, F_POINT const& rotationCenter) noexcept -> F_POINT;
void rotflt(F_POINT& point, float rotationAngle, F_POINT const& rotationCenter) noexcept;
void rotfn(float rotationAngle, F_POINT const& rotationCenter);
void rotmrk();
void rotseg();
void sCor2px(F_POINT const& stitchCoordinate, POINT& pixelCoordinate);
void savAs();
void savdo();
void save();
void selalstch();
void selCol();
void selfpnt();
void selRct(F_RECTANGLE& sourceRect);
auto setFileName() -> fs::path;
void set1knot();
void setknots();
void setpsel();
void setsrch(uint32_t stitch);
void setSrchLargest();
void setSrchSmallest();
void setVerticalIndex(uint8_t index);
void shft(F_POINT const& delta) noexcept;
void shftflt(F_POINT const& point) noexcept;
void shorter();
void showColorWin() noexcept;
void sizstch(F_RECTANGLE& rectangle, std::vector<F_POINT_ATTR> const& stitches) noexcept;
auto stch2px(uint32_t iStitch, POINT& stitchCoordsInPixels) -> bool;
auto stch2px1(uint32_t iStitch) -> POINT;
auto stch2pxr(F_POINT const& stitchCoordinate) -> POINT;
void stchrct(F_RECTANGLE& rectangle) noexcept;
void stretch();
void strtchbox(std::vector<POINT> const& stretchBoxLine) noexcept(std::is_same_v<size_t, uint32_t>);
void thumnail();
void tglhid();
void toglup();
auto txtWid(wchar_t const* string) noexcept(std::is_same_v<size_t, uint32_t>) -> SIZE;
void tst();
void unbox();
void unbBox();
void unbsho();
void undat() noexcept;
void unmsg();
auto unselectAll() -> bool;
void unsid() noexcept;
void updateBackground();
auto usedcol(uint8_t index) -> bool;
void xlin();
void xlin1();
void zumhom();
void zumin();
void zumout();
void zumshft();

} // namespace thred
