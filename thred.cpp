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
void		frmx (POINT p_cpnt, HDC dc);
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
extern	void			delcon (unsigned p_cpnt);
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
extern	void			selsqr (POINT p_cpnt, HDC dc);
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
extern	void			stCor2px (fPOINTATTR stpnt, POINT* pxpnt);
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

extern	unsigned		ActivePointIndex;
extern	fRECTANGLE		AllItemsRect;
extern	double			BorderWidth;
extern	BSEQPNT			BSequence[BSEQLEN];
extern	float			ButtonholeCornerLength;
extern	fPOINT			ClipPoints[MAXCLPNTS];
extern	unsigned		ClosestFormToCursor;
extern	unsigned		ClosestVertexToCursor;
extern	unsigned		ClipPointIndex;
extern	SATCON*			CurrentFormConnections;
extern	fPOINT*			CurrentFormVertices;
extern	HWND			LabelWindow[LASTLIN];
extern	HWND			ValueWindow[LASTLIN];
extern	POINT			FormLines[MAXFRMLINS];
extern	unsigned		FormVertexIndex;
extern	fPOINT			FormMoveDelta;
extern	unsigned		FormIndex;
extern	FRMHED			FormList[MAXFORMS];
extern	POINT			FormOutlineRect[10];
extern	fPOINT			FormVertices[MAXFRMPNTS];
extern	TCHAR			HelpBuffer[HBUFSIZ];
extern	double			HorizontalRatio;
extern	fPOINT			InterleaveSequence[MAXSEQ];
extern	fPOINT			LowerLeftStitch;
extern	HWND			MsgWindow;
extern	unsigned		NewFormVertexCount;
extern	unsigned		OutputIndex;
extern	fPOINT			OSequence[OSEQLEN];
extern	float			PicotSpacing;
extern	long			PreferenceWindowWidth;
extern	unsigned		PreviousFormIndex;
extern	unsigned		PseudoRandomValue;
extern	POINT			RubberBandLine[3];
extern	unsigned		SatinConnectIndex;
extern	SATCON			SatinConnects[MAXSAC];
extern	unsigned		SelectedFormControlVertex;
extern	unsigned		SelectedFormCount;
extern	unsigned short	SelectedFormList[MAXFORMS];
extern	POINT			SelectedFormsLine[9];
extern	RECT			SelectedFormsRect;
extern	POINT			SelectedPointsLine[9];
extern	unsigned short	SideCount;
extern	double			SnapLength;
extern	double			SpiralWrap;
extern	TCHAR*			StringTable[STR_LEN];
extern	double			StarRatio;
extern	double			StitchSpacing;
extern	int				TextureHistoryIndex;
extern	TXHST			TextureHistory[16];
extern	TXTSCR			TextureScreen;
extern	int				TextureIndex;
extern	TXPNT			TexturePointsBuffer[MAXSEQ];
extern	double			VerticalRatio;
extern	unsigned short	WordParam;
extern	double			XYratio;

//select box
#define				NERCNT		4		//number of entries in the near array

//main variables
HINSTANCE		ThrEdInstance;			//main instance handle
HWND			ThrEdWindow;			//main window handle
MSG				Msg;					//main message loop message
RECT			ThredWindowRect;		//main window size
RECT			ColorBarRect;			//color bar rectangle
RECT			MinLenRect;				//minimum length rectangle
RECT			MaxLenRect;				//maximum length rectangle
unsigned		SmallestStitchIndex;	//pointer to the smallest stitch in the selected range
unsigned		LargestStitchIndex;		//pointer to the largest stitch in the selected range
unsigned		CurrentStitchIndex;		//pointer to the current selection for length search
HDC				ThredDC;				//main device context handle
HDC				StitchWindowMemDC;		//stitch window memory device context
HDC				StitchWindowDC;			//stitch window device context
HDC				BitmapDC;				//bitmap device context
HDC				ColorBarDC;				//color bar device context
HBITMAP			StitchWindowBmp;		//bitmap for the memory stitch device context
POINT			ThredWindowOrigin;		//offset origin of the main window
POINT			StitchWindowOrigin;		//offset origin of the stitch window
SIZE			TextSize;				//used for measuring sizes of text items
SIZE			ScreenSizePixels;		//screen size in pixels
SIZE			ScreenSizeMM;			//screen size in millimeters
dRECTANGLE		ZoomRect;				//zoom rectangle
RECT			StitchWindowAbsRect;	//stitch window size,absolute
RECT			StitchWindowClientRect;	//stitch window size,client
DRAWITEMSTRUCT*	DrawItem;				//for owner-draw windows
double			ZoomFactor = 1;			//zoom factor
POINT			UnzoomedRect;			//size of the unzoomed stitch window
POINT			StitchCoordinatesPixels;	//converted from stitch to pixel
POINT			NearestPixel[NERCNT];	//selected points
POINT			BoxCoordinate;			//single select box point
double			DistanceToClick;		//distance of closest point to a mouse click
unsigned		ClosestPointIndex;		//index of closest point
unsigned		ClosestPointIndexClone;	//copy of index of closest point
unsigned		GroupStitchIndex;		//last point selected in group
unsigned		GroupStartStitch;		//lower end of selected stitches
unsigned		GroupEndStitch;			//higher end of selected stitches
unsigned		PrevGroupStartStitch;	//lower end of previous selection
unsigned		PrevGroupEndStitch;		//higher end of previous selection
TCHAR			StitchEntryBuffer[5];	//stitch number entered by user
TCHAR			SideWindowEntryBuffer[11];	//side window number for entering form data sheet numbers
unsigned		BufferIndex = 0;		//pointer to the next number to be entered
unsigned		BufferDigitCount;		//number of decimal digits in the number of stitches
POINT*			LinePoints;				//for drawing lines on the screen
unsigned		LineIndex;				//line index for display routine
double			StitchWindowAspectRatio;	//aspect ratio of the stitch window
double			MinStitchLength	  = MINSIZ*PFAFGRAN;	//minimum stitch size
double			UserStitchLength  = USESIZ*PFAFGRAN;	//user selected stitch size
double			SmallStitchLength = SMALSIZ*PFAFGRAN;	//user can remove stitches smaller than this
unsigned		MarkedStitchMap[RMAPSIZ];	//bitmap to tell when stitches have been marked
TCHAR*			PcdClipFormat = "PMust_Format";
TCHAR*			ThrEdClipFormat = "threditor";
CLPSTCH*		ClipStitchData;			//for pcs clipboard data
FORMCLIP*		ClipFormHeader;			//for thred form clipboard data
FORMSCLIP*		ClipFormsHeader;		//multiple form clipboard header
FORMVERTEXCLIP*	ClipFormVerticesData;	//form points clipboard header
void*			ClipPointer;			//for memory allocation for clipboard data
void*			ThrEdClipPointer;		//for memory allocation for thred format clipboard data
FLSIZ			ClipRectSize;			//clipboard rectangle size
fRECTANGLE		ClipRect;				//clipboard rectangle
unsigned		ClipStitchCount;		//number of stitchs extracted from clipboard
POINT			ClipOrigin;				//origin of clipboard box in stitch coordinates
HGLOBAL			ClipMemory;				//handle to the clipboard memory
SIZE			SelectBoxSize;			//size of the select box
POINT			SelectBoxOffset;		//offset of the spot the user selected from the lower left of the select box
dPOINT			ZoomRatio;				//zoom ratio used to draw stitch window
double			RotationAngle;			//rotation angle
double			RotationHandleAngle;	//angle of the rotation handle
fRECTANGLE		RotationRect;			//rotation rectangle
dPOINT			BmpStitchRatio;			//bitmap to stitch hoop ratios
dPOINT			StitchBmpRatio;			//stitch hoop to bitmap ratios
RECT			BitmapSrcRect;			//bitmap source rectangle for zoomed view
RECT			BitmapDstRect;			//stitch window destination rectangle for zooomed view
unsigned		BitmapWidth;			//bitmap width
unsigned		BitmapHeight;			//bitmap height
dPOINT			BitmapSizeinStitches;	//bitmap end points in stitch points
unsigned*		MonoBitmapData;			//monochrome bitmap data
unsigned		BitmapColor = BITCOL;	//bitmap color
TCHAR			MsgBuffer[MSGSIZ];		//for user messages
unsigned		MsgIndex;				//pointer to the message buffer
double			ShowStitchThreshold = SHOPNTS;	//show stitch grid below this zoom level
double			ThreadSize30 = TSIZ30;	//#30 thread size
double			ThreadSize40 = TSIZ40;	//#40 thread size
double			ThreadSize60 = TSIZ60;	//#40 thread size
unsigned		RunPoint;				//point for animating stitchout
unsigned		StitchesPerFrame;		//number of stitches to draw in each frame
int				MovieTimeStep;			//time delay for stitchout
double			StitchBoxesThreshold = STCHBOX;//threshold for drawing stitch boxes
//WARNING the size of the following array must be changed if the maximum movie speed is changed
POINT			MovieLine[100];			//line for movie stitch draw
RECT			MsgRect;				//rectangle containing the text message
void*			UndoBuffer[16];			//backup data
unsigned		UndoBufferWriteIndex = 0;	//undo storage pointer
unsigned		UndoBufferReadIndex = 0;	//undo retrieval pointers
unsigned		UnderlayColor = 15;		//underlay color
unsigned		LastKeyCode = 0xffff;	//last key code
unsigned		FormMenuChoice = 0;		//data type for form data form numerical entry
dPOINT			ZoomMarkPoint;			//stitch coordinates of the zoom mark
unsigned		PreferenceIndex = 0;	//index to the active preference window
unsigned		LRUMenuId[] = { FM_ONAM0,FM_ONAM1,FM_ONAM2,FM_ONAM3 };	//recently used file menu ID's
TCHAR			VersionNames[OLDVER][_MAX_PATH];	//temporary storage for old file version names
unsigned		FileVersionIndex;		//points to old version to be read
unsigned		ActiveLayer = 0;		//active layer
unsigned		LayerIndex;				//active layer code
long			ClipFormsCount;			//number of forms the on the clipboard
unsigned*		FormIndices;			//array of form indices for delete multiple forms
POINT			StitchArrow[3];			//arrow for selected stitch
RANGE			SelectedRange;			//first and last stitch for min/max stitch select
unsigned		FileNameOrder[50];		//file name order table
unsigned char	FilenameEncoded[128];	//file name encoding
unsigned char	FilenameDecode[256];	//file name decode
TCHAR			DesignerName[50];		//file designer name in clear
HWND			FirstWin;				//first window not destroyed for exiting enumerate loop
RANGE			SelectedFormsRange;		//range of selected forms
unsigned		TmpFormIndex;			//saved form index
unsigned char	CursorMask[128];		//cursor and mask
double			ZoomMin;				//minimum allowed zoom value
fRECTANGLE		CheckHoopRect;			//for checking the hoop size
BALSTCH*		BalaradStitch;			//balarad stitch pointer
fPOINT			BalaradOffset;			//balarad offset
unsigned		ClipTypeMap = MCLPF | MVCLPF | MHCLPF | MANGCLPF; //for checking if a fill is a clipboard fill
fPOINTATTR*		RotatedStitches;		//rotated stitches for rotate and save
unsigned		SideWindowLocation;		//side message window location
POINT			SideWindowSize;			//size of the side message window
TCHAR**			SideWindowsStrings;		//string array displayed in sidmsg
TCHAR			ColorFileName[_MAX_PATH];	//.thw file name
TCHAR			RGBFileName[_MAX_PATH];	//.rgb file name
dPOINT			CellSize;				//size of an MarkedStitchMap cell for drawing stitch boxes
unsigned		DraggedColor;			//color being dragged
FORMVERTICES	SelectedFormVertices;	//selected form vertices
fRECTANGLE		SelectedVerticesRect;	//rectangle enclosing selected form verticess
RECT			SelectedPixelsRect;		//display form vertex select rectangle
POINT*			FormVerticesAsLine;		//form vertex clipboard paste into form line
unsigned		LastFormSelected;		//end point of selected range of forms

#if	PESACT
unsigned char*	PEScolors;				//pes colors
TCHAR*			PESdata;				//pes card data buffer
fPOINT			PESstichCenterOffset;	//offset for writing pes files
PESTCH*			PESstitches;			//pes stitch buffer
unsigned char	PESequivColors[16];		//pes equivalent colors
unsigned char*	PESstitch;				//pes stitches
unsigned		PEScolorIndex;			//pes color index
#endif

POINT			EndPointCross;			//point to draw at the end cross for form select points
#if	 __UseASM__
unsigned		fsizeof;				//size of form header divided by 4
#endif
HDC				TraceDC;				//trace device context
HBITMAP			TraceBitmap;			//trace bitmap
unsigned*		TraceBitmapData;		//trace bitmap data
POINT			CurrentTracePoint;		//current point being traced
unsigned		TraceDataSize;			//size of the trace bitmap in double words
unsigned*		TracedPixels;			//bitmap of selected trace pixels
unsigned*		TracedEdges;			//detected edges of trace areas
TRCPNT*			tracedPoints;			//collection of traced points
TRCPNT*			DecimatedLine;			//differenced collection of traced points
unsigned		TraceDirection;			//trace direction
unsigned		InitialDirection;		//initial trace direction
TRCPNT			TraceDiff0;				//difference from one trace pixel to the next
TRCPNT			TraceDiff1;				//difference from one trace pixel to the next
COLORREF		UpPixelColor;			//color of the up reference pixel
COLORREF		DownPixelColor;			//color of the down reference pixel
COLORREF		InvertUpColor;			//complement color of the up reference pixel
COLORREF		InvertDownColor;		//complement color of the down reference pixel
POINT			TraceMsgPoint;			//message point for trace parsing
RECT			TraceHighMaskRect;		//high trace mask rectangle
RECT			TraceMiddleMaskRect;	//middle trace mask rectangle
RECT			TraceLowMaskRect;		//low trace mask rectangle
unsigned		HighColors[3];			//separated upper reference colors
unsigned		PixelColors[3];			//separated pixel reference colors
unsigned		LowColors[3];			//separated lower reference colors
unsigned		TraceRGBFlag[]	= { TRCRED,TRCGRN,TRCBLU };	//trace bits
unsigned		TraceRGBMask[]	= { REDMSK,GRNMSK,BLUMSK };	//trace masks
unsigned		TraceRGB[]		= { BLUCOL,GRNCOL,REDCOL };	//trace colors
unsigned		TraceAdjacentColors[9];	//separated colors for adjacent pixels
unsigned*		DifferenceBitmap;		//difference bitmap
TCHAR			TraceInputBuffer[4];	//for user input color numbers
unsigned		ColumnColor;			//trace color column
POINT			BitmapPoint;			//a point on the bitmap

//cursors
HCURSOR 		ArrowCursor;			//arrow
HCURSOR 		CrossCursor;			//cross
HCURSOR 		FormCursor;				//form
HCURSOR 		DLineCursor;			//dline
HCURSOR			NeedleUpCursor;			//upright needle
HCURSOR			NeedleLeftUpCursor;		//left up needle 
HCURSOR			NeedleLeftDownCursor;	//left down needle 
HCURSOR			NeedleRightUpCursor;	//right up needle 
HCURSOR			NeedleRightDownCursor;	//right down needle 
CURSORMASK		CursorMasks;			//cursor mask structure

HPEN			LinePen;				//line pen for stitch move lines			
HPEN			BoxPen[4];				//box pens
HPEN			UserPen[16];			//user color pens
HPEN			CrossPen;				//pen for crosses in color windows
HPEN			GroupSelectPen;			//pen for group select
HPEN			GridPen;				//pen for stitch grid
HPEN			BackgroundPen;			//background color pen
HPEN			BitmapPen;				//bitmap pen
HPEN			FormPen;				//form pen
HPEN			LayerPen[6];			//layer pens
HPEN			FormPen3px;				//three-pixel form pen
HPEN			FormSelectedPen;		//form select pen
HPEN			ZoomMarkPen;			//zoom mark pen
HPEN			MultiFormPen;			//multiple selected forms pen
HPEN			SelectAllPen;			//pen for drawing large boxes
HPEN			KnotPen;				//knot pen
HPEN			BlackPen;				//black pen

unsigned		BackgroundPenWidth;		//width of the background pen

//brushes
HBRUSH			BackgroundBrush;		//background color brush
HBRUSH			DefaultColorBrush[16];	//default color brushes
HBRUSH			UserColorBrush[16];		//user color brushes

//clipboard handle
unsigned		Clip = 0;				//pcs format
unsigned		ThrEdClip = 0;			//thred format

//for the choose color dialog box
CHOOSECOLOR		ColorStruct;
COLORREF		CustomColor[16];
//for the background color dialog box
CHOOSECOLOR		BackgroundColorStruct;
COLORREF		CustomBackgroundColor[16];
//for the bitmap color dialog box
CHOOSECOLOR		BitMapColorStruct;
COLORREF		BitmapBackgroundColors[16];

TCHAR			ThreadSize[16][2];		//user selected thread sizes
unsigned		ThreadSizePixels[16];	//thread sizes in pixels
unsigned		ThreadSizeIndex[16];	//thread size indices

HMENU			MainMenu;				//main menu
HMENU			FillMenu;				//fill submenu
HMENU			FileMenu;				//file submenu
HMENU			BorderFillMenu;			//border fill submenu
HMENU			ViewMenu;				//view submenu
HMENU			ViewSetMenu;			//view/set
HMENU			EditMenu;				//edit submenu
HWND			DefaultColorWin[16];	//default color windows
HWND			UserColorWin[16];		//user color windows
HWND			ThreadSizeWin[16];		//thread size windows
HWND			ChangeThreadSizeWin[3];	//thread size change windows
HWND			MainStitchWin;			//stitch window
HWND			VerticalScrollBar;		//vertical scroll bar
HWND			HorizontalScrollBar;	//horizontal scroll bar
HWND			ButtonWin[9];			//button windows
HWND			TraceStepWin;			//trace stepSize window
HWND			ColorBar;				//color bar
HWND			OKButton;				//ok button
HWND			DiscardButton;			//discard button
HWND			CancelButton;			//cancel button
HWND			GeneralNumberInputBox;	//general number input box
HWND			SpeedScrollBar;			//speed scroll bar for movie
HWND			SideMessageWindow = 0;	//main side message window
HWND			SideWindow[16];			//side message windows
HWND			PreferencesWindow;		//preferences window
HWND			FormDataSheet = 0;		//form data sheet
HWND			DeleteStitchesDialog;	//asking user if they want to delete the sitches associated with a form
HWND			BackupViewer[OLDVER];	//handles of multiple file viewing windows
HWND			TraceUpWindow[3];		//trace up number windows
HWND			TraceDownWindow[3];		//trace down number windows

HWND			TraceControlWindow[3];	//trace control windows
HWND			TraceSelectWindow[3];	//trace select windows
unsigned		TraceShift[] = { 0,8,16 };	//trace shift values
HBRUSH			TraceBrush[3];			//red,green,and blue brushes
HWND			TraceNumberInput;		//trace number input window
HBRUSH			BlackBrush;				//black brush
HBRUSH			TempBrush;				//temporary brush

COLORREF		UserColor[16];			//user colors
COLORREF		BackgroundColor;		//stich window background
COLORREF		BoxColor[] = { 0x404040,0x408040,0x804040,0x404080 };
unsigned		ActiveColor = 0;		//active color selector
unsigned		ButtonHeight;			//button height
unsigned		ButtonWidth;			//button width
unsigned		ButtonWidthX3;			//button width times 3
unsigned		NumeralWidth;			//width of 0
int				ThreadWidthPixels[3];	//thread sizes in pixels
unsigned		DisplayedColorBitmap;	//color bitmap for recording colors on screen
double			GapToNearest[NERCNT];	//distances of the closest points
										//to a mouse click
long			NearestPoint[NERCNT];	//indices of the closest points
unsigned		NearestCount;			//number of boxes selected
//ToDo - convert SearchLine to calloc'd local variable?
POINT			SearchLine[MAXSEQ];		//stitch select line 
unsigned		SearchLineIndex = 0;	//pointer for drawing stitch select lines
fRECTANGLE		StitchRangeRect;		//stitch range rectangle
fPOINT			StitchRangeSize;		//form check ranges
fPOINT			SelectedFormsSize;		//size of multiple select rectangle
unsigned		MoveAnchor;				//for resequencing stitches
double			RotateAngle;			//angle for pixel rotate
SIZE			PickColorMsgSize;		//size of the pick color message
POINT			InsertSize;				//size of file insert window
fPOINT			InsertCenter;			//center point in inserted file
unsigned		NumericCode;			//keyboard numerical input
double			LowestAngle;			//low angle for angle from mark
double			OriginalAngle;			//original angle for angle from mark
double			HighestAngle;			//hi angle for angle from mark
unsigned		Knots[MAXKNOTS];		//pointers to knots
unsigned		KnotCount;				//number of knots in the design
unsigned		KnotAttribute;			//knot stitch attribute
fPOINT			KnotStep;				//knot stepSize
TCHAR			KnotAtStartOrder[] = { 2,3,1,4,0 };		//knot spacings
TCHAR			KnotAtEndOrder[] = { -2,-3,-1,-4,0 };	//reverse knot spacings
TCHAR			KnotAtLastOrder[] = { 0,-4,-1,-3,-2 };	//reverse knot spacings
fRECTANGLE		ClipRectAdjusted;		//rectangle for adjust range ends for clipboard fills
TCHAR*			BalaradFileName;		//used in the balarad interface
HANDLE			BalaradFile;			//balarad file handle

//graphics variables		
double			AspectRatio = (double)LHUPX / LHUPY;	//aspect ratio of the stich window
SCROLLINFO		ScrollInfo;				//scroll bar i/o structure
POINT			StitchWindowSize;		//size of the stich window in pixels
fPOINT			SelectedPoint;			//for converting stitch coordinates
										// to metric cordinates (mm)
fPOINT			ZoomBoxOrigin;			//zoom box origin

TCHAR*	LayerText[] = {
	"0",
	"1",
	"2",
	"3",
	"4"
};

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

long				BoxOffset[4];

unsigned			TextColorMap = 0xaf;	//bitmap for text color number colors
unsigned			VerticalIndex;		//vertical index, calculated from mouse click
unsigned			ThreadSizeSelected;	//thread selected for size change

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
	MsgBuffer,
	13,
};

const TCHAR		AllFilter[_MAX_PATH + 1] = "Thredworks (THR)\0*.thr\0Pfaff (PCS)\0*.pcs\0Tajima (DST)\0*.dst\0";
const TCHAR		BmpFilter[_MAX_PATH + 1] = "Microsoft (BMP)\0*.bmp\0";
TCHAR			CustomFilter[_MAX_PATH + 1] = "Thredworks (THR)\0*.thr\0";
TCHAR			WorkingFileName[_MAX_PATH + 1] = { 0 };
TCHAR			ThrName[_MAX_PATH + 1];
TCHAR			AuxName[_MAX_PATH + 1];
TCHAR			GeName[_MAX_PATH + 1];
TCHAR			DefaultDirectory[_MAX_PATH + 1] = "c:\\";
TCHAR			DefaultBMPDirectory[_MAX_PATH + 1] = "c:\\";
TCHAR			BalaradName0[_MAX_PATH + 1] = { 0 };	//balarad semaphore file
TCHAR			BalaradName1[_MAX_PATH + 1] = { 0 };	//balarad data file
TCHAR			BalaradName2[_MAX_PATH + 1];
TCHAR			SearchName[_MAX_PATH + 1];
TCHAR			HomeDirectory[_MAX_PATH + 1];			//directory from which thred was executed
PCSTCH*			PCSStitchBuffer;
HANDLE			FileHandle = 0;
HANDLE			PCSFileHandle = 0;
HANDLE			IniFileHandle = 0;
HANDLE			InsertedFileHandle;			//insert file handle
HANDLE			BitmapFileHandle;			//bitmap handle
unsigned		FileSize;					//size of file
unsigned long	BytesRead;					//bytes actually read from file
unsigned		ColorChanges;				//number of color changes
TCHAR			IniFileName[_MAX_PATH];		//.ini file name
TCHAR			PCSBMPFileName[16];			//bitmap file name from pcs file
TCHAR			UserBMPFileName[_MAX_PATH];	//bitmap file name from user load
OPENFILENAME	OpenFileName = {

	sizeof(OPENFILENAME),	//lStructsize
	ThrEdWindow,			//hwndOwner 
	ThrEdInstance,			//hInstance 
	AllFilter,				//lpstrFilter 
	CustomFilter,			//lpstrCustomFilter 
	_MAX_PATH,				//nMaxCustFilter 
	0,						//nFilterIndex 
	WorkingFileName,		//lpstrFile 
	_MAX_PATH,				//nMaxFile 
	0,						//lpstrFileTitle 
	0,						//nMaxFileTitle 
	DefaultDirectory,		//lpstrInitialDir
	0,						//lpstrTitle
	OFN_OVERWRITEPROMPT,	//Flags
	0,						//nFileOffset
	0,						//nFileExtension 
	"thr",					//lpstrDefExt 
	0,						//lCustData 
	0,						//lpfnHook
	0,						//lpTemplateName
};
TCHAR*				ThumbnailNames;			//pointer to storage for thumbnail names
TCHAR**				Thumbnails;				//pointers to the thumbnail names
TCHAR*				ThumbnailsSelected[4];	//pointers to thumbnails selected for display
unsigned			ThumbnailCount;			//number of thumbnail file names
unsigned			ThumbnailDisplayCount;	//number of thumbnail file names selected for display
unsigned			ThumbnailIndex;			//index into the thumbnail filname table
TCHAR				ThumbnailSearchString[32];	//storage for the thumnail search string
TCHAR				InsertedFileName[_MAX_PATH] = { 0 };	//insert file name
unsigned			InsertedVertexIndex;	//saved float pointer for inserting files
unsigned			InsertedFormIndex;		//saved form pointer for inserting files
unsigned			InsertedStitchCount;	//saved stitch pointer for inserting files

OPENFILENAME OpenBitmapName = {
	sizeof(OPENFILENAME),	//lStructsize
	ThrEdWindow,			//hwndOwner 
	ThrEdInstance,			//hInstance 
	BmpFilter,				//lpstrFilter 
	CustomFilter,			//lpstrCustomFilter 
	_MAX_PATH,				//nMaxCustFilter 
	0,						//nFilterIndex 
	UserBMPFileName,		//lpstrFile 
	_MAX_PATH,				//nMaxFile 
	0,						//lpstrFileTitle 
	0,						//nMaxFileTitle 
	DefaultBMPDirectory,	//lpstrInitialDir 
	0,						//lpstrTitle
	OFN_OVERWRITEPROMPT,	//Flags
	0,						//nFileOffset
	0,						//nFileExtension 
	"bmp",					//lpstrDefExt 
	0,						//lCustData 
	0,						//lpfnHook
	0,						//lpTemplateName
};

BITMAPFILEHEADER	BitmapFileHeader;	//bitmap file header
BITMAPV4HEADER		BitmapFileHeaderV4;	//bitmap version4 file header
BITMAPINFO			BitmapInfo;			//bitmap info
BITMAPINFOHEADER	BitmapInfoHeader;	//bitmap info header

COLORREF DefaultUserColors[] = {

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

COLORREF DefaultCustomColors[] = {

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

COLORREF DefaultCustomBackgroundColors[] = {

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

COLORREF DefaultBitmapBackgroundColors[] = {

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

POINT		MoveLine0[2];				//move point line
POINT		MoveLine1[2];				//move point line
POINT		InsertLine[3];				//the insert line
POINT		ZoomBoxLine[5];				//the zoom box
POINT		ClipInsertBoxLine[5];		//for displaying clipboard insert rectangle
POINT		RotateBoxOutline[5];		//for drawing the rotate rectangle
POINT		RotateBoxCrossVertLine[2];	//vertical part of the rotate cross
POINT		RotateBoxCrossHorzLine[2];	//horizontal part of the rotate cross
POINT		RotateBoxToCursorLine[2];	//line from the cursor to the center of the rotate cross
POINT		StretchBoxLine[5];			//stretch and expand

COLCHNG		ColorChangeTable[MAXCHNG];
PCSHEADER	PCSHeader;					//pcs file header
STREX		ExtendedHeader;				//thred file header extension
INIFILE		IniFile;					//initialization file
dPOINT		RotationCenter;				//center of stitch rotation
POINT		RotationCenterPixels;		//center of pixel rotation

unsigned	FormFirstStitchIndex;		//points to the first stitch in a form

typedef struct _dstdat {

	TCHAR	cor;
	TCHAR	val;
}DSTDAT;

#define XCOR 0
#define YCOR 1

DSTDAT	DSTValues[] = {

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

unsigned RightDownCursorStart[] = { 0x8,0x18,0x3c,0x7c };
unsigned RightDownCursorFinish[] = { 0x48000000,0x50000000,0xa0000000,0xc0000000,0x80000000 };
unsigned RightUpCursorStart[] = { 0x80000000,0xa0000000,0x48000000,0x24000000,0x12000000 };
unsigned RightUpCursorFinish[] = { 0x3c,0x1c,0xc };
unsigned LeftDownCursorStart[] = { 0x20000000,0x60000000,0xf0000000,0xf8000000 };
unsigned LeftDownCursorFinish[] = { 0x44,0x24,0x14,0x6,0x4 };
unsigned LeftUpCursorStart[] = { 0x2,0x6,0x14,0x24,0x44 };
unsigned LeftUpCursorEnd[] = { 0x70000000,0x30000000,0x20000000 };

unsigned Xdst[] = {

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

unsigned Ydst[] = {

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

unsigned PESColorTranslate[] = {

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

DSTREC*		DSTRecords;			//pointer to destination stitch records
unsigned	DSTRecordCount;		//number of destination stitch records
POINT		DSTPositiveOffset;	//plus offset written into the destination file header
POINT		DSTNegativeOffset;	//minus offset written into the destination file header

//bitmap for upper case characters
unsigned UpperCaseMap[] = {

	//fedcba9876543210fedcba9876543210
0,			//00000000000000000000000000000000
			//?>=</:9876543210 /.-,+*)('&%$#"!
0,			//00000000000000000000000000000000
			//_^]\[ZYXWVUTSRQPONMLKJIHGFEDCBA@
0x07fffffe,	//00000111111111111111111111111110
			// ~}|{zyxwvutsrqponmlkjihgfedcba'
0,			//00000000000000000000000000000000
};

unsigned			FlagMap[MAPLEN];			//bitmap
unsigned			UserFlagMap = 0;	//for storage of persistent binary variables set by the user
// ToDo - StitchBuffer and tmpStitichBuffer have been allocated at the same place. Should they be separate?
fPOINTATTR			StitchBuffer[MAXPCS];		//main stitch buffer
fPOINTATTR			ClipBuffer[MAXSEQ];			//for temporary copy of imported clipboard data
FRMHED*				SelectedForm;				//pointer to selected form
unsigned			FillTypes[] =				//fill type array for side window display
{ 0,VRTF,HORF,ANGF,SATF,CLPF,CONTF,VCLPF,HCLPF,ANGCLPF,FTHF,TXVRTF,TXHORF,TXANGF };
//edge fill type array for side window display
unsigned			EdgeFillTypes[] = { 0,EDGELINE,EDGEBEAN,EDGECLIP,EDGEANGSAT,EDGEAPPL,EDGEPROPSAT,EDGEBHOL,EDGEPICOT,EDGEDOUBLE,EDGELCHAIN,EDGEOCHAIN,EDGECLIPX };
//feather fill types
unsigned			FeatherFillTypes[] = { FTHSIN,FTHSIN2,FTHLIN,FTHPSG,FTHRMP,FTHFAZ };

//bitmap functions
unsigned setMap(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset FlagMap
		mov		ecx, bit
		bts[ebx], ecx
		jnc		short setx
		dec		eax
		setx :
	}
#else
	return _bittestandset((long *)FlagMap, bit) ? 0xffffffff : 0;
#endif
}

void clrMap(unsigned mapLength) {

#if  __UseASM__
	_asm {

		mov		edi, offset FlagMap
		mov		ecx, mapLength
		xor		eax, eax
		rep		stosd
	}
#else
	memset(FlagMap, 0, sizeof(*FlagMap) * mapLength);
#endif
}

unsigned rstMap(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset FlagMap
		mov		ecx, bit
		btr[ebx], ecx
		jnc		short rstx
		dec		eax
		rstx :
	}
#else
	return _bittestandreset((long *)FlagMap, bit) ? 0xffffffff : 0;
#endif
}

unsigned toglMap(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset FlagMap
		mov		edx, bit
		btc[ebx], edx
		jnc		short toglx
		dec		eax
		toglx :
	}
#else
	return _bittestandcomplement((long *)FlagMap, bit) ? 0xffffffff : 0;
#endif
}

unsigned chkMap(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset FlagMap
		mov		edx, bit
		bt[ebx], edx
		jnc		short chkx
		dec		eax
		chkx :
	}
#else
	return _bittest((long *)FlagMap, bit) ? 0xffffffff : 0;
#endif
}

void cpymap(unsigned destinationBit, unsigned sourceBit) {

#if  __UseASM__
	_asm {

		mov		ebx, offset FlagMap
		mov		eax, destinationBit
		btr[ebx], eax
		mov		edx, sourceBit
		bt[ebx], edx
		jnc		short cpx
		bts[ebx], eax
		cpx :
	}
#else
	// check translation
	if (_bittest((long *)FlagMap, sourceBit)) {
		_bittestandset((long *)FlagMap, destinationBit);
	} else {
		_bittestandreset((long *)FlagMap, destinationBit);
	}
#endif
}

//user bitmap functions
unsigned setu(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset UserFlagMap
		mov		ecx, bit
		bts[ebx], ecx
		jnc		short setx
		dec		eax
		setx :
	}
#else
	return _bittestandset((long *)&UserFlagMap, bit) ? 0xffffffff : 0;
#endif
}

unsigned rstu(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset UserFlagMap
		mov		ecx, bit
		btr[ebx], ecx
		jnc		short rstx
		dec		eax
		rstx :
	}
#else
	return _bittestandreset((long *)&UserFlagMap, bit) ? 0xffffffff : 0;
#endif
}

unsigned chku(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset UserFlagMap
		mov		edx, bit
		bt[ebx], edx
		jnc		short chkx
		dec		eax
		chkx :
	}
#else
	return _bittest((long *)&UserFlagMap, bit) ? 0xffffffff : 0;
#endif
}

unsigned toglu(unsigned bit) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ebx, offset UserFlagMap
		mov		edx, bit
		btc[ebx], edx
		jnc		short toglx
		dec		eax
		toglx :
	}
#else
	return _bittestandcomplement((long *)&UserFlagMap, bit) ? 0xffffffff : 0;
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
		SetWindowText(hwnd, IniFile.designerName);
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
			GetWindowText(hwnd, IniFile.designerName, 50);
			EndDialog(hwndlg, 0);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRED], IniFile.designerName);
			SetWindowText(ThrEdWindow, MsgBuffer);
			return TRUE;
		}
	}
	return 0;
}

void getdes()
{
	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DESNAM), ThrEdWindow, (DLGPROC)dnamproc);
}

BOOL isclp(unsigned find) {

	if ((1 << FormList[find].fillType)&ClipTypeMap)
		return 1;
	return 0;
}

BOOL isclpx(unsigned find) {

	if (isclp(find) && FormList[find].lengthOrCount.clipCount)
		return 1;
	return 0;
}

BOOL isfclp() {

	if (isclp(ClosestFormToCursor) && FormList[ClosestFormToCursor].fillType != CLPF)
		return 1;
	return 0;
}

BOOL iseclp(unsigned find) {

	if (FormList[find].edgeType == EDGECLIP || FormList[find].edgeType == EDGEPICOT || FormList[find].edgeType == EDGECLIPX)
		return 1;
	return 0;
}

BOOL iseclpx(unsigned find) {

	if (iseclp(find) && FormList[find].clipEntries)
		return 1;
	return 0;
}

double stlen(unsigned iStitch) {

	return hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
}

void undat() {

	if (FormDataSheet) {

		DestroyWindow(FormDataSheet);
		FormDataSheet = 0;
	}
}

void qchk() {

	if (chku(MARQ)) {

		CheckMenuItem(MainMenu, ID_MARKESC, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_MARKQ, MF_UNCHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_MARKESC, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_MARKQ, MF_CHECKED);
	}
	setMap(DUMEN);
}

void nedmen() {

	if (chku(NEDOF)) {

		CheckMenuItem(MainMenu, ID_SETNEDL, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_RSTNEDL, MF_CHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_SETNEDL, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_RSTNEDL, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void knotmen() {

	if (chku(KNOTOF)) {

		CheckMenuItem(MainMenu, ID_KNOTON, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_KNOTOF, MF_CHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_KNOTON, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_KNOTOF, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void bsavmen() {

	if (chku(BSAVOF)) {

		CheckMenuItem(MainMenu, ID_BSAVON, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_BSAVOF, MF_CHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_BSAVON, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_BSAVOF, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void linbmen() {

	if (chku(LINSPAC)) {

		CheckMenuItem(MainMenu, ID_LINBEXACT, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_LINBEVEN, MF_CHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_LINBEXACT, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_LINBEVEN, MF_UNCHECKED);
	}
	setMap(DUMEN);
}

void wrnmen()
{
	unsigned code;

	code = MF_CHECKED;
	if (chku(WRNOF))
		code = MF_UNCHECKED;
	CheckMenuItem(MainMenu, ID_WARNOF, code);
}

int datcod[] = { ID_CHKOF,ID_CHKON,ID_CHKREP,ID_CHKREPMSG };

void chkmen()
{
	int iCode, code;

	for (iCode = 0; iCode < (sizeof(datcod) / sizeof(int)); iCode++)
	{
		code = MF_UNCHECKED;
		if (iCode == IniFile.dataCheck)
			code = MF_CHECKED;
		CheckMenuItem(MainMenu, datcod[iCode], code);
	}
}

void duzrat() {

	if (!ZoomRect.right)
		ZoomRect.right = LHUPX;
	if (!ZoomRect.top)
		ZoomRect.top = LHUPY;
	ZoomRatio.x = (double)(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	ZoomRatio.y = (double)(StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom));
}

unsigned rsed() {

	SYSTEMTIME time;

	GetLocalTime(&time);
	return (time.wSecond << 16) | time.wMilliseconds;
}

void ritfnam(TCHAR* fileName) {

	unsigned		iName;
	unsigned char	tmpFileName[50];

	// ToDo - is this comparison correct?
	if (*FileNameOrder > 50)
		fnamtabs();
	PseudoRandomValue = rsed();
	for (iName = 0; iName < 50; iName++)
		tmpFileName[iName] = psg() & 0xff;
	for (iName = 0; iName < 50; iName++) {

		if (fileName[iName])
		{
			tmpFileName[iName] = FilenameEncoded[fileName[iName]];
		} else {

			while (FilenameDecode[tmpFileName[iName]])
				tmpFileName[iName] = psg() & 0xff;
			break;
		}
	}
	if (iName == 50) {

		while (FilenameDecode[tmpFileName[49]])
			tmpFileName[49] = psg() & 0xff;
	}
	for (iName = 0; iName < 50; iName++)
		if (FileNameOrder[iName] < 50) { ExtendedHeader.creatorName[FileNameOrder[iName]] = tmpFileName[iName]; }
}

void redfnam(TCHAR* fileName) {

	unsigned iName;
	unsigned char tmpFileName[50];

	for (iName = 0; iName < 50; iName++)
		if (FileNameOrder[iName] < 50)
			tmpFileName[iName] = ExtendedHeader.creatorName[FileNameOrder[iName]];
		else
			tmpFileName[iName] = 111;
	for (iName = 0; iName < 50; iName++) {

		fileName[iName] = FilenameDecode[tmpFileName[iName]];
		if (!fileName[iName])
			return;
	}
}

void fnamtabs() {

	unsigned		iName, swapInteger, source, destination;
	unsigned char	swapCharacter;
	unsigned*		pTemp;

	for (iName = 0; iName < 50; iName++)
		FileNameOrder[iName] = iName;
	PseudoRandomValue = NORDSED;
	for (iName = 0; iName < 100; iName++) {

		source = psg() % 50;
		destination = psg() % 50;
		swapInteger = FileNameOrder[destination];
		FileNameOrder[destination] = FileNameOrder[source];
		FileNameOrder[source] = swapInteger;
	}
	for (iName = 0; iName < 128; iName++)
		FilenameEncoded[iName] = (unsigned char)iName + NCODOF;
	PseudoRandomValue = NCODSED;
	for (iName = 0; iName < 256; iName++) {

		source = psg() & 0x7f;
		destination = psg() & 0x7f;
		swapCharacter = FilenameEncoded[destination];
		FilenameEncoded[destination] = FilenameEncoded[source];
		FilenameEncoded[source] = swapCharacter;
	}
	// ToDo - is pTemp used as an 'optimization'?
	pTemp = (unsigned*)&FilenameDecode;
	for (iName = 0; iName < 64; iName++)
		pTemp[iName] = 0;
	for (iName = 32; iName < 127; iName++)
		FilenameDecode[FilenameEncoded[iName]] = (unsigned char)iName;
}

void dstin(unsigned number, POINT* pout) {
	// ToDo - what is this code doing and what should nextBufferIndex be called?
	unsigned ind, shift;

	shift = 1;
	pout->x = pout->y = 0;
	for (ind = 0; ind < 22; ind++) {

		if (number&shift) {

			if (DSTValues[ind].cor)
				pout->y += DSTValues[ind].val;
			else
				pout->x += DSTValues[ind].val;
		}
		shift <<= 1;
	}
}

fPOINT* adflt(unsigned count) {

	unsigned iFormVertex = FormVertexIndex;

	if (FormVertexIndex + count > MAXFLT)
		tabmsg(IDS_FRMOVR);
	FormVertexIndex += count;
	return &FormVertices[iFormVertex];
}

SATCON* adsatk(unsigned count) {

	unsigned iSatinConnect = SatinConnectIndex;

	SatinConnectIndex += count;
	return &SatinConnects[iSatinConnect];
}

fPOINT* adclp(unsigned count) {

	unsigned iClipPoint = ClipPointIndex;

	ClipPointIndex += count;
	return &ClipPoints[iClipPoint];
}

unsigned duthrsh(double threshold) {

	unsigned	iZoomLevel = 0;
	double		zoom = 1;

	if (threshold) {

		while (zoom > threshold) {

			zoom *= ZUMFCT;
			iZoomLevel++;
		}
	} else
		return 0;
	return iZoomLevel + 1;
}

double unthrsh(unsigned level) {

	double	zoom = 1;

	if (level)
		level--;
	else
		return 0;
	while (level) {

		zoom *= ZUMFCT;
		level--;
	}
	return zoom;
}

void ritfcor(fPOINT* point) {

	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "x%.0f y%.0f", point->x / PFGRAN, point->y / PFGRAN);
	butxt(HCOR, MsgBuffer);
}

void ritcor(fPOINTATTR* pointAttribute) {

	fPOINT	point;

	point.x = pointAttribute->x;
	point.y = pointAttribute->y;
	ritfcor(&point);
}

void coltab() {

	unsigned	iStitch, iColor, nextColor;
	unsigned	currentColor;
	dRECTANGLE	range;
	fPOINTATTR*	Stitch;

	ColorChanges = 0;
	if (PCSHeader.stitchCount)
	{
		StitchBuffer[0].attribute &= NCOLMSK;
		StitchBuffer[0].attribute |= (StitchBuffer[1].attribute&COLMSK);
		StitchBuffer[PCSHeader.stitchCount - 1].attribute &= NCOLMSK;
		StitchBuffer[PCSHeader.stitchCount - 1].attribute |= (StitchBuffer[PCSHeader.stitchCount - 2].attribute&COLMSK);
		currentColor = StitchBuffer[0].attribute&COLMSK;
		for (iStitch = 1; iStitch < (unsigned)PCSHeader.stitchCount - 1; iStitch++)
		{
			if ((StitchBuffer[iStitch].attribute&COLMSK) != currentColor)
			{
				if ((StitchBuffer[iStitch + 1].attribute&COLMSK) == currentColor)
				{
					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= currentColor;
				}
				currentColor = StitchBuffer[iStitch].attribute&COLMSK;
			}
		}
		iColor = 0;
		currentColor = 0xffffffff;
		range.left = -UnzoomedRect.x;
		range.right = UnzoomedRect.x * 2;
		range.bottom = -UnzoomedRect.y;
		range.top = UnzoomedRect.y * 2;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		{
			Stitch = &StitchBuffer[iStitch];
			if (Stitch->x < range.left)
				Stitch->x = (float)range.left;
			if (Stitch->x > range.right)
				Stitch->x = (float)range.right;
			if (Stitch->y > range.top)
				Stitch->y = (float)range.top;
			if (Stitch->y < range.bottom)
				Stitch->y = (float)range.bottom;
			nextColor = Stitch->attribute&COLMSK;
			if (currentColor != nextColor)
			{
				ColorChangeTable[iColor].colorIndex = (unsigned char)nextColor;
				ColorChangeTable[iColor++].stitchIndex = (unsigned short)iStitch;
				currentColor = nextColor;
			}
		}
		ColorChanges = iColor;
		ColorChangeTable[iColor].stitchIndex = (unsigned short)iStitch;
		if (ClosestPointIndex > (unsigned)PCSHeader.stitchCount - 1)
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		fndknt();
	}
}

void ladj() {

	unsigned iLayer;

	for (iLayer = 0; iLayer < 5; iLayer++) {

		if (iLayer == ActiveLayer)
			EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_GRAYED);
		else
			EnableMenuItem(MainMenu, iLayer + M_ALL, MF_BYPOSITION | MF_ENABLED);
	}
	setMap(DUMEN);
}

void stchcpy(unsigned count, fPOINTATTR* destination) {

#if  __UseASM__
	_asm {

		mov		esi, offset StitchBuffer
		mov		edi, destination
		mov		ecx, count
		rep		movsd
	}
#else
	memcpy(destination, StitchBuffer, count * 4);
#endif
}

void deldu() {

	unsigned iBuffer;

	for (iBuffer = 0; iBuffer < 16; iBuffer++) {

		if (UndoBuffer[iBuffer]) {

			free(UndoBuffer[iBuffer]);
			UndoBuffer[iBuffer] = 0;
		}
	}
	UndoBufferWriteIndex = 0;
	rstMap(BAKWRAP);
	rstMap(BAKACT);
}

TCHAR* mvflpnt(fPOINT* destination, fPOINT* source, unsigned count) {

#if  __UseASM__
	_asm {

		mov		ecx, count
		shl		ecx, 1
		mov		esi, source
		mov		edi, destination
		rep		movsd
		mov		eax, edi
	}
#else
	memcpy(destination, source, count * sizeof(fPOINT));
	return (TCHAR *)(destination + count);
#endif
}

void mvsatk(SATCON* destination, SATCON* source, unsigned count) {

#if  __UseASM__
	_asm {

		mov		ecx, count
		mov		esi, source
		mov		edi, destination
		rep		movsd
	}
#else
	memcpy(destination, source, count * sizeof(SATCON));
#endif
}

void dudat() {

	unsigned	size;
	BAKHED*		backupData;

	if (UndoBuffer[UndoBufferWriteIndex])
		free(UndoBuffer[UndoBufferWriteIndex]);
	size = sizeof(BAKHED) + sizeof(FRMHED)*FormIndex + sizeof(fPOINTATTR)*PCSHeader.stitchCount
		+ sizeof(fPOINT)*(FormVertexIndex + ClipPointIndex) + sizeof(SATCON)*SatinConnectIndex + sizeof(COLORREF) * 16 +
		sizeof(TXPNT)*TextureIndex;
	UndoBuffer[UndoBufferWriteIndex] = calloc(size,sizeof(unsigned));
	backupData = (BAKHED*)UndoBuffer[UndoBufferWriteIndex];
	if (backupData) {

		backupData->zoomRect.x = UnzoomedRect.x;
		backupData->zoomRect.y = UnzoomedRect.y;
		backupData->formCount = FormIndex;
		backupData->forms = (FRMHED*)&backupData[1];
		//		for(unsigned iStitch=0;iStitch<FormIndex;iStitch++)
		//			frmcpy(&backupData->forms[iStitch],&FormList[iStitch]);
		MoveMemory(backupData->forms, &FormList, sizeof(FRMHED)*FormIndex);
		backupData->stitchCount = PCSHeader.stitchCount;
		backupData->stitches = (fPOINTATTR*)&backupData->forms[FormIndex];
		if (PCSHeader.stitchCount)
			stchcpy((sizeof(fPOINTATTR)*PCSHeader.stitchCount) >> 2, backupData->stitches);
		backupData->vertexCount = FormVertexIndex;
		backupData->vertices = (fPOINT*)&backupData->stitches[PCSHeader.stitchCount];
		if (FormVertexIndex)
			mvflpnt(backupData->vertices, &FormVertices[0], FormVertexIndex);
		backupData->satinConnectionCount = SatinConnectIndex;
		backupData->satinConnection = (SATCON*)&backupData->vertices[FormVertexIndex];
		if (SatinConnectIndex)
			mvsatk(backupData->satinConnection, &SatinConnects[0], SatinConnectIndex);
		backupData->clipPointCount = ClipPointIndex;
		backupData->clipPoints = (fPOINT*)&backupData->satinConnection[SatinConnectIndex];
		if (ClipPointIndex) {

			if (ClipPointIndex > MAXCLPNTS)
				ClipPointIndex = MAXCLPNTS;
			mvflpnt(backupData->clipPoints, &ClipPoints[0], ClipPointIndex);
		}
		backupData->colors = (COLORREF*)&backupData->clipPoints[ClipPointIndex];
		MoveMemory(backupData->colors, &UserColor, sizeof(COLORREF) * 16);
		backupData->texturePoints = (TXPNT*)&backupData->colors[16];
		backupData->texturePointCount = TextureIndex;
		if (TextureIndex)
			MoveMemory(backupData->texturePoints, &TexturePointsBuffer, sizeof(TXPNT)*TextureIndex);
	}
}

void savdo() {

	setMap(WASDO);
	setMap(CMPDO);
	if (rstMap(SAVACT)) {

		if (rstMap(BAKING)) {

			rstMap(REDUSHO);
			EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
		}
		setMap(BAKACT);
		EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
		setMap(DUMEN);
		dudat();
		UndoBufferWriteIndex++;
		UndoBufferWriteIndex &= 0xf;
		if (!UndoBufferWriteIndex)
			setMap(BAKWRAP);
	}
}

void redfils() {

	unsigned			iLRU;
	WIN32_FIND_DATA		findData;
	HANDLE				fileHandle;

	for (iLRU = 0; iLRU < OLDNUM; iLRU++) {
		if (GetMenuState(FileMenu, LRUMenuId[iLRU], MF_BYCOMMAND) != -1)
			DeleteMenu(FileMenu, LRUMenuId[iLRU], MF_BYCOMMAND);
	}
	for (iLRU = 0; iLRU < OLDNUM; iLRU++) {

		if (IniFile.prevNames[iLRU][0]) {

			if (chkMap(SAVAS))
				AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], IniFile.prevNames[iLRU]);
			else {

				fileHandle = FindFirstFile(IniFile.prevNames[iLRU], &findData);
				if (fileHandle == INVALID_HANDLE_VALUE)
					IniFile.prevNames[iLRU][0] = 0;
				else {

					AppendMenu(FileMenu, MF_BYCOMMAND | MF_STRING, LRUMenuId[iLRU], IniFile.prevNames[iLRU]);
					FindClose(fileHandle);
				}
			}
		}
	}
	setMap(DUMEN);
}

void nunams() {

	TCHAR*		iFileExtention;
	unsigned	iPrevious;
	unsigned	nameLength;
	TCHAR		swapName[_MAX_PATH];

	_strlwr_s(WorkingFileName);
	iFileExtention = strrchr(WorkingFileName, '.');
	if (iFileExtention)
		iFileExtention++;
	else
		iFileExtention = &WorkingFileName[strlen(WorkingFileName)];
	nameLength = iFileExtention - WorkingFileName;
	strncpy_s(AuxName, WorkingFileName, nameLength);
	strncpy_s(ThrName, WorkingFileName, nameLength);
	strncpy_s(GeName, WorkingFileName, nameLength);
	iFileExtention = AuxName + nameLength;
	switch (IniFile.auxFileType) {

	case AUXDST:

		strcpy_s(iFileExtention, sizeof(AuxName) - nameLength, "dst");
		break;

#if PESACT

	case AUXPES:

		strcpy_s(iFileExtention, sizeof(AuxName) - nameLength, "pes");
		break;

#endif

	default:

		strcpy_s(iFileExtention, sizeof(AuxName) - nameLength, "pcs");
	}
	iFileExtention = ThrName + nameLength;
	strcpy_s(iFileExtention, sizeof(ThrName) - nameLength, "thr");
	iFileExtention = GeName + nameLength;
	strcpy_s(iFileExtention, sizeof(GeName) - nameLength, "th*");
	for (iPrevious = 0; iPrevious < OLDNUM; iPrevious++) {

		if (!strcmp(IniFile.prevNames[iPrevious], ThrName)) {

			if (iPrevious) {

				strcpy_s(swapName, sizeof(swapName), IniFile.prevNames[0]);
				strcpy_s(IniFile.prevNames[0], sizeof(IniFile.prevNames[0]), IniFile.prevNames[iPrevious]);
				strcpy_s(IniFile.prevNames[iPrevious], sizeof(IniFile.prevNames[iPrevious]), swapName);
				goto mendun;
			} else
				goto nomen;
		}
	}
	for (iPrevious = 0; iPrevious < OLDNUM; iPrevious++) {

		if (!IniFile.prevNames[iPrevious][0]) {

			strcpy_s(IniFile.prevNames[iPrevious], ThrName);
			goto mendun;
		}
	}
	strcpy_s(IniFile.prevNames[3], IniFile.prevNames[2]);
	strcpy_s(IniFile.prevNames[2], IniFile.prevNames[1]);
	strcpy_s(IniFile.prevNames[1], IniFile.prevNames[0]);
	strcpy_s(IniFile.prevNames[0], ThrName);
mendun:;
	redfils();
nomen:;
}

void moveStitch(fPOINTATTR* destination, fPOINTATTR* source) {

#if  __UseASM__
	_asm {

		mov		esi, source
		mov		edi, destination
		xor		ecx, ecx
		mov		cl, 3
		rep		movsd
	}
#else
	memcpy(destination, source, sizeof(fPOINTATTR));
#endif
}

void duzero() {

	unsigned		iStitch, iForm;
	unsigned short	iDestination;
	double			stitchLength;
	fPOINTATTR*		currentStitch;

	if (SelectedFormCount) {

		clRmap(RMAPSIZ);
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			setr(SelectedFormList[iForm]);
		rstMap(CONTIG);
		iDestination = 0;
		currentStitch = StitchBuffer;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (StitchBuffer[iStitch].attribute&TYPMSK&&chkr((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT)) {

				if (setMap(CONTIG)) {

					stitchLength = hypot(StitchBuffer[iStitch].x - currentStitch->x, StitchBuffer[iStitch].y - currentStitch->y);
					if (stitchLength > MinStitchLength) {

						currentStitch = &StitchBuffer[iStitch];
						moveStitch(&StitchBuffer[iDestination++], &StitchBuffer[iStitch]);
					}
				} else
					currentStitch = &StitchBuffer[iStitch];
			} else {

				moveStitch(&StitchBuffer[iDestination++], &StitchBuffer[iStitch]);
				rstMap(CONTIG);
			}
		}
		PCSHeader.stitchCount = iDestination;
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		delsmal(GroupStartStitch, GroupEndStitch);
	} else
		delsmal(0, PCSHeader.stitchCount);
}

void rshft(POINT shiftPoint) {

	ZoomRect.right -= shiftPoint.x;
	ZoomRect.left -= shiftPoint.x;
	ZoomRect.top -= shiftPoint.y;
	ZoomRect.bottom -= shiftPoint.y;
	zRctAdj();
	setMap(RESTCH);
}

void pgdwn() {

	POINT scrollPosition;

	if (chkMap(ZUMED)) {

		scrollPosition.y = (ZoomRect.top - ZoomRect.bottom)*PAGSCROL;
		scrollPosition.x = 0;
		rshft(scrollPosition);
	}
}

void pgup() {

	POINT scrollPosition;

	if (chkMap(ZUMED)) {

		scrollPosition.y = -(ZoomRect.top - ZoomRect.bottom)*PAGSCROL;
		scrollPosition.x = 0;
		rshft(scrollPosition);
	}
}

void pglft() {

	POINT scrollPosition;

	if (chkMap(ZUMED)) {

		scrollPosition.x = (ZoomRect.right - ZoomRect.left)*PAGSCROL;
		scrollPosition.y = 0;
		rshft(scrollPosition);
	}
}

void pgrit() {

	POINT scrollPosition;

	if (chkMap(ZUMED)) {

		scrollPosition.x = -(ZoomRect.right - ZoomRect.left)*PAGSCROL;
		scrollPosition.y = 0;
		rshft(scrollPosition);
	}
}

void selin(unsigned start, unsigned end, HDC dc) {

	unsigned		iStitch,swap;
	double			tcor;
	long			hi;
	
	SelectObject(dc, GroupSelectPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	//Todo - Is SearchLine initialized at this point?
	if (SearchLineIndex)
		Polyline(dc, SearchLine, SearchLineIndex);
	if (start > end) {

		swap = start;
		start = end;
		end = swap;
	}
	hi = StitchWindowClientRect.bottom;
	SearchLineIndex = 0;
	for (iStitch = start; iStitch <= end; iStitch++) {

		tcor = ((StitchBuffer[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5);
		SearchLine[SearchLineIndex].x = (long)tcor;
		tcor = (hi - (StitchBuffer[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5);
		SearchLine[SearchLineIndex++].y = (long)tcor;
	}
	Polyline(dc, SearchLine, SearchLineIndex);
	SetROP2(dc, R2_COPYPEN);
}

void uncros() {

	if (rstMap(SCROS))
		cros(GroupStartStitch);
	if (rstMap(ECROS)) {

		if (GroupEndStitch != GroupStartStitch)
			cros(GroupEndStitch);
		setMap(ELIN);
	}
}

void ducros(HDC dc) {

	uncros();
	setMap(SCROS);
	cros(GroupStartStitch);
	if (GroupStartStitch != GroupEndStitch) {

		cros(GroupEndStitch);
		setMap(ECROS);
	}
	selin(GroupStartStitch, GroupEndStitch, dc);
}

void selRct(fRECTANGLE* sourceRect) {

	unsigned iStitch;

	sourceRect->left = sourceRect->right = StitchBuffer[GroupStartStitch].x;
	sourceRect->top = sourceRect->bottom = StitchBuffer[GroupStartStitch].y;
	for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {

		if (StitchBuffer[iStitch].x > sourceRect->right)
			sourceRect->right = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].x < sourceRect->left)
			sourceRect->left = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].y < sourceRect->bottom)
			sourceRect->bottom = StitchBuffer[iStitch].y;
		if (StitchBuffer[iStitch].y > sourceRect->top)
			sourceRect->top = StitchBuffer[iStitch].y;
	}
	if (sourceRect->right - sourceRect->left == 0) {

		sourceRect->right++;
		sourceRect->left--;
	}
	if (sourceRect->top - sourceRect->bottom == 0) {

		sourceRect->top++;
		sourceRect->bottom--;
	}
}

void rngadj() {

	if (ClosestPointIndex > (unsigned)PCSHeader.stitchCount - 1)
		ClosestPointIndex = PCSHeader.stitchCount - 1;
	if (GroupStitchIndex > PCSHeader.stitchCount)
		GroupStitchIndex = ClosestPointIndex;
	if (GroupStitchIndex > ClosestPointIndex) {

		GroupStartStitch = ClosestPointIndex;
		GroupEndStitch = GroupStitchIndex;
	} else {

		GroupStartStitch = GroupStitchIndex;
		GroupEndStitch = ClosestPointIndex;
	}
}

void lenfn(unsigned start, unsigned end) {

	unsigned	iStitch;
	double		maxLength = 0;
	double		minLength = 1e99;
	double		length;
	TCHAR		buffer[50];

	SmallestStitchIndex = 0; LargestStitchIndex = 0;
	for (iStitch = start; iStitch < end; iStitch++) {

		length = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
		if (length > maxLength) {

			maxLength = length;
			LargestStitchIndex = iStitch;
		}
		if (length < minLength) {

			minLength = length;
			SmallestStitchIndex = iStitch;
		}
	}
	sprintf_s(buffer, sizeof(buffer), "max %.2f", maxLength / PFGRAN);
	butxt(HMAXLEN, buffer);
	sprintf_s(buffer, sizeof(buffer), "min %.2f", minLength / PFGRAN);
	butxt(HMINLEN, buffer);
}

void frmcalc() {

	unsigned	iStitch, code;
	double		maxLength = 0;
	double		minLength = 1e99;
	double		length;

	if (FormList[ClosestFormToCursor].fillType || FormList[ClosestFormToCursor].edgeType) {

		code = ClosestFormToCursor << FRMSHFT;
		for (iStitch = 0; iStitch < (unsigned)PCSHeader.stitchCount - 1; iStitch++) {

			if ((StitchBuffer[iStitch].attribute&FRMSK) == code && !(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch + 1].attribute&FRMSK) == code && (StitchBuffer[iStitch + 1].attribute&TYPMSK)) {

				length = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
				if (length > maxLength) {

					maxLength = length;
					LargestStitchIndex = iStitch;
				}
				if (length < minLength) {

					minLength = length;
					SmallestStitchIndex = iStitch;
				}
			}
		}
		if (fabs(maxLength < 10000)) {

			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "max %.2f", maxLength / PFGRAN);
			butxt(HMAXLEN, MsgBuffer);
		}
		if (fabs(minLength < 10000)) {

			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "min %.2f", minLength / PFGRAN);
			butxt(HMINLEN, MsgBuffer);
		}
	} else {

		butxt(HMAXLEN, "");
		butxt(HMINLEN, "");
	}
}

void lenCalc() {

	if (chkMap(LENSRCH)) {

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x, StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y) / PFGRAN);
		butxt(HMINLEN, MsgBuffer);
		msgstr(IDS_SRCH);
		butxt(HMAXLEN, MsgBuffer);
	} else {

		if (PCSHeader.stitchCount > 1) {

			if (chkMap(FORMSEL)) {

				frmcalc();
				butxt(HCOR, "");
				return;
			}
			rngadj();
			if (chkMap(GRPSEL) && GroupStartStitch != GroupEndStitch)
				lenfn(GroupStartStitch, GroupEndStitch);
			else
				lenfn(0, PCSHeader.stitchCount - 1);
		} else {

			butxt(HMAXLEN, "");
			butxt(HMINLEN, "");
		}
	}
}

void nuselrct() {

	fPOINT		outline[9];
	unsigned	iLine;

	unsel();
	outline[0].x = outline[6].x = outline[7].x = outline[8].x = StitchRangeRect.left;
	outline[1].x = outline[5].x = midl(StitchRangeRect.right, StitchRangeRect.left);
	outline[0].y = outline[1].y = outline[2].y = outline[8].y = StitchRangeRect.top;
	outline[3].y = outline[7].y = midl(StitchRangeRect.top, StitchRangeRect.bottom);
	outline[4].y = outline[5].y = outline[6].y = StitchRangeRect.bottom;
	outline[2].x = outline[3].x = outline[4].x = StitchRangeRect.right;
	for (iLine = 0; iLine < 9; iLine++)
		sfCor2px(outline[iLine], &FormOutlineRect[iLine]);
}

void grpAdj() {

	POINT		newSize;
	double		coordinate;

	uncros();
	unsel();
	rngadj();
	ducros(StitchWindowDC);
	lenCalc();
	selRct(&StitchRangeRect);
	if (chkMap(ZUMED) && GroupEndStitch != GroupStartStitch) {

		if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1
			|| StitchRangeRect.left<ZoomRect.left + 1 || StitchRangeRect.right>ZoomRect.right - 1) {

			newSize.x = StitchRangeRect.right - StitchRangeRect.left;
			newSize.y = StitchRangeRect.top - StitchRangeRect.bottom;
			if (newSize.x < MINZUM) {

				if (newSize.x < TINY)
					newSize.x = 1;
				coordinate = MINZUM / newSize.x;
				newSize.x = MINZUM;
				newSize.y = coordinate*newSize.y;
			}
			if (newSize.x > newSize.y) {

				coordinate = newSize.x*ZMARGIN;
				newSize.x += (long)coordinate;
				coordinate = newSize.x / StitchWindowAspectRatio;
				newSize.y = (long)coordinate;
			} else {

				coordinate = newSize.y*ZMARGIN;
				newSize.y = (long)coordinate;
				coordinate = newSize.y*StitchWindowAspectRatio;
				newSize.x = (long)coordinate;
			}
			if (newSize.x > UnzoomedRect.x || newSize.y > UnzoomedRect.y) {

				ZoomRect.left = ZoomRect.bottom = 0;
				ZoomRect.right = UnzoomedRect.x;
				ZoomRect.top = UnzoomedRect.y;
				rstMap(ZUMED);
				ZoomFactor = 1;
				movStch();
			} else {

				ZoomRect.right = ZoomRect.left + (newSize.x);
				ZoomFactor = (double)newSize.x / UnzoomedRect.x;
				ZoomRect.top = ZoomRect.bottom + (newSize.y);
				SelectedPoint.x = ((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left;
				SelectedPoint.y = ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom;
				shft(SelectedPoint);
			}
		}
	}
	setMap(RESTCH);
}

void lensadj() {

	uncros();
	unsel();
	ClosestPointIndex = CurrentStitchIndex;
	GroupStitchIndex = CurrentStitchIndex + 1;
	rngadj();
	ducros(StitchWindowDC);
	lenCalc();
	selRct(&StitchRangeRect);
	if (StitchRangeRect.top > ZoomRect.top - 1 || StitchRangeRect.bottom < ZoomRect.bottom - 1
		|| StitchRangeRect.left<ZoomRect.left + 1 || StitchRangeRect.right>ZoomRect.right - 1) {

		SelectedPoint.x = ((StitchRangeRect.right - StitchRangeRect.left) / 2) + StitchRangeRect.left;
		SelectedPoint.y = ((StitchRangeRect.top - StitchRangeRect.bottom) / 2) + StitchRangeRect.bottom;
		shft(SelectedPoint);
	}
	nuAct(GroupStartStitch);
	setMap(RESTCH);
}

void ritot(unsigned number) {

	TCHAR buffer[64];

	sprintf_s(buffer, sizeof(buffer), StringTable[STR_TOT], number);
	BufferDigitCount = strlen(buffer);
	butxt(HTOT, buffer);
}

void ritlayr() {

	TCHAR buffer[12];
	unsigned layer;

	layer = 0xffffffff;
	if (chkMap(SELBOX))
		layer = (StitchBuffer[ClosestPointIndex].attribute&LAYMSK) >> LAYSHFT;
	else {

		if (chkMap(FORMSEL) || chkMap(FRMPSEL))
			layer = (FormList[ClosestFormToCursor].attribute&FRMLMSK) >> 1;
	}
	if (layer & 0xffff0000)
		butxt(HLAYR, "");
	else {

		sprintf_s(buffer, sizeof(buffer), StringTable[STR_LAYR], layer);
		BufferDigitCount = strlen(buffer);
		butxt(HLAYR, buffer);
	}
}

void nuRct() {

	GetClientRect(ThrEdWindow, &ThredWindowRect);
	GetWindowRect(ColorBar, &ColorBarRect);
	GetWindowRect(ButtonWin[HMINLEN], &MinLenRect);
	GetWindowRect(ButtonWin[HMAXLEN], &MaxLenRect);
	ReleaseDC(ColorBar, ColorBarDC);
	ColorBarDC = GetDC(ColorBar);
	DeleteDC(StitchWindowMemDC);
	ReleaseDC(MainStitchWin, StitchWindowDC);
	DeleteObject(StitchWindowBmp);
	ReleaseDC(MainStitchWin, StitchWindowDC);
	StitchWindowDC = GetDCEx(MainStitchWin, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	DeleteDC(StitchWindowMemDC);
	StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
	GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
	ReleaseDC(ThrEdWindow, ThredDC);
	ThredDC = GetDC(ThrEdWindow);
	SetStretchBltMode(ThredDC, COLORONCOLOR);
	GetDCOrgEx(ThredDC, &ThredWindowOrigin);
	GetWindowRect(MainStitchWin, &StitchWindowAbsRect);
	GetClientRect(MainStitchWin, &StitchWindowClientRect);
	StitchWindowBmp = CreateCompatibleBitmap(StitchWindowDC, StitchWindowClientRect.right, StitchWindowClientRect.bottom);
	SelectObject(StitchWindowMemDC, StitchWindowBmp);
}

HPEN nuPen(HPEN pen, unsigned width, COLORREF color) {

	DeleteObject(pen);
	return CreatePen(PS_SOLID, width, color);
}

void nuStchSiz(unsigned iColor, unsigned width) {

	if (width != ThreadSizePixels[iColor]) {

		UserPen[iColor] = nuPen(UserPen[iColor], width, UserColor[iColor]);
		ThreadSizePixels[iColor] = width;
	}
}

HBRUSH nuBrush(HBRUSH brush, COLORREF color) {

	DeleteObject(brush);
	return CreateSolidBrush(color);
}

void box(unsigned iNearest, HDC dc) {

	long boxWidth = BoxOffset[iNearest];
	POINT line[5];

	line[0].x = NearestPixel[iNearest].x - boxWidth;
	line[0].y = NearestPixel[iNearest].y - boxWidth;
	line[1].x = NearestPixel[iNearest].x + boxWidth;
	line[1].y = NearestPixel[iNearest].y - boxWidth;
	line[2].x = NearestPixel[iNearest].x + boxWidth;
	line[2].y = NearestPixel[iNearest].y + boxWidth;
	line[3].x = NearestPixel[iNearest].x - boxWidth;
	line[3].y = NearestPixel[iNearest].y + boxWidth;
	line[4].x = NearestPixel[iNearest].x - boxWidth;
	line[4].y = NearestPixel[iNearest].y - boxWidth;
	Polyline(dc, line, 5);
}

void boxs() {

	unsigned	iNear;

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	for (iNear = 0; iNear < NearestCount; iNear++) {

		SelectObject(StitchWindowDC, BoxPen[iNear]);
		box(iNear, StitchWindowDC);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void dubx() {

	POINT line[5];
	long boxWidth = BoxOffset[0];

	SelectObject(StitchWindowMemDC, BoxPen[0]);
	SelectObject(StitchWindowDC, BoxPen[0]);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	// ToDo - is BoxCoordinate initialized at this point
	line[0].x = BoxCoordinate.x - boxWidth;
	line[0].y = BoxCoordinate.y - boxWidth;
	line[1].x = BoxCoordinate.x + boxWidth;
	line[1].y = BoxCoordinate.y - boxWidth;
	line[2].x = BoxCoordinate.x + boxWidth;
	line[2].y = BoxCoordinate.y + boxWidth;
	line[3].x = BoxCoordinate.x - boxWidth;
	line[3].y = BoxCoordinate.y + boxWidth;
	line[4].x = BoxCoordinate.x - boxWidth;
	line[4].y = BoxCoordinate.y - boxWidth;
	Polyline(StitchWindowMemDC, line, 5);
	Polyline(StitchWindowDC, line, 5);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void duar() {

	POINT arrowCenter;

	StitchArrow[1].x = StitchCoordinatesPixels.x;
	StitchArrow[1].y = StitchCoordinatesPixels.y;
	RotationCenterPixels.x = StitchCoordinatesPixels.x;
	RotationCenterPixels.y = StitchCoordinatesPixels.y;
	arrowCenter.x = StitchCoordinatesPixels.x - 10;
	arrowCenter.y = StitchCoordinatesPixels.y + 10;
	rotpix(arrowCenter, &StitchArrow[0]);
	arrowCenter.y = StitchCoordinatesPixels.y - 10;
	rotpix(arrowCenter, &StitchArrow[2]);
	SelectObject(StitchWindowMemDC, BoxPen[0]);
	SelectObject(StitchWindowDC, BoxPen[0]);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, StitchArrow, 3);
	Polyline(StitchWindowDC, StitchArrow, 3);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void dubox() {

	RotateAngle = atan2(StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y, StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x);
	duar();
	rstMap(ELIN);
	setMap(SELBOX);
	rstMap(FRMPSEL);
	redraw(ColorBar);
	if (!BufferIndex)
		ritnum(STR_NUMSEL, ClosestPointIndex);
}

void unbox() {

	if (rstMap(SELBOX)) {

		SelectObject(StitchWindowDC, BoxPen[0]);
		SetROP2(StitchWindowDC, R2_NOTXORPEN);
		Polyline(StitchWindowDC, StitchArrow, 3);
		SetROP2(StitchWindowDC, R2_COPYPEN);
	}
}

void unboxs() {

	if (NearestCount) {

		boxs();
		NearestCount = 0;
	}
}

void redraw(HWND window) {

	unsigned iWindow;

	RedrawWindow(window, NULL, NULL, RDW_INVALIDATE);
	if (window == MainStitchWin) {

		for (iWindow = 0; iWindow < 16; iWindow++) {

			if (DefaultColorWin[iWindow])
				RedrawWindow(DefaultColorWin[iWindow], NULL, NULL, RDW_INVALIDATE);
		}
		RedrawWindow(ColorBar, NULL, NULL, RDW_INVALIDATE);
	}
}

unsigned stch2px(unsigned iStitch) {

	StitchCoordinatesPixels.x = (StitchBuffer[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (StitchBuffer[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
	if (StitchCoordinatesPixels.x >= 0 &&
		StitchCoordinatesPixels.x <= StitchWindowClientRect.right&&
		StitchCoordinatesPixels.y >= 0 &&
		StitchCoordinatesPixels.y <= StitchWindowClientRect.bottom)

		return 1;
	else
		return 0;
}

void stch2px1(unsigned iStitch) {

	StitchCoordinatesPixels.x = (StitchBuffer[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (StitchBuffer[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void stch2pxr(fPOINT stitchCoordinate) {

	StitchCoordinatesPixels.x = (stitchCoordinate.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (stitchCoordinate.y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void movins() {

	if (chkMap(INSRT)) {

		if (chkMap(LIN1)) {

			if (chkMap(BAKEND))
				stch2px1(PCSHeader.stitchCount - 1);
			else
				stch2px1(0);
			endpnt();
		} else
			duIns();
	}
}

void defNam(TCHAR* fileName) {

	TCHAR* iLast;

	if (fileName[0]) {

		strcpy_s(DefaultDirectory, fileName);
		iLast = strrchr(DefaultDirectory, '\\');
		if (iLast - DefaultDirectory == 2)
			iLast[1] = 0;
		else
			iLast[0] = 0;
	}
}

void defbNam() {

	TCHAR* iLast;

	if (UserBMPFileName[0]) {

		strcpy_s(DefaultBMPDirectory, UserBMPFileName);
		iLast = strrchr(DefaultBMPDirectory, '\\');
		if (iLast - DefaultBMPDirectory == 2)
			iLast[1] = 0;
		else
			iLast[0] = 0;
	}
}

void ritini() {

	unsigned	iColor;
	RECT		windowRect;

	// ToDo - ensure ini names are the same as variable names
	strcpy_s(IniFile.defaultDirectory, DefaultDirectory);
	for (iColor = 0; iColor < 16; iColor++) {

		IniFile.stitchColors[iColor] = UserColor[iColor];
		IniFile.backgroundPreferredColors[iColor] = CustomBackgroundColor[iColor];
		IniFile.stitchPreferredColors[iColor] = CustomColor[iColor];
		IniFile.bitmapBackgroundColors[iColor] = BitmapBackgroundColors[iColor];
	}
	IniFile.backgroundColor = BackgroundColor;
	IniFile.bitmapColor = BitmapColor;
	IniFile.minStitchLength = MinStitchLength;
	IniFile.showStitchThreshold = ShowStitchThreshold;
	IniFile.threadSize30 = ThreadSize30;
	IniFile.threadSize40 = ThreadSize40;
	IniFile.threadSize60 = ThreadSize60;
	IniFile.userStitchLength = UserStitchLength;
	IniFile.smallStitchLength = SmallStitchLength;
	IniFile.stitchBoxesThreshold = StitchBoxesThreshold;
	IniFile.stitchSpace = StitchSpacing;
	IniFile.userFlagMap = UserFlagMap;
	IniFile.borderWidth = BorderWidth;
	IniFile.underlayColor = UnderlayColor;
	IniFile.snapLength = SnapLength;
	IniFile.starRatio = StarRatio;
	IniFile.spiralWrap = SpiralWrap;
	IniFile.buttonholeFillCornerLength = ButtonholeCornerLength;
	IniFile.picotSpace = PicotSpacing;
	if (!chku(SAVMAX)) {

		GetWindowRect(ThrEdWindow, &windowRect);
		IniFile.initialWindowCoords.left = windowRect.left;
		IniFile.initialWindowCoords.right = windowRect.right;
		IniFile.initialWindowCoords.bottom = windowRect.bottom;
		IniFile.initialWindowCoords.top = windowRect.top;
	}
	IniFileHandle = CreateFile(IniFileName, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (IniFileHandle != INVALID_HANDLE_VALUE)
		WriteFile(IniFileHandle, &IniFile, sizeof(INIFILE), &BytesRead, NULL);
	CloseHandle(IniFileHandle);
}

BOOL savcmp() {

#ifdef _DEBUG

	return 1;

#else

	return !chkMap(CMPDO);

#endif
}

void thr2bal(unsigned destination, unsigned source, unsigned code) {

#define BALRAT 1.6666666666667

	BalaradStitch[destination].flag = 0;
	BalaradStitch[destination].code = (unsigned char)code;
	BalaradStitch[destination].x = (StitchBuffer[source].x - BalaradOffset.x)*BALRAT;
	BalaradStitch[destination].y = (StitchBuffer[source].y - BalaradOffset.y)*BALRAT;
}

unsigned coldis(COLORREF colorA, COLORREF colorB) {

	unsigned distance;

	distance = ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	colorA >>= 8;
	colorB >>= 8;
	distance += ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	colorA >>= 8;
	colorB >>= 8;
	distance += ((colorA & 0xff) > (colorB & 0xff)) ? ((colorA & 0xff) - (colorB & 0xff)) : ((colorB & 0xff) - (colorA & 0xff));
	return distance;
}

void bal2thr(unsigned destination, unsigned source, unsigned code) {

#define IBALRAT 0.6

	StitchBuffer[destination].attribute = code;
	StitchBuffer[destination].x = BalaradStitch[source].x*IBALRAT + BalaradOffset.x;
	StitchBuffer[destination].y = BalaradStitch[source].y*IBALRAT + BalaradOffset.y;
}

unsigned colmatch(COLORREF color) {

	unsigned iColor, distance, minDistance, iDistance = 0;

	if (ColorChanges < 16) {

		for (iColor = 0; iColor < ColorChanges; iColor++) {

			if (color == UserColor[iColor])
				return iColor;
		}
		UserColor[ColorChanges++] = color;
		return ColorChanges - 1;
	} else {

		minDistance = 0xffffffff;
		for (iColor = 0; iColor < ColorChanges; iColor++) {

			distance = coldis(color, UserColor[iColor]);
			if (!distance)
				return iColor;
			iDistance = 0;
			if (distance < minDistance) {

				iDistance = iColor;
				minDistance = distance;
			}
		}
		return iDistance;
	}
}

void redbal() {

	HANDLE			balaradFile;
	BALHED			balaradHeader;
	unsigned long	bytesRead;
	unsigned		iStitch, iColor, iBalaradStitch, stitchCount, color;

	PCSHeader.stitchCount = 0;
	FormIndex = 0;
	balaradFile = CreateFile(BalaradName2, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (balaradFile != INVALID_HANDLE_VALUE) {

		ReadFile(balaradFile, (BALHED*)&balaradHeader, sizeof(BALHED), &bytesRead, 0);
		if (bytesRead == sizeof(BALHED)) {

			BalaradStitch = (BALSTCH*)&BSequence;
			ReadFile(balaradFile, (BALSTCH*)BalaradStitch, sizeof(BSequence), &bytesRead, 0);
			stitchCount = bytesRead / sizeof(BALSTCH);
			IniFile.backgroundColor = BackgroundColor = balaradHeader.backgroundColor;
			BackgroundPen = nuPen(BackgroundPen, 1, BackgroundColor);
			BackgroundPenWidth = 1;
			DeleteObject(BackgroundBrush);
			BackgroundBrush = CreateSolidBrush(BackgroundColor);
			UnzoomedRect.x = IniFile.hoopSizeX = balaradHeader.hoopSizeX*IBALRAT;
			UnzoomedRect.y = IniFile.hoopSizeY = balaradHeader.hoopSizeY*IBALRAT;
			BalaradOffset.x = IniFile.hoopSizeX / 2;
			BalaradOffset.y = IniFile.hoopSizeY / 2;
			IniFile.hoopType = PCSHeader.hoopType = CUSTHUP;
			UserColor[0] = balaradHeader.color[0];
			color = 0;
			iColor = 1;
			iBalaradStitch = 0;
			ColorChanges = 1;
			for (iStitch = 0; iStitch < stitchCount; iStitch++) {

				switch (BalaradStitch[iStitch].code) {

				case BALNORM:

					bal2thr(iBalaradStitch++, iStitch, color);
					break;

				case BALSTOP:

					color = colmatch(balaradHeader.color[iColor++]);
					break;
				}
			}
			for (iColor = 0; iColor < ColorChanges; iColor++) {

				UserPen[iColor] = CreatePen(PS_SOLID, 1, UserColor[iColor]);
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
			}
			PCSHeader.stitchCount = iBalaradStitch;
			coltab();
			redraw(ColorBar);
			setMap(INIT);
			setMap(RESTCH);
		}
	}
	CloseHandle(balaradFile);
}

void ritbal() {

	BALHED			balaradHeader;
	unsigned*		ptrBalaradHeader;
	unsigned		iHeader, iStitch, iColor, color;
	HANDLE			balaradFile;
	TCHAR*			lastNameCharacter;
	TCHAR			outputName[_MAX_PATH];
	unsigned long	bytesWritten;

	if (*BalaradName0&&*BalaradName1&&PCSHeader.stitchCount) {

		if (!*WorkingFileName) {

			strcpy_s(WorkingFileName, DefaultDirectory);
			strcat_s(WorkingFileName, "\\balfil.thr");
		}
		strcpy_s(outputName, WorkingFileName);
		lastNameCharacter = strrchr(outputName, '.');
		if (lastNameCharacter)
			strcpy_s(lastNameCharacter, sizeof(outputName) - (lastNameCharacter - outputName), ".thv");
		else
			strcat_s(outputName, sizeof(outputName) - strlen(outputName), ".thv");
		balaradFile = CreateFile(outputName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (balaradFile == INVALID_HANDLE_VALUE)
			return;
		ptrBalaradHeader = (unsigned*)&balaradHeader;
		for (iHeader = 0; iHeader < (sizeof(BALHED) >> 2); iHeader++)
			ptrBalaradHeader[iHeader] = 0;
		color = StitchBuffer[0].attribute&COLMSK;
		balaradHeader.color[0] = UserColor[color];
		iColor = 1;
		for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (color != (StitchBuffer[iStitch].attribute&COLMSK)) {

				color = StitchBuffer[iStitch].attribute&COLMSK;
				balaradHeader.color[iColor++] = UserColor[color];
				if (iColor & 0xffffff00)
					break;
			}
		}
		balaradHeader.signature = 'drbm';
		balaradHeader.backgroundColor = BackgroundColor;
		balaradHeader.hoopSizeX = IniFile.hoopSizeX*BALRAT;
		balaradHeader.hoopSizeY = IniFile.hoopSizeY*BALRAT;
		WriteFile(balaradFile, (BALHED*)&balaradHeader, sizeof(BALHED), &bytesWritten, 0);
		BalaradOffset.x = IniFile.hoopSizeX / 2;
		BalaradOffset.y = IniFile.hoopSizeY / 2;
		BalaradStitch = (BALSTCH*)&BSequence;
		color = StitchBuffer[0].attribute&COLMSK;
		thr2bal(0, 0, BALJUMP);
		BalaradStitch[1].flag = (unsigned char)color;
		iColor = 1;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			thr2bal(iColor++, iStitch, BALNORM);
			if ((StitchBuffer[iStitch].attribute&COLMSK) != color) {

				thr2bal(iColor, iStitch, BALSTOP);
				color = StitchBuffer[iStitch].attribute&COLMSK;
				BalaradStitch[iColor++].flag = (unsigned char)color;
			}
		}
		WriteFile(balaradFile, (BALSTCH*)BalaradStitch, iColor * sizeof(BALSTCH), &bytesWritten, 0);
		CloseHandle(balaradFile);
		balaradFile = CreateFile(BalaradName1, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		WriteFile(balaradFile, (TCHAR*)outputName, strlen(outputName) + 1, &bytesWritten, 0);
		CloseHandle(balaradFile);
	} else {

		if (*BalaradName1) {

			balaradFile = CreateFile(BalaradName1, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			CloseHandle(balaradFile);
		}
	}
	if (*BalaradName0)
		DeleteFile(BalaradName0);
}

void savmsg() {

	TCHAR	buffer[HBUFSIZ];

	LoadString(ThrEdInstance, IDS_SAVFIL, buffer, HBUFSIZ);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), buffer, ThrName);
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

		DestroyWindow(PreferencesWindow);
		rstMap(WASRT);
	}
	unsid();
	unbsho();
	rstAll();
	//	if(savcmp()||(*BalaradName0&&*BalaradName1&&PCSHeader.stitchCount&&!FormIndex))
	if (savcmp() || (*BalaradName0))
		reldun();
	else {

		if (StitchWindowClientRect.right) {

			savdisc();
			setMap(SAVEX);
		} else {

			savmsg();
			if (MessageBox(ThrEdWindow, MsgBuffer, StringTable[STR_CLOS], MB_YESNO) == IDYES)
				save();
			reldun();;
		}
	}
}

void dusid(unsigned entry) {

	SideWindow[entry] = CreateWindow(
		"STATIC",
		SideWindowsStrings[entry],
		SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
		3,
		SideWindowLocation*SideWindowSize.y + 3,
		SideWindowSize.x + 3,
		SideWindowSize.y,
		SideMessageWindow,
		NULL,
		ThrEdInstance,
		NULL);
	SideWindowLocation++;
}

void sidmsg(HWND window, TCHAR** strings, unsigned entries) {

	RECT		childListRect;
	RECT		parentListRect;
	unsigned	iEntry, entryCount = entries;

	FillMemory(&ValueWindow, sizeof(int)*LASTLIN, 0);
	SideWindowSize.x = SideWindowSize.y = 0;
	SideWindowLocation = 0;
	SideWindowsStrings = strings;
	GetWindowRect(window, &childListRect);
	GetWindowRect(FormDataSheet, &parentListRect);
	ispcdclp();
	if (chkMap(FILTYP)) {

		for (iEntry = 0; iEntry < EDGETYPS + 1; iEntry++) {

			if ((unsigned)(SelectedForm->edgeType&NEGUND) == EdgeFillTypes[iEntry])
				entryCount--;
			else {

				if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT || EdgeFillTypes[iEntry] == EDGECLIPX) {

					if (chkMap(WASPCDCLP))
						maxtsiz(strings[iEntry], &SideWindowSize);
					else
						entryCount--;
				} else
					maxtsiz(strings[iEntry], &SideWindowSize);
			}
		}
		SideMessageWindow = CreateWindow(
			"STATIC",
			0,
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			parentListRect.right - ThredWindowOrigin.x + 3,
			childListRect.top - ThredWindowOrigin.y - 3,
			SideWindowSize.x + 12,
			SideWindowSize.y*entryCount + 12,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);
		for (iEntry = 0; iEntry < entries; iEntry++) {

			if ((unsigned)(SelectedForm->edgeType&NEGUND) != EdgeFillTypes[iEntry]) {

				if (EdgeFillTypes[iEntry] == EDGECLIP || EdgeFillTypes[iEntry] == EDGEPICOT || EdgeFillTypes[iEntry] == EDGECLIPX) {

					if (chkMap(WASPCDCLP))
						dusid(iEntry);
				} else
					dusid(iEntry);
			}
		}
	} else {

		if (FormMenuChoice == LLAYR)
			maxtsiz("0", &SideWindowSize);
		else {

			if (FormMenuChoice == LFTHTYP) {

				entryCount = 5;
				SideWindowSize.x = ButtonWidthX3;
				SideWindowSize.y = ButtonHeight;
			} else {

				for (iEntry = 0; iEntry < entries; iEntry++) {

					if ((1 << FillTypes[iEntry])&ClipTypeMap) {

						if (chkMap(WASPCDCLP))
							maxtsiz(strings[iEntry], &SideWindowSize);
						else
							entryCount--;
					} else {

						if (FillTypes[iEntry] == SelectedForm->fillType)
							entryCount--;
						else
							maxtsiz(strings[iEntry], &SideWindowSize);
					}
				}
			}
		}
		SideMessageWindow = CreateWindow(
			"STATIC",
			0,
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			parentListRect.right - ThredWindowOrigin.x + 3,
			childListRect.top - ThredWindowOrigin.y - 3,
			SideWindowSize.x + 12,
			SideWindowSize.y*entryCount + 12,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);
		if (FormMenuChoice == LLAYR) {

			for (iEntry = 0; iEntry < entries; iEntry++)
				dusid(iEntry);
		} else {

			if (FormMenuChoice == LFTHTYP) {

				for (iEntry = 0; iEntry < 6; iEntry++) {

					if (FeatherFillTypes[iEntry] != SelectedForm->fillInfo.feather.fillType)
						dusid(iEntry);
				}
			} else {

				for (iEntry = 0; iEntry < entries; iEntry++) {

					if (FillTypes[iEntry] != SelectedForm->fillType) {

						if ((1 << FillTypes[iEntry])&ClipTypeMap) {

							if (chkMap(WASPCDCLP))
								dusid(iEntry);
						} else
							dusid(iEntry);
					}
				}
			}
		}
	}
	setMap(SIDACT);
}


void stchPars() {

	AspectRatio = (double)UnzoomedRect.x / UnzoomedRect.y;
	if (chkMap(RUNPAT) || chkMap(WASPAT))
		StitchWindowSize.x = (long)(ThredWindowRect.bottom - (SCROLSIZ << 1))*AspectRatio;
	else
		StitchWindowSize.x = (long)(ThredWindowRect.bottom - SCROLSIZ)*AspectRatio;

	if ((StitchWindowSize.x + (long)ButtonWidthX3 + RIGHTSIZ) < ThredWindowRect.right) {

		if (chkMap(RUNPAT) || chkMap(WASPAT))
			StitchWindowSize.y = ThredWindowRect.bottom - (SCROLSIZ << 1);
		else
			StitchWindowSize.y = ThredWindowRect.bottom - SCROLSIZ;
	} else {

		StitchWindowSize.x = (ThredWindowRect.right - ButtonWidthX3 - COLSIZ);
		StitchWindowSize.y = ThredWindowRect.bottom - ThredWindowRect.top;
		if ((double)StitchWindowSize.x / StitchWindowSize.y > AspectRatio)
			StitchWindowSize.x = StitchWindowSize.y*AspectRatio;
		else
			StitchWindowSize.y = StitchWindowSize.x / AspectRatio;
	}
}

void movStch() {

	POINT		clientSize;
	long		verticalOffset = 0;
	long		actualWindowHeight = StitchWindowSize.y;

	clientSize.x = ThredWindowRect.right - ButtonWidthX3 - RIGHTSIZ;
	clientSize.y = ThredWindowRect.bottom;
	unboxs();
	if (chkMap(RUNPAT) || chkMap(WASPAT)) {

		verticalOffset = SCROLSIZ;
		clientSize.y -= SCROLSIZ;
		actualWindowHeight -= SCROLSIZ;
	}
	if (chkMap(ZUMED)) {

		clientSize.y -= SCROLSIZ;
		MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, clientSize.x, clientSize.y, FALSE);
		MoveWindow(VerticalScrollBar, ButtonWidthX3 + clientSize.x, 0, SCROLSIZ, clientSize.y, TRUE);
		MoveWindow(HorizontalScrollBar, ButtonWidthX3, clientSize.y + verticalOffset, clientSize.x, SCROLSIZ, TRUE);
		StitchWindowAspectRatio = (double)clientSize.x / clientSize.y;
		if (chkMap(RUNPAT) || chkMap(WASPAT))
			MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, clientSize.x, SCROLSIZ, TRUE);
		ShowWindow(VerticalScrollBar, TRUE);
		ShowWindow(HorizontalScrollBar, TRUE);
	} else {

		stchPars();
		actualWindowHeight = StitchWindowSize.y + SCROLSIZ;
		MoveWindow(MainStitchWin, ButtonWidthX3, verticalOffset, StitchWindowSize.x, actualWindowHeight, TRUE);
		ShowWindow(VerticalScrollBar, FALSE);
		ShowWindow(HorizontalScrollBar, FALSE);
		StitchWindowAspectRatio = (double)StitchWindowSize.x / actualWindowHeight;
		if (chkMap(RUNPAT) || chkMap(WASPAT))
			MoveWindow(SpeedScrollBar, ButtonWidthX3, 0, StitchWindowSize.x, SCROLSIZ, TRUE);
	}
	MoveWindow(ColorBar, ThredWindowRect.right - COLSIZ, 0, COLSIZ, ThredWindowRect.bottom, TRUE);
	nuRct();
	redraw(ColorBar);
}

void zRctAdj() {

	double	adjustment;

	if (ZoomRect.top > UnzoomedRect.y) {

		adjustment = ZoomRect.top - UnzoomedRect.y;
		ZoomRect.top -= adjustment;
		ZoomRect.bottom -= adjustment;
	}
	if (ZoomRect.bottom < 0) {

		ZoomRect.top -= ZoomRect.bottom;
		ZoomRect.bottom = 0;
	}
	if (ZoomRect.right > UnzoomedRect.x) {

		adjustment = ZoomRect.right - UnzoomedRect.x;
		ZoomRect.right -= adjustment;
		ZoomRect.left -= adjustment;
	}
	if (ZoomRect.left < 0) {

		ZoomRect.right -= ZoomRect.left;
		ZoomRect.left = 0;
	}
}

void shft(fPOINT delta) {

	dPOINT	halfZoomRect;
	dPOINT	center;
	dPOINT	shift;

	halfZoomRect.x = (ZoomRect.right - ZoomRect.left)/2;
	halfZoomRect.y = (ZoomRect.top - ZoomRect.bottom)/2;
	center.x = ZoomRect.left + halfZoomRect.x;
	center.y = ZoomRect.bottom + halfZoomRect.y;
	shift.x = center.x - delta.x;
	shift.y = center.y - delta.y;

	ZoomRect.bottom -= shift.y;
	ZoomRect.top -= shift.y;
	ZoomRect.left -= shift.x;
	ZoomRect.right -= shift.x;

	zRctAdj();
}

void centr() {

	POINT center;

	center.x = ZoomRect.top - ZoomRect.bottom;
	center.y = ZoomRect.right - ZoomRect.left;
	center.x >>= 1;
	center.y >>= 1;
	SelectedPoint.x = ZoomRect.left + center.x;
	SelectedPoint.y = ZoomRect.bottom + center.y;
}

double pxchk(double pixelSize) {

	if (pixelSize < 0.2)
		return 1;
	if (pixelSize > 20)
		return 20;
	return pixelSize;
}

void chknum() {

	double		value;
	unsigned	edgeType,borderColor;

	clrstch();
	value = atof(MsgBuffer);
	if (rstMap(ENTRDUP)) {

		if (value) {

			RotationAngle = value*PI / 180;
			IniFile.rotationAngle = RotationAngle;
		} else
			RotationAngle = IniFile.rotationAngle;
		duprot();
		return;
	}
	if (rstMap(NUROT)) {

		if (value) {

			RotationAngle = value*PI / 180;
			IniFile.rotationAngle = RotationAngle;
		} else
			RotationAngle = IniFile.rotationAngle;
		return;
	}
	if (rstMap(ENTRSEG)) {

		if (value) {

			RotationAngle = 2 * PI / value;
			IniFile.rotationAngle = RotationAngle;
		} else
			RotationAngle = IniFile.rotationAngle;
		return;
	}
	if (rstMap(ENTROT)) {

		if (value) {

			RotationAngle = value*PI / 180;
			IniFile.rotationAngle = RotationAngle;
		} else
			RotationAngle = IniFile.rotationAngle;
		rotfns();
		return;
	}
	if (MsgIndex)
	{
		if (FormMenuChoice) {

			value = atof(SideWindowEntryBuffer)*PFGRAN;
			switch (FormMenuChoice) {

			case LTXOF:

				savdo();
				SelectedForm->txof = value;
				break;

			case LUANG:

				savdo();
				SelectedForm->underlayStitchAngle = value / 180 * PI / PFGRAN;
				break;

			case LUSPAC:

				savdo();
				SelectedForm->underlaySpacing = value;
				break;

			case LWLKIND:

				savdo();
				SelectedForm->underlayIndent = value;
				break;

			case LULEN:

				savdo();
				SelectedForm->underlayStitchLen = value;
				break;

			case LDSTRT:

				savdo();
				SelectedForm->fillStart = value / PFGRAN;
				SelectedForm->fillStart %= SideCount;
				break;

			case LDEND:

				savdo();
				SelectedForm->fillEnd = value / PFGRAN;
				SelectedForm->fillEnd %= SideCount;
				break;

			case LFTHUPCNT:

				savdo();
				value /= PFGRAN;
				if (value > 255)
					value = 255;
				SelectedForm->fillInfo.feather.upCount = value;
				break;

			case LFTHCOL:

				if (value) {

					savdo();
					nufthcol((atoi(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText(ValueWindow[LFRMCOL], SideWindowEntryBuffer);
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LFRMCOL:

				if (value) {

					savdo();
					nufilcol((atoi(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText(ValueWindow[LFRMCOL], SideWindowEntryBuffer);
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LUNDCOL:

				if (value) {

					savdo();
					SelectedForm->underlayColor = (atoi(SideWindowEntryBuffer) - 1) & 0xf;
					SetWindowText(ValueWindow[LUNDCOL], SideWindowEntryBuffer);
					refilfn();
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LBRDCOL:

				if (value) {

					savdo();
					nubrdcol((atoi(SideWindowEntryBuffer) - 1) & 0xf);
					SetWindowText(ValueWindow[LFRMCOL], SideWindowEntryBuffer);
					coltab();
				}
				unsid();
				setMap(RESTCH);
				return;

			case LBRDPIC:

				savdo();
				SelectedForm->edgeSpacing = value;
				refil();
				return;

			case LFRMFAZ:

				savdo();
				SelectedForm->wordParam = floor(value) / PFGRAN;
				unsid();
				refil();
				return;

			case LBRDPOS:

				savdo();
				SelectedForm->edgeStitchLen = value / PFGRAN;
				unsid();
				refil();
				return;

			case LMAXFIL:

				savdo();
				SelectedForm->maxFillStitchLen = value;
				unsid();
				refil();
				return;

			case LMINFIL:

				savdo();
				SelectedForm->minFillStitchLen = value;
				unsid();
				refil();
				return;

			case LMAXBRD:

				savdo();
				SelectedForm->maxBorderStitchLen = value;
				unsid();
				refil();
				return;

			case LMINBRD:

				savdo();
				SelectedForm->minBorderStitchLen = value;
				unsid();
				refil();
				return;
			}
			if (FormMenuChoice == LBCSIZ) {

				savdo();
				if (SelectedForm->edgeType == EDGEBHOL)
					savblen((float)value);
				else
					savplen((float)value);
			} else {

				if (value) {

					switch (FormMenuChoice) {

					case LFTHSIZ:

						savdo();
						SelectedForm->fillInfo.feather.ratio = value / PFGRAN;
						break;

					case LFTHNUM:

						savdo();
						SelectedForm->fillInfo.feather.count = value / PFGRAN;
						break;

					case LFTHFLR:

						savdo();
						SelectedForm->fillInfo.feather.minStitchSize = value;
						break;

					case LFTHDWNCNT:

						savdo();
						value /= PFGRAN;
						if (value > 255)
							value = 255;
						SelectedForm->fillInfo.feather.downCount = value;
						break;

					case LFRMSPAC:

						savdo();
						SelectedForm->fillSpacing = value;
						break;

					case LFRMLEN:

						savdo();
						SelectedForm->lengthOrCount.stitchLength = value;
						break;

					case LBRDSPAC:

						savdo();
						edgeType = SelectedForm->edgeType&NEGUND;
						switch (edgeType) {

						case EDGEPROPSAT:

							SelectedForm->edgeSpacing = value;
							break;

						case EDGEOCHAIN:
						case EDGELCHAIN:

							SelectedForm->edgeSpacing = value;
							break;

						default:

							SelectedForm->edgeSpacing = value / 2;
						}
						break;

					case LBRDLEN:

						savdo();
						SelectedForm->edgeStitchLen = value;
						break;

					case LBRDSIZ:

						savdo();
						SelectedForm->borderSize = value;
						break;

					case LFRMANG:

						savdo();
						SelectedForm->angleOrClipData.angle = value / 180 * PI / PFGRAN;
						break;

					case LSACANG:

						savdo();
						SelectedForm->satinOrAngle.angle = value / 180 * PI / PFGRAN;
						break;

					case LAPCOL:

						savdo();
						SelectedForm->borderColor &= 0xf;
						borderColor = value / 6;
						if (borderColor)
							borderColor--;
						borderColor &= 0xf;
						SelectedForm->borderColor |= (borderColor << 4);
						break;
					}
				} else {

					savdo();
					if (FormMenuChoice == LFRMSPAC&&isfclp())
						SelectedForm->fillSpacing = 0;
				}
			}
			unsid();
			refil();
		} else {

			if (PreferenceIndex) {

				value = atof(SideWindowEntryBuffer);
				switch (PreferenceIndex - 1) {

				case PEG:

					IniFile.eggRatio = value;
					sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", value);
					SetWindowText(ValueWindow[PEG], MsgBuffer);
					break;

				case PNUDG:

					IniFile.cursorNudgeStep = value;
					IniFile.nudgePixels = pxchk(value);
					sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", value);
					SetWindowText(ValueWindow[PNUDG], MsgBuffer);
					break;

				case PPIC:

					PicotSpacing = value*PFGRAN;
					sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", value);
					SetWindowText(ValueWindow[PPIC], MsgBuffer);
					break;

				case PCLPOF:

					IniFile.clipOffset = value*PFGRAN;
					sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f mm", value);
					SetWindowText(ValueWindow[PCLPOF], MsgBuffer);
					break;

				case PFAZ:

					IniFile.fillPhase = floor(value);
					sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", IniFile.fillPhase);
					SetWindowText(ValueWindow[PFAZ], MsgBuffer);
					break;

				case PCHRAT:

					IniFile.chainRatio = value;
					sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.chainRatio);
					SetWindowText(ValueWindow[PCHRAT], MsgBuffer);
					break;

				case PMIN:

					MinStitchLength = value*PFGRAN;
					sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", value);
					SetWindowText(ValueWindow[PMIN], MsgBuffer);
					break;

				default:

					if (value) {

						sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", value);
						switch (PreferenceIndex - 1) {

						case PSPAC:

							StitchSpacing = value*PFGRAN;
							SetWindowText(ValueWindow[PSPAC], MsgBuffer);
							break;

						case PANGL:

							IniFile.fillAngle = value / 180 * PI;
							SetWindowText(ValueWindow[PANGL], MsgBuffer);
							break;

						case PSAT:

							BorderWidth = value*PFGRAN;
							IniFile.borderWidth = BorderWidth;
							SetWindowText(ValueWindow[PSAT], MsgBuffer);
							break;

						case PMAX:

							IniFile.maxStitchLength = value*PFGRAN;
							SetWindowText(ValueWindow[PMAX], MsgBuffer);
							break;

						case PUSE:

							UserStitchLength = value*PFGRAN;
							SetWindowText(ValueWindow[PUSE], MsgBuffer);
							break;

						case PSMAL:

							SmallStitchLength = value*PFGRAN;
							SetWindowText(ValueWindow[PSMAL], MsgBuffer);
							break;

						case PAP:

							UnderlayColor = (unsigned)(value - 1) % 16;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", UnderlayColor + 1);
							SetWindowText(ValueWindow[PAP], MsgBuffer);
							break;

						case PSNP:

							SnapLength = value*PFGRAN;
							SetWindowText(ValueWindow[PSNP], MsgBuffer);
							break;

						case PSTAR:

							StarRatio = value;
							if (StarRatio > 1)
								StarRatio = 1;
							if (StarRatio < 0.05)
								StarRatio = 0.05;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", StarRatio);
							SetWindowText(ValueWindow[PSTAR], MsgBuffer);
							break;

						case PSPIR:

							SpiralWrap = value;
							if (StarRatio > 20)
								StarRatio = 20.;
							if (StarRatio < 0.3)
								StarRatio = 0.3;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SpiralWrap);
							SetWindowText(ValueWindow[PSPIR], MsgBuffer);
							break;

						case PBUT:

							ButtonholeCornerLength = value*PFGRAN;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", value);
							SetWindowText(ValueWindow[PBUT], MsgBuffer);
							break;

						case PHUPX:

							IniFile.hoopSizeX = value*PFGRAN;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.0f mm", value);
							SetWindowText(ValueWindow[PHUPX], MsgBuffer);
							sethup();
							prfmsg();
							chkhup();
							break;

						case PHUPY:

							IniFile.hoopSizeY = value*PFGRAN;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.0f mm", value);
							SetWindowText(ValueWindow[PHUPY], MsgBuffer);
							sethup();
							prfmsg();
							chkhup();
							break;

						case PGRD:

							IniFile.gridSize = value*PFGRAN;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f mm", value);
							SetWindowText(ValueWindow[PGRD], MsgBuffer);
							break;

						case PCHN:

							IniFile.chainSpace = value*PFGRAN;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", value);
							SetWindowText(ValueWindow[PCHN], MsgBuffer);
							break;
						}
					}
				}
				unsid();
				PreferenceIndex = 0;
			} else {

				value = atof(MsgBuffer);
				if (rstMap(SCLPSPAC))
					IniFile.clipOffset = value*PFGRAN;
				if (rstMap(FSETFIND))
					dufind(value);
				if (rstMap(FSETFHI))
				{
					if (value)
						dufhi(value*PFGRAN);
				}
				if (rstMap(FSETFWID))
				{
					if (value)
						dufwid(value*PFGRAN);
				}
				if (rstMap(FSETFMAX))
				{
					if (value)
						dufmax(value*PFGRAN);
				}
				if (rstMap(FSETFMIN))
					dufmin(value*PFGRAN);
				if (rstMap(FSETBMAX))
				{
					if (value)
						dubmax(value*PFGRAN);
				}
				if (rstMap(FSETBMIN))
					dubmin(value*PFGRAN);
				if (rstMap(FSETBSPAC))
				{
					if (value)
						dubspac(value*PFGRAN);
				}
				if (rstMap(FSETFLEN))
				{
					if (value)
						dublen(value*PFGRAN);
				}
				if (rstMap(FSETBCOL))
					dubcol(value);
				if (rstMap(FSETFCOL))
					dufcol(value);
				if (rstMap(FSETUCOL))
					dundcol(value);
				if (rstMap(FSETFANG))
					dufxang(value);
				if (rstMap(FSETFSPAC))
				{
					if (value)
						dufspac(value*PFGRAN);
				}
				if (rstMap(FSETUANG))
					dufang(value);
				if (rstMap(FSETFLEN))
				{
					if (value)
						duflen(value*PFGRAN);
				}
				if (rstMap(FSETUSPAC))
				{
					if (value)
						duspac(value*PFGRAN);
				}
				if (rstMap(FSETULEN))
				{
					if (value)
						dusulen(value*PFGRAN);
				}
				if (rstMap(GTUANG))
					IniFile.underlayAngle = value / 180 * PI;
				if (rstMap(GTUSPAC))
				{
					if (value)
						IniFile.underlaySpacing = value*PFGRAN;
				}
				if (rstMap(GTWLKIND))
					IniFile.underlayIndent = value*PFGRAN;
				if (rstMap(GTWLKLEN))
				{
					if (value)
						IniFile.underlayStitchLen = value*PFGRAN;
				}
				if (rstMap(PIXIN))
					IniFile.nudgePixels = pxchk(value);
				if (rstMap(STPXIN))
					IniFile.stitchSizePixels = pxchk(value);
				if (rstMap(FRMPXIN))
					IniFile.formVertexSizePixels = value;
				if (rstMap(FRMBOXIN))
					IniFile.formBoxSizePixels = value;
				if (rstMap(GETMIN))
					SmallStitchLength = value*PFGRAN;
				if (rstMap(ENTR30))
					ThreadSize30 = value;
				if (rstMap(ENTR40))
					ThreadSize40 = value;
				if (rstMap(ENTR60))
					ThreadSize60 = value;
				if (rstMap(ENTRFNUM)) {

					if (value < FormIndex)
						frmnumfn(value);
					else
						tabmsg(IDS_FRMN1);
					return;
				}
				value = floor(value);
				if (rstMap(ENTRPOL))
					durpoli(value);
				if (rstMap(ENTRSTAR))
					dustar(value, 250 / value*ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY));
				if (rstMap(ENTRSPIR))
					duspir(value);
				if (rstMap(ENTRHART))
					duhart(value);
				if (rstMap(ENTRLENS))
					dulens(value);
				if (rstMap(ENTREG))
					dueg(value);
				if (rstMap(ENTRZIG))
					duzig(value);
			}
		}
	}
	setMap(RESTCH);
}

void noMsg() {

	if (DestroyWindow(MsgWindow))
		MsgWindow = 0;
	DestroyWindow(OKButton);
	DestroyWindow(DiscardButton);
	DestroyWindow(CancelButton);
	DestroyWindow(DeleteStitchesDialog);
	if (rstMap(NUMIN) || FormMenuChoice || PreferenceIndex)
		chknum();
	rstMap(MOVMSG);
	rstMap(DELFRM);
	rstMap(FILMSG);
	setMap(RESTCH);
}

void stchWnd() {

	stchPars();

	MainStitchWin = CreateWindow(
		"STATIC",
		0,
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		0,
		StitchWindowSize.x,
		StitchWindowSize.y,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);

	GetWindowRect(MainStitchWin, &StitchWindowAbsRect);

	VerticalScrollBar = CreateWindow(
		"SCROLLBAR",
		0,
		SBS_VERT | WS_CHILD | WS_VISIBLE,
		StitchWindowSize.x + ButtonWidthX3,
		0,
		SCROLSIZ,
		StitchWindowSize.y,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);

	HorizontalScrollBar = CreateWindow(
		"SCROLLBAR",
		0,
		SBS_HORZ | WS_CHILD | WS_VISIBLE,
		ButtonWidthX3,
		StitchWindowSize.y,
		StitchWindowSize.x,
		SCROLSIZ,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
}

//check if a click occurred in A vertical set of 16 windows
// and calculate which window had the click
unsigned chkMsgs(POINT clickCoord, HWND topWindow, HWND bottomWindow) {

	RECT	topRect;
	RECT	bottomRect;

	GetWindowRect(topWindow, &topRect);
	GetWindowRect(bottomWindow, &bottomRect);

	if (clickCoord.x > topRect.left&&clickCoord.x<bottomRect.right&&clickCoord.y>topRect.top&&clickCoord.y < bottomRect.bottom) {

		VerticalIndex = 15 - (bottomRect.bottom - clickCoord.y) / ButtonHeight;
		return 1;
	} else
		return 0;
}

HWND nuSiz(unsigned iThreadSize) {

	TCHAR*	str[] = { "30","40","60" };

	return CreateWindow(
		"STATIC",
		str[iThreadSize],
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		ButtonHeight*(iThreadSize + VerticalIndex),
		ButtonWidth,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void delstch1(unsigned iStitch) {

	if (PCSHeader.stitchCount) {

		while (iStitch <= PCSHeader.stitchCount) {

			StitchBuffer[iStitch].attribute = StitchBuffer[iStitch + 1].attribute;
			StitchBuffer[iStitch].x = StitchBuffer[iStitch + 1].x;
			StitchBuffer[iStitch].y = StitchBuffer[iStitch + 1].y;
			iStitch++;
		}
		PCSHeader.stitchCount--;
		if (ClosestPointIndex > (unsigned)PCSHeader.stitchCount - 1)
			ClosestPointIndex = PCSHeader.stitchCount - 1;
	}
}

void stchred(unsigned count, fPOINTATTR* source) {

#if  __UseASM__
	_asm {

		mov		edi, offset StitchBuffer
		mov		esi, source
		mov		ecx, count
		rep		movsd
	}
#else
	memcpy(StitchBuffer, source, count * 4);
#endif
}

void redbak() {

	BAKHED*		undoData;
	unsigned	iColor;

	undoData = (BAKHED*)UndoBuffer[UndoBufferWriteIndex];
	PCSHeader.stitchCount = undoData->stitchCount;
	if (PCSHeader.stitchCount)
		stchred((sizeof(fPOINTATTR)*PCSHeader.stitchCount) >> 2, undoData->stitches);
	UnzoomedRect.x = undoData->zoomRect.x;
	UnzoomedRect.y = undoData->zoomRect.y;
	FormIndex = undoData->formCount;
	//	for(nextBufferIndex=0;nextBufferIndex<FormIndex;nextBufferIndex++)
	//		frmcpy(&FormList[nextBufferIndex],&undoData->forms[nextBufferIndex]);
	MoveMemory(&FormList, undoData->forms, sizeof(FRMHED)*FormIndex);
	FormVertexIndex = undoData->vertexCount;
	if (FormVertexIndex)
		mvflpnt(&FormVertices[0], &undoData->vertices[0], FormVertexIndex);
	SatinConnectIndex = undoData->satinConnectionCount;
	if (SatinConnectIndex)
		mvsatk(&SatinConnects[0], &undoData->satinConnection[0], SatinConnectIndex);
	ClipPointIndex = undoData->clipPointCount;
	if (ClipPointIndex)
		mvflpnt(&ClipPoints[0], &undoData->clipPoints[0], ClipPointIndex);
	MoveMemory(&UserColor, undoData->colors, sizeof(COLORREF) * 16);
	for (iColor = 0; iColor < 16; iColor++) {

		UserPen[iColor] = nuPen(UserPen[iColor], 1, UserColor[iColor]);
		UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	}
	for (iColor = 0; iColor < 16; iColor++)
		redraw(UserColorWin[iColor]);
	TextureIndex = undoData->texturePointCount;
	if (TextureIndex)
		MoveMemory(&TexturePointsBuffer, undoData->texturePoints, sizeof(TXPNT)*TextureIndex);
	coltab();
	setMap(RESTCH);
}

void redo() {

	unsigned nextBufferIndex;

	if (chkMap(BAKACT) && chkMap(REDUSHO)) {

		UndoBufferWriteIndex++;
		UndoBufferWriteIndex &= 0xf;
		nextBufferIndex = (UndoBufferWriteIndex + 1) & 0xf;
		if (nextBufferIndex == UndoBufferReadIndex) {

			EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
			rstMap(REDUSHO);
		} else {

			if (!setMap(REDUSHO))
				EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
		}
		if (!setMap(UNDUSHO)) {

			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_ENABLED);
		}
		redbak();
		setMap(DUMEN);
	}
}

void bak() {

	unsigned previousBufferIndex;

	unmsg();
	rstMap(FPSEL);
	rstMap(FRMPSEL);
	rstMap(BIGBOX);
	if (rstMap(PRFACT)) {

		rstMap(WASRT);
		DestroyWindow(PreferencesWindow);
		PreferenceIndex = 0;
		unsid();
	}
	if (!setMap(BAKING)) {

		dudat();
		UndoBufferReadIndex = UndoBufferWriteIndex + 1;
	}
	if (chkMap(BAKWRAP)) {

		UndoBufferWriteIndex--;
		UndoBufferWriteIndex &= 0xf;
		previousBufferIndex = UndoBufferWriteIndex - 1;
		if (previousBufferIndex == UndoBufferReadIndex) {

			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			setMap(DUMEN);
			rstMap(UNDUSHO);
		}
	} else {

		if (UndoBufferWriteIndex)
			UndoBufferWriteIndex--;
		if (!UndoBufferWriteIndex) {

			EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
			setMap(DUMEN);
			rstMap(UNDUSHO);
		}
	}
	if (!setMap(REDUSHO)) {

		EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_ENABLED);
		setMap(DUMEN);
	}
	rstMap(FORMSEL);
	rstMap(GRPSEL);
	redbak();
}

void bitsiz() {

	double				screenAspectRatio;
	double				bitmapAspectRatio;

	bitmapAspectRatio = (double)BitmapWidth / BitmapHeight;
	screenAspectRatio = (double)UnzoomedRect.x / UnzoomedRect.y;
	if (bitmapAspectRatio > screenAspectRatio) {

		BitmapSizeinStitches.x = UnzoomedRect.x;
		BitmapSizeinStitches.y = UnzoomedRect.x / bitmapAspectRatio;
	} else {

		BitmapSizeinStitches.y = UnzoomedRect.y;
		BitmapSizeinStitches.x = UnzoomedRect.y*bitmapAspectRatio;
	}
	BmpStitchRatio.x = (double)BitmapWidth / BitmapSizeinStitches.x;
	BmpStitchRatio.y = (double)BitmapHeight / BitmapSizeinStitches.y;
	StitchBmpRatio.x = (double)BitmapSizeinStitches.x / BitmapWidth;
	StitchBmpRatio.y = (double)BitmapSizeinStitches.y / BitmapHeight;
}

// Get a rough estimate of whether black or white 
// is dominant in the monochrome bitmap
BOOL binv(unsigned bitmapWidthInWords) {

	unsigned	iHeight, iBytes, whiteBits, blackBits, byteCount;
	TCHAR*		bcpnt;

	whiteBits = blackBits = 0;
	byteCount = BitmapWidth >> 3;
	for (iHeight = 0; iHeight < BitmapHeight; iHeight++) {

		bcpnt = (TCHAR*)&MonoBitmapData[bitmapWidthInWords*iHeight];
		for (iBytes = 0; iBytes < byteCount; iBytes++) {

			if (!bcpnt[iBytes])
				blackBits++;
			else {

				if (bcpnt[iBytes] == (TBYTE)0xff)
					whiteBits++;
			}
		}
	}
	if (whiteBits > blackBits)
		return 1;
	else
		return 0;
}

void bitlin(unsigned* source, unsigned* destination, COLORREF foreground, COLORREF background) {

#if  __UseASM__
	_asm {

		mov		esi, source
		mov		edi, destination
		mov		ecx, BitmapWidth
		xor		eax, eax

blup :	mov		ebx, eax
		xor		bl, 7
		mov		edx, background
		bt[esi], ebx
		jnc		short blup1
		mov		edx, foreground

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

	for (unsigned i = 0; i < BitmapWidth; i++) {
		bit.dword0 = i;
		bit.bytes.byte0 = bit.bytes.byte0 ^ 0x07;

		*destination = (_bittest((long *)source, bit.dword0)) ? foreground : background;

		destination++;
	}
#endif
}

COLORREF fswap(COLORREF color) {

#if  __UseASM__
	_asm {

		mov		eax, color
		bswap	eax
		shr		eax, 8
	}
#else
	return _byteswap_ulong(color) >> 8;
#endif
}

BOOL gudtyp(WORD bitCount) {

	switch (bitCount) {

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
		mov		esi, TraceBitmapData
		mov		edi, offset BSequence
		movlup : mov		eax, [esi]
				 add		esi, 4
				 mov[edi], eax
				 add		edi, 3
				 loop	movlup
	}
#else
	// ToDo check translation
	unsigned *source = TraceBitmapData;
	char *destination = (char *)BSequence;

	for (unsigned i = 0; i < cnt; i++) {
		*(unsigned *)destination = *(source++);

		destination += 3;
	}
#endif
}

void savmap() {

	unsigned long		bytesWritten;

	if (*PCSBMPFileName) {

		if (chkMap(MONOMAP)) {

			tabmsg(IDS_SAVMAP);
			return;
		}
		if (!chkMap(WASTRAC)) {

			tabmsg(IDS_MAPCHG);
			return;
		}
		if (GetSaveFileName(&OpenBitmapName)) {

			BitmapFileHandle = CreateFile(UserBMPFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
			if (IniFileHandle == INVALID_HANDLE_VALUE) {

				crmsg(UserBMPFileName);
				return;
			}
			WriteFile(BitmapFileHandle, (BITMAPFILEHEADER*)&BitmapFileHeader, 14, &bytesWritten, NULL);
			WriteFile(BitmapFileHandle, (BITMAPV4HEADER*)&BitmapFileHeaderV4, BitmapFileHeader.bfOffBits - 14, &bytesWritten, NULL);
			movmap(BitmapWidth*BitmapHeight);
			WriteFile(BitmapFileHandle, (BSEQPNT*)BSequence, BitmapWidth*BitmapHeight * 3, &bytesWritten, NULL);
			CloseHandle(BitmapFileHandle);
		}
	} else
		tabmsg(IDS_SHOMAP);
}

void bfil() {

	unsigned			bitmapWidthWords, widthOverflow, fileHeaderSize, bitmapSizeWords, iHeight;
	unsigned*			pbits;
	HBITMAP				bitmap;
	HDC					deviceContext;
	COLORREF			foreground;
	COLORREF			background;
	COLORREF			InverseBackgroundColor;

	InverseBackgroundColor = fswap(BackgroundColor);
	BitmapFileHandle = CreateFile(UserBMPFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (BitmapFileHandle == INVALID_HANDLE_VALUE) {

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Couldn't open bitmap file: %s\n", UserBMPFileName);
		shoMsg(MsgBuffer);
		CloseHandle(BitmapFileHandle);
		BitmapFileHandle = 0;
		*PCSBMPFileName = 0;
		return;
	}
	ReadFile(BitmapFileHandle, (BITMAPFILEHEADER*)&BitmapFileHeader, 14, &BytesRead, NULL);
	if (BitmapFileHeader.bfType == 'MB') {

		fileHeaderSize = BitmapFileHeader.bfOffBits - 14;
		if (fileHeaderSize > sizeof(BITMAPV4HEADER))
			fileHeaderSize = sizeof(BITMAPV4HEADER);
		ReadFile(BitmapFileHandle, (BITMAPV4HEADER*)&BitmapFileHeaderV4, fileHeaderSize, &BytesRead, NULL);
	} else {

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%s is not a Windows Bitmap\n", UserBMPFileName);
		CloseHandle(BitmapFileHandle);
		BitmapFileHandle = 0;
		*PCSBMPFileName = 0;
		return;
	}
	if (gudtyp(BitmapFileHeaderV4.bV4BitCount)) {

		if (!rstMap(WASESC))
			rstMap(TRSET);
		BitmapWidth = BitmapFileHeaderV4.bV4Width;
		BitmapHeight = BitmapFileHeaderV4.bV4Height;
		setMap(INIT);
		ZoomRect.left = ZoomRect.bottom = 0;
		ZoomRect.right = UnzoomedRect.x;
		ZoomRect.top = UnzoomedRect.y;
		BitmapDC = CreateCompatibleDC(StitchWindowDC);
		if (BitmapFileHeaderV4.bV4BitCount == 1) {

			setMap(MONOMAP);
			bitmapWidthWords = BitmapWidth >> 5;
			widthOverflow = BitmapWidth % 32;
			if (widthOverflow)
				bitmapWidthWords++;
			bitmapSizeWords = bitmapWidthWords*BitmapHeight;
			MonoBitmapData = new unsigned[bitmapSizeWords];
			ReadFile(BitmapFileHandle, (unsigned*)MonoBitmapData, bitmapSizeWords << 2, &BytesRead, NULL);
			CloseHandle(BitmapFileHandle);
			if (binv(bitmapWidthWords)) {

				background = BitmapColor;
				foreground = InverseBackgroundColor;
			} else {

				foreground = BitmapColor;
				background = InverseBackgroundColor;
			}
			memset((BITMAPINFOHEADER*)&BitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
			BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
			BitmapInfoHeader.biWidth = BitmapWidth;
			BitmapInfoHeader.biHeight = BitmapHeight;
			BitmapInfoHeader.biPlanes = 1;
			BitmapInfoHeader.biBitCount = 32;
			BitmapInfoHeader.biCompression = BI_RGB;
			BitmapInfo.bmiHeader = BitmapInfoHeader;
			bitmap = CreateDIBSection(BitmapDC, &BitmapInfo, DIB_RGB_COLORS, (void**)&pbits, 0, 0);
			// ToDo - Do I need to call GDIFlush() before or in this loop?
			for (iHeight = 0; iHeight < BitmapHeight; iHeight++)
				bitlin(&MonoBitmapData[iHeight*bitmapWidthWords], &pbits[iHeight*BitmapWidth], background, foreground);
			deviceContext = CreateCompatibleDC(StitchWindowDC);
			if (bitmap && deviceContext) {
				SelectObject(deviceContext, bitmap);
				BitmapFileHandle = CreateCompatibleBitmap(StitchWindowDC, BitmapWidth, BitmapHeight);
				SelectObject(BitmapDC, BitmapFileHandle);
				BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, deviceContext, 0, 0, SRCCOPY);
				DeleteObject(bitmap);
				ReleaseDC(ThrEdWindow, deviceContext);
			}
			delete[] MonoBitmapData;
		} else {

			CloseHandle(BitmapFileHandle);
			rstMap(MONOMAP);
			BitmapFileHandle = (HBITMAP)LoadImage(ThrEdInstance, UserBMPFileName, IMAGE_BITMAP, BitmapWidth, BitmapHeight, LR_LOADFROMFILE);
			SelectObject(BitmapDC, BitmapFileHandle);
			setMap(RESTCH);
		}
		bitsiz();
		UpPixelColor = 0;
		DownPixelColor = 0x7f7f7f;
		InvertUpColor = 0xffffff;
		InvertDownColor = 0x808080;
		rstMap(HIDMAP);
	} else {

		CloseHandle(BitmapFileHandle);
		BitmapFileHandle = 0;
		*PCSBMPFileName = 0;
		tabmsg(IDS_BMAP);
	}
}

void prtred() {

	CloseHandle(FileHandle);
	rstMap(INIT);
	FormIndex = 0;
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

	TCHAR*	extentionLocation;

	strcpy_s(ColorFileName, WorkingFileName);
	strcpy_s(RGBFileName, WorkingFileName);
	extentionLocation = strrchr(ColorFileName, '.');
	if (extentionLocation) {

		extentionLocation++;
		strcpy_s(extentionLocation, sizeof(ColorFileName) - (extentionLocation - ColorFileName), "thw");
		// ToDo - do we have to search again?
		extentionLocation = strrchr(RGBFileName, '.');
		extentionLocation++;
		strcpy_s(extentionLocation, sizeof(RGBFileName) - (extentionLocation - RGBFileName), "rgb");
		return 1;
	} else
		return 0;
}

void dstran() {

	unsigned		iRecord, iStitch, color;
	POINT			localStitch;
	POINT			dstStitch;
	fPOINT			maximumCoordinate;
	fPOINT			mimimumCoordinate;
	fPOINT			dstSize;
	fPOINT			delta;
	HANDLE			colorFile;
	unsigned*		colors;
	unsigned		iColor;
	DWORD			bytesRead;
	LARGE_INTEGER	colorFileSize;
	BOOL			retval;

	colors = 0;
	if (colfil()) {

		colorFile = CreateFile(ColorFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (colorFile != INVALID_HANDLE_VALUE) {

			retval = GetFileSizeEx(colorFile, &colorFileSize);
			// ToDo - check HighPart is non-zero
			colors = new unsigned[colorFileSize.u.LowPart];
			ReadFile(colorFile, (unsigned*)colors, colorFileSize.u.LowPart, &bytesRead, 0);
			CloseHandle(colorFile);
			if (bytesRead > 1) {
				if (bytesRead&&colors&&colors[0] == COLVER) {

					BackgroundColor = colors[1];
					ColorChanges = 0;
				}
			} else {

				if (colors) {

					delete[] colors;
					colors = 0;
				}
			}
		}
	}
	iStitch = 0;
	if (colors)
		color = colmatch(colors[2]);
	else
		color = 0;
	iColor = 3;
	localStitch.x = localStitch.y = 0;
	maximumCoordinate.x = maximumCoordinate.y = (float)-1e12;
	mimimumCoordinate.x = mimimumCoordinate.y = (float)1e12;
	for (iRecord = 0; iRecord < DSTRecordCount; iRecord++) {

		if (DSTRecords[iRecord].nd & 0x40) {

			if (colors)
				color = colmatch(colors[iColor++]);
			else {

				color++;
				color &= 0xf;
			}
		} else {

			dstin(dtrn(&DSTRecords[iRecord]), &dstStitch);
			// ToDo - Is localStitch Required?
			localStitch.x += dstStitch.x;
			localStitch.y += dstStitch.y;
			if (!(DSTRecords[iRecord].nd & 0x80)) {

				StitchBuffer[iStitch].attribute = color | NOTFRM;
				StitchBuffer[iStitch].x = localStitch.x*0.6;
				StitchBuffer[iStitch].y = localStitch.y*0.6;
				if (StitchBuffer[iStitch].x > maximumCoordinate.x)
					maximumCoordinate.x = StitchBuffer[iStitch].x;
				if (StitchBuffer[iStitch].y > maximumCoordinate.y)
					maximumCoordinate.y = StitchBuffer[iStitch].y;
				if (StitchBuffer[iStitch].x < mimimumCoordinate.x)
					mimimumCoordinate.x = StitchBuffer[iStitch].x;
				if (StitchBuffer[iStitch].y < mimimumCoordinate.y)
					mimimumCoordinate.y = StitchBuffer[iStitch].y;
				iStitch++;
			}
		}
	}
	if (colors)
		delete[] colors;
	PCSHeader.stitchCount = iStitch;
	dstSize.x = maximumCoordinate.x - mimimumCoordinate.x;
	dstSize.y = maximumCoordinate.y - mimimumCoordinate.y;
	IniFile.hoopType = CUSTHUP;
	UnzoomedRect.x = IniFile.hoopSizeX;
	UnzoomedRect.y = IniFile.hoopSizeY;
	if (dstSize.x > UnzoomedRect.x || dstSize.y > UnzoomedRect.y) {

		IniFile.hoopSizeX = UnzoomedRect.x = dstSize.x*1.1;
		IniFile.hoopSizeY = UnzoomedRect.y = dstSize.y*1.1;
		hsizmsg();
	}
	delta.x = (UnzoomedRect.x - dstSize.x) / 2 - mimimumCoordinate.x;
	delta.y = (UnzoomedRect.y - dstSize.y) / 2 - mimimumCoordinate.y;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		StitchBuffer[iStitch].x += delta.x;
		StitchBuffer[iStitch].y += delta.y;
	}
}

BOOL chkdst(DSTHED* dstHeader) {

	if (strncmp(dstHeader->desched, "LA:", 3))
		return 0;
	return 1;
}

void auxmen() {

	switch (IniFile.auxFileType) {

	case AUXDST:

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_AUXTXT], "DST");
		CheckMenuItem(MainMenu, ID_AUXDST, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_AUXPCS, MF_UNCHECKED);
		break;

	default:

		CheckMenuItem(MainMenu, ID_AUXDST, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_AUXPCS, MF_CHECKED);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_AUXTXT], "PCS");
	}
	SetMenuItemInfo(FileMenu, ID_OPNPCD, MF_BYCOMMAND, &filinfo);
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

unsigned pesmtch(COLORREF referenceColor, unsigned char colorIndex) {

	unsigned	colorDistance, iRGB;
	COLORREF	color, translatedColor;

	color = referenceColor;
	translatedColor = PESColorTranslate[colorIndex];
	colorDistance = 0;
	for (iRGB = 0; iRGB < 3; iRGB++) {

		colorDistance += ((color & 0xff) > (translatedColor & 0xff)) ? ((color & 0xff) - (translatedColor & 0xff)) : ((translatedColor & 0xff) - (color & 0xff));
		translatedColor >>= 8;
		color >>= 8;
	}
	return colorDistance;
}

unsigned dupcol() {

	unsigned	iColor;
	COLORREF	color;
	unsigned	matchDistance, minimumDistance, matchIndex = 0;

	color = PESColorTranslate[PEScolors[PEScolorIndex++]];
	for (iColor = 0; iColor < ActivePointIndex; iColor++) {

		if (UserColor[iColor] == color)
			return iColor;
	}
	minimumDistance = 0xffffff;
	for (iColor = 1; iColor < ActivePointIndex; iColor++) {

		matchDistance = pesmtch(color, PEScolors[iColor]);
		if (matchDistance < minimumDistance) {

			minimumDistance = matchDistance;
			matchIndex = iColor;
		}
	}
	return matchIndex;
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

	unsigned	iForm;
	FRMHEDO*	formListCopy;

	formListCopy = (FRMHEDO*)&BSequence;
	FillMemory(&BSequence, 0, sizeof(FRMHED)*FormIndex);
	for (iForm = 0; iForm < FormIndex; iForm++)
		MoveMemory(&FormList[iForm], &formListCopy[iForm], sizeof(FRMHEDO));
}

void nuFil() {

	unsigned		fileSize, iStitch;
	unsigned		version;
	DWORD			fileSizeHigh;
	unsigned		textureHistoryFlag, pcsStitchCount;
	unsigned		iPCSstitch, iPESstitch, color, iColor;
	unsigned		iColorChange;
	TCHAR*			fileExtention;
	TCHAR			firstCharacter;
	STRHED			thredHeader;
	TCHAR			buffer[3];
	TCHAR*			tnam;
	DSTHED			dstHeader;
	fRECTANGLE		stitchRect;
	FRMHEDO*		formListCopy;
	long			totalBytesRead;

#if PESACT

	unsigned		iActualPESstitches;
	PESHED*			pesHeader;
	TCHAR*			l_peschr;
	unsigned		pecof;
	unsigned char*	pesColorCount;
	dPOINT			loc;
	PESTCH*			pabstch;
	unsigned		pabind;
	double			locof;
#endif

	if (rstMap(REDOLD) || GetOpenFileName(&OpenFileName)) {

		fnamtabs();
		untrace();
		if (FormIndex)
			delfrms();
		rstMap(ZUMED);
		rstMap(FRMOF);
		rstMap(HID);
		rstMap(BIGBOX);
		unthum();
		unbsho();
		if (PCSBMPFileName[0]) {

			DeleteObject(BitmapFileHandle);
			ReleaseDC(ThrEdWindow, BitmapDC);
			*PCSBMPFileName = 0;
		}
		FileHandle = CreateFile(WorkingFileName, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, 0, NULL);
		if (FileHandle == INVALID_HANDLE_VALUE) {

			if (GetLastError() == 32)
				filnopn(IDS_FNOPNA, WorkingFileName);
			else
				filnopn(IDS_FNOPN, WorkingFileName);
			FileHandle = 0;
		} else {

			rstMap(CMPDO);
			rstMap(SAVACT);
			rstMap(BAKING);
			rstMap(REDUSHO);
			TextureIndex = 0;
			EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
			deldu();
			strcpy_s(DesignerName, IniFile.designerName);
			unbsho();
			rstMap(MOVSET);
			frmon();
			SelectedFormCount = 0;
			if (rstMap(PRFACT)) {

				DestroyWindow(PreferencesWindow);
				rstMap(WASRT);
				PreferenceIndex = 0;
			}
			PCSBMPFileName[0] = 0;
			if (SearchLineIndex)
				SearchLineIndex = 0;
			rstMap(SCROS);
			rstMap(ECROS);
			rstMap(BZUMIN);
			rstdu();
			unmsg();
			UndoBufferWriteIndex = 0;
			rstMap(BAKWRAP);
			ZoomFactor = 1;
			setMap(RESTCH);
			defNam(WorkingFileName);
			NearestCount = 0;
			if (rstMap(WASPAT))
				DestroyWindow(SpeedScrollBar);
			textureHistoryFlag = 0;
			if (chkMap(WASTXBAK))
				textureHistoryFlag = 1;
			clrMap(MAPLEN);
			if (textureHistoryFlag)
				setMap(WASTXBAK);
			fileSize = GetFileSize(FileHandle, &fileSizeHigh);
			fileExtention = strrchr(WorkingFileName, '.');
			if (fileExtention)
				fileExtention++;
			else {

				strcat_s(WorkingFileName, ".thr");
				fileExtention = strrchr(WorkingFileName, '.') + 1;
			}
			firstCharacter = tolower(fileExtention[0]);
			if (firstCharacter == 't') {

				ReadFile(FileHandle, (STRHED*)&thredHeader, sizeof(STRHED), &BytesRead, NULL);
				if ((thredHeader.headerType & 0xffffff) == 0x746872) {

					if (BytesRead != sizeof(STRHED)) {

						tabmsg(IDS_SHRTF);
						return;
					}
					version = (thredHeader.headerType & 0xff000000) >> 24;
					switch (version) {

					case 0:

						if (PCSHeader.hoopType == SMALHUP) {

							UnzoomedRect.x = IniFile.hoopSizeX = SHUPX;
							UnzoomedRect.y = IniFile.hoopSizeY = SHUPY;
						} else {

							UnzoomedRect.x = IniFile.hoopSizeX = LHUPX;
							UnzoomedRect.y = IniFile.hoopSizeY = LHUPY;
							PCSHeader.hoopType = LARGHUP;
						}
						ritfnam(IniFile.designerName);
						strcpy_s(DesignerName, IniFile.designerName);
						strcpy_s(ExtendedHeader.modifierName, IniFile.designerName);
						break;

					case 1:
					case 2:

						ReadFile(FileHandle, (STREX*)&ExtendedHeader, sizeof(STREX), &BytesRead, NULL);
						if (BytesRead != sizeof(STREX)) {

							tabmsg(IDS_SHRTF);
							return;
						}
						IniFile.hoopSizeX = UnzoomedRect.x = ExtendedHeader.hoopSizeX;
						IniFile.hoopSizeY = UnzoomedRect.y = ExtendedHeader.hoopSizeY;
						redfnam(DesignerName);
						break;

					default:

						tabmsg(IDS_NOTVER);
						return;
					}
					ZoomRect.bottom = ZoomRect.left = 0;
					ZoomRect.right = UnzoomedRect.x = IniFile.hoopSizeX;
					ZoomRect.top = UnzoomedRect.y = IniFile.hoopSizeY;
					PCSHeader.stitchCount = thredHeader.stitchCount;
					ReadFile(FileHandle, (fPOINTATTR*)StitchBuffer, PCSHeader.stitchCount * sizeof(fPOINTATTR), &BytesRead, NULL);
					if (BytesRead != PCSHeader.stitchCount * sizeof(fPOINTATTR)) {

						PCSHeader.stitchCount = BytesRead / sizeof(fPOINTATTR);
						prtred();
						return;
					}
					ReadFile(FileHandle, (TCHAR*)PCSBMPFileName, 16, &BytesRead, 0);
					totalBytesRead = BytesRead;
					if (BytesRead != 16) {

						PCSBMPFileName[0] = 0;
						prtred();
						return;
					}
					ReadFile(FileHandle, (COLORREF*)&BackgroundColor, 4, &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != 4) {

						BackgroundColor = IniFile.backgroundColor;
						prtred();
						return;
					}
					BackgroundBrush = CreateSolidBrush(BackgroundColor);
					ReadFile(FileHandle, (COLORREF*)UserColor, 64, &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != 64) {

						prtred();
						return;
					}
					ReadFile(FileHandle, (COLORREF*)CustomColor, 64, &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != 64) {

						prtred();
						return;
					}
					ReadFile(FileHandle, (TCHAR*)MsgBuffer, 16, &BytesRead, 0);
					totalBytesRead += BytesRead;
					if (BytesRead != 16) {

						prtred();
						return;
					}
					for (int iThread = 0; iThread < 16; iThread++)
						ThreadSize[iThread][0] = MsgBuffer[iThread];
					FormIndex = thredHeader.formCount;
					if (FormIndex > MAXFORMS)
						FormIndex = MAXFORMS;
					pcsStitchCount = 0;
					if (FormIndex) {

						FormVertexIndex = SatinConnectIndex = ClipPointIndex = 0;
						MsgBuffer[0] = 0;
						if (version < 2) {

							formListCopy = (FRMHEDO*)&BSequence;
							ReadFile(FileHandle, (FRMHEDO*)formListCopy, FormIndex * sizeof(FRMHEDO), &BytesRead, 0);
							if (BytesRead != FormIndex * sizeof(FRMHEDO)) {

								FormIndex = BytesRead / sizeof(FRMHEDO);
								setMap(BADFIL);
							}
							xofrm();
						} else {

							ReadFile(FileHandle, (FRMHED*)FormList, FormIndex * sizeof(FRMHED), &BytesRead, 0);
							rstMap(BADFIL);
							if (BytesRead != FormIndex * sizeof(FRMHED)) {

								FormIndex = BytesRead / sizeof(FRMHED);
								setMap(BADFIL);
							}
						}
						//						nextBufferIndex=SetFilePointer(FileHandle,0,0,FILE_CURRENT);  //bug
						ReadFile(FileHandle, (fPOINT*)FormVertices, thredHeader.vertexCount * sizeof(fPOINT), &BytesRead, 0);
						if (BytesRead != sizeof(fPOINT)*thredHeader.vertexCount) {

							FormVertexIndex = BytesRead / sizeof(fPOINT);
							for (unsigned iVertex = FormVertexIndex; iVertex < thredHeader.vertexCount; iVertex++)
								FormVertices[iVertex].x = FormVertices[iVertex].y = 0;
							setMap(BADFIL);
						}
						ReadFile(FileHandle, (SATCON*)SatinConnects, thredHeader.dlineCount * sizeof(SATCON), &BytesRead, 0);
						if (BytesRead != thredHeader.dlineCount * sizeof(SATCON)) {

							SatinConnectIndex = BytesRead / sizeof(SATCON);
							setMap(BADFIL);
						}
						ReadFile(FileHandle, (fPOINT*)ClipPoints, thredHeader.clipDataCount * sizeof(fPOINT), &BytesRead, 0);
						if (BytesRead != thredHeader.clipDataCount * sizeof(fPOINT)) {

							ClipPointIndex = BytesRead / sizeof(fPOINT);
							setMap(BADFIL);
						}
						ReadFile(FileHandle, (TXPNT*)TexturePointsBuffer, ExtendedHeader.texturePointCount * sizeof(TXPNT), &BytesRead, 0);
						TextureIndex = BytesRead / sizeof(TXPNT);
						if (rstMap(BADFIL))
							bfilmsg();
						for (unsigned iForm = 0; iForm < FormIndex; iForm++) {

							FormList[iForm].vertices = adflt(FormList[iForm].sides);
							if (FormList[iForm].type == SAT) {

								if (FormList[iForm].satinGuideCount)
									FormList[iForm].satinOrAngle.sac = adsatk(FormList[iForm].satinGuideCount);
							}
							if (isclp(iForm))
								FormList[iForm].angleOrClipData.clip = adclp(FormList[iForm].lengthOrCount.clipCount);
							if (iseclpx(iForm))
								FormList[iForm].borderClipData = adclp(FormList[iForm].clipEntries);
						}
						setfchk();
					}
				} else
					tabmsg(IDS_NOTHR);
			} else {

				setMap(NOTHRFIL);
				if (firstCharacter == 'p') {

					if (tolower(fileExtention[01]) == 'c') {

						ReadFile(FileHandle, (PCSHEADER*)&PCSHeader, 0x46, &BytesRead, NULL);
						if (!fileSize) {

							filnopn(IDS_ZEROL, WorkingFileName);
							return;
						}
						if (PCSHeader.leadIn == 0x32 && PCSHeader.colorCount == 16) {

							for (iColor = 0; iColor < 16; iColor++)
								UserColor[iColor] = PCSHeader.colors[iColor];
							fileSize -= 0x46;
							pcsStitchCount = fileSize / sizeof(PCSTCH) + 2;
							PCSStitchBuffer = new PCSTCH[pcsStitchCount];
							ReadFile(FileHandle, PCSStitchBuffer, fileSize, &BytesRead, NULL);
							iStitch = 0;
							iColorChange = 0;
							color = 0;
							iPCSstitch = 0;
							while (iStitch < PCSHeader.stitchCount&&iPCSstitch < pcsStitchCount) {

								if (PCSStitchBuffer[iPCSstitch].tag == 3) {

									ColorChangeTable[iColorChange].colorIndex = PCSStitchBuffer[iPCSstitch].fx;
									ColorChangeTable[iColorChange++].stitchIndex = iStitch;
									color = NOTFRM | PCSStitchBuffer[iPCSstitch++].fx;
								} else {

									StitchBuffer[iStitch].x = PCSStitchBuffer[iPCSstitch].x + (float)PCSStitchBuffer[iPCSstitch].fx / 256;
									StitchBuffer[iStitch].y = PCSStitchBuffer[iPCSstitch].y + (float)PCSStitchBuffer[iPCSstitch].fy / 256;
									StitchBuffer[iStitch++].attribute = color;
									iPCSstitch++;
								}
							}
							PCSHeader.stitchCount = iStitch;
							// ToDo - Do these 2 lines make sense?
							tnam = (TCHAR*)&PCSStitchBuffer[iPCSstitch];
							strcpy_s(PCSBMPFileName, tnam);
							delete[] PCSStitchBuffer;
							strcpy_s(fileExtention, sizeof(WorkingFileName) - (fileExtention - WorkingFileName), "thr");
							IniFile.auxFileType = AUXPCS;
							if (PCSHeader.hoopType != LARGHUP&&PCSHeader.hoopType != SMALHUP)
								PCSHeader.hoopType = LARGHUP;
							sizstch(&stitchRect);
							if (stitchRect.left<0 || stitchRect.right>LHUPY || stitchRect.bottom<0 || stitchRect.top>LHUPY) {

								IniFile.hoopSizeX = LHUPX;
								IniFile.hoopSizeY = LHUPY;
								chkhup();
							} else {

								if (PCSHeader.hoopType == LARGHUP) {

									IniFile.hoopType = LARGHUP;
									IniFile.hoopSizeX = LHUPX;
									IniFile.hoopSizeY = LHUPY;
								} else {

									if (stitchRect.right > SHUPX || stitchRect.top > SHUPY || PCSHeader.hoopType == LARGHUP) {

										IniFile.hoopType = LARGHUP;
										IniFile.hoopSizeX = SHUPX;
										IniFile.hoopSizeY = SHUPY;
									} else {

										IniFile.hoopType = SMALHUP;
										IniFile.hoopSizeX = SHUPX;
										IniFile.hoopSizeY = SHUPY;
									}
								}
							}
						}
					}
#if PESACT
					else {

						ReadFile(FileHandle, (BSEQPNT*)&BSequence, sizeof(BSequence), &BytesRead, 0);
						pesHeader = (PESHED*)&BSequence;
						l_peschr = (TCHAR*)&BSequence;
						if (strncmp(pesHeader->led, "#PES00", 6)) {

							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Not a PES file: %s\n", WorkingFileName);
							shoMsg(MsgBuffer);
							return;
						}
						pecof = tripl(pesHeader->off);
						PESstitch = (unsigned char*)&l_peschr[pecof + 532];
						ActivePointIndex = 0;
						pesColorCount = (unsigned char*)&l_peschr[pecof + 48];
						PEScolors = &pesColorCount[1];
						MarkedStitchMap[0] = MarkedStitchMap[1] = 0;
						ActivePointIndex = 0;
						for (iColor = 0; iColor < (unsigned)(*pesColorCount + 1); iColor++) {

							if (setRmp(PEScolors[iColor])) {

								UserColor[ActivePointIndex++] = PESColorTranslate[PEScolors[iColor] & PESCMSK];
								if (ActivePointIndex >= 16)
									break;
							}
						}
						color = 0;
						PEScolorIndex = 1;
						loc.x = loc.y = 0;
						pabind = 0;
						rstMap(FILDIR);
						pabstch = (PESTCH*)&l_peschr[sizeof(PESHED) + 4];
						iPESstitch = 0;
						iActualPESstitches = 1;
						StitchBuffer[0].x = StitchBuffer[0].y;
						while (iPESstitch < BytesRead - pecof - 529) {

							if (PESstitch[iPESstitch] == 0xff && PESstitch[iPESstitch + 1] == 0)
								break;
							if (PESstitch[iPESstitch] == 0xfe && PESstitch[iPESstitch + 1] == 0xb0) {

								color = dupcol();
								iPESstitch += 2;
							} else {

								if (PESstitch[iPESstitch] & 0x80) {

									locof = dubl(&PESstitch[iPESstitch]);
									iPESstitch++;
								} else {

									if (PESstitch[iPESstitch] & 0x40)
										locof = PESstitch[iPESstitch] - 128;
									else
										locof = PESstitch[iPESstitch];
								}
								locof *= 0.6;
								if (toglMap(FILDIR)) {

									loc.y -= locof;
									StitchBuffer[iActualPESstitches].x = loc.x;
									StitchBuffer[iActualPESstitches].y = loc.y;
									StitchBuffer[iActualPESstitches].attribute = color;
									iActualPESstitches++;
								} else
									loc.x += locof;
							}
							iPESstitch++;
						}
						PCSHeader.stitchCount = iActualPESstitches;
						//IniFile.auxFileType=AUXPES;
						hupfn();
					}
#endif
				} else {

					ReadFile(FileHandle, (DSTHED*)&dstHeader, sizeof(DSTHED), &BytesRead, 0);
					if (BytesRead == sizeof(DSTHED)) {

						if (chkdst(&dstHeader)) {

							PCSBMPFileName[0] = 0;
							fileSize = GetFileSize(FileHandle, &BytesRead) - sizeof(DSTHED);
							DSTRecordCount = fileSize / sizeof(DSTREC);
							DSTRecords = new DSTREC[DSTRecordCount];
							ReadFile(FileHandle, (DSTREC*)DSTRecords, sizeof(DSTREC)*DSTRecordCount, &BytesRead, 0);
							dstran();
							delete[] DSTRecords;
							IniFile.auxFileType = AUXDST;
						}
					} else {

						tabmsg(IDS_DST2S);
						CloseHandle(FileHandle);
						return;
					}
				}
			}
			if (PCSBMPFileName[0]) {

				SetCurrentDirectory(DefaultDirectory);
				strcpy_s(UserBMPFileName, PCSBMPFileName);
				bfil();
			}
			ritot(PCSHeader.stitchCount);
			BufferIndex = 0;
			ZoomRect.left = 0;
			ZoomRect.bottom = 0;
			ZoomRect.right = UnzoomedRect.x = IniFile.hoopSizeX;
			ZoomRect.top = UnzoomedRect.y = IniFile.hoopSizeY;
			movStch();
			coltab();
			rstMap(ZUMED);
			buffer[2] = 0;
			for (iColor = 0; iColor < 16; iColor++) {

				UserPen[iColor] = nuPen(UserPen[iColor], 1, UserColor[iColor]);
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
				strncpy_s(buffer, ThreadSize[iColor], 2);
				SetWindowText(ThreadSizeWin[iColor], buffer);
			}
			for (iColor = 0; iColor < 16; iColor++)
				redraw(UserColorWin[iColor]);
			redraw(ColorBar);
			if (PCSHeader.stitchCount)
				setMap(INIT);
			nuAct(0);
			setMap(RESTCH);
			nunams();
			ritini();
			butxt(HNUM, "");
			if (PCSHeader.stitchCount)
				nuAct(StitchBuffer[0].attribute&COLMSK);
			else
				nuAct(0);
			auxmen();
		}
		lenCalc();
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRDBY], WorkingFileName, DesignerName);
		SetWindowText(ThrEdWindow, MsgBuffer);
		CloseHandle(FileHandle);
		setMap(INIT);
		rstMap(TRSET);
		if (chkMap(NOTHRFIL))
		{
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
				StitchBuffer[iStitch].attribute |= NOTFRM;
		}
		lodchk();
	}
}

void clrfbuf(unsigned count) {

#if  __UseASM__
	_asm {

		mov		edi, PCSStitchBuffer
		mov		ecx, count
		xor		eax, eax
		rep		stosd
	}
#else
	memset(PCSStitchBuffer, 0, count * 4);
#endif
}

unsigned dudbits(POINT dif) {

	return Xdst[dif.x + 121] | Ydst[dif.y + 121];
}

void savdst(unsigned data) {

#if  __UseASM__
	_asm {
		xor		eax, eax
		mov		al, 3
		mov		ecx, DSTRecordCount
		mul		ecx
		inc		ecx
		mov		DSTRecordCount, ecx
		add		eax, DSTRecords
		mov		ebx, data
		mov[eax], ebx
	}
#else
	union {
		unsigned data;
		DSTREC dstRecord;
	} x;

	x.data = data;

	DSTRecords[DSTRecordCount++] = x.dstRecord;
#endif
}

void ritdst() {

#define DSTMAX 121

	unsigned		iStitch, dstType, color, count, iColor;
	POINT			centerCoordinate, lengths, absoluteLengths, difference, stepSize;
	fRECTANGLE		boundingRect;
	fPOINTATTR*		highStitchBuffer;
	unsigned*		colorData;
	HANDLE			colorFile;
	unsigned long	bytesWritten;

	colorData = (unsigned*)&OSequence;
	iColor = 3;
	colorData[0] = COLVER;
	colorData[1] = BackgroundColor;
	colorData[2] = UserColor[StitchBuffer[0].attribute&COLMSK];
	highStitchBuffer = &StitchBuffer[MAXSEQ];
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		highStitchBuffer[iStitch].x = RotatedStitches[iStitch].x * 5 / 3;
		highStitchBuffer[iStitch].y = RotatedStitches[iStitch].y * 5 / 3;
		highStitchBuffer[iStitch].attribute = RotatedStitches[iStitch].attribute;
	}
	boundingRect.left = boundingRect.right = highStitchBuffer[0].x;
	boundingRect.bottom = boundingRect.top = highStitchBuffer[0].y;
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (highStitchBuffer[iStitch].x > boundingRect.right)
			boundingRect.right = highStitchBuffer[iStitch].x + 0.5;
		if (highStitchBuffer[iStitch].x < boundingRect.left)
			boundingRect.left = highStitchBuffer[iStitch].x - 0.5;
		if (highStitchBuffer[iStitch].y > boundingRect.top)
			boundingRect.top = highStitchBuffer[iStitch].y + 0.5;
		if (highStitchBuffer[iStitch].y < boundingRect.bottom)
			boundingRect.bottom = highStitchBuffer[iStitch].y - 0.5;
	}
	DSTRecords = (DSTREC*)&BSequence;
	DSTRecordCount = 0;
	centerCoordinate.x = (boundingRect.right - boundingRect.left) / 2 + boundingRect.left;
	centerCoordinate.y = (boundingRect.top - boundingRect.bottom) / 2 + boundingRect.bottom;
	DSTPositiveOffset.x = boundingRect.right - centerCoordinate.x + 1;
	DSTPositiveOffset.y = boundingRect.top - centerCoordinate.y + 1;
	DSTNegativeOffset.x = centerCoordinate.x - boundingRect.left - 1;
	DSTNegativeOffset.y = centerCoordinate.y - boundingRect.bottom - 1;
	color = highStitchBuffer[0].attribute & 0xf;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (color != (highStitchBuffer[iStitch].attribute & 0xf)) {

			savdst(0xc30000);
			color = highStitchBuffer[iStitch].attribute & 0xf;
			colorData[iColor++] = UserColor[color];
		}
		lengths.x = highStitchBuffer[iStitch].x - centerCoordinate.x;
		lengths.y = highStitchBuffer[iStitch].y - centerCoordinate.y;
		absoluteLengths.x = abs(lengths.x);
		absoluteLengths.y = abs(lengths.y);
		// ToDo - is 'abs' neccessary since absoluteLengths was already abs?
		if (abs(absoluteLengths.x) > abs(absoluteLengths.y))
			count = absoluteLengths.x / DSTMAX + 1;
		else
			count = absoluteLengths.y / DSTMAX + 1;
		stepSize.x = absoluteLengths.x / count + 1;
		stepSize.y = absoluteLengths.y / count + 1;
		while (lengths.x || lengths.y) {

			dstType = REGTYP;
			if (abs(lengths.x) > stepSize.x) {

				dstType = JMPTYP;
				if (lengths.x > 0)
					difference.x = stepSize.x;
				else
					difference.x = -stepSize.x;
			} else
				difference.x = lengths.x;
			if (abs(lengths.y) > stepSize.y) {

				dstType = JMPTYP;
				if (lengths.y > 0)
					difference.y = stepSize.y;
				else
					difference.y = -stepSize.y;
			} else
				difference.y = lengths.y;
			savdst(dudbits(difference) | dstType);
			centerCoordinate.x += difference.x;
			centerCoordinate.y += difference.y;
			lengths.x -= difference.x;
			lengths.y -= difference.y;
		}
	}
	DSTRecords[DSTRecordCount].led = DSTRecords[DSTRecordCount].mid = 0;
	DSTRecords[DSTRecordCount++].nd = (TBYTE)0xf3;

	if (colfil()) {

		colorFile = CreateFile(ColorFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (colorFile != INVALID_HANDLE_VALUE)
			WriteFile(colorFile, &OSequence, iColor << 2, &bytesWritten, 0);
		CloseHandle(colorFile);
		colorFile = CreateFile(RGBFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (colorFile != INVALID_HANDLE_VALUE)
			WriteFile(colorFile, &colorData[2], (iColor - 2) << 2, &bytesWritten, 0);
		CloseHandle(colorFile);
	}
}

BOOL pcshup() {

	fRECTANGLE	boundingRect;
	fPOINT		boundingSize;
	unsigned	iStitch;
	fPOINT		hoopSize;
	fPOINT		delta;

	boundingRect.left = boundingRect.right = RotatedStitches[0].x;
	boundingRect.bottom = boundingRect.top = RotatedStitches[0].y;
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (RotatedStitches[iStitch].x < boundingRect.left)
			boundingRect.left = RotatedStitches[iStitch].x;
		if (RotatedStitches[iStitch].x > boundingRect.right)
			boundingRect.right = RotatedStitches[iStitch].x;
		if (RotatedStitches[iStitch].y < boundingRect.bottom)
			boundingRect.bottom = RotatedStitches[iStitch].y;
		if (RotatedStitches[iStitch].y > boundingRect.top)
			boundingRect.top = RotatedStitches[iStitch].y;
	}
	boundingSize.x = boundingRect.right - boundingRect.left;
	boundingSize.y = boundingRect.top - boundingRect.bottom;
	if (boundingSize.x > LHUPX || boundingSize.y > LHUPY) {

		tabmsg(IDS_PFAF2L);
		return 1;
	}
	if (boundingSize.x > SHUPX || boundingSize.y > SHUPY) {

		PCSHeader.hoopType = LARGHUP;
		hoopSize.x = LHUPX;
		hoopSize.y = LHUPY;
	} else {

		if (IniFile.hoopSizeX == LHUPX&&IniFile.hoopSizeY == LHUPY) {

			PCSHeader.hoopType = LARGHUP;
			hoopSize.x = LHUPX;
			hoopSize.y = LHUPY;
		} else {

			PCSHeader.hoopType = SMALHUP;
			hoopSize.x = SHUPX;
			hoopSize.y = SHUPY;
		}
	}
	delta.x = delta.y = 0;
	if (boundingRect.right > hoopSize.x)
		delta.x = hoopSize.x - boundingRect.right;
	if (boundingRect.top > hoopSize.y)
		delta.y = hoopSize.y - boundingRect.top;
	if (boundingRect.left < 0)
		delta.x = -boundingRect.left;
	if (boundingRect.bottom < 0)
		delta.y = -boundingRect.bottom;
	if (delta.x || delta.y) {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			RotatedStitches[iStitch].x += delta.x;
			RotatedStitches[iStitch].y += delta.y;
		}
	}
	return 0;
}

#if PESACT

void ritpes(unsigned iStitch) {

	PESstitches[OutputIndex].x = -RotatedStitches[iStitch].x * 3 / 5 + PESstichCenterOffset.x;
	PESstitches[OutputIndex++].y = RotatedStitches[iStitch].y * 3 / 5 - PESstichCenterOffset.y;
}

void ritpcol(unsigned char colorIndex) {

#if  __UseASM__
	_asm {

		mov		ebx, OutputIndex
		mov		eax, ebx
		inc		eax
		mov		OutputIndex, eax
		shl		ebx, 2
		add		ebx, PESstitches
		xor		eax, eax
		mov		al, colorIndex
		mov[ebx], eax
	}
#else
	_asm {

		mov		ebx, OutputIndex
		mov		eax, ebx
		inc		eax
		mov		OutputIndex, eax
		shl		ebx, 2
		add		ebx, PESstitches
		xor		eax, eax
		mov		al, colorIndex
		mov[ebx], eax
	}
#endif
}

unsigned pesnam() {

#if  __UseASM__
	_asm {
			mov		ebx, offset AuxName
			mov		ecx, _MAX_PATH
			mov		edx, ebx
peslup0 :	mov		al, [ebx]
			or		al, al
			je		short peslup1
			cmp		al, '\\'
			jne		short peslup0a
			mov		edx, ebx
peslup0a :	inc		ebx
			loop	peslup0
peslup1 :	mov		ebx, edx
			cmp		byte ptr[ebx], '\\'
			jne		short peslup1a
			inc		ebx
peslup1a :	xor		ecx, ecx
			mov		cl, 17
			mov		edi, offset BSequence
			mov		dword ptr[edi], ':AL'
			add		edi, 3
peslup :	mov		al, [ebx]
			inc		ebx
			cmp		al, '.'
			je		pesnamx
			mov[edi], al
			inc		edi
			loop	peslup
pesnamx :	mov		eax, edi
			sub		eax, offset BSequence
	}
#else
	// ToDo - Complete translation from assembler
	_asm {
			mov		ebx, offset AuxName
			mov		ecx, _MAX_PATH
			mov		edx, ebx
peslup0 :	mov		al, [ebx]
			or		al, al
			je		short peslup1
			cmp		al, '\\'
			jne		short peslup0a
			mov		edx, ebx
peslup0a :	inc		ebx
			loop	peslup0
peslup1 :	mov		ebx, edx
			cmp		byte ptr[ebx], '\\'
			jne		short peslup1a
			inc		ebx
peslup1a :	xor		ecx, ecx
			mov		cl, 17
			mov		edi, offset BSequence
			mov		dword ptr[edi], ':AL'
			add		edi, 3
peslup :	mov		al, [ebx]
			inc		ebx
			cmp		al, '.'
			je		pesnamx
			mov[edi], al
			inc		edi
			loop	peslup
pesnamx :	mov		eax, edi
			sub		eax, offset BSequence
	}
#endif
}

void rpcrd(float stitchDelta) {

	int pesDelta;

	pesDelta = stitchDelta * 5 / 3;
	if (pesDelta < 0) {

		if (pesDelta > -64) {

			PESdata[OutputIndex] = pesDelta - 128;
			OutputIndex++;
		} else {

			PESdata[OutputIndex] = pesDelta >> 8;
			OutputIndex++;
			PESdata[OutputIndex] = pesDelta & 0x8f;
			OutputIndex++;
		}
	} else {

		if (pesDelta < 64) {

			PESdata[OutputIndex] = pesDelta;
			OutputIndex++;
		} else {

			PESdata[OutputIndex] = (pesDelta >> 8) | 0x80;
			OutputIndex++;
			PESdata[OutputIndex] = pesDelta & 0xff;
			OutputIndex++;
		}
	}
}

void pecdat() {

	unsigned	iStitch, color, iColor;

	OutputIndex = 532;
	iColor = 1;
	color = StitchBuffer[0].attribute&COLMSK;
	PESdata = (TCHAR*)&BSequence;
	PEScolors = (unsigned char*)&PESdata[49];
	rpcrd(StitchBuffer[0].x);
	rpcrd(-StitchBuffer[0].y);
	for (iStitch = 0; iStitch < (unsigned)PCSHeader.stitchCount - 1; iStitch++) {

		if ((StitchBuffer[iStitch].attribute&COLMSK) != color) {

			color = StitchBuffer[iStitch].attribute&COLMSK;
			PESdata[OutputIndex] = (TBYTE)254;
			OutputIndex++;
			PESdata[OutputIndex] = (TBYTE)176;
			OutputIndex++;
			PESdata[OutputIndex] = iColor;
			OutputIndex++;
			PEScolors[iColor] = PESequivColors[color];
			iColor++;
		}
		rpcrd(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x);
		rpcrd(-StitchBuffer[iStitch + 1].y + StitchBuffer[iStitch].y);
	}
	PESdata[OutputIndex++] = (TBYTE)0xff;
	PESdata[OutputIndex++] = 0;
}

#endif

void chk1col() {

	unsigned iColorChange, iStitch, color;

	coltab();
	setMap(RESTCH);
	for (iColorChange = 0; iColorChange < ColorChanges; iColorChange++) {

		if (ColorChangeTable[iColorChange + 1].stitchIndex - ColorChangeTable[iColorChange].stitchIndex == 1) {

			iStitch = ColorChangeTable[iColorChange].stitchIndex;
			color = StitchBuffer[iStitch - 1].attribute&COLMSK;
			StitchBuffer[iStitch].attribute &= NCOLMSK;
			StitchBuffer[iStitch].attribute |= color;
		}
	}
}

BOOL chkattr(TCHAR* filename) {

	unsigned		attributes, buttonPressed;
	TCHAR			drive[_MAX_PATH];
	DWORD			SectorsPerCluster;
	DWORD			BytesPerSector;
	DWORD			NumberOfFreeClusters;
	DWORD			TotalNumberOfClusters;

	if (rstMap(NOTFREE))
		return 1;
	attributes = GetFileAttributes(filename);
	if (attributes&FILE_ATTRIBUTE_READONLY&&attributes != 0xffffffff) {

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_OVRLOK], filename);
		buttonPressed = MessageBox(ThrEdWindow, MsgBuffer, StringTable[STR_OVRIT], MB_YESNO);
		if (buttonPressed == IDYES)
			SetFileAttributes(filename, attributes&(0xffffffff ^ FILE_ATTRIBUTE_READONLY));
		else
			return 1;
	}
	strcpy_s(drive, HomeDirectory);
	drive[3] = 0;
	// ToDo - does a return value of 0 mean no free space?
	if (!GetDiskFreeSpace(drive, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters)) {

		setMap(NOTFREE);
		return 1;
	}
	return 0;
}

// Suppress C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead
#pragma warning(push)
#pragma warning(disable : 4996)
void sav() {

	unsigned		iStitch, iColor, iHeader, iPCSstitch;
	unsigned long	bytesWritten;
	double			fractionalPart, integerPart;
	DSTHED			dstHeader;
	TCHAR*			pchr;
	unsigned		savcol;	

#if PESACT

	PESHED			pesHeader;
	unsigned		match;
	unsigned		matchIndex = 0;
	unsigned		matchMin;
	unsigned char	pesColor;
	fRECTANGLE		boundingRect;
	unsigned		color, iColorMatch;
	unsigned		pesColorCount;
	unsigned*		pesOffset;
	unsigned*		upnt;
	short*			psiz;
#endif

	duauxnam();
	if (chkattr(AuxName))
		return;
	if (!PCSHeader.stitchCount)
		return;
	chk1col();
	coltab();
	RotatedStitches = (fPOINTATTR*)&BSequence;
	if (chku(ROTAUX)) {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			RotatedStitches[iStitch].y = -StitchBuffer[iStitch].x;
			RotatedStitches[iStitch].x = StitchBuffer[iStitch].y;
			RotatedStitches[iStitch].attribute = StitchBuffer[iStitch].attribute;
		}
	} else {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			RotatedStitches[iStitch].x = StitchBuffer[iStitch].x;
			RotatedStitches[iStitch].y = StitchBuffer[iStitch].y;
			RotatedStitches[iStitch].attribute = StitchBuffer[iStitch].attribute;
		}
	}
	PCSFileHandle = CreateFile(AuxName, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (PCSFileHandle == INVALID_HANDLE_VALUE) {

		crmsg(AuxName);
		PCSFileHandle = 0;
	} else {

		switch (IniFile.auxFileType) {

		case AUXDST:

			ritdst();
			pchr = (TCHAR*)&dstHeader;
			for (iHeader = 0; iHeader < sizeof(DSTHED); iHeader++)
				pchr[iHeader] = ' ';
			strncpy(dstHeader.desched, "LA:", 3);
			pchr = strrchr(AuxName, '\\') + 1;
			for (iHeader = 0; iHeader < sizeof(dstHeader.desc); iHeader++) {

				if (pchr[iHeader] && pchr[iHeader] != '.')
					dstHeader.desc[iHeader] = pchr[iHeader];
				else
					break;
			}
			dstHeader.desc[16] = 0xd;
			strncpy(dstHeader.recshed, "ST:", 3);
			sprintf(dstHeader.recs, "%7d\r", DSTRecordCount);
			strncpy(dstHeader.cohed, "CO:", 3);
			strncpy(dstHeader.co, "  0\xd", 4);
			strncpy(dstHeader.xplushed, "+X:", 3);
			sprintf(dstHeader.xplus, "%5d\xd", DSTNegativeOffset.x);
			strncpy(dstHeader.xminhed, "-X:", 3);
			sprintf(dstHeader.xmin, "%5d\xd", DSTPositiveOffset.x);
			strncpy(dstHeader.yplushed, "+Y:", 3);
			sprintf(dstHeader.yplus, "%5d\xd", DSTPositiveOffset.y);
			strncpy(dstHeader.yminhed, "-Y:", 3);
			sprintf(dstHeader.ymin, "%5d\xd", DSTNegativeOffset.y);
			strncpy(dstHeader.axhed, "AX:", 3);
			strncpy(dstHeader.ax, "-    0\r", 7);
			strncpy(dstHeader.ayhed, "AY:", 3);
			strncpy(dstHeader.ay, "+    0\r", 7);
			strncpy(dstHeader.mxhed, "MX:", 3);
			strncpy(dstHeader.mx, "+    0\r", 7);
			strncpy(dstHeader.myhed, "MY:", 3);
			strncpy(dstHeader.my, "+    0\r", 7);
			strncpy(dstHeader.pdhed, "PD", 2);
			strncpy(dstHeader.pd, "******\r", 7);
			strncpy(dstHeader.eof, "\x1a", 1);
			WriteFile(PCSFileHandle, (DSTHED*)&dstHeader, sizeof(DSTHED), &bytesWritten, 0);
			WriteFile(PCSFileHandle, (DSTREC*)DSTRecords, sizeof(DSTREC)*DSTRecordCount, &bytesWritten, 0);
			break;

#if PESACT

		case AUXPES:

			pchr = (TCHAR*)&pesHeader;
			for (iHeader = 0; iHeader < sizeof(PESHED); iHeader++)
				pchr[iHeader] = 0;
			strncpy(pesHeader.led, "#PES0001",sizeof(pesHeader.led));
			strncpy(pesHeader.ce, "CEmbOne", sizeof(pesHeader.ce));
			strncpy(pesHeader.cs, "CSewSeg", sizeof(pesHeader.cs));
			for (iColor = 0; iColor < 16; iColor++) {

				matchMin = 0xffffffff;
				for (iColorMatch = 0; iColorMatch < sizeof(PESColorTranslate) >> 2; iColorMatch++) {

					match = pesmtch(UserColor[iColor], iColorMatch);
					if (match < matchMin) {

						matchIndex = iColorMatch;
						matchMin = match;
					}
				}
				PESequivColors[iColor] = (unsigned char)matchIndex;
			}
			color = StitchBuffer[0].attribute&COLMSK;
			pesColor = pesHeader.scol = PESequivColors[StitchBuffer[0].attribute&COLMSK];
			sizstch(&boundingRect);
			PESstichCenterOffset.x = midl(boundingRect.right, boundingRect.left);
			PESstichCenterOffset.y = midl(boundingRect.top, boundingRect.bottom);
			pesHeader.xsiz = boundingRect.right - boundingRect.left;
			pesHeader.ysiz = boundingRect.top - boundingRect.bottom;
			OutputIndex = 0;
			PESstitches = (PESTCH*)&BSequence;
			ritpes(0);
			PESstitches[OutputIndex].x = -32765; // 0x8003
			PESstitches[OutputIndex++].y = 0;
			ritpcol(PESequivColors[color]);
			ritpes(0);
			pesColorCount = 0;
			for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {

				if (color == (StitchBuffer[iStitch].attribute&COLMSK))
					ritpes(iStitch);
				else {

					ritpes(iStitch);
					PESstitches[OutputIndex].x = -32767; // 0x8001
					PESstitches[OutputIndex++].y = 0;
					ritpcol(PESequivColors[color]);
					color = StitchBuffer[iStitch].attribute&COLMSK;
					ritpes(iStitch++);
					ritpes(iStitch);
					PESstitches[OutputIndex].x = -32765; // 0x8003
					PESstitches[OutputIndex++].y = 0;
					ritpcol(PESequivColors[color]);
					ritpes(iStitch);
					pesColorCount++;
				}
			}
			PESstitches[OutputIndex].x = iColorMatch;
			PESstitches[OutputIndex++].y = 0;
			pesOffset = (unsigned*)&pesHeader.off;
			*pesOffset = (OutputIndex << 2) + sizeof(PESHED);
			*pesHeader.m1 = 0x20;
			GroupStartStitch = 0;
			GroupEndStitch = PCSHeader.stitchCount - 1;
			pesHeader.xsiz = 10000;
			pesHeader.ysiz = 10000;
			WriteFile(PCSFileHandle, (PESHED*)&pesHeader, sizeof(PESHED), &bytesWritten, 0);
			WriteFile(PCSFileHandle, (PESTCH*)&BSequence, OutputIndex << 2, &bytesWritten, 0);
			iHeader = pesnam();
			pchr = (TCHAR*)&BSequence;
			while (iHeader < 512)
				pchr[iHeader++] = ' ';
			pchr[19] = 13;
			pchr[48] = (TCHAR)pesColorCount;
			pecdat();
			upnt = (unsigned*)&pchr[514];
			*upnt = OutputIndex - 512;
			pchr[517] = 0x20;
			pchr[518] = -1;
			pchr[519] = -17;
			psiz = (short*)&pchr[520];
			*psiz = pesHeader.xsiz;
			psiz++;
			*psiz = pesHeader.ysiz;
			psiz++;
			*psiz = 480;
			pesOffset = (unsigned*)psiz;
			*pesOffset = 11534816;
			//			pchr[527]=(TCHAR)0x0;
			//			pchr[528]=(TCHAR)0x90;
			//			pchr[529]=(TCHAR)0x0;
			//			pchr[530]=(TCHAR)0x8f;
			pchr[527] = (TBYTE)0x00;
			pchr[528] = (TBYTE)0x80;	//hor	msb
			pchr[529] = (TBYTE)0x80; //hor lsb
			pchr[530] = (TBYTE)0x82; //vert msb
			pchr[531] = (TBYTE)0xff; //vert lsb
			WriteFile(PCSFileHandle, (TBYTE*)&BSequence, OutputIndex, &bytesWritten, 0);
			break;
#endif
		default:

			for (iColor = 0; iColor < 16; iColor++)
				PCSHeader.colors[iColor] = UserColor[iColor];
			if (pcshup())
				return;
			if (!WriteFile(PCSFileHandle, &PCSHeader, 0x46, &bytesWritten, 0)) {

				riter();
				return;
			}
			PCSStitchBuffer = new PCSTCH[PCSHeader.stitchCount + ColorChanges + 2];
			clrfbuf((sizeof(PCSTCH)*(PCSHeader.stitchCount + ColorChanges + 1)) >> 2);
			iPCSstitch = 0;
			savcol = 0xff;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

				if ((RotatedStitches[iStitch].attribute&COLMSK) != savcol) {

					savcol = RotatedStitches[iStitch].attribute&COLMSK;
					PCSStitchBuffer[iPCSstitch].tag = 3;
					PCSStitchBuffer[iPCSstitch++].fx = savcol;
				}
				fractionalPart = modf(RotatedStitches[iStitch].x, &integerPart);
				PCSStitchBuffer[iPCSstitch].fx = fractionalPart * 256;
				PCSStitchBuffer[iPCSstitch].x = integerPart;
				fractionalPart = modf(RotatedStitches[iStitch].y, &integerPart);
				PCSStitchBuffer[iPCSstitch].fy = fractionalPart * 256;
				PCSStitchBuffer[iPCSstitch++].y = integerPart;
			}
			if (!WriteFile(PCSFileHandle, PCSStitchBuffer, iPCSstitch * sizeof(PCSTCH), &bytesWritten, 0)) {

				riter();
				return;
			}
			if (chku(BSAVOF)) {

				*MsgBuffer = 0;
				if (!WriteFile(PCSFileHandle, MsgBuffer, 15, &bytesWritten, 0)) {

					riter();
					return;
				}
			} else {

				if (!WriteFile(PCSFileHandle, PCSBMPFileName, 15, &bytesWritten, 0)) {

					riter();
					return;
				}
			}
			delete[] PCSStitchBuffer;
		}
		defNam(WorkingFileName);
		CloseHandle(PCSFileHandle);
		if (chku(ROTAUX))
			filnopn(IDS_FILROT, AuxName);
	}
}
#pragma warning(pop)

void savAs() {

	TCHAR*	pchr;

	if (PCSHeader.stitchCount || FormIndex || *PCSBMPFileName) {

		OpenFileName.nFilterIndex = 0;
		if (GetSaveFileName(&OpenFileName)) {

			_strlwr_s(WorkingFileName);
			pchr = strrchr(WorkingFileName, '.');
			if (!pchr)
				pchr = &WorkingFileName[strlen(WorkingFileName)];
			switch (OpenFileName.nFilterIndex) {

			case 1:

				strcpy_s(pchr, sizeof(WorkingFileName) - (pchr - WorkingFileName), ".thr");
				break;

			case 2:

				strcpy_s(pchr, sizeof(WorkingFileName) - (pchr - WorkingFileName), ".pcs");
				IniFile.auxFileType = AUXPCS;
				auxmen();
				break;

			case 3:

				strcpy_s(pchr, sizeof(WorkingFileName) - (pchr - WorkingFileName), ".dst");
				IniFile.auxFileType = AUXDST;
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
			SetWindowText(ThrEdWindow, ThrName);
		}
	}
}

void save() {

	TCHAR*	pchr;

	if (WorkingFileName[0]) {

		pchr = strrchr(WorkingFileName, '.');
		if (pchr)
			pchr++;
		else {

			strcat_s(WorkingFileName, ".thr");
			pchr = strrchr(WorkingFileName, '.') + 1;
		}
		thrsav();
		if (PCSHeader.stitchCount)
			sav();
	} else
		savAs();
}

COLORREF nuCol(COLORREF init) {

	ColorStruct.Flags = CC_ANYCOLOR | CC_RGBINIT;
	ColorStruct.hwndOwner = ThrEdWindow;
	ColorStruct.lCustData = 0;
	ColorStruct.lpCustColors = CustomColor;
	ColorStruct.lpfnHook = 0;
	ColorStruct.lpTemplateName = 0;
	ColorStruct.rgbResult = init;
	ColorStruct.lStructSize = sizeof(ColorStruct);
	return ChooseColor(&ColorStruct);
}

COLORREF nuBak() {

	BackgroundColorStruct.Flags = CC_ANYCOLOR | CC_RGBINIT;
	BackgroundColorStruct.hwndOwner = ThrEdWindow;
	BackgroundColorStruct.lCustData = 0;
	BackgroundColorStruct.lpCustColors = CustomBackgroundColor;
	BackgroundColorStruct.lpfnHook = 0;
	BackgroundColorStruct.lpTemplateName = 0;
	BackgroundColorStruct.rgbResult = BackgroundColor;
	BackgroundColorStruct.lStructSize = sizeof(BackgroundColorStruct);
	return ChooseColor(&BackgroundColorStruct);
}

COLORREF nuBit() {

	BitMapColorStruct.Flags = CC_ANYCOLOR | CC_RGBINIT;
	BitMapColorStruct.hwndOwner = ThrEdWindow;
	BitMapColorStruct.lCustData = 0;
	BitMapColorStruct.lpCustColors = BitmapBackgroundColors;
	BitMapColorStruct.lpfnHook = 0;
	BitMapColorStruct.lpTemplateName = 0;
	BitMapColorStruct.rgbResult = BitmapColor;
	BitMapColorStruct.lStructSize = sizeof(BackgroundColorStruct);
	return ChooseColor(&BitMapColorStruct);
}

void pxCor2stch(POINT pnt) {

	double	tdub;

	tdub = (double)(pnt.x - StitchWindowAbsRect.left) / StitchWindowClientRect.right;
	SelectedPoint.x = tdub*(ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	tdub = (double)(StitchWindowAbsRect.bottom - pnt.y) / StitchWindowClientRect.bottom;
	SelectedPoint.y = tdub*(ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

unsigned px2stch() {

	double	tdub;

	if (Msg.pt.x >= StitchWindowAbsRect.left&&Msg.pt.x <= StitchWindowAbsRect.right
		&&Msg.pt.y >= StitchWindowAbsRect.top&&Msg.pt.y <= StitchWindowAbsRect.bottom) {

		tdub = (double)(Msg.pt.x - StitchWindowAbsRect.left) / StitchWindowClientRect.right;
		SelectedPoint.x = tdub*(ZoomRect.right - ZoomRect.left) + ZoomRect.left;
		tdub = (double)(StitchWindowAbsRect.bottom - Msg.pt.y) / StitchWindowClientRect.bottom;
		SelectedPoint.y = tdub*(ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
		return 1;
	} else
		return 0;
}

void shft2box() {

	SelectedPoint.x = StitchBuffer[ClosestPointIndex].x;
	SelectedPoint.y = StitchBuffer[ClosestPointIndex].y;
	shft(SelectedPoint);
	stch2px1(ClosestPointIndex);
}

void zumin() {

	dPOINT		newSize;
	fRECTANGLE	groupBoundingRect;
	fRECTANGLE*	boundingRect;
	unsigned	iForm;

	if (!rstMap(ZUMACT))
		ZoomFactor *= ZUMFCT;
	if (ZoomFactor < ZoomMin)
		ZoomFactor = ZoomMin;
	if (!setMap(ZUMED))
		movStch();
	if (!rstMap(BZUMIN)) {

		if (chkMap(GMRK)) {

			SelectedPoint.x = ZoomMarkPoint.x;
			SelectedPoint.y = ZoomMarkPoint.y;
			goto gotc;
		}
		if (chkMap(FORMSEL)) {

			boundingRect = &FormList[ClosestFormToCursor].rectangle;
			SelectedPoint.x = ((boundingRect->right - boundingRect->left) / 2) + boundingRect->left;
			SelectedPoint.y = ((boundingRect->top - boundingRect->bottom) / 2) + boundingRect->bottom;
			goto gotc;
		}
		if (chkMap(FRMPSEL)) {

			SelectedPoint.x = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].x;
			SelectedPoint.y = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].y;
			goto gotc;
		}
		if (chkMap(SELBOX)) {

			SelectedPoint.x = StitchBuffer[ClosestPointIndex].x;
			SelectedPoint.y = StitchBuffer[ClosestPointIndex].y;
			goto gotc;
		}
		if (chkMap(GRPSEL)) {

			selRct(&groupBoundingRect);
			SelectedPoint.x = ((groupBoundingRect.right - groupBoundingRect.left) / 2) + groupBoundingRect.left;
			SelectedPoint.y = ((groupBoundingRect.top - groupBoundingRect.bottom) / 2) + groupBoundingRect.bottom;
			goto gotc;
		}
		if (chkMap(INSRT)) {

			if (chkMap(LIN1)) {

				if (chkMap(BAKEND)) {

					SelectedPoint.x = StitchBuffer[PCSHeader.stitchCount - 1].x;
					SelectedPoint.y = StitchBuffer[PCSHeader.stitchCount - 1].y;
				} else {

					SelectedPoint.x = StitchBuffer[0].x;
					SelectedPoint.y = StitchBuffer[0].y;
				}
			} else {

				SelectedPoint.x = (StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x) / 2 + StitchBuffer[ClosestPointIndex].x;
				SelectedPoint.y = (StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y) / 2 + StitchBuffer[ClosestPointIndex].y;
			}
			goto gotc;
		}
		if (SelectedFormCount) {

			SelectedFormsRect.bottom = FormList[SelectedFormList[0]].rectangle.bottom;
			SelectedFormsRect.top = FormList[SelectedFormList[0]].rectangle.top;
			SelectedFormsRect.left = FormList[SelectedFormList[0]].rectangle.left;
			SelectedFormsRect.right = FormList[SelectedFormList[0]].rectangle.right;
			for (iForm = 1; iForm < SelectedFormCount; iForm++) {

				if (FormList[SelectedFormList[iForm]].rectangle.bottom < SelectedFormsRect.bottom)
					SelectedFormsRect.bottom = FormList[SelectedFormList[iForm]].rectangle.bottom;
				if (FormList[SelectedFormList[iForm]].rectangle.top > SelectedFormsRect.top)
					SelectedFormsRect.top = FormList[SelectedFormList[iForm]].rectangle.top;
				if (FormList[SelectedFormList[iForm]].rectangle.left < SelectedFormsRect.left)
					SelectedFormsRect.left = FormList[SelectedFormList[iForm]].rectangle.left;
				if (FormList[SelectedFormList[iForm]].rectangle.right > SelectedFormsRect.right)
					SelectedFormsRect.right = FormList[SelectedFormList[iForm]].rectangle.right;
			}
			SelectedPoint.x = (SelectedFormsRect.right - SelectedFormsRect.left) / 2 + SelectedFormsRect.left;
			SelectedPoint.y = (SelectedFormsRect.top - SelectedFormsRect.bottom) / 2 + SelectedFormsRect.bottom;
			goto gotc;
		}
		if (!px2stch())
			centr();
	}
gotc:;
	newSize.x = UnzoomedRect.x*ZoomFactor;
	newSize.y = newSize.x / StitchWindowAspectRatio;
	ZoomRect.left = ZoomRect.bottom = 0;
	ZoomRect.right = newSize.x;
	ZoomRect.top = newSize.y;
	shft(SelectedPoint);
	NearestCount = 0;
	if (!chkMap(GMRK) && chkMap(SELBOX))
		shft2box();
	if (chkMap(RUNPAT)) {

		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		RunPoint = 0;
	}
	duzrat();
	movins();
	setMap(RESTCH);
	ilin();
}

void zumhom() {

	ZoomRect.bottom = 0;
	ZoomRect.left = 0;
	ZoomRect.right = UnzoomedRect.x;
	ZoomRect.top = UnzoomedRect.y;
	ZoomFactor = 1;
	rstMap(ZUMED);
	movStch();
	NearestCount = 0;
	if (chkMap(RUNPAT)) {

		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		RunPoint = 0;
	}
	setMap(RESTCH);
	if (chkMap(SELBOX))
		shft2box();
	duzrat();
	movins();
}

void zumshft() {

	if (chkMap(ZUMED)) {

		unboxs();
		if (px2stch()) {
			NearestCount = 0;
			shft(SelectedPoint);
		if (chkMap(RUNPAT)) {

				FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
				RunPoint = 0;
			}
			setMap(RESTCH);
		}
	}
	movins();
}

void zumout() {

	POINT	newSize;
	fRECTANGLE	groupBoundingRect;
	fRECTANGLE*	boundingRect;

	unboxs();
	if (chkMap(ZUMED)) {

		if (chkMap(GMRK)) {

			SelectedPoint.x = ZoomMarkPoint.x;
			SelectedPoint.y = ZoomMarkPoint.y;
			goto gots;
		}
		if (chkMap(FORMSEL)) {

			boundingRect = &FormList[ClosestFormToCursor].rectangle;
			SelectedPoint.x = ((boundingRect->right - boundingRect->left) / 2) + boundingRect->left;
			SelectedPoint.y = ((boundingRect->top - boundingRect->bottom) / 2) + boundingRect->bottom;
			goto gots;
		}
		if (chkMap(FRMPSEL)) {

			SelectedPoint.x = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].x;
			SelectedPoint.y = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].y;
			goto gots;
		}
		if (chkMap(SELBOX) || chkMap(INSRT)) {

			SelectedPoint.x = StitchBuffer[ClosestPointIndex].x;
			SelectedPoint.y = StitchBuffer[ClosestPointIndex].y;
			goto gots;
		}
		if (chkMap(GRPSEL)) {

			selRct(&groupBoundingRect);
			SelectedPoint.x = ((groupBoundingRect.right - groupBoundingRect.left) / 2) + groupBoundingRect.left;
			SelectedPoint.y = ((groupBoundingRect.top - groupBoundingRect.bottom) / 2) + groupBoundingRect.bottom;
			goto gots;
		}
		if (chkMap(SELBOX)) {

			shft2box();
			goto gots;
		}
		if (!px2stch())
			centr();
	gots:;
		ZoomFactor /= ZUMFCT;
		if (ZoomFactor > 0.98) {

			ZoomFactor = 1;
			rstMap(ZUMED);
			ZoomRect.bottom = 0;
			ZoomRect.left = 0;
			ZoomRect.right = UnzoomedRect.x;
			ZoomRect.top = UnzoomedRect.y;
			movStch();
			NearestCount = 0;
		} else {

			newSize.x = UnzoomedRect.x*ZoomFactor;
			newSize.y = newSize.x / StitchWindowAspectRatio;
			ZoomRect.left = ZoomRect.bottom = 0;
			ZoomRect.top = newSize.y;
			ZoomRect.right = newSize.x;
			shft(SelectedPoint);
		}
		if (chkMap(RUNPAT)) {

			FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
			RunPoint = 0;
		}
		setMap(RESTCH);
		duzrat();
		movins();
	}
}

void duClos(unsigned startStitch, unsigned stitchCount) {

	unsigned	iStitch, iNear, tind0, tind1;
	double		sum, lowestSum;
	long		cx, cy;

	for (iStitch = startStitch; iStitch < startStitch + stitchCount; iStitch++) {

		cx = ((StitchBuffer[iStitch].x > SelectedPoint.x) ? (StitchBuffer[iStitch].x - SelectedPoint.x) : (SelectedPoint.x - StitchBuffer[iStitch].x));
		cy = ((StitchBuffer[iStitch].y > SelectedPoint.y) ? (StitchBuffer[iStitch].y - SelectedPoint.y) : (SelectedPoint.y - StitchBuffer[iStitch].y));
		sum = hypot(cx, cy);
		tind0 = iStitch;
		for (iNear = 0; iNear < NERCNT; iNear++) {

			if (sum < GapToNearest[iNear]) {

				lowestSum = GapToNearest[iNear];
				tind1 = NearestPoint[iNear];
				GapToNearest[iNear] = sum;
				NearestPoint[iNear] = tind0;
				sum = lowestSum;
				tind0 = tind1;
			}
		}
	}
}

void closPnt() {

	unsigned	iColor, iNear, iStitch0, iStitch1;

	unbox();
	unboxs();
	for (iNear = 0; iNear < 4; iNear++) {

		GapToNearest[iNear] = 1e99;
		NearestPoint[iNear] = -1;
	}
	px2stch();
	for (iColor = 0; iColor < ColorChanges; iColor++) {

		iStitch0 = ColorChangeTable[iColor].stitchIndex;
		iStitch1 = ColorChangeTable[iColor + 1].stitchIndex;
		if (chkMap(HID)) {

			if (ColorChangeTable[iColor].colorIndex == ActiveColor)
				duClos(iStitch0, iStitch1 - iStitch0);
		} else
			duClos(iStitch0, iStitch1 - iStitch0);
	}
	GetClientRect(MainStitchWin, &StitchWindowClientRect);
	for (iNear = 0; iNear < NERCNT; iNear++) {

		if (stch2px(NearestPoint[iNear])) {

			NearestPoint[NearestCount] = NearestPoint[iNear];
			NearestPixel[NearestCount].x = StitchCoordinatesPixels.x;
			NearestPixel[NearestCount++].y = StitchCoordinatesPixels.y;
		}
	}
	boxs();
}

unsigned closPnt1(unsigned* closestStitch) {

	unsigned	iColor, iNear, iStitch, closestIndex = 0;
	unsigned	layer;
	long		offset;
	double		cx, cy;
	POINT		pointToCheck;
	double		currentDistance = 1e99;

	pointToCheck.x = Msg.pt.x - StitchWindowOrigin.x;
	pointToCheck.y = Msg.pt.y - StitchWindowOrigin.y;
	if (chkMap(SELBOX) && stch2px(ClosestPointIndex)) {

		if (hypot(StitchCoordinatesPixels.x - pointToCheck.x, StitchCoordinatesPixels.y - pointToCheck.y) < CLOSENUF) {

			*closestStitch = ClosestPointIndex;
			return 1;
		}
	}
	for (iNear = 0; iNear < NearestCount; iNear++) {

		offset = BoxOffset[iNear];
		if (pointToCheck.x >= NearestPixel[iNear].x - offset&&
			pointToCheck.x <= NearestPixel[iNear].x + offset&&
			pointToCheck.y >= NearestPixel[iNear].y - offset&&
			pointToCheck.y <= NearestPixel[iNear].y + offset) {

			*closestStitch = NearestPoint[iNear];
			return 1;
		}
	}
	px2stch();
	DistanceToClick = 1e99;
	if (chkMap(HID)) {

		for (iColor = 0; iColor < ColorChanges; iColor++) {

			if (ColorChangeTable[iColor].colorIndex == ActiveColor) {

				for (iStitch = ColorChangeTable[iColor].stitchIndex; iStitch < ColorChangeTable[iColor + 1].stitchIndex; iStitch++) {

					if (StitchBuffer[iStitch].x >= ZoomRect.left&&
						StitchBuffer[iStitch].x <= ZoomRect.right&&
						StitchBuffer[iStitch].y >= ZoomRect.bottom&&
						StitchBuffer[iStitch].y <= ZoomRect.top) {

						cx = ((StitchBuffer[iStitch].x > SelectedPoint.x) ? (StitchBuffer[iStitch].x - SelectedPoint.x) : (SelectedPoint.x - StitchBuffer[iStitch].x));
						cy = ((StitchBuffer[iStitch].y > SelectedPoint.y) ? (StitchBuffer[iStitch].y - SelectedPoint.y) : (SelectedPoint.y - StitchBuffer[iStitch].y));
						currentDistance = hypot(cx, cy);
						if (currentDistance < DistanceToClick) {

							DistanceToClick = currentDistance;
							closestIndex = iStitch;
						}
					}
				}
			}
		}
	} else {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			layer = (StitchBuffer[iStitch].attribute&LAYMSK) >> LAYSHFT;
			if (!ActiveLayer || !layer || layer == ActiveLayer) {

				if (StitchBuffer[iStitch].x >= ZoomRect.left&&
					StitchBuffer[iStitch].x <= ZoomRect.right&&
					StitchBuffer[iStitch].y >= ZoomRect.bottom&&
					StitchBuffer[iStitch].y <= ZoomRect.top) {

					cx = StitchBuffer[iStitch].x - SelectedPoint.x;
					cy = StitchBuffer[iStitch].y - SelectedPoint.y;
					currentDistance = hypot(cx, cy);
					if (currentDistance < DistanceToClick) {

						DistanceToClick = currentDistance;
						closestIndex = iStitch;
					}
				}
			}
		}
	}
	if (DistanceToClick == 1e99)
		return 0;
	else {

		stch2px(closestIndex);
		if (chkMap(IGNTHR)) {

			*closestStitch = closestIndex;
			return 1;
		} else {

			if (hypot(pointToCheck.x - StitchCoordinatesPixels.x, pointToCheck.y - StitchCoordinatesPixels.y) < CLOSENUF) {

				*closestStitch = closestIndex;
				return 1;
			} else
				return 0;
		}
	}
}

unsigned pt2colInd(unsigned iStitch) {

	unsigned iColor;

	for (iColor = 0; iColor < ColorChanges; iColor++) {

		if (ColorChangeTable[iColor].stitchIndex > iStitch)
			break;
	}
	return iColor;
}

void unthum() {

	unsigned iBackup;

	if (rstMap(THUMSHO)) {

		for (iBackup = 0; iBackup < 4; iBackup++)
			DestroyWindow(BackupViewer[iBackup]);
		if (chkMap(UPTO))
			butxt(HUPTO, StringTable[STR_UPON]);
		else
			butxt(HUPTO, StringTable[STR_UPOF]);
		butxt(HNUM, "");
		redraw(ButtonWin[HHID]);
		butxt(HBOXSEL, StringTable[STR_BOXSEL]);
	}
}

void toglup() {

	if (toglMap(UPTO))
		butxt(HUPTO, StringTable[STR_UPOF]);
	else {

		if (rstMap(GRPSEL)) {

			rngadj();
			ClosestPointIndex = GroupStartStitch;
			setMap(SELBOX);
			rstMap(FRMPSEL);
		} else {

			if (!chkMap(SELBOX)) {

				ClosestPointIndex = 0;
				if (rstMap(FORMSEL)) {

					while (ClosestPointIndex < PCSHeader.stitchCount&&notfstch(StitchBuffer[ClosestPointIndex].attribute))
						ClosestPointIndex++;
					setMap(SELBOX);
					rstMap(FRMPSEL);
				}
			}
		}
		butxt(HUPTO, StringTable[STR_UPON]);
	}
	setMap(RESTCH);
}

void toglHid() {

	if (toglMap(HID))
		rstMap(FRMOF);
	else
		setMap(FRMOF);
	unthum();
	redraw(ButtonWin[HHID]);
	setMap(RESTCH);
}

void dulin() {

	SelectObject(StitchWindowDC, LinePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	if (MoveLine0[0].x == MoveLine1[1].x&&MoveLine0[0].y == MoveLine1[1].y) {

		if (chkMap(ISDWN)) {

			Polyline(StitchWindowDC, MoveLine0, 2);
			goto dux;
		} else {

			Polyline(StitchWindowDC, MoveLine1, 2);
			goto dux;
		}
	} else {

		if (chkMap(ISDWN))
			Polyline(StitchWindowDC, MoveLine0, 2);
		if (chkMap(ISUP))
			Polyline(StitchWindowDC, MoveLine1, 2);
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

	if (stch2px(ClosestPointIndex)) {

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
	nuAct(ClosestPointIndex);
	ritcor(&StitchBuffer[ClosestPointIndex]);
}

BOOL chkhid(unsigned colorToCheck) {

	if (chkMap(HID)) {

		if (ColorChangeTable[colorToCheck].colorIndex == ActiveColor)
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
 to the select point (SelectedPoint) is calculated.

Find the equation for the line by solving the linear parametric eauations

  StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y=offset
  StitchBuffer[StitchIndex+1].x+Slope*StitchBuffer[StitchIndex+1].y=offset

substituting:

  StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y=StitchBuffer[StitchIndex+1].x+Slope*StitchBuffer[StitchIndex+1].y

collecting terms:

  Slope*StitchBuffer[StitchIndex].y-Slope*StitchBuffer[StitchIndex-1].y=StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x
  Slope*(StitchBuffer[StitchIndex].y-StitchBuffer[StitchIndex-1].y)=StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x
  Slope=(StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x)/(StitchBuffer[StitchIndex].y-StitchBuffer[StitchIndex-1].y)

define xba=StitchBuffer[StitchIndex+1].x-StitchBuffer[StitchIndex].x
define yab=StitchBuffer[StitchIndex].y-StitchBuffer[StitchIndex+1].y

  Slope=xba/yab;

back substituting into StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y=offset

  offset=StitchBuffer[StitchIndex].x+Slope*StitchBuffer[StitchIndex].y

The equation for a point vertical to the equation for the line and running
 through SelectedPoint is:

  SelectedPoint.x-SelectedPoint.y/Slope=poff

If intersection is the intersections between the two lines then

  intersection.x-Slope*intersection.y=offset
  intersection.x+intersection.y/Slope=poff

Subtracting the two equations

  Slope*intersection.y+intersection.y/Slope=offset-poff

Multiply by Slope

  Slope*Slope*intersection.y+intersection.y=Slope(offset-poff)
  intersection.y(Slope*Slope+1)=Slope(offset-poff)
  intersection.y=Slope*(offset-poff)/(Slope*Slope+1)

back substitute into intersection.x+Slope*intersection.y=offset

  intersection.x=offset-Slope*intersection.y

if dx=intersection.x-spnt.x & dy=intersection.y-spnt.y

the distance from the point to the line is given by

  sqrt(dx*dx+dy*dy)
*/

unsigned closlin() {
	// ToDo - This function needs to be thoroughly checked

	unsigned	iStitch, iChange, stitchCount, closestPoint = 0xffffffff;
	double		sum = 1e99;
	long		xba, yab;
	double		tsum;
	dRECTANGLE	boundingRect;
	double		slope, offset, poff, tolerance, dx, dy;
	dPOINT		intersection;
	dPOINT		checkedPoint;
	fPOINTATTR*	stitches;
	unsigned	layer;

#define		TOL	20
	// ToDo - Is tsum calculated too many times?
	unboxs();
	offset = (float)(Msg.pt.x - StitchWindowAbsRect.left) / (StitchWindowAbsRect.right - StitchWindowAbsRect.left);
	checkedPoint.x = offset*(ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	offset = (float)(StitchWindowAbsRect.bottom - Msg.pt.y) / (StitchWindowAbsRect.bottom - StitchWindowAbsRect.top);
	checkedPoint.y = (offset*(ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom);
	offset = (double)(ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
	tolerance = offset*TOL;
	for (iChange = 0; iChange < ColorChanges; iChange++) {

		stitchCount = ColorChangeTable[iChange + 1].stitchIndex - ColorChangeTable[iChange].stitchIndex;
		stitches = &StitchBuffer[ColorChangeTable[iChange].stitchIndex];
		if (chkhid(iChange)) {

			for (iStitch = 0; iStitch < stitchCount; iStitch++) {
				// ToDo - should the index in line below be iChange or iStitch?
				layer = (stitches[iChange].attribute&LAYMSK) >> LAYSHFT;
				if (!ActiveLayer || !layer || (layer == ActiveLayer)) {

					xba = stitches[iStitch + 1].x - stitches[iStitch].x;
					yab = stitches[iStitch].y - stitches[iStitch + 1].y;
					if (xba > 0) {

						boundingRect.left = stitches[iStitch].x - tolerance;
						boundingRect.right = stitches[iStitch + 1].x + tolerance;
					} else {

						boundingRect.left = stitches[iStitch + 1].x - tolerance;
						boundingRect.right = stitches[iStitch].x + tolerance;
					}
					if (yab < 0) {

						boundingRect.bottom = stitches[iStitch].y - tolerance;
						boundingRect.top = stitches[iStitch + 1].y + tolerance;
					} else {

						boundingRect.bottom = stitches[iStitch + 1].y - tolerance;
						boundingRect.top = stitches[iStitch].y + tolerance;
					}
					if (checkedPoint.x > boundingRect.left&&checkedPoint.x<boundingRect.right
						&&checkedPoint.y>boundingRect.bottom&&checkedPoint.y < boundingRect.top) {

						if (yab == 0) {

							//stitch is horizontal
							intersection.x = checkedPoint.x;
							intersection.y = stitches[iStitch].y;
							tsum = ((stitches[iStitch].y > checkedPoint.y) ? (stitches[iStitch].y - checkedPoint.y) : (checkedPoint.y - stitches[iStitch].y));
							goto gotsum;
						}
						if (xba == 0) {

							//stitch is vertical
							dx = stitches[iStitch].x - checkedPoint.x;
							boundingRect.top -= tolerance;
							boundingRect.bottom += tolerance;
							if (checkedPoint.y > boundingRect.top) {

								dy = checkedPoint.y - boundingRect.top;
								tsum = hypot(dx, dy);
								goto gotal;
							}
							if (checkedPoint.y < boundingRect.bottom) {

								dy = checkedPoint.y - boundingRect.bottom;
								tsum = hypot(dx, dy);
								goto gotal;
							}
							tsum = fabs(dx);
							goto gotal;
						}
						slope = (double)xba / yab;
						offset = stitches[iStitch].x + slope*stitches[iStitch].y;
						poff = checkedPoint.x - checkedPoint.y / slope;
						intersection.y = slope*(offset - poff) / (slope*slope + 1);
						intersection.x = offset - slope*intersection.y;
						dx = intersection.x - checkedPoint.x;
						dy = intersection.y - checkedPoint.y;
						tsum = hypot(dx, dy);
					gotsum:;
						boundingRect.bottom += tolerance;
						boundingRect.top -= tolerance;
						boundingRect.left += tolerance;
						boundingRect.right -= tolerance;
						// ToDo - Is tsum being caclulated as sqrt(x+y) and not as sqrt(x*x + y*y)?
						if (intersection.x < boundingRect.left) {

							if (intersection.y < boundingRect.bottom)
								tsum = sqrt(((checkedPoint.x > boundingRect.left) ? (checkedPoint.x - boundingRect.left) : (boundingRect.left - checkedPoint.x)) + ((checkedPoint.y > boundingRect.bottom) ? (checkedPoint.y - boundingRect.bottom) : (boundingRect.bottom - checkedPoint.y)));
							else
								tsum = sqrt(((checkedPoint.x > boundingRect.left) ? (checkedPoint.x - boundingRect.left) : (boundingRect.left - checkedPoint.x)) + ((checkedPoint.y > boundingRect.top) ? (checkedPoint.y - boundingRect.top) : (checkedPoint.y - boundingRect.top)));
						} else {

							if (intersection.x > boundingRect.right) {

								if (intersection.y < boundingRect.bottom)
									tsum = sqrt(((checkedPoint.x > boundingRect.right) ? (checkedPoint.x - boundingRect.right) : (boundingRect.right - checkedPoint.x)) + ((checkedPoint.y > boundingRect.bottom) ? (checkedPoint.y - boundingRect.bottom) : (boundingRect.bottom - checkedPoint.y)));
								else
									tsum = sqrt(((checkedPoint.x > boundingRect.right) ? (checkedPoint.x - boundingRect.right) : (boundingRect.right - checkedPoint.x)) + ((checkedPoint.y > boundingRect.top) ? (checkedPoint.y - boundingRect.top) : (boundingRect.top - checkedPoint.y)));
							}
						}
					gotal:;
						if (tsum < sum) {

							sum = tsum;
							closestPoint = iStitch + ColorChangeTable[iChange].stitchIndex;
						}
					}
				}
			}
		}
	}
	if (sum > tolerance)
		return 0xffffffff;
	else
		return closestPoint;
}

void ilin() {

	SelectObject(StitchWindowDC, LinePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_XORPEN);
	Polyline(StitchWindowDC, &InsertLine[1], 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	SelectObject(StitchWindowMemDC, LinePen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	Polyline(StitchWindowMemDC, &InsertLine[1], 2);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void xlin() {

	if (rstMap(ILIN))
		ilin();
}

void ilin1() {

	SelectObject(StitchWindowDC, LinePen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void xlin1() {

	if (rstMap(ILIN1))
		ilin1();
}

void duIns() {

	POINT tlin[2];
	// ToDo - what is tlin being used for?
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	stch2px1(ClosestPointIndex);
	tlin[0].x = InsertLine[0].x = StitchCoordinatesPixels.x;
	tlin[0].y = InsertLine[0].y = StitchCoordinatesPixels.y;
	stch2px1(ClosestPointIndex + 1);
	tlin[1].x = InsertLine[2].x = StitchCoordinatesPixels.x;
	tlin[1].y = InsertLine[2].y = StitchCoordinatesPixels.y;
	xlin();
	setMap(ILIN);
	ilin();
}

void istch() {

	double angb, angf, angt;

	xlin();
	xlin1();
	if (chkMap(SELBOX)) {

		if (ClosestPointIndex&&ClosestPointIndex != (unsigned)PCSHeader.stitchCount - 1) {

			px2stch();
			angt = atan2(StitchBuffer[ClosestPointIndex].y - SelectedPoint.y, StitchBuffer[ClosestPointIndex].x - SelectedPoint.x);
			angb = atan2(StitchBuffer[ClosestPointIndex].y - StitchBuffer[ClosestPointIndex - 1].y, StitchBuffer[ClosestPointIndex].x - StitchBuffer[ClosestPointIndex - 1].x);
			angf = atan2(StitchBuffer[ClosestPointIndex].y - StitchBuffer[ClosestPointIndex + 1].y, StitchBuffer[ClosestPointIndex].x - StitchBuffer[ClosestPointIndex + 1].x);
			if (fabs(angf - angt) > fabs(angb - angt))
				ClosestPointIndex--;
		} else {

			if (ClosestPointIndex == (unsigned)PCSHeader.stitchCount - 1)
				ClosestPointIndex--;
		}
	} else
		ClosestPointIndex = closlin();
	if (ClosestPointIndex == -1)
		rstMap(INSRT);
	else {

		setMap(INSRT);
		duIns();
		SetCapture(ThrEdWindow);
		ritnum(STR_NUMSEL, ClosestPointIndex);
		nuAct(ClosestPointIndex);
	}
}

void cros(unsigned ind) {

	long armLength = BoxOffset[0];

	stch2px1(ind);
	InsertLine[0].x = StitchCoordinatesPixels.x - armLength;
	InsertLine[1].y = InsertLine[0].y = StitchCoordinatesPixels.y;
	InsertLine[1].x = StitchCoordinatesPixels.x + armLength;
	SelectObject(StitchWindowDC, CrossPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, InsertLine, 2);
	SelectObject(StitchWindowMemDC, CrossPen);
	SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	InsertLine[0].y = StitchCoordinatesPixels.y - armLength;
	InsertLine[0].x = InsertLine[1].x = StitchCoordinatesPixels.x;
	InsertLine[1].y = StitchCoordinatesPixels.y - 1;
	Polyline(StitchWindowDC, InsertLine, 2);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	InsertLine[0].y = StitchCoordinatesPixels.y + 2;
	InsertLine[1].y = StitchCoordinatesPixels.y + armLength;
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	Polyline(StitchWindowMemDC, InsertLine, 2);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void mark() {

	if (chkMap(SELBOX) || chkMap(INSRT) || closPnt1(&ClosestPointIndex)) {

		unbox();
		xlin();
		rstMap(CAPT);
		rstMap(SELBOX);
		rstMap(INSRT);
		rstMap(SCROS);
		rstMap(ECROS);
		GroupStitchIndex = ClosestPointIndex;
		setMap(GRPSEL);
	}
}

void selCol()
{
	unsigned iStitch, color;

	if (PCSHeader.stitchCount)
	{
		if (chkMap(SELBOX))
			iStitch = ClosestPointIndex;
		else
		{
			if (chkMap(GRPSEL))
				iStitch = GroupStitchIndex;
			else
				iStitch = 0;
		}
		if (iStitch > (unsigned)PCSHeader.stitchCount - 1)
			iStitch = PCSHeader.stitchCount - 1;
		ClosestPointIndex = GroupStitchIndex = iStitch;
		color = StitchBuffer[iStitch].attribute&COLMSK;
		while (ClosestPointIndex && (StitchBuffer[ClosestPointIndex].attribute&COLMSK) == color)
			ClosestPointIndex--;
		if ((StitchBuffer[ClosestPointIndex].attribute&COLMSK) != color)
			ClosestPointIndex++;
		while (GroupStitchIndex < PCSHeader.stitchCount && (StitchBuffer[GroupStitchIndex].attribute&COLMSK) == color)
			GroupStitchIndex++;
		if ((StitchBuffer[ClosestPointIndex].attribute&COLMSK) != color)
			ClosestPointIndex--;
		if (GroupStitchIndex > (unsigned)PCSHeader.stitchCount - 1)
			GroupStitchIndex = PCSHeader.stitchCount - 1;
		setMap(GRPSEL);
		unbox();
		grpAdj();
		ActiveColor = color;
		setMap(RESTCH);
	}
}

void nuAct(unsigned ind) {

	unsigned color;

	color = ActiveColor;
	ActiveColor = StitchBuffer[ind].attribute&COLMSK;
	redraw(UserColorWin[color]);
	redraw(UserColorWin[ActiveColor]);
}

void newFil() {

	unsigned iColor;

	rstMap(CMPDO);
	if (PCSBMPFileName[0]) {

		PCSBMPFileName[0] = 0;
		DeleteObject(BitmapFileHandle);
		ReleaseDC(ThrEdWindow, BitmapDC);
	}
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRED], IniFile.designerName);
	deldu();
	SetWindowText(ThrEdWindow, MsgBuffer);
	strcpy_s(ThrName, StringTable[STR_NUFIL]);
	ritfnam(IniFile.designerName);
	strcpy_s(ExtendedHeader.modifierName, IniFile.designerName);
	rstdu();
	rstAll();
	clrhbut(3);
	rstMap(MOVSET);
	PCSHeader.leadIn = 0x32;
	PCSHeader.colorCount = 16;
	unbox();
	xlin();
	xlin1();
	rstMap(INIT);
	rstMap(INSRT);
	rstMap(LIN1);
	rstMap(FORMSEL);
	rstMap(BAKACT);
	rstMap(GMRK);
	PCSHeader.stitchCount = 0;
	DisplayedColorBitmap = 0;
	PCSBMPFileName[0] = 0;
	PCSHeader.stitchCount = 0;
	FormVertexIndex = 0;
	ClipPointIndex = 0;
	TextureIndex = 0;
	SatinConnectIndex = 0;
	FormIndex = 0;
	WorkingFileName[0] = 0;
	ColorChanges = 0;
	KnotCount = 0;
	for (iColor = 0; iColor < 16; iColor++) {

		redraw(DefaultColorWin[iColor]);
		redraw(UserColorWin[iColor]);
		redraw(ThreadSizeWin[iColor]);
	}
	zumhom();
}

void bBox() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, ZoomBoxLine, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unbBox() {

	if (rstMap(BZUM))
		bBox();
}

void rebox() {

	unsigned iColor;

	unbox();
	if (closPnt1(&ClosestPointIndex)) {

		nuAct(ClosestPointIndex);
		if (stch2px(ClosestPointIndex))
			dubox();
		if (rstMap(GRPSEL)) {

			rstMap(SCROS);
			rstMap(ECROS);
			SearchLineIndex = 0;
			setMap(RESTCH);
			for (iColor = 0; iColor < 16; iColor++)
				redraw(UserColorWin[iColor]);
		}
		ritcor(&StitchBuffer[ClosestPointIndex]);
	}
}

void endpnt() {

	unbox();
	xlin();
	xlin1();
	setMap(LIN1);
	setMap(INSRT);
	rstMap(GRPSEL);
	InsertLine[0].x = StitchCoordinatesPixels.x;
	InsertLine[0].y = StitchCoordinatesPixels.y;
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
}

void delstchm() {

	unsigned iStitch, destination;

	rngadj();
	destination = GroupStartStitch;
	for (iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {

		StitchBuffer[destination].attribute = StitchBuffer[iStitch].attribute;
		StitchBuffer[destination].x = StitchBuffer[iStitch].x;
		StitchBuffer[destination++].y = StitchBuffer[iStitch].y;
	}
	PCSHeader.stitchCount = destination - 1;
}

void unsid() {

	FormMenuChoice = 0;
	if (SideMessageWindow) {

		DestroyWindow(SideMessageWindow);
		SideMessageWindow = 0;
	}
}

void unbsho() {

	unsigned iBackup;

	if (rstMap(BAKSHO)) {

		for (iBackup = 0; iBackup < OLDVER; iBackup++)
			DestroyWindow(BackupViewer[iBackup]);
	}
}

BOOL oldwnd(HWND window) {

	unsigned iWindow, iColor;

	for (iColor = 0; iColor < 16; iColor++) {

		if (DefaultColorWin[iColor] == window || UserColorWin[iColor] == window || ThreadSizeWin[iColor] == window)
			return 0;
	}
	for (iWindow = 0; iWindow < 9; iWindow++) {

		if (ButtonWin[iWindow] == window)
			return 0;
	}
	if (MainStitchWin == window)
		return 0;
	if (VerticalScrollBar == window)
		return 0;
	if (HorizontalScrollBar == window)
		return 0;
	if (ColorBar == window)
		return 0;
	if (SpeedScrollBar == window)
		return 0;
	return 1;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	if (oldwnd(hwnd))
		return DestroyWindow(hwnd);
	else {

		if (FirstWin) {

			if (FirstWin == hwnd)
				return 0;
			else
				return TRUE;
		} else {

			FirstWin = hwnd;
			return TRUE;
		}
	}
}

void insadj() {

	rstMap(PRELIN);
	fvars(ClosestFormToCursor);
	refil();
	coltab();
	frmout(ClosestFormToCursor);
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
	butxt(HUPTO, StringTable[STR_UPOF]);
	if (ZoomFactor == 1)
		rstMap(ZUMED);
	movStch();
	ShowWindow(MainStitchWin, TRUE);
	unbsho();
	rstMap(SIDACT);
	unsid();
	rstMap(PRFACT);
	rstMap(WASRT);
	DestroyWindow(PreferencesWindow);
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
	SearchLineIndex = SelectedFormCount = 0;
	FirstWin = 0;
	while (EnumChildWindows(MainStitchWin, EnumChildProc, 0));
}

void rstdu() {

	deldu();
	rstMap(REDUSHO);
	rstMap(UNDUSHO);
	EnableMenuItem(MainMenu, M_UNDO, MF_BYPOSITION | MF_GRAYED);
	EnableMenuItem(MainMenu, M_REDO, MF_BYPOSITION | MF_GRAYED);
	setMap(DUMEN);
}

CLPSTCH* deref(void* point) {

#if  __UseASM__
	_asm {

		mov		eax, point
		mov		eax, [eax]
	}
#else
	//check translation
	return *(CLPSTCH **)point;
#endif
}

void duclp() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, ClipInsertBoxLine, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unclp() {

	if (rstMap(CLPSHO))
		duclp();
}

void dusel(HDC dc) {

	unsigned ind;

	SetROP2(dc, R2_NOTXORPEN);
	SelectObject(dc, LinePen);
	Polyline(dc, FormOutlineRect, 9);
	for (ind = 0; ind < 8; ind++)
		selsqr(FormOutlineRect[ind], dc);
	SetROP2(dc, R2_COPYPEN);
}

void unsel() {

	if (rstMap(SELSHO))
		dusel(StitchWindowDC);
}

void clpbox() {

	SIZE	adjustedSize;
	double	ratio;

	px2stch();
	if (SelectedPoint.x + ClipRectSize.cx > UnzoomedRect.x)
		SelectedPoint.x = UnzoomedRect.x - ClipRectSize.cx;
	if (SelectedPoint.y + ClipRectSize.cy > UnzoomedRect.y)
		SelectedPoint.y = UnzoomedRect.y - ClipRectSize.cy;
	ClipOrigin.x = SelectedPoint.x;
	ClipOrigin.y = SelectedPoint.y;
	ratio = (double)StitchWindowClientRect.right / (ZoomRect.right - ZoomRect.left);
	adjustedSize.cx = ClipRectSize.cx*ratio + 0.5;
	adjustedSize.cy = ClipRectSize.cy*ratio + 0.5;
	StitchCoordinatesPixels.x = (SelectedPoint.x - ZoomRect.left)*ratio + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (SelectedPoint.y - ZoomRect.bottom)*ratio + 0.5 - adjustedSize.cy;
	ClipInsertBoxLine[0].x = ClipInsertBoxLine[3].x = ClipInsertBoxLine[4].x = StitchCoordinatesPixels.x;
	ClipInsertBoxLine[0].y = ClipInsertBoxLine[1].y = ClipInsertBoxLine[4].y = StitchCoordinatesPixels.y;
	ClipInsertBoxLine[1].x = ClipInsertBoxLine[2].x = ClipInsertBoxLine[0].x + adjustedSize.cx;
	ClipInsertBoxLine[2].y = ClipInsertBoxLine[3].y = ClipInsertBoxLine[0].y + adjustedSize.cy;
	setMap(CLPSHO);
	duclp();
}

void lodclp(unsigned iStitch) {

	fPOINT		adjustment;
	unsigned	source, destination;

	if (iStitch != PCSHeader.stitchCount)
		iStitch++;
	adjustment.x = ClipOrigin.x - ClipRect.left;
	adjustment.y = ClipOrigin.y - ClipRect.bottom;
	if (iStitch < (unsigned)PCSHeader.stitchCount&&chkMap(INIT)) {

		source = PCSHeader.stitchCount - 1;
		destination = PCSHeader.stitchCount + ClipStitchCount - 1;
		while (source >= iStitch) {

			StitchBuffer[destination].x = StitchBuffer[source].x;
			StitchBuffer[destination].y = StitchBuffer[source--].y;
			StitchBuffer[destination--].attribute = StitchBuffer[source].attribute;
		}
	}
	ClosestPointIndex = iStitch;
	for (source = 0; source < ClipStitchCount; source++) {

		StitchBuffer[iStitch].x = ClipBuffer[source].x + adjustment.x;
		StitchBuffer[iStitch].y = ClipBuffer[source].y + adjustment.y;
		StitchBuffer[iStitch++].attribute = ClipBuffer[source].attribute&COLMSK | LayerIndex | NOTFRM;
	}
	GroupStitchIndex = iStitch - 1;
	setMap(GRPSEL);
	PCSHeader.stitchCount += ClipStitchCount;
	if (PCSHeader.stitchCount)
		setMap(INIT);
}

void rSelbox() {

	SIZE	adjustedSelectSize;
	double	ratio;

	unsel();
	px2stch();
	if (SelectedPoint.x - SelectBoxOffset.x + SelectBoxSize.cx >= UnzoomedRect.x)
		SelectedPoint.x = UnzoomedRect.x - SelectBoxSize.cx + SelectBoxOffset.x;
	if (SelectedPoint.y - SelectBoxOffset.y + SelectBoxSize.cy >= UnzoomedRect.y)
		SelectedPoint.y = UnzoomedRect.y - SelectBoxSize.cy + SelectBoxOffset.y;
	if (SelectedPoint.x - SelectBoxOffset.x < 0)
		SelectedPoint.x = SelectBoxOffset.x;
	if (SelectedPoint.y - SelectBoxOffset.y < 0)
		SelectedPoint.y = SelectBoxOffset.y;
	ratio = (double)StitchWindowClientRect.right / (ZoomRect.right - ZoomRect.left);
	adjustedSelectSize.cx = SelectBoxSize.cx*ratio + 0.5;
	adjustedSelectSize.cy = SelectBoxSize.cy*ratio + 0.5;
	StitchCoordinatesPixels.x = (SelectedPoint.x - ZoomRect.left - SelectBoxOffset.x)*ratio + 0.5;
	StitchCoordinatesPixels.y = StitchWindowClientRect.bottom - (SelectedPoint.y - ZoomRect.bottom - SelectBoxOffset.y)*ratio + 0.5 - adjustedSelectSize.cy;
	FormOutlineRect[0].x = FormOutlineRect[6].x = FormOutlineRect[7].x = FormOutlineRect[8].x = StitchCoordinatesPixels.x;
	FormOutlineRect[1].x = FormOutlineRect[5].x = StitchCoordinatesPixels.x + adjustedSelectSize.cx / 2;
	FormOutlineRect[0].y = FormOutlineRect[1].y = FormOutlineRect[2].y = FormOutlineRect[8].y = StitchCoordinatesPixels.y;
	FormOutlineRect[3].y = FormOutlineRect[7].y = StitchCoordinatesPixels.y + adjustedSelectSize.cy / 2;
	FormOutlineRect[4].y = FormOutlineRect[5].y = FormOutlineRect[6].y = StitchCoordinatesPixels.y + adjustedSelectSize.cy;
	FormOutlineRect[2].x = FormOutlineRect[3].x = FormOutlineRect[4].x = StitchCoordinatesPixels.x + adjustedSelectSize.cx;
	setMap(SELSHO);
	dusel(StitchWindowDC);
}

void duSelbox() {

	px2stch();
	SelectBoxSize.cx = StitchRangeRect.right - StitchRangeRect.left;
	SelectBoxSize.cy = StitchRangeRect.top - StitchRangeRect.bottom;
	SelectBoxOffset.x = SelectedPoint.x - StitchRangeRect.left;
	SelectBoxOffset.y = SelectedPoint.y - StitchRangeRect.bottom;
}

void setbak(unsigned penWidth) {

	if (BackgroundPenWidth != penWidth)
		BackgroundPen = nuPen(BackgroundPen, penWidth, BackgroundColor);
}

void stchbox(unsigned iStitch, HDC dc) {

	POINT		line[5];
	unsigned	layer;

	layer = (StitchBuffer[iStitch].attribute&LAYMSK) >> LAYSHFT;
	if (!ActiveLayer || !layer || layer == ActiveLayer) {

		stch2px1(iStitch);
		line[0].x = line[3].x = line[4].x = StitchCoordinatesPixels.x - IniFile.stitchSizePixels;
		line[0].y = line[1].y = StitchCoordinatesPixels.y - IniFile.stitchSizePixels;
		line[1].x = line[2].x = StitchCoordinatesPixels.x + IniFile.stitchSizePixels;
		line[2].y = line[3].y = StitchCoordinatesPixels.y + IniFile.stitchSizePixels;
		line[4].y = StitchCoordinatesPixels.y - IniFile.stitchSizePixels;
		Polyline(dc, line, 5);
	}
}

void sCor2px(dPOINT stitchCoordinate, POINT* pixelCoordinate) {

	pixelCoordinate->x = (stitchCoordinate.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	pixelCoordinate->y = StitchWindowClientRect.bottom + (ZoomRect.bottom - stitchCoordinate.y)*ZoomRatio.y + 0.5;
}

void sdCor2px(fPOINTATTR stitchPoint, POINT* pixelCoordinate) {

	pixelCoordinate->x = (stitchPoint.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	pixelCoordinate->y = StitchWindowClientRect.bottom + (ZoomRect.bottom - stitchPoint.y)*ZoomRatio.y + 0.5;
}

void durot() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, RotateBoxOutline, 5);
	Polyline(StitchWindowDC, RotateBoxCrossVertLine, 2);
	Polyline(StitchWindowDC, RotateBoxCrossHorzLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unrot() {

	if (rstMap(ROTSHO))
		durot();
}

void durotu() {

	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	SelectObject(StitchWindowDC, LinePen);
	Polyline(StitchWindowDC, RotateBoxToCursorLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unrotu() {

	if (rstMap(ROTUSHO))
		durotu();
}

void rotang(dPOINT unrotatedPoint, POINT* rotatedPoint) {

	double	distanceToCenter, newAngle;
	dPOINT	point;
	long	dx, dy;

	dx = unrotatedPoint.x - RotationCenter.x;
	dy = unrotatedPoint.y - RotationCenter.y;
	if (dx) {

		distanceToCenter = hypot(dx, dy);
		newAngle = atan2(dy, dx);
		newAngle += RotationAngle;
	} else {

		distanceToCenter = abs(dy);
		if (dy > 0)
			newAngle = PI / 2 + RotationAngle;
		else
			newAngle = RotationAngle - PI / 2;
	}
	point.y = RotationCenter.y + distanceToCenter*sin(newAngle);
	point.x = RotationCenter.x + distanceToCenter*cos(newAngle);
	sCor2px(point, rotatedPoint);
}

void rotang1(fPOINTATTR unrotatedPoint, fPOINT* rotatedPoint) {

	double	distanceToCenter, newAngle;
	double	dx, dy;

	dx = unrotatedPoint.x - RotationCenter.x;
	dy = unrotatedPoint.y - RotationCenter.y;
	if (dx) {

		distanceToCenter = hypot(dx, dy);
		newAngle = atan2(dy, dx);
		newAngle += RotationAngle;
	} else {

		if (dy > 0) {
			distanceToCenter = dy;
			newAngle = PI / 2 + RotationAngle;
		} else {
			distanceToCenter = -dy;
			newAngle = RotationAngle - PI / 2;
		}
	}
	rotatedPoint->y = RotationCenter.y + distanceToCenter*sin(newAngle);
	rotatedPoint->x = RotationCenter.x + distanceToCenter*cos(newAngle);
}

void rotangf(fPOINT unrotatedPoint, fPOINT* rotatedPoint) {

	double	distanceToCenter, newAngle;
	double	dx, dy;

	dx = unrotatedPoint.x - RotationCenter.x;
	dy = unrotatedPoint.y - RotationCenter.y;
	if (dx) {

		distanceToCenter = hypot(dx, dy);
		newAngle = atan2(dy, dx);
		newAngle += RotationAngle;
	} else {

		if (dy > 0) {
			distanceToCenter = dy;
			newAngle = PI / 2 + RotationAngle;
		} else {
			distanceToCenter = -dy;
			newAngle = RotationAngle - PI / 2;
		}
	}
	rotatedPoint->y = RotationCenter.y + distanceToCenter*sin(newAngle);
	rotatedPoint->x = RotationCenter.x + distanceToCenter*cos(newAngle);
}

void rotpix(POINT unrotatedPoint, POINT* rotatedPoint) {

	//won't handle vertical lines
	double	distanceToCenter, newAngle;
	long	dx, dy;

	dx = unrotatedPoint.x - RotationCenterPixels.x;
	dy = unrotatedPoint.y - RotationCenterPixels.y;
	distanceToCenter = hypot(dx, dy);
	newAngle = atan2(dy, dx);
	newAngle -= RotateAngle;
	rotatedPoint->y = RotationCenterPixels.y + distanceToCenter*sin(newAngle);
	rotatedPoint->x = RotationCenterPixels.x + distanceToCenter*cos(newAngle);
}

void rotflt(fPOINT* point) {

	double	len, ang0;
	double	dx, dy;

	dx = point->x - RotationCenter.x;
	dy = point->y - RotationCenter.y;
	if (dx) {

		len = hypot(dx, dy);
		ang0 = atan2(dy, dx);
		ang0 += RotationAngle;
	} else {

		if (dy > 0) {
			len = dy;
			ang0 = PI / 2 + RotationAngle;
		} else {
			len = -dy;
			ang0 = RotationAngle - PI / 2;
		}
	}
	point->y = RotationCenter.y + len*sin(ang0);
	point->x = RotationCenter.x + len*cos(ang0);
}

void rotstch(fPOINTATTR* stitch) {

	double	distanceToCenter, newAngle;
	double	dx, dy;

	dx = stitch->x - RotationCenter.x;
	dy = stitch->y - RotationCenter.y;
	if (dx) {

		distanceToCenter = hypot(dx, dy);
		newAngle = atan2(dy, dx);
		newAngle += RotationAngle;
	} else {

		if (dy > 0) {
			distanceToCenter = dy;
			newAngle = PI / 2 + RotationAngle;
		} else {
			distanceToCenter = -dy;
			newAngle = RotationAngle - PI / 2;
		}
	}
	stitch->y = RotationCenter.y + distanceToCenter*sin(newAngle);
	stitch->x = RotationCenter.x + distanceToCenter*cos(newAngle);
}

void ritrot() {

	POINT		rotated;
	dPOINT		rotationReference;

	rotationReference.x = RotationRect.left;
	rotationReference.y = RotationRect.top;
	rotang(rotationReference, &rotated);
	RotateBoxOutline[0].x = RotateBoxOutline[4].x = rotated.x;
	RotateBoxOutline[0].y = RotateBoxOutline[4].y = rotated.y;
	rotationReference.x = RotationCenter.x;
	rotang(rotationReference, &rotated);
	RotateBoxCrossVertLine[0].x = rotated.x;
	RotateBoxCrossVertLine[0].y = rotated.y;
	rotationReference.x = RotationRect.right;
	rotang(rotationReference, &rotated);
	RotateBoxOutline[1].x = rotated.x;
	RotateBoxOutline[1].y = rotated.y;
	rotationReference.y = RotationCenter.y;
	rotang(rotationReference, &rotated);
	RotateBoxCrossHorzLine[1].x = rotated.x;
	RotateBoxCrossHorzLine[1].y = rotated.y;
	rotationReference.y = RotationRect.bottom;
	rotang(rotationReference, &rotated);
	RotateBoxOutline[2].x = rotated.x;
	RotateBoxOutline[2].y = rotated.y;
	rotationReference.x = RotationCenter.x;
	rotang(rotationReference, &rotated);
	RotateBoxCrossVertLine[1].x = rotated.x;
	RotateBoxCrossVertLine[1].y = rotated.y;
	rotationReference.x = RotationRect.left;
	rotang(rotationReference, &rotated);
	RotateBoxOutline[3].x = rotated.x;
	RotateBoxOutline[3].y = rotated.y;
	rotationReference.y = RotationCenter.y;
	rotang(rotationReference, &rotated);
	RotateBoxCrossHorzLine[0].x = rotated.x;
	RotateBoxCrossHorzLine[0].y = rotated.y;
	sCor2px(RotationCenter, &RotateBoxToCursorLine[0]);
	setMap(ROTSHO);
	durot();
}

void durcntr() {

	RotationCenter.x = midl(RotationRect.right, RotationRect.left);
	RotationCenter.y = midl(RotationRect.top, RotationRect.bottom);
}

void rot() {

	if (chkMap(FPSEL)) {

		MoveMemory(&RotationRect, &SelectedPointsLine, sizeof(fRECTANGLE));
		goto rotskp;
	}
	if (chkMap(BIGBOX)) {

		MoveMemory(&RotationRect, &AllItemsRect, sizeof(fRECTANGLE));
		goto rotskp;
	}
	if (SelectedFormCount) {

		pxrct2stch(SelectedFormsRect, &RotationRect);
		setMap(FRMSROT);
		goto rotskp;
	}
	if (chkMap(FORMSEL)) {

		fvars(ClosestFormToCursor);
		setMap(FRMROT);
		MoveMemory(&RotationRect, &SelectedForm->rectangle, sizeof(fRECTANGLE));
		goto rotskp;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		selRct(&RotationRect);
		goto rotskp;
	}
	shoseln(IDS_FGRPF, IDS_ROT);
	return;
rotskp:;
	setMap(ROTAT);
	durcntr();
	RotationAngle = 0;
	ritrot();
}

void savclp(unsigned destination, unsigned source) {

	double	frct, intg;

	ClipStitchData[destination].led = StitchBuffer[source].attribute&COLMSK;
	frct = modf((double)StitchBuffer[source].x - LowerLeftStitch.x, &intg);
	ClipStitchData[destination].fx = frct * 256;
	ClipStitchData[destination].x = intg;
	ClipStitchData[destination].spcx = 0;
	frct = modf((double)StitchBuffer[source].y - LowerLeftStitch.y, &intg);
	ClipStitchData[destination].fy = frct * 256;
	ClipStitchData[destination].y = intg;
	ClipStitchData[destination].spcy = 0;
	// ToDo - Are these structure members needed?
	ClipStitchData[destination].myst = 1;
	ClipStitchData[destination].tag = 0x14;
}

void rtclpfn(unsigned destination, unsigned source) {

	double	fractional, integer;

	ClipStitchData[destination].led = 0;
	fractional = modf(ClipBuffer[source].x - LowerLeftStitch.x, &integer);
	ClipStitchData[destination].fx = fractional;
	ClipStitchData[destination].x = integer;
	ClipStitchData[destination].spcx = 0;
	fractional = modf(ClipBuffer[source].y - LowerLeftStitch.y, &integer);
	ClipStitchData[destination].fy = fractional;
	ClipStitchData[destination].y = integer;
	ClipStitchData[destination].spcy = 0;
	// ToDo - Are these structure members needed?
	ClipStitchData[destination].myst = 1;
	ClipStitchData[destination].tag = 0x14;
}

FORMCLIP* frmref(void* clipForm) {

#if  __UseASM__
	_asm {

		mov		eax, clipForm
		mov		eax, [eax]
	}
#else
	return *(FORMCLIP **)clipForm;
#endif
}

FORMSCLIP* frmsref(void* clipForms) {

#if  __UseASM__
	_asm {

		mov		eax, clipForms
		mov		eax, [eax]
	}
#else
	return *(FORMSCLIP **)clipForms;
#endif
}

FORMVERTEXCLIP* fpref(void* clipVertex) {

#if  __UseASM__
	_asm {

		mov		eax, clipVertex
		mov		eax, [eax]
	}
#else
	return *(FORMVERTEXCLIP **)clipVertex;
#endif
}

unsigned sizfclp()
{
	unsigned clipSize;

	clipSize = sizeof(FORMCLIP) + SideCount * sizeof(fPOINT);
	if (SelectedForm->type == SAT)
		clipSize += SelectedForm->satinGuideCount * sizeof(SATCON);
	if (iseclp(ClosestFormToCursor))
		clipSize += SelectedForm->clipEntries * sizeof(fPOINT);
	if (isclpx(ClosestFormToCursor))
		clipSize += SelectedForm->lengthOrCount.clipCount * sizeof(fPOINT);
	if (istx(ClosestFormToCursor))
		clipSize += SelectedForm->fillInfo.texture.count * sizeof(TXPNT);
	return clipSize;
}

unsigned frmcnt(unsigned iForm) {

	unsigned	codedAttribute, stitchCount = 0, iStitch;

	LowerLeftStitch.x = LowerLeftStitch.y = (float)1e20;
	// ToDo should this be using FRMSHFT?
	codedAttribute = iForm << 4;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if ((StitchBuffer[iStitch].attribute&FRMSK) == codedAttribute&&StitchBuffer[iStitch].attribute&TYPMSK)
			goto fskip;
	}
	return 0;
fskip:;
	FormFirstStitchIndex = iStitch;
	while (iStitch < PCSHeader.stitchCount) {

		if ((StitchBuffer[iStitch].attribute&FRMSK) == codedAttribute&&StitchBuffer[iStitch].attribute&TYPMSK) {

			if (StitchBuffer[iStitch].x < LowerLeftStitch.x)
				LowerLeftStitch.x = StitchBuffer[iStitch].x;
			if (StitchBuffer[iStitch].y < LowerLeftStitch.y)
				LowerLeftStitch.y = StitchBuffer[iStitch].y;
			stitchCount++;
		}
		iStitch++;
	}
	return stitchCount;
}

unsigned sizclp() {

	unsigned length = 0;

	length = FileSize = sizeof(FORMCLIP) + SideCount * sizeof(fPOINT);
	if (SelectedForm->type == SAT)
		FileSize += SelectedForm->satinGuideCount * sizeof(SATCON);
	if (SelectedForm->fillType || SelectedForm->edgeType) {

		length += frmcnt(ClosestFormToCursor);
		FileSize += length * sizeof(fPOINTATTR);
	}
	if (iseclp(ClosestFormToCursor))
		FileSize += SelectedForm->clipEntries * sizeof(fPOINT);
	if (isclpx(ClosestFormToCursor))
		FileSize += SelectedForm->lengthOrCount.clipCount * sizeof(fPOINT);
	if (istx(ClosestFormToCursor))
		FileSize += SelectedForm->fillInfo.texture.count * sizeof(TXPNT);
	
	return length;
}

unsigned lenclp()
{
	unsigned codedAttribute, iStitch, stitchCount;

	stitchCount = 0;
	codedAttribute = ClosestFormToCursor << FRMSHFT;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
	{
		if ((StitchBuffer[iStitch].attribute&(FRMSK | NOTFRM)) == codedAttribute)
			stitchCount++;
	}
	return stitchCount;
}

void duclip() {

	unsigned		iClip, iDestination, iForm, iGuide;
	unsigned		iSide, iStitch, iSource, iTexture, iVertex;
	unsigned		guideCount, pointCount, textureCount, stitchCount;
	unsigned		length = 0, codedAttribute, msiz = 0;
	SATCON*			guides;
	fPOINT*			points;
	fPOINT*			mclp;
	TXPNT*			textures;
	TXPNT*			ptxs;
	FRMHED*			forms;
	fPOINTATTR*		astch;
	FORMVERTEXCLIP*	clipHeader;
	fPOINT*			vertices;

	if (chkMap(FPSEL)) {

		if (OpenClipboard(ThrEdWindow)) {

			EmptyClipboard();
			ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
			ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (SelectedFormVertices.vertexCount + 1) * sizeof(fPOINT) + sizeof(FORMVERTEXCLIP));
			clipHeader = fpref(ThrEdClipPointer);
			clipHeader->clipType = CLP_FRMPS;
			clipHeader->vertexCount = SelectedFormVertices.vertexCount;
			if (chkMap(PSELDIR))
				clipHeader->direction = 1;
			else
				clipHeader->direction = 0;
			// skip past the header
			vertices = (fPOINT*)&clipHeader[1];
			fvars(ClosestFormToCursor);
			iSource = SelectedFormVertices.start;
			for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {

				vertices[iVertex].x = CurrentFormVertices[iSource].x;
				vertices[iVertex].y = CurrentFormVertices[iSource].y;
				iSource = pdir(iSource);
			}
			SetClipboardData(ThrEdClip, ThrEdClipPointer);
			CloseClipboard();
		}
		return;
	}
	if (chkMap(BIGBOX))
		tabmsg(IDS_INSF);
	else {

		if (OpenClipboard(ThrEdWindow)) {

			EmptyClipboard();
			ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
			if (SelectedFormCount) {

				for (iForm = 0; iForm < SelectedFormCount; iForm++) {

					ClosestFormToCursor = SelectedFormList[iForm];
					fvars(ClosestFormToCursor);
					length += sizfclp();
					msiz += FileSize;
				}
				ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, msiz + length);
				ClipFormsHeader = frmsref(ThrEdClipPointer);
				ClipFormsHeader->clipType = CLP_FRMS;
				ClipFormsHeader->formCount = SelectedFormCount;
				// Skip past the header
				forms = (FRMHED*)&ClipFormsHeader[1];
				for (iForm = 0; iForm < SelectedFormCount; iForm++) {

					SelectedForm = &FormList[SelectedFormList[iForm]];
					frmcpy(&forms[iForm], &FormList[SelectedFormList[iForm]]);
				}
				// skip past the forms
				CurrentFormVertices = (fPOINT*)&forms[iForm];
				iVertex = 0;
				for (iForm = 0; iForm < SelectedFormCount; iForm++) {

					SelectedForm = &FormList[SelectedFormList[iForm]];
					for (iSide = 0; iSide < SelectedForm->sides; iSide++) {

						CurrentFormVertices[iVertex].x = SelectedForm->vertices[iSide].x;
						CurrentFormVertices[iVertex++].y = SelectedForm->vertices[iSide].y;
					}
				}
				// skip past the vertex list
				guides = (SATCON*)&CurrentFormVertices[iVertex];
				guideCount = 0;
				for (iForm = 0; iForm < SelectedFormCount; iForm++) {

					SelectedForm = &FormList[SelectedFormList[iForm]];
					if (SelectedForm->type == SAT) {

						for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
							
							guides[guideCount].start = SelectedForm->satinOrAngle.sac[iGuide].start;
							guides[guideCount++].finish = SelectedForm->satinOrAngle.sac[iGuide].finish;
						}
					}
				}
				// skip past the guides
				points = (fPOINT*)&guides[guideCount];
				pointCount = 0;
				for (iForm = 0; iForm < SelectedFormCount; iForm++) {

					SelectedForm = &FormList[SelectedFormList[iForm]];
					if (isclpx(SelectedFormList[iForm])) {

						for (iClip = 0; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {

							points[pointCount].x = SelectedForm->angleOrClipData.clip[iClip].x;
							points[pointCount++].y = SelectedForm->angleOrClipData.clip[iClip].y;
						}
					}
					if (iseclp(SelectedFormList[iForm])) {

						for (iClip = 0; iClip < SelectedForm->clipEntries; iClip++) {

							points[pointCount].x = SelectedForm->borderClipData[iClip].x;
							points[pointCount++].y = SelectedForm->borderClipData[iClip].y;
						}
					}
				}
				// Skip past the points
				textures = (TXPNT*)&points[pointCount];
				textureCount = 0;
				for (iForm = 0; iForm < SelectedFormCount; iForm++)
				{
					SelectedForm = &FormList[SelectedFormList[iForm]];
					if (istx(SelectedFormList[iForm]))
					{
						MoveMemory(&textures[textureCount], &TexturePointsBuffer[SelectedForm->fillInfo.texture.index], SelectedForm->fillInfo.texture.count * sizeof(TXPNT));
						forms[iForm].fillInfo.texture.index = textureCount;
						textureCount += SelectedForm->fillInfo.texture.count;
					}
				}
				SetClipboardData(ThrEdClip, ThrEdClipPointer);
				CloseClipboard();
				clRmap(RMAPSIZ);
				for (iForm = 0; iForm < SelectedFormCount; iForm++)
					setr(SelectedFormList[iForm]);
				astch = &StitchBuffer[MAXSEQ];
				stitchCount = 0;
				LowerLeftStitch.x = LowerLeftStitch.y = (float)1e30;
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

					if (!(StitchBuffer[iStitch].attribute&NOTFRM) && chkr((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT)) {

						if (StitchBuffer[iStitch].x < LowerLeftStitch.x)
							LowerLeftStitch.x = StitchBuffer[iStitch].x;
						if (StitchBuffer[iStitch].y < LowerLeftStitch.y)
							LowerLeftStitch.y = StitchBuffer[iStitch].y;
						astch[stitchCount].x = StitchBuffer[iStitch].x;
						astch[stitchCount].y = StitchBuffer[iStitch].y;
						astch[stitchCount++].attribute = StitchBuffer[iStitch].attribute;
					}
				}
				if (PCSHeader.stitchCount&&stitchCount) {

					Clip = RegisterClipboardFormat(PcdClipFormat);
					ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, stitchCount * sizeof(CLPSTCH) + 2);
					ClipStitchData = deref(ClipPointer);
					iStitch = MAXSEQ;
					iDestination = 0;
					savclp(0, iStitch);
					iStitch++;
					ClipStitchData[0].led = stitchCount;
					iDestination++;
					while (iStitch < stitchCount + MAXSEQ)
						savclp(iDestination++, iStitch++);
					SetClipboardData(Clip, ClipPointer);
					CloseClipboard();
				}
			} else {

				if (chkMap(FORMSEL)) {

					length = sizclp();
					fvars(ClosestFormToCursor);
					// ToDo - use FRMSHFT?
					guides = 0; codedAttribute = ClosestFormToCursor << 4;
					FileSize += sizeof(FORMCLIP);
					ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, FileSize);
					ClipFormHeader = frmref(ThrEdClipPointer);
					ClipFormHeader->clipType = CLP_FRM;
					frmcpy(&ClipFormHeader->form, &FormList[ClosestFormToCursor]);
					CurrentFormVertices = (fPOINT*)&ClipFormHeader[1];
					for (iSide = 0; iSide < SelectedForm->sides; iSide++) {

						CurrentFormVertices[iSide].x = SelectedForm->vertices[iSide].x;
						CurrentFormVertices[iSide].y = SelectedForm->vertices[iSide].y;
					}
					guides = (SATCON*)&CurrentFormVertices[SideCount];
					iGuide = 0;
					if (SelectedForm->type == SAT) {

						for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {

							guides[iGuide].start = SelectedForm->satinOrAngle.sac[iGuide].start;
							guides[iGuide].finish = SelectedForm->satinOrAngle.sac[iGuide].finish;
						}
					}
					mclp = (fPOINT*)&guides[iGuide];
					iClip = 0;
					if (isclpx(ClosestFormToCursor)) {

						for (iClip = 0; iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {

							mclp[iClip].x = SelectedForm->angleOrClipData.clip[iClip].x;
							mclp[iClip].y = SelectedForm->angleOrClipData.clip[iClip].y;
						}
					}
					points = (fPOINT*)&mclp[iClip];
					if (iseclpx(ClosestFormToCursor)) {

						for (iClip = 0; iClip < SelectedForm->clipEntries; iClip++) {

							points[iClip].x = SelectedForm->borderClipData[iClip].x;
							points[iClip].y = SelectedForm->borderClipData[iClip].y;
						}
					}
					textures = (TXPNT*)&points[iClip];
					if (istx(ClosestFormToCursor))
					{
						ptxs = &TexturePointsBuffer[SelectedForm->fillInfo.texture.index];
						for (iTexture = 0; iTexture < SelectedForm->fillInfo.texture.count; iTexture++)
						{
							textures[iTexture].line = ptxs[iTexture].line;
							textures[iTexture].y = ptxs[iTexture].y;
						}
					}
					SetClipboardData(ThrEdClip, ThrEdClipPointer);
					iTexture = 0;
					if ((SelectedForm->fillType || SelectedForm->edgeType))
					{
						length = lenclp();
						Clip = RegisterClipboardFormat(PcdClipFormat);
						ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length * sizeof(CLPSTCH) + 2);
						ClipStitchData = deref(ClipPointer);
						iStitch = FormFirstStitchIndex;
						savclp(0, iTexture);
						ClipStitchData[0].led = length;
						iTexture++;
						iDestination = 1;
						// ToDo - use FRMSHFT?
						codedAttribute = ClosestFormToCursor << 4;
						while (iTexture < PCSHeader.stitchCount) {

							if ((StitchBuffer[iTexture].attribute&FRMSK) == codedAttribute && !(StitchBuffer[iTexture].attribute&NOTFRM))
								savclp(iDestination++, iTexture);
							iTexture++;
						}
						SetClipboardData(Clip, ClipPointer);
						ispcdclp();
					}
					CloseClipboard();
				} else {

					if (PCSHeader.stitchCount&&chkMap(GRPSEL)) {

						Clip = RegisterClipboardFormat(PcdClipFormat);
						rngadj();
						LowerLeftStitch.x = LowerLeftStitch.y = (float)1e30;
						for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {

							if (StitchBuffer[iStitch].x < LowerLeftStitch.x)
								LowerLeftStitch.x = StitchBuffer[iStitch].x;
							if (StitchBuffer[iStitch].y < LowerLeftStitch.y)
								LowerLeftStitch.y = StitchBuffer[iStitch].y;
						}
						length = GroupEndStitch - GroupStartStitch + 1;
						iSource = GroupStartStitch;
						ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length * sizeof(CLPSTCH) + 2);
						ClipStitchData = deref(ClipPointer);
						savclp(0, iSource);
						ClipStitchData[0].led = length;
						// ToDo - check use of iSource as the destination index
						iSource++;
						for (iStitch = 1; iStitch < length; iStitch++)
							savclp(iStitch, iSource++);
						SetClipboardData(Clip, ClipPointer);
						CloseClipboard();
					}
				}
			}
			CloseClipboard();
		}
	}
}

void delfstchs() {

	unsigned iSourceStitch, iDestinationStitch = 0;

	for (iSourceStitch = 0; iSourceStitch < PCSHeader.stitchCount; iSourceStitch++) {

		if (StitchBuffer[iSourceStitch].attribute&NOTFRM) {

			StitchBuffer[iDestinationStitch].attribute = StitchBuffer[iSourceStitch].attribute;
			StitchBuffer[iDestinationStitch].x = StitchBuffer[iSourceStitch].x;
			StitchBuffer[iDestinationStitch++].y = StitchBuffer[iSourceStitch].y;
		} else {

			if (((StitchBuffer[iSourceStitch].attribute&FRMSK) >> 4) != ClosestFormToCursor) {

				StitchBuffer[iDestinationStitch].attribute = StitchBuffer[iSourceStitch].attribute;
				StitchBuffer[iDestinationStitch].x = StitchBuffer[iSourceStitch].x;
				StitchBuffer[iDestinationStitch++].y = StitchBuffer[iSourceStitch].y;
			}
		}
	}
	PCSHeader.stitchCount = iDestinationStitch;
}

void cut() {

	duclip();
	if (SelectedFormCount)
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

	RECT messageRect;

	GetClientRect(MsgWindow, &messageRect);
	GeneralNumberInputBox = CreateWindow(
		"STATIC",
		0,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		messageRect.bottom + 15,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThrEdInstance,
		NULL);
	MsgIndex = 0;
	*MsgBuffer = 0;
}

void unmsg() {

	if (MsgWindow)
		noMsg();
}

void unpat() {

	if (rstMap(WASPAT)) {

		ShowWindow(SpeedScrollBar, FALSE);
		DestroyWindow(SpeedScrollBar);
		movStch();
		setMap(RESTCH);
	}
}
void delsmal(unsigned startStitch, unsigned endStitch) {
	// ToDo - does this function work correctly?
	// ToDo - rename ine and inf
	unsigned	iStitch, ine, codedAttribute = ClosestFormToCursor << 4;
	unsigned	inf;
	long		dx, dy;
	double		stitchSize = 1e99;

	savdo();
	if (chkMap(FORMSEL)) {

		ine = find1st();
		iStitch = ine + 1;
		while (iStitch<(unsigned)PCSHeader.stitchCount - 1 && stitchSize>SmallStitchLength) {

			if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == codedAttribute) {

				if (StitchBuffer[iStitch].attribute&KNOTMSK)
					ine = iStitch;
				else {

					dx = StitchBuffer[iStitch].x - StitchBuffer[ine].x;
					dy = StitchBuffer[iStitch].y - StitchBuffer[ine].y;
					stitchSize = hypot(dx, dy);
				}
				iStitch++;
			}
		}
		if (iStitch != endStitch - 2) {

			iStitch--;
			ine = iStitch + 2;
			while (ine < (unsigned)PCSHeader.stitchCount - 1) {

				do {

					if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == codedAttribute && !(StitchBuffer[iStitch].attribute&KNOTMSK)) {

						dx = StitchBuffer[ine].x - StitchBuffer[iStitch].x;
						dy = StitchBuffer[ine++].y - StitchBuffer[iStitch].y;
						stitchSize = hypot(dx, dy);
					} else
						ine++;
				} while (stitchSize < SmallStitchLength&&ine < PCSHeader.stitchCount);
				StitchBuffer[++iStitch].attribute = StitchBuffer[--ine].attribute;
				StitchBuffer[iStitch].x = StitchBuffer[ine].x;
				StitchBuffer[iStitch].y = StitchBuffer[ine++].y;
			}
			iStitch++;
			while (ine < PCSHeader.stitchCount) {

				StitchBuffer[iStitch].attribute = StitchBuffer[ine].attribute;
				StitchBuffer[iStitch].x = StitchBuffer[ine].x;
				StitchBuffer[iStitch++].y = StitchBuffer[ine++].y;
			}
			PCSHeader.stitchCount = iStitch;
			coltab();
		}
	} else {

		iStitch = startStitch;
		ine = startStitch + 1;
		SelectedPoint.x = StitchBuffer[iStitch].x;
		SelectedPoint.y = StitchBuffer[iStitch].y;
		for (inf = ine; inf < endStitch; inf++) {

			if (StitchBuffer[ine].attribute&KNOTMSK) {

				SelectedPoint.x = StitchBuffer[ine].x;
				SelectedPoint.y = StitchBuffer[ine].y;
				mvstch(ine++, inf);
			} else {

				dx = StitchBuffer[inf].x - SelectedPoint.x;
				dy = StitchBuffer[inf].y - SelectedPoint.y;
				stitchSize = hypot(dx, dy);
				if (stitchSize > SmallStitchLength) {

					mvstch(ine++, inf);
					SelectedPoint.x = StitchBuffer[inf].x;
					SelectedPoint.y = StitchBuffer[inf].y;
				}
			}
		}
		while (inf < PCSHeader.stitchCount)
			mvstch(ine++, inf++);
		PCSHeader.stitchCount = ine;
		coltab();
	}
	rstAll();
	ritot(PCSHeader.stitchCount);
	lenCalc();
	setMap(RESTCH);
}

BOOL cmpstch(unsigned iStitchA, unsigned iStitchB) {

#if  __UseASM__
	_asm {
		mov		eax, iStitchA
		xor		ecx, ecx
		mov		cl, 12
		mul		ecx
		mov		ebx, eax
		add		ebx, offset StitchBuffer
		mov		eax, iStitchB
		mul		ecx
		add		eax, offset StitchBuffer
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
	if (StitchBuffer[iStitchA].x != StitchBuffer[iStitchB].x)
		return 0;

	return StitchBuffer[iStitchA].y == StitchBuffer[iStitchB].y;
#endif
}

void mvstch(unsigned destination, unsigned source) {

#if  __UseASM__
	_asm {
		mov		eax, destination
		xor		ecx, ecx
		mov		cl, 12
		mul		ecx
		mov		ebx, eax
		add		ebx, offset StitchBuffer
		mov		eax, source
		mul		ecx
		add		eax, offset StitchBuffer
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
	StitchBuffer[destination] = StitchBuffer[source];
#endif
}

void ofstch(unsigned iSource, TCHAR offset) {

	StitchBuffer[OutputIndex].x = StitchBuffer[iSource].x + KnotStep.x*offset;
	StitchBuffer[OutputIndex].y = StitchBuffer[iSource].y + KnotStep.y*offset;
	StitchBuffer[OutputIndex++].attribute = KnotAttribute;
}

void endknt(unsigned finish) {

	double		length;
	dPOINT		delta;
	unsigned	iStart, iKnot;
	TCHAR*		knots;

	iStart = finish - 1;
	KnotAttribute = StitchBuffer[iStart].attribute | KNOTMSK;
	do {

		delta.x = StitchBuffer[finish].x - StitchBuffer[iStart].x;
		delta.y = StitchBuffer[finish].y - StitchBuffer[iStart].y;
		length = hypot(delta.x, delta.y);
		iStart--;
	} while (!length);
	if (chkMap(FILDIR))
		knots = KnotAtLastOrder;
	else
		knots = KnotAtEndOrder;
	if (!(iStart & 0x8000000)) {

		KnotStep.x = 2 / length*delta.x;
		KnotStep.y = 2 / length*delta.y;
		for (iKnot = 0; iKnot < 5; iKnot++)
			ofstch(finish, knots[iKnot]);
		if (chkMap(FILDIR))
			ofstch(finish, 0);
	}
}

void strtknt(unsigned start) {

	double		length;
	dPOINT		delta;
	unsigned	finish, iKnot;

	finish = start + 1;
	do {

		delta.x = StitchBuffer[finish].x - StitchBuffer[start].x;
		delta.y = StitchBuffer[finish].y - StitchBuffer[start].y;
		length = hypot(delta.x, delta.y);
		finish++;
	} while (length < 2 && finish < PCSHeader.stitchCount);
	if (finish < PCSHeader.stitchCount) {

		KnotAttribute = StitchBuffer[start].attribute | KNOTMSK;
		KnotStep.x = 2 / length*delta.x;
		KnotStep.y = 2 / length*delta.y;
		for (iKnot = 0; iKnot < 5; iKnot++)
			ofstch(start, KnotAtStartOrder[iKnot]);
	}
}

void fndknt() {

	unsigned	iStitch;
	BOOL		flag;

	if (PCSHeader.stitchCount > 4) {

		flag = 0;
		KnotCount = 0;
		for (iStitch = 0; iStitch < (unsigned)PCSHeader.stitchCount - 4; iStitch++) {

			if (StitchBuffer[iStitch].attribute&KNOTMSK) {

				if (!flag)
					Knots[KnotCount++] = iStitch;
			} else
				flag = 0;
		}
	} else KnotCount = 0;
}

void delknt() {

	unsigned	iStitch, newStitchCount;

	newStitchCount = 0;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (!(StitchBuffer[iStitch].attribute&KNOTMSK))
			mvstch(newStitchCount++, iStitch);
	}
	PCSHeader.stitchCount = newStitchCount;
}

BOOL isknots() {

	unsigned iStitch;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (StitchBuffer[iStitch].attribute&KNOTMSK)
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

unsigned kjmp(unsigned start) {

	while (start<(unsigned)PCSHeader.stitchCount - 1 && stlen(start)>KNOTLEN)
		mvstch(OutputIndex++, start++);
	strtknt(start);
	return start;
}

void setknt() {

	unsigned	iStitch;

	OutputIndex = MAXSEQ;
	mvstch(OutputIndex++, 0);
	strtknt(0);
	if (stlen(0) > KNOTLEN)
		iStitch = kjmp(1);
	else
		iStitch = 1;
	rstMap(FILDIR);
	while (iStitch < (unsigned)PCSHeader.stitchCount - 1) {

		mvstch(OutputIndex++, iStitch);
		if (stlen(iStitch) > KNOTLEN) {

			endknt(iStitch);
			iStitch = kjmp(iStitch + 1);
			mvstch(OutputIndex++, iStitch);
		}
		iStitch++;
	}
	setMap(FILDIR);
	endknt(iStitch);
	StitchBuffer[OutputIndex - 1].attribute &= (~KNOTMSK);
	PCSHeader.stitchCount = OutputIndex - MAXSEQ;
	mvstchs(0, MAXSEQ, PCSHeader.stitchCount);
}

unsigned srchknot(unsigned source) {

	while (Knots[ActivePointIndex] < source&&ActivePointIndex < KnotCount)
		ActivePointIndex++;
	ActivePointIndex--;
	if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5) {

		ActivePointIndex++;
		if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5)
			return 0;
		else
			return 2;
	} else {

		ActivePointIndex++;
		if (((Knots[ActivePointIndex] > source) ? (Knots[ActivePointIndex] - source) : (source - Knots[ActivePointIndex])) < 5)
			return 1;
		else
			return 3;
	}
}

void chkncol() {

	unsigned	iStitch, initialColor, color, code;

	OutputIndex = MAXSEQ;
	initialColor = StitchBuffer[0].attribute&COLMSK;
	rstMap(FILDIR);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		color = StitchBuffer[iStitch].attribute&COLMSK;
		if (color == initialColor)
			mvstch(OutputIndex++, iStitch);
		else {

			initialColor = StitchBuffer[iStitch].attribute&COLMSK;
			code = srchknot(iStitch);
			if (code & 1) {

				endknt(iStitch - 1);
			}
			mvstch(OutputIndex++, iStitch);
			if (code & 2)
				strtknt(iStitch);
		}
	}
	PCSHeader.stitchCount = OutputIndex - MAXSEQ;
	mvstchs(0, MAXSEQ, PCSHeader.stitchCount);
}

void setknots()
{
	if (PCSHeader.stitchCount)
	{
		savdo();
		delknt();
		setknt();
		fndknt();
		chkncol();
		coltab();
		setMap(RESTCH);
		ritot(PCSHeader.stitchCount);
	}
}

void lodbmp() {

	TCHAR*	filename;

	if (PCSBMPFileName[0]) {

		DeleteObject(BitmapFileHandle);
		ReleaseDC(ThrEdWindow, BitmapDC);
	}
	if (GetOpenFileName(&OpenBitmapName)) {

		untrace();
		filename = strrchr(UserBMPFileName, '\\') + 1;
		// PCS file can only store a 16 character filename?
		strncpy_s(PCSBMPFileName, filename, sizeof(PCSBMPFileName));
		defbNam();
		bfil();
		setMap(RESTCH);
	}
}

void duhbit(unsigned cod)
{
	CheckMenuItem(MainMenu, ID_HIDBIT, MF_BYCOMMAND | cod);
	CheckMenuItem(MainMenu, ID_HIDBITF, MF_BYCOMMAND | cod);
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
	KillTimer(ThrEdWindow, 0);
	setMap(WASPAT);
	movStch();
	setMap(RESTCH);
	tabmsg(IDS_END);
}

void drwlstch(unsigned finish) {

	unsigned	color, iMovieFrame, origin;
	unsigned	flag;

	origin = RunPoint;
	if (chkMap(HID)) {

		while ((StitchBuffer[RunPoint].attribute&COLMSK) != ActiveColor&&RunPoint < finish - 1)
			RunPoint++;
	}
	if (chkMap(ZUMED)) {

		iMovieFrame = 1;
		while (RunPoint < StitchesPerFrame + 1 && RunPoint < finish - 2 && !stch2px(RunPoint))
			RunPoint++;
		origin = RunPoint - 1;
		color = StitchBuffer[RunPoint].attribute&COLMSK;
		flag = 1;
		while (iMovieFrame < StitchesPerFrame + 1 && RunPoint < finish - 2 && (StitchBuffer[RunPoint].attribute&COLMSK) == color) {

			if (stch2px(RunPoint)) {

				MovieLine[iMovieFrame].x = StitchCoordinatesPixels.x;
				MovieLine[iMovieFrame++].y = StitchCoordinatesPixels.y;
				if (flag) {

					flag = 0;
					if (stch2px(RunPoint - 1)) {

						MovieLine[0].x = MovieLine[1].x;
						MovieLine[0].y = MovieLine[1].y;
					} else {

						MovieLine[0].x = StitchCoordinatesPixels.x;
						MovieLine[0].y = StitchCoordinatesPixels.y;
					}
				}
			}
			RunPoint++;
		}
		if (RunPoint == origin)
			RunPoint++;
		if (!stch2px(RunPoint)) {

			if ((StitchBuffer[RunPoint].attribute&COLMSK) == color) {

				MovieLine[iMovieFrame].x = StitchCoordinatesPixels.x;
				MovieLine[iMovieFrame++].y = StitchCoordinatesPixels.y;
				RunPoint++;
			}
		}
		SelectObject(StitchWindowDC, UserPen[color]);
		Polyline(StitchWindowDC, MovieLine, iMovieFrame);
		if (!flag)
			RunPoint--;
	} else {

		iMovieFrame = 0;
		color = StitchBuffer[RunPoint].attribute&COLMSK;
		SelectObject(StitchWindowDC, UserPen[color]);
		while (iMovieFrame < StitchesPerFrame && (RunPoint + 1 < finish - 1) && ((StitchBuffer[RunPoint].attribute&COLMSK) == color)) {

			stch2px1(RunPoint++);
			MovieLine[iMovieFrame].x = StitchCoordinatesPixels.x;
			MovieLine[iMovieFrame++].y = StitchCoordinatesPixels.y;
		}
		RunPoint--;
		Polyline(StitchWindowDC, MovieLine, iMovieFrame);
	}
	if ((StitchBuffer[RunPoint + 1].attribute & 0xf) != color)
		RunPoint++;
	ritnum(STR_NUMSEL, RunPoint);
	if (RunPoint + 3 > finish - 1)
		patdun();
}

void stchout() {

	if (chkMap(GRPSEL))
		drwlstch(GroupEndStitch);
	else
		drwlstch(PCSHeader.stitchCount - 1);
}

unsigned duth(TCHAR* name) {

	unsigned iLast = strlen(name);

	do iLast--;
	while (tolower(name[iLast]) != 'h'&&iLast);
	if (name[iLast - 1] == 't')
		return iLast + 1;
	else
		return 0;
}

void duver(TCHAR* name) {

	unsigned	lastChar;
	int			version;

	lastChar = duth(name);
	if (lastChar) {

		version = tolower(name[lastChar]) - 'r';
		name[_MAX_PATH - 1] = 0;
		if (version >= 0 && version <= 3)
			strcpy_s(VersionNames[version], name);
	}
}

void durit(void* source, unsigned cnt) {

#if  __UseASM__
	_asm {

		mov		esi, source
		mov		edi, OutputIndex
		mov		ecx, cnt
		rep		movsb
		mov		OutputIndex, edi
	}
#else
	memcpy((void *)OutputIndex, source, cnt);
	OutputIndex += cnt;
#endif
}

unsigned bufref() {

#if  __UseASM__
	_asm {

		mov		eax, OutputIndex
		sub		eax, offset BSequence
	}
#else
	return (unsigned)OutputIndex - (unsigned)BSequence;
#endif
}

void dubuf() {

	STRHED				stitchHeader;
	unsigned			iForm, iColor, iVertex, iGuide, iClip;
	unsigned			vertexCount, guideCount, clipDataCount;
	FRMHED*				forms;
	fPOINT*				vertices;
	SATCON*				guides;
	fPOINT*				points;
	unsigned			iDestinationVertex = 0;
	unsigned			iDestinationGuide = 0;
	unsigned			iDestinationClip = 0;

	OutputIndex = (unsigned)&BSequence;
	stitchHeader.headerType = 0x2746872;
	stitchHeader.fileLength = PCSHeader.stitchCount * sizeof(fPOINTATTR) + sizeof(STRHED) + 16;
	stitchHeader.stitchCount = PCSHeader.stitchCount;
	stitchHeader.hoopType = IniFile.hoopType;
	vertexCount = 0; guideCount = 0; clipDataCount = 0;
	strcpy_s(ExtendedHeader.modifierName, IniFile.designerName);
	if (FormIndex) {

		for (iForm = 0; iForm < FormIndex; iForm++) {

			vertexCount += FormList[iForm].sides;
			if (FormList[iForm].type == SAT)
				guideCount += FormList[iForm].satinGuideCount;
			if (isclp(iForm))
				clipDataCount += FormList[iForm].lengthOrCount.clipCount;
			if (iseclp(iForm))
				clipDataCount += FormList[iForm].clipEntries;
		}
	}
	stitchHeader.formCount = FormIndex;
	stitchHeader.vertexCount = vertexCount;
	stitchHeader.dlineCount = guideCount;
	stitchHeader.clipDataCount = clipDataCount;
	stitchHeader.vertexLen = sizeof(STRHED) + PCSHeader.stitchCount * sizeof(fPOINTATTR) + 164;
	stitchHeader.dlineLen = sizeof(fPOINT)*vertexCount;
	stitchHeader.clipDataLen = sizeof(fPOINT)*clipDataCount;
	durit(&stitchHeader, sizeof(STRHED));
	ExtendedHeader.auxFormat = IniFile.auxFileType;
	ExtendedHeader.hoopSizeX = IniFile.hoopSizeX;
	ExtendedHeader.hoopSizeY = IniFile.hoopSizeY;
	ExtendedHeader.texturePointCount = TextureIndex;
	durit(&ExtendedHeader, sizeof(STREX));
	durit(StitchBuffer, PCSHeader.stitchCount * sizeof(fPOINTATTR));
	if (!PCSBMPFileName[0]) {

		for (iColor = 0; iColor < 16; iColor++)
			PCSBMPFileName[iColor] = 0;
	}
	durit(PCSBMPFileName, 16);
	durit(&BackgroundColor, 4);
	durit(UserColor, 64);
	durit(CustomColor, 64);
	for (iColor = 0; iColor < 16; iColor++)
		MsgBuffer[iColor] = ThreadSize[iColor][0];
	durit(MsgBuffer, 16);
	if (FormIndex) {

		forms = new FRMHED[FormIndex];
		vertices = new fPOINT[vertexCount + 1];
		guides = new SATCON[guideCount];
		points = new fPOINT[clipDataCount + 1];
		for (iForm = 0; iForm < FormIndex; iForm++) {

			frmcpy(&forms[iForm], &FormList[iForm]);
			forms[iForm].vertices = (fPOINT*)(&vertices[iDestinationVertex] - &vertices[0]);
			for (iVertex = 0; iVertex < FormList[iForm].sides; iVertex++) {

				vertices[iDestinationVertex].x = FormList[iForm].vertices[iVertex].x;
				vertices[iDestinationVertex++].y = FormList[iForm].vertices[iVertex].y;
			}
			if (FormList[iForm].type == SAT) {

				forms[iForm].satinOrAngle.sac = (SATCON*)(&guides[iDestinationGuide] - &guides[0]);
				forms[iForm].satinGuideCount = FormList[iForm].satinGuideCount;
				for (iGuide = 0; iGuide < FormList[iForm].satinGuideCount; iGuide++) {

					guides[iDestinationGuide].start = FormList[iForm].satinOrAngle.sac[iGuide].start;
					guides[iDestinationGuide++].finish = FormList[iForm].satinOrAngle.sac[iGuide].finish;
				}
			}
			if (isclp(iForm)) {

				forms[iForm].angleOrClipData.clip = (fPOINT*)(&points[iDestinationClip] - &points[0]);
				for (iClip = 0; (iClip < FormList[iForm].lengthOrCount.clipCount) && (iDestinationClip < clipDataCount); iClip++) {

					points[iDestinationClip].x = FormList[iForm].angleOrClipData.clip[iClip].x;
					points[iDestinationClip++].y = FormList[iForm].angleOrClipData.clip[iClip].y;
				}
			}
			if (iseclpx(iForm)) {

				forms[iForm].borderClipData = (fPOINT*)(&points[iDestinationClip] - &points[0]);
				for (iClip = 0; (iClip < FormList[iForm].clipEntries) && ((iDestinationClip < clipDataCount)); iClip++) {
					points[iDestinationClip].x = FormList[iForm].borderClipData[iClip].x;
					points[iDestinationClip++].y = FormList[iForm].borderClipData[iClip].y;
				}
			}
		}
		durit(forms, FormIndex * sizeof(FRMHED));
		durit(vertices, vertexCount * sizeof(fPOINT));
		durit(guides, guideCount * sizeof(SATCON));
		durit(points, clipDataCount * sizeof(fPOINT));
		durit(TexturePointsBuffer, TextureIndex * sizeof(TXPNT));
		delete[] forms;
		delete[] vertices;
		delete[] guides;
		delete[] points;
	}
}

void thrsav() {

	unsigned		iVersion, lastCharacter;
	int				iBackup;
	unsigned long	bytesWritten;
	WIN32_FIND_DATA	fileData;
	HANDLE			file;
	TCHAR			newFileName[_MAX_PATH];

	if (chkattr(WorkingFileName))
		return;
	if (!rstMap(IGNAM)) {

		file = FindFirstFile(GeName, &fileData);
		iVersion = 0;
		if (file != INVALID_HANDLE_VALUE) {

			rstMap(CMPDO);
			for (iVersion = 0; iVersion < OLDVER; iVersion++)
				VersionNames[iVersion][0] = 0;
			duver(fileData.cFileName);
			while (FindNextFile(file, &fileData))
				duver(fileData.cFileName);
			FindClose(file);
			DeleteFile(VersionNames[OLDVER - 1]);
			// ToDo - Does this loop have to decrement or could it increment?
			for (iBackup = OLDVER - 2; iBackup >= 0; iBackup--) {

				if (VersionNames[iBackup][0]) {

					VersionNames[iBackup][_MAX_PATH - 1] = 0;
					strcpy_s(newFileName, VersionNames[iBackup]);
					lastCharacter = duth(newFileName);
					newFileName[lastCharacter] = iBackup + 's';
					MoveFile(VersionNames[iBackup], newFileName);
				}
			}
		}
	}
	FileHandle = CreateFile(ThrName, (GENERIC_WRITE), 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE) {

		crmsg(ThrName);
		FileHandle = 0;
	} else {

		dubuf();
		WriteFile(FileHandle, BSequence, bufref(), &bytesWritten, 0);
		if (bytesWritten != (unsigned long)bufref()) {

			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "File Write Error: %s\n", ThrName);
			shoMsg(MsgBuffer);
		}
		CloseHandle(FileHandle);
	}
}

void setsped() {

	unsigned	elapsedTimePerFrame;
	double		userTimePerFrame;

	if (!MovieTimeStep)
		MovieTimeStep = 1;
	userTimePerFrame = (double)MovieTimeStep / 10;
	if (userTimePerFrame < 10) {

		elapsedTimePerFrame = 100;  // units are millseconds
		StitchesPerFrame = (unsigned)elapsedTimePerFrame / userTimePerFrame;
		if (StitchesPerFrame > 99)
			StitchesPerFrame = 99;
	} else {

		elapsedTimePerFrame = (unsigned)userTimePerFrame;
		StitchesPerFrame = 2;
	}
	if (StitchesPerFrame < 2)
		StitchesPerFrame = 2;
	SetTimer(ThrEdWindow, 0, elapsedTimePerFrame, 0);
}

void f1del() {

	unsigned cod, iStitch, stitchCount;

	if (chkMap(DELTO)) {

		cod = ClosestFormToCursor << FRMSHFT;
		stitchCount = 0;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		{
			if ((StitchBuffer[iStitch].attribute&(FRMSK | NOTFRM)) != cod)
			{
				// ToDo - could this be simplified to StitchBuffer[stitchCount] = StitchBuffer[iStitch]
				StitchBuffer[stitchCount].x = StitchBuffer[iStitch].x;
				StitchBuffer[stitchCount].y = StitchBuffer[iStitch].y;
				StitchBuffer[stitchCount].attribute = StitchBuffer[iStitch].attribute;
				stitchCount++;
			}
		}
		PCSHeader.stitchCount = stitchCount;
	}
	deleclp(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
	delsac(ClosestFormToCursor);
	delflt(ClosestFormToCursor);
	deltx();
}

void frmdel() {

	unsigned iForm, iStitch;
	unsigned stitchForm, stitchAttributeFormBits;

	fvars(ClosestFormToCursor);
	f1del();
	for (iForm = ClosestFormToCursor; iForm < (unsigned)FormIndex; iForm++)
		frmcpy(&FormList[iForm], &FormList[iForm + 1]);
	if (rstMap(DELTO)) {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (!(StitchBuffer[iStitch].attribute&NOTFRM)) {

				stitchAttributeFormBits = (StitchBuffer[iStitch].attribute&FRMSK);
				stitchForm = stitchAttributeFormBits >> 4;
				if (stitchForm > ClosestFormToCursor) {

					StitchBuffer[iStitch].attribute &= NFRMSK;
					StitchBuffer[iStitch].attribute |= (stitchAttributeFormBits - 0x10);
				}
			}
		}
	} else {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (!(StitchBuffer[iStitch].attribute&NOTFRM)) {

				stitchAttributeFormBits = (StitchBuffer[iStitch].attribute&FRMSK);
				stitchForm = stitchAttributeFormBits >> 4;
				if (stitchForm == ClosestFormToCursor)
					StitchBuffer[iStitch].attribute &= (NFRMSK&NTYPMSK);
				if (stitchForm > ClosestFormToCursor) {

					StitchBuffer[iStitch].attribute &= NFRMSK;
					StitchBuffer[iStitch].attribute |= (stitchAttributeFormBits - 0x10);
				}
			}
		}
	}
	FormIndex--;
	rstMap(FORMSEL);
}

void deltot() {

	strcpy_s(DesignerName, IniFile.designerName);
	FormIndex = PCSHeader.stitchCount = FormVertexIndex = ClipPointIndex = SatinConnectIndex = TextureIndex = 0;
	rstMap(GMRK);
	rstAll();
	coltab();
	zumhom();
	strcpy_s(DesignerName, IniFile.designerName);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRDBY], ThrName, DesignerName);
	SetWindowText(ThrEdWindow, MsgBuffer);
}

BOOL wastch() {

	unsigned iStitch;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if ((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor)
			return 1;
	}
	return 0;
}

BOOL frmstch()
{
	unsigned iForm, iStitch, formCode;

	clRmap((FormIndex >> 5) >> 1);
	for (iForm = 0; iForm < FormIndex; iForm++)
		setr(SelectedFormList[iForm]);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
	{
		formCode = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
		if (chkr(formCode))
			return 1;
	}
	return 0;
}

void delet() {

	unsigned iVertex, iForm, currentFormVertex;
	BOOL satinFlag=0;

	undat();
	if (rstMap(FPSEL)) {

		savdo();
		fvars(ClosestFormToCursor);
		clRmap((SideCount >> 5) + 1);
		currentFormVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {

			setr(currentFormVertex);
			currentFormVertex = pdir(currentFormVertex);
		}
		currentFormVertex = 0;
		for (iVertex = 0; iVertex < SideCount; iVertex++) {

			if (!chkr(iVertex)) {

				CurrentFormVertices[currentFormVertex].x = CurrentFormVertices[iVertex].x;
				CurrentFormVertices[currentFormVertex].y = CurrentFormVertices[iVertex].y;
				currentFormVertex++;
			}
		}
		currentFormVertex = fltind(&CurrentFormVertices[currentFormVertex]);
		iVertex = fltind(&CurrentFormVertices[iVertex]);
		while (iVertex < FormVertexIndex) {

			FormVertices[currentFormVertex].x = FormVertices[iVertex].x;
			FormVertices[currentFormVertex].y = FormVertices[iVertex].y;
			currentFormVertex++;
			iVertex++;
		}
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
			FormList[iForm].vertices -= (SelectedFormVertices.vertexCount + 1);
		FormVertexIndex -= (SelectedFormVertices.vertexCount + 1);
		SelectedForm->sides -= (SelectedFormVertices.vertexCount + 1);
		frmout(ClosestFormToCursor);
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
		if (SelectedFormCount) {

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
		if (chkMap(FORMSEL) && FormIndex) {

			if (wastch()) {

				setMap(DELFRM);
				tabmsg(IDS_FDEL);
				okcan();
				tomsg();
			} else {

				if (FormIndex) {

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
			if (PCSHeader.stitchCount > 2) {

				delstch1(ClosestPointIndex);
				if (!stch2px(ClosestPointIndex))
					movbox();
			} else {

				PCSHeader.stitchCount = 0;
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

			SelectedForm = &FormList[ClosestFormToCursor];
			switch (SelectedForm->type) {

			case FRMLINE:

				if (SelectedForm->fillType == CONTF) {

					if (ClosestVertexToCursor == SelectedForm->angleOrClipData.sat.start || ClosestVertexToCursor == SelectedForm->angleOrClipData.sat.finish) {

						delmfil(SelectedForm->fillColor);
						SelectedForm->fillType = 0;
						coltab();
						setMap(RESTCH);
						return;
					}
					if (SelectedForm->angleOrClipData.sat.start > ClosestVertexToCursor)
						SelectedForm->angleOrClipData.sat.start--;
					if (SelectedForm->angleOrClipData.sat.finish > ClosestVertexToCursor)
						SelectedForm->angleOrClipData.sat.finish--;
				}
				break;

			case SAT:

				if (ClosestVertexToCursor <= 1) {

					if (SelectedForm->attribute&FRMEND) {

						if (SelectedForm->wordParam)
							SelectedForm->wordParam = 0;
						else
							SelectedForm->attribute &= 0xfe;
						satinFlag = 1;
						goto deldun;
					}
				}
				if (WordParam) {

					if (ClosestVertexToCursor == (unsigned)WordParam || ClosestVertexToCursor == (unsigned)WordParam + 1) {

						SelectedForm->wordParam = 0;
						satinFlag = 1;
						goto deldun;
					}
				}
				for (iForm = 0; iForm < SelectedForm->satinGuideCount; iForm++) {

					if (SelectedForm->satinOrAngle.sac[iForm].start == ClosestVertexToCursor || SelectedForm->satinOrAngle.sac[iForm].finish == ClosestVertexToCursor) {

						delcon(iForm);
						satinFlag = 1;
						goto deldun;
					}
				}
			}
			delspnt();
		deldun:;
			if (ClosestFormToCursor > FormIndex - 1)
				ClosestFormToCursor = FormIndex - 1;
			if (FormIndex) {

				frmout(ClosestFormToCursor);
				fvars(ClosestFormToCursor);
				refil();
			}
			coltab();
			setMap(RESTCH);
		}
		if (!satinFlag&&closPnt1(&ClosestPointIndex))
			goto selab;
	}
	fndknt();
}

void movi() {

	double stepCount;

	rstAll();
	if (PCSHeader.stitchCount) {

		if (MsgWindow) {

			DestroyWindow(MsgWindow);
			MsgWindow = 0;
		}
		setMap(RUNPAT);
		if (chkMap(GRPSEL)) {

			rngadj();
			RunPoint = GroupStartStitch;
		} else
			RunPoint = 0;
		movStch();
		if (!chkMap(WASPAT)) {

			SpeedScrollBar = CreateWindow(
				"SCROLLBAR",
				0,
				SBS_HORZ | WS_CHILD | WS_VISIBLE,
				ButtonWidthX3,
				0,
				StitchWindowSize.x,
				SCROLSIZ,
				ThrEdWindow,
				NULL,
				ThrEdInstance,
				NULL);
		}
		if (chkMap(ZUMED))
			stepCount = PCSHeader.stitchCount*ZoomFactor*ZoomFactor;
		else
			stepCount = PCSHeader.stitchCount;
		if (!chkMap(WASPAT))
			MovieTimeStep = MOVITIM * 10000 / stepCount;
		if (MovieTimeStep < MINDELAY)
			MovieTimeStep = MINDELAY;
		if (MovieTimeStep > MAXDELAY)
			MovieTimeStep = MAXDELAY;
		ScrollInfo.cbSize = sizeof(ScrollInfo);
		ScrollInfo.fMask = SIF_ALL;
		ScrollInfo.nMax = MAXDELAY;
		ScrollInfo.nMin = MINDELAY;
		ScrollInfo.nPage = 1;
		ScrollInfo.nPos = MAXDELAY - MovieTimeStep;
		SetScrollInfo(SpeedScrollBar, SB_CTL, &ScrollInfo, TRUE);
		FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
		setsped();
	}
}

#define CLPBUG 0

void redclp() {

	int		iStitch, codedLayer;

	codedLayer = ActiveLayer << LAYSHFT;
	ClipPointer = GlobalLock(ClipMemory);
	ClipStitchData = (CLPSTCH*)ClipPointer;
	if (ClipPointer) {
		ClipStitchCount = ClipStitchData[0].led;
		ClipBuffer[0].x = ClipStitchData[0].x + (float)ClipStitchData[0].fx / 256;
		ClipBuffer[0].y = ClipStitchData[0].y + (float)ClipStitchData[0].fy / 256;
		ClipBuffer[0].attribute = 0;

#if CLPBUG

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "interator: 0 x: %6.2f,y: %6.2f\n", ClipBuffer[0].x, ClipBuffer[0].y);
		OutputDebugString(MsgBuffer);
#endif
		ClipRect.left = ClipRect.right = ClipBuffer[0].x;
		ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
		for (iStitch = 1; iStitch < (long)ClipStitchCount; iStitch++) {

			ClipBuffer[iStitch].x = ClipStitchData[iStitch].x + (float)ClipStitchData[iStitch].fx / 256;
			ClipBuffer[iStitch].y = ClipStitchData[iStitch].y + (float)ClipStitchData[iStitch].fy / 256;
			ClipBuffer[iStitch].attribute = (ClipStitchData[iStitch].led & 0xf) | codedLayer;

#if CLPBUG

			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "iterator: %d x: %6.2f,y: %6.2f\n", iStitch, ClipBuffer[iStitch].x, ClipBuffer[iStitch].y);
			OutputDebugString(MsgBuffer);
#endif
			if (ClipBuffer[iStitch].x < ClipRect.left)
				ClipRect.left = ClipBuffer[iStitch].x;
			if (ClipBuffer[iStitch].x > ClipRect.right)
				ClipRect.right = ClipBuffer[iStitch].x;
			if (ClipBuffer[iStitch].y < ClipRect.bottom)
				ClipRect.bottom = ClipBuffer[iStitch].y;
			if (ClipBuffer[iStitch].y > ClipRect.top)
				ClipRect.top = ClipBuffer[iStitch].y;
		}
		ClipBuffer[0].attribute = ActiveColor | codedLayer;
		ClipRectSize.cx = ClipRect.right - ClipRect.left;
		ClipRectSize.cy = ClipRect.top - ClipRect.bottom;
		GlobalUnlock(ClipMemory);
		if (ClipRect.left || ClipRect.bottom) {

			for (iStitch = 0; iStitch < (int)ClipStitchCount; iStitch++) {

				ClipBuffer[iStitch].x -= ClipRect.left;
				ClipBuffer[iStitch].y -= ClipRect.bottom;
			}
			ClipRect.top -= ClipRect.bottom;
			ClipRect.right -= ClipRect.left;
			ClipRect.bottom = ClipRect.left = 0;
		}
	}
}

unsigned nxtcrnr(unsigned corner) {

#if  __UseASM__
	_asm {

		mov		eax, corner
		inc		eax
		and		al, 3
	}
#else
	return (corner + 1) & 3;
#endif
}

void drwmrk(HDC dc) {

	POINT	markCoordinates;
	POINT	markLine[2];
	long	markOffset = 6;

	sCor2px(ZoomMarkPoint, &markCoordinates);
	SelectObject(dc, ZoomMarkPen);
	SetROP2(dc, R2_XORPEN);
	markLine[0].x = markCoordinates.x - markOffset;
	markLine[0].y = markCoordinates.y - markOffset;
	markLine[1].x = markCoordinates.x + markOffset;
	markLine[1].y = markCoordinates.y + markOffset;
	Polyline(dc, markLine, 2);
	markLine[0].x = markCoordinates.x - markOffset;
	markLine[0].y = markCoordinates.y + markOffset;
	markLine[1].x = markCoordinates.x + markOffset;
	markLine[1].y = markCoordinates.y - markOffset;
	Polyline(dc, markLine, 2);
	SetROP2(dc, R2_COPYPEN);
}

void vubak() {

	unsigned	iVersion;
	long		dx, dy, verticalLocation;

	if (FileHandle || chkMap(THUMSHO)) {

		setMap(ZUMED);
		movStch();
		FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
		dx = (StitchWindowClientRect.right >> 1);
		dy = (StitchWindowClientRect.bottom >> 1);
		for (iVersion = 0; iVersion < OLDVER; iVersion++) {

			if (iVersion & 2)
				verticalLocation = dy;
			else
				verticalLocation = 0;
			BackupViewer[iVersion] = CreateWindow(
				"STATIC",
				"",
				SS_NOTIFY | SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
				dx*(iVersion & 1) + ButtonWidthX3,
				verticalLocation,
				dx,
				dy,
				ThrEdWindow,
				NULL,
				ThrEdInstance,
				NULL);
		}
		setMap(BAKSHO);
	}
}

void getbak() {

	TCHAR*		pchr;

	if (chkMap(THUMSHO)) {

		if (ThumbnailsSelected[FileVersionIndex]) {

			if (chkMap(RBUT)) {

				strcpy_s(InsertedFileName, ThumbnailsSelected[FileVersionIndex]);
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

				strcpy_s(WorkingFileName, DefaultDirectory);
				pchr = &WorkingFileName[strlen(WorkingFileName) - 1];
				if (pchr[0] != '\\') {

					pchr[1] = '\\';
					pchr[2] = 0;
				}
				strcat_s(WorkingFileName, ThumbnailsSelected[FileVersionIndex]);
				setMap(REDOLD);
				nuFil();
			}
		} else
			unthum();
	}
}

void rebak() {

	unsigned	iVersion;
	TCHAR		newFileName[_MAX_PATH];
	TCHAR		safetyFileName[_MAX_PATH];

	for (iVersion = 0; iVersion < OLDVER; iVersion++)
		DestroyWindow(BackupViewer[iVersion]);
	strcpy_s(newFileName, ThrName);
	strcpy_s(safetyFileName, ThrName);
	iVersion = duth(newFileName);
	newFileName[iVersion] = FileVersionIndex + 's';
	safetyFileName[iVersion] = 'x';
	MoveFile(ThrName, safetyFileName);
	MoveFile(newFileName, ThrName);
	MoveFile(safetyFileName, newFileName);
	strcpy_s(WorkingFileName, ThrName);
	setMap(REDOLD);
	nuFil();
	DeleteFile(safetyFileName);
}

void thumbak() {

	unsigned iVersion;

	for (iVersion = 0; iVersion < OLDVER; iVersion++)
		DestroyWindow(BackupViewer[iVersion]);
	getbak();
}

void movbak(TCHAR source, TCHAR destination) {

	TCHAR	sourceFileName[_MAX_PATH];
	TCHAR	destinationFileName[_MAX_PATH];

	unsigned lastChar = duth(ThrName);

	strcpy_s(sourceFileName, ThrName);
	strcpy_s(destinationFileName, ThrName);
	sourceFileName[lastChar] = (TCHAR)source;
	destinationFileName[lastChar] = (TCHAR)destination;
	DeleteFile(destinationFileName);
	MoveFile(sourceFileName, destinationFileName);
}

void purg() {

	TCHAR		fileName[_MAX_PATH];
	unsigned	lastChar, iLast;

	if (FileHandle) {

		strcpy_s(fileName, ThrName);
		lastChar = duth(fileName);
		for (iLast = 1; iLast < 6; iLast++) {

			fileName[lastChar] = (TCHAR)iLast + 'r';
			DeleteFile(fileName);
		}
	}
}

void purgdir() {

	setMap(PRGMSG);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Delete all backups in %s\n", DefaultDirectory);
	shoMsg(MsgBuffer);
	okcan();
}

void deldir() {

	unsigned			iLastChar;
	TCHAR				fileName[_MAX_PATH];
	TCHAR*				fileSpec;
	WIN32_FIND_DATA		findFileData;
	HANDLE				file;

	unmsg();
	tabmsg(IDS_BAKDEL);
	strcpy_s(fileName, DefaultDirectory);
	fileSpec = &fileName[strlen(fileName)];
	strcpy_s(fileSpec, sizeof(fileName) - (fileSpec - fileName), "\\*.th0");
	for (iLastChar = 1; iLastChar < 6; iLastChar++) {

		fileSpec[5] = (TCHAR)iLastChar + 'r';
		file = FindFirstFile(fileName, &findFileData);
		if (file != INVALID_HANDLE_VALUE) {

			DeleteFile(findFileData.cFileName);
			while (FindNextFile(file, &findFileData))
				DeleteFile(findFileData.cFileName);
		}
		FindClose(file);
	}
	unmsg();
	tabmsg(IDS_BAKDT);
}

BOOL chkwnd(HWND window) {

	RECT	windowRect;

	GetWindowRect(window, &windowRect);
	if (Msg.pt.x >= windowRect.left&&Msg.pt.x <= windowRect.right
		&&Msg.pt.y >= windowRect.top&&Msg.pt.y <= windowRect.bottom)
		return 1;
	else
		return 0;
}

BOOL chkok() {

	return chkwnd(OKButton);
}

void mv2f() {

	unsigned	attribute, iLowBuffer, iHighBuffer, iStitch;

	if (rstMap(FORMSEL)) {

		savdo();
		iLowBuffer = 0;
		iHighBuffer = MAXPCS >> 1;
		attribute = ClosestFormToCursor << 4;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == attribute) {

				StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch].y;
			} else {

				StitchBuffer[iHighBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iHighBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iHighBuffer++].y = StitchBuffer[iStitch].y;
			}
		}
		iStitch = MAXPCS >> 1;
		while (iStitch < iHighBuffer) {

			StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
			StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
			StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch++].y;
		}
		coltab();
		setMap(RESTCH);
	} else {

		if (rstMap(GRPSEL)) {

			savdo();
			rngadj();
			iHighBuffer = MAXPCS >> 1;
			iLowBuffer = 0;
			for (iStitch = 0; iStitch < GroupStartStitch; iStitch++) {

				StitchBuffer[iHighBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iHighBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iHighBuffer++].y = StitchBuffer[iStitch].y;
			}
			for (iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {

				StitchBuffer[iHighBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iHighBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iHighBuffer++].y = StitchBuffer[iStitch].y;
			}
			for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {

				StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch].y;
			}
			for (iStitch = MAXPCS >> 1; iStitch < iHighBuffer; iStitch++) {

				StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch].y;
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void mv2b() {

	unsigned	attribute, iLowBuffer, iHighBuffer, iStitch;

	if (rstMap(FORMSEL)) {

		savdo();
		iLowBuffer = 0;
		iHighBuffer = MAXPCS >> 1;
		attribute = ClosestFormToCursor << 4;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == attribute) {

				StitchBuffer[iHighBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iHighBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iHighBuffer++].y = StitchBuffer[iStitch].y;
			} else {

				StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch].y;
			}
		}
		iStitch = MAXPCS >> 1;
		while (iStitch < iHighBuffer) {

			StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
			StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
			StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch++].y;
		}
		coltab();
		setMap(RESTCH);
	} else {

		if (rstMap(GRPSEL)) {

			savdo();
			rngadj();
			iHighBuffer = MAXPCS >> 1;
			for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {

				StitchBuffer[iHighBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iHighBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iHighBuffer++].y = StitchBuffer[iStitch].y;
			}
			iLowBuffer = GroupStartStitch;
			for (iStitch = GroupEndStitch; iStitch < PCSHeader.stitchCount; iStitch++) {

				StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch].y;
			}
			for (iStitch = MAXPCS >> 1; iStitch < iHighBuffer; iStitch++) {

				StitchBuffer[iLowBuffer].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[iLowBuffer].x = StitchBuffer[iStitch].x;
				StitchBuffer[iLowBuffer++].y = StitchBuffer[iStitch].y;
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void infadj(float* xCoordinate, float* yCoordinate) {

	if (!_finite(*xCoordinate)) {

		if (*xCoordinate > UnzoomedRect.x)
			*xCoordinate = UnzoomedRect.x;
		else {

			if (*xCoordinate < 0)
				*xCoordinate = 0;
		}
	}
	if (!_finite(*yCoordinate)) {

		if (*yCoordinate > UnzoomedRect.y)
			*yCoordinate = UnzoomedRect.y;
		else {

			if (*yCoordinate < 0)
				*yCoordinate = 0;
		}
	}
}

void delinf() {

	unsigned iStitch, iVertex;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		infadj(&StitchBuffer[iStitch].x, &StitchBuffer[iStitch].y);
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
		infadj(&FormVertices[iVertex].x, &FormVertices[iVertex].y);
}

void chkrng(fPOINT* range) {

	unsigned	iStitch, stitchCount = 0;

	delinf();
	range->x = UnzoomedRect.x;
	range->y = UnzoomedRect.y;
	if (FormIndex) {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (StitchBuffer[iStitch].attribute&NOTFRM
				|| ((StitchBuffer[iStitch].attribute&TYPMSK) && ((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT < FormIndex))) {
				StitchBuffer[stitchCount].attribute = StitchBuffer[iStitch].attribute;
				StitchBuffer[stitchCount].x = StitchBuffer[iStitch].x;
				StitchBuffer[stitchCount].y = StitchBuffer[iStitch].y;
				if (StitchBuffer[stitchCount].x > range->x)
					StitchBuffer[stitchCount].x = range->x - 1;
				if (StitchBuffer[stitchCount].x < 0)
					StitchBuffer[stitchCount].x = 0;
				if (StitchBuffer[stitchCount].y > range->y)
					StitchBuffer[stitchCount].y = range->y - 1;
				if (StitchBuffer[stitchCount].y < 0)
					StitchBuffer[stitchCount].y = 0;
				stitchCount++;
			}
		}
		PCSHeader.stitchCount = stitchCount;
	} else {

		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (StitchBuffer[iStitch].x > range->x)
				StitchBuffer[iStitch].x = range->x - 1;
			if (StitchBuffer[iStitch].x < 0)
				StitchBuffer[iStitch].x = 0;
			if (StitchBuffer[iStitch].y > range->y)
				StitchBuffer[iStitch].y = range->y - 1;
			if (StitchBuffer[iStitch].y < 0)
				StitchBuffer[iStitch].y = 0;
		}
	}
}

void ritmov() {

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if (ClosestVertexToCursor) {

		if (ClosestVertexToCursor == (unsigned)SelectedForm->sides - 1 && SelectedForm->type == FRMLINE)
			Polyline(StitchWindowDC, RubberBandLine, 2);
		else
			Polyline(StitchWindowDC, RubberBandLine, 3);
	} else {

		RubberBandLine[2].x = FormLines[1].x;
		RubberBandLine[2].y = FormLines[1].y;
		if (SelectedForm->type == FRMLINE)
			Polyline(StitchWindowDC, &RubberBandLine[1], 2);
		else
			Polyline(StitchWindowDC, RubberBandLine, 3);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unmov() {

	if (rstMap(SHOMOV))
		ritmov();
}

void duprct() {

	unsigned iVertex, currentVertex;

	currentVertex = SelectedFormVertices.start;
	SelectedVerticesRect.left = SelectedVerticesRect.right = CurrentFormVertices[currentVertex].x;
	SelectedVerticesRect.top = SelectedVerticesRect.bottom = CurrentFormVertices[currentVertex].y;
	currentVertex = pdir(currentVertex);
	for (iVertex = 0; iVertex < SelectedFormVertices.vertexCount; iVertex++) {

		if (CurrentFormVertices[currentVertex].x < SelectedVerticesRect.left)
			SelectedVerticesRect.left = CurrentFormVertices[currentVertex].x;
		if (CurrentFormVertices[currentVertex].x > SelectedVerticesRect.right)
			SelectedVerticesRect.right = CurrentFormVertices[currentVertex].x;
		if (CurrentFormVertices[currentVertex].y < SelectedVerticesRect.bottom)
			SelectedVerticesRect.bottom = CurrentFormVertices[currentVertex].y;
		if (CurrentFormVertices[currentVertex].y > SelectedVerticesRect.top)
			SelectedVerticesRect.top = CurrentFormVertices[currentVertex].y;
		currentVertex = pdir(currentVertex);
	}
}

void setpsel() {

	unpsel();
	duprct();
	sRct2px(SelectedVerticesRect, &SelectedPixelsRect);
	rct2sel(SelectedPixelsRect, SelectedPointsLine);
	sfCor2px(CurrentFormVertices[SelectedFormVertices.finish], &EndPointCross);
	setMap(SHOPSEL);
	dupsel(StitchWindowDC);
	setMap(FPSEL);
}

void rotfn() {

	unsigned	iVertex, iStitch, iForm, currentVertex;
	double		length;
	double		angle;
	dPOINT		center;

	savdo();
	if (chkMap(FPSEL)) {

		fvars(ClosestFormToCursor);
		currentVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {

			rotflt(&CurrentFormVertices[currentVertex]);
			currentVertex = pdir(currentVertex);
		}
		frmout(ClosestFormToCursor);
		setpsel();
		refil();
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {

		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
			rotflt(&FormVertices[iVertex]);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			rotstch(&StitchBuffer[iStitch]);
		for (iForm = 0; iForm < FormIndex; iForm++)
			frmout(iForm);
		selal();
		return;
	}
	if (rstMap(FRMSROT)) {

		angle = RotationAngle;
		center.x = RotationCenter.x;
		center.y = RotationCenter.y;
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {

			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < SideCount; iVertex++)
				rotflt(&CurrentFormVertices[iVertex]);
			frmout(ClosestFormToCursor);
			refilfn();
			RotationAngle = angle;
			RotationCenter.x = center.x;
			RotationCenter.y = center.y;
		}
		setMap(RESTCH);
	} else {

		if (rstMap(FRMROT)) {

			fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < SideCount; iVertex++)
				rotflt(&CurrentFormVertices[iVertex]);
			frmout(ClosestFormToCursor);
			refil();
			setMap(RESTCH);
		} else {

			length = GroupEndStitch - GroupStartStitch + 1;
			for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
				rotstch(&StitchBuffer[iStitch]);
			rngadj();
			selin(GroupStartStitch, GroupEndStitch, StitchWindowDC);
			setMap(RESTCH);
		}
	}
}

void rotfns() {

	savdo();
	rotfn();
}

void nulayr(unsigned play) {

	ActiveLayer = play;
	LayerIndex = play >> 1;
	ladj();
	if (ActiveLayer) {

		if (chkMap(FORMSEL) && ((unsigned)((FormList[ClosestFormToCursor].attribute&FRMLMSK) >> 1) != ActiveLayer))
			rstMap(FORMSEL);
		rstMap(GRPSEL);
		if (chkMap(SELBOX)) {

			if (ActiveLayer != ((StitchBuffer[ClosestPointIndex].attribute&LAYMSK) >> LAYSHFT) + 1)
				rstMap(SELBOX);
		}
		SelectedFormCount = 0;
	}
	setMap(RESTCH);
}

BOOL iselpnt() {

	unsigned	ind, ine = 0;
	double		len, minlen = 1e99;
	POINT		tpnt;

	tpnt.x = Msg.pt.x - StitchWindowOrigin.x;
	tpnt.y = Msg.pt.y - StitchWindowOrigin.y;
	for (ind = 0; ind < 9; ind++) {

		len = hypot(tpnt.x - FormOutlineRect[ind].x, tpnt.y - FormOutlineRect[ind].y);
		if (len < minlen) {

			minlen = len;
			ine = ind;
		}
	}
	if (minlen < CLOSENUF) {

		SelectedFormControlVertex = ine;
		return 1;
	} else
		return 0;
}

void rtrclpfn() {

	unsigned ind, len = 0;

	if (OpenClipboard(ThrEdWindow)) {

		fvars(ClosestFormToCursor);
		if (iseclp(ClosestFormToCursor)) {

			len = SelectedForm->clipEntries;
			oclp(SelectedForm->borderClipData, len);
		} else {

			if (isclp(ClosestFormToCursor)) {

				len = SelectedForm->lengthOrCount.clipCount;
				oclp(SelectedForm->angleOrClipData.clip, len);
			}
		}
		if (len) {

			LowerLeftStitch.x = LowerLeftStitch.y = 0;
			EmptyClipboard();
			Clip = RegisterClipboardFormat(PcdClipFormat);
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len * sizeof(CLPSTCH) + 2);
			ClipStitchData = deref(ClipPointer);
			rtclpfn(0, 0);
			ClipStitchData[0].led = len;
			for (ind = 1; ind < len; ind++)
				rtclpfn(ind, ind);
			SetClipboardData(Clip, ClipPointer);
			CloseClipboard();
		}
	}
}

BOOL chkbig() {

	unsigned	ind;
	double		len, minlen = 1e99;
	POINT		tpnt;

	tpnt.x = Msg.pt.x - StitchWindowOrigin.x;
	tpnt.y = Msg.pt.y - StitchWindowOrigin.y;
	for (ind = 0; ind < 9; ind++) {

		len = hypot(tpnt.x - SelectedFormsLine[ind].x, tpnt.y - SelectedFormsLine[ind].y);
		if (len < minlen) {

			minlen = len;
			SelectedFormControlVertex = ind;
		}
	}
	for (ind = 0; ind < 4; ind++) {

		FormLines[ind].x = SelectedFormsLine[ind << 1].x;
		FormLines[ind].y = SelectedFormsLine[ind << 1].y;
	}
	FormLines[4].x = FormLines[0].x;
	FormLines[4].y = FormLines[0].y;
	if (minlen < CLOSENUF) {

		for (ind = 0; ind < 4; ind++) {

			StretchBoxLine[ind].x = SelectedFormsLine[ind << 1].x;
			StretchBoxLine[ind].y = SelectedFormsLine[ind << 1].y;
		}
		StretchBoxLine[4].x = StretchBoxLine[0].x;
		StretchBoxLine[4].y = StretchBoxLine[0].y;
		if (SelectedFormControlVertex & 1)
			setMap(STRTCH);
		else {

			setMap(EXPAND);
			XYratio = (double)(SelectedFormsRect.right - SelectedFormsRect.left) / (SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		SelectedFormControlVertex >>= 1;
		setMap(SHOSTRTCH);
		strtchbox();
		return 1;
	}
	if (tpnt.x >= SelectedFormsRect.left&&tpnt.x <= SelectedFormsRect.right
		&&tpnt.y >= SelectedFormsRect.top&&tpnt.y <= SelectedFormsRect.bottom) {

		SelectedFormsSize.x = SelectedFormsRect.right - SelectedFormsRect.left;
		SelectedFormsSize.y = SelectedFormsRect.bottom - SelectedFormsRect.top;
		setMap(MOVFRMS);
		FormMoveDelta.x = tpnt.x - SelectedFormsRect.left;
		FormMoveDelta.y = tpnt.y - SelectedFormsRect.top;
		setMap(SHOSTRTCH);
		strtchbox();
		return 1;
	}
	return 0;
}

void delfre() {

	unsigned ind, ine = 0;

	savdo();
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

		if (!(StitchBuffer[ind].attribute&NOTFRM)) {

			StitchBuffer[ine].attribute = StitchBuffer[ind].attribute;
			StitchBuffer[ine].x = StitchBuffer[ind].x;
			StitchBuffer[ine++].y = StitchBuffer[ind].y;
		}
	}
	PCSHeader.stitchCount = ine;
	coltab();
	setMap(RESTCH);
}

void setmov() {

	if (chkMap(SELBOX)) {

		MoveAnchor = ClosestPointIndex;
		setMap(MOVSET);
		setMap(RESTCH);
	}
}

void dufsel()
{
	unsigned strt, fin;

	if (LastFormSelected > ClosestFormToCursor)
	{
		strt = ClosestFormToCursor;
		fin = LastFormSelected;
	} else
	{
		strt = LastFormSelected;
		fin = ClosestFormToCursor;
	}
	SelectedFormCount = 0;
	while (strt <= fin)
	{
		SelectedFormList[SelectedFormCount] = strt;
		strt++;
		SelectedFormCount++;
	}
}

void selup() {

	unsigned at;

	if (GetKeyState(VK_SHIFT) & 0X8000) {

		rstMap(SELBOX);
		if (rstMap(FORMSEL)) {

			if (ClosestFormToCursor < FormIndex - 1) {

				SelectedFormList[0] = ClosestFormToCursor;
				LastFormSelected = ClosestFormToCursor + 1;
				SelectedFormList[1] = LastFormSelected;
				SelectedFormCount = 2;
			} else
				return;
		} else {

			if (SelectedFormCount)
			{
				if (LastFormSelected < FormIndex - 1)
				{
					LastFormSelected++;
					dufsel();
				}
			} else
				setMap(FORMSEL);
		}
		setMap(RESTCH);
	} else {

		if (chkMap(SELBOX)) {

			unbox();
			at = StitchBuffer[ClosestPointIndex].attribute&ATMSK;
			while (ClosestPointIndex < (unsigned)PCSHeader.stitchCount - 1 && (StitchBuffer[ClosestPointIndex].attribute&ATMSK) == at)
				ClosestPointIndex++;
			stch2px(ClosestPointIndex);
			dubox();
		} else {

			if (FormIndex) {

				if (setMap(FORMSEL)) {

					if (ClosestFormToCursor < (unsigned)FormIndex - 1)
						ClosestFormToCursor++;
				} else
					ClosestFormToCursor = 0;
				ritnum(STR_NUMFORM, ClosestFormToCursor);
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

			if (ClosestFormToCursor) {

				SelectedFormList[0] = ClosestFormToCursor;
				LastFormSelected = ClosestFormToCursor - 1;
				SelectedFormList[1] = LastFormSelected;
				SelectedFormCount = 2;
			} else
				return;
		} else {

			if (SelectedFormCount)
			{
				if (LastFormSelected)
				{
					LastFormSelected--;
					dufsel();
				}
			} else
				setMap(FORMSEL);
		}
		setMap(RESTCH);
	} else {

		if (chkMap(SELBOX)) {

			unbox();
			at = StitchBuffer[ClosestPointIndex].attribute&ATMSK;
			while (ClosestPointIndex && (StitchBuffer[ClosestPointIndex].attribute&ATMSK) == at)
				ClosestPointIndex--;
			stch2px(ClosestPointIndex);
			dubox();
		} else {

			if (FormIndex) {

				if (setMap(FORMSEL)) {

					if (ClosestFormToCursor)
						ClosestFormToCursor--;
				} else
					ClosestFormToCursor = FormIndex - 1;
				ritnum(STR_NUMFORM, ClosestFormToCursor);
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
		add		edi, offset StitchBuffer
		mov		eax, src
		mul		ecx
		mov		esi, eax
		add		esi, offset StitchBuffer
		mov		cl, 3
		mov		eax, cnt
		mul		ecx
		mov		ecx, eax
		rep		movsd
	}
#else
	memcpy(StitchBuffer + dst, StitchBuffer + src, cnt * sizeof(*StitchBuffer));
#endif
}

BOOL movstchs(unsigned dst, unsigned strt, unsigned fin) {

	unsigned cnt, ind;
	unsigned dind = MAXPCS >> 1;

	if (dst < (unsigned)PCSHeader.stitchCount - 1)
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
			if (movstchs(MoveAnchor, GroupStartStitch, GroupEndStitch)) {

				coltab();
				setMap(RESTCH);
			}
		} else {

			if (chkMap(SELBOX)) {

				if (movstchs(MoveAnchor, ClosestPointIndex, ClosestPointIndex + 1)) {

					setMap(RESTCH);
					coltab();
				}
			}
		}
	}
}

void vuthrds() {

	if (GetMenuState(ViewMenu, ID_VUTHRDS, MF_BYCOMMAND)&MF_CHECKED) {

		CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		rstMap(THRDS);
	} else {

		CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_CHECKED);
		setMap(THRDS);
	}
	setMap(RESTCH);
}

void vuselthr() {

	if (GetMenuState(ViewMenu, ID_VUSELTHRDS, MF_BYCOMMAND)&MF_CHECKED) {

		CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
		rstMap(COL);
	} else {

		CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_CHECKED);
		setMap(COL);
	}
	setMap(RESTCH);
}

void colchk() {

	unsigned col, cind, ind;

	col = StitchBuffer[0].attribute&COLMSK;
	cind = 0;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

		if (col != (StitchBuffer[ind].attribute&COLMSK)) {

			if ((ind - cind == 1) && (cind))
				StitchBuffer[ind - 1].attribute = StitchBuffer[ind].attribute&NCOLMSK | (StitchBuffer[cind - 1].attribute&COLMSK);
			col = StitchBuffer[ind].attribute&COLMSK;
			cind = ind;
		}
	}
}

unsigned makbig(unsigned strt, unsigned fin) {

	unsigned	dst, src, cnt, ine, adcnt = 0, at;
	fPOINTATTR*	tstchs0;
	fPOINTATTR*	tstchs1;
	double		len;
	dPOINT		dif, stp, pnt;

	dst = MAXPCS >> 1;
	for (src = strt; src < (unsigned)fin - 1; src++) {

		dif.x = StitchBuffer[src + 1].x - StitchBuffer[src].x;
		dif.y = StitchBuffer[src + 1].y - StitchBuffer[src].y;
		len = hypot(dif.x, dif.y);
		mvstch(dst++, src);
		if (len > IniFile.maxStitchLength) {

			cnt = ceil(len / UserStitchLength);
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
			pnt.x = StitchBuffer[src].x + stp.x;
			pnt.y = StitchBuffer[src].y + stp.y;
			at = StitchBuffer[src].attribute;
			if (at != StitchBuffer[src + 1].attribute) {

				if (!(at&NOTFRM) && StitchBuffer[src + 1].attribute&TYPMSK) {

					if (!((at&FRMSK) == (StitchBuffer[src + 1].attribute&FRMSK)))
						at &= NTYPMSK;
				} else
					at &= NTYPMSK;
			}
			at &= (~KNOTMSK);
			for (ine = 0; ine < cnt - 1; ine++) {

				StitchBuffer[dst].attribute = at;
				StitchBuffer[dst].x = pnt.x;
				StitchBuffer[dst++].y = pnt.y;
				pnt.x += stp.x;
				pnt.y += stp.y;
				adcnt++;
			}
		}
	}
	while (src < PCSHeader.stitchCount)
		mvstch(dst++, src++);
	tstchs0 = &StitchBuffer[MAXPCS >> 1];
	tstchs1 = &StitchBuffer[strt];
	cnt = dst - (MAXPCS >> 1);
	for (src = 0; src < cnt; src++)
		tstchs1[src] = tstchs0[src];
	PCSHeader.stitchCount = strt + cnt;
	return adcnt;
}

void rembig() {

	RANGE rng;
	unsigned ind;

	if (UserStitchLength < IniFile.maxStitchLength) {

		savdo();
		if (SelectedFormCount) {

			for (ind = 0; ind < SelectedFormCount; ind++) {

				frmrng(SelectedFormList[ind], &rng);
				makbig(rng.start, rng.finish);
			}
			goto bigdun;
		}
		if (chkMap(FORMSEL)) {

			frmrng(ClosestFormToCursor, &rng);
			makbig(rng.start, rng.finish);
			goto bigdun;
		}
		if (chkMap(GRPSEL)) {

			rngadj();
			if (GroupEndStitch < PCSHeader.stitchCount)
				GroupEndStitch++;
			if (ClosestPointIndex < GroupStitchIndex)
				GroupStitchIndex += makbig(GroupStartStitch, GroupEndStitch);
			else
				ClosestPointIndex += makbig(GroupStartStitch, GroupEndStitch);
			grpAdj();
			goto bigdun;
		}
		makbig(0, PCSHeader.stitchCount);

	bigdun:;
		coltab();
		setMap(RESTCH);
	} else
		tabmsg(IDS_REM1);
}

void duselrng() {

	SelectedRange.start = 0;
	SelectedRange.finish = PCSHeader.stitchCount;
	if (chkMap(GRPSEL)) {

		rngadj();
		SelectedRange.start = GroupStartStitch;
		SelectedRange.finish = GroupEndStitch;
		return;
	}
}

void longer() {

	unsigned	ind, ine;
	double		len, minlen = 1e99;
	double		curlen;

	if (ClosestPointIndex == LargestStitchIndex)
		return;
	curlen = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x, StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (ind = ClosestPointIndex + 1; ind < SelectedRange.finish; ind++) {

		len = hypot(StitchBuffer[ind + 1].x - StitchBuffer[ind].x, StitchBuffer[ind + 1].y - StitchBuffer[ind].y);
		if (len == curlen)
			goto long1;
	}
	for (ine = SelectedRange.start; ine < SelectedRange.finish - 1; ine++) {

		len = hypot(StitchBuffer[ine + 1].x - StitchBuffer[ine].x, StitchBuffer[ine + 1].y - StitchBuffer[ine].y);
		if (len > curlen&&len < minlen) {

			minlen = len;
			ind = ine;
		}
	}
	if (minlen == 1e99)
		return;
long1:;
	CurrentStitchIndex = ind;
	lensadj();
	ritnum(STR_NUMSCH, ClosestPointIndex);
}

void shorter() {

	unsigned	ind, ine;
	double		len, maxlen = 0;
	double		curmax;

	if (ClosestPointIndex == SmallestStitchIndex)
		return;
	curmax = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x, StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (ine = ClosestPointIndex; ine != 0; ine--) {

		len = hypot(StitchBuffer[ine].x - StitchBuffer[ine - 1].x, StitchBuffer[ine].y - StitchBuffer[ine - 1].y);
		if (len == curmax) {
			ine--;
			goto short1;
		}
	}
	for (ind = SelectedRange.start; ind < SelectedRange.finish - 1; ind++) {

		len = hypot(StitchBuffer[ind + 1].x - StitchBuffer[ind].x, StitchBuffer[ind + 1].y - StitchBuffer[ind].y);
		if (len<curmax&&len>maxlen) {

			maxlen = len;
			ine = ind;
		}
	}
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", hypot(StitchBuffer[ind + 1].x - StitchBuffer[ind].x, StitchBuffer[ind + 1].y - StitchBuffer[ind].y));
	butxt(HMINLEN, MsgBuffer);
short1:;
	CurrentStitchIndex = ine;
	lensadj();
	ritnum(STR_NUMSCH, ClosestPointIndex);
}

void setsrch(unsigned p_stind) {

	CurrentStitchIndex = p_stind;
	lensadj();
	ritnum(STR_NUMSCH, ClosestPointIndex);
}

BOOL inrng(unsigned p_stind) {

	if (StitchBuffer[p_stind].x >= StitchRangeRect.left
		&&StitchBuffer[p_stind].x <= StitchRangeRect.right
		&&StitchBuffer[p_stind].y >= StitchRangeRect.bottom
		&&StitchBuffer[p_stind].y <= StitchRangeRect.top)

		return 1;
	else
		return 0;
}

BOOL finrng(unsigned find) {

	unsigned cod;

	if (FormList[find].rectangle.left >= StitchRangeRect.left
		&&FormList[find].rectangle.right <= StitchRangeRect.right
		&&FormList[find].rectangle.bottom >= StitchRangeRect.bottom
		&&FormList[find].rectangle.top <= StitchRangeRect.top) {

		if (!ActiveLayer)
			return 1;
		cod = (FormList[find].attribute&FRMLMSK) >> 1;
		if (!cod || ActiveLayer == cod)
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
		ClosestPointIndex = GroupStartStitch;
		setMap(SELBOX);
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				if (!(StitchBuffer[ind].attribute&NOTFRM) && ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) == ClosestFormToCursor) {

					ClosestPointIndex = ind;
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
		ClosestPointIndex = GroupEndStitch;
		setMap(SELBOX);
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			for (ind = PCSHeader.stitchCount; ind != 0; ind--) {

				if (!(StitchBuffer[ind - 1].attribute&NOTFRM) && ((StitchBuffer[ind - 1].attribute&FRMSK) >> FRMSHFT) == ClosestFormToCursor) {

					ClosestPointIndex = ind - 1;
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

	if (PCSHeader.stitchCount) {

		rct->bottom = rct->top = RotatedStitches[0].y;
		rct->left = rct->right = RotatedStitches[0].x;
		for (ind = 1; ind < PCSHeader.stitchCount; ind++) {

			if (RotatedStitches[ind].x < rct->left)
				rct->left = RotatedStitches[ind].x;
			if (RotatedStitches[ind].x > rct->right)
				rct->right = RotatedStitches[ind].x;
			if (RotatedStitches[ind].y < rct->bottom)
				rct->bottom = RotatedStitches[ind].y;
			if (RotatedStitches[ind].y > rct->top)
				rct->top = RotatedStitches[ind].y;
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
	RotatedStitches = StitchBuffer;
	sizstch(&CheckHoopRect);
	if (FormIndex) {

		if (!PCSHeader.stitchCount) {

			CheckHoopRect.bottom = CheckHoopRect.top = CurrentFormVertices[0].y;
			CheckHoopRect.left = CheckHoopRect.right = CurrentFormVertices[0].x;
		}
		for (ind = 0; ind < FormVertexIndex; ind++) {

			if (FormVertices[ind].x < CheckHoopRect.left)
				CheckHoopRect.left = FormVertices[ind].x;
			if (FormVertices[ind].x > CheckHoopRect.right)
				CheckHoopRect.right = FormVertices[ind].x;
			if (FormVertices[ind].y < CheckHoopRect.bottom)
				CheckHoopRect.bottom = FormVertices[ind].y;
			if (FormVertices[ind].y > CheckHoopRect.top)
				CheckHoopRect.top = FormVertices[ind].y;
		}
	}
	if (PCSHeader.stitchCount || FormVertexIndex || chkMap(HUPEX)) {

		if (CheckHoopRect.left<0 ||
			CheckHoopRect.right>IniFile.hoopSizeX ||
			CheckHoopRect.bottom<0 ||
			CheckHoopRect.top>IniFile.hoopSizeY) {

			setMap(HUPEX);
		}
		if (chkMap(HUPEX)) {

			tsiz.x = CheckHoopRect.right - CheckHoopRect.left;
			tsiz.y = CheckHoopRect.top - CheckHoopRect.bottom;
			if (tsiz.x > IniFile.hoopSizeX) {

				IniFile.hoopSizeX = tsiz.x;
				setMap(HUPCHNG);
			}
			if (tsiz.y > IniFile.hoopSizeY) {

				IniFile.hoopSizeY = tsiz.y;
				setMap(HUPCHNG);
			}
			descntr.x = tsiz.x / 2 + CheckHoopRect.left;
			descntr.y = tsiz.y / 2 + CheckHoopRect.bottom;
			hupcntr.x = IniFile.hoopSizeX / 2;
			hupcntr.y = IniFile.hoopSizeY / 2;
			dif.x = hupcntr.x - descntr.x;
			dif.y = hupcntr.y - descntr.y;
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				StitchBuffer[ind].x += dif.x;
				StitchBuffer[ind].y += dif.y;
			}
			for (ind = 0; ind < FormVertexIndex; ind++) {

				FormVertices[ind].x += dif.x;
				FormVertices[ind].y += dif.y;
			}
			for (ind = 0; ind < FormIndex; ind++) {

				FormList[ind].rectangle.left += dif.x;
				FormList[ind].rectangle.right += dif.x;
				FormList[ind].rectangle.top += dif.y;
				FormList[ind].rectangle.bottom += dif.y;
			}
			UnzoomedRect.x = IniFile.hoopSizeX;
			UnzoomedRect.y = IniFile.hoopSizeY;
			ZoomMin = (double)MINZUM / UnzoomedRect.x;
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

		mov		ebx, offset UpperCaseMap
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

		if (_bittest((long *)UpperCaseMap, chr))
			chr |= 0x20;

		*(dst++) = chr;
	}

	*dst = 0;
#endif
}

TCHAR lchr(int op) {

#if  __UseASM__
	_asm {

		mov		ebx, offset UpperCaseMap
		mov		eax, op
		and		eax, 0x7f
		bt[ebx], eax
		jnc		short lchrx
		or al, 0x20
		lchrx:
	}
#else
	TCHAR chr = op & 0x7f;

	return _bittest((long *)UpperCaseMap, chr) ? chr : chr | 0x20;
#endif
}

void barnam(HWND hndl, unsigned fpnt) {

	TCHAR		buf[_MAX_PATH];
	TCHAR*		pchr;

	if (fpnt < ThumbnailDisplayCount&&ThumbnailsSelected[fpnt]) {

		strcpy_s(buf, ThumbnailsSelected[fpnt]);
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

		barnam(ButtonWin[HNUM], ind);
		break;

	case 1:

		barnam(ButtonWin[HTOT], ind);
		break;

	case 2:

		barnam(ButtonWin[HMINLEN], ind);
		break;

	case 3:

		barnam(ButtonWin[HMAXLEN], ind);
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
	ThumbnailNames = (TCHAR*)OSequence;
	Thumbnails = (TCHAR**)&OSequence[MAXSEQ >> 1];

	SetCurrentDirectory(DefaultDirectory);
	strcpy_s(SearchName, DefaultDirectory);
	pchr = &SearchName[strlen(SearchName) - 1];
	if (pchr[0] != '\\') {

		pchr[1] = '\\';
		pchr[2] = 0;
	}
	strcat_s(SearchName, "*.thr");
	shndl = FindFirstFile(SearchName, &fdat);
	if (shndl == INVALID_HANDLE_VALUE) {

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Can't find %s\n", SearchName);
		shoMsg(MsgBuffer);
		unthum();
	} else {

		ine = 0;
		Thumbnails[0] = ThumbnailNames;
		strcpy(Thumbnails[0], fdat.cFileName);
		ine += strlen(fdat.cFileName) + 1;
		Thumbnails[1] = &ThumbnailNames[ine];
		ind = 1;
		while (FindNextFile(shndl, &fdat)) {

			strcpy(Thumbnails[ind], fdat.cFileName);
			ine += strlen(fdat.cFileName) + 1;
			Thumbnails[++ind] = &ThumbnailNames[ine];
		}
		FindClose(shndl);
		ThumbnailCount = ind;
		qsort((void*)&Thumbnails[0], ind, 4, strcomp);
		ind = ThumbnailIndex = 0;
		while (ind < 4 && ThumbnailIndex < ThumbnailCount&&ind < ThumbnailCount) {

			ThumbnailsSelected[ind] = Thumbnails[ind];
			ind++;
		}
		ThumbnailIndex = ThumbnailDisplayCount = ind;
		while (ind < 4 && ind < ThumbnailCount)
			rthumnam(ind++);
		setMap(THUMSHO);
		ThumbnailSearchString[0] = 0;
		SetWindowText(ButtonWin[HBOXSEL], "");
		butxt(HBOXSEL, "");
		vubak();
		setMap(RESTCH);
	}
}
#pragma warning(pop)

void nuthsel() {

	unsigned ind, len, bakind;

	if (ThumbnailIndex < ThumbnailCount) {

		bakind = ThumbnailIndex;
		ind = 0;
		len = strlen(ThumbnailSearchString);
		setMap(RESTCH);
		if (len) {

			while (ind < 4 && ThumbnailIndex < ThumbnailCount) {

				if (!strncmp(ThumbnailSearchString, Thumbnails[ThumbnailIndex], len)) {

					ThumbnailsSelected[ind] = Thumbnails[ThumbnailIndex];
					redraw(BackupViewer[ind++]);
				}
				ThumbnailIndex++;
			}
		} else {

			while (ind < 4 && ThumbnailIndex < ThumbnailCount) {

				ThumbnailsSelected[ind] = Thumbnails[ThumbnailIndex];
				redraw(BackupViewer[ind++]);
				ThumbnailIndex++;
			}
		}
		if (ind) {

			ThumbnailDisplayCount = ind;
			while (ind < 4)
				rthumnam(ind++);
		} else
			ThumbnailIndex = bakind;
	}
}

void nuthbak(unsigned cnt) {

	unsigned len;

	if (ThumbnailIndex) {

		len = strlen(ThumbnailSearchString);
		if (len) {

			while (cnt&&ThumbnailIndex < MAXFORMS) {

				if (ThumbnailIndex) {

					ThumbnailIndex--;
					if (!strncmp(ThumbnailSearchString, Thumbnails[ThumbnailIndex], len))
						cnt--;
				} else
					break;
			}
		} else
			ThumbnailIndex -= cnt;
		if (ThumbnailIndex > MAXFORMS)
			ThumbnailIndex = 0;
		nuthsel();
	}
}

void nuthum(TCHAR chr) {

	unsigned ind;

	ind = strlen(ThumbnailSearchString);
	if (ind < 16) {

		setMap(RESTCH);
		ThumbnailSearchString[ind++] = chr;
		ThumbnailSearchString[ind] = 0;
		butxt(HBOXSEL, ThumbnailSearchString);
		ThumbnailIndex = 0;
		nuthsel();
	}
}

void bakthum() {

	unsigned ind;

	ind = strlen(ThumbnailSearchString);
	if (ind) {

		setMap(RESTCH);
		ThumbnailSearchString[--ind] = 0;
		ThumbnailIndex = 0;
		butxt(HBOXSEL, ThumbnailSearchString);
		nuthsel();
	}
}

void selalstch() {

	if (PCSHeader.stitchCount) {

		ClosestPointIndex = 0;
		GroupStitchIndex = PCSHeader.stitchCount - 1;
		GroupStartStitch = ClosestPointIndex;
		GroupEndStitch = GroupStitchIndex;
		setMap(GRPSEL);
		setMap(SCROS);
		setMap(ECROS);
		grpAdj();
		setMap(RESTCH);
	}
}

void insflin(POINT ipnt) {

	POINT off;

	off.x = InsertSize.x >> 1;
	off.y = InsertSize.y >> 1;

	FormLines[0].x = FormLines[3].x = FormLines[4].x = ipnt.x - off.x;
	FormLines[1].x = FormLines[2].x = ipnt.x + off.x;
	FormLines[0].y = FormLines[1].y = FormLines[4].y = ipnt.y - off.y;
	FormLines[2].y = FormLines[3].y = ipnt.y + off.y;
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

unsigned gethand(fPOINTATTR* p_pstch, unsigned cnt) {

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
		ThrEdWindow,					//hwndOwner 
		ThrEdInstance,					//hInstance 
		"THR files\0*.thr\0\0",	//lpstrFilter 
		CustomFilter,			//lpstrCustomFilter 
		_MAX_PATH,				//nMaxCustFilter 
		0,						//nFilterIndex 
		InsertedFileName,			//lpstrFile 
		_MAX_PATH,				//nMaxFile 
		0,						//lpstrFileTitle 
		0,						//nMaxFileTitle 
		DefaultDirectory,					//lpstr	ialDir 
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

		InsertedFileHandle = CreateFile(InsertedFileName, (GENERIC_READ), 0, NULL,
			OPEN_EXISTING, 0, NULL);
		if (InsertedFileHandle == INVALID_HANDLE_VALUE) {

			filnopn(IDS_FNOPN, InsertedFileName);
			FileHandle = 0;
			CloseHandle(InsertedFileHandle);
		} else {

			InsertedStitchCount = PCSHeader.stitchCount;
			if (isthr(InsertedFileName)) {

				ReadFile(InsertedFileHandle, (STRHED*)&thed, sizeof(STRHED), &BytesRead, NULL);
				if ((thed.headerType & 0xffffff) != 0x746872)
					tabmsg(IDS_NOTHR);
				else {

					vervar = (thed.headerType & 0xff000000) >> 24;
					if (vervar) {
#define FRMW 5
#define HANDW 4
#define FRMPW 2
#define STCHW 1			
						gethand(StitchBuffer, PCSHeader.stitchCount);
						homscor = (double)
							FormIndex*FRMW +
							gethand(StitchBuffer, PCSHeader.stitchCount)*HANDW +
							FormVertexIndex*FRMPW +
							PCSHeader.stitchCount*STCHW;
						ReadFile(InsertedFileHandle, (STREX*)&thedx, sizeof(STREX), &BytesRead, 0);
					}
					savdo();
					ReadFile(InsertedFileHandle, (fPOINTATTR*)&StitchBuffer[PCSHeader.stitchCount], thed.stitchCount * sizeof(fPOINTATTR), &BytesRead, NULL);
					SetFilePointer(InsertedFileHandle, 164, 0, FILE_CURRENT);
					trct.left = trct.bottom = (float)1e9;
					trct.top = trct.right = (float)1e-9;
					codof = FormIndex << FRMSHFT;
					InsertedVertexIndex = FormVertexIndex; 
					InsertedFormIndex = FormIndex;
					if (thed.vertexCount) {

						if (vervar < 2) {

							frmlstx = (FRMHEDO*)&BSequence;
							ReadFile(InsertedFileHandle, (FRMHEDO*)&BSequence, thed.formCount * sizeof(FRMHEDO), &BytesRead, 0);
							if (BytesRead != thed.formCount * sizeof(FRMHEDO)) {

								FormIndex = BytesRead / sizeof(FRMHEDO);
								setMap(BADFIL);
							}
							if (FormIndex + thed.vertexCount < MAXFORMS) {

								FillMemory(&FormList[FormIndex], thed.formCount * sizeof(FRMHED), 0);
								ine = FormIndex;
								for (ind = 0; ind < thed.formCount; ind++) {

									MoveMemory(&FormList[ine], &frmlstx[ind], sizeof(FRMHEDO));
									ine++;
								}
							}
						} else
							ReadFile(InsertedFileHandle, (FRMHED*)&FormList[FormIndex], thed.formCount * sizeof(FRMHED), &BytesRead, 0);
						ReadFile(InsertedFileHandle, (fPOINT*)&FormVertices[FormVertexIndex], thed.vertexCount * sizeof(fPOINT), &BytesRead, 0);
						ReadFile(InsertedFileHandle, (SATCON*)&SatinConnects[SatinConnectIndex], thed.dlineCount * sizeof(SATCON), &BytesRead, 0);
						ReadFile(InsertedFileHandle, (fPOINT*)&ClipPoints[ClipPointIndex], thed.clipDataCount * sizeof(fPOINT), &BytesRead, 0);
						CloseHandle(InsertedFileHandle);
						InsertedFileHandle = 0;
						for (ind = FormIndex; ind < FormIndex + thed.formCount; ind++) {

							FormList[ind].vertices = adflt(FormList[ind].sides);
							if (FormList[ind].type == SAT) {

								if (FormList[ind].satinGuideCount)
									FormList[ind].satinOrAngle.sac = adsatk(FormList[ind].satinGuideCount);
								if (isclpx(ind))
									FormList[ind].angleOrClipData.clip = adclp(FormList[ind].lengthOrCount.clipCount);
							}
							if (isclp(ind))
								FormList[ind].angleOrClipData.clip = adclp(FormList[ind].lengthOrCount.clipCount);
							if (iseclpx(ind))
								FormList[ind].borderClipData = adclp(FormList[ind].clipEntries);
						}
						FormIndex += thed.formCount;
						if (thed.formCount) {

							trct.left = trct.right = FormVertices[InsertedVertexIndex].x;
							trct.bottom = trct.top = FormVertices[InsertedVertexIndex].y;
							for (ind = InsertedVertexIndex + 1; ind < FormVertexIndex; ind++) {

								if (FormVertices[ind].x < trct.left)
									trct.left = FormVertices[ind].x;
								if (FormVertices[ind].x > trct.right)
									trct.right = FormVertices[ind].x;
								if (FormVertices[ind].y < trct.bottom)
									trct.bottom = FormVertices[ind].y;
								if (FormVertices[ind].y > trct.top)
									trct.top = FormVertices[ind].y;
							}
						}
					}
					if (thed.stitchCount) {

						for (ind = PCSHeader.stitchCount; ind < (unsigned)PCSHeader.stitchCount + thed.stitchCount; ind++) {

							if (StitchBuffer[ind].attribute&ALTYPMSK) {

								cod = (StitchBuffer[ind].attribute&FRMSK) + codof;
								StitchBuffer[ind].attribute &= NFRMSK;
								StitchBuffer[ind].attribute |= cod;
							}
							if (StitchBuffer[ind].x < trct.left)
								trct.left = StitchBuffer[ind].x;
							if (StitchBuffer[ind].x > trct.right)
								trct.right = StitchBuffer[ind].x;
							if (StitchBuffer[ind].y < trct.bottom)
								trct.bottom = StitchBuffer[ind].y;
							if (StitchBuffer[ind].y > trct.top)
								trct.top = StitchBuffer[ind].y;
						}
					}
					if (thed.headerType & 0x1000000) {
						// ToDo - should FRMPW be sizeof(fPOINTATTR)?
						filscor = (double)thed.formCount*FRMW +
							gethand(&StitchBuffer[PCSHeader.stitchCount], thed.stitchCount)*HANDW +
							thed.vertexLen*FRMPW +
							thed.stitchCount*STCHW;
						if (filscor > homscor) {

							for (ind = 0; ind < 50; ind++)
								ExtendedHeader.creatorName[ind] = thedx.creatorName[ind];
							redfnam(DesignerName);
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRDBY], ThrName, DesignerName);
							SetWindowText(ThrEdWindow, MsgBuffer);
						}
					}
					InsertCenter.x = (trct.right - trct.left) / 2 + trct.left;
					InsertCenter.y = (trct.top - trct.bottom) / 2 + trct.bottom;
					PCSHeader.stitchCount += thed.stitchCount;
					tsiz.x = trct.right - trct.left;
					tsiz.y = trct.top - trct.bottom;
					ratsr();
					InsertSize.x = tsiz.x*HorizontalRatio;
					// ToDo - Should this be vertical ratio?
					InsertSize.y = tsiz.y*HorizontalRatio;
					tpnt.x = StitchWindowClientRect.right >> 1;
					tpnt.y = StitchWindowClientRect.bottom >> 1;
					insflin(tpnt);
					NewFormVertexCount = 5;
					setMap(SHOFRM);
					setMap(INSFIL);
					dufrm();
				}
			} else {

				ReadFile(InsertedFileHandle, (PCSHEADER*)&tphed, 0x46, &BytesRead, NULL);
				if (PCSHeader.leadIn == 0x32 && PCSHeader.colorCount == 16) {

					savdo();
					tbuf = (PCSTCH*)BSequence;
					ReadFile(InsertedFileHandle, (PCSTCH*)tbuf, tphed.stitchCount * sizeof(PCSTCH), &BytesRead, NULL);
					ine = PCSHeader.stitchCount;
					cod = 0;
					for (ind = 0; ind < tphed.stitchCount; ind++) {

						if (tbuf[ind].tag == 3)
							cod = tbuf[ind++].fx;
						else {

							StitchBuffer[ine].x = tbuf[ind].x + (float)tbuf[ind].fx / 256;
							StitchBuffer[ine].y = tbuf[ind].y + (float)tbuf[ind].fy / 256;
							StitchBuffer[ine++].attribute = cod;
						}
					}
					ind = PCSHeader.stitchCount;
					trct.left = trct.right = StitchBuffer[ind].x;
					trct.top = trct.bottom = StitchBuffer[ind++].y;
					while (ind < (unsigned)ine) {

						if (StitchBuffer[ind].x < trct.left)
							trct.left = StitchBuffer[ind].x;
						if (StitchBuffer[ind].x > trct.right)
							trct.right = StitchBuffer[ind].x;
						if (StitchBuffer[ind].y < trct.bottom)
							trct.bottom = StitchBuffer[ind].y;
						if (StitchBuffer[ind].y > trct.top)
							trct.top = StitchBuffer[ind].y;
						ind++;
					}
					InsertCenter.x = (trct.right - trct.left) / 2 + trct.left;
					InsertCenter.y = (trct.top - trct.bottom) / 2 + trct.bottom;
					PCSHeader.stitchCount = ine;
					tsiz.x = trct.right - trct.left;
					tsiz.y = trct.top - trct.bottom;
					ratsr();
					InsertSize.x = tsiz.x*HorizontalRatio;
					InsertSize.y = tsiz.y*HorizontalRatio;
					tpnt.x = StitchWindowClientRect.right >> 1;
					tpnt.y = StitchWindowClientRect.bottom >> 1;
					insflin(tpnt);
					NewFormVertexCount = 5;
					setMap(SHOFRM);
					setMap(INSFIL);
					dufrm();
				}
			}
			if (InsertedFileHandle)
				CloseHandle(InsertedFileHandle);
		}
	}
}

void duinsfil() {

	fPOINT		off;
	unsigned	ind;
	fRECTANGLE*		trct;

	px2stch();
	off.x = SelectedPoint.x - InsertCenter.x;
	off.y = SelectedPoint.y - InsertCenter.y;
	for (ind = InsertedFormIndex; ind < FormIndex; ind++) {

		trct = &FormList[ind].rectangle;
		trct->bottom += off.y;
		trct->top += off.y;
		trct->left += off.x;
		trct->right += off.x;
	}
	for (ind = InsertedVertexIndex; ind < FormVertexIndex; ind++) {

		FormVertices[ind].x += off.x;
		FormVertices[ind].y += off.y;
	}
	for (ind = InsertedStitchCount; ind < PCSHeader.stitchCount; ind++) {

		StitchBuffer[ind].x += off.x;
		StitchBuffer[ind].y += off.y;
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
		prng = (RANGE*)&BSequence;
		rstMap(GRPSEL);
		while (ind < PCSHeader.stitchCount) {

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

				ClosestPointIndex = prng[pmax].start;
				GroupStitchIndex = prng[pmax].finish;
				setMap(GRPSEL);
			}
			gotbox();
		}
	}
}

void nucols() {

	unsigned ind;

	clRmap((FormIndex << 5) + 1);
	for (ind = 0; ind < SelectedFormCount; ind++) {

		setr(SelectedFormList[ind]);
		SelectedForm = &FormList[ind];
		if (SelectedForm->fillType) {

			SelectedForm->fillColor = ActiveColor;
			SelectedForm->fillInfo.feather.color = ActiveColor;
		}
		if (SelectedForm->edgeType)
			SelectedForm->borderColor = ActiveColor;
		if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
			SelectedForm->underlayColor = ActiveColor;
	}
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

		if (chkr((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) && (StitchBuffer[ind].attribute&TYPMSK) != TYPMSK) {

			StitchBuffer[ind].attribute &= NCOLMSK;
			StitchBuffer[ind].attribute |= ActiveColor;
		}
	}
}

BOOL dunum(unsigned cod) {

	if (cod >= '0'&&cod <= '9') {

		NumericCode = cod;
		return 1;
	}
	if (cod >= VK_NUMPAD0 && cod <= VK_NUMPAD9) {

		NumericCode = cod - '0';
		return 1;
	}
	return 0;
}

void stchrct(fRECTANGLE* rct)
{
	unsigned ind;

	if (PCSHeader.stitchCount)
	{
		rct->bottom = rct->left = 1e10;
		rct->top = rct->right = 0;
		if (PCSHeader.stitchCount) {

			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				if (StitchBuffer[ind].x < rct->left)
					rct->left = StitchBuffer[ind].x;
				if (StitchBuffer[ind].x > rct->right)
					rct->right = StitchBuffer[ind].x;
				if (StitchBuffer[ind].y < rct->bottom)
					rct->bottom = StitchBuffer[ind].y;
				if (StitchBuffer[ind].y > rct->top)
					rct->top = StitchBuffer[ind].y;
			}
		}
	}
}

void frmrct(fRECTANGLE* rct)
{
	unsigned ind;

	rct->left = rct->right = FormVertices[0].x;
	rct->top = rct->bottom = FormVertices[0].y;
	for (ind = 0; ind < FormVertexIndex; ind++) {

		if (FormVertices[ind].x < rct->left)
			rct->left = FormVertices[ind].x;
		if (FormVertices[ind].x > rct->right)
			rct->right = FormVertices[ind].x;
		if (FormVertices[ind].y > rct->top)
			rct->top = FormVertices[ind].y;
		if (FormVertices[ind].y < rct->bottom)
			rct->bottom = FormVertices[ind].y;
	}
}

void desiz() {

	fRECTANGLE		trct;
	FLOAT		sizx, sizy;
	TCHAR*		pmsg;

	pmsg = MsgBuffer;
	if (PCSHeader.stitchCount) {

		stchrct(&trct);
		sizx = (trct.right - trct.left) / PFGRAN;
		sizy = (trct.top - trct.bottom) / PFGRAN;
		if ((trct.left < 0) || (trct.bottom < 0) || (trct.right > IniFile.hoopSizeX) || (trct.top > IniFile.hoopSizeY)) {

			strcpy_s(MsgBuffer, StringTable[STR_STCHOUT]);
			pmsg = &MsgBuffer[strlen(MsgBuffer)];
		}
#if LANG==ENG||LANG==HNG

		sprintf_s(pmsg, sizeof(MsgBuffer),
			StringTable[STR_STCHS],
			PCSHeader.stitchCount,
			sizx, sizx / 25.4,
			sizy, sizy / 25.4);
#endif

#if LANG==GRM

		sprintf_s(pmsg, sizeof(MsgBuffer),
			StringTable[STR_STCHS],
			PCSHeader.stitchCount,
			sizx,
			sizy);
#endif
		pmsg = &pmsg[strlen(pmsg)];
	}
	if (FormIndex) {

		frmrct(&trct);
		sizx = (trct.right - trct.left) / PFGRAN;
		sizy = (trct.top - trct.bottom) / PFGRAN;
#if LANG==ENG||LANG==HNG

		sprintf_s(pmsg, sizeof(MsgBuffer), StringTable[STR_FORMS],
			FormIndex,
			sizx, sizx / 25.4,
			sizy, sizy / 25.4);
#endif

#if LANG==GRM

		sprintf_s(pmsg, sizeof(MsgBuffer), StringTable[STR_FORMS],
			FormIndex,
			sizx,
			sizy);
#endif
		pmsg = &pmsg[strlen(pmsg)];
	}
	sprintf_s(pmsg, sizeof(MsgBuffer), StringTable[STR_HUPWID],
		IniFile.hoopSizeX / PFGRAN,
		IniFile.hoopSizeY / PFGRAN);
	pmsg = &pmsg[strlen(pmsg)];
	if (PCSHeader.stitchCount) {

		sprintf_s(pmsg, sizeof(MsgBuffer), StringTable[STR_CREATBY],
			DesignerName,
			ExtendedHeader.modifierName);
	}
	shoMsg(MsgBuffer);
}

void sidhup() {

	RECT		huprct;
	RECT		prfrct;
	unsigned	ind;

	setMap(HUPMSG);
	GetWindowRect(ValueWindow[PHUP], &huprct);
	GetWindowRect(PreferencesWindow, &prfrct);
	SideMessageWindow = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		prfrct.right + 3 - ThredWindowOrigin.x,
		huprct.top - ThredWindowOrigin.y,
		ButtonWidthX3 + ButtonWidth * 2 + 6,
		ButtonHeight*HUPS + 6,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
	for (ind = 0; ind < HUPS; ind++) {

		SideWindow[ind] = CreateWindow(
			"STATIC",
			StringTable[ind + STR_HUP0],
			SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
			3,
			ButtonHeight*ind + 3,
			ButtonWidthX3 + ButtonWidth * 2,
			ButtonHeight,
			SideMessageWindow,
			NULL,
			ThrEdInstance,
			NULL);
	}
}

void setpcs() {

	IniFile.auxFileType = AUXPCS;
	auxmen();
}

void setdst() {

	IniFile.auxFileType = AUXDST;
	auxmen();
}

void fop() {

	untrace();
	if (FormIndex || PCSHeader.stitchCount) {

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

void clpradj(fPOINTATTR pnt) {

	if (pnt.x < ClipRectAdjusted.left)
		ClipRectAdjusted.left = pnt.x;
	if (pnt.x > ClipRectAdjusted.right)
		ClipRectAdjusted.right = pnt.x;
	if (pnt.y < ClipRectAdjusted.bottom)
		ClipRectAdjusted.bottom = pnt.y;
	if (pnt.y > ClipRectAdjusted.top)
		ClipRectAdjusted.top = pnt.y;
}

void clpadj() {

	double		mid;
	unsigned	ind;

	if (chkMap(GRPSEL)) {

		rngadj();
		ind = GroupStartStitch;
		ClipRectAdjusted.left = ClipRect.right = StitchBuffer[ind++].x;
		ClipRectAdjusted.top = ClipRectAdjusted.bottom = StitchBuffer[ind].y;
		clpradj(StitchBuffer[ind]);
		while (ind < GroupEndStitch)
			clpradj(StitchBuffer[ind++]);
		if (StitchBuffer[ind].x < ClipRectAdjusted.left)
			ClipRectAdjusted.left = StitchBuffer[ind].x;
		if (StitchBuffer[ind].x > ClipRectAdjusted.right)
			ClipRectAdjusted.right = StitchBuffer[ind].x;
		mid = (ClipRectAdjusted.right - ClipRectAdjusted.left) / 2 + ClipRectAdjusted.left;
		StitchBuffer[GroupStartStitch].y = StitchBuffer[GroupEndStitch].y = (ClipRectAdjusted.top - ClipRectAdjusted.bottom) / 2 + ClipRectAdjusted.bottom;
		if (StitchBuffer[GroupStartStitch].x < mid) {

			StitchBuffer[GroupStartStitch].x = ClipRectAdjusted.left;
			StitchBuffer[GroupEndStitch].x = ClipRectAdjusted.right;
		} else {

			StitchBuffer[GroupEndStitch].x = ClipRectAdjusted.left;
			StitchBuffer[GroupStartStitch].x = ClipRectAdjusted.right;
		}
		coltab();
		setMap(RESTCH);
	} else
		shoseln(IDS_GRPMSG, IDS_RNGEND);
}

void shftflt(fPOINT pnt) {

	if (pnt.x <= ZoomRect.left || pnt.x >= ZoomRect.right || pnt.y <= ZoomRect.bottom || pnt.y >= ZoomRect.top)
		shft(pnt);
}

void fil2men() {

	if (chku(FIL2OF)) {

		CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_CHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_FIL2SEL_ON, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_FIL2SEL_OFF, MF_UNCHECKED);
	}

}

void defpref() {

	unsigned ind;

	UserFlagMap = 0;
	for (ind = 0; ind < 16; ind++) {

		UserColor[ind] = DefaultUserColors[ind];
		CustomColor[ind] = DefaultCustomColors[ind];
		CustomBackgroundColor[ind] = DefaultCustomBackgroundColors[ind];
		BitmapBackgroundColors[ind] = DefaultBitmapBackgroundColors[ind];
	}
	dazdef();
	UnderlayColor = 15;
	BorderWidth = BRDWID;
	ButtonholeCornerLength = IBFCLEN;
	IniFile.chainSpace = CHSDEF;
	IniFile.chainRatio = CHRDEF;
	IniFile.fillAngle = DEFANG;
	rstu(SQRFIL);
	StitchSpacing = DEFSPACE*PFGRAN;
	ShowStitchThreshold = SHOPNTS;
	IniFile.gridSize = 12;
	IniFile.hoopType = LARGHUP;
	IniFile.hoopSizeX = LHUPX;
	IniFile.hoopSizeY = LHUPY;
	IniFile.cursorNudgeStep = NUGINI;
	IniFile.nudgePixels = DEFPIX;
	setu(BLUNT);
	SmallStitchLength = SMALSIZ*PFGRAN;
	SnapLength = SNPLEN*PFGRAN;
	SpiralWrap = SPIRWRAP;
	StarRatio = STARAT;
	StitchBoxesThreshold = STCHBOX;
	IniFile.maxStitchLength = MAXSIZ*PFGRAN;
	UserStitchLength = USESIZ*PFGRAN;
	MinStitchLength = MINSIZ*PFGRAN;
	IniFile.clipOffset = 0;
	IniFile.fillPhase = 0;
	BitmapColor = 0xc8dfee;
	if (!IniFile.customHoopX)
		IniFile.customHoopX = LHUPX;
	if (!IniFile.customHoopY)
		IniFile.customHoopY = LHUPY;
	PicotSpacing = IPICSPAC;
	setu(FIL2OF);
	fil2men();
	BackgroundColor = 0xa8c4b1;
	UnzoomedRect.x = IniFile.hoopSizeX;
	UnzoomedRect.y = IniFile.hoopSizeY;
	IniFile.waveEnd = IWAVEND;
	IniFile.wavePoints = IWAVPNTS;
	IniFile.waveLobes = IWAVS;
	IniFile.waveStart = IWAVSTRT;
	IniFile.featherFillType = FDEFTYP;
	IniFile.featherUpCount = FDEFUP;
	IniFile.featherDownCount = FDEFDWN;
	IniFile.featherRatio = (float)FDEFRAT;
	IniFile.featherMinStitchSize = FDEFFLR;
	IniFile.featherCount = FDEFNUM;
	IniFile.underlayStitchLen = DEFULEN;
	IniFile.underlaySpacing = DEFUSPAC;
	IniFile.formBoxSizePixels = DEFBPIX;
	IniFile.textureHeight = ITXHI;
	IniFile.textureWidth = ITXWID;
	IniFile.textureSpacing = (float)ITXSPAC;
}

void dumrk(double pntx, double pnty) {

	if (rstMap(GMRK))
		drwmrk(StitchWindowDC);
	ZoomMarkPoint.x = pntx;
	ZoomMarkPoint.y = pnty;
	setMap(INIT);
	setMap(GMRK);
	drwmrk(StitchWindowDC);
	setMap(WASMRK);
}

void gselrng() {

	unsigned ind;

	SelectedFormsRange.start = SelectedFormsRange.finish = *SelectedFormList;
	for (ind = 1; ind < SelectedFormCount; ind++) {

		if (SelectedFormList[ind] < SelectedFormsRange.start)
			SelectedFormsRange.start = SelectedFormList[ind];
		if (SelectedFormList[ind] > SelectedFormsRange.finish)
			SelectedFormsRange.finish = SelectedFormList[ind];
	}
}

double nuang(double ydif, double xdif) {

	double tang, rang;

	tang = atan2(ydif, xdif);
	rang = tang - OriginalAngle;
	if (fabs(rang) > PI) {

		if (rang > 0)
			rang = 2 * PI - rang;
		else
			rang += 2 * PI;
	}
	return rang;
}

void angdif(double p_ang) {

	if (p_ang > HighestAngle)
		HighestAngle = p_ang;
	else {

		if (p_ang < LowestAngle)
			LowestAngle = p_ang;
	}
}

void rotmrk() {

	unsigned	ind, cnt, cod;
	double		tang;

	if (chkMap(GMRK) && (chkMap(FORMSEL) || chkMap(GRPSEL))) {

		if (chkMap(FORMSEL)) {

			cod = ClosestFormToCursor << FRMSHFT;
			fvars(ClosestFormToCursor);
			LowestAngle = HighestAngle = 0;
			OriginalAngle = atan2(CurrentFormVertices[0].y - ZoomMarkPoint.y, CurrentFormVertices[0].x - ZoomMarkPoint.x);
			for (ind = 1; ind < SideCount; ind++)
				angdif(nuang(CurrentFormVertices[ind].y - ZoomMarkPoint.y, CurrentFormVertices[ind].x - ZoomMarkPoint.x));
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				if ((StitchBuffer[ind].attribute&FRMSK) == cod)
					angdif(nuang(StitchBuffer[ind].y - ZoomMarkPoint.y, StitchBuffer[ind].x - ZoomMarkPoint.x));
			}
		} else {

			rngadj();
			LowestAngle = HighestAngle = 0;
			OriginalAngle = atan2(StitchBuffer[GroupStartStitch].y - ZoomMarkPoint.y, StitchBuffer[GroupStartStitch].x - ZoomMarkPoint.x);
			for (ind = GroupStartStitch + 1; ind <= GroupEndStitch; ind++)
				angdif(nuang(StitchBuffer[ind].y - ZoomMarkPoint.y, StitchBuffer[ind].x - ZoomMarkPoint.x));
		}
		tang = HighestAngle - LowestAngle;
		cnt = 2 * PI / tang;
		IniFile.rotationAngle = 2 * PI / cnt;
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Rotation Angle: %.2f\n"
			"Segments: %d\n", IniFile.fillAngle * 180 / PI, cnt);
		shoMsg(MsgBuffer);
	} else
		shoseln(IDS_FSZ, IDS_SETROTM);
}

void segentr() {

	if (!RotationAngle)
		RotationAngle = PI / 180;
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_ENTROT], 2 * PI / RotationAngle);
	shoMsg(MsgBuffer);
	setMap(NUMIN);
	numWnd();
}

void rotseg() {

	RotationAngle = IniFile.rotationAngle;
	segentr();
	setMap(ENTRSEG);
}

void pntmrk() {

	if (chkMap(SELBOX)) {

		dumrk(StitchBuffer[ClosestPointIndex].x, StitchBuffer[ClosestPointIndex].y);
		goto mrkdun;
	}
	if (chkMap(FRMPSEL)) {

		dumrk(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].x, FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].y);
		goto mrkdun;
	}
	shoseln(IDS_STCH_FRM, IDS_SETMRK);

mrkdun:;
}

void filfrms() {

	unsigned ine;

	if (SelectedFormCount) {

		savdo();
		for (ine = 0; ine < SelectedFormCount; ine++) {

			ClosestFormToCursor = SelectedFormList[ine];
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
	if (find < SelectedFormsRange.start) {

		for (ind = find; ind < SelectedFormsRange.finish; ind++)
			SelectedFormList[ine++] = ind;
		goto gotsrng;
	}
	if (find > SelectedFormsRange.finish) {

		for (ind = SelectedFormsRange.start; ind <= find; ind++)
			SelectedFormList[ine++] = ind;
		goto gotsrng;
	}
	for (ind = SelectedFormsRange.start; ind <= find; ind++)
		SelectedFormList[ine++] = ind;
gotsrng:;
	SelectedFormCount = ine;
}

void srchk() {

	rstMap(FORMSEL);
	SelectedFormCount = 0;
	if (setMap(LENSRCH)) {

		if (chkMap(WASGRP)) {

			ClosestPointIndex = GroupStartStitch = PrevGroupStartStitch;
			GroupStitchIndex = GroupEndStitch = PrevGroupEndStitch;
		} else
			rstMap(GRPSEL);
	} else {

		if (chkMap(GRPSEL)) {

			setMap(WASGRP);
			rngadj();
			PrevGroupStartStitch = GroupStartStitch;
			PrevGroupEndStitch = GroupEndStitch;
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
	cpos.x -= (StitchWindowOrigin.x + ici.xHotspot);
	cpos.y -= (StitchWindowOrigin.y + ici.yHotspot);
	GetBitmapBits(ici.hbmMask, 256, (unsigned char*)&MarkedStitchMap);
	if (tcur == ArrowCursor) {

		for (ind = 0; ind < 32; ind++) {

			l_amsk = duswap(MarkedStitchMap[ind]);
			binv = duswap(MarkedStitchMap[ind + 32]);
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

			binv = duswap(MarkedStitchMap[ind + 32]);
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

		inf = (FormIndex >> 5) + 1;
		for (ine = 0; ine < inf; ine++)
			MarkedStitchMap[ine] = 0;
		for (ind = 0; (unsigned)ind < SelectedFormCount; ind++) {

			ClosestFormToCursor = SelectedFormList[ind];
			setr(ClosestFormToCursor);
			fvars(ClosestFormToCursor);
			f1del();
		}
		FormIndices = (unsigned*)BSequence;
		inf = 0; ind = 0;
		for (ine = 0; ine < FormIndex; ine++) {

			if (!chkr(ine)) {

				frmcpy(&FormList[inf], &FormList[ine]);
				FormIndices[ine] = (ine - ind) << 4;
				inf++;
			} else
				ind++;
		}
		FormIndex = inf;
		inf = 0;
		if (chkMap(DELTO)) {

			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				if (StitchBuffer[ind].attribute&ALTYPMSK) {

					cod = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
					if (!chkr(cod)) {

						StitchBuffer[inf].attribute = StitchBuffer[ind].attribute &= NFRMSK;
						StitchBuffer[inf].attribute |= FormIndices[cod];
						StitchBuffer[inf].x = StitchBuffer[ind].x;
						StitchBuffer[inf++].y = StitchBuffer[ind].y;
					}
				} else {

					StitchBuffer[inf].attribute = StitchBuffer[ind].attribute;
					StitchBuffer[inf].x = StitchBuffer[ind].x;
					StitchBuffer[inf++].y = StitchBuffer[ind].y;
				}
			}
			PCSHeader.stitchCount = inf;
		} else {

			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				if (!(StitchBuffer[ind].attribute&NOTFRM)) {

					cod = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
					if (chkr(cod))
						StitchBuffer[ind].attribute &= (NFRMSK&NTYPMSK);
				}
			}
		}
		SelectedFormCount = 0;
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

	if (isclp(ClosestFormToCursor)) {

		SelectedForm->fillSpacing = StitchSpacing;
		fsizpar();
	}
}

BOOL chkminus(unsigned cod) {

	if (cod == 189 || cod == 109) {

		if (PreferenceIndex == PCLPOF + 1)
			return 1;
		if (FormMenuChoice == LFRMSPAC&&isfclp())
			return 1;
		if (FormMenuChoice == LWLKIND)
			return 1;
	}
	return 0;
}

void retrac() {

	unsigned src, dst;

	if (chkMap(GRPSEL)) {

		savdo();
		rngadj();
		if (!GroupStartStitch)
			GroupStartStitch++;
		makspac(GroupEndStitch + 1, GroupEndStitch - GroupStartStitch);
		src = GroupEndStitch - 1;
		dst = GroupEndStitch + 1;
		while (src >= GroupStartStitch) {

			StitchBuffer[dst].attribute = StitchBuffer[src].attribute;
			StitchBuffer[dst].x = StitchBuffer[src].x;
			StitchBuffer[dst++].y = StitchBuffer[src--].y;
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
			CheckMenuItem(MainMenu, cods[ind].id, MF_CHECKED);
		else
			CheckMenuItem(MainMenu, cods[ind].id, MF_UNCHECKED);
	}
	GridPen = nuPen(GridPen, 1, col);
	IniFile.gridColor = col;
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

		CheckMenuItem(MainMenu, ID_ROTAUXON, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_UNCHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_ROTAUXON, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_ROTAUXOFF, MF_CHECKED);
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

		CheckMenuItem(MainMenu, ID_FRMX, MF_CHECKED);
		CheckMenuItem(MainMenu, ID_FRMBOX, MF_UNCHECKED);
	} else {

		CheckMenuItem(MainMenu, ID_FRMX, MF_UNCHECKED);
		CheckMenuItem(MainMenu, ID_FRMBOX, MF_CHECKED);
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
		add		eax, offset StitchBuffer
		fld		IniFile.gridSize
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
	fPOINTATTR *pnt = StitchBuffer + strt;

	for (unsigned i = 0; i < fin - strt; i++) {
		pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
		pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;

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
		fld		IniFile.gridSize
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
		strt->x = rintf(strt->x / IniFile.gridSize) * IniFile.gridSize;
		strt->y = rintf(strt->y / IniFile.gridSize) * IniFile.gridSize;

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
	if (SelectedFormCount) {

		savdo();
		for (ind = 0; ind < SelectedFormCount; ind++) {

			ClosestFormToCursor = SelectedFormList[ind];
			frmsnap(FormList[ClosestFormToCursor].vertices, FormList[ClosestFormToCursor].sides);
			frmout(ClosestFormToCursor);
			refil();
		}
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			savdo();
			frmsnap(FormList[ClosestFormToCursor].vertices, FormList[ClosestFormToCursor].sides);
			frmout(ClosestFormToCursor);
			refil();
			setMap(RESTCH);
		} else {

			if (chkMap(GRPSEL)) {

				savdo();
				rngadj();
				stchsnap(GroupStartStitch, GroupEndStitch + 1);
				setMap(RESTCH);
			} else
				shoseln(IDS_FGRPF, IDS_SNAP2GRD);
		}
	}
}

#if PESACT

void lodpes() {

	setMap(REDOLD);
	strcpy_s(WorkingFileName, "u:\\mrd\\t.pes");
	nuFil();
}

void savpes() {

	IniFile.auxFileType = AUXPES;
	strcpy_s(WorkingFileName, "u:\\mrd\\t1.thr");
	nunams();
	sav();
}
#endif

void ritlock(HWND hwndlg) {

	unsigned ind;
	WIN32_FIND_DATA*	pdat;

	pdat = (WIN32_FIND_DATA*)&BSequence;
	SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_RESETCONTENT, 0, 0);
	SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_RESETCONTENT, 0, 0);
	for (ind = 0; ind < ActivePointIndex; ind++) {

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

	pdat = (WIN32_FIND_DATA*)&BSequence;
	switch (umsg) {

	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		strcpy_s(snam, DefaultDirectory);
		strcat_s(snam, "\\*.thr");
		srch = FindFirstFile(snam, &pdat[0]);
		if (srch == INVALID_HANDLE_VALUE) {

			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Directory: %s has no .thr files\n", DefaultDirectory);
			shoMsg(MsgBuffer);
			EndDialog(hwndlg, wparam);
			return TRUE;
		}
		ActivePointIndex = 1;
		while (FindNextFile(srch, &pdat[ActivePointIndex++]));
		ActivePointIndex--;
		ritlock(hwndlg);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {

		case IDCANCEL:

			EndDialog(hwndlg, wparam);
			return TRUE;

		case IDC_LOCKAL:

			for (ind = 0; ind < ActivePointIndex; ind++)
				pdat[ind].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
			ritlock(hwndlg);
			break;

		case IDC_UNLOCKAL:

			for (ind = 0; ind < ActivePointIndex; ind++)
				pdat[ind].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
			ritlock(hwndlg);
			break;

		case IDC_LOCK:

			ine = 0;
			hunlok = GetDlgItem(hwndlg, IDC_UNLOCKED);
			for (ind = 0; ind < ActivePointIndex; ind++) {

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
			for (ind = 0; ind < ActivePointIndex; ind++) {

				if ((pdat[ind].dwFileAttributes&FILE_ATTRIBUTE_READONLY)) {

					if (SendMessage(hlok, LB_GETSEL, ine, 0))
						pdat[ind].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
					ine++;
				}
			}
			ritlock(hwndlg);
			break;

		case IDOK:

			strcpy_s(snam, DefaultDirectory);
			strcat_s(snam, "\\");
			ine = 0;
			for (ind = 0; ind < ActivePointIndex; ind++) {

				strcpy_s(tnam, snam);
				strcat_s(tnam, pdat[ind].cFileName);
				if (!SetFileAttributes(tnam, pdat[ind].dwFileAttributes))
					ine++;
			}
			if (ine) {

				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d files could not be locked or unlocked\n", ine);
				shoMsg(MsgBuffer);
			}
			EndDialog(hwndlg, wparam);
			return TRUE;
			break;
		}
	}
	return 0;
}

void lock() {

	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DLOCK), ThrEdWindow, (DLGPROC)LockPrc);
}

unsigned colsum(COLORREF col) {

	unsigned colsum;
	unsigned ind;

	trcols(col);
	colsum = 0;
	for (ind = 0; ind < 3; ind++) {

		if (chkMap(TraceRGBFlag[ind]))
			colsum += PixelColors[ind];
	}
	return colsum;
}

unsigned icolsum(COLORREF col) {

	unsigned colsum;
	unsigned ind;

	trcols(col);
	colsum = 0;
	for (ind = 0; ind < 3; ind++) {

		if (chkMap(TraceRGBFlag[ind]))
			colsum += 255 - PixelColors[ind];
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
		mov		ebx, offset OSequence
		bts[ebx], eax
	}
#else
	_bittestandset((long *)OSequence, p_bpnt);
#endif
}

BOOL getrac(unsigned p_bpnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, p_bpnt
		mov		ebx, offset OSequence
		bt[ebx], ecx
		jnc		short getracx
		inc		eax
		getracx :
	}
#else
	return _bittest((long *)OSequence, p_bpnt);
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

		DeleteObject(TraceBitmap);
		ReleaseDC(ThrEdWindow, TraceDC);
		rstMap(WASEDG);
		for (ind = 0; ind < 16; ind++) {

			shownd(DefaultColorWin[ind]);
			shownd(UserColorWin[ind]);
			shownd(ThreadSizeWin[ind]);
		}
		for (ind = 0; ind < 3; ind++) {

			hidwnd(TraceControlWindow[ind]);
			hidwnd(TraceSelectWindow[ind]);
			hidwnd(TraceUpWindow[ind]);
			hidwnd(TraceDownWindow[ind]);
		}
		for (ind = 0; ind < 9; ind++)
			shownd(ButtonWin[ind]);
		hidwnd(TraceStepWin);
	} else {

		if (chkMap(TRCUP))
			DownPixelColor = 0xffffff;
		else
			UpPixelColor = 0;
	}
}

void trcstpnum() {

	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "len: %.2f", IniFile.traceLength / PFGRAN);
	SetWindowText(TraceStepWin, MsgBuffer);
}

void trcratnum() {

	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "pnts: %.2f", -log10(IniFile.traceRatio - 1));
	butxt(HLIN, MsgBuffer);
}

void clrhbut(unsigned strt)
{
	unsigned ind;

	for (ind = strt; ind < 9; ind++)
		SetWindowText(ButtonWin[ind], "");
}

void tracwnd() {

	unsigned ind;

	for (ind = 0; ind < 16; ind++) {

		hidwnd(DefaultColorWin[ind]);
		hidwnd(UserColorWin[ind]);
		hidwnd(ThreadSizeWin[ind]);
	}
	for (ind = 0; ind < 3; ind++) {

		shownd(TraceControlWindow[ind]);
		shownd(TraceSelectWindow[ind]);
		shownd(TraceUpWindow[ind]);
		shownd(TraceDownWindow[ind]);
	}
	hidwnd(ButtonWin[HBOXSEL]);
	hidwnd(ButtonWin[HUPTO]);
	shownd(TraceStepWin);
	trcstpnum();
	trcratnum();
	clrhbut(4);
}

void trcols(COLORREF col) {

#if  __UseASM__
	_asm {

		mov		eax, col
		mov		ecx, offset PixelColors
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
	PixelColors[0] = col & 0xff;
	PixelColors[1] = (col & 0xff00) >> 8;
	PixelColors[2] = (col & 0xff0000) >> 16;
#endif
}

BOOL trcin(COLORREF col) {

	if (col) {

		trcols(col);
		if (chkMap(TRCRED)) {

			if (PixelColors[0] > HighColors[0])
				return 0;
			if (PixelColors[0] < LowColors[0])
				return 0;
		}
		if (chkMap(TRCGRN)) {

			if (PixelColors[1] > HighColors[1])
				return 0;
			if (PixelColors[1] < LowColors[1])
				return 0;
		}
		if (chkMap(TRCBLU)) {

			if (PixelColors[2] > HighColors[2])
				return 0;
			if (PixelColors[2] < LowColors[2])
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
	l_binfh.biWidth = BitmapWidth;
	l_binfh.biHeight = BitmapHeight;
	l_binfh.biPlanes = 1;
	l_binfh.biBitCount = 32;
	l_binfh.biCompression = BI_RGB;
	l_binf.bmiHeader = l_binfh;
	TraceBitmap = CreateDIBSection(BitmapDC, &l_binf, DIB_RGB_COLORS, (void**)&TraceBitmapData, 0, 0);
	TraceDC = CreateCompatibleDC(StitchWindowDC);
	if (TraceBitmap && TraceDC) {
		SelectObject(TraceDC, TraceBitmap);
		BitBlt(TraceDC, 0, 0, BitmapWidth, BitmapHeight, BitmapDC, 0, 0, SRCCOPY);
		setMap(WASTRAC);
		TracedPixels = (unsigned*)OSequence;
		TraceDataSize = ((BitmapWidth*BitmapHeight) >> 5) + 1;
		for (ind = 0; ind < TraceDataSize; ind++)
			TracedPixels[ind] = 0;
		StretchBlt(StitchWindowMemDC, BitmapDstRect.left, BitmapDstRect.top, BitmapDstRect.right - BitmapDstRect.left, BitmapDstRect.bottom - BitmapDstRect.top,
			BitmapDC, BitmapSrcRect.left, BitmapSrcRect.top, BitmapSrcRect.right - BitmapSrcRect.left, BitmapSrcRect.bottom - BitmapSrcRect.top, SRCCOPY);
	}
}

void trace() {

	unsigned			ind;
	unsigned			traceColorMask;	

#if TRCMTH==0

	unsigned			usum, psum, dsum;
#endif

	if (*PCSBMPFileName) {

		untrace();
		tracwnd();
		getrmap();
		if (px2stch() && !rstMap(WASTRCOL)) {

			if (chkMap(LANDSCAP))
				SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
			BitmapPoint.x = BmpStitchRatio.x*SelectedPoint.x;
			BitmapPoint.y = BmpStitchRatio.y*SelectedPoint.y - 1;
			ind = TraceBitmapData[BitmapPoint.y*BitmapWidth + BitmapPoint.x] ^ 0xffffff;
			if (chkMap(TRCUP)) {

				UpPixelColor = ind;
				DownPixelColor = 0xffffff;
			} else {

				DownPixelColor = ind;
				UpPixelColor = 0;
			}
			setMap(TRCRED);
			setMap(TRCGRN);
			setMap(TRCBLU);
		}
		traceColorMask = 0xffffff;
		if (!chkMap(TRCRED))
			traceColorMask &= REDMSK;
		if (!chkMap(TRCGRN))
			traceColorMask &= GRNMSK;
		if (!chkMap(TRCBLU))
			traceColorMask &= BLUMSK;
		if (traceColorMask != 0xffffff) {

			for (ind = 0; ind < BitmapWidth*BitmapHeight; ind++)
				TraceBitmapData[ind] &= traceColorMask;
		}

#if TRCMTH==0

		usum = icolsum(UpPixelColor);
		dsum = icolsum(DownPixelColor);
		for (index = 0; index < BitmapWidth*BitmapHeight; index++) {

			psum = colsum(TraceBitmapData[index]);
			if (usum > psum&&dsum < psum)
				setrac(index);
			else
				TraceBitmapData[index] = 0;
		}
#endif

#if TRCMTH==1
		InvertUpColor = UpPixelColor ^ 0xffffff;
		InvertDownColor = DownPixelColor ^ 0xffffff;
		trcols(InvertUpColor);
		for (ind = 0; ind < 3; ind++)
			HighColors[ind] = PixelColors[ind];
		trcols(InvertDownColor);
		for (ind = 0; ind < 3; ind++)
			LowColors[ind] = PixelColors[ind];
		for (ind = 0; ind < BitmapWidth*BitmapHeight; ind++) {

			if (trcin(TraceBitmapData[ind]))
				setrac(ind);
			else
				TraceBitmapData[ind] = 0;
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
		mov		ebx, TracedEdges
		bts[ebx], eax
	}
#else
	_bittestandset((long *)TracedEdges, p_bpnt);
#endif
}

BOOL chkedg(unsigned p_bpnt) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, p_bpnt
		mov		ebx, TracedEdges
		bt[ebx], ecx
		jnc		short chkedgx
		inc		eax
		chkedgx :
	}
#else
	return _bittest((long *)TracedEdges, p_bpnt);
#endif
}

void tracedg() {

	unsigned	ind, ine, flg;
	long		l_bpnt;

	if (!chkMap(WASTRAC))
		trace();
	TracedEdges = (unsigned*)&OSequence[TraceDataSize];
	for (ind = 0; ind < TraceDataSize; ind++)
		TracedEdges[ind] = 0;
	l_bpnt = 0;
	for (ind = 0; ind < BitmapHeight; ind++) {

		flg = 0;
		for (ine = 0; ine < BitmapWidth; ine++) {

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
	for (ind = 0; ind < BitmapWidth; ind++) {

		l_bpnt = ind;
		flg = 0;
		for (ine = 0; ine < BitmapHeight; ine++) {

			if (getrac(l_bpnt)) {

				if (!flg) {

					setedg(l_bpnt);
					flg = 1;
				}
			} else {

				if (flg) {

					setedg(l_bpnt - BitmapWidth);
					flg = 0;
				}
			}
			l_bpnt += BitmapWidth;
		}
		if (flg)
			setedg(l_bpnt - BitmapWidth);
	}
	for (ind = 0; ind < BitmapWidth*BitmapHeight; ind++) {

		if (chkedg(ind))
			TraceBitmapData[ind] = 0xffffff;
		else
			TraceBitmapData[ind] = 0;
	}
	setMap(RESTCH);
	setMap(WASEDG);
}

BOOL trcbit() {
	unsigned l_bpnt;

	l_bpnt = CurrentTracePoint.y*BitmapWidth + CurrentTracePoint.x;
	switch (TraceDirection) {

	case TRCR:

		l_bpnt += (1 - BitmapWidth);
		if (CurrentTracePoint.x == (int)BitmapWidth - 1)
			TraceDirection = TRCU;
		else {

			if (chkedg(l_bpnt)) {

				CurrentTracePoint.x++;
				CurrentTracePoint.y--;
				TraceDirection = TRCD;
			} else {

				l_bpnt += BitmapWidth;
				if (chkedg(l_bpnt))
					CurrentTracePoint.x++;
				else
					TraceDirection = TRCU;
			}
		}
		break;

	case TRCD:

		l_bpnt -= (BitmapWidth + 1);
		if (!CurrentTracePoint.y)
			TraceDirection = TRCR;
		else {

			if (chkedg(l_bpnt)) {

				CurrentTracePoint.x--;
				CurrentTracePoint.y--;
				TraceDirection = TRCL;
			} else {

				l_bpnt++;
				if (chkedg(l_bpnt))
					CurrentTracePoint.y--;
				else
					TraceDirection = TRCR;
			}
		}
		break;

	case TRCL:

		l_bpnt += (BitmapWidth - 1);
		if (!CurrentTracePoint.x)
			TraceDirection = TRCD;
		else {

			if (chkedg(l_bpnt)) {

				CurrentTracePoint.x--;
				CurrentTracePoint.y++;
				TraceDirection = TRCU;
			} else {

				l_bpnt -= BitmapWidth;
				if (chkedg(l_bpnt))
					CurrentTracePoint.x--;
				else
					TraceDirection = TRCD;
			}
		}
		break;

	case TRCU:

		l_bpnt += (1 + BitmapWidth);
		if (CurrentTracePoint.y == (int)BitmapHeight - 1)
			TraceDirection = TRCL;
		else {

			if (chkedg(l_bpnt)) {

				CurrentTracePoint.x++;
				CurrentTracePoint.y++;
				TraceDirection = TRCR;
			} else {

				l_bpnt--;
				if (chkedg(l_bpnt))
					CurrentTracePoint.y++;
				else
					TraceDirection = TRCL;
			}
		}
		break;
	}
	if (tracedPoints[ActivePointIndex - 1].x != CurrentTracePoint.x || tracedPoints[ActivePointIndex - 1].y != CurrentTracePoint.y) {

		tracedPoints[ActivePointIndex].x = CurrentTracePoint.x;
		tracedPoints[ActivePointIndex++].y = CurrentTracePoint.y;
		if (ActivePointIndex >= 500000)
			return 0;
	}
	if (TraceDirection == InitialDirection&&CurrentTracePoint.x == tracedPoints[0].x&&CurrentTracePoint.y == tracedPoints[0].y)
		return 0;
	else
		return 1;
}

void dutdif(TRCPNT* pnt) {

	TraceDiff0.x = pnt[1].x - pnt[0].x;
	// ToDo this is likely incorrect
	TraceDiff0.y = pnt[1].x - pnt[0].y;
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
			SelectedPoint.y -= (UnzoomedRect.y - BitmapSizeinStitches.y);
		CurrentTracePoint.x = BmpStitchRatio.x*SelectedPoint.x;
		CurrentTracePoint.y = BmpStitchRatio.y*SelectedPoint.y;
		if (CurrentTracePoint.x > (long)BitmapWidth)
			CurrentTracePoint.x = BitmapWidth;
		if (CurrentTracePoint.y > (long)BitmapHeight)
			CurrentTracePoint.y = BitmapHeight;
		bakpnt = l_bpnt = CurrentTracePoint.y*BitmapWidth + CurrentTracePoint.x;
		if (!chkedg(l_bpnt)) {

			flg = 20;
			l_bpnt = bakpnt;
			blim = (CurrentTracePoint.y + 1)*BitmapWidth;
			while (l_bpnt < blim && !chkedg(l_bpnt))
				l_bpnt++;
			if (l_bpnt < blim)
				fndrct.right = l_bpnt - CurrentTracePoint.y*BitmapWidth;
			else
				fndrct.right = BitmapWidth;
			l_bpnt = bakpnt;
			blim = CurrentTracePoint.y*BitmapWidth;
			while (l_bpnt > blim && !chkedg(l_bpnt))
				l_bpnt--;
			if (l_bpnt == blim)
				fndrct.left = 0;
			else
				fndrct.left = l_bpnt - blim;
			l_bpnt = bakpnt;
			while (l_bpnt > 0 && !chkedg(l_bpnt))
				l_bpnt -= BitmapWidth;
			if (l_bpnt > 0)
				fndrct.bottom = l_bpnt / BitmapWidth;
			else
				fndrct.bottom = 0;
			l_bpnt = bakpnt;
			blim = BitmapWidth*BitmapHeight;
			while (l_bpnt < blim && !chkedg(l_bpnt))
				l_bpnt += BitmapWidth;
			if (l_bpnt < blim)
				fndrct.top = l_bpnt / BitmapWidth;
			else
				fndrct.top = BitmapHeight;
			flg = 0;
			mdis = 0x7fffffff;
			if (fndrct.left) {

				mdis = CurrentTracePoint.x - fndrct.left;
				flg = TRCL;
			}
			if (fndrct.right < (long)BitmapWidth) {

				dis = fndrct.right - CurrentTracePoint.x;
				if (dis < mdis) {

					mdis = dis;
					flg = TRCR;
				}
			}
			if (fndrct.bottom) {

				dis = CurrentTracePoint.y - fndrct.bottom;
				if (dis < mdis) {

					mdis = dis;
					flg = TRCD;
				}
			}
			if (fndrct.top < (long)BitmapHeight) {

				dis = fndrct.top - CurrentTracePoint.y;
				if (dis < mdis) {

					mdis = dis;
					flg = TRCU;
				}
			}
			switch (flg) {

			case TRCU:

				CurrentTracePoint.y = fndrct.top;
				TraceDirection = TRCR;
				break;

			case TRCR:

				CurrentTracePoint.x = fndrct.right;
				TraceDirection = TRCD;
				break;

			case TRCD:

				CurrentTracePoint.y = fndrct.bottom;
				TraceDirection = TRCL;
				break;

			case TRCL:

				CurrentTracePoint.x = fndrct.left;
				TraceDirection = TRCU;
				break;

			default:
				return;
			}
		}
		InitialDirection = TraceDirection;
		l_bpnt = CurrentTracePoint.y*BitmapWidth + CurrentTracePoint.x;
		ActivePointIndex = 1;
		tracedPoints = (TRCPNT*)BSequence;
		tracedPoints[0].x = CurrentTracePoint.x;
		tracedPoints[0].y = CurrentTracePoint.y;
		while (trcbit());
		if (ActivePointIndex >= 500000) {

			tabmsg(IDS_FRM2L);
			return;
		}
		DecimatedLine = &tracedPoints[ActivePointIndex];
		DecimatedLine[0].x = tracedPoints[0].x;
		DecimatedLine[0].y = tracedPoints[0].y;
		dutdif(&tracedPoints[0]);
		OutputIndex = 1;
		for (ind = 1; ind < ActivePointIndex; ind++) {

			TraceDiff1.x = TraceDiff0.x;
			TraceDiff1.y = TraceDiff0.y;
			dutdif(&tracedPoints[ind]);
			if (TraceDiff1.x != TraceDiff0.x || TraceDiff1.y != TraceDiff0.y) {

				DecimatedLine[OutputIndex].x = tracedPoints[ind].x;
				DecimatedLine[OutputIndex++].y = tracedPoints[ind].y;
			}
		}
		tracedPoints[0].x = DecimatedLine[0].x;
		tracedPoints[0].y = DecimatedLine[0].y;
		ine = 0;
		ActivePointIndex = 0;
		for (ind = 1; ind < OutputIndex; ind++) {

			tlen = hypot(DecimatedLine[ind].x - DecimatedLine[ine].x, DecimatedLine[ind].y - DecimatedLine[ine].y);
			if (tlen > IniFile.traceLength) {

				tracedPoints[ActivePointIndex].x = DecimatedLine[ine].x;
				tracedPoints[ActivePointIndex].y = DecimatedLine[ine].y;
				ine = ind;
				ActivePointIndex++;
			}
		}
		for (ind = ine + 1; ind < OutputIndex; ind++) {

			tracedPoints[ActivePointIndex].x = DecimatedLine[ind].x;
			tracedPoints[ActivePointIndex].y = DecimatedLine[ind].y;
			ActivePointIndex++;
		}
		SelectedForm = &FormList[FormIndex];
		frmclr(SelectedForm);
		CurrentFormVertices = &FormVertices[FormVertexIndex];
		CurrentFormVertices[0].x = tracedPoints[0].x*StitchBmpRatio.x;
		CurrentFormVertices[0].y = tracedPoints[0].y*StitchBmpRatio.y;
		ine = 0;
		OutputIndex = 0;
		tlens = 0;
		if (chkMap(LANDSCAP))
			ratof = UnzoomedRect.y - BitmapSizeinStitches.y;
		else
			ratof = 0;
		for (ind = 1; ind < ActivePointIndex; ind++) {

			tlens += hypot(tracedPoints[ind].x - tracedPoints[ind - 1].x, tracedPoints[ind].y - tracedPoints[ind - 1].y);
			tlen = hypot(tracedPoints[ind].x - tracedPoints[ine].x, tracedPoints[ind].y - tracedPoints[ine].y);
			if (tlens > tlen*IniFile.traceRatio) {

				CurrentFormVertices[OutputIndex].x = tracedPoints[ind - 1].x*StitchBmpRatio.x;
				CurrentFormVertices[OutputIndex].y = tracedPoints[ind - 1].y*StitchBmpRatio.y + ratof;
				OutputIndex++;
				ind--;
				ine = ind;
				tlens = 0;
			}
		}
		if (FormVertexIndex + OutputIndex > MAXFLT) {

			tabmsg(IDS_FRMOVR);
			return;
		}
		SelectedForm->vertices = adflt(OutputIndex);
		SelectedForm->sides = OutputIndex;
		SelectedForm->type = FRMFPOLY;
		SelectedForm->attribute = ActiveLayer << 1;
		frmout(FormIndex);
		FormList[FormIndex].satinGuideCount = 0;
		FormIndex++;
		setMap(RESTCH);
		setMap(FRMOF);
		tglfrm();
	}
}

unsigned ducolm() {

	if (TraceMsgPoint.x < (int)ButtonWidth)
		return 0;
	if (TraceMsgPoint.x < (int)ButtonWidth << 1)
		return 1;
	else
		return 2;
}

void chkref() {

#if  __UseASM__
	_asm {
		mov		eax, UpPixelColor
		mov		ebx, DownPixelColor
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
									  mov		UpPixelColor, eax
									  mov		DownPixelColor, ebx
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

	u.cr = UpPixelColor;
	d.cr = DownPixelColor;

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

	UpPixelColor = u.cr;
	DownPixelColor = d.cr;
#endif
}

void trnumwnd0(int pos) {

	TraceNumberInput = CreateWindow(
		"STATIC",
		0,
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		pos,
		ButtonWidth,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void trnumwnd1(int pos) {

	GeneralNumberInputBox = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		pos,
		ButtonWidthX3,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void tracpar() {

	unsigned	pos;
	double		rat;
	COLORREF	tcol;
	COLORREF	tpos;

	if (chkMap(TRNIN0))
		dutrnum0(atoi(TraceInputBuffer));
	if (chkMap(TRNIN1))
		dutrnum1();
	TraceMsgPoint.x = Msg.pt.x - ThredWindowOrigin.x;
	TraceMsgPoint.y = Msg.pt.y - ThredWindowOrigin.y;
	if (TraceMsgPoint.x > (int)ButtonWidthX3)
		dutrac();
	else {

		ColumnColor = ducolm();
		if (TraceMsgPoint.y < (int)ButtonHeight * 15) {

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				UpPixelColor &= TraceRGBMask[ColumnColor];
				DownPixelColor |= TraceRGB[2 - ColumnColor];
				goto tracpar1;
			}
			rat = (float)TraceMsgPoint.y / (ButtonHeight * 15);
			pos = rat * 255;
			tcol = UpPixelColor&TraceRGB[2 - ColumnColor];
			tpos = pos << TraceShift[ColumnColor];
			if (tpos < tcol) {

				UpPixelColor &= TraceRGBMask[ColumnColor];
				UpPixelColor |= tpos;
				goto tracpar1;
			}
			tcol = DownPixelColor&TraceRGB[2 - ColumnColor];
			if (tpos > tcol) {

				DownPixelColor &= TraceRGBMask[ColumnColor];
				DownPixelColor |= tpos;
				goto tracpar1;
			}
			if (Msg.message == WM_LBUTTONDOWN) {

				UpPixelColor &= TraceRGBMask[ColumnColor];
				UpPixelColor |= pos << TraceShift[ColumnColor];
			} else {

				DownPixelColor &= TraceRGBMask[ColumnColor];
				DownPixelColor |= pos << TraceShift[ColumnColor];
			}
		tracpar1:
			redraw(TraceControlWindow[ColumnColor]);
			trace();
		} else {

			pos = floor(TraceMsgPoint.y / ButtonHeight);
			if (pos < 16) {

				toglMap(TraceRGBFlag[ColumnColor]);
				redraw(TraceSelectWindow[ColumnColor]);
				trace();
			} else {

				if (pos < 18) {

					setMap(NUMIN);
					setMap(TRNIN0);
					MsgIndex = 0;
					*TraceInputBuffer = 0;
					if (pos < 17) {

						trnumwnd0(ButtonHeight * 16);
						setMap(TRNUP);
					} else {

						trnumwnd0(ButtonHeight * 17);
						rstMap(TRNUP);
					}
				} else {

					if (pos < 20) {

						setMap(NUMIN);
						setMap(TRNIN1);
						MsgIndex = 0;
						*TraceInputBuffer = 0;
						if (pos < 19) {

							trnumwnd1(ButtonHeight * 18);
							setMap(TRNUP);
						} else {

							trnumwnd1(ButtonHeight * 19);
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
					mov		edi, offset TraceAdjacentColors
					mov		ebx, 0xff
					mov		edx, BitmapWidth
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
	unsigned *dst = TraceAdjacentColors;

	difsub(l_bpnt, shft, dst);

	l_bpnt -= BitmapWidth;
	difsub(l_bpnt, shft, dst);

	l_bpnt -= 1;
	difsub(l_bpnt, shft, dst);

	l_bpnt += 2;
	difsub(l_bpnt, shft, dst);

	l_bpnt += BitmapWidth;
	difsub(l_bpnt, shft, dst);

	l_bpnt -= 2;
	difsub(l_bpnt, shft, dst);

	l_bpnt += BitmapWidth;
	difsub(l_bpnt, shft, dst);

	l_bpnt += 1;
	difsub(l_bpnt, shft, dst);

	l_bpnt += 1;
	difsub(l_bpnt, shft, dst);
#endif
}

void blanklin(unsigned bind) {

	unsigned ind;

	for (ind = bind; ind < bind + BitmapWidth; ind++)
		DifferenceBitmap[ind] = 0;
}

unsigned trsum() {

	unsigned rsum, ind;

	rsum = 0;
	for (ind = 1; ind < 9; ind++)
		rsum += ((TraceAdjacentColors[ind] > TraceAdjacentColors[0]) ? (TraceAdjacentColors[ind] - TraceAdjacentColors[0]) : (TraceAdjacentColors[0] - TraceAdjacentColors[ind]));
	return rsum;
}

void trdif() {

	unsigned	ind, ine, inf, l_bpnt = 0, rsum, rmax, rmin;
	double		rat;

	if (!*PCSBMPFileName) {

		tabmsg(IDS_MAPLOD);
		return;
	}
	rstMap(TRSET);
	rstMap(HIDMAP);
	untrace();
	DifferenceBitmap = (unsigned*)BSequence;
	rmax = 0;
	rmin = 0xffffffff;
	if (!chkMap(WASTRAC))
		getrmap();
	ind = 0;
	for (ind = 0; ind < 3; ind++) {

		blanklin(0);
		for (ine = 1; ine < (unsigned)BitmapHeight - 1; ine++) {

			l_bpnt = ine*BitmapWidth;
			DifferenceBitmap[l_bpnt++] = 0;
			for (inf = 1; inf < BitmapWidth - 1; inf++) {

				difbits(TraceShift[ind], &TraceBitmapData[l_bpnt]);
				rsum = DifferenceBitmap[l_bpnt] = trsum();
				l_bpnt++;
				if (rsum > rmax)
					rmax = rsum;
				if (rsum < rmin)
					rmin = rsum;
			}
			DifferenceBitmap[l_bpnt++] = 0;
		}
		blanklin(l_bpnt);
		rat = (double)255 / (rmax - rmin);
		for (ine = 0; ine < BitmapWidth*BitmapHeight; ine++) {

			TraceBitmapData[ine] &= TraceRGBMask[ind];
			if (DifferenceBitmap[ine]) {

				inf = (DifferenceBitmap[ine] - rmin)*rat;
				TraceBitmapData[ine] |= inf << TraceShift[ind];
			}
		}
	}
	BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
	setMap(WASDIF);
	setMap(RESTCH);
	tracwnd();
}

void delstch() {

	savdo();
	PCSHeader.stitchCount = 0;
	ClipPointIndex = 0;
	TextureIndex = 0;
	rstAll();
	clrfills();
	ColorChanges = 0;
	butxt(HNUM, "");
	butxt(HTOT, "");
	setMap(RESTCH);
}

void chkbit() {

	if (*PCSBMPFileName && (chkMap(WASDIF) || chkMap(WASDSEL) || chkMap(WASBLAK))) {

		setMap(WASESC);
		bfil();
	}
}

void trcnum(unsigned shft, COLORREF col, unsigned ind) {

	unsigned	len;
	unsigned	nwid;
	TCHAR		trbuf[11] = { 0 };

	col >>= shft;
	col &= 0xff;
	_itoa_s(col, trbuf, 10);
	len = strlen(trbuf);
	nwid = NumeralWidth*(3 - len) + 1;
	SetBkColor(DrawItem->hDC, TraceRGB[ind]);
	TextOut(DrawItem->hDC, nwid, 1, trbuf, strlen(trbuf));
}

void upnum(unsigned ind) {

	trcnum(TraceShift[ind], InvertUpColor, ind);
}

void dwnum(unsigned ind) {

	trcnum(TraceShift[ind], InvertDownColor, ind);
}

void ritrcol(COLORREF* tref, unsigned num) {

	*tref &= TraceRGBMask[ColumnColor];
	num &= 0xff;
	*tref |= (num << TraceShift[ColumnColor]);
}

void dutrnum0(unsigned num) {

	rstMap(NUMIN);
	rstMap(TRNIN0);
	if (chkMap(TRNUP)) {

		ritrcol(&InvertUpColor, num);
		UpPixelColor = InvertUpColor ^ 0xffffff;
		redraw(TraceUpWindow[ColumnColor]);
	} else {

		ritrcol(&InvertDownColor, num);
		DownPixelColor = InvertDownColor ^ 0xffffff;
		redraw(TraceDownWindow[ColumnColor]);
	}
	redraw(TraceControlWindow[ColumnColor]);
	DestroyWindow(TraceNumberInput);
	setMap(WASTRCOL);
	trace();
}

void dutrnum1() {

	double	tdub;

	DestroyWindow(GeneralNumberInputBox);
	rstMap(NUMIN);
	rstMap(TRNIN1);
	tdub = atof(MsgBuffer);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", tdub);
	if (tdub > 9)
		tdub = 9;
	if (chkMap(TRNUP)) {

		IniFile.traceLength = tdub*PFGRAN;
		trcstpnum();
	} else {

		IniFile.traceRatio = 1 + pow(0.1, tdub);
		trcratnum();
	}
}

void trcsel() {

	unsigned ind, max, xind;

	if (*PCSBMPFileName) {

		setMap(WASTRCOL);
		setMap(TRCRED);
		setMap(TRCBLU);
		setMap(TRCGRN);
		DownPixelColor = 0xffffff;
		UpPixelColor = 0;
		trace();
		rstMap(HIDMAP);
		rstMap(TRSET);
		for (ind = 0; ind < BitmapWidth*BitmapHeight; ind++) {

			trcols(TraceBitmapData[ind]);
			max = PixelColors[0];
			xind = 2;
			if (PixelColors[1] > max) {

				max = PixelColors[1];
				xind = 1;
			}
			if (PixelColors[2] > max)
				xind = 0;
			TraceBitmapData[ind] &= TraceRGB[xind];
		}
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
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

	if (*PCSBMPFileName) {

		if (!chkMap(TRSET)) {

			setMap(TRCRED);
			setMap(TRCGRN);
			setMap(TRCBLU);
			if (!chkMap(WASTRAC))
				getrmap();
			if (chkMap(MONOMAP)) {

				tcol = TraceBitmapData[0];
				for (ind = 0; ind < BitmapWidth*BitmapHeight; ind++) {

					if (TraceBitmapData[ind] != tcol)
						break;
				}
				if (ind < BitmapWidth*BitmapHeight) {

					trcols(TraceBitmapData[ind]);
					HighColors[0] = PixelColors[0];
					HighColors[1] = PixelColors[1];
					HighColors[2] = PixelColors[2];
					trcols(tcol);
					for (ind = 0; ind < 3; ind++) {

						if (PixelColors[ind] > HighColors[ind]) {

							ine = PixelColors[ind];
							PixelColors[ind] = HighColors[ind];
							HighColors[ind] = ine;
						}
						col[ind] = ((HighColors[ind] - PixelColors[ind]) >> 1) + PixelColors[ind];
					}
				}
			} else {

				phst = (unsigned*)hst;
				for (ind = 0; ind < 768; ind++)
					phst[ind] = 0;
				for (ind = 0; ind < BitmapWidth*BitmapHeight; ind++) {

					trcols(TraceBitmapData[ind]);
					for (ine = 0; ine < 3; ine++)
						hst[ine][PixelColors[ine]]++;
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
			InvertDownColor = 0;
			for (ind = 0; ind < 3; ind++) {

				if (col[ind])
					col[ind]--;
				InvertDownColor |= col[ind] << TraceShift[ind];
			}
			DownPixelColor = InvertDownColor ^ 0xffffff;
			InvertUpColor = 0xffffff;
			UpPixelColor = 0;
		}
		setMap(WASTRCOL);
		trace();
	} else
		tabmsg(IDS_MAPLOD);
}

void stch2bit(double px, double py) {

	if (chkMap(LANDSCAP))
		py -= (UnzoomedRect.y - BitmapSizeinStitches.y);
	BitmapPoint.x = BmpStitchRatio.x*px;
	BitmapPoint.y = (BitmapHeight - BmpStitchRatio.y*py);
}

void pxlin(unsigned strt, unsigned fin) {

	POINT		lin[2];

	stch2bit(CurrentFormVertices[strt].x, CurrentFormVertices[strt].y);
	lin[0].x = BitmapPoint.x;
	lin[0].y = BitmapPoint.y;
	stch2bit(CurrentFormVertices[fin].x, CurrentFormVertices[fin].y);
	lin[1].x = BitmapPoint.x;
	lin[1].y = BitmapPoint.y;
	Polyline(BitmapDC, lin, 2);
	Polyline(TraceDC, lin, 2);
}

void bfrm() {

	unsigned ind;

	if (SideCount) {

		for (ind = 0; ind < (unsigned)SideCount - 1; ind++)
			pxlin(ind, ind + 1);
		if (SelectedForm->type != FRMLINE)
			pxlin(ind, 0);
	}
}

void blak() {

	unsigned ind;

	if (!*PCSBMPFileName) {

		tabmsg(IDS_MAPLOD);
		return;
	}
	tracwnd();
	if (FormIndex) {

		BlackPen = CreatePen(PS_SOLID, 1, 0);
		SelectObject(BitmapDC, BlackPen);
		SelectObject(TraceDC, BlackPen);
		if (!chkMap(WASTRAC))
			getrmap();
		for (ind = 0; ind < FormIndex; ind++) {

			fvars(ind);
			bfrm();
		}
		DeleteObject(BlackPen);
		setMap(WASBLAK);
		setMap(RESTCH);
	} else
		tabmsg(IDS_FRMNO);
}

void delmap() {

	*PCSBMPFileName = 0;
	setMap(RESTCH);
}

void closfn() {

	deltot();
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRED], IniFile.designerName);
	KnotCount = 0;
	*WorkingFileName = 0;
	*PCSBMPFileName = 0;
	deldu();
	clrhbut(3);
	SetWindowText(ThrEdWindow, MsgBuffer);
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

	if (chkMap(BIGBOX) || SelectedFormCount || chkMap(FORMSEL) || chkMap(GRPSEL) || chkMap(SELBOX))
		savdo();
	if (chkMap(BIGBOX)) {

		for (ind = 0; ind < FormIndex; ind++) {

			SelectedForm = &FormList[ind];
			frmpos(dx, dy);
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

			StitchBuffer[ind].x += dx;
			StitchBuffer[ind].y += dy;
		}
		AllItemsRect.bottom += dy;
		AllItemsRect.top += dy;
		AllItemsRect.left += dx;
		AllItemsRect.right += dx;
		stchrct2px(AllItemsRect, &SelectedFormsRect);
		setMap(RESTCH);
		return;
	}
	if (SelectedFormCount) {

		rcnt = (FormIndex >> 5) + 1;
		clRmap(rcnt);
		for (ind = 0; ind < SelectedFormCount; ind++)
			setr(SelectedFormList[ind]);
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

			if (chkr((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {

				StitchBuffer[ind].x += dx;
				StitchBuffer[ind].y += dy;
			}
		}
		for (ind = 0; ind < SelectedFormCount; ind++) {

			SelectedForm = &FormList[SelectedFormList[ind]];
			frmpos(dx, dy);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL)) {

		SelectedForm = &FormList[ClosestFormToCursor];
		frmpos(dx, dy);
		if (SelectedForm->fillType || SelectedForm->edgeType) {

			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				if ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor) {

					StitchBuffer[ind].x += dx;
					StitchBuffer[ind].y += dy;
				}
			}
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++) {

			StitchBuffer[ind].x += dx;
			StitchBuffer[ind].y += dy;
		}
		grpAdj();
		setMap(RESTCH);
		return;
	}
	if (chkMap(SELBOX)) {

		StitchBuffer[ClosestPointIndex].x += dx;
		StitchBuffer[ClosestPointIndex].y += dy;
		setMap(RESTCH);
		return;
	}
	pix.x = pix.y = 0;
	if (dx) {

		if (dx > 0)
			pix.x = IniFile.nudgePixels;
		else
			pix.x = -IniFile.nudgePixels;
	}
	if (dy) {

		if (dy > 0)
			pix.y = -IniFile.nudgePixels;
		else
			pix.y = +IniFile.nudgePixels;
	}
	mouse_event(MOUSEEVENTF_MOVE, pix.x, pix.y, 0, 0);
}

void pixmsg(unsigned cod, unsigned pix) {

	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[cod], pix);
	shoMsg(MsgBuffer);
}

void getnpix() {

	pixmsg(STR_NUDG, IniFile.nudgePixels);
	setMap(NUMIN);
	setMap(PIXIN);
	numWnd();
}

void getstpix() {

	pixmsg(STR_STCHP, IniFile.stitchSizePixels);
	setMap(NUMIN);
	setMap(STPXIN);
	numWnd();
}

void getfrmpix() {

	pixmsg(STR_FRMP, IniFile.formVertexSizePixels);
	setMap(NUMIN);
	setMap(FRMPXIN);
	numWnd();
}

void getfrmbox() {

	pixmsg(STR_FRMBOX, IniFile.formBoxSizePixels);
	setMap(NUMIN);
	setMap(FRMBOXIN);
	numWnd();
}

void bakmrk() {

	fPOINT	tpnt;

	if (chkMap(WASMRK)) {

		if (ZoomMarkPoint.x > IniFile.hoopSizeX)
			ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
		if (ZoomMarkPoint.y > IniFile.hoopSizeY)
			ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
		dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
		tpnt.x = ZoomMarkPoint.x;
		tpnt.y = ZoomMarkPoint.y;
		shft(tpnt);
		setMap(RESTCH);
	} else
		tabmsg(IDS_MRK);
}

void nuscol(unsigned ind) {

	UserPen[ind] = nuPen(UserPen[ind], 1, UserColor[ind]);
	UserColorBrush[ind] = nuBrush(UserColorBrush[ind], UserColor[ind]);
	redraw(UserColorWin[ind]);
}

void movchk() {

	unsigned	ind, col, key, swtch;
	FRMHED*		pfrm;

	if (Msg.wParam&MK_LBUTTON) {

		if (!setMap(WASMOV)) {

			if (chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {

				DraggedColor = VerticalIndex & 0xf;
				setMap(WASCOL);
			}
		}
	} else {

		if (rstMap(WASMOV) && rstMap(WASCOL)) {

			if (chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {

				key = GetKeyState(VK_SHIFT) & 0X8000;
				swtch = GetKeyState(VK_CONTROL) & 0X8000;
				for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

					col = StitchBuffer[ind].attribute&COLMSK;
					if (col == VerticalIndex) {

						StitchBuffer[ind].attribute &= NCOLMSK;
						StitchBuffer[ind].attribute |= DraggedColor;
					} else {

						if (!key&&col == DraggedColor) {

							StitchBuffer[ind].attribute &= NCOLMSK;
							StitchBuffer[ind].attribute |= VerticalIndex;
						}
					}
				}
				for (ind = 0; ind < FormIndex; ind++) {

					pfrm = &FormList[ind];
					if (pfrm->fillType) {

						if (pfrm->fillColor == VerticalIndex)
							pfrm->fillColor = DraggedColor;
						else {

							if (!key&&pfrm->fillColor == DraggedColor)
								pfrm->fillColor = VerticalIndex;
						}
						if (pfrm->fillInfo.feather.color == VerticalIndex)
							pfrm->fillInfo.feather.color = DraggedColor;
						else {

							if (!key&&pfrm->fillInfo.feather.color == DraggedColor)
								pfrm->fillInfo.feather.color = VerticalIndex;
						}
					}
					if (pfrm->edgeType) {

						if (pfrm->borderColor == VerticalIndex)
							pfrm->borderColor = DraggedColor;
						else {

							if (!key&&pfrm->borderColor == DraggedColor)
								pfrm->borderColor = VerticalIndex;
						}
					}
				}
				if (!swtch) {

					ind = UserColor[VerticalIndex];
					UserColor[VerticalIndex] = UserColor[DraggedColor];
					if (!key) {

						UserColor[DraggedColor] = ind;
						nuscol(DraggedColor);
					}
					nuscol(VerticalIndex);
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

	*MarkedStitchMap = 0;
	if (chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {

		VerticalIndex &= COLMSK;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			setr(StitchBuffer[ind].attribute&COLMSK);
		if (*MarkedStitchMap == COLSMSK)
			tabmsg(IDS_COLAL);
		else {

			dcol = 15;
			while (chkr(dcol))
				dcol--;
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				col = StitchBuffer[ind].attribute&COLMSK;
				if (col >= VerticalIndex&&col < dcol) {

					StitchBuffer[ind].attribute &= NCOLMSK;
					StitchBuffer[ind].attribute |= col + 1;
				}
			}
			for (ind = 0; ind < FormIndex; ind++) {

				tfrm = &FormList[ind];
				if (tfrm->fillType) {

					if (tfrm->fillColor >= VerticalIndex&&tfrm->fillColor < dcol)
						tfrm->fillColor++;
					if (tfrm->fillInfo.feather.color >= VerticalIndex&&tfrm->fillInfo.feather.color < dcol)
						tfrm->fillInfo.feather.color++;
				}
				if (tfrm->edgeType) {

					if (tfrm->borderColor >= VerticalIndex&&tfrm->borderColor < dcol)
						tfrm->borderColor++;
				}
			}
			for (ind = dcol; ind > VerticalIndex; ind--) {

				UserColor[ind] = UserColor[ind - 1];
				nuscol(ind);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

BOOL usedcol() {

	unsigned ind;

	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

		if ((StitchBuffer[ind].attribute&COLMSK) == VerticalIndex)
			return 1;
	}
	return 0;
}

void delcol() {

	unsigned	ind, col;
	FRMHED*		tfrm;

	if (chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {

		VerticalIndex &= 0xf;
		if (usedcol())
			tabmsg(IDS_COLU);
		else {

			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				col = StitchBuffer[ind].attribute&COLMSK;
				if (col > VerticalIndex&&col) {

					StitchBuffer[ind].attribute &= NCOLMSK;
					StitchBuffer[ind].attribute |= col - 1;
				}
			}
			for (ind = 0; ind < FormIndex; ind++) {

				tfrm = &FormList[ind];
				if (tfrm->fillType) {

					if (tfrm->fillColor > VerticalIndex)
						tfrm->fillColor--;
					if (tfrm->fillInfo.feather.color > VerticalIndex)
						tfrm->fillInfo.feather.color--;
				}
				if (tfrm->edgeType) {

					if (tfrm->borderColor > VerticalIndex)
						tfrm->borderColor--;
				}
			}
			for (ind = VerticalIndex; ind < 15; ind++) {

				UserColor[ind] = UserColor[ind + 1];
				nuscol(ind);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void set1knot() {

	unsigned src, dst;

	if (PCSHeader.stitchCount&&chkMap(SELBOX)) {

		savdo();
		if (ClosestPointIndex == (unsigned)PCSHeader.stitchCount - 1) {

			setMap(FILDIR);
			OutputIndex = ClosestPointIndex + 1;
			endknt(ClosestPointIndex);
		} else {

			src = PCSHeader.stitchCount - 1;
			dst = PCSHeader.stitchCount + 4;
			while (src&&src >= ClosestPointIndex)
				mvstch(dst--, src--);
			OutputIndex = ClosestPointIndex + 1;
			strtknt(ClosestPointIndex);
		}
		PCSHeader.stitchCount += 5;
		coltab();
		setMap(RESTCH);
	} else {

		msgstr(IDS_NOSTCHSEL);
		shoMsg(MsgBuffer);
	}
}

void selfrm0() {

	rstMap(GRPSEL);
	if (rstMap(FORMSEL)) {

		setMap(FRMPSEL);
		ClosestVertexToCursor = 0;
	}
	setMap(RESTCH);
}

void selfrmx() {

	rstMap(GRPSEL);
	if (rstMap(FORMSEL)) {

		setMap(FRMPSEL);
		ClosestVertexToCursor = FormList[ClosestFormToCursor].sides - 1;
	}
	setMap(RESTCH);
}

void setpclp() {

	POINT		tof;
	POINT		tpnt;
	unsigned	ind, ine;

	sfCor2px(InterleaveSequence[0], &tpnt);
	FormVerticesAsLine[0].x = tpnt.x;
	FormVerticesAsLine[0].y = tpnt.y;
	sfCor2px(InterleaveSequence[1], &tpnt);
	tof.x = Msg.pt.x - StitchWindowOrigin.x - tpnt.x;
	tof.y = Msg.pt.y - StitchWindowOrigin.y - tpnt.y;
	for (ind = 0; ind < OutputIndex - 2; ind++) {

		ine = ind + 1;
		sfCor2px(InterleaveSequence[ine], &tpnt);
		FormVerticesAsLine[ine].x = tpnt.x + tof.x;
		FormVerticesAsLine[ine].y = tpnt.y + tof.y;
	}
	ind++;
	sfCor2px(InterleaveSequence[ind], &tpnt);
	FormVerticesAsLine[ind].x = tpnt.x;
	FormVerticesAsLine[ind].y = tpnt.y;
}

void dupclp() {

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, FormVerticesAsLine, OutputIndex);
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

	tpnt.x = Msg.pt.x + FormMoveDelta.x;
	tpnt.y = Msg.pt.y + FormMoveDelta.y;
	pxCor2stch(tpnt);
	pof.x = SelectedPoint.x - InterleaveSequence[1].x;
	pof.y = SelectedPoint.y - InterleaveSequence[1].y;
	ine = nxt(ClosestVertexToCursor);
	cnt = OutputIndex - 2;
	fltspac(&CurrentFormVertices[ine], cnt);
	for (ind = 1; ind < OutputIndex - 1; ind++) {

		CurrentFormVertices[ine].x = InterleaveSequence[ind].x + pof.x;
		CurrentFormVertices[ine].y = InterleaveSequence[ind].y + pof.y;
		ine++;
	}
	SelectedForm->sides += cnt;
	refil();
	coltab();
	setMap(RESTCH);
}

BOOL sidclp() {

	fvars(ClosestFormToCursor);
	if (OpenClipboard(ThrEdWindow)) {

		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {

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
	SelectedFormVertices.start = ClosestVertexToCursor;
	SelectedFormVertices.form = ClosestFormToCursor;
	ritfcor(&FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
	setMap(RESTCH);
}

unsigned chkMsg() {

	unsigned		l_code, dst;
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

	if (Msg.message == WM_MOUSEMOVE) {

		if (chkMap(TXTMOV))
		{
			txtrmov();
			return 1;
		}
		movchk();
		if (Msg.pt.x >= StitchWindowAbsRect.left&&Msg.pt.x <= StitchWindowAbsRect.right
			&&Msg.pt.y >= StitchWindowAbsRect.top&&Msg.pt.y <= StitchWindowAbsRect.bottom) {

			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				mvshft();
				return 1;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000 && px2stch())
				ritfcor(&SelectedPoint);
			if (*FlagMap&(PRFACTB | FORMINB | POLIMOVB) || FormDataSheet) {

				SetCursor(ArrowCursor);
				goto gotcur;
			}
			if (!chkMap(INIT)) {

				if (chku(NEDOF))
					SetCursor(CrossCursor);
				else
					SetCursor(NeedleUpCursor);
				goto gotcur;
			}
			if (chkMap(INSRT)) {

				if (chku(NEDOF))
					SetCursor(CrossCursor);
				else {

					if (chkMap(LIN1))
						SetCursor(NeedleUpCursor);
					else {

						if (StitchBuffer[ClosestPointIndex + 1].x > StitchBuffer[ClosestPointIndex].x) {

							if (StitchBuffer[ClosestPointIndex + 1].y > StitchBuffer[ClosestPointIndex].y)
								SetCursor(NeedleLeftUpCursor);
							else
								SetCursor(NeedleLeftDownCursor);
						} else {

							if (StitchBuffer[ClosestPointIndex + 1].y > StitchBuffer[ClosestPointIndex].y)
								SetCursor(NeedleRightUpCursor);
							else
								SetCursor(NeedleRightDownCursor);
						}
					}
				}
				goto gotcur;
			}
			if (*FlagMap&(BZUMINB | BOXZUMB | SELPNTB)) {

				SetCursor(CrossCursor);
				goto gotcur;
			}
			if (*FlagMap&(SATINB | SATPNTB | INSFRMB)) {

				if (chku(FRMX))
					SetCursor(CrossCursor);
				else
					SetCursor(FormCursor);
				goto gotcur;
			}
			if (*FlagMap&SATCNKTB)
				SetCursor(DLineCursor);
			else
				SetCursor(ArrowCursor);
		gotcur:;
			if (chkMap(FPUNCLP)) {

				unpclp();
				setpclp();
				setMap(SHOP);
				dupclp();
			}
			if (chkMap(INSFIL)) {

				unfrm();
				tpnt.x = Msg.pt.x - StitchWindowOrigin.x;
				tpnt.y = Msg.pt.y - StitchWindowOrigin.y;
				insflin(tpnt);
				setMap(SHOFRM);
				dufrm();
				return 1;
			}
			if (chkMap(MOVFRMS)) {

				unstrtch();
				StretchBoxLine[0].x = StretchBoxLine[3].x = StretchBoxLine[4].x = Msg.pt.x - FormMoveDelta.x - StitchWindowOrigin.x;
				StretchBoxLine[1].x = StretchBoxLine[2].x = Msg.pt.x + SelectedFormsSize.x - FormMoveDelta.x - StitchWindowOrigin.x;
				StretchBoxLine[0].y = StretchBoxLine[1].y = StretchBoxLine[4].y = Msg.pt.y - FormMoveDelta.y - StitchWindowOrigin.y;
				StretchBoxLine[2].y = StretchBoxLine[3].y = Msg.pt.y + SelectedFormsSize.y - FormMoveDelta.y - StitchWindowOrigin.y;
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
				nusiz.x = Msg.pt.x - StitchWindowOrigin.x;
				nusiz.y = Msg.pt.y - StitchWindowOrigin.y;
				ind = (SelectedFormControlVertex + 2) % 4;
				tdub = fabs((double)(nusiz.x - StretchBoxLine[ind].x) / (nusiz.y - StretchBoxLine[ind].y));
				if (ind & 1) {

					if (tdub < XYratio)
						nusiz.x = (StretchBoxLine[ind].y - nusiz.y)*XYratio + StretchBoxLine[ind].x;
					else
						nusiz.y = (StretchBoxLine[ind].x - nusiz.x) / XYratio + StretchBoxLine[ind].y;
					ind = nxtcrnr(ind);
					StretchBoxLine[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					StretchBoxLine[ind].x = nusiz.x;
					StretchBoxLine[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					StretchBoxLine[ind].x = nusiz.x;
				} else {

					if (tdub < XYratio)
						nusiz.x = (nusiz.y - StretchBoxLine[ind].y)*XYratio + StretchBoxLine[ind].x;
					else
						nusiz.y = (nusiz.x - StretchBoxLine[ind].x) / XYratio + StretchBoxLine[ind].y;
					ind = nxtcrnr(ind);
					StretchBoxLine[ind].x = nusiz.x;
					ind = nxtcrnr(ind);
					StretchBoxLine[ind].x = nusiz.x;
					StretchBoxLine[ind].y = nusiz.y;
					ind = nxtcrnr(ind);
					StretchBoxLine[ind].y = nusiz.y;
				}
				StretchBoxLine[4].x = StretchBoxLine[0].x;
				StretchBoxLine[4].y = StretchBoxLine[0].y;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			}
			if (chkMap(STRTCH)) {

				unstrtch();
				if (SelectedFormControlVertex & 1)
					tlng = Msg.pt.x - StitchWindowOrigin.x;
				else
					tlng = Msg.pt.y - StitchWindowOrigin.y;
				dst = (SelectedFormControlVertex + 2) % 4;
				l_code = nxtcrnr(dst);
				for (ind = 0; ind < 4; ind++) {

					if ((unsigned)ind != dst && (unsigned)ind != l_code) {

						if (SelectedFormControlVertex & 1)
							StretchBoxLine[ind].x = tlng;
						else
							StretchBoxLine[ind].y = tlng;
					}
				}
				StretchBoxLine[4].x = StretchBoxLine[0].x;
				StretchBoxLine[4].y = StretchBoxLine[0].y;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			}
			if (chkMap(INSFRM)) {

				uninsf();
				InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
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
				RubberBandLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				RubberBandLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				setMap(SHOMOV);
				ritmov();
				if (px2stch())
					ritfcor(&SelectedPoint);
				return 1;
			}
			if (chkMap(MOVCNTR)) {

				unrot();
				px2stch();
				RotationCenter.x = SelectedPoint.x;
				RotationCenter.y = SelectedPoint.y;
				ritrot();
				return 1;
			}
			if (chkMap(ROTCAPT)) {

				unrotu();
				unrot();
				RotateBoxToCursorLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				RotateBoxToCursorLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				padj.x = RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x;
				padj.y = RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y;
				if (padj.x)
					RotationAngle = -atan2(padj.y, padj.x);
				else {

					if (padj.y > 0)
						RotationAngle = PI / 2;
					else
						RotationAngle = -PI / 2;
				}
				RotationAngle -= RotationHandleAngle;
				ritrot();
				setMap(ROTUSHO);
				durotu();
				return 1;
			}
			if (chkMap(SELPNT)) {

				if (setMap(VCAPT))
					SetCapture(ThrEdWindow);
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
					ritfcor(&SelectedPoint);
				unlin();
				MoveLine0[1].x = MoveLine1[0].x = Msg.pt.x - StitchWindowOrigin.x;
				MoveLine0[1].y = MoveLine1[0].y = Msg.pt.y - StitchWindowOrigin.y;
				dulin();
				return 1;
			}
			if (chkMap(INSRT)) {

				if (px2stch())
					ritfcor(&SelectedPoint);
				if (setMap(VCAPT))
					SetCapture(ThrEdWindow);
				if (chkMap(LIN1)) {

					if (PCSHeader.stitchCount) {

						xlin1();
						InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
						InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
						setMap(ILIN1);
						ilin1();
					}
					return 1;
				}
				xlin();
				InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				setMap(ILIN);
				ilin();
				return 1;
			}
			if (chkMap(BOXZUM) && setMap(VCAPT))
				SetCapture(ThrEdWindow);
			if (chkMap(BZUMIN) && (Msg.wParam&MK_LBUTTON)) {

				if (setMap(VCAPT))
					SetCapture(ThrEdWindow);
				unbBox();
				ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
				ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
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
	if (Msg.message == WM_LBUTTONUP)
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
			tpnt.x = (Msg.pt.x - FormMoveDelta.x - StitchWindowOrigin.x) - SelectedFormsRect.left;
			tpnt.y = (Msg.pt.y - FormMoveDelta.y - StitchWindowOrigin.y) - SelectedFormsRect.top;
			ratsr();
			FormMoveDelta.x = tpnt.x / HorizontalRatio;
			FormMoveDelta.y = tpnt.y / VerticalRatio;
			if (chkMap(FPSEL)) {

				fvars(ClosestFormToCursor);
				ine = SelectedFormVertices.start;
				for (ind = 0; (unsigned)ind <= SelectedFormVertices.vertexCount; ind++) {

					CurrentFormVertices[ine].x += FormMoveDelta.x;
					CurrentFormVertices[ine].y -= FormMoveDelta.y;
					ine = pdir(ine);
				}
				setpsel();
				frmout(ClosestFormToCursor);
				refil();
				setMap(RESTCH);
			} else {

				if (chkMap(BIGBOX)) {

					savdo();
					for (ind = 0; ind < (long)FormIndex; ind++)
						frmadj(ind);
					for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

						StitchBuffer[ind].x += FormMoveDelta.x;
						StitchBuffer[ind].y -= FormMoveDelta.y;
					}
					selal();
				} else {

					savdo();
					for (l_code = 0; l_code < SelectedFormCount; l_code++)
						frmadj(SelectedFormList[l_code]);
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
			padj.x = (StitchRangeRect.left + SelectBoxOffset.x) - SelectedPoint.x;
			padj.y = (StitchRangeRect.bottom + SelectBoxOffset.y) - SelectedPoint.y;
			for (ind = GroupStartStitch; ind <= (long)GroupEndStitch; ind++) {

				StitchBuffer[ind].x -= padj.x;
				StitchBuffer[ind].y -= padj.y;
			}
			setMap(RESTCH);
			return 1;
		}
		if (rstMap(CAPT)) {

			unlin();
			ReleaseCapture();
			rstMap(CAPT);
			px2stch();
			StitchBuffer[ClosestPointIndex].x = SelectedPoint.x;
			StitchBuffer[ClosestPointIndex].y = SelectedPoint.y;
			StitchBuffer[ClosestPointIndex].attribute |= USMSK;
			if (ZoomFactor < STCHBOX) {

				SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
				SelectObject(StitchWindowMemDC, LinePen);
				stchbox(ClosestPointIndex - 1, StitchWindowDC);
				stchbox(ClosestPointIndex, StitchWindowDC);
				stchbox(ClosestPointIndex + 1, StitchWindowDC);
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

				if (ZoomBoxOrigin.x > SelectedPoint.x) {

					StitchRangeRect.right = ZoomBoxOrigin.x;
					StitchRangeRect.left = SelectedPoint.x;
				} else {

					StitchRangeRect.right = SelectedPoint.x;
					StitchRangeRect.left = ZoomBoxOrigin.x;
				}
				if (ZoomBoxOrigin.y > SelectedPoint.y) {

					StitchRangeRect.top = ZoomBoxOrigin.y;
					StitchRangeRect.bottom = SelectedPoint.y;
				} else {

					StitchRangeRect.top = SelectedPoint.y;
					StitchRangeRect.bottom = ZoomBoxOrigin.y;
				}
				if (rstMap(GRPSEL)) {

					rngadj();
					for (ine = GroupStartStitch; ine < GroupEndStitch; ine++) {

						if (inrng(ine)) {

							ClosestPointIndex = ine;
							setMap(SELBOX);
							break;
						}
					}
				}
				if (rstMap(NOSEL)) {

					SelectedFormCount = 0;
					rstMap(FORMSEL);
					for (ine = 0; ine < FormIndex; ine++) {

						if (finrng(ine))
							SelectedFormList[SelectedFormCount++] = ine;
					}
					if (SelectedFormCount) {

						if (SelectedFormCount == 1) {

							ReleaseCapture();
							gotbox();
							ClosestFormToCursor = SelectedFormList[0];
							ClosestVertexToCursor = 0;
							SelectedFormCount = 0;
							ritnum(STR_NUMFORM, ClosestFormToCursor);
							setMap(RESTCH);
							setMap(FORMSEL);
							return 1;
						}
					}
					if (SelectedFormCount) {

						gotbox();
						return 1;
					}
					rstMap(BZUMIN);
					rstMap(BOXSLCT);
					rstMap(FRMPSEL);
					setMap(RESTCH);
				}
				if (rstMap(SELBOX)) {

					if (inrng(ClosestPointIndex)) {

						GroupStitchIndex = ClosestPointIndex + 1;
						ClosestPointIndex--;
						while (inrng(ClosestPointIndex))
							ClosestPointIndex--;
						ClosestPointIndex++;
						while (inrng(GroupStitchIndex))
							GroupStitchIndex++;
						GroupStitchIndex--;
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

				if (SelectedPoint.x < ZoomBoxOrigin.x) {

					tdub = ZoomBoxOrigin.x;
					ZoomBoxOrigin.x = SelectedPoint.x;
					SelectedPoint.x = tdub;
				}
				if (SelectedPoint.y < ZoomBoxOrigin.y) {

					tdub = ZoomBoxOrigin.y;
					ZoomBoxOrigin.y = SelectedPoint.y;
					SelectedPoint.y = tdub;
				}
				nusiz.x = SelectedPoint.x - ZoomBoxOrigin.x;
				nusiz.y = SelectedPoint.y - ZoomBoxOrigin.y;
				SelectedPoint.x = ZoomBoxOrigin.x + nusiz.x / 2;
				SelectedPoint.y = ZoomBoxOrigin.y + nusiz.y / 2;
				tdub = ZoomFactor;
				if (nusiz.x > nusiz.y) {

					nusiz.y = nusiz.x / StitchWindowAspectRatio;
					ZoomFactor = nusiz.x / UnzoomedRect.x;
				} else {

					nusiz.x = nusiz.y*StitchWindowAspectRatio;
					ZoomFactor = nusiz.y / UnzoomedRect.x;
				}
				if (ZoomFactor < ZoomMin) {

					ZoomFactor = tdub;
					zumin();
					return 1;
				}
				ZoomRect.left = ZoomRect.bottom = 0;
				ZoomRect.right = nusiz.x;
				ZoomRect.top = nusiz.y;
				shft(SelectedPoint);
				rstMap(BZUMIN);
				setMap(RESTCH);
				if (!setMap(ZUMED))
					movStch();
				return 1;
			}
			//			rstMap(BZUMIN);
		}
	}
	if (Msg.message == WM_RBUTTONDOWN || Msg.message == WM_LBUTTONDOWN) {

		if (rstMap(THUMON)) {

			if (chkok()) {

				save();
				thumbak();
				unmsg();
				return 1;
			}
			if (chkwnd(DiscardButton)) {

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

			if (Msg.message == WM_RBUTTONDOWN)
				setMap(RBUT);
			else
				rstMap(RBUT);
			for (ind = 0; ind < OLDVER; ind++) {

				if (Msg.hwnd == BackupViewer[ind]) {

					FileVersionIndex = ind;
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
		if (Msg.pt.x >= ColorBarRect.left&&Msg.pt.x <= ColorBarRect.right
			&&Msg.pt.y >= ColorBarRect.top&&Msg.pt.y <= ColorBarRect.bottom) {

			unpat();
			tdub = (double)(Msg.pt.y - ColorBarRect.top) / (ColorBarRect.bottom - ColorBarRect.top);
			if (Msg.message == WM_RBUTTONDOWN) {

				tdub = (double)(Msg.pt.y - ColorBarRect.top) / (ColorBarRect.bottom - ColorBarRect.top);
				if (Msg.wParam&MK_SHIFT && (chkMap(SELBOX) || chkMap(GRPSEL))) {

					unbox();
					GroupStitchIndex = tdub*PCSHeader.stitchCount;
					setMap(GRPSEL);
					grpAdj();
					nuAct(GroupStitchIndex);
					setMap(RESTCH);
				} else {

					ClosestPointIndex = tdub*PCSHeader.stitchCount;
					nuAct(ClosestPointIndex);
					movbox();
					if (rstMap(GRPSEL)) {

						rstMap(SCROS);
						rstMap(ECROS);
						setMap(RESTCH);
					}
				}
			} else {

				ClosestPointIndex = tdub*PCSHeader.stitchCount;
				nuAct(ClosestPointIndex);
				rstAll();
				setMap(SELBOX);
				selCol();
			}
			redraw(ColorBar);
			return 1;
		}
	}
	if (Msg.message == WM_RBUTTONDOWN) {

		if (chkMap(TXTRED) && !MsgWindow)
		{
			txtrbut();
			return 1;
		}
		if (GetKeyState(VK_SHIFT) & 0X8000) {

			if (closfrm()) {

				if ((chkMap(FRMPSEL) || chkMap(FPSEL)) && SelectedFormVertices.form == ClosestFormToCursor) {

					rstMap(FRMPSEL);
					setMap(FPSEL);
					SelectedFormVertices.finish = ClosestVertexToCursor;
					ine = (SelectedFormVertices.finish - SelectedFormVertices.start + SideCount) % SideCount;
					if (ine < (unsigned)SideCount >> 1) {

						SelectedFormVertices.vertexCount = ine;
						setMap(PSELDIR);
					} else {

						SelectedFormVertices.vertexCount = SideCount - ine;
						rstMap(PSELDIR);
					}
					setpsel();
					return 1;
				}
				rstMap(FPSEL);
				setMap(FRMPSEL);
				SelectedFormVertices.start = ClosestVertexToCursor;
				SelectedFormVertices.form = ClosestFormToCursor;
				selfpnt();
				return 1;
			}
		}
		if (chkMap(WASTRAC)) {

			if (chkMap(TRNIN0))
				dutrnum0(atoi(TraceInputBuffer));
			if (chkMap(TRNIN1))
				dutrnum1();
			if (!chkMap(WASEDG))
				tracpar();
			return 1;
		}
		unmsg();
		unboxs();
		unpat();
		BufferIndex = 0;
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

				if (PreferenceIndex) {

					chknum();
					unsid();
					PreferenceIndex = 0;
				} else {

					DestroyWindow(PreferencesWindow);
					rstMap(PRFACT);
					if (rstMap(WASRT))
						setMap(INSRT);
				}
			}
			setMap(RESTCH);
			return 1;
		} else {

			if (FormMenuChoice) {

				chknum();
				FormMenuChoice = 0;
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
		if (px2stch() && !(chkMap(SIZSEL) && chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {

			if (FormIndex && !chkMap(FRMOF)) {

				if (Msg.wParam&MK_SHIFT) {

					TmpFormIndex = ClosestFormToCursor;
					if (closfrm()) {

						ine = 0;
						if (SelectedFormCount) {

							nuslst(ClosestFormToCursor);
							setMap(RESTCH);
							return 1;
						} else {

							if (rstMap(FORMSEL) && TmpFormIndex != ClosestFormToCursor) {

								if (TmpFormIndex > ClosestFormToCursor) {

									ine = ClosestFormToCursor;
									ClosestFormToCursor = TmpFormIndex;
									TmpFormIndex = ine;
								}
								for (ind = TmpFormIndex; ind <= (long)ClosestFormToCursor; ind++)
									SelectedFormList[ine++] = ind;
								SelectedFormCount = ine;
								setMap(RESTCH);
								return 1;
							} else
								nufsel();
						}
					}
				}
				if (Msg.wParam&MK_CONTROL) {

					if (!SelectedFormCount&&chkMap(FORMSEL)) {

						setMap(WASEL);
						PreviousFormIndex = ClosestFormToCursor;
					}
					if (closfrm())
						nufsel();
					if (SelectedFormCount > 1)
						butxt(HNUM, "");
					else
						ritnum(STR_NUMFORM, SelectedFormList[0] + 1);
					return 1;
				}
				if (chkMap(FORMSEL)) {

					if (FormMenuChoice) {

						chknum();
						FormMenuChoice = 0;
						goto frmskip;
					}
					if (FormDataSheet) {

						undat();
						unsid();
						FormMenuChoice = 0;
						setMap(RESTCH);
						goto frmskip;
					}
					ritfrct(ClosestFormToCursor, StitchWindowDC);
					lenCalc();
					if (!rstMap(ENTROT))
						rstMap(FORMSEL);
					setMap(DUMEN);
					if (!closPnt1(&ClosestPointIndex))
						unbox();
				frmskip:;
				}
				if (closfrm()) {

					setMap(FORMSEL);
					rstMap(FPSEL);
					unpsel();
					fvars(ClosestFormToCursor);
					ritfrct(ClosestFormToCursor, StitchWindowDC);
					if (rstMap(FRMPSEL) || SelectedFormCount) {

						setMap(RESTCH);
						SelectedFormCount = 0;
					}
					ritnum(STR_NUMFORM, ClosestFormToCursor);
					l_code = (FormList[ClosestFormToCursor].attribute&FRMLMSK) >> 1;
					lenCalc();
					unrot();
					return 1;
				}
				if (SelectedFormCount) {

					SelectedFormCount = 0;
					setMap(RESTCH);
				}
				if (rstMap(FRMPSEL))
					setMap(RESTCH);
			}
			if (chkMap(INIT) || FileHandle) {

				if (Msg.wParam&MK_SHIFT) {

					if (chkMap(SELBOX)) {

						l_code = ClosestPointIndex;
						closPnt1(&ClosestPointIndex);
						if (ClosestPointIndex != l_code) {

							unbox();
							GroupStitchIndex = ClosestPointIndex;
							ClosestPointIndex = l_code;
							setMap(GRPSEL);
							grpAdj();
						}
						nuAct(GroupStitchIndex);
						redraw(ColorBar);
						return 1;
					}
					if (chkMap(GRPSEL)) {

						l_code = ClosestPointIndex;
						closPnt1(&ClosestPointIndex);
						GroupStitchIndex = ClosestPointIndex;
						ClosestPointIndex = l_code;
						grpAdj();
						nuAct(GroupStitchIndex);
						redraw(ColorBar);
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
						if (PCSHeader.stitchCount)
							rebox();
					}
				}
				clrstch();
				setMap(RESTCH);
			}
			return 1;
		}
		if (Msg.pt.x >= MinLenRect.left&&Msg.pt.x <= MinLenRect.right
			&&Msg.pt.y > MinLenRect.top&&Msg.pt.y <= MinLenRect.bottom) {

			srchk();
			setsrch(SmallestStitchIndex);
			lensadj();
			setMap(GRPSEL);
			setMap(RESTCH);
			return 1;
		}
		if (Msg.pt.x >= MaxLenRect.left&&Msg.pt.x <= MaxLenRect.right
			&&Msg.pt.y > MaxLenRect.top&&Msg.pt.y <= MaxLenRect.bottom) {

			srchk();
			setsrch(LargestStitchIndex);
			lensadj();
			setMap(GRPSEL);
			setMap(RESTCH);
		}
		return 1;
	}
	if (Msg.message == WM_LBUTTONDOWN) {

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
			if (chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15]))
			{
				dufcol(VerticalIndex + 1);
				return 1;
			}
		}
		if (rstMap(FSETBCOL))
		{
			unsid();
			unmsg();
			if (chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15]))
			{
				dubcol(VerticalIndex + 1);
				return 1;
			}
		}
		if (rstMap(FPUNCLP)) {

			savdo();
			fixpclp();
			frmout(ClosestFormToCursor);
			return 1;
		}
		if (chkMap(FPSEL) && !chkMap(FUNCLP) && !chkMap(ROTAT)) {

			MoveMemory(&SelectedFormsLine, &SelectedPointsLine, sizeof(POINT) * 9);
			MoveMemory(&SelectedFormsRect, &SelectedPixelsRect, sizeof(RECT));
			if (chkbig())
				return 1;
		}
		if (chkMap(WASTRAC)) {

			tracpar();
			return 1;
		}
		if (rstMap(HUPMSG)) {

			for (ind = 0; ind < HUPS; ind++) {

				if (Msg.hwnd == SideWindow[ind]) {

					switch (ind + 1) {

					case SETCUST:

						IniFile.customHoopX = IniFile.hoopSizeX;
						IniFile.customHoopY = IniFile.hoopSizeY;
						setMap(MSGOF);
						sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_CUSTHUP], IniFile.hoopSizeX / PFGRAN, IniFile.hoopSizeY / PFGRAN);
						shoMsg(MsgBuffer);
						break;

					case SMALHUP:

						IniFile.hoopSizeX = SHUPX;
						IniFile.hoopSizeY = SHUPY;
						IniFile.hoopType = SMALHUP;
						break;

					case LARGHUP:

						IniFile.hoopSizeX = LHUPX;
						IniFile.hoopSizeY = LHUPY;
						IniFile.hoopType = LARGHUP;
						break;

					case CUSTHUP:

						IniFile.hoopSizeX = IniFile.customHoopX;
						IniFile.hoopSizeY = IniFile.customHoopY;
						IniFile.hoopType = CUSTHUP;
						break;

					case HUP100:

						IniFile.hoopSizeX = HUP100XY;
						IniFile.hoopSizeY = HUP100XY;
						IniFile.hoopType = CUSTHUP;
						break;
					}
					UnzoomedRect.x = IniFile.hoopSizeX;
					UnzoomedRect.y = IniFile.hoopSizeY;
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

			l_code = 0;
			if (chkok()) {

				savdo();
				rstMap(DELTO);
				l_code = 1;
			} else {

				GetWindowRect(DeleteStitchesDialog, &trct);
				if (Msg.pt.x >= trct.left&&Msg.pt.x <= trct.right
					&&Msg.pt.y >= trct.top&&Msg.pt.y <= trct.bottom) {

					savdo();
					setMap(DELTO);
					l_code = 1;
				}
			}
			delsfrms(l_code);
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
			pxrct2stch(SelectedFormsRect, &tbig);
			px2stch();
			FormMoveDelta.x = SelectedPoint.x - ((tbig.right - tbig.left) / 2 + tbig.left);
			FormMoveDelta.y = SelectedPoint.y - ((tbig.top - tbig.bottom) / 2 + tbig.bottom);
			for (ind = 0; ind < ClipFormsCount; ind++) {

				ClosestFormToCursor = FormIndex + ind;
				fvars(ClosestFormToCursor);
				for (ine = 0; ine < SelectedForm->sides; ine++) {

					SelectedForm->vertices[ine].x += FormMoveDelta.x;
					SelectedForm->vertices[ine].y += FormMoveDelta.y;
				}
				frmout(ClosestFormToCursor);
				refil();
			}
			FormIndex += ClipFormsCount;
			setMap(RESTCH);
			return 1;
		}
		if (SelectedFormCount && !chkMap(ROTAT) && chkbig())
			return 1;
		if (chkMap(SIDCOL) && chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {

			savdo();
			if (rstMap(FSETFCOL))
			{
				dufcol(VerticalIndex + 1);
				return 1;
			}
			if (rstMap(BRDSID))
			{
				nubrdcol(VerticalIndex);
				goto chkcolx;
			}
			if (rstMap(APSID))
			{
				nulapcol(VerticalIndex);
				goto chkcolx;
			}
			if (rstMap(FTHSID))
			{
				nufthcol(VerticalIndex);
				goto chkcolx;
			}
			if (rstMap(UNDCOL))
			{
				SelectedForm->underlayColor = VerticalIndex & 0xf;
				refilfn();
				coltab();
				goto chkcolx;
			}
			if (rstMap(FSETUCOL))
			{
				dundcol(VerticalIndex + 1);
				return 1;
			}
			nufilcol(VerticalIndex);
		chkcolx:;
			MsgBuffer[0] = (TCHAR)VerticalIndex + 0x30;
			MsgBuffer[1] = 0;
			SetWindowText(ValueWindow[LBRDCOL], MsgBuffer);
			unsid();
			coltab();
			setMap(RESTCH);
			rstMap(SIDCOL);
			FormMenuChoice = 0;
			return 1;
		}
		if (rstMap(OSAV)) {

			if (chkok()) {

				save();
				nuFil();
				unmsg();
				return 1;
			}
			if (chkwnd(DiscardButton)) {

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

				if (!chkwnd(CancelButton))
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
			if (chkwnd(DiscardButton))
				reldun();
			unmsg();
			return 1;
		}
		if (chkMap(PRFACT)) {

			chknum();
			if (Msg.hwnd == ValueWindow[PSQR]) {

				if (toglu(SQRFIL))
					SetWindowText(ValueWindow[PSQR], StringTable[STR_PNTD]);
				else
					SetWindowText(ValueWindow[PSQR], StringTable[STR_SQR]);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[PBLNT]) {

				if (toglu(BLUNT))
					SetWindowText(ValueWindow[PBLNT], StringTable[STR_TAPR]);
				else
					SetWindowText(ValueWindow[PBLNT], StringTable[STR_BLUNT]);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[PUND]) {

				if (toglu(DUND))
					SetWindowText(ValueWindow[PUND], StringTable[STR_OFF]);
				else
					SetWindowText(ValueWindow[PUND], StringTable[STR_ON]);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[PHUP]) {

				sidhup();
				return 1;
			}
			for (ind = 0; ind < PRFLINS; ind++) {

				if (Msg.hwnd == ValueWindow[ind]) {

					PreferenceIndex = ind + 1;
					prfsid(ValueWindow[ind]);
					break;
				}
			}
			return 1;
		}
		if (!chkMap(ROTAT) && chkMap(GRPSEL)) {

			if (iselpnt()) {

				for (ind = 0; ind < 4; ind++) {

					StretchBoxLine[ind].x = FormOutlineRect[ind << 1].x;
					StretchBoxLine[ind].y = FormOutlineRect[ind << 1].y;
				}
				StretchBoxLine[4].x = StretchBoxLine[0].x;
				StretchBoxLine[4].y = StretchBoxLine[0].y;
				if (SelectedFormControlVertex & 1)
					setMap(STRTCH);
				else {

					setMap(EXPAND);
					XYratio = (double)(StitchRangeRect.right - StitchRangeRect.left) / (StitchRangeRect.top - StitchRangeRect.bottom);
				}
				SelectedFormControlVertex >>= 1;
				setMap(SHOSTRTCH);
				strtchbox();
				return 1;
			} else {

				StitchCoordinatesPixels.x = Msg.pt.x - StitchWindowOrigin.x;
				StitchCoordinatesPixels.y = Msg.pt.y - StitchWindowOrigin.y;
				if (StitchCoordinatesPixels.x >= FormOutlineRect[0].x&&StitchCoordinatesPixels.x <= FormOutlineRect[2].x&&
					StitchCoordinatesPixels.y >= FormOutlineRect[0].y&&StitchCoordinatesPixels.y <= FormOutlineRect[4].y) {

					duSelbox();
					setMap(SELPNT);
					SetCapture(ThrEdWindow);
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
			if (chkwnd(DiscardButton)) {

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

					ind = duth(ThrName);
					ThrName[ind] = 't';
					setMap(IGNAM);
					thrsav();
					ThrName[ind] = 'r';
					if (FileVersionIndex)
						WorkingFileName[ind] = FileVersionIndex + 0x2f;
					setMap(REDOLD);
					nuFil();
					WorkingFileName[ind] = 'r';
					switch (FileVersionIndex) {

					case 3:

						movbak('1', '2');

					case 2:

						movbak('0', '1');

					case 1:

						movbak('r', '0');
					}
					movbak('t', 'r');
					ThrName[ind] = 't';
					DeleteFile(ThrName);
					ThrName[ind] = 'r';
					FileHandle = CreateFile(ThrName, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
						OPEN_EXISTING, 0, NULL);
					if (FileHandle == INVALID_HANDLE_VALUE)
						FileHandle = 0;
					return 1;
				}
				GetWindowRect(CancelButton, &trct);
				if (Msg.pt.x >= trct.left&&Msg.pt.x <= trct.right
					&&Msg.pt.y >= trct.top&&Msg.pt.y <= trct.bottom) {

					getbak();
					return 1;
				}
			}
		}
		if (chkMap(DELFRM)) {

			l_code = 0;
			if (chkok()) {

				savdo();
				rstMap(DELTO);
				l_code = 1;
			} else {

				GetWindowRect(DeleteStitchesDialog, &trct);
				if (Msg.pt.x >= trct.left&&Msg.pt.x <= trct.right
					&&Msg.pt.y >= trct.top&&Msg.pt.y <= trct.bottom) {

					savdo();
					setMap(DELTO);
					l_code = 1;
				}
			}
			if (l_code) {

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

			if (chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2])) {

				VerticalIndex -= 13;
				ThreadSize[ThreadSizeSelected][0] = cmap[VerticalIndex];
				ThreadSizeIndex[ThreadSizeSelected] = VerticalIndex;
				strncpy_s(buf, ThreadSize[ThreadSizeSelected], 2);
				buf[2] = 0;
				SetWindowText(ThreadSizeWin[ThreadSizeSelected], buf);
				setMap(RESTCH);
				for (ind = 0; ind < 3; ind++)
					DestroyWindow(ChangeThreadSizeWin[ind]);
				return 1;
			} else {

				for (ind = 0; ind < 3; ind++)
					DestroyWindow(ChangeThreadSizeWin[ind]);
			}
		}
		if (rstMap(POLIMOV)) {

			savdo();
			setfrm();
			return 1;
		}
		if (rstMap(FORMIN)) {

			GetWindowRect(MsgWindow, &trct);
			if (Msg.pt.x >= trct.left&&Msg.pt.x <= trct.right
				&&Msg.pt.y >= trct.top&&Msg.pt.y <= trct.bottom) {

				ind = (Msg.pt.y - trct.top - 1) / (ButtonHeight - 4);
				if (rstMap(FENDIN)) {

					if (ind == 3)
						rstu(SQRFIL);
					if (ind == 4)
						setu(SQRFIL);
				} else
					duform(ind);
				switch (ind) {

				case FRMRPOLY - 1:
				case FRMSTAR - 1:
				case FRMSPIRAL - 1:
				case FRMHEART - 2:
				case FRMLENS - 2:
				case FRMEGG - 2:
				case FRMZIGZAG - 2:

					return 1;
				}
			}
			unmsg();
			return 1;
		}
		if (PreferenceIndex == PAP + 1 && chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {

			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", VerticalIndex);
			UnderlayColor = VerticalIndex;
			SetWindowText(ValueWindow[PAP], MsgBuffer);
			unsid();
			return 1;
		}
		if (rstMap(SIDACT)) {

			savdo();
			if (FormMenuChoice == LFTHTYP) {

				for (ind = 0; ind < 6; ind++) {

					if (Msg.hwnd == SideWindow[ind]) {

						SelectedForm->fillInfo.feather.fillType = ind + 1;
						refil();
						refrm();
						break;
					}
				}
				return 1;
			}
			if (FormMenuChoice == LLAYR) {

				for (ind = 0; ind < 5; ind++) {

					if (Msg.hwnd == SideWindow[ind])
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

				if (iseclp(ClosestFormToCursor))
					deleclp(ClosestFormToCursor);
				if (Msg.hwnd == SideWindow[0]) {

					SelectedForm->edgeType = 0;
					refilfn();
					coltab();
					unsid();
					setMap(RESTCH);
					return 1;
				}
				if (Msg.hwnd == SideWindow[1]) {

					if (SelectedForm->edgeType) {

						l_code = SelectedForm->edgeType&NEGUND;
						if (l_code == EDGECLIP || l_code == EDGEANGSAT || l_code == EDGEAPPL)
							bsizpar();
						SelectedForm->edgeType = EDGELINE;
						goto didfil;
					} else {

						unmsg();
						unsid();
						bord();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[2]) {

					if (SelectedForm->edgeType) {

						l_code = SelectedForm->edgeType&NEGUND;
						if (l_code == EDGECLIP || l_code == EDGEANGSAT || l_code == EDGEAPPL)
							bsizpar();
						SelectedForm->edgeType = EDGEBEAN;
						goto didfil;
					} else {

						unmsg();
						unsid();
						dubold();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[3]) {

					unmsg();
					unsid();
					fclp();
					return 1;
				}
				if (Msg.hwnd == SideWindow[4]) {

					if (SelectedForm->edgeType) {

						switch (SelectedForm->edgeType) {

						case EDGECLIP:

							bsizpar();

						case EDGELINE:
						case EDGEBEAN:

							SelectedForm->borderSize = BorderWidth;
							SelectedForm->edgeSpacing = StitchSpacing;
							break;

						case EDGEPROPSAT:

							SelectedForm->edgeSpacing /= 2;
							break;
						}
						SelectedForm->edgeType = EDGEANGSAT;
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
				if (Msg.hwnd == SideWindow[5]) {

					if (SelectedForm->fillType) {

						delmfil(SelectedForm->fillColor);
						SelectedForm->fillType = 0;
					}
					if (SelectedForm->edgeType) {

						if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN || SelectedForm->edgeType == EDGECLIP) {

							SelectedForm->borderSize = BorderWidth;
							SelectedForm->edgeSpacing = StitchSpacing;
							if (SelectedForm->edgeType == EDGECLIP)
								bsizpar();
						}
						SelectedForm->edgeType = EDGEAPPL;
						if (chku(DUND))
							SelectedForm->edgeType |= EGUND;
						SelectedForm->borderColor |= (UnderlayColor << 4);
						goto didfil;
					} else {

						unmsg();
						unsid();
						apliq();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[6]) {

					if (SelectedForm->edgeType) {

						switch (SelectedForm->edgeType) {

						case EDGECLIP:

							bsizpar();
						case EDGELINE:
						case EDGEBEAN:

							SelectedForm->borderSize = BorderWidth;
							SelectedForm->edgeSpacing = StitchSpacing;
							break;

						case EDGEANGSAT:

							SelectedForm->edgeSpacing *= 2;
						}
						SelectedForm->edgeType = EDGEPROPSAT;
						if (chku(DUND))
							SelectedForm->edgeType |= EGUND;
						goto didfil;
					} else {

						unmsg();
						unsid();
						prpbrd(StitchSpacing);
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[7]) {

					if (SelectedForm->edgeType) {

						if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN || SelectedForm->edgeType == EDGECLIP) {

							SelectedForm->borderSize = BorderWidth;
							SelectedForm->edgeSpacing = StitchSpacing;
							if (SelectedForm->edgeType == EDGECLIP)
								bsizpar();
						}
						SelectedForm->edgeType = EDGEBHOL;
						goto didfil;
					} else {

						unmsg();
						unsid();
						bhol();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[8]) {

					if (SelectedForm->edgeType) {

						if (SelectedForm->edgeType == EDGELINE || SelectedForm->edgeType == EDGEBEAN || SelectedForm->edgeType == EDGECLIP) {

							SelectedForm->borderSize = BorderWidth;
							SelectedForm->edgeSpacing = StitchSpacing;
							if (SelectedForm->edgeType == EDGECLIP)
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
				if (Msg.hwnd == SideWindow[9]) {

					if (SelectedForm->edgeType) {

						l_code = SelectedForm->edgeType&NEGUND;
						if (l_code == EDGECLIP || l_code == EDGEANGSAT || l_code == EDGEAPPL)
							bsizpar();
						SelectedForm->edgeType = EDGEDOUBLE;
						goto didfil;
					} else {

						unmsg();
						unsid();
						dubsfil();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[10]) {

					setMap(LINCHN);
					unmsg();
					unsid();
					chan();
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (Msg.hwnd == SideWindow[11]) {

					rstMap(LINCHN);
					unmsg();
					unsid();
					chan();
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (Msg.hwnd == SideWindow[12]) {

					unmsg();
					unsid();
					filclpx();
					return 1;
				}
			} else {

				if (SelectedForm->fillType == SAT&&SelectedForm->satinGuideCount)
					delsac(ClosestFormToCursor);
				if ((SelectedForm->edgeType&NEGUND) == EDGEAPPL) {

					SelectedForm->edgeType = EDGEANGSAT;
					if (chku(DUND))
						SelectedForm->edgeType |= EGUND;
				}
				if (Msg.hwnd == SideWindow[0]) {

					SelectedForm->type = FRMFPOLY;
					delmfil(SelectedForm->fillColor);
					SelectedForm->fillType = 0;
					coltab();
					unsid();
					setMap(RESTCH);
					return 1;
				}
				if (Msg.hwnd == SideWindow[1]) {

					savdo();
					SelectedForm->type = FRMFPOLY;
					if (SelectedForm->fillType) {

						respac();
						SelectedForm->fillType = VRTF;
						SelectedForm->type = FRMFPOLY;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filvrt();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[2]) {

					SelectedForm->type = FRMFPOLY;
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
				if (Msg.hwnd == SideWindow[3]) {

					SelectedForm->type = FRMFPOLY;
					if (SelectedForm->fillType) {

						respac();
						SelectedForm->fillType = ANGF;
						SelectedForm->angleOrClipData.angle = IniFile.fillAngle;
						goto didfil;
					} else {

						unmsg();
						unsid();
						filangl();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[4]) {

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
				if (Msg.hwnd == SideWindow[5]) {

					SelectedForm->type = SAT;
					unmsg();
					unsid();
					clpfil();
					return 1;
				}
				if (Msg.hwnd == SideWindow[6]) {

					if (SelectedForm->fillType) {

						if (SelectedForm->fillType == CLPF)
							SelectedForm->fillSpacing = StitchSpacing;
						chkcont();
						goto didfil;
					} else {

						unmsg();
						unsid();
						contfil();
						return 1;
					}
				}
				if (Msg.hwnd == SideWindow[7]) {

					unmsg();
					unsid();
					if (sidclp())
						vrtsclp();
					rstMap(CLPSHO);
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (Msg.hwnd == SideWindow[8]) {

					unmsg();
					unsid();
					if (sidclp())
						horsclp();
					rstMap(CLPSHO);
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (Msg.hwnd == SideWindow[9]) {

					unmsg();
					unsid();
					if (sidclp())
						angsclp();
					rstMap(CLPSHO);
					coltab();
					setMap(RESTCH);
					return 1;
				}
				if (Msg.hwnd == SideWindow[10]) {

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
				if (Msg.hwnd == SideWindow[11])	//vertical texture
				{
					if (istx(ClosestFormToCursor))
					{
						SelectedForm->fillType = TXVRTF;
						goto didfil;
					}
					dutxtfil();
				}
				if (Msg.hwnd == SideWindow[12])	//horizontal texture
				{
					if (istx(ClosestFormToCursor))
					{
						SelectedForm->fillType = TXHORF;
						goto didfil;
					}
					dutxtfil();
				}
				if (Msg.hwnd == SideWindow[13])	//angle texture
				{
					if (istx(ClosestFormToCursor))
					{
						SelectedForm->fillType = TXANGF;
						SelectedForm->angleOrClipData.angle = IniFile.fillAngle;
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
		if (FormDataSheet) {

			chknum();
			unsid();
			if (Msg.hwnd == ValueWindow[LTXOF])
			{
				FormMenuChoice = LTXOF;
				sidwnd(ValueWindow[LTXOF]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LUSPAC])
			{
				FormMenuChoice = LUSPAC;
				sidwnd(ValueWindow[LUSPAC]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LUANG])
			{
				FormMenuChoice = LUANG;
				sidwnd(ValueWindow[LUANG]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LULEN])
			{
				FormMenuChoice = LULEN;
				sidwnd(ValueWindow[LULEN]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LWLKIND])
			{
				FormMenuChoice = LWLKIND;
				sidwnd(ValueWindow[LWLKIND]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFTHSIZ]) {

				FormMenuChoice = LFTHSIZ;
				sidwnd(ValueWindow[LFTHSIZ]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFTHNUM]) {

				FormMenuChoice = LFTHNUM;
				sidwnd(ValueWindow[LFTHNUM]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFTHFLR]) {

				FormMenuChoice = LFTHFLR;
				sidwnd(ValueWindow[LFTHFLR]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFTHUPCNT]) {

				FormMenuChoice = LFTHUPCNT;
				sidwnd(ValueWindow[LFTHUPCNT]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFTHDWNCNT]) {

				FormMenuChoice = LFTHDWNCNT;
				sidwnd(ValueWindow[LFTHDWNCNT]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFTHBLND]) {

				setMap(FLPBLND);
				goto didfil;
			}
			if (Msg.hwnd == ValueWindow[LFTHUP]) {

				SelectedForm->extendedAttribute ^= AT_FTHUP;
				goto didfil;
			}
			if (Msg.hwnd == ValueWindow[LFTHDWN]) {

				SelectedForm->extendedAttribute ^= AT_FTHDWN;
				goto didfil;
			}
			if (Msg.hwnd == ValueWindow[LFTHTYP]) {

				FormMenuChoice = LFTHTYP;
				sidmsg(ValueWindow[LFTHTYP], &StringTable[STR_FTH0], 6);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFRM]) {

				savdo();
				unfil();
				if (SelectedForm->type == FRMLINE)
					SelectedForm->type = FRMFPOLY;
				else
					SelectedForm->type = FRMLINE;
				coltab();
				delsac(ClosestFormToCursor);
				setMap(RESTCH);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LLAYR]) {

				FormMenuChoice = LLAYR;
				rstMap(FILTYP);
				sidmsg(ValueWindow[LLAYR], LayerText, 5);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFRMFIL]) {

				rstMap(FILTYP);
				FormMenuChoice = LFRMFIL;
				sidmsg(ValueWindow[LFRMFIL], &StringTable[STR_FIL0], 14);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFRMCOL]) {

				FormMenuChoice = LFRMCOL;
				sidwnd(ValueWindow[LFRMCOL]);
				setMap(SIDCOL);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LUNDCOL]) {

				FormMenuChoice = LUNDCOL;
				sidwnd(ValueWindow[LUNDCOL]);
				setMap(SIDCOL);
				setMap(UNDCOL);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFTHCOL]) {

				FormMenuChoice = LFTHCOL;
				sidwnd(ValueWindow[LFTHCOL]);
				setMap(SIDCOL);
				setMap(FTHSID);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFRMSPAC]) {

				FormMenuChoice = LFRMSPAC;
				sidwnd(ValueWindow[LFRMSPAC]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFRMLEN]) {

				FormMenuChoice = LFRMLEN;
				sidwnd(ValueWindow[LFRMLEN]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRD]) {

				setMap(FILTYP);
				sidmsg(ValueWindow[LBRD], &StringTable[STR_EDG0], EDGETYPS + 1);
				setMap(BRDACT);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRDCOL]) {

				FormMenuChoice = LBRDCOL;
				sidwnd(ValueWindow[LBRDCOL]);
				setMap(SIDCOL);
				setMap(BRDSID);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRDSPAC]) {

				FormMenuChoice = LBRDSPAC;
				sidwnd(ValueWindow[LBRDSPAC]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRDLEN]) {

				FormMenuChoice = LBRDLEN;
				sidwnd(ValueWindow[LBRDLEN]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRDSIZ]) {

				FormMenuChoice = LBRDSIZ;
				sidwnd(ValueWindow[LBRDSIZ]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LAPCOL]) {

				FormMenuChoice = LAPCOL;
				setMap(SIDCOL);
				setMap(APSID);
				sidwnd(ValueWindow[LAPCOL]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBCSIZ]) {

				FormMenuChoice = LBCSIZ;
				sidwnd(ValueWindow[LBCSIZ]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBSTRT]) {

				l_code = SelectedForm->attribute&SBLNT;
				if (l_code)
					SelectedForm->attribute &= NSBLNT;
				else
					SelectedForm->attribute |= SBLNT;
				refil();
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LBFIN]) {

				l_code = SelectedForm->attribute&FBLNT;
				if (l_code)
					SelectedForm->attribute &= NFBLNT;
				else
					SelectedForm->attribute |= FBLNT;
				refil();
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LFRMANG]) {

				FormMenuChoice = LFRMANG;
				sidwnd(ValueWindow[LFRMANG]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRDPIC]) {

				FormMenuChoice = LBRDPIC;
				sidwnd(ValueWindow[LBRDPIC]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRDUND]) {

				SelectedForm->edgeType ^= EGUND;
				refil();
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LSACANG]) {

				FormMenuChoice = LSACANG;
				sidwnd(ValueWindow[LSACANG]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFRMFAZ]) {

				FormMenuChoice = LFRMFAZ;
				sidwnd(ValueWindow[LFRMFAZ]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBRDPOS]) {

				FormMenuChoice = LBRDPOS;
				sidwnd(ValueWindow[LBRDPOS]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LBFILSQR]) {

				dubit(AT_SQR);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LFSTRT]) {

				dubit(AT_STRT);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LDSTRT])
			{
				FormMenuChoice = LDSTRT;
				sidwnd(ValueWindow[LDSTRT]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LFEND]) {

				dubit(AT_END);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LDEND])
			{
				FormMenuChoice = LDEND;
				sidwnd(ValueWindow[LDEND]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LWALK])
			{
				dubit(AT_WALK);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LCWLK])
			{
				dubit(AT_CWLK);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LUND])
			{
				dubit(AT_UND);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LMAXFIL]) {

				FormMenuChoice = LMAXFIL;
				sidwnd(ValueWindow[LMAXFIL]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LMINFIL]) {

				FormMenuChoice = LMINFIL;
				sidwnd(ValueWindow[LMINFIL]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LMAXBRD]) {

				FormMenuChoice = LMAXBRD;
				sidwnd(ValueWindow[LMAXBRD]);
				goto didat;
			}
			if (Msg.hwnd == ValueWindow[LMINBRD]) {

				FormMenuChoice = LMINBRD;
				sidwnd(ValueWindow[LMINBRD]);
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
			FormIndex++;
			setMap(INIT);
			rstfrm();
			fvars(ClosestFormToCursor);
			refil();
			rstMap(FUNCLP);
			if (rstMap(FPSEL))
				frmout(FormIndex - 1);
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
		if (px2stch() && !(chkMap(SIZSEL) && chkMsgs(Msg.pt, ChangeThreadSizeWin[0], ChangeThreadSizeWin[2]))) {

			unpat();
			if (rstMap(ROTAT)) {

				RotateBoxToCursorLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
				RotateBoxToCursorLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				padj.x = RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x;
				padj.y = RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y;
				if (hypot(padj.x, padj.y) < CLOSENUF) {

					px2stch();
					RotationCenter.x = SelectedPoint.x;
					RotationCenter.y = SelectedPoint.y;
					setMap(MOVCNTR);
					unrot();
					ritrot();
				} else {

					if (padj.x)
						RotationHandleAngle = -atan2(padj.y, padj.x);
					else {

						if (padj.y > 0)
							RotationHandleAngle = PI / 2;
						else
							RotationHandleAngle = -PI / 2;
					}
					setMap(ROTUSHO);
					durotu();
					setMap(ROTCAPT);
				}
				return 1;
			}
			if (rstMap(CLPSHO)) {

				savdo();
				if ((rstMap(SELBOX) || rstMap(INSRT)) && ClosestPointIndex != (unsigned)PCSHeader.stitchCount - 1)
					lodclp(ClosestPointIndex);
				else
					lodclp(PCSHeader.stitchCount);
				rngadj();
				selRct(&tbig);
				FormOutlineRect[0].x = FormOutlineRect[6].x = FormOutlineRect[7].x = FormOutlineRect[8].x = trct.left;
				FormOutlineRect[1].x = FormOutlineRect[5].x = midl(trct.right, trct.left);
				FormOutlineRect[0].y = FormOutlineRect[1].y = FormOutlineRect[2].y = FormOutlineRect[8].y = trct.top;
				FormOutlineRect[3].y = FormOutlineRect[7].y = midl(trct.top, trct.bottom);
				FormOutlineRect[4].y = FormOutlineRect[5].y = FormOutlineRect[6].y = trct.bottom;
				FormOutlineRect[2].x = FormOutlineRect[3].x = FormOutlineRect[4].x = trct.right;
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (rstMap(BOXZUM)) {

				setMap(BZUMIN);
				ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
				ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
				ZoomBoxLine[4].y = ZoomBoxLine[0].y - 1;
				px2stch();
				ZoomBoxOrigin.x = SelectedPoint.x;
				ZoomBoxOrigin.y = SelectedPoint.y;
				setMap(VCAPT);
				return 1;
			}
			if (FormIndex && !chkMap(FRMOF)) {

				if (!chkMap(INSRT)) {

					if (rstMap(FORMSEL))
						ritfrct(ClosestFormToCursor, StitchWindowDC);
					if (closfrm()) {

						setMap(FRMPMOV);
						fvars(ClosestFormToCursor);
						frmovlin();
						return 1;
					}
				}
			}
			if (chkMap(INIT)) {

				unlin();
				savdo();
				if (chkMap(INSRT) && PCSHeader.stitchCount < MAXSEQ) {

					px2stch();
					l_code = (ActiveColor | USMSK | (ActiveLayer << LAYSHFT) | NOTFRM)&NKNOTMSK;
					if (chkMap(LIN1)) {

						if (chkMap(BAKEND)) {

							xlin1();
							ind = PCSHeader.stitchCount;
							StitchBuffer[ind].x = SelectedPoint.x;
							StitchBuffer[ind].y = SelectedPoint.y;
							StitchBuffer[ind].attribute = l_code;
							duzrat();
							stch2px1(ind);
							InsertLine[0].x = StitchCoordinatesPixels.x;
							InsertLine[0].y = StitchCoordinatesPixels.y;
							InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
							InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
							PCSHeader.stitchCount++;
							coltab();
							setMap(RESTCH);
							return 1;
						} else {

							xlin1();
							for (ind = PCSHeader.stitchCount; ind; ind--) {

								StitchBuffer[ind].attribute = StitchBuffer[ind - 1].attribute;
								StitchBuffer[ind].x = StitchBuffer[ind - 1].x;
								StitchBuffer[ind].y = StitchBuffer[ind - 1].y;
							}
							StitchBuffer[0].attribute = l_code;
							StitchBuffer[ind].attribute &= (~KNOTMSK);
							StitchBuffer[0].x = SelectedPoint.x;
							StitchBuffer[0].y = SelectedPoint.y;
							stch2px1(ind);
							InsertLine[0].x = StitchCoordinatesPixels.x;
							InsertLine[0].y = StitchCoordinatesPixels.y;
							InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
							InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
							PCSHeader.stitchCount++;
							coltab();
							setMap(RESTCH);
							return 1;
						}
					} else {

						xlin();
						if (StitchBuffer[ClosestPointIndex].attribute&ALTYPMSK&&StitchBuffer[ClosestPointIndex + 1].attribute&ALTYPMSK)
						{
							if ((StitchBuffer[ClosestPointIndex].attribute&FRMSK) == (StitchBuffer[ClosestPointIndex + 1].attribute&FRMSK))
								l_code = StitchBuffer[ClosestPointIndex].attribute | USMSK;
						}
						ind = PCSHeader.stitchCount;
						do {

							StitchBuffer[ind].x = StitchBuffer[ind - 1].x;
							StitchBuffer[ind].y = StitchBuffer[ind - 1].y;
							StitchBuffer[ind].attribute = StitchBuffer[ind - 1].attribute;
							ind--;
						} while (ind > (long)ClosestPointIndex);
						PCSHeader.stitchCount++;
						ClosestPointIndex++;
						StitchBuffer[ClosestPointIndex].x = SelectedPoint.x;
						StitchBuffer[ClosestPointIndex].y = SelectedPoint.y;
						StitchBuffer[ClosestPointIndex].attribute = l_code;
						xlin();
						InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
						InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
						stch2px1(ClosestPointIndex);
						InsertLine[0].x = StitchCoordinatesPixels.x;
						InsertLine[0].y = StitchCoordinatesPixels.y;
						stch2px1(ClosestPointIndex + 1);
						InsertLine[2].x = StitchCoordinatesPixels.x;
						InsertLine[2].y = StitchCoordinatesPixels.y;
						coltab();
						setMap(RESTCH);
						ritnum(STR_NUMSEL, ClosestPointIndex);
						return 1;
					}
				}
				if ((!chkMap(HIDSTCH)) && closPnt1(&ClosestPointIndexClone)) {

					ClosestPointIndex = ClosestPointIndexClone;
					unbox();
					unboxs();
					setbak(ThreadSizePixels[StitchBuffer[ClosestPointIndex].attribute & 0xf] + 3);
					LinePoints = new POINT[3];
					LineIndex = 0;
					SetROP2(StitchWindowDC, R2_NOTXORPEN);
					if (ClosestPointIndex == 0) {

						if (ZoomFactor < STCHBOX) {

							SelectObject(StitchWindowDC, LinePen);
							stchbox(0, StitchWindowDC);
							stchbox(1, StitchWindowDC);
						}
						SetROP2(StitchWindowDC, R2_COPYPEN);
						drwLin(0, 2, BackgroundPen);
					} else {

						if (ClosestPointIndex == (unsigned)PCSHeader.stitchCount - 1) {

							if (ZoomFactor < STCHBOX) {

								SelectObject(StitchWindowDC, LinePen);
								stchbox(PCSHeader.stitchCount - 1, StitchWindowDC);
								stchbox(PCSHeader.stitchCount - 2, StitchWindowDC);
							}
							SetROP2(StitchWindowDC, R2_COPYPEN);
							drwLin(PCSHeader.stitchCount - 2, 2, BackgroundPen);
						} else {

							if (ZoomFactor < STCHBOX) {

								SelectObject(StitchWindowDC, LinePen);
								stchbox(ClosestPointIndex - 1, StitchWindowDC);
								stchbox(ClosestPointIndex, StitchWindowDC);
								stchbox(ClosestPointIndex + 1, StitchWindowDC);
							}
							SetROP2(StitchWindowDC, R2_COPYPEN);
							drwLin(ClosestPointIndex - 1, 3, BackgroundPen);
						}
					}
					delete[] LinePoints;
					MoveLine0[1].x = MoveLine1[0].x = Msg.pt.x - StitchWindowOrigin.x;
					MoveLine0[1].y = MoveLine1[0].y = Msg.pt.y - StitchWindowOrigin.y;
					if (ClosestPointIndex <= 0)
						rstMap(ISDWN);
					else {

						setMap(ISDWN);
						stch2px1(ClosestPointIndex - 1);
						MoveLine0[0].x = StitchCoordinatesPixels.x;
						MoveLine0[0].y = StitchCoordinatesPixels.y;
					}
					tlng = PCSHeader.stitchCount - 1;
					if (ClosestPointIndex >= (unsigned)tlng)
						rstMap(ISUP);
					else {

						setMap(ISUP);
						stch2px1(ClosestPointIndex + 1);
						MoveLine1[1].x = StitchCoordinatesPixels.x;
						MoveLine1[1].y = StitchCoordinatesPixels.y;
					}
					dulin();
					SetCapture(ThrEdWindow);
					setMap(CAPT);
					ritnum(STR_NUMSEL, ClosestPointIndex);
				}
			} else {

				if (px2stch()) {

					savdo();
					PCSHeader.stitchCount = 1;
					InsertLine[0].x = Msg.pt.x - StitchWindowOrigin.x;
					InsertLine[0].y = Msg.pt.y - StitchWindowOrigin.y;
					StitchBuffer[0].attribute = USMSK | ActiveColor | LayerIndex | NOTFRM;
					StitchBuffer[0].x = SelectedPoint.x;
					StitchBuffer[0].y = SelectedPoint.y;
					ColorChanges = 1;
					ColorChangeTable[0].colorIndex = ActiveColor;
					ColorChangeTable[0].stitchIndex = 0;
					ColorChangeTable[1].stitchIndex = 1;
					setMap(LIN1);
					setMap(INSRT);
					SetCapture(ThrEdWindow);
					ClosestPointIndex = 1;
					setMap(INIT);
					setMap(BAKEND);
				}
			}
			ritot(PCSHeader.stitchCount);
			setMap(BOXSLCT);
			setMap(BZUMIN);
			setMap(NOSEL);
			ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
			ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
			ZoomBoxLine[4].y = ZoomBoxLine[0].y - 1;
			px2stch();
			ZoomBoxOrigin.x = SelectedPoint.x;
			ZoomBoxOrigin.y = SelectedPoint.y;
			setMap(VCAPT);
			return 1;
		}
		if (Msg.hwnd == ButtonWin[HBOXSEL]) {

			boxsel();
			return 1;
		}
		if (Msg.hwnd == ButtonWin[HUPTO]) {

			toglup();
			return 1;
		}
		if (Msg.hwnd == ButtonWin[HHID]) {

			toglHid();
			return 1;
		}
		if (chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15])) {

			if (Msg.message == WM_LBUTTONDOWN) {

				savdo();
				l_code = ActiveColor;
				ActiveColor = VerticalIndex & 0xf;
				redraw(UserColorWin[l_code]);
				redraw(UserColorWin[ActiveColor]);
				if (chkMap(HID)) {

					rstMap(SELBOX);
					rstMap(GRPSEL);
					rstMap(SCROS);
					rstMap(ECROS);
					setMap(RESTCH);
					redraw(ButtonWin[HHID]);
				} else {

					if (SelectedFormCount) {

						nucols();
						coltab();
						setMap(RESTCH);
						return 1;
					} else {

						if (chkMap(FORMSEL)) {

							SelectedForm = &FormList[ClosestFormToCursor];
							if (SelectedForm->fillType || SelectedForm->edgeType || SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) {

								if (FormList[ClosestFormToCursor].fillType) {

									FormList[ClosestFormToCursor].fillColor = ActiveColor;
									if (FormList[ClosestFormToCursor].fillType == FTHF)
										FormList[ClosestFormToCursor].fillInfo.feather.color = ActiveColor;
								}
								if (FormList[ClosestFormToCursor].edgeType)
								{
									if (FormList[ClosestFormToCursor].edgeType == EDGEAPPL)
									{
										FormList[ClosestFormToCursor].borderColor &= 0xf0;
										FormList[ClosestFormToCursor].borderColor |= ActiveColor;
									} else
										FormList[ClosestFormToCursor].borderColor = ActiveColor;
								}
								if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
									FormList[ClosestFormToCursor].underlayColor = ActiveColor;
								l_code = ClosestFormToCursor << FRMSHFT;
								for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

									ine = StitchBuffer[ind].attribute;
									if (ine&ALTYPMSK && (ine&FRMSK) == l_code && (ine&TYPMSK) != TYPMSK)
									{
										ine &= NCOLMSK;
										ine |= ActiveColor;
										StitchBuffer[ind].attribute = ine;
									}
								}
								coltab();
								setMap(RESTCH);
							}
						} else {

							if (chkMap(GRPSEL)) {

								for (ind = GroupStartStitch + 1; ind <= (long)GroupEndStitch; ind++) {

									StitchBuffer[ind].attribute &= 0xfffffff0;
									StitchBuffer[ind].attribute |= ActiveColor;
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
		if (chkMsgs(Msg.pt, UserColorWin[0], UserColorWin[15])) {

			if (Msg.message == WM_LBUTTONDOWN&&nuCol(UserColor[VerticalIndex])) {

				savdo();
				UserColor[VerticalIndex] = ColorStruct.rgbResult;
				UserPen[VerticalIndex] = nuPen(UserPen[VerticalIndex], 1, UserColor[VerticalIndex]);
				UserColorBrush[VerticalIndex] = nuBrush(UserColorBrush[VerticalIndex], UserColor[VerticalIndex]);
				redraw(UserColorWin[VerticalIndex]);
				setMap(RESTCH);
			}
			return 1;
		}
		if (chkMsgs(Msg.pt, ThreadSizeWin[0], ThreadSizeWin[15])) {

			if (Msg.message == WM_LBUTTONDOWN) {

				savdo();
				ThreadSizeSelected = VerticalIndex;
				for (ind = 0; ind < 3; ind++)
					ChangeThreadSizeWin[ind] = nuSiz(ind);
				setMap(SIZSEL);
			}
			return 1;
		}
	}
	switch (Msg.message) {

	case WM_TIMER:

		if (chkMap(RUNPAT) && !Msg.wParam)
			stchout();
		break;

	case WM_CHAR:

		if (isgraph(Msg.wParam))
			nuthum(lchr(Msg.wParam));
		break;

	case WM_KEYDOWN:

		l_code = Msg.wParam & 0xffff;
		if (chkMap(TXTRED))
		{
			txtkey(l_code);
			return 1;
		}
		fvars(ClosestFormToCursor);

#if LANG==GRM
		if (l_code >= 0x30 && l_code <= 0x39) {

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				switch (l_code) {

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
		// ToDo - value passed to duform is wierd because it is dependant on order of enumeration of the form types.
		//        and value 'SAT' throws it off
		if (chkMap(FORMIN)) {

			if (GetKeyState(VK_CONTROL) & 0X8000)
				return 1;

			switch (l_code) {

			case 'E':

				rstMap(FORMIN);
				unmsg();
				duform(FRMLINE - 1);
				return 1;

			case 'F':

				rstMap(FORMIN);
				unmsg();
				duform(FRMFPOLY - 1);
				return 1;

			case 'R':

				duform(FRMRPOLY - 1);
				return 1;

			case 'S':

				duform(FRMSTAR - 1);
				return 1;

			case 'A':

				duform(FRMSPIRAL - 1);
				return 1;

			case 'H':

				duform(FRMHEART - 2);
				return 1;

			case 'L':

				duform(FRMLENS - 2);
				return 1;

			case 'G':

				duform(FRMEGG - 2);
				return 1;

			case 'T':

				duform(FRMTEAR - 2);
				return 1;

			case 'Z':

				duform(FRMZIGZAG - 2);
				return 1;

			case 'W':

				duform(FRMWAVE - 2);
				return 1;

			case 'D':

				duform(FRMDAISY - 2);
				return 1;
			}
		}
		if (chkMap(FILMSG)) {

			if (l_code == VK_RETURN || l_code == 0xc0) {

				savdo();
				unfil();
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (rstMap(MOVMSG)) {

			if (l_code == VK_RETURN || l_code == 0xc0) {

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

			if (l_code == VK_RETURN || l_code == 0xc0) {

				deldir();
				return 1;
			}
		}
		if (rstMap(DELSFRMS)) {

			if (l_code == 'S' || l_code == VK_RETURN || l_code == 0xc0) {

				savdo();
				if (l_code == 'S')
					setMap(DELTO);
				else
					rstMap(DELTO);
				delsfrms(l_code);
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (rstMap(DELFRM)) {

			if (l_code == 'S' || l_code == VK_RETURN || l_code == 0xc0) {

				savdo();
				if (l_code == 'S')
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

			switch (l_code) {

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

				ThumbnailIndex = 0;
				nuthsel();
				break;

			case VK_END:

				ThumbnailIndex = ThumbnailCount;
				nuthbak(4);
				break;

			case 8:			//backspace
			case VK_LEFT:

				bakthum();
				break;

			default:

				TranslateMessage(&Msg);
			}
			return 1;
		}
		if (chkMap(FSETFSPAC) || chkMap(GTWLKIND))
		{
			if (l_code == 189 || l_code == 109)
			{
				*MsgBuffer = '-';
				MsgIndex = 1;
				MsgBuffer[1] = 0;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;
			}
		}
		if (FormMenuChoice || PreferenceIndex) {

			if (chkminus(l_code)) {

				MsgIndex = 1;
				SideWindowEntryBuffer[0] = '-';
				SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				return 1;
			}
			if (dunum(l_code)) {

				if (PreferenceIndex == PSHO + 1 || PreferenceIndex == PBOX + 1) {

					MsgBuffer[0] = (TCHAR)NumericCode;
					MsgBuffer[1] = 0;
					if (PreferenceIndex == PSHO + 1) {

						ShowStitchThreshold = unthrsh(NumericCode - 0x30);
						SetWindowText(ValueWindow[PSHO], MsgBuffer);
					} else {

						StitchBoxesThreshold = unthrsh(NumericCode - 0x30);
						SetWindowText(ValueWindow[PBOX], MsgBuffer);
					}
					unsid();
				} else {

					SideWindowEntryBuffer[MsgIndex++] = NumericCode;
					SideWindowEntryBuffer[MsgIndex] = 0;
					SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				}
				return 1;
			}
			switch (l_code) {

			case 0x6e://numpat period
			case 0xbe://period

				SideWindowEntryBuffer[MsgIndex++] = '.';
				SideWindowEntryBuffer[MsgIndex] = 0;
				SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				return 1;

			case 8:	//backspace

				if (MsgIndex) {

					SideWindowEntryBuffer[--MsgIndex] = 0;
					SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				}
				return 1;

			case VK_RETURN:

				chknum();
				FormMenuChoice = 0;
				PreferenceIndex = 0;
				return 1;
			}
		}
		if (l_code == 'I') {

			movi();
			LastKeyCode = 'I';
			return 1;
		}
		if (l_code == 'Q'&&LastKeyCode == 'Q')
			unpat();
		LastKeyCode = l_code;
		if (chkMap(NUMIN)) {

			if (chkMap(SCLPSPAC) && l_code == 0xbd && !MsgIndex)
			{
				MsgBuffer[0] = '-';
				MsgBuffer[1] = 0;
				MsgIndex = 1;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;
			}
			if (dunum(l_code)) {

				if (chkMap(TRNIN0)) {

					TraceInputBuffer[MsgIndex++] = NumericCode;
					TraceInputBuffer[MsgIndex] = 0;
					ind = atoi(TraceInputBuffer);
					switch (MsgIndex) {

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
					redraw(TraceNumberInput);
				} else {

					MsgBuffer[MsgIndex++] = NumericCode;
					MsgBuffer[MsgIndex] = 0;
					SetWindowText(GeneralNumberInputBox, MsgBuffer);
				}
				return 1;
			}
			switch (l_code) {

			case 0x6e://numpad period
			case 0xbe://period

				MsgBuffer[MsgIndex++] = '.';
				MsgBuffer[MsgIndex] = 0;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;

			case 8:	//backspace

				if (MsgIndex) {

					MsgIndex--;
					if (chkMap(TRNIN0)) {

						TraceInputBuffer[MsgIndex] = 0;
						redraw(TraceNumberInput);
					} else {

						MsgBuffer[MsgIndex] = 0;
						SetWindowText(GeneralNumberInputBox, MsgBuffer);
					}
				}
				return 1;

			case VK_RETURN:

				if (chkMap(TRNIN0))
					dutrnum0(atoi(TraceInputBuffer));
				else {

					if (chkMap(TRNIN1))
						dutrnum1();
				}
				break;
			}
		}
		if (l_code == 8 && BufferIndex) {

			StitchEntryBuffer[--BufferIndex] = 0;
			butxt(HNUM, StitchEntryBuffer);
			ClosestPointIndex = atoi(StitchEntryBuffer);
			movbox();
			return 1;
		}
		if (!chkMap(WASTRAC) && dunum(l_code)) {

			if (BufferIndex > BufferDigitCount - 1)
				BufferIndex = 0;
			if (StitchEntryBuffer[0] == '0'&&BufferIndex)
				BufferIndex--;
			StitchEntryBuffer[BufferIndex++] = NumericCode;
			StitchEntryBuffer[BufferIndex] = 0;
			ClosestPointIndex = atoi(StitchEntryBuffer);
			if (ClosestPointIndex > (unsigned)PCSHeader.stitchCount - 1) {

				sprintf_s(StitchEntryBuffer, sizeof(StitchEntryBuffer), "%d", PCSHeader.stitchCount - 1);
				ClosestPointIndex = PCSHeader.stitchCount - 1;
			}
			butxt(HNUM, StitchEntryBuffer);
			movbox();
			rstMap(NUMIN);
			return 1;
		}
		BufferIndex = 0;
		unmsg();
		switch (l_code) {

		case VK_ESCAPE:

		thumout:;
			chkbit();
			duhbit(MF_UNCHECKED);
			unthum();
			rstMap(MOVSET);
			rstMap(HID);
			rstMap(FRMOF);
			rstMap(THRDS);
			redraw(ButtonWin[HHID]);
			CheckMenuItem(MainMenu, ID_VUTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
			rstMap(COL);
			CheckMenuItem(MainMenu, ID_VUSELTHRDS, MF_BYCOMMAND | MF_UNCHECKED);
			setMap(DUMEN);
			rstMap(RUNPAT);
			rstMap(WASPAT);
			rstMap(WASBLAK);
			rstMap(GTUANG);
			rstMap(GTUSPAC);
			rstMap(GTWLKIND);
			rstMap(GTWLKLEN);
			DestroyWindow(SpeedScrollBar);
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
			if (PCSHeader.stitchCount == 1)
				PCSHeader.stitchCount = 0;
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
			if (BufferIndex) {

				BufferIndex = 0;
				StitchEntryBuffer[0] = 0;
				ClosestPointIndex = 0;
				movbox();
			}
			butxt(HNUM, "");
			return 1;

		case 0xc0:		//`

			if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState(VK_MENU) & 0x8000) {

				BackgroundColor = 0x505050;
				BackgroundPen = nuPen(BackgroundPen, 1, BackgroundColor);
				BackgroundPenWidth = 1;
				DeleteObject(BackgroundBrush);
				BackgroundBrush = CreateSolidBrush(BackgroundColor);
				if (*PCSBMPFileName)
					bfil();
				setMap(RESTCH);
			} else {
#if PESACT
				//				IniFile.auxFileType=AUXPES;
				//				strcpy_s(WorkingFileName,"u:\\mrd\\t.thr");
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
				nudgfn(0, -IniFile.cursorNudgeStep);
			else
				seldwn();
			break;

		case VK_UP:

			if (GetKeyState(VK_CONTROL) & 0X8000)
				nudgfn(0, IniFile.cursorNudgeStep);
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

				ZoomFactor = ZoomMin;
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
					PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
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

			if (SelectedFormCount)
			{
				PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
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

			if (GetKeyState(VK_CONTROL) & 0X8000 && OpenClipboard(ThrEdWindow)) {

				ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
				ClipMemory = GetClipboardData(ThrEdClip);
				if (ClipMemory) {

					ClipPointer = GlobalLock(ClipMemory);
					ClipFormVerticesData = (FORMVERTEXCLIP*)ClipPointer;
					if (ClipFormVerticesData->clipType == CLP_FRMPS) {

						duzrat();
						ind = sizeof(FORMVERTEXCLIP) + (ClipFormVerticesData->vertexCount + 1) * sizeof(fPOINT);
						MoveMemory(&BSequence, ClipPointer, ind);
						GlobalUnlock(ClipMemory);
						ClipFormVerticesData = (FORMVERTEXCLIP*)&BSequence;
						if (chkMap(FRMPSEL)) {

							pchr = (unsigned char*)&BSequence;
							fvars(ClosestFormToCursor);
							InterleaveSequence[0].x = CurrentFormVertices[ClosestVertexToCursor].x;
							InterleaveSequence[0].y = CurrentFormVertices[ClosestVertexToCursor].y;
							l_clipData = (fPOINT*)&ClipFormVerticesData[1];
							for (ind = 0; ind <= ClipFormVerticesData->vertexCount; ind++) {

								InterleaveSequence[ind + 1].x = l_clipData[ind].x;
								InterleaveSequence[ind + 1].y = l_clipData[ind].y;
							}
							ine = nxt(ClosestVertexToCursor);
							ind++;
							InterleaveSequence[ind].x = CurrentFormVertices[ine].x;
							InterleaveSequence[ind].y = CurrentFormVertices[ine].y;
							OutputIndex = ind + 1;
							FormVerticesAsLine = (POINT*)&InterleaveSequence[OutputIndex];
							setpclp();
							setMap(FPUNCLP);
							setMap(SHOP);
							dupclp();
						} else {

							FormMoveDelta.x = FormMoveDelta.y = 0;
							setMap(FUNCLP);
							ClosestFormToCursor = FormIndex;
							SelectedForm = &FormList[FormIndex];
							FillMemory(SelectedForm, sizeof(FRMHED), 0);
							SelectedForm->type = FRMLINE;
							SelectedForm->sides = ClipFormVerticesData->vertexCount + 1;
							SelectedForm->vertices = adflt(SelectedForm->sides);
							fvars(ClosestFormToCursor);
							MoveMemory(SelectedForm->vertices, &ClipFormVerticesData[1], sizeof(fPOINT)*SelectedForm->sides);
							setMap(INIT);
							NewFormVertexCount = SelectedForm->sides;
							unfrm();
							setmfrm();
							setMap(SHOFRM);
							dufrm();
						}
						return 1;
					}
					ClipFormsHeader = (FORMSCLIP*)ClipPointer;
					if (ClipFormsHeader->clipType == CLP_FRMS) {

						ClipFormsCount = ClipFormsHeader->formCount;
						tfrm = (FRMHED*)&ClipFormsHeader[1];
						for (ind = 0; ind < ClipFormsCount; ind++) {

							frmcpy(&FormList[FormIndex + ind], &tfrm[ind]);
							FormList[FormIndex + ind].attribute = (FormList[FormIndex + ind].attribute&NFRMLMSK) | (ActiveLayer << 1);
						}
						CurrentFormVertices = (fPOINT*)&tfrm[ind];
						inf = 0;
						for (ind = 0; ind < ClipFormsCount; ind++) {

							SelectedForm = &FormList[FormIndex + ind];
							SelectedForm->vertices = adflt(SelectedForm->sides);
							for (ine = 0; ine < SelectedForm->sides; ine++) {

								SelectedForm->vertices[ine].x = CurrentFormVertices[inf].x;
								SelectedForm->vertices[ine].y = CurrentFormVertices[inf++].y;
							}
						}
						l_sac = (SATCON*)&CurrentFormVertices[inf];
						inf = 0;
						for (ind = 0; ind < ClipFormsCount; ind++) {

							SelectedForm = &FormList[FormIndex + ind];
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
						for (ind = 0; ind < ClipFormsCount; ind++) {

							SelectedForm = &FormList[FormIndex + ind];
							if (isclpx(FormIndex + ind)) {

								SelectedForm->angleOrClipData.clip = adclp(SelectedForm->lengthOrCount.clipCount);
								for (ine = 0; (unsigned)ine < SelectedForm->lengthOrCount.clipCount; ine++) {

									SelectedForm->angleOrClipData.clip[ine].x = l_clipData[inf].x;
									SelectedForm->angleOrClipData.clip[ine].y = l_clipData[inf++].y;
								}
							}
							if (iseclpx(FormIndex + ind)) {

								SelectedForm->borderClipData = adclp(SelectedForm->clipEntries);
								for (ine = 0; ine < SelectedForm->clipEntries; ine++) {

									SelectedForm->borderClipData[ine].x = l_clipData[inf].x;
									SelectedForm->borderClipData[ine].y = l_clipData[inf++].y;
								}
							}
						}
						pts = (TXPNT*)&l_clipData[inf];
						ptx = &TexturePointsBuffer[TextureIndex];
						ine = 0;
						for (ind = 0; ind < ClipFormsCount; ind++)
						{
							if (istx(FormIndex + ind))
							{
								SelectedForm = &FormList[FormIndex + ind];
								ine += SelectedForm->fillInfo.texture.count;
								SelectedForm->fillInfo.texture.index += TextureIndex;
								MoveMemory(ptx, pts, (SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count - TextureIndex) * sizeof(TXPNT));
							}
						}
						TextureIndex += ine;
						GlobalUnlock(ClipMemory);
						SelectedFormsRect.top = SelectedFormsRect.left = 0x7fffffff;
						SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
						ratsr();
						for (OutputIndex = 0; OutputIndex < (unsigned)ClipFormsCount; OutputIndex++) {

#if  __UseASM__
							fselrct(OutputIndex + FormIndex);
#else
							fselrct(OutputIndex + FormIndex);
#endif
							SelectedFormList[OutputIndex] = OutputIndex + FormIndex;
						}
						SelectedFormCount = ClipFormsCount;
						SelectedFormsSize.x = SelectedFormsRect.right - SelectedFormsRect.left;
						SelectedFormsSize.y = SelectedFormsRect.bottom - SelectedFormsRect.top;
						setMap(INIT);
						FormLines[0].x = FormLines[3].x = FormLines[4].x = SelectedFormsRect.left;
						FormLines[1].x = FormLines[2].x = SelectedFormsRect.right;
						FormLines[0].y = FormLines[1].y = FormLines[4].y = SelectedFormsRect.top;
						FormLines[2].y = FormLines[3].y = SelectedFormsRect.bottom;
						setMap(SHOSTRTCH);
						strtchbox();
						FormMoveDelta.x = ((SelectedFormsRect.right - SelectedFormsRect.left) >> 1);
						FormMoveDelta.y = ((SelectedFormsRect.bottom - SelectedFormsRect.top) >> 1);
						setMap(MOVFRMS);
						setMap(FUNSCLP);
					} else {

						ClipFormHeader = (FORMCLIP*)ClipPointer;
						if (ClipFormHeader->clipType == CLP_FRM) {

							FormMoveDelta.x = FormMoveDelta.y = 0;
							setMap(FUNCLP);
							ClosestFormToCursor = FormIndex;
							fvars(FormIndex);
							frmcpy(&FormList[FormIndex], &ClipFormHeader->form);
							FormList[FormIndex].attribute = (FormList[FormIndex].attribute&NFRMLMSK) | (ActiveLayer << 1);
							SelectedForm->vertices = adflt(FormList[FormIndex].sides);
							CurrentFormVertices = (fPOINT*)&ClipFormHeader[1];
							mvflpnt(&SelectedForm->vertices[0], &CurrentFormVertices[0], SelectedForm->sides);
							l_sac = (SATCON*)&CurrentFormVertices[SelectedForm->sides];
							ind = 0;
							if (SelectedForm->type == SAT&&SelectedForm->satinGuideCount) {

								SelectedForm->satinOrAngle.sac = adsatk(SelectedForm->satinGuideCount);
								mvsatk(&SelectedForm->satinOrAngle.sac[0], &l_sac[0], SelectedForm->satinGuideCount);
							}
							l_clipData = (fPOINT*)&l_sac[ind];
							inf = 0;
							if (isclpx(FormIndex)) {

								SelectedForm->angleOrClipData.clip = adclp(SelectedForm->lengthOrCount.clipCount);
								mvflpnt(SelectedForm->angleOrClipData.clip, l_clipData, SelectedForm->lengthOrCount.clipCount);
								inf += SelectedForm->lengthOrCount.clipCount;
							}
							if (iseclpx(FormIndex)) {

								l_clipData = (fPOINT*)&l_clipData[inf];
								SelectedForm->borderClipData = adclp(SelectedForm->clipEntries);
								mvflpnt(SelectedForm->borderClipData, l_clipData, SelectedForm->clipEntries);
								inf += SelectedForm->clipEntries;
							}
							pts = (TXPNT*)&l_clipData[inf];
							if (istx(FormIndex))
							{
								SelectedForm->fillInfo.texture.index = TextureIndex;
								ptx = adtx(SelectedForm->fillInfo.texture.count);
								MoveMemory(ptx, pts, SelectedForm->fillInfo.texture.count * sizeof(TXPNT));
							}
						}
						GlobalUnlock(ClipMemory);
						setMap(INIT);
						NewFormVertexCount = SelectedForm->sides;
						if (SelectedForm->type != FRMLINE)
							NewFormVertexCount++;
						unfrm();
						duzrat();
						setmfrm();
						setMap(SHOFRM);
						dufrm();
					}
				} else {

					Clip = RegisterClipboardFormat(PcdClipFormat);
					ClipMemory = GetClipboardData(Clip);
					if (ClipMemory) {

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

					GroupStitchIndex = 0;
					dubx();
					setMap(GRPSEL);
				} else {

					if (chkMap(GRPSEL)) {

						if (GroupStitchIndex > ClosestPointIndex)
							ClosestPointIndex = 0;
						else
							GroupStitchIndex = 0;
					} else {

						setMap(SELBOX);
						setMap(RESTCH);
					}
				}
				grpAdj();
				redraw(ColorBar);
				return 1;
			}
			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (rstMap(SELBOX)) {

					dubx();
					setMap(GRPSEL);
					ind = pt2colInd(ClosestPointIndex);
				} else
					ind = pt2colInd(GroupStitchIndex);
				ind--;
				GroupStitchIndex = ColorChangeTable[ind].stitchIndex;
				grpAdj();
				redraw(ColorBar);
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000) {

					stch2px1(0);
					endpnt();
					rstMap(BAKEND);
					setMap(RESTCH);
				} else {

					if (chkMap(SELBOX)) {

						ind = pt2colInd(ClosestPointIndex);
						if (ind)
							ind--;
						ClosestPointIndex = ColorChangeTable[ind].stitchIndex;
					} else {

						if (chkMap(LENSRCH)) {

							setsrch(SmallestStitchIndex);
							return 1;
						} else
							ClosestPointIndex = 0;
					}
					movbox();
				}
			}
			break;

		case VK_END:

			if (GetKeyState(VK_SHIFT) & 0X8000 && GetKeyState(VK_CONTROL) & 0X8000) {

				if (rstMap(SELBOX)) {

					GroupStitchIndex = PCSHeader.stitchCount - 1;
					dubx();
					setMap(GRPSEL);
				} else {

					if (chkMap(GRPSEL)) {

						if (GroupStitchIndex > ClosestPointIndex)
							GroupStitchIndex = PCSHeader.stitchCount - 1;
						else
							ClosestPointIndex = PCSHeader.stitchCount - 1;
					} else {

						setMap(SELBOX);
						ClosestPointIndex = PCSHeader.stitchCount - 1;
						setMap(RESTCH);
						return 1;
					}
				}
				grpAdj();
				redraw(ColorBar);
				break;
			}
			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (rstMap(SELBOX)) {

					dubx();
					setMap(GRPSEL);
					ind = pt2colInd(ClosestPointIndex);
				} else
					ind = pt2colInd(GroupStitchIndex);
				GroupStitchIndex = ColorChangeTable[ind].stitchIndex - 1;
				grpAdj();
				redraw(ColorBar);
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000) {

					stch2px1(PCSHeader.stitchCount - 1);
					endpnt();
					setMap(BAKEND);
					setMap(RESTCH);
				} else {

					if (chkMap(SELBOX)) {

						ind = pt2colInd(ClosestPointIndex);
						ClosestPointIndex = ColorChangeTable[ind].stitchIndex - 1;
					} else {

						if (chkMap(LENSRCH)) {

							setsrch(LargestStitchIndex);
							return 1;
						} else
							ClosestPointIndex = PCSHeader.stitchCount - 1;
					}
					movbox();
				}
			}
			break;

		case VK_RIGHT:

			fvars(ClosestFormToCursor);
			if (GetKeyState(VK_SHIFT) & 0X8000) {

				if (chkMap(FPSEL)) {

					if (chkMap(PSELDIR)) {

						++SelectedFormVertices.vertexCount %= SideCount;
						SelectedFormVertices.finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % SideCount;
					} else {

						if (SelectedFormVertices.vertexCount) {

							SelectedFormVertices.vertexCount--;
							SelectedFormVertices.finish = (SelectedFormVertices.start + SideCount - SelectedFormVertices.vertexCount) % SideCount;
						} else {

							SelectedFormVertices.vertexCount = 1;
							setMap(PSELDIR);
							SelectedFormVertices.finish = (SelectedFormVertices.start + 1) % SideCount;
						}
					}
					setpsel();
				} else {

					if (rstMap(FRMPSEL)) {

						unpsel();
						SelectedFormVertices.start = ClosestVertexToCursor;
						SelectedFormVertices.form = ClosestFormToCursor;
						SelectedFormVertices.vertexCount = 1;
						SelectedFormVertices.finish = (SelectedFormVertices.start + 1) % SideCount;
						setMap(PSELDIR);
						setpsel();
					} else {

						rstMap(LENSRCH);
						rstMap(FORMSEL);
						if (rstMap(SELBOX)) {

							dubx();
							if (ClosestPointIndex < (unsigned)PCSHeader.stitchCount - 1) {

								setMap(GRPSEL);
								GroupStitchIndex = ClosestPointIndex + 1;
							}
						} else {

							if (GroupStitchIndex < (unsigned)PCSHeader.stitchCount - 1) {

								GroupStitchIndex++;
								nuAct(GroupStitchIndex);
							}
						}
						grpAdj();
						redraw(ColorBar);
					}
				}
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(IniFile.cursorNudgeStep, 0);
				else {

					if (chkMap(LENSRCH)) {

						longer();
						setMap(RESTCH);
					} else {

						if (chkMap(FRMPSEL)) {

							fvars(ClosestFormToCursor);
							ClosestVertexToCursor = nxt(ClosestVertexToCursor);
							ritnum(STR_NUMPNT, ClosestVertexToCursor);
							ritfcor(&FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							shftflt(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							setMap(RESTCH);
						} else {

							if (chkMap(SELBOX)) {

								if (ClosestPointIndex < (unsigned)PCSHeader.stitchCount - 1)
									ClosestPointIndex++;
								movbox();
								return 1;
							}
							if (chkMap(GRPSEL)) {

								if (GroupStitchIndex < (unsigned)PCSHeader.stitchCount - 1) {

									GroupStitchIndex++;
									grpAdj();
									redraw(ColorBar);
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

						++SelectedFormVertices.vertexCount %= SideCount;
						SelectedFormVertices.finish = (SelectedFormVertices.start + SideCount - SelectedFormVertices.vertexCount) % SideCount;
					} else {

						if (SelectedFormVertices.vertexCount) {

							SelectedFormVertices.vertexCount--;
							SelectedFormVertices.finish = (SelectedFormVertices.start + SideCount - SelectedFormVertices.vertexCount) % SideCount;
						} else {

							SelectedFormVertices.vertexCount = 1;
							SelectedFormVertices.finish = (SelectedFormVertices.start + SideCount - 1) % SideCount;
							rstMap(PSELDIR);
						}
					}
					setpsel();
				} else {

					if (rstMap(FRMPSEL)) {

						unpsel();
						SelectedFormVertices.start = ClosestVertexToCursor;
						SelectedFormVertices.vertexCount = 1;
						rstMap(PSELDIR);
						setpsel();
					} else {

						rstMap(LENSRCH);
						rstMap(FORMSEL);
						if (rstMap(SELBOX)) {

							dubx();
							setMap(GRPSEL);
							GroupStitchIndex = ClosestPointIndex - 1;
						} else
							if (GroupStitchIndex) {

								GroupStitchIndex--;
								nuAct(GroupStitchIndex);
							}
						grpAdj();
						redraw(ColorBar);
					}
				}
			} else {

				if (GetKeyState(VK_CONTROL) & 0X8000)
					nudgfn(-IniFile.cursorNudgeStep, 0);
				else {

					if (chkMap(LENSRCH)) {

						shorter();
						setMap(RESTCH);
					} else {

						if (chkMap(FRMPSEL)) {

							fvars(ClosestFormToCursor);
							ClosestVertexToCursor = prv(ClosestVertexToCursor);
							ritnum(STR_NUMPNT, ClosestVertexToCursor);
							ritfcor(&FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							shftflt(FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
							setMap(RESTCH);
						} else {

							if (chkMap(SELBOX)) {

								if (ClosestPointIndex)
									ClosestPointIndex--;
								movbox();
								return 1;
							}
							if (chkMap(GRPSEL)) {

								if (GroupStitchIndex) {

									GroupStitchIndex--;
									grpAdj();
									redraw(ColorBar);
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
					dumrk(UnzoomedRect.x / 2, UnzoomedRect.y / 2);
				else {

					if (px2stch())
						dumrk(SelectedPoint.x, SelectedPoint.y);
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

			if (chkMsgs(Msg.pt, DefaultColorWin[0], DefaultColorWin[15]))
				delcol();
			else
				delet();
			break;
		}
		break;

	case WM_COMMAND:

		unmsg();
		if (chkMap(FORMSEL))
			fvars(ClosestFormToCursor);
		for (ind = 0; ind < OLDNUM; ind++) {

			if (Msg.wParam == LRUMenuId[ind]) {

				strcpy_s(WorkingFileName, IniFile.prevNames[ind]);
				setMap(REDOLD);
				nuFil();
			}
		}
		ind = LOWORD(Msg.wParam);
		if (ind > 40000 && ind < 40400)
			undat();
		switch (LOWORD(Msg.wParam)) {

		case ID_CHKOF: // view / Set / Data check / Off

			chgchk(0);
			break;

		case ID_CHKON: // view / Set / Data check / On

			chgchk(1);
			break;

		case ID_CHKREP: // view / Set / Data Check / Auto Repair on

			chgchk(2);
			break;

		case ID_CHKREPMSG: // view / Set / Data Check / Auto Repair with Message

			chgchk(3);
			break;

		case ID_REPAIR: // edit / Repair Data

			repar();
			break;

		case ID_WARNOF: // set / Warn if edited

			chgwrn();
			break;

		case ID_CLPSPAC: // set / Clipboard Fill  Spacing

			setclpspac();
			break;

		case ID_FRMIND: // edit / Form Update / Indent

			setfind();
			break;

		case ID_SETSIZ: // edit / Set / Design Size

			nudsiz();
			break;

		case ID_TXFIL: // Fill / Texture Editor

			dutxtfil();
			break;

		case ID_FRMHI: // edit / Form Update / Height

			setfhi();
			break;

		case ID_FRMWID: // edit / Form Update / Width

			setfwid();
			break;

		case ID_MAXFLEN: // edit / Form Update / Fill /  Maximum Stitch Length

			setfmax();
			break;

		case ID_MINFLEN: // edit / Form Update / Fill /  Minimum Stitch Length

			setfmin();
			break;

		case ID_MAXBLEN: // edit / Form Update / Border /  Maximum Stitch Length

			setbmax();
			break;

		case ID_MINBLEN: // edit / Form Update / Border /  Minimum Stitch Length

			setbmin();
			break;

		case ID_SETBSPAC: // edit / Form Update / Border /  Spacing

			setbspac();
			break;

		case ID_SETBLEN: // edit / Form Update / Border /  Stitch Length

			setblen();
			break;

		case ID_SETBCOL: // edit / Form Update / Border /  Color

			setbcol();
			break;

		case ID_SETFCOL: // edit / Form Update / Fill /  Color

			setfcol();
			break;

		case ID_SETUCOL: // edit / Form Update / Underlay /  Color

			setucol();
			break;

		case ID_SETFANG: // edit / Form Update / Fill /  Angle

			setfang();
			break;

		case ID_SETFSPAC: // edit / Form Update / Fill /  Spacing

			setfspac();
			break;

		case ID_SETFLEN: // edit / Form Update / Fill /  Stitch Length

			setflen();
			break;

		case ID_SETUANG: // edit / Form Update / Underlay /  Angle

			sfuang();
			break;

		case ID_SETUSPAC: // edit / Form Update / Underlay /  Spacing

			uspac();
			break;

		case ID_UNDLEN: // edit / Form Update / Underlay / Stitch Length

			undlen();
			break;

		case ID_SETCWLK: // edit / Form Update / Center Walk / On

			setcwlk();
			break;

		case ID_SETWLK: // edit / Form Update / Edge Walk /  On

			setwlk();
			break;

		case ID_SETUND: // edit / Form Update / Underlay / On

			setund();
			break;

		case ID_NOTCWLK: // edit / Form Update / Center Walk / Off

			notcwlk();
			break;

		case ID_NOTWLK: // edit / Form Update / Edge Walk / Off

			notwlk();
			break;

		case ID_NOTUND: // edit / Form Update / Underlay / Off

			notund();
			break;

		case ID_SELUND: // edit / Select / Form Underlay Stitches

			selfil(UNDMSK);
			break;

		case ID_SELWLK: // edit / Select / Form Edge Walk Stitches

			selfil(WLKMSK);
			break;

		case ID_ALFRM: // edit / Select / All Forms

			selalfrm();
			break;

		case ID_USPAC: // view / Set / Underlay / Spacing

			setuspac();
			break;

		case ID_UANG: // view / Set / Underlay / Angle

			setuang();
			break;

		case ID_USTCH: // view / Set / Underlay / Stitch Length

			setulen();
			break;

		case ID_WIND: // view / Set / Underlay / Indent

			setwlkind();
			break;

		case ID_FILSTRT: //Edit / Set / Fill Start Point

			setfilstrt();
			break;

		case ID_FILEND: //Edit / Set / Fill End Point

			setfilend();
			break;

		case ID_PES2CRD: // file / PES2Card

			pes2crd();
			break;

		case ID_2FTHR: // edit / Convert / to Feather Ribbon

			setMap(CNV2FTH);
			ribon();
			break;

		case ID_FETHR: // fill / Feather

			fethr();
			break;

		case ID_FTHDEF: // edit / Set / Feather Defaults

			dufdef();
			break;

		case ID_SRTF: // edit / Sort / by Form

			srtfrm();
			break;

		case ID_FILCLPX: // fill / Border / Clipboard, Even

			filclpx();
			break;

		case ID_FRMX: // view / Set / Form Cursor / Cross

			frmcursel(1);
			break;

		case ID_FRMBOX: // view / Set / Form Cursor / Box

			frmcursel(0);
			break;

		case ID_KNOTAT: // edit / Set / Knot at Selected Stitch

			set1knot();
			break;

		case ID_STCHPIX: // view / Set / Point Size / Stitch Point Boxes

			getstpix();;
			break;

		case ID_FRMPIX: // view / Set / Point Size / Form Point Triangles

			getfrmpix();
			break;

		case ID_FRMPBOX: // view / Set / Point Size / Form Box

			getfrmbox();
			break;

		case ID_CROP: // edit / Crop to Form

			crop();
			break;

		case ID_BAKMRK: // view / Retrieve Mark

			bakmrk();
			break;

		case ID_MARKESC: // view / Set / Retrieve Mark / Escape

			setu(MARQ);
			qchk();
			break;

		case ID_MARKQ: // view / Set / Retrieve Mark / Q

			rstu(MARQ);
			qchk();
			break;

		case ID_NUDGPIX: // view / Set / Nudge Pixels

			getnpix();
			break;

		case ID_LINCHN: // fill / Border / Line chain

			setMap(LINCHN);
			chain();
			break;

		case ID_OPNCHN: // fill / Border / Open chain

			rstMap(LINCHN);
			chain();
			break;

		case ID_CLOSE: // file / Close

			filclos();
			break;

		case ID_DELMAP: // file / Remove Bitmap

			delmap();
			break;

		case ID_BLAK: // edit / Trace / Reset Form Pixels

			blak();
			break;

		case ID_SAVMAP: // file / Save Bitmap

			savmap();
			break;

		case ID_TRDIF: // edit / Trace / Find Edges

			trdif();
			break;

		case ID_TRACEDG: // edit / Trace / Show Traced Edges

			tracedg();
			break;

		case ID_TRCSEL: // edit / Trace / Select Colors

			trcsel();
			break;

		case ID_TRACE: // edit / Trace / Trace Mode

			trinit();
			break;

		case ID_FLOK: // file / Locking

			lock();
			break;

		case ID_ROTAUXON: // view / Set / Rotate Machine File / On

			rotauxsel(1);
			break;

		case ID_ROTAUXOFF: // view / Set / Rotate Machine File / Off

			rotauxsel(0);
			break;

		case ID_FRM2COL: // edit / Set / Form Color to  Stich Color

			col2frm();
			break;

		case ID_SNAP2GRD: // edit / Snap / to Grid

			gsnap();
			break;

		case ID_FIL2SEL_ON: // view / Set / Fill at Select / On

			fil2sel(1);
			break;

		case ID_FIL2SEL_OFF: // view / Set / Fill at Select / Off

			fil2sel(0);
			break;

		case ID_OVRLAY: // file / Overlay

			ovrlay();
			break;

		case ID_GRDHI: // view / Set / Grid Mask / High

			setgrd(HIGRD);
			break;

		case ID_GRDMED: // view / Set / Grid Mask / Medium

			setgrd(MEDGRD);
			break;

		case ID_GRDEF: // view / Set / Grid Mask / Default

			setgrd(DEFGRD);
			break;

		case ID_GRDRED: // view / Set / Grid Mask / UnRed

			setgrd(REDGRD);
			break;

		case ID_GRDBLU: // view / Set / Grid Mask / UnBlue

			setgrd(BLUGRD);
			break;

		case ID_GRDGRN: // view / Set / Grid Mask / UnGreen

			setgrd(GRNGRD);
			break;

		case ID_RETRACE: // edit / Retrace

			retrac();
			break;

		case ID_DUBFIL: // fill / Border / Double

			dubfil();
			break;

		case ID_HORCLP: // fill / Clipboard / Horizontal

			if (chkMap(FORMSEL) || SelectedFormCount)
				savdo();
			horclp();
			break;

		case ID_ANGCLP: // fill / Clipboard / Angle

			if (chkMap(FORMSEL) || SelectedFormCount)
				savdo();
			angclp();
			break;

		case ID_VRTCLP: // fill / Clipboard / Vertical

			if (chkMap(FORMSEL) || SelectedFormCount)
				savdo();
			vrtclp();
			break;

		case ID_LINBEXACT: // view / Set / Line Border Spacing / Exact

			rstu(LINSPAC);
			linbmen();
			break;

		case ID_LINBEVEN: // view / Set / Line Border Spacing / Even

			setu(LINSPAC);
			linbmen();
			break;

		case ID_BSAVON: // view / Set / PCS Bitmap Save / On

			pcsbsavon();
			break;

		case ID_BSAVOF: // view / Set / PCS Bitmap Save / Off

			pcsbsavof();
			break;

		case ID_KNOTON: // view / Knots / On

			shoknot();
			break;

		case ID_KNOTOF: // view / Knots / Off

			hidknot();
			break;

		case ID_DELKNOT: // edit / Delete / Knots

			delknot();
			break;

		case ID_RSTNEDL: // view / Set / Needle Cursor / Off

			nedof();
			break;

		case ID_SETNEDL: // view / Set / Needle Cursor / On

			nedon();
			break;

		case ID_STCHS2FRM: // edit / Convert / Stitches to Form

			stchs2frm();
			break;

		case ID_SPLTFRM: // edit / Split Form

			spltfrm();
			break;

		case ID_UNBEAN: // edit / Convert / from Bean to Line

			debean();
			break;

		case ID_DUBEAN: // edit / Convert / to Bean

			dubean();
			break;

		case ID_SRTBF: // edit / Sort / by Color then Form

			srtbyfrm();
			break;

		case ID_CENTIRE: // edit / Center / Entire Design

			centir();
			break;

		case ID_CNTRX: // edit / Center / Both

			rstMap(CNTRH);
			rstMap(CNTRV);
			cntrx();
			break;

		case ID_CNTRH: // edit / Center / Horizontal

			rstMap(CNTRH);
			setMap(CNTRV);
			cntrx();
			break;

		case ID_CNTRV: // edit / Center / Vertical

			setMap(CNTRH);
			rstMap(CNTRV);
			cntrx();
			break;

		case ID_FRMNUM: // edit / Set / Form Number

			frmnum();
			break;

		case ID_REFILF: // fill / Refill

			filfrms();
			break;

		case ID_MRKPNT: // edit / Set / Zoom Mark at Selected Point

			pntmrk();
			break;

		case ID_ROTSEG: // edit / Set / Rotation / Segments

			rotseg();
			break;

		case ID_ROTMRK: // edit / Set / Rotation / Angle from Mark

			rotmrk();
			break;

		case ID_MRKCNTR: // edit / Set / Zoom Mark at Center

			dumrk(UnzoomedRect.x / 2, UnzoomedRect.y / 2);
			setMap(RESTCH);
			break;

		case ID_SETROT: // edit / Set / Rotation / Angle

			setrang();
			break;

		case ID_SETPREF: // view / Set / Default Preferences

			defpref();
			break;

		case ID_SHRINK: // edit / Shrink Clipboard Border

			shrnk();
			break;

		case ID_DUPAGAIN: // edit / Rotate / and Duplicate again

			redup();
			break;

		case ID_CLPADJ: // edit / Set / Range Ends for Clipboard Fills

			clpadj();
			break;

		case ID_DELTOT: // edit / Delete / All Forms and Stitches

			deltot();
			break;

		case ID_AUXPCS: // view / Set / Machine File Type / Pfaff PCS

			setpcs();
			break;

		case ID_AUXDST: // view / Set / Machine File Type / Tajima DST

			setdst();
			break;

		case ID_ABOUT: // view / About ThrEd4

			tabmsg(IDS_CPYRIT);
			break;

		case ID_RIBON: // edit / Convert / to Satin Ribbon

			rstMap(CNV2FTH);
			ribon();
			break;

		case ID_DESIZ: // view / Design Information

			desiz();
			break;

		case ID_HLP: // help

			help();
			break;

		case ID_CNTR: // edit / Center / Forms

			fcntr();
			break;

		case ID_INSFIL: // file / Insert

			rstMap(IGNORINS);
			insfil();
			break;

		case ID_SELALSTCH: // edit / Select / All Stitches

			selalstch();
			break;

		case ID_UNGRPLO: // edit / Ungroup / First

			ungrplo();
			break;

		case ID_UNGRPHI: // edit / Ungroup / Last

			ungrphi();
			break;

		case ID_CONTF: // fill / Contour

			contfil();
			break;

		case ID_THUM: // file / Thumbnails

			thumnail();
			break;

		case ID_PICOT: // fill / Border / Picot

			picot();
			break;

		case ID_FILBUT: // fill / Border / Buttonhole

			bhol();
			break;

		case ID_REMBIG: // edit / Delete / Large Stitches

			rembig();
			break;

		case ID_VUSELTHRDS: // view / Show Threads for Selected Color

			vuselthr();
			break;

		case ID_VUTHRDS: // view / Show Threads

			vuthrds();
			break;

		case ID_MOVMRK: // edit / Move / to Mark

			movmrk();
			break;

		case ID_SELFIL: // edit / Select / Form Fill Stitches

			selfil(FRMFIL);
			break;

		case ID_SELBRD: // edit / Select / Form Border Stitches

			selfil(FRMBFIL);
			break;

		case ID_SELAP: // edit / Select / Form Applique Stitches

			selfil(FRMAPFIL);
			break;

		case ID_SELFSTCHS: // edit / Select / Form Stitches

			selalfil();
			break;

		case ID_SETMRK: // edit / Set / Order Mark

			setmov();
			break;

		case ID_DELFRE: // edit / Delete / Free Stitches

			delfre();
			break;

		case ID_SELAL: // edit / Select / All Forms and Stitches

			selal();
			break;

		case ID_REFILAL: // edit / Refill All

			refilal();
			break;

		case ID_CHK: // edit / Check Range

			chkrng(&StitchRangeSize);
			setMap(RESTCH);
			break;

		case ID_RTRVCLP: // edit / Retrieve Clipboard Stitches

			rtrclp();
			break;

		case ID_SORT: // edit / Sort / Auto

			fsort();
			break;

		case ID_LAYMOV0: // edit / Move to Layer / 0

			movlayr(0);
			break;

		case ID_LAYMOV1: // edit / Move to Layer / 1

			movlayr(2);
			break;

		case ID_LAYMOV2: // edit / Move to Layer / 2

			movlayr(4);
			break;

		case ID_LAYMOV3: // edit / Move to Layer / 3

			movlayr(6);
			break;

		case ID_LAYMOV4: // edit / Move to Layer / 4

			movlayr(8);
			break;

		case ID_LAYCPY0: // edit / Copy to Layer / 0

			cpylayr(0);
			break;

		case ID_LAYCPY1: // edit / Copy to Layer / 1

			cpylayr(2);
			break;

		case ID_LAYCPY2: // edit / Copy to Layer / 2

			cpylayr(4);
			break;

		case ID_LAYCPY3: // edit / Copy to Layer / 3

			cpylayr(6);
			break;

		case ID_LAYCPY4: // edit / Copy to Layer / 4

			cpylayr(8);
			break;

		case ID_LA: // all (Show all layers)

			nulayr(0);
			break;

		case ID_L1: // 1 (Show layer 1 only)

			nulayr(1);
			break;

		case ID_L2: // 2 (Show layer 2 only)

			nulayr(2);
			break;

		case ID_L3: // 3 (Show layer 3 only)

			nulayr(3);
			break;

		case ID_L4: // 4 (Show layer 4 only)

			nulayr(4);
			break;

		case ID_ROTDUP: // edit / Rotate / and Duplicate

			rotdup();
			break;

		case ID_ROTAGAIN: // edit / Rotate / Again

			rotagain();
			break;

		case ID_ROTCMD: // edit / Rotate / Command

			rotcmd();
			break;

		case ID_DELFRMS: // edit / Delete / All Forms

			delfrms();
			rstAll();
			setMap(RESTCH);
			break;

		case ID_SNAP2: // edit / Snap / Together

			snap();
			break;

		case ID_CLPFIL: // fill / Clipboard / Fan

			clpfil();
			break;

		case ID_FLPORD: // edit / Flip / Order

			flpord();
			break;

		case ID_FRMOF: // frm+

			if (GetKeyState(VK_SHIFT) & 0X8000)
				tglhid();
			else
				tglfrm();
			break;

		case ID_MV2BAK: // edit / move / to End

			mv2b();
			break;

		case ID_MV2FRNT: // edit / move / to Start

			mv2f();
			break;

		case ID_PERP: // fill / Border / Perpendicular Satin

			if (chkMap(FORMSEL) || SelectedFormCount)
				savdo();
			prpbrd(StitchSpacing);
			break;

		case ID_PURGDIR: // file / Delete Backups / All backups in the selected directory

			purgdir();
			break;

		case ID_PURG: // file / Delete Backups / Backups for the selected file

			purg();
			break;

		case ID_VUBAK: // view / Backups

			vubak();
			break;

		case ID_DELETE: // edit / Delete / Selected

			delet();
			break;

		case ID_FLIPH: // edit / Flip / Horizontal

			fliph();
			break;

		case ID_FLIPV: // edit / Flip / Vertical

			flipv();
			break;

		case ID_FILANG: // fil/ / Angle 

			if (chkMap(FORMSEL))
				savdo();
			filangl();
			break;

		case ID_PREF: // pref

			prfmsg();
			break;

		case ID_BOLD: // fill / Border / Bean

			if (chkMap(FORMSEL) || SelectedFormCount)
				savdo();
			dubold();
			break;

		case ID_ADEND: // add

			stch2px1(PCSHeader.stitchCount - 1);
			endpnt();
			setMap(BAKEND);
			setMap(RESTCH);
			break;

		case ID_SETAP: // view / Set / Applique Color

			setap();
			break;

		case ID_APLIQ: // fill / Border / Applique

			if (chkMap(FORMSEL))
				savdo();
			apliq();
			break;

		case ID_SATBRD: // fill / Border / Angle Satin

			if (chkMap(FORMSEL))
				savdo();
			satbrd();
			break;

		case ID_FILCLP: // fill / Border / Clipboard

			fclp();
			break;

		case ID_FILIN: // fill / Border / Line

			if (chkMap(FORMSEL))
				savdo();
			bord();
			break;

		case ID_FRM0: // edit / Set / Form Zero Point

			frm0();
			break;

		case ID_REDO: // redo

			redo();
			break;

		case ID_UNFIL: // fill / Unfill

			savdo();
			unfil();
			coltab();
			setMap(RESTCH);
			break;

		case ID_FORM: // Form

			frmon();
			if (chkMap(FORMSEL))
				refrm();
			else
				form();
			break;

		case ID_FILSAT: // fill / Fan

			rstMap(FTHR);
			filsat();
			break;

		case ID_OPNPCD: // file / Open Auxiliary file

			switch (IniFile.auxFileType) {

			case AUXDST:

				OpenFileName.nFilterIndex = 3;
				break;

			default:

				OpenFileName.nFilterIndex = 2;
			}
			nuFil();
			nulayr(0);
			break;

		case ID_DELSTCH: // edit / Delete / All Stitches

			delstch();
			break;

		case ID_FILL_VERT: // fill / Vertical

			if (chkMap(FORMSEL) || SelectedFormCount)
				savdo();
			filvrt();
			break;

		case ID_FILL_HOR: // fill / Horizontal

			if (chkMap(FORMSEL))
				savdo();
			filhor();
			break;

		case ID_RUNPAT: // view / Movie

			movi();
			break;

		case ID_LENDEF: // Todo - No menu item in thred.rc for this option

			IniFile.maxStitchLength = MAXSIZ*PFGRAN;
			UserStitchLength = USESIZ*PFGRAN;
			MinStitchLength = MINSIZ*PFGRAN;
			break;

		case ID_TSIZDEF: // view / Thread Size / Set Defaults

			ThreadSize30 = TSIZ30;
			ThreadSize40 = TSIZ40;
			ThreadSize60 = TSIZ60;
			break;

		case ID_SIZ30: // view / Thread Size / 30

			tsizmsg("30", ThreadSize30);
			setMap(ENTR30);
			break;

		case ID_SIZ40: // view / Thread Size / 40

			tsizmsg("40", ThreadSize40);
			setMap(ENTR40);
			break;

		case ID_SIZ60: // view / Thread Size / 60

			tsizmsg("60", ThreadSize60);
			setMap(ENTR60);
			break;

		case ID_HIDBITF: // file / Hide Bitmap
		case ID_HIDBIT:  // edit / Trace / Hide Bitmap

			hidbit();
			break;

		case ID_LODBIT: // file / Load Bitmap

			lodbmp();
			break;

		case ID_KNOTS: // edit / Set / Knots

			setknots();
			break;

		case ID_REMZERO: // edit / Delete / Small Stitches

			duzero();
			break;

		case ID_ROT: // rot

			rot();
			break;

		case ZUMIN: // in

			if (chkMap(GMRK) || chkMap(SELBOX) || chkMap(INSRT) || chkMap(GRPSEL) || chkMap(FORMSEL))
				zumin();
			else {

				rstMap(BZUM);
				setMap(BOXZUM);
				rstMap(BZUMIN);
				setMap(VCAPT);
				SetCapture(ThrEdWindow);
			}
			break;

		case ID_ZUMOUT: // out

			zumout();
			break;

		case ID_FILE_OPEN1: // file / Open

			fop();
			break;

		case ID_VIEW_STCHBAK: // view / Set / Background Color

			if (nuBak()) {

				BackgroundColor = BackgroundColorStruct.rgbResult;
				BackgroundPen = nuPen(BackgroundPen, 1, BackgroundColor);
				BackgroundPenWidth = 1;
				DeleteObject(BackgroundBrush);
				BackgroundBrush = CreateSolidBrush(BackgroundColor);
				if (*PCSBMPFileName)
					bfil();
				zumhom();
			}
			break;

		case ID_BITCOL: // view / Set / Bitmap Color

			if (nuBit()) {

				BitmapColor = fswap(BitMapColorStruct.rgbResult);
				if (*PCSBMPFileName)
					bfil();
				BitmapPen = nuPen(BitmapPen, 1, BitmapColor);
				zumhom();
			}
			break;

		case ID_FILE_SAVE3: // file / Save As

			colchk();
			savAs();
			break;

		case ID_EDIT_RESET_COL: // edit / Reset Colors

			for (ind = 0; ind < 16; ind++) {

				UserColor[ind] = defCol[ind];
				UserColorBrush[ind] = nuBrush(UserColorBrush[ind], UserColor[ind]);
				UserPen[ind] = nuPen(UserPen[ind], 1, UserColor[ind]);
				redraw(UserColorWin[ind]);
			}
			setMap(RESTCH);
			break;

		case ID_FILE_SAVE2: // file / Save

			colchk();
			save();
			break;

		case VU_ZUMFUL: // view / Zoom Full

			zumhom();
			break;

		case ID_EDIT_SELECTCOLOR: // edit / Select / Color

			selCol();
			break;

		case ID_FILE_NEW1: // file / New

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

		case ID_BACK: // undo

			bak();
			break;

		case ID_DESNAM: // edit / Set / Designer Name

			getdes();
			break;
		}
		break;
	}
	return 0;
}

//return the width of a text item
int txtWid(TCHAR* str) {

	GetTextExtentPoint32(ThredDC, str, strlen(str), &TextSize);
	return TextSize.cx;
}

void makCol() {

	unsigned	ind;
	TCHAR		buf[3];

	buf[1] = '0';
	buf[2] = 0;
	for (ind = 0; ind < 16; ind++) {

		DefaultColorWin[ind] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			0,
			ButtonHeight*ind,
			ButtonWidth,
			ButtonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);

		UserColorWin[ind] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			ButtonWidth,
			ButtonHeight*ind,
			ButtonWidth,
			ButtonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);

		buf[0] = ThreadSize[ind][0];
		ThreadSizeWin[ind] = CreateWindow(
			"STATIC",
			buf,
			SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
			ButtonWidth << 1,
			ButtonHeight*ind,
			ButtonWidth,
			ButtonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
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
		WriteFile(hloc, (TCHAR*)HomeDirectory, strlen(HomeDirectory) + 1, &rot, 0);
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
					mov		esi, offset LeftUpCursorStart
					call	delsubt
					mov		cl, 4
					mov		eax, 0x88
					call	delsubl
					mov		cl, 19
					mov		eax, 0x1f00
					call	delsubl
					mov		esi, offset LeftUpCursorEnd
					mov		cl, 3
					call	delsubt

					//left down
					add		ebx, 128
					mov		edi, ebx
					mov		esi, offset LeftDownCursorStart
					mov		cl, 4
					call	delsubt
					mov		cl, 19
					mov		eax, 0x7c000000
					call	delsubr
					mov		cl, 4
					mov		eax, 0x440
					call	delsubr
					mov		cl, 5
					mov		esi, offset LeftDownCursorFinish
					call	delsubt

					//right up
					add		ebx, 128
					mov		edi, ebx
					mov		cl, 5
					mov		esi, offset RightUpCursorStart
					call	delsubt
					mov		eax, 0x9000000
					mov		cl, 4
					call	delsubr
					mov		cl, 19
					mov		eax, 0x1f00000
					call	delsubr
					mov		esi, offset RightUpCursorFinish
					mov		cl, 3
					call	delsubt

					//right down
					add		ebx, 128
					mov		edi, ebx
					mov		esi, offset RightDownCursorStart
					mov		cl, 4
					call	delsubt
					mov		eax, 0xf8
					mov		cl, 19
					call	delsubl
					mov		eax, 0x8400000
					mov		cl, 4
					call	delsubl
					mov		esi, offset RightDownCursorFinish
					mov		cl, 5
					call	delsubt

	}
#else
	unsigned *zPnt = (unsigned *)pnt;
	unsigned *dst = zPnt;
	//form cursor
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
	//dline cursor
	for (int i = 0; i < 32; i++) {
		*(dst++) = _byteswap_ulong(ltor | rtol | 0x80000001);

		rtol <<= 1;
		ltor >>= 1;
	}

	*(--dst) = 0xffffffff;
	*zPnt = 0xffffffff;

	zPnt += 32;
	dst = zPnt;
	//straight up needle cursor
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
	//left up
	delsubt(dst, LeftUpCursorStart, 5);

	delsubl(dst, 0x88, 4);
	delsubl(dst, 0x1f00, 19);

	delsubt(dst, LeftUpCursorEnd, 3);

	zPnt += 32;
	dst = zPnt;
	//left down
	delsubt(dst, LeftDownCursorStart, 4);

	delsubr(dst, 0x7c000000, 19);
	delsubr(dst, 0x440, 4);

	delsubt(dst, LeftDownCursorFinish, 5);

	zPnt += 32;
	dst = zPnt;
	//right up
	delsubt(dst, RightUpCursorStart, 5);

	delsubr(dst, 0x9000000, 4);
	delsubr(dst, 0x1f00000, 19);

	delsubt(dst, RightUpCursorFinish, 3);

	zPnt += 32;
	dst = zPnt;
	//right down
	delsubt(dst, RightDownCursorStart, 4);

	delsubl(dst, 0xf8, 19);
	delsubl(dst, 0x8400000, 4);

	delsubt(dst, RightDownCursorFinish, 5);
#endif
}

void duamsk() {

#if  __UseASM__
	_asm {

		xor		eax, eax
		dec		eax
		xor		ecx, ecx
		mov		cl, 32
		mov		edi, offset CursorMask
		rep		stosd
	}
#else
	memset(CursorMask, 0xff, 128);
#endif
}

void crtcurs() {

	duamsk();
	ducurs(CursorMasks.form);
	FormCursor = CreateCursor(ThrEdInstance, 16, 16, 32, 32, (void*)CursorMask, (void*)&CursorMasks.form);
	DLineCursor = CreateCursor(ThrEdInstance, 16, 16, 32, 32, (void*)CursorMask, (void*)&CursorMasks.dline);
	NeedleUpCursor = CreateCursor(ThrEdInstance, 16, 32, 32, 32, (void*)CursorMask, (void*)&CursorMasks.uprightNeedle);
	NeedleRightDownCursor = CreateCursor(ThrEdInstance, 1, 31, 32, 32, (void*)CursorMask, (void*)&CursorMasks.rightDownNeedle);
	NeedleRightUpCursor = CreateCursor(ThrEdInstance, 1, 1, 32, 32, (void*)CursorMask, (void*)&CursorMasks.rightUpNeedle);
	NeedleLeftDownCursor = CreateCursor(ThrEdInstance, 30, 30, 32, 32, (void*)CursorMask, (void*)&CursorMasks.leftDownNeedle);
	NeedleLeftUpCursor = CreateCursor(ThrEdInstance, 32, 1, 32, 32, (void*)CursorMask, (void*)&CursorMasks.leftUpNeedle);
}

void dstcurs() {

	DestroyCursor(FormCursor);
	DestroyCursor(DLineCursor);
	DestroyCursor(NeedleUpCursor);
	DestroyCursor(NeedleRightDownCursor);
	DestroyCursor(NeedleRightUpCursor);
	DestroyCursor(NeedleLeftDownCursor);
	DestroyCursor(NeedleLeftUpCursor);
}

void duhom() {

	unsigned	ind;
	TCHAR*		phom;	//pointer to the home file name

	strcpy_s(HomeDirectory, __argv[0]);
	phom = strrchr(HomeDirectory, '\\');
	if (phom)
		phom++;
	else {

		ind = GetCurrentDirectory(_MAX_PATH, HomeDirectory);
		if (ind) {
			HomeDirectory[ind++] = '\\';
			phom = &HomeDirectory[ind];
		}
	}
	if (phom) { *phom = 0; }
}

#if  __UseASM__
void bcpy(TCHAR* destination, TCHAR* source) {

	_asm {

		mov		ebx, source
		mov		edx, destination
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
	//	strcpy(destination, source);
	//#endif
}
#endif

void ducmd() {

	unsigned long	l_red;
	int				ind;

	if (__argc > 1) {

#if  __UseASM__
		bcpy(WorkingFileName, __argv[1]);
#else
		strcpy_s(WorkingFileName, __argv[1]);
#endif
		if (!strncmp(WorkingFileName, "/F1:", 4)) {

			BalaradFileName = &WorkingFileName[4];
			BalaradFile = CreateFile(BalaradFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			CloseHandle(BalaradFile);
			if (BalaradFile != INVALID_HANDLE_VALUE) {

#if  __UseASM__
				bcpy(BalaradName0, BalaradFileName);
#else
				strcpy_s(BalaradName0, BalaradFileName);
#endif
				if (__argc > 2) {

#if  __UseASM__
					bcpy(WorkingFileName, __argv[2]);
#else
					strcpy_s(WorkingFileName, __argv[2]);
#endif
					if (!strncmp(WorkingFileName, "/F2:", 4)) {

						BalaradFileName = &WorkingFileName[4];
						BalaradFile = CreateFile(BalaradFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
						if (BalaradFile != INVALID_HANDLE_VALUE) {

#if  __UseASM__
							bcpy(BalaradName1, BalaradFileName);
#else
							strcpy_s(BalaradName1, BalaradFileName);
#endif
							//ToDo - Does this line make sense?
							BalaradFileName = (TCHAR*)&BSequence;
							ReadFile(BalaradFile, (void*)&BalaradName2, 10000, &l_red, 0);
							strcat_s(BalaradName2, "");
							if (l_red)
								redbal();
						}
					}
				}
				SetWindowText(ThrEdWindow, StringTable[STR_EMB]);
			}
			*WorkingFileName = 0;
			CloseHandle(BalaradFile);
			DeleteFile(BalaradName1);
		} else {

			for (ind = 2; ind < __argc; ind++) {

				strcat_s(WorkingFileName, " ");
				strcat_s(WorkingFileName, __argv[ind]);
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
		UndoBuffer[ind] = 0;
	for (ind = 0; ind < OLDNUM; ind++)
		IniFile.prevNames[ind][0] = 0;
	duhom();
	strcpy_s(IniFileName, HomeDirectory);
	strcat_s(IniFileName, "thred.ini");
	IniFileHandle = CreateFile(IniFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, 0, NULL);
	if (IniFileHandle == INVALID_HANDLE_VALUE)
		defpref();
	else {

		ReadFile(IniFileHandle, &IniFile, sizeof(IniFile), &wrot, 0);
		if (wrot < 2061)
			IniFile.formBoxSizePixels = DEFBPIX;
		strcpy_s(DefaultDirectory, IniFile.defaultDirectory);
		strcpy_s(DefaultBMPDirectory, IniFile.defaultDirectory);
		for (ind = 0; ind < 16; ind++) {

			UserColor[ind] = IniFile.stitchColors[ind];
			CustomColor[ind] = IniFile.stitchPreferredColors[ind];
			CustomBackgroundColor[ind] = IniFile.backgroundPreferredColors[ind];
			BitmapBackgroundColors[ind] = IniFile.bitmapBackgroundColors[ind];
		}
		BackgroundColor = IniFile.backgroundColor;
		BitmapColor = IniFile.bitmapColor;
		MinStitchLength = IniFile.minStitchLength;
		ShowStitchThreshold = IniFile.showStitchThreshold;
		if (IniFile.threadSize30)
			ThreadSize30 = IniFile.threadSize30;
		if (IniFile.threadSize40)
			ThreadSize40 = IniFile.threadSize40;
		if (IniFile.threadSize60)
			ThreadSize60 = IniFile.threadSize60;
		if (IniFile.userStitchLength)
			UserStitchLength = IniFile.userStitchLength;
		if (!IniFile.maxStitchLength)
			IniFile.maxStitchLength = MAXSIZ*PFGRAN;
		if (IniFile.smallStitchLength)
			SmallStitchLength = IniFile.smallStitchLength;
		StitchBoxesThreshold = IniFile.stitchBoxesThreshold;
		if (IniFile.stitchSpace)
			StitchSpacing = IniFile.stitchSpace;
		UserFlagMap = IniFile.userFlagMap;
		if (IniFile.borderWidth)
			BorderWidth = IniFile.borderWidth;
		if (IniFile.underlayColor)
			UnderlayColor = IniFile.underlayColor & 0xf;
		if (IniFile.snapLength)
			SnapLength = IniFile.snapLength;
		if (IniFile.starRatio)
			StarRatio = IniFile.starRatio;
		if (IniFile.spiralWrap)
			SpiralWrap = IniFile.spiralWrap;
		if (IniFile.buttonholeFillCornerLength)
			ButtonholeCornerLength = IniFile.buttonholeFillCornerLength;
		if (!IniFile.gridSize)
			IniFile.gridSize = 12;
		if (!IniFile.wavePoints)
			IniFile.wavePoints = IWAVPNTS;
		if (!IniFile.waveStart)
			IniFile.waveStart = IWAVSTRT;
		if (!IniFile.waveEnd)
			IniFile.waveEnd = IWAVEND;
		if (!IniFile.waveLobes)
			IniFile.waveLobes = IWAVS;
		if (!IniFile.featherFillType)
			IniFile.featherFillType = FDEFTYP;
		if (!IniFile.featherUpCount)
			IniFile.featherUpCount = FDEFUP;
		if (!IniFile.featherDownCount)
			IniFile.featherDownCount = FDEFDWN;
		if (!IniFile.featherRatio)
			IniFile.featherRatio = (float)FDEFRAT;
		if (!IniFile.featherMinStitchSize)
			IniFile.featherMinStitchSize = FDEFFLR;
		if (!IniFile.featherCount)
			IniFile.featherCount = FDEFNUM;
		if (!IniFile.daisyHoleDiameter)
			IniFile.daisyHoleDiameter = DAZHLEN;
		if (!IniFile.daisyPetalPoints)
			IniFile.daisyPetalPoints = DAZCNT;
		if (!IniFile.daisyInnerCount)
			IniFile.daisyInnerCount = DAZICNT;
		if (!IniFile.daisyDiameter)
			IniFile.daisyDiameter = DAZLEN;
		if (!IniFile.daisyPetalCount)
			IniFile.daisyPetalCount = DAZPETS;
		if (!IniFile.daisyPetalLen)
			IniFile.daisyPetalLen = DAZPLEN;

		switch (IniFile.hoopType) {

		case SMALHUP:

			IniFile.hoopSizeX = SHUPX;
			IniFile.hoopSizeY = SHUPY;
			break;

		case LARGHUP:

			IniFile.hoopSizeX = LHUPX;
			IniFile.hoopSizeY = LHUPY;
			break;

		case CUSTHUP:

			if (!IniFile.hoopSizeX)
				IniFile.hoopSizeX = LHUPX;
			if (!IniFile.hoopSizeY)
				IniFile.hoopSizeY = LHUPY;
			break;

		case HUP100:

			if (!IniFile.hoopSizeX)
				IniFile.hoopSizeX = HUP100XY;
			if (!IniFile.hoopSizeY)
				IniFile.hoopSizeY = HUP100XY;
			break;

		default:

			IniFile.hoopType = LARGHUP;
			IniFile.hoopSizeX = LHUPX;
			IniFile.hoopSizeY = LHUPY;
		}
		UnzoomedRect.x = IniFile.hoopSizeX;
		UnzoomedRect.y = IniFile.hoopSizeY;
		PicotSpacing = IniFile.picotSpace;
	}
	if (!IniFile.gridColor)
		IniFile.gridColor = DEFGRD;
	CloseHandle(IniFileHandle);
	if (!IniFile.fillAngle)
		IniFile.fillAngle = PI / 6;
	tdc = GetDC(0);
	ScreenSizePixels.cx = GetDeviceCaps(tdc, HORZRES);
	ScreenSizePixels.cy = GetDeviceCaps(tdc, VERTRES);
	if (IniFile.initialWindowCoords.left < 0)
		IniFile.initialWindowCoords.left = 0;
	if (IniFile.initialWindowCoords.top < 0)
		IniFile.initialWindowCoords.top = 0;
	ind = ScreenSizePixels.cx - 30;
	if (IniFile.initialWindowCoords.right > (int)ind)
		IniFile.initialWindowCoords.right = ind;
	if (IniFile.initialWindowCoords.bottom > ScreenSizePixels.cy)
		IniFile.initialWindowCoords.bottom = ScreenSizePixels.cy;
}

void trcsub(HWND* hwnd, unsigned xloc, unsigned yloc, unsigned hi) {

	*hwnd = CreateWindow(
		"STATIC",
		"",
		SS_OWNERDRAW | WS_CHILD | WS_BORDER,
		xloc,
		yloc,
		ButtonWidth,
		hi,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void chkirct() {

	POINT lim;

	lim.x = ScreenSizePixels.cx - 1;
	lim.y = ScreenSizePixels.cy - 1;
	if (IniFile.initialWindowCoords.left > lim.x)
		IniFile.initialWindowCoords.left = lim.x;
	if (IniFile.initialWindowCoords.right > lim.x)
		IniFile.initialWindowCoords.right = lim.x;
	if (IniFile.initialWindowCoords.left < 0)
		IniFile.initialWindowCoords.left = 0;
	if (IniFile.initialWindowCoords.right < 0)
		IniFile.initialWindowCoords.right = 0;
	if (IniFile.initialWindowCoords.top > lim.y)
		IniFile.initialWindowCoords.top = lim.y;
	if (IniFile.initialWindowCoords.bottom > lim.y)
		IniFile.initialWindowCoords.bottom = lim.y;
	if (IniFile.initialWindowCoords.top < 0)
		IniFile.initialWindowCoords.top = 0;
	if (IniFile.initialWindowCoords.bottom < 0)
		IniFile.initialWindowCoords.bottom = 0;
	if (IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left < 300) {

		IniFile.initialWindowCoords.left = 0;
		IniFile.initialWindowCoords.right = lim.x >> 1;
	}
	if (IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top < 300) {

		IniFile.initialWindowCoords.top = 0;
		IniFile.initialWindowCoords.bottom = lim.y >> 1;
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

	TextureIndex = 0;
#if	 __UseASM__
	fsizeof = sizeof(FRMHED) >> 2;
#endif
	LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1));
	MainMenu = GetMenu(ThrEdWindow);
	totdc = GetDC(NULL);
	mwid = GetDeviceCaps(totdc, HORZRES);
	ReleaseDC(NULL, totdc);
	GetWindowRect(ThrEdWindow, &wrct);
	GetMenuItemRect(ThrEdWindow, MainMenu, 0, &tRct);
	//menhi = tRct.bottom - tRct.top;
	wrct.left = tRct.left;
	wrct.right = tRct.right;
	for (ind = 0; ind <= M_HELP; ind++) {

		GetMenuItemRect(ThrEdWindow, MainMenu, ind, &tRct);
		wrct.right += (tRct.right - tRct.left);
	}
	wrct.right += 20;
	thwid = wrct.right - wrct.left;
	mwid >>= 1;
	if (thwid < mwid)
		wrct.right = wrct.left += mwid;
	FillMenu = GetSubMenu(MainMenu, M_FIL);
	FileMenu = GetSubMenu(MainMenu, M_FILE);
	EditMenu = GetSubMenu(MainMenu, M_EDIT);
	BorderFillMenu = GetSubMenu(FillMenu, MFIL_BORD);
	ViewMenu = GetSubMenu(MainMenu, M_VIEW);
	ViewSetMenu = GetSubMenu(ViewMenu, M_VIEW);
	qchk();
	ArrowCursor = LoadCursor(0, IDC_ARROW);
	CrossCursor = LoadCursor(0, IDC_CROSS);
	crtcurs();
	redfils();
	clrMap(MAPLEN);						//clear the bitmap
	//set up the size variables
	ThredDC = GetDC(ThrEdWindow);
	SetStretchBltMode(ThredDC, COLORONCOLOR);
	StitchWindowDC = GetDCEx(MainStitchWin, 0, DCX_PARENTCLIP | DCX_CLIPSIBLINGS);
	StitchWindowMemDC = CreateCompatibleDC(StitchWindowDC);
	ScreenSizeMM.cx = GetDeviceCaps(ThredDC, HORZSIZE);
	ScreenSizeMM.cy = GetDeviceCaps(ThredDC, VERTSIZE);
	chkirct();
	if (!chku(SAVMAX))
		MoveWindow(ThrEdWindow, IniFile.initialWindowCoords.left, IniFile.initialWindowCoords.top, IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left, IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top, 0);
	ButtonWidth = txtWid("MM") + TXTSIDS;
	ButtonWidthX3 = ButtonWidth * 3;
	ButtonHeight = TextSize.cy + 4;
	NumeralWidth = txtWid("0");
	selbox = txtWid("0");
	for (ind = 0; ind < NERCNT; ind++)
		BoxOffset[ind] = selbox + selbox*ind;
	GetClientRect(ThrEdWindow, &ThredWindowRect);
	stchWnd();
	lodstr();
	maxwid(STR_PRF0, STR_PRF27);
	if (!IniFileHandle) {

		//initialize the user color and thread size arrays
		for (ind = 0; ind < 16; ind++)
			UserColor[ind] = defCol[ind];
	}
	for (ind = 0; ind < 16; ind++) {

		ThreadSize[ind][0] = '4';
		ThreadSize[ind][1] = '0';
	}
	if (!IniFile.traceLength)
		IniFile.traceLength = TRACLEN;
	if (!IniFile.traceRatio)
		IniFile.traceRatio = TRACRAT;
	if (!IniFile.chainSpace)
		IniFile.chainSpace = CHSDEF;
	if (!IniFile.chainRatio)
		IniFile.chainRatio = CHRDEF;
	if (!IniFile.cursorNudgeStep)
		IniFile.cursorNudgeStep = NUGINI;
	if (!IniFile.nudgePixels)
		IniFile.nudgePixels = DEFPIX;
	if (!IniFile.eggRatio)
		IniFile.eggRatio = DEFEGRAT;
	if (!IniFile.stitchSizePixels)
		IniFile.stitchSizePixels = DEFPNTPIX;
	if (!IniFile.formVertexSizePixels)
		IniFile.formVertexSizePixels = DEFPNTPIX;
	if (!IniFile.formSides)
		IniFile.formSides = 24;
	if (!IniFile.tearTailLength)
		IniFile.tearTailLength = (float)1.4;
	if (!IniFile.underlayStitchLen)
		IniFile.underlayStitchLen = DEFULEN;
	if (!IniFile.underlaySpacing)
		IniFile.underlaySpacing = DEFUSPAC;
	setgrd(IniFile.gridColor);
	makCol();		//make the color change windows
	for (ind = 0; ind < 9; ind++) {

		switch (ind) {

		case HBOXSEL:

			flg = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			pchr = StringTable[STR_BOXSEL];
			break;

		case HUPTO:

			flg = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			pchr = StringTable[STR_UPOF];
			break;

		case HHID:

			flg = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER;
			pchr = StringTable[STR_PIKOL];
			break;

		default:

			pchr = "";
			flg = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		}
		ButtonWin[ind] = CreateWindow(
			"STATIC",
			pchr,
			flg,
			0,
			ButtonHeight*(16 + ind),
			ButtonWidthX3,
			ButtonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);
	}
	TraceStepWin = CreateWindow(

		"STATIC",
		"",
		SS_NOTIFY | SS_CENTER | WS_CHILD | WS_BORDER,
		0,
		ButtonHeight * 18,
		ButtonWidthX3,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);

	ColorBar = CreateWindow(
		"STATIC",
		"",
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		ThredWindowRect.right - COLSIZ,
		0,
		COLSIZ,
		ThredWindowRect.bottom,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);

	for (ind = 0; ind < 3; ind++) {

		trcsub(&TraceControlWindow[ind], ButtonWidth*ind, 0, ButtonHeight * 15);
		trcsub(&TraceSelectWindow[ind], ButtonWidth*ind, ButtonHeight * 15, ButtonHeight);
		trcsub(&TraceUpWindow[ind], ButtonWidth*ind, ButtonHeight * 16, ButtonHeight);
		trcsub(&TraceDownWindow[ind], ButtonWidth*ind, ButtonHeight * 17, ButtonHeight);
		TraceBrush[ind] = CreateSolidBrush(TraceRGB[ind]);
	}

	nuRct();
	//create pens
	for (ind = 0; ind < 4; ind++)
		BoxPen[ind] = CreatePen(PS_SOLID, 1, BoxColor[ind]);
	LinePen = CreatePen(PS_SOLID, 1, 0x404040);
	CrossPen = CreatePen(PS_SOLID, 5, 0x804080);
	GroupSelectPen = CreatePen(PS_SOLID, 1, 0x804080);
	GridPen = CreatePen(PS_SOLID, 1, IniFile.gridColor);
	BackgroundPen = CreatePen(PS_SOLID, 3, BackgroundColor);
	BitmapPen = CreatePen(PS_SOLID, 1, BitmapColor);
	FormPen = CreatePen(PS_SOLID, 1, 0xc0c0c0);
	MultiFormPen = CreatePen(PS_SOLID, 1, 0xc0c080);
	FormPen3px = CreatePen(PS_SOLID, 3, 0xc0c0c0);
	FormSelectedPen = CreatePen(PS_SOLID, 1, 0x80c080);
	ZoomMarkPen = CreatePen(PS_SOLID, 3, 0x40c040);
	SelectAllPen = CreatePen(PS_SOLID, 1, 0xc08080);
	KnotPen = CreatePen(PS_SOLID, 1, 0xffffff);
	LayerPen[0] = CreatePen(PS_SOLID, 1, 0xc0c0c0);
	LayerPen[1] = CreatePen(PS_SOLID, 1, 0x80c0c0);
	LayerPen[2] = CreatePen(PS_SOLID, 1, 0xc080c0);
	LayerPen[3] = CreatePen(PS_SOLID, 1, 0xc0c080);
	LayerPen[4] = CreatePen(PS_SOLID, 1, 0x40c0c0);
	LayerPen[5] = CreatePen(PS_SOLID, 1, 0xc0c040);
	BackgroundPenWidth = 1;
	for (ind = 0; ind < 16; ind++) {

		ThreadSizePixels[ind] = 1;
		ThreadSizeIndex[ind] = 1;
		UserPen[ind] = CreatePen(PS_SOLID, 1, UserColor[ind]);
	}
	BackgroundBrush = CreateSolidBrush(BackgroundColor);
	//create brushes
	for (ind = 0; ind < 16; ind++) {

		DefaultColorBrush[ind] = CreateSolidBrush(defCol[ind]);
		UserColorBrush[ind] = CreateSolidBrush(UserColor[ind]);
	}
	BlackBrush = CreateSolidBrush(0);
	ZoomFactor = 1;
	PCSHeader.leadIn = 0x32;
	PCSHeader.colorCount = 16;
	PCSHeader.stitchCount = 0;
	GetDCOrgEx(StitchWindowDC, &StitchWindowOrigin);
	ladj();
	GetTextExtentPoint(StitchWindowMemDC, StringTable[STR_PIKOL], strlen(StringTable[STR_PIKOL]), &PickColorMsgSize);
	auxmen();
	fnamtabs();
	ritfnam(IniFile.designerName);
	strcpy_s(ExtendedHeader.modifierName, IniFile.designerName);
	ExtendedHeader.stgran = 0;
	for (ind = 0; ind < 26; ind++)
		ExtendedHeader.res[ind] = 0;
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRED], IniFile.designerName);
	SetWindowText(ThrEdWindow, MsgBuffer);
}

COLORREF defTxt(unsigned colInd) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, TextColorMap
		mov		edx, colInd
		bt		ecx, edx
		jnc		short defx
		mov		eax, 0xffffff
		defx:
	}
#else
	return _bittest((long *)&TextColorMap, colInd) ? 0xffffff : 0;
#endif
}

void setCol(unsigned color) {

#if  __UseASM__
	_asm {

		mov		ebx, color
		mov		eax, DisplayedColorBitmap
		bts		eax, ebx
		mov		DisplayedColorBitmap, eax
	}
#else
	// mask nextBufferIndex for sanity
	_bittestandset((long *)&DisplayedColorBitmap, color & 31);
#endif
}

void relin() {

	stch2px1(ClosestPointIndex - 1);
	MoveLine0[0].x = StitchCoordinatesPixels.x;
	MoveLine0[0].y = StitchCoordinatesPixels.y;
	stch2px1(ClosestPointIndex + 1);
	MoveLine1[1].x = StitchCoordinatesPixels.x;
	MoveLine1[1].y = StitchCoordinatesPixels.y;
	rstMap(WASLIN);
	dulin();
}

unsigned setRmp(unsigned pbit) {

#if  __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, offset MarkedStitchMap
		mov		ecx, pbit
		bts[ebx], ecx
		jc		short setrm
		dec		eax
		setrm :
	}
#else
	return _bittestandset((long *)MarkedStitchMap, pbit) ? 0 : 0xffffffff;
#endif
}

void drwLin(unsigned ind, unsigned len, HPEN hPen) {

	unsigned	ine, layr;
	fPOINTATTR*	l_pstch = &StitchBuffer[ind];

	if (ActiveLayer)
		LineIndex = 0;
	for (ine = 0; ine < len; ine++) {

		layr = (l_pstch[ine].attribute&LAYMSK) >> LAYSHFT;
		if (!ActiveLayer || !layr || (layr == ActiveLayer)) {

			LinePoints[LineIndex].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x;
			LinePoints[LineIndex++].y = StitchWindowClientRect.bottom - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y;
		}
	}
	SelectObject(StitchWindowMemDC, hPen);
	if (LineIndex < 16000)
		Polyline(StitchWindowMemDC, LinePoints, LineIndex);
	else {

		ine = 0;
		while (LineIndex) {

			if (LineIndex > 16000) {

				Polyline(StitchWindowMemDC, &LinePoints[ine], 16000);
				ine += 15999;
				LineIndex -= 15999;
			} else {

				Polyline(StitchWindowMemDC, &LinePoints[ine], LineIndex);
				break;
			}
		}
	}
	LineIndex = 1;
	layr = (l_pstch[ine].attribute&LAYMSK) >> LAYSHFT;
	if (!ActiveLayer || !layr || layr == ActiveLayer) {

		LinePoints[0].x = (l_pstch[ine - 1].x - ZoomRect.left)*ZoomRatio.x;
		LinePoints[0].y = StitchWindowClientRect.bottom - (l_pstch[ine - 1].y - ZoomRect.bottom)*ZoomRatio.y;
	}
}

void dumov() {

	fPOINTATTR*	tstch;
	POINT		arlin[8];
	POINT		trot;

	RotateAngle = atan2(StitchBuffer[MoveAnchor + 1].y - StitchBuffer[MoveAnchor].y, StitchBuffer[MoveAnchor + 1].x - StitchBuffer[MoveAnchor].x);
	tstch = &StitchBuffer[MoveAnchor];
	if (tstch->x >= ZoomRect.left&&tstch->x <= ZoomRect.right
		&&tstch->y >= ZoomRect.bottom&&tstch->y <= ZoomRect.top) {

		sdCor2px(StitchBuffer[MoveAnchor], &RotationCenterPixels);
		arlin[0].x = arlin[6].x = RotationCenterPixels.x;
		arlin[0].y = arlin[6].y = RotationCenterPixels.y;
		trot.x = RotationCenterPixels.x + 12;
		trot.y = RotationCenterPixels.y + 2;
		rotpix(trot, &arlin[1]);
		trot.y = RotationCenterPixels.y - 2;
		rotpix(trot, &arlin[5]);
		trot.y = RotationCenterPixels.y + 6;
		rotpix(trot, &arlin[2]);
		trot.y = RotationCenterPixels.y - 6;
		rotpix(trot, &arlin[4]);
		trot.x = RotationCenterPixels.x + 20;
		trot.y = RotationCenterPixels.y;
		rotpix(trot, &arlin[3]);
		SelectObject(StitchWindowMemDC, FormPen);
		SetROP2(StitchWindowMemDC, R2_XORPEN);
		Polyline(StitchWindowMemDC, arlin, 7);
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

unsigned chkup(unsigned cnt, unsigned ind) {

	if (chkMap(UPTO) && ClosestPointIndex) {

		if (ColorChangeTable[ind].stitchIndex < ClosestPointIndex) {

			if (ColorChangeTable[ind + 1].stitchIndex < ClosestPointIndex)
				return cnt;
			else
				return ClosestPointIndex - ColorChangeTable[ind].stitchIndex + 1;
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

	zirct.bottom = UnzoomedRect.y - ZoomRect.bottom;
	zirct.top = UnzoomedRect.y - ZoomRect.top;
	zirct.left = ZoomRect.left;
	zirct.right = ZoomRect.right;
	if (zirct.top > BitmapSizeinStitches.y || zirct.left > BitmapSizeinStitches.x)
		return 0;
	else {

		BitmapSrcRect.top = ceil(zirct.top*BmpStitchRatio.y);
		BitmapSrcRect.left = ceil(ZoomRect.left*BmpStitchRatio.x);
		BitmapSrcRect.right = floor(ZoomRect.right*BmpStitchRatio.x);
		BitmapSrcRect.bottom = floor(zirct.bottom*BmpStitchRatio.y);
		if (BitmapSrcRect.right > (long)BitmapWidth) {

			BitmapSrcRect.right = BitmapWidth;
			rstMap(LANDSCAP);
		}
		if (BitmapSrcRect.bottom > (long)BitmapHeight) {

			BitmapSrcRect.bottom = BitmapHeight;
			setMap(LANDSCAP);
		}
		bakrct.top = BitmapSrcRect.top*StitchBmpRatio.y;
		bakrct.left = BitmapSrcRect.left*StitchBmpRatio.x;
		bakrct.right = BitmapSrcRect.right*StitchBmpRatio.x;
		bakrct.bottom = BitmapSrcRect.bottom*StitchBmpRatio.y;
		difrct.top = bakrct.top - zirct.top;
		difrct.left = bakrct.left - zirct.left;
		difrct.right = zirct.right - bakrct.right;
		difrct.bottom = zirct.bottom - bakrct.bottom;
		bsdrat.x = (double)StitchWindowClientRect.right / (ZoomRect.right - ZoomRect.left);
		bsdrat.y = (double)StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
		BitmapDstRect.top = difrct.top*bsdrat.y;
		BitmapDstRect.left = difrct.left*bsdrat.x;
		BitmapDstRect.right = StitchWindowClientRect.right - difrct.right*bsdrat.x;
		BitmapDstRect.bottom = StitchWindowClientRect.bottom - difrct.bottom*bsdrat.y;
		return 1;
	}
}

void drwknot() {

#define			KSIZ 5
#define			KLSIZ 10

	unsigned	ind;
	POINT		pnt;
	POINT		tlin[5];

	if (!chku(KNOTOF) && KnotCount&&PCSHeader.stitchCount) {

		for (ind = 0; ind < KnotCount; ind++) {

			stCor2px(StitchBuffer[Knots[ind]], &pnt);
			SelectObject(StitchWindowMemDC, KnotPen);
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

	if (ZoomFactor < ShowStitchThreshold || !ShowStitchThreshold) {

		SetROP2(StitchWindowMemDC, R2_XORPEN);
		SelectObject(StitchWindowMemDC, GridPen);
		grdrct.left = ceil(ZoomRect.left / IniFile.gridSize);
		grdrct.right = floor(ZoomRect.right / IniFile.gridSize);
		grdrct.bottom = ceil(ZoomRect.bottom / IniFile.gridSize);
		grdrct.top = floor(ZoomRect.top / IniFile.gridSize);
		tlin[0].x = 0;
		tlin[1].x = StitchWindowClientRect.right;
		for (ind = grdrct.bottom; ind <= (unsigned)grdrct.top; ind++) {

			tlin[0].y = tlin[1].y = StitchWindowClientRect.bottom - (ind*IniFile.gridSize - ZoomRect.bottom)*ZoomRatio.y + 0.5;
			Polyline(StitchWindowMemDC, tlin, 2);
		}
		tlin[0].y = 0;
		tlin[1].y = StitchWindowClientRect.bottom;
		for (ind = grdrct.left; ind <= (unsigned)grdrct.right; ind++) {

			tlin[0].x = tlin[1].x = (ind*IniFile.gridSize - ZoomRect.left)*ZoomRatio.x + 0.5;
			Polyline(StitchWindowMemDC, tlin, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void rint() {

	unsigned l_siz;

	CellSize.x = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
	CellSize.y = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
	l_siz = StitchWindowClientRect.right*StitchWindowClientRect.bottom;
	if (l_siz > RMAPBITS) {

		CellSize.x *= l_siz / RMAPBITS;
		CellSize.y *= l_siz / RMAPBITS;
	}
}

unsigned setRmap(fPOINTATTR p_sPnt) {

	unsigned l_bpnt;

	l_bpnt = floor((p_sPnt.x - ZoomRect.left) / CellSize.x)*floor((p_sPnt.y - ZoomRect.bottom) / CellSize.y);
	if (l_bpnt < RMAPBITS)
		return setRmp(l_bpnt);
	return 0;
}

void drwStch() {

	unsigned	ind, ine, scnt, pwid, wascol;
	unsigned	layr;
	double		slop, off;
	double		dub6;
	fPOINTATTR*	l_pstch;
	POINT		tlin[2];
	long		hi = DrawItem->rcItem.bottom;
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
	for (ind = 0; ind < ColorChanges; ind++) {

		LineIndex = ColorChangeTable[ind + 1].stitchIndex - ColorChangeTable[ind].stitchIndex;
		if (LineIndex > scnt)
			scnt = LineIndex;
	}
	LinePoints = new POINT[scnt + 2];
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	duzrat();
	if (*PCSBMPFileName && !chkMap(HIDMAP) && !chkMap(UPTO)) {

		if (chkMap(WASTRAC))
			tdc = TraceDC;
		else
			tdc = BitmapDC;
		if (bitar())
			StretchBlt(StitchWindowMemDC, BitmapDstRect.left, BitmapDstRect.top, BitmapDstRect.right - BitmapDstRect.left, BitmapDstRect.bottom - BitmapDstRect.top,
				tdc, BitmapSrcRect.left, BitmapSrcRect.top, BitmapSrcRect.right - BitmapSrcRect.left, BitmapSrcRect.bottom - BitmapSrcRect.top, SRCCOPY);
	}
	dugrid();
	if (chkMap(INIT)) {

		if (chkMap(ZUMED)) {

			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_ALL;
			ScrollInfo.nMax = UnzoomedRect.y;
			ScrollInfo.nMin = 0;
			ScrollInfo.nPage = ZoomRect.top - ZoomRect.bottom;
			ScrollInfo.nPos = UnzoomedRect.y - ZoomRect.top;
			SetScrollInfo(VerticalScrollBar, SB_CTL, &ScrollInfo, TRUE);
			ScrollInfo.nMax = UnzoomedRect.x;
			ScrollInfo.nPage = ZoomRect.right - ZoomRect.left;
			ScrollInfo.nPos = ZoomRect.left;
			SetScrollInfo(HorizontalScrollBar, SB_CTL, &ScrollInfo, TRUE);
			ShowWindow(HorizontalScrollBar, TRUE);
			ShowWindow(VerticalScrollBar, TRUE);
		} else {

			ShowWindow(VerticalScrollBar, FALSE);
			ShowWindow(HorizontalScrollBar, FALSE);
		}
		duzrat();
		dub6 = ZoomRatio.x * 6;
		ThreadWidthPixels[0] = dub6*TSIZ30;
		ThreadWidthPixels[1] = dub6*TSIZ40;
		ThreadWidthPixels[2] = dub6*TSIZ60;
		for (ind = 0; ind < 16; ind++) {

			if (chkMap(THRDS))
				nuStchSiz(ind, ThreadWidthPixels[ThreadSizeIndex[ind]]);
			else {
				if (chkMap(COL)) {

					if (ind == ActiveColor)
						nuStchSiz(ind, ThreadWidthPixels[ThreadSizeIndex[ind]]);
					else
						nuStchSiz(ind, 1);
				} else
					nuStchSiz(ind, 1);
			}
		}
		LineIndex = 0; DisplayedColorBitmap = 0;
		if (chkMap(ZUMED)) {

			LineIndex = 0;
			rstMap(LINED);
			rstMap(LININ);
			for (ind = 0; ind < ColorChanges; ind++) {

				if (chkMap(HID)) {

					if (ColorChangeTable[ind].colorIndex != ActiveColor) {

						scnt = ColorChangeTable[ind + 1].stitchIndex - ColorChangeTable[ind].stitchIndex;
						l_pstch = &StitchBuffer[ColorChangeTable[ind].stitchIndex];
						for (ine = 0; ine < scnt; ine++) {

							if (l_pstch[ine].x >= ZoomRect.left
								&&l_pstch[ine].x <= ZoomRect.right
								&&l_pstch[ine].y >= ZoomRect.bottom
								&&l_pstch[ine].y <= ZoomRect.top) {

								setCol(ColorChangeTable[ind].colorIndex);
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
				SelectObject(StitchWindowMemDC, UserPen[ColorChangeTable[ind].colorIndex]);
				scnt = ColorChangeTable[ind + 1].stitchIndex - ColorChangeTable[ind].stitchIndex;
				l_pstch = &StitchBuffer[ColorChangeTable[ind].stitchIndex];
				scnt = chkup(scnt, ind);
				for (ine = 0; ine < scnt; ine++) {

					layr = (l_pstch[ine].attribute&LAYMSK) >> LAYSHFT;
					if (!ActiveLayer || !layr || (layr == ActiveLayer)) {

						if (l_pstch[ine].x >= ZoomRect.left
							&&l_pstch[ine].x <= ZoomRect.right
							&&l_pstch[ine].y >= ZoomRect.bottom
							&&l_pstch[ine].y <= ZoomRect.top) {

							wascol = 1;
							if (setMap(LINED)) {

								if (setMap(LININ)) {

									LinePoints[LineIndex].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[LineIndex++].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;

								} else {

									LinePoints[LineIndex].x = (l_pstch[ine - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[LineIndex++].y = hi - (l_pstch[ine - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LinePoints[LineIndex].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[LineIndex++].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
								}
							} else {

								if (ine == 0 && ind == 0) {

									LinePoints[0].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[0].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LineIndex = 1;
								} else {

									LinePoints[0].x = (l_pstch[ine - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[0].y = hi - (l_pstch[ine - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LinePoints[1].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[1].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LineIndex = 2;
								}
								setMap(LININ);
							}
						} else {

							if (rstMap(LININ)) {

								LinePoints[LineIndex].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
								LinePoints[LineIndex++].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
								Polyline(StitchWindowMemDC, LinePoints, LineIndex);
								LineIndex = 0;
							} else {

								if (ine) {

									//write an equation for this line
									xba = l_pstch[ine].x - l_pstch[ine - 1].x;
									yab = l_pstch[ine - 1].y - l_pstch[ine].y;
									slop = (double)xba / yab;
									off = l_pstch[ine].x + slop*l_pstch[ine].y;
									//does the line intersect with the top of the screen?
									tlng = off - slop*ZoomRect.top;
									if (tlng >= ZoomRect.left&&tlng <= ZoomRect.right) {

										tlin[0].x = (l_pstch[ine - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										tlin[0].y = hi - (l_pstch[ine - 1].y - ZoomRect.bottom)*ZoomRatio.x + 0.5;
										tlin[1].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										tlin[1].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.x + 0.5;
										Polyline(StitchWindowMemDC, tlin, 2);
										goto rotlin;
									}
									//does the line intersect the bottom of the screen?
									tlng = off - slop*ZoomRect.bottom;
									if (tlng >= ZoomRect.left&&tlng <= ZoomRect.right) {

										tlin[0].x = (l_pstch[ine - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										tlin[0].y = hi - (l_pstch[ine - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
										tlin[1].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										tlin[1].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
										Polyline(StitchWindowMemDC, tlin, 2);
										goto rotlin;
									}
									//does the line intersect the left side of the screen?
									if (slop) {

										tlng = (off - ZoomRect.left) / slop;
										if (tlng >= ZoomRect.bottom&&tlng <= ZoomRect.top) {

											tlin[0].x = (l_pstch[ine - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
											tlin[0].y = hi - (l_pstch[ine - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
											tlin[1].x = (l_pstch[ine].x - ZoomRect.left)*ZoomRatio.x + 0.5;
											tlin[1].y = hi - (l_pstch[ine].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
											Polyline(StitchWindowMemDC, tlin, 2);
										}
									}
								}
							rotlin:;

							}
						}
					}
				}
				if (LineIndex) {

					Polyline(StitchWindowMemDC, LinePoints, LineIndex);
					LinePoints[0].x = LinePoints[LineIndex - 1].x;
					LinePoints[0].y = LinePoints[LineIndex - 1].y;
					LineIndex = 1;
				}
				if (wascol)
					setCol(ColorChangeTable[ind].colorIndex);
			skip:;
			}
		} else {

			pwid = chkMap(HID);
			for (ind = 0; ind < ColorChanges; ind++) {

				setCol(ColorChangeTable[ind].colorIndex);
				scnt = ColorChangeTable[ind + 1].stitchIndex - ColorChangeTable[ind].stitchIndex;
				scnt = chkup(scnt, ind);
				if (!pwid || (pwid&&ColorChangeTable[ind].colorIndex == ActiveColor))
					drwLin(ColorChangeTable[ind].stitchIndex, scnt, UserPen[ColorChangeTable[ind].colorIndex]);
			}
		}
		if (chkMap(SELBOX)) {

			ritcor(&StitchBuffer[ClosestPointIndex]);
			if (stch2px(ClosestPointIndex))
				dubox();
		}
		if (chkMap(FRMPSEL))
			ritfcor(&FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor]);
		if (!chkMap(SELBOX) && !chkMap(FRMPSEL))
			butxt(HCOR, "");
		if (chkMap(WASLIN))
			relin();
		if (chkMap(GRPSEL)) {

			if (cmpstch(ClosestPointIndex, GroupStitchIndex))
				cros(ClosestPointIndex);
			else {

				SearchLineIndex = 0;
				ducros(StitchWindowMemDC);
			}
			selRct(&StitchRangeRect);
			nuselrct();
			setMap(SELSHO);
			dusel(StitchWindowMemDC);
		}
		if (ZoomFactor < StitchBoxesThreshold) {

			clRmap(RMAPSIZ);
			SelectObject(StitchWindowMemDC, LinePen);
			SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
			rint();
			if (chkMap(HID)) {

				for (ind = 0; ind < ColorChanges; ind++) {

					if (ColorChangeTable[ind].colorIndex == ActiveColor) {

						for (ine = ColorChangeTable[ind].stitchIndex; ine < ColorChangeTable[ind + 1].stitchIndex; ine++) {

							if (StitchBuffer[ine].x >= ZoomRect.left&&StitchBuffer[ine].x <= ZoomRect.right
								&&StitchBuffer[ine].y >= ZoomRect.bottom&&StitchBuffer[ine].y <= ZoomRect.top
								&&setRmap(StitchBuffer[ine]))

								stchbox(ine, StitchWindowMemDC);
						}
					}
				}
			} else {

				for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

					if (StitchBuffer[ind].x >= ZoomRect.left&&StitchBuffer[ind].x <= ZoomRect.right
						&&StitchBuffer[ind].y >= ZoomRect.bottom&&StitchBuffer[ind].y <= ZoomRect.top
						&&setRmap(StitchBuffer[ind]))

						stchbox(ind, StitchWindowMemDC);
				}
			}
			SetROP2(StitchWindowMemDC, R2_COPYPEN);
		}
		if (chkMap(CLPSHO))
			duclp();
		if (chkMap(ROTAT) || chkMap(ROTCAPT) || chkMap(MOVCNTR))
			ritrot();
		delete[] LinePoints;
	}
	if (FormIndex && !chkMap(FRMOF))
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
	if (FormDataSheet)
		refrm();
	if (chkMap(GMRK))
		drwmrk(StitchWindowMemDC);
	if (chkMap(PRFACT))
		redraw(PreferencesWindow);
	if (chkMap(SELBOX))
		ritnum(STR_NUMSEL, ClosestPointIndex);
	ritot(PCSHeader.stitchCount);
	if (chkMap(INIT))
		lenCalc();
	if (chkMap(MOVSET))
		dumov();
	drwknot();
}

unsigned chkCol(unsigned color) {

#if  __UseASM__
	_asm {

		mov		ebx, color
		mov		edx, DisplayedColorBitmap
		xor		eax, eax
		bt		edx, ebx
		jnc		chk1
		dec		eax
		chk1 :
	}
#else
	return _bittest((long *)&DisplayedColorBitmap, color) ? 0xffffffff : 0;
#endif
}

void dubar() {

	unsigned	ind;
	RECT		tRct;
	double		tdub;
	POINT		lin[2];

	tRct.left = DrawItem->rcItem.left;
	tRct.right = DrawItem->rcItem.right;
	tRct.top = 0;
	tRct.bottom = DrawItem->rcItem.bottom;
	for (ind = 0; ind < ColorChanges; ind++) {

		tdub = (double)ColorChangeTable[ind + 1].stitchIndex / PCSHeader.stitchCount;
		tRct.bottom = tdub*DrawItem->rcItem.bottom;
		FillRect(DrawItem->hDC, &tRct, UserColorBrush[ColorChangeTable[ind].colorIndex]);
		tRct.top = tRct.bottom;
	}
	if (chkMap(SELBOX) || chkMap(GRPSEL)) {

		tdub = (double)ClosestPointIndex / PCSHeader.stitchCount;
		lin[0].y = lin[1].y = tRct.bottom*tdub + 0.5;
		lin[0].x = tRct.left;
		lin[1].x = tRct.right;
		SelectObject(DrawItem->hDC, CrossPen);
		SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
		Polyline(DrawItem->hDC, lin, 2);
		if (chkMap(GRPSEL)) {

			tdub = (double)GroupStitchIndex / PCSHeader.stitchCount;
			lin[0].y = lin[1].y = tRct.bottom*tdub + 0.5;
			lin[0].x = tRct.left;
			lin[1].x = tRct.right;
			Polyline(DrawItem->hDC, lin, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void ritbak(TCHAR* nam, DRAWITEMSTRUCT* p_ds) {

	unsigned	ind, ine, inf, col, lind, redsiz;
	HANDLE		thfil;
	POINT		l_siz;
	fPOINTATTR*	tstch;
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

		ReadFile(thfil, (STRHED*)&sthed, sizeof(STRHED), &BytesRead, NULL);
		if (BytesRead == sizeof(STRHED)) {

			if ((sthed.headerType & 0xffffff) == 0x746872) {

				vervar = (sthed.headerType & 0xff000000) >> 24;
				switch (vervar) {

				case 0:

					if (PCSHeader.hoopType == SMALHUP) {

						rsiz.x = IniFile.hoopSizeX = SHUPX;
						rsiz.y = IniFile.hoopSizeY = SHUPY;
					} else {

						rsiz.x = IniFile.hoopSizeX = LHUPX;
						rsiz.y = IniFile.hoopSizeY = LHUPY;
					}
					break;

				case 1:
				case 2:

					ReadFile(thfil, (STREX*)&thedx, sizeof(STREX), &BytesRead, NULL);
					if (BytesRead != sizeof(STREX))
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

				tstch = new fPOINTATTR[sthed.stitchCount];
				l_plin = new POINT[sthed.stitchCount];
				redsiz = sthed.stitchCount * sizeof(fPOINTATTR);
				ReadFile(thfil, (fPOINTATTR*)tstch, redsiz, &BytesRead, 0);
				if (redsiz == BytesRead) {

					SetFilePointer(thfil, 16, 0, FILE_CURRENT);
					ReadFile(thfil, (COLORREF*)&tbak, 4, &BytesRead, 0);
					ReadFile(thfil, (COLORREF*)tcol, 64, &BytesRead, 0);
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
				tflt = new fPOINT[sthed.vertexCount];
				if (vervar < 2) {

					frmlstx = (FRMHEDO*)&BSequence;
					ReadFile(thfil, (FRMHEDO*)frmlstx, sthed.formCount * sizeof(FRMHEDO), &BytesRead, 0);
					if (BytesRead != sthed.vertexCount * sizeof(FRMHEDO))
						goto bakskp;
					FillMemory(&BSequence, 0, sizeof(FRMHED)*FormIndex);
					for (ind = 0; ind < sthed.formCount; ind++) {

						SelectedForm = &flst[ind];
						MoveMemory(SelectedForm, &frmlstx[ind], sizeof(FRMHEDO));
					}
				} else {

					redsiz = sthed.formCount * sizeof(FRMHED);
					ReadFile(thfil, (FRMHED*)flst, redsiz, &BytesRead, 0);
					if (BytesRead != redsiz)
						goto bakskp;
				}
				redsiz = sthed.vertexCount * sizeof(fPOINT);
				ReadFile(thfil, (fPOINT*)tflt, redsiz, &BytesRead, 0);
				if (BytesRead != redsiz)
					goto bakskp;
				inf = 0;
				for (ind = 0; ind < sthed.formCount; ind++) {

					lind = inf;
					for (ine = 0; (ine < flst[ind].sides) && (inf < sthed.vertexCount); ine++) {

						l_plin[ine].x = tflt[inf].x*rat;
						l_plin[ine].y = l_siz.y - tflt[inf++].y*rat;
					}
					l_plin[ine].x = tflt[lind].x*rat;
					l_plin[ine].y = l_siz.y - tflt[lind].y*rat;
					SelectObject(p_ds->hDC, FormPen);
					SetROP2(p_ds->hDC, R2_XORPEN);
					if (FormList[ind].type == FRMLINE)
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

	lo = (UpPixelColor >> shft) & 0xff;
	hi = (DownPixelColor >> shft) & 0xff;
	TraceHighMaskRect.left = TraceLowMaskRect.left = TraceMiddleMaskRect.left = irct.left;
	TraceHighMaskRect.right = TraceLowMaskRect.right = TraceMiddleMaskRect.right = irct.right;
	len = irct.bottom - irct.top;
	rat = (double)lo / 255;
	TraceMiddleMaskRect.top = len*rat + irct.top;
	rat = (double)hi / 255;
	TraceMiddleMaskRect.bottom = len*rat + irct.top;
	rstMap(DUHI);
	rstMap(DULO);
	if (lo) {

		setMap(DULO);
		TraceLowMaskRect.bottom = TraceMiddleMaskRect.top;
		TraceLowMaskRect.top = 0;
	}
	if (hi != 255) {

		setMap(DUHI);
		TraceHighMaskRect.top = TraceMiddleMaskRect.bottom;
		TraceHighMaskRect.bottom = irct.bottom;
	}
}

void dublk(HDC dc) {

	if (chkMap(DUHI))
		FillRect(dc, &TraceHighMaskRect, BlackBrush);
	if (chkMap(DULO))
		FillRect(dc, &TraceLowMaskRect, BlackBrush);
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

			DestroyWindow(PreferencesWindow);
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

				MovieTimeStep += SPEDLIN;
				if (MovieTimeStep > MAXDELAY)
					MovieTimeStep = MAXDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			} else {

				scPnt.x = (ZoomRect.right - ZoomRect.left)*LINSCROL;
				if (!scPnt.x)
					scPnt.x = 1;
				scPnt.y = 0;
				rshft(scPnt);
			}
			return 1;

		case SB_LINERIGHT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				MovieTimeStep -= SPEDLIN;
				if (MovieTimeStep < MINDELAY)
					MovieTimeStep = MINDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			} else {

				scPnt.x = -(ZoomRect.right - ZoomRect.left)*LINSCROL;
				if (!scPnt.x)
					scPnt.x = -1;
				scPnt.y = 0;
				rshft(scPnt);
			}
			return 1;

		case SB_PAGELEFT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				MovieTimeStep += SPEDPAG;
				if (MovieTimeStep < MINDELAY)
					MovieTimeStep = MINDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			} else
				pglft();
			return 1;

		case SB_PAGERIGHT:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				MovieTimeStep -= SPEDPAG;
				if (MovieTimeStep < MINDELAY)
					MovieTimeStep = MINDELAY;
				setsped();
				SetScrollPos(SpeedScrollBar, SB_CTL, MAXDELAY - MovieTimeStep, TRUE);
			} else
				pgrit();
			return 1;

		case SB_THUMBPOSITION:

			if (chkMap(RUNPAT) || chkMap(WASPAT)) {

				if ((HWND)lParam == SpeedScrollBar) {

					ind = (unsigned)HIWORD(wParam);
					MovieTimeStep = MAXDELAY - ind;
					setsped();
					SetScrollPos(SpeedScrollBar, SB_CTL, ind, TRUE);
				}
			} else {

				if ((HWND)lParam == HorizontalScrollBar) {

					scPnt.x = ZoomRect.right - ZoomRect.left;
					ZoomRect.left = (long)HIWORD(wParam);
					ZoomRect.right = ZoomRect.left + scPnt.x;
					if (ZoomRect.right > UnzoomedRect.x) {

						ZoomRect.right = UnzoomedRect.x;
						ZoomRect.left = UnzoomedRect.x - scPnt.x;
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

			scPnt.y = (ZoomRect.top - ZoomRect.bottom)*LINSCROL;
			if (!scPnt.y)
				scPnt.y = 1;
			scPnt.x = 0;
			rshft(scPnt);
			return 1;

		case SB_LINEUP:

			scPnt.y = -(ZoomRect.top - ZoomRect.bottom)*LINSCROL;
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

			scPnt.y = ZoomRect.top - ZoomRect.bottom;
			ZoomRect.top = UnzoomedRect.y - (long)HIWORD(wParam);
			ZoomRect.bottom = ZoomRect.top - scPnt.y;
			if (ZoomRect.bottom < 0) {

				ZoomRect.bottom = 0;
				ZoomRect.top = scPnt.y;
			}
			setMap(RESTCH);
			return 1;
		}
		break;

	case WM_DRAWITEM:

		//owner draw windows
		DrawItem = (LPDRAWITEMSTRUCT)lParam;
		if (DrawItem->hwndItem == MainStitchWin&&DrawItem->itemAction == ODA_DRAWENTIRE) {

			if (chkMap(TXTRED))
			{
				drwtxtr();
				return 1;
			}
			if (!chkMap(RUNPAT)) {

				if (!chkMap(HIDSTCH) && (FileHandle || chkMap(INIT) || FormIndex || chkMap(SATPNT)) && !chkMap(BAKSHO))
					drwStch();
				else {

					FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
					duzrat();
					dugrid();
					if (chkMap(HIDSTCH)) {

						drwfrm();
						if (chkMap(SATPNT))
							satzum();
					}
					if (chkMap(PRFACT))
						redraw(PreferencesWindow);
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
					StitchWindowClientRect.right,	// width of destination rectangle
					StitchWindowClientRect.bottom,	// height of destination rectangle
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
		if (DrawItem->hwndItem == ColorBar&&DrawItem->itemAction == ODA_DRAWENTIRE) {

			if (PCSHeader.stitchCount)
				dubar();
			else
				FillRect(DrawItem->hDC, &DrawItem->rcItem, (HBRUSH)(COLOR_WINDOW + 1));
			return 1;
		}
		if (DrawItem->hwndItem == ButtonWin[HHID] && DrawItem->itemAction == ODA_DRAWENTIRE) {

			ind = (ButtonWidthX3 - PickColorMsgSize.cx) >> 1;
			if (chkMap(HID)) {

				FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[ActiveColor]);
				SetBkColor(DrawItem->hDC, UserColor[ActiveColor]);
			} else
				FillRect(DrawItem->hDC, &DrawItem->rcItem, (HBRUSH)(COLOR_BTNFACE + 1));
			if (chkMap(TXTRED))
			{
				LoadString(ThrEdInstance, IDS_TXWID, nam, _MAX_PATH);
				sprintf_s(HelpBuffer, sizeof(HelpBuffer), nam, TextureScreen.width / PFGRAN);
				TextOut(DrawItem->hDC, ind, 1, HelpBuffer, strlen(HelpBuffer));;
			} else
				TextOut(DrawItem->hDC, ind, 1, StringTable[STR_PIKOL], strlen(StringTable[STR_PIKOL]));;
			return 1;
		}
		if (chkMap(WASTRAC)) {

			for (ind = 0; ind < 3; ind++) {

				if (DrawItem->hwndItem == TraceUpWindow[ind]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[ind]);
					upnum(ind);
					return 1;
				}
				if (DrawItem->hwndItem == TraceDownWindow[ind]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[ind]);
					dwnum(ind);
				}
				if (DrawItem->hwndItem == TraceControlWindow[ind]) {

					durct(TraceShift[ind], DrawItem->rcItem);
					FillRect(DrawItem->hDC, &TraceMiddleMaskRect, TraceBrush[ind]);
					dublk(DrawItem->hDC);
					return 1;
				}
				if (DrawItem->hwndItem == TraceSelectWindow[ind]) {

					TempBrush = BlackBrush;
					strcpy_s(buf, StringTable[STR_OFF]);
					SetBkColor(DrawItem->hDC, 0);
					SetTextColor(DrawItem->hDC, TraceRGB[ind]);
					if (chkMap(TraceRGBFlag[ind])) {

						TempBrush = TraceBrush[ind];
						strcpy_s(buf, StringTable[STR_ON]);
						SetTextColor(DrawItem->hDC, 0);
						SetBkColor(DrawItem->hDC, TraceRGB[ind]);
					}
					FillRect(DrawItem->hDC, &DrawItem->rcItem, TempBrush);
					TextOut(DrawItem->hDC, 1, 1, buf, strlen(buf));
					return 1;
				}
				if (DrawItem->hwndItem == TraceNumberInput) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[ColumnColor]);
					SetBkColor(DrawItem->hDC, TraceRGB[ColumnColor]);
					TextOut(DrawItem->hDC, 1, 1, TraceInputBuffer, strlen(TraceInputBuffer));
					return 1;
				}
			}
		} else {

			for (ind = 0; ind < 16; ind++) {

				if (DrawItem->hwndItem == DefaultColorWin[ind]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, DefaultColorBrush[ind]);
					if (chkCol(ind)) {

						SetBkColor(DrawItem->hDC, defCol[ind]);
						SetTextColor(DrawItem->hDC, defTxt(ind));
						sprintf_s(buf, sizeof(buf), "%d", ind + 1);
						len = strlen(buf);
						GetTextExtentPoint32(DrawItem->hDC, buf, len, &l_txSiz);
						TextOut(DrawItem->hDC, (ButtonWidth - l_txSiz.cx) >> 1, 0, buf, len);
					}
					return 1;
				}
				if (DrawItem->hwndItem == UserColorWin[ind]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[ind]);
					if (ind == ActiveColor) {

						SelectObject(DrawItem->hDC, CrossPen);
						SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
						lin[0].x = lin[1].x = DrawItem->rcItem.right >> 1;
						lin[0].y = 0;
						lin[1].y = DrawItem->rcItem.bottom;
						Polyline(DrawItem->hDC, lin, 2);
						lin[0].y = lin[1].y = DrawItem->rcItem.bottom >> 1;
						lin[0].x = 0;
						lin[1].x = DrawItem->rcItem.right;
						Polyline(DrawItem->hDC, lin, 2);
						SetROP2(StitchWindowMemDC, R2_COPYPEN);
					}
					return 1;
				}
			}
		}
		if (chkMap(BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {

			if (chkMap(THUMSHO)) {

				for (ind = 0; ind < 4; ind++) {

					if (DrawItem->hwndItem == BackupViewer[ind] && ind < ThumbnailDisplayCount) {

						ritbak(ThumbnailsSelected[ind], DrawItem);
						rthumnam(ind);
						return 1;
					}
				}
			} else {

				for (ind = 0; ind < OLDVER; ind++) {

					if (DrawItem->hwndItem == BackupViewer[ind]) {

						strcpy_s(nam, ThrName);
						ine = duth(nam);
						nam[ine] = (TCHAR)ind + 's';
						ritbak(nam, DrawItem);
						return 1;
					}
				}
			}
		}
		break;

	case WM_SIZE:

		GetClientRect(p_hWnd, &ThredWindowRect);
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
				ind = (double)ScreenSizePixels.cx - 30;
				if ((unsigned)(ThredWindowRect.right - ThredWindowRect.left) > ind) {

					ine = (ScreenSizePixels.cx - ind) >> 1;
					ThredWindowRect.left = ine;
					ThredWindowRect.right = ScreenSizePixels.cx - ine;
				}
				ind = (double)ScreenSizePixels.cy - 30;
				if ((unsigned)(ThredWindowRect.bottom - ThredWindowRect.top) > ind) {

					ine = (ScreenSizePixels.cy - ind) >> 1;
					ThredWindowRect.top = ine;
					ThredWindowRect.bottom = ScreenSizePixels.cy - ine;
				}
				if (ine)
					MoveWindow(p_hWnd, ThredWindowRect.left, ThredWindowRect.top, ThredWindowRect.right - ThredWindowRect.left, ThredWindowRect.bottom - ThredWindowRect.top, TRUE);
			} else {

				MoveWindow(p_hWnd, IniFile.initialWindowCoords.left, IniFile.initialWindowCoords.top, IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left, IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top, TRUE);
			}
			ShowWindow(p_hWnd, SW_SHOW);
			break;
		}
		GetClientRect(p_hWnd, &ThredWindowRect);
		movStch();
		if (chkMap(ZUMED)) {

			tdub = (double)StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
			twid = StitchWindowClientRect.right / tdub;
			if (twid + ZoomRect.left > UnzoomedRect.x) {

				ZoomRect.right = UnzoomedRect.x;
				ZoomRect.left = UnzoomedRect.x - twid;
			} else
				ZoomRect.right = twid + ZoomRect.left;
		} else {

			ZoomRect.left = ZoomRect.bottom = 0;
			ZoomRect.right = UnzoomedRect.x;
			ZoomRect.top = UnzoomedRect.y;
		}
		NearestCount = 0;
		setMap(RESTCH);
		if (chkMap(SELBOX))
			shft2box();
		if (chkMap(RUNPAT)) {

			FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
			if (chkMap(ZUMED))
				RunPoint = GroupStartStitch;
			else
				RunPoint = 0;
		}
		return 1;

	case WM_MOVE:

		GetClientRect(p_hWnd, &ThredWindowRect);
		if ((ThredWindowRect.right - ThredWindowRect.left) < 20) {

			ThredWindowRect.left = 0;
			ThredWindowRect.right = 300;
		}
		if ((ThredWindowRect.bottom - ThredWindowRect.top) < 20) {

			ThredWindowRect.top = 0;
			ThredWindowRect.bottom = 300;
		}
		movStch();
		if (chkMap(RUNPAT)) {

			duzrat();
			RunPoint = 0;
			FillRect(StitchWindowDC, &StitchWindowClientRect, BackgroundBrush);
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

	pchr = MsgBuffer;
	for (ind = 0; ind < FormIndex; ind++) {

		tfrm = &FormList[ind];
		if (tfrm->type == SAT&&tfrm->satinGuideCount) {

			ts = tfrm->satinOrAngle.sac;
			for (ine = 0; ine < tfrm->satinGuideCount; ine++) {

				if (ts[ine].start > tfrm->sides || ts[ine].finish > tfrm->sides) {

					bakclo = ClosestFormToCursor;
					ClosestFormToCursor = ind;
					delsac(ind);
					ClosestFormToCursor = bakclo;
				}
			}
		}
	}
	if (pchr != MsgBuffer)
		shoMsg(MsgBuffer);
}

#if __UseASM__ == 0
#define BAD_FPU_EX (_EM_OVERFLOW | _EM_ZERODIVIDE | _EM_INVALID)
#define COMMON_FPU_EX (_EM_INEXACT | _EM_UNDERFLOW | _EM_DENORMAL)
#define ALL_FPU_EX (BAD_FPU_EX | COMMON_FPU_EX)
#endif

int	fltex(int p_code) {

#if  __UseASM__
	short	cw;

	_asm {

		xor		eax, eax
		cmp		p_code, 0x10
		jne		short fltex1
		mov		cw, 0x27f
		fldcw	cw
		dec		eax
		fltex1 :
	}
#else
	if (p_code != 0x10)
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

	ThrEdInstance = hInstance;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ThrEdInstance;
	wc.hIcon = (HICON)LoadImage(ThrEdInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,
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
		if (IniFile.initialWindowCoords.right) {

			ThrEdWindow = CreateWindow(
				"thred",
				"",
				WS_OVERLAPPEDWINDOW,
				IniFile.initialWindowCoords.left,
				IniFile.initialWindowCoords.right,
				IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left,
				IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top,
				0,
				0,
				hInstance,
				0);
		} else {

			ThrEdWindow = CreateWindow(
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
			GetClientRect(ThrEdWindow, &ThredWindowRect);
			IniFile.initialWindowCoords.left = ThredWindowRect.left;
			IniFile.initialWindowCoords.right = ThredWindowRect.right;
			IniFile.initialWindowCoords.top = ThredWindowRect.top;
			IniFile.initialWindowCoords.bottom = ThredWindowRect.bottom;
		}
		init();
		if (chku(SAVMAX))
			ShowWindow(ThrEdWindow, SW_SHOWMAXIMIZED);
		else
			ShowWindow(ThrEdWindow, SW_SHOW);
		if (!*IniFile.designerName)
		{
			LoadString(ThrEdInstance, IDS_UNAM, IniFile.designerName, 50);
			getdes();
		}
		while (GetMessage(&Msg, NULL, 0, 0)) {

			setMap(SAVACT);
			if (!chkMsg())
				DispatchMessage(&Msg);
			if (rstMap(FCHK))
				frmchkx();
			if (rstMap(RESTCH))
				redraw(MainStitchWin);
			if (rstMap(RELAYR))
				ritlayr();
			if (!chkMap(TXTRED))
				sachk();
			if (rstMap(DUMEN))
				DrawMenuBar(ThrEdWindow);
		}
		return 0;
	}
	return -1;
}