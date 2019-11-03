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
void frmrct(fRECTANGLE& rectangle);
auto getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP;
void grpAdj();
void hidbit();
void movStch();
auto px2stch() noexcept -> bool;
void pxCor2stch(const POINT& point) noexcept;
void redclp();
void redraw(HWND window) noexcept;
void ritfcor(const fPOINT& point);
void ritmov();
void ritot(uint32_t number);
void rngadj();
void rotang1(const fPOINTATTR& unrotatedPoint, fPOINT& rotatedPoint, float rotationAngle, const fPOINT& rotationCenter) noexcept;
void rotangf(const fPOINT& unrotatedPoint, fPOINT& rotatedPoint, float rotationAngle, const fPOINT& rotationCenter) noexcept;
void rotflt(fPOINT& point, float rotationAngle, const fPOINT& rotationCenter) noexcept;
void rotfn(float rotationAngle, const fPOINT& rotationCenter);
void rtclpfn(uint32_t destination, uint32_t source);
void sCor2px(const fPOINT& stitchCoordinate, POINT& pixelCoordinate);
void savdo();
void save();
void selRct(fRECTANGLE& sourceRect) noexcept;
void setpsel();
void shft(const fPOINT& delta) noexcept;
void stch2pxr(const fPOINT& stitchCoordinate);
void stchrct(fRECTANGLE& rectangle) noexcept;
void strtchbox(std::vector<POINT>& stretchBoxLine) noexcept;
auto txtWid(const wchar_t* string) noexcept -> int32_t;
void unbBox();
void unbsho();
void undat() noexcept;
void unmsg();
void unsid() noexcept;
void wrnmen();
void zumhom();

