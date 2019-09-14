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
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace fs = std::filesystem;

namespace thred {

uint32_t adclp(uint32_t count);
uint32_t adflt(uint32_t count);

void     bBox() noexcept;
void     chkmen() noexcept;
void     chkrng(fPOINT& range);
void     coltab();
void     delfstchs();
void     delinf() noexcept;
void     delstchm();
uint32_t duthrsh(double threshold) noexcept;
void     duzrat() noexcept;
void     frmdel();
void     frmrct(fRECTANGLE& rectangle);
HBITMAP  getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits);
void     grpAdj();
void     hidbit();
void     movStch();

bool px2stch() noexcept;
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
void selRct(fRECTANGLE& sourceRect);
void setpsel();
void shft(const fPOINT& delta) noexcept;
void stch2pxr(const fPOINT& stitchCoordinate);
void stchrct(fRECTANGLE& rectangle) noexcept;
void strtchbox(std::vector<POINT>& stretchBoxLine) noexcept;

int32_t txtWid(const wchar_t* string) noexcept;

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
	bool binv(const std::vector<uint8_t>& monoBitmapData, uint32_t bitmapWidthInBytes);
	bool bitar();
	void bitlin(const uint8_t* source, uint32_t* destination, COLORREF foreground, COLORREF background);
	void bitsiz();
	void box(uint32_t iNearest, HDC dc) noexcept;
	void boxs() noexcept;
	void bsavmen();
	void centr();
	void chk1col();
	bool chkMsg(std::vector<POINT>& stretchBoxLine, float& xyRatio, float& angle, fPOINT& rotationCenter, FRMHED& textureForm);
	bool chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) noexcept;
	bool chkattr(const fs::path& filename);
	bool chkbig(std::vector<POINT>& stretchBoxLine, float& xyRatio);
	void chkbit();
	bool chkdst(const DSTHED* dstHeader) noexcept;
	bool chkhid(uint32_t colorToCheck);
	void chkhup();
	void chkirct() noexcept;
	bool chkminus(uint32_t code);
	void chkncol();
	void chknum();
	bool chkok() noexcept;

	uint32_t chkup(uint32_t count, uint32_t iStitch);

	bool     chkwnd(HWND window) noexcept;
	void     closPnt();
	bool     closPnt1(uint32_t* closestStitch);
	void     closfn();
	uint32_t closlin();
	void     clpadj();
	void     clpbox();
	void     clpradj(fRECTANGLE& clipRectAdjusted, const fPOINTATTR& stitch) noexcept;
	bool     cmpstch(uint32_t iStitchA, uint32_t iStitchB);
	void     colchk();
	DWORD    coldis(COLORREF colorA, COLORREF colorB);
	bool     colfil();
	uint32_t colmatch(COLORREF color);
	void     cros(uint32_t iStitch);
	void     crtcurs() noexcept;
	void     cut();
	void     defNam(const fs::path& fileName);
	COLORREF defTxt(uint32_t iColor);
	void     defbNam();
	void     defpref();
	void     delcol();
	void     deldir();
	void     deldu();
	void     delet();
	void     delfre();
	void     delknot();
	void     delknt();
	void     delmap();
	void     delsfrms(uint32_t code);
	void     delsmal(uint32_t startStitch, uint32_t endStitch);
	void     delstch();
	void     delstch1(uint32_t iStitch);
	void     deltot();
	void     desiz();

	BOOL CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	bool doPaste(std::vector<POINT>& stretchBoxLine, bool& retflag);
	void drawBackground();
	void drwLin(std::vector<POINT>& linePoints, uint32_t currentStitch, uint32_t length, HPEN hPen);
	void drwStch();
	void drwknot();
	void drwlstch(uint32_t finish);
	void drwmrk(HDC dc);
	void dstcurs() noexcept;
	void dstin(uint32_t number, POINT& pout) noexcept;
	void dstran(std::vector<DSTREC>& DSTData);

	uint32_t dtrn(DSTREC* dpnt);

	void duClos(uint32_t startStitch, uint32_t stitchCount);
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

	constexpr uint32_t dudbits(const POINT& dif);

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
	bool dunum(uint32_t code) noexcept;
	void dupclp();
	void duprct();
	void durit(std::vector<char>& destination, const void* source, uint32_t count);
	void durot() noexcept;
	void durotu() noexcept;
	void dusel(HDC dc);
	void duselrng();
	void dusid(uint32_t entry) noexcept;

	constexpr uint32_t byteSwap(uint32_t data) noexcept;

	void duver(const fs::path& name);
	void duzero();
	void endknt(std::vector<fPOINTATTR>& buffer, uint32_t finish);
	void endpnt();

	BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);

	void     esccode();
	void     f1del();
	void     fil2men();
	void     fil2sel(uint32_t stat);
	void     filclos();
	void     filfrms();
	bool     finrng(uint32_t find);
	void     fixpclp();
	void     fnamtabs();
	void     fndknt();
	void     fop();
	void     frmcalc();
	uint32_t frmcnt(uint32_t iForm, uint32_t& formFirstStitchIndex);
	void     frmcurmen();
	void     frmcursel(uint32_t cursorType);
	void     frmpos(float deltaX, float deltaY);
	void     frmsnap(uint32_t start, uint32_t count);
	bool     frmstch();
	COLORREF fswap(COLORREF color) noexcept;

	BOOL CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void     getbak();
	void     getdes() noexcept;
	void     getDocsFolder(fs::path* directory);
	void     getfrmbox();
	void     getfrmpix();
	uint32_t gethand(std::vector<fPOINTATTR>& stitch, uint32_t stitchCount);

	void getnpix();
	void getstpix();
	void gotbox();
	void gselrng();
	void gsnap();
	bool gudtyp(WORD bitCount) noexcept;
	bool handleEditMenu(const WORD& wParameter);
	bool handleEitherButtonDown(bool& retflag);
	bool handleEndKey(int32_t& retflag);
	bool handleFileMenu(const WORD& wParameter);
	bool handleFillMenu(const WORD& wParameter);
	bool handleFormDataSheet();
	bool handleHomeKey(bool& retflag);
	bool handleLeftButtonDown(std::vector<POINT>& stretchBoxLine,
	                          float&              xyRatio,
	                          float               rotationAngle,
	                          const FRMHED&       textureForm,
	                          bool&               retflag);
	bool handleLeftButtonUp(float xyRatio, float rotationAngle, fPOINT& rotationCenter, bool& retflag);
	bool handleLeftKey(bool& retflag);
	bool handleMainMenu(const WORD& wParameter, fPOINT& rotationCenter);
	bool handleMainWinKeys(const uint32_t& code, fPOINT& rotationCenter, std::vector<POINT>& stretchBoxLine, bool& retflag);
	bool handleMouseMove(std::vector<POINT>& stretchBoxLine,
	                     float               xyRatio,
	                     float&              rotationAngle,
	                     const fPOINT&       rotationCenter,
	                     const FRMHED&       textureForm);
	bool handleNumericInput(const uint32_t& code, bool& retflag);
	bool handleRightButtonDown();
	bool handleRightKey(bool& retflag);
	bool handleSideWindowActive();
	bool handleViewMenu(const WORD& wParameter);
	void hidknot();
	void hupfn();
	void ilin() noexcept;
	void ilin1() noexcept;
	void infadj(float& xCoordinate, float& yCoordinate) noexcept;
	void init();
	bool inrng(uint32_t stitch);
	void insadj();
	void inscol();
	void insfil();
	void insflin(POINT insertPoint);
	bool iselpnt();
	bool isfclp();
	void istch();
	bool isthr(const wchar_t* filename);

	uint32_t kjmp(std::vector<fPOINTATTR>& buffer, uint32_t start);
	void     knotmen();
	void     ladj();
	void     lenCalc();
	void     lenfn(uint32_t start, uint32_t end);
	void     lensadj();
	void     linbmen();
	void     lock();

	INT_PTR CALLBACK LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void     lodbmp();
	void     lodclp(uint32_t iStitch);
	void     longer();
	void     makCol();
	uint32_t makbig(uint32_t start, uint32_t finish);
	void     mark();
	void     movbak(char source, char destination);
	void     movbox();
	void     movchk();
	void     movi();
	void     movins();
	void     movmap(uint32_t cnt, uint8_t* buffer);
	void     movmrk();
	bool     movstchs(uint32_t destination, uint32_t start, uint32_t finish);
	void     mv2b();
	void     mv2f();
	void     nedmen();
	void     nedof();
	void     nedon();
	void     newFil();
	void     noMsg();
	void     nuAct(uint32_t iStitch);
	COLORREF nuBak() noexcept;
	COLORREF nuBit() noexcept;
	HBRUSH   nuBrush(HBRUSH brush, COLORREF color) noexcept;
	COLORREF nuCol(COLORREF init) noexcept;
	void     nuFil();
	HPEN     nuPen(HPEN pen, uint32_t width, COLORREF color) noexcept;
	void     nuRct();
	void     nuStchSiz(uint32_t iColor, uint32_t width) noexcept;
	float    nuang(float yDelta, float xDelta) noexcept;
	void     nucols();
	void     nudgfn(float deltaX, float deltaY);
	void     nulayr(uint32_t play);
	void     nunams();
	void     nuscol(uint32_t iColor) noexcept;
	void     nuselrct();
	void     nuslst(uint32_t find);
	void     nuthbak(uint32_t count);
	void     nuthsel();
	void     nuthum(char character);

	constexpr uint32_t nxtcrnr(uint32_t corner);

	void ofstch(std::vector<fPOINTATTR>& buffer, uint32_t iSource, char offset);
	bool oldwnd(HWND window);
	void ovrlay();
	void patdun();
	void pcsbsavof();
	void pcsbsavon();
	bool pcshup(std::vector<fPOINTATTR>& stitches);
	void pgdwn();
	void pglft();
	void pgrit();
	void pgup();

	void inline pixmsg(uint32_t iString, uint32_t pixelCount);

	void     pntmrk();
	void     prtred();
	uint32_t pt2colInd(uint32_t iStitch) noexcept;
	void     purg();
	void     purgdir();
	double   pxchk(double pixelSize) noexcept;
	void     qchk();
	void     qcode();
	void     rSelbox();
	void     rebak();
	void     rebox();
	void     redbak();
	void     redbal();
	void     redfils();
	void     redfnam(std::wstring& designerName);
	void     redini();
	void     redo();
	void     reldun();
	void     relin();
	void     rembig();
	void     respac();
	void     retrac();
	void     rint() noexcept;
	void     ritbak(const fs::path& fileName, DRAWITEMSTRUCT* drawItem);
	void     ritbal();
	void     ritcor(const fPOINTATTR& pointAttribute);
	void     ritcur() noexcept;
	void     ritdst(DSTOffsets& DSTOffsetData, std::vector<DSTREC>& DSTRecords, const std::vector<fPOINTATTR>& stitches);
	void     ritfnam(const std::wstring& designerName);
	void     ritini();
	void     ritlayr();
	void     ritloc();
	void     ritlock(const WIN32_FIND_DATA* fileData, uint32_t fileIndex, HWND hwndlg) noexcept;
	void     ritrot(float rotationAngle, const fPOINT& rotationCenter);
	void     rngal();
	void     rot(fPOINT& rotationCenter);
	void     rotang(fPOINT unrotatedPoint, POINT& rotatedPoint, float rotationAngle, const fPOINT& rotationCenter);
	void     rotauxmen();
	void     rotauxsel(uint32_t stat);
	void     rotfns(float rotationAngle);
	void     rotmrk();
	void     rotpix(const POINT& unrotatedPoint, POINT& rotatedPoint, const POINT& rotationCenterPixels);
	void     rotseg();
	void     rotstch(fPOINTATTR& stitch, float rotationAngle, const fPOINT& rotationCenter) noexcept;
	uint32_t rsed() noexcept;
	void     rshft(const POINT& shiftPoint);
	void     rstAll();
	void     rstdu();
	void     rthumnam(uint32_t iThumbnail);
	void     sachk();
	void     sav();
	void     savAs();
	void     savclp(uint32_t destination, std::vector<fPOINTATTR>& buffer, uint32_t source);
	bool     savcmp() noexcept;
	void     savdst(std::vector<DSTREC>& DSTRecords, uint32_t data);
	void     savmap();
	void     sdCor2px(fPOINTATTR stitchPoint, POINT& pixelCoordinate);
	void     segentr(float rotationAngle);
	void     selCol();
	void     selalstch();
	void     seldwn();
	void     selfpnt();
	void     selfrm0();
	void     selfrmx();
	void     selin(uint32_t start, uint32_t end, HDC dc);
	void     selup();
	void     set1knot();
	bool     setRmap(boost::dynamic_bitset<>& stitchMap, const fPOINTATTR& stitchPoint);
	void     setbak(uint32_t penWidth) noexcept;
	void     setdst();
	void     setgrd(COLORREF color);
	void     setknots();
	void     setknt();
	void     setmov();
	void     setpclp();
	void     setpcs();
	void     setPrefs();
	void     setsped();
	void     setsrch(uint32_t stitch);
	void     shft2box();
	void     shftflt(fPOINT point) noexcept;
	void     shoknot();
	void     shorter();
	bool     sidclp();
	void     sidhup();
	void     sidmsg(HWND window, std::wstring* strings, uint32_t entries);
	uint32_t sizclp(uint32_t& formFirstStitchIndex, uint32_t& formStitchCount);
	uint32_t sizfclp();
	void     sizstch(fRECTANGLE& rectangle, std::vector<fPOINTATTR>& stitches);
	void     srchk();
	uint32_t srchknot(uint32_t source) noexcept;

	inline void stCor2px(const fPOINTATTR& stitch, POINT& point);

	bool  stch2px(uint32_t iStitch);
	void  stch2px1(uint32_t iStitch);
	void  stchPars();
	void  stchWnd();
	void  stchbox(uint32_t iStitch, HDC dc);
	void  stchout();
	void  stchsnap(uint32_t start, uint32_t finish);
	float stlen(uint32_t iStitch);
	void  strtknt(std::vector<fPOINTATTR>& buffer, uint32_t start);
	void  tglhid();
	void  thr2bal(std::vector<BALSTCH>& balaradStitch, uint32_t destination, uint32_t source, uint32_t code);
	void  thrsav();
	void  thumbak();
	void  thumnail();
	void  toglHid();
	void  toglup();
	void  unbox();
	void  unboxs() noexcept;
	void  unclp();
	void  uncros();
	void  ungrphi();
	void  ungrplo();
	void  unlin();
	void  unmov();
	void  unpat();
	void  unpclp();
	void  unrot();
	void  unrotu();
	void  unsel();
	void  unstrtch(std::vector<POINT>& stretchBoxLine);

	double unthrsh(uint32_t level) noexcept;
	void   unthum();
	bool   updateHoopSize();
	bool   updateFillColor();
	bool   updatePreferences();
	bool   usedcol() noexcept;
	void   vubak();
	void   vuselthr();
	void   vuthrds();

	LRESULT CALLBACK WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool wastch() noexcept;
	void xlin();
	void xlin1();
	void zRctAdj() noexcept;
	void zumin();
	void zumout();
	void zumshft();

