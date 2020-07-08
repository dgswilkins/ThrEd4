#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <filesystem>
#include <vector>

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26814)
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace fs = std::filesystem;

// File open types
enum class fileStyles : uint8_t {
  ALL_FILES, // All file Types
  INS_FILES  // Insert file types
};

// File Indices
enum class fileIndices : uint8_t {
  THR, // Thredworks
  PCS, // Pfaff
  PES, // Brother
  DST  // Tajima
};

namespace thred {
auto adclp(uint32_t count) -> uint32_t;
auto adflt(uint32_t count) -> uint32_t;
void bBox() noexcept;
void chkmen() noexcept;
void chkrng(fPOINT& range);
void coltab();
void delfstchs();
void delinf() noexcept;
void delstchm();
auto duthrsh(double threshold) noexcept -> uint32_t;
void duzrat() noexcept;
void frmdel();
void frmrct(fRECTANGLE& rectangle) noexcept;
auto getFileHandle(std::filesystem::path const& newFileName, HANDLE& fileHandle) -> bool;
auto getFileSize(std::filesystem::path const& newFileName, uintmax_t& size) -> bool;
void grpAdj();
void hidbit();
void hupfn();
auto inStitchWin() noexcept -> bool;
void movStch();
auto nuPen(HPEN pen, uint32_t width, COLORREF color) noexcept -> HPEN;
auto pxCor2stch(POINT const& point) noexcept -> fPOINT;
void redclp();
void redraw(HWND window) noexcept;
void resetColorChanges() noexcept;
void ritfcor(fPOINT const& point);
void ritmov(uint32_t formIndex);
void ritot(uint32_t number);
void rngadj();
void rotang1(fPOINTATTR const& unrotatedPoint, fPOINT& rotatedPoint, float rotationAngle, fPOINT const& rotationCenter) noexcept;
void rotangf(fPOINT const& unrotatedPoint, fPOINT& rotatedPoint, float rotationAngle, fPOINT const& rotationCenter) noexcept;
void rotflt(fPOINT& point, float rotationAngle, fPOINT const& rotationCenter) noexcept;
void rotfn(float rotationAngle, fPOINT const& rotationCenter);
void sCor2px(fPOINT const& stitchCoordinate, POINT& pixelCoordinate);
void savclp(CLPSTCH& destination, fPOINTATTR const& source, uint32_t led);
void savdo();
void save();
void selRct(fRECTANGLE& sourceRect) noexcept;
void setpsel();
void shft(fPOINT const& delta) noexcept;
void sizstch(fRECTANGLE& rectangle, std::vector<fPOINTATTR>& stitches) noexcept;
auto stch2pxr(fPOINT const& stitchCoordinate) -> POINT;
void stchrct(fRECTANGLE& rectangle) noexcept;
void strtchbox(std::vector<POINT>& stretchBoxLine) noexcept;
auto txtWid(wchar_t const* string) noexcept -> SIZE;
void unbBox();
void unbsho();
void undat() noexcept;
void unmsg();
void unsid() noexcept;
void wrnmen();
void zumhom();

namespace internal {
  void angdif(float& lowestAngle, float& highestAngle, float angle) noexcept;
  void auxmen();
  void bak();
  void bakmrk();
  void bakthum();
  void barnam(HWND window, uint32_t iThumbnail);
  void box(uint32_t iNearest, HDC dc) noexcept;
  void boxs() noexcept;
  void bsavmen();
  auto centr() -> fPOINT;
  void chk1col();
  auto chkMsg(std::vector<POINT>& stretchBoxLine, float& xyRatio, float& angle, fPOINT& rotationCenter, FRMHED& textureForm)
      -> bool;
  auto chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) noexcept -> bool;
  auto chkattr(fs::path const& filename) -> bool;
  auto chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
  void chkbit();
  auto chkhid(uint32_t colorToCheck) -> bool;
  void chkhup();
  void chkirct() noexcept;
  auto chkminus(uint32_t code) noexcept -> bool;
  void chkncol();
  void chknum();
  auto chkok() noexcept -> bool;
  auto chkup(uint32_t count, uint32_t iStitch) -> uint32_t;
  auto chkwnd(HWND window) noexcept -> bool;
  void closPnt();
  auto closPnt1(uint32_t& closestStitch) -> bool;
  void closfn();
  auto closlin() -> uint32_t;
  void clpadj();
  void clpbox();
  void clpradj(fRECTANGLE& clipRectAdjusted, fPOINTATTR const& stitch) noexcept;
  auto cmpstch(uint32_t iStitchA, uint32_t iStitchB) noexcept -> bool;
  void colchk();
  void cros(uint32_t iStitch);
  void crtcurs() noexcept;
  void cut();
  void defNam(fs::path const& fileName);
  auto defTxt(uint32_t iColor) -> COLORREF;
  void defpref();
  void delcol();
  void deldir();
  void deldu();
  void delet();
  void delfre();
  void delknot();
  void delknt();
  void delmap();
  void delsfrms(uint32_t code);
  void delsmal(uint32_t startStitch, uint32_t endStitch);
  void delstch();
  void delstch1(uint32_t iStitch);
  void deltot();
  void desiz();