namespace internal {
	void angdif(float angle) noexcept;
	void auxmen();
	void bak();
	void bakmrk();
	void bakthum();
	void barnam(HWND window, uint32_t iThumbnail);
	void bfil();
	auto binv(const std::vector<uint8_t>& monoBitmapData, uint32_t bitmapWidthInBytes) -> bool;
	auto bitar() -> bool;
	void bitlin(const uint8_t* source, uint32_t* destination, COLORREF foreground, COLORREF background);
	void bitsiz();
	void box(uint32_t iNearest, HDC dc) noexcept;
	void boxs() noexcept;
	void bsavmen();
	void centr();
	void chk1col();
	auto chkMsg(std::vector<POINT>& stretchBoxLine, float& xyRatio, float& angle, fPOINT& rotationCenter, FRMHED& textureForm)
	    -> bool;
	auto chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) noexcept -> bool;
	auto chkattr(const fs::path& filename) -> bool;
	auto chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
	void chkbit();
	auto chkdst(const DSTHED* dstHeader) noexcept -> bool;
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
	auto closPnt1(uint32_t* closestStitch) -> bool;
	void closfn();
	auto closlin() -> uint32_t;
	void clpadj();
	void clpbox();
	void clpradj(fRECTANGLE& clipRectAdjusted, const fPOINTATTR& stitch) noexcept;
	auto cmpstch(uint32_t iStitchA, uint32_t iStitchB) noexcept -> bool;
	void colchk();
	auto coldis(COLORREF colorA, COLORREF colorB) -> DWORD;
	auto colfil() -> bool;
	auto colmatch(COLORREF color) -> uint32_t;
	void cros(uint32_t iStitch);
	void crtcurs() noexcept;
	void cut();
	void defNam(const fs::path& fileName);
	auto defTxt(uint32_t iColor) -> COLORREF;
	void defbNam();
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
	void dstin(uint32_t number, POINT& pout) noexcept;
	void dstran(std::vector<DSTREC>& DSTData);
	auto dtrn(DSTREC* dpnt) -> uint32_t;
	void duClos(uint32_t startStitch, uint32_t stitchCount) noexcept;
	void duIns();
	void duSelbox();
	void duar();
	void dubar();
	void dubox();
	void dubuf(std::vector<char>& buffer);
	void duclip();
	void duclp() noexcept;
	void ducmd();
	void ducros(HDC dc);
	void dudat();

	constexpr auto dudbits(const POINT& dif) -> uint32_t;

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
	void duprct();
	void durit(std::vector<char>& destination, const void* source, uint32_t count);
	void durot() noexcept;
	void durotu() noexcept;
	void dusel(HDC dc);
	void duselrng();
	void dusid(uint32_t entry) noexcept;

	constexpr auto byteSwap(uint32_t data) noexcept -> uint32_t;

	void duver(const fs::path& name);
	void duzero();
	void endknt(std::vector<fPOINTATTR>& buffer, uint32_t finish);
	void endpnt();

	auto CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) noexcept -> BOOL;

	void esccode();
	void f1del();
	void fil2men();
	void fil2sel(uint32_t stat);
	void filclos();
	void filfrms();
	auto finrng(uint32_t find) noexcept -> bool;
	void fixpclp(uint32_t closestFormToCursor);
	void fnamtabs();
	void fndknt();
	void fop();
	void frmcalc();
	auto frmcnt(uint32_t iForm, uint32_t& formFirstStitchIndex) -> uint32_t;
	void frmcurmen();
	void frmcursel(uint32_t cursorType);
	void frmpos(float deltaX, float deltaY);
	void frmsnap(uint32_t start, uint32_t count);
	auto frmstch() -> bool;
	auto fswap(COLORREF color) noexcept -> COLORREF;

	auto CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

	void getbak();
	void getdes() noexcept;
	void getDocsFolder(fs::path* directory);
	void getfrmbox();
	void getfrmpix();
	auto gethand(std::vector<fPOINTATTR>& stitch, uint32_t stitchCount) noexcept -> uint32_t;
	void getnpix();
	void getstpix();
	void gotbox();
	void gselrng() noexcept;
	void gsnap();

	constexpr auto gudtyp(WORD bitCount) noexcept -> bool;

	auto handleEditMenu(const WORD& wParameter) -> bool;
	auto handleEitherButtonDown(bool& retflag) -> bool;
	auto handleEndKey(int32_t& retflag) -> bool;
	auto handleFileMenu(const WORD& wParameter) -> bool;
	auto handleFillMenu(const WORD& wParameter) -> bool;
	auto handleFormDataSheet() -> bool;
	auto handleHomeKey(bool& retflag) -> bool;
	auto handleLeftButtonDown(std::vector<POINT>& stretchBoxLine,
	                          float&              xyRatio,
	                          float               rotationAngle,
	                          const FRMHED&       textureForm,
	                          bool&               retflag) -> bool;
	auto handleLeftButtonUp(float xyRatio, float rotationAngle, fPOINT& rotationCenter, bool& retflag) -> bool;
	auto handleLeftKey(bool& retflag) -> bool;
	auto handleMainMenu(const WORD& wParameter, fPOINT& rotationCenter) -> bool;
	auto handleMainWinKeys(const uint32_t& code, fPOINT& rotationCenter, std::vector<POINT>& stretchBoxLine, bool& retflag)
	    -> bool;
	auto handleMouseMove(std::vector<POINT>& stretchBoxLine,
	                     float               xyRatio,
	                     float&              rotationAngle,
	                     const fPOINT&       rotationCenter,
	                     const FRMHED&       textureForm) -> bool;
	auto handleNumericInput(const uint32_t& code, bool& retflag) -> bool;
	auto handleRightButtonDown() -> bool;
	auto handleRightKey(bool& retflag) -> bool;
	auto handleSideWindowActive() -> bool;
	auto handleViewMenu(const WORD& wParameter) -> bool;
	void hidknot();
	void hupfn();
	void ilin() noexcept;
	void ilin1() noexcept;
	void infadj(float& xCoordinate, float& yCoordinate) noexcept;
	void init();
	auto inrng(uint32_t stitch) noexcept -> bool;
	void insadj();
	void inscol();
	void insfil();
	void insflin(POINT insertPoint);
	auto iselpnt() noexcept -> bool;
	auto isfclp() noexcept -> bool;
	void istch();
	auto isthr(const wchar_t* filename) -> bool;
	auto kjmp(std::vector<fPOINTATTR>& buffer, uint32_t start) -> uint32_t;
	void knotmen();
	void ladj();
	void lenCalc();
	void lenfn(uint32_t start, uint32_t end);
	void lensadj();
	void linbmen();
	void lock();

	auto CALLBACK LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

	void lodbmp();
	void lodclp(uint32_t iStitch);
	void longer();
	void makCol() noexcept;
	auto makbig(uint32_t start, uint32_t finish) -> uint32_t;
	void mark();
	void movbak(char source, char destination);
	void movbox();
	void movchk();
	void movi();
	void movins();
	void movmap(uint32_t cnt, uint8_t* buffer);
	void movmrk();
	auto movstchs(uint32_t destination, uint32_t start, uint32_t finish) -> bool;
	void mv2b();
	void mv2f();
	void nedmen();
	void nedof();
	void nedon();
	void newFil();
	void noMsg();
	void nuAct(uint32_t iStitch) noexcept;
	auto nuBak() noexcept -> COLORREF;
	auto nuBit() noexcept -> COLORREF;
	auto nuBrush(HBRUSH brush, COLORREF color) noexcept -> HBRUSH;
	auto nuCol(COLORREF init) noexcept -> COLORREF;
	void nuFil();
	auto nuPen(HPEN pen, uint32_t width, COLORREF color) noexcept -> HPEN;
	void nuRct() noexcept;
	void nuStchSiz(uint32_t iColor, uint32_t width) noexcept;
	auto nuang(float yDelta, float xDelta) noexcept -> float;
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

	void ofstch(std::vector<fPOINTATTR>& buffer, uint32_t iSource, char offset);
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
	void prtred();
	auto pt2colInd(uint32_t iStitch) noexcept -> uint32_t;
	void purg();
	void purgdir();
	auto pxchk(double pixelSize) noexcept -> double;
	void qchk();
	void qcode();
	void rSelbox();
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
	void respac() noexcept;
	void retrac();
	void rint() noexcept;
	void ritbak(const fs::path& fileName, DRAWITEMSTRUCT* drawItem);
	void ritbal();
	void ritcor(const fPOINTATTR& pointAttribute);
	void ritcur() noexcept;
	void ritdst(DSTOffsets& DSTOffsetData, std::vector<DSTREC>& DSTRecords, const std::vector<fPOINTATTR>& stitches);
	void ritfnam(const std::wstring& designerName);
	void ritini();
	void ritlayr();
	void ritloc();
	void ritlock(const WIN32_FIND_DATA* fileData, uint32_t fileIndex, HWND hwndlg) noexcept;
	void ritrot(float rotationAngle, const fPOINT& rotationCenter);
	void rngal();
	void rot(fPOINT& rotationCenter);
	void rotang(fPOINT unrotatedPoint, POINT& rotatedPoint, float rotationAngle, const fPOINT& rotationCenter);
	void rotauxmen();
	void rotauxsel(uint32_t stat);
	void rotfns(float rotationAngle);
	void rotmrk();
	void rotpix(const POINT& unrotatedPoint, POINT& rotatedPoint, const POINT& rotationCenterPixels);
	void rotseg();
	void rotstch(fPOINTATTR& stitch, float rotationAngle, const fPOINT& rotationCenter) noexcept;
	auto rsed() noexcept -> uint32_t;
	void rshft(const POINT& shiftPoint);
	void rstAll();
	void rstdu();
	void rthumnam(uint32_t iThumbnail);
	void sachk();
	void sav();
	void savAs();
	void savclp(uint32_t destination, std::vector<fPOINTATTR>& buffer, uint32_t source);
	auto savcmp() noexcept -> bool;
	void savdst(std::vector<DSTREC>& DSTRecords, uint32_t data);
	void savmap();
	void sdCor2px(fPOINTATTR stitchPoint, POINT& pixelCoordinate);
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
	auto setRmap(boost::dynamic_bitset<>& stitchMap, const fPOINTATTR& stitchPoint) -> bool;
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
	void sidmsg(HWND window, std::wstring* strings, uint32_t entries);
	auto sizclp(uint32_t& formFirstStitchIndex, uint32_t& formStitchCount) -> uint32_t;
	auto sizfclp() -> uint32_t;
	void sizstch(fRECTANGLE& rectangle, std::vector<fPOINTATTR>& stitches) noexcept;
	void srchk();
	auto srchknot(uint32_t source) noexcept -> uint32_t;
	void stCor2px(const fPOINTATTR& stitch, POINT& point);
	auto stch2px(uint32_t iStitch) -> bool;
	void stch2px1(uint32_t iStitch);
	void stchPars();
	void stchWnd();
	void stchbox(uint32_t iStitch, HDC dc);
	void stchout();
	void stchsnap(uint32_t start, uint32_t finish) noexcept;
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
	auto usedcol() noexcept -> bool;
	void vubak();
	void vuselthr();
	void vuthrds();

	auto CALLBACK WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT;

	auto wastch() noexcept -> bool;
	void xlin();
	void xlin1();
	void zRctAdj() noexcept;
	void zumin();
	void zumout();
	void zumshft();

