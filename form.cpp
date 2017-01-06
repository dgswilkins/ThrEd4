#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include "lang.h"
#include "resource.h"
#include "thred.h"

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)

extern fPOINT*	adclp (unsigned cnt);
extern fPOINT*	adflt (unsigned cnt);
extern SATCON*	adsatk (unsigned cnt);
extern void		alrotmsg ();
extern void		angrct (fRECTANGLE* rct);
extern void		centr ();
extern void		chkcwlk ();
extern unsigned	chkMap (unsigned bPnt);
extern void		chkrng (fPOINT* rsiz);
extern unsigned	chku (unsigned bPnt);
extern void		chkund ();
extern void		chkwlk ();
extern void		clpxadj ();
extern void		clRmap (unsigned len);
extern void		coltab ();
extern void		dasyfrm ();
extern void		delfstchs ();
extern void		delinf ();
extern void		delstchm ();
extern void		deltx ();
extern void		durpoli (unsigned nsids);
extern unsigned	duthrsh (double var);
extern void		duzrat ();
extern void		fdelstch ();
extern BOOL		filmsgs (unsigned cod);
extern void		fritfil ();
extern void		frm1pnt ();
extern void		frmdel ();
extern void		fthrfn ();
extern void		grpAdj ();
extern void		insadj ();
extern fPOINT*	insid ();
extern void		intlv ();
extern BOOL		isclp (unsigned find);
extern BOOL		isclpx (unsigned find);
extern BOOL		isfclp ();
extern BOOL		istx (unsigned find);
extern void		moveStitch (fPOINTATTR* dst, fPOINTATTR* src);
extern void		movStch ();
extern TCHAR*	mvflpnt (fPOINT* dst, fPOINT* src, unsigned cnt);
extern void		mvsatk (SATCON* dst, SATCON* src, unsigned cnt);
extern void		mvstch (unsigned dst, unsigned src);
extern void		mvstchs (unsigned dst, unsigned src, unsigned cnt);
extern void		numWnd ();
extern void		nuRct ();
extern void		pntmsg (unsigned cod);
extern unsigned	px2stch ();
extern void		pxCor2stch (POINT pnt);
extern void		redclp ();
extern void		redraw (HWND dWnd);
extern void		ritfcor (fPOINT* pnt);
extern void		ritmov ();
extern void		ritnum (unsigned cod, unsigned num);
extern void		ritot (unsigned num);
extern void		rngadj ();
extern void		rotang1 (fPOINTATTR rpnt, fPOINT* tpnt);
extern void		rotangf (fPOINT rpnt, fPOINT* tpnt);
extern void		rotflt (fPOINT* pnt);
extern void		rotfn ();
extern void		rstAll ();
extern unsigned	rstMap (unsigned bPnt);
extern void		savdo ();
extern void		sCor2px (dPOINT stpnt, POINT* pxpnt);
extern void		sdmsg ();
extern void		selRct (fRECTANGLE* srct);
extern void		setangf (double tang);
extern void		setfchk ();
extern unsigned	setMap (unsigned bPnt);
extern void		setpsel ();
extern unsigned	setRmap (fPOINTATTR sPnt);
extern void		setxt ();
extern void		shft (fPOINT shPnt);
extern void		shoMsg (TCHAR* str);
extern void		shord ();
extern void		shoseln (unsigned cod0, unsigned cod1);
extern void		spltmsg ();
extern void		srtcol ();
extern void		stch2pxr (fPOINT stpnt);
extern void		tabmsg (unsigned cod);
extern unsigned	toglMap (unsigned bPnt);
extern unsigned	toglu (unsigned bPnt);
extern void		unbsho ();
extern void		uncros ();
extern void		undat ();
extern void		unlin ();
extern void		unmsg ();
extern void		unsel ();
extern void		unsid ();
extern void		zumhom ();

extern			unsigned		ActiveColor;
extern			unsigned		ActiveLayer;
extern			unsigned		BitmapHeight;
extern			unsigned		BitmapWidth;
extern			unsigned		ButtonHeight;
extern			unsigned		ButtonWidth;
extern			unsigned		ButtonWidthX3;
extern			fRECTANGLE		ClipRect;
extern			FLSIZ			ClipRectSize;
extern			unsigned		ClipStitchCount;	//number of stitchs extracted from clipboard
extern			CLPSTCH*		ClipStitchData;
extern			fPOINTATTR		ClipBuffer[MAXFRMLINS];
extern			unsigned		ClosestPointIndex;
extern			POINT			EndPointCross;
extern			unsigned		FormMenuChoice;
extern			HPEN			FormPen;
extern			HPEN			FormPen3px;
extern			HPEN			FormSelectedPen;
#if	 __UseASM__
extern			unsigned		fsizeof;
#endif
extern			unsigned		GroupEndStitch;
extern			unsigned		GroupStartStitch;
extern			unsigned		GroupStitchIndex;
extern			HCURSOR			ArrowCursor;
extern			HWND			CancelButton;
extern			unsigned		Clip;
extern			HGLOBAL			ClipMemory;
extern			HCURSOR			CrossCursor;
extern			HWND			DiscardButton;
extern			PCSHEADER		PCSHeader;
extern			HMENU			FillMenu;
extern			HWND			FormDataSheet;
extern			HINSTANCE		ThredInstance;
extern			HMENU			MainMenu;
extern			HWND			MainStitchWin;
extern			HWND			hMsgWindow;
extern			HWND			OKButton;
extern			HWND			PreferencesWindow;
extern			HWND			SideMessageWindow;
extern			HWND			SideWindow[11];
extern			HWND			DeleteStitchesDialog;
extern			HWND			ThredWindow;
extern			INIFILE			IniFile;
extern			POINT			InsertLine[3];
extern			fPOINT			interleaveSequence[MAXSEQ];
extern			unsigned		interleaveSequenceIndex;
extern			unsigned		interleaveSequenceIndex2;
extern			INSREC			interleaveSequenceIndices[10];
extern			HPEN			LayerPen[5];
extern			POINT			ThredWindowOrigin;
extern			unsigned		MarkedStitchMap[RMAPSIZ];
extern			double			MinStitchLength;
extern			MSG				Msg;
extern			TCHAR			MsgBuffer[MSGSIZ];
extern			unsigned		MsgIndex;
extern			RECT			MsgRect;
extern			HPEN			MultiFormPen;
extern			unsigned		NearestCount;
extern			unsigned		numpnt;
extern			TCHAR*			PcdClipFormat;
extern			unsigned		PreferenceIndex;
extern			void*			ClipPointer;
extern			FRMHED*			SelectedForm;	//pointer to selected form
extern			double			RotationAngle;
extern			dPOINT			RotationCenter;
extern			fRECTANGLE		RotationRect;
extern			POINT			scend;
extern			HPEN			SelectAllPen;
extern			FORMPOINTS		SelectedFormPoints;
extern			fPOINT			SelectedFormsSize;
extern			RECT			SelectedPixelsRect;
extern			fPOINT			SelectedPoint;
extern			fRECTANGLE		SelectedPointsRect;
extern			double			ShowStitchThreshold;
extern			TCHAR			SideWindowEntryBuffer[11];
extern			unsigned		SearchLineIndex;
extern			double			SmallStitchLength;
extern			unsigned		fillStartsMap;
extern			TCHAR*			StringTable[STR_LEN];
extern			double			StitchBoxesThreshold;
extern			fPOINTATTR		StitchBuffer[MAXPCS];
extern			fRECTANGLE		StitchRangeRect;
extern			POINT			StitchSizePixels;
extern			RECT			StitchWindowClientRect;
extern			HDC				StitchWindowDC;
extern			HDC				StitchWindowMemDC;
extern			POINT			StitchWindowOrigin;
extern			POINT			StitchWindowSize;
extern			POINT			StretchBoxLine[5];
extern			TCHAR			ThrName[_MAX_PATH];
extern			int				textureIndex;
extern			TXPNT			texturePointsBuffer[MAXSEQ];
extern			RNGCNT*			textureSegments;
extern			unsigned		UnderlayColor;
extern			void*			UndoBuffer[16];
extern			unsigned		UndoBufferWriteIndex;
extern			POINT			UnzoomedRect;
extern			HPEN			UserPen[16];
extern			double			UserStitchLength;
extern			double			ZoomFactor;
extern			dPOINT			ZoomMarkPoint;
extern			dPOINT			ZoomRatio;
extern			dRECTANGLE		ZoomRect;

void			angclpfn ();
void			angsclp ();
void			apbrd ();
void			bakseq ();
void			bdrlin (unsigned strt, unsigned fin, double pd_Size);
void			bhbrd (double spac);
void			blbrd (double spac);
void			bold (double pd_Size);
void			brdfil (double pd_Size);
void			chan ();
BOOL			chkr (unsigned pbit);
void			chnfn ();
BOOL			cisin (float pntx, float pnty);
void			clpbrd (unsigned short slin);
void			clpcon ();
void			clpic (unsigned short strtlin);
void			clpout ();
void			contf ();
void			deleclp (unsigned ind);
void			delmclp (unsigned ind);
void			duangs ();
void			dubfn ();
void			dufcntr (dPOINT* cntr);
void			dulast ();
void			dufxlen ();
void			duhart (unsigned nsids);
void			duxclp ();
void			filang ();
void			filinsb (dPOINT pnt);
void			filvrt ();
void			flipv ();
void			fmclp ();
void			fnhor ();
void			fnvrt ();
void			frmpnts (unsigned typ);
void			frmsqr (unsigned ind);
void			fvars (unsigned ind);
void			horclpfn ();
void			horsclp ();
BOOL			iseclp (unsigned find);
BOOL			iseclpx (unsigned find);
void			lapbrd ();
void			lcon ();
unsigned short	nxt (unsigned short ind);
void			oclp (fPOINT* p_clp, unsigned p_nclp);
void			pbrd (double spac);
void			plbrd (double spac);
void			prpbrd (double spac);
unsigned short	prv (unsigned ind);
unsigned		psg ();
void			rbrd ();
void			rotfrm (unsigned nu0);
void			satfil ();
void			satout (double satwid);
void			sbrd ();
void			selal ();
unsigned		setdun (unsigned bpnt);
void			setr (unsigned pbit);
void			slbrd ();
void			sRct2px (fRECTANGLE strct, RECT* pxrct);
void			uncon ();
void			uninsf ();
void wavfrm ();

unsigned		formMenuEntryCount;		//lines in the form-form
float			maxStitchLen;			//maximum stitch length
float			userStichLen;			//user stitch length
double*			listSINEs;				//a list of sins of form angles
double*			listCOSINEs;			//a list of cosins of form angles
double			adjustedSpace;			//adjusted space
unsigned		nextStart;				//index of the endpoint of the line segment being processed
fPOINT*			chainEndPoints;			//end points of chain stiches
unsigned		chainSequence[] = { 0,1,2,3,0,1,4,3,0,3 };//chain stich sequence
double			div4;					//chain space divided by four
unsigned		chainCount;				//number of elements of the chain sequence to process
unsigned		closestFormToCursor;	//closest form to the cursor
unsigned		closestVertexToCursor;	//point closest to the cursor
double			verticalRatio;			//vertical ratio between the zoom window
										// and the entire stitch space
double			horizontalRatio;		//horizontal ratio between the zoom window
										// and the entire stitch space
POINT			formLines[MAXFRMLINS];		//used in the form drawing routines
unsigned		newFormVertexCount;		//points in the new form
FORMINFO		formInfo;				//form info used in drawing forms
FRMHED			formList[MAXFORMS];		//a list of pointers to forms
unsigned		formIndex = 0;			//index into the list of forms
double			stitchSpace = DEFSPACE*PFGRAN;//stitch spacing in stitch units
double			fillAngle = DEFANG;		//fill rotation angle
fPOINT*			currentFillVertices;	//pointer to the line of the polygon being filled
fPOINT*			clipFillData;			//data for clipboard fills
fPOINT*			clipReversedData;		//data for clipboard fills
unsigned		stitchLineCount;		//count of stitch lines
SMALPNTL*		lineEndpoints;			//pairs of fill line endpoints
unsigned		activePointIndex;		//pointer to the active point in the sequencing algorithm
unsigned		lineGroupIndex;			//pointer for groups of fill line segments
SMALPNTL**		ptrSortedLines;			//sorted pointers to lineEndpoints
unsigned short	sides;					//sides of the selected form to fill
unsigned		sortedLineIndex;		//for connecting fill lines
unsigned		sequenceIndex;			//sequencing pointer
BSEQPNT			bseq[BSEQLEN];			//reverse sequence for polygon fills
fPOINT			oseq[OSEQLEN];			//temporary storage for sequencing
double			slope;					//slope of line in angle fills
unsigned		satinIndex;				//pointer to next satin point to enter
fPOINT			formMoveDelta;			//offset for moving forms
fPOINT			tempPolygon[MAXFRMLINS];	//temporary storage when user is entering a polygon;
unsigned		outputIndex;			//output pointer for sequencing
double*			lengths;				//array of cumulative lengths used in satin fills
fPOINT*			currentFormVertices;	//points in the currently selected form
SATCON*			currentFormConnections;	//connections in the currently selecteed form
unsigned short	currentFormConnectionsCount;	//number of connections in the currently selected form
unsigned short	wordParam;				//word paramater from the currently selected form
unsigned*		checkMap;				//bitmap used by satin fill to delete illegal lines
unsigned short	startPoint;				//starting point for a satin stitch guide-line
double			plen;					//horizontal length of a clipboard fill/2
double			plen2;					//horizontal length of a clipboard fill
fPOINT			lastPoint;				//last point written by line connect routine
fPOINT			outsidePointList[MAXFRMLINS];	//list of outside outline points for satin or clipboard fills
fPOINT			insidePointList[MAXFRMLINS];	//list of inside outline points for satin or clipboard fills
fPOINT*			outsidePoints;			//pointer to the list of outside outline points
fPOINT*			insidePoints;			//pointer to the list of inside outline points
fPOINT			clipReference;			//clipboard reference point
double			borderWidth = BRDWID;	//border width for satin borders
unsigned		selectedFormControlVertex;	//user selected form control point
POINT			formOutlineRectangle[10];	//form control rectangle in pixel coordinates
double			xyRatio;				//expand form aspect ratio
HWND			thDat[LASTLIN];			//data handles for the form data sheet
HWND			thTxt[LASTLIN];			//text handles for the form data sheet
RECT			leftWindowCoords;		//location of left windows in the form data sheet
RECT			rightWindowCoords;		//location of right windows in the form data sheet
POINT			leftWindowSize;			//size of the left windows in the form data sheet
POINT			rightWindowSize;		//size of the right windows in the form data sheet
fPOINT			lowerLeftStitch;		//lower left point in a form
VRCT2*			pointsVertRect;			//fill points for vertical satin fill
VRCT2*			underlayVertRect;		//underlay fill points for vertical satin fill
POINT			rubberBandLine[3];		//points to form points to be moved
unsigned*		xHistogram;				//x histogram for snap together
double			snapLength = SNPLEN*PFGRAN;		//snap together length
unsigned*		xPoints;				//stitch indices sorted according to x values
unsigned		colorBitmap;			//bitmap of colors in a design for sort
double			starRatio = STARAT;			//star point to body ratio
double			spiralWrap = SPIRWRAP;		//number of revolutions in a spiral
unsigned		srtmsk = (1 << EDGEANGSAT) | (1 << EDGEAPPL) | (1 << EDGEPROPSAT);	 //mask for switchable fill types
fRECTANGLE		boundingRect;			//isin rectangle
RCON*			pathMap;				//path map for sequencing
unsigned		pathMapIndex;			//number of entries in the path map
TCHAR*			visitedRegions;			//visited character map for sequencing
unsigned		visitedIndex;			//next unvisited region for sequencing
unsigned		regionCount;			//number of regions to be sequenced
REGION*			regionsList;			//a list of regions for sequencing
unsigned		doneRegion;				//last region sequenced
double			gapToClosestRegion;		//region close enough threshold for sequencing
unsigned*		mapIndexSequence;		//pointers to sets of adjacent regions
RGSEQ*			regionPath;				//path to a region
unsigned		groupIndexCount;		//number of group indices
unsigned*		groupIndexSequence;		//array of group indices for sequencing
unsigned		lastGroup;				//group of the last line written in the previous region;
FSEQ*			sequencePath;			//path of sequenced regions
RGSEQ*			tmpath;					//temporary path connections
unsigned		sequencePathIndex;		//index to path of sequenced regions
unsigned		nextGroup;				//group that connects to the next region
unsigned*		sequenceFlagBitmap;		//a bitmap of sequenced lines
REGION*			currentRegion;			//region currently being sequenced
fPOINT			lastRegionCorners[4];	//corners of last region sequenced
FRMHED			angledForm;				//a temporary rotated form for angle fills
unsigned short	selectedFormList[MAXFORMS];		//a list of selected forms
unsigned		selectedFormCount = 0;	//number of selected forms
unsigned		previousFormIndex;		//previously selected form
RECT			selectedFormsRectangle;	//for multiple selections;
POINT			selectedFormsLine[9];	//line derived from the big rectangle
POINT			selectedPointsRectangle[9];	//line derived from the point select rectangle
fRECTANGLE		allItemsRectangle;		//rectangle enclosing all forms and stitches
double			formAngles[MAXFRMLINS];	//angles of a form for satin border fills
fPOINT			FormVertices[MAXFRMPNTS];	//form points
unsigned		FormVertexIndex;		//next index to append form points
fPOINT			clipPoints[MAXCLPNTS];	//main clipboard fill points for forms
unsigned		clipPointIndex;			//next index to append main clipboard points
SATCON			satinConns[MAXSAC];		//satin form connects
unsigned		satinConnectIndex;		//next index to append satin connect points
float			buttonholeFillCornerLength = IBFCLEN;	//buttonhole corner length
float			picotSpace = IPICSPAC;	//space between border picots
unsigned		pseudoRandomValue;		//pseudo-random sequence register
dPOINT			satinFillBackup[8];		//backup stitches in satin fills
unsigned		fillBackupIndex;		//pointer for backup stitches in satin fills
double			clipAngle;				//for clipboard border fill
dPOINT			moveToPoint;			//moving point for clipboard fill
double			cosAngle;				//cosine for clipboard fill
double			sinAngle;				//sine for clipboard fill
fPOINT			borderClipReference;	//reference for clipboard line border
unsigned		currentSide;			//active form point for line clipboard fill
dPOINT			vector0;				//x size of the clipboard fill at the fill angle
FRMHED*			tmpFormList;			//temporary form header storage for reordering forms
fPOINT*			tmpFormPoints;			//temporary form vertex storage for reordering forms
SATCON*			tmpSatinConns;			//temporary satin guidline storage for reordering forms
fPOINT*			tmpClipPoints;			//temporary clipboard point storage for reordering forms
unsigned		formRelocationIndex;	//form relocator pointer
fPOINTATTR*		tmpStitchBuffer;		//pointer to high stitch area for form sort
unsigned		beanCount;				//number of stitches added by convert to bean
FRMHED*			formForInsert;			//insert form vertex in this form
unsigned		formVertexPrev;			//form vertex storage for form vertex insert
unsigned		formVertexNext;			//form vertex storage for form vertex insert
SMALPNTL*		sequenceLines;			//line for vertical/horizontal/angle fills
unsigned		mPathIndex;				//point to the next path element for vertical fill sequencing
fPOINT*			workingFormVertices;	//form points for angle fills
fPOINT			angledFormVertices[MAXFRMLINS];	//form point data for angle fills
CLPSEG*			clipSegments;			//clipboard segments for virtual clipboard fill
unsigned		clipSegmentIndex;		//clipboard segment pointer
unsigned short	clipIntersectSide;		//clipboard intersect side;
float**			sortedLengths;			//array of sorted side lengths for vertical clipboard fill
fPOINT			vpnt0;					//vertical clipboard line segment start
fPOINT			vpnt1;					//vertical clipboard line segment end
double*			clipSideLengths;		//lengths of form sides for vertical clipboard fill
CLIPSORT*		clipIntersectData;		//intersect points for vertical clipboard fill
CLIPSORT**		ptrClipIntersectData;	//pointers to line intersect points
CLIPNT*			clipStitchPoints;		//points for vertical clipboard fills
VCLPX			regionCrossingData[MAXFRMLINS];	//region crossing data for vertical clipboard fills
unsigned		regionCrossingStart;	//start of region crossing data for a particular region
unsigned		regionCrossingEnd;		//end of region crossing data for a particular region
float			clipWidth;				//horizontal spacing for vertical clipboard fill
HWND			hTime;					//progress bar
HDC				timeDC;					//progress bar device context
double			timeStep;				//progress bar step
double			timePosition;			//progress bar postiion
FLOAT			formOffset;				//form offset for clipboard fills
long			preferenceWindowWidth;	//width of the preference window
double			eggRatio;				//ratio for shrinking eggs
unsigned		preferenceWindowTextWidth;	//size of the text part of the preference window

TCHAR*		laytxt[] = {
	"0",
	"1",
	"2",
	"3",
	"4"
};

TCHAR		oftxt[16];

MENUITEMINFO meninfo = {
	sizeof(MENUITEMINFO),
	MIIM_TYPE,
	MFT_STRING,
	0,
	0,
	0,
	0,
	0,
	0,
	oftxt,
	16,
};

unsigned short egaray[] = {
	MEGLIN,
	MEGBLD,
	MEGCLP,
	MEGSAT,
	MEGAP,
	MEGPRP,
	MEGHOL,
	MEGPIC,
	MEGDUB,
	MEGCHNH,
	MEGCHNL,
	MEGCLPX,
	0
};

unsigned char lvl00 = 0;
unsigned char lvl01 = 1;
unsigned char lvl02[] = { 0,1 };
unsigned char lvl03[] = { 1,0,2 };
unsigned char lvl04[] = { 1,3,0,2 };
unsigned char lvl05[] = { 2,0,3,1,4 };
unsigned char lvl06[] = { 3,0,2,4,1,5 };
unsigned char lvl07[] = { 3,0,4,1,6,2,5 };
unsigned char lvl08[] = { 4,0,5,1,3,6,2,7 };
unsigned char lvl09[] = { 4,0,5,1,6,2,7,3,8 };
unsigned char lvl10[] = { 5,0,6,1,7,2,8,3,9,4 };
unsigned char lvl11[] = { 5,0,6,1,7,2,8,3,9,10 };
unsigned char lvl12[] = { 6,0,7,1,8,2,9,3,10,4,11 };
unsigned char lvl13[] = { 6,0,1,7,2,8,3,9,4,10,5,11,6,12 };
unsigned char lvl14[] = { 7,0,8,1,9,2,10,3,11,4,12,5,13,6 };
unsigned char lvl15[] = { 7,0,8,1,9,2,10,3,11,4,12,5,13,6,14,7,15 };

unsigned char* lvls[] = {
&lvl00,&lvl01,&lvl02[0],&lvl03[0],&lvl04[0],&lvl05[0],&lvl06[0],&lvl07[0],&lvl08[0],
&lvl09[0],&lvl10[0],&lvl11[0],&lvl12[0],&lvl13[0],&lvl14[0],&lvl15[0]
};


void frmcpy(FRMHED* dst, FRMHED* src) {
#if	 __UseASM__
	unsigned index = sizeof(FRMHED);

	_asm {
		mov		edi, dst
		mov		ecx, index
		shr		ecx, 2
		mov		esi, src
		rep		movsd
	}
#else
	memcpy(dst, src, sizeof(FRMHED));
#endif
}

void frmclr(FRMHED* dst) {
#if	 __UseASM__
	unsigned index = sizeof(FRMHED);

	_asm {
		mov		edi, dst
		mov		ecx, index
		shr		ecx, 2
		xor eax, eax
		rep		stosd
	}
#else
	memset(dst, 0, sizeof(FRMHED));
#endif
}

void duinf(FRMHED* p_hed) {
#if	 __UseASM__
	_asm {
		mov		ebx, p_hed
		mov		eax, [ebx]
		mov		ebx, offset formInfo
		xor		ecx, ecx
		mov		cl, al
		and		cl, 0xf
		mov[ebx], ecx
		add		ebx, 4
		mov		cl, al
		shr		ecx, 4
		and cl, 0xf
		mov[ebx], ecx
		add		ebx, 4
		shr		eax, 8
		and eax, 0xffff
		mov[ebx], eax
	}
#else
	//Correct
	formInfo.type = p_hed->attribute & 0xf;
	formInfo.attribute = (p_hed->attribute >> 4) & 0xf;
	formInfo.sideCount = p_hed->sides;
#endif
}

int comp(const void *arg1, const void *arg2) {
#if	 __UseASM__
	_asm {
		mov		ebx, arg2
		mov		ebx, [ebx]
		add		ebx, 8
		fld		qword ptr[ebx]
		mov		ecx, arg1
		mov		ecx, [ecx]
		add		ecx, 8
		fld		qword ptr[ecx]
		fucompp
		fstsw	ax
		and		ah, 0x43
		je		short cmpg
		cmp		ah, 1
		jne		short cmp1
		xor		eax, eax
		dec		eax
		jmp		short cmpx
		cmp1 : sub		ecx, 8
			   sub		ebx, 8
			   fld		qword ptr[ebx]
			   fld		qword ptr[ecx]
			   fucompp
			   fstsw	ax
			   je		short cmpg
			   cmp		ah, 1
			   jne		short cmp2
			   xor		eax, eax
			   dec		eax
			   jmp		short cmpx
			   cmp2 : xor		eax, eax
					  jmp		short cmpx
					  cmpg : xor		eax, eax
							 inc		eax
							 cmpx :
	}
#else
	const dPOINTLINE **pnts1 = (const dPOINTLINE **)arg1, **pnts2 = (const dPOINTLINE **)arg2;

	if ((*pnts2)->y < (*pnts1)->y)
		return 1;
	if ((*pnts2)->y > (*pnts1)->y)
		return -1;

	if ((*pnts2)->x < (*pnts1)->x)
		return 1;
	if ((*pnts2)->x > (*pnts1)->x)
		return -1;

	return 0;
#endif
}

void getfinfo(unsigned ind) {
	duinf(&formList[ind]);
}

unsigned satind(SATCON* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset satinConns
		shr		eax, 2
	}
#else
	return pnt - satinConns;
#endif
}

void dusqr() {
	if (chku(SQRFIL))
		SelectedForm->extendedAttribute |= AT_SQR;
	else
		SelectedForm->extendedAttribute &= (~AT_SQR);
}

void sacspac(SATCON* strt, unsigned cnt) {
	int			strti, cnti;
	int			src, dst;
	unsigned	ind;

	strti = satind(strt);
	cnti = satinConnectIndex - strti;
	src = satinConnectIndex - 1;
	dst = satinConnectIndex + cnt - 1;
	while (src >= strti) {
		satinConns[dst].start = satinConns[src].start;
		satinConns[dst--].finish = satinConns[src--].finish;
	}
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++) {
		if (formList[ind].type == SAT)
			formList[ind].satinOrAngle.sac += cnt;
	}
	satinConnectIndex += cnt;
}

SATCON* nusac(unsigned pfrm, unsigned cnt) {
	unsigned ind, ine;

	ine = 0;
	for (ind = 0; ind < pfrm; ind++) {
		if (formList[ind].type == SAT)
			ine += formList[ind].satinGuideCount;
	}
	formList[pfrm].satinOrAngle.sac = &satinConns[ine];
	sacspac(formList[pfrm].satinOrAngle.sac, cnt);
	return formList[pfrm].satinOrAngle.sac;
}

void delclps(unsigned ind) {
	deleclp(ind);
	delmclp(ind);
}

unsigned findclp(unsigned fpnt) {
	int ind;

	for (ind = fpnt - 1; ind >= 0; ind--) {
		if (iseclp(ind))
			return formList[ind].borderClipData - clipPoints + formList[ind].clipEntries;
		if (isclp(ind))
			return formList[ind].angleOrClipData.clip - clipPoints + formList[ind].lengthOrCount.clipCount;
	}
	return 0;
}

BOOL chkmax(unsigned arg0, unsigned arg1) {
	if (arg0&MAXMSK)
		return 1;
	if (arg1&MAXMSK)
		return 1;
	if ((arg1 + arg0)&MAXMSK)
		return 1;
	return 0;
}

void clpsub(unsigned fpnt, unsigned cnt) {
	unsigned ind;

	for (ind = fpnt + 1; ind < formIndex; ind++) {
		if (isclpx(ind))
			formList[ind].angleOrClipData.clip -= cnt;
		if (iseclpx(fpnt))
			formList[ind].borderClipData -= cnt;
	}
}

void delmclp(unsigned fpnt) {
	unsigned	src, dst;

	if (clipPointIndex) {
		if (isclp(fpnt)) {
			dst = findclp(fpnt);
			src = dst + formList[fpnt].lengthOrCount.clipCount;
			MoveMemory(&clipPoints[dst], &clipPoints[src], sizeof(fPOINT)*(clipPointIndex - src));
			if (iseclp(fpnt))
				formList[fpnt].borderClipData -= formList[fpnt].lengthOrCount.clipCount;
			clpsub(fpnt, formList[fpnt].lengthOrCount.clipCount);
			if (clipPointIndex > formList[fpnt].lengthOrCount.clipCount)
				clipPointIndex -= formList[fpnt].lengthOrCount.clipCount;
			else
				clipPointIndex = 0;
			formList[fpnt].lengthOrCount.clipCount = 0;
		}
	}
}

void deleclp(unsigned fpnt) {
	unsigned	src, dst;

	if (clipPointIndex) {
		if (iseclpx(fpnt)) {
			dst = findclp(fpnt);
			src = dst + formList[fpnt].clipEntries;
			while (src < clipPointIndex) {
				clipPoints[dst].x = clipPoints[src].x;
				clipPoints[dst++].y = clipPoints[src++].y;
			}
			clpsub(fpnt, formList[fpnt].clipEntries);
			if (clipPointIndex > formList[fpnt].clipEntries)
				clipPointIndex -= formList[fpnt].clipEntries;
			else
				clipPointIndex = 0;
			formList[fpnt].clipEntries = 0;
		}
	}
}

fPOINT* nueclp(unsigned pfrm, unsigned cnt) {
	int			find;
	int			src, dst;
	unsigned	ind;

	find = findclp(closestFormToCursor);
	if (isclp(closestFormToCursor))
		find += formList[closestFormToCursor].lengthOrCount.clipCount;
	src = clipPointIndex - 1;
	dst = clipPointIndex + cnt - 1;
	while (src >= find) {
		clipPoints[dst].x = clipPoints[src].x;
		clipPoints[dst--].y = clipPoints[src--].y;
	}
	formList[find].borderClipData = &clipPoints[find];
	for (ind = pfrm; ind < formIndex; ind++) {
		if (iseclpx(ind))
			formList[ind].borderClipData += cnt;
	}
	for (ind = pfrm + 1; ind < formIndex; ind++) {
		if (isclp(ind))
			formList[ind].angleOrClipData.clip += cnt;
	}
	clipPointIndex += cnt;
	return &clipPoints[find];
}

fPOINT* numclp() {
	int			find;
	int			src, dst;
	unsigned	ind;

	find = findclp(closestFormToCursor);
	src = clipPointIndex - 1;
	dst = clipPointIndex + ClipStitchCount - 1;
	while (src >= find) {
		clipPoints[dst].x = clipPoints[src].x;
		clipPoints[dst--].y = clipPoints[src--].y;
	}
	formList[closestFormToCursor].angleOrClipData.clip = &clipPoints[find];
	if (iseclpx(closestFormToCursor))
		formList[closestFormToCursor].borderClipData += ClipStitchCount;
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++) {
		if (isclpx(ind))
			formList[ind].angleOrClipData.clip += ClipStitchCount;
		if (iseclpx(ind))
			formList[ind].borderClipData += ClipStitchCount;
	}
	clipPointIndex += ClipStitchCount;
	return &clipPoints[find];
}

unsigned fltind(fPOINT* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset FormVertices
		shr		eax, 3
	}
#else
	return pnt - FormVertices;
#endif
}

unsigned sacind(SATCON* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset satinConns
		shr		eax, 2
	}
#else
	return pnt - satinConns;
#endif
}

unsigned clpind(fPOINT* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset clipPoints
		shr		eax, 3
	}
#else
	return pnt - clipPoints;
#endif
}

void fltspac(fPOINT* strt, unsigned cnt) {
	int			strti;
	int			src, dst;
	unsigned	ind;

	strti = fltind(strt);
	src = FormVertexIndex - 1;
	dst = FormVertexIndex + cnt - 1;
	while (src >= strti) {
		FormVertices[dst].x = FormVertices[src].x;
		FormVertices[dst--].y = FormVertices[src--].y;
	}
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
		formList[ind].vertices += cnt;
	FormVertexIndex += cnt;
}

void delsac(unsigned fpnt) {
	unsigned	ind;
	unsigned	src, dst;

	if (satinConnectIndex) {
		if (formList[fpnt].type == SAT&&formList[fpnt].satinGuideCount) {
			dst = satind(formList[fpnt].satinOrAngle.sac);
			src = dst + formList[fpnt].satinGuideCount;
			while (src < satinConnectIndex) {
				satinConns[dst].start = satinConns[src].start;
				satinConns[dst++].finish = satinConns[src++].finish;
			}
			for (ind = fpnt + 1; ind < formIndex; ind++) {
				if (formList[ind].type == SAT&&formList[ind].satinGuideCount)
					formList[ind].satinOrAngle.sac -= formList[fpnt].satinGuideCount;
			}
			satinConnectIndex -= formList[fpnt].satinGuideCount;
		}
	}
	formList[fpnt].satinGuideCount = 0;
}

void delflt(unsigned fpnt) {
	unsigned	ind;
	unsigned	src, dst;

	if (formList[fpnt].sides) {
		dst = fltind(formList[fpnt].vertices);
		src = dst + formList[fpnt].sides;
		while (src < FormVertexIndex) {
			FormVertices[dst].x = FormVertices[src].x;
			FormVertices[dst++].y = FormVertices[src++].y;
		}
		for (ind = fpnt + 1; ind < formIndex; ind++)
			formList[ind].vertices -= formList[fpnt].sides;
		FormVertexIndex -= formList[fpnt].sides;
		if (FormVertexIndex & 0x8000000)
			FormVertexIndex = 0;
	}
}

BOOL chk2of() {
	if (!chkMap(SELBOX))
		return 0;
	if (chku(FIL2OF))
		return 0;
	return 1;
}

void rotbak() {
	unsigned ind;

	for (ind = 0; ind < sequenceIndex; ind++)
		rotflt(&oseq[ind]);
}

void delfil(unsigned at) {
	unsigned ind, ine;

	at &= TYPMSK | FRMSK;
	ine = ind = 0;
	rstMap(WASDEL);
	while ((StitchBuffer[ind].attribute&(TYPMSK | FRMSK)) != at&&ind < PCSHeader.stitchCount)
		ind++;
	ine = ind;
	if (ind < PCSHeader.stitchCount) {
		setMap(WASDEL);
	}
	while (ind < PCSHeader.stitchCount) {
		if ((StitchBuffer[ind].attribute&(TYPMSK | FRMSK)) != at) {
			StitchBuffer[ine].attribute = StitchBuffer[ind].attribute;
			StitchBuffer[ine].x = StitchBuffer[ind].x;
			StitchBuffer[ine++].y = StitchBuffer[ind].y;
		}
		ind++;
	}
	PCSHeader.stitchCount = ine;
}

void delmfil(unsigned col) {
	unsigned at, sat;
	unsigned ind, ine;

	if (istx(closestFormToCursor))
		deltx();
	delmclp(closestFormToCursor);
	col &= 0xf;
	at = closestFormToCursor << FRMSHFT;
	ine = 0;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		sat = StitchBuffer[ind].attribute;
		if (!(sat&NOTFRM))
		{
			if (!((sat&FRMSK) == at && (sat&(TYPFRM | FTHMSK))))
				mvstch(ine++, ind);
		}
		else
			mvstch(ine++, ind);
	}
	PCSHeader.stitchCount = ine;
}

void fsizpar() {
	SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
	SelectedForm->lengthOrCount.stitchLength = UserStitchLength;
	SelectedForm->minFillStitchLen = MinStitchLength;
}

void chkcont() {
	unsigned ind, ine = 0, len, minlen = 10000;

	delmclp(closestFormToCursor);
	deleclp(closestFormToCursor);
	fsizpar();
	if (SelectedForm->fillType != CONTF) {
		if (currentFormConnectionsCount) {
			for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {
				len = SelectedForm->satinOrAngle.sac[ind].finish - SelectedForm->satinOrAngle.sac[ind].start;
				if (len < minlen) {
					minlen = len;
					ine = ind;
				}
			}
			SelectedForm->angleOrClipData.sat.start = SelectedForm->satinOrAngle.sac[ine].start;
			SelectedForm->angleOrClipData.sat.finish = SelectedForm->satinOrAngle.sac[ine].finish;
			delsac(closestFormToCursor);
		}
		else {
			SelectedForm->angleOrClipData.sat.start = 1;
			SelectedForm->angleOrClipData.sat.finish = SelectedForm->sides - 2;
		}
	}
	SelectedForm->type = FRMLINE;
	SelectedForm->fillType = CONTF;
	SelectedForm->attribute |= FRECONT;
}

unsigned find1st() {
	unsigned ind = 0, at;

	if (chk2of())
		return ClosestPointIndex;
	else {
		at = closestFormToCursor << 4;
		while (ind < PCSHeader.stitchCount) {
			if ((StitchBuffer[ind].attribute&FRMSK) == at) {
				if (!(StitchBuffer[ind].attribute&NOTFRM))
					return ind;
			}
			ind++;
		}
		return ind;
	}
}

void ispcdclp() {
	rstMap(WASPCDCLP);
	if (OpenClipboard(ThredWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory)
			setMap(WASPCDCLP);
	}
	CloseClipboard();
}

void frmout(unsigned ind) {
	fRECTANGLE*		trct;
	unsigned	ine;
	float		off;

	if (formList[ind].sides) {
		trct = &formList[ind].rectangle;
		currentFormVertices = formList[ind].vertices;
		trct->left = trct->right = currentFormVertices[0].x;
		trct->bottom = trct->top = currentFormVertices[0].y;
		for (ine = 1; ine < formList[ind].sides; ine++) {
			if (currentFormVertices[ine].x > trct->right)
				trct->right = currentFormVertices[ine].x;
			if (currentFormVertices[ine].x < trct->left)
				trct->left = currentFormVertices[ine].x;
			if (currentFormVertices[ine].y < trct->bottom)
				trct->bottom = currentFormVertices[ine].y;
			if (currentFormVertices[ine].y > trct->top)
				trct->top = currentFormVertices[ine].y;
		}
		if (trct->top - trct->bottom < MINRCT) {
			off = (MINRCT - (trct->top - trct->bottom)) / 2;
			trct->top += off;
			trct->bottom -= off;
		}
		if (trct->right - trct->left < MINRCT) {
			off = (MINRCT - (trct->right - trct->left)) / 2;
			trct->left -= off;
			trct->right += off;
		}
	}
}

void sfCor2px(fPOINT stpnt, POINT* pxpnt) {
	pxpnt->x = (stpnt.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	pxpnt->y = StitchWindowClientRect.bottom - (stpnt.y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void stCor2px(fPOINTATTR stpnt, POINT* pxpnt) {
	pxpnt->x = (stpnt.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	pxpnt->y = StitchWindowClientRect.bottom - (stpnt.y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void px2stchf(POINT pxpnt, fPOINT* stpnt) {
	double	tdub;

	tdub = (double)(pxpnt.x - StitchWindowClientRect.left) / StitchWindowClientRect.right;
	stpnt->x = tdub*(ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	tdub = (double)(StitchWindowClientRect.bottom - pxpnt.y) / StitchWindowClientRect.bottom;
	stpnt->y = tdub*(ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

void frmlin(fPOINT* scr, unsigned sidz) {
	unsigned ind;

	if (sides) {
		for (ind = 0; ind < sidz; ind++) {
			formLines[ind].x = (scr[ind].x - ZoomRect.left)*ZoomRatio.x;
			formLines[ind].y = StitchWindowClientRect.bottom - (scr[ind].y - ZoomRect.bottom)*ZoomRatio.y;
		}
		formLines[ind].x = (scr[0].x - ZoomRect.left)*ZoomRatio.x;
		formLines[ind].y = StitchWindowClientRect.bottom - (scr[0].y - ZoomRect.bottom)*ZoomRatio.y;
	}
}

void dufrm() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, formLines, newFormVertexCount);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unfrm() {
	if (rstMap(SHOFRM))
		dufrm();
}

void mdufrm() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if (formList[closestFormToCursor].type == FRMLINE)
		Polyline(StitchWindowDC, formLines, newFormVertexCount - 1);
	else
		Polyline(StitchWindowDC, formLines, newFormVertexCount);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void munfrm() {
	if (rstMap(SHOFRM))
		mdufrm();
}

void rats() {
	if (chkMap(ZUMED)) {
		horizontalRatio = (double)(ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
		verticalRatio = (double)(ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
	}
	else {
		horizontalRatio = (double)UnzoomedRect.x / StitchWindowClientRect.right;
		verticalRatio = (double)UnzoomedRect.y / StitchWindowClientRect.bottom;
	}
}

void setfrm() {
	unsigned	ind;
	fPOINT		tpnt;
	fPOINT		dif;

	rats();
	closestFormToCursor = formIndex;
	fvars(closestFormToCursor);
	px2stchf(formLines[0], &tpnt);
	dif.x = tpnt.x - currentFormVertices[0].x;
	dif.y = tpnt.y - currentFormVertices[0].y;
	SelectedForm->rectangle.left = SelectedForm->rectangle.bottom = (float)1e30;
	SelectedForm->rectangle.right = SelectedForm->rectangle.top = 0;
	for (ind = 0; ind < newFormVertexCount - 1; ind++) {
		currentFormVertices[ind].x += dif.x;
		currentFormVertices[ind].y += dif.y;
		if (currentFormVertices[ind].x < SelectedForm->rectangle.left)
			SelectedForm->rectangle.left = currentFormVertices[ind].x;
		if (currentFormVertices[ind].x > SelectedForm->rectangle.right)
			SelectedForm->rectangle.right = currentFormVertices[ind].x;
		if (currentFormVertices[ind].y > SelectedForm->rectangle.top)
			SelectedForm->rectangle.top = currentFormVertices[ind].y;
		if (currentFormVertices[ind].y < SelectedForm->rectangle.bottom)
			SelectedForm->rectangle.bottom = currentFormVertices[ind].y;
	}
	formIndex++;
	rstMap(FORMIN);
	setMap(INIT);
	setMap(RESTCH);
}

void form() {
	shoMsg(StringTable[STR_FMEN]);
	setMap(FORMIN);
	rstMap(INSRT);
	duzrat();
}

void frmsqr(unsigned ind) {
	double	lang;
	double	rat;
	float	len;
	fPOINT	off;
	fPOINT	fpnt;
	fPOINT	l_dpnt;
	dPOINT	dif;
	POINT	sqlin[4];

	stch2pxr(currentFormVertices[ind]);
	sqlin[1].x = StitchSizePixels.x;
	sqlin[1].y = StitchSizePixels.y;
	rat = (double)IniFile.formPointSizePixels / StitchWindowClientRect.right;
	len = (ZoomRect.right - ZoomRect.left)*rat * 2;
	dif.x = currentFormVertices[ind - 1].x - currentFormVertices[ind].x;
	dif.y = currentFormVertices[ind - 1].y - currentFormVertices[ind].y;
	lang = atan2(dif.y, dif.x);
	off.x = len*cos(lang);
	off.y = len*sin(lang);
	fpnt.x = currentFormVertices[ind].x + off.x;
	fpnt.y = currentFormVertices[ind].y + off.y;
	lang = atan2(-dif.x, dif.y);
	len /= 2;
	off.x = len*cos(lang);
	off.y = len*sin(lang);
	l_dpnt.x = fpnt.x + off.x;
	l_dpnt.y = fpnt.y + off.y;
	stch2pxr(l_dpnt);
	sqlin[0].x = sqlin[3].x = StitchSizePixels.x;
	sqlin[0].y = sqlin[3].y = StitchSizePixels.y;
	l_dpnt.x = fpnt.x - off.x;
	l_dpnt.y = fpnt.y - off.y;
	stch2pxr(l_dpnt);
	sqlin[2].x = StitchSizePixels.x;
	sqlin[2].y = StitchSizePixels.y;
	Polyline(StitchWindowMemDC, sqlin, 4);
}

void selsqr(POINT p_cpnt, HDC dc) {
	POINT	sqlin[5];

	sqlin[0].x = sqlin[3].x = sqlin[4].x = p_cpnt.x - IniFile.formPointSizePixels;
	sqlin[0].y = sqlin[1].y = p_cpnt.y - IniFile.formPointSizePixels;
	sqlin[1].x = sqlin[2].x = p_cpnt.x + IniFile.formPointSizePixels;
	sqlin[2].y = sqlin[3].y = p_cpnt.y + IniFile.formPointSizePixels;
	sqlin[4].y = p_cpnt.y - IniFile.formPointSizePixels;
	Polyline(dc, sqlin, 5);
}

void frmsqr0(POINT p_cpnt) {
	POINT		sqlin[5];
	unsigned	pix;

	pix = IniFile.formBoxSizePixels;
	if (pix)
	{
		sqlin[0].x = sqlin[3].x = sqlin[4].x = p_cpnt.x - pix;
		sqlin[0].y = sqlin[1].y = p_cpnt.y - pix;
		sqlin[1].x = sqlin[2].x = p_cpnt.x + pix + 1;
		sqlin[2].y = sqlin[3].y = p_cpnt.y + pix + 1;
		sqlin[4].y = p_cpnt.y - 1;
		Polyline(StitchWindowMemDC, sqlin, 5);
	}
}

void frmx(POINT p_cpnt, HDC dc) {
	POINT	xlin[2];

	SelectObject(dc, FormSelectedPen);
	xlin[0].x = xlin[1].x = p_cpnt.x;
	xlin[0].y = p_cpnt.y + 8;
	xlin[1].y = p_cpnt.y - 8;
	Polyline(dc, xlin, 2);
	xlin[0].y = xlin[1].y = p_cpnt.y;
	xlin[0].x = p_cpnt.x - 8;
	xlin[1].x = p_cpnt.x + 8;
	Polyline(dc, xlin, 2);
	SelectObject(dc, FormPen);
}

void ratsr() {
	if (chkMap(ZUMED)) {
		horizontalRatio = (double)StitchWindowClientRect.right / (ZoomRect.right - ZoomRect.left);
		verticalRatio = (double)StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
	}
	else {
		horizontalRatio = (double)StitchWindowClientRect.right / UnzoomedRect.x;
		verticalRatio = (double)StitchWindowClientRect.bottom / UnzoomedRect.y;
	}
}

float midl(float hi, float lo) {
	return (hi - lo) / 2 + lo;
}

void fvars(unsigned ind) {
	SelectedForm = &formList[ind];
	currentFormVertices = formList[ind].vertices;
	sides = formList[ind].sides;
	currentFormConnections = formList[ind].satinOrAngle.sac;
	currentFormConnectionsCount = formList[ind].satinGuideCount;
	wordParam = formList[ind].wordParam;
}

void ritfrct(unsigned ind, HDC dc) {
	unsigned	ine;
	POINT		sqrlin[10];
	fRECTANGLE*		trct;
	fPOINT		rlin[10];

	ratsr();
	SelectObject(StitchWindowDC, FormPen);
	SetROP2(StitchWindowDC, R2_XORPEN);
	getfinfo(ind);
	trct = &formList[ind].rectangle;
	SelectObject(dc, FormSelectedPen);
	rlin[0].x = rlin[6].x = rlin[7].x = rlin[8].x = trct->left;
	rlin[1].x = rlin[5].x = midl(trct->right, trct->left);
	rlin[0].y = rlin[1].y = rlin[2].y = rlin[8].y = trct->top;
	rlin[3].y = rlin[7].y = midl(trct->top, trct->bottom);
	rlin[4].y = rlin[5].y = rlin[6].y = trct->bottom;
	rlin[2].x = rlin[3].x = rlin[4].x = trct->right;
	for (ind = 0; ind < 8; ind++)
		sfCor2px(rlin[ind], &sqrlin[ind]);
	sfCor2px(rlin[0], &sqrlin[ind]);
	Polyline(dc, sqrlin, 9);
	for (ine = 0; ine < 8; ine++)
		selsqr(sqrlin[ine], dc);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	if (rstMap(GRPSEL)) {
		rstMap(SELSHO);
		SearchLineIndex = 0;
		setMap(RESTCH);
	}
}

void delfrms() {
	unsigned ind;

	savdo();
	formIndex = FormVertexIndex = satinConnectIndex = clipPointIndex = 0;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		StitchBuffer[ind].attribute &= NFRM_NTYP;
		StitchBuffer[ind].attribute |= NOTFRM;
	}
}

void fselrct(unsigned fpnt) {
	FRMHED*		tpnt = &formList[fpnt];
	fPOINT		tlin[5];
	POINT		sqrlin[6];
	unsigned	ind;

	tlin[0].x = tlin[3].x = tlin[4].x = tpnt->rectangle.left;
	tlin[1].x = tlin[2].x = tpnt->rectangle.right;
	tlin[0].y = tlin[1].y = tlin[4].y = tpnt->rectangle.top;
	tlin[2].y = tlin[3].y = tpnt->rectangle.bottom;
	for (ind = 0; ind < 5; ind++) {
		sqrlin[ind].x = (tlin[ind].x - ZoomRect.left)*horizontalRatio;
		sqrlin[ind].y = (ZoomRect.top - tlin[ind].y)*verticalRatio;
		if (sqrlin[ind].x < selectedFormsRectangle.left)
			selectedFormsRectangle.left = sqrlin[ind].x;
		if (sqrlin[ind].x > selectedFormsRectangle.right)
			selectedFormsRectangle.right = sqrlin[ind].x;
		if (sqrlin[ind].y < selectedFormsRectangle.top)
			selectedFormsRectangle.top = sqrlin[ind].y;
		if (sqrlin[ind].y > selectedFormsRectangle.bottom)
			selectedFormsRectangle.bottom = sqrlin[ind].y;
	}
	sqrlin[5].x = (tlin[0].x - ZoomRect.left)*horizontalRatio;
	sqrlin[5].y = (ZoomRect.top - tlin[0].y)*verticalRatio;
	if (sqrlin[5].x < selectedFormsRectangle.left)
		selectedFormsRectangle.left = sqrlin[5].x;
	if (sqrlin[5].x > selectedFormsRectangle.right)
		selectedFormsRectangle.right = sqrlin[5].x;
	if (sqrlin[5].y < selectedFormsRectangle.top)
		selectedFormsRectangle.top = sqrlin[5].y;
	if (sqrlin[5].y > selectedFormsRectangle.bottom)
		selectedFormsRectangle.bottom = sqrlin[5].y;
	Polyline(StitchWindowMemDC, sqrlin, 5);
}

void rct2sel(RECT rct, POINT* p_lin) {
	p_lin[0].x = p_lin[6].x = p_lin[7].x = p_lin[8].x = rct.left;
	p_lin[1].x = p_lin[5].x = ((rct.right - rct.left) >> 1) + rct.left;
	p_lin[2].x = p_lin[3].x = p_lin[4].x = rct.right;
	p_lin[0].y = p_lin[1].y = p_lin[2].y = p_lin[8].y = rct.top;
	p_lin[3].y = p_lin[7].y = ((rct.bottom - rct.top) >> 1) + rct.top;
	p_lin[4].y = p_lin[5].y = p_lin[6].y = rct.bottom;
}

void dubig() {
	unsigned ind;

	rct2sel(selectedFormsRectangle, selectedFormsLine);
	SelectObject(StitchWindowMemDC, SelectAllPen);
	Polyline(StitchWindowMemDC, selectedFormsLine, 9);
	for (ind = 0; ind < 8; ind++)
		selsqr(selectedFormsLine[ind], StitchWindowMemDC);
}

void frmpoly(POINT* p_lin, unsigned cnt) {
	unsigned ind;

	if (cnt) {
		for (ind = 0; ind < cnt - 1; ind++)
			Polyline(StitchWindowMemDC, &p_lin[ind], 2);
	}
}

void dupsel(HDC dc) {
	unsigned ind;

	SelectObject(dc, FormPen);
	SetROP2(dc, R2_XORPEN);
	Polyline(dc, selectedPointsRectangle, 9);
	ind = SelectedFormPoints.start;
	for (ind = 0; ind < 8; ind++)
		selsqr(selectedPointsRectangle[ind], dc);
	frmx(EndPointCross, dc);
}

void unpsel() {
	if (rstMap(SHOPSEL))
		dupsel(StitchWindowDC);
}

void drwfrm() {
	unsigned	ind, ine, inf, layr;
	POINT		l_lin[2];
	dPOINT		tpnt;

	rstMap(SHOMOV);
	rstMap(SHOPSEL);
	l_lin[0].x = l_lin[0].y = l_lin[1].x = l_lin[1].y = 0;
	Polyline(StitchWindowMemDC, l_lin, 2);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	ratsr();
	duzrat();
	for (ind = 0; ind < formIndex; ind++) {
		fvars(ind);
		frmlin(SelectedForm->vertices, sides);
		inf = 0;
		layr = ((SelectedForm->attribute&FRMLMSK) >> 1);
		if (!ActiveLayer || !layr || layr == ActiveLayer) {
			if (SelectedForm->type == SAT) {
				if (SelectedForm->attribute&FRMEND) {
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, formLines, 2);
					inf = 1;
				}
				if (SelectedForm->wordParam) {
					SelectObject(StitchWindowMemDC, FormPen);
					frmpoly(&formLines[1], SelectedForm->wordParam);
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, &formLines[SelectedForm->wordParam], 2);
					SelectObject(StitchWindowMemDC, LayerPen[layr]);
					inf = SelectedForm->wordParam + 1;
				}
				for (ine = 0; ine < formList[ind].satinGuideCount; ine++) {
					sfCor2px(currentFormVertices[currentFormConnections[ine].start], &l_lin[0]);
					sfCor2px(currentFormVertices[currentFormConnections[ine].finish], &l_lin[1]);
					SelectObject(StitchWindowMemDC, FormPen);
					Polyline(StitchWindowMemDC, l_lin, 2);
				}
			}
			SelectObject(StitchWindowMemDC, LayerPen[layr]);
			if (SelectedForm->type == FRMLINE) {
				frmpoly(formLines, sides);
				if (SelectedForm->fillType == CONTF) {
					tpnt.x = currentFormVertices[SelectedForm->angleOrClipData.sat.start].x;
					tpnt.y = currentFormVertices[SelectedForm->angleOrClipData.sat.start].y;
					sCor2px(tpnt, &l_lin[0]);
					tpnt.x = currentFormVertices[SelectedForm->angleOrClipData.sat.finish].x;
					tpnt.y = currentFormVertices[SelectedForm->angleOrClipData.sat.finish].y;
					sCor2px(tpnt, &l_lin[1]);
					Polyline(StitchWindowMemDC, l_lin, 2);
				}
			}
			else
				frmpoly(&formLines[inf], sides + 1 - inf);
			if (closestFormToCursor == ind&&chkMap(FRMPSEL)) {
				for (ine = 1; ine < sides; ine++) {
					if (ine == closestVertexToCursor)
						frmx(formLines[ine], StitchWindowMemDC);
					else
						frmsqr(ine);
				}
				if (closestVertexToCursor)
					frmsqr0(formLines[0]);
				else
					frmx(formLines[0], StitchWindowMemDC);
				ritnum(STR_NUMPNT, closestVertexToCursor);
			}
			else {
				for (ine = 1; ine < sides; ine++)
					frmsqr(ine);
				SelectObject(StitchWindowMemDC, FormSelectedPen);
				frmsqr0(formLines[0]);
			}
			if (chkMap(FPSEL) && closestFormToCursor == ind) {
				sRct2px(SelectedPointsRect, &SelectedPixelsRect);
				rct2sel(SelectedPixelsRect, selectedPointsRectangle);
				setMap(SHOPSEL);
				dupsel(StitchWindowMemDC);
			}
		}
	}
	if (selectedFormCount) {
		SelectObject(StitchWindowMemDC, MultiFormPen);
		ratsr();
		selectedFormsRectangle.top = selectedFormsRectangle.left = 0x7fffffff;
		selectedFormsRectangle.bottom = selectedFormsRectangle.right = 0;
		for (ind = 0; ind < selectedFormCount; ind++)
			fselrct(selectedFormList[ind]);
		SelectedFormsSize.x = selectedFormsRectangle.right - selectedFormsRectangle.left;
		SelectedFormsSize.y = selectedFormsRectangle.bottom - selectedFormsRectangle.top;
		dubig();
	}
	else {
		if (chkMap(FORMSEL))
			ritfrct(closestFormToCursor, StitchWindowMemDC);
		if (chkMap(FRMPMOV)) {
			ritmov();
			rubberBandLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
			rubberBandLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
			setMap(SHOMOV);
			ritmov();
		}
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void setpoli() {
	frmclr(&formList[formIndex]);
	formList[formIndex].type = FRMFPOLY;
	//frmtyp = FRMFPOLY;
	formList[formIndex].attribute = ActiveLayer << 1;
	formList[formIndex].underlayIndent = IniFile.underlayIndent;
	setMap(SATIN);
	setMap(INIT);
}

void setlin() {
	frmclr(&formList[formIndex]);
	formList[formIndex].type = FRMLINE;
	//frmtyp = FRMLINE;
	formList[formIndex].attribute = ActiveLayer << 1;
	setMap(SATIN);
	setMap(INIT);
}

void setrpoli() {
	unmsg();
	pntmsg(IDS_REGP);
	setMap(NUMIN);
	setMap(ENTRPOL);
	numWnd();
}

void setstar() {
	unmsg();
	pntmsg(IDS_STAR);
	setMap(NUMIN);
	setMap(ENTRSTAR);
	numWnd();
}

void setspir() {
	unmsg();
	pntmsg(IDS_SPIR);
	setMap(NUMIN);
	setMap(ENTRSPIR);
	numWnd();
}

void sethart() {
	unmsg();
	pntmsg(IDS_HEART);
	setMap(NUMIN);
	setMap(ENTRHART);
	numWnd();
}

void setlens() {
	unmsg();
	pntmsg(IDS_LENS);
	setMap(NUMIN);
	setMap(ENTRLENS);
	numWnd();
}

void seteg() {
	unmsg();
	pntmsg(IDS_EGG);
	setMap(NUMIN);
	setMap(ENTREG);
	numWnd();
}

void setzig() {
	unmsg();
	pntmsg(IDS_ZIG);
	setMap(NUMIN);
	setMap(ENTRZIG);
	numWnd();
}

BOOL CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	TCHAR		buf[HBUFSIZ];

	switch (umsg) {
	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	reinit:;
		sprintf_s(buf, sizeof(buf), "%d", IniFile.formSides);
		SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buf);
		sprintf_s(buf, sizeof(buf), "%.3f", IniFile.tearTailLength);
		SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buf);
		sprintf_s(buf, sizeof(buf), "%.3f", IniFile.tearTwistStep / PFGRAN);
		SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buf);
		sprintf_s(buf, sizeof(buf), "%.3f", IniFile.tearTwistRatio);
		SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buf);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buf, HBUFSIZ);
			IniFile.formSides = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buf, HBUFSIZ);
			IniFile.tearTailLength = atof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buf, HBUFSIZ);
			IniFile.tearTwistStep = atof(buf)*PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buf, HBUFSIZ);
			IniFile.tearTwistRatio = atof(buf);
			EndDialog(hwndlg, 1);
			break;

		case IDC_DEFTEAR:

			IniFile.formSides = 20;
			IniFile.tearTailLength = (float)1.1;
			IniFile.tearTwistStep = (float)0.0;
			IniFile.tearTwistRatio = (float)1.6;
			goto reinit;

		case IDC_DEFPAIS:

			IniFile.formSides = 24;
			IniFile.tearTailLength = (float)1.15;
			IniFile.tearTwistStep = (float)0.3*PFGRAN;
			IniFile.tearTwistRatio = (float)1.8;
			goto reinit;
		}
	}
	return 0;
}

void setear() {
	unsigned	ind, cnt, rind, lind;
	double		mid;
	double		vpos;
	double		len;
	double		stp;
	double		hrat;
	double		vrat;
	float		xstp;
	fPOINT		lSize;

	unmsg();
	ind = DialogBox(ThredInstance, MAKEINTRESOURCE(IDD_TEAR), ThredWindow, (DLGPROC)tearprc);
	if (ind > 0) {
		xstp = IniFile.tearTwistStep;
		durpoli(IniFile.formSides);
		fvars(formIndex);
		cnt = sides / 4;
		mid = (currentFormVertices[1].x - currentFormVertices[0].x) / 2 + currentFormVertices[0].x;
		len = currentFormVertices[cnt].y - currentFormVertices[0].y;
		stp = currentFormVertices[cnt + 1].y - currentFormVertices[cnt].y;
		vpos = currentFormVertices[cnt + 1].y;
		lind = sides - cnt;
		rind = cnt + 1;
		for (ind = 0; ind < (unsigned)cnt; ind++) {
			currentFormVertices[rind].y = currentFormVertices[lind].y = vpos;
			currentFormVertices[rind].x += xstp;
			currentFormVertices[lind].x += xstp;
			xstp *= IniFile.tearTwistRatio;
			vpos -= stp;
			stp *= IniFile.tearTailLength;
			rind--;
			lind++;
		}
		currentFormVertices[0].y = currentFormVertices[1].y = vpos;
		currentFormVertices[0].x += xstp;
		currentFormVertices[1].x += xstp;
		vpos -= stp / 2;
		currentFormVertices[sides].x = currentFormVertices[0].x;
		currentFormVertices[sides].y = currentFormVertices[0].y;
		if (xstp)
			currentFormVertices[0].x = currentFormVertices[1].x + xstp / 4;
		else
			currentFormVertices[0].x = mid;
		currentFormVertices[0].y = vpos;
		SelectedForm->sides++;
		newFormVertexCount++;
		FormVertexIndex++;
		setMap(FORMSEL);
		fvars(formIndex);
		frmout(formIndex);
		flipv();
		rstMap(FORMSEL);
		lSize.x = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		lSize.y = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		hrat = UnzoomedRect.x / 4 / lSize.x;
		if (hrat > 1)
			hrat = 1;
		vrat = UnzoomedRect.y / 4 / lSize.y;
		if (vrat < hrat)
			hrat = vrat;
		if (hrat < 1) {
			for (ind = 0; ind < sides; ind++) {
				currentFormVertices[ind].x = (currentFormVertices[ind].x - currentFormVertices[0].x)*hrat + currentFormVertices[0].x;
				currentFormVertices[ind].y = (currentFormVertices[ind].y - currentFormVertices[0].y)*hrat + currentFormVertices[0].y;
			}
		}
		frmout(formIndex);
		for (ind = 0; ind < sides; ind++) {
			currentFormVertices[ind].x -= SelectedForm->rectangle.left;
			currentFormVertices[ind].y -= SelectedForm->rectangle.bottom;
		}
	}
}

void duform(unsigned ind) {
	frmclr(&formList[formIndex]);
	switch (ind + 1) {
	case FRMLINE:

		setlin();
		break;

	case FRMFPOLY:

		setpoli();
		break;

	case FRMRPOLY:

		setrpoli();
		break;

	case FRMSTAR:

		setstar();
		break;

	case FRMSPIRAL:

		setspir();
		break;

	case FRMHEART - 1:

		sethart();
		break;

	case FRMLENS - 1:

		setlens();
		break;

	case FRMEGG - 1:

		seteg();
		break;

	case FRMTEAR - 1:

		setear();
		break;

	case FRMZIGZAG - 1:

		setzig();
		break;

	case FRMWAVE - 1:

		wavfrm();
		break;

	case FRMDAISY - 1:

		dasyfrm();
		break;
	}
}

unsigned closfrm() {
	unsigned	ind, ine, tind, tine, playcod, frmcod;
	fPOINT*		pdat;
	fPOINT		tpnt;
	POINT		stchpx;
	double		len, minlen = 1e99;

	if (formIndex) {
		stchpx.x = Msg.pt.x - StitchWindowOrigin.x;
		stchpx.y = Msg.pt.y - StitchWindowOrigin.y;
		rats();
		tind = tine = 0;
		px2stchf(stchpx, &tpnt);
		playcod = ActiveLayer << 1;
		for (ind = 0; ind < formIndex; ind++) {
			if (chkMap(FRMSAM) && ind == closestFormToCursor)
				continue;

			frmcod = formList[ind].attribute&FRMLMSK;
			if (!ActiveLayer || !frmcod || frmcod == playcod) {
				getfinfo(ind);
				pdat = formList[ind].vertices;
				for (ine = 0; ine < formInfo.sideCount; ine++) {
					len = hypot(tpnt.x - pdat[ine].x, tpnt.y - pdat[ine].y);
					if (len < minlen&&len >= 0) {
						minlen = len;
						tind = ind;
						tine = ine;
					}
				}
			}
		}
		stch2pxr(formList[tind].vertices[tine]);
		minlen = hypot(StitchSizePixels.x - stchpx.x, StitchSizePixels.y - stchpx.y);
		if (minlen < CLOSENUF) {
			closestFormToCursor = tind;
			closestVertexToCursor = tine;
			fvars(closestFormToCursor);
			setMap(RELAYR);
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

void frmovlin() {
	unsigned ind, ine;

	fvars(closestFormToCursor);
	ratsr();
	if (SelectedForm->type == FRMLINE)
		newFormVertexCount = SelectedForm->sides;
	else
		newFormVertexCount = SelectedForm->sides + 1;
	frmlin(SelectedForm->vertices, SelectedForm->sides);
	ine = prv(closestVertexToCursor);
	for (ind = 0; ind < 3; ind++)
	{
		rubberBandLine[ind].x = formLines[ine].x;
		rubberBandLine[ind].y = formLines[ine].y;
		ine++;
	}
	ritmov();
}

void makspac(unsigned strt, unsigned cnt) {
	if (!chkmax(PCSHeader.stitchCount, cnt))
	{
		MoveMemory(&StitchBuffer[strt + cnt], &StitchBuffer[strt], sizeof(fPOINTATTR)*(PCSHeader.stitchCount - strt));
		PCSHeader.stitchCount += cnt;
	}
}

void rseq(unsigned strt, unsigned fin, unsigned ostrt, unsigned at) {
	while (strt < fin) {
		StitchBuffer[strt].attribute = at;
		StitchBuffer[strt].x = interleaveSequence[ostrt].x;
		StitchBuffer[strt++].y = interleaveSequence[ostrt++].y;
	}
	ostrt--;
	lastPoint.x = interleaveSequence[ostrt].x;
	lastPoint.y = interleaveSequence[ostrt].y;
}

BOOL ritlin(fPOINT strt, fPOINT fin)
{
	dPOINT		dif, stp, pnt;
	double		len;
	unsigned	ine, cnt;

	dif.x = fin.x - strt.x;
	dif.y = fin.y - strt.y;
	len = hypot(dif.x, dif.y);
	interleaveSequence[interleaveSequenceIndex].x = strt.x;
	interleaveSequence[interleaveSequenceIndex++].y = strt.y;
	if (len > maxStitchLen) {
		cnt = ceil(len / userStichLen);
		if (!cnt)
			cnt = 1;
		while (len / cnt > maxStitchLen)
			cnt++;
		if (!chkmax(interleaveSequenceIndex, cnt)) {
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
			pnt.x = strt.x + stp.x;
			pnt.y = strt.y + stp.y;
			for (ine = 0; ine < cnt - 1; ine++) {
				if (interleaveSequenceIndex&MAXMSK) {
					interleaveSequenceIndex = MAXSEQ - 2;
					return 0;
				}
				interleaveSequence[interleaveSequenceIndex].x = pnt.x;
				interleaveSequence[interleaveSequenceIndex++].y = pnt.y;
				pnt.x += stp.x;
				pnt.y += stp.y;
			}
		}
		else {
			sequenceIndex = MAXSEQ - 2;
			return 0;
		}
	}
	return 1;
}

unsigned closflt(float px, float py)
{
	double		len, minlen;
	unsigned	ind, ine = 0;

	minlen = 1e99;
	for (ind = 0; ind < sides; ind++)
	{
		len = hypot(px - currentFormVertices[ind].x, py - currentFormVertices[ind].y);
		if (len < minlen)
		{
			ine = ind;
			minlen = len;
		}
	}
	return ine;
}

void chkseq(BOOL brd) {
#if BUGBAK

	unsigned index;

	for (index = 0; index < sequenceIndex; index++)
	{
		interleaveSequence[index].x = oseq[index].x;
		interleaveSequence[index].y = oseq[index].y;
	}
	interleaveSequenceIndex = sequenceIndex;
#else

	double		len;
	unsigned	ind, ine, bakind;
	float		mins;

	bakind = interleaveSequenceIndex;
	if (brd) {
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
		maxStitchLen = SelectedForm->maxBorderStitchLen;
		if (SelectedForm->edgeType == EDGELCHAIN || SelectedForm->edgeType == EDGEOCHAIN)
			userStichLen = 9 * PFGRAN;
		else
			userStichLen = SelectedForm->edgeStitchLen;
		mins = SelectedForm->minBorderStitchLen;
	}
	else {
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		maxStitchLen = SelectedForm->maxFillStitchLen;
		if (isclp(closestFormToCursor))
			userStichLen = maxStitchLen;
		else
			userStichLen = SelectedForm->lengthOrCount.stitchLength;
		mins = SelectedForm->minFillStitchLen;
	}
	if (userStichLen > maxStitchLen)
		userStichLen = maxStitchLen;
	ine = 0;
	for (ind = 0; ind < sequenceIndex - 1; ind++)
	{
		if (!ritlin(oseq[ind], oseq[ind + 1]))
			goto seqskp;
	}
	interleaveSequence[interleaveSequenceIndex].x = oseq[ind].x;
	interleaveSequence[interleaveSequenceIndex++].y = oseq[ind].y;
seqskp:;
	if (!mins)
		return;
	ine = bakind + 1;
	for (ind = bakind + 1; ind < interleaveSequenceIndex; ind++) {
		len = hypot(interleaveSequence[ind].x - interleaveSequence[ind - 1].x, interleaveSequence[ind].y - interleaveSequence[ind - 1].y);
		if (len > mins) {
			interleaveSequence[ine].x = interleaveSequence[ind].x;
			interleaveSequence[ine].y = interleaveSequence[ind].y;
			ine++;
		}
	}
	interleaveSequenceIndex = ine;
#endif
}

void ritbrd() {
	if (sequenceIndex)
	{
		interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
		interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_BRD;
		interleaveSequenceIndices[interleaveSequenceIndex2].cod = TYPBRD;
		interleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->borderColor&COLMSK;
		chkseq(1);
		interleaveSequenceIndex2++;
	}
}

void ritapbrd() {
	if (sequenceIndex)
	{
		interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
		interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_AP;
		interleaveSequenceIndices[interleaveSequenceIndex2].cod = TYPMSK;
		interleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->borderColor >> 4;
		chkseq(1);
		interleaveSequenceIndex2++;
	}
}

void ritfil() {
	if (sequenceIndex)
	{
		interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
		interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_FIL;
		interleaveSequenceIndices[interleaveSequenceIndex2].cod = TYPFRM;
		interleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillColor;
		chkseq(0);
		interleaveSequenceIndex2++;
	}
}

void okcan() {
	GetClientRect(hMsgWindow, &MsgRect);

	OKButton = CreateWindow(
		"STATIC",
		StringTable[STR_OKENT],
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		MsgRect.bottom + 15,
		ButtonWidth << 2,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThredInstance,
		NULL);

	CancelButton = CreateWindow(
		"STATIC",
		StringTable[STR_CANCEL],
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidth * 5,
		MsgRect.bottom + 15,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThredInstance,
		NULL);
}

void savdisc() {
	TCHAR	buf[HBUFSIZ];

	sdmsg();
	rstMap(BIGBOX);

	GetClientRect(hMsgWindow, &MsgRect);

	LoadString(ThredInstance, IDS_SAV, buf, HBUFSIZ);
	OKButton = CreateWindow(
		"STATIC",
		buf,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		MsgRect.bottom + 15,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThredInstance,
		NULL);

	LoadString(ThredInstance, IDS_DISC, buf, HBUFSIZ);
	DiscardButton = CreateWindow(
		"STATIC",
		buf,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3 + 15,
		MsgRect.bottom + 15,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThredInstance,
		NULL);

	CancelButton = CreateWindow(
		"STATIC",
		StringTable[STR_CANCEL],
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		2 * ButtonWidthX3 + 25,
		MsgRect.bottom + 15,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThredInstance,
		NULL);
}

BOOL lastch() {
	if (interleaveSequenceIndex)
	{
		lastPoint.x = interleaveSequence[interleaveSequenceIndex - 1].x;
		lastPoint.y = interleaveSequence[interleaveSequenceIndex - 1].y;
		return 1;
	}
	else
		return 0;
}

unsigned getlast() {
	unsigned		ind;
	unsigned		tclos = 0;
	double			len, dx, dy, min = 1e99;

	if (SelectedForm->fillType) {
		lastch();
		for (ind = 0; ind < sides; ind++) {
			dx = lastPoint.x - currentFormVertices[ind].x;
			dy = lastPoint.y - currentFormVertices[ind].y;
			len = hypot(dx, dy);
			if (len < min) {
				min = len;
				tclos = ind;
			}
		}
		return tclos;
	}
	else
		return 0;
}

void flt2dub(fPOINT ipnt, dPOINT* p_opnt) {
	p_opnt->x = ipnt.x;
	p_opnt->y = ipnt.y;
}

void linrutb(unsigned strt) {
	double		tspac;
	unsigned	ind;
	dPOINT		tpnt;

	tspac = stitchSpace;
	SelectedPoint.x = currentFormVertices[strt].x;
	SelectedPoint.y = currentFormVertices[strt].y;
	for (ind = strt + 1; ind < sides; ind++) {
		flt2dub(currentFormVertices[ind], &tpnt);
		filinsb(tpnt);
	}
	flt2dub(currentFormVertices[0], &tpnt);
	filinsb(tpnt);
	stitchSpace = tspac;
}

void oclp(fPOINT* p_clp, unsigned p_nclp) {
	unsigned	ind;

	if (!chkMap(NOCLP))
	{
		for (ind = 0; ind < p_nclp; ind++) {
			ClipBuffer[ind].x = p_clp[ind].x;
			ClipBuffer[ind].y = p_clp[ind].y;
		}
		ClipRect.left = ClipRect.right = ClipBuffer[0].x;
		ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
		for (ind = 1; ind < (unsigned)p_nclp; ind++) {
			if (ClipBuffer[ind].x < ClipRect.left)
				ClipRect.left = ClipBuffer[ind].x;
			if (ClipBuffer[ind].x > ClipRect.right)
				ClipRect.right = ClipBuffer[ind].x;
			if (ClipBuffer[ind].y < ClipRect.bottom)
				ClipRect.bottom = ClipBuffer[ind].y;
			if (ClipBuffer[ind].y > ClipRect.top)
				ClipRect.top = ClipBuffer[ind].y;
		}
		ClipRectSize.cx = ClipRect.right - ClipRect.left;
		ClipRectSize.cy = ClipRect.top - ClipRect.bottom;
		ClipStitchCount = p_nclp;
	}
}

float getblen() {
#if	 __UseASM__
	float		fileLength;
	unsigned	tlen;

	tlen = (formList[closestFormToCursor].clipEntries << 16) | formList[closestFormToCursor].picoLength;

	_asm {
		mov		eax, tlen
		mov		fileLength, eax
	}
	return fileLength;
#else
	union {
		float		len;
		unsigned	tlen;
	} x;

	x.tlen = (formList[closestFormToCursor].clipEntries << 16) | formList[closestFormToCursor].picoLength;
	return x.len;
#endif
}

void savblen(float len) {
#if	 __UseASM__
	unsigned short l_nclp;
	unsigned short picoLength;

	_asm {
		mov		eax, len
		mov		picoLength, ax
		shr		eax, 16
		mov		l_nclp, ax
	}
	formList[closestFormToCursor].clipEntries = l_nclp;
	formList[closestFormToCursor].picoLength = picoLength;
#else
	union {
		unsigned int i;
		float f;
	} x;

	x.f = len;

	formList[closestFormToCursor].clipEntries = x.i >> 16;
	formList[closestFormToCursor].picoLength = x.i & 0xFFFF;
#endif
}

float getplen() {
	unsigned num = SelectedForm->picoLength;

	return((float)(num >> 8) + (num & 0xff) / 256);
}

void savplen(float len) {
	unsigned	num, fr;
	double		fnum, frf;

	if (len > 255)
		len = 255;
	frf = modf(len, &fnum);
	fr = (unsigned)floor(frf * 256);
	num = fnum;
	formList[closestFormToCursor].picoLength = (unsigned short)(num << 8) | fr;
}

void chkbrd() {
	float	tlen;

	fvars(closestFormToCursor);
	if (SelectedForm->edgeType) {
		switch (SelectedForm->edgeType&NEGUND) {
		case EDGELINE: // Line

			brdfil(SelectedForm->edgeStitchLen);
			break;

		case EDGEBEAN: // Bean

			bold(SelectedForm->edgeStitchLen);
			break;

		case EDGECLIP: // Clipboard

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpbrd(getlast());
			break;

		case EDGECLIPX: // Even Clipboard

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			duxclp();
			break;

		case EDGEANGSAT: // Angle Satin

			plen = SelectedForm->borderSize;
			sbrd();
			break;

		case EDGEPROPSAT: //Proportional Satin

			plen = SelectedForm->borderSize;
			pbrd(SelectedForm->edgeSpacing);
			break;

		case EDGEAPPL: // Applique

			sequenceIndex = 0;
			apbrd();
			ritapbrd();
			plen = SelectedForm->borderSize;
			sbrd();
			break;

		case EDGEBHOL: // BH Buttonhole

			plen = SelectedForm->borderSize * 2;
			tlen = buttonholeFillCornerLength;
			buttonholeFillCornerLength = getblen();
			satout(20);
			bhbrd(SelectedForm->edgeSpacing);
			buttonholeFillCornerLength = tlen;
			break;

		case EDGEPICOT: // Picot

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpic(0);
			break;

		case EDGEDOUBLE: // Double

			dubfn();
			break;

		case EDGELCHAIN: // Lin Chain

			setMap(LINCHN);
			chnfn();
			break;

		case EDGEOCHAIN: // Open Chain

			rstMap(LINCHN);
			chnfn();
			break;
		}
		ritbrd();
	}
}

void boldlin(unsigned strt, unsigned fin, double pd_Size) {
	dPOINT		dif, stp, pnt0, pnt1;
	double		len;
	unsigned	cnt;

	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	len = hypot(dif.x, dif.y);
	cnt = len / pd_Size;
	if (cnt) {
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		pnt0.x = currentFormVertices[strt].x;
		pnt0.y = currentFormVertices[strt].y;
		pnt1.x = pnt0.x + stp.x;
		pnt1.y = pnt0.y + stp.y;
		while (cnt) {
			oseq[sequenceIndex].x = pnt1.x;
			oseq[sequenceIndex++].y = pnt1.y;
			oseq[sequenceIndex].x = pnt0.x;
			oseq[sequenceIndex++].y = pnt0.y;
			oseq[sequenceIndex].x = pnt1.x;
			oseq[sequenceIndex++].y = pnt1.y;
			pnt0.x += stp.x;
			pnt0.y += stp.y;
			pnt1.x += stp.x;
			pnt1.y += stp.y;
			cnt--;
		}
		oseq[sequenceIndex].x = currentFormVertices[fin].x;
		oseq[sequenceIndex++].y = currentFormVertices[fin].y;
	}
	else {
		oseq[sequenceIndex].x = currentFormVertices[fin].x;
		oseq[sequenceIndex++].y = currentFormVertices[fin].y;
		oseq[sequenceIndex].x = currentFormVertices[strt].x;
		oseq[sequenceIndex++].y = currentFormVertices[strt].y;
		oseq[sequenceIndex].x = currentFormVertices[fin].x;
		oseq[sequenceIndex++].y = currentFormVertices[fin].y;
	}
}

void bold(double pd_Size) {
	unsigned		ind, ine = 0;
	unsigned short	nlin, tlin;
	double			len;

	nlin = tlin = getlast();
	sequenceIndex = 0;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	for (ind = 0; ind < (unsigned)sides - 1; ind++) {
		nlin = nxt(tlin);
		boldlin(tlin, nlin, pd_Size);
		tlin = nlin;
	}
	if (SelectedForm->type != FRMLINE) {
		nlin = nxt(tlin);
		boldlin(tlin, nlin, pd_Size);
	}
	for (ind = 0; ind < sequenceIndex - 1; ind++) {
		len = hypot(oseq[ind + 1].x - oseq[ind].x, oseq[ind + 1].y - oseq[ind].y);
		if (len > TINY) {
			oseq[ine].x = oseq[ind].x;
			oseq[ine++].y = oseq[ind].y;
		}
	}
	oseq[ine].x = currentFormVertices[nlin].x;
	oseq[ine++].y = currentFormVertices[nlin].y;
	sequenceIndex = ine;
}

void refilfn() {
	double		tspac;
	double		tsiz = UserStitchLength;
	float		tlen;

	rstMap(TXFIL);
	fvars(closestFormToCursor);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->underlayIndent = 0;
	savdo();
	fdelstch();
	setMap(WASREFIL);
	if (SelectedForm->fillSpacing < 0.5 && !isclp(closestFormToCursor))
		SelectedForm->fillSpacing = 0.5;
	if (SelectedForm->edgeSpacing < 0.5)
		SelectedForm->edgeSpacing = 0.5;
	UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
	if (!chkMap(WASDO))
		savdo();
	rstMap(WASDO);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK) && SelectedForm->type == FRMLINE&&SelectedForm->fillType != CONTF)
		SelectedForm->type = FRMFPOLY;
	interleaveSequenceIndex = interleaveSequenceIndex2 = 0;
	rstMap(ISUND);
	switch (SelectedForm->type) {
	case FRMLINE:

		plen = SelectedForm->borderSize;
		switch (SelectedForm->edgeType&NEGUND) {
		case EDGELINE:

			brdfil(SelectedForm->edgeStitchLen);
			ritbrd();
			break;

		case EDGEBEAN:

			bold(SelectedForm->edgeStitchLen);
			ritbrd();
			break;

		case EDGECLIP:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpout();
			sequenceIndex = 0;
			clpbrd(0);
			ritbrd();
			break;

		case EDGEANGSAT:

			rstMap(SAT1);
			slbrd();
			ritbrd();
			break;

		case EDGEPROPSAT:

			if (SelectedForm->sides > 2) {
				rstMap(SAT1);
				plbrd(SelectedForm->edgeSpacing);
				ritbrd();
			}
			break;

		case EDGEAPPL:

			lapbrd();
			ritapbrd();
			rstMap(SAT1);
			slbrd();
			ritbrd();
			break;

		case EDGEBHOL:

			plen = SelectedForm->borderSize * 2;
			tlen = buttonholeFillCornerLength;
			buttonholeFillCornerLength = getblen();
			satout(20);
			blbrd(SelectedForm->edgeSpacing);
			buttonholeFillCornerLength = tlen;
			ritbrd();
			break;

		case EDGEPICOT:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			sequenceIndex = 0;
			tlen = buttonholeFillCornerLength;
			buttonholeFillCornerLength = getplen();
			clpic(0);
			buttonholeFillCornerLength = tlen;
			ritbrd();
			break;

		case EDGEDOUBLE:

			dubfn();
			ritbrd();
			break;

		case EDGELCHAIN:

			setMap(LINCHN);
			chnfn();
			ritbrd();
			break;

		case EDGEOCHAIN:

			rstMap(LINCHN);
			chnfn();
			ritbrd();
			break;

		case EDGECLIPX:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			duxclp();
			ritbrd();
			break;
		}
		if (SelectedForm->fillType == CONTF&&SelectedForm->attribute&FRECONT) {
			contf();
			ritfil();
		}
		break;

	case FRMFPOLY:

		chkcwlk();
		chkwlk();
		chkund();
		rstMap(ISUND);
		if (SelectedForm->fillType) {
			tspac = stitchSpace;
			stitchSpace = SelectedForm->fillSpacing;
			switch ((unsigned)SelectedForm->fillType) {
			case VRTF:

				fnvrt();
				workingFormVertices = SelectedForm->vertices;
				break;

			case HORF:

				fnhor();
				workingFormVertices = angledForm.vertices;
				break;

			case ANGF:

				RotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
				filang();
				workingFormVertices = angledForm.vertices;
				break;

			case VCLPF:

				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				setangf(0);
				fvars(closestFormToCursor);
				clpcon();
				goto skpfil;

			case HCLPF:

				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				horclpfn();
				goto skpfil;

			case ANGCLPF:

				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				rstMap(ISUND);
				angclpfn();
				goto skpfil;

			case TXVRTF:

				setangf(0);
				setxt();
				clpcon();
				goto skpfil;

			case TXHORF:

				setxt();
				horclpfn();
				goto skpfil;

			case TXANGF:

				setxt();
				rstMap(ISUND);
				angclpfn();
				goto skpfil;
			}
			lcon();
			bakseq();
			if (SelectedForm->fillType != VRTF&&SelectedForm->fillType != TXVRTF) {
				RotationAngle = -RotationAngle;
				rotbak();
			}
		skpfil:;
			ritfil();
			stitchSpace = tspac;
		}
		chkbrd();
		break;

	case SAT:

		chkcwlk();
		chkwlk();
		chkund();
		rstMap(ISUND);
		switch (SelectedForm->fillType) {
		case SATF:

			tspac = stitchSpace;
			stitchSpace = SelectedForm->fillSpacing;
			UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
			satfil();
			stitchSpace = tspac;
			ritfil();
			break;

		case CLPF:

			oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
			fmclp();
			ritfil();
			break;

		case FTHF:

			if (rstMap(FLPBLND))
				SelectedForm->extendedAttribute ^= AT_FTHBLND;
			setMap(CNV2FTH);
			fthrfn();
			fritfil();
			break;
		}
		chkbrd();
	}
	UserStitchLength = tsiz;
	intlv();
	ritot(PCSHeader.stitchCount);
	setfchk();
}

void refil() {
	unsigned trg, ind, at;

	if (!chku(WRNOF))
	{
		trg = (closestFormToCursor << FRMSHFT) | USMSK;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			at = StitchBuffer[ind].attribute;
			if (!(at&NOTFRM) && (at&(USMSK | FRMSK)) == trg) {
				if (FormDataSheet)
					setMap(WASFRMFRM);
				undat();
				tabmsg(IDS_REFIL);
				setMap(MOVMSG);
				okcan();
				return;
			}
		}
	}
	refilfn();
}

void setfpnt() {
	POINT		stchpx;
	fPOINT*		l_dpnt;

	unfrm();
	fvars(closestFormToCursor);
	l_dpnt = &SelectedForm->vertices[0];
	stchpx.x = Msg.pt.x - StitchWindowOrigin.x;
	stchpx.y = Msg.pt.y - StitchWindowOrigin.y;
	rats();
	px2stchf(stchpx, &l_dpnt[closestVertexToCursor]);
	frmout(closestFormToCursor);
	refil();
	setMap(WASFPNT);
	rstMap(SELBOX);
	setMap(FRMPSEL);
	ritfcor(&currentFormVertices[closestVertexToCursor]);
	setMap(RESTCH);
}

unsigned short nxt(unsigned short ind) {
	ind++;
	if (ind > (unsigned)sides - 1)
		ind = 0;
	return ind;
}

unsigned short prv(unsigned ind) {
	if (ind)
		ind--;
	else
		ind = sides - 1;
	return ind;
}

unsigned proj(dPOINT pnt, double p_slop, fPOINT pnt0, fPOINT pnt1, dPOINT* ipnt) {
	dPOINT	difl;
	double	slopl, con, conl, xmin, xmax, ymin, ymax, tdub;

	difl.x = pnt1.x - pnt0.x;
	difl.y = pnt1.y - pnt0.y;
	if (difl.x) {
		slopl = difl.y / difl.x;
		conl = pnt0.y - slopl*pnt0.x;
		con = pnt.y - p_slop*pnt.x;
		ipnt->x = (conl - con) / (p_slop - slopl);
		ipnt->y = ipnt->x*p_slop + con;
	}
	else {
		ipnt->x = pnt0.x;
		con = pnt.y - p_slop*pnt.x;
		ipnt->y = ipnt->x*p_slop + con;
	}
	xmin = pnt0.x;
	xmax = pnt1.x;
	if (xmin > xmax) {
		tdub = xmin;
		xmin = xmax;
		xmax = tdub;
	}
	if (difl.y) {
		ymin = pnt0.y;
		ymax = pnt1.y;
		if (ymin > ymax) {
			tdub = ymin;
			ymin = ymax;
			ymax = tdub;
		}
		if (ipnt->x<xmin || ipnt->x>xmax || ipnt->y<ymin || ipnt->y>ymax)
			return 0;
		else
			return 1;
	}
	else {
		if (ipnt->x<xmin || ipnt->x>xmax)
			return 0;
		else
			return 1;
	}
}

unsigned projv(double pntx, fPOINT pnt0, fPOINT pnt1, dPOINT* ipnt) {
	double tdub, l_slop, dx;

	ipnt->x = pntx;
	dx = pnt1.x - pnt0.x;
	if (dx) {
		l_slop = (pnt1.y - pnt0.y) / dx;
		ipnt->y = (pntx - pnt0.x)*l_slop + pnt0.y;
		if (pnt0.x > pnt1.x) {
			tdub = pnt0.x;
			pnt0.x = pnt1.x;
			pnt1.x = tdub;
		}
		if (pntx<pnt0.x || pntx>pnt1.x)
			return 0;
		else
			return 1;
	}
	else
		return 0;
}

unsigned projh(double pnty, fPOINT pnt0, fPOINT pnt1, dPOINT* ipnt) {
	double tdub, l_slop, dx, dy;

	ipnt->y = pnty;
	dx = pnt1.x - pnt0.x;
	if (dx) {
		dy = pnt1.y - pnt0.y;
		if (dy) {
			l_slop = dy / dx;
			ipnt->x = (pnty - pnt0.y) / l_slop + pnt0.x;
		}
		else
			return 0;
	}
	else
		ipnt->x = pnt0.x;
	if (pnt0.y > pnt1.y) {
		tdub = pnt0.y;
		pnt0.y = pnt1.y;
		pnt1.y = tdub;
	}
	if (pnty<pnt0.y || pnty>pnt1.y)
		return 0;
	else
		return 1;
}

void filang() {
	unsigned	ind;

	frmcpy(&angledForm, &formList[closestFormToCursor]);
	RotationCenter.x = (double)(angledForm.rectangle.right - angledForm.rectangle.left) / 2 + angledForm.rectangle.left;
	RotationCenter.y = (double)(angledForm.rectangle.top - angledForm.rectangle.bottom) / 2 + angledForm.rectangle.bottom;
	angledForm.vertices = angledFormVertices;
	for (ind = 0; ind < angledForm.sides; ind++) {
		angledForm.vertices[ind].x = SelectedForm->vertices[ind].x;
		angledForm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&angledForm.vertices[ind]);
	}
	SelectedForm = &angledForm;
	fnvrt();
	SelectedForm = &formList[closestFormToCursor];
}

void makpoli() {
	if (SelectedForm->type == SAT) {
		if (SelectedForm->satinGuideCount)
			delsac(closestFormToCursor);
	}
	SelectedForm->type = FRMFPOLY;
}

void filinu(float pntx, float pnty) {
	unsigned	cnt;
	double		len;
	dPOINT		ipnt, stp, dif;

	ipnt.x = SelectedPoint.x;
	ipnt.y = SelectedPoint.y;
	dif.x = pntx - SelectedPoint.x;
	dif.y = pnty - SelectedPoint.y;
	len = hypot(dif.x, dif.y);
	cnt = len / UserStitchLength;
	if (chkmax(cnt, sequenceIndex) || cnt + sequenceIndex > MAXSEQ - 3)
		return;
	if (cnt) {
		if (chkMap(FILDIR))
			cnt++;
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		while (cnt > 0) {
			ipnt.x += stp.x;
			ipnt.y += stp.y;
			oseq[sequenceIndex].x = ipnt.x;
			oseq[sequenceIndex++].y = ipnt.y;
			cnt--;
		}
	}
	else {
		oseq[sequenceIndex].x = pntx;
		oseq[sequenceIndex++].y = pnty;
	}
	SelectedPoint.x = pntx;
	SelectedPoint.y = pnty;
}

void filin(dPOINT pnt) {
	dPOINT dif, stp, ipnt;
	double len;
	int cnt;

	dif.x = pnt.x - SelectedPoint.x;
	dif.y = pnt.y - SelectedPoint.y;
	len = hypot(dif.x, dif.y);
	ipnt.x = SelectedPoint.x;
	ipnt.y = SelectedPoint.y;
	cnt = len / UserStitchLength;
	if (chkmax(cnt, sequenceIndex) || (cnt + sequenceIndex) > MAXSEQ - 3)
		return;
	if (cnt) {
		if (chkMap(FILDIR))
			cnt++;
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		while (cnt > 0) {
			ipnt.x += stp.x;
			ipnt.y += stp.y;
			oseq[sequenceIndex].x = ipnt.x;
			oseq[sequenceIndex++].y = ipnt.y;
			cnt--;
		}
	}
	else {
		oseq[sequenceIndex].x = pnt.x;
		oseq[sequenceIndex++].y = pnt.y;
	}
	SelectedPoint.x = pnt.x;
	SelectedPoint.y = pnt.y;
}

unsigned short isclos(SMALPNTL* pnt0, SMALPNTL* pnt1) {
	float		lo0, hi0, lo1, hi1;

	hi0 = pnt0[1].y + gapToClosestRegion;
	lo0 = pnt0[0].y - gapToClosestRegion;
	hi1 = pnt1[1].y + gapToClosestRegion;
	lo1 = pnt1[0].y - gapToClosestRegion;
	if (hi0 < lo1)
		return 0;
	if (hi1 < lo0)
		return 0;
	return 1;
}

BOOL lnclos(unsigned gp0, unsigned ln0, unsigned gp1, unsigned ln1) {
	unsigned	ind0, ind1, cnt0, cnt1;
	SMALPNTL*	pnt0;
	SMALPNTL*	pnt1;

	if (gp1 > groupIndexCount - 2)
		return 0;
	if (!gp0)
		return 0;
	cnt0 = (groupIndexSequence[gp0 + 1] - groupIndexSequence[gp0]) >> 1;
	ind0 = 0;
	pnt0 = &lineEndpoints[groupIndexSequence[gp0]];
	while (cnt0&&pnt0[ind0].line != ln0) {
		cnt0--;
		ind0 += 2;
	}
	if (cnt0) {
		cnt1 = (groupIndexSequence[gp1 + 1] - groupIndexSequence[gp1]) >> 1;
		ind1 = 0;
		pnt1 = &lineEndpoints[groupIndexSequence[gp1]];
		while (cnt1&&pnt1[ind1].line != ln1) {
			cnt1--;
			ind1 += 2;
		}
		if (cnt1) {
			if (isclos(&pnt0[ind0], &pnt1[ind1]))
				return 1;
			else
				return 0;
		}
	}
	return 0;
}

unsigned short regclos(unsigned rg0, unsigned rg1) {
	SMALPNTL*	pnt0s;
	SMALPNTL*	pnt0e;
	SMALPNTL*	pnt1s;
	SMALPNTL*	pnt1e;
	unsigned	grp0s;
	unsigned	grp0e;
	unsigned	grp1s;
	unsigned	grp1e;
	unsigned	grps;
	unsigned	grpe;
	unsigned	l_lins, line;
	unsigned	prlin, polin;

	pnt0s = &*ptrSortedLines[regionsList[rg0].start];
	pnt1s = &*ptrSortedLines[regionsList[rg1].start];
	grp1s = pnt1s->group;
	grp0s = pnt0s->group;
	if (grp0s > grp1s) {
		grps = grp0s;
		l_lins = pnt0s->line;
		prlin = pnt1s->line;
	}
	else {
		grps = grp1s;
		l_lins = pnt1s->line;
		prlin = pnt0s->line;
	}
	if (grps&&lnclos(grps - 1, prlin, grps, l_lins)) {
		nextGroup = grps;
		return 1;
	}
	else {
		pnt0e = &*ptrSortedLines[regionsList[rg0].end];
		pnt1e = &*ptrSortedLines[regionsList[rg1].end];
		grp1e = pnt1e->group;
		grp0e = pnt0e->group;
		if (grp0e < grp1e) {
			grpe = grp0e;
			line = pnt0e->line;
			polin = pnt1e->line;
		}
		else {
			grpe = grp1e;
			line = pnt1e->line;
			polin = pnt0e->line;
		}
		if (lnclos(grpe, line, grpe + 1, polin)) {
			nextGroup = grpe;
			return 1;
		}
	}
	if (((grp0s > grp1s) ? (grp0s - grp1s) : (grp1s - grp0s)) < 2) {
		if (isclos(pnt0s, pnt1s)) {
			nextGroup = grp0s;
			return 1;
		}
	}
	if (((grp0s > grp1e) ? (grp0s - grp1e) : (grp1e - grp0s)) < 2) {
		if (isclos(pnt0s, pnt1e)) {
			nextGroup = grp0s;
			return 1;
		}
	}
	if (((grp0e > grp1s) ? (grp0e - grp1s) : (grp1s - grp0e)) < 2) {
		if (isclos(pnt0e, pnt1s)) {
			nextGroup = grp0e;
			return 1;
		}
	}
	if (((grp0e > grp1e) ? (grp0e - grp1e) : (grp1e - grp0e)) < 2) {
		if (isclos(pnt0e, pnt1e)) {
			nextGroup = grp0e;
			return 1;
		}
	}
	return 0;
}

BOOL unvis() {
	for (visitedIndex = 0; visitedIndex < regionCount; visitedIndex++) {
		if (!visitedRegions[visitedIndex])
			return 1;
	}
	return 0;
}

unsigned setseq(unsigned bpnt) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, sequenceFlagBitmap
		mov		ecx, bpnt
		bts[ebx], ecx
		jnc		short setseqx
		inc		eax
		setseqx :
	}
#else
	return _bittestandset((long *)sequenceFlagBitmap, bpnt);
#endif
}

void rspnt(float fx, float fy) {
	bseq[outputIndex].x = fx;
	bseq[outputIndex].y = fy;
	bseq[outputIndex++].attribute = 0;
}

void dunseq(unsigned strt, unsigned fin) {
	SMALPNTL*	l_lin0;
	SMALPNTL*	l_lin1;
	unsigned	ind;
	double		dy, miny;

	miny = 1e30;
	for (ind = strt; ind <= fin; ind++) {
		l_lin0 = &*ptrSortedLines[ind];
		dy = l_lin0[1].y - l_lin0->y;
		if (dy < miny)
			miny = dy;
	}
	miny /= 2;
	l_lin0 = &*ptrSortedLines[strt];
	l_lin1 = &*ptrSortedLines[fin];
	if (miny == 1e30 / 2)
		miny = 0;
	rspnt(l_lin0->x, l_lin0->y + miny);
	rspnt(l_lin1->x, l_lin1->y + miny);
	lastGroup = l_lin1->group;
}

void movseq(unsigned ind) {
	SMALPNTL* l_lin;

	l_lin = &*ptrSortedLines[ind];
	bseq[outputIndex].attribute = SEQBOT;
	bseq[outputIndex].x = l_lin->x;
	bseq[outputIndex].y = l_lin->y;
	outputIndex++;
	l_lin++;
	bseq[outputIndex].attribute = SEQTOP;
	bseq[outputIndex].x = l_lin->x;
	bseq[outputIndex].y = l_lin->y;
	outputIndex++;
}

void duseq2(unsigned ind) {
	sequenceLines = &*ptrSortedLines[ind];
	rspnt((sequenceLines[1].x - sequenceLines[0].x) / 2 + sequenceLines[0].x, (sequenceLines[1].y - sequenceLines[0].y) / 2 + sequenceLines[0].y);
}

void duseq1() {
	rspnt((sequenceLines[1].x - sequenceLines[0].x) / 2 + sequenceLines[0].x, (sequenceLines[1].y - sequenceLines[0].y) / 2 + sequenceLines[0].y);
}

void duseq(unsigned strt, unsigned fin) {
	unsigned	ind, topbak;

	sequenceLines = nullptr;
	rstMap(SEQDUN);
	topbak = ptrSortedLines[strt][1].line;
	if (strt > fin) {
		for (ind = strt; (int)ind >= (int)fin; ind--) {
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq2(ind);
				else {
					if (topbak != ptrSortedLines[ind][1].line) {
						if (ind)
							duseq2(ind + 1);
						duseq2(ind);
						topbak = sequenceLines[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN))
					duseq2(ind + 1);
				sequenceLines = &*ptrSortedLines[ind];
				movseq(ind);
			}
		}
		if (rstMap(SEQDUN))
			duseq2(ind + 1);
		if (sequenceLines != nullptr) { lastGroup = sequenceLines->group; }
	}
	else {
		for (ind = strt; ind <= fin; ind++) {
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq2(ind);
				else {
					if (topbak != ptrSortedLines[ind][1].line) {
						if (ind)
							duseq2(ind - 1);
						duseq2(ind);
						topbak = sequenceLines[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN)) {
					if (ind)
						duseq2(ind - 1);
				}
				sequenceLines = &*ptrSortedLines[ind];
				movseq(ind);
			}
		}
		if (rstMap(SEQDUN)) {
			if (ind)
				duseq2(ind - 1);
		}
		if (sequenceLines != nullptr) { lastGroup = sequenceLines->group; }
	}
}

void brkseq(unsigned strt, unsigned fin) {
	//SMALPNTL* line=0;
	unsigned	ind, bgrp = 0;

	rstMap(SEQDUN);
	if (strt > fin) {
		bgrp = ptrSortedLines[strt]->group + 1;
		for (ind = strt; (int)ind >= (int)fin; ind--) {
			bgrp--;
			if (ptrSortedLines[ind]->group != bgrp) {
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				sequenceLines = &*ptrSortedLines[ind];
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				bgrp = sequenceLines[0].group;
			}
			else
				sequenceLines = &*ptrSortedLines[ind];
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(ind);
		}
		lastGroup = sequenceLines->group;
	}
	else {
		bgrp = ptrSortedLines[strt]->group - 1;
		for (ind = strt; ind <= fin; ind++) {
			bgrp++;
			if (ptrSortedLines[ind]->group != bgrp) {
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				sequenceLines = &*ptrSortedLines[ind];
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				bgrp = sequenceLines[0].group;
			}
			else
				sequenceLines = &*ptrSortedLines[ind];
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(ind);
		}
		lastGroup = sequenceLines->group;
	}
	if (rstMap(SEQDUN))
		duseq1();
}

void brkdun(unsigned strt, unsigned fin) {
	rspnt(ptrSortedLines[strt]->x, ptrSortedLines[strt]->y);
	rspnt(ptrSortedLines[fin]->x, ptrSortedLines[fin]->y);
	rspnt(workingFormVertices[ptrSortedLines[strt]->line].x, workingFormVertices[ptrSortedLines[strt]->line].y);
	setMap(BRKFIX);
}

void durgn(unsigned pthi) {
	unsigned	dun, gdif, mindif = 0, ind, fdif, bdif;
	unsigned	seql, seqn;
	unsigned	seqs, seqe;
	unsigned	grpn, grps, grpe;
	unsigned	rgind;
	SMALPNTL*	pnts;
	SMALPNTL*	pnte;
	double		len, minlen;
	BSEQPNT*	bpnt;

	rgind = sequencePath[pthi].vrt;
	currentRegion = &regionsList[rgind];
	grpn = sequencePath[pthi].grpn;
	seqs = currentRegion->start;
	seqe = currentRegion->end;
	if (sequencePath[pthi].skp || rstMap(BRKFIX)) {
		if (bseq[outputIndex - 1].attribute != SEQBOT)
			rspnt(bseq[outputIndex - 2].x, bseq[outputIndex - 2].y);
		pnts = &*ptrSortedLines[rgind];
		dun = ptrSortedLines[seqs]->line;
		bpnt = &bseq[outputIndex - 1];
		minlen = 1e99;
		for (ind = 0; ind < sides; ind++) {
			len = hypot(bpnt->x - workingFormVertices[ind].x, bpnt->y - workingFormVertices[ind].y);
			if (len < minlen) {
				minlen = len;
				mindif = ind;
			}
		}
		if (minlen)
			rspnt(workingFormVertices[mindif].x, workingFormVertices[mindif].y);
		fdif = (sides + dun - mindif) % sides;
		bdif = (sides - dun + mindif) % sides;
		if (fdif < bdif) {
			ind = nxt(mindif);
			while (ind != dun) {
				rspnt(workingFormVertices[ind].x, workingFormVertices[ind].y);
				ind = nxt(ind);
			}
			rspnt(workingFormVertices[ind].x, workingFormVertices[ind].y);
		}
		else {
			ind = prv(mindif);
			while (ind != dun) {
				rspnt(workingFormVertices[ind].x, workingFormVertices[ind].y);
				ind = prv(ind);
			}
			rspnt(workingFormVertices[ind].x, workingFormVertices[ind].y);
		}
	}
	if (visitedRegions[rgind])
		dun = 1;
	else {
		dun = 0;
		visitedRegions[rgind]++;
	}
	pnts = &*ptrSortedLines[currentRegion->start];
	pnte = &*ptrSortedLines[currentRegion->end];
	grps = pnts->group;
	grpe = pnte->group;
	if (grpe != grps)
		seql = (double)(lastGroup - grps) / (grpe - grps)*(seqe - seqs) + seqs;
	else
		seql = 0;
	if (seql > sortedLineIndex)
		seql = 0;
	len = (double)(grpe - grps)*(seqe - seqs);
	if (len)
		seqn = (double)(grpn - grps) / len + seqs;
	else
		seqn = seqe;
	if (seql < seqs)
		seql = seqs;
	if (seql > seqe)
		seql = seqe;
	if (seqn < seqs)
		seqn = seqs;
	if (seqn > seqe)
		seqn = seqe;
	if (ptrSortedLines[seql]->group != lastGroup) {
		if (seql < seqe&&ptrSortedLines[seql + 1]->group == lastGroup)
			seql++;
		else {
			if (seql > seqs&&ptrSortedLines[seql - 1]->group == lastGroup)
				seql--;
			else {
				mindif = 0xffffffff;
				for (ind = seqs; ind <= seqe; ind++) {
					gdif = ((ptrSortedLines[ind]->group > lastGroup) ? (ptrSortedLines[ind]->group - lastGroup) : (lastGroup - ptrSortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seql = ind;
					}
				}
			}
		}
	}
	if (ptrSortedLines[seqn]->group != grpn) {
		if (seqn < seqe&&ptrSortedLines[seqn + 1]->group == grpn)
			seqn++;
		else {
			if (seqn > seqs&&ptrSortedLines[seqn - 1]->group == grpn)
				seqn--;
			else {
				mindif = 0xffffffff;
				for (ind = seqs; ind <= seqe; ind++) {
					gdif = ((ptrSortedLines[ind]->group > grpn) ? (ptrSortedLines[ind]->group - grpn) : (grpn - ptrSortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seqn = ind;
					}
				}
			}
		}
	}
	if (currentRegion->cntbrk) {
		if (dun) {
			brkdun(seql, seqn);
		}
		else {
			if (lastGroup >= grpe) {
				brkseq(seqe, seqs);
				if (pthi < sequencePathIndex - 1 && seqe != seqn)
					brkseq(seqs, seqn);
			}
			else {
				if (grps <= grpn) {
					if (seql != seqs)
						brkseq(seql, seqs);
					brkseq(seqs, seqe);
					if (pthi < sequencePathIndex - 1 && seqe != seqn)
						brkseq(seqe, seqn);
				}
				else {
					if (seql != seqe)
						brkseq(seql, seqe);
					brkseq(seqe, seqs);
					if (pthi < sequencePathIndex - 1 && seqs != seqn)
						brkseq(seqs, seqn);
				}
			}
		}
	}
	else {
		if (dun)
			dunseq(seql, seqn);
		else {
			if (lastGroup >= grpe) {
				duseq(seqe, seqs);
				duseq(seqs, seqn);
			}
			else {
				if (grps <= grpn) {
					if (seql != seqs)
						duseq(seql, seqs);
					duseq(seqs, seqe);
					if (pthi < sequencePathIndex - 1 && seqe != seqn)
						duseq(seqe, seqn);
				}
				else {
					if (seql != seqe)
						duseq(seql, seqe);
					duseq(seqe, seqs);
					if (pthi < sequencePathIndex - 1 && seqs != seqn)
						duseq(seqs, seqn);
				}
			}
		}
	}
}

unsigned notdun(unsigned lvl) {
	unsigned	ind;
	int			tpiv, pivot = lvl - 1;

	regionPath = &tmpath[sequencePathIndex];
	regionPath[0].pcon = mapIndexSequence[doneRegion];
	regionPath[0].cnt = mapIndexSequence[doneRegion + 1] - regionPath[0].pcon;
	for (ind = 1; ind < lvl; ind++) {
		regionPath[ind].pcon = mapIndexSequence[pathMap[regionPath[ind - 1].pcon].vrt];
		regionPath[ind].cnt = mapIndexSequence[pathMap[regionPath[ind - 1].pcon].vrt + 1] - regionPath[ind].pcon;
	}
	while (visitedRegions[pathMap[regionPath[pivot].pcon].vrt] && pivot >= 0) {
		if (--regionPath[pivot].cnt > 0)
			regionPath[pivot].pcon++;
		else {
			tpiv = pivot;
			do {
				tpiv--;
				if (tpiv < 0)
					return 1;
				regionPath[tpiv].cnt--;
				regionPath[tpiv].pcon++;
			} while (!regionPath[tpiv].cnt);
			if (tpiv < 0)
				return 1;
			tpiv++;
			while (tpiv <= pivot) {
				if (tpiv) {
					regionPath[tpiv].pcon = mapIndexSequence[pathMap[regionPath[tpiv - 1].pcon].vrt];
					regionPath[tpiv].cnt = mapIndexSequence[pathMap[regionPath[tpiv - 1].pcon].vrt + 1] - regionPath[tpiv].pcon;
				}
				else {
					if (--regionPath[0].cnt)
						regionPath[0].pcon++;
					else
						return 1;
				}
				tpiv++;
			}
		}
	}
	return 0;
}

double reglen(unsigned reg) {
	double		len, minlen = 1e99;
	unsigned	ind, ine;
	SMALPNTL*	pnts[4];

	pnts[0] = ptrSortedLines[regionsList[reg].start];
	pnts[1] = &ptrSortedLines[regionsList[reg].start][1];
	pnts[2] = ptrSortedLines[regionsList[reg].end];
	pnts[3] = &ptrSortedLines[regionsList[reg].end][1];
	for (ind = 0; ind < 4; ind++) {
		for (ine = 0; ine < 4; ine++) {
			len = hypot(lastRegionCorners[ind].x - pnts[ine]->x, lastRegionCorners[ind].y - pnts[ine]->y);
			if (len < minlen)
				minlen = len;
		}
	}
	return minlen;
}

void nxtrgn() {
	unsigned	ind, newRegion;
	SMALPNTL*	tpnt;
	double		len, minlen = 1e99;
	unsigned	pathLength;					//length of the path to the region

	pathLength = 1;
	while (notdun(pathLength)) {
		pathLength++;
		if (pathLength > 8) {
			tpnt = &*ptrSortedLines[regionsList[doneRegion].start];
			lastRegionCorners[0].x = tpnt[0].x;
			lastRegionCorners[0].y = tpnt[0].y;
			lastRegionCorners[1].x = tpnt[1].x;
			lastRegionCorners[1].y = tpnt[1].y;
			tpnt = &*ptrSortedLines[regionsList[doneRegion].end];
			lastRegionCorners[2].x = tpnt[0].x;
			lastRegionCorners[2].y = tpnt[0].y;
			lastRegionCorners[3].x = tpnt[1].x;
			lastRegionCorners[3].y = tpnt[1].y;
			newRegion = 0;
			for (ind = 0; ind < regionCount; ind++) {
				if (!visitedRegions[ind]) {
					len = reglen(ind);
					if (len < minlen) {
						minlen = len;
						newRegion = ind;
					}
				}
			}
			tmpath[sequencePathIndex].skp = 1;
			for (ind = 0; ind < pathMapIndex; ind++) {
				if (pathMap[ind].vrt == newRegion) {
					tmpath[sequencePathIndex++].pcon = ind;
					visitedRegions[newRegion] = 1;
					doneRegion = newRegion;
					return;
				}
			}
			tmpath[sequencePathIndex].cnt = visitedIndex;
			tmpath[sequencePathIndex++].pcon = 0xffffffff;
			visitedRegions[visitedIndex] = 1;
			doneRegion = visitedIndex;
			return;
		}
	}
	for (ind = 0; ind < pathLength; ind++) {
		tmpath[sequencePathIndex].skp = 0;
		tmpath[sequencePathIndex++].pcon = regionPath[ind].pcon;
		visitedRegions[pathMap[regionPath[ind].pcon].vrt] = 1;
	}
	doneRegion = pathMap[regionPath[ind - 1].pcon].vrt;
}

#if	 __UseASM__
SMALPNTL* srtref(const void* arg) {
	_asm {
		mov		eax, arg
		mov		eax, [eax]
	}
}
#endif

int sqcomp(const void *arg1, const void *arg2) {
	SMALPNTL* pnt0;
	SMALPNTL* pnt1;

#if	 __UseASM__
	pnt0 = srtref(arg1);
	pnt1 = srtref(arg2);
#else
	pnt0 = (SMALPNTL*)arg1;
	pnt1 = (SMALPNTL*)arg2;
#endif

	if (pnt0->line == pnt1->line) {
		if (pnt0->group == pnt1->group) {
			if (pnt0->y == pnt1->y)return 0;
			else {
				if (pnt0->y > pnt1->y)
					return 1;
				else
					return -1;
			}
		}
		else {
			if (pnt0->group > pnt1->group)
				return 1;
			else
				return -1;
		}
	}
	else {
		if (pnt0->line > pnt1->line)
			return 1;
		else
			return -1;
	}
}

void nxtseq(unsigned pthi) {
	unsigned nxtvrt, ind;

	ind = mapIndexSequence[sequencePath[pthi].vrt];
	nxtvrt = sequencePath[pthi + 1].vrt;
	while (ind < mapIndexSequence[sequencePath[pthi].vrt + 1] && pathMap[ind].vrt != nxtvrt) {
		ind++;
	}
	sequencePath[mPathIndex++].grpn = pathMap[ind].grpn;
}

#define BUGSEQ 0

void lcon() {
	unsigned		ind, ine, l_bLine, cnt, sgrp;
	REGION*			trgns;
	short			tcon;
	RCON*			pcon;
	RCON*			tmap;
	SMALPNTL*		tpnt;
	unsigned*		tsrgns;
	unsigned		sind;

#if BUGSEQ

	unsigned		bugcol;
#endif

	if (stitchLineCount) {
		ptrSortedLines = new SMALPNTL*[stitchLineCount >> 1];
		sortedLineIndex = 0;
		for (ind = 0; ind < stitchLineCount; ind += 2)
			ptrSortedLines[sortedLineIndex++] = &lineEndpoints[ind];
		qsort((void*)ptrSortedLines, sortedLineIndex, 4, sqcomp);
		regionCount = 0;
		trgns = (REGION*)oseq;
		trgns[0].start = 0;
		l_bLine = ptrSortedLines[0]->line;
		for (ind = 0; ind < sortedLineIndex; ind++) {
			if (l_bLine != ptrSortedLines[ind]->line) {
				trgns[regionCount++].end = ind - 1;
				trgns[regionCount].start = ind;
				l_bLine = ptrSortedLines[ind]->line;
			}
		}
		trgns[regionCount++].end = ind - 1;
		regionsList = new REGION[regionCount];
		visitedRegions = new char[regionCount];
		for (ind = 0; ind < regionCount; ind++) {
			regionsList[ind].start = trgns[ind].start;
			regionsList[ind].end = trgns[ind].end;
			visitedRegions[ind] = 0;
			regionsList[ind].cntbrk = 0;
		}
		tsrgns = (unsigned*)oseq;
		sind = 0;
		for (ind = 0; ind < regionCount; ind++) {
			cnt = 0;
			if ((regionsList[ind].end - regionsList[ind].start) > 1) {
				sgrp = ptrSortedLines[regionsList[ind].start]->group;
				for (ine = regionsList[ind].start + 1; ine <= regionsList[ind].end; ine++) {
					sgrp++;
					if (ptrSortedLines[ine]->group != sgrp) {
						if (!cnt)
							regionsList[ind].brk = sind;
						cnt++;
						sgrp = ptrSortedLines[ine]->group;
						tsrgns[sind++] = ine;
					}
				}
			}
			regionsList[ind].cntbrk = cnt;
		}
		tmap = (RCON*)bseq;
		mapIndexSequence = new unsigned[regionCount + 1];

#if BUGSEQ
		bugcol = 0; sequenceIndex = 0;
		for (ind = 0; ind < regionCount; ind++) {
			for (ine = regionsList[ind].start; ine <= regionsList[ind].end; ine++) {
				tpnt = &*ptrSortedLines[ine];
				StitchBuffer[sequenceIndex].attribute = bugcol;
				StitchBuffer[sequenceIndex].x = tpnt[0].x;
				StitchBuffer[sequenceIndex++].y = tpnt[0].y;
				StitchBuffer[sequenceIndex].attribute = bugcol;
				StitchBuffer[sequenceIndex].x = tpnt[1].x;
				StitchBuffer[sequenceIndex++].y = tpnt[1].y;
			}
			bugcol++;
			bugcol &= 0xf;
		}
		PCSHeader.stitchCount = sequenceIndex;
		goto seqskip;
#endif
		outputIndex = 0;
		if (regionCount > 1) {
			ine = 0; pathMapIndex = 0;
			for (ind = 0; ind < regionCount; ind++) {
				pcon = &tmap[pathMapIndex];
				mapIndexSequence[ind] = pathMapIndex;
				cnt = 0; gapToClosestRegion = 0;
				for (ine = 0; ine < regionCount; ine++) {
					if (ind != ine) {
						tcon = regclos(ind, ine);
						if (tcon) {
							tmap[pathMapIndex].con = tcon;
							tmap[pathMapIndex].grpn = nextGroup;
							tmap[pathMapIndex++].vrt = ine;
							cnt++;
						}
					}
				}
				while (!cnt) {
					gapToClosestRegion += stitchSpace;
					cnt = 0;
					for (ine = 0; ine < regionCount; ine++) {
						if (ind != ine) {
							tcon = regclos(ind, ine);
							if (tcon) {
								tmap[pathMapIndex].con = tcon;
								tmap[pathMapIndex].grpn = nextGroup;
								tmap[pathMapIndex++].vrt = ine;
								cnt++;
							}
						}
					}
				}
			}
			mapIndexSequence[ind] = pathMapIndex;
			pathMap = new RCON[pathMapIndex + 1];
			for (ind = 0; ind < pathMapIndex; ind++) {
				pathMap[ind].con = tmap[ind].con;
				pathMap[ind].vrt = tmap[ind].vrt;
				pathMap[ind].grpn = tmap[ind].grpn;
			}
			//find the leftmost region
			sgrp = 0xffffffff; ine = 0;
			for (ind = 0; ind < regionCount; ind++) {
				tpnt = &*ptrSortedLines[regionsList[ind].start];
				if (tpnt->group < sgrp) {
					sgrp = tpnt->group;
					ine = ind;
				}
			}
			outputIndex = 0;
			tmpath = (RGSEQ*)oseq;
			//find the leftmost region in pathMap
			sequencePathIndex = 1;
			for (ind = 0; ind < pathMapIndex; ind++) {
				if (pathMap[ind].vrt == ine)
					goto lconskip;
			}
			pathMap[pathMapIndex].vrt = ine;
			pathMap[pathMapIndex].grpn = 0;
			ind = pathMapIndex;
		lconskip:;
			//set the first entry in the temporary path to the leftmost region
			tmpath[0].pcon = ind;
			tmpath[0].cnt = 1;
			tmpath[0].skp = 0;
			visitedRegions[ine] = 1;
			doneRegion = ine;
			while (unvis())
				nxtrgn();
			ine = 0;
			cnt = 0xffffffff;
			sequencePath = (FSEQ*)&oseq[OSEQLEN >> 1];
			for (ind = 0; ind < sequencePathIndex; ind++) {
				sequencePath[ine].skp = tmpath[ind].skp;
				if (tmpath[ind].pcon == 0xffffffff) {
					sequencePath[ine++].vrt = tmpath[ind].cnt;
					cnt = tmpath[ind].cnt;
				}
				else {
					if (tmpath[ind].pcon != cnt) {
						cnt = tmpath[ind].pcon;
						sequencePath[ine++].vrt = pathMap[tmpath[ind].pcon].vrt;
					}
				}
			}
			sequencePathIndex = ind; mPathIndex = 0;
			for (ind = 0; ind < sequencePathIndex; ind++)
				nxtseq(ind);
			ine = (sortedLineIndex >> 5) + 1;
			sequenceFlagBitmap = new unsigned[ine];
			for (ind = 0; ind < ine; ind++)
				sequenceFlagBitmap[ind] = 0;
			for (ind = 0; ind < regionCount; ind++)
				visitedRegions[ind] = 0;
			lastGroup = 0;
			for (ind = 0; ind < mPathIndex; ind++) {
				//				sprintf_s(MsgBuffer, sizeof(MsgBuffer),"ind %d,vrt %d,grpn %d\n",ind,pathMap[ind].vrt,pathMap[ind].grpn);
				//				OutputDebugString(MsgBuffer);
				if (!unvis())
					break;
				durgn(ind);
			}
		}
		else {
			pathMap = new RCON[1];
			sequencePath = new FSEQ[1];
			ine = (sortedLineIndex >> 5) + 1;
			sequenceFlagBitmap = new unsigned[ine];
			for (ind = 0; ind < ine; ind++)
				sequenceFlagBitmap[ind] = 0;
			lastGroup = 0;
			sequencePath[0].vrt = 0;
			sequencePath[0].grpn = ptrSortedLines[regionsList[0].end]->group;
			sequencePath[0].skp = 0;
			durgn(0);
			delete[] sequencePath;
		}
		//skip:;

#if BUGSEQ

		seqskip : ;
#endif
				  delete[] ptrSortedLines;
				  delete[] lineEndpoints;
				  delete[] regionsList;
				  delete[] mapIndexSequence;
				  delete[] visitedRegions;
				  delete[] pathMap;
				  delete[] groupIndexSequence;
				  delete[] sequenceFlagBitmap;
	}
}

BOOL notin(unsigned ond) {
	dPOINT		pnt;
	unsigned	ind, cnt;
	dPOINT		ipnt;
	double		len;
	double		minot;		//minimum distance from a line
	unsigned	minotl;		//index to the minimum distance line

	cnt = 0; minot = 1e99;
	pnt.x = (oseq[ond + 1].x - oseq[ond].x) / 2 + oseq[ond].x;
	pnt.y = (oseq[ond + 1].y - oseq[ond].y) / 2 + oseq[ond].y;
	for (ind = 0; ind < SelectedForm->sides; ind++) {
		if (projv(pnt.x, currentFillVertices[ind], currentFillVertices[ind + 1], &ipnt)) {
			if (ipnt.y > pnt.y)
				cnt++;
			len = fabs(ipnt.y - pnt.y);
			if (len < minot) {
				minot = len;
				minotl = ind;
			}
		}
	}
	if (projv(pnt.x, currentFillVertices[ind], currentFillVertices[0], &ipnt)) {
		if (ipnt.y > pnt.y)
			cnt++;
		len = fabs(ipnt.y - pnt.y);
		if (len < minot) {
			minot = len;
			minotl = ind;
		}
	}
	return !(cnt & 1);
}

unsigned seqtab[] =
{
	12,7,15,11,13,17,
};

#define RITSIZ 6

void bakseq() {
#if BUGBAK

	for (sequenceIndex = 0; sequenceIndex < outputIndex; sequenceIndex++)
	{
		oseq[sequenceIndex].x = bseq[sequenceIndex].x;
		oseq[sequenceIndex].y = bseq[sequenceIndex].y;
	}
	SelectedForm->maxFillStitchLen = 6000;
#else

	int			cnt, rcnt;
	int			ind, rit;
	dPOINT		dif, pnt, stp;
	double		len, rslop;
	double		usesiz2 = UserStitchLength * 2;
	double		usesiz9 = UserStitchLength / 9;
	double		stspac2 = stitchSpace * 2;

	sequenceIndex = 0;
	rstMap(FILDIR);
	ind = outputIndex - 1;
	oseq[sequenceIndex].x = bseq[ind].x;
	oseq[sequenceIndex].y = bseq[ind].y;
	sequenceIndex++;
	SelectedPoint.x = bseq[ind].x;
	SelectedPoint.y = bseq[ind].y;
	ind--;
	while (ind > 0) {
		rcnt = ind%RITSIZ;
		if (sequenceIndex > MAXSEQ) {
			sequenceIndex = MAXSEQ - 1;
			return;
		}
		rit = bseq[ind].x / stspac2;
		dif.x = bseq[ind].x - bseq[ind + 1].x;
		dif.y = bseq[ind].y - bseq[ind + 1].y;
		if (dif.y)
			rslop = dif.x / dif.y;
		else
			rslop = 1e99;
		switch (bseq[ind].attribute) {
		case SEQTOP:

			if (SelectedForm->extendedAttribute&AT_SQR) {
				if (toglMap(FILDIR)) {
					oseq[sequenceIndex].x = bseq[ind - 1].x;
					oseq[sequenceIndex++].y = bseq[ind - 1].y;
					cnt = ceil(bseq[ind].y / UserStitchLength);
				blntop:;
					oseq[sequenceIndex].y = cnt*UserStitchLength + (rit%seqtab[rcnt])*usesiz9;
					if (oseq[sequenceIndex].y > bseq[ind].y)
						goto blntopx;
					dif.y = oseq[sequenceIndex].y - bseq[ind].y;
					oseq[sequenceIndex++].x = bseq[ind].x;
					cnt++;
					goto blntop;
				blntopx:;
					oseq[sequenceIndex].x = bseq[ind].x;
					oseq[sequenceIndex++].y = bseq[ind].y;
				}
				else {
					oseq[sequenceIndex].x = bseq[ind].x;
					oseq[sequenceIndex++].y = bseq[ind].y;
					cnt = floor(bseq[ind].y / UserStitchLength);
				blntbot:;
					oseq[sequenceIndex].y = cnt*UserStitchLength - ((rit + 2) % seqtab[rcnt])*usesiz9;
					if (oseq[sequenceIndex].y < bseq[ind - 1].y)
						goto blntbotx;
					dif.y = oseq[sequenceIndex].y - bseq[ind - 1].y;
					oseq[sequenceIndex++].x = bseq[ind].x;
					cnt--;
					goto blntbot;
				blntbotx:;
					oseq[sequenceIndex].x = bseq[ind - 1].x;
					oseq[sequenceIndex++].y = bseq[ind - 1].y;
				}
			}
			else {
				cnt = ceil(bseq[ind + 1].y / UserStitchLength);
			toplab:;
				oseq[sequenceIndex].y = cnt*UserStitchLength + (rit%seqtab[rcnt])*usesiz9;
				if (oseq[sequenceIndex].y > bseq[ind].y)
					goto toplabx;
				dif.y = oseq[sequenceIndex].y - bseq[ind + 1].y;
				dif.x = rslop*dif.y;
				oseq[sequenceIndex++].x = bseq[ind + 1].x + dif.x;
				cnt++;
				goto toplab;
			toplabx:;
				oseq[sequenceIndex].x = bseq[ind].x;
				oseq[sequenceIndex++].y = bseq[ind].y;
			}
			break;

		case SEQBOT:

			if (!(SelectedForm->extendedAttribute&AT_SQR)) {
				cnt = floor(bseq[ind + 1].y / UserStitchLength);
			botlab:;
				oseq[sequenceIndex].y = cnt*UserStitchLength - ((rit + 2) % seqtab[rcnt])*usesiz9;
				if (oseq[sequenceIndex].y < bseq[ind].y)
					goto botlabx;
				dif.y = oseq[sequenceIndex].y - bseq[ind + 1].y;
				dif.x = rslop*dif.y;
				oseq[sequenceIndex++].x = bseq[ind + 1].x + dif.x;
				cnt--;
				goto botlab;
			botlabx:;
				oseq[sequenceIndex].x = bseq[ind].x;
				oseq[sequenceIndex++].y = bseq[ind].y;
			}
			break;

		case 0:

			dif.x = bseq[ind].x - bseq[ind + 1].x;
			dif.y = bseq[ind].y - bseq[ind + 1].y;
			rstMap(FILDIR);
			len = hypot(dif.x, dif.y);
			if (len) {
				if (len > usesiz2) {
					pnt.x = bseq[ind + 1].x;
					pnt.y = bseq[ind + 1].y;
					cnt = len / UserStitchLength - 1;
					if (chkmax(cnt, sequenceIndex) || (cnt + sequenceIndex) > MAXSEQ - 3)
						return;
					stp.x = dif.x / cnt;
					stp.y = dif.y / cnt;
					while (cnt) {
						pnt.x += stp.x;
						pnt.y += stp.y;
						oseq[sequenceIndex].x = pnt.x;
						oseq[sequenceIndex++].y = pnt.y;
						cnt--;
					}
				}
			}
			oseq[sequenceIndex].x = bseq[ind].x;
			oseq[sequenceIndex++].y = bseq[ind].y;
		}
		ind--;
	}
#endif
}

void fnvrt() {
	unsigned	ind, ine, inf, cnt, lincnt = 0, tind;
	int			tcnt;
	dPOINTLINE*	jpts;
	dPOINTLINE**	pjpts;
	double		lox, hix, mx0, mstp;
	dPOINT		tpnt;
	unsigned*	tgrinds;
	unsigned	maxlins = 0;	//maximum angle fill lines for any adjusted y cordinate

	currentFillVertices = SelectedForm->vertices;
	sides = SelectedForm->sides;
	lox = hix = currentFillVertices[0].x;
	for (ind = 1; ind < sides; ind++) {
		if (currentFillVertices[ind].x > hix)
			hix = currentFillVertices[ind].x;
		if (currentFillVertices[ind].x < lox)
			lox = currentFillVertices[ind].x;
	}
	tcnt = lox / stitchSpace;
	lox = stitchSpace*tcnt;
	cnt = (hix - lox) / stitchSpace + 1;
	jpts = new dPOINTLINE[sides + 2];
	pjpts = new dPOINTLINE*[sides + 2];
	mstp = (hix - lox) / cnt;
	mx0 = lox;
	for (ind = 0; ind < cnt; ind++) {
		inf = 0;
		lox += mstp;
		for (ine = 0; ine < (unsigned)sides - 1; ine++) {
			if (projv(lox, currentFillVertices[ine], currentFillVertices[ine + 1], &tpnt))
				inf++;
		}
		if (projv(lox, currentFillVertices[ine], currentFillVertices[0], &tpnt))
			inf++;
		lincnt += inf;
		if (inf > maxlins)
			maxlins = inf;
	}
	maxlins = (maxlins >> 1);
	lineEndpoints = new SMALPNTL[lincnt + 1];
	stitchLineCount = 0; lineGroupIndex = 0;
	tgrinds = (unsigned*)bseq;
	groupIndexCount = 0;
	for (ind = 0; ind < cnt; ind++) {
		mx0 += mstp;
		inf = 0;
		for (ine = 0; ine < (unsigned)sides - 1; ine++) {
			if (projv(mx0, currentFillVertices[ine], currentFillVertices[ine + 1], &tpnt)) {
				pjpts[inf] = &jpts[inf];
				jpts[inf].line = ine;
				jpts[inf].x = tpnt.x;
				jpts[inf++].y = tpnt.y;

			}
		}
		if (projv(mx0, currentFillVertices[ine], currentFillVertices[0], &tpnt)) {
			pjpts[inf] = &jpts[inf];
			jpts[inf].line = ine;
			jpts[inf].x = tpnt.x;
			jpts[inf++].y = tpnt.y;
		}
		if (inf > 1) {
			inf &= 0xfffffffe;
			tgrinds[groupIndexCount++] = stitchLineCount;
			qsort((void*)pjpts, inf, 4, comp);
			ine = 0;
			tind = stitchLineCount;
			while (ine < inf) {
				if (stitchLineCount < lincnt) {
					lineEndpoints[stitchLineCount].line = pjpts[ine]->line;
					lineEndpoints[stitchLineCount].group = lineGroupIndex;
					lineEndpoints[stitchLineCount].x = pjpts[ine]->x;
					lineEndpoints[stitchLineCount++].y = pjpts[ine++]->y;
					lineEndpoints[stitchLineCount].line = pjpts[ine]->line;
					lineEndpoints[stitchLineCount].group = lineGroupIndex;
					lineEndpoints[stitchLineCount].x = pjpts[ine]->x;
					lineEndpoints[stitchLineCount++].y = pjpts[ine++]->y;
				}
			}
			if (stitchLineCount != tind)
				lineGroupIndex++;
		}
	}
	tgrinds[groupIndexCount++] = stitchLineCount;
	groupIndexSequence = new unsigned[groupIndexCount];
	for (ind = 0; ind < groupIndexCount; ind++)
		groupIndexSequence[ind] = tgrinds[ind];
	lineGroupIndex--;
	delete[] jpts;
	delete[] pjpts;
}

void fnhor() {
	unsigned	ind;

	frmcpy(&angledForm, &formList[closestFormToCursor]);
	RotationCenter.x = (double)(angledForm.rectangle.right - angledForm.rectangle.left) / 2 + angledForm.rectangle.left;
	RotationCenter.y = (double)(angledForm.rectangle.top - angledForm.rectangle.bottom) / 2 + angledForm.rectangle.bottom;
	RotationAngle = PI / 2;
	angledForm.vertices = angledFormVertices;
	for (ind = 0; ind < angledForm.sides; ind++) {
		angledForm.vertices[ind].x = SelectedForm->vertices[ind].x;
		angledForm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&angledForm.vertices[ind]);
	}
	SelectedForm = &angledForm;
	fnvrt();
	SelectedForm = &formList[closestFormToCursor];
}

void fsvrt() {
	delmclp(closestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = VRTF;
	SelectedForm->type = FRMFPOLY;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = stitchSpace;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refilfn();
}

void filvrt() {
	unsigned ind;;

	if (filmsgs(FMM_VRT))
		return;
	if (selectedFormCount) {
		savdo();
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				fsvrt();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			fsvrt();
			setMap(INIT);
			coltab();
			setMap(RESTCH);
		}
	}
}

void fshor() {
	delmclp(closestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = HORF;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = stitchSpace;
	SelectedForm->angleOrClipData.angle = (float)PI / 2;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refil();
}

void filhor() {
	unsigned ind;

	if (filmsgs(FMM_HOR))
		return;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				fshor();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			fshor();
			setMap(INIT);
			coltab();
			setMap(RESTCH);
		}
	}
}

void fsangl() {
	double bang;

	bang = RotationAngle;
	delmclp(closestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = ANGF;
	RotationAngle = PI / 2 - RotationAngle;
	SelectedForm->angleOrClipData.angle = (float)IniFile.fillAngle;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = stitchSpace;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refil();
	RotationAngle = bang;
}

void filangl() {
	unsigned ind;

	if (filmsgs(FMM_ANG))
		return;
	RotationAngle = IniFile.fillAngle;
	if (selectedFormCount) {
		savdo();
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				fsangl();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			fsangl();
			setMap(INIT);
			coltab();
			setMap(RESTCH);
		}
	}
}

void sRct2px(fRECTANGLE strct, RECT* pxrct) {
	pxrct->left = (strct.left - ZoomRect.left)*ZoomRatio.x + 0.5;
	pxrct->right = (strct.right - ZoomRect.left)*ZoomRatio.x + 0.5;
	pxrct->top = (StitchWindowClientRect.bottom) - (strct.top - ZoomRect.bottom)*ZoomRatio.y + 0.5;
	pxrct->bottom = (StitchWindowClientRect.bottom) - (strct.bottom - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void setmfrm() {
	unsigned	ind;
	POINT		tpnt;
	POINT		tof;

	sfCor2px(formList[closestFormToCursor].vertices[0], &tpnt);
	tof.x = Msg.pt.x - StitchWindowOrigin.x - tpnt.x + formMoveDelta.x;
	tof.y = Msg.pt.y - StitchWindowOrigin.y - tpnt.y + formMoveDelta.y;
	for (ind = 0; ind < formList[closestFormToCursor].sides; ind++) {
		sfCor2px(formList[closestFormToCursor].vertices[ind], &tpnt);
		formLines[ind].x = tpnt.x + tof.x;
		formLines[ind].y = tpnt.y + tof.y;
	}
	formLines[ind].x = formLines[0].x;
	formLines[ind].y = formLines[0].y;
}

void strtchbox() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, StretchBoxLine, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unstrtch() {
	if (rstMap(SHOSTRTCH))
		strtchbox();
}

unsigned chkfrm() {
	POINT		tpnt0, tpnt1;
	RECT		trct;
	unsigned	ind;
	double		len, mlen = 1e99;

	fvars(closestFormToCursor);
	tpnt0.x = Msg.pt.x - StitchWindowOrigin.x;
	tpnt0.y = Msg.pt.y - StitchWindowOrigin.y;
	newFormVertexCount = SelectedForm->sides + 1;
	duzrat();
	sRct2px(SelectedForm->rectangle, &trct);
	formOutlineRectangle[0].x = formOutlineRectangle[6].x = formOutlineRectangle[7].x = formOutlineRectangle[8].x = trct.left;
	formOutlineRectangle[1].x = formOutlineRectangle[5].x = midl(trct.right, trct.left);
	formOutlineRectangle[0].y = formOutlineRectangle[1].y = formOutlineRectangle[2].y = formOutlineRectangle[8].y = trct.top;
	formOutlineRectangle[3].y = formOutlineRectangle[7].y = midl(trct.top, trct.bottom);
	formOutlineRectangle[4].y = formOutlineRectangle[5].y = formOutlineRectangle[6].y = trct.bottom;
	formOutlineRectangle[2].x = formOutlineRectangle[3].x = formOutlineRectangle[4].x = trct.right;
	for (ind = 0; ind < 10; ind++) {
		len = hypot(formOutlineRectangle[ind].x - tpnt0.x, formOutlineRectangle[ind].y - tpnt0.y);
		if (len < mlen) {
			mlen = len;
			selectedFormControlVertex = ind;
		}
		if (mlen < CLOSENUF) {
			ritfrct(closestFormToCursor, StitchWindowDC);
			for (ind = 0; ind < 4; ind++) {
				StretchBoxLine[ind].x = formOutlineRectangle[ind << 1].x;
				StretchBoxLine[ind].y = formOutlineRectangle[ind << 1].y;
			}
			StretchBoxLine[4].x = StretchBoxLine[0].x;
			StretchBoxLine[4].y = StretchBoxLine[0].y;
			if (selectedFormControlVertex & 1)
				setMap(STRTCH);
			else {
				setMap(EXPAND);
				xyRatio = (double)(SelectedForm->rectangle.right - SelectedForm->rectangle.left) / (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom);
			}
			selectedFormControlVertex >>= 1;
			setMap(SHOSTRTCH);
			strtchbox();
			return 1;
		}
	}
	if (tpnt0.x >= trct.left&&tpnt0.x <= trct.right&&
		tpnt0.y >= trct.top&&tpnt0.y <= trct.bottom) {
		sfCor2px(SelectedForm->vertices[0], &tpnt1);
		formMoveDelta.x = tpnt1.x - tpnt0.x;
		formMoveDelta.y = tpnt1.y - tpnt0.y;
		setMap(FRMOV);
		return 1;
	}
	else
		return 0;
}

void rstfrm() {
	fPOINT		pof;
	unsigned	ind;
	POINT		tpnt;
	unsigned	tat = (closestFormToCursor << 4);

	setmfrm();
	rstMap(FRMOV);
	tpnt.x = Msg.pt.x + formMoveDelta.x;
	tpnt.y = Msg.pt.y + formMoveDelta.y;
	pxCor2stch(tpnt);
	pof.x = SelectedPoint.x - SelectedForm->vertices[0].x;
	pof.y = SelectedPoint.y - SelectedForm->vertices[0].y;
	for (ind = 0; ind < SelectedForm->sides; ind++) {
		SelectedForm->vertices[ind].x += pof.x;
		SelectedForm->vertices[ind].y += pof.y;
	}
	SelectedForm->rectangle.bottom += pof.y;
	SelectedForm->rectangle.top += pof.y;
	SelectedForm->rectangle.left += pof.x;
	SelectedForm->rectangle.right += pof.x;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		if ((StitchBuffer[ind].attribute&FRMSK) == tat&&StitchBuffer[ind].attribute&ALTYPMSK && !(StitchBuffer[ind].attribute&NOTFRM)) {
			StitchBuffer[ind].x += pof.x;
			StitchBuffer[ind].y += pof.y;
		}
	}
}

void clrfills() {
	unsigned ind;

	for (ind = 0; ind < formIndex; ind++) {
		formList[ind].clipEntries = 0;
		formList[ind].lengthOrCount.clipCount = 0;
		formList[ind].edgeType = 0;
		formList[ind].fillType = 0;
		formList[ind].attribute &= NFRECONT;
		formList[ind].extendedAttribute &= !(AT_UND | AT_WALK);
	}
	clipPointIndex = 0;
}

void dusat() {
	POINT* l_plin = &formLines[satinIndex - 1];

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, l_plin, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unsat() {
	if (rstMap(SHOSAT))
		dusat();
}

void drwsat() {
	unsat();
	px2stch();
	formLines[satinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	formLines[satinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	setMap(SHOSAT);
	dusat();
}

void satpnt0() {
	px2stch();
	formLines[0].x = Msg.pt.x - StitchWindowOrigin.x;
	formLines[0].y = Msg.pt.y - StitchWindowOrigin.y;
	tempPolygon[0].x = SelectedPoint.x;
	tempPolygon[0].y = SelectedPoint.y;
	satinIndex = 1;
	setMap(SATPNT);
}

void satpnt1() {
	unsat();
	px2stch();
	formLines[satinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	formLines[satinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	dusat();
	tempPolygon[satinIndex].x = SelectedPoint.x;
	tempPolygon[satinIndex].y = SelectedPoint.y;
	satinIndex++;
	setMap(RESTCH);
}

void satfix() {
	unsigned ind;

	if (satinIndex > 1) {
		formList[formIndex].vertices = adflt(satinIndex);
		for (ind = 0; ind < satinIndex; ind++) {
			formList[formIndex].vertices[ind].x = tempPolygon[ind].x;
			formList[formIndex].vertices[ind].y = tempPolygon[ind].y;
		}
		formList[formIndex].sides = satinIndex;
		frmout(formIndex);
		formList[formIndex].satinGuideCount = 0;
		formIndex++;
		setMap(INIT);
	}
	rstMap(SHOSAT);
	setMap(RESTCH);
}

void delcon(unsigned p_cpnt)
{
	unsigned ind;
	unsigned loc;
	SATCON* tp;
	FRMHED* fp;

	tp = &SelectedForm->satinOrAngle.sac[p_cpnt];
	loc = &SelectedForm->satinOrAngle.sac[p_cpnt] - satinConns;
	if (satinConnectIndex > loc)
		MoveMemory(tp, &tp[1], (satinConnectIndex - loc + 1) * sizeof(SATCON));
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
	{
		fp = &formList[ind];
		if (fp->type == SAT&&fp->satinGuideCount)
			fp->satinOrAngle.sac--;
	}
	if (closestVertexToCursor < wordParam)
		wordParam--;
	SelectedForm->satinGuideCount--;
	satinConnectIndex--;
	currentFormConnectionsCount = SelectedForm->satinGuideCount;
	if (SelectedForm->fillType == SATF)
		refil();
	coltab();
	setMap(RESTCH);
}

BOOL satselfn() {
	unsigned ind, ine, playcod;
	double		tlen, len = 1e99;
	double		dx, dy;

	px2stch();
	for (ind = 0; ind < formIndex; ind++) {
		playcod = (formList[ind].attribute&FRMLMSK) >> 1;
		if (!ActiveLayer || !playcod || playcod == ActiveLayer) {
			for (ine = 0; ine < formList[ind].sides; ine++) {
				dx = SelectedPoint.x - formList[ind].vertices[ine].x;
				dy = SelectedPoint.y - formList[ind].vertices[ine].y;
				tlen = hypot(dx, dy);
				if (tlen < len) {
					len = tlen;
					closestFormToCursor = ind;
					closestVertexToCursor = ine;
				}
			}
		}
	}
	if (len < CLOSENUF)
		return 1;
	else
		return 0;
}

void satsel() {
	if (satselfn()) {
		fvars(closestFormToCursor);
		duzrat();
		startPoint = closestVertexToCursor;
		sfCor2px(SelectedForm->vertices[closestVertexToCursor], &formLines[0]);
		rstMap(SHOCON);
		setMap(SATCNKT);
		if (SelectedForm->type == FRMFPOLY)
			SelectedForm->type = SAT;
	}
}

void ducon() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, formLines, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uncon() {
	if (rstMap(SHOCON))
		ducon();
}

void drwcon() {
	uncon();
	formLines[1].x = Msg.pt.x - StitchWindowOrigin.x;
	formLines[1].y = Msg.pt.y - StitchWindowOrigin.y;
	setMap(SHOCON);
	ducon();
}

int scomp(const void *arg1, const void *arg2) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, arg2
		mov		ebx, [ebx]
		mov		bx, [ebx]
		mov		ecx, arg1
		mov		ecx, [ecx]
		mov		cx, [ecx]
		cmp		bx, cx
		je		short scmpx
		jc		short scmp1
		dec		eax
		jmp		short scmpx
		scmp1 : inc		eax
				scmpx :
	}
#else
	unsigned short **s1 = (unsigned short **)arg1, **s2 = (unsigned short **)arg2;
	if (**s2 == **s1) return 0;
	if (**s2 < **s1) return 1;
	return -1;
#endif
}

unsigned setchk(unsigned bPnt) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, checkMap
		mov		ecx, bPnt
		bts[ebx], ecx
		jnc		short setcx
		dec		eax
		setcx :
	}
#else
	return _bittestandset((long *)checkMap, bPnt) ? 0xFFFFFFFF : 0;
#endif
}

unsigned chkchk(unsigned ind) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, checkMap
		mov		ecx, ind
		bt[ebx], ecx
		jc		short ccx
		dec		eax
		ccx :
	}
#else
	return _bittest((long *)chkMap, ind) ? 0xFFFFFFFF : 0;
#endif
}

unsigned nxtchk(unsigned ind) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, ind
		shl		ebx, 2
		add		ebx, checkMap
		mov		ecx, [ebx]
		bsf		eax, ecx
		jne		short nxtc1
		dec		eax
		jmp		short nxtcx
		nxtc1 : btr		ecx, eax
				mov[ebx], ecx
				nxtcx :
	}
#else
	if (checkMap[ind] == 0)
		return 0xffffffff;

	DWORD bit;

	_BitScanForward(&bit, checkMap[ind]);
	_bittestandreset((long *)(checkMap + ind), bit);

	return bit;
#endif
}

unsigned prvchk(unsigned ind) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, ind
		shl		ebx, 2
		add		ebx, checkMap
		mov		ecx, [ebx]
		bsr		eax, ecx
		jne		short prvc1
		dec		eax
		jmp		short prvcx
		prvc1 : btr		ecx, eax
				mov[ebx], ecx
				prvcx :
	}
#else
	//Check translation
	DWORD bit;

	if (checkMap[ind] == 0)
		return 0xffffffff;

	_BitScanReverse(&bit, checkMap[ind]);
	_bittestandreset((long *)(checkMap + ind), bit);

	return bit;
#endif
}

void satadj()
{
	unsigned	ind, ine, inf, psac, mapsiz, prstpt;
	SATCON*		l_spnt;
	SATCON*		l_dpnt;
	unsigned short bstpt;
	FRMHED*		fp;

	fvars(closestFormToCursor);
	bstpt = SelectedForm->satinGuideCount;
	for (ind = 0; ind < SelectedForm->satinGuideCount; ind++)
	{
		if (currentFormConnections[ind].finish > sides - 1)
			currentFormConnections[ind].finish = sides - 1;
		if (currentFormConnections[ind].start > sides - 1)
			currentFormConnections[ind].start = sides - 1;
	}
	l_spnt = (SATCON*)bseq;
	mapsiz = (sides >> 5) + 1;
	checkMap = (unsigned*)oseq;
	psac = 0;
	prstpt = currentFormConnectionsCount;
	for (ind = 0; ind < currentFormConnectionsCount; ind++)
	{
		if (currentFormConnections[ind].start != currentFormConnections[ind].finish)
		{
			currentFormConnections[psac].start = currentFormConnections[ind].start;
			currentFormConnections[psac].finish = currentFormConnections[ind].finish;
			psac++;
		}
	}
	currentFormConnectionsCount = SelectedForm->satinGuideCount = ind;
	if (wordParam || SelectedForm->attribute&FRMEND)
	{
		for (ind = 0; ind < mapsiz; ind++)
			checkMap[ind] = 0;
		if (SelectedForm->attribute&FRMEND)
		{
			setchk(0);
			setchk(1);
		}
		if (wordParam)
		{
			setchk(wordParam);
			setchk(wordParam + 1);
		}
		ine = 0;
		for (ind = 0; ind < currentFormConnectionsCount; ind++)
		{
			if (chkchk(currentFormConnections[ind].start) && chkchk(currentFormConnections[ind].finish))
			{
				l_spnt[ine].start = currentFormConnections[ind].start;
				l_spnt[ine].finish = currentFormConnections[ind].finish;
				ine++;
			}
		}
		currentFormConnectionsCount = SelectedForm->satinGuideCount = ine;
		if (wordParam)
		{
			ine = 0;
			for (ind = 0; ind < currentFormConnectionsCount; ind++)
			{
				if (currentFormConnections[ind].start < wordParam)
				{
					l_spnt[ine].start = currentFormConnections[ind].start;
					l_spnt[ine++].finish = currentFormConnections[ind].finish;
				}
			}
			currentFormConnectionsCount = SelectedForm->satinGuideCount = ine;
		}
	}
	else
	{
		for (ind = 0; ind < currentFormConnectionsCount; ind++)
		{
			l_spnt[ind].start = currentFormConnections[ind].start;
			l_spnt[ind].finish = currentFormConnections[ind].finish;
		}
	}
	if (currentFormConnectionsCount)
	{
		for (ind = 0; ind < mapsiz; ind++)
			checkMap[ind] = 0;
		for (ind = 0; ind < currentFormConnectionsCount; ind++)
		{
			ine = currentFormConnections[ind].start;
			if (ine > (unsigned)wordParam - 1)
				ine = wordParam - 1;
			if (setchk(ine))
			{
				inf = ine;
				if (inf)
					inf--;
				while (!chkchk(ine) && ine < (unsigned)wordParam - 1)
					ine++;
				while (inf && (!chkchk(inf)))
					inf--;
				if (!chkchk(ine) && !chkchk(inf))
					break;
				if (chkchk(ine) && chkchk(inf))
				{
					if (ine - currentFormConnections[ind].start > currentFormConnections[ind].start - inf)
						setchk(inf);
					else
						setchk(ine);
				}
				else {
					if (chkchk(ine))
						setchk(inf);
					else
						setchk(ine);
				}

			}
		}
		inf = 0;
		for (ind = 0; ind < mapsiz; ind++)
		{
			do
			{
				ine = nxtchk(ind);
				if (ine < sides)
					currentFormConnections[inf++].start = ine + (ind << 5);
			} while (ine < sides);
		}
		currentFormConnectionsCount = SelectedForm->satinGuideCount = inf;
		for (ind = 0; ind < mapsiz; ind++)
			checkMap[ind] = 0;
		for (ind = 0; ind < currentFormConnectionsCount; ind++)
		{
			ine = inf = currentFormConnections[ind].finish;
			if (ine > (unsigned)sides - 1)
				ine = sides - 1;
			if (setchk(ine))
			{
				if (ine < (unsigned)sides - 1)
					ine++;
				if (inf > (unsigned)wordParam + 1)
					inf--;
				while (!chkchk(ine) && ine < (unsigned)sides - 1)
					ine++;
				while (inf > (unsigned)wordParam - 1 && (!chkchk(inf)))
					inf--;
				if (!chkchk(ine) && !chkchk(inf))
					break;
				if (chkchk(ine) && chkchk(inf))
				{
					if (ine - currentFormConnections[ind].finish > currentFormConnections[ind].finish - inf)
						setchk(inf);
					else
						setchk(ine);
				}
				else
				{
					if (chkchk(ine))
						setchk(ine);
					else
						setchk(inf);
				}
			}
		}
		inf = 0;
		for (ind = mapsiz; ind != 0; ind--)
		{
			do
			{
				ine = prvchk(ind - 1);
				if (ine < sides)
					currentFormConnections[inf++].finish = ine + ((ind - 1) << 5);
			} while (ine < sides);
		}
		if (inf < currentFormConnectionsCount)
			inf = currentFormConnectionsCount;
		currentFormConnectionsCount = SelectedForm->satinGuideCount = inf;
		if (wordParam)
		{
			if (currentFormConnectionsCount > sides - wordParam - 2)
				currentFormConnectionsCount = sides - wordParam - 2;
			if (currentFormConnectionsCount > wordParam - 2)
				currentFormConnectionsCount = wordParam - 2;
			SelectedForm->satinGuideCount = currentFormConnectionsCount;
		}
	}
	if (SelectedForm->satinGuideCount < bstpt)
	{
		ine = bstpt - currentFormConnectionsCount;
		l_spnt = l_dpnt = SelectedForm->satinOrAngle.sac;
		l_dpnt += SelectedForm->satinGuideCount;
		l_spnt += bstpt;
		MoveMemory(l_dpnt, l_spnt, sizeof(SATCON)*(&satinConns[satinConnectIndex] - l_spnt + 1));
		for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
		{
			fp = &formList[ind];
			if (fp->type == SAT)
				fp->satinOrAngle.sac -= ine;
		}
		satinConnectIndex -= ine;
	}
}

void satclos() {
	unsigned	ind, clos, psac;
	double		dx, dy, tlen, len = 1e99;
	SATCON*		tsac;

	uninsf();
	px2stch();
	psac = SelectedForm->satinGuideCount;
	tsac = SelectedForm->satinOrAngle.sac;
	clos = startPoint;
	for (ind = 0; ind < SelectedForm->sides; ind++) {
		dx = SelectedPoint.x - SelectedForm->vertices[ind].x;
		dy = SelectedPoint.y - SelectedForm->vertices[ind].y;
		tlen = hypot(dx, dy);
		if (tlen < len) {
			len = tlen;
			closestVertexToCursor = ind;
		}
	}
	rstMap(SATCNKT);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->fillType = CONTF;
		clos = closestVertexToCursor;
		if (startPoint > clos) {
			psac = clos;
			clos = startPoint;
			startPoint = psac;
		}
		if (!startPoint)
			startPoint++;
		if (startPoint == SelectedForm->sides - 2 && clos == (unsigned)SelectedForm->sides - 1) {
			startPoint = 1;
			clos = SelectedForm->sides - 2;
		}
		if (clos >= (unsigned)SelectedForm->sides - 2) {
			clos = SelectedForm->sides - 2;
			if (startPoint >= (unsigned)SelectedForm->sides - 2)
				startPoint = SelectedForm->sides - 2;
		}
		if (clos - startPoint < 2) {
			clos = startPoint + 2;
			if (clos > (unsigned)SelectedForm->sides - 2) {
				clos = clos - SelectedForm->sides - 2;
				clos -= clos;
				startPoint -= clos;
			}
		}
		SelectedForm->angleOrClipData.sat.start = startPoint;
		SelectedForm->angleOrClipData.sat.finish = clos;
	}
	else {
		if (closestVertexToCursor < clos) {
			ind = closestVertexToCursor;
			closestVertexToCursor = clos;
			clos = ind;
		}
		if (clos == 0 && closestVertexToCursor == (unsigned)sides - 1) {
			clos = sides - 1;
			closestVertexToCursor = sides;
		}
		if (clos == 1 && closestVertexToCursor == (unsigned)sides) {
			clos = 0;
			closestVertexToCursor = 1;
		}
		if (closestVertexToCursor - clos == 1) {
			if (SelectedForm->attribute&FRMEND)
				SelectedForm->wordParam = clos;
			else {
				if (clos)
					rotfrm(clos);
				SelectedForm->attribute |= FRMEND;
			}
			satadj();
		}
		else {
			if (currentFormConnectionsCount) {
				sacspac(&SelectedForm->satinOrAngle.sac[SelectedForm->satinGuideCount], 1);
				SelectedForm->satinOrAngle.sac[SelectedForm->satinGuideCount].start = clos;
				SelectedForm->satinOrAngle.sac[SelectedForm->satinGuideCount++].finish = closestVertexToCursor;
				satadj();
			}
			else {
				SelectedForm->satinOrAngle.sac = nusac(closestFormToCursor, 1);
				SelectedForm->satinOrAngle.sac[psac].start = clos;
				SelectedForm->satinOrAngle.sac[psac].finish = closestVertexToCursor;
				SelectedForm->satinGuideCount = 1;
			}
		}
	}
}

void satknkt() {
	satclos();
	fvars(closestFormToCursor);
	refil();
	setMap(RESTCH);
}

void ritseq1(unsigned ind) {
	bseq[sequenceIndex].x = currentFormVertices[ind].x;
	bseq[sequenceIndex].y = currentFormVertices[ind].y;
	sequenceIndex++;
}

void satfn(unsigned astrt, unsigned afin, unsigned bstrt, unsigned bfin) {
	unsigned	anxt, bprv, cnt, ind, acnt, bcnt, ine, inf, tcnt, acind, bcind, pacnt = 0, pbcnt = 0;
	unsigned	asegs, bsegs;
	unsigned*	acnts;
	unsigned*	bcnts;
	double		alen, blen;
	dPOINT		apnt, bpnt, adif, bdif, astp, bstp;

	if (astrt != afin&&bstrt != bfin) {
		if (!setMap(SAT1)) {
			if (chkMap(FTHR)) {
				bseq[sequenceIndex].attribute = 0;
				ritseq1(astrt%sides);
			}
			else {
				if (chkMap(BARSAT)) {
					ritseq1(astrt%sides);
					ritseq1(bstrt%sides);
				}
				else {
					SelectedPoint.x = currentFormVertices[astrt].x;
					SelectedPoint.y = currentFormVertices[astrt].y;
					oseq[sequenceIndex].x = SelectedPoint.x;
					oseq[sequenceIndex++].y = SelectedPoint.y;
				}
			}
		}
		alen = lengths[afin] - lengths[astrt];
		blen = lengths[bstrt] - lengths[bfin];
		if (fabs(alen) > fabs(blen))
			cnt = fabs(blen) / stitchSpace;
		else
			cnt = fabs(alen) / stitchSpace;
		asegs = ((afin > astrt) ? (afin - astrt) : (astrt - afin));
		bsegs = ((bstrt > bfin) ? (bstrt - bfin) : (bfin - bstrt));
		acnts = new unsigned[asegs];
		bcnts = new unsigned[bsegs + 1];
		ine = astrt;
		tcnt = 0;
		for (ind = 0; ind < asegs - 1; ind++) {
			inf = nxt(ine);
			acnts[ind] = ((lengths[inf] - lengths[ine]) / alen)*cnt + 0.5;
			tcnt += acnts[ind];
			ine++;
		}
		acnts[ind] = cnt - tcnt;
		ind = bstrt;
		ine = prv(ind);
		inf = 0;
		tcnt = 0;
		while (ine > bfin) {
			bcnts[inf] = ((lengths[ind] - lengths[ine]) / blen)*cnt + 0.5;
			tcnt += bcnts[inf++];
			ine = prv(--ind);
		}
		bcnts[inf] = cnt - tcnt;
		apnt.x = currentFormVertices[astrt].x;
		apnt.y = currentFormVertices[astrt].y;
		anxt = nxt(astrt);
		bprv = prv(bstrt);
		acnt = acnts[pacnt++];
		bcnt = bcnts[pbcnt++];
		acind = astrt;
		bcind = bstrt;
		adif.x = currentFormVertices[anxt].x - currentFormVertices[acind].x;
		adif.y = currentFormVertices[anxt].y - currentFormVertices[acind].y;
		if (bcind == sides) {
			bdif.x = currentFormVertices[bprv].x - currentFormVertices[0].x;
			bdif.y = currentFormVertices[bprv].y - currentFormVertices[0].y;
			bpnt.x = currentFormVertices[0].x;
			bpnt.y = currentFormVertices[0].y;
		}
		else {
			bdif.x = currentFormVertices[bprv].x - currentFormVertices[bcind].x;
			bdif.y = currentFormVertices[bprv].y - currentFormVertices[bcind].y;
			bpnt.x = currentFormVertices[bcind].x;
			bpnt.y = currentFormVertices[bcind].y;
		}
		acind = nxt(acind);
		bcind = prv(bcind);
		astp.x = adif.x / acnt;
		astp.y = adif.y / acnt;
		bstp.x = bdif.x / bcnt;
		bstp.y = bdif.y / bcnt;
	nuseg:;

		if (chkMap(FTHR)) {
			while (acnt&&bcnt) {
				apnt.x += astp.x;
				apnt.y += astp.y;
				bpnt.x += bstp.x;
				bpnt.y += bstp.y;
				if (toglMap(FILDIR)) {
					bseq[sequenceIndex].attribute = 0;
					bseq[sequenceIndex].x = apnt.x;
					bseq[sequenceIndex++].y = apnt.y;
				}
				else {
					bseq[sequenceIndex].attribute = 1;
					bseq[sequenceIndex].x = bpnt.x;
					bseq[sequenceIndex++].y = bpnt.y;
				}
				if (sequenceIndex > MAXSEQ - 6) {
					sequenceIndex = MAXSEQ - 6;
					return;
				}
				acnt--;
				bcnt--;
			}
		}
		else {
			if (chkMap(BARSAT)) {
				while (acnt&&bcnt) {
					apnt.x += astp.x;
					apnt.y += astp.y;
					bpnt.x += bstp.x;
					bpnt.y += bstp.y;
					if (toglMap(FILDIR)) {
						bseq[sequenceIndex].attribute = 0;
						bseq[sequenceIndex].x = apnt.x;
						bseq[sequenceIndex++].y = apnt.y;
						bseq[sequenceIndex].attribute = 1;
						bseq[sequenceIndex].x = bpnt.x;
						bseq[sequenceIndex++].y = bpnt.y;
					}
					else {
						bseq[sequenceIndex].attribute = 2;
						bseq[sequenceIndex].x = bpnt.x;
						bseq[sequenceIndex++].y = bpnt.y;
						bseq[sequenceIndex].attribute = 3;
						bseq[sequenceIndex].x = apnt.x;
						bseq[sequenceIndex++].y = apnt.y;
					}
					if (sequenceIndex > MAXSEQ - 6) {
						sequenceIndex = MAXSEQ - 6;
						return;
					}
					acnt--;
					bcnt--;
				}
			}
			else {
				while (acnt&&bcnt) {
					apnt.x += astp.x;
					apnt.y += astp.y;
					bpnt.x += bstp.x;
					bpnt.y += bstp.y;
					if (toglMap(FILDIR)) {
						if (chku(SQRFIL))
							filinu(bpnt.x, bpnt.y);
						filin(apnt);
					}
					else {
						if (chku(SQRFIL))
							filinu(apnt.x, apnt.y);
						filin(bpnt);
					}
					acnt--;
					bcnt--;
				}
			}
		}
		if ((pacnt < asegs || pbcnt < bsegs)) {
			if (!acnt) {
				acnt = acnts[pacnt++];
				anxt = nxt(acind);
				adif.x = currentFormVertices[anxt].x - currentFormVertices[acind].x;
				adif.y = currentFormVertices[anxt].y - currentFormVertices[acind].y;
				acind = nxt(acind);
				astp.x = adif.x / acnt;
				astp.y = adif.y / acnt;
			}
			if (!bcnt) {
				bcnt = bcnts[pbcnt++];
				bprv = prv(bcind);
				bdif.x = currentFormVertices[bprv].x - currentFormVertices[bcind].x;
				bdif.y = currentFormVertices[bprv].y - currentFormVertices[bcind].y;
				bcind = prv(bcind);
				bstp.x = bdif.x / bcnt;
				bstp.y = bdif.y / bcnt;
			}
			if ((acnt || bcnt) && acnt < MAXSEQ&&bcnt < MAXSEQ)
				goto nuseg;
		}
		delete[] acnts;
		delete[] bcnts;
	}
}

void satmf() {
	unsigned	ind, ine;
	double		len, dx, dy;

	ind = 0;
	if (SelectedForm->attribute&FRMEND)
		ind = 1;
	satfn(ind, currentFormConnections[0].start, sides, currentFormConnections[0].finish);
	for (ind = 0; ind < (unsigned)currentFormConnectionsCount - 1; ind++)
		satfn(currentFormConnections[ind].start, currentFormConnections[ind + 1].start, currentFormConnections[ind].finish, currentFormConnections[ind + 1].finish);
	if (wordParam)
		satfn(currentFormConnections[ind].start, wordParam, currentFormConnections[ind].finish, wordParam + 1);
	else {
		if (currentFormConnections[ind].finish - currentFormConnections[ind].start > 2) {
			len = (lengths[currentFormConnections[ind].finish] - lengths[currentFormConnections[ind].start]) / 2 + lengths[currentFormConnections[ind].start];
			ine = currentFormConnections[ind].start;
			while (len > lengths[ine])
				ine++;
			dx = lengths[ine] - len;
			dy = len - lengths[ine - 1];
			if (dy > dx)
				ine--;
			satfn(currentFormConnections[ind].start, ine, currentFormConnections[ind].finish, ine);
		}
		else
			satfn(currentFormConnections[ind].start, currentFormConnections[ind].start + 1, currentFormConnections[ind].finish, currentFormConnections[ind].start + 1);
	}
}

extern void prbug();

void satfil() {
	unsigned		ind;
	double			len, dx, dy;
	double			tspac;

	fvars(closestFormToCursor);
	satadj();
	tspac = stitchSpace;
	stitchSpace /= 2;
	sequenceIndex = 0;
	rstMap(SAT1);
	rstMap(FILDIR);
	SelectedForm->fillType = SATF;
	lengths = new double[sides + 2];
	len = 0;
	for (ind = 0; ind < (unsigned)sides - 1; ind++) {
		lengths[ind] = len;
		dx = currentFormVertices[ind + 1].x - currentFormVertices[ind].x;
		dy = currentFormVertices[ind + 1].y - currentFormVertices[ind].y;
		len += hypot(dx, dy);
	}
	lengths[ind] = len;
	dx = currentFormVertices[0].x - currentFormVertices[ind].x;
	dy = currentFormVertices[0].y - currentFormVertices[ind].y;
	len += hypot(dx, dy);
	lengths[ind + 1] = len;
	if (wordParam) {
		if (currentFormConnectionsCount) {
			satmf();
			goto satdun;
		}
		else {
			satfn(1, wordParam, sides, wordParam + 1);
			goto satdun;
		}
	}
	if (SelectedForm->attribute&FRMEND) {
		if (currentFormConnectionsCount) {
			satmf();
			goto satdun;
		}
		else {
			if (sides == 3 && formList[closestFormToCursor].attribute & 1) {
				satfn(2, 3, 2, 1);
				goto satdun;
			}
			else {
				len = (len - lengths[1]) / 2;
				ind = 1;
				if (!chkMap(BARSAT)) {
					oseq[0].x = SelectedPoint.x = currentFormVertices[1].x;
					oseq[0].y = SelectedPoint.y = currentFormVertices[1].y;
					sequenceIndex = 1;
				}
				while ((len > lengths[ind]) && (ind < (unsigned int)(sides + 1)))
					ind++;
				dx = lengths[ind] - len;
				dy = len - lengths[ind - 1];
				if (dy > dx)
					ind--;
				satfn(1, ind, sides, ind);
			}
			goto satdun;
		}
	}
	if (currentFormConnectionsCount) {
		satmf();
		goto satdun;
	}
	len /= 2;
	ind = 0;
	if (!chkMap(BARSAT) && !chkMap(FTHR)) {
		oseq[0].x = SelectedPoint.x = currentFormVertices[0].x;
		oseq[0].y = SelectedPoint.y = currentFormVertices[0].y;
		sequenceIndex = 1;
	}
	while (len > lengths[ind])
		ind++;
	dx = lengths[ind] - len;
	dy = len - lengths[ind - 1];
	if (dy > dx)
		ind--;
	satfn(0, ind, sides, ind);
satdun:;

	delete[] lengths;
	stitchSpace = tspac;
}

void filsfn() {
	delmclp(closestFormToCursor);
	deltx();
	SelectedForm->type = SAT;
	fsizpar();
	SelectedForm->fillType = SATF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->fillSpacing = stitchSpace;
	SelectedForm->type = SAT;
	refilfn();
}

void filsat() {
	unsigned ind;

	if (filmsgs(FMM_FAN))
		return;
	if (selectedFormCount) {
		savdo();
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				filsfn();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			filsfn();
			setMap(INIT);
			coltab();
			setMap(RESTCH);
		}
	}
}

unsigned closat() {
	unsigned	ind, ine;
	double		len = 1e99, tlen, dx, dy;

	px2stch();
	for (ind = 0; ind < formIndex; ind++) {
		if (!ActiveLayer || (unsigned)((formList[ind].attribute&FRMLMSK) >> 1) == ActiveLayer || !(formList[ind].attribute&FRMLMSK)) {
			currentFormVertices = formList[ind].vertices;
			for (ine = 0; ine < formList[ind].sides; ine++) {
				dx = SelectedPoint.x - currentFormVertices[ine].x;
				dy = SelectedPoint.y - currentFormVertices[ine].y;
				tlen = hypot(dx, dy);
				if (tlen < len) {
					len = tlen;
					closestFormToCursor = ind;
					closestVertexToCursor = ine;
				}
			}
		}
	}
	if (len == 1e99)
		return 0;
	else
		return 1;
}

void nufpnt(unsigned pnu) {
	unsigned ind;

	pnu++;
	fltspac(&formForInsert->vertices[pnu], 1);
	formForInsert->vertices[pnu].x = SelectedPoint.x;
	formForInsert->vertices[pnu].y = SelectedPoint.y;
	formForInsert->sides++;
	for (ind = 0; ind < formForInsert->satinGuideCount; ind++) {
		if (formForInsert->satinOrAngle.sac[ind].start > pnu - 1)
			formForInsert->satinOrAngle.sac[ind].start++;
		if (formForInsert->satinOrAngle.sac[ind].finish > pnu - 1)
			formForInsert->satinOrAngle.sac[ind].finish++;
	}
	if (formForInsert->wordParam >= pnu) {
		formForInsert->wordParam++;
		formForInsert->wordParam %= sides;
	}
	if (formForInsert->fillType == CONTF) {
		if (formForInsert->angleOrClipData.sat.start > pnu - 1)
			formForInsert->angleOrClipData.sat.start++;
		if (formForInsert->angleOrClipData.sat.finish > pnu - 1)
			formForInsert->angleOrClipData.sat.finish++;
	}
	frmlin(formForInsert->vertices, formForInsert->sides);
}

double p2p(fPOINT pnt0, fPOINT pnt1) {
	return hypot(pnt0.x - pnt1.x, pnt0.y - pnt1.y);
}

unsigned upsat() {
	unsigned	ind;
	double		dwnlen, uplen, p2clos, p2up, p2dwn;

	sides = formForInsert->sides;
	currentFormVertices = formForInsert->vertices;
	p2clos = p2p(SelectedPoint, currentFormVertices[closestVertexToCursor]);
	ind = prv(closestVertexToCursor);
	dwnlen = p2p(currentFormVertices[ind], currentFormVertices[closestVertexToCursor]);
	p2dwn = p2p(currentFormVertices[ind], SelectedPoint);
	ind = nxt(closestVertexToCursor);
	uplen = p2p(currentFormVertices[ind], currentFormVertices[closestVertexToCursor]);
	p2up = p2p(currentFormVertices[ind], SelectedPoint);
	if ((p2dwn + p2clos) / dwnlen > (p2up + p2clos) / uplen)
		return 0;
	else {
		return 1;
	}
}

void insat() {
	if (closat()) {
		savdo();
		SelectedForm = &formList[closestFormToCursor];
		formForInsert = SelectedForm;
		fvars(closestFormToCursor);
		if (upsat()) {
			if (!closestVertexToCursor&&formForInsert->type == FRMLINE)
				setMap(PRELIN);
			else
				closestVertexToCursor = prv(closestVertexToCursor);
			nufpnt(closestVertexToCursor);
			if (rstMap(PRELIN)) {
				SelectedPoint.x = formForInsert->vertices[0].x;
				SelectedPoint.y = formForInsert->vertices[0].y;
				formForInsert->vertices[0].x = formForInsert->vertices[1].x;
				formForInsert->vertices[0].y = formForInsert->vertices[1].y;
				formForInsert->vertices[1].x = SelectedPoint.x;
				formForInsert->vertices[1].y = SelectedPoint.y;
			}
		}
		else
			nufpnt(closestVertexToCursor);
		refil();
	}
	setMap(RESTCH);
}

BOOL chkdel() {
	if (SelectedForm->type == FRMLINE) {
		if (SelectedForm->sides > 2)
			return 0;
		else
			return 1;
	}
	else {
		if (SelectedForm->sides > 3)
			return 0;
		else
			return 1;
	}
}

void delspnt() {
	unsigned	ind, ine;
	SATCON*		l_sac;
	fPOINT*		tflt;
	FRMHED* fp;

	fvars(closestFormToCursor);
	if (chkdel()) {
		setMap(DELTO);
		frmdel();
		rstMap(FRMPSEL);
		coltab();
		setMap(RESTCH);
		return;
	}
	if (SelectedForm->type == SAT) {
		if (closestVertexToCursor < SelectedForm->wordParam)
			SelectedForm->wordParam--;
		if (SelectedForm->satinGuideCount) {
			l_sac = SelectedForm->satinOrAngle.sac;
			ind = 0;
			while (l_sac[ind].start != closestVertexToCursor&&l_sac[ind].finish != closestVertexToCursor&&ind < SelectedForm->satinGuideCount)
				ind++;
			if (ind < SelectedForm->satinGuideCount && (l_sac[ind].start == closestVertexToCursor || l_sac[ind].finish == closestVertexToCursor)) {
				while (ind < SelectedForm->satinGuideCount) {
					l_sac[ind].start = l_sac[ind + 1].start;
					l_sac[ind].finish = l_sac[ind + 1].finish;
					ind++;
				}
				SelectedForm->satinGuideCount--;
				satinConnectIndex--;
				for (ine = closestFormToCursor + 1; ine < formIndex; ine++)
				{
					fp = &formList[ine];
					if (fp->type == SAT&&fp->satinGuideCount)
						fp->satinOrAngle.sac++;
				}
			}
			for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {
				if (l_sac[ind].start > closestVertexToCursor)
					l_sac[ind].start--;
				if (l_sac[ind].finish > closestVertexToCursor)
					l_sac[ind].finish--;
			}
		}
	}
	MoveMemory(&SelectedForm->vertices[closestVertexToCursor], &SelectedForm->vertices[closestVertexToCursor + 1], (FormVertexIndex - closestVertexToCursor) * sizeof(fPOINTATTR));
	SelectedForm->sides--;
	FormVertexIndex--;
	fvars(closestFormToCursor);
	if (closestVertexToCursor > (unsigned)SelectedForm->sides - 1)
		closestVertexToCursor = SelectedForm->sides - 1;
	setMap(FRMPSEL);
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
		formList[ind].vertices--;
	ritfcor(&currentFormVertices[closestVertexToCursor]);
	ritnum(STR_NUMPNT, closestVertexToCursor);
	frmout(closestFormToCursor);
	tflt = &SelectedForm->vertices[closestVertexToCursor];
	if (tflt->x<ZoomRect.left || tflt->x>ZoomRect.right || tflt->y<ZoomRect.bottom || tflt->y>ZoomRect.top)
		shft(SelectedForm->vertices[closestVertexToCursor]);
	refil();
}

void unfil() {
	unsigned src, dst, trg, at;
	unsigned mlen;

	if (filmsgs(FMX_UNF))
		return;
	if (selectedFormCount) {
		mlen = (selectedFormCount >> 5) + 1;
		for (src = 0; src < mlen; src++)
			MarkedStitchMap[src] = 0;
		for (src = 0; src < selectedFormCount; src++) {
			SelectedForm = &formList[selectedFormList[src]];
			if (SelectedForm->fillType || SelectedForm->edgeType) {
				delclps(selectedFormList[src]);
				deltx();
				setr(selectedFormList[src]);
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				SelectedForm->extendedAttribute &= !(AT_UND | AT_CWLK | AT_WALK);
			}
		}
		dst = 0;
		for (src = 0; src < PCSHeader.stitchCount; src++) {
			if (!chkr((StitchBuffer[src].attribute&FRMSK) >> FRMSHFT)) {
				StitchBuffer[dst].attribute = StitchBuffer[src].attribute;
				StitchBuffer[dst].x = StitchBuffer[src].x;
				StitchBuffer[dst++].y = StitchBuffer[src].y;
			}
		}
		PCSHeader.stitchCount = dst;
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			if (!rstMap(IGNOR) && !chku(WRNOF)) {
				trg = (closestFormToCursor << FRMSHFT) | USMSK;
				mlen = StitchBuffer[0].attribute&(FRMSK | USMSK);
				for (src = 0; src < PCSHeader.stitchCount; src++) {
					at = StitchBuffer[src].attribute;
					if (!(at&NOTFRM) && (at&(USMSK | FRMSK)) == trg) {
						tabmsg(IDS_UNFIL);
						setMap(FILMSG);
						okcan();
						setMap(IGNOR);
						return;
					}
				}
			}
			trg = closestFormToCursor << FRMSHFT;
			dst = 0;
			for (src = 0; src < PCSHeader.stitchCount; src++) {
				if ((StitchBuffer[src].attribute&FRMSK) != trg || (StitchBuffer[src].attribute&NOTFRM)) {
					StitchBuffer[dst].attribute = StitchBuffer[src].attribute;
					StitchBuffer[dst].x = StitchBuffer[src].x;
					StitchBuffer[dst++].y = StitchBuffer[src].y;
				}
			}
			delclps(closestFormToCursor);
			deltx();
			SelectedForm->fillType = 0;
			SelectedForm->edgeType = 0;
			SelectedForm->extendedAttribute &= !(AT_UND | AT_CWLK | AT_WALK);
			PCSHeader.stitchCount = dst;
			ritot(PCSHeader.stitchCount);
		}
	}
}

void satzum() {
	rstMap(SHOSAT);
	duzrat();
	sides = satinIndex;
	frmlin(tempPolygon, satinIndex);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, FormPen);
	Polyline(StitchWindowMemDC, formLines, satinIndex);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	drwsat();
}

void rotfrm(unsigned nu0) {
	fPOINT*			flt0;
	fPOINT*			flt1;
	SATCON*			nsac;
	SATCON**		psac;
	unsigned		ind, ine, l_xpnt = nu0;
	unsigned short	tlin;

	fvars(closestFormToCursor);
	flt0 = SelectedForm->vertices;
	sides = SelectedForm->sides;
	flt1 = new fPOINT[sides];
	for (ind = 0; ind < sides; ind++) {
		flt1[ind].x = flt0[ind].x;
		flt1[ind].y = flt0[ind].y;
	}
	for (ind = 0; ind < sides; ind++) {
		flt0[ind].x = flt1[l_xpnt].x;
		flt0[ind].y = flt1[l_xpnt].y;
		l_xpnt = nxt(l_xpnt);
	}
	ine = 0;
	if (SelectedForm->type == SAT) {
		if (SelectedForm->wordParam)
			SelectedForm->wordParam = (SelectedForm->wordParam + SelectedForm->sides
				- nu0) % SelectedForm->sides;
		for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {
			if (currentFormConnections[ind].start != nu0&&currentFormConnections[ind].finish != nu0) {
				currentFormConnections[ine].start = (currentFormConnections[ind].start + sides - nu0) % sides;
				currentFormConnections[ine].finish = (currentFormConnections[ind].finish + sides - nu0) % sides;
				if (currentFormConnections[ine].start > currentFormConnections[ine].finish) {
					tlin = currentFormConnections[ine].start;
					currentFormConnections[ine].start = currentFormConnections[ine].finish;
					currentFormConnections[ind].finish = tlin;
				}
				ine++;
			}
		}
	}
	SelectedForm->satinGuideCount = ine;
	psac = new SATCON*[ine];
	nsac = new SATCON[ine];
	for (ind = 0; ind < ine; ind++) {
		psac[ind] = &nsac[ind];
		nsac[ind].start = currentFormConnections[ind].start;
		nsac[ind].finish = currentFormConnections[ind].finish;
	}
	qsort((void*)psac, ine, 4, scomp);
	for (ind = 0; ind < ine; ind++) {
		currentFormConnections[ind].start = psac[ind]->start;
		currentFormConnections[ind].finish = psac[ind]->finish;
	}
	if (SelectedForm->extendedAttribute&AT_STRT)
		SelectedForm->fillStart = (SelectedForm->fillStart + sides - nu0) % sides;
	if (SelectedForm->extendedAttribute&AT_END)
		SelectedForm->fillEnd = (SelectedForm->fillEnd + sides - nu0) % sides;
	delete[] flt1;
	delete[] nsac;
	delete[] psac;
}


void frm0() {
	if (chkMap(FRMPSEL)) {
		savdo();
		rotfrm(closestVertexToCursor);
		closestVertexToCursor = 0;
		satadj();
		refil();
		coltab();
		setMap(RESTCH);
	}
}

void duinsf() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uninsf() {
	if (rstMap(SHOINSF))
		duinsf();
}

void rinfrm() {
	frmlin(formForInsert->vertices, formForInsert->sides);
	SelectObject(StitchWindowMemDC, FormPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	if (formVertexNext || formForInsert->type != FRMLINE)
		Polyline(StitchWindowMemDC, &formLines[formVertexPrev], 2);
	InsertLine[0].x = formLines[formVertexPrev].x;
	InsertLine[0].y = formLines[formVertexPrev].y;
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	setMap(SHOINSF);
	duinsf();
}

void infrm() {
	if (closat()) {
		formForInsert = &formList[closestFormToCursor];
		fvars(closestFormToCursor);
		if (upsat()) {
			if (!closestVertexToCursor&&formForInsert->type == FRMLINE) {
				formVertexPrev = 0;
				setMap(PRELIN);
			}
			else {
				formVertexPrev = prv(closestVertexToCursor);
				formVertexNext = closestVertexToCursor;
			}
		}
		else {
			formVertexPrev = closestVertexToCursor;
			formVertexNext = nxt(closestVertexToCursor);
		}
		setMap(INSFRM);
		setMap(INIT);
		rinfrm();
	}
}

void setins() {
	px2stch();
	nufpnt(formVertexPrev);
	if (chkMap(PRELIN)) {
		SelectedPoint.x = formForInsert->vertices[0].x;
		SelectedPoint.y = formForInsert->vertices[0].y;
		formForInsert->vertices[0].x = formForInsert->vertices[1].x;
		formForInsert->vertices[0].y = formForInsert->vertices[1].y;
		formForInsert->vertices[1].x = SelectedPoint.x;
		formForInsert->vertices[1].y = SelectedPoint.y;
	}
	else {
		sides = formForInsert->sides;
		formVertexPrev = nxt(formVertexPrev);
		formVertexNext = nxt(formVertexPrev);
	}
	frmlin(formForInsert->vertices, formForInsert->sides);
	InsertLine[0].x = formLines[formVertexPrev].x;
	InsertLine[0].y = formLines[formVertexPrev].y;
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	setMap(INSFRM);
	duinsf();
	setMap(RESTCH);
}

void bdrlin(unsigned strt, unsigned fin, double pd_Size) {
	dPOINT		dif, stp, pnt;
	double		len, tang;
	unsigned	cnt;

	//_asm finit;
	stp.x = 0;
	stp.y = 0;
	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	len = hypot(dif.x, dif.y);
	if (chku(LINSPAC)) {
		cnt = len / pd_Size + 0.5;
		if (cnt) {
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
		}
	}
	else {
		cnt = (len - pd_Size / 2) / pd_Size + 1;
		tang = atan2(dif.y, dif.x);
		stp.x = cos(tang)*pd_Size;
		stp.y = sin(tang)*pd_Size;
	}
	if (cnt) {
		pnt.x = currentFormVertices[strt].x + stp.x;
		pnt.y = currentFormVertices[strt].y + stp.y;
		cnt--;
		while (cnt) {
			oseq[sequenceIndex].x = pnt.x;
			oseq[sequenceIndex++].y = pnt.y;
			pnt.x += stp.x;
			pnt.y += stp.y;
			cnt--;
		}
	}
	oseq[sequenceIndex].x = currentFormVertices[fin].x;
	oseq[sequenceIndex++].y = currentFormVertices[fin].y;
}

void brdfil(double pd_Size) {
	unsigned		ind;
	unsigned short	nlin, tlin;

	if (SelectedForm->extendedAttribute&AT_STRT)
		tlin = SelectedForm->fillStart;
	else
		tlin = getlast();
	sequenceIndex = 0;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	for (ind = 0; ind < (unsigned)sides - 1; ind++) {
		nlin = nxt(tlin);
		bdrlin(tlin, nlin, pd_Size);
		tlin = nlin;
	}
	if (SelectedForm->type != FRMLINE) {
		nlin = nxt(tlin);
		bdrlin(tlin, nlin, pd_Size);
	}
}

void bsizpar() {
	SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	SelectedForm->edgeStitchLen = UserStitchLength;
	SelectedForm->minBorderStitchLen = MinStitchLength;
}

void sbord() {
	fvars(closestFormToCursor);
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGELINE;
	bsizpar();
	refilfn();
}

void bord() {
	unsigned ind;

	if (filmsgs(FML_LIN))
		return;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			SelectedForm->borderColor = ActiveColor;
			sbord();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			SelectedForm->borderColor = ActiveColor;
			sbord();
			coltab();
			setMap(INIT);
			setMap(RESTCH);
		}
	}
}

BOOL ritclp(fPOINT pnt) {
	fPOINT		adj;
	unsigned	ind;

	if (chkmax(ClipStitchCount, sequenceIndex))
		return 1;
	adj.x = pnt.x - clipReference.x;
	adj.y = pnt.y - clipReference.y;
	for (ind = 0; ind < ClipStitchCount; ind++) {
		oseq[sequenceIndex].x = clipFillData[ind].x + adj.x;
		oseq[sequenceIndex++].y = clipFillData[ind].y + adj.y;
	}
	return 0;
}

BOOL clpsid(unsigned strt, unsigned fin) {
	unsigned		ind, cnt;
	fPOINT			dif, stp, pnt;
	double			len, tdub;
	fPOINTATTR		rpnt;

	rpnt.x = ClipRect.left;
	rpnt.y = ClipRect.bottom;
	pnt.x = currentFormVertices[strt].x;
	pnt.y = currentFormVertices[fin].y;
	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	len = hypot(dif.x, dif.y);
	RotationAngle = atan2(dif.y, dif.x);
	rotang1(rpnt, &clipReference);
	cnt = len / ClipRectSize.cx;
	if (cnt) {
		if (cnt > 1)
			tdub = ((len - cnt*ClipRectSize.cx) / (cnt - 1) + ClipRectSize.cx) / len;
		else
			tdub = (len - ClipRectSize.cx) / 2;
		stp.x = dif.x*tdub;
		stp.y = dif.y*tdub;
		pnt.x = currentFormVertices[strt].x;
		pnt.y = currentFormVertices[strt].y;
		RotationAngle = atan2(dif.y, dif.x);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(clipReversedData[ind], &clipFillData[ind]);
		for (ind = 0; ind < cnt; ind++) {
			if (ritclp(pnt))
				break;
			pnt.x += stp.x;
			pnt.y += stp.y;
		}
		return 1;
	}
	return 0;
}

void linsid() {
	fPOINT		dif;
	unsigned	cnt, ind;
	double		len;

	dif.x = currentFormVertices[currentSide + 1].x - SelectedPoint.x;
	dif.y = currentFormVertices[currentSide + 1].y - SelectedPoint.y;
	len = hypot(dif.x, dif.y);
	cnt = len / ClipRectSize.cx;
	if (cnt) {
		RotationAngle = clipAngle;
		rotangf(borderClipReference, &clipReference);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(clipReversedData[ind], &clipFillData[ind]);
		for (ind = 0; ind < cnt; ind++) {
			ritclp(SelectedPoint);
			SelectedPoint.x += vector0.x;
			SelectedPoint.y += vector0.y;
		}
	}
}

BOOL nupnt() {
	double		len, dif;
	unsigned	ind;

	moveToPoint.x = currentFormVertices[currentSide + 2].x;
	moveToPoint.y = currentFormVertices[currentSide + 2].y;
	len = hypot(moveToPoint.x - SelectedPoint.x, moveToPoint.y - SelectedPoint.y);
	if (len > ClipRectSize.cx) {
		for (ind = 0; ind < 10; ind++) {
			len = hypot(moveToPoint.x - SelectedPoint.x, moveToPoint.y - SelectedPoint.y);
			dif = ClipRectSize.cx - len;
			moveToPoint.x += dif*cosAngle;
			moveToPoint.y += dif*sinAngle;
			if (fabs(dif) < 0.01)
				break;
		}
		return 1;
	}
	return 0;
}

void lincrnr() {
	dPOINT		dif;
	unsigned	ind;

	sinAngle = sin(clipAngle);
	cosAngle = cos(clipAngle);
	if (nupnt()) {
		dif.x = moveToPoint.x - SelectedPoint.x;
		dif.y = moveToPoint.y - SelectedPoint.y;
		RotationAngle = atan2(dif.y, dif.x);
		rotangf(borderClipReference, &clipReference);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(clipReversedData[ind], &clipFillData[ind]);
		ritclp(SelectedPoint);
		SelectedPoint.x = moveToPoint.x;
		SelectedPoint.y = moveToPoint.y;
	}
}

void durev() {
	unsigned	ind;
	double		mid;

	mid = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	if (ClipBuffer[0].x > mid) {
		for (ind = 0; ind < ClipStitchCount; ind++) {
			clipReversedData[ind].x = ClipRect.right - ClipBuffer[ind].x;
			clipReversedData[ind].y = ClipBuffer[ind].y;
		}
	}
	else {
		for (ind = 0; ind < ClipStitchCount; ind++) {
			clipReversedData[ind].x = ClipBuffer[ind].x;
			clipReversedData[ind].y = ClipBuffer[ind].y;
		}
	}
}

void setvct(unsigned strt, unsigned fin) {
	clipAngle = atan2(currentFormVertices[fin].y - currentFormVertices[strt].y, currentFormVertices[fin].x - currentFormVertices[strt].x);
	vector0.x = ClipRectSize.cx*cos(clipAngle);
	// ToDo - is this line below correct?
	vector0.y = ClipRectSize.cx*sin(clipAngle);
}

void clpbrd(unsigned short strtlin) {
	unsigned		ind, ref;
	unsigned short	nlin;

	sequenceIndex = 0;
	rstMap(CLPBAK);
	plen = ClipRectSize.cx / 2;
	plen2 = ClipRectSize.cx;
	clipFillData = new fPOINT[ClipStitchCount];
	clipReversedData = new fPOINT[ClipStitchCount];
	RotationCenter.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	clipReference.y = RotationCenter.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
	clipReference.x = ClipRect.left;
	durev();
	ref = 0;
	if (SelectedForm->type == FRMLINE) {
		SelectedPoint.x = currentFormVertices[0].x;
		SelectedPoint.y = currentFormVertices[0].y;
		setvct(0, 1);
		borderClipReference.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
		// ToDo - is this line below correct?
		borderClipReference.x = (ClipRect.top = ClipRect.left) / 2 + ClipRect.left;
		for (currentSide = 0; currentSide < (unsigned)sides - 2; currentSide++) {
			linsid();
			setvct(currentSide + 1, currentSide + 2);
			lincrnr();
		}
		linsid();
	}
	else {
		clpout();
		ref = strtlin;
		for (ind = 0; ind < sides; ind++) {
			nlin = prv(strtlin);
			if (clpsid(ref, nlin))
				ref = nlin;
			strtlin = nlin;
		}
	}
	delete[] clipFillData;
	delete[] clipReversedData;
}

void outfn(unsigned strt, unsigned fin, double satwid) {
	double		l_ang;
	double		len;
	double		xOffset, yOffset;

	if (fabs(formAngles[strt]) < TINY && fabs(formAngles[fin]) < TINY) {
		xOffset = 0;
		yOffset = satwid;
	} else {
#define SATHRESH 10

		l_ang = (formAngles[fin] - formAngles[strt]) / 2;
		len = satwid / cos(l_ang);
		if (len < -satwid*SATHRESH)
			len = -satwid*SATHRESH;
		if (len > satwid*SATHRESH)
			len = satwid*SATHRESH;
		l_ang += formAngles[strt] + PI / 2;
		xOffset = len*cos(l_ang);
		yOffset = len*sin(l_ang);
	}
	insidePoints[fin].x = currentFormVertices[fin].x - xOffset;
	insidePoints[fin].y = currentFormVertices[fin].y - yOffset;
	outsidePoints[fin].x = currentFormVertices[fin].x + xOffset;
	outsidePoints[fin].y = currentFormVertices[fin].y + yOffset;
}

void duangs() {
	unsigned ind;

	for (ind = 0; ind < (unsigned)sides - 1; ind++)
		formAngles[ind] = atan2(currentFormVertices[ind + 1].y - currentFormVertices[ind].y, currentFormVertices[ind + 1].x - currentFormVertices[ind].x);
	formAngles[ind] = atan2(currentFormVertices[0].y - currentFormVertices[ind].y, currentFormVertices[0].x - currentFormVertices[ind].x);
}

void satout(double satwid) {
	unsigned	ind;
	unsigned	cnt;

	if (sides) {
		duangs();
		outsidePoints = outsidePointList;
		insidePoints = insidePointList;
		for (ind = 0; ind < (unsigned)sides - 1; ind++)
			outfn(ind, ind + 1, 0.1);
		cnt = 0;
		for (ind = 0; ind < sides; ind++)
		{
			if (cisin(insidePoints[ind].x, insidePoints[ind].y))
				cnt++;
		}
		satwid /= 2;
		for (ind = 0; ind < (unsigned)sides - 1; ind++)
			outfn(ind, ind + 1, satwid);
		outfn(ind, 0, satwid);
		rstMap(INDIR);
		if (cnt < (unsigned)sides >> 1)
		{
			setMap(INDIR);
			outsidePoints = insidePointList;
			insidePoints = outsidePointList;
		}
	}
}

void clpout() {
	if (SelectedForm->type == FRMLINE)
		satout(plen);
	else {
		satout(ClipRectSize.cy);
		insidePoints = SelectedForm->vertices;
	}
}

void fsclp() {
	unsigned	ind;

	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGECLIP;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(closestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = ClipRectSize.cx;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = ClipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = ClipBuffer[ind].y;
	}
	plen = ClipRectSize.cy / 2;
	clpout();
	refilfn();
}

void fclp() {
	unsigned ind;

	if (filmsgs(FML_CLP))
		return;
	if (OpenClipboard(ThredWindow)) {
		fvars(closestFormToCursor);
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			savdo();
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (selectedFormCount) {
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						fsclp();
					}
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
				else {
					if (chkMap(FORMSEL)) {
						fsclp();
						setMap(INIT);
						coltab();
						setMap(RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void filinsb(dPOINT pnt) {
	double		len;
	dPOINT		dif, stp;
	unsigned	cnt;

	dif.x = pnt.x - SelectedPoint.x;
	dif.y = pnt.y - SelectedPoint.y;
	len = hypot(dif.x, dif.y);
	if (len > MAXSTCH) {
		cnt = len / MAXSTCH + 1;
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		cnt--;
		if (chkmax(cnt, sequenceIndex))
			return;
		while (cnt) {
			SelectedPoint.x += stp.x;
			SelectedPoint.y += stp.y;
			oseq[sequenceIndex].x = SelectedPoint.x;
			oseq[sequenceIndex++].y = SelectedPoint.y;
			cnt--;
		}
	}
	if (sequenceIndex & 0xffff0000)
		return;
	oseq[sequenceIndex].x = pnt.x;
	oseq[sequenceIndex++].y = pnt.y;
	SelectedPoint.x = pnt.x;
	SelectedPoint.y = pnt.y;
}

BOOL chkbak(dPOINT pnt) {
	unsigned ind;
	double		len;

	for (ind = 0; ind < 8; ind++) {
		len = hypot(satinFillBackup[ind].x - pnt.x, satinFillBackup[ind].y - pnt.y);
		if (len < stitchSpace)
			return 1;
	}
	return 0;
}

BOOL linx(fPOINT* p_flt, unsigned strt, unsigned fin, dPOINT* npnt) {
	dPOINT	dif;
	dPOINT	tdub;

	dif.x = outsidePoints[strt].x - p_flt[strt].x;
	dif.y = outsidePoints[strt].y - p_flt[strt].y;
	if (!dif.x && !dif.y)
		return 0;
	tdub.x = p_flt[strt].x;
	tdub.y = p_flt[strt].y;
	if (dif.x) {
		if (proj(tdub, dif.y / dif.x, outsidePoints[fin], p_flt[fin], npnt))
			return 1;
		else
			return 0;
	}
	else {
		if (projv(tdub.x, p_flt[fin], outsidePoints[fin], npnt))
			return 1;
		else
			return 0;
	}
}

void filinsbw(dPOINT pnt) {
	satinFillBackup[fillBackupIndex].x = pnt.x;
	satinFillBackup[fillBackupIndex++].y = pnt.y;
	fillBackupIndex &= 0x7;
	filinsb(pnt);
}

void sbfn(fPOINT* p_flt, unsigned start, unsigned finish) {
	dPOINT		idif, odif, istp, ostp, l_opnt, ipnt;
	dPOINT		bdif, bstp, bpnt;
	dPOINT		npnt;
	double		ilen, olen, blen;
	unsigned	cnt, iflg, oflg, bcnt, ind, xflg;

	if (!setMap(SAT1)) {
		SelectedPoint.x = p_flt[start].x;
		SelectedPoint.y = p_flt[start].y;
	}
	idif.x = p_flt[finish].x - p_flt[start].x;
	idif.y = p_flt[finish].y - p_flt[start].y;
	odif.x = outsidePoints[finish].x - outsidePoints[start].x;
	odif.y = outsidePoints[finish].y - outsidePoints[start].y;
	ilen = hypot(idif.x, idif.y);
	olen = hypot(odif.x, odif.y);
	ipnt.x = p_flt[start].x;
	ipnt.y = p_flt[start].y;
	l_opnt.x = outsidePoints[start].x;
	l_opnt.y = outsidePoints[start].y;
	xflg = fillBackupIndex = iflg = oflg = bcnt = 0;
	for (ind = 0; ind < 8; ind++) {
		satinFillBackup[ind].x = (float)1e12;
		satinFillBackup[ind].y = (float)1e12;
	}
	if (olen > ilen) {
		cnt = olen / stitchSpace;
		iflg = 1;
		if (linx(p_flt, start, finish, &npnt)) {
			xflg = 1;
			idif.x = idif.y = ilen = 0;
			ipnt.x = npnt.x;
			ipnt.y = npnt.y;
		}
	}
	else {
		cnt = ilen / stitchSpace;
		oflg = 1;
		if (linx(p_flt, start, finish, &npnt)) {
			xflg = 1;
			odif.x = odif.y = olen = 0;
			l_opnt.x = npnt.x;
			l_opnt.y = npnt.y;
		}
	}
	if (!cnt)
		cnt = 1;
	if (chkmax(cnt, sequenceIndex))
		return;
	istp.x = idif.x / cnt;
	istp.y = idif.y / cnt;
	ostp.x = odif.x / cnt;
	ostp.y = odif.y / cnt;
	for (ind = 0; ind < cnt; ind++) {
		ipnt.x += istp.x;
		ipnt.y += istp.y;
		l_opnt.x += ostp.x;
		l_opnt.y += ostp.y;
		if (toglMap(FILDIR)) {
			if (iflg) {
				bdif.x = ipnt.x - SelectedPoint.x;
				bdif.y = ipnt.y - SelectedPoint.y;
				blen = hypot(bdif.x, bdif.y);
				bcnt = blen / stitchSpace;
				bstp.x = bdif.x / bcnt;
				bstp.y = bdif.y / bcnt;
				bpnt.x = ipnt.x;
				bpnt.y = ipnt.y;
				while (chkbak(bpnt)) {
					bpnt.x -= bstp.x;
					bpnt.y -= bstp.y;
				}
				filinsbw(bpnt);
			}
			else
				filinsb(ipnt);
		}
		else {
			if (oflg) {
				bdif.x = l_opnt.x - SelectedPoint.x;
				bdif.y = l_opnt.y - SelectedPoint.y;
				blen = hypot(bdif.x, bdif.y);
				bcnt = blen / stitchSpace;
				bstp.x = bdif.x / bcnt;
				bstp.y = bdif.y / bcnt;
				bpnt.x = l_opnt.x;
				bpnt.y = l_opnt.y;
				while (chkbak(bpnt)) {
					bpnt.x -= bstp.x;
					bpnt.y -= bstp.y;
				}
				filinsbw(bpnt);
			}
			else
				filinsb(l_opnt);
		}
	}
}

void sfn(unsigned short startLine) {
	unsigned ind;
	unsigned short nextLine;

	for (ind = 0; ind < SelectedForm->sides; ind++) {
		nextLine = nxt(startLine);
		sbfn(insidePoints, startLine, nextLine);
		startLine = nextLine;
	}
	oseq[0].x = oseq[sequenceIndex - 1].x;
	oseq[0].y = oseq[sequenceIndex - 1].y;
	if (sequenceIndex > MAXSEQ - 2)
		sequenceIndex = MAXSEQ - 2;
}

void sbrd() {
	double			tspac;
	unsigned		strt;

	strt = getlast();
	tspac = stitchSpace;
	rstMap(SAT1);
	rstMap(FILDIR);
	sequenceIndex = 1;
	if (SelectedForm->edgeType&EGUND) {
		stitchSpace = USPAC;
		satout(plen*URAT);
		sfn(strt);
		setMap(FILDIR);
		sfn(strt);
	}
	fvars(closestFormToCursor);
	satout(plen);
	stitchSpace = SelectedForm->edgeSpacing;
	sfn(strt);
	stitchSpace = tspac;
}

void rfn(unsigned tlin) {
	unsigned ind, nlin;

	for (ind = 0; ind < SelectedForm->sides; ind++) {
		nlin = nxt(tlin);
		sbfn(insidePoints, tlin, nlin);
		tlin = nlin;
	}
}

void rbrd() {
	unsigned short	tlin = getlast();
	double			tspac;

	rstMap(SAT1);
	rstMap(FILDIR);
	tspac = stitchSpace;
	stitchSpace = USPAC;
	rfn(tlin);
	setMap(FILDIR);
	rfn(tlin);
	satout(plen);
	stitchSpace = tspac;
	rfn(tlin);
}

void satends(unsigned blnt) {
	fPOINT		step;

	if (blnt&SBLNT) {
		step.x = sin(formAngles[0])*plen / 2;
		step.y = cos(formAngles[0])*plen / 2;
		if (chkMap(INDIR))
		{
			step.x = -step.x;
			step.y = -step.y;
		}
		insidePoints[0].x = SelectedForm->vertices[0].x + step.x;
		insidePoints[0].y = SelectedForm->vertices[0].y - step.y;
		outsidePoints[0].x = SelectedForm->vertices[0].x - step.x;
		outsidePoints[0].y = SelectedForm->vertices[0].y + step.y;
	}
	else {
		insidePoints[0].x = outsidePoints[0].x = currentFormVertices[0].x;
		insidePoints[0].y = outsidePoints[0].y = currentFormVertices[0].y;
	}
	if (blnt&FBLNT) {
		step.x = sin(formAngles[sides - 2])*plen / 2;
		step.y = cos(formAngles[sides - 2])*plen / 2;
		if (chkMap(INDIR))
		{
			step.x = -step.x;
			step.y = -step.y;
		}
		insidePoints[sides - 1].x = SelectedForm->vertices[sides - 1].x + step.x;
		insidePoints[sides - 1].y = SelectedForm->vertices[sides - 1].y - step.y;
		outsidePoints[sides - 1].x = SelectedForm->vertices[sides - 1].x - step.x;
		outsidePoints[sides - 1].y = SelectedForm->vertices[sides - 1].y + step.y;
	}
	else {
		insidePoints[sides - 1].x = outsidePoints[sides - 1].x = currentFormVertices[sides - 1].x;
		insidePoints[sides - 1].y = outsidePoints[sides - 1].y = currentFormVertices[sides - 1].y;
	}
}

void slbrd() {
	unsigned	ind;
	double		tspac = stitchSpace;

	sequenceIndex = 0;
	if (SelectedForm->edgeType&EGUND) {
		plen = SelectedForm->borderSize*URAT;
		satout(plen);
		satends(SelectedForm->attribute);
		rstMap(SAT1);
		rstMap(FILDIR);
		stitchSpace = USPAC;
		for (ind = 0; ind < (unsigned)SelectedForm->sides - 1; ind++)
			sbfn(insidePoints, ind, ind + 1);
		toglMap(FILDIR);
		for (ind = SelectedForm->sides - 1; ind; ind--)
			sbfn(insidePoints, ind, ind - 1);
	}
	plen = SelectedForm->borderSize;
	satout(plen);
	satends(SelectedForm->attribute);
	stitchSpace = SelectedForm->edgeSpacing;
	rstMap(SAT1);
	for (ind = 0; ind < (unsigned)SelectedForm->sides - 1; ind++)
		sbfn(insidePoints, ind, ind + 1);
	stitchSpace = tspac;
}

void satsbrd() {
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGEANGSAT;
	if (chku(DUND))
		SelectedForm->edgeType |= EGUND;
	bsizpar();
	SelectedForm->borderSize = borderWidth;
	SelectedForm->edgeSpacing = stitchSpace / 2;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void satbrd() {
	unsigned ind;

	if (filmsgs(FML_ANGS))
		return;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (chku(BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satsbrd();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			fvars(closestFormToCursor);
			if (chku(BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satsbrd();
			setMap(INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			setMap(RESTCH);
		}
	}
}

void lapbrd() {
	double		tsiz;
	unsigned	ind;

	sequenceIndex = 0;
	tsiz = UserStitchLength;
	UserStitchLength = APSPAC;
	for (ind = 0; ind < (unsigned)sides - 1; ind++)
		bdrlin(ind, ind + 1, APSPAC);
	for (ind = sides - 1; ind; ind--)
		bdrlin(ind, ind - 1, APSPAC);
	UserStitchLength = tsiz;
}

void apbrd() {
	unsigned		ind;
	unsigned short	nlin, tlin = 0;

	sequenceIndex = 0;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	for (ind = 0; ind < (unsigned)sides << 1; ind++) {
		nlin = nxt(tlin);
		bdrlin(tlin, nlin, APSPAC);
		tlin = nlin;
	}
}

void sapliq()
{
	fvars(closestFormToCursor);
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGEAPPL;
	if (chku(DUND))
		SelectedForm->edgeType |= EGUND;
	SelectedForm->edgeSpacing = stitchSpace / 2;
	SelectedForm->borderSize = IniFile.borderWidth;
	bsizpar();
	SelectedForm->borderColor = ActiveColor | (UnderlayColor << 4);
	if (SelectedForm->type != FRMLINE)
	{
		if (SelectedForm->fillType == SAT&&SelectedForm->satinGuideCount)
			delsac(closestFormToCursor);
	}
	SelectedForm->fillType = 0;
	SelectedForm->type = FRMFPOLY;
	refilfn();
}

void apliq() {
	unsigned ind;

	if (filmsgs(FML_APLQ))
		return;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (chku(BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			sapliq();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			if (chku(BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			sapliq();
			setMap(INIT);
			coltab();
			setMap(RESTCH);
		}
	}
}

void setap() {
	TCHAR	buf[HBUFSIZ];

	UnderlayColor = ActiveColor;
	LoadString(ThredInstance, IDS_APCOL, buf, HBUFSIZ);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), buf, UnderlayColor + 1);
	shoMsg(MsgBuffer);
}

void maxtsiz(TCHAR* str, POINT* pt) {
	SIZE	tsiz;

	GetTextExtentPoint32(StitchWindowMemDC, str, strlen(str), &tsiz);
	pt->y = tsiz.cy;
	if (tsiz.cx > pt->x)
		pt->x = tsiz.cx;
}

void maxwid(unsigned strt, unsigned fin) {
	POINT	pnt;

	pnt.x = 0;
	pnt.y = 0;
	while (strt <= fin)
		maxtsiz(StringTable[strt++], &pnt);
	preferenceWindowTextWidth = pnt.x + 6;
}

HWND txtwin(TCHAR* str, RECT loc) {
	if (chkMap(REFCNT))
	{
		maxtsiz(str, &leftWindowSize);
		return 0;
	}
	return CreateWindow(
		"STATIC",
		str,
		WS_CHILD | WS_VISIBLE,
		loc.left,
		loc.top,
		loc.right - loc.left,
		loc.bottom - loc.top,
		FormDataSheet,
		NULL,
		ThredInstance,
		NULL);
}

HWND txtrwin(TCHAR* str, RECT loc) {
	if (chkMap(REFCNT))
	{
		maxtsiz(str, &rightWindowSize);
		return 0;
	}
	return CreateWindow(
		"STATIC",
		str,
		SS_NOTIFY | WS_BORDER | WS_CHILD | WS_VISIBLE,
		loc.left,
		loc.top,
		loc.right - loc.left,
		loc.bottom - loc.top,
		FormDataSheet,
		NULL,
		ThredInstance,
		NULL);
}

HWND numwin(TCHAR* str, RECT loc) {
	if (chkMap(REFCNT))
	{
		maxtsiz(str, &rightWindowSize);
		return 0;
	}
	return CreateWindow(
		"STATIC",
		str,
		SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
		loc.left,
		loc.top,
		loc.right - loc.left,
		loc.bottom - loc.top,
		FormDataSheet,
		NULL,
		ThredInstance,
		NULL);
}

void nxtlin() {
	formMenuEntryCount++;
	leftWindowCoords.top += leftWindowSize.y;
	leftWindowCoords.bottom += leftWindowSize.y;
	rightWindowCoords.top += rightWindowSize.y;
	rightWindowCoords.bottom += rightWindowSize.y;;
}

BOOL CALLBACK chenum(HWND hwnd, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	return DestroyWindow(hwnd);
}

void refrmfn()
{
	char*		pchr;
	unsigned	cod, fpnt;

	cod = SelectedForm->edgeType&NEGUND;
	if (cod >= EDGELAST) {
		cod = EDGELAST - 1;
	}
	fpnt = cod - 1;
	leftWindowCoords.top = rightWindowCoords.top = 3;
	leftWindowCoords.bottom = rightWindowCoords.bottom = 3 + leftWindowSize.y;
	leftWindowCoords.left = 3;
	leftWindowCoords.right = 3 + leftWindowSize.x;
	rightWindowCoords.left = 6 + leftWindowSize.x;
	rightWindowCoords.right = 6 + leftWindowSize.x + rightWindowSize.x + 6;
	thTxt[LFRM] = txtwin(StringTable[STR_TXT0], leftWindowCoords);
	if (SelectedForm->type == FRMLINE)
		thDat[LFRM] = txtrwin(StringTable[STR_EDG1], rightWindowCoords);
	else
		thDat[LFRM] = txtrwin(StringTable[STR_FREH], rightWindowCoords);
	nxtlin();
	thTxt[LLAYR] = txtwin(StringTable[STR_TXT1], leftWindowCoords);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", (SelectedForm->attribute&FRMLMSK) >> 1);
	thDat[LLAYR] = txtrwin(MsgBuffer, rightWindowCoords);
	nxtlin();
	if (SelectedForm->type != FRMLINE)
	{
		thTxt[LCWLK] = txtwin(StringTable[STR_CWLK], leftWindowCoords);
		if (SelectedForm->extendedAttribute&AT_CWLK)
			thDat[LCWLK] = txtrwin(StringTable[STR_ON], rightWindowCoords);
		else
			thDat[LCWLK] = txtrwin(StringTable[STR_OFF], rightWindowCoords);
		nxtlin();
		thTxt[LWALK] = txtwin(StringTable[STR_WALK], leftWindowCoords);
		if (SelectedForm->extendedAttribute&AT_WALK)
			thDat[LWALK] = txtrwin(StringTable[STR_ON], rightWindowCoords);
		else
			thDat[LWALK] = txtrwin(StringTable[STR_OFF], rightWindowCoords);
		nxtlin();
		thTxt[LUND] = txtwin(StringTable[STR_UND], leftWindowCoords);
		if (SelectedForm->extendedAttribute&AT_UND)
			thDat[LUND] = txtrwin(StringTable[STR_ON], rightWindowCoords);
		else
			thDat[LUND] = txtrwin(StringTable[STR_OFF], rightWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&(AT_WALK | AT_UND | AT_CWLK))
		{
			thTxt[LUNDCOL] = txtwin(StringTable[STR_UNDCOL], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->underlayColor + 1);
			thDat[LUNDCOL] = txtrwin(MsgBuffer, rightWindowCoords);
			nxtlin();
			thTxt[LULEN] = txtwin(StringTable[STR_ULEN], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlayStitchLen / PFGRAN);
			thDat[LULEN] = txtrwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		thTxt[LWLKIND] = txtwin(StringTable[STR_UWLKIND], leftWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlayIndent / PFGRAN);
		thDat[LWLKIND] = txtrwin(MsgBuffer, rightWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&AT_UND)
		{
			thTxt[LUSPAC] = txtwin(StringTable[STR_FUSPAC], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlaySpacing / PFGRAN);
			thDat[LUSPAC] = txtrwin(MsgBuffer, rightWindowCoords);
			nxtlin();
			thTxt[LUANG] = txtwin(StringTable[STR_FUANG], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlayStitchAngle * 180 / PI);
			thDat[LUANG] = txtrwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
	}
	thTxt[LFRMFIL] = txtwin(StringTable[STR_TXT2], leftWindowCoords);
	thDat[LFRMFIL] = txtrwin(StringTable[STR_FIL0 + SelectedForm->fillType], rightWindowCoords);
	nxtlin();
	if (SelectedForm->fillType) {
		thTxt[LFRMCOL] = txtwin(StringTable[STR_TXT3], leftWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillColor + 1);
		thDat[LFRMCOL] = numwin(MsgBuffer, rightWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == FTHF) {
			thTxt[LFTHCOL] = txtwin(StringTable[STR_FTHCOL], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.color + 1);
			thDat[LFTHCOL] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
			thTxt[LFTHTYP] = txtwin(StringTable[STR_FTHTYP], leftWindowCoords);
			thDat[LFTHTYP] = numwin(StringTable[STR_FTH0 + SelectedForm->fillInfo.feather.fillType - 1], rightWindowCoords);
			nxtlin();
			thTxt[LFTHBLND] = txtwin(StringTable[STR_FTHBLND], leftWindowCoords);
			if (SelectedForm->extendedAttribute&AT_FTHBLND)
				pchr = StringTable[STR_ON];
			else
				pchr = StringTable[STR_OFF];
			thDat[LFTHBLND] = txtrwin(pchr, rightWindowCoords);
			nxtlin();
			if (!(SelectedForm->extendedAttribute&AT_FTHBLND)) {
				// ToDo - check whether we are doing 'feather down' or 'feather both'
				// only 'feather both' exists in the string table
				thTxt[LFTHDWN] = txtwin(StringTable[STR_FTHBOTH], leftWindowCoords);
				if (SelectedForm->extendedAttribute&(AT_FTHDWN))
					pchr = StringTable[STR_ON];
				else
					pchr = StringTable[STR_OFF];
				thDat[LFTHDWN] = txtrwin(pchr, rightWindowCoords);
				nxtlin();
				if (!(SelectedForm->extendedAttribute&AT_FTHDWN)) {
					thTxt[LFTHUP] = txtwin(StringTable[STR_FTHUP], leftWindowCoords);
					if (SelectedForm->extendedAttribute&AT_FTHUP)
						pchr = StringTable[STR_ON];
					else
						pchr = StringTable[STR_OFF];
					thDat[LFTHUP] = txtrwin(pchr, rightWindowCoords);
					nxtlin();
				}
			}
			thTxt[LFTHUPCNT] = txtwin(StringTable[STR_FTHUPCNT], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.upCount);
			thDat[LFTHUPCNT] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
			thTxt[LFTHDWNCNT] = txtwin(StringTable[STR_FTHDWNCNT], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.downCount);
			thDat[LFTHDWNCNT] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
			thTxt[LFTHSIZ] = txtwin(StringTable[STR_FTHSIZ], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->fillInfo.feather.ratio);
			thDat[LFTHSIZ] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
			if (SelectedForm->fillInfo.feather.fillType == FTHPSG) {
				thTxt[LFTHNUM] = txtwin(StringTable[STR_FTHNUM], leftWindowCoords);
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.count);
				thDat[LFTHNUM] = numwin(MsgBuffer, rightWindowCoords);
				nxtlin();
			}
			thTxt[LFTHFLR] = txtwin(StringTable[STR_FTHFLR], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->fillInfo.feather.minStitchSize / PFGRAN);
			thDat[LFTHFLR] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType != CLPF) {
			thTxt[LFRMSPAC] = txtwin(StringTable[STR_TXT4], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->fillSpacing / PFGRAN);
			thDat[LFRMSPAC] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (istx(closestFormToCursor))
		{
			thTxt[LTXOF] = txtwin(StringTable[STR_TXOF], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->txof / PFGRAN);
			thDat[LTXOF] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		thTxt[LMAXFIL] = txtwin(StringTable[STR_TXT20], leftWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->maxFillStitchLen / PFGRAN);
		thDat[LMAXFIL] = numwin(MsgBuffer, rightWindowCoords);
		nxtlin();
		if (!isclp(closestFormToCursor) && !istx(closestFormToCursor)) {
			thTxt[LFRMLEN] = txtwin(StringTable[STR_TXT5], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->lengthOrCount.stitchLength / PFGRAN);
			thDat[LFRMLEN] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		thTxt[LMINFIL] = txtwin(StringTable[STR_TXT21], leftWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->minFillStitchLen / PFGRAN);
		thDat[LMINFIL] = numwin(MsgBuffer, rightWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == ANGF || SelectedForm->fillType == TXANGF) {
			thTxt[LFRMANG] = txtwin(StringTable[STR_TXT6], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->angleOrClipData.angle * 180 / PI);
			thDat[LFRMANG] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == ANGCLPF) {
			thTxt[LSACANG] = txtwin(StringTable[STR_TXT6], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->satinOrAngle.angle * 180 / PI);
			thDat[LSACANG] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VCLPF || SelectedForm->fillType == HCLPF || SelectedForm->fillType == ANGCLPF) {
			thTxt[LFRMFAZ] = txtwin(StringTable[STR_TXT18], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->wordParam);
			thDat[LFRMFAZ] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VRTF || SelectedForm->fillType == HORF || SelectedForm->fillType == ANGF || istx(closestFormToCursor)) {
			thTxt[LBFILSQR] = txtwin(StringTable[STR_PRF2], leftWindowCoords);
			if (SelectedForm->extendedAttribute&AT_SQR)
				strcpy_s(MsgBuffer, StringTable[STR_SQR]);
			else
				strcpy_s(MsgBuffer, StringTable[STR_PNTD]);
			thDat[LBFILSQR] = txtrwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
	}
	thTxt[LFSTRT] = txtwin(StringTable[STR_FSTRT], leftWindowCoords);
	if (SelectedForm->extendedAttribute&AT_STRT)
		strcpy_s(MsgBuffer, StringTable[STR_ON]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_OFF]);
	thDat[LFSTRT] = txtrwin(MsgBuffer, rightWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_STRT)
	{
		thTxt[LDSTRT] = txtwin(StringTable[STR_FSTRT], leftWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillStart);
		thDat[LDSTRT] = numwin(MsgBuffer, rightWindowCoords);
		nxtlin();
	}
	thTxt[LFEND] = txtwin(StringTable[STR_FEND], leftWindowCoords);
	if (SelectedForm->extendedAttribute&AT_END)
		strcpy_s(MsgBuffer, StringTable[STR_ON]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_OFF]);
	thDat[LFEND] = txtrwin(MsgBuffer, rightWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_END)
	{
		thTxt[LDEND] = txtwin(StringTable[STR_FEND], leftWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillEnd);
		thDat[LDEND] = numwin(MsgBuffer, rightWindowCoords);
		nxtlin();
	}
	thTxt[LBRD] = txtwin(StringTable[STR_TXT7], leftWindowCoords);
	thDat[LBRD] = txtrwin(StringTable[STR_EDG0 + cod], rightWindowCoords);
	nxtlin();
	if (cod) {
		thTxt[LBRDCOL] = txtwin(StringTable[STR_TXT8], leftWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", (SelectedForm->borderColor & 0xf) + 1);
		thDat[LBRDCOL] = numwin(MsgBuffer, rightWindowCoords);
		nxtlin();
		if (egaray[fpnt] & BESPAC) {
			thTxt[LBRDSPAC] = txtwin(StringTable[STR_TXT9], leftWindowCoords);
			if (cod == EDGEPROPSAT || cod == EDGEOCHAIN || cod == EDGELCHAIN)
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeSpacing / PFGRAN);
			else
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeSpacing / PFGRAN * 2);
			thDat[LBRDSPAC] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BPICSPAC) {
			thTxt[LBRDPIC] = txtwin(StringTable[STR_TXT16], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeSpacing / PFGRAN);
			thDat[LBRDPIC] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BEMAX) {
			thTxt[LMAXBRD] = txtwin(StringTable[STR_TXT22], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->maxBorderStitchLen / PFGRAN);
			thDat[LMAXBRD] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BELEN) {
			thTxt[LBRDLEN] = txtwin(StringTable[STR_TXT10], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeStitchLen / PFGRAN);
			thDat[LBRDLEN] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BEMIN) {
			thTxt[LMINBRD] = txtwin(StringTable[STR_TXT23], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->minBorderStitchLen / PFGRAN);
			thDat[LMINBRD] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BESIZ) {
			thTxt[LBRDSIZ] = txtwin(StringTable[STR_TXT11], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->borderSize / PFGRAN);
			thDat[LBRDSIZ] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BRDPOS) {
			thTxt[LBRDPOS] = txtwin(StringTable[STR_TXT18], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeStitchLen);
			thDat[LBRDPOS] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & CHNPOS) {
			thTxt[LBRDPOS] = txtwin(StringTable[STR_TXT19], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeStitchLen);
			thDat[LBRDPOS] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (cod == EDGEAPPL) {
			thTxt[LAPCOL] = txtwin(StringTable[STR_TXT12], leftWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", (SelectedForm->borderColor >> 4) + 1);
			thDat[LAPCOL] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (cod == EDGEANGSAT || cod == EDGEAPPL || cod == EDGEPROPSAT) {
			thTxt[LBRDUND] = txtwin(StringTable[STR_TXT17], leftWindowCoords);
			if (SelectedForm->edgeType&EGUND)
				thDat[LBRDUND] = numwin(StringTable[STR_ON], rightWindowCoords);
			else
				thDat[LBRDUND] = numwin(StringTable[STR_OFF], rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BCNRSIZ) {
			if (SelectedForm->edgeType == EDGEBHOL)
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", getblen() / PFGRAN);
			else
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", getplen() / PFGRAN);
			thTxt[LBCSIZ] = txtwin(StringTable[STR_TXT13], leftWindowCoords);
			thDat[LBCSIZ] = numwin(MsgBuffer, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->type == FRMLINE&&egaray[fpnt] & BRDEND) {
			thTxt[LBSTRT] = txtwin(StringTable[STR_TXT14], leftWindowCoords);
			if (SelectedForm->attribute&SBLNT)
				thDat[LBSTRT] = numwin(StringTable[STR_BLUNT], rightWindowCoords);
			else
				thDat[LBSTRT] = numwin(StringTable[STR_TAPR], rightWindowCoords);
			nxtlin();
			thTxt[LBFIN] = txtwin(StringTable[STR_TXT15], leftWindowCoords);
			if (SelectedForm->attribute&FBLNT)
				thDat[LBFIN] = numwin(StringTable[STR_BLUNT], rightWindowCoords);
			else
				thDat[LBFIN] = numwin(StringTable[STR_TAPR], rightWindowCoords);
			nxtlin();
		}
	}
}

void refrm() {
	SelectedForm = &formList[closestFormToCursor];
	if (rstMap(PRFACT)) {
		DestroyWindow(PreferencesWindow);
		rstMap(WASRT);
	}
	leftWindowSize.x = leftWindowSize.y = rightWindowSize.x = rightWindowSize.y = 0;
	setMap(REFCNT);
	formMenuEntryCount = 0;
	refrmfn();
	if (FormDataSheet) {
		while (EnumChildWindows(FormDataSheet, chenum, 0));
		MoveWindow(FormDataSheet, ButtonWidthX3 + 3, 3, leftWindowSize.x + rightWindowSize.x + 18, leftWindowSize.y*formMenuEntryCount + 12, TRUE);
		redraw(FormDataSheet);
	}
	else {
		FormDataSheet = CreateWindow(
			"STATIC",
			0,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			ButtonWidthX3 + 3,
			3,
			leftWindowSize.x + rightWindowSize.x + 18,
			leftWindowSize.y*formMenuEntryCount + 12,
			ThredWindow,
			NULL,
			ThredInstance,
			NULL);
	}
	rstMap(REFCNT);
	refrmfn();
}

void pxrct2stch(RECT pxr, fRECTANGLE* str) {
	POINT	tpnt;

	tpnt.x = pxr.left + StitchWindowOrigin.x;
	tpnt.y = pxr.top + StitchWindowOrigin.y;
	pxCor2stch(tpnt);
	str->left = SelectedPoint.x;
	str->top = SelectedPoint.y;
	tpnt.x = pxr.right + StitchWindowOrigin.x;
	tpnt.y = pxr.bottom + StitchWindowOrigin.y;
	pxCor2stch(tpnt);
	str->right = SelectedPoint.x;
	str->bottom = SelectedPoint.y;
}

unsigned pdir(unsigned ind) {
	if (chkMap(PSELDIR))
		return nxt(ind);
	else
		return prv(ind);
}

void setstrtch() {
	FLOAT		ref = 0;
	double		rat = 1;
	unsigned	ind, ine;
	fRECTANGLE		trct;
	long		tlng;

	savdo();
	if (chkMap(FPSEL))
		MoveMemory(&trct, &SelectedPointsRect, sizeof(fRECTANGLE));
	else {
		if (selectedFormCount || chkMap(BIGBOX))
			pxrct2stch(selectedFormsRectangle, &trct);
		else {
			fvars(closestFormToCursor);
			px2stch();
			trct.bottom = trct.left = trct.right = trct.top = 0;
		}
	}
	switch (selectedFormControlVertex) {
	case 0:

		if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.bottom;
			tlng = Msg.pt.y - StitchWindowOrigin.y;
			rat = (double)(selectedFormsRectangle.bottom - tlng) / (selectedFormsRectangle.bottom - selectedFormsRectangle.top);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.bottom;
				rat = (double)(SelectedPoint.y - ref) / (SelectedForm->rectangle.top - ref);
				SelectedForm->rectangle.top = SelectedPoint.y;
			}
			else {
				ref = StitchRangeRect.bottom;
				rat = (double)(SelectedPoint.y - ref) / (StitchRangeRect.top - ref);
			}
		}
		break;

	case 1:

		if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.left;
			tlng = Msg.pt.x - StitchWindowOrigin.x;
			rat = (double)(tlng - selectedFormsRectangle.left) / (selectedFormsRectangle.right - selectedFormsRectangle.left);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.left;
				rat = (double)(SelectedPoint.x - ref) / (SelectedForm->rectangle.right - ref);
				SelectedForm->rectangle.right = SelectedPoint.x;
			}
			else {
				ref = StitchRangeRect.left;
				rat = (double)(SelectedPoint.x - ref) / (StitchRangeRect.right - ref);
			}
		}
		break;

	case 2:

		if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.top;
			tlng = Msg.pt.y - StitchWindowOrigin.y;
			rat = (double)(tlng - selectedFormsRectangle.top) / (selectedFormsRectangle.bottom - selectedFormsRectangle.top);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.top;
				rat = (double)(SelectedPoint.y - ref) / (SelectedForm->rectangle.bottom - ref);
				SelectedForm->rectangle.bottom = SelectedPoint.y;
			}
			else {
				ref = StitchRangeRect.top;
				rat = (double)(SelectedPoint.y - ref) / (StitchRangeRect.bottom - ref);
			}
		}
		break;

	case 3:

		if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.right;
			tlng = Msg.pt.x - StitchWindowOrigin.x;
			rat = (double)(selectedFormsRectangle.right - tlng) / (selectedFormsRectangle.right - selectedFormsRectangle.left);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.right;
				rat = (double)(SelectedPoint.x - ref) / (SelectedForm->rectangle.left - ref);
				SelectedForm->rectangle.left = SelectedPoint.x;
			}
			else {
				ref = StitchRangeRect.right;
				rat = (double)(SelectedPoint.x - ref) / (StitchRangeRect.left - ref);
			}
		}
		break;
	}
	if (selectedFormControlVertex & 1) {
		if (chkMap(FPSEL)) {
			fvars(closestFormToCursor);
			ine = SelectedFormPoints.start;
			for (ind = 0; ind <= SelectedFormPoints.pointCount; ind++) {
				currentFormVertices[ine].x = (currentFormVertices[ine].x - ref)*rat + ref;
				ine = pdir(ine);
			}
			frmout(closestFormToCursor);
			setpsel();
			setMap(RESTCH);
			return;
		}
		if (chkMap(BIGBOX)) {
			for (ind = 0; ind < formIndex; ind++) {
				currentFormVertices = formList[ind].vertices;
				for (ine = 0; ine < formList[ind].sides; ine++)
					currentFormVertices[ine].x = (currentFormVertices[ine].x - ref)*rat + ref;
				frmout(ind);
			}
			for (ind = 0; ind < PCSHeader.stitchCount; ind++)
				StitchBuffer[ind].x = (StitchBuffer[ind].x - ref)*rat + ref;
			selal();
			return;
		}
		else {
			if (selectedFormCount) {
				for (ind = 0; ind < selectedFormCount; ind++) {
					currentFormVertices = formList[selectedFormList[ind]].vertices;
					for (ine = 0; ine < formList[selectedFormList[ind]].sides; ine++)
						currentFormVertices[ine].x = (currentFormVertices[ine].x - ref)*rat + ref;
				}
			}
			else {
				if (chkMap(FORMSEL)) {
					for (ind = 0; ind < sides; ind++)
						currentFormVertices[ind].x = (currentFormVertices[ind].x - ref)*rat + ref;
				}
				else {
					for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++)
						StitchBuffer[ind].x = (StitchBuffer[ind].x - ref)*rat + ref;
				}
			}
		}
	}
	else {
		if (chkMap(FPSEL)) {
			fvars(closestFormToCursor);
			ine = SelectedFormPoints.start;
			for (ind = 0; ind <= SelectedFormPoints.pointCount; ind++) {
				currentFormVertices[ine].y = (currentFormVertices[ine].y - ref)*rat + ref;
				ine = pdir(ine);
			}
			frmout(closestFormToCursor);
			setpsel();
			refil();
			setMap(RESTCH);
			return;
		}
		if (chkMap(BIGBOX)) {
			for (ind = 0; ind < formIndex; ind++) {
				currentFormVertices = formList[ind].vertices;
				for (ine = 0; ine < formList[ind].sides; ine++)
					currentFormVertices[ine].y = (currentFormVertices[ine].y - ref)*rat + ref;
				frmout(ind);
			}
			for (ind = 0; ind < PCSHeader.stitchCount; ind++)
				StitchBuffer[ind].y = (StitchBuffer[ind].y - ref)*rat + ref;
			selal();
			return;
		}
		else {
			if (selectedFormCount) {
				for (ind = 0; ind < selectedFormCount; ind++) {
					currentFormVertices = formList[selectedFormList[ind]].vertices;
					for (ine = 0; ine < formList[selectedFormList[ind]].sides; ine++)
						currentFormVertices[ine].y = (currentFormVertices[ine].y - ref)*rat + ref;
				}
			}
			else {
				if (chkMap(FORMSEL)) {
					for (ind = 0; ind < sides; ind++)
						currentFormVertices[ind].y = (currentFormVertices[ind].y - ref)*rat + ref;
				}
				else {
					for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++)
						StitchBuffer[ind].y = (StitchBuffer[ind].y - ref)*rat + ref;
				}
			}
		}
	}
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			ine = selectedFormList[ind];
			frmout(ine);
			closestFormToCursor = ine;
			refil();
		}
	}
	else
		if (chkMap(FORMSEL))
			refil();
	setMap(RESTCH);
}

void setexpand() {
	dPOINT		ref;
	POINT		tref;
	fPOINT		sref;
	dPOINT		l_siz0;
	dPOINT		l_siz1;
	dPOINT		rat;
	double		aspect;
	unsigned	ind, ine;
	fRECTANGLE		rct;

	savdo();
	if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
		rct.bottom = selectedFormsRectangle.bottom;
		rct.left = selectedFormsRectangle.left;
		rct.right = selectedFormsRectangle.right;
		rct.top = selectedFormsRectangle.top;
		SelectedPoint.x = Msg.pt.x - StitchWindowOrigin.x;
		SelectedPoint.y = Msg.pt.y - StitchWindowOrigin.y;
		l_siz0.y = rct.bottom - rct.top;
	}
	else {
		px2stch();
		fvars(closestFormToCursor);
		if (chkMap(FORMSEL))
			rct = SelectedForm->rectangle;
		else {
			rct.bottom = StitchRangeRect.bottom;
			rct.top = StitchRangeRect.top;
			rct.right = StitchRangeRect.right;
			rct.left = StitchRangeRect.left;
		}
		l_siz0.y = rct.top - rct.bottom;
	}
	rat.x = rat.y = 1;
	ref.x = ref.y = 0;
	l_siz0.x = rct.right - rct.left;
	switch (selectedFormControlVertex) {
	case 0:

		ref.x = rct.right;
		ref.y = rct.bottom;
		l_siz1.x = fabs(SelectedPoint.x - ref.x);
		l_siz1.y = fabs(SelectedPoint.y - ref.y);
		aspect = l_siz1.x / l_siz1.y;
		if (aspect < xyRatio)
			l_siz1.x = l_siz1.y*xyRatio;
		else
			l_siz1.y = l_siz1.x / xyRatio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!selectedFormCount&&chkMap(FORMSEL)) {
			SelectedForm->rectangle.left = rct.right - l_siz1.x;
			SelectedForm->rectangle.top = rct.bottom + l_siz1.y;
		}
		break;

	case 1:

		ref.x = rct.left;
		ref.y = rct.bottom;
		l_siz1.x = fabs(SelectedPoint.x - ref.x);
		l_siz1.y = fabs(SelectedPoint.y - ref.y);
		aspect = l_siz1.x / l_siz1.y;
		if (aspect < xyRatio)
			l_siz1.x = l_siz1.y*xyRatio;
		else
			l_siz1.y = l_siz1.x / xyRatio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!selectedFormCount&&chkMap(FORMSEL)) {
			SelectedForm->rectangle.right = rct.left + l_siz1.x;
			SelectedForm->rectangle.top = rct.bottom + l_siz1.y;
		}
		break;

	case 2:

		ref.x = rct.left;
		ref.y = rct.top;
		l_siz1.x = fabs(SelectedPoint.x - ref.x);
		l_siz1.y = fabs(SelectedPoint.y - ref.y);
		aspect = l_siz1.x / l_siz1.y;
		if (aspect < xyRatio)
			l_siz1.x = l_siz1.y*xyRatio;
		else
			l_siz1.y = l_siz1.x / xyRatio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!selectedFormCount&&chkMap(FORMSEL)) {
			SelectedForm->rectangle.right = rct.left + l_siz1.x;
			SelectedForm->rectangle.bottom = rct.top - l_siz1.y;
		}
		break;

	case 3:

		ref.x = rct.right;
		ref.y = rct.top;
		l_siz1.x = fabs(SelectedPoint.x - ref.x);
		l_siz1.y = fabs(SelectedPoint.y - ref.y);
		aspect = l_siz1.x / l_siz1.y;
		if (aspect < xyRatio)
			l_siz1.x = l_siz1.y*xyRatio;
		else
			l_siz1.y = l_siz1.x / xyRatio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!selectedFormCount&&chkMap(FORMSEL)) {
			SelectedForm->rectangle.left = rct.right - l_siz1.x;
			SelectedForm->rectangle.bottom = rct.top - l_siz1.y;
		}
		break;
	}
	tref.x = ref.x;
	tref.y = ref.y;
	px2stchf(tref, &sref);
	if (chkMap(FPSEL)) {
		fvars(closestFormToCursor);
		ine = SelectedFormPoints.start;
		for (ind = 0; ind <= SelectedFormPoints.pointCount; ind++) {
			currentFormVertices[ine].x = (currentFormVertices[ine].x - sref.x)*rat.x + sref.x;
			currentFormVertices[ine].y = (currentFormVertices[ine].y - sref.y)*rat.y + sref.y;
			ine = pdir(ine);
		}
		setpsel();
		frmout(closestFormToCursor);
		refil();
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		for (ind = 0; ind < formIndex; ind++) {
			fvars(ind);
			for (ine = 0; ine < SelectedForm->sides; ine++) {
				currentFormVertices[ine].x = (currentFormVertices[ine].x - sref.x)*rat.x + sref.x;
				currentFormVertices[ine].y = (currentFormVertices[ine].y - sref.y)*rat.y + sref.y;
			}
			frmout(ind);
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			StitchBuffer[ind].x = (StitchBuffer[ind].x - sref.x)*rat.x + sref.x;
			StitchBuffer[ind].y = (StitchBuffer[ind].y - sref.y)*rat.y + sref.y;
		}
		selal();
		return;
	}
	else {
		if (selectedFormCount) {
			for (ind = 0; ind < selectedFormCount; ind++) {
				fvars(selectedFormList[ind]);
				for (ine = 0; ine < SelectedForm->sides; ine++) {
					currentFormVertices[ine].x = (currentFormVertices[ine].x - sref.x)*rat.x + sref.x;
					currentFormVertices[ine].y = (currentFormVertices[ine].y - sref.y)*rat.y + sref.y;
				}
				frmout(selectedFormList[ind]);
				closestFormToCursor = selectedFormList[ind];
				refil();
			}
		}
		else {
			if (chkMap(FORMSEL)) {
				for (ind = 0; ind < sides; ind++) {
					currentFormVertices[ind].x = (currentFormVertices[ind].x - ref.x)*rat.x + ref.x;
					currentFormVertices[ind].y = (currentFormVertices[ind].y - ref.y)*rat.y + ref.y;
				}
				refil();
			}
			else {
				for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++) {
					StitchBuffer[ind].x = (StitchBuffer[ind].x - ref.x)*rat.x + ref.x;
					StitchBuffer[ind].y = (StitchBuffer[ind].y - ref.y)*rat.y + ref.y;
				}
			}
		}
		setMap(RESTCH);
	}
}

void nufilcol(unsigned col) {
	unsigned at, ind;

	if (SelectedForm->fillColor != col) {
		SelectedForm->fillColor = col;
		at = (closestFormToCursor << 4) | FRMFIL;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if ((StitchBuffer[ind].attribute&(FRMSK | TYPMSK | FTHMSK)) == at) {
				StitchBuffer[ind].attribute &= 0xfffffff0;
				StitchBuffer[ind].attribute |= col;
			}
		}
	}
}

void nufthcol(unsigned col) {
	unsigned at, ind;

	if (SelectedForm->fillInfo.feather.color != col) {
		SelectedForm->fillInfo.feather.color = col;
		at = (closestFormToCursor << 4) | FTHMSK;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if ((StitchBuffer[ind].attribute&(FRMSK | FTHMSK)) == at) {
				StitchBuffer[ind].attribute &= 0xfffffff0;
				StitchBuffer[ind].attribute |= col;
			}
		}
	}
}

void nubrdcol(unsigned col) {
	unsigned at, ind;

	SelectedForm->borderColor = col;
	at = (closestFormToCursor << 4) | FRMBFIL;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		if ((StitchBuffer[ind].attribute&(FRMSK | TYPMSK)) == at) {
			StitchBuffer[ind].attribute &= 0xfffffff0;
			StitchBuffer[ind].attribute |= col;
		}
	}
}

void nulapcol(unsigned col) {
	unsigned at, ind;

	if ((unsigned)(SelectedForm->borderColor >> 4) != col) {
		SelectedForm->borderColor &= 0xf;
		SelectedForm->borderColor |= col << 4;
		at = (closestFormToCursor << 4) | TYPMSK;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if ((StitchBuffer[ind].attribute&(TYPMSK | FRMSK)) == at) {
				StitchBuffer[ind].attribute &= 0xfffffff0;
				StitchBuffer[ind].attribute |= col;
			}
		}
	}
}

void sidwnd(HWND wnd) {
	RECT		wrct;
	unsigned	baksid;

	MsgIndex = 0;
	SideWindowEntryBuffer[0] = 0;
	baksid = FormMenuChoice;
	unsid();
	FormMenuChoice = baksid;
	GetWindowRect(wnd, &wrct);
	GetWindowRect(FormDataSheet, &MsgRect);
	SideMessageWindow = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		MsgRect.right - ThredWindowOrigin.x + 3,
		wrct.top - ThredWindowOrigin.y - 3,
		ButtonWidthX3,
		wrct.bottom - wrct.top + 3,
		ThredWindow,
		NULL,
		ThredInstance,
		NULL);
}

void prfsid(HWND wnd) {
	RECT	wrct;

	MsgIndex = 0;
	SideWindowEntryBuffer[0] = 0;
	unsid();
	GetWindowRect(wnd, &wrct);
	GetClientRect(PreferencesWindow, &MsgRect);
	SideMessageWindow = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		wrct.right - ThredWindowOrigin.x + 6,
		wrct.top - ThredWindowOrigin.y - 3,
		rightWindowSize.x,
		wrct.bottom - wrct.top + 3,
		ThredWindow,
		NULL,
		ThredInstance,
		NULL);
}

void sbold() {
	fvars(closestFormToCursor);
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGEBEAN;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void dubold() {
	unsigned ind;

	if (filmsgs(FML_BLD))
		return;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			sbold();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			sbold();
			coltab();
			setMap(INIT);
			setMap(RESTCH);
			ritot(PCSHeader.stitchCount);
		}
	}
}

void prftwin(TCHAR* str) {
	CreateWindow(
		"STATIC",
		str,
		WS_CHILD | WS_VISIBLE,
		leftWindowCoords.left,
		leftWindowCoords.top,
		leftWindowCoords.right - leftWindowCoords.left,
		leftWindowCoords.bottom - leftWindowCoords.top,
		PreferencesWindow,
		NULL,
		ThredInstance,
		NULL);
}

HWND prfnwin(TCHAR* str) {
	return CreateWindow(
		"STATIC",
		str,
		SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
		rightWindowCoords.left,
		rightWindowCoords.top,
		rightWindowCoords.right - rightWindowCoords.left,
		rightWindowCoords.bottom - rightWindowCoords.top,
		PreferencesWindow,
		NULL,
		ThredInstance,
		NULL);
}

void prflin(unsigned p_lin) {
	prftwin(StringTable[p_lin]);
	thDat[p_lin - STR_PRF0] = prfnwin(MsgBuffer);
	nxtlin();
}

void sethup() {
	if (IniFile.hoopSizeX == LHUPX&&IniFile.hoopSizeY == LHUPY) {
		IniFile.hoopType = LARGHUP;
		return;
	}
	if (IniFile.hoopSizeX == SHUPX&&IniFile.hoopSizeY == SHUPY) {
		IniFile.hoopType = SMALHUP;
		return;
	}
	if (IniFile.hoopSizeX == HUP100XY&&IniFile.hoopSizeY == HUP100XY) {
		IniFile.hoopType = HUP100;
		return;
	}
	IniFile.hoopType = CUSTHUP;
}

#if LANG==ENG||LANG==HNG

void prfmsg() {
	HDC		prfdc;
	RECT	prfrct;

	if (rstMap(INSRT))
		setMap(WASRT);
	rstMap(BIGBOX);
	selectedFormCount = 0;
	if (FormDataSheet) {
		undat();
		unsid();
		FormMenuChoice = 0;
	}
	leftWindowSize.x = leftWindowSize.y = 0;
	rightWindowSize.x = rightWindowSize.y = 0;
	maxtsiz(StringTable[STR_PRF0 + 4], &leftWindowSize);
	maxtsiz(StringTable[STR_TAPR], &rightWindowSize);
	leftWindowSize.x = preferenceWindowTextWidth;
	leftWindowSize.x += 4;
#if LANG==HNG
	leftWindowSize.x += 10;
#endif
	DestroyWindow(PreferencesWindow);
	preferenceWindowWidth = leftWindowSize.x + rightWindowSize.x + 18;
	PreferencesWindow = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3 + 3,
		3,
		preferenceWindowWidth,
		leftWindowSize.y*PRFLINS + 12,
		ThredWindow,
		NULL,
		ThredInstance,
		NULL);
	prfdc = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &prfrct);
	FillRect(prfdc, &prfrct, (HBRUSH)(COLOR_WINDOW + 1));
	leftWindowCoords.top = rightWindowCoords.top = 3;
	leftWindowCoords.bottom = rightWindowCoords.bottom = 3 + leftWindowSize.y;
	leftWindowCoords.left = 3;
	leftWindowCoords.right = 3 + leftWindowSize.x;
	rightWindowCoords.left = 6 + leftWindowSize.x;
	rightWindowCoords.right = 6 + leftWindowSize.x + rightWindowSize.x + 6;
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", UnderlayColor + 1);
	prflin(STR_PRF10);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", borderWidth / PFGRAN);
	prflin(STR_PRF3);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", buttonholeFillCornerLength / PFGRAN);
	prflin(STR_PRF14);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.chainSpace / PFGRAN);
	prflin(STR_PRF23);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.chainRatio);
	prflin(STR_PRF24);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f mm", IniFile.clipOffset / PFGRAN);
	prflin(STR_PRF21);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", IniFile.fillPhase);
	prflin(STR_PRF22);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.eggRatio);
	prflin(STR_PRF26);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.fillAngle / PI * 180);
	prflin(STR_PRF1);
	if (chku(SQRFIL))
		strcpy_s(MsgBuffer, StringTable[STR_SQR]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_PNTD]);
	prflin(STR_PRF2);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", stitchSpace / PFGRAN);
	prflin(STR_PRF0);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", duthrsh(ShowStitchThreshold));
	prflin(STR_PRF7);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f mm", IniFile.gridSize / PFGRAN);
	prflin(STR_PRF20);
	sethup();
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%s", StringTable[STR_HUP0 + IniFile.hoopType - 1]);
	prflin(STR_PRF17);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.0f mm", IniFile.hoopSizeY / PFGRAN);
	prflin(STR_PRF27);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.0f mm", IniFile.hoopSizeX / PFGRAN);
	prflin(STR_PRF18);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.cursorNudgeStep);
	prflin(STR_PRF25);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", picotSpace / PFGRAN);
	prflin(STR_PRF16);
	if (chku(BLUNT))
		strcpy_s(MsgBuffer, StringTable[STR_BLUNT]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_TAPR]);
	prflin(STR_PRF15);
	if (chku(DUND))
		strcpy_s(MsgBuffer, StringTable[STR_ON]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_OFF]);
	prflin(STR_PRF19);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SmallStitchLength / PFGRAN);
	prflin(STR_PRF9);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", snapLength / PFGRAN);
	prflin(STR_PRF11);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", spiralWrap);
	prflin(STR_PRF13);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", starRatio);
	prflin(STR_PRF12);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", duthrsh(StitchBoxesThreshold));
	prflin(STR_PRF8);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.maxStitchLength / PFGRAN);
	prflin(STR_PRF4);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", UserStitchLength / PFGRAN);
	prflin(STR_PRF5);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", MinStitchLength / PFGRAN);
	prflin(STR_PRF6);
	setMap(PRFACT);
	ReleaseDC(ThredWindow, prfdc);
}
#endif

#if LANG==GRM

void prfmsg() {
	HDC		prfdc;
	RECT	prfrct;

	if (rstMap(INSRT))
		setMap(WASRT);
	rstMap(BIGBOX);
	selectedFormCount = 0;
	if (FormDataSheet) {
		undat();
		unsid();
		FormMenuChoice = 0;
	}
	leftWindowSize.x = leftWindowSize.y = 0;
	rightWindowSize.x = rightWindowSize.y = 0;
	maxtsiz(StringTable[STR_PRF0 + 4], &leftWindowSize);
	maxtsiz(StringTable[STR_BLUNT], &rightWindowSize);
	leftWindowSize.x = preferenceWindowTextWidth;
	rightWindowSize.x += 4;
	DestroyWindow(PreferencesWindow);
	preferenceWindowWidth = leftWindowSize.x + rightWindowSize.x + 18;
	PreferencesWindow = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3 + 3,
		3,
		preferenceWindowWidth,
		leftWindowSize.y*PRFLINS + 12,
		ThredWindow,
		NULL,
		ThredInstance,
		NULL);
	prfdc = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &prfrct);
	FillRect(prfdc, &prfrct, (HBRUSH)(COLOR_WINDOW + 1));
	leftWindowCoords.top = rightWindowCoords.top = 3;
	leftWindowCoords.bottom = rightWindowCoords.bottom = 3 + leftWindowSize.y;
	leftWindowCoords.left = 3;
	leftWindowCoords.right = 3 + leftWindowSize.x;
	rightWindowCoords.left = 6 + leftWindowSize.x;
	rightWindowCoords.right = 6 + leftWindowSize.x + rightWindowSize.x + 6;
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", UserStitchLength / PFGRAN);
	prflin(STR_PRF5);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.maxStitchLength / PFGRAN);
	prflin(STR_PRF4);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", MinStitchLength / PFGRAN);
	prflin(STR_PRF6);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SmallStitchLength / PFGRAN);
	prflin(STR_PRF9);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.fillAngle / PI * 180);
	prflin(STR_PRF1);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", stitchSpace / PFGRAN);
	prflin(STR_PRF0);
	if (chku(SQRFIL))
		strcpy_s(MsgBuffer, StringTable[STR_SQR]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_PNTD]);
	prflin(STR_PRF2);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", borderWidth / PFGRAN);
	prflin(STR_PRF3);
	if (chku(BLUNT))
		strcpy_s(MsgBuffer, StringTable[STR_BLUNT]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_TAPR]);
	prflin(STR_PRF15);
	if (chku(DUND))
		strcpy_s(MsgBuffer, StringTable[STR_ON]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_OFF]);
	prflin(STR_PRF19);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", buttonholeFillCornerLength / PFGRAN);
	prflin(STR_PRF14);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.chainSpace / PFGRAN);
	prflin(STR_PRF23);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.chainRatio);
	prflin(STR_PRF24);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f mm", IniFile.clipOffset / PFGRAN);
	prflin(STR_PRF21);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", IniFile.fillPhase);
	prflin(STR_PRF22);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", picotSpace / PFGRAN);
	prflin(STR_PRF16);
	sethup();
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable[STR_HUP0 + IniFile.hoopType - 1]);
	prflin(STR_PRF17);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.0f mm", IniFile.hoopSizeY / PFGRAN);
	prflin(STR_PRF27);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.0f mm", IniFile.hoopSizeX / PFGRAN);
	prflin(STR_PRF18);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f mm", IniFile.gridSize / PFGRAN);
	prflin(STR_PRF20);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", duthrsh(ShowStitchThreshold));
	prflin(STR_PRF7);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.cursorNudgeStep);
	prflin(STR_PRF25);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", snapLength / PFGRAN);
	prflin(STR_PRF11);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", duthrsh(StitchBoxesThreshold));
	prflin(STR_PRF8);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.eggRatio);
	prflin(STR_PRF26);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", spiralWrap);
	prflin(STR_PRF13);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", starRatio);
	prflin(STR_PRF12);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", UnderlayColor + 1);
	prflin(STR_PRF10);
	setMap(PRFACT);
	ReleaseDC(ThredWindow, prfdc);
}
#endif

void durpoli(unsigned nsids) {
	double		dang;
	double		l_ang = 0;
	double		len;
	unsigned	ind;
	dPOINT		pnt;

	if (nsids < 3)
		nsids = 3;
	if (nsids > 100)
		nsids = 100;
	dang = PI * 2 / nsids;
	len = 500 / nsids*ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(nsids);
	SelectedForm->sides = nsids;
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(formIndex);
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	for (ind = 0; ind < sides; ind++) {
		currentFormVertices[ind].x = pnt.x;
		currentFormVertices[ind].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	SelectedForm->type = FRMFPOLY;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	formMoveDelta.x = formMoveDelta.y = 0;
	newFormVertexCount = nsids + 1;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

void dustar(unsigned nsids, double len) {
	double		dang;
	double		l_ang;
	unsigned	ind, tsid;
	dPOINT		pnt, cntr;

	if (nsids < 3)
		nsids = 3;
	if (nsids > 100)
		nsids = 100;
	dang = PI / nsids;
	l_ang = dang / 2 + PI;
	tsid = nsids << 1;
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(tsid);
	SelectedForm->sides = tsid;
	SelectedForm->attribute = (ActiveLayer << 1);
	fvars(formIndex);
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	setMap(FILDIR);
	for (ind = 0; ind < tsid; ind++) {
		currentFormVertices[ind].x = pnt.x;
		currentFormVertices[ind].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	cntr.x = (currentFormVertices[nsids].x - currentFormVertices[0].x) / 2 + currentFormVertices[0].x;
	cntr.y = (currentFormVertices[nsids].y - currentFormVertices[0].y) / 2 + currentFormVertices[0].y;
	for (ind = 1; ind < tsid; ind += 2) {
		currentFormVertices[ind].x = (currentFormVertices[ind].x - cntr.x)*starRatio + cntr.x;
		currentFormVertices[ind].y = (currentFormVertices[ind].y - cntr.y)*starRatio + cntr.y;
	}
	SelectedForm->type = FRMFPOLY;
	frmout(formIndex);
	formMoveDelta.x = formMoveDelta.y = 0;
	newFormVertexCount = tsid + 1;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

void duspir(unsigned nsids) {
	double		dang;
	double		l_ang = 0;
	double		len, drat, rat;
	unsigned	ind, ine, num;
	dPOINT		pnt;
	dPOINT		cntr;
	fPOINT*		tflt;
	fPOINT*		tdif;

	if (nsids < 3)
		nsids = 3;
	if (nsids > 100)
		nsids = 100;
	dang = PI * 2 / nsids;
	len = 800 / nsids*ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	num = nsids*spiralWrap;
	SelectedForm->vertices = adflt(num);
	tflt = new fPOINT[nsids];
	tdif = new fPOINT[nsids];
	SelectedForm->sides = num;
	SelectedForm->attribute = (ActiveLayer << 1);
	fvars(formIndex);
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	for (ind = 0; ind < nsids; ind++) {
		tflt[ind].x = pnt.x;
		tflt[ind].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	cntr.x = (tflt[nsids >> 1].x - tflt[0].x) / 2 + tflt[0].x;
	cntr.y = (tflt[nsids >> 1].y - tflt[0].y) / 2 + tflt[0].y;
	for (ind = 0; ind < nsids; ind++) {
		tdif[ind].x = tflt[ind].x - cntr.x;
		tdif[ind].y = tflt[ind].y - cntr.y;
	}
	drat = (double)1 / num;
	rat = drat; ine = 0;
	for (ind = 0; ind < num; ind++) {
		SelectedForm->vertices[ine].x = tdif[ine%nsids].x*rat + cntr.x;
		SelectedForm->vertices[ine].y = tdif[ine%nsids].y*rat + cntr.y;
		rat += drat;
		ine++;
	}
	SelectedForm->type = FRMLINE;
	frmout(formIndex);
	formMoveDelta.x = formMoveDelta.y = 0;
	newFormVertexCount = num + 1;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
	delete[] tflt;
	delete[] tdif;
}

void duhart(unsigned nsids) {
	double		dang;
	double		l_ang;
	double		len;
	unsigned	ind, ine, bind;
	float		av;
	dPOINT		pnt;
	double		rat;

	if (nsids > 100)
		nsids = 100;
	if (nsids < 6)
		nsids = 6;
	SelectedForm = &formList[formIndex];
	frmclr(SelectedForm);
	SelectedForm->attribute = ActiveLayer << 1;
	currentFormVertices = &FormVertices[FormVertexIndex];
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	dang = PI * 2 / nsids;
	len = 300 / nsids*ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	l_ang = PI*0.28;
	ind = 0;
	av = 0;
	while (l_ang > -PI*0.7) {
		if (pnt.x > av)
			av = pnt.x;
		currentFormVertices[ind].x = pnt.x;
		currentFormVertices[ind++].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang -= dang;
	}
	dang /= 4.5;
	bind = ind;
	while (pnt.x > currentFormVertices[0].x&&ind < 200) {
		currentFormVertices[ind].x = pnt.x;
		currentFormVertices[ind++].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang -= dang;
	}
	bind--;
	rat = (currentFormVertices[bind].x - currentFormVertices[0].x) / (currentFormVertices[bind].x - currentFormVertices[ind - 1].x);
	for (ine = bind + 1; ine < ind; ine++)
		currentFormVertices[ine].x = (currentFormVertices[ine].x - currentFormVertices[bind].x)*rat + currentFormVertices[bind].x;
	bind = ine;
	for (ind = bind - 2; ind; ind--) {
		currentFormVertices[ine].y = currentFormVertices[ind].y;
		currentFormVertices[ine].x = av + av - currentFormVertices[ind].x - 2 * (av - currentFormVertices[0].x);
		ine++;
	}
	newFormVertexCount = ine + 1;
	SelectedForm->vertices = adflt(ine);
	SelectedForm->sides = ine;
	SelectedForm->type = FRMFPOLY;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	formMoveDelta.x = formMoveDelta.y = 0;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

void dulens(unsigned nsids) {
	double		dang;
	double		l_ang;
	double		len;
	unsigned	ind, ine, bind, cnt;
	float		av;
	dPOINT		pnt;

	if (nsids < 6)
		nsids = 6;
	if (nsids > 48)
		nsids = 48;
	nsids <<= 1;
	dang = PI * 2 / nsids;
	cnt = nsids / 2 * 0.3;
	l_ang = cnt*dang;
	len = 500 / nsids*ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(formIndex);
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	ind = 0;
	SelectedPoint.x -= (float)0.0001;
	while (pnt.x >= SelectedPoint.x) {
		currentFormVertices[ind].x = pnt.x;
		currentFormVertices[ind++].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	bind = ind - 1;
	ine = ind;
	av = currentFormVertices[0].x;
	for (ind = bind; ind != 0; ind--) {
		currentFormVertices[ine].y = currentFormVertices[ind - 1].y;
		currentFormVertices[ine].x = av + av - currentFormVertices[ind - 1].x;
		ine++;
	}
	newFormVertexCount = ine;
	SelectedForm->vertices = adflt(ine - 1);
	SelectedForm->sides = ine - 1;
	SelectedForm->type = FRMFPOLY;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	formMoveDelta.x = formMoveDelta.y = 0;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

float shreg(float hi, float ref) {
	return (hi - ref)*eggRatio + ref;
}

void dueg(unsigned nsids) {
	double		ref, hi;
	unsigned	ind;

	if (nsids < 8)
		nsids = 8;
	fvars(formIndex);
	durpoli(nsids);
	ref = midl(currentFormVertices[nsids / 2].y, currentFormVertices[0].y);
	hi = currentFormVertices[nsids >> 2].y - currentFormVertices[0].y;
	for (ind = 0; ind < nsids; ind++) {
		if (currentFormVertices[ind].y < ref)
			currentFormVertices[ind].y = ref - (ref - currentFormVertices[ind].y)*IniFile.eggRatio;
	}
	eggRatio = hi / (currentFormVertices[nsids >> 2].y - currentFormVertices[0].y);
	for (ind = 1; ind < sides; ind++) {
		currentFormVertices[ind].x = shreg(currentFormVertices[ind].x, currentFormVertices[0].x);
		currentFormVertices[ind].y = shreg(currentFormVertices[ind].y, currentFormVertices[0].y);
	}
}

void duzig(unsigned nsids) {
	fPOINT		off;
	unsigned	ind;

	if (nsids < 3)
		nsids = 3;
	if (nsids > 100)
		nsids = 100;
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(nsids);
	SelectedForm->sides = nsids;
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(formIndex);
	px2stch();
	SelectedPoint.x;
	SelectedPoint.y;
	off.x = UnzoomedRect.x / 6;
	off.y = UnzoomedRect.y / (6 * nsids);
	for (ind = 0; ind < nsids; ind++) {
		currentFormVertices[ind].x = SelectedPoint.x;
		currentFormVertices[ind].y = SelectedPoint.y;
		SelectedPoint.y -= off.y;
		if (ind & 1)
			SelectedPoint.x += off.x;
		else
			SelectedPoint.x -= off.x;
	}
	SelectedForm->type = FRMLINE;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	formMoveDelta.x = formMoveDelta.y = 0;
	newFormVertexCount = nsids + 1;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

void fliph() {
	unsigned	ind, ine;
	float		av = 0;
	fRECTANGLE		trct;

	fvars(closestFormToCursor);
	if (chkMap(FPSEL)) {
		savdo();
		av = (SelectedPointsRect.right - SelectedPointsRect.left)*0.5 + SelectedPointsRect.left;
		ine = SelectedFormPoints.start;
		for (ind = 0; ind <= SelectedFormPoints.pointCount; ind++) {
			currentFormVertices[ine].x = av + av - currentFormVertices[ine].x;
			ine = pdir(ine);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		av = (allItemsRectangle.right - allItemsRectangle.left) / 2 + allItemsRectangle.left;
		for (ind = 0; ind < FormVertexIndex; ind++)
			FormVertices[ind].x = av + av - FormVertices[ind].x;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			StitchBuffer[ind].x = av + av - StitchBuffer[ind].x;
		for (ind = 0; ind < formIndex; ind++) {
			formList[ind].rectangle.left = av + av - formList[ind].rectangle.left;
			formList[ind].rectangle.right = av + av - formList[ind].rectangle.right;
		}
		setMap(RESTCH);
		return;
	}
	if (selectedFormCount) {
		savdo();
		clRmap((formIndex >> 5) + 1);
		pxrct2stch(selectedFormsRectangle, &trct);
		av = (trct.right - trct.left) / 2 + trct.left;
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			setr(closestFormToCursor);
			fvars(closestFormToCursor);
			for (ine = 0; ine < SelectedForm->sides; ine++)
				currentFormVertices[ine].x = av + av - currentFormVertices[ine].x;
			frmout(closestFormToCursor);
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			ine = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
			if (chkr(ine) && !(StitchBuffer[ind].attribute&NOTFRM))
				StitchBuffer[ind].x = av + av - StitchBuffer[ind].x;
		}
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			av = (SelectedForm->rectangle.right - SelectedForm->rectangle.left) / 2 + SelectedForm->rectangle.left;
			for (ind = 0; ind < sides; ind++)
				currentFormVertices[ind].x = av + av - currentFormVertices[ind].x;
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor && !(StitchBuffer[ind].attribute&NOTFRM))
					StitchBuffer[ind].x = av + av - StitchBuffer[ind].x;
			}
			frmout(closestFormToCursor);
			setMap(RESTCH);
		}
		else {
			if (chkMap(GRPSEL)) {
				savdo();
				rngadj();
				selRct(&trct);
				av = (trct.right - trct.left) / 2 + trct.left;
				for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++)
					StitchBuffer[ind].x = av + av - StitchBuffer[ind].x;
				setMap(RESTCH);
			}
		}
	}
}

void flipv() {
	unsigned	ind, ine;
	float		av = 0;
	fRECTANGLE		trct;

	fvars(closestFormToCursor);
	if (chkMap(FPSEL)) {
		savdo();
		av = (SelectedPointsRect.top - SelectedPointsRect.bottom)*0.5 + SelectedPointsRect.bottom;
		ine = SelectedFormPoints.start;
		for (ind = 0; ind <= SelectedFormPoints.pointCount; ind++) {
			currentFormVertices[ine].y = av + av - currentFormVertices[ine].y;
			ine = pdir(ine);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		savdo();
		av = (allItemsRectangle.top - allItemsRectangle.bottom) / 2 + allItemsRectangle.bottom;
		for (ind = 0; ind < FormVertexIndex; ind++)
			FormVertices[ind].y = av + av - FormVertices[ind].y;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			StitchBuffer[ind].y = av + av - StitchBuffer[ind].y;
		for (ind = 0; ind < formIndex; ind++) {
			formList[ind].rectangle.bottom = av + av - formList[ind].rectangle.bottom;
			formList[ind].rectangle.top = av + av - formList[ind].rectangle.top;
		}
		setMap(RESTCH);
		return;
	}
	if (selectedFormCount) {
		savdo();
		clRmap((formIndex >> 5) + 1);
		pxrct2stch(selectedFormsRectangle, &trct);
		av = (trct.top - trct.bottom) / 2 + trct.bottom;
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			setr(closestFormToCursor);
			fvars(closestFormToCursor);
			for (ine = 0; ine < SelectedForm->sides; ine++)
				currentFormVertices[ine].y = av + av - currentFormVertices[ine].y;
			frmout(closestFormToCursor);
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			ine = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
			if (chkr(ine) && !(StitchBuffer[ind].attribute&NOTFRM))
				StitchBuffer[ind].y = av + av - StitchBuffer[ind].y;
		}
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			av = (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom) / 2 + SelectedForm->rectangle.bottom;
			for (ind = 0; ind < sides; ind++)
				currentFormVertices[ind].y = av + av - currentFormVertices[ind].y;
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor && !(StitchBuffer[ind].attribute&NOTFRM))
					StitchBuffer[ind].y = av + av - StitchBuffer[ind].y;
			}
			frmout(closestFormToCursor);
			setMap(RESTCH);
		}
		else {
			if (chkMap(GRPSEL)) {
				savdo();
				rngadj();
				selRct(&trct);
				av = (trct.top - trct.bottom) / 2 + trct.bottom;
				for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++)
					StitchBuffer[ind].y = av + av - StitchBuffer[ind].y;
				setMap(RESTCH);
			}
		}
	}
}

void tomsg() {
	RECT	okrct;
	SIZE	tsiz;

	GetWindowRect(OKButton, &okrct);
	GetTextExtentPoint32(StitchWindowMemDC, StringTable[STR_DELST2], strlen(StringTable[STR_DELST2]), &tsiz);
	DeleteStitchesDialog = CreateWindow(
		"STATIC",
		StringTable[STR_DELST2],
		SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
		3,
		okrct.bottom - StitchWindowOrigin.y + 6 + tsiz.cy,
		tsiz.cx + 6,
		tsiz.cy + 6,
		MainStitchWin,
		NULL,
		ThredInstance,
		NULL);
}

void sprct(unsigned strt, unsigned fin) {
	dPOINT	dif, tpnt;
	VRCT2*	tvrct;

	tvrct = &pointsVertRect[strt];
	dif.x = outsidePoints[fin].x - outsidePoints[strt].x;
	dif.y = outsidePoints[fin].y - outsidePoints[strt].y;
	if (dif.x&&dif.y) {
		slope = -dif.x / dif.y;
		tpnt.x = currentFormVertices[fin].x;
		tpnt.y = currentFormVertices[fin].y;
		proj(tpnt, slope, outsidePoints[strt], outsidePoints[fin], &tvrct->dopnt);
		proj(tpnt, slope, insidePoints[strt], insidePoints[fin], &tvrct->dipnt);
		tpnt.x = currentFormVertices[strt].x;
		tpnt.y = currentFormVertices[strt].y;
		proj(tpnt, slope, outsidePoints[strt], outsidePoints[fin], &tvrct->aopnt);
		proj(tpnt, slope, insidePoints[strt], insidePoints[fin], &tvrct->aipnt);
		tpnt.x = insidePoints[strt].x;
		tpnt.y = insidePoints[strt].y;
		if (proj(tpnt, slope, outsidePoints[strt], outsidePoints[fin], &tvrct->bopnt)) {
			tvrct->bipnt.x = insidePoints[strt].x;
			tvrct->bipnt.y = insidePoints[strt].y;
		}
		else {
			tvrct->bopnt.x = outsidePoints[strt].x;
			tvrct->bopnt.y = outsidePoints[strt].y;
			tpnt.x = outsidePoints[strt].x;
			tpnt.y = outsidePoints[strt].y;
			proj(tpnt, slope, insidePoints[strt], insidePoints[fin], &tvrct->bipnt);
		}
		tpnt.x = insidePoints[fin].x;
		tpnt.y = insidePoints[fin].y;
		if (proj(tpnt, slope, outsidePoints[strt], outsidePoints[fin], &tvrct->copnt)) {
			tvrct->cipnt.x = insidePoints[fin].x;
			tvrct->cipnt.y = insidePoints[fin].y;
		}
		else {
			tvrct->copnt.x = outsidePoints[fin].x;
			tvrct->copnt.y = outsidePoints[fin].y;
			tpnt.x = outsidePoints[fin].x;
			tpnt.y = outsidePoints[fin].y;
			proj(tpnt, slope, insidePoints[strt], insidePoints[fin], &tvrct->cipnt);
		}
	}
	else {
		if (dif.x) {
			tpnt.x = currentFormVertices[fin].x;
			projv(tpnt.x, outsidePoints[strt], outsidePoints[fin], &tvrct->dopnt);
			projv(tpnt.x, insidePoints[strt], insidePoints[fin], &tvrct->dipnt);
			tpnt.x = currentFormVertices[strt].x;
			projv(tpnt.x, outsidePoints[strt], outsidePoints[fin], &tvrct->aopnt);
			projv(tpnt.x, insidePoints[strt], insidePoints[fin], &tvrct->aipnt);
			tpnt.x = insidePoints[strt].x;
			if (projv(tpnt.x, outsidePoints[strt], outsidePoints[fin], &tvrct->bopnt)) {
				tvrct->bipnt.x = insidePoints[strt].x;
				tvrct->bipnt.y = insidePoints[strt].y;
			}
			else {
				tvrct->bopnt.x = outsidePoints[strt].x;
				tvrct->bopnt.y = outsidePoints[strt].y;
				tpnt.x = outsidePoints[strt].x;
				projv(tpnt.x, insidePoints[strt], insidePoints[fin], &tvrct->bipnt);
			}
			tpnt.x = insidePoints[fin].x;
			if (projv(tpnt.x, outsidePoints[strt], outsidePoints[fin], &tvrct->copnt)) {
				tvrct->cipnt.x = insidePoints[fin].x;
				tvrct->cipnt.y = insidePoints[fin].y;
			}
			else {
				tvrct->copnt.x = outsidePoints[fin].x;
				tvrct->copnt.y = outsidePoints[fin].y;
				tpnt.x = outsidePoints[fin].x;
				projv(tpnt.x, insidePoints[strt], insidePoints[fin], &tvrct->cipnt);
			}
		}
		else {
			tpnt.y = currentFormVertices[fin].y;
			projh(tpnt.y, outsidePoints[strt], outsidePoints[fin], &tvrct->dopnt);
			projh(tpnt.y, insidePoints[strt], insidePoints[fin], &tvrct->dipnt);
			tpnt.y = currentFormVertices[strt].y;
			projh(tpnt.y, outsidePoints[strt], outsidePoints[fin], &tvrct->aopnt);
			projh(tpnt.y, insidePoints[strt], insidePoints[fin], &tvrct->aipnt);
			tpnt.y = insidePoints[strt].y;
			if (projh(tpnt.y, outsidePoints[strt], outsidePoints[fin], &tvrct->bopnt)) {
				tvrct->bipnt.x = insidePoints[strt].x;
				tvrct->bipnt.y = insidePoints[strt].y;
			}
			else {
				tvrct->bopnt.x = outsidePoints[strt].x;
				tvrct->bopnt.y = outsidePoints[strt].y;
				tpnt.y = outsidePoints[strt].y;
				projh(tpnt.y, insidePoints[strt], insidePoints[fin], &tvrct->bipnt);
			}
			tpnt.y = insidePoints[fin].y;
			if (projh(tpnt.y, outsidePoints[strt], outsidePoints[fin], &tvrct->copnt)) {
				tvrct->cipnt.x = insidePoints[fin].x;
				tvrct->cipnt.y = insidePoints[fin].y;
			}
			else {
				tvrct->copnt.x = outsidePoints[fin].x;
				tvrct->copnt.y = outsidePoints[fin].y;
				tpnt.y = outsidePoints[fin].y;
				projh(outsidePoints[fin].y, insidePoints[strt], insidePoints[fin], &tvrct->cipnt);
			}
		}
	}
}

void spurfn(dPOINT* ipnt, dPOINT* p_opnt, dPOINT* uipnt, dPOINT* uopnt) {
	dPOINT	dif;

	dif.x = p_opnt->x - ipnt->x;
	dif.y = p_opnt->y - ipnt->y;
	uipnt->x = dif.x*DIURAT + ipnt->x;
	uipnt->y = dif.y*DIURAT + ipnt->y;
	uopnt->x = dif.x*DOURAT + ipnt->x;
	uopnt->y = dif.y*DOURAT + ipnt->y;
}

void spurct(unsigned ind) {
	spurfn(&pointsVertRect[ind].aipnt, &pointsVertRect[ind].aopnt, &underlayVertRect[ind].aipnt, &underlayVertRect[ind].aopnt);
	spurfn(&pointsVertRect[ind].bipnt, &pointsVertRect[ind].bopnt, &underlayVertRect[ind].bipnt, &underlayVertRect[ind].bopnt);
	spurfn(&pointsVertRect[ind].cipnt, &pointsVertRect[ind].copnt, &underlayVertRect[ind].cipnt, &underlayVertRect[ind].copnt);
	spurfn(&pointsVertRect[ind].dipnt, &pointsVertRect[ind].dopnt, &underlayVertRect[ind].dipnt, &underlayVertRect[ind].dopnt);
}

unsigned psg() {
	unsigned tmp;

	if (!pseudoRandomValue)
		pseudoRandomValue = SEED;
	tmp = pseudoRandomValue & 0x48000000;
	pseudoRandomValue <<= 1;
	if (tmp == 0x40000000 || tmp == 0x8000000)
		pseudoRandomValue++;
	return pseudoRandomValue;
}

void duromb(dPOINT strt0, dPOINT fin0, dPOINT strt1, dPOINT fin1) {
	dPOINT		dif0, dif1, stp0, stp1;
	double		len0, len1;
	unsigned	cnt, ind;

	if (!chkMap(UND)) {
		dif0.x = SelectedPoint.x - strt0.x;
		dif0.y = SelectedPoint.y - strt0.y;
		dif1.x = SelectedPoint.x - strt1.x;
		dif1.y = SelectedPoint.y - strt1.y;
		len0 = hypot(dif0.x, dif0.y);
		len1 = hypot(dif1.x, dif1.y);
		if (len0 > len1)
			setMap(FILDIR);
		else
			rstMap(FILDIR);
	}
	dif0.x = fin0.x - strt0.x;
	dif0.y = fin0.y - strt0.y;
	dif1.x = fin1.x - strt1.x;
	dif1.y = fin1.y - strt1.y;
	len0 = hypot(dif0.x, dif0.y);
	cnt = len0 / (stitchSpace / 2);
	if (!cnt)
		cnt++;
	stp0.x = dif0.x / cnt;
	stp0.y = dif0.y / cnt;
	stp1.x = dif1.x / cnt;
	stp1.y = dif1.y / cnt;
	for (ind = 0; ind < cnt; ind++) {
		if (toglMap(FILDIR))
			filinsb(strt0);
		else
			filinsb(strt1);
		strt0.x += stp0.x;
		strt0.y += stp0.y;
		strt1.x += stp1.x;
		strt1.y += stp1.y;
	}
}

void spend(unsigned strt, unsigned fin) {
	double		ilen, olen;
	dPOINT		idif, odif;
	double		sang, fang, dang, stang;
	dPOINT		sdif, fdif;
	dPOINT		piv;
	double		rad, arc, irad;
	unsigned	ind, cnt, lvl;
	dPOINT		ipnt, l_opnt;

	idif.x = pointsVertRect[fin].cipnt.x - pointsVertRect[strt].bipnt.x;
	idif.y = pointsVertRect[fin].cipnt.y - pointsVertRect[strt].bipnt.y;
	odif.x = pointsVertRect[fin].copnt.x - pointsVertRect[strt].bopnt.x;
	odif.y = pointsVertRect[fin].copnt.y - pointsVertRect[strt].bopnt.y;
	ilen = hypot(idif.x, idif.y);
	olen = hypot(odif.x, odif.y);
	if (olen > ilen) {
		piv.x = pointsVertRect[strt].cipnt.x;
		piv.y = pointsVertRect[strt].cipnt.y;
		sdif.x = pointsVertRect[strt].copnt.x - piv.x;
		sdif.y = pointsVertRect[strt].copnt.y - piv.y;
		fdif.x = pointsVertRect[fin].bopnt.x - piv.x;
		fdif.y = pointsVertRect[fin].bopnt.y - piv.y;
	}
	else {
		piv.x = pointsVertRect[strt].copnt.x;
		piv.y = pointsVertRect[strt].copnt.y;
		sdif.x = pointsVertRect[strt].cipnt.x - piv.x;
		sdif.y = pointsVertRect[strt].cipnt.y - piv.y;
		fdif.x = pointsVertRect[fin].bipnt.x - piv.x;
		fdif.y = pointsVertRect[fin].bipnt.y - piv.y;
	}
	if (hypot(SelectedPoint.x - piv.x, SelectedPoint.y - piv.y) > 2 * PI)
		filinsb(piv);
	sang = atan2(sdif.y, sdif.x);
	fang = atan2(fdif.y, fdif.x);
	dang = fang - sang;
	if (dang > PI)
		dang -= 2 * PI;
	if (dang < -PI)
		dang += 2 * PI;
	rad = hypot(sdif.x, sdif.y);
	arc = fabs(rad*dang);
	cnt = arc / stitchSpace;
	stang = dang / cnt;
	if (!cnt)
		cnt = 1;
	for (ind = 0; ind < cnt; ind++) {
		l_opnt.x = piv.x + cos(sang)*rad;
		l_opnt.y = piv.y + sin(sang)*rad;
		filinsb(l_opnt);
		if (cnt & 0xfffffff0)
			lvl = psg() % cnt;
		else
			lvl = lvls[cnt][ind];
		irad = rad*lvl / cnt*0.4;
		ipnt.x = piv.x + cos(sang)*irad;
		ipnt.y = piv.y + sin(sang)*irad;
		filinsb(ipnt);
		sang += stang;
	}
}

void duspnd(unsigned strt, unsigned fin) {
	double	len, tang;
	dPOINT	tpnt, dif;

	if (chkMap(UND)) {
		if (chkMap(UNDPHAS)) {
			filinsb(underlayVertRect[strt].copnt);
			filinsb(underlayVertRect[strt].cipnt);
			dif.x = underlayVertRect[fin].bipnt.x - underlayVertRect[strt].cipnt.x;
			dif.y = underlayVertRect[fin].bipnt.y - underlayVertRect[strt].cipnt.y;
			len = hypot(dif.x, dif.y);
			if (len > SelectedForm->edgeStitchLen) {
				tang = atan2(insidePoints[fin].y - outsidePoints[fin].y, insidePoints[fin].x - outsidePoints[fin].x);
				tpnt.x = underlayVertRect[fin].bopnt.x + cos(tang)*plen;
				tpnt.y = underlayVertRect[fin].bopnt.y + sin(tang)*plen;
				filinsb(tpnt);
			}
			filinsb(underlayVertRect[fin].bipnt);
			filinsb(underlayVertRect[fin].bopnt);
		}
		else {
			filinsb(underlayVertRect[strt].cipnt);
			filinsb(underlayVertRect[strt].copnt);
			dif.x = underlayVertRect[fin].bopnt.x - underlayVertRect[strt].copnt.x;
			dif.y = underlayVertRect[fin].bopnt.y - underlayVertRect[strt].copnt.y;
			len = hypot(dif.x, dif.y);
			if (len > SelectedForm->edgeStitchLen) {
				tang = atan2(outsidePoints[fin].y - insidePoints[fin].y, outsidePoints[fin].x - insidePoints[fin].x);
				tpnt.x = underlayVertRect[fin].bipnt.x + cos(tang)*plen;
				tpnt.y = underlayVertRect[fin].bipnt.y + sin(tang)*plen;
				filinsb(tpnt);
			}
			filinsb(underlayVertRect[fin].bopnt);
			filinsb(underlayVertRect[fin].bipnt);
		}
	}
	else
		spend(strt, fin);
}

void pfn(unsigned strtlin, VRCT2* vrct) {
	unsigned		ind;
	unsigned short	nlin;

	SelectedPoint.x = currentFormVertices[strtlin].x;
	SelectedPoint.y = currentFormVertices[strtlin].y;
	nlin = nxt(strtlin);
	for (ind = 0; ind < SelectedForm->sides; ind++) {
		duromb(vrct[strtlin].bipnt, vrct[strtlin].cipnt, vrct[strtlin].bopnt, vrct[strtlin].copnt);
		duspnd(strtlin, nlin);
		strtlin = nlin;
		nlin = nxt(nlin);
	}
}

void plfn(VRCT2* prct) {
	unsigned	ind;

	duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt);
	duspnd(1, 2);
	for (ind = 2; ind < (unsigned)sides - 4; ind++) {
		duromb(prct[ind].bipnt, prct[ind].cipnt, prct[ind].bopnt, prct[ind].copnt);
		duspnd(ind, ind + 1);
	}
	duromb(prct[sides - 4].bipnt, prct[sides - 4].dipnt, prct[sides - 4].bopnt, prct[sides - 4].dopnt);
}

void prsmal() {
	unsigned	ind, ine, ref;
	double		lSize, len;
	dPOINT		dif;

	ref = 0; ine = 0;
	lSize = USPAC*0.8;
	if (lSize > plen)
		lSize = plen*0.9;
	for (ind = 1; ind < sequenceIndex; ind++) {
		dif.x = oseq[ind].x - oseq[ref].x;
		dif.y = oseq[ind].y - oseq[ref].y;
		len = hypot(dif.x, dif.y);
		if (len > lSize) {
			oseq[ine].x = oseq[ind].x;
			oseq[ine++].y = oseq[ind].y;
			ref = ind;
		}
	}
	sequenceIndex = ine;
}

void plbak(unsigned bpnt) {
	unsigned	ind = sequenceIndex - 1;
	fPOINT		tflt;

	while (ind > bpnt) {
		tflt.x = oseq[ind].x;
		tflt.y = oseq[ind].y;
		oseq[ind].x = oseq[bpnt].x;
		oseq[ind].y = oseq[bpnt].y;
		oseq[bpnt].x = tflt.x;
		oseq[bpnt].y = tflt.y;
		ind--; bpnt++;
	}
}

void plvct(unsigned pind, dPOINT* vp0, dPOINT* vp1, double len)
{
	double l_angl;
	double len2;
	fPOINT vct;

	len2 = len / 2;
	l_angl = atan2(currentFormVertices[pind + 1].y - currentFormVertices[pind].y, currentFormVertices[pind + 1].x - currentFormVertices[pind].x) + PI / 2;
	vct.x = cos(l_angl)*len2;
	vct.y = sin(l_angl)*len2;
	vp0->x = currentFormVertices[pind].x + vct.x;
	vp0->y = currentFormVertices[pind].y + vct.y;
	vp1->x = currentFormVertices[pind].x - vct.x;
	vp1->y = currentFormVertices[pind].y - vct.y;
}

void prebrd()
{
	fPOINT dif;
	double rat;

	MoveMemory(&angledFormVertices[1], currentFormVertices, sizeof(fPOINT)*sides);
	dif.x = currentFormVertices[1].x - currentFormVertices[0].x;
	dif.y = currentFormVertices[1].y - currentFormVertices[0].y;
	if (fabs(dif.x) > fabs(dif.y))
		rat = fabs(0.1 / dif.x);
	else
		rat = fabs(0.1 / dif.y);
	angledFormVertices[0].x = currentFormVertices[0].x - dif.x*rat;
	angledFormVertices[0].y = currentFormVertices[0].y - dif.y*rat;
	MoveMemory(&angledForm, SelectedForm, sizeof(FRMHED));
	angledForm.vertices = angledFormVertices;
	angledForm.sides += 3;
	dif.x = currentFormVertices[sides - 1].x - currentFormVertices[sides - 2].x;
	dif.y = currentFormVertices[sides - 1].y - currentFormVertices[sides - 2].y;
	if (dif.x > dif.y)
		rat = 0.1 / dif.x;
	else
		rat = 0.1 / dif.y;
	angledFormVertices[angledForm.sides - 1].x = currentFormVertices[sides - 1].x + dif.x*rat;
	angledFormVertices[angledForm.sides - 1].y = currentFormVertices[sides - 1].y + dif.y*rat;
	SelectedForm = &angledForm;
	sides = angledForm.sides;
	currentFormVertices = angledForm.vertices;
}

void plbrd(double spac) {
	double			tspac;
	unsigned short	slin;
	unsigned bpnt;
	unsigned ind;

	slin = getlast();
	prebrd();
	tspac = stitchSpace;
	pointsVertRect = (VRCT2*)bseq;
	underlayVertRect = &pointsVertRect[sides];
	satout(SelectedForm->borderSize);
	insidePoints[sides].x = insidePoints[0].x;
	insidePoints[sides].y = insidePoints[0].y;
	outsidePoints[sides].x = outsidePoints[0].x;
	outsidePoints[sides].y = outsidePoints[0].y;
	for (ind = 0; ind < (unsigned)sides - 1; ind++)
	{
		sprct(ind, ind + 1);
		spurct(ind);
	}
	sprct(ind, 0);
	spurct(ind);
	if (!(SelectedForm->attribute&SBLNT))
	{
		pointsVertRect[1].aipnt.x = pointsVertRect[1].aopnt.x = underlayVertRect[1].aipnt.x = underlayVertRect[1].aopnt.x = SelectedForm->vertices[1].x;
		pointsVertRect[1].aipnt.y = pointsVertRect[1].aopnt.y = underlayVertRect[1].aipnt.y = underlayVertRect[1].aopnt.y = SelectedForm->vertices[1].y;
	}
	if (!(SelectedForm->attribute&FBLNT))
	{
		pointsVertRect[sides - 4].dipnt.x = pointsVertRect[sides - 4].dopnt.x = underlayVertRect[sides - 4].dipnt.x = underlayVertRect[sides - 4].dopnt.x = SelectedForm->vertices[sides - 1].x;
		pointsVertRect[sides - 4].dipnt.y = pointsVertRect[sides - 4].dopnt.y = underlayVertRect[sides - 4].dipnt.y = underlayVertRect[sides - 4].dopnt.y = SelectedForm->vertices[sides - 1].y;
	}
	sequenceIndex = 0;
	SelectedPoint.x = currentFormVertices[0].x;
	SelectedPoint.y = currentFormVertices[0].y;
	if (SelectedForm->edgeType&EGUND) {
		stitchSpace = USPAC;
		setMap(UND);
		plen = SelectedForm->borderSize*URAT;
		setMap(UNDPHAS);
		rstMap(FILDIR);
		plfn(&underlayVertRect[0]);
		bpnt = sequenceIndex;
		rstMap(UNDPHAS);
		SelectedPoint.x = currentFormVertices[0].x;
		SelectedPoint.y = currentFormVertices[0].y;
		setMap(FILDIR);
		plfn(&underlayVertRect[0]);
		plbak(bpnt);
		prsmal();
		if (sequenceIndex) { //ensure that we can do a valid read from oseq
			SelectedPoint.x = oseq[sequenceIndex - 1].x;
			SelectedPoint.y = oseq[sequenceIndex - 1].y;
		}
	}
	rstMap(UND);
	stitchSpace = SelectedForm->edgeSpacing;
	plfn(&pointsVertRect[0]);
	stitchSpace = spac;
	fvars(closestFormToCursor);
}

void pbrd(double spac) {
	double			tspac;
	unsigned		ind;
	unsigned		strt;

	tspac = stitchSpace;
	stitchSpace = SelectedForm->edgeSpacing;
	sequenceIndex = 0;
	pointsVertRect = (VRCT2*)bseq;
	underlayVertRect = &pointsVertRect[sides];
	strt = getlast();
	satout(SelectedForm->borderSize);
	for (ind = 0; ind < (unsigned)sides - 1; ind++) {
		sprct(ind, ind + 1);
		spurct(ind);
	}
	sprct(ind, 0);
	spurct(ind);
	if (SelectedForm->edgeType&EGUND) {
		rstMap(SAT1);
		stitchSpace = USPAC;
		setMap(UND);
		plen = SelectedForm->borderSize*URAT;
		satout(plen);
		setMap(UNDPHAS);
		setMap(FILDIR);
		pfn(strt, &underlayVertRect[0]);
		rstMap(UNDPHAS);
		rstMap(FILDIR);
		pfn(strt, &underlayVertRect[0]);
		stitchSpace = spac;
		prsmal();
		plen = SelectedForm->borderSize;
		rstMap(UND);
	}
	pfn(strt, &pointsVertRect[0]);
	stitchSpace = tspac;
}

void prpsbrd() {
	if (SelectedForm->sides > 2) {
		deleclp(closestFormToCursor);
		SelectedForm->edgeType = EDGEPROPSAT;
		if (chku(DUND))
			SelectedForm->edgeType |= EGUND;
		bsizpar();
		SelectedForm->borderSize = borderWidth;
		SelectedForm->edgeSpacing = stitchSpace;
		SelectedForm->borderColor = ActiveColor;
		refilfn();
	}
}

void prpbrd(double spac) {
	double		tspac = stitchSpace;
	unsigned	ind;

	if (filmsgs(FML_PRPS))
		return;
	stitchSpace = spac;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			SelectedForm->borderSize = stitchSpace;
			if (chku(BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			prpsbrd();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			fvars(closestFormToCursor);
			if (chku(BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			prpsbrd();
			setMap(INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			setMap(RESTCH);
		}
	}
	stitchSpace = tspac;
}

void tglfrm() {
	if (rstMap(SATPNT))
		satfix();
	rstMap(HIDSTCH);
	if (toglMap(FRMOF))
		strcpy_s(oftxt, StringTable[STR_FRMPLUS]);
	else {
		strcpy_s(oftxt, StringTable[STR_FRMINUS]);
		rstMap(FORMSEL);
		rstMap(FORMIN);
		rstMap(MOVFRM);
		rstMap(FRMPMOV);
		rstMap(FRMOV);
		rstMap(SATIN);
		rstMap(SATPNT);
		rstMap(SATCNKT);
		rstMap(FENDIN);
		rstMap(DELFRM);
		rstMap(FRMPSEL);
		rstMap(INSFRM);
	}
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, &meninfo);
	setMap(DUMEN);
	setMap(RESTCH);
}

void frmon() {
	unbsho();
	rstMap(FRMOF);
	strcpy_s(oftxt, StringTable[STR_FRMPLUS]);
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, &meninfo);
	setMap(DUMEN);
}

void fnord() {
	int			ind;
	fPOINTATTR	tpnt;

	fvars(closestFormToCursor);
	SelectedForm = &formList[closestFormToCursor];
	for (ind = 0; ind < SelectedForm->sides >> 1; ind++) {
		tpnt.x = SelectedForm->vertices[ind].x;
		tpnt.y = SelectedForm->vertices[ind].y;
		SelectedForm->vertices[ind].x = SelectedForm->vertices[SelectedForm->sides - ind - 1].x;
		SelectedForm->vertices[ind].y = SelectedForm->vertices[SelectedForm->sides - ind - 1].y;
		SelectedForm->vertices[SelectedForm->sides - ind - 1].x = tpnt.x;
		SelectedForm->vertices[SelectedForm->sides - ind - 1].y = tpnt.y;
	}
	refil();
}

void flpord() {
	int			ind;
	unsigned	uind, strt, fin;
	fPOINTATTR	tpnt;

	fvars(closestFormToCursor);
	if (chkMap(FPSEL)) {
		savdo();
		strt = SelectedFormPoints.start;
		fin = (SelectedFormPoints.start + SelectedFormPoints.pointCount) % sides;
		for (uind = 0; uind <= SelectedFormPoints.pointCount >> 1; uind++) {
			tpnt.x = SelectedForm->vertices[strt].x;
			tpnt.y = SelectedForm->vertices[strt].y;
			SelectedForm->vertices[strt].x = SelectedForm->vertices[fin].x;
			SelectedForm->vertices[strt].y = SelectedForm->vertices[fin].y;
			SelectedForm->vertices[fin].x = tpnt.x;
			SelectedForm->vertices[fin].y = tpnt.y;
			strt = pdir(strt);
			toglMap(PSELDIR);
			fin = pdir(fin);
			toglMap(PSELDIR);
		}
		refil();
		return;
	}
	if (selectedFormCount) {
		savdo();
		for (uind = 0; uind < selectedFormCount; uind++) {
			closestFormToCursor = selectedFormList[uind];
			fnord();
		}
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			fnord();
			coltab();
			setMap(RESTCH);
		}
		else {
			if (chkMap(GRPSEL)) {
				savdo();
				rngadj();
				uind = GroupStartStitch;
				for (ind = 0; ind < ((signed)(GroupEndStitch - GroupStartStitch) >> 1) + 1; ind++) {
					tpnt.attribute = StitchBuffer[uind].attribute;
					tpnt.x = StitchBuffer[uind].x;
					tpnt.y = StitchBuffer[uind].y;
					StitchBuffer[uind].attribute = StitchBuffer[GroupEndStitch - ind].attribute;
					StitchBuffer[uind].x = StitchBuffer[GroupEndStitch - ind].x;
					StitchBuffer[uind].y = StitchBuffer[GroupEndStitch - ind].y;
					StitchBuffer[GroupEndStitch - ind].attribute = tpnt.attribute;
					StitchBuffer[GroupEndStitch - ind].x = tpnt.x;
					StitchBuffer[GroupEndStitch - ind].y = tpnt.y;
					uind++;
				}
				coltab();
				setMap(RESTCH);
			}
		}
	}
}

void dudif(dPOINT strt, dPOINT fin, dPOINT* dif) {
	dif->x = fin.x - strt.x;
	dif->y = fin.y - strt.y;
}

void trfrm(dPOINT lpnt0, dPOINT lpnt1, dPOINT rpnt0, dPOINT rpnt1) {
	unsigned	ind;
	dPOINT		tdif, bdif, ldif, rdif;
	dPOINT		clprat, tmid, bmid, mdif;

	dudif(lpnt1, rpnt1, &tdif);
	dudif(lpnt0, rpnt0, &bdif);
	dudif(lpnt0, lpnt1, &ldif);
	dudif(rpnt0, rpnt1, &rdif);
	for (ind = 0; ind < ClipStitchCount; ind++) {
		clprat.x = ClipBuffer[ind].x / ClipRectSize.cx;
		clprat.y = ClipBuffer[ind].y / ClipRectSize.cy;
		tmid.x = clprat.x*(tdif.x) + lpnt1.x;
		tmid.y = clprat.x*(tdif.y) + lpnt1.y;
		bmid.x = clprat.x*(bdif.x) + lpnt0.x;
		bmid.y = clprat.x*(bdif.y) + lpnt0.y;
		dudif(bmid, tmid, &mdif);
		oseq[activePointIndex].x = clprat.y*mdif.x + bmid.x;
		oseq[activePointIndex].y = clprat.y*mdif.y + bmid.y;
		activePointIndex++;
	}
}

void clpfm() {
	unsigned	ind, ine, cnt;
	double		lsiz, l_rsiz;
	dPOINT		ldif, rdif;
	dPOINT		lstp, rstp;
	dPOINT		lpnt0, rpnt0;
	dPOINT		lpnt1, rpnt1;

	activePointIndex = 0;
	for (ind = 0; ind < sequenceIndex - 2; ind += 2) {
		lsiz = hypot(bseq[ind + 1].x - bseq[ind].x, bseq[ind + 1].y - bseq[ind].y);
		l_rsiz = hypot(bseq[ind + 3].x - bseq[ind + 2].x, bseq[ind + 3].y - bseq[ind + 2].y);
		ldif.x = bseq[ind + 1].x - bseq[ind].x;
		ldif.y = bseq[ind + 1].y - bseq[ind].y;
		rdif.x = bseq[ind + 2].x - bseq[ind + 3].x;
		rdif.y = bseq[ind + 2].y - bseq[ind + 3].y;
		if (l_rsiz > lsiz)
			cnt = lsiz / ClipRectSize.cy;
		else
			cnt = l_rsiz / ClipRectSize.cy;
		if (!cnt)
			cnt = 1;
		lstp.x = ldif.x / cnt;
		lstp.y = ldif.y / cnt;
		rstp.x = rdif.x / cnt;
		rstp.y = rdif.y / cnt;
		lpnt1.x = bseq[ind].x;
		lpnt1.y = bseq[ind].y;
		rpnt1.x = bseq[ind + 3].x;
		rpnt1.y = bseq[ind + 3].y;
		for (ine = 0; ine < cnt; ine++) {
			lpnt0.x = lpnt1.x;
			lpnt0.y = lpnt1.y;
			rpnt0.x = rpnt1.x;
			rpnt0.y = rpnt1.y;
			lpnt1.x += lstp.x;
			lpnt1.y += lstp.y;
			rpnt1.x += rstp.x;
			rpnt1.y += rstp.y;
			if (activePointIndex > MAXSEQ - ClipStitchCount - 1)
				return;
			trfrm(lpnt0, lpnt1, rpnt0, rpnt1);
		}
	}
}

void fmclp() {
	double		tspac = stitchSpace;

	stitchSpace = ClipRectSize.cx;
	setMap(BARSAT);
	satfil();
	rstMap(BARSAT);
	clpfm();
	SelectedForm->fillType = CLPF;
	sequenceIndex = activePointIndex;
	stitchSpace = tspac;
}

void filsclp() {
	unsigned ind;

	deltx();
	fvars(closestFormToCursor);
	if (SelectedForm->type != SAT)
		SelectedForm->wordParam = 0;
	SelectedForm->type = SAT;
	SelectedForm->fillType = CLPF;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = ClipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = ClipBuffer[ind].y;
	}
	refilfn();
}

void clpfil() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThredWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN&&ClipRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							filsclp();
					}
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
				else {
					if (chkMap(FORMSEL)) {
						filsclp();
						setMap(INIT);
						coltab();
						setMap(RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void snpfn(unsigned xind, unsigned len) {
	unsigned	ind, ine, fin, ref, chk;
	double		tlen;

	fin = xHistogram[xind + len];
	ind = xHistogram[xind];
	if (fin - ind) {
		for (ind = xHistogram[xind]; ind < xHistogram[xind + 1]; ind++) {
			ref = xPoints[ind];
			for (ine = ind + 1; ine < fin; ine++) {
				chk = xPoints[ine];
				tlen = hypot(StitchBuffer[chk].x - StitchBuffer[ref].x, StitchBuffer[chk].y - StitchBuffer[ref].y);
				if (tlen < snapLength) {
					StitchBuffer[chk].x = StitchBuffer[ref].x;
					StitchBuffer[chk].y = StitchBuffer[ref].y;
				}
			}
		}
	}
}

void nutim(double pl_Size) {
	hTime = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		0,
		StitchWindowSize.x,
		ButtonHeight,
		ThredWindow,
		NULL,
		ThredInstance,
		NULL);
	timeDC = GetDC(hTime);
	timeStep = (double)StitchWindowSize.x / pl_Size;
	timePosition = 0;
	formLines[0].y = 0;
	formLines[1].y = ButtonHeight;
	formLines[0].x = formLines[1].x = 0;
	SelectObject(timeDC, UserPen[0]);
}

void nxtim() {
	Polyline(timeDC, formLines, 2);
	timePosition += timeStep;
	formLines[0].x = formLines[1].x = timePosition;
}

void snp(unsigned strt, unsigned fin) {
	unsigned	ind, tuns, acc, chklen, at = 0;
	fPOINT		l_rsiz;
	unsigned*	txhst;

	chkrng(&l_rsiz);
	xPoints = (unsigned*)bseq;
	ZeroMemory(bseq, 65536 * sizeof(unsigned));
	xHistogram = txhst = new unsigned[static_cast<int>(l_rsiz.x) + 1];
	for (ind = 0; ind < l_rsiz.x; ind++)
		xHistogram[ind] = 0;
	if (chkMap(FORMSEL)) {
		at = (closestFormToCursor << 4)&FRMSK;
		for (ind = strt; ind < fin; ind++) {
			if (!(StitchBuffer[ind].attribute&NOTFRM) && (StitchBuffer[ind].attribute&FRMSK) == at) {
				tuns = StitchBuffer[ind].x;
				xHistogram[tuns]++;
			}
		}
	}
	else {
		for (ind = strt; ind < fin; ind++) {
			tuns = StitchBuffer[ind].x;
			xHistogram[tuns]++;
		}
	}
	acc = 0;
	for (ind = 0; ind < l_rsiz.x; ind++) {
		tuns = xHistogram[ind];
		xHistogram[ind] = acc;
		acc += tuns;
	}
	xHistogram[ind] = acc;
	if (chkMap(FORMSEL)) {
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if (!(StitchBuffer[ind].attribute&NOTFRM) && (StitchBuffer[ind].attribute&FRMSK) == at) {
				tuns = StitchBuffer[ind].x;
				xPoints[xHistogram[tuns]++] = ind;
			}
		}
	}
	else {
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			tuns = StitchBuffer[ind].x;
			xPoints[xHistogram[tuns]++] = ind;
		}
	}
	xHistogram = &xHistogram[1];
	chklen = snapLength * 2 + 1;
	nutim(l_rsiz.x);
	for (ind = 0; ind < l_rsiz.x - chklen; ind++) {
		snpfn(ind, chklen);
		nxtim();
	}
	DestroyWindow(hTime);
	delete[] txhst;
}

void snap() {
	savdo();
	if (chkMap(GRPSEL)) {
		rngadj();
		snp(GroupStartStitch, GroupEndStitch);
	}
	else
		snp(0, PCSHeader.stitchCount);
	coltab();
	setMap(RESTCH);
}

void setcmap(unsigned bpnt) {
#if	 __UseASM__
	_asm {
		mov		eax, colorBitmap
		mov		ebx, bpnt
		bts		eax, ebx
		mov		colorBitmap, eax
	}
#else
	_bittestandset((long *)&colorBitmap, bpnt);
#endif
}

unsigned nxtcol() {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, colorBitmap
		mov		ecx, UnderlayColor
		bt		ebx, ecx
		jnc		short nxtcol1
		mov		eax, ecx
		jmp		short nxtcolx
		nxtcol1 : bsf		eax, ebx
				  jne		short nxtcolx
				  dec		eax
				  nxtcolx : btc		ebx, eax
							mov		colorBitmap, ebx
	}
#else
	DWORD col;
	if (_bittest((long *)&colorBitmap, UnderlayColor)) {
		col = UnderlayColor;
	}
	else {
		if (colorBitmap == 0) {
			return 0xffffffff;
		}

		_BitScanForward(&col, colorBitmap);
	}

	_bittestandcomplement((long *)&colorBitmap, col);

	return col;
#endif
}

unsigned isrt(unsigned bpnt) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, offset srtmsk
		mov		ecx, bpnt
		bt[ebx], ecx
		jnc		isrtx
		inc		eax
		isrtx :
	}
#else
	return _bittest((long *)&srtmsk, bpnt);
#endif
}

unsigned prgflg(unsigned ind) {
	if ((StitchBuffer[ind].attribute&TYPMSK) == FRMBFIL)
		return isrt(formList[(StitchBuffer[ind].attribute&FRMSK) >> 4].edgeType&NEGUND);
	else
		return 0;
}

void rotpar() {
	if (IniFile.rotationAngle < (PI / 180))
		tabmsg(IDS_ROTIN);
	if (chkMap(FPSEL)) {
		RotationCenter.x = midl(SelectedPointsRect.right, SelectedPointsRect.left);
		RotationCenter.y = midl(SelectedPointsRect.top, SelectedPointsRect.bottom);
		return;
	}
	if (chkMap(BIGBOX)) {
		RotationCenter.x = midl(allItemsRectangle.right, allItemsRectangle.left);
		RotationCenter.y = midl(allItemsRectangle.top, allItemsRectangle.bottom);
		return;
	}
	if (selectedFormCount) {
		if (chkMap(GMRK)) {
			RotationCenter.x = ZoomMarkPoint.x;
			RotationCenter.y = ZoomMarkPoint.y;
		}
		else
			dufcntr(&RotationCenter);
		setMap(FRMSROT);
		return;
	}
	if (chkMap(FORMSEL)) {
		RotationRect.left = SelectedForm->rectangle.left;
		RotationRect.right = SelectedForm->rectangle.right;
		RotationRect.top = SelectedForm->rectangle.top;
		RotationRect.bottom = SelectedForm->rectangle.bottom;
		if (!chkMap(GMRK)) {
			RotationCenter.x = (double)(RotationRect.right - RotationRect.left) / 2 + RotationRect.left;
			RotationCenter.y = (double)(RotationRect.top - RotationRect.bottom) / 2 + RotationRect.bottom;
		}
		setMap(FRMROT);
	}
	else {
		if (chkMap(GRPSEL)) {
			rngadj();
			selRct(&RotationRect);
		}
	}
	if (chkMap(GMRK)) {
		RotationCenter.x = ZoomMarkPoint.x;
		RotationCenter.y = ZoomMarkPoint.y;
	}
	else {
		RotationCenter.x = (double)(RotationRect.right - RotationRect.left) / 2 + RotationRect.left;
		RotationCenter.y = (double)(RotationRect.top - RotationRect.bottom) / 2 + RotationRect.bottom;
	}
}

void rotentr() {
	TCHAR buf[HBUFSIZ];

	LoadString(ThredInstance, IDS_ROTA, buf, HBUFSIZ);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), buf, RotationAngle / PI * 180);
	shoMsg(MsgBuffer);
	setMap(NUMIN);
	numWnd();
}

void setrang() {
	RotationAngle = IniFile.rotationAngle;
	rotentr();
	setMap(NUROT);
}

void rotcmd() {
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
		RotationAngle = IniFile.rotationAngle;
		rotpar();
		rotentr();
		setMap(ENTROT);
	}
	else
		shoseln(IDS_ALLX, IDS_ROTCMD);
}

void fnagain() {
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
		rotpar();
		rotfn();
	}
	else
		alrotmsg();
}

void rotagain() {
	RotationAngle = IniFile.rotationAngle;
	fnagain();
}

void bakagain() {
	RotationAngle = 2 * PI - IniFile.rotationAngle;
	fnagain();
}

void rotdup() {
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || selectedFormCount) {
		RotationAngle = IniFile.rotationAngle;
		rotpar();
		rotentr();
		setMap(ENTRDUP);
	}
	else
		shord();
}

void adfrm(unsigned fpnt) {
	FRMHED*		nupnt;

	SelectedForm = &formList[fpnt];
	nupnt = &formList[formIndex];
	frmcpy(nupnt, SelectedForm);
	closestFormToCursor = formIndex;
	nupnt->vertices = adflt(SelectedForm->sides);
	mvflpnt(nupnt->vertices, SelectedForm->vertices, SelectedForm->sides);
	if (nupnt->type == SAT&&nupnt->satinGuideCount) {
		nupnt->satinOrAngle.sac = adsatk(nupnt->satinGuideCount);
		mvsatk(nupnt->satinOrAngle.sac, SelectedForm->satinOrAngle.sac, SelectedForm->satinGuideCount);
	}
	if (iseclpx(formIndex)) {
		nupnt->borderClipData = adclp(nupnt->clipEntries);
		mvflpnt(nupnt->borderClipData, SelectedForm->borderClipData, SelectedForm->clipEntries);
	}
	if (isclpx(formIndex)) {
		nupnt->angleOrClipData.clip = adclp(nupnt->lengthOrCount.clipCount);
		mvflpnt(nupnt->angleOrClipData.clip, SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
	}
	formIndex++;
}

void duprot() {
	adfrm(closestFormToCursor);
	rotfn();
	refil();
	setMap(FORMSEL);
	setMap(RESTCH);
}

void duprotfs() {
	unsigned	l_bakpnt, ind;

	l_bakpnt = formIndex;
	rotpar();
	for (ind = 0; ind < selectedFormCount; ind++)
		adfrm(selectedFormList[ind]);
	formIndex = l_bakpnt;
	for (ind = 0; ind < selectedFormCount; ind++)
		selectedFormList[ind] = formIndex++;
	fnagain();
}

void duprots() {
	unsigned src, dst;

	rngadj();
	dst = PCSHeader.stitchCount;
	for (src = GroupStartStitch; src <= GroupEndStitch; src++) {
		StitchBuffer[dst].x = StitchBuffer[src].x;
		StitchBuffer[dst].y = StitchBuffer[src].y;
		StitchBuffer[dst++].attribute = StitchBuffer[src].attribute&(~(FRMSK | TYPMSK));
	}
	ClosestPointIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount += (GroupEndStitch - GroupStartStitch);
	GroupStitchIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount++;
	rngadj();
	rotfn();
	coltab();
	setMap(RESTCH);
}

void cplayfn(unsigned fpnt, unsigned play) {
	FRMHED*		l_spnt;

	l_spnt = &formList[fpnt];
	fvars(formIndex);
	frmcpy(SelectedForm, l_spnt);
	fvars(formIndex);
	SelectedForm->vertices = adflt(SelectedForm->sides);
	MoveMemory(SelectedForm->vertices, l_spnt->vertices, sides * sizeof(fPOINT));
	if (SelectedForm->type == SAT&&SelectedForm->satinGuideCount)
	{
		SelectedForm->satinOrAngle.sac = adsatk(SelectedForm->satinGuideCount);
		MoveMemory(SelectedForm->satinOrAngle.sac, l_spnt->satinOrAngle.sac, SelectedForm->satinGuideCount * sizeof(SATCON));
	}
	SelectedForm->clipEntries = 0;
	SelectedForm->fillType = 0;
	SelectedForm->lengthOrCount.clipCount = 0;;
	SelectedForm->edgeType = 0;
	SelectedForm->fillInfo.texture.index = 0;
	SelectedForm->attribute = formList[formIndex].attribute&NFRMLMSK;
	SelectedForm->attribute |= play;
	formIndex++;
}

void cpylayr(unsigned play) {
	unsigned ind, ine, playcod;

	if (selectedFormCount) {
		savdo();
		for (ind = 0; ind < selectedFormCount; ind++)
			cplayfn(selectedFormList[ind], play);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			cplayfn(closestFormToCursor, play);
		}
		else {
			if (chkMap(GRPSEL)) {
				savdo();
				playcod = play << (LAYSHFT - 1);
				rngadj();
				ine = PCSHeader.stitchCount;
				for (ind = GroupStartStitch; ind < GroupEndStitch; ind++) {
					StitchBuffer[ine].attribute = StitchBuffer[ind].attribute&NLAYMSK | playcod;
					StitchBuffer[ine].x = StitchBuffer[ind].x;
					StitchBuffer[ine++].y = StitchBuffer[ind].y;
				}
				PCSHeader.stitchCount = ine;
				coltab();
				setMap(RESTCH);
			}
		}
	}
}

void movlayr(unsigned play) {
	unsigned slay, ind, ine;
	unsigned stchcod;

	stchcod = play << (LAYSHFT - 1);
	if (selectedFormCount) {
		savdo();
		clRmap((formIndex >> 5) + 1);
		for (ind = 0; ind < selectedFormCount; ind++) {
			ine = selectedFormList[ind];
			slay = (formList[ine].attribute&FRMLMSK) >> 1;
			formList[ine].attribute = (TCHAR)(formList[ine].attribute&NFRMLMSK) | play;
			setr(ine);
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if (StitchBuffer[ind].attribute&ALTYPMSK) {
				ine = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
				if (chkr(ine))
					StitchBuffer[ind].attribute = StitchBuffer[ind].attribute&NLAYMSK | stchcod;
			}
		}
		selectedFormCount = 0;
		rstMap(FORMSEL);
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			slay = (formList[closestFormToCursor].attribute&FRMLMSK) >> 1;
			formList[closestFormToCursor].attribute = (TCHAR)(formList[closestFormToCursor].attribute&NFRMLMSK) | play;
			rstMap(FORMSEL);
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if (StitchBuffer[ind].attribute&ALTYPMSK && ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) == closestFormToCursor)
					StitchBuffer[ind].attribute = StitchBuffer[ind].attribute&NLAYMSK | stchcod;
			}
			setMap(RESTCH);
		}
		else {
			if (chkMap(GRPSEL)) {
				savdo();
				rngadj();
				for (ind = GroupStartStitch; ind < GroupEndStitch; ind++)
					StitchBuffer[ind].attribute = StitchBuffer[ind].attribute&NLAYMSK | stchcod;
				setMap(RESTCH);
			}
		}
	}
}

void join() {
	unsigned	l_bakpnt, ind, l_sids;
	fPOINT*		tflt;
	fPOINT*		ipnt;

	l_bakpnt = closestFormToCursor;
	setMap(FRMSAM);
	if (formIndex > 1 && chkMap(FORMSEL) && closfrm()) {
		l_sids = formList[closestFormToCursor].sides;
		tflt = (fPOINT*)&bseq;
		for (ind = 0; ind < l_sids; ind++) {
			tflt[ind].x = formList[closestFormToCursor].vertices[closestVertexToCursor].x;
			tflt[ind].y = formList[closestFormToCursor].vertices[closestVertexToCursor].y;
			closestVertexToCursor = nxt(closestVertexToCursor);
		}
		setMap(DELTO);
		frmdel();
		if (l_bakpnt > closestFormToCursor)
			closestFormToCursor = l_bakpnt - 1;
		else
			closestFormToCursor = l_bakpnt;
		ipnt = &formList[closestFormToCursor].vertices[formList[closestFormToCursor].sides];
		fltspac(ipnt, l_sids);
		for (ind = 0; ind < l_sids; ind++) {
			ipnt[ind].x = tflt[ind].x;
			ipnt[ind].y = tflt[ind].y;
		}
		SelectedForm = &formList[closestFormToCursor];
		SelectedForm->sides += l_sids;
		SelectedForm->rectangle.left = SelectedForm->rectangle.right = SelectedForm->vertices[0].x;
		SelectedForm->rectangle.top = SelectedForm->rectangle.bottom = SelectedForm->vertices[0].y;
		for (ind = 1; ind < SelectedForm->sides; ind++) {
			if (SelectedForm->vertices[ind].x < SelectedForm->rectangle.left)
				SelectedForm->rectangle.left = SelectedForm->vertices[ind].x;
			if (SelectedForm->vertices[ind].x > SelectedForm->rectangle.right)
				SelectedForm->rectangle.right = SelectedForm->vertices[ind].x;
			if (SelectedForm->vertices[ind].y > SelectedForm->rectangle.top)
				SelectedForm->rectangle.top = SelectedForm->vertices[ind].y;
			if (SelectedForm->vertices[ind].y < SelectedForm->rectangle.bottom)
				SelectedForm->rectangle.bottom = SelectedForm->vertices[ind].y;
		}
		refil();
		coltab();
		setMap(RESTCH);
	}
	rstMap(FRMSAM);
}

BOOL chkbfil() {
	switch (SelectedForm->edgeType) {
	case EDGELINE:
	case EDGEBEAN:
	case EDGEANGSAT:
	case EDGEAPPL:
	case EDGEPROPSAT:
	case EDGEBHOL:
	case EDGEPICOT:
	case EDGECLIP:
		return 1;
	}
	return 0;
}

void refilal() {
	unsigned l_bakpnt;

	savdo();
	l_bakpnt = closestFormToCursor;
	for (closestFormToCursor = 0; closestFormToCursor < formIndex; closestFormToCursor++)
		refilfn();
	closestFormToCursor = l_bakpnt;
	fvars(closestFormToCursor);
	coltab();
	setMap(RESTCH);
}

BOOL notsel() {
	unsigned ind;

	for (ind = 0; ind < selectedFormCount; ind++) {
		if ((unsigned)selectedFormList[ind] == closestFormToCursor)
			return 0;
	}
	return 1;
}

void nufsel() {
	if (formIndex) {
		rstMap(FORMSEL);
		if (rstMap(WASEL))
			selectedFormList[selectedFormCount++] = previousFormIndex;
		if (notsel())
			selectedFormList[selectedFormCount++] = (unsigned short)closestFormToCursor;
		setMap(RESTCH);
	}
}

void frmadj(unsigned find) {
	unsigned ind;

	fvars(find);
	for (ind = 0; ind < SelectedForm->sides; ind++) {
		currentFormVertices[ind].x += formMoveDelta.x;
		currentFormVertices[ind].y -= formMoveDelta.y;
	}
	frmout(find);
}

void setr(unsigned pbit) {
#if	 __UseASM__
	_asm {
		mov		ebx, offset MarkedStitchMap
		mov		eax, pbit
		bts[ebx], eax
	}
#else
	_bittestandset((long *)MarkedStitchMap, pbit);
#endif
}

void clRmap(unsigned len) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ecx, len
		mov		edi, offset MarkedStitchMap
		rep		stosd
	}
#else
	memset(MarkedStitchMap, 0, len * sizeof(*MarkedStitchMap));
#endif
}

#if PESACT

BOOL setrc(unsigned pbit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, offset MarkedStitchMap
		mov		ecx, pbit
		bts[ebx], ecx
		jnc		short setrcx
		inc		eax
		setrcx :
	}
#else
	return _bittestandset((long *)MarkedStitchMap, pbit);
#endif
}

#endif

BOOL chkr(unsigned pbit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ecx, pbit
		mov		ebx, offset MarkedStitchMap
		bt[ebx], ecx
		jnc		chkrx
		inc		eax
		chkrx :
	}
#else
	return _bittest((long *)MarkedStitchMap, pbit);
#endif
}

void frmsadj() {
	unsigned ind;

	clRmap(RMAPSIZ);
	for (ind = 0; ind < selectedFormCount; ind++)
		setr(selectedFormList[ind]);
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		if (StitchBuffer[ind].attribute&ALTYPMSK&&chkr((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {
			StitchBuffer[ind].x += formMoveDelta.x;
			StitchBuffer[ind].y -= formMoveDelta.y;
		}
	}
}

void stchrct2px(fRECTANGLE srct, RECT* prct) {
	dPOINT	ipnt;
	POINT	l_opnt;

	ipnt.x = srct.left;
	ipnt.y = srct.top;
	sCor2px(ipnt, &l_opnt);
	prct->left = l_opnt.x;
	prct->top = l_opnt.y;
	ipnt.x = srct.right;
	ipnt.y = srct.bottom;
	sCor2px(ipnt, &l_opnt);
	prct->right = l_opnt.x;
	prct->bottom = l_opnt.y;
}

void getbig() {
	unsigned	ind;
	fRECTANGLE*		trct;

	allItemsRectangle.bottom = allItemsRectangle.left = 1e9;
	allItemsRectangle.top = allItemsRectangle.right = 0;
	for (ind = 0; ind < formIndex; ind++) {
		trct = &formList[ind].rectangle;
		if (trct->bottom < allItemsRectangle.bottom)
			allItemsRectangle.bottom = trct->bottom;
		if (trct->left < allItemsRectangle.left)
			allItemsRectangle.left = trct->left;
		if (trct->right > allItemsRectangle.right)
			allItemsRectangle.right = trct->right;
		if (trct->top > allItemsRectangle.top)
			allItemsRectangle.top = trct->top;
	}
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		if (StitchBuffer[ind].x < allItemsRectangle.left)
			allItemsRectangle.left = StitchBuffer[ind].x;
		if (StitchBuffer[ind].x > allItemsRectangle.right)
			allItemsRectangle.right = StitchBuffer[ind].x;
		if (StitchBuffer[ind].y < allItemsRectangle.bottom)
			allItemsRectangle.bottom = StitchBuffer[ind].y;
		if (StitchBuffer[ind].y > allItemsRectangle.top)
			allItemsRectangle.top = StitchBuffer[ind].y;
	}
}

void selal() {
	rstMap(FORMSEL);
	selectedFormCount = 0;
	rstMap(SELBOX);
	rstMap(GRPSEL);
	getbig();
	ZoomRect.bottom = 0;
	ZoomRect.left = 0;
	ZoomRect.right = UnzoomedRect.x;
	ZoomRect.top = UnzoomedRect.y;
	ZoomFactor = 1;
	rstMap(ZUMED);
	movStch();
	NearestCount = 0;
	rstMap(RUNPAT);
	duzrat();
	stchrct2px(allItemsRectangle, &selectedFormsRectangle);
	setMap(BIGBOX);
	setMap(RESTCH);
}

void frmpnts(unsigned typ) {
	unsigned ind = 0;
	unsigned trg = (closestFormToCursor << 4) | typ;

	while (ind < PCSHeader.stitchCount && (StitchBuffer[ind].attribute&(ALTYPMSK | FRMSK)) != trg)
		ind++;
	ClosestPointIndex = ind;
	if (PCSHeader.stitchCount > 0) {
		ind = PCSHeader.stitchCount - 1;
	}
	else {
		ind = 0;
	}
	while (ind > ClosestPointIndex && (StitchBuffer[ind].attribute&(ALTYPMSK | FRMSK)) != trg)
		ind--;
	GroupStitchIndex = ind;
}

void selfil(unsigned typ) {
	frm1pnt();
	if (chkMap(FORMSEL)) {
		if (typ == FRMFIL && !formList[closestFormToCursor].fillType) {
			tabmsg(IDS_FSELF);
			return;
		}
		if (typ == FRMBFIL && !formList[closestFormToCursor].edgeType) {
			tabmsg(IDS_FSELB);
			return;
		}
		if (typ == FRMAPFIL && (formList[closestFormToCursor].edgeType&NEGUND) != EDGEAPPL) {
			tabmsg(IDS_FSELA);
			return;
		}
		frmpnts(typ);
		setMap(GRPSEL);
		rstMap(FORMSEL);
		rngadj();
		setMap(RESTCH);
	}
	else
		tabmsg(IDS_SEL1FRM);
}

BOOL notfstch(unsigned at)
{
	if (at&NOTFRM)
		return 1;
	if (((at&FRMSK) >> FRMSHFT) == closestFormToCursor)
		return 0;
	return 1;
}

void selalfil() {
	frm1pnt();
	if (chkMap(FORMSEL)) {
		ClosestPointIndex = 0;
		while (ClosestPointIndex < PCSHeader.stitchCount&&notfstch(StitchBuffer[ClosestPointIndex].attribute))
			ClosestPointIndex++;
		if (ClosestPointIndex != PCSHeader.stitchCount)
		{
			if (ClosestPointIndex)
				ClosestPointIndex--;
			GroupStitchIndex = PCSHeader.stitchCount - 1;
			while (GroupStitchIndex > ClosestPointIndex&&notfstch(StitchBuffer[GroupStitchIndex].attribute))
				GroupStitchIndex--;
			setMap(GRPSEL);
			rstMap(FORMSEL);
			rngadj();
			setMap(RESTCH);
		}
		else
			tabmsg(IDS_FSELM);
	}
	else
		tabmsg(IDS_SEL1FRM);
}

BOOL frmrng(unsigned fpnt, RANGE* rng) {
	rng->start = 0;
	rng->finish = PCSHeader.stitchCount;
	if (formList[fpnt].fillType || formList[fpnt].edgeType) {
		while (rng->start < PCSHeader.stitchCount&&notfstch(StitchBuffer[rng->start].attribute))
			rng->start++;
		rng->finish = PCSHeader.stitchCount - 1;
		while (rng->finish > rng->start&&notfstch(StitchBuffer[rng->finish].attribute))
			rng->finish--;
		if (rng->finish > rng->start)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void bhfn(unsigned strt, unsigned fin, double spac) {
	double		len;
	dPOINT		dif, stp, ostp, pnti, pntf, pnto;
	unsigned	cnt, ind;

	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	RotationAngle = atan2(-dif.x, dif.y);
	ostp.x = SelectedForm->borderSize*cos(RotationAngle);
	ostp.y = SelectedForm->borderSize*sin(RotationAngle);
	len = hypot(dif.x, dif.y);
	cnt = len / spac;
	stp.x = dif.x / cnt;
	stp.y = dif.y / cnt;
	pnti.x = currentFormVertices[strt].x;
	pnti.y = currentFormVertices[strt].y;
	if (cnt) {
		for (ind = 0; ind < cnt - 1; ind++) {
			pntf.x = pnti.x + stp.x;
			pntf.y = pnti.y + stp.y;
			pnto.x = pntf.x + ostp.x;
			pnto.y = pntf.y + ostp.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			oseq[sequenceIndex].x = pnti.x;
			oseq[sequenceIndex++].y = pnti.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			oseq[sequenceIndex].x = pnto.x;
			oseq[sequenceIndex++].y = pnto.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			oseq[sequenceIndex].x = pnto.x;
			oseq[sequenceIndex++].y = pnto.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			pnti.x += stp.x;
			pnti.y += stp.y;
		}
		pntf.x = pnti.x + stp.x;
		pntf.y = pnti.y + stp.y;
		oseq[sequenceIndex].x = pntf.x;
		oseq[sequenceIndex++].y = pntf.y;
		oseq[sequenceIndex].x = pnti.x;
		oseq[sequenceIndex++].y = pnti.y;
	}
}

void bhcrnr(unsigned p_lin) {
	unsigned	tlin = nxt(p_lin);
	dPOINT		dif, pnt;
	double		len, rat;

	if (chkMap(INDIR))
	{
		dif.x = outsidePoints[tlin].x - currentFormVertices[tlin].x;
		dif.y = outsidePoints[tlin].y - currentFormVertices[tlin].y;
	}
	else
	{
		dif.x = insidePoints[tlin].x - currentFormVertices[tlin].x;
		dif.y = insidePoints[tlin].y - currentFormVertices[tlin].y;
	}
	len = hypot(dif.x, dif.y);
	rat = buttonholeFillCornerLength / len;
	dif.x *= rat;
	dif.y *= rat;
	pnt.x = currentFormVertices[tlin].x + dif.x;
	pnt.y = currentFormVertices[tlin].y + dif.y;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	oseq[sequenceIndex].x = pnt.x;
	oseq[sequenceIndex++].y = pnt.y;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	oseq[sequenceIndex].x = pnt.x;
	oseq[sequenceIndex++].y = pnt.y;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
}

void bhbrd(double spac) {
	unsigned short	slin;
	unsigned short	nlin, ind;

	slin = getlast();
	sequenceIndex = 0;
	oseq[sequenceIndex].x = currentFormVertices[slin].x;
	oseq[sequenceIndex++].y = currentFormVertices[slin].y;
	for (ind = 0; ind < sides; ind++) {
		nlin = nxt(slin);
		bhfn(slin, nlin, spac);
		bhcrnr(slin);
		slin = nlin;
	}
}

void blbrd(double spac) {
	unsigned ind;

	sequenceIndex = 0;
	oseq[sequenceIndex].x = currentFormVertices[0].x;
	oseq[sequenceIndex++].y = currentFormVertices[0].y;
	for (ind = 0; ind < (unsigned)sides - 2; ind++) {
		bhfn(ind, ind + 1, spac);
		bhcrnr(ind);
	}
	bhfn(ind, ind + 1, spac);
	oseq[sequenceIndex].x = currentFormVertices[sides - 1].x;
	oseq[sequenceIndex++].y = currentFormVertices[sides - 1].y;
}

void bholbrd() {
	deleclp(closestFormToCursor);
	SelectedForm->borderSize = borderWidth;
	bsizpar();
	SelectedForm->edgeType = EDGEBHOL;
	SelectedForm->edgeSpacing = stitchSpace;
	SelectedForm->borderColor = ActiveColor;
	savblen(buttonholeFillCornerLength);
	refilfn();
}

void bhol() {
	double		tspac = stitchSpace;
	unsigned	ind;

	if (filmsgs(FML_BHOL))
		return;
	stitchSpace = tspac;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			bholbrd();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			fvars(closestFormToCursor);
			bholbrd();
			setMap(INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			setMap(RESTCH);
		}
	}
	stitchSpace = tspac;
}

void fcntr() {
	unsigned	ind, ine, inf, tat;
	dRECTANGLE		avrct;
	dPOINT		avcntr;
	dPOINT		cntr;
	dPOINT		dif;

	avrct.bottom = avrct.left = avrct.right = avrct.top = 0;
	if (selectedFormCount) {
		savdo();
		ine = selectedFormList[0];
		avcntr.x = (formList[ine].rectangle.right - formList[ine].rectangle.left) / 2 + formList[ine].rectangle.left;
		avcntr.y = (formList[ine].rectangle.top - formList[ine].rectangle.bottom) / 2 + formList[ine].rectangle.bottom;
		for (ind = 1; ind < selectedFormCount; ind++) {
			ine = selectedFormList[ind];
			cntr.x = (formList[ine].rectangle.right - formList[ine].rectangle.left) / 2 + formList[ine].rectangle.left;
			cntr.y = (formList[ine].rectangle.top - formList[ine].rectangle.bottom) / 2 + formList[ine].rectangle.bottom;
			dif.x = avcntr.x - cntr.x;
			dif.y = avcntr.y - cntr.y;
			SelectedForm = &formList[ine];
			for (inf = 0; inf < SelectedForm->sides; inf++) {
				SelectedForm->vertices[inf].x += dif.x;
				SelectedForm->vertices[inf].y += dif.y;
			}
			frmout(ine);
			tat = (ine << 4);
			for (inf = 0; inf < PCSHeader.stitchCount; inf++) {
				if ((StitchBuffer[inf].attribute&FRMSK) == tat && !(StitchBuffer[ind].attribute&NOTFRM)) {
					StitchBuffer[inf].x += dif.x;
					StitchBuffer[inf].y += dif.y;
				}
			}
		}
		setMap(RESTCH);
	}
	else
		tabmsg(IDS_SELC);
}

void boxsel() {
	rstMap(INSRT);
	if (!chkMap(THUMSHO)) {
		rstMap(BZUM);
		rstMap(BZUMIN);
		setMap(VCAPT);
		SetCapture(ThredWindow);
		setMap(RESTCH);
	}
}

void clpcrnr(unsigned p_lin) {
	unsigned	tlin = nxt(p_lin);
	unsigned	ind;
	dPOINT		dif, pnt;
	fPOINT		tpnt;
	double		len, rat;
	fPOINTATTR	rpnt;

	rpnt.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	rpnt.y = ClipRect.top;
	if (chkMap(INDIR))
	{
		dif.x = outsidePoints[tlin].x - currentFormVertices[tlin].x;
		dif.y = outsidePoints[tlin].y - currentFormVertices[tlin].y;
	}
	else
	{
		dif.x = insidePoints[tlin].x - currentFormVertices[tlin].x;
		dif.y = insidePoints[tlin].y - currentFormVertices[tlin].y;
	}
	RotationAngle = atan2(dif.y, dif.x) + PI / 2;
	rotang1(rpnt, &clipReference);
	for (ind = 0; ind < ClipStitchCount; ind++)
		rotang1(ClipBuffer[ind], &clipFillData[ind]);
	len = hypot(dif.x, dif.y);
	rat = getplen() / len;
	dif.x *= rat;
	dif.y *= rat;
	pnt.x = currentFormVertices[tlin].x + dif.x;
	pnt.y = currentFormVertices[tlin].y + dif.y;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	oseq[sequenceIndex].x = pnt.x;
	oseq[sequenceIndex++].y = pnt.y;
	oseq[sequenceIndex].x = currentFormVertices[tlin].x;
	oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	oseq[sequenceIndex].x = pnt.x;
	oseq[sequenceIndex++].y = pnt.y;
	tpnt.x = pnt.x;
	tpnt.y = pnt.y;
	if (!ritclp(tpnt)) {
		oseq[sequenceIndex].x = pnt.x;
		oseq[sequenceIndex++].y = pnt.y;
		oseq[sequenceIndex].x = currentFormVertices[tlin].x;
		oseq[sequenceIndex++].y = currentFormVertices[tlin].y;
	}
}

void picfn(unsigned strt, unsigned fin, double spac) {
	double		len, tdub;
	dPOINT		dif, stp, ostp, pnti, pntf, pnto;
	fPOINT		tpnt;
	fPOINTATTR	rpnt;
	unsigned	cnt, ind;

	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	RotationAngle = atan2(-dif.x, dif.y);
	ostp.x = SelectedForm->borderSize*cos(RotationAngle);
	ostp.y = SelectedForm->borderSize*sin(RotationAngle);
	len = hypot(dif.x, dif.y);
	spac += ClipRectSize.cx;
	cnt = len / spac;
	rpnt.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	rpnt.y = ClipRect.top;
	RotationAngle = atan2(dif.y, dif.x);
	rotang1(rpnt, &clipReference);
	pnti.x = currentFormVertices[strt].x;
	pnti.y = currentFormVertices[strt].y;
	if (cnt) {
		stp.x = 0;
		stp.y = 0;
		if (cnt > 1) {
			tdub = ((len - cnt*spac) / (cnt - 1) + spac) / len;
			stp.x = dif.x*tdub;
			stp.y = dif.y*tdub;
		}
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotang1(ClipBuffer[ind], &clipFillData[ind]);
		for (ind = 0; ind < cnt - 1; ind++) {
			pntf.x = pnti.x + stp.x;
			pntf.y = pnti.y + stp.y;
			pnto.x = pntf.x + ostp.x;
			pnto.y = pntf.y + ostp.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			oseq[sequenceIndex].x = pnti.x;
			oseq[sequenceIndex++].y = pnti.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			oseq[sequenceIndex].x = pnto.x;
			oseq[sequenceIndex++].y = pnto.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			oseq[sequenceIndex].x = pnto.x;
			oseq[sequenceIndex++].y = pnto.y;
			tpnt.x = pnto.x;
			tpnt.y = pnto.y;
			if (ritclp(tpnt))
				goto picfnx;
			oseq[sequenceIndex].x = pnto.x;
			oseq[sequenceIndex++].y = pnto.y;
			oseq[sequenceIndex].x = pntf.x;
			oseq[sequenceIndex++].y = pntf.y;
			pnti.x += stp.x;
			pnti.y += stp.y;
		}
		oseq[sequenceIndex].x = currentFormVertices[fin].x;
		oseq[sequenceIndex++].y = currentFormVertices[fin].y;
		oseq[sequenceIndex].x = pnti.x;
		oseq[sequenceIndex++].y = pnti.y;
	}
picfnx:;
}

void clpic(unsigned short strtlin) {
	unsigned		ind;
	unsigned short	nlin;

	sequenceIndex = 0;
	rstMap(CLPBAK);
	plen = ClipRectSize.cx / 2;
	plen2 = ClipRectSize.cx;
	clipFillData = new fPOINT[ClipStitchCount];
	RotationCenter.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	clipReference.y = RotationCenter.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
	clipReference.x = ClipRect.left;
	satout(20);
	if (SelectedForm->type == FRMLINE) {
		for (ind = 0; ind < (unsigned)sides - 2; ind++) {
			picfn(ind, ind + 1, SelectedForm->edgeSpacing);
			clpcrnr(ind);
		}
		picfn(ind, ind + 1, SelectedForm->edgeSpacing);
	}
	else {
		if (!SelectedForm->fillType) {
			oseq[sequenceIndex].x = currentFormVertices[strtlin].x;
			oseq[sequenceIndex++].y = currentFormVertices[strtlin].y;
		}
		for (ind = 0; ind < sides; ind++) {
			nlin = nxt(strtlin);
			picfn(strtlin, nlin, SelectedForm->edgeSpacing);
			clpcrnr(strtlin);
			strtlin = nlin;
		}
		oseq[sequenceIndex].x = currentFormVertices[strtlin].x;
		oseq[sequenceIndex++].y = currentFormVertices[strtlin].y;
	}
	delete[] clipFillData;
}

void fspic() {
	unsigned	ind;

	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGEPICOT;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(closestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = picotSpace;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	savplen(buttonholeFillCornerLength);
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = ClipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = ClipBuffer[ind].y;
	}
	plen = ClipRectSize.cy / 2;
	refilfn();
}

void picot() {
	unsigned ind;

	if (filmsgs(FML_PIC))
		return;
	if (OpenClipboard(ThredWindow)) {
		savdo();
		fvars(closestFormToCursor);
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (selectedFormCount) {
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						fspic();
					}
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
				else {
					if (chkMap(FORMSEL)) {
						fspic();
						setMap(INIT);
						coltab();
						setMap(RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void contf() {
	unsigned	ind, loind, hind, selind, lolins, hilins, selins, strt, fin;
	double		lolen, hilen, len, lospac, hispac;
	unsigned	hicnt, locnt, cnt;
	unsigned*	locnts;
	unsigned*	hicnts;
	double*		lolens;
	double*		hilens;
	dPOINT*		lodifs;
	dPOINT*		hidifs;
	dPOINT*		lostps;
	dPOINT*		histps;
	fPOINT*		loflts;
	dPOINT*		hiflts;
	dPOINT		lostp, lopnt;
	dPOINT		histp, hipnt;
	dPOINT		dif;
	fPOINT*		sels;
	fPOINT*		his;
	PVEC*		pols;
	PVEC		polref, polin, poldif;

	sequenceIndex = 0;
	strt = SelectedForm->angleOrClipData.sat.start;
	fin = SelectedForm->angleOrClipData.sat.finish;
	hilins = sides - strt - 1;
	lolins = strt;
	selins = fin - strt;
	sels = &currentFormVertices[strt];
	his = &currentFormVertices[fin];
	lolen = hilen = 0;

	locnts = (unsigned*)bseq;
	hicnts = &locnts[lolins];

	lolens = (double*)&hicnts[hilins];
	hilens = &lolens[lolins];

	lodifs = (dPOINT*)&hilens[hilins];
	hidifs = &lodifs[lolins];
	lostps = &hidifs[hilins];
	histps = &lostps[lolins];
	hiflts = &histps[hilins];

	pols = (PVEC*)&hiflts[hilins];
	loflts = (fPOINT*)&pols[selins];

	lolen = loind = 0;
	lostp.x = lostp.y = 0;
	lopnt.x = lopnt.y = 0;
	histp.x = histp.y = 0;
	hipnt.x = hipnt.y = 0;

	for (ind = lolins; ind; ind--) {
		loflts[loind].x = currentFormVertices[ind].x;
		loflts[loind].y = currentFormVertices[ind].y;
		lodifs[loind].x = currentFormVertices[ind - 1].x - currentFormVertices[ind].x;
		lodifs[loind].y = currentFormVertices[ind - 1].y - currentFormVertices[ind].y;
		lolens[loind] = hypot(lodifs[loind].x, lodifs[loind].y);
		lolen += lolens[loind];
		loind++;
	}
	selind = 0;
	for (ind = strt + 1; ind <= fin; ind++) {
		dif.x = currentFormVertices[ind].x - sels[0].x;
		dif.y = currentFormVertices[ind].y - sels[0].y;
		pols[selind].length = hypot(dif.x, dif.y);
		pols[selind].angle = atan2(dif.y, dif.x);
		selind++;
	}
	hind = 0; hilen = 0;
	for (ind = fin; ind < (unsigned)sides - 1; ind++) {
		hiflts[hind].x = currentFormVertices[ind].x;
		hiflts[hind].y = currentFormVertices[ind].y;
		hidifs[hind].x = currentFormVertices[ind + 1].x - currentFormVertices[ind].x;
		hidifs[hind].y = currentFormVertices[ind + 1].y - currentFormVertices[ind].y;
		hilens[hind] = hypot(hidifs[hind].x, hidifs[hind].y);
		hilen += hilens[hind];
		hind++;
	}
	if (hilen > lolen)
		len = lolen;
	else
		len = hilen;
	cnt = len / SelectedForm->fillSpacing;
	if (hilen < lolen) {
		lospac = SelectedForm->fillSpacing;
		hispac = SelectedForm->fillSpacing*hilen / lolen;
	}
	else {
		hispac = SelectedForm->fillSpacing;
		lospac = SelectedForm->fillSpacing*lolen / hilen;
	}
	for (ind = 0; ind < lolins; ind++) {
		locnts[ind] = lolens[ind] / lospac;
		lostps[ind].x = lodifs[ind].x / locnts[ind];
		lostps[ind].y = lodifs[ind].y / locnts[ind];
	}
	for (ind = 0; ind < hilins; ind++) {
		hicnts[ind] = hilens[ind] / hispac;
		histps[ind].x = hidifs[ind].x / hicnts[ind];
		histps[ind].y = hidifs[ind].y / hicnts[ind];
	}
	loind = hind = 0;
	rstMap(FILDIR);
	locnt = hicnt = 0;
	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	polref.length = hypot(dif.x, dif.y);
	polref.angle = atan2(dif.y, dif.x);
	while (locnt || (loind < lolins&&hind < hilins)) {
		if (locnt)
			locnt--;
		else {
			if (loind < lolins) {
				locnt = locnts[loind];
				lostp.x = lostps[loind].x;
				lostp.y = lostps[loind].y;
				lopnt.x = loflts[loind].x;
				lopnt.y = loflts[loind].y;
				loind++;
			}
		}
		if (hicnt)
			hicnt--;
		else {
			if (hind < hilins) {
				hicnt = hicnts[hind];
				histp.x = histps[hind].x;
				histp.y = histps[hind].y;
				hipnt.x = hiflts[hind].x;
				hipnt.y = hiflts[hind].y;
				hind++;
			}
		}
		dif.x = hipnt.x - lopnt.x;
		dif.y = hipnt.y - lopnt.y;
		polin.angle = atan2(dif.y, dif.x);
		polin.length = hypot(dif.x, dif.y);
		poldif.angle = polin.angle - polref.angle;
		if (polref.length > 0.9*stitchSpace) {
			poldif.length = polin.length / polref.length;
			if (toglMap(FILDIR)) {
				oseq[sequenceIndex].x = lopnt.x;
				oseq[sequenceIndex].y = lopnt.y;
				sequenceIndex++;
				for (ind = 0; ind < (selins - 1); ind++) {
					RotationAngle = pols[ind].angle + poldif.angle;
					len = pols[ind].length*poldif.length;
					oseq[sequenceIndex].x = lopnt.x + cos(RotationAngle)*len;
					oseq[sequenceIndex].y = lopnt.y + sin(RotationAngle)*len;
					sequenceIndex++;
				}
			}
			else {
				oseq[sequenceIndex].x = hipnt.x;
				oseq[sequenceIndex].y = hipnt.y;
				sequenceIndex++;
				for (ind = selins - 1; ind != 0; ind--) {
					RotationAngle = pols[ind - 1].angle + poldif.angle;
					len = pols[ind - 1].length*poldif.length;
					oseq[sequenceIndex].x = lopnt.x + cos(RotationAngle)*len;
					oseq[sequenceIndex].y = lopnt.y + sin(RotationAngle)*len;
					sequenceIndex++;
				}
			}
		}
		lopnt.x += lostp.x;
		lopnt.y += lostp.y;
		hipnt.x += histp.x;
		hipnt.y += histp.y;
	}
	if (chkMap(FILDIR)) {
		oseq[sequenceIndex].x = currentFormVertices[0].x;
		oseq[sequenceIndex++].y = currentFormVertices[0].y;
	}
	else {
		oseq[sequenceIndex].x = currentFormVertices[sides - 1].x;
		oseq[sequenceIndex++].y = currentFormVertices[sides - 1].y;
	}
	if (SelectedForm->lengthOrCount.stitchLength < MinStitchLength)
		SelectedForm->lengthOrCount.stitchLength = MinStitchLength;
}

BOOL contsf(unsigned find)
{
	closestFormToCursor = find;
	fvars(find);
	if (SelectedForm->sides > 4)
	{
		delclps(closestFormToCursor);
		deltx();
		chkcont();
		SelectedForm->fillSpacing = stitchSpace;
		SelectedForm->fillColor = ActiveColor;
		fsizpar();
		SelectedForm->attribute |= (ActiveLayer << 1);
		refilfn();
		return 1;
	}
	return 0;
}

void contfil() {
	unsigned ind;

	if (filmsgs(FML_CONT))
		return;
	if (selectedFormCount) {
		savdo();
		for (ind = 0; ind < selectedFormCount; ind++)
			contsf(selectedFormList[ind]);
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		fvars(closestFormToCursor);
		savdo();
		if (contsf(closestFormToCursor))
		{
			setMap(INIT);
			coltab();
			setMap(RESTCH);
		}
		else
			tabmsg(IDS_CONT);
	}
}

BOOL cmpflt(fPOINT* flt0, fPOINT* flt1) {
	if (flt0->x != flt1->x)
		return 0;
	if (flt0->y == flt1->y)
		return 1;
	else
		return 0;
}

void ribon() {
	FRMHED*		tfrm;
	unsigned	ind, fpnt, clobak;

	frm1pnt();
	if (chkMap(FORMSEL)) {
		fvars(closestFormToCursor);
		if (sides > 2) {
			savdo();
			clobak = closestFormToCursor;
			satout(borderWidth);

			plen = borderWidth / 2;
			tfrm = &formList[formIndex];
			frmclr(tfrm);
			fpnt = 0;
			tfrm->maxFillStitchLen = 9 * PFGRAN;
			tfrm->minFillStitchLen = MinStitchLength;
			maxStitchLen = 9 * PFGRAN;
			if (SelectedForm->type == FRMLINE) {
				if (chku(BLUNT))
					ind = 0xffffffff;
				else
					ind = 0;
				satends(ind);
				tfrm->vertices = adflt(sides << 1);
				tfrm->vertices[0].x = outsidePoints[0].x;
				tfrm->vertices[fpnt++].y = outsidePoints[0].y;
				for (ind = 0; ind < sides; ind++) {
					tfrm->vertices[fpnt].x = insidePoints[ind].x;
					tfrm->vertices[fpnt++].y = insidePoints[ind].y;
				}
				for (ind = sides - 1; ind; ind--) {
					tfrm->vertices[fpnt].x = outsidePoints[ind].x;
					tfrm->vertices[fpnt++].y = outsidePoints[ind].y;
				}
			}
			else {
				tfrm->vertices = adflt((sides << 1) + 2);
				tfrm->vertices[0].x = outsidePoints[0].x;
				tfrm->vertices[fpnt++].y = outsidePoints[0].y;
				tfrm->underlayIndent = IniFile.underlayIndent;
				for (ind = 0; ind < sides; ind++) {
					tfrm->vertices[fpnt].x = insidePoints[ind].x;
					tfrm->vertices[fpnt++].y = insidePoints[ind].y;
				}
				tfrm->vertices[fpnt].x = insidePoints[0].x;
				tfrm->vertices[fpnt++].y = insidePoints[0].y;
				tfrm->vertices[fpnt].x = outsidePoints[0].x;
				tfrm->vertices[fpnt++].y = outsidePoints[0].y;
				for (ind = sides - 1; ind; ind--) {
					tfrm->vertices[fpnt].x = outsidePoints[ind].x;
					tfrm->vertices[fpnt++].y = outsidePoints[ind].y;
				}
			}
			tfrm->type = SAT;
			tfrm->fillColor = ActiveColor;
			tfrm->fillSpacing = stitchSpace;
			tfrm->lengthOrCount.stitchLength = IniFile.maxStitchLength;
			tfrm->sides = fpnt;
			tfrm->attribute = 1;
			tfrm->wordParam = fpnt >> 1;
			tfrm->satinGuideCount = tfrm->wordParam - 2;
			tfrm->satinOrAngle.sac = adsatk(tfrm->satinGuideCount);
			if (chkMap(CNV2FTH)) {
				tfrm->fillType = FTHF;
				tfrm->fillInfo.feather.ratio = IniFile.featherRatio;
				tfrm->fillInfo.feather.upCount = IniFile.featherUpCount;
				tfrm->fillInfo.feather.downCount = IniFile.featherDownCount;
				tfrm->fillInfo.feather.fillType = IniFile.featherFillType;
				tfrm->fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
				tfrm->extendedAttribute = IniFile.featherType;
				tfrm->fillInfo.feather.count = IniFile.featherCount;
				tfrm->fillInfo.feather.color = (ActiveColor + 1)&COLMSK;
			}
			else
				tfrm->fillType = SATF;
			for (ind = 0; ind < tfrm->satinGuideCount; ind++) {
				tfrm->satinOrAngle.sac[ind].start = ind + 2;
				tfrm->satinOrAngle.sac[ind].finish = tfrm->sides - ind - 1;
			}
			formIndex++;
			frmout(formIndex - 1);
			closestFormToCursor = formIndex - 1;
			refilfn();
			closestFormToCursor = clobak;
			setMap(DELTO);
			frmdel();
			closestFormToCursor = formIndex - 1;
			setMap(FORMSEL);
			setMap(INIT);
			setMap(RESTCH);
		}
		else
			tabmsg(IDS_FRM2);
	}
	else
		shoseln(IDS_FRM1MSG, IDS_CONVRIB);
}

void dupfn() {
	savdo();
	rotpar();
	if (IniFile.rotationAngle) {
		if (chkMap(FORMSEL))
			duprot();
		else {
			if (chkMap(GRPSEL))
				duprots();
			else {
				if (selectedFormCount)
					duprotfs();
				else
					shord();
			}
		}
	}
	else {
		rotentr();
		setMap(ENTRDUP);
	}
}

void redup() {
	RotationAngle = IniFile.rotationAngle;
	dupfn();
}

void bakdup() {
	RotationAngle = 2 * PI - IniFile.rotationAngle;
	dupfn();
}

void shrnks() {
	unsigned	ind, ine, cnt;
	dPOINT		dif;
	double		len, rat, dlen, adif;

	oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
	for (ind = 0; ind < (unsigned)sides - 1; ind++) {
		dif.x = currentFormVertices[ind + 1].x - currentFormVertices[ind].x;
		dif.y = currentFormVertices[ind + 1].y - currentFormVertices[ind].y;
		len = hypot(dif.x, dif.y);
		cnt = len / ClipRectSize.cx + 0.5;
		rat = (ClipRectSize.cx*cnt + 0.004) / len;
		currentFormVertices[ind + 1].x = currentFormVertices[ind].x + dif.x*rat;
		currentFormVertices[ind + 1].y = currentFormVertices[ind].y + dif.y*rat;
	}
	dif.x = currentFormVertices[0].x - currentFormVertices[1].x;
	dif.y = currentFormVertices[0].y - currentFormVertices[1].y;
	len = hypot(dif.x, dif.y);
	RotationAngle = atan2(dif.y, dif.x);
	for (ine = 0; ine < 5; ine++) {
		dif.x = currentFormVertices[0].x - currentFormVertices[ind].x;
		dif.y = currentFormVertices[0].y - currentFormVertices[ind].y;
		dlen = hypot(dif.x, dif.y);
		cnt = dlen / ClipRectSize.cx;
		adif = dlen - cnt*ClipRectSize.cx;
		RotationAngle -= adif / len;
		dif.x = cos(RotationAngle)*len;
		dif.y = sin(RotationAngle)*len;
		currentFormVertices[0].x = currentFormVertices[1].x + dif.x;
		currentFormVertices[0].y = currentFormVertices[1].y + dif.y;
	}
	refil();
}

void shrnk() {
	fvars(closestFormToCursor);
	if (chkMap(FORMSEL) && SelectedForm->edgeType == EDGECLIP) {
		shrnks();
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_FRMCLP, IDS_SHRNK);
}

void mvfrms(FRMHED* dst, FRMHED* src, unsigned cnt) {
#if	 __UseASM__
	_asm {
		mov		esi, src
		mov		edi, dst
		mov		eax, cnt
		mov		ecx, fsizeof
		mul		ecx
		mov		ecx, eax
		rep		movsd
	}
#else
	memcpy(dst, src, cnt * sizeof (FRMHED));
#endif
}

void dufdat(unsigned find) {
	FRMHED* dst;
	FRMHED* src;

	dst = &tmpFormList[formRelocationIndex++];
	src = &formList[find];
	mvfrms(dst, src, 1);
	mvflpnt(&tmpFormPoints[FormVertexIndex], dst->vertices, dst->sides);
	dst->vertices = &FormVertices[FormVertexIndex];
	FormVertexIndex += dst->sides;
	if (dst->satinGuideCount) {
		mvsatk(&tmpSatinConns[satinConnectIndex], dst->satinOrAngle.sac, dst->satinGuideCount);
		dst->satinOrAngle.sac = &satinConns[satinConnectIndex];
		satinConnectIndex += dst->satinGuideCount;
	}
	if (iseclpx(find))
	{
		mvflpnt(&tmpClipPoints[clipPointIndex], dst->borderClipData, dst->clipEntries);
		dst->borderClipData = &clipPoints[clipPointIndex];
		clipPointIndex += dst->clipEntries;
	}
	if (isclpx(find))
	{
		mvflpnt(&tmpClipPoints[clipPointIndex], dst->angleOrClipData.clip, dst->lengthOrCount.clipCount);
		dst->angleOrClipData.clip = &clipPoints[clipPointIndex];
		clipPointIndex += dst->lengthOrCount.clipCount;
	}
}

void stchfrm(unsigned fnum, unsigned* at) {
#if	 __UseASM__
	_asm {
		mov		eax, fnum
		shl		eax, FRMSHFT
		mov		ebx, at
		mov		ecx, [ebx]
		and ecx, NFRMSK
		or ecx, eax
		mov[ebx], ecx
	}
#else
	*at &= NFRMSK;
	*at |= fnum << FRMSHFT;
#endif
}

void frmnumfn(unsigned nunum) {
	unsigned	ind, srcpnt, strt, fin, cod;

	if (nunum != closestFormToCursor) {
		if (closestFormToCursor > nunum) {
			strt = nunum;
			fin = closestFormToCursor;
		}
		else {
			strt = closestFormToCursor;
			fin = nunum;
		}
		srcpnt = formRelocationIndex = 0;
		tmpFormList = (FRMHED*)&bseq;
		tmpFormPoints = (fPOINT*)&tmpFormList[formIndex];
		tmpSatinConns = (SATCON*)&oseq;
		tmpClipPoints = (fPOINT*)&tmpSatinConns[satinConnectIndex];
		FormVertexIndex = satinConnectIndex = clipPointIndex = 0;
		for (ind = 0; ind < formIndex; ind++) {
			if (ind == nunum)
				dufdat(closestFormToCursor);
			else {
				if (srcpnt == closestFormToCursor)
					srcpnt++;
				dufdat(srcpnt++);
			}
		}
		mvfrms(formList, tmpFormList, formIndex);
		mvflpnt(FormVertices, tmpFormPoints, FormVertexIndex);
		mvsatk(satinConns, tmpSatinConns, satinConnectIndex);
		mvflpnt(clipPoints, tmpClipPoints, clipPointIndex);
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if (StitchBuffer[ind].attribute&TYPMSK) {
				cod = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
				if (cod == closestFormToCursor)
					stchfrm(nunum, &StitchBuffer[ind].attribute);
				else {
					if (cod >= strt&&cod <= fin) {
						if (nunum < closestFormToCursor)
							stchfrm(cod + 1, &StitchBuffer[ind].attribute);
						else
							stchfrm(cod - 1, &StitchBuffer[ind].attribute);
					}
				}
			}
		}
		closestFormToCursor = nunum;
		ritnum(STR_NUMFRM, closestFormToCursor);
	}
}

void frmnum() {
	TCHAR	buf[HBUFSIZ];

	if (formIndex&&chkMap(FORMSEL)) {
		LoadString(ThredInstance, IDS_FRML, buf, HBUFSIZ);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), buf, formIndex);
		shoMsg(MsgBuffer);
		setMap(NUMIN);
		setMap(ENTRFNUM);
		numWnd();
	}
	else
		shoseln(IDS_FRM1MSG, IDS_SETFRM);
}

unsigned duat(unsigned at) {
#if	 __UseASM__
	_asm {
		mov		eax, at
		mov		ebx, eax
		shr		eax, TYPSHFT
		inc		eax
		and		al, 3
		and ebx, FRMSK
		shr		ebx, 2
		or eax, ebx
	}
#else
	unsigned typ = ((at >> TYPSHFT) + 1) & 3;
	unsigned frm = (at & FRMSK) >> 2;

	return typ | frm;
#endif
}

void srtf(unsigned strt, unsigned fin) {
	unsigned*	frmhst;
	unsigned	ind, ine, tmp;

	if (strt != fin) {
		frmhst = (unsigned*)&bseq;
		for (ind = 0; ind < formIndex << 2; ind++)
			frmhst[ind] = 0;
		for (ind = strt; ind < fin; ind++)
			frmhst[duat(tmpStitchBuffer[ind].attribute)]++;
		ine = strt;
		for (ind = 0; ind < formIndex << 2; ind++) {
			tmp = frmhst[ind];
			frmhst[ind] = ine;
			ine += tmp;
		}
		for (ind = strt; ind < fin; ind++)
			moveStitch(&StitchBuffer[frmhst[duat(tmpStitchBuffer[ind].attribute)]++], &tmpStitchBuffer[ind]);
	}
}

void srtbyfrm() {
	unsigned	ind, ine, tmp;
	unsigned	colhst[16];
	unsigned	colr[16];

	if (formIndex) {
		savdo();
		colr[UnderlayColor] = 0;
		for (ind = 0; ind < 16; ind++) {
			if (ind != UnderlayColor)
				colr[ind] = ind + 1;
		}
		tmpStitchBuffer = &StitchBuffer[MAXSEQ];
		for (ind = 0; ind < 16; ind++)
			colhst[ind] = 0;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			colhst[colr[StitchBuffer[ind].attribute & 0xf]]++;
		ine = 0;
		for (ind = 0; ind < 16; ind++) {
			tmp = colhst[ind];
			colhst[ind] = ine;
			ine += tmp;
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			moveStitch(&tmpStitchBuffer[colhst[colr[StitchBuffer[ind].attribute & 0xf]]++], &StitchBuffer[ind]);
		srtf(0, colhst[0]);
		for (ind = 0; ind < 15; ind++)
			srtf(colhst[ind], colhst[ind + 1]);
	}
	else
		srtcol();
	coltab();
	setMap(RESTCH);
}

void dufcntr(dPOINT* cntr) {
	unsigned	ind;
	fRECTANGLE*		trct;
	fRECTANGLE		brct;

	trct = &formList[selectedFormList[0]].rectangle;
	brct.left = trct->left;
	brct.right = trct->right;
	brct.top = trct->top;
	brct.bottom = trct->bottom;
	for (ind = 1; ind < selectedFormCount; ind++) {
		trct = &formList[selectedFormList[ind]].rectangle;
		if (trct->left < brct.left)
			brct.left = trct->left;
		if (trct->right > brct.right)
			brct.right = trct->right;
		if (trct->bottom < brct.bottom)
			brct.bottom = trct->bottom;
		if (trct->top > brct.top)
			brct.top = trct->top;
	}
	cntr->x = (brct.right - brct.left) / 2 + brct.left;
	cntr->y = (brct.top - brct.bottom) / 2 + brct.bottom;
}

void cntrx() {
	dPOINT		mrkcntr;
	dPOINT		selcntr;
	unsigned	ind, flg;
	fRECTANGLE*		trct;
	fRECTANGLE		grct;

	if (chkMap(GMRK)) {
		mrkcntr.x = ZoomMarkPoint.x;
		mrkcntr.y = ZoomMarkPoint.y;
	}
	else {
		mrkcntr.x = UnzoomedRect.x / 2;
		mrkcntr.y = UnzoomedRect.y / 2;
	}
	flg = 0;
	if (selectedFormCount) {
		flg = 1;
		savdo();
		dufcntr(&selcntr);
		formMoveDelta.x = mrkcntr.x - selcntr.x;
		formMoveDelta.y = -mrkcntr.y + selcntr.y;
		if (chkMap(CNTRV))
			formMoveDelta.y = 0;
		if (chkMap(CNTRH))
			formMoveDelta.x = 0;
		for (ind = 0; ind < selectedFormCount; ind++)
			frmadj(selectedFormList[ind]);
		frmsadj();
	}
	else {
		if (chkMap(FORMSEL)) {
			flg = 1;
			savdo();
			trct = &formList[closestFormToCursor].rectangle;
			selcntr.x = (trct->right - trct->left) / 2 + trct->left;
			selcntr.y = (trct->top - trct->bottom) / 2 + trct->bottom;
			formMoveDelta.x = mrkcntr.x - selcntr.x;
			formMoveDelta.y = -mrkcntr.y + selcntr.y;
			if (chkMap(CNTRV))
				formMoveDelta.y = 0;
			if (chkMap(CNTRH))
				formMoveDelta.x = 0;
			frmadj(closestFormToCursor);
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if (StitchBuffer[ind].attribute&ALTYPMSK && (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor) {
					StitchBuffer[ind].x += formMoveDelta.x;
					StitchBuffer[ind].y -= formMoveDelta.y;
				}
			}
		}
		else {
			if (chkMap(GRPSEL)) {
				flg = 1;
				savdo();
				rngadj();
				grct.right = grct.left = StitchBuffer[GroupStartStitch].x;
				grct.top = grct.bottom = StitchBuffer[GroupStartStitch].y;
				for (ind = GroupStartStitch + 1; ind <= GroupEndStitch; ind++) {
					if (StitchBuffer[ind].x < grct.left)
						grct.left = StitchBuffer[ind].x;
					if (StitchBuffer[ind].x > grct.right)
						grct.right = StitchBuffer[ind].x;
					if (StitchBuffer[ind].y < grct.bottom)
						grct.bottom = StitchBuffer[ind].y;
					if (StitchBuffer[ind].y > grct.top)
						grct.top = StitchBuffer[ind].y;
				}
				selcntr.x = (grct.right - grct.left) / 2 + grct.left;
				selcntr.y = (grct.top - grct.bottom) / 2 + grct.bottom;
				formMoveDelta.x = mrkcntr.x - selcntr.x;
				formMoveDelta.y = -mrkcntr.y + selcntr.y;
				if (chkMap(CNTRV))
					formMoveDelta.y = 0;
				if (chkMap(CNTRH))
					formMoveDelta.x = 0;
				for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++) {
					StitchBuffer[ind].x += formMoveDelta.x;
					StitchBuffer[ind].y -= formMoveDelta.y;
				}
			}
			else
				shoseln(IDS_FGRPF, IDS_CENT);
		}
	}
	if (flg)
		setMap(RESTCH);
}

void centir() {
	dPOINT		scntr;
	dPOINT		dcntr;
	dPOINT		dif;
	unsigned	ind;

	rstMap(BIGBOX);
	getbig();
	dcntr.x = (allItemsRectangle.right - allItemsRectangle.left) / 2 + allItemsRectangle.left;
	dcntr.y = (allItemsRectangle.top - allItemsRectangle.bottom) / 2 + allItemsRectangle.bottom;
	scntr.x = UnzoomedRect.x / 2;
	scntr.y = UnzoomedRect.y / 2;
	dif.x = scntr.x - dcntr.x;
	dif.y = scntr.y - dcntr.y;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		StitchBuffer[ind].x += dif.x;
		StitchBuffer[ind].y += dif.y;
	}
	for (ind = 0; ind < FormVertexIndex; ind++) {
		FormVertices[ind].x += dif.x;
		FormVertices[ind].y += dif.y;
	}
	for (ind = 0; ind < formIndex; ind++)
		frmout(ind);
	setfchk();
	setMap(RESTCH);
}

void bean(unsigned strt, unsigned fin) {
	unsigned ind, ine, inf;

	ine = MAXSEQ;
	ind = strt;
	beanCount = 0;
	mvstch(ine++, ind);
	if (StitchBuffer[ind + 2].x != StitchBuffer[ind].x || StitchBuffer[ind + 2].y != StitchBuffer[ind].y) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		beanCount += 2;
	}
	ind++;
	mvstch(ine++, ind);
	if (StitchBuffer[ind + 2].x != StitchBuffer[ind].x || StitchBuffer[ind + 2].y != StitchBuffer[ind].y) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		beanCount += 2;
	}
	ind++;
	while (ind < (unsigned)fin - 2) {
		mvstch(ine++, ind);
		if ((StitchBuffer[ind + 2].x != StitchBuffer[ind].x || StitchBuffer[ind + 2].y != StitchBuffer[ind].y) &&
			(StitchBuffer[ind - 2].x != StitchBuffer[ind].x || StitchBuffer[ind - 2].y != StitchBuffer[ind].y)) {
			mvstch(ine++, ind + 1);
			mvstch(ine++, ind);
			beanCount += 2;
		}
		ind++;
	}
	mvstch(ine++, ind);
	if ((StitchBuffer[ind - 2].x != StitchBuffer[ind].x || StitchBuffer[ind - 2].y != StitchBuffer[ind].y)) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		beanCount += 2;
	}
	ind++;
	while (ind < PCSHeader.stitchCount)
		mvstch(ine++, ind++);
	inf = strt;
	for (ind = MAXSEQ; ind < ine; ind++)
		mvstch(inf++, ind);
	PCSHeader.stitchCount = inf;
}

void dubean() {
	if (PCSHeader.stitchCount) {
		savdo();
		if (chkMap(GRPSEL)) {
			rngadj();
			bean(GroupStartStitch, GroupEndStitch);
			if (ClosestPointIndex > GroupStitchIndex)
				ClosestPointIndex += beanCount;
			else
				GroupStitchIndex += beanCount;
			grpAdj();
		}
		else
			bean(0, PCSHeader.stitchCount - 1);
		coltab();
		setMap(RESTCH);
	}
}

void unbean(unsigned strt, unsigned fin) {
	unsigned ind, ine;

	ine = MAXSEQ;
	beanCount = 0;
	for (ind = strt; ind <= fin; ind++) {
		mvstch(ine++, ind);
		if (StitchBuffer[ind].x == StitchBuffer[ind + 2].x&&StitchBuffer[ind].y == StitchBuffer[ind + 2].y) {
			ind += 2;
			beanCount += 2;
		}
	}
	if (beanCount)
		beanCount -= 2;;
	if (ind > (unsigned)PCSHeader.stitchCount - 1)
		ind = PCSHeader.stitchCount - 1;
	while (ind < PCSHeader.stitchCount)
		mvstch(ine++, ind++);
	mvstchs(strt, MAXSEQ, ine);
	PCSHeader.stitchCount = strt + ine;
}

void debean() {
	savdo();
	if (chkMap(GRPSEL)) {
		rngadj();
		unbean(GroupStartStitch, GroupEndStitch);
		if (ClosestPointIndex > GroupStitchIndex)
			ClosestPointIndex -= beanCount;
		else
			GroupStitchIndex -= beanCount;
		if (ClosestPointIndex > (unsigned)PCSHeader.stitchCount - 1)
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		if (GroupStitchIndex > (unsigned)PCSHeader.stitchCount - 1)
			GroupStitchIndex = PCSHeader.stitchCount - 1;
		grpAdj();
	}
	else
		unbean(0, PCSHeader.stitchCount - 1);
	coltab();
	setMap(RESTCH);
}

void mvfrmsb(FRMHED* dst, FRMHED* src, unsigned cnt) {
#if	 __UseASM__
	_asm {
		std
		mov		eax, fsizeof
		mul		cnt
		mov		ecx, eax
		mov		edi, dst
		add		edi, 64
		mov		esi, src
		add		esi, 64
		rep		movsd
		cld
	}
#else
	memmove(dst, src, cnt * sizeof(FRMHED));
#endif
}

void mvfltsb(fPOINT* dst, fPOINT* src, unsigned cnt) {
#if	 __UseASM__
	_asm {
		std
		mov		ecx, cnt
		shl		ecx, 1
		mov		esi, src
		add		esi, 4
		mov		edi, dst
		add		edi, 4
		rep		movsd
		cld
	}
#else
	memmove(dst, src, cnt * sizeof(fPOINT));
#endif
}

void clpspac(fPOINT* pins, unsigned cnt) {
	mvfltsb(&clipPoints[clipPointIndex + cnt - 1], &clipPoints[clipPointIndex - 1], clipPointIndex - clpind(pins));
}

void stchadj() {
	unsigned ind, lo, hi;

	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		hi = StitchBuffer[ind].attribute&FRMSK;
		lo = hi >> FRMSHFT;
		if (lo > closestFormToCursor) {
			StitchBuffer[ind].attribute &= NFRMSK;
			hi += 1 << FRMSHFT;
			StitchBuffer[ind].attribute |= hi;
		}
	}
	refilfn();
	lo = closestFormToCursor << FRMSHFT;
	for (ind = PCSHeader.stitchCount; ind != 0; ind--) {
		hi = StitchBuffer[ind - 1].attribute&FRMSK;
		if ((StitchBuffer[ind - 1].attribute&FRMSK) == lo) {
			break;
		}
	}
	closestFormToCursor++;
	refilfn();
	rstMap(FRMPSEL);
}

void spltsat(SATCON tsac) {
	fPOINT*		hiflt;
	unsigned	ind, lo, hi, hibak;

	hiflt = (fPOINT*)&oseq;
	mvfrmsb(&formList[formIndex], &formList[formIndex - 1], formIndex - closestFormToCursor);
	formIndex++;
	if (closestFormToCursor < (unsigned)formIndex - 2)
		mvfltsb(&FormVertices[FormVertexIndex + 1], &FormVertices[FormVertexIndex - 1], FormVertexIndex - fltind(formList[closestFormToCursor + 2].vertices));
	FormVertexIndex += 2;
	for (ind = closestFormToCursor + 2; ind < formIndex; ind++)
		formList[ind].vertices += 2;
	lo = 0;
	hibak = tsac.start + (sides - tsac.finish) + 1;
	hi = hibak + 1;
	for (ind = 0; ind < sides; ind++) {
		if (ind == tsac.start || ind == tsac.finish) {
			hiflt[lo].x = currentFormVertices[ind].x;
			hiflt[lo++].y = currentFormVertices[ind].y;
			if (ind == tsac.start) {
				hiflt[hi].x = currentFormVertices[ind].x;
				hiflt[hi++].y = currentFormVertices[ind].y;
			}
			else {
				hiflt[hibak].x = currentFormVertices[ind].x;
				hiflt[hibak].y = currentFormVertices[ind].y;
			}
		}
		else {
			if (ind < tsac.start) {
				hiflt[lo].x = currentFormVertices[ind].x;
				hiflt[lo++].y = currentFormVertices[ind].y;
			}
			else {
				if (ind < tsac.finish) {
					hiflt[hi].x = currentFormVertices[ind].x;
					hiflt[hi++].y = currentFormVertices[ind].y;
				}
				else {
					hiflt[lo].x = currentFormVertices[ind].x;
					hiflt[lo++].y = currentFormVertices[ind].y;
				}
			}
		}
	}
	for (ind = 0; ind < hi; ind++) {
		currentFormVertices[ind].x = hiflt[ind].x;
		currentFormVertices[ind].y = hiflt[ind].y;
	}
	SelectedForm->sides = lo;
	formList[closestFormToCursor + 1].sides = hi - lo;
	formList[closestFormToCursor + 1].vertices = &currentFormVertices[lo];
	frmout(closestFormToCursor);
	frmout(closestFormToCursor + 1);
	lo = tsac.start + 1 - tsac.finish;
	for (ind = 0; ind < activePointIndex; ind++)
		SelectedForm->satinOrAngle.sac[ind].finish += lo;
	if (SelectedForm->wordParam)
		SelectedForm->wordParam = tsac.start;
	lo = ind + 1;
	while (ind < (unsigned)SelectedForm->satinGuideCount) {
		SelectedForm->satinOrAngle.sac[ind].start -= (tsac.start - 1);
		SelectedForm->satinOrAngle.sac[ind].finish -= (tsac.start - 1);
		ind++;
	}
	if (formList[closestFormToCursor + 1].wordParam)
		formList[closestFormToCursor + 1].wordParam -= (tsac.start - 1);
	mvsatk(&SelectedForm->satinOrAngle.sac[lo - 1], &SelectedForm->satinOrAngle.sac[lo], satinConnectIndex - sacind(&SelectedForm->satinOrAngle.sac[lo]));
	satinConnectIndex--;
	formList[closestFormToCursor + 1].satinOrAngle.sac = &SelectedForm->satinOrAngle.sac[activePointIndex];
	formList[closestFormToCursor + 1].satinGuideCount = SelectedForm->satinGuideCount - activePointIndex - 1;
	SelectedForm->satinGuideCount = activePointIndex;
	for (ind = closestFormToCursor + 2; ind < formIndex; ind++)
		formList[ind].satinOrAngle.sac--;
	if (iseclp(closestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
			formList[ind].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
}

BOOL spltlin() {
	unsigned ind;

	if (closestVertexToCursor < 2 || SelectedForm->sides - closestVertexToCursor < 2)
		return 0;
	mvfrmsb(&formList[formIndex], &formList[formIndex - 1], formIndex - closestFormToCursor);
	formIndex++;
	SelectedForm->sides = closestVertexToCursor;
	formList[closestFormToCursor + 1].sides -= closestVertexToCursor;
	formList[closestFormToCursor + 1].vertices = &SelectedForm->vertices[closestVertexToCursor];
	frmout(closestFormToCursor);
	frmout(closestFormToCursor + 1);
	if (iseclp(closestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
			formList[ind].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
	return 1;
}

void spltfrm() {
	if (chkMap(FRMPSEL)) {
		savdo();
		fvars(closestFormToCursor);
		if (SelectedForm->type == SAT) {
			if (SelectedForm->satinGuideCount) {
				delfstchs();
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				for (activePointIndex = 0; activePointIndex < SelectedForm->satinGuideCount; activePointIndex++) {
					if (SelectedForm->satinOrAngle.sac[activePointIndex].start == closestVertexToCursor || SelectedForm->satinOrAngle.sac[activePointIndex].finish == closestVertexToCursor) {
						spltsat(SelectedForm->satinOrAngle.sac[activePointIndex]);
						return;
					}
				}
				spltmsg();
			}
			else
				spltmsg();
		}
		else {
			if (SelectedForm->type == FRMLINE) {
				if (spltlin()) {
					coltab();
					setMap(RESTCH);
				}
				else
					tabmsg(IDS_FRM3);
				return;
			}
			else
				spltmsg();
		}
	}
	spltmsg();
}

void stchs2frm() {
	unsigned ind, ine, len;

	if (chkMap(GRPSEL)) {
		rngadj();
		if ((GroupEndStitch - GroupStartStitch) > 12000) {
			tabmsg(IDS_STMAX);
			return;
		}
		len = GroupEndStitch - GroupStartStitch + 1;
		SelectedForm = &formList[formIndex];
		frmclr(SelectedForm);
		SelectedForm->type = FRMLINE;
		SelectedForm->sides = len;
		SelectedForm->vertices = adflt(len);
		ine = 0;
		for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++) {
			SelectedForm->vertices[ine].x = StitchBuffer[ind].x;
			SelectedForm->vertices[ine++].y = StitchBuffer[ind].y;
		}
		frmout(formIndex);
		formIndex++;
		if (ClosestPointIndex > GroupStitchIndex) {
			if (ClosestPointIndex < (unsigned)PCSHeader.stitchCount - 1)
				ClosestPointIndex++;
		}
		else {
			if (GroupStitchIndex < (unsigned)PCSHeader.stitchCount - 1)
				GroupStitchIndex++;
		}
		delstchm();
		rstMap(GRPSEL);
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_GRPMSG, IDS_STCH2FRM);
}

int lencmp(const void *arg1, const void *arg2) {
#if	 __UseASM__
	_asm {
		mov		ebx, arg1
		mov		ebx, [ebx]
		fld		dword ptr[ebx]
		mov		ebx, arg2
		mov		ebx, [ebx]
		fld		dword ptr[ebx]
		fcompp
		fstsw	ax
		mov		ebx, eax
		xor		eax, eax
		test	bh, 6
		jne		short lcmpx
		test	bh, 1
		je		short lcmp2
		inc		eax
		jmp		short lcmpx
		lcmp2 : dec		eax
				lcmpx :
	}
#else
	//Correct
	float local1 = **(float **)arg1, local2 = **(float **)arg2;

	if (local1 < local2)
		return -1;

	if (local1 > local2)
		return 1;

	return 0;
#endif
}

void chksid(unsigned find) {
	unsigned ind, lim;

	if (clipIntersectSide != find) {
		if ((find - clipIntersectSide + sides) % sides < (unsigned)(sides >> 1)) {
			ind = nxt(clipIntersectSide);
			lim = nxt(find);
			while (ind != lim) {
				oseq[sequenceIndex].x = currentFormVertices[ind].x;
				oseq[sequenceIndex++].y = currentFormVertices[ind].y;
				ind = nxt(ind);
			}
		}
		else {
			ind = clipIntersectSide;
			while (ind != find) {
				oseq[sequenceIndex].x = currentFormVertices[ind].x;
				oseq[sequenceIndex++].y = currentFormVertices[ind].y;
				ind = prv(ind);
			}
		}
	}
}

void ritseg() {
	unsigned	ind;
	BOOL		pntd;

	pntd = 1;
	if (SelectedForm->extendedAttribute&AT_SQR)
		pntd = 0;
	if (chkMap(FILDIR)) {
		ind = clipSegments[activePointIndex].start;
		if (chkMap(TXFIL) && pntd)
			ind++;
		chksid(clipSegments[activePointIndex].asid);
		while (ind <= clipSegments[activePointIndex].finish) {
			oseq[sequenceIndex].x = clipStitchPoints[ind].x;
			oseq[sequenceIndex++].y = clipStitchPoints[ind++].y;
		}
		clipIntersectSide = clipSegments[activePointIndex].zsid;
	}
	else {
		ind = clipSegments[activePointIndex].finish;
		if (chkMap(TXFIL) && pntd)
			ind--;
		chksid(clipSegments[activePointIndex].zsid);
		if (clipSegments[activePointIndex].start) {
			while (ind >= clipSegments[activePointIndex].start) {
				oseq[sequenceIndex].x = clipStitchPoints[ind].x;
				oseq[sequenceIndex++].y = clipStitchPoints[ind--].y;
			}
		}
		else {
			while (ind < clipSegments[activePointIndex].start) {
				oseq[sequenceIndex].x = clipStitchPoints[ind].x;
				oseq[sequenceIndex++].y = clipStitchPoints[ind--].y;
			}
		}
		clipIntersectSide = clipSegments[activePointIndex].asid;
	}
	clipSegments[activePointIndex].dun = 1;
}

unsigned lenref(float* pflt) {
#if	 __UseASM__
	_asm {
		mov		eax, pflt
		sub		eax, clipSegments
		xor		ecx, ecx
		mov		ecx, 29
		xor edx, edx
		div		ecx
		shl		eax, 1
		cmp		dl, 18
		jne		short lrefx
		inc		eax
		lrefx :
	}
#else
	//Correct
	unsigned eax = (unsigned)pflt - (unsigned)clipSegments;

	unsigned edx = eax % 29;
	eax /= 29;

	eax <<= 1;

	if ((edx & 0xFF) == 18)
		eax++;

	return eax;
#endif
}

BOOL clpnxt(unsigned sind) {
	unsigned	ind;
	unsigned	indexDoubled;	//clipboard segment index * 2

	ind = 1;
	indexDoubled = clipSegmentIndex << 1;
	rstMap(FILDIR);
	while (ind < clipSegmentIndex) {
		if (toglMap(FILDIR)) {
			outputIndex = (sind + ind) % indexDoubled;
			if (!clipSegments[lenref(sortedLengths[outputIndex]) >> 1].dun)
				return 0;
			ind++;
		}
		else {
			outputIndex = (sind + indexDoubled - ind) % indexDoubled;
			if (!clipSegments[lenref(sortedLengths[outputIndex]) >> 1].dun)
				return 0;
		}
	}
	return 1;
}

BOOL nucseg() {
	unsigned	ind;

	if (chkMap(FILDIR))
		ind = clipSegments[activePointIndex].edgeIndex;
	else
		ind = clipSegments[activePointIndex].bind;
	if (clpnxt(ind))
		return 0;
	ind = lenref(sortedLengths[outputIndex]);
	if (ind & 1)
		rstMap(FILDIR);
	else
		setMap(FILDIR);
	activePointIndex = ind >> 1;
	return 1;
}

void mvpclp(unsigned dst, unsigned src) {
#if	 __UseASM__
	_asm {
		mov		edi, dst
		shl		edi, 2
		add		edi, ptrClipIntersectData
		mov		edi, [edi]
		mov		esi, src
		shl		esi, 2
		add		esi, ptrClipIntersectData
		mov		esi, [esi]
		xor ecx, ecx
		mov		cl, 5
		rep		movsd
	}
#else
	//ToDo - Should '20' be sizeof(CLIPSORT)'?
	memcpy(ptrClipIntersectData[dst], ptrClipIntersectData[src], 20);
#endif
}

float getlen(unsigned ind) {
	clipStitchPoints[ind].side %= sides;
	return	lengths[clipStitchPoints[ind].side] +
		hypot(currentFormVertices[clipStitchPoints[ind].side].x - clipStitchPoints[ind].x,
			currentFormVertices[clipStitchPoints[ind].side].y - clipStitchPoints[ind].y);
}

unsigned leftsid() {
	unsigned	ind, sid;
	float		minx;

	minx = 1e9;
	sid = 0;
	for (ind = 0; ind < sides; ind++) {
		if (currentFormVertices[ind].x < minx) {
			minx = currentFormVertices[ind].x;
			sid = ind;
		}
	}
	return sid;
}

int clpcmp(const void* arg1, const void* arg2) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, arg1
		mov		esi, arg2
		mov		ecx, [ebx]
		mov		edx, [esi]
		cmp		ecx, edx
		jne		short clpcmp1
		add		ebx, 4
		add		esi, 4
		mov		ecx, [ebx]
		mov		edx, [esi]
		cmp		ecx, edx
		je		short clpcmpx
		clpcmp1 : jc		short clpcmp2
				  inc		eax
				  jmp		short clpcmpx
				  clpcmp2 : dec		eax
							clpcmpx :
	}
#else
	VCLPX *vclpx1 = (VCLPX *)arg1, *vclpx2 = (VCLPX *)arg2;
	if (vclpx1->seg < vclpx2->seg)
		return -1;

	if (vclpx1->seg > vclpx2->seg)
		return 1;

	if (vclpx1->sid == vclpx2->sid)
		return 0;

	if (vclpx1->sid < vclpx2->sid)
		return -1;

	return 1;
#endif
}

BOOL isect(unsigned find0, unsigned find1, fPOINT* ipnt, float* len) {
	dPOINT		dif;
	dPOINT		tpnt;
	dPOINT		tipnt;
	unsigned	flg;
	float		lft;
	float		rit;

	dif.x = vpnt1.x - vpnt0.x;
	dif.y = vpnt1.y - vpnt0.y;
	tpnt.x = vpnt0.x;
	tpnt.y = vpnt0.y;
	flg = 0;
	if (dif.x&&dif.y)
		flg = proj(tpnt, dif.y / dif.x, currentFormVertices[find0], currentFormVertices[find1], &tipnt);
	else {
		if (dif.y)
			flg = projv(tpnt.x, currentFormVertices[find0], currentFormVertices[find1], &tipnt);
		else {
			if (dif.x)
				flg = projh(tpnt.y, currentFormVertices[find0], currentFormVertices[find1], &tipnt);
			else
				if (currentFormVertices[find0].y == vpnt0.y&&currentFormVertices[find1].y == vpnt0.y)
				{
					if (currentFormVertices[find0].x < currentFormVertices[find1].x)
					{
						lft = currentFormVertices[find0].x;
						rit = currentFormVertices[find1].x;
					}
					else
					{
						lft = currentFormVertices[find1].x;
						rit = currentFormVertices[find0].x;
					}
					if (vpnt0.x > lft&&vpnt0.x < rit)
					{
						ipnt->x = vpnt0.x;
						ipnt->y = vpnt0.y;
						*len = 0;
						return 1;
					}
					return 0;
				}
				else
					return 0;
		}
	}
	if (tipnt.x < TINY)
		tipnt.x = 0;
	if (tipnt.y < TINY)
		tipnt.y = 0;
	ipnt->x = (float)tipnt.x;
	ipnt->y = (float)tipnt.y;
	*len = hypot(tipnt.x - vpnt0.x, tipnt.y - vpnt0.y);
	// ToDo - Why is this line here?
	//	 hypot(tipnt.x-vpnt1.x,tipnt.y-vpnt1.y);
	return flg;
}

unsigned insect() { 
	unsigned	ind, ine, cnt;
	unsigned	svrt, nvrt;
	fRECTANGLE		lrct;
	fPOINT*		ipnt;

	if (vpnt1.x > vpnt0.x) {
		lrct.left = vpnt0.x;
		lrct.right = vpnt1.x;
	}
	else {
		lrct.left = vpnt1.x;
		lrct.right = vpnt0.x;
	}
	if (vpnt1.y > vpnt0.y) {
		lrct.top = vpnt1.y;
		lrct.bottom = vpnt0.y;
	}
	else {
		lrct.top = vpnt0.y;
		lrct.bottom = vpnt1.y;
	}
	ine = cnt = 0;
	for (ind = regionCrossingStart; ind < regionCrossingEnd; ind++) {
		svrt = regionCrossingData[ind].sid;
		nvrt = nxt(svrt);
		if (isect(svrt, nvrt, &clipIntersectData[ine].point, &clipIntersectData[ine].sidlen)) {
			ipnt = &clipIntersectData[ine].point;
			if (ipnt->x >= lrct.left&&
				ipnt->x <= lrct.right&&
				ipnt->y >= lrct.bottom&&
				ipnt->y <= lrct.top) {
				clipIntersectData[ine].seglen = hypot(clipIntersectData[ine].point.x - vpnt0.x, clipIntersectData[ine].point.y - vpnt0.y);
				clipIntersectData[ine].lin = svrt;
				ptrClipIntersectData[ine] = &clipIntersectData[ine];
				ine++;
				cnt++;
			}
		}
	}
	if (cnt > 1) {
		qsort((void*)ptrClipIntersectData, cnt, 4, lencmp);
		ine = 1;
		for (ind = 0; ind < cnt - 1; ind++) {
			if (fabs(ptrClipIntersectData[ind]->seglen - ptrClipIntersectData[ind + 1]->seglen) > TINY)
				mvpclp(ine++, ind + 1);
		}
		cnt = ine;
	}
	return cnt;
}

BOOL isin(float pntx, float pnty) {
	unsigned	ind, acnt;
	unsigned	svrt, nvrt;
	dPOINT		ipnt;

	if (pntx < boundingRect.left)
		return 0;
	if (pntx > boundingRect.right)
		return 0;
	if (pnty < boundingRect.bottom)
		return 0;
	if (pnty > boundingRect.top)
		return 0;
	acnt = 0;
	for (ind = regionCrossingStart; ind < regionCrossingEnd; ind++)
	{
		svrt = regionCrossingData[ind].sid;
		nvrt = nxt(svrt);
		if (projv(pntx, currentFormVertices[svrt], currentFormVertices[nvrt], &ipnt))
		{
			if (ipnt.y > pnty)
			{
				if (currentFormVertices[svrt].x != pntx&&currentFormVertices[nvrt].x != pntx)
					acnt++;
				else
				{
					if (currentFormVertices[svrt].x < currentFormVertices[nvrt].x)
					{
						if (currentFormVertices[nvrt].x != pntx)
							acnt++;
					}
					else
					{
						if (currentFormVertices[svrt].x != pntx)
							acnt++;
					}
				}
			}
		}
	}
	return acnt & 1;
}

unsigned clpnseg(unsigned strt, unsigned fin) {
	clipSegments[clipSegmentIndex].start = strt;
	clipSegments[clipSegmentIndex].borderLength = getlen(strt);
	clipSegments[clipSegmentIndex].asid = clipStitchPoints[strt].side;
	clipSegments[clipSegmentIndex].edgeLength = getlen(fin);
	clipSegments[clipSegmentIndex].zsid = clipStitchPoints[fin].side;
	clipSegments[clipSegmentIndex].finish = fin;
	clipSegments[clipSegmentIndex++].dun = 0;
	return fin + 1;
}

unsigned vclpfor(unsigned ind) {
	while (!clipStitchPoints[ind].flag&&ind < activePointIndex)
		ind++;
	return ind;
}

unsigned vclpbak(unsigned ind) {
	while (!clipStitchPoints[ind].flag&&ind)
		ind--;
	return ind;
}

BOOL vscmp(unsigned ind, unsigned ine) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		esi, ind
		mov		edi, ine
		shl		esi, 3
		shl		edi, 3
		mov		ecx, offset oseq
		add		esi, ecx
		add		edi, ecx
		mov		ecx, [esi]
		cmp		ecx, [edi]
		je		short vscmp1
		inc		eax
		jmp		short vscmpx
		vscmp1 : add		esi, 4
				 add		edi, 4
				 mov		ecx, [esi]
				 cmp		ecx, [edi]
				 je		short vscmpx
				 inc		eax
				 vscmpx :
	}
#else
	if (oseq[ind].x != oseq[ine].x)
		return 1;

	return oseq[ind].y != oseq[ine].y ? 1 : 0;
#endif
}

void duflt() {
	unsigned	ind;
	float		leftEdge;

	leftEdge = 1e9;
	for (ind = 0; ind < sides; ind++) {
		if (currentFormVertices[ind].x < leftEdge)
			leftEdge = currentFormVertices[ind].x;
	}
	if (leftEdge < ClipRectSize.cx) {
		setMap(WASNEG);
		formOffset = ClipRectSize.cx + fabs(leftEdge) + .05;
		for (ind = 0; ind < sides; ind++)
			currentFormVertices[ind].x += formOffset;
		SelectedForm->rectangle.left += formOffset;
		SelectedForm->rectangle.right += formOffset;
	}
	else
		rstMap(WASNEG);
}

void inspnt()
{
	clipStitchPoints[activePointIndex + 1].x = clipStitchPoints[activePointIndex].x;
	clipStitchPoints[activePointIndex + 1].y = clipStitchPoints[activePointIndex].y;
	clipStitchPoints[activePointIndex].x = midl(clipStitchPoints[activePointIndex + 1].x, clipStitchPoints[activePointIndex - 1].x);
	clipStitchPoints[activePointIndex].y = midl(clipStitchPoints[activePointIndex + 1].y, clipStitchPoints[activePointIndex - 1].y);
	clipStitchPoints[activePointIndex].flag = 1;
	activePointIndex++;
}

void clpcon() {
	RECT		nrct;
	unsigned	ind, ine, inf, ing, nof, clpneg;
	unsigned	strt, fin, segxs, segps, seg, clrnum;
	unsigned	cnt;
	int			tine;
	fPOINT		ploc;
	double		tlen, minx;
	float		fnof;
	unsigned	clpnof;
	double		clpvof;
	TXPNT*		ptx = nullptr;
	unsigned*	iclpx;			//indices into region crossing data for vertical clipboard fills
	unsigned	clplim;			//vertical clipboard search limit

	duflt();
	clipWidth = ClipRectSize.cx + SelectedForm->fillSpacing;
	if (chkMap(ISUND))
		clipWidth = SelectedForm->underlaySpacing;
	if (SelectedForm->fillSpacing < 0)
		clpneg = 1;
	else
		clpneg = 0;
	if (clipWidth < CLPMINAUT)
		clipWidth = (float)CLPMINAUT;
	if (chkMap(TXFIL))
	{
		if (textureIndex&&SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count <= textureIndex)
			clipWidth = SelectedForm->fillSpacing;
		else
			return;
	}
	lengths = new double[sides + 1];
	clipSideLengths = new double[sides];
	clipIntersectData = new CLIPSORT[sides];
	ptrClipIntersectData = new CLIPSORT*[sides + 1]();
	ine = leftsid();
	tlen = 0;
	lengths[ine] = 0;
	ine = nxt(ine);
	for (ind = 0; ind <= sides; ind++) {
		inf = nxt(ine);
		lengths[ine] = tlen;
		clipSideLengths[ine] = hypot(currentFormVertices[inf].x - currentFormVertices[ine].x, currentFormVertices[inf].y - currentFormVertices[ine].y);
		tlen += clipSideLengths[ine];
		ine = inf;
	}
	clipSegments = (CLPSEG*)&StitchBuffer[MAXSEQ];
	nrct.left = floor(SelectedForm->rectangle.left / clipWidth);
	nrct.right = ceil(SelectedForm->rectangle.right / clipWidth);
	nrct.bottom = floor(SelectedForm->rectangle.bottom / ClipRectSize.cy - 1);
	nrct.top = ceil(SelectedForm->rectangle.top / ClipRectSize.cy + 1) + 2;
	nof = 0;
	if (SelectedForm->wordParam > 1)
		clpnof = SelectedForm->wordParam;
	else
		clpnof = 0;
	if (clpnof) {
		nrct.top++;
		if (SelectedForm->fillSpacing < 0) {
			nrct.bottom--;
			nrct.left -= (float)ClipRectSize.cx / clipWidth;
			nrct.right += (float)ClipRectSize.cx / clipWidth;
		}
	}
	if (clpneg && !clpnof)
		nrct.left -= (float)ClipRectSize.cx / clipWidth;
	if (nrct.bottom < 0) {
		nof = 1 - nrct.bottom;
		nrct.bottom += nof;
		nrct.top += nof;
		fnof = ClipRectSize.cy*nof;
		for (ind = 0; ind < sides; ind++)
			currentFormVertices[ind].y += fnof;
	}
	clipStitchPoints = (CLIPNT*)&bseq;
	segxs = 0;
	for (ind = 0; ind < sides; ind++) {
		strt = floor(currentFormVertices[ind].x / clipWidth);
		fin = floor((currentFormVertices[nxt(ind)].x) / clipWidth);
		if (strt > fin) {
			ine = strt;
			strt = fin;
			fin = ine;
		}
		if (SelectedForm->fillSpacing < 0)
			fin += ClipRectSize.cx / clipWidth;
		if (fin > (unsigned)nrct.right)
			fin = nrct.right;
		if (clpneg)
			strt -= (float)ClipRectSize.cx / clipWidth;
		for (ine = strt; ine <= fin; ine++) {
			regionCrossingData[segxs].sid = ind;
			regionCrossingData[segxs++].seg = ine;
		}
	}
	qsort((void*)regionCrossingData, segxs, 8, clpcmp);
	iclpx = (unsigned*)&regionCrossingData[segxs];
	ine = 1; inf = regionCrossingData[0].seg;
	iclpx[0] = 0;
	for (ind = 1; ind < segxs; ind++) {
		if (regionCrossingData[ind].seg != inf) {
			iclpx[ine++] = ind;
			inf = regionCrossingData[ind].seg;
		}
	}
	iclpx[ine] = ind;
	boundingRect.left = boundingRect.right = currentFormVertices[0].x;
	boundingRect.top = boundingRect.bottom = currentFormVertices[0].y;
	for (ind = 1; ind < sides; ind++)
	{
		if (currentFormVertices[ind].x > boundingRect.right)
			boundingRect.right = currentFormVertices[ind].x;
		if (currentFormVertices[ind].x < boundingRect.left)
			boundingRect.left = currentFormVertices[ind].x;
		if (currentFormVertices[ind].y > boundingRect.top)
			boundingRect.top = currentFormVertices[ind].y;
		if (currentFormVertices[ind].y < boundingRect.bottom)
			boundingRect.bottom = currentFormVertices[ind].y;
	}
	segps = ine;
	ind = regionCrossingStart = cnt = 0;
	seg = regionCrossingData[0].seg;
	clrnum = (nrct.top >> 5) + 1;
	activePointIndex = 0;
	for (ind = 0; ind < segps; ind++) {
		regionCrossingStart = iclpx[ind];
		regionCrossingEnd = iclpx[ind + 1];
		ploc.x = clipWidth*(ind + nrct.left);
		clpvof = 0;
		if (chkMap(TXFIL))
		{
			tine = (ind + nrct.left) % SelectedForm->fillInfo.texture.lines;
			ClipStitchCount = textureSegments[tine].stitchCount;
			ptx = &texturePointsBuffer[SelectedForm->fillInfo.texture.index + textureSegments[tine].line];
			vpnt0.x = ploc.x;
			if (SelectedForm->txof)
			{
				inf = (ind + nrct.left) / SelectedForm->fillInfo.texture.lines;
				clpvof = fmod(SelectedForm->txof*inf, SelectedForm->fillInfo.texture.height);
			}
		}
		else
		{
			if (clpnof)
				clpvof = (float)(ind%clpnof) / clpnof*ClipRectSize.cy;
			vpnt0.x = ploc.x + ClipBuffer[0].x;
		}
		vpnt0.y = nrct.bottom*ClipRectSize.cy;
		if (clpnof)
			clpvof = (float)(ind%clpnof) / clpnof*ClipRectSize.cy;
		for (tine = nrct.bottom; tine < nrct.top; tine++) {
			ploc.y = tine*ClipRectSize.cy - clpvof;
			vpnt1.x = ploc.x + ClipBuffer[0].x;
			vpnt1.y = ploc.y + ClipBuffer[0].y;
			if (!activePointIndex) {
				vpnt0.x = vpnt1.x;
				vpnt0.y = vpnt1.y;
			}
			for (inf = 0; inf < ClipStitchCount; inf++) {
				if (chkMap(TXFIL))
				{
					if (ptx != nullptr) {
						vpnt1.x = ploc.x;
						vpnt1.y = ploc.y + ptx[inf].y;
					}
				}
				else
				{
					vpnt1.x = ploc.x + ClipBuffer[inf].x;
					vpnt1.y = ploc.y + ClipBuffer[inf].y;
				}

				clipStitchPoints[activePointIndex].x = vpnt0.x;
				clipStitchPoints[activePointIndex].y = vpnt0.y;
				if (isin(vpnt0.x, vpnt0.y))
				{
					if (activePointIndex&&clipStitchPoints[activePointIndex - 1].flag == 2)
						inspnt();
					clipStitchPoints[activePointIndex].flag = 0;
				}
				else
				{
					if (activePointIndex && !clipStitchPoints[activePointIndex - 1].flag)
						inspnt();
					clipStitchPoints[activePointIndex].flag = 2;
				}
				activePointIndex++;
				cnt = insect();
				if (cnt)
				{
					for (ing = 0; ing < cnt; ing++) {
						if (ptrClipIntersectData != nullptr) {
							clipStitchPoints[activePointIndex].side = ptrClipIntersectData[ing]->lin;
							clipStitchPoints[activePointIndex].x = ptrClipIntersectData[ing]->point.x;
							clipStitchPoints[activePointIndex].y = ptrClipIntersectData[ing]->point.y;
							clipStitchPoints[activePointIndex].flag = 1;
							activePointIndex++;
							if (activePointIndex > MAXSEQ << 2)
								goto clpskp;
						}
					}
				}
				vpnt0.x = vpnt1.x;
				vpnt0.y = vpnt1.y;
			}
		}
		clipStitchPoints[activePointIndex - 1].flag = 2;
	}
clpskp:;

	clipStitchPoints[activePointIndex].flag = 2;
	if (nof) {
		fnof = nof*ClipRectSize.cy;
		for (ind = 0; ind < activePointIndex; ind++)
			clipStitchPoints[ind].y -= fnof;
		for (ind = 0; ind < sides; ind++)
			currentFormVertices[ind].y -= fnof;
	}
#define CLPVU 0


#if CLPVU==1

	goto clp1skp;

#endif

	clipSegmentIndex = 0;
	rstMap(FILDIR);
	ine = 0;
	if (activePointIndex)
	{
		for (ind = 0; ind < activePointIndex - 1; ind++) {
			switch (clipStitchPoints[ind].flag)
			{
			case 0:		//inside

				setMap(FILDIR);
				break;

			case 1:		//line

				if (toglMap(FILDIR))
					clpnseg(ine, ind);
				else
					ine = ind;
				break;

			case 2:		//outside

				rstMap(FILDIR);
				break;
			}
		}
	}

#if CLPVU==1

	clp1skp:;

#endif

			delete[] lengths;
			delete[] clipSideLengths;
			delete[] clipIntersectData;
			delete[] ptrClipIntersectData;

			if (clipSegmentIndex) {
				clplim = clipSegmentIndex >> 3;
				clplim = clipSegmentIndex >> 1;
				if (!clplim)
					clplim = 1;
				if (clplim > 12)
					clplim = 12;
				sortedLengths = (float**)&clipSegments[clipSegmentIndex];
				ine = 0;
				for (ind = 0; ind < clipSegmentIndex; ind++) {
					sortedLengths[ine++] = &clipSegments[ind].borderLength;
					sortedLengths[ine++] = &clipSegments[ind].edgeLength;
				}
				qsort((void*)sortedLengths, ine, 4, lencmp);
				ind = sizeof(CLPSEG);
				for (ind = 0; ind < ine; ind++) {
					inf = lenref(sortedLengths[ind]);
					ing = inf >> 1;
					if (inf & 1)
						clipSegments[ing].edgeIndex = ind;
					else
						clipSegments[ing].bind = ind;
				}

#if CLPVU==1

				for (ind = 0; ind < activePointIndex; ind++) {
					StitchBuffer[ind].x = clipStitchPoints[ind].x;
					StitchBuffer[ind].y = clipStitchPoints[ind].y;
					StitchBuffer[ind].attribute = 0;
				}
				PCSHeader.stitchCount = activePointIndex;
#endif

#if CLPVU==2

				inf = 0;
				for (ind = 0; ind < clipSegmentIndex; ind++) {
					for (ine = clipSegments[ind].start; ine <= clipSegments[ind].finish; ine++) {
						StitchBuffer[inf].x = clipStitchPoints[ine].x;
						StitchBuffer[inf].y = clipStitchPoints[ine].y;
						StitchBuffer[inf++].attribute = ind & 0xf;
					}
				}
				PCSHeader.stitchCount = inf;

#endif

				minx = 1e99;

#if CLPVU==0

				activePointIndex = 0;
				setMap(FILDIR);
				sequenceIndex = 0;
				clipIntersectSide = clipSegments[0].asid;
				ritseg();
				while (nucseg()) {
					if (sequenceIndex > MAXSEQ - 3)
						break;
					ritseg();
				}
				chksid(0);
				if (sequenceIndex > MAXSEQ - 100)
					sequenceIndex = MAXSEQ - 100;
				ine = 0; inf = 0;
				for (ind = 0; ind < sequenceIndex; ind++) {
					if (vscmp(ind, ine)) {
						ine++;
						oseq[ine].x = oseq[ind].x;
						oseq[ine].y = oseq[ind].y;
					}
					else
						inf++;
				}
				sequenceIndex = ine;
				if (chkMap(WASNEG)) {
					for (ind = 0; ind < sequenceIndex; ind++)
						oseq[ind].x -= formOffset;
					for (ind = 0; ind < sides; ind++)
						currentFormVertices[ind].x -= formOffset;
					SelectedForm->rectangle.left -= formOffset;
					SelectedForm->rectangle.right -= formOffset;
				}
#endif
			}
}

void vrtsclp() {
	unsigned ind;

	fvars(closestFormToCursor);
	delmclp(closestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->wordParam = IniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = ClipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = ClipBuffer[ind].y;
	}
	SelectedForm->fillType = VCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type = FRMFPOLY;
	refilfn();
}

void vrtclp() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThredWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							vrtsclp();
					}
					rstMap(NOCLP);
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
				else {
					if (chkMap(FORMSEL)) {
						fvars(closestFormToCursor);
						vrtsclp();
						setMap(INIT);
						coltab();
						setMap(RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void angout() {
	fRECTANGLE*		trct;
	unsigned	ine;

	if (angledForm.sides) {
		trct = &angledForm.rectangle;
		currentFormVertices = angledForm.vertices;
		trct->left = trct->right = currentFormVertices[0].x;
		trct->bottom = trct->top = currentFormVertices[0].y;
		for (ine = 1; ine < angledForm.sides; ine++) {
			if (currentFormVertices[ine].x > trct->right)
				trct->right = currentFormVertices[ine].x;
			if (currentFormVertices[ine].x < trct->left)
				trct->left = currentFormVertices[ine].x;
			if (currentFormVertices[ine].y < trct->bottom)
				trct->bottom = currentFormVertices[ine].y;
			if (currentFormVertices[ine].y > trct->top)
				trct->top = currentFormVertices[ine].y;
		}
	}
}

void horclpfn() {
	unsigned ind;

	frmcpy(&angledForm, &formList[closestFormToCursor]);
	RotationCenter.x = (double)(angledForm.rectangle.right - angledForm.rectangle.left) / 2 + angledForm.rectangle.left;
	RotationCenter.y = (double)(angledForm.rectangle.top - angledForm.rectangle.bottom) / 2 + angledForm.rectangle.bottom;
	angledForm.vertices = angledFormVertices;
	RotationAngle = PI / 2;
	for (ind = 0; ind < angledForm.sides; ind++) {
		angledForm.vertices[ind].x = SelectedForm->vertices[ind].x;
		angledForm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&angledForm.vertices[ind]);
	}
	angout();
	SelectedForm = &angledForm;
	currentFormVertices = angledForm.vertices;
	clpcon();
	RotationAngle = -PI / 2;
	rotbak();
	fvars(closestFormToCursor);
}

void horsclp() {
	unsigned ind;

	fvars(closestFormToCursor);
	delmclp(closestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->wordParam = IniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = ClipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = ClipBuffer[ind].y;
	}
	SelectedForm->fillType = HCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type = FRMFPOLY;
	currentFormVertices = SelectedForm->vertices;
	refilfn();
}

void horclp() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThredWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							horsclp();
					}
					setMap(NOCLP);
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
				else {
					if (chkMap(FORMSEL)) {
						fvars(closestFormToCursor);
						horsclp();
						setMap(INIT);
						coltab();
						setMap(RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void angclpfn()
{
	unsigned	ind;
	fPOINT*		tflt;

	frmcpy(&angledForm, &formList[closestFormToCursor]);
	RotationCenter.x = (double)(angledForm.rectangle.right - angledForm.rectangle.left) / 2 + angledForm.rectangle.left;
	RotationCenter.y = (double)(angledForm.rectangle.top - angledForm.rectangle.bottom) / 2 + angledForm.rectangle.bottom;
	angledForm.vertices = angledFormVertices;
	if (chkMap(ISUND))
	{
		RotationAngle = PI / 2 - SelectedForm->underlayStitchAngle;
		tflt = insid();
		for (ind = 0; ind < angledForm.sides; ind++)
		{
			angledFormVertices[ind].x = tflt[ind].x;
			angledFormVertices[ind].y = tflt[ind].y;
			rotflt(&angledFormVertices[ind]);
		}
	}
	else
	{
		if (chkMap(TXFIL))
			RotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
		else
			RotationAngle = PI / 2 - SelectedForm->satinOrAngle.angle;
		for (ind = 0; ind < angledForm.sides; ind++)
		{
			angledFormVertices[ind].x = SelectedForm->vertices[ind].x;
			angledFormVertices[ind].y = SelectedForm->vertices[ind].y;
			rotflt(&angledFormVertices[ind]);
		}
	}
	angout();
	SelectedForm = &angledForm;
	currentFormVertices = angledFormVertices;
	clpcon();
	RotationAngle = -RotationAngle;
	rotbak();
	fvars(closestFormToCursor);
}

void angsclp() {
	unsigned ind;

	fvars(closestFormToCursor);
	delmclp(closestFormToCursor);
	deltx();
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->wordParam = IniFile.fillPhase;
	makpoli();
	SelectedForm->satinOrAngle.angle = IniFile.fillAngle;
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = ClipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = ClipBuffer[ind].y;
	}
	SelectedForm->fillType = ANGCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type = FRMFPOLY;
	refilfn();
}

void angclp() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThredWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							angsclp();
					}
					rstMap(NOCLP);
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
				else {
					if (chkMap(FORMSEL)) {
						SelectedForm = &formList[closestFormToCursor];
						angsclp();
						setMap(INIT);
						coltab();
						setMap(RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else
			CloseClipboard();
	}
}

void dubfn() {
	unsigned ind, ine;

	brdfil(SelectedForm->edgeStitchLen);
	ine = sequenceIndex;
	for (ind = sequenceIndex; ind != 0; ind--) {
		oseq[ine].x = oseq[ind - 1].x;
		oseq[ine++].y = oseq[ind - 1].y;
	}
	sequenceIndex = ine;
}

void dubsfil() {
	fvars(closestFormToCursor);
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGEDOUBLE;
	sequenceIndex = 0;
	SelectedForm->borderColor = ActiveColor;
	dubfn();
	bsizpar();
	refilfn();
}

void dubfil() {
	unsigned ind;

	if (filmsgs(FML_LIN))
		return;
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			dubsfil();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			dubsfil();
			coltab();
			setMap(INIT);
			setMap(RESTCH);
		}
	}
}

void col2frm() {
	unsigned*	fcnts;
	unsigned*	bcnts;
	unsigned*	fthcnts;
	unsigned*	ucnts;
	unsigned	ind, ine, cod, cnt, maxcol;
	unsigned	strt, fin, chngcnt, fp16;
	TCHAR		buf[HBUFSIZ];

	chngcnt = 0;
	fp16 = formIndex << 4;
	if (formIndex) {
		fcnts = (unsigned*)&oseq;
		bcnts = (unsigned*)&bseq;
		fthcnts = &bcnts[fp16];
		ucnts = &fthcnts[fp16];
		for (ind = 0; ind < fp16; ind++) {
			fcnts[ind] = 0;
			bcnts[ind] = 0;
			fthcnts[ind] = 0;
			ucnts[ind] = 0;
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			cod = StitchBuffer[ind].attribute & 0x3fff;
			if (StitchBuffer[ind].attribute&(WLKMSK | CWLKMSK | UNDMSK))
				ucnts[cod]++;
			else
			{
				if (StitchBuffer[ind].attribute&FTHMSK)
					fthcnts[cod]++;
				else {
					switch (StitchBuffer[ind].attribute&TYPMSK) {
					case FRMFIL:

						fcnts[cod]++;
						break;

					case FRMBFIL:

						bcnts[cod]++;
						break;
					}
				}
			}
		}
		strt = 0; fin = 16;
		for (ind = 0; ind < formIndex; ind++) {
			if (formList[ind].fillType) {
				cnt = maxcol = 0;
				for (ine = strt; ine < fin; ine++) {
					if (fcnts[ine] > cnt) {
						cnt = fcnts[ine];
						maxcol = ine;
					}
				}
				maxcol &= 0xf;
				if (formList[ind].fillColor != maxcol) {
					chngcnt++;
					formList[ind].fillColor = maxcol;
				}
				if (formList[ind].fillType == FTHF&&formList[ind].extendedAttribute&AT_FTHBLND) {
					cnt = maxcol = 0;
					for (ine = strt; ine < fin; ine++) {
						if (fthcnts[ine] > cnt) {
							cnt = fcnts[ine];
							maxcol = ine;
						}
					}
					maxcol &= 0xf;
					if (formList[ind].fillInfo.feather.color != maxcol) {
						chngcnt++;
						formList[ind].fillInfo.feather.color = maxcol;
					}
				}
			}
			if (formList[ind].edgeType) {
				cnt = maxcol = 0;
				for (ine = strt; ine < fin; ine++) {
					if (bcnts[ine] > cnt) {
						cnt = bcnts[ine];
						maxcol = ine;
					}
				}
				maxcol &= 0xf;
				if (formList[ind].borderColor != maxcol) {
					chngcnt++;
					formList[ind].borderColor = maxcol;
				}
			}
			if (formList[ind].extendedAttribute&(AT_WALK | AT_CWLK | AT_UND)) {
				cnt = maxcol = 0;
				for (ine = strt; ine < fin; ine++) {
					if (ucnts[ine] > cnt) {
						cnt = bcnts[ine];
						maxcol = ine;
					}
				}
				maxcol &= 0xf;
				if (formList[ind].underlayColor != maxcol) {
					chngcnt++;
					formList[ind].underlayColor = maxcol;
				}
			}
			strt += 16;
			fin += 16;
		}
	}
	LoadString(ThredInstance, IDS_NCOLCHG, buf, HBUFSIZ);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), buf, chngcnt);
	shoMsg(MsgBuffer);
}

BOOL fxpnt() {
	double		len, dif;
	unsigned	ind;

	moveToPoint.x = currentFormVertices[nextStart].x;
	moveToPoint.y = currentFormVertices[nextStart].y;
	len = hypot(moveToPoint.x - SelectedPoint.x, moveToPoint.y - SelectedPoint.y);
	if (len > adjustedSpace) {
		for (ind = 0; ind < 10; ind++) {
			len = hypot(moveToPoint.x - SelectedPoint.x, moveToPoint.y - SelectedPoint.y);
			dif = adjustedSpace - len;
			moveToPoint.x += dif*listCOSINEs[currentSide];
			moveToPoint.y += dif*listSINEs[currentSide];
			if (fabs(dif) < 0.2)
				break;
		}
		return 1;
	}
	return 0;
}

void fxlit() {
	double		len;
	unsigned	cnt;
	dPOINT		dif;

	if (fxpnt()) {
		SelectedPoint.x = moveToPoint.x;
		SelectedPoint.y = moveToPoint.y;
		beanCount++;
		len = hypot(currentFormVertices[nextStart].x - SelectedPoint.x, currentFormVertices[nextStart].y - SelectedPoint.y);
		cnt = floor(len / adjustedSpace);
		dif.x = adjustedSpace*listCOSINEs[currentSide];
		dif.y = adjustedSpace*listSINEs[currentSide];
		SelectedPoint.x += dif.x*cnt;
		SelectedPoint.y += dif.y*cnt;
		beanCount += cnt;
	}
}

void fxlin() {
	double		len;
	unsigned	cnt;
	unsigned	ind;
	dPOINT		dif;

	if (fxpnt()) {
		chainEndPoints[activePointIndex].x = SelectedPoint.x = moveToPoint.x;
		chainEndPoints[activePointIndex].y = SelectedPoint.y = moveToPoint.y;
		activePointIndex++;
		len = hypot(currentFormVertices[nextStart].x - SelectedPoint.x, currentFormVertices[nextStart].y - SelectedPoint.y);
		cnt = floor(len / adjustedSpace);
		dif.x = adjustedSpace*listCOSINEs[currentSide];
		dif.y = adjustedSpace*listSINEs[currentSide];
		for (ind = 0; ind < cnt; ind++) {
			SelectedPoint.x += dif.x;
			SelectedPoint.y += dif.y;
			chainEndPoints[activePointIndex].x = SelectedPoint.x;
			chainEndPoints[activePointIndex].y = SelectedPoint.y;
			activePointIndex++;
		}
	}
}

void fxlen() {
	double		ter;
	double		minter;
	double		minspac;
	double		hival;
	double		loval;
	double		spac2;
	unsigned	inicnt;
	unsigned	lupcnt;
	unsigned	ind, mind;
	double		len;

	chainEndPoints = (fPOINT*)bseq;
	adjustedSpace = 0;
	for (ind = 1; ind < (unsigned)sides; ind++) {
		len = hypot(currentFormVertices[ind].x - currentFormVertices[0].x, currentFormVertices[ind].y - currentFormVertices[0].y);
		if (len > SelectedForm->edgeSpacing)
			goto fxlab;
		else {
			if (len > adjustedSpace) {
				adjustedSpace = len;
				mind = ind;
			}
		}
	}
	chainEndPoints[0].x = currentFormVertices[0].x;
	chainEndPoints[0].y = currentFormVertices[0].y;
	chainEndPoints[1].x = currentFormVertices[1].x;
	chainEndPoints[1].y = currentFormVertices[1].y;
	activePointIndex = 2;
	return;
fxlab:;
	adjustedSpace = minspac = SelectedForm->edgeSpacing;
	spac2 = adjustedSpace / 2;
	ter = minter = 1e9;
	lupcnt = inicnt = 0;
	loval = 0;
	hival = 1;
	while (lupcnt<100 && (hival - loval)>TINY) {
		beanCount = 0;
		SelectedPoint.x = currentFormVertices[0].x;
		SelectedPoint.y = currentFormVertices[0].y;
		activePointIndex = 1;
		chainEndPoints[0].x = SelectedPoint.x;
		chainEndPoints[0].y = SelectedPoint.y;
		for (currentSide = 0; currentSide < (unsigned)sides - 1; currentSide++) {
			nextStart = currentSide + 1;
			fxlit();
		}
		if (SelectedForm->type != FRMLINE) {
			nextStart = 0;
			fxlit();
		}
		else
			nextStart = sides - 1;
		if (!inicnt) {
			inicnt = beanCount;
			loval = adjustedSpace;
			minter = ter = (double)hypot(currentFormVertices[nextStart].x - SelectedPoint.x, currentFormVertices[nextStart].y - SelectedPoint.y);
			minspac = adjustedSpace;
			ter /= inicnt;
			adjustedSpace += ter / 2;
			hival = loval + ter;
		}
		else {
			ter = hypot(currentFormVertices[nextStart].x - SelectedPoint.x, currentFormVertices[nextStart].y - SelectedPoint.y);
			if (ter > spac2)
				ter = SelectedForm->edgeSpacing - ter;
			if (ter < minter) {
				minter = ter;
				minspac = adjustedSpace;
			}
			if (inicnt == beanCount)
				loval = adjustedSpace;
			else {
				if (beanCount > inicnt)
					hival = adjustedSpace;
				else
					loval = adjustedSpace;
			}
			adjustedSpace = loval + (hival - loval) / 2;
		}
		lupcnt++;
	}
	SelectedPoint.x = currentFormVertices[0].x;
	SelectedPoint.y = currentFormVertices[0].y;
	outputIndex = 1;
	adjustedSpace = minspac;
	for (currentSide = 0; currentSide < (unsigned)sides - 1; currentSide++) {
		nextStart = currentSide + 1;
		fxlin();
	}
	if (SelectedForm->type != FRMLINE) {
		nextStart = 0;
		fxlin();
	}
	ter = hypot(currentFormVertices[nextStart].x - SelectedPoint.x, currentFormVertices[nextStart].y - SelectedPoint.y);
	if (ter < spac2)
		activePointIndex--;
	chainEndPoints[activePointIndex].x = currentFormVertices[nextStart].x;
	chainEndPoints[activePointIndex].y = currentFormVertices[nextStart].y;
	activePointIndex++;
}

void duchfn(unsigned strt, unsigned fin) {
	fPOINT		pnts[5];
	float		midx;
	float		midy;
	dPOINT		dif;
	dPOINT		difrat;
	dPOINT		off;
	unsigned	ine;
	double		tang;

	dif.x = chainEndPoints[fin].x - chainEndPoints[strt].x;
	dif.y = chainEndPoints[fin].y - chainEndPoints[strt].y;
	difrat.x = dif.x*SelectedForm->edgeStitchLen;
	difrat.y = dif.y*SelectedForm->edgeStitchLen;
	pnts[0].x = chainEndPoints[strt].x;
	pnts[0].y = chainEndPoints[strt].y;
	pnts[4].x = chainEndPoints[fin].x;
	pnts[4].y = chainEndPoints[fin].y;
	tang = atan2(dif.y, dif.x) + PI / 2;
	off.x = cos(tang)*SelectedForm->borderSize;
	off.y = sin(tang)*SelectedForm->borderSize;
	midx = chainEndPoints[strt].x + difrat.x;
	midy = chainEndPoints[strt].y + difrat.y;
	pnts[1].x = midx + off.x;
	pnts[1].y = midy + off.y;
	pnts[3].x = midx - off.x;
	pnts[3].y = midy - off.y;
	dif.x = chainEndPoints[fin + 1].x - chainEndPoints[fin].x;
	dif.y = chainEndPoints[fin + 1].y - chainEndPoints[fin].y;
	pnts[2].x = chainEndPoints[fin].x + dif.x / 4;
	pnts[2].y = chainEndPoints[fin].y + dif.y / 4;
	for (ine = 0; ine < chainCount; ine++) {
		oseq[sequenceIndex].x = pnts[chainSequence[ine]].x;
		oseq[sequenceIndex].y = pnts[chainSequence[ine]].y;
		sequenceIndex++;
	}
}

void duch() {
	unsigned ind, bak;

	div4 = adjustedSpace / 4;
	sequenceIndex = 0;
	if (activePointIndex > 1) {
		for (ind = 0; ind < (unsigned)activePointIndex - 2; ind++)
			duchfn(ind, ind + 1);
		if (SelectedForm->type == FRMLINE) {
			duchfn(ind, ind + 1);
			bak = 8;
			if (chkMap(LINCHN))
				bak--;
			if ((sequenceIndex >= bak)) {
				oseq[sequenceIndex - bak].x = chainEndPoints[ind + 1].x;
				oseq[sequenceIndex - bak].y = chainEndPoints[ind + 1].y;
			}
			oseq[sequenceIndex].x = chainEndPoints[ind + 1].x;
			oseq[sequenceIndex++].y = chainEndPoints[ind + 1].y;
		}
		else {
			duchfn(ind, 0);
			oseq[sequenceIndex].x = chainEndPoints[activePointIndex - 1].x;
			oseq[sequenceIndex].y = chainEndPoints[activePointIndex - 1].y;
			sequenceIndex++;
		}
	}
	else
		tabmsg(IDS_CHANSMAL);
}

void dufxlen() {
	unsigned ind;

	duangs();
	// ToDo - Use locations that are not fPOINT for the list of doubles
	listSINEs = (double*)angledFormVertices;
	listCOSINEs = (double*)tempPolygon;
	for (ind = 0; ind < (unsigned)sides; ind++) {
		listSINEs[ind] = sin(formAngles[ind]);
		listCOSINEs[ind] = cos(formAngles[ind]);
	}
	listSINEs[ind] = sin((formAngles[0] > formAngles[ind]) ? (formAngles[0] - formAngles[ind]) : (formAngles[ind] - formAngles[0]));
	fxlen();
}

void chnfn() {
	chainCount = 10;
	if (chkMap(LINCHN))
		chainCount--;
	fvars(closestFormToCursor);
	deleclp(closestFormToCursor);
	dufxlen();
	dulast();
	sequenceIndex = 0;
	duch();
}

void chan() {
	SelectedForm->borderColor = ActiveColor;
	SelectedForm->edgeSpacing = IniFile.chainSpace;
	SelectedForm->borderSize = borderWidth;
	SelectedForm->edgeStitchLen = IniFile.chainRatio;
	SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	SelectedForm->minBorderStitchLen = MinStitchLength;
	if (chkMap(LINCHN))
		SelectedForm->edgeType = EDGELCHAIN;
	else
		SelectedForm->edgeType = EDGEOCHAIN;
	refilfn();
}

void chain() {
	unsigned ind;

	if (filmsgs(FML_CHAIN))
		return;
	savdo();
	if (selectedFormCount) {
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			chan();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			fvars(closestFormToCursor);
			chan();
			setMap(INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			setMap(RESTCH);
		}
	}
}

BOOL cisin(float pntx, float pnty) {
	unsigned	ind, acnt;
	unsigned	nvrt;
	dPOINT		ipnt;
	fRECTANGLE*		trct;

	trct = &SelectedForm->rectangle;
	if (pntx >= trct->right)
		return 0;
	if (pntx <= trct->left)
		return 0;
	if (pnty >= trct->top)
		return 0;
	if (pnty <= trct->bottom)
		return 0;
	acnt = 0;
	for (ind = 0; ind < sides; ind++)
	{
		nvrt = nxt(ind);
		if (projv(pntx, currentFormVertices[ind], currentFormVertices[nvrt], &ipnt))
		{
			if (ipnt.y >= pnty)
			{
				if (currentFormVertices[ind].x != pntx&&currentFormVertices[nvrt].x != pntx)
					acnt++;
				else
				{
					if (currentFormVertices[ind].x < currentFormVertices[nvrt].x)
					{
						if (currentFormVertices[nvrt].x != pntx)
							acnt++;
					}
					else
					{
						if (currentFormVertices[ind].x != pntx)
							acnt++;
					}
				}
			}
		}
	}
	return acnt & 1;
}

void crop() {
	unsigned ind, ine;

	frm1pnt();
	if (chkMap(FORMSEL)) {
		savdo();
		fvars(closestFormToCursor);
		ine = 0;
		regionCrossingStart = 0;
		regionCrossingEnd = sides;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if (cisin(StitchBuffer[ind].x, StitchBuffer[ind].y)) {
				StitchBuffer[ine].x = StitchBuffer[ind].x;
				StitchBuffer[ine].y = StitchBuffer[ind].y;
				StitchBuffer[ine++].attribute = StitchBuffer[ind].attribute;
			}
		}
		PCSHeader.stitchCount = ine;
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_FRM1MSG, IDS_CROP);
}

void xclpfn(unsigned strt, unsigned fin) {
	unsigned	ind;
	dPOINT		dif;
	double		len;
	double		rat;
	fPOINT*		tflt;

	tflt = (fPOINT*)&StitchBuffer[MAXPCS];
	dif.x = chainEndPoints[fin].x - chainEndPoints[strt].x;
	dif.y = chainEndPoints[fin].y - chainEndPoints[strt].y;
	len = hypot(dif.x, dif.y);
	rat = len / ClipRectSize.cx;
	RotationAngle = atan2(dif.y, dif.x);
	for (ind = 0; ind < ClipStitchCount; ind++) {
		tflt[ind].x = tmpClipPoints[ind].x*rat;
		tflt[ind].y = tmpClipPoints[ind].y;
		rotflt(&tflt[ind]);
		oseq[sequenceIndex].x = chainEndPoints[strt].x + tflt[ind].x;
		oseq[sequenceIndex++].y = chainEndPoints[strt].y + tflt[ind].y;
	}
}

void duxclp() {
	unsigned ind;

	duangs();
	dufxlen();
	clpxadj();
	sequenceIndex = 0;
	RotationCenter.x = RotationCenter.y = 0;
	for (ind = 1; ind < activePointIndex; ind++)
		xclpfn(ind - 1, ind);
	if (SelectedForm->type != FRMLINE) {
		oseq[sequenceIndex].x = chainEndPoints[0].x;
		oseq[sequenceIndex++].y = chainEndPoints[0].y;
	}
}

void dulast() {
	unsigned	ind, ine, mind;
	double		minlen;
	double		len;

	tmpClipPoints = (fPOINT*)&chainEndPoints[activePointIndex];
	if (lastch()) {
		minlen = 1e99;
		mind = 0;
		for (ind = 0; ind < activePointIndex; ind++) {
			len = hypot(lastPoint.x - chainEndPoints[ind].x, lastPoint.y - chainEndPoints[ind].y);
			if (len < minlen) {
				minlen = len;
				mind = ind;
			}
		}
		if (mind) {
			ine = 0;
			for (ind = mind; ind < activePointIndex - 1; ind++) {
				tmpClipPoints[ine].x = chainEndPoints[ind].x;
				tmpClipPoints[ine++].y = chainEndPoints[ind].y;
			}
			for (ind = 0; ind <= mind; ind++) {
				tmpClipPoints[ine].x = chainEndPoints[ind].x;
				tmpClipPoints[ine++].y = chainEndPoints[ind].y;
			}
			MoveMemory(chainEndPoints, tmpClipPoints, sizeof(fPOINT)*ine);
		}
	}
}

void clpxadj() {
	unsigned	ind;
	double		pivot;

	dulast();
	if (SelectedForm->type == FRMLINE) {
		pivot = ClipRectSize.cy / 2;
		for (ind = 0; ind < ClipStitchCount; ind++) {
			tmpClipPoints[ind].x = ClipBuffer[ind].x;
			tmpClipPoints[ind].y = -ClipBuffer[ind].y + pivot;
		}
	}
	else {
		for (ind = 0; ind < ClipStitchCount; ind++) {
			tmpClipPoints[ind].x = ClipBuffer[ind].x;
			tmpClipPoints[ind].y = -ClipBuffer[ind].y;
		}
	}
}

void fsclpx() {
	unsigned	ind;

	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EDGECLIPX;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(closestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = ClipRectSize.cx;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = ClipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = ClipBuffer[ind].y;
	}
	RotationCenter.x = RotationCenter.y = 0;
	duxclp();
	refilfn();
}

void filclpx() {
	unsigned	ind;

	if (filmsgs(FML_CLP))
		return;
	if (OpenClipboard(ThredWindow)) {
		fvars(closestFormToCursor);
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			savdo();
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (selectedFormCount) {
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						fsclpx();
					}
					setMap(INIT);
					coltab();
					setMap(RESTCH);
				}
				else {
					if (chkMap(FORMSEL)) {
						fsclpx();
						setMap(INIT);
						coltab();
						setMap(RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else {
			shoMsg("no clipboard data");
			CloseClipboard();
		}
	}
}

BOOL CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	TCHAR	buf[HBUFSIZ];

	switch (umsg) {
	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	reinit:;
		sprintf_s(buf, sizeof(buf), "%d", IniFile.wavePoints);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buf);
		sprintf_s(buf, sizeof(buf), "%d", IniFile.waveStart);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buf);
		sprintf_s(buf, sizeof(buf), "%d", IniFile.waveEnd);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buf);
		sprintf_s(buf, sizeof(buf), "%d", IniFile.waveLobes);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buf);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buf, HBUFSIZ);
			IniFile.wavePoints = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buf, HBUFSIZ);
			IniFile.waveStart = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buf, HBUFSIZ);
			IniFile.waveEnd = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buf, HBUFSIZ);
			IniFile.waveLobes = atoi(buf);
			if (IniFile.wavePoints > 100)
				IniFile.wavePoints = 100;
			if (IniFile.wavePoints < 3)
				IniFile.wavePoints = 3;
			if (IniFile.waveStart == IniFile.waveEnd)
				IniFile.waveEnd += (IniFile.wavePoints >> 2);
			IniFile.waveStart %= IniFile.wavePoints;
			IniFile.waveEnd %= IniFile.wavePoints;
			EndDialog(hwndlg, 1);
			break;

		case IDC_DEFWAV:

			IniFile.wavePoints = IWAVPNTS;
			IniFile.waveStart = IWAVSTRT;
			IniFile.waveEnd = IWAVEND;
			IniFile.waveLobes = IWAVS;
			goto reinit;
		}
	}
	return 0;
}

void wavfrm() {
	unsigned	ind, ine, inf;
	unsigned	end;
	unsigned	cnt;
	fPOINT*		tflt;
	fPOINT		pos;
	double		hrat;
	double		vrat;
	fPOINT		lfp_Size;

	unmsg();
	if (DialogBox(ThredInstance, MAKEINTRESOURCE(IDD_WAV), ThredWindow, (DLGPROC)wavprc)) {
		ind = FormVertexIndex;
		end = IniFile.waveEnd + 1;
		durpoli(IniFile.wavePoints);
		mdufrm();
		FormVertexIndex = ind;
		tflt = (fPOINT*)&bseq;
		ine = 0;
		ind = IniFile.waveStart;
		while (ind != IniFile.waveEnd) {
			inf = (ind + 1) % IniFile.wavePoints;
			tflt[ine].x = -currentFormVertices[inf].x + currentFormVertices[ind].x;
			tflt[ine].y = -currentFormVertices[inf].y + currentFormVertices[ind].y;
			ine++;
			ind = inf;
		}
		cnt = ine;
		ine = 0;
		pos.x = pos.y = 0;
		for (inf = 0; inf < IniFile.waveLobes; inf++) {
			if (inf & 1) {
				for (ind = 0; ind < cnt; ind++) {
					currentFormVertices[ine].x = pos.x;
					currentFormVertices[ine].y = pos.y;
					ine++;
					pos.x += tflt[ind].x;
					pos.y += tflt[ind].y;
				}
			}
			else {
				for (ind = cnt; ind != 0; ind--) {
					currentFormVertices[ine].x = pos.x;
					currentFormVertices[ine].y = pos.y;
					ine++;
					pos.x += tflt[ind - 1].x;
					pos.y += tflt[ind - 1].y;
				}
			}
		}
		currentFormVertices[ine].x = pos.x;
		currentFormVertices[ine].y = pos.y;
		ine++;
		RotationAngle = -atan2(currentFormVertices[ine - 1].y - currentFormVertices[0].y, currentFormVertices[ine - 1].x - currentFormVertices[0].x);
		for (ind = 0; ind < ine; ind++)
			rotflt(&currentFormVertices[ind]);
		SelectedForm->type = FRMLINE;
		SelectedForm->sides = ine;
		FormVertexIndex += ine;
		frmout(formIndex);
		rstMap(FORMSEL);
		lfp_Size.x = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		lfp_Size.y = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		hrat = UnzoomedRect.x / 4 / lfp_Size.x;
		if (hrat > 1)
			hrat = 1;
		vrat = UnzoomedRect.y / 4 / lfp_Size.y;
		if (vrat < hrat)
			hrat = vrat;
		if (hrat < 1) {
			for (ind = 0; ind < ine; ind++) {
				currentFormVertices[ind].x = (currentFormVertices[ind].x - currentFormVertices[0].x)*hrat + currentFormVertices[0].x;
				currentFormVertices[ind].y = (currentFormVertices[ind].y - currentFormVertices[0].y)*hrat + currentFormVertices[0].y;
			}
		}
		frmout(formIndex);
		for (ind = 0; ind < ine; ind++) {
			currentFormVertices[ind].x -= SelectedForm->rectangle.left;
			currentFormVertices[ind].y -= SelectedForm->rectangle.bottom;
		}
		formMoveDelta.x = formMoveDelta.y = 0;
		newFormVertexCount = ine + 1;
		setmfrm();
		mdufrm();
	}
}

void srtfrm() {
	unsigned	ind, ine, inf, tot, sav;
	unsigned	hst[MAXFORMS];
	fPOINTATTR*	hstch;

	if (PCSHeader.stitchCount) {
		savdo();
		FillMemory(hst, sizeof(unsigned)*MAXFORMS, 0);
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			hst[(StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT]++;
		tot = 0;
		for (ind = 0; ind < MAXFORMS; ind++) {
			sav = hst[ind];
			hst[ind] = tot;
			tot += sav;
		}
		hstch = &StitchBuffer[MAXSEQ];
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			ine = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
			inf = hst[ine]++;
			hstch[inf].x = StitchBuffer[ind].x;
			hstch[inf].y = StitchBuffer[ind].y;
			hstch[inf].attribute = StitchBuffer[ind].attribute;
		}
		MoveMemory(StitchBuffer, hstch, sizeof(fPOINTATTR)*PCSHeader.stitchCount);
		coltab();
		setMap(RESTCH);
	}
}