  auto CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  auto doPaste(std::vector<POINT>& stretchBoxLine, bool& retflag) -> bool;
  void drawBackground();
  void drwLin(std::vector<POINT>& linePoints, uint32_t currentStitch, uint32_t length, HPEN hPen);
  void drwStch();
  void drwknot();
  void drwlstch(uint32_t finish);
  void drwmrk(HDC dc);
  void dstcurs() noexcept;
  void duClos(uint32_t startStitch, uint32_t stitchCount, fPOINT const& stitchPoint, std::vector<double>& gapToNearest) noexcept;
  void duIns();
  void duSelbox();
  void duar(POINT const& stitchCoordsInPixels);
  void dubar();
  void dubox(POINT const& stitchCoordsInPixels);
  void dubuf(std::vector<char>& buffer);
  void duclip();
  void duclp() noexcept;
  void ducmd();
  void ducros(HDC dc);
  void dudat();
  void dufdef() noexcept;
  void dufsel();
  void dugrid();
  void duhbit(uint32_t cod) noexcept;
  void duhom();
  void duinsfil();
  void dulin();
  void dumov();
  void dumrk(float xCoord, float yCoord);
  void dun();
  auto dunum(uint32_t code) noexcept -> bool;
  void dupclp();
  void duprct(FRMHED const& form);
  void durit(std::vector<char>& destination, const void* source, uint32_t count);
  void durot() noexcept;
  void durotu() noexcept;
  void dusel(HDC dc);
  void duselrng(RANGE& selectedRange);
  void dusid(uint32_t entry, uint32_t& windowLocation, POINT const& windowSize, std::wstring const* const strings) noexcept;

  constexpr auto byteSwap(uint32_t data) noexcept -> uint32_t;

  void duver(fs::path const& name);
  void duzero();
  void endknt(std::vector<fPOINTATTR>& buffer, uint32_t finish);
  void endpnt(POINT const& stitchCoordsInPixels);

