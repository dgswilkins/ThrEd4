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

fPOINT* adclp(size_t count);
fPOINT* adflt(size_t count);
SATCON* adsatk(size_t count);
//void    angdif(double angle);
//void    auxmen();
void    bBox();
//void    bak();
//void    bakmrk();
//void    bakthum();
//void    bal2thr(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code);
//void    barnam(HWND window, unsigned iThumbnail);
//void    bfil();
//void    bfrm();
//bool    binv(const std::vector<unsigned char>& monoBitmapData, unsigned bitmapWidthInBytes);
//bool    bitar();
//void    bitlin(const unsigned char* source, unsigned* destination, COLORREF foreground, COLORREF background);
//void    bitsiz();
//void    box(unsigned iNearest, HDC dc);
//void    boxs();
//void    bsavmen();
//float   bufToDouble(wchar_t* buffer);

//unsigned long long bufTou64(wchar_t* buffer);

//void     centr();
//void     chk1col();
//unsigned chkMsg(std::vector<POINT>& stretchBoxLine, double& xyRatio, double& rotationAngle, dPOINT& rotationCenter);
//unsigned chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow);
//bool     chkattr(const fs::path& filename);
//bool     chkbig(std::vector<POINT>& stretchBoxLine, double& xyRatio);
//void     chkbit();
//bool     chkdst(const DSTHED* dstHeader);
//bool     chkhid(unsigned colorToCheck);
//void     chkhup();
//void     chkirct();
void     chkmen();
//bool     chkminus(unsigned code);
//void     chkncol();
//void     chknum();
//bool     chkok();
//void     chkref();
void     chkrng(fPOINT& range);
//unsigned chkup(unsigned count, unsigned iStitch);
//bool     chkwnd(HWND window);
//void     closPnt();
//bool     closPnt1(unsigned* closestStitch);
//void     closfn();
//unsigned closlin();
//void     clpadj();
//void     clpbox();
//void     clpradj(fPOINTATTR stitch);
//void     clrhbut(unsigned startButton);
//bool     cmpstch(unsigned iStitchA, unsigned iStitchB);
//void     colchk();

//constexpr unsigned coldis(COLORREF colorA, COLORREF colorB);

//bool     colfil();
//unsigned colmatch(COLORREF color);
void     coltab();
//void     cros(unsigned iStitch);
//void     crtcurs();
//void     cut();
//void     defNam(const fs::path& fileName);
//COLORREF defTxt(unsigned iColor);
//void     defbNam();
//void     defpref();
//void     delcol();
//void     deldir();
//void     deldu();
//void     delet();
//void     delfre();
void     delfstchs();
void     delinf();
//void     delknot();
//void     delknt();
//void     delmap();
//void     delsfrms(unsigned code);
//void     delsmal(unsigned startStitch, unsigned endStitch);
//void     delstch();
//void     delstch1(unsigned iStitch);
void     delstchm();
//void     deltot();
//void     desiz();
//BOOL CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

//void drwLin(std::vector<POINT>& linePoints, unsigned currentStitch, unsigned length, HPEN hPen);
//void drwStch();
//void drwknot();
//void drwlstch(unsigned finish);
//void drwmrk(HDC dc);
//void dstcurs();
//void dstin(unsigned number, POINT& pout);
//void dstran(std::vector<DSTREC>& DSTData);

//inline unsigned dtrn(DSTREC* dpnt);

//void     duClos(unsigned startStitch, unsigned stitchCount);
//void     duIns();
//void     duSelbox();
//void     duar();
//void     dubar();
//void     dubox();
//void     dubuf(char* const buffer, unsigned& count);
//void     duclip();
//void     duclp();
//void     ducmd();
//void     ducros(HDC dc);
//void     dudat();

//constexpr unsigned dudbits(const POINT& dif);