#if PESACT
	uint32_t dupcol(uint32_t activeColor);
	void     pecdat(std::vector<uint8_t>& buffer);
	void     pecEncodeint32_t(std::vector<uint8_t>& buffer, int32_t deltaY);
	void     pecEncodeStop(std::vector<uint8_t>& buffer, uint8_t val);
	void     pecImage(std::vector<uint8_t>& pecBuffer);
	void     pecnam(gsl::span<char> label);
	uint32_t pesmtch(const COLORREF& referenceColor, const uint8_t& colorIndex);
	void     ritpes(std::vector<uint8_t>& buffer, const fPOINTATTR& stitch);
	void     ritpesBlock(std::vector<uint8_t>& buffer, PESSTCHLST newBlock);
	void     ritpesCode(std::vector<uint8_t>& buffer);
	void     rpcrd(std::vector<uint8_t>& buffer, fPOINT& thisStitch, float srcX, float srcY);
	void     setpes();
	uint32_t tripl(char* dat);
	void     writeThumbnail(std::vector<uint8_t>& buffer, uint8_t const (*image)[ThumbHeight][ThumbWidth]);
#endif

#ifdef ALLOCFAILURE
	int32_t handle_program_memory_depletion(uint32_t);
#endif
} // namespace internal
} // namespace thred
