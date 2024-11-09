#pragma once

// Local Headers
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
void bBox() noexcept(std::is_same_v<size_t, uint32_t>);
void bakmrk();
auto chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
void chkDelCol();
void chkInsCol();
auto chkForm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
void chkhup();
auto chkwnd(HWND window) noexcept -> bool;
void chkrng(F_POINT& range);
void closPnt();
auto closPnt1(uint32_t& closestStitch) -> bool;
void closfn();
auto closlin() -> uint32_t;
void clpadj();
void clpbox();
void colchk() noexcept(std::is_same_v<size_t, uint32_t>);
void coltab();
void contract();
auto createChangeThreadSizeWindows() -> uint32_t;
void cut();
void defpref();
void deldir();
void delet();
void delfre();
void delinf() noexcept;
void delknot();
void delsfrms();
void delstch();
void delstchm();
void deltot();
void destroyChangeThreadSizeWindows() noexcept;
auto displayBackups() -> bool;
void drawCapturedStitchBox();
void duIns();
void duSelbox() noexcept;
void dubox(POINT const& stitchCoordsInPixels);
void dufdef() noexcept;
void duinsfil();
void dumrk(float xCoord, float yCoord);
void dun();
void durotu() noexcept(std::is_same_v<size_t, uint32_t>);
auto duthrsh(float threshold) noexcept -> uint32_t;
void duzero();
void duzrat() noexcept;
void endpnt(POINT const& stitchCoordsInPixels);
void esccode();
void filclos();
void fileLock() noexcept;
void filfrms();
auto findFirstStitch(uint32_t form) -> uint32_t;
auto findLastStitch(uint32_t form) -> uint32_t;
void fixpclp(uint32_t closestFormToCursor);
void fop();
void frmdel();
void frmrct(F_RECTANGLE& rectangle) noexcept;
auto getAdjustedDelta() -> F_POINT;
auto getAdjustedPoint(F_POINT stitchPoint) noexcept -> F_POINT;
auto getBackGroundBrush() noexcept -> HBRUSH;
auto getColorbarVertPosition() noexcept -> float;
auto getColorChangeIndex(uint32_t iColor) noexcept -> uint16_t;
auto getDesigner() -> std::wstring;
auto getFileHandle(fs::path const& newFileName, HANDLE& fileHandle) -> bool;
auto getFileSize(fs::path const& newFileName, uintmax_t& size) -> bool;
auto getFormControlPoints() noexcept -> std::vector<POINT>&;
auto getHomeDir() noexcept -> fs::path&;
auto getLabelWindow() noexcept -> std::vector<HWND>&;
auto getMoveDelta(const POINT& point) noexcept -> F_POINT;
auto getMsgBufferValue() -> float;
auto getRotationHandleAngle() noexcept -> float;
auto getStitchWindowX() -> LONG;
auto getStitchBoxesThreshold() -> float;
auto getThrEdName() noexcept -> fs::path const&;
auto getUserPen(uint32_t iPen) noexcept -> HPEN;
auto getVerticalIndex() noexcept -> uint8_t;
auto getZoomMin() noexcept -> float;
void getdes() noexcept;
void gotbox();
void grpAdj();
void gsnap();
auto handleFormDataSheet() -> bool;
void handleFormSelected();
auto handleSideWindowActive() -> bool;
void hidbit();
void hideColorWin() noexcept;
void hupfn();
void ilin() noexcept;
void ilin1() noexcept;
auto inChangeThreadWindows() -> bool;
auto inDefaultColorWindows() -> bool;
auto inColorbar() noexcept -> bool;
auto inrng(uint32_t iStitch) noexcept -> bool;
auto inStitchWin() noexcept -> bool;
auto inThreadWindows() -> bool;
auto inThreadSizeWindows() -> bool;
auto inUserColorWindows() -> bool;
void initBackPenBrush() noexcept;
void initPenBrush() noexcept;
void insfil(fs::path& insertedFile);
void insflin(POINT insertPoint);
auto isLine(std::vector<POINT> const& boxOutline) noexcept -> bool;
auto iselpnt() noexcept -> bool;
void lenCalc();
auto maxColor() noexcept -> size_t;
void movStch();
void movbox();
void movchk();
void movi();
void mv2b();
void mv2f();
void newFil();
void nextSortedStitch(bool direction);
void nuAct(uint32_t iStitch) noexcept;
void nuTHRfile();
void nuPen(HPEN& pen, int32_t width, COLORREF color) noexcept;
void nucols();
void nudgfn(float deltaX, float deltaY);
void nuscol(size_t iColor);
void nuslst(uint32_t find);
void nuthbak(uint32_t count);
void nuthsel();
void openAuxFile();
void openBitMapFile();
void openNewFile();
void otherBack() noexcept;
void otherPeriod() noexcept;
void otherReturn();
void ovrlay();
void pgdwn();
void pglft();
void pgrit();
void pgup();
void pntmrk();
auto pt2colInd(uint32_t iStitch) noexcept -> uint32_t;
void purg();
void purgdir();
auto pxCor2stch(POINT const& point) noexcept -> F_POINT;
void qcode();
void rSelbox();
void rats();
void ratsr();
void rebox();
void redclp();
void redraw(HWND window) noexcept;
void redrawColorBar() noexcept;
void redrawCapturedStitch(uint32_t closestPointIndexClone);
void refreshColors() noexcept;
void reldun();
void rembig();
void resetColorChanges() noexcept;
void resetColors();
void resetMsgBuffer();
void resetSideBuffer();
void retrac();
void ritfcor(F_POINT const& point);
void ritmov(uint32_t formIndex) noexcept;
void ritot(uint32_t number);
void ritrot(float rotationAngle, F_POINT const& rotationCenter);
void rngadj() noexcept(std::is_same_v<size_t, uint32_t>);
void rot(F_POINT& rotationCenter);
auto rotang1(F_POINT_ATTR const& unrotatedPoint, float rotationAngle, F_POINT const& rotationCenter) noexcept
    -> F_POINT;
