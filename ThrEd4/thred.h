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
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace fs = std::experimental::filesystem;

namespace thred {

fPOINT*  adclp(size_t count) noexcept;
fPOINT*  adflt(size_t count);
void     bBox() noexcept;
void     chkmen() noexcept;
void     chkrng(fPOINT& range) noexcept;
void     coltab();
void     delfstchs() noexcept;
void     delinf() noexcept;
void     delstchm();
unsigned duthrsh(double threshold) noexcept;
void     duzrat() noexcept;
void     frmdel();
void     frmrct(fRECTANGLE& rectangle) noexcept;
HBITMAP  getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ unsigned int** ppvBits);
void     grpAdj();
void     hidbit();
void     movStch();

void inline mvstch(size_t destination, size_t source) noexcept;

void mvstchs(unsigned destination, unsigned source, unsigned count);

unsigned px2stch() noexcept;

void pxCor2stch(const POINT& point) noexcept;
void redclp();
void redraw(HWND window) noexcept;
void ritfcor(const fPOINT& point);
void ritmov();
void ritot(unsigned number);
void rngadj();
void rotang1(const fPOINTATTR& unrotatedPoint,
             fPOINT&           rotatedPoint,
             const double&     rotationAngle,
             const dPOINT&     rotationCenter) noexcept;
void rotangf(const fPOINT& unrotatedPoint,
             fPOINT&       rotatedPoint,
             const double  rotationAngle,
             const dPOINT& rotationCenter) noexcept;

void rotflt(fPOINT& point, const double rotationAngle, const dPOINT& rotationCenter) noexcept;
void rotfn(double rotationAngle, const dPOINT& rotationCenter);
void rtclpfn(size_t destination, size_t source) noexcept;
void sCor2px(const dPOINT& stitchCoordinate, POINT& pixelCoordinate) noexcept;
void savdo();
void save();
void selRct(fRECTANGLE& sourceRect) noexcept;
void setpsel();
void shft(const fPOINT& delta) noexcept;
void stch2pxr(const fPOINT& stitchCoordinate) noexcept;
void stchrct(fRECTANGLE& rectangle) noexcept;
void strtchbox(std::vector<POINT>& stretchBoxLine) noexcept;
int  txtWid(const wchar_t* string);
void unbBox();
void unbsho();
void undat() noexcept;
void unmsg();
void unsid() noexcept;
void wrnmen();
void zumhom();

namespace internal {
	void  angdif(double angle) noexcept;
	void  auxmen();
	void  bak();
	void  bakmrk();
	void  bakthum();
	void  bal2thr(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code);
	void  barnam(HWND window, unsigned iThumbnail);
	void  bfil();
	bool  binv(const std::vector<unsigned char>& monoBitmapData, unsigned bitmapWidthInBytes);
	bool  bitar();
	void  bitlin(const unsigned char* source, unsigned* destination, COLORREF foreground, COLORREF background);
	void  bitsiz() noexcept;
	void  box(unsigned iNearest, HDC dc) noexcept;
	void  boxs() noexcept;
	void  bsavmen();
	float bufToDouble(wchar_t* buffer);

	unsigned long long bufTou64(wchar_t* buffer);