  auto CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) noexcept -> BOOL;

  void esccode();
  void f1del(uint32_t formIndex);
  void fil2men();
  void fil2sel(uint32_t stat);
  void filclos();
  void filfrms();
  auto finrng(uint32_t find) noexcept -> bool;
  void fixpclp(uint32_t closestFormToCursor);
  void fnamtabs();
  void fndknt();
  void fop();
  void frmcalc(uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex);
  auto frmcnt(uint32_t iForm, uint32_t& formFirstStitchIndex) -> uint32_t;
  void frmcurmen();
  void frmcursel(uint32_t cursorType);
  void frmpos(FRMHED& form, float deltaX, float deltaY);
  void frmsnap(uint32_t start, uint32_t count);
  auto frmstch() -> bool;

  auto CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

  void getbak();
  void getdes() noexcept;
  void getDocsFolder(fs::path* directory);
  void getfrmbox();
  void getfrmpix();
  auto gethand(std::vector<fPOINTATTR> const& stitch, uint32_t stitchCount) noexcept -> uint32_t;
  auto getNewFileName(fs::path& newFileName, fileStyles fileTypes, fileIndices fileIndex) -> bool;
  void getnpix();
  void getstpix();
  void gotbox();
  void gselrng() noexcept;
  void gsnap();
  auto handleEditMenu(WORD const& wParameter) -> bool;
  auto handleEitherButtonDown(bool& retflag) -> bool;
  auto handleEndKey(int32_t& retflag) -> bool;
  auto handleFileMenu(WORD const& wParameter) -> bool;
  auto handleFillMenu(WORD const& wParameter) -> bool;
  auto handleFormDataSheet() -> bool;
  auto handleHomeKey(bool& retflag) -> bool;
  auto handleLeftButtonDown(std::vector<POINT>& stretchBoxLine, float& xyRatio, FRMHED const& textureForm, bool& retflag)
      -> bool;
  auto handleLeftButtonUp(float xyRatio, float rotationAngle, fPOINT& rotationCenter, bool& retflag) -> bool;
  auto handleLeftKey(bool& retflag) -> bool;
  auto handleMainMenu(WORD const& wParameter, fPOINT& rotationCenter) -> bool;
  auto handleMainWinKeys(uint32_t const& code, fPOINT& rotationCenter, std::vector<POINT>& stretchBoxLine, bool& retflag)
      -> bool;
  auto handleMouseMove(std::vector<POINT>& stretchBoxLine,
                       float               xyRatio,
                       float&              rotationAngle,
                       fPOINT const&       rotationCenter,
                       FRMHED const&       textureForm) -> bool;
  auto handleNumericInput(uint32_t const& code, bool& retflag) -> bool;
  auto handleRightButtonDown() -> bool;
  auto handleRightKey(bool& retflag) -> bool;
  auto handleSideWindowActive() -> bool;
  auto handleViewMenu(WORD const& wParameter) -> bool;
  void hidknot();
  void ilin() noexcept;
  void ilin1() noexcept;
  void infadj(float& xCoordinate, float& yCoordinate) noexcept;
  void init();
  auto inrng(uint32_t stitch) noexcept -> bool;
  void insadj();
  void inscol();
  void insfil(fs::path& insertedFile);
  void insflin(POINT insertPoint);
  auto isInBox(POINT const& point, RECT const& box) noexcept -> bool;
  auto isLine(std::vector<POINT>& boxOutline) noexcept -> bool;
  auto iselpnt() noexcept -> bool;
  auto isfclp() noexcept -> bool;
  void istch();
  auto isthr(fs::path const& thredPath) -> bool;
  auto kjmp(std::vector<fPOINTATTR>& buffer, uint32_t start) -> uint32_t;
  void knotmen();
  void ladj();
  void lenCalc();
  void lenfn(uint32_t start, uint32_t end, uint32_t& largestStitchIndex, uint32_t& smallestStitchIndex);
  void lensadj();
  void linbmen();
  void lock();

  auto CALLBACK LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

  void lodclp(uint32_t iStitch);
  void longer();
  void makCol() noexcept;
  auto makbig(uint32_t start, uint32_t finish) -> uint32_t;
  void mark();
  void movbox();
  void movchk();
  void movi();
  void movins();
  void movmrk();
  auto movstchs(uint32_t destination, uint32_t start, uint32_t finish) -> bool;
  void mv2b();
  void mv2f();
  void nedmen();
  void nedof();
  void nedon();
  void newFil();
  auto getSaveName(fs::path* fileName, fileIndices& fileType) -> bool;
  void noMsg();
  void nuAct(uint32_t iStitch) noexcept;
  auto nuBak() noexcept -> COLORREF;
  auto nuBrush(HBRUSH brush, COLORREF color) noexcept -> HBRUSH;
  auto nuCol(COLORREF init) noexcept -> COLORREF;
  void resetState();
  void nuFil(fileIndices fileIndex);
  void nuRct() noexcept;
  void nuStchSiz(uint32_t iColor, uint32_t width) noexcept;
  auto nuang(float OriginalAngle, float yDelta, float xDelta) noexcept -> float;
  void nucols();
  void nudgfn(float deltaX, float deltaY);
  void nulayr(uint32_t play);
  void nunams();
  void nuscol(uint32_t iColor) noexcept;
  void nuselrct();
  void nuslst(uint32_t find);
  void nuthbak(uint32_t count);
  void nuthsel();
  void nuthum(char character);

  constexpr auto nxtcrnr(uint32_t corner) -> uint32_t;

  void ofstch(std::vector<fPOINTATTR>& buffer, uint32_t iSource, char offset, uint32_t KnotAttribute);
  auto oldwnd(HWND window) noexcept -> bool;
  void ovrlay();
  void patdun();
  void pcsbsavof();
  void pcsbsavon();
  auto pcshup(std::vector<fPOINTATTR>& stitches) -> bool;
  void pgdwn();
  void pglft();
  void pgrit();
  void pgup();
  void pixmsg(uint32_t iString, uint32_t pixelCount);
  void pntmrk();
  void prtred(HANDLE fileHandle, uint32_t code);
  auto pt2colInd(uint32_t iStitch) noexcept -> uint32_t;
  void purg();
  void purgdir();
  auto pxchk(double pixelSize) noexcept -> double;
  void qchk();
  void qcode();
  void rSelbox();
  auto readPCSFile(std::filesystem::path const& newFileName) -> bool;
  auto readTHRFile(std::filesystem::path const& newFileName) -> bool;
  void rebak();
  void rebox();
  void redbak();
  void redbal();
  void redfils();
  void redfnam(std::wstring& designerName);
  void redini();
  void redo();
  void reldun();
  void relin();
  void rembig();
  void respac(FRMHED& form) noexcept;
  void retrac();
  void ritbak(fs::path const& fileName, DRAWITEMSTRUCT* drawItem);
  void ritbal();
  void ritcor(fPOINTATTR const& pointAttribute);
  void ritcur() noexcept;
  void ritfnam(std::wstring const& designerName);
  void ritini();
  void ritlayr();
  void ritloc();
  void ritlock(WIN32_FIND_DATA const* fileData, uint32_t fileIndex, HWND hwndlg) noexcept;
  void ritrot(float rotationAngle, fPOINT const& rotationCenter);
  void rngal();
  void rot(fPOINT& rotationCenter);
  void rotang(fPOINT unrotatedPoint, POINT& rotatedPoint, float rotationAngle, fPOINT const& rotationCenter);
  void rotauxmen();
  void rotauxsel(uint32_t stat);
  void rotfns(float rotationAngle);
  void rotmrk();
  void rotpix(POINT const& unrotatedPoint, POINT& rotatedPoint, POINT const& rotationCenterPixels);
  void rotseg();
  void rotstch(fPOINTATTR& stitch, float rotationAngle, fPOINT const& rotationCenter) noexcept;
  auto rsed() noexcept -> uint32_t;
  void rshft(POINT const& shiftPoint);
  void rstAll();
  void rstdu();
  void rthumnam(uint32_t iThumbnail);
  void sachk();
  auto savePCS(fs::path const* auxName, std::vector<fPOINTATTR>& saveStitches) -> bool;
  void sav();
  void savAs();
  auto savcmp() noexcept -> bool;
  auto sdCor2px(fPOINTATTR const& stitchPoint) -> POINT;
  void segentr(float rotationAngle);
  void selCol();
  void selalstch();
  void seldwn();
  void selfpnt();
  void selfrm0();
  void selfrmx();
  void selin(uint32_t start, uint32_t end, HDC dc);
  void selup();
  void set1knot();
  auto setRmap(boost::dynamic_bitset<>& stitchMap, fPOINTATTR const& stitchPoint, fPOINT const& cellSize) -> bool;
  void setbak(uint32_t penWidth) noexcept;
  void setdst();
  void setgrd(COLORREF color);
  void setknots();
  void setknt();
  void setmov();
  void setpclp();
  void setpcs();
  void setPrefs();
  void setsped();
  void setsrch(uint32_t stitch);
  void shft2box();
  void shftflt(fPOINT point) noexcept;
  void shoknot();
  void shorter();
  auto sidclp() -> bool;
  void sidhup();
  void sidmsg(FRMHED const& form, HWND window, std::wstring const* const strings, uint32_t entries);
  void sizclp(FRMHED const& form, uint32_t& formFirstStitchIndex, uint32_t& formStitchCount, uint32_t& length, uint32_t& fileSize);
  auto sizfclp(FRMHED const& form) -> uint32_t;
  void srchk();
  auto srchknot(uint32_t source) noexcept -> uint32_t;
  void stCor2px(fPOINTATTR const& stitch, POINT& point);
  auto stch2px(uint32_t iStitch, POINT& stitchCoordsInPixels) -> bool;
  auto stch2px1(uint32_t iStitch) -> POINT;
  auto stch2px2(uint32_t iStitch) -> bool;
  void stchPars();
  void stchWnd();
  void stchbox(uint32_t iStitch, HDC dc);
  void stchout();
  void stchsnap(uint32_t start, uint32_t finish);
  auto stlen(uint32_t iStitch) noexcept -> float;
  void strtknt(std::vector<fPOINTATTR>& buffer, uint32_t start);
  void tglhid();
  void thr2bal(std::vector<BALSTCH>& balaradStitch, uint32_t destination, uint32_t source, uint32_t code);
  void thrsav();
  void thumbak();
  void thumnail();
  void toglHid();
  void toglup();
  void unbox();
  void unboxs() noexcept;
  void unclp();
  void uncros();
  void ungrphi();
  void ungrplo();
  void unlin();
  void unmov();
  void unpat();
  void unpclp();
  void unrot();
  void unrotu();
  void unsel();
  void unstrtch(std::vector<POINT>& stretchBoxLine);
  auto unthrsh(uint32_t level) noexcept -> double;
  void unthum();
  auto updateHoopSize() -> bool;
  auto updateFillColor() -> bool;
  auto updatePreferences() -> bool;
  auto usedcol(uint32_t& VerticalIndex) -> bool;
  void vubak();
  void vuselthr();
  void vuthrds();

  auto CALLBACK WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT;

  auto wastch(uint32_t const& formIndex) -> bool;
  void xlin();
  void xlin1();
  void zRctAdj() noexcept;
  void zumin();
  void zumout();
  void zumshft();

#if PESACT
  void setpes();
#endif

#ifdef ALLOCFAILURE
  auto handle_program_memory_depletion(uint32_t) -> int32_t;
#endif
} // namespace internal
} // namespace thred
