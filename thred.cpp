#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <tchar.h>
#include "lang.h"
#include "resource.h"
#include "thred.h"

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
// Suppress C6031: return value ignored
#pragma warning(disable:6031)

void clrhbut(unsigned strt);
void delsfrms(unsigned cod);
void fthrfn();
void dufdef();
void delet();
void frmx(POINT cpnt, HDC dc);
void setpsel();
void unlin();
void redraw(HWND dWnd);
void duIns();
void cros(unsigned ind);
void shft(fPOINT shPnt);
void movStch();
void nuAct(unsigned ind);
void movbox();
void xlin();
unsigned pt2colInd(unsigned pInd);
void xlin1();
void delstchm();
void drwLin(unsigned ind, unsigned len, HPEN hPen);
void zRctAdj();
void lenCalc();
void ritnum(unsigned cod, unsigned num);
void ritot(unsigned num);
void endpnt();
void rstAll();
void shoMsg(TCHAR* str);
void delsmal(unsigned ind, unsigned fin);
void patdun();
void selRct(fRECTANGLE* srct);
void thrsav();
void okcan();
void rstdu();
void unmsg();
void frmdel();
void unsid();
void unbsho();
void rngadj();
void rotfn();
void dusel(HDC dc);
void rSelbox();
void unsel();
void rotpix(POINT rpnt, POINT* tpnt);
BOOL cmpstch(unsigned ind0, unsigned ind1);
void duselrng();
void unthum();
void insfil();
void chkhup();
void ilin();
void clRmap(unsigned len);
void rotfns();
void nuslst(unsigned find);
void dstcurs();
void mvstch(unsigned dst, unsigned src);
void save();
void mvstchs(unsigned dst, unsigned src, unsigned cnt);
void fndknt();
COLORREF fswap(COLORREF tcol);
void dubuf();
unsigned bufref();
void hupfn();
unsigned setRmp(unsigned pbit);
void untrace();
void trcols(COLORREF col);
void dutrnum0(unsigned num);
void dutrnum1();
void trinit();
void trdif();
void blak();
void trcsel();
void fnamtabs();

extern void txsnap();
extern void dazdef();
extern void txof();
extern void rstxt();
extern void frmchkx();
extern unsigned clofinx;
extern void prbug();
extern void repar();
extern void setfchk();
extern void chgchk(int cod);
extern void lodchk();
extern void chgwrn();
extern void savtxt();
extern void redtx();
extern void txdun();
extern void deltx();
extern void clrstch();
extern void setclpspac();
extern void setshft();
extern void mvshft();
extern void dushft();
extern void dufind(float idnt);
extern void setfind();
extern void nudsiz();
extern void rtrclp();
extern void redtx();
extern TXPNT* adtx(int cnt);
extern BOOL istx(unsigned find);
extern void dutxtfil();
extern TXPNT txpnts[MAXSEQ];
extern int txad;
extern TCHAR hlpbuf[HBUFSIZ];
extern TXTSCR tscr;
extern void txtkey(unsigned cod);
extern int ptxhst;
extern TXHST thsts[16];
extern void txtlin();
extern void txtclp();
extern void rstxt();
extern int cloxcnt;
extern void txtrup();
extern void txtrmov();
extern void txtrbut();
extern void txtlbut();
extern void duauxnam();
extern void hsizmsg();
extern void drwtxtr();
extern void setfilstrt();
extern void setfilend();
extern void setfhi();
extern void dufhi(float len);
extern void setfwid();
extern void dufwid(float len);
extern void setfmin();
extern void setfmax();
extern void setbmin();
extern void setbmax();
extern void dufmin(float len);
extern void dufmax(float len);
extern void dubmin(float len);
extern void dubmax(float len);
extern void dubspac(float len);
extern void setbspac();
extern void setbcol();
extern void setfcol();
extern void setblen();
extern void dublen(float len);
extern void dufcol(unsigned col);
extern void dubcol(unsigned col);
extern void dundcol(unsigned col);
extern void setucol();
extern void setfang();
extern void dufspac(float spac);
extern void setfspac();
extern void setflen();
extern void duflen(float len);
extern void dufang(float ang);
extern void dufxang(float ang);
extern void sfuang();
extern void duspac(float spac);
extern void uspac();
extern void dusulen(float len);
extern void undlen();
extern void setund();
extern void setwlk();
extern void setcwlk();
extern void notund();
extern void notwlk();
extern void notcwlk();
#ifdef _DEBUG
extern void dmpat();
#endif
extern fPOINT iseq[MAXSEQ];
extern BOOL chkrypt(unsigned arg0, unsigned arg1, unsigned arg2, unsigned arg3);
extern void selalfrm();
extern void setuang();
extern void setuspac();
extern void setulen();
extern void setwlkind();
extern void dubit(unsigned bit);
extern void pes2crd();
extern void fritfil();
extern void tst();
extern void fethrf();
extern void nufthcol(unsigned col);
extern void fethr();
extern void fltspac(fPOINT* strt, unsigned cnt);
extern unsigned fltind(fPOINT* pnt);
extern unsigned pdir(unsigned ind);
extern void unpsel();
extern void	sRct2px(fRECTANGLE strct, RECT* pxrct);
extern void dupsel(HDC dc);
extern void fsizpar();
extern void bsizpar();
extern void form();
extern void duform(unsigned ind);
extern void drwfrm();
extern void dufrm();
extern void mdufrm();
extern void setfrm();
extern void unfrm();
extern void munfrm();
extern unsigned closfrm();
extern void frmovlin();
extern void setfpnt();
extern void ritfrct(unsigned ind, HDC dc);
extern void delfrms();
extern void filvrt();
extern void filhor();
extern void frmcpy(FRMHED* dst, FRMHED* src);
extern void filangl();
extern void satpnt0();
extern unsigned chkfrm();
extern void setmfrm();
extern void rstfrm();
extern void clrfills();
extern void drwsat();
extern void satpnt1();
extern void satfix();
extern void filsat();
extern void satsel();
extern void drwcon();
extern void satknkt();
extern void insat();
extern void delspnt();
extern void unfil();
extern void frmlin(fPOINT* scr, unsigned sidz);
extern void rats();
extern void satzum();
extern void frm0();
extern void infrm();
extern void duinsf();
extern void uninsf();
extern void setins();
extern void refil();
extern void frmout(unsigned ind);
extern void delcon(unsigned cpnt);
extern void satadj();
extern void fvars(unsigned ind);
extern void	bord();
extern void fclp();
extern void ispcdclp();
extern void	satbrd();
extern void	apliq();
extern void setap();
extern void refrm();
extern void strtchbox();
extern void unstrtch();
extern void setstrtch();
extern void setexpand();
extern void nufilcol(unsigned col);
extern void maxtsiz(TCHAR* str, POINT* pt);
extern void nubrdcol(unsigned col);
extern void nulapcol(unsigned col);
extern void sidwnd(HWND wnd);
extern void delmfil(unsigned col);
extern unsigned find1st();
extern void dubold();
extern void px2stchf(POINT pxpnt, fPOINT* stpnt);
extern void prfmsg();
extern void prfsid(HWND wnd);
extern void durpoli(unsigned sidz);
extern void fliph();
extern void flipv();
extern void savdisc();
extern void tomsg();
extern void	getfinfo(unsigned ind);
extern void prpbrd(double spac);
extern void tglfrm();
extern void frmon();
extern void frmclr(FRMHED* dst);
extern void flpord();
extern void rinfrm();
extern void clpfil();
extern void snap();
extern void fsort();
extern void dustar(unsigned nsids, double len);
extern void rotcmd();
extern void rotagain();
extern void rotdup();
extern void duprot();
extern void cpylayr(unsigned play);
extern void movlayr(unsigned play);
extern void duspir(unsigned nsids);
extern void sfCor2px(fPOINT stpnt, POINT* pxpnt);
extern void frmsqr(unsigned ind);
extern float midl(float hi, float lo);
extern void oclp(fPOINT* clp, unsigned nclp);
extern void join();
extern void refilal();
extern void nufsel();
extern void frmadj(unsigned find);
extern void ratsr();
extern void frmsadj();
extern void pxrct2stch(RECT pxr, fRECTANGLE* str);
extern void fselrct(unsigned fpnt);
extern unsigned short nxt(unsigned short ind);
extern unsigned short prv(unsigned ind);
extern void setr(unsigned pbit);
extern BOOL chkr(unsigned pbit);
extern void selal();
extern void selfil(unsigned typ);
extern void	selalfil();
extern BOOL notfstch(unsigned at);
extern BOOL frmrng(unsigned fpnt, RANGE* rng);
extern void refilfn();
extern void bhol();
extern void deleclp(unsigned fpnt);
extern void delmclp(unsigned fpnt);
extern void delsac(unsigned fpnt);
extern void delflt(unsigned fpnt);
extern void bholbrd();
extern void fcntr();
extern void savblen(float len);
extern void boxsel();
extern void picot();
extern void clpic(unsigned short strtlin);
extern unsigned getlast();
extern void ritbrd();
extern void savplen(float len);
extern void contfil();
extern void chkcont();
extern void grpmsg();
extern void grpmsg1();
extern void help();
extern void ribon();
extern void dubig();
extern unsigned psg();
extern void redup();
extern void bakdup();
extern void bakagain();
extern void shrnk();
extern void setrang();
extern void frmnum();
extern void frmnumfn(unsigned nunum);
extern void srtbyfrm();
extern void cntrx();
extern void centir();
extern BOOL notsel();
extern void dubean();
extern void debean();
extern void spltfrm();
extern void stchs2frm();
extern void stCor2px(fPOINTATTRIBUTE stpnt, POINT* pxpnt);
extern void	vrtclp();
extern void horclp();
extern void angclp();
extern void dubfil();
extern void horclpfn();
extern void horsclp();
extern void vrtsclp();
extern void angsclp();
extern void dubsfil();
extern void makspac(unsigned strt, unsigned cnt);
extern void	duhart(unsigned nsids);
extern void dulens(unsigned nsids);
extern void sethup();
extern void sizstch(fRECTANGLE* rct);
extern void col2frm();
extern unsigned projh(double pnty, fPOINT pnt0, fPOINT pnt1, dPOINT* ipnt);
extern void stchrct2px(fRECTANGLE srct, RECT* prct);
extern void chan();
extern void crop();
extern void dueg(unsigned nsids);
extern void selsqr(POINT cpnt, HDC dc);
extern void msgstr(unsigned cod);
extern void shoseln(unsigned cod0, unsigned cod1);
extern void	tabmsg(unsigned cod);
extern void lodstr();
extern TCHAR* stab[STR_LEN];
extern void butxt(unsigned ind, TCHAR* str);
extern void riter();
extern void crmsg(TCHAR* nam);
extern void filnopn(unsigned cod, TCHAR* nam);
extern void bfilmsg();
extern void tsizmsg(TCHAR* sizstr, double siz);
extern void setear();
extern void maxwid(unsigned strt, unsigned fin);
extern void	chain();
extern void duzig(unsigned nsids);
extern void dusat();
extern void filclpx();
extern void duxclp();
extern void fsclpx();
extern void wavfrm();
extern void srtfrm();
extern void rct2sel(RECT rct, POINT* lin);

#if PESACT
extern BOOL setrc(unsigned pbit);
#endif

extern POINT			selectedPointsRectangle[9];
extern fPOINT			lastpnt;
extern unsigned			xpnt;
extern unsigned			formIndex;
extern unsigned			frmstrt;
extern unsigned			frmend;
extern unsigned			delpnt;
extern FRMHED			formList[MAXFORMS];
extern POINT			flin[MAXFRMLINS];
extern unsigned			closestVertexToCursor;
extern unsigned			closestFormToCursor;
extern unsigned			newFormVertexCount;
extern unsigned short	wpar;
extern double			StitchSpace;
extern double			fillAngle;
extern double			borderWidth;
extern unsigned			selectedFormControlVertex;
extern double			xyRatio;
extern unsigned short	sides;
extern fPOINT			lowerLeftStitch;
extern HWND				thDat[LASTLIN];
extern fPOINT*			currentFormVertices;
extern SATCON*			currentFormConnections;
extern unsigned			satpt;
extern POINT			mvlin[3];
extern double			snapLength;
extern double			starRatio;
extern double			spiralWrap;
extern POINT			formOutlineRectangle[10];
extern fPOINT			fmovdif;
extern unsigned			selectedFormCount;
extern unsigned			previousFormIndex;
extern POINT			selectedFormsLine[9];
extern RECT				selectedFormsRectangle;
extern fPOINT			fmovdif;
extern unsigned short	selectedFormList[MAXFORMS];
extern double			horizontalRatio;
extern double			verticalRatio;
extern BSEQPNT			bseq[BSEQLEN];
extern fPOINT			formPoints[MAXFRMPNTS];		//form points
extern unsigned			fltad;					//next index to append form points
extern fPOINT			clipboardPoints[MAXCLPNTS];		//main clipboard fill points for forms
extern unsigned			clpad;					//next index to append main clipboard points
extern SATCON			satks[MAXSAC];			//satin form connects
extern unsigned			satkad;					//next index to append satin connect points
extern float			buttonholeFillCornerLength;
extern HWND				thTxt[LASTLIN];
extern float			picotSpace;
extern fPOINT			oseq[OSEQLEN];
extern HWND				hindx;
extern HWND				hnxt;
extern HWND				hsrch;
extern HWND				hlptxt;
extern TCHAR*			cpyrit;
extern unsigned			psgacc;
extern TCHAR*			laytxt[];
extern FRMHED*			finspnt;
extern unsigned			fgpnt0;
extern unsigned			opnt;
extern TCHAR*			shrtmsg;
extern fRECTANGLE		allItemsRectangle;
extern long				prfwid;
extern HWND				hMsg;

unsigned char			cryptkey[4096];

//select box
#define				NERCNT		4		//number of entries in the near array

unsigned short chk1loc[] =
{
	3632,
	3168,
	393,
	2246,
	802,
	1817,
	3175,
	2254
};

//main variables
HINSTANCE			hInst;					//main instance handle
HWND				hWnd;					//main window handle
MSG					msg;					//main message loop message
RECT				mRct;					//main window size
RECT				colorBarRect;			//color bar rectangle
//RECT				bcRct;					//color bar client rectangle
RECT				minLenRct;				//minimum length rectangle
RECT				maxLenRct;				//maximum length rectangle
unsigned			smallestStitchIndex;	//pointer to the smallest stitch in the selected range
unsigned			largestStitchIndex;		//pointer to the largest stitch in the selected range
unsigned			currentStitchIndex;		//pointer to the current selection for length search
HDC					mainDC;					//main device context handle
HDC					StitchWindowMemDC;		//stitch window memory device context
HDC					StitchWindowDC;			//stitch window device context
HDC					bitmapDC;				//bitmap device context
HDC					colorBarDC;				//color bar device context
HBITMAP				stitchWindowBmp;		//bitmap for the memory stitch device context
POINT				mainWindowOrigin;		//offset origin of the main window
POINT				stitchWindowOrigin;		//offset origin of the stitch window
SIZE				textSize;				//used for measuring sizes of text items
SIZE				screenSizePixels;		//screen size in pixels
SIZE				screenSize_mm;			//screen size in millimeters
dRECTANGLE			zoomRect;				//zoom rectangle
RECT				stitchWindowAbsRect;	//stitch window size,absolute
RECT				stitchWindowClientRect;	//stitch window size,client
DRAWITEMSTRUCT*		ds;						//for owner-draw windows
double				zoomFactor = 1;			//zoom factor
POINT				unzoomedRect;			//size of the unzoomed stitch window
POINT				stitchSizePixels;		//converted from stich to pixel
POINT				pxNer[NERCNT];			//selected points
POINT				boxPix;					//single select box point
double				distanceToClick;		//distance of closest point to a mouse click
unsigned			closestPointIndex;		//index of closest point
unsigned			closestPointIndexClone;	//copy of index of closest point
unsigned			groupStitchIndex;		//last point selected in group
unsigned			groupStartStitch;		//lower end of selected stitches
unsigned			groupEndStitch;			//higher end of selected stitches
unsigned			prevGroupStartStitch;	//lower end of previous selection
unsigned			prevGroupEndStitch;		//higher end of previous selection
TCHAR				stitchEntryBuffer[5];	//stitch number entered by user
TCHAR				sideWindowEntryBuffer[11];	//side window number for entering form data sheet numbers
unsigned			bufferIndex = 0;		//pointer to the next number to be entered
unsigned			bufferDigitCount;		//number of decimal digits in the number of stitches
POINT*				linePoints;				//for drawing lines on the screen
unsigned			lineIndex;				//line index for display routine
double				stitchWindowAspectRatio;	//aspect ratio of the stitch window
double				minStitchLength	  = MINSIZ*PFAFGRAN;	//minimum stitch size
double				userStitchLength  = USESIZ*PFAFGRAN;	//user selected stitch size
double				smallStitchLength = SMALSIZ*PFAFGRAN;	//user can remove stitches smaller than this
unsigned			markedStitchMap[RMAPSIZ];	//bitmap to tell when stitches have been marked
TCHAR*				pcdClipFormat = "PMust_Format";
TCHAR*				thrEdClipFormat = "threditor";
CLPSTCH*			clipboardStitchData;	//for pcs clipboard data
FORMCLIP*			clipboardFormData;		//for thred form clipboard data
FORMSCLIP*			clipboardFormsData;		//multiple form clipboard header
FORMPOINTCLIP*		clipboardFormPointsData;	//form points clipboard header
void*				ptrClipVoid;			//for memory allocation for clipboard data
void*				ptrThrEdClipVoid;		//for memory allocation for thred format clipboard data
FLSIZ				clipboardRectSize;		//clipboard rectangle size
fRECTANGLE			clipboardRect;			//clipboard rectangle
unsigned			clipboardStitchCount;	//number of stitchs extracted from clipboard
POINT				clipboardOrigin;		//origin of clipboard box in stitch coordinates
HGLOBAL				hClipMem;				//handle to the clipboard memory
SIZE				selectBoxSize;			//size of the select box
POINT				selectBoxOffset;		//offset of the spot the user selected from the lower left of the select box
dPOINT				zoomRatio;				//zoom ratio used to draw stitch window
double				rotationAngle;			//rotation angle
double				rotationHandleAngle;	//angle of the rotation handle
fRECTANGLE			rotationRect;			//rotation rectangle
dPOINT				bmpStitchRatio;			//bitmap to stitch hoop ratios
dPOINT				stitchBmpRatio;			//stitch hoop to bitmap ratios
RECT				bitmapSrcRect;			//bitmap source rectangle for zoomed view
RECT				bitmapDstRect;			//stitch window destination rectangle for zooomed view
unsigned			bitmapWidth;			//bitmap width
unsigned			bitmapHeight;			//bitmap height
dPOINT				bitmapSizeinStitches;	//bitmap end points in stitch points
unsigned*			ptrBitmap;				//monochrome bitmap data
unsigned			bitmapColor = BITCOL;	//bitmap color
TCHAR				msgbuf[MSGSIZ];			//for user messages
unsigned			msgIndex;				//pointer to the message buffer
double				showStitchThreshold = SHOPNTS;//show stitch grid below this zoom level
double				threadSize30 = TSIZ30;	//#30 thread size
double				threadSize40 = TSIZ40;	//#40 thread size
double				threadSize60 = TSIZ60;	//#40 thread size
unsigned			runPoint;				//point for animating stitchout
unsigned			stitchesPerFrame;		//number of stitches to draw in each frame
int					delay;					//time delay for stitchout
double				StitchBoxesThreshold = STCHBOX;//threshold for drawing stitch boxes
//WARNING the size of the following array must be changed if the maximum movie speed is changed
POINT				movieLine[100];			//line for movie stitch draw
RECT				msgRct;					//rectangle containing the text message
void*				undoBuffer[16];			//backup data
unsigned			undoBufferWriteIndex = 0;	//undo storage pointer
unsigned			undoBufferReadIndex = 0;	//undo retrieval pointers
unsigned			underlayColor = 15;		//underlay color
unsigned			lastKeyCode = 0xffff;	//last key code
unsigned			formMenuChoice = 0;		//data type for form data form numerical entry
dPOINT				zoomMarkPoint;			//stitch coordinates of the zoom mark
unsigned			preferenceIndex = 0;	//index to the active preference window
unsigned			LRUMenuId[] = { FM_ONAM0,FM_ONAM1,FM_ONAM2,FM_ONAM3 };	//recently used file menu ID's
TCHAR				versionNames[OLDVER][_MAX_PATH];	//temporary storage for old file version names
unsigned			fileVersionIndex;		//points to old version to be read
unsigned			activeLayer = 0;		//active layer
unsigned			layerIndex;				//active layer code
long				clipboardFormsCount;	//number of forms the on the clipboard
unsigned*			finds;					//array of form indices for delete multiple forms
POINT				stitchArrow[3];			//arrow for selected stitch
RANGE				selectedRange;			//first and last stitch for min/max stitch select
unsigned			fileNameOrder[50];		//file name order table
unsigned char		fnamcod[128];			//file name encoding
unsigned char		frencod[256];			//file name decode
TCHAR				fildes[50];				//file designer name in clear
HWND				fwnd;					//first window not destroyed for exiting enumerate loop
RANGE				selectedFormsRange;		//range of selected forms
unsigned			tmpFormIndex;			//saved form index
unsigned char		cursorMask[128];		//cursor and mask
double				zoomMin;				//minimum allowed zoom value
fRECTANGLE			checkHoopRect;			//for checking the hoop size
BALSTCH*			ptrBalaradStitich;		//balarad stitch pointer
fPOINT				balaradOffset;			//balarad offset
unsigned			clipTypeMap = MCLPF | MVCLPF | MHCLPF | MANGCLPF; //for checking if a fill is a clipboard fill
fPOINTATTRIBUTE*	rotatedStitches;		//rotated stitches for rotate and save
unsigned			sideWindowLocation;		//side message window location
POINT				sideWindowSize;			//size of the side message window
TCHAR**				ptrSideWindowsStrings;	//string array displayed in sidmsg
TCHAR				colorFileName[_MAX_PATH];	//.thw file name
TCHAR				rgbFileName[_MAX_PATH];	//.rgb file name
dPOINT				rcel;					//size of an markedStitchMap cell for drawing stitch boxes
unsigned			draggedColor;			//color being dragged
FORMPOINTS			selectedFormPoints;		//selected form points
fRECTANGLE			selectedPointsRect;		//rectangle enclosing selected form points
RECT				selectedPixelsRect;		//display form point select rectangle
POINT*				fplin;					//form point clipboard paste into form line

#if	PESACT
unsigned char*		pesColors;				//pes colors
TCHAR*				ptrPesData;				//pes card data buffer
fPOINT				stitchCenterOffset;		//offset for writing pes files
PESTCH*				pesStitches;			//pes stitch buffer
unsigned char		pesEquivColors[16];		//pes equivalent colors
unsigned char*		pestch;					//pes stitches
unsigned			pesColorIndex;			//pes color index
#endif

POINT				pselfin;		//point to draw at the end cross for form select points
unsigned			fsizeof;		//size of form header divided by 4
HDC					tracedc;		//trace device context
HBITMAP				hTraceBitmap;	//trace bitmap
unsigned			sref;			//brightness of reference pixel
unsigned*			traceBitmap;	//trace bitmap data
POINT				currentTracePoint;	//current point being traced
unsigned			traceDataSize;	//size of the trace bitmap in double words
unsigned*			tracedPixels;	//bitmap of selected trace pixels
unsigned*			tracedEdges;	//detected edges of trace areas
TRCPNT*				tracedPoints;	//collection of traced points
TRCPNT*				decimatedLine;	//differenced collection of traced points
unsigned			traceDirection;	//trace direction
unsigned			initialDirection;	//initial trace direction
TRCPNT				traceDiff0;		//difference from one trace pixel to the next
TRCPNT				traceDiff1;		//difference from one trace pixel to the next
COLORREF			upColor;		//color of the up reference pixel
COLORREF			downColor;		//color of the down reference pixel
COLORREF			invertUpColor;	//complement color of the up reference pixel
COLORREF			invertDownColor;	//complement color of the down reference pixel
POINT				traceMsgPoint;	//message point for trace parsing
RECT				highRect;		//high trace mask rectangle
RECT				middleRect;		//middle trace mask rectangle
RECT				lowRect;		//low trace mask rectangle
unsigned			highColors[3];	//separated upper reference colors
unsigned			pixelColors[3];	//separated pixel reference colors
unsigned			lowColors[3];	//separated lower reference colors
unsigned			traceRGBFlag[]	= { TRCRED,TRCGRN,TRCBLU };	//trace bits
unsigned			traceRGBMask[]	= { REDMSK,GRNMSK,BLUMSK };	//trace masks
unsigned			traceRGB[]		= { BLUCOL,GRNCOL,REDCOL };	//trace colors
unsigned			traceAdjacentColors[9];	//separated colors for adjacent pixels
unsigned*			differenceBitmap;	//difference bitmap
TCHAR				trbuf[4];		//for composing color numbers
TCHAR				trinbuf[4];		//for user input color numbers
unsigned			colm;			//trace color column
POINT				bitpnt;			//a point on the bitmap

//cursors
HCURSOR 			hArrowCurs;			//arrow
HCURSOR 			hCrossCurs;			//cross
HCURSOR 			hFormCurs;			//form
HCURSOR 			hDlinCurs;			//dline
HCURSOR				hNeedleUpCurs;			//upright needle
HCURSOR				hNeedleLeftUpCurs;		//left up needle 
HCURSOR				hNeedleLeftDownCurs;	//left down needle 
HCURSOR				hNeedleRightUpCurs;		//right up needle 
HCURSOR				hNeedleRightDownCurs;	//right down needle 
CURSORMASK			cursorMasks;			//cursor mask structure

HPEN				linePen;		//line pen for stitch move lines			
HPEN				boxPen[4];		//box pens
HPEN				userPen[16];	//user color pens
HPEN				crossPen;		//pen for crosses in color windows
HPEN				groupSelectPen;	//pen for group select
HPEN				gridPen;		//pen for stitch grid
HPEN				backgroundPen;	//background color pen
HPEN				bitmapPen;		//bitmap pen
HPEN				formPen;		//form pen
HPEN				layerPen[6];	//layer pens
HPEN				formPen3px;		//three-pixel form pen
HPEN				formSelectedPen;	//form select pen
HPEN				zoomMarkPen;	//zoom mark pen
HPEN				multiFormPen;	//multiple selected forms pen
HPEN				selectAllPen;	//pen for drawing large boxes
HPEN				knotPen;		//knot pen
HPEN				blackPen;		//black pen

unsigned			backgroundPenWidth;	//width of the background pen

//brushes
HBRUSH				hBackgroundBrush;		//background color brush
HBRUSH				hDefaultColorBrush[16];	//default color brushes
HBRUSH				hUserColorBrush[16];	//user color brushes

//clipboard handle
unsigned			hClip = 0;		//pcs format
unsigned			htclp = 0;		//thred format

//for the choose color dialog box
CHOOSECOLOR			gCol;
COLORREF			custCol[16];
//for the bakground color dialog box
CHOOSECOLOR			bCol;
COLORREF			bakCust[16];
//for the bitmap color dialog box
CHOOSECOLOR			btCol;
COLORREF			bitmapBackgroundColors[16];

TCHAR				thrdSiz[16][2];	//user selected thread sizes
unsigned			pixSiz[16];		//thread sizes in pixels
unsigned			sizInd[16];		//thread size indices

HMENU				hMen;			//main menu
HMENU				hfilMen;		//fill submenu
HMENU				hfileMen;		//file submenu
HMENU				hfilBrdMen;		//border fill submenu
HMENU				hvumen;			//view submenu
HMENU				hvsmen;			//view/set
HMENU				hedmen;			//edit submenu
HWND				hDef[16];		//default color windows
HWND				hCol[16];		//user color windows
HWND				hSiz[16];		//thread size windows
HWND				hSizChng[3];	//thread size change windows
HWND				hStch;			//stitch window
HWND				hVrt;			//vertical scroll bar
HWND				hHor;			//horizontal scroll bar
HWND				hbuts[9];		//button windows
HWND				hstep;			//trace step window
HWND				hBar;			//color bar
HWND				hok;			//ok button
HWND				hdsc;			//discard button
HWND				hcan;			//cancel button
HWND				hmin;			//minimum stitch text box
HWND				hgNum;			//general number input box
HWND				hSped;			//speed scroll bar for movie
HWND				hSid = 0;		//main side message window
HWND				hsidWnd[16];	//side message windows
HWND				hPrf;			//preferences window
HWND				hfdat = 0;		//form data sheet
HWND				hto;			//asking user if she wants delete the sitches associated with a form
HWND				hvu[OLDVER];	//handles of mulitple file viewing windows
HWND				htrup[3];		//trace up number windows
HWND				htrdwn[3];		//trace down number windows

HWND				hctrc[3];		//trace control windows
HWND				hstrc[3];		//trace select windows
unsigned			trshft[] = { 0,8,16 };//trace shift values
HBRUSH				trbrsh[3];		//red,green,and blue brushes
HWND				htred;			//red trace window
HWND				htgrn;			//green trace window
HWND				htblu;			//blue trace window
HWND				hcred;			//red control window
HWND				hcgrn;			//green control window
HWND				hcblu;			//blue control window
HWND				htrnum;			//trace number input window
HBRUSH				hbred;			//red brush
HBRUSH				hbgrn;			//green brush
HBRUSH				hblu;			//blue brush
HBRUSH				hblk;			//black brush
HBRUSH				tbrsh;			//temporary brush

COLORREF			useCol[16];		//user colors
COLORREF			backgroundColor;	//stich window background
COLORREF			boxRef[] = { 0x404040,0x408040,0x804040,0x404080 };
unsigned			actcol = 0;		//active color selector
unsigned			buttonHi;		//button height
unsigned			buttonWid;		//button width
unsigned			buttonWid3;		//button width times 3
unsigned			numwid;			//width of 0
int					tPix[3];		//thread sizes in pixels
unsigned			cbit;			//color bitmap for recording colors on screen
double				ner[NERCNT];	//distances of the closest points
									//to a mouse click
long				nerInd[NERCNT];	//indices of the closest points
unsigned			selCnt;			//number of boxes selected
unsigned			slpnt = 0;		//pointer for drawing stitch select lines
POINT				slin[MAXSEQ];	//stitch select line
fRECTANGLE			rngrct;			//stitch range rectangle
fPOINT				rngsiz;			//from check ranges
fPOINT				bigsiz;			//size of multiple select rectangle
unsigned			movpnt;			//for resequencing stitches
double				pixang;			//angle for pixel rotate
SIZE				pcolsiz;		//size of the pick color message
POINT				inspnt;			//size of file insert window
fPOINT				inscntr;		//center point in inserted file
unsigned			numcod;			//keyboard numerical input
double				loang;			//low angle for angle from mark
double				organg;			//original angle for angle from mark
double				hiang;			//hi angle for angle from mark
unsigned			knotab[MAXKNOTS];//pointers to knots
unsigned			knotcnt;		//number of knots in the design
unsigned			knotat;			//knot stitch attribute
fPOINT				knotstp;		//knot step
TCHAR				kstrtdat[] = { 2,3,1,4,0 };	//knot spacings
TCHAR				kendat[] = { -2,-3,-1,-4,0 };	//reverse knot spacings
TCHAR				klstdat[] = { 0,-4,-1,-3,-2 };	//reverse knot spacings
fRECTANGLE			clprcta;		//rectangle for adjsut range ends for clipboard fills
TCHAR*				balpnt;			//used in the balarad interface
HANDLE				balfil;			//balarad file handle

//graphics variables		
double				aspct = (double)LHUPX / LHUPY;	//aspect ratio of the stich window
SCROLLINFO			scrol;			//scrool bar i/o structure
POINT				stchSiz;		//size of the stich window in pixels
fPOINT				sPnt;			//for converting pixels coordinates
									// to stitch cordinates
POINT				cPnt;			//closest point to a mouse click
fPOINT				zBoxo;			//zoom box origin

COLORREF defCol[] = {

	0x000000,
	0x800000,
	0xFF0000,
	0x808000,
	0xFFFF00,
	0x800080,
	0xFF00FF,
	0x000080,
	0x0000FF,
	0x008000,
	0x00FF00,
	0x008080,
	0x00FFFF,
	0x808080,
	0xC0C0C0,
	0xFFFFFF
};

long				selbox;
long				boxOff[4];

unsigned			defMap = 0xaf;	//bitmap for color number colors
unsigned			vind;			//vertical index, calculated from mouse click
unsigned			thrdSel;		//thread selected for size change

//file open stuff

MENUITEMINFO filinfo = {

	sizeof(MENUITEMINFO),
	MIIM_TYPE,
	MFT_STRING,
	0,
	0,
	0,
	0,
	0,
	0,
	msgbuf,
	13,
};

const TCHAR			fltr[_MAX_PATH + 1] = "Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Tajima (DST)\0*.dst\0";
const TCHAR			bfltr[_MAX_PATH + 1] = "Microsoft (BMP)\0*.bmp\0";
TCHAR				cstFltr[_MAX_PATH + 1] = "Thredworks (THR)\0*.thr\0";
TCHAR				filnam[_MAX_PATH + 1] = { 0 };
TCHAR				thrnam[_MAX_PATH + 1];
TCHAR				auxnam[_MAX_PATH + 1];
TCHAR				genam[_MAX_PATH + 1];
TCHAR				defDir[_MAX_PATH + 1] = "c:\\";
TCHAR				defbmp[_MAX_PATH + 1] = "c:\\";
TCHAR				balnam0[_MAX_PATH + 1] = { 0 };	//balarad semaphore file
TCHAR				balnam1[_MAX_PATH + 1] = { 0 };	//balarad data file
TCHAR				balnam2[_MAX_PATH + 1];
TCHAR				srchnam[_MAX_PATH + 1];
TCHAR				homdir[_MAX_PATH + 1];	//directory from which thred was executed
TCHAR*				phom;			//pointer to the home file name
PCSTCH*				filBuf;
HANDLE				hFil = 0;
HANDLE				hPcs = 0;
HANDLE				hIni = 0;
HANDLE				hinsf;			//insert file handle
HANDLE				hBmp;			//bitmap handle
unsigned			siz;			//size of file
unsigned long		red;			//bytes actually read from file
unsigned			colCnt;			//number of color changes
TCHAR				iniNam[_MAX_PATH];	//.ini file name
unsigned			savcol;			//for saving files
TCHAR				bnam[16];		//bitmap file name from pcs file
TCHAR				lbnam[_MAX_PATH];//bitmap file name from user load
OPENFILENAME		ofn = {

	sizeof(OPENFILENAME),	//lStructsize
	hWnd,					//hwndOwner 
	hInst,					//hInstance 
	fltr,					//lpstrFilter 
	cstFltr,				//lpstrCustomFilter 
	_MAX_PATH,				//nMaxCustFilter 
	0,						//nFilterIndex 
	filnam,					//lpstrFile 
	_MAX_PATH,				//nMaxFile 
	0,						//lpstrFileTitle 
	0,						//nMaxFileTitle 
	defDir,					//lpstrInitialDir
	0,						//lpstrTitle
	OFN_OVERWRITEPROMPT,	//Flags
	0,						//nFileOffset
	0,						//nFileExtension 
	"thr",					//lpstrDefExt 
	0,						//lCustData 
	0,						//lpfnHook
	0,						//lpTemplateName
};
TCHAR*				thumnams;				//pointer to storage for thumbnail names
TCHAR**				pthums;					//pointers to the thumbnail names
TCHAR*				thumsel[4];				//pointers to thumbnails selectef for display
unsigned			thumcnt;				//number of thumbnail file names
unsigned			thumdcnt;				//number of thumbnail file names selected for display
unsigned			thumind;				//index into the thumbnail filname table
TCHAR				thumsrch[32];			//storage for the thumnail search string
TCHAR				insnam[_MAX_PATH] = { 0 };	//insert file name
unsigned			insflt;					//saved float pointer for inserting files
unsigned			insfpnt;				//saved form pointer for inserting files
unsigned			insfstch;				//saved stitch pointer for inserting files

OPENFILENAME obn = {
	sizeof(OPENFILENAME),	//lStructsize
	hWnd,					//hwndOwner 
	hInst,					//hInstance 
	bfltr,					//lpstrFilter 
	cstFltr,				//lpstrCustomFilter 
	_MAX_PATH,				//nMaxCustFilter 
	0,						//nFilterIndex 
	lbnam,					//lpstrFile 
	_MAX_PATH,				//nMaxFile 
	0,						//lpstrFileTitle 
	0,						//nMaxFileTitle 
	defbmp,					//lpstr	ialDir 
	0,						//lpstrTitle
	OFN_OVERWRITEPROMPT,	//Flags
	0,						//nFileOffset
	0,						//nFileExtension 
	"bmp",					//lpstrDefExt 
	0,						//lCustData 
	0,						//lpfnHook
	0,						//lpTemplateName
};

BITMAPFILEHEADER	bhed;	//bitmap file header
BITMAPV4HEADER		b4hed;	//bitmap version4 file header
BITMAPINFO			binf;	//bitmap info
BITMAPINFOHEADER	binfh;	//bitmap info header

COLORREF defUseCol[] = {

	0x00000000,
	0x002dffff,
	0x003f87e9,
	0x000762f8,
	0x000000ff,
	0x002f03af,
	0x007248b7,
	0x00ff0080,
	0x00920166,
	0x00a00000,
	0x00ff2424,
	0x006a4d15,
	0x00f5800a,
	0x004b7807,
	0x00156a1e,
	0x00dbe6e3,
};

COLORREF defCustCol[] = {

	0x729674,
	0x1a1eb9,
	0x427347,
	0xbfff,
	0xd3c25f,
	0xc3ced0,
	0x4a8459,
	0x8cacd0,
	0x81aeb6,
	0x7243a5,
	0xbdadda,
	0x9976c5,
	0x96d9f5,
	0xe2ddd6,
	0x245322,
	0x7b60ae
};

COLORREF defBakCust[] = {

	0xa3c5dc,
	0xadc7b6,
	0xd1fcfb,
	0xdcd7c0,
	0xebddcd,
	0xc6b3b3,
	0xdec9ce,
	0xd2d1e9,
	0xdfdffd,
	0xbee6ef,
	0x8fb8b1,
	0x85c2e0,
	0xabc1c9,
	0xd3d3c7,
	0x7c9c84,
	0x9acddc
};

COLORREF defBakBit[] = {

	0xc0d5bf,
	0xc8dfee,
	0x708189,
	0xa5a97a,
	0xb8d6fe,
	0x8a8371,
	0x4b6cb8,
	0x9cdcc2,
	0x366d39,
	0xdcfcfb,
	0x3c4f75,
	0x95b086,
	0xc9dcba,
	0x43377b,
	0xb799ae,
	0x54667a
};

unsigned namloc[] = {

	999,	//0
	1024,	//1
	2,		//2
	2051,	//3
	2050,	//4
	5,		//5
	7,		//6
	1,		//7
	17,		//8
	9,		//9
	2058,	//10
	11,		//11
	51,		//12
	6,		//13
	14,		//14
	2055,	//15
	8,		//16
	2056,	//17
	18,		//18
	2057,	//19
	10,		//20
	21,		//21
	22,		//22
	23,		//23
	99,		//24
	3075,	//25
	26,		//26
	2075,	//27
	28,		//28
	2062,	//29
	30,		//30
	63,		//31
	65,		//32
	132,	//33
	68,		//34
	2083,	//35
	36,		//36
	37,		//37
	311,	//38
	3081,	//39
	80,		//40
	41,		//41
	42,		//42
	43,		//43
	1035,	//44
	1547,	//45
	3800,	//46
	47,		//47
	96,		//48
	2054,	//49
};

POINT				mlin0[2];		//move point line
POINT				mlin1[2];		//move point line
POINT				inlin[3];		//the insert line
POINT				blin[5];		//the zoom box
POINT				clplin[5];		//for displaying clipboard insert rectangle
POINT				rlinb[5];		//for drawing the rotate rectangle
POINT				rlinv[2];		//vertical part of the rotate cross
POINT				rlinh[2];		//horizontal part of the rotate cross
POINT				rlinu[2];		//line from the cursor to the center of the rotate cross
POINT				sizlin[5];		//stretch and expand

COLCHNG				colch[MAXCHNG];
PCSHEADER					hed;			//pcs file header
STREX				hedx;			//thred file header extension
INIFILE				ini;			//initialization file
dPOINT				rotcntr;		//center of stitch rotation
POINT				pixcntr;		//center of pixel rotation

PCSTCH*				stind;			//stitch pointer
void sCor2px(dPOINT stpnt, POINT* pxpnt);

typedef struct _dstdat {

	TCHAR	cor;
	TCHAR	val;
}DSTDAT;

#define XCOR 0
#define YCOR 1

DSTDAT	dstval[] = {

	{XCOR,1},
	{XCOR,-1},
	{XCOR,9},
	{XCOR,-9},
	{YCOR,-9},
	{YCOR,9},
	{YCOR,-1},
	{YCOR,1},
	{XCOR,3},
	{XCOR,-3},
	{XCOR,27},
	{XCOR,-27},
	{YCOR,-27},
	{YCOR,27},
	{YCOR,-3},
	{YCOR,3},
	{XCOR,0},
	{XCOR,0},
	{XCOR,81},
	{XCOR,-81},
	{YCOR,-81},
	{YCOR,81},
};

unsigned char rencod[] = {		//name decryption table

	0x40,
	0x61,
	0x2,
	0x1,
	0x4,
	0x5,
	0x6,
	0x3c,
	0x8,
	0x12,
	0x15,
	0xb,
	0x18,
	0xd,
	0x1d,
	0x3,
	0x10,
	0x23,
	0x9,
	0x13,
	0x28,
	0xa,
	0x16,
	0x2f,
	0x58,
	0x19,
	0x1a,
	0x1b,
	0x3b,
	0x11,
	0x1e,
	0xf,
	0x68,
	0x41,
	0x22,
	0x63,
	0x48,
	0x4a,
	0x26,
	0x27,
	0x14,
	0x29,
	0x55,
	0x57,
	0x2c,
	0x2d,
	0x6b,
	0x5f,
	0xe,
	0xc,
	0x59,
	0x33,
	0x34,
	0x5a,
	0x76,
	0x37,
	0x70,
	0x73,
	0x50,
	0x5d,
	0x1f,
	0x7b,
	0x7c,
	0x3e,
	0x71,
	0x20,
	0x42,
	0x43,
	0x62,
	0x45,
	0x31,
	0x46,
	0x24,
	0x49,
	0x25,
	0x65,
	0x66,
	0x4d,
	0x4e,
	0x67,
	0x21,
	0x74,
	0x69,
	0x53,
	0x54,
	0x2a,
	0x56,
	0x5c,
	0x30,
	0x32,
	0x35,
	0x6c,
	0x2e,
	0x6d,
	0x5e,
	0x17,
	0x0,
	0x7,
	0x44,
	0x1c,
	0x64,
	0x4b,
	0x4c,
	0x4f,
	0x51,
	0x52,
	0x6a,
	0x75,
	0x47,
	0x5b,
	0x6e,
	0x77,
	0x60,
	0x38,
	0x72,
	0x79,
	0x3a,
	0x2b,
	0x36,
	0x6f,
	0x7f,
	0x39,
	0x7d,
	0x3d,
	0x78,
	0x7a,
	0x3f,
	0x7e,
};

unsigned rdcurstrt[] = { 0x8,0x18,0x3c,0x7c };
unsigned rdcurfin[] = { 0x48000000,0x50000000,0xa0000000,0xc0000000,0x80000000 };
unsigned rucurstrt[] = { 0x80000000,0xa0000000,0x48000000,0x24000000,0x12000000 };
unsigned rucurfin[] = { 0x3c,0x1c,0xc };
unsigned ldcurstrt[] = { 0x20000000,0x60000000,0xf0000000,0xf8000000 };
unsigned ldcurfin[] = { 0x44,0x24,0x14,0x6,0x4 };
unsigned lucurstrt[] = { 0x2,0x6,0x14,0x24,0x44 };
unsigned lucurfin[] = { 0x70000000,0x30000000,0x20000000 };

unsigned xdst[] = {

	0x090a0a,	//-121
	0x090a08,	//-120
	0x090a09,	//-119
	0x09080a,	//-118
	0x090808,	//-117
	0x090809,	//-116
	0x09090a,	//-115
	0x090908,	//-114
	0x090909,	//-113
	0x090a02,	//-112
	0x090a00,	//-111
	0x090a01,	//-110
	0x090802,	//-109
	0x090800,	//-108
	0x090801,	//-107
	0x090902,	//-106
	0x090900,	//-105
	0x090901,	//-104
	0x090a06,	//-103
	0x090a04,	//-102
	0x090a05,	//-101
	0x090806,	//-100
	0x090804,	//-99
	0x090805,	//-98
	0x090906,	//-97
	0x090904,	//-96
	0x090905,	//-95
	0x09020a,	//-94
	0x090208,	//-93
	0x090209,	//-92
	0x09000a,	//-91
	0x090008,	//-90
	0x090009,	//-89
	0x09010a,	//-88
	0x090108,	//-87
	0x090109,	//-86
	0x090202,	//-85
	0x090200,	//-84
	0x090201,	//-83
	0x090002,	//-82
	0x090000,	//-81
	0x090001,	//-80
	0x090102,	//-79
	0x090100,	//-78
	0x090101,	//-77
	0x090206,	//-76
	0x090204,	//-75
	0x090205,	//-74
	0x090006,	//-73
	0x090004,	//-72
	0x090005,	//-71
	0x090106,	//-70
	0x090104,	//-69
	0x090105,	//-68
	0x09060a,	//-67
	0x090608,	//-66
	0x090609,	//-65
	0x09040a,	//-64
	0x090408,	//-63
	0x090409,	//-62
	0x09050a,	//-61
	0x090508,	//-60
	0x090509,	//-59
	0x090602,	//-58
	0x090600,	//-57
	0x090601,	//-56
	0x090402,	//-55
	0x090400,	//-54
	0x090401,	//-53
	0x090502,	//-52
	0x090500,	//-51
	0x090501,	//-50
	0x090606,	//-49
	0x090604,	//-48
	0x090605,	//-47
	0x090406,	//-46
	0x090404,	//-45
	0x090405,	//-44
	0x090506,	//-43
	0x090504,	//-42
	0x090505,	//-41
	0x010a0a,	//-40
	0x010a08,	//-39
	0x010a09,	//-38
	0x01080a,	//-37
	0x010808,	//-36
	0x010809,	//-35
	0x01090a,	//-34
	0x010908,	//-33
	0x010909,	//-32
	0x010a02,	//-31
	0x010a00,	//-30
	0x010a01,	//-29
	0x010802,	//-28
	0x010800,	//-27
	0x010801,	//-26
	0x010902,	//-25
	0x010900,	//-24
	0x010901,	//-23
	0x010a06,	//-22
	0x010a04,	//-21
	0x010a05,	//-20
	0x010806,	//-19
	0x010804,	//-18
	0x010805,	//-17
	0x010906,	//-16
	0x010904,	//-15
	0x010905,	//-14
	0x01020a,	//-13
	0x010208,	//-12
	0x010209,	//-11
	0x01000a,	//-10
	0x010008,	//-9
	0x010009,	//-8
	0x01010a,	//-7
	0x010108,	//-6
	0x010109,	//-5
	0x010202,	//-4
	0x010200,	//-3
	0x010201,	//-2
	0x010002,	//-1
	0x010000,	//0
	0x010001,	//1
	0x010102,	//2
	0x010100,	//3
	0x010101,	//4
	0x010206,	//5
	0x010204,	//6
	0x010205,	//7
	0x010006,	//8
	0x010004,	//9
	0x010005,	//10
	0x010106,	//11
	0x010104,	//12
	0x010105,	//13
	0x01060a,	//14
	0x010608,	//15
	0x010609,	//16
	0x01040a,	//17
	0x010408,	//18
	0x010409,	//19
	0x01050a,	//20
	0x010508,	//21
	0x010509,	//22
	0x010602,	//23
	0x010600,	//24
	0x010601,	//25
	0x010402,	//26
	0x010400,	//27
	0x010401,	//28
	0x010502,	//29
	0x010500,	//30
	0x010501,	//31
	0x010606,	//32
	0x010604,	//33
	0x010605,	//34
	0x010406,	//35
	0x010404,	//36
	0x010405,	//37
	0x010506,	//38
	0x010504,	//39
	0x010505,	//40
	0x050a0a,	//41
	0x050a08,	//42
	0x050a09,	//43
	0x05080a,	//44
	0x050808,	//45
	0x050809,	//46
	0x05090a,	//47
	0x050908,	//48
	0x050909,	//49
	0x050a02,	//50
	0x050a00,	//51
	0x050a01,	//52
	0x050802,	//53
	0x050800,	//54
	0x050801,	//55
	0x050902,	//56
	0x050900,	//57
	0x050901,	//58
	0x050a06,	//59
	0x050a04,	//60
	0x050a05,	//61
	0x050806,	//62
	0x050804,	//63
	0x050805,	//64
	0x050906,	//65
	0x050904,	//66
	0x050905,	//67
	0x05020a,	//68
	0x050208,	//69
	0x050209,	//70
	0x05000a,	//71
	0x050008,	//72
	0x050009,	//73
	0x05010a,	//74
	0x050108,	//75
	0x050109,	//76
	0x050202,	//77
	0x050200,	//78
	0x050201,	//79
	0x050002,	//80
	0x050000,	//81
	0x050001,	//82
	0x050102,	//83
	0x050100,	//84
	0x050101,	//85
	0x050206,	//86
	0x050204,	//87
	0x050205,	//88
	0x050006,	//89
	0x050004,	//90
	0x050005,	//91
	0x050106,	//92
	0x050104,	//93
	0x050105,	//94
	0x05060a,	//95
	0x050608,	//96
	0x050609,	//97
	0x05040a,	//98
	0x050408,	//99
	0x050409,	//100
	0x05050a,	//101
	0x050508,	//102
	0x050509,	//103
	0x050602,	//104
	0x050600,	//105
	0x050601,	//106
	0x050402,	//107
	0x050400,	//108
	0x050401,	//109
	0x050502,	//110
	0x050500,	//111
	0x050501,	//112
	0x050606,	//113
	0x050604,	//114
	0x050605,	//115
	0x050406,	//116
	0x050404,	//117
	0x050405,	//118
	0x050506,	//119
	0x050504,	//120
	0x050505,	//121
};

unsigned ydst[] = {

	0x115050,	//-121
	0x115010,	//-120
	0x115090,	//-119
	0x111050,	//-118
	0x111010,	//-117
	0x111090,	//-116
	0x119050,	//-115
	0x119010,	//-114
	0x119090,	//-113
	0x115040,	//-112
	0x115000,	//-111
	0x115080,	//-110
	0x111040,	//-109
	0x111000,	//-108
	0x111080,	//-107
	0x119040,	//-106
	0x119000,	//-105
	0x119080,	//-104
	0x115060,	//-103
	0x115020,	//-102
	0x1150a0,	//-101
	0x111060,	//-100
	0x111020,	//-99
	0x1110a0,	//-98
	0x119060,	//-97
	0x119020,	//-96
	0x1190a0,	//-95
	0x114050,	//-94
	0x114010,	//-93
	0x114090,	//-92
	0x110050,	//-91
	0x110010,	//-90
	0x110090,	//-89
	0x118050,	//-88
	0x118010,	//-87
	0x118090,	//-86
	0x114040,	//-85
	0x114000,	//-84
	0x114080,	//-83
	0x110040,	//-82
	0x110000,	//-81
	0x110080,	//-80
	0x118040,	//-79
	0x118000,	//-78
	0x118080,	//-77
	0x114060,	//-76
	0x114020,	//-75
	0x1140a0,	//-74
	0x110060,	//-73
	0x110020,	//-72
	0x1100a0,	//-71
	0x118060,	//-70
	0x118020,	//-69
	0x1180a0,	//-68
	0x116050,	//-67
	0x116010,	//-66
	0x116090,	//-65
	0x112050,	//-64
	0x112010,	//-63
	0x112090,	//-62
	0x11a050,	//-61
	0x11a010,	//-60
	0x11a090,	//-59
	0x116040,	//-58
	0x116000,	//-57
	0x116080,	//-56
	0x112040,	//-55
	0x112000,	//-54
	0x112080,	//-53
	0x11a040,	//-52
	0x11a000,	//-51
	0x11a080,	//-50
	0x116060,	//-49
	0x116020,	//-48
	0x1160a0,	//-47
	0x112060,	//-46
	0x112020,	//-45
	0x1120a0,	//-44
	0x11a060,	//-43
	0x11a020,	//-42
	0x11a0a0,	//-41
	0x015050,	//-40
	0x015010,	//-39
	0x015090,	//-38
	0x011050,	//-37
	0x011010,	//-36
	0x011090,	//-35
	0x019050,	//-34
	0x019010,	//-33
	0x019090,	//-32
	0x015040,	//-31
	0x015000,	//-30
	0x015080,	//-29
	0x011040,	//-28
	0x011000,	//-27
	0x011080,	//-26
	0x019040,	//-25
	0x019000,	//-24
	0x019080,	//-23
	0x015060,	//-22
	0x015020,	//-21
	0x0150a0,	//-20
	0x011060,	//-19
	0x011020,	//-18
	0x0110a0,	//-17
	0x019060,	//-16
	0x019020,	//-15
	0x0190a0,	//-14
	0x014050,	//-13
	0x014010,	//-12
	0x014090,	//-11
	0x010050,	//-10
	0x010010,	//-9
	0x010090,	//-8
	0x018050,	//-7
	0x018010,	//-6
	0x018090,	//-5
	0x014040,	//-4
	0x014000,	//-3
	0x014080,	//-2
	0x010040,	//-1
	0x010000,	//0
	0x010080,	//1
	0x018040,	//2
	0x018000,	//3
	0x018080,	//4
	0x014060,	//5
	0x014020,	//6
	0x0140a0,	//7
	0x010060,	//8
	0x010020,	//9
	0x0100a0,	//10
	0x018060,	//11
	0x018020,	//12
	0x0180a0,	//13
	0x016050,	//14
	0x016010,	//15
	0x016090,	//16
	0x012050,	//17
	0x012010,	//18
	0x012090,	//19
	0x01a050,	//20
	0x01a010,	//21
	0x01a090,	//22
	0x016040,	//23
	0x016000,	//24
	0x016080,	//25
	0x012040,	//26
	0x012000,	//27
	0x012080,	//28
	0x01a040,	//29
	0x01a000,	//30
	0x01a080,	//31
	0x016060,	//32
	0x016020,	//33
	0x0160a0,	//34
	0x012060,	//35
	0x012020,	//36
	0x0120a0,	//37
	0x01a060,	//38
	0x01a020,	//39
	0x01a0a0,	//40
	0x215050,	//41
	0x215010,	//42
	0x215090,	//43
	0x211050,	//44
	0x211010,	//45
	0x211090,	//46
	0x219050,	//47
	0x219010,	//48
	0x219090,	//49
	0x215040,	//50
	0x215000,	//51
	0x215080,	//52
	0x211040,	//53
	0x211000,	//54
	0x211080,	//55
	0x219040,	//56
	0x219000,	//57
	0x219080,	//58
	0x215060,	//59
	0x215020,	//60
	0x2150a0,	//61
	0x211060,	//62
	0x211020,	//63
	0x2110a0,	//64
	0x219060,	//65
	0x219020,	//66
	0x2190a0,	//67
	0x214050,	//68
	0x214010,	//69
	0x214090,	//70
	0x210050,	//71
	0x210010,	//72
	0x210090,	//73
	0x218050,	//74
	0x218010,	//75
	0x218090,	//76
	0x214040,	//77
	0x214000,	//78
	0x214080,	//79
	0x210040,	//80
	0x210000,	//81
	0x210080,	//82
	0x218040,	//83
	0x218000,	//84
	0x218080,	//85
	0x214060,	//86
	0x214020,	//87
	0x2140a0,	//88
	0x210060,	//89
	0x210020,	//90
	0x2100a0,	//91
	0x218060,	//92
	0x218020,	//93
	0x2180a0,	//94
	0x216050,	//95
	0x216010,	//96
	0x216090,	//97
	0x212050,	//98
	0x212010,	//99
	0x212090,	//100
	0x21a050,	//101
	0x21a010,	//102
	0x21a090,	//103
	0x216040,	//104
	0x216000,	//105
	0x216080,	//106
	0x212040,	//107
	0x212000,	//108
	0x212080,	//109
	0x21a040,	//110
	0x21a000,	//111
	0x21a080,	//112
	0x216060,	//113
	0x216020,	//114
	0x2160a0,	//115
	0x212060,	//116
	0x212020,	//117
	0x2120a0,	//118
	0x21a060,	//119
	0x21a020,	//120
	0x21a0a0,	//121
};

#if PESACT

unsigned pestrn[] = {

	0xf0f0f0, //00 
	0x940a1a, //01 
	0xff750f, //02 
	0x4c9300, //03 
	0xfebdba, //04 
	0x0000ec, //05 
	0x5a99e4, //06 
	0xab48cc, //07 
	0xfac4fd, //08 
	0xcd84dd, //09 
	0x8ad36b, //10 
	0x45a9e4, //11 
	0x42bdff, //12 
	0x00e6ff, //13 
	0x00d96c, //14 
	0x41a9c1, //15 
	0x97adb5, //16 
	0x5f9cba, //17 
	0x9ef5fa, //18 
	0x808080, //19 
	0x000000, //20
	0xdf1c00, //21 
	0xb800df, //22 
	0x626262, //23 
	0x0d2669, //24 
	0x6000ff, //25 
	0x0082bf, //26 
	0x7891f3, //27 
	0x0568ff, //28 
	0xf0f0f0, //29 
	0xcd32c8, //30 
	0x9bbfb0, //31 
	0xebbf65, //32 
	0x04baff, //33 
	0x6cf0ff, //34 
	0x15cafe, //35 
	0x0181f3, //36 
	0x23a937, //37 
	0x5f4623, //38 
	0x95a6a6, //39 
	0xa6bfce, //40 
	0x02aa96, //41 
	0xc6e3ff, //42 
	0xd799ff, //43 
	0x047000, //44 
	0xfbcced, //45 
	0xd889c0, //46 
	0xb4d9e7, //47 
	0x860ee9, //48 
	0x2968cf, //49 
	0x158640, //50 
	0x9717db, //51 
	0x04a7ff, //52 
	0xffffb9, //53 
	0x278922, //54 
	0xcd12b6, //55 
	0x00aa00, //56 
	0xdca9fe, //57 
	0x10d5fe, //58 
	0xdf9700, //59 
	0x84ffff, //60 
	0x74e7cf, //61 
	0x42bdff, //62 
	0xb4d9e7  //63 
};
#endif

DSTREC*		drecs;		//pointer to dst stitch records
unsigned	dstcnt;		//number of dst stitch records
unsigned	trinum;		//for trinary/binary translation
POINT		dstplus;	//plus offset written into the dst file header
POINT		dstmin;		//minus offset written into the dst file header

//bitmap for upper case characters
unsigned upmap[] = {

	//fedcba9876543210fedcba9876543210
0,			//00000000000000000000000000000000
			//?>=</:9876543210 /.-,+*)('&%$#"!
0,			//00000000000000000000000000000000
			//_^]\[ZYXWVUTSRQPONMLKJIHGFEDCBA@
0x07fffffe,	//00000111111111111111111111111110
			// ~}|{zyxwvutsrqponmlkjihgfedcba'
0,			//00000000000000000000000000000000
};

unsigned			flagMap[MAPLEN];	//bitmap
unsigned			binaryVariableBitmap = 0;			//for storage of persistent binary variables set by the user

fPOINTATTRIBUTE		stitchBuffer[MAXPCS];	//main stitch buffer
fPOINTATTRIBUTE		clipBuffer[MAXSEQ];	//for temporary copy of imported clipboard data
FRMHED*				ptrSelectedForm;			//pointer to selected form
unsigned			fillTypes[] =		//fill type array for side window display
{ 0,VRTF,HORF,ANGF,SATF,CLPF,CONTF,VCLPF,HCLPF,ANGCLPF,FTHF,TXVRTF,TXHORF,TXANGF };
//edge fill type array for side window display
unsigned			edgeFillTypes[] = { 0,EGLIN,EGBLD,EGCLP,EGSAT,EGAP,EGPRP,EGHOL,EGPIC,EGDUB,EGCHNL,EGCHNH,EGCLPX };
//feather fill types
unsigned			featherFillTypes[] = { FTHSIN,FTHSIN2,FTHLIN,FTHPSG,FTHRMP,FTHFAZ };

//bitmap functions
unsigned setMap(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset flagMap
		mov		ecx, bPnt
		bts[ebx], ecx
		jnc		short setx
		dec		eax
		setx :
	}
#else
	return _bittestandset((long *)flagMap, bPnt) ? 0xffffffff : 0;
#endif
}

void clrMap(unsigned len) {

#if  __UseASM__
	_asm {

		mov		edi, offset flagMap
		mov		ecx, len
		xor		eax, eax
		rep		stosd
	}
#else
	memset(flagMap, 0, sizeof(*flagMap) * len);
#endif
}

unsigned rstMap(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset flagMap
		mov		ecx, bPnt
		btr[ebx], ecx
		jnc		short rstx
		dec		eax
		rstx :
	}
#else
	return _bittestandreset((long *)flagMap, bPnt) ? 0xffffffff : 0;
#endif
}

unsigned toglMap(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset flagMap
		mov		edx, bPnt
		btc[ebx], edx
		jnc		short toglx
		dec		eax
		toglx :
	}
#else
	return _bittestandcomplement((long *)flagMap, bPnt) ? 0xffffffff : 0;
#endif
}

unsigned chkMap(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset flagMap
		mov		edx, bPnt
		bt[ebx], edx
		jnc		short chkx
		dec		eax
		chkx :
	}
#else
	return _bittest((long *)flagMap, bPnt) ? 0xffffffff : 0;
#endif
}

void cpymap(unsigned dst, unsigned src) {

#if  __UseASM__
	_asm {

		mov		ebx, offset flagMap
		mov		eax, dst
		btr[ebx], eax
		mov		edx, src
		bt[ebx], edx
		jnc		short cpx
		bts[ebx], eax
		cpx :
	}
#else
	// check translation
	if (_bittest((long *)flagMap, src)) {
		_bittestandset((long *)flagMap, dst);
	} else {
		_bittestandreset((long *)flagMap, dst);
	}
#endif
}

//user bitmap functions
unsigned setu(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset binaryVariableBitmap
		mov		ecx, bPnt
		bts[ebx], ecx
		jnc		short setx
		dec		eax
		setx :
	}
#else
	return _bittestandset((long *)&binaryVariableBitmap, bPnt) ? 0xffffffff : 0;
#endif
}

unsigned rstu(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset binaryVariableBitmap
		mov		ecx, bPnt
		btr[ebx], ecx
		jnc		short rstx
		dec		eax
		rstx :
	}
#else
	return _bittestandreset((long *)&binaryVariableBitmap, bPnt) ? 0xffffffff : 0;
#endif
}

unsigned chku(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset binaryVariableBitmap
		mov		edx, bPnt
		bt[ebx], edx
		jnc		short chkx
		dec		eax
		chkx :
	}
#else
	return _bittest((long *)&binaryVariableBitmap, bPnt) ? 0xffffffff : 0;
#endif
}

unsigned toglu(unsigned bPnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset binaryVariableBitmap
		mov		edx, bPnt
		btc[ebx], edx
		jnc		short toglx
		dec		eax
		toglx :
	}
#else
	return _bittestandcomplement((long *)&binaryVariableBitmap, bPnt) ? 0xffffffff : 0;
#endif
}

unsigned dumsk(unsigned pnt) {

#if  __UseASM__
	_asm {

		mov		eax, pnt
		and		al, 31
		mov		cl, 32
		sub		cl, al
		xor		eax, eax
		inc		eax
		shl		eax, cl
		dec		eax
		not eax
		bswap	eax
	}
#else
	return _byteswap_ulong(0xffffffff << (32 - (pnt & 0x1F)));
#endif
}

BOOL CALLBACK dnamproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(lparam);

	HWND hwnd;
	switch (umsg)
	{
	case WM_INITDIALOG:

		hwnd = GetDlgItem(hwndlg, IDC_DESED);
		SetWindowText(hwnd, ini.designerName);
		SetFocus(hwnd);
		SendMessage(hwnd, EM_SETSEL, 0, -1);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			hwnd = GetDlgItem(hwndlg, IDC_DESED);
			GetWindowText(hwnd, ini.designerName, 50);
			EndDialog(hwndlg, 0);
			sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], ini.designerName);
			SetWindowText(hWnd, msgbuf);
			return TRUE;
		}
	}
	return 0;
}

void getdes()
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DESNAM), hWnd, (DLGPROC)dnamproc);
}

BOOL isclp(unsigned find) {

	if ((1 << formList[find].fillType)&clipTypeMap)
		return 1;
	return 0;
}

BOOL isclpx(unsigned find) {

	if (isclp(find) && formList[find].lengthOrCount.clipCount)
		return 1;
	return 0;
}

BOOL isfclp() {

	if (isclp(closestFormToCursor) && formList[closestFormToCursor].fillType != CLPF)
		return 1;
	return 0;
}

BOOL iseclp(unsigned find) {

	if (formList[find].edgeType == EGCLP || formList[find].edgeType == EGPIC || formList[find].edgeType == EGCLPX)
		return 1;
	return 0;
}

BOOL iseclpx(unsigned find) {

	if (iseclp(find) && formList[find].clipEntries)
		return 1;
	return 0;
}

double stlen(unsigned p_stind) {

	return hypot(stitchBuffer[p_stind + 1].x - stitchBuffer[p_stind].x, stitchBuffer[p_stind + 1].y - stitchBuffer[p_stind].y);
}

void undat() {

	if (hfdat) {

		DestroyWindow(hfdat);
		hfdat = 0;
	}
}

void qchk() {

	if (chku(MARQ)) {

		CheckMenuItem(hMen, ID_MARKESC, MF_CHECKED);
		CheckMenuItem(hMen, ID_MARKQ, MF_UNCHECKED);
	} else {

		CheckMenuItem(hMen, ID_MARKESC, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_MARKQ, MF_CHECKED);
	}
	setMap(DUMEN);
}

void nedmen() {

	if (chku(NEDOF)) {

		CheckMenuItem(hMen, ID_SETNEDL, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_RSTNEDL, MF_CHECKED);
	} else {

		CheckMenuItem(hMen, ID_SETNEDL, MF_CHECKED);
		CheckMenuItem(hMen, ID_RSTNEDL, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void knotmen() {

	if (chku(KNOTOF)) {

		CheckMenuItem(hMen, ID_KNOTON, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_KNOTOF, MF_CHECKED);
	} else {

		CheckMenuItem(hMen, ID_KNOTON, MF_CHECKED);
		CheckMenuItem(hMen, ID_KNOTOF, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void bsavmen() {

	if (chku(BSAVOF)) {

		CheckMenuItem(hMen, ID_BSAVON, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_BSAVOF, MF_CHECKED);
	} else {

		CheckMenuItem(hMen, ID_BSAVON, MF_CHECKED);
		CheckMenuItem(hMen, ID_BSAVOF, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void linbmen() {

	if (chku(LINSPAC)) {

		CheckMenuItem(hMen, ID_LINBEXACT, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_LINBEVEN, MF_CHECKED);
	} else {

		CheckMenuItem(hMen, ID_LINBEXACT, MF_CHECKED);
		CheckMenuItem(hMen, ID_LINBEVEN, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void wrnmen()
{
	unsigned cod;

	cod = MF_CHECKED;
	if (chku(WRNOF))
		cod = MF_UNCHECKED;
	CheckMenuItem(hMen, ID_WARNOF, cod);
}

int datcod[] = { ID_CHKOF,ID_CHKON,ID_CHKREP,ID_CHKREPMSG };

void chkmen()
{
	int ind, cod;

	for (ind = 0; ind < (sizeof(datcod) / sizeof(int)); ind++)
	{
		cod = MF_UNCHECKED;
		if (ind == ini.dataCheck)
			cod = MF_CHECKED;
		CheckMenuItem(hMen, datcod[ind], cod);
	}
}

void duzrat() {

	if (!zoomRect.right)
		zoomRect.right = LHUPX;
	if (!zoomRect.top)
		zoomRect.top = LHUPY;
	zoomRatio.x = (double)(stitchWindowClientRect.right) / (zoomRect.right - zoomRect.left);
	zoomRatio.y = (double)(stitchWindowClientRect.bottom / (zoomRect.top - zoomRect.bottom));
}

unsigned rsed() {

	SYSTEMTIME tim;

	GetLocalTime(&tim);
	return (tim.wSecond << 16) | tim.wMilliseconds;
}

void ritfnam(TCHAR* nam) {

	unsigned		ind;
	unsigned char	tnam[50];

	if (*fileNameOrder > 50)
		fnamtabs();
	psgacc = rsed();
	for (ind = 0; ind < 50; ind++)
		tnam[ind] = psg() & 0xff;
	for (ind = 0; ind < 50; ind++) {

		if (nam[ind])
		{
			tnam[ind] = fnamcod[nam[ind]];
		} else {

			while (frencod[tnam[ind]])
				tnam[ind] = psg() & 0xff;
			break;
		}
	}
	if (ind == 50) {

		while (frencod[tnam[49]])
			tnam[49] = psg() & 0xff;
	}
	for (ind = 0; ind < 50; ind++)
		if (fileNameOrder[ind] < 50) { hedx.creatorName[fileNameOrder[ind]] = tnam[ind]; }
}

void redfnam(TCHAR* nam) {

	unsigned ind;
	unsigned char tnam[50];

	for (ind = 0; ind < 50; ind++)
		if (fileNameOrder[ind] < 50)
			tnam[ind] = hedx.creatorName[fileNameOrder[ind]];
		else
			tnam[ind] = 111;
	for (ind = 0; ind < 50; ind++) {

		nam[ind] = frencod[tnam[ind]];
		if (!nam[ind])
			return;
	}
}

void fnamtabs() {

	unsigned		ind, tuns, src, dst;
	unsigned char	chr;
	unsigned*		puns;

	for (ind = 0; ind < 50; ind++)
		fileNameOrder[ind] = ind;
	psgacc = NORDSED;
	for (ind = 0; ind < 100; ind++) {

		src = psg() % 50;
		dst = psg() % 50;
		tuns = fileNameOrder[dst];
		fileNameOrder[dst] = fileNameOrder[src];
		fileNameOrder[src] = tuns;
	}
	for (ind = 0; ind < 128; ind++)
		fnamcod[ind] = (unsigned char)ind + NCODOF;
	psgacc = NCODSED;
	for (ind = 0; ind < 256; ind++) {

		src = psg() & 0x7f;
		dst = psg() & 0x7f;
		chr = fnamcod[dst];
		fnamcod[dst] = fnamcod[src];
		fnamcod[src] = chr;
	}
	puns = (unsigned*)&frencod;
	for (ind = 0; ind < 64; ind++)
		puns[ind] = 0;
	for (ind = 32; ind < 127; ind++)
		frencod[fnamcod[ind]] = (unsigned char)ind;
}

void dstin(unsigned num, POINT* pout) {

	unsigned ind, shft;

	shft = 1;
	pout->x = pout->y = 0;
	for (ind = 0; ind < 22; ind++) {

		if (num&shft) {

			if (dstval[ind].cor)
				pout->y += dstval[ind].val;
			else
				pout->x += dstval[ind].val;
		}
		shft <<= 1;
	}
}

fPOINT* adflt(unsigned cnt) {

	unsigned ind = fltad;

	if (fltad + cnt > MAXFLT)
		tabmsg(IDS_FRMOVR);
	fltad += cnt;
	return &formPoints[ind];
}

SATCON* adsatk(unsigned cnt) {

	unsigned ind = satkad;

	satkad += cnt;
	return &satks[ind];
}

fPOINT* adclp(unsigned cnt) {

	unsigned ind = clpad;

	clpad += cnt;
	return &clipboardPoints[ind];
}

unsigned duthrsh(double var) {

	unsigned	ind = 0;
	double		tdub = 1;

	if (var) {

		while (tdub > var) {

			tdub *= ZUMFCT;
			ind++;
		}
	} else
		return 0;
	return ind + 1;
}

double unthrsh(unsigned lev) {

	double		tdub = 1;

	if (lev)
		lev--;
	else
		return 0;
	while (lev) {

		tdub *= ZUMFCT;
		lev--;
	}
	return tdub;
}

void ritfcor(fPOINT* pnt) {

	sprintf_s(msgbuf, sizeof(msgbuf), "x%.0f y%.0f", pnt->x / PFGRAN, pnt->y / PFGRAN);
	butxt(HCOR, msgbuf);
}

void ritcor(fPOINTATTRIBUTE* pnt) {

	fPOINT	tpnt;

	tpnt.x = pnt->x;
	tpnt.y = pnt->y;
	ritfcor(&tpnt);
}

void coltab() {

	unsigned	ind, col, tuns;
	unsigned	ocol;
	dRECTANGLE		rng;
	fPOINTATTRIBUTE*	ts;

	colCnt = 0;
	if (hed.stitchCount)
	{
		stitchBuffer[0].attribute &= NCOLMSK;
		stitchBuffer[0].attribute |= (stitchBuffer[1].attribute&COLMSK);
		stitchBuffer[hed.stitchCount - 1].attribute &= NCOLMSK;
		stitchBuffer[hed.stitchCount - 1].attribute |= (stitchBuffer[hed.stitchCount - 2].attribute&COLMSK);
		ocol = stitchBuffer[0].attribute&COLMSK;
		for (ind = 1; ind < (unsigned)hed.stitchCount - 1; ind++)
		{
			if ((stitchBuffer[ind].attribute&COLMSK) != ocol)
			{
				if ((stitchBuffer[ind + 1].attribute&COLMSK) == ocol)
				{
					stitchBuffer[ind].attribute &= NCOLMSK;
					stitchBuffer[ind].attribute |= ocol;
				}
				ocol = stitchBuffer[ind].attribute&COLMSK;
			}
		}
		col = 0;
		ocol = 0xffffffff;
		rng.left = -unzoomedRect.x;
		rng.right = unzoomedRect.x * 2;
		rng.bottom = -unzoomedRect.y;
		rng.top = unzoomedRect.y * 2;
		for (ind = 0; ind < hed.stitchCount; ind++)
		{
			ts = &stitchBuffer[ind];
			if (ts->x < rng.left)
				ts->x = (float)rng.left;
			if (ts->x > rng.right)
				ts->x = (float)rng.right;
			if (ts->y > rng.top)
				ts->y = (float)rng.top;
			if (ts->y < rng.bottom)
				ts->y = (float)rng.bottom;
			tuns = ts->attribute&COLMSK;
			if (ocol != tuns)
			{
				colch[col].colorIndex = (unsigned char)tuns;
				colch[col++].stitchIndex = (unsigned short)ind;
				ocol = tuns;
			}
		}
		colCnt = col;
		colch[col].stitchIndex = (unsigned short)ind;
		if (closestPointIndex > (unsigned)hed.stitchCount - 1)
			closestPointIndex = hed.stitchCount - 1;
		fndknt();
	}
}

void ladj() {

	unsigned ind;

	for (ind = 0; ind < 5; ind++) {

		if (ind == activeLayer)
			EnableMenuItem(hMen, ind + M_ALL, MF_BYPOSITION | MF_GRAYED);
		else
			EnableMenuItem(hMen, ind + M_ALL, MF_BYPOSITION | MF_ENABLED);
	}
	setMap(DUMEN);
}

void stchcpy(unsigned p_siz, fPOINTATTRIBUTE* dst) {

#if  __UseASM__
	_asm {

		mov		esi, offset stitchBuffer
		mov		edi, dst
		mov		ecx, p_siz
		rep		movsd
	}
#else
	memcpy(dst, stitchBuffer, p_siz * 4);
#endif
}

void deldu() {

	unsigned ind;

	for (ind = 0; ind < 16; ind++) {

		if (undoBuffer[ind]) {

			free(undoBuffer[ind]);
			undoBuffer[ind] = 0;
		}
	}
	undoBufferWriteIndex = 0;
	rstMap(BAKWRAP);
	rstMap(BAKACT);
}

TCHAR* mvflpnt(fPOINT* dst, fPOINT* src, unsigned cnt) {

#if  __UseASM__
	_asm {

		mov		ecx, cnt
		shl		ecx, 1
		mov		esi, src
		mov		edi, dst
		rep		movsd
		mov		eax, edi
	}
#else
	memcpy(dst, src, cnt * sizeof(fPOINT));
	return (TCHAR *)(dst + cnt);
#endif
}

void mvsatk(SATCON* dst, SATCON* src, unsigned cnt) {

#if  __UseASM__
	_asm {

		mov		ecx, cnt
		mov		esi, src
		mov		edi, dst
		rep		movsd
	}
#else
	memcpy(dst, src, cnt * sizeof(SATCON));
#endif
}

void dudat() {

	unsigned	l_siz;
	BAKHED*		bdat;
	//	unsigned	ind;
	//	unsigned	addr;
	//	unsigned	chksiz;

	if (undoBuffer[undoBufferWriteIndex])
		free(undoBuffer[undoBufferWriteIndex]);
	l_siz = sizeof(BAKHED) + sizeof(FRMHED)*formIndex + sizeof(fPOINTATTRIBUTE)*hed.stitchCount
		+ sizeof(fPOINT)*(fltad + clpad) + sizeof(SATCON)*satkad + sizeof(COLORREF) * 16 +
		sizeof(TXPNT)*txad;
	undoBuffer[undoBufferWriteIndex] = malloc(l_siz);
	bdat = (BAKHED*)undoBuffer[undoBufferWriteIndex];
	if (bdat) {

		bdat->zum0.x = unzoomedRect.x;
		bdat->zum0.y = unzoomedRect.y;
		bdat->fcnt = formIndex;
		bdat->frmp = (FRMHED*)&bdat[1];
		//		for(ind=0;ind<formIndex;ind++)
		//			frmcpy(&bdat->frmp[ind],&formList[ind]);
		MoveMemory(bdat->frmp, &formList, sizeof(FRMHED)*formIndex);
		bdat->scnt = hed.stitchCount;
		bdat->stch = (fPOINTATTRIBUTE*)&bdat->frmp[formIndex];
		if (hed.stitchCount)
			stchcpy((sizeof(fPOINTATTRIBUTE)*hed.stitchCount) >> 2, bdat->stch);
		bdat->fltcnt = fltad;
		bdat->flt = (fPOINT*)&bdat->stch[hed.stitchCount];
		if (fltad)
			mvflpnt(bdat->flt, &formPoints[0], fltad);
		bdat->sacnt = satkad;
		bdat->sac = (SATCON*)&bdat->flt[fltad];
		if (satkad)
			mvsatk(bdat->sac, &satks[0], satkad);
		bdat->nclp = clpad;
		bdat->clp = (fPOINT*)&bdat->sac[satkad];
		if (clpad) {

			if (clpad > MAXCLPNTS)
				clpad = MAXCLPNTS;
			mvflpnt(bdat->clp, &clipboardPoints[0], clpad);
		}
		bdat->cols = (COLORREF*)&bdat->clp[clpad];
		MoveMemory(bdat->cols, &useCol, sizeof(COLORREF) * 16);
		bdat->txp = (TXPNT*)&bdat->cols[16];
		bdat->ntx = txad;
		if (txad)
			MoveMemory(bdat->txp, &txpnts, sizeof(TXPNT)*txad);
	}
}

void savdo() {

	setMap(WASDO);
	setMap(CMPDO);
	if (rstMap(SAVACT)) {

		if (rstMap(BAKING)) {

			rstMap(REDUSHO);
			EnableMenuItem(hMen, M_REDO, MF_BYPOSITION | MF_GRAYED);
		}
		setMap(BAKACT);
		EnableMenuItem(hMen, M_UNDO, MF_BYPOSITION | MF_ENABLED);
		setMap(DUMEN);
		dudat();
		undoBufferWriteIndex++;
		undoBufferWriteIndex &= 0xf;
		if (!undoBufferWriteIndex)
			setMap(BAKWRAP);
	}
}

void redfils() {

	unsigned			ind;
	WIN32_FIND_DATA		fdat;
	HANDLE				hndl;

	for (ind = 0; ind < OLDNUM; ind++) {
		if (GetMenuState(hfileMen, LRUMenuId[ind], MF_BYCOMMAND) != -1)
			DeleteMenu(hfileMen, LRUMenuId[ind], MF_BYCOMMAND);
	}
	for (ind = 0; ind < OLDNUM; ind++) {

		if (ini.prevNames[ind][0]) {

			if (chkMap(SAVAS))
				AppendMenu(hfileMen, MF_BYCOMMAND | MF_STRING, LRUMenuId[ind], ini.prevNames[ind]);
			else {

				hndl = FindFirstFile(ini.prevNames[ind], &fdat);
				if (hndl == INVALID_HANDLE_VALUE)
					ini.prevNames[ind][0] = 0;
				else {

					AppendMenu(hfileMen, MF_BYCOMMAND | MF_STRING, LRUMenuId[ind], ini.prevNames[ind]);
					FindClose(hndl);
				}
			}
		}
	}
	setMap(DUMEN);
}

void nunams() {

	TCHAR*		pext;
	unsigned	ind;
	TCHAR		tnam[_MAX_PATH];

	_strlwr_s(filnam);
	pext = strrchr(filnam, '.');
	if (pext)
		pext++;
	else
		pext = &filnam[strlen(filnam)];
	ind = pext - filnam;
	strncpy_s(auxnam, filnam, ind);
	strncpy_s(thrnam, filnam, ind);
	strncpy_s(genam, filnam, ind);
	pext = auxnam + ind;
	switch (ini.auxFileType) {

	case AUXDST:

		strcpy_s(pext, sizeof(auxnam) - ind, "dst");
		break;

#if PESACT

	case AUXPES:

		strcpy_s(pext, sizeof(auxnam) - ind, "pes");
		break;

#endif

	default:

		strcpy_s(pext, sizeof(auxnam) - ind, "pcs");
	}
	pext = thrnam + ind;
	strcpy_s(pext, sizeof(thrnam) - ind, "thr");
	pext = genam + ind;
	strcpy_s(pext, sizeof(genam) - ind, "th*");
	for (ind = 0; ind < OLDNUM; ind++) {

		if (!strcmp(ini.prevNames[ind], thrnam)) {

			if (ind) {

				strcpy_s(tnam, sizeof(tnam), ini.prevNames[0]);
				strcpy_s(ini.prevNames[0], sizeof(ini.prevNames[0]), ini.prevNames[ind]);
				strcpy_s(ini.prevNames[ind], sizeof(ini.prevNames[ind]), tnam);
				goto mendun;
			} else
				goto nomen;
		}
	}
	for (ind = 0; ind < OLDNUM; ind++) {

		if (!ini.prevNames[ind][0]) {

			strcpy_s(ini.prevNames[ind], thrnam);
			goto mendun;
		}
	}
	strcpy_s(ini.prevNames[3], ini.prevNames[2]);
	strcpy_s(ini.prevNames[2], ini.prevNames[1]);
	strcpy_s(ini.prevNames[1], ini.prevNames[0]);
	strcpy_s(ini.prevNames[0], thrnam);
mendun:;
	redfils();
nomen:;
}

void mvstch(fPOINTATTRIBUTE* dst, fPOINTATTRIBUTE* src) {

#if  __UseASM__
	_asm {

		mov		esi, src
		mov		edi, dst
		xor		ecx, ecx
		mov		cl, 3
		rep		movsd
	}
#else
	memcpy(dst, src, sizeof(fPOINTATTRIBUTE));
#endif
}

void duzero() {

	unsigned		ind;
	unsigned short	dst;
	double			len;
	fPOINTATTRIBUTE*		lpnt;

	if (selectedFormCount) {

		clRmap(RMAPSIZ);
		for (ind = 0; ind < selectedFormCount; ind++)
			setr(selectedFormList[ind]);
		rstMap(CONTIG);
		dst = 0;
		lpnt = stitchBuffer;
		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (stitchBuffer[ind].attribute&TYPMSK&&chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {

				if (setMap(CONTIG)) {

					len = hypot(stitchBuffer[ind].x - lpnt->x, stitchBuffer[ind].y - lpnt->y);
					if (len > minStitchLength) {

						lpnt = &stitchBuffer[ind];
						mvstch(&stitchBuffer[dst++], &stitchBuffer[ind]);
					}
				} else
					lpnt = &stitchBuffer[ind];
			} else {

				mvstch(&stitchBuffer[dst++], &stitchBuffer[ind]);
				rstMap(CONTIG);
			}
		}
		hed.stitchCount = dst;
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		delsmal(groupStartStitch, groupEndStitch);
	} else
		delsmal(0, hed.stitchCount);
}

void rshft(POINT shpnt) {

	zoomRect.right -= shpnt.x;
	zoomRect.left -= shpnt.x;
	zoomRect.top -= shpnt.y;
	zoomRect.bottom -= shpnt.y;
	zRctAdj();
	setMap(RESTCH);
}

void pgdwn() {

	POINT scPnt;

	if (chkMap(ZUMED)) {

		scPnt.y = (zoomRect.top - zoomRect.bottom)*PAGSCROL;
		scPnt.x = 0;
		rshft(scPnt);
	}
}

void pgup() {

	POINT scPnt;

	if (chkMap(ZUMED)) {

		scPnt.y = -(zoomRect.top - zoomRect.bottom)*PAGSCROL;
		scPnt.x = 0;
		rshft(scPnt);
	}
}

void pglft() {

	POINT scPnt;

	if (chkMap(ZUMED)) {

		scPnt.x = (zoomRect.right - zoomRect.left)*PAGSCROL;
		scPnt.y = 0;
		rshft(scPnt);
	}
}

void pgrit() {

	POINT scPnt;

	if (chkMap(ZUMED)) {

		scPnt.x = -(zoomRect.right - zoomRect.left)*PAGSCROL;
		scPnt.y = 0;
		rshft(scPnt);
	}
}

void selin(unsigned strt, unsigned end, HDC dc) {

	unsigned		ind;
	double			tcor;
	long			hi;

	SelectObject(dc, groupSelectPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	if (slpnt)
		Polyline(dc, slin, slpnt);
	if (strt > end) {

		ind = strt;
		strt = end;
		end = ind;
	}
	hi = stitchWindowClientRect.bottom;
	ind = end - strt + 1;
	if (ind < 5)
		ind = 5;
	slpnt = 0;
	for (ind = strt; ind <= end; ind++) {

		tcor = ((stitchBuffer[ind].x - zoomRect.left)*zoomRatio.x + 0.5);
		slin[slpnt].x = (long)tcor;
		tcor = (hi - (stitchBuffer[ind].y - zoomRect.bottom)*zoomRatio.y + 0.5);
		slin[slpnt++].y = (long)tcor;
	}
	Polyline(dc, slin, slpnt);
	SetROP2(dc, R2_COPYPEN);
}

void uncros() {

	if (rstMap(SCROS))
		cros(groupStartStitch);
	if (rstMap(ECROS)) {

		if (groupEndStitch != groupStartStitch)
			cros(groupEndStitch);
		setMap(ELIN);
	}
}

void ducros(HDC dc) {

	uncros();
	setMap(SCROS);
	cros(groupStartStitch);
	if (groupStartStitch != groupEndStitch) {

		cros(groupEndStitch);
		setMap(ECROS);
	}
	selin(groupStartStitch, groupEndStitch, dc);
}

void selRct(fRECTANGLE* srct) {

	unsigned ind;

	srct->left = srct->right = stitchBuffer[groupStartStitch].x;
	srct->top = srct->bottom = stitchBuffer[groupStartStitch].y;
	for (ind = groupStartStitch + 1; ind <= groupEndStitch; ind++) {

		if (stitchBuffer[ind].x > srct->right)
			srct->right = stitchBuffer[ind].x;
		if (stitchBuffer[ind].x < srct->left)
			srct->left = stitchBuffer[ind].x;
		if (stitchBuffer[ind].y < srct->bottom)
			srct->bottom = stitchBuffer[ind].y;
		if (stitchBuffer[ind].y > srct->top)
			srct->top = stitchBuffer[ind].y;
	}
	if (srct->right - srct->left == 0) {

		srct->right++;
		srct->left--;
	}
	if (srct->top - srct->bottom == 0) {

		srct->top++;
		srct->bottom--;
	}
}

void rngadj() {

	if (closestPointIndex > (unsigned)hed.stitchCount - 1)
		closestPointIndex = hed.stitchCount - 1;
	if (groupStitchIndex > hed.stitchCount)
		groupStitchIndex = closestPointIndex;
	if (groupStitchIndex > closestPointIndex) {

		groupStartStitch = closestPointIndex;
		groupEndStitch = groupStitchIndex;
	} else {

		groupStartStitch = groupStitchIndex;
		groupEndStitch = closestPointIndex;
	}
}

void lenfn(unsigned strt, unsigned fin) {

	unsigned	ind;
	double		maxlen = 0;
	double		minlen = 1e99;
	double		tdub;
	TCHAR		buf[50];

	smallestStitchIndex = 0; largestStitchIndex = 0;
	for (ind = strt; ind < fin; ind++) {

		tdub = hypot(stitchBuffer[ind + 1].x - stitchBuffer[ind].x, stitchBuffer[ind + 1].y - stitchBuffer[ind].y);
		if (tdub > maxlen) {

			maxlen = tdub;
			largestStitchIndex = ind;
		}
		if (tdub < minlen) {

			minlen = tdub;
			smallestStitchIndex = ind;
		}
	}
	sprintf_s(buf, sizeof(buf), "max %.2f", maxlen / PFGRAN);
	butxt(HMAXLEN, buf);
	sprintf_s(buf, sizeof(buf), "min %.2f", minlen / PFGRAN);
	butxt(HMINLEN, buf);
}

void frmcalc() {

	unsigned	ind, cod;
	double		maxlen = 0;
	double		minlen = 1e99;
	double		len;

	if (formList[closestFormToCursor].fillType || formList[closestFormToCursor].edgeType) {

		cod = closestFormToCursor << FRMSHFT;
		for (ind = 0; ind < (unsigned)hed.stitchCount - 1; ind++) {

			if ((stitchBuffer[ind].attribute&FRMSK) == cod && !(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind + 1].attribute&FRMSK) == cod && (stitchBuffer[ind + 1].attribute&TYPMSK)) {

				len = hypot(stitchBuffer[ind + 1].x - stitchBuffer[ind].x, stitchBuffer[ind + 1].y - stitchBuffer[ind].y);
				if (len > maxlen) {

					maxlen = len;
					largestStitchIndex = ind;
				}
				if (len < minlen) {

					minlen = len;
					smallestStitchIndex = ind;
				}
			}
		}
		if (fabs(maxlen < 10000)) {

			sprintf_s(msgbuf, sizeof(msgbuf), "max %.2f", maxlen / PFGRAN);
			butxt(HMAXLEN, msgbuf);
		}
		if (fabs(minlen < 10000)) {

			sprintf_s(msgbuf, sizeof(msgbuf), "min %.2f", minlen / PFGRAN);
			butxt(HMINLEN, msgbuf);
		}
	} else {

		butxt(HMAXLEN, "");
		butxt(HMINLEN, "");
	}
}

void lenCalc() {

	if (chkMap(LENSRCH)) {

		sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", hypot(stitchBuffer[closestPointIndex + 1].x - stitchBuffer[closestPointIndex].x, stitchBuffer[closestPointIndex + 1].y - stitchBuffer[closestPointIndex].y) / PFGRAN);
		butxt(HMINLEN, msgbuf);
		msgstr(IDS_SRCH);
		butxt(HMAXLEN, msgbuf);
	} else {

		if (hed.stitchCount > 1) {

			if (chkMap(FORMSEL)) {

				frmcalc();
				butxt(HCOR, "");
				return;
			}
			rngadj();
			if (chkMap(GRPSEL) && groupStartStitch != groupEndStitch)
				lenfn(groupStartStitch, groupEndStitch);
			else
				lenfn(0, hed.stitchCount - 1);
		} else {

			butxt(HMAXLEN, "");
			butxt(HMINLEN, "");
		}
	}
}

void nuselrct() {

	fPOINT		rlin[9];
	unsigned	ind;

	unsel();
	rlin[0].x = rlin[6].x = rlin[7].x = rlin[8].x = rngrct.left;
	rlin[1].x = rlin[5].x = midl(rngrct.right, rngrct.left);
	rlin[0].y = rlin[1].y = rlin[2].y = rlin[8].y = rngrct.top;
	rlin[3].y = rlin[7].y = midl(rngrct.top, rngrct.bottom);
	rlin[4].y = rlin[5].y = rlin[6].y = rngrct.bottom;
	rlin[2].x = rlin[3].x = rlin[4].x = rngrct.right;
	for (ind = 0; ind < 9; ind++)
		sfCor2px(rlin[ind], &formOutlineRectangle[ind]);
}

void grpAdj() {

	POINT		nusiz;
	double		tdub;

	uncros();
	unsel();
	rngadj();
	ducros(StitchWindowDC);
	lenCalc();
	selRct(&rngrct);
	if (chkMap(ZUMED) && groupEndStitch != groupStartStitch) {

		if (rngrct.top > zoomRect.top - 1 || rngrct.bottom < zoomRect.bottom - 1
			|| rngrct.left<zoomRect.left + 1 || rngrct.right>zoomRect.right - 1) {

			nusiz.x = rngrct.right - rngrct.left;
			nusiz.y = rngrct.top - rngrct.bottom;
			if (nusiz.x < MINZUM) {

				if (nusiz.x < TINY)
					nusiz.x = 1;
				tdub = MINZUM / nusiz.x;
				nusiz.x = MINZUM;
				nusiz.y = tdub*nusiz.y;
			}
			if (nusiz.x > nusiz.y) {

				tdub = nusiz.x*ZMARGIN;
				nusiz.x += (long)tdub;
				tdub = nusiz.x / stitchWindowAspectRatio;
				nusiz.y = (long)tdub;
			} else {

				tdub = nusiz.y*ZMARGIN;
				nusiz.y = (long)tdub;
				tdub = nusiz.y*stitchWindowAspectRatio;
				nusiz.x = (long)tdub;
			}
			if (nusiz.x > unzoomedRect.x || nusiz.y > unzoomedRect.y) {

				zoomRect.left = zoomRect.bottom = 0;
				zoomRect.right = unzoomedRect.x;
				zoomRect.top = unzoomedRect.y;
				rstMap(ZUMED);
				zoomFactor = 1;
				movStch();
			} else {

				zoomRect.right = zoomRect.left + (nusiz.x);
				zoomFactor = (double)nusiz.x / unzoomedRect.x;
				zoomRect.top = zoomRect.bottom + (nusiz.y);
				sPnt.x = ((rngrct.right - rngrct.left) / 2) + rngrct.left;
				sPnt.y = ((rngrct.top - rngrct.bottom) / 2) + rngrct.bottom;
				shft(sPnt);
			}
		}
	}
	setMap(RESTCH);
}

void lensadj() {

	uncros();
	unsel();
	closestPointIndex = currentStitchIndex;
	groupStitchIndex = currentStitchIndex + 1;
	rngadj();
	ducros(StitchWindowDC);
	lenCalc();
	selRct(&rngrct);
	if (rngrct.top > zoomRect.top - 1 || rngrct.bottom < zoomRect.bottom - 1
		|| rngrct.left<zoomRect.left + 1 || rngrct.right>zoomRect.right - 1) {

		sPnt.x = ((rngrct.right - rngrct.left) / 2) + rngrct.left;
		sPnt.y = ((rngrct.top - rngrct.bottom) / 2) + rngrct.bottom;
		shft(sPnt);
	}
	nuAct(groupStartStitch);
	setMap(RESTCH);
}

void ritot(unsigned num) {

	TCHAR buf[64];

	sprintf_s(buf, sizeof(buf), stab[STR_TOT], num);
	bufferDigitCount = strlen(buf);
	butxt(HTOT, buf);
}

void ritlayr() {

	TCHAR buf[12];
	unsigned layr;

	layr = 0xffffffff;
	if (chkMap(SELBOX))
		layr = (stitchBuffer[closestPointIndex].attribute&LAYMSK) >> LAYSHFT;
	else {

		if (chkMap(FORMSEL) || chkMap(FRMPSEL))
			layr = (formList[closestFormToCursor].attribute&FRMLMSK) >> 1;
	}
	if (layr & 0xffff0000)
		butxt(HLAYR, "");
	else {

		sprintf_s(buf, sizeof(buf), stab[STR_LAYR], layr);
		bufferDigitCount = strlen(buf);
		butxt(HLAYR, buf);
	}
}

void nuRct() {

	GetClientRect(hWnd, &mRct);
	GetWindowRect(hBar, &colorBarRect);
	//GetClientRect(hBar, &bcRct);
	GetWindowRect(hbuts[HMINLEN], &minLenRct);
	GetWindowRect(hbuts[HMAXLEN], &maxLenRct);
	ReleaseDC(hBar, colorBarDC);
	colorBarDC = GetDC(hBar);
	DeleteDC(StitchWindowMemDC);
	ReleaseDC(hStch, StitchWindowDC);
	DeleteObject(stitchWindowBmp);
	ReleaseDC(hStch, StitchWindowDC);
	StitchWindowDC = GetDCEx(hStch, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	DeleteDC(StitchWindowMemDC);
	StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
	GetDCOrgEx(StitchWindowDC, &stitchWindowOrigin);
	ReleaseDC(hWnd, mainDC);
	mainDC = GetDC(hWnd);
	SetStretchBltMode(mainDC, COLORONCOLOR);
	GetDCOrgEx(mainDC, &mainWindowOrigin);
	GetWindowRect(hStch, &stitchWindowAbsRect);
	GetClientRect(hStch, &stitchWindowClientRect);
	stitchWindowBmp = CreateCompatibleBitmap(StitchWindowDC, stitchWindowClientRect.right, stitchWindowClientRect.bottom);
	SelectObject(StitchWindowMemDC, stitchWindowBmp);
}

HPEN nuPen(HPEN pen, unsigned wid, COLORREF col) {

	DeleteObject(pen);
	return CreatePen(PS_SOLID, wid, col);
}

void nuStchSiz(unsigned ind, unsigned wid) {

	if (wid != pixSiz[ind]) {

		userPen[ind] = nuPen(userPen[ind], wid, useCol[ind]);
		pixSiz[ind] = wid;
	}
}

HBRUSH nuBrush(HBRUSH brsh, COLORREF col) {

	DeleteObject(brsh);
	return CreateSolidBrush(col);
}

void box(unsigned ind, HDC dc) {

	long pwid = boxOff[ind];
	POINT lin[5];

	lin[0].x = pxNer[ind].x - pwid;
	lin[0].y = pxNer[ind].y - pwid;
	lin[1].x = pxNer[ind].x + pwid;
	lin[1].y = pxNer[ind].y - pwid;
	lin[2].x = pxNer[ind].x + pwid;
	lin[2].y = pxNer[ind].y + pwid;
	lin[3].x = pxNer[ind].x - pwid;
	lin[3].y = pxNer[ind].y + pwid;
	lin[4].x = pxNer[ind].x - pwid;
	lin[4].y = pxNer[ind].y - pwid;
	Polyline(dc, lin, 5);
}

void boxs() {

	unsigned	ind;

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	for (ind = 0; ind < selCnt; ind++) {

		SelectObject(StitchWindowDC, boxPen[ind]);
		box(ind, StitchWindowDC);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void dubx() {

	POINT lin[5];
	long pwid = boxOff[0];

	SelectObject(StitchWindowMemDC, boxPen[0]);
	SelectObject(StitchWindowDC, boxPen[0]);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	lin[0].x = boxPix.x - pwid;
	lin[0].y = boxPix.y - pwid;
	lin[1].x = boxPix.x + pwid;
	lin[1].y = boxPix.y - pwid;
	lin[2].x = boxPix.x + pwid;
	lin[2].y = boxPix.y + pwid;
	lin[3].x = boxPix.x - pwid;
	lin[3].y = boxPix.y + pwid;
	lin[4].x = boxPix.x - pwid;
	lin[4].y = boxPix.y - pwid;
	Polyline(StitchWindowMemDC, lin, 5);
	Polyline(StitchWindowDC, lin, 5);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void duar() {

	POINT rpnt;

	stitchArrow[1].x = stitchSizePixels.x;
	stitchArrow[1].y = stitchSizePixels.y;
	pixcntr.x = stitchSizePixels.x;
	pixcntr.y = stitchSizePixels.y;
	rpnt.x = stitchSizePixels.x - 10;
	rpnt.y = stitchSizePixels.y + 10;
	rotpix(rpnt, &stitchArrow[0]);
	rpnt.y = stitchSizePixels.y - 10;
	rotpix(rpnt, &stitchArrow[2]);
	SelectObject(StitchWindowMemDC, boxPen[0]);
	SelectObject(StitchWindowDC, boxPen[0]);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, stitchArrow, 3);
	Polyline(StitchWindowDC, stitchArrow, 3);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void dubox() {

	pixang = atan2(stitchBuffer[closestPointIndex + 1].y - stitchBuffer[closestPointIndex].y, stitchBuffer[closestPointIndex + 1].x - stitchBuffer[closestPointIndex].x);
	duar();
	rstMap(ELIN);
	setMap(SELBOX);
	rstMap(FRMPSEL);
	redraw(hBar);
	if (!bufferIndex)
		ritnum(STR_NUMSEL, closestPointIndex);
}

void unbox() {

	if (rstMap(SELBOX)) {

		SelectObject(StitchWindowDC, boxPen[0]);
		SetROP2(StitchWindowDC, R2_NOTXORPEN);
		Polyline(StitchWindowDC, stitchArrow, 3);
		SetROP2(StitchWindowDC, R2_COPYPEN);
	}
}

void unboxs() {

	if (selCnt) {

		boxs();
		selCnt = 0;
	}
}

void redraw(HWND dWnd) {

	unsigned ind;

	RedrawWindow(dWnd, NULL, NULL, RDW_INVALIDATE);
	if (dWnd == hStch) {

		for (ind = 0; ind < 16; ind++) {

			if (hDef[ind])
				RedrawWindow(hDef[ind], NULL, NULL, RDW_INVALIDATE);
		}
		RedrawWindow(hBar, NULL, NULL, RDW_INVALIDATE);
	}
}

unsigned stch2px(unsigned p_stind) {

	stitchSizePixels.x = (stitchBuffer[p_stind].x - zoomRect.left)*zoomRatio.x + 0.5;
	stitchSizePixels.y = stitchWindowClientRect.bottom - (stitchBuffer[p_stind].y - zoomRect.bottom)*zoomRatio.y + 0.5;
	if (stitchSizePixels.x >= 0 &&
		stitchSizePixels.x <= stitchWindowClientRect.right&&
		stitchSizePixels.y >= 0 &&
		stitchSizePixels.y <= stitchWindowClientRect.bottom)

		return 1;
	else
		return 0;
}

void stch2px1(unsigned p_stind) {

	stitchSizePixels.x = (stitchBuffer[p_stind].x - zoomRect.left)*zoomRatio.x + 0.5;
	stitchSizePixels.y = stitchWindowClientRect.bottom - (stitchBuffer[p_stind].y - zoomRect.bottom)*zoomRatio.y + 0.5;
}

void stch2pxr(fPOINT stpnt) {

	stitchSizePixels.x = (stpnt.x - zoomRect.left)*zoomRatio.x + 0.5;
	stitchSizePixels.y = stitchWindowClientRect.bottom - (stpnt.y - zoomRect.bottom)*zoomRatio.y + 0.5;
}

void movins() {

	if (chkMap(INSRT)) {

		if (chkMap(LIN1)) {

			if (chkMap(BAKEND))
				stch2px1(hed.stitchCount - 1);
			else
				stch2px1(0);
			endpnt();
		} else
			duIns();
	}
}

void defNam(TCHAR* fNam) {

	TCHAR* last;

	if (fNam[0]) {

		strcpy_s(defDir, fNam);
		last = strrchr(defDir, '\\');
		if (last - defDir == 2)
			last[1] = 0;
		else
			last[0] = 0;
	}
}

void defbNam() {

	TCHAR* last;

	if (lbnam[0]) {

		strcpy_s(defbmp, lbnam);
		last = strrchr(defbmp, '\\');
		if (last - defbmp == 2)
			last[1] = 0;
		else
			last[0] = 0;
	}
}

void ritini() {

	unsigned	ind;
	RECT		wrct;

	strcpy_s(ini.defaultDirectory, defDir);
	for (ind = 0; ind < 16; ind++) {

		ini.stitchColors[ind] = useCol[ind];
		ini.backgroundPreferredColors[ind] = bakCust[ind];
		ini.stitchPreferredColors[ind] = custCol[ind];
		ini.bitmapBackgroundColors[ind] = bitmapBackgroundColors[ind];
	}
	ini.backgroundColor = backgroundColor;
	ini.bitmapColor = bitmapColor;
	ini.minStitchLength = minStitchLength;
	ini.showStitchThreshold = showStitchThreshold;
	ini.threadSize30 = threadSize30;
	ini.threadSize40 = threadSize40;
	ini.threadSize60 = threadSize60;
	ini.userStitchLength = userStitchLength;
	ini.smallStitchLength = smallStitchLength;
	ini.StitchBoxesThreshold = StitchBoxesThreshold;
	ini.StitchSpace = StitchSpace;
	ini.binaryVariableBitmap = binaryVariableBitmap;
	ini.borderWidth = borderWidth;
	ini.underlayColor = underlayColor;
	ini.snapLength = snapLength;
	ini.starRatio = starRatio;
	ini.spiralWrap = spiralWrap;
	ini.buttonholeFillCornerLength = buttonholeFillCornerLength;
	ini.picotSpace = picotSpace;
	if (!chku(SAVMAX)) {

		GetWindowRect(hWnd, &wrct);
		ini.initialWindowCoords.left = wrct.left;
		ini.initialWindowCoords.right = wrct.right;
		ini.initialWindowCoords.bottom = wrct.bottom;
		ini.initialWindowCoords.top = wrct.top;
	}
	hIni = CreateFile(iniNam, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hIni != INVALID_HANDLE_VALUE)
		WriteFile(hIni, &ini, sizeof(INIFILE), &red, NULL);
	CloseHandle(hIni);
}

BOOL savcmp() {

#ifdef _DEBUG

	return 1;

#else

	return !chkMap(CMPDO);

#endif
}

void thr2bal(unsigned dst, unsigned src, unsigned cod) {

#define BALRAT 1.6666666666667

	ptrBalaradStitich[dst].flag = 0;
	ptrBalaradStitich[dst].code = (unsigned char)cod;
	ptrBalaradStitich[dst].x = (stitchBuffer[src].x - balaradOffset.x)*BALRAT;
	ptrBalaradStitich[dst].y = (stitchBuffer[src].y - balaradOffset.y)*BALRAT;
}

unsigned coldis(COLORREF acol, COLORREF bcol) {

	unsigned dis;

	dis = ((acol & 0xff) > (bcol & 0xff)) ? ((acol & 0xff) - (bcol & 0xff)) : ((bcol & 0xff) - (acol & 0xff));
	acol >>= 8;
	bcol >>= 8;
	dis += ((acol & 0xff) > (bcol & 0xff)) ? ((acol & 0xff) - (bcol & 0xff)) : ((bcol & 0xff) - (acol & 0xff));
	acol >>= 8;
	bcol >>= 8;
	dis += ((acol & 0xff) > (bcol & 0xff)) ? ((acol & 0xff) - (bcol & 0xff)) : ((bcol & 0xff) - (acol & 0xff));
	return dis;
}

void bal2thr(unsigned dst, unsigned src, unsigned cod) {

#define IBALRAT 0.6

	stitchBuffer[dst].attribute = cod;
	stitchBuffer[dst].x = ptrBalaradStitich[src].x*IBALRAT + balaradOffset.x;
	stitchBuffer[dst].y = ptrBalaradStitich[src].y*IBALRAT + balaradOffset.y;
}

unsigned colmatch(COLORREF col) {

	unsigned ind, dis, mindis, pdis = 0;

	if (colCnt < 16) {

		for (ind = 0; ind < colCnt; ind++) {

			if (col == useCol[ind])
				return ind;
		}
		useCol[colCnt++] = col;
		return colCnt - 1;
	} else {

		mindis = 0xffffffff;
		for (ind = 0; ind < colCnt; ind++) {

			dis = coldis(col, useCol[ind]);
			if (!dis)
				return ind;
			pdis = 0;
			if (dis < mindis) {

				pdis = ind;
				mindis = dis;
			}
		}
		return pdis;
	}
}

void redbal() {

	HANDLE			btfil;
	BALHED			l_bhed;
	unsigned long	l_red;
	unsigned		ind, ine, bcnt, pbcol, col;

	hed.stitchCount = 0;
	formIndex = 0;
	btfil = CreateFile(balnam2, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (btfil != INVALID_HANDLE_VALUE) {

		ReadFile(btfil, (BALHED*)&l_bhed, sizeof(BALHED), &l_red, 0);
		if (l_red == sizeof(BALHED)) {

			ptrBalaradStitich = (BALSTCH*)&bseq;
			ReadFile(btfil, (BALSTCH*)ptrBalaradStitich, sizeof(bseq), &l_red, 0);
			bcnt = l_red / sizeof(BALSTCH);
			ini.backgroundColor = backgroundColor = l_bhed.backgroundColor;
			backgroundPen = nuPen(backgroundPen, 1, backgroundColor);
			backgroundPenWidth = 1;
			DeleteObject(hBackgroundBrush);
			hBackgroundBrush = CreateSolidBrush(backgroundColor);
			unzoomedRect.x = ini.hoopSizeX = l_bhed.hoopSizeX*IBALRAT;
			unzoomedRect.y = ini.hoopSizeY = l_bhed.hoopSizeY*IBALRAT;
			balaradOffset.x = ini.hoopSizeX / 2;
			balaradOffset.y = ini.hoopSizeY / 2;
			ini.hoopType = hed.hoopType = CUSTHUP;
			useCol[0] = l_bhed.color[0];
			col = 0;
			pbcol = 1;
			ine = 0;
			colCnt = 1;
			for (ind = 0; ind < bcnt; ind++) {

				switch (ptrBalaradStitich[ind].code) {

				case BALNORM:

					bal2thr(ine++, ind, col);
					break;

				case BALSTOP:

					col = colmatch(l_bhed.color[pbcol++]);
					break;
				}
			}
			for (ind = 0; ind < colCnt; ind++) {

				userPen[ind] = CreatePen(PS_SOLID, 1, useCol[ind]);
				hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], useCol[ind]);
			}
			hed.stitchCount = ine;
			coltab();
			redraw(hBar);
			setMap(INIT);
			setMap(RESTCH);
		}
	}
	CloseHandle(btfil);
}

void ritbal() {

	BALHED			l_bhed;
	unsigned*		phed;
	unsigned		ind, ine, col;
	HANDLE			bfil;
	TCHAR*			pbchr;
	TCHAR			onam[_MAX_PATH];
	unsigned long	wrot;

	if (*balnam0&&*balnam1&&hed.stitchCount) {

		if (!*filnam) {

			strcpy_s(filnam, defDir);
			strcat_s(filnam, "\\balfil.thr");
		}
		strcpy_s(onam, filnam);
		pbchr = strrchr(onam, '.');
		if (pbchr)
			strcpy_s(pbchr, sizeof(onam) - (pbchr - onam), ".thv");
		else
			strcat_s(onam, sizeof(onam) - strlen(onam), ".thv");
		bfil = CreateFile(onam, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (bfil == INVALID_HANDLE_VALUE)
			return;
		phed = (unsigned*)&l_bhed;
		for (ind = 0; ind < (sizeof(BALHED) >> 2); ind++)
			phed[ind] = 0;
		col = stitchBuffer[0].attribute&COLMSK;
		l_bhed.color[0] = useCol[col];
		ine = 1;
		for (ind = 1; ind < hed.stitchCount; ind++) {

			if (col != (stitchBuffer[ind].attribute&COLMSK)) {

				col = stitchBuffer[ind].attribute&COLMSK;
				l_bhed.color[ine++] = useCol[col];
				if (ine & 0xffffff00)
					break;
			}
		}
		l_bhed.signature = 'drbm';
		l_bhed.backgroundColor = backgroundColor;
		l_bhed.hoopSizeX = ini.hoopSizeX*BALRAT;
		l_bhed.hoopSizeY = ini.hoopSizeY*BALRAT;
		WriteFile(bfil, (BALHED*)&l_bhed, sizeof(BALHED), &wrot, 0);
		balaradOffset.x = ini.hoopSizeX / 2;
		balaradOffset.y = ini.hoopSizeY / 2;
		ptrBalaradStitich = (BALSTCH*)&bseq;
		col = stitchBuffer[0].attribute&COLMSK;
		thr2bal(0, 0, BALJUMP);
		ptrBalaradStitich[1].flag = (unsigned char)col;
		ine = 1;
		for (ind = 0; ind < hed.stitchCount; ind++) {

			thr2bal(ine++, ind, BALNORM);
			if ((stitchBuffer[ind].attribute&COLMSK) != col) {

				thr2bal(ine, ind, BALSTOP);
				col = stitchBuffer[ind].attribute&COLMSK;
				ptrBalaradStitich[ine++].flag = (unsigned char)col;
			}
		}
		WriteFile(bfil, (BALSTCH*)ptrBalaradStitich, ine * sizeof(BALSTCH), &wrot, 0);
		CloseHandle(bfil);
		bfil = CreateFile(balnam1, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		WriteFile(bfil, (TCHAR*)onam, strlen(onam) + 1, &wrot, 0);
		CloseHandle(bfil);
	} else {

		if (*balnam1) {

			bfil = CreateFile(balnam1, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			CloseHandle(bfil);
		}
	}
	if (*balnam0)
		DeleteFile(balnam0);
}

void savmsg() {

	TCHAR	buf[HBUFSIZ];

	LoadString(hInst, IDS_SAVFIL, buf, HBUFSIZ);
	sprintf_s(msgbuf, sizeof(msgbuf), buf, thrnam);
}

void reldun()
{
	txdun();
	ritbal();
	ritini();
	dstcurs();
	PostQuitMessage(0);
}

void dun() {

	savtxt();
	txof();
	rstxt();
	if (rstMap(PRFACT)) {

		DestroyWindow(hPrf);
		rstMap(WASRT);
	}
	unsid();
	unbsho();
	rstAll();
	//	if(savcmp()||(*balnam0&&*balnam1&&hed.stitchCount&&!formIndex))
	if (savcmp() || (*balnam0))
		reldun();
	else {

		if (stitchWindowClientRect.right) {

			savdisc();
			setMap(SAVEX);
		} else {

			savmsg();
			if (MessageBox(hWnd, msgbuf, stab[STR_CLOS], MB_YESNO) == IDYES)
				save();
			reldun();;
		}
	}
}

void dusid(unsigned str) {

	hsidWnd[str] = CreateWindow(
		"STATIC",
		ptrSideWindowsStrings[str],
		SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
		3,
		sideWindowLocation*sideWindowSize.y + 3,
		sideWindowSize.x + 3,
		sideWindowSize.y,
		hSid,
		NULL,
		hInst,
		NULL);
	sideWindowLocation++;
}

void sidmsg(HWND hndl, TCHAR** pstr, unsigned cnt) {

	RECT		chkrct;
	RECT		mrct;
	unsigned	ind, cnt1 = cnt;

	FillMemory(&thDat, sizeof(int)*LASTLIN, 0);
	sideWindowSize.x = sideWindowSize.y = 0;
	sideWindowLocation = 0;
	ptrSideWindowsStrings = pstr;
	GetWindowRect(hndl, &chkrct);
	GetWindowRect(hfdat, &mrct);
	ispcdclp();
	if (chkMap(FILTYP)) {

		for (ind = 0; ind < EDGETYPS + 1; ind++) {

			if ((unsigned)(ptrSelectedForm->edgeType&NEGUND) == edgeFillTypes[ind])
				cnt1--;
			else {

				if (edgeFillTypes[ind] == EGCLP || edgeFillTypes[ind] == EGPIC || edgeFillTypes[ind] == EGCLPX) {

					if (chkMap(WASPCDCLP))
						maxtsiz(pstr[ind], &sideWindowSize);
					else
						cnt1--;
				} else
					maxtsiz(pstr[ind], &sideWindowSize);
			}
		}
		hSid = CreateWindow(
			"STATIC",
			0,
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			mrct.right - mainWindowOrigin.x + 3,
			chkrct.top - mainWindowOrigin.y - 3,
			sideWindowSize.x + 12,
			sideWindowSize.y*cnt1 + 12,
			hWnd,
			NULL,
			hInst,
			NULL);
		for (ind = 0; ind < cnt; ind++) {

			if ((unsigned)(ptrSelectedForm->edgeType&NEGUND) != edgeFillTypes[ind]) {

				if (edgeFillTypes[ind] == EGCLP || edgeFillTypes[ind] == EGPIC || edgeFillTypes[ind] == EGCLPX) {

					if (chkMap(WASPCDCLP))
						dusid(ind);
				} else
					dusid(ind);
			}
		}
	} else {

		if (formMenuChoice == LLAYR)
			maxtsiz("0", &sideWindowSize);
		else {

			if (formMenuChoice == LFTHTYP) {

				cnt1 = 5;
				sideWindowSize.x = buttonWid3;
				sideWindowSize.y = buttonHi;
			} else {

				for (ind = 0; ind < cnt; ind++) {

					if ((1 << fillTypes[ind])&clipTypeMap) {

						if (chkMap(WASPCDCLP))
							maxtsiz(pstr[ind], &sideWindowSize);
						else
							cnt1--;
					} else {

						if (fillTypes[ind] == ptrSelectedForm->fillType)
							cnt1--;
						else
							maxtsiz(pstr[ind], &sideWindowSize);
					}
				}
			}
		}
		hSid = CreateWindow(
			"STATIC",
			0,
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			mrct.right - mainWindowOrigin.x + 3,
			chkrct.top - mainWindowOrigin.y - 3,
			sideWindowSize.x + 12,
			sideWindowSize.y*cnt1 + 12,
			hWnd,
			NULL,
			hInst,
			NULL);
		if (formMenuChoice == LLAYR) {

			for (ind = 0; ind < cnt; ind++)
				dusid(ind);
		} else {

			if (formMenuChoice == LFTHTYP) {

				for (ind = 0; ind < 6; ind++) {

					if (featherFillTypes[ind] != ptrSelectedForm->fillInfo.feather.fillType)
						dusid(ind);
				}
			} else {

				for (ind = 0; ind < cnt; ind++) {

					if (fillTypes[ind] != ptrSelectedForm->fillType) {

						if ((1 << fillTypes[ind])&clipTypeMap) {

							if (chkMap(WASPCDCLP))
								dusid(ind);
						} else
							dusid(ind);
					}
				}
			}
		}
	}
	setMap(SIDACT);
}


void stchPars() {

	aspct = (double)unzoomedRect.x / unzoomedRect.y;
	if (chkMap(RUNPAT) || chkMap(WASPAT))
		stchSiz.x = (long)(mRct.bottom - (SCROLSIZ << 1))*aspct;
	else
		stchSiz.x = (long)(mRct.bottom - SCROLSIZ)*aspct;

	if ((stchSiz.x + (long)buttonWid3 + RIGHTSIZ) < mRct.right) {

		if (chkMap(RUNPAT) || chkMap(WASPAT))
			stchSiz.y = mRct.bottom - (SCROLSIZ << 1);
		else
			stchSiz.y = mRct.bottom - SCROLSIZ;
	} else {

		stchSiz.x = (mRct.right - buttonWid3 - COLSIZ);
		stchSiz.y = mRct.bottom - mRct.top;
		if ((double)stchSiz.x / stchSiz.y > aspct)
			stchSiz.x = stchSiz.y*aspct;
		else
			stchSiz.y = stchSiz.x / aspct;
	}
}

void movStch() {

	POINT		psiz;
	long		vof = 0;
	long		tstchy = stchSiz.y;

	psiz.x = mRct.right - buttonWid3 - RIGHTSIZ;
	psiz.y = mRct.bottom;
	unboxs();
	if (chkMap(RUNPAT) || chkMap(WASPAT)) {

		vof = SCROLSIZ;
		psiz.y -= SCROLSIZ;
		tstchy -= SCROLSIZ;
	}
	if (chkMap(ZUMED)) {

		psiz.y -= SCROLSIZ;
		MoveWindow(hStch, buttonWid3, vof, psiz.x, psiz.y, FALSE);
		MoveWindow(hVrt, buttonWid3 + psiz.x, 0, SCROLSIZ, psiz.y, TRUE);
		MoveWindow(hHor, buttonWid3, psiz.y + vof, psiz.x, SCROLSIZ, TRUE);
		stitchWindowAspectRatio = (double)psiz.x / psiz.y;
		if (chkMap(RUNPAT) || chkMap(WASPAT))
			MoveWindow(hSped, buttonWid3, 0, psiz.x, SCROLSIZ, TRUE);
		ShowWindow(hVrt, TRUE);
		ShowWindow(hHor, TRUE);
	} else {

		stchPars();
		tstchy = stchSiz.y + SCROLSIZ;
		MoveWindow(hStch, buttonWid3, vof, stchSiz.x, tstchy, TRUE);
		ShowWindow(hVrt, FALSE);
		ShowWindow(hHor, FALSE);
		stitchWindowAspectRatio = (double)stchSiz.x / tstchy;
		if (chkMap(RUNPAT) || chkMap(WASPAT))
			MoveWindow(hSped, buttonWid3, 0, stchSiz.x, SCROLSIZ, TRUE);
	}
	MoveWindow(hBar, mRct.right - COLSIZ, 0, COLSIZ, mRct.bottom, TRUE);
	nuRct();
	redraw(hBar);
}

void zRctAdj() {

	double	tdub;

	if (zoomRect.top > unzoomedRect.y) {

		tdub = zoomRect.top - unzoomedRect.y;
		zoomRect.top -= tdub;
		zoomRect.bottom -= tdub;
	}
	if (zoomRect.bottom < 0) {

		zoomRect.top -= zoomRect.bottom;
		zoomRect.bottom = 0;
	}
	if (zoomRect.right > unzoomedRect.x) {

		tdub = zoomRect.right - unzoomedRect.x;
		zoomRect.right -= tdub;
		zoomRect.left -= tdub;
	}
	if (zoomRect.left < 0) {

		zoomRect.right -= zoomRect.left;
		zoomRect.left = 0;
	}
}

void shft(fPOINT shPnt) {

	dPOINT	tsiz;
	dPOINT	centr;
	dPOINT	mov;

	tsiz.x = zoomRect.right - zoomRect.left;
	tsiz.y = zoomRect.top - zoomRect.bottom;
	tsiz.x /= 2;
	tsiz.y /= 2;
	centr.x = zoomRect.left + tsiz.x;
	centr.y = zoomRect.bottom + tsiz.y;
	mov.x = centr.x - shPnt.x;
	mov.y = centr.y - shPnt.y;

	zoomRect.bottom -= mov.y;
	zoomRect.top -= mov.y;
	zoomRect.left -= mov.x;
	zoomRect.right -= mov.x;

	zRctAdj();
}

void centr() {

	POINT l_siz;

	l_siz.x = zoomRect.top - zoomRect.bottom;
	l_siz.y = zoomRect.right - zoomRect.left;
	l_siz.x >>= 1;
	l_siz.y >>= 1;
	sPnt.x = zoomRect.left + l_siz.x;
	sPnt.y = zoomRect.bottom + l_siz.y;
}

double pxchk(double dub) {

	if (dub < 0.2)
		return 1;
	if (dub > 20)
		return 20;
	return dub;
}

void chknum() {

	double		tdub;
	unsigned	tuns;

	clrstch();
	tdub = atof(msgbuf);
	if (rstMap(ENTRDUP)) {

		if (tdub) {

			rotationAngle = tdub*PI / 180;
			ini.rotationAngle = rotationAngle;
		} else
			rotationAngle = ini.rotationAngle;
		duprot();
		return;
	}
	if (rstMap(NUROT)) {

		if (tdub) {

			rotationAngle = tdub*PI / 180;
			ini.rotationAngle = rotationAngle;
		} else
			rotationAngle = ini.rotationAngle;
		return;
	}
	if (rstMap(ENTRSEG)) {

		if (tdub) {

			rotationAngle = 2 * PI / tdub;
			ini.rotationAngle = rotationAngle;
		} else
			rotationAngle = ini.rotationAngle;
		return;
	}
	if (rstMap(ENTROT)) {

		if (tdub) {

			rotationAngle = tdub*PI / 180;
			ini.rotationAngle = rotationAngle;
		} else
			rotationAngle = ini.rotationAngle;
		rotfns();
		return;
	}
	if (msgIndex)
	{
		if (formMenuChoice) {

			tdub = atof(sideWindowEntryBuffer)*PFGRAN;
			switch (formMenuChoice) {

			case LTXOF:

				savdo();
				ptrSelectedForm->txof = tdub;
				break;

			case LUANG:

				savdo();
				ptrSelectedForm->underlayStitchAngle = tdub / 180 * PI / PFGRAN;
				break;

			case LUSPAC:

				savdo();
				ptrSelectedForm->underlaySpacing = tdub;
				break;

			case LWLKIND:

				savdo();
				ptrSelectedForm->underlayIndent = tdub;
				break;

			case LULEN:

				savdo();
				ptrSelectedForm->underlayStitchLen = tdub;
				break;

			case LDSTRT:

				savdo();
				ptrSelectedForm->fillStart = tdub / PFGRAN;
				ptrSelectedForm->fillStart %= sides;
				break;

			case LDEND:

				savdo();
				ptrSelectedForm->fillEnd = tdub / PFGRAN;
				ptrSelectedForm->fillEnd %= sides;
				break;

			case LFTHUPCNT:

				savdo();
				tdub /= PFGRAN;
				if (tdub > 255)
					tdub = 255;
				ptrSelectedForm->fillInfo.feather.upCount = tdub;
				break;

			case LFTHCOL:

				if (tdub) {

					savdo();
					nufthcol((atoi(sideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText(thDat[LFRMCOL], sideWindowEntryBuffer);
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LFRMCOL:

				if (tdub) {

					savdo();
					nufilcol((atoi(sideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText(thDat[LFRMCOL], sideWindowEntryBuffer);
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LUNDCOL:

				if (tdub) {

					savdo();
					ptrSelectedForm->underlayColor = (atoi(sideWindowEntryBuffer) - 1) & 0xf;
					SetWindowText(thDat[LUNDCOL], sideWindowEntryBuffer);
					refilfn();
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LBRDCOL:

				if (tdub) {

					savdo();
					nubrdcol((atoi(sideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText(thDat[LFRMCOL], sideWindowEntryBuffer);
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LBRDPIC:

				savdo();
				ptrSelectedForm->edgeSpacing = tdub;
				refil();
				return;

			case LFRMFAZ:

				savdo();
				ptrSelectedForm->wpar = floor(tdub) / PFGRAN;
				unsid();
				refil();
				return;

			case LBRDPOS:

				savdo();
				ptrSelectedForm->edgeStitchLen = tdub / PFGRAN;
				unsid();
				refil();
				return;

			case LMAXFIL:

				savdo();
				ptrSelectedForm->maxFillStitchLen = tdub;
				unsid();
				refil();
				return;

			case LMINFIL:

				savdo();
				ptrSelectedForm->minFillStitchLen = tdub;
				unsid();
				refil();
				return;

			case LMAXBRD:

				savdo();
				ptrSelectedForm->maxBorderStitchLen = tdub;
				unsid();
				refil();
				return;

			case LMINBRD:

				savdo();
				ptrSelectedForm->minBorderStitchLen = tdub;
				unsid();
				refil();
				return;
			}
			if (formMenuChoice == LBCSIZ) {

				savdo();
				if (ptrSelectedForm->edgeType == EGHOL)
					savblen((float)tdub);
				else
					savplen((float)tdub);
			} else {

				if (tdub) {

					switch (formMenuChoice) {

					case LFTHSIZ:

						savdo();
						ptrSelectedForm->fillInfo.feather.ratio = tdub / PFGRAN;
						break;

					case LFTHNUM:

						savdo();
						ptrSelectedForm->fillInfo.feather.count = tdub / PFGRAN;
						break;

					case LFTHFLR:

						savdo();
						ptrSelectedForm->fillInfo.feather.minStitchSize = tdub;
						break;

					case LFTHDWNCNT:

						savdo();
						tdub /= PFGRAN;
						if (tdub > 255)
							tdub = 255;
						ptrSelectedForm->fillInfo.feather.downCount = tdub;
						break;

					case LFRMSPAC:

						savdo();
						ptrSelectedForm->fillSpacing = tdub;
						break;

					case LFRMLEN:

						savdo();
						ptrSelectedForm->lengthOrCount.stitchLength = tdub;
						break;

					case LBRDSPAC:

						savdo();
						tuns = ptrSelectedForm->edgeType&NEGUND;
						switch (tuns) {

						case EGPRP:

							ptrSelectedForm->edgeSpacing = tdub;
							break;

						case EGCHNH:
						case EGCHNL:

							ptrSelectedForm->edgeSpacing = tdub;
							break;

						default:

							ptrSelectedForm->edgeSpacing = tdub / 2;
						}
						break;

					case LBRDLEN:

						savdo();
						ptrSelectedForm->edgeStitchLen = tdub;
						break;

					case LBRDSIZ:

						savdo();
						ptrSelectedForm->borderSize = tdub;
						break;

					case LFRMANG:

						savdo();
						ptrSelectedForm->angleOrClipData.angle = tdub / 180 * PI / PFGRAN;
						break;

					case LSACANG:

						savdo();
						ptrSelectedForm->satinOrAngle.angle = tdub / 180 * PI / PFGRAN;
						break;

					case LAPCOL:

						savdo();
						ptrSelectedForm->borderColor &= 0xf;
						tuns = tdub / 6;
						if (tuns)
							tuns--;
						tuns &= 0xf;
						ptrSelectedForm->borderColor |= (tuns << 4);
						break;
					}
				} else {

					savdo();
					if (formMenuChoice == LFRMSPAC&&isfclp())
						ptrSelectedForm->fillSpacing = 0;
				}
			}
			unsid();
			refil();
		} else {

			if (preferenceIndex) {

				tdub = atof(sideWindowEntryBuffer);
				switch (preferenceIndex - 1) {

				case PEG:

					ini.eggRatio = tdub;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
					SetWindowText(thDat[PEG], msgbuf);
					break;

				case PNUDG:

					ini.cursorNudgeStep = tdub;
					ini.nudgePixels = pxchk(tdub);
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
					SetWindowText(thDat[PNUDG], msgbuf);
					break;

				case PPIC:

					picotSpace = tdub*PFGRAN;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
					SetWindowText(thDat[PPIC], msgbuf);
					break;

				case PCLPOF:

					ini.clipboardOffset = tdub*PFGRAN;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", tdub);
					SetWindowText(thDat[PCLPOF], msgbuf);
					break;

				case PFAZ:

					ini.fillPhase = floor(tdub);
					sprintf_s(msgbuf, sizeof(msgbuf), "%d", ini.fillPhase);
					SetWindowText(thDat[PFAZ], msgbuf);
					break;

				case PCHRAT:

					ini.chainRatio = tdub;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", ini.chainRatio);
					SetWindowText(thDat[PCHRAT], msgbuf);
					break;

				case PMIN:

					minStitchLength = tdub*PFGRAN;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
					SetWindowText(thDat[PMIN], msgbuf);
					break;

				default:

					if (tdub) {

						sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
						switch (preferenceIndex - 1) {

						case PSPAC:

							StitchSpace = tdub*PFGRAN;
							SetWindowText(thDat[PSPAC], msgbuf);
							break;

						case PANGL:

							ini.fillAngle = tdub / 180 * PI;
							SetWindowText(thDat[PANGL], msgbuf);
							break;

						case PSAT:

							borderWidth = tdub*PFGRAN;
							ini.borderWidth = borderWidth;
							SetWindowText(thDat[PSAT], msgbuf);
							break;

						case PMAX:

							ini.maxStitchLength = tdub*PFGRAN;
							SetWindowText(thDat[PMAX], msgbuf);
							break;

						case PUSE:

							userStitchLength = tdub*PFGRAN;
							SetWindowText(thDat[PUSE], msgbuf);
							break;

						case PSMAL:

							smallStitchLength = tdub*PFGRAN;
							SetWindowText(thDat[PSMAL], msgbuf);
							break;

						case PAP:

							underlayColor = (unsigned)(tdub - 1) % 16;
							sprintf_s(msgbuf, sizeof(msgbuf), "%d", underlayColor + 1);
							SetWindowText(thDat[PAP], msgbuf);
							break;

						case PSNP:

							snapLength = tdub*PFGRAN;
							SetWindowText(thDat[PSNP], msgbuf);
							break;

						case PSTAR:

							starRatio = tdub;
							if (starRatio > 1)
								starRatio = 1;
							if (starRatio < 0.05)
								starRatio = 0.05;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", starRatio);
							SetWindowText(thDat[PSTAR], msgbuf);
							break;

						case PSPIR:

							spiralWrap = tdub;
							if (starRatio > 20)
								starRatio = 20.;
							if (starRatio < 0.3)
								starRatio = 0.3;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", spiralWrap);
							SetWindowText(thDat[PSPIR], msgbuf);
							break;

						case PBUT:

							buttonholeFillCornerLength = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
							SetWindowText(thDat[PBUT], msgbuf);
							break;

						case PHUPX:

							ini.hoopSizeX = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", tdub);
							SetWindowText(thDat[PHUPX], msgbuf);
							sethup();
							prfmsg();
							chkhup();
							break;

						case PHUPY:

							ini.hoopSizeY = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", tdub);
							SetWindowText(thDat[PHUPY], msgbuf);
							sethup();
							prfmsg();
							chkhup();
							break;

						case PGRD:

							ini.gridSize = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", tdub);
							SetWindowText(thDat[PGRD], msgbuf);
							break;

						case PCHN:

							ini.chainSpace = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
							SetWindowText(thDat[PCHN], msgbuf);
							break;
						}
					}
				}
				unsid();
				preferenceIndex = 0;
			} else {

				tdub = atof(msgbuf);
				if (rstMap(SCLPSPAC))
					ini.clipboardOffset = tdub*PFGRAN;
				if (rstMap(FSETFIND))
					dufind(tdub);
				if (rstMap(FSETFHI))
				{
					if (tdub)
						dufhi(tdub*PFGRAN);
				}
				if (rstMap(FSETFWID))
				{
					if (tdub)
						dufwid(tdub*PFGRAN);
				}
				if (rstMap(FSETFMAX))
				{
					if (tdub)
						dufmax(tdub*PFGRAN);
				}
				if (rstMap(FSETFMIN))
					dufmin(tdub*PFGRAN);
				if (rstMap(FSETBMAX))
				{
					if (tdub)
						dubmax(tdub*PFGRAN);
				}
				if (rstMap(FSETBMIN))
					dubmin(tdub*PFGRAN);
				if (rstMap(FSETBSPAC))
				{
					if (tdub)
						dubspac(tdub*PFGRAN);
				}
				if (rstMap(FSETFLEN))
				{
					if (tdub)
						dublen(tdub*PFGRAN);
				}
				if (rstMap(FSETBCOL))
					dubcol(tdub);
				if (rstMap(FSETFCOL))
					dufcol(tdub);
				if (rstMap(FSETUCOL))
					dundcol(tdub);
				if (rstMap(FSETFANG))
					dufxang(tdub);
				if (rstMap(FSETFSPAC))
				{
					if (tdub)
						dufspac(tdub*PFGRAN);
				}
				if (rstMap(FSETUANG))
					dufang(tdub);
				if (rstMap(FSETFLEN))
				{
					if (tdub)
						duflen(tdub*PFGRAN);
				}
				if (rstMap(FSETUSPAC))
				{
					if (tdub)
						duspac(tdub*PFGRAN);
				}
				if (rstMap(FSETULEN))
				{
					if (tdub)
						dusulen(tdub*PFGRAN);
				}
				if (rstMap(GTUANG))
					ini.underlayAngle = tdub / 180 * PI;
				if (rstMap(GTUSPAC))
				{
					if (tdub)
						ini.underlaySpacing = tdub*PFGRAN;
				}
				if (rstMap(GTWLKIND))
					ini.underlayIndent = tdub*PFGRAN;
				if (rstMap(GTWLKLEN))
				{
					if (tdub)
						ini.underlayStitchLen = tdub*PFGRAN;
				}
				if (rstMap(PIXIN))
					ini.nudgePixels = pxchk(tdub);
				if (rstMap(STPXIN))
					ini.stitchSizePixels = pxchk(tdub);
				if (rstMap(FRMPXIN))
					ini.formPointSizePixels = tdub;
				if (rstMap(FRMBOXIN))
					ini.formBoxSizePixels = tdub;
				if (rstMap(GETMIN))
					smallStitchLength = tdub*PFGRAN;
				if (rstMap(ENTR30))
					threadSize30 = tdub;
				if (rstMap(ENTR40))
					threadSize40 = tdub;
				if (rstMap(ENTR60))
					threadSize60 = tdub;
				if (rstMap(ENTRFNUM)) {

					if (tdub < formIndex)
						frmnumfn(tdub);
					else
						tabmsg(IDS_FRMN1);
					return;
				}
				tdub = floor(tdub);
				if (rstMap(ENTRPOL))
					durpoli(tdub);
				if (rstMap(ENTRSTAR))
					dustar(tdub, 250 / tdub*zoomFactor*(unzoomedRect.x + unzoomedRect.y) / (LHUPX + LHUPY));
				if (rstMap(ENTRSPIR))
					duspir(tdub);
				if (rstMap(ENTRHART))
					duhart(tdub);
				if (rstMap(ENTRLENS))
					dulens(tdub);
				if (rstMap(ENTREG))
					dueg(tdub);
				if (rstMap(ENTRZIG))
					duzig(tdub);
			}
		}
	}
	setMap(RESTCH);
}

void noMsg() {

	if (DestroyWindow(hMsg))
		hMsg = 0;
	DestroyWindow(hok);
	DestroyWindow(hdsc);
	DestroyWindow(hcan);
	DestroyWindow(hmin);
	DestroyWindow(hto);
	if (rstMap(NUMIN) || formMenuChoice || preferenceIndex)
		chknum();
	rstMap(MOVMSG);
	rstMap(DELFRM);
	rstMap(FILMSG);
	setMap(RESTCH);
}

void stchWnd() {

	stchPars();

	hStch = CreateWindow(
		"STATIC",
		0,
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWid3,
		0,
		stchSiz.x,
		stchSiz.y,
		hWnd,
		NULL,
		hInst,
		NULL);

	GetWindowRect(hStch, &stitchWindowAbsRect);

	hVrt = CreateWindow(
		"SCROLLBAR",
		0,
		SBS_VERT | WS_CHILD | WS_VISIBLE,
		stchSiz.x + buttonWid3,
		0,
		SCROLSIZ,
		stchSiz.y,
		hWnd,
		NULL,
		hInst,
		NULL);

	hHor = CreateWindow(
		"SCROLLBAR",
		0,
		SBS_HORZ | WS_CHILD | WS_VISIBLE,
		buttonWid3,
		stchSiz.y,
		stchSiz.x,
		SCROLSIZ,
		hWnd,
		NULL,
		hInst,
		NULL);
	ShowWindow(hVrt, FALSE);
	ShowWindow(hHor, FALSE);
}

//check if a click occurred in A veritcal set of 16 windows
// and calculate which window had the click
unsigned chkMsgs(POINT pt, HWND tWnd, HWND bWnd) {

	RECT	tRct;
	RECT	l_bRct;

	GetWindowRect(tWnd, &tRct);
	GetWindowRect(bWnd, &l_bRct);

	if (pt.x > tRct.left&&pt.x<l_bRct.right&&pt.y>tRct.top&&pt.y < l_bRct.bottom) {

		vind = 15 - (l_bRct.bottom - pt.y) / buttonHi;
		return 1;
	} else
		return 0;
}

HWND nuSiz(unsigned wndPnt) {

	TCHAR*	str[] = { "30","40","60" };

	return CreateWindow(
		"STATIC",
		str[wndPnt],
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWid3,
		buttonHi*(wndPnt + vind),
		buttonWid,
		buttonHi,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void delstch1(unsigned ind) {

	if (hed.stitchCount) {

		while (ind <= hed.stitchCount) {

			stitchBuffer[ind].attribute = stitchBuffer[ind + 1].attribute;
			stitchBuffer[ind].x = stitchBuffer[ind + 1].x;
			stitchBuffer[ind].y = stitchBuffer[ind + 1].y;
			ind++;
		}
		hed.stitchCount--;
		if (closestPointIndex > (unsigned)hed.stitchCount - 1)
			closestPointIndex = hed.stitchCount - 1;
	}
}

void stchred(unsigned p_siz, fPOINTATTRIBUTE* src) {

#if  __UseASM__
	_asm {

		mov		edi, offset stitchBuffer
		mov		esi, src
		mov		ecx, p_siz
		rep		movsd
	}
#else
	memcpy(stitchBuffer, src, p_siz * 4);
#endif
}

void redbak() {

	BAKHED*		bdat;
	unsigned	ind;

	bdat = (BAKHED*)undoBuffer[undoBufferWriteIndex];
	hed.stitchCount = bdat->scnt;
	if (hed.stitchCount)
		stchred((sizeof(fPOINTATTRIBUTE)*hed.stitchCount) >> 2, bdat->stch);
	unzoomedRect.x = bdat->zum0.x;
	unzoomedRect.y = bdat->zum0.y;
	formIndex = bdat->fcnt;
	//	for(ind=0;ind<formIndex;ind++)
	//		frmcpy(&formList[ind],&bdat->frmp[ind]);
	MoveMemory(&formList, bdat->frmp, sizeof(FRMHED)*formIndex);
	fltad = bdat->fltcnt;
	if (fltad)
		mvflpnt(&formPoints[0], &bdat->flt[0], fltad);
	satkad = bdat->sacnt;
	if (satkad)
		mvsatk(&satks[0], &bdat->sac[0], satkad);
	clpad = bdat->nclp;
	if (clpad)
		mvflpnt(&clipboardPoints[0], &bdat->clp[0], clpad);
	MoveMemory(&useCol, bdat->cols, sizeof(COLORREF) * 16);
	for (ind = 0; ind < 16; ind++) {

		userPen[ind] = nuPen(userPen[ind], 1, useCol[ind]);
		hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], useCol[ind]);
	}
	for (ind = 0; ind < 16; ind++)
		redraw(hCol[ind]);
	txad = bdat->ntx;
	if (txad)
		MoveMemory(&txpnts, bdat->txp, sizeof(TXPNT)*txad);
	coltab();
	setMap(RESTCH);
}

void redo() {

	unsigned ind;

	if (chkMap(BAKACT) && chkMap(REDUSHO)) {

		undoBufferWriteIndex++;
		undoBufferWriteIndex &= 0xf;
		ind = (undoBufferWriteIndex + 1) & 0xf;
		if (ind == undoBufferReadIndex) {

			EnableMenuItem(hMen, M_REDO, MF_BYPOSITION | MF_GRAYED);
			rstMap(REDUSHO);
		} else {

			if (!setMap(REDUSHO))
				EnableMenuItem(hMen, M_REDO, MF_BYPOSITION | MF_ENABLED);
		}
		if (!setMap(UNDUSHO)) {

			EnableMenuItem(hMen, M_UNDO, MF_BYPOSITION | MF_ENABLED);
		}
		redbak();
		setMap(DUMEN);
	}
}

void bak() {

	unsigned ind;

	unmsg();
	rstMap(FPSEL);
	rstMap(FRMPSEL);
	rstMap(BIGBOX);
	if (rstMap(PRFACT)) {

		rstMap(WASRT);
		DestroyWindow(hPrf);
		preferenceIndex = 0;
		unsid();
	}
	if (!setMap(BAKING)) {

		dudat();
		undoBufferReadIndex = undoBufferWriteIndex + 1;
	}
	if (chkMap(BAKWRAP)) {

		undoBufferWriteIndex--;
		undoBufferWriteIndex &= 0xf;
		ind = undoBufferWriteIndex - 1;
		if (ind == undoBufferReadIndex) {

			EnableMenuItem(hMen, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			setMap(DUMEN);
			rstMap(UNDUSHO);
		}
	} else {

		if (undoBufferWriteIndex)
			undoBufferWriteIndex--;
		if (!undoBufferWriteIndex) {

			EnableMenuItem(hMen, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			setMap(DUMEN);
			rstMap(UNDUSHO);
		}
	}
	if (!setMap(REDUSHO)) {

		EnableMenuItem(hMen, M_REDO, MF_BYPOSITION | MF_ENABLED);
		setMap(DUMEN);
	}
	rstMap(FORMSEL);
	rstMap(GRPSEL);
	redbak();
}

void bitsiz() {

	double				srat;
	double				brat;

	brat = (double)bitmapWidth / bitmapHeight;
	srat = (double)unzoomedRect.x / unzoomedRect.y;
	if (brat > srat) {

		bitmapSizeinStitches.x = unzoomedRect.x;
		bitmapSizeinStitches.y = unzoomedRect.x / brat;
	} else {

		bitmapSizeinStitches.y = unzoomedRect.y;
		bitmapSizeinStitches.x = unzoomedRect.y*brat;
	}
	bmpStitchRatio.x = (double)bitmapWidth / bitmapSizeinStitches.x;
	bmpStitchRatio.y = (double)bitmapHeight / bitmapSizeinStitches.y;
	stitchBmpRatio.x = (double)bitmapSizeinStitches.x / bitmapWidth;
	stitchBmpRatio.y = (double)bitmapSizeinStitches.y / bitmapHeight;
}

BOOL binv(unsigned cnt) {

	unsigned	ind, ine, icnt, ncnt, lcnt;
	TCHAR*		bcpnt;

	icnt = ncnt = 0;
	lcnt = bitmapWidth >> 3;
	for (ind = 0; ind < bitmapHeight; ind++) {

		bcpnt = (TCHAR*)&ptrBitmap[cnt*ind];
		for (ine = 0; ine < lcnt; ine++) {

			if (!bcpnt[ine])
				ncnt++;
			else {

				if (bcpnt[ine] == (TBYTE)0xff)
					icnt++;
			}
		}
	}
	if (icnt > ncnt)
		return 1;
	else
		return 0;
}

void bitlin(unsigned* src, unsigned* dst, COLORREF fgnd, COLORREF bgnd) {

#if  __UseASM__
	_asm {

		mov		esi, src
		mov		edi, dst
		mov		ecx, bitmapWidth
		xor		eax, eax
		blup : mov		ebx, eax
			   xor		bl, 7
			   mov		edx, bgnd
			   bt[esi], ebx
			   jnc		short blup1
			   mov		edx, fgnd
			   blup1 : mov[edi], edx
					   add		edi, 4
					   inc		eax
					   loop	blup
	}
#else
	//correct
	union {
		struct {
			unsigned char byte0;
			unsigned char byte1;
			unsigned char byte2;
			unsigned char byte3;
		} bytes;
		unsigned dword0;
	} bit;

	for (unsigned i = 0; i < bitmapWidth; i++) {
		bit.dword0 = i;
		bit.bytes.byte0 = bit.bytes.byte0 ^ 0x07;

		*dst = (_bittest((long *)src, bit.dword0)) ? fgnd : bgnd;

		dst++;
	}
#endif
}

COLORREF fswap(COLORREF tcol) {

#if  __UseASM__
	_asm {

		mov		eax, tcol
		bswap	eax
		shr		eax, 8
	}
#else
	return _byteswap_ulong(tcol) >> 8;
#endif
}

BOOL gudtyp(WORD bcnt) {

	switch (bcnt) {

	case 1:

		return 1;

	case 24:

		return 1;

	case 32:

		return 1;
	}
	return 0;
}

void movmap(unsigned cnt) {

#if  __UseASM__
	_asm {

		mov		ecx, cnt
		mov		esi, traceBitmap
		mov		edi, offset bseq
		movlup : mov		eax, [esi]
				 add		esi, 4
				 mov[edi], eax
				 add		edi, 3
				 loop	movlup
	}
#else
	//check translation
	unsigned *src = traceBitmap;
	char *dst = (char *)bseq;

	for (unsigned i = 0; i < cnt; i++) {
		*(unsigned *)dst = *(src++);

		dst += 3;
	}
#endif
}

void savmap() {

	unsigned long		rot;

	if (*bnam) {

		if (chkMap(MONOMAP)) {

			tabmsg(IDS_SAVMAP);
			return;
		}
		if (!chkMap(WASTRAC)) {

			tabmsg(IDS_MAPCHG);
			return;
		}
		if (GetSaveFileName(&obn)) {

			hBmp = CreateFile(lbnam, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if (hIni == INVALID_HANDLE_VALUE) {

				crmsg(lbnam);
				return;
			}
			WriteFile(hBmp, (BITMAPFILEHEADER*)&bhed, 14, &rot, NULL);
			WriteFile(hBmp, (BITMAPV4HEADER*)&b4hed, bhed.bfOffBits - 14, &rot, NULL);
			movmap(bitmapWidth*bitmapHeight);
			WriteFile(hBmp, (BSEQPNT*)bseq, bitmapWidth*bitmapHeight * 3, &rot, NULL);
			CloseHandle(hBmp);
		}
	} else
		tabmsg(IDS_SHOMAP);
}

void bfil() {

	unsigned			bwidw, bovr, bsiz, ind;
	unsigned*			pbits;
	HBITMAP				tbit;
	HDC					tdc;
	COLORREF			fgnd;
	COLORREF			bgnd;
	COLORREF			bitbak;

	bitbak = fswap(backgroundColor);
	hBmp = CreateFile(lbnam, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (hBmp == INVALID_HANDLE_VALUE) {

		sprintf_s(msgbuf, sizeof(msgbuf), "Couldn't open bitmap file: %s\n", lbnam);
		shoMsg(msgbuf);
		CloseHandle(hBmp);
		hBmp = 0;
		*bnam = 0;
		return;
	}
	ReadFile(hBmp, (BITMAPFILEHEADER*)&bhed, 14, &red, NULL);
	if (bhed.bfType == 'MB') {

		bsiz = bhed.bfOffBits - 14;
		if (bsiz > sizeof(BITMAPV4HEADER))
			bsiz = sizeof(BITMAPV4HEADER);
		ReadFile(hBmp, (BITMAPV4HEADER*)&b4hed, bsiz, &red, NULL);
	} else {

		sprintf_s(msgbuf, sizeof(msgbuf), "%s is not a Windows Bitmap\n", lbnam);
		CloseHandle(hBmp);
		hBmp = 0;
		*bnam = 0;
		return;
	}
	if (gudtyp(b4hed.bV4BitCount)) {

		if (!rstMap(WASESC))
			rstMap(TRSET);
		bitmapWidth = b4hed.bV4Width;
		bitmapHeight = b4hed.bV4Height;
		setMap(INIT);
		zoomRect.left = zoomRect.bottom = 0;
		zoomRect.right = unzoomedRect.x;
		zoomRect.top = unzoomedRect.y;
		bitmapDC = CreateCompatibleDC(StitchWindowDC);
		if (b4hed.bV4BitCount == 1) {

			setMap(MONOMAP);
			bwidw = bitmapWidth >> 5;
			bovr = bitmapWidth % 32;
			if (bovr)
				bwidw++;
			bsiz = bwidw*bitmapHeight;
			ptrBitmap = new unsigned[bsiz];
			ReadFile(hBmp, (unsigned*)ptrBitmap, bsiz << 2, &red, NULL);
			CloseHandle(hBmp);
			if (binv(bwidw)) {

				bgnd = bitmapColor;
				fgnd = bitbak;
			} else {

				fgnd = bitmapColor;
				bgnd = bitbak;
			}
			memset((BITMAPINFOHEADER*)&binfh, 0, sizeof(BITMAPINFOHEADER));
			binfh.biSize = sizeof(BITMAPINFOHEADER);
			binfh.biWidth = bitmapWidth;
			binfh.biHeight = bitmapHeight;
			binfh.biPlanes = 1;
			binfh.biBitCount = 32;
			binfh.biCompression = BI_RGB;
			binf.bmiHeader = binfh;
			tbit = CreateDIBSection(bitmapDC, &binf, DIB_RGB_COLORS, (void**)&pbits, 0, 0);
			for (ind = 0; ind < bitmapHeight; ind++)
				bitlin(&ptrBitmap[ind*bwidw], &pbits[ind*bitmapWidth], bgnd, fgnd);
			tdc = CreateCompatibleDC(StitchWindowDC);
			if (tbit && tdc) {
				SelectObject(tdc, tbit);
				hBmp = CreateCompatibleBitmap(StitchWindowDC, bitmapWidth, bitmapHeight);
				SelectObject(bitmapDC, hBmp);
				BitBlt(bitmapDC, 0, 0, bitmapWidth, bitmapHeight, tdc, 0, 0, SRCCOPY);
				DeleteObject(tbit);
				ReleaseDC(hWnd, tdc);
			}
			delete[] ptrBitmap;
		} else {

			CloseHandle(hBmp);
			rstMap(MONOMAP);
			hBmp = (HBITMAP)LoadImage(hInst, lbnam, IMAGE_BITMAP, bitmapWidth, bitmapHeight, LR_LOADFROMFILE);
			SelectObject(bitmapDC, hBmp);
			setMap(RESTCH);
		}
		bitsiz();
		upColor = 0;
		downColor = 0x7f7f7f;
		invertUpColor = 0xffffff;
		invertDownColor = 0x808080;
		rstMap(HIDMAP);
	} else {

		CloseHandle(hBmp);
		hBmp = 0;
		*bnam = 0;
		tabmsg(IDS_BMAP);
	}
}

void prtred() {

	CloseHandle(hFil);
	rstMap(INIT);
	formIndex = 0;
	tabmsg(IDS_PRT);
	coltab();
	setMap(RESTCH);
}

unsigned dtrn(DSTREC* dpnt) {

#if  __UseASM__
	_asm {

		mov		eax, dpnt
		mov		eax, [eax]
	}
#else
	return *(unsigned *)dpnt;
#endif
}

BOOL colfil() {

	TCHAR*	pext;

	strcpy_s(colorFileName, filnam);
	strcpy_s(rgbFileName, filnam);
	pext = strrchr(colorFileName, '.');
	if (pext) {

		pext++;
		strcpy_s(pext, sizeof(colorFileName) - (pext - colorFileName), "thw");
		pext = strrchr(rgbFileName, '.');
		pext++;
		strcpy_s(pext, sizeof(rgbFileName) - (pext - rgbFileName), "rgb");
		return 1;
	} else
		return 0;
}

void dstran() {

	unsigned		ind, ine, col;
	POINT			loc;
	POINT			nu;
	fPOINT			max;
	fPOINT			min;
	fPOINT			l_siz;
	fPOINT			dif;
	HANDLE			hcol;
	unsigned*		pcol;
	unsigned		colind;
	DWORD			hisiz;
	LARGE_INTEGER	fisiz;
	BOOL			retval;

	pcol = 0;
	if (colfil()) {

		hcol = CreateFile(colorFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (hcol != INVALID_HANDLE_VALUE) {

			retval = GetFileSizeEx(hcol, &fisiz);
			// ToDo - check HighPart is non-zero
			pcol = new unsigned[fisiz.u.LowPart];
			ReadFile(hcol, (unsigned*)pcol, fisiz.u.LowPart, &hisiz, 0);
			CloseHandle(hcol);
			if (hisiz > 1) {
				if (hisiz&&pcol&&pcol[0] == COLVER) {

					backgroundColor = pcol[1];
					colCnt = 0;
				}
			} else {

				if (pcol) {

					delete[] pcol;
					pcol = 0;
				}
			}
		}
	}
	ine = 0;
	if (pcol)
		col = colmatch(pcol[2]);
	else
		col = 0;
	colind = 3;
	loc.x = loc.y = 0;
	max.x = max.y = (float)-1e12;
	min.x = min.y = (float)1e12;
	for (ind = 0; ind < dstcnt; ind++) {

		if (drecs[ind].nd & 0x40) {

			if (pcol)
				col = colmatch(pcol[colind++]);
			else {

				col++;
				col &= 0xf;
			}
		} else {

			dstin(dtrn(&drecs[ind]), &nu);
			loc.x += nu.x;
			loc.y += nu.y;
			if (!(drecs[ind].nd & 0x80)) {

				stitchBuffer[ine].attribute = col | NOTFRM;
				stitchBuffer[ine].x = loc.x*0.6;
				stitchBuffer[ine].y = loc.y*0.6;
				if (stitchBuffer[ine].x > max.x)
					max.x = stitchBuffer[ine].x;
				if (stitchBuffer[ine].y > max.y)
					max.y = stitchBuffer[ine].y;
				if (stitchBuffer[ine].x < min.x)
					min.x = stitchBuffer[ine].x;
				if (stitchBuffer[ine].y < min.y)
					min.y = stitchBuffer[ine].y;
				ine++;
			}
		}
	}
	if (pcol)
		delete[] pcol;
	hed.stitchCount = ine;
	l_siz.x = max.x - min.x;
	l_siz.y = max.y - min.y;
	ini.hoopType = CUSTHUP;
	unzoomedRect.x = ini.hoopSizeX;
	unzoomedRect.y = ini.hoopSizeY;
	if (l_siz.x > unzoomedRect.x || l_siz.y > unzoomedRect.y) {

		ini.hoopSizeX = unzoomedRect.x = l_siz.x*1.1;
		ini.hoopSizeY = unzoomedRect.y = l_siz.y*1.1;
		hsizmsg();
	}
	dif.x = (unzoomedRect.x - l_siz.x) / 2 - min.x;
	dif.y = (unzoomedRect.y - l_siz.y) / 2 - min.y;
	for (ind = 0; ind < hed.stitchCount; ind++) {

		stitchBuffer[ind].x += dif.x;
		stitchBuffer[ind].y += dif.y;
	}
}

BOOL chkdst(DSTHED* dsthed) {

	if (strncmp(dsthed->desched, "LA:", 3))
		return 0;
	return 1;
}

void auxmen() {

	switch (ini.auxFileType) {

	case AUXDST:

		sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_AUXTXT], "DST");
		CheckMenuItem(hMen, ID_AUXDST, MF_CHECKED);
		CheckMenuItem(hMen, ID_AUXPCS, MF_UNCHECKED);
		break;

	default:

		CheckMenuItem(hMen, ID_AUXDST, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_AUXPCS, MF_CHECKED);
		sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_AUXTXT], "PCS");
	}
	SetMenuItemInfo(hfileMen, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
	setMap(DUMEN);
}

#if PESACT

unsigned tripl(TCHAR* dat) {

#if  __UseASM__
	_asm {

		mov		eax, dat
		mov		eax, [eax]
		and eax, 0xffffff
	}
#else
	return (*(unsigned *)dat) & 0xffffff;
#endif
}

unsigned pesmtch(COLORREF rcol, unsigned char pcol) {

	unsigned	rval, ind;
	COLORREF	tcol;

	tcol = pestrn[pcol];
	rval = 0;
	for (ind = 0; ind < 3; ind++) {

		rval += ((rcol & 0xff) > (tcol & 0xff)) ? ((rcol & 0xff) - (tcol & 0xff)) : ((tcol & 0xff) - (rcol & 0xff));
		tcol >>= 8;
		rcol >>= 8;
	}
	return rval;
}

unsigned dupcol() {

	unsigned	ind;
	COLORREF	col;
	unsigned	mat, matm, pmatm = 0;

	col = pestrn[pesColors[pesColorIndex++]];
	for (ind = 0; ind < xpnt; ind++) {

		if (useCol[ind] == col)
			return ind;
	}
	matm = 0xffffff;
	for (ind = 1; ind < xpnt; ind++) {

		mat = pesmtch(col, pesColors[ind]);
		if (mat < matm) {

			matm = mat;
			pmatm = ind;
		}
	}
	return pmatm;
}


double dubl(unsigned char* pnt) {

#if  __UseASM__
	unsigned tdat;

	_asm {

		mov		ecx, pnt
		mov		ecx, [ecx]
		movzx	ebx, ch
		test	cl, 8
		je		short dubl1
		mov		ch, 15
		and cl, ch
		sub		ch, cl
		movzx	ecx, ch
		shl		ecx, 8
		mov		eax, 256
		sub		eax, ebx
		add		ecx, eax
		neg		ecx
		jmp		short dubl2
		dubl1 : and		ecx, 0x7
				shl		ecx, 8
				add		ecx, ebx
				dubl2 : mov		tdat, ecx
						fild	tdat
	}
#else
	unsigned tdat;

	_asm {

		mov		ecx, pnt
		mov		ecx, [ecx]
		movzx	ebx, ch
		test	cl, 8
		je		short dubl1
		mov		ch, 15
		and cl, ch
		sub		ch, cl
		movzx	ecx, ch
		shl		ecx, 8
		mov		eax, 256
		sub		eax, ebx
		add		ecx, eax
		neg		ecx
		jmp		short dubl2
		dubl1 : and		ecx, 0x7
				shl		ecx, 8
				add		ecx, ebx
				dubl2 : mov		tdat, ecx
						fild	tdat
	}
#endif
}
#endif

void xofrm() {

	unsigned	ind;
	FRMHEDO*	frmlstx;

	frmlstx = (FRMHEDO*)&bseq;
	FillMemory(&bseq, 0, sizeof(FRMHED)*formIndex);
	for (ind = 0; ind < formIndex; ind++)
		MoveMemory(&formList[ind], &frmlstx[ind], sizeof(FRMHEDO));
}

void nuFil() {

	unsigned		l_siz, l_stind;
	unsigned		vervar;
	DWORD			sizh;
	unsigned		ind, inf, ing, inh, tcol;
	unsigned		l_cPnt;
	TCHAR*			pext;
	TCHAR			tchr;
	STRHED			sthed;
	TCHAR			buf[3];
	TCHAR*			tnam;
	DSTHED			dsthed;
	fRECTANGLE			strct;
	FRMHEDO*		frmlstx;
	long			tred;

#if PESACT

	unsigned		ine;
	PESHED*			peshed;
	TCHAR*			l_peschr;
	unsigned		pecof;
	unsigned char*	pcolcnt;
	dPOINT			loc;
	PESTCH*			pabstch;
	unsigned		pabind;
	double			locof;
#endif

	if (rstMap(REDOLD) || GetOpenFileName(&ofn)) {

		fnamtabs();
		untrace();
		if (formIndex)
			delfrms();
		rstMap(ZUMED);
		rstMap(FRMOF);
		rstMap(HID);
		rstMap(BIGBOX);
		unthum();
		unbsho();
		if (bnam[0]) {

			DeleteObject(hBmp);
			ReleaseDC(hWnd, bitmapDC);
			*bnam = 0;
		}
		hFil = CreateFile(filnam, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hFil == INVALID_HANDLE_VALUE) {

			ind = GetLastError();
			if (GetLastError() == 32)
				filnopn(IDS_FNOPNA, filnam);
			else
				filnopn(IDS_FNOPN, filnam);
			hFil = 0;
		} else {

			rstMap(CMPDO);
			rstMap(SAVACT);
			rstMap(BAKING);
			rstMap(REDUSHO);
			txad = 0;
			EnableMenuItem(hMen, M_REDO, MF_BYPOSITION | MF_GRAYED);
			deldu();
			strcpy_s(fildes, ini.designerName);
			unbsho();
			rstMap(MOVSET);
			frmon();
			selectedFormCount = 0;
			if (rstMap(PRFACT)) {

				DestroyWindow(hPrf);
				rstMap(WASRT);
				preferenceIndex = 0;
			}
			bnam[0] = 0;
			if (slpnt)
				slpnt = 0;
			rstMap(SCROS);
			rstMap(ECROS);
			rstMap(BZUMIN);
			rstdu();
			unmsg();
			undoBufferWriteIndex = 0;
			rstMap(BAKWRAP);
			zoomFactor = 1;
			setMap(RESTCH);
			defNam(filnam);
			selCnt = 0;
			if (rstMap(WASPAT))
				DestroyWindow(hSped);
			ind = 0;
			if (chkMap(WASTXBAK))
				ind = 1;
			clrMap(MAPLEN);
			if (ind)
				setMap(WASTXBAK);
			l_siz = GetFileSize(hFil, &sizh);
			pext = strrchr(filnam, '.');
			if (pext)
				pext++;
			else {

				strcat_s(filnam, ".thr");
				pext = strrchr(filnam, '.') + 1;
			}
			tchr = tolower(pext[0]);
			if (tchr == 't') {

				ReadFile(hFil, (STRHED*)&sthed, sizeof(STRHED), &red, NULL);
				if ((sthed.headerType & 0xffffff) == 0x746872) {

					if (red != sizeof(STRHED)) {

						tabmsg(IDS_SHRTF);
						return;
					}
					vervar = (sthed.headerType & 0xff000000) >> 24;
					switch (vervar) {

					case 0:

						if (hed.hoopType == SMALHUP) {

							unzoomedRect.x = ini.hoopSizeX = SHUPX;
							unzoomedRect.y = ini.hoopSizeY = SHUPY;
						} else {

							unzoomedRect.x = ini.hoopSizeX = LHUPX;
							unzoomedRect.y = ini.hoopSizeY = LHUPY;
							hed.hoopType = LARGHUP;
						}
						ritfnam(ini.designerName);
						strcpy_s(fildes, ini.designerName);
						strcpy_s(hedx.modifierName, ini.designerName);
						break;

					case 1:
					case 2:

						ReadFile(hFil, (STREX*)&hedx, sizeof(STREX), &red, NULL);
						if (red != sizeof(STREX)) {

							tabmsg(IDS_SHRTF);
							return;
						}
						ini.hoopSizeX = unzoomedRect.x = hedx.hoopSizeX;
						ini.hoopSizeY = unzoomedRect.y = hedx.hoopSizeY;
						redfnam(fildes);
						break;

					default:

						tabmsg(IDS_NOTVER);
						return;
					}
					zoomRect.bottom = zoomRect.left = 0;
					zoomRect.right = unzoomedRect.x = ini.hoopSizeX;
					zoomRect.top = unzoomedRect.y = ini.hoopSizeY;
					hed.stitchCount = sthed.stitchCount;
					ReadFile(hFil, (fPOINTATTRIBUTE*)stitchBuffer, hed.stitchCount * sizeof(fPOINTATTRIBUTE), &red, NULL);
					if (red != hed.stitchCount * sizeof(fPOINTATTRIBUTE)) {

						hed.stitchCount = red / sizeof(fPOINTATTRIBUTE);
						prtred();
						return;
					}
					ReadFile(hFil, (TCHAR*)bnam, 16, &red, 0);
					tred = red;
					if (red != 16) {

						bnam[0] = 0;
						prtred();
						return;
					}
					ReadFile(hFil, (COLORREF*)&backgroundColor, 4, &red, 0);
					tred += red;
					if (red != 4) {

						backgroundColor = ini.backgroundColor;
						prtred();
						return;
					}
					hBackgroundBrush = CreateSolidBrush(backgroundColor);
					ReadFile(hFil, (COLORREF*)useCol, 64, &red, 0);
					tred += red;
					if (red != 64) {

						prtred();
						return;
					}
					ReadFile(hFil, (COLORREF*)custCol, 64, &red, 0);
					tred += red;
					if (red != 64) {

						prtred();
						return;
					}
					ReadFile(hFil, (TCHAR*)msgbuf, 16, &red, 0);
					tred += red;
					if (red != 16) {

						prtred();
						return;
					}
					for (ind = 0; ind < 16; ind++)
						thrdSiz[ind][0] = msgbuf[ind];
					formIndex = sthed.formCount;
					if (formIndex > MAXFORMS)
						formIndex = MAXFORMS;
					inf = 0; ing = 0, inh = 0;
					if (formIndex) {

						ind = fltad = satkad = clpad = 0;
						msgbuf[0] = 0;
						if (vervar < 2) {

							frmlstx = (FRMHEDO*)&bseq;
							ReadFile(hFil, (FRMHEDO*)frmlstx, formIndex * sizeof(FRMHEDO), &red, 0);
							if (red != formIndex * sizeof(FRMHEDO)) {

								formIndex = red / sizeof(FRMHEDO);
								setMap(BADFIL);
							}
							xofrm();
						} else {

							ReadFile(hFil, (FRMHED*)formList, formIndex * sizeof(FRMHED), &red, 0);
							rstMap(BADFIL);
							if (red != formIndex * sizeof(FRMHED)) {

								formIndex = red / sizeof(FRMHED);
								setMap(BADFIL);
							}
						}
						//						ind=SetFilePointer(hFil,0,0,FILE_CURRENT);  //bug
						ReadFile(hFil, (fPOINT*)formPoints, sthed.pointCount * sizeof(fPOINT), &red, 0);
						if (red != sizeof(fPOINT)*sthed.pointCount) {

							fltad = red / sizeof(fPOINT);
							for (ind = fltad; ind < sthed.pointCount; ind++)
								formPoints[ind].x = formPoints[ind].y = 0;
							setMap(BADFIL);
						}
						ReadFile(hFil, (SATCON*)satks, sthed.dlineCount * sizeof(SATCON), &red, 0);
						if (red != sthed.dlineCount * sizeof(SATCON)) {

							satkad = red / sizeof(SATCON);
							setMap(BADFIL);
						}
						ReadFile(hFil, (fPOINT*)clipboardPoints, sthed.clipboardDataCount * sizeof(fPOINT), &red, 0);
						if (red != sthed.clipboardDataCount * sizeof(fPOINT)) {

							clpad = red / sizeof(fPOINT);
							setMap(BADFIL);
						}
						ReadFile(hFil, (TXPNT*)txpnts, hedx.texturePointCount * sizeof(TXPNT), &red, 0);
						txad = red / sizeof(TXPNT);
						if (rstMap(BADFIL))
							bfilmsg();
						for (ind = 0; ind < formIndex; ind++) {

							formList[ind].vertices = adflt(formList[ind].sides);
							if (formList[ind].type == SAT) {

								if (formList[ind].satinGuideCount)
									formList[ind].satinOrAngle.sac = adsatk(formList[ind].satinGuideCount);
							}
							if (isclp(ind))
								formList[ind].angleOrClipData.clip = adclp(formList[ind].lengthOrCount.clipCount);
							if (iseclpx(ind))
								formList[ind].borderClipData = adclp(formList[ind].clipEntries);
						}
						setfchk();
					}
				} else
					tabmsg(IDS_NOTHR);
			} else {

				setMap(NOTHRFIL);
				if (tchr == 'p') {

					if (tolower(pext[01]) == 'c') {

						ReadFile(hFil, (PCSHEADER*)&hed, 0x46, &red, NULL);
						if (!l_siz) {

							filnopn(IDS_ZEROL, filnam);
							return;
						}
						if (hed.leadIn == 0x32 && hed.colorCount == 16) {

							for (ind = 0; ind < 16; ind++)
								useCol[ind] = hed.colors[ind];
							l_siz -= 0x46;
							inf = l_siz / sizeof(PCSTCH) + 2;
							filBuf = new PCSTCH[inf];
							ReadFile(hFil, filBuf, l_siz, &red, NULL);
							l_stind = 0;
							l_cPnt = 0;
							tcol = 0;
							ind = 0;
							while (l_stind < hed.stitchCount&&ind < inf) {

								if (filBuf[ind].typ == 3) {

									colch[l_cPnt].colorIndex = filBuf[ind].fx;
									colch[l_cPnt++].stitchIndex = l_stind;
									tcol = NOTFRM | filBuf[ind++].fx;
								} else {

									stitchBuffer[l_stind].x = filBuf[ind].x + (float)filBuf[ind].fx / 256;
									stitchBuffer[l_stind].y = filBuf[ind].y + (float)filBuf[ind].fy / 256;
									stitchBuffer[l_stind++].attribute = tcol;
									ind++;
								}
							}
							hed.stitchCount = l_stind;
							tnam = (TCHAR*)&filBuf[ind];
							strcpy_s(bnam, tnam);
							delete[] filBuf;
							strcpy_s(pext, sizeof(filnam) - (pext - filnam), "thr");
							ini.auxFileType = AUXPCS;
							if (hed.hoopType != LARGHUP&&hed.hoopType != SMALHUP)
								hed.hoopType = LARGHUP;
							sizstch(&strct);
							if (strct.left<0 || strct.right>LHUPY || strct.bottom<0 || strct.top>LHUPY) {

								ini.hoopSizeX = LHUPX;
								ini.hoopSizeY = LHUPY;
								chkhup();
							} else {

								if (hed.hoopType == LARGHUP) {

									ini.hoopType = LARGHUP;
									ini.hoopSizeX = LHUPX;
									ini.hoopSizeY = LHUPY;
								} else {

									if (strct.right > SHUPX || strct.top > SHUPY || hed.hoopType == LARGHUP) {

										ini.hoopType = LARGHUP;
										ini.hoopSizeX = SHUPX;
										ini.hoopSizeY = SHUPY;
									} else {

										ini.hoopType = SMALHUP;
										ini.hoopSizeX = SHUPX;
										ini.hoopSizeY = SHUPY;
									}
								}
							}
						}
					}
#if PESACT
					else {

						ReadFile(hFil, (BSEQPNT*)&bseq, sizeof(bseq), &red, 0);
						peshed = (PESHED*)&bseq;
						l_peschr = (TCHAR*)&bseq;
						if (strncmp(peshed->led, "#PES00", 6)) {

							sprintf_s(msgbuf, sizeof(msgbuf), "Not a PES file: %s\n", filnam);
							shoMsg(msgbuf);
							return;
						}
						pecof = tripl(peshed->off);
						pestch = (unsigned char*)&l_peschr[pecof + 532];
						xpnt = 0;
						pcolcnt = (unsigned char*)&l_peschr[pecof + 48];
						pesColors = &pcolcnt[1];
						markedStitchMap[0] = markedStitchMap[1] = 0;
						xpnt = 0;
						for (ind = 0; ind < (unsigned)(*pcolcnt + 1); ind++) {

							if (setRmp(pesColors[ind])) {

								useCol[xpnt++] = pestrn[pesColors[ind] & PESCMSK];
								if (xpnt >= 16)
									break;
							}
						}
						tcol = 0;
						pesColorIndex = 1;
						loc.x = loc.y = 0;
						pabind = ind = ine = 0;
						rstMap(FILDIR);
						ind = 0;
						pabstch = (PESTCH*)&l_peschr[sizeof(PESHED) + 4];
						ind = 0;
						ine = 1;
						stitchBuffer[0].x = stitchBuffer[0].y;
						while (ind < red - pecof - 529) {

							if (pestch[ind] == 0xff && pestch[ind + 1] == 0)
								break;
							if (pestch[ind] == 0xfe && pestch[ind + 1] == 0xb0) {

								tcol = dupcol();
								ind += 2;
							} else {

								if (pestch[ind] & 0x80) {

									locof = dubl(&pestch[ind]);
									ind++;
								} else {

									if (pestch[ind] & 0x40)
										locof = pestch[ind] - 128;
									else
										locof = pestch[ind];
								}
								locof *= 0.6;
								if (toglMap(FILDIR)) {

									loc.y -= locof;
									stitchBuffer[ine].x = loc.x;
									stitchBuffer[ine].y = loc.y;
									stitchBuffer[ine].attribute = tcol;
									ine++;
								} else
									loc.x += locof;
							}
							ind++;
						}
						hed.stitchCount = ine;
						//ini.auxFileType=AUXPES;
						hupfn();
					}
#endif
				} else {

					ReadFile(hFil, (DSTHED*)&dsthed, sizeof(DSTHED), &red, 0);
					if (red == sizeof(DSTHED)) {

						if (chkdst(&dsthed)) {

							bnam[0] = 0;
							l_siz = GetFileSize(hFil, &red) - sizeof(DSTHED);
							dstcnt = l_siz / sizeof(DSTREC);
							drecs = new DSTREC[dstcnt];
							ReadFile(hFil, (DSTREC*)drecs, sizeof(DSTREC)*dstcnt, &red, 0);
							dstran();
							delete[] drecs;
							ini.auxFileType = AUXDST;
						}
					} else {

						tabmsg(IDS_DST2S);
						CloseHandle(hFil);
						return;
					}
				}
			}
			if (bnam[0]) {

				SetCurrentDirectory(defDir);
				strcpy_s(lbnam, bnam);
				bfil();
			}
			ritot(hed.stitchCount);
			bufferIndex = 0;
			zoomRect.left = 0;
			zoomRect.bottom = 0;
			zoomRect.right = unzoomedRect.x = ini.hoopSizeX;
			zoomRect.top = unzoomedRect.y = ini.hoopSizeY;
			movStch();
			coltab();
			rstMap(ZUMED);
			buf[2] = 0;
			for (ind = 0; ind < 16; ind++) {

				userPen[ind] = nuPen(userPen[ind], 1, useCol[ind]);
				hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], useCol[ind]);
				strncpy_s(buf, thrdSiz[ind], 2);
				SetWindowText(hSiz[ind], buf);
			}
			for (ind = 0; ind < 16; ind++)
				redraw(hCol[ind]);
			redraw(hBar);
			if (hed.stitchCount)
				setMap(INIT);
			nuAct(0);
			setMap(RESTCH);
			nunams();
			ritini();
			butxt(HNUM, "");
			if (hed.stitchCount)
				nuAct(stitchBuffer[0].attribute&COLMSK);
			else
				nuAct(0);
			auxmen();
		}
		lenCalc();
		sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRDBY], filnam, fildes);
		SetWindowText(hWnd, msgbuf);
		CloseHandle(hFil);
		setMap(INIT);
		rstMap(TRSET);
		if (chkMap(NOTHRFIL))
		{
			for (ind = 0; ind < hed.stitchCount; ind++)
				stitchBuffer[ind].attribute |= NOTFRM;
		}
		lodchk();
	}
}

void clrfbuf(unsigned p_siz) {

#if  __UseASM__
	_asm {

		mov		edi, filBuf
		mov		ecx, p_siz
		xor		eax, eax
		rep		stosd
	}
#else
	memset(filBuf, 0, p_siz * 4);
#endif
}

unsigned xbits(unsigned src, unsigned dst) {

#if  __UseASM__
	_asm {

		mov		eax, trinum
		mov		ecx, src
		shr		eax, cl
		and		eax, 3
		mov		ecx, dst
		shl		eax, cl
	}
#else
	//check translation
	unsigned bits = trinum;

	bits >>= src & 0xFF;
	bits &= 3;

	bits <<= dst & 0xFF;

	return bits;
#endif
}

unsigned dudbits(POINT dif) {

	return xdst[dif.x + 121] | ydst[dif.y + 121];
}

void savdst(unsigned dat) {

#if  __UseASM__
	_asm {
		xor		eax, eax
		mov		al, 3
		mov		ecx, dstcnt
		mul		ecx
		inc		ecx
		mov		dstcnt, ecx
		add		eax, drecs
		mov		ebx, dat
		mov[eax], ebx
	}
#else
	union {
		unsigned dat;
		DSTREC drec;
	} x;

	x.dat = dat;

	drecs[dstcnt++] = x.drec;
#endif
}

void ritdst() {

#define DSTMAX 121

	unsigned		ind, styp, col, cnt, colind;
	POINT			loc, len, alen, dif, stp;
	fRECTANGLE			trct;
	fPOINTATTRIBUTE*		histch;
	unsigned*		pcol;
	HANDLE			hcol;
	unsigned long	wrot;

	pcol = (unsigned*)&oseq;
	colind = 3;
	pcol[0] = COLVER;
	pcol[1] = backgroundColor;
	pcol[2] = useCol[stitchBuffer[0].attribute&COLMSK];
	histch = &stitchBuffer[MAXSEQ];
	for (ind = 0; ind < hed.stitchCount; ind++) {

		histch[ind].x = rotatedStitches[ind].x * 5 / 3;
		histch[ind].y = rotatedStitches[ind].y * 5 / 3;
		histch[ind].attribute = rotatedStitches[ind].attribute;
	}
	trct.left = trct.right = histch[0].x;
	trct.bottom = trct.top = histch[0].y;
	for (ind = 1; ind < hed.stitchCount; ind++) {

		if (histch[ind].x > trct.right)
			trct.right = histch[ind].x + 0.5;
		if (histch[ind].x < trct.left)
			trct.left = histch[ind].x - 0.5;
		if (histch[ind].y > trct.top)
			trct.top = histch[ind].y + 0.5;
		if (histch[ind].y < trct.bottom)
			trct.bottom = histch[ind].y - 0.5;
	}
	drecs = (DSTREC*)&bseq;
	dstcnt = 0;
	loc.x = (trct.right - trct.left) / 2 + trct.left;
	loc.y = (trct.top - trct.bottom) / 2 + trct.bottom;
	dstplus.x = trct.right - loc.x + 1;
	dstplus.y = trct.top - loc.y + 1;
	dstmin.x = loc.x - trct.left - 1;
	dstmin.y = loc.y - trct.bottom - 1;
	col = histch[0].attribute & 0xf;
	for (ind = 0; ind < hed.stitchCount; ind++) {

		if (col != (histch[ind].attribute & 0xf)) {

			savdst(0xc30000);
			col = histch[ind].attribute & 0xf;
			pcol[colind++] = useCol[col];
		}
		len.x = histch[ind].x - loc.x;
		len.y = histch[ind].y - loc.y;
		alen.x = abs(len.x);
		alen.y = abs(len.y);
		if (abs(alen.x) > abs(alen.y))
			cnt = alen.x / DSTMAX + 1;
		else
			cnt = alen.y / DSTMAX + 1;
		stp.x = alen.x / cnt + 1;
		stp.y = alen.y / cnt + 1;
		while (len.x || len.y) {

			styp = REGTYP;
			if (abs(len.x) > stp.x) {

				styp = JMPTYP;
				if (len.x > 0)
					dif.x = stp.x;
				else
					dif.x = -stp.x;
			} else
				dif.x = len.x;
			if (abs(len.y) > stp.y) {

				styp = JMPTYP;
				if (len.y > 0)
					dif.y = stp.y;
				else
					dif.y = -stp.y;
			} else
				dif.y = len.y;
			savdst(dudbits(dif) | styp);
			loc.x += dif.x;
			loc.y += dif.y;
			len.x -= dif.x;
			len.y -= dif.y;
		}
	}
	drecs[dstcnt].led = drecs[dstcnt].mid = 0;
	drecs[dstcnt++].nd = (TBYTE)0xf3;

	if (colfil()) {

		hcol = CreateFile(colorFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (hcol != INVALID_HANDLE_VALUE)
			WriteFile(hcol, &oseq, colind << 2, &wrot, 0);
		CloseHandle(hcol);
		hcol = CreateFile(rgbFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (hcol != INVALID_HANDLE_VALUE)
			WriteFile(hcol, &pcol[2], (colind - 2) << 2, &wrot, 0);
		CloseHandle(hcol);
	}
}

BOOL pcshup() {

	fRECTANGLE		trct;
	fPOINT		l_siz;
	unsigned	ind;
	fPOINT		hupsiz;
	fPOINT		dif;

	trct.left = trct.right = rotatedStitches[0].x;
	trct.bottom = trct.top = rotatedStitches[0].y;
	for (ind = 1; ind < hed.stitchCount; ind++) {

		if (rotatedStitches[ind].x < trct.left)
			trct.left = rotatedStitches[ind].x;
		if (rotatedStitches[ind].x > trct.right)
			trct.right = rotatedStitches[ind].x;
		if (rotatedStitches[ind].y < trct.bottom)
			trct.bottom = rotatedStitches[ind].y;
		if (rotatedStitches[ind].y > trct.top)
			trct.top = rotatedStitches[ind].y;
	}
	l_siz.x = trct.right - trct.left;
	l_siz.y = trct.top - trct.bottom;
	if (l_siz.x > LHUPX || l_siz.y > LHUPY) {

		tabmsg(IDS_PFAF2L);
		return 1;
	}
	if (l_siz.x > SHUPX || l_siz.y > SHUPY) {

		hed.hoopType = LARGHUP;
		hupsiz.x = LHUPX;
		hupsiz.y = LHUPY;
	} else {

		if (ini.hoopSizeX == LHUPX&&ini.hoopSizeY == LHUPY) {

			hed.hoopType = LARGHUP;
			hupsiz.x = LHUPX;
			hupsiz.y = LHUPY;
		} else {

			hed.hoopType = SMALHUP;
			hupsiz.x = SHUPX;
			hupsiz.y = SHUPY;
		}
	}
	dif.x = dif.y = 0;
	if (trct.right > hupsiz.x)
		dif.x = hupsiz.x - trct.right;
	if (trct.top > hupsiz.y)
		dif.y = hupsiz.y - trct.top;
	if (trct.left < 0)
		dif.x = -trct.left;
	if (trct.bottom < 0)
		dif.y = -trct.bottom;
	if (dif.x || dif.y) {

		for (ind = 0; ind < hed.stitchCount; ind++) {

			rotatedStitches[ind].x += dif.x;
			rotatedStitches[ind].y += dif.y;
		}
	}
	return 0;
}

#if PESACT

void ritpes(unsigned p_stind) {

	pesStitches[opnt].x = -rotatedStitches[p_stind].x * 3 / 5 + stitchCenterOffset.x;
	pesStitches[opnt++].y = rotatedStitches[p_stind].y * 3 / 5 - stitchCenterOffset.y;
}

void ritpcol(unsigned char col) {

#if  __UseASM__
	_asm {

		mov		ebx, opnt
		mov		eax, ebx
		inc		eax
		mov		opnt, eax
		shl		ebx, 2
		add		ebx, pesStitches
		xor		eax, eax
		mov		al, col
		mov[ebx], eax
	}
#else
	_asm {

		mov		ebx, opnt
		mov		eax, ebx
		inc		eax
		mov		opnt, eax
		shl		ebx, 2
		add		ebx, pesStitches
		xor		eax, eax
		mov		al, col
		mov[ebx], eax
	}
#endif
}

unsigned pesnam() {

#if  __UseASM__
	_asm {
		mov		ebx, offset auxnam
		mov		ecx, _MAX_PATH
		mov		edx, ebx
		peslup0 : mov		al, [ebx]
				  or al, al
				  je		short peslup1
				  cmp		al, '\\'
				  jne		short peslup0a
				  mov		edx, ebx
				  peslup0a : inc		ebx
							 loop	peslup0
							 peslup1 : mov		ebx, edx
									   cmp		byte ptr[ebx], '\\'
									   jne		short peslup1a
									   inc		ebx
									   peslup1a : xor		ecx, ecx
												  mov		cl, 17
												  mov		edi, offset bseq
												  mov		dword ptr[edi], ':AL'
												  add		edi, 3
												  peslup : mov		al, [ebx]
														   inc		ebx
														   cmp		al, '.'
														   je		pesnamx
														   mov[edi], al
														   inc		edi
														   loop	peslup
														   pesnamx : mov		eax, edi
																	 sub		eax, offset bseq
	}
#else
	_asm {
		mov		ebx, offset auxnam
		mov		ecx, _MAX_PATH
		mov		edx, ebx
		peslup0 : mov		al, [ebx]
				  or al, al
				  je		short peslup1
				  cmp		al, '\\'
				  jne		short peslup0a
				  mov		edx, ebx
				  peslup0a : inc		ebx
							 loop	peslup0
							 peslup1 : mov		ebx, edx
									   cmp		byte ptr[ebx], '\\'
									   jne		short peslup1a
									   inc		ebx
									   peslup1a : xor		ecx, ecx
												  mov		cl, 17
												  mov		edi, offset bseq
												  mov		dword ptr[edi], ':AL'
												  add		edi, 3
												  peslup : mov		al, [ebx]
														   inc		ebx
														   cmp		al, '.'
														   je		pesnamx
														   mov[edi], al
														   inc		edi
														   loop	peslup
														   pesnamx : mov		eax, edi
																	 sub		eax, offset bseq
	}
#endif
}

void rpcrd(float dif) {

	int idif;

	idif = dif * 5 / 3;
	if (idif < 0) {

		if (idif > -64) {

			ptrPesData[opnt] = idif - 128;
			opnt++;
		} else {

			ptrPesData[opnt] = idif >> 8;
			opnt++;
			ptrPesData[opnt] = idif & 0x8f;
			opnt++;
		}
	} else {

		if (idif < 64) {

			ptrPesData[opnt] = idif;
			opnt++;
		} else {

			ptrPesData[opnt] = (idif >> 8) | 0x80;
			opnt++;
			ptrPesData[opnt] = idif & 0xff;
			opnt++;
		}
	}
}

void pecdat() {

	unsigned	ind, col, pcol;

	opnt = 532;
	pcol = 1;
	col = stitchBuffer[0].attribute&COLMSK;
	ptrPesData = (TCHAR*)&bseq;
	pesColors = (unsigned char*)&ptrPesData[49];
	rpcrd(stitchBuffer[0].x);
	rpcrd(-stitchBuffer[0].y);
	for (ind = 0; ind < (unsigned)hed.stitchCount - 1; ind++) {

		if ((stitchBuffer[ind].attribute&COLMSK) != col) {

			col = stitchBuffer[ind].attribute&COLMSK;
			ptrPesData[opnt] = (TBYTE)254;
			opnt++;
			ptrPesData[opnt] = (TBYTE)176;
			opnt++;
			ptrPesData[opnt] = pcol;
			opnt++;
			pesColors[pcol] = pesEquivColors[col];
			pcol++;
		}
		rpcrd(stitchBuffer[ind + 1].x - stitchBuffer[ind].x);
		rpcrd(-stitchBuffer[ind + 1].y + stitchBuffer[ind].y);
	}
	ptrPesData[opnt++] = (TBYTE)0xff;
	ptrPesData[opnt++] = 0;
}

#endif

void chk1col() {

	unsigned ind, ine, col;

	coltab();
	setMap(RESTCH);
	for (ind = 0; ind < colCnt; ind++) {

		if (colch[ind + 1].stitchIndex - colch[ind].stitchIndex == 1) {

			ine = colch[ind].stitchIndex;
			col = stitchBuffer[ine - 1].attribute&COLMSK;
			stitchBuffer[ine].attribute &= NCOLMSK;
			stitchBuffer[ine].attribute |= col;
		}
	}
}

BOOL chkattr(TCHAR* nam) {

	unsigned		attr, ind;
	TCHAR			driv[_MAX_PATH];
	DWORD			sec;
	DWORD			byt;
	DWORD			fclst;
	DWORD			tclst;

	if (rstMap(NOTFREE))
		return 1;
	attr = GetFileAttributes(nam);
	if (attr&FILE_ATTRIBUTE_READONLY&&attr != 0xffffffff) {

		sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_OVRLOK], nam);
		ind = MessageBox(hWnd, msgbuf, stab[STR_OVRIT], MB_YESNO);
		if (ind == IDYES)
			SetFileAttributes(nam, attr&(0xffffffff ^ FILE_ATTRIBUTE_READONLY));
		else
			return 1;
	}
	strcpy_s(driv, homdir);
	driv[3] = 0;
	if (!GetDiskFreeSpace(driv, &sec, &byt, &fclst, &tclst)) {

		setMap(NOTFREE);
		return 1;
	}
	return 0;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void sav() {

	unsigned		ind, l_stind;
	unsigned long	wrot;
	double			frct, intg;
	DSTHED			dsthed;
	TCHAR*			pchr;
	POINT			loc;

#if PESACT

	PESHED			peshed;
	PESTCH*			l_pestch;
	unsigned		match;
	unsigned		mtchind = 0;
	unsigned		mtchmin;
	unsigned char	pescol;
	fRECTANGLE			srct;
	unsigned		tcol, ine;
	unsigned		pcolcnt;
	unsigned*		pesof;
	unsigned*		upnt;
	short*			psiz;
#endif

	duauxnam();
	if (chkattr(auxnam))
		return;
	if (!hed.stitchCount)
		return;
	chk1col();
	coltab();
	rotatedStitches = (fPOINTATTRIBUTE*)&bseq;
	if (chku(ROTAUX)) {

		for (ind = 0; ind < hed.stitchCount; ind++) {

			rotatedStitches[ind].y = -stitchBuffer[ind].x;
			rotatedStitches[ind].x = stitchBuffer[ind].y;
			rotatedStitches[ind].attribute = stitchBuffer[ind].attribute;
		}
	} else {

		for (ind = 0; ind < hed.stitchCount; ind++) {

			rotatedStitches[ind].x = stitchBuffer[ind].x;
			rotatedStitches[ind].y = stitchBuffer[ind].y;
			rotatedStitches[ind].attribute = stitchBuffer[ind].attribute;
		}
	}
	hPcs = CreateFile(auxnam, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hPcs == INVALID_HANDLE_VALUE) {

		crmsg(auxnam);
		hPcs = 0;
	} else {

		switch (ini.auxFileType) {

		case AUXDST:

			ritdst();
			ind = 0; loc.x = loc.y = 0;
			pchr = (TCHAR*)&dsthed;
			for (ind = 0; ind < sizeof(DSTHED); ind++)
				pchr[ind] = ' ';
			strncpy(dsthed.desched, "LA:", 3);
			pchr = strrchr(auxnam, '\\') + 1;
			for (ind = 0; ind < sizeof(dsthed.desc); ind++) {

				if (pchr[ind] && pchr[ind] != '.')
					dsthed.desc[ind] = pchr[ind];
				else
					break;
			}
			dsthed.desc[16] = 0xd;
			strncpy(dsthed.recshed, "ST:", 3);
			sprintf(dsthed.recs, "%7d\r", dstcnt);
			strncpy(dsthed.cohed, "CO:", 3);
			strncpy(dsthed.co, "  0\xd", 4);
			strncpy(dsthed.xplushed, "+X:", 3);
			sprintf(dsthed.xplus, "%5d\xd", dstmin.x);
			strncpy(dsthed.xminhed, "-X:", 3);
			sprintf(dsthed.xmin, "%5d\xd", dstplus.x);
			strncpy(dsthed.yplushed, "+Y:", 3);
			sprintf(dsthed.yplus, "%5d\xd", dstplus.y);
			strncpy(dsthed.yminhed, "-Y:", 3);
			sprintf(dsthed.ymin, "%5d\xd", dstmin.y);
			strncpy(dsthed.axhed, "AX:", 3);
			strncpy(dsthed.ax, "-    0\r", 7);
			strncpy(dsthed.ayhed, "AY:", 3);
			strncpy(dsthed.ay, "+    0\r", 7);
			strncpy(dsthed.mxhed, "MX:", 3);
			strncpy(dsthed.mx, "+    0\r", 7);
			strncpy(dsthed.myhed, "MY:", 3);
			strncpy(dsthed.my, "+    0\r", 7);
			strncpy(dsthed.pdhed, "PD", 2);
			strncpy(dsthed.pd, "******\r", 7);
			strncpy(dsthed.eof, "\x1a", 1);
			WriteFile(hPcs, (DSTHED*)&dsthed, sizeof(DSTHED), &wrot, 0);
			WriteFile(hPcs, (DSTREC*)drecs, sizeof(DSTREC)*dstcnt, &wrot, 0);
			break;

#if PESACT

		case AUXPES:

			pchr = (TCHAR*)&peshed;
			for (ind = 0; ind < sizeof(PESHED); ind++)
				pchr[ind] = 0;
			strncpy(peshed.led, "#PES0001",sizeof(peshed.led));
			strncpy(peshed.ce, "CEmbOne", sizeof(peshed.ce));
			strncpy(peshed.cs, "CSewSeg", sizeof(peshed.cs));
			l_pestch = (PESTCH*)&bseq;
			for (ind = 0; ind < 16; ind++) {

				mtchmin = 0xffffffff;
				for (ine = 0; ine < sizeof(pestrn) >> 2; ine++) {

					match = pesmtch(useCol[ind], ine);
					if (match < mtchmin) {

						mtchind = ine;
						mtchmin = match;
					}
				}
				pesEquivColors[ind] = (unsigned char)mtchind;
			}
			tcol = stitchBuffer[0].attribute&COLMSK;
			pescol = peshed.scol = pesEquivColors[stitchBuffer[0].attribute&COLMSK];
			sizstch(&srct);
			stitchCenterOffset.x = midl(srct.right, srct.left);
			stitchCenterOffset.y = midl(srct.top, srct.bottom);
			peshed.xsiz = srct.right - srct.left;
			peshed.ysiz = srct.top - srct.bottom;
			opnt = 0;
			pesStitches = (PESTCH*)&bseq;
			ritpes(0);
			pesStitches[opnt].x = -32765; // 0x8003
			pesStitches[opnt++].y = 0;
			ritpcol(pesEquivColors[tcol]);
			ritpes(0);
			pcolcnt = 0;
			for (ind = 1; ind < hed.stitchCount; ind++) {

				if (tcol == (stitchBuffer[ind].attribute&COLMSK))
					ritpes(ind);
				else {

					ritpes(ind);
					pesStitches[opnt].x = -32767; // 0x8001
					pesStitches[opnt++].y = 0;
					ritpcol(pesEquivColors[tcol]);
					tcol = stitchBuffer[ind].attribute&COLMSK;
					ritpes(ind++);
					ritpes(ind);
					pesStitches[opnt].x = -32765; // 0x8003
					pesStitches[opnt++].y = 0;
					ritpcol(pesEquivColors[tcol]);
					ritpes(ind);
					pcolcnt++;
				}
			}
			pesStitches[opnt].x = ine;
			pesStitches[opnt++].y = 0;
			pesof = (unsigned*)&peshed.off;
			*pesof = (opnt << 2) + sizeof(PESHED);
			*peshed.m1 = 0x20;
			groupStartStitch = 0;
			groupEndStitch = hed.stitchCount - 1;
			peshed.xsiz = 10000;
			peshed.ysiz = 10000;
			WriteFile(hPcs, (PESHED*)&peshed, sizeof(PESHED), &wrot, 0);
			WriteFile(hPcs, (PESTCH*)&bseq, opnt << 2, &wrot, 0);
			ind = pesnam();
			pchr = (TCHAR*)&bseq;
			while (ind < 512)
				pchr[ind++] = ' ';
			pchr[19] = 13;
			pchr[48] = (TCHAR)pcolcnt;
			pecdat();
			upnt = (unsigned*)&pchr[514];
			*upnt = opnt - 512;
			pchr[517] = 0x20;
			pchr[518] = -1;
			pchr[519] = -17;
			psiz = (short*)&pchr[520];
			*psiz = peshed.xsiz;
			psiz++;
			*psiz = peshed.ysiz;
			psiz++;
			*psiz = 480;
			pesof = (unsigned*)psiz;
			*pesof = 11534816;
			//			pchr[527]=(TCHAR)0x0;
			//			pchr[528]=(TCHAR)0x90;
			//			pchr[529]=(TCHAR)0x0;
			//			pchr[530]=(TCHAR)0x8f;
			pchr[527] = (TBYTE)0x00;
			pchr[528] = (TBYTE)0x80;	//hor	msb
			pchr[529] = (TBYTE)0x80; //hor lsb
			pchr[530] = (TBYTE)0x82; //vert msb
			pchr[531] = (TBYTE)0xff; //vert lsb
			WriteFile(hPcs, (TBYTE*)&bseq, opnt, &wrot, 0);
			break;
#endif
		default:

			for (ind = 0; ind < 16; ind++)
				hed.colors[ind] = useCol[ind];
			if (pcshup())
				return;
			if (!WriteFile(hPcs, &hed, 0x46, &wrot, 0)) {

				riter();
				return;
			}
			filBuf = new PCSTCH[hed.stitchCount + colCnt + 2];
			clrfbuf((sizeof(PCSTCH)*(hed.stitchCount + colCnt + 1)) >> 2);
			l_stind = 0;
			savcol = 0xff;
			for (ind = 0; ind < hed.stitchCount; ind++) {

				if ((rotatedStitches[ind].attribute&COLMSK) != savcol) {

					savcol = rotatedStitches[ind].attribute&COLMSK;
					filBuf[l_stind].typ = 3;
					filBuf[l_stind++].fx = savcol;
				}
				frct = modf(rotatedStitches[ind].x, &intg);
				filBuf[l_stind].fx = frct * 256;
				filBuf[l_stind].x = intg;
				frct = modf(rotatedStitches[ind].y, &intg);
				filBuf[l_stind].fy = frct * 256;
				filBuf[l_stind++].y = intg;
			}
			if (!WriteFile(hPcs, filBuf, l_stind * sizeof(PCSTCH), &wrot, 0)) {

				riter();
				return;
			}
			if (chku(BSAVOF)) {

				*msgbuf = 0;
				if (!WriteFile(hPcs, msgbuf, 15, &wrot, 0)) {

					riter();
					return;
				}
			} else {

				if (!WriteFile(hPcs, bnam, 15, &wrot, 0)) {

					riter();
					return;
				}
			}
			delete[] filBuf;
		}
		defNam(filnam);
		CloseHandle(hPcs);
		if (chku(ROTAUX))
			filnopn(IDS_FILROT, auxnam);
	}
}
#pragma warning(pop)

void savAs() {

	TCHAR*	pchr;

	if (hed.stitchCount || formIndex || *bnam) {

		ofn.nFilterIndex = 0;
		if (GetSaveFileName(&ofn)) {

			_strlwr_s(filnam);
			pchr = strrchr(filnam, '.');
			if (!pchr)
				pchr = &filnam[strlen(filnam)];
			switch (ofn.nFilterIndex) {

			case 1:

				strcpy_s(pchr, sizeof(filnam) - (pchr - filnam), ".thr");
				break;

			case 2:

				strcpy_s(pchr, sizeof(filnam) - (pchr - filnam), ".pcs");
				ini.auxFileType = AUXPCS;
				auxmen();
				break;

			case 3:

				strcpy_s(pchr, sizeof(filnam) - (pchr - filnam), ".dst");
				ini.auxFileType = AUXDST;
				auxmen();
				break;
			}
			setMap(SAVAS);
			nunams();
			ritini();
			rstMap(SAVAS);
			rstMap(CMPDO);
			thrsav();
			sav();
			SetWindowText(hWnd, thrnam);
		}
	}
}

void save() {

	TCHAR*	pchr;
	TCHAR	tchr;

	if (filnam[0]) {

		pchr = strrchr(filnam, '.');
		if (pchr)
			pchr++;
		else {

			strcat_s(filnam, ".thr");
			pchr = strrchr(filnam, '.') + 1;
		}
		tchr = pchr[0] | 0x20;
		thrsav();
		if (hed.stitchCount)
			sav();
	} else
		savAs();
}

COLORREF nuCol(COLORREF init) {

	gCol.Flags = CC_ANYCOLOR | CC_RGBINIT;
	gCol.hwndOwner = hWnd;
	gCol.lCustData = 0;
	gCol.lpCustColors = custCol;
	gCol.lpfnHook = 0;
	gCol.lpTemplateName = 0;
	gCol.rgbResult = init;
	gCol.lStructSize = sizeof(gCol);
	return ChooseColor(&gCol);
}

COLORREF nuBak() {

	bCol.Flags = CC_ANYCOLOR | CC_RGBINIT;
	bCol.hwndOwner = hWnd;
	bCol.lCustData = 0;
	bCol.lpCustColors = bakCust;
	bCol.lpfnHook = 0;
	bCol.lpTemplateName = 0;
	bCol.rgbResult = backgroundColor;
	bCol.lStructSize = sizeof(bCol);
	return ChooseColor(&bCol);
}

COLORREF nuBit() {

	btCol.Flags = CC_ANYCOLOR | CC_RGBINIT;
	btCol.hwndOwner = hWnd;
	btCol.lCustData = 0;
	btCol.lpCustColors = bitmapBackgroundColors;
	btCol.lpfnHook = 0;
	btCol.lpTemplateName = 0;
	btCol.rgbResult = bitmapColor;
	btCol.lStructSize = sizeof(bCol);
	return ChooseColor(&btCol);
}

void pxCor2stch(POINT pnt) {

	double	tdub;

	tdub = (double)(pnt.x - stitchWindowAbsRect.left) / stitchWindowClientRect.right;
	sPnt.x = tdub*(zoomRect.right - zoomRect.left) + zoomRect.left;
	tdub = (double)(stitchWindowAbsRect.bottom - pnt.y) / stitchWindowClientRect.bottom;
	sPnt.y = tdub*(zoomRect.top - zoomRect.bottom) + zoomRect.bottom;
}

unsigned px2stch() {

	double	tdub;

	if (msg.pt.x >= stitchWindowAbsRect.left&&msg.pt.x <= stitchWindowAbsRect.right
		&&msg.pt.y >= stitchWindowAbsRect.top&&msg.pt.y <= stitchWindowAbsRect.bottom) {

		tdub = (double)(msg.pt.x - stitchWindowAbsRect.left) / stitchWindowClientRect.right;
		sPnt.x = tdub*(zoomRect.right - zoomRect.left) + zoomRect.left;
		tdub = (double)(stitchWindowAbsRect.bottom - msg.pt.y) / stitchWindowClientRect.bottom;
		sPnt.y = tdub*(zoomRect.top - zoomRect.bottom) + zoomRect.bottom;
		return 1;
	} else
		return 0;
}

void shft2box() {

	sPnt.x = stitchBuffer[closestPointIndex].x;
	sPnt.y = stitchBuffer[closestPointIndex].y;
	shft(sPnt);
	stch2px1(closestPointIndex);
}

void zumin() {

	dPOINT		nusiz;
	fRECTANGLE		srct;
	fRECTANGLE*		trct;
	unsigned	ind;

	if (!rstMap(ZUMACT))
		zoomFactor *= ZUMFCT;
	if (zoomFactor < zoomMin)
		zoomFactor = zoomMin;
	if (!setMap(ZUMED))
		movStch();
	if (!rstMap(BZUMIN)) {

		if (chkMap(GMRK)) {

			sPnt.x = zoomMarkPoint.x;
			sPnt.y = zoomMarkPoint.y;
			goto gotc;
		}
		if (chkMap(FORMSEL)) {

			trct = &formList[closestFormToCursor].rectangle;
			sPnt.x = ((trct->right - trct->left) / 2) + trct->left;
			sPnt.y = ((trct->top - trct->bottom) / 2) + trct->bottom;
			goto gotc;
		}
		if (chkMap(FRMPSEL)) {

			sPnt.x = formList[closestFormToCursor].vertices[closestVertexToCursor].x;
			sPnt.y = formList[closestFormToCursor].vertices[closestVertexToCursor].y;
			goto gotc;
		}
		if (chkMap(SELBOX)) {

			sPnt.x = stitchBuffer[closestPointIndex].x;
			sPnt.y = stitchBuffer[closestPointIndex].y;
			goto gotc;
		}
		if (chkMap(GRPSEL)) {

			selRct(&srct);
			sPnt.x = ((srct.right - srct.left) / 2) + srct.left;
			sPnt.y = ((srct.top - srct.bottom) / 2) + srct.bottom;
			goto gotc;
		}
		if (chkMap(INSRT)) {

			if (chkMap(LIN1)) {

				if (chkMap(BAKEND)) {

					sPnt.x = stitchBuffer[hed.stitchCount - 1].x;
					sPnt.y = stitchBuffer[hed.stitchCount - 1].y;
				} else {

					sPnt.x = stitchBuffer[0].x;
					sPnt.y = stitchBuffer[0].y;
				}
			} else {

				sPnt.x = (stitchBuffer[closestPointIndex + 1].x - stitchBuffer[closestPointIndex].x) / 2 + stitchBuffer[closestPointIndex].x;
				sPnt.y = (stitchBuffer[closestPointIndex + 1].y - stitchBuffer[closestPointIndex].y) / 2 + stitchBuffer[closestPointIndex].y;
			}
			goto gotc;
		}
		if (selectedFormCount) {

			selectedFormsRectangle.bottom = formList[selectedFormList[0]].rectangle.bottom;
			selectedFormsRectangle.top = formList[selectedFormList[0]].rectangle.top;
			selectedFormsRectangle.left = formList[selectedFormList[0]].rectangle.left;
			selectedFormsRectangle.right = formList[selectedFormList[0]].rectangle.right;
			for (ind = 1; ind < selectedFormCount; ind++) {

				if (formList[selectedFormList[ind]].rectangle.bottom < selectedFormsRectangle.bottom)
					selectedFormsRectangle.bottom = formList[selectedFormList[ind]].rectangle.bottom;
				if (formList[selectedFormList[ind]].rectangle.top > selectedFormsRectangle.top)
					selectedFormsRectangle.top = formList[selectedFormList[ind]].rectangle.top;
				if (formList[selectedFormList[ind]].rectangle.left < selectedFormsRectangle.left)
					selectedFormsRectangle.left = formList[selectedFormList[ind]].rectangle.left;
				if (formList[selectedFormList[ind]].rectangle.right > selectedFormsRectangle.right)
					selectedFormsRectangle.right = formList[selectedFormList[ind]].rectangle.right;
			}
			sPnt.x = (selectedFormsRectangle.right - selectedFormsRectangle.left) / 2 + selectedFormsRectangle.left;
			sPnt.y = (selectedFormsRectangle.top - selectedFormsRectangle.bottom) / 2 + selectedFormsRectangle.bottom;
			goto gotc;
		}
		if (!px2stch())
			centr();
	}
gotc:;
	nusiz.x = unzoomedRect.x*zoomFactor;
	nusiz.y = nusiz.x / stitchWindowAspectRatio;
	zoomRect.left = zoomRect.bottom = 0;
	zoomRect.right = nusiz.x;
	zoomRect.top = nusiz.y;
	shft(sPnt);
	selCnt = 0;
	if (!chkMap(GMRK) && chkMap(SELBOX))
		shft2box();
	if (chkMap(RUNPAT)) {

		FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
		runPoint = 0;
	}
	duzrat();
	movins();
	setMap(RESTCH);
	ilin();
}

void zumhom() {

	zoomRect.bottom = 0;
	zoomRect.left = 0;
	zoomRect.right = unzoomedRect.x;
	zoomRect.top = unzoomedRect.y;
	zoomFactor = 1;
	rstMap(ZUMED);
	movStch();
	selCnt = 0;
	if (chkMap(RUNPAT)) {

		FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
		runPoint = 0;
	}
	setMap(RESTCH);
	if (chkMap(SELBOX))
		shft2box();
	duzrat();
	movins();
}

void zumshft() {

	POINT pnt;
	dPOINT spnt;

	if (chkMap(ZUMED)) {

		unboxs();
		if (px2stch()) {
			selCnt = 0;
			shft(sPnt);
			spnt.x = sPnt.x;
			spnt.y = sPnt.y;
			sCor2px(spnt, &pnt);
			if (chkMap(RUNPAT)) {

				FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
				runPoint = 0;
			}
			setMap(RESTCH);
		}
	}
	movins();
}

void zumout() {

	POINT	nusiz;
	fRECTANGLE	srct;
	fRECTANGLE*	trct;

	unboxs();
	if (chkMap(ZUMED)) {

		if (chkMap(GMRK)) {

			sPnt.x = zoomMarkPoint.x;
			sPnt.y = zoomMarkPoint.y;
			goto gots;
		}
		if (chkMap(FORMSEL)) {

			trct = &formList[closestFormToCursor].rectangle;
			sPnt.x = ((trct->right - trct->left) / 2) + trct->left;
			sPnt.y = ((trct->top - trct->bottom) / 2) + trct->bottom;
			goto gots;
		}
		if (chkMap(FRMPSEL)) {

			sPnt.x = formList[closestFormToCursor].vertices[closestVertexToCursor].x;
			sPnt.y = formList[closestFormToCursor].vertices[closestVertexToCursor].y;
			goto gots;
		}
		if (chkMap(SELBOX) || chkMap(INSRT)) {

			sPnt.x = stitchBuffer[closestPointIndex].x;
			sPnt.y = stitchBuffer[closestPointIndex].y;
			goto gots;
		}
		if (chkMap(GRPSEL)) {

			selRct(&srct);
			sPnt.x = ((srct.right - srct.left) / 2) + srct.left;
			sPnt.y = ((srct.top - srct.bottom) / 2) + srct.bottom;
			goto gots;
		}
		if (chkMap(SELBOX)) {

			shft2box();
			goto gots;
		}
		if (!px2stch())
			centr();
	gots:;
		zoomFactor /= ZUMFCT;
		if (zoomFactor > 0.98) {

			zoomFactor = 1;
			rstMap(ZUMED);
			zoomRect.bottom = 0;
			zoomRect.left = 0;
			zoomRect.right = unzoomedRect.x;
			zoomRect.top = unzoomedRect.y;
			movStch();
			selCnt = 0;
		} else {

			nusiz.x = unzoomedRect.x*zoomFactor;
			nusiz.y = nusiz.x / stitchWindowAspectRatio;
			zoomRect.left = zoomRect.bottom = 0;
			zoomRect.top = nusiz.y;
			zoomRect.right = nusiz.x;
			shft(sPnt);
		}
		if (chkMap(RUNPAT)) {

			FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
			runPoint = 0;
		}
		setMap(RESTCH);
		duzrat();
		movins();
	}
}

void duClos(unsigned strt, unsigned cnt) {

	unsigned	ind, ine, tind0, tind1;
	double		sum, tsum;
	long		cx, cy;

	for (ind = strt; ind < strt + cnt; ind++) {

		cx = ((stitchBuffer[ind].x > sPnt.x) ? (stitchBuffer[ind].x - sPnt.x) : (sPnt.x - stitchBuffer[ind].x));
		cy = ((stitchBuffer[ind].y > sPnt.y) ? (stitchBuffer[ind].y - sPnt.y) : (sPnt.y - stitchBuffer[ind].y));
		sum = hypot(cx, cy);
		tind0 = ind;
		for (ine = 0; ine < NERCNT; ine++) {

			if (sum < ner[ine]) {

				tsum = ner[ine];
				tind1 = nerInd[ine];
				ner[ine] = sum;
				nerInd[ine] = tind0;
				sum = tsum;
				tind0 = tind1;
			}
		}
	}
}

void closPnt() {

	unsigned	ind, sind0, sind1;

	unbox();
	unboxs();
	for (ind = 0; ind < 4; ind++) {

		ner[ind] = 1e99;
		nerInd[ind] = -1;
	}
	px2stch();
	for (ind = 0; ind < colCnt; ind++) {

		sind0 = colch[ind].stitchIndex;
		sind1 = colch[ind + 1].stitchIndex;
		if (chkMap(HID)) {

			if (colch[ind].colorIndex == actcol)
				duClos(sind0, sind1 - sind0);
		} else
			duClos(sind0, sind1 - sind0);
	}
	GetClientRect(hStch, &stitchWindowClientRect);
	for (ind = 0; ind < NERCNT; ind++) {

		if (stch2px(nerInd[ind])) {

			nerInd[selCnt] = nerInd[ind];
			pxNer[selCnt].x = stitchSizePixels.x;
			pxNer[selCnt++].y = stitchSizePixels.y;
		}
	}
	boxs();
}

unsigned closPnt1(unsigned* clo) {

	unsigned	ind, ine, tclo = 0;
	unsigned	layr;
	long		nerSid;
	double		cx, cy;
	POINT		chkPnt;
	double		tsum = 1e99;

	chkPnt.x = msg.pt.x - stitchWindowOrigin.x;
	chkPnt.y = msg.pt.y - stitchWindowOrigin.y;
	if (chkMap(SELBOX) && stch2px(closestPointIndex)) {

		if (hypot(stitchSizePixels.x - chkPnt.x, stitchSizePixels.y - chkPnt.y) < CLOSENUF) {

			*clo = closestPointIndex;
			return 1;
		}
	}
	for (ind = 0; ind < selCnt; ind++) {

		nerSid = boxOff[ind];
		if (chkPnt.x >= pxNer[ind].x - nerSid&&
			chkPnt.x <= pxNer[ind].x + nerSid&&
			chkPnt.y >= pxNer[ind].y - nerSid&&
			chkPnt.y <= pxNer[ind].y + nerSid) {

			*clo = nerInd[ind];
			return 1;
		}
	}
	px2stch();
	distanceToClick = 1e99;
	if (chkMap(HID)) {

		for (ind = 0; ind < colCnt; ind++) {

			if (colch[ind].colorIndex == actcol) {

				for (ine = colch[ind].stitchIndex; ine < colch[ind + 1].stitchIndex; ine++) {

					if (stitchBuffer[ine].x >= zoomRect.left&&
						stitchBuffer[ine].x <= zoomRect.right&&
						stitchBuffer[ine].y >= zoomRect.bottom&&
						stitchBuffer[ine].y <= zoomRect.top) {

						cx = ((stitchBuffer[ine].x > sPnt.x) ? (stitchBuffer[ine].x - sPnt.x) : (sPnt.x - stitchBuffer[ine].x));
						cy = ((stitchBuffer[ine].y > sPnt.y) ? (stitchBuffer[ine].y - sPnt.y) : (sPnt.y - stitchBuffer[ine].y));
						tsum = hypot(cx, cy);
						if (tsum < distanceToClick) {

							distanceToClick = tsum;
							tclo = ine;
						}
					}
				}
			}
		}
	} else {

		for (ind = 0; ind < hed.stitchCount; ind++) {

			layr = (stitchBuffer[ind].attribute&LAYMSK) >> LAYSHFT;
			if (!activeLayer || !layr || layr == activeLayer) {

				if (stitchBuffer[ind].x >= zoomRect.left&&
					stitchBuffer[ind].x <= zoomRect.right&&
					stitchBuffer[ind].y >= zoomRect.bottom&&
					stitchBuffer[ind].y <= zoomRect.top) {

					cx = stitchBuffer[ind].x - sPnt.x;
					cy = stitchBuffer[ind].y - sPnt.y;
					tsum = hypot(cx, cy);
					if (tsum < distanceToClick) {

						distanceToClick = tsum;
						tclo = ind;
					}
				}
			}
		}
	}
	if (distanceToClick == 1e99)
		return 0;
	else {

		stch2px(tclo);
		if (chkMap(IGNTHR)) {

			*clo = tclo;
			return 1;
		} else {

			if (hypot(chkPnt.x - stitchSizePixels.x, chkPnt.y - stitchSizePixels.y) < CLOSENUF) {

				*clo = tclo;
				return 1;
			} else
				return 0;
		}
	}
}

unsigned pt2colInd(unsigned pInd) {

	unsigned ind;

	for (ind = 0; ind < colCnt; ind++) {

		if (colch[ind].stitchIndex > pInd)
			break;
	}
	return ind;
}

void unthum() {

	unsigned ind;

	if (rstMap(THUMSHO)) {

		for (ind = 0; ind < 4; ind++)
			DestroyWindow(hvu[ind]);
		if (chkMap(UPTO))
			butxt(HUPTO, stab[STR_UPON]);
		else
			butxt(HUPTO, stab[STR_UPOF]);
		butxt(HNUM, "");
		redraw(hbuts[HHID]);
		butxt(HBOXSEL, stab[STR_BOXSEL]);
	}
}

void toglup() {

	if (toglMap(UPTO))
		butxt(HUPTO, stab[STR_UPOF]);
	else {

		if (rstMap(GRPSEL)) {

			rngadj();
			closestPointIndex = groupStartStitch;
			setMap(SELBOX);
			rstMap(FRMPSEL);
		} else {

			if (!chkMap(SELBOX)) {

				closestPointIndex = 0;
				if (rstMap(FORMSEL)) {

					while (closestPointIndex < hed.stitchCount&&notfstch(stitchBuffer[closestPointIndex].attribute))
						closestPointIndex++;
					setMap(SELBOX);
					rstMap(FRMPSEL);
				}
			}
		}
		butxt(HUPTO, stab[STR_UPON]);
	}
	setMap(RESTCH);
}

void toglHid() {

	if (toglMap(HID))
		rstMap(FRMOF);
	else
		setMap(FRMOF);
	unthum();
	redraw(hbuts[HHID]);
	setMap(RESTCH);
}

void dulin() {

	SelectObject(StitchWindowDC, linePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	if (mlin0[0].x == mlin1[1].x&&mlin0[0].y == mlin1[1].y) {

		if (chkMap(ISDWN)) {

			Polyline(StitchWindowDC, mlin0, 2);
			goto dux;
		} else {

			Polyline(StitchWindowDC, mlin1, 2);
			goto dux;
		}
	} else {

		if (chkMap(ISDWN))
			Polyline(StitchWindowDC, mlin0, 2);
		if (chkMap(ISUP))
			Polyline(StitchWindowDC, mlin1, 2);
	}
dux:;
	SetROP2(StitchWindowDC, R2_COPYPEN);
	toglMap(WASLIN);
}

void unlin() {

	if (chkMap(WASLIN))
		dulin();
}

void movbox() {

	if (stch2px(closestPointIndex)) {

		unbox();
		dubox();
		if (chkMap(UPTO))
			setMap(RESTCH);
	} else {

		shft2box();
		setMap(SELBOX);
		rstMap(FRMPSEL);
		setMap(RESTCH);
	}
	nuAct(closestPointIndex);
	ritcor(&stitchBuffer[closestPointIndex]);
}

BOOL chkhid(unsigned cpnt) {

	if (chkMap(HID)) {

		if (colch[cpnt].colorIndex == actcol)
			return 1;
		else
			return 0;
	} else
		return 1;
}

/*
This function attempts to find the stitch the user is trying to select.
A rectangle is created that is slightly larger than the stitch.
If the mouse position is outside this rectangle, the stitch is disqualified.
If the mouse position is inside the rectangle, the distance from the stitch
 to the select point (sPnt) is calculated.

Find the equation for the line by solving the linear parametric eauations

  stitchBuffer[ind].x+slop*stitchBuffer[ind].y=off
  stitchBuffer[ind+1].x+slop*stitchBuffer[ind+1].y=off

substituting:

  stitchBuffer[ind].x+slop*stitchBuffer[ind].y=stitchBuffer[ind+1].x+slop*stitchBuffer[ind+1].y

collecting terms:

  slop*stitchBuffer[ind].y-slop*stitchBuffer[ind-1].y=stitchBuffer[ind+1].x-stitchBuffer[ind].x
  slop*(stitchBuffer[ind].y-stitchBuffer[ind-1].y)=stitchBuffer[ind+1].x-stitchBuffer[ind].x
  slop=(stitchBuffer[ind+1].x-stitchBuffer[ind].x)/(stitchBuffer[ind].y-stitchBuffer[ind-1].y)

define xba=stitchBuffer[ind+1].x-stitchBuffer[ind].x
define yab=stitchBuffer[ind].y-stitchBuffer[ind+1].y

  slop=xba/yab;

back substituting into stitchBuffer[ind].x+slop*stitchBuffer[ind].y=off

  off=stitchBuffer[ind].x+slop*stitchBuffer[ind].y

The equation for a point vertical to the equation for the line and running
 through sPnt is:

  sPnt.x-sPnt.y/slop=poff

If ipnt is the intersections between the two lines then

  ipnt.x-slop*ipnt.y=off
  ipnt.x+ipnt.y/slop=poff

Subtracting the two equations

  slop*ipnt.y+ipnt.y/slop=off-poff

Multiply by slop

  slop*slop*ipnt.y+ipnt.y=slop(off-poff)
  ipnt.y(slop*slop+1)=slop(off-poff)
  ipnt.y=slop*(off-poff)/(slop*slop+1)

back substitute into ipnt.x+slop*ipnt.y=off

  ipnt.x=off-slop*ipnt.y

if dx=ipnt.x-spnt.x & dy=ipnt.y-spnt.y

the distance from the point to the line is given by

  sqrt(dx*dx+dy*dy)
*/

unsigned closlin() {

	unsigned	ind, ine, scnt, clos = 0xffffffff;
	double		sum = 1e99;
	long		xba, yab;
	double		tsum;
	dRECTANGLE		trct;
	double		slop, off, poff, tol, dx, dy;
	dPOINT		ipnt;
	dPOINT		chkpnt;
	fPOINTATTRIBUTE*	l_pstch;
	unsigned	layr;

#define		TOL	20

	unboxs();
	off = (float)(msg.pt.x - stitchWindowAbsRect.left) / (stitchWindowAbsRect.right - stitchWindowAbsRect.left);
	chkpnt.x = off*(zoomRect.right - zoomRect.left) + zoomRect.left;
	off = (float)(stitchWindowAbsRect.bottom - msg.pt.y) / (stitchWindowAbsRect.bottom - stitchWindowAbsRect.top);
	chkpnt.y = (off*(zoomRect.top - zoomRect.bottom) + zoomRect.bottom);
	off = (double)(zoomRect.right - zoomRect.left) / stitchWindowClientRect.right;
	tol = off*TOL;
	for (ine = 0; ine < colCnt; ine++) {

		scnt = colch[ine + 1].stitchIndex - colch[ine].stitchIndex;
		l_pstch = &stitchBuffer[colch[ine].stitchIndex];
		if (chkhid(ine)) {

			for (ind = 0; ind < scnt; ind++) {

				layr = (l_pstch[ine].attribute&LAYMSK) >> LAYSHFT;
				if (!activeLayer || !layr || (layr == activeLayer)) {

					xba = l_pstch[ind + 1].x - l_pstch[ind].x;
					yab = l_pstch[ind].y - l_pstch[ind + 1].y;
					if (xba > 0) {

						trct.left = l_pstch[ind].x - tol;
						trct.right = l_pstch[ind + 1].x + tol;
					} else {

						trct.left = l_pstch[ind + 1].x - tol;
						trct.right = l_pstch[ind].x + tol;
					}
					if (yab < 0) {

						trct.bottom = l_pstch[ind].y - tol;
						trct.top = l_pstch[ind + 1].y + tol;
					} else {

						trct.bottom = l_pstch[ind + 1].y - tol;
						trct.top = l_pstch[ind].y + tol;
					}
					if (chkpnt.x > trct.left&&chkpnt.x<trct.right
						&&chkpnt.y>trct.bottom&&chkpnt.y < trct.top) {

						if (yab == 0) {

							//stitch is horizontal
							ipnt.x = chkpnt.x;
							ipnt.y = l_pstch[ind].y;
							tsum = ((l_pstch[ind].y > chkpnt.y) ? (l_pstch[ind].y - chkpnt.y) : (chkpnt.y - l_pstch[ind].y));
							goto gotsum;
						}
						if (xba == 0) {

							//stitch is vertical
							dx = l_pstch[ind].x - chkpnt.x;
							trct.top -= tol;
							trct.bottom += tol;
							if (chkpnt.y > trct.top) {

								dy = chkpnt.y - trct.top;
								tsum = hypot(dx, dy);
								goto gotal;
							}
							if (chkpnt.y < trct.bottom) {

								dy = chkpnt.y - trct.bottom;
								tsum = hypot(dx, dy);
								goto gotal;
							}
							tsum = fabs(dx);
							goto gotal;
						}
						slop = (double)xba / yab;
						off = l_pstch[ind].x + slop*l_pstch[ind].y;
						poff = chkpnt.x - chkpnt.y / slop;
						ipnt.y = slop*(off - poff) / (slop*slop + 1);
						ipnt.x = off - slop*ipnt.y;
						dx = ipnt.x - chkpnt.x;
						dy = ipnt.y - chkpnt.y;
						tsum = hypot(dx, dy);
					gotsum:;
						trct.bottom += tol;
						trct.top -= tol;
						trct.left += tol;
						trct.right -= tol;
						if (ipnt.x < trct.left) {

							if (ipnt.y < trct.bottom)
								tsum = sqrt(((chkpnt.x > trct.left) ? (chkpnt.x - trct.left) : (trct.left - chkpnt.x)) + ((chkpnt.y > trct.bottom) ? (chkpnt.y - trct.bottom) : (trct.bottom - chkpnt.y)));
							else
								tsum = sqrt(((chkpnt.x > trct.left) ? (chkpnt.x - trct.left) : (trct.left - chkpnt.x)) + ((chkpnt.y > trct.top) ? (chkpnt.y - trct.top) : (chkpnt.y - trct.top)));
						} else {

							if (ipnt.x > trct.right) {

								if (ipnt.y < trct.bottom)
									tsum = sqrt(((chkpnt.x > trct.right) ? (chkpnt.x - trct.right) : (trct.right - chkpnt.x)) + ((chkpnt.y > trct.bottom) ? (chkpnt.y - trct.bottom) : (trct.bottom - chkpnt.y)));
								else
									tsum = sqrt(((chkpnt.x > trct.right) ? (chkpnt.x - trct.right) : (trct.right - chkpnt.x)) + ((chkpnt.y > trct.top) ? (chkpnt.y - trct.top) : (trct.top - chkpnt.y)));
							}
						}
					gotal:;
						if (tsum < sum) {

							sum = tsum;
							clos = ind + colch[ine].stitchIndex;
						}
					}
				}
			}
		}
	}
	if (sum > tol)
		return 0xffffffff;
	else
		return clos;
}

void ilin() {

	SelectObject(StitchWindowDC, linePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, inlin, 2);
	SetROP2(StitchWindowDC, R2_XORPEN);
	Polyline(StitchWindowDC, &inlin[1], 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	SelectObject(StitchWindowMemDC, linePen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, inlin, 2);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	Polyline(StitchWindowMemDC, &inlin[1], 2);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void xlin() {

	if (rstMap(ILIN))
		ilin();
}

void ilin1() {

	SelectObject(StitchWindowDC, linePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, inlin, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void xlin1() {

	if (rstMap(ILIN1))
		ilin1();
}

void duIns() {

	POINT tlin[2];

	inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
	inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
	stch2px1(closestPointIndex);
	tlin[0].x = inlin[0].x = stitchSizePixels.x;
	tlin[0].y = inlin[0].y = stitchSizePixels.y;
	stch2px1(closestPointIndex + 1);
	tlin[1].x = inlin[2].x = stitchSizePixels.x;
	tlin[1].y = inlin[2].y = stitchSizePixels.y;
	xlin();
	setMap(ILIN);
	ilin();
}

void istch() {

	double angb, angf, angt;

	xlin();
	xlin1();
	if (chkMap(SELBOX)) {

		if (closestPointIndex&&closestPointIndex != (unsigned)hed.stitchCount - 1) {

			px2stch();
			angt = atan2(stitchBuffer[closestPointIndex].y - sPnt.y, stitchBuffer[closestPointIndex].x - sPnt.x);
			angb = atan2(stitchBuffer[closestPointIndex].y - stitchBuffer[closestPointIndex - 1].y, stitchBuffer[closestPointIndex].x - stitchBuffer[closestPointIndex - 1].x);
			angf = atan2(stitchBuffer[closestPointIndex].y - stitchBuffer[closestPointIndex + 1].y, stitchBuffer[closestPointIndex].x - stitchBuffer[closestPointIndex + 1].x);
			if (fabs(angf - angt) > fabs(angb - angt))
				closestPointIndex--;
		} else {

			if (closestPointIndex == (unsigned)hed.stitchCount - 1)
				closestPointIndex--;
		}
	} else
		closestPointIndex = closlin();
	if (closestPointIndex == -1)
		rstMap(INSRT);
	else {

		setMap(INSRT);
		duIns();
		SetCapture(hWnd);
		ritnum(STR_NUMSEL, closestPointIndex);
		nuAct(closestPointIndex);
	}
}

void cros(unsigned ind) {

	long pwid = boxOff[0];

	stch2px1(ind);
	inlin[0].x = stitchSizePixels.x - pwid;
	inlin[1].y = inlin[0].y = stitchSizePixels.y;
	inlin[1].x = stitchSizePixels.x + pwid;
	SelectObject(StitchWindowDC, crossPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, inlin, 2);
	SelectObject(StitchWindowMemDC, crossPen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, inlin, 2);
	inlin[0].y = stitchSizePixels.y - pwid;
	inlin[0].x = inlin[1].x = stitchSizePixels.x;
	inlin[1].y = stitchSizePixels.y - 1;
	Polyline(StitchWindowDC, inlin, 2);
	Polyline(StitchWindowMemDC, inlin, 2);
	inlin[0].y = stitchSizePixels.y + 2;
	inlin[1].y = stitchSizePixels.y + pwid;
	Polyline(StitchWindowDC, inlin, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	Polyline(StitchWindowMemDC, inlin, 2);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void mark() {

	if (chkMap(SELBOX) || chkMap(INSRT) || closPnt1(&closestPointIndex)) {

		unbox();
		xlin();
		rstMap(CAPT);
		rstMap(SELBOX);
		rstMap(INSRT);
		rstMap(SCROS);
		rstMap(ECROS);
		groupStitchIndex = closestPointIndex;
		setMap(GRPSEL);
	}
}

void selCol()
{
	unsigned ind, col;

	if (hed.stitchCount)
	{
		if (chkMap(SELBOX))
			ind = closestPointIndex;
		else
		{
			if (chkMap(GRPSEL))
				ind = groupStitchIndex;
			else
				ind = 0;
		}
		if (ind > (unsigned)hed.stitchCount - 1)
			ind = hed.stitchCount - 1;
		closestPointIndex = groupStitchIndex = ind;
		col = stitchBuffer[ind].attribute&COLMSK;
		while (closestPointIndex && (stitchBuffer[closestPointIndex].attribute&COLMSK) == col)
			closestPointIndex--;
		if ((stitchBuffer[closestPointIndex].attribute&COLMSK) != col)
			closestPointIndex++;
		while (groupStitchIndex < hed.stitchCount && (stitchBuffer[groupStitchIndex].attribute&COLMSK) == col)
			groupStitchIndex++;
		if ((stitchBuffer[closestPointIndex].attribute&COLMSK) != col)
			closestPointIndex--;
		if (groupStitchIndex > (unsigned)hed.stitchCount - 1)
			groupStitchIndex = hed.stitchCount - 1;
		setMap(GRPSEL);
		unbox();
		grpAdj();
		actcol = col;
		setMap(RESTCH);
	}
}

void nuAct(unsigned ind) {

	unsigned tcol;

	tcol = actcol;
	actcol = stitchBuffer[ind].attribute&COLMSK;
	redraw(hCol[tcol]);
	redraw(hCol[actcol]);
}

void newFil() {

	unsigned ind;

	rstMap(CMPDO);
	if (bnam[0]) {

		bnam[0] = 0;
		DeleteObject(hBmp);
		ReleaseDC(hWnd, bitmapDC);
	}
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], ini.designerName);
	deldu();
	SetWindowText(hWnd, msgbuf);
	strcpy_s(thrnam, stab[STR_NUFIL]);
	ritfnam(ini.designerName);
	strcpy_s(hedx.modifierName, ini.designerName);
	rstdu();
	rstAll();
	clrhbut(3);
	rstMap(MOVSET);
	hed.leadIn = 0x32;
	hed.colorCount = 16;
	unbox();
	xlin();
	xlin1();
	rstMap(INIT);
	rstMap(INSRT);
	rstMap(LIN1);
	rstMap(FORMSEL);
	rstMap(BAKACT);
	rstMap(GMRK);
	hed.stitchCount = 0;
	cbit = 0;
	bnam[0] = 0;
	hed.stitchCount = 0;
	fltad = 0;
	clpad = 0;
	txad = 0;
	satkad = 0;
	formIndex = 0;
	filnam[0] = 0;
	colCnt = 0;
	knotcnt = 0;
	for (ind = 0; ind < 16; ind++) {

		redraw(hDef[ind]);
		redraw(hCol[ind]);
		redraw(hSiz[ind]);
	}
	zumhom();
}

void bBox() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, linePen);
	Polyline(StitchWindowDC, blin, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unbBox() {

	if (rstMap(BZUM))
		bBox();
}

void rebox() {

	unsigned ind;

	unbox();
	if (closPnt1(&closestPointIndex)) {

		nuAct(closestPointIndex);
		if (stch2px(closestPointIndex))
			dubox();
		if (rstMap(GRPSEL)) {

			rstMap(SCROS);
			rstMap(ECROS);
			slpnt = 0;
			setMap(RESTCH);
			for (ind = 0; ind < 16; ind++)
				redraw(hCol[ind]);
		}
		ritcor(&stitchBuffer[closestPointIndex]);
	}
}

void endpnt() {

	unbox();
	xlin();
	xlin1();
	setMap(LIN1);
	setMap(INSRT);
	rstMap(GRPSEL);
	inlin[0].x = stitchSizePixels.x;
	inlin[0].y = stitchSizePixels.y;
	inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
	inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
}

void delstchm() {

	unsigned ind, dst;

	rngadj();
	dst = groupStartStitch;
	for (ind = groupEndStitch; ind < hed.stitchCount; ind++) {

		stitchBuffer[dst].attribute = stitchBuffer[ind].attribute;
		stitchBuffer[dst].x = stitchBuffer[ind].x;
		stitchBuffer[dst++].y = stitchBuffer[ind].y;
	}
	hed.stitchCount = dst - 1;
}

void unsid() {

	formMenuChoice = 0;
	if (hSid) {

		DestroyWindow(hSid);
		hSid = 0;
	}
}

void unbsho() {

	unsigned ind;

	if (rstMap(BAKSHO)) {

		for (ind = 0; ind < OLDVER; ind++)
			DestroyWindow(hvu[ind]);
	}
}

BOOL oldwnd(HWND hwnd) {

	unsigned ind;

	for (ind = 0; ind < 16; ind++) {

		if (hDef[ind] == hwnd || hCol[ind] == hwnd || hSiz[ind] == hwnd)
			return 0;
	}
	for (ind = 0; ind < 9; ind++) {

		if (hbuts[ind] == hwnd)
			return 0;
	}
	if (hStch == hwnd)
		return 0;
	if (hVrt == hwnd)
		return 0;
	if (hHor == hwnd)
		return 0;
	if (hBar == hwnd)
		return 0;
	if (hmin == hwnd)
		return 0;
	if (hSped == hwnd)
		return 0;
	return 1;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	if (oldwnd(hwnd))
		return DestroyWindow(hwnd);
	else {

		if (fwnd) {

			if (fwnd == hwnd)
				return 0;
			else
				return TRUE;
		} else {

			fwnd = hwnd;
			return TRUE;
		}
	}
}

void insadj() {

	rstMap(PRELIN);
	fvars(closestFormToCursor);
	refil();
	coltab();
	frmout(closestFormToCursor);
}

void rstAll() {

	rstMap(WASFPNT);
	rstMap(WASFRMFRM);
	rstMap(FPUNCLP);
	rstMap(FPSEL);
	rstMap(CAPT);
	rstMap(INSRT);
	rstMap(SELBOX);
	rstMap(GRPSEL);
	rstMap(SCROS);
	rstMap(ECROS);
	rstMap(LIN1);
	rstMap(CLPSHO);
	rstMap(SELPNT);
	rstMap(ROTAT);
	rstMap(ROTCAPT);
	rstMap(FRMPMOV);
	rstMap(MOVFRM);
	rstMap(SATCNKT);
	rstMap(FRMPSEL);
	rstMap(SHOINSF);
	rstMap(STRTCH);
	rstMap(SHOSTRTCH);
	rstMap(EXPAND);
	rstMap(POLIMOV);
	rstMap(BZUMIN);
	rstMap(OSAV);
	rstMap(SAVEX);
	rstMap(MOVFRMS);
	rstMap(FRMROT);
	rstMap(DELSFRMS);
	rstMap(BIGBOX);
	rstMap(UPTO);
	rstMap(LENSRCH);
	rstMap(WASGRP);
	rstMap(BOXZUM);
	rstMap(HIDSTCH);
	rstMap(ENTRPOL);
	rstMap(ENTRSTAR);
	rstMap(ENTRSPIR);
	rstMap(ENTRHART);
	rstMap(FORMIN);
	rstMap(GTUANG);
	rstMap(GTUSPAC);
	rstMap(GTWLKIND);
	rstMap(GTWLKLEN);
	untrace();
	rstMap(WASEDG);
	butxt(HUPTO, stab[STR_UPOF]);
	if (zoomFactor == 1)
		rstMap(ZUMED);
	movStch();
	ShowWindow(hStch, TRUE);
	unbsho();
	rstMap(SIDACT);
	unsid();
	rstMap(PRFACT);
	rstMap(WASRT);
	DestroyWindow(hPrf);
	undat();
	if (rstMap(INSFRM))
		insadj();
	rstMap(FUNCLP);
	if (rstMap(SATPNT))
		satfix();
	if (rstMap(RUNPAT))
		patdun();
	rstMap(FORMSEL);
	rstMap(FRMPSEL);
	unmsg();
	slpnt = selectedFormCount = 0;
	fwnd = 0;
	while (EnumChildWindows(hStch, EnumChildProc, 0));
}

void rstdu() {

	deldu();
	rstMap(REDUSHO);
	rstMap(UNDUSHO);
	EnableMenuItem(hMen, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	EnableMenuItem(hMen, M_REDO, MF_BYPOSITION | MF_GRAYED);
	setMap(DUMEN);
}

CLPSTCH* deref(void* pnt) {

#if  __UseASM__
	_asm {

		mov		eax, pnt
		mov		eax, [eax]
	}
#else
	//check translation
	return *(CLPSTCH **)pnt;
#endif
}

void duclp() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, linePen);
	Polyline(StitchWindowDC, clplin, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unclp() {

	if (rstMap(CLPSHO))
		duclp();
}

void dusel(HDC dc) {

	unsigned ind;

	SetROP2(dc, R2_NOTXORPEN);
	SelectObject(dc, linePen);
	Polyline(dc, formOutlineRectangle, 9);
	for (ind = 0; ind < 8; ind++)
		selsqr(formOutlineRectangle[ind], dc);
	SetROP2(dc, R2_COPYPEN);
}

void unsel() {

	if (rstMap(SELSHO))
		dusel(StitchWindowDC);
}

void clpbox() {

	SIZE	clpx;
	double	tdub;

	px2stch();
	if (sPnt.x + clipboardRectSize.cx > unzoomedRect.x)
		sPnt.x = unzoomedRect.x - clipboardRectSize.cx;
	if (sPnt.y + clipboardRectSize.cy > unzoomedRect.y)
		sPnt.y = unzoomedRect.y - clipboardRectSize.cy;
	clipboardOrigin.x = sPnt.x;
	clipboardOrigin.y = sPnt.y;
	tdub = (double)stitchWindowClientRect.right / (zoomRect.right - zoomRect.left);
	clpx.cx = clipboardRectSize.cx*tdub + 0.5;
	clpx.cy = clipboardRectSize.cy*tdub + 0.5;
	stitchSizePixels.x = (sPnt.x - zoomRect.left)*tdub + 0.5;
	stitchSizePixels.y = stitchWindowClientRect.bottom - (sPnt.y - zoomRect.bottom)*tdub + 0.5 - clpx.cy;
	clplin[0].x = clplin[3].x = clplin[4].x = stitchSizePixels.x;
	clplin[0].y = clplin[1].y = clplin[4].y = stitchSizePixels.y;
	clplin[1].x = clplin[2].x = clplin[0].x + clpx.cx;
	clplin[2].y = clplin[3].y = clplin[0].y + clpx.cy;
	setMap(CLPSHO);
	duclp();
}

void lodclp(unsigned ind) {

	fPOINT		padj;
	unsigned	src, dst;

	if (ind != hed.stitchCount)
		ind++;
	padj.x = clipboardOrigin.x - clipboardRect.left;
	padj.y = clipboardOrigin.y - clipboardRect.bottom;
	if (ind < (unsigned)hed.stitchCount&&chkMap(INIT)) {

		src = hed.stitchCount - 1;
		dst = hed.stitchCount + clipboardStitchCount - 1;
		while (src >= ind) {

			stitchBuffer[dst].x = stitchBuffer[src].x;
			stitchBuffer[dst].y = stitchBuffer[src--].y;
			stitchBuffer[dst--].attribute = stitchBuffer[src].attribute;
		}
	}
	closestPointIndex = ind;
	for (src = 0; src < clipboardStitchCount; src++) {

		stitchBuffer[ind].x = clipBuffer[src].x + padj.x;
		stitchBuffer[ind].y = clipBuffer[src].y + padj.y;
		stitchBuffer[ind++].attribute = clipBuffer[src].attribute&COLMSK | layerIndex | NOTFRM;
	}
	groupStitchIndex = ind - 1;
	setMap(GRPSEL);
	hed.stitchCount += clipboardStitchCount;
	if (hed.stitchCount)
		setMap(INIT);
}

void rSelbox() {

	SIZE	selx;
	double	tdub;

	unsel();
	px2stch();
	if (sPnt.x - selectBoxOffset.x + selectBoxSize.cx >= unzoomedRect.x)
		sPnt.x = unzoomedRect.x - selectBoxSize.cx + selectBoxOffset.x;
	if (sPnt.y - selectBoxOffset.y + selectBoxSize.cy >= unzoomedRect.y)
		sPnt.y = unzoomedRect.y - selectBoxSize.cy + selectBoxOffset.y;
	if (sPnt.x - selectBoxOffset.x < 0)
		sPnt.x = selectBoxOffset.x;
	if (sPnt.y - selectBoxOffset.y < 0)
		sPnt.y = selectBoxOffset.y;
	tdub = (double)stitchWindowClientRect.right / (zoomRect.right - zoomRect.left);
	selx.cx = selectBoxSize.cx*tdub + 0.5;
	selx.cy = selectBoxSize.cy*tdub + 0.5;
	stitchSizePixels.x = (sPnt.x - zoomRect.left - selectBoxOffset.x)*tdub + 0.5;
	stitchSizePixels.y = stitchWindowClientRect.bottom - (sPnt.y - zoomRect.bottom - selectBoxOffset.y)*tdub + 0.5 - selx.cy;
	formOutlineRectangle[0].x = formOutlineRectangle[6].x = formOutlineRectangle[7].x = formOutlineRectangle[8].x = stitchSizePixels.x;
	formOutlineRectangle[1].x = formOutlineRectangle[5].x = stitchSizePixels.x + selx.cx / 2;
	formOutlineRectangle[0].y = formOutlineRectangle[1].y = formOutlineRectangle[2].y = formOutlineRectangle[8].y = stitchSizePixels.y;
	formOutlineRectangle[3].y = formOutlineRectangle[7].y = stitchSizePixels.y + selx.cy / 2;
	formOutlineRectangle[4].y = formOutlineRectangle[5].y = formOutlineRectangle[6].y = stitchSizePixels.y + selx.cy;
	formOutlineRectangle[2].x = formOutlineRectangle[3].x = formOutlineRectangle[4].x = stitchSizePixels.x + selx.cx;
	setMap(SELSHO);
	dusel(StitchWindowDC);
}

void duSelbox() {

	px2stch();
	selectBoxSize.cx = rngrct.right - rngrct.left;
	selectBoxSize.cy = rngrct.top - rngrct.bottom;
	selectBoxOffset.x = sPnt.x - rngrct.left;
	selectBoxOffset.y = sPnt.y - rngrct.bottom;
}

void setbak(unsigned wid) {

	if (backgroundPenWidth != wid)
		backgroundPen = nuPen(backgroundPen, wid, backgroundColor);
}

void stchbox(unsigned ind, HDC dc) {

	POINT		stlin[5];
	unsigned	layr;

	layr = (stitchBuffer[ind].attribute&LAYMSK) >> LAYSHFT;
	if (!activeLayer || !layr || layr == activeLayer) {

		stch2px1(ind);
		stlin[0].x = stlin[3].x = stlin[4].x = stitchSizePixels.x - ini.stitchSizePixels;
		stlin[0].y = stlin[1].y = stitchSizePixels.y - ini.stitchSizePixels;
		stlin[1].x = stlin[2].x = stitchSizePixels.x + ini.stitchSizePixels;
		stlin[2].y = stlin[3].y = stitchSizePixels.y + ini.stitchSizePixels;
		stlin[4].y = stitchSizePixels.y - ini.stitchSizePixels;
		Polyline(dc, stlin, 5);
	}
}

void sCor2px(dPOINT stpnt, POINT* pxpnt) {

	pxpnt->x = (stpnt.x - zoomRect.left)*zoomRatio.x + 0.5;
	pxpnt->y = stitchWindowClientRect.bottom + (zoomRect.bottom - stpnt.y)*zoomRatio.y + 0.5;
}

void sdCor2px(fPOINTATTRIBUTE stpnt, POINT* pxpnt) {

	pxpnt->x = (stpnt.x - zoomRect.left)*zoomRatio.x + 0.5;
	pxpnt->y = stitchWindowClientRect.bottom + (zoomRect.bottom - stpnt.y)*zoomRatio.y + 0.5;
}

void durot() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, linePen);
	Polyline(StitchWindowDC, rlinb, 5);
	Polyline(StitchWindowDC, rlinv, 2);
	Polyline(StitchWindowDC, rlinh, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unrot() {

	if (rstMap(ROTSHO))
		durot();
}

void durotu() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, linePen);
	Polyline(StitchWindowDC, rlinu, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unrotu() {

	if (rstMap(ROTUSHO))
		durotu();
}

void rotang(dPOINT rpnt, POINT* tpnt) {

	double	len, ang0;
	dPOINT	ipnt;
	long	dx, dy;

	dx = rpnt.x - rotcntr.x;
	dy = rpnt.y - rotcntr.y;
	if (dx) {

		len = hypot(dx, dy);
		ang0 = atan2(dy, dx);
		ang0 += rotationAngle;
	} else {

		len = abs(dy);
		if (dy > 0)
			ang0 = PI / 2 + rotationAngle;
		else
			ang0 = rotationAngle - PI / 2;
	}
	ipnt.y = rotcntr.y + len*sin(ang0);
	ipnt.x = rotcntr.x + len*cos(ang0);
	sCor2px(ipnt, tpnt);
}

void rotang1(fPOINTATTRIBUTE rpnt, fPOINT* tpnt) {

	double	len, ang0;
	double	dx, dy;

	dx = rpnt.x - rotcntr.x;
	dy = rpnt.y - rotcntr.y;
	if (dx) {

		len = hypot(dx, dy);
		ang0 = atan2(dy, dx);
		ang0 += rotationAngle;
	} else {

		if (dy > 0) {
			len = dy;
			ang0 = PI / 2 + rotationAngle;
		} else {
			len = -dy;
			ang0 = rotationAngle - PI / 2;
		}
	}
	tpnt->y = rotcntr.y + len*sin(ang0);
	tpnt->x = rotcntr.x + len*cos(ang0);
}

void rotangf(fPOINT rpnt, fPOINT* tpnt) {

	double	len, ang0;
	double	dx, dy;

	dx = rpnt.x - rotcntr.x;
	dy = rpnt.y - rotcntr.y;
	if (dx) {

		len = hypot(dx, dy);
		ang0 = atan2(dy, dx);
		ang0 += rotationAngle;
	} else {

		if (dy > 0) {
			len = dy;
			ang0 = PI / 2 + rotationAngle;
		} else {
			len = -dy;
			ang0 = rotationAngle - PI / 2;
		}
	}
	tpnt->y = rotcntr.y + len*sin(ang0);
	tpnt->x = rotcntr.x + len*cos(ang0);
}

void rotpix(POINT rpnt, POINT* tpnt) {

	//won't handle vertical lines
	double	len, ang0;
	long	dx, dy;

	dx = rpnt.x - pixcntr.x;
	dy = rpnt.y - pixcntr.y;
	len = hypot(dx, dy);
	ang0 = atan2(dy, dx);
	ang0 -= pixang;
	tpnt->y = pixcntr.y + len*sin(ang0);
	tpnt->x = pixcntr.x + len*cos(ang0);
}

void rotflt(fPOINT* pnt) {

	double	len, ang0;
	double	dx, dy;

	dx = pnt->x - rotcntr.x;
	dy = pnt->y - rotcntr.y;
	if (dx) {

		len = hypot(dx, dy);
		ang0 = atan2(dy, dx);
		ang0 += rotationAngle;
	} else {

		if (dy > 0) {
			len = dy;
			ang0 = PI / 2 + rotationAngle;
		} else {
			len = -dy;
			ang0 = rotationAngle - PI / 2;
		}
	}
	pnt->y = rotcntr.y + len*sin(ang0);
	pnt->x = rotcntr.x + len*cos(ang0);
}

void rotstch(fPOINTATTRIBUTE* pnt) {

	double	len, ang0;
	double	dx, dy;

	dx = pnt->x - rotcntr.x;
	dy = pnt->y - rotcntr.y;
	if (dx) {

		len = hypot(dx, dy);
		ang0 = atan2(dy, dx);
		ang0 += rotationAngle;
	} else {

		if (dy > 0) {
			len = dy;
			ang0 = PI / 2 + rotationAngle;
		} else {
			len = -dy;
			ang0 = rotationAngle - PI / 2;
		}
	}
	pnt->y = rotcntr.y + len*sin(ang0);
	pnt->x = rotcntr.x + len*cos(ang0);
}

void ritrot() {

	POINT		tpnt;
	dPOINT		rotpnt;

	rotpnt.x = rotationRect.left;
	rotpnt.y = rotationRect.top;
	rotang(rotpnt, &tpnt);
	rlinb[0].x = rlinb[4].x = tpnt.x;
	rlinb[0].y = rlinb[4].y = tpnt.y;
	rotpnt.x = rotcntr.x;
	rotang(rotpnt, &tpnt);
	rlinv[0].x = tpnt.x;
	rlinv[0].y = tpnt.y;
	rotpnt.x = rotationRect.right;
	rotang(rotpnt, &tpnt);
	rlinb[1].x = tpnt.x;
	rlinb[1].y = tpnt.y;
	rotpnt.y = rotcntr.y;
	rotang(rotpnt, &tpnt);
	rlinh[1].x = tpnt.x;
	rlinh[1].y = tpnt.y;
	rotpnt.y = rotationRect.bottom;
	rotang(rotpnt, &tpnt);
	rlinb[2].x = tpnt.x;
	rlinb[2].y = tpnt.y;
	rotpnt.x = rotcntr.x;
	rotang(rotpnt, &tpnt);
	rlinv[1].x = tpnt.x;
	rlinv[1].y = tpnt.y;
	rotpnt.x = rotationRect.left;
	rotang(rotpnt, &tpnt);
	rlinb[3].x = tpnt.x;
	rlinb[3].y = tpnt.y;
	rotpnt.y = rotcntr.y;
	rotang(rotpnt, &tpnt);
	rlinh[0].x = tpnt.x;
	rlinh[0].y = tpnt.y;
	sCor2px(rotcntr, &rlinu[0]);
	setMap(ROTSHO);
	durot();
}

void durcntr() {

	rotcntr.x = midl(rotationRect.right, rotationRect.left);
	rotcntr.y = midl(rotationRect.top, rotationRect.bottom);
}

void rot() {

	if (chkMap(FPSEL)) {

		MoveMemory(&rotationRect, &selectedPointsRect, sizeof(fRECTANGLE));
		goto rotskp;
	}
	if (chkMap(BIGBOX)) {

		MoveMemory(&rotationRect, &allItemsRectangle, sizeof(fRECTANGLE));
		goto rotskp;
	}
	if (selectedFormCount) {

		pxrct2stch(selectedFormsRectangle, &rotationRect);
		setMap(FRMSROT);
		goto rotskp;
	}
	if (chkMap(FORMSEL)) {

		fvars(closestFormToCursor);
		setMap(FRMROT);
		MoveMemory(&rotationRect, &ptrSelectedForm->rectangle, sizeof(fRECTANGLE));
		goto rotskp;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		selRct(&rotationRect);
		goto rotskp;
	}
	shoseln(IDS_FGRPF, IDS_ROT);
	return;
rotskp:;
	setMap(ROTAT);
	durcntr();
	rotationAngle = 0;
	ritrot();
}

void savclp(unsigned dst, unsigned src) {

	double	frct, intg;

	clipboardStitchData[dst].led = stitchBuffer[src].attribute&COLMSK;
	frct = modf((double)stitchBuffer[src].x - lowerLeftStitch.x, &intg);
	clipboardStitchData[dst].fx = frct * 256;
	clipboardStitchData[dst].x = intg;
	clipboardStitchData[dst].spcx = 0;
	frct = modf((double)stitchBuffer[src].y - lowerLeftStitch.y, &intg);
	clipboardStitchData[dst].fy = frct * 256;
	clipboardStitchData[dst].y = intg;
	clipboardStitchData[dst].spcy = 0;
	clipboardStitchData[dst].myst = 1;
	clipboardStitchData[dst].typ = 0x14;
}

void rtclpfn(unsigned dst, unsigned src) {

	double	frct, intg;

	clipboardStitchData[dst].led = 0;
	frct = modf(clipBuffer[src].x - lowerLeftStitch.x, &intg);
	clipboardStitchData[dst].fx = frct;
	clipboardStitchData[dst].x = intg;
	clipboardStitchData[dst].spcx = 0;
	frct = modf(clipBuffer[src].y - lowerLeftStitch.y, &intg);
	clipboardStitchData[dst].fy = frct;
	clipboardStitchData[dst].y = intg;
	clipboardStitchData[dst].spcy = 0;
	clipboardStitchData[dst].myst = 1;
	clipboardStitchData[dst].typ = 0x14;
}

FORMCLIP* frmref(void* pnt) {

#if  __UseASM__
	_asm {

		mov		eax, pnt
		mov		eax, [eax]
	}
#else
	return *(FORMCLIP **)pnt;
#endif
}

FORMSCLIP* frmsref(void* pnt) {

#if  __UseASM__
	_asm {

		mov		eax, pnt
		mov		eax, [eax]
	}
#else
	return *(FORMSCLIP **)pnt;
#endif
}

FORMPOINTCLIP* fpref(void* pnt) {

#if  __UseASM__
	_asm {

		mov		eax, pnt
		mov		eax, [eax]
	}
#else
	return *(FORMPOINTCLIP **)pnt;
#endif
}

unsigned sizfclp()
{
	unsigned l_siz;

	l_siz = sizeof(FORMCLIP) + sides * sizeof(fPOINT);
	if (ptrSelectedForm->type == SAT)
		l_siz += ptrSelectedForm->satinGuideCount * sizeof(SATCON);
	if (iseclp(closestFormToCursor))
		l_siz += ptrSelectedForm->clipEntries * sizeof(fPOINT);
	if (isclpx(closestFormToCursor))
		l_siz += ptrSelectedForm->lengthOrCount.clipCount * sizeof(fPOINT);
	if (istx(closestFormToCursor))
		l_siz += ptrSelectedForm->fillInfo.texture.count * sizeof(TXPNT);
	return l_siz;
}

unsigned frmcnt(unsigned fpnt) {

	unsigned	at, cnt = 0, ind;

	lowerLeftStitch.x = lowerLeftStitch.y = (float)1e20;
	at = fpnt << 4;
	for (ind = 0; ind < hed.stitchCount; ind++) {

		if ((stitchBuffer[ind].attribute&FRMSK) == at&&stitchBuffer[ind].attribute&TYPMSK)
			goto fskip;
	}
	return 0;
fskip:;
	frmstrt = ind;
	while (ind < hed.stitchCount) {

		if ((stitchBuffer[ind].attribute&FRMSK) == at&&stitchBuffer[ind].attribute&TYPMSK) {

			if (stitchBuffer[ind].x < lowerLeftStitch.x)
				lowerLeftStitch.x = stitchBuffer[ind].x;
			if (stitchBuffer[ind].y < lowerLeftStitch.y)
				lowerLeftStitch.y = stitchBuffer[ind].y;
			cnt++;
		}
		ind++;
	}
	return cnt;
}

unsigned sizclp() {

	unsigned len = 0;

	len = siz = sizeof(FORMCLIP) + sides * sizeof(fPOINT);
	if (ptrSelectedForm->type == SAT)
		siz += ptrSelectedForm->satinGuideCount * sizeof(SATCON);
	if (ptrSelectedForm->fillType || ptrSelectedForm->edgeType) {

		len += frmcnt(closestFormToCursor);
		siz += len * sizeof(fPOINTATTRIBUTE);
	}
	if (iseclp(closestFormToCursor))
		siz += ptrSelectedForm->clipEntries * sizeof(fPOINT);
	if (isclpx(closestFormToCursor))
		siz += ptrSelectedForm->lengthOrCount.clipCount * sizeof(fPOINT);
	if (istx(closestFormToCursor))
		siz += ptrSelectedForm->fillInfo.texture.count * sizeof(TXPNT);
	return len;
}

unsigned lenclp()
{
	unsigned cod, ind, cnt;

	cnt = 0;
	cod = closestFormToCursor << FRMSHFT;
	for (ind = 0; ind < hed.stitchCount; ind++)
	{
		if ((stitchBuffer[ind].attribute&(FRMSK | NOTFRM)) == cod)
			cnt++;
	}
	return cnt;
}

void duclip() {

	unsigned	ind, ine, len = 0, inf, at, msiz = 0;
	SATCON*		l_sac;
	fPOINT*		clp;
	fPOINT*		mclp;
	TXPNT*		ptx;
	TXPNT*		ptxs;
	FRMHED*		tfrm;
	fPOINTATTRIBUTE*	astch;
	FORMPOINTCLIP*		pclp;
	fPOINT*		tflt;

	if (chkMap(FPSEL)) {

		if (OpenClipboard(hWnd)) {

			EmptyClipboard();
			htclp = RegisterClipboardFormat(thrEdClipFormat);
			ptrThrEdClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (selectedFormPoints.pointCount + 1) * sizeof(fPOINT) + sizeof(FORMPOINTCLIP));
			pclp = fpref(ptrThrEdClipVoid);
			pclp->clipType = CLP_FRMPS;
			pclp->pointCount = selectedFormPoints.pointCount;
			if (chkMap(PSELDIR))
				pclp->direction = 1;
			else
				pclp->direction = 0;
			tflt = (fPOINT*)&pclp[1];
			fvars(closestFormToCursor);
			ine = selectedFormPoints.start;
			for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {

				tflt[ind].x = currentFormVertices[ine].x;
				tflt[ind].y = currentFormVertices[ine].y;
				ine = pdir(ine);
			}
			SetClipboardData(htclp, ptrThrEdClipVoid);
			CloseClipboard();
		}
		return;
	}
	if (chkMap(BIGBOX))
		tabmsg(IDS_INSF);
	else {

		if (OpenClipboard(hWnd)) {

			EmptyClipboard();
			htclp = RegisterClipboardFormat(thrEdClipFormat);
			if (selectedFormCount) {

				for (ind = 0; ind < selectedFormCount; ind++) {

					closestFormToCursor = selectedFormList[ind];
					fvars(closestFormToCursor);
					len += sizfclp();
					msiz += siz;
				}
				ptrThrEdClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, msiz + len);
				clipboardFormsData = frmsref(ptrThrEdClipVoid);
				clipboardFormsData->clipType = CLP_FRMS;
				clipboardFormsData->formCount = selectedFormCount;
				tfrm = (FRMHED*)&clipboardFormsData[1];
				for (ind = 0; ind < selectedFormCount; ind++) {

					ptrSelectedForm = &formList[selectedFormList[ind]];
					frmcpy(&tfrm[ind], &formList[selectedFormList[ind]]);
				}
				currentFormVertices = (fPOINT*)&tfrm[ind];
				inf = 0;
				for (ind = 0; ind < selectedFormCount; ind++) {

					ptrSelectedForm = &formList[selectedFormList[ind]];
					for (ine = 0; ine < ptrSelectedForm->sides; ine++) {

						currentFormVertices[inf].x = ptrSelectedForm->vertices[ine].x;
						currentFormVertices[inf++].y = ptrSelectedForm->vertices[ine].y;
					}
				}
				l_sac = (SATCON*)&currentFormVertices[inf];
				inf = 0;
				for (ind = 0; ind < selectedFormCount; ind++) {

					ptrSelectedForm = &formList[selectedFormList[ind]];
					if (ptrSelectedForm->type == SAT) {

						for (ine = 0; ine < ptrSelectedForm->satinGuideCount; ine++) {

							l_sac[inf].start = ptrSelectedForm->satinOrAngle.sac[ine].start;
							l_sac[inf++].finish = ptrSelectedForm->satinOrAngle.sac[ine].finish;
						}
					}
				}
				clp = (fPOINT*)&l_sac[inf];
				inf = 0;
				for (ind = 0; ind < selectedFormCount; ind++) {

					ptrSelectedForm = &formList[selectedFormList[ind]];
					if (isclpx(selectedFormList[ind])) {

						for (ine = 0; ine < ptrSelectedForm->lengthOrCount.clipCount; ine++) {

							clp[inf].x = ptrSelectedForm->angleOrClipData.clip[ine].x;
							clp[inf++].y = ptrSelectedForm->angleOrClipData.clip[ine].y;
						}
					}
					if (iseclp(selectedFormList[ind])) {

						for (ine = 0; ine < ptrSelectedForm->clipEntries; ine++) {

							clp[inf].x = ptrSelectedForm->borderClipData[ine].x;
							clp[inf++].y = ptrSelectedForm->borderClipData[ine].y;
						}
					}
				}
				ptx = (TXPNT*)&clp[inf];
				ine = 0;
				for (ind = 0; ind < selectedFormCount; ind++)
				{
					ptrSelectedForm = &formList[selectedFormList[ind]];
					if (istx(selectedFormList[ind]))
					{
						MoveMemory(&ptx[ine], &txpnts[ptrSelectedForm->fillInfo.texture.index], ptrSelectedForm->fillInfo.texture.count * sizeof(TXPNT));
						tfrm[ind].fillInfo.texture.index = ine;
						ine += ptrSelectedForm->fillInfo.texture.count;
					}
				}
				SetClipboardData(htclp, ptrThrEdClipVoid);
				CloseClipboard();
				clRmap(RMAPSIZ);
				for (ind = 0; ind < selectedFormCount; ind++)
					setr(selectedFormList[ind]);
				astch = &stitchBuffer[MAXSEQ];
				ine = 0;
				lowerLeftStitch.x = lowerLeftStitch.y = (float)1e30;
				for (ind = 0; ind < hed.stitchCount; ind++) {

					if (!(stitchBuffer[ind].attribute&NOTFRM) && chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {

						if (stitchBuffer[ind].x < lowerLeftStitch.x)
							lowerLeftStitch.x = stitchBuffer[ind].x;
						if (stitchBuffer[ind].y < lowerLeftStitch.y)
							lowerLeftStitch.y = stitchBuffer[ind].y;
						astch[ine].x = stitchBuffer[ind].x;
						astch[ine].y = stitchBuffer[ind].y;
						astch[ine++].attribute = stitchBuffer[ind].attribute;
					}
				}
				if (hed.stitchCount&&ine) {

					hClip = RegisterClipboardFormat(pcdClipFormat);
					ptrClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, ine * sizeof(CLPSTCH) + 2);
					clipboardStitchData = deref(ptrClipVoid);
					ind = MAXSEQ;
					inf = 0;
					savclp(0, ind);
					ind++;
					clipboardStitchData[0].led = ine;
					inf++;
					while (ind < ine + MAXSEQ)
						savclp(inf++, ind++);
					SetClipboardData(hClip, ptrClipVoid);
					CloseClipboard();
				}
			} else {

				if (chkMap(FORMSEL)) {

					len = sizclp();
					fvars(closestFormToCursor);
					l_sac = 0; at = closestFormToCursor << 4;
					siz += sizeof(FORMCLIP);
					ptrThrEdClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, siz);
					clipboardFormData = frmref(ptrThrEdClipVoid);
					clipboardFormData->clipType = CLP_FRM;
					frmcpy(&clipboardFormData->form, &formList[closestFormToCursor]);
					currentFormVertices = (fPOINT*)&clipboardFormData[1];
					for (ind = 0; ind < ptrSelectedForm->sides; ind++) {

						currentFormVertices[ind].x = ptrSelectedForm->vertices[ind].x;
						currentFormVertices[ind].y = ptrSelectedForm->vertices[ind].y;
					}
					l_sac = (SATCON*)&currentFormVertices[sides];
					ind = 0;
					if (ptrSelectedForm->type == SAT) {

						for (ind = 0; ind < ptrSelectedForm->satinGuideCount; ind++) {

							l_sac[ind].start = ptrSelectedForm->satinOrAngle.sac[ind].start;
							l_sac[ind].finish = ptrSelectedForm->satinOrAngle.sac[ind].finish;
						}
					}
					mclp = (fPOINT*)&l_sac[ind];
					if (isclpx(closestFormToCursor)) {

						for (ind = 0; ind < ptrSelectedForm->lengthOrCount.clipCount; ind++) {

							mclp[ind].x = ptrSelectedForm->angleOrClipData.clip[ind].x;
							mclp[ind].y = ptrSelectedForm->angleOrClipData.clip[ind].y;
						}
					}
					clp = (fPOINT*)&mclp[ind];
					if (iseclpx(closestFormToCursor)) {

						for (ind = 0; ind < ptrSelectedForm->clipEntries; ind++) {

							clp[ind].x = ptrSelectedForm->borderClipData[ind].x;
							clp[ind].y = ptrSelectedForm->borderClipData[ind].y;
						}
					}
					ptx = (TXPNT*)&clp[ind];
					if (istx(closestFormToCursor))
					{
						ptxs = &txpnts[ptrSelectedForm->fillInfo.texture.index];
						for (ind = 0; ind < ptrSelectedForm->fillInfo.texture.count; ind++)
						{
							ptx[ind].line = ptxs[ind].line;
							ptx[ind].y = ptxs[ind].y;
						}
					}
					SetClipboardData(htclp, ptrThrEdClipVoid);
					if ((ptrSelectedForm->fillType || ptrSelectedForm->edgeType))
					{
						len = lenclp();
						hClip = RegisterClipboardFormat(pcdClipFormat);
						ptrClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len * sizeof(CLPSTCH) + 2);
						clipboardStitchData = deref(ptrClipVoid);
						ind = frmstrt;
						savclp(0, ind);
						clipboardStitchData[0].led = len;
						ind++;
						inf = 1;
						at = closestFormToCursor << 4;
						while (ind < hed.stitchCount) {

							if ((stitchBuffer[ind].attribute&FRMSK) == at && !(stitchBuffer[ind].attribute&NOTFRM))
								savclp(inf++, ind);
							ind++;
						}
						SetClipboardData(hClip, ptrClipVoid);
						ispcdclp();
					}
					CloseClipboard();
				} else {

					if (hed.stitchCount&&chkMap(GRPSEL)) {

						hClip = RegisterClipboardFormat(pcdClipFormat);
						rngadj();
						lowerLeftStitch.x = lowerLeftStitch.y = (float)1e30;
						for (ind = groupStartStitch; ind <= groupEndStitch; ind++) {

							if (stitchBuffer[ind].x < lowerLeftStitch.x)
								lowerLeftStitch.x = stitchBuffer[ind].x;
							if (stitchBuffer[ind].y < lowerLeftStitch.y)
								lowerLeftStitch.y = stitchBuffer[ind].y;
						}
						len = groupEndStitch - groupStartStitch + 1;
						inf = groupStartStitch;
						ptrClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len * sizeof(CLPSTCH) + 2);
						clipboardStitchData = deref(ptrClipVoid);
						savclp(0, inf);
						clipboardStitchData[0].led = len;
						inf++;
						for (ind = 1; ind < len; ind++)
							savclp(ind, inf++);
						SetClipboardData(hClip, ptrClipVoid);
						CloseClipboard();
					}
				}
			}
			CloseClipboard();
		}
	}
}

void delfstchs() {

	unsigned ind, ine = 0;;

	for (ind = 0; ind < hed.stitchCount; ind++) {

		if (stitchBuffer[ind].attribute&NOTFRM) {

			stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
			stitchBuffer[ine].x = stitchBuffer[ind].x;
			stitchBuffer[ine++].y = stitchBuffer[ind].y;
		} else {

			if (((stitchBuffer[ind].attribute&FRMSK) >> 4) != closestFormToCursor) {

				stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[ine].x = stitchBuffer[ind].x;
				stitchBuffer[ine++].y = stitchBuffer[ind].y;
			}
		}
	}
	hed.stitchCount = ine;
}

void cut() {

	duclip();
	if (selectedFormCount)
	{
		setMap(DELTO);
		delsfrms('s');
	} else
	{
		if (chkMap(FORMSEL)) {

			delfstchs();
			frmdel();
		} else {

			if (chkMap(GRPSEL))
				delstchm();
		}
	}
	coltab();
	rstAll();
	setMap(RESTCH);
}

void numWnd() {

	RECT l_msgRct;

	GetClientRect(hMsg, &l_msgRct);
	hgNum = CreateWindow(
		"STATIC",
		0,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		l_msgRct.bottom + 15,
		buttonWid3,
		buttonHi,
		hStch,
		NULL,
		hInst,
		NULL);
	msgIndex = 0;
	*msgbuf = 0;
}

void unmsg() {

	if (hMsg)
		noMsg();
}

void unpat() {

	if (rstMap(WASPAT)) {

		ShowWindow(hSped, FALSE);
		DestroyWindow(hSped);
		movStch();
		setMap(RESTCH);
	}
}
void delsmal(unsigned ind, unsigned fin) {

	unsigned	ine, at = closestFormToCursor << 4;
	unsigned	inf;
	long		dx, dy;
	double		l_siz = 1e99;

	savdo();
	if (chkMap(FORMSEL)) {

		ine = find1st();
		ind = ine + 1;
		while (ind<(unsigned)hed.stitchCount - 1 && l_siz>smallStitchLength) {

			if (!(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind].attribute&FRMSK) == at) {

				if (stitchBuffer[ind].attribute&KNOTMSK)
					ine = ind;
				else {

					dx = stitchBuffer[ind].x - stitchBuffer[ine].x;
					dy = stitchBuffer[ind].y - stitchBuffer[ine].y;
					l_siz = hypot(dx, dy);
				}
				ind++;
			}
		}
		if (ind != fin - 2) {

			ind--;
			ine = ind + 2;
			while (ine < (unsigned)hed.stitchCount - 1) {

				do {

					if (!(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind].attribute&FRMSK) == at && !(stitchBuffer[ind].attribute&KNOTMSK)) {

						dx = stitchBuffer[ine].x - stitchBuffer[ind].x;
						dy = stitchBuffer[ine++].y - stitchBuffer[ind].y;
						l_siz = hypot(dx, dy);
					} else
						ine++;
				} while (l_siz < smallStitchLength&&ine < hed.stitchCount);
				stitchBuffer[++ind].attribute = stitchBuffer[--ine].attribute;
				stitchBuffer[ind].x = stitchBuffer[ine].x;
				stitchBuffer[ind].y = stitchBuffer[ine++].y;
			}
			ind++;
			while (ine < hed.stitchCount) {

				stitchBuffer[ind].attribute = stitchBuffer[ine].attribute;
				stitchBuffer[ind].x = stitchBuffer[ine].x;
				stitchBuffer[ind++].y = stitchBuffer[ine++].y;
			}
			hed.stitchCount = ind;
			coltab();
		}
	} else {

		ine = ind + 1;
		sPnt.x = stitchBuffer[ind].x;
		sPnt.y = stitchBuffer[ind].y;
		for (inf = ine; inf < fin; inf++) {

			if (stitchBuffer[ine].attribute&KNOTMSK) {

				sPnt.x = stitchBuffer[ine].x;
				sPnt.y = stitchBuffer[ine].y;
				mvstch(ine++, inf);
			} else {

				dx = stitchBuffer[inf].x - sPnt.x;
				dy = stitchBuffer[inf].y - sPnt.y;
				l_siz = hypot(dx, dy);
				if (l_siz > smallStitchLength) {

					mvstch(ine++, inf);
					sPnt.x = stitchBuffer[inf].x;
					sPnt.y = stitchBuffer[inf].y;
				}
			}
		}
		while (inf < hed.stitchCount)
			mvstch(ine++, inf++);
		hed.stitchCount = ine;
		coltab();
	}
	rstAll();
	ritot(hed.stitchCount);
	lenCalc();
	setMap(RESTCH);
}

BOOL cmpstch(unsigned ind0, unsigned ind1) {

#if  __UseASM__
	_asm {
		mov		eax, ind0
		xor		ecx, ecx
		mov		cl, 12
		mul		ecx
		mov		ebx, eax
		add		ebx, offset stitchBuffer
		mov		eax, ind1
		mul		ecx
		add		eax, offset stitchBuffer
		mov		edx, eax
		mov		eax, [edx]
		cmp		eax, [ebx]
		jne		short ncmpx
		add		edx, 4
		add		ebx, 4
		mov		eax, [edx]
		cmp		eax, [ebx]
		jne		short ncmpx
		mov		al, 1
		jmp		short doscmpx
		ncmpx : xor		eax, eax
				doscmpx :
	}
#else
	if (stitchBuffer[ind0].x != stitchBuffer[ind1].x)
		return 0;

	return stitchBuffer[ind0].y == stitchBuffer[ind1].y;
#endif
}

void mvstch(unsigned dst, unsigned src) {

#if  __UseASM__
	_asm {
		mov		eax, dst
		xor		ecx, ecx
		mov		cl, 12
		mul		ecx
		mov		ebx, eax
		add		ebx, offset stitchBuffer
		mov		eax, src
		mul		ecx
		add		eax, offset stitchBuffer
		mov		cl, 4
		mov		edx, [eax]
		mov[ebx], edx
		add		ebx, ecx
		add		eax, ecx
		mov		edx, [eax]
		mov[ebx], edx
		add		ebx, ecx
		add		eax, ecx
		mov		edx, [eax]
		mov[ebx], edx
	}
#else
	stitchBuffer[dst] = stitchBuffer[src];
#endif
}

void ofstch(unsigned ind, TCHAR of) {

	stitchBuffer[opnt].x = stitchBuffer[ind].x + knotstp.x*of;
	stitchBuffer[opnt].y = stitchBuffer[ind].y + knotstp.y*of;
	stitchBuffer[opnt++].attribute = knotat;
}

void endknt(unsigned fin) {

	double		len;
	dPOINT		dif;
	unsigned	strt, ind;
	TCHAR*		tdat;

	strt = fin - 1;
	knotat = stitchBuffer[strt].attribute | KNOTMSK;
	do {

		dif.x = stitchBuffer[fin].x - stitchBuffer[strt].x;
		dif.y = stitchBuffer[fin].y - stitchBuffer[strt].y;
		len = hypot(dif.x, dif.y);
		strt--;
	} while (!len);
	if (chkMap(FILDIR))
		tdat = klstdat;
	else
		tdat = kendat;
	if (!(strt & 0x8000000)) {

		knotstp.x = 2 / len*dif.x;
		knotstp.y = 2 / len*dif.y;
		for (ind = 0; ind < 5; ind++)
			ofstch(fin, tdat[ind]);
		if (chkMap(FILDIR))
			ofstch(fin, 0);
	}
}

void strtknt(unsigned strt) {

	double		len;
	dPOINT		dif;
	unsigned	fin, ind;

	fin = strt + 1;
	do {

		dif.x = stitchBuffer[fin].x - stitchBuffer[strt].x;
		dif.y = stitchBuffer[fin].y - stitchBuffer[strt].y;
		len = hypot(dif.x, dif.y);
		fin++;
	} while (len < 2 && fin < hed.stitchCount);
	if (fin < hed.stitchCount) {

		knotat = stitchBuffer[strt].attribute | KNOTMSK;
		knotstp.x = 2 / len*dif.x;
		knotstp.y = 2 / len*dif.y;
		for (ind = 0; ind < 5; ind++)
			ofstch(strt, kstrtdat[ind]);
	}
}

BOOL aprxlen(double stitchLength, unsigned ulen) {

#define KNER 0.05

	if (stitchLength > ulen + KNER)
		return 1;
	if (stitchLength < ulen - KNER)
		return 1;
	return 0;
}

void fndknt() {

	unsigned	ind;
	BOOL		flg;

	if (hed.stitchCount > 4) {

		flg = 0;
		knotcnt = 0;
		for (ind = 0; ind < (unsigned)hed.stitchCount - 4; ind++) {

			if (stitchBuffer[ind].attribute&KNOTMSK) {

				if (!flg)
					knotab[knotcnt++] = ind;
			} else
				flg = 0;
		}
	} else knotcnt = 0;
}

void delknt() {

	unsigned	ind, ine;

	ine = 0;
	for (ind = 0; ind < hed.stitchCount; ind++) {

		if (!(stitchBuffer[ind].attribute&KNOTMSK))
			mvstch(ine++, ind);
	}
	hed.stitchCount = ine;
}

BOOL isknots() {

	unsigned ind;

	for (ind = 0; ind < hed.stitchCount; ind++) {

		if (stitchBuffer[ind].attribute&KNOTMSK)
			return 1;
	}
	return 0;
}

void delknot() {

	if (isknots()) {

		savdo();
		delknt();
		coltab();
		setMap(RESTCH);
	}
}

unsigned kjmp(unsigned strt) {

	while (strt<(unsigned)hed.stitchCount - 1 && stlen(strt)>KNOTLEN)
		mvstch(opnt++, strt++);
	strtknt(strt);
	return strt;
}

void setknt() {

	unsigned	ind;

	opnt = MAXSEQ;
	mvstch(opnt++, 0);
	strtknt(0);
	if (stlen(0) > KNOTLEN)
		ind = kjmp(1);
	else
		ind = 1;
	rstMap(FILDIR);
	while (ind < (unsigned)hed.stitchCount - 1) {

		mvstch(opnt++, ind);
		if (stlen(ind) > KNOTLEN) {

			endknt(ind);
			ind = kjmp(ind + 1);
			mvstch(opnt++, ind);
		}
		ind++;
	}
	setMap(FILDIR);
	endknt(ind);
	stitchBuffer[opnt - 1].attribute &= (~KNOTMSK);
	hed.stitchCount = opnt - MAXSEQ;
	mvstchs(0, MAXSEQ, hed.stitchCount);
}

unsigned srchknot(unsigned src) {

	while (knotab[xpnt] < src&&xpnt < knotcnt)
		xpnt++;
	xpnt--;
	if (((knotab[xpnt] > src) ? (knotab[xpnt] - src) : (src - knotab[xpnt])) < 5) {

		xpnt++;
		if (((knotab[xpnt] > src) ? (knotab[xpnt] - src) : (src - knotab[xpnt])) < 5)
			return 0;
		else
			return 2;
	} else {

		xpnt++;
		if (((knotab[xpnt] > src) ? (knotab[xpnt] - src) : (src - knotab[xpnt])) < 5)
			return 1;
		else
			return 3;
	}
}

void chkncol() {

	unsigned	src, col, tcol, cod;

	opnt = MAXSEQ;
	col = stitchBuffer[0].attribute&COLMSK;
	rstMap(FILDIR);
	for (src = 0; src < hed.stitchCount; src++) {

		tcol = stitchBuffer[src].attribute&COLMSK;
		if (tcol == col)
			mvstch(opnt++, src);
		else {

			col = stitchBuffer[src].attribute&COLMSK;
			cod = srchknot(src);
			if (cod & 1) {

				endknt(src - 1);
			}
			mvstch(opnt++, src);
			if (cod & 2)
				strtknt(src);
		}
	}
	hed.stitchCount = opnt - MAXSEQ;
	mvstchs(0, MAXSEQ, hed.stitchCount);
}

void setknots()
{
	if (hed.stitchCount)
	{
		savdo();
		delknt();
		setknt();
		fndknt();
		chkncol();
		coltab();
		setMap(RESTCH);
		ritot(hed.stitchCount);
	}
}

void lodbmp() {

	TCHAR*	pchr;

	if (bnam[0]) {

		DeleteObject(hBmp);
		ReleaseDC(hWnd, bitmapDC);
	}
	if (GetOpenFileName(&obn)) {

		untrace();
		pchr = strrchr(lbnam, '\\') + 1;
		// PCS file can only store a 16 character filename?
		strncpy_s(bnam, pchr, sizeof(bnam));
		defbNam();
		bfil();
		setMap(RESTCH);
	}
}

void duhbit(unsigned cod)
{
	CheckMenuItem(hMen, ID_HIDBIT, MF_BYCOMMAND | cod);
	CheckMenuItem(hMen, ID_HIDBITF, MF_BYCOMMAND | cod);
}

void hidbit() {

	if (toglMap(HIDMAP))
		duhbit(MF_UNCHECKED);
	else
		duhbit(MF_CHECKED);
	setMap(DUMEN);
	setMap(RESTCH);
}

void patdun() {

	rstMap(RUNPAT);
	KillTimer(hWnd, 0);
	setMap(WASPAT);
	movStch();
	setMap(RESTCH);
	tabmsg(IDS_END);
}

void drwlstch(unsigned fin) {

	unsigned	col, ind, prepnt;
	unsigned	flg;

	prepnt = runPoint;
	if (chkMap(HID)) {

		while ((stitchBuffer[runPoint].attribute&COLMSK) != actcol&&runPoint < fin - 1)
			runPoint++;
	}
	if (chkMap(ZUMED)) {

		ind = 1;
		while (runPoint < stitchesPerFrame + 1 && runPoint < fin - 2 && !stch2px(runPoint))
			runPoint++;
		prepnt = runPoint - 1;
		col = stitchBuffer[runPoint].attribute&COLMSK;
		flg = 1;
		while (ind < stitchesPerFrame + 1 && runPoint < fin - 2 && (stitchBuffer[runPoint].attribute&COLMSK) == col) {

			if (stch2px(runPoint)) {

				movieLine[ind].x = stitchSizePixels.x;
				movieLine[ind++].y = stitchSizePixels.y;
				if (flg) {

					flg = 0;
					if (stch2px(runPoint - 1)) {

						movieLine[0].x = movieLine[1].x;
						movieLine[0].y = movieLine[1].y;
					} else {

						movieLine[0].x = stitchSizePixels.x;
						movieLine[0].y = stitchSizePixels.y;
					}
				}
			}
			runPoint++;
		}
		if (prepnt == runPoint)
			runPoint++;
		if (!stch2px(runPoint)) {

			if ((stitchBuffer[runPoint].attribute&COLMSK) == col) {

				movieLine[ind].x = stitchSizePixels.x;
				movieLine[ind++].y = stitchSizePixels.y;
				runPoint++;
			}
		}
		SelectObject(StitchWindowDC, userPen[col]);
		Polyline(StitchWindowDC, movieLine, ind);
		if (!flg)
			runPoint--;
	} else {

		ind = 0;
		col = stitchBuffer[runPoint].attribute&COLMSK;
		SelectObject(StitchWindowDC, userPen[col]);
		while (ind < stitchesPerFrame && (runPoint + 1 < fin - 1) && ((stitchBuffer[runPoint].attribute&COLMSK) == col)) {

			stch2px1(runPoint++);
			movieLine[ind].x = stitchSizePixels.x;
			movieLine[ind++].y = stitchSizePixels.y;
		}
		runPoint--;
		Polyline(StitchWindowDC, movieLine, ind);
	}
	if ((stitchBuffer[runPoint + 1].attribute & 0xf) != col)
		runPoint++;
	ritnum(STR_NUMSEL, runPoint);
	if (runPoint + 3 > fin - 1)
		patdun();
}

void stchout() {

	if (chkMap(GRPSEL))
		drwlstch(groupEndStitch);
	else
		drwlstch(hed.stitchCount - 1);
}

unsigned duth(TCHAR* nam) {

	unsigned ind = strlen(nam);

	do ind--;
	while (tolower(nam[ind]) != 'h'&&ind);
	if (nam[ind - 1] == 't')
		return ind + 1;
	else
		return 0;
}

void duver(TCHAR* nam) {

	unsigned	ind;
	int			chr;

	ind = duth(nam);
	if (ind) {

		chr = tolower(nam[ind]) - 'r';
		nam[_MAX_PATH - 1] = 0;
		if (chr >= 0 && chr <= 3)
			strcpy_s(versionNames[chr], nam);
	}
}

void durit(void* src, unsigned cnt) {

#if  __UseASM__
	_asm {

		mov		esi, src
		mov		edi, opnt
		mov		ecx, cnt
		rep		movsb
		mov		opnt, edi
	}
#else
	memcpy((void *)opnt, src, cnt);
	opnt += cnt;
#endif
}

unsigned bufref() {

#if  __UseASM__
	_asm {

		mov		eax, opnt
		sub		eax, offset bseq
	}
#else
	return (unsigned)opnt - (unsigned)bseq;
#endif
}

void dubuf() {

	STRHED				sthed;
	unsigned			ind, ine, len, slen, elen;
	FRMHED*				theds;
	fPOINT*				tpnts;
	SATCON*				spnts;
	fPOINT*				epnts;
	unsigned			flind = 0;
	unsigned			slind = 0;
	unsigned			elind = 0;

	opnt = (unsigned)&bseq;
	sthed.headerType = 0x2746872;
	sthed.fileLength = hed.stitchCount * sizeof(fPOINTATTRIBUTE) + sizeof(STRHED) + 16;
	sthed.stitchCount = hed.stitchCount;
	sthed.hoopType = ini.hoopType;
	len = 0; slen = 0; elen = 0;
	strcpy_s(hedx.modifierName, ini.designerName);
	if (formIndex) {

		for (ind = 0; ind < formIndex; ind++) {

			len += formList[ind].sides;
			if (formList[ind].type == SAT)
				slen += formList[ind].satinGuideCount;
			if (isclp(ind))
				elen += formList[ind].lengthOrCount.clipCount;
			if (iseclp(ind))
				elen += formList[ind].clipEntries;
		}
	}
	sthed.formCount = formIndex;
	sthed.pointCount = len;
	sthed.dlineCount = slen;
	sthed.clipboardDataCount = elen;
	sthed.FormPointsLen = sizeof(STRHED) + hed.stitchCount * sizeof(fPOINTATTRIBUTE) + 164;
	sthed.dlineLen = sizeof(fPOINT)*len;
	sthed.clipboardDataLen = sizeof(fPOINT)*elen;
	durit(&sthed, sizeof(STRHED));
	hedx.auxFormat = ini.auxFileType;
	hedx.hoopSizeX = ini.hoopSizeX;
	hedx.hoopSizeY = ini.hoopSizeY;
	hedx.texturePointCount = txad;
	durit(&hedx, sizeof(STREX));
	durit(stitchBuffer, hed.stitchCount * sizeof(fPOINTATTRIBUTE));
	if (!bnam[0]) {

		for (ind = 0; ind < 16; ind++)
			bnam[ind] = 0;
	}
	durit(bnam, 16);
	durit(&backgroundColor, 4);
	durit(useCol, 64);
	durit(custCol, 64);
	for (ind = 0; ind < 16; ind++)
		msgbuf[ind] = thrdSiz[ind][0];
	durit(msgbuf, 16);
	if (formIndex) {

		theds = new FRMHED[formIndex];
		tpnts = new fPOINT[len + 1];
		spnts = new SATCON[slen];
		epnts = new fPOINT[elen + 1];
		for (ind = 0; ind < formIndex; ind++) {

			frmcpy(&theds[ind], &formList[ind]);
			theds[ind].vertices = (fPOINT*)(&tpnts[flind] - &tpnts[0]);
			for (ine = 0; ine < formList[ind].sides; ine++) {

				tpnts[flind].x = formList[ind].vertices[ine].x;
				tpnts[flind++].y = formList[ind].vertices[ine].y;
			}
			if (formList[ind].type == SAT) {

				theds[ind].satinOrAngle.sac = (SATCON*)(&spnts[slind] - &spnts[0]);
				theds[ind].satinGuideCount = formList[ind].satinGuideCount;
				for (ine = 0; ine < formList[ind].satinGuideCount; ine++) {

					spnts[slind].start = formList[ind].satinOrAngle.sac[ine].start;
					spnts[slind++].finish = formList[ind].satinOrAngle.sac[ine].finish;
				}
			}
			if (isclp(ind)) {

				theds[ind].angleOrClipData.clip = (fPOINT*)(&epnts[elind] - &epnts[0]);
				for (ine = 0; (ine < formList[ind].lengthOrCount.clipCount) && (elind < elen); ine++) {

					epnts[elind].x = formList[ind].angleOrClipData.clip[ine].x;
					epnts[elind++].y = formList[ind].angleOrClipData.clip[ine].y;
				}
			}
			if (iseclpx(ind)) {

				theds[ind].borderClipData = (fPOINT*)(&epnts[elind] - &epnts[0]);
				for (ine = 0; (ine < formList[ind].clipEntries) && ((elind < elen)); ine++) {
					epnts[elind].x = formList[ind].borderClipData[ine].x;
					epnts[elind++].y = formList[ind].borderClipData[ine].y;
				}
			}
		}
		durit(theds, formIndex * sizeof(FRMHED));
		durit(tpnts, len * sizeof(fPOINT));
		durit(spnts, slen * sizeof(SATCON));
		durit(epnts, elen * sizeof(fPOINT));
		durit(txpnts, txad * sizeof(TXPNT));
		delete[] theds;
		delete[] tpnts;
		delete[] spnts;
		delete[] epnts;
	}
}

void thrsav() {

	unsigned			ind, len;
	int					tind;
	unsigned long		wrot;
	//unsigned			flind=0;
	//unsigned			slind=0;
	//unsigned			elind=0;
	WIN32_FIND_DATA		fdat;
	HANDLE				hndl;
	TCHAR				nunam[_MAX_PATH];

	if (chkattr(filnam))
		return;
	if (!rstMap(IGNAM)) {

		hndl = FindFirstFile(genam, &fdat);
		ind = 0;
		if (hndl != INVALID_HANDLE_VALUE) {

			rstMap(CMPDO);
			for (ind = 0; ind < OLDVER; ind++)
				versionNames[ind][0] = 0;
			duver(fdat.cFileName);
			while (FindNextFile(hndl, &fdat))
				duver(fdat.cFileName);
			FindClose(hndl);
			DeleteFile(versionNames[OLDVER - 1]);
			for (tind = OLDVER - 2; tind >= 0; tind--) {

				if (versionNames[tind][0]) {

					versionNames[tind][_MAX_PATH - 1] = 0;
					strcpy_s(nunam, versionNames[tind]);
					len = duth(nunam);
					nunam[len] = tind + 's';
					MoveFile(versionNames[tind], nunam);
				}
			}
		}
	}
	hFil = CreateFile(thrnam, (GENERIC_WRITE), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hFil == INVALID_HANDLE_VALUE) {

		crmsg(thrnam);
		hFil = 0;
	} else {

		dubuf();
		WriteFile(hFil, bseq, bufref(), &wrot, 0);
		if (wrot != (unsigned long)bufref()) {

			sprintf_s(msgbuf, sizeof(msgbuf), "File Write Error: %s\n", thrnam);
			shoMsg(msgbuf);
		}
		CloseHandle(hFil);
	}
}

void setsped() {

	unsigned	len;
	double		tdub;

	if (!delay)
		delay = 1;
	tdub = (double)delay / 10;
	if (tdub < 10) {

		len = 100;
		stitchesPerFrame = (unsigned)len / tdub;
		if (stitchesPerFrame > 99)
			stitchesPerFrame = 99;
	} else {

		len = (unsigned)tdub;
		stitchesPerFrame = 2;
	}
	if (stitchesPerFrame < 2)
		stitchesPerFrame = 2;
	SetTimer(hWnd, 0, len, 0);
}

void f1del() {

	unsigned cod, ind, ine;

	if (chkMap(DELTO)) {

		cod = closestFormToCursor << FRMSHFT;
		ine = 0;
		for (ind = 0; ind < hed.stitchCount; ind++)
		{
			if ((stitchBuffer[ind].attribute&(FRMSK | NOTFRM)) != cod)
			{
				stitchBuffer[ine].x = stitchBuffer[ind].x;
				stitchBuffer[ine].y = stitchBuffer[ind].y;
				stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
				ine++;
			}
		}
		hed.stitchCount = ine;
	}
	deleclp(closestFormToCursor);
	delmclp(closestFormToCursor);
	delsac(closestFormToCursor);
	delflt(closestFormToCursor);
	deltx();
}

void frmdel() {

	unsigned ind, ine;
	unsigned stfrm, tind;

	fvars(closestFormToCursor);
	f1del();
	for (ind = closestFormToCursor; ind < (unsigned)formIndex; ind++)
		frmcpy(&formList[ind], &formList[ind + 1]);
	if (rstMap(DELTO)) {

		ine = 0;
		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (!(stitchBuffer[ind].attribute&NOTFRM)) {

				tind = (stitchBuffer[ind].attribute&FRMSK);
				stfrm = tind >> 4;
				if (stfrm > closestFormToCursor) {

					stitchBuffer[ind].attribute &= NFRMSK;
					stitchBuffer[ind].attribute |= (tind - 0x10);
				}
			}
		}
	} else {

		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (!(stitchBuffer[ind].attribute&NOTFRM)) {

				tind = (stitchBuffer[ind].attribute&FRMSK);
				stfrm = tind >> 4;
				if (stfrm == closestFormToCursor)
					stitchBuffer[ind].attribute &= (NFRMSK&NTYPMSK);
				if (stfrm > closestFormToCursor) {

					stitchBuffer[ind].attribute &= NFRMSK;
					stitchBuffer[ind].attribute |= (tind - 0x10);
				}
			}
		}
	}
	formIndex--;
	rstMap(FORMSEL);
}

void deltot() {

	strcpy_s(fildes, ini.designerName);
	formIndex = hed.stitchCount = fltad = clpad = satkad = txad = 0;
	rstMap(GMRK);
	rstAll();
	coltab();
	zumhom();
	strcpy_s(fildes, ini.designerName);
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRDBY], thrnam, fildes);
	SetWindowText(hWnd, msgbuf);
}

BOOL wastch() {

	unsigned ind;

	for (ind = 0; ind < hed.stitchCount; ind++) {

		if ((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor)
			return 1;
	}
	return 0;
}

BOOL frmstch()
{
	unsigned ind, cod;

	clRmap((formIndex >> 5) >> 1);
	for (ind = 0; ind < formIndex; ind++)
		setr(selectedFormList[ind]);
	for (ind = 0; ind < hed.stitchCount; ind++)
	{
		cod = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
		if (chkr(cod))
			return 1;
	}
	return 0;
}

void delet() {

	unsigned ind, ine;
	BOOL satflg;

	undat();
	satflg = 0;
	if (rstMap(FPSEL)) {

		savdo();
		fvars(closestFormToCursor);
		clRmap((sides >> 5) + 1);
		ine = selectedFormPoints.start;
		for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {

			setr(ine);
			ine = pdir(ine);
		}
		ine = 0;
		for (ind = 0; ind < sides; ind++) {

			if (!chkr(ind)) {

				currentFormVertices[ine].x = currentFormVertices[ind].x;
				currentFormVertices[ine].y = currentFormVertices[ind].y;
				ine++;
			}
		}
		ine = fltind(&currentFormVertices[ine]);
		ind = fltind(&currentFormVertices[ind]);
		while (ind < fltad) {

			formPoints[ine].x = formPoints[ind].x;
			formPoints[ine].y = formPoints[ind].y;
			ine++;
			ind++;
		}
		for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
			formList[ind].vertices -= (selectedFormPoints.pointCount + 1);
		fltad -= (selectedFormPoints.pointCount + 1);
		ptrSelectedForm->sides -= (selectedFormPoints.pointCount + 1);
		frmout(closestFormToCursor);
		if (ptrSelectedForm->type == SAT)
			satadj();
		refil();
		fndknt();
		setMap(RESTCH);
		return;
	}
	if (GetKeyState(VK_CONTROL)&GetKeyState(VK_SHIFT) & 0X8000)
		deltot();
	else {

		savdo();
		if (selectedFormCount) {

			if (frmstch())
			{
				setMap(DELSFRMS);
				tabmsg(IDS_DELFRM);
				okcan();
				tomsg();
			} else
				delsfrms(1);
			return;
		}
		if (chkMap(FORMSEL) && formIndex) {

			if (wastch()) {

				setMap(DELFRM);
				tabmsg(IDS_FDEL);
				okcan();
				tomsg();
			} else {

				if (formIndex) {

					rstMap(DELTO);
					frmdel();
					setMap(RESTCH);
				}
			}
			fndknt();
			return;
		}
		if (chkMap(SELBOX)) {

		selab:;
			if (hed.stitchCount > 2) {

				delstch1(closestPointIndex);
				if (!stch2px(closestPointIndex))
					movbox();
			} else {

				hed.stitchCount = 0;
				rstMap(SELBOX);
			}
			coltab();
			setfchk();
			fndknt();
			setMap(RESTCH);
			return;
		}
		if (chkMap(GRPSEL)) {

			delstchm();
			coltab();
			rstAll();
			setfchk();
			fndknt();
			setMap(RESTCH);
			return;
		}
		if (chkMap(FRMPSEL) || closfrm()) {

			ptrSelectedForm = &formList[closestFormToCursor];
			switch (ptrSelectedForm->type) {

			case LIN:

				if (ptrSelectedForm->fillType == CONTF) {

					if (closestVertexToCursor == ptrSelectedForm->angleOrClipData.sat.start || closestVertexToCursor == ptrSelectedForm->angleOrClipData.sat.finish) {

						delmfil(ptrSelectedForm->fillColor);
						ptrSelectedForm->fillType = 0;
						coltab();
						setMap(RESTCH);
						return;
					}
					if (ptrSelectedForm->angleOrClipData.sat.start > closestVertexToCursor)
						ptrSelectedForm->angleOrClipData.sat.start--;
					if (ptrSelectedForm->angleOrClipData.sat.finish > closestVertexToCursor)
						ptrSelectedForm->angleOrClipData.sat.finish--;
				}
				break;

			case SAT:

				if (closestVertexToCursor <= 1) {

					if (ptrSelectedForm->attribute&FRMEND) {

						if (ptrSelectedForm->wpar)
							ptrSelectedForm->wpar = 0;
						else
							ptrSelectedForm->attribute &= 0xfe;
						satflg = 1;
						goto deldun;
					}
				}
				if (wpar) {

					if (closestVertexToCursor == (unsigned)wpar || closestVertexToCursor == (unsigned)wpar + 1) {

						ptrSelectedForm->wpar = 0;
						satflg = 1;
						goto deldun;
					}
				}
				for (ind = 0; ind < ptrSelectedForm->satinGuideCount; ind++) {

					if (ptrSelectedForm->satinOrAngle.sac[ind].start == closestVertexToCursor || ptrSelectedForm->satinOrAngle.sac[ind].finish == closestVertexToCursor) {

						delcon(ind);
						satflg = 1;
						goto deldun;
					}
				}
			}
			delspnt();
		deldun:;
			if (closestFormToCursor > formIndex - 1)
				closestFormToCursor = formIndex - 1;
			if (formIndex) {

				frmout(closestFormToCursor);
				fvars(closestFormToCursor);
				refil();
			}
			coltab();
			setMap(RESTCH);
		}
		if (!satflg&&closPnt1(&closestPointIndex))
			goto selab;
	}
	fndknt();
}

void movi() {

	double tdub;

	rstAll();
	if (hed.stitchCount) {

		if (hMsg) {

			DestroyWindow(hMsg);
			hMsg = 0;
		}
		setMap(RUNPAT);
		if (chkMap(GRPSEL)) {

			rngadj();
			runPoint = groupStartStitch;
		} else
			runPoint = 0;
		movStch();
		if (!chkMap(WASPAT)) {

			hSped = CreateWindow(
				"SCROLLBAR",
				0,
				SBS_HORZ | WS_CHILD | WS_VISIBLE,
				buttonWid3,
				0,
				stchSiz.x,
				SCROLSIZ,
				hWnd,
				NULL,
				hInst,
				NULL);
		}
		if (chkMap(ZUMED))
			tdub = hed.stitchCount*zoomFactor*zoomFactor;
		else
			tdub = hed.stitchCount;
		if (!chkMap(WASPAT))
			delay = MOVITIM * 10000 / tdub;
		if (delay < MINDELAY)
			delay = MINDELAY;
		if (delay > MAXDELAY)
			delay = MAXDELAY;
		scrol.cbSize = sizeof(scrol);
		scrol.fMask = SIF_ALL;
		scrol.nMax = MAXDELAY;
		scrol.nMin = MINDELAY;
		scrol.nPage = 1;
		scrol.nPos = MAXDELAY - delay;
		SetScrollInfo(hSped, SB_CTL, &scrol, TRUE);
		FillRect(StitchWindowDC, &stitchWindowClientRect, hBackgroundBrush);
		setsped();
	}
}

#define CLPBUG 0

void redclp() {

	int		ind, playcod;

	playcod = activeLayer << LAYSHFT;
	ptrClipVoid = GlobalLock(hClipMem);
	clipboardStitchData = (CLPSTCH*)ptrClipVoid;
	if (ptrClipVoid) {
		clipboardStitchCount = clipboardStitchData[0].led;
		clipBuffer[0].x = clipboardStitchData[0].x + (float)clipboardStitchData[0].fx / 256;
		clipBuffer[0].y = clipboardStitchData[0].y + (float)clipboardStitchData[0].fy / 256;
		clipBuffer[0].attribute = 0;

#if CLPBUG

		sprintf_s(msgbuf, sizeof(msgbuf), "ind: 0 x: %6.2f,y: %6.2f\n", clipBuffer[0].x, clipBuffer[0].y);
		OutputDebugString(msgbuf);
#endif
		clipboardRect.left = clipboardRect.right = clipBuffer[0].x;
		clipboardRect.bottom = clipboardRect.top = clipBuffer[0].y;
		for (ind = 1; ind < (long)clipboardStitchCount; ind++) {

			clipBuffer[ind].x = clipboardStitchData[ind].x + (float)clipboardStitchData[ind].fx / 256;
			clipBuffer[ind].y = clipboardStitchData[ind].y + (float)clipboardStitchData[ind].fy / 256;
			clipBuffer[ind].attribute = (clipboardStitchData[ind].led & 0xf) | playcod;

#if CLPBUG

			sprintf_s(msgbuf, sizeof(msgbuf), "ind: %d x: %6.2f,y: %6.2f\n", index, clipBuffer[index].x, clipBuffer[index].y);
			OutputDebugString(msgbuf);
#endif
			if (clipBuffer[ind].x < clipboardRect.left)
				clipboardRect.left = clipBuffer[ind].x;
			if (clipBuffer[ind].x > clipboardRect.right)
				clipboardRect.right = clipBuffer[ind].x;
			if (clipBuffer[ind].y < clipboardRect.bottom)
				clipboardRect.bottom = clipBuffer[ind].y;
			if (clipBuffer[ind].y > clipboardRect.top)
				clipboardRect.top = clipBuffer[ind].y;
		}
		clipBuffer[0].attribute = actcol | playcod;
		clipboardRectSize.cx = clipboardRect.right - clipboardRect.left;
		clipboardRectSize.cy = clipboardRect.top - clipboardRect.bottom;
		GlobalUnlock(hClipMem);
		if (clipboardRect.left || clipboardRect.bottom) {

			for (ind = 0; ind < (int)clipboardStitchCount; ind++) {

				clipBuffer[ind].x -= clipboardRect.left;
				clipBuffer[ind].y -= clipboardRect.bottom;
			}
			clipboardRect.top -= clipboardRect.bottom;
			clipboardRect.right -= clipboardRect.left;
			clipboardRect.bottom = clipboardRect.left = 0;
		}
	}
}

unsigned nxtcrnr(unsigned ind) {

#if  __UseASM__
	_asm {

		mov		eax, ind
		inc		eax
		and		al, 3
	}
#else
	return (ind + 1) & 3;
#endif
}

void drwmrk(HDC dc) {

	POINT pxpnt;
	POINT mlin[2];

	sCor2px(zoomMarkPoint, &pxpnt);
	SelectObject(dc, zoomMarkPen);
	SetROP2(dc, R2_XORPEN);
	mlin[0].x = pxpnt.x - 6;
	mlin[0].y = pxpnt.y - 6;
	mlin[1].x = pxpnt.x + 6;
	mlin[1].y = pxpnt.y + 6;
	Polyline(dc, mlin, 2);
	mlin[0].x = pxpnt.x - 6;
	mlin[0].y = pxpnt.y + 6;
	mlin[1].x = pxpnt.x + 6;
	mlin[1].y = pxpnt.y - 6;
	Polyline(dc, mlin, 2);
	SetROP2(dc, R2_COPYPEN);
}

void vubak() {

	unsigned	ind;
	long		dx, dy, vloc;

	if (hFil || chkMap(THUMSHO)) {

		setMap(ZUMED);
		movStch();
		FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
		dx = (stitchWindowClientRect.right >> 1);
		dy = (stitchWindowClientRect.bottom >> 1);
		for (ind = 0; ind < OLDVER; ind++) {

			if (ind & 2)
				vloc = dy;
			else
				vloc = 0;
			hvu[ind] = CreateWindow(
				"STATIC",
				"",
				SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
				dx*(ind & 1) + buttonWid3,
				vloc,
				dx,
				dy,
				hWnd,
				NULL,
				hInst,
				NULL);
		}
		setMap(BAKSHO);
	}
}

void getbak() {

	TCHAR*		pchr;

	if (chkMap(THUMSHO)) {

		if (thumsel[fileVersionIndex]) {

			if (chkMap(RBUT)) {

				strcpy_s(insnam, thumsel[fileVersionIndex]);
				setMap(IGNORINS);
				unthum();
				setMap(FRMOF);
				insfil();
				if (GetKeyState(VK_SHIFT) & 0X8000) {

					rstMap(INSFIL);
					rstMap(FRMOF);
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
			} else {

				strcpy_s(filnam, defDir);
				pchr = &filnam[strlen(filnam) - 1];
				if (pchr[0] != '\\') {

					pchr[1] = '\\';
					pchr[2] = 0;
				}
				strcat_s(filnam, thumsel[fileVersionIndex]);
				setMap(REDOLD);
				nuFil();
			}
		} else
			unthum();
	}
}

void rebak() {

	unsigned	ind;
	TCHAR		tnaml[_MAX_PATH];
	TCHAR		tnamx[_MAX_PATH];

	for (ind = 0; ind < OLDVER; ind++)
		DestroyWindow(hvu[ind]);
	strcpy_s(tnaml, thrnam);
	strcpy_s(tnamx, thrnam);
	ind = duth(tnaml);
	tnaml[ind] = fileVersionIndex + 's';
	tnamx[ind] = 'x';
	MoveFile(thrnam, tnamx);
	MoveFile(tnaml, thrnam);
	MoveFile(tnamx, tnaml);
	strcpy_s(filnam, thrnam);
	setMap(REDOLD);
	nuFil();
	DeleteFile(tnamx);
}

void thumbak() {

	unsigned ind;

	for (ind = 0; ind < OLDVER; ind++)
		DestroyWindow(hvu[ind]);
	getbak();
}

void movbak(TCHAR src, TCHAR dst) {

	TCHAR	strsrc[_MAX_PATH];
	TCHAR	strdst[_MAX_PATH];

	unsigned ind = duth(thrnam);

	strcpy_s(strsrc, thrnam);
	strcpy_s(strdst, thrnam);
	strsrc[ind] = (TCHAR)src;
	strdst[ind] = (TCHAR)dst;
	DeleteFile(strdst);
	MoveFile(strsrc, strdst);
}

void purg() {

	TCHAR		tnam[_MAX_PATH];
	unsigned	pind, ind;

	if (hFil) {

		strcpy_s(tnam, thrnam);
		pind = duth(tnam);
		for (ind = 1; ind < 6; ind++) {

			tnam[pind] = (TCHAR)ind + 'r';
			DeleteFile(tnam);
		}
	}
}

void purgdir() {

	setMap(PRGMSG);
	sprintf_s(msgbuf, sizeof(msgbuf), "Delete all backups in %s\n", defDir);
	shoMsg(msgbuf);
	okcan();
}

void deldir() {

	unsigned			ind;
	TCHAR				tnam[_MAX_PATH];
	TCHAR*				pchr;
	WIN32_FIND_DATA		fdat;
	HANDLE				hndl;

	unmsg();
	tabmsg(IDS_BAKDEL);
	strcpy_s(tnam, defDir);
	pchr = &tnam[strlen(tnam)];
	strcpy_s(pchr, sizeof(tnam) - (pchr - tnam), "\\*.th0");
	for (ind = 1; ind < 6; ind++) {

		pchr[5] = (TCHAR)ind + 'r';
		hndl = FindFirstFile(tnam, &fdat);
		if (hndl != INVALID_HANDLE_VALUE) {

			DeleteFile(fdat.cFileName);
			while (FindNextFile(hndl, &fdat))
				DeleteFile(fdat.cFileName);
		}
		FindClose(hndl);
	}
	unmsg();
	tabmsg(IDS_BAKDT);
}

BOOL chkwnd(HWND hwind) {

	RECT	trct;

	GetWindowRect(hwind, &trct);
	if (msg.pt.x >= trct.left&&msg.pt.x <= trct.right
		&&msg.pt.y >= trct.top&&msg.pt.y <= trct.bottom)
		return 1;
	else
		return 0;
}

BOOL chkok() {

	return chkwnd(hok);
}

void mv2f() {

	unsigned	at, lo, hi, ind;

	if (rstMap(FORMSEL)) {

		savdo();
		lo = 0;
		hi = MAXPCS >> 1;
		at = closestFormToCursor << 4;
		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (!(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind].attribute&FRMSK) == at) {

				stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[lo].x = stitchBuffer[ind].x;
				stitchBuffer[lo++].y = stitchBuffer[ind].y;
			} else {

				stitchBuffer[hi].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[hi].x = stitchBuffer[ind].x;
				stitchBuffer[hi++].y = stitchBuffer[ind].y;
			}
		}
		ind = MAXPCS >> 1;
		while (ind < hi) {

			stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
			stitchBuffer[lo].x = stitchBuffer[ind].x;
			stitchBuffer[lo++].y = stitchBuffer[ind++].y;
		}
		coltab();
		setMap(RESTCH);
	} else {

		if (rstMap(GRPSEL)) {

			savdo();
			rngadj();
			hi = MAXPCS >> 1;
			lo = 0;
			for (ind = 0; ind < groupStartStitch; ind++) {

				stitchBuffer[hi].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[hi].x = stitchBuffer[ind].x;
				stitchBuffer[hi++].y = stitchBuffer[ind].y;
			}
			for (ind = groupEndStitch; ind < hed.stitchCount; ind++) {

				stitchBuffer[hi].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[hi].x = stitchBuffer[ind].x;
				stitchBuffer[hi++].y = stitchBuffer[ind].y;
			}
			for (ind = groupStartStitch; ind < groupEndStitch; ind++) {

				stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[lo].x = stitchBuffer[ind].x;
				stitchBuffer[lo++].y = stitchBuffer[ind].y;
			}
			for (ind = MAXPCS >> 1; ind < hi; ind++) {

				stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[lo].x = stitchBuffer[ind].x;
				stitchBuffer[lo++].y = stitchBuffer[ind].y;
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void mv2b() {

	unsigned	at, lo, hi, ind;

	if (rstMap(FORMSEL)) {

		savdo();
		lo = 0;
		hi = MAXPCS >> 1;
		at = closestFormToCursor << 4;
		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (!(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind].attribute&FRMSK) == at) {

				stitchBuffer[hi].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[hi].x = stitchBuffer[ind].x;
				stitchBuffer[hi++].y = stitchBuffer[ind].y;
			} else {

				stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[lo].x = stitchBuffer[ind].x;
				stitchBuffer[lo++].y = stitchBuffer[ind].y;
			}
		}
		ind = MAXPCS >> 1;
		while (ind < hi) {

			stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
			stitchBuffer[lo].x = stitchBuffer[ind].x;
			stitchBuffer[lo++].y = stitchBuffer[ind++].y;
		}
		coltab();
		setMap(RESTCH);
	} else {

		if (rstMap(GRPSEL)) {

			savdo();
			rngadj();
			hi = MAXPCS >> 1;
			for (ind = groupStartStitch; ind < groupEndStitch; ind++) {

				stitchBuffer[hi].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[hi].x = stitchBuffer[ind].x;
				stitchBuffer[hi++].y = stitchBuffer[ind].y;
			}
			lo = groupStartStitch;
			for (ind = groupEndStitch; ind < hed.stitchCount; ind++) {

				stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[lo].x = stitchBuffer[ind].x;
				stitchBuffer[lo++].y = stitchBuffer[ind].y;
			}
			for (ind = MAXPCS >> 1; ind < hi; ind++) {

				stitchBuffer[lo].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[lo].x = stitchBuffer[ind].x;
				stitchBuffer[lo++].y = stitchBuffer[ind].y;
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void infadj(float* px, float* py) {

	if (!_finite(*px)) {

		if (*px > unzoomedRect.x)
			*px = unzoomedRect.x;
		else {

			if (*px < 0)
				*px = 0;
		}
	}
	if (!_finite(*py)) {

		if (*py > unzoomedRect.y)
			*py = unzoomedRect.y;
		else {

			if (*py < 0)
				*py = 0;
		}
	}
}

void delinf() {

	unsigned ind;

	for (ind = 0; ind < hed.stitchCount; ind++)
		infadj(&stitchBuffer[ind].x, &stitchBuffer[ind].y);
	for (ind = 0; ind < fltad; ind++)
		infadj(&formPoints[ind].x, &formPoints[ind].y);
}

void chkrng(fPOINT* rsiz) {

	unsigned	ind, ine = 0;

	delinf();
	rsiz->x = unzoomedRect.x;
	rsiz->y = unzoomedRect.y;
	if (formIndex) {

		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (stitchBuffer[ind].attribute&NOTFRM
				|| ((stitchBuffer[ind].attribute&TYPMSK) && ((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT < formIndex))) {
				stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
				stitchBuffer[ine].x = stitchBuffer[ind].x;
				stitchBuffer[ine].y = stitchBuffer[ind].y;
				if (stitchBuffer[ine].x > rsiz->x)
					stitchBuffer[ine].x = rsiz->x - 1;
				if (stitchBuffer[ine].x < 0)
					stitchBuffer[ine].x = 0;
				if (stitchBuffer[ine].y > rsiz->y)
					stitchBuffer[ine].y = rsiz->y - 1;
				if (stitchBuffer[ine].y < 0)
					stitchBuffer[ine].y = 0;
				ine++;
			}
		}
		hed.stitchCount = ine;
	} else {

		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (stitchBuffer[ind].x > rsiz->x)
				stitchBuffer[ind].x = rsiz->x - 1;
			if (stitchBuffer[ind].x < 0)
				stitchBuffer[ind].x = 0;
			if (stitchBuffer[ind].y > rsiz->y)
				stitchBuffer[ind].y = rsiz->y - 1;
			if (stitchBuffer[ind].y < 0)
				stitchBuffer[ind].y = 0;
		}
	}
}

void ritmov() {

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, formPen);
	if (closestVertexToCursor) {

		if (closestVertexToCursor == (unsigned)ptrSelectedForm->sides - 1 && ptrSelectedForm->type == LIN)
			Polyline(StitchWindowDC, mvlin, 2);
		else
			Polyline(StitchWindowDC, mvlin, 3);
	} else {

		mvlin[2].x = flin[1].x;
		mvlin[2].y = flin[1].y;
		if (ptrSelectedForm->type == LIN)
			Polyline(StitchWindowDC, &mvlin[1], 2);
		else
			Polyline(StitchWindowDC, mvlin, 3);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unmov() {

	if (rstMap(SHOMOV))
		ritmov();
}

void duprct() {

	unsigned ind, strt;

	strt = selectedFormPoints.start;
	selectedPointsRect.left = selectedPointsRect.right = currentFormVertices[strt].x;
	selectedPointsRect.top = selectedPointsRect.bottom = currentFormVertices[strt].y;
	strt = pdir(strt);
	for (ind = 0; ind < selectedFormPoints.pointCount; ind++) {

		if (currentFormVertices[strt].x < selectedPointsRect.left)
			selectedPointsRect.left = currentFormVertices[strt].x;
		if (currentFormVertices[strt].x > selectedPointsRect.right)
			selectedPointsRect.right = currentFormVertices[strt].x;
		if (currentFormVertices[strt].y < selectedPointsRect.bottom)
			selectedPointsRect.bottom = currentFormVertices[strt].y;
		if (currentFormVertices[strt].y > selectedPointsRect.top)
			selectedPointsRect.top = currentFormVertices[strt].y;
		strt = pdir(strt);
	}
}

void setpsel() {

	unpsel();
	duprct();
	sRct2px(selectedPointsRect, &selectedPixelsRect);
	rct2sel(selectedPixelsRect, selectedPointsRectangle);
	sfCor2px(currentFormVertices[selectedFormPoints.finish], &pselfin);
	setMap(SHOPSEL);
	dupsel(StitchWindowDC);
	setMap(FPSEL);
}

void rotfn() {

	unsigned	ind, ine;
	double		len;
	double		tang;
	dPOINT		trot;
	unsigned	dst;

	savdo();
	if (chkMap(FPSEL)) {

		fvars(closestFormToCursor);
		ine = selectedFormPoints.start;
		for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {

			rotflt(&currentFormVertices[ine]);
			ine = pdir(ine);
		}
		frmout(closestFormToCursor);
		setpsel();
		refil();
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {

		for (ind = 0; ind < fltad; ind++)
			rotflt(&formPoints[ind]);
		for (ind = 0; ind < hed.stitchCount; ind++)
			rotstch(&stitchBuffer[ind]);
		for (ind = 0; ind < formIndex; ind++)
			frmout(ind);
		selal();
		return;
	}
	if (rstMap(FRMSROT)) {

		tang = rotationAngle;
		trot.x = rotcntr.x;
		trot.y = rotcntr.y;
		for (ind = 0; ind < selectedFormCount; ind++) {

			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			for (ine = 0; ine < sides; ine++)
				rotflt(&currentFormVertices[ine]);
			frmout(closestFormToCursor);
			refilfn();
			rotationAngle = tang;
			rotcntr.x = trot.x;
			rotcntr.y = trot.y;
		}
		setMap(RESTCH);
	} else {

		if (rstMap(FRMROT)) {

			fvars(closestFormToCursor);
			for (ind = 0; ind < sides; ind++)
				rotflt(&currentFormVertices[ind]);
			frmout(closestFormToCursor);
			refil();
			setMap(RESTCH);
		} else {

			len = groupEndStitch - groupStartStitch + 1;
			dst = 0;
			for (ind = groupStartStitch; ind <= groupEndStitch; ind++)
				rotstch(&stitchBuffer[ind]);
			rngadj();
			selin(groupStartStitch, groupEndStitch, StitchWindowDC);
			setMap(RESTCH);
		}
	}
}

void rotfns() {

	savdo();
	rotfn();
}

void nulayr(unsigned play) {

	activeLayer = play;
	layerIndex = play >> 1;
	ladj();
	if (activeLayer) {

		if (chkMap(FORMSEL) && ((unsigned)((formList[closestFormToCursor].attribute&FRMLMSK) >> 1) != activeLayer))
			rstMap(FORMSEL);
		rstMap(GRPSEL);
		if (chkMap(SELBOX)) {

			if (activeLayer != ((stitchBuffer[closestPointIndex].attribute&LAYMSK) >> LAYSHFT) + 1)
				rstMap(SELBOX);
		}
		selectedFormCount = 0;
	}
	setMap(RESTCH);
}

BOOL iselpnt() {

	unsigned	ind, ine = 0;
	double		len, minlen = 1e99;
	POINT		tpnt;

	tpnt.x = msg.pt.x - stitchWindowOrigin.x;
	tpnt.y = msg.pt.y - stitchWindowOrigin.y;
	for (ind = 0; ind < 9; ind++) {

		len = hypot(tpnt.x - formOutlineRectangle[ind].x, tpnt.y - formOutlineRectangle[ind].y);
		if (len < minlen) {

			minlen = len;
			ine = ind;
		}
	}
	if (minlen < CLOSENUF) {

		selectedFormControlVertex = ine;
		return 1;
	} else
		return 0;
}

void rtrclpfn() {

	unsigned ind, len = 0;

	if (OpenClipboard(hWnd)) {

		fvars(closestFormToCursor);
		if (iseclp(closestFormToCursor)) {

			len = ptrSelectedForm->clipEntries;
			oclp(ptrSelectedForm->borderClipData, len);
		} else {

			if (isclp(closestFormToCursor)) {

				len = ptrSelectedForm->lengthOrCount.clipCount;
				oclp(ptrSelectedForm->angleOrClipData.clip, len);
			}
		}
		if (len) {

			lowerLeftStitch.x = lowerLeftStitch.y = 0;
			EmptyClipboard();
			hClip = RegisterClipboardFormat(pcdClipFormat);
			ptrClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len * sizeof(CLPSTCH) + 2);
			clipboardStitchData = deref(ptrClipVoid);
			rtclpfn(0, 0);
			clipboardStitchData[0].led = len;
			for (ind = 1; ind < len; ind++)
				rtclpfn(ind, ind);
			SetClipboardData(hClip, ptrClipVoid);
			CloseClipboard();
		}
	}
}

BOOL chkbig() {

	unsigned	ind;
	double		len, minlen = 1e99;
	POINT		tpnt;

	tpnt.x = msg.pt.x - stitchWindowOrigin.x;
	tpnt.y = msg.pt.y - stitchWindowOrigin.y;
	for (ind = 0; ind < 9; ind++) {

		len = hypot(tpnt.x - selectedFormsLine[ind].x, tpnt.y - selectedFormsLine[ind].y);
		if (len < minlen) {

			minlen = len;
			selectedFormControlVertex = ind;
		}
	}
	for (ind = 0; ind < 4; ind++) {

		flin[ind].x = selectedFormsLine[ind << 1].x;
		flin[ind].y = selectedFormsLine[ind << 1].y;
	}
	flin[4].x = flin[0].x;
	flin[4].y = flin[0].y;
	if (minlen < CLOSENUF) {

		for (ind = 0; ind < 4; ind++) {

			sizlin[ind].x = selectedFormsLine[ind << 1].x;
			sizlin[ind].y = selectedFormsLine[ind << 1].y;
		}
		sizlin[4].x = sizlin[0].x;
		sizlin[4].y = sizlin[0].y;
		if (selectedFormControlVertex & 1)
			setMap(STRTCH);
		else {

			setMap(EXPAND);
			xyRatio = (double)(selectedFormsRectangle.right - selectedFormsRectangle.left) / (selectedFormsRectangle.bottom - selectedFormsRectangle.top);
		}
		selectedFormControlVertex >>= 1;
		setMap(SHOSTRTCH);
		strtchbox();
		return 1;
	}
	if (tpnt.x >= selectedFormsRectangle.left&&tpnt.x <= selectedFormsRectangle.right
		&&tpnt.y >= selectedFormsRectangle.top&&tpnt.y <= selectedFormsRectangle.bottom) {

		bigsiz.x = selectedFormsRectangle.right - selectedFormsRectangle.left;
		bigsiz.y = selectedFormsRectangle.bottom - selectedFormsRectangle.top;
		setMap(MOVFRMS);
		fmovdif.x = tpnt.x - selectedFormsRectangle.left;
		fmovdif.y = tpnt.y - selectedFormsRectangle.top;
		setMap(SHOSTRTCH);
		strtchbox();
		return 1;
	}
	return 0;
}

void delfre() {

	unsigned ind, ine = 0;

	savdo();
	for (ind = 0; ind < hed.stitchCount; ind++) {

		if (!(stitchBuffer[ind].attribute&NOTFRM)) {

			stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
			stitchBuffer[ine].x = stitchBuffer[ind].x;
			stitchBuffer[ine++].y = stitchBuffer[ind].y;
		}
	}
	hed.stitchCount = ine;
	coltab();
	setMap(RESTCH);
}

void setmov() {

	if (chkMap(SELBOX)) {

		movpnt = closestPointIndex;
		setMap(MOVSET);
		setMap(RESTCH);
	}
}

void dufsel()
{
	unsigned strt, fin;

	if (clofinx > closestFormToCursor)
	{
		strt = closestFormToCursor;
		fin = clofinx;
	} else
	{
		strt = clofinx;
		fin = closestFormToCursor;
	}
	selectedFormCount = 0;
	while (strt <= fin)
	{
		selectedFormList[selectedFormCount] = strt;
		strt++;
		selectedFormCount++;
	}
}

void selup() {

	unsigned at;

	if (GetKeyState(VK_SHIFT) & 0X8000) {

		rstMap(SELBOX);
		if (rstMap(FORMSEL)) {

			if (closestFormToCursor < formIndex - 1) {

				selectedFormList[0] = closestFormToCursor;
				clofinx = closestFormToCursor + 1;
				selectedFormList[1] = clofinx;
				selectedFormCount = 2;
			} else
				return;
		} else {

			if (selectedFormCount)
			{
				if (clofinx < formIndex - 1)
				{
					clofinx++;
					dufsel();
				}
			} else
				setMap(FORMSEL);
		}
		setMap(RESTCH);
	} else {

		if (chkMap(SELBOX)) {

			unbox();
			at = stitchBuffer[closestPointIndex].attribute&ATMSK;
			while (closestPointIndex < (unsigned)hed.stitchCount - 1 && (stitchBuffer[closestPointIndex].attribute&ATMSK) == at)
				closestPointIndex++;
			stch2px(closestPointIndex);
			dubox();
		} else {

			if (formIndex) {

				if (setMap(FORMSEL)) {

					if (closestFormToCursor < (unsigned)formIndex - 1)
						closestFormToCursor++;
				} else
					closestFormToCursor = 0;
				ritnum(STR_NUMFORM, closestFormToCursor);
				setMap(RESTCH);
			}
		}
	}
}

void seldwn() {

	unsigned at;

	if (GetKeyState(VK_SHIFT) & 0X8000) {

		rstMap(SELBOX);
		if (rstMap(FORMSEL)) {

			if (closestFormToCursor) {

				selectedFormList[0] = closestFormToCursor;
				clofinx = closestFormToCursor - 1;
				selectedFormList[1] = clofinx;
				selectedFormCount = 2;
			} else
				return;
		} else {

			if (selectedFormCount)
			{
				if (clofinx)
				{
					clofinx--;
					dufsel();
				}
			} else
				setMap(FORMSEL);
		}
		setMap(RESTCH);
	} else {

		if (chkMap(SELBOX)) {

			unbox();
			at = stitchBuffer[closestPointIndex].attribute&ATMSK;
			while (closestPointIndex && (stitchBuffer[closestPointIndex].attribute&ATMSK) == at)
				closestPointIndex--;
			stch2px(closestPointIndex);
			dubox();
		} else {

			if (formIndex) {

				if (setMap(FORMSEL)) {

					if (closestFormToCursor)
						closestFormToCursor--;
				} else
					closestFormToCursor = formIndex - 1;
				ritnum(STR_NUMFORM, closestFormToCursor);
				setMap(RESTCH);
			}
		}
	}
}

void mvstchs(unsigned dst, unsigned src, unsigned cnt) {

#if  __UseASM__
	_asm {

		xor		ecx, ecx
		mov		cl, 12
		mov		eax, dst
		mul		ecx
		mov		edi, eax
		add		edi, offset stitchBuffer
		mov		eax, src
		mul		ecx
		mov		esi, eax
		add		esi, offset stitchBuffer
		mov		cl, 3
		mov		eax, cnt
		mul		ecx
		mov		ecx, eax
		rep		movsd
	}
#else
	memcpy(stitchBuffer + dst, stitchBuffer + src, cnt * sizeof(*stitchBuffer));
#endif
}

BOOL movstchs(unsigned dst, unsigned strt, unsigned fin) {

	unsigned cnt, ind;
	unsigned dind = MAXPCS >> 1;

	if (dst < (unsigned)hed.stitchCount - 1)
		dst++;
	if (strt > fin) {

		ind = strt;
		strt = fin;
		fin = ind;
	}
	if (dst >= strt&&dst < fin) {

		tabmsg(IDS_DST1);
		return 0;
	}
	if (dst < strt) {

		cnt = fin - strt;
		mvstchs(dind, strt, cnt);
		dind += cnt;
		cnt = strt - dst;
		mvstchs(dind, dst, cnt);
		dind += cnt;
		mvstchs(dst, MAXPCS >> 1, dind - (MAXPCS >> 1));
	} else {

		cnt = dst - fin;
		mvstchs(dind, fin, cnt);
		dind += cnt;
		cnt = fin - strt;
		mvstchs(dind, strt, cnt);
		dind += cnt;
		mvstchs(strt, MAXPCS >> 1, dind - (MAXPCS >> 1));
	}
	return 1;
}

void movmrk() {

	if (chkMap(MOVSET)) {

		if (chkMap(GRPSEL)) {

			rngadj();
			if (movstchs(movpnt, groupStartStitch, groupEndStitch)) {

				coltab();
				setMap(RESTCH);
			}
		} else {

			if (chkMap(SELBOX)) {

				if (movstchs(movpnt, closestPointIndex, closestPointIndex + 1)) {

					setMap(RESTCH);
					coltab();
				}
			}
		}
	}
}

void vuthrds() {

	if (GetMenuState(hvumen, ID_VUTHRDS, MF_BYCOMMAND)&MF_CHECKED) {

		CheckMenuItem(hMen, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		rstMap(THRDS);
	} else {

		CheckMenuItem(hMen, ID_VUTHRDS, MF_BYCOMMAND | MF_CHECKED);
		setMap(THRDS);
	}
	setMap(RESTCH);
}

void vuselthr() {

	if (GetMenuState(hvumen, ID_VUSELTHRDS, MF_BYCOMMAND)&MF_CHECKED) {

		CheckMenuItem(hMen, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		rstMap(COL);
	} else {

		CheckMenuItem(hMen, ID_VUSELTHRDS, MF_BYCOMMAND | MF_CHECKED);
		setMap(COL);
	}
	setMap(RESTCH);
}

void colchk() {

	unsigned col, cind, ind;

	col = stitchBuffer[0].attribute&COLMSK;
	cind = 0;
	for (ind = 0; ind < hed.stitchCount; ind++) {

		if (col != (stitchBuffer[ind].attribute&COLMSK)) {

			if ((ind - cind == 1) && (cind))
				stitchBuffer[ind - 1].attribute = stitchBuffer[ind].attribute&NCOLMSK | (stitchBuffer[cind - 1].attribute&COLMSK);
			col = stitchBuffer[ind].attribute&COLMSK;
			cind = ind;
		}
	}
}

unsigned makbig(unsigned strt, unsigned fin) {

	unsigned	dst, src, cnt, ine, adcnt = 0, at;
	fPOINTATTRIBUTE*	tstchs0;
	fPOINTATTRIBUTE*	tstchs1;
	double		len;
	dPOINT		dif, stp, pnt;

	dst = MAXPCS >> 1;
	for (src = strt; src < (unsigned)fin - 1; src++) {

		dif.x = stitchBuffer[src + 1].x - stitchBuffer[src].x;
		dif.y = stitchBuffer[src + 1].y - stitchBuffer[src].y;
		len = hypot(dif.x, dif.y);
		mvstch(dst++, src);
		if (len > ini.maxStitchLength) {

			cnt = ceil(len / userStitchLength);
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
			pnt.x = stitchBuffer[src].x + stp.x;
			pnt.y = stitchBuffer[src].y + stp.y;
			at = stitchBuffer[src].attribute;
			if (at != stitchBuffer[src + 1].attribute) {

				if (!(at&NOTFRM) && stitchBuffer[src + 1].attribute&TYPMSK) {

					if (!((at&FRMSK) == (stitchBuffer[src + 1].attribute&FRMSK)))
						at &= NTYPMSK;
				} else
					at &= NTYPMSK;
			}
			at &= (~KNOTMSK);
			for (ine = 0; ine < cnt - 1; ine++) {

				stitchBuffer[dst].attribute = at;
				stitchBuffer[dst].x = pnt.x;
				stitchBuffer[dst++].y = pnt.y;
				pnt.x += stp.x;
				pnt.y += stp.y;
				adcnt++;
			}
		}
	}
	while (src < hed.stitchCount)
		mvstch(dst++, src++);
	tstchs0 = &stitchBuffer[MAXPCS >> 1];
	tstchs1 = &stitchBuffer[strt];
	cnt = dst - (MAXPCS >> 1);
	for (src = 0; src < cnt; src++)
		tstchs1[src] = tstchs0[src];
	hed.stitchCount = strt + cnt;
	return adcnt;
}

void rembig() {

	RANGE rng;
	unsigned ind;

	if (userStitchLength < ini.maxStitchLength) {

		savdo();
		if (selectedFormCount) {

			for (ind = 0; ind < selectedFormCount; ind++) {

				frmrng(selectedFormList[ind], &rng);
				makbig(rng.start, rng.finish);
			}
			goto bigdun;
		}
		if (chkMap(FORMSEL)) {

			frmrng(closestFormToCursor, &rng);
			makbig(rng.start, rng.finish);
			goto bigdun;
		}
		if (chkMap(GRPSEL)) {

			rngadj();
			if (groupEndStitch < hed.stitchCount)
				groupEndStitch++;
			if (closestPointIndex < groupStitchIndex)
				groupStitchIndex += makbig(groupStartStitch, groupEndStitch);
			else
				closestPointIndex += makbig(groupStartStitch, groupEndStitch);
			grpAdj();
			goto bigdun;
		}
		makbig(0, hed.stitchCount);

	bigdun:;
		coltab();
		setMap(RESTCH);
	} else
		tabmsg(IDS_REM1);
}

void duselrng() {

	selectedRange.start = 0;
	selectedRange.finish = hed.stitchCount;
	if (chkMap(GRPSEL)) {

		rngadj();
		selectedRange.start = groupStartStitch;
		selectedRange.finish = groupEndStitch;
		return;
	}
}

void longer() {

	unsigned	ind, ine;
	double		len, minlen = 1e99;
	double		curlen;

	if (closestPointIndex == largestStitchIndex)
		return;
	curlen = hypot(stitchBuffer[closestPointIndex + 1].x - stitchBuffer[closestPointIndex].x, stitchBuffer[closestPointIndex + 1].y - stitchBuffer[closestPointIndex].y);
	for (ind = closestPointIndex + 1; ind < selectedRange.finish; ind++) {

		len = hypot(stitchBuffer[ind + 1].x - stitchBuffer[ind].x, stitchBuffer[ind + 1].y - stitchBuffer[ind].y);
		if (len == curlen)
			goto long1;
	}
	for (ine = selectedRange.start; ine < selectedRange.finish - 1; ine++) {

		len = hypot(stitchBuffer[ine + 1].x - stitchBuffer[ine].x, stitchBuffer[ine + 1].y - stitchBuffer[ine].y);
		if (len > curlen&&len < minlen) {

			minlen = len;
			ind = ine;
		}
	}
	if (minlen == 1e99)
		return;
long1:;
	currentStitchIndex = ind;
	lensadj();
	ritnum(STR_NUMSCH, closestPointIndex);
}

void shorter() {

	unsigned	ind, ine;
	double		len, maxlen = 0;
	double		curmax;

	if (closestPointIndex == smallestStitchIndex)
		return;
	curmax = hypot(stitchBuffer[closestPointIndex + 1].x - stitchBuffer[closestPointIndex].x, stitchBuffer[closestPointIndex + 1].y - stitchBuffer[closestPointIndex].y);
	for (ine = closestPointIndex; ine != 0; ine--) {

		len = hypot(stitchBuffer[ine].x - stitchBuffer[ine - 1].x, stitchBuffer[ine].y - stitchBuffer[ine - 1].y);
		if (len == curmax) {
			ine--;
			goto short1;
		}
	}
	for (ind = selectedRange.start; ind < selectedRange.finish - 1; ind++) {

		len = hypot(stitchBuffer[ind + 1].x - stitchBuffer[ind].x, stitchBuffer[ind + 1].y - stitchBuffer[ind].y);
		if (len<curmax&&len>maxlen) {

			maxlen = len;
			ine = ind;
		}
	}
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", hypot(stitchBuffer[ind + 1].x - stitchBuffer[ind].x, stitchBuffer[ind + 1].y - stitchBuffer[ind].y));
	butxt(HMINLEN, msgbuf);
short1:;
	currentStitchIndex = ine;
	lensadj();
	ritnum(STR_NUMSCH, closestPointIndex);
}

void setsrch(unsigned p_stind) {

	currentStitchIndex = p_stind;
	lensadj();
	ritnum(STR_NUMSCH, closestPointIndex);
}

BOOL inrng(unsigned p_stind) {

	if (stitchBuffer[p_stind].x >= rngrct.left
		&&stitchBuffer[p_stind].x <= rngrct.right
		&&stitchBuffer[p_stind].y >= rngrct.bottom
		&&stitchBuffer[p_stind].y <= rngrct.top)

		return 1;
	else
		return 0;
}

BOOL finrng(unsigned find) {

	unsigned cod;

	if (formList[find].rectangle.left >= rngrct.left
		&&formList[find].rectangle.right <= rngrct.right
		&&formList[find].rectangle.bottom >= rngrct.bottom
		&&formList[find].rectangle.top <= rngrct.top) {

		if (!activeLayer)
			return 1;
		cod = (formList[find].attribute&FRMLMSK) >> 1;
		if (!cod || activeLayer == cod)
			return 1;
		else
			return 0;
	} else
		return 0;
}

void ungrplo() {

	unsigned ind;

	if (rstMap(GRPSEL)) {

		rngadj();
		closestPointIndex = groupStartStitch;
		setMap(SELBOX);
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			for (ind = 0; ind < hed.stitchCount; ind++) {

				if (!(stitchBuffer[ind].attribute&NOTFRM) && ((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) == closestFormToCursor) {

					closestPointIndex = ind;
					setMap(SELBOX);
					setMap(RESTCH);
					goto upgrpdun;
				}
			}
			grpmsg1();
		} else
			grpmsg();
	}
upgrpdun:;
}

void ungrphi() {

	unsigned ind;

	if (rstMap(GRPSEL)) {

		rngadj();
		closestPointIndex = groupEndStitch;
		setMap(SELBOX);
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			for (ind = hed.stitchCount; ind != 0; ind--) {

				if (!(stitchBuffer[ind - 1].attribute&NOTFRM) && ((stitchBuffer[ind - 1].attribute&FRMSK) >> FRMSHFT) == closestFormToCursor) {

					closestPointIndex = ind - 1;
					setMap(SELBOX);
					setMap(RESTCH);
					goto dwngrpdun;
				}
			}
			grpmsg1();
		} else
			grpmsg();
	}
dwngrpdun:;
}

void sizstch(fRECTANGLE* rct) {

	unsigned ind;

	if (hed.stitchCount) {

		rct->bottom = rct->top = rotatedStitches[0].y;
		rct->left = rct->right = rotatedStitches[0].x;
		for (ind = 1; ind < hed.stitchCount; ind++) {

			if (rotatedStitches[ind].x < rct->left)
				rct->left = rotatedStitches[ind].x;
			if (rotatedStitches[ind].x > rct->right)
				rct->right = rotatedStitches[ind].x;
			if (rotatedStitches[ind].y < rct->bottom)
				rct->bottom = rotatedStitches[ind].y;
			if (rotatedStitches[ind].y > rct->top)
				rct->top = rotatedStitches[ind].y;
		}
	} else
		rct->top = rct->bottom = rct->left = rct->right = 0;
}

void hupfn() {

	unsigned	ind;
	dPOINT		hupcntr;
	dPOINT		descntr;
	dPOINT		tsiz;
	dPOINT		dif;

	rstMap(HUPCHNG);
	rotatedStitches = stitchBuffer;
	sizstch(&checkHoopRect);
	if (formIndex) {

		if (!hed.stitchCount) {

			checkHoopRect.bottom = checkHoopRect.top = currentFormVertices[0].y;
			checkHoopRect.left = checkHoopRect.right = currentFormVertices[0].x;
		}
		for (ind = 0; ind < fltad; ind++) {

			if (formPoints[ind].x < checkHoopRect.left)
				checkHoopRect.left = formPoints[ind].x;
			if (formPoints[ind].x > checkHoopRect.right)
				checkHoopRect.right = formPoints[ind].x;
			if (formPoints[ind].y < checkHoopRect.bottom)
				checkHoopRect.bottom = formPoints[ind].y;
			if (formPoints[ind].y > checkHoopRect.top)
				checkHoopRect.top = formPoints[ind].y;
		}
	}
	if (hed.stitchCount || fltad || chkMap(HUPEX)) {

		if (checkHoopRect.left<0 ||
			checkHoopRect.right>ini.hoopSizeX ||
			checkHoopRect.bottom<0 ||
			checkHoopRect.top>ini.hoopSizeY) {

			setMap(HUPEX);
		}
		if (chkMap(HUPEX)) {

			tsiz.x = checkHoopRect.right - checkHoopRect.left;
			tsiz.y = checkHoopRect.top - checkHoopRect.bottom;
			if (tsiz.x > ini.hoopSizeX) {

				ini.hoopSizeX = tsiz.x;
				setMap(HUPCHNG);
			}
			if (tsiz.y > ini.hoopSizeY) {

				ini.hoopSizeY = tsiz.y;
				setMap(HUPCHNG);
			}
			descntr.x = tsiz.x / 2 + checkHoopRect.left;
			descntr.y = tsiz.y / 2 + checkHoopRect.bottom;
			hupcntr.x = ini.hoopSizeX / 2;
			hupcntr.y = ini.hoopSizeY / 2;
			dif.x = hupcntr.x - descntr.x;
			dif.y = hupcntr.y - descntr.y;
			for (ind = 0; ind < hed.stitchCount; ind++) {

				stitchBuffer[ind].x += dif.x;
				stitchBuffer[ind].y += dif.y;
			}
			for (ind = 0; ind < fltad; ind++) {

				formPoints[ind].x += dif.x;
				formPoints[ind].y += dif.y;
			}
			for (ind = 0; ind < formIndex; ind++) {

				formList[ind].rectangle.left += dif.x;
				formList[ind].rectangle.right += dif.x;
				formList[ind].rectangle.top += dif.y;
				formList[ind].rectangle.bottom += dif.y;
			}
			unzoomedRect.x = ini.hoopSizeX;
			unzoomedRect.y = ini.hoopSizeY;
			zoomMin = (double)MINZUM / unzoomedRect.x;
			zumhom();
		}
	}
}

void chkhup() {

	setMap(HUPEX);
	hupfn();
	if (chkMap(INIT))
		prfmsg();
	setfchk();
}

int strcomp(const void *arg1, const void *arg2) {

	return _stricmp(*(TCHAR**)arg1, *(TCHAR**)arg2);
}

void strlcpy(TCHAR* dst, TCHAR* src) {

#if  __UseASM__
	_asm {

		mov		ebx, offset upmap
		mov		ecx, dst
		mov		edx, src
		xor		eax, eax
		lup : mov		al, [edx]
			  inc		edx
			  or al, al
			  je		lupx
			  and		al, 0x7f
			  bt[ebx], eax
			  jnc		short lup1
			  or al, 0x20
			  lup1:		mov[ecx], al
						inc		ecx
						jmp		lup
						lupx : mov[ecx], al
	}
#else
	//Correct
	for (;;) {
		TCHAR chr = *(src++);

		if (chr == 0)
			break;

		chr &= 0x7f;

		if (_bittest((long *)upmap, chr))
			chr |= 0x20;

		*(dst++) = chr;
	}

	*dst = 0;
#endif
}

TCHAR lchr(int op) {

#if  __UseASM__
	_asm {

		mov		ebx, offset upmap
		mov		eax, op
		and		eax, 0x7f
		bt[ebx], eax
		jnc		short lchrx
		or al, 0x20
		lchrx:
	}
#else
	TCHAR chr = op & 0x7f;

	return _bittest((long *)upmap, chr) ? chr : chr | 0x20;
#endif
}

void barnam(HWND hndl, unsigned fpnt) {

	TCHAR		buf[_MAX_PATH];
	TCHAR*		pchr;

	if (fpnt < thumdcnt&&thumsel[fpnt]) {

		strcpy_s(buf, thumsel[fpnt]);
		pchr = strrchr(buf, '.');
		if (pchr)
			pchr[0] = 0;
		SetWindowText(hndl, buf);
	} else
		SetWindowText(hndl, "");
}

void rthumnam(unsigned ind) {

	switch (ind) {

	case 0:

		barnam(hbuts[HNUM], ind);
		break;

	case 1:

		barnam(hbuts[HTOT], ind);
		break;

	case 2:

		barnam(hbuts[HMINLEN], ind);
		break;

	case 3:

		barnam(hbuts[HMAXLEN], ind);
		break;
	}
}

#pragma warning(push)
#pragma warning(disable : 4996)
void thumnail() {

	WIN32_FIND_DATA	fdat;
	unsigned		ind, ine;
	HANDLE			shndl;
	TCHAR*			pchr;

	unbsho();
	undat();
	untrace();
	// is this correct? types do not match
	thumnams = (TCHAR*)oseq;
	pthums = (TCHAR**)&oseq[MAXSEQ >> 1];

	SetCurrentDirectory(defDir);
	strcpy_s(srchnam, defDir);
	pchr = &srchnam[strlen(srchnam) - 1];
	if (pchr[0] != '\\') {

		pchr[1] = '\\';
		pchr[2] = 0;
	}
	strcat_s(srchnam, "*.thr");
	shndl = FindFirstFile(srchnam, &fdat);
	if (shndl == INVALID_HANDLE_VALUE) {

		sprintf_s(msgbuf, sizeof(msgbuf), "Can't find %s\n", srchnam);
		shoMsg(msgbuf);
		unthum();
	} else {

		ine = 0;
		pthums[0] = thumnams;
		strcpy(pthums[0], fdat.cFileName);
		ine += strlen(fdat.cFileName) + 1;
		pthums[1] = &thumnams[ine];
		ind = 1;
		while (FindNextFile(shndl, &fdat)) {

			strcpy(pthums[ind], fdat.cFileName);
			ine += strlen(fdat.cFileName) + 1;
			pthums[++ind] = &thumnams[ine];
		}
		FindClose(shndl);
		thumcnt = ind;
		qsort((void*)&pthums[0], ind, 4, strcomp);
		ind = thumind = 0;
		while (ind < 4 && thumind < thumcnt&&ind < thumcnt) {

			thumsel[ind] = pthums[ind];
			ind++;
		}
		thumind = thumdcnt = ind;
		while (ind < 4 && ind < thumcnt)
			rthumnam(ind++);
		setMap(THUMSHO);
		thumsrch[0] = 0;
		SetWindowText(hbuts[HBOXSEL], "");
		butxt(HBOXSEL, "");
		vubak();
		setMap(RESTCH);
	}
}
#pragma warning(pop)

void nuthsel() {

	unsigned ind, len, bakind;

	if (thumind < thumcnt) {

		bakind = thumind;
		ind = 0;
		len = strlen(thumsrch);
		setMap(RESTCH);
		if (len) {

			while (ind < 4 && thumind < thumcnt) {

				if (!strncmp(thumsrch, pthums[thumind], len)) {

					thumsel[ind] = pthums[thumind];
					redraw(hvu[ind++]);
				}
				thumind++;
			}
		} else {

			while (ind < 4 && thumind < thumcnt) {

				thumsel[ind] = pthums[thumind];
				redraw(hvu[ind++]);
				thumind++;
			}
		}
		if (ind) {

			thumdcnt = ind;
			while (ind < 4)
				rthumnam(ind++);
		} else
			thumind = bakind;
	}
}

void nuthbak(unsigned cnt) {

	unsigned len;

	if (thumind) {

		len = strlen(thumsrch);
		if (len) {

			while (cnt&&thumind < MAXFORMS) {

				if (thumind) {

					thumind--;
					if (!strncmp(thumsrch, pthums[thumind], len))
						cnt--;
				} else
					break;
			}
		} else
			thumind -= cnt;
		if (thumind > MAXFORMS)
			thumind = 0;
		nuthsel();
	}
}

void nuthum(TCHAR chr) {

	unsigned ind;

	ind = strlen(thumsrch);
	if (ind < 16) {

		setMap(RESTCH);
		thumsrch[ind++] = chr;
		thumsrch[ind] = 0;
		butxt(HBOXSEL, thumsrch);
		thumind = 0;
		nuthsel();
	}
}

void bakthum() {

	unsigned ind;

	ind = strlen(thumsrch);
	if (ind) {

		setMap(RESTCH);
		thumsrch[--ind] = 0;
		thumind = 0;
		butxt(HBOXSEL, thumsrch);
		nuthsel();
	}
}

void selalstch() {

	if (hed.stitchCount) {

		closestPointIndex = 0;
		groupStitchIndex = hed.stitchCount - 1;
		groupStartStitch = closestPointIndex;
		groupEndStitch = groupStitchIndex;
		setMap(GRPSEL);
		setMap(SCROS);
		setMap(ECROS);
		grpAdj();
		setMap(RESTCH);
	}
}

void insflin(POINT ipnt) {

	POINT off;

	off.x = inspnt.x >> 1;
	off.y = inspnt.y >> 1;

	flin[0].x = flin[3].x = flin[4].x = ipnt.x - off.x;
	flin[1].x = flin[2].x = ipnt.x + off.x;
	flin[0].y = flin[1].y = flin[4].y = ipnt.y - off.y;
	flin[2].y = flin[3].y = ipnt.y + off.y;
}

BOOL isthr(TCHAR* nam) {

	TCHAR* pchr;

	pchr = strrchr(nam, '.');
	if (pchr) {

		pchr++;
		if (!_strnicmp(pchr, "th", 2))
			return 1;
		else
			return 0;
	} else
		return 1;
}

unsigned gethand(fPOINTATTRIBUTE* p_pstch, unsigned cnt) {

	unsigned ind;
	unsigned ocnt;

	ocnt = 0;
	for (ind = 0; ind < cnt; ind++) {

		if (p_pstch[ind].attribute&USMSK)
			ocnt++;
	}
	return ocnt;
}

void insfil() {

	OPENFILENAME oin = {

		sizeof(OPENFILENAME),	//lStructsize
		hWnd,					//hwndOwner 
		hInst,					//hInstance 
		"THR files\0*.thr\0\0",	//lpstrFilter 
		cstFltr,				//lpstrCustomFilter 
		_MAX_PATH,				//nMaxCustFilter 
		0,						//nFilterIndex 
		insnam,					//lpstrFile 
		_MAX_PATH,				//nMaxFile 
		0,						//lpstrFileTitle 
		0,						//nMaxFileTitle 
		defDir,					//lpstr	ialDir 
		0,						//lpstrTitle
		OFN_EXPLORER | OFN_OVERWRITEPROMPT,//Flags
		0,						//nFileOffset
		0,						//nFileExtension 
		"thr",					//lpstrDefExt 
		0,						//lCustData 
		0,						//lpfnHook
		0,						//lpTemplateName
	};
	STRHED		thed;
	STREX		thedx = { 0 };
	PCSHEADER			tphed;
	unsigned	ind, ine, cod, codof;
	fRECTANGLE		trct;
	fPOINT		tsiz;
	POINT		tpnt;
	PCSTCH*		tbuf;
	double		homscor = 0;
	double		filscor;
	unsigned	vervar;
	FRMHEDO*	frmlstx;

	if (chkMap(IGNORINS) || GetOpenFileName(&oin)) {

		hinsf = CreateFile(insnam, (GENERIC_READ), 0, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hinsf == INVALID_HANDLE_VALUE) {

			filnopn(IDS_FNOPN, insnam);
			hFil = 0;
			CloseHandle(hinsf);
		} else {

			insfstch = hed.stitchCount;
			if (isthr(insnam)) {

				ReadFile(hinsf, (STRHED*)&thed, sizeof(STRHED), &red, NULL);
				if ((thed.headerType & 0xffffff) != 0x746872)
					tabmsg(IDS_NOTHR);
				else {

					vervar = (thed.headerType & 0xff000000) >> 24;
					if (vervar) {
#define FRMW 5
#define HANDW 4
#define FRMPW 2
#define STCHW 1			
						gethand(stitchBuffer, hed.stitchCount);
						homscor = (double)
							formIndex*FRMW +
							gethand(stitchBuffer, hed.stitchCount)*HANDW +
							fltad*FRMPW +
							hed.stitchCount*STCHW;
						ReadFile(hinsf, (STREX*)&thedx, sizeof(STREX), &red, 0);
					}
					savdo();
					ReadFile(hinsf, (fPOINTATTRIBUTE*)&stitchBuffer[hed.stitchCount], thed.stitchCount * sizeof(fPOINTATTRIBUTE), &red, NULL);
					SetFilePointer(hinsf, 164, 0, FILE_CURRENT);
					trct.left = trct.bottom = (float)1e9;
					trct.top = trct.right = (float)1e-9;
					codof = formIndex << FRMSHFT;
					insflt = fltad; insfpnt = formIndex;
					if (thed.pointCount) {

						if (vervar < 2) {

							frmlstx = (FRMHEDO*)&bseq;
							ReadFile(hinsf, (FRMHEDO*)&bseq, thed.formCount * sizeof(FRMHEDO), &red, 0);
							if (red != thed.formCount * sizeof(FRMHEDO)) {

								formIndex = red / sizeof(FRMHEDO);
								setMap(BADFIL);
							}
							if (formIndex + thed.pointCount < MAXFORMS) {

								FillMemory(&formList[formIndex], thed.formCount * sizeof(FRMHED), 0);
								ine = formIndex;
								for (ind = 0; ind < thed.formCount; ind++) {

									MoveMemory(&formList[ine], &frmlstx[ind], sizeof(FRMHEDO));
									ine++;
								}
							}
						} else
							ReadFile(hinsf, (FRMHED*)&formList[formIndex], thed.formCount * sizeof(FRMHED), &red, 0);
						ReadFile(hinsf, (fPOINT*)&formPoints[fltad], thed.pointCount * sizeof(fPOINT), &red, 0);
						ReadFile(hinsf, (SATCON*)&satks[satkad], thed.dlineCount * sizeof(SATCON), &red, 0);
						ReadFile(hinsf, (fPOINT*)&clipboardPoints[clpad], thed.clipboardDataCount * sizeof(fPOINT), &red, 0);
						CloseHandle(hinsf);
						hinsf = 0;
						for (ind = formIndex; ind < formIndex + thed.formCount; ind++) {

							formList[ind].vertices = adflt(formList[ind].sides);
							if (formList[ind].type == SAT) {

								if (formList[ind].satinGuideCount)
									formList[ind].satinOrAngle.sac = adsatk(formList[ind].satinGuideCount);
								if (isclpx(ind))
									formList[ind].angleOrClipData.clip = adclp(formList[ind].lengthOrCount.clipCount);
							}
							if (isclp(ind))
								formList[ind].angleOrClipData.clip = adclp(formList[ind].lengthOrCount.clipCount);
							if (iseclpx(ind))
								formList[ind].borderClipData = adclp(formList[ind].clipEntries);
						}
						formIndex += thed.formCount;
						if (thed.formCount) {

							trct.left = trct.right = formPoints[insflt].x;
							trct.bottom = trct.top = formPoints[insflt].y;
							for (ind = insflt + 1; ind < fltad; ind++) {

								if (formPoints[ind].x < trct.left)
									trct.left = formPoints[ind].x;
								if (formPoints[ind].x > trct.right)
									trct.right = formPoints[ind].x;
								if (formPoints[ind].y < trct.bottom)
									trct.bottom = formPoints[ind].y;
								if (formPoints[ind].y > trct.top)
									trct.top = formPoints[ind].y;
							}
						}
					}
					if (thed.stitchCount) {

						for (ind = hed.stitchCount; ind < (unsigned)hed.stitchCount + thed.stitchCount; ind++) {

							if (stitchBuffer[ind].attribute&ALTYPMSK) {

								cod = (stitchBuffer[ind].attribute&FRMSK) + codof;
								stitchBuffer[ind].attribute &= NFRMSK;
								stitchBuffer[ind].attribute |= cod;
							}
							if (stitchBuffer[ind].x < trct.left)
								trct.left = stitchBuffer[ind].x;
							if (stitchBuffer[ind].x > trct.right)
								trct.right = stitchBuffer[ind].x;
							if (stitchBuffer[ind].y < trct.bottom)
								trct.bottom = stitchBuffer[ind].y;
							if (stitchBuffer[ind].y > trct.top)
								trct.top = stitchBuffer[ind].y;
						}
					}
					if (thed.headerType & 0x1000000) {

						filscor = (double)thed.formCount*FRMW +
							gethand(&stitchBuffer[hed.stitchCount], thed.stitchCount)*HANDW +
							thed.FormPointsLen*FRMPW +
							thed.stitchCount*STCHW;
						if (filscor > homscor) {

							for (ind = 0; ind < 50; ind++)
								hedx.creatorName[ind] = thedx.creatorName[ind];
							redfnam(fildes);
							sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRDBY], thrnam, fildes);
							SetWindowText(hWnd, msgbuf);
						}
					}
					inscntr.x = (trct.right - trct.left) / 2 + trct.left;
					inscntr.y = (trct.top - trct.bottom) / 2 + trct.bottom;
					hed.stitchCount += thed.stitchCount;
					tsiz.x = trct.right - trct.left;
					tsiz.y = trct.top - trct.bottom;
					ratsr();
					inspnt.x = tsiz.x*horizontalRatio;
					inspnt.y = tsiz.y*horizontalRatio;
					tpnt.x = stitchWindowClientRect.right >> 1;
					tpnt.y = stitchWindowClientRect.bottom >> 1;
					insflin(tpnt);
					newFormVertexCount = 5;
					setMap(SHOFRM);
					setMap(INSFIL);
					dufrm();
				}
			} else {

				ReadFile(hinsf, (PCSHEADER*)&tphed, 0x46, &red, NULL);
				if (hed.leadIn == 0x32 && hed.colorCount == 16) {

					savdo();
					tbuf = (PCSTCH*)bseq;
					ReadFile(hinsf, (PCSTCH*)tbuf, tphed.stitchCount * sizeof(PCSTCH), &red, NULL);
					ine = hed.stitchCount;
					cod = 0;
					for (ind = 0; ind < tphed.stitchCount; ind++) {

						if (tbuf[ind].typ == 3)
							cod = tbuf[ind++].fx;
						else {

							stitchBuffer[ine].x = tbuf[ind].x + (float)tbuf[ind].fx / 256;
							stitchBuffer[ine].y = tbuf[ind].y + (float)tbuf[ind].fy / 256;
							stitchBuffer[ine++].attribute = cod;
						}
					}
					ind = hed.stitchCount;
					trct.left = trct.right = stitchBuffer[ind].x;
					trct.top = trct.bottom = stitchBuffer[ind++].y;
					while (ind < (unsigned)ine) {

						if (stitchBuffer[ind].x < trct.left)
							trct.left = stitchBuffer[ind].x;
						if (stitchBuffer[ind].x > trct.right)
							trct.right = stitchBuffer[ind].x;
						if (stitchBuffer[ind].y < trct.bottom)
							trct.bottom = stitchBuffer[ind].y;
						if (stitchBuffer[ind].y > trct.top)
							trct.top = stitchBuffer[ind].y;
						ind++;
					}
					inscntr.x = (trct.right - trct.left) / 2 + trct.left;
					inscntr.y = (trct.top - trct.bottom) / 2 + trct.bottom;
					hed.stitchCount = ine;
					tsiz.x = trct.right - trct.left;
					tsiz.y = trct.top - trct.bottom;
					ratsr();
					inspnt.x = tsiz.x*horizontalRatio;
					inspnt.y = tsiz.y*horizontalRatio;
					tpnt.x = stitchWindowClientRect.right >> 1;
					tpnt.y = stitchWindowClientRect.bottom >> 1;
					insflin(tpnt);
					newFormVertexCount = 5;
					setMap(SHOFRM);
					setMap(INSFIL);
					dufrm();
				}
			}
			if (hinsf)
				CloseHandle(hinsf);
		}
	}
}

void duinsfil() {

	fPOINT		off;
	unsigned	ind;
	fRECTANGLE*		trct;

	px2stch();
	off.x = sPnt.x - inscntr.x;
	off.y = sPnt.y - inscntr.y;
	for (ind = insfpnt; ind < formIndex; ind++) {

		trct = &formList[ind].rectangle;
		trct->bottom += off.y;
		trct->top += off.y;
		trct->left += off.x;
		trct->right += off.x;
	}
	for (ind = insflt; ind < fltad; ind++) {

		formPoints[ind].x += off.x;
		formPoints[ind].y += off.y;
	}
	for (ind = insfstch; ind < hed.stitchCount; ind++) {

		stitchBuffer[ind].x += off.x;
		stitchBuffer[ind].y += off.y;
	}
	rstMap(FRMOF);
	setMap(INIT);
	coltab();
	setMap(RESTCH);
}

void gotbox() {

	rstMap(BZUMIN);
	rstMap(BOXSLCT);
	rstMap(FRMPSEL);
	grpAdj();
}

void rngal() {

	unsigned	ind, ine, in;
	unsigned	pmax = 0, maxlen, len;
	RANGE*		prng;

	if (!rstMap(WASFPNT)) {

		rstMap(GRPSEL);
		ind = ine = in = 0;
		prng = (RANGE*)&bseq;
		rstMap(GRPSEL);
		while (ind < hed.stitchCount) {

			if (inrng(ind)) {

				if (!in) {

					prng[ine].start = ind;
					in = 1;
				}
			} else {

				if (in) {

					prng[ine].finish = ind - 1;
					ine++;
					in = 0;
				}
			}
			ind++;
		}
		if (in) {

			prng[ine].finish = ind - 1;
			ine++;
		}
		if (ine) {

			maxlen = 0;
			for (ind = 0; ind < ine; ind++) {

				len = prng[ind].finish - prng[ind].start;
				if (len > maxlen) {

					maxlen = len;
					pmax = ind;
				}
			}
			if (maxlen) {

				closestPointIndex = prng[pmax].start;
				groupStitchIndex = prng[pmax].finish;
				setMap(GRPSEL);
			}
			gotbox();
		}
	}
}

void nucols() {

	unsigned ind;

	clRmap((formIndex << 5) + 1);
	for (ind = 0; ind < selectedFormCount; ind++) {

		setr(selectedFormList[ind]);
		ptrSelectedForm = &formList[ind];
		if (ptrSelectedForm->fillType) {

			ptrSelectedForm->fillColor = actcol;
			ptrSelectedForm->fillInfo.feather.color = actcol;
		}
		if (ptrSelectedForm->edgeType)
			ptrSelectedForm->borderColor = actcol;
		if (ptrSelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
			ptrSelectedForm->underlayColor = actcol;
	}
	for (ind = 0; ind < hed.stitchCount; ind++) {

		if (chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) && (stitchBuffer[ind].attribute&TYPMSK) != TYPMSK) {

			stitchBuffer[ind].attribute &= NCOLMSK;
			stitchBuffer[ind].attribute |= actcol;
		}
	}
}

BOOL dunum(unsigned cod) {

	if (cod >= '0'&&cod <= '9') {

		numcod = cod;
		return 1;
	}
	if (cod >= VK_NUMPAD0&&cod <= VK_NUMPAD9) {

		numcod = cod - '0';
		return 1;
	}
	return 0;
}

void stchrct(fRECTANGLE* rct)
{
	unsigned ind;

	if (hed.stitchCount)
	{
		rct->bottom = rct->left = 1e10;
		rct->top = rct->right = 0;
		if (hed.stitchCount) {

			for (ind = 0; ind < hed.stitchCount; ind++) {

				if (stitchBuffer[ind].x < rct->left)
					rct->left = stitchBuffer[ind].x;
				if (stitchBuffer[ind].x > rct->right)
					rct->right = stitchBuffer[ind].x;
				if (stitchBuffer[ind].y < rct->bottom)
					rct->bottom = stitchBuffer[ind].y;
				if (stitchBuffer[ind].y > rct->top)
					rct->top = stitchBuffer[ind].y;
			}
		}
	}
}

void frmrct(fRECTANGLE* rct)
{
	unsigned ind;

	rct->left = rct->right = formPoints[0].x;
	rct->top = rct->bottom = formPoints[0].y;
	for (ind = 0; ind < fltad; ind++) {

		if (formPoints[ind].x < rct->left)
			rct->left = formPoints[ind].x;
		if (formPoints[ind].x > rct->right)
			rct->right = formPoints[ind].x;
		if (formPoints[ind].y > rct->top)
			rct->top = formPoints[ind].y;
		if (formPoints[ind].y < rct->bottom)
			rct->bottom = formPoints[ind].y;
	}
}

void desiz() {

	fRECTANGLE		trct;
	FLOAT		sizx, sizy;
	TCHAR*		pmsg;

	pmsg = msgbuf;
	if (hed.stitchCount) {

		stchrct(&trct);
		sizx = (trct.right - trct.left) / PFGRAN;
		sizy = (trct.top - trct.bottom) / PFGRAN;
		if ((trct.left < 0) || (trct.bottom < 0) || (trct.right > ini.hoopSizeX) || (trct.top > ini.hoopSizeY)) {

			strcpy_s(msgbuf, stab[STR_STCHOUT]);
			pmsg = &msgbuf[strlen(msgbuf)];
		}
#if LANG==ENG||LANG==HNG

		sprintf_s(pmsg, sizeof(msgbuf),
			stab[STR_STCHS],
			hed.stitchCount,
			sizx, sizx / 25.4,
			sizy, sizy / 25.4);
#endif

#if LANG==GRM

		sprintf_s(pmsg, sizeof(msgbuf),
			stab[STR_STCHS],
			hed.stitchCount,
			sizx,
			sizy);
#endif
		pmsg = &pmsg[strlen(pmsg)];
	}
	if (formIndex) {

		frmrct(&trct);
		sizx = (trct.right - trct.left) / PFGRAN;
		sizy = (trct.top - trct.bottom) / PFGRAN;
#if LANG==ENG||LANG==HNG

		sprintf_s(pmsg, sizeof(msgbuf), stab[STR_FORMS],
			formIndex,
			sizx, sizx / 25.4,
			sizy, sizy / 25.4);
#endif

#if LANG==GRM

		sprintf_s(pmsg, sizeof(msgbuf), stab[STR_FORMS],
			formIndex,
			sizx,
			sizy);
#endif
		pmsg = &pmsg[strlen(pmsg)];
	}
	sprintf_s(pmsg, sizeof(msgbuf), stab[STR_HUPWID],
		ini.hoopSizeX / PFGRAN,
		ini.hoopSizeY / PFGRAN);
	pmsg = &pmsg[strlen(pmsg)];
	if (hed.stitchCount) {

		sprintf_s(pmsg, sizeof(msgbuf), stab[STR_CREATBY],
			fildes,
			hedx.modifierName);
	}
	shoMsg(msgbuf);
}

void sidhup() {

	RECT		huprct;
	RECT		prfrct;
	unsigned	ind;

	setMap(HUPMSG);
	GetWindowRect(thDat[PHUP], &huprct);
	GetWindowRect(hPrf, &prfrct);
	hSid = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		prfrct.right + 3 - mainWindowOrigin.x,
		huprct.top - mainWindowOrigin.y,
		buttonWid3 + buttonWid * 2 + 6,
		buttonHi*HUPS + 6,
		hWnd,
		NULL,
		hInst,
		NULL);
	for (ind = 0; ind < HUPS; ind++) {

		hsidWnd[ind] = CreateWindow(
			"STATIC",
			stab[ind + STR_HUP0],
			SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
			3,
			buttonHi*ind + 3,
			buttonWid3 + buttonWid * 2,
			buttonHi,
			hSid,
			NULL,
			hInst,
			NULL);
	}
}

void setpcs() {

	ini.auxFileType = AUXPCS;
	auxmen();
}

void setdst() {

	ini.auxFileType = AUXDST;
	auxmen();
}

void fop() {

	untrace();
	if (formIndex || hed.stitchCount) {

		if (savcmp()) {

			nuFil();
			nulayr(0);
		} else {

			savdisc();
			setMap(OSAV);
		}
	} else {

		nuFil();
		nulayr(0);
	}
}

void clpradj(fPOINTATTRIBUTE pnt) {

	if (pnt.x < clprcta.left)
		clprcta.left = pnt.x;
	if (pnt.x > clprcta.right)
		clprcta.right = pnt.x;
	if (pnt.y < clprcta.bottom)
		clprcta.bottom = pnt.y;
	if (pnt.y > clprcta.top)
		clprcta.top = pnt.y;
}

void clpadj() {

	double		mid;
	unsigned	ind;

	if (chkMap(GRPSEL)) {

		rngadj();
		ind = groupStartStitch;
		clprcta.left = clipboardRect.right = stitchBuffer[ind++].x;
		clprcta.top = clprcta.bottom = stitchBuffer[ind].y;
		clpradj(stitchBuffer[ind]);
		while (ind < groupEndStitch)
			clpradj(stitchBuffer[ind++]);
		if (stitchBuffer[ind].x < clprcta.left)
			clprcta.left = stitchBuffer[ind].x;
		if (stitchBuffer[ind].x > clprcta.right)
			clprcta.right = stitchBuffer[ind].x;
		mid = (clprcta.right - clprcta.left) / 2 + clprcta.left;
		stitchBuffer[groupStartStitch].y = stitchBuffer[groupEndStitch].y = (clprcta.top - clprcta.bottom) / 2 + clprcta.bottom;
		if (stitchBuffer[groupStartStitch].x < mid) {

			stitchBuffer[groupStartStitch].x = clprcta.left;
			stitchBuffer[groupEndStitch].x = clprcta.right;
		} else {

			stitchBuffer[groupEndStitch].x = clprcta.left;
			stitchBuffer[groupStartStitch].x = clprcta.right;
		}
		coltab();
		setMap(RESTCH);
	} else
		shoseln(IDS_GRPMSG, IDS_RNGEND);
}

void shftflt(fPOINT pnt) {

	if (pnt.x <= zoomRect.left || pnt.x >= zoomRect.right || pnt.y <= zoomRect.bottom || pnt.y >= zoomRect.top)
		shft(pnt);
}

void fil2men() {

	if (chku(FIL2OF)) {

		CheckMenuItem(hMen, ID_FIL2SEL_ON, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_FIL2SEL_OFF, MF_CHECKED);
	} else {

		CheckMenuItem(hMen, ID_FIL2SEL_ON, MF_CHECKED);
		CheckMenuItem(hMen, ID_FIL2SEL_OFF, MF_UNCHECKED);
	}

}

void defpref() {

	unsigned ind;

	binaryVariableBitmap = 0;
	for (ind = 0; ind < 16; ind++) {

		useCol[ind] = defUseCol[ind];
		custCol[ind] = defCustCol[ind];
		bakCust[ind] = defBakCust[ind];
		bitmapBackgroundColors[ind] = defBakBit[ind];
	}
	dazdef();
	underlayColor = 15;
	borderWidth = BRDWID;
	buttonholeFillCornerLength = IBFCLEN;
	ini.chainSpace = CHSDEF;
	ini.chainRatio = CHRDEF;
	ini.fillAngle = DEFANG;
	rstu(SQRFIL);
	StitchSpace = DEFSPACE*PFGRAN;
	showStitchThreshold = SHOPNTS;
	ini.gridSize = 12;
	ini.hoopType = LARGHUP;
	ini.hoopSizeX = LHUPX;
	ini.hoopSizeY = LHUPY;
	ini.cursorNudgeStep = NUGINI;
	ini.nudgePixels = DEFPIX;
	setu(BLUNT);
	smallStitchLength = SMALSIZ*PFGRAN;
	snapLength = SNPLEN*PFGRAN;
	spiralWrap = SPIRWRAP;
	starRatio = STARAT;
	StitchBoxesThreshold = STCHBOX;
	ini.maxStitchLength = MAXSIZ*PFGRAN;
	userStitchLength = USESIZ*PFGRAN;
	minStitchLength = MINSIZ*PFGRAN;
	ini.clipboardOffset = 0;
	ini.fillPhase = 0;
	bitmapColor = 0xc8dfee;
	if (!ini.customHoopX)
		ini.customHoopX = LHUPX;
	if (!ini.customHoopY)
		ini.customHoopY = LHUPY;
	picotSpace = IPICSPAC;
	setu(FIL2OF);
	fil2men();
	backgroundColor = 0xa8c4b1;
	unzoomedRect.x = ini.hoopSizeX;
	unzoomedRect.y = ini.hoopSizeY;
	ini.waveEnd = IWAVEND;
	ini.wavePoints = IWAVPNTS;
	ini.waveLobes = IWAVS;
	ini.waveStart = IWAVSTRT;
	ini.featherFillType = FDEFTYP;
	ini.featherUpCount = FDEFUP;
	ini.featherDownCount = FDEFDWN;
	ini.featherRatio = (float)FDEFRAT;
	ini.featherMinStitchSize = FDEFFLR;
	ini.featherCount = FDEFNUM;
	ini.underlayStitchLen = DEFULEN;
	ini.underlaySpacing = DEFUSPAC;
	ini.formBoxSizePixels = DEFBPIX;
	ini.textureHeight = ITXHI;
	ini.textureWidth = ITXWID;
	ini.textureSpacing = (float)ITXSPAC;
}

void dumrk(double pntx, double pnty) {

	if (rstMap(GMRK))
		drwmrk(StitchWindowDC);
	zoomMarkPoint.x = pntx;
	zoomMarkPoint.y = pnty;
	setMap(INIT);
	setMap(GMRK);
	drwmrk(StitchWindowDC);
	setMap(WASMRK);
}

void gselrng() {

	unsigned ind;

	selectedFormsRange.start = selectedFormsRange.finish = *selectedFormList;
	for (ind = 1; ind < selectedFormCount; ind++) {

		if (selectedFormList[ind] < selectedFormsRange.start)
			selectedFormsRange.start = selectedFormList[ind];
		if (selectedFormList[ind] > selectedFormsRange.finish)
			selectedFormsRange.finish = selectedFormList[ind];
	}
}

double nuang(double ydif, double xdif) {

	double tang, rang;

	tang = atan2(ydif, xdif);
	rang = tang - organg;
	if (fabs(rang) > PI) {

		if (rang > 0)
			rang = 2 * PI - rang;
		else
			rang += 2 * PI;
	}
	return rang;
}

void angdif(double p_ang) {

	if (p_ang > hiang)
		hiang = p_ang;
	else {

		if (p_ang < loang)
			loang = p_ang;
	}
}

void rotmrk() {

	unsigned	ind, cnt, cod;
	double		tang;

	if (chkMap(GMRK) && (chkMap(FORMSEL) || chkMap(GRPSEL))) {

		if (chkMap(FORMSEL)) {

			cod = closestFormToCursor << FRMSHFT;
			fvars(closestFormToCursor);
			loang = hiang = 0;
			organg = atan2(currentFormVertices[0].y - zoomMarkPoint.y, currentFormVertices[0].x - zoomMarkPoint.x);
			for (ind = 1; ind < sides; ind++)
				angdif(nuang(currentFormVertices[ind].y - zoomMarkPoint.y, currentFormVertices[ind].x - zoomMarkPoint.x));
			for (ind = 0; ind < hed.stitchCount; ind++) {

				if ((stitchBuffer[ind].attribute&FRMSK) == cod)
					angdif(nuang(stitchBuffer[ind].y - zoomMarkPoint.y, stitchBuffer[ind].x - zoomMarkPoint.x));
			}
		} else {

			rngadj();
			loang = hiang = 0;
			organg = atan2(stitchBuffer[groupStartStitch].y - zoomMarkPoint.y, stitchBuffer[groupStartStitch].x - zoomMarkPoint.x);
			for (ind = groupStartStitch + 1; ind <= groupEndStitch; ind++)
				angdif(nuang(stitchBuffer[ind].y - zoomMarkPoint.y, stitchBuffer[ind].x - zoomMarkPoint.x));
		}
		tang = hiang - loang;
		cnt = 2 * PI / tang;
		ini.rotationAngle = 2 * PI / cnt;
		sprintf_s(msgbuf, sizeof(msgbuf), "Rotation Angle: %.2f\n"
			"Segments: %d\n", ini.fillAngle * 180 / PI, cnt);
		shoMsg(msgbuf);
	} else
		shoseln(IDS_FSZ, IDS_SETROTM);
}

void segentr() {

	if (!rotationAngle)
		rotationAngle = PI / 180;
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_ENTROT], 2 * PI / rotationAngle);
	shoMsg(msgbuf);
	setMap(NUMIN);
	numWnd();
}

void rotseg() {

	rotationAngle = ini.rotationAngle;
	segentr();
	setMap(ENTRSEG);
}

void pntmrk() {

	if (chkMap(SELBOX)) {

		dumrk(stitchBuffer[closestPointIndex].x, stitchBuffer[closestPointIndex].y);
		goto mrkdun;
	}
	if (chkMap(FRMPSEL)) {

		dumrk(formList[closestFormToCursor].vertices[closestVertexToCursor].x, formList[closestFormToCursor].vertices[closestVertexToCursor].y);
		goto mrkdun;
	}
	shoseln(IDS_STCH_FRM, IDS_SETMRK);

mrkdun:;
}

void filfrms() {

	unsigned ine;

	if (selectedFormCount) {

		savdo();
		for (ine = 0; ine < selectedFormCount; ine++) {

			closestFormToCursor = selectedFormList[ine];
			refilfn();
		}
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			savdo();
			refil();
			setMap(RESTCH);
		}
	}
}

void nuslst(unsigned find) {

	unsigned ind, ine;

	gselrng();
	ine = 0;
	if (find < selectedFormsRange.start) {

		for (ind = find; ind < selectedFormsRange.finish; ind++)
			selectedFormList[ine++] = ind;
		goto gotsrng;
	}
	if (find > selectedFormsRange.finish) {

		for (ind = selectedFormsRange.start; ind <= find; ind++)
			selectedFormList[ine++] = ind;
		goto gotsrng;
	}
	for (ind = selectedFormsRange.start; ind <= find; ind++)
		selectedFormList[ine++] = ind;
gotsrng:;
	selectedFormCount = ine;
}

void srchk() {

	rstMap(FORMSEL);
	selectedFormCount = 0;
	if (setMap(LENSRCH)) {

		if (chkMap(WASGRP)) {

			closestPointIndex = groupStartStitch = prevGroupStartStitch;
			groupStitchIndex = groupEndStitch = prevGroupEndStitch;
		} else
			rstMap(GRPSEL);
	} else {

		if (chkMap(GRPSEL)) {

			setMap(WASGRP);
			rngadj();
			prevGroupStartStitch = groupStartStitch;
			prevGroupEndStitch = groupEndStitch;
		} else
			rstMap(WASGRP);
	}
	duselrng();
}

unsigned duswap(unsigned dat) {

#if  __UseASM__
	_asm {

		mov		eax, dat
		bswap	eax
	}
#else
	return _byteswap_ulong(dat);
#endif
}

void ritcur() {

	ICONINFO			ici;
	HCURSOR				tcur;
	POINT				cpos;
	unsigned			ind, ine;
	unsigned			l_amsk, binv;
	unsigned			bmsk;
	unsigned			tpix;

	tcur = GetCursor();
	GetIconInfo(tcur, &ici);
	GetCursorPos(&cpos);
	cpos.x -= (stitchWindowOrigin.x + ici.xHotspot);
	cpos.y -= (stitchWindowOrigin.y + ici.yHotspot);
	GetBitmapBits(ici.hbmMask, 256, (unsigned char*)&markedStitchMap);
	if (tcur == hArrowCurs) {

		for (ind = 0; ind < 32; ind++) {

			l_amsk = duswap(markedStitchMap[ind]);
			binv = duswap(markedStitchMap[ind + 32]);
			bmsk = 0x80000000;
			for (ine = 0; ine < 32; ine++) {

				if (!(bmsk&l_amsk)) {

					if (binv&bmsk)
						tpix = 0xffffff;
					else
						tpix = 0;
					SetPixel(StitchWindowDC, cpos.x + ine, cpos.y + ind, tpix);
				}
				bmsk >>= 1;
			}
		}
	} else {

		for (ind = 0; ind < 32; ind++) {

			binv = duswap(markedStitchMap[ind + 32]);
			bmsk = 0x80000000;
			for (ine = 0; ine < 32; ine++) {

				if (bmsk&binv)
					SetPixel(StitchWindowDC, cpos.x + ine, cpos.y + ind, GetPixel(StitchWindowDC, cpos.x + ine, cpos.y + ind) ^ 0xffffff);
				bmsk >>= 1;
			}
		}
	}
}

void delsfrms(unsigned cod) {

	unsigned ind, ine, inf;

	if (cod) {

		inf = (formIndex >> 5) + 1;
		for (ine = 0; ine < inf; ine++)
			markedStitchMap[ine] = 0;
		for (ind = 0; (unsigned)ind < selectedFormCount; ind++) {

			closestFormToCursor = selectedFormList[ind];
			setr(closestFormToCursor);
			fvars(closestFormToCursor);
			f1del();
		}
		finds = (unsigned*)bseq;
		inf = 0; ind = 0;
		for (ine = 0; ine < formIndex; ine++) {

			if (!chkr(ine)) {

				frmcpy(&formList[inf], &formList[ine]);
				finds[ine] = (ine - ind) << 4;
				inf++;
			} else
				ind++;
		}
		formIndex = inf;
		inf = 0;
		if (chkMap(DELTO)) {

			for (ind = 0; ind < hed.stitchCount; ind++) {

				if (stitchBuffer[ind].attribute&ALTYPMSK) {

					cod = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
					if (!chkr(cod)) {

						stitchBuffer[inf].attribute = stitchBuffer[ind].attribute &= NFRMSK;
						stitchBuffer[inf].attribute |= finds[cod];
						stitchBuffer[inf].x = stitchBuffer[ind].x;
						stitchBuffer[inf++].y = stitchBuffer[ind].y;
					}
				} else {

					stitchBuffer[inf].attribute = stitchBuffer[ind].attribute;
					stitchBuffer[inf].x = stitchBuffer[ind].x;
					stitchBuffer[inf++].y = stitchBuffer[ind].y;
				}
			}
			hed.stitchCount = inf;
		} else {

			for (ind = 0; ind < hed.stitchCount; ind++) {

				if (!(stitchBuffer[ind].attribute&NOTFRM)) {

					cod = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
					if (chkr(cod))
						stitchBuffer[ind].attribute &= (NFRMSK&NTYPMSK);
				}
			}
		}
		selectedFormCount = 0;
		rstMap(FORMSEL);
		coltab();
		setMap(RESTCH);
	}
}

void nedon() {

	rstu(NEDOF);
	nedmen();
}

void nedof() {

	setu(NEDOF);
	nedmen();
}

void shoknot() {

	rstu(KNOTOF);
	knotmen();
	setMap(RESTCH);
}

void hidknot() {

	setu(KNOTOF);
	knotmen();
	setMap(RESTCH);
}

void pcsbsavon() {

	rstu(BSAVOF);
	bsavmen();
	setMap(RESTCH);
}

void pcsbsavof() {

	setu(BSAVOF);
	bsavmen();
	setMap(RESTCH);
}

void tglhid() {

	if (!toglMap(HIDSTCH))
		rstMap(FRMOF);
	setMap(RESTCH);
}

void respac() {

	if (isclp(closestFormToCursor)) {

		ptrSelectedForm->fillSpacing = StitchSpace;
		fsizpar();
	}
}

BOOL chkminus(unsigned cod) {

	if (cod == 189 || cod == 109) {

		if (preferenceIndex == PCLPOF + 1)
			return 1;
		if (formMenuChoice == LFRMSPAC&&isfclp())
			return 1;
		if (formMenuChoice == LWLKIND)
			return 1;
	}
	return 0;
}

void retrac() {

	unsigned src, dst;

	if (chkMap(GRPSEL)) {

		savdo();
		rngadj();
		if (!groupStartStitch)
			groupStartStitch++;
		makspac(groupEndStitch + 1, groupEndStitch - groupStartStitch);
		src = groupEndStitch - 1;
		dst = groupEndStitch + 1;
		while (src >= groupStartStitch) {

			stitchBuffer[dst].attribute = stitchBuffer[src].attribute;
			stitchBuffer[dst].x = stitchBuffer[src].x;
			stitchBuffer[dst++].y = stitchBuffer[src--].y;
		}
		coltab();
		setMap(RESTCH);
	} else
		shoseln(IDS_GRPMSG, IDS_RETRAC);
}

void setgrd(COLORREF col) {

	unsigned	ind;
	GRDCOD		cods[] = {

		ID_GRDHI,HIGRD,
		ID_GRDMED,MEDGRD,
		ID_GRDEF,DEFGRD,
		ID_GRDRED,REDGRD,
		ID_GRDBLU,BLUGRD,
		ID_GRDGRN,GRNGRD,
	};

	for (ind = 0; ind < 6; ind++) {

		if (col == cods[ind].col)
			CheckMenuItem(hMen, cods[ind].id, MF_CHECKED);
		else
			CheckMenuItem(hMen, cods[ind].id, MF_UNCHECKED);
	}
	gridPen = nuPen(gridPen, 1, col);
	ini.gridColor = col;
	setMap(RESTCH);
	setMap(DUMEN);
}

void ovrlay() {

	rstMap(IGNORINS);
	insfil();
	rstMap(INSFIL);
	rstMap(FRMOF);
	setMap(INIT);
	coltab();
	setMap(RESTCH);
}

void fil2sel(unsigned stat) {

	setu(FIL2OF);
	if (stat)
		rstu(FIL2OF);
	fil2men();
}

void rotauxmen() {

	if (chku(ROTAUX)) {

		CheckMenuItem(hMen, ID_ROTAUXON, MF_CHECKED);
		CheckMenuItem(hMen, ID_ROTAUXOFF, MF_UNCHECKED);
	} else {

		CheckMenuItem(hMen, ID_ROTAUXON, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_ROTAUXOFF, MF_CHECKED);
	}
}

void rotauxsel(unsigned stat) {

	setu(ROTAUX);
	if (!stat)
		rstu(ROTAUX);
	rotauxmen();
	setMap(DUMEN);
}

void frmcurmen() {

	if (chku(FRMX)) {

		CheckMenuItem(hMen, ID_FRMX, MF_CHECKED);
		CheckMenuItem(hMen, ID_FRMBOX, MF_UNCHECKED);
	} else {

		CheckMenuItem(hMen, ID_FRMX, MF_UNCHECKED);
		CheckMenuItem(hMen, ID_FRMBOX, MF_CHECKED);
	}
}

void frmcursel(unsigned stat) {

	setu(FRMX);
	if (!stat)
		rstu(FRMX);
	frmcurmen();
	setMap(DUMEN);
}

#pragma warning (push)
#pragma warning (disable : 4725)
void stchsnap(unsigned strt, unsigned fin) {

#if  __UseASM__
	_asm {
		xor		eax, eax
		mov		al, 12
		mul		strt
		mov		ecx, fin
		sub		ecx, strt
		je		short stchsnapx
		add		eax, offset stitchBuffer
		fld		ini.gridSize
		snplup : fld		dword ptr[eax]
				 fdiv	st, st(1)
				 frndint
				 fmul	st, st(1)
				 fstp	dword ptr[eax]
				 add		eax, 4
				 fld		dword ptr[eax]
				 fdiv	st, st(1)
				 frndint
				 fmul	st, st(1)
				 fstp	dword ptr[eax]
				 add		eax, 8
				 loop	snplup
				 stchsnapx :
	}
#else
	fPOINTATTRIBUTE *pnt = stitchBuffer + strt;

	for (unsigned i = 0; i < fin - strt; i++) {
		pnt->x = rintf(pnt->x / ini.gridSize) * ini.gridSize;
		pnt->y = rintf(pnt->y / ini.gridSize) * ini.gridSize;

		pnt++;
	}
#endif
}
#pragma warning (pop)

#pragma warning (push)
#pragma warning (disable : 4725)
void frmsnap(fPOINT* strt, unsigned cnt) {

#if  __UseASM__
	_asm {
		mov		eax, strt
		mov		ecx, cnt
		shl		ecx, 1
		je		short frmsnapx
		fld		ini.gridSize
		snpflup : fld		dword ptr[eax]
				  fdiv	st, st(1)
				  frndint
				  fmul	st, st(1)
				  fstp	dword ptr[eax]
				  add		eax, 4
				  loop	snpflup
				  frmsnapx :
	}
#else
	for (unsigned i = 0; i < cnt; i++) {
		strt->x = rintf(strt->x / ini.gridSize) * ini.gridSize;
		strt->y = rintf(strt->y / ini.gridSize) * ini.gridSize;

		strt++;
	}
#endif
}
#pragma warning (pop)

void gsnap() {

	unsigned ind;

	if (chkMap(TXTRED))
	{
		txsnap();
		return;
	}
	if (selectedFormCount) {

		savdo();
		for (ind = 0; ind < selectedFormCount; ind++) {

			closestFormToCursor = selectedFormList[ind];
			frmsnap(formList[closestFormToCursor].vertices, formList[closestFormToCursor].sides);
			frmout(closestFormToCursor);
			refil();
		}
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			savdo();
			frmsnap(formList[closestFormToCursor].vertices, formList[closestFormToCursor].sides);
			frmout(closestFormToCursor);
			refil();
			setMap(RESTCH);
		} else {

			if (chkMap(GRPSEL)) {

				savdo();
				rngadj();
				stchsnap(groupStartStitch, groupEndStitch + 1);
				setMap(RESTCH);
			} else
				shoseln(IDS_FGRPF, IDS_SNAP2GRD);
		}
	}
}

#if PESACT

void lodpes() {

	setMap(REDOLD);
	strcpy_s(filnam, "u:\\mrd\\t.pes");
	nuFil();
}

void savpes() {

	ini.auxFileType = AUXPES;
	strcpy_s(filnam, "u:\\mrd\\t1.thr");
	nunams();
	sav();
}
#endif

void ritlock(HWND hwndlg) {

	unsigned ind;
	WIN32_FIND_DATA*	pdat;

	pdat = (WIN32_FIND_DATA*)&bseq;
	SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_RESETCONTENT, 0, 0);
	SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_RESETCONTENT, 0, 0);
	for (ind = 0; ind < xpnt; ind++) {

		if (pdat[ind].dwFileAttributes&FILE_ATTRIBUTE_READONLY)
			SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_ADDSTRING, 0, (long)pdat[ind].cFileName);
		else
			SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_ADDSTRING, 0, (long)pdat[ind].cFileName);
	}
}

BOOL CALLBACK LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	WIN32_FIND_DATA*	pdat;
	HANDLE				srch;
	TCHAR				snam[_MAX_PATH];
	TCHAR				tnam[_MAX_PATH];
	unsigned			ind, ine;
	HWND				hlok, hunlok;

	pdat = (WIN32_FIND_DATA*)&bseq;
	switch (umsg) {

	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		strcpy_s(snam, defDir);
		strcat_s(snam, "\\*.thr");
		srch = FindFirstFile(snam, &pdat[0]);
		if (srch == INVALID_HANDLE_VALUE) {

			sprintf_s(msgbuf, sizeof(msgbuf), "Directory: %s has no .thr files\n", defDir);
			shoMsg(msgbuf);
			EndDialog(hwndlg, wparam);
			return TRUE;
		}
		xpnt = 1;
		while (FindNextFile(srch, &pdat[xpnt++]));
		xpnt--;
		ritlock(hwndlg);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {

		case IDCANCEL:

			EndDialog(hwndlg, wparam);
			return TRUE;

		case IDC_LOCKAL:

			for (ind = 0; ind < xpnt; ind++)
				pdat[ind].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
			ritlock(hwndlg);
			break;

		case IDC_UNLOCKAL:

			for (ind = 0; ind < xpnt; ind++)
				pdat[ind].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
			ritlock(hwndlg);
			break;

		case IDC_LOCK:

			ine = 0;
			hunlok = GetDlgItem(hwndlg, IDC_UNLOCKED);
			for (ind = 0; ind < xpnt; ind++) {

				if (!(pdat[ind].dwFileAttributes&FILE_ATTRIBUTE_READONLY)) {

					if (SendMessage(hunlok, LB_GETSEL, ine, 0))
						pdat[ind].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
					ine++;
				}
			}
			ritlock(hwndlg);
			break;

		case IDC_UNLOCK:

			ine = 0;
			hlok = GetDlgItem(hwndlg, IDC_LOCKED);
			for (ind = 0; ind < xpnt; ind++) {

				if ((pdat[ind].dwFileAttributes&FILE_ATTRIBUTE_READONLY)) {

					if (SendMessage(hlok, LB_GETSEL, ine, 0))
						pdat[ind].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
					ine++;
				}
			}
			ritlock(hwndlg);
			break;

		case IDOK:

			strcpy_s(snam, defDir);
			strcat_s(snam, "\\");
			ine = 0;
			for (ind = 0; ind < xpnt; ind++) {

				strcpy_s(tnam, snam);
				strcat_s(tnam, pdat[ind].cFileName);
				if (!SetFileAttributes(tnam, pdat[ind].dwFileAttributes))
					ine++;
			}
			if (ine) {

				sprintf_s(msgbuf, sizeof(msgbuf), "%d files could not be locked or unlocked\n", ine);
				shoMsg(msgbuf);
			}
			EndDialog(hwndlg, wparam);
			return TRUE;
			break;
		}
	}
	return 0;
}

void lock() {

	DialogBox(hInst, MAKEINTRESOURCE(IDD_DLOCK), hWnd, (DLGPROC)LockPrc);
}

unsigned colsum(COLORREF col) {

	unsigned colsum;
	unsigned ind;

	trcols(col);
	colsum = 0;
	for (ind = 0; ind < 3; ind++) {

		if (chkMap(traceRGBFlag[ind]))
			colsum += pixelColors[ind];
	}
	return colsum;
}

unsigned icolsum(COLORREF col) {

	unsigned colsum;
	unsigned ind;

	trcols(col);
	colsum = 0;
	for (ind = 0; ind < 3; ind++) {

		if (chkMap(traceRGBFlag[ind]))
			colsum += 255 - pixelColors[ind];
	}
	return colsum;
}

COLORREF dwnshft(COLORREF col) {

#if  __UseASM__
	_asm {

		mov		eax, col
		shr		eax, 1
		and eax, 0x3f3f3f
	}
#else
	return (col >> 1) & 0x3f3f3f;
#endif
}

void setrac(unsigned p_bpnt) {

#if  __UseASM__
	_asm {

		mov		eax, p_bpnt
		mov		ebx, offset oseq
		bts[ebx], eax
	}
#else
	_bittestandset((long *)oseq, p_bpnt);
#endif
}

BOOL getrac(unsigned p_bpnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, p_bpnt
		mov		ebx, offset oseq
		bt[ebx], ecx
		jnc		short getracx
		inc		eax
		getracx :
	}
#else
	return _bittest((long *)oseq, p_bpnt);
#endif
}

void shownd(HWND hwnd) {

	ShowWindow(hwnd, SW_SHOW);
}

void hidwnd(HWND hwnd) {

	ShowWindow(hwnd, SW_HIDE);
}

void untrace() {

	unsigned	ind;

	if (rstMap(WASTRAC)) {

		DeleteObject(hTraceBitmap);
		ReleaseDC(hWnd, tracedc);
		rstMap(WASEDG);
		for (ind = 0; ind < 16; ind++) {

			shownd(hDef[ind]);
			shownd(hCol[ind]);
			shownd(hSiz[ind]);
		}
		for (ind = 0; ind < 3; ind++) {

			hidwnd(hctrc[ind]);
			hidwnd(hstrc[ind]);
			hidwnd(htrup[ind]);
			hidwnd(htrdwn[ind]);
		}
		for (ind = 0; ind < 9; ind++)
			shownd(hbuts[ind]);
		hidwnd(hstep);
	} else {

		if (chkMap(TRCUP))
			downColor = 0xffffff;
		else
			upColor = 0;
	}
}

void trcstpnum() {

	sprintf_s(msgbuf, sizeof(msgbuf), "len: %.2f", ini.traceLength / PFGRAN);
	SetWindowText(hstep, msgbuf);
}

void trcratnum() {

	sprintf_s(msgbuf, sizeof(msgbuf), "pnts: %.2f", -log10(ini.traceRatio - 1));
	butxt(HLIN, msgbuf);
}

void clrhbut(unsigned strt)
{
	unsigned ind;

	for (ind = strt; ind < 9; ind++)
		SetWindowText(hbuts[ind], "");
}

void tracwnd() {

	unsigned ind;

	for (ind = 0; ind < 16; ind++) {

		hidwnd(hDef[ind]);
		hidwnd(hCol[ind]);
		hidwnd(hSiz[ind]);
	}
	for (ind = 0; ind < 3; ind++) {

		shownd(hctrc[ind]);
		shownd(hstrc[ind]);
		shownd(htrup[ind]);
		shownd(htrdwn[ind]);
	}
	hidwnd(hbuts[HBOXSEL]);
	hidwnd(hbuts[HUPTO]);
	shownd(hstep);
	trcstpnum();
	trcratnum();
	clrhbut(4);
}

void trcols(COLORREF col) {

#if  __UseASM__
	_asm {

		mov		eax, col
		mov		ecx, offset pixelColors
		movzx	ebx, al
		mov[ecx], ebx
		add		ecx, 4
		movzx	ebx, ah
		mov[ecx], ebx
		shr		eax, 8
		movzx	ebx, ah
		add		ecx, 4
		mov[ecx], ebx
	}
#else
	//Check translation
	pixelColors[0] = col & 0xff;
	pixelColors[1] = (col & 0xff00) >> 8;
	pixelColors[2] = (col & 0xff0000) >> 16;
#endif
}

BOOL trcin(COLORREF col) {

	if (col) {

		trcols(col);
		if (chkMap(TRCRED)) {

			if (pixelColors[0] > highColors[0])
				return 0;
			if (pixelColors[0] < lowColors[0])
				return 0;
		}
		if (chkMap(TRCGRN)) {

			if (pixelColors[1] > highColors[1])
				return 0;
			if (pixelColors[1] < lowColors[1])
				return 0;
		}
		if (chkMap(TRCBLU)) {

			if (pixelColors[2] > highColors[2])
				return 0;
			if (pixelColors[2] < lowColors[2])
				return 0;
		}
		return 1;
	}
	return 0;
}

void getrmap() {

	BITMAPINFO			l_binf;
	BITMAPINFOHEADER	l_binfh;
	unsigned			ind;

	memset((BITMAPINFOHEADER*)&l_binfh, 0, sizeof(BITMAPINFOHEADER));
	l_binfh.biSize = sizeof(BITMAPINFOHEADER);
	l_binfh.biWidth = bitmapWidth;
	l_binfh.biHeight = bitmapHeight;
	l_binfh.biPlanes = 1;
	l_binfh.biBitCount = 32;
	l_binfh.biCompression = BI_RGB;
	l_binf.bmiHeader = l_binfh;
	hTraceBitmap = CreateDIBSection(bitmapDC, &l_binf, DIB_RGB_COLORS, (void**)&traceBitmap, 0, 0);
	tracedc = CreateCompatibleDC(StitchWindowDC);
	if (hTraceBitmap && tracedc) {
		SelectObject(tracedc, hTraceBitmap);
		BitBlt(tracedc, 0, 0, bitmapWidth, bitmapHeight, bitmapDC, 0, 0, SRCCOPY);
		setMap(WASTRAC);
		tracedPixels = (unsigned*)oseq;
		traceDataSize = ((bitmapWidth*bitmapHeight) >> 5) + 1;
		for (ind = 0; ind < traceDataSize; ind++)
			tracedPixels[ind] = 0;
		StretchBlt(StitchWindowMemDC, bitmapDstRect.left, bitmapDstRect.top, bitmapDstRect.right - bitmapDstRect.left, bitmapDstRect.bottom - bitmapDstRect.top,
			bitmapDC, bitmapSrcRect.left, bitmapSrcRect.top, bitmapSrcRect.right - bitmapSrcRect.left, bitmapSrcRect.bottom - bitmapSrcRect.top, SRCCOPY);
	}
}

void trace() {

	unsigned			ind;

#if TRCMTH==0

	unsigned			usum, psum, dsum;
#endif

	if (*bnam) {

		untrace();
		tracwnd();
		getrmap();
		if (px2stch() && !rstMap(WASTRCOL)) {

			if (chkMap(LANDSCAP))
				sPnt.y -= (unzoomedRect.y - bitmapSizeinStitches.y);
			bitpnt.x = bmpStitchRatio.x*sPnt.x;
			bitpnt.y = bmpStitchRatio.y*sPnt.y - 1;
			ind = traceBitmap[bitpnt.y*bitmapWidth + bitpnt.x] ^ 0xffffff;
			if (chkMap(TRCUP)) {

				upColor = ind;
				downColor = 0xffffff;
			} else {

				downColor = ind;
				upColor = 0;
			}
			setMap(TRCRED);
			setMap(TRCGRN);
			setMap(TRCBLU);
		}
		sref = 0xffffff;
		if (!chkMap(TRCRED))
			sref &= REDMSK;
		if (!chkMap(TRCGRN))
			sref &= GRNMSK;
		if (!chkMap(TRCBLU))
			sref &= BLUMSK;
		if (sref != 0xffffff) {

			for (ind = 0; ind < bitmapWidth*bitmapHeight; ind++)
				traceBitmap[ind] &= sref;
		}

#if TRCMTH==0

		usum = icolsum(upColor);
		dsum = icolsum(downColor);
		for (index = 0; index < bitmapWidth*bitmapHeight; index++) {

			psum = colsum(traceBitmap[index]);
			if (usum > psum&&dsum < psum)
				setrac(index);
			else
				traceBitmap[index] = 0;
		}
#endif

#if TRCMTH==1
		invertUpColor = upColor ^ 0xffffff;
		invertDownColor = downColor ^ 0xffffff;
		trcols(invertUpColor);
		for (ind = 0; ind < 3; ind++)
			highColors[ind] = pixelColors[ind];
		trcols(invertDownColor);
		for (ind = 0; ind < 3; ind++)
			lowColors[ind] = pixelColors[ind];
		for (ind = 0; ind < bitmapWidth*bitmapHeight; ind++) {

			if (trcin(traceBitmap[ind]))
				setrac(ind);
			else
				traceBitmap[ind] = 0;
		}
#endif
		setMap(TRSET);
		setMap(RESTCH);
	} else
		tabmsg(IDS_MAPLOD);
}

void setedg(unsigned p_bpnt) {

#if  __UseASM__
	_asm {

		mov		eax, p_bpnt
		mov		ebx, tracedEdges
		bts[ebx], eax
	}
#else
	_bittestandset((long *)tracedEdges, p_bpnt);
#endif
}

BOOL chkedg(unsigned p_bpnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, p_bpnt
		mov		ebx, tracedEdges
		bt[ebx], ecx
		jnc		short chkedgx
		inc		eax
		chkedgx :
	}
#else
	return _bittest((long *)tracedEdges, p_bpnt);
#endif
}

void tracedg() {

	unsigned	ind, ine, flg;
	long		l_bpnt;

	if (!chkMap(WASTRAC))
		trace();
	tracedEdges = (unsigned*)&oseq[traceDataSize];
	for (ind = 0; ind < traceDataSize; ind++)
		tracedEdges[ind] = 0;
	l_bpnt = 0;
	for (ind = 0; ind < bitmapHeight; ind++) {

		flg = 0;
		for (ine = 0; ine < bitmapWidth; ine++) {

			if (getrac(l_bpnt)) {

				if (!flg) {

					setedg(l_bpnt);
					flg = 1;
				}
			} else {

				if (flg) {

					setedg(l_bpnt - 1);
					flg = 0;
				}
			}
			l_bpnt++;
		}
		if (flg)
			setedg(l_bpnt - 1);
	}
	for (ind = 0; ind < bitmapWidth; ind++) {

		l_bpnt = ind;
		flg = 0;
		for (ine = 0; ine < bitmapHeight; ine++) {

			if (getrac(l_bpnt)) {

				if (!flg) {

					setedg(l_bpnt);
					flg = 1;
				}
			} else {

				if (flg) {

					setedg(l_bpnt - bitmapWidth);
					flg = 0;
				}
			}
			l_bpnt += bitmapWidth;
		}
		if (flg)
			setedg(l_bpnt - bitmapWidth);
	}
	for (ind = 0; ind < bitmapWidth*bitmapHeight; ind++) {

		if (chkedg(ind))
			traceBitmap[ind] = 0xffffff;
		else
			traceBitmap[ind] = 0;
	}
	setMap(RESTCH);
	setMap(WASEDG);
}

BOOL trcbit() {
	unsigned l_bpnt;

	l_bpnt = currentTracePoint.y*bitmapWidth + currentTracePoint.x;
	switch (traceDirection) {

	case TRCR:

		l_bpnt += (1 - bitmapWidth);
		if (currentTracePoint.x == (int)bitmapWidth - 1)
			traceDirection = TRCU;
		else {

			if (chkedg(l_bpnt)) {

				currentTracePoint.x++;
				currentTracePoint.y--;
				traceDirection = TRCD;
			} else {

				l_bpnt += bitmapWidth;
				if (chkedg(l_bpnt))
					currentTracePoint.x++;
				else
					traceDirection = TRCU;
			}
		}
		break;

	case TRCD:

		l_bpnt -= (bitmapWidth + 1);
		if (!currentTracePoint.y)
			traceDirection = TRCR;
		else {

			if (chkedg(l_bpnt)) {

				currentTracePoint.x--;
				currentTracePoint.y--;
				traceDirection = TRCL;
			} else {

				l_bpnt++;
				if (chkedg(l_bpnt))
					currentTracePoint.y--;
				else
					traceDirection = TRCR;
			}
		}
		break;

	case TRCL:

		l_bpnt += (bitmapWidth - 1);
		if (!currentTracePoint.x)
			traceDirection = TRCD;
		else {

			if (chkedg(l_bpnt)) {

				currentTracePoint.x--;
				currentTracePoint.y++;
				traceDirection = TRCU;
			} else {

				l_bpnt -= bitmapWidth;
				if (chkedg(l_bpnt))
					currentTracePoint.x--;
				else
					traceDirection = TRCD;
			}
		}
		break;

	case TRCU:

		l_bpnt += (1 + bitmapWidth);
		if (currentTracePoint.y == (int)bitmapHeight - 1)
			traceDirection = TRCL;
		else {

			if (chkedg(l_bpnt)) {

				currentTracePoint.x++;
				currentTracePoint.y++;
				traceDirection = TRCR;
			} else {

				l_bpnt--;
				if (chkedg(l_bpnt))
					currentTracePoint.y++;
				else
					traceDirection = TRCL;
			}
		}
		break;
	}
	if (tracedPoints[xpnt - 1].x != currentTracePoint.x || tracedPoints[xpnt - 1].y != currentTracePoint.y) {

		tracedPoints[xpnt].x = currentTracePoint.x;
		tracedPoints[xpnt++].y = currentTracePoint.y;
		if (xpnt >= 500000)
			return 0;
	}
	if (traceDirection == initialDirection&&currentTracePoint.x == tracedPoints[0].x&&currentTracePoint.y == tracedPoints[0].y)
		return 0;
	else
		return 1;
}

void dutdif(TRCPNT* pnt) {

	traceDiff0.x = pnt[1].x - pnt[0].x;
	// ToDo this is likely incorrect
	traceDiff0.y = pnt[1].x - pnt[0].y;
}

void dutrac() {

	unsigned	flg;
	RECT		fndrct;
	long		l_bpnt, blim, bakpnt;
	long		dis, mdis;
	unsigned	ind;
	double		tlen;
	double		tlens;
	float		ratof;
	unsigned	ine;

	if (px2stch()) {

		if (!chkMap(WASEDG)) {

			tracedg();
			return;
		}
		savdo();
		if (chkMap(LANDSCAP))
			sPnt.y -= (unzoomedRect.y - bitmapSizeinStitches.y);
		currentTracePoint.x = bmpStitchRatio.x*sPnt.x;
		currentTracePoint.y = bmpStitchRatio.y*sPnt.y;
		if (currentTracePoint.x > (long)bitmapWidth)
			currentTracePoint.x = bitmapWidth;
		if (currentTracePoint.y > (long)bitmapHeight)
			currentTracePoint.y = bitmapHeight;
		bakpnt = l_bpnt = currentTracePoint.y*bitmapWidth + currentTracePoint.x;
		if (!chkedg(l_bpnt)) {

			flg = 20;
			l_bpnt = bakpnt;
			blim = (currentTracePoint.y + 1)*bitmapWidth;
			while (l_bpnt < blim && !chkedg(l_bpnt))
				l_bpnt++;
			if (l_bpnt < blim)
				fndrct.right = l_bpnt - currentTracePoint.y*bitmapWidth;
			else
				fndrct.right = bitmapWidth;
			l_bpnt = bakpnt;
			blim = currentTracePoint.y*bitmapWidth;
			while (l_bpnt > blim && !chkedg(l_bpnt))
				l_bpnt--;
			if (l_bpnt == blim)
				fndrct.left = 0;
			else
				fndrct.left = l_bpnt - blim;
			l_bpnt = bakpnt;
			while (l_bpnt > 0 && !chkedg(l_bpnt))
				l_bpnt -= bitmapWidth;
			if (l_bpnt > 0)
				fndrct.bottom = l_bpnt / bitmapWidth;
			else
				fndrct.bottom = 0;
			l_bpnt = bakpnt;
			blim = bitmapWidth*bitmapHeight;
			while (l_bpnt < blim && !chkedg(l_bpnt))
				l_bpnt += bitmapWidth;
			if (l_bpnt < blim)
				fndrct.top = l_bpnt / bitmapWidth;
			else
				fndrct.top = bitmapHeight;
			flg = 0;
			mdis = 0x7fffffff;
			if (fndrct.left) {

				mdis = currentTracePoint.x - fndrct.left;
				flg = TRCL;
			}
			if (fndrct.right < (long)bitmapWidth) {

				dis = fndrct.right - currentTracePoint.x;
				if (dis < mdis) {

					mdis = dis;
					flg = TRCR;
				}
			}
			if (fndrct.bottom) {

				dis = currentTracePoint.y - fndrct.bottom;
				if (dis < mdis) {

					mdis = dis;
					flg = TRCD;
				}
			}
			if (fndrct.top < (long)bitmapHeight) {

				dis = fndrct.top - currentTracePoint.y;
				if (dis < mdis) {

					mdis = dis;
					flg = TRCU;
				}
			}
			switch (flg) {

			case TRCU:

				currentTracePoint.y = fndrct.top;
				traceDirection = TRCR;
				break;

			case TRCR:

				currentTracePoint.x = fndrct.right;
				traceDirection = TRCD;
				break;

			case TRCD:

				currentTracePoint.y = fndrct.bottom;
				traceDirection = TRCL;
				break;

			case TRCL:

				currentTracePoint.x = fndrct.left;
				traceDirection = TRCU;
				break;

			default:
				return;
			}
		}
		initialDirection = traceDirection;
		l_bpnt = currentTracePoint.y*bitmapWidth + currentTracePoint.x;
		xpnt = 1;
		tracedPoints = (TRCPNT*)bseq;
		tracedPoints[0].x = currentTracePoint.x;
		tracedPoints[0].y = currentTracePoint.y;
		while (trcbit());
		if (xpnt >= 500000) {

			tabmsg(IDS_FRM2L);
			return;
		}
		decimatedLine = &tracedPoints[xpnt];
		decimatedLine[0].x = tracedPoints[0].x;
		decimatedLine[0].y = tracedPoints[0].y;
		dutdif(&tracedPoints[0]);
		opnt = 1;
		for (ind = 1; ind < xpnt; ind++) {

			traceDiff1.x = traceDiff0.x;
			traceDiff1.y = traceDiff0.y;
			dutdif(&tracedPoints[ind]);
			if (traceDiff1.x != traceDiff0.x || traceDiff1.y != traceDiff0.y) {

				decimatedLine[opnt].x = tracedPoints[ind].x;
				decimatedLine[opnt++].y = tracedPoints[ind].y;
			}
		}
		tracedPoints[0].x = decimatedLine[0].x;
		tracedPoints[0].y = decimatedLine[0].y;
		ine = 0;
		xpnt = 0;
		for (ind = 1; ind < opnt; ind++) {

			tlen = hypot(decimatedLine[ind].x - decimatedLine[ine].x, decimatedLine[ind].y - decimatedLine[ine].y);
			if (tlen > ini.traceLength) {

				tracedPoints[xpnt].x = decimatedLine[ine].x;
				tracedPoints[xpnt].y = decimatedLine[ine].y;
				ine = ind;
				xpnt++;
			}
		}
		for (ind = ine + 1; ind < opnt; ind++) {

			tracedPoints[xpnt].x = decimatedLine[ind].x;
			tracedPoints[xpnt].y = decimatedLine[ind].y;
			xpnt++;
		}
		ptrSelectedForm = &formList[formIndex];
		frmclr(ptrSelectedForm);
		currentFormVertices = &formPoints[fltad];
		currentFormVertices[0].x = tracedPoints[0].x*stitchBmpRatio.x;
		currentFormVertices[0].y = tracedPoints[0].y*stitchBmpRatio.y;
		ine = 0;
		opnt = 0;
		tlens = 0;
		if (chkMap(LANDSCAP))
			ratof = unzoomedRect.y - bitmapSizeinStitches.y;
		else
			ratof = 0;
		for (ind = 1; ind < xpnt; ind++) {

			tlens += hypot(tracedPoints[ind].x - tracedPoints[ind - 1].x, tracedPoints[ind].y - tracedPoints[ind - 1].y);
			tlen = hypot(tracedPoints[ind].x - tracedPoints[ine].x, tracedPoints[ind].y - tracedPoints[ine].y);
			if (tlens > tlen*ini.traceRatio) {

				currentFormVertices[opnt].x = tracedPoints[ind - 1].x*stitchBmpRatio.x;
				currentFormVertices[opnt].y = tracedPoints[ind - 1].y*stitchBmpRatio.y + ratof;
				opnt++;
				ind--;
				ine = ind;
				tlens = 0;
			}
		}
		if (fltad + opnt > MAXFLT) {

			tabmsg(IDS_FRMOVR);
			return;
		}
		ptrSelectedForm->vertices = adflt(opnt);
		ptrSelectedForm->sides = opnt;
		ptrSelectedForm->type = POLI;
		ptrSelectedForm->attribute = activeLayer << 1;
		frmout(formIndex);
		formList[formIndex].satinGuideCount = 0;
		formIndex++;
		setMap(RESTCH);
		setMap(FRMOF);
		tglfrm();
	}
}

unsigned ducolm() {

	if (traceMsgPoint.x < (int)buttonWid)
		return 0;
	if (traceMsgPoint.x < (int)buttonWid << 1)
		return 1;
	else
		return 2;
}

void chkref() {

#if  __UseASM__
	_asm {
		mov		eax, upColor
		mov		ebx, downColor
		cmp		al, bl
		jc		short chklup1
		mov		dl, al
		mov		al, bl
		mov		bl, dl
		chklup1 : cmp		ah, bh
				  jc		short chklup2
				  mov		dl, ah
				  mov		ah, bh
				  mov		bh, dl
				  chklup2 : ror		eax, 16
							ror		ebx, 16
							cmp		al, bl
							jc		short chklup3
							mov		dl, al
							mov		al, bl
							mov		bl, dl
							chklup3 : rol		eax, 16
									  rol		ebx, 16
									  mov		upColor, eax
									  mov		downColor, ebx
	}
#else
	//check translation
	union {
		COLORREF cr;
		struct {
			unsigned char g;
			unsigned char r;
			unsigned char b;
			unsigned char x;
		} components;
	} u, d;

	u.cr = upColor;
	d.cr = downColor;

	unsigned char tmp;

	if (u.components.r < d.components.r) {
		tmp = u.components.r;

		u.components.r = d.components.r;
		d.components.r = tmp;
	}

	if (u.components.g < d.components.g) {
		tmp = u.components.g;

		u.components.g = d.components.g;
		d.components.g = tmp;
	}

	if (u.components.b < d.components.b) {
		tmp = u.components.b;

		u.components.b = d.components.b;
		d.components.b = tmp;
	}

	upColor = u.cr;
	downColor = d.cr;
#endif
}

void trnumwnd0(int pos) {

	htrnum = CreateWindow(
		"STATIC",
		0,
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWid3,
		pos,
		buttonWid,
		buttonHi,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void trnumwnd1(int pos) {

	hgNum = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWid3,
		pos,
		buttonWid3,
		buttonHi,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void tracpar() {

	unsigned	pos;
	double		rat;
	COLORREF	tcol;
	COLORREF	tpos;

	if (chkMap(TRNIN0))
		dutrnum0(atoi(trinbuf));
	if (chkMap(TRNIN1))
		dutrnum1();
	traceMsgPoint.x = msg.pt.x - mainWindowOrigin.x;
	traceMsgPoint.y = msg.pt.y - mainWindowOrigin.y;
	if (traceMsgPoint.x > (int)buttonWid3)
		dutrac();
	else {

		colm = ducolm();
		if (traceMsgPoint.y < (int)buttonHi * 15) {

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				upColor &= traceRGBMask[colm];
				downColor |= traceRGB[2 - colm];
				goto tracpar1;
			}
			rat = (float)traceMsgPoint.y / (buttonHi * 15);
			pos = rat * 255;
			tcol = upColor&traceRGB[2 - colm];
			tpos = pos << trshft[colm];
			if (tpos < tcol) {

				upColor &= traceRGBMask[colm];
				upColor |= tpos;
				goto tracpar1;
			}
			tcol = downColor&traceRGB[2 - colm];
			if (tpos > tcol) {

				downColor &= traceRGBMask[colm];
				downColor |= tpos;
				goto tracpar1;
			}
			if (msg.message == WM_LBUTTONDOWN) {

				upColor &= traceRGBMask[colm];
				upColor |= pos << trshft[colm];
			} else {

				downColor &= traceRGBMask[colm];
				downColor |= pos << trshft[colm];
			}
		tracpar1:
			redraw(hctrc[colm]);
			trace();
		} else {

			pos = floor(traceMsgPoint.y / buttonHi);
			if (pos < 16) {

				toglMap(traceRGBFlag[colm]);
				redraw(hstrc[colm]);
				trace();
			} else {

				if (pos < 18) {

					setMap(NUMIN);
					setMap(TRNIN0);
					msgIndex = 0;
					*trinbuf = 0;
					if (pos < 17) {

						trnumwnd0(buttonHi * 16);
						setMap(TRNUP);
					} else {

						trnumwnd0(buttonHi * 17);
						rstMap(TRNUP);
					}
				} else {

					if (pos < 20) {

						setMap(NUMIN);
						setMap(TRNIN1);
						msgIndex = 0;
						*trinbuf = 0;
						if (pos < 19) {

							trnumwnd1(buttonHi * 18);
							setMap(TRNUP);
						} else {

							trnumwnd1(buttonHi * 19);
							rstMap(TRNUP);
						}
					} else {

						switch (pos) {

						case 20:

							trdif();
							break;

						case 21:

							hidbit();
							break;

						case 22:

							blak();
							break;

						case 23:

							trcsel();
							break;

						case 24:

							tracedg();
							break;
						}
					}
				}
			}
		}
	}
}

#if __UseASM__ == 0
//Check Translation
static inline void difsub(unsigned *src, unsigned shft, unsigned *&dst) {
	*(dst++) = (*src >> (shft & 0x0f)) & 0xff;
}
#endif

void difbits(unsigned shft, unsigned* p_bpnt) {

#if  __UseASM__
	_asm {

		jmp		short difbts

		difsub :	mov		eax, [esi]
					shr		eax, cl
					and		eax, ebx
					mov[edi], eax
					add		edi, 4
					ret

		difbts :	mov		esi, p_bpnt
					mov		ecx, shft
					mov		edi, offset traceAdjacentColors
					mov		ebx, 0xff
					mov		edx, bitmapWidth
					shl		edx, 2
					call	difsub		//4
					sub		esi, edx
					call	difsub		//1
					sub		esi, 4
					call	difsub		//0
					add		esi, 8
					call	difsub		//2
					add		esi, edx
					call	difsub		//5
					sub		esi, 8
					call	difsub		//3
					add		esi, edx
					call	difsub		//6
					add		esi, 4
					call	difsub		//7
					add		esi, 4
					call	difsub		//8
	}
#else
	//check translation
	unsigned* l_bpnt = p_bpnt;
	unsigned *dst = traceAdjacentColors;

	difsub(l_bpnt, shft, dst);

	l_bpnt -= bitmapWidth;
	difsub(l_bpnt, shft, dst);

	l_bpnt -= 1;
	difsub(l_bpnt, shft, dst);

	l_bpnt += 2;
	difsub(l_bpnt, shft, dst);

	l_bpnt += bitmapWidth;
	difsub(l_bpnt, shft, dst);

	l_bpnt -= 2;
	difsub(l_bpnt, shft, dst);

	l_bpnt += bitmapWidth;
	difsub(l_bpnt, shft, dst);

	l_bpnt += 1;
	difsub(l_bpnt, shft, dst);

	l_bpnt += 1;
	difsub(l_bpnt, shft, dst);
#endif
}

void blanklin(unsigned bind) {

	unsigned ind;

	for (ind = bind; ind < bind + bitmapWidth; ind++)
		differenceBitmap[ind] = 0;
}

unsigned trsum() {

	unsigned rsum, ind;

	rsum = 0;
	for (ind = 1; ind < 9; ind++)
		rsum += ((traceAdjacentColors[ind] > traceAdjacentColors[0]) ? (traceAdjacentColors[ind] - traceAdjacentColors[0]) : (traceAdjacentColors[0] - traceAdjacentColors[ind]));
	return rsum;
}

void trdif() {

	unsigned	ind, ine, inf, l_bpnt = 0, rsum, rmax, rmin;
	double		rat;

	if (!*bnam) {

		tabmsg(IDS_MAPLOD);
		return;
	}
	rstMap(TRSET);
	rstMap(HIDMAP);
	untrace();
	differenceBitmap = (unsigned*)bseq;
	rmax = 0;
	rmin = 0xffffffff;
	if (!chkMap(WASTRAC))
		getrmap();
	ind = 0;
	for (ind = 0; ind < 3; ind++) {

		blanklin(0);
		for (ine = 1; ine < (unsigned)bitmapHeight - 1; ine++) {

			l_bpnt = ine*bitmapWidth;
			differenceBitmap[l_bpnt++] = 0;
			for (inf = 1; inf < bitmapWidth - 1; inf++) {

				difbits(trshft[ind], &traceBitmap[l_bpnt]);
				rsum = differenceBitmap[l_bpnt] = trsum();
				l_bpnt++;
				if (rsum > rmax)
					rmax = rsum;
				if (rsum < rmin)
					rmin = rsum;
			}
			differenceBitmap[l_bpnt++] = 0;
		}
		blanklin(l_bpnt);
		rat = (double)255 / (rmax - rmin);
		for (ine = 0; ine < bitmapWidth*bitmapHeight; ine++) {

			traceBitmap[ine] &= traceRGBMask[ind];
			if (differenceBitmap[ine]) {

				inf = (differenceBitmap[ine] - rmin)*rat;
				traceBitmap[ine] |= inf << trshft[ind];
			}
		}
	}
	BitBlt(bitmapDC, 0, 0, bitmapWidth, bitmapHeight, tracedc, 0, 0, SRCCOPY);
	setMap(WASDIF);
	setMap(RESTCH);
	tracwnd();
}

void delstch() {

	savdo();
	hed.stitchCount = 0;
	clpad = 0;
	txad = 0;
	rstAll();
	clrfills();
	colCnt = 0;
	butxt(HNUM, "");
	butxt(HTOT, "");
	setMap(RESTCH);
}

void chkbit() {

	if (*bnam && (chkMap(WASDIF) || chkMap(WASDSEL) || chkMap(WASBLAK))) {

		setMap(WASESC);
		bfil();
	}
}

void trcnum(unsigned shft, COLORREF col, unsigned ind) {

	unsigned len;
	unsigned nwid;

	col >>= shft;
	col &= 0xff;
	_itoa_s(col, trbuf, 10);
	len = strlen(trbuf);
	nwid = numwid*(3 - len) + 1;
	SetBkColor(ds->hDC, traceRGB[ind]);
	TextOut(ds->hDC, nwid, 1, trbuf, strlen(trbuf));
}

void upnum(unsigned ind) {

	trcnum(trshft[ind], invertUpColor, ind);
}

void dwnum(unsigned ind) {

	trcnum(trshft[ind], invertDownColor, ind);
}

void ritrcol(COLORREF* tref, unsigned num) {

	*tref &= traceRGBMask[colm];
	num &= 0xff;
	*tref |= (num << trshft[colm]);
}

void dutrnum0(unsigned num) {

	rstMap(NUMIN);
	rstMap(TRNIN0);
	if (chkMap(TRNUP)) {

		ritrcol(&invertUpColor, num);
		upColor = invertUpColor ^ 0xffffff;
		redraw(htrup[colm]);
	} else {

		ritrcol(&invertDownColor, num);
		downColor = invertDownColor ^ 0xffffff;
		redraw(htrdwn[colm]);
	}
	redraw(hctrc[colm]);
	DestroyWindow(htrnum);
	setMap(WASTRCOL);
	trace();
}

void dutrnum1() {

	double	tdub;

	DestroyWindow(hgNum);
	rstMap(NUMIN);
	rstMap(TRNIN1);
	tdub = atof(msgbuf);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
	if (tdub > 9)
		tdub = 9;
	if (chkMap(TRNUP)) {

		ini.traceLength = tdub*PFGRAN;
		trcstpnum();
	} else {

		ini.traceRatio = 1 + pow(0.1, tdub);
		trcratnum();
	}
}

void trcsel() {

	unsigned ind, max, xind;

	if (*bnam) {

		setMap(WASTRCOL);
		setMap(TRCRED);
		setMap(TRCBLU);
		setMap(TRCGRN);
		downColor = 0xffffff;
		upColor = 0;
		trace();
		rstMap(HIDMAP);
		rstMap(TRSET);
		for (ind = 0; ind < bitmapWidth*bitmapHeight; ind++) {

			trcols(traceBitmap[ind]);
			max = pixelColors[0];
			xind = 2;
			if (pixelColors[1] > max) {

				max = pixelColors[1];
				xind = 1;
			}
			if (pixelColors[2] > max)
				xind = 0;
			traceBitmap[ind] &= traceRGB[xind];
		}
		BitBlt(bitmapDC, 0, 0, bitmapWidth, bitmapHeight, tracedc, 0, 0, SRCCOPY);
		setMap(WASDSEL);
		setMap(RESTCH);
		tracwnd();
	} else
		tabmsg(IDS_MAPLOD);
}

void trinit() {

	unsigned	ind, ine;
	unsigned	hst[3][256];
	unsigned*	phst;
	unsigned	hi[3];
	unsigned	col[3] = { 0 };
	COLORREF	tcol;

	if (*bnam) {

		if (!chkMap(TRSET)) {

			setMap(TRCRED);
			setMap(TRCGRN);
			setMap(TRCBLU);
			if (!chkMap(WASTRAC))
				getrmap();
			if (chkMap(MONOMAP)) {

				tcol = traceBitmap[0];
				for (ind = 0; ind < bitmapWidth*bitmapHeight; ind++) {

					if (traceBitmap[ind] != tcol)
						break;
				}
				if (ind < bitmapWidth*bitmapHeight) {

					trcols(traceBitmap[ind]);
					highColors[0] = pixelColors[0];
					highColors[1] = pixelColors[1];
					highColors[2] = pixelColors[2];
					trcols(tcol);
					for (ind = 0; ind < 3; ind++) {

						if (pixelColors[ind] > highColors[ind]) {

							ine = pixelColors[ind];
							pixelColors[ind] = highColors[ind];
							highColors[ind] = ine;
						}
						col[ind] = ((highColors[ind] - pixelColors[ind]) >> 1) + pixelColors[ind];
					}
				}
			} else {

				phst = (unsigned*)hst;
				for (ind = 0; ind < 768; ind++)
					phst[ind] = 0;
				for (ind = 0; ind < bitmapWidth*bitmapHeight; ind++) {

					trcols(traceBitmap[ind]);
					for (ine = 0; ine < 3; ine++)
						hst[ine][pixelColors[ine]]++;
				}
				for (ind = 0; ind < 3; ind++)
					hi[ind] = 0;
				for (ind = 0; ind < 256; ind++) {

					for (ine = 0; ine < 3; ine++) {

						if (hst[ine][ind] > hi[ine]) {

							hi[ine] = hst[ine][ind];
							col[ine] = ind;
						}
					}
				}
			}
			invertDownColor = 0;
			for (ind = 0; ind < 3; ind++) {

				if (col[ind])
					col[ind]--;
				invertDownColor |= col[ind] << trshft[ind];
			}
			downColor = invertDownColor ^ 0xffffff;
			invertUpColor = 0xffffff;
			upColor = 0;
		}
		setMap(WASTRCOL);
		trace();
	} else
		tabmsg(IDS_MAPLOD);
}

void stch2bit(double px, double py) {

	if (chkMap(LANDSCAP))
		py -= (unzoomedRect.y - bitmapSizeinStitches.y);
	bitpnt.x = bmpStitchRatio.x*px;
	bitpnt.y = (bitmapHeight - bmpStitchRatio.y*py);
}

void pxlin(unsigned strt, unsigned fin) {

	POINT		lin[2];

	stch2bit(currentFormVertices[strt].x, currentFormVertices[strt].y);
	lin[0].x = bitpnt.x;
	lin[0].y = bitpnt.y;
	stch2bit(currentFormVertices[fin].x, currentFormVertices[fin].y);
	lin[1].x = bitpnt.x;
	lin[1].y = bitpnt.y;
	Polyline(bitmapDC, lin, 2);
	Polyline(tracedc, lin, 2);
}

void bfrm() {

	unsigned ind;

	if (sides) {

		for (ind = 0; ind < (unsigned)sides - 1; ind++)
			pxlin(ind, ind + 1);
		if (ptrSelectedForm->type != LIN)
			pxlin(ind, 0);
	}
}

void blak() {

	unsigned ind;

	if (!*bnam) {

		tabmsg(IDS_MAPLOD);
		return;
	}
	tracwnd();
	if (formIndex) {

		blackPen = CreatePen(PS_SOLID, 1, 0);
		SelectObject(bitmapDC, blackPen);
		SelectObject(tracedc, blackPen);
		if (!chkMap(WASTRAC))
			getrmap();
		for (ind = 0; ind < formIndex; ind++) {

			fvars(ind);
			bfrm();
		}
		DeleteObject(blackPen);
		setMap(WASBLAK);
		setMap(RESTCH);
	} else
		tabmsg(IDS_FRMNO);
}

void delmap() {

	*bnam = 0;
	setMap(RESTCH);
}

void closfn() {

	deltot();
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], ini.designerName);
	knotcnt = 0;
	*filnam = 0;
	*bnam = 0;
	deldu();
	clrhbut(3);
	SetWindowText(hWnd, msgbuf);
}

void filclos() {

	if (savcmp())
		closfn();
	else {

		setMap(FCLOS);
		savdisc();
	}
}

void frmpos(float dx, float dy) {

	unsigned ine;

	for (ine = 0; ine < ptrSelectedForm->sides; ine++) {

		ptrSelectedForm->vertices[ine].x += dx;
		ptrSelectedForm->vertices[ine].y += dy;
	}
	ptrSelectedForm->rectangle.bottom += dy;
	ptrSelectedForm->rectangle.top += dy;
	ptrSelectedForm->rectangle.left += dx;
	ptrSelectedForm->rectangle.right += dx;
}

void nudgfn(float dx, float dy) {

	unsigned	ind, rcnt;
	POINT		pix;

	if (chkMap(BIGBOX) || selectedFormCount || chkMap(FORMSEL) || chkMap(GRPSEL) || chkMap(SELBOX))
		savdo();
	if (chkMap(BIGBOX)) {

		for (ind = 0; ind < formIndex; ind++) {

			ptrSelectedForm = &formList[ind];
			frmpos(dx, dy);
		}
		for (ind = 0; ind < hed.stitchCount; ind++) {

			stitchBuffer[ind].x += dx;
			stitchBuffer[ind].y += dy;
		}
		allItemsRectangle.bottom += dy;
		allItemsRectangle.top += dy;
		allItemsRectangle.left += dx;
		allItemsRectangle.right += dx;
		stchrct2px(allItemsRectangle, &selectedFormsRectangle);
		setMap(RESTCH);
		return;
	}
	if (selectedFormCount) {

		rcnt = (formIndex >> 5) + 1;
		clRmap(rcnt);
		for (ind = 0; ind < selectedFormCount; ind++)
			setr(selectedFormList[ind]);
		for (ind = 0; ind < hed.stitchCount; ind++) {

			if (chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {

				stitchBuffer[ind].x += dx;
				stitchBuffer[ind].y += dy;
			}
		}
		for (ind = 0; ind < selectedFormCount; ind++) {

			ptrSelectedForm = &formList[selectedFormList[ind]];
			frmpos(dx, dy);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL)) {

		ptrSelectedForm = &formList[closestFormToCursor];
		frmpos(dx, dy);
		if (ptrSelectedForm->fillType || ptrSelectedForm->edgeType) {

			for (ind = 0; ind < hed.stitchCount; ind++) {

				if ((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor) {

					stitchBuffer[ind].x += dx;
					stitchBuffer[ind].y += dy;
				}
			}
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		for (ind = groupStartStitch; ind <= groupEndStitch; ind++) {

			stitchBuffer[ind].x += dx;
			stitchBuffer[ind].y += dy;
		}
		grpAdj();
		setMap(RESTCH);
		return;
	}
	if (chkMap(SELBOX)) {

		stitchBuffer[closestPointIndex].x += dx;
		stitchBuffer[closestPointIndex].y += dy;
		setMap(RESTCH);
		return;
	}
	pix.x = pix.y = 0;
	if (dx) {

		if (dx > 0)
			pix.x = ini.nudgePixels;
		else
			pix.x = -ini.nudgePixels;
	}
	if (dy) {

		if (dy > 0)
			pix.y = -ini.nudgePixels;
		else
			pix.y = +ini.nudgePixels;
	}
	mouse_event(MOUSEEVENTF_MOVE, pix.x, pix.y, 0, 0);
}

void pixmsg(unsigned cod, unsigned pix) {

	sprintf_s(msgbuf, sizeof(msgbuf), stab[cod], pix);
	shoMsg(msgbuf);
}

void getnpix() {

	pixmsg(STR_NUDG, ini.nudgePixels);
	setMap(NUMIN);
	setMap(PIXIN);
	numWnd();
}

void getstpix() {

	pixmsg(STR_STCHP, ini.stitchSizePixels);
	setMap(NUMIN);
	setMap(STPXIN);
	numWnd();
}

void getfrmpix() {

	pixmsg(STR_FRMP, ini.formPointSizePixels);
	setMap(NUMIN);
	setMap(FRMPXIN);
	numWnd();
}

void getfrmbox() {

	pixmsg(STR_FRMBOX, ini.formBoxSizePixels);
	setMap(NUMIN);
	setMap(FRMBOXIN);
	numWnd();
}

void bakmrk() {

	fPOINT	tpnt;

	if (chkMap(WASMRK)) {

		if (zoomMarkPoint.x > ini.hoopSizeX)
			zoomMarkPoint.x = ini.hoopSizeY / 2;
		if (zoomMarkPoint.y > ini.hoopSizeY)
			zoomMarkPoint.y = ini.hoopSizeY / 2;
		dumrk(zoomMarkPoint.x, zoomMarkPoint.y);
		tpnt.x = zoomMarkPoint.x;
		tpnt.y = zoomMarkPoint.y;
		shft(tpnt);
		setMap(RESTCH);
	} else
		tabmsg(IDS_MRK);
}

void nuscol(unsigned ind) {

	userPen[ind] = nuPen(userPen[ind], 1, useCol[ind]);
	hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], useCol[ind]);
	redraw(hCol[ind]);
}

void movchk() {

	unsigned	ind, col, key, swtch;
	FRMHED*		pfrm;

	if (msg.wParam&MK_LBUTTON) {

		if (!setMap(WASMOV)) {

			if (chkMsgs(msg.pt, hDef[0], hDef[15])) {

				draggedColor = vind & 0xf;
				setMap(WASCOL);
			}
		}
	} else {

		if (rstMap(WASMOV) && rstMap(WASCOL)) {

			if (chkMsgs(msg.pt, hDef[0], hDef[15])) {

				key = GetKeyState(VK_SHIFT) & 0X8000;
				swtch = GetKeyState(VK_CONTROL) & 0X8000;
				for (ind = 0; ind < hed.stitchCount; ind++) {

					col = stitchBuffer[ind].attribute&COLMSK;
					if (col == vind) {

						stitchBuffer[ind].attribute &= NCOLMSK;
						stitchBuffer[ind].attribute |= draggedColor;
					} else {

						if (!key&&col == draggedColor) {

							stitchBuffer[ind].attribute &= NCOLMSK;
							stitchBuffer[ind].attribute |= vind;
						}
					}
				}
				for (ind = 0; ind < formIndex; ind++) {

					pfrm = &formList[ind];
					if (pfrm->fillType) {

						if (pfrm->fillColor == vind)
							pfrm->fillColor = draggedColor;
						else {

							if (!key&&pfrm->fillColor == draggedColor)
								pfrm->fillColor = vind;
						}
						if (pfrm->fillInfo.feather.color == vind)
							pfrm->fillInfo.feather.color = draggedColor;
						else {

							if (!key&&pfrm->fillInfo.feather.color == draggedColor)
								pfrm->fillInfo.feather.color = vind;
						}
					}
					if (pfrm->edgeType) {

						if (pfrm->borderColor == vind)
							pfrm->borderColor = draggedColor;
						else {

							if (!key&&pfrm->borderColor == draggedColor)
								pfrm->borderColor = vind;
						}
					}
				}
				if (!swtch) {

					ind = useCol[vind];
					useCol[vind] = useCol[draggedColor];
					if (!key) {

						useCol[draggedColor] = ind;
						nuscol(draggedColor);
					}
					nuscol(vind);
				}
				coltab();
				setMap(RESTCH);
			}
		}
	}
}

void inscol() {

	unsigned	ind, dcol, col;
	FRMHED*		tfrm;

	*markedStitchMap = 0;
	if (chkMsgs(msg.pt, hDef[0], hCol[15])) {

		vind &= COLMSK;
		for (ind = 0; ind < hed.stitchCount; ind++)
			setr(stitchBuffer[ind].attribute&COLMSK);
		if (*markedStitchMap == COLSMSK)
			tabmsg(IDS_COLAL);
		else {

			dcol = 15;
			while (chkr(dcol))
				dcol--;
			for (ind = 0; ind < hed.stitchCount; ind++) {

				col = stitchBuffer[ind].attribute&COLMSK;
				if (col >= vind&&col < dcol) {

					stitchBuffer[ind].attribute &= NCOLMSK;
					stitchBuffer[ind].attribute |= col + 1;
				}
			}
			for (ind = 0; ind < formIndex; ind++) {

				tfrm = &formList[ind];
				if (tfrm->fillType) {

					if (tfrm->fillColor >= vind&&tfrm->fillColor < dcol)
						tfrm->fillColor++;
					if (tfrm->fillInfo.feather.color >= vind&&tfrm->fillInfo.feather.color < dcol)
						tfrm->fillInfo.feather.color++;
				}
				if (tfrm->edgeType) {

					if (tfrm->borderColor >= vind&&tfrm->borderColor < dcol)
						tfrm->borderColor++;
				}
			}
			for (ind = dcol; ind > vind; ind--) {

				useCol[ind] = useCol[ind - 1];
				nuscol(ind);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

BOOL usedcol() {

	unsigned ind;

	for (ind = 0; ind < hed.stitchCount; ind++) {

		if ((stitchBuffer[ind].attribute&COLMSK) == vind)
			return 1;
	}
	return 0;
}

void delcol() {

	unsigned	ind, col;
	FRMHED*		tfrm;

	if (chkMsgs(msg.pt, hDef[0], hCol[15])) {

		vind &= 0xf;
		if (usedcol())
			tabmsg(IDS_COLU);
		else {

			for (ind = 0; ind < hed.stitchCount; ind++) {

				col = stitchBuffer[ind].attribute&COLMSK;
				if (col > vind&&col) {

					stitchBuffer[ind].attribute &= NCOLMSK;
					stitchBuffer[ind].attribute |= col - 1;
				}
			}
			for (ind = 0; ind < formIndex; ind++) {

				tfrm = &formList[ind];
				if (tfrm->fillType) {

					if (tfrm->fillColor > vind)
						tfrm->fillColor--;
					if (tfrm->fillInfo.feather.color > vind)
						tfrm->fillInfo.feather.color--;
				}
				if (tfrm->edgeType) {

					if (tfrm->borderColor > vind)
						tfrm->borderColor--;
				}
			}
			for (ind = vind; ind < 15; ind++) {

				useCol[ind] = useCol[ind + 1];
				nuscol(ind);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void set1knot() {

	unsigned src, dst;

	if (hed.stitchCount&&chkMap(SELBOX)) {

		savdo();
		if (closestPointIndex == (unsigned)hed.stitchCount - 1) {

			setMap(FILDIR);
			opnt = closestPointIndex + 1;
			endknt(closestPointIndex);
		} else {

			src = hed.stitchCount - 1;
			dst = hed.stitchCount + 4;
			while (src&&src >= closestPointIndex)
				mvstch(dst--, src--);
			opnt = closestPointIndex + 1;
			strtknt(closestPointIndex);
		}
		hed.stitchCount += 5;
		coltab();
		setMap(RESTCH);
	} else {

		msgstr(IDS_NOSTCHSEL);
		shoMsg(msgbuf);
	}
}

void selfrm0() {

	rstMap(GRPSEL);
	if (rstMap(FORMSEL)) {

		setMap(FRMPSEL);
		closestVertexToCursor = 0;
	}
	setMap(RESTCH);
}

void selfrmx() {

	rstMap(GRPSEL);
	if (rstMap(FORMSEL)) {

		setMap(FRMPSEL);
		closestVertexToCursor = formList[closestFormToCursor].sides - 1;
	}
	setMap(RESTCH);
}

void setpclp() {

	POINT		tof;
	POINT		tpnt;
	unsigned	ind, ine;

	sfCor2px(iseq[0], &tpnt);
	fplin[0].x = tpnt.x;
	fplin[0].y = tpnt.y;
	sfCor2px(iseq[1], &tpnt);
	tof.x = msg.pt.x - stitchWindowOrigin.x - tpnt.x;
	tof.y = msg.pt.y - stitchWindowOrigin.y - tpnt.y;
	for (ind = 0; ind < opnt - 2; ind++) {

		ine = ind + 1;
		sfCor2px(iseq[ine], &tpnt);
		fplin[ine].x = tpnt.x + tof.x;
		fplin[ine].y = tpnt.y + tof.y;
	}
	ind++;
	sfCor2px(iseq[ind], &tpnt);
	fplin[ind].x = tpnt.x;
	fplin[ind].y = tpnt.y;
}

void dupclp() {

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, formPen);
	Polyline(StitchWindowDC, fplin, opnt);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unpclp() {

	if (rstMap(SHOP))
		dupclp();
}

void fixpclp() {

	POINT		tpnt;
	fPOINT		pof;
	unsigned	ind, ine, cnt;

	tpnt.x = msg.pt.x + fmovdif.x;
	tpnt.y = msg.pt.y + fmovdif.y;
	pxCor2stch(tpnt);
	pof.x = sPnt.x - iseq[1].x;
	pof.y = sPnt.y - iseq[1].y;
	ine = nxt(closestVertexToCursor);
	cnt = opnt - 2;
	fltspac(&currentFormVertices[ine], cnt);
	for (ind = 1; ind < opnt - 1; ind++) {

		currentFormVertices[ine].x = iseq[ind].x + pof.x;
		currentFormVertices[ine].y = iseq[ind].y + pof.y;
		ine++;
	}
	ptrSelectedForm->sides += cnt;
	refil();
	coltab();
	setMap(RESTCH);
}

BOOL sidclp() {

	fvars(closestFormToCursor);
	if (OpenClipboard(hWnd)) {

		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {

			redclp();
			CloseClipboard();
			return 1;
		}
	}
	CloseClipboard();
	return 0;
}

void selfpnt() {

	rstMap(FORMSEL);
	setMap(FRMPSEL);
	rstMap(FPSEL);
	rstMap(SELBOX);
	selectedFormPoints.start = closestVertexToCursor;
	selectedFormPoints.form = closestFormToCursor;
	ritfcor(&formList[closestFormToCursor].vertices[closestVertexToCursor]);
	setMap(RESTCH);
}

unsigned chkMsg() {

	unsigned		cod, dst;
	unsigned		ine, inf;
	long			ind;
	long			tlng;
	TCHAR			cmap[] = { '3','4','6' };
	TCHAR			buf[20];
	double			tdub;
	fPOINT			nusiz;
	fPOINT			padj;
	RECT			trct;
	SATCON*			l_sac;
	fPOINT*			clp;
	POINT			tpnt;
	FRMHED*			tfrm;
	fRECTANGLE			tbig;
	unsigned char*	pchr;
	TXPNT*			ptx;
	TXPNT*			pts;

	if (msg.message == WM_MOUSEMOVE) {

		if (chkMap(TXTMOV))
		{
			txtrmov();
			return 1;
		}
		movchk();
		if (msg.pt.x >= stitchWindowAbsRect.left&&msg.pt.x <= stitchWindowAbsRect.right
			&&msg.pt.y >= stitchWindowAbsRect.top&&msg.pt.y <= stitchWindowAbsRect.bottom) {

			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				mvshft();
				return 1;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000 && px2stch())
				ritfcor(&sPnt);
			if (*flagMap&(PRFACTB | FORMINB | POLIMOVB) || hfdat) {

				SetCursor(hArrowCurs);
				goto gotcur;
			}
			if (!chkMap(INIT)) {

				if (chku(NEDOF))
					SetCursor(hCrossCurs);
				else
					SetCursor(hNeedleUpCurs);
				goto gotcur;
			}
			if (chkMap(INSRT)) {

				if (chku(NEDOF))
					SetCursor(hCrossCurs);
				else {

					if (chkMap(LIN1))
						SetCursor(hNeedleUpCurs);
					else {

						if (stitchBuffer[closestPointIndex + 1].x > stitchBuffer[closestPointIndex].x) {

							if (stitchBuffer[closestPointIndex + 1].y > stitchBuffer[closestPointIndex].y)
								SetCursor(hNeedleLeftUpCurs);
							else
								SetCursor(hNeedleLeftDownCurs);
						} else {

							if (stitchBuffer[closestPointIndex + 1].y > stitchBuffer[closestPointIndex].y)
								SetCursor(hNeedleRightUpCurs);
							else
								SetCursor(hNeedleRightDownCurs);
						}
					}
				}
				goto gotcur;
			}
			if (*flagMap&(BZUMINB | BOXZUMB | SELPNTB)) {

				SetCursor(hCrossCurs);
				goto gotcur;
			}
			if (*flagMap&(SATINB | SATPNTB | INSFRMB)) {

				if (chku(FRMX))
					SetCursor(hCrossCurs);
				else
					SetCursor(hFormCurs);
				goto gotcur;
			}
			if (*flagMap&SATCNKTB)
				SetCursor(hDlinCurs);
			else
				SetCursor(hArrowCurs);
		gotcur:;
			if (chkMap(FPUNCLP)) {

				unpclp();
				setpclp();
				setMap(SHOP);
				dupclp();
			}
			if (chkMap(INSFIL)) {

				unfrm();
				tpnt.x = msg.pt.x - stitchWindowOrigin.x;
				tpnt.y = msg.pt.y - stitchWindowOrigin.y;
				insflin(tpnt);
				setMap(SHOFRM);
				dufrm();
				return 1;
			}
			if (chkMap(MOVFRMS)) {

				unstrtch();
				sizlin[0].x = sizlin[3].x = sizlin[4].x = msg.pt.x - fmovdif.x - stitchWindowOrigin.x;
				sizlin[1].x = sizlin[2].x = msg.pt.x + bigsiz.x - fmovdif.x - stitchWindowOrigin.x;
				sizlin[0].y = sizlin[1].y = sizlin[4].y = msg.pt.y - fmovdif.y - stitchWindowOrigin.y;
				sizlin[2].y = sizlin[3].y = msg.pt.y + bigsiz.y - fmovdif.y - stitchWindowOrigin.y;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			}
			if (chkMap(POLIMOV)) {

				munfrm();
				setmfrm();
				setMap(SHOFRM);
				mdufrm();
				return 1;
			}
			if (chkMap(EXPAND)) {

				unstrtch();
				nusiz.x = msg.pt.x - stitchWindowOrigin.x;
				nusiz.y = msg.pt.y - stitchWindowOrigin.y;
				ind = (selectedFormControlVertex + 2) % 4;
				tdub = fabs((double)(nusiz.x - sizlin[ind].x) / (nusiz.y - sizlin[ind].y));
				if (ind & 1) {

					if (tdub < xyRatio)
						nusiz.x = (sizlin[ind].y - nusiz.y)*xyRatio + sizlin[ind].x;
					else
						nusiz.y = (sizlin[ind].x - nusiz.x) / xyRatio + sizlin[ind].y;
					ind = nxtcrnr(ind);
					sizlin[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					sizlin[ind].x = nusiz.x;
					sizlin[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					sizlin[ind].x = nusiz.x;
				} else {

					if (tdub < xyRatio)
						nusiz.x = (nusiz.y - sizlin[ind].y)*xyRatio + sizlin[ind].x;
					else
						nusiz.y = (nusiz.x - sizlin[ind].x) / xyRatio + sizlin[ind].y;
					ind = nxtcrnr(ind);
					sizlin[ind].x = nusiz.x;
					ind = nxtcrnr(ind);
					sizlin[ind].x = nusiz.x;
					sizlin[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					sizlin[ind].y = nusiz.y;
				}
				sizlin[4].x = sizlin[0].x;
				sizlin[4].y = sizlin[0].y;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			}
			if (chkMap(STRTCH)) {

				unstrtch();
				if (selectedFormControlVertex & 1)
					tlng = msg.pt.x - stitchWindowOrigin.x;
				else
					tlng = msg.pt.y - stitchWindowOrigin.y;
				dst = (selectedFormControlVertex + 2) % 4;
				cod = nxtcrnr(dst);
				for (ind = 0; ind < 4; ind++) {

					if ((unsigned)ind != dst && (unsigned)ind != cod) {

						if (selectedFormControlVertex & 1)
							sizlin[ind].x = tlng;
						else
							sizlin[ind].y = tlng;
					}
				}
				sizlin[4].x = sizlin[0].x;
				sizlin[4].y = sizlin[0].y;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			}
			if (chkMap(INSFRM)) {

				uninsf();
				inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
				inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
				setMap(SHOINSF);
				duinsf();
				return 1;
			}
			if (chkMap(FUNCLP)) {

				unfrm();
				setmfrm();
				setMap(SHOFRM);
				dufrm();
				return 1;
			}
			if (chkMap(SATCNKT)) {

				drwcon();
				return 1;
			}
			if (chkMap(SATPNT)) {

				drwsat();
				return 1;
			}
			if (chkMap(FRMOV)) {

				munfrm();
				setmfrm();
				setMap(SHOFRM);
				mdufrm();
				return 1;
			}
			if (chkMap(FRMPMOV)) {

				unmov();
				mvlin[1].x = msg.pt.x - stitchWindowOrigin.x;
				mvlin[1].y = msg.pt.y - stitchWindowOrigin.y;
				setMap(SHOMOV);
				ritmov();
				if (px2stch())
					ritfcor(&sPnt);
				return 1;
			}
			if (chkMap(MOVCNTR)) {

				unrot();
				px2stch();
				rotcntr.x = sPnt.x;
				rotcntr.y = sPnt.y;
				ritrot();
				return 1;
			}
			if (chkMap(ROTCAPT)) {

				unrotu();
				unrot();
				rlinu[1].x = msg.pt.x - stitchWindowOrigin.x;
				rlinu[1].y = msg.pt.y - stitchWindowOrigin.y;
				padj.x = rlinu[0].x - rlinu[1].x;
				padj.y = rlinu[0].y - rlinu[1].y;
				if (padj.x)
					rotationAngle = -atan2(padj.y, padj.x);
				else {

					if (padj.y > 0)
						rotationAngle = PI / 2;
					else
						rotationAngle = -PI / 2;
				}
				rotationAngle -= rotationHandleAngle;
				ritrot();
				setMap(ROTUSHO);
				durotu();
				return 1;
			}
			if (chkMap(SELPNT)) {

				if (setMap(VCAPT))
					SetCapture(hWnd);
				unsel();
				rSelbox();
				return 1;
			}
			if (chkMap(CLPSHO)) {

				unclp();
				clpbox();
				return 1;
			}
			if (chkMap(CAPT)) {

				if (px2stch())
					ritfcor(&sPnt);
				unlin();
				mlin0[1].x = mlin1[0].x = msg.pt.x - stitchWindowOrigin.x;
				mlin0[1].y = mlin1[0].y = msg.pt.y - stitchWindowOrigin.y;
				dulin();
				return 1;
			}
			if (chkMap(INSRT)) {

				if (px2stch())
					ritfcor(&sPnt);
				if (setMap(VCAPT))
					SetCapture(hWnd);
				if (chkMap(LIN1)) {

					if (hed.stitchCount) {

						xlin1();
						inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
						inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
						setMap(ILIN1);
						ilin1();
					}
					return 1;
				}
				xlin();
				inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
				inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
				setMap(ILIN);
				ilin();
				return 1;
			}
			if (chkMap(BOXZUM) && setMap(VCAPT))
				SetCapture(hWnd);
			if (chkMap(BZUMIN) && (msg.wParam&MK_LBUTTON)) {

				if (setMap(VCAPT))
					SetCapture(hWnd);
				unbBox();
				blin[1].x = blin[2].x = msg.pt.x - stitchWindowOrigin.x;
				blin[2].y = blin[3].y = msg.pt.y - stitchWindowOrigin.y;
				setMap(BZUM);
				bBox();
				return 1;
			}
		} else {

			if (rstMap(VCAPT))
				ReleaseCapture();
		}
		return 1;
	}
	if (msg.message == WM_LBUTTONUP)
	{
		if (GetKeyState(VK_SHIFT) & 0X8000 && px2stch())
		{
			setshft();
			return 1;
		}
		if (chkMap(TXTRED))
		{
			txtrup();
			return 1;
		}
		ReleaseCapture();
		movchk();
		if (rstMap(MOVFRMS)) {

			savdo();
			tpnt.x = (msg.pt.x - fmovdif.x - stitchWindowOrigin.x) - selectedFormsRectangle.left;
			tpnt.y = (msg.pt.y - fmovdif.y - stitchWindowOrigin.y) - selectedFormsRectangle.top;
			ratsr();
			fmovdif.x = tpnt.x / horizontalRatio;
			fmovdif.y = tpnt.y / verticalRatio;
			if (chkMap(FPSEL)) {

				fvars(closestFormToCursor);
				ine = selectedFormPoints.start;
				for (ind = 0; (unsigned)ind <= selectedFormPoints.pointCount; ind++) {

					currentFormVertices[ine].x += fmovdif.x;
					currentFormVertices[ine].y -= fmovdif.y;
					ine = pdir(ine);
				}
				setpsel();
				frmout(closestFormToCursor);
				refil();
				setMap(RESTCH);
			} else {

				if (chkMap(BIGBOX)) {

					savdo();
					for (ind = 0; ind < (long)formIndex; ind++)
						frmadj(ind);
					for (ind = 0; ind < hed.stitchCount; ind++) {

						stitchBuffer[ind].x += fmovdif.x;
						stitchBuffer[ind].y -= fmovdif.y;
					}
					selal();
				} else {

					savdo();
					for (cod = 0; cod < selectedFormCount; cod++)
						frmadj(selectedFormList[cod]);
					frmsadj();
					setMap(RESTCH);
				}
			}
			return 1;
		}
		if (rstMap(EXPAND)) {

			setexpand();
			return 1;
		}
		if (rstMap(STRTCH)) {

			setstrtch();
			return 1;
		}
		if (rstMap(FRMOV)) {

			savdo();
			rstfrm();
			setMap(RESTCH);
			return 1;
		}
		if (rstMap(FRMPMOV)) {

			savdo();
			setfpnt();
			return 1;
		}
		if (rstMap(MOVCNTR)) {

			setMap(ROTAT);
			return 1;
		}
		if (rstMap(ROTCAPT)) {

			rotfn();
			return 1;
		}
		if (rstMap(SELPNT)) {

			savdo();
			ReleaseCapture();
			unsel();
			padj.x = (rngrct.left + selectBoxOffset.x) - sPnt.x;
			padj.y = (rngrct.bottom + selectBoxOffset.y) - sPnt.y;
			for (ind = groupStartStitch; ind <= (long)groupEndStitch; ind++) {

				stitchBuffer[ind].x -= padj.x;
				stitchBuffer[ind].y -= padj.y;
			}
			setMap(RESTCH);
			return 1;
		}
		if (rstMap(CAPT)) {

			unlin();
			ReleaseCapture();
			rstMap(CAPT);
			px2stch();
			stitchBuffer[closestPointIndex].x = sPnt.x;
			stitchBuffer[closestPointIndex].y = sPnt.y;
			stitchBuffer[closestPointIndex].attribute |= USMSK;
			if (zoomFactor < STCHBOX) {

				SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
				SelectObject(StitchWindowMemDC, linePen);
				stchbox(closestPointIndex - 1, StitchWindowDC);
				stchbox(closestPointIndex, StitchWindowDC);
				stchbox(closestPointIndex + 1, StitchWindowDC);
				SetROP2(StitchWindowMemDC, R2_COPYPEN);
			}
			setMap(SELBOX);
			rstMap(FRMPSEL);
			setMap(RESTCH);
			return 1;
		}
		if (chkMap(BZUMIN)) {

			px2stch();
			if (rstMap(BOXSLCT)) {

				if (zBoxo.x > sPnt.x) {

					rngrct.right = zBoxo.x;
					rngrct.left = sPnt.x;
				} else {

					rngrct.right = sPnt.x;
					rngrct.left = zBoxo.x;
				}
				if (zBoxo.y > sPnt.y) {

					rngrct.top = zBoxo.y;
					rngrct.bottom = sPnt.y;
				} else {

					rngrct.top = sPnt.y;
					rngrct.bottom = zBoxo.y;
				}
				if (rstMap(GRPSEL)) {

					rngadj();
					for (ine = groupStartStitch; ine < groupEndStitch; ine++) {

						if (inrng(ine)) {

							closestPointIndex = ine;
							setMap(SELBOX);
							break;
						}
					}
				}
				if (rstMap(NOSEL)) {

					selectedFormCount = 0;
					rstMap(FORMSEL);
					for (ine = 0; ine < formIndex; ine++) {

						if (finrng(ine))
							selectedFormList[selectedFormCount++] = ine;
					}
					if (selectedFormCount) {

						if (selectedFormCount == 1) {

							ReleaseCapture();
							gotbox();
							closestFormToCursor = selectedFormList[0];
							closestVertexToCursor = 0;
							selectedFormCount = 0;
							ritnum(STR_NUMFORM, closestFormToCursor);
							setMap(RESTCH);
							setMap(FORMSEL);
							return 1;
						}
					}
					if (selectedFormCount) {

						gotbox();
						return 1;
					}
					rstMap(BZUMIN);
					rstMap(BOXSLCT);
					rstMap(FRMPSEL);
					setMap(RESTCH);
				}
				if (rstMap(SELBOX)) {

					if (inrng(closestPointIndex)) {

						groupStitchIndex = closestPointIndex + 1;
						closestPointIndex--;
						while (inrng(closestPointIndex))
							closestPointIndex--;
						closestPointIndex++;
						while (inrng(groupStitchIndex))
							groupStitchIndex++;
						groupStitchIndex--;
						setMap(GRPSEL);
						gotbox();
						return 1;
					}
				}
				if (!chkMap(INSRT))
					rngal();
				//				gotbox();
				return 1;
			} else {

				if (sPnt.x < zBoxo.x) {

					tdub = zBoxo.x;
					zBoxo.x = sPnt.x;
					sPnt.x = tdub;
				}
				if (sPnt.y < zBoxo.y) {

					tdub = zBoxo.y;
					zBoxo.y = sPnt.y;
					sPnt.y = tdub;
				}
				nusiz.x = sPnt.x - zBoxo.x;
				nusiz.y = sPnt.y - zBoxo.y;
				sPnt.x = zBoxo.x + nusiz.x / 2;
				sPnt.y = zBoxo.y + nusiz.y / 2;
				tdub = zoomFactor;
				if (nusiz.x > nusiz.y) {

					nusiz.y = nusiz.x / stitchWindowAspectRatio;
					zoomFactor = nusiz.x / unzoomedRect.x;
				} else {

					nusiz.x = nusiz.y*stitchWindowAspectRatio;
					zoomFactor = nusiz.y / unzoomedRect.x;
				}
				if (zoomFactor < zoomMin) {

					zoomFactor = tdub;
					zumin();
					return 1;
				}
				zoomRect.left = zoomRect.bottom = 0;
				zoomRect.right = nusiz.x;
				zoomRect.top = nusiz.y;
				shft(sPnt);
				rstMap(BZUMIN);
				setMap(RESTCH);
				if (!setMap(ZUMED))
					movStch();
				return 1;
			}
			//			rstMap(BZUMIN);
		}
	}
	if (msg.message == WM_RBUTTONDOWN || msg.message == WM_LBUTTONDOWN) {

		if (rstMap(THUMON)) {

			if (chkok()) {

				save();
				thumbak();
				unmsg();
				return 1;
			}
			if (chkwnd(hdsc)) {

				thumbak();
				unmsg();
				return 1;
			}
			setMap(BAKSHO);
			unbsho();
			rstMap(THUMSHO);
			unmsg();
			return 1;
		}
		if (rstMap(BAKSHO)) {

			if (msg.message == WM_RBUTTONDOWN)
				setMap(RBUT);
			else
				rstMap(RBUT);
			for (ind = 0; ind < OLDVER; ind++) {

				if (msg.hwnd == hvu[ind]) {

					fileVersionIndex = ind;
					if (chkMap(THUMSHO)) {

						if (savcmp())
							thumbak();
						else {

							if (chkMap(RBUT))
								thumbak();
							else {

								setMap(THUMON);
								savdisc();
							}
							return 1;
						}
					} else
						rebak();
					rstAll();
					setMap(RESTCH);
					return 1;
				}
			}
		}
		if (msg.pt.x >= colorBarRect.left&&msg.pt.x <= colorBarRect.right
			&&msg.pt.y >= colorBarRect.top&&msg.pt.y <= colorBarRect.bottom) {

			unpat();
			tdub = (double)(msg.pt.y - colorBarRect.top) / (colorBarRect.bottom - colorBarRect.top);
			if (msg.message == WM_RBUTTONDOWN) {

				tdub = (double)(msg.pt.y - colorBarRect.top) / (colorBarRect.bottom - colorBarRect.top);
				if (msg.wParam&MK_SHIFT && (chkMap(SELBOX) || chkMap(GRPSEL))) {

					unbox();
					groupStitchIndex = tdub*hed.stitchCount;
					setMap(GRPSEL);
					grpAdj();
					nuAct(groupStitchIndex);
					setMap(RESTCH);
				} else {

					closestPointIndex = tdub*hed.stitchCount;
					nuAct(closestPointIndex);
					movbox();
					if (rstMap(GRPSEL)) {

						rstMap(SCROS);
						rstMap(ECROS);
						setMap(RESTCH);
					}
				}
			} else {

				closestPointIndex = tdub*hed.stitchCount;
				nuAct(closestPointIndex);
				rstAll();
				setMap(SELBOX);
				selCol();
			}
			redraw(hBar);
			return 1;
		}
	}
	if (msg.message == WM_RBUTTONDOWN) {

		if (chkMap(TXTRED) && !hMsg)
		{
			txtrbut();
			return 1;
		}
		if (GetKeyState(VK_SHIFT) & 0X8000) {

			if (closfrm()) {

				if ((chkMap(FRMPSEL) || chkMap(FPSEL)) && selectedFormPoints.form == closestFormToCursor) {

					rstMap(FRMPSEL);
					setMap(FPSEL);
					selectedFormPoints.finish = closestVertexToCursor;
					ine = (selectedFormPoints.finish - selectedFormPoints.start + sides) % sides;
					if (ine < (unsigned)sides >> 1) {

						selectedFormPoints.pointCount = ine;
						setMap(PSELDIR);
					} else {

						selectedFormPoints.pointCount = sides - ine;
						rstMap(PSELDIR);
					}
					setpsel();
					return 1;
				}
				rstMap(FPSEL);
				setMap(FRMPSEL);
				selectedFormPoints.start = closestVertexToCursor;
				selectedFormPoints.form = closestFormToCursor;
				selfpnt();
				return 1;
			}
		}
		if (chkMap(WASTRAC)) {

			if (chkMap(TRNIN0))
				dutrnum0(atoi(trinbuf));
			if (chkMap(TRNIN1))
				dutrnum1();
			if (!chkMap(WASEDG))
				tracpar();
			return 1;
		}
		unmsg();
		unboxs();
		unpat();
		bufferIndex = 0;
		rstMap(SATIN);
		if (rstMap(SATPNT))
			satfix();
		if (chkMap(BAKSHO)) {

			unbsho();
			return 1;
		}
		if (rstMap(BIGBOX))
			setMap(RESTCH);
		if (chkMap(PRFACT)) {

			if (rstMap(HUPMSG))
				unsid();
			else {

				if (preferenceIndex) {

					chknum();
					unsid();
					preferenceIndex = 0;
				} else {

					DestroyWindow(hPrf);
					rstMap(PRFACT);
					if (rstMap(WASRT))
						setMap(INSRT);
				}
			}
			setMap(RESTCH);
			return 1;
		} else {

			if (formMenuChoice) {

				chknum();
				formMenuChoice = 0;
				setMap(RESTCH);
				return 1;
			}
		}
		if (rstMap(INSFRM)) {

			insadj();
			rstMap(SHOINSF);
			setMap(RESTCH);
			return 1;
		}
		if (px2stch() && !(chkMap(SIZSEL) && chkMsgs(msg.pt, hSizChng[0], hSizChng[2]))) {

			if (formIndex && !chkMap(FRMOF)) {

				if (msg.wParam&MK_SHIFT) {

					tmpFormIndex = closestFormToCursor;
					if (closfrm()) {

						ine = 0;
						if (selectedFormCount) {

							nuslst(closestFormToCursor);
							setMap(RESTCH);
							return 1;
						} else {

							if (rstMap(FORMSEL) && tmpFormIndex != closestFormToCursor) {

								if (tmpFormIndex > closestFormToCursor) {

									ine = closestFormToCursor;
									closestFormToCursor = tmpFormIndex;
									tmpFormIndex = ine;
								}
								for (ind = tmpFormIndex; ind <= (long)closestFormToCursor; ind++)
									selectedFormList[ine++] = ind;
								selectedFormCount = ine;
								setMap(RESTCH);
								return 1;
							} else
								nufsel();
						}
					}
				}
				if (msg.wParam&MK_CONTROL) {

					if (!selectedFormCount&&chkMap(FORMSEL)) {

						setMap(WASEL);
						previousFormIndex = closestFormToCursor;
					}
					if (closfrm())
						nufsel();
					if (selectedFormCount > 1)
						butxt(HNUM, "");
					else
						ritnum(STR_NUMFORM, selectedFormList[0] + 1);
					return 1;
				}
				if (chkMap(FORMSEL)) {

					if (formMenuChoice) {

						chknum();
						formMenuChoice = 0;
						goto frmskip;
					}
					if (hfdat) {

						undat();
						unsid();
						formMenuChoice = 0;
						setMap(RESTCH);
						goto frmskip;
					}
					ritfrct(closestFormToCursor, StitchWindowDC);
					lenCalc();
					if (!rstMap(ENTROT))
						rstMap(FORMSEL);
					setMap(DUMEN);
					if (!closPnt1(&closestPointIndex))
						unbox();
				frmskip:;
				}
				if (closfrm()) {

					setMap(FORMSEL);
					rstMap(FPSEL);
					unpsel();
					fvars(closestFormToCursor);
					ritfrct(closestFormToCursor, StitchWindowDC);
					if (rstMap(FRMPSEL) || selectedFormCount) {

						setMap(RESTCH);
						selectedFormCount = 0;
					}
					ritnum(STR_NUMFORM, closestFormToCursor);
					cod = (formList[closestFormToCursor].attribute&FRMLMSK) >> 1;
					lenCalc();
					unrot();
					return 1;
				}
				if (selectedFormCount) {

					selectedFormCount = 0;
					setMap(RESTCH);
				}
				if (rstMap(FRMPSEL))
					setMap(RESTCH);
			}
			if (chkMap(INIT) || hFil) {

				if (msg.wParam&MK_SHIFT) {

					if (chkMap(SELBOX)) {

						cod = closestPointIndex;
						closPnt1(&closestPointIndex);
						if (closestPointIndex != cod) {

							unbox();
							groupStitchIndex = closestPointIndex;
							closestPointIndex = cod;
							setMap(GRPSEL);
							grpAdj();
						}
						nuAct(groupStitchIndex);
						redraw(hBar);
						return 1;
					}
					if (chkMap(GRPSEL)) {

						cod = closestPointIndex;
						closPnt1(&closestPointIndex);
						groupStitchIndex = closestPointIndex;
						closestPointIndex = cod;
						grpAdj();
						nuAct(groupStitchIndex);
						redraw(hBar);
						return 1;
					}
					rebox();
				} else {

					rstMap(LENSRCH);
					rstMap(WASGRP);
					rstMap(FPSEL);
					rstMap(WASFRMFRM);
					rstMap(SIDACT);
					if (rstMap(INSRT)) {

						ReleaseCapture();
						rstAll();
						setMap(RESTCH);
					} else {

						if (rstMap(GRPSEL))
							setMap(RESTCH);
						if (hed.stitchCount)
							rebox();
					}
				}
				clrstch();
				setMap(RESTCH);
			}
			return 1;
		}
		if (msg.pt.x >= minLenRct.left&&msg.pt.x <= minLenRct.right
			&&msg.pt.y > minLenRct.top&&msg.pt.y <= minLenRct.bottom) {

			srchk();
			setsrch(smallestStitchIndex);
			lensadj();
			setMap(GRPSEL);
			setMap(RESTCH);
			return 1;
		}
		if (msg.pt.x >= maxLenRct.left&&msg.pt.x <= maxLenRct.right
			&&msg.pt.y > maxLenRct.top&&msg.pt.y <= maxLenRct.bottom) {

			srchk();
			setsrch(largestStitchIndex);
			lensadj();
			setMap(GRPSEL);
			setMap(RESTCH);
		}
		return 1;
	}
	if (msg.message == WM_LBUTTONDOWN) {

		if (GetKeyState(VK_SHIFT) & 0X8000 && px2stch())
		{
			dushft();
			return 1;
		}
		if (chkMap(TXTRED) && !chkMap(FORMIN))
		{
			txtlbut();
			return 1;
		}
		if (rstMap(FSETFCOL))
		{
			unsid();
			unmsg();
			if (chkMsgs(msg.pt, hDef[0], hDef[15]))
			{
				dufcol(vind + 1);
				return 1;
			}
		}
		if (rstMap(FSETBCOL))
		{
			unsid();
			unmsg();
			if (chkMsgs(msg.pt, hDef[0], hDef[15]))
			{
				dubcol(vind + 1);
				return 1;
			}
		}
		if (rstMap(FPUNCLP)) {

			savdo();
			fixpclp();
			frmout(closestFormToCursor);
			return 1;
		}
		if (chkMap(FPSEL) && !chkMap(FUNCLP) && !chkMap(ROTAT)) {

			MoveMemory(&selectedFormsLine, &selectedPointsRectangle, sizeof(POINT) * 9);
			MoveMemory(&selectedFormsRectangle, &selectedPixelsRect, sizeof(RECT));
			if (chkbig())
				return 1;
		}
		if (chkMap(WASTRAC)) {

			tracpar();
			return 1;
		}
		if (rstMap(HUPMSG)) {

			for (ind = 0; ind < HUPS; ind++) {

				if (msg.hwnd == hsidWnd[ind]) {

					switch (ind + 1) {

					case SETCUST:

						ini.customHoopX = ini.hoopSizeX;
						ini.customHoopY = ini.hoopSizeY;
						setMap(MSGOF);
						sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_CUSTHUP], ini.hoopSizeX / PFGRAN, ini.hoopSizeY / PFGRAN);
						shoMsg(msgbuf);
						break;

					case SMALHUP:

						ini.hoopSizeX = SHUPX;
						ini.hoopSizeY = SHUPY;
						ini.hoopType = SMALHUP;
						break;

					case LARGHUP:

						ini.hoopSizeX = LHUPX;
						ini.hoopSizeY = LHUPY;
						ini.hoopType = LARGHUP;
						break;

					case CUSTHUP:

						ini.hoopSizeX = ini.customHoopX;
						ini.hoopSizeY = ini.customHoopY;
						ini.hoopType = CUSTHUP;
						break;

					case HUP100:

						ini.hoopSizeX = HUP100XY;
						ini.hoopSizeY = HUP100XY;
						ini.hoopType = CUSTHUP;
						break;
					}
					unzoomedRect.x = ini.hoopSizeX;
					unzoomedRect.y = ini.hoopSizeY;
					sethup();
					chkhup();
					break;
				}
			}
			unsid();
			prfmsg();
			return 1;
		}
		if (rstMap(INSFIL)) {

			duinsfil();
			return 1;
		}
		if (chkMap(BIGBOX) && chkbig())
			return 1;
		if (rstMap(DELSFRMS)) {

			cod = 0;
			if (chkok()) {

				savdo();
				rstMap(DELTO);
				cod = 1;
			} else {

				GetWindowRect(hto, &trct);
				if (msg.pt.x >= trct.left&&msg.pt.x <= trct.right
					&&msg.pt.y >= trct.top&&msg.pt.y <= trct.bottom) {

					savdo();
					setMap(DELTO);
					cod = 1;
				}
			}
			delsfrms(cod);
			unmsg();
			return 1;
		}
		if (rstMap(MOVMSG)) {

			if (chkok()) {

				savdo();
				refilfn();
			}
			unmsg();
			if (rstMap(WASFRMFRM))
				refrm();
			return 1;
		}
		if (rstMap(FUNSCLP)) {

			savdo();
			rstMap(MOVFRMS);
			pxrct2stch(selectedFormsRectangle, &tbig);
			px2stch();
			fmovdif.x = sPnt.x - ((tbig.right - tbig.left) / 2 + tbig.left);
			fmovdif.y = sPnt.y - ((tbig.top - tbig.bottom) / 2 + tbig.bottom);
			for (ind = 0; ind < clipboardFormsCount; ind++) {

				closestFormToCursor = formIndex + ind;
				fvars(closestFormToCursor);
				for (ine = 0; ine < ptrSelectedForm->sides; ine++) {

					ptrSelectedForm->vertices[ine].x += fmovdif.x;
					ptrSelectedForm->vertices[ine].y += fmovdif.y;
				}
				frmout(closestFormToCursor);
				refil();
			}
			formIndex += clipboardFormsCount;
			setMap(RESTCH);
			return 1;
		}
		if (selectedFormCount && !chkMap(ROTAT) && chkbig())
			return 1;
		if (chkMap(SIDCOL) && chkMsgs(msg.pt, hDef[0], hDef[15])) {

			savdo();
			if (rstMap(FSETFCOL))
			{
				dufcol(vind + 1);
				return 1;
			}
			if (rstMap(BRDSID))
			{
				nubrdcol(vind);
				goto chkcolx;
			}
			if (rstMap(APSID))
			{
				nulapcol(vind);
				goto chkcolx;
			}
			if (rstMap(FTHSID))
			{
				nufthcol(vind);
				goto chkcolx;
			}
			if (rstMap(UNDCOL))
			{
				ptrSelectedForm->underlayColor = vind & 0xf;
				refilfn();
				coltab();
				goto chkcolx;
			}
			if (rstMap(FSETUCOL))
			{
				dundcol(vind + 1);
				return 1;
			}
			nufilcol(vind);
		chkcolx:;
			msgbuf[0] = (TCHAR)vind + 0x30;
			msgbuf[1] = 0;
			SetWindowText(thDat[LBRDCOL], msgbuf);
			unsid();
			coltab();
			setMap(RESTCH);
			rstMap(SIDCOL);
			formMenuChoice = 0;
			return 1;
		}
		if (rstMap(OSAV)) {

			if (chkok()) {

				save();
				nuFil();
				unmsg();
				return 1;
			}
			if (chkwnd(hdsc)) {

				nuFil();
				unmsg();
				return 1;
			}
			unmsg();
			return 1;
		}
		if (rstMap(FCLOS)) {

			if (chkok())
				save();
			else {

				if (!chkwnd(hcan))
					closfn();
			}
			unmsg();
			return 1;
		}
		if (rstMap(SAVEX)) {

			if (chkok()) {

				save();
				reldun();
			}
			if (chkwnd(hdsc))
				reldun();
			unmsg();
			return 1;
		}
		if (chkMap(PRFACT)) {

			chknum();
			if (msg.hwnd == thDat[PSQR]) {

				if (toglu(SQRFIL))
					SetWindowText(thDat[PSQR], stab[STR_PNTD]);
				else
					SetWindowText(thDat[PSQR], stab[STR_SQR]);
				return 1;
			}
			if (msg.hwnd == thDat[PBLNT]) {

				if (toglu(BLUNT))
					SetWindowText(thDat[PBLNT], stab[STR_TAPR]);
				else
					SetWindowText(thDat[PBLNT], stab[STR_BLUNT]);
				return 1;
			}
			if (msg.hwnd == thDat[PUND]) {

				if (toglu(DUND))
					SetWindowText(thDat[PUND], stab[STR_OFF]);
				else
					SetWindowText(thDat[PUND], stab[STR_ON]);
				return 1;
			}
			if (msg.hwnd == thDat[PHUP]) {

				sidhup();
				return 1;
			}
			for (ind = 0; ind < PRFLINS; ind++) {

				if (msg.hwnd == thDat[ind]) {

					preferenceIndex = ind + 1;
					prfsid(thDat[ind]);
					break;
				}
			}
			return 1;
		}
		if (!chkMap(ROTAT) && chkMap(GRPSEL)) {

			if (iselpnt()) {

				for (ind = 0; ind < 4; ind++) {

					sizlin[ind].x = formOutlineRectangle[ind << 1].x;
					sizlin[ind].y = formOutlineRectangle[ind << 1].y;
				}
				sizlin[4].x = sizlin[0].x;
				sizlin[4].y = sizlin[0].y;
				if (selectedFormControlVertex & 1)
					setMap(STRTCH);
				else {

					setMap(EXPAND);
					xyRatio = (double)(rngrct.right - rngrct.left) / (rngrct.top - rngrct.bottom);
				}
				selectedFormControlVertex >>= 1;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			} else {

				stitchSizePixels.x = msg.pt.x - stitchWindowOrigin.x;
				stitchSizePixels.y = msg.pt.y - stitchWindowOrigin.y;
				if (stitchSizePixels.x >= formOutlineRectangle[0].x&&stitchSizePixels.x <= formOutlineRectangle[2].x&&
					stitchSizePixels.y >= formOutlineRectangle[0].y&&stitchSizePixels.y <= formOutlineRectangle[4].y) {

					duSelbox();
					setMap(SELPNT);
					SetCapture(hWnd);
					setMap(VCAPT);
					rSelbox();
					return 1;
				}
			}
		}
		if (rstMap(NEWBAK)) {

			if (chkok()) {

				unmsg();
				save();
				newFil();
				return 1;
			}
			if (chkwnd(hdsc)) {

				unmsg();
				newFil();
				return 1;
			}
			unmsg();
			return 1;
		}
		if (rstMap(PRGMSG)) {

			if (chkok()) {

				deldir();
				return 1;
			}
		}
		if (rstMap(DUBAK)) {

			if (chkMap(THUMSHO)) {

				if (chkok())
					thrsav();
				getbak();
				rstMap(THUMSHO);
				return 1;
			} else {

				if (chkok()) {

					ind = duth(thrnam);
					thrnam[ind] = 't';
					setMap(IGNAM);
					thrsav();
					thrnam[ind] = 'r';
					if (fileVersionIndex)
						filnam[ind] = fileVersionIndex + 0x2f;
					setMap(REDOLD);
					nuFil();
					filnam[ind] = 'r';
					switch (fileVersionIndex) {

					case 3:

						movbak('1', '2');

					case 2:

						movbak('0', '1');

					case 1:

						movbak('r', '0');
					}
					movbak('t', 'r');
					thrnam[ind] = 't';
					DeleteFile(thrnam);
					thrnam[ind] = 'r';
					hFil = CreateFile(thrnam, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
						OPEN_EXISTING, 0, NULL);
					if (hFil == INVALID_HANDLE_VALUE)
						hFil = 0;
					return 1;
				}
				GetWindowRect(hcan, &trct);
				if (msg.pt.x >= trct.left&&msg.pt.x <= trct.right
					&&msg.pt.y >= trct.top&&msg.pt.y <= trct.bottom) {

					getbak();
					return 1;
				}
			}
		}
		if (chkMap(DELFRM)) {

			cod = 0;
			if (chkok()) {

				savdo();
				rstMap(DELTO);
				cod = 1;
			} else {

				GetWindowRect(hto, &trct);
				if (msg.pt.x >= trct.left&&msg.pt.x <= trct.right
					&&msg.pt.y >= trct.top&&msg.pt.y <= trct.bottom) {

					savdo();
					setMap(DELTO);
					cod = 1;
				}
			}
			if (cod) {

				frmdel();
				coltab();
				setMap(RESTCH);
			}
			unmsg();
			return 1;
		}
		if (chkMap(FILMSG)) {

			if (chkok()) {

				savdo();
				unfil();
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (rstMap(SIZSEL)) {

			if (chkMsgs(msg.pt, hSizChng[0], hSizChng[2])) {

				vind -= 13;
				thrdSiz[thrdSel][0] = cmap[vind];
				sizInd[thrdSel] = vind;
				strncpy_s(buf, thrdSiz[thrdSel], 2);
				buf[2] = 0;
				SetWindowText(hSiz[thrdSel], buf);
				setMap(RESTCH);
				for (ind = 0; ind < 3; ind++)
					DestroyWindow(hSizChng[ind]);
				return 1;
			} else {

				for (ind = 0; ind < 3; ind++)
					DestroyWindow(hSizChng[ind]);
			}
		}
		if (rstMap(POLIMOV)) {

			savdo();
			setfrm();
			return 1;
		}
		if (rstMap(FORMIN)) {

			GetWindowRect(hMsg, &trct);
			if (msg.pt.x >= trct.left&&msg.pt.x <= trct.right
				&&msg.pt.y >= trct.top&&msg.pt.y <= trct.bottom) {

				ind = (msg.pt.y - trct.top - 1) / (buttonHi - 4);
				if (rstMap(FENDIN)) {

					if (ind == 3)
						rstu(SQRFIL);
					if (ind == 4)
						setu(SQRFIL);
				} else
					duform(ind);
				switch (ind) {

				case RPOLI - 1:
				case STAR - 1:
				case SPIR - 1:
				case HART - 2:
				case LENS - 2:
				case EGG - 2:
				case ZIG - 2:

					return 1;
				}
			}
			unmsg();
			return 1;
		}
		if (preferenceIndex == PAP + 1 && chkMsgs(msg.pt, hDef[0], hDef[15])) {

			sprintf_s(msgbuf, sizeof(msgbuf), "%d", vind);
			underlayColor = vind;
			SetWindowText(thDat[PAP], msgbuf);
			unsid();
			return 1;
		}
		if (rstMap(SIDACT)) {

			savdo();
			if (formMenuChoice == LFTHTYP) {

				for (ind = 0; ind < 6; ind++) {

					if (msg.hwnd == hsidWnd[ind]) {

						ptrSelectedForm->fillInfo.feather.fillType = ind + 1;
						refil();
						refrm();
						break;
					}
				}
				return 1;
			}
			if (formMenuChoice == LLAYR) {

				for (ind = 0; ind < 5; ind++) {

					if (msg.hwnd == hsidWnd[ind])
						break;
				}
				if (ind < 5) {

					movlayr(ind << 1);
					setMap(FORMSEL);
				}
				return 1;
			}
			ptrSelectedForm->borderColor &= 0xf;
			if (rstMap(BRDACT)) {

				if (iseclp(closestFormToCursor))
					deleclp(closestFormToCursor);
				if (msg.hwnd == hsidWnd[0]) {

					ptrSelectedForm->edgeType = 0;
					refilfn();
					coltab();
					unsid();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[1]) {

					if (ptrSelectedForm->edgeType) {

						cod = ptrSelectedForm->edgeType&NEGUND;
						if (cod == EGCLP || cod == EGSAT || cod == EGAP)
							bsizpar();
						ptrSelectedForm->edgeType = EGLIN;
						goto didfil;
					} else {

						unmsg();
						unsid();
						bord();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[2]) {

					if (ptrSelectedForm->edgeType) {

						cod = ptrSelectedForm->edgeType&NEGUND;
						if (cod == EGCLP || cod == EGSAT || cod == EGAP)
							bsizpar();
						ptrSelectedForm->edgeType = EGBLD;
						goto didfil;
					} else {

						unmsg();
						unsid();
						dubold();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[3]) {

					unmsg();
					unsid();
					fclp();
					return 1;
				}
				if (msg.hwnd == hsidWnd[4]) {

					if (ptrSelectedForm->edgeType) {

						switch (ptrSelectedForm->edgeType) {

						case EGCLP:

							bsizpar();

						case EGLIN:
						case EGBLD:

							ptrSelectedForm->borderSize = borderWidth;
							ptrSelectedForm->edgeSpacing = StitchSpace;
							break;

						case EGPRP:

							ptrSelectedForm->edgeSpacing /= 2;
							break;
						}
						ptrSelectedForm->edgeType = EGSAT;
						if (chku(DUND))
							ptrSelectedForm->edgeType |= EGUND;
						goto didfil;
					} else {

						unmsg();
						unsid();
						satbrd();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[5]) {

					if (ptrSelectedForm->fillType) {

						delmfil(ptrSelectedForm->fillColor);
						ptrSelectedForm->fillType = 0;
					}
					if (ptrSelectedForm->edgeType) {

						if (ptrSelectedForm->edgeType == EGLIN || ptrSelectedForm->edgeType == EGBLD || ptrSelectedForm->edgeType == EGCLP) {

							ptrSelectedForm->borderSize = borderWidth;
							ptrSelectedForm->edgeSpacing = StitchSpace;
							if (ptrSelectedForm->edgeType == EGCLP)
								bsizpar();
						}
						ptrSelectedForm->edgeType = EGAP;
						if (chku(DUND))
							ptrSelectedForm->edgeType |= EGUND;
						ptrSelectedForm->borderColor |= (underlayColor << 4);
						goto didfil;
					} else {

						unmsg();
						unsid();
						apliq();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[6]) {

					if (ptrSelectedForm->edgeType) {

						switch (ptrSelectedForm->edgeType) {

						case EGCLP:

							bsizpar();
						case EGLIN:
						case EGBLD:

							ptrSelectedForm->borderSize = borderWidth;
							ptrSelectedForm->edgeSpacing = StitchSpace;
							break;

						case EGSAT:

							ptrSelectedForm->edgeSpacing *= 2;
						}
						ptrSelectedForm->edgeType = EGPRP;
						if (chku(DUND))
							ptrSelectedForm->edgeType |= EGUND;
						goto didfil;
					} else {

						unmsg();
						unsid();
						prpbrd(StitchSpace);
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[7]) {

					if (ptrSelectedForm->edgeType) {

						if (ptrSelectedForm->edgeType == EGLIN || ptrSelectedForm->edgeType == EGBLD || ptrSelectedForm->edgeType == EGCLP) {

							ptrSelectedForm->borderSize = borderWidth;
							ptrSelectedForm->edgeSpacing = StitchSpace;
							if (ptrSelectedForm->edgeType == EGCLP)
								bsizpar();
						}
						ptrSelectedForm->edgeType = EGHOL;
						goto didfil;
					} else {

						unmsg();
						unsid();
						bhol();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[8]) {

					if (ptrSelectedForm->edgeType) {

						if (ptrSelectedForm->edgeType == EGLIN || ptrSelectedForm->edgeType == EGBLD || ptrSelectedForm->edgeType == EGCLP) {

							ptrSelectedForm->borderSize = borderWidth;
							ptrSelectedForm->edgeSpacing = StitchSpace;
							if (ptrSelectedForm->edgeType == EGCLP)
								bsizpar();
						}
						picot();
						goto didfil;
					} else {

						unmsg();
						unsid();
						picot();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[9]) {

					if (ptrSelectedForm->edgeType) {

						cod = ptrSelectedForm->edgeType&NEGUND;
						if (cod == EGCLP || cod == EGSAT || cod == EGAP)
							bsizpar();
						ptrSelectedForm->edgeType = EGDUB;
						goto didfil;
					} else {

						unmsg();
						unsid();
						dubsfil();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[10]) {

					setMap(LINCHN);
					unmsg();
					unsid();
					chan();
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[11]) {

					rstMap(LINCHN);
					unmsg();
					unsid();
					chan();
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[12]) {

					unmsg();
					unsid();
					filclpx();
					return 1;
				}
			} else {

				if (ptrSelectedForm->fillType == SAT&&ptrSelectedForm->satinGuideCount)
					delsac(closestFormToCursor);
				if ((ptrSelectedForm->edgeType&NEGUND) == EGAP) {

					ptrSelectedForm->edgeType = EGSAT;
					if (chku(DUND))
						ptrSelectedForm->edgeType |= EGUND;
				}
				if (msg.hwnd == hsidWnd[0]) {

					ptrSelectedForm->type = POLI;
					delmfil(ptrSelectedForm->fillColor);
					ptrSelectedForm->fillType = 0;
					coltab();
					unsid();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[1]) {

					savdo();
					ptrSelectedForm->type = POLI;
					if (ptrSelectedForm->fillType) {

						respac();
						ptrSelectedForm->fillType = VRTF;
						ptrSelectedForm->type = POLI;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filvrt();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[2]) {

					ptrSelectedForm->type = POLI;
					if (ptrSelectedForm->fillType) {

						respac();
						ptrSelectedForm->fillType = HORF;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filhor();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[3]) {

					ptrSelectedForm->type = POLI;
					if (ptrSelectedForm->fillType) {

						respac();
						ptrSelectedForm->fillType = ANGF;
						ptrSelectedForm->angleOrClipData.angle = ini.fillAngle;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filangl();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[4]) {

					ptrSelectedForm->type = SAT;
					if (ptrSelectedForm->fillType) {

						respac();
						ptrSelectedForm->fillType = SATF;
						goto didfil;
					} else {

						unmsg();
						unsid();
						rstMap(FTHR);
						filsat();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[5]) {

					ptrSelectedForm->type = SAT;
					unmsg();
					unsid();
					clpfil();
					return 1;
				}
				if (msg.hwnd == hsidWnd[6]) {

					if (ptrSelectedForm->fillType) {

						if (ptrSelectedForm->fillType == CLPF)
							ptrSelectedForm->fillSpacing = StitchSpace;
						chkcont();
						goto didfil;
					} else {

						unmsg();
						unsid();
						contfil();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[7]) {

					unmsg();
					unsid();
					if (sidclp())
						vrtsclp();
					rstMap(CLPSHO);
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[8]) {

					unmsg();
					unsid();
					if (sidclp())
						horsclp();
					rstMap(CLPSHO);
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[9]) {

					unmsg();
					unsid();
					if (sidclp())
						angsclp();
					rstMap(CLPSHO);
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[10]) {

					unmsg();
					unsid();
					if (ptrSelectedForm->fillType == FTHF) {

						fthrfn();
						fritfil();
					} else
						fethrf();
					setMap(INIT);
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[11])	//vertical texture
				{
					if (istx(closestFormToCursor))
					{
						ptrSelectedForm->fillType = TXVRTF;
						goto didfil;
					}
					dutxtfil();
				}
				if (msg.hwnd == hsidWnd[12])	//horizontal texture
				{
					if (istx(closestFormToCursor))
					{
						ptrSelectedForm->fillType = TXHORF;
						goto didfil;
					}
					dutxtfil();
				}
				if (msg.hwnd == hsidWnd[13])	//angle texture
				{
					if (istx(closestFormToCursor))
					{
						ptrSelectedForm->fillType = TXANGF;
						ptrSelectedForm->angleOrClipData.angle = ini.fillAngle;
						goto didfil;
					}
					dutxtfil();
				}
			didfil:;
				refrm();
				refil();
				unsid();
				setMap(RESTCH);
				return 1;
			}
		}
		if (hfdat) {

			chknum();
			unsid();
			if (msg.hwnd == thDat[LTXOF])
			{
				formMenuChoice = LTXOF;
				sidwnd(thDat[LTXOF]);
				goto didat;
			}
			if (msg.hwnd == thDat[LUSPAC])
			{
				formMenuChoice = LUSPAC;
				sidwnd(thDat[LUSPAC]);
				goto didat;
			}
			if (msg.hwnd == thDat[LUANG])
			{
				formMenuChoice = LUANG;
				sidwnd(thDat[LUANG]);
				goto didat;
			}
			if (msg.hwnd == thDat[LULEN])
			{
				formMenuChoice = LULEN;
				sidwnd(thDat[LULEN]);
				goto didat;
			}
			if (msg.hwnd == thDat[LWLKIND])
			{
				formMenuChoice = LWLKIND;
				sidwnd(thDat[LWLKIND]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFTHSIZ]) {

				formMenuChoice = LFTHSIZ;
				sidwnd(thDat[LFTHSIZ]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFTHNUM]) {

				formMenuChoice = LFTHNUM;
				sidwnd(thDat[LFTHNUM]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFTHFLR]) {

				formMenuChoice = LFTHFLR;
				sidwnd(thDat[LFTHFLR]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFTHUPCNT]) {

				formMenuChoice = LFTHUPCNT;
				sidwnd(thDat[LFTHUPCNT]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFTHDWNCNT]) {

				formMenuChoice = LFTHDWNCNT;
				sidwnd(thDat[LFTHDWNCNT]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFTHBLND]) {

				setMap(FLPBLND);
				goto didfil;
			}
			if (msg.hwnd == thDat[LFTHUP]) {

				ptrSelectedForm->extendedAttribute ^= AT_FTHUP;
				goto didfil;
			}
			if (msg.hwnd == thDat[LFTHDWN]) {

				ptrSelectedForm->extendedAttribute ^= AT_FTHDWN;
				goto didfil;
			}
			if (msg.hwnd == thDat[LFTHTYP]) {

				formMenuChoice = LFTHTYP;
				sidmsg(thDat[LFTHTYP], &stab[STR_FTH0], 6);
				goto didat;
			}
			if (msg.hwnd == thDat[LFRM]) {

				savdo();
				unfil();
				if (ptrSelectedForm->type == LIN)
					ptrSelectedForm->type = POLI;
				else
					ptrSelectedForm->type = LIN;
				coltab();
				delsac(closestFormToCursor);
				setMap(RESTCH);
				goto didat;
			}
			if (msg.hwnd == thDat[LLAYR]) {

				formMenuChoice = LLAYR;
				rstMap(FILTYP);
				sidmsg(thDat[LLAYR], laytxt, 5);
				goto didat;
			}
			if (msg.hwnd == thDat[LFRMFIL]) {

				rstMap(FILTYP);
				formMenuChoice = LFRMFIL;
				sidmsg(thDat[LFRMFIL], &stab[STR_FIL0], 14);
				goto didat;
			}
			if (msg.hwnd == thDat[LFRMCOL]) {

				formMenuChoice = LFRMCOL;
				sidwnd(thDat[LFRMCOL]);
				setMap(SIDCOL);
				goto didat;
			}
			if (msg.hwnd == thDat[LUNDCOL]) {

				formMenuChoice = LUNDCOL;
				sidwnd(thDat[LUNDCOL]);
				setMap(SIDCOL);
				setMap(UNDCOL);
				goto didat;
			}
			if (msg.hwnd == thDat[LFTHCOL]) {

				formMenuChoice = LFTHCOL;
				sidwnd(thDat[LFTHCOL]);
				setMap(SIDCOL);
				setMap(FTHSID);
				goto didat;
			}
			if (msg.hwnd == thDat[LFRMSPAC]) {

				formMenuChoice = LFRMSPAC;
				sidwnd(thDat[LFRMSPAC]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFRMLEN]) {

				formMenuChoice = LFRMLEN;
				sidwnd(thDat[LFRMLEN]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRD]) {

				setMap(FILTYP);
				sidmsg(thDat[LBRD], &stab[STR_EDG0], EDGETYPS + 1);
				setMap(BRDACT);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRDCOL]) {

				formMenuChoice = LBRDCOL;
				sidwnd(thDat[LBRDCOL]);
				setMap(SIDCOL);
				setMap(BRDSID);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRDSPAC]) {

				formMenuChoice = LBRDSPAC;
				sidwnd(thDat[LBRDSPAC]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRDLEN]) {

				formMenuChoice = LBRDLEN;
				sidwnd(thDat[LBRDLEN]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRDSIZ]) {

				formMenuChoice = LBRDSIZ;
				sidwnd(thDat[LBRDSIZ]);
				goto didat;
			}
			if (msg.hwnd == thDat[LAPCOL]) {

				formMenuChoice = LAPCOL;
				setMap(SIDCOL);
				setMap(APSID);
				sidwnd(thDat[LAPCOL]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBCSIZ]) {

				formMenuChoice = LBCSIZ;
				sidwnd(thDat[LBCSIZ]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBSTRT]) {

				cod = ptrSelectedForm->attribute&SBLNT;
				if (cod)
					ptrSelectedForm->attribute &= NSBLNT;
				else
					ptrSelectedForm->attribute |= SBLNT;
				refil();
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (msg.hwnd == thDat[LBFIN]) {

				cod = ptrSelectedForm->attribute&FBLNT;
				if (cod)
					ptrSelectedForm->attribute &= NFBLNT;
				else
					ptrSelectedForm->attribute |= FBLNT;
				refil();
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (msg.hwnd == thDat[LFRMANG]) {

				formMenuChoice = LFRMANG;
				sidwnd(thDat[LFRMANG]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRDPIC]) {

				formMenuChoice = LBRDPIC;
				sidwnd(thDat[LBRDPIC]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRDUND]) {

				ptrSelectedForm->edgeType ^= EGUND;
				refil();
				goto didat;
			}
			if (msg.hwnd == thDat[LSACANG]) {

				formMenuChoice = LSACANG;
				sidwnd(thDat[LSACANG]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFRMFAZ]) {

				formMenuChoice = LFRMFAZ;
				sidwnd(thDat[LFRMFAZ]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBRDPOS]) {

				formMenuChoice = LBRDPOS;
				sidwnd(thDat[LBRDPOS]);
				goto didat;
			}
			if (msg.hwnd == thDat[LBFILSQR]) {

				dubit(AT_SQR);
				return 1;
			}
			if (msg.hwnd == thDat[LFSTRT]) {

				dubit(AT_STRT);
				return 1;
			}
			if (msg.hwnd == thDat[LDSTRT])
			{
				formMenuChoice = LDSTRT;
				sidwnd(thDat[LDSTRT]);
				goto didat;
			}
			if (msg.hwnd == thDat[LFEND]) {

				dubit(AT_END);
				return 1;
			}
			if (msg.hwnd == thDat[LDEND])
			{
				formMenuChoice = LDEND;
				sidwnd(thDat[LDEND]);
				goto didat;
			}
			if (msg.hwnd == thDat[LWALK])
			{
				dubit(AT_WALK);
				return 1;
			}
			if (msg.hwnd == thDat[LCWLK])
			{
				dubit(AT_CWLK);
				return 1;
			}
			if (msg.hwnd == thDat[LUND])
			{
				dubit(AT_UND);
				return 1;
			}
			if (msg.hwnd == thDat[LMAXFIL]) {

				formMenuChoice = LMAXFIL;
				sidwnd(thDat[LMAXFIL]);
				goto didat;
			}
			if (msg.hwnd == thDat[LMINFIL]) {

				formMenuChoice = LMINFIL;
				sidwnd(thDat[LMINFIL]);
				goto didat;
			}
			if (msg.hwnd == thDat[LMAXBRD]) {

				formMenuChoice = LMAXBRD;
				sidwnd(thDat[LMAXBRD]);
				goto didat;
			}
			if (msg.hwnd == thDat[LMINBRD]) {

				formMenuChoice = LMINBRD;
				sidwnd(thDat[LMINBRD]);
				goto didat;
			}
			undat();
			goto didskip;
		didat:;
			return 1;
		didskip:;
		}
		if (chkMap(INSFRM)) {

			savdo();
			setins();
			return 1;
		}
		if (chkMap(FUNCLP)) {

			savdo();
			formIndex++;
			setMap(INIT);
			rstfrm();
			fvars(closestFormToCursor);
			refil();
			rstMap(FUNCLP);
			if (rstMap(FPSEL))
				frmout(formIndex - 1);
			setMap(RESTCH);
			return 1;
		}
		if (chkMap(SATCNKT)) {

			savdo();
			satknkt();
			return 1;
		}
		if (chkMap(SATPNT)) {

			satpnt1();
			return 1;
		}
		if (rstMap(SATIN)) {

			satpnt0();
			return 1;
		}
		if (chkMap(FORMSEL)) {

			if (!chkMap(FRMROT) && chkfrm())
				return 1;
		}
		if (rstMap(MOVFRM)) {

			savdo();
			setfrm();
			return 1;
		}
		unmsg();
		if (px2stch() && !(chkMap(SIZSEL) && chkMsgs(msg.pt, hSizChng[0], hSizChng[2]))) {

			unpat();
			if (rstMap(ROTAT)) {

				rlinu[1].x = msg.pt.x - stitchWindowOrigin.x;
				rlinu[1].y = msg.pt.y - stitchWindowOrigin.y;
				padj.x = rlinu[0].x - rlinu[1].x;
				padj.y = rlinu[0].y - rlinu[1].y;
				if (hypot(padj.x, padj.y) < CLOSENUF) {

					px2stch();
					rotcntr.x = sPnt.x;
					rotcntr.y = sPnt.y;
					setMap(MOVCNTR);
					unrot();
					ritrot();
				} else {

					if (padj.x)
						rotationHandleAngle = -atan2(padj.y, padj.x);
					else {

						if (padj.y > 0)
							rotationHandleAngle = PI / 2;
						else
							rotationHandleAngle = -PI / 2;
					}
					setMap(ROTUSHO);
					durotu();
					setMap(ROTCAPT);
				}
				return 1;
			}
			if (rstMap(CLPSHO)) {

				savdo();
				if ((rstMap(SELBOX) || rstMap(INSRT)) && closestPointIndex != (unsigned)hed.stitchCount - 1)
					lodclp(closestPointIndex);
				else
					lodclp(hed.stitchCount);
				rngadj();
				selRct(&tbig);
				formOutlineRectangle[0].x = formOutlineRectangle[6].x = formOutlineRectangle[7].x = formOutlineRectangle[8].x = trct.left;
				formOutlineRectangle[1].x = formOutlineRectangle[5].x = midl(trct.right, trct.left);
				formOutlineRectangle[0].y = formOutlineRectangle[1].y = formOutlineRectangle[2].y = formOutlineRectangle[8].y = trct.top;
				formOutlineRectangle[3].y = formOutlineRectangle[7].y = midl(trct.top, trct.bottom);
				formOutlineRectangle[4].y = formOutlineRectangle[5].y = formOutlineRectangle[6].y = trct.bottom;
				formOutlineRectangle[2].x = formOutlineRectangle[3].x = formOutlineRectangle[4].x = trct.right;
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (rstMap(BOXZUM)) {

				setMap(BZUMIN);
				blin[0].x = blin[3].x = blin[4].x = msg.pt.x - stitchWindowOrigin.x;
				blin[0].y = blin[1].y = msg.pt.y - stitchWindowOrigin.y;
				blin[4].y = blin[0].y - 1;
				px2stch();
				zBoxo.x = sPnt.x;
				zBoxo.y = sPnt.y;
				setMap(VCAPT);
				return 1;
			}
			if (formIndex && !chkMap(FRMOF)) {

				if (!chkMap(INSRT)) {

					if (rstMap(FORMSEL))
						ritfrct(closestFormToCursor, StitchWindowDC);
					if (closfrm()) {

						setMap(FRMPMOV);
						fvars(closestFormToCursor);
						frmovlin();
						return 1;
					}
				}
			}
			if (chkMap(INIT)) {

				unlin();
				savdo();
				if (chkMap(INSRT) && hed.stitchCount < MAXSEQ) {

					px2stch();
					cod = (actcol | USMSK | (activeLayer << LAYSHFT) | NOTFRM)&NKNOTMSK;
					if (chkMap(LIN1)) {

						if (chkMap(BAKEND)) {

							xlin1();
							ind = hed.stitchCount;
							stitchBuffer[ind].x = sPnt.x;
							stitchBuffer[ind].y = sPnt.y;
							stitchBuffer[ind].attribute = cod;
							duzrat();
							stch2px1(ind);
							inlin[0].x = stitchSizePixels.x;
							inlin[0].y = stitchSizePixels.y;
							inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
							inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
							hed.stitchCount++;
							coltab();
							setMap(RESTCH);
							return 1;
						} else {

							xlin1();
							for (ind = hed.stitchCount; ind; ind--) {

								stitchBuffer[ind].attribute = stitchBuffer[ind - 1].attribute;
								stitchBuffer[ind].x = stitchBuffer[ind - 1].x;
								stitchBuffer[ind].y = stitchBuffer[ind - 1].y;
							}
							stitchBuffer[0].attribute = cod;
							stitchBuffer[ind].attribute &= (~KNOTMSK);
							stitchBuffer[0].x = sPnt.x;
							stitchBuffer[0].y = sPnt.y;
							stch2px1(ind);
							inlin[0].x = stitchSizePixels.x;
							inlin[0].y = stitchSizePixels.y;
							inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
							inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
							hed.stitchCount++;
							coltab();
							setMap(RESTCH);
							return 1;
						}
					} else {

						xlin();
						if (stitchBuffer[closestPointIndex].attribute&ALTYPMSK&&stitchBuffer[closestPointIndex + 1].attribute&ALTYPMSK)
						{
							if ((stitchBuffer[closestPointIndex].attribute&FRMSK) == (stitchBuffer[closestPointIndex + 1].attribute&FRMSK))
								cod = stitchBuffer[closestPointIndex].attribute | USMSK;
						}
						ind = hed.stitchCount;
						do {

							stitchBuffer[ind].x = stitchBuffer[ind - 1].x;
							stitchBuffer[ind].y = stitchBuffer[ind - 1].y;
							stitchBuffer[ind].attribute = stitchBuffer[ind - 1].attribute;
							ind--;
						} while (ind > (long)closestPointIndex);
						hed.stitchCount++;
						closestPointIndex++;
						stitchBuffer[closestPointIndex].x = sPnt.x;
						stitchBuffer[closestPointIndex].y = sPnt.y;
						stitchBuffer[closestPointIndex].attribute = cod;
						xlin();
						inlin[1].x = msg.pt.x - stitchWindowOrigin.x;
						inlin[1].y = msg.pt.y - stitchWindowOrigin.y;
						stch2px1(closestPointIndex);
						inlin[0].x = stitchSizePixels.x;
						inlin[0].y = stitchSizePixels.y;
						stch2px1(closestPointIndex + 1);
						inlin[2].x = stitchSizePixels.x;
						inlin[2].y = stitchSizePixels.y;
						coltab();
						setMap(RESTCH);
						ritnum(STR_NUMSEL, closestPointIndex);
						return 1;
					}
				}
				if ((!chkMap(HIDSTCH)) && closPnt1(&closestPointIndexClone)) {

					closestPointIndex = closestPointIndexClone;
					unbox();
					unboxs();
					setbak(pixSiz[stitchBuffer[closestPointIndex].attribute & 0xf] + 3);
					linePoints = new POINT[3];
					lineIndex = 0;
					SetROP2(StitchWindowDC, R2_NOTXORPEN);
					if (closestPointIndex == 0) {

						if (zoomFactor < STCHBOX) {

							SelectObject(StitchWindowDC, linePen);
							stchbox(0, StitchWindowDC);
							stchbox(1, StitchWindowDC);
						}
						SetROP2(StitchWindowDC, R2_COPYPEN);
						drwLin(0, 2, backgroundPen);
					} else {

						if (closestPointIndex == (unsigned)hed.stitchCount - 1) {

							if (zoomFactor < STCHBOX) {

								SelectObject(StitchWindowDC, linePen);
								stchbox(hed.stitchCount - 1, StitchWindowDC);
								stchbox(hed.stitchCount - 2, StitchWindowDC);
							}
							SetROP2(StitchWindowDC, R2_COPYPEN);
							drwLin(hed.stitchCount - 2, 2, backgroundPen);
						} else {

							if (zoomFactor < STCHBOX) {

								SelectObject(StitchWindowDC, linePen);
								stchbox(closestPointIndex - 1, StitchWindowDC);
								stchbox(closestPointIndex, StitchWindowDC);
								stchbox(closestPointIndex + 1, StitchWindowDC);
							}
							SetROP2(StitchWindowDC, R2_COPYPEN);
							drwLin(closestPointIndex - 1, 3, backgroundPen);
						}
					}
					delete[] linePoints;
					mlin0[1].x = mlin1[0].x = msg.pt.x - stitchWindowOrigin.x;
					mlin0[1].y = mlin1[0].y = msg.pt.y - stitchWindowOrigin.y;
					if (closestPointIndex <= 0)
						rstMap(ISDWN);
					else {

						setMap(ISDWN);
						stch2px1(closestPointIndex - 1);
						mlin0[0].x = stitchSizePixels.x;
						mlin0[0].y = stitchSizePixels.y;
					}
					tlng = hed.stitchCount - 1;
					if (closestPointIndex >= (unsigned)tlng)
						rstMap(ISUP);
					else {

						setMap(ISUP);
						stch2px1(closestPointIndex + 1);
						mlin1[1].x = stitchSizePixels.x;
						mlin1[1].y = stitchSizePixels.y;
					}
					dulin();
					SetCapture(hWnd);
					setMap(CAPT);
					ritnum(STR_NUMSEL, closestPointIndex);
				}
			} else {

				if (px2stch()) {

					savdo();
					hed.stitchCount = 1;
					inlin[0].x = msg.pt.x - stitchWindowOrigin.x;
					inlin[0].y = msg.pt.y - stitchWindowOrigin.y;
					stitchBuffer[0].attribute = USMSK | actcol | layerIndex | NOTFRM;
					stitchBuffer[0].x = sPnt.x;
					stitchBuffer[0].y = sPnt.y;
					colCnt = 1;
					colch[0].colorIndex = actcol;
					colch[0].stitchIndex = 0;
					colch[1].stitchIndex = 1;
					setMap(LIN1);
					setMap(INSRT);
					SetCapture(hWnd);
					closestPointIndex = 1;
					setMap(INIT);
					setMap(BAKEND);
				}
			}
			ritot(hed.stitchCount);
			setMap(BOXSLCT);
			setMap(BZUMIN);
			setMap(NOSEL);
			blin[0].x = blin[3].x = blin[4].x = msg.pt.x - stitchWindowOrigin.x;
			blin[0].y = blin[1].y = msg.pt.y - stitchWindowOrigin.y;
			blin[4].y = blin[0].y - 1;
			px2stch();
			zBoxo.x = sPnt.x;
			zBoxo.y = sPnt.y;
			setMap(VCAPT);
			return 1;
		}
		if (msg.hwnd == hbuts[HBOXSEL]) {

			boxsel();
			return 1;
		}
		if (msg.hwnd == hbuts[HUPTO]) {

			toglup();
			return 1;
		}
		if (msg.hwnd == hbuts[HHID]) {

			toglHid();
			return 1;
		}
		if (chkMsgs(msg.pt, hDef[0], hDef[15])) {

			if (msg.message == WM_LBUTTONDOWN) {

				savdo();
				cod = actcol;
				actcol = vind & 0xf;
				redraw(hCol[cod]);
				redraw(hCol[actcol]);
				if (chkMap(HID)) {

					rstMap(SELBOX);
					rstMap(GRPSEL);
					rstMap(SCROS);
					rstMap(ECROS);
					setMap(RESTCH);
					redraw(hbuts[HHID]);
				} else {

					if (selectedFormCount) {

						nucols();
						coltab();
						setMap(RESTCH);
						return 1;
					} else {

						if (chkMap(FORMSEL)) {

							ptrSelectedForm = &formList[closestFormToCursor];
							if (ptrSelectedForm->fillType || ptrSelectedForm->edgeType || ptrSelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) {

								if (formList[closestFormToCursor].fillType) {

									formList[closestFormToCursor].fillColor = actcol;
									if (formList[closestFormToCursor].fillType == FTHF)
										formList[closestFormToCursor].fillInfo.feather.color = actcol;
								}
								if (formList[closestFormToCursor].edgeType)
								{
									if (formList[closestFormToCursor].edgeType == EGAP)
									{
										formList[closestFormToCursor].borderColor &= 0xf0;
										formList[closestFormToCursor].borderColor |= actcol;
									} else
										formList[closestFormToCursor].borderColor = actcol;
								}
								if (ptrSelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
									formList[closestFormToCursor].underlayColor = actcol;
								cod = closestFormToCursor << FRMSHFT;
								for (ind = 0; ind < hed.stitchCount; ind++) {

									ine = stitchBuffer[ind].attribute;
									if (ine&ALTYPMSK && (ine&FRMSK) == cod && (ine&TYPMSK) != TYPMSK)
									{
										ine &= NCOLMSK;
										ine |= actcol;
										stitchBuffer[ind].attribute = ine;
									}
								}
								coltab();
								setMap(RESTCH);
							}
						} else {

							if (chkMap(GRPSEL)) {

								for (ind = groupStartStitch + 1; ind <= (long)groupEndStitch; ind++) {

									stitchBuffer[ind].attribute &= 0xfffffff0;
									stitchBuffer[ind].attribute |= actcol;
								}
								coltab();
								setMap(RESTCH);
							} else {

								if (chkMap(COL))
									setMap(RESTCH);
							}
						}
					}
				}
			}
			return 1;
		}
		if (chkMsgs(msg.pt, hCol[0], hCol[15])) {

			if (msg.message == WM_LBUTTONDOWN&&nuCol(useCol[vind])) {

				savdo();
				useCol[vind] = gCol.rgbResult;
				userPen[vind] = nuPen(userPen[vind], 1, useCol[vind]);
				hUserColorBrush[vind] = nuBrush(hUserColorBrush[vind], useCol[vind]);
				redraw(hCol[vind]);
				setMap(RESTCH);
			}
			return 1;
		}
		if (chkMsgs(msg.pt, hSiz[0], hSiz[15])) {

			if (msg.message == WM_LBUTTONDOWN) {

				savdo();
				thrdSel = vind;
				for (ind = 0; ind < 3; ind++)
					hSizChng[ind] = nuSiz(ind);
				setMap(SIZSEL);
			}
			return 1;
		}
	}
	switch (msg.message) {

	case WM_TIMER:

		if (chkMap(RUNPAT) && !msg.wParam)
			stchout();
		break;

	case WM_CHAR:

		if (isgraph(msg.wParam))
			nuthum(lchr(msg.wParam));
		break;

	case WM_KEYDOWN:

		cod = msg.wParam & 0xffff;
		if (chkMap(TXTRED))
		{
			txtkey(cod);
			return 1;
		}
		fvars(closestFormToCursor);

#if LANG==GRM
		if (cod >= 0x30 && cod <= 0x39) {

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				switch (cod) {

				case 0x38:

					selfrm0();
					break;

				case 0x39:

					selfrmx();
					break;
				}
			}
		}
#endif
		if (chkMap(FORMIN)) {

			if (GetKeyState(VK_CONTROL) & 0X8000)
				return 1;

			switch (cod) {

			case 'E':

				rstMap(FORMIN);
				unmsg();
				duform(LIN - 1);
				return 1;

			case 'F':

				rstMap(FORMIN);
				unmsg();
				duform(POLI - 1);
				return 1;

			case 'R':

				duform(RPOLI - 1);
				return 1;

			case 'S':

				duform(STAR - 1);
				return 1;

			case 'A':

				duform(SPIR - 1);
				return 1;

			case 'H':

				duform(HART - 2);
				return 1;

			case 'L':

				duform(LENS - 2);
				return 1;

			case 'G':

				duform(EGG - 2);
				return 1;

			case 'T':

				duform(TEAR - 2);
				return 1;

			case 'Z':

				duform(ZIG - 2);
				return 1;

			case 'W':

				duform(WAV - 2);
				return 1;

			case 'D':

				duform(DASY - 2);
				return 1;
			}
		}
		if (chkMap(FILMSG)) {

			if (cod == VK_RETURN || cod == 0xc0) {

				savdo();
				unfil();
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (rstMap(MOVMSG)) {

			if (cod == VK_RETURN || cod == 0xc0) {

				savdo();
				refilfn();
				unmsg();
			} else
				unmsg();
			if (rstMap(WASFRMFRM))
				refrm();
			return 1;
		}
		if (rstMap(PRGMSG)) {

			if (cod == VK_RETURN || cod == 0xc0) {

				deldir();
				return 1;
			}
		}
		if (rstMap(DELSFRMS)) {

			if (cod == 'S' || cod == VK_RETURN || cod == 0xc0) {

				savdo();
				if (cod == 'S')
					setMap(DELTO);
				else
					rstMap(DELTO);
				delsfrms(cod);
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (rstMap(DELFRM)) {

			if (cod == 'S' || cod == VK_RETURN || cod == 0xc0) {

				savdo();
				if (cod == 'S')
					setMap(DELTO);
				else
					rstMap(DELTO);
				frmdel();
				fndknt();
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (chkMap(THUMSHO)) {

			switch (cod) {

			case VK_ESCAPE:

				unthum();
				rstMap(BAKSHO);
				goto thumout;

			case VK_DOWN:
			case 0x22:		//page down

				nuthsel();
				break;

			case VK_UP:
			case 0x21:		//page up

				nuthbak(8);
				break;

			case VK_HOME:

				thumind = 0;
				nuthsel();
				break;

			case VK_END:

				thumind = thumcnt;
				nuthbak(4);
				break;

			case 8:			//backspace
			case VK_LEFT:

				bakthum();
				break;

			default:

				TranslateMessage(&msg);
			}
			return 1;
		}
		if (chkMap(FSETFSPAC) || chkMap(GTWLKIND))
		{
			if (cod == 189 || cod == 109)
			{
				*msgbuf = '-';
				msgIndex = 1;
				msgbuf[1] = 0;
				SetWindowText(hgNum, msgbuf);
				return 1;
			}
		}
		if (formMenuChoice || preferenceIndex) {

			if (chkminus(cod)) {

				msgIndex = 1;
				sideWindowEntryBuffer[0] = '-';
				SetWindowText(hSid, sideWindowEntryBuffer);
				return 1;
			}
			if (dunum(cod)) {

				if (preferenceIndex == PSHO + 1 || preferenceIndex == PBOX + 1) {

					msgbuf[0] = (TCHAR)numcod;
					msgbuf[1] = 0;
					if (preferenceIndex == PSHO + 1) {

						showStitchThreshold = unthrsh(numcod - 0x30);
						SetWindowText(thDat[PSHO], msgbuf);
					} else {

						StitchBoxesThreshold = unthrsh(numcod - 0x30);
						SetWindowText(thDat[PBOX], msgbuf);
					}
					unsid();
				} else {

					sideWindowEntryBuffer[msgIndex++] = numcod;
					sideWindowEntryBuffer[msgIndex] = 0;
					SetWindowText(hSid, sideWindowEntryBuffer);
				}
				return 1;
			}
			switch (cod) {

			case 0x6e://numpat period
			case 0xbe://period

				sideWindowEntryBuffer[msgIndex++] = '.';
				sideWindowEntryBuffer[msgIndex] = 0;
				SetWindowText(hSid, sideWindowEntryBuffer);
				return 1;

			case 8:	//backspace

				if (msgIndex) {

					sideWindowEntryBuffer[--msgIndex] = 0;
					SetWindowText(hSid, sideWindowEntryBuffer);
				}
				return 1;

			case VK_RETURN:

				chknum();
				formMenuChoice = 0;
				preferenceIndex = 0;
				return 1;
			}
		}
		if (cod == 'I') {

			movi();
			lastKeyCode = 'I';
			return 1;
		}
		if (cod == 'Q'&&lastKeyCode == 'Q')
			unpat();
		lastKeyCode = cod;
		if (chkMap(NUMIN)) {

			if (chkMap(SCLPSPAC) && cod == 0xbd && !msgIndex)
			{
				msgbuf[0] = '-';
				msgbuf[1] = 0;
				msgIndex = 1;
				SetWindowText(hgNum, msgbuf);
				return 1;
			}
			if (dunum(cod)) {

				if (chkMap(TRNIN0)) {

					trinbuf[msgIndex++] = numcod;
					trinbuf[msgIndex] = 0;
					ind = atoi(trinbuf);
					switch (msgIndex) {

					case 2:

						if (ind > 25)
							dutrnum0(ind);
						break;

					case 3:

						if (ind > 255)
							ind = 255;
						dutrnum0(ind);
						break;
					}
					redraw(htrnum);
				} else {

					msgbuf[msgIndex++] = numcod;
					msgbuf[msgIndex] = 0;
					SetWindowText(hgNum, msgbuf);
				}
				return 1;
			}
			switch (cod) {

			case 0x6e://numpad period
			case 0xbe://period

				msgbuf[msgIndex++] = '.';
				msgbuf[msgIndex] = 0;
				SetWindowText(hgNum, msgbuf);
				return 1;

			case 8:	//backspace

				if (msgIndex) {

					msgIndex--;
					if (chkMap(TRNIN0)) {

						trinbuf[msgIndex] = 0;
						redraw(htrnum);
					} else {

						msgbuf[msgIndex] = 0;
						SetWindowText(hgNum, msgbuf);
					}
				}
				return 1;

			case VK_RETURN:

				if (chkMap(TRNIN0))
					dutrnum0(atoi(trinbuf));
				else {

					if (chkMap(TRNIN1))
						dutrnum1();
				}
				break;
			}
		}
		if (cod == 8 && bufferIndex) {

			stitchEntryBuffer[--bufferIndex] = 0;
			butxt(HNUM, stitchEntryBuffer);
			closestPointIndex = atoi(stitchEntryBuffer);
			movbox();
			return 1;
		}
		if (!chkMap(WASTRAC) && dunum(cod)) {

			if (bufferIndex > bufferDigitCount - 1)
				bufferIndex = 0;
			if (stitchEntryBuffer[0] == '0'&&bufferIndex)
				bufferIndex--;
			stitchEntryBuffer[bufferIndex++] = numcod;
			stitchEntryBuffer[bufferIndex] = 0;
			closestPointIndex = atoi(stitchEntryBuffer);
			if (closestPointIndex > (unsigned)hed.stitchCount - 1) {

				sprintf_s(stitchEntryBuffer, sizeof(stitchEntryBuffer), "%d", hed.stitchCount - 1);
				closestPointIndex = hed.stitchCount - 1;
			}
			butxt(HNUM, stitchEntryBuffer);
			movbox();
			rstMap(NUMIN);
			return 1;
		}
		bufferIndex = 0;
		unmsg();
		switch (cod) {

		case VK_ESCAPE:

		thumout:;
			chkbit();
			duhbit(MF_UNCHECKED);
			unthum();
			rstMap(MOVSET);
			rstMap(HID);
			rstMap(FRMOF);
			rstMap(THRDS);
			redraw(hbuts[HHID]);
			CheckMenuItem(hMen, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
			rstMap(COL);
			CheckMenuItem(hMen, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
			setMap(DUMEN);
			rstMap(RUNPAT);
			rstMap(WASPAT);
			rstMap(WASBLAK);
			rstMap(GTUANG);
			rstMap(GTUSPAC);
			rstMap(GTWLKIND);
			rstMap(GTWLKLEN);
			DestroyWindow(hSped);
			rstMap(GMRK);

		case 'Q':

			if (GetKeyState(VK_SHIFT) & 0x8000) {

				ritcur();
				return 1;
			}
			untrace();
			rstMap(HIDMAP);
			rstMap(FILDIR);
			ReleaseCapture();
			if (hed.stitchCount == 1)
				hed.stitchCount = 0;
			if (!chku(MARQ))
				rstMap(GMRK);
			if (rstMap(PRFACT)) {

				if (rstMap(WASRT)) {

					setMap(INSRT);
					setMap(RESTCH);
					return 1;
				} else
					rstAll();
			} else
				rstAll();
			setMap(RESTCH);
			if (bufferIndex) {

				bufferIndex = 0;
				stitchEntryBuffer[0] = 0;
				closestPointIndex = 0;
				movbox();
			}
			butxt(HNUM, "");
			return 1;

		case 0xc0:		//`

			if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState(VK_MENU) & 0x8000) {

				backgroundColor = 0x505050;
				backgroundPen = nuPen(backgroundPen, 1, backgroundColor);
				backgroundPenWidth = 1;
				DeleteObject(hBackgroundBrush);
				hBackgroundBrush = CreateSolidBrush(backgroundColor);
				if (*bnam)
					bfil();
				setMap(RESTCH);
			} else {
#if PESACT
				//				ini.auxFileType=AUXPES;
				//				strcpy_s(filnam,"u:\\mrd\\t.thr");
				//				setMap(REDOLD);
				//				nuFil();
				//				lodpes();
				//				savpes();
#endif

				tst();
			}
			break;

		case VK_INSERT:

			inscol();
			break;

		case 0x6d:		//keypad -

			toglup();
			break;

		case 0xbd:		//-

			rstMap(CNTRH);
			rstMap(CNTRV);
			if (GetKeyState(VK_SHIFT) & 0X8000)
				setMap(CNTRV);
			else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					setMap(CNTRH);
			}
			cntrx();
			break;

		case 0xbf:		///forward slash /

			frmnum();
			break;

		case 0xbb:		//=

			shrnk();
			break;

		case 0x21:		//page up

			if (GetKeyState(VK_SHIFT) & 0X8000)
				redup();
			else
				rotagain();
			break;

		case 0x22:		//page down

			if (GetKeyState(VK_SHIFT) & 0X8000)
				bakdup();
			else
				bakagain();
			break;

		case 0xde:	//'

			desiz();
			break;

		case 0xdb:		//[

#if LANG==ENG||LANG==HNG

			if (GetKeyState(VK_SHIFT) & 0X8000)
				selfrm0();
			else
#endif
				ungrplo();
			break;

		case 0xdd:		//]

#if LANG==ENG||LANG==HNG

			if (GetKeyState(VK_SHIFT) & 0X8000)
				selfrmx();
			else
#endif
				ungrphi();
			break;

		case 0xba:		//semicolon

			movmrk();
			break;

		case VK_DOWN:

			if (GetKeyState(VK_CONTROL) & 0X8000)
				nudgfn(0, -ini.cursorNudgeStep);
			else
				seldwn();
			break;

		case VK_UP:

			if (GetKeyState(VK_CONTROL) & 0X8000)
				nudgfn(0, ini.cursorNudgeStep);
			else
				selup();
			break;

		case 0xbe:		//period

			if (GetKeyState(VK_SHIFT) & 0X8000)
				setfilend();
			else
				setmov();
			break;

		case 'W':

			if (GetKeyState(VK_SHIFT) & 0X8000)
				crop();
			else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					pes2crd();
				else
					insat();
			}
			break;

		case 'E':

			if (GetKeyState(VK_SHIFT) & 0X8000)
				dutxtfil();
			else
				infrm();
			break;

		case VK_F2:

			snap();
			break;

		case VK_F1:

			help();
			break;

		case VK_F3:

			if (GetKeyState(VK_CONTROL) & 0X8000)
				srtfrm();
			else {

				if (GetKeyState(VK_SHIFT) & 0X8000)
					srtbyfrm();
				else
					fsort();
			}
			break;

		case VK_F4:

			rtrclp();
			break;

		case VK_F5:

			filfrms();
			break;

		case VK_F6:

			vuthrds();
			break;

		case VK_F7:

			save();
			break;

		case VK_F8:

			savAs();
			break;

		case VK_F9:

			selCol();
			break;

		case VK_F11:

			if (GetKeyState(VK_SHIFT) & 0X8000)
				rembig();
			else
				duzero();
			break;

		case VK_F12:

			if (GetKeyState(VK_CONTROL)&GetKeyState(VK_MENU) & 0X8000)
				reldun();
			else
			{
#ifdef _DEBUG
				if (GetKeyState(VK_SHIFT) & 0X8000)
					dmpat();
				else
#endif
					dun();
			}
			break;

		case 'Y':

			if (closfrm())
				selfpnt();
			break;

		case 'O':

			fop();
			break;

		case 'Z':

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				zoomFactor = zoomMin;
				zumin();
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					bak();
				else
					zumin();
			}
			break;

		case 'X':

			if (GetKeyState(VK_CONTROL) & 0X8000)
				cut();
			else {

				if (GetKeyState(VK_SHIFT) & 0X8000)
					hidbit();
				else
					zumhom();
			}
			break;

		case 'P':

			prfmsg();
			break;

		case 'N':

			if (GetKeyState(VK_CONTROL) & 0X8000)
				nudsiz();
			else
			{
				if (GetKeyState(VK_SHIFT) & 0X8000)
					pgdwn();
				else
				{
					setMap(TRCUP);
					trace();
				}
			}
			break;

		case 'U':

			if (GetKeyState(VK_SHIFT) & 0X8000)
				pgup();
			else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					trdif();
				else {

					rstMap(TRCUP);
					trace();
				}
			}
			break;

		case 'H':

			if (GetKeyState(VK_SHIFT) & 0X8000)
				pglft();
			else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					trcsel();
				else
					blak();
			}
			break;

		case 'J':

			if (GetKeyState(VK_SHIFT) & 0X8000)
				pgrit();
			else
				refilal();
			break;

		case 0xbc:		//comma

			if (GetKeyState(VK_SHIFT) & 0X8000)
				setfilstrt();
			else
			{
				savdo();
				join();
			}
			break;

		case 'B':

			if (GetKeyState(VK_CONTROL) & 0X8000)
				redo();
			else {

				if (GetKeyState(VK_SHIFT) & 0X8000)
					bakmrk();
				else
					bak();
			}
			break;

		case 'D':

			if (GetKeyState(VK_SHIFT) & 0X8000)
			{
				if (chkMap(FORMSEL))
				{
					PostMessage(hWnd, WM_SYSCOMMAND, SC_KEYMENU, 'E');
					keybd_event('F', 0, 0, 0);
				}
			} else
				satsel();
			break;

		case 'K':

			if (GetKeyState(VK_MENU)&GetKeyState(VK_CONTROL) & 0X8000)
				setknots();
			else
			{
				if (GetKeyState(VK_SHIFT) & 0X8000)
					tglhid();
				else {

					if (GetKeyState(VK_CONTROL) & 0X8000)
						set1knot();
					else
						tglfrm();
				}
			}
			break;

		case 'A':

			if (GetKeyState(VK_CONTROL) & 0X8000)
				selalstch();
			else {

				if (GetKeyState(VK_SHIFT) & 0X8000)
					selal();
				else
					zumout();
			}
			break;

		case VK_SPACE:

			if (toglMap(INSRT)) {

				ReleaseCapture();
				setMap(RESTCH);
			} else
				istch();
			unbox();
			if (rstMap(GRPSEL) || rstMap(FORMSEL))
				setMap(RESTCH);
			break;

		case 'T':

			if (GetKeyState(VK_SHIFT) & 0x8000)
				retrac();
			else {

				if (GetKeyState(VK_CONTROL) & 0x8000)
					trinit();
				else
					thumnail();
			}
			break;

		case 'R':

			if (GetKeyState(VK_CONTROL)&GetKeyState(VK_SHIFT) & 0x8000) {

				movStch();
				unbox();
				setMap(RESTCH);
			} else {

				if (GetKeyState(VK_CONTROL) & 0x8000) {

					setrang();
					return 1;
				}
				if (GetKeyState(VK_SHIFT) & 0x8000) {

					rotmrk();
					return 1;
				} else
					rotseg();
			}
			break;

		case VK_TAB:

			rot();
			break;

		case 'S':

			if (GetKeyState(VK_CONTROL) & 0X8000) {

				colchk();
				save();
			} else {

				if (GetKeyState(VK_SHIFT) & 0X8000)
					gsnap();
				else
					zumshft();
			}
			break;

		case 'F':

			if (selectedFormCount)
			{
				PostMessage(hWnd, WM_SYSCOMMAND, SC_KEYMENU, 'E');
				keybd_event('F', 0, 0, 0);
			} else {

				if (GetKeyState(VK_SHIFT) & 0X8000) {

					setMap(CNV2FTH);
					ribon();
				} else {

					frmon();
					if (chkMap(FORMSEL))
						refrm();
					else
						form();
				}
			}
			break;

		case 'V':

			if (GetKeyState(VK_CONTROL) & 0X8000 && OpenClipboard(hWnd)) {

				htclp = RegisterClipboardFormat(thrEdClipFormat);
				hClipMem = GetClipboardData(htclp);
				if (hClipMem) {

					ptrClipVoid = GlobalLock(hClipMem);
					clipboardFormPointsData = (FORMPOINTCLIP*)ptrClipVoid;
					if (clipboardFormPointsData->clipType == CLP_FRMPS) {

						duzrat();
						ind = sizeof(FORMPOINTCLIP) + (clipboardFormPointsData->pointCount + 1) * sizeof(fPOINT);
						MoveMemory(&bseq, ptrClipVoid, ind);
						GlobalUnlock(hClipMem);
						clipboardFormPointsData = (FORMPOINTCLIP*)&bseq;
						if (chkMap(FRMPSEL)) {

							pchr = (unsigned char*)&bseq;
							fvars(closestFormToCursor);
							iseq[0].x = currentFormVertices[closestVertexToCursor].x;
							iseq[0].y = currentFormVertices[closestVertexToCursor].y;
							clp = (fPOINT*)&clipboardFormPointsData[1];
							for (ind = 0; ind <= clipboardFormPointsData->pointCount; ind++) {

								iseq[ind + 1].x = clp[ind].x;
								iseq[ind + 1].y = clp[ind].y;
							}
							ine = nxt(closestVertexToCursor);
							ind++;
							iseq[ind].x = currentFormVertices[ine].x;
							iseq[ind].y = currentFormVertices[ine].y;
							opnt = ind + 1;
							fplin = (POINT*)&iseq[opnt];
							setpclp();
							setMap(FPUNCLP);
							setMap(SHOP);
							dupclp();
						} else {

							fmovdif.x = fmovdif.y = 0;
							setMap(FUNCLP);
							closestFormToCursor = formIndex;
							ptrSelectedForm = &formList[formIndex];
							FillMemory(ptrSelectedForm, sizeof(FRMHED), 0);
							ptrSelectedForm->type = LIN;
							ptrSelectedForm->sides = clipboardFormPointsData->pointCount + 1;
							ptrSelectedForm->vertices = adflt(ptrSelectedForm->sides);
							fvars(closestFormToCursor);
							MoveMemory(ptrSelectedForm->vertices, &clipboardFormPointsData[1], sizeof(fPOINT)*ptrSelectedForm->sides);
							setMap(INIT);
							newFormVertexCount = ptrSelectedForm->sides;
							unfrm();
							setmfrm();
							setMap(SHOFRM);
							dufrm();
						}
						return 1;
					}
					clipboardFormsData = (FORMSCLIP*)ptrClipVoid;
					if (clipboardFormsData->clipType == CLP_FRMS) {

						clipboardFormsCount = clipboardFormsData->formCount;
						tfrm = (FRMHED*)&clipboardFormsData[1];
						for (ind = 0; ind < clipboardFormsCount; ind++) {

							frmcpy(&formList[formIndex + ind], &tfrm[ind]);
							formList[formIndex + ind].attribute = (formList[formIndex + ind].attribute&NFRMLMSK) | (activeLayer << 1);
						}
						currentFormVertices = (fPOINT*)&tfrm[ind];
						inf = 0;
						for (ind = 0; ind < clipboardFormsCount; ind++) {

							ptrSelectedForm = &formList[formIndex + ind];
							ptrSelectedForm->vertices = adflt(ptrSelectedForm->sides);
							for (ine = 0; ine < ptrSelectedForm->sides; ine++) {

								ptrSelectedForm->vertices[ine].x = currentFormVertices[inf].x;
								ptrSelectedForm->vertices[ine].y = currentFormVertices[inf++].y;
							}
						}
						l_sac = (SATCON*)&currentFormVertices[inf];
						inf = 0;
						for (ind = 0; ind < clipboardFormsCount; ind++) {

							ptrSelectedForm = &formList[formIndex + ind];
							if (ptrSelectedForm->type == SAT&&ptrSelectedForm->satinGuideCount) {

								ptrSelectedForm->satinOrAngle.sac = adsatk(ptrSelectedForm->satinGuideCount);
								for (ine = 0; ine < ptrSelectedForm->satinGuideCount; ine++) {

									ptrSelectedForm->satinOrAngle.sac[ine].start = l_sac[inf].start;
									ptrSelectedForm->satinOrAngle.sac[ine].finish = l_sac[inf++].finish;
								}
							}
						}
						clp = (fPOINT*)&l_sac[inf];
						inf = 0;
						for (ind = 0; ind < clipboardFormsCount; ind++) {

							ptrSelectedForm = &formList[formIndex + ind];
							if (isclpx(formIndex + ind)) {

								ptrSelectedForm->angleOrClipData.clip = adclp(ptrSelectedForm->lengthOrCount.clipCount);
								for (ine = 0; (unsigned)ine < ptrSelectedForm->lengthOrCount.clipCount; ine++) {

									ptrSelectedForm->angleOrClipData.clip[ine].x = clp[inf].x;
									ptrSelectedForm->angleOrClipData.clip[ine].y = clp[inf++].y;
								}
							}
							if (iseclpx(formIndex + ind)) {

								ptrSelectedForm->borderClipData = adclp(ptrSelectedForm->clipEntries);
								for (ine = 0; ine < ptrSelectedForm->clipEntries; ine++) {

									ptrSelectedForm->borderClipData[ine].x = clp[inf].x;
									ptrSelectedForm->borderClipData[ine].y = clp[inf++].y;
								}
							}
						}
						pts = (TXPNT*)&clp[inf];
						ptx = &txpnts[txad];
						ine = 0;
						for (ind = 0; ind < clipboardFormsCount; ind++)
						{
							if (istx(formIndex + ind))
							{
								ptrSelectedForm = &formList[formIndex + ind];
								ine += ptrSelectedForm->fillInfo.texture.count;
								ptrSelectedForm->fillInfo.texture.index += txad;
								MoveMemory(ptx, pts, (ptrSelectedForm->fillInfo.texture.index + ptrSelectedForm->fillInfo.texture.count - txad) * sizeof(TXPNT));
							}
						}
						txad += ine;
						GlobalUnlock(hClipMem);
						selectedFormsRectangle.top = selectedFormsRectangle.left = 0x7fffffff;
						selectedFormsRectangle.bottom = selectedFormsRectangle.right = 0;
						ratsr();
						for (opnt = 0; opnt < (unsigned)clipboardFormsCount; opnt++) {

#if  __UseASM__
							fselrct(opnt + formIndex);
#else
							fselrct(opnt + formIndex);
#endif
							selectedFormList[opnt] = opnt + formIndex;
						}
						selectedFormCount = clipboardFormsCount;
						bigsiz.x = selectedFormsRectangle.right - selectedFormsRectangle.left;
						bigsiz.y = selectedFormsRectangle.bottom - selectedFormsRectangle.top;
						setMap(INIT);
						flin[0].x = flin[3].x = flin[4].x = selectedFormsRectangle.left;
						flin[1].x = flin[2].x = selectedFormsRectangle.right;
						flin[0].y = flin[1].y = flin[4].y = selectedFormsRectangle.top;
						flin[2].y = flin[3].y = selectedFormsRectangle.bottom;
						setMap(SHOSTRTCH);
						strtchbox();
						fmovdif.x = ((selectedFormsRectangle.right - selectedFormsRectangle.left) >> 1);
						fmovdif.y = ((selectedFormsRectangle.bottom - selectedFormsRectangle.top) >> 1);
						setMap(MOVFRMS);
						setMap(FUNSCLP);
					} else {

						clipboardFormData = (FORMCLIP*)ptrClipVoid;
						if (clipboardFormData->clipType == CLP_FRM) {

							fmovdif.x = fmovdif.y = 0;
							setMap(FUNCLP);
							closestFormToCursor = formIndex;
							fvars(formIndex);
							frmcpy(&formList[formIndex], &clipboardFormData->form);
							formList[formIndex].attribute = (formList[formIndex].attribute&NFRMLMSK) | (activeLayer << 1);
							ptrSelectedForm->vertices = adflt(formList[formIndex].sides);
							currentFormVertices = (fPOINT*)&clipboardFormData[1];
							mvflpnt(&ptrSelectedForm->vertices[0], &currentFormVertices[0], ptrSelectedForm->sides);
							l_sac = (SATCON*)&currentFormVertices[ptrSelectedForm->sides];
							ind = 0;
							if (ptrSelectedForm->type == SAT&&ptrSelectedForm->satinGuideCount) {

								ptrSelectedForm->satinOrAngle.sac = adsatk(ptrSelectedForm->satinGuideCount);
								mvsatk(&ptrSelectedForm->satinOrAngle.sac[0], &l_sac[0], ptrSelectedForm->satinGuideCount);
							}
							clp = (fPOINT*)&l_sac[ind];
							inf = 0;
							if (isclpx(formIndex)) {

								ptrSelectedForm->angleOrClipData.clip = adclp(ptrSelectedForm->lengthOrCount.clipCount);
								mvflpnt(ptrSelectedForm->angleOrClipData.clip, clp, ptrSelectedForm->lengthOrCount.clipCount);
								inf += ptrSelectedForm->lengthOrCount.clipCount;
							}
							if (iseclpx(formIndex)) {

								clp = (fPOINT*)&clp[inf];
								ptrSelectedForm->borderClipData = adclp(ptrSelectedForm->clipEntries);
								mvflpnt(ptrSelectedForm->borderClipData, clp, ptrSelectedForm->clipEntries);
								inf += ptrSelectedForm->clipEntries;
							}
							pts = (TXPNT*)&clp[inf];
							if (istx(formIndex))
							{
								ptrSelectedForm->fillInfo.texture.index = txad;
								ptx = adtx(ptrSelectedForm->fillInfo.texture.count);
								MoveMemory(ptx, pts, ptrSelectedForm->fillInfo.texture.count * sizeof(TXPNT));
							}
						}
						GlobalUnlock(hClipMem);
						setMap(INIT);
						newFormVertexCount = ptrSelectedForm->sides;
						if (ptrSelectedForm->type != LIN)
							newFormVertexCount++;
						unfrm();
						duzrat();
						setmfrm();
						setMap(SHOFRM);
						dufrm();
					}
				} else {

					hClip = RegisterClipboardFormat(pcdClipFormat);
					hClipMem = GetClipboardData(hClip);
					if (hClipMem) {

						redclp();
						clpbox();
						setMap(CLPSHO);
					}
				}
				CloseClipboard();
			} else
				closPnt();
			break;

		case 'C':

			if (GetKeyState(VK_CONTROL) & 0X8000)
				duclip();
			else {

				if (GetKeyState(VK_SHIFT) & 0X8000) {

					rstMap(CNV2FTH);
					ribon();
				} else {

					rstMap(HIDSTCH);
					setMap(IGNTHR);
					rebox();
					rstMap(IGNTHR);
					setMap(RESTCH);
				}
			}
			break;

		case VK_HOME:

			if (GetKeyState(VK_SHIFT) & 0X8000 && GetKeyState(VK_CONTROL) & 0X8000) {

				if (rstMap(SELBOX)) {

					groupStitchIndex = 0;
					dubx();
					setMap(GRPSEL);
				} else {

					if (chkMap(GRPSEL)) {

						if (groupStitchIndex > closestPointIndex)
							closestPointIndex = 0;
						else
							groupStitchIndex = 0;
					} else {

						setMap(SELBOX);
						setMap(RESTCH);
					}
				}
				grpAdj();
				redraw(hBar);
				return 1;
			}
			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (rstMap(SELBOX)) {

					dubx();
					setMap(GRPSEL);
					ind = pt2colInd(closestPointIndex);
				} else
					ind = pt2colInd(groupStitchIndex);
				ind--;
				groupStitchIndex = colch[ind].stitchIndex;
				grpAdj();
				redraw(hBar);
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000) {

					stch2px1(0);
					endpnt();
					rstMap(BAKEND);
					setMap(RESTCH);
				} else {

					if (chkMap(SELBOX)) {

						ind = pt2colInd(closestPointIndex);
						if (ind)
							ind--;
						closestPointIndex = colch[ind].stitchIndex;
					} else {

						if (chkMap(LENSRCH)) {

							setsrch(smallestStitchIndex);
							return 1;
						} else
							closestPointIndex = 0;
					}
					movbox();
				}
			}
			break;

		case VK_END:

			if (GetKeyState(VK_SHIFT) & 0X8000 && GetKeyState(VK_CONTROL) & 0X8000) {

				if (rstMap(SELBOX)) {

					groupStitchIndex = hed.stitchCount - 1;
					dubx();
					setMap(GRPSEL);
				} else {

					if (chkMap(GRPSEL)) {

						if (groupStitchIndex > closestPointIndex)
							groupStitchIndex = hed.stitchCount - 1;
						else
							closestPointIndex = hed.stitchCount - 1;
					} else {

						setMap(SELBOX);
						closestPointIndex = hed.stitchCount - 1;
						setMap(RESTCH);
						return 1;
					}
				}
				grpAdj();
				redraw(hBar);
				break;
			}
			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (rstMap(SELBOX)) {

					dubx();
					setMap(GRPSEL);
					ind = pt2colInd(closestPointIndex);
				} else
					ind = pt2colInd(groupStitchIndex);
				groupStitchIndex = colch[ind].stitchIndex - 1;
				grpAdj();
				redraw(hBar);
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000) {

					stch2px1(hed.stitchCount - 1);
					endpnt();
					setMap(BAKEND);
					setMap(RESTCH);
				} else {

					if (chkMap(SELBOX)) {

						ind = pt2colInd(closestPointIndex);
						closestPointIndex = colch[ind].stitchIndex - 1;
					} else {

						if (chkMap(LENSRCH)) {

							setsrch(largestStitchIndex);
							return 1;
						} else
							closestPointIndex = hed.stitchCount - 1;
					}
					movbox();
				}
			}
			break;

		case VK_RIGHT:

			fvars(closestFormToCursor);
			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (chkMap(FPSEL)) {

					if (chkMap(PSELDIR)) {

						++selectedFormPoints.pointCount %= sides;
						selectedFormPoints.finish = (selectedFormPoints.start + selectedFormPoints.pointCount) % sides;
					} else {

						if (selectedFormPoints.pointCount) {

							selectedFormPoints.pointCount--;
							selectedFormPoints.finish = (selectedFormPoints.start + sides - selectedFormPoints.pointCount) % sides;
						} else {

							selectedFormPoints.pointCount = 1;
							setMap(PSELDIR);
							selectedFormPoints.finish = (selectedFormPoints.start + 1) % sides;
						}
					}
					setpsel();
				} else {

					if (rstMap(FRMPSEL)) {

						unpsel();
						selectedFormPoints.start = closestVertexToCursor;
						selectedFormPoints.form = closestFormToCursor;
						selectedFormPoints.pointCount = 1;
						selectedFormPoints.finish = (selectedFormPoints.start + 1) % sides;
						setMap(PSELDIR);
						setpsel();
					} else {

						rstMap(LENSRCH);
						rstMap(FORMSEL);
						if (rstMap(SELBOX)) {

							dubx();
							if (closestPointIndex < (unsigned)hed.stitchCount - 1) {

								setMap(GRPSEL);
								groupStitchIndex = closestPointIndex + 1;
							}
						} else {

							if (groupStitchIndex < (unsigned)hed.stitchCount - 1) {

								groupStitchIndex++;
								nuAct(groupStitchIndex);
							}
						}
						grpAdj();
						redraw(hBar);
					}
				}
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(ini.cursorNudgeStep, 0);
				else {

					if (chkMap(LENSRCH)) {

						longer();
						setMap(RESTCH);
					} else {

						if (chkMap(FRMPSEL)) {

							fvars(closestFormToCursor);
							closestVertexToCursor = nxt(closestVertexToCursor);
							ritnum(STR_NUMPNT, closestVertexToCursor);
							ritfcor(&formList[closestFormToCursor].vertices[closestVertexToCursor]);
							shftflt(formList[closestFormToCursor].vertices[closestVertexToCursor]);
							setMap(RESTCH);
						} else {

							if (chkMap(SELBOX)) {

								if (closestPointIndex < (unsigned)hed.stitchCount - 1)
									closestPointIndex++;
								movbox();
								return 1;
							}
							if (chkMap(GRPSEL)) {

								if (groupStitchIndex < (unsigned)hed.stitchCount - 1) {

									groupStitchIndex++;
									grpAdj();
									redraw(hBar);
								}
							}
						}
					}
				}
			}
			break;

		case VK_LEFT:

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (chkMap(FPSEL)) {

					if (!chkMap(PSELDIR)) {

						++selectedFormPoints.pointCount %= sides;
						selectedFormPoints.finish = (selectedFormPoints.start + sides - selectedFormPoints.pointCount) % sides;
					} else {

						if (selectedFormPoints.pointCount) {

							selectedFormPoints.pointCount--;
							selectedFormPoints.finish = (selectedFormPoints.start + sides - selectedFormPoints.pointCount) % sides;
						} else {

							selectedFormPoints.pointCount = 1;
							selectedFormPoints.finish = (selectedFormPoints.start + sides - 1) % sides;
							rstMap(PSELDIR);
						}
					}
					setpsel();
				} else {

					if (rstMap(FRMPSEL)) {

						unpsel();
						selectedFormPoints.start = closestVertexToCursor;
						selectedFormPoints.pointCount = 1;
						rstMap(PSELDIR);
						setpsel();
					} else {

						rstMap(LENSRCH);
						rstMap(FORMSEL);
						if (rstMap(SELBOX)) {

							dubx();
							setMap(GRPSEL);
							groupStitchIndex = closestPointIndex - 1;
						} else
							if (groupStitchIndex) {

								groupStitchIndex--;
								nuAct(groupStitchIndex);
							}
						grpAdj();
						redraw(hBar);
					}
				}
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(-ini.cursorNudgeStep, 0);
				else {

					if (chkMap(LENSRCH)) {

						shorter();
						setMap(RESTCH);
					} else {

						if (chkMap(FRMPSEL)) {

							fvars(closestFormToCursor);
							closestVertexToCursor = prv(closestVertexToCursor);
							ritnum(STR_NUMPNT, closestVertexToCursor);
							ritfcor(&formList[closestFormToCursor].vertices[closestVertexToCursor]);
							shftflt(formList[closestFormToCursor].vertices[closestVertexToCursor]);
							setMap(RESTCH);
						} else {

							if (chkMap(SELBOX)) {

								if (closestPointIndex)
									closestPointIndex--;
								movbox();
								return 1;
							}
							if (chkMap(GRPSEL)) {

								if (groupStitchIndex) {

									groupStitchIndex--;
									grpAdj();
									redraw(hBar);
								}
							}
						}
					}
				}
			}
			break;

		case 'G':

			mark();
			break;

		case 'M':

			if (GetKeyState(VK_CONTROL) & 0x8000)
				pntmrk();
			else {

				if (GetKeyState(VK_SHIFT) & 0X8000)
					dumrk(unzoomedRect.x / 2, unzoomedRect.y / 2);
				else {

					if (px2stch())
						dumrk(sPnt.x, sPnt.y);
				}
				setMap(RESTCH);
			}
			break;

		case 'L':

			if (GetKeyState(VK_SHIFT) & 0x8000)
				delstch();
			else
				fcntr();
			break;

		case VK_DELETE:

			if (chkMsgs(msg.pt, hDef[0], hDef[15]))
				delcol();
			else
				delet();
			break;
		}
		break;

	case WM_COMMAND:

		unmsg();
		if (chkMap(FORMSEL))
			fvars(closestFormToCursor);
		for (ind = 0; ind < OLDNUM; ind++) {

			if (msg.wParam == LRUMenuId[ind]) {

				strcpy_s(filnam, ini.prevNames[ind]);
				setMap(REDOLD);
				nuFil();
			}
		}
		ind = LOWORD(msg.wParam);
		if (ind > 40000 && ind < 40400)
			undat();
		switch (LOWORD(msg.wParam)) {

		case ID_CHKOF:

			chgchk(0);
			break;

		case ID_CHKON:

			chgchk(1);
			break;

		case ID_CHKREP:

			chgchk(2);
			break;

		case ID_CHKREPMSG:

			chgchk(3);
			break;

		case ID_REPAIR:

			repar();
			break;

		case ID_WARNOF:

			chgwrn();
			break;

		case ID_CLPSPAC:

			setclpspac();
			break;

		case ID_FRMIND:

			setfind();
			break;

		case ID_SETSIZ:

			nudsiz();
			break;

		case ID_TXFIL:

			dutxtfil();
			break;

		case ID_FRMHI:

			setfhi();
			break;

		case ID_FRMWID:

			setfwid();
			break;

		case ID_MAXFLEN:

			setfmax();
			break;

		case ID_MINFLEN:

			setfmin();
			break;

		case ID_MAXBLEN:

			setbmax();
			break;

		case ID_MINBLEN:

			setbmin();
			break;

		case ID_SETBSPAC:

			setbspac();
			break;

		case ID_SETBLEN:

			setblen();
			break;

		case ID_SETBCOL:

			setbcol();
			break;

		case ID_SETFCOL:

			setfcol();
			break;

		case ID_SETUCOL:

			setucol();
			break;

		case ID_SETFANG:

			setfang();
			break;

		case ID_SETFSPAC:

			setfspac();
			break;

		case ID_SETFLEN:

			setflen();
			break;

		case ID_SETUANG:

			sfuang();
			break;

		case ID_SETUSPAC:

			uspac();
			break;

		case ID_UNDLEN:

			undlen();
			break;

		case ID_SETCWLK:

			setcwlk();
			break;

		case ID_SETWLK:

			setwlk();
			break;

		case ID_SETUND:

			setund();
			break;

		case ID_NOTCWLK:

			notcwlk();
			break;

		case ID_NOTWLK:

			notwlk();
			break;

		case ID_NOTUND:

			notund();
			break;

		case ID_SELUND:

			selfil(UNDMSK);
			break;

		case ID_SELWLK:

			selfil(WLKMSK);
			break;

		case ID_ALFRM:

			selalfrm();
			break;

		case ID_USPAC:

			setuspac();
			break;

		case ID_UANG:

			setuang();
			break;

		case ID_USTCH:

			setulen();
			break;

		case ID_WIND:

			setwlkind();
			break;

		case ID_FILSTRT:

			setfilstrt();
			break;

		case ID_FILEND:

			setfilend();
			break;

		case ID_PES2CRD:

			pes2crd();
			break;

		case ID_2FTHR:

			setMap(CNV2FTH);
			ribon();
			break;

		case ID_FETHR:

			fethr();
			break;

		case ID_FTHDEF:

			dufdef();
			break;

		case ID_SRTF:

			srtfrm();
			break;

		case ID_FILCLPX:

			filclpx();
			break;

		case ID_FRMX:

			frmcursel(1);
			break;

		case ID_FRMBOX:

			frmcursel(0);
			break;

		case ID_KNOTAT:

			set1knot();
			break;

		case ID_STCHPIX:

			getstpix();;
			break;

		case ID_FRMPIX:

			getfrmpix();
			break;

		case ID_FRMPBOX:

			getfrmbox();
			break;

		case ID_CROP:

			crop();
			break;

		case ID_BAKMRK:

			bakmrk();
			break;

		case ID_MARKESC:

			setu(MARQ);
			qchk();
			break;

		case ID_MARKQ:

			rstu(MARQ);
			qchk();
			break;

		case ID_NUDGPIX:

			getnpix();
			break;

		case ID_LINCHN:

			setMap(LINCHN);
			chain();
			break;

		case ID_OPNCHN:

			rstMap(LINCHN);
			chain();
			break;

		case ID_CLOSE:

			filclos();
			break;

		case ID_DELMAP:

			delmap();
			break;

		case ID_BLAK:

			blak();
			break;

		case ID_SAVMAP:

			savmap();
			break;

		case ID_TRDIF:

			trdif();
			break;

		case ID_TRACEDG:

			tracedg();
			break;

		case ID_TRCSEL:

			trcsel();
			break;

		case ID_TRACE:

			trinit();
			break;

		case ID_FLOK:

			lock();
			break;

		case ID_ROTAUXON:

			rotauxsel(1);
			break;

		case ID_ROTAUXOFF:

			rotauxsel(0);
			break;

		case ID_FRM2COL:

			col2frm();
			break;

		case ID_SNAP2GRD:

			gsnap();
			break;

		case ID_FIL2SEL_ON:

			fil2sel(1);
			break;

		case ID_FIL2SEL_OFF:

			fil2sel(0);
			break;

		case ID_OVRLAY:

			ovrlay();
			break;

		case ID_GRDHI:

			setgrd(HIGRD);
			break;

		case ID_GRDMED:

			setgrd(MEDGRD);
			break;

		case ID_GRDEF:

			setgrd(DEFGRD);
			break;

		case ID_GRDRED:

			setgrd(REDGRD);
			break;

		case ID_GRDBLU:

			setgrd(BLUGRD);
			break;

		case ID_GRDGRN:

			setgrd(GRNGRD);
			break;

		case ID_RETRACE:

			retrac();
			break;

		case ID_DUBFIL:

			dubfil();
			break;

		case ID_HORCLP:

			if (chkMap(FORMSEL) || selectedFormCount)
				savdo();
			horclp();
			break;

		case ID_ANGCLP:

			if (chkMap(FORMSEL) || selectedFormCount)
				savdo();
			angclp();
			break;

		case ID_VRTCLP:

			if (chkMap(FORMSEL) || selectedFormCount)
				savdo();
			vrtclp();
			break;

		case ID_LINBEXACT:

			rstu(LINSPAC);
			linbmen();
			break;

		case ID_LINBEVEN:

			setu(LINSPAC);
			linbmen();
			break;

		case ID_BSAVON:

			pcsbsavon();
			break;

		case ID_BSAVOF:

			pcsbsavof();
			break;

		case ID_KNOTON:

			shoknot();
			break;

		case ID_KNOTOF:

			hidknot();
			break;

		case ID_DELKNOT:

			delknot();
			break;

		case ID_RSTNEDL:

			nedof();
			break;

		case ID_SETNEDL:

			nedon();
			break;

		case ID_STCHS2FRM:

			stchs2frm();
			break;

		case ID_SPLTFRM:

			spltfrm();
			break;

		case ID_UNBEAN:

			debean();
			break;

		case ID_DUBEAN:

			dubean();
			break;

		case ID_SRTBF:

			srtbyfrm();
			break;

		case ID_CENTIRE:

			centir();
			break;

		case ID_CNTRX:

			rstMap(CNTRH);
			rstMap(CNTRV);
			cntrx();
			break;

		case ID_CNTRH:

			rstMap(CNTRH);
			setMap(CNTRV);
			cntrx();
			break;

		case ID_CNTRV:

			setMap(CNTRH);
			rstMap(CNTRV);
			cntrx();
			break;

		case ID_FRMNUM:

			frmnum();
			break;

		case ID_REFILF:

			filfrms();
			break;

		case ID_MRKPNT:

			pntmrk();
			break;

		case ID_ROTSEG:

			rotseg();
			break;

		case ID_ROTMRK:

			rotmrk();
			break;

		case ID_MRKCNTR:

			dumrk(unzoomedRect.x / 2, unzoomedRect.y / 2);
			setMap(RESTCH);
			break;

		case ID_SETROT:

			setrang();
			break;

		case ID_SETPREF:

			defpref();
			break;

		case ID_SHRINK:

			shrnk();
			break;

		case ID_DUPAGAIN:

			redup();
			break;

		case ID_CLPADJ:

			clpadj();
			break;

		case ID_DELTOT:

			deltot();
			break;

		case ID_AUXPCS:

			setpcs();
			break;

		case ID_AUXDST:

			setdst();
			break;

		case ID_ABOUT:

			tabmsg(IDS_CPYRIT);
			break;

		case ID_RIBON:

			rstMap(CNV2FTH);
			ribon();
			break;

		case ID_DESIZ:

			desiz();
			break;

		case ID_HLP:

			help();
			break;

		case ID_CNTR:

			fcntr();
			break;

		case ID_INSFIL:

			rstMap(IGNORINS);
			insfil();
			break;

		case ID_SELALSTCH:

			selalstch();
			break;

		case ID_UNGRPLO:

			ungrplo();
			break;

		case ID_UNGRPHI:

			ungrphi();
			break;

		case ID_CONTF:

			contfil();
			break;

		case ID_THUM:

			thumnail();
			break;

		case ID_PICOT:

			picot();
			break;

		case ID_FILBUT:

			bhol();
			break;

		case ID_REMBIG:

			rembig();
			break;

		case ID_VUSELTHRDS:

			vuselthr();
			break;

		case ID_VUTHRDS:

			vuthrds();
			break;

		case ID_MOVMRK:

			movmrk();
			break;

		case ID_SELFIL:

			selfil(FRMFIL);
			break;

		case ID_SELBRD:

			selfil(FRMBFIL);
			break;

		case ID_SELAP:

			selfil(FRMAPFIL);
			break;

		case ID_SELFSTCHS:

			selalfil();
			break;

		case ID_SETMRK:

			setmov();
			break;

		case ID_DELFRE:

			delfre();
			break;

		case ID_SELAL:

			selal();
			break;

		case ID_REFILAL:

			refilal();
			break;

		case ID_CHK:

			chkrng(&rngsiz);
			setMap(RESTCH);
			break;

		case ID_RTRVCLP:

			rtrclp();
			break;

		case ID_SORT:

			fsort();
			break;

		case ID_LAYMOV0:

			movlayr(0);
			break;

		case ID_LAYMOV1:

			movlayr(2);
			break;

		case ID_LAYMOV2:

			movlayr(4);
			break;

		case ID_LAYMOV3:

			movlayr(6);
			break;

		case ID_LAYMOV4:

			movlayr(8);
			break;

		case ID_LAYCPY0:

			cpylayr(0);
			break;

		case ID_LAYCPY1:

			cpylayr(2);
			break;

		case ID_LAYCPY2:

			cpylayr(4);
			break;

		case ID_LAYCPY3:

			cpylayr(6);
			break;

		case ID_LAYCPY4:

			cpylayr(8);
			break;

		case ID_LA:

			nulayr(0);
			break;

		case ID_L1:

			nulayr(1);
			break;

		case ID_L2:

			nulayr(2);
			break;

		case ID_L3:

			nulayr(3);
			break;

		case ID_L4:

			nulayr(4);
			break;

		case ID_ROTDUP:

			rotdup();
			break;

		case ID_ROTAGAIN:

			rotagain();
			break;

		case ID_ROTCMD:

			rotcmd();
			break;

		case ID_DELFRMS:

			delfrms();
			rstAll();
			setMap(RESTCH);
			break;

		case ID_SNAP2:

			snap();
			break;

		case ID_CLPFIL:

			clpfil();
			break;

		case ID_FLPORD:

			flpord();
			break;

		case ID_FRMOF:

			if (GetKeyState(VK_SHIFT) & 0X8000)
				tglhid();
			else
				tglfrm();
			break;

		case ID_MV2BAK:

			mv2b();
			break;

		case ID_MV2FRNT:

			mv2f();
			break;

		case ID_PERP:

			if (chkMap(FORMSEL) || selectedFormCount)
				savdo();
			prpbrd(StitchSpace);
			break;

		case ID_PURGDIR:

			purgdir();
			break;

		case ID_PURG:

			purg();
			break;

		case ID_VUBAK:

			vubak();
			break;

		case ID_DELETE:

			delet();
			break;

		case ID_FLIPH:

			fliph();
			break;

		case ID_FLIPV:

			flipv();
			break;

		case ID_FILANG:

			if (chkMap(FORMSEL))
				savdo();
			filangl();
			break;

		case ID_PREF:

			prfmsg();
			break;

		case ID_BOLD:

			if (chkMap(FORMSEL) || selectedFormCount)
				savdo();
			dubold();
			break;

		case ID_ADEND:

			stch2px1(hed.stitchCount - 1);
			endpnt();
			setMap(BAKEND);
			setMap(RESTCH);
			break;

		case ID_SETAP:

			setap();
			break;

		case ID_APLIQ:

			if (chkMap(FORMSEL))
				savdo();
			apliq();
			break;

		case ID_SATBRD:

			if (chkMap(FORMSEL))
				savdo();
			satbrd();
			break;

		case ID_FILCLP:

			fclp();
			break;

		case ID_FILIN:

			if (chkMap(FORMSEL))
				savdo();
			bord();
			break;

		case ID_FRM0:

			frm0();
			break;

		case ID_REDO:

			redo();
			break;

		case ID_UNFIL:

			savdo();
			unfil();
			coltab();
			setMap(RESTCH);
			break;

		case ID_FORM:

			frmon();
			if (chkMap(FORMSEL))
				refrm();
			else
				form();
			break;

		case ID_FILSAT:

			rstMap(FTHR);
			filsat();
			break;

		case ID_OPNPCD:

			switch (ini.auxFileType) {

			case AUXDST:

				ofn.nFilterIndex = 3;
				break;

			default:

				ofn.nFilterIndex = 2;
			}
			nuFil();
			nulayr(0);
			break;

		case ID_DELSTCH:

			delstch();
			break;

		case ID_FILL_VERT:

			if (chkMap(FORMSEL) || selectedFormCount)
				savdo();
			filvrt();
			break;

		case ID_FILL_HOR:

			if (chkMap(FORMSEL))
				savdo();
			filhor();
			break;

		case ID_RUNPAT:

			movi();
			break;

		case ID_LENDEF:

			ini.maxStitchLength = MAXSIZ*PFGRAN;
			userStitchLength = USESIZ*PFGRAN;
			minStitchLength = MINSIZ*PFGRAN;
			break;

		case ID_TSIZDEF:

			threadSize30 = TSIZ30;
			threadSize40 = TSIZ40;
			threadSize60 = TSIZ60;
			break;

		case ID_SIZ30:

			tsizmsg("30", threadSize30);
			setMap(ENTR30);
			break;

		case ID_SIZ40:

			tsizmsg("40", threadSize40);
			setMap(ENTR40);
			break;

		case ID_SIZ60:

			tsizmsg("60", threadSize60);
			setMap(ENTR60);
			break;

		case ID_HIDBITF:
		case ID_HIDBIT:

			hidbit();
			break;

		case ID_LODBIT:

			lodbmp();
			break;

		case ID_KNOTS:

			setknots();
			break;

		case ID_REMZERO:

			duzero();
			break;

		case ID_ROT:

			rot();
			break;

		case ZUMIN:

			if (chkMap(GMRK) || chkMap(SELBOX) || chkMap(INSRT) || chkMap(GRPSEL) || chkMap(FORMSEL))
				zumin();
			else {

				rstMap(BZUM);
				setMap(BOXZUM);
				rstMap(BZUMIN);
				setMap(VCAPT);
				SetCapture(hWnd);
			}
			break;

		case ID_ZUMOUT:

			zumout();
			break;

		case ID_FILE_OPEN1:

			fop();
			break;

		case ID_VIEW_STCHBAK:

			if (nuBak()) {

				backgroundColor = bCol.rgbResult;
				backgroundPen = nuPen(backgroundPen, 1, backgroundColor);
				backgroundPenWidth = 1;
				DeleteObject(hBackgroundBrush);
				hBackgroundBrush = CreateSolidBrush(backgroundColor);
				if (*bnam)
					bfil();
				zumhom();
			}
			break;

		case ID_BITCOL:

			if (nuBit()) {

				bitmapColor = fswap(btCol.rgbResult);
				if (*bnam)
					bfil();
				bitmapPen = nuPen(bitmapPen, 1, bitmapColor);
				zumhom();
			}
			break;

		case ID_FILE_SAVE3:

			colchk();
			savAs();
			break;

		case ID_EDIT_RESET_COL:

			for (ind = 0; ind < 16; ind++) {

				useCol[ind] = defCol[ind];
				hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], useCol[ind]);
				userPen[ind] = nuPen(userPen[ind], 1, useCol[ind]);
				redraw(hCol[ind]);
			}
			setMap(RESTCH);
			break;

		case ID_FILE_SAVE2:

			colchk();
			save();
			break;

		case VU_ZUMFUL:

			zumhom();
			break;

		case ID_EDIT_SELECTCOLOR:

			selCol();
			break;

		case ID_FILE_NEW1:

			if (!savcmp()) {

				savdisc();
				setMap(NEWBAK);
				rstMap(PRFACT);
				undat();
			} else {

				newFil();
				nulayr(0);
			}
			break;

		case ID_BACK:

			bak();
			break;

		case ID_DESNAM:

			getdes();
			break;
		}
		break;
	}
	return 0;
}

//return the width of a text item
int txtWid(TCHAR* str) {

	GetTextExtentPoint32(mainDC, str, strlen(str), &textSize);
	return textSize.cx;
}

void makCol() {

	unsigned	ind;
	TCHAR		buf[3];

	buf[1] = '0';
	buf[2] = 0;
	for (ind = 0; ind < 16; ind++) {

		hDef[ind] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			0,
			buttonHi*ind,
			buttonWid,
			buttonHi,
			hWnd,
			NULL,
			hInst,
			NULL);

		hCol[ind] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonWid,
			buttonHi*ind,
			buttonWid,
			buttonHi,
			hWnd,
			NULL,
			hInst,
			NULL);

		buf[0] = thrdSiz[ind][0];
		hSiz[ind] = CreateWindow(
			"STATIC",
			buf,
			SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonWid << 1,
			buttonHi*ind,
			buttonWid,
			buttonHi,
			hWnd,
			NULL,
			hInst,
			NULL);
	}
}

void int2tim(ULARGE_INTEGER num, FILETIME* tim) {

	tim->dwHighDateTime = num.HighPart;
	tim->dwLowDateTime = num.LowPart;
}

void ritloc() {

	TCHAR*			penv;
	TCHAR			locnam[_MAX_PATH] = { 0 };
	HANDLE			hloc;
	DWORD			rot;
	size_t			len;
	errno_t			err;
	err = _dupenv_s(&penv, &len, "COMSPEC");

	if (err) {
		free(penv);
		return;
	} else {
		if (penv) {
			strcpy_s(locnam, penv);
			free(penv);
		}
	}
	penv = strrchr(locnam, '\\') + 1;
	strcpy_s(penv, sizeof(locnam) - (penv - locnam), "thredloc.txt");
	hloc = CreateFile(locnam, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hloc != INVALID_HANDLE_VALUE) {
		WriteFile(hloc, (TCHAR*)homdir, strlen(homdir) + 1, &rot, 0);
		CloseHandle(hloc);
	}
}

#if __UseASM__ == 0
static inline void delsubl(unsigned *&dst,
	unsigned val,
	unsigned cnt) {
	for (unsigned i = 0; i < cnt; i++) {
		*(dst++) = _byteswap_ulong(val);

		val <<= 1;
	}
}
static inline void delsubr(unsigned *&dst,
	unsigned val,
	unsigned cnt) {
	for (unsigned i = 0; i < cnt; i++) {
		*(dst++) = _byteswap_ulong(val);

		val >>= 1;
	}
}

static inline void delsubt(unsigned *&dst,
	unsigned *src,
	unsigned cnt) {

	for (unsigned i = 0; i < cnt; i++) {
		*(dst++) = _byteswap_ulong(*(src++));
	}
}
#endif

void ducurs(unsigned char* pnt) {

#if  __UseASM__
	_asm {

		jmp		short delups


		delsubl : bswap	eax
				  mov[edi], eax
				  add		edi, 4
				  bswap	eax
				  shl		eax, 1
				  loop	delsubl
				  ret

				  delsubr : bswap	eax
							mov[edi], eax
							add		edi, 4
							bswap	eax
							shr		eax, 1
							loop	delsubr
							ret

							delsubt : mov		eax, [esi]
									  add		esi, 4
									  bswap	eax
									  mov[edi], eax
									  add		edi, 4
									  loop	delsubt
									  ret

									  //form cursor
									  delups : mov		ebx, pnt
											   mov		edi, ebx
											   xor		ecx, ecx
											   mov		cl, 32
											   mov		eax, 0x00000100
											   rep		stosd
											   xor		eax, eax
											   dec		eax
											   mov		edi, ebx
											   add		edi, 64
											   mov[edi], eax
											   mov		eax, 0x00ffff00
											   add		edi, 32
											   mov[edi], eax
											   sub		edi, 64
											   mov[edi], eax
											   mov		eax, 0x00018000
											   mov		cl, 16
											   dulup0: or [edi], eax
													   add		edi, 4
													   loop	dulup0

													   //dline cursor

													   add		ebx, 128
													   mov		edi, ebx
													   mov		ecx, 32
													   xor edx, edx
													   inc		edx
													   mov		esi, edx
													   ror		esi, 1
													   dulup:		mov		eax, edx
																	or eax, esi
																	or eax, 0x80000001
																	bswap	eax
																	mov[edi], eax
																	add		edi, 4
																	shl		edx, 1
																	shr		esi, 1
																	loop	dulup
																	xor		eax, eax
																	dec		eax
																	sub		edi, 4
																	mov[edi], eax
																	mov[ebx], eax

																	//straight up needle cursor
																	add		ebx, 128
																	mov		edi, ebx
																	mov		eax, 0x0003c000
																	bswap	eax
																	mov		cl, 24
																	rep		stosd
																	mov		eax, 0x00024000
																	bswap	eax
																	mov		cl, 5
																	rep		stosd
																	mov		eax, 0x00018000
																	bswap	eax
																	mov		cl, 2
																	rep		stosd

																	//left up
																	add		ebx, 128
																	mov		edi, ebx
																	mov		cl, 5
																	mov		esi, offset lucurstrt
																	call	delsubt
																	mov		cl, 4
																	mov		eax, 0x88
																	call	delsubl
																	mov		cl, 19
																	mov		eax, 0x1f00
																	call	delsubl
																	mov		esi, offset lucurfin
																	mov		cl, 3
																	call	delsubt

																	//left down

																	add		ebx, 128
																	mov		edi, ebx
																	mov		esi, offset ldcurstrt
																	mov		cl, 4
																	call	delsubt
																	mov		cl, 19
																	mov		eax, 0x7c000000
																	call	delsubr
																	mov		cl, 4
																	mov		eax, 0x440
																	call	delsubr
																	mov		cl, 5
																	mov		esi, offset ldcurfin
																	call	delsubt

																	//right up

																	add		ebx, 128
																	mov		edi, ebx
																	mov		cl, 5
																	mov		esi, offset rucurstrt
																	call	delsubt
																	mov		eax, 0x9000000
																	mov		cl, 4
																	call	delsubr
																	mov		cl, 19
																	mov		eax, 0x1f00000
																	call	delsubr
																	mov		esi, offset rucurfin
																	mov		cl, 3
																	call	delsubt

																	//right down
																	add		ebx, 128
																	mov		edi, ebx
																	mov		esi, offset rdcurstrt
																	mov		cl, 4
																	call	delsubt
																	mov		eax, 0xf8
																	mov		cl, 19
																	call	delsubl
																	mov		eax, 0x8400000
																	mov		cl, 4
																	call	delsubl
																	mov		esi, offset rdcurfin
																	mov		cl, 5
																	call	delsubt

	}
#else
	unsigned *zPnt = (unsigned *)pnt;
	unsigned *dst = zPnt;
	for (int i = 0; i < 32; i++) {
		*(dst++) = 0x100;
	}


	dst = zPnt + 16;

	*dst = 0xffffffff;

	dst += 8;

	*dst = 0x00ffff00;

	dst -= 16;

	*dst = 0x00ffff00;

	for (int i = 0; i < 16; i++) {
		*(dst++) |= 0x00018000;
	}

	zPnt += 32;
	dst = zPnt;

	unsigned ltor = 0x80000000;
	unsigned rtol = 0x00000001;

	for (int i = 0; i < 32; i++) {
		*(dst++) = _byteswap_ulong(ltor | rtol | 0x80000001);

		rtol <<= 1;
		ltor >>= 1;
	}

	*(--dst) = 0xffffffff;
	*zPnt = 0xffffffff;

	zPnt += 32;
	dst = zPnt;

	for (int i = 0; i < 24; i++) {
		*(dst++) = _byteswap_ulong(0x0003c000);
	}

	for (int i = 0; i < 5; i++) {
		*(dst++) = _byteswap_ulong(0x00024000);
	}

	for (int i = 0; i < 2; i++) {
		*(dst++) = _byteswap_ulong(0x00018000);
	}


	zPnt += 32;
	dst = zPnt;

	delsubt(dst, lucurstrt, 5);

	delsubl(dst, 0x88, 4);
	delsubl(dst, 0x1f00, 19);

	delsubt(dst, lucurfin, 3);

	zPnt += 32;
	dst = zPnt;

	delsubt(dst, ldcurstrt, 4);

	delsubr(dst, 0x7c000000, 19);
	delsubr(dst, 0x440, 4);

	delsubt(dst, ldcurfin, 5);

	zPnt += 32;
	dst = zPnt;

	delsubt(dst, rucurstrt, 5);

	delsubr(dst, 0x9000000, 4);
	delsubr(dst, 0x1f00000, 19);

	delsubt(dst, rucurfin, 3);

	zPnt += 32;
	dst = zPnt;

	delsubt(dst, rdcurstrt, 4);

	delsubl(dst, 0xf8, 19);
	delsubl(dst, 0x8400000, 4);

	delsubt(dst, rdcurfin, 5);
#endif
}

void duamsk() {

#if  __UseASM__
	_asm {

		xor		eax, eax
		dec		eax
		xor		ecx, ecx
		mov		cl, 32
		mov		edi, offset cursorMask
		rep		stosd
	}
#else
	memset(cursorMask, 0xff, 128);
#endif
}

void crtcurs() {

	duamsk();
	ducurs(cursorMasks.form);
	hFormCurs = CreateCursor(hInst, 16, 16, 32, 32, (void*)cursorMask, (void*)&cursorMasks.form);
	hDlinCurs = CreateCursor(hInst, 16, 16, 32, 32, (void*)cursorMask, (void*)&cursorMasks.dline);
	hNeedleUpCurs = CreateCursor(hInst, 16, 32, 32, 32, (void*)cursorMask, (void*)&cursorMasks.uprightNeedle);
	hNeedleRightDownCurs = CreateCursor(hInst, 1, 31, 32, 32, (void*)cursorMask, (void*)&cursorMasks.rightDownNeedle);
	hNeedleRightUpCurs = CreateCursor(hInst, 1, 1, 32, 32, (void*)cursorMask, (void*)&cursorMasks.rightUpNeedle);
	hNeedleLeftDownCurs = CreateCursor(hInst, 30, 30, 32, 32, (void*)cursorMask, (void*)&cursorMasks.leftDownNeedle);
	hNeedleLeftUpCurs = CreateCursor(hInst, 32, 1, 32, 32, (void*)cursorMask, (void*)&cursorMasks.leftUpNeedle);
}

void dstcurs() {

	DestroyCursor(hFormCurs);
	DestroyCursor(hDlinCurs);
	DestroyCursor(hNeedleUpCurs);
	DestroyCursor(hNeedleRightDownCurs);
	DestroyCursor(hNeedleRightUpCurs);
	DestroyCursor(hNeedleLeftDownCurs);
	DestroyCursor(hNeedleLeftUpCurs);
}

void duhom() {

	unsigned ind;

	strcpy_s(homdir, __argv[0]);
	phom = strrchr(homdir, '\\');
	if (phom)
		phom++;
	else {

		ind = GetCurrentDirectory(_MAX_PATH, homdir);
		if (ind) {
			homdir[ind++] = '\\';
			phom = &homdir[ind];
		}
	}
	if (phom) { *phom = 0; }
}

#if  __UseASM__
void bcpy(TCHAR* dst, TCHAR* src) {

	_asm {

		mov		ebx, src
		mov		edx, dst
		xor		eax, eax
		blup : mov		al, [ebx]
			   mov[edx], al
			   or eax, eax
			   je		short blupx
			   inc		ebx
			   inc		edx
			   jmp		blup
			   blupx :
	}
	//#else
	//	strcpy(dst, src);
	//#endif
}
#endif

void ducmd() {

	unsigned long	l_red;
	int				ind;

	if (__argc > 1) {

#if  __UseASM__
		bcpy(filnam, __argv[1]);
#else
		strcpy_s(filnam, __argv[1]);
#endif
		if (!strncmp(filnam, "/F1:", 4)) {

			balpnt = &filnam[4];
			balfil = CreateFile(balpnt, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			CloseHandle(balfil);
			if (balfil != INVALID_HANDLE_VALUE) {

#if  __UseASM__
				bcpy(balnam0, balpnt);
#else
				strcpy_s(balnam0, balpnt);
#endif
				if (__argc > 2) {

#if  __UseASM__
					bcpy(filnam, __argv[2]);
#else
					strcpy_s(filnam, __argv[2]);
#endif
					if (!strncmp(filnam, "/F2:", 4)) {

						balpnt = &filnam[4];
						balfil = CreateFile(balpnt, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
						if (balfil != INVALID_HANDLE_VALUE) {

#if  __UseASM__
							bcpy(balnam1, balpnt);
#else
							strcpy_s(balnam1, balpnt);
#endif
							balpnt = (TCHAR*)&bseq;
							ReadFile(balfil, (void*)&balnam2, 10000, &l_red, 0);
							strcat_s(balnam2, "");
							if (l_red)
								redbal();
						}
					}
				}
				SetWindowText(hWnd, stab[STR_EMB]);
			}
			*filnam = 0;
			CloseHandle(balfil);
			DeleteFile(balnam1);
		} else {

			for (ind = 2; ind < __argc; ind++) {

				strcat_s(filnam, " ");
				strcat_s(filnam, __argv[ind]);
			}
			setMap(REDOLD);
			nuFil();
		}
	}
}

void redini() {

	unsigned		ind;
	unsigned long	wrot;
	HDC				tdc;

	for (ind = 0; ind < 16; ind++)
		undoBuffer[ind] = 0;
	for (ind = 0; ind < OLDNUM; ind++)
		ini.prevNames[ind][0] = 0;
	duhom();
	strcpy_s(iniNam, homdir);
	strcat_s(iniNam, "thred.ini");
	hIni = CreateFile(iniNam, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hIni == INVALID_HANDLE_VALUE)
		defpref();
	else {

		ReadFile(hIni, &ini, sizeof(ini), &wrot, 0);
		if (wrot < 2061)
			ini.formBoxSizePixels = DEFBPIX;
		strcpy_s(defDir, ini.defaultDirectory);
		strcpy_s(defbmp, ini.defaultDirectory);
		for (ind = 0; ind < 16; ind++) {

			useCol[ind] = ini.stitchColors[ind];
			custCol[ind] = ini.stitchPreferredColors[ind];
			bakCust[ind] = ini.backgroundPreferredColors[ind];
			bitmapBackgroundColors[ind] = ini.bitmapBackgroundColors[ind];
		}
		backgroundColor = ini.backgroundColor;
		bitmapColor = ini.bitmapColor;
		minStitchLength = ini.minStitchLength;
		showStitchThreshold = ini.showStitchThreshold;
		if (ini.threadSize30)
			threadSize30 = ini.threadSize30;
		if (ini.threadSize40)
			threadSize40 = ini.threadSize40;
		if (ini.threadSize60)
			threadSize60 = ini.threadSize60;
		if (ini.userStitchLength)
			userStitchLength = ini.userStitchLength;
		if (!ini.maxStitchLength)
			ini.maxStitchLength = MAXSIZ*PFGRAN;
		if (ini.smallStitchLength)
			smallStitchLength = ini.smallStitchLength;
		StitchBoxesThreshold = ini.StitchBoxesThreshold;
		if (ini.StitchSpace)
			StitchSpace = ini.StitchSpace;
		binaryVariableBitmap = ini.binaryVariableBitmap;
		if (ini.borderWidth)
			borderWidth = ini.borderWidth;
		if (ini.underlayColor)
			underlayColor = ini.underlayColor & 0xf;
		if (ini.snapLength)
			snapLength = ini.snapLength;
		if (ini.starRatio)
			starRatio = ini.starRatio;
		if (ini.spiralWrap)
			spiralWrap = ini.spiralWrap;
		if (ini.buttonholeFillCornerLength)
			buttonholeFillCornerLength = ini.buttonholeFillCornerLength;
		if (!ini.gridSize)
			ini.gridSize = 12;
		if (!ini.wavePoints)
			ini.wavePoints = IWAVPNTS;
		if (!ini.waveStart)
			ini.waveStart = IWAVSTRT;
		if (!ini.waveEnd)
			ini.waveEnd = IWAVEND;
		if (!ini.waveLobes)
			ini.waveLobes = IWAVS;
		if (!ini.featherFillType)
			ini.featherFillType = FDEFTYP;
		if (!ini.featherUpCount)
			ini.featherUpCount = FDEFUP;
		if (!ini.featherDownCount)
			ini.featherDownCount = FDEFDWN;
		if (!ini.featherRatio)
			ini.featherRatio = (float)FDEFRAT;
		if (!ini.featherMinStitchSize)
			ini.featherMinStitchSize = FDEFFLR;
		if (!ini.featherCount)
			ini.featherCount = FDEFNUM;
		if (!ini.daisyHoleDiameter)
			ini.daisyHoleDiameter = DAZHLEN;
		if (!ini.daisyPetalPoints)
			ini.daisyPetalPoints = DAZCNT;
		if (!ini.daisyInnerCount)
			ini.daisyInnerCount = DAZICNT;
		if (!ini.daisyDiameter)
			ini.daisyDiameter = DAZLEN;
		if (!ini.daisyPetalCount)
			ini.daisyPetalCount = DAZPETS;
		if (!ini.daisyPetalLen)
			ini.daisyPetalLen = DAZPLEN;

		switch (ini.hoopType) {

		case SMALHUP:

			ini.hoopSizeX = SHUPX;
			ini.hoopSizeY = SHUPY;
			break;

		case LARGHUP:

			ini.hoopSizeX = LHUPX;
			ini.hoopSizeY = LHUPY;
			break;

		case CUSTHUP:

			if (!ini.hoopSizeX)
				ini.hoopSizeX = LHUPX;
			if (!ini.hoopSizeY)
				ini.hoopSizeY = LHUPY;
			break;

		case HUP100:

			if (!ini.hoopSizeX)
				ini.hoopSizeX = HUP100XY;
			if (!ini.hoopSizeY)
				ini.hoopSizeY = HUP100XY;
			break;

		default:

			ini.hoopType = LARGHUP;
			ini.hoopSizeX = LHUPX;
			ini.hoopSizeY = LHUPY;
		}
		unzoomedRect.x = ini.hoopSizeX;
		unzoomedRect.y = ini.hoopSizeY;
		picotSpace = ini.picotSpace;
	}
	if (!ini.gridColor)
		ini.gridColor = DEFGRD;
	CloseHandle(hIni);
	if (!ini.fillAngle)
		ini.fillAngle = PI / 6;
	tdc = GetDC(0);
	screenSizePixels.cx = GetDeviceCaps(tdc, HORZRES);
	screenSizePixels.cy = GetDeviceCaps(tdc, VERTRES);
	if (ini.initialWindowCoords.left < 0)
		ini.initialWindowCoords.left = 0;
	if (ini.initialWindowCoords.top < 0)
		ini.initialWindowCoords.top = 0;
	ind = screenSizePixels.cx - 30;
	if (ini.initialWindowCoords.right > (int)ind)
		ini.initialWindowCoords.right = ind;
	if (ini.initialWindowCoords.bottom > screenSizePixels.cy)
		ini.initialWindowCoords.bottom = screenSizePixels.cy;
}

void trcsub(HWND* hwnd, unsigned xloc, unsigned yloc, unsigned hi) {

	*hwnd = CreateWindow(
		"STATIC",
		"",
		SS_OWNERDRAW | WS_CHILD | WS_BORDER,
		xloc,
		yloc,
		buttonWid,
		hi,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void chkirct() {

	POINT lim;

	lim.x = screenSizePixels.cx - 1;
	lim.y = screenSizePixels.cy - 1;
	if (ini.initialWindowCoords.left > lim.x)
		ini.initialWindowCoords.left = lim.x;
	if (ini.initialWindowCoords.right > lim.x)
		ini.initialWindowCoords.right = lim.x;
	if (ini.initialWindowCoords.left < 0)
		ini.initialWindowCoords.left = 0;
	if (ini.initialWindowCoords.right < 0)
		ini.initialWindowCoords.right = 0;
	if (ini.initialWindowCoords.top > lim.y)
		ini.initialWindowCoords.top = lim.y;
	if (ini.initialWindowCoords.bottom > lim.y)
		ini.initialWindowCoords.bottom = lim.y;
	if (ini.initialWindowCoords.top < 0)
		ini.initialWindowCoords.top = 0;
	if (ini.initialWindowCoords.bottom < 0)
		ini.initialWindowCoords.bottom = 0;
	if (ini.initialWindowCoords.right - ini.initialWindowCoords.left < 300) {

		ini.initialWindowCoords.left = 0;
		ini.initialWindowCoords.right = lim.x >> 1;
	}
	if (ini.initialWindowCoords.bottom - ini.initialWindowCoords.top < 300) {

		ini.initialWindowCoords.top = 0;
		ini.initialWindowCoords.bottom = lim.y >> 1;
	}
}

void init() {

	unsigned		ind, flg;
	unsigned long	thwid, mwid;
	RECT			tRct;
	RECT			wrct;
	HDC				totdc;
	TCHAR*			pchr;

	txad = 0;
	fsizeof = sizeof(FRMHED) >> 2;
	LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
	hMen = GetMenu(hWnd);
	totdc = GetDC(NULL);
	mwid = GetDeviceCaps(totdc, HORZRES);
	ReleaseDC(NULL, totdc);
	GetWindowRect(hWnd, &wrct);
	GetMenuItemRect(hWnd, hMen, 0, &tRct);
	//menhi = tRct.bottom - tRct.top;
	wrct.left = tRct.left;
	wrct.right = tRct.right;
	for (ind = 0; ind <= M_HELP; ind++) {

		GetMenuItemRect(hWnd, hMen, ind, &tRct);
		wrct.right += (tRct.right - tRct.left);
	}
	wrct.right += 20;
	thwid = wrct.right - wrct.left;
	mwid >>= 1;
	if (thwid < mwid)
		wrct.right = wrct.left += mwid;
	hfilMen = GetSubMenu(hMen, M_FIL);
	hfileMen = GetSubMenu(hMen, M_FILE);
	hedmen = GetSubMenu(hMen, M_EDIT);
	hfilBrdMen = GetSubMenu(hfilMen, MFIL_BORD);
	hvumen = GetSubMenu(hMen, M_VIEW);
	hvsmen = GetSubMenu(hvumen, M_VIEW);
	qchk();
	hArrowCurs = LoadCursor(0, IDC_ARROW);
	hCrossCurs = LoadCursor(0, IDC_CROSS);
	crtcurs();
	redfils();
	clrMap(MAPLEN);						//clear the bitmap
	//set up the size variables
	mainDC = GetDC(hWnd);
	SetStretchBltMode(mainDC, COLORONCOLOR);
	StitchWindowDC = GetDCEx(hStch, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
	screenSize_mm.cx = GetDeviceCaps(mainDC, HORZSIZE);
	screenSize_mm.cy = GetDeviceCaps(mainDC, VERTSIZE);
	chkirct();
	if (!chku(SAVMAX))
		MoveWindow(hWnd, ini.initialWindowCoords.left, ini.initialWindowCoords.top, ini.initialWindowCoords.right - ini.initialWindowCoords.left, ini.initialWindowCoords.bottom - ini.initialWindowCoords.top, 0);
	buttonWid = txtWid("MM") + TXTSIDS;
	buttonWid3 = buttonWid * 3;
	buttonHi = textSize.cy + 4;
	numwid = txtWid("0");
	selbox = txtWid("0");
	for (ind = 0; ind < NERCNT; ind++)
		boxOff[ind] = selbox + selbox*ind;
	GetClientRect(hWnd, &mRct);
	stchWnd();
	lodstr();
	maxwid(STR_PRF0, STR_PRF27);
	if (!hIni) {

		//initialize the user color and thread size arrays
		for (ind = 0; ind < 16; ind++)
			useCol[ind] = defCol[ind];
	}
	for (ind = 0; ind < 16; ind++) {

		thrdSiz[ind][0] = '4';
		thrdSiz[ind][1] = '0';
	}
	if (!ini.traceLength)
		ini.traceLength = TRACLEN;
	if (!ini.traceRatio)
		ini.traceRatio = TRACRAT;
	if (!ini.chainSpace)
		ini.chainSpace = CHSDEF;
	if (!ini.chainRatio)
		ini.chainRatio = CHRDEF;
	if (!ini.cursorNudgeStep)
		ini.cursorNudgeStep = NUGINI;
	if (!ini.nudgePixels)
		ini.nudgePixels = DEFPIX;
	if (!ini.eggRatio)
		ini.eggRatio = DEFEGRAT;
	if (!ini.stitchSizePixels)
		ini.stitchSizePixels = DEFPNTPIX;
	if (!ini.formPointSizePixels)
		ini.formPointSizePixels = DEFPNTPIX;
	if (!ini.formSides)
		ini.formSides = 24;
	if (!ini.tearTailLength)
		ini.tearTailLength = (float)1.4;
	if (!ini.underlayStitchLen)
		ini.underlayStitchLen = DEFULEN;
	if (!ini.underlaySpacing)
		ini.underlaySpacing = DEFUSPAC;
	setgrd(ini.gridColor);
	makCol();		//make the color change windows
	for (ind = 0; ind < 9; ind++) {

		switch (ind) {

		case HBOXSEL:

			flg = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			pchr = stab[STR_BOXSEL];
			break;

		case HUPTO:

			flg = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			pchr = stab[STR_UPOF];
			break;

		case HHID:

			flg = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER;
			pchr = stab[STR_PIKOL];
			break;

		default:

			pchr = "";
			flg = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		}
		hbuts[ind] = CreateWindow(
			"STATIC",
			pchr,
			flg,
			0,
			buttonHi*(16 + ind),
			buttonWid3,
			buttonHi,
			hWnd,
			NULL,
			hInst,
			NULL);
	}
	hstep = CreateWindow(

		"STATIC",
		"",
		SS_NOTIFY | SS_CENTER | WS_CHILD | WS_BORDER,
		0,
		buttonHi * 18,
		buttonWid3,
		buttonHi,
		hWnd,
		NULL,
		hInst,
		NULL);

	hBar = CreateWindow(
		"STATIC",
		"",
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		mRct.right - COLSIZ,
		0,
		COLSIZ,
		mRct.bottom,
		hWnd,
		NULL,
		hInst,
		NULL);

	for (ind = 0; ind < 3; ind++) {

		trcsub(&hctrc[ind], buttonWid*ind, 0, buttonHi * 15);
		trcsub(&hstrc[ind], buttonWid*ind, buttonHi * 15, buttonHi);
		trcsub(&htrup[ind], buttonWid*ind, buttonHi * 16, buttonHi);
		trcsub(&htrdwn[ind], buttonWid*ind, buttonHi * 17, buttonHi);
		trbrsh[ind] = CreateSolidBrush(traceRGB[ind]);
	}

	nuRct();
	//create pens
	for (ind = 0; ind < 4; ind++)
		boxPen[ind] = CreatePen(PS_SOLID, 1, boxRef[ind]);
	linePen = CreatePen(PS_SOLID, 1, 0x404040);
	crossPen = CreatePen(PS_SOLID, 5, 0x804080);
	groupSelectPen = CreatePen(PS_SOLID, 1, 0x804080);
	gridPen = CreatePen(PS_SOLID, 1, ini.gridColor);
	backgroundPen = CreatePen(PS_SOLID, 3, backgroundColor);
	bitmapPen = CreatePen(PS_SOLID, 1, bitmapColor);
	formPen = CreatePen(PS_SOLID, 1, 0xc0c0c0);
	multiFormPen = CreatePen(PS_SOLID, 1, 0xc0c080);
	formPen3px = CreatePen(PS_SOLID, 3, 0xc0c0c0);
	formSelectedPen = CreatePen(PS_SOLID, 1, 0x80c080);
	zoomMarkPen = CreatePen(PS_SOLID, 3, 0x40c040);
	selectAllPen = CreatePen(PS_SOLID, 1, 0xc08080);
	knotPen = CreatePen(PS_SOLID, 1, 0xffffff);
	layerPen[0] = CreatePen(PS_SOLID, 1, 0xc0c0c0);
	layerPen[1] = CreatePen(PS_SOLID, 1, 0x80c0c0);
	layerPen[2] = CreatePen(PS_SOLID, 1, 0xc080c0);
	layerPen[3] = CreatePen(PS_SOLID, 1, 0xc0c080);
	layerPen[4] = CreatePen(PS_SOLID, 1, 0x40c0c0);
	layerPen[5] = CreatePen(PS_SOLID, 1, 0xc0c040);
	backgroundPenWidth = 1;
	for (ind = 0; ind < 16; ind++) {

		pixSiz[ind] = 1;
		sizInd[ind] = 1;
		userPen[ind] = CreatePen(PS_SOLID, 1, useCol[ind]);
	}
	hBackgroundBrush = CreateSolidBrush(backgroundColor);
	//create brushes
	for (ind = 0; ind < 16; ind++) {

		hDefaultColorBrush[ind] = CreateSolidBrush(defCol[ind]);
		hUserColorBrush[ind] = CreateSolidBrush(useCol[ind]);
	}
	hblk = CreateSolidBrush(0);
	zoomFactor = 1;
	hed.leadIn = 0x32;
	hed.colorCount = 16;
	hed.stitchCount = 0;
	GetDCOrgEx(StitchWindowDC, &stitchWindowOrigin);
	ladj();
	GetTextExtentPoint(StitchWindowMemDC, stab[STR_PIKOL], strlen(stab[STR_PIKOL]), &pcolsiz);
	auxmen();
	fnamtabs();
	ritfnam(ini.designerName);
	strcpy_s(hedx.modifierName, ini.designerName);
	hedx.stgran = 0;
	for (ind = 0; ind < 26; ind++)
		hedx.res[ind] = 0;
	chkhup();
	nedmen();
	fil2men();
	knotmen();
	bsavmen();
	rotauxmen();
	linbmen();
	wrnmen();
	ritloc();
	frmcurmen();
	redtx();
	setMap(HIDMAP);
	hidbit();
	chkmen();
	//check command line-should be last item in init
	ducmd();
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], ini.designerName);
	SetWindowText(hWnd, msgbuf);
}

COLORREF defTxt(unsigned colInd) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, defMap
		mov		edx, colInd
		bt		ecx, edx
		jnc		short defx
		mov		eax, 0xffffff
		defx:
	}
#else
	return _bittest((long *)&defMap, colInd) ? 0xffffff : 0;
#endif
}

void setCol(unsigned ind) {

#if  __UseASM__
	_asm {

		mov		ebx, ind
		mov		eax, cbit
		bts		eax, ebx
		mov		cbit, eax
	}
#else
	// mask ind for sanity
	_bittestandset((long *)&cbit, ind & 31);
#endif
}

void relin() {

	stch2px1(closestPointIndex - 1);
	mlin0[0].x = stitchSizePixels.x;
	mlin0[0].y = stitchSizePixels.y;
	stch2px1(closestPointIndex + 1);
	mlin1[1].x = stitchSizePixels.x;
	mlin1[1].y = stitchSizePixels.y;
	rstMap(WASLIN);
	dulin();
}

unsigned setRmp(unsigned pbit) {

#if  __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, offset markedStitchMap
		mov		ecx, pbit
		bts[ebx], ecx
		jc		short setrm
		dec		eax
		setrm :
	}
#else
	return _bittestandset((long *)markedStitchMap, pbit) ? 0 : 0xffffffff;
#endif
}

void drwLin(unsigned ind, unsigned len, HPEN hPen) {

	unsigned	ine, layr;
	fPOINTATTRIBUTE*	l_pstch = &stitchBuffer[ind];

	if (activeLayer)
		lineIndex = 0;
	for (ine = 0; ine < len; ine++) {

		layr = (l_pstch[ine].attribute&LAYMSK) >> LAYSHFT;
		if (!activeLayer || !layr || (layr == activeLayer)) {

			linePoints[lineIndex].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x;
			linePoints[lineIndex++].y = stitchWindowClientRect.bottom - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y;
		}
	}
	SelectObject(StitchWindowMemDC, hPen);
	if (lineIndex < 16000)
		Polyline(StitchWindowMemDC, linePoints, lineIndex);
	else {

		ine = 0;
		while (lineIndex) {

			if (lineIndex > 16000) {

				Polyline(StitchWindowMemDC, &linePoints[ine], 16000);
				ine += 15999;
				lineIndex -= 15999;
			} else {

				Polyline(StitchWindowMemDC, &linePoints[ine], lineIndex);
				break;
			}
		}
	}
	lineIndex = 1;
	layr = (l_pstch[ine].attribute&LAYMSK) >> LAYSHFT;
	if (!activeLayer || !layr || layr == activeLayer) {

		linePoints[0].x = (l_pstch[ine - 1].x - zoomRect.left)*zoomRatio.x;
		linePoints[0].y = stitchWindowClientRect.bottom - (l_pstch[ine - 1].y - zoomRect.bottom)*zoomRatio.y;
	}
}

void dumov() {

	fPOINTATTRIBUTE*	tstch;
	POINT		arlin[8];
	POINT		trot;

	pixang = atan2(stitchBuffer[movpnt + 1].y - stitchBuffer[movpnt].y, stitchBuffer[movpnt + 1].x - stitchBuffer[movpnt].x);
	tstch = &stitchBuffer[movpnt];
	if (tstch->x >= zoomRect.left&&tstch->x <= zoomRect.right
		&&tstch->y >= zoomRect.bottom&&tstch->y <= zoomRect.top) {

		sdCor2px(stitchBuffer[movpnt], &pixcntr);
		arlin[0].x = arlin[6].x = pixcntr.x;
		arlin[0].y = arlin[6].y = pixcntr.y;
		trot.x = pixcntr.x + 12;
		trot.y = pixcntr.y + 2;
		rotpix(trot, &arlin[1]);
		trot.y = pixcntr.y - 2;
		rotpix(trot, &arlin[5]);
		trot.y = pixcntr.y + 6;
		rotpix(trot, &arlin[2]);
		trot.y = pixcntr.y - 6;
		rotpix(trot, &arlin[4]);
		trot.x = pixcntr.x + 20;
		trot.y = pixcntr.y;
		rotpix(trot, &arlin[3]);
		SelectObject(StitchWindowMemDC, formPen);
		SetROP2(StitchWindowMemDC, R2_XORPEN);
		Polyline(StitchWindowMemDC, arlin, 7);
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

unsigned chkup(unsigned cnt, unsigned ind) {

	if (chkMap(UPTO) && closestPointIndex) {

		if (colch[ind].stitchIndex < closestPointIndex) {

			if (colch[ind + 1].stitchIndex < closestPointIndex)
				return cnt;
			else
				return closestPointIndex - colch[ind].stitchIndex + 1;
		} else
			return 0;
	} else
		return cnt;
}

BOOL bitar() {

	dRECTANGLE	bakrct;
	dRECTANGLE	difrct;
	dPOINT	bsdrat;
	dRECTANGLE	zirct;

	zirct.bottom = unzoomedRect.y - zoomRect.bottom;
	zirct.top = unzoomedRect.y - zoomRect.top;
	zirct.left = zoomRect.left;
	zirct.right = zoomRect.right;
	if (zirct.top > bitmapSizeinStitches.y || zirct.left > bitmapSizeinStitches.x)
		return 0;
	else {

		bitmapSrcRect.top = ceil(zirct.top*bmpStitchRatio.y);
		bitmapSrcRect.left = ceil(zoomRect.left*bmpStitchRatio.x);
		bitmapSrcRect.right = floor(zoomRect.right*bmpStitchRatio.x);
		bitmapSrcRect.bottom = floor(zirct.bottom*bmpStitchRatio.y);
		if (bitmapSrcRect.right > (long)bitmapWidth) {

			bitmapSrcRect.right = bitmapWidth;
			rstMap(LANDSCAP);
		}
		if (bitmapSrcRect.bottom > (long)bitmapHeight) {

			bitmapSrcRect.bottom = bitmapHeight;
			setMap(LANDSCAP);
		}
		bakrct.top = bitmapSrcRect.top*stitchBmpRatio.y;
		bakrct.left = bitmapSrcRect.left*stitchBmpRatio.x;
		bakrct.right = bitmapSrcRect.right*stitchBmpRatio.x;
		bakrct.bottom = bitmapSrcRect.bottom*stitchBmpRatio.y;
		difrct.top = bakrct.top - zirct.top;
		difrct.left = bakrct.left - zirct.left;
		difrct.right = zirct.right - bakrct.right;
		difrct.bottom = zirct.bottom - bakrct.bottom;
		bsdrat.x = (double)stitchWindowClientRect.right / (zoomRect.right - zoomRect.left);
		bsdrat.y = (double)stitchWindowClientRect.bottom / (zoomRect.top - zoomRect.bottom);
		bitmapDstRect.top = difrct.top*bsdrat.y;
		bitmapDstRect.left = difrct.left*bsdrat.x;
		bitmapDstRect.right = stitchWindowClientRect.right - difrct.right*bsdrat.x;
		bitmapDstRect.bottom = stitchWindowClientRect.bottom - difrct.bottom*bsdrat.y;
		return 1;
	}
}

void drwknot() {

#define			KSIZ 5
#define			KLSIZ 10

	unsigned	ind;
	POINT		pnt;
	POINT		tlin[5];

	if (!chku(KNOTOF) && knotcnt&&hed.stitchCount) {

		for (ind = 0; ind < knotcnt; ind++) {

			stCor2px(stitchBuffer[knotab[ind]], &pnt);
			SelectObject(StitchWindowMemDC, knotPen);
			SetROP2(StitchWindowMemDC, R2_XORPEN);
			tlin[0].x = tlin[3].x = tlin[4].x = pnt.x - KSIZ;
			tlin[1].x = tlin[2].x = pnt.x + KSIZ;
			tlin[0].y = tlin[1].y = tlin[4].y = pnt.y + KSIZ;
			tlin[2].y = tlin[3].y = pnt.y - KSIZ;
			Polyline(StitchWindowMemDC, tlin, 5);
			tlin[0].x = pnt.x - KLSIZ;
			tlin[1].x = pnt.x + KLSIZ;
			tlin[0].y = tlin[1].y = pnt.y;
			Polyline(StitchWindowMemDC, tlin, 2);
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		}
	}
}

void dugrid() {

	POINT		tlin[2];
	RECT		grdrct;
	unsigned	ind;

	if (zoomFactor < showStitchThreshold || !showStitchThreshold) {

		SetROP2(StitchWindowMemDC, R2_XORPEN);
		SelectObject(StitchWindowMemDC, gridPen);
		grdrct.left = ceil(zoomRect.left / ini.gridSize);
		grdrct.right = floor(zoomRect.right / ini.gridSize);
		grdrct.bottom = ceil(zoomRect.bottom / ini.gridSize);
		grdrct.top = floor(zoomRect.top / ini.gridSize);
		tlin[0].x = 0;
		tlin[1].x = stitchWindowClientRect.right;
		for (ind = grdrct.bottom; ind <= (unsigned)grdrct.top; ind++) {

			tlin[0].y = tlin[1].y = stitchWindowClientRect.bottom - (ind*ini.gridSize - zoomRect.bottom)*zoomRatio.y + 0.5;
			Polyline(StitchWindowMemDC, tlin, 2);
		}
		tlin[0].y = 0;
		tlin[1].y = stitchWindowClientRect.bottom;
		for (ind = grdrct.left; ind <= (unsigned)grdrct.right; ind++) {

			tlin[0].x = tlin[1].x = (ind*ini.gridSize - zoomRect.left)*zoomRatio.x + 0.5;
			Polyline(StitchWindowMemDC, tlin, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void rint() {

	unsigned l_siz;

	rcel.x = (zoomRect.right - zoomRect.left) / stitchWindowClientRect.right;
	rcel.y = (zoomRect.top - zoomRect.bottom) / stitchWindowClientRect.bottom;
	l_siz = stitchWindowClientRect.right*stitchWindowClientRect.bottom;
	if (l_siz > RMAPBITS) {

		rcel.x *= l_siz / RMAPBITS;
		rcel.y *= l_siz / RMAPBITS;
	}
}

unsigned setRmap(fPOINTATTRIBUTE p_sPnt) {

	unsigned l_bpnt;

	l_bpnt = floor((p_sPnt.x - zoomRect.left) / rcel.x)*floor((p_sPnt.y - zoomRect.bottom) / rcel.y);
	if (l_bpnt < RMAPBITS)
		return setRmp(l_bpnt);
	return 0;
}

void drwStch() {

	unsigned	ind, ine, scnt, pwid, wascol;
	unsigned	layr;
	double		slop, off;
	double		dub6;
	fPOINTATTRIBUTE*	l_pstch;
	POINT		tlin[2];
	long		hi = ds->rcItem.bottom;
	long		xba, yab, tlng;
	HDC			tdc;

	setMap(RELAYR);
	rstMap(SELSHO);
	rstMap(ILIN1);
	rstMap(BZUM);
	unboxs();
	unrotu();
	unrot();
	rstMap(ILIN);
	uncros();
	rstMap(SHOFRM);
	scnt = 0;
	for (ind = 0; ind < colCnt; ind++) {

		lineIndex = colch[ind + 1].stitchIndex - colch[ind].stitchIndex;
		if (lineIndex > scnt)
			scnt = lineIndex;
	}
	linePoints = new POINT[scnt + 2];
	FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
	duzrat();
	if (*bnam && !chkMap(HIDMAP) && !chkMap(UPTO)) {

		if (chkMap(WASTRAC))
			tdc = tracedc;
		else
			tdc = bitmapDC;
		if (bitar())
			StretchBlt(StitchWindowMemDC, bitmapDstRect.left, bitmapDstRect.top, bitmapDstRect.right - bitmapDstRect.left, bitmapDstRect.bottom - bitmapDstRect.top,
				tdc, bitmapSrcRect.left, bitmapSrcRect.top, bitmapSrcRect.right - bitmapSrcRect.left, bitmapSrcRect.bottom - bitmapSrcRect.top, SRCCOPY);
	}
	dugrid();
	if (chkMap(INIT)) {

		if (chkMap(ZUMED)) {

			scrol.cbSize = sizeof(scrol);
			scrol.fMask = SIF_ALL;
			scrol.nMax = unzoomedRect.y;
			scrol.nMin = 0;
			scrol.nPage = zoomRect.top - zoomRect.bottom;
			scrol.nPos = unzoomedRect.y - zoomRect.top;
			SetScrollInfo(hVrt, SB_CTL, &scrol, TRUE);
			scrol.nMax = unzoomedRect.x;
			scrol.nPage = zoomRect.right - zoomRect.left;
			scrol.nPos = zoomRect.left;
			SetScrollInfo(hHor, SB_CTL, &scrol, TRUE);
			ShowWindow(hHor, TRUE);
			ShowWindow(hVrt, TRUE);
		} else {

			ShowWindow(hVrt, FALSE);
			ShowWindow(hHor, FALSE);
		}
		duzrat();
		dub6 = zoomRatio.x * 6;
		tPix[0] = dub6*TSIZ30;
		tPix[1] = dub6*TSIZ40;
		tPix[2] = dub6*TSIZ60;
		for (ind = 0; ind < 16; ind++) {

			if (chkMap(THRDS))
				nuStchSiz(ind, tPix[sizInd[ind]]);
			else {
				if (chkMap(COL)) {

					if (ind == actcol)
						nuStchSiz(ind, tPix[sizInd[ind]]);
					else
						nuStchSiz(ind, 1);
				} else
					nuStchSiz(ind, 1);
			}
		}
		lineIndex = 0; cbit = 0;
		if (chkMap(ZUMED)) {

			lineIndex = 0;
			rstMap(LINED);
			rstMap(LININ);
			for (ind = 0; ind < colCnt; ind++) {

				if (chkMap(HID)) {

					if (colch[ind].colorIndex != actcol) {

						scnt = colch[ind + 1].stitchIndex - colch[ind].stitchIndex;
						l_pstch = &stitchBuffer[colch[ind].stitchIndex];
						for (ine = 0; ine < scnt; ine++) {

							if (l_pstch[ine].x >= zoomRect.left
								&&l_pstch[ine].x <= zoomRect.right
								&&l_pstch[ine].y >= zoomRect.bottom
								&&l_pstch[ine].y <= zoomRect.top) {

								setCol(colch[ind].colorIndex);
								goto skip;
							}
						}
						goto skip;
					} else
						goto skip1;
				} else {
				skip1:;

					wascol = 0;
				}
				SelectObject(StitchWindowMemDC, userPen[colch[ind].colorIndex]);
				scnt = colch[ind + 1].stitchIndex - colch[ind].stitchIndex;
				l_pstch = &stitchBuffer[colch[ind].stitchIndex];
				scnt = chkup(scnt, ind);
				for (ine = 0; ine < scnt; ine++) {

					layr = (l_pstch[ine].attribute&LAYMSK) >> LAYSHFT;
					if (!activeLayer || !layr || (layr == activeLayer)) {

						if (l_pstch[ine].x >= zoomRect.left
							&&l_pstch[ine].x <= zoomRect.right
							&&l_pstch[ine].y >= zoomRect.bottom
							&&l_pstch[ine].y <= zoomRect.top) {

							wascol = 1;
							if (setMap(LINED)) {

								if (setMap(LININ)) {

									linePoints[lineIndex].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
									linePoints[lineIndex++].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;

								} else {

									linePoints[lineIndex].x = (l_pstch[ine - 1].x - zoomRect.left)*zoomRatio.x + 0.5;
									linePoints[lineIndex++].y = hi - (l_pstch[ine - 1].y - zoomRect.bottom)*zoomRatio.y + 0.5;
									linePoints[lineIndex].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
									linePoints[lineIndex++].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;
								}
							} else {

								if (ine == 0 && ind == 0) {

									linePoints[0].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
									linePoints[0].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;
									lineIndex = 1;
								} else {

									linePoints[0].x = (l_pstch[ine - 1].x - zoomRect.left)*zoomRatio.x + 0.5;
									linePoints[0].y = hi - (l_pstch[ine - 1].y - zoomRect.bottom)*zoomRatio.y + 0.5;
									linePoints[1].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
									linePoints[1].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;
									lineIndex = 2;
								}
								setMap(LININ);
							}
						} else {

							if (rstMap(LININ)) {

								linePoints[lineIndex].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
								linePoints[lineIndex++].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;
								Polyline(StitchWindowMemDC, linePoints, lineIndex);
								lineIndex = 0;
							} else {

								if (ine) {

									//write an equation for this line
									xba = l_pstch[ine].x - l_pstch[ine - 1].x;
									yab = l_pstch[ine - 1].y - l_pstch[ine].y;
									slop = (double)xba / yab;
									off = l_pstch[ine].x + slop*l_pstch[ine].y;
									//does the line intersect with the top of the screen?
									tlng = off - slop*zoomRect.top;
									if (tlng >= zoomRect.left&&tlng <= zoomRect.right) {

										tlin[0].x = (l_pstch[ine - 1].x - zoomRect.left)*zoomRatio.x + 0.5;
										tlin[0].y = hi - (l_pstch[ine - 1].y - zoomRect.bottom)*zoomRatio.x + 0.5;
										tlin[1].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
										tlin[1].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.x + 0.5;
										Polyline(StitchWindowMemDC, tlin, 2);
										goto rotlin;
									}
									//does the line intersect the bottom of the screen?
									tlng = off - slop*zoomRect.bottom;
									if (tlng >= zoomRect.left&&tlng <= zoomRect.right) {

										tlin[0].x = (l_pstch[ine - 1].x - zoomRect.left)*zoomRatio.x + 0.5;
										tlin[0].y = hi - (l_pstch[ine - 1].y - zoomRect.bottom)*zoomRatio.y + 0.5;
										tlin[1].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
										tlin[1].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;
										Polyline(StitchWindowMemDC, tlin, 2);
										goto rotlin;
									}
									//does the line intersect the left side of the screen?
									if (slop) {

										tlng = (off - zoomRect.left) / slop;
										if (tlng >= zoomRect.bottom&&tlng <= zoomRect.top) {

											tlin[0].x = (l_pstch[ine - 1].x - zoomRect.left)*zoomRatio.x + 0.5;
											tlin[0].y = hi - (l_pstch[ine - 1].y - zoomRect.bottom)*zoomRatio.y + 0.5;
											tlin[1].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
											tlin[1].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;
											Polyline(StitchWindowMemDC, tlin, 2);
										}
									}
								}
							rotlin:;

							}
						}
					}
				}
				if (lineIndex) {

					Polyline(StitchWindowMemDC, linePoints, lineIndex);
					linePoints[0].x = linePoints[lineIndex - 1].x;
					linePoints[0].y = linePoints[lineIndex - 1].y;
					lineIndex = 1;
				}
				if (wascol)
					setCol(colch[ind].colorIndex);
			skip:;
			}
		} else {

			pwid = chkMap(HID);
			for (ind = 0; ind < colCnt; ind++) {

				setCol(colch[ind].colorIndex);
				scnt = colch[ind + 1].stitchIndex - colch[ind].stitchIndex;
				scnt = chkup(scnt, ind);
				if (!pwid || (pwid&&colch[ind].colorIndex == actcol))
					drwLin(colch[ind].stitchIndex, scnt, userPen[colch[ind].colorIndex]);
			}
		}
		if (chkMap(SELBOX)) {

			ritcor(&stitchBuffer[closestPointIndex]);
			if (stch2px(closestPointIndex))
				dubox();
		}
		if (chkMap(FRMPSEL))
			ritfcor(&formList[closestFormToCursor].vertices[closestVertexToCursor]);
		if (!chkMap(SELBOX) && !chkMap(FRMPSEL))
			butxt(HCOR, "");
		if (chkMap(WASLIN))
			relin();
		if (chkMap(GRPSEL)) {

			if (cmpstch(closestPointIndex, groupStitchIndex))
				cros(closestPointIndex);
			else {

				slpnt = 0;
				ducros(StitchWindowMemDC);
			}
			selRct(&rngrct);
			nuselrct();
			setMap(SELSHO);
			dusel(StitchWindowMemDC);
		}
		if (zoomFactor < StitchBoxesThreshold) {

			clRmap(RMAPSIZ);
			SelectObject(StitchWindowMemDC, linePen);
			SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
			rint();
			if (chkMap(HID)) {

				for (ind = 0; ind < colCnt; ind++) {

					if (colch[ind].colorIndex == actcol) {

						for (ine = colch[ind].stitchIndex; ine < colch[ind + 1].stitchIndex; ine++) {

							if (stitchBuffer[ine].x >= zoomRect.left&&stitchBuffer[ine].x <= zoomRect.right
								&&stitchBuffer[ine].y >= zoomRect.bottom&&stitchBuffer[ine].y <= zoomRect.top
								&&setRmap(stitchBuffer[ine]))

								stchbox(ine, StitchWindowMemDC);
						}
					}
				}
			} else {

				for (ind = 0; ind < hed.stitchCount; ind++) {

					if (stitchBuffer[ind].x >= zoomRect.left&&stitchBuffer[ind].x <= zoomRect.right
						&&stitchBuffer[ind].y >= zoomRect.bottom&&stitchBuffer[ind].y <= zoomRect.top
						&&setRmap(stitchBuffer[ind]))

						stchbox(ind, StitchWindowMemDC);
				}
			}
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		}
		if (chkMap(CLPSHO))
			duclp();
		if (chkMap(ROTAT) || chkMap(ROTCAPT) || chkMap(MOVCNTR))
			ritrot();
		delete[] linePoints;
	}
	if (formIndex && !chkMap(FRMOF))
		drwfrm();
	if (chkMap(INSFRM))
		rinfrm();
	if (chkMap(BIGBOX))
		dubig();
	if (chkMap(MOVFRM)) {

		setMap(SHOFRM);
		dufrm();
	}
	if (chkMap(SATPNT))
		satzum();
	if (hfdat)
		refrm();
	if (chkMap(GMRK))
		drwmrk(StitchWindowMemDC);
	if (chkMap(PRFACT))
		redraw(hPrf);
	if (chkMap(SELBOX))
		ritnum(STR_NUMSEL, closestPointIndex);
	ritot(hed.stitchCount);
	if (chkMap(INIT))
		lenCalc();
	if (chkMap(MOVSET))
		dumov();
	drwknot();
}

unsigned chkCol(unsigned ind) {

#if  __UseASM__
	_asm {

		mov		ebx, ind
		mov		edx, cbit
		xor		eax, eax
		bt		edx, ebx
		jnc		chk1
		dec		eax
		chk1 :
	}
#else
	return _bittest((long *)&cbit, ind) ? 0xffffffff : 0;
#endif
}

void dubar() {

	unsigned	ind;
	RECT		tRct;
	double		tdub;
	POINT		lin[2];

	tRct.left = ds->rcItem.left;
	tRct.right = ds->rcItem.right;
	tRct.top = 0;
	tRct.bottom = ds->rcItem.bottom;
	for (ind = 0; ind < colCnt; ind++) {

		tdub = (double)colch[ind + 1].stitchIndex / hed.stitchCount;
		tRct.bottom = tdub*ds->rcItem.bottom;
		FillRect(ds->hDC, &tRct, hUserColorBrush[colch[ind].colorIndex]);
		tRct.top = tRct.bottom;
	}
	if (chkMap(SELBOX) || chkMap(GRPSEL)) {

		tdub = (double)closestPointIndex / hed.stitchCount;
		lin[0].y = lin[1].y = tRct.bottom*tdub + 0.5;
		lin[0].x = tRct.left;
		lin[1].x = tRct.right;
		SelectObject(ds->hDC, crossPen);
		SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
		Polyline(ds->hDC, lin, 2);
		if (chkMap(GRPSEL)) {

			tdub = (double)groupStitchIndex / hed.stitchCount;
			lin[0].y = lin[1].y = tRct.bottom*tdub + 0.5;
			lin[0].x = tRct.left;
			lin[1].x = tRct.right;
			Polyline(ds->hDC, lin, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void ritbak(TCHAR* nam, DRAWITEMSTRUCT* p_ds) {

	unsigned	ind, ine, inf, col, lind, redsiz;
	HANDLE		thfil;
	POINT		l_siz;
	fPOINTATTRIBUTE*	tstch;
	POINT*		l_plin;
	STRHED		sthed;
	fPOINT		rsiz;
	double		rat;
	double		ratx, raty;
	COLORREF	tbak;
	COLORREF	tcol[16];
	HBRUSH		tbr;
	HPEN		pen;
	FRMHED*		flst;
	fPOINT*		tflt;
	STREX		thedx;
	unsigned	vervar = 0;
	FRMHEDO*	frmlstx;

	thfil = CreateFile(nam, GENERIC_READ, 0, 0,
		OPEN_EXISTING, 0, 0);
	l_siz.x = p_ds->rcItem.right - p_ds->rcItem.left;
	l_siz.y = p_ds->rcItem.bottom - p_ds->rcItem.top;
	rsiz.x = rsiz.y = 1;

	if (thfil != INVALID_HANDLE_VALUE) {

		ReadFile(thfil, (STRHED*)&sthed, sizeof(STRHED), &red, NULL);
		if (red == sizeof(STRHED)) {

			if ((sthed.headerType & 0xffffff) == 0x746872) {

				vervar = (sthed.headerType & 0xff000000) >> 24;
				switch (vervar) {

				case 0:

					if (hed.hoopType == SMALHUP) {

						rsiz.x = ini.hoopSizeX = SHUPX;
						rsiz.y = ini.hoopSizeY = SHUPY;
					} else {

						rsiz.x = ini.hoopSizeX = LHUPX;
						rsiz.y = ini.hoopSizeY = LHUPY;
					}
					break;

				case 1:
				case 2:

					ReadFile(thfil, (STREX*)&thedx, sizeof(STREX), &red, NULL);
					if (red != sizeof(STREX))
						return;
					rsiz.x = thedx.hoopSizeX;
					rsiz.y = thedx.hoopSizeY;

					break;

				default:

					return;
				}
			}
			ratx = (double)l_siz.x / rsiz.x;
			raty = (double)l_siz.y / rsiz.y;
			if (ratx < raty)
				rat = ratx;
			else
				rat = raty;
			if (sthed.stitchCount) {

				tstch = new fPOINTATTRIBUTE[sthed.stitchCount];
				l_plin = new POINT[sthed.stitchCount];
				redsiz = sthed.stitchCount * sizeof(fPOINTATTRIBUTE);
				ReadFile(thfil, (fPOINTATTRIBUTE*)tstch, redsiz, &red, 0);
				if (redsiz == red) {

					SetFilePointer(thfil, 16, 0, FILE_CURRENT);
					ReadFile(thfil, (COLORREF*)&tbak, 4, &red, 0);
					ReadFile(thfil, (COLORREF*)tcol, 64, &red, 0);
					tbr = CreateSolidBrush(tbak);
					SelectObject(p_ds->hDC, tbr);
					FillRect(p_ds->hDC, &p_ds->rcItem, tbr);
					col = tstch[0].attribute & 0xf;
					pen = CreatePen(PS_SOLID, 1, tcol[col]);
					lind = 0;
					for (ine = 0; ine < sthed.stitchCount; ine++) {

						if ((tstch[ine].attribute & 0xf) == col) {

							l_plin[lind].x = tstch[ine].x*rat;
							l_plin[lind++].y = l_siz.y - tstch[ine].y*rat;
						} else {

							pen = nuPen(pen, 1, tcol[col]);
							SelectObject(p_ds->hDC, pen);
							Polyline(p_ds->hDC, l_plin, lind);
							lind = 0;
							col = tstch[ine].attribute & 0xf;
						}
					}
					if (lind) {

						pen = nuPen(pen, 1, tcol[col]);
						SelectObject(p_ds->hDC, pen);
						Polyline(p_ds->hDC, l_plin, lind);
					}
					DeleteObject(tbr);
					DeleteObject(pen);
					delete[] tstch;
					delete[] l_plin;
				} else {

					delete[] tstch;
					delete[] l_plin;
					CloseHandle(thfil);
					return;
				}
			} else
				SetFilePointer(thfil, 84, 0, FILE_CURRENT);
			if (sthed.formCount) {

				l_plin = new POINT[MAXFRMLINS];
				SetFilePointer(thfil, 80, 0, FILE_CURRENT);
				flst = new FRMHED[sthed.formCount];
				tflt = new fPOINT[sthed.pointCount];
				if (vervar < 2) {

					frmlstx = (FRMHEDO*)&bseq;
					ReadFile(thfil, (FRMHEDO*)frmlstx, sthed.formCount * sizeof(FRMHEDO), &red, 0);
					if (red != sthed.pointCount * sizeof(FRMHEDO))
						goto bakskp;
					FillMemory(&bseq, 0, sizeof(FRMHED)*formIndex);
					for (ind = 0; ind < sthed.formCount; ind++) {

						ptrSelectedForm = &flst[ind];
						MoveMemory(ptrSelectedForm, &frmlstx[ind], sizeof(FRMHEDO));
					}
				} else {

					redsiz = sthed.formCount * sizeof(FRMHED);
					ReadFile(thfil, (FRMHED*)flst, redsiz, &red, 0);
					if (red != redsiz)
						goto bakskp;
				}
				redsiz = sthed.pointCount * sizeof(fPOINT);
				ReadFile(thfil, (fPOINT*)tflt, redsiz, &red, 0);
				if (red != redsiz)
					goto bakskp;
				inf = 0;
				for (ind = 0; ind < sthed.formCount; ind++) {

					lind = inf;
					for (ine = 0; (ine < flst[ind].sides) && (inf < sthed.pointCount); ine++) {

						l_plin[ine].x = tflt[inf].x*rat;
						l_plin[ine].y = l_siz.y - tflt[inf++].y*rat;
					}
					l_plin[ine].x = tflt[lind].x*rat;
					l_plin[ine].y = l_siz.y - tflt[lind].y*rat;
					SelectObject(p_ds->hDC, formPen);
					SetROP2(p_ds->hDC, R2_XORPEN);
					if (formList[ind].type == LIN)
						Polyline(p_ds->hDC, l_plin, flst[ind].sides);
					else
						Polyline(p_ds->hDC, l_plin, flst[ind].sides + 1);
					SetROP2(StitchWindowMemDC, R2_COPYPEN);
				}
			bakskp:;
				delete[] flst;
				delete[] tflt;
				delete[] l_plin;
			}
		}
		CloseHandle(thfil);
	}
}

void durct(unsigned shft, RECT irct) {

	unsigned	lo, hi, len;
	double		rat;

	lo = (upColor >> shft) & 0xff;
	hi = (downColor >> shft) & 0xff;
	highRect.left = lowRect.left = middleRect.left = irct.left;
	highRect.right = lowRect.right = middleRect.right = irct.right;
	len = irct.bottom - irct.top;
	rat = (double)lo / 255;
	middleRect.top = len*rat + irct.top;
	rat = (double)hi / 255;
	middleRect.bottom = len*rat + irct.top;
	rstMap(DUHI);
	rstMap(DULO);
	if (lo) {

		setMap(DULO);
		lowRect.bottom = middleRect.top;
		lowRect.top = 0;
	}
	if (hi != 255) {

		setMap(DUHI);
		highRect.top = middleRect.bottom;
		highRect.bottom = irct.bottom;
	}
}

void dublk(HDC dc) {

	if (chkMap(DUHI))
		FillRect(dc, &highRect, hblk);
	if (chkMap(DULO))
		FillRect(dc, &lowRect, hblk);
}

LRESULT CALLBACK WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	unsigned			ind, ine;
	TCHAR				buf[10];		//for integer to string conversion
	unsigned			len;			//string length
	SIZE				l_txSiz;			//for measuring text items
	POINT				scPnt;			//for scroll bar functions
	POINT				lin[2];
	long				twid;
	double				tdub;
	TCHAR				nam[_MAX_PATH];

	switch (message) {

	case WM_INITMENU:

		if (rstMap(PRFACT)) {

			DestroyWindow(hPrf);
			rstMap(WASRT);
		}
		unmsg();
		undat();
		rstMap(FORMIN);
		setMap(RESTCH);
		break;

	case WM_HSCROLL:

		switch ((long)(LOWORD(wParam))) {

		case SB_LINELEFT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				delay += SPEDLIN;
				if (delay > MAXDELAY)
					delay = MAXDELAY;
				setsped();
				SetScrollPos(hSped, SB_CTL, MAXDELAY - delay, TRUE);
			} else {

				scPnt.x = (zoomRect.right - zoomRect.left)*LINSCROL;
				if (!scPnt.x)
					scPnt.x = 1;
				scPnt.y = 0;
				rshft(scPnt);
			}
			return 1;

		case SB_LINERIGHT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				delay -= SPEDLIN;
				if (delay < MINDELAY)
					delay = MINDELAY;
				setsped();
				SetScrollPos(hSped, SB_CTL, MAXDELAY - delay, TRUE);
			} else {

				scPnt.x = -(zoomRect.right - zoomRect.left)*LINSCROL;
				if (!scPnt.x)
					scPnt.x = -1;
				scPnt.y = 0;
				rshft(scPnt);
			}
			return 1;

		case SB_PAGELEFT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				delay += SPEDPAG;
				if (delay < MINDELAY)
					delay = MINDELAY;
				setsped();
				SetScrollPos(hSped, SB_CTL, MAXDELAY - delay, TRUE);
			} else
				pglft();
			return 1;

		case SB_PAGERIGHT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				delay -= SPEDPAG;
				if (delay < MINDELAY)
					delay = MINDELAY;
				setsped();
				SetScrollPos(hSped, SB_CTL, MAXDELAY - delay, TRUE);
			} else
				pgrit();
			return 1;

		case SB_THUMBPOSITION:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				if ((HWND)lParam == hSped) {

					ind = (unsigned)HIWORD(wParam);
					delay = MAXDELAY - ind;
					setsped();
					SetScrollPos(hSped, SB_CTL, ind, TRUE);
				}
			} else {

				if ((HWND)lParam == hHor) {

					scPnt.x = zoomRect.right - zoomRect.left;
					zoomRect.left = (long)HIWORD(wParam);
					zoomRect.right = zoomRect.left + scPnt.x;
					if (zoomRect.right > unzoomedRect.x) {

						zoomRect.right = unzoomedRect.x;
						zoomRect.left = unzoomedRect.x - scPnt.x;
					}
					setMap(RESTCH);
				}
			}
			return 1;
		}
		break;

	case WM_VSCROLL:

		switch ((int)LOWORD(wParam)) {

			//		unpat();
		case SB_LINEDOWN:

			scPnt.y = (zoomRect.top - zoomRect.bottom)*LINSCROL;
			if (!scPnt.y)
				scPnt.y = 1;
			scPnt.x = 0;
			rshft(scPnt);
			return 1;

		case SB_LINEUP:

			scPnt.y = -(zoomRect.top - zoomRect.bottom)*LINSCROL;
			if (!scPnt.y)
				scPnt.y = -1;
			scPnt.x = 0;
			rshft(scPnt);
			return 1;

		case SB_PAGEDOWN:

			pgdwn();
			return 1;

		case SB_PAGEUP:

			pgup();
			return 1;

		case SB_THUMBPOSITION:

			scPnt.y = zoomRect.top - zoomRect.bottom;
			zoomRect.top = unzoomedRect.y - (long)HIWORD(wParam);
			zoomRect.bottom = zoomRect.top - scPnt.y;
			if (zoomRect.bottom < 0) {

				zoomRect.bottom = 0;
				zoomRect.top = scPnt.y;
			}
			setMap(RESTCH);
			return 1;
		}
		break;

	case WM_DRAWITEM:

		//owner draw windows
		ds = (LPDRAWITEMSTRUCT)lParam;
		if (ds->hwndItem == hStch&&ds->itemAction == ODA_DRAWENTIRE) {

			if (chkMap(TXTRED))
			{
				drwtxtr();
				return 1;
			}
			if (!chkMap(RUNPAT)) {

				if (!chkMap(HIDSTCH) && (hFil || chkMap(INIT) || formIndex || chkMap(SATPNT)) && !chkMap(BAKSHO))
					drwStch();
				else {

					FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
					duzrat();
					dugrid();
					if (chkMap(HIDSTCH)) {

						drwfrm();
						if (chkMap(SATPNT))
							satzum();
					}
					if (chkMap(PRFACT))
						redraw(hPrf);
					else {

						if (chkMap(MOVFRM)) {

							unfrm();
							setMap(SHOFRM);
							dufrm();
						}
						if (chkMap(POLIMOV)) {

							setMap(SHOFRM);
							mdufrm();
						}
					}
				}
				BitBlt(
					StitchWindowDC,			// handle to destination DC
					0,			// x-coord of destination upper-left corner
					0,			// y-coord of destination upper-left corner
					stitchWindowClientRect.right,	// width of destination rectangle
					stitchWindowClientRect.bottom,	// height of destination rectangle
					StitchWindowMemDC,			// handle to source DC
					0,			// x-coordinate of source upper-left corner
					0,			// y-coordinate of source upper-left corner
					SRCCOPY		// raster operation code
				);
				if (chkMap(ROTSHO))
					durot();
				if (chkMap(SHOSAT))
					dusat();
				if (chkMap(SHOINSF))
					duinsf();
			}
			return 1;
		}
		if (ds->hwndItem == hBar&&ds->itemAction == ODA_DRAWENTIRE) {

			if (hed.stitchCount)
				dubar();
			else
				FillRect(ds->hDC, &ds->rcItem, (HBRUSH)(COLOR_WINDOW + 1));
			return 1;
		}
		if (ds->hwndItem == hbuts[HHID] && ds->itemAction == ODA_DRAWENTIRE) {

			ind = (buttonWid3 - pcolsiz.cx) >> 1;
			if (chkMap(HID)) {

				FillRect(ds->hDC, &ds->rcItem, hUserColorBrush[actcol]);
				SetBkColor(ds->hDC, useCol[actcol]);
			} else
				FillRect(ds->hDC, &ds->rcItem, (HBRUSH)(COLOR_BTNFACE + 1));
			if (chkMap(TXTRED))
			{
				LoadString(hInst, IDS_TXWID, nam, _MAX_PATH);
				sprintf_s(hlpbuf, sizeof(hlpbuf), nam, tscr.width / PFGRAN);
				TextOut(ds->hDC, ind, 1, hlpbuf, strlen(hlpbuf));;
			} else
				TextOut(ds->hDC, ind, 1, stab[STR_PIKOL], strlen(stab[STR_PIKOL]));;
			return 1;
		}
		if (chkMap(WASTRAC)) {

			for (ind = 0; ind < 3; ind++) {

				if (ds->hwndItem == htrup[ind]) {

					FillRect(ds->hDC, &ds->rcItem, trbrsh[ind]);
					upnum(ind);
					return 1;
				}
				if (ds->hwndItem == htrdwn[ind]) {

					FillRect(ds->hDC, &ds->rcItem, trbrsh[ind]);
					dwnum(ind);
				}
				if (ds->hwndItem == hctrc[ind]) {

					durct(trshft[ind], ds->rcItem);
					FillRect(ds->hDC, &middleRect, trbrsh[ind]);
					dublk(ds->hDC);
					return 1;
				}
				if (ds->hwndItem == hstrc[ind]) {

					tbrsh = hblk;
					strcpy_s(buf, stab[STR_OFF]);
					SetBkColor(ds->hDC, 0);
					SetTextColor(ds->hDC, traceRGB[ind]);
					if (chkMap(traceRGBFlag[ind])) {

						tbrsh = trbrsh[ind];
						strcpy_s(buf, stab[STR_ON]);
						SetTextColor(ds->hDC, 0);
						SetBkColor(ds->hDC, traceRGB[ind]);
					}
					FillRect(ds->hDC, &ds->rcItem, tbrsh);
					TextOut(ds->hDC, 1, 1, buf, strlen(buf));
					return 1;
				}
				if (ds->hwndItem == htrnum) {

					FillRect(ds->hDC, &ds->rcItem, trbrsh[colm]);
					SetBkColor(ds->hDC, traceRGB[colm]);
					TextOut(ds->hDC, 1, 1, trinbuf, strlen(trinbuf));
					return 1;
				}
			}
		} else {

			for (ind = 0; ind < 16; ind++) {

				if (ds->hwndItem == hDef[ind]) {

					FillRect(ds->hDC, &ds->rcItem, hDefaultColorBrush[ind]);
					if (chkCol(ind)) {

						SetBkColor(ds->hDC, defCol[ind]);
						SetTextColor(ds->hDC, defTxt(ind));
						sprintf_s(buf, sizeof(buf), "%d", ind + 1);
						len = strlen(buf);
						GetTextExtentPoint32(ds->hDC, buf, len, &l_txSiz);
						TextOut(ds->hDC, (buttonWid - l_txSiz.cx) >> 1, 0, buf, len);
					}
					return 1;
				}
				if (ds->hwndItem == hCol[ind]) {

					FillRect(ds->hDC, &ds->rcItem, hUserColorBrush[ind]);
					if (ind == actcol) {

						SelectObject(ds->hDC, crossPen);
						SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
						lin[0].x = lin[1].x = ds->rcItem.right >> 1;
						lin[0].y = 0;
						lin[1].y = ds->rcItem.bottom;
						Polyline(ds->hDC, lin, 2);
						lin[0].y = lin[1].y = ds->rcItem.bottom >> 1;
						lin[0].x = 0;
						lin[1].x = ds->rcItem.right;
						Polyline(ds->hDC, lin, 2);
						SetROP2(StitchWindowMemDC, R2_COPYPEN);
					}
					return 1;
				}
			}
		}
		if (chkMap(BAKSHO) && ds->itemAction == ODA_DRAWENTIRE) {

			if (chkMap(THUMSHO)) {

				for (ind = 0; ind < 4; ind++) {

					if (ds->hwndItem == hvu[ind] && ind < thumdcnt) {

						ritbak(thumsel[ind], ds);
						rthumnam(ind);
						return 1;
					}
				}
			} else {

				for (ind = 0; ind < OLDVER; ind++) {

					if (ds->hwndItem == hvu[ind]) {

						strcpy_s(nam, thrnam);
						ine = duth(nam);
						nam[ine] = (TCHAR)ind + 's';
						ritbak(nam, ds);
						return 1;
					}
				}
			}
		}
		break;

	case WM_SIZE:

		GetClientRect(p_hWnd, &mRct);
		switch (wParam) {

		case SIZE_MAXIMIZED:

			setu(SAVMAX);
			break;

		case SIZE_MINIMIZED:

			rstu(SAVMAX);
			break;

		case SIZE_RESTORED:

			rstu(SAVMAX);
			chkirct();
			if (setMap(SIZED)) {

				ine = 0;
				ind = (double)screenSizePixels.cx - 30;
				if ((unsigned)(mRct.right - mRct.left) > ind) {

					ine = (screenSizePixels.cx - ind) >> 1;
					mRct.left = ine;
					mRct.right = screenSizePixels.cx - ine;
				}
				ind = (double)screenSizePixels.cy - 30;
				if ((unsigned)(mRct.bottom - mRct.top) > ind) {

					ine = (screenSizePixels.cy - ind) >> 1;
					mRct.top = ine;
					mRct.bottom = screenSizePixels.cy - ine;
				}
				if (ine)
					MoveWindow(p_hWnd, mRct.left, mRct.top, mRct.right - mRct.left, mRct.bottom - mRct.top, TRUE);
			} else {

				MoveWindow(p_hWnd, ini.initialWindowCoords.left, ini.initialWindowCoords.top, ini.initialWindowCoords.right - ini.initialWindowCoords.left, ini.initialWindowCoords.bottom - ini.initialWindowCoords.top, TRUE);
			}
			ShowWindow(p_hWnd, SW_SHOW);
			break;
		}
		GetClientRect(p_hWnd, &mRct);
		movStch();
		if (chkMap(ZUMED)) {

			tdub = (double)stitchWindowClientRect.bottom / (zoomRect.top - zoomRect.bottom);
			twid = stitchWindowClientRect.right / tdub;
			if (twid + zoomRect.left > unzoomedRect.x) {

				zoomRect.right = unzoomedRect.x;
				zoomRect.left = unzoomedRect.x - twid;
			} else
				zoomRect.right = twid + zoomRect.left;
		} else {

			zoomRect.left = zoomRect.bottom = 0;
			zoomRect.right = unzoomedRect.x;
			zoomRect.top = unzoomedRect.y;
		}
		selCnt = 0;
		setMap(RESTCH);
		if (chkMap(SELBOX))
			shft2box();
		if (chkMap(RUNPAT)) {

			FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
			if (chkMap(ZUMED))
				runPoint = groupStartStitch;
			else
				runPoint = 0;
		}
		return 1;

	case WM_MOVE:

		GetClientRect(p_hWnd, &mRct);
		if ((mRct.right - mRct.left) < 20) {

			mRct.left = 0;
			mRct.right = 300;
		}
		if ((mRct.bottom - mRct.top) < 20) {

			mRct.top = 0;
			mRct.bottom = 300;
		}
		movStch();
		if (chkMap(RUNPAT)) {

			duzrat();
			runPoint = 0;
			FillRect(StitchWindowDC, &stitchWindowClientRect, hBackgroundBrush);
		}
		return 1;

	case WM_CLOSE:

		dun();
		if (chkMap(SAVEX))
			return 1;
		break;
	}
	return DefWindowProc(p_hWnd, message, wParam, lParam);
}

void sachk() {

	unsigned	ind, ine, bakclo;
	SATCON*		ts;
	TCHAR*		pchr;
	FRMHED*		tfrm;

	pchr = msgbuf;
	for (ind = 0; ind < formIndex; ind++) {

		tfrm = &formList[ind];
		if (tfrm->type == SAT&&tfrm->satinGuideCount) {

			ts = tfrm->satinOrAngle.sac;
			for (ine = 0; ine < tfrm->satinGuideCount; ine++) {

				if (ts[ine].start > tfrm->sides || ts[ine].finish > tfrm->sides) {

					bakclo = closestFormToCursor;
					closestFormToCursor = ind;
					delsac(ind);
					closestFormToCursor = bakclo;
				}
			}
		}
	}
	if (pchr != msgbuf)
		shoMsg(msgbuf);
}

#if __UseASM__ == 0
#define BAD_FPU_EX (_EM_OVERFLOW | _EM_ZERODIVIDE | _EM_INVALID)
#define COMMON_FPU_EX (_EM_INEXACT | _EM_UNDERFLOW | _EM_DENORMAL)
#define ALL_FPU_EX (BAD_FPU_EX | COMMON_FPU_EX)
#endif

int	fltex(int cod) {

#if  __UseASM__
	short	cw;

	_asm {

		xor		eax, eax
		cmp		cod, 0x10
		jne		short fltex1
		mov		cw, 0x27f
		fldcw	cw
		dec		eax
		fltex1 :
	}
#else
	if (cod != 0x10)
		return 0;
	unsigned int current_word = 0;
	_controlfp_s(&current_word, ALL_FPU_EX, _MCW_EM);
	_controlfp_s(&current_word, _RC_NEAR, _MCW_RC);

	return -1;
#endif
}

int APIENTRY WinMain(_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow) {
	UNREFERENCED_PARAMETER(nCmdShow);

	WNDCLASSEX		wc;
	LOGBRUSH		br;

	br.lbColor = COLOR_BACKGROUND + 1;
	br.lbHatch = 0;
	br.lbStyle = BS_SOLID;

	hInst = hInstance;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
		32, 32, LR_SHARED);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = "thred";
	wc.hIconSm = NULL;

	//to keep the compiler from complaining
	hPrevInstance;
	lpCmdLine;

	if (RegisterClassEx(&wc)) {

		redini();
		if (ini.initialWindowCoords.right) {

			hWnd = CreateWindow(
				"thred",
				"",
				WS_OVERLAPPEDWINDOW,
				ini.initialWindowCoords.left,
				ini.initialWindowCoords.right,
				ini.initialWindowCoords.right - ini.initialWindowCoords.left,
				ini.initialWindowCoords.bottom - ini.initialWindowCoords.top,
				0,
				0,
				hInstance,
				0);
		} else {

			hWnd = CreateWindow(
				"thred",
				"",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				0,
				0,
				hInstance,
				0);
			GetClientRect(hWnd, &mRct);
			ini.initialWindowCoords.left = mRct.left;
			ini.initialWindowCoords.right = mRct.right;
			ini.initialWindowCoords.top = mRct.top;
			ini.initialWindowCoords.bottom = mRct.bottom;
		}
		init();
		if (chku(SAVMAX))
			ShowWindow(hWnd, SW_SHOWMAXIMIZED);
		else
			ShowWindow(hWnd, SW_SHOW);
		if (!*ini.designerName)
		{
			LoadString(hInst, IDS_UNAM, ini.designerName, 50);
			getdes();
		}
		while (GetMessage(&msg, NULL, 0, 0)) {

			setMap(SAVACT);
			if (!chkMsg())
				DispatchMessage(&msg);
			if (rstMap(FCHK))
				frmchkx();
			if (rstMap(RESTCH))
				redraw(hStch);
			if (rstMap(RELAYR))
				ritlayr();
			if (!chkMap(TXTRED))
				sachk();
			if (rstMap(DUMEN))
				DrawMenuBar(hWnd);
		}
		return 0;
	}
	return -1;
}