#if PESACT
	auto dupcol(uint32_t activeColor) -> uint32_t;
	void pecdat(std::vector<uint8_t>& buffer);
	void pecEncodeint32_t(std::vector<uint8_t>& buffer, int32_t deltaY);
	void pecEncodeStop(std::vector<uint8_t>& buffer, uint8_t val);
	void pecImage(std::vector<uint8_t>& pecBuffer);
	void pecnam(gsl::span<char> label);
	auto pesmtch(const COLORREF& referenceColor, const uint8_t& colorIndex) -> uint32_t;
	void ritpes(std::vector<uint8_t>& buffer, const fPOINTATTR& stitch);
	void ritpesBlock(std::vector<uint8_t>& buffer, PESSTCHLST newBlock);
	void ritpesCode(std::vector<uint8_t>& buffer);
	void rpcrd(std::vector<uint8_t>& buffer, fPOINT& thisStitch, float srcX, float srcY);
	void setpes();
	auto tripl(char* dat) -> uint32_t;
	void writeThumbnail(std::vector<uint8_t>& buffer, uint8_t const (*image)[ThumbHeight][ThumbWidth]);
#endif

#ifdef ALLOCFAILURE
	auto handle_program_memory_depletion(uint32_t) -> int32_t;
#endif
} // namespace internal
} // namespace thred