//void dufdef();
//void dufsel();
//void dugrid();
//void duhbit(unsigned cod);
//void duhom();
//void duinsfil();
//void dulin();
//void dumov();
//void dumrk(double xCoord, double yCoord);
//void dun();
//bool dunum(unsigned code);
//void dupclp();
//void duprct();
//void durcntr(dPOINT& rotationCenter) noexcept;
//void     durit(char** destination, const void* const source, size_t count);
//void     durot();
//void     durotu();
//void     dusel(HDC dc);
//void     duselrng();
//void     dusid(unsigned entry);
//unsigned duswap(unsigned data);
unsigned duthrsh(double threshold);
//void     duver(const fs::path& name);
//void     duzero();
void     duzrat();
//void     endknt(unsigned finish);
//void     endpnt();

//BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);

//void     esccode();
//void     f1del();
//void     fil2men();
//void     fil2sel(unsigned stat);
//void     filclos();
//void     filfrms();
//bool     finrng(size_t find);
//void     fixpclp();
//int      fltex(int code);
//void     fnamtabs();
//void     fndknt();
//void     fop();
//void     frmcalc();
//unsigned frmcnt(size_t iForm, unsigned formFirstStitchIndex);
//void     frmcurmen();
//void     frmcursel(unsigned cursorType);
void     frmdel();
//void     frmpos(float deltaX, float deltaY);
void     frmrct(fRECTANGLE& rectangle);
//void     frmsnap(fPOINT* start, size_t count);
//bool     frmstch();
//COLORREF fswap(COLORREF color);

//BOOL CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

HBITMAP  getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ unsigned int** ppvBits);
//void     getbak();
//void     getdes();
//void     getfrmbox();
//void     getfrmpix();
//unsigned gethand(const fPOINTATTR* stitch, unsigned stitchCount);
//void     getnpix();
//void     getstpix();
//void     gotbox();
void     grpAdj();
//void     gselrng();
//void     gsnap();
//bool     gudtyp(WORD bitCount);
void     hidbit();
//void     hidknot();
//void     hupfn();
//void     ilin();
//void     ilin1();
//void     infadj(float* xCoordinate, float* yCoordinate);
//void     init();
//bool     inrng(unsigned stitch);
//void     insadj();
//void     inscol();
//void     insfil();
//void     insflin(POINT insertPoint);
bool     isclp(size_t iForm);
bool     isclpx(size_t iForm);
bool     iseclp(size_t iForm);
//bool     iselpnt();
//bool     isfclp();
//bool     isknots();
//void     istch();
//bool     isthr(const wchar_t* const filename);
//unsigned kjmp(unsigned start);
//void     knotmen();
//void     ladj();
//void     lenCalc();
//unsigned lenclp();
//void     lenfn(unsigned start, unsigned end);
//void     lensadj();
//void     linbmen();
//void     lock();

//INT_PTR CALLBACK LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam);

//void     lodbmp();
//void     lodclp(unsigned iStitch);
//void     longer();
//void     makCol();
//unsigned makbig(unsigned start, unsigned finish);
//void     mark();
void     movStch();
//void     movbak(char source, char destination);
//void     movbox();
//void     movchk();
//void     movi();
//void     movins();
//void     movmap(unsigned cnt, unsigned char* buffer);
//void     movmrk();
//bool     movstchs(unsigned destination, unsigned start, unsigned finish);
//void     mv2b();
//void     mv2f();

void inline mvstch(size_t destination, size_t source);

void     mvstchs(unsigned destination, unsigned source, unsigned count);
//void     nedmen();
//void     nedof();
//void     nedon();
//void     newFil();
//void     noMsg();
//void     nuAct(unsigned iStitch);
//COLORREF nuBak();
//COLORREF nuBit();
//HBRUSH   nuBrush(HBRUSH brush, COLORREF color);
//COLORREF nuCol(COLORREF init);
//void     nuFil();
//HPEN     nuPen(HPEN pen, unsigned width, COLORREF color);
//void     nuRct();
//void     nuStchSiz(unsigned iColor, unsigned width);
//double   nuang(double yDelta, double xDelta);
//void     nucols();
//void     nudgfn(float deltaX, float deltaY);
//void     nulayr(unsigned play);
//void     nunams();
//void     nuscol(unsigned iColor);
//void     nuselrct();
//void     nuslst(size_t find);
//void     nuthbak(unsigned count);
//void     nuthsel();
//void     nuthum(char character);