	void     centr() noexcept;
	void     chk1col();
	unsigned chkMsg(std::vector<POINT>& stretchBoxLine, double& xyRatio, double& rotationAngle, dPOINT& rotationCenter);
	unsigned chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) noexcept;
	bool     chkattr(const fs::path& filename);
	bool     chkbig(std::vector<POINT>& stretchBoxLine, double& xyRatio);
	void     chkbit();
	bool     chkdst(const DSTHED* dstHeader) noexcept;
	bool     chkhid(unsigned colorToCheck);
	void     chkhup();
	void     chkirct() noexcept;
	bool     chkminus(unsigned code) noexcept;
	void     chkncol();
	void     chknum();
	bool     chkok() noexcept;
	unsigned chkup(unsigned count, unsigned iStitch);
	bool     chkwnd(HWND window) noexcept;
	void     closPnt();
	bool     closPnt1(unsigned* closestStitch);
	void     closfn();
	unsigned closlin();
	void     clpadj();
	void     clpbox();
	void     clpradj(fPOINTATTR stitch) noexcept;
	bool     cmpstch(unsigned iStitchA, unsigned iStitchB) noexcept;
	void     colchk() noexcept;

	constexpr unsigned coldis(COLORREF colorA, COLORREF colorB);

	bool     colfil();
	unsigned colmatch(COLORREF color) noexcept;
	void     cros(unsigned iStitch) noexcept;
	void     crtcurs() noexcept;
	void     cut();
	void     defNam(const fs::path& fileName);
	COLORREF defTxt(unsigned iColor);
	void     defbNam();
	void     defpref();
	void     delcol();
	void     deldir();
	void     deldu();
	void     delet();
	void     delfre();
	void     delknot();
	void     delknt() noexcept;
	void     delmap();
	void     delsfrms(unsigned code);
	void     delsmal(unsigned startStitch, unsigned endStitch);
	void     delstch();
	void     delstch1(unsigned iStitch);
	void     deltot();
	void     desiz();
	BOOL CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void drwLin(std::vector<POINT>& linePoints, unsigned currentStitch, unsigned length, HPEN hPen);
	void drwStch();
	void drwknot();
	void drwlstch(unsigned finish);
	void drwmrk(HDC dc) noexcept;
	void dstcurs() noexcept;
	void dstin(unsigned number, POINT& pout) noexcept;
	void dstran(std::vector<DSTREC>& DSTData);

	inline unsigned dtrn(DSTREC* dpnt);

	void duClos(unsigned startStitch, unsigned stitchCount);
	void duIns();
	void duSelbox() noexcept;
	void duar();
	void dubar();
	void dubox();
	void dubuf(char* const buffer, unsigned& count);
	void duclip();
	void duclp() noexcept;
	void ducmd();
	void ducros(HDC dc);
	void dudat();

	constexpr unsigned dudbits(const POINT& dif);

	void     dufdef() noexcept;
	void     dufsel();
	void     dugrid();
	void     duhbit(unsigned cod) noexcept;
	void     duhom();
	void     duinsfil();
	void     dulin();
	void     dumov();
	void     dumrk(double xCoord, double yCoord);
	void     dun();
	bool     dunum(unsigned code) noexcept;
	void     dupclp();
	void     duprct();
	void     durcntr(dPOINT& rotationCenter) noexcept;
	void     durit(char** destination, const void* const source, size_t count) noexcept;
	void     durot() noexcept;
	void     durotu() noexcept;
	void     dusel(HDC dc);
	void     duselrng();
	void     dusid(unsigned entry) noexcept;
	unsigned duswap(unsigned data) noexcept;
	void     duver(const fs::path& name);
	void     duzero();
	void     endknt(unsigned finish);
	void     endpnt();

	BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);

	void     esccode();
	void     f1del();
	void     fil2men();
	void     fil2sel(unsigned stat);
	void     filclos();
	void     filfrms();
	bool     finrng(size_t find) noexcept;
	void     fixpclp();
	int      fltex(int code) noexcept;
	void     fnamtabs();
	void     fndknt();
	void     fop();
	void     frmcalc();
	unsigned frmcnt(size_t iForm, unsigned formFirstStitchIndex) noexcept;
	void     frmcurmen();
	void     frmcursel(unsigned cursorType);
	void     frmpos(float deltaX, float deltaY) noexcept;
	void     frmsnap(fPOINT* start, size_t count) noexcept;
	bool     frmstch();
	COLORREF fswap(COLORREF color) noexcept;

	BOOL CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void     getbak();
	void     getdes() noexcept;
	void     getDocsFolder(fs::path* directory);
	void     getfrmbox();
	void     getfrmpix();
	unsigned gethand(const fPOINTATTR* stitch, unsigned stitchCount) noexcept;
	void     getnpix();
	void     getstpix();
	void     gotbox();
	void     gselrng();
	void     gsnap();
	bool     gudtyp(WORD bitCount) noexcept;
	void     hidknot();
	void     hupfn();
	void     ilin() noexcept;
	void     ilin1() noexcept;
	void     infadj(float* xCoordinate, float* yCoordinate) noexcept;
	void     init();
	bool     inrng(unsigned stitch) noexcept;
	void     insadj();
	void     inscol();
	void     cpyTmpGuides(std::vector<SATCONOUT>& inSatinGuides);
	void     insfil();
	void     insflin(POINT insertPoint) noexcept;
	bool     iselpnt();
	bool     isfclp() noexcept;
	bool     isknots() noexcept;
	void     istch();
	bool     isthr(const wchar_t* const filename);
	unsigned kjmp(unsigned start);
	void     knotmen();
	void     ladj();
	void     lenCalc();
	unsigned lenclp();
	void     lenfn(unsigned start, unsigned end);
	void     lensadj();
	void     linbmen();
	void     lock();

	INT_PTR CALLBACK LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

	void     lodbmp();
	void     lodclp(unsigned iStitch);
	void     longer();
	void     makCol() noexcept;
	unsigned makbig(unsigned start, unsigned finish);
	void     mark();
	void     movbak(char source, char destination);
	void     movbox();
	void     movchk();
	void     movi();
	void     movins();
	void     movmap(unsigned cnt, unsigned char* buffer);
	void     movmrk();
	bool     movstchs(unsigned destination, unsigned start, unsigned finish);
	void     mv2b();
	void     mv2f();

	void     nedmen();
	void     nedof();
	void     nedon();
	void     newFil();
	void     noMsg();
	void     nuAct(unsigned iStitch) noexcept;
	COLORREF nuBak() noexcept;
	COLORREF nuBit() noexcept;
	HBRUSH   nuBrush(HBRUSH brush, COLORREF color) noexcept;
	COLORREF nuCol(COLORREF init) noexcept;
	void     nuFil();
	HPEN     nuPen(HPEN pen, unsigned width, COLORREF color) noexcept;
	void     nuRct();
	void     nuStchSiz(unsigned iColor, unsigned width) noexcept;
	double   nuang(double yDelta, double xDelta) noexcept;
	void     nucols();
	void     nudgfn(float deltaX, float deltaY);
	void     nulayr(unsigned play);
	void     nunams();
	void     nuscol(unsigned iColor) noexcept;
	void     nuselrct();
	void     nuslst(size_t find);
	void     nuthbak(unsigned count);
	void     nuthsel();
	void     nuthum(char character);

	constexpr unsigned nxtcrnr(unsigned corner);

	void ofstch(unsigned iSource, char offset) noexcept;
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

	void inline pixmsg(unsigned iString, unsigned pixelCount);

	void pntmrk();
	void prtred();

	constexpr unsigned pt2colInd(unsigned iStitch);

	void   purg();
	void   purgdir();
	double pxchk(double pixelSize) noexcept;
	void   qchk();
	void   qcode();
	void   rSelbox();
	void   rebak();
	void   rebox();
	void   redbak();
	void   cpyUndoGuides(const BAKHED* undoData);
	void   redbal();
	void   redfils();
	void   redfnam(std::wstring& designerName);
	void   redini();
	void   redo();
	void   reldun();
	void   relin();
	void   rembig();
	void   respac() noexcept;
	void   retrac();
	void   rint() noexcept;
	void   ritbak(const fs::path& fileName, DRAWITEMSTRUCT* drawItem);
	void   ritbal();
	void   ritcor(const fPOINTATTR& pointAttribute);
	void   ritcur() noexcept;
	void   ritdst(DSTOffsets& DSTOffsetData, std::vector<DSTREC>& DSTRecords, const std::vector<fPOINTATTR>& stitches);
	void   ritfnam(const std::wstring& designerName);
	void   ritini();
	void   ritlayr();
	void   ritloc();
	void   ritlock(const WIN32_FIND_DATA* fileData, unsigned fileIndex, HWND hwndlg) noexcept;
	void   ritrot(double rotationAngle, const dPOINT& rotationCenter);
	void   rngal();
	void   rot(dPOINT& rotationCenter);
	void   rotang(dPOINT unrotatedPoint, POINT& rotatedPoint, double rotationAngle, const dPOINT& rotationCenter);

	void        rotauxmen();
	void        rotauxsel(unsigned stat);
	void        rotfns(double rotationAngle, const dPOINT& rotationCenter);
	void        rotmrk();
	void        rotpix(const POINT& unrotatedPoint, POINT& rotatedPoint, const POINT& rotationCenterPixels);
	void        rotseg();
	void        rotstch(fPOINTATTR* stitch, const double rotationAngle, const dPOINT& rotationCenter) noexcept;
	unsigned    rsed() noexcept;
	void        rshft(const POINT& shiftPoint);
	void        rstAll();
	void        rstdu();
	void        rthumnam(unsigned iThumbnail);
	void        sachk() noexcept;
	void        sav();
	void        savAs();
	void        savclp(size_t destination, size_t source) noexcept;
	bool        savcmp() noexcept;
	void        savdst(std::vector<DSTREC>& DSTRecords, unsigned data);
	void        savmap();
	void        sdCor2px(const fPOINTATTR stitchPoint, POINT& pixelCoordinate) noexcept;
	void        segentr(double rotationAngle);
	void        selCol();
	void        selalstch();
	void        seldwn();
	void        selfpnt();
	void        selfrm0();
	void        selfrmx();
	void        selin(unsigned start, unsigned end, HDC dc);
	void        selup();
	void        set1knot();
	bool        setRmap(boost::dynamic_bitset<>& stitchMap, const fPOINTATTR& stitchPoint);
	void        setbak(unsigned penWidth) noexcept;
	void        setdst();
	void        setgrd(COLORREF color);
	void        setknots();
	void        setknt();
	void        setmov();
	void        setpclp() noexcept;
	void        setpcs();
	void        setsped();
	void        setsrch(unsigned stitch);
	void        shft2box() noexcept;
	void        shftflt(fPOINT point) noexcept;
	void        shoknot();
	void        shorter();
	bool        sidclp();
	void        sidhup();
	void        sidmsg(HWND window, std::wstring* const strings, unsigned entries);
	size_t      sizclp(unsigned formFirstStitchIndex) noexcept;
	size_t      sizfclp() noexcept;
	void        sizstch(fRECTANGLE& rectangle, const fPOINTATTR* const stitches) noexcept;
	void        srchk();
	unsigned    srchknot(unsigned source) noexcept;
	inline void stCor2px(const fPOINTATTR& stitch, POINT& point) noexcept;
	unsigned    stch2px(unsigned iStitch) noexcept;
	void        stch2px1(unsigned iStitch) noexcept;
	void        stchPars();
	void        stchWnd();
	void        stchbox(unsigned iStitch, HDC dc) noexcept;
	void        stchout();
	void        stchsnap(unsigned start, unsigned finish) noexcept;
	double      stlen(unsigned iStitch) noexcept;
	int         strcomp(const void* arg1, const void* arg2) noexcept;
	void        strtknt(unsigned start) noexcept;
	void        tglhid();
	void        thr2bal(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code);
	void        thrsav();
	void        thumbak();
	void        thumnail();
	void        toglHid();
	void        toglup();

	void   unbox();
	void   unboxs() noexcept;
	void   unclp();
	void   uncros();
	void   ungrphi();
	void   ungrplo();
	void   unlin();
	void   unmov();
	void   unpat();
	void   unpclp();
	void   unrot();
	void   unrotu();
	void   unsel();
	void   unstrtch(std::vector<POINT>& stretchBoxLine);
	double unthrsh(unsigned level) noexcept;
	void   unthum();
	bool   usedcol() noexcept;
	void   vubak();
	void   vuselthr();
	void   vuthrds();

	LRESULT CALLBACK WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow);
	bool         wastch() noexcept;
	void         xlin();
	void         xlin1();
	void         zRctAdj() noexcept;
	void         zumin();
	void         zumout();
	void         zumshft();

#if PESACT
	double   dubl(unsigned char* pnt) noexcept;
	unsigned dupcol() noexcept;
	void     pecdat(unsigned char* buffer);
	unsigned pesmtch(COLORREF referenceColor, unsigned char colorIndex) noexcept;
	unsigned pesnam() noexcept;
	void     ritpcol(unsigned char colorIndex) noexcept;
	void     ritpes(unsigned iStitch, const std::vector<fPOINTATTR>& stitches);
	void     rpcrd(float stitchDelta) noexcept;
	void     setpes();
	unsigned tripl(char* dat);
#endif

#ifdef ALLOCFAILURE
	int handle_program_memory_depletion(size_t);
#endif
} // namespace internal
} // namespace ThrEd