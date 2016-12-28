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

//Forward Declarations
void		blak ();
unsigned	bufref ();
void		chkhup ();
void		clRmap (unsigned len);
void		clrhbut (unsigned strt);
BOOL		cmpstch (unsigned ind0, unsigned ind1);
void		cros (unsigned ind);
void		delet ();
void		delsfrms (unsigned cod);
void		delsmal (unsigned ind, unsigned fin);
void		delstchm ();
void		drwLin (unsigned ind, unsigned len, HPEN hPen);
void		dstcurs ();
void		duIns ();
void		dubuf ();
void		dufdef ();
void		dusel (HDC dc);
void		duselrng ();
void		dutrnum0 (unsigned num);
void		dutrnum1 ();
void		endpnt ();
void		fnamtabs ();
void		fndknt ();
void		frmdel ();
void		frmx (POINT cpnt, HDC dc);
COLORREF	fswap (COLORREF tcol);
void		fthrfn ();
void		hupfn ();
void		ilin ();
void		insfil ();
void		lenCalc ();
void		movStch ();
void		movbox ();
void		mvstch (unsigned dst, unsigned src);
void		mvstchs (unsigned dst, unsigned src, unsigned cnt);
void		nuAct (unsigned ind);
void		nuslst (unsigned find);
void		okcan ();
void		patdun ();
unsigned	pt2colInd (unsigned pInd);
void		rSelbox ();
void		redraw (HWND dWnd);
void		ritnum (unsigned cod, unsigned num);
void		ritot (unsigned num);
void		rngadj ();
void		rotfn ();
void		rotfns ();
void		rotpix (POINT rpnt, POINT* tpnt);
void		rstAll ();
void		rstdu ();
void		save ();
void		sCor2px (dPOINT stpnt, POINT* pxpnt);
void		selRct (fRECTANGLE* srct);
unsigned	setRmp (unsigned pbit);
void		setpsel ();
void		shft (fPOINT shPnt);
void		shoMsg (TCHAR* str);
void		thrsav ();
void		trcols (COLORREF col);
void		trcsel ();
void		trdif ();
void		trinit ();
void		unbsho ();
void		unlin ();
void		unmsg ();
void		unsel ();
void		unsid ();
void		unthum ();
void		untrace ();
void		xlin ();
void		xlin1 ();
void		zRctAdj (); 

extern	TXPNT*			adtx (int cnt);
extern	void			angclp ();
extern	void			angsclp ();
extern	void			apliq ();
extern	void			bakagain ();
extern	void			bakdup ();
extern	void			bfilmsg ();
extern	void			bhol ();
extern	void			bholbrd ();
extern	void			bord ();
extern	void			boxsel ();
extern	void			bsizpar ();
extern	void			butxt (unsigned ind, TCHAR* str);
extern	void			centir ();
extern	void			chain ();
extern	void			chan ();
extern	void			chgchk (int cod);
extern	void			chgwrn ();
extern	void			chkcont ();
extern	unsigned		chkfrm ();
extern	BOOL			chkr (unsigned pbit);
extern	BOOL			chkrypt (unsigned arg0, unsigned arg1, unsigned arg2, unsigned arg3);
extern	unsigned		closfrm ();
extern	void			clpfil ();
extern	void			clpic (unsigned short strtlin);
extern	void			clrfills ();
extern	void			clrstch ();
extern	void			cntrx ();
extern	void			col2frm ();
extern	void			contfil ();
extern	void			cpylayr (unsigned play);
extern	void			crmsg (TCHAR* nam);
extern	void			crop ();
extern	void			dazdef ();
extern	void			debean ();
extern	void			delcon (unsigned cpnt);
extern	void			deleclp (unsigned fpnt);
extern	void			delflt (unsigned fpnt);
extern	void			delfrms ();
extern	void			delmclp (unsigned fpnt);
extern	void			delmfil (unsigned col);
extern	void			delsac (unsigned fpnt);
extern	void			delspnt ();
extern	void			deltx ();
#ifdef _DEBUG
extern	void			dmpat ();
#endif
extern	void			drwcon ();
extern	void			drwfrm ();
extern	void			drwsat ();
extern	void			drwtxtr ();
extern	void			duauxnam ();
extern	void			dubcol (unsigned col);
extern	void			dubean ();
extern	void			dubfil ();
extern	void			dubig ();
extern	void			dubit (unsigned bit);
extern	void			dublen (float len);
extern	void			dubmax (float len);
extern	void			dubmin (float len);
extern	void			dubold ();
extern	void			dubsfil ();
extern	void			dubspac (float len);
extern	void			dueg (unsigned nsids);
extern	void			dufang (float ang);
extern	void			dufcol (unsigned col);
extern	void			dufhi (float len);
extern	void			dufind (float idnt);
extern	void			duflen (float len);
extern	void			dufmax (float len);
extern	void			dufmin (float len);
extern	void			duform (unsigned ind);
extern	void			dufrm ();
extern	void			dufspac (float spac);
extern	void			dufwid (float len);
extern	void			dufxang (float ang);
extern	void			duhart (unsigned nsids);
extern	void			duinsf ();
extern	void			dulens (unsigned nsids);
extern	void			dundcol (unsigned col);
extern	void			duprot ();
extern	void			dupsel (HDC dc);
extern	void			durpoli (unsigned sidz);
extern	void			dusat ();
extern	void			dushft ();
extern	void			duspac (float spac);
extern	void			duspir (unsigned nsids);
extern	void			dustar (unsigned nsids, double len);
extern	void			dusulen (float len);
extern	void			dutxtfil ();
extern	void			duxclp ();
extern	void			duzig (unsigned nsids);
extern	void			fclp ();
extern	void			fcntr ();
extern	void			fethr ();
extern	void			fethrf ();
extern	void			filangl ();
extern	void			filclpx ();
extern	void			filhor ();
extern	void			filnopn (unsigned cod, TCHAR* nam);
extern	void			filsat ();
extern	void			filvrt ();
extern	unsigned		find1st ();
extern	void			fliph ();
extern	void			flipv ();
extern	void			flpord ();
extern	unsigned		fltind (fPOINT* pnt);
extern	void			fltspac (fPOINT* strt, unsigned cnt);
extern	void			form ();
extern	void			fritfil ();
extern	void			frm0 ();
extern	void			frmadj (unsigned find);
extern	void			frmchkx ();
extern	void			frmclr (FRMHED* dst);
extern	void			frmcpy (FRMHED* dst, FRMHED* src);
extern	void			frmlin (fPOINT* scr, unsigned sidz);
extern	void			frmnum ();
extern	void			frmnumfn (unsigned nunum);
extern	void			frmon ();
extern	void			frmout (unsigned ind);
extern	void			frmovlin ();
extern	BOOL			frmrng (unsigned fpnt, RANGE* rng);
extern	void			frmsadj ();
extern	void			frmsqr (unsigned ind);
extern	void			fsclpx ();
extern	void			fselrct (unsigned fpnt);
extern	void			fsizpar ();
extern	void			fsort ();
extern	void			fvars (unsigned ind);
extern	void			getfinfo (unsigned ind);
extern	unsigned		getlast ();
extern	void			grpmsg ();
extern	void			grpmsg1 ();
extern	void			help ();
extern	void			horclp ();
extern	void			horclpfn ();
extern	void			horsclp ();
extern	void			hsizmsg ();
extern	void			infrm ();
extern	void			insat ();
extern	void			ispcdclp ();
extern	BOOL			istx (unsigned find);
extern	void			join ();
extern	void			lodchk ();
extern	void			lodstr ();
extern	void			makspac (unsigned strt, unsigned cnt);
extern	void			maxtsiz (TCHAR* str, POINT* pt);
extern	void			maxwid (unsigned strt, unsigned fin);
extern	float			midl (float hi, float lo);
extern	void			mdufrm ();
extern	void			movlayr (unsigned play);
extern	void			msgstr (unsigned cod);
extern	void			munfrm ();
extern	void			mvshft ();
extern	void			notcwlk ();
extern	BOOL			notfstch (unsigned at);
extern	BOOL			notsel ();
extern	void			notund ();
extern	void			notwlk ();
extern	void			nubrdcol (unsigned col);
extern	void			nudsiz ();
extern	void			nufilcol (unsigned col);
extern	void			nufsel ();
extern	void			nufthcol (unsigned col);
extern	void			nulapcol (unsigned col);
extern	unsigned short	nxt (unsigned short ind);
extern	void			oclp (fPOINT* p_clp, unsigned p_nclp);
extern	unsigned		pdir (unsigned ind);
extern	void			pes2crd ();
extern	void			picot ();
extern	void			prbug ();
extern	void			prfmsg ();
extern	void			prfsid (HWND wnd);
extern	unsigned		projh (double pnty, fPOINT pnt0, fPOINT pnt1, dPOINT* ipnt);
extern	void			prpbrd (double spac);
extern	unsigned short	prv (unsigned ind);
extern	unsigned		psg ();
extern	void			px2stchf (POINT pxpnt, fPOINT* stpnt);
extern	void			pxrct2stch (RECT pxr, fRECTANGLE* str);
extern	void			rats ();
extern	void			ratsr ();
extern	void			rct2sel (RECT rct, POINT* lin);
extern	void			redtx ();
extern	void			redtx ();
extern	void			redup ();
extern	void			refil ();
extern	void			refilal ();
extern	void			refilfn ();
extern	void			refrm ();
extern	void			repar ();
extern	void			ribon ();
extern	void			rinfrm ();
extern	void			ritbrd ();
extern	void			riter ();
extern	void			ritfrct (unsigned ind, HDC dc);
extern	void			rotagain ();
extern	void			rotcmd ();
extern	void			rotdup ();
extern	void			rstfrm ();
extern	void			rstxt ();
extern	void			rstxt ();
extern	void			rtrclp ();
extern	void			sRct2px (fRECTANGLE strct, RECT* pxrct);
extern	void			satadj ();
extern	void			satbrd ();
extern	void			satfix ();
extern	void			satknkt ();
extern	void			satpnt0 ();
extern	void			satpnt1 ();
extern	void			satsel ();
extern	void			satzum ();
extern	void			savblen (float len);
extern	void			savdisc ();
extern	void			savplen (float len);
extern	void			savtxt ();
extern	void			selal ();
extern	void			selalfil ();
extern	void			selalfrm ();
extern	void			selfil (unsigned typ);
extern	void			selsqr (POINT cpnt, HDC dc);
extern	void			setap ();
extern	void			setbcol ();
extern	void			setblen ();
extern	void			setbmax ();
extern	void			setbmin ();
extern	void			setbspac ();
extern	void			setclpspac ();
extern	void			setcwlk ();
extern	void			setear ();
extern	void			setexpand ();
extern	void			setfang ();
extern	void			setfchk ();
extern	void			setfcol ();
extern	void			setfhi ();
extern	void			setfilend ();
extern	void			setfilstrt ();
extern	void			setfind ();
extern	void			setflen ();
extern	void			setfmax ();
extern	void			setfmin ();
extern	void			setfpnt ();
extern	void			setfrm ();
extern	void			setfspac ();
extern	void			setfwid ();
extern	void			sethup ();
extern	void			setins ();
extern	void			setmfrm ();
extern	void			setr (unsigned pbit);
extern	void			setrang ();
#if PESACT
extern	BOOL			setrc (unsigned pbit);
#endif
extern	void			setshft ();
extern	void			setstrtch ();
extern	void			setuang ();
extern	void			setucol ();
extern	void			setulen ();
extern	void			setund ();
extern	void			setuspac ();
extern	void			setwlk ();
extern	void			setwlkind ();
extern	void			sfCor2px (fPOINT stpnt, POINT* pxpnt);
extern	void			sfuang ();
extern	void			shoseln (unsigned cod0, unsigned cod1);
extern	void			shrnk ();
extern	void			sidwnd (HWND wnd);
extern	void			sizstch (fRECTANGLE* rct);
extern	void			snap ();
extern	void			spltfrm ();
extern	void			srtbyfrm ();
extern	void			srtfrm ();
extern	void			stCor2px (fPOINTATTRIBUTE stpnt, POINT* pxpnt);
extern	void			stchrct2px (fRECTANGLE srct, RECT* prct);
extern	void			stchs2frm ();
extern	void			strtchbox ();
extern	void			tabmsg (unsigned cod);
extern	void			tglfrm ();
extern	void			tomsg ();
extern	void			tsizmsg (TCHAR* sizstr, double pd_Size);
extern	void			tst ();
extern	void			txdun ();
extern	void			txof ();
extern	void			txsnap ();
extern	void			txtclp ();
extern	void			txtkey (unsigned cod);
extern	void			txtlbut ();
extern	void			txtlin ();
extern	void			txtrbut ();
extern	void			txtrmov ();
extern	void			txtrup ();
extern	void			undlen ();
extern	void			unfil ();
extern	void			unfrm ();
extern	void			uninsf ();
extern	void			unpsel ();
extern	void			unstrtch ();
extern	void			uspac ();
extern	void			vrtclp ();
extern	void			vrtsclp ();
extern	void			wavfrm ();

extern	fRECTANGLE		allItemsRectangle;
extern	double			borderWidth;
extern	BSEQPNT			bseq[BSEQLEN];
extern	float			buttonholeFillCornerLength;
extern	fPOINT			clipboardPoints[MAXCLPNTS];
extern	unsigned		closestFormToCursor;
extern	unsigned		closestVertexToCursor;
extern	int				cloxcnt;
extern	unsigned		clpad;
extern	TCHAR*			cpyrit;
extern	SATCON*			currentFormConnections;
extern	fPOINT*			currentFormVertices;
extern	unsigned		fgpnt0;
extern	double			fillAngle;
extern	POINT			formLines[MAXFRMLINS];
extern	unsigned		fltad;
extern	fPOINT			fmovdif;
extern	unsigned		formIndex;
extern	FRMHED			formList[MAXFORMS];
extern	POINT			formOutlineRectangle[10];
extern	fPOINT			formPoints[MAXFRMPNTS];
extern	HWND			hindx;
extern	TCHAR			hlpbuf[HBUFSIZ];
extern	HWND			hlptxt;
extern	HWND			hMsg;
extern	HWND			hnxt;
extern	double			horizontalRatio;
extern	HWND			hsrch;
extern	fPOINT			iseq[MAXSEQ];
extern	TCHAR*			laytxt[];
extern	fPOINT			lowerLeftStitch;
extern	POINT			mvlin[3];
extern	unsigned		newFormVertexCount;
extern	unsigned		outputIndex;
extern	fPOINT			oseq[OSEQLEN];
extern	float			picotSpace;
extern	unsigned		previousFormIndex;
extern	long			prfwid;
extern	unsigned		pseudoRandomValue;
extern	int				ptxhst;
extern	unsigned		satkad;
extern	SATCON			satks[MAXSAC];
extern	unsigned		selectedFormControlVertex;
extern	unsigned		selectedFormCount;
extern	unsigned short	selectedFormList[MAXFORMS];
extern	POINT			selectedFormsLine[9];
extern	RECT			selectedFormsRectangle;
extern	POINT			selectedPointsRectangle[9];
extern	TCHAR*			shrtmsg;
extern	unsigned short	sides;
extern	double			snapLength;
extern	double			spiralWrap;
extern	TCHAR*			stab[STR_LEN];
extern	double			starRatio;
extern	double			stitchSpace;
extern	HWND			thDat[LASTLIN];
extern	TXHST			thsts[16];
extern	HWND			thTxt[LASTLIN];
extern	TXTSCR			tscr;
extern	int				txad;
extern	TXPNT			txpnts[MAXSEQ];
extern	double			verticalRatio;
extern	unsigned short	wordParam;
extern	unsigned		activePointIndex;
extern	double			xyRatio;

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
RECT				mainWindowRect;			//main window size
RECT				colorBarRect;			//color bar rectangle
RECT				minLenRect;				//minimum length rectangle
RECT				maxLenRect;				//maximum length rectangle
unsigned			smallestStitchIndex;	//pointer to the smallest stitch in the selected range
unsigned			largestStitchIndex;		//pointer to the largest stitch in the selected range
unsigned			currentStitchIndex;		//pointer to the current selection for length search
HDC					mainDC;					//main device context handle
HDC					stitchWindowMemDC;		//stitch window memory device context
HDC					stitchWindowDC;			//stitch window device context
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
double				stitchBoxesThreshold = STCHBOX;//threshold for drawing stitch boxes
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
TCHAR				designerName[50];		//file designer name in clear
HWND				firstWin;				//first window not destroyed for exiting enumerate loop
RANGE				selectedFormsRange;		//range of selected forms
unsigned			tmpFormIndex;			//saved form index
unsigned char		cursorMask[128];		//cursor and mask
double				zoomMin;				//minimum allowed zoom value
fRECTANGLE			checkHoopRect;			//for checking the hoop size
BALSTCH*			ptrBalaradStitch;		//balarad stitch pointer
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
POINT*				formPointsAsLine;		//form point clipboard paste into form line
unsigned			lastFormSelected;		//end point of selected range of forms

#if	PESACT
unsigned char*		pesColors;				//pes colors
TCHAR*				ptrPesData;				//pes card data buffer
fPOINT				stitchCenterOffset;		//offset for writing pes files
PESTCH*				pesStitches;			//pes stitch buffer
unsigned char		pesEquivColors[16];		//pes equivalent colors
unsigned char*		pesStitch;					//pes stitches
unsigned			pesColorIndex;			//pes color index
#endif

POINT				endPointCross;			//point to draw at the end cross for form select points
#if	 __UseASM__
unsigned			fsizeof;				//size of form header divided by 4
#endif
HDC					hTraceDC;				//trace device context
HBITMAP				hTraceBitmap;			//trace bitmap
unsigned			sref;					//brightness of reference pixel
unsigned*			traceBitmap;			//trace bitmap data
POINT				currentTracePoint;		//current point being traced
unsigned			traceDataSize;			//size of the trace bitmap in double words
unsigned*			tracedPixels;			//bitmap of selected trace pixels
unsigned*			tracedEdges;			//detected edges of trace areas
TRCPNT*				tracedPoints;			//collection of traced points
TRCPNT*				decimatedLine;			//differenced collection of traced points
unsigned			traceDirection;			//trace direction
unsigned			initialDirection;		//initial trace direction
TRCPNT				traceDiff0;				//difference from one trace pixel to the next
TRCPNT				traceDiff1;				//difference from one trace pixel to the next
COLORREF			upColor;				//color of the up reference pixel
COLORREF			downColor;				//color of the down reference pixel
COLORREF			invertUpColor;			//complement color of the up reference pixel
COLORREF			invertDownColor;		//complement color of the down reference pixel
POINT				traceMsgPoint;			//message point for trace parsing
RECT				highRect;				//high trace mask rectangle
RECT				middleRect;				//middle trace mask rectangle
RECT				lowRect;				//low trace mask rectangle
unsigned			highColors[3];			//separated upper reference colors
unsigned			pixelColors[3];			//separated pixel reference colors
unsigned			lowColors[3];			//separated lower reference colors
unsigned			traceRGBFlag[]	= { TRCRED,TRCGRN,TRCBLU };	//trace bits
unsigned			traceRGBMask[]	= { REDMSK,GRNMSK,BLUMSK };	//trace masks
unsigned			traceRGB[]		= { BLUCOL,GRNCOL,REDCOL };	//trace colors
unsigned			traceAdjacentColors[9];	//separated colors for adjacent pixels
unsigned*			differenceBitmap;		//difference bitmap
TCHAR				traceInputBuffer[4];	//for user input color numbers
unsigned			columnColor;			//trace color column
POINT				bitpnt;					//a point on the bitmap

//cursors
HCURSOR 			hArrowCurs;				//arrow
HCURSOR 			hCrossCurs;				//cross
HCURSOR 			hFormCurs;				//form
HCURSOR 			hDlinCurs;				//dline
HCURSOR				hNeedleUpCurs;			//upright needle
HCURSOR				hNeedleLeftUpCurs;		//left up needle 
HCURSOR				hNeedleLeftDownCurs;	//left down needle 
HCURSOR				hNeedleRightUpCurs;		//right up needle 
HCURSOR				hNeedleRightDownCurs;	//right down needle 
CURSORMASK			cursorMasks;			//cursor mask structure

HPEN				linePen;				//line pen for stitch move lines			
HPEN				boxPen[4];				//box pens
HPEN				userPen[16];			//user color pens
HPEN				crossPen;				//pen for crosses in color windows
HPEN				groupSelectPen;			//pen for group select
HPEN				gridPen;				//pen for stitch grid
HPEN				backgroundPen;			//background color pen
HPEN				bitmapPen;				//bitmap pen
HPEN				formPen;				//form pen
HPEN				layerPen[6];			//layer pens
HPEN				formPen3px;				//three-pixel form pen
HPEN				formSelectedPen;		//form select pen
HPEN				zoomMarkPen;			//zoom mark pen
HPEN				multiFormPen;			//multiple selected forms pen
HPEN				selectAllPen;			//pen for drawing large boxes
HPEN				knotPen;				//knot pen
HPEN				blackPen;				//black pen

unsigned			backgroundPenWidth;		//width of the background pen

//brushes
HBRUSH				hBackgroundBrush;		//background color brush
HBRUSH				hDefaultColorBrush[16];	//default color brushes
HBRUSH				hUserColorBrush[16];	//user color brushes

//clipboard handle
unsigned			hClip = 0;				//pcs format
unsigned			hThrEdClip = 0;			//thred format

//for the choose color dialog box
CHOOSECOLOR			ccColorStruct;
COLORREF			customColor[16];
//for the background color dialog box
CHOOSECOLOR			ccBackgroundColorStruct;
COLORREF			customBackgroundColor[16];
//for the bitmap color dialog box
CHOOSECOLOR			ccBitMapColorStruct;
COLORREF			bitmapBackgroundColors[16];

TCHAR				threadSize[16][2];		//user selected thread sizes
unsigned			threadSizePixels[16];	//thread sizes in pixels
unsigned			threadSizeIndex[16];	//thread size indices

HMENU				hMainMenu;				//main menu
HMENU				hfillMenu;				//fill submenu
HMENU				hfileMenu;				//file submenu
HMENU				hBorderFillMenu;		//border fill submenu
HMENU				hViewMenu;				//view submenu
HMENU				hViewSetMenu;			//view/set
HMENU				hEditMenu;				//edit submenu
HWND				hDefaultColorWin[16];	//default color windows
HWND				hUserColorWin[16];		//user color windows
HWND				hThreadSizeWin[16];		//thread size windows
HWND				hChangeThreadSizeWin[3];	//thread size change windows
HWND				hMainStitchWin;			//stitch window
HWND				hVerticalScrollBar;		//vertical scroll bar
HWND				hHorizontalScrollBar;	//horizontal scroll bar
HWND				hButtonWin[9];			//button windows
HWND				hTraceStepWin;			//trace step window
HWND				hColorBar;				//color bar
HWND				hOKButton;				//ok button
HWND				hDiscardButton;			//discard button
HWND				hCancelButton;			//cancel button
HWND				hGeneralNumberInputBox;	//general number input box
HWND				hSpeedScrollBar;		//speed scroll bar for movie
HWND				hSideMessageWin = 0;	//main side message window
HWND				hsidWnd[16];			//side message windows
HWND				hPreferencesWindow;		//preferences window
HWND				hFormData = 0;			//form data sheet
HWND				hto;					//asking user if they want to delete the sitches associated with a form
HWND				hBackupViewer[OLDVER];	//handles of multiple file viewing windows
HWND				hTraceUp[3];			//trace up number windows
HWND				hTraceDown[3];			//trace down number windows

HWND				hTraceControl[3];		//trace control windows
HWND				hTraceSelect[3];		//trace select windows
unsigned			traceShift[] = { 0,8,16 };	//trace shift values
HBRUSH				hTraceBrush[3];			//red,green,and blue brushes
HWND				hTraceNumberInput;		//trace number input window
HBRUSH				hBlackBrush;			//black brush
HBRUSH				hTempBrush;				//temporary brush

COLORREF			userColor[16];			//user colors
COLORREF			backgroundColor;		//stich window background
COLORREF			boxColor[] = { 0x404040,0x408040,0x804040,0x404080 };
unsigned			activeColor = 0;		//active color selector
unsigned			buttonHeight;			//button height
unsigned			buttonWidth;			//button width
unsigned			buttonWidthX3;			//button width times 3
unsigned			numeralWidth;			//width of 0
int					threadWidthPixels[3];	//thread sizes in pixels
unsigned			DiplayedColorBitmap;	//color bitmap for recording colors on screen
double				gapToNearest[NERCNT];	//distances of the closest points
											//to a mouse click
long				nearestPoint[NERCNT];	//indices of the closest points
unsigned			nearestCount;			//number of boxes selected
unsigned			slpnt = 0;				//pointer for drawing stitch select lines
fRECTANGLE			stitchRangeRect;		//stitch range rectangle
fPOINT				stitchRangeSize;		//from check ranges
fPOINT				selectedFormsSize;		//size of multiple select rectangle
unsigned			moveAnchor;				//for resequencing stitches
double				rotateAnglePixels;		//angle for pixel rotate
SIZE				pickColorMsgSize;		//size of the pick color message
POINT				insertSize;				//size of file insert window
fPOINT				insertCenter;			//center point in inserted file
unsigned			numericCode;			//keyboard numerical input
double				lowestAngle;			//low angle for angle from mark
double				originalAngle;			//original angle for angle from mark
double				highestAngle;			//hi angle for angle from mark
unsigned			knots[MAXKNOTS];		//pointers to knots
unsigned			knotCount;				//number of knots in the design
unsigned			knotAttribute;			//knot stitch attribute
fPOINT				knotStep;				//knot step
TCHAR				knotAtStartOrder[] = { 2,3,1,4,0 };		//knot spacings
TCHAR				knotAtEndOrder[] = { -2,-3,-1,-4,0 };	//reverse knot spacings
TCHAR				knotAtLastOrder[] = { 0,-4,-1,-3,-2 };	//reverse knot spacings
fRECTANGLE			clipRectAdjusted;		//rectangle for adjust range ends for clipboard fills
TCHAR*				balaradFileName;		//used in the balarad interface
HANDLE				hBalaradFile;			//balarad file handle

//graphics variables		
double				aspectRatio = (double)LHUPX / LHUPY;	//aspect ratio of the stich window
SCROLLINFO			scrollInfo;				//scroll bar i/o structure
POINT				stitchWindowSize;		//size of the stich window in pixels
fPOINT				selectedPoint;			//for converting pixels coordinates
											// to stitch cordinates
fPOINT				zoomBoxOrigin;			//zoom box origin

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

long				boxOffset[4];

unsigned			defMap = 0xaf;		//bitmap for color number colors
unsigned			verticalIndex;		//vertical index, calculated from mouse click
unsigned			threadSizeSelected;	//thread selected for size change

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

const TCHAR		allFilter[_MAX_PATH + 1] = "Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Tajima (DST)\0*.dst\0";
const TCHAR		bmpFilter[_MAX_PATH + 1] = "Microsoft (BMP)\0*.bmp\0";
TCHAR			customFilter[_MAX_PATH + 1] = "Thredworks (THR)\0*.thr\0";
TCHAR			fileName[_MAX_PATH + 1] = { 0 };
TCHAR			thrName[_MAX_PATH + 1];
TCHAR			auxName[_MAX_PATH + 1];
TCHAR			geName[_MAX_PATH + 1];
TCHAR			defaultDirectory[_MAX_PATH + 1] = "c:\\";
TCHAR			defaultBMPDirectory[_MAX_PATH + 1] = "c:\\";
TCHAR			balaradName0[_MAX_PATH + 1] = { 0 };	//balarad semaphore file
TCHAR			balaradName1[_MAX_PATH + 1] = { 0 };	//balarad data file
TCHAR			balaradName2[_MAX_PATH + 1];
TCHAR			searchName[_MAX_PATH + 1];
TCHAR			homeDirectory[_MAX_PATH + 1];			//directory from which thred was executed
PCSTCH*			ptrFileBuffer;
HANDLE			hFile = 0;
HANDLE			hPcsFile = 0;
HANDLE			hIniFile = 0;
HANDLE			hInsertedFile;				//insert file handle
HANDLE			hBmp;						//bitmap handle
unsigned		fileSize;					//size of file
unsigned long	bytesRead;					//bytes actually read from file
unsigned		colorChanges;				//number of color changes
TCHAR			iniFileName[_MAX_PATH];		//.ini file name
TCHAR			pcsBMPFileName[16];			//bitmap file name from pcs file
TCHAR			usrBMPFileName[_MAX_PATH];	//bitmap file name from user load
OPENFILENAME	ofn = {

	sizeof(OPENFILENAME),	//lStructsize
	hWnd,					//hwndOwner 
	hInst,					//hInstance 
	allFilter,				//lpstrFilter 
	customFilter,			//lpstrCustomFilter 
	_MAX_PATH,				//nMaxCustFilter 
	0,						//nFilterIndex 
	fileName,				//lpstrFile 
	_MAX_PATH,				//nMaxFile 
	0,						//lpstrFileTitle 
	0,						//nMaxFileTitle 
	defaultDirectory,		//lpstrInitialDir
	0,						//lpstrTitle
	OFN_OVERWRITEPROMPT,	//Flags
	0,						//nFileOffset
	0,						//nFileExtension 
	"thr",					//lpstrDefExt 
	0,						//lCustData 
	0,						//lpfnHook
	0,						//lpTemplateName
};
TCHAR*				thumbnailNames;			//pointer to storage for thumbnail names
TCHAR**				ptrThumbnails;			//pointers to the thumbnail names
TCHAR*				thumbnailsSelected[4];	//pointers to thumbnails selected for display
unsigned			thumbnailCount;			//number of thumbnail file names
unsigned			thumbnailDisplayCount;	//number of thumbnail file names selected for display
unsigned			thumbnailIndex;			//index into the thumbnail filname table
TCHAR				thumbnailSearchString[32];	//storage for the thumnail search string
TCHAR				insertFileName[_MAX_PATH] = { 0 };	//insert file name
unsigned			insflt;					//saved float pointer for inserting files
unsigned			insertFileFormIndex;	//saved form pointer for inserting files
unsigned			insertFileStitchCount;	//saved stitch pointer for inserting files

OPENFILENAME obn = {
	sizeof(OPENFILENAME),	//lStructsize
	hWnd,					//hwndOwner 
	hInst,					//hInstance 
	bmpFilter,				//lpstrFilter 
	customFilter,			//lpstrCustomFilter 
	_MAX_PATH,				//nMaxCustFilter 
	0,						//nFilterIndex 
	usrBMPFileName,			//lpstrFile 
	_MAX_PATH,				//nMaxFile 
	0,						//lpstrFileTitle 
	0,						//nMaxFileTitle 
	defaultBMPDirectory,	//lpstrInitialDir 
	0,						//lpstrTitle
	OFN_OVERWRITEPROMPT,	//Flags
	0,						//nFileOffset
	0,						//nFileExtension 
	"bmp",					//lpstrDefExt 
	0,						//lCustData 
	0,						//lpfnHook
	0,						//lpTemplateName
};

BITMAPFILEHEADER	bitmapFileHeader;	//bitmap file header
BITMAPV4HEADER		bitmapFileHeaderV4;	//bitmap version4 file header
BITMAPINFO			bitmapInfo;			//bitmap info
BITMAPINFOHEADER	bitmapInfoHeader;	//bitmap info header

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

POINT		moveLine0[2];				//move point line
POINT		moveLine1[2];				//move point line
POINT		insertLine[3];				//the insert line
POINT		zoomBoxLine[5];				//the zoom box
POINT		clipInsertBoxLine[5];		//for displaying clipboard insert rectangle
POINT		rotateBoxOutline[5];		//for drawing the rotate rectangle
POINT		rotateBoxCrossVertLine[2];	//vertical part of the rotate cross
POINT		rotateBoxCrossHorzLine[2];	//horizontal part of the rotate cross
POINT		rotateBoxToCursorLine[2];	//line from the cursor to the center of the rotate cross
POINT		stretchBoxLine[5];			//stretch and expand

COLCHNG		colorChangeTable[MAXCHNG];
PCSHEADER	header;			//pcs file header
STREX		extendedHeader;	//thred file header extension
INIFILE		iniFile;		//initialization file
dPOINT		rotationCenter;	//center of stitch rotation
POINT		rotationCenterPixels;	//center of pixel rotation

unsigned	frmstrt;					//points to the first stitch in a form

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

DSTREC*		dstRecords;		//pointer to dst stitch records
unsigned	dstRecordCount;		//number of dst stitch records
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

unsigned			flagMap[MAPLEN];			//bitmap
unsigned			binaryVariableBitmap = 0;	//for storage of persistent binary variables set by the user

fPOINTATTRIBUTE		stitchBuffer[MAXPCS];		//main stitch buffer
fPOINTATTRIBUTE		clipBuffer[MAXSEQ];			//for temporary copy of imported clipboard data
FRMHED*				SelectedForm;			//pointer to selected form
unsigned			fillTypes[] =				//fill type array for side window display
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
		SetWindowText(hwnd, iniFile.designerName);
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
			GetWindowText(hwnd, iniFile.designerName, 50);
			EndDialog(hwndlg, 0);
			sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], iniFile.designerName);
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

	if (hFormData) {

		DestroyWindow(hFormData);
		hFormData = 0;
	}
}

void qchk() {

	if (chku(MARQ)) {

		CheckMenuItem(hMainMenu, ID_MARKESC, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_MARKQ, MF_UNCHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_MARKESC, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_MARKQ, MF_CHECKED);
	}
	setMap(DUMEN);
}

void nedmen() {

	if (chku(NEDOF)) {

		CheckMenuItem(hMainMenu, ID_SETNEDL, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_RSTNEDL, MF_CHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_SETNEDL, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_RSTNEDL, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void knotmen() {

	if (chku(KNOTOF)) {

		CheckMenuItem(hMainMenu, ID_KNOTON, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_KNOTOF, MF_CHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_KNOTON, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_KNOTOF, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void bsavmen() {

	if (chku(BSAVOF)) {

		CheckMenuItem(hMainMenu, ID_BSAVON, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_BSAVOF, MF_CHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_BSAVON, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_BSAVOF, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void linbmen() {

	if (chku(LINSPAC)) {

		CheckMenuItem(hMainMenu, ID_LINBEXACT, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_LINBEVEN, MF_CHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_LINBEXACT, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_LINBEVEN, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void wrnmen()
{
	unsigned cod;

	cod = MF_CHECKED;
	if (chku(WRNOF))
		cod = MF_UNCHECKED;
	CheckMenuItem(hMainMenu, ID_WARNOF, cod);
}

int datcod[] = { ID_CHKOF,ID_CHKON,ID_CHKREP,ID_CHKREPMSG };

void chkmen()
{
	int ind, cod;

	for (ind = 0; ind < (sizeof(datcod) / sizeof(int)); ind++)
	{
		cod = MF_UNCHECKED;
		if (ind == iniFile.dataCheck)
			cod = MF_CHECKED;
		CheckMenuItem(hMainMenu, datcod[ind], cod);
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
	pseudoRandomValue = rsed();
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
		if (fileNameOrder[ind] < 50) { extendedHeader.creatorName[fileNameOrder[ind]] = tnam[ind]; }
}

void redfnam(TCHAR* nam) {

	unsigned ind;
	unsigned char tnam[50];

	for (ind = 0; ind < 50; ind++)
		if (fileNameOrder[ind] < 50)
			tnam[ind] = extendedHeader.creatorName[fileNameOrder[ind]];
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
	pseudoRandomValue = NORDSED;
	for (ind = 0; ind < 100; ind++) {

		src = psg() % 50;
		dst = psg() % 50;
		tuns = fileNameOrder[dst];
		fileNameOrder[dst] = fileNameOrder[src];
		fileNameOrder[src] = tuns;
	}
	for (ind = 0; ind < 128; ind++)
		fnamcod[ind] = (unsigned char)ind + NCODOF;
	pseudoRandomValue = NCODSED;
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

	colorChanges = 0;
	if (header.stitchCount)
	{
		stitchBuffer[0].attribute &= NCOLMSK;
		stitchBuffer[0].attribute |= (stitchBuffer[1].attribute&COLMSK);
		stitchBuffer[header.stitchCount - 1].attribute &= NCOLMSK;
		stitchBuffer[header.stitchCount - 1].attribute |= (stitchBuffer[header.stitchCount - 2].attribute&COLMSK);
		ocol = stitchBuffer[0].attribute&COLMSK;
		for (ind = 1; ind < (unsigned)header.stitchCount - 1; ind++)
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
		for (ind = 0; ind < header.stitchCount; ind++)
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
				colorChangeTable[col].colorIndex = (unsigned char)tuns;
				colorChangeTable[col++].stitchIndex = (unsigned short)ind;
				ocol = tuns;
			}
		}
		colorChanges = col;
		colorChangeTable[col].stitchIndex = (unsigned short)ind;
		if (closestPointIndex > (unsigned)header.stitchCount - 1)
			closestPointIndex = header.stitchCount - 1;
		fndknt();
	}
}

void ladj() {

	unsigned ind;

	for (ind = 0; ind < 5; ind++) {

		if (ind == activeLayer)
			EnableMenuItem(hMainMenu, ind + M_ALL, MF_BYPOSITION | MF_GRAYED);
		else
			EnableMenuItem(hMainMenu, ind + M_ALL, MF_BYPOSITION | MF_ENABLED);
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

	if (undoBuffer[undoBufferWriteIndex])
		free(undoBuffer[undoBufferWriteIndex]);
	l_siz = sizeof(BAKHED) + sizeof(FRMHED)*formIndex + sizeof(fPOINTATTRIBUTE)*header.stitchCount
		+ sizeof(fPOINT)*(fltad + clpad) + sizeof(SATCON)*satkad + sizeof(COLORREF) * 16 +
		sizeof(TXPNT)*txad;
	undoBuffer[undoBufferWriteIndex] = calloc(l_siz,sizeof(unsigned));
	bdat = (BAKHED*)undoBuffer[undoBufferWriteIndex];
	if (bdat) {

		bdat->zoomRect.x = unzoomedRect.x;
		bdat->zoomRect.y = unzoomedRect.y;
		bdat->fcnt = formIndex;
		bdat->frmp = (FRMHED*)&bdat[1];
		//		for(ind=0;ind<formIndex;ind++)
		//			frmcpy(&bdat->frmp[ind],&formList[ind]);
		MoveMemory(bdat->frmp, &formList, sizeof(FRMHED)*formIndex);
		bdat->scnt = header.stitchCount;
		bdat->stch = (fPOINTATTRIBUTE*)&bdat->frmp[formIndex];
		if (header.stitchCount)
			stchcpy((sizeof(fPOINTATTRIBUTE)*header.stitchCount) >> 2, bdat->stch);
		bdat->fltcnt = fltad;
		bdat->flt = (fPOINT*)&bdat->stch[header.stitchCount];
		if (fltad)
			mvflpnt(bdat->flt, &formPoints[0], fltad);
		bdat->sacnt = satkad;
		bdat->sac = (SATCON*)&bdat->flt[fltad];
		if (satkad)
			mvsatk(bdat->sac, &satks[0], satkad);
		bdat->nclp = clpad;
		bdat->clipData = (fPOINT*)&bdat->sac[satkad];
		if (clpad) {

			if (clpad > MAXCLPNTS)
				clpad = MAXCLPNTS;
			mvflpnt(bdat->clipData, &clipboardPoints[0], clpad);
		}
		bdat->cols = (COLORREF*)&bdat->clipData[clpad];
		MoveMemory(bdat->cols, &userColor, sizeof(COLORREF) * 16);
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
			EnableMenuItem(hMainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
		}
		setMap(BAKACT);
		EnableMenuItem(hMainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
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
		if (GetMenuState(hfileMenu, LRUMenuId[ind], MF_BYCOMMAND) != -1)
			DeleteMenu(hfileMenu, LRUMenuId[ind], MF_BYCOMMAND);
	}
	for (ind = 0; ind < OLDNUM; ind++) {

		if (iniFile.prevNames[ind][0]) {

			if (chkMap(SAVAS))
				AppendMenu(hfileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[ind], iniFile.prevNames[ind]);
			else {

				hndl = FindFirstFile(iniFile.prevNames[ind], &fdat);
				if (hndl == INVALID_HANDLE_VALUE)
					iniFile.prevNames[ind][0] = 0;
				else {

					AppendMenu(hfileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[ind], iniFile.prevNames[ind]);
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

	_strlwr_s(fileName);
	pext = strrchr(fileName, '.');
	if (pext)
		pext++;
	else
		pext = &fileName[strlen(fileName)];
	ind = pext - fileName;
	strncpy_s(auxName, fileName, ind);
	strncpy_s(thrName, fileName, ind);
	strncpy_s(geName, fileName, ind);
	pext = auxName + ind;
	switch (iniFile.auxFileType) {

	case AUXDST:

		strcpy_s(pext, sizeof(auxName) - ind, "dst");
		break;

#if PESACT

	case AUXPES:

		strcpy_s(pext, sizeof(auxName) - ind, "pes");
		break;

#endif

	default:

		strcpy_s(pext, sizeof(auxName) - ind, "pcs");
	}
	pext = thrName + ind;
	strcpy_s(pext, sizeof(thrName) - ind, "thr");
	pext = geName + ind;
	strcpy_s(pext, sizeof(geName) - ind, "th*");
	for (ind = 0; ind < OLDNUM; ind++) {

		if (!strcmp(iniFile.prevNames[ind], thrName)) {

			if (ind) {

				strcpy_s(tnam, sizeof(tnam), iniFile.prevNames[0]);
				strcpy_s(iniFile.prevNames[0], sizeof(iniFile.prevNames[0]), iniFile.prevNames[ind]);
				strcpy_s(iniFile.prevNames[ind], sizeof(iniFile.prevNames[ind]), tnam);
				goto mendun;
			} else
				goto nomen;
		}
	}
	for (ind = 0; ind < OLDNUM; ind++) {

		if (!iniFile.prevNames[ind][0]) {

			strcpy_s(iniFile.prevNames[ind], thrName);
			goto mendun;
		}
	}
	strcpy_s(iniFile.prevNames[3], iniFile.prevNames[2]);
	strcpy_s(iniFile.prevNames[2], iniFile.prevNames[1]);
	strcpy_s(iniFile.prevNames[1], iniFile.prevNames[0]);
	strcpy_s(iniFile.prevNames[0], thrName);
mendun:;
	redfils();
nomen:;
}

void moveStitchPoints(fPOINTATTRIBUTE* dst, fPOINTATTRIBUTE* src) {

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

	unsigned			ind;
	unsigned short		dst;
	double				len;
	fPOINTATTRIBUTE*	l_pnt;

	if (selectedFormCount) {

		clRmap(RMAPSIZ);
		for (ind = 0; ind < selectedFormCount; ind++)
			setr(selectedFormList[ind]);
		rstMap(CONTIG);
		dst = 0;
		l_pnt = stitchBuffer;
		for (ind = 0; ind < header.stitchCount; ind++) {

			if (stitchBuffer[ind].attribute&TYPMSK&&chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {

				if (setMap(CONTIG)) {

					len = hypot(stitchBuffer[ind].x - l_pnt->x, stitchBuffer[ind].y - l_pnt->y);
					if (len > minStitchLength) {

						l_pnt = &stitchBuffer[ind];
						moveStitchPoints(&stitchBuffer[dst++], &stitchBuffer[ind]);
					}
				} else
					l_pnt = &stitchBuffer[ind];
			} else {

				moveStitchPoints(&stitchBuffer[dst++], &stitchBuffer[ind]);
				rstMap(CONTIG);
			}
		}
		header.stitchCount = dst;
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		delsmal(groupStartStitch, groupEndStitch);
	} else
		delsmal(0, header.stitchCount);
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
	POINT			slin[MAXSEQ];	//stitch select line

	SelectObject(dc, groupSelectPen);
	SetROP2(stitchWindowDC, R2_NOTXORPEN);
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

	if (closestPointIndex > (unsigned)header.stitchCount - 1)
		closestPointIndex = header.stitchCount - 1;
	if (groupStitchIndex > header.stitchCount)
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
		for (ind = 0; ind < (unsigned)header.stitchCount - 1; ind++) {

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

		if (header.stitchCount > 1) {

			if (chkMap(FORMSEL)) {

				frmcalc();
				butxt(HCOR, "");
				return;
			}
			rngadj();
			if (chkMap(GRPSEL) && groupStartStitch != groupEndStitch)
				lenfn(groupStartStitch, groupEndStitch);
			else
				lenfn(0, header.stitchCount - 1);
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
	rlin[0].x = rlin[6].x = rlin[7].x = rlin[8].x = stitchRangeRect.left;
	rlin[1].x = rlin[5].x = midl(stitchRangeRect.right, stitchRangeRect.left);
	rlin[0].y = rlin[1].y = rlin[2].y = rlin[8].y = stitchRangeRect.top;
	rlin[3].y = rlin[7].y = midl(stitchRangeRect.top, stitchRangeRect.bottom);
	rlin[4].y = rlin[5].y = rlin[6].y = stitchRangeRect.bottom;
	rlin[2].x = rlin[3].x = rlin[4].x = stitchRangeRect.right;
	for (ind = 0; ind < 9; ind++)
		sfCor2px(rlin[ind], &formOutlineRectangle[ind]);
}

void grpAdj() {

	POINT		nusiz;
	double		tdub;

	uncros();
	unsel();
	rngadj();
	ducros(stitchWindowDC);
	lenCalc();
	selRct(&stitchRangeRect);
	if (chkMap(ZUMED) && groupEndStitch != groupStartStitch) {

		if (stitchRangeRect.top > zoomRect.top - 1 || stitchRangeRect.bottom < zoomRect.bottom - 1
			|| stitchRangeRect.left<zoomRect.left + 1 || stitchRangeRect.right>zoomRect.right - 1) {

			nusiz.x = stitchRangeRect.right - stitchRangeRect.left;
			nusiz.y = stitchRangeRect.top - stitchRangeRect.bottom;
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
				selectedPoint.x = ((stitchRangeRect.right - stitchRangeRect.left) / 2) + stitchRangeRect.left;
				selectedPoint.y = ((stitchRangeRect.top - stitchRangeRect.bottom) / 2) + stitchRangeRect.bottom;
				shft(selectedPoint);
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
	ducros(stitchWindowDC);
	lenCalc();
	selRct(&stitchRangeRect);
	if (stitchRangeRect.top > zoomRect.top - 1 || stitchRangeRect.bottom < zoomRect.bottom - 1
		|| stitchRangeRect.left<zoomRect.left + 1 || stitchRangeRect.right>zoomRect.right - 1) {

		selectedPoint.x = ((stitchRangeRect.right - stitchRangeRect.left) / 2) + stitchRangeRect.left;
		selectedPoint.y = ((stitchRangeRect.top - stitchRangeRect.bottom) / 2) + stitchRangeRect.bottom;
		shft(selectedPoint);
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

	GetClientRect(hWnd, &mainWindowRect);
	GetWindowRect(hColorBar, &colorBarRect);
	GetWindowRect(hButtonWin[HMINLEN], &minLenRect);
	GetWindowRect(hButtonWin[HMAXLEN], &maxLenRect);
	ReleaseDC(hColorBar, colorBarDC);
	colorBarDC = GetDC(hColorBar);
	DeleteDC(stitchWindowMemDC);
	ReleaseDC(hMainStitchWin, stitchWindowDC);
	DeleteObject(stitchWindowBmp);
	ReleaseDC(hMainStitchWin, stitchWindowDC);
	stitchWindowDC = GetDCEx(hMainStitchWin, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	DeleteDC(stitchWindowMemDC);
	stitchWindowMemDC = CreateCompatibleDC(stitchWindowDC);
	GetDCOrgEx(stitchWindowDC, &stitchWindowOrigin);
	ReleaseDC(hWnd, mainDC);
	mainDC = GetDC(hWnd);
	SetStretchBltMode(mainDC, COLORONCOLOR);
	GetDCOrgEx(mainDC, &mainWindowOrigin);
	GetWindowRect(hMainStitchWin, &stitchWindowAbsRect);
	GetClientRect(hMainStitchWin, &stitchWindowClientRect);
	stitchWindowBmp = CreateCompatibleBitmap(stitchWindowDC, stitchWindowClientRect.right, stitchWindowClientRect.bottom);
	SelectObject(stitchWindowMemDC, stitchWindowBmp);
}

HPEN nuPen(HPEN pen, unsigned wid, COLORREF col) {

	DeleteObject(pen);
	return CreatePen(PS_SOLID, wid, col);
}

void nuStchSiz(unsigned ind, unsigned wid) {

	if (wid != threadSizePixels[ind]) {

		userPen[ind] = nuPen(userPen[ind], wid, userColor[ind]);
		threadSizePixels[ind] = wid;
	}
}

HBRUSH nuBrush(HBRUSH brsh, COLORREF col) {

	DeleteObject(brsh);
	return CreateSolidBrush(col);
}

void box(unsigned ind, HDC dc) {

	long pwid = boxOffset[ind];
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

	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	for (ind = 0; ind < nearestCount; ind++) {

		SelectObject(stitchWindowDC, boxPen[ind]);
		box(ind, stitchWindowDC);
	}
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void dubx() {

	POINT lin[5];
	long pwid = boxOffset[0];

	SelectObject(stitchWindowMemDC, boxPen[0]);
	SelectObject(stitchWindowDC, boxPen[0]);
	SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
	SetROP2(stitchWindowDC, R2_NOTXORPEN);
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
	Polyline(stitchWindowMemDC, lin, 5);
	Polyline(stitchWindowDC, lin, 5);
	SetROP2(stitchWindowMemDC, R2_COPYPEN);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void duar() {

	POINT rpnt;

	stitchArrow[1].x = stitchSizePixels.x;
	stitchArrow[1].y = stitchSizePixels.y;
	rotationCenterPixels.x = stitchSizePixels.x;
	rotationCenterPixels.y = stitchSizePixels.y;
	rpnt.x = stitchSizePixels.x - 10;
	rpnt.y = stitchSizePixels.y + 10;
	rotpix(rpnt, &stitchArrow[0]);
	rpnt.y = stitchSizePixels.y - 10;
	rotpix(rpnt, &stitchArrow[2]);
	SelectObject(stitchWindowMemDC, boxPen[0]);
	SelectObject(stitchWindowDC, boxPen[0]);
	SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	Polyline(stitchWindowMemDC, stitchArrow, 3);
	Polyline(stitchWindowDC, stitchArrow, 3);
	SetROP2(stitchWindowMemDC, R2_COPYPEN);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void dubox() {

	rotateAnglePixels = atan2(stitchBuffer[closestPointIndex + 1].y - stitchBuffer[closestPointIndex].y, stitchBuffer[closestPointIndex + 1].x - stitchBuffer[closestPointIndex].x);
	duar();
	rstMap(ELIN);
	setMap(SELBOX);
	rstMap(FRMPSEL);
	redraw(hColorBar);
	if (!bufferIndex)
		ritnum(STR_NUMSEL, closestPointIndex);
}

void unbox() {

	if (rstMap(SELBOX)) {

		SelectObject(stitchWindowDC, boxPen[0]);
		SetROP2(stitchWindowDC, R2_NOTXORPEN);
		Polyline(stitchWindowDC, stitchArrow, 3);
		SetROP2(stitchWindowDC, R2_COPYPEN);
	}
}

void unboxs() {

	if (nearestCount) {

		boxs();
		nearestCount = 0;
	}
}

void redraw(HWND dWnd) {

	unsigned ind;

	RedrawWindow(dWnd, NULL, NULL, RDW_INVALIDATE);
	if (dWnd == hMainStitchWin) {

		for (ind = 0; ind < 16; ind++) {

			if (hDefaultColorWin[ind])
				RedrawWindow(hDefaultColorWin[ind], NULL, NULL, RDW_INVALIDATE);
		}
		RedrawWindow(hColorBar, NULL, NULL, RDW_INVALIDATE);
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
				stch2px1(header.stitchCount - 1);
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

		strcpy_s(defaultDirectory, fNam);
		last = strrchr(defaultDirectory, '\\');
		if (last - defaultDirectory == 2)
			last[1] = 0;
		else
			last[0] = 0;
	}
}

void defbNam() {

	TCHAR* last;

	if (usrBMPFileName[0]) {

		strcpy_s(defaultBMPDirectory, usrBMPFileName);
		last = strrchr(defaultBMPDirectory, '\\');
		if (last - defaultBMPDirectory == 2)
			last[1] = 0;
		else
			last[0] = 0;
	}
}

void ritini() {

	unsigned	ind;
	RECT		wrct;

	strcpy_s(iniFile.defaultDirectory, defaultDirectory);
	for (ind = 0; ind < 16; ind++) {

		iniFile.stitchColors[ind] = userColor[ind];
		iniFile.backgroundPreferredColors[ind] = customBackgroundColor[ind];
		iniFile.stitchPreferredColors[ind] = customColor[ind];
		iniFile.bitmapBackgroundColors[ind] = bitmapBackgroundColors[ind];
	}
	iniFile.backgroundColor = backgroundColor;
	iniFile.bitmapColor = bitmapColor;
	iniFile.minStitchLength = minStitchLength;
	iniFile.showStitchThreshold = showStitchThreshold;
	iniFile.threadSize30 = threadSize30;
	iniFile.threadSize40 = threadSize40;
	iniFile.threadSize60 = threadSize60;
	iniFile.userStitchLength = userStitchLength;
	iniFile.smallStitchLength = smallStitchLength;
	iniFile.stitchBoxesThreshold = stitchBoxesThreshold;
	iniFile.stitchSpace = stitchSpace;
	iniFile.binaryVariableBitmap = binaryVariableBitmap;
	iniFile.borderWidth = borderWidth;
	iniFile.underlayColor = underlayColor;
	iniFile.snapLength = snapLength;
	iniFile.starRatio = starRatio;
	iniFile.spiralWrap = spiralWrap;
	iniFile.buttonholeFillCornerLength = buttonholeFillCornerLength;
	iniFile.picotSpace = picotSpace;
	if (!chku(SAVMAX)) {

		GetWindowRect(hWnd, &wrct);
		iniFile.initialWindowCoords.left = wrct.left;
		iniFile.initialWindowCoords.right = wrct.right;
		iniFile.initialWindowCoords.bottom = wrct.bottom;
		iniFile.initialWindowCoords.top = wrct.top;
	}
	hIniFile = CreateFile(iniFileName, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hIniFile != INVALID_HANDLE_VALUE)
		WriteFile(hIniFile, &iniFile, sizeof(INIFILE), &bytesRead, NULL);
	CloseHandle(hIniFile);
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

	ptrBalaradStitch[dst].flag = 0;
	ptrBalaradStitch[dst].code = (unsigned char)cod;
	ptrBalaradStitch[dst].x = (stitchBuffer[src].x - balaradOffset.x)*BALRAT;
	ptrBalaradStitch[dst].y = (stitchBuffer[src].y - balaradOffset.y)*BALRAT;
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
	stitchBuffer[dst].x = ptrBalaradStitch[src].x*IBALRAT + balaradOffset.x;
	stitchBuffer[dst].y = ptrBalaradStitch[src].y*IBALRAT + balaradOffset.y;
}

unsigned colmatch(COLORREF col) {

	unsigned ind, dis, mindis, pdis = 0;

	if (colorChanges < 16) {

		for (ind = 0; ind < colorChanges; ind++) {

			if (col == userColor[ind])
				return ind;
		}
		userColor[colorChanges++] = col;
		return colorChanges - 1;
	} else {

		mindis = 0xffffffff;
		for (ind = 0; ind < colorChanges; ind++) {

			dis = coldis(col, userColor[ind]);
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

	header.stitchCount = 0;
	formIndex = 0;
	btfil = CreateFile(balaradName2, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (btfil != INVALID_HANDLE_VALUE) {

		ReadFile(btfil, (BALHED*)&l_bhed, sizeof(BALHED), &l_red, 0);
		if (l_red == sizeof(BALHED)) {

			ptrBalaradStitch = (BALSTCH*)&bseq;
			ReadFile(btfil, (BALSTCH*)ptrBalaradStitch, sizeof(bseq), &l_red, 0);
			bcnt = l_red / sizeof(BALSTCH);
			iniFile.backgroundColor = backgroundColor = l_bhed.backgroundColor;
			backgroundPen = nuPen(backgroundPen, 1, backgroundColor);
			backgroundPenWidth = 1;
			DeleteObject(hBackgroundBrush);
			hBackgroundBrush = CreateSolidBrush(backgroundColor);
			unzoomedRect.x = iniFile.hoopSizeX = l_bhed.hoopSizeX*IBALRAT;
			unzoomedRect.y = iniFile.hoopSizeY = l_bhed.hoopSizeY*IBALRAT;
			balaradOffset.x = iniFile.hoopSizeX / 2;
			balaradOffset.y = iniFile.hoopSizeY / 2;
			iniFile.hoopType = header.hoopType = CUSTHUP;
			userColor[0] = l_bhed.color[0];
			col = 0;
			pbcol = 1;
			ine = 0;
			colorChanges = 1;
			for (ind = 0; ind < bcnt; ind++) {

				switch (ptrBalaradStitch[ind].code) {

				case BALNORM:

					bal2thr(ine++, ind, col);
					break;

				case BALSTOP:

					col = colmatch(l_bhed.color[pbcol++]);
					break;
				}
			}
			for (ind = 0; ind < colorChanges; ind++) {

				userPen[ind] = CreatePen(PS_SOLID, 1, userColor[ind]);
				hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], userColor[ind]);
			}
			header.stitchCount = ine;
			coltab();
			redraw(hColorBar);
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

	if (*balaradName0&&*balaradName1&&header.stitchCount) {

		if (!*fileName) {

			strcpy_s(fileName, defaultDirectory);
			strcat_s(fileName, "\\balfil.thr");
		}
		strcpy_s(onam, fileName);
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
		l_bhed.color[0] = userColor[col];
		ine = 1;
		for (ind = 1; ind < header.stitchCount; ind++) {

			if (col != (stitchBuffer[ind].attribute&COLMSK)) {

				col = stitchBuffer[ind].attribute&COLMSK;
				l_bhed.color[ine++] = userColor[col];
				if (ine & 0xffffff00)
					break;
			}
		}
		l_bhed.signature = 'drbm';
		l_bhed.backgroundColor = backgroundColor;
		l_bhed.hoopSizeX = iniFile.hoopSizeX*BALRAT;
		l_bhed.hoopSizeY = iniFile.hoopSizeY*BALRAT;
		WriteFile(bfil, (BALHED*)&l_bhed, sizeof(BALHED), &wrot, 0);
		balaradOffset.x = iniFile.hoopSizeX / 2;
		balaradOffset.y = iniFile.hoopSizeY / 2;
		ptrBalaradStitch = (BALSTCH*)&bseq;
		col = stitchBuffer[0].attribute&COLMSK;
		thr2bal(0, 0, BALJUMP);
		ptrBalaradStitch[1].flag = (unsigned char)col;
		ine = 1;
		for (ind = 0; ind < header.stitchCount; ind++) {

			thr2bal(ine++, ind, BALNORM);
			if ((stitchBuffer[ind].attribute&COLMSK) != col) {

				thr2bal(ine, ind, BALSTOP);
				col = stitchBuffer[ind].attribute&COLMSK;
				ptrBalaradStitch[ine++].flag = (unsigned char)col;
			}
		}
		WriteFile(bfil, (BALSTCH*)ptrBalaradStitch, ine * sizeof(BALSTCH), &wrot, 0);
		CloseHandle(bfil);
		bfil = CreateFile(balaradName1, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		WriteFile(bfil, (TCHAR*)onam, strlen(onam) + 1, &wrot, 0);
		CloseHandle(bfil);
	} else {

		if (*balaradName1) {

			bfil = CreateFile(balaradName1, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			CloseHandle(bfil);
		}
	}
	if (*balaradName0)
		DeleteFile(balaradName0);
}

void savmsg() {

	TCHAR	buf[HBUFSIZ];

	LoadString(hInst, IDS_SAVFIL, buf, HBUFSIZ);
	sprintf_s(msgbuf, sizeof(msgbuf), buf, thrName);
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

		DestroyWindow(hPreferencesWindow);
		rstMap(WASRT);
	}
	unsid();
	unbsho();
	rstAll();
	//	if(savcmp()||(*balaradName0&&*balaradName1&&header.stitchCount&&!formIndex))
	if (savcmp() || (*balaradName0))
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
		hSideMessageWin,
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
	GetWindowRect(hFormData, &mrct);
	ispcdclp();
	if (chkMap(FILTYP)) {

		for (ind = 0; ind < EDGETYPS + 1; ind++) {

			if ((unsigned)(SelectedForm->edgeType&NEGUND) == edgeFillTypes[ind])
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
		hSideMessageWin = CreateWindow(
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

			if ((unsigned)(SelectedForm->edgeType&NEGUND) != edgeFillTypes[ind]) {

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
				sideWindowSize.x = buttonWidthX3;
				sideWindowSize.y = buttonHeight;
			} else {

				for (ind = 0; ind < cnt; ind++) {

					if ((1 << fillTypes[ind])&clipTypeMap) {

						if (chkMap(WASPCDCLP))
							maxtsiz(pstr[ind], &sideWindowSize);
						else
							cnt1--;
					} else {

						if (fillTypes[ind] == SelectedForm->fillType)
							cnt1--;
						else
							maxtsiz(pstr[ind], &sideWindowSize);
					}
				}
			}
		}
		hSideMessageWin = CreateWindow(
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

					if (featherFillTypes[ind] != SelectedForm->fillInfo.feather.fillType)
						dusid(ind);
				}
			} else {

				for (ind = 0; ind < cnt; ind++) {

					if (fillTypes[ind] != SelectedForm->fillType) {

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

	aspectRatio = (double)unzoomedRect.x / unzoomedRect.y;
	if (chkMap(RUNPAT) || chkMap(WASPAT))
		stitchWindowSize.x = (long)(mainWindowRect.bottom - (SCROLSIZ << 1))*aspectRatio;
	else
		stitchWindowSize.x = (long)(mainWindowRect.bottom - SCROLSIZ)*aspectRatio;

	if ((stitchWindowSize.x + (long)buttonWidthX3 + RIGHTSIZ) < mainWindowRect.right) {

		if (chkMap(RUNPAT) || chkMap(WASPAT))
			stitchWindowSize.y = mainWindowRect.bottom - (SCROLSIZ << 1);
		else
			stitchWindowSize.y = mainWindowRect.bottom - SCROLSIZ;
	} else {

		stitchWindowSize.x = (mainWindowRect.right - buttonWidthX3 - COLSIZ);
		stitchWindowSize.y = mainWindowRect.bottom - mainWindowRect.top;
		if ((double)stitchWindowSize.x / stitchWindowSize.y > aspectRatio)
			stitchWindowSize.x = stitchWindowSize.y*aspectRatio;
		else
			stitchWindowSize.y = stitchWindowSize.x / aspectRatio;
	}
}

void movStch() {

	POINT		psiz;
	long		vof = 0;
	long		tstchy = stitchWindowSize.y;

	psiz.x = mainWindowRect.right - buttonWidthX3 - RIGHTSIZ;
	psiz.y = mainWindowRect.bottom;
	unboxs();
	if (chkMap(RUNPAT) || chkMap(WASPAT)) {

		vof = SCROLSIZ;
		psiz.y -= SCROLSIZ;
		tstchy -= SCROLSIZ;
	}
	if (chkMap(ZUMED)) {

		psiz.y -= SCROLSIZ;
		MoveWindow(hMainStitchWin, buttonWidthX3, vof, psiz.x, psiz.y, FALSE);
		MoveWindow(hVerticalScrollBar, buttonWidthX3 + psiz.x, 0, SCROLSIZ, psiz.y, TRUE);
		MoveWindow(hHorizontalScrollBar, buttonWidthX3, psiz.y + vof, psiz.x, SCROLSIZ, TRUE);
		stitchWindowAspectRatio = (double)psiz.x / psiz.y;
		if (chkMap(RUNPAT) || chkMap(WASPAT))
			MoveWindow(hSpeedScrollBar, buttonWidthX3, 0, psiz.x, SCROLSIZ, TRUE);
		ShowWindow(hVerticalScrollBar, TRUE);
		ShowWindow(hHorizontalScrollBar, TRUE);
	} else {

		stchPars();
		tstchy = stitchWindowSize.y + SCROLSIZ;
		MoveWindow(hMainStitchWin, buttonWidthX3, vof, stitchWindowSize.x, tstchy, TRUE);
		ShowWindow(hVerticalScrollBar, FALSE);
		ShowWindow(hHorizontalScrollBar, FALSE);
		stitchWindowAspectRatio = (double)stitchWindowSize.x / tstchy;
		if (chkMap(RUNPAT) || chkMap(WASPAT))
			MoveWindow(hSpeedScrollBar, buttonWidthX3, 0, stitchWindowSize.x, SCROLSIZ, TRUE);
	}
	MoveWindow(hColorBar, mainWindowRect.right - COLSIZ, 0, COLSIZ, mainWindowRect.bottom, TRUE);
	nuRct();
	redraw(hColorBar);
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
	selectedPoint.x = zoomRect.left + l_siz.x;
	selectedPoint.y = zoomRect.bottom + l_siz.y;
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
			iniFile.rotationAngle = rotationAngle;
		} else
			rotationAngle = iniFile.rotationAngle;
		duprot();
		return;
	}
	if (rstMap(NUROT)) {

		if (tdub) {

			rotationAngle = tdub*PI / 180;
			iniFile.rotationAngle = rotationAngle;
		} else
			rotationAngle = iniFile.rotationAngle;
		return;
	}
	if (rstMap(ENTRSEG)) {

		if (tdub) {

			rotationAngle = 2 * PI / tdub;
			iniFile.rotationAngle = rotationAngle;
		} else
			rotationAngle = iniFile.rotationAngle;
		return;
	}
	if (rstMap(ENTROT)) {

		if (tdub) {

			rotationAngle = tdub*PI / 180;
			iniFile.rotationAngle = rotationAngle;
		} else
			rotationAngle = iniFile.rotationAngle;
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
				SelectedForm->txof = tdub;
				break;

			case LUANG:

				savdo();
				SelectedForm->underlayStitchAngle = tdub / 180 * PI / PFGRAN;
				break;

			case LUSPAC:

				savdo();
				SelectedForm->underlaySpacing = tdub;
				break;

			case LWLKIND:

				savdo();
				SelectedForm->underlayIndent = tdub;
				break;

			case LULEN:

				savdo();
				SelectedForm->underlayStitchLen = tdub;
				break;

			case LDSTRT:

				savdo();
				SelectedForm->fillStart = tdub / PFGRAN;
				SelectedForm->fillStart %= sides;
				break;

			case LDEND:

				savdo();
				SelectedForm->fillEnd = tdub / PFGRAN;
				SelectedForm->fillEnd %= sides;
				break;

			case LFTHUPCNT:

				savdo();
				tdub /= PFGRAN;
				if (tdub > 255)
					tdub = 255;
				SelectedForm->fillInfo.feather.upCount = tdub;
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
					SelectedForm->underlayColor = (atoi(sideWindowEntryBuffer) - 1) & 0xf;
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
				SelectedForm->edgeSpacing = tdub;
				refil();
				return;

			case LFRMFAZ:

				savdo();
				SelectedForm->wordParam = floor(tdub) / PFGRAN;
				unsid();
				refil();
				return;

			case LBRDPOS:

				savdo();
				SelectedForm->edgeStitchLen = tdub / PFGRAN;
				unsid();
				refil();
				return;

			case LMAXFIL:

				savdo();
				SelectedForm->maxFillStitchLen = tdub;
				unsid();
				refil();
				return;

			case LMINFIL:

				savdo();
				SelectedForm->minFillStitchLen = tdub;
				unsid();
				refil();
				return;

			case LMAXBRD:

				savdo();
				SelectedForm->maxBorderStitchLen = tdub;
				unsid();
				refil();
				return;

			case LMINBRD:

				savdo();
				SelectedForm->minBorderStitchLen = tdub;
				unsid();
				refil();
				return;
			}
			if (formMenuChoice == LBCSIZ) {

				savdo();
				if (SelectedForm->edgeType == EGHOL)
					savblen((float)tdub);
				else
					savplen((float)tdub);
			} else {

				if (tdub) {

					switch (formMenuChoice) {

					case LFTHSIZ:

						savdo();
						SelectedForm->fillInfo.feather.ratio = tdub / PFGRAN;
						break;

					case LFTHNUM:

						savdo();
						SelectedForm->fillInfo.feather.count = tdub / PFGRAN;
						break;

					case LFTHFLR:

						savdo();
						SelectedForm->fillInfo.feather.minStitchSize = tdub;
						break;

					case LFTHDWNCNT:

						savdo();
						tdub /= PFGRAN;
						if (tdub > 255)
							tdub = 255;
						SelectedForm->fillInfo.feather.downCount = tdub;
						break;

					case LFRMSPAC:

						savdo();
						SelectedForm->fillSpacing = tdub;
						break;

					case LFRMLEN:

						savdo();
						SelectedForm->lengthOrCount.stitchLength = tdub;
						break;

					case LBRDSPAC:

						savdo();
						tuns = SelectedForm->edgeType&NEGUND;
						switch (tuns) {

						case EGPRP:

							SelectedForm->edgeSpacing = tdub;
							break;

						case EGCHNH:
						case EGCHNL:

							SelectedForm->edgeSpacing = tdub;
							break;

						default:

							SelectedForm->edgeSpacing = tdub / 2;
						}
						break;

					case LBRDLEN:

						savdo();
						SelectedForm->edgeStitchLen = tdub;
						break;

					case LBRDSIZ:

						savdo();
						SelectedForm->borderSize = tdub;
						break;

					case LFRMANG:

						savdo();
						SelectedForm->angleOrClipData.angle = tdub / 180 * PI / PFGRAN;
						break;

					case LSACANG:

						savdo();
						SelectedForm->satinOrAngle.angle = tdub / 180 * PI / PFGRAN;
						break;

					case LAPCOL:

						savdo();
						SelectedForm->borderColor &= 0xf;
						tuns = tdub / 6;
						if (tuns)
							tuns--;
						tuns &= 0xf;
						SelectedForm->borderColor |= (tuns << 4);
						break;
					}
				} else {

					savdo();
					if (formMenuChoice == LFRMSPAC&&isfclp())
						SelectedForm->fillSpacing = 0;
				}
			}
			unsid();
			refil();
		} else {

			if (preferenceIndex) {

				tdub = atof(sideWindowEntryBuffer);
				switch (preferenceIndex - 1) {

				case PEG:

					iniFile.eggRatio = tdub;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
					SetWindowText(thDat[PEG], msgbuf);
					break;

				case PNUDG:

					iniFile.cursorNudgeStep = tdub;
					iniFile.nudgePixels = pxchk(tdub);
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
					SetWindowText(thDat[PNUDG], msgbuf);
					break;

				case PPIC:

					picotSpace = tdub*PFGRAN;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
					SetWindowText(thDat[PPIC], msgbuf);
					break;

				case PCLPOF:

					iniFile.clipboardOffset = tdub*PFGRAN;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", tdub);
					SetWindowText(thDat[PCLPOF], msgbuf);
					break;

				case PFAZ:

					iniFile.fillPhase = floor(tdub);
					sprintf_s(msgbuf, sizeof(msgbuf), "%d", iniFile.fillPhase);
					SetWindowText(thDat[PFAZ], msgbuf);
					break;

				case PCHRAT:

					iniFile.chainRatio = tdub;
					sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.chainRatio);
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

							stitchSpace = tdub*PFGRAN;
							SetWindowText(thDat[PSPAC], msgbuf);
							break;

						case PANGL:

							iniFile.fillAngle = tdub / 180 * PI;
							SetWindowText(thDat[PANGL], msgbuf);
							break;

						case PSAT:

							borderWidth = tdub*PFGRAN;
							iniFile.borderWidth = borderWidth;
							SetWindowText(thDat[PSAT], msgbuf);
							break;

						case PMAX:

							iniFile.maxStitchLength = tdub*PFGRAN;
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

							iniFile.hoopSizeX = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", tdub);
							SetWindowText(thDat[PHUPX], msgbuf);
							sethup();
							prfmsg();
							chkhup();
							break;

						case PHUPY:

							iniFile.hoopSizeY = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", tdub);
							SetWindowText(thDat[PHUPY], msgbuf);
							sethup();
							prfmsg();
							chkhup();
							break;

						case PGRD:

							iniFile.gridSize = tdub*PFGRAN;
							sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", tdub);
							SetWindowText(thDat[PGRD], msgbuf);
							break;

						case PCHN:

							iniFile.chainSpace = tdub*PFGRAN;
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
					iniFile.clipboardOffset = tdub*PFGRAN;
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
					iniFile.underlayAngle = tdub / 180 * PI;
				if (rstMap(GTUSPAC))
				{
					if (tdub)
						iniFile.underlaySpacing = tdub*PFGRAN;
				}
				if (rstMap(GTWLKIND))
					iniFile.underlayIndent = tdub*PFGRAN;
				if (rstMap(GTWLKLEN))
				{
					if (tdub)
						iniFile.underlayStitchLen = tdub*PFGRAN;
				}
				if (rstMap(PIXIN))
					iniFile.nudgePixels = pxchk(tdub);
				if (rstMap(STPXIN))
					iniFile.stitchSizePixels = pxchk(tdub);
				if (rstMap(FRMPXIN))
					iniFile.formPointSizePixels = tdub;
				if (rstMap(FRMBOXIN))
					iniFile.formBoxSizePixels = tdub;
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
	DestroyWindow(hOKButton);
	DestroyWindow(hDiscardButton);
	DestroyWindow(hCancelButton);
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

	hMainStitchWin = CreateWindow(
		"STATIC",
		0,
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidthX3,
		0,
		stitchWindowSize.x,
		stitchWindowSize.y,
		hWnd,
		NULL,
		hInst,
		NULL);

	GetWindowRect(hMainStitchWin, &stitchWindowAbsRect);

	hVerticalScrollBar = CreateWindow(
		"SCROLLBAR",
		0,
		SBS_VERT | WS_CHILD | WS_VISIBLE,
		stitchWindowSize.x + buttonWidthX3,
		0,
		SCROLSIZ,
		stitchWindowSize.y,
		hWnd,
		NULL,
		hInst,
		NULL);

	hHorizontalScrollBar = CreateWindow(
		"SCROLLBAR",
		0,
		SBS_HORZ | WS_CHILD | WS_VISIBLE,
		buttonWidthX3,
		stitchWindowSize.y,
		stitchWindowSize.x,
		SCROLSIZ,
		hWnd,
		NULL,
		hInst,
		NULL);
	ShowWindow(hVerticalScrollBar, FALSE);
	ShowWindow(hHorizontalScrollBar, FALSE);
}

//check if a click occurred in A veritcal set of 16 windows
// and calculate which window had the click
unsigned chkMsgs(POINT pt, HWND tWnd, HWND bWnd) {

	RECT	tRct;
	RECT	l_bRct;

	GetWindowRect(tWnd, &tRct);
	GetWindowRect(bWnd, &l_bRct);

	if (pt.x > tRct.left&&pt.x<l_bRct.right&&pt.y>tRct.top&&pt.y < l_bRct.bottom) {

		verticalIndex = 15 - (l_bRct.bottom - pt.y) / buttonHeight;
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
		buttonWidthX3,
		buttonHeight*(wndPnt + verticalIndex),
		buttonWidth,
		buttonHeight,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void delstch1(unsigned ind) {

	if (header.stitchCount) {

		while (ind <= header.stitchCount) {

			stitchBuffer[ind].attribute = stitchBuffer[ind + 1].attribute;
			stitchBuffer[ind].x = stitchBuffer[ind + 1].x;
			stitchBuffer[ind].y = stitchBuffer[ind + 1].y;
			ind++;
		}
		header.stitchCount--;
		if (closestPointIndex > (unsigned)header.stitchCount - 1)
			closestPointIndex = header.stitchCount - 1;
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
	header.stitchCount = bdat->scnt;
	if (header.stitchCount)
		stchred((sizeof(fPOINTATTRIBUTE)*header.stitchCount) >> 2, bdat->stch);
	unzoomedRect.x = bdat->zoomRect.x;
	unzoomedRect.y = bdat->zoomRect.y;
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
		mvflpnt(&clipboardPoints[0], &bdat->clipData[0], clpad);
	MoveMemory(&userColor, bdat->cols, sizeof(COLORREF) * 16);
	for (ind = 0; ind < 16; ind++) {

		userPen[ind] = nuPen(userPen[ind], 1, userColor[ind]);
		hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], userColor[ind]);
	}
	for (ind = 0; ind < 16; ind++)
		redraw(hUserColorWin[ind]);
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

			EnableMenuItem(hMainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
			rstMap(REDUSHO);
		} else {

			if (!setMap(REDUSHO))
				EnableMenuItem(hMainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
		}
		if (!setMap(UNDUSHO)) {

			EnableMenuItem(hMainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
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
		DestroyWindow(hPreferencesWindow);
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

			EnableMenuItem(hMainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			setMap(DUMEN);
			rstMap(UNDUSHO);
		}
	} else {

		if (undoBufferWriteIndex)
			undoBufferWriteIndex--;
		if (!undoBufferWriteIndex) {

			EnableMenuItem(hMainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			setMap(DUMEN);
			rstMap(UNDUSHO);
		}
	}
	if (!setMap(REDUSHO)) {

		EnableMenuItem(hMainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
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

	if (*pcsBMPFileName) {

		if (chkMap(MONOMAP)) {

			tabmsg(IDS_SAVMAP);
			return;
		}
		if (!chkMap(WASTRAC)) {

			tabmsg(IDS_MAPCHG);
			return;
		}
		if (GetSaveFileName(&obn)) {

			hBmp = CreateFile(usrBMPFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if (hIniFile == INVALID_HANDLE_VALUE) {

				crmsg(usrBMPFileName);
				return;
			}
			WriteFile(hBmp, (BITMAPFILEHEADER*)&bitmapFileHeader, 14, &rot, NULL);
			WriteFile(hBmp, (BITMAPV4HEADER*)&bitmapFileHeaderV4, bitmapFileHeader.bfOffBits - 14, &rot, NULL);
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
	hBmp = CreateFile(usrBMPFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (hBmp == INVALID_HANDLE_VALUE) {

		sprintf_s(msgbuf, sizeof(msgbuf), "Couldn't open bitmap file: %s\n", usrBMPFileName);
		shoMsg(msgbuf);
		CloseHandle(hBmp);
		hBmp = 0;
		*pcsBMPFileName = 0;
		return;
	}
	ReadFile(hBmp, (BITMAPFILEHEADER*)&bitmapFileHeader, 14, &bytesRead, NULL);
	if (bitmapFileHeader.bfType == 'MB') {

		bsiz = bitmapFileHeader.bfOffBits - 14;
		if (bsiz > sizeof(BITMAPV4HEADER))
			bsiz = sizeof(BITMAPV4HEADER);
		ReadFile(hBmp, (BITMAPV4HEADER*)&bitmapFileHeaderV4, bsiz, &bytesRead, NULL);
	} else {

		sprintf_s(msgbuf, sizeof(msgbuf), "%s is not a Windows Bitmap\n", usrBMPFileName);
		CloseHandle(hBmp);
		hBmp = 0;
		*pcsBMPFileName = 0;
		return;
	}
	if (gudtyp(bitmapFileHeaderV4.bV4BitCount)) {

		if (!rstMap(WASESC))
			rstMap(TRSET);
		bitmapWidth = bitmapFileHeaderV4.bV4Width;
		bitmapHeight = bitmapFileHeaderV4.bV4Height;
		setMap(INIT);
		zoomRect.left = zoomRect.bottom = 0;
		zoomRect.right = unzoomedRect.x;
		zoomRect.top = unzoomedRect.y;
		bitmapDC = CreateCompatibleDC(stitchWindowDC);
		if (bitmapFileHeaderV4.bV4BitCount == 1) {

			setMap(MONOMAP);
			bwidw = bitmapWidth >> 5;
			bovr = bitmapWidth % 32;
			if (bovr)
				bwidw++;
			bsiz = bwidw*bitmapHeight;
			ptrBitmap = new unsigned[bsiz];
			ReadFile(hBmp, (unsigned*)ptrBitmap, bsiz << 2, &bytesRead, NULL);
			CloseHandle(hBmp);
			if (binv(bwidw)) {

				bgnd = bitmapColor;
				fgnd = bitbak;
			} else {

				fgnd = bitmapColor;
				bgnd = bitbak;
			}
			memset((BITMAPINFOHEADER*)&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
			bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitmapInfoHeader.biWidth = bitmapWidth;
			bitmapInfoHeader.biHeight = bitmapHeight;
			bitmapInfoHeader.biPlanes = 1;
			bitmapInfoHeader.biBitCount = 32;
			bitmapInfoHeader.biCompression = BI_RGB;
			bitmapInfo.bmiHeader = bitmapInfoHeader;
			tbit = CreateDIBSection(bitmapDC, &bitmapInfo, DIB_RGB_COLORS, (void**)&pbits, 0, 0);
			for (ind = 0; ind < bitmapHeight; ind++)
				bitlin(&ptrBitmap[ind*bwidw], &pbits[ind*bitmapWidth], bgnd, fgnd);
			tdc = CreateCompatibleDC(stitchWindowDC);
			if (tbit && tdc) {
				SelectObject(tdc, tbit);
				hBmp = CreateCompatibleBitmap(stitchWindowDC, bitmapWidth, bitmapHeight);
				SelectObject(bitmapDC, hBmp);
				BitBlt(bitmapDC, 0, 0, bitmapWidth, bitmapHeight, tdc, 0, 0, SRCCOPY);
				DeleteObject(tbit);
				ReleaseDC(hWnd, tdc);
			}
			delete[] ptrBitmap;
		} else {

			CloseHandle(hBmp);
			rstMap(MONOMAP);
			hBmp = (HBITMAP)LoadImage(hInst, usrBMPFileName, IMAGE_BITMAP, bitmapWidth, bitmapHeight, LR_LOADFROMFILE);
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
		*pcsBMPFileName = 0;
		tabmsg(IDS_BMAP);
	}
}

void prtred() {

	CloseHandle(hFile);
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

	strcpy_s(colorFileName, fileName);
	strcpy_s(rgbFileName, fileName);
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
					colorChanges = 0;
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
	for (ind = 0; ind < dstRecordCount; ind++) {

		if (dstRecords[ind].nd & 0x40) {

			if (pcol)
				col = colmatch(pcol[colind++]);
			else {

				col++;
				col &= 0xf;
			}
		} else {

			dstin(dtrn(&dstRecords[ind]), &nu);
			loc.x += nu.x;
			loc.y += nu.y;
			if (!(dstRecords[ind].nd & 0x80)) {

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
	header.stitchCount = ine;
	l_siz.x = max.x - min.x;
	l_siz.y = max.y - min.y;
	iniFile.hoopType = CUSTHUP;
	unzoomedRect.x = iniFile.hoopSizeX;
	unzoomedRect.y = iniFile.hoopSizeY;
	if (l_siz.x > unzoomedRect.x || l_siz.y > unzoomedRect.y) {

		iniFile.hoopSizeX = unzoomedRect.x = l_siz.x*1.1;
		iniFile.hoopSizeY = unzoomedRect.y = l_siz.y*1.1;
		hsizmsg();
	}
	dif.x = (unzoomedRect.x - l_siz.x) / 2 - min.x;
	dif.y = (unzoomedRect.y - l_siz.y) / 2 - min.y;
	for (ind = 0; ind < header.stitchCount; ind++) {

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

	switch (iniFile.auxFileType) {

	case AUXDST:

		sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_AUXTXT], "DST");
		CheckMenuItem(hMainMenu, ID_AUXDST, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_AUXPCS, MF_UNCHECKED);
		break;

	default:

		CheckMenuItem(hMainMenu, ID_AUXDST, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_AUXPCS, MF_CHECKED);
		sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_AUXTXT], "PCS");
	}
	SetMenuItemInfo(hfileMenu, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
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
	for (ind = 0; ind < activePointIndex; ind++) {

		if (userColor[ind] == col)
			return ind;
	}
	matm = 0xffffff;
	for (ind = 1; ind < activePointIndex; ind++) {

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
		if (pcsBMPFileName[0]) {

			DeleteObject(hBmp);
			ReleaseDC(hWnd, bitmapDC);
			*pcsBMPFileName = 0;
		}
		hFile = CreateFile(fileName, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {

			ind = GetLastError();
			if (GetLastError() == 32)
				filnopn(IDS_FNOPNA, fileName);
			else
				filnopn(IDS_FNOPN, fileName);
			hFile = 0;
		} else {

			rstMap(CMPDO);
			rstMap(SAVACT);
			rstMap(BAKING);
			rstMap(REDUSHO);
			txad = 0;
			EnableMenuItem(hMainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
			deldu();
			strcpy_s(designerName, iniFile.designerName);
			unbsho();
			rstMap(MOVSET);
			frmon();
			selectedFormCount = 0;
			if (rstMap(PRFACT)) {

				DestroyWindow(hPreferencesWindow);
				rstMap(WASRT);
				preferenceIndex = 0;
			}
			pcsBMPFileName[0] = 0;
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
			defNam(fileName);
			nearestCount = 0;
			if (rstMap(WASPAT))
				DestroyWindow(hSpeedScrollBar);
			ind = 0;
			if (chkMap(WASTXBAK))
				ind = 1;
			clrMap(MAPLEN);
			if (ind)
				setMap(WASTXBAK);
			l_siz = GetFileSize(hFile, &sizh);
			pext = strrchr(fileName, '.');
			if (pext)
				pext++;
			else {

				strcat_s(fileName, ".thr");
				pext = strrchr(fileName, '.') + 1;
			}
			tchr = tolower(pext[0]);
			if (tchr == 't') {

				ReadFile(hFile, (STRHED*)&sthed, sizeof(STRHED), &bytesRead, NULL);
				if ((sthed.headerType & 0xffffff) == 0x746872) {

					if (bytesRead != sizeof(STRHED)) {

						tabmsg(IDS_SHRTF);
						return;
					}
					vervar = (sthed.headerType & 0xff000000) >> 24;
					switch (vervar) {

					case 0:

						if (header.hoopType == SMALHUP) {

							unzoomedRect.x = iniFile.hoopSizeX = SHUPX;
							unzoomedRect.y = iniFile.hoopSizeY = SHUPY;
						} else {

							unzoomedRect.x = iniFile.hoopSizeX = LHUPX;
							unzoomedRect.y = iniFile.hoopSizeY = LHUPY;
							header.hoopType = LARGHUP;
						}
						ritfnam(iniFile.designerName);
						strcpy_s(designerName, iniFile.designerName);
						strcpy_s(extendedHeader.modifierName, iniFile.designerName);
						break;

					case 1:
					case 2:

						ReadFile(hFile, (STREX*)&extendedHeader, sizeof(STREX), &bytesRead, NULL);
						if (bytesRead != sizeof(STREX)) {

							tabmsg(IDS_SHRTF);
							return;
						}
						iniFile.hoopSizeX = unzoomedRect.x = extendedHeader.hoopSizeX;
						iniFile.hoopSizeY = unzoomedRect.y = extendedHeader.hoopSizeY;
						redfnam(designerName);
						break;

					default:

						tabmsg(IDS_NOTVER);
						return;
					}
					zoomRect.bottom = zoomRect.left = 0;
					zoomRect.right = unzoomedRect.x = iniFile.hoopSizeX;
					zoomRect.top = unzoomedRect.y = iniFile.hoopSizeY;
					header.stitchCount = sthed.stitchCount;
					ReadFile(hFile, (fPOINTATTRIBUTE*)stitchBuffer, header.stitchCount * sizeof(fPOINTATTRIBUTE), &bytesRead, NULL);
					if (bytesRead != header.stitchCount * sizeof(fPOINTATTRIBUTE)) {

						header.stitchCount = bytesRead / sizeof(fPOINTATTRIBUTE);
						prtred();
						return;
					}
					ReadFile(hFile, (TCHAR*)pcsBMPFileName, 16, &bytesRead, 0);
					tred = bytesRead;
					if (bytesRead != 16) {

						pcsBMPFileName[0] = 0;
						prtred();
						return;
					}
					ReadFile(hFile, (COLORREF*)&backgroundColor, 4, &bytesRead, 0);
					tred += bytesRead;
					if (bytesRead != 4) {

						backgroundColor = iniFile.backgroundColor;
						prtred();
						return;
					}
					hBackgroundBrush = CreateSolidBrush(backgroundColor);
					ReadFile(hFile, (COLORREF*)userColor, 64, &bytesRead, 0);
					tred += bytesRead;
					if (bytesRead != 64) {

						prtred();
						return;
					}
					ReadFile(hFile, (COLORREF*)customColor, 64, &bytesRead, 0);
					tred += bytesRead;
					if (bytesRead != 64) {

						prtred();
						return;
					}
					ReadFile(hFile, (TCHAR*)msgbuf, 16, &bytesRead, 0);
					tred += bytesRead;
					if (bytesRead != 16) {

						prtred();
						return;
					}
					for (ind = 0; ind < 16; ind++)
						threadSize[ind][0] = msgbuf[ind];
					formIndex = sthed.formCount;
					if (formIndex > MAXFORMS)
						formIndex = MAXFORMS;
					inf = 0; ing = 0, inh = 0;
					if (formIndex) {

						ind = fltad = satkad = clpad = 0;
						msgbuf[0] = 0;
						if (vervar < 2) {

							frmlstx = (FRMHEDO*)&bseq;
							ReadFile(hFile, (FRMHEDO*)frmlstx, formIndex * sizeof(FRMHEDO), &bytesRead, 0);
							if (bytesRead != formIndex * sizeof(FRMHEDO)) {

								formIndex = bytesRead / sizeof(FRMHEDO);
								setMap(BADFIL);
							}
							xofrm();
						} else {

							ReadFile(hFile, (FRMHED*)formList, formIndex * sizeof(FRMHED), &bytesRead, 0);
							rstMap(BADFIL);
							if (bytesRead != formIndex * sizeof(FRMHED)) {

								formIndex = bytesRead / sizeof(FRMHED);
								setMap(BADFIL);
							}
						}
						//						ind=SetFilePointer(hFile,0,0,FILE_CURRENT);  //bug
						ReadFile(hFile, (fPOINT*)formPoints, sthed.pointCount * sizeof(fPOINT), &bytesRead, 0);
						if (bytesRead != sizeof(fPOINT)*sthed.pointCount) {

							fltad = bytesRead / sizeof(fPOINT);
							for (ind = fltad; ind < sthed.pointCount; ind++)
								formPoints[ind].x = formPoints[ind].y = 0;
							setMap(BADFIL);
						}
						ReadFile(hFile, (SATCON*)satks, sthed.dlineCount * sizeof(SATCON), &bytesRead, 0);
						if (bytesRead != sthed.dlineCount * sizeof(SATCON)) {

							satkad = bytesRead / sizeof(SATCON);
							setMap(BADFIL);
						}
						ReadFile(hFile, (fPOINT*)clipboardPoints, sthed.clipboardDataCount * sizeof(fPOINT), &bytesRead, 0);
						if (bytesRead != sthed.clipboardDataCount * sizeof(fPOINT)) {

							clpad = bytesRead / sizeof(fPOINT);
							setMap(BADFIL);
						}
						ReadFile(hFile, (TXPNT*)txpnts, extendedHeader.texturePointCount * sizeof(TXPNT), &bytesRead, 0);
						txad = bytesRead / sizeof(TXPNT);
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

						ReadFile(hFile, (PCSHEADER*)&header, 0x46, &bytesRead, NULL);
						if (!l_siz) {

							filnopn(IDS_ZEROL, fileName);
							return;
						}
						if (header.leadIn == 0x32 && header.colorCount == 16) {

							for (ind = 0; ind < 16; ind++)
								userColor[ind] = header.colors[ind];
							l_siz -= 0x46;
							inf = l_siz / sizeof(PCSTCH) + 2;
							ptrFileBuffer = new PCSTCH[inf];
							ReadFile(hFile, ptrFileBuffer, l_siz, &bytesRead, NULL);
							l_stind = 0;
							l_cPnt = 0;
							tcol = 0;
							ind = 0;
							while (l_stind < header.stitchCount&&ind < inf) {

								if (ptrFileBuffer[ind].typ == 3) {

									colorChangeTable[l_cPnt].colorIndex = ptrFileBuffer[ind].fx;
									colorChangeTable[l_cPnt++].stitchIndex = l_stind;
									tcol = NOTFRM | ptrFileBuffer[ind++].fx;
								} else {

									stitchBuffer[l_stind].x = ptrFileBuffer[ind].x + (float)ptrFileBuffer[ind].fx / 256;
									stitchBuffer[l_stind].y = ptrFileBuffer[ind].y + (float)ptrFileBuffer[ind].fy / 256;
									stitchBuffer[l_stind++].attribute = tcol;
									ind++;
								}
							}
							header.stitchCount = l_stind;
							tnam = (TCHAR*)&ptrFileBuffer[ind];
							strcpy_s(pcsBMPFileName, tnam);
							delete[] ptrFileBuffer;
							strcpy_s(pext, sizeof(fileName) - (pext - fileName), "thr");
							iniFile.auxFileType = AUXPCS;
							if (header.hoopType != LARGHUP&&header.hoopType != SMALHUP)
								header.hoopType = LARGHUP;
							sizstch(&strct);
							if (strct.left<0 || strct.right>LHUPY || strct.bottom<0 || strct.top>LHUPY) {

								iniFile.hoopSizeX = LHUPX;
								iniFile.hoopSizeY = LHUPY;
								chkhup();
							} else {

								if (header.hoopType == LARGHUP) {

									iniFile.hoopType = LARGHUP;
									iniFile.hoopSizeX = LHUPX;
									iniFile.hoopSizeY = LHUPY;
								} else {

									if (strct.right > SHUPX || strct.top > SHUPY || header.hoopType == LARGHUP) {

										iniFile.hoopType = LARGHUP;
										iniFile.hoopSizeX = SHUPX;
										iniFile.hoopSizeY = SHUPY;
									} else {

										iniFile.hoopType = SMALHUP;
										iniFile.hoopSizeX = SHUPX;
										iniFile.hoopSizeY = SHUPY;
									}
								}
							}
						}
					}
#if PESACT
					else {

						ReadFile(hFile, (BSEQPNT*)&bseq, sizeof(bseq), &bytesRead, 0);
						peshed = (PESHED*)&bseq;
						l_peschr = (TCHAR*)&bseq;
						if (strncmp(peshed->led, "#PES00", 6)) {

							sprintf_s(msgbuf, sizeof(msgbuf), "Not a PES file: %s\n", fileName);
							shoMsg(msgbuf);
							return;
						}
						pecof = tripl(peshed->off);
						pesStitch = (unsigned char*)&l_peschr[pecof + 532];
						activePointIndex = 0;
						pcolcnt = (unsigned char*)&l_peschr[pecof + 48];
						pesColors = &pcolcnt[1];
						markedStitchMap[0] = markedStitchMap[1] = 0;
						activePointIndex = 0;
						for (ind = 0; ind < (unsigned)(*pcolcnt + 1); ind++) {

							if (setRmp(pesColors[ind])) {

								userColor[activePointIndex++] = pestrn[pesColors[ind] & PESCMSK];
								if (activePointIndex >= 16)
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
						while (ind < bytesRead - pecof - 529) {

							if (pesStitch[ind] == 0xff && pesStitch[ind + 1] == 0)
								break;
							if (pesStitch[ind] == 0xfe && pesStitch[ind + 1] == 0xb0) {

								tcol = dupcol();
								ind += 2;
							} else {

								if (pesStitch[ind] & 0x80) {

									locof = dubl(&pesStitch[ind]);
									ind++;
								} else {

									if (pesStitch[ind] & 0x40)
										locof = pesStitch[ind] - 128;
									else
										locof = pesStitch[ind];
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
						header.stitchCount = ine;
						//iniFile.auxFileType=AUXPES;
						hupfn();
					}
#endif
				} else {

					ReadFile(hFile, (DSTHED*)&dsthed, sizeof(DSTHED), &bytesRead, 0);
					if (bytesRead == sizeof(DSTHED)) {

						if (chkdst(&dsthed)) {

							pcsBMPFileName[0] = 0;
							l_siz = GetFileSize(hFile, &bytesRead) - sizeof(DSTHED);
							dstRecordCount = l_siz / sizeof(DSTREC);
							dstRecords = new DSTREC[dstRecordCount];
							ReadFile(hFile, (DSTREC*)dstRecords, sizeof(DSTREC)*dstRecordCount, &bytesRead, 0);
							dstran();
							delete[] dstRecords;
							iniFile.auxFileType = AUXDST;
						}
					} else {

						tabmsg(IDS_DST2S);
						CloseHandle(hFile);
						return;
					}
				}
			}
			if (pcsBMPFileName[0]) {

				SetCurrentDirectory(defaultDirectory);
				strcpy_s(usrBMPFileName, pcsBMPFileName);
				bfil();
			}
			ritot(header.stitchCount);
			bufferIndex = 0;
			zoomRect.left = 0;
			zoomRect.bottom = 0;
			zoomRect.right = unzoomedRect.x = iniFile.hoopSizeX;
			zoomRect.top = unzoomedRect.y = iniFile.hoopSizeY;
			movStch();
			coltab();
			rstMap(ZUMED);
			buf[2] = 0;
			for (ind = 0; ind < 16; ind++) {

				userPen[ind] = nuPen(userPen[ind], 1, userColor[ind]);
				hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], userColor[ind]);
				strncpy_s(buf, threadSize[ind], 2);
				SetWindowText(hThreadSizeWin[ind], buf);
			}
			for (ind = 0; ind < 16; ind++)
				redraw(hUserColorWin[ind]);
			redraw(hColorBar);
			if (header.stitchCount)
				setMap(INIT);
			nuAct(0);
			setMap(RESTCH);
			nunams();
			ritini();
			butxt(HNUM, "");
			if (header.stitchCount)
				nuAct(stitchBuffer[0].attribute&COLMSK);
			else
				nuAct(0);
			auxmen();
		}
		lenCalc();
		sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRDBY], fileName, designerName);
		SetWindowText(hWnd, msgbuf);
		CloseHandle(hFile);
		setMap(INIT);
		rstMap(TRSET);
		if (chkMap(NOTHRFIL))
		{
			for (ind = 0; ind < header.stitchCount; ind++)
				stitchBuffer[ind].attribute |= NOTFRM;
		}
		lodchk();
	}
}

void clrfbuf(unsigned p_siz) {

#if  __UseASM__
	_asm {

		mov		edi, ptrFileBuffer
		mov		ecx, p_siz
		xor		eax, eax
		rep		stosd
	}
#else
	memset(ptrFileBuffer, 0, p_siz * 4);
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
		mov		ecx, dstRecordCount
		mul		ecx
		inc		ecx
		mov		dstRecordCount, ecx
		add		eax, dstRecords
		mov		ebx, dat
		mov[eax], ebx
	}
#else
	union {
		unsigned dat;
		DSTREC drec;
	} x;

	x.dat = dat;

	dstRecords[dstRecordCount++] = x.drec;
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
	pcol[2] = userColor[stitchBuffer[0].attribute&COLMSK];
	histch = &stitchBuffer[MAXSEQ];
	for (ind = 0; ind < header.stitchCount; ind++) {

		histch[ind].x = rotatedStitches[ind].x * 5 / 3;
		histch[ind].y = rotatedStitches[ind].y * 5 / 3;
		histch[ind].attribute = rotatedStitches[ind].attribute;
	}
	trct.left = trct.right = histch[0].x;
	trct.bottom = trct.top = histch[0].y;
	for (ind = 1; ind < header.stitchCount; ind++) {

		if (histch[ind].x > trct.right)
			trct.right = histch[ind].x + 0.5;
		if (histch[ind].x < trct.left)
			trct.left = histch[ind].x - 0.5;
		if (histch[ind].y > trct.top)
			trct.top = histch[ind].y + 0.5;
		if (histch[ind].y < trct.bottom)
			trct.bottom = histch[ind].y - 0.5;
	}
	dstRecords = (DSTREC*)&bseq;
	dstRecordCount = 0;
	loc.x = (trct.right - trct.left) / 2 + trct.left;
	loc.y = (trct.top - trct.bottom) / 2 + trct.bottom;
	dstplus.x = trct.right - loc.x + 1;
	dstplus.y = trct.top - loc.y + 1;
	dstmin.x = loc.x - trct.left - 1;
	dstmin.y = loc.y - trct.bottom - 1;
	col = histch[0].attribute & 0xf;
	for (ind = 0; ind < header.stitchCount; ind++) {

		if (col != (histch[ind].attribute & 0xf)) {

			savdst(0xc30000);
			col = histch[ind].attribute & 0xf;
			pcol[colind++] = userColor[col];
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
	dstRecords[dstRecordCount].led = dstRecords[dstRecordCount].mid = 0;
	dstRecords[dstRecordCount++].nd = (TBYTE)0xf3;

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
	for (ind = 1; ind < header.stitchCount; ind++) {

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

		header.hoopType = LARGHUP;
		hupsiz.x = LHUPX;
		hupsiz.y = LHUPY;
	} else {

		if (iniFile.hoopSizeX == LHUPX&&iniFile.hoopSizeY == LHUPY) {

			header.hoopType = LARGHUP;
			hupsiz.x = LHUPX;
			hupsiz.y = LHUPY;
		} else {

			header.hoopType = SMALHUP;
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

		for (ind = 0; ind < header.stitchCount; ind++) {

			rotatedStitches[ind].x += dif.x;
			rotatedStitches[ind].y += dif.y;
		}
	}
	return 0;
}

#if PESACT

void ritpes(unsigned p_stind) {

	pesStitches[outputIndex].x = -rotatedStitches[p_stind].x * 3 / 5 + stitchCenterOffset.x;
	pesStitches[outputIndex++].y = rotatedStitches[p_stind].y * 3 / 5 - stitchCenterOffset.y;
}

void ritpcol(unsigned char col) {

#if  __UseASM__
	_asm {

		mov		ebx, outputIndex
		mov		eax, ebx
		inc		eax
		mov		outputIndex, eax
		shl		ebx, 2
		add		ebx, pesStitches
		xor		eax, eax
		mov		al, col
		mov[ebx], eax
	}
#else
	_asm {

		mov		ebx, outputIndex
		mov		eax, ebx
		inc		eax
		mov		outputIndex, eax
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
		mov		ebx, offset auxName
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
		mov		ebx, offset auxName
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

			ptrPesData[outputIndex] = idif - 128;
			outputIndex++;
		} else {

			ptrPesData[outputIndex] = idif >> 8;
			outputIndex++;
			ptrPesData[outputIndex] = idif & 0x8f;
			outputIndex++;
		}
	} else {

		if (idif < 64) {

			ptrPesData[outputIndex] = idif;
			outputIndex++;
		} else {

			ptrPesData[outputIndex] = (idif >> 8) | 0x80;
			outputIndex++;
			ptrPesData[outputIndex] = idif & 0xff;
			outputIndex++;
		}
	}
}

void pecdat() {

	unsigned	ind, col, pcol;

	outputIndex = 532;
	pcol = 1;
	col = stitchBuffer[0].attribute&COLMSK;
	ptrPesData = (TCHAR*)&bseq;
	pesColors = (unsigned char*)&ptrPesData[49];
	rpcrd(stitchBuffer[0].x);
	rpcrd(-stitchBuffer[0].y);
	for (ind = 0; ind < (unsigned)header.stitchCount - 1; ind++) {

		if ((stitchBuffer[ind].attribute&COLMSK) != col) {

			col = stitchBuffer[ind].attribute&COLMSK;
			ptrPesData[outputIndex] = (TBYTE)254;
			outputIndex++;
			ptrPesData[outputIndex] = (TBYTE)176;
			outputIndex++;
			ptrPesData[outputIndex] = pcol;
			outputIndex++;
			pesColors[pcol] = pesEquivColors[col];
			pcol++;
		}
		rpcrd(stitchBuffer[ind + 1].x - stitchBuffer[ind].x);
		rpcrd(-stitchBuffer[ind + 1].y + stitchBuffer[ind].y);
	}
	ptrPesData[outputIndex++] = (TBYTE)0xff;
	ptrPesData[outputIndex++] = 0;
}

#endif

void chk1col() {

	unsigned ind, ine, col;

	coltab();
	setMap(RESTCH);
	for (ind = 0; ind < colorChanges; ind++) {

		if (colorChangeTable[ind + 1].stitchIndex - colorChangeTable[ind].stitchIndex == 1) {

			ine = colorChangeTable[ind].stitchIndex;
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
	strcpy_s(driv, homeDirectory);
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
	unsigned		savcol;	

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
	if (chkattr(auxName))
		return;
	if (!header.stitchCount)
		return;
	chk1col();
	coltab();
	rotatedStitches = (fPOINTATTRIBUTE*)&bseq;
	if (chku(ROTAUX)) {

		for (ind = 0; ind < header.stitchCount; ind++) {

			rotatedStitches[ind].y = -stitchBuffer[ind].x;
			rotatedStitches[ind].x = stitchBuffer[ind].y;
			rotatedStitches[ind].attribute = stitchBuffer[ind].attribute;
		}
	} else {

		for (ind = 0; ind < header.stitchCount; ind++) {

			rotatedStitches[ind].x = stitchBuffer[ind].x;
			rotatedStitches[ind].y = stitchBuffer[ind].y;
			rotatedStitches[ind].attribute = stitchBuffer[ind].attribute;
		}
	}
	hPcsFile = CreateFile(auxName, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hPcsFile == INVALID_HANDLE_VALUE) {

		crmsg(auxName);
		hPcsFile = 0;
	} else {

		switch (iniFile.auxFileType) {

		case AUXDST:

			ritdst();
			ind = 0; loc.x = loc.y = 0;
			pchr = (TCHAR*)&dsthed;
			for (ind = 0; ind < sizeof(DSTHED); ind++)
				pchr[ind] = ' ';
			strncpy(dsthed.desched, "LA:", 3);
			pchr = strrchr(auxName, '\\') + 1;
			for (ind = 0; ind < sizeof(dsthed.desc); ind++) {

				if (pchr[ind] && pchr[ind] != '.')
					dsthed.desc[ind] = pchr[ind];
				else
					break;
			}
			dsthed.desc[16] = 0xd;
			strncpy(dsthed.recshed, "ST:", 3);
			sprintf(dsthed.recs, "%7d\r", dstRecordCount);
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
			WriteFile(hPcsFile, (DSTHED*)&dsthed, sizeof(DSTHED), &wrot, 0);
			WriteFile(hPcsFile, (DSTREC*)dstRecords, sizeof(DSTREC)*dstRecordCount, &wrot, 0);
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

					match = pesmtch(userColor[ind], ine);
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
			outputIndex = 0;
			pesStitches = (PESTCH*)&bseq;
			ritpes(0);
			pesStitches[outputIndex].x = -32765; // 0x8003
			pesStitches[outputIndex++].y = 0;
			ritpcol(pesEquivColors[tcol]);
			ritpes(0);
			pcolcnt = 0;
			for (ind = 1; ind < header.stitchCount; ind++) {

				if (tcol == (stitchBuffer[ind].attribute&COLMSK))
					ritpes(ind);
				else {

					ritpes(ind);
					pesStitches[outputIndex].x = -32767; // 0x8001
					pesStitches[outputIndex++].y = 0;
					ritpcol(pesEquivColors[tcol]);
					tcol = stitchBuffer[ind].attribute&COLMSK;
					ritpes(ind++);
					ritpes(ind);
					pesStitches[outputIndex].x = -32765; // 0x8003
					pesStitches[outputIndex++].y = 0;
					ritpcol(pesEquivColors[tcol]);
					ritpes(ind);
					pcolcnt++;
				}
			}
			pesStitches[outputIndex].x = ine;
			pesStitches[outputIndex++].y = 0;
			pesof = (unsigned*)&peshed.off;
			*pesof = (outputIndex << 2) + sizeof(PESHED);
			*peshed.m1 = 0x20;
			groupStartStitch = 0;
			groupEndStitch = header.stitchCount - 1;
			peshed.xsiz = 10000;
			peshed.ysiz = 10000;
			WriteFile(hPcsFile, (PESHED*)&peshed, sizeof(PESHED), &wrot, 0);
			WriteFile(hPcsFile, (PESTCH*)&bseq, outputIndex << 2, &wrot, 0);
			ind = pesnam();
			pchr = (TCHAR*)&bseq;
			while (ind < 512)
				pchr[ind++] = ' ';
			pchr[19] = 13;
			pchr[48] = (TCHAR)pcolcnt;
			pecdat();
			upnt = (unsigned*)&pchr[514];
			*upnt = outputIndex - 512;
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
			WriteFile(hPcsFile, (TBYTE*)&bseq, outputIndex, &wrot, 0);
			break;
#endif
		default:

			for (ind = 0; ind < 16; ind++)
				header.colors[ind] = userColor[ind];
			if (pcshup())
				return;
			if (!WriteFile(hPcsFile, &header, 0x46, &wrot, 0)) {

				riter();
				return;
			}
			ptrFileBuffer = new PCSTCH[header.stitchCount + colorChanges + 2];
			clrfbuf((sizeof(PCSTCH)*(header.stitchCount + colorChanges + 1)) >> 2);
			l_stind = 0;
			savcol = 0xff;
			for (ind = 0; ind < header.stitchCount; ind++) {

				if ((rotatedStitches[ind].attribute&COLMSK) != savcol) {

					savcol = rotatedStitches[ind].attribute&COLMSK;
					ptrFileBuffer[l_stind].typ = 3;
					ptrFileBuffer[l_stind++].fx = savcol;
				}
				frct = modf(rotatedStitches[ind].x, &intg);
				ptrFileBuffer[l_stind].fx = frct * 256;
				ptrFileBuffer[l_stind].x = intg;
				frct = modf(rotatedStitches[ind].y, &intg);
				ptrFileBuffer[l_stind].fy = frct * 256;
				ptrFileBuffer[l_stind++].y = intg;
			}
			if (!WriteFile(hPcsFile, ptrFileBuffer, l_stind * sizeof(PCSTCH), &wrot, 0)) {

				riter();
				return;
			}
			if (chku(BSAVOF)) {

				*msgbuf = 0;
				if (!WriteFile(hPcsFile, msgbuf, 15, &wrot, 0)) {

					riter();
					return;
				}
			} else {

				if (!WriteFile(hPcsFile, pcsBMPFileName, 15, &wrot, 0)) {

					riter();
					return;
				}
			}
			delete[] ptrFileBuffer;
		}
		defNam(fileName);
		CloseHandle(hPcsFile);
		if (chku(ROTAUX))
			filnopn(IDS_FILROT, auxName);
	}
}
#pragma warning(pop)

void savAs() {

	TCHAR*	pchr;

	if (header.stitchCount || formIndex || *pcsBMPFileName) {

		ofn.nFilterIndex = 0;
		if (GetSaveFileName(&ofn)) {

			_strlwr_s(fileName);
			pchr = strrchr(fileName, '.');
			if (!pchr)
				pchr = &fileName[strlen(fileName)];
			switch (ofn.nFilterIndex) {

			case 1:

				strcpy_s(pchr, sizeof(fileName) - (pchr - fileName), ".thr");
				break;

			case 2:

				strcpy_s(pchr, sizeof(fileName) - (pchr - fileName), ".pcs");
				iniFile.auxFileType = AUXPCS;
				auxmen();
				break;

			case 3:

				strcpy_s(pchr, sizeof(fileName) - (pchr - fileName), ".dst");
				iniFile.auxFileType = AUXDST;
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
			SetWindowText(hWnd, thrName);
		}
	}
}

void save() {

	TCHAR*	pchr;
	TCHAR	tchr;

	if (fileName[0]) {

		pchr = strrchr(fileName, '.');
		if (pchr)
			pchr++;
		else {

			strcat_s(fileName, ".thr");
			pchr = strrchr(fileName, '.') + 1;
		}
		tchr = pchr[0] | 0x20;
		thrsav();
		if (header.stitchCount)
			sav();
	} else
		savAs();
}

COLORREF nuCol(COLORREF init) {

	ccColorStruct.Flags = CC_ANYCOLOR | CC_RGBINIT;
	ccColorStruct.hwndOwner = hWnd;
	ccColorStruct.lCustData = 0;
	ccColorStruct.lpCustColors = customColor;
	ccColorStruct.lpfnHook = 0;
	ccColorStruct.lpTemplateName = 0;
	ccColorStruct.rgbResult = init;
	ccColorStruct.lStructSize = sizeof(ccColorStruct);
	return ChooseColor(&ccColorStruct);
}

COLORREF nuBak() {

	ccBackgroundColorStruct.Flags = CC_ANYCOLOR | CC_RGBINIT;
	ccBackgroundColorStruct.hwndOwner = hWnd;
	ccBackgroundColorStruct.lCustData = 0;
	ccBackgroundColorStruct.lpCustColors = customBackgroundColor;
	ccBackgroundColorStruct.lpfnHook = 0;
	ccBackgroundColorStruct.lpTemplateName = 0;
	ccBackgroundColorStruct.rgbResult = backgroundColor;
	ccBackgroundColorStruct.lStructSize = sizeof(ccBackgroundColorStruct);
	return ChooseColor(&ccBackgroundColorStruct);
}

COLORREF nuBit() {

	ccBitMapColorStruct.Flags = CC_ANYCOLOR | CC_RGBINIT;
	ccBitMapColorStruct.hwndOwner = hWnd;
	ccBitMapColorStruct.lCustData = 0;
	ccBitMapColorStruct.lpCustColors = bitmapBackgroundColors;
	ccBitMapColorStruct.lpfnHook = 0;
	ccBitMapColorStruct.lpTemplateName = 0;
	ccBitMapColorStruct.rgbResult = bitmapColor;
	ccBitMapColorStruct.lStructSize = sizeof(ccBackgroundColorStruct);
	return ChooseColor(&ccBitMapColorStruct);
}

void pxCor2stch(POINT pnt) {

	double	tdub;

	tdub = (double)(pnt.x - stitchWindowAbsRect.left) / stitchWindowClientRect.right;
	selectedPoint.x = tdub*(zoomRect.right - zoomRect.left) + zoomRect.left;
	tdub = (double)(stitchWindowAbsRect.bottom - pnt.y) / stitchWindowClientRect.bottom;
	selectedPoint.y = tdub*(zoomRect.top - zoomRect.bottom) + zoomRect.bottom;
}

unsigned px2stch() {

	double	tdub;

	if (msg.pt.x >= stitchWindowAbsRect.left&&msg.pt.x <= stitchWindowAbsRect.right
		&&msg.pt.y >= stitchWindowAbsRect.top&&msg.pt.y <= stitchWindowAbsRect.bottom) {

		tdub = (double)(msg.pt.x - stitchWindowAbsRect.left) / stitchWindowClientRect.right;
		selectedPoint.x = tdub*(zoomRect.right - zoomRect.left) + zoomRect.left;
		tdub = (double)(stitchWindowAbsRect.bottom - msg.pt.y) / stitchWindowClientRect.bottom;
		selectedPoint.y = tdub*(zoomRect.top - zoomRect.bottom) + zoomRect.bottom;
		return 1;
	} else
		return 0;
}

void shft2box() {

	selectedPoint.x = stitchBuffer[closestPointIndex].x;
	selectedPoint.y = stitchBuffer[closestPointIndex].y;
	shft(selectedPoint);
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

			selectedPoint.x = zoomMarkPoint.x;
			selectedPoint.y = zoomMarkPoint.y;
			goto gotc;
		}
		if (chkMap(FORMSEL)) {

			trct = &formList[closestFormToCursor].rectangle;
			selectedPoint.x = ((trct->right - trct->left) / 2) + trct->left;
			selectedPoint.y = ((trct->top - trct->bottom) / 2) + trct->bottom;
			goto gotc;
		}
		if (chkMap(FRMPSEL)) {

			selectedPoint.x = formList[closestFormToCursor].vertices[closestVertexToCursor].x;
			selectedPoint.y = formList[closestFormToCursor].vertices[closestVertexToCursor].y;
			goto gotc;
		}
		if (chkMap(SELBOX)) {

			selectedPoint.x = stitchBuffer[closestPointIndex].x;
			selectedPoint.y = stitchBuffer[closestPointIndex].y;
			goto gotc;
		}
		if (chkMap(GRPSEL)) {

			selRct(&srct);
			selectedPoint.x = ((srct.right - srct.left) / 2) + srct.left;
			selectedPoint.y = ((srct.top - srct.bottom) / 2) + srct.bottom;
			goto gotc;
		}
		if (chkMap(INSRT)) {

			if (chkMap(LIN1)) {

				if (chkMap(BAKEND)) {

					selectedPoint.x = stitchBuffer[header.stitchCount - 1].x;
					selectedPoint.y = stitchBuffer[header.stitchCount - 1].y;
				} else {

					selectedPoint.x = stitchBuffer[0].x;
					selectedPoint.y = stitchBuffer[0].y;
				}
			} else {

				selectedPoint.x = (stitchBuffer[closestPointIndex + 1].x - stitchBuffer[closestPointIndex].x) / 2 + stitchBuffer[closestPointIndex].x;
				selectedPoint.y = (stitchBuffer[closestPointIndex + 1].y - stitchBuffer[closestPointIndex].y) / 2 + stitchBuffer[closestPointIndex].y;
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
			selectedPoint.x = (selectedFormsRectangle.right - selectedFormsRectangle.left) / 2 + selectedFormsRectangle.left;
			selectedPoint.y = (selectedFormsRectangle.top - selectedFormsRectangle.bottom) / 2 + selectedFormsRectangle.bottom;
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
	shft(selectedPoint);
	nearestCount = 0;
	if (!chkMap(GMRK) && chkMap(SELBOX))
		shft2box();
	if (chkMap(RUNPAT)) {

		FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
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
	nearestCount = 0;
	if (chkMap(RUNPAT)) {

		FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
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
			nearestCount = 0;
			shft(selectedPoint);
			spnt.x = selectedPoint.x;
			spnt.y = selectedPoint.y;
			sCor2px(spnt, &pnt);
			if (chkMap(RUNPAT)) {

				FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
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

			selectedPoint.x = zoomMarkPoint.x;
			selectedPoint.y = zoomMarkPoint.y;
			goto gots;
		}
		if (chkMap(FORMSEL)) {

			trct = &formList[closestFormToCursor].rectangle;
			selectedPoint.x = ((trct->right - trct->left) / 2) + trct->left;
			selectedPoint.y = ((trct->top - trct->bottom) / 2) + trct->bottom;
			goto gots;
		}
		if (chkMap(FRMPSEL)) {

			selectedPoint.x = formList[closestFormToCursor].vertices[closestVertexToCursor].x;
			selectedPoint.y = formList[closestFormToCursor].vertices[closestVertexToCursor].y;
			goto gots;
		}
		if (chkMap(SELBOX) || chkMap(INSRT)) {

			selectedPoint.x = stitchBuffer[closestPointIndex].x;
			selectedPoint.y = stitchBuffer[closestPointIndex].y;
			goto gots;
		}
		if (chkMap(GRPSEL)) {

			selRct(&srct);
			selectedPoint.x = ((srct.right - srct.left) / 2) + srct.left;
			selectedPoint.y = ((srct.top - srct.bottom) / 2) + srct.bottom;
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
			nearestCount = 0;
		} else {

			nusiz.x = unzoomedRect.x*zoomFactor;
			nusiz.y = nusiz.x / stitchWindowAspectRatio;
			zoomRect.left = zoomRect.bottom = 0;
			zoomRect.top = nusiz.y;
			zoomRect.right = nusiz.x;
			shft(selectedPoint);
		}
		if (chkMap(RUNPAT)) {

			FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
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

		cx = ((stitchBuffer[ind].x > selectedPoint.x) ? (stitchBuffer[ind].x - selectedPoint.x) : (selectedPoint.x - stitchBuffer[ind].x));
		cy = ((stitchBuffer[ind].y > selectedPoint.y) ? (stitchBuffer[ind].y - selectedPoint.y) : (selectedPoint.y - stitchBuffer[ind].y));
		sum = hypot(cx, cy);
		tind0 = ind;
		for (ine = 0; ine < NERCNT; ine++) {

			if (sum < gapToNearest[ine]) {

				tsum = gapToNearest[ine];
				tind1 = nearestPoint[ine];
				gapToNearest[ine] = sum;
				nearestPoint[ine] = tind0;
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

		gapToNearest[ind] = 1e99;
		nearestPoint[ind] = -1;
	}
	px2stch();
	for (ind = 0; ind < colorChanges; ind++) {

		sind0 = colorChangeTable[ind].stitchIndex;
		sind1 = colorChangeTable[ind + 1].stitchIndex;
		if (chkMap(HID)) {

			if (colorChangeTable[ind].colorIndex == activeColor)
				duClos(sind0, sind1 - sind0);
		} else
			duClos(sind0, sind1 - sind0);
	}
	GetClientRect(hMainStitchWin, &stitchWindowClientRect);
	for (ind = 0; ind < NERCNT; ind++) {

		if (stch2px(nearestPoint[ind])) {

			nearestPoint[nearestCount] = nearestPoint[ind];
			pxNer[nearestCount].x = stitchSizePixels.x;
			pxNer[nearestCount++].y = stitchSizePixels.y;
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
	for (ind = 0; ind < nearestCount; ind++) {

		nerSid = boxOffset[ind];
		if (chkPnt.x >= pxNer[ind].x - nerSid&&
			chkPnt.x <= pxNer[ind].x + nerSid&&
			chkPnt.y >= pxNer[ind].y - nerSid&&
			chkPnt.y <= pxNer[ind].y + nerSid) {

			*clo = nearestPoint[ind];
			return 1;
		}
	}
	px2stch();
	distanceToClick = 1e99;
	if (chkMap(HID)) {

		for (ind = 0; ind < colorChanges; ind++) {

			if (colorChangeTable[ind].colorIndex == activeColor) {

				for (ine = colorChangeTable[ind].stitchIndex; ine < colorChangeTable[ind + 1].stitchIndex; ine++) {

					if (stitchBuffer[ine].x >= zoomRect.left&&
						stitchBuffer[ine].x <= zoomRect.right&&
						stitchBuffer[ine].y >= zoomRect.bottom&&
						stitchBuffer[ine].y <= zoomRect.top) {

						cx = ((stitchBuffer[ine].x > selectedPoint.x) ? (stitchBuffer[ine].x - selectedPoint.x) : (selectedPoint.x - stitchBuffer[ine].x));
						cy = ((stitchBuffer[ine].y > selectedPoint.y) ? (stitchBuffer[ine].y - selectedPoint.y) : (selectedPoint.y - stitchBuffer[ine].y));
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

		for (ind = 0; ind < header.stitchCount; ind++) {

			layr = (stitchBuffer[ind].attribute&LAYMSK) >> LAYSHFT;
			if (!activeLayer || !layr || layr == activeLayer) {

				if (stitchBuffer[ind].x >= zoomRect.left&&
					stitchBuffer[ind].x <= zoomRect.right&&
					stitchBuffer[ind].y >= zoomRect.bottom&&
					stitchBuffer[ind].y <= zoomRect.top) {

					cx = stitchBuffer[ind].x - selectedPoint.x;
					cy = stitchBuffer[ind].y - selectedPoint.y;
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

	for (ind = 0; ind < colorChanges; ind++) {

		if (colorChangeTable[ind].stitchIndex > pInd)
			break;
	}
	return ind;
}

void unthum() {

	unsigned ind;

	if (rstMap(THUMSHO)) {

		for (ind = 0; ind < 4; ind++)
			DestroyWindow(hBackupViewer[ind]);
		if (chkMap(UPTO))
			butxt(HUPTO, stab[STR_UPON]);
		else
			butxt(HUPTO, stab[STR_UPOF]);
		butxt(HNUM, "");
		redraw(hButtonWin[HHID]);
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

					while (closestPointIndex < header.stitchCount&&notfstch(stitchBuffer[closestPointIndex].attribute))
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
	redraw(hButtonWin[HHID]);
	setMap(RESTCH);
}

void dulin() {

	SelectObject(stitchWindowDC, linePen);
	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	if (moveLine0[0].x == moveLine1[1].x&&moveLine0[0].y == moveLine1[1].y) {

		if (chkMap(ISDWN)) {

			Polyline(stitchWindowDC, moveLine0, 2);
			goto dux;
		} else {

			Polyline(stitchWindowDC, moveLine1, 2);
			goto dux;
		}
	} else {

		if (chkMap(ISDWN))
			Polyline(stitchWindowDC, moveLine0, 2);
		if (chkMap(ISUP))
			Polyline(stitchWindowDC, moveLine1, 2);
	}
dux:;
	SetROP2(stitchWindowDC, R2_COPYPEN);
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

		if (colorChangeTable[cpnt].colorIndex == activeColor)
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
 to the select point (selectedPoint) is calculated.

Find the equation for the line by solving the linear parametric eauations

  stitchBuffer[ind].x+slope*stitchBuffer[ind].y=off
  stitchBuffer[ind+1].x+slope*stitchBuffer[ind+1].y=off

substituting:

  stitchBuffer[ind].x+slope*stitchBuffer[ind].y=stitchBuffer[ind+1].x+slope*stitchBuffer[ind+1].y

collecting terms:

  slope*stitchBuffer[ind].y-slope*stitchBuffer[ind-1].y=stitchBuffer[ind+1].x-stitchBuffer[ind].x
  slope*(stitchBuffer[ind].y-stitchBuffer[ind-1].y)=stitchBuffer[ind+1].x-stitchBuffer[ind].x
  slope=(stitchBuffer[ind+1].x-stitchBuffer[ind].x)/(stitchBuffer[ind].y-stitchBuffer[ind-1].y)

define xba=stitchBuffer[ind+1].x-stitchBuffer[ind].x
define yab=stitchBuffer[ind].y-stitchBuffer[ind+1].y

  slope=xba/yab;

back substituting into stitchBuffer[ind].x+slope*stitchBuffer[ind].y=off

  off=stitchBuffer[ind].x+slope*stitchBuffer[ind].y

The equation for a point vertical to the equation for the line and running
 through selectedPoint is:

  selectedPoint.x-selectedPoint.y/slope=poff

If ipnt is the intersections between the two lines then

  ipnt.x-slope*ipnt.y=off
  ipnt.x+ipnt.y/slope=poff

Subtracting the two equations

  slope*ipnt.y+ipnt.y/slope=off-poff

Multiply by slope

  slope*slope*ipnt.y+ipnt.y=slope(off-poff)
  ipnt.y(slope*slope+1)=slope(off-poff)
  ipnt.y=slope*(off-poff)/(slope*slope+1)

back substitute into ipnt.x+slope*ipnt.y=off

  ipnt.x=off-slope*ipnt.y

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
	for (ine = 0; ine < colorChanges; ine++) {

		scnt = colorChangeTable[ine + 1].stitchIndex - colorChangeTable[ine].stitchIndex;
		l_pstch = &stitchBuffer[colorChangeTable[ine].stitchIndex];
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
							clos = ind + colorChangeTable[ine].stitchIndex;
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

	SelectObject(stitchWindowDC, linePen);
	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	Polyline(stitchWindowDC, insertLine, 2);
	SetROP2(stitchWindowDC, R2_XORPEN);
	Polyline(stitchWindowDC, &insertLine[1], 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
	SelectObject(stitchWindowMemDC, linePen);
	SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
	Polyline(stitchWindowMemDC, insertLine, 2);
	SetROP2(stitchWindowMemDC, R2_XORPEN);
	Polyline(stitchWindowMemDC, &insertLine[1], 2);
	SetROP2(stitchWindowMemDC, R2_COPYPEN);
}

void xlin() {

	if (rstMap(ILIN))
		ilin();
}

void ilin1() {

	SelectObject(stitchWindowDC, linePen);
	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	Polyline(stitchWindowDC, insertLine, 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void xlin1() {

	if (rstMap(ILIN1))
		ilin1();
}

void duIns() {

	POINT tlin[2];

	insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
	insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
	stch2px1(closestPointIndex);
	tlin[0].x = insertLine[0].x = stitchSizePixels.x;
	tlin[0].y = insertLine[0].y = stitchSizePixels.y;
	stch2px1(closestPointIndex + 1);
	tlin[1].x = insertLine[2].x = stitchSizePixels.x;
	tlin[1].y = insertLine[2].y = stitchSizePixels.y;
	xlin();
	setMap(ILIN);
	ilin();
}

void istch() {

	double angb, angf, angt;

	xlin();
	xlin1();
	if (chkMap(SELBOX)) {

		if (closestPointIndex&&closestPointIndex != (unsigned)header.stitchCount - 1) {

			px2stch();
			angt = atan2(stitchBuffer[closestPointIndex].y - selectedPoint.y, stitchBuffer[closestPointIndex].x - selectedPoint.x);
			angb = atan2(stitchBuffer[closestPointIndex].y - stitchBuffer[closestPointIndex - 1].y, stitchBuffer[closestPointIndex].x - stitchBuffer[closestPointIndex - 1].x);
			angf = atan2(stitchBuffer[closestPointIndex].y - stitchBuffer[closestPointIndex + 1].y, stitchBuffer[closestPointIndex].x - stitchBuffer[closestPointIndex + 1].x);
			if (fabs(angf - angt) > fabs(angb - angt))
				closestPointIndex--;
		} else {

			if (closestPointIndex == (unsigned)header.stitchCount - 1)
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

	long pwid = boxOffset[0];

	stch2px1(ind);
	insertLine[0].x = stitchSizePixels.x - pwid;
	insertLine[1].y = insertLine[0].y = stitchSizePixels.y;
	insertLine[1].x = stitchSizePixels.x + pwid;
	SelectObject(stitchWindowDC, crossPen);
	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	Polyline(stitchWindowDC, insertLine, 2);
	SelectObject(stitchWindowMemDC, crossPen);
	SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
	Polyline(stitchWindowMemDC, insertLine, 2);
	insertLine[0].y = stitchSizePixels.y - pwid;
	insertLine[0].x = insertLine[1].x = stitchSizePixels.x;
	insertLine[1].y = stitchSizePixels.y - 1;
	Polyline(stitchWindowDC, insertLine, 2);
	Polyline(stitchWindowMemDC, insertLine, 2);
	insertLine[0].y = stitchSizePixels.y + 2;
	insertLine[1].y = stitchSizePixels.y + pwid;
	Polyline(stitchWindowDC, insertLine, 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
	Polyline(stitchWindowMemDC, insertLine, 2);
	SetROP2(stitchWindowMemDC, R2_COPYPEN);
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

	if (header.stitchCount)
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
		if (ind > (unsigned)header.stitchCount - 1)
			ind = header.stitchCount - 1;
		closestPointIndex = groupStitchIndex = ind;
		col = stitchBuffer[ind].attribute&COLMSK;
		while (closestPointIndex && (stitchBuffer[closestPointIndex].attribute&COLMSK) == col)
			closestPointIndex--;
		if ((stitchBuffer[closestPointIndex].attribute&COLMSK) != col)
			closestPointIndex++;
		while (groupStitchIndex < header.stitchCount && (stitchBuffer[groupStitchIndex].attribute&COLMSK) == col)
			groupStitchIndex++;
		if ((stitchBuffer[closestPointIndex].attribute&COLMSK) != col)
			closestPointIndex--;
		if (groupStitchIndex > (unsigned)header.stitchCount - 1)
			groupStitchIndex = header.stitchCount - 1;
		setMap(GRPSEL);
		unbox();
		grpAdj();
		activeColor = col;
		setMap(RESTCH);
	}
}

void nuAct(unsigned ind) {

	unsigned tcol;

	tcol = activeColor;
	activeColor = stitchBuffer[ind].attribute&COLMSK;
	redraw(hUserColorWin[tcol]);
	redraw(hUserColorWin[activeColor]);
}

void newFil() {

	unsigned ind;

	rstMap(CMPDO);
	if (pcsBMPFileName[0]) {

		pcsBMPFileName[0] = 0;
		DeleteObject(hBmp);
		ReleaseDC(hWnd, bitmapDC);
	}
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], iniFile.designerName);
	deldu();
	SetWindowText(hWnd, msgbuf);
	strcpy_s(thrName, stab[STR_NUFIL]);
	ritfnam(iniFile.designerName);
	strcpy_s(extendedHeader.modifierName, iniFile.designerName);
	rstdu();
	rstAll();
	clrhbut(3);
	rstMap(MOVSET);
	header.leadIn = 0x32;
	header.colorCount = 16;
	unbox();
	xlin();
	xlin1();
	rstMap(INIT);
	rstMap(INSRT);
	rstMap(LIN1);
	rstMap(FORMSEL);
	rstMap(BAKACT);
	rstMap(GMRK);
	header.stitchCount = 0;
	DiplayedColorBitmap = 0;
	pcsBMPFileName[0] = 0;
	header.stitchCount = 0;
	fltad = 0;
	clpad = 0;
	txad = 0;
	satkad = 0;
	formIndex = 0;
	fileName[0] = 0;
	colorChanges = 0;
	knotCount = 0;
	for (ind = 0; ind < 16; ind++) {

		redraw(hDefaultColorWin[ind]);
		redraw(hUserColorWin[ind]);
		redraw(hThreadSizeWin[ind]);
	}
	zumhom();
}

void bBox() {

	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	SelectObject(stitchWindowDC, linePen);
	Polyline(stitchWindowDC, zoomBoxLine, 5);
	SetROP2(stitchWindowDC, R2_COPYPEN);
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
				redraw(hUserColorWin[ind]);
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
	insertLine[0].x = stitchSizePixels.x;
	insertLine[0].y = stitchSizePixels.y;
	insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
	insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
}

void delstchm() {

	unsigned ind, dst;

	rngadj();
	dst = groupStartStitch;
	for (ind = groupEndStitch; ind < header.stitchCount; ind++) {

		stitchBuffer[dst].attribute = stitchBuffer[ind].attribute;
		stitchBuffer[dst].x = stitchBuffer[ind].x;
		stitchBuffer[dst++].y = stitchBuffer[ind].y;
	}
	header.stitchCount = dst - 1;
}

void unsid() {

	formMenuChoice = 0;
	if (hSideMessageWin) {

		DestroyWindow(hSideMessageWin);
		hSideMessageWin = 0;
	}
}

void unbsho() {

	unsigned ind;

	if (rstMap(BAKSHO)) {

		for (ind = 0; ind < OLDVER; ind++)
			DestroyWindow(hBackupViewer[ind]);
	}
}

BOOL oldwnd(HWND hwnd) {

	unsigned ind;

	for (ind = 0; ind < 16; ind++) {

		if (hDefaultColorWin[ind] == hwnd || hUserColorWin[ind] == hwnd || hThreadSizeWin[ind] == hwnd)
			return 0;
	}
	for (ind = 0; ind < 9; ind++) {

		if (hButtonWin[ind] == hwnd)
			return 0;
	}
	if (hMainStitchWin == hwnd)
		return 0;
	if (hVerticalScrollBar == hwnd)
		return 0;
	if (hHorizontalScrollBar == hwnd)
		return 0;
	if (hColorBar == hwnd)
		return 0;
	if (hSpeedScrollBar == hwnd)
		return 0;
	return 1;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	if (oldwnd(hwnd))
		return DestroyWindow(hwnd);
	else {

		if (firstWin) {

			if (firstWin == hwnd)
				return 0;
			else
				return TRUE;
		} else {

			firstWin = hwnd;
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
	ShowWindow(hMainStitchWin, TRUE);
	unbsho();
	rstMap(SIDACT);
	unsid();
	rstMap(PRFACT);
	rstMap(WASRT);
	DestroyWindow(hPreferencesWindow);
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
	firstWin = 0;
	while (EnumChildWindows(hMainStitchWin, EnumChildProc, 0));
}

void rstdu() {

	deldu();
	rstMap(REDUSHO);
	rstMap(UNDUSHO);
	EnableMenuItem(hMainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	EnableMenuItem(hMainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
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

	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	SelectObject(stitchWindowDC, linePen);
	Polyline(stitchWindowDC, clipInsertBoxLine, 5);
	SetROP2(stitchWindowDC, R2_COPYPEN);
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
		dusel(stitchWindowDC);
}

void clpbox() {

	SIZE	clpx;
	double	tdub;

	px2stch();
	if (selectedPoint.x + clipboardRectSize.cx > unzoomedRect.x)
		selectedPoint.x = unzoomedRect.x - clipboardRectSize.cx;
	if (selectedPoint.y + clipboardRectSize.cy > unzoomedRect.y)
		selectedPoint.y = unzoomedRect.y - clipboardRectSize.cy;
	clipboardOrigin.x = selectedPoint.x;
	clipboardOrigin.y = selectedPoint.y;
	tdub = (double)stitchWindowClientRect.right / (zoomRect.right - zoomRect.left);
	clpx.cx = clipboardRectSize.cx*tdub + 0.5;
	clpx.cy = clipboardRectSize.cy*tdub + 0.5;
	stitchSizePixels.x = (selectedPoint.x - zoomRect.left)*tdub + 0.5;
	stitchSizePixels.y = stitchWindowClientRect.bottom - (selectedPoint.y - zoomRect.bottom)*tdub + 0.5 - clpx.cy;
	clipInsertBoxLine[0].x = clipInsertBoxLine[3].x = clipInsertBoxLine[4].x = stitchSizePixels.x;
	clipInsertBoxLine[0].y = clipInsertBoxLine[1].y = clipInsertBoxLine[4].y = stitchSizePixels.y;
	clipInsertBoxLine[1].x = clipInsertBoxLine[2].x = clipInsertBoxLine[0].x + clpx.cx;
	clipInsertBoxLine[2].y = clipInsertBoxLine[3].y = clipInsertBoxLine[0].y + clpx.cy;
	setMap(CLPSHO);
	duclp();
}

void lodclp(unsigned ind) {

	fPOINT		padj;
	unsigned	src, dst;

	if (ind != header.stitchCount)
		ind++;
	padj.x = clipboardOrigin.x - clipboardRect.left;
	padj.y = clipboardOrigin.y - clipboardRect.bottom;
	if (ind < (unsigned)header.stitchCount&&chkMap(INIT)) {

		src = header.stitchCount - 1;
		dst = header.stitchCount + clipboardStitchCount - 1;
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
	header.stitchCount += clipboardStitchCount;
	if (header.stitchCount)
		setMap(INIT);
}

void rSelbox() {

	SIZE	selx;
	double	tdub;

	unsel();
	px2stch();
	if (selectedPoint.x - selectBoxOffset.x + selectBoxSize.cx >= unzoomedRect.x)
		selectedPoint.x = unzoomedRect.x - selectBoxSize.cx + selectBoxOffset.x;
	if (selectedPoint.y - selectBoxOffset.y + selectBoxSize.cy >= unzoomedRect.y)
		selectedPoint.y = unzoomedRect.y - selectBoxSize.cy + selectBoxOffset.y;
	if (selectedPoint.x - selectBoxOffset.x < 0)
		selectedPoint.x = selectBoxOffset.x;
	if (selectedPoint.y - selectBoxOffset.y < 0)
		selectedPoint.y = selectBoxOffset.y;
	tdub = (double)stitchWindowClientRect.right / (zoomRect.right - zoomRect.left);
	selx.cx = selectBoxSize.cx*tdub + 0.5;
	selx.cy = selectBoxSize.cy*tdub + 0.5;
	stitchSizePixels.x = (selectedPoint.x - zoomRect.left - selectBoxOffset.x)*tdub + 0.5;
	stitchSizePixels.y = stitchWindowClientRect.bottom - (selectedPoint.y - zoomRect.bottom - selectBoxOffset.y)*tdub + 0.5 - selx.cy;
	formOutlineRectangle[0].x = formOutlineRectangle[6].x = formOutlineRectangle[7].x = formOutlineRectangle[8].x = stitchSizePixels.x;
	formOutlineRectangle[1].x = formOutlineRectangle[5].x = stitchSizePixels.x + selx.cx / 2;
	formOutlineRectangle[0].y = formOutlineRectangle[1].y = formOutlineRectangle[2].y = formOutlineRectangle[8].y = stitchSizePixels.y;
	formOutlineRectangle[3].y = formOutlineRectangle[7].y = stitchSizePixels.y + selx.cy / 2;
	formOutlineRectangle[4].y = formOutlineRectangle[5].y = formOutlineRectangle[6].y = stitchSizePixels.y + selx.cy;
	formOutlineRectangle[2].x = formOutlineRectangle[3].x = formOutlineRectangle[4].x = stitchSizePixels.x + selx.cx;
	setMap(SELSHO);
	dusel(stitchWindowDC);
}

void duSelbox() {

	px2stch();
	selectBoxSize.cx = stitchRangeRect.right - stitchRangeRect.left;
	selectBoxSize.cy = stitchRangeRect.top - stitchRangeRect.bottom;
	selectBoxOffset.x = selectedPoint.x - stitchRangeRect.left;
	selectBoxOffset.y = selectedPoint.y - stitchRangeRect.bottom;
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
		stlin[0].x = stlin[3].x = stlin[4].x = stitchSizePixels.x - iniFile.stitchSizePixels;
		stlin[0].y = stlin[1].y = stitchSizePixels.y - iniFile.stitchSizePixels;
		stlin[1].x = stlin[2].x = stitchSizePixels.x + iniFile.stitchSizePixels;
		stlin[2].y = stlin[3].y = stitchSizePixels.y + iniFile.stitchSizePixels;
		stlin[4].y = stitchSizePixels.y - iniFile.stitchSizePixels;
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

	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	SelectObject(stitchWindowDC, linePen);
	Polyline(stitchWindowDC, rotateBoxOutline, 5);
	Polyline(stitchWindowDC, rotateBoxCrossVertLine, 2);
	Polyline(stitchWindowDC, rotateBoxCrossHorzLine, 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void unrot() {

	if (rstMap(ROTSHO))
		durot();
}

void durotu() {

	SetROP2(stitchWindowDC, R2_NOTXORPEN);
	SelectObject(stitchWindowDC, linePen);
	Polyline(stitchWindowDC, rotateBoxToCursorLine, 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void unrotu() {

	if (rstMap(ROTUSHO))
		durotu();
}

void rotang(dPOINT rpnt, POINT* tpnt) {

	double	len, ang0;
	dPOINT	ipnt;
	long	dx, dy;

	dx = rpnt.x - rotationCenter.x;
	dy = rpnt.y - rotationCenter.y;
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
	ipnt.y = rotationCenter.y + len*sin(ang0);
	ipnt.x = rotationCenter.x + len*cos(ang0);
	sCor2px(ipnt, tpnt);
}

void rotang1(fPOINTATTRIBUTE rpnt, fPOINT* tpnt) {

	double	len, ang0;
	double	dx, dy;

	dx = rpnt.x - rotationCenter.x;
	dy = rpnt.y - rotationCenter.y;
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
	tpnt->y = rotationCenter.y + len*sin(ang0);
	tpnt->x = rotationCenter.x + len*cos(ang0);
}

void rotangf(fPOINT rpnt, fPOINT* tpnt) {

	double	len, ang0;
	double	dx, dy;

	dx = rpnt.x - rotationCenter.x;
	dy = rpnt.y - rotationCenter.y;
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
	tpnt->y = rotationCenter.y + len*sin(ang0);
	tpnt->x = rotationCenter.x + len*cos(ang0);
}

void rotpix(POINT rpnt, POINT* tpnt) {

	//won't handle vertical lines
	double	len, ang0;
	long	dx, dy;

	dx = rpnt.x - rotationCenterPixels.x;
	dy = rpnt.y - rotationCenterPixels.y;
	len = hypot(dx, dy);
	ang0 = atan2(dy, dx);
	ang0 -= rotateAnglePixels;
	tpnt->y = rotationCenterPixels.y + len*sin(ang0);
	tpnt->x = rotationCenterPixels.x + len*cos(ang0);
}

void rotflt(fPOINT* pnt) {

	double	len, ang0;
	double	dx, dy;

	dx = pnt->x - rotationCenter.x;
	dy = pnt->y - rotationCenter.y;
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
	pnt->y = rotationCenter.y + len*sin(ang0);
	pnt->x = rotationCenter.x + len*cos(ang0);
}

void rotstch(fPOINTATTRIBUTE* pnt) {

	double	len, ang0;
	double	dx, dy;

	dx = pnt->x - rotationCenter.x;
	dy = pnt->y - rotationCenter.y;
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
	pnt->y = rotationCenter.y + len*sin(ang0);
	pnt->x = rotationCenter.x + len*cos(ang0);
}

void ritrot() {

	POINT		tpnt;
	dPOINT		rotpnt;

	rotpnt.x = rotationRect.left;
	rotpnt.y = rotationRect.top;
	rotang(rotpnt, &tpnt);
	rotateBoxOutline[0].x = rotateBoxOutline[4].x = tpnt.x;
	rotateBoxOutline[0].y = rotateBoxOutline[4].y = tpnt.y;
	rotpnt.x = rotationCenter.x;
	rotang(rotpnt, &tpnt);
	rotateBoxCrossVertLine[0].x = tpnt.x;
	rotateBoxCrossVertLine[0].y = tpnt.y;
	rotpnt.x = rotationRect.right;
	rotang(rotpnt, &tpnt);
	rotateBoxOutline[1].x = tpnt.x;
	rotateBoxOutline[1].y = tpnt.y;
	rotpnt.y = rotationCenter.y;
	rotang(rotpnt, &tpnt);
	rotateBoxCrossHorzLine[1].x = tpnt.x;
	rotateBoxCrossHorzLine[1].y = tpnt.y;
	rotpnt.y = rotationRect.bottom;
	rotang(rotpnt, &tpnt);
	rotateBoxOutline[2].x = tpnt.x;
	rotateBoxOutline[2].y = tpnt.y;
	rotpnt.x = rotationCenter.x;
	rotang(rotpnt, &tpnt);
	rotateBoxCrossVertLine[1].x = tpnt.x;
	rotateBoxCrossVertLine[1].y = tpnt.y;
	rotpnt.x = rotationRect.left;
	rotang(rotpnt, &tpnt);
	rotateBoxOutline[3].x = tpnt.x;
	rotateBoxOutline[3].y = tpnt.y;
	rotpnt.y = rotationCenter.y;
	rotang(rotpnt, &tpnt);
	rotateBoxCrossHorzLine[0].x = tpnt.x;
	rotateBoxCrossHorzLine[0].y = tpnt.y;
	sCor2px(rotationCenter, &rotateBoxToCursorLine[0]);
	setMap(ROTSHO);
	durot();
}

void durcntr() {

	rotationCenter.x = midl(rotationRect.right, rotationRect.left);
	rotationCenter.y = midl(rotationRect.top, rotationRect.bottom);
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
		MoveMemory(&rotationRect, &SelectedForm->rectangle, sizeof(fRECTANGLE));
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
	if (SelectedForm->type == SAT)
		l_siz += SelectedForm->satinGuideCount * sizeof(SATCON);
	if (iseclp(closestFormToCursor))
		l_siz += SelectedForm->clipEntries * sizeof(fPOINT);
	if (isclpx(closestFormToCursor))
		l_siz += SelectedForm->lengthOrCount.clipCount * sizeof(fPOINT);
	if (istx(closestFormToCursor))
		l_siz += SelectedForm->fillInfo.texture.count * sizeof(TXPNT);
	return l_siz;
}

unsigned frmcnt(unsigned fpnt) {

	unsigned	at, cnt = 0, ind;

	lowerLeftStitch.x = lowerLeftStitch.y = (float)1e20;
	at = fpnt << 4;
	for (ind = 0; ind < header.stitchCount; ind++) {

		if ((stitchBuffer[ind].attribute&FRMSK) == at&&stitchBuffer[ind].attribute&TYPMSK)
			goto fskip;
	}
	return 0;
fskip:;
	frmstrt = ind;
	while (ind < header.stitchCount) {

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

	len = fileSize = sizeof(FORMCLIP) + sides * sizeof(fPOINT);
	if (SelectedForm->type == SAT)
		fileSize += SelectedForm->satinGuideCount * sizeof(SATCON);
	if (SelectedForm->fillType || SelectedForm->edgeType) {

		len += frmcnt(closestFormToCursor);
		fileSize += len * sizeof(fPOINTATTRIBUTE);
	}
	if (iseclp(closestFormToCursor))
		fileSize += SelectedForm->clipEntries * sizeof(fPOINT);
	if (isclpx(closestFormToCursor))
		fileSize += SelectedForm->lengthOrCount.clipCount * sizeof(fPOINT);
	if (istx(closestFormToCursor))
		fileSize += SelectedForm->fillInfo.texture.count * sizeof(TXPNT);
	return len;
}

unsigned lenclp()
{
	unsigned cod, ind, cnt;

	cnt = 0;
	cod = closestFormToCursor << FRMSHFT;
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		if ((stitchBuffer[ind].attribute&(FRMSK | NOTFRM)) == cod)
			cnt++;
	}
	return cnt;
}

void duclip() {

	unsigned	ind, ine, len = 0, inf, at, msiz = 0;
	SATCON*		l_sac;
	fPOINT*		l_clipData;
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
			hThrEdClip = RegisterClipboardFormat(thrEdClipFormat);
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
			SetClipboardData(hThrEdClip, ptrThrEdClipVoid);
			CloseClipboard();
		}
		return;
	}
	if (chkMap(BIGBOX))
		tabmsg(IDS_INSF);
	else {

		if (OpenClipboard(hWnd)) {

			EmptyClipboard();
			hThrEdClip = RegisterClipboardFormat(thrEdClipFormat);
			if (selectedFormCount) {

				for (ind = 0; ind < selectedFormCount; ind++) {

					closestFormToCursor = selectedFormList[ind];
					fvars(closestFormToCursor);
					len += sizfclp();
					msiz += fileSize;
				}
				ptrThrEdClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, msiz + len);
				clipboardFormsData = frmsref(ptrThrEdClipVoid);
				clipboardFormsData->clipType = CLP_FRMS;
				clipboardFormsData->formCount = selectedFormCount;
				tfrm = (FRMHED*)&clipboardFormsData[1];
				for (ind = 0; ind < selectedFormCount; ind++) {

					SelectedForm = &formList[selectedFormList[ind]];
					frmcpy(&tfrm[ind], &formList[selectedFormList[ind]]);
				}
				currentFormVertices = (fPOINT*)&tfrm[ind];
				inf = 0;
				for (ind = 0; ind < selectedFormCount; ind++) {

					SelectedForm = &formList[selectedFormList[ind]];
					for (ine = 0; ine < SelectedForm->sides; ine++) {

						currentFormVertices[inf].x = SelectedForm->vertices[ine].x;
						currentFormVertices[inf++].y = SelectedForm->vertices[ine].y;
					}
				}
				l_sac = (SATCON*)&currentFormVertices[inf];
				inf = 0;
				for (ind = 0; ind < selectedFormCount; ind++) {

					SelectedForm = &formList[selectedFormList[ind]];
					if (SelectedForm->type == SAT) {

						for (ine = 0; ine < SelectedForm->satinGuideCount; ine++) {

							l_sac[inf].start = SelectedForm->satinOrAngle.sac[ine].start;
							l_sac[inf++].finish = SelectedForm->satinOrAngle.sac[ine].finish;
						}
					}
				}
				l_clipData = (fPOINT*)&l_sac[inf];
				inf = 0;
				for (ind = 0; ind < selectedFormCount; ind++) {

					SelectedForm = &formList[selectedFormList[ind]];
					if (isclpx(selectedFormList[ind])) {

						for (ine = 0; ine < SelectedForm->lengthOrCount.clipCount; ine++) {

							l_clipData[inf].x = SelectedForm->angleOrClipData.clip[ine].x;
							l_clipData[inf++].y = SelectedForm->angleOrClipData.clip[ine].y;
						}
					}
					if (iseclp(selectedFormList[ind])) {

						for (ine = 0; ine < SelectedForm->clipEntries; ine++) {

							l_clipData[inf].x = SelectedForm->borderClipData[ine].x;
							l_clipData[inf++].y = SelectedForm->borderClipData[ine].y;
						}
					}
				}
				ptx = (TXPNT*)&l_clipData[inf];
				ine = 0;
				for (ind = 0; ind < selectedFormCount; ind++)
				{
					SelectedForm = &formList[selectedFormList[ind]];
					if (istx(selectedFormList[ind]))
					{
						MoveMemory(&ptx[ine], &txpnts[SelectedForm->fillInfo.texture.index], SelectedForm->fillInfo.texture.count * sizeof(TXPNT));
						tfrm[ind].fillInfo.texture.index = ine;
						ine += SelectedForm->fillInfo.texture.count;
					}
				}
				SetClipboardData(hThrEdClip, ptrThrEdClipVoid);
				CloseClipboard();
				clRmap(RMAPSIZ);
				for (ind = 0; ind < selectedFormCount; ind++)
					setr(selectedFormList[ind]);
				astch = &stitchBuffer[MAXSEQ];
				ine = 0;
				lowerLeftStitch.x = lowerLeftStitch.y = (float)1e30;
				for (ind = 0; ind < header.stitchCount; ind++) {

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
				if (header.stitchCount&&ine) {

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
					fileSize += sizeof(FORMCLIP);
					ptrThrEdClipVoid = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, fileSize);
					clipboardFormData = frmref(ptrThrEdClipVoid);
					clipboardFormData->clipType = CLP_FRM;
					frmcpy(&clipboardFormData->form, &formList[closestFormToCursor]);
					currentFormVertices = (fPOINT*)&clipboardFormData[1];
					for (ind = 0; ind < SelectedForm->sides; ind++) {

						currentFormVertices[ind].x = SelectedForm->vertices[ind].x;
						currentFormVertices[ind].y = SelectedForm->vertices[ind].y;
					}
					l_sac = (SATCON*)&currentFormVertices[sides];
					ind = 0;
					if (SelectedForm->type == SAT) {

						for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {

							l_sac[ind].start = SelectedForm->satinOrAngle.sac[ind].start;
							l_sac[ind].finish = SelectedForm->satinOrAngle.sac[ind].finish;
						}
					}
					mclp = (fPOINT*)&l_sac[ind];
					if (isclpx(closestFormToCursor)) {

						for (ind = 0; ind < SelectedForm->lengthOrCount.clipCount; ind++) {

							mclp[ind].x = SelectedForm->angleOrClipData.clip[ind].x;
							mclp[ind].y = SelectedForm->angleOrClipData.clip[ind].y;
						}
					}
					l_clipData = (fPOINT*)&mclp[ind];
					if (iseclpx(closestFormToCursor)) {

						for (ind = 0; ind < SelectedForm->clipEntries; ind++) {

							l_clipData[ind].x = SelectedForm->borderClipData[ind].x;
							l_clipData[ind].y = SelectedForm->borderClipData[ind].y;
						}
					}
					ptx = (TXPNT*)&l_clipData[ind];
					if (istx(closestFormToCursor))
					{
						ptxs = &txpnts[SelectedForm->fillInfo.texture.index];
						for (ind = 0; ind < SelectedForm->fillInfo.texture.count; ind++)
						{
							ptx[ind].line = ptxs[ind].line;
							ptx[ind].y = ptxs[ind].y;
						}
					}
					SetClipboardData(hThrEdClip, ptrThrEdClipVoid);
					if ((SelectedForm->fillType || SelectedForm->edgeType))
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
						while (ind < header.stitchCount) {

							if ((stitchBuffer[ind].attribute&FRMSK) == at && !(stitchBuffer[ind].attribute&NOTFRM))
								savclp(inf++, ind);
							ind++;
						}
						SetClipboardData(hClip, ptrClipVoid);
						ispcdclp();
					}
					CloseClipboard();
				} else {

					if (header.stitchCount&&chkMap(GRPSEL)) {

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

	for (ind = 0; ind < header.stitchCount; ind++) {

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
	header.stitchCount = ine;
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
	hGeneralNumberInputBox = CreateWindow(
		"STATIC",
		0,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		l_msgRct.bottom + 15,
		buttonWidthX3,
		buttonHeight,
		hMainStitchWin,
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

		ShowWindow(hSpeedScrollBar, FALSE);
		DestroyWindow(hSpeedScrollBar);
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
		while (ind<(unsigned)header.stitchCount - 1 && l_siz>smallStitchLength) {

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
			while (ine < (unsigned)header.stitchCount - 1) {

				do {

					if (!(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind].attribute&FRMSK) == at && !(stitchBuffer[ind].attribute&KNOTMSK)) {

						dx = stitchBuffer[ine].x - stitchBuffer[ind].x;
						dy = stitchBuffer[ine++].y - stitchBuffer[ind].y;
						l_siz = hypot(dx, dy);
					} else
						ine++;
				} while (l_siz < smallStitchLength&&ine < header.stitchCount);
				stitchBuffer[++ind].attribute = stitchBuffer[--ine].attribute;
				stitchBuffer[ind].x = stitchBuffer[ine].x;
				stitchBuffer[ind].y = stitchBuffer[ine++].y;
			}
			ind++;
			while (ine < header.stitchCount) {

				stitchBuffer[ind].attribute = stitchBuffer[ine].attribute;
				stitchBuffer[ind].x = stitchBuffer[ine].x;
				stitchBuffer[ind++].y = stitchBuffer[ine++].y;
			}
			header.stitchCount = ind;
			coltab();
		}
	} else {

		ine = ind + 1;
		selectedPoint.x = stitchBuffer[ind].x;
		selectedPoint.y = stitchBuffer[ind].y;
		for (inf = ine; inf < fin; inf++) {

			if (stitchBuffer[ine].attribute&KNOTMSK) {

				selectedPoint.x = stitchBuffer[ine].x;
				selectedPoint.y = stitchBuffer[ine].y;
				mvstch(ine++, inf);
			} else {

				dx = stitchBuffer[inf].x - selectedPoint.x;
				dy = stitchBuffer[inf].y - selectedPoint.y;
				l_siz = hypot(dx, dy);
				if (l_siz > smallStitchLength) {

					mvstch(ine++, inf);
					selectedPoint.x = stitchBuffer[inf].x;
					selectedPoint.y = stitchBuffer[inf].y;
				}
			}
		}
		while (inf < header.stitchCount)
			mvstch(ine++, inf++);
		header.stitchCount = ine;
		coltab();
	}
	rstAll();
	ritot(header.stitchCount);
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

	stitchBuffer[outputIndex].x = stitchBuffer[ind].x + knotStep.x*of;
	stitchBuffer[outputIndex].y = stitchBuffer[ind].y + knotStep.y*of;
	stitchBuffer[outputIndex++].attribute = knotAttribute;
}

void endknt(unsigned fin) {

	double		len;
	dPOINT		dif;
	unsigned	strt, ind;
	TCHAR*		tdat;

	strt = fin - 1;
	knotAttribute = stitchBuffer[strt].attribute | KNOTMSK;
	do {

		dif.x = stitchBuffer[fin].x - stitchBuffer[strt].x;
		dif.y = stitchBuffer[fin].y - stitchBuffer[strt].y;
		len = hypot(dif.x, dif.y);
		strt--;
	} while (!len);
	if (chkMap(FILDIR))
		tdat = knotAtLastOrder;
	else
		tdat = knotAtEndOrder;
	if (!(strt & 0x8000000)) {

		knotStep.x = 2 / len*dif.x;
		knotStep.y = 2 / len*dif.y;
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
	} while (len < 2 && fin < header.stitchCount);
	if (fin < header.stitchCount) {

		knotAttribute = stitchBuffer[strt].attribute | KNOTMSK;
		knotStep.x = 2 / len*dif.x;
		knotStep.y = 2 / len*dif.y;
		for (ind = 0; ind < 5; ind++)
			ofstch(strt, knotAtStartOrder[ind]);
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

	if (header.stitchCount > 4) {

		flg = 0;
		knotCount = 0;
		for (ind = 0; ind < (unsigned)header.stitchCount - 4; ind++) {

			if (stitchBuffer[ind].attribute&KNOTMSK) {

				if (!flg)
					knots[knotCount++] = ind;
			} else
				flg = 0;
		}
	} else knotCount = 0;
}

void delknt() {

	unsigned	ind, ine;

	ine = 0;
	for (ind = 0; ind < header.stitchCount; ind++) {

		if (!(stitchBuffer[ind].attribute&KNOTMSK))
			mvstch(ine++, ind);
	}
	header.stitchCount = ine;
}

BOOL isknots() {

	unsigned ind;

	for (ind = 0; ind < header.stitchCount; ind++) {

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

	while (strt<(unsigned)header.stitchCount - 1 && stlen(strt)>KNOTLEN)
		mvstch(outputIndex++, strt++);
	strtknt(strt);
	return strt;
}

void setknt() {

	unsigned	ind;

	outputIndex = MAXSEQ;
	mvstch(outputIndex++, 0);
	strtknt(0);
	if (stlen(0) > KNOTLEN)
		ind = kjmp(1);
	else
		ind = 1;
	rstMap(FILDIR);
	while (ind < (unsigned)header.stitchCount - 1) {

		mvstch(outputIndex++, ind);
		if (stlen(ind) > KNOTLEN) {

			endknt(ind);
			ind = kjmp(ind + 1);
			mvstch(outputIndex++, ind);
		}
		ind++;
	}
	setMap(FILDIR);
	endknt(ind);
	stitchBuffer[outputIndex - 1].attribute &= (~KNOTMSK);
	header.stitchCount = outputIndex - MAXSEQ;
	mvstchs(0, MAXSEQ, header.stitchCount);
}

unsigned srchknot(unsigned src) {

	while (knots[activePointIndex] < src&&activePointIndex < knotCount)
		activePointIndex++;
	activePointIndex--;
	if (((knots[activePointIndex] > src) ? (knots[activePointIndex] - src) : (src - knots[activePointIndex])) < 5) {

		activePointIndex++;
		if (((knots[activePointIndex] > src) ? (knots[activePointIndex] - src) : (src - knots[activePointIndex])) < 5)
			return 0;
		else
			return 2;
	} else {

		activePointIndex++;
		if (((knots[activePointIndex] > src) ? (knots[activePointIndex] - src) : (src - knots[activePointIndex])) < 5)
			return 1;
		else
			return 3;
	}
}

void chkncol() {

	unsigned	src, col, tcol, cod;

	outputIndex = MAXSEQ;
	col = stitchBuffer[0].attribute&COLMSK;
	rstMap(FILDIR);
	for (src = 0; src < header.stitchCount; src++) {

		tcol = stitchBuffer[src].attribute&COLMSK;
		if (tcol == col)
			mvstch(outputIndex++, src);
		else {

			col = stitchBuffer[src].attribute&COLMSK;
			cod = srchknot(src);
			if (cod & 1) {

				endknt(src - 1);
			}
			mvstch(outputIndex++, src);
			if (cod & 2)
				strtknt(src);
		}
	}
	header.stitchCount = outputIndex - MAXSEQ;
	mvstchs(0, MAXSEQ, header.stitchCount);
}

void setknots()
{
	if (header.stitchCount)
	{
		savdo();
		delknt();
		setknt();
		fndknt();
		chkncol();
		coltab();
		setMap(RESTCH);
		ritot(header.stitchCount);
	}
}

void lodbmp() {

	TCHAR*	pchr;

	if (pcsBMPFileName[0]) {

		DeleteObject(hBmp);
		ReleaseDC(hWnd, bitmapDC);
	}
	if (GetOpenFileName(&obn)) {

		untrace();
		pchr = strrchr(usrBMPFileName, '\\') + 1;
		// PCS file can only store a 16 character filename?
		strncpy_s(pcsBMPFileName, pchr, sizeof(pcsBMPFileName));
		defbNam();
		bfil();
		setMap(RESTCH);
	}
}

void duhbit(unsigned cod)
{
	CheckMenuItem(hMainMenu, ID_HIDBIT, MF_BYCOMMAND | cod);
	CheckMenuItem(hMainMenu, ID_HIDBITF, MF_BYCOMMAND | cod);
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

		while ((stitchBuffer[runPoint].attribute&COLMSK) != activeColor&&runPoint < fin - 1)
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
		SelectObject(stitchWindowDC, userPen[col]);
		Polyline(stitchWindowDC, movieLine, ind);
		if (!flg)
			runPoint--;
	} else {

		ind = 0;
		col = stitchBuffer[runPoint].attribute&COLMSK;
		SelectObject(stitchWindowDC, userPen[col]);
		while (ind < stitchesPerFrame && (runPoint + 1 < fin - 1) && ((stitchBuffer[runPoint].attribute&COLMSK) == col)) {

			stch2px1(runPoint++);
			movieLine[ind].x = stitchSizePixels.x;
			movieLine[ind++].y = stitchSizePixels.y;
		}
		runPoint--;
		Polyline(stitchWindowDC, movieLine, ind);
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
		drwlstch(header.stitchCount - 1);
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
		mov		edi, outputIndex
		mov		ecx, cnt
		rep		movsb
		mov		outputIndex, edi
	}
#else
	memcpy((void *)outputIndex, src, cnt);
	outputIndex += cnt;
#endif
}

unsigned bufref() {

#if  __UseASM__
	_asm {

		mov		eax, outputIndex
		sub		eax, offset bseq
	}
#else
	return (unsigned)outputIndex - (unsigned)bseq;
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

	outputIndex = (unsigned)&bseq;
	sthed.headerType = 0x2746872;
	sthed.fileLength = header.stitchCount * sizeof(fPOINTATTRIBUTE) + sizeof(STRHED) + 16;
	sthed.stitchCount = header.stitchCount;
	sthed.hoopType = iniFile.hoopType;
	len = 0; slen = 0; elen = 0;
	strcpy_s(extendedHeader.modifierName, iniFile.designerName);
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
	sthed.FormPointsLen = sizeof(STRHED) + header.stitchCount * sizeof(fPOINTATTRIBUTE) + 164;
	sthed.dlineLen = sizeof(fPOINT)*len;
	sthed.clipboardDataLen = sizeof(fPOINT)*elen;
	durit(&sthed, sizeof(STRHED));
	extendedHeader.auxFormat = iniFile.auxFileType;
	extendedHeader.hoopSizeX = iniFile.hoopSizeX;
	extendedHeader.hoopSizeY = iniFile.hoopSizeY;
	extendedHeader.texturePointCount = txad;
	durit(&extendedHeader, sizeof(STREX));
	durit(stitchBuffer, header.stitchCount * sizeof(fPOINTATTRIBUTE));
	if (!pcsBMPFileName[0]) {

		for (ind = 0; ind < 16; ind++)
			pcsBMPFileName[ind] = 0;
	}
	durit(pcsBMPFileName, 16);
	durit(&backgroundColor, 4);
	durit(userColor, 64);
	durit(customColor, 64);
	for (ind = 0; ind < 16; ind++)
		msgbuf[ind] = threadSize[ind][0];
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

	if (chkattr(fileName))
		return;
	if (!rstMap(IGNAM)) {

		hndl = FindFirstFile(geName, &fdat);
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
	hFile = CreateFile(thrName, (GENERIC_WRITE), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {

		crmsg(thrName);
		hFile = 0;
	} else {

		dubuf();
		WriteFile(hFile, bseq, bufref(), &wrot, 0);
		if (wrot != (unsigned long)bufref()) {

			sprintf_s(msgbuf, sizeof(msgbuf), "File Write Error: %s\n", thrName);
			shoMsg(msgbuf);
		}
		CloseHandle(hFile);
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
		for (ind = 0; ind < header.stitchCount; ind++)
		{
			if ((stitchBuffer[ind].attribute&(FRMSK | NOTFRM)) != cod)
			{
				stitchBuffer[ine].x = stitchBuffer[ind].x;
				stitchBuffer[ine].y = stitchBuffer[ind].y;
				stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
				ine++;
			}
		}
		header.stitchCount = ine;
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
		for (ind = 0; ind < header.stitchCount; ind++) {

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

		for (ind = 0; ind < header.stitchCount; ind++) {

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

	strcpy_s(designerName, iniFile.designerName);
	formIndex = header.stitchCount = fltad = clpad = satkad = txad = 0;
	rstMap(GMRK);
	rstAll();
	coltab();
	zumhom();
	strcpy_s(designerName, iniFile.designerName);
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRDBY], thrName, designerName);
	SetWindowText(hWnd, msgbuf);
}

BOOL wastch() {

	unsigned ind;

	for (ind = 0; ind < header.stitchCount; ind++) {

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
	for (ind = 0; ind < header.stitchCount; ind++)
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
		SelectedForm->sides -= (selectedFormPoints.pointCount + 1);
		frmout(closestFormToCursor);
		if (SelectedForm->type == SAT)
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
			if (header.stitchCount > 2) {

				delstch1(closestPointIndex);
				if (!stch2px(closestPointIndex))
					movbox();
			} else {

				header.stitchCount = 0;
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

			SelectedForm = &formList[closestFormToCursor];
			switch (SelectedForm->type) {

			case LIN:

				if (SelectedForm->fillType == CONTF) {

					if (closestVertexToCursor == SelectedForm->angleOrClipData.sat.start || closestVertexToCursor == SelectedForm->angleOrClipData.sat.finish) {

						delmfil(SelectedForm->fillColor);
						SelectedForm->fillType = 0;
						coltab();
						setMap(RESTCH);
						return;
					}
					if (SelectedForm->angleOrClipData.sat.start > closestVertexToCursor)
						SelectedForm->angleOrClipData.sat.start--;
					if (SelectedForm->angleOrClipData.sat.finish > closestVertexToCursor)
						SelectedForm->angleOrClipData.sat.finish--;
				}
				break;

			case SAT:

				if (closestVertexToCursor <= 1) {

					if (SelectedForm->attribute&FRMEND) {

						if (SelectedForm->wordParam)
							SelectedForm->wordParam = 0;
						else
							SelectedForm->attribute &= 0xfe;
						satflg = 1;
						goto deldun;
					}
				}
				if (wordParam) {

					if (closestVertexToCursor == (unsigned)wordParam || closestVertexToCursor == (unsigned)wordParam + 1) {

						SelectedForm->wordParam = 0;
						satflg = 1;
						goto deldun;
					}
				}
				for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {

					if (SelectedForm->satinOrAngle.sac[ind].start == closestVertexToCursor || SelectedForm->satinOrAngle.sac[ind].finish == closestVertexToCursor) {

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
	if (header.stitchCount) {

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

			hSpeedScrollBar = CreateWindow(
				"SCROLLBAR",
				0,
				SBS_HORZ | WS_CHILD | WS_VISIBLE,
				buttonWidthX3,
				0,
				stitchWindowSize.x,
				SCROLSIZ,
				hWnd,
				NULL,
				hInst,
				NULL);
		}
		if (chkMap(ZUMED))
			tdub = header.stitchCount*zoomFactor*zoomFactor;
		else
			tdub = header.stitchCount;
		if (!chkMap(WASPAT))
			delay = MOVITIM * 10000 / tdub;
		if (delay < MINDELAY)
			delay = MINDELAY;
		if (delay > MAXDELAY)
			delay = MAXDELAY;
		scrollInfo.cbSize = sizeof(scrollInfo);
		scrollInfo.fMask = SIF_ALL;
		scrollInfo.nMax = MAXDELAY;
		scrollInfo.nMin = MINDELAY;
		scrollInfo.nPage = 1;
		scrollInfo.nPos = MAXDELAY - delay;
		SetScrollInfo(hSpeedScrollBar, SB_CTL, &scrollInfo, TRUE);
		FillRect(stitchWindowDC, &stitchWindowClientRect, hBackgroundBrush);
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
		clipBuffer[0].attribute = activeColor | playcod;
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

	if (hFile || chkMap(THUMSHO)) {

		setMap(ZUMED);
		movStch();
		FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
		dx = (stitchWindowClientRect.right >> 1);
		dy = (stitchWindowClientRect.bottom >> 1);
		for (ind = 0; ind < OLDVER; ind++) {

			if (ind & 2)
				vloc = dy;
			else
				vloc = 0;
			hBackupViewer[ind] = CreateWindow(
				"STATIC",
				"",
				SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
				dx*(ind & 1) + buttonWidthX3,
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

		if (thumbnailsSelected[fileVersionIndex]) {

			if (chkMap(RBUT)) {

				strcpy_s(insertFileName, thumbnailsSelected[fileVersionIndex]);
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

				strcpy_s(fileName, defaultDirectory);
				pchr = &fileName[strlen(fileName) - 1];
				if (pchr[0] != '\\') {

					pchr[1] = '\\';
					pchr[2] = 0;
				}
				strcat_s(fileName, thumbnailsSelected[fileVersionIndex]);
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
		DestroyWindow(hBackupViewer[ind]);
	strcpy_s(tnaml, thrName);
	strcpy_s(tnamx, thrName);
	ind = duth(tnaml);
	tnaml[ind] = fileVersionIndex + 's';
	tnamx[ind] = 'x';
	MoveFile(thrName, tnamx);
	MoveFile(tnaml, thrName);
	MoveFile(tnamx, tnaml);
	strcpy_s(fileName, thrName);
	setMap(REDOLD);
	nuFil();
	DeleteFile(tnamx);
}

void thumbak() {

	unsigned ind;

	for (ind = 0; ind < OLDVER; ind++)
		DestroyWindow(hBackupViewer[ind]);
	getbak();
}

void movbak(TCHAR src, TCHAR dst) {

	TCHAR	strsrc[_MAX_PATH];
	TCHAR	strdst[_MAX_PATH];

	unsigned ind = duth(thrName);

	strcpy_s(strsrc, thrName);
	strcpy_s(strdst, thrName);
	strsrc[ind] = (TCHAR)src;
	strdst[ind] = (TCHAR)dst;
	DeleteFile(strdst);
	MoveFile(strsrc, strdst);
}

void purg() {

	TCHAR		tnam[_MAX_PATH];
	unsigned	pind, ind;

	if (hFile) {

		strcpy_s(tnam, thrName);
		pind = duth(tnam);
		for (ind = 1; ind < 6; ind++) {

			tnam[pind] = (TCHAR)ind + 'r';
			DeleteFile(tnam);
		}
	}
}

void purgdir() {

	setMap(PRGMSG);
	sprintf_s(msgbuf, sizeof(msgbuf), "Delete all backups in %s\n", defaultDirectory);
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
	strcpy_s(tnam, defaultDirectory);
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

	return chkwnd(hOKButton);
}

void mv2f() {

	unsigned	at, lo, hi, ind;

	if (rstMap(FORMSEL)) {

		savdo();
		lo = 0;
		hi = MAXPCS >> 1;
		at = closestFormToCursor << 4;
		for (ind = 0; ind < header.stitchCount; ind++) {

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
			for (ind = groupEndStitch; ind < header.stitchCount; ind++) {

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
		for (ind = 0; ind < header.stitchCount; ind++) {

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
			for (ind = groupEndStitch; ind < header.stitchCount; ind++) {

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

	for (ind = 0; ind < header.stitchCount; ind++)
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

		for (ind = 0; ind < header.stitchCount; ind++) {

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
		header.stitchCount = ine;
	} else {

		for (ind = 0; ind < header.stitchCount; ind++) {

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

	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	if (closestVertexToCursor) {

		if (closestVertexToCursor == (unsigned)SelectedForm->sides - 1 && SelectedForm->type == LIN)
			Polyline(stitchWindowDC, mvlin, 2);
		else
			Polyline(stitchWindowDC, mvlin, 3);
	} else {

		mvlin[2].x = formLines[1].x;
		mvlin[2].y = formLines[1].y;
		if (SelectedForm->type == LIN)
			Polyline(stitchWindowDC, &mvlin[1], 2);
		else
			Polyline(stitchWindowDC, mvlin, 3);
	}
	SetROP2(stitchWindowDC, R2_COPYPEN);
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
	sfCor2px(currentFormVertices[selectedFormPoints.finish], &endPointCross);
	setMap(SHOPSEL);
	dupsel(stitchWindowDC);
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
		for (ind = 0; ind < header.stitchCount; ind++)
			rotstch(&stitchBuffer[ind]);
		for (ind = 0; ind < formIndex; ind++)
			frmout(ind);
		selal();
		return;
	}
	if (rstMap(FRMSROT)) {

		tang = rotationAngle;
		trot.x = rotationCenter.x;
		trot.y = rotationCenter.y;
		for (ind = 0; ind < selectedFormCount; ind++) {

			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			for (ine = 0; ine < sides; ine++)
				rotflt(&currentFormVertices[ine]);
			frmout(closestFormToCursor);
			refilfn();
			rotationAngle = tang;
			rotationCenter.x = trot.x;
			rotationCenter.y = trot.y;
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
			selin(groupStartStitch, groupEndStitch, stitchWindowDC);
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

			len = SelectedForm->clipEntries;
			oclp(SelectedForm->borderClipData, len);
		} else {

			if (isclp(closestFormToCursor)) {

				len = SelectedForm->lengthOrCount.clipCount;
				oclp(SelectedForm->angleOrClipData.clip, len);
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

		formLines[ind].x = selectedFormsLine[ind << 1].x;
		formLines[ind].y = selectedFormsLine[ind << 1].y;
	}
	formLines[4].x = formLines[0].x;
	formLines[4].y = formLines[0].y;
	if (minlen < CLOSENUF) {

		for (ind = 0; ind < 4; ind++) {

			stretchBoxLine[ind].x = selectedFormsLine[ind << 1].x;
			stretchBoxLine[ind].y = selectedFormsLine[ind << 1].y;
		}
		stretchBoxLine[4].x = stretchBoxLine[0].x;
		stretchBoxLine[4].y = stretchBoxLine[0].y;
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

		selectedFormsSize.x = selectedFormsRectangle.right - selectedFormsRectangle.left;
		selectedFormsSize.y = selectedFormsRectangle.bottom - selectedFormsRectangle.top;
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
	for (ind = 0; ind < header.stitchCount; ind++) {

		if (!(stitchBuffer[ind].attribute&NOTFRM)) {

			stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
			stitchBuffer[ine].x = stitchBuffer[ind].x;
			stitchBuffer[ine++].y = stitchBuffer[ind].y;
		}
	}
	header.stitchCount = ine;
	coltab();
	setMap(RESTCH);
}

void setmov() {

	if (chkMap(SELBOX)) {

		moveAnchor = closestPointIndex;
		setMap(MOVSET);
		setMap(RESTCH);
	}
}

void dufsel()
{
	unsigned strt, fin;

	if (lastFormSelected > closestFormToCursor)
	{
		strt = closestFormToCursor;
		fin = lastFormSelected;
	} else
	{
		strt = lastFormSelected;
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
				lastFormSelected = closestFormToCursor + 1;
				selectedFormList[1] = lastFormSelected;
				selectedFormCount = 2;
			} else
				return;
		} else {

			if (selectedFormCount)
			{
				if (lastFormSelected < formIndex - 1)
				{
					lastFormSelected++;
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
			while (closestPointIndex < (unsigned)header.stitchCount - 1 && (stitchBuffer[closestPointIndex].attribute&ATMSK) == at)
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
				lastFormSelected = closestFormToCursor - 1;
				selectedFormList[1] = lastFormSelected;
				selectedFormCount = 2;
			} else
				return;
		} else {

			if (selectedFormCount)
			{
				if (lastFormSelected)
				{
					lastFormSelected--;
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

	if (dst < (unsigned)header.stitchCount - 1)
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
			if (movstchs(moveAnchor, groupStartStitch, groupEndStitch)) {

				coltab();
				setMap(RESTCH);
			}
		} else {

			if (chkMap(SELBOX)) {

				if (movstchs(moveAnchor, closestPointIndex, closestPointIndex + 1)) {

					setMap(RESTCH);
					coltab();
				}
			}
		}
	}
}

void vuthrds() {

	if (GetMenuState(hViewMenu, ID_VUTHRDS, MF_BYCOMMAND)&MF_CHECKED) {

		CheckMenuItem(hMainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		rstMap(THRDS);
	} else {

		CheckMenuItem(hMainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_CHECKED);
		setMap(THRDS);
	}
	setMap(RESTCH);
}

void vuselthr() {

	if (GetMenuState(hViewMenu, ID_VUSELTHRDS, MF_BYCOMMAND)&MF_CHECKED) {

		CheckMenuItem(hMainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		rstMap(COL);
	} else {

		CheckMenuItem(hMainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_CHECKED);
		setMap(COL);
	}
	setMap(RESTCH);
}

void colchk() {

	unsigned col, cind, ind;

	col = stitchBuffer[0].attribute&COLMSK;
	cind = 0;
	for (ind = 0; ind < header.stitchCount; ind++) {

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
		if (len > iniFile.maxStitchLength) {

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
	while (src < header.stitchCount)
		mvstch(dst++, src++);
	tstchs0 = &stitchBuffer[MAXPCS >> 1];
	tstchs1 = &stitchBuffer[strt];
	cnt = dst - (MAXPCS >> 1);
	for (src = 0; src < cnt; src++)
		tstchs1[src] = tstchs0[src];
	header.stitchCount = strt + cnt;
	return adcnt;
}

void rembig() {

	RANGE rng;
	unsigned ind;

	if (userStitchLength < iniFile.maxStitchLength) {

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
			if (groupEndStitch < header.stitchCount)
				groupEndStitch++;
			if (closestPointIndex < groupStitchIndex)
				groupStitchIndex += makbig(groupStartStitch, groupEndStitch);
			else
				closestPointIndex += makbig(groupStartStitch, groupEndStitch);
			grpAdj();
			goto bigdun;
		}
		makbig(0, header.stitchCount);

	bigdun:;
		coltab();
		setMap(RESTCH);
	} else
		tabmsg(IDS_REM1);
}

void duselrng() {

	selectedRange.start = 0;
	selectedRange.finish = header.stitchCount;
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

	if (stitchBuffer[p_stind].x >= stitchRangeRect.left
		&&stitchBuffer[p_stind].x <= stitchRangeRect.right
		&&stitchBuffer[p_stind].y >= stitchRangeRect.bottom
		&&stitchBuffer[p_stind].y <= stitchRangeRect.top)

		return 1;
	else
		return 0;
}

BOOL finrng(unsigned find) {

	unsigned cod;

	if (formList[find].rectangle.left >= stitchRangeRect.left
		&&formList[find].rectangle.right <= stitchRangeRect.right
		&&formList[find].rectangle.bottom >= stitchRangeRect.bottom
		&&formList[find].rectangle.top <= stitchRangeRect.top) {

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

			for (ind = 0; ind < header.stitchCount; ind++) {

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

			for (ind = header.stitchCount; ind != 0; ind--) {

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

	if (header.stitchCount) {

		rct->bottom = rct->top = rotatedStitches[0].y;
		rct->left = rct->right = rotatedStitches[0].x;
		for (ind = 1; ind < header.stitchCount; ind++) {

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

		if (!header.stitchCount) {

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
	if (header.stitchCount || fltad || chkMap(HUPEX)) {

		if (checkHoopRect.left<0 ||
			checkHoopRect.right>iniFile.hoopSizeX ||
			checkHoopRect.bottom<0 ||
			checkHoopRect.top>iniFile.hoopSizeY) {

			setMap(HUPEX);
		}
		if (chkMap(HUPEX)) {

			tsiz.x = checkHoopRect.right - checkHoopRect.left;
			tsiz.y = checkHoopRect.top - checkHoopRect.bottom;
			if (tsiz.x > iniFile.hoopSizeX) {

				iniFile.hoopSizeX = tsiz.x;
				setMap(HUPCHNG);
			}
			if (tsiz.y > iniFile.hoopSizeY) {

				iniFile.hoopSizeY = tsiz.y;
				setMap(HUPCHNG);
			}
			descntr.x = tsiz.x / 2 + checkHoopRect.left;
			descntr.y = tsiz.y / 2 + checkHoopRect.bottom;
			hupcntr.x = iniFile.hoopSizeX / 2;
			hupcntr.y = iniFile.hoopSizeY / 2;
			dif.x = hupcntr.x - descntr.x;
			dif.y = hupcntr.y - descntr.y;
			for (ind = 0; ind < header.stitchCount; ind++) {

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
			unzoomedRect.x = iniFile.hoopSizeX;
			unzoomedRect.y = iniFile.hoopSizeY;
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

	if (fpnt < thumbnailDisplayCount&&thumbnailsSelected[fpnt]) {

		strcpy_s(buf, thumbnailsSelected[fpnt]);
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

		barnam(hButtonWin[HNUM], ind);
		break;

	case 1:

		barnam(hButtonWin[HTOT], ind);
		break;

	case 2:

		barnam(hButtonWin[HMINLEN], ind);
		break;

	case 3:

		barnam(hButtonWin[HMAXLEN], ind);
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
	// ToDo is this correct? types do not match
	thumbnailNames = (TCHAR*)oseq;
	ptrThumbnails = (TCHAR**)&oseq[MAXSEQ >> 1];

	SetCurrentDirectory(defaultDirectory);
	strcpy_s(searchName, defaultDirectory);
	pchr = &searchName[strlen(searchName) - 1];
	if (pchr[0] != '\\') {

		pchr[1] = '\\';
		pchr[2] = 0;
	}
	strcat_s(searchName, "*.thr");
	shndl = FindFirstFile(searchName, &fdat);
	if (shndl == INVALID_HANDLE_VALUE) {

		sprintf_s(msgbuf, sizeof(msgbuf), "Can't find %s\n", searchName);
		shoMsg(msgbuf);
		unthum();
	} else {

		ine = 0;
		ptrThumbnails[0] = thumbnailNames;
		strcpy(ptrThumbnails[0], fdat.cFileName);
		ine += strlen(fdat.cFileName) + 1;
		ptrThumbnails[1] = &thumbnailNames[ine];
		ind = 1;
		while (FindNextFile(shndl, &fdat)) {

			strcpy(ptrThumbnails[ind], fdat.cFileName);
			ine += strlen(fdat.cFileName) + 1;
			ptrThumbnails[++ind] = &thumbnailNames[ine];
		}
		FindClose(shndl);
		thumbnailCount = ind;
		qsort((void*)&ptrThumbnails[0], ind, 4, strcomp);
		ind = thumbnailIndex = 0;
		while (ind < 4 && thumbnailIndex < thumbnailCount&&ind < thumbnailCount) {

			thumbnailsSelected[ind] = ptrThumbnails[ind];
			ind++;
		}
		thumbnailIndex = thumbnailDisplayCount = ind;
		while (ind < 4 && ind < thumbnailCount)
			rthumnam(ind++);
		setMap(THUMSHO);
		thumbnailSearchString[0] = 0;
		SetWindowText(hButtonWin[HBOXSEL], "");
		butxt(HBOXSEL, "");
		vubak();
		setMap(RESTCH);
	}
}
#pragma warning(pop)

void nuthsel() {

	unsigned ind, len, bakind;

	if (thumbnailIndex < thumbnailCount) {

		bakind = thumbnailIndex;
		ind = 0;
		len = strlen(thumbnailSearchString);
		setMap(RESTCH);
		if (len) {

			while (ind < 4 && thumbnailIndex < thumbnailCount) {

				if (!strncmp(thumbnailSearchString, ptrThumbnails[thumbnailIndex], len)) {

					thumbnailsSelected[ind] = ptrThumbnails[thumbnailIndex];
					redraw(hBackupViewer[ind++]);
				}
				thumbnailIndex++;
			}
		} else {

			while (ind < 4 && thumbnailIndex < thumbnailCount) {

				thumbnailsSelected[ind] = ptrThumbnails[thumbnailIndex];
				redraw(hBackupViewer[ind++]);
				thumbnailIndex++;
			}
		}
		if (ind) {

			thumbnailDisplayCount = ind;
			while (ind < 4)
				rthumnam(ind++);
		} else
			thumbnailIndex = bakind;
	}
}

void nuthbak(unsigned cnt) {

	unsigned len;

	if (thumbnailIndex) {

		len = strlen(thumbnailSearchString);
		if (len) {

			while (cnt&&thumbnailIndex < MAXFORMS) {

				if (thumbnailIndex) {

					thumbnailIndex--;
					if (!strncmp(thumbnailSearchString, ptrThumbnails[thumbnailIndex], len))
						cnt--;
				} else
					break;
			}
		} else
			thumbnailIndex -= cnt;
		if (thumbnailIndex > MAXFORMS)
			thumbnailIndex = 0;
		nuthsel();
	}
}

void nuthum(TCHAR chr) {

	unsigned ind;

	ind = strlen(thumbnailSearchString);
	if (ind < 16) {

		setMap(RESTCH);
		thumbnailSearchString[ind++] = chr;
		thumbnailSearchString[ind] = 0;
		butxt(HBOXSEL, thumbnailSearchString);
		thumbnailIndex = 0;
		nuthsel();
	}
}

void bakthum() {

	unsigned ind;

	ind = strlen(thumbnailSearchString);
	if (ind) {

		setMap(RESTCH);
		thumbnailSearchString[--ind] = 0;
		thumbnailIndex = 0;
		butxt(HBOXSEL, thumbnailSearchString);
		nuthsel();
	}
}

void selalstch() {

	if (header.stitchCount) {

		closestPointIndex = 0;
		groupStitchIndex = header.stitchCount - 1;
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

	off.x = insertSize.x >> 1;
	off.y = insertSize.y >> 1;

	formLines[0].x = formLines[3].x = formLines[4].x = ipnt.x - off.x;
	formLines[1].x = formLines[2].x = ipnt.x + off.x;
	formLines[0].y = formLines[1].y = formLines[4].y = ipnt.y - off.y;
	formLines[2].y = formLines[3].y = ipnt.y + off.y;
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
		customFilter,			//lpstrCustomFilter 
		_MAX_PATH,				//nMaxCustFilter 
		0,						//nFilterIndex 
		insertFileName,			//lpstrFile 
		_MAX_PATH,				//nMaxFile 
		0,						//lpstrFileTitle 
		0,						//nMaxFileTitle 
		defaultDirectory,					//lpstr	ialDir 
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

		hInsertedFile = CreateFile(insertFileName, (GENERIC_READ), 0, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hInsertedFile == INVALID_HANDLE_VALUE) {

			filnopn(IDS_FNOPN, insertFileName);
			hFile = 0;
			CloseHandle(hInsertedFile);
		} else {

			insertFileStitchCount = header.stitchCount;
			if (isthr(insertFileName)) {

				ReadFile(hInsertedFile, (STRHED*)&thed, sizeof(STRHED), &bytesRead, NULL);
				if ((thed.headerType & 0xffffff) != 0x746872)
					tabmsg(IDS_NOTHR);
				else {

					vervar = (thed.headerType & 0xff000000) >> 24;
					if (vervar) {
#define FRMW 5
#define HANDW 4
#define FRMPW 2
#define STCHW 1			
						gethand(stitchBuffer, header.stitchCount);
						homscor = (double)
							formIndex*FRMW +
							gethand(stitchBuffer, header.stitchCount)*HANDW +
							fltad*FRMPW +
							header.stitchCount*STCHW;
						ReadFile(hInsertedFile, (STREX*)&thedx, sizeof(STREX), &bytesRead, 0);
					}
					savdo();
					ReadFile(hInsertedFile, (fPOINTATTRIBUTE*)&stitchBuffer[header.stitchCount], thed.stitchCount * sizeof(fPOINTATTRIBUTE), &bytesRead, NULL);
					SetFilePointer(hInsertedFile, 164, 0, FILE_CURRENT);
					trct.left = trct.bottom = (float)1e9;
					trct.top = trct.right = (float)1e-9;
					codof = formIndex << FRMSHFT;
					insflt = fltad; insertFileFormIndex = formIndex;
					if (thed.pointCount) {

						if (vervar < 2) {

							frmlstx = (FRMHEDO*)&bseq;
							ReadFile(hInsertedFile, (FRMHEDO*)&bseq, thed.formCount * sizeof(FRMHEDO), &bytesRead, 0);
							if (bytesRead != thed.formCount * sizeof(FRMHEDO)) {

								formIndex = bytesRead / sizeof(FRMHEDO);
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
							ReadFile(hInsertedFile, (FRMHED*)&formList[formIndex], thed.formCount * sizeof(FRMHED), &bytesRead, 0);
						ReadFile(hInsertedFile, (fPOINT*)&formPoints[fltad], thed.pointCount * sizeof(fPOINT), &bytesRead, 0);
						ReadFile(hInsertedFile, (SATCON*)&satks[satkad], thed.dlineCount * sizeof(SATCON), &bytesRead, 0);
						ReadFile(hInsertedFile, (fPOINT*)&clipboardPoints[clpad], thed.clipboardDataCount * sizeof(fPOINT), &bytesRead, 0);
						CloseHandle(hInsertedFile);
						hInsertedFile = 0;
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

						for (ind = header.stitchCount; ind < (unsigned)header.stitchCount + thed.stitchCount; ind++) {

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
							gethand(&stitchBuffer[header.stitchCount], thed.stitchCount)*HANDW +
							thed.FormPointsLen*FRMPW +
							thed.stitchCount*STCHW;
						if (filscor > homscor) {

							for (ind = 0; ind < 50; ind++)
								extendedHeader.creatorName[ind] = thedx.creatorName[ind];
							redfnam(designerName);
							sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRDBY], thrName, designerName);
							SetWindowText(hWnd, msgbuf);
						}
					}
					insertCenter.x = (trct.right - trct.left) / 2 + trct.left;
					insertCenter.y = (trct.top - trct.bottom) / 2 + trct.bottom;
					header.stitchCount += thed.stitchCount;
					tsiz.x = trct.right - trct.left;
					tsiz.y = trct.top - trct.bottom;
					ratsr();
					insertSize.x = tsiz.x*horizontalRatio;
					insertSize.y = tsiz.y*horizontalRatio;
					tpnt.x = stitchWindowClientRect.right >> 1;
					tpnt.y = stitchWindowClientRect.bottom >> 1;
					insflin(tpnt);
					newFormVertexCount = 5;
					setMap(SHOFRM);
					setMap(INSFIL);
					dufrm();
				}
			} else {

				ReadFile(hInsertedFile, (PCSHEADER*)&tphed, 0x46, &bytesRead, NULL);
				if (header.leadIn == 0x32 && header.colorCount == 16) {

					savdo();
					tbuf = (PCSTCH*)bseq;
					ReadFile(hInsertedFile, (PCSTCH*)tbuf, tphed.stitchCount * sizeof(PCSTCH), &bytesRead, NULL);
					ine = header.stitchCount;
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
					ind = header.stitchCount;
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
					insertCenter.x = (trct.right - trct.left) / 2 + trct.left;
					insertCenter.y = (trct.top - trct.bottom) / 2 + trct.bottom;
					header.stitchCount = ine;
					tsiz.x = trct.right - trct.left;
					tsiz.y = trct.top - trct.bottom;
					ratsr();
					insertSize.x = tsiz.x*horizontalRatio;
					insertSize.y = tsiz.y*horizontalRatio;
					tpnt.x = stitchWindowClientRect.right >> 1;
					tpnt.y = stitchWindowClientRect.bottom >> 1;
					insflin(tpnt);
					newFormVertexCount = 5;
					setMap(SHOFRM);
					setMap(INSFIL);
					dufrm();
				}
			}
			if (hInsertedFile)
				CloseHandle(hInsertedFile);
		}
	}
}

void duinsfil() {

	fPOINT		off;
	unsigned	ind;
	fRECTANGLE*		trct;

	px2stch();
	off.x = selectedPoint.x - insertCenter.x;
	off.y = selectedPoint.y - insertCenter.y;
	for (ind = insertFileFormIndex; ind < formIndex; ind++) {

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
	for (ind = insertFileStitchCount; ind < header.stitchCount; ind++) {

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
		while (ind < header.stitchCount) {

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
		SelectedForm = &formList[ind];
		if (SelectedForm->fillType) {

			SelectedForm->fillColor = activeColor;
			SelectedForm->fillInfo.feather.color = activeColor;
		}
		if (SelectedForm->edgeType)
			SelectedForm->borderColor = activeColor;
		if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
			SelectedForm->underlayColor = activeColor;
	}
	for (ind = 0; ind < header.stitchCount; ind++) {

		if (chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) && (stitchBuffer[ind].attribute&TYPMSK) != TYPMSK) {

			stitchBuffer[ind].attribute &= NCOLMSK;
			stitchBuffer[ind].attribute |= activeColor;
		}
	}
}

BOOL dunum(unsigned cod) {

	if (cod >= '0'&&cod <= '9') {

		numericCode = cod;
		return 1;
	}
	if (cod >= VK_NUMPAD0 && cod <= VK_NUMPAD9) {

		numericCode = cod - '0';
		return 1;
	}
	return 0;
}

void stchrct(fRECTANGLE* rct)
{
	unsigned ind;

	if (header.stitchCount)
	{
		rct->bottom = rct->left = 1e10;
		rct->top = rct->right = 0;
		if (header.stitchCount) {

			for (ind = 0; ind < header.stitchCount; ind++) {

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
	if (header.stitchCount) {

		stchrct(&trct);
		sizx = (trct.right - trct.left) / PFGRAN;
		sizy = (trct.top - trct.bottom) / PFGRAN;
		if ((trct.left < 0) || (trct.bottom < 0) || (trct.right > iniFile.hoopSizeX) || (trct.top > iniFile.hoopSizeY)) {

			strcpy_s(msgbuf, stab[STR_STCHOUT]);
			pmsg = &msgbuf[strlen(msgbuf)];
		}
#if LANG==ENG||LANG==HNG

		sprintf_s(pmsg, sizeof(msgbuf),
			stab[STR_STCHS],
			header.stitchCount,
			sizx, sizx / 25.4,
			sizy, sizy / 25.4);
#endif

#if LANG==GRM

		sprintf_s(pmsg, sizeof(msgbuf),
			stab[STR_STCHS],
			header.stitchCount,
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
		iniFile.hoopSizeX / PFGRAN,
		iniFile.hoopSizeY / PFGRAN);
	pmsg = &pmsg[strlen(pmsg)];
	if (header.stitchCount) {

		sprintf_s(pmsg, sizeof(msgbuf), stab[STR_CREATBY],
			designerName,
			extendedHeader.modifierName);
	}
	shoMsg(msgbuf);
}

void sidhup() {

	RECT		huprct;
	RECT		prfrct;
	unsigned	ind;

	setMap(HUPMSG);
	GetWindowRect(thDat[PHUP], &huprct);
	GetWindowRect(hPreferencesWindow, &prfrct);
	hSideMessageWin = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		prfrct.right + 3 - mainWindowOrigin.x,
		huprct.top - mainWindowOrigin.y,
		buttonWidthX3 + buttonWidth * 2 + 6,
		buttonHeight*HUPS + 6,
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
			buttonHeight*ind + 3,
			buttonWidthX3 + buttonWidth * 2,
			buttonHeight,
			hSideMessageWin,
			NULL,
			hInst,
			NULL);
	}
}

void setpcs() {

	iniFile.auxFileType = AUXPCS;
	auxmen();
}

void setdst() {

	iniFile.auxFileType = AUXDST;
	auxmen();
}

void fop() {

	untrace();
	if (formIndex || header.stitchCount) {

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

	if (pnt.x < clipRectAdjusted.left)
		clipRectAdjusted.left = pnt.x;
	if (pnt.x > clipRectAdjusted.right)
		clipRectAdjusted.right = pnt.x;
	if (pnt.y < clipRectAdjusted.bottom)
		clipRectAdjusted.bottom = pnt.y;
	if (pnt.y > clipRectAdjusted.top)
		clipRectAdjusted.top = pnt.y;
}

void clpadj() {

	double		mid;
	unsigned	ind;

	if (chkMap(GRPSEL)) {

		rngadj();
		ind = groupStartStitch;
		clipRectAdjusted.left = clipboardRect.right = stitchBuffer[ind++].x;
		clipRectAdjusted.top = clipRectAdjusted.bottom = stitchBuffer[ind].y;
		clpradj(stitchBuffer[ind]);
		while (ind < groupEndStitch)
			clpradj(stitchBuffer[ind++]);
		if (stitchBuffer[ind].x < clipRectAdjusted.left)
			clipRectAdjusted.left = stitchBuffer[ind].x;
		if (stitchBuffer[ind].x > clipRectAdjusted.right)
			clipRectAdjusted.right = stitchBuffer[ind].x;
		mid = (clipRectAdjusted.right - clipRectAdjusted.left) / 2 + clipRectAdjusted.left;
		stitchBuffer[groupStartStitch].y = stitchBuffer[groupEndStitch].y = (clipRectAdjusted.top - clipRectAdjusted.bottom) / 2 + clipRectAdjusted.bottom;
		if (stitchBuffer[groupStartStitch].x < mid) {

			stitchBuffer[groupStartStitch].x = clipRectAdjusted.left;
			stitchBuffer[groupEndStitch].x = clipRectAdjusted.right;
		} else {

			stitchBuffer[groupEndStitch].x = clipRectAdjusted.left;
			stitchBuffer[groupStartStitch].x = clipRectAdjusted.right;
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

		CheckMenuItem(hMainMenu, ID_FIL2SEL_ON, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_FIL2SEL_OFF, MF_CHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_FIL2SEL_ON, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_FIL2SEL_OFF, MF_UNCHECKED);
	}

}

void defpref() {

	unsigned ind;

	binaryVariableBitmap = 0;
	for (ind = 0; ind < 16; ind++) {

		userColor[ind] = defUseCol[ind];
		customColor[ind] = defCustCol[ind];
		customBackgroundColor[ind] = defBakCust[ind];
		bitmapBackgroundColors[ind] = defBakBit[ind];
	}
	dazdef();
	underlayColor = 15;
	borderWidth = BRDWID;
	buttonholeFillCornerLength = IBFCLEN;
	iniFile.chainSpace = CHSDEF;
	iniFile.chainRatio = CHRDEF;
	iniFile.fillAngle = DEFANG;
	rstu(SQRFIL);
	stitchSpace = DEFSPACE*PFGRAN;
	showStitchThreshold = SHOPNTS;
	iniFile.gridSize = 12;
	iniFile.hoopType = LARGHUP;
	iniFile.hoopSizeX = LHUPX;
	iniFile.hoopSizeY = LHUPY;
	iniFile.cursorNudgeStep = NUGINI;
	iniFile.nudgePixels = DEFPIX;
	setu(BLUNT);
	smallStitchLength = SMALSIZ*PFGRAN;
	snapLength = SNPLEN*PFGRAN;
	spiralWrap = SPIRWRAP;
	starRatio = STARAT;
	stitchBoxesThreshold = STCHBOX;
	iniFile.maxStitchLength = MAXSIZ*PFGRAN;
	userStitchLength = USESIZ*PFGRAN;
	minStitchLength = MINSIZ*PFGRAN;
	iniFile.clipboardOffset = 0;
	iniFile.fillPhase = 0;
	bitmapColor = 0xc8dfee;
	if (!iniFile.customHoopX)
		iniFile.customHoopX = LHUPX;
	if (!iniFile.customHoopY)
		iniFile.customHoopY = LHUPY;
	picotSpace = IPICSPAC;
	setu(FIL2OF);
	fil2men();
	backgroundColor = 0xa8c4b1;
	unzoomedRect.x = iniFile.hoopSizeX;
	unzoomedRect.y = iniFile.hoopSizeY;
	iniFile.waveEnd = IWAVEND;
	iniFile.wavePoints = IWAVPNTS;
	iniFile.waveLobes = IWAVS;
	iniFile.waveStart = IWAVSTRT;
	iniFile.featherFillType = FDEFTYP;
	iniFile.featherUpCount = FDEFUP;
	iniFile.featherDownCount = FDEFDWN;
	iniFile.featherRatio = (float)FDEFRAT;
	iniFile.featherMinStitchSize = FDEFFLR;
	iniFile.featherCount = FDEFNUM;
	iniFile.underlayStitchLen = DEFULEN;
	iniFile.underlaySpacing = DEFUSPAC;
	iniFile.formBoxSizePixels = DEFBPIX;
	iniFile.textureHeight = ITXHI;
	iniFile.textureWidth = ITXWID;
	iniFile.textureSpacing = (float)ITXSPAC;
}

void dumrk(double pntx, double pnty) {

	if (rstMap(GMRK))
		drwmrk(stitchWindowDC);
	zoomMarkPoint.x = pntx;
	zoomMarkPoint.y = pnty;
	setMap(INIT);
	setMap(GMRK);
	drwmrk(stitchWindowDC);
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
	rang = tang - originalAngle;
	if (fabs(rang) > PI) {

		if (rang > 0)
			rang = 2 * PI - rang;
		else
			rang += 2 * PI;
	}
	return rang;
}

void angdif(double p_ang) {

	if (p_ang > highestAngle)
		highestAngle = p_ang;
	else {

		if (p_ang < lowestAngle)
			lowestAngle = p_ang;
	}
}

void rotmrk() {

	unsigned	ind, cnt, cod;
	double		tang;

	if (chkMap(GMRK) && (chkMap(FORMSEL) || chkMap(GRPSEL))) {

		if (chkMap(FORMSEL)) {

			cod = closestFormToCursor << FRMSHFT;
			fvars(closestFormToCursor);
			lowestAngle = highestAngle = 0;
			originalAngle = atan2(currentFormVertices[0].y - zoomMarkPoint.y, currentFormVertices[0].x - zoomMarkPoint.x);
			for (ind = 1; ind < sides; ind++)
				angdif(nuang(currentFormVertices[ind].y - zoomMarkPoint.y, currentFormVertices[ind].x - zoomMarkPoint.x));
			for (ind = 0; ind < header.stitchCount; ind++) {

				if ((stitchBuffer[ind].attribute&FRMSK) == cod)
					angdif(nuang(stitchBuffer[ind].y - zoomMarkPoint.y, stitchBuffer[ind].x - zoomMarkPoint.x));
			}
		} else {

			rngadj();
			lowestAngle = highestAngle = 0;
			originalAngle = atan2(stitchBuffer[groupStartStitch].y - zoomMarkPoint.y, stitchBuffer[groupStartStitch].x - zoomMarkPoint.x);
			for (ind = groupStartStitch + 1; ind <= groupEndStitch; ind++)
				angdif(nuang(stitchBuffer[ind].y - zoomMarkPoint.y, stitchBuffer[ind].x - zoomMarkPoint.x));
		}
		tang = highestAngle - lowestAngle;
		cnt = 2 * PI / tang;
		iniFile.rotationAngle = 2 * PI / cnt;
		sprintf_s(msgbuf, sizeof(msgbuf), "Rotation Angle: %.2f\n"
			"Segments: %d\n", iniFile.fillAngle * 180 / PI, cnt);
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

	rotationAngle = iniFile.rotationAngle;
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
					SetPixel(stitchWindowDC, cpos.x + ine, cpos.y + ind, tpix);
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
					SetPixel(stitchWindowDC, cpos.x + ine, cpos.y + ind, GetPixel(stitchWindowDC, cpos.x + ine, cpos.y + ind) ^ 0xffffff);
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

			for (ind = 0; ind < header.stitchCount; ind++) {

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
			header.stitchCount = inf;
		} else {

			for (ind = 0; ind < header.stitchCount; ind++) {

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

		SelectedForm->fillSpacing = stitchSpace;
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
			CheckMenuItem(hMainMenu, cods[ind].id, MF_CHECKED);
		else
			CheckMenuItem(hMainMenu, cods[ind].id, MF_UNCHECKED);
	}
	gridPen = nuPen(gridPen, 1, col);
	iniFile.gridColor = col;
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

		CheckMenuItem(hMainMenu, ID_ROTAUXON, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_ROTAUXOFF, MF_UNCHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_ROTAUXON, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_ROTAUXOFF, MF_CHECKED);
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

		CheckMenuItem(hMainMenu, ID_FRMX, MF_CHECKED);
		CheckMenuItem(hMainMenu, ID_FRMBOX, MF_UNCHECKED);
	} else {

		CheckMenuItem(hMainMenu, ID_FRMX, MF_UNCHECKED);
		CheckMenuItem(hMainMenu, ID_FRMBOX, MF_CHECKED);
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
		fld		iniFile.gridSize
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
		pnt->x = rintf(pnt->x / iniFile.gridSize) * iniFile.gridSize;
		pnt->y = rintf(pnt->y / iniFile.gridSize) * iniFile.gridSize;

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
		fld		iniFile.gridSize
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
		strt->x = rintf(strt->x / iniFile.gridSize) * iniFile.gridSize;
		strt->y = rintf(strt->y / iniFile.gridSize) * iniFile.gridSize;

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
	strcpy_s(fileName, "u:\\mrd\\t.pes");
	nuFil();
}

void savpes() {

	iniFile.auxFileType = AUXPES;
	strcpy_s(fileName, "u:\\mrd\\t1.thr");
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
	for (ind = 0; ind < activePointIndex; ind++) {

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
		strcpy_s(snam, defaultDirectory);
		strcat_s(snam, "\\*.thr");
		srch = FindFirstFile(snam, &pdat[0]);
		if (srch == INVALID_HANDLE_VALUE) {

			sprintf_s(msgbuf, sizeof(msgbuf), "Directory: %s has no .thr files\n", defaultDirectory);
			shoMsg(msgbuf);
			EndDialog(hwndlg, wparam);
			return TRUE;
		}
		activePointIndex = 1;
		while (FindNextFile(srch, &pdat[activePointIndex++]));
		activePointIndex--;
		ritlock(hwndlg);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {

		case IDCANCEL:

			EndDialog(hwndlg, wparam);
			return TRUE;

		case IDC_LOCKAL:

			for (ind = 0; ind < activePointIndex; ind++)
				pdat[ind].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
			ritlock(hwndlg);
			break;

		case IDC_UNLOCKAL:

			for (ind = 0; ind < activePointIndex; ind++)
				pdat[ind].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
			ritlock(hwndlg);
			break;

		case IDC_LOCK:

			ine = 0;
			hunlok = GetDlgItem(hwndlg, IDC_UNLOCKED);
			for (ind = 0; ind < activePointIndex; ind++) {

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
			for (ind = 0; ind < activePointIndex; ind++) {

				if ((pdat[ind].dwFileAttributes&FILE_ATTRIBUTE_READONLY)) {

					if (SendMessage(hlok, LB_GETSEL, ine, 0))
						pdat[ind].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
					ine++;
				}
			}
			ritlock(hwndlg);
			break;

		case IDOK:

			strcpy_s(snam, defaultDirectory);
			strcat_s(snam, "\\");
			ine = 0;
			for (ind = 0; ind < activePointIndex; ind++) {

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
		ReleaseDC(hWnd, hTraceDC);
		rstMap(WASEDG);
		for (ind = 0; ind < 16; ind++) {

			shownd(hDefaultColorWin[ind]);
			shownd(hUserColorWin[ind]);
			shownd(hThreadSizeWin[ind]);
		}
		for (ind = 0; ind < 3; ind++) {

			hidwnd(hTraceControl[ind]);
			hidwnd(hTraceSelect[ind]);
			hidwnd(hTraceUp[ind]);
			hidwnd(hTraceDown[ind]);
		}
		for (ind = 0; ind < 9; ind++)
			shownd(hButtonWin[ind]);
		hidwnd(hTraceStepWin);
	} else {

		if (chkMap(TRCUP))
			downColor = 0xffffff;
		else
			upColor = 0;
	}
}

void trcstpnum() {

	sprintf_s(msgbuf, sizeof(msgbuf), "len: %.2f", iniFile.traceLength / PFGRAN);
	SetWindowText(hTraceStepWin, msgbuf);
}

void trcratnum() {

	sprintf_s(msgbuf, sizeof(msgbuf), "pnts: %.2f", -log10(iniFile.traceRatio - 1));
	butxt(HLIN, msgbuf);
}

void clrhbut(unsigned strt)
{
	unsigned ind;

	for (ind = strt; ind < 9; ind++)
		SetWindowText(hButtonWin[ind], "");
}

void tracwnd() {

	unsigned ind;

	for (ind = 0; ind < 16; ind++) {

		hidwnd(hDefaultColorWin[ind]);
		hidwnd(hUserColorWin[ind]);
		hidwnd(hThreadSizeWin[ind]);
	}
	for (ind = 0; ind < 3; ind++) {

		shownd(hTraceControl[ind]);
		shownd(hTraceSelect[ind]);
		shownd(hTraceUp[ind]);
		shownd(hTraceDown[ind]);
	}
	hidwnd(hButtonWin[HBOXSEL]);
	hidwnd(hButtonWin[HUPTO]);
	shownd(hTraceStepWin);
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
	hTraceDC = CreateCompatibleDC(stitchWindowDC);
	if (hTraceBitmap && hTraceDC) {
		SelectObject(hTraceDC, hTraceBitmap);
		BitBlt(hTraceDC, 0, 0, bitmapWidth, bitmapHeight, bitmapDC, 0, 0, SRCCOPY);
		setMap(WASTRAC);
		tracedPixels = (unsigned*)oseq;
		traceDataSize = ((bitmapWidth*bitmapHeight) >> 5) + 1;
		for (ind = 0; ind < traceDataSize; ind++)
			tracedPixels[ind] = 0;
		StretchBlt(stitchWindowMemDC, bitmapDstRect.left, bitmapDstRect.top, bitmapDstRect.right - bitmapDstRect.left, bitmapDstRect.bottom - bitmapDstRect.top,
			bitmapDC, bitmapSrcRect.left, bitmapSrcRect.top, bitmapSrcRect.right - bitmapSrcRect.left, bitmapSrcRect.bottom - bitmapSrcRect.top, SRCCOPY);
	}
}

void trace() {

	unsigned			ind;

#if TRCMTH==0

	unsigned			usum, psum, dsum;
#endif

	if (*pcsBMPFileName) {

		untrace();
		tracwnd();
		getrmap();
		if (px2stch() && !rstMap(WASTRCOL)) {

			if (chkMap(LANDSCAP))
				selectedPoint.y -= (unzoomedRect.y - bitmapSizeinStitches.y);
			bitpnt.x = bmpStitchRatio.x*selectedPoint.x;
			bitpnt.y = bmpStitchRatio.y*selectedPoint.y - 1;
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
	if (tracedPoints[activePointIndex - 1].x != currentTracePoint.x || tracedPoints[activePointIndex - 1].y != currentTracePoint.y) {

		tracedPoints[activePointIndex].x = currentTracePoint.x;
		tracedPoints[activePointIndex++].y = currentTracePoint.y;
		if (activePointIndex >= 500000)
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
			selectedPoint.y -= (unzoomedRect.y - bitmapSizeinStitches.y);
		currentTracePoint.x = bmpStitchRatio.x*selectedPoint.x;
		currentTracePoint.y = bmpStitchRatio.y*selectedPoint.y;
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
		activePointIndex = 1;
		tracedPoints = (TRCPNT*)bseq;
		tracedPoints[0].x = currentTracePoint.x;
		tracedPoints[0].y = currentTracePoint.y;
		while (trcbit());
		if (activePointIndex >= 500000) {

			tabmsg(IDS_FRM2L);
			return;
		}
		decimatedLine = &tracedPoints[activePointIndex];
		decimatedLine[0].x = tracedPoints[0].x;
		decimatedLine[0].y = tracedPoints[0].y;
		dutdif(&tracedPoints[0]);
		outputIndex = 1;
		for (ind = 1; ind < activePointIndex; ind++) {

			traceDiff1.x = traceDiff0.x;
			traceDiff1.y = traceDiff0.y;
			dutdif(&tracedPoints[ind]);
			if (traceDiff1.x != traceDiff0.x || traceDiff1.y != traceDiff0.y) {

				decimatedLine[outputIndex].x = tracedPoints[ind].x;
				decimatedLine[outputIndex++].y = tracedPoints[ind].y;
			}
		}
		tracedPoints[0].x = decimatedLine[0].x;
		tracedPoints[0].y = decimatedLine[0].y;
		ine = 0;
		activePointIndex = 0;
		for (ind = 1; ind < outputIndex; ind++) {

			tlen = hypot(decimatedLine[ind].x - decimatedLine[ine].x, decimatedLine[ind].y - decimatedLine[ine].y);
			if (tlen > iniFile.traceLength) {

				tracedPoints[activePointIndex].x = decimatedLine[ine].x;
				tracedPoints[activePointIndex].y = decimatedLine[ine].y;
				ine = ind;
				activePointIndex++;
			}
		}
		for (ind = ine + 1; ind < outputIndex; ind++) {

			tracedPoints[activePointIndex].x = decimatedLine[ind].x;
			tracedPoints[activePointIndex].y = decimatedLine[ind].y;
			activePointIndex++;
		}
		SelectedForm = &formList[formIndex];
		frmclr(SelectedForm);
		currentFormVertices = &formPoints[fltad];
		currentFormVertices[0].x = tracedPoints[0].x*stitchBmpRatio.x;
		currentFormVertices[0].y = tracedPoints[0].y*stitchBmpRatio.y;
		ine = 0;
		outputIndex = 0;
		tlens = 0;
		if (chkMap(LANDSCAP))
			ratof = unzoomedRect.y - bitmapSizeinStitches.y;
		else
			ratof = 0;
		for (ind = 1; ind < activePointIndex; ind++) {

			tlens += hypot(tracedPoints[ind].x - tracedPoints[ind - 1].x, tracedPoints[ind].y - tracedPoints[ind - 1].y);
			tlen = hypot(tracedPoints[ind].x - tracedPoints[ine].x, tracedPoints[ind].y - tracedPoints[ine].y);
			if (tlens > tlen*iniFile.traceRatio) {

				currentFormVertices[outputIndex].x = tracedPoints[ind - 1].x*stitchBmpRatio.x;
				currentFormVertices[outputIndex].y = tracedPoints[ind - 1].y*stitchBmpRatio.y + ratof;
				outputIndex++;
				ind--;
				ine = ind;
				tlens = 0;
			}
		}
		if (fltad + outputIndex > MAXFLT) {

			tabmsg(IDS_FRMOVR);
			return;
		}
		SelectedForm->vertices = adflt(outputIndex);
		SelectedForm->sides = outputIndex;
		SelectedForm->type = POLI;
		SelectedForm->attribute = activeLayer << 1;
		frmout(formIndex);
		formList[formIndex].satinGuideCount = 0;
		formIndex++;
		setMap(RESTCH);
		setMap(FRMOF);
		tglfrm();
	}
}

unsigned ducolm() {

	if (traceMsgPoint.x < (int)buttonWidth)
		return 0;
	if (traceMsgPoint.x < (int)buttonWidth << 1)
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

	hTraceNumberInput = CreateWindow(
		"STATIC",
		0,
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidthX3,
		pos,
		buttonWidth,
		buttonHeight,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void trnumwnd1(int pos) {

	hGeneralNumberInputBox = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidthX3,
		pos,
		buttonWidthX3,
		buttonHeight,
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
		dutrnum0(atoi(traceInputBuffer));
	if (chkMap(TRNIN1))
		dutrnum1();
	traceMsgPoint.x = msg.pt.x - mainWindowOrigin.x;
	traceMsgPoint.y = msg.pt.y - mainWindowOrigin.y;
	if (traceMsgPoint.x > (int)buttonWidthX3)
		dutrac();
	else {

		columnColor = ducolm();
		if (traceMsgPoint.y < (int)buttonHeight * 15) {

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				upColor &= traceRGBMask[columnColor];
				downColor |= traceRGB[2 - columnColor];
				goto tracpar1;
			}
			rat = (float)traceMsgPoint.y / (buttonHeight * 15);
			pos = rat * 255;
			tcol = upColor&traceRGB[2 - columnColor];
			tpos = pos << traceShift[columnColor];
			if (tpos < tcol) {

				upColor &= traceRGBMask[columnColor];
				upColor |= tpos;
				goto tracpar1;
			}
			tcol = downColor&traceRGB[2 - columnColor];
			if (tpos > tcol) {

				downColor &= traceRGBMask[columnColor];
				downColor |= tpos;
				goto tracpar1;
			}
			if (msg.message == WM_LBUTTONDOWN) {

				upColor &= traceRGBMask[columnColor];
				upColor |= pos << traceShift[columnColor];
			} else {

				downColor &= traceRGBMask[columnColor];
				downColor |= pos << traceShift[columnColor];
			}
		tracpar1:
			redraw(hTraceControl[columnColor]);
			trace();
		} else {

			pos = floor(traceMsgPoint.y / buttonHeight);
			if (pos < 16) {

				toglMap(traceRGBFlag[columnColor]);
				redraw(hTraceSelect[columnColor]);
				trace();
			} else {

				if (pos < 18) {

					setMap(NUMIN);
					setMap(TRNIN0);
					msgIndex = 0;
					*traceInputBuffer = 0;
					if (pos < 17) {

						trnumwnd0(buttonHeight * 16);
						setMap(TRNUP);
					} else {

						trnumwnd0(buttonHeight * 17);
						rstMap(TRNUP);
					}
				} else {

					if (pos < 20) {

						setMap(NUMIN);
						setMap(TRNIN1);
						msgIndex = 0;
						*traceInputBuffer = 0;
						if (pos < 19) {

							trnumwnd1(buttonHeight * 18);
							setMap(TRNUP);
						} else {

							trnumwnd1(buttonHeight * 19);
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

	if (!*pcsBMPFileName) {

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

				difbits(traceShift[ind], &traceBitmap[l_bpnt]);
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
				traceBitmap[ine] |= inf << traceShift[ind];
			}
		}
	}
	BitBlt(bitmapDC, 0, 0, bitmapWidth, bitmapHeight, hTraceDC, 0, 0, SRCCOPY);
	setMap(WASDIF);
	setMap(RESTCH);
	tracwnd();
}

void delstch() {

	savdo();
	header.stitchCount = 0;
	clpad = 0;
	txad = 0;
	rstAll();
	clrfills();
	colorChanges = 0;
	butxt(HNUM, "");
	butxt(HTOT, "");
	setMap(RESTCH);
}

void chkbit() {

	if (*pcsBMPFileName && (chkMap(WASDIF) || chkMap(WASDSEL) || chkMap(WASBLAK))) {

		setMap(WASESC);
		bfil();
	}
}

void trcnum(unsigned shft, COLORREF col, unsigned ind) {

	unsigned	len;
	unsigned	nwid;
	TCHAR		trbuf[4];

	col >>= shft;
	col &= 0xff;
	_itoa_s(col, trbuf, 10);
	len = strlen(trbuf);
	nwid = numeralWidth*(3 - len) + 1;
	SetBkColor(ds->hDC, traceRGB[ind]);
	TextOut(ds->hDC, nwid, 1, trbuf, strlen(trbuf));
}

void upnum(unsigned ind) {

	trcnum(traceShift[ind], invertUpColor, ind);
}

void dwnum(unsigned ind) {

	trcnum(traceShift[ind], invertDownColor, ind);
}

void ritrcol(COLORREF* tref, unsigned num) {

	*tref &= traceRGBMask[columnColor];
	num &= 0xff;
	*tref |= (num << traceShift[columnColor]);
}

void dutrnum0(unsigned num) {

	rstMap(NUMIN);
	rstMap(TRNIN0);
	if (chkMap(TRNUP)) {

		ritrcol(&invertUpColor, num);
		upColor = invertUpColor ^ 0xffffff;
		redraw(hTraceUp[columnColor]);
	} else {

		ritrcol(&invertDownColor, num);
		downColor = invertDownColor ^ 0xffffff;
		redraw(hTraceDown[columnColor]);
	}
	redraw(hTraceControl[columnColor]);
	DestroyWindow(hTraceNumberInput);
	setMap(WASTRCOL);
	trace();
}

void dutrnum1() {

	double	tdub;

	DestroyWindow(hGeneralNumberInputBox);
	rstMap(NUMIN);
	rstMap(TRNIN1);
	tdub = atof(msgbuf);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", tdub);
	if (tdub > 9)
		tdub = 9;
	if (chkMap(TRNUP)) {

		iniFile.traceLength = tdub*PFGRAN;
		trcstpnum();
	} else {

		iniFile.traceRatio = 1 + pow(0.1, tdub);
		trcratnum();
	}
}

void trcsel() {

	unsigned ind, max, xind;

	if (*pcsBMPFileName) {

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
		BitBlt(bitmapDC, 0, 0, bitmapWidth, bitmapHeight, hTraceDC, 0, 0, SRCCOPY);
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

	if (*pcsBMPFileName) {

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
				invertDownColor |= col[ind] << traceShift[ind];
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
	Polyline(hTraceDC, lin, 2);
}

void bfrm() {

	unsigned ind;

	if (sides) {

		for (ind = 0; ind < (unsigned)sides - 1; ind++)
			pxlin(ind, ind + 1);
		if (SelectedForm->type != LIN)
			pxlin(ind, 0);
	}
}

void blak() {

	unsigned ind;

	if (!*pcsBMPFileName) {

		tabmsg(IDS_MAPLOD);
		return;
	}
	tracwnd();
	if (formIndex) {

		blackPen = CreatePen(PS_SOLID, 1, 0);
		SelectObject(bitmapDC, blackPen);
		SelectObject(hTraceDC, blackPen);
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

	*pcsBMPFileName = 0;
	setMap(RESTCH);
}

void closfn() {

	deltot();
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], iniFile.designerName);
	knotCount = 0;
	*fileName = 0;
	*pcsBMPFileName = 0;
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

	for (ine = 0; ine < SelectedForm->sides; ine++) {

		SelectedForm->vertices[ine].x += dx;
		SelectedForm->vertices[ine].y += dy;
	}
	SelectedForm->rectangle.bottom += dy;
	SelectedForm->rectangle.top += dy;
	SelectedForm->rectangle.left += dx;
	SelectedForm->rectangle.right += dx;
}

void nudgfn(float dx, float dy) {

	unsigned	ind, rcnt;
	POINT		pix;

	if (chkMap(BIGBOX) || selectedFormCount || chkMap(FORMSEL) || chkMap(GRPSEL) || chkMap(SELBOX))
		savdo();
	if (chkMap(BIGBOX)) {

		for (ind = 0; ind < formIndex; ind++) {

			SelectedForm = &formList[ind];
			frmpos(dx, dy);
		}
		for (ind = 0; ind < header.stitchCount; ind++) {

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
		for (ind = 0; ind < header.stitchCount; ind++) {

			if (chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {

				stitchBuffer[ind].x += dx;
				stitchBuffer[ind].y += dy;
			}
		}
		for (ind = 0; ind < selectedFormCount; ind++) {

			SelectedForm = &formList[selectedFormList[ind]];
			frmpos(dx, dy);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL)) {

		SelectedForm = &formList[closestFormToCursor];
		frmpos(dx, dy);
		if (SelectedForm->fillType || SelectedForm->edgeType) {

			for (ind = 0; ind < header.stitchCount; ind++) {

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
			pix.x = iniFile.nudgePixels;
		else
			pix.x = -iniFile.nudgePixels;
	}
	if (dy) {

		if (dy > 0)
			pix.y = -iniFile.nudgePixels;
		else
			pix.y = +iniFile.nudgePixels;
	}
	mouse_event(MOUSEEVENTF_MOVE, pix.x, pix.y, 0, 0);
}

void pixmsg(unsigned cod, unsigned pix) {

	sprintf_s(msgbuf, sizeof(msgbuf), stab[cod], pix);
	shoMsg(msgbuf);
}

void getnpix() {

	pixmsg(STR_NUDG, iniFile.nudgePixels);
	setMap(NUMIN);
	setMap(PIXIN);
	numWnd();
}

void getstpix() {

	pixmsg(STR_STCHP, iniFile.stitchSizePixels);
	setMap(NUMIN);
	setMap(STPXIN);
	numWnd();
}

void getfrmpix() {

	pixmsg(STR_FRMP, iniFile.formPointSizePixels);
	setMap(NUMIN);
	setMap(FRMPXIN);
	numWnd();
}

void getfrmbox() {

	pixmsg(STR_FRMBOX, iniFile.formBoxSizePixels);
	setMap(NUMIN);
	setMap(FRMBOXIN);
	numWnd();
}

void bakmrk() {

	fPOINT	tpnt;

	if (chkMap(WASMRK)) {

		if (zoomMarkPoint.x > iniFile.hoopSizeX)
			zoomMarkPoint.x = iniFile.hoopSizeY / 2;
		if (zoomMarkPoint.y > iniFile.hoopSizeY)
			zoomMarkPoint.y = iniFile.hoopSizeY / 2;
		dumrk(zoomMarkPoint.x, zoomMarkPoint.y);
		tpnt.x = zoomMarkPoint.x;
		tpnt.y = zoomMarkPoint.y;
		shft(tpnt);
		setMap(RESTCH);
	} else
		tabmsg(IDS_MRK);
}

void nuscol(unsigned ind) {

	userPen[ind] = nuPen(userPen[ind], 1, userColor[ind]);
	hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], userColor[ind]);
	redraw(hUserColorWin[ind]);
}

void movchk() {

	unsigned	ind, col, key, swtch;
	FRMHED*		pfrm;

	if (msg.wParam&MK_LBUTTON) {

		if (!setMap(WASMOV)) {

			if (chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15])) {

				draggedColor = verticalIndex & 0xf;
				setMap(WASCOL);
			}
		}
	} else {

		if (rstMap(WASMOV) && rstMap(WASCOL)) {

			if (chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15])) {

				key = GetKeyState(VK_SHIFT) & 0X8000;
				swtch = GetKeyState(VK_CONTROL) & 0X8000;
				for (ind = 0; ind < header.stitchCount; ind++) {

					col = stitchBuffer[ind].attribute&COLMSK;
					if (col == verticalIndex) {

						stitchBuffer[ind].attribute &= NCOLMSK;
						stitchBuffer[ind].attribute |= draggedColor;
					} else {

						if (!key&&col == draggedColor) {

							stitchBuffer[ind].attribute &= NCOLMSK;
							stitchBuffer[ind].attribute |= verticalIndex;
						}
					}
				}
				for (ind = 0; ind < formIndex; ind++) {

					pfrm = &formList[ind];
					if (pfrm->fillType) {

						if (pfrm->fillColor == verticalIndex)
							pfrm->fillColor = draggedColor;
						else {

							if (!key&&pfrm->fillColor == draggedColor)
								pfrm->fillColor = verticalIndex;
						}
						if (pfrm->fillInfo.feather.color == verticalIndex)
							pfrm->fillInfo.feather.color = draggedColor;
						else {

							if (!key&&pfrm->fillInfo.feather.color == draggedColor)
								pfrm->fillInfo.feather.color = verticalIndex;
						}
					}
					if (pfrm->edgeType) {

						if (pfrm->borderColor == verticalIndex)
							pfrm->borderColor = draggedColor;
						else {

							if (!key&&pfrm->borderColor == draggedColor)
								pfrm->borderColor = verticalIndex;
						}
					}
				}
				if (!swtch) {

					ind = userColor[verticalIndex];
					userColor[verticalIndex] = userColor[draggedColor];
					if (!key) {

						userColor[draggedColor] = ind;
						nuscol(draggedColor);
					}
					nuscol(verticalIndex);
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
	if (chkMsgs(msg.pt, hDefaultColorWin[0], hUserColorWin[15])) {

		verticalIndex &= COLMSK;
		for (ind = 0; ind < header.stitchCount; ind++)
			setr(stitchBuffer[ind].attribute&COLMSK);
		if (*markedStitchMap == COLSMSK)
			tabmsg(IDS_COLAL);
		else {

			dcol = 15;
			while (chkr(dcol))
				dcol--;
			for (ind = 0; ind < header.stitchCount; ind++) {

				col = stitchBuffer[ind].attribute&COLMSK;
				if (col >= verticalIndex&&col < dcol) {

					stitchBuffer[ind].attribute &= NCOLMSK;
					stitchBuffer[ind].attribute |= col + 1;
				}
			}
			for (ind = 0; ind < formIndex; ind++) {

				tfrm = &formList[ind];
				if (tfrm->fillType) {

					if (tfrm->fillColor >= verticalIndex&&tfrm->fillColor < dcol)
						tfrm->fillColor++;
					if (tfrm->fillInfo.feather.color >= verticalIndex&&tfrm->fillInfo.feather.color < dcol)
						tfrm->fillInfo.feather.color++;
				}
				if (tfrm->edgeType) {

					if (tfrm->borderColor >= verticalIndex&&tfrm->borderColor < dcol)
						tfrm->borderColor++;
				}
			}
			for (ind = dcol; ind > verticalIndex; ind--) {

				userColor[ind] = userColor[ind - 1];
				nuscol(ind);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

BOOL usedcol() {

	unsigned ind;

	for (ind = 0; ind < header.stitchCount; ind++) {

		if ((stitchBuffer[ind].attribute&COLMSK) == verticalIndex)
			return 1;
	}
	return 0;
}

void delcol() {

	unsigned	ind, col;
	FRMHED*		tfrm;

	if (chkMsgs(msg.pt, hDefaultColorWin[0], hUserColorWin[15])) {

		verticalIndex &= 0xf;
		if (usedcol())
			tabmsg(IDS_COLU);
		else {

			for (ind = 0; ind < header.stitchCount; ind++) {

				col = stitchBuffer[ind].attribute&COLMSK;
				if (col > verticalIndex&&col) {

					stitchBuffer[ind].attribute &= NCOLMSK;
					stitchBuffer[ind].attribute |= col - 1;
				}
			}
			for (ind = 0; ind < formIndex; ind++) {

				tfrm = &formList[ind];
				if (tfrm->fillType) {

					if (tfrm->fillColor > verticalIndex)
						tfrm->fillColor--;
					if (tfrm->fillInfo.feather.color > verticalIndex)
						tfrm->fillInfo.feather.color--;
				}
				if (tfrm->edgeType) {

					if (tfrm->borderColor > verticalIndex)
						tfrm->borderColor--;
				}
			}
			for (ind = verticalIndex; ind < 15; ind++) {

				userColor[ind] = userColor[ind + 1];
				nuscol(ind);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void set1knot() {

	unsigned src, dst;

	if (header.stitchCount&&chkMap(SELBOX)) {

		savdo();
		if (closestPointIndex == (unsigned)header.stitchCount - 1) {

			setMap(FILDIR);
			outputIndex = closestPointIndex + 1;
			endknt(closestPointIndex);
		} else {

			src = header.stitchCount - 1;
			dst = header.stitchCount + 4;
			while (src&&src >= closestPointIndex)
				mvstch(dst--, src--);
			outputIndex = closestPointIndex + 1;
			strtknt(closestPointIndex);
		}
		header.stitchCount += 5;
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
	formPointsAsLine[0].x = tpnt.x;
	formPointsAsLine[0].y = tpnt.y;
	sfCor2px(iseq[1], &tpnt);
	tof.x = msg.pt.x - stitchWindowOrigin.x - tpnt.x;
	tof.y = msg.pt.y - stitchWindowOrigin.y - tpnt.y;
	for (ind = 0; ind < outputIndex - 2; ind++) {

		ine = ind + 1;
		sfCor2px(iseq[ine], &tpnt);
		formPointsAsLine[ine].x = tpnt.x + tof.x;
		formPointsAsLine[ine].y = tpnt.y + tof.y;
	}
	ind++;
	sfCor2px(iseq[ind], &tpnt);
	formPointsAsLine[ind].x = tpnt.x;
	formPointsAsLine[ind].y = tpnt.y;
}

void dupclp() {

	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	Polyline(stitchWindowDC, formPointsAsLine, outputIndex);
	SetROP2(stitchWindowDC, R2_COPYPEN);
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
	pof.x = selectedPoint.x - iseq[1].x;
	pof.y = selectedPoint.y - iseq[1].y;
	ine = nxt(closestVertexToCursor);
	cnt = outputIndex - 2;
	fltspac(&currentFormVertices[ine], cnt);
	for (ind = 1; ind < outputIndex - 1; ind++) {

		currentFormVertices[ine].x = iseq[ind].x + pof.x;
		currentFormVertices[ine].y = iseq[ind].y + pof.y;
		ine++;
	}
	SelectedForm->sides += cnt;
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
	fPOINT*			l_clipData;
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
				ritfcor(&selectedPoint);
			if (*flagMap&(PRFACTB | FORMINB | POLIMOVB) || hFormData) {

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
				stretchBoxLine[0].x = stretchBoxLine[3].x = stretchBoxLine[4].x = msg.pt.x - fmovdif.x - stitchWindowOrigin.x;
				stretchBoxLine[1].x = stretchBoxLine[2].x = msg.pt.x + selectedFormsSize.x - fmovdif.x - stitchWindowOrigin.x;
				stretchBoxLine[0].y = stretchBoxLine[1].y = stretchBoxLine[4].y = msg.pt.y - fmovdif.y - stitchWindowOrigin.y;
				stretchBoxLine[2].y = stretchBoxLine[3].y = msg.pt.y + selectedFormsSize.y - fmovdif.y - stitchWindowOrigin.y;
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
				tdub = fabs((double)(nusiz.x - stretchBoxLine[ind].x) / (nusiz.y - stretchBoxLine[ind].y));
				if (ind & 1) {

					if (tdub < xyRatio)
						nusiz.x = (stretchBoxLine[ind].y - nusiz.y)*xyRatio + stretchBoxLine[ind].x;
					else
						nusiz.y = (stretchBoxLine[ind].x - nusiz.x) / xyRatio + stretchBoxLine[ind].y;
					ind = nxtcrnr(ind);
					stretchBoxLine[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					stretchBoxLine[ind].x = nusiz.x;
					stretchBoxLine[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					stretchBoxLine[ind].x = nusiz.x;
				} else {

					if (tdub < xyRatio)
						nusiz.x = (nusiz.y - stretchBoxLine[ind].y)*xyRatio + stretchBoxLine[ind].x;
					else
						nusiz.y = (nusiz.x - stretchBoxLine[ind].x) / xyRatio + stretchBoxLine[ind].y;
					ind = nxtcrnr(ind);
					stretchBoxLine[ind].x = nusiz.x;
					ind = nxtcrnr(ind);
					stretchBoxLine[ind].x = nusiz.x;
					stretchBoxLine[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					stretchBoxLine[ind].y = nusiz.y;
				}
				stretchBoxLine[4].x = stretchBoxLine[0].x;
				stretchBoxLine[4].y = stretchBoxLine[0].y;
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
							stretchBoxLine[ind].x = tlng;
						else
							stretchBoxLine[ind].y = tlng;
					}
				}
				stretchBoxLine[4].x = stretchBoxLine[0].x;
				stretchBoxLine[4].y = stretchBoxLine[0].y;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			}
			if (chkMap(INSFRM)) {

				uninsf();
				insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
				insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
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
					ritfcor(&selectedPoint);
				return 1;
			}
			if (chkMap(MOVCNTR)) {

				unrot();
				px2stch();
				rotationCenter.x = selectedPoint.x;
				rotationCenter.y = selectedPoint.y;
				ritrot();
				return 1;
			}
			if (chkMap(ROTCAPT)) {

				unrotu();
				unrot();
				rotateBoxToCursorLine[1].x = msg.pt.x - stitchWindowOrigin.x;
				rotateBoxToCursorLine[1].y = msg.pt.y - stitchWindowOrigin.y;
				padj.x = rotateBoxToCursorLine[0].x - rotateBoxToCursorLine[1].x;
				padj.y = rotateBoxToCursorLine[0].y - rotateBoxToCursorLine[1].y;
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
					ritfcor(&selectedPoint);
				unlin();
				moveLine0[1].x = moveLine1[0].x = msg.pt.x - stitchWindowOrigin.x;
				moveLine0[1].y = moveLine1[0].y = msg.pt.y - stitchWindowOrigin.y;
				dulin();
				return 1;
			}
			if (chkMap(INSRT)) {

				if (px2stch())
					ritfcor(&selectedPoint);
				if (setMap(VCAPT))
					SetCapture(hWnd);
				if (chkMap(LIN1)) {

					if (header.stitchCount) {

						xlin1();
						insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
						insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
						setMap(ILIN1);
						ilin1();
					}
					return 1;
				}
				xlin();
				insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
				insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
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
				zoomBoxLine[1].x = zoomBoxLine[2].x = msg.pt.x - stitchWindowOrigin.x;
				zoomBoxLine[2].y = zoomBoxLine[3].y = msg.pt.y - stitchWindowOrigin.y;
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
					for (ind = 0; ind < header.stitchCount; ind++) {

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
			padj.x = (stitchRangeRect.left + selectBoxOffset.x) - selectedPoint.x;
			padj.y = (stitchRangeRect.bottom + selectBoxOffset.y) - selectedPoint.y;
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
			stitchBuffer[closestPointIndex].x = selectedPoint.x;
			stitchBuffer[closestPointIndex].y = selectedPoint.y;
			stitchBuffer[closestPointIndex].attribute |= USMSK;
			if (zoomFactor < STCHBOX) {

				SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
				SelectObject(stitchWindowMemDC, linePen);
				stchbox(closestPointIndex - 1, stitchWindowDC);
				stchbox(closestPointIndex, stitchWindowDC);
				stchbox(closestPointIndex + 1, stitchWindowDC);
				SetROP2(stitchWindowMemDC, R2_COPYPEN);
			}
			setMap(SELBOX);
			rstMap(FRMPSEL);
			setMap(RESTCH);
			return 1;
		}
		if (chkMap(BZUMIN)) {

			px2stch();
			if (rstMap(BOXSLCT)) {

				if (zoomBoxOrigin.x > selectedPoint.x) {

					stitchRangeRect.right = zoomBoxOrigin.x;
					stitchRangeRect.left = selectedPoint.x;
				} else {

					stitchRangeRect.right = selectedPoint.x;
					stitchRangeRect.left = zoomBoxOrigin.x;
				}
				if (zoomBoxOrigin.y > selectedPoint.y) {

					stitchRangeRect.top = zoomBoxOrigin.y;
					stitchRangeRect.bottom = selectedPoint.y;
				} else {

					stitchRangeRect.top = selectedPoint.y;
					stitchRangeRect.bottom = zoomBoxOrigin.y;
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

				if (selectedPoint.x < zoomBoxOrigin.x) {

					tdub = zoomBoxOrigin.x;
					zoomBoxOrigin.x = selectedPoint.x;
					selectedPoint.x = tdub;
				}
				if (selectedPoint.y < zoomBoxOrigin.y) {

					tdub = zoomBoxOrigin.y;
					zoomBoxOrigin.y = selectedPoint.y;
					selectedPoint.y = tdub;
				}
				nusiz.x = selectedPoint.x - zoomBoxOrigin.x;
				nusiz.y = selectedPoint.y - zoomBoxOrigin.y;
				selectedPoint.x = zoomBoxOrigin.x + nusiz.x / 2;
				selectedPoint.y = zoomBoxOrigin.y + nusiz.y / 2;
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
				shft(selectedPoint);
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
			if (chkwnd(hDiscardButton)) {

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

				if (msg.hwnd == hBackupViewer[ind]) {

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
					groupStitchIndex = tdub*header.stitchCount;
					setMap(GRPSEL);
					grpAdj();
					nuAct(groupStitchIndex);
					setMap(RESTCH);
				} else {

					closestPointIndex = tdub*header.stitchCount;
					nuAct(closestPointIndex);
					movbox();
					if (rstMap(GRPSEL)) {

						rstMap(SCROS);
						rstMap(ECROS);
						setMap(RESTCH);
					}
				}
			} else {

				closestPointIndex = tdub*header.stitchCount;
				nuAct(closestPointIndex);
				rstAll();
				setMap(SELBOX);
				selCol();
			}
			redraw(hColorBar);
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
				dutrnum0(atoi(traceInputBuffer));
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

					DestroyWindow(hPreferencesWindow);
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
		if (px2stch() && !(chkMap(SIZSEL) && chkMsgs(msg.pt, hChangeThreadSizeWin[0], hChangeThreadSizeWin[2]))) {

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
					if (hFormData) {

						undat();
						unsid();
						formMenuChoice = 0;
						setMap(RESTCH);
						goto frmskip;
					}
					ritfrct(closestFormToCursor, stitchWindowDC);
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
					ritfrct(closestFormToCursor, stitchWindowDC);
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
			if (chkMap(INIT) || hFile) {

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
						redraw(hColorBar);
						return 1;
					}
					if (chkMap(GRPSEL)) {

						cod = closestPointIndex;
						closPnt1(&closestPointIndex);
						groupStitchIndex = closestPointIndex;
						closestPointIndex = cod;
						grpAdj();
						nuAct(groupStitchIndex);
						redraw(hColorBar);
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
						if (header.stitchCount)
							rebox();
					}
				}
				clrstch();
				setMap(RESTCH);
			}
			return 1;
		}
		if (msg.pt.x >= minLenRect.left&&msg.pt.x <= minLenRect.right
			&&msg.pt.y > minLenRect.top&&msg.pt.y <= minLenRect.bottom) {

			srchk();
			setsrch(smallestStitchIndex);
			lensadj();
			setMap(GRPSEL);
			setMap(RESTCH);
			return 1;
		}
		if (msg.pt.x >= maxLenRect.left&&msg.pt.x <= maxLenRect.right
			&&msg.pt.y > maxLenRect.top&&msg.pt.y <= maxLenRect.bottom) {

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
			if (chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15]))
			{
				dufcol(verticalIndex + 1);
				return 1;
			}
		}
		if (rstMap(FSETBCOL))
		{
			unsid();
			unmsg();
			if (chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15]))
			{
				dubcol(verticalIndex + 1);
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

						iniFile.customHoopX = iniFile.hoopSizeX;
						iniFile.customHoopY = iniFile.hoopSizeY;
						setMap(MSGOF);
						sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_CUSTHUP], iniFile.hoopSizeX / PFGRAN, iniFile.hoopSizeY / PFGRAN);
						shoMsg(msgbuf);
						break;

					case SMALHUP:

						iniFile.hoopSizeX = SHUPX;
						iniFile.hoopSizeY = SHUPY;
						iniFile.hoopType = SMALHUP;
						break;

					case LARGHUP:

						iniFile.hoopSizeX = LHUPX;
						iniFile.hoopSizeY = LHUPY;
						iniFile.hoopType = LARGHUP;
						break;

					case CUSTHUP:

						iniFile.hoopSizeX = iniFile.customHoopX;
						iniFile.hoopSizeY = iniFile.customHoopY;
						iniFile.hoopType = CUSTHUP;
						break;

					case HUP100:

						iniFile.hoopSizeX = HUP100XY;
						iniFile.hoopSizeY = HUP100XY;
						iniFile.hoopType = CUSTHUP;
						break;
					}
					unzoomedRect.x = iniFile.hoopSizeX;
					unzoomedRect.y = iniFile.hoopSizeY;
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
			fmovdif.x = selectedPoint.x - ((tbig.right - tbig.left) / 2 + tbig.left);
			fmovdif.y = selectedPoint.y - ((tbig.top - tbig.bottom) / 2 + tbig.bottom);
			for (ind = 0; ind < clipboardFormsCount; ind++) {

				closestFormToCursor = formIndex + ind;
				fvars(closestFormToCursor);
				for (ine = 0; ine < SelectedForm->sides; ine++) {

					SelectedForm->vertices[ine].x += fmovdif.x;
					SelectedForm->vertices[ine].y += fmovdif.y;
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
		if (chkMap(SIDCOL) && chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15])) {

			savdo();
			if (rstMap(FSETFCOL))
			{
				dufcol(verticalIndex + 1);
				return 1;
			}
			if (rstMap(BRDSID))
			{
				nubrdcol(verticalIndex);
				goto chkcolx;
			}
			if (rstMap(APSID))
			{
				nulapcol(verticalIndex);
				goto chkcolx;
			}
			if (rstMap(FTHSID))
			{
				nufthcol(verticalIndex);
				goto chkcolx;
			}
			if (rstMap(UNDCOL))
			{
				SelectedForm->underlayColor = verticalIndex & 0xf;
				refilfn();
				coltab();
				goto chkcolx;
			}
			if (rstMap(FSETUCOL))
			{
				dundcol(verticalIndex + 1);
				return 1;
			}
			nufilcol(verticalIndex);
		chkcolx:;
			msgbuf[0] = (TCHAR)verticalIndex + 0x30;
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
			if (chkwnd(hDiscardButton)) {

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

				if (!chkwnd(hCancelButton))
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
			if (chkwnd(hDiscardButton))
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

					stretchBoxLine[ind].x = formOutlineRectangle[ind << 1].x;
					stretchBoxLine[ind].y = formOutlineRectangle[ind << 1].y;
				}
				stretchBoxLine[4].x = stretchBoxLine[0].x;
				stretchBoxLine[4].y = stretchBoxLine[0].y;
				if (selectedFormControlVertex & 1)
					setMap(STRTCH);
				else {

					setMap(EXPAND);
					xyRatio = (double)(stitchRangeRect.right - stitchRangeRect.left) / (stitchRangeRect.top - stitchRangeRect.bottom);
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
			if (chkwnd(hDiscardButton)) {

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

					ind = duth(thrName);
					thrName[ind] = 't';
					setMap(IGNAM);
					thrsav();
					thrName[ind] = 'r';
					if (fileVersionIndex)
						fileName[ind] = fileVersionIndex + 0x2f;
					setMap(REDOLD);
					nuFil();
					fileName[ind] = 'r';
					switch (fileVersionIndex) {

					case 3:

						movbak('1', '2');

					case 2:

						movbak('0', '1');

					case 1:

						movbak('r', '0');
					}
					movbak('t', 'r');
					thrName[ind] = 't';
					DeleteFile(thrName);
					thrName[ind] = 'r';
					hFile = CreateFile(thrName, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
						OPEN_EXISTING, 0, NULL);
					if (hFile == INVALID_HANDLE_VALUE)
						hFile = 0;
					return 1;
				}
				GetWindowRect(hCancelButton, &trct);
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

			if (chkMsgs(msg.pt, hChangeThreadSizeWin[0], hChangeThreadSizeWin[2])) {

				verticalIndex -= 13;
				threadSize[threadSizeSelected][0] = cmap[verticalIndex];
				threadSizeIndex[threadSizeSelected] = verticalIndex;
				strncpy_s(buf, threadSize[threadSizeSelected], 2);
				buf[2] = 0;
				SetWindowText(hThreadSizeWin[threadSizeSelected], buf);
				setMap(RESTCH);
				for (ind = 0; ind < 3; ind++)
					DestroyWindow(hChangeThreadSizeWin[ind]);
				return 1;
			} else {

				for (ind = 0; ind < 3; ind++)
					DestroyWindow(hChangeThreadSizeWin[ind]);
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

				ind = (msg.pt.y - trct.top - 1) / (buttonHeight - 4);
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
		if (preferenceIndex == PAP + 1 && chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15])) {

			sprintf_s(msgbuf, sizeof(msgbuf), "%d", verticalIndex);
			underlayColor = verticalIndex;
			SetWindowText(thDat[PAP], msgbuf);
			unsid();
			return 1;
		}
		if (rstMap(SIDACT)) {

			savdo();
			if (formMenuChoice == LFTHTYP) {

				for (ind = 0; ind < 6; ind++) {

					if (msg.hwnd == hsidWnd[ind]) {

						SelectedForm->fillInfo.feather.fillType = ind + 1;
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
			SelectedForm->borderColor &= 0xf;
			if (rstMap(BRDACT)) {

				if (iseclp(closestFormToCursor))
					deleclp(closestFormToCursor);
				if (msg.hwnd == hsidWnd[0]) {

					SelectedForm->edgeType = 0;
					refilfn();
					coltab();
					unsid();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[1]) {

					if (SelectedForm->edgeType) {

						cod = SelectedForm->edgeType&NEGUND;
						if (cod == EGCLP || cod == EGSAT || cod == EGAP)
							bsizpar();
						SelectedForm->edgeType = EGLIN;
						goto didfil;
					} else {

						unmsg();
						unsid();
						bord();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[2]) {

					if (SelectedForm->edgeType) {

						cod = SelectedForm->edgeType&NEGUND;
						if (cod == EGCLP || cod == EGSAT || cod == EGAP)
							bsizpar();
						SelectedForm->edgeType = EGBLD;
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

					if (SelectedForm->edgeType) {

						switch (SelectedForm->edgeType) {

						case EGCLP:

							bsizpar();

						case EGLIN:
						case EGBLD:

							SelectedForm->borderSize = borderWidth;
							SelectedForm->edgeSpacing = stitchSpace;
							break;

						case EGPRP:

							SelectedForm->edgeSpacing /= 2;
							break;
						}
						SelectedForm->edgeType = EGSAT;
						if (chku(DUND))
							SelectedForm->edgeType |= EGUND;
						goto didfil;
					} else {

						unmsg();
						unsid();
						satbrd();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[5]) {

					if (SelectedForm->fillType) {

						delmfil(SelectedForm->fillColor);
						SelectedForm->fillType = 0;
					}
					if (SelectedForm->edgeType) {

						if (SelectedForm->edgeType == EGLIN || SelectedForm->edgeType == EGBLD || SelectedForm->edgeType == EGCLP) {

							SelectedForm->borderSize = borderWidth;
							SelectedForm->edgeSpacing = stitchSpace;
							if (SelectedForm->edgeType == EGCLP)
								bsizpar();
						}
						SelectedForm->edgeType = EGAP;
						if (chku(DUND))
							SelectedForm->edgeType |= EGUND;
						SelectedForm->borderColor |= (underlayColor << 4);
						goto didfil;
					} else {

						unmsg();
						unsid();
						apliq();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[6]) {

					if (SelectedForm->edgeType) {

						switch (SelectedForm->edgeType) {

						case EGCLP:

							bsizpar();
						case EGLIN:
						case EGBLD:

							SelectedForm->borderSize = borderWidth;
							SelectedForm->edgeSpacing = stitchSpace;
							break;

						case EGSAT:

							SelectedForm->edgeSpacing *= 2;
						}
						SelectedForm->edgeType = EGPRP;
						if (chku(DUND))
							SelectedForm->edgeType |= EGUND;
						goto didfil;
					} else {

						unmsg();
						unsid();
						prpbrd(stitchSpace);
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[7]) {

					if (SelectedForm->edgeType) {

						if (SelectedForm->edgeType == EGLIN || SelectedForm->edgeType == EGBLD || SelectedForm->edgeType == EGCLP) {

							SelectedForm->borderSize = borderWidth;
							SelectedForm->edgeSpacing = stitchSpace;
							if (SelectedForm->edgeType == EGCLP)
								bsizpar();
						}
						SelectedForm->edgeType = EGHOL;
						goto didfil;
					} else {

						unmsg();
						unsid();
						bhol();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[8]) {

					if (SelectedForm->edgeType) {

						if (SelectedForm->edgeType == EGLIN || SelectedForm->edgeType == EGBLD || SelectedForm->edgeType == EGCLP) {

							SelectedForm->borderSize = borderWidth;
							SelectedForm->edgeSpacing = stitchSpace;
							if (SelectedForm->edgeType == EGCLP)
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

					if (SelectedForm->edgeType) {

						cod = SelectedForm->edgeType&NEGUND;
						if (cod == EGCLP || cod == EGSAT || cod == EGAP)
							bsizpar();
						SelectedForm->edgeType = EGDUB;
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

				if (SelectedForm->fillType == SAT&&SelectedForm->satinGuideCount)
					delsac(closestFormToCursor);
				if ((SelectedForm->edgeType&NEGUND) == EGAP) {

					SelectedForm->edgeType = EGSAT;
					if (chku(DUND))
						SelectedForm->edgeType |= EGUND;
				}
				if (msg.hwnd == hsidWnd[0]) {

					SelectedForm->type = POLI;
					delmfil(SelectedForm->fillColor);
					SelectedForm->fillType = 0;
					coltab();
					unsid();
					setMap(RESTCH);
					return 1;
				}
				if (msg.hwnd == hsidWnd[1]) {

					savdo();
					SelectedForm->type = POLI;
					if (SelectedForm->fillType) {

						respac();
						SelectedForm->fillType = VRTF;
						SelectedForm->type = POLI;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filvrt();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[2]) {

					SelectedForm->type = POLI;
					if (SelectedForm->fillType) {

						respac();
						SelectedForm->fillType = HORF;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filhor();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[3]) {

					SelectedForm->type = POLI;
					if (SelectedForm->fillType) {

						respac();
						SelectedForm->fillType = ANGF;
						SelectedForm->angleOrClipData.angle = iniFile.fillAngle;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filangl();
						return 1;
					}
				}
				if (msg.hwnd == hsidWnd[4]) {

					SelectedForm->type = SAT;
					if (SelectedForm->fillType) {

						respac();
						SelectedForm->fillType = SATF;
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

					SelectedForm->type = SAT;
					unmsg();
					unsid();
					clpfil();
					return 1;
				}
				if (msg.hwnd == hsidWnd[6]) {

					if (SelectedForm->fillType) {

						if (SelectedForm->fillType == CLPF)
							SelectedForm->fillSpacing = stitchSpace;
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
					if (SelectedForm->fillType == FTHF) {

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
						SelectedForm->fillType = TXVRTF;
						goto didfil;
					}
					dutxtfil();
				}
				if (msg.hwnd == hsidWnd[12])	//horizontal texture
				{
					if (istx(closestFormToCursor))
					{
						SelectedForm->fillType = TXHORF;
						goto didfil;
					}
					dutxtfil();
				}
				if (msg.hwnd == hsidWnd[13])	//angle texture
				{
					if (istx(closestFormToCursor))
					{
						SelectedForm->fillType = TXANGF;
						SelectedForm->angleOrClipData.angle = iniFile.fillAngle;
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
		if (hFormData) {

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

				SelectedForm->extendedAttribute ^= AT_FTHUP;
				goto didfil;
			}
			if (msg.hwnd == thDat[LFTHDWN]) {

				SelectedForm->extendedAttribute ^= AT_FTHDWN;
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
				if (SelectedForm->type == LIN)
					SelectedForm->type = POLI;
				else
					SelectedForm->type = LIN;
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

				cod = SelectedForm->attribute&SBLNT;
				if (cod)
					SelectedForm->attribute &= NSBLNT;
				else
					SelectedForm->attribute |= SBLNT;
				refil();
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (msg.hwnd == thDat[LBFIN]) {

				cod = SelectedForm->attribute&FBLNT;
				if (cod)
					SelectedForm->attribute &= NFBLNT;
				else
					SelectedForm->attribute |= FBLNT;
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

				SelectedForm->edgeType ^= EGUND;
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
		if (px2stch() && !(chkMap(SIZSEL) && chkMsgs(msg.pt, hChangeThreadSizeWin[0], hChangeThreadSizeWin[2]))) {

			unpat();
			if (rstMap(ROTAT)) {

				rotateBoxToCursorLine[1].x = msg.pt.x - stitchWindowOrigin.x;
				rotateBoxToCursorLine[1].y = msg.pt.y - stitchWindowOrigin.y;
				padj.x = rotateBoxToCursorLine[0].x - rotateBoxToCursorLine[1].x;
				padj.y = rotateBoxToCursorLine[0].y - rotateBoxToCursorLine[1].y;
				if (hypot(padj.x, padj.y) < CLOSENUF) {

					px2stch();
					rotationCenter.x = selectedPoint.x;
					rotationCenter.y = selectedPoint.y;
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
				if ((rstMap(SELBOX) || rstMap(INSRT)) && closestPointIndex != (unsigned)header.stitchCount - 1)
					lodclp(closestPointIndex);
				else
					lodclp(header.stitchCount);
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
				zoomBoxLine[0].x = zoomBoxLine[3].x = zoomBoxLine[4].x = msg.pt.x - stitchWindowOrigin.x;
				zoomBoxLine[0].y = zoomBoxLine[1].y = msg.pt.y - stitchWindowOrigin.y;
				zoomBoxLine[4].y = zoomBoxLine[0].y - 1;
				px2stch();
				zoomBoxOrigin.x = selectedPoint.x;
				zoomBoxOrigin.y = selectedPoint.y;
				setMap(VCAPT);
				return 1;
			}
			if (formIndex && !chkMap(FRMOF)) {

				if (!chkMap(INSRT)) {

					if (rstMap(FORMSEL))
						ritfrct(closestFormToCursor, stitchWindowDC);
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
				if (chkMap(INSRT) && header.stitchCount < MAXSEQ) {

					px2stch();
					cod = (activeColor | USMSK | (activeLayer << LAYSHFT) | NOTFRM)&NKNOTMSK;
					if (chkMap(LIN1)) {

						if (chkMap(BAKEND)) {

							xlin1();
							ind = header.stitchCount;
							stitchBuffer[ind].x = selectedPoint.x;
							stitchBuffer[ind].y = selectedPoint.y;
							stitchBuffer[ind].attribute = cod;
							duzrat();
							stch2px1(ind);
							insertLine[0].x = stitchSizePixels.x;
							insertLine[0].y = stitchSizePixels.y;
							insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
							insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
							header.stitchCount++;
							coltab();
							setMap(RESTCH);
							return 1;
						} else {

							xlin1();
							for (ind = header.stitchCount; ind; ind--) {

								stitchBuffer[ind].attribute = stitchBuffer[ind - 1].attribute;
								stitchBuffer[ind].x = stitchBuffer[ind - 1].x;
								stitchBuffer[ind].y = stitchBuffer[ind - 1].y;
							}
							stitchBuffer[0].attribute = cod;
							stitchBuffer[ind].attribute &= (~KNOTMSK);
							stitchBuffer[0].x = selectedPoint.x;
							stitchBuffer[0].y = selectedPoint.y;
							stch2px1(ind);
							insertLine[0].x = stitchSizePixels.x;
							insertLine[0].y = stitchSizePixels.y;
							insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
							insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
							header.stitchCount++;
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
						ind = header.stitchCount;
						do {

							stitchBuffer[ind].x = stitchBuffer[ind - 1].x;
							stitchBuffer[ind].y = stitchBuffer[ind - 1].y;
							stitchBuffer[ind].attribute = stitchBuffer[ind - 1].attribute;
							ind--;
						} while (ind > (long)closestPointIndex);
						header.stitchCount++;
						closestPointIndex++;
						stitchBuffer[closestPointIndex].x = selectedPoint.x;
						stitchBuffer[closestPointIndex].y = selectedPoint.y;
						stitchBuffer[closestPointIndex].attribute = cod;
						xlin();
						insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
						insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
						stch2px1(closestPointIndex);
						insertLine[0].x = stitchSizePixels.x;
						insertLine[0].y = stitchSizePixels.y;
						stch2px1(closestPointIndex + 1);
						insertLine[2].x = stitchSizePixels.x;
						insertLine[2].y = stitchSizePixels.y;
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
					setbak(threadSizePixels[stitchBuffer[closestPointIndex].attribute & 0xf] + 3);
					linePoints = new POINT[3];
					lineIndex = 0;
					SetROP2(stitchWindowDC, R2_NOTXORPEN);
					if (closestPointIndex == 0) {

						if (zoomFactor < STCHBOX) {

							SelectObject(stitchWindowDC, linePen);
							stchbox(0, stitchWindowDC);
							stchbox(1, stitchWindowDC);
						}
						SetROP2(stitchWindowDC, R2_COPYPEN);
						drwLin(0, 2, backgroundPen);
					} else {

						if (closestPointIndex == (unsigned)header.stitchCount - 1) {

							if (zoomFactor < STCHBOX) {

								SelectObject(stitchWindowDC, linePen);
								stchbox(header.stitchCount - 1, stitchWindowDC);
								stchbox(header.stitchCount - 2, stitchWindowDC);
							}
							SetROP2(stitchWindowDC, R2_COPYPEN);
							drwLin(header.stitchCount - 2, 2, backgroundPen);
						} else {

							if (zoomFactor < STCHBOX) {

								SelectObject(stitchWindowDC, linePen);
								stchbox(closestPointIndex - 1, stitchWindowDC);
								stchbox(closestPointIndex, stitchWindowDC);
								stchbox(closestPointIndex + 1, stitchWindowDC);
							}
							SetROP2(stitchWindowDC, R2_COPYPEN);
							drwLin(closestPointIndex - 1, 3, backgroundPen);
						}
					}
					delete[] linePoints;
					moveLine0[1].x = moveLine1[0].x = msg.pt.x - stitchWindowOrigin.x;
					moveLine0[1].y = moveLine1[0].y = msg.pt.y - stitchWindowOrigin.y;
					if (closestPointIndex <= 0)
						rstMap(ISDWN);
					else {

						setMap(ISDWN);
						stch2px1(closestPointIndex - 1);
						moveLine0[0].x = stitchSizePixels.x;
						moveLine0[0].y = stitchSizePixels.y;
					}
					tlng = header.stitchCount - 1;
					if (closestPointIndex >= (unsigned)tlng)
						rstMap(ISUP);
					else {

						setMap(ISUP);
						stch2px1(closestPointIndex + 1);
						moveLine1[1].x = stitchSizePixels.x;
						moveLine1[1].y = stitchSizePixels.y;
					}
					dulin();
					SetCapture(hWnd);
					setMap(CAPT);
					ritnum(STR_NUMSEL, closestPointIndex);
				}
			} else {

				if (px2stch()) {

					savdo();
					header.stitchCount = 1;
					insertLine[0].x = msg.pt.x - stitchWindowOrigin.x;
					insertLine[0].y = msg.pt.y - stitchWindowOrigin.y;
					stitchBuffer[0].attribute = USMSK | activeColor | layerIndex | NOTFRM;
					stitchBuffer[0].x = selectedPoint.x;
					stitchBuffer[0].y = selectedPoint.y;
					colorChanges = 1;
					colorChangeTable[0].colorIndex = activeColor;
					colorChangeTable[0].stitchIndex = 0;
					colorChangeTable[1].stitchIndex = 1;
					setMap(LIN1);
					setMap(INSRT);
					SetCapture(hWnd);
					closestPointIndex = 1;
					setMap(INIT);
					setMap(BAKEND);
				}
			}
			ritot(header.stitchCount);
			setMap(BOXSLCT);
			setMap(BZUMIN);
			setMap(NOSEL);
			zoomBoxLine[0].x = zoomBoxLine[3].x = zoomBoxLine[4].x = msg.pt.x - stitchWindowOrigin.x;
			zoomBoxLine[0].y = zoomBoxLine[1].y = msg.pt.y - stitchWindowOrigin.y;
			zoomBoxLine[4].y = zoomBoxLine[0].y - 1;
			px2stch();
			zoomBoxOrigin.x = selectedPoint.x;
			zoomBoxOrigin.y = selectedPoint.y;
			setMap(VCAPT);
			return 1;
		}
		if (msg.hwnd == hButtonWin[HBOXSEL]) {

			boxsel();
			return 1;
		}
		if (msg.hwnd == hButtonWin[HUPTO]) {

			toglup();
			return 1;
		}
		if (msg.hwnd == hButtonWin[HHID]) {

			toglHid();
			return 1;
		}
		if (chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15])) {

			if (msg.message == WM_LBUTTONDOWN) {

				savdo();
				cod = activeColor;
				activeColor = verticalIndex & 0xf;
				redraw(hUserColorWin[cod]);
				redraw(hUserColorWin[activeColor]);
				if (chkMap(HID)) {

					rstMap(SELBOX);
					rstMap(GRPSEL);
					rstMap(SCROS);
					rstMap(ECROS);
					setMap(RESTCH);
					redraw(hButtonWin[HHID]);
				} else {

					if (selectedFormCount) {

						nucols();
						coltab();
						setMap(RESTCH);
						return 1;
					} else {

						if (chkMap(FORMSEL)) {

							SelectedForm = &formList[closestFormToCursor];
							if (SelectedForm->fillType || SelectedForm->edgeType || SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) {

								if (formList[closestFormToCursor].fillType) {

									formList[closestFormToCursor].fillColor = activeColor;
									if (formList[closestFormToCursor].fillType == FTHF)
										formList[closestFormToCursor].fillInfo.feather.color = activeColor;
								}
								if (formList[closestFormToCursor].edgeType)
								{
									if (formList[closestFormToCursor].edgeType == EGAP)
									{
										formList[closestFormToCursor].borderColor &= 0xf0;
										formList[closestFormToCursor].borderColor |= activeColor;
									} else
										formList[closestFormToCursor].borderColor = activeColor;
								}
								if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
									formList[closestFormToCursor].underlayColor = activeColor;
								cod = closestFormToCursor << FRMSHFT;
								for (ind = 0; ind < header.stitchCount; ind++) {

									ine = stitchBuffer[ind].attribute;
									if (ine&ALTYPMSK && (ine&FRMSK) == cod && (ine&TYPMSK) != TYPMSK)
									{
										ine &= NCOLMSK;
										ine |= activeColor;
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
									stitchBuffer[ind].attribute |= activeColor;
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
		if (chkMsgs(msg.pt, hUserColorWin[0], hUserColorWin[15])) {

			if (msg.message == WM_LBUTTONDOWN&&nuCol(userColor[verticalIndex])) {

				savdo();
				userColor[verticalIndex] = ccColorStruct.rgbResult;
				userPen[verticalIndex] = nuPen(userPen[verticalIndex], 1, userColor[verticalIndex]);
				hUserColorBrush[verticalIndex] = nuBrush(hUserColorBrush[verticalIndex], userColor[verticalIndex]);
				redraw(hUserColorWin[verticalIndex]);
				setMap(RESTCH);
			}
			return 1;
		}
		if (chkMsgs(msg.pt, hThreadSizeWin[0], hThreadSizeWin[15])) {

			if (msg.message == WM_LBUTTONDOWN) {

				savdo();
				threadSizeSelected = verticalIndex;
				for (ind = 0; ind < 3; ind++)
					hChangeThreadSizeWin[ind] = nuSiz(ind);
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

				thumbnailIndex = 0;
				nuthsel();
				break;

			case VK_END:

				thumbnailIndex = thumbnailCount;
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
				SetWindowText(hGeneralNumberInputBox, msgbuf);
				return 1;
			}
		}
		if (formMenuChoice || preferenceIndex) {

			if (chkminus(cod)) {

				msgIndex = 1;
				sideWindowEntryBuffer[0] = '-';
				SetWindowText(hSideMessageWin, sideWindowEntryBuffer);
				return 1;
			}
			if (dunum(cod)) {

				if (preferenceIndex == PSHO + 1 || preferenceIndex == PBOX + 1) {

					msgbuf[0] = (TCHAR)numericCode;
					msgbuf[1] = 0;
					if (preferenceIndex == PSHO + 1) {

						showStitchThreshold = unthrsh(numericCode - 0x30);
						SetWindowText(thDat[PSHO], msgbuf);
					} else {

						stitchBoxesThreshold = unthrsh(numericCode - 0x30);
						SetWindowText(thDat[PBOX], msgbuf);
					}
					unsid();
				} else {

					sideWindowEntryBuffer[msgIndex++] = numericCode;
					sideWindowEntryBuffer[msgIndex] = 0;
					SetWindowText(hSideMessageWin, sideWindowEntryBuffer);
				}
				return 1;
			}
			switch (cod) {

			case 0x6e://numpat period
			case 0xbe://period

				sideWindowEntryBuffer[msgIndex++] = '.';
				sideWindowEntryBuffer[msgIndex] = 0;
				SetWindowText(hSideMessageWin, sideWindowEntryBuffer);
				return 1;

			case 8:	//backspace

				if (msgIndex) {

					sideWindowEntryBuffer[--msgIndex] = 0;
					SetWindowText(hSideMessageWin, sideWindowEntryBuffer);
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
				SetWindowText(hGeneralNumberInputBox, msgbuf);
				return 1;
			}
			if (dunum(cod)) {

				if (chkMap(TRNIN0)) {

					traceInputBuffer[msgIndex++] = numericCode;
					traceInputBuffer[msgIndex] = 0;
					ind = atoi(traceInputBuffer);
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
					redraw(hTraceNumberInput);
				} else {

					msgbuf[msgIndex++] = numericCode;
					msgbuf[msgIndex] = 0;
					SetWindowText(hGeneralNumberInputBox, msgbuf);
				}
				return 1;
			}
			switch (cod) {

			case 0x6e://numpad period
			case 0xbe://period

				msgbuf[msgIndex++] = '.';
				msgbuf[msgIndex] = 0;
				SetWindowText(hGeneralNumberInputBox, msgbuf);
				return 1;

			case 8:	//backspace

				if (msgIndex) {

					msgIndex--;
					if (chkMap(TRNIN0)) {

						traceInputBuffer[msgIndex] = 0;
						redraw(hTraceNumberInput);
					} else {

						msgbuf[msgIndex] = 0;
						SetWindowText(hGeneralNumberInputBox, msgbuf);
					}
				}
				return 1;

			case VK_RETURN:

				if (chkMap(TRNIN0))
					dutrnum0(atoi(traceInputBuffer));
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
			stitchEntryBuffer[bufferIndex++] = numericCode;
			stitchEntryBuffer[bufferIndex] = 0;
			closestPointIndex = atoi(stitchEntryBuffer);
			if (closestPointIndex > (unsigned)header.stitchCount - 1) {

				sprintf_s(stitchEntryBuffer, sizeof(stitchEntryBuffer), "%d", header.stitchCount - 1);
				closestPointIndex = header.stitchCount - 1;
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
			redraw(hButtonWin[HHID]);
			CheckMenuItem(hMainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
			rstMap(COL);
			CheckMenuItem(hMainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
			setMap(DUMEN);
			rstMap(RUNPAT);
			rstMap(WASPAT);
			rstMap(WASBLAK);
			rstMap(GTUANG);
			rstMap(GTUSPAC);
			rstMap(GTWLKIND);
			rstMap(GTWLKLEN);
			DestroyWindow(hSpeedScrollBar);
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
			if (header.stitchCount == 1)
				header.stitchCount = 0;
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
				if (*pcsBMPFileName)
					bfil();
				setMap(RESTCH);
			} else {
#if PESACT
				//				iniFile.auxFileType=AUXPES;
				//				strcpy_s(fileName,"u:\\mrd\\t.thr");
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
				nudgfn(0, -iniFile.cursorNudgeStep);
			else
				seldwn();
			break;

		case VK_UP:

			if (GetKeyState(VK_CONTROL) & 0X8000)
				nudgfn(0, iniFile.cursorNudgeStep);
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

				hThrEdClip = RegisterClipboardFormat(thrEdClipFormat);
				hClipMem = GetClipboardData(hThrEdClip);
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
							l_clipData = (fPOINT*)&clipboardFormPointsData[1];
							for (ind = 0; ind <= clipboardFormPointsData->pointCount; ind++) {

								iseq[ind + 1].x = l_clipData[ind].x;
								iseq[ind + 1].y = l_clipData[ind].y;
							}
							ine = nxt(closestVertexToCursor);
							ind++;
							iseq[ind].x = currentFormVertices[ine].x;
							iseq[ind].y = currentFormVertices[ine].y;
							outputIndex = ind + 1;
							formPointsAsLine = (POINT*)&iseq[outputIndex];
							setpclp();
							setMap(FPUNCLP);
							setMap(SHOP);
							dupclp();
						} else {

							fmovdif.x = fmovdif.y = 0;
							setMap(FUNCLP);
							closestFormToCursor = formIndex;
							SelectedForm = &formList[formIndex];
							FillMemory(SelectedForm, sizeof(FRMHED), 0);
							SelectedForm->type = LIN;
							SelectedForm->sides = clipboardFormPointsData->pointCount + 1;
							SelectedForm->vertices = adflt(SelectedForm->sides);
							fvars(closestFormToCursor);
							MoveMemory(SelectedForm->vertices, &clipboardFormPointsData[1], sizeof(fPOINT)*SelectedForm->sides);
							setMap(INIT);
							newFormVertexCount = SelectedForm->sides;
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

							SelectedForm = &formList[formIndex + ind];
							SelectedForm->vertices = adflt(SelectedForm->sides);
							for (ine = 0; ine < SelectedForm->sides; ine++) {

								SelectedForm->vertices[ine].x = currentFormVertices[inf].x;
								SelectedForm->vertices[ine].y = currentFormVertices[inf++].y;
							}
						}
						l_sac = (SATCON*)&currentFormVertices[inf];
						inf = 0;
						for (ind = 0; ind < clipboardFormsCount; ind++) {

							SelectedForm = &formList[formIndex + ind];
							if (SelectedForm->type == SAT&&SelectedForm->satinGuideCount) {

								SelectedForm->satinOrAngle.sac = adsatk(SelectedForm->satinGuideCount);
								for (ine = 0; ine < SelectedForm->satinGuideCount; ine++) {

									SelectedForm->satinOrAngle.sac[ine].start = l_sac[inf].start;
									SelectedForm->satinOrAngle.sac[ine].finish = l_sac[inf++].finish;
								}
							}
						}
						l_clipData = (fPOINT*)&l_sac[inf];
						inf = 0;
						for (ind = 0; ind < clipboardFormsCount; ind++) {

							SelectedForm = &formList[formIndex + ind];
							if (isclpx(formIndex + ind)) {

								SelectedForm->angleOrClipData.clip = adclp(SelectedForm->lengthOrCount.clipCount);
								for (ine = 0; (unsigned)ine < SelectedForm->lengthOrCount.clipCount; ine++) {

									SelectedForm->angleOrClipData.clip[ine].x = l_clipData[inf].x;
									SelectedForm->angleOrClipData.clip[ine].y = l_clipData[inf++].y;
								}
							}
							if (iseclpx(formIndex + ind)) {

								SelectedForm->borderClipData = adclp(SelectedForm->clipEntries);
								for (ine = 0; ine < SelectedForm->clipEntries; ine++) {

									SelectedForm->borderClipData[ine].x = l_clipData[inf].x;
									SelectedForm->borderClipData[ine].y = l_clipData[inf++].y;
								}
							}
						}
						pts = (TXPNT*)&l_clipData[inf];
						ptx = &txpnts[txad];
						ine = 0;
						for (ind = 0; ind < clipboardFormsCount; ind++)
						{
							if (istx(formIndex + ind))
							{
								SelectedForm = &formList[formIndex + ind];
								ine += SelectedForm->fillInfo.texture.count;
								SelectedForm->fillInfo.texture.index += txad;
								MoveMemory(ptx, pts, (SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count - txad) * sizeof(TXPNT));
							}
						}
						txad += ine;
						GlobalUnlock(hClipMem);
						selectedFormsRectangle.top = selectedFormsRectangle.left = 0x7fffffff;
						selectedFormsRectangle.bottom = selectedFormsRectangle.right = 0;
						ratsr();
						for (outputIndex = 0; outputIndex < (unsigned)clipboardFormsCount; outputIndex++) {

#if  __UseASM__
							fselrct(outputIndex + formIndex);
#else
							fselrct(outputIndex + formIndex);
#endif
							selectedFormList[outputIndex] = outputIndex + formIndex;
						}
						selectedFormCount = clipboardFormsCount;
						selectedFormsSize.x = selectedFormsRectangle.right - selectedFormsRectangle.left;
						selectedFormsSize.y = selectedFormsRectangle.bottom - selectedFormsRectangle.top;
						setMap(INIT);
						formLines[0].x = formLines[3].x = formLines[4].x = selectedFormsRectangle.left;
						formLines[1].x = formLines[2].x = selectedFormsRectangle.right;
						formLines[0].y = formLines[1].y = formLines[4].y = selectedFormsRectangle.top;
						formLines[2].y = formLines[3].y = selectedFormsRectangle.bottom;
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
							SelectedForm->vertices = adflt(formList[formIndex].sides);
							currentFormVertices = (fPOINT*)&clipboardFormData[1];
							mvflpnt(&SelectedForm->vertices[0], &currentFormVertices[0], SelectedForm->sides);
							l_sac = (SATCON*)&currentFormVertices[SelectedForm->sides];
							ind = 0;
							if (SelectedForm->type == SAT&&SelectedForm->satinGuideCount) {

								SelectedForm->satinOrAngle.sac = adsatk(SelectedForm->satinGuideCount);
								mvsatk(&SelectedForm->satinOrAngle.sac[0], &l_sac[0], SelectedForm->satinGuideCount);
							}
							l_clipData = (fPOINT*)&l_sac[ind];
							inf = 0;
							if (isclpx(formIndex)) {

								SelectedForm->angleOrClipData.clip = adclp(SelectedForm->lengthOrCount.clipCount);
								mvflpnt(SelectedForm->angleOrClipData.clip, l_clipData, SelectedForm->lengthOrCount.clipCount);
								inf += SelectedForm->lengthOrCount.clipCount;
							}
							if (iseclpx(formIndex)) {

								l_clipData = (fPOINT*)&l_clipData[inf];
								SelectedForm->borderClipData = adclp(SelectedForm->clipEntries);
								mvflpnt(SelectedForm->borderClipData, l_clipData, SelectedForm->clipEntries);
								inf += SelectedForm->clipEntries;
							}
							pts = (TXPNT*)&l_clipData[inf];
							if (istx(formIndex))
							{
								SelectedForm->fillInfo.texture.index = txad;
								ptx = adtx(SelectedForm->fillInfo.texture.count);
								MoveMemory(ptx, pts, SelectedForm->fillInfo.texture.count * sizeof(TXPNT));
							}
						}
						GlobalUnlock(hClipMem);
						setMap(INIT);
						newFormVertexCount = SelectedForm->sides;
						if (SelectedForm->type != LIN)
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
				redraw(hColorBar);
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
				groupStitchIndex = colorChangeTable[ind].stitchIndex;
				grpAdj();
				redraw(hColorBar);
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
						closestPointIndex = colorChangeTable[ind].stitchIndex;
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

					groupStitchIndex = header.stitchCount - 1;
					dubx();
					setMap(GRPSEL);
				} else {

					if (chkMap(GRPSEL)) {

						if (groupStitchIndex > closestPointIndex)
							groupStitchIndex = header.stitchCount - 1;
						else
							closestPointIndex = header.stitchCount - 1;
					} else {

						setMap(SELBOX);
						closestPointIndex = header.stitchCount - 1;
						setMap(RESTCH);
						return 1;
					}
				}
				grpAdj();
				redraw(hColorBar);
				break;
			}
			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (rstMap(SELBOX)) {

					dubx();
					setMap(GRPSEL);
					ind = pt2colInd(closestPointIndex);
				} else
					ind = pt2colInd(groupStitchIndex);
				groupStitchIndex = colorChangeTable[ind].stitchIndex - 1;
				grpAdj();
				redraw(hColorBar);
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000) {

					stch2px1(header.stitchCount - 1);
					endpnt();
					setMap(BAKEND);
					setMap(RESTCH);
				} else {

					if (chkMap(SELBOX)) {

						ind = pt2colInd(closestPointIndex);
						closestPointIndex = colorChangeTable[ind].stitchIndex - 1;
					} else {

						if (chkMap(LENSRCH)) {

							setsrch(largestStitchIndex);
							return 1;
						} else
							closestPointIndex = header.stitchCount - 1;
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
							if (closestPointIndex < (unsigned)header.stitchCount - 1) {

								setMap(GRPSEL);
								groupStitchIndex = closestPointIndex + 1;
							}
						} else {

							if (groupStitchIndex < (unsigned)header.stitchCount - 1) {

								groupStitchIndex++;
								nuAct(groupStitchIndex);
							}
						}
						grpAdj();
						redraw(hColorBar);
					}
				}
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(iniFile.cursorNudgeStep, 0);
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

								if (closestPointIndex < (unsigned)header.stitchCount - 1)
									closestPointIndex++;
								movbox();
								return 1;
							}
							if (chkMap(GRPSEL)) {

								if (groupStitchIndex < (unsigned)header.stitchCount - 1) {

									groupStitchIndex++;
									grpAdj();
									redraw(hColorBar);
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
						redraw(hColorBar);
					}
				}
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(-iniFile.cursorNudgeStep, 0);
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
									redraw(hColorBar);
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
						dumrk(selectedPoint.x, selectedPoint.y);
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

			if (chkMsgs(msg.pt, hDefaultColorWin[0], hDefaultColorWin[15]))
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

				strcpy_s(fileName, iniFile.prevNames[ind]);
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

			chkrng(&stitchRangeSize);
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
			prpbrd(stitchSpace);
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

			stch2px1(header.stitchCount - 1);
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

			switch (iniFile.auxFileType) {

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

			iniFile.maxStitchLength = MAXSIZ*PFGRAN;
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

				backgroundColor = ccBackgroundColorStruct.rgbResult;
				backgroundPen = nuPen(backgroundPen, 1, backgroundColor);
				backgroundPenWidth = 1;
				DeleteObject(hBackgroundBrush);
				hBackgroundBrush = CreateSolidBrush(backgroundColor);
				if (*pcsBMPFileName)
					bfil();
				zumhom();
			}
			break;

		case ID_BITCOL:

			if (nuBit()) {

				bitmapColor = fswap(ccBitMapColorStruct.rgbResult);
				if (*pcsBMPFileName)
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

				userColor[ind] = defCol[ind];
				hUserColorBrush[ind] = nuBrush(hUserColorBrush[ind], userColor[ind]);
				userPen[ind] = nuPen(userPen[ind], 1, userColor[ind]);
				redraw(hUserColorWin[ind]);
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

		hDefaultColorWin[ind] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			0,
			buttonHeight*ind,
			buttonWidth,
			buttonHeight,
			hWnd,
			NULL,
			hInst,
			NULL);

		hUserColorWin[ind] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonWidth,
			buttonHeight*ind,
			buttonWidth,
			buttonHeight,
			hWnd,
			NULL,
			hInst,
			NULL);

		buf[0] = threadSize[ind][0];
		hThreadSizeWin[ind] = CreateWindow(
			"STATIC",
			buf,
			SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonWidth << 1,
			buttonHeight*ind,
			buttonWidth,
			buttonHeight,
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
		WriteFile(hloc, (TCHAR*)homeDirectory, strlen(homeDirectory) + 1, &rot, 0);
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


		delsubl:	bswap	eax
					mov[edi], eax
					add		edi, 4
					bswap	eax
					shl		eax, 1
					loop	delsubl
					ret

		delsubr:	bswap	eax
					mov[edi], eax
					add		edi, 4
					bswap	eax
					shr		eax, 1
					loop	delsubr
					ret

		delsubt:	mov		eax, [esi]
					add		esi, 4
					bswap	eax
					mov[edi], eax
					add		edi, 4
					loop	delsubt
					ret

		//form cursor
		delups:		mov		ebx, pnt
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
		dulup0:		or [edi], eax
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

	unsigned	ind;
	TCHAR*		phom;	//pointer to the home file name

	strcpy_s(homeDirectory, __argv[0]);
	phom = strrchr(homeDirectory, '\\');
	if (phom)
		phom++;
	else {

		ind = GetCurrentDirectory(_MAX_PATH, homeDirectory);
		if (ind) {
			homeDirectory[ind++] = '\\';
			phom = &homeDirectory[ind];
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
		bcpy(fileName, __argv[1]);
#else
		strcpy_s(fileName, __argv[1]);
#endif
		if (!strncmp(fileName, "/F1:", 4)) {

			balaradFileName = &fileName[4];
			hBalaradFile = CreateFile(balaradFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			CloseHandle(hBalaradFile);
			if (hBalaradFile != INVALID_HANDLE_VALUE) {

#if  __UseASM__
				bcpy(balaradName0, balaradFileName);
#else
				strcpy_s(balaradName0, balaradFileName);
#endif
				if (__argc > 2) {

#if  __UseASM__
					bcpy(fileName, __argv[2]);
#else
					strcpy_s(fileName, __argv[2]);
#endif
					if (!strncmp(fileName, "/F2:", 4)) {

						balaradFileName = &fileName[4];
						hBalaradFile = CreateFile(balaradFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
						if (hBalaradFile != INVALID_HANDLE_VALUE) {

#if  __UseASM__
							bcpy(balaradName1, balaradFileName);
#else
							strcpy_s(balaradName1, balaradFileName);
#endif
							//ToDo - Does this line make sense?
							balaradFileName = (TCHAR*)&bseq;
							ReadFile(hBalaradFile, (void*)&balaradName2, 10000, &l_red, 0);
							strcat_s(balaradName2, "");
							if (l_red)
								redbal();
						}
					}
				}
				SetWindowText(hWnd, stab[STR_EMB]);
			}
			*fileName = 0;
			CloseHandle(hBalaradFile);
			DeleteFile(balaradName1);
		} else {

			for (ind = 2; ind < __argc; ind++) {

				strcat_s(fileName, " ");
				strcat_s(fileName, __argv[ind]);
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
		iniFile.prevNames[ind][0] = 0;
	duhom();
	strcpy_s(iniFileName, homeDirectory);
	strcat_s(iniFileName, "thred.ini");
	hIniFile = CreateFile(iniFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hIniFile == INVALID_HANDLE_VALUE)
		defpref();
	else {

		ReadFile(hIniFile, &iniFile, sizeof(iniFile), &wrot, 0);
		if (wrot < 2061)
			iniFile.formBoxSizePixels = DEFBPIX;
		strcpy_s(defaultDirectory, iniFile.defaultDirectory);
		strcpy_s(defaultBMPDirectory, iniFile.defaultDirectory);
		for (ind = 0; ind < 16; ind++) {

			userColor[ind] = iniFile.stitchColors[ind];
			customColor[ind] = iniFile.stitchPreferredColors[ind];
			customBackgroundColor[ind] = iniFile.backgroundPreferredColors[ind];
			bitmapBackgroundColors[ind] = iniFile.bitmapBackgroundColors[ind];
		}
		backgroundColor = iniFile.backgroundColor;
		bitmapColor = iniFile.bitmapColor;
		minStitchLength = iniFile.minStitchLength;
		showStitchThreshold = iniFile.showStitchThreshold;
		if (iniFile.threadSize30)
			threadSize30 = iniFile.threadSize30;
		if (iniFile.threadSize40)
			threadSize40 = iniFile.threadSize40;
		if (iniFile.threadSize60)
			threadSize60 = iniFile.threadSize60;
		if (iniFile.userStitchLength)
			userStitchLength = iniFile.userStitchLength;
		if (!iniFile.maxStitchLength)
			iniFile.maxStitchLength = MAXSIZ*PFGRAN;
		if (iniFile.smallStitchLength)
			smallStitchLength = iniFile.smallStitchLength;
		stitchBoxesThreshold = iniFile.stitchBoxesThreshold;
		if (iniFile.stitchSpace)
			stitchSpace = iniFile.stitchSpace;
		binaryVariableBitmap = iniFile.binaryVariableBitmap;
		if (iniFile.borderWidth)
			borderWidth = iniFile.borderWidth;
		if (iniFile.underlayColor)
			underlayColor = iniFile.underlayColor & 0xf;
		if (iniFile.snapLength)
			snapLength = iniFile.snapLength;
		if (iniFile.starRatio)
			starRatio = iniFile.starRatio;
		if (iniFile.spiralWrap)
			spiralWrap = iniFile.spiralWrap;
		if (iniFile.buttonholeFillCornerLength)
			buttonholeFillCornerLength = iniFile.buttonholeFillCornerLength;
		if (!iniFile.gridSize)
			iniFile.gridSize = 12;
		if (!iniFile.wavePoints)
			iniFile.wavePoints = IWAVPNTS;
		if (!iniFile.waveStart)
			iniFile.waveStart = IWAVSTRT;
		if (!iniFile.waveEnd)
			iniFile.waveEnd = IWAVEND;
		if (!iniFile.waveLobes)
			iniFile.waveLobes = IWAVS;
		if (!iniFile.featherFillType)
			iniFile.featherFillType = FDEFTYP;
		if (!iniFile.featherUpCount)
			iniFile.featherUpCount = FDEFUP;
		if (!iniFile.featherDownCount)
			iniFile.featherDownCount = FDEFDWN;
		if (!iniFile.featherRatio)
			iniFile.featherRatio = (float)FDEFRAT;
		if (!iniFile.featherMinStitchSize)
			iniFile.featherMinStitchSize = FDEFFLR;
		if (!iniFile.featherCount)
			iniFile.featherCount = FDEFNUM;
		if (!iniFile.daisyHoleDiameter)
			iniFile.daisyHoleDiameter = DAZHLEN;
		if (!iniFile.daisyPetalPoints)
			iniFile.daisyPetalPoints = DAZCNT;
		if (!iniFile.daisyInnerCount)
			iniFile.daisyInnerCount = DAZICNT;
		if (!iniFile.daisyDiameter)
			iniFile.daisyDiameter = DAZLEN;
		if (!iniFile.daisyPetalCount)
			iniFile.daisyPetalCount = DAZPETS;
		if (!iniFile.daisyPetalLen)
			iniFile.daisyPetalLen = DAZPLEN;

		switch (iniFile.hoopType) {

		case SMALHUP:

			iniFile.hoopSizeX = SHUPX;
			iniFile.hoopSizeY = SHUPY;
			break;

		case LARGHUP:

			iniFile.hoopSizeX = LHUPX;
			iniFile.hoopSizeY = LHUPY;
			break;

		case CUSTHUP:

			if (!iniFile.hoopSizeX)
				iniFile.hoopSizeX = LHUPX;
			if (!iniFile.hoopSizeY)
				iniFile.hoopSizeY = LHUPY;
			break;

		case HUP100:

			if (!iniFile.hoopSizeX)
				iniFile.hoopSizeX = HUP100XY;
			if (!iniFile.hoopSizeY)
				iniFile.hoopSizeY = HUP100XY;
			break;

		default:

			iniFile.hoopType = LARGHUP;
			iniFile.hoopSizeX = LHUPX;
			iniFile.hoopSizeY = LHUPY;
		}
		unzoomedRect.x = iniFile.hoopSizeX;
		unzoomedRect.y = iniFile.hoopSizeY;
		picotSpace = iniFile.picotSpace;
	}
	if (!iniFile.gridColor)
		iniFile.gridColor = DEFGRD;
	CloseHandle(hIniFile);
	if (!iniFile.fillAngle)
		iniFile.fillAngle = PI / 6;
	tdc = GetDC(0);
	screenSizePixels.cx = GetDeviceCaps(tdc, HORZRES);
	screenSizePixels.cy = GetDeviceCaps(tdc, VERTRES);
	if (iniFile.initialWindowCoords.left < 0)
		iniFile.initialWindowCoords.left = 0;
	if (iniFile.initialWindowCoords.top < 0)
		iniFile.initialWindowCoords.top = 0;
	ind = screenSizePixels.cx - 30;
	if (iniFile.initialWindowCoords.right > (int)ind)
		iniFile.initialWindowCoords.right = ind;
	if (iniFile.initialWindowCoords.bottom > screenSizePixels.cy)
		iniFile.initialWindowCoords.bottom = screenSizePixels.cy;
}

void trcsub(HWND* hwnd, unsigned xloc, unsigned yloc, unsigned hi) {

	*hwnd = CreateWindow(
		"STATIC",
		"",
		SS_OWNERDRAW | WS_CHILD | WS_BORDER,
		xloc,
		yloc,
		buttonWidth,
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
	if (iniFile.initialWindowCoords.left > lim.x)
		iniFile.initialWindowCoords.left = lim.x;
	if (iniFile.initialWindowCoords.right > lim.x)
		iniFile.initialWindowCoords.right = lim.x;
	if (iniFile.initialWindowCoords.left < 0)
		iniFile.initialWindowCoords.left = 0;
	if (iniFile.initialWindowCoords.right < 0)
		iniFile.initialWindowCoords.right = 0;
	if (iniFile.initialWindowCoords.top > lim.y)
		iniFile.initialWindowCoords.top = lim.y;
	if (iniFile.initialWindowCoords.bottom > lim.y)
		iniFile.initialWindowCoords.bottom = lim.y;
	if (iniFile.initialWindowCoords.top < 0)
		iniFile.initialWindowCoords.top = 0;
	if (iniFile.initialWindowCoords.bottom < 0)
		iniFile.initialWindowCoords.bottom = 0;
	if (iniFile.initialWindowCoords.right - iniFile.initialWindowCoords.left < 300) {

		iniFile.initialWindowCoords.left = 0;
		iniFile.initialWindowCoords.right = lim.x >> 1;
	}
	if (iniFile.initialWindowCoords.bottom - iniFile.initialWindowCoords.top < 300) {

		iniFile.initialWindowCoords.top = 0;
		iniFile.initialWindowCoords.bottom = lim.y >> 1;
	}
}

void init() {

	unsigned		ind, flg;
	unsigned long	thwid, mwid;
	long			selbox;
	RECT			tRct;
	RECT			wrct;
	HDC				totdc;
	TCHAR*			pchr;

	txad = 0;
#if	 __UseASM__
	fsizeof = sizeof(FRMHED) >> 2;
#endif
	LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
	hMainMenu = GetMenu(hWnd);
	totdc = GetDC(NULL);
	mwid = GetDeviceCaps(totdc, HORZRES);
	ReleaseDC(NULL, totdc);
	GetWindowRect(hWnd, &wrct);
	GetMenuItemRect(hWnd, hMainMenu, 0, &tRct);
	//menhi = tRct.bottom - tRct.top;
	wrct.left = tRct.left;
	wrct.right = tRct.right;
	for (ind = 0; ind <= M_HELP; ind++) {

		GetMenuItemRect(hWnd, hMainMenu, ind, &tRct);
		wrct.right += (tRct.right - tRct.left);
	}
	wrct.right += 20;
	thwid = wrct.right - wrct.left;
	mwid >>= 1;
	if (thwid < mwid)
		wrct.right = wrct.left += mwid;
	hfillMenu = GetSubMenu(hMainMenu, M_FIL);
	hfileMenu = GetSubMenu(hMainMenu, M_FILE);
	hEditMenu = GetSubMenu(hMainMenu, M_EDIT);
	hBorderFillMenu = GetSubMenu(hfillMenu, MFIL_BORD);
	hViewMenu = GetSubMenu(hMainMenu, M_VIEW);
	hViewSetMenu = GetSubMenu(hViewMenu, M_VIEW);
	qchk();
	hArrowCurs = LoadCursor(0, IDC_ARROW);
	hCrossCurs = LoadCursor(0, IDC_CROSS);
	crtcurs();
	redfils();
	clrMap(MAPLEN);						//clear the bitmap
	//set up the size variables
	mainDC = GetDC(hWnd);
	SetStretchBltMode(mainDC, COLORONCOLOR);
	stitchWindowDC = GetDCEx(hMainStitchWin, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	stitchWindowMemDC = CreateCompatibleDC(stitchWindowDC);
	screenSize_mm.cx = GetDeviceCaps(mainDC, HORZSIZE);
	screenSize_mm.cy = GetDeviceCaps(mainDC, VERTSIZE);
	chkirct();
	if (!chku(SAVMAX))
		MoveWindow(hWnd, iniFile.initialWindowCoords.left, iniFile.initialWindowCoords.top, iniFile.initialWindowCoords.right - iniFile.initialWindowCoords.left, iniFile.initialWindowCoords.bottom - iniFile.initialWindowCoords.top, 0);
	buttonWidth = txtWid("MM") + TXTSIDS;
	buttonWidthX3 = buttonWidth * 3;
	buttonHeight = textSize.cy + 4;
	numeralWidth = txtWid("0");
	selbox = txtWid("0");
	for (ind = 0; ind < NERCNT; ind++)
		boxOffset[ind] = selbox + selbox*ind;
	GetClientRect(hWnd, &mainWindowRect);
	stchWnd();
	lodstr();
	maxwid(STR_PRF0, STR_PRF27);
	if (!hIniFile) {

		//initialize the user color and thread size arrays
		for (ind = 0; ind < 16; ind++)
			userColor[ind] = defCol[ind];
	}
	for (ind = 0; ind < 16; ind++) {

		threadSize[ind][0] = '4';
		threadSize[ind][1] = '0';
	}
	if (!iniFile.traceLength)
		iniFile.traceLength = TRACLEN;
	if (!iniFile.traceRatio)
		iniFile.traceRatio = TRACRAT;
	if (!iniFile.chainSpace)
		iniFile.chainSpace = CHSDEF;
	if (!iniFile.chainRatio)
		iniFile.chainRatio = CHRDEF;
	if (!iniFile.cursorNudgeStep)
		iniFile.cursorNudgeStep = NUGINI;
	if (!iniFile.nudgePixels)
		iniFile.nudgePixels = DEFPIX;
	if (!iniFile.eggRatio)
		iniFile.eggRatio = DEFEGRAT;
	if (!iniFile.stitchSizePixels)
		iniFile.stitchSizePixels = DEFPNTPIX;
	if (!iniFile.formPointSizePixels)
		iniFile.formPointSizePixels = DEFPNTPIX;
	if (!iniFile.formSides)
		iniFile.formSides = 24;
	if (!iniFile.tearTailLength)
		iniFile.tearTailLength = (float)1.4;
	if (!iniFile.underlayStitchLen)
		iniFile.underlayStitchLen = DEFULEN;
	if (!iniFile.underlaySpacing)
		iniFile.underlaySpacing = DEFUSPAC;
	setgrd(iniFile.gridColor);
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
		hButtonWin[ind] = CreateWindow(
			"STATIC",
			pchr,
			flg,
			0,
			buttonHeight*(16 + ind),
			buttonWidthX3,
			buttonHeight,
			hWnd,
			NULL,
			hInst,
			NULL);
	}
	hTraceStepWin = CreateWindow(

		"STATIC",
		"",
		SS_NOTIFY | SS_CENTER | WS_CHILD | WS_BORDER,
		0,
		buttonHeight * 18,
		buttonWidthX3,
		buttonHeight,
		hWnd,
		NULL,
		hInst,
		NULL);

	hColorBar = CreateWindow(
		"STATIC",
		"",
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		mainWindowRect.right - COLSIZ,
		0,
		COLSIZ,
		mainWindowRect.bottom,
		hWnd,
		NULL,
		hInst,
		NULL);

	for (ind = 0; ind < 3; ind++) {

		trcsub(&hTraceControl[ind], buttonWidth*ind, 0, buttonHeight * 15);
		trcsub(&hTraceSelect[ind], buttonWidth*ind, buttonHeight * 15, buttonHeight);
		trcsub(&hTraceUp[ind], buttonWidth*ind, buttonHeight * 16, buttonHeight);
		trcsub(&hTraceDown[ind], buttonWidth*ind, buttonHeight * 17, buttonHeight);
		hTraceBrush[ind] = CreateSolidBrush(traceRGB[ind]);
	}

	nuRct();
	//create pens
	for (ind = 0; ind < 4; ind++)
		boxPen[ind] = CreatePen(PS_SOLID, 1, boxColor[ind]);
	linePen = CreatePen(PS_SOLID, 1, 0x404040);
	crossPen = CreatePen(PS_SOLID, 5, 0x804080);
	groupSelectPen = CreatePen(PS_SOLID, 1, 0x804080);
	gridPen = CreatePen(PS_SOLID, 1, iniFile.gridColor);
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

		threadSizePixels[ind] = 1;
		threadSizeIndex[ind] = 1;
		userPen[ind] = CreatePen(PS_SOLID, 1, userColor[ind]);
	}
	hBackgroundBrush = CreateSolidBrush(backgroundColor);
	//create brushes
	for (ind = 0; ind < 16; ind++) {

		hDefaultColorBrush[ind] = CreateSolidBrush(defCol[ind]);
		hUserColorBrush[ind] = CreateSolidBrush(userColor[ind]);
	}
	hBlackBrush = CreateSolidBrush(0);
	zoomFactor = 1;
	header.leadIn = 0x32;
	header.colorCount = 16;
	header.stitchCount = 0;
	GetDCOrgEx(stitchWindowDC, &stitchWindowOrigin);
	ladj();
	GetTextExtentPoint(stitchWindowMemDC, stab[STR_PIKOL], strlen(stab[STR_PIKOL]), &pickColorMsgSize);
	auxmen();
	fnamtabs();
	ritfnam(iniFile.designerName);
	strcpy_s(extendedHeader.modifierName, iniFile.designerName);
	extendedHeader.stgran = 0;
	for (ind = 0; ind < 26; ind++)
		extendedHeader.res[ind] = 0;
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
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_THRED], iniFile.designerName);
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
		mov		eax, DiplayedColorBitmap
		bts		eax, ebx
		mov		DiplayedColorBitmap, eax
	}
#else
	// mask ind for sanity
	_bittestandset((long *)&DiplayedColorBitmap, ind & 31);
#endif
}

void relin() {

	stch2px1(closestPointIndex - 1);
	moveLine0[0].x = stitchSizePixels.x;
	moveLine0[0].y = stitchSizePixels.y;
	stch2px1(closestPointIndex + 1);
	moveLine1[1].x = stitchSizePixels.x;
	moveLine1[1].y = stitchSizePixels.y;
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
	SelectObject(stitchWindowMemDC, hPen);
	if (lineIndex < 16000)
		Polyline(stitchWindowMemDC, linePoints, lineIndex);
	else {

		ine = 0;
		while (lineIndex) {

			if (lineIndex > 16000) {

				Polyline(stitchWindowMemDC, &linePoints[ine], 16000);
				ine += 15999;
				lineIndex -= 15999;
			} else {

				Polyline(stitchWindowMemDC, &linePoints[ine], lineIndex);
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

	rotateAnglePixels = atan2(stitchBuffer[moveAnchor + 1].y - stitchBuffer[moveAnchor].y, stitchBuffer[moveAnchor + 1].x - stitchBuffer[moveAnchor].x);
	tstch = &stitchBuffer[moveAnchor];
	if (tstch->x >= zoomRect.left&&tstch->x <= zoomRect.right
		&&tstch->y >= zoomRect.bottom&&tstch->y <= zoomRect.top) {

		sdCor2px(stitchBuffer[moveAnchor], &rotationCenterPixels);
		arlin[0].x = arlin[6].x = rotationCenterPixels.x;
		arlin[0].y = arlin[6].y = rotationCenterPixels.y;
		trot.x = rotationCenterPixels.x + 12;
		trot.y = rotationCenterPixels.y + 2;
		rotpix(trot, &arlin[1]);
		trot.y = rotationCenterPixels.y - 2;
		rotpix(trot, &arlin[5]);
		trot.y = rotationCenterPixels.y + 6;
		rotpix(trot, &arlin[2]);
		trot.y = rotationCenterPixels.y - 6;
		rotpix(trot, &arlin[4]);
		trot.x = rotationCenterPixels.x + 20;
		trot.y = rotationCenterPixels.y;
		rotpix(trot, &arlin[3]);
		SelectObject(stitchWindowMemDC, formPen);
		SetROP2(stitchWindowMemDC, R2_XORPEN);
		Polyline(stitchWindowMemDC, arlin, 7);
		SetROP2(stitchWindowMemDC, R2_COPYPEN);
	}
}

unsigned chkup(unsigned cnt, unsigned ind) {

	if (chkMap(UPTO) && closestPointIndex) {

		if (colorChangeTable[ind].stitchIndex < closestPointIndex) {

			if (colorChangeTable[ind + 1].stitchIndex < closestPointIndex)
				return cnt;
			else
				return closestPointIndex - colorChangeTable[ind].stitchIndex + 1;
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

	if (!chku(KNOTOF) && knotCount&&header.stitchCount) {

		for (ind = 0; ind < knotCount; ind++) {

			stCor2px(stitchBuffer[knots[ind]], &pnt);
			SelectObject(stitchWindowMemDC, knotPen);
			SetROP2(stitchWindowMemDC, R2_XORPEN);
			tlin[0].x = tlin[3].x = tlin[4].x = pnt.x - KSIZ;
			tlin[1].x = tlin[2].x = pnt.x + KSIZ;
			tlin[0].y = tlin[1].y = tlin[4].y = pnt.y + KSIZ;
			tlin[2].y = tlin[3].y = pnt.y - KSIZ;
			Polyline(stitchWindowMemDC, tlin, 5);
			tlin[0].x = pnt.x - KLSIZ;
			tlin[1].x = pnt.x + KLSIZ;
			tlin[0].y = tlin[1].y = pnt.y;
			Polyline(stitchWindowMemDC, tlin, 2);
			SetROP2(stitchWindowMemDC, R2_COPYPEN);
		}
	}
}

void dugrid() {

	POINT		tlin[2];
	RECT		grdrct;
	unsigned	ind;

	if (zoomFactor < showStitchThreshold || !showStitchThreshold) {

		SetROP2(stitchWindowMemDC, R2_XORPEN);
		SelectObject(stitchWindowMemDC, gridPen);
		grdrct.left = ceil(zoomRect.left / iniFile.gridSize);
		grdrct.right = floor(zoomRect.right / iniFile.gridSize);
		grdrct.bottom = ceil(zoomRect.bottom / iniFile.gridSize);
		grdrct.top = floor(zoomRect.top / iniFile.gridSize);
		tlin[0].x = 0;
		tlin[1].x = stitchWindowClientRect.right;
		for (ind = grdrct.bottom; ind <= (unsigned)grdrct.top; ind++) {

			tlin[0].y = tlin[1].y = stitchWindowClientRect.bottom - (ind*iniFile.gridSize - zoomRect.bottom)*zoomRatio.y + 0.5;
			Polyline(stitchWindowMemDC, tlin, 2);
		}
		tlin[0].y = 0;
		tlin[1].y = stitchWindowClientRect.bottom;
		for (ind = grdrct.left; ind <= (unsigned)grdrct.right; ind++) {

			tlin[0].x = tlin[1].x = (ind*iniFile.gridSize - zoomRect.left)*zoomRatio.x + 0.5;
			Polyline(stitchWindowMemDC, tlin, 2);
		}
		SetROP2(stitchWindowMemDC, R2_COPYPEN);
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
	for (ind = 0; ind < colorChanges; ind++) {

		lineIndex = colorChangeTable[ind + 1].stitchIndex - colorChangeTable[ind].stitchIndex;
		if (lineIndex > scnt)
			scnt = lineIndex;
	}
	linePoints = new POINT[scnt + 2];
	FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
	duzrat();
	if (*pcsBMPFileName && !chkMap(HIDMAP) && !chkMap(UPTO)) {

		if (chkMap(WASTRAC))
			tdc = hTraceDC;
		else
			tdc = bitmapDC;
		if (bitar())
			StretchBlt(stitchWindowMemDC, bitmapDstRect.left, bitmapDstRect.top, bitmapDstRect.right - bitmapDstRect.left, bitmapDstRect.bottom - bitmapDstRect.top,
				tdc, bitmapSrcRect.left, bitmapSrcRect.top, bitmapSrcRect.right - bitmapSrcRect.left, bitmapSrcRect.bottom - bitmapSrcRect.top, SRCCOPY);
	}
	dugrid();
	if (chkMap(INIT)) {

		if (chkMap(ZUMED)) {

			scrollInfo.cbSize = sizeof(scrollInfo);
			scrollInfo.fMask = SIF_ALL;
			scrollInfo.nMax = unzoomedRect.y;
			scrollInfo.nMin = 0;
			scrollInfo.nPage = zoomRect.top - zoomRect.bottom;
			scrollInfo.nPos = unzoomedRect.y - zoomRect.top;
			SetScrollInfo(hVerticalScrollBar, SB_CTL, &scrollInfo, TRUE);
			scrollInfo.nMax = unzoomedRect.x;
			scrollInfo.nPage = zoomRect.right - zoomRect.left;
			scrollInfo.nPos = zoomRect.left;
			SetScrollInfo(hHorizontalScrollBar, SB_CTL, &scrollInfo, TRUE);
			ShowWindow(hHorizontalScrollBar, TRUE);
			ShowWindow(hVerticalScrollBar, TRUE);
		} else {

			ShowWindow(hVerticalScrollBar, FALSE);
			ShowWindow(hHorizontalScrollBar, FALSE);
		}
		duzrat();
		dub6 = zoomRatio.x * 6;
		threadWidthPixels[0] = dub6*TSIZ30;
		threadWidthPixels[1] = dub6*TSIZ40;
		threadWidthPixels[2] = dub6*TSIZ60;
		for (ind = 0; ind < 16; ind++) {

			if (chkMap(THRDS))
				nuStchSiz(ind, threadWidthPixels[threadSizeIndex[ind]]);
			else {
				if (chkMap(COL)) {

					if (ind == activeColor)
						nuStchSiz(ind, threadWidthPixels[threadSizeIndex[ind]]);
					else
						nuStchSiz(ind, 1);
				} else
					nuStchSiz(ind, 1);
			}
		}
		lineIndex = 0; DiplayedColorBitmap = 0;
		if (chkMap(ZUMED)) {

			lineIndex = 0;
			rstMap(LINED);
			rstMap(LININ);
			for (ind = 0; ind < colorChanges; ind++) {

				if (chkMap(HID)) {

					if (colorChangeTable[ind].colorIndex != activeColor) {

						scnt = colorChangeTable[ind + 1].stitchIndex - colorChangeTable[ind].stitchIndex;
						l_pstch = &stitchBuffer[colorChangeTable[ind].stitchIndex];
						for (ine = 0; ine < scnt; ine++) {

							if (l_pstch[ine].x >= zoomRect.left
								&&l_pstch[ine].x <= zoomRect.right
								&&l_pstch[ine].y >= zoomRect.bottom
								&&l_pstch[ine].y <= zoomRect.top) {

								setCol(colorChangeTable[ind].colorIndex);
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
				SelectObject(stitchWindowMemDC, userPen[colorChangeTable[ind].colorIndex]);
				scnt = colorChangeTable[ind + 1].stitchIndex - colorChangeTable[ind].stitchIndex;
				l_pstch = &stitchBuffer[colorChangeTable[ind].stitchIndex];
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
								Polyline(stitchWindowMemDC, linePoints, lineIndex);
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
										Polyline(stitchWindowMemDC, tlin, 2);
										goto rotlin;
									}
									//does the line intersect the bottom of the screen?
									tlng = off - slop*zoomRect.bottom;
									if (tlng >= zoomRect.left&&tlng <= zoomRect.right) {

										tlin[0].x = (l_pstch[ine - 1].x - zoomRect.left)*zoomRatio.x + 0.5;
										tlin[0].y = hi - (l_pstch[ine - 1].y - zoomRect.bottom)*zoomRatio.y + 0.5;
										tlin[1].x = (l_pstch[ine].x - zoomRect.left)*zoomRatio.x + 0.5;
										tlin[1].y = hi - (l_pstch[ine].y - zoomRect.bottom)*zoomRatio.y + 0.5;
										Polyline(stitchWindowMemDC, tlin, 2);
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
											Polyline(stitchWindowMemDC, tlin, 2);
										}
									}
								}
							rotlin:;

							}
						}
					}
				}
				if (lineIndex) {

					Polyline(stitchWindowMemDC, linePoints, lineIndex);
					linePoints[0].x = linePoints[lineIndex - 1].x;
					linePoints[0].y = linePoints[lineIndex - 1].y;
					lineIndex = 1;
				}
				if (wascol)
					setCol(colorChangeTable[ind].colorIndex);
			skip:;
			}
		} else {

			pwid = chkMap(HID);
			for (ind = 0; ind < colorChanges; ind++) {

				setCol(colorChangeTable[ind].colorIndex);
				scnt = colorChangeTable[ind + 1].stitchIndex - colorChangeTable[ind].stitchIndex;
				scnt = chkup(scnt, ind);
				if (!pwid || (pwid&&colorChangeTable[ind].colorIndex == activeColor))
					drwLin(colorChangeTable[ind].stitchIndex, scnt, userPen[colorChangeTable[ind].colorIndex]);
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
				ducros(stitchWindowMemDC);
			}
			selRct(&stitchRangeRect);
			nuselrct();
			setMap(SELSHO);
			dusel(stitchWindowMemDC);
		}
		if (zoomFactor < stitchBoxesThreshold) {

			clRmap(RMAPSIZ);
			SelectObject(stitchWindowMemDC, linePen);
			SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
			rint();
			if (chkMap(HID)) {

				for (ind = 0; ind < colorChanges; ind++) {

					if (colorChangeTable[ind].colorIndex == activeColor) {

						for (ine = colorChangeTable[ind].stitchIndex; ine < colorChangeTable[ind + 1].stitchIndex; ine++) {

							if (stitchBuffer[ine].x >= zoomRect.left&&stitchBuffer[ine].x <= zoomRect.right
								&&stitchBuffer[ine].y >= zoomRect.bottom&&stitchBuffer[ine].y <= zoomRect.top
								&&setRmap(stitchBuffer[ine]))

								stchbox(ine, stitchWindowMemDC);
						}
					}
				}
			} else {

				for (ind = 0; ind < header.stitchCount; ind++) {

					if (stitchBuffer[ind].x >= zoomRect.left&&stitchBuffer[ind].x <= zoomRect.right
						&&stitchBuffer[ind].y >= zoomRect.bottom&&stitchBuffer[ind].y <= zoomRect.top
						&&setRmap(stitchBuffer[ind]))

						stchbox(ind, stitchWindowMemDC);
				}
			}
			SetROP2(stitchWindowMemDC, R2_COPYPEN);
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
	if (hFormData)
		refrm();
	if (chkMap(GMRK))
		drwmrk(stitchWindowMemDC);
	if (chkMap(PRFACT))
		redraw(hPreferencesWindow);
	if (chkMap(SELBOX))
		ritnum(STR_NUMSEL, closestPointIndex);
	ritot(header.stitchCount);
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
		mov		edx, DiplayedColorBitmap
		xor		eax, eax
		bt		edx, ebx
		jnc		chk1
		dec		eax
		chk1 :
	}
#else
	return _bittest((long *)&DiplayedColorBitmap, ind) ? 0xffffffff : 0;
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
	for (ind = 0; ind < colorChanges; ind++) {

		tdub = (double)colorChangeTable[ind + 1].stitchIndex / header.stitchCount;
		tRct.bottom = tdub*ds->rcItem.bottom;
		FillRect(ds->hDC, &tRct, hUserColorBrush[colorChangeTable[ind].colorIndex]);
		tRct.top = tRct.bottom;
	}
	if (chkMap(SELBOX) || chkMap(GRPSEL)) {

		tdub = (double)closestPointIndex / header.stitchCount;
		lin[0].y = lin[1].y = tRct.bottom*tdub + 0.5;
		lin[0].x = tRct.left;
		lin[1].x = tRct.right;
		SelectObject(ds->hDC, crossPen);
		SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
		Polyline(ds->hDC, lin, 2);
		if (chkMap(GRPSEL)) {

			tdub = (double)groupStitchIndex / header.stitchCount;
			lin[0].y = lin[1].y = tRct.bottom*tdub + 0.5;
			lin[0].x = tRct.left;
			lin[1].x = tRct.right;
			Polyline(ds->hDC, lin, 2);
		}
		SetROP2(stitchWindowMemDC, R2_COPYPEN);
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

		ReadFile(thfil, (STRHED*)&sthed, sizeof(STRHED), &bytesRead, NULL);
		if (bytesRead == sizeof(STRHED)) {

			if ((sthed.headerType & 0xffffff) == 0x746872) {

				vervar = (sthed.headerType & 0xff000000) >> 24;
				switch (vervar) {

				case 0:

					if (header.hoopType == SMALHUP) {

						rsiz.x = iniFile.hoopSizeX = SHUPX;
						rsiz.y = iniFile.hoopSizeY = SHUPY;
					} else {

						rsiz.x = iniFile.hoopSizeX = LHUPX;
						rsiz.y = iniFile.hoopSizeY = LHUPY;
					}
					break;

				case 1:
				case 2:

					ReadFile(thfil, (STREX*)&thedx, sizeof(STREX), &bytesRead, NULL);
					if (bytesRead != sizeof(STREX))
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
				ReadFile(thfil, (fPOINTATTRIBUTE*)tstch, redsiz, &bytesRead, 0);
				if (redsiz == bytesRead) {

					SetFilePointer(thfil, 16, 0, FILE_CURRENT);
					ReadFile(thfil, (COLORREF*)&tbak, 4, &bytesRead, 0);
					ReadFile(thfil, (COLORREF*)tcol, 64, &bytesRead, 0);
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
					ReadFile(thfil, (FRMHEDO*)frmlstx, sthed.formCount * sizeof(FRMHEDO), &bytesRead, 0);
					if (bytesRead != sthed.pointCount * sizeof(FRMHEDO))
						goto bakskp;
					FillMemory(&bseq, 0, sizeof(FRMHED)*formIndex);
					for (ind = 0; ind < sthed.formCount; ind++) {

						SelectedForm = &flst[ind];
						MoveMemory(SelectedForm, &frmlstx[ind], sizeof(FRMHEDO));
					}
				} else {

					redsiz = sthed.formCount * sizeof(FRMHED);
					ReadFile(thfil, (FRMHED*)flst, redsiz, &bytesRead, 0);
					if (bytesRead != redsiz)
						goto bakskp;
				}
				redsiz = sthed.pointCount * sizeof(fPOINT);
				ReadFile(thfil, (fPOINT*)tflt, redsiz, &bytesRead, 0);
				if (bytesRead != redsiz)
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
					SetROP2(stitchWindowMemDC, R2_COPYPEN);
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
		FillRect(dc, &highRect, hBlackBrush);
	if (chkMap(DULO))
		FillRect(dc, &lowRect, hBlackBrush);
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

			DestroyWindow(hPreferencesWindow);
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
				SetScrollPos(hSpeedScrollBar, SB_CTL, MAXDELAY - delay, TRUE);
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
				SetScrollPos(hSpeedScrollBar, SB_CTL, MAXDELAY - delay, TRUE);
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
				SetScrollPos(hSpeedScrollBar, SB_CTL, MAXDELAY - delay, TRUE);
			} else
				pglft();
			return 1;

		case SB_PAGERIGHT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				delay -= SPEDPAG;
				if (delay < MINDELAY)
					delay = MINDELAY;
				setsped();
				SetScrollPos(hSpeedScrollBar, SB_CTL, MAXDELAY - delay, TRUE);
			} else
				pgrit();
			return 1;

		case SB_THUMBPOSITION:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				if ((HWND)lParam == hSpeedScrollBar) {

					ind = (unsigned)HIWORD(wParam);
					delay = MAXDELAY - ind;
					setsped();
					SetScrollPos(hSpeedScrollBar, SB_CTL, ind, TRUE);
				}
			} else {

				if ((HWND)lParam == hHorizontalScrollBar) {

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
		if (ds->hwndItem == hMainStitchWin&&ds->itemAction == ODA_DRAWENTIRE) {

			if (chkMap(TXTRED))
			{
				drwtxtr();
				return 1;
			}
			if (!chkMap(RUNPAT)) {

				if (!chkMap(HIDSTCH) && (hFile || chkMap(INIT) || formIndex || chkMap(SATPNT)) && !chkMap(BAKSHO))
					drwStch();
				else {

					FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
					duzrat();
					dugrid();
					if (chkMap(HIDSTCH)) {

						drwfrm();
						if (chkMap(SATPNT))
							satzum();
					}
					if (chkMap(PRFACT))
						redraw(hPreferencesWindow);
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
					stitchWindowDC,			// handle to destination DC
					0,			// x-coord of destination upper-left corner
					0,			// y-coord of destination upper-left corner
					stitchWindowClientRect.right,	// width of destination rectangle
					stitchWindowClientRect.bottom,	// height of destination rectangle
					stitchWindowMemDC,			// handle to source DC
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
		if (ds->hwndItem == hColorBar&&ds->itemAction == ODA_DRAWENTIRE) {

			if (header.stitchCount)
				dubar();
			else
				FillRect(ds->hDC, &ds->rcItem, (HBRUSH)(COLOR_WINDOW + 1));
			return 1;
		}
		if (ds->hwndItem == hButtonWin[HHID] && ds->itemAction == ODA_DRAWENTIRE) {

			ind = (buttonWidthX3 - pickColorMsgSize.cx) >> 1;
			if (chkMap(HID)) {

				FillRect(ds->hDC, &ds->rcItem, hUserColorBrush[activeColor]);
				SetBkColor(ds->hDC, userColor[activeColor]);
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

				if (ds->hwndItem == hTraceUp[ind]) {

					FillRect(ds->hDC, &ds->rcItem, hTraceBrush[ind]);
					upnum(ind);
					return 1;
				}
				if (ds->hwndItem == hTraceDown[ind]) {

					FillRect(ds->hDC, &ds->rcItem, hTraceBrush[ind]);
					dwnum(ind);
				}
				if (ds->hwndItem == hTraceControl[ind]) {

					durct(traceShift[ind], ds->rcItem);
					FillRect(ds->hDC, &middleRect, hTraceBrush[ind]);
					dublk(ds->hDC);
					return 1;
				}
				if (ds->hwndItem == hTraceSelect[ind]) {

					hTempBrush = hBlackBrush;
					strcpy_s(buf, stab[STR_OFF]);
					SetBkColor(ds->hDC, 0);
					SetTextColor(ds->hDC, traceRGB[ind]);
					if (chkMap(traceRGBFlag[ind])) {

						hTempBrush = hTraceBrush[ind];
						strcpy_s(buf, stab[STR_ON]);
						SetTextColor(ds->hDC, 0);
						SetBkColor(ds->hDC, traceRGB[ind]);
					}
					FillRect(ds->hDC, &ds->rcItem, hTempBrush);
					TextOut(ds->hDC, 1, 1, buf, strlen(buf));
					return 1;
				}
				if (ds->hwndItem == hTraceNumberInput) {

					FillRect(ds->hDC, &ds->rcItem, hTraceBrush[columnColor]);
					SetBkColor(ds->hDC, traceRGB[columnColor]);
					TextOut(ds->hDC, 1, 1, traceInputBuffer, strlen(traceInputBuffer));
					return 1;
				}
			}
		} else {

			for (ind = 0; ind < 16; ind++) {

				if (ds->hwndItem == hDefaultColorWin[ind]) {

					FillRect(ds->hDC, &ds->rcItem, hDefaultColorBrush[ind]);
					if (chkCol(ind)) {

						SetBkColor(ds->hDC, defCol[ind]);
						SetTextColor(ds->hDC, defTxt(ind));
						sprintf_s(buf, sizeof(buf), "%d", ind + 1);
						len = strlen(buf);
						GetTextExtentPoint32(ds->hDC, buf, len, &l_txSiz);
						TextOut(ds->hDC, (buttonWidth - l_txSiz.cx) >> 1, 0, buf, len);
					}
					return 1;
				}
				if (ds->hwndItem == hUserColorWin[ind]) {

					FillRect(ds->hDC, &ds->rcItem, hUserColorBrush[ind]);
					if (ind == activeColor) {

						SelectObject(ds->hDC, crossPen);
						SetROP2(stitchWindowMemDC, R2_NOTXORPEN);
						lin[0].x = lin[1].x = ds->rcItem.right >> 1;
						lin[0].y = 0;
						lin[1].y = ds->rcItem.bottom;
						Polyline(ds->hDC, lin, 2);
						lin[0].y = lin[1].y = ds->rcItem.bottom >> 1;
						lin[0].x = 0;
						lin[1].x = ds->rcItem.right;
						Polyline(ds->hDC, lin, 2);
						SetROP2(stitchWindowMemDC, R2_COPYPEN);
					}
					return 1;
				}
			}
		}
		if (chkMap(BAKSHO) && ds->itemAction == ODA_DRAWENTIRE) {

			if (chkMap(THUMSHO)) {

				for (ind = 0; ind < 4; ind++) {

					if (ds->hwndItem == hBackupViewer[ind] && ind < thumbnailDisplayCount) {

						ritbak(thumbnailsSelected[ind], ds);
						rthumnam(ind);
						return 1;
					}
				}
			} else {

				for (ind = 0; ind < OLDVER; ind++) {

					if (ds->hwndItem == hBackupViewer[ind]) {

						strcpy_s(nam, thrName);
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

		GetClientRect(p_hWnd, &mainWindowRect);
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
				if ((unsigned)(mainWindowRect.right - mainWindowRect.left) > ind) {

					ine = (screenSizePixels.cx - ind) >> 1;
					mainWindowRect.left = ine;
					mainWindowRect.right = screenSizePixels.cx - ine;
				}
				ind = (double)screenSizePixels.cy - 30;
				if ((unsigned)(mainWindowRect.bottom - mainWindowRect.top) > ind) {

					ine = (screenSizePixels.cy - ind) >> 1;
					mainWindowRect.top = ine;
					mainWindowRect.bottom = screenSizePixels.cy - ine;
				}
				if (ine)
					MoveWindow(p_hWnd, mainWindowRect.left, mainWindowRect.top, mainWindowRect.right - mainWindowRect.left, mainWindowRect.bottom - mainWindowRect.top, TRUE);
			} else {

				MoveWindow(p_hWnd, iniFile.initialWindowCoords.left, iniFile.initialWindowCoords.top, iniFile.initialWindowCoords.right - iniFile.initialWindowCoords.left, iniFile.initialWindowCoords.bottom - iniFile.initialWindowCoords.top, TRUE);
			}
			ShowWindow(p_hWnd, SW_SHOW);
			break;
		}
		GetClientRect(p_hWnd, &mainWindowRect);
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
		nearestCount = 0;
		setMap(RESTCH);
		if (chkMap(SELBOX))
			shft2box();
		if (chkMap(RUNPAT)) {

			FillRect(stitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
			if (chkMap(ZUMED))
				runPoint = groupStartStitch;
			else
				runPoint = 0;
		}
		return 1;

	case WM_MOVE:

		GetClientRect(p_hWnd, &mainWindowRect);
		if ((mainWindowRect.right - mainWindowRect.left) < 20) {

			mainWindowRect.left = 0;
			mainWindowRect.right = 300;
		}
		if ((mainWindowRect.bottom - mainWindowRect.top) < 20) {

			mainWindowRect.top = 0;
			mainWindowRect.bottom = 300;
		}
		movStch();
		if (chkMap(RUNPAT)) {

			duzrat();
			runPoint = 0;
			FillRect(stitchWindowDC, &stitchWindowClientRect, hBackgroundBrush);
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
		if (iniFile.initialWindowCoords.right) {

			hWnd = CreateWindow(
				"thred",
				"",
				WS_OVERLAPPEDWINDOW,
				iniFile.initialWindowCoords.left,
				iniFile.initialWindowCoords.right,
				iniFile.initialWindowCoords.right - iniFile.initialWindowCoords.left,
				iniFile.initialWindowCoords.bottom - iniFile.initialWindowCoords.top,
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
			GetClientRect(hWnd, &mainWindowRect);
			iniFile.initialWindowCoords.left = mainWindowRect.left;
			iniFile.initialWindowCoords.right = mainWindowRect.right;
			iniFile.initialWindowCoords.top = mainWindowRect.top;
			iniFile.initialWindowCoords.bottom = mainWindowRect.bottom;
		}
		init();
		if (chku(SAVMAX))
			ShowWindow(hWnd, SW_SHOWMAXIMIZED);
		else
			ShowWindow(hWnd, SW_SHOW);
		if (!*iniFile.designerName)
		{
			LoadString(hInst, IDS_UNAM, iniFile.designerName, 50);
			getdes();
		}
		while (GetMessage(&msg, NULL, 0, 0)) {

			setMap(SAVACT);
			if (!chkMsg())
				DispatchMessage(&msg);
			if (rstMap(FCHK))
				frmchkx();
			if (rstMap(RESTCH))
				redraw(hMainStitchWin);
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