//constexpr unsigned nxtcrnr(unsigned corner);

//void ofstch(unsigned iSource, char offset);
//bool oldwnd(HWND window);
//void ovrlay();
//void patdun();
//void pcsbsavof();
//void pcsbsavon();
//bool pcshup(std::vector<fPOINTATTR>& stitches);
//void pgdwn();
//void pglft();
//void pgrit();
//void pgup();

//void inline pixmsg(unsigned iString, unsigned pixelCount);

//void pntmrk();
//void prtred();

//constexpr unsigned pt2colInd(unsigned iStitch);

//void     purg();
//void     purgdir();
unsigned px2stch();
void     pxCor2stch(const POINT& point);
//double   pxchk(double pixelSize);
//void     qchk();
//void     qcode();
//void     rSelbox();
//void     rebak();
//void     rebox();
//void     redbak();
//void     redbal();
void     redclp();
//void     redfils();
//void     redfnam(std::wstring& designerName);
//void     redini();
//void     redo();
void     redraw(HWND window);
//void     reldun();
//void     relin();
//void     rembig();
//void     respac();
//void     retrac();
//void     rint();
//void     ritbak(const fs::path& fileName, DRAWITEMSTRUCT* drawItem);
//void     ritbal();
//void     ritcor(const fPOINTATTR& pointAttribute);
//void     ritcur();
//void     ritdst(DSTOffsets& DSTOffsetData, std::vector<DSTREC>& DSTRecords, const std::vector<fPOINTATTR>& stitches);
void     ritfcor(const fPOINT& point);
//void     ritfnam(const std::wstring& designerName);
//void     ritini();
//void     ritlayr();
//void     ritloc();
//void     ritlock(const WIN32_FIND_DATA* fileData, unsigned fileIndex, HWND hwndlg);
void     ritmov();
void     ritot(unsigned number);
//void     ritrot(double rotationAngle, const dPOINT& rotationCenter);
void     rngadj();
//void     rngal();
//void     rot(dPOINT& rotationCenter);
//void     rotang(dPOINT unrotatedPoint, POINT& rotatedPoint, double rotationAngle, const dPOINT& rotationCenter);

void rotang1(const fPOINTATTR& unrotatedPoint, fPOINT& rotatedPoint, const double& rotationAngle, const dPOINT& rotationCenter);
void rotangf(const fPOINT& unrotatedPoint, fPOINT& rotatedPoint, const double rotationAngle, const dPOINT& rotationCenter);

