#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <tchar.h>
#ifdef ALLOCFAILURE
#include <new.h>
#endif
#include "lang.h"
#include "resource.h"
#include "thred.h"

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
// Suppress C6031: return value ignored
#pragma warning(disable:6031)

//Forward Declarations
void		blak ();
void		chkhup ();
void		clrhbut (unsigned startButton);
BOOL		cmpstch (unsigned iStitchA, unsigned iStitchB);
void		cros (unsigned iStitch);
void		delet ();
void		delsfrms (unsigned code);
void		delsmal (unsigned startStitch, unsigned endStitch);
void		delstchm ();
void		drwLin (unsigned currentStitch, unsigned length, HPEN hPen);
void		dstcurs ();
void		duIns ();
void		dufdef ();
void		dusel (HDC dc);
void		duselrng ();
void		dutrnum0 (unsigned color);
void		dutrnum1 ();
void		endpnt ();
void		fnamtabs ();
void		fndknt ();
void		frmdel ();
COLORREF	fswap (COLORREF color);
void		fthrfn ();
void		hupfn ();
void		ilin ();
void		insfil ();
void		lenCalc ();
void		movStch ();
void		movbox ();
void		mvstch (unsigned destination, unsigned source);
void		mvstchs (unsigned destination, unsigned source, unsigned count);
void		nuAct (unsigned iStitch);
void		nuslst (unsigned find);
void		okcan ();
void		patdun ();
unsigned	pt2colInd (unsigned iStitch);
void		rSelbox ();
void		redraw (HWND window);
void		ritnum (unsigned code, unsigned value);
void		ritot (unsigned number);
void		rngadj ();
void		rotfn ();
void		rotfns ();
void		rotpix (POINT unrotatedPoint, POINT* rotatedPoint);
void		rstAll ();
void		rstdu ();
void		save ();
void		sCor2px (dPOINT stitchCoordinate, POINT* pixelCoordinate);
void		selRct (fRECTANGLE* sourceRect);
unsigned	setRmp (unsigned bit);
void		setpsel ();
void		shft (fPOINT delta);
void		sizstch (fRECTANGLE* rectangle);
void		thrsav ();
void		trcols (COLORREF color);
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

extern	TXPNT*			adtx (int count);
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
extern	void			butxt (unsigned iButton, TCHAR* buttonText);
extern	void			centir ();
extern	void			chain ();
extern	void			chan ();
extern	void			chgchk (int code);
extern	void			chgwrn ();
extern	void			chkcont ();
extern	unsigned		chkfrm ();
extern	BOOL			chkr (unsigned bit);
extern	unsigned		closfrm ();
extern	void			clRmap (unsigned mapSize);
extern	void			clpfil ();
extern	void			clpic (unsigned short start);
extern	void			clrfills ();
extern	void			clrstch ();
extern	void			cntrx ();
extern	void			col2frm ();
extern	void			contfil ();
extern	void			cpylayr (unsigned codedLayer);
extern	void			crmsg (TCHAR* fileName);
extern	void			crop ();
extern	void			dazdef ();
extern	void			debean ();
extern	void			delcon (unsigned GuideIndex);
extern	void			deleclp (unsigned iForm);
extern	void			delflt (unsigned formIndex);
extern	void			delfrms ();
extern	void			delmclp (unsigned iForm);
extern	void			delmfil ();
extern	void			delsac (unsigned formIndex);
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
extern	void			dubcol (unsigned color);
extern	void			dubean ();
extern	void			dubfil ();
extern	void			dubig ();
extern	void			dubit (unsigned bit);
extern	void			dublen (float length);
extern	void			dubmax (float length);
extern	void			dubmin (float length);
extern	void			dubold ();
extern	void			dubsfil ();
extern	void			dubspac (float length);
extern	void			dueg (unsigned nsids);
extern	void			dufang (float angle);
extern	void			dufcol (unsigned color);
extern	void			dufhi (float length);
extern	void			dufind (float indent);
extern	void			duflen (float length);
extern	void			dufmax (float length);
extern	void			dufmin (float length);
extern	void			duform (unsigned ind);
extern	void			dufrm ();
extern	void			dufspac (float spacing);
extern	void			dufwid (float length);
extern	void			dufxang (float angle);
extern	void			duhart (unsigned sideCount);
extern	void			duinsf ();
extern	void			dulens (unsigned sides);
extern	void			dundcol (unsigned color);
extern	void			duprot ();
extern	void			dupsel (HDC dc);
extern	void			durpoli (unsigned vertexCount);
extern	void			dusat ();
extern	void			dushft ();
extern	void			duspac (float spacing);
extern	void			duspir (unsigned stepCount);
extern	void			dustar (unsigned starCount, double length);
extern	void			dusulen (float length);
extern	void			dutxtfil ();
extern	void			duxclp ();
extern	void			duzig (unsigned vertices);
extern	void			fclp ();
extern	void			fcntr ();
extern	void			fethr ();
extern	void			fethrf ();
extern	void			filangl ();
extern	void			filclpx ();
extern	void			filhor ();
extern	void			filnopn (unsigned code, TCHAR* fileName);
extern	void			filsat ();
extern	void			filvrt ();
extern	unsigned		find1st ();
extern	void			fliph ();
extern	void			flipv ();
extern	void			flpord ();
extern	unsigned		fltind (fPOINT* point);
extern	void			fltspac (fPOINT* start, unsigned count);
extern	void			form ();
extern	void			fritfil ();
extern	void			frm0 ();
extern	void			frmadj (unsigned formIndex);
extern	void			frmchkx ();
extern	void			frmclr (FRMHED* destination);
extern	void			frmcpy (FRMHED* destination, FRMHED* source);
extern	void			frmlin (fPOINT* vertices, unsigned vertexCount);
extern	void			frmnum ();
extern	void			frmnumfn (unsigned newFormIndex);
extern	void			frmon ();
extern	void			frmout (unsigned formIndex);
extern	void			frmovlin ();
extern	BOOL			frmrng (unsigned iForm, RANGE* range);
extern	void			frmsadj ();
extern	void			frmsqr (unsigned iVertex);
extern	void			frmx (POINT controlPoint, HDC dc);
extern	void			fsclpx ();
extern	void			fselrct (unsigned iForm);
extern	void			fsizpar ();
extern	void			fsort ();
extern	void			fvars (unsigned iForm);
extern	void			getfinfo (unsigned iForm);
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
extern	BOOL			istx (unsigned iForm);
extern	void			join ();
extern	void			lodchk ();
extern	void			lodstr ();
extern	void			makspac (unsigned start, unsigned count);
extern	void			maxtsiz (TCHAR* string, POINT* textSize);
extern	void			maxwid (unsigned start, unsigned finish);
extern	float			midl (float high, float low);
extern	void			mdufrm ();
extern	void			movlayr (unsigned codedLayer);
extern	void			msgstr (unsigned code);
extern	void			munfrm ();
extern	void			mvshft ();
extern	void			notcwlk ();
extern	BOOL			notfstch (unsigned attribute);
extern	BOOL			notsel ();
extern	void			notund ();
extern	void			notwlk ();
extern	void			nubrdcol (unsigned color);
extern	void			nudsiz ();
extern	void			nufilcol (unsigned color);
extern	void			nufsel ();
extern	void			nufthcol (unsigned color);
extern	void			nulapcol (unsigned color);
extern	unsigned		nxt (unsigned iVertex);
extern	void			oclp (fPOINT* clip, unsigned clipEntries);
extern	unsigned		pdir (unsigned vertex);
extern	void			pes2crd ();
extern	void			picot ();
extern	void			prbug ();
extern	void			prfmsg ();
extern	void			prfsid (HWND wnd);
extern	unsigned		projh (double yCoordinate, fPOINT point0, fPOINT point1, dPOINT* intersection);
extern	void			prpbrd (double borderStitchSpacing);
extern	unsigned		prv (unsigned iVertex);
extern	unsigned		psg ();
extern	void			px2stchf (POINT screen, fPOINT* stitchPoint);
extern	void			pxrct2stch (RECT screenRect, fRECTANGLE* stitchRect);
extern	void			rats ();
extern	void			ratsr ();
extern	void			rct2sel (RECT rectangle, POINT* line);
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
extern	void			ritfrct (unsigned iForm, HDC dc);
extern	void			rotagain ();
extern	void			rotcmd ();
extern	void			rotdup ();
extern	void			rstfrm ();
extern	void			rstxt ();
extern	void			rstxt ();
extern	void			rtrclp ();
extern	void			sRct2px (fRECTANGLE stitchRect, RECT* screenRect);
extern	void			satadj ();
extern	void			satbrd ();
extern	void			satfix ();
extern	void			satknkt ();
extern	void			satpnt0 ();
extern	void			satpnt1 ();
extern	void			satsel ();
extern	void			satzum ();
extern	void			savblen (float fLength);
extern	void			savdisc ();
extern	void			savplen (float length);
extern	void			savtxt ();
extern	void			selal ();
extern	void			selalfil ();
extern	void			selalfrm ();
extern	void			selfil (unsigned type);
extern	void			selsqr (POINT controlPoint, HDC dc);
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
extern	void			setr (unsigned bit);
extern	void			setrang ();
#if PESACT
extern	BOOL			setrc (unsigned bit);
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
extern	void			sfCor2px (fPOINT stitchPoint, POINT* screen);
extern	void			sfuang ();
extern	void			shoMsg (TCHAR* string);
extern	void			shoseln (unsigned code0, unsigned code1);
extern	void			shrnk ();
extern	void			sidwnd (HWND wnd);
extern	void			snap ();
extern	void			spltfrm ();
extern	void			srtbyfrm ();
extern	void			srtfrm ();
extern	void			stCor2px (fPOINTATTR stitch, POINT* screen);
extern	void			stchrct2px (fRECTANGLE stitchRect, RECT* screenRect);
extern	void			stchs2frm ();
extern	void			strtchbox ();
extern	void			tabmsg (unsigned code);
extern	void			tglfrm ();
extern	void			tomsg ();
extern	void			tsizmsg (TCHAR* threadSizeText, double threadSize);
extern	void			tst ();
extern	void			txdun ();
extern	void			txof ();
extern	void			txsnap ();
extern	void			txtclp ();
extern	void			txtkey (unsigned keyCode);
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
extern	fPOINT			ClipPoints[MAXITEMS];
extern	unsigned		ClosestFormToCursor;
extern	unsigned		ClosestVertexToCursor;
extern	unsigned		ClipPointIndex;
extern	SATCON*			CurrentFormGuides;
extern	fPOINT*			CurrentFormVertices;
extern	HWND			LabelWindow[LASTLIN];
extern	HWND			ValueWindow[LASTLIN];
extern	POINT			FormLines[MAXFRMLINS];
extern	unsigned		FormVertexIndex;
extern	fPOINT			FormMoveDelta;
extern	unsigned		FormIndex;
extern	FRMHED			FormList[MAXFORMS];
extern	POINT			FormControlPoints[10];
extern	fPOINT			FormVertices[MAXITEMS];
extern	TCHAR			HelpBuffer[HBUFSIZ];
extern	double			HorizontalRatio;
extern	fPOINT			InterleaveSequence[MAXITEMS];
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
extern	double			SnapLength;
extern	double			SpiralWrap;
extern	TCHAR*			StringData;
extern	TCHAR*			StringTable[STR_LEN];
extern	double			StarRatio;
extern	double			StitchSpacing;
extern	int				TextureHistoryIndex;
extern	TXHST			TextureHistory[16];
extern	TXTSCR			TextureScreen;
extern	int				TextureIndex;
extern	TXPNT			TexturePointsBuffer[MAXITEMS];
extern	unsigned		VertexCount;
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
unsigned		MarkedStitchMap[MAXITEMS];	//bitmap to tell when stitches have been marked
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
unsigned		AppliqueColor = 15;		//underlay color
unsigned		LastKeyCode = 0xffff;	//last key code
unsigned		FormMenuChoice = 0;		//data type for form data form numerical entry
dPOINT			ZoomMarkPoint;			//stitch coordinates of the zoom mark
unsigned		PreferenceIndex = 0;	//index to the active preference window
unsigned		LRUMenuId[] = { FM_ONAM0,FM_ONAM1,FM_ONAM2,FM_ONAM3 };	//recently used file menu ID's
TCHAR			VersionNames[OLDVER][_MAX_PATH];	//temporary storage for old file version names
unsigned		FileVersionIndex;		//points to old version to be read
unsigned		ActiveLayer = 0;		//active layer
unsigned		LayerIndex;				//active layer code
unsigned		ClipFormsCount;			//number of forms the on the clipboard
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
POINT			SearchLine[MAXITEMS];	//stitch select line 
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