auto rotangf(F_POINT const& unrotatedPoint, float rotationAngle, F_POINT const& rotationCenter) noexcept -> F_POINT;
void rotflt(F_POINT& point, float rotationAngle, F_POINT const& rotationCenter) noexcept;
void rotfn(float rotationAngle, F_POINT const& rotationCenter);
void rotmrk();
void rotseg();
void rstAll();
void sCor2px(F_POINT const& stitchCoordinate, POINT& pixelCoordinate);
void savAs();
void savdo();
void save();
auto scaleHorizontal(float const& value) noexcept -> long;
auto scaleVertical(float const& value) noexcept -> long;
void selCol();
void selRct(F_RECTANGLE& sourceRect) noexcept(!std::is_same_v<size_t, uint32_t>);
void selalstch();
void selfpnt();
void set1knot();
void setBackGroundColor();
auto setFileName() -> fs::path;
void setFormControls() noexcept(!std::is_same_v<size_t, uint32_t>);
void setLargestStitchVal();
void setRotateCapture();
void setSmallestStitchVal();
void setSrchLargest();
void setSrchSmallest();
void setThreadSize() noexcept;
void setgrd(COLORREF color);
void setknots();
void setpsel();
void shft(F_POINT const& delta) noexcept;
void shftflt(F_POINT const& point) noexcept;
void showColorWin() noexcept;
void showOnlyLayer(uint8_t play);
void ShowScrollBars(bool Show) noexcept;
auto stch2px(uint32_t iStitch, POINT& stitchCoordsInPixels) -> bool;
auto stch2px1(uint32_t iStitch) -> POINT;
auto stch2pxr(F_POINT const& stitchCoordinate) -> POINT;
void stchrct(F_RECTANGLE& rectangle) noexcept;
void stretch();
void strtchbox(std::vector<POINT> const& stretchBoxLine) noexcept(std::is_same_v<size_t, uint32_t>);
void switchUserColors() noexcept;
void tglhid();
void toglHid();
void thumbak();
void thumbBack();
void thumbEnd();
void thumbHome();
void thumbQuit();
void thumnail();
void toglup();
void tst();
auto txtWid(wchar_t const* string) noexcept(std::is_same_v<size_t, uint32_t>) -> SIZE;
void unbBox();
void unbox();
void unbsho();
void unclp();
void undat() noexcept;
void unlin();
void unmsg();
void unpat();
void unrot();
void unrotu();
void unsel();
auto unselectAll() -> bool;
void unsid() noexcept;
void unstrtch(std::vector<POINT> const& stretchBoxLine);
void updateBackground();
auto updateFillColor() -> bool;
void updateHoopSize();
void updateMoveLine();
auto updatePreferences() -> bool;
void updateThreadSize(uint32_t threadSizeSelected);
void updateUserColor();
auto updateZoomFactor(F_POINT& newSize) noexcept -> float;
auto usedcol(uint8_t index) -> bool;
void viewThread30();
void viewThread40();
void viewThread60();
void vubak();
void xlin();
void xlin1();
void zumhom();
void zumin();
void zumout();
void zumshft();

} // namespace thred