//void        rotauxmen();
//void        rotauxsel(unsigned stat);
void        rotflt(fPOINT& point, const double rotationAngle, const dPOINT& rotationCenter);
void        rotfn(double rotationAngle, const dPOINT& rotationCenter);
//void        rotfns(double rotationAngle, const dPOINT& rotationCenter);
//void        rotmrk();
//void        rotpix(const POINT& unrotatedPoint, POINT& rotatedPoint, const POINT& rotationCenterPixels);
//void        rotseg();
//void        rotstch(fPOINTATTR* stitch, const double rotationAngle, const dPOINT& rotationCenter);
//unsigned    rsed();
//void        rshft(const POINT& shiftPoint);
//void        rstAll();
//void        rstdu();
//void        rtclpfn(size_t destination, size_t source);
//void        rthumnam(unsigned iThumbnail);
void        rtrclpfn();
void        sCor2px(const dPOINT& stitchCoordinate, POINT& pixelCoordinate);
//void        sachk();
//void        sav();
//void        savAs();
//void        savclp(size_t destination, size_t source);
//bool        savcmp();
void        savdo();
//void        savdst(std::vector<DSTREC>& DSTRecords, unsigned data);
void        save();
//void        savmap();
//void        sdCor2px(const fPOINTATTR stitchPoint, POINT& pixelCoordinate);
//void        segentr(double rotationAngle);
//void        selCol();
void        selRct(fRECTANGLE& sourceRect);
//void        selalstch();
//void        seldwn();
//void        selfpnt();
//void        selfrm0();
//void        selfrmx();
//void        selin(unsigned start, unsigned end, HDC dc);
//void        selup();
//void        set1knot();
//bool        setRmap(boost::dynamic_bitset<>& stitchMap, const fPOINTATTR& stitchPoint);
//void        setbak(unsigned penWidth);
//void        setdst();
//void        setgrd(COLORREF color);
//void        setknots();
//void        setknt();
//void        setmov();
//void        setpclp();
//void        setpcs();
void        setpsel();
//void        setsped();
//void        setsrch(unsigned stitch);
void        shft(const fPOINT& delta);
//void        shft2box();
//void        shftflt(fPOINT point);
//void        shoknot();
//void        shorter();
//bool        sidclp();
//void        sidhup();
//void        sidmsg(HWND window, std::wstring* const strings, unsigned entries);
//size_t      sizclp(unsigned formFirstStitchIndex);
//size_t      sizfclp();
//void        sizstch(fRECTANGLE& rectangle, const fPOINTATTR* const stitches);
//void        srchk();
//unsigned    srchknot(unsigned source);
//inline void stCor2px(const fPOINTATTR& stitch, POINT& point);
//unsigned    stch2px(unsigned iStitch);
//void        stch2px1(unsigned iStitch);
void        stch2pxr(const fPOINT& stitchCoordinate);
//void        stchPars();
//void        stchWnd();
//void        stchbox(unsigned iStitch, HDC dc);
//void        stchout();
void        stchrct(fRECTANGLE& rectangle);
//void        stchsnap(unsigned start, unsigned finish);
//double      stlen(unsigned iStitch);
//int         strcomp(const void* arg1, const void* arg2);
void        strtchbox(std::vector<POINT>& stretchBoxLine);
//void        strtknt(unsigned start);
//void        tglhid();
//void        thr2bal(std::vector<BALSTCH>& balaradStitch, unsigned destination, unsigned source, unsigned code);
//void        thrsav();
//void        thumbak();
//void        thumnail();
//void        toglHid();
//void        toglup();

int    txtWid(const wchar_t* string);
void   unbBox();
//void   unbox();
//void   unboxs();
void   unbsho();
//void   unclp();
//void   uncros();
void   undat();
//void   ungrphi();
//void   ungrplo();
//void   unlin();
//void   unmov();
void   unmsg();
//void   unpat();
//void   unpclp();
//void   unrot();
//void   unrotu();
//void   unsel();
void   unsid();
//void   unstrtch(std::vector<POINT>& stretchBoxLine);
//double unthrsh(unsigned level);
//void   unthum();
//bool   usedcol();
//void   vubak();
//void   vuselthr();
//void   vuthrds();

//LRESULT CALLBACK WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow);
//bool         wastch();
void         wrnmen();
//void         xlin();
//void         xlin1();
//void         zRctAdj();
void         zumhom();
//void         zumin();
//void         zumout();
//void         zumshft();

#if PESACT
//double   dubl(unsigned char* pnt) noexcept;
//unsigned dupcol() noexcept;
//void     pecdat(unsigned char* buffer);
//unsigned pesmtch(COLORREF referenceColor, unsigned char colorIndex) noexcept;
//unsigned pesnam() noexcept;
//void     ritpcol(unsigned char colorIndex) noexcept;
//void     ritpes(unsigned iStitch, const std::vector<fPOINTATTR>& stitches);
//void     rpcrd(float stitchDelta) noexcept;
//void     setpes();
//unsigned tripl(char* dat);
#endif

#ifdef ALLOCFAILURE
//int handle_program_memory_depletion(size_t);
#endif