COLORREF DefaultColors[] = {

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

long			BoxOffset[4];

unsigned		TextColorMap = 0xaf;	//bitmap for text color number colors
unsigned		VerticalIndex;		//vertical index of the color window, calculated from mouse click
unsigned		ThreadSizeSelected;	//thread selected for size change

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

unsigned			FlagMap[MAPLEN];		//bitmap
unsigned			UserFlagMap = 0;		//for storage of persistent binary variables set by the user
fPOINTATTR			StitchBuffer[MAXITEMS * 2];	//main stitch buffer
fPOINTATTR			ClipBuffer[MAXITEMS];	//for temporary copy of imported clipboard data
FRMHED*				SelectedForm;			//pointer to selected form
unsigned			FillTypes[] =			//fill type array for side window display
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

BOOL isclp(unsigned iForm) {

	if ((1 << FormList[iForm].fillType)&ClipTypeMap)
		return 1;
	return 0;
}

BOOL isclpx(unsigned iForm) {

	if (isclp(iForm) && FormList[iForm].lengthOrCount.clipCount)
		return 1;
	return 0;
}

BOOL isfclp() {

	if (isclp(ClosestFormToCursor) && FormList[ClosestFormToCursor].fillType != CLPF)
		return 1;
	return 0;
}

BOOL iseclp(unsigned iForm) {

	if (FormList[iForm].edgeType == EDGECLIP || FormList[iForm].edgeType == EDGEPICOT || FormList[iForm].edgeType == EDGECLIPX)
		return 1;
	return 0;
}

BOOL iseclpx(unsigned iForm) {

	if (iseclp(iForm) && FormList[iForm].clipEntries)
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
	ZoomRatio.x = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	ZoomRatio.y = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
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

	if (FormVertexIndex + count > MAXITEMS)
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
		backupData->guideCount = SatinConnectIndex;
		backupData->guide = (SATCON*)&backupData->vertices[FormVertexIndex];
		if (SatinConnectIndex)
			mvsatk(backupData->guide, &SatinConnects[0], SatinConnectIndex);
		backupData->clipPointCount = ClipPointIndex;
		backupData->clipPoints = (fPOINT*)&backupData->guide[SatinConnectIndex];
		if (ClipPointIndex) {

			if (ClipPointIndex > MAXITEMS)
				ClipPointIndex = MAXITEMS;
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
	MoveMemory(destination, source, sizeof(fPOINTATTR));
#endif
}

void duzero() {

	unsigned		iStitch, iForm;
	unsigned short	iDestination;
	double			stitchLength;
	fPOINTATTR*		currentStitch;

	if (SelectedFormCount) {

		clRmap(MAXITEMS);
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
	double			coordinate;
	long			hi;
	
	SelectObject(dc, GroupSelectPen);
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
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

		coordinate = ((StitchBuffer[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5);
		SearchLine[SearchLineIndex].x = static_cast<long>(coordinate);
		coordinate = (hi - (StitchBuffer[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5);
		SearchLine[SearchLineIndex++].y = static_cast<long>(coordinate);
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
		sfCor2px(outline[iLine], &FormControlPoints[iLine]);
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
	IniFile.underlayColor = AppliqueColor;
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
			BalaradStitch = new BALSTCH[MAXITEMS];
			ReadFile(balaradFile, (BALSTCH*)BalaradStitch, MAXITEMS * sizeof(BALSTCH), &bytesRead, 0);
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
			delete[] BalaradStitch;
		}
	}
	CloseHandle(balaradFile);
}

void ritbal() {

	BALHED			balaradHeader;
	unsigned*		ptrBalaradHeader;
	unsigned		iHeader, iStitch, iColor, iOutput, color;
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
		BalaradStitch = new BALSTCH[PCSHeader.stitchCount + 2];
		color = StitchBuffer[0].attribute&COLMSK;
		iOutput = 0;
		thr2bal(iOutput++, 0, BALJUMP);
		BalaradStitch[iOutput].flag = (unsigned char)color;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount && iOutput < 2; iStitch++) {

			thr2bal(iOutput++, iStitch, BALNORM);
			if ((StitchBuffer[iStitch].attribute&COLMSK) != color) {

				thr2bal(iOutput, iStitch, BALSTOP);
				color = StitchBuffer[iStitch].attribute&COLMSK;
				BalaradStitch[iOutput++].flag = (unsigned char)color;
			}
		}
		WriteFile(balaradFile, (BALSTCH*)BalaradStitch, iOutput * sizeof(BALSTCH), &bytesWritten, 0);
		CloseHandle(balaradFile);
		balaradFile = CreateFile(BalaradName1, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		WriteFile(balaradFile, (TCHAR*)outputName, strlen(outputName) + 1, &bytesWritten, 0);
		CloseHandle(balaradFile);
		delete[] BalaradStitch;
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
			reldun();
		}
	}
	delete[] StringData; // This is allocated in lodstr
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
				SelectedForm->fillStart %= VertexCount;
				break;

			case LDEND:

				savdo();
				SelectedForm->fillEnd = value / PFGRAN;
				SelectedForm->fillEnd %= VertexCount;
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

							AppliqueColor = (unsigned)(value - 1) % 16;
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", AppliqueColor + 1);
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
	SatinConnectIndex = undoData->guideCount;
	if (SatinConnectIndex)
		mvsatk(&SatinConnects[0], &undoData->guide[0], SatinConnectIndex);
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

void xofrm(FRMHEDO*	formListOriginal) {

	unsigned	iForm;
	
	FillMemory(FormList, sizeof(FRMHED)*FormIndex, 0);
	// ToDo - find a better way to write original form header data into new version
	for (iForm = 0; iForm < FormIndex; iForm++)
		MoveMemory(&FormList[iForm], &formListOriginal[iForm], sizeof(FRMHEDO));
}

void nuFil() {

	unsigned		fileSize, iStitch;
	unsigned		version;
	DWORD			fileSizeHigh;
	unsigned		textureHistoryFlag, pcsStitchCount;
	unsigned		iPCSstitch, iPESstitch, color, iColor;
	unsigned		iColorChange;
	unsigned char*	fileBuffer;
	TCHAR*			fileExtention;
	TCHAR			firstCharacter;
	STRHED			thredHeader;
	TCHAR			buffer[3];
	TCHAR*			tnam;
	DSTHED			dstHeader;
	fRECTANGLE		stitchRect;
	FRMHEDO*		formListOriginal;
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
							formListOriginal = new FRMHEDO[FormIndex];
							ReadFile(FileHandle, (FRMHEDO*)formListOriginal, FormIndex * sizeof(FRMHEDO), &BytesRead, 0);
							if (BytesRead != FormIndex * sizeof(FRMHEDO)) {

								FormIndex = BytesRead / sizeof(FRMHEDO);
								setMap(BADFIL);
							}
							xofrm(formListOriginal);
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

							FormList[iForm].vertices = adflt(FormList[iForm].vertexCount);
							if (FormList[iForm].type == SAT) {

								if (FormList[iForm].satinGuideCount)
									FormList[iForm].satinOrAngle.guide = adsatk(FormList[iForm].satinGuideCount);
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
						// ToDo - is there a better estimate for data size?
						fileBuffer = new unsigned char[MAXITEMS * 8];
						ReadFile(FileHandle, fileBuffer, MAXITEMS * 8, &BytesRead, 0);
						pesHeader = (PESHED*)fileBuffer;
						l_peschr = (TCHAR*)pesHeader;
						if (strncmp(pesHeader->led, "#PES00", 6)) {

							sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Not a PES file: %s\n", WorkingFileName);
							shoMsg(MsgBuffer);
							return;
						}
						pecof = tripl(pesHeader->off);
						PESstitch = fileBuffer + (pecof + 532);
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
								// ToDo - Use a new flag bit for this since FILDIR is not correct
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
						delete[] fileBuffer;
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
	fPOINTATTR*		dstStitchBuffer;
	unsigned*		colorData;
	HANDLE			colorFile;
	unsigned long	bytesWritten;

	// In this case, there could be as many colors as there are stitches
	// And we have to account for at least one color/stitch
	colorData = new unsigned[PCSHeader.stitchCount+3];
	iColor = 0;
	colorData[iColor++] = COLVER;
	colorData[iColor++] = BackgroundColor;
	colorData[iColor++] = UserColor[StitchBuffer[0].attribute&COLMSK];
	dstStitchBuffer = new fPOINTATTR[PCSHeader.stitchCount];
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		dstStitchBuffer[iStitch].x = RotatedStitches[iStitch].x * 5 / 3;
		dstStitchBuffer[iStitch].y = RotatedStitches[iStitch].y * 5 / 3;
		dstStitchBuffer[iStitch].attribute = RotatedStitches[iStitch].attribute;
	}
	boundingRect.left = boundingRect.right = dstStitchBuffer[0].x;
	boundingRect.bottom = boundingRect.top = dstStitchBuffer[0].y;
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (dstStitchBuffer[iStitch].x > boundingRect.right)
			boundingRect.right = dstStitchBuffer[iStitch].x + 0.5;
		if (dstStitchBuffer[iStitch].x < boundingRect.left)
			boundingRect.left = dstStitchBuffer[iStitch].x - 0.5;
		if (dstStitchBuffer[iStitch].y > boundingRect.top)
			boundingRect.top = dstStitchBuffer[iStitch].y + 0.5;
		if (dstStitchBuffer[iStitch].y < boundingRect.bottom)
			boundingRect.bottom = dstStitchBuffer[iStitch].y - 0.5;
	}
	DSTRecordCount = 0;
	centerCoordinate.x = (boundingRect.right - boundingRect.left) / 2 + boundingRect.left;
	centerCoordinate.y = (boundingRect.top - boundingRect.bottom) / 2 + boundingRect.bottom;
	DSTPositiveOffset.x = boundingRect.right - centerCoordinate.x + 1;
	DSTPositiveOffset.y = boundingRect.top - centerCoordinate.y + 1;
	DSTNegativeOffset.x = centerCoordinate.x - boundingRect.left - 1;
	DSTNegativeOffset.y = centerCoordinate.y - boundingRect.bottom - 1;
	color = dstStitchBuffer[0].attribute & 0xf;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (color != (dstStitchBuffer[iStitch].attribute & 0xf)) {

			savdst(0xc30000);
			color = dstStitchBuffer[iStitch].attribute & 0xf;
			colorData[iColor++] = UserColor[color];
		}
		lengths.x = dstStitchBuffer[iStitch].x - centerCoordinate.x;
		lengths.y = dstStitchBuffer[iStitch].y - centerCoordinate.y;
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
			WriteFile(colorFile, &colorData, iColor * sizeof(unsigned), &bytesWritten, 0);
		CloseHandle(colorFile);
		colorFile = CreateFile(RGBFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (colorFile != INVALID_HANDLE_VALUE)
			WriteFile(colorFile, &colorData[2], (iColor - 2) * sizeof(unsigned), &bytesWritten, 0);
		CloseHandle(colorFile);
	}
	delete[] dstStitchBuffer;
	delete[] colorData;
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
	// ToDo - Complete translation from assembler
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

void pecdat(TCHAR* buffer) {

	unsigned	iStitch, color, iColor;

	OutputIndex = 532;
	iColor = 1;
	color = StitchBuffer[0].attribute&COLMSK;
	PESdata = buffer;
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
	RotatedStitches = new fPOINTATTR[PCSHeader.stitchCount];
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

			// There are always going to be more recordds in the DST format
			DSTRecords = new DSTREC[2 * PCSHeader.stitchCount];
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
			delete[] DSTRecords;
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
			// There cannot be more color changes than stitches
			PESstitches = new PESTCH[PCSHeader.stitchCount * 2];
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
			WriteFile(PCSFileHandle, PESstitches, OutputIndex * sizeof(PESTCH), &bytesWritten, 0);
			delete[] PESstitches;
			iHeader = pesnam();
			// ToDo - is there a better estimate for data size?
			pchr = new TCHAR[MAXITEMS * 4];
			while (iHeader < 512)
				pchr[iHeader++] = ' ';
			pchr[19] = 13;
			pchr[48] = (TCHAR)pesColorCount;
			pecdat(pchr);
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
			pchr[528] = (TBYTE)0x80;	//hor msb
			pchr[529] = (TBYTE)0x80;	//hor lsb
			pchr[530] = (TBYTE)0x82;	//vert msb
			pchr[531] = (TBYTE)0xff;	//vert lsb
			//WriteFile(PCSFileHandle, (TBYTE*)&BSequence, OutputIndex, &bytesWritten, 0);
			WriteFile(PCSFileHandle, pchr, OutputIndex, &bytesWritten, 0);
			delete[] pchr;
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
	delete[] RotatedStitches;
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

void pxCor2stch(POINT point) {

	double	ratio;

	ratio = (double)(point.x - StitchWindowAbsRect.left) / StitchWindowClientRect.right;
	SelectedPoint.x = ratio*(ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	ratio = (double)(StitchWindowAbsRect.bottom - point.y) / StitchWindowClientRect.bottom;
	SelectedPoint.y = ratio*(ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
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
		// allocated in thumnail
		delete[] ThumbnailNames;
		delete[] Thumbnails;
		
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
				// ToDo - should the iterator in line below be iChange or iStitch?
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

void cros(unsigned iStitch) {

	long armLength = BoxOffset[0];

	stch2px1(iStitch);
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

void nuAct(unsigned iStitch) {

	unsigned color;

	color = ActiveColor;
	ActiveColor = StitchBuffer[iStitch].attribute&COLMSK;
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
	Polyline(dc, FormControlPoints, 9);
	for (ind = 0; ind < 8; ind++)
		selsqr(FormControlPoints[ind], dc);
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
	FormControlPoints[0].x = FormControlPoints[6].x = FormControlPoints[7].x = FormControlPoints[8].x = StitchCoordinatesPixels.x;
	FormControlPoints[1].x = FormControlPoints[5].x = StitchCoordinatesPixels.x + adjustedSelectSize.cx / 2;
	FormControlPoints[0].y = FormControlPoints[1].y = FormControlPoints[2].y = FormControlPoints[8].y = StitchCoordinatesPixels.y;
	FormControlPoints[3].y = FormControlPoints[7].y = StitchCoordinatesPixels.y + adjustedSelectSize.cy / 2;
	FormControlPoints[4].y = FormControlPoints[5].y = FormControlPoints[6].y = StitchCoordinatesPixels.y + adjustedSelectSize.cy;
	FormControlPoints[2].x = FormControlPoints[3].x = FormControlPoints[4].x = StitchCoordinatesPixels.x + adjustedSelectSize.cx;
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

	clipSize = sizeof(FORMCLIP) + VertexCount * sizeof(fPOINT);
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
	codedAttribute = iForm << FRMSHFT;
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

	length = FileSize = sizeof(FORMCLIP) + VertexCount * sizeof(fPOINT);
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
				// ToDo - Is global memory the right thing to use here?
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
					for (iSide = 0; iSide < SelectedForm->vertexCount; iSide++) {

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
							
							guides[guideCount].start = SelectedForm->satinOrAngle.guide[iGuide].start;
							guides[guideCount++].finish = SelectedForm->satinOrAngle.guide[iGuide].finish;
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
				clRmap(MAXITEMS);
				for (iForm = 0; iForm < SelectedFormCount; iForm++)
					setr(SelectedFormList[iForm]);
				// Todo - Allocate memory locally for astch
				astch = &StitchBuffer[MAXITEMS];
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
					iStitch = MAXITEMS;
					iDestination = 0;
					savclp(0, iStitch);
					iStitch++;
					ClipStitchData[0].led = stitchCount;
					iDestination++;
					while (iStitch < stitchCount + MAXITEMS)
						savclp(iDestination++, iStitch++);
					SetClipboardData(Clip, ClipPointer);
					CloseClipboard();
				}
			} else {

				if (chkMap(FORMSEL)) {

					length = sizclp();
					fvars(ClosestFormToCursor);
					guides = 0; codedAttribute = ClosestFormToCursor << FRMSHFT;
					FileSize += sizeof(FORMCLIP);
					ThrEdClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, FileSize);
					ClipFormHeader = frmref(ThrEdClipPointer);
					ClipFormHeader->clipType = CLP_FRM;
					frmcpy(&ClipFormHeader->form, &FormList[ClosestFormToCursor]);
					CurrentFormVertices = (fPOINT*)&ClipFormHeader[1];
					for (iSide = 0; iSide < SelectedForm->vertexCount; iSide++) {

						CurrentFormVertices[iSide].x = SelectedForm->vertices[iSide].x;
						CurrentFormVertices[iSide].y = SelectedForm->vertices[iSide].y;
					}
					guides = (SATCON*)&CurrentFormVertices[VertexCount];
					iGuide = 0;
					if (SelectedForm->type == SAT) {

						for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {

							guides[iGuide].start = SelectedForm->satinOrAngle.guide[iGuide].start;
							guides[iGuide].finish = SelectedForm->satinOrAngle.guide[iGuide].finish;
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
						codedAttribute = ClosestFormToCursor << FRMSHFT;
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
	// ToDo - rename inf
	unsigned	iStitch, iNextStitch, codedAttribute = ClosestFormToCursor << 4;
	unsigned	inf;
	long		dx, dy;
	double		stitchSize = 1e99;

	savdo();
	if (chkMap(FORMSEL)) {

		iNextStitch = find1st();
		iStitch = iNextStitch + 1;
		while (iStitch<(unsigned)PCSHeader.stitchCount - 1 && stitchSize>SmallStitchLength) {

			if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == codedAttribute) {

				if (StitchBuffer[iStitch].attribute&KNOTMSK)
					iNextStitch = iStitch;
				else {

					dx = StitchBuffer[iStitch].x - StitchBuffer[iNextStitch].x;
					dy = StitchBuffer[iStitch].y - StitchBuffer[iNextStitch].y;
					stitchSize = hypot(dx, dy);
				}
				iStitch++;
			}
		}
		if (iStitch != endStitch - 2) {

			iStitch--;
			iNextStitch = iStitch + 2;
			while (iNextStitch < (unsigned)PCSHeader.stitchCount - 1) {

				do {

					if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == codedAttribute && !(StitchBuffer[iStitch].attribute&KNOTMSK)) {

						dx = StitchBuffer[iNextStitch].x - StitchBuffer[iStitch].x;
						dy = StitchBuffer[iNextStitch++].y - StitchBuffer[iStitch].y;
						stitchSize = hypot(dx, dy);
					} else
						iNextStitch++;
				} while (stitchSize < SmallStitchLength&&iNextStitch < PCSHeader.stitchCount);
				StitchBuffer[++iStitch].attribute = StitchBuffer[--iNextStitch].attribute;
				StitchBuffer[iStitch].x = StitchBuffer[iNextStitch].x;
				StitchBuffer[iStitch].y = StitchBuffer[iNextStitch++].y;
			}
			iStitch++;
			while (iNextStitch < PCSHeader.stitchCount) {

				StitchBuffer[iStitch].attribute = StitchBuffer[iNextStitch].attribute;
				StitchBuffer[iStitch].x = StitchBuffer[iNextStitch].x;
				StitchBuffer[iStitch++].y = StitchBuffer[iNextStitch++].y;
			}
			PCSHeader.stitchCount = iStitch;
			coltab();
		}
	} else {

		iStitch = startStitch;
		iNextStitch = startStitch + 1;
		SelectedPoint.x = StitchBuffer[iStitch].x;
		SelectedPoint.y = StitchBuffer[iStitch].y;
		for (inf = iNextStitch; inf < endStitch; inf++) {

			if (StitchBuffer[iNextStitch].attribute&KNOTMSK) {

				SelectedPoint.x = StitchBuffer[iNextStitch].x;
				SelectedPoint.y = StitchBuffer[iNextStitch].y;
				mvstch(iNextStitch++, inf);
			} else {

				dx = StitchBuffer[inf].x - SelectedPoint.x;
				dy = StitchBuffer[inf].y - SelectedPoint.y;
				stitchSize = hypot(dx, dy);
				if (stitchSize > SmallStitchLength) {

					mvstch(iNextStitch++, inf);
					SelectedPoint.x = StitchBuffer[inf].x;
					SelectedPoint.y = StitchBuffer[inf].y;
				}
			}
		}
		while (inf < PCSHeader.stitchCount)
			mvstch(iNextStitch++, inf++);
		PCSHeader.stitchCount = iNextStitch;
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

	// ToDo - Use a temp buffer rather than the high buffer
	OutputIndex = MAXITEMS;
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
	PCSHeader.stitchCount = OutputIndex - MAXITEMS;
	mvstchs(0, MAXITEMS, PCSHeader.stitchCount);
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

	// ToDo - Use a temp buffer rather than the high buffer
	OutputIndex = MAXITEMS;
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
	PCSHeader.stitchCount = OutputIndex - MAXITEMS;
	mvstchs(0, MAXITEMS, PCSHeader.stitchCount);
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
	// ToDo - Can I use strrchr here?
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

void durit(char **destination, void* source, unsigned count) {

	CopyMemory(static_cast<void *>(*destination), source, count);
	*destination += count;
}


void dubuf(char *buffer, unsigned *count) {

	STRHED		stitchHeader;
	unsigned	iForm, iColor, iVertex, iGuide, iClip;
	unsigned	vertexCount, guideCount, clipDataCount;
	FRMHED*		forms;
	fPOINT*		vertices;
	SATCON*		guides;
	fPOINT*		points;
	unsigned	iDestinationVertex = 0;
	unsigned	iDestinationGuide = 0;
	unsigned	iDestinationClip = 0;
	char*		output;

	output = buffer;
	stitchHeader.headerType = 0x2746872;
	stitchHeader.fileLength = PCSHeader.stitchCount * sizeof(fPOINTATTR) + sizeof(STRHED) + 16;
	stitchHeader.stitchCount = PCSHeader.stitchCount;
	stitchHeader.hoopType = IniFile.hoopType;
	vertexCount = 0; guideCount = 0; clipDataCount = 0;
	strcpy_s(ExtendedHeader.modifierName, IniFile.designerName);
	if (FormIndex) {

		for (iForm = 0; iForm < FormIndex; iForm++) {

			vertexCount += FormList[iForm].vertexCount;
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
	durit(&output, &stitchHeader, sizeof(STRHED));
	ExtendedHeader.auxFormat = IniFile.auxFileType;
	ExtendedHeader.hoopSizeX = IniFile.hoopSizeX;
	ExtendedHeader.hoopSizeY = IniFile.hoopSizeY;
	ExtendedHeader.texturePointCount = TextureIndex;
	durit(&output, &ExtendedHeader, sizeof(STREX));
	durit(&output, StitchBuffer, PCSHeader.stitchCount * sizeof(fPOINTATTR));
	if (!PCSBMPFileName[0]) {

		for (iColor = 0; iColor < 16; iColor++)
			PCSBMPFileName[iColor] = 0;
	}
	durit(&output, PCSBMPFileName, 16);
	durit(&output, &BackgroundColor, 4);
	durit(&output, UserColor, 64);
	durit(&output, CustomColor, 64);
	for (iColor = 0; iColor < 16; iColor++)
		MsgBuffer[iColor] = ThreadSize[iColor][0];
	durit(&output, MsgBuffer, 16);
	if (FormIndex) {

		forms = new FRMHED[FormIndex];
		vertices = new fPOINT[vertexCount + 1];
		guides = new SATCON[guideCount];
		points = new fPOINT[clipDataCount + 1];
		for (iForm = 0; iForm < FormIndex; iForm++) {

			frmcpy(&forms[iForm], &FormList[iForm]);
			forms[iForm].vertices = (fPOINT*)(&vertices[iDestinationVertex] - &vertices[0]);
			for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++) {

				vertices[iDestinationVertex].x = FormList[iForm].vertices[iVertex].x;
				vertices[iDestinationVertex++].y = FormList[iForm].vertices[iVertex].y;
			}
			if (FormList[iForm].type == SAT) {

				forms[iForm].satinOrAngle.guide = (SATCON*)(&guides[iDestinationGuide] - &guides[0]);
				forms[iForm].satinGuideCount = FormList[iForm].satinGuideCount;
				for (iGuide = 0; iGuide < FormList[iForm].satinGuideCount; iGuide++) {

					guides[iDestinationGuide].start = FormList[iForm].satinOrAngle.guide[iGuide].start;
					guides[iDestinationGuide++].finish = FormList[iForm].satinOrAngle.guide[iGuide].finish;
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
		durit(&output, forms, FormIndex * sizeof(FRMHED));
		durit(&output, vertices, vertexCount * sizeof(fPOINT));
		durit(&output, guides, guideCount * sizeof(SATCON));
		durit(&output, points, clipDataCount * sizeof(fPOINT));
		durit(&output, TexturePointsBuffer, TextureIndex * sizeof(TXPNT));
		delete[] forms;
		delete[] vertices;
		delete[] guides;
		delete[] points;
	}
	*count = output - buffer;
}

void thrsav() {

	unsigned		iVersion, lastCharacter;
	int				iBackup;
	unsigned long	bytesWritten;
	WIN32_FIND_DATA	fileData;
	HANDLE			file;
	TCHAR			newFileName[_MAX_PATH];
	char*			output;
	unsigned		count;

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
		output = new char[MAXITEMS * 4];
		dubuf(output, &count);
		WriteFile(FileHandle, output, count, &bytesWritten, 0);
		delete[] output;
		if (bytesWritten != count) {

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
		clRmap((VertexCount >> 5) + 1);
		currentFormVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {

			setr(currentFormVertex);
			currentFormVertex = pdir(currentFormVertex);
		}
		currentFormVertex = 0;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {

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
		SelectedForm->vertexCount -= (SelectedFormVertices.vertexCount + 1);
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

					if (ClosestVertexToCursor == SelectedForm->angleOrClipData.guide.start || ClosestVertexToCursor == SelectedForm->angleOrClipData.guide.finish) {

						delmfil();
						SelectedForm->fillType = 0;
						coltab();
						setMap(RESTCH);
						return;
					}
					if (SelectedForm->angleOrClipData.guide.start > ClosestVertexToCursor)
						SelectedForm->angleOrClipData.guide.start--;
					if (SelectedForm->angleOrClipData.guide.finish > ClosestVertexToCursor)
						SelectedForm->angleOrClipData.guide.finish--;
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

					if (SelectedForm->satinOrAngle.guide[iForm].start == ClosestVertexToCursor || SelectedForm->satinOrAngle.guide[iForm].finish == ClosestVertexToCursor) {

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

		// ToDo - Use a temp buffer rather than the high buffer
		savdo();
		iLowBuffer = 0;
		iHighBuffer = MAXITEMS;
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
		iStitch = MAXITEMS;
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
			iHighBuffer = MAXITEMS;
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
			for (iStitch = MAXITEMS; iStitch < iHighBuffer; iStitch++) {

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

		// ToDo - Use a temp buffer rather than the high buffer
		savdo();
		iLowBuffer = 0;
		iHighBuffer = MAXITEMS;
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
		iStitch = MAXITEMS;
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
			iHighBuffer = MAXITEMS;
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
			for (iStitch = MAXITEMS; iStitch < iHighBuffer; iStitch++) {

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

		if (ClosestVertexToCursor == (unsigned)SelectedForm->vertexCount - 1 && SelectedForm->type == FRMLINE)
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
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
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
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
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

	unsigned	iControlPoint, closestControlPoint = 0;
	double		length, minimumLength = 1e99;
	POINT		pointToTest;

	pointToTest.x = Msg.pt.x - StitchWindowOrigin.x;
	pointToTest.y = Msg.pt.y - StitchWindowOrigin.y;
	for (iControlPoint = 0; iControlPoint < 9; iControlPoint++) {

		length = hypot(pointToTest.x - FormControlPoints[iControlPoint].x, pointToTest.y - FormControlPoints[iControlPoint].y);
		if (length < minimumLength) {

			minimumLength = length;
			closestControlPoint = iControlPoint;
		}
	}
	if (minimumLength < CLOSENUF) {

		SelectedFormControlVertex = closestControlPoint;
		return 1;
	} else
		return 0;
}

void rtrclpfn() {

	unsigned iStitch, count = 0;

	if (OpenClipboard(ThrEdWindow)) {

		fvars(ClosestFormToCursor);
		if (iseclp(ClosestFormToCursor)) {

			count = SelectedForm->clipEntries;
			oclp(SelectedForm->borderClipData, count);
		} else {

			if (isclp(ClosestFormToCursor)) {

				count = SelectedForm->lengthOrCount.clipCount;
				oclp(SelectedForm->angleOrClipData.clip, count);
			}
		}
		if (count) {

			LowerLeftStitch.x = LowerLeftStitch.y = 0;
			EmptyClipboard();
			Clip = RegisterClipboardFormat(PcdClipFormat);
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, count * sizeof(CLPSTCH) + 2);
			ClipStitchData = deref(ClipPointer);
			rtclpfn(0, 0);
			ClipStitchData[0].led = count;
			for (iStitch = 1; iStitch < count; iStitch++)
				rtclpfn(iStitch, iStitch);
			SetClipboardData(Clip, ClipPointer);
			CloseClipboard();
		}
	}
}

BOOL chkbig() {

	unsigned	iControlPoint, iCorner;
	double		length, minimumLength = 1e99;
	POINT		pointToTest;

	pointToTest.x = Msg.pt.x - StitchWindowOrigin.x;
	pointToTest.y = Msg.pt.y - StitchWindowOrigin.y;
	for (iControlPoint = 0; iControlPoint < 9; iControlPoint++) {

		length = hypot(pointToTest.x - SelectedFormsLine[iControlPoint].x, pointToTest.y - SelectedFormsLine[iControlPoint].y);
		if (length < minimumLength) {

			minimumLength = length;
			SelectedFormControlVertex = iControlPoint;
		}
	}
	for (iCorner = 0; iCorner < 4; iCorner++) {

		FormLines[iCorner].x = SelectedFormsLine[iCorner << 1].x;
		FormLines[iCorner].y = SelectedFormsLine[iCorner << 1].y;
	}
	FormLines[4].x = FormLines[0].x;
	FormLines[4].y = FormLines[0].y;
	if (minimumLength < CLOSENUF) {

		for (iCorner = 0; iCorner < 4; iCorner++) {

			StretchBoxLine[iCorner].x = SelectedFormsLine[iCorner << 1].x;
			StretchBoxLine[iCorner].y = SelectedFormsLine[iCorner << 1].y;
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
	if (pointToTest.x >= SelectedFormsRect.left&&pointToTest.x <= SelectedFormsRect.right
		&&pointToTest.y >= SelectedFormsRect.top&&pointToTest.y <= SelectedFormsRect.bottom) {

		SelectedFormsSize.x = SelectedFormsRect.right - SelectedFormsRect.left;
		SelectedFormsSize.y = SelectedFormsRect.bottom - SelectedFormsRect.top;
		setMap(MOVFRMS);
		FormMoveDelta.x = pointToTest.x - SelectedFormsRect.left;
		FormMoveDelta.y = pointToTest.y - SelectedFormsRect.top;
		setMap(SHOSTRTCH);
		strtchbox();
		return 1;
	}
	return 0;
}

void delfre() {

	unsigned iStitch, currentStitchCount = 0;

	savdo();
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (!(StitchBuffer[iStitch].attribute&NOTFRM)) {

			StitchBuffer[currentStitchCount].attribute = StitchBuffer[iStitch].attribute;
			StitchBuffer[currentStitchCount].x = StitchBuffer[iStitch].x;
			StitchBuffer[currentStitchCount++].y = StitchBuffer[iStitch].y;
		}
	}
	PCSHeader.stitchCount = currentStitchCount;
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
	unsigned start, finish;

	if (LastFormSelected > ClosestFormToCursor)
	{
		start = ClosestFormToCursor;
		finish = LastFormSelected;
	} else
	{
		start = LastFormSelected;
		finish = ClosestFormToCursor;
	}
	SelectedFormCount = 0;
	while (start <= finish)
	{
		SelectedFormList[SelectedFormCount] = start;
		start++;
		SelectedFormCount++;
	}
}

void selup() {

	unsigned attribute;

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
			attribute = StitchBuffer[ClosestPointIndex].attribute&ATMSK;
			while (ClosestPointIndex < (unsigned)PCSHeader.stitchCount - 1 && (StitchBuffer[ClosestPointIndex].attribute&ATMSK) == attribute)
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

	unsigned attribute;

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
			attribute = StitchBuffer[ClosestPointIndex].attribute&ATMSK;
			while (ClosestPointIndex && (StitchBuffer[ClosestPointIndex].attribute&ATMSK) == attribute)
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

void mvstchs(unsigned destination, unsigned source, unsigned count) {

#if  __UseASM__
	_asm {

		xor		ecx, ecx
		mov		cl, 12
		mov		eax, destination
		mul		ecx
		mov		edi, eax
		add		edi, offset StitchBuffer
		mov		eax, source
		mul		ecx
		mov		esi, eax
		add		esi, offset StitchBuffer
		mov		cl, 3
		mov		eax, count
		mul		ecx
		mov		ecx, eax
		rep		movsd
	}
#else
	memcpy(StitchBuffer + destination, StitchBuffer + source, count * sizeof(*StitchBuffer));
#endif
}

BOOL movstchs(unsigned destination, unsigned start, unsigned finish) {

	unsigned count, swap;
	unsigned dind = MAXITEMS;

	// ToDo - Use a temp buffer rather than the high buffer
	if (destination < (unsigned)PCSHeader.stitchCount - 1)
		destination++;
	if (start > finish) {

		swap = start;
		start = finish;
		finish = swap;
	}
	if (destination >= start&&destination < finish) {

		tabmsg(IDS_DST1);
		return 0;
	}
	if (destination < start) {

		count = finish - start;
		mvstchs(dind, start, count);
		dind += count;
		count = start - destination;
		mvstchs(dind, destination, count);
		dind += count;
		mvstchs(destination, MAXITEMS, dind - (MAXITEMS));
	} else {

		count = destination - finish;
		mvstchs(dind, finish, count);
		dind += count;
		count = finish - start;
		mvstchs(dind, start, count);
		dind += count;
		mvstchs(start, MAXITEMS, dind - (MAXITEMS));
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

	unsigned color, currentStitch, iStitch;

	color = StitchBuffer[0].attribute&COLMSK;
	currentStitch = 0;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (color != (StitchBuffer[iStitch].attribute&COLMSK)) {

			if ((iStitch - currentStitch == 1) && (currentStitch))
				StitchBuffer[iStitch - 1].attribute = StitchBuffer[iStitch].attribute&NCOLMSK | (StitchBuffer[currentStitch - 1].attribute&COLMSK);
			color = StitchBuffer[iStitch].attribute&COLMSK;
			currentStitch = iStitch;
		}
	}
}

unsigned makbig(unsigned start, unsigned finish) {

	unsigned	destination, source, stitchCount, iStitch, adcnt = 0, attribute;
	fPOINTATTR*	sourceStitch;
	fPOINTATTR*	destinationStitch;
	double		length;
	dPOINT		delta, step, point;

	// ToDo - Use a temp buffer rather than the high buffer
	destination = MAXITEMS;
	for (source = start; source < (unsigned)finish - 1; source++) {

		delta.x = StitchBuffer[source + 1].x - StitchBuffer[source].x;
		delta.y = StitchBuffer[source + 1].y - StitchBuffer[source].y;
		length = hypot(delta.x, delta.y);
		mvstch(destination++, source);
		if (length > IniFile.maxStitchLength) {

			stitchCount = ceil(length / UserStitchLength);
			step.x = delta.x / stitchCount;
			step.y = delta.y / stitchCount;
			point.x = StitchBuffer[source].x + step.x;
			point.y = StitchBuffer[source].y + step.y;
			attribute = StitchBuffer[source].attribute;
			if (attribute != StitchBuffer[source + 1].attribute) {

				if (!(attribute&NOTFRM) && StitchBuffer[source + 1].attribute&TYPMSK) {

					if (!((attribute&FRMSK) == (StitchBuffer[source + 1].attribute&FRMSK)))
						attribute &= NTYPMSK;
				} else
					attribute &= NTYPMSK;
			}
			attribute &= (~KNOTMSK);
			for (iStitch = 0; iStitch < stitchCount - 1; iStitch++) {

				StitchBuffer[destination].attribute = attribute;
				StitchBuffer[destination].x = point.x;
				StitchBuffer[destination++].y = point.y;
				point.x += step.x;
				point.y += step.y;
				adcnt++;
			}
		}
	}
	while (source < PCSHeader.stitchCount)
		mvstch(destination++, source++);
	sourceStitch = &StitchBuffer[MAXITEMS];
	destinationStitch = &StitchBuffer[start];
	stitchCount = destination - (MAXITEMS);
	for (source = 0; source < stitchCount; source++)
		destinationStitch[source] = sourceStitch[source];
	PCSHeader.stitchCount = start + stitchCount;
	return adcnt;
}

void rembig() {

	RANGE range;
	unsigned iForm;

	if (UserStitchLength < IniFile.maxStitchLength) {

		savdo();
		if (SelectedFormCount) {

			for (iForm = 0; iForm < SelectedFormCount; iForm++) {

				frmrng(SelectedFormList[iForm], &range);
				makbig(range.start, range.finish);
			}
			goto bigdun;
		}
		if (chkMap(FORMSEL)) {

			frmrng(ClosestFormToCursor, &range);
			makbig(range.start, range.finish);
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

	unsigned	iStitch, currentStitch;
	double		length, minimumLength = 1e99;
	double		currentLength;

	if (ClosestPointIndex == LargestStitchIndex)
		return;
	currentLength = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x, StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (iStitch = ClosestPointIndex + 1; iStitch < SelectedRange.finish; iStitch++) {

		length = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
		if (length == currentLength)
			goto long1;
	}
	for (currentStitch = SelectedRange.start; currentStitch < SelectedRange.finish - 1; currentStitch++) {

		length = hypot(StitchBuffer[currentStitch + 1].x - StitchBuffer[currentStitch].x, StitchBuffer[currentStitch + 1].y - StitchBuffer[currentStitch].y);
		if (length > currentLength&&length < minimumLength) {

			minimumLength = length;
			iStitch = currentStitch;
		}
	}
	if (minimumLength == 1e99)
		return;
long1:;
	CurrentStitchIndex = iStitch;
	lensadj();
	ritnum(STR_NUMSCH, ClosestPointIndex);
}

void shorter() {

	unsigned	iStitch, currentStitch;
	double		length, maximumLength = 0;
	double		currentLength;

	if (ClosestPointIndex == SmallestStitchIndex)
		return;
	currentLength = hypot(StitchBuffer[ClosestPointIndex + 1].x - StitchBuffer[ClosestPointIndex].x, StitchBuffer[ClosestPointIndex + 1].y - StitchBuffer[ClosestPointIndex].y);
	for (currentStitch = ClosestPointIndex; currentStitch != 0; currentStitch--) {

		length = hypot(StitchBuffer[currentStitch].x - StitchBuffer[currentStitch - 1].x, StitchBuffer[currentStitch].y - StitchBuffer[currentStitch - 1].y);
		if (length == currentLength) {
			currentStitch--;
			goto short1;
		}
	}
	for (iStitch = SelectedRange.start; iStitch < SelectedRange.finish - 1; iStitch++) {

		length = hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y);
		if (length<currentLength&&length>maximumLength) {

			maximumLength = length;
			currentStitch = iStitch;
		}
	}
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", hypot(StitchBuffer[iStitch + 1].x - StitchBuffer[iStitch].x, StitchBuffer[iStitch + 1].y - StitchBuffer[iStitch].y));
	butxt(HMINLEN, MsgBuffer);
short1:;
	CurrentStitchIndex = currentStitch;
	lensadj();
	ritnum(STR_NUMSCH, ClosestPointIndex);
}

void setsrch(unsigned stitch) {

	CurrentStitchIndex = stitch;
	lensadj();
	ritnum(STR_NUMSCH, ClosestPointIndex);
}

BOOL inrng(unsigned stitch) {

	if (StitchBuffer[stitch].x >= StitchRangeRect.left
		&&StitchBuffer[stitch].x <= StitchRangeRect.right
		&&StitchBuffer[stitch].y >= StitchRangeRect.bottom
		&&StitchBuffer[stitch].y <= StitchRangeRect.top)

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

	unsigned iStitch;

	if (rstMap(GRPSEL)) {

		rngadj();
		ClosestPointIndex = GroupStartStitch;
		setMap(SELBOX);
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

				if (!(StitchBuffer[iStitch].attribute&NOTFRM) && ((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT) == ClosestFormToCursor) {

					ClosestPointIndex = iStitch;
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

	unsigned iStitch;

	if (rstMap(GRPSEL)) {

		rngadj();
		ClosestPointIndex = GroupEndStitch;
		setMap(SELBOX);
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			for (iStitch = PCSHeader.stitchCount; iStitch != 0; iStitch--) {

				if (!(StitchBuffer[iStitch - 1].attribute&NOTFRM) && ((StitchBuffer[iStitch - 1].attribute&FRMSK) >> FRMSHFT) == ClosestFormToCursor) {

					ClosestPointIndex = iStitch - 1;
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

void sizstch(fRECTANGLE* rectangle) {

	unsigned iStitch;

	if (PCSHeader.stitchCount) {

		rectangle->bottom = rectangle->top = RotatedStitches[0].y;
		rectangle->left = rectangle->right = RotatedStitches[0].x;
		for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (RotatedStitches[iStitch].x < rectangle->left)
				rectangle->left = RotatedStitches[iStitch].x;
			if (RotatedStitches[iStitch].x > rectangle->right)
				rectangle->right = RotatedStitches[iStitch].x;
			if (RotatedStitches[iStitch].y < rectangle->bottom)
				rectangle->bottom = RotatedStitches[iStitch].y;
			if (RotatedStitches[iStitch].y > rectangle->top)
				rectangle->top = RotatedStitches[iStitch].y;
		}
	} else
		rectangle->top = rectangle->bottom = rectangle->left = rectangle->right = 0;
}

void hupfn() {

	unsigned	iForm, iVertex, iStitch;
	dPOINT		hoopCenter;
	dPOINT		designCenter;
	dPOINT		hoopSize;
	dPOINT		delta;

	rstMap(HUPCHNG);
	RotatedStitches = StitchBuffer;
	sizstch(&CheckHoopRect);
	if (FormIndex) {

		if (!PCSHeader.stitchCount) {

			CheckHoopRect.bottom = CheckHoopRect.top = CurrentFormVertices[0].y;
			CheckHoopRect.left = CheckHoopRect.right = CurrentFormVertices[0].x;
		}
		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {

			if (FormVertices[iVertex].x < CheckHoopRect.left)
				CheckHoopRect.left = FormVertices[iVertex].x;
			if (FormVertices[iVertex].x > CheckHoopRect.right)
				CheckHoopRect.right = FormVertices[iVertex].x;
			if (FormVertices[iVertex].y < CheckHoopRect.bottom)
				CheckHoopRect.bottom = FormVertices[iVertex].y;
			if (FormVertices[iVertex].y > CheckHoopRect.top)
				CheckHoopRect.top = FormVertices[iVertex].y;
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

			hoopSize.x = CheckHoopRect.right - CheckHoopRect.left;
			hoopSize.y = CheckHoopRect.top - CheckHoopRect.bottom;
			if (hoopSize.x > IniFile.hoopSizeX) {

				IniFile.hoopSizeX = hoopSize.x;
				setMap(HUPCHNG);
			}
			if (hoopSize.y > IniFile.hoopSizeY) {

				IniFile.hoopSizeY = hoopSize.y;
				setMap(HUPCHNG);
			}
			designCenter.x = hoopSize.x / 2 + CheckHoopRect.left;
			designCenter.y = hoopSize.y / 2 + CheckHoopRect.bottom;
			hoopCenter.x = IniFile.hoopSizeX / 2;
			hoopCenter.y = IniFile.hoopSizeY / 2;
			delta.x = hoopCenter.x - designCenter.x;
			delta.y = hoopCenter.y - designCenter.y;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

				StitchBuffer[iStitch].x += delta.x;
				StitchBuffer[iStitch].y += delta.y;
			}
			for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {

				FormVertices[iVertex].x += delta.x;
				FormVertices[iVertex].y += delta.y;
			}
			for (iForm = 0; iForm < FormIndex; iForm++) {

				FormList[iForm].rectangle.left += delta.x;
				FormList[iForm].rectangle.right += delta.x;
				FormList[iForm].rectangle.top += delta.y;
				FormList[iForm].rectangle.bottom += delta.y;
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

void strlcpy(TCHAR* destination, TCHAR* source) {

#if  __UseASM__
	_asm {

		mov		ebx, offset UpperCaseMap
		mov		ecx, destination
		mov		edx, source
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
		TCHAR character = *(source++);

		if (character == 0)
			break;

		character &= 0x7f;

		if (_bittest((long *)UpperCaseMap, character))
			character |= 0x20;

		*(destination++) = character;
	}

	*destination = 0;
#endif
}

TCHAR lchr(int character) {

#if  __UseASM__
	_asm {

		mov		ebx, offset UpperCaseMap
		mov		eax, character
		and		eax, 0x7f
		bt[ebx], eax
		jnc		short lchrx
		or al, 0x20
		lchrx:
	}
#else
	TCHAR tchar = character & 0x7f;

	return _bittest((long *)UpperCaseMap, tchar) ? tchar : tchar | 0x20;
#endif
}

void barnam(HWND window, unsigned iThumbnail) {

	TCHAR		buffer[_MAX_PATH];
	TCHAR*		lastCharacter;

	if (iThumbnail < ThumbnailDisplayCount&&ThumbnailsSelected[iThumbnail]) {

		strcpy_s(buffer, ThumbnailsSelected[iThumbnail]);
		lastCharacter = strrchr(buffer, '.');
		if (lastCharacter)
			lastCharacter[0] = 0;
		SetWindowText(window, buffer);
	} else
		SetWindowText(window, "");
}

void rthumnam(unsigned iThumbnail) {

	switch (iThumbnail) {

	case 0:

		barnam(ButtonWin[HNUM], iThumbnail);
		break;

	case 1:

		barnam(ButtonWin[HTOT], iThumbnail);
		break;

	case 2:

		barnam(ButtonWin[HMINLEN], iThumbnail);
		break;

	case 3:

		barnam(ButtonWin[HMAXLEN], iThumbnail);
		break;
	}
}

#pragma warning(push)
#pragma warning(disable : 4996)
void thumnail() {

	WIN32_FIND_DATA	fileData;
	unsigned		iThumbnail, iThumbnailName;
	HANDLE			file;
	TCHAR*			lastCharacter;

	unbsho();
	undat();
	untrace();
	// ToDo - Check for a better value than MAXITEMS/2
	ThumbnailNames = new TCHAR[MAXITEMS >> 1];
	Thumbnails = new TCHAR*[MAXITEMS >> 1];

	SetCurrentDirectory(DefaultDirectory);
	strcpy_s(SearchName, DefaultDirectory);
	lastCharacter = &SearchName[strlen(SearchName) - 1];
	if (lastCharacter[0] != '\\') {

		lastCharacter[1] = '\\';
		lastCharacter[2] = 0;
	}
	strcat_s(SearchName, "*.thr");
	file = FindFirstFile(SearchName, &fileData);
	if (file == INVALID_HANDLE_VALUE) {

		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Can't find %s\n", SearchName);
		shoMsg(MsgBuffer);
		unthum();
	} else {

		iThumbnailName = 0;
		Thumbnails[0] = ThumbnailNames;
		strcpy(Thumbnails[0], fileData.cFileName);
		iThumbnailName += strlen(fileData.cFileName) + 1;
		Thumbnails[1] = &ThumbnailNames[iThumbnailName];
		iThumbnail = 1;
		while (FindNextFile(file, &fileData)) {

			strcpy(Thumbnails[iThumbnail], fileData.cFileName);
			iThumbnailName += strlen(fileData.cFileName) + 1;
			Thumbnails[++iThumbnail] = &ThumbnailNames[iThumbnailName];
		}
		FindClose(file);
		ThumbnailCount = iThumbnail;
		qsort(Thumbnails, iThumbnail, sizeof(TCHAR *), strcomp);
		iThumbnail = ThumbnailIndex = 0;
		while (iThumbnail < 4 && ThumbnailIndex < ThumbnailCount&&iThumbnail < ThumbnailCount) {

			ThumbnailsSelected[iThumbnail] = Thumbnails[iThumbnail];
			iThumbnail++;
		}
		ThumbnailIndex = ThumbnailDisplayCount = iThumbnail;
		while (iThumbnail < 4 && iThumbnail < ThumbnailCount)
			rthumnam(iThumbnail++);
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

	unsigned iThumbnail, length, savedIndex;

	if (ThumbnailIndex < ThumbnailCount) {

		savedIndex = ThumbnailIndex;
		iThumbnail = 0;
		length = strlen(ThumbnailSearchString);
		setMap(RESTCH);
		if (length) {

			while (iThumbnail < 4 && ThumbnailIndex < ThumbnailCount) {

				if (!strncmp(ThumbnailSearchString, Thumbnails[ThumbnailIndex], length)) {

					ThumbnailsSelected[iThumbnail] = Thumbnails[ThumbnailIndex];
					redraw(BackupViewer[iThumbnail++]);
				}
				ThumbnailIndex++;
			}
		} else {

			while (iThumbnail < 4 && ThumbnailIndex < ThumbnailCount) {

				ThumbnailsSelected[iThumbnail] = Thumbnails[ThumbnailIndex];
				redraw(BackupViewer[iThumbnail++]);
				ThumbnailIndex++;
			}
		}
		if (iThumbnail) {

			ThumbnailDisplayCount = iThumbnail;
			while (iThumbnail < 4)
				rthumnam(iThumbnail++);
		} else
			ThumbnailIndex = savedIndex;
	}
}

void nuthbak(unsigned count) {

	unsigned length;

	if (ThumbnailIndex) {

		length = strlen(ThumbnailSearchString);
		if (length) {

			while (count&&ThumbnailIndex < MAXFORMS) {

				if (ThumbnailIndex) {

					ThumbnailIndex--;
					if (!strncmp(ThumbnailSearchString, Thumbnails[ThumbnailIndex], length))
						count--;
				} else
					break;
			}
		} else
			ThumbnailIndex -= count;
		if (ThumbnailIndex > MAXFORMS)
			ThumbnailIndex = 0;
		nuthsel();
	}
}

void nuthum(TCHAR character) {

	unsigned iString;

	iString = strlen(ThumbnailSearchString);
	if (iString < 16) {

		setMap(RESTCH);
		ThumbnailSearchString[iString++] = character;
		ThumbnailSearchString[iString] = 0;
		butxt(HBOXSEL, ThumbnailSearchString);
		ThumbnailIndex = 0;
		nuthsel();
	}
}

void bakthum() {

	unsigned searchStringLength;

	searchStringLength = strlen(ThumbnailSearchString);
	if (searchStringLength) {

		setMap(RESTCH);
		ThumbnailSearchString[--searchStringLength] = 0;
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

void insflin(POINT insertPoint) {

	POINT offset;

	offset.x = InsertSize.x >> 1;
	offset.y = InsertSize.y >> 1;

	FormLines[0].x = FormLines[3].x = FormLines[4].x = insertPoint.x - offset.x;
	FormLines[1].x = FormLines[2].x = insertPoint.x + offset.x;
	FormLines[0].y = FormLines[1].y = FormLines[4].y = insertPoint.y - offset.y;
	FormLines[2].y = FormLines[3].y = insertPoint.y + offset.y;
}

BOOL isthr(TCHAR* filename) {

	TCHAR* lastCharacter;

	lastCharacter = strrchr(filename, '.');
	if (lastCharacter) {

		lastCharacter++;
		if (!_strnicmp(lastCharacter, "th", 2))
			return 1;
		else
			return 0;
	} else
		return 1;
}

unsigned gethand(fPOINTATTR* stitch, unsigned stitchCount) {

	unsigned iStitch;
	unsigned userStitchCount;

	userStitchCount = 0;
	for (iStitch = 0; iStitch < stitchCount; iStitch++) {

		if (stitch[iStitch].attribute&USMSK)
			userStitchCount++;
	}
	return userStitchCount;
}

void insfil() {

	OPENFILENAME file = {

		sizeof(OPENFILENAME),	//lStructsize
		ThrEdWindow,			//hwndOwner 
		ThrEdInstance,			//hInstance 
		"THR files\0*.thr\0\0",	//lpstrFilter 
		CustomFilter,			//lpstrCustomFilter 
		_MAX_PATH,				//nMaxCustFilter 
		0,						//nFilterIndex 
		InsertedFileName,		//lpstrFile 
		_MAX_PATH,				//nMaxFile 
		0,						//lpstrFileTitle 
		0,						//nMaxFileTitle 
		DefaultDirectory,		//lpstr	ialDir 
		0,						//lpstrTitle
		OFN_EXPLORER | OFN_OVERWRITEPROMPT,//Flags
		0,						//nFileOffset
		0,						//nFileExtension 
		"thr",					//lpstrDefExt 
		0,						//lCustData 
		0,						//lpfnHook
		0,						//lpTemplateName
	};
	STRHED		fileHeader;
	STREX		thredHeader = { 0 };
	PCSHEADER	pcsFileHeader;
	unsigned	iForm, iFormList, iVertex, iStitch, iName, iPCSStitch;
	unsigned	newStitchCount, newAttribute, encodedFormIndex;
	fRECTANGLE	insertedRectangle;
	fPOINT		insertedSize;
	POINT		initialInsertPoint;
	PCSTCH*		pcsStitchBuffer;
	double		homscor = 0;
	double		filscor;
	unsigned	version;
	FRMHEDO*	formHeader;

	if (chkMap(IGNORINS) || GetOpenFileName(&file)) {

		InsertedFileHandle = CreateFile(InsertedFileName, (GENERIC_READ), 0, NULL,
			OPEN_EXISTING, 0, NULL);
		if (InsertedFileHandle == INVALID_HANDLE_VALUE) {

			filnopn(IDS_FNOPN, InsertedFileName);
			FileHandle = 0;
			CloseHandle(InsertedFileHandle);
		} else {

			InsertedStitchCount = PCSHeader.stitchCount;
			if (isthr(InsertedFileName)) {

				ReadFile(InsertedFileHandle, (STRHED*)&fileHeader, sizeof(STRHED), &BytesRead, NULL);
				if ((fileHeader.headerType & 0xffffff) != 0x746872)
					tabmsg(IDS_NOTHR);
				else {

					version = (fileHeader.headerType & 0xff000000) >> 24;
					if (version) {
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
						ReadFile(InsertedFileHandle, (STREX*)&thredHeader, sizeof(STREX), &BytesRead, 0);
					}
					savdo();
					ReadFile(InsertedFileHandle, (fPOINTATTR*)&StitchBuffer[PCSHeader.stitchCount], fileHeader.stitchCount * sizeof(fPOINTATTR), &BytesRead, NULL);
					SetFilePointer(InsertedFileHandle, 164, 0, FILE_CURRENT);
					insertedRectangle.left = insertedRectangle.bottom = (float)1e9;
					insertedRectangle.top = insertedRectangle.right = (float)1e-9;
					encodedFormIndex = FormIndex << FRMSHFT;
					InsertedVertexIndex = FormVertexIndex; 
					InsertedFormIndex = FormIndex;
					if (fileHeader.vertexCount) {

						if (version < 2) {
							// Todo - Allocate memory locally for formHeader and replace use of BSequence in ReadFile
							formHeader = (FRMHEDO*)&BSequence;
							ReadFile(InsertedFileHandle, (FRMHEDO*)&BSequence, fileHeader.formCount * sizeof(FRMHEDO), &BytesRead, 0);
							if (BytesRead != fileHeader.formCount * sizeof(FRMHEDO)) {

								FormIndex = BytesRead / sizeof(FRMHEDO);
								setMap(BADFIL);
							}
							if (FormIndex + fileHeader.vertexCount < MAXFORMS) {

								FillMemory(&FormList[FormIndex], fileHeader.formCount * sizeof(FRMHED), 0);
								iFormList = FormIndex;
								for (iForm = 0; iForm < fileHeader.formCount; iForm++) {

									MoveMemory(&FormList[iFormList], &formHeader[iForm], sizeof(FRMHEDO));
									iFormList++;
								}
							}
						} else
							ReadFile(InsertedFileHandle, (FRMHED*)&FormList[FormIndex], fileHeader.formCount * sizeof(FRMHED), &BytesRead, 0);
						ReadFile(InsertedFileHandle, (fPOINT*)&FormVertices[FormVertexIndex], fileHeader.vertexCount * sizeof(fPOINT), &BytesRead, 0);
						ReadFile(InsertedFileHandle, (SATCON*)&SatinConnects[SatinConnectIndex], fileHeader.dlineCount * sizeof(SATCON), &BytesRead, 0);
						ReadFile(InsertedFileHandle, (fPOINT*)&ClipPoints[ClipPointIndex], fileHeader.clipDataCount * sizeof(fPOINT), &BytesRead, 0);
						CloseHandle(InsertedFileHandle);
						InsertedFileHandle = 0;
						for (iFormList = FormIndex; iFormList < FormIndex + fileHeader.formCount; iFormList++) {

							FormList[iFormList].vertices = adflt(FormList[iFormList].vertexCount);
							if (FormList[iFormList].type == SAT) {

								if (FormList[iFormList].satinGuideCount)
									FormList[iFormList].satinOrAngle.guide = adsatk(FormList[iFormList].satinGuideCount);
								if (isclpx(iFormList))
									FormList[iFormList].angleOrClipData.clip = adclp(FormList[iFormList].lengthOrCount.clipCount);
							}
							if (isclp(iFormList))
								FormList[iFormList].angleOrClipData.clip = adclp(FormList[iFormList].lengthOrCount.clipCount);
							if (iseclpx(iFormList))
								FormList[iFormList].borderClipData = adclp(FormList[iFormList].clipEntries);
						}
						FormIndex += fileHeader.formCount;
						if (fileHeader.formCount) {

							insertedRectangle.left = insertedRectangle.right = FormVertices[InsertedVertexIndex].x;
							insertedRectangle.bottom = insertedRectangle.top = FormVertices[InsertedVertexIndex].y;
							for (iVertex = InsertedVertexIndex + 1; iVertex < FormVertexIndex; iVertex++) {

								if (FormVertices[iVertex].x < insertedRectangle.left)
									insertedRectangle.left = FormVertices[iVertex].x;
								if (FormVertices[iVertex].x > insertedRectangle.right)
									insertedRectangle.right = FormVertices[iVertex].x;
								if (FormVertices[iVertex].y < insertedRectangle.bottom)
									insertedRectangle.bottom = FormVertices[iVertex].y;
								if (FormVertices[iVertex].y > insertedRectangle.top)
									insertedRectangle.top = FormVertices[iVertex].y;
							}
						}
					}
					if (fileHeader.stitchCount) {

						for (iStitch = PCSHeader.stitchCount; iStitch < (unsigned)PCSHeader.stitchCount + fileHeader.stitchCount; iStitch++) {

							if (StitchBuffer[iStitch].attribute&ALTYPMSK) {

								newAttribute = (StitchBuffer[iStitch].attribute&FRMSK) + encodedFormIndex;
								StitchBuffer[iStitch].attribute &= NFRMSK;
								StitchBuffer[iStitch].attribute |= newAttribute;
							}
							if (StitchBuffer[iStitch].x < insertedRectangle.left)
								insertedRectangle.left = StitchBuffer[iStitch].x;
							if (StitchBuffer[iStitch].x > insertedRectangle.right)
								insertedRectangle.right = StitchBuffer[iStitch].x;
							if (StitchBuffer[iStitch].y < insertedRectangle.bottom)
								insertedRectangle.bottom = StitchBuffer[iStitch].y;
							if (StitchBuffer[iStitch].y > insertedRectangle.top)
								insertedRectangle.top = StitchBuffer[iStitch].y;
						}
					}
					if (fileHeader.headerType & 0x1000000) {
						// ToDo - should FRMPW be sizeof(fPOINTATTR) etc.?
						filscor = (double)fileHeader.formCount*FRMW +
							gethand(&StitchBuffer[PCSHeader.stitchCount], fileHeader.stitchCount)*HANDW +
							fileHeader.vertexLen*FRMPW +
							fileHeader.stitchCount*STCHW;
						if (filscor > homscor) {

							for (iName = 0; iName < 50; iName++)
								ExtendedHeader.creatorName[iName] = thredHeader.creatorName[iName];
							redfnam(DesignerName);
							sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_THRDBY], ThrName, DesignerName);
							SetWindowText(ThrEdWindow, MsgBuffer);
						}
					}
					InsertCenter.x = (insertedRectangle.right - insertedRectangle.left) / 2 + insertedRectangle.left;
					InsertCenter.y = (insertedRectangle.top - insertedRectangle.bottom) / 2 + insertedRectangle.bottom;
					PCSHeader.stitchCount += fileHeader.stitchCount;
					insertedSize.x = insertedRectangle.right - insertedRectangle.left;
					insertedSize.y = insertedRectangle.top - insertedRectangle.bottom;
					ratsr();
					InsertSize.x = insertedSize.x*HorizontalRatio;
					// ToDo - Should this be vertical ratio?
					InsertSize.y = insertedSize.y*HorizontalRatio;
					initialInsertPoint.x = StitchWindowClientRect.right >> 1;
					initialInsertPoint.y = StitchWindowClientRect.bottom >> 1;
					insflin(initialInsertPoint);
					NewFormVertexCount = 5;
					setMap(SHOFRM);
					setMap(INSFIL);
					dufrm();
				}
			} else {

				ReadFile(InsertedFileHandle, (PCSHEADER*)&pcsFileHeader, 0x46, &BytesRead, NULL);
				if (PCSHeader.leadIn == 0x32 && PCSHeader.colorCount == 16) {

					savdo();
					pcsStitchBuffer = (PCSTCH*)BSequence;
					ReadFile(InsertedFileHandle, (PCSTCH*)pcsStitchBuffer, pcsFileHeader.stitchCount * sizeof(PCSTCH), &BytesRead, NULL);
					iStitch = PCSHeader.stitchCount;
					newAttribute = 0;
					for (iPCSStitch = 0; iPCSStitch < pcsFileHeader.stitchCount; iPCSStitch++) {

						if (pcsStitchBuffer[iPCSStitch].tag == 3)
							newAttribute = pcsStitchBuffer[iPCSStitch++].fx;
						else {

							StitchBuffer[iStitch].x = pcsStitchBuffer[iPCSStitch].x + (float)pcsStitchBuffer[iPCSStitch].fx / 256;
							StitchBuffer[iStitch].y = pcsStitchBuffer[iPCSStitch].y + (float)pcsStitchBuffer[iPCSStitch].fy / 256;
							StitchBuffer[iStitch++].attribute = newAttribute;
						}
					}
					newStitchCount = iStitch;
					iStitch = PCSHeader.stitchCount;
					insertedRectangle.left = insertedRectangle.right = StitchBuffer[iPCSStitch].x;
					insertedRectangle.top = insertedRectangle.bottom = StitchBuffer[iPCSStitch++].y;
					while (iStitch < (unsigned)newStitchCount) {

						if (StitchBuffer[iStitch].x < insertedRectangle.left)
							insertedRectangle.left = StitchBuffer[iStitch].x;
						if (StitchBuffer[iStitch].x > insertedRectangle.right)
							insertedRectangle.right = StitchBuffer[iStitch].x;
						if (StitchBuffer[iStitch].y < insertedRectangle.bottom)
							insertedRectangle.bottom = StitchBuffer[iStitch].y;
						if (StitchBuffer[iStitch].y > insertedRectangle.top)
							insertedRectangle.top = StitchBuffer[iStitch].y;
						iStitch++;
					}
					InsertCenter.x = (insertedRectangle.right - insertedRectangle.left) / 2 + insertedRectangle.left;
					InsertCenter.y = (insertedRectangle.top - insertedRectangle.bottom) / 2 + insertedRectangle.bottom;
					PCSHeader.stitchCount = newStitchCount;
					insertedSize.x = insertedRectangle.right - insertedRectangle.left;
					insertedSize.y = insertedRectangle.top - insertedRectangle.bottom;
					ratsr();
					InsertSize.x = insertedSize.x*HorizontalRatio;
					// ToDo - Should this be vertical ratio?
					InsertSize.y = insertedSize.y*HorizontalRatio;
					initialInsertPoint.x = StitchWindowClientRect.right >> 1;
					initialInsertPoint.y = StitchWindowClientRect.bottom >> 1;
					insflin(initialInsertPoint);
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

	fPOINT		offset;
	unsigned	iForm, iVertex, iStitch;
	fRECTANGLE*	formRectangle;

	px2stch();
	offset.x = SelectedPoint.x - InsertCenter.x;
	offset.y = SelectedPoint.y - InsertCenter.y;
	for (iForm = InsertedFormIndex; iForm < FormIndex; iForm++) {

		formRectangle = &FormList[iForm].rectangle;
		formRectangle->bottom += offset.y;
		formRectangle->top += offset.y;
		formRectangle->left += offset.x;
		formRectangle->right += offset.x;
	}
	for (iVertex = InsertedVertexIndex; iVertex < FormVertexIndex; iVertex++) {

		FormVertices[iVertex].x += offset.x;
		FormVertices[iVertex].y += offset.y;
	}
	for (iStitch = InsertedStitchCount; iStitch < PCSHeader.stitchCount; iStitch++) {

		StitchBuffer[iStitch].x += offset.x;
		StitchBuffer[iStitch].y += offset.y;
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

	unsigned	iStitch = 0, iRange=0, rangeCount;
	bool		flagInRange = false;
	unsigned	largestRange = 0, maximumLength, length;
	RANGE*		prng;

	if (!rstMap(WASFPNT)) {

		rstMap(GRPSEL);
		prng = new RANGE[MAXITEMS];
		rstMap(GRPSEL);
		while (iStitch < PCSHeader.stitchCount) {

			if (inrng(iStitch)) {

				if (!flagInRange) {

					prng[iRange].start = iStitch;
					flagInRange = true;
				}
			} else {

				if (flagInRange) {

					prng[iRange].finish = iStitch - 1;
					iRange++;
					flagInRange = false;
				}
			}
			iStitch++;
		}
		if (flagInRange) {

			prng[iRange].finish = iStitch - 1;
			iRange++;
		}
		rangeCount = iRange;
		if (rangeCount) {

			maximumLength = 0;
			for (iRange = 0; iRange < rangeCount; iRange++) {

				length = prng[iRange].finish - prng[iRange].start;
				if (length > maximumLength) {

					maximumLength = length;
					largestRange = iRange;
				}
			}
			if (maximumLength) {

				ClosestPointIndex = prng[largestRange].start;
				GroupStitchIndex = prng[largestRange].finish;
				setMap(GRPSEL);
			}
			gotbox();
		}
		delete[] prng;
	}
}

void nucols() {

	unsigned iForm, iStitch;

	clRmap((FormIndex << 5) + 1);
	for (iForm = 0; iForm < SelectedFormCount; iForm++) {

		setr(SelectedFormList[iForm]);
		SelectedForm = &FormList[iForm];
		if (SelectedForm->fillType) {

			SelectedForm->fillColor = ActiveColor;
			SelectedForm->fillInfo.feather.color = ActiveColor;
		}
		if (SelectedForm->edgeType)
			SelectedForm->borderColor = ActiveColor;
		if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
			SelectedForm->underlayColor = ActiveColor;
	}
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if (chkr((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT) && (StitchBuffer[iStitch].attribute&TYPMSK) != TYPMSK) {

			StitchBuffer[iStitch].attribute &= NCOLMSK;
			StitchBuffer[iStitch].attribute |= ActiveColor;
		}
	}
}

BOOL dunum(unsigned code) {

	if (code >= '0'&&code <= '9') {

		NumericCode = code;
		return 1;
	}
	if (code >= VK_NUMPAD0 && code <= VK_NUMPAD9) {

		NumericCode = code - '0';
		return 1;
	}
	return 0;
}

void stchrct(fRECTANGLE* rectangle)
{
	unsigned ind;

	if (PCSHeader.stitchCount)
	{
		rectangle->bottom = rectangle->left = 1e10;
		rectangle->top = rectangle->right = 0;
		if (PCSHeader.stitchCount) {

			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

				if (StitchBuffer[ind].x < rectangle->left)
					rectangle->left = StitchBuffer[ind].x;
				if (StitchBuffer[ind].x > rectangle->right)
					rectangle->right = StitchBuffer[ind].x;
				if (StitchBuffer[ind].y < rectangle->bottom)
					rectangle->bottom = StitchBuffer[ind].y;
				if (StitchBuffer[ind].y > rectangle->top)
					rectangle->top = StitchBuffer[ind].y;
			}
		}
	}
}

void frmrct(fRECTANGLE* rectangle)
{
	unsigned iVertex;

	rectangle->left = rectangle->right = FormVertices[0].x;
	rectangle->top = rectangle->bottom = FormVertices[0].y;
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {

		if (FormVertices[iVertex].x < rectangle->left)
			rectangle->left = FormVertices[iVertex].x;
		if (FormVertices[iVertex].x > rectangle->right)
			rectangle->right = FormVertices[iVertex].x;
		if (FormVertices[iVertex].y > rectangle->top)
			rectangle->top = FormVertices[iVertex].y;
		if (FormVertices[iVertex].y < rectangle->bottom)
			rectangle->bottom = FormVertices[iVertex].y;
	}
}

void desiz() {

	fRECTANGLE	rectangle;
	FLOAT		xSize, ySize;
	TCHAR*		message;

	message = MsgBuffer;
	if (PCSHeader.stitchCount) {

		stchrct(&rectangle);
		xSize = (rectangle.right - rectangle.left) / PFGRAN;
		ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
		if ((rectangle.left < 0) || (rectangle.bottom < 0) || (rectangle.right > IniFile.hoopSizeX) || (rectangle.top > IniFile.hoopSizeY)) {

			strcpy_s(MsgBuffer, StringTable[STR_STCHOUT]);
			message = &MsgBuffer[strlen(MsgBuffer)];
		}
#if LANG==ENG||LANG==HNG

		sprintf_s(message, sizeof(MsgBuffer),
			StringTable[STR_STCHS],
			PCSHeader.stitchCount,
			xSize, xSize / 25.4,
			ySize, ySize / 25.4);
#endif

#if LANG==GRM

		sprintf_s(message, sizeof(MsgBuffer),
			StringTable[STR_STCHS],
			PCSHeader.stitchCount,
			xSize,
			ySize);
#endif
		message = &message[strlen(message)];
	}
	if (FormIndex) {

		frmrct(&rectangle);
		xSize = (rectangle.right - rectangle.left) / PFGRAN;
		ySize = (rectangle.top - rectangle.bottom) / PFGRAN;
#if LANG==ENG||LANG==HNG

		sprintf_s(message, sizeof(MsgBuffer), StringTable[STR_FORMS],
			FormIndex,
			xSize, xSize / 25.4,
			ySize, ySize / 25.4);
#endif

#if LANG==GRM

		sprintf_s(message, sizeof(MsgBuffer), StringTable[STR_FORMS],
			FormIndex,
			xSize,
			ySize);
#endif
		message = &message[strlen(message)];
	}
	sprintf_s(message, sizeof(MsgBuffer), StringTable[STR_HUPWID],
		IniFile.hoopSizeX / PFGRAN,
		IniFile.hoopSizeY / PFGRAN);
	message = &message[strlen(message)];
	if (PCSHeader.stitchCount) {

		sprintf_s(message, sizeof(MsgBuffer), StringTable[STR_CREATBY],
			DesignerName,
			ExtendedHeader.modifierName);
	}
	shoMsg(MsgBuffer);
}

void sidhup() {

	RECT		hoopRectangle;
	RECT		preferencesRectangle;
	unsigned	iHoop;

	setMap(HUPMSG);
	GetWindowRect(ValueWindow[PHUP], &hoopRectangle);
	GetWindowRect(PreferencesWindow, &preferencesRectangle);
	SideMessageWindow = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		preferencesRectangle.right + 3 - ThredWindowOrigin.x,
		hoopRectangle.top - ThredWindowOrigin.y,
		ButtonWidthX3 + ButtonWidth * 2 + 6,
		ButtonHeight*HUPS + 6,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
	for (iHoop = 0; iHoop < HUPS; iHoop++) {

		SideWindow[iHoop] = CreateWindow(
			"STATIC",
			StringTable[iHoop + STR_HUP0],
			SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
			3,
			ButtonHeight*iHoop + 3,
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

void clpradj(fPOINTATTR stitch) {

	if (stitch.x < ClipRectAdjusted.left)
		ClipRectAdjusted.left = stitch.x;
	if (stitch.x > ClipRectAdjusted.right)
		ClipRectAdjusted.right = stitch.x;
	if (stitch.y < ClipRectAdjusted.bottom)
		ClipRectAdjusted.bottom = stitch.y;
	if (stitch.y > ClipRectAdjusted.top)
		ClipRectAdjusted.top = stitch.y;
}

void clpadj() {

	double		clipMiddle;
	unsigned	iStitch;

	if (chkMap(GRPSEL)) {

		rngadj();
		iStitch = GroupStartStitch;
		ClipRectAdjusted.left = ClipRect.right = StitchBuffer[iStitch++].x;
		ClipRectAdjusted.top = ClipRectAdjusted.bottom = StitchBuffer[iStitch].y;
		clpradj(StitchBuffer[iStitch]);
		while (iStitch < GroupEndStitch)
			clpradj(StitchBuffer[iStitch++]);
		if (StitchBuffer[iStitch].x < ClipRectAdjusted.left)
			ClipRectAdjusted.left = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].x > ClipRectAdjusted.right)
			ClipRectAdjusted.right = StitchBuffer[iStitch].x;
		clipMiddle = (ClipRectAdjusted.right - ClipRectAdjusted.left) / 2 + ClipRectAdjusted.left;
		StitchBuffer[GroupStartStitch].y = StitchBuffer[GroupEndStitch].y = (ClipRectAdjusted.top - ClipRectAdjusted.bottom) / 2 + ClipRectAdjusted.bottom;
		if (StitchBuffer[GroupStartStitch].x < clipMiddle) {

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

void shftflt(fPOINT point) {

	if (point.x <= ZoomRect.left || point.x >= ZoomRect.right || point.y <= ZoomRect.bottom || point.y >= ZoomRect.top)
		shft(point);
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

	unsigned iColor;

	UserFlagMap = 0;
	for (iColor = 0; iColor < 16; iColor++) {

		UserColor[iColor] = DefaultUserColors[iColor];
		CustomColor[iColor] = DefaultCustomColors[iColor];
		CustomBackgroundColor[iColor] = DefaultCustomBackgroundColors[iColor];
		BitmapBackgroundColors[iColor] = DefaultBitmapBackgroundColors[iColor];
	}
	dazdef();
	AppliqueColor = 15;
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

void dumrk(double xCoord, double yCoord) {

	if (rstMap(GMRK))
		drwmrk(StitchWindowDC);
	ZoomMarkPoint.x = xCoord;
	ZoomMarkPoint.y = yCoord;
	setMap(INIT);
	setMap(GMRK);
	drwmrk(StitchWindowDC);
	setMap(WASMRK);
}

void gselrng() {

	unsigned iForm;

	SelectedFormsRange.start = SelectedFormsRange.finish = *SelectedFormList;
	for (iForm = 1; iForm < SelectedFormCount; iForm++) {

		if (SelectedFormList[iForm] < SelectedFormsRange.start)
			SelectedFormsRange.start = SelectedFormList[iForm];
		if (SelectedFormList[iForm] > SelectedFormsRange.finish)
			SelectedFormsRange.finish = SelectedFormList[iForm];
	}
}

double nuang(double yDelta, double xDelta) {

	double angle, relativeAngle;

	angle = atan2(yDelta, xDelta);
	relativeAngle = angle - OriginalAngle;
	if (fabs(relativeAngle) > PI) {

		if (relativeAngle > 0)
			relativeAngle = 2 * PI - relativeAngle;
		else
			relativeAngle += 2 * PI;
	}
	return relativeAngle;
}

void angdif(double angle) {

	if (angle > HighestAngle)
		HighestAngle = angle;
	else {

		if (angle < LowestAngle)
			LowestAngle = angle;
	}
}

void rotmrk() {

	unsigned	iVertex, iStitch, segments, codedFormIndex;
	double		tang;

	if (chkMap(GMRK) && (chkMap(FORMSEL) || chkMap(GRPSEL))) {

		if (chkMap(FORMSEL)) {

			codedFormIndex = ClosestFormToCursor << FRMSHFT;
			fvars(ClosestFormToCursor);
			LowestAngle = HighestAngle = 0;
			OriginalAngle = atan2(CurrentFormVertices[0].y - ZoomMarkPoint.y, CurrentFormVertices[0].x - ZoomMarkPoint.x);
			for (iVertex = 1; iVertex < VertexCount; iVertex++)
				angdif(nuang(CurrentFormVertices[iVertex].y - ZoomMarkPoint.y, CurrentFormVertices[iVertex].x - ZoomMarkPoint.x));
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

				if ((StitchBuffer[iStitch].attribute&FRMSK) == codedFormIndex)
					angdif(nuang(StitchBuffer[iStitch].y - ZoomMarkPoint.y, StitchBuffer[iStitch].x - ZoomMarkPoint.x));
			}
		} else {

			rngadj();
			LowestAngle = HighestAngle = 0;
			OriginalAngle = atan2(StitchBuffer[GroupStartStitch].y - ZoomMarkPoint.y, StitchBuffer[GroupStartStitch].x - ZoomMarkPoint.x);
			for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++)
				angdif(nuang(StitchBuffer[iStitch].y - ZoomMarkPoint.y, StitchBuffer[iStitch].x - ZoomMarkPoint.x));
		}
		tang = HighestAngle - LowestAngle;
		segments = 2 * PI / tang;
		IniFile.rotationAngle = 2 * PI / segments;
		// ToDo - should this be IniFile.rotationAngle?
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Rotation Angle: %.2f\n"
			"Segments: %d\n", IniFile.fillAngle * 180 / PI, segments);
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

	unsigned iForm;

	if (SelectedFormCount) {

		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {

			ClosestFormToCursor = SelectedFormList[iForm];
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

	unsigned form, iFormList;

	gselrng();
	iFormList = 0;
	if (find < SelectedFormsRange.start) {

		for (form = find; form < SelectedFormsRange.finish; form++)
			SelectedFormList[iFormList++] = form;
		goto gotsrng;
	}
	if (find > SelectedFormsRange.finish) {

		for (form = SelectedFormsRange.start; form <= find; form++)
			SelectedFormList[iFormList++] = form;
		goto gotsrng;
	}
	for (form = SelectedFormsRange.start; form <= find; form++)
		SelectedFormList[iFormList++] = form;
gotsrng:;
	SelectedFormCount = iFormList;
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

unsigned duswap(unsigned data) {

#if  __UseASM__
	_asm {

		mov		eax, data
		bswap	eax
	}
#else
	return _byteswap_ulong(data);
#endif
}

void ritcur() {

	ICONINFO		iconInfo;
	HCURSOR			currentCursor;
	POINT			cursorPosition;
	unsigned		iRow, iPixel;
	unsigned		mask, bitmapInverse;
	unsigned		bitMask;
	unsigned		pixelColor;
	unsigned char*	bitmapBits;

	currentCursor = GetCursor();
	GetIconInfo(currentCursor, &iconInfo);
	GetCursorPos(&cursorPosition);
	cursorPosition.x -= (StitchWindowOrigin.x + iconInfo.xHotspot);
	cursorPosition.y -= (StitchWindowOrigin.y + iconInfo.yHotspot);
	bitmapBits = new unsigned char[64];
	// ToDo - replace with GetDIBits
	GetBitmapBits(iconInfo.hbmMask, 256, &bitmapBits);
	if (currentCursor == ArrowCursor) {

		for (iRow = 0; iRow < 32; iRow++) {

			mask = duswap(bitmapBits[iRow]);
			bitmapInverse = duswap(bitmapBits[iRow + 32]);
			bitMask = 0x80000000;
			for (iPixel = 0; iPixel < 32; iPixel++) {

				if (!(bitMask&mask)) {

					if (bitmapInverse&bitMask)
						pixelColor = 0xffffff;
					else
						pixelColor = 0x000000;
					SetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow, pixelColor);
				}
				bitMask >>= 1;
			}
		}
	} else {

		for (iRow = 0; iRow < 32; iRow++) {

			bitmapInverse = duswap(bitmapBits[iRow + 32]);
			bitMask = 0x80000000;
			for (iPixel = 0; iPixel < 32; iPixel++) {

				if (bitMask&bitmapInverse)
					SetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow, GetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow) ^ 0xffffff);
				bitMask >>= 1;
			}
		}
	}
	delete[] bitmapBits;
}

#pragma warning (push)
#pragma warning (disable : 6001)
void delsfrms(unsigned code) {

	unsigned	iForm, formFlagWordCount, iWord;
	unsigned	validFormCount, deletedFormCount, iStitch, validStitchCount;
	unsigned*	formIndices;

	if (code) {
		if (FormIndex) {
			// ToDo - why does this result in a 6001 warning
			formIndices = new unsigned[FormIndex];
			formFlagWordCount = (FormIndex >> 5) + 1;
			// ToDo - use local memory allocation for map of deleted forms instead of MarkedStitchMap
			for (iWord = 0; iWord < formFlagWordCount; iWord++)
				MarkedStitchMap[iWord] = 0;
			for (iForm = 0; iForm < SelectedFormCount; iForm++) {
				ClosestFormToCursor = SelectedFormList[iForm];
				setr(ClosestFormToCursor);
				fvars(ClosestFormToCursor);
				f1del();
			}
			validFormCount = 0; deletedFormCount = 0;
			for (iForm = 0; iForm < FormIndex; iForm++) {

				if (!chkr(iForm)) {

					frmcpy(&FormList[validFormCount], &FormList[iForm]);
					formIndices[iForm] = (iForm - deletedFormCount) << 4;
					validFormCount++;
				} else
					deletedFormCount++;
			}
			FormIndex = validFormCount;
			validStitchCount = 0;
			if (chkMap(DELTO)) {

				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

					if (StitchBuffer[iStitch].attribute&ALTYPMSK) {

						iForm = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
						if (!chkr(iForm)) {

							StitchBuffer[validStitchCount].attribute = StitchBuffer[iStitch].attribute &= NFRMSK;
							StitchBuffer[validStitchCount].attribute |= formIndices[iForm];
							StitchBuffer[validStitchCount].x = StitchBuffer[iStitch].x;
							StitchBuffer[validStitchCount++].y = StitchBuffer[iStitch].y;
						}
					} else {

						StitchBuffer[validStitchCount].attribute = StitchBuffer[iStitch].attribute;
						StitchBuffer[validStitchCount].x = StitchBuffer[iStitch].x;
						StitchBuffer[validStitchCount++].y = StitchBuffer[iStitch].y;
					}
				}
				PCSHeader.stitchCount = validStitchCount;
			} else {

				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

					if (!(StitchBuffer[iStitch].attribute&NOTFRM)) {

						iForm = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
						if (chkr(iForm))
							StitchBuffer[iStitch].attribute &= (NFRMSK&NTYPMSK);
					}
				}
			}
			SelectedFormCount = 0;
			delete[] formIndices;
			rstMap(FORMSEL);
			coltab();
			setMap(RESTCH);
		}
	}
}
#pragma warning (pop)

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

BOOL chkminus(unsigned code) {

	if (code == 189 || code == 109) {

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

	unsigned source, destination;

	if (chkMap(GRPSEL)) {

		savdo();
		rngadj();
		if (!GroupStartStitch)
			GroupStartStitch++;
		makspac(GroupEndStitch + 1, GroupEndStitch - GroupStartStitch);
		source = GroupEndStitch - 1;
		destination = GroupEndStitch + 1;
		while (source >= GroupStartStitch) {
			// ToDo - can this be simplified to StitchBuffer[destination++] = StitchBuffer[Source--]
			StitchBuffer[destination].attribute = StitchBuffer[source].attribute;
			StitchBuffer[destination].x = StitchBuffer[source].x;
			StitchBuffer[destination++].y = StitchBuffer[source--].y;
		}
		coltab();
		setMap(RESTCH);
	} else
		shoseln(IDS_GRPMSG, IDS_RETRAC);
}

void setgrd(COLORREF color) {

	unsigned	iCode;
	GRDCOD		gridCodes[] = {

		ID_GRDHI,HIGRD,
		ID_GRDMED,MEDGRD,
		ID_GRDEF,DEFGRD,
		ID_GRDRED,REDGRD,
		ID_GRDBLU,BLUGRD,
		ID_GRDGRN,GRNGRD,
	};

	for (iCode = 0; iCode < 6; iCode++) {

		if (color == gridCodes[iCode].col)
			CheckMenuItem(MainMenu, gridCodes[iCode].id, MF_CHECKED);
		else
			CheckMenuItem(MainMenu, gridCodes[iCode].id, MF_UNCHECKED);
	}
	GridPen = nuPen(GridPen, 1, color);
	IniFile.gridColor = color;
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

void frmcursel(unsigned cursorType) {

	setu(FRMX);
	if (!cursorType)
		rstu(FRMX);
	frmcurmen();
	setMap(DUMEN);
}

#pragma warning (push)
#pragma warning (disable : 4725)
void stchsnap(unsigned start, unsigned finish) {

#if  __UseASM__
	_asm {
			xor		eax, eax
			mov		al, 12
			mul		start
			mov		ecx, finish
			sub		ecx, start
			je		short stchsnapx
			add		eax, offset StitchBuffer
			fld		IniFile.gridSize
snplup :	fld		dword ptr[eax]
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
	fPOINTATTR *pnt = StitchBuffer + start;

	for (unsigned i = 0; i < finish - start; i++) {
		pnt->x = rintf(pnt->x / IniFile.gridSize) * IniFile.gridSize;
		pnt->y = rintf(pnt->y / IniFile.gridSize) * IniFile.gridSize;

		pnt++;
	}
#endif
}
#pragma warning (pop)

#pragma warning (push)
#pragma warning (disable : 4725)
void frmsnap(fPOINT* start, unsigned count) {

#if  __UseASM__
	_asm {
			mov		eax, start
			mov		ecx, count
			shl		ecx, 1
			je		short frmsnapx
			fld		IniFile.gridSize
snpflup :	fld		dword ptr[eax]
			fdiv	st, st(1)
			frndint
			fmul	st, st(1)
			fstp	dword ptr[eax]
			add		eax, 4
			loop	snpflup
			frmsnapx :
	}
#else
	for (unsigned i = 0; i < count; i++) {
		start->x = rintf(start->x / IniFile.gridSize) * IniFile.gridSize;
		start->y = rintf(start->y / IniFile.gridSize) * IniFile.gridSize;

		start++;
	}
#endif
}
#pragma warning (pop)

void gsnap() {

	unsigned iForm;

	if (chkMap(TXTRED))
	{
		txsnap();
		return;
	}
	if (SelectedFormCount) {

		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {

			ClosestFormToCursor = SelectedFormList[iForm];
			frmsnap(FormList[ClosestFormToCursor].vertices, FormList[ClosestFormToCursor].vertexCount);
			frmout(ClosestFormToCursor);
			refil();
		}
		setMap(RESTCH);
	} else {

		if (chkMap(FORMSEL)) {

			savdo();
			frmsnap(FormList[ClosestFormToCursor].vertices, FormList[ClosestFormToCursor].vertexCount);
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

void ritlock(WIN32_FIND_DATA* fileData, unsigned fileIndex, HWND hwndlg) {

	unsigned			iFile;

	SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_RESETCONTENT, 0, 0);
	SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_RESETCONTENT, 0, 0);
	for (iFile = 0; iFile < fileIndex; iFile++) {

		if (fileData[iFile].dwFileAttributes&FILE_ATTRIBUTE_READONLY)
			SendMessage(GetDlgItem(hwndlg, IDC_LOCKED), LB_ADDSTRING, 0, (long)fileData[iFile].cFileName);
		else
			SendMessage(GetDlgItem(hwndlg, IDC_UNLOCKED), LB_ADDSTRING, 0, (long)fileData[iFile].cFileName);
	}
}

BOOL CALLBACK LockPrc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	FINDINFO	*fileInfo;
	HANDLE		searchResult;
	TCHAR		searchName[_MAX_PATH];
	TCHAR		fileName[_MAX_PATH];
	unsigned	iFile, fileError;
	HWND		lockHandle, unlockHandle;

	switch (umsg) {

	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		SetWindowLongPtr(hwndlg, DWLP_USER, lparam);
		fileInfo = reinterpret_cast<FINDINFO*>(lparam);
		strcpy_s(searchName, DefaultDirectory);
		strcat_s(searchName, "\\*.thr");
		searchResult = FindFirstFile(searchName, &(fileInfo->data[0]));
		if (searchResult == INVALID_HANDLE_VALUE) {

			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "Directory: %s has no .thr files\n", DefaultDirectory);
			shoMsg(MsgBuffer);
			EndDialog(hwndlg, wparam);
			return TRUE;
		}
		fileInfo->count = 1;
		while (FindNextFile(searchResult, &fileInfo->data[fileInfo->count++]));
		fileInfo->count--;
		ritlock(fileInfo->data, fileInfo->count, hwndlg);
		break;

	case WM_COMMAND:

		fileInfo = reinterpret_cast<FINDINFO *>(GetWindowLongPtr(hwndlg, DWLP_USER));

		switch (LOWORD(wparam)) {

		case IDCANCEL:

			EndDialog(hwndlg, wparam);
			return TRUE;

		case IDC_LOCKAL:

			for (iFile = 0; iFile < fileInfo->count; iFile++)
				fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;

		case IDC_UNLOCKAL:

			for (iFile = 0; iFile < fileInfo->count; iFile++)
				fileInfo->data[iFile].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;

		case IDC_LOCK:

			fileError = 0;
			unlockHandle = GetDlgItem(hwndlg, IDC_UNLOCKED);
			for (iFile = 0; iFile < fileInfo->count; iFile++) {

				if (!(fileInfo->data[iFile].dwFileAttributes&FILE_ATTRIBUTE_READONLY)) {

					if (SendMessage(unlockHandle, LB_GETSEL, fileError, 0))
						fileInfo->data[iFile].dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
					fileError++;
				}
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;

		case IDC_UNLOCK:

			fileError = 0;
			lockHandle = GetDlgItem(hwndlg, IDC_LOCKED);
			for (iFile = 0; iFile < fileInfo->count; iFile++) {

				if ((fileInfo->data[iFile].dwFileAttributes&FILE_ATTRIBUTE_READONLY)) {

					if (SendMessage(lockHandle, LB_GETSEL, fileError, 0))
						fileInfo->data[iFile].dwFileAttributes &= 0xffffffff ^ FILE_ATTRIBUTE_READONLY;
					fileError++;
				}
			}
			ritlock(fileInfo->data, fileInfo->count, hwndlg);
			break;

		case IDOK:

			strcpy_s(searchName, DefaultDirectory);
			strcat_s(searchName, "\\");
			fileError = 0;
			for (iFile = 0; iFile < fileInfo->count; iFile++) {

				strcpy_s(fileName, searchName);
				strcat_s(fileName, fileInfo->data[iFile].cFileName);
				if (!SetFileAttributes(fileName, fileInfo->data[iFile].dwFileAttributes))
					fileError++;
			}
			if (fileError) {

				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d files could not be locked or unlocked\n", fileError);
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
	FINDINFO lockInfo;

	lockInfo.count = 0;
	// ToDo - Replace 512 with maximum files in subdirectory
	lockInfo.data = new WIN32_FIND_DATA[512];

	DialogBoxParam(ThrEdInstance, MAKEINTRESOURCE(IDD_DLOCK), ThrEdWindow, (DLGPROC)LockPrc, (LPARAM)&lockInfo);

	delete[] lockInfo.data;

}

unsigned colsum(COLORREF color) {

	unsigned colorSum;
	unsigned iRGB;

	trcols(color);
	colorSum = 0;
	for (iRGB = 0; iRGB < 3; iRGB++) {

		if (chkMap(TraceRGBFlag[iRGB]))
			colorSum += PixelColors[iRGB];
	}
	return colorSum;
}

unsigned icolsum(COLORREF color) {

	unsigned colorSum;
	unsigned iRGB;

	trcols(color);
	colorSum = 0;
	for (iRGB = 0; iRGB < 3; iRGB++) {

		if (chkMap(TraceRGBFlag[iRGB]))
			colorSum += 255 - PixelColors[iRGB];
	}
	return colorSum;
}

COLORREF dwnshft(COLORREF color) {

#if  __UseASM__
	_asm {

		mov		eax, color
		shr		eax, 1
		and eax, 0x3f3f3f
	}
#else
	return (color >> 1) & 0x3f3f3f;
#endif
}

void setrac(unsigned point) {

#if  __UseASM__
	_asm {

		mov		eax, point
		mov		ebx, offset OSequence
		bts[ebx], eax
	}
#else
	_bittestandset((long *)OSequence, point);
#endif
}

BOOL getrac(unsigned point) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, point
		mov		ebx, offset OSequence
		bt[ebx], ecx
		jnc		short getracx
		inc		eax
		getracx :
	}
#else
	return _bittest((long *)OSequence, point);
#endif
}

void shownd(HWND hwnd) {

	ShowWindow(hwnd, SW_SHOW);
}

void hidwnd(HWND hwnd) {

	ShowWindow(hwnd, SW_HIDE);
}

void untrace() {

	unsigned	iColor, iTrace, iButton;

	if (rstMap(WASTRAC)) {

		DeleteObject(TraceBitmap);
		ReleaseDC(ThrEdWindow, TraceDC);
		rstMap(WASEDG);
		for (iColor = 0; iColor < 16; iColor++) {

			shownd(DefaultColorWin[iColor]);
			shownd(UserColorWin[iColor]);
			shownd(ThreadSizeWin[iColor]);
		}
		for (iTrace = 0; iTrace < 3; iTrace++) {

			hidwnd(TraceControlWindow[iTrace]);
			hidwnd(TraceSelectWindow[iTrace]);
			hidwnd(TraceUpWindow[iTrace]);
			hidwnd(TraceDownWindow[iTrace]);
		}
		for (iButton = 0; iButton < 9; iButton++)
			shownd(ButtonWin[iButton]);
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

void clrhbut(unsigned startButton)
{
	unsigned iButton;

	for (iButton = startButton; iButton < 9; iButton++)
		SetWindowText(ButtonWin[iButton], "");
}

void tracwnd() {

	unsigned iColor, iTrace;

	for (iColor = 0; iColor < 16; iColor++) {

		hidwnd(DefaultColorWin[iColor]);
		hidwnd(UserColorWin[iColor]);
		hidwnd(ThreadSizeWin[iColor]);
	}
	for (iTrace = 0; iTrace < 3; iTrace++) {

		shownd(TraceControlWindow[iTrace]);
		shownd(TraceSelectWindow[iTrace]);
		shownd(TraceUpWindow[iTrace]);
		shownd(TraceDownWindow[iTrace]);
	}
	hidwnd(ButtonWin[HBOXSEL]);
	hidwnd(ButtonWin[HUPTO]);
	shownd(TraceStepWin);
	trcstpnum();
	trcratnum();
	clrhbut(4);
}

void trcols(COLORREF color) {

#if  __UseASM__
	_asm {

		mov		eax, color
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
	PixelColors[0] = color & 0xff;
	PixelColors[1] = (color & 0xff00) >> 8;
	PixelColors[2] = (color & 0xff0000) >> 16;
#endif
}

BOOL trcin(COLORREF color) {

	if (color) {

		trcols(color);
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

	BITMAPINFO			info;
	BITMAPINFOHEADER	header;
	unsigned			iPixel;

	memset((BITMAPINFOHEADER*)&header, 0, sizeof(BITMAPINFOHEADER));
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = BitmapWidth;
	header.biHeight = BitmapHeight;
	header.biPlanes = 1;
	header.biBitCount = 32;
	header.biCompression = BI_RGB;
	info.bmiHeader = header;
	TraceBitmap = CreateDIBSection(BitmapDC, &info, DIB_RGB_COLORS, (void**)&TraceBitmapData, 0, 0);
	TraceDC = CreateCompatibleDC(StitchWindowDC);
	if (TraceBitmap && TraceDC) {
		SelectObject(TraceDC, TraceBitmap);
		BitBlt(TraceDC, 0, 0, BitmapWidth, BitmapHeight, BitmapDC, 0, 0, SRCCOPY);
		setMap(WASTRAC);
		TracedPixels = (unsigned*)OSequence;
		TraceDataSize = ((BitmapWidth*BitmapHeight) >> 5) + 1;
		for (iPixel = 0; iPixel < TraceDataSize; iPixel++)
			TracedPixels[iPixel] = 0;
		StretchBlt(StitchWindowMemDC, BitmapDstRect.left, BitmapDstRect.top, BitmapDstRect.right - BitmapDstRect.left, BitmapDstRect.bottom - BitmapDstRect.top,
			BitmapDC, BitmapSrcRect.left, BitmapSrcRect.top, BitmapSrcRect.right - BitmapSrcRect.left, BitmapSrcRect.bottom - BitmapSrcRect.top, SRCCOPY);
	}
}

void trace() {

	unsigned			color, iPixel, iRGB;
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
			color = TraceBitmapData[BitmapPoint.y*BitmapWidth + BitmapPoint.x] ^ 0xffffff;
			if (chkMap(TRCUP)) {

				UpPixelColor = color;
				DownPixelColor = 0xffffff;
			} else {

				DownPixelColor = color;
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

			for (iPixel = 0; iPixel < BitmapWidth*BitmapHeight; iPixel++)
				TraceBitmapData[iPixel] &= traceColorMask;
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
		for (iRGB = 0; iRGB < 3; iRGB++)
			HighColors[iRGB] = PixelColors[iRGB];
		trcols(InvertDownColor);
		for (iRGB = 0; iRGB < 3; iRGB++)
			LowColors[iRGB] = PixelColors[iRGB];
		for (iPixel = 0; iPixel < BitmapWidth*BitmapHeight; iPixel++) {

			if (trcin(TraceBitmapData[iPixel]))
				setrac(iPixel);
			else
				TraceBitmapData[iPixel] = 0;
		}
#endif
		setMap(TRSET);
		setMap(RESTCH);
	} else
		tabmsg(IDS_MAPLOD);
}

void setedg(unsigned point) {

#if  __UseASM__
	_asm {

		mov		eax, point
		mov		ebx, TracedEdges
		bts[ebx], eax
	}
#else
	_bittestandset((long *)TracedEdges, point);
#endif
}

BOOL chkedg(unsigned point) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, point
		mov		ebx, TracedEdges
		bt[ebx], ecx
		jnc		short chkedgx
		inc		eax
		chkedgx :
	}
#else
	return _bittest((long *)TracedEdges, point);
#endif
}

void tracedg() {

	unsigned	iHeight, iWidth, iPixel, flag;
	long		pixelIndex;

	if (!chkMap(WASTRAC))
		trace();
	// Todo - Allocate memory locally for TracedEdges
	TracedEdges = (unsigned*)&OSequence[TraceDataSize];
	for (iPixel = 0; iPixel < TraceDataSize; iPixel++)
		TracedEdges[iPixel] = 0;
	pixelIndex = 0;
	for (iHeight = 0; iHeight < BitmapHeight; iHeight++) {

		flag = 0;
		for (iWidth = 0; iWidth < BitmapWidth; iWidth++) {

			if (getrac(pixelIndex)) {

				if (!flag) {

					setedg(pixelIndex);
					flag = 1;
				}
			} else {

				if (flag) {

					setedg(pixelIndex - 1);
					flag = 0;
				}
			}
			pixelIndex++;
		}
		if (flag)
			setedg(pixelIndex - 1);
	}
	for (iWidth = 0; iWidth < BitmapWidth; iWidth++) {

		pixelIndex = iWidth;
		flag = 0;
		for (iHeight = 0; iHeight < BitmapHeight; iHeight++) {

			if (getrac(pixelIndex)) {

				if (!flag) {

					setedg(pixelIndex);
					flag = 1;
				}
			} else {

				if (flag) {

					setedg(pixelIndex - BitmapWidth);
					flag = 0;
				}
			}
			pixelIndex += BitmapWidth;
		}
		if (flag)
			setedg(pixelIndex - BitmapWidth);
	}
	for (iPixel = 0; iPixel < BitmapWidth*BitmapHeight; iPixel++) {

		if (chkedg(iPixel))
			TraceBitmapData[iPixel] = 0xffffff;
		else
			TraceBitmapData[iPixel] = 0;
	}
	setMap(RESTCH);
	setMap(WASEDG);
}

BOOL trcbit() {
	unsigned pixelIndex;

	pixelIndex = CurrentTracePoint.y*BitmapWidth + CurrentTracePoint.x;
	switch (TraceDirection) {

	case TRCR:

		pixelIndex += (1 - BitmapWidth);
		if (CurrentTracePoint.x == (int)BitmapWidth - 1)
			TraceDirection = TRCU;
		else {

			if (chkedg(pixelIndex)) {

				CurrentTracePoint.x++;
				CurrentTracePoint.y--;
				TraceDirection = TRCD;
			} else {

				pixelIndex += BitmapWidth;
				if (chkedg(pixelIndex))
					CurrentTracePoint.x++;
				else
					TraceDirection = TRCU;
			}
		}
		break;

	case TRCD:

		pixelIndex -= (BitmapWidth + 1);
		if (!CurrentTracePoint.y)
			TraceDirection = TRCR;
		else {

			if (chkedg(pixelIndex)) {

				CurrentTracePoint.x--;
				CurrentTracePoint.y--;
				TraceDirection = TRCL;
			} else {

				pixelIndex++;
				if (chkedg(pixelIndex))
					CurrentTracePoint.y--;
				else
					TraceDirection = TRCR;
			}
		}
		break;

	case TRCL:

		pixelIndex += (BitmapWidth - 1);
		if (!CurrentTracePoint.x)
			TraceDirection = TRCD;
		else {

			if (chkedg(pixelIndex)) {

				CurrentTracePoint.x--;
				CurrentTracePoint.y++;
				TraceDirection = TRCU;
			} else {

				pixelIndex -= BitmapWidth;
				if (chkedg(pixelIndex))
					CurrentTracePoint.x--;
				else
					TraceDirection = TRCD;
			}
		}
		break;

	case TRCU:

		pixelIndex += (1 + BitmapWidth);
		if (CurrentTracePoint.y == (int)BitmapHeight - 1)
			TraceDirection = TRCL;
		else {

			if (chkedg(pixelIndex)) {

				CurrentTracePoint.x++;
				CurrentTracePoint.y++;
				TraceDirection = TRCR;
			} else {

				pixelIndex--;
				if (chkedg(pixelIndex))
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

void dutdif(TRCPNT* point) {

	TraceDiff0.x = point[1].x - point[0].x;
	// ToDo this is likely incorrect
	TraceDiff0.y = point[1].x - point[0].y;
}

void dutrac() {

	unsigned	flag;
	RECT		findRectangle;
	long		point, limit, savedPoint;
	long		edgeDistance, minimumEdgeDistance;
	unsigned	iPoint, iCurrent, iNext;
	double		traceLength;
	double		traceLengthSum;
	float		landscapeOffset;

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
		savedPoint = point = CurrentTracePoint.y*BitmapWidth + CurrentTracePoint.x;
		if (!chkedg(point)) {

			flag = 20;
			point = savedPoint;
			limit = (CurrentTracePoint.y + 1)*BitmapWidth;
			while (point < limit && !chkedg(point))
				point++;
			if (point < limit)
				findRectangle.right = point - CurrentTracePoint.y*BitmapWidth;
			else
				findRectangle.right = BitmapWidth;
			point = savedPoint;
			limit = CurrentTracePoint.y*BitmapWidth;
			while (point > limit && !chkedg(point))
				point--;
			if (point == limit)
				findRectangle.left = 0;
			else
				findRectangle.left = point - limit;
			point = savedPoint;
			while (point > 0 && !chkedg(point))
				point -= BitmapWidth;
			if (point > 0)
				findRectangle.bottom = point / BitmapWidth;
			else
				findRectangle.bottom = 0;
			point = savedPoint;
			limit = BitmapWidth*BitmapHeight;
			while (point < limit && !chkedg(point))
				point += BitmapWidth;
			if (point < limit)
				findRectangle.top = point / BitmapWidth;
			else
				findRectangle.top = BitmapHeight;
			flag = 0;
			minimumEdgeDistance = 0x7fffffff;
			if (findRectangle.left) {

				minimumEdgeDistance = CurrentTracePoint.x - findRectangle.left;
				flag = TRCL;
			}
			if (findRectangle.right < (long)BitmapWidth) {

				edgeDistance = findRectangle.right - CurrentTracePoint.x;
				if (edgeDistance < minimumEdgeDistance) {

					minimumEdgeDistance = edgeDistance;
					flag = TRCR;
				}
			}
			if (findRectangle.bottom) {

				edgeDistance = CurrentTracePoint.y - findRectangle.bottom;
				if (edgeDistance < minimumEdgeDistance) {

					minimumEdgeDistance = edgeDistance;
					flag = TRCD;
				}
			}
			if (findRectangle.top < (long)BitmapHeight) {

				edgeDistance = findRectangle.top - CurrentTracePoint.y;
				if (edgeDistance < minimumEdgeDistance) {

					minimumEdgeDistance = edgeDistance;
					flag = TRCU;
				}
			}
			switch (flag) {

			case TRCU:

				CurrentTracePoint.y = findRectangle.top;
				TraceDirection = TRCR;
				break;

			case TRCR:

				CurrentTracePoint.x = findRectangle.right;
				TraceDirection = TRCD;
				break;

			case TRCD:

				CurrentTracePoint.y = findRectangle.bottom;
				TraceDirection = TRCL;
				break;

			case TRCL:

				CurrentTracePoint.x = findRectangle.left;
				TraceDirection = TRCU;
				break;

			default:
				return;
			}
		}
		InitialDirection = TraceDirection;
		point = CurrentTracePoint.y*BitmapWidth + CurrentTracePoint.x;
		ActivePointIndex = 1;
		// ToDo - Use local memory for tracedPoints
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
		for (iPoint = 1; iPoint < ActivePointIndex; iPoint++) {

			TraceDiff1.x = TraceDiff0.x;
			TraceDiff1.y = TraceDiff0.y;
			dutdif(&tracedPoints[iPoint]);
			if (TraceDiff1.x != TraceDiff0.x || TraceDiff1.y != TraceDiff0.y) {

				DecimatedLine[OutputIndex].x = tracedPoints[iPoint].x;
				DecimatedLine[OutputIndex++].y = tracedPoints[iPoint].y;
			}
		}
		tracedPoints[0].x = DecimatedLine[0].x;
		tracedPoints[0].y = DecimatedLine[0].y;
		iNext = 0;
		ActivePointIndex = 0;
		for (iCurrent = 1; iCurrent < OutputIndex; iCurrent++) {

			traceLength = hypot(DecimatedLine[iCurrent].x - DecimatedLine[iNext].x, DecimatedLine[iCurrent].y - DecimatedLine[iNext].y);
			if (traceLength > IniFile.traceLength) {

				tracedPoints[ActivePointIndex].x = DecimatedLine[iNext].x;
				tracedPoints[ActivePointIndex].y = DecimatedLine[iNext].y;
				iNext = iCurrent;
				ActivePointIndex++;
			}
		}
		for (iCurrent = iNext + 1; iCurrent < OutputIndex; iCurrent++) {

			tracedPoints[ActivePointIndex].x = DecimatedLine[iCurrent].x;
			tracedPoints[ActivePointIndex].y = DecimatedLine[iCurrent].y;
			ActivePointIndex++;
		}
		SelectedForm = &FormList[FormIndex];
		frmclr(SelectedForm);
		CurrentFormVertices = &FormVertices[FormVertexIndex];
		CurrentFormVertices[0].x = tracedPoints[0].x*StitchBmpRatio.x;
		CurrentFormVertices[0].y = tracedPoints[0].y*StitchBmpRatio.y;
		iNext = 0;
		OutputIndex = 0;
		traceLengthSum = 0;
		if (chkMap(LANDSCAP))
			landscapeOffset = UnzoomedRect.y - BitmapSizeinStitches.y;
		else
			landscapeOffset = 0;
		for (iCurrent = 1; iCurrent < ActivePointIndex; iCurrent++) {

			traceLengthSum += hypot(tracedPoints[iCurrent].x - tracedPoints[iCurrent - 1].x, tracedPoints[iCurrent].y - tracedPoints[iCurrent - 1].y);
			traceLength = hypot(tracedPoints[iCurrent].x - tracedPoints[iNext].x, tracedPoints[iCurrent].y - tracedPoints[iNext].y);
			if (traceLengthSum > traceLength*IniFile.traceRatio) {

				CurrentFormVertices[OutputIndex].x = tracedPoints[iCurrent - 1].x*StitchBmpRatio.x;
				CurrentFormVertices[OutputIndex].y = tracedPoints[iCurrent - 1].y*StitchBmpRatio.y + landscapeOffset;
				OutputIndex++;
				iCurrent--;
				iNext = iCurrent;
				traceLengthSum = 0;
			}
		}
		if (FormVertexIndex + OutputIndex > MAXITEMS) {

			tabmsg(IDS_FRMOVR);
			return;
		}
		SelectedForm->vertices = adflt(OutputIndex);
		SelectedForm->vertexCount = OutputIndex;
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
chklup1 :	cmp		ah, bh
			jc		short chklup2
			mov		dl, ah
			mov		ah, bh
			mov		bh, dl
chklup2 :	ror		eax, 16
			ror		ebx, 16
			cmp		al, bl
			jc		short chklup3
			mov		dl, al
			mov		al, bl
			mov		bl, dl
chklup3 :	rol		eax, 16
			rol		ebx, 16
			mov		UpPixelColor, eax
			mov		DownPixelColor, ebx
	}
#else
	// ToDo - check translation
	union {
		COLORREF color;
		struct {
			unsigned char g;
			unsigned char r;
			unsigned char b;
			unsigned char x;
		} components;
	} u, d;

	u.color = UpPixelColor;
	d.color = DownPixelColor;

	unsigned char color;

	if (u.components.r < d.components.r) {
		color = u.components.r;

		u.components.r = d.components.r;
		d.components.r = color;
	}

	if (u.components.g < d.components.g) {
		color = u.components.g;

		u.components.g = d.components.g;
		d.components.g = color;
	}

	if (u.components.b < d.components.b) {
		color = u.components.b;

		u.components.b = d.components.b;
		d.components.b = color;
	}

	UpPixelColor = u.color;
	DownPixelColor = d.color;
#endif
}

void trnumwnd0(int position) {

	TraceNumberInput = CreateWindow(
		"STATIC",
		0,
		SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		position,
		ButtonWidth,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void trnumwnd1(int position) {

	GeneralNumberInputBox = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		position,
		ButtonWidthX3,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void tracpar() {

	unsigned	position;
	double		ratio;
	COLORREF	traceColor;
	COLORREF	tracePosition;

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
			ratio = (float)TraceMsgPoint.y / (ButtonHeight * 15);
			position = ratio * 255;
			traceColor = UpPixelColor&TraceRGB[2 - ColumnColor];
			tracePosition = position << TraceShift[ColumnColor];
			if (tracePosition < traceColor) {

				UpPixelColor &= TraceRGBMask[ColumnColor];
				UpPixelColor |= tracePosition;
				goto tracpar1;
			}
			traceColor = DownPixelColor&TraceRGB[2 - ColumnColor];
			if (tracePosition > traceColor) {

				DownPixelColor &= TraceRGBMask[ColumnColor];
				DownPixelColor |= tracePosition;
				goto tracpar1;
			}
			if (Msg.message == WM_LBUTTONDOWN) {

				UpPixelColor &= TraceRGBMask[ColumnColor];
				UpPixelColor |= position << TraceShift[ColumnColor];
			} else {

				DownPixelColor &= TraceRGBMask[ColumnColor];
				DownPixelColor |= position << TraceShift[ColumnColor];
			}
		tracpar1:
			redraw(TraceControlWindow[ColumnColor]);
			trace();
		} else {

			position = floor(TraceMsgPoint.y / ButtonHeight);
			if (position < 16) {

				toglMap(TraceRGBFlag[ColumnColor]);
				redraw(TraceSelectWindow[ColumnColor]);
				trace();
			} else {

				if (position < 18) {

					setMap(NUMIN);
					setMap(TRNIN0);
					MsgIndex = 0;
					*TraceInputBuffer = 0;
					if (position < 17) {

						trnumwnd0(ButtonHeight * 16);
						setMap(TRNUP);
					} else {

						trnumwnd0(ButtonHeight * 17);
						rstMap(TRNUP);
					}
				} else {

					if (position < 20) {

						setMap(NUMIN);
						setMap(TRNIN1);
						MsgIndex = 0;
						*TraceInputBuffer = 0;
						if (position < 19) {

							trnumwnd1(ButtonHeight * 18);
							setMap(TRNUP);
						} else {

							trnumwnd1(ButtonHeight * 19);
							rstMap(TRNUP);
						}
					} else {

						switch (position) {

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
static inline void difsub(unsigned *source, unsigned shift, unsigned *&destination) {
	*(destination++) = (*source >> (shift & 0x0f)) & 0xff;
}
#endif

void difbits(unsigned shift, unsigned* point) {

#if  __UseASM__
	_asm {

		jmp		short difbts

		difsub :	mov		eax, [esi]
					shr		eax, cl
					and		eax, ebx
					mov[edi], eax
					add		edi, 4
					ret

		difbts :	mov		esi, point
					mov		ecx, shift
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
	// ToDo - check translation
	unsigned* testPoint = point;
	unsigned *destination = TraceAdjacentColors;

	difsub(testPoint, shift, destination);

	testPoint -= BitmapWidth;
	difsub(testPoint, shift, destination);

	testPoint -= 1;
	difsub(testPoint, shift, destination);

	testPoint += 2;
	difsub(testPoint, shift, destination);

	testPoint += BitmapWidth;
	difsub(testPoint, shift, destination);

	testPoint -= 2;
	difsub(testPoint, shift, destination);

	testPoint += BitmapWidth;
	difsub(testPoint, shift, destination);

	testPoint += 1;
	difsub(testPoint, shift, destination);

	testPoint += 1;
	difsub(testPoint, shift, destination);
#endif
}

void blanklin(unsigned lineStart) {

	unsigned iPoint;

	for (iPoint = lineStart; iPoint < lineStart + BitmapWidth; iPoint++)
		DifferenceBitmap[iPoint] = 0;
}

unsigned trsum() {

	unsigned sumAdjacent, iAdjacent;

	sumAdjacent = 0;
	for (iAdjacent = 1; iAdjacent < 9; iAdjacent++)
		sumAdjacent += ((TraceAdjacentColors[iAdjacent] > TraceAdjacentColors[0]) ? (TraceAdjacentColors[iAdjacent] - TraceAdjacentColors[0]) : (TraceAdjacentColors[0] - TraceAdjacentColors[iAdjacent]));
	return sumAdjacent;
}

#pragma warning (push)
#pragma warning (disable : 6001)
void trdif() {

	unsigned	iHeight, iPixel, iRGB, iWidth, adjustedColorSum;
	unsigned	iPoint = 0, colorSum, colorSumMaximum, colorSumMinimum;
	double		ratio;

	if (!*PCSBMPFileName) {

		tabmsg(IDS_MAPLOD);
		return;
	}
	rstMap(TRSET);
	rstMap(HIDMAP);
	untrace();
	// ToDo - why does this result in a 6001 warning
	DifferenceBitmap = new unsigned[BitmapHeight*BitmapWidth];
	colorSumMaximum = 0;
	colorSumMinimum = 0xffffffff;
	if (!chkMap(WASTRAC))
		getrmap();
	for (iRGB = 0; iRGB < 3; iRGB++) {

		blanklin(0);
		for (iHeight = 1; iHeight < (unsigned)BitmapHeight - 1; iHeight++) {

			iPoint = iHeight*BitmapWidth;
			DifferenceBitmap[iPoint++] = 0;
			for (iWidth = 1; iWidth < BitmapWidth - 1; iWidth++) {

				difbits(TraceShift[iRGB], &TraceBitmapData[iPoint]);
				colorSum = DifferenceBitmap[iPoint] = trsum();
				iPoint++;
				if (colorSum > colorSumMaximum)
					colorSumMaximum = colorSum;
				if (colorSum < colorSumMinimum)
					colorSumMinimum = colorSum;
			}
			DifferenceBitmap[iPoint++] = 0;
		}
		blanklin(iPoint);
		ratio = (double)255 / (colorSumMaximum - colorSumMinimum);
		for (iPixel = 0; iPixel < BitmapWidth*BitmapHeight; iPixel++) {

			TraceBitmapData[iPixel] &= TraceRGBMask[iRGB];
			if (DifferenceBitmap[iPixel]) {

				adjustedColorSum = (DifferenceBitmap[iPixel] - colorSumMinimum)*ratio;
				TraceBitmapData[iPixel] |= adjustedColorSum << TraceShift[iRGB];
			}
		}
	}
	BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
	setMap(WASDIF);
	setMap(RESTCH);
	tracwnd();
	delete[] DifferenceBitmap;
}
#pragma warning (pop)

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

void trcnum(unsigned shift, COLORREF color, unsigned iRGB) {

	unsigned	bufferLength;
	unsigned	xPosition;
	TCHAR		buffer[11] = { 0 };

	color >>= shift;
	color &= 0xff;
	_itoa_s(color, buffer, 10);
	bufferLength = strlen(buffer);
	xPosition = NumeralWidth*(3 - bufferLength) + 1;
	SetBkColor(DrawItem->hDC, TraceRGB[iRGB]);
	TextOut(DrawItem->hDC, xPosition, 1, buffer, strlen(buffer));
}

void upnum(unsigned iRGB) {

	trcnum(TraceShift[iRGB], InvertUpColor, iRGB);
}

void dwnum(unsigned iRGB) {

	trcnum(TraceShift[iRGB], InvertDownColor, iRGB);
}

void ritrcol(COLORREF* color, unsigned number) {

	*color &= TraceRGBMask[ColumnColor];
	number &= 0xff;
	*color |= (number << TraceShift[ColumnColor]);
}

void dutrnum0(unsigned color) {

	rstMap(NUMIN);
	rstMap(TRNIN0);
	if (chkMap(TRNUP)) {

		ritrcol(&InvertUpColor, color);
		UpPixelColor = InvertUpColor ^ 0xffffff;
		redraw(TraceUpWindow[ColumnColor]);
	} else {

		ritrcol(&InvertDownColor, color);
		DownPixelColor = InvertDownColor ^ 0xffffff;
		redraw(TraceDownWindow[ColumnColor]);
	}
	redraw(TraceControlWindow[ColumnColor]);
	DestroyWindow(TraceNumberInput);
	setMap(WASTRCOL);
	trace();
}

void dutrnum1() {

	double	traceLength;

	DestroyWindow(GeneralNumberInputBox);
	rstMap(NUMIN);
	rstMap(TRNIN1);
	traceLength = atof(MsgBuffer);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", traceLength);
	if (traceLength > 9)
		traceLength = 9;
	if (chkMap(TRNUP)) {

		IniFile.traceLength = traceLength*PFGRAN;
		trcstpnum();
	} else {

		IniFile.traceRatio = 1 + pow(0.1, traceLength);
		trcratnum();
	}
}

void trcsel() {

	unsigned iPixel, maximumColorComponent, iRGB;

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
		for (iPixel = 0; iPixel < BitmapWidth*BitmapHeight; iPixel++) {

			trcols(TraceBitmapData[iPixel]);
			maximumColorComponent = PixelColors[0];
			iRGB = 2;
			if (PixelColors[1] > maximumColorComponent) {

				maximumColorComponent = PixelColors[1];
				iRGB = 1;
			}
			if (PixelColors[2] > maximumColorComponent)
				iRGB = 0;
			TraceBitmapData[iPixel] &= TraceRGB[iRGB];
		}
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
		setMap(WASDSEL);
		setMap(RESTCH);
		tracwnd();
	} else
		tabmsg(IDS_MAPLOD);
}

void trinit() {

	unsigned	iPixel, iRGB, swapComponent, iData, iLevel;
	unsigned	histogramData[3][256];
	unsigned*	histogram1D;
	unsigned	componentPeakCount[3];
	unsigned	componentPeak[3] = { 0 };
	COLORREF	color;

	if (*PCSBMPFileName) {

		if (!chkMap(TRSET)) {

			setMap(TRCRED);
			setMap(TRCGRN);
			setMap(TRCBLU);
			if (!chkMap(WASTRAC))
				getrmap();
			if (chkMap(MONOMAP)) {

				color = TraceBitmapData[0];
				for (iPixel = 0; iPixel < BitmapWidth*BitmapHeight; iPixel++) {

					if (TraceBitmapData[iPixel] != color)
						break;
				}
				if (iPixel < BitmapWidth*BitmapHeight) {

					trcols(TraceBitmapData[iPixel]);
					HighColors[0] = PixelColors[0];
					HighColors[1] = PixelColors[1];
					HighColors[2] = PixelColors[2];
					trcols(color);
					for (iRGB = 0; iRGB < 3; iRGB++) {

						if (PixelColors[iRGB] > HighColors[iRGB]) {

							swapComponent = PixelColors[iRGB];
							PixelColors[iRGB] = HighColors[iRGB];
							HighColors[iRGB] = swapComponent;
						}
						componentPeak[iRGB] = ((HighColors[iRGB] - PixelColors[iRGB]) >> 1) + PixelColors[iRGB];
					}
				}
			} else {
				// ToDo - does this perform better than a 2D loop?
				histogram1D = (unsigned*)histogramData;
				for (iData = 0; iData < 768; iData++)
					histogram1D[iData] = 0;
				for (iPixel = 0; iPixel < BitmapWidth*BitmapHeight; iPixel++) {

					trcols(TraceBitmapData[iPixel]);
					for (iRGB = 0; iRGB < 3; iRGB++)
						histogramData[iRGB][PixelColors[iRGB]]++;
				}
				for (iRGB = 0; iRGB < 3; iRGB++)
					componentPeakCount[iRGB] = 0;
				for (iLevel = 0; iLevel < 256; iLevel++) {

					for (iRGB = 0; iRGB < 3; iRGB++) {

						if (histogramData[iRGB][iLevel] > componentPeakCount[iRGB]) {

							componentPeakCount[iRGB] = histogramData[iRGB][iLevel];
							componentPeak[iRGB] = iLevel;
						}
					}
				}
			}
			InvertDownColor = 0;
			for (iRGB = 0; iRGB < 3; iRGB++) {

				if (componentPeak[iRGB])
					componentPeak[iRGB]--;
				InvertDownColor |= componentPeak[iRGB] << TraceShift[iRGB];
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

void stch2bit(double xCoord, double yCoord) {

	if (chkMap(LANDSCAP))
		yCoord -= (UnzoomedRect.y - BitmapSizeinStitches.y);
	BitmapPoint.x = BmpStitchRatio.x*xCoord;
	BitmapPoint.y = (BitmapHeight - BmpStitchRatio.y*yCoord);
}

void pxlin(unsigned start, unsigned finish) {

	POINT		line[2];

	stch2bit(CurrentFormVertices[start].x, CurrentFormVertices[start].y);
	line[0].x = BitmapPoint.x;
	line[0].y = BitmapPoint.y;
	stch2bit(CurrentFormVertices[finish].x, CurrentFormVertices[finish].y);
	line[1].x = BitmapPoint.x;
	line[1].y = BitmapPoint.y;
	Polyline(BitmapDC, line, 2);
	Polyline(TraceDC, line, 2);
}

void bfrm() {

	unsigned iVertex;

	if (VertexCount) {

		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			pxlin(iVertex, iVertex + 1);
		if (SelectedForm->type != FRMLINE)
			pxlin(iVertex, 0);
	}
}

void blak() {

	unsigned iForm;

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
		for (iForm = 0; iForm < FormIndex; iForm++) {

			fvars(iForm);
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

void frmpos(float deltaX, float deltaY) {

	unsigned iVertex;

	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {

		SelectedForm->vertices[iVertex].x += deltaX;
		SelectedForm->vertices[iVertex].y += deltaY;
	}
	SelectedForm->rectangle.bottom += deltaY;
	SelectedForm->rectangle.top += deltaY;
	SelectedForm->rectangle.left += deltaX;
	SelectedForm->rectangle.right += deltaX;
}

void nudgfn(float deltaX, float deltaY) {

	unsigned	iForm, iStitch, wordCount;
	POINT		pixel;

	if (chkMap(BIGBOX) || SelectedFormCount || chkMap(FORMSEL) || chkMap(GRPSEL) || chkMap(SELBOX))
		savdo();
	if (chkMap(BIGBOX)) {

		for (iForm = 0; iForm < FormIndex; iForm++) {

			SelectedForm = &FormList[iForm];
			frmpos(deltaX, deltaY);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			StitchBuffer[iStitch].x += deltaX;
			StitchBuffer[iStitch].y += deltaY;
		}
		AllItemsRect.bottom += deltaY;
		AllItemsRect.top += deltaY;
		AllItemsRect.left += deltaX;
		AllItemsRect.right += deltaX;
		stchrct2px(AllItemsRect, &SelectedFormsRect);
		setMap(RESTCH);
		return;
	}
	if (SelectedFormCount) {

		wordCount = (FormIndex >> 5) + 1;
		clRmap(wordCount);
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			setr(SelectedFormList[iForm]);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

			if (chkr((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT)) {

				StitchBuffer[iStitch].x += deltaX;
				StitchBuffer[iStitch].y += deltaY;
			}
		}
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {

			SelectedForm = &FormList[SelectedFormList[iForm]];
			frmpos(deltaX, deltaY);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL)) {

		SelectedForm = &FormList[ClosestFormToCursor];
		frmpos(deltaX, deltaY);
		if (SelectedForm->fillType || SelectedForm->edgeType) {

			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

				if ((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor) {

					StitchBuffer[iStitch].x += deltaX;
					StitchBuffer[iStitch].y += deltaY;
				}
			}
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(GRPSEL)) {

		rngadj();
		for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {

			StitchBuffer[iStitch].x += deltaX;
			StitchBuffer[iStitch].y += deltaY;
		}
		grpAdj();
		setMap(RESTCH);
		return;
	}
	if (chkMap(SELBOX)) {

		StitchBuffer[ClosestPointIndex].x += deltaX;
		StitchBuffer[ClosestPointIndex].y += deltaY;
		setMap(RESTCH);
		return;
	}
	pixel.x = pixel.y = 0;
	if (deltaX) {

		if (deltaX > 0)
			pixel.x = IniFile.nudgePixels;
		else
			pixel.x = -IniFile.nudgePixels;
	}
	if (deltaY) {

		if (deltaY > 0)
			pixel.y = -IniFile.nudgePixels;
		else
			pixel.y = +IniFile.nudgePixels;
	}
	// ToDo - use SendInput instead
	mouse_event(MOUSEEVENTF_MOVE, pixel.x, pixel.y, 0, 0);
}

void pixmsg(unsigned iString, unsigned pixelCount) {

	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[iString], pixelCount);
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

	fPOINT	point;

	if (chkMap(WASMRK)) {

		if (ZoomMarkPoint.x > IniFile.hoopSizeX)
			ZoomMarkPoint.x = IniFile.hoopSizeY / 2;
		if (ZoomMarkPoint.y > IniFile.hoopSizeY)
			ZoomMarkPoint.y = IniFile.hoopSizeY / 2;
		dumrk(ZoomMarkPoint.x, ZoomMarkPoint.y);
		point.x = ZoomMarkPoint.x;
		point.y = ZoomMarkPoint.y;
		shft(point);
		setMap(RESTCH);
	} else
		tabmsg(IDS_MRK);
}

void nuscol(unsigned iColor) {

	UserPen[iColor] = nuPen(UserPen[iColor], 1, UserColor[iColor]);
	UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
	redraw(UserColorWin[iColor]);
}

void movchk() {

	unsigned	iStitch, iForm, color, key, swapColor, switchColors;
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
				switchColors = GetKeyState(VK_CONTROL) & 0X8000;
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

					color = StitchBuffer[iStitch].attribute&COLMSK;
					if (color == VerticalIndex) {

						StitchBuffer[iStitch].attribute &= NCOLMSK;
						StitchBuffer[iStitch].attribute |= DraggedColor;
					} else {

						if (!key&&color == DraggedColor) {

							StitchBuffer[iStitch].attribute &= NCOLMSK;
							StitchBuffer[iStitch].attribute |= VerticalIndex;
						}
					}
				}
				for (iForm = 0; iForm < FormIndex; iForm++) {

					pfrm = &FormList[iForm];
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
				if (!switchColors) {

					swapColor = UserColor[VerticalIndex];
					UserColor[VerticalIndex] = UserColor[DraggedColor];
					if (!key) {

						UserColor[DraggedColor] = swapColor;
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

	unsigned	iStitch, iForm, iColor, nextColor, color;
	FRMHED*		form;

	*MarkedStitchMap = 0;
	if (chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {

		VerticalIndex &= COLMSK;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			setr(StitchBuffer[iStitch].attribute&COLMSK);
		if (*MarkedStitchMap == COLSMSK)
			tabmsg(IDS_COLAL);
		else {

			nextColor = 15;
			while (chkr(nextColor))
				nextColor--;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

				color = StitchBuffer[iStitch].attribute&COLMSK;
				if (color >= VerticalIndex&&color < nextColor) {

					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= color + 1;
				}
			}
			for (iForm = 0; iForm < FormIndex; iForm++) {

				form = &FormList[iForm];
				if (form->fillType) {

					if (form->fillColor >= VerticalIndex&&form->fillColor < nextColor)
						form->fillColor++;
					if (form->fillInfo.feather.color >= VerticalIndex&&form->fillInfo.feather.color < nextColor)
						form->fillInfo.feather.color++;
				}
				if (form->edgeType) {

					if (form->borderColor >= VerticalIndex&&form->borderColor < nextColor)
						form->borderColor++;
				}
			}
			for (iColor = nextColor; iColor > VerticalIndex; iColor--) {

				UserColor[iColor] = UserColor[iColor - 1];
				nuscol(iColor);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

BOOL usedcol() {

	unsigned iStitch;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		if ((StitchBuffer[iStitch].attribute&COLMSK) == VerticalIndex)
			return 1;
	}
	return 0;
}

void delcol() {

	unsigned	iStitch, iForm, iColor, color;
	FRMHED*		form;

	if (chkMsgs(Msg.pt, DefaultColorWin[0], UserColorWin[15])) {

		VerticalIndex &= 0xf;
		if (usedcol())
			tabmsg(IDS_COLU);
		else {

			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

				color = StitchBuffer[iStitch].attribute&COLMSK;
				if (color > VerticalIndex&&color) {

					StitchBuffer[iStitch].attribute &= NCOLMSK;
					StitchBuffer[iStitch].attribute |= color - 1;
				}
			}
			for (iForm = 0; iForm < FormIndex; iForm++) {

				form = &FormList[iForm];
				if (form->fillType) {

					if (form->fillColor > VerticalIndex)
						form->fillColor--;
					if (form->fillInfo.feather.color > VerticalIndex)
						form->fillInfo.feather.color--;
				}
				if (form->edgeType) {

					if (form->borderColor > VerticalIndex)
						form->borderColor--;
				}
			}
			for (iColor = VerticalIndex; iColor < 15; iColor++) {

				UserColor[iColor] = UserColor[iColor + 1];
				nuscol(iColor);
			}
			coltab();
			setMap(RESTCH);
		}
	}
}

void set1knot() {

	unsigned source, destination;

	if (PCSHeader.stitchCount&&chkMap(SELBOX)) {

		savdo();
		if (ClosestPointIndex == (unsigned)PCSHeader.stitchCount - 1) {

			setMap(FILDIR);
			OutputIndex = ClosestPointIndex + 1;
			endknt(ClosestPointIndex);
		} else {

			source = PCSHeader.stitchCount - 1;
			destination = PCSHeader.stitchCount + 4;
			while (source&&source >= ClosestPointIndex)
				mvstch(destination--, source--);
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
		ClosestVertexToCursor = FormList[ClosestFormToCursor].vertexCount - 1;
	}
	setMap(RESTCH);
}

void setpclp() {

	POINT		offset;
	POINT		point;
	unsigned	ind, ine;

	sfCor2px(InterleaveSequence[0], &point);
	FormVerticesAsLine[0].x = point.x;
	FormVerticesAsLine[0].y = point.y;
	sfCor2px(InterleaveSequence[1], &point);
	offset.x = Msg.pt.x - StitchWindowOrigin.x - point.x;
	offset.y = Msg.pt.y - StitchWindowOrigin.y - point.y;
	// ToDo - Why use ine instead of ind
	for (ind = 0; ind < OutputIndex - 2; ind++) {

		ine = ind + 1;
		sfCor2px(InterleaveSequence[ine], &point);
		FormVerticesAsLine[ine].x = point.x + offset.x;
		FormVerticesAsLine[ine].y = point.y + offset.y;
	}
	ind++;
	sfCor2px(InterleaveSequence[ind], &point);
	FormVerticesAsLine[ind].x = point.x;
	FormVerticesAsLine[ind].y = point.y;
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

	POINT		point;
	fPOINT		offset;
	unsigned	iOutput, iNext, count;

	point.x = Msg.pt.x + FormMoveDelta.x;
	point.y = Msg.pt.y + FormMoveDelta.y;
	pxCor2stch(point);
	offset.x = SelectedPoint.x - InterleaveSequence[1].x;
	offset.y = SelectedPoint.y - InterleaveSequence[1].y;
	iNext = nxt(ClosestVertexToCursor);
	count = OutputIndex - 2;
	fltspac(&CurrentFormVertices[iNext], count);
	for (iOutput = 1; iOutput < OutputIndex - 1; iOutput++) {

		CurrentFormVertices[iNext].x = InterleaveSequence[iOutput].x + offset.x;
		CurrentFormVertices[iNext].y = InterleaveSequence[iOutput].y + offset.y;
		iNext++;
	}
	SelectedForm->vertexCount += count;
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

	double			colorBarPosition, ratio, swapCoordinate, swapFactor;
	FRMHED*			forms;
	fPOINT			adjustedPoint;
	fPOINT*			clipData;
	fPOINT			newSize;
	fRECTANGLE		formsRect;
	long			lineLength;
	POINT			point;
	RECT			windowRect;
	SATCON*			guides;
	TCHAR			buffer[20];
	unsigned char*	clipCopyBuffer;
	TCHAR			threadSizeMap[] = { '3','4','6' };
	TXPNT*			textureDestination;
	TXPNT*			textureSource;
	unsigned		byteCount, clipCount, code, currentClip, currentGuide, currentVertex;
	unsigned		dst, formCount, iClip, iColor, iFillType, iForm, iGuide, iHoop;
	unsigned		iLayer, iName, iPreference, iSelectedVertex, iSide, iStitch;
	unsigned		iThreadSize, iVersion, iVertex, iWindow, nextVertex, selectedVertexCount;
	unsigned		stitchAttribute, textureCount, traceColor;
	WPARAM			wParameter;

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
				point.x = Msg.pt.x - StitchWindowOrigin.x;
				point.y = Msg.pt.y - StitchWindowOrigin.y;
				insflin(point);
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
				newSize.x = Msg.pt.x - StitchWindowOrigin.x;
				newSize.y = Msg.pt.y - StitchWindowOrigin.y;
				iSide = (SelectedFormControlVertex + 2) % 4;
				ratio = fabs((double)(newSize.x - StretchBoxLine[iSide].x) / (newSize.y - StretchBoxLine[iSide].y));
				if (iSide & 1) {

					if (ratio < XYratio)
						newSize.x = (StretchBoxLine[iSide].y - newSize.y)*XYratio + StretchBoxLine[iSide].x;
					else
						newSize.y = (StretchBoxLine[iSide].x - newSize.x) / XYratio + StretchBoxLine[iSide].y;
					iSide = nxtcrnr(iSide);
					StretchBoxLine[iSide].y = newSize.y;
					iSide = nxtcrnr(iSide);
					StretchBoxLine[iSide].x = newSize.x;
					StretchBoxLine[iSide].y = newSize.y;
					iSide = nxtcrnr(iSide);
					StretchBoxLine[iSide].x = newSize.x;
				} else {

					if (ratio < XYratio)
						newSize.x = (newSize.y - StretchBoxLine[iSide].y)*XYratio + StretchBoxLine[iSide].x;
					else
						newSize.y = (newSize.x - StretchBoxLine[iSide].x) / XYratio + StretchBoxLine[iSide].y;
					iSide = nxtcrnr(iSide);
					StretchBoxLine[iSide].x = newSize.x;
					iSide = nxtcrnr(iSide);
					StretchBoxLine[iSide].x = newSize.x;
					StretchBoxLine[iSide].y = newSize.y;
					iSide = nxtcrnr(iSide);
					StretchBoxLine[iSide].y = newSize.y;
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
					lineLength = Msg.pt.x - StitchWindowOrigin.x;
				else
					lineLength = Msg.pt.y - StitchWindowOrigin.y;
				dst = (SelectedFormControlVertex + 2) % 4;
				code = nxtcrnr(dst);
				for (iSide = 0; iSide < 4; iSide++) {

					if (iSide != dst && iSide != code) {

						if (SelectedFormControlVertex & 1)
							StretchBoxLine[iSide].x = lineLength;
						else
							StretchBoxLine[iSide].y = lineLength;
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
				adjustedPoint.x = RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x;
				adjustedPoint.y = RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y;
				if (adjustedPoint.x)
					RotationAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
				else {

					if (adjustedPoint.y > 0)
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
			point.x = (Msg.pt.x - FormMoveDelta.x - StitchWindowOrigin.x) - SelectedFormsRect.left;
			point.y = (Msg.pt.y - FormMoveDelta.y - StitchWindowOrigin.y) - SelectedFormsRect.top;
			ratsr();
			FormMoveDelta.x = point.x / HorizontalRatio;
			FormMoveDelta.y = point.y / VerticalRatio;
			if (chkMap(FPSEL)) {

				fvars(ClosestFormToCursor);
				iSelectedVertex = SelectedFormVertices.start;
				for (iVertex = 0; (unsigned)iVertex <= SelectedFormVertices.vertexCount; iVertex++) {

					CurrentFormVertices[iSelectedVertex].x += FormMoveDelta.x;
					CurrentFormVertices[iSelectedVertex].y -= FormMoveDelta.y;
					iSelectedVertex = pdir(iSelectedVertex);
				}
				setpsel();
				frmout(ClosestFormToCursor);
				refil();
				setMap(RESTCH);
			} else {

				if (chkMap(BIGBOX)) {

					savdo();
					for (iForm = 0; iForm < FormIndex; iForm++)
						frmadj(iForm);
					for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

						StitchBuffer[iStitch].x += FormMoveDelta.x;
						StitchBuffer[iStitch].y -= FormMoveDelta.y;
					}
					selal();
				} else {

					savdo();
					for (iForm = 0; iForm < SelectedFormCount; iForm++)
						frmadj(SelectedFormList[iForm]);
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
			adjustedPoint.x = (StitchRangeRect.left + SelectBoxOffset.x) - SelectedPoint.x;
			adjustedPoint.y = (StitchRangeRect.bottom + SelectBoxOffset.y) - SelectedPoint.y;
			for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {

				StitchBuffer[iStitch].x -= adjustedPoint.x;
				StitchBuffer[iStitch].y -= adjustedPoint.y;
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
					for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {

						if (inrng(iStitch)) {

							ClosestPointIndex = iStitch;
							setMap(SELBOX);
							break;
						}
					}
				}
				if (rstMap(NOSEL)) {

					SelectedFormCount = 0;
					rstMap(FORMSEL);
					for (iForm = 0; iForm < FormIndex; iForm++) {

						if (finrng(iForm))
							SelectedFormList[SelectedFormCount++] = iForm;
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

					swapCoordinate = ZoomBoxOrigin.x;
					ZoomBoxOrigin.x = SelectedPoint.x;
					SelectedPoint.x = swapCoordinate;
				}
				if (SelectedPoint.y < ZoomBoxOrigin.y) {

					swapCoordinate = ZoomBoxOrigin.y;
					ZoomBoxOrigin.y = SelectedPoint.y;
					SelectedPoint.y = swapCoordinate;
				}
				newSize.x = SelectedPoint.x - ZoomBoxOrigin.x;
				newSize.y = SelectedPoint.y - ZoomBoxOrigin.y;
				SelectedPoint.x = ZoomBoxOrigin.x + newSize.x / 2;
				SelectedPoint.y = ZoomBoxOrigin.y + newSize.y / 2;
				swapFactor = ZoomFactor;
				if (newSize.x > newSize.y) {

					newSize.y = newSize.x / StitchWindowAspectRatio;
					ZoomFactor = newSize.x / UnzoomedRect.x;
				} else {

					newSize.x = newSize.y*StitchWindowAspectRatio;
					ZoomFactor = newSize.y / UnzoomedRect.x;
				}
				if (ZoomFactor < ZoomMin) {

					ZoomFactor = swapFactor;
					zumin();
					return 1;
				}
				ZoomRect.left = ZoomRect.bottom = 0;
				ZoomRect.right = newSize.x;
				ZoomRect.top = newSize.y;
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
			for (iVersion = 0; iVersion < OLDVER; iVersion++) {

				if (Msg.hwnd == BackupViewer[iVersion]) {

					FileVersionIndex = iVersion;
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
			colorBarPosition = (double)(Msg.pt.y - ColorBarRect.top) / (ColorBarRect.bottom - ColorBarRect.top);
			if (Msg.message == WM_RBUTTONDOWN) {

				if (Msg.wParam&MK_SHIFT && (chkMap(SELBOX) || chkMap(GRPSEL))) {

					unbox();
					GroupStitchIndex = colorBarPosition*PCSHeader.stitchCount;
					setMap(GRPSEL);
					grpAdj();
					nuAct(GroupStitchIndex);
					setMap(RESTCH);
				} else {

					ClosestPointIndex = colorBarPosition*PCSHeader.stitchCount;
					nuAct(ClosestPointIndex);
					movbox();
					if (rstMap(GRPSEL)) {

						rstMap(SCROS);
						rstMap(ECROS);
						setMap(RESTCH);
					}
				}
			} else {

				ClosestPointIndex = colorBarPosition*PCSHeader.stitchCount;
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
					selectedVertexCount = (SelectedFormVertices.finish - SelectedFormVertices.start + VertexCount) % VertexCount;
					if (selectedVertexCount < VertexCount >> 1) {

						SelectedFormVertices.vertexCount = selectedVertexCount;
						setMap(PSELDIR);
					} else {

						SelectedFormVertices.vertexCount = VertexCount - selectedVertexCount;
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

						if (SelectedFormCount) {

							nuslst(ClosestFormToCursor);
							setMap(RESTCH);
							return 1;
						} else {

							if (rstMap(FORMSEL) && TmpFormIndex != ClosestFormToCursor) {

								formCount = 0;
								if (TmpFormIndex > ClosestFormToCursor) {

									formCount = ClosestFormToCursor;
									ClosestFormToCursor = TmpFormIndex;
									TmpFormIndex = formCount;
								}
								for (iForm = TmpFormIndex; iForm <= ClosestFormToCursor; iForm++)
									SelectedFormList[formCount++] = iForm;
								SelectedFormCount = formCount;
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
					code = (FormList[ClosestFormToCursor].attribute&FRMLMSK) >> 1;
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

						code = ClosestPointIndex;
						closPnt1(&ClosestPointIndex);
						if (ClosestPointIndex != code) {

							unbox();
							GroupStitchIndex = ClosestPointIndex;
							ClosestPointIndex = code;
							setMap(GRPSEL);
							grpAdj();
						}
						nuAct(GroupStitchIndex);
						redraw(ColorBar);
						return 1;
					}
					if (chkMap(GRPSEL)) {

						code = ClosestPointIndex;
						closPnt1(&ClosestPointIndex);
						GroupStitchIndex = ClosestPointIndex;
						ClosestPointIndex = code;
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

			for (iHoop = 0; iHoop < HUPS; iHoop++) {

				if (Msg.hwnd == SideWindow[iHoop]) {

					switch (iHoop + 1) {

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

			code = 0;
			if (chkok()) {

				savdo();
				rstMap(DELTO);
				code = 1;
			} else {

				GetWindowRect(DeleteStitchesDialog, &windowRect);
				if (Msg.pt.x >= windowRect.left&&Msg.pt.x <= windowRect.right
					&&Msg.pt.y >= windowRect.top&&Msg.pt.y <= windowRect.bottom) {

					savdo();
					setMap(DELTO);
					code = 1;
				}
			}
			delsfrms(code);
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
			pxrct2stch(SelectedFormsRect, &formsRect);
			px2stch();
			FormMoveDelta.x = SelectedPoint.x - ((formsRect.right - formsRect.left) / 2 + formsRect.left);
			FormMoveDelta.y = SelectedPoint.y - ((formsRect.top - formsRect.bottom) / 2 + formsRect.bottom);
			for (iForm = 0; iForm < ClipFormsCount; iForm++) {

				ClosestFormToCursor = FormIndex + iForm;
				fvars(ClosestFormToCursor);
				for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {

					SelectedForm->vertices[iVertex].x += FormMoveDelta.x;
					SelectedForm->vertices[iVertex].y += FormMoveDelta.y;
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
			for (iPreference = 0; iPreference < PRFLINS; iPreference++) {

				if (Msg.hwnd == ValueWindow[iPreference]) {

					PreferenceIndex = iPreference + 1;
					prfsid(ValueWindow[iPreference]);
					break;
				}
			}
			return 1;
		}
		if (!chkMap(ROTAT) && chkMap(GRPSEL)) {

			if (iselpnt()) {

				for (iSide = 0; iSide < 4; iSide++) {

					StretchBoxLine[iSide].x = FormControlPoints[iSide << 1].x;
					StretchBoxLine[iSide].y = FormControlPoints[iSide << 1].y;
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
				if (StitchCoordinatesPixels.x >= FormControlPoints[0].x&&StitchCoordinatesPixels.x <= FormControlPoints[2].x&&
					StitchCoordinatesPixels.y >= FormControlPoints[0].y&&StitchCoordinatesPixels.y <= FormControlPoints[4].y) {

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

					iName = duth(ThrName);
					ThrName[iName] = 't';
					setMap(IGNAM);
					thrsav();
					ThrName[iName] = 'r';
					if (FileVersionIndex)
						WorkingFileName[iName] = FileVersionIndex + 0x2f;
					setMap(REDOLD);
					nuFil();
					WorkingFileName[iName] = 'r';
					switch (FileVersionIndex) {

					case 3:

						movbak('1', '2');

					case 2:

						movbak('0', '1');

					case 1:

						movbak('r', '0');
					}
					movbak('t', 'r');
					ThrName[iName] = 't';
					DeleteFile(ThrName);
					ThrName[iName] = 'r';
					FileHandle = CreateFile(ThrName, (GENERIC_WRITE | GENERIC_READ), 0, NULL,
						OPEN_EXISTING, 0, NULL);
					if (FileHandle == INVALID_HANDLE_VALUE)
						FileHandle = 0;
					return 1;
				}
				GetWindowRect(CancelButton, &windowRect);
				if (Msg.pt.x >= windowRect.left&&Msg.pt.x <= windowRect.right
					&&Msg.pt.y >= windowRect.top&&Msg.pt.y <= windowRect.bottom) {

					getbak();
					return 1;
				}
			}
		}
		if (chkMap(DELFRM)) {

			code = 0;
			if (chkok()) {

				savdo();
				rstMap(DELTO);
				code = 1;
			} else {

				GetWindowRect(DeleteStitchesDialog, &windowRect);
				if (Msg.pt.x >= windowRect.left&&Msg.pt.x <= windowRect.right
					&&Msg.pt.y >= windowRect.top&&Msg.pt.y <= windowRect.bottom) {

					savdo();
					setMap(DELTO);
					code = 1;
				}
			}
			if (code) {

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
				ThreadSize[ThreadSizeSelected][0] = threadSizeMap[VerticalIndex];
				ThreadSizeIndex[ThreadSizeSelected] = VerticalIndex;
				strncpy_s(buffer, ThreadSize[ThreadSizeSelected], 2);
				buffer[2] = 0;
				SetWindowText(ThreadSizeWin[ThreadSizeSelected], buffer);
				setMap(RESTCH);
				for (iWindow = 0; iWindow < 3; iWindow++)
					DestroyWindow(ChangeThreadSizeWin[iWindow]);
				return 1;
			} else {

				for (iWindow = 0; iWindow < 3; iWindow++)
					DestroyWindow(ChangeThreadSizeWin[iWindow]);
			}
		}
		if (rstMap(POLIMOV)) {

			savdo();
			setfrm();
			return 1;
		}
		if (rstMap(FORMIN)) {

			GetWindowRect(MsgWindow, &windowRect);
			if (Msg.pt.x >= windowRect.left&&Msg.pt.x <= windowRect.right
				&&Msg.pt.y >= windowRect.top&&Msg.pt.y <= windowRect.bottom) {

				iFillType = (Msg.pt.y - windowRect.top - 1) / (ButtonHeight - 4);
				if (rstMap(FENDIN)) {

					if (iFillType == 3)
						rstu(SQRFIL);
					if (iFillType == 4)
						setu(SQRFIL);
				} else
					duform(iFillType);
				switch (iFillType) {

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
			AppliqueColor = VerticalIndex;
			SetWindowText(ValueWindow[PAP], MsgBuffer);
			unsid();
			return 1;
		}
		if (rstMap(SIDACT)) {

			savdo();
			if (FormMenuChoice == LFTHTYP) {

				for (iFillType = 0; iFillType < 6; iFillType++) {

					if (Msg.hwnd == SideWindow[iFillType]) {

						SelectedForm->fillInfo.feather.fillType = iFillType + 1;
						refil();
						refrm();
						break;
					}
				}
				return 1;
			}
			if (FormMenuChoice == LLAYR) {

				for (iLayer = 0; iLayer < 5; iLayer++) {

					if (Msg.hwnd == SideWindow[iLayer])
						break;
				}
				if (iLayer < 5) {

					movlayr(iLayer << 1);
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

						code = SelectedForm->edgeType&NEGUND;
						if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL)
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

						code = SelectedForm->edgeType&NEGUND;
						if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL)
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

						delmfil();
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
						SelectedForm->borderColor |= (AppliqueColor << 4);
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

						code = SelectedForm->edgeType&NEGUND;
						if (code == EDGECLIP || code == EDGEANGSAT || code == EDGEAPPL)
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
					delmfil();
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

				code = SelectedForm->attribute&SBLNT;
				if (code)
					SelectedForm->attribute &= NSBLNT;
				else
					SelectedForm->attribute |= SBLNT;
				refil();
				coltab();
				setMap(RESTCH);
				return 1;
			}
			if (Msg.hwnd == ValueWindow[LBFIN]) {

				code = SelectedForm->attribute&FBLNT;
				if (code)
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
				adjustedPoint.x = RotateBoxToCursorLine[0].x - RotateBoxToCursorLine[1].x;
				adjustedPoint.y = RotateBoxToCursorLine[0].y - RotateBoxToCursorLine[1].y;
				if (hypot(adjustedPoint.x, adjustedPoint.y) < CLOSENUF) {

					px2stch();
					RotationCenter.x = SelectedPoint.x;
					RotationCenter.y = SelectedPoint.y;
					setMap(MOVCNTR);
					unrot();
					ritrot();
				} else {

					if (adjustedPoint.x)
						RotationHandleAngle = -atan2(adjustedPoint.y, adjustedPoint.x);
					else {

						if (adjustedPoint.y > 0)
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
				selRct(&formsRect);
				// ToDo - windowRect should be be formsRect? windowRect is not initialized before use here

				FormControlPoints[0].x = FormControlPoints[6].x = FormControlPoints[7].x = FormControlPoints[8].x = windowRect.left;
				FormControlPoints[1].x = FormControlPoints[5].x = midl(windowRect.right, windowRect.left);
				FormControlPoints[0].y = FormControlPoints[1].y = FormControlPoints[2].y = FormControlPoints[8].y = windowRect.top;
				FormControlPoints[3].y = FormControlPoints[7].y = midl(windowRect.top, windowRect.bottom);
				FormControlPoints[4].y = FormControlPoints[5].y = FormControlPoints[6].y = windowRect.bottom;
				FormControlPoints[2].x = FormControlPoints[3].x = FormControlPoints[4].x = windowRect.right;
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
				if (chkMap(INSRT) && PCSHeader.stitchCount < MAXITEMS) {

					px2stch();
					code = (ActiveColor | USMSK | (ActiveLayer << LAYSHFT) | NOTFRM)&NKNOTMSK;
					if (chkMap(LIN1)) {

						if (chkMap(BAKEND)) {

							xlin1();
							iStitch = PCSHeader.stitchCount;
							StitchBuffer[iStitch].x = SelectedPoint.x;
							StitchBuffer[iStitch].y = SelectedPoint.y;
							StitchBuffer[iStitch].attribute = code;
							duzrat();
							stch2px1(iStitch);
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
							for (iStitch = PCSHeader.stitchCount; iStitch; iStitch--) {

								StitchBuffer[iStitch].attribute = StitchBuffer[iStitch - 1].attribute;
								StitchBuffer[iStitch].x = StitchBuffer[iStitch - 1].x;
								StitchBuffer[iStitch].y = StitchBuffer[iStitch - 1].y;
							}
							StitchBuffer[0].attribute = code;
							StitchBuffer[iStitch].attribute &= (~KNOTMSK);
							StitchBuffer[0].x = SelectedPoint.x;
							StitchBuffer[0].y = SelectedPoint.y;
							stch2px1(iStitch);
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
								code = StitchBuffer[ClosestPointIndex].attribute | USMSK;
						}
						iStitch = PCSHeader.stitchCount;
						do {

							StitchBuffer[iStitch].x = StitchBuffer[iStitch - 1].x;
							StitchBuffer[iStitch].y = StitchBuffer[iStitch - 1].y;
							StitchBuffer[iStitch].attribute = StitchBuffer[iStitch - 1].attribute;
							iStitch--;
						} while (iStitch > ClosestPointIndex);
						PCSHeader.stitchCount++;
						ClosestPointIndex++;
						StitchBuffer[ClosestPointIndex].x = SelectedPoint.x;
						StitchBuffer[ClosestPointIndex].y = SelectedPoint.y;
						StitchBuffer[ClosestPointIndex].attribute = code;
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
					iStitch = PCSHeader.stitchCount - 1;
					if (ClosestPointIndex >= iStitch)
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
				code = ActiveColor;
				ActiveColor = VerticalIndex & 0xf;
				redraw(UserColorWin[code]);
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
								code = ClosestFormToCursor << FRMSHFT;
								for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

									stitchAttribute = StitchBuffer[iStitch].attribute;
									if (stitchAttribute&ALTYPMSK && (stitchAttribute&FRMSK) == code && (stitchAttribute&TYPMSK) != TYPMSK)
									{
										stitchAttribute &= NCOLMSK;
										stitchAttribute |= ActiveColor;
										StitchBuffer[iStitch].attribute = stitchAttribute;
									}
								}
								coltab();
								setMap(RESTCH);
							}
						} else {

							if (chkMap(GRPSEL)) {

								for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {

									StitchBuffer[iStitch].attribute &= 0xfffffff0;
									StitchBuffer[iStitch].attribute |= ActiveColor;
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
				for (iThreadSize = 0; iThreadSize < 3; iThreadSize++)
					ChangeThreadSizeWin[iThreadSize] = nuSiz(iThreadSize);
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

		code = Msg.wParam & 0xffff;
		if (chkMap(TXTRED))
		{
			txtkey(code);
			return 1;
		}
		fvars(ClosestFormToCursor);

#if LANG==GRM
		if (code >= 0x30 && code <= 0x39) {

			if (GetKeyState(VK_SHIFT) & 0X8000) {

				switch (code) {

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

			switch (code) {

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

			if (code == VK_RETURN || code == 0xc0) {

				savdo();
				unfil();
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (rstMap(MOVMSG)) {

			if (code == VK_RETURN || code == 0xc0) {

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

			if (code == VK_RETURN || code == 0xc0) {

				deldir();
				return 1;
			}
		}
		if (rstMap(DELSFRMS)) {

			if (code == 'S' || code == VK_RETURN || code == 0xc0) {

				savdo();
				if (code == 'S')
					setMap(DELTO);
				else
					rstMap(DELTO);
				delsfrms(code);
				coltab();
				setMap(RESTCH);
				unmsg();
				return 1;
			}
		}
		if (rstMap(DELFRM)) {

			if (code == 'S' || code == VK_RETURN || code == 0xc0) {

				savdo();
				if (code == 'S')
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

			switch (code) {

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
			// ToDo - add defines for keycode 'dash' and numpad 'subtract'
			if (code == 189 || code == 109)
			{
				*MsgBuffer = '-';
				MsgIndex = 1;
				MsgBuffer[1] = 0;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;
			}
		}
		if (FormMenuChoice || PreferenceIndex) {

			if (chkminus(code)) {

				MsgIndex = 1;
				SideWindowEntryBuffer[0] = '-';
				SetWindowText(SideMessageWindow, SideWindowEntryBuffer);
				return 1;
			}
			if (dunum(code)) {

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
			switch (code) {

			case 0x6e://numpad period
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
		if (code == 'I') {

			movi();
			LastKeyCode = 'I';
			return 1;
		}
		if (code == 'Q'&&LastKeyCode == 'Q')
			unpat();
		LastKeyCode = code;
		if (chkMap(NUMIN)) {

			if (chkMap(SCLPSPAC) && code == 0xbd && !MsgIndex)
			{
				MsgBuffer[0] = '-';
				MsgBuffer[1] = 0;
				MsgIndex = 1;
				SetWindowText(GeneralNumberInputBox, MsgBuffer);
				return 1;
			}
			if (dunum(code)) {

				if (chkMap(TRNIN0)) {

					TraceInputBuffer[MsgIndex++] = NumericCode;
					TraceInputBuffer[MsgIndex] = 0;
					traceColor = atoi(TraceInputBuffer);
					switch (MsgIndex) {

					case 2:

						if (traceColor > 25)
							dutrnum0(traceColor);
						break;

					case 3:

						if (traceColor > 255)
							traceColor = 255;
						dutrnum0(traceColor);
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
			switch (code) {

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
		if (code == 8 && BufferIndex) {

			StitchEntryBuffer[--BufferIndex] = 0;
			butxt(HNUM, StitchEntryBuffer);
			ClosestPointIndex = atoi(StitchEntryBuffer);
			movbox();
			return 1;
		}
		if (!chkMap(WASTRAC) && dunum(code)) {

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
		switch (code) {

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
						byteCount = sizeof(FORMVERTEXCLIP) + (ClipFormVerticesData->vertexCount + 1) * sizeof(fPOINT);
						clipCopyBuffer = new unsigned char[byteCount];
						MoveMemory(clipCopyBuffer, ClipPointer, byteCount);
						GlobalUnlock(ClipMemory);
						CloseClipboard();
						ClipFormVerticesData = (FORMVERTEXCLIP*)clipCopyBuffer;
						if (chkMap(FRMPSEL)) {

							fvars(ClosestFormToCursor);
							InterleaveSequence[0].x = CurrentFormVertices[ClosestVertexToCursor].x;
							InterleaveSequence[0].y = CurrentFormVertices[ClosestVertexToCursor].y;
							clipData = (fPOINT*)&ClipFormVerticesData[1];
							for (iVertex = 0; iVertex <= ClipFormVerticesData->vertexCount; iVertex++) {

								InterleaveSequence[iVertex + 1].x = clipData[iVertex].x;
								InterleaveSequence[iVertex + 1].y = clipData[iVertex].y;
							}
							nextVertex = nxt(ClosestVertexToCursor);
							iVertex++;
							InterleaveSequence[iVertex].x = CurrentFormVertices[nextVertex].x;
							InterleaveSequence[iVertex].y = CurrentFormVertices[nextVertex].y;
							OutputIndex = iVertex + 1;
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
							SelectedForm->vertexCount = ClipFormVerticesData->vertexCount + 1;
							SelectedForm->vertices = adflt(SelectedForm->vertexCount);
							fvars(ClosestFormToCursor);
							MoveMemory(SelectedForm->vertices, &ClipFormVerticesData[1], sizeof(fPOINT)*SelectedForm->vertexCount);
							setMap(INIT);
							NewFormVertexCount = SelectedForm->vertexCount;
							unfrm();
							setmfrm();
							setMap(SHOFRM);
							dufrm();
						}
						delete[] clipCopyBuffer;
						return 1;
					}
					// ToDo - Add more information to the clipboard so that memory can be allocated 
					ClipFormsHeader = (FORMSCLIP*)ClipPointer;
					if (ClipFormsHeader->clipType == CLP_FRMS) {

						ClipFormsCount = ClipFormsHeader->formCount;
						forms = (FRMHED*)&ClipFormsHeader[1];
						for (iForm = 0; iForm < ClipFormsCount; iForm++) {

							frmcpy(&FormList[FormIndex + iForm], &forms[iForm]);
							FormList[FormIndex + iForm].attribute = (FormList[FormIndex + iForm].attribute&NFRMLMSK) | (ActiveLayer << 1);
						}
						CurrentFormVertices = (fPOINT*)&forms[iForm];
						currentVertex = 0;
						for (iForm = 0; iForm < ClipFormsCount; iForm++) {

							SelectedForm = &FormList[FormIndex + iForm];
							SelectedForm->vertices = adflt(SelectedForm->vertexCount);
							for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {

								SelectedForm->vertices[iVertex].x = CurrentFormVertices[currentVertex].x;
								SelectedForm->vertices[iVertex].y = CurrentFormVertices[currentVertex++].y;
							}
						}
						guides = (SATCON*)&CurrentFormVertices[currentVertex];
						currentGuide = 0;
						for (iForm = 0; iForm < ClipFormsCount; iForm++) {

							SelectedForm = &FormList[FormIndex + iForm];
							if (SelectedForm->type == SAT&&SelectedForm->satinGuideCount) {

								SelectedForm->satinOrAngle.guide = adsatk(SelectedForm->satinGuideCount);
								for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {

									SelectedForm->satinOrAngle.guide[iGuide].start = guides[currentGuide].start;
									SelectedForm->satinOrAngle.guide[iGuide].finish = guides[currentGuide++].finish;
								}
							}
						}
						clipData = (fPOINT*)&guides[currentGuide];
						currentClip = 0;
						for (iForm = 0; iForm < ClipFormsCount; iForm++) {

							SelectedForm = &FormList[FormIndex + iForm];
							if (isclpx(FormIndex + iForm)) {

								SelectedForm->angleOrClipData.clip = adclp(SelectedForm->lengthOrCount.clipCount);
								for (iClip = 0; (unsigned)iClip < SelectedForm->lengthOrCount.clipCount; iClip++) {

									SelectedForm->angleOrClipData.clip[iClip].x = clipData[currentClip].x;
									SelectedForm->angleOrClipData.clip[iClip].y = clipData[currentClip++].y;
								}
							}
							if (iseclpx(FormIndex + iForm)) {

								SelectedForm->borderClipData = adclp(SelectedForm->clipEntries);
								for (iClip = 0; iClip < SelectedForm->clipEntries; iClip++) {

									SelectedForm->borderClipData[iClip].x = clipData[currentClip].x;
									SelectedForm->borderClipData[iClip].y = clipData[currentClip++].y;
								}
							}
						}
						textureSource = (TXPNT*)&clipData[currentClip];
						textureDestination = &TexturePointsBuffer[TextureIndex];
						textureCount = 0;
						for (iForm = 0; iForm < ClipFormsCount; iForm++)
						{
							if (istx(FormIndex + iForm))
							{
								SelectedForm = &FormList[FormIndex + iForm];
								textureCount += SelectedForm->fillInfo.texture.count;
								SelectedForm->fillInfo.texture.index += TextureIndex;
								MoveMemory(textureDestination, textureSource, (SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count - TextureIndex) * sizeof(TXPNT));
							}
						}
						TextureIndex += textureCount;
						GlobalUnlock(ClipMemory);
						SelectedFormsRect.top = SelectedFormsRect.left = 0x7fffffff;
						SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
						ratsr();
						for (OutputIndex = 0; OutputIndex < (unsigned)ClipFormsCount; OutputIndex++) {

							fselrct(OutputIndex + FormIndex);
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
							SelectedForm->vertices = adflt(FormList[FormIndex].vertexCount);
							CurrentFormVertices = (fPOINT*)&ClipFormHeader[1];
							mvflpnt(&SelectedForm->vertices[0], &CurrentFormVertices[0], SelectedForm->vertexCount);
							guides = (SATCON*)&CurrentFormVertices[SelectedForm->vertexCount];
							if (SelectedForm->type == SAT&&SelectedForm->satinGuideCount) {

								SelectedForm->satinOrAngle.guide = adsatk(SelectedForm->satinGuideCount);
								mvsatk(&SelectedForm->satinOrAngle.guide[0], &guides[0], SelectedForm->satinGuideCount);
							}
							clipData = (fPOINT*)&guides[0];
							clipCount = 0;
							if (isclpx(FormIndex)) {

								SelectedForm->angleOrClipData.clip = adclp(SelectedForm->lengthOrCount.clipCount);
								mvflpnt(SelectedForm->angleOrClipData.clip, clipData, SelectedForm->lengthOrCount.clipCount);
								clipCount += SelectedForm->lengthOrCount.clipCount;
							}
							if (iseclpx(FormIndex)) {

								clipData = (fPOINT*)&clipData[clipCount];
								SelectedForm->borderClipData = adclp(SelectedForm->clipEntries);
								mvflpnt(SelectedForm->borderClipData, clipData, SelectedForm->clipEntries);
								clipCount += SelectedForm->clipEntries;
							}
							textureSource = (TXPNT*)&clipData[clipCount];
							if (istx(FormIndex))
							{
								SelectedForm->fillInfo.texture.index = TextureIndex;
								textureDestination = adtx(SelectedForm->fillInfo.texture.count);
								MoveMemory(textureDestination, textureSource, SelectedForm->fillInfo.texture.count * sizeof(TXPNT));
							}
						}
						GlobalUnlock(ClipMemory);
						setMap(INIT);
						NewFormVertexCount = SelectedForm->vertexCount;
						if (SelectedForm->type != FRMLINE)
							NewFormVertexCount++;
						unfrm();
						duzrat();
						setmfrm();
						setMap(SHOFRM);
						dufrm();
					}
					CloseClipboard();
				} else {

					Clip = RegisterClipboardFormat(PcdClipFormat);
					ClipMemory = GetClipboardData(Clip);
					if (ClipMemory) {

						redclp();
						clpbox();
						setMap(CLPSHO);
					}
					CloseClipboard();
				}
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
					iColor = pt2colInd(ClosestPointIndex);
				} else
					iColor = pt2colInd(GroupStitchIndex);
				iColor--;
				GroupStitchIndex = ColorChangeTable[iColor].stitchIndex;
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

						iColor = pt2colInd(ClosestPointIndex);
						if (iColor)
							iColor--;
						ClosestPointIndex = ColorChangeTable[iColor].stitchIndex;
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
					iColor = pt2colInd(ClosestPointIndex);
				} else
					iColor = pt2colInd(GroupStitchIndex);
				GroupStitchIndex = ColorChangeTable[iColor].stitchIndex - 1;
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

						iColor = pt2colInd(ClosestPointIndex);
						ClosestPointIndex = ColorChangeTable[iColor].stitchIndex - 1;
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

						++SelectedFormVertices.vertexCount %= VertexCount;
						SelectedFormVertices.finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % VertexCount;
					} else {

						if (SelectedFormVertices.vertexCount) {

							SelectedFormVertices.vertexCount--;
							SelectedFormVertices.finish = (SelectedFormVertices.start + VertexCount - SelectedFormVertices.vertexCount) % VertexCount;
						} else {

							SelectedFormVertices.vertexCount = 1;
							setMap(PSELDIR);
							SelectedFormVertices.finish = (SelectedFormVertices.start + 1) % VertexCount;
						}
					}
					setpsel();
				} else {

					if (rstMap(FRMPSEL)) {

						unpsel();
						SelectedFormVertices.start = ClosestVertexToCursor;
						SelectedFormVertices.form = ClosestFormToCursor;
						SelectedFormVertices.vertexCount = 1;
						SelectedFormVertices.finish = (SelectedFormVertices.start + 1) % VertexCount;
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

						++SelectedFormVertices.vertexCount %= VertexCount;
						SelectedFormVertices.finish = (SelectedFormVertices.start + VertexCount - SelectedFormVertices.vertexCount) % VertexCount;
					} else {

						if (SelectedFormVertices.vertexCount) {

							SelectedFormVertices.vertexCount--;
							SelectedFormVertices.finish = (SelectedFormVertices.start + VertexCount - SelectedFormVertices.vertexCount) % VertexCount;
						} else {

							SelectedFormVertices.vertexCount = 1;
							SelectedFormVertices.finish = (SelectedFormVertices.start + VertexCount - 1) % VertexCount;
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
		for (iVersion = 0; iVersion < OLDNUM; iVersion++) {

			if (Msg.wParam == LRUMenuId[iVersion]) {

				strcpy_s(WorkingFileName, IniFile.prevNames[iVersion]);
				setMap(REDOLD);
				nuFil();
			}
		}
		wParameter = LOWORD(Msg.wParam);
		if (wParameter > 40000 && wParameter < 40400)
			undat();
		switch (wParameter) {

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

			for (iColor = 0; iColor < 16; iColor++) {

				UserColor[iColor] = DefaultColors[iColor];
				UserColorBrush[iColor] = nuBrush(UserColorBrush[iColor], UserColor[iColor]);
				UserPen[iColor] = nuPen(UserPen[iColor], 1, UserColor[iColor]);
				redraw(UserColorWin[iColor]);
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
int txtWid(TCHAR* string) {

	GetTextExtentPoint32(ThredDC, string, strlen(string), &TextSize);
	return TextSize.cx;
}

void makCol() {

	unsigned	iColor;
	TCHAR		buffer[3];

	buffer[1] = '0';
	buffer[2] = 0;
	for (iColor = 0; iColor < 16; iColor++) {

		DefaultColorWin[iColor] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			0,
			ButtonHeight*iColor,
			ButtonWidth,
			ButtonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);

		UserColorWin[iColor] = CreateWindow(
			"STATIC",
			0,
			SS_OWNERDRAW | WS_CHILD | WS_VISIBLE | WS_BORDER,
			ButtonWidth,
			ButtonHeight*iColor,
			ButtonWidth,
			ButtonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);

		buffer[0] = ThreadSize[iColor][0];
		ThreadSizeWin[iColor] = CreateWindow(
			"STATIC",
			buffer,
			SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
			ButtonWidth << 1,
			ButtonHeight*iColor,
			ButtonWidth,
			ButtonHeight,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
			NULL);
	}
}

void ritloc() {

	TCHAR*			environment;
	TCHAR			lockFileName[_MAX_PATH] = { 0 };
	HANDLE			lockFile;
	DWORD			bytesWritten;
	size_t			length;
	errno_t			error;

	error = _dupenv_s(&environment, &length, "COMSPEC");

	if (error) {
		free(environment);
		return;
	} else {
		if (environment) {
			strcpy_s(lockFileName, environment);
			free(environment);
		}
	}
	environment = strrchr(lockFileName, '\\') + 1;
	strcpy_s(environment, sizeof(lockFileName) - (environment - lockFileName), "thredloc.txt");
	lockFile = CreateFile(lockFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (lockFile != INVALID_HANDLE_VALUE) {
		WriteFile(lockFile, (TCHAR*)HomeDirectory, strlen(HomeDirectory) + 1, &bytesWritten, 0);
		CloseHandle(lockFile);
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

	unsigned	pathLength;
	TCHAR*		lastCharacter;

	strcpy_s(HomeDirectory, __argv[0]);
	lastCharacter = strrchr(HomeDirectory, '\\');
	if (lastCharacter)
		lastCharacter++;
	else {

		pathLength = GetCurrentDirectory(_MAX_PATH, HomeDirectory);
		if (pathLength) {
			HomeDirectory[pathLength++] = '\\';
			lastCharacter = &HomeDirectory[pathLength];
		}
	}
	if (lastCharacter) { *lastCharacter = 0; }
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

	unsigned long	bytesRead;
	int				iArgument;
	TCHAR*			balaradFileName;

	if (__argc > 1) {

#if  __UseASM__
		bcpy(WorkingFileName, __argv[1]);
#else
		strcpy_s(WorkingFileName, __argv[1]);
#endif
		if (!strncmp(WorkingFileName, "/F1:", 4)) {

			balaradFileName = &WorkingFileName[4];
			BalaradFile = CreateFile(balaradFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
			CloseHandle(BalaradFile);
			if (BalaradFile != INVALID_HANDLE_VALUE) {

#if  __UseASM__
				bcpy(BalaradName0, balaradFileName);
#else
				strcpy_s(BalaradName0, balaradFileName);
#endif
				if (__argc > 2) {

#if  __UseASM__
					bcpy(WorkingFileName, __argv[2]);
#else
					strcpy_s(WorkingFileName, __argv[2]);
#endif
					if (!strncmp(WorkingFileName, "/F2:", 4)) {

						balaradFileName = &WorkingFileName[4];
						BalaradFile = CreateFile(balaradFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
						if (BalaradFile != INVALID_HANDLE_VALUE) {

#if  __UseASM__
							bcpy(BalaradName1, balaradFileName);
#else
							strcpy_s(BalaradName1, balaradFileName);
#endif
							ReadFile(BalaradFile, (void*)&BalaradName2, (_MAX_PATH + 1), &bytesRead, 0);
							strcat_s(BalaradName2, "");
							if (bytesRead)
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

			for (iArgument = 2; iArgument < __argc; iArgument++) {

				strcat_s(WorkingFileName, " ");
				strcat_s(WorkingFileName, __argv[iArgument]);
			}
			setMap(REDOLD);
			nuFil();
		}
	}
}

void redini() {

	unsigned		iUnDo, iVersion, iColor, adjustedWidth;
	unsigned long	bytesRead;
	HDC				deviceContext;

	for (iUnDo = 0; iUnDo < 16; iUnDo++)
		UndoBuffer[iUnDo] = 0;
	for (iVersion = 0; iVersion < OLDNUM; iVersion++)
		IniFile.prevNames[iVersion][0] = 0;
	duhom();
	strcpy_s(IniFileName, HomeDirectory);
	strcat_s(IniFileName, "thred.ini");
	IniFileHandle = CreateFile(IniFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, 0, NULL);
	if (IniFileHandle == INVALID_HANDLE_VALUE)
		defpref();
	else {

		ReadFile(IniFileHandle, &IniFile, sizeof(IniFile), &bytesRead, 0);
		if (bytesRead < 2061)
			IniFile.formBoxSizePixels = DEFBPIX;
		strcpy_s(DefaultDirectory, IniFile.defaultDirectory);
		strcpy_s(DefaultBMPDirectory, IniFile.defaultDirectory);
		for (iColor = 0; iColor < 16; iColor++) {

			UserColor[iColor] = IniFile.stitchColors[iColor];
			CustomColor[iColor] = IniFile.stitchPreferredColors[iColor];
			CustomBackgroundColor[iColor] = IniFile.backgroundPreferredColors[iColor];
			BitmapBackgroundColors[iColor] = IniFile.bitmapBackgroundColors[iColor];
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
			AppliqueColor = IniFile.underlayColor & 0xf;
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
	deviceContext = GetDC(0);
	ScreenSizePixels.cx = GetDeviceCaps(deviceContext, HORZRES);
	ScreenSizePixels.cy = GetDeviceCaps(deviceContext, VERTRES);
	if (IniFile.initialWindowCoords.left < 0)
		IniFile.initialWindowCoords.left = 0;
	if (IniFile.initialWindowCoords.top < 0)
		IniFile.initialWindowCoords.top = 0;
	adjustedWidth = ScreenSizePixels.cx - 30;
	if (IniFile.initialWindowCoords.right > (int)adjustedWidth)
		IniFile.initialWindowCoords.right = adjustedWidth;
	if (IniFile.initialWindowCoords.bottom > ScreenSizePixels.cy)
		IniFile.initialWindowCoords.bottom = ScreenSizePixels.cy;
}

void trcsub(HWND* window, unsigned xCoordinate, unsigned yCoordinate, unsigned buttonHeight) {

	*window = CreateWindow(
		"STATIC",
		"",
		SS_OWNERDRAW | WS_CHILD | WS_BORDER,
		xCoordinate,
		yCoordinate,
		ButtonWidth,
		buttonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void chkirct() {

	POINT screenLimits;

	screenLimits.x = ScreenSizePixels.cx - 1;
	screenLimits.y = ScreenSizePixels.cy - 1;
	if (IniFile.initialWindowCoords.left > screenLimits.x)
		IniFile.initialWindowCoords.left = screenLimits.x;
	if (IniFile.initialWindowCoords.right > screenLimits.x)
		IniFile.initialWindowCoords.right = screenLimits.x;
	if (IniFile.initialWindowCoords.left < 0)
		IniFile.initialWindowCoords.left = 0;
	if (IniFile.initialWindowCoords.right < 0)
		IniFile.initialWindowCoords.right = 0;
	if (IniFile.initialWindowCoords.top > screenLimits.y)
		IniFile.initialWindowCoords.top = screenLimits.y;
	if (IniFile.initialWindowCoords.bottom > screenLimits.y)
		IniFile.initialWindowCoords.bottom = screenLimits.y;
	if (IniFile.initialWindowCoords.top < 0)
		IniFile.initialWindowCoords.top = 0;
	if (IniFile.initialWindowCoords.bottom < 0)
		IniFile.initialWindowCoords.bottom = 0;
	if (IniFile.initialWindowCoords.right - IniFile.initialWindowCoords.left < 300) {

		IniFile.initialWindowCoords.left = 0;
		IniFile.initialWindowCoords.right = screenLimits.x >> 1;
	}
	if (IniFile.initialWindowCoords.bottom - IniFile.initialWindowCoords.top < 300) {

		IniFile.initialWindowCoords.top = 0;
		IniFile.initialWindowCoords.bottom = screenLimits.y >> 1;
	}
}

void init() {

	unsigned		iButton, iRGB, iRGBK, iByte, flag, iColor, iOffset, iMenu;
	unsigned long	thwid, screenHalfWidth;
	long			offsetStepSize;
	RECT			tRct;
	RECT			wrct;
	HDC				deviceContext;
	TCHAR*			buttonText;

	TextureIndex = 0;
#if	 __UseASM__
	fsizeof = sizeof(FRMHED) >> 2;
#endif
	LoadMenu(ThrEdInstance, MAKEINTRESOURCE(IDR_MENU1));
	MainMenu = GetMenu(ThrEdWindow);
	deviceContext = GetDC(NULL);
	screenHalfWidth = GetDeviceCaps(deviceContext, HORZRES);
	screenHalfWidth >>= 1;
	ReleaseDC(NULL, deviceContext);
	GetWindowRect(ThrEdWindow, &wrct);
	GetMenuItemRect(ThrEdWindow, MainMenu, 0, &tRct);
	//menhi = tRct.bottom - tRct.top;
	wrct.left = tRct.left;
	wrct.right = tRct.right;
	for (iMenu = 0; iMenu <= M_HELP; iMenu++) {

		GetMenuItemRect(ThrEdWindow, MainMenu, iMenu, &tRct);
		wrct.right += (tRct.right - tRct.left);
	}
	wrct.right += 20;
	thwid = wrct.right - wrct.left;
	if (thwid < screenHalfWidth)
		wrct.right = wrct.left += screenHalfWidth;
	FillMenu = GetSubMenu(MainMenu, M_FILL);
	FileMenu = GetSubMenu(MainMenu, M_FILE);
	EditMenu = GetSubMenu(MainMenu, M_EDIT);
	BorderFillMenu = GetSubMenu(FillMenu, MFIL_BORD);
	ViewMenu = GetSubMenu(MainMenu, M_VIEW);
	ViewSetMenu = GetSubMenu(ViewMenu, MVW_SET);
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
	offsetStepSize = txtWid("0");
	for (iOffset = 0; iOffset < NERCNT; iOffset++)
		BoxOffset[iOffset] = offsetStepSize*(iOffset + 1);
	GetClientRect(ThrEdWindow, &ThredWindowRect);
	stchWnd();
	lodstr();
	maxwid(STR_PRF0, STR_PRF27);
	if (!IniFileHandle) {

		//initialize the user color and thread size arrays
		for (iColor = 0; iColor < 16; iColor++)
			UserColor[iColor] = DefaultColors[iColor];
	}
	for (iColor = 0; iColor < 16; iColor++) {

		ThreadSize[iColor][0] = '4';
		ThreadSize[iColor][1] = '0';
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
	for (iButton = 0; iButton < 9; iButton++) {

		switch (iButton) {

		case HBOXSEL:

			flag = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonText = StringTable[STR_BOXSEL];
			break;

		case HUPTO:

			flag = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonText = StringTable[STR_UPOF];
			break;

		case HHID:

			flag = SS_OWNERDRAW | SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER;
			buttonText = StringTable[STR_PIKOL];
			break;

		default:

			buttonText = "";
			flag = SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER;
		}
		ButtonWin[iButton] = CreateWindow(
			"STATIC",
			buttonText,
			flag,
			0,
			ButtonHeight*(16 + iButton),
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

	for (iRGB = 0; iRGB < 3; iRGB++) {

		trcsub(&TraceControlWindow[iRGB], ButtonWidth*iRGB, 0, ButtonHeight * 15);
		trcsub(&TraceSelectWindow[iRGB], ButtonWidth*iRGB, ButtonHeight * 15, ButtonHeight);
		trcsub(&TraceUpWindow[iRGB], ButtonWidth*iRGB, ButtonHeight * 16, ButtonHeight);
		trcsub(&TraceDownWindow[iRGB], ButtonWidth*iRGB, ButtonHeight * 17, ButtonHeight);
		TraceBrush[iRGB] = CreateSolidBrush(TraceRGB[iRGB]);
	}

	nuRct();
	//create pens
	for (iRGBK = 0; iRGBK < 4; iRGBK++)
		BoxPen[iRGBK] = CreatePen(PS_SOLID, 1, BoxColor[iRGBK]);
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
	for (iColor = 0; iColor < 16; iColor++) {

		ThreadSizePixels[iColor] = 1;
		ThreadSizeIndex[iColor] = 1;
		UserPen[iColor] = CreatePen(PS_SOLID, 1, UserColor[iColor]);
	}
	BackgroundBrush = CreateSolidBrush(BackgroundColor);
	//create brushes
	for (iColor = 0; iColor < 16; iColor++) {

		DefaultColorBrush[iColor] = CreateSolidBrush(DefaultColors[iColor]);
		UserColorBrush[iColor] = CreateSolidBrush(UserColor[iColor]);
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
	for (iByte = 0; iByte < RES_SIZE; iByte++)
		ExtendedHeader.res[iByte] = 0;
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

COLORREF defTxt(unsigned iColor) {

#if  __UseASM__
	_asm {

		xor		eax, eax
		mov		ecx, TextColorMap
		mov		edx, iColor
		bt		ecx, edx
		jnc		short defx
		mov		eax, 0xffffff
		defx:
	}
#else
	return _bittest((long *)&TextColorMap, iColor) ? 0xffffff : 0;
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

unsigned setRmp(unsigned bit) {

#if  __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, offset MarkedStitchMap
		mov		ecx, bit
		bts[ebx], ecx
		jc		short setrm
		dec		eax
		setrm :
	}
#else
	return _bittestandset((long *)MarkedStitchMap, bit) ? 0 : 0xffffffff;
#endif
}

void drwLin(unsigned currentStitch, unsigned length, HPEN hPen) {

	unsigned	iOffset, layer;
	fPOINTATTR*	activeStitch = &StitchBuffer[currentStitch];

	if (ActiveLayer)
		LineIndex = 0;
	for (iOffset = 0; iOffset < length; iOffset++) {

		layer = (activeStitch[iOffset].attribute&LAYMSK) >> LAYSHFT;
		if (!ActiveLayer || !layer || (layer == ActiveLayer)) {

			LinePoints[LineIndex].x = (activeStitch[iOffset].x - ZoomRect.left)*ZoomRatio.x;
			LinePoints[LineIndex++].y = StitchWindowClientRect.bottom - (activeStitch[iOffset].y - ZoomRect.bottom)*ZoomRatio.y;
		}
	}
	SelectObject(StitchWindowMemDC, hPen);
	if (LineIndex < 16000)
		Polyline(StitchWindowMemDC, LinePoints, LineIndex);
	else {

		iOffset = 0;
		while (LineIndex) {

			if (LineIndex > 16000) {

				Polyline(StitchWindowMemDC, &LinePoints[iOffset], 16000);
				iOffset += 15999;
				LineIndex -= 15999;
			} else {

				Polyline(StitchWindowMemDC, &LinePoints[iOffset], LineIndex);
				break;
			}
		}
	}
	LineIndex = 1;
	layer = (activeStitch[iOffset].attribute&LAYMSK) >> LAYSHFT;
	if (!ActiveLayer || !layer || layer == ActiveLayer) {

		LinePoints[0].x = (activeStitch[iOffset - 1].x - ZoomRect.left)*ZoomRatio.x;
		LinePoints[0].y = StitchWindowClientRect.bottom - (activeStitch[iOffset - 1].y - ZoomRect.bottom)*ZoomRatio.y;
	}
}

void dumov() {

	fPOINTATTR*	anchorStitch;
	POINT		rotationOutline[8];
	POINT		OffsetFromCenter;

	RotateAngle = atan2(StitchBuffer[MoveAnchor + 1].y - StitchBuffer[MoveAnchor].y, StitchBuffer[MoveAnchor + 1].x - StitchBuffer[MoveAnchor].x);
	anchorStitch = &StitchBuffer[MoveAnchor];
	if (anchorStitch->x >= ZoomRect.left&&anchorStitch->x <= ZoomRect.right
		&&anchorStitch->y >= ZoomRect.bottom&&anchorStitch->y <= ZoomRect.top) {

		sdCor2px(StitchBuffer[MoveAnchor], &RotationCenterPixels);
		rotationOutline[0].x = rotationOutline[6].x = RotationCenterPixels.x;
		rotationOutline[0].y = rotationOutline[6].y = RotationCenterPixels.y;
		OffsetFromCenter.x = RotationCenterPixels.x + 12;
		OffsetFromCenter.y = RotationCenterPixels.y + 2;
		rotpix(OffsetFromCenter, &rotationOutline[1]);
		OffsetFromCenter.y = RotationCenterPixels.y - 2;
		rotpix(OffsetFromCenter, &rotationOutline[5]);
		OffsetFromCenter.y = RotationCenterPixels.y + 6;
		rotpix(OffsetFromCenter, &rotationOutline[2]);
		OffsetFromCenter.y = RotationCenterPixels.y - 6;
		rotpix(OffsetFromCenter, &rotationOutline[4]);
		OffsetFromCenter.x = RotationCenterPixels.x + 20;
		OffsetFromCenter.y = RotationCenterPixels.y;
		rotpix(OffsetFromCenter, &rotationOutline[3]);
		SelectObject(StitchWindowMemDC, FormPen);
		SetROP2(StitchWindowMemDC, R2_XORPEN);
		Polyline(StitchWindowMemDC, rotationOutline, 7);
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

unsigned chkup(unsigned count, unsigned iStitch) {

	if (chkMap(UPTO) && ClosestPointIndex) {

		if (ColorChangeTable[iStitch].stitchIndex < ClosestPointIndex) {

			if (ColorChangeTable[iStitch + 1].stitchIndex < ClosestPointIndex)
				return count;
			else
				return ClosestPointIndex - ColorChangeTable[iStitch].stitchIndex + 1;
		} else
			return 0;
	} else
		return count;
}

BOOL bitar() {

	dRECTANGLE	backingRect;
	dRECTANGLE	differenceRect;
	dPOINT		bitmapStitchRatio;
	dRECTANGLE	ZoomedInRect;

	ZoomedInRect.bottom = UnzoomedRect.y - ZoomRect.bottom;
	ZoomedInRect.top = UnzoomedRect.y - ZoomRect.top;
	ZoomedInRect.left = ZoomRect.left;
	ZoomedInRect.right = ZoomRect.right;
	if (ZoomedInRect.top > BitmapSizeinStitches.y || ZoomedInRect.left > BitmapSizeinStitches.x)
		return 0;
	else {

		BitmapSrcRect.top = ceil(ZoomedInRect.top*BmpStitchRatio.y);
		BitmapSrcRect.left = ceil(ZoomRect.left*BmpStitchRatio.x);
		BitmapSrcRect.right = floor(ZoomRect.right*BmpStitchRatio.x);
		BitmapSrcRect.bottom = floor(ZoomedInRect.bottom*BmpStitchRatio.y);
		if (BitmapSrcRect.right > (long)BitmapWidth) {

			BitmapSrcRect.right = BitmapWidth;
			rstMap(LANDSCAP);
		}
		if (BitmapSrcRect.bottom > (long)BitmapHeight) {

			BitmapSrcRect.bottom = BitmapHeight;
			setMap(LANDSCAP);
		}
		backingRect.top = BitmapSrcRect.top*StitchBmpRatio.y;
		backingRect.left = BitmapSrcRect.left*StitchBmpRatio.x;
		backingRect.right = BitmapSrcRect.right*StitchBmpRatio.x;
		backingRect.bottom = BitmapSrcRect.bottom*StitchBmpRatio.y;
		differenceRect.top = backingRect.top - ZoomedInRect.top;
		differenceRect.left = backingRect.left - ZoomedInRect.left;
		differenceRect.right = ZoomedInRect.right - backingRect.right;
		differenceRect.bottom = ZoomedInRect.bottom - backingRect.bottom;
		bitmapStitchRatio.x = (double)StitchWindowClientRect.right / (ZoomRect.right - ZoomRect.left);
		bitmapStitchRatio.y = (double)StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
		BitmapDstRect.top = differenceRect.top*bitmapStitchRatio.y;
		BitmapDstRect.left = differenceRect.left*bitmapStitchRatio.x;
		BitmapDstRect.right = StitchWindowClientRect.right - differenceRect.right*bitmapStitchRatio.x;
		BitmapDstRect.bottom = StitchWindowClientRect.bottom - differenceRect.bottom*bitmapStitchRatio.y;
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

	POINT		gridLine[2];
	RECT		gridRect;
	unsigned	iGrid;

	if (ZoomFactor < ShowStitchThreshold || !ShowStitchThreshold) {

		SetROP2(StitchWindowMemDC, R2_XORPEN);
		SelectObject(StitchWindowMemDC, GridPen);
		gridRect.left = ceil(ZoomRect.left / IniFile.gridSize);
		gridRect.right = floor(ZoomRect.right / IniFile.gridSize);
		gridRect.bottom = ceil(ZoomRect.bottom / IniFile.gridSize);
		gridRect.top = floor(ZoomRect.top / IniFile.gridSize);
		gridLine[0].x = 0;
		gridLine[1].x = StitchWindowClientRect.right;
		for (iGrid = gridRect.bottom; iGrid <= (unsigned)gridRect.top; iGrid++) {

			gridLine[0].y = gridLine[1].y = StitchWindowClientRect.bottom - (iGrid*IniFile.gridSize - ZoomRect.bottom)*ZoomRatio.y + 0.5;
			Polyline(StitchWindowMemDC, gridLine, 2);
		}
		gridLine[0].y = 0;
		gridLine[1].y = StitchWindowClientRect.bottom;
		for (iGrid = gridRect.left; iGrid <= (unsigned)gridRect.right; iGrid++) {

			gridLine[0].x = gridLine[1].x = (iGrid*IniFile.gridSize - ZoomRect.left)*ZoomRatio.x + 0.5;
			Polyline(StitchWindowMemDC, gridLine, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void rint() {

	unsigned size;

	CellSize.x = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
	CellSize.y = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
	size = StitchWindowClientRect.right*StitchWindowClientRect.bottom;
	if (size > RMAPBITS) {

		CellSize.x *= size / RMAPBITS;
		CellSize.y *= size / RMAPBITS;
	}
}

unsigned setRmap(fPOINTATTR stitchPoint) {

	unsigned bitPoint;

	bitPoint = floor((stitchPoint.x - ZoomRect.left) / CellSize.x)*floor((stitchPoint.y - ZoomRect.bottom) / CellSize.y);
	if (bitPoint < RMAPBITS)
		return setRmp(bitPoint);
	return 0;
}

void drwStch() {

	unsigned	iColor, iStitch, stitchCount, pwid, wascol;
	unsigned	layer;
	double		slope, offset;
	double		dub6;
	fPOINTATTR*	currentStitches;
	POINT		stitchLine[2];
	long		maxYcoord = DrawItem->rcItem.bottom;
	long		xDelta, yDelta, gapToEdge;
	HDC			deviceContext;

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
	stitchCount = 0;
	for (iColor = 0; iColor < ColorChanges; iColor++) {

		LineIndex = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
		if (LineIndex > stitchCount)
			stitchCount = LineIndex;
	}
	LinePoints = new POINT[stitchCount + 2];
	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	duzrat();
	if (*PCSBMPFileName && !chkMap(HIDMAP) && !chkMap(UPTO)) {

		if (chkMap(WASTRAC))
			deviceContext = TraceDC;
		else
			deviceContext = BitmapDC;
		if (bitar())
			StretchBlt(StitchWindowMemDC, BitmapDstRect.left, BitmapDstRect.top, BitmapDstRect.right - BitmapDstRect.left, BitmapDstRect.bottom - BitmapDstRect.top,
				deviceContext, BitmapSrcRect.left, BitmapSrcRect.top, BitmapSrcRect.right - BitmapSrcRect.left, BitmapSrcRect.bottom - BitmapSrcRect.top, SRCCOPY);
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
		for (iColor = 0; iColor < 16; iColor++) {

			if (chkMap(THRDS))
				nuStchSiz(iColor, ThreadWidthPixels[ThreadSizeIndex[iColor]]);
			else {
				if (chkMap(COL)) {

					if (iColor == ActiveColor)
						nuStchSiz(iColor, ThreadWidthPixels[ThreadSizeIndex[iColor]]);
					else
						nuStchSiz(iColor, 1);
				} else
					nuStchSiz(iColor, 1);
			}
		}
		LineIndex = 0; DisplayedColorBitmap = 0;
		if (chkMap(ZUMED)) {

			LineIndex = 0;
			rstMap(LINED);
			rstMap(LININ);
			for (iColor = 0; iColor < ColorChanges; iColor++) {

				if (chkMap(HID)) {

					if (ColorChangeTable[iColor].colorIndex != ActiveColor) {

						stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
						currentStitches = &StitchBuffer[ColorChangeTable[iColor].stitchIndex];
						for (iStitch = 0; iStitch < stitchCount; iStitch++) {

							if (currentStitches[iStitch].x >= ZoomRect.left
								&&currentStitches[iStitch].x <= ZoomRect.right
								&&currentStitches[iStitch].y >= ZoomRect.bottom
								&&currentStitches[iStitch].y <= ZoomRect.top) {

								setCol(ColorChangeTable[iColor].colorIndex);
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
				SelectObject(StitchWindowMemDC, UserPen[ColorChangeTable[iColor].colorIndex]);
				stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
				currentStitches = &StitchBuffer[ColorChangeTable[iColor].stitchIndex];
				stitchCount = chkup(stitchCount, iColor);
				for (iStitch = 0; iStitch < stitchCount; iStitch++) {

					layer = (currentStitches[iStitch].attribute&LAYMSK) >> LAYSHFT;
					if (!ActiveLayer || !layer || (layer == ActiveLayer)) {

						if (currentStitches[iStitch].x >= ZoomRect.left
							&&currentStitches[iStitch].x <= ZoomRect.right
							&&currentStitches[iStitch].y >= ZoomRect.bottom
							&&currentStitches[iStitch].y <= ZoomRect.top) {

							wascol = 1;
							if (setMap(LINED)) {

								if (setMap(LININ)) {

									LinePoints[LineIndex].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[LineIndex++].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;

								} else {

									LinePoints[LineIndex].x = (currentStitches[iStitch - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[LineIndex++].y = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LinePoints[LineIndex].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[LineIndex++].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
								}
							} else {

								if (iStitch == 0 && iColor == 0) {

									LinePoints[0].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[0].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LineIndex = 1;
								} else {

									LinePoints[0].x = (currentStitches[iStitch - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[0].y = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LinePoints[1].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
									LinePoints[1].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
									LineIndex = 2;
								}
								setMap(LININ);
							}
						} else {

							if (rstMap(LININ)) {

								LinePoints[LineIndex].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
								LinePoints[LineIndex++].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
								Polyline(StitchWindowMemDC, LinePoints, LineIndex);
								LineIndex = 0;
							} else {

								if (iStitch) {

									//write an equation for this line
									xDelta = currentStitches[iStitch].x - currentStitches[iStitch - 1].x;
									yDelta = currentStitches[iStitch - 1].y - currentStitches[iStitch].y;
									slope = (double)xDelta / yDelta;
									offset = currentStitches[iStitch].x + slope*currentStitches[iStitch].y;
									//does the line intersect with the top of the screen?
									gapToEdge = offset - slope*ZoomRect.top;
									if (gapToEdge >= ZoomRect.left&&gapToEdge <= ZoomRect.right) {

										stitchLine[0].x = (currentStitches[iStitch - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										stitchLine[0].y = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom)*ZoomRatio.x + 0.5;
										stitchLine[1].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										stitchLine[1].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.x + 0.5;
										Polyline(StitchWindowMemDC, stitchLine, 2);
										goto rotlin;
									}
									//does the line intersect the bottom of the screen?
									gapToEdge = offset - slope*ZoomRect.bottom;
									if (gapToEdge >= ZoomRect.left&&gapToEdge <= ZoomRect.right) {

										stitchLine[0].x = (currentStitches[iStitch - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										stitchLine[0].y = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
										stitchLine[1].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
										stitchLine[1].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
										Polyline(StitchWindowMemDC, stitchLine, 2);
										goto rotlin;
									}
									//does the line intersect the left side of the screen?
									if (slope) {

										gapToEdge = (offset - ZoomRect.left) / slope;
										if (gapToEdge >= ZoomRect.bottom&&gapToEdge <= ZoomRect.top) {

											stitchLine[0].x = (currentStitches[iStitch - 1].x - ZoomRect.left)*ZoomRatio.x + 0.5;
											stitchLine[0].y = maxYcoord - (currentStitches[iStitch - 1].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
											stitchLine[1].x = (currentStitches[iStitch].x - ZoomRect.left)*ZoomRatio.x + 0.5;
											stitchLine[1].y = maxYcoord - (currentStitches[iStitch].y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
											Polyline(StitchWindowMemDC, stitchLine, 2);
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
					setCol(ColorChangeTable[iColor].colorIndex);
			skip:;
			}
		} else {

			pwid = chkMap(HID);
			for (iColor = 0; iColor < ColorChanges; iColor++) {

				setCol(ColorChangeTable[iColor].colorIndex);
				stitchCount = ColorChangeTable[iColor + 1].stitchIndex - ColorChangeTable[iColor].stitchIndex;
				stitchCount = chkup(stitchCount, iColor);
				if (!pwid || (pwid&&ColorChangeTable[iColor].colorIndex == ActiveColor))
					drwLin(ColorChangeTable[iColor].stitchIndex, stitchCount, UserPen[ColorChangeTable[iColor].colorIndex]);
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

			clRmap(MAXITEMS);
			SelectObject(StitchWindowMemDC, LinePen);
			SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
			rint();
			if (chkMap(HID)) {

				for (iColor = 0; iColor < ColorChanges; iColor++) {

					if (ColorChangeTable[iColor].colorIndex == ActiveColor) {

						for (iStitch = ColorChangeTable[iColor].stitchIndex; iStitch < ColorChangeTable[iColor + 1].stitchIndex; iStitch++) {

							if (StitchBuffer[iStitch].x >= ZoomRect.left&&StitchBuffer[iStitch].x <= ZoomRect.right
								&&StitchBuffer[iStitch].y >= ZoomRect.bottom&&StitchBuffer[iStitch].y <= ZoomRect.top
								&&setRmap(StitchBuffer[iStitch]))

								stchbox(iStitch, StitchWindowMemDC);
						}
					}
				}
			} else {

				for (iColor = 0; iColor < PCSHeader.stitchCount; iColor++) {

					if (StitchBuffer[iColor].x >= ZoomRect.left&&StitchBuffer[iColor].x <= ZoomRect.right
						&&StitchBuffer[iColor].y >= ZoomRect.bottom&&StitchBuffer[iColor].y <= ZoomRect.top
						&&setRmap(StitchBuffer[iColor]))

						stchbox(iColor, StitchWindowMemDC);
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

	unsigned	iColorChange;
	RECT		colorBarRect;
	double		barSectionHeight, selectedIndicator;
	POINT		indicatorLine[2];

	colorBarRect.left = DrawItem->rcItem.left;
	colorBarRect.right = DrawItem->rcItem.right;
	colorBarRect.top = 0;
	colorBarRect.bottom = DrawItem->rcItem.bottom;
	for (iColorChange = 0; iColorChange < ColorChanges; iColorChange++) {

		barSectionHeight = (double)ColorChangeTable[iColorChange + 1].stitchIndex / PCSHeader.stitchCount;
		colorBarRect.bottom = barSectionHeight*DrawItem->rcItem.bottom;
		FillRect(DrawItem->hDC, &colorBarRect, UserColorBrush[ColorChangeTable[iColorChange].colorIndex]);
		colorBarRect.top = colorBarRect.bottom;
	}
	if (chkMap(SELBOX) || chkMap(GRPSEL)) {

		selectedIndicator = (double)ClosestPointIndex / PCSHeader.stitchCount;
		indicatorLine[0].y = indicatorLine[1].y = colorBarRect.bottom*selectedIndicator + 0.5;
		indicatorLine[0].x = colorBarRect.left;
		indicatorLine[1].x = colorBarRect.right;
		SelectObject(DrawItem->hDC, CrossPen);
		SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
		Polyline(DrawItem->hDC, indicatorLine, 2);
		if (chkMap(GRPSEL)) {

			selectedIndicator = (double)GroupStitchIndex / PCSHeader.stitchCount;
			indicatorLine[0].y = indicatorLine[1].y = colorBarRect.bottom*selectedIndicator + 0.5;
			indicatorLine[0].x = colorBarRect.left;
			indicatorLine[1].x = colorBarRect.right;
			Polyline(DrawItem->hDC, indicatorLine, 2);
		}
		SetROP2(StitchWindowMemDC, R2_COPYPEN);
	}
}

void ritbak(TCHAR* fileName, DRAWITEMSTRUCT* drawItem) {

	unsigned	iStitch, iForm, iVertexInForm, iVertex, iColor, iLine, bytesToRead;
	HANDLE		thrEdFile;
	POINT		drawingDestinationSize;
	fPOINTATTR*	stitchesToDraw;
	POINT*		lines;
	STRHED		stitchHeader;
	fPOINT		stitchSourceSize;
	double		ratio;
	double		xRatio, yRatio;
	COLORREF	brushColor;
	COLORREF	colors[16];
	HBRUSH		brush;
	HPEN		pen;
	FRMHED*		formList;
	fPOINT*		vertexList;
	STREX		extendedHeader;
	unsigned	fileTypeVersion = 0;
	FRMHEDO*	formListOriginal;

	thrEdFile = CreateFile(fileName, GENERIC_READ, 0, 0,
		OPEN_EXISTING, 0, 0);
	drawingDestinationSize.x = drawItem->rcItem.right - drawItem->rcItem.left;
	drawingDestinationSize.y = drawItem->rcItem.bottom - drawItem->rcItem.top;
	stitchSourceSize.x = stitchSourceSize.y = 1;

	if (thrEdFile != INVALID_HANDLE_VALUE) {

		ReadFile(thrEdFile, (STRHED*)&stitchHeader, sizeof(STRHED), &BytesRead, NULL);
		if (BytesRead == sizeof(STRHED)) {

			if ((stitchHeader.headerType & 0xffffff) == 0x746872) {

				fileTypeVersion = (stitchHeader.headerType & 0xff000000) >> 24;
				switch (fileTypeVersion) {

				case 0:

					if (PCSHeader.hoopType == SMALHUP) {

						stitchSourceSize.x = IniFile.hoopSizeX = SHUPX;
						stitchSourceSize.y = IniFile.hoopSizeY = SHUPY;
					} else {

						stitchSourceSize.x = IniFile.hoopSizeX = LHUPX;
						stitchSourceSize.y = IniFile.hoopSizeY = LHUPY;
					}
					break;

				case 1:
				case 2:

					ReadFile(thrEdFile, (STREX*)&extendedHeader, sizeof(STREX), &BytesRead, NULL);
					if (BytesRead != sizeof(STREX))
						return;
					stitchSourceSize.x = extendedHeader.hoopSizeX;
					stitchSourceSize.y = extendedHeader.hoopSizeY;

					break;

				default:

					return;
				}
			}
			xRatio = (double)drawingDestinationSize.x / stitchSourceSize.x;
			yRatio = (double)drawingDestinationSize.y / stitchSourceSize.y;
			if (xRatio < yRatio)
				ratio = xRatio;
			else
				ratio = yRatio;
			if (stitchHeader.stitchCount) {

				stitchesToDraw = new fPOINTATTR[stitchHeader.stitchCount];
				lines = new POINT[stitchHeader.stitchCount];
				bytesToRead = stitchHeader.stitchCount * sizeof(fPOINTATTR);
				ReadFile(thrEdFile, (fPOINTATTR*)stitchesToDraw, bytesToRead, &BytesRead, 0);
				if (bytesToRead == BytesRead) {

					SetFilePointer(thrEdFile, 16, 0, FILE_CURRENT);
					// ToDo - replace 4 & 64 with SIZEOF
					ReadFile(thrEdFile, (COLORREF*)&brushColor, 4, &BytesRead, 0);
					ReadFile(thrEdFile, (COLORREF*)colors, 64, &BytesRead, 0);
					brush = CreateSolidBrush(brushColor);
					SelectObject(drawItem->hDC, brush);
					FillRect(drawItem->hDC, &drawItem->rcItem, brush);
					iColor = stitchesToDraw[0].attribute & 0xf;
					pen = CreatePen(PS_SOLID, 1, colors[iColor]);
					iLine = 0;
					for (iStitch = 0; iStitch < stitchHeader.stitchCount; iStitch++) {

						if ((stitchesToDraw[iStitch].attribute & 0xf) == iColor) {

							lines[iLine].x = stitchesToDraw[iStitch].x*ratio;
							lines[iLine++].y = drawingDestinationSize.y - stitchesToDraw[iStitch].y*ratio;
						} else {

							pen = nuPen(pen, 1, colors[iColor]);
							SelectObject(drawItem->hDC, pen);
							Polyline(drawItem->hDC, lines, iLine);
							iLine = 0;
							iColor = stitchesToDraw[iStitch].attribute & 0xf;
						}
					}
					if (iLine) {

						pen = nuPen(pen, 1, colors[iColor]);
						SelectObject(drawItem->hDC, pen);
						Polyline(drawItem->hDC, lines, iLine);
					}
					DeleteObject(brush);
					DeleteObject(pen);
					delete[] stitchesToDraw;
					delete[] lines;
				} else {

					delete[] stitchesToDraw;
					delete[] lines;
					CloseHandle(thrEdFile);
					return;
				}
			} else
				SetFilePointer(thrEdFile, 84, 0, FILE_CURRENT);
			if (stitchHeader.formCount) {

				lines = new POINT[MAXFRMLINS];
				SetFilePointer(thrEdFile, 80, 0, FILE_CURRENT);
				formList = new FRMHED[stitchHeader.formCount]();
				vertexList = new fPOINT[stitchHeader.vertexCount];
				if (fileTypeVersion < 2) {
					formListOriginal = new FRMHEDO[stitchHeader.formCount];
					bytesToRead = stitchHeader.formCount * sizeof(FRMHEDO);
					ReadFile(thrEdFile, formListOriginal, bytesToRead, &BytesRead, 0);
					if (BytesRead != bytesToRead)
						goto bakskp;
					for (iForm = 0; iForm < stitchHeader.formCount; iForm++) {

						SelectedForm = &formList[iForm];
						MoveMemory(SelectedForm, &formListOriginal[iForm], sizeof(FRMHEDO));
					}
					delete[] formListOriginal;
				} else {

					bytesToRead = stitchHeader.formCount * sizeof(FRMHED);
					ReadFile(thrEdFile, formList, bytesToRead, &BytesRead, 0);
					if (BytesRead != bytesToRead)
						goto bakskp;
				}
				bytesToRead = stitchHeader.vertexCount * sizeof(fPOINT);
				ReadFile(thrEdFile, (fPOINT*)vertexList, bytesToRead, &BytesRead, 0);
				if (BytesRead != bytesToRead)
					goto bakskp;
				iVertex = 0;
				for (iStitch = 0; iStitch < stitchHeader.formCount; iStitch++) {

					iLine = iVertex;
					for (iVertexInForm = 0; (iVertexInForm < formList[iStitch].vertexCount) && (iVertex < stitchHeader.vertexCount); iVertexInForm++) {

						lines[iVertexInForm].x = vertexList[iVertex].x*ratio;
						lines[iVertexInForm].y = drawingDestinationSize.y - vertexList[iVertex++].y*ratio;
					}
					lines[iVertexInForm].x = vertexList[iLine].x*ratio;
					lines[iVertexInForm].y = drawingDestinationSize.y - vertexList[iLine].y*ratio;
					SelectObject(drawItem->hDC, FormPen);
					SetROP2(drawItem->hDC, R2_XORPEN);
					if (FormList[iStitch].type == FRMLINE)
						Polyline(drawItem->hDC, lines, formList[iStitch].vertexCount);
					else
						Polyline(drawItem->hDC, lines, formList[iStitch].vertexCount + 1);
					SetROP2(StitchWindowMemDC, R2_COPYPEN);
				}
			bakskp:;
				delete[] formList;
				delete[] vertexList;
				delete[] lines;
			}
		}
		CloseHandle(thrEdFile);
	}
}

void durct(unsigned shift, RECT traceControlRect, RECT* traceHighMask, RECT* traceMiddleMask, RECT* traceLowMask) {

	unsigned	lowerColor, upperColor, controlHeight;
	double		ratio;

	lowerColor = (UpPixelColor >> shift) & 0xff;
	upperColor = (DownPixelColor >> shift) & 0xff;
	traceHighMask->left = traceLowMask->left = traceMiddleMask->left = traceControlRect.left;
	traceHighMask->right = traceLowMask->right = traceMiddleMask->right = traceControlRect.right;
	controlHeight = traceControlRect.bottom - traceControlRect.top;
	ratio = (double)lowerColor / 255;
	traceMiddleMask->top = controlHeight*ratio + traceControlRect.top;
	ratio = (double)upperColor / 255;
	traceMiddleMask->bottom = controlHeight*ratio + traceControlRect.top;
	rstMap(DUHI);
	rstMap(DULO);
	if (lowerColor) {

		setMap(DULO);
		traceLowMask->bottom = traceMiddleMask->top;
		traceLowMask->top = 0;
	}
	if (upperColor != 255) {

		setMap(DUHI);
		traceHighMask->top = traceMiddleMask->bottom;
		traceHighMask->bottom = traceControlRect.bottom;
	}
}

void dublk(HDC dc, RECT* traceHighMask, RECT* traceLowMask) {

	if (chkMap(DUHI))
		FillRect(dc, traceHighMask, BlackBrush);
	if (chkMap(DULO))
		FillRect(dc, traceLowMask, BlackBrush);
}

LRESULT CALLBACK WndProc(HWND p_hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	unsigned	position, iRGB, iColor, iThumb, iVersion, lastCharacter;
	unsigned	screenCenterOffset;
	SIZE		maxWindowDimension;
	TCHAR		buffer[10];		//for integer to string conversion
	unsigned	length;			//string length
	SIZE		textSize;		//for measuring text items
	POINT		scrollPoint;	//for scroll bar functions
	POINT		line[2];
	long		adjustedWidth;
	double		tdub;
	TCHAR		fileName[_MAX_PATH];
	RECT		traceHighMaskRect;		//high trace mask rectangle
	RECT		traceMiddleMaskRect;	//middle trace mask rectangle
	RECT		traceLowMaskRect;		//low trace mask rectangle

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

				scrollPoint.x = (ZoomRect.right - ZoomRect.left)*LINSCROL;
				if (!scrollPoint.x)
					scrollPoint.x = 1;
				scrollPoint.y = 0;
				rshft(scrollPoint);
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

				scrollPoint.x = -(ZoomRect.right - ZoomRect.left)*LINSCROL;
				if (!scrollPoint.x)
					scrollPoint.x = -1;
				scrollPoint.y = 0;
				rshft(scrollPoint);
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

					position = (unsigned)HIWORD(wParam);
					MovieTimeStep = MAXDELAY - position;
					setsped();
					SetScrollPos(SpeedScrollBar, SB_CTL, position, TRUE);
				}
			} else {

				if ((HWND)lParam == HorizontalScrollBar) {

					scrollPoint.x = ZoomRect.right - ZoomRect.left;
					ZoomRect.left = (long)HIWORD(wParam);
					ZoomRect.right = ZoomRect.left + scrollPoint.x;
					if (ZoomRect.right > UnzoomedRect.x) {

						ZoomRect.right = UnzoomedRect.x;
						ZoomRect.left = UnzoomedRect.x - scrollPoint.x;
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

			scrollPoint.y = (ZoomRect.top - ZoomRect.bottom)*LINSCROL;
			if (!scrollPoint.y)
				scrollPoint.y = 1;
			scrollPoint.x = 0;
			rshft(scrollPoint);
			return 1;

		case SB_LINEUP:

			scrollPoint.y = -(ZoomRect.top - ZoomRect.bottom)*LINSCROL;
			if (!scrollPoint.y)
				scrollPoint.y = -1;
			scrollPoint.x = 0;
			rshft(scrollPoint);
			return 1;

		case SB_PAGEDOWN:

			pgdwn();
			return 1;

		case SB_PAGEUP:

			pgup();
			return 1;

		case SB_THUMBPOSITION:

			scrollPoint.y = ZoomRect.top - ZoomRect.bottom;
			ZoomRect.top = UnzoomedRect.y - (long)HIWORD(wParam);
			ZoomRect.bottom = ZoomRect.top - scrollPoint.y;
			if (ZoomRect.bottom < 0) {

				ZoomRect.bottom = 0;
				ZoomRect.top = scrollPoint.y;
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
					StitchWindowDC,					// handle to destination DC
					0,								// x-coord of destination upper-left corner
					0,								// y-coord of destination upper-left corner
					StitchWindowClientRect.right,	// width of destination rectangle
					StitchWindowClientRect.bottom,	// height of destination rectangle
					StitchWindowMemDC,				// handle to source DC
					0,								// x-coordinate of source upper-left corner
					0,								// y-coordinate of source upper-left corner
					SRCCOPY							// raster operation code
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

			position = (ButtonWidthX3 - PickColorMsgSize.cx) >> 1;
			if (chkMap(HID)) {

				FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[ActiveColor]);
				SetBkColor(DrawItem->hDC, UserColor[ActiveColor]);
			} else
				FillRect(DrawItem->hDC, &DrawItem->rcItem, (HBRUSH)(COLOR_BTNFACE + 1));
			if (chkMap(TXTRED))
			{
				LoadString(ThrEdInstance, IDS_TXWID, fileName, _MAX_PATH);
				sprintf_s(HelpBuffer, sizeof(HelpBuffer), fileName, TextureScreen.width / PFGRAN);
				TextOut(DrawItem->hDC, position, 1, HelpBuffer, strlen(HelpBuffer));;
			} else
				TextOut(DrawItem->hDC, position, 1, StringTable[STR_PIKOL], strlen(StringTable[STR_PIKOL]));;
			return 1;
		}
		if (chkMap(WASTRAC)) {

			for (iRGB = 0; iRGB < 3; iRGB++) {

				if (DrawItem->hwndItem == TraceUpWindow[iRGB]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[iRGB]);
					upnum(iRGB);
					return 1;
				}
				if (DrawItem->hwndItem == TraceDownWindow[iRGB]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, TraceBrush[iRGB]);
					dwnum(iRGB);
				}
				if (DrawItem->hwndItem == TraceControlWindow[iRGB]) {

					durct(TraceShift[iRGB], DrawItem->rcItem, &traceHighMaskRect, &traceMiddleMaskRect, &traceLowMaskRect);
					FillRect(DrawItem->hDC, &traceMiddleMaskRect, TraceBrush[iRGB]);
					dublk(DrawItem->hDC, &traceHighMaskRect, &traceLowMaskRect);
					return 1;
				}
				if (DrawItem->hwndItem == TraceSelectWindow[iRGB]) {

					TempBrush = BlackBrush;
					strcpy_s(buffer, StringTable[STR_OFF]);
					SetBkColor(DrawItem->hDC, 0);
					SetTextColor(DrawItem->hDC, TraceRGB[iRGB]);
					if (chkMap(TraceRGBFlag[iRGB])) {

						TempBrush = TraceBrush[iRGB];
						strcpy_s(buffer, StringTable[STR_ON]);
						SetTextColor(DrawItem->hDC, 0);
						SetBkColor(DrawItem->hDC, TraceRGB[iRGB]);
					}
					FillRect(DrawItem->hDC, &DrawItem->rcItem, TempBrush);
					TextOut(DrawItem->hDC, 1, 1, buffer, strlen(buffer));
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

			for (iColor = 0; iColor < 16; iColor++) {

				if (DrawItem->hwndItem == DefaultColorWin[iColor]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, DefaultColorBrush[iColor]);
					if (chkCol(iColor)) {

						SetBkColor(DrawItem->hDC, DefaultColors[iColor]);
						SetTextColor(DrawItem->hDC, defTxt(iColor));
						sprintf_s(buffer, sizeof(buffer), "%d", iColor + 1);
						length = strlen(buffer);
						GetTextExtentPoint32(DrawItem->hDC, buffer, length, &textSize);
						TextOut(DrawItem->hDC, (ButtonWidth - textSize.cx) >> 1, 0, buffer, length);
					}
					return 1;
				}
				if (DrawItem->hwndItem == UserColorWin[iColor]) {

					FillRect(DrawItem->hDC, &DrawItem->rcItem, UserColorBrush[iColor]);
					if (iColor == ActiveColor) {

						SelectObject(DrawItem->hDC, CrossPen);
						SetROP2(StitchWindowMemDC, R2_NOTXORPEN);
						line[0].x = line[1].x = DrawItem->rcItem.right >> 1;
						line[0].y = 0;
						line[1].y = DrawItem->rcItem.bottom;
						Polyline(DrawItem->hDC, line, 2);
						line[0].y = line[1].y = DrawItem->rcItem.bottom >> 1;
						line[0].x = 0;
						line[1].x = DrawItem->rcItem.right;
						Polyline(DrawItem->hDC, line, 2);
						SetROP2(StitchWindowMemDC, R2_COPYPEN);
					}
					return 1;
				}
			}
		}
		if (chkMap(BAKSHO) && DrawItem->itemAction == ODA_DRAWENTIRE) {

			if (chkMap(THUMSHO)) {

				for (iThumb = 0; iThumb < 4; iThumb++) {

					if (DrawItem->hwndItem == BackupViewer[iThumb] && iThumb < ThumbnailDisplayCount) {

						ritbak(ThumbnailsSelected[iThumb], DrawItem);
						rthumnam(iThumb);
						return 1;
					}
				}
			} else {

				for (iVersion = 0; iVersion < OLDVER; iVersion++) {

					if (DrawItem->hwndItem == BackupViewer[iVersion]) {

						strcpy_s(fileName, ThrName);
						lastCharacter = duth(fileName);
						fileName[lastCharacter] = (TCHAR)iVersion + 's';
						ritbak(fileName, DrawItem);
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

				screenCenterOffset = 0;
				maxWindowDimension.cx = ScreenSizePixels.cx - 30;
				if ((ThredWindowRect.right - ThredWindowRect.left) > maxWindowDimension.cx) {

					screenCenterOffset = (ScreenSizePixels.cx - maxWindowDimension.cx) >> 1;
					ThredWindowRect.left = screenCenterOffset;
					ThredWindowRect.right = ScreenSizePixels.cx - screenCenterOffset;
				}
				maxWindowDimension.cy = ScreenSizePixels.cy - 30;
				if ((ThredWindowRect.bottom - ThredWindowRect.top) > maxWindowDimension.cy) {

					screenCenterOffset = (ScreenSizePixels.cy - maxWindowDimension.cy) >> 1;
					ThredWindowRect.top = screenCenterOffset;
					ThredWindowRect.bottom = ScreenSizePixels.cy - screenCenterOffset;
				}
				if (screenCenterOffset)
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
			// ToDo - Should this be s.top-s.bottom/z.top-z.bottom
			tdub = (double)StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
			adjustedWidth = StitchWindowClientRect.right / tdub;
			if (adjustedWidth + ZoomRect.left > UnzoomedRect.x) {

				ZoomRect.right = UnzoomedRect.x;
				ZoomRect.left = UnzoomedRect.x - adjustedWidth;
			} else
				ZoomRect.right = adjustedWidth + ZoomRect.left;
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

	unsigned	iForm, iGuide, bakclo;
	SATCON*		guide;
	TCHAR*		buffer;
	FRMHED*		form;

	buffer = MsgBuffer;
	for (iForm = 0; iForm < FormIndex; iForm++) {

		form = &FormList[iForm];
		if (form->type == SAT&&form->satinGuideCount) {

			guide = form->satinOrAngle.guide;
			for (iGuide = 0; iGuide < form->satinGuideCount; iGuide++) {

				if (guide[iGuide].start > form->vertexCount || guide[iGuide].finish > form->vertexCount) {

					bakclo = ClosestFormToCursor;
					ClosestFormToCursor = iForm;
					delsac(iForm);
					ClosestFormToCursor = bakclo;
				}
			}
		}
	}
	// ToDo - Why is this code here?
	if (buffer != MsgBuffer)
		shoMsg(MsgBuffer);
}

#if __UseASM__ == 0
#define BAD_FPU_EX (_EM_OVERFLOW | _EM_ZERODIVIDE | _EM_INVALID)
#define COMMON_FPU_EX (_EM_INEXACT | _EM_UNDERFLOW | _EM_DENORMAL)
#define ALL_FPU_EX (BAD_FPU_EX | COMMON_FPU_EX)
#endif

int	fltex(int code) {

#if  __UseASM__
	short	cw;

	_asm {

		xor		eax, eax
		cmp		code, 0x10
		jne		short fltex1
		mov		cw, 0x27f
		fldcw	cw
		dec		eax
		fltex1 :
	}
#else
	if (code != 0x10)
		return 0;
	unsigned int current_word = 0;
	_controlfp_s(&current_word, ALL_FPU_EX, _MCW_EM);
	_controlfp_s(&current_word, _RC_NEAR, _MCW_RC);

	return -1;
#endif
}

#ifdef ALLOCFAILURE
int handle_program_memory_depletion(size_t)
{
	// ToDo - Make this handle the failure with more user notifiication
	shoMsg("Memory Allocation Failure");
	exit(EXIT_FAILURE);
}
#endif

int APIENTRY WinMain(_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow) {
	UNREFERENCED_PARAMETER(nCmdShow);

	WNDCLASSEX		wc;
	LOGBRUSH		br;

#ifdef ALLOCFAILURE
	_PNH old_handler = _set_new_handler(handle_program_memory_depletion);
	//char* testalloc = new char[(~unsigned int((int)0) / 2) - 1]();
	//testalloc[0] = 1;
#endif
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
#ifdef ALLOCFAILURE
		_set_new_handler(old_handler);
#endif
		return 0;
	}
#ifdef ALLOCFAILURE
	_set_new_handler(old_handler);
#endif
	return -1;
}