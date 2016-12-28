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
extern void		moveStitchPoints (fPOINTATTRIBUTE* dst, fPOINTATTRIBUTE* src);
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
extern void		rotang1 (fPOINTATTRIBUTE rpnt, fPOINT* tpnt);
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
extern unsigned	setRmap (fPOINTATTRIBUTE sPnt);
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

extern			unsigned		activeColor;
extern			unsigned		activeLayer;
extern			unsigned		bitmapHeight;
extern			unsigned		bitmapWidth;
extern			unsigned		buttonHeight;
extern			unsigned		buttonWidth;
extern			unsigned		buttonWidthX3;
extern			fRECTANGLE		clipboardRect;
extern			FLSIZ			clipboardRectSize;
extern			unsigned		clipboardStitchCount;	//number of stitchs extracted from clipboard
extern			CLPSTCH*		clipboardStitchData;
extern			fPOINTATTRIBUTE	clipBuffer[MAXFRMLINS];
extern			unsigned		closestPointIndex;
extern			POINT			endPointCross;
extern			unsigned		formMenuChoice;
extern			HPEN			formPen;
extern			HPEN			formPen3px;
extern			HPEN			formSelectedPen;
#if	 __UseASM__
extern			unsigned		fsizeof;
#endif
extern			FSTRTS			fstrts;
extern			unsigned		groupEndStitch;
extern			unsigned		groupStartStitch;
extern			unsigned		groupStitchIndex;
extern			HCURSOR			hArrowCurs;
extern			HWND			hCancelButton;
extern			unsigned		hClip;
extern			HGLOBAL			hClipMem;
extern			HCURSOR			hCrossCurs;
extern			HWND			hDiscardButton;
extern			PCSHEADER		header;
extern			HMENU			hfillMenu;
extern			HWND			hFormData;
extern			HINSTANCE		hInst;
extern			HMENU			hMainMenu;
extern			HWND			hMainStitchWin;
extern			HWND			hMsg;
extern			HWND			hOKButton;
extern			HWND			hPreferencesWindow;
extern			HWND			hSideMessageWin;
extern			HWND			hsidWnd[11];
extern			HWND			hto;
extern			HWND			hWnd;
extern			INIFILE			iniFile;
extern			POINT			insertLine[3];
extern			fPOINT			iseq[MAXSEQ];
extern			unsigned		isind;
extern			unsigned		isind2;
extern			INSREC			isinds[10];
extern			fRECTANGLE		isrct;
extern			unsigned		layerIndex;
extern			HPEN			layerPen[5];
extern			POINT			mainWindowOrigin;
extern			unsigned		markedStitchMap[RMAPSIZ];
extern			double			minStitchLength;
extern			MSG				msg;
extern			TCHAR			msgbuf[MSGSIZ];
extern			unsigned		msgIndex;
extern			RECT			msgRct;
extern			HPEN			multiFormPen;
extern			unsigned		nearestCount;
extern			unsigned		numpnt;
extern			TCHAR*			pcdClipFormat;
extern			unsigned		preferenceIndex;
extern			void*			ptrClipVoid;
extern			FRMHED*			SelectedForm;	//pointer to selected form
extern			double			rotationAngle;
extern			dPOINT			rotationCenter;
extern			fRECTANGLE		rotationRect;
extern			POINT			scend;
extern			HPEN			selectAllPen;
extern			FORMPOINTS		selectedFormPoints;
extern			fPOINT			selectedFormsSize;
extern			RECT			selectedPixelsRect;
extern			fPOINT			selectedPoint;
extern			fRECTANGLE		selectedPointsRect;
extern			double			showStitchThreshold;
extern			TCHAR			sideWindowEntryBuffer[11];
extern			unsigned		slpnt;
extern			double			smallStitchLength;
extern			unsigned		smap;
extern			TCHAR*			stab[STR_LEN];
extern			double			stitchBoxesThreshold;
extern			fPOINTATTRIBUTE	stitchBuffer[MAXPCS];
extern			fRECTANGLE		stitchRangeRect;
extern			POINT			stitchSizePixels;
extern			RECT			stitchWindowClientRect;
extern			HDC				stitchWindowDC;
extern			HDC				stitchWindowMemDC;
extern			POINT			stitchWindowOrigin;
extern			POINT			stitchWindowSize;
extern			POINT			stretchBoxLine[5];
extern			TCHAR			thrName[_MAX_PATH];
extern			int				txad;
extern			TXPNT			txpnts[MAXSEQ];
extern			RNGCNT*			txsegs;
extern			fPOINT*			uflt;
extern			unsigned		underlayColor;
extern			void*			undoBuffer[16];
extern			unsigned		undoBufferWriteIndex;
extern			POINT			unzoomedRect;
extern			HPEN			userPen[16];
extern			double			userStitchLength;
extern			double			zoomFactor;
extern			dPOINT			zoomMarkPoint;
extern			dPOINT			zoomRatio;
extern			dRECTANGLE		zoomRect;

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
fPOINT			fmovdif;				//offset for moving forms
POINT			bakpnt;					//user moved a form point to here
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
fPOINT			opntlst[MAXFRMLINS];	//list of outside outline points for satin or clipboard fills
fPOINT			ipntlst[MAXFRMLINS];	//list of inside outline points for satin or clipboard fills
fPOINT*			opnts;					//pointer to the list of outside outline points
fPOINT*			ipnts;					//pointer to the list of inside outline points
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
unsigned		srtmsk = (1 << EGSAT) | (1 << EGAP) | (1 << EGPRP);	 //mask for switchable fill types
RCON*			pmap;					//path map for sequencing
unsigned		cpnt;					//number of entries in the path map
TCHAR*			visit;					//visited character map for sequencing
unsigned		vispnt;					//next unvisited region for sequencing
unsigned		rgcnt;					//number of regions to be sequenced
REGION*			rgns;					//a list of regions for sequencing
unsigned		dunrgn;					//last region sequenced
double			rgclos;					//region close enough threshold for sequencing
unsigned*		minds;					//pointers to sets of adjacent regions
RGSEQ*			rgpth;					//path to a region
unsigned		pthlen;					//length of the path to the region
unsigned		grindpnt;				//number of group indices
unsigned*		grinds;					//array of group indices for sequencing
unsigned		lastgrp;				//group of the last line written in the previous region;
FSEQ*			mpath;					//path of sequenced regions
RGSEQ*			tmpath;					//temporary path connections
unsigned		mpathi;					//index to path of sequenced regions
unsigned		nxtgrp;					//group that connects to the next region
unsigned*		seqmap;					//a bitmap of sequenced lines
unsigned*		srgns;					//an array of subregion starts
REGION*			durpnt;					//region currently being sequenced
fPOINT			dunpnts[4];				//corners of last region sequenced
FRMHED			angfrm;					//a temporary rotated form for angle fills
unsigned short	selectedFormList[MAXFORMS];		//a list of selected forms
unsigned		selectedFormCount = 0;	//number of selected forms
unsigned		previousFormIndex;		//previously selected form
RECT			selectedFormsRectangle;	//for multiple selections;
POINT			selectedFormsLine[9];	//line derived from the big rectangle
POINT			selectedPointsRectangle[9];	//line derived from the point select rectangle
fRECTANGLE		allItemsRectangle;		//rectangle enclosing all forms and stitches
double			formAngles[MAXFRMLINS];	//angles of a form for satin border fills
fPOINT			formPoints[MAXFRMPNTS];	//form points
unsigned		fltad;					//next index to append form points
fPOINT			clipboardPoints[MAXCLPNTS];		//main clipboard fill points for forms
unsigned		clpad;					//next index to append main clipboard points
SATCON			satks[MAXSAC];			//satin form connects
unsigned		satkad;					//next index to append satin connect points
float			buttonholeFillCornerLength = IBFCLEN;			//buttonhole corner length
float			picotSpace = IPICSPAC;		//space between border picots
unsigned		pseudoRandomValue;		//pseudo-random sequence register
unsigned		prgind;					//segment sorting index;
dPOINT			filbak[8];				//backup stitches in satin fills
unsigned		pfbak;					//pointer for backup stitches in satin fills
double			clpang;					//for clipboard border fill
dPOINT			mvpnt;					//moving point for clipboard fill
fPOINTATTRIBUTE	rclpnt;					//for rotating clipboard fill
double			cosang;					//cosine for clipboard fill
double			sinang;					//sine for clipboard fill
fPOINT			rclpref;				//reference for clipboard line border
unsigned		clpstrt;				//active form point for line clipboard fill
dPOINT			vct0;					//x size of the clipbord fill attribute the fill angle
FRMHED*			rfrmlst;				//temporary form header storage for reordering forms
fPOINT*			rflts;					//temporary form point storage for reording forms
SATCON*			rsats;					//temporary satin guidline storage for reording forms
fPOINT*			rclps;					//temporary clipboard point storage for reording forms
unsigned		frepnt;					//form relocator pointer
fPOINTATTRIBUTE*		hifstch;				//pointer to high stitch area for form sort
unsigned		beancnt;				//number of stitches added by convert to bean
FRMHED*			finspnt;				//insert form points in this form
unsigned		fgpnt0;					//form point storage for form point insert
unsigned		fgpnt1;					//form point storage for form point insert
SMALPNTL*		seqlin;					//line for vertical/horizontal/angle fills
unsigned		mpath0;					//point to the next path element for vertical fill sequencint
fPOINT*			lconflt;				//form points for angle fills
fPOINT			angflt[MAXFRMLINS];		//form point data for angle fills
fPOINT*			clpseq;					//pointer to vertical clipboard fill data
double			minot;					//minimum distance from a line
unsigned		minotl;					//index to the minimum distance line
CLPSEG*			clpsegs;				//clipboard segments for virtual clipboard fill
unsigned		pcseg;					//clipboard segment pointer
unsigned		pcseg2;					//clipboard segment pointer * 2
unsigned short	vclpsid;				//clipboard intersect side;
float**			plens;					//array of sorted side lengths for verticoal clipboard fill
fPOINT			vpnt0;					//vertical clipboard line segement start
fPOINT			vpnt1;					//vertical clipboard line segement end
double*			clplens;				//lengths of form sides for vertical clipboard fill
CLIPSORT*		clpsrt;					//intersect points for vertical clipboard fill
CLIPSORT**		pclpsrt;				//pointers to line intersect points
CLIPNT*			clipnts;				//points for vertical clipboard fills
VCLPX			vclpx[MAXFRMLINS];		//region crossing data for vertical clipboard fills
unsigned*		iclpx;					//indices into region crossing data for vertical clipboard fills
unsigned		vlim;					//wrap limit for vertical clipboard fills
unsigned		clplim;					//vertical clipboard search limit
float			clpcirc;				//circumference of the vertical clipboard fill form
float			clpcirc2;				//circumference of the vertical clipboard fill form / 2
float			strtlen;				//distance from zero point of first vertical clipboard segment
unsigned		vstrt;					//start of region crossing data for a particular region
unsigned		vfin;					//end of region crossing data for a particular region
float			clpwid;					//horizontal spacing for vertical clipboard fill
unsigned		regof;					//starting region for vertical clipboard fill
HWND			htim;					//prograss bar
HDC				timdc;					//progress bar device context
double			timstp;					//progress bar step
double			timpos;					//progress bar postiion
FLOAT			fltof;					//form offset for clipboard fills
long			prfwid;					//width of the preference window
double			egrat;					//ratio for shrinking eggs
unsigned		prfsiz;					//size of the text part of the preference window

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
		sub		eax, offset satks
		shr		eax, 2
	}
#else
	return pnt - satks;
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
	cnti = satkad - strti;
	src = satkad - 1;
	dst = satkad + cnt - 1;
	while (src >= strti) {
		satks[dst].start = satks[src].start;
		satks[dst--].finish = satks[src--].finish;
	}
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++) {
		if (formList[ind].type == SAT)
			formList[ind].satinOrAngle.sac += cnt;
	}
	satkad += cnt;
}

SATCON* nusac(unsigned pfrm, unsigned cnt) {
	unsigned ind, ine;

	ine = 0;
	for (ind = 0; ind < pfrm; ind++) {
		if (formList[ind].type == SAT)
			ine += formList[ind].satinGuideCount;
	}
	formList[pfrm].satinOrAngle.sac = &satks[ine];
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
			return formList[ind].borderClipData - clipboardPoints + formList[ind].clipEntries;
		if (isclp(ind))
			return formList[ind].angleOrClipData.clip - clipboardPoints + formList[ind].lengthOrCount.clipCount;
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

	if (clpad) {
		if (isclp(fpnt)) {
			dst = findclp(fpnt);
			src = dst + formList[fpnt].lengthOrCount.clipCount;
			MoveMemory(&clipboardPoints[dst], &clipboardPoints[src], sizeof(fPOINT)*(clpad - src));
			if (iseclp(fpnt))
				formList[fpnt].borderClipData -= formList[fpnt].lengthOrCount.clipCount;
			clpsub(fpnt, formList[fpnt].lengthOrCount.clipCount);
			if (clpad > formList[fpnt].lengthOrCount.clipCount)
				clpad -= formList[fpnt].lengthOrCount.clipCount;
			else
				clpad = 0;
			formList[fpnt].lengthOrCount.clipCount = 0;
		}
	}
}

void deleclp(unsigned fpnt) {
	unsigned	src, dst;

	if (clpad) {
		if (iseclpx(fpnt)) {
			dst = findclp(fpnt);
			src = dst + formList[fpnt].clipEntries;
			while (src < clpad) {
				clipboardPoints[dst].x = clipboardPoints[src].x;
				clipboardPoints[dst++].y = clipboardPoints[src++].y;
			}
			clpsub(fpnt, formList[fpnt].clipEntries);
			if (clpad > formList[fpnt].clipEntries)
				clpad -= formList[fpnt].clipEntries;
			else
				clpad = 0;
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
	src = clpad - 1;
	dst = clpad + cnt - 1;
	while (src >= find) {
		clipboardPoints[dst].x = clipboardPoints[src].x;
		clipboardPoints[dst--].y = clipboardPoints[src--].y;
	}
	formList[find].borderClipData = &clipboardPoints[find];
	for (ind = pfrm; ind < formIndex; ind++) {
		if (iseclpx(ind))
			formList[ind].borderClipData += cnt;
	}
	for (ind = pfrm + 1; ind < formIndex; ind++) {
		if (isclp(ind))
			formList[ind].angleOrClipData.clip += cnt;
	}
	clpad += cnt;
	return &clipboardPoints[find];
}

fPOINT* numclp() {
	int			find;
	int			src, dst;
	unsigned	ind;

	find = findclp(closestFormToCursor);
	src = clpad - 1;
	dst = clpad + clipboardStitchCount - 1;
	while (src >= find) {
		clipboardPoints[dst].x = clipboardPoints[src].x;
		clipboardPoints[dst--].y = clipboardPoints[src--].y;
	}
	formList[closestFormToCursor].angleOrClipData.clip = &clipboardPoints[find];
	if (iseclpx(closestFormToCursor))
		formList[closestFormToCursor].borderClipData += clipboardStitchCount;
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++) {
		if (isclpx(ind))
			formList[ind].angleOrClipData.clip += clipboardStitchCount;
		if (iseclpx(ind))
			formList[ind].borderClipData += clipboardStitchCount;
	}
	clpad += clipboardStitchCount;
	return &clipboardPoints[find];
}

unsigned fltind(fPOINT* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset formPoints
		shr		eax, 3
	}
#else
	return pnt - formPoints;
#endif
}

unsigned sacind(SATCON* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset satks
		shr		eax, 2
	}
#else
	return pnt - satks;
#endif
}

unsigned clpind(fPOINT* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset clipboardPoints
		shr		eax, 3
	}
#else
	return pnt - clipboardPoints;
#endif
}

void fltspac(fPOINT* strt, unsigned cnt) {
	int			strti;
	int			src, dst;
	unsigned	ind;

	strti = fltind(strt);
	src = fltad - 1;
	dst = fltad + cnt - 1;
	while (src >= strti) {
		formPoints[dst].x = formPoints[src].x;
		formPoints[dst--].y = formPoints[src--].y;
	}
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
		formList[ind].vertices += cnt;
	fltad += cnt;
}

void delsac(unsigned fpnt) {
	unsigned	ind;
	unsigned	src, dst;

	if (satkad) {
		if (formList[fpnt].type == SAT&&formList[fpnt].satinGuideCount) {
			dst = satind(formList[fpnt].satinOrAngle.sac);
			src = dst + formList[fpnt].satinGuideCount;
			while (src < satkad) {
				satks[dst].start = satks[src].start;
				satks[dst++].finish = satks[src++].finish;
			}
			for (ind = fpnt + 1; ind < formIndex; ind++) {
				if (formList[ind].type == SAT&&formList[ind].satinGuideCount)
					formList[ind].satinOrAngle.sac -= formList[fpnt].satinGuideCount;
			}
			satkad -= formList[fpnt].satinGuideCount;
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
		while (src < fltad) {
			formPoints[dst].x = formPoints[src].x;
			formPoints[dst++].y = formPoints[src++].y;
		}
		for (ind = fpnt + 1; ind < formIndex; ind++)
			formList[ind].vertices -= formList[fpnt].sides;
		fltad -= formList[fpnt].sides;
		if (fltad & 0x8000000)
			fltad = 0;
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
	while ((stitchBuffer[ind].attribute&(TYPMSK | FRMSK)) != at&&ind < header.stitchCount)
		ind++;
	ine = ind;
	if (ind < header.stitchCount) {
		setMap(WASDEL);
	}
	while (ind < header.stitchCount) {
		if ((stitchBuffer[ind].attribute&(TYPMSK | FRMSK)) != at) {
			stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
			stitchBuffer[ine].x = stitchBuffer[ind].x;
			stitchBuffer[ine++].y = stitchBuffer[ind].y;
		}
		ind++;
	}
	header.stitchCount = ine;
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
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		sat = stitchBuffer[ind].attribute;
		if (!(sat&NOTFRM))
		{
			if (!((sat&FRMSK) == at && (sat&(TYPFRM | FTHMSK))))
				mvstch(ine++, ind);
		}
		else
			mvstch(ine++, ind);
	}
	header.stitchCount = ine;
}

void fsizpar() {
	SelectedForm->maxFillStitchLen = iniFile.maxStitchLength;
	SelectedForm->lengthOrCount.stitchLength = userStitchLength;
	SelectedForm->minFillStitchLen = minStitchLength;
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
	SelectedForm->type = LIN;
	SelectedForm->fillType = CONTF;
	SelectedForm->attribute |= FRECONT;
}

unsigned find1st() {
	unsigned ind = 0, at;

	if (chk2of())
		return closestPointIndex;
	else {
		at = closestFormToCursor << 4;
		while (ind < header.stitchCount) {
			if ((stitchBuffer[ind].attribute&FRMSK) == at) {
				if (!(stitchBuffer[ind].attribute&NOTFRM))
					return ind;
			}
			ind++;
		}
		return ind;
	}
}

void ispcdclp() {
	rstMap(WASPCDCLP);
	if (OpenClipboard(hWnd)) {
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem)
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
	pxpnt->x = (stpnt.x - zoomRect.left)*zoomRatio.x + 0.5;
	pxpnt->y = stitchWindowClientRect.bottom - (stpnt.y - zoomRect.bottom)*zoomRatio.y + 0.5;
}

void stCor2px(fPOINTATTRIBUTE stpnt, POINT* pxpnt) {
	pxpnt->x = (stpnt.x - zoomRect.left)*zoomRatio.x + 0.5;
	pxpnt->y = stitchWindowClientRect.bottom - (stpnt.y - zoomRect.bottom)*zoomRatio.y + 0.5;
}

void px2stchf(POINT pxpnt, fPOINT* stpnt) {
	double	tdub;

	tdub = (double)(pxpnt.x - stitchWindowClientRect.left) / stitchWindowClientRect.right;
	stpnt->x = tdub*(zoomRect.right - zoomRect.left) + zoomRect.left;
	tdub = (double)(stitchWindowClientRect.bottom - pxpnt.y) / stitchWindowClientRect.bottom;
	stpnt->y = tdub*(zoomRect.top - zoomRect.bottom) + zoomRect.bottom;
}

void frmlin(fPOINT* scr, unsigned sidz) {
	unsigned ind;

	if (sides) {
		for (ind = 0; ind < sidz; ind++) {
			formLines[ind].x = (scr[ind].x - zoomRect.left)*zoomRatio.x;
			formLines[ind].y = stitchWindowClientRect.bottom - (scr[ind].y - zoomRect.bottom)*zoomRatio.y;
		}
		formLines[ind].x = (scr[0].x - zoomRect.left)*zoomRatio.x;
		formLines[ind].y = stitchWindowClientRect.bottom - (scr[0].y - zoomRect.bottom)*zoomRatio.y;
	}
}

void dufrm() {
	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	Polyline(stitchWindowDC, formLines, newFormVertexCount);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void unfrm() {
	if (rstMap(SHOFRM))
		dufrm();
}

void mdufrm() {
	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	if (formList[closestFormToCursor].type == LIN)
		Polyline(stitchWindowDC, formLines, newFormVertexCount - 1);
	else
		Polyline(stitchWindowDC, formLines, newFormVertexCount);
	SetROP2(stitchWindowMemDC, R2_COPYPEN);
}

void munfrm() {
	if (rstMap(SHOFRM))
		mdufrm();
}

void rats() {
	if (chkMap(ZUMED)) {
		horizontalRatio = (double)(zoomRect.right - zoomRect.left) / stitchWindowClientRect.right;
		verticalRatio = (double)(zoomRect.top - zoomRect.bottom) / stitchWindowClientRect.bottom;
	}
	else {
		horizontalRatio = (double)unzoomedRect.x / stitchWindowClientRect.right;
		verticalRatio = (double)unzoomedRect.y / stitchWindowClientRect.bottom;
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
	shoMsg(stab[STR_FMEN]);
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
	sqlin[1].x = stitchSizePixels.x;
	sqlin[1].y = stitchSizePixels.y;
	rat = (double)iniFile.formPointSizePixels / stitchWindowClientRect.right;
	len = (zoomRect.right - zoomRect.left)*rat * 2;
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
	sqlin[0].x = sqlin[3].x = stitchSizePixels.x;
	sqlin[0].y = sqlin[3].y = stitchSizePixels.y;
	l_dpnt.x = fpnt.x - off.x;
	l_dpnt.y = fpnt.y - off.y;
	stch2pxr(l_dpnt);
	sqlin[2].x = stitchSizePixels.x;
	sqlin[2].y = stitchSizePixels.y;
	Polyline(stitchWindowMemDC, sqlin, 4);
}

void selsqr(POINT p_cpnt, HDC dc) {
	POINT	sqlin[5];

	sqlin[0].x = sqlin[3].x = sqlin[4].x = p_cpnt.x - iniFile.formPointSizePixels;
	sqlin[0].y = sqlin[1].y = p_cpnt.y - iniFile.formPointSizePixels;
	sqlin[1].x = sqlin[2].x = p_cpnt.x + iniFile.formPointSizePixels;
	sqlin[2].y = sqlin[3].y = p_cpnt.y + iniFile.formPointSizePixels;
	sqlin[4].y = p_cpnt.y - iniFile.formPointSizePixels;
	Polyline(dc, sqlin, 5);
}

void frmsqr0(POINT p_cpnt) {
	POINT		sqlin[5];
	unsigned	pix;

	pix = iniFile.formBoxSizePixels;
	if (pix)
	{
		sqlin[0].x = sqlin[3].x = sqlin[4].x = p_cpnt.x - pix;
		sqlin[0].y = sqlin[1].y = p_cpnt.y - pix;
		sqlin[1].x = sqlin[2].x = p_cpnt.x + pix + 1;
		sqlin[2].y = sqlin[3].y = p_cpnt.y + pix + 1;
		sqlin[4].y = p_cpnt.y - 1;
		Polyline(stitchWindowMemDC, sqlin, 5);
	}
}

void frmx(POINT p_cpnt, HDC dc) {
	POINT	xlin[2];

	SelectObject(dc, formSelectedPen);
	xlin[0].x = xlin[1].x = p_cpnt.x;
	xlin[0].y = p_cpnt.y + 8;
	xlin[1].y = p_cpnt.y - 8;
	Polyline(dc, xlin, 2);
	xlin[0].y = xlin[1].y = p_cpnt.y;
	xlin[0].x = p_cpnt.x - 8;
	xlin[1].x = p_cpnt.x + 8;
	Polyline(dc, xlin, 2);
	SelectObject(dc, formPen);
}

void ratsr() {
	if (chkMap(ZUMED)) {
		horizontalRatio = (double)stitchWindowClientRect.right / (zoomRect.right - zoomRect.left);
		verticalRatio = (double)stitchWindowClientRect.bottom / (zoomRect.top - zoomRect.bottom);
	}
	else {
		horizontalRatio = (double)stitchWindowClientRect.right / unzoomedRect.x;
		verticalRatio = (double)stitchWindowClientRect.bottom / unzoomedRect.y;
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
	SelectObject(stitchWindowDC, formPen);
	SetROP2(stitchWindowDC, R2_XORPEN);
	getfinfo(ind);
	trct = &formList[ind].rectangle;
	SelectObject(dc, formSelectedPen);
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
	SetROP2(stitchWindowDC, R2_COPYPEN);
	if (rstMap(GRPSEL)) {
		rstMap(SELSHO);
		slpnt = 0;
		setMap(RESTCH);
	}
}

void delfrms() {
	unsigned ind;

	savdo();
	formIndex = fltad = satkad = clpad = 0;
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		stitchBuffer[ind].attribute &= NFRM_NTYP;
		stitchBuffer[ind].attribute |= NOTFRM;
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
		sqrlin[ind].x = (tlin[ind].x - zoomRect.left)*horizontalRatio;
		sqrlin[ind].y = (zoomRect.top - tlin[ind].y)*verticalRatio;
		if (sqrlin[ind].x < selectedFormsRectangle.left)
			selectedFormsRectangle.left = sqrlin[ind].x;
		if (sqrlin[ind].x > selectedFormsRectangle.right)
			selectedFormsRectangle.right = sqrlin[ind].x;
		if (sqrlin[ind].y < selectedFormsRectangle.top)
			selectedFormsRectangle.top = sqrlin[ind].y;
		if (sqrlin[ind].y > selectedFormsRectangle.bottom)
			selectedFormsRectangle.bottom = sqrlin[ind].y;
	}
	sqrlin[5].x = (tlin[0].x - zoomRect.left)*horizontalRatio;
	sqrlin[5].y = (zoomRect.top - tlin[0].y)*verticalRatio;
	if (sqrlin[5].x < selectedFormsRectangle.left)
		selectedFormsRectangle.left = sqrlin[5].x;
	if (sqrlin[5].x > selectedFormsRectangle.right)
		selectedFormsRectangle.right = sqrlin[5].x;
	if (sqrlin[5].y < selectedFormsRectangle.top)
		selectedFormsRectangle.top = sqrlin[5].y;
	if (sqrlin[5].y > selectedFormsRectangle.bottom)
		selectedFormsRectangle.bottom = sqrlin[5].y;
	Polyline(stitchWindowMemDC, sqrlin, 5);
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
	SelectObject(stitchWindowMemDC, selectAllPen);
	Polyline(stitchWindowMemDC, selectedFormsLine, 9);
	for (ind = 0; ind < 8; ind++)
		selsqr(selectedFormsLine[ind], stitchWindowMemDC);
}

void frmpoly(POINT* p_lin, unsigned cnt) {
	unsigned ind;

	if (cnt) {
		for (ind = 0; ind < cnt - 1; ind++)
			Polyline(stitchWindowMemDC, &p_lin[ind], 2);
	}
}

void dupsel(HDC dc) {
	unsigned ind;

	SelectObject(dc, formPen);
	SetROP2(dc, R2_XORPEN);
	Polyline(dc, selectedPointsRectangle, 9);
	ind = selectedFormPoints.start;
	for (ind = 0; ind < 8; ind++)
		selsqr(selectedPointsRectangle[ind], dc);
	frmx(endPointCross, dc);
}

void unpsel() {
	if (rstMap(SHOPSEL))
		dupsel(stitchWindowDC);
}

void drwfrm() {
	unsigned	ind, ine, inf, layr;
	POINT		l_lin[2];
	dPOINT		tpnt;

	rstMap(SHOMOV);
	rstMap(SHOPSEL);
	l_lin[0].x = l_lin[0].y = l_lin[1].x = l_lin[1].y = 0;
	Polyline(stitchWindowMemDC, l_lin, 2);
	SetROP2(stitchWindowMemDC, R2_XORPEN);
	ratsr();
	duzrat();
	for (ind = 0; ind < formIndex; ind++) {
		fvars(ind);
		frmlin(SelectedForm->vertices, sides);
		inf = 0;
		layr = ((SelectedForm->attribute&FRMLMSK) >> 1);
		if (!activeLayer || !layr || layr == activeLayer) {
			if (SelectedForm->type == SAT) {
				if (SelectedForm->attribute&FRMEND) {
					SelectObject(stitchWindowMemDC, formPen3px);
					Polyline(stitchWindowMemDC, formLines, 2);
					inf = 1;
				}
				if (SelectedForm->wordParam) {
					SelectObject(stitchWindowMemDC, formPen);
					frmpoly(&formLines[1], SelectedForm->wordParam);
					SelectObject(stitchWindowMemDC, formPen3px);
					Polyline(stitchWindowMemDC, &formLines[SelectedForm->wordParam], 2);
					SelectObject(stitchWindowMemDC, layerPen[layr]);
					inf = SelectedForm->wordParam + 1;
				}
				for (ine = 0; ine < formList[ind].satinGuideCount; ine++) {
					sfCor2px(currentFormVertices[currentFormConnections[ine].start], &l_lin[0]);
					sfCor2px(currentFormVertices[currentFormConnections[ine].finish], &l_lin[1]);
					SelectObject(stitchWindowMemDC, formPen);
					Polyline(stitchWindowMemDC, l_lin, 2);
				}
			}
			SelectObject(stitchWindowMemDC, layerPen[layr]);
			if (SelectedForm->type == LIN) {
				frmpoly(formLines, sides);
				if (SelectedForm->fillType == CONTF) {
					tpnt.x = currentFormVertices[SelectedForm->angleOrClipData.sat.start].x;
					tpnt.y = currentFormVertices[SelectedForm->angleOrClipData.sat.start].y;
					sCor2px(tpnt, &l_lin[0]);
					tpnt.x = currentFormVertices[SelectedForm->angleOrClipData.sat.finish].x;
					tpnt.y = currentFormVertices[SelectedForm->angleOrClipData.sat.finish].y;
					sCor2px(tpnt, &l_lin[1]);
					Polyline(stitchWindowMemDC, l_lin, 2);
				}
			}
			else
				frmpoly(&formLines[inf], sides + 1 - inf);
			if (closestFormToCursor == ind&&chkMap(FRMPSEL)) {
				for (ine = 1; ine < sides; ine++) {
					if (ine == closestVertexToCursor)
						frmx(formLines[ine], stitchWindowMemDC);
					else
						frmsqr(ine);
				}
				if (closestVertexToCursor)
					frmsqr0(formLines[0]);
				else
					frmx(formLines[0], stitchWindowMemDC);
				ritnum(STR_NUMPNT, closestVertexToCursor);
			}
			else {
				for (ine = 1; ine < sides; ine++)
					frmsqr(ine);
				SelectObject(stitchWindowMemDC, formSelectedPen);
				frmsqr0(formLines[0]);
			}
			if (chkMap(FPSEL) && closestFormToCursor == ind) {
				sRct2px(selectedPointsRect, &selectedPixelsRect);
				rct2sel(selectedPixelsRect, selectedPointsRectangle);
				setMap(SHOPSEL);
				dupsel(stitchWindowMemDC);
			}
		}
	}
	if (selectedFormCount) {
		SelectObject(stitchWindowMemDC, multiFormPen);
		ratsr();
		selectedFormsRectangle.top = selectedFormsRectangle.left = 0x7fffffff;
		selectedFormsRectangle.bottom = selectedFormsRectangle.right = 0;
		for (ind = 0; ind < selectedFormCount; ind++)
			fselrct(selectedFormList[ind]);
		selectedFormsSize.x = selectedFormsRectangle.right - selectedFormsRectangle.left;
		selectedFormsSize.y = selectedFormsRectangle.bottom - selectedFormsRectangle.top;
		dubig();
	}
	else {
		if (chkMap(FORMSEL))
			ritfrct(closestFormToCursor, stitchWindowMemDC);
		if (chkMap(FRMPMOV)) {
			ritmov();
			rubberBandLine[1].x = msg.pt.x - stitchWindowOrigin.x;
			rubberBandLine[1].y = msg.pt.y - stitchWindowOrigin.y;
			setMap(SHOMOV);
			ritmov();
		}
	}
	SetROP2(stitchWindowMemDC, R2_COPYPEN);
}

void setpoli() {
	frmclr(&formList[formIndex]);
	formList[formIndex].type = POLI;
	//frmtyp = POLI;
	formList[formIndex].attribute = activeLayer << 1;
	formList[formIndex].underlayIndent = iniFile.underlayIndent;
	setMap(SATIN);
	setMap(INIT);
}

void setlin() {
	frmclr(&formList[formIndex]);
	formList[formIndex].type = LIN;
	//frmtyp = LIN;
	formList[formIndex].attribute = activeLayer << 1;
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
		sprintf_s(buf, sizeof(buf), "%d", iniFile.formSides);
		SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buf);
		sprintf_s(buf, sizeof(buf), "%.3f", iniFile.tearTailLength);
		SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buf);
		sprintf_s(buf, sizeof(buf), "%.3f", iniFile.tearTwistStep / PFGRAN);
		SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buf);
		sprintf_s(buf, sizeof(buf), "%.3f", iniFile.tearTwistRatio);
		SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buf);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buf, HBUFSIZ);
			iniFile.formSides = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buf, HBUFSIZ);
			iniFile.tearTailLength = atof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buf, HBUFSIZ);
			iniFile.tearTwistStep = atof(buf)*PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buf, HBUFSIZ);
			iniFile.tearTwistRatio = atof(buf);
			EndDialog(hwndlg, 1);
			break;

		case IDC_DEFTEAR:

			iniFile.formSides = 20;
			iniFile.tearTailLength = (float)1.1;
			iniFile.tearTwistStep = (float)0.0;
			iniFile.tearTwistRatio = (float)1.6;
			goto reinit;

		case IDC_DEFPAIS:

			iniFile.formSides = 24;
			iniFile.tearTailLength = (float)1.15;
			iniFile.tearTwistStep = (float)0.3*PFGRAN;
			iniFile.tearTwistRatio = (float)1.8;
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
	ind = DialogBox(hInst, MAKEINTRESOURCE(IDD_TEAR), hWnd, (DLGPROC)tearprc);
	if (ind > 0) {
		xstp = iniFile.tearTwistStep;
		durpoli(iniFile.formSides);
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
			xstp *= iniFile.tearTwistRatio;
			vpos -= stp;
			stp *= iniFile.tearTailLength;
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
		fltad++;
		setMap(FORMSEL);
		fvars(formIndex);
		frmout(formIndex);
		flipv();
		rstMap(FORMSEL);
		lSize.x = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		lSize.y = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		hrat = unzoomedRect.x / 4 / lSize.x;
		if (hrat > 1)
			hrat = 1;
		vrat = unzoomedRect.y / 4 / lSize.y;
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
	case LIN:

		setlin();
		break;

	case POLI:

		setpoli();
		break;

	case RPOLI:

		setrpoli();
		break;

	case STAR:

		setstar();
		break;

	case SPIR:

		setspir();
		break;

	case HART - 1:

		sethart();
		break;

	case LENS - 1:

		setlens();
		break;

	case EGG - 1:

		seteg();
		break;

	case TEAR - 1:

		setear();
		break;

	case ZIG - 1:

		setzig();
		break;

	case WAV - 1:

		wavfrm();
		break;

	case DASY - 1:

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
		stchpx.x = msg.pt.x - stitchWindowOrigin.x;
		stchpx.y = msg.pt.y - stitchWindowOrigin.y;
		rats();
		tind = tine = 0;
		px2stchf(stchpx, &tpnt);
		playcod = activeLayer << 1;
		for (ind = 0; ind < formIndex; ind++) {
			if (chkMap(FRMSAM) && ind == closestFormToCursor)
				continue;

			frmcod = formList[ind].attribute&FRMLMSK;
			if (!activeLayer || !frmcod || frmcod == playcod) {
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
		minlen = hypot(stitchSizePixels.x - stchpx.x, stitchSizePixels.y - stchpx.y);
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
	if (SelectedForm->type == LIN)
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
	if (!chkmax(header.stitchCount, cnt))
	{
		MoveMemory(&stitchBuffer[strt + cnt], &stitchBuffer[strt], sizeof(fPOINTATTRIBUTE)*(header.stitchCount - strt));
		header.stitchCount += cnt;
	}
}

void rseq(unsigned strt, unsigned fin, unsigned ostrt, unsigned at) {
	while (strt < fin) {
		stitchBuffer[strt].attribute = at;
		stitchBuffer[strt].x = iseq[ostrt].x;
		stitchBuffer[strt++].y = iseq[ostrt++].y;
	}
	ostrt--;
	lastPoint.x = iseq[ostrt].x;
	lastPoint.y = iseq[ostrt].y;
}

BOOL ritlin(fPOINT strt, fPOINT fin)
{
	dPOINT		dif, stp, pnt;
	double		len;
	unsigned	ine, cnt;

	dif.x = fin.x - strt.x;
	dif.y = fin.y - strt.y;
	len = hypot(dif.x, dif.y);
	iseq[isind].x = strt.x;
	iseq[isind++].y = strt.y;
	if (len > maxStitchLen) {
		cnt = ceil(len / userStichLen);
		if (!cnt)
			cnt = 1;
		while (len / cnt > maxStitchLen)
			cnt++;
		if (!chkmax(isind, cnt)) {
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
			pnt.x = strt.x + stp.x;
			pnt.y = strt.y + stp.y;
			for (ine = 0; ine < cnt - 1; ine++) {
				if (isind&MAXMSK) {
					isind = MAXSEQ - 2;
					return 0;
				}
				iseq[isind].x = pnt.x;
				iseq[isind++].y = pnt.y;
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
		iseq[index].x = oseq[index].x;
		iseq[index].y = oseq[index].y;
	}
	isind = sequenceIndex;
#else

	double		len;
	unsigned	ind, ine, bakind;
	float		mins;

	bakind = isind;
	if (brd) {
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = iniFile.maxStitchLength;
		maxStitchLen = SelectedForm->maxBorderStitchLen;
		if (SelectedForm->edgeType == EGCHNL || SelectedForm->edgeType == EGCHNH)
			userStichLen = 9 * PFGRAN;
		else
			userStichLen = SelectedForm->edgeStitchLen;
		mins = SelectedForm->minBorderStitchLen;
	}
	else {
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = iniFile.maxStitchLength;
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
	iseq[isind].x = oseq[ind].x;
	iseq[isind++].y = oseq[ind].y;
seqskp:;
	if (!mins)
		return;
	ine = bakind + 1;
	for (ind = bakind + 1; ind < isind; ind++) {
		len = hypot(iseq[ind].x - iseq[ind - 1].x, iseq[ind].y - iseq[ind - 1].y);
		if (len > mins) {
			iseq[ine].x = iseq[ind].x;
			iseq[ine].y = iseq[ind].y;
			ine++;
		}
	}
	isind = ine;
#endif
}

void ritbrd() {
	if (sequenceIndex)
	{
		isinds[isind2].ind = isind;
		isinds[isind2].seq = I_BRD;
		isinds[isind2].cod = TYPBRD;
		isinds[isind2].col = SelectedForm->borderColor&COLMSK;
		chkseq(1);
		isind2++;
	}
}

void ritapbrd() {
	if (sequenceIndex)
	{
		isinds[isind2].ind = isind;
		isinds[isind2].seq = I_AP;
		isinds[isind2].cod = TYPMSK;
		isinds[isind2].col = SelectedForm->borderColor >> 4;
		chkseq(1);
		isind2++;
	}
}

void ritfil() {
	if (sequenceIndex)
	{
		isinds[isind2].ind = isind;
		isinds[isind2].seq = I_FIL;
		isinds[isind2].cod = TYPFRM;
		isinds[isind2].col = SelectedForm->fillColor;
		chkseq(0);
		isind2++;
	}
}

void okcan() {
	GetClientRect(hMsg, &msgRct);

	hOKButton = CreateWindow(
		"STATIC",
		stab[STR_OKENT],
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		msgRct.bottom + 15,
		buttonWidth << 2,
		buttonHeight,
		hMainStitchWin,
		NULL,
		hInst,
		NULL);

	hCancelButton = CreateWindow(
		"STATIC",
		stab[STR_CANCEL],
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidth * 5,
		msgRct.bottom + 15,
		buttonWidthX3,
		buttonHeight,
		hMainStitchWin,
		NULL,
		hInst,
		NULL);
}

void savdisc() {
	TCHAR	buf[HBUFSIZ];

	sdmsg();
	rstMap(BIGBOX);

	GetClientRect(hMsg, &msgRct);

	LoadString(hInst, IDS_SAV, buf, HBUFSIZ);
	hOKButton = CreateWindow(
		"STATIC",
		buf,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		msgRct.bottom + 15,
		buttonWidthX3,
		buttonHeight,
		hMainStitchWin,
		NULL,
		hInst,
		NULL);

	LoadString(hInst, IDS_DISC, buf, HBUFSIZ);
	hDiscardButton = CreateWindow(
		"STATIC",
		buf,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidthX3 + 15,
		msgRct.bottom + 15,
		buttonWidthX3,
		buttonHeight,
		hMainStitchWin,
		NULL,
		hInst,
		NULL);

	hCancelButton = CreateWindow(
		"STATIC",
		stab[STR_CANCEL],
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		2 * buttonWidthX3 + 25,
		msgRct.bottom + 15,
		buttonWidthX3,
		buttonHeight,
		hMainStitchWin,
		NULL,
		hInst,
		NULL);
}

BOOL lastch() {
	if (isind)
	{
		lastPoint.x = iseq[isind - 1].x;
		lastPoint.y = iseq[isind - 1].y;
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
	selectedPoint.x = currentFormVertices[strt].x;
	selectedPoint.y = currentFormVertices[strt].y;
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
			clipBuffer[ind].x = p_clp[ind].x;
			clipBuffer[ind].y = p_clp[ind].y;
		}
		clipboardRect.left = clipboardRect.right = clipBuffer[0].x;
		clipboardRect.bottom = clipboardRect.top = clipBuffer[0].y;
		for (ind = 1; ind < (unsigned)p_nclp; ind++) {
			if (clipBuffer[ind].x < clipboardRect.left)
				clipboardRect.left = clipBuffer[ind].x;
			if (clipBuffer[ind].x > clipboardRect.right)
				clipboardRect.right = clipBuffer[ind].x;
			if (clipBuffer[ind].y < clipboardRect.bottom)
				clipboardRect.bottom = clipBuffer[ind].y;
			if (clipBuffer[ind].y > clipboardRect.top)
				clipboardRect.top = clipBuffer[ind].y;
		}
		clipboardRectSize.cx = clipboardRect.right - clipboardRect.left;
		clipboardRectSize.cy = clipboardRect.top - clipboardRect.bottom;
		clipboardStitchCount = p_nclp;
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
		case EGLIN:

			brdfil(SelectedForm->edgeStitchLen);
			break;

		case EGBLD:

			bold(SelectedForm->edgeStitchLen);
			break;

		case EGCLP:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpbrd(getlast());
			break;

		case EGCLPX:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			duxclp();
			break;

		case EGSAT:

			plen = SelectedForm->borderSize;
			sbrd();
			break;

		case EGPRP:

			plen = SelectedForm->borderSize;
			pbrd(SelectedForm->edgeSpacing);
			break;

		case EGAP:

			sequenceIndex = 0;
			apbrd();
			ritapbrd();
			plen = SelectedForm->borderSize;
			sbrd();
			break;

		case EGHOL:

			plen = SelectedForm->borderSize * 2;
			tlen = buttonholeFillCornerLength;
			buttonholeFillCornerLength = getblen();
			satout(20);
			bhbrd(SelectedForm->edgeSpacing);
			buttonholeFillCornerLength = tlen;
			break;

		case EGPIC:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpic(0);
			break;

		case EGDUB:

			dubfn();
			break;

		case EGCHNL:

			setMap(LINCHN);
			chnfn();
			break;

		case EGCHNH:

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
	if (SelectedForm->type != LIN) {
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
	double		tsiz = userStitchLength;
	float		tlen;

	rstMap(TXFIL);
	fvars(closestFormToCursor);
	if (SelectedForm->type == LIN)
		SelectedForm->underlayIndent = 0;
	savdo();
	fdelstch();
	setMap(WASREFIL);
	if (SelectedForm->fillSpacing < 0.5 && !isclp(closestFormToCursor))
		SelectedForm->fillSpacing = 0.5;
	if (SelectedForm->edgeSpacing < 0.5)
		SelectedForm->edgeSpacing = 0.5;
	userStitchLength = SelectedForm->lengthOrCount.stitchLength;
	if (!chkMap(WASDO))
		savdo();
	rstMap(WASDO);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK) && SelectedForm->type == LIN&&SelectedForm->fillType != CONTF)
		SelectedForm->type = POLI;
	isind = isind2 = 0;
	rstMap(ISUND);
	switch (SelectedForm->type) {
	case LIN:

		plen = SelectedForm->borderSize;
		switch (SelectedForm->edgeType&NEGUND) {
		case EGLIN:

			brdfil(SelectedForm->edgeStitchLen);
			ritbrd();
			break;

		case EGBLD:

			bold(SelectedForm->edgeStitchLen);
			ritbrd();
			break;

		case EGCLP:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clpout();
			sequenceIndex = 0;
			clpbrd(0);
			ritbrd();
			break;

		case EGSAT:

			rstMap(SAT1);
			slbrd();
			ritbrd();
			break;

		case EGPRP:

			if (SelectedForm->sides > 2) {
				rstMap(SAT1);
				plbrd(SelectedForm->edgeSpacing);
				ritbrd();
			}
			break;

		case EGAP:

			lapbrd();
			ritapbrd();
			rstMap(SAT1);
			slbrd();
			ritbrd();
			break;

		case EGHOL:

			plen = SelectedForm->borderSize * 2;
			tlen = buttonholeFillCornerLength;
			buttonholeFillCornerLength = getblen();
			satout(20);
			blbrd(SelectedForm->edgeSpacing);
			buttonholeFillCornerLength = tlen;
			ritbrd();
			break;

		case EGPIC:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			sequenceIndex = 0;
			tlen = buttonholeFillCornerLength;
			buttonholeFillCornerLength = getplen();
			clpic(0);
			buttonholeFillCornerLength = tlen;
			ritbrd();
			break;

		case EGDUB:

			dubfn();
			ritbrd();
			break;

		case EGCHNL:

			setMap(LINCHN);
			chnfn();
			ritbrd();
			break;

		case EGCHNH:

			rstMap(LINCHN);
			chnfn();
			ritbrd();
			break;

		case EGCLPX:

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

	case POLI:

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
				lconflt = SelectedForm->vertices;
				break;

			case HORF:

				fnhor();
				lconflt = angfrm.vertices;
				break;

			case ANGF:

				rotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
				filang();
				lconflt = angfrm.vertices;
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
				rotationAngle = -rotationAngle;
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
			userStitchLength = SelectedForm->lengthOrCount.stitchLength;
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
	userStitchLength = tsiz;
	intlv();
	ritot(header.stitchCount);
	setfchk();
}

void refil() {
	unsigned trg, ind, at;

	if (!chku(WRNOF))
	{
		trg = (closestFormToCursor << FRMSHFT) | USMSK;
		for (ind = 0; ind < header.stitchCount; ind++) {
			at = stitchBuffer[ind].attribute;
			if (!(at&NOTFRM) && (at&(USMSK | FRMSK)) == trg) {
				if (hFormData)
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
	stchpx.x = msg.pt.x - stitchWindowOrigin.x;
	stchpx.y = msg.pt.y - stitchWindowOrigin.y;
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

	frmcpy(&angfrm, &formList[closestFormToCursor]);
	rotationCenter.x = (double)(angfrm.rectangle.right - angfrm.rectangle.left) / 2 + angfrm.rectangle.left;
	rotationCenter.y = (double)(angfrm.rectangle.top - angfrm.rectangle.bottom) / 2 + angfrm.rectangle.bottom;
	angfrm.vertices = angflt;
	for (ind = 0; ind < angfrm.sides; ind++) {
		angfrm.vertices[ind].x = SelectedForm->vertices[ind].x;
		angfrm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&angfrm.vertices[ind]);
	}
	SelectedForm = &angfrm;
	fnvrt();
	SelectedForm = &formList[closestFormToCursor];
}

void makpoli() {
	if (SelectedForm->type == SAT) {
		if (SelectedForm->satinGuideCount)
			delsac(closestFormToCursor);
	}
	SelectedForm->type = POLI;
}

void filinu(float pntx, float pnty) {
	unsigned	cnt;
	double		len;
	dPOINT		ipnt, stp, dif;

	ipnt.x = selectedPoint.x;
	ipnt.y = selectedPoint.y;
	dif.x = pntx - selectedPoint.x;
	dif.y = pnty - selectedPoint.y;
	len = hypot(dif.x, dif.y);
	cnt = len / userStitchLength;
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
	selectedPoint.x = pntx;
	selectedPoint.y = pnty;
}

void filin(dPOINT pnt) {
	dPOINT dif, stp, ipnt;
	double len;
	int cnt;

	dif.x = pnt.x - selectedPoint.x;
	dif.y = pnt.y - selectedPoint.y;
	len = hypot(dif.x, dif.y);
	ipnt.x = selectedPoint.x;
	ipnt.y = selectedPoint.y;
	cnt = len / userStitchLength;
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
	selectedPoint.x = pnt.x;
	selectedPoint.y = pnt.y;
}

unsigned short isclos(SMALPNTL* pnt0, SMALPNTL* pnt1) {
	float		lo0, hi0, lo1, hi1;

	hi0 = pnt0[1].y + rgclos;
	lo0 = pnt0[0].y - rgclos;
	hi1 = pnt1[1].y + rgclos;
	lo1 = pnt1[0].y - rgclos;
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

	if (gp1 > grindpnt - 2)
		return 0;
	if (!gp0)
		return 0;
	cnt0 = (grinds[gp0 + 1] - grinds[gp0]) >> 1;
	ind0 = 0;
	pnt0 = &lineEndpoints[grinds[gp0]];
	while (cnt0&&pnt0[ind0].line != ln0) {
		cnt0--;
		ind0 += 2;
	}
	if (cnt0) {
		cnt1 = (grinds[gp1 + 1] - grinds[gp1]) >> 1;
		ind1 = 0;
		pnt1 = &lineEndpoints[grinds[gp1]];
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

	pnt0s = &*ptrSortedLines[rgns[rg0].start];
	pnt1s = &*ptrSortedLines[rgns[rg1].start];
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
		nxtgrp = grps;
		return 1;
	}
	else {
		pnt0e = &*ptrSortedLines[rgns[rg0].end];
		pnt1e = &*ptrSortedLines[rgns[rg1].end];
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
			nxtgrp = grpe;
			return 1;
		}
	}
	if (((grp0s > grp1s) ? (grp0s - grp1s) : (grp1s - grp0s)) < 2) {
		if (isclos(pnt0s, pnt1s)) {
			nxtgrp = grp0s;
			return 1;
		}
	}
	if (((grp0s > grp1e) ? (grp0s - grp1e) : (grp1e - grp0s)) < 2) {
		if (isclos(pnt0s, pnt1e)) {
			nxtgrp = grp0s;
			return 1;
		}
	}
	if (((grp0e > grp1s) ? (grp0e - grp1s) : (grp1s - grp0e)) < 2) {
		if (isclos(pnt0e, pnt1s)) {
			nxtgrp = grp0e;
			return 1;
		}
	}
	if (((grp0e > grp1e) ? (grp0e - grp1e) : (grp1e - grp0e)) < 2) {
		if (isclos(pnt0e, pnt1e)) {
			nxtgrp = grp0e;
			return 1;
		}
	}
	return 0;
}

BOOL unvis() {
	for (vispnt = 0; vispnt < rgcnt; vispnt++) {
		if (!visit[vispnt])
			return 1;
	}
	return 0;
}

unsigned setseq(unsigned bpnt) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, seqmap
		mov		ecx, bpnt
		bts[ebx], ecx
		jnc		short setseqx
		inc		eax
		setseqx :
	}
#else
	return _bittestandset((long *)seqmap, bpnt);
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
	lastgrp = l_lin1->group;
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
	seqlin = &*ptrSortedLines[ind];
	rspnt((seqlin[1].x - seqlin[0].x) / 2 + seqlin[0].x, (seqlin[1].y - seqlin[0].y) / 2 + seqlin[0].y);
}

void duseq1() {
	rspnt((seqlin[1].x - seqlin[0].x) / 2 + seqlin[0].x, (seqlin[1].y - seqlin[0].y) / 2 + seqlin[0].y);
}

void duseq(unsigned strt, unsigned fin) {
	unsigned	ind, topbak;

	seqlin = nullptr;
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
						topbak = seqlin[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN))
					duseq2(ind + 1);
				seqlin = &*ptrSortedLines[ind];
				movseq(ind);
			}
		}
		if (rstMap(SEQDUN))
			duseq2(ind + 1);
		if (seqlin != nullptr) { lastgrp = seqlin->group; }
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
						topbak = seqlin[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN)) {
					if (ind)
						duseq2(ind - 1);
				}
				seqlin = &*ptrSortedLines[ind];
				movseq(ind);
			}
		}
		if (rstMap(SEQDUN)) {
			if (ind)
				duseq2(ind - 1);
		}
		if (seqlin != nullptr) { lastgrp = seqlin->group; }
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
				rspnt(seqlin[0].x, seqlin[0].y);
				seqlin = &*ptrSortedLines[ind];
				rspnt(seqlin[0].x, seqlin[0].y);
				bgrp = seqlin[0].group;
			}
			else
				seqlin = &*ptrSortedLines[ind];
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(ind);
		}
		lastgrp = seqlin->group;
	}
	else {
		bgrp = ptrSortedLines[strt]->group - 1;
		for (ind = strt; ind <= fin; ind++) {
			bgrp++;
			if (ptrSortedLines[ind]->group != bgrp) {
				rspnt(seqlin[0].x, seqlin[0].y);
				seqlin = &*ptrSortedLines[ind];
				rspnt(seqlin[0].x, seqlin[0].y);
				bgrp = seqlin[0].group;
			}
			else
				seqlin = &*ptrSortedLines[ind];
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(ind);
		}
		lastgrp = seqlin->group;
	}
	if (rstMap(SEQDUN))
		duseq1();
}

void brkdun(unsigned strt, unsigned fin) {
	rspnt(ptrSortedLines[strt]->x, ptrSortedLines[strt]->y);
	rspnt(ptrSortedLines[fin]->x, ptrSortedLines[fin]->y);
	rspnt(lconflt[ptrSortedLines[strt]->line].x, lconflt[ptrSortedLines[strt]->line].y);
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

	rgind = mpath[pthi].vrt;
	durpnt = &rgns[rgind];
	grpn = mpath[pthi].grpn;
	seqs = durpnt->start;
	seqe = durpnt->end;
	if (mpath[pthi].skp || rstMap(BRKFIX)) {
		if (bseq[outputIndex - 1].attribute != SEQBOT)
			rspnt(bseq[outputIndex - 2].x, bseq[outputIndex - 2].y);
		pnts = &*ptrSortedLines[rgind];
		dun = ptrSortedLines[seqs]->line;
		bpnt = &bseq[outputIndex - 1];
		minlen = 1e99;
		for (ind = 0; ind < sides; ind++) {
			len = hypot(bpnt->x - lconflt[ind].x, bpnt->y - lconflt[ind].y);
			if (len < minlen) {
				minlen = len;
				mindif = ind;
			}
		}
		if (minlen)
			rspnt(lconflt[mindif].x, lconflt[mindif].y);
		fdif = (sides + dun - mindif) % sides;
		bdif = (sides - dun + mindif) % sides;
		if (fdif < bdif) {
			ind = nxt(mindif);
			while (ind != dun) {
				rspnt(lconflt[ind].x, lconflt[ind].y);
				ind = nxt(ind);
			}
			rspnt(lconflt[ind].x, lconflt[ind].y);
		}
		else {
			ind = prv(mindif);
			while (ind != dun) {
				rspnt(lconflt[ind].x, lconflt[ind].y);
				ind = prv(ind);
			}
			rspnt(lconflt[ind].x, lconflt[ind].y);
		}
	}
	if (visit[rgind])
		dun = 1;
	else {
		dun = 0;
		visit[rgind]++;
	}
	pnts = &*ptrSortedLines[durpnt->start];
	pnte = &*ptrSortedLines[durpnt->end];
	grps = pnts->group;
	grpe = pnte->group;
	if (grpe != grps)
		seql = (double)(lastgrp - grps) / (grpe - grps)*(seqe - seqs) + seqs;
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
	if (ptrSortedLines[seql]->group != lastgrp) {
		if (seql < seqe&&ptrSortedLines[seql + 1]->group == lastgrp)
			seql++;
		else {
			if (seql > seqs&&ptrSortedLines[seql - 1]->group == lastgrp)
				seql--;
			else {
				mindif = 0xffffffff;
				for (ind = seqs; ind <= seqe; ind++) {
					gdif = ((ptrSortedLines[ind]->group > lastgrp) ? (ptrSortedLines[ind]->group - lastgrp) : (lastgrp - ptrSortedLines[ind]->group));
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
	if (durpnt->cntbrk) {
		if (dun) {
			brkdun(seql, seqn);
		}
		else {
			if (lastgrp >= grpe) {
				brkseq(seqe, seqs);
				if (pthi < mpathi - 1 && seqe != seqn)
					brkseq(seqs, seqn);
			}
			else {
				if (grps <= grpn) {
					if (seql != seqs)
						brkseq(seql, seqs);
					brkseq(seqs, seqe);
					if (pthi < mpathi - 1 && seqe != seqn)
						brkseq(seqe, seqn);
				}
				else {
					if (seql != seqe)
						brkseq(seql, seqe);
					brkseq(seqe, seqs);
					if (pthi < mpathi - 1 && seqs != seqn)
						brkseq(seqs, seqn);
				}
			}
		}
	}
	else {
		if (dun)
			dunseq(seql, seqn);
		else {
			if (lastgrp >= grpe) {
				duseq(seqe, seqs);
				duseq(seqs, seqn);
			}
			else {
				if (grps <= grpn) {
					if (seql != seqs)
						duseq(seql, seqs);
					duseq(seqs, seqe);
					if (pthi < mpathi - 1 && seqe != seqn)
						duseq(seqe, seqn);
				}
				else {
					if (seql != seqe)
						duseq(seql, seqe);
					duseq(seqe, seqs);
					if (pthi < mpathi - 1 && seqs != seqn)
						duseq(seqs, seqn);
				}
			}
		}
	}
}

unsigned notdun(unsigned lvl) {
	unsigned	ind;
	int			tpiv, pivot = lvl - 1;

	rgpth = &tmpath[mpathi];
	rgpth[0].pcon = minds[dunrgn];
	rgpth[0].cnt = minds[dunrgn + 1] - rgpth[0].pcon;
	for (ind = 1; ind < lvl; ind++) {
		rgpth[ind].pcon = minds[pmap[rgpth[ind - 1].pcon].vrt];
		rgpth[ind].cnt = minds[pmap[rgpth[ind - 1].pcon].vrt + 1] - rgpth[ind].pcon;
	}
	while (visit[pmap[rgpth[pivot].pcon].vrt] && pivot >= 0) {
		if (--rgpth[pivot].cnt > 0)
			rgpth[pivot].pcon++;
		else {
			tpiv = pivot;
			do {
				tpiv--;
				if (tpiv < 0)
					return 1;
				rgpth[tpiv].cnt--;
				rgpth[tpiv].pcon++;
			} while (!rgpth[tpiv].cnt);
			if (tpiv < 0)
				return 1;
			tpiv++;
			while (tpiv <= pivot) {
				if (tpiv) {
					rgpth[tpiv].pcon = minds[pmap[rgpth[tpiv - 1].pcon].vrt];
					rgpth[tpiv].cnt = minds[pmap[rgpth[tpiv - 1].pcon].vrt + 1] - rgpth[tpiv].pcon;
				}
				else {
					if (--rgpth[0].cnt)
						rgpth[0].pcon++;
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

	pnts[0] = ptrSortedLines[rgns[reg].start];
	pnts[1] = &ptrSortedLines[rgns[reg].start][1];
	pnts[2] = ptrSortedLines[rgns[reg].end];
	pnts[3] = &ptrSortedLines[rgns[reg].end][1];
	for (ind = 0; ind < 4; ind++) {
		for (ine = 0; ine < 4; ine++) {
			len = hypot(dunpnts[ind].x - pnts[ine]->x, dunpnts[ind].y - pnts[ine]->y);
			if (len < minlen)
				minlen = len;
		}
	}
	return minlen;
}

void nxtrgn() {
	unsigned	ind, nureg;;
	SMALPNTL*	tpnt;
	double		len, minlen = 1e99;

	pthlen = 1;
	while (notdun(pthlen)) {
		pthlen++;
		if (pthlen > 8) {
			tpnt = &*ptrSortedLines[rgns[dunrgn].start];
			dunpnts[0].x = tpnt[0].x;
			dunpnts[0].y = tpnt[0].y;
			dunpnts[1].x = tpnt[1].x;
			dunpnts[1].y = tpnt[1].y;
			tpnt = &*ptrSortedLines[rgns[dunrgn].end];
			dunpnts[2].x = tpnt[0].x;
			dunpnts[2].y = tpnt[0].y;
			dunpnts[3].x = tpnt[1].x;
			dunpnts[3].y = tpnt[1].y;
			nureg = 0;
			for (ind = 0; ind < rgcnt; ind++) {
				if (!visit[ind]) {
					len = reglen(ind);
					if (len < minlen) {
						minlen = len;
						nureg = ind;
					}
				}
			}
			tmpath[mpathi].skp = 1;
			for (ind = 0; ind < cpnt; ind++) {
				if (pmap[ind].vrt == nureg) {
					tmpath[mpathi++].pcon = ind;
					visit[nureg] = 1;
					dunrgn = nureg;
					return;
				}
			}
			tmpath[mpathi].cnt = vispnt;
			tmpath[mpathi++].pcon = 0xffffffff;
			visit[vispnt] = 1;
			dunrgn = vispnt;
			return;
		}
	}
	for (ind = 0; ind < pthlen; ind++) {
		tmpath[mpathi].skp = 0;
		tmpath[mpathi++].pcon = rgpth[ind].pcon;
		visit[pmap[rgpth[ind].pcon].vrt] = 1;
	}
	dunrgn = pmap[rgpth[ind - 1].pcon].vrt;
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

	ind = minds[mpath[pthi].vrt];
	nxtvrt = mpath[pthi + 1].vrt;
	while (ind < minds[mpath[pthi].vrt + 1] && pmap[ind].vrt != nxtvrt) {
		ind++;
	}
	mpath[mpath0++].grpn = pmap[ind].grpn;
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
		rgcnt = 0;
		trgns = (REGION*)oseq;
		trgns[0].start = 0;
		l_bLine = ptrSortedLines[0]->line;
		for (ind = 0; ind < sortedLineIndex; ind++) {
			if (l_bLine != ptrSortedLines[ind]->line) {
				trgns[rgcnt++].end = ind - 1;
				trgns[rgcnt].start = ind;
				l_bLine = ptrSortedLines[ind]->line;
			}
		}
		trgns[rgcnt++].end = ind - 1;
		rgns = new REGION[rgcnt];
		visit = new char[rgcnt];
		for (ind = 0; ind < rgcnt; ind++) {
			rgns[ind].start = trgns[ind].start;
			rgns[ind].end = trgns[ind].end;
			visit[ind] = 0;
			rgns[ind].cntbrk = 0;
		}
		tsrgns = (unsigned*)oseq;
		sind = 0;
		for (ind = 0; ind < rgcnt; ind++) {
			cnt = 0;
			if ((rgns[ind].end - rgns[ind].start) > 1) {
				sgrp = ptrSortedLines[rgns[ind].start]->group;
				for (ine = rgns[ind].start + 1; ine <= rgns[ind].end; ine++) {
					sgrp++;
					if (ptrSortedLines[ine]->group != sgrp) {
						if (!cnt)
							rgns[ind].brk = sind;
						cnt++;
						sgrp = ptrSortedLines[ine]->group;
						tsrgns[sind++] = ine;
					}
				}
			}
			rgns[ind].cntbrk = cnt;
		}
		srgns = new unsigned[sind];
		for (ind = 0; ind < sind; ind++)
			srgns[ind] = tsrgns[ind];
		tmap = (RCON*)bseq;
		minds = new unsigned[rgcnt + 1];

#if BUGSEQ
		bugcol = 0; sequenceIndex = 0;
		for (index = 0; index < rgcnt; index++) {
			for (ine = rgns[index].start; ine <= rgns[index].fillEnd; ine++) {
				tpnt = &*ptrSortedLines[ine];
				stitchCount[sequenceIndex].attribute = bugcol;
				stitchCount[sequenceIndex].x = tpnt[0].x;
				stitchCount[sequenceIndex++].y = tpnt[0].y;
				stitchCount[sequenceIndex].attribute = bugcol;
				stitchCount[sequenceIndex].x = tpnt[1].x;
				stitchCount[sequenceIndex++].y = tpnt[1].y;
			}
			bugcol++;
			bugcol &= 0xf;
		}
		header.stitchCount = sequenceIndex;
		goto seqskip;
#endif
		outputIndex = 0;
		if (rgcnt > 1) {
			ine = 0; cpnt = 0;
			for (ind = 0; ind < rgcnt; ind++) {
				pcon = &tmap[cpnt];
				minds[ind] = cpnt;
				cnt = 0; rgclos = 0;
				for (ine = 0; ine < rgcnt; ine++) {
					if (ind != ine) {
						tcon = regclos(ind, ine);
						if (tcon) {
							tmap[cpnt].con = tcon;
							tmap[cpnt].grpn = nxtgrp;
							tmap[cpnt++].vrt = ine;
							cnt++;
						}
					}
				}
				while (!cnt) {
					rgclos += stitchSpace;
					cnt = 0;
					for (ine = 0; ine < rgcnt; ine++) {
						if (ind != ine) {
							tcon = regclos(ind, ine);
							if (tcon) {
								tmap[cpnt].con = tcon;
								tmap[cpnt].grpn = nxtgrp;
								tmap[cpnt++].vrt = ine;
								cnt++;
							}
						}
					}
				}
			}
			minds[ind] = cpnt;
			pmap = new RCON[cpnt + 1];
			for (ind = 0; ind < cpnt; ind++) {
				pmap[ind].con = tmap[ind].con;
				pmap[ind].vrt = tmap[ind].vrt;
				pmap[ind].grpn = tmap[ind].grpn;
			}
			//find the leftmost region
			sgrp = 0xffffffff; ine = 0;
			for (ind = 0; ind < rgcnt; ind++) {
				tpnt = &*ptrSortedLines[rgns[ind].start];
				if (tpnt->group < sgrp) {
					sgrp = tpnt->group;
					ine = ind;
				}
			}
			outputIndex = 0;
			tmpath = (RGSEQ*)oseq;
			//find the leftmost region in pmap
			mpathi = 1;
			for (ind = 0; ind < cpnt; ind++) {
				if (pmap[ind].vrt == ine)
					goto lconskip;
			}
			pmap[cpnt].vrt = ine;
			pmap[cpnt].grpn = 0;
			ind = cpnt;
		lconskip:;
			//set the first entry in the temporary path to the leftmost region
			tmpath[0].pcon = ind;
			tmpath[0].cnt = 1;
			tmpath[0].skp = 0;
			visit[ine] = 1;
			dunrgn = ine;
			while (unvis())
				nxtrgn();
			ine = 0;
			cnt = 0xffffffff;
			mpath = (FSEQ*)&oseq[OSEQLEN >> 1];
			for (ind = 0; ind < mpathi; ind++) {
				mpath[ine].skp = tmpath[ind].skp;
				if (tmpath[ind].pcon == 0xffffffff) {
					mpath[ine++].vrt = tmpath[ind].cnt;
					cnt = tmpath[ind].cnt;
				}
				else {
					if (tmpath[ind].pcon != cnt) {
						cnt = tmpath[ind].pcon;
						mpath[ine++].vrt = pmap[tmpath[ind].pcon].vrt;
					}
				}
			}
			mpathi = ind; mpath0 = 0;
			for (ind = 0; ind < mpathi; ind++)
				nxtseq(ind);
			ine = (sortedLineIndex >> 5) + 1;
			seqmap = new unsigned[ine];
			for (ind = 0; ind < ine; ind++)
				seqmap[ind] = 0;
			for (ind = 0; ind < rgcnt; ind++)
				visit[ind] = 0;
			lastgrp = 0;
			for (ind = 0; ind < mpath0; ind++) {
				//				sprintf_s(msgbuf, sizeof(msgbuf),"ind %d,vrt %d,grpn %d\n",ind,pmap[ind].vrt,pmap[ind].grpn);
				//				OutputDebugString(msgbuf);
				if (!unvis())
					break;
				durgn(ind);
			}
		}
		else {
			pmap = new RCON[1];
			mpath = new FSEQ[1];
			ine = (sortedLineIndex >> 5) + 1;
			seqmap = new unsigned[ine];
			for (ind = 0; ind < ine; ind++)
				seqmap[ind] = 0;
			lastgrp = 0;
			mpath[0].vrt = 0;
			mpath[0].grpn = ptrSortedLines[rgns[0].end]->group;
			mpath[0].skp = 0;
			durgn(0);
			delete[] mpath;
		}
		//skip:;

#if BUGSEQ

		seqskip : ;
#endif
				  delete[] ptrSortedLines;
				  delete[] lineEndpoints;
				  delete[] rgns;
				  delete[] minds;
				  delete[] visit;
				  delete[] pmap;
				  delete[] grinds;
				  delete[] seqmap;
				  delete[] srgns;
	}
}

BOOL notin(unsigned ond) {
	dPOINT		pnt;
	unsigned	ind, cnt;
	dPOINT		ipnt;
	double		len;

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
	double		usesiz2 = userStitchLength * 2;
	double		usesiz9 = userStitchLength / 9;
	double		stspac2 = stitchSpace * 2;

	sequenceIndex = 0;
	rstMap(FILDIR);
	ind = outputIndex - 1;
	oseq[sequenceIndex].x = bseq[ind].x;
	oseq[sequenceIndex].y = bseq[ind].y;
	sequenceIndex++;
	selectedPoint.x = bseq[ind].x;
	selectedPoint.y = bseq[ind].y;
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
					cnt = ceil(bseq[ind].y / userStitchLength);
				blntop:;
					oseq[sequenceIndex].y = cnt*userStitchLength + (rit%seqtab[rcnt])*usesiz9;
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
					cnt = floor(bseq[ind].y / userStitchLength);
				blntbot:;
					oseq[sequenceIndex].y = cnt*userStitchLength - ((rit + 2) % seqtab[rcnt])*usesiz9;
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
				cnt = ceil(bseq[ind + 1].y / userStitchLength);
			toplab:;
				oseq[sequenceIndex].y = cnt*userStitchLength + (rit%seqtab[rcnt])*usesiz9;
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
				cnt = floor(bseq[ind + 1].y / userStitchLength);
			botlab:;
				oseq[sequenceIndex].y = cnt*userStitchLength - ((rit + 2) % seqtab[rcnt])*usesiz9;
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
					cnt = len / userStitchLength - 1;
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
	grindpnt = 0;
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
			tgrinds[grindpnt++] = stitchLineCount;
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
	tgrinds[grindpnt++] = stitchLineCount;
	grinds = new unsigned[grindpnt];
	for (ind = 0; ind < grindpnt; ind++)
		grinds[ind] = tgrinds[ind];
	lineGroupIndex--;
	delete[] jpts;
	delete[] pjpts;
}

void fnhor() {
	unsigned	ind;

	frmcpy(&angfrm, &formList[closestFormToCursor]);
	rotationCenter.x = (double)(angfrm.rectangle.right - angfrm.rectangle.left) / 2 + angfrm.rectangle.left;
	rotationCenter.y = (double)(angfrm.rectangle.top - angfrm.rectangle.bottom) / 2 + angfrm.rectangle.bottom;
	rotationAngle = PI / 2;
	angfrm.vertices = angflt;
	for (ind = 0; ind < angfrm.sides; ind++) {
		angfrm.vertices[ind].x = SelectedForm->vertices[ind].x;
		angfrm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&angfrm.vertices[ind]);
	}
	SelectedForm = &angfrm;
	fnvrt();
	SelectedForm = &formList[closestFormToCursor];
}

void fsvrt() {
	delmclp(closestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = VRTF;
	SelectedForm->type = POLI;
	SelectedForm->fillColor = activeColor;
	fsizpar();
	SelectedForm->fillSpacing = stitchSpace;
	SelectedForm->type = POLI;
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
			if (SelectedForm->type != LIN)
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
	SelectedForm->fillColor = activeColor;
	fsizpar();
	SelectedForm->fillSpacing = stitchSpace;
	SelectedForm->angleOrClipData.angle = (float)PI / 2;
	SelectedForm->type = POLI;
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
			if (SelectedForm->type != LIN)
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

	bang = rotationAngle;
	delmclp(closestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = ANGF;
	rotationAngle = PI / 2 - rotationAngle;
	SelectedForm->angleOrClipData.angle = (float)iniFile.fillAngle;
	SelectedForm->fillColor = activeColor;
	fsizpar();
	SelectedForm->fillSpacing = stitchSpace;
	SelectedForm->type = POLI;
	dusqr();
	refil();
	rotationAngle = bang;
}

void filangl() {
	unsigned ind;

	if (filmsgs(FMM_ANG))
		return;
	rotationAngle = iniFile.fillAngle;
	if (selectedFormCount) {
		savdo();
		for (ind = 0; ind < selectedFormCount; ind++) {
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (SelectedForm->type != LIN)
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
	pxrct->left = (strct.left - zoomRect.left)*zoomRatio.x + 0.5;
	pxrct->right = (strct.right - zoomRect.left)*zoomRatio.x + 0.5;
	pxrct->top = (stitchWindowClientRect.bottom) - (strct.top - zoomRect.bottom)*zoomRatio.y + 0.5;
	pxrct->bottom = (stitchWindowClientRect.bottom) - (strct.bottom - zoomRect.bottom)*zoomRatio.y + 0.5;
}

void setmfrm() {
	unsigned	ind;
	POINT		tpnt;
	POINT		tof;

	sfCor2px(formList[closestFormToCursor].vertices[0], &tpnt);
	tof.x = msg.pt.x - stitchWindowOrigin.x - tpnt.x + fmovdif.x;
	tof.y = msg.pt.y - stitchWindowOrigin.y - tpnt.y + fmovdif.y;
	for (ind = 0; ind < formList[closestFormToCursor].sides; ind++) {
		sfCor2px(formList[closestFormToCursor].vertices[ind], &tpnt);
		formLines[ind].x = tpnt.x + tof.x;
		formLines[ind].y = tpnt.y + tof.y;
	}
	formLines[ind].x = formLines[0].x;
	formLines[ind].y = formLines[0].y;
}

void strtchbox() {
	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	Polyline(stitchWindowDC, stretchBoxLine, 5);
	SetROP2(stitchWindowDC, R2_COPYPEN);
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
	tpnt0.x = msg.pt.x - stitchWindowOrigin.x;
	tpnt0.y = msg.pt.y - stitchWindowOrigin.y;
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
			ritfrct(closestFormToCursor, stitchWindowDC);
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
		fmovdif.x = tpnt1.x - tpnt0.x;
		fmovdif.y = tpnt1.y - tpnt0.y;
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
	tpnt.x = msg.pt.x + fmovdif.x;
	tpnt.y = msg.pt.y + fmovdif.y;
	pxCor2stch(tpnt);
	pof.x = selectedPoint.x - SelectedForm->vertices[0].x;
	pof.y = selectedPoint.y - SelectedForm->vertices[0].y;
	for (ind = 0; ind < SelectedForm->sides; ind++) {
		SelectedForm->vertices[ind].x += pof.x;
		SelectedForm->vertices[ind].y += pof.y;
	}
	SelectedForm->rectangle.bottom += pof.y;
	SelectedForm->rectangle.top += pof.y;
	SelectedForm->rectangle.left += pof.x;
	SelectedForm->rectangle.right += pof.x;
	for (ind = 0; ind < header.stitchCount; ind++) {
		if ((stitchBuffer[ind].attribute&FRMSK) == tat&&stitchBuffer[ind].attribute&ALTYPMSK && !(stitchBuffer[ind].attribute&NOTFRM)) {
			stitchBuffer[ind].x += pof.x;
			stitchBuffer[ind].y += pof.y;
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
	clpad = 0;
}

void dusat() {
	POINT* l_plin = &formLines[satinIndex - 1];

	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	Polyline(stitchWindowDC, l_plin, 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void unsat() {
	if (rstMap(SHOSAT))
		dusat();
}

void drwsat() {
	unsat();
	px2stch();
	formLines[satinIndex].x = msg.pt.x - stitchWindowOrigin.x;
	formLines[satinIndex].y = msg.pt.y - stitchWindowOrigin.y;
	setMap(SHOSAT);
	dusat();
}

void satpnt0() {
	px2stch();
	formLines[0].x = msg.pt.x - stitchWindowOrigin.x;
	formLines[0].y = msg.pt.y - stitchWindowOrigin.y;
	tempPolygon[0].x = selectedPoint.x;
	tempPolygon[0].y = selectedPoint.y;
	satinIndex = 1;
	setMap(SATPNT);
}

void satpnt1() {
	unsat();
	px2stch();
	formLines[satinIndex].x = msg.pt.x - stitchWindowOrigin.x;
	formLines[satinIndex].y = msg.pt.y - stitchWindowOrigin.y;
	dusat();
	tempPolygon[satinIndex].x = selectedPoint.x;
	tempPolygon[satinIndex].y = selectedPoint.y;
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
	loc = &SelectedForm->satinOrAngle.sac[p_cpnt] - satks;
	if (satkad > loc)
		MoveMemory(tp, &tp[1], (satkad - loc + 1) * sizeof(SATCON));
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
	{
		fp = &formList[ind];
		if (fp->type == SAT&&fp->satinGuideCount)
			fp->satinOrAngle.sac--;
	}
	if (closestVertexToCursor < wordParam)
		wordParam--;
	SelectedForm->satinGuideCount--;
	satkad--;
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
		if (!activeLayer || !playcod || playcod == activeLayer) {
			for (ine = 0; ine < formList[ind].sides; ine++) {
				dx = selectedPoint.x - formList[ind].vertices[ine].x;
				dy = selectedPoint.y - formList[ind].vertices[ine].y;
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
		if (SelectedForm->type == POLI)
			SelectedForm->type = SAT;
	}
}

void ducon() {
	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	Polyline(stitchWindowDC, formLines, 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void uncon() {
	if (rstMap(SHOCON))
		ducon();
}

void drwcon() {
	uncon();
	formLines[1].x = msg.pt.x - stitchWindowOrigin.x;
	formLines[1].y = msg.pt.y - stitchWindowOrigin.y;
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
		MoveMemory(l_dpnt, l_spnt, sizeof(SATCON)*(&satks[satkad] - l_spnt + 1));
		for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
		{
			fp = &formList[ind];
			if (fp->type == SAT)
				fp->satinOrAngle.sac -= ine;
		}
		satkad -= ine;
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
		dx = selectedPoint.x - SelectedForm->vertices[ind].x;
		dy = selectedPoint.y - SelectedForm->vertices[ind].y;
		tlen = hypot(dx, dy);
		if (tlen < len) {
			len = tlen;
			closestVertexToCursor = ind;
		}
	}
	rstMap(SATCNKT);
	if (SelectedForm->type == LIN) {
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
					selectedPoint.x = currentFormVertices[astrt].x;
					selectedPoint.y = currentFormVertices[astrt].y;
					oseq[sequenceIndex].x = selectedPoint.x;
					oseq[sequenceIndex++].y = selectedPoint.y;
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
					oseq[0].x = selectedPoint.x = currentFormVertices[1].x;
					oseq[0].y = selectedPoint.y = currentFormVertices[1].y;
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
		oseq[0].x = selectedPoint.x = currentFormVertices[0].x;
		oseq[0].y = selectedPoint.y = currentFormVertices[0].y;
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
	SelectedForm->fillColor = activeColor;
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
			if (SelectedForm->type != LIN)
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
		if (!activeLayer || (unsigned)((formList[ind].attribute&FRMLMSK) >> 1) == activeLayer || !(formList[ind].attribute&FRMLMSK)) {
			currentFormVertices = formList[ind].vertices;
			for (ine = 0; ine < formList[ind].sides; ine++) {
				dx = selectedPoint.x - currentFormVertices[ine].x;
				dy = selectedPoint.y - currentFormVertices[ine].y;
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
	fltspac(&finspnt->vertices[pnu], 1);
	finspnt->vertices[pnu].x = selectedPoint.x;
	finspnt->vertices[pnu].y = selectedPoint.y;
	finspnt->sides++;
	for (ind = 0; ind < finspnt->satinGuideCount; ind++) {
		if (finspnt->satinOrAngle.sac[ind].start > pnu - 1)
			finspnt->satinOrAngle.sac[ind].start++;
		if (finspnt->satinOrAngle.sac[ind].finish > pnu - 1)
			finspnt->satinOrAngle.sac[ind].finish++;
	}
	if (finspnt->wordParam >= pnu) {
		finspnt->wordParam++;
		finspnt->wordParam %= sides;
	}
	if (finspnt->fillType == CONTF) {
		if (finspnt->angleOrClipData.sat.start > pnu - 1)
			finspnt->angleOrClipData.sat.start++;
		if (finspnt->angleOrClipData.sat.finish > pnu - 1)
			finspnt->angleOrClipData.sat.finish++;
	}
	frmlin(finspnt->vertices, finspnt->sides);
}

double p2p(fPOINT pnt0, fPOINT pnt1) {
	return hypot(pnt0.x - pnt1.x, pnt0.y - pnt1.y);
}

unsigned upsat() {
	unsigned	ind;
	double		dwnlen, uplen, p2clos, p2up, p2dwn;

	sides = finspnt->sides;
	currentFormVertices = finspnt->vertices;
	p2clos = p2p(selectedPoint, currentFormVertices[closestVertexToCursor]);
	ind = prv(closestVertexToCursor);
	dwnlen = p2p(currentFormVertices[ind], currentFormVertices[closestVertexToCursor]);
	p2dwn = p2p(currentFormVertices[ind], selectedPoint);
	ind = nxt(closestVertexToCursor);
	uplen = p2p(currentFormVertices[ind], currentFormVertices[closestVertexToCursor]);
	p2up = p2p(currentFormVertices[ind], selectedPoint);
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
		finspnt = SelectedForm;
		fvars(closestFormToCursor);
		if (upsat()) {
			if (!closestVertexToCursor&&finspnt->type == LIN)
				setMap(PRELIN);
			else
				closestVertexToCursor = prv(closestVertexToCursor);
			nufpnt(closestVertexToCursor);
			if (rstMap(PRELIN)) {
				selectedPoint.x = finspnt->vertices[0].x;
				selectedPoint.y = finspnt->vertices[0].y;
				finspnt->vertices[0].x = finspnt->vertices[1].x;
				finspnt->vertices[0].y = finspnt->vertices[1].y;
				finspnt->vertices[1].x = selectedPoint.x;
				finspnt->vertices[1].y = selectedPoint.y;
			}
		}
		else
			nufpnt(closestVertexToCursor);
		refil();
	}
	setMap(RESTCH);
}

BOOL chkdel() {
	if (SelectedForm->type == LIN) {
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
				satkad--;
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
	MoveMemory(&SelectedForm->vertices[closestVertexToCursor], &SelectedForm->vertices[closestVertexToCursor + 1], (fltad - closestVertexToCursor) * sizeof(fPOINTATTRIBUTE));
	SelectedForm->sides--;
	fltad--;
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
	if (tflt->x<zoomRect.left || tflt->x>zoomRect.right || tflt->y<zoomRect.bottom || tflt->y>zoomRect.top)
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
			markedStitchMap[src] = 0;
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
		for (src = 0; src < header.stitchCount; src++) {
			if (!chkr((stitchBuffer[src].attribute&FRMSK) >> FRMSHFT)) {
				stitchBuffer[dst].attribute = stitchBuffer[src].attribute;
				stitchBuffer[dst].x = stitchBuffer[src].x;
				stitchBuffer[dst++].y = stitchBuffer[src].y;
			}
		}
		header.stitchCount = dst;
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			if (!rstMap(IGNOR) && !chku(WRNOF)) {
				trg = (closestFormToCursor << FRMSHFT) | USMSK;
				mlen = stitchBuffer[0].attribute&(FRMSK | USMSK);
				for (src = 0; src < header.stitchCount; src++) {
					at = stitchBuffer[src].attribute;
					if (!(at&NOTFRM) && (at&(USMSK | FRMSK)) == trg) {
						tabmsg(IDS_UNFIL);
						setMap(FILMSG);
						okcan();
						setMap(IGNOR);
						bakpnt.x = msg.pt.x;
						bakpnt.y = msg.pt.y;
						return;
					}
				}
			}
			trg = closestFormToCursor << FRMSHFT;
			dst = 0;
			for (src = 0; src < header.stitchCount; src++) {
				if ((stitchBuffer[src].attribute&FRMSK) != trg || (stitchBuffer[src].attribute&NOTFRM)) {
					stitchBuffer[dst].attribute = stitchBuffer[src].attribute;
					stitchBuffer[dst].x = stitchBuffer[src].x;
					stitchBuffer[dst++].y = stitchBuffer[src].y;
				}
			}
			delclps(closestFormToCursor);
			deltx();
			SelectedForm->fillType = 0;
			SelectedForm->edgeType = 0;
			SelectedForm->extendedAttribute &= !(AT_UND | AT_CWLK | AT_WALK);
			header.stitchCount = dst;
			ritot(header.stitchCount);
		}
	}
}

void satzum() {
	rstMap(SHOSAT);
	duzrat();
	sides = satinIndex;
	frmlin(tempPolygon, satinIndex);
	SetROP2(stitchWindowMemDC, R2_XORPEN);
	SelectObject(stitchWindowMemDC, formPen);
	Polyline(stitchWindowMemDC, formLines, satinIndex);
	SetROP2(stitchWindowMemDC, R2_COPYPEN);
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
	SetROP2(stitchWindowDC, R2_XORPEN);
	SelectObject(stitchWindowDC, formPen);
	Polyline(stitchWindowDC, insertLine, 2);
	SetROP2(stitchWindowDC, R2_COPYPEN);
}

void uninsf() {
	if (rstMap(SHOINSF))
		duinsf();
}

void rinfrm() {
	frmlin(finspnt->vertices, finspnt->sides);
	SelectObject(stitchWindowMemDC, formPen);
	SetROP2(stitchWindowMemDC, R2_XORPEN);
	if (fgpnt1 || finspnt->type != LIN)
		Polyline(stitchWindowMemDC, &formLines[fgpnt0], 2);
	insertLine[0].x = formLines[fgpnt0].x;
	insertLine[0].y = formLines[fgpnt0].y;
	insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
	insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
	setMap(SHOINSF);
	duinsf();
}

void infrm() {
	if (closat()) {
		finspnt = &formList[closestFormToCursor];
		fvars(closestFormToCursor);
		if (upsat()) {
			if (!closestVertexToCursor&&finspnt->type == LIN) {
				fgpnt0 = 0;
				setMap(PRELIN);
			}
			else {
				fgpnt0 = prv(closestVertexToCursor);
				fgpnt1 = closestVertexToCursor;
			}
		}
		else {
			fgpnt0 = closestVertexToCursor;
			fgpnt1 = nxt(closestVertexToCursor);
		}
		setMap(INSFRM);
		setMap(INIT);
		rinfrm();
	}
}

void setins() {
	px2stch();
	nufpnt(fgpnt0);
	if (chkMap(PRELIN)) {
		selectedPoint.x = finspnt->vertices[0].x;
		selectedPoint.y = finspnt->vertices[0].y;
		finspnt->vertices[0].x = finspnt->vertices[1].x;
		finspnt->vertices[0].y = finspnt->vertices[1].y;
		finspnt->vertices[1].x = selectedPoint.x;
		finspnt->vertices[1].y = selectedPoint.y;
	}
	else {
		sides = finspnt->sides;
		fgpnt0 = nxt(fgpnt0);
		fgpnt1 = nxt(fgpnt0);
	}
	frmlin(finspnt->vertices, finspnt->sides);
	insertLine[0].x = formLines[fgpnt0].x;
	insertLine[0].y = formLines[fgpnt0].y;
	insertLine[1].x = msg.pt.x - stitchWindowOrigin.x;
	insertLine[1].y = msg.pt.y - stitchWindowOrigin.y;
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
	if (SelectedForm->type != LIN) {
		nlin = nxt(tlin);
		bdrlin(tlin, nlin, pd_Size);
	}
}

void bsizpar() {
	SelectedForm->maxBorderStitchLen = iniFile.maxStitchLength;
	SelectedForm->edgeStitchLen = userStitchLength;
	SelectedForm->minBorderStitchLen = minStitchLength;
}

void sbord() {
	fvars(closestFormToCursor);
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EGLIN;
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
			SelectedForm->borderColor = activeColor;
			sbord();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			SelectedForm->borderColor = activeColor;
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

	if (chkmax(clipboardStitchCount, sequenceIndex))
		return 1;
	adj.x = pnt.x - clipReference.x;
	adj.y = pnt.y - clipReference.y;
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		oseq[sequenceIndex].x = clipFillData[ind].x + adj.x;
		oseq[sequenceIndex++].y = clipFillData[ind].y + adj.y;
	}
	return 0;
}

BOOL clpsid(unsigned strt, unsigned fin) {
	unsigned		ind, cnt;
	fPOINT			dif, stp, pnt;
	double			len, tdub;
	fPOINTATTRIBUTE			rpnt;

	rpnt.x = clipboardRect.left;
	rpnt.y = clipboardRect.bottom;
	pnt.x = currentFormVertices[strt].x;
	pnt.y = currentFormVertices[fin].y;
	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	len = hypot(dif.x, dif.y);
	rotationAngle = atan2(dif.y, dif.x);
	rotang1(rpnt, &clipReference);
	cnt = len / clipboardRectSize.cx;
	if (cnt) {
		if (cnt > 1)
			tdub = ((len - cnt*clipboardRectSize.cx) / (cnt - 1) + clipboardRectSize.cx) / len;
		else
			tdub = (len - clipboardRectSize.cx) / 2;
		stp.x = dif.x*tdub;
		stp.y = dif.y*tdub;
		pnt.x = currentFormVertices[strt].x;
		pnt.y = currentFormVertices[strt].y;
		rotationAngle = atan2(dif.y, dif.x);
		for (ind = 0; ind < clipboardStitchCount; ind++)
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

	dif.x = currentFormVertices[clpstrt + 1].x - selectedPoint.x;
	dif.y = currentFormVertices[clpstrt + 1].y - selectedPoint.y;
	len = hypot(dif.x, dif.y);
	cnt = len / clipboardRectSize.cx;
	if (cnt) {
		rotationAngle = clpang;
		rotangf(rclpref, &clipReference);
		for (ind = 0; ind < clipboardStitchCount; ind++)
			rotangf(clipReversedData[ind], &clipFillData[ind]);
		for (ind = 0; ind < cnt; ind++) {
			ritclp(selectedPoint);
			selectedPoint.x += vct0.x;
			selectedPoint.y += vct0.y;
		}
	}
}

BOOL nupnt() {
	double		len, dif;
	unsigned	ind;

	mvpnt.x = currentFormVertices[clpstrt + 2].x;
	mvpnt.y = currentFormVertices[clpstrt + 2].y;
	len = hypot(mvpnt.x - selectedPoint.x, mvpnt.y - selectedPoint.y);
	if (len > clipboardRectSize.cx) {
		for (ind = 0; ind < 10; ind++) {
			len = hypot(mvpnt.x - selectedPoint.x, mvpnt.y - selectedPoint.y);
			dif = clipboardRectSize.cx - len;
			mvpnt.x += dif*cosang;
			mvpnt.y += dif*sinang;
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

	sinang = sin(clpang);
	cosang = cos(clpang);
	if (nupnt()) {
		dif.x = mvpnt.x - selectedPoint.x;
		dif.y = mvpnt.y - selectedPoint.y;
		rotationAngle = atan2(dif.y, dif.x);
		rotangf(rclpref, &clipReference);
		for (ind = 0; ind < clipboardStitchCount; ind++)
			rotangf(clipReversedData[ind], &clipFillData[ind]);
		ritclp(selectedPoint);
		selectedPoint.x = mvpnt.x;
		selectedPoint.y = mvpnt.y;
	}
}

void durev() {
	unsigned	ind;
	double		mid;

	mid = (clipboardRect.right - clipboardRect.left) / 2 + clipboardRect.left;
	if (clipBuffer[0].x > mid) {
		for (ind = 0; ind < clipboardStitchCount; ind++) {
			clipReversedData[ind].x = clipboardRect.right - clipBuffer[ind].x;
			clipReversedData[ind].y = clipBuffer[ind].y;
		}
	}
	else {
		for (ind = 0; ind < clipboardStitchCount; ind++) {
			clipReversedData[ind].x = clipBuffer[ind].x;
			clipReversedData[ind].y = clipBuffer[ind].y;
		}
	}
}

void setvct(unsigned strt, unsigned fin) {
	clpang = atan2(currentFormVertices[fin].y - currentFormVertices[strt].y, currentFormVertices[fin].x - currentFormVertices[strt].x);
	vct0.x = clipboardRectSize.cx*cos(clpang);
	vct0.y = clipboardRectSize.cx*sin(clpang);
}

void clpbrd(unsigned short strtlin) {
	unsigned		ind, ref;
	unsigned short	nlin;

	sequenceIndex = 0;
	rstMap(CLPBAK);
	plen = clipboardRectSize.cx / 2;
	plen2 = clipboardRectSize.cx;
	clipFillData = new fPOINT[clipboardStitchCount];
	clipReversedData = new fPOINT[clipboardStitchCount];
	rotationCenter.x = (clipboardRect.right - clipboardRect.left) / 2 + clipboardRect.left;
	clipReference.y = rotationCenter.y = (clipboardRect.top - clipboardRect.bottom) / 2 + clipboardRect.bottom;
	clipReference.x = clipboardRect.left;
	durev();
	ref = 0;
	if (SelectedForm->type == LIN) {
		selectedPoint.x = currentFormVertices[0].x;
		selectedPoint.y = currentFormVertices[0].y;
		setvct(0, 1);
		rclpref.y = (clipboardRect.top - clipboardRect.bottom) / 2 + clipboardRect.bottom;
		rclpref.x = (clipboardRect.top = clipboardRect.left) / 2 + clipboardRect.left;
		for (clpstrt = 0; clpstrt < (unsigned)sides - 2; clpstrt++) {
			linsid();
			setvct(clpstrt + 1, clpstrt + 2);
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
	double		xof, yof;

	if (fabs(formAngles[strt]) < TINY&&fabs(formAngles[fin]) < TINY) {
		xof = 0;
		yof = satwid;
	}
	else {
#define SATHRESH 10

		l_ang = (formAngles[fin] - formAngles[strt]) / 2;
		len = satwid / cos(l_ang);
		if (len < -satwid*SATHRESH)
			len = -satwid*SATHRESH;
		if (len > satwid*SATHRESH)
			len = satwid*SATHRESH;
		l_ang += formAngles[strt] + PI / 2;
		xof = len*cos(l_ang);
		yof = len*sin(l_ang);
	}
	ipnts[fin].x = currentFormVertices[fin].x - xof;
	ipnts[fin].y = currentFormVertices[fin].y - yof;
	opnts[fin].x = currentFormVertices[fin].x + xof;
	opnts[fin].y = currentFormVertices[fin].y + yof;
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
		opnts = opntlst;
		ipnts = ipntlst;
		for (ind = 0; ind < (unsigned)sides - 1; ind++)
			outfn(ind, ind + 1, 0.1);
		cnt = 0;
		for (ind = 0; ind < sides; ind++)
		{
			if (cisin(ipnts[ind].x, ipnts[ind].y))
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
			opnts = ipntlst;
			ipnts = opntlst;
		}
	}
}

void clpout() {
	if (SelectedForm->type == LIN)
		satout(plen);
	else {
		satout(clipboardRectSize.cy);
		ipnts = SelectedForm->vertices;
	}
}

void fsclp() {
	unsigned	ind;

	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EGCLP;
	SelectedForm->clipEntries = clipboardStitchCount;
	SelectedForm->borderClipData = nueclp(closestFormToCursor, clipboardStitchCount);
	SelectedForm->borderSize = clipboardRectSize.cy;
	SelectedForm->edgeSpacing = clipboardRectSize.cx;
	SelectedForm->borderColor = activeColor;
	bsizpar();
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = clipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = clipBuffer[ind].y;
	}
	plen = clipboardRectSize.cy / 2;
	clpout();
	refilfn();
}

void fclp() {
	unsigned ind;

	if (filmsgs(FML_CLP))
		return;
	if (OpenClipboard(hWnd)) {
		fvars(closestFormToCursor);
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {
			savdo();
			redclp();
			CloseClipboard();
			if (clipboardRectSize.cx > CLPMIN) {
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

	dif.x = pnt.x - selectedPoint.x;
	dif.y = pnt.y - selectedPoint.y;
	len = hypot(dif.x, dif.y);
	if (len > MAXSTCH) {
		cnt = len / MAXSTCH + 1;
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		cnt--;
		if (chkmax(cnt, sequenceIndex))
			return;
		while (cnt) {
			selectedPoint.x += stp.x;
			selectedPoint.y += stp.y;
			oseq[sequenceIndex].x = selectedPoint.x;
			oseq[sequenceIndex++].y = selectedPoint.y;
			cnt--;
		}
	}
	if (sequenceIndex & 0xffff0000)
		return;
	oseq[sequenceIndex].x = pnt.x;
	oseq[sequenceIndex++].y = pnt.y;
	selectedPoint.x = pnt.x;
	selectedPoint.y = pnt.y;
}

BOOL chkbak(dPOINT pnt) {
	unsigned ind;
	double		len;

	for (ind = 0; ind < 8; ind++) {
		len = hypot(filbak[ind].x - pnt.x, filbak[ind].y - pnt.y);
		if (len < stitchSpace)
			return 1;
	}
	return 0;
}

BOOL linx(fPOINT* p_flt, unsigned strt, unsigned fin, dPOINT* npnt) {
	dPOINT	dif;
	dPOINT	tdub;

	dif.x = opnts[strt].x - p_flt[strt].x;
	dif.y = opnts[strt].y - p_flt[strt].y;
	if (!dif.x && !dif.y)
		return 0;
	tdub.x = p_flt[strt].x;
	tdub.y = p_flt[strt].y;
	if (dif.x) {
		if (proj(tdub, dif.y / dif.x, opnts[fin], p_flt[fin], npnt))
			return 1;
		else
			return 0;
	}
	else {
		if (projv(tdub.x, p_flt[fin], opnts[fin], npnt))
			return 1;
		else
			return 0;
	}
}

void filinsbw(dPOINT pnt) {
	filbak[pfbak].x = pnt.x;
	filbak[pfbak++].y = pnt.y;
	pfbak &= 0x7;
	filinsb(pnt);
}

void sbfn(fPOINT* p_flt, unsigned strt, unsigned fin) {
	dPOINT		idif, odif, istp, ostp, l_opnt, ipnt;
	dPOINT		bdif, bstp, bpnt;
	dPOINT		npnt;
	double		ilen, olen, blen;
	unsigned	cnt, iflg, oflg, bcnt, ind, xflg;

	if (!setMap(SAT1)) {
		selectedPoint.x = p_flt[strt].x;
		selectedPoint.y = p_flt[strt].y;
	}
	idif.x = p_flt[fin].x - p_flt[strt].x;
	idif.y = p_flt[fin].y - p_flt[strt].y;
	odif.x = opnts[fin].x - opnts[strt].x;
	odif.y = opnts[fin].y - opnts[strt].y;
	ilen = hypot(idif.x, idif.y);
	olen = hypot(odif.x, odif.y);
	ipnt.x = p_flt[strt].x;
	ipnt.y = p_flt[strt].y;
	l_opnt.x = opnts[strt].x;
	l_opnt.y = opnts[strt].y;
	xflg = pfbak = iflg = oflg = bcnt = 0;
	for (ind = 0; ind < 8; ind++) {
		filbak[ind].x = (float)1e12;
		filbak[ind].y = (float)1e12;
	}
	if (olen > ilen) {
		cnt = olen / stitchSpace;
		iflg = 1;
		if (linx(p_flt, strt, fin, &npnt)) {
			xflg = 1;
			idif.x = idif.y = ilen = 0;
			ipnt.x = npnt.x;
			ipnt.y = npnt.y;
		}
	}
	else {
		cnt = ilen / stitchSpace;
		oflg = 1;
		if (linx(p_flt, strt, fin, &npnt)) {
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
				bdif.x = ipnt.x - selectedPoint.x;
				bdif.y = ipnt.y - selectedPoint.y;
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
				bdif.x = l_opnt.x - selectedPoint.x;
				bdif.y = l_opnt.y - selectedPoint.y;
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

void sfn(unsigned short strtlin) {
	unsigned ind;
	unsigned short nlin;

	for (ind = 0; ind < SelectedForm->sides; ind++) {
		nlin = nxt(strtlin);
		sbfn(ipnts, strtlin, nlin);
		strtlin = nlin;
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
		sbfn(ipnts, tlin, nlin);
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
	fPOINT		stp;

	if (blnt&SBLNT) {
		stp.x = sin(formAngles[0])*plen / 2;
		stp.y = cos(formAngles[0])*plen / 2;
		if (chkMap(INDIR))
		{
			stp.x = -stp.x;
			stp.y = -stp.y;
		}
		ipnts[0].x = SelectedForm->vertices[0].x + stp.x;
		ipnts[0].y = SelectedForm->vertices[0].y - stp.y;
		opnts[0].x = SelectedForm->vertices[0].x - stp.x;
		opnts[0].y = SelectedForm->vertices[0].y + stp.y;
	}
	else {
		ipnts[0].x = opnts[0].x = currentFormVertices[0].x;
		ipnts[0].y = opnts[0].y = currentFormVertices[0].y;
	}
	if (blnt&FBLNT) {
		stp.x = sin(formAngles[sides - 2])*plen / 2;
		stp.y = cos(formAngles[sides - 2])*plen / 2;
		if (chkMap(INDIR))
		{
			stp.x = -stp.x;
			stp.y = -stp.y;
		}
		ipnts[sides - 1].x = SelectedForm->vertices[sides - 1].x + stp.x;
		ipnts[sides - 1].y = SelectedForm->vertices[sides - 1].y - stp.y;
		opnts[sides - 1].x = SelectedForm->vertices[sides - 1].x - stp.x;
		opnts[sides - 1].y = SelectedForm->vertices[sides - 1].y + stp.y;
	}
	else {
		ipnts[sides - 1].x = opnts[sides - 1].x = currentFormVertices[sides - 1].x;
		ipnts[sides - 1].y = opnts[sides - 1].y = currentFormVertices[sides - 1].y;
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
			sbfn(ipnts, ind, ind + 1);
		toglMap(FILDIR);
		for (ind = SelectedForm->sides - 1; ind; ind--)
			sbfn(ipnts, ind, ind - 1);
	}
	plen = SelectedForm->borderSize;
	satout(plen);
	satends(SelectedForm->attribute);
	stitchSpace = SelectedForm->edgeSpacing;
	rstMap(SAT1);
	for (ind = 0; ind < (unsigned)SelectedForm->sides - 1; ind++)
		sbfn(ipnts, ind, ind + 1);
	stitchSpace = tspac;
}

void satsbrd() {
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EGSAT;
	if (chku(DUND))
		SelectedForm->edgeType |= EGUND;
	bsizpar();
	SelectedForm->borderSize = borderWidth;
	SelectedForm->edgeSpacing = stitchSpace / 2;
	SelectedForm->borderColor = activeColor;
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
			ritot(header.stitchCount);
			setMap(RESTCH);
		}
	}
}

void lapbrd() {
	double		tsiz;
	unsigned	ind;

	sequenceIndex = 0;
	tsiz = userStitchLength;
	userStitchLength = APSPAC;
	for (ind = 0; ind < (unsigned)sides - 1; ind++)
		bdrlin(ind, ind + 1, APSPAC);
	for (ind = sides - 1; ind; ind--)
		bdrlin(ind, ind - 1, APSPAC);
	userStitchLength = tsiz;
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
	SelectedForm->edgeType = EGAP;
	if (chku(DUND))
		SelectedForm->edgeType |= EGUND;
	SelectedForm->edgeSpacing = stitchSpace / 2;
	SelectedForm->borderSize = iniFile.borderWidth;
	bsizpar();
	SelectedForm->borderColor = activeColor | (underlayColor << 4);
	if (SelectedForm->type != LIN)
	{
		if (SelectedForm->fillType == SAT&&SelectedForm->satinGuideCount)
			delsac(closestFormToCursor);
	}
	SelectedForm->fillType = 0;
	SelectedForm->type = POLI;
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

	underlayColor = activeColor;
	LoadString(hInst, IDS_APCOL, buf, HBUFSIZ);
	sprintf_s(msgbuf, sizeof(msgbuf), buf, underlayColor + 1);
	shoMsg(msgbuf);
}

void maxtsiz(TCHAR* str, POINT* pt) {
	SIZE	tsiz;

	GetTextExtentPoint32(stitchWindowMemDC, str, strlen(str), &tsiz);
	pt->y = tsiz.cy;
	if (tsiz.cx > pt->x)
		pt->x = tsiz.cx;
}

void maxwid(unsigned strt, unsigned fin) {
	POINT	pnt;

	pnt.x = 0;
	pnt.y = 0;
	while (strt <= fin)
		maxtsiz(stab[strt++], &pnt);
	prfsiz = pnt.x + 6;
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
		hFormData,
		NULL,
		hInst,
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
		hFormData,
		NULL,
		hInst,
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
		hFormData,
		NULL,
		hInst,
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
	if (cod >= EGLAST) {
		cod = EGLAST - 1;
	}
	fpnt = cod - 1;
	leftWindowCoords.top = rightWindowCoords.top = 3;
	leftWindowCoords.bottom = rightWindowCoords.bottom = 3 + leftWindowSize.y;
	leftWindowCoords.left = 3;
	leftWindowCoords.right = 3 + leftWindowSize.x;
	rightWindowCoords.left = 6 + leftWindowSize.x;
	rightWindowCoords.right = 6 + leftWindowSize.x + rightWindowSize.x + 6;
	thTxt[LFRM] = txtwin(stab[STR_TXT0], leftWindowCoords);
	if (SelectedForm->type == LIN)
		thDat[LFRM] = txtrwin(stab[STR_EDG1], rightWindowCoords);
	else
		thDat[LFRM] = txtrwin(stab[STR_FREH], rightWindowCoords);
	nxtlin();
	thTxt[LLAYR] = txtwin(stab[STR_TXT1], leftWindowCoords);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", (SelectedForm->attribute&FRMLMSK) >> 1);
	thDat[LLAYR] = txtrwin(msgbuf, rightWindowCoords);
	nxtlin();
	if (SelectedForm->type != LIN)
	{
		thTxt[LCWLK] = txtwin(stab[STR_CWLK], leftWindowCoords);
		if (SelectedForm->extendedAttribute&AT_CWLK)
			thDat[LCWLK] = txtrwin(stab[STR_ON], rightWindowCoords);
		else
			thDat[LCWLK] = txtrwin(stab[STR_OFF], rightWindowCoords);
		nxtlin();
		thTxt[LWALK] = txtwin(stab[STR_WALK], leftWindowCoords);
		if (SelectedForm->extendedAttribute&AT_WALK)
			thDat[LWALK] = txtrwin(stab[STR_ON], rightWindowCoords);
		else
			thDat[LWALK] = txtrwin(stab[STR_OFF], rightWindowCoords);
		nxtlin();
		thTxt[LUND] = txtwin(stab[STR_UND], leftWindowCoords);
		if (SelectedForm->extendedAttribute&AT_UND)
			thDat[LUND] = txtrwin(stab[STR_ON], rightWindowCoords);
		else
			thDat[LUND] = txtrwin(stab[STR_OFF], rightWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&(AT_WALK | AT_UND | AT_CWLK))
		{
			thTxt[LUNDCOL] = txtwin(stab[STR_UNDCOL], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->underlayColor + 1);
			thDat[LUNDCOL] = txtrwin(msgbuf, rightWindowCoords);
			nxtlin();
			thTxt[LULEN] = txtwin(stab[STR_ULEN], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->underlayStitchLen / PFGRAN);
			thDat[LULEN] = txtrwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		thTxt[LWLKIND] = txtwin(stab[STR_UWLKIND], leftWindowCoords);
		sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->underlayIndent / PFGRAN);
		thDat[LWLKIND] = txtrwin(msgbuf, rightWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&AT_UND)
		{
			thTxt[LUSPAC] = txtwin(stab[STR_FUSPAC], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->underlaySpacing / PFGRAN);
			thDat[LUSPAC] = txtrwin(msgbuf, rightWindowCoords);
			nxtlin();
			thTxt[LUANG] = txtwin(stab[STR_FUANG], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->underlayStitchAngle * 180 / PI);
			thDat[LUANG] = txtrwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
	}
	thTxt[LFRMFIL] = txtwin(stab[STR_TXT2], leftWindowCoords);
	thDat[LFRMFIL] = txtrwin(stab[STR_FIL0 + SelectedForm->fillType], rightWindowCoords);
	nxtlin();
	if (SelectedForm->fillType) {
		thTxt[LFRMCOL] = txtwin(stab[STR_TXT3], leftWindowCoords);
		sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->fillColor + 1);
		thDat[LFRMCOL] = numwin(msgbuf, rightWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == FTHF) {
			thTxt[LFTHCOL] = txtwin(stab[STR_FTHCOL], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->fillInfo.feather.color + 1);
			thDat[LFTHCOL] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
			thTxt[LFTHTYP] = txtwin(stab[STR_FTHTYP], leftWindowCoords);
			thDat[LFTHTYP] = numwin(stab[STR_FTH0 + SelectedForm->fillInfo.feather.fillType - 1], rightWindowCoords);
			nxtlin();
			thTxt[LFTHBLND] = txtwin(stab[STR_FTHBLND], leftWindowCoords);
			if (SelectedForm->extendedAttribute&AT_FTHBLND)
				pchr = stab[STR_ON];
			else
				pchr = stab[STR_OFF];
			thDat[LFTHBLND] = txtrwin(pchr, rightWindowCoords);
			nxtlin();
			if (!(SelectedForm->extendedAttribute&AT_FTHBLND)) {
				thTxt[LFTHDWN] = txtwin(stab[STR_FTHDWN], leftWindowCoords);
				if (SelectedForm->extendedAttribute&(AT_FTHDWN))
					pchr = stab[STR_ON];
				else
					pchr = stab[STR_OFF];
				thDat[LFTHDWN] = txtrwin(pchr, rightWindowCoords);
				nxtlin();
				if (!(SelectedForm->extendedAttribute&AT_FTHDWN)) {
					thTxt[LFTHUP] = txtwin(stab[STR_FTHUP], leftWindowCoords);
					if (SelectedForm->extendedAttribute&AT_FTHUP)
						pchr = stab[STR_ON];
					else
						pchr = stab[STR_OFF];
					thDat[LFTHUP] = txtrwin(pchr, rightWindowCoords);
					nxtlin();
				}
			}
			thTxt[LFTHUPCNT] = txtwin(stab[STR_FTHUPCNT], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->fillInfo.feather.upCount);
			thDat[LFTHUPCNT] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
			thTxt[LFTHDWNCNT] = txtwin(stab[STR_FTHDWNCNT], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->fillInfo.feather.downCount);
			thDat[LFTHDWNCNT] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
			thTxt[LFTHSIZ] = txtwin(stab[STR_FTHSIZ], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->fillInfo.feather.ratio);
			thDat[LFTHSIZ] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
			if (SelectedForm->fillInfo.feather.fillType == FTHPSG) {
				thTxt[LFTHNUM] = txtwin(stab[STR_FTHNUM], leftWindowCoords);
				sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->fillInfo.feather.count);
				thDat[LFTHNUM] = numwin(msgbuf, rightWindowCoords);
				nxtlin();
			}
			thTxt[LFTHFLR] = txtwin(stab[STR_FTHFLR], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->fillInfo.feather.minStitchSize / PFGRAN);
			thDat[LFTHFLR] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType != CLPF) {
			thTxt[LFRMSPAC] = txtwin(stab[STR_TXT4], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->fillSpacing / PFGRAN);
			thDat[LFRMSPAC] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (istx(closestFormToCursor))
		{
			thTxt[LTXOF] = txtwin(stab[STR_TXOF], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->txof / PFGRAN);
			thDat[LTXOF] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		thTxt[LMAXFIL] = txtwin(stab[STR_TXT20], leftWindowCoords);
		sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->maxFillStitchLen / PFGRAN);
		thDat[LMAXFIL] = numwin(msgbuf, rightWindowCoords);
		nxtlin();
		if (!isclp(closestFormToCursor) && !istx(closestFormToCursor)) {
			thTxt[LFRMLEN] = txtwin(stab[STR_TXT5], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->lengthOrCount.stitchLength / PFGRAN);
			thDat[LFRMLEN] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		thTxt[LMINFIL] = txtwin(stab[STR_TXT21], leftWindowCoords);
		sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->minFillStitchLen / PFGRAN);
		thDat[LMINFIL] = numwin(msgbuf, rightWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == ANGF || SelectedForm->fillType == TXANGF) {
			thTxt[LFRMANG] = txtwin(stab[STR_TXT6], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->angleOrClipData.angle * 180 / PI);
			thDat[LFRMANG] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == ANGCLPF) {
			thTxt[LSACANG] = txtwin(stab[STR_TXT6], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->satinOrAngle.angle * 180 / PI);
			thDat[LSACANG] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VCLPF || SelectedForm->fillType == HCLPF || SelectedForm->fillType == ANGCLPF) {
			thTxt[LFRMFAZ] = txtwin(stab[STR_TXT18], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->wordParam);
			thDat[LFRMFAZ] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VRTF || SelectedForm->fillType == HORF || SelectedForm->fillType == ANGF || istx(closestFormToCursor)) {
			thTxt[LBFILSQR] = txtwin(stab[STR_PRF2], leftWindowCoords);
			if (SelectedForm->extendedAttribute&AT_SQR)
				strcpy_s(msgbuf, stab[STR_SQR]);
			else
				strcpy_s(msgbuf, stab[STR_PNTD]);
			thDat[LBFILSQR] = txtrwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
	}
	thTxt[LFSTRT] = txtwin(stab[STR_FSTRT], leftWindowCoords);
	if (SelectedForm->extendedAttribute&AT_STRT)
		strcpy_s(msgbuf, stab[STR_ON]);
	else
		strcpy_s(msgbuf, stab[STR_OFF]);
	thDat[LFSTRT] = txtrwin(msgbuf, rightWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_STRT)
	{
		thTxt[LDSTRT] = txtwin(stab[STR_FSTRT], leftWindowCoords);
		sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->fillStart);
		thDat[LDSTRT] = numwin(msgbuf, rightWindowCoords);
		nxtlin();
	}
	thTxt[LFEND] = txtwin(stab[STR_FEND], leftWindowCoords);
	if (SelectedForm->extendedAttribute&AT_END)
		strcpy_s(msgbuf, stab[STR_ON]);
	else
		strcpy_s(msgbuf, stab[STR_OFF]);
	thDat[LFEND] = txtrwin(msgbuf, rightWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_END)
	{
		thTxt[LDEND] = txtwin(stab[STR_FEND], leftWindowCoords);
		sprintf_s(msgbuf, sizeof(msgbuf), "%d", SelectedForm->fillEnd);
		thDat[LDEND] = numwin(msgbuf, rightWindowCoords);
		nxtlin();
	}
	thTxt[LBRD] = txtwin(stab[STR_TXT7], leftWindowCoords);
	thDat[LBRD] = txtrwin(stab[STR_EDG0 + cod], rightWindowCoords);
	nxtlin();
	if (cod) {
		thTxt[LBRDCOL] = txtwin(stab[STR_TXT8], leftWindowCoords);
		sprintf_s(msgbuf, sizeof(msgbuf), "%d", (SelectedForm->borderColor & 0xf) + 1);
		thDat[LBRDCOL] = numwin(msgbuf, rightWindowCoords);
		nxtlin();
		if (egaray[fpnt] & BESPAC) {
			thTxt[LBRDSPAC] = txtwin(stab[STR_TXT9], leftWindowCoords);
			if (cod == EGPRP || cod == EGCHNH || cod == EGCHNL)
				sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->edgeSpacing / PFGRAN);
			else
				sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->edgeSpacing / PFGRAN * 2);
			thDat[LBRDSPAC] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BPICSPAC) {
			thTxt[LBRDPIC] = txtwin(stab[STR_TXT16], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->edgeSpacing / PFGRAN);
			thDat[LBRDPIC] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BEMAX) {
			thTxt[LMAXBRD] = txtwin(stab[STR_TXT22], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->maxBorderStitchLen / PFGRAN);
			thDat[LMAXBRD] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BELEN) {
			thTxt[LBRDLEN] = txtwin(stab[STR_TXT10], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->edgeStitchLen / PFGRAN);
			thDat[LBRDLEN] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BEMIN) {
			thTxt[LMINBRD] = txtwin(stab[STR_TXT23], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->minBorderStitchLen / PFGRAN);
			thDat[LMINBRD] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BESIZ) {
			thTxt[LBRDSIZ] = txtwin(stab[STR_TXT11], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->borderSize / PFGRAN);
			thDat[LBRDSIZ] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BRDPOS) {
			thTxt[LBRDPOS] = txtwin(stab[STR_TXT18], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->edgeStitchLen);
			thDat[LBRDPOS] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & CHNPOS) {
			thTxt[LBRDPOS] = txtwin(stab[STR_TXT19], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", SelectedForm->edgeStitchLen);
			thDat[LBRDPOS] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (cod == EGAP) {
			thTxt[LAPCOL] = txtwin(stab[STR_TXT12], leftWindowCoords);
			sprintf_s(msgbuf, sizeof(msgbuf), "%d", (SelectedForm->borderColor >> 4) + 1);
			thDat[LAPCOL] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (cod == EGSAT || cod == EGAP || cod == EGPRP) {
			thTxt[LBRDUND] = txtwin(stab[STR_TXT17], leftWindowCoords);
			if (SelectedForm->edgeType&EGUND)
				thDat[LBRDUND] = numwin(stab[STR_ON], rightWindowCoords);
			else
				thDat[LBRDUND] = numwin(stab[STR_OFF], rightWindowCoords);
			nxtlin();
		}
		if (egaray[fpnt] & BCNRSIZ) {
			if (SelectedForm->edgeType == EGHOL)
				sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", getblen() / PFGRAN);
			else
				sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", getplen() / PFGRAN);
			thTxt[LBCSIZ] = txtwin(stab[STR_TXT13], leftWindowCoords);
			thDat[LBCSIZ] = numwin(msgbuf, rightWindowCoords);
			nxtlin();
		}
		if (SelectedForm->type == LIN&&egaray[fpnt] & BRDEND) {
			thTxt[LBSTRT] = txtwin(stab[STR_TXT14], leftWindowCoords);
			if (SelectedForm->attribute&SBLNT)
				thDat[LBSTRT] = numwin(stab[STR_BLUNT], rightWindowCoords);
			else
				thDat[LBSTRT] = numwin(stab[STR_TAPR], rightWindowCoords);
			nxtlin();
			thTxt[LBFIN] = txtwin(stab[STR_TXT15], leftWindowCoords);
			if (SelectedForm->attribute&FBLNT)
				thDat[LBFIN] = numwin(stab[STR_BLUNT], rightWindowCoords);
			else
				thDat[LBFIN] = numwin(stab[STR_TAPR], rightWindowCoords);
			nxtlin();
		}
	}
}

void refrm() {
	SelectedForm = &formList[closestFormToCursor];
	if (rstMap(PRFACT)) {
		DestroyWindow(hPreferencesWindow);
		rstMap(WASRT);
	}
	leftWindowSize.x = leftWindowSize.y = rightWindowSize.x = rightWindowSize.y = 0;
	setMap(REFCNT);
	formMenuEntryCount = 0;
	refrmfn();
	if (hFormData) {
		while (EnumChildWindows(hFormData, chenum, 0));
		MoveWindow(hFormData, buttonWidthX3 + 3, 3, leftWindowSize.x + rightWindowSize.x + 18, leftWindowSize.y*formMenuEntryCount + 12, TRUE);
		redraw(hFormData);
	}
	else {
		hFormData = CreateWindow(
			"STATIC",
			0,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonWidthX3 + 3,
			3,
			leftWindowSize.x + rightWindowSize.x + 18,
			leftWindowSize.y*formMenuEntryCount + 12,
			hWnd,
			NULL,
			hInst,
			NULL);
	}
	rstMap(REFCNT);
	refrmfn();
}

void pxrct2stch(RECT pxr, fRECTANGLE* str) {
	POINT	tpnt;

	tpnt.x = pxr.left + stitchWindowOrigin.x;
	tpnt.y = pxr.top + stitchWindowOrigin.y;
	pxCor2stch(tpnt);
	str->left = selectedPoint.x;
	str->top = selectedPoint.y;
	tpnt.x = pxr.right + stitchWindowOrigin.x;
	tpnt.y = pxr.bottom + stitchWindowOrigin.y;
	pxCor2stch(tpnt);
	str->right = selectedPoint.x;
	str->bottom = selectedPoint.y;
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
		MoveMemory(&trct, &selectedPointsRect, sizeof(fRECTANGLE));
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
			tlng = msg.pt.y - stitchWindowOrigin.y;
			rat = (double)(selectedFormsRectangle.bottom - tlng) / (selectedFormsRectangle.bottom - selectedFormsRectangle.top);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.bottom;
				rat = (double)(selectedPoint.y - ref) / (SelectedForm->rectangle.top - ref);
				SelectedForm->rectangle.top = selectedPoint.y;
			}
			else {
				ref = stitchRangeRect.bottom;
				rat = (double)(selectedPoint.y - ref) / (stitchRangeRect.top - ref);
			}
		}
		break;

	case 1:

		if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.left;
			tlng = msg.pt.x - stitchWindowOrigin.x;
			rat = (double)(tlng - selectedFormsRectangle.left) / (selectedFormsRectangle.right - selectedFormsRectangle.left);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.left;
				rat = (double)(selectedPoint.x - ref) / (SelectedForm->rectangle.right - ref);
				SelectedForm->rectangle.right = selectedPoint.x;
			}
			else {
				ref = stitchRangeRect.left;
				rat = (double)(selectedPoint.x - ref) / (stitchRangeRect.right - ref);
			}
		}
		break;

	case 2:

		if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.top;
			tlng = msg.pt.y - stitchWindowOrigin.y;
			rat = (double)(tlng - selectedFormsRectangle.top) / (selectedFormsRectangle.bottom - selectedFormsRectangle.top);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.top;
				rat = (double)(selectedPoint.y - ref) / (SelectedForm->rectangle.bottom - ref);
				SelectedForm->rectangle.bottom = selectedPoint.y;
			}
			else {
				ref = stitchRangeRect.top;
				rat = (double)(selectedPoint.y - ref) / (stitchRangeRect.bottom - ref);
			}
		}
		break;

	case 3:

		if (selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.right;
			tlng = msg.pt.x - stitchWindowOrigin.x;
			rat = (double)(selectedFormsRectangle.right - tlng) / (selectedFormsRectangle.right - selectedFormsRectangle.left);
		}
		else {
			if (chkMap(FORMSEL)) {
				ref = SelectedForm->rectangle.right;
				rat = (double)(selectedPoint.x - ref) / (SelectedForm->rectangle.left - ref);
				SelectedForm->rectangle.left = selectedPoint.x;
			}
			else {
				ref = stitchRangeRect.right;
				rat = (double)(selectedPoint.x - ref) / (stitchRangeRect.left - ref);
			}
		}
		break;
	}
	if (selectedFormControlVertex & 1) {
		if (chkMap(FPSEL)) {
			fvars(closestFormToCursor);
			ine = selectedFormPoints.start;
			for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {
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
			for (ind = 0; ind < header.stitchCount; ind++)
				stitchBuffer[ind].x = (stitchBuffer[ind].x - ref)*rat + ref;
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
					for (ind = groupStartStitch; ind <= groupEndStitch; ind++)
						stitchBuffer[ind].x = (stitchBuffer[ind].x - ref)*rat + ref;
				}
			}
		}
	}
	else {
		if (chkMap(FPSEL)) {
			fvars(closestFormToCursor);
			ine = selectedFormPoints.start;
			for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {
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
			for (ind = 0; ind < header.stitchCount; ind++)
				stitchBuffer[ind].y = (stitchBuffer[ind].y - ref)*rat + ref;
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
					for (ind = groupStartStitch; ind <= groupEndStitch; ind++)
						stitchBuffer[ind].y = (stitchBuffer[ind].y - ref)*rat + ref;
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
		selectedPoint.x = msg.pt.x - stitchWindowOrigin.x;
		selectedPoint.y = msg.pt.y - stitchWindowOrigin.y;
		l_siz0.y = rct.bottom - rct.top;
	}
	else {
		px2stch();
		fvars(closestFormToCursor);
		if (chkMap(FORMSEL))
			rct = SelectedForm->rectangle;
		else {
			rct.bottom = stitchRangeRect.bottom;
			rct.top = stitchRangeRect.top;
			rct.right = stitchRangeRect.right;
			rct.left = stitchRangeRect.left;
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
		l_siz1.x = fabs(selectedPoint.x - ref.x);
		l_siz1.y = fabs(selectedPoint.y - ref.y);
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
		l_siz1.x = fabs(selectedPoint.x - ref.x);
		l_siz1.y = fabs(selectedPoint.y - ref.y);
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
		l_siz1.x = fabs(selectedPoint.x - ref.x);
		l_siz1.y = fabs(selectedPoint.y - ref.y);
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
		l_siz1.x = fabs(selectedPoint.x - ref.x);
		l_siz1.y = fabs(selectedPoint.y - ref.y);
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
		ine = selectedFormPoints.start;
		for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {
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
		for (ind = 0; ind < header.stitchCount; ind++) {
			stitchBuffer[ind].x = (stitchBuffer[ind].x - sref.x)*rat.x + sref.x;
			stitchBuffer[ind].y = (stitchBuffer[ind].y - sref.y)*rat.y + sref.y;
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
				for (ind = groupStartStitch; ind <= groupEndStitch; ind++) {
					stitchBuffer[ind].x = (stitchBuffer[ind].x - ref.x)*rat.x + ref.x;
					stitchBuffer[ind].y = (stitchBuffer[ind].y - ref.y)*rat.y + ref.y;
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
		for (ind = 0; ind < header.stitchCount; ind++) {
			if ((stitchBuffer[ind].attribute&(FRMSK | TYPMSK | FTHMSK)) == at) {
				stitchBuffer[ind].attribute &= 0xfffffff0;
				stitchBuffer[ind].attribute |= col;
			}
		}
	}
}

void nufthcol(unsigned col) {
	unsigned at, ind;

	if (SelectedForm->fillInfo.feather.color != col) {
		SelectedForm->fillInfo.feather.color = col;
		at = (closestFormToCursor << 4) | FTHMSK;
		for (ind = 0; ind < header.stitchCount; ind++) {
			if ((stitchBuffer[ind].attribute&(FRMSK | FTHMSK)) == at) {
				stitchBuffer[ind].attribute &= 0xfffffff0;
				stitchBuffer[ind].attribute |= col;
			}
		}
	}
}

void nubrdcol(unsigned col) {
	unsigned at, ind;

	SelectedForm->borderColor = col;
	at = (closestFormToCursor << 4) | FRMBFIL;
	for (ind = 0; ind < header.stitchCount; ind++) {
		if ((stitchBuffer[ind].attribute&(FRMSK | TYPMSK)) == at) {
			stitchBuffer[ind].attribute &= 0xfffffff0;
			stitchBuffer[ind].attribute |= col;
		}
	}
}

void nulapcol(unsigned col) {
	unsigned at, ind;

	if ((unsigned)(SelectedForm->borderColor >> 4) != col) {
		SelectedForm->borderColor &= 0xf;
		SelectedForm->borderColor |= col << 4;
		at = (closestFormToCursor << 4) | TYPMSK;
		for (ind = 0; ind < header.stitchCount; ind++) {
			if ((stitchBuffer[ind].attribute&(TYPMSK | FRMSK)) == at) {
				stitchBuffer[ind].attribute &= 0xfffffff0;
				stitchBuffer[ind].attribute |= col;
			}
		}
	}
}

void sidwnd(HWND wnd) {
	RECT		wrct;
	unsigned	baksid;

	msgIndex = 0;
	sideWindowEntryBuffer[0] = 0;
	baksid = formMenuChoice;
	unsid();
	formMenuChoice = baksid;
	GetWindowRect(wnd, &wrct);
	GetWindowRect(hFormData, &msgRct);
	hSideMessageWin = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		msgRct.right - mainWindowOrigin.x + 3,
		wrct.top - mainWindowOrigin.y - 3,
		buttonWidthX3,
		wrct.bottom - wrct.top + 3,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void prfsid(HWND wnd) {
	RECT	wrct;

	msgIndex = 0;
	sideWindowEntryBuffer[0] = 0;
	unsid();
	GetWindowRect(wnd, &wrct);
	GetClientRect(hPreferencesWindow, &msgRct);
	hSideMessageWin = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		wrct.right - mainWindowOrigin.x + 6,
		wrct.top - mainWindowOrigin.y - 3,
		rightWindowSize.x,
		wrct.bottom - wrct.top + 3,
		hWnd,
		NULL,
		hInst,
		NULL);
}

void sbold() {
	fvars(closestFormToCursor);
	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EGBLD;
	SelectedForm->borderColor = activeColor;
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
			ritot(header.stitchCount);
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
		hPreferencesWindow,
		NULL,
		hInst,
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
		hPreferencesWindow,
		NULL,
		hInst,
		NULL);
}

void prflin(unsigned p_lin) {
	prftwin(stab[p_lin]);
	thDat[p_lin - STR_PRF0] = prfnwin(msgbuf);
	nxtlin();
}

void sethup() {
	if (iniFile.hoopSizeX == LHUPX&&iniFile.hoopSizeY == LHUPY) {
		iniFile.hoopType = LARGHUP;
		return;
	}
	if (iniFile.hoopSizeX == SHUPX&&iniFile.hoopSizeY == SHUPY) {
		iniFile.hoopType = SMALHUP;
		return;
	}
	if (iniFile.hoopSizeX == HUP100XY&&iniFile.hoopSizeY == HUP100XY) {
		iniFile.hoopType = HUP100;
		return;
	}
	iniFile.hoopType = CUSTHUP;
}

#if LANG==ENG||LANG==HNG

void prfmsg() {
	HDC		prfdc;
	RECT	prfrct;

	if (rstMap(INSRT))
		setMap(WASRT);
	rstMap(BIGBOX);
	selectedFormCount = 0;
	if (hFormData) {
		undat();
		unsid();
		formMenuChoice = 0;
	}
	leftWindowSize.x = leftWindowSize.y = 0;
	rightWindowSize.x = rightWindowSize.y = 0;
	maxtsiz(stab[STR_PRF0 + 4], &leftWindowSize);
	maxtsiz(stab[STR_TAPR], &rightWindowSize);
	leftWindowSize.x = prfsiz;
	leftWindowSize.x += 4;
#if LANG==HNG
	leftWindowSize.x += 10;
#endif
	DestroyWindow(hPreferencesWindow);
	prfwid = leftWindowSize.x + rightWindowSize.x + 18;
	hPreferencesWindow = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidthX3 + 3,
		3,
		prfwid,
		leftWindowSize.y*PRFLINS + 12,
		hWnd,
		NULL,
		hInst,
		NULL);
	prfdc = GetDC(hPreferencesWindow);
	GetClientRect(hPreferencesWindow, &prfrct);
	FillRect(prfdc, &prfrct, (HBRUSH)(COLOR_WINDOW + 1));
	leftWindowCoords.top = rightWindowCoords.top = 3;
	leftWindowCoords.bottom = rightWindowCoords.bottom = 3 + leftWindowSize.y;
	leftWindowCoords.left = 3;
	leftWindowCoords.right = 3 + leftWindowSize.x;
	rightWindowCoords.left = 6 + leftWindowSize.x;
	rightWindowCoords.right = 6 + leftWindowSize.x + rightWindowSize.x + 6;
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", underlayColor + 1);
	prflin(STR_PRF10);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", borderWidth / PFGRAN);
	prflin(STR_PRF3);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", buttonholeFillCornerLength / PFGRAN);
	prflin(STR_PRF14);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.chainSpace / PFGRAN);
	prflin(STR_PRF23);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.chainRatio);
	prflin(STR_PRF24);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", iniFile.clipboardOffset / PFGRAN);
	prflin(STR_PRF21);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", iniFile.fillPhase);
	prflin(STR_PRF22);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.eggRatio);
	prflin(STR_PRF26);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.fillAngle / PI * 180);
	prflin(STR_PRF1);
	if (chku(SQRFIL))
		strcpy_s(msgbuf, stab[STR_SQR]);
	else
		strcpy_s(msgbuf, stab[STR_PNTD]);
	prflin(STR_PRF2);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", stitchSpace / PFGRAN);
	prflin(STR_PRF0);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", duthrsh(showStitchThreshold));
	prflin(STR_PRF7);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", iniFile.gridSize / PFGRAN);
	prflin(STR_PRF20);
	sethup();
	sprintf_s(msgbuf, sizeof(msgbuf), "%s", stab[STR_HUP0 + iniFile.hoopType - 1]);
	prflin(STR_PRF17);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", iniFile.hoopSizeY / PFGRAN);
	prflin(STR_PRF27);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", iniFile.hoopSizeX / PFGRAN);
	prflin(STR_PRF18);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.cursorNudgeStep);
	prflin(STR_PRF25);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", picotSpace / PFGRAN);
	prflin(STR_PRF16);
	if (chku(BLUNT))
		strcpy_s(msgbuf, stab[STR_BLUNT]);
	else
		strcpy_s(msgbuf, stab[STR_TAPR]);
	prflin(STR_PRF15);
	if (chku(DUND))
		strcpy_s(msgbuf, stab[STR_ON]);
	else
		strcpy_s(msgbuf, stab[STR_OFF]);
	prflin(STR_PRF19);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", smallStitchLength / PFGRAN);
	prflin(STR_PRF9);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", snapLength / PFGRAN);
	prflin(STR_PRF11);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", spiralWrap);
	prflin(STR_PRF13);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", starRatio);
	prflin(STR_PRF12);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", duthrsh(stitchBoxesThreshold));
	prflin(STR_PRF8);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.maxStitchLength / PFGRAN);
	prflin(STR_PRF4);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", userStitchLength / PFGRAN);
	prflin(STR_PRF5);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", minStitchLength / PFGRAN);
	prflin(STR_PRF6);
	setMap(PRFACT);
	ReleaseDC(hWnd, prfdc);
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
	if (hFormData) {
		undat();
		unsid();
		formMenuChoice = 0;
	}
	leftWindowSize.x = leftWindowSize.y = 0;
	rightWindowSize.x = rightWindowSize.y = 0;
	maxtsiz(stab[STR_PRF0 + 4], &leftWindowSize);
	maxtsiz(stab[STR_BLUNT], &rightWindowSize);
	leftWindowSize.x = prfsiz;
	rightWindowSize.x += 4;
	DestroyWindow(hPreferencesWindow);
	prfwid = leftWindowSize.x + rightWindowSize.x + 18;
	hPreferencesWindow = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidthX3 + 3,
		3,
		prfwid,
		leftWindowSize.y*PRFLINS + 12,
		hWnd,
		NULL,
		hInst,
		NULL);
	prfdc = GetDC(hPreferencesWindow);
	GetClientRect(hPreferencesWindow, &prfrct);
	FillRect(prfdc, &prfrct, (HBRUSH)(COLOR_WINDOW + 1));
	leftWindowCoords.top = rightWindowCoords.top = 3;
	leftWindowCoords.bottom = rightWindowCoords.bottom = 3 + leftWindowSize.y;
	leftWindowCoords.left = 3;
	leftWindowCoords.right = 3 + leftWindowSize.x;
	rightWindowCoords.left = 6 + leftWindowSize.x;
	rightWindowCoords.right = 6 + leftWindowSize.x + rightWindowSize.x + 6;
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", userStitchLength / PFGRAN);
	prflin(STR_PRF5);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.maxStitchLength / PFGRAN);
	prflin(STR_PRF4);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", minStitchLength / PFGRAN);
	prflin(STR_PRF6);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", smallStitchLength / PFGRAN);
	prflin(STR_PRF9);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.fillAngle / PI * 180);
	prflin(STR_PRF1);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", stitchSpace / PFGRAN);
	prflin(STR_PRF0);
	if (chku(SQRFIL))
		strcpy_s(msgbuf, stab[STR_SQR]);
	else
		strcpy_s(msgbuf, stab[STR_PNTD]);
	prflin(STR_PRF2);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", borderWidth / PFGRAN);
	prflin(STR_PRF3);
	if (chku(BLUNT))
		strcpy_s(msgbuf, stab[STR_BLUNT]);
	else
		strcpy_s(msgbuf, stab[STR_TAPR]);
	prflin(STR_PRF15);
	if (chku(DUND))
		strcpy_s(msgbuf, stab[STR_ON]);
	else
		strcpy_s(msgbuf, stab[STR_OFF]);
	prflin(STR_PRF19);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", buttonholeFillCornerLength / PFGRAN);
	prflin(STR_PRF14);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.chainSpace / PFGRAN);
	prflin(STR_PRF23);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.chainRatio);
	prflin(STR_PRF24);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", iniFile.clipboardOffset / PFGRAN);
	prflin(STR_PRF21);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", iniFile.fillPhase);
	prflin(STR_PRF22);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", picotSpace / PFGRAN);
	prflin(STR_PRF16);
	sethup();
	sprintf_s(msgbuf, sizeof(msgbuf), stab[STR_HUP0 + iniFile.hoopType - 1]);
	prflin(STR_PRF17);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", iniFile.hoopSizeY / PFGRAN);
	prflin(STR_PRF27);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.0f mm", iniFile.hoopSizeX / PFGRAN);
	prflin(STR_PRF18);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f mm", iniFile.gridSize / PFGRAN);
	prflin(STR_PRF20);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", duthrsh(showStitchThreshold));
	prflin(STR_PRF7);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.cursorNudgeStep);
	prflin(STR_PRF25);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", snapLength / PFGRAN);
	prflin(STR_PRF11);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", duthrsh(stitchBoxesThreshold));
	prflin(STR_PRF8);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", iniFile.eggRatio);
	prflin(STR_PRF26);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", spiralWrap);
	prflin(STR_PRF13);
	sprintf_s(msgbuf, sizeof(msgbuf), "%.2f", starRatio);
	prflin(STR_PRF12);
	sprintf_s(msgbuf, sizeof(msgbuf), "%d", underlayColor + 1);
	prflin(STR_PRF10);
	setMap(PRFACT);
	ReleaseDC(hWnd, prfdc);
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
	len = 500 / nsids*zoomFactor*(unzoomedRect.x + unzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(nsids);
	SelectedForm->sides = nsids;
	SelectedForm->attribute = activeLayer << 1;
	fvars(formIndex);
	px2stch();
	pnt.x = selectedPoint.x;
	pnt.y = selectedPoint.y;
	for (ind = 0; ind < sides; ind++) {
		currentFormVertices[ind].x = pnt.x;
		currentFormVertices[ind].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	SelectedForm->type = POLI;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	fmovdif.x = fmovdif.y = 0;
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
	SelectedForm->attribute = (activeLayer << 1);
	fvars(formIndex);
	px2stch();
	pnt.x = selectedPoint.x;
	pnt.y = selectedPoint.y;
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
	SelectedForm->type = POLI;
	frmout(formIndex);
	fmovdif.x = fmovdif.y = 0;
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
	len = 800 / nsids*zoomFactor*(unzoomedRect.x + unzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	num = nsids*spiralWrap;
	SelectedForm->vertices = adflt(num);
	tflt = new fPOINT[nsids];
	tdif = new fPOINT[nsids];
	SelectedForm->sides = num;
	SelectedForm->attribute = (activeLayer << 1);
	fvars(formIndex);
	px2stch();
	pnt.x = selectedPoint.x;
	pnt.y = selectedPoint.y;
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
	SelectedForm->type = LIN;
	frmout(formIndex);
	fmovdif.x = fmovdif.y = 0;
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
	SelectedForm->attribute = activeLayer << 1;
	currentFormVertices = &formPoints[fltad];
	px2stch();
	pnt.x = selectedPoint.x;
	pnt.y = selectedPoint.y;
	dang = PI * 2 / nsids;
	len = 300 / nsids*zoomFactor*(unzoomedRect.x + unzoomedRect.y) / (LHUPX + LHUPY);
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
	SelectedForm->type = POLI;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	fmovdif.x = fmovdif.y = 0;
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
	len = 500 / nsids*zoomFactor*(unzoomedRect.x + unzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = &formPoints[fltad];
	SelectedForm->attribute = activeLayer << 1;
	fvars(formIndex);
	px2stch();
	pnt.x = selectedPoint.x;
	pnt.y = selectedPoint.y;
	ind = 0;
	selectedPoint.x -= (float)0.0001;
	while (pnt.x >= selectedPoint.x) {
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
	SelectedForm->type = POLI;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	fmovdif.x = fmovdif.y = 0;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

float shreg(float hi, float ref) {
	return (hi - ref)*egrat + ref;
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
			currentFormVertices[ind].y = ref - (ref - currentFormVertices[ind].y)*iniFile.eggRatio;
	}
	egrat = hi / (currentFormVertices[nsids >> 2].y - currentFormVertices[0].y);
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
	SelectedForm->attribute = activeLayer << 1;
	fvars(formIndex);
	px2stch();
	selectedPoint.x;
	selectedPoint.y;
	off.x = unzoomedRect.x / 6;
	off.y = unzoomedRect.y / (6 * nsids);
	for (ind = 0; ind < nsids; ind++) {
		currentFormVertices[ind].x = selectedPoint.x;
		currentFormVertices[ind].y = selectedPoint.y;
		selectedPoint.y -= off.y;
		if (ind & 1)
			selectedPoint.x += off.x;
		else
			selectedPoint.x -= off.x;
	}
	SelectedForm->type = LIN;
	closestFormToCursor = formIndex;
	frmout(formIndex);
	fmovdif.x = fmovdif.y = 0;
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
		av = (selectedPointsRect.right - selectedPointsRect.left)*0.5 + selectedPointsRect.left;
		ine = selectedFormPoints.start;
		for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {
			currentFormVertices[ine].x = av + av - currentFormVertices[ine].x;
			ine = pdir(ine);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		av = (allItemsRectangle.right - allItemsRectangle.left) / 2 + allItemsRectangle.left;
		for (ind = 0; ind < fltad; ind++)
			formPoints[ind].x = av + av - formPoints[ind].x;
		for (ind = 0; ind < header.stitchCount; ind++)
			stitchBuffer[ind].x = av + av - stitchBuffer[ind].x;
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
		for (ind = 0; ind < header.stitchCount; ind++) {
			ine = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
			if (chkr(ine) && !(stitchBuffer[ind].attribute&NOTFRM))
				stitchBuffer[ind].x = av + av - stitchBuffer[ind].x;
		}
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			av = (SelectedForm->rectangle.right - SelectedForm->rectangle.left) / 2 + SelectedForm->rectangle.left;
			for (ind = 0; ind < sides; ind++)
				currentFormVertices[ind].x = av + av - currentFormVertices[ind].x;
			for (ind = 0; ind < header.stitchCount; ind++) {
				if ((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor && !(stitchBuffer[ind].attribute&NOTFRM))
					stitchBuffer[ind].x = av + av - stitchBuffer[ind].x;
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
				for (ind = groupStartStitch; ind <= groupEndStitch; ind++)
					stitchBuffer[ind].x = av + av - stitchBuffer[ind].x;
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
		av = (selectedPointsRect.top - selectedPointsRect.bottom)*0.5 + selectedPointsRect.bottom;
		ine = selectedFormPoints.start;
		for (ind = 0; ind <= selectedFormPoints.pointCount; ind++) {
			currentFormVertices[ine].y = av + av - currentFormVertices[ine].y;
			ine = pdir(ine);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		savdo();
		av = (allItemsRectangle.top - allItemsRectangle.bottom) / 2 + allItemsRectangle.bottom;
		for (ind = 0; ind < fltad; ind++)
			formPoints[ind].y = av + av - formPoints[ind].y;
		for (ind = 0; ind < header.stitchCount; ind++)
			stitchBuffer[ind].y = av + av - stitchBuffer[ind].y;
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
		for (ind = 0; ind < header.stitchCount; ind++) {
			ine = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
			if (chkr(ine) && !(stitchBuffer[ind].attribute&NOTFRM))
				stitchBuffer[ind].y = av + av - stitchBuffer[ind].y;
		}
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			av = (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom) / 2 + SelectedForm->rectangle.bottom;
			for (ind = 0; ind < sides; ind++)
				currentFormVertices[ind].y = av + av - currentFormVertices[ind].y;
			for (ind = 0; ind < header.stitchCount; ind++) {
				if ((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor && !(stitchBuffer[ind].attribute&NOTFRM))
					stitchBuffer[ind].y = av + av - stitchBuffer[ind].y;
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
				for (ind = groupStartStitch; ind <= groupEndStitch; ind++)
					stitchBuffer[ind].y = av + av - stitchBuffer[ind].y;
				setMap(RESTCH);
			}
		}
	}
}

void tomsg() {
	RECT	okrct;
	SIZE	tsiz;

	GetWindowRect(hOKButton, &okrct);
	GetTextExtentPoint32(stitchWindowMemDC, stab[STR_DELST2], strlen(stab[STR_DELST2]), &tsiz);
	hto = CreateWindow(
		"STATIC",
		stab[STR_DELST2],
		SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
		3,
		okrct.bottom - stitchWindowOrigin.y + 6 + tsiz.cy,
		tsiz.cx + 6,
		tsiz.cy + 6,
		hMainStitchWin,
		NULL,
		hInst,
		NULL);
}

void sprct(unsigned strt, unsigned fin) {
	dPOINT	dif, tpnt;
	VRCT2*	tvrct;

	tvrct = &pointsVertRect[strt];
	dif.x = opnts[fin].x - opnts[strt].x;
	dif.y = opnts[fin].y - opnts[strt].y;
	if (dif.x&&dif.y) {
		slope = -dif.x / dif.y;
		tpnt.x = currentFormVertices[fin].x;
		tpnt.y = currentFormVertices[fin].y;
		proj(tpnt, slope, opnts[strt], opnts[fin], &tvrct->dopnt);
		proj(tpnt, slope, ipnts[strt], ipnts[fin], &tvrct->dipnt);
		tpnt.x = currentFormVertices[strt].x;
		tpnt.y = currentFormVertices[strt].y;
		proj(tpnt, slope, opnts[strt], opnts[fin], &tvrct->aopnt);
		proj(tpnt, slope, ipnts[strt], ipnts[fin], &tvrct->aipnt);
		tpnt.x = ipnts[strt].x;
		tpnt.y = ipnts[strt].y;
		if (proj(tpnt, slope, opnts[strt], opnts[fin], &tvrct->bopnt)) {
			tvrct->bipnt.x = ipnts[strt].x;
			tvrct->bipnt.y = ipnts[strt].y;
		}
		else {
			tvrct->bopnt.x = opnts[strt].x;
			tvrct->bopnt.y = opnts[strt].y;
			tpnt.x = opnts[strt].x;
			tpnt.y = opnts[strt].y;
			proj(tpnt, slope, ipnts[strt], ipnts[fin], &tvrct->bipnt);
		}
		tpnt.x = ipnts[fin].x;
		tpnt.y = ipnts[fin].y;
		if (proj(tpnt, slope, opnts[strt], opnts[fin], &tvrct->copnt)) {
			tvrct->cipnt.x = ipnts[fin].x;
			tvrct->cipnt.y = ipnts[fin].y;
		}
		else {
			tvrct->copnt.x = opnts[fin].x;
			tvrct->copnt.y = opnts[fin].y;
			tpnt.x = opnts[fin].x;
			tpnt.y = opnts[fin].y;
			proj(tpnt, slope, ipnts[strt], ipnts[fin], &tvrct->cipnt);
		}
	}
	else {
		if (dif.x) {
			tpnt.x = currentFormVertices[fin].x;
			projv(tpnt.x, opnts[strt], opnts[fin], &tvrct->dopnt);
			projv(tpnt.x, ipnts[strt], ipnts[fin], &tvrct->dipnt);
			tpnt.x = currentFormVertices[strt].x;
			projv(tpnt.x, opnts[strt], opnts[fin], &tvrct->aopnt);
			projv(tpnt.x, ipnts[strt], ipnts[fin], &tvrct->aipnt);
			tpnt.x = ipnts[strt].x;
			if (projv(tpnt.x, opnts[strt], opnts[fin], &tvrct->bopnt)) {
				tvrct->bipnt.x = ipnts[strt].x;
				tvrct->bipnt.y = ipnts[strt].y;
			}
			else {
				tvrct->bopnt.x = opnts[strt].x;
				tvrct->bopnt.y = opnts[strt].y;
				tpnt.x = opnts[strt].x;
				projv(tpnt.x, ipnts[strt], ipnts[fin], &tvrct->bipnt);
			}
			tpnt.x = ipnts[fin].x;
			if (projv(tpnt.x, opnts[strt], opnts[fin], &tvrct->copnt)) {
				tvrct->cipnt.x = ipnts[fin].x;
				tvrct->cipnt.y = ipnts[fin].y;
			}
			else {
				tvrct->copnt.x = opnts[fin].x;
				tvrct->copnt.y = opnts[fin].y;
				tpnt.x = opnts[fin].x;
				projv(tpnt.x, ipnts[strt], ipnts[fin], &tvrct->cipnt);
			}
		}
		else {
			tpnt.y = currentFormVertices[fin].y;
			projh(tpnt.y, opnts[strt], opnts[fin], &tvrct->dopnt);
			projh(tpnt.y, ipnts[strt], ipnts[fin], &tvrct->dipnt);
			tpnt.y = currentFormVertices[strt].y;
			projh(tpnt.y, opnts[strt], opnts[fin], &tvrct->aopnt);
			projh(tpnt.y, ipnts[strt], ipnts[fin], &tvrct->aipnt);
			tpnt.y = ipnts[strt].y;
			if (projh(tpnt.y, opnts[strt], opnts[fin], &tvrct->bopnt)) {
				tvrct->bipnt.x = ipnts[strt].x;
				tvrct->bipnt.y = ipnts[strt].y;
			}
			else {
				tvrct->bopnt.x = opnts[strt].x;
				tvrct->bopnt.y = opnts[strt].y;
				tpnt.y = opnts[strt].y;
				projh(tpnt.y, ipnts[strt], ipnts[fin], &tvrct->bipnt);
			}
			tpnt.y = ipnts[fin].y;
			if (projh(tpnt.y, opnts[strt], opnts[fin], &tvrct->copnt)) {
				tvrct->cipnt.x = ipnts[fin].x;
				tvrct->cipnt.y = ipnts[fin].y;
			}
			else {
				tvrct->copnt.x = opnts[fin].x;
				tvrct->copnt.y = opnts[fin].y;
				tpnt.y = opnts[fin].y;
				projh(opnts[fin].y, ipnts[strt], ipnts[fin], &tvrct->cipnt);
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
		dif0.x = selectedPoint.x - strt0.x;
		dif0.y = selectedPoint.y - strt0.y;
		dif1.x = selectedPoint.x - strt1.x;
		dif1.y = selectedPoint.y - strt1.y;
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
	if (hypot(selectedPoint.x - piv.x, selectedPoint.y - piv.y) > 2 * PI)
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
				tang = atan2(ipnts[fin].y - opnts[fin].y, ipnts[fin].x - opnts[fin].x);
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
				tang = atan2(opnts[fin].y - ipnts[fin].y, opnts[fin].x - ipnts[fin].x);
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

	selectedPoint.x = currentFormVertices[strtlin].x;
	selectedPoint.y = currentFormVertices[strtlin].y;
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

	MoveMemory(&angflt[1], currentFormVertices, sizeof(fPOINT)*sides);
	dif.x = currentFormVertices[1].x - currentFormVertices[0].x;
	dif.y = currentFormVertices[1].y - currentFormVertices[0].y;
	if (fabs(dif.x) > fabs(dif.y))
		rat = fabs(0.1 / dif.x);
	else
		rat = fabs(0.1 / dif.y);
	angflt[0].x = currentFormVertices[0].x - dif.x*rat;
	angflt[0].y = currentFormVertices[0].y - dif.y*rat;
	MoveMemory(&angfrm, SelectedForm, sizeof(FRMHED));
	angfrm.vertices = angflt;
	angfrm.sides += 3;
	dif.x = currentFormVertices[sides - 1].x - currentFormVertices[sides - 2].x;
	dif.y = currentFormVertices[sides - 1].y - currentFormVertices[sides - 2].y;
	if (dif.x > dif.y)
		rat = 0.1 / dif.x;
	else
		rat = 0.1 / dif.y;
	angflt[angfrm.sides - 1].x = currentFormVertices[sides - 1].x + dif.x*rat;
	angflt[angfrm.sides - 1].y = currentFormVertices[sides - 1].y + dif.y*rat;
	SelectedForm = &angfrm;
	sides = angfrm.sides;
	currentFormVertices = angfrm.vertices;
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
	ipnts[sides].x = ipnts[0].x;
	ipnts[sides].y = ipnts[0].y;
	opnts[sides].x = opnts[0].x;
	opnts[sides].y = opnts[0].y;
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
	selectedPoint.x = currentFormVertices[0].x;
	selectedPoint.y = currentFormVertices[0].y;
	if (SelectedForm->edgeType&EGUND) {
		stitchSpace = USPAC;
		setMap(UND);
		plen = SelectedForm->borderSize*URAT;
		setMap(UNDPHAS);
		rstMap(FILDIR);
		plfn(&underlayVertRect[0]);
		bpnt = sequenceIndex;
		rstMap(UNDPHAS);
		selectedPoint.x = currentFormVertices[0].x;
		selectedPoint.y = currentFormVertices[0].y;
		setMap(FILDIR);
		plfn(&underlayVertRect[0]);
		plbak(bpnt);
		prsmal();
		if (sequenceIndex) { //ensure that we can do a valid read from oseq
			selectedPoint.x = oseq[sequenceIndex - 1].x;
			selectedPoint.y = oseq[sequenceIndex - 1].y;
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
		SelectedForm->edgeType = EGPRP;
		if (chku(DUND))
			SelectedForm->edgeType |= EGUND;
		bsizpar();
		SelectedForm->borderSize = borderWidth;
		SelectedForm->edgeSpacing = stitchSpace;
		SelectedForm->borderColor = activeColor;
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
			ritot(header.stitchCount);
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
		strcpy_s(oftxt, stab[STR_FRMPLUS]);
	else {
		strcpy_s(oftxt, stab[STR_FRMINUS]);
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
	SetMenuItemInfo(hMainMenu, ID_FRMOF, FALSE, &meninfo);
	setMap(DUMEN);
	setMap(RESTCH);
}

void frmon() {
	unbsho();
	rstMap(FRMOF);
	strcpy_s(oftxt, stab[STR_FRMPLUS]);
	SetMenuItemInfo(hMainMenu, ID_FRMOF, FALSE, &meninfo);
	setMap(DUMEN);
}

void fnord() {
	int			ind;
	fPOINTATTRIBUTE		tpnt;

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
	fPOINTATTRIBUTE		tpnt;

	fvars(closestFormToCursor);
	if (chkMap(FPSEL)) {
		savdo();
		strt = selectedFormPoints.start;
		fin = (selectedFormPoints.start + selectedFormPoints.pointCount) % sides;
		for (uind = 0; uind <= selectedFormPoints.pointCount >> 1; uind++) {
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
				uind = groupStartStitch;
				for (ind = 0; ind < ((signed)(groupEndStitch - groupStartStitch) >> 1) + 1; ind++) {
					tpnt.attribute = stitchBuffer[uind].attribute;
					tpnt.x = stitchBuffer[uind].x;
					tpnt.y = stitchBuffer[uind].y;
					stitchBuffer[uind].attribute = stitchBuffer[groupEndStitch - ind].attribute;
					stitchBuffer[uind].x = stitchBuffer[groupEndStitch - ind].x;
					stitchBuffer[uind].y = stitchBuffer[groupEndStitch - ind].y;
					stitchBuffer[groupEndStitch - ind].attribute = tpnt.attribute;
					stitchBuffer[groupEndStitch - ind].x = tpnt.x;
					stitchBuffer[groupEndStitch - ind].y = tpnt.y;
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
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		clprat.x = clipBuffer[ind].x / clipboardRectSize.cx;
		clprat.y = clipBuffer[ind].y / clipboardRectSize.cy;
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
			cnt = lsiz / clipboardRectSize.cy;
		else
			cnt = l_rsiz / clipboardRectSize.cy;
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
			if (activePointIndex > MAXSEQ - clipboardStitchCount - 1)
				return;
			trfrm(lpnt0, lpnt1, rpnt0, rpnt1);
		}
	}
}

void fmclp() {
	double		tspac = stitchSpace;

	stitchSpace = clipboardRectSize.cx;
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
	SelectedForm->lengthOrCount.clipCount = clipboardStitchCount;
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = clipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = clipBuffer[ind].y;
	}
	refilfn();
}

void clpfil() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(hWnd)) {
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {
			redclp();
			CloseClipboard();
			if (clipboardRectSize.cx > CLPMIN&&clipboardRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != LIN)
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
				tlen = hypot(stitchBuffer[chk].x - stitchBuffer[ref].x, stitchBuffer[chk].y - stitchBuffer[ref].y);
				if (tlen < snapLength) {
					stitchBuffer[chk].x = stitchBuffer[ref].x;
					stitchBuffer[chk].y = stitchBuffer[ref].y;
				}
			}
		}
	}
}

void nutim(double pl_Size) {
	htim = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonWidthX3,
		0,
		stitchWindowSize.x,
		buttonHeight,
		hWnd,
		NULL,
		hInst,
		NULL);
	timdc = GetDC(htim);
	timstp = (double)stitchWindowSize.x / pl_Size;
	timpos = 0;
	formLines[0].y = 0;
	formLines[1].y = buttonHeight;
	formLines[0].x = formLines[1].x = 0;
	SelectObject(timdc, userPen[0]);
}

void nxtim() {
	Polyline(timdc, formLines, 2);
	timpos += timstp;
	formLines[0].x = formLines[1].x = timpos;
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
			if (!(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind].attribute&FRMSK) == at) {
				tuns = stitchBuffer[ind].x;
				xHistogram[tuns]++;
			}
		}
	}
	else {
		for (ind = strt; ind < fin; ind++) {
			tuns = stitchBuffer[ind].x;
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
		for (ind = 0; ind < header.stitchCount; ind++) {
			if (!(stitchBuffer[ind].attribute&NOTFRM) && (stitchBuffer[ind].attribute&FRMSK) == at) {
				tuns = stitchBuffer[ind].x;
				xPoints[xHistogram[tuns]++] = ind;
			}
		}
	}
	else {
		for (ind = 0; ind < header.stitchCount; ind++) {
			tuns = stitchBuffer[ind].x;
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
	DestroyWindow(htim);
	delete[] txhst;
}

void snap() {
	savdo();
	if (chkMap(GRPSEL)) {
		rngadj();
		snp(groupStartStitch, groupEndStitch);
	}
	else
		snp(0, header.stitchCount);
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
		mov		ecx, underlayColor
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
	if (_bittest((long *)&colorBitmap, underlayColor)) {
		col = underlayColor;
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
	if ((stitchBuffer[ind].attribute&TYPMSK) == FRMBFIL)
		return isrt(formList[(stitchBuffer[ind].attribute&FRMSK) >> 4].edgeType&NEGUND);
	else
		return 0;
}

void rotpar() {
	if (iniFile.rotationAngle < (PI / 180))
		tabmsg(IDS_ROTIN);
	if (chkMap(FPSEL)) {
		rotationCenter.x = midl(selectedPointsRect.right, selectedPointsRect.left);
		rotationCenter.y = midl(selectedPointsRect.top, selectedPointsRect.bottom);
		return;
	}
	if (chkMap(BIGBOX)) {
		rotationCenter.x = midl(allItemsRectangle.right, allItemsRectangle.left);
		rotationCenter.y = midl(allItemsRectangle.top, allItemsRectangle.bottom);
		return;
	}
	if (selectedFormCount) {
		if (chkMap(GMRK)) {
			rotationCenter.x = zoomMarkPoint.x;
			rotationCenter.y = zoomMarkPoint.y;
		}
		else
			dufcntr(&rotationCenter);
		setMap(FRMSROT);
		return;
	}
	if (chkMap(FORMSEL)) {
		rotationRect.left = SelectedForm->rectangle.left;
		rotationRect.right = SelectedForm->rectangle.right;
		rotationRect.top = SelectedForm->rectangle.top;
		rotationRect.bottom = SelectedForm->rectangle.bottom;
		if (!chkMap(GMRK)) {
			rotationCenter.x = (double)(rotationRect.right - rotationRect.left) / 2 + rotationRect.left;
			rotationCenter.y = (double)(rotationRect.top - rotationRect.bottom) / 2 + rotationRect.bottom;
		}
		setMap(FRMROT);
	}
	else {
		if (chkMap(GRPSEL)) {
			rngadj();
			selRct(&rotationRect);
		}
	}
	if (chkMap(GMRK)) {
		rotationCenter.x = zoomMarkPoint.x;
		rotationCenter.y = zoomMarkPoint.y;
	}
	else {
		rotationCenter.x = (double)(rotationRect.right - rotationRect.left) / 2 + rotationRect.left;
		rotationCenter.y = (double)(rotationRect.top - rotationRect.bottom) / 2 + rotationRect.bottom;
	}
}

void rotentr() {
	TCHAR buf[HBUFSIZ];

	LoadString(hInst, IDS_ROTA, buf, HBUFSIZ);
	sprintf_s(msgbuf, sizeof(msgbuf), buf, rotationAngle / PI * 180);
	shoMsg(msgbuf);
	setMap(NUMIN);
	numWnd();
}

void setrang() {
	rotationAngle = iniFile.rotationAngle;
	rotentr();
	setMap(NUROT);
}

void rotcmd() {
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || selectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
		rotationAngle = iniFile.rotationAngle;
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
	rotationAngle = iniFile.rotationAngle;
	fnagain();
}

void bakagain() {
	rotationAngle = 2 * PI - iniFile.rotationAngle;
	fnagain();
}

void rotdup() {
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || selectedFormCount) {
		rotationAngle = iniFile.rotationAngle;
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
	dst = header.stitchCount;
	for (src = groupStartStitch; src <= groupEndStitch; src++) {
		stitchBuffer[dst].x = stitchBuffer[src].x;
		stitchBuffer[dst].y = stitchBuffer[src].y;
		stitchBuffer[dst++].attribute = stitchBuffer[src].attribute&(~(FRMSK | TYPMSK));
	}
	closestPointIndex = header.stitchCount;
	header.stitchCount += (groupEndStitch - groupStartStitch);
	groupStitchIndex = header.stitchCount;
	header.stitchCount++;
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
				ine = header.stitchCount;
				for (ind = groupStartStitch; ind < groupEndStitch; ind++) {
					stitchBuffer[ine].attribute = stitchBuffer[ind].attribute&NLAYMSK | playcod;
					stitchBuffer[ine].x = stitchBuffer[ind].x;
					stitchBuffer[ine++].y = stitchBuffer[ind].y;
				}
				header.stitchCount = ine;
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
		for (ind = 0; ind < header.stitchCount; ind++) {
			if (stitchBuffer[ind].attribute&ALTYPMSK) {
				ine = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
				if (chkr(ine))
					stitchBuffer[ind].attribute = stitchBuffer[ind].attribute&NLAYMSK | stchcod;
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
			for (ind = 0; ind < header.stitchCount; ind++) {
				if (stitchBuffer[ind].attribute&ALTYPMSK && ((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) == closestFormToCursor)
					stitchBuffer[ind].attribute = stitchBuffer[ind].attribute&NLAYMSK | stchcod;
			}
			setMap(RESTCH);
		}
		else {
			if (chkMap(GRPSEL)) {
				savdo();
				rngadj();
				for (ind = groupStartStitch; ind < groupEndStitch; ind++)
					stitchBuffer[ind].attribute = stitchBuffer[ind].attribute&NLAYMSK | stchcod;
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
	case EGLIN:
	case EGBLD:
	case EGSAT:
	case EGAP:
	case EGPRP:
	case EGHOL:
	case EGPIC:
	case EGCLP:
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
		currentFormVertices[ind].x += fmovdif.x;
		currentFormVertices[ind].y -= fmovdif.y;
	}
	frmout(find);
}

void setr(unsigned pbit) {
#if	 __UseASM__
	_asm {
		mov		ebx, offset markedStitchMap
		mov		eax, pbit
		bts[ebx], eax
	}
#else
	_bittestandset((long *)markedStitchMap, pbit);
#endif
}

void clRmap(unsigned len) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ecx, len
		mov		edi, offset markedStitchMap
		rep		stosd
	}
#else
	memset(markedStitchMap, 0, len * sizeof(*markedStitchMap));
#endif
}

#if PESACT

BOOL setrc(unsigned pbit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, offset markedStitchMap
		mov		ecx, pbit
		bts[ebx], ecx
		jnc		short setrcx
		inc		eax
		setrcx :
	}
#else
	return _bittestandset((long *)markedStitchMap, pbit);
#endif
}

#endif

BOOL chkr(unsigned pbit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ecx, pbit
		mov		ebx, offset markedStitchMap
		bt[ebx], ecx
		jnc		chkrx
		inc		eax
		chkrx :
	}
#else
	return _bittest((long *)markedStitchMap, pbit);
#endif
}

void frmsadj() {
	unsigned ind;

	clRmap(RMAPSIZ);
	for (ind = 0; ind < selectedFormCount; ind++)
		setr(selectedFormList[ind]);
	for (ind = 0; ind < header.stitchCount; ind++) {
		if (stitchBuffer[ind].attribute&ALTYPMSK&&chkr((stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {
			stitchBuffer[ind].x += fmovdif.x;
			stitchBuffer[ind].y -= fmovdif.y;
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
	for (ind = 0; ind < header.stitchCount; ind++) {
		if (stitchBuffer[ind].x < allItemsRectangle.left)
			allItemsRectangle.left = stitchBuffer[ind].x;
		if (stitchBuffer[ind].x > allItemsRectangle.right)
			allItemsRectangle.right = stitchBuffer[ind].x;
		if (stitchBuffer[ind].y < allItemsRectangle.bottom)
			allItemsRectangle.bottom = stitchBuffer[ind].y;
		if (stitchBuffer[ind].y > allItemsRectangle.top)
			allItemsRectangle.top = stitchBuffer[ind].y;
	}
}

void selal() {
	rstMap(FORMSEL);
	selectedFormCount = 0;
	rstMap(SELBOX);
	rstMap(GRPSEL);
	getbig();
	zoomRect.bottom = 0;
	zoomRect.left = 0;
	zoomRect.right = unzoomedRect.x;
	zoomRect.top = unzoomedRect.y;
	zoomFactor = 1;
	rstMap(ZUMED);
	movStch();
	nearestCount = 0;
	rstMap(RUNPAT);
	duzrat();
	stchrct2px(allItemsRectangle, &selectedFormsRectangle);
	setMap(BIGBOX);
	setMap(RESTCH);
}

void frmpnts(unsigned typ) {
	unsigned ind = 0;
	unsigned trg = (closestFormToCursor << 4) | typ;

	while (ind < header.stitchCount && (stitchBuffer[ind].attribute&(ALTYPMSK | FRMSK)) != trg)
		ind++;
	closestPointIndex = ind;
	if (header.stitchCount > 0) {
		ind = header.stitchCount - 1;
	}
	else {
		ind = 0;
	}
	while (ind > closestPointIndex && (stitchBuffer[ind].attribute&(ALTYPMSK | FRMSK)) != trg)
		ind--;
	groupStitchIndex = ind;
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
		if (typ == FRMAPFIL && (formList[closestFormToCursor].edgeType&NEGUND) != EGAP) {
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
		closestPointIndex = 0;
		while (closestPointIndex < header.stitchCount&&notfstch(stitchBuffer[closestPointIndex].attribute))
			closestPointIndex++;
		if (closestPointIndex != header.stitchCount)
		{
			if (closestPointIndex)
				closestPointIndex--;
			groupStitchIndex = header.stitchCount - 1;
			while (groupStitchIndex > closestPointIndex&&notfstch(stitchBuffer[groupStitchIndex].attribute))
				groupStitchIndex--;
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
	rng->finish = header.stitchCount;
	if (formList[fpnt].fillType || formList[fpnt].edgeType) {
		while (rng->start < header.stitchCount&&notfstch(stitchBuffer[rng->start].attribute))
			rng->start++;
		rng->finish = header.stitchCount - 1;
		while (rng->finish > rng->start&&notfstch(stitchBuffer[rng->finish].attribute))
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
	rotationAngle = atan2(-dif.x, dif.y);
	ostp.x = SelectedForm->borderSize*cos(rotationAngle);
	ostp.y = SelectedForm->borderSize*sin(rotationAngle);
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
		dif.x = opnts[tlin].x - currentFormVertices[tlin].x;
		dif.y = opnts[tlin].y - currentFormVertices[tlin].y;
	}
	else
	{
		dif.x = ipnts[tlin].x - currentFormVertices[tlin].x;
		dif.y = ipnts[tlin].y - currentFormVertices[tlin].y;
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
	SelectedForm->edgeType = EGHOL;
	SelectedForm->edgeSpacing = stitchSpace;
	SelectedForm->borderColor = activeColor;
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
			ritot(header.stitchCount);
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
			for (inf = 0; inf < header.stitchCount; inf++) {
				if ((stitchBuffer[inf].attribute&FRMSK) == tat && !(stitchBuffer[ind].attribute&NOTFRM)) {
					stitchBuffer[inf].x += dif.x;
					stitchBuffer[inf].y += dif.y;
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
		SetCapture(hWnd);
		setMap(RESTCH);
	}
}

void clpcrnr(unsigned p_lin) {
	unsigned	tlin = nxt(p_lin);
	unsigned	ind;
	dPOINT		dif, pnt;
	fPOINT		tpnt;
	double		len, rat;
	fPOINTATTRIBUTE		rpnt;

	rpnt.x = (clipboardRect.right - clipboardRect.left) / 2 + clipboardRect.left;
	rpnt.y = clipboardRect.top;
	if (chkMap(INDIR))
	{
		dif.x = opnts[tlin].x - currentFormVertices[tlin].x;
		dif.y = opnts[tlin].y - currentFormVertices[tlin].y;
	}
	else
	{
		dif.x = ipnts[tlin].x - currentFormVertices[tlin].x;
		dif.y = ipnts[tlin].y - currentFormVertices[tlin].y;
	}
	rotationAngle = atan2(dif.y, dif.x) + PI / 2;
	rotang1(rpnt, &clipReference);
	for (ind = 0; ind < clipboardStitchCount; ind++)
		rotang1(clipBuffer[ind], &clipFillData[ind]);
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
	fPOINTATTRIBUTE		rpnt;
	unsigned	cnt, ind;

	dif.x = currentFormVertices[fin].x - currentFormVertices[strt].x;
	dif.y = currentFormVertices[fin].y - currentFormVertices[strt].y;
	rotationAngle = atan2(-dif.x, dif.y);
	ostp.x = SelectedForm->borderSize*cos(rotationAngle);
	ostp.y = SelectedForm->borderSize*sin(rotationAngle);
	len = hypot(dif.x, dif.y);
	spac += clipboardRectSize.cx;
	cnt = len / spac;
	rpnt.x = (clipboardRect.right - clipboardRect.left) / 2 + clipboardRect.left;
	rpnt.y = clipboardRect.top;
	rotationAngle = atan2(dif.y, dif.x);
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
		for (ind = 0; ind < clipboardStitchCount; ind++)
			rotang1(clipBuffer[ind], &clipFillData[ind]);
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
	plen = clipboardRectSize.cx / 2;
	plen2 = clipboardRectSize.cx;
	clipFillData = new fPOINT[clipboardStitchCount];
	rotationCenter.x = (clipboardRect.right - clipboardRect.left) / 2 + clipboardRect.left;
	clipReference.y = rotationCenter.y = (clipboardRect.top - clipboardRect.bottom) / 2 + clipboardRect.bottom;
	clipReference.x = clipboardRect.left;
	satout(20);
	if (SelectedForm->type == LIN) {
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
	SelectedForm->edgeType = EGPIC;
	SelectedForm->clipEntries = clipboardStitchCount;
	SelectedForm->borderClipData = nueclp(closestFormToCursor, clipboardStitchCount);
	SelectedForm->borderSize = clipboardRectSize.cy;
	SelectedForm->edgeSpacing = picotSpace;
	SelectedForm->borderColor = activeColor;
	bsizpar();
	savplen(buttonholeFillCornerLength);
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = clipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = clipBuffer[ind].y;
	}
	plen = clipboardRectSize.cy / 2;
	refilfn();
}

void picot() {
	unsigned ind;

	if (filmsgs(FML_PIC))
		return;
	if (OpenClipboard(hWnd)) {
		savdo();
		fvars(closestFormToCursor);
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {
			redclp();
			CloseClipboard();
			if (clipboardRectSize.cx > CLPMIN) {
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
					rotationAngle = pols[ind].angle + poldif.angle;
					len = pols[ind].length*poldif.length;
					oseq[sequenceIndex].x = lopnt.x + cos(rotationAngle)*len;
					oseq[sequenceIndex].y = lopnt.y + sin(rotationAngle)*len;
					sequenceIndex++;
				}
			}
			else {
				oseq[sequenceIndex].x = hipnt.x;
				oseq[sequenceIndex].y = hipnt.y;
				sequenceIndex++;
				for (ind = selins - 1; ind != 0; ind--) {
					rotationAngle = pols[ind - 1].angle + poldif.angle;
					len = pols[ind - 1].length*poldif.length;
					oseq[sequenceIndex].x = lopnt.x + cos(rotationAngle)*len;
					oseq[sequenceIndex].y = lopnt.y + sin(rotationAngle)*len;
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
	if (SelectedForm->lengthOrCount.stitchLength < minStitchLength)
		SelectedForm->lengthOrCount.stitchLength = minStitchLength;
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
		SelectedForm->fillColor = activeColor;
		fsizpar();
		SelectedForm->attribute |= (activeLayer << 1);
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
			tfrm->minFillStitchLen = minStitchLength;
			maxStitchLen = 9 * PFGRAN;
			if (SelectedForm->type == LIN) {
				if (chku(BLUNT))
					ind = 0xffffffff;
				else
					ind = 0;
				satends(ind);
				tfrm->vertices = adflt(sides << 1);
				tfrm->vertices[0].x = opnts[0].x;
				tfrm->vertices[fpnt++].y = opnts[0].y;
				for (ind = 0; ind < sides; ind++) {
					tfrm->vertices[fpnt].x = ipnts[ind].x;
					tfrm->vertices[fpnt++].y = ipnts[ind].y;
				}
				for (ind = sides - 1; ind; ind--) {
					tfrm->vertices[fpnt].x = opnts[ind].x;
					tfrm->vertices[fpnt++].y = opnts[ind].y;
				}
			}
			else {
				tfrm->vertices = adflt((sides << 1) + 2);
				tfrm->vertices[0].x = opnts[0].x;
				tfrm->vertices[fpnt++].y = opnts[0].y;
				tfrm->underlayIndent = iniFile.underlayIndent;
				for (ind = 0; ind < sides; ind++) {
					tfrm->vertices[fpnt].x = ipnts[ind].x;
					tfrm->vertices[fpnt++].y = ipnts[ind].y;
				}
				tfrm->vertices[fpnt].x = ipnts[0].x;
				tfrm->vertices[fpnt++].y = ipnts[0].y;
				tfrm->vertices[fpnt].x = opnts[0].x;
				tfrm->vertices[fpnt++].y = opnts[0].y;
				for (ind = sides - 1; ind; ind--) {
					tfrm->vertices[fpnt].x = opnts[ind].x;
					tfrm->vertices[fpnt++].y = opnts[ind].y;
				}
			}
			tfrm->type = SAT;
			tfrm->fillColor = activeColor;
			tfrm->fillSpacing = stitchSpace;
			tfrm->lengthOrCount.stitchLength = iniFile.maxStitchLength;
			tfrm->sides = fpnt;
			tfrm->attribute = 1;
			tfrm->wordParam = fpnt >> 1;
			tfrm->satinGuideCount = tfrm->wordParam - 2;
			tfrm->satinOrAngle.sac = adsatk(tfrm->satinGuideCount);
			if (chkMap(CNV2FTH)) {
				tfrm->fillType = FTHF;
				tfrm->fillInfo.feather.ratio = iniFile.featherRatio;
				tfrm->fillInfo.feather.upCount = iniFile.featherUpCount;
				tfrm->fillInfo.feather.downCount = iniFile.featherDownCount;
				tfrm->fillInfo.feather.fillType = iniFile.featherFillType;
				tfrm->fillInfo.feather.minStitchSize = iniFile.featherMinStitchSize;
				tfrm->extendedAttribute = iniFile.featherType;
				tfrm->fillInfo.feather.count = iniFile.featherCount;
				tfrm->fillInfo.feather.color = (activeColor + 1)&COLMSK;
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
	if (iniFile.rotationAngle) {
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
	rotationAngle = iniFile.rotationAngle;
	dupfn();
}

void bakdup() {
	rotationAngle = 2 * PI - iniFile.rotationAngle;
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
		cnt = len / clipboardRectSize.cx + 0.5;
		rat = (clipboardRectSize.cx*cnt + 0.004) / len;
		currentFormVertices[ind + 1].x = currentFormVertices[ind].x + dif.x*rat;
		currentFormVertices[ind + 1].y = currentFormVertices[ind].y + dif.y*rat;
	}
	dif.x = currentFormVertices[0].x - currentFormVertices[1].x;
	dif.y = currentFormVertices[0].y - currentFormVertices[1].y;
	len = hypot(dif.x, dif.y);
	rotationAngle = atan2(dif.y, dif.x);
	for (ine = 0; ine < 5; ine++) {
		dif.x = currentFormVertices[0].x - currentFormVertices[ind].x;
		dif.y = currentFormVertices[0].y - currentFormVertices[ind].y;
		dlen = hypot(dif.x, dif.y);
		cnt = dlen / clipboardRectSize.cx;
		adif = dlen - cnt*clipboardRectSize.cx;
		rotationAngle -= adif / len;
		dif.x = cos(rotationAngle)*len;
		dif.y = sin(rotationAngle)*len;
		currentFormVertices[0].x = currentFormVertices[1].x + dif.x;
		currentFormVertices[0].y = currentFormVertices[1].y + dif.y;
	}
	refil();
}

void shrnk() {
	fvars(closestFormToCursor);
	if (chkMap(FORMSEL) && SelectedForm->edgeType == EGCLP) {
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

	dst = &rfrmlst[frepnt++];
	src = &formList[find];
	mvfrms(dst, src, 1);
	mvflpnt(&rflts[fltad], dst->vertices, dst->sides);
	dst->vertices = &formPoints[fltad];
	fltad += dst->sides;
	if (dst->satinGuideCount) {
		mvsatk(&rsats[satkad], dst->satinOrAngle.sac, dst->satinGuideCount);
		dst->satinOrAngle.sac = &satks[satkad];
		satkad += dst->satinGuideCount;
	}
	if (iseclpx(find))
	{
		mvflpnt(&rclps[clpad], dst->borderClipData, dst->clipEntries);
		dst->borderClipData = &clipboardPoints[clpad];
		clpad += dst->clipEntries;
	}
	if (isclpx(find))
	{
		mvflpnt(&rclps[clpad], dst->angleOrClipData.clip, dst->lengthOrCount.clipCount);
		dst->angleOrClipData.clip = &clipboardPoints[clpad];
		clpad += dst->lengthOrCount.clipCount;
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
		srcpnt = frepnt = 0;
		rfrmlst = (FRMHED*)&bseq;
		rflts = (fPOINT*)&rfrmlst[formIndex];
		rsats = (SATCON*)&oseq;
		rclps = (fPOINT*)&rsats[satkad];
		fltad = satkad = clpad = 0;
		for (ind = 0; ind < formIndex; ind++) {
			if (ind == nunum)
				dufdat(closestFormToCursor);
			else {
				if (srcpnt == closestFormToCursor)
					srcpnt++;
				dufdat(srcpnt++);
			}
		}
		mvfrms(formList, rfrmlst, formIndex);
		mvflpnt(formPoints, rflts, fltad);
		mvsatk(satks, rsats, satkad);
		mvflpnt(clipboardPoints, rclps, clpad);
		for (ind = 0; ind < header.stitchCount; ind++) {
			if (stitchBuffer[ind].attribute&TYPMSK) {
				cod = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
				if (cod == closestFormToCursor)
					stchfrm(nunum, &stitchBuffer[ind].attribute);
				else {
					if (cod >= strt&&cod <= fin) {
						if (nunum < closestFormToCursor)
							stchfrm(cod + 1, &stitchBuffer[ind].attribute);
						else
							stchfrm(cod - 1, &stitchBuffer[ind].attribute);
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
		LoadString(hInst, IDS_FRML, buf, HBUFSIZ);
		sprintf_s(msgbuf, sizeof(msgbuf), buf, formIndex);
		shoMsg(msgbuf);
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
			frmhst[duat(hifstch[ind].attribute)]++;
		ine = strt;
		for (ind = 0; ind < formIndex << 2; ind++) {
			tmp = frmhst[ind];
			frmhst[ind] = ine;
			ine += tmp;
		}
		for (ind = strt; ind < fin; ind++)
			moveStitchPoints(&stitchBuffer[frmhst[duat(hifstch[ind].attribute)]++], &hifstch[ind]);
	}
}

void srtbyfrm() {
	unsigned	ind, ine, tmp;
	unsigned	colhst[16];
	unsigned	colr[16];

	if (formIndex) {
		savdo();
		colr[underlayColor] = 0;
		for (ind = 0; ind < 16; ind++) {
			if (ind != underlayColor)
				colr[ind] = ind + 1;
		}
		hifstch = &stitchBuffer[MAXSEQ];
		for (ind = 0; ind < 16; ind++)
			colhst[ind] = 0;
		for (ind = 0; ind < header.stitchCount; ind++)
			colhst[colr[stitchBuffer[ind].attribute & 0xf]]++;
		ine = 0;
		for (ind = 0; ind < 16; ind++) {
			tmp = colhst[ind];
			colhst[ind] = ine;
			ine += tmp;
		}
		for (ind = 0; ind < header.stitchCount; ind++)
			moveStitchPoints(&hifstch[colhst[colr[stitchBuffer[ind].attribute & 0xf]]++], &stitchBuffer[ind]);
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
		mrkcntr.x = zoomMarkPoint.x;
		mrkcntr.y = zoomMarkPoint.y;
	}
	else {
		mrkcntr.x = unzoomedRect.x / 2;
		mrkcntr.y = unzoomedRect.y / 2;
	}
	flg = 0;
	if (selectedFormCount) {
		flg = 1;
		savdo();
		dufcntr(&selcntr);
		fmovdif.x = mrkcntr.x - selcntr.x;
		fmovdif.y = -mrkcntr.y + selcntr.y;
		if (chkMap(CNTRV))
			fmovdif.y = 0;
		if (chkMap(CNTRH))
			fmovdif.x = 0;
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
			fmovdif.x = mrkcntr.x - selcntr.x;
			fmovdif.y = -mrkcntr.y + selcntr.y;
			if (chkMap(CNTRV))
				fmovdif.y = 0;
			if (chkMap(CNTRH))
				fmovdif.x = 0;
			frmadj(closestFormToCursor);
			for (ind = 0; ind < header.stitchCount; ind++) {
				if (stitchBuffer[ind].attribute&ALTYPMSK && (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == closestFormToCursor) {
					stitchBuffer[ind].x += fmovdif.x;
					stitchBuffer[ind].y -= fmovdif.y;
				}
			}
		}
		else {
			if (chkMap(GRPSEL)) {
				flg = 1;
				savdo();
				rngadj();
				grct.right = grct.left = stitchBuffer[groupStartStitch].x;
				grct.top = grct.bottom = stitchBuffer[groupStartStitch].y;
				for (ind = groupStartStitch + 1; ind <= groupEndStitch; ind++) {
					if (stitchBuffer[ind].x < grct.left)
						grct.left = stitchBuffer[ind].x;
					if (stitchBuffer[ind].x > grct.right)
						grct.right = stitchBuffer[ind].x;
					if (stitchBuffer[ind].y < grct.bottom)
						grct.bottom = stitchBuffer[ind].y;
					if (stitchBuffer[ind].y > grct.top)
						grct.top = stitchBuffer[ind].y;
				}
				selcntr.x = (grct.right - grct.left) / 2 + grct.left;
				selcntr.y = (grct.top - grct.bottom) / 2 + grct.bottom;
				fmovdif.x = mrkcntr.x - selcntr.x;
				fmovdif.y = -mrkcntr.y + selcntr.y;
				if (chkMap(CNTRV))
					fmovdif.y = 0;
				if (chkMap(CNTRH))
					fmovdif.x = 0;
				for (ind = groupStartStitch; ind <= groupEndStitch; ind++) {
					stitchBuffer[ind].x += fmovdif.x;
					stitchBuffer[ind].y -= fmovdif.y;
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
	scntr.x = unzoomedRect.x / 2;
	scntr.y = unzoomedRect.y / 2;
	dif.x = scntr.x - dcntr.x;
	dif.y = scntr.y - dcntr.y;
	for (ind = 0; ind < header.stitchCount; ind++) {
		stitchBuffer[ind].x += dif.x;
		stitchBuffer[ind].y += dif.y;
	}
	for (ind = 0; ind < fltad; ind++) {
		formPoints[ind].x += dif.x;
		formPoints[ind].y += dif.y;
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
	beancnt = 0;
	mvstch(ine++, ind);
	if (stitchBuffer[ind + 2].x != stitchBuffer[ind].x || stitchBuffer[ind + 2].y != stitchBuffer[ind].y) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		beancnt += 2;
	}
	ind++;
	mvstch(ine++, ind);
	if (stitchBuffer[ind + 2].x != stitchBuffer[ind].x || stitchBuffer[ind + 2].y != stitchBuffer[ind].y) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		beancnt += 2;
	}
	ind++;
	while (ind < (unsigned)fin - 2) {
		mvstch(ine++, ind);
		if ((stitchBuffer[ind + 2].x != stitchBuffer[ind].x || stitchBuffer[ind + 2].y != stitchBuffer[ind].y) &&
			(stitchBuffer[ind - 2].x != stitchBuffer[ind].x || stitchBuffer[ind - 2].y != stitchBuffer[ind].y)) {
			mvstch(ine++, ind + 1);
			mvstch(ine++, ind);
			beancnt += 2;
		}
		ind++;
	}
	mvstch(ine++, ind);
	if ((stitchBuffer[ind - 2].x != stitchBuffer[ind].x || stitchBuffer[ind - 2].y != stitchBuffer[ind].y)) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		beancnt += 2;
	}
	ind++;
	while (ind < header.stitchCount)
		mvstch(ine++, ind++);
	inf = strt;
	for (ind = MAXSEQ; ind < ine; ind++)
		mvstch(inf++, ind);
	header.stitchCount = inf;
}

void dubean() {
	if (header.stitchCount) {
		savdo();
		if (chkMap(GRPSEL)) {
			rngadj();
			bean(groupStartStitch, groupEndStitch);
			if (closestPointIndex > groupStitchIndex)
				closestPointIndex += beancnt;
			else
				groupStitchIndex += beancnt;
			grpAdj();
		}
		else
			bean(0, header.stitchCount - 1);
		coltab();
		setMap(RESTCH);
	}
}

void unbean(unsigned strt, unsigned fin) {
	unsigned ind, ine;

	ine = MAXSEQ;
	beancnt = 0;
	for (ind = strt; ind <= fin; ind++) {
		mvstch(ine++, ind);
		if (stitchBuffer[ind].x == stitchBuffer[ind + 2].x&&stitchBuffer[ind].y == stitchBuffer[ind + 2].y) {
			ind += 2;
			beancnt += 2;
		}
	}
	if (beancnt)
		beancnt -= 2;;
	if (ind > (unsigned)header.stitchCount - 1)
		ind = header.stitchCount - 1;
	while (ind < header.stitchCount)
		mvstch(ine++, ind++);
	mvstchs(strt, MAXSEQ, ine);
	header.stitchCount = strt + ine;
}

void debean() {
	savdo();
	if (chkMap(GRPSEL)) {
		rngadj();
		unbean(groupStartStitch, groupEndStitch);
		if (closestPointIndex > groupStitchIndex)
			closestPointIndex -= beancnt;
		else
			groupStitchIndex -= beancnt;
		if (closestPointIndex > (unsigned)header.stitchCount - 1)
			closestPointIndex = header.stitchCount - 1;
		if (groupStitchIndex > (unsigned)header.stitchCount - 1)
			groupStitchIndex = header.stitchCount - 1;
		grpAdj();
	}
	else
		unbean(0, header.stitchCount - 1);
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
	mvfltsb(&clipboardPoints[clpad + cnt - 1], &clipboardPoints[clpad - 1], clpad - clpind(pins));
}

void stchadj() {
	unsigned ind, lo, hi;

	for (ind = 0; ind < header.stitchCount; ind++) {
		hi = stitchBuffer[ind].attribute&FRMSK;
		lo = hi >> FRMSHFT;
		if (lo > closestFormToCursor) {
			stitchBuffer[ind].attribute &= NFRMSK;
			hi += 1 << FRMSHFT;
			stitchBuffer[ind].attribute |= hi;
		}
	}
	refilfn();
	lo = closestFormToCursor << FRMSHFT;
	for (ind = header.stitchCount; ind != 0; ind--) {
		hi = stitchBuffer[ind - 1].attribute&FRMSK;
		if ((stitchBuffer[ind - 1].attribute&FRMSK) == lo) {
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
		mvfltsb(&formPoints[fltad + 1], &formPoints[fltad - 1], fltad - fltind(formList[closestFormToCursor + 2].vertices));
	fltad += 2;
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
	mvsatk(&SelectedForm->satinOrAngle.sac[lo - 1], &SelectedForm->satinOrAngle.sac[lo], satkad - sacind(&SelectedForm->satinOrAngle.sac[lo]));
	satkad--;
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
			if (SelectedForm->type == LIN) {
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
		if ((groupEndStitch - groupStartStitch) > 12000) {
			tabmsg(IDS_STMAX);
			return;
		}
		len = groupEndStitch - groupStartStitch + 1;
		SelectedForm = &formList[formIndex];
		frmclr(SelectedForm);
		SelectedForm->type = LIN;
		SelectedForm->sides = len;
		SelectedForm->vertices = adflt(len);
		ine = 0;
		for (ind = groupStartStitch; ind <= groupEndStitch; ind++) {
			SelectedForm->vertices[ine].x = stitchBuffer[ind].x;
			SelectedForm->vertices[ine++].y = stitchBuffer[ind].y;
		}
		frmout(formIndex);
		formIndex++;
		if (closestPointIndex > groupStitchIndex) {
			if (closestPointIndex < (unsigned)header.stitchCount - 1)
				closestPointIndex++;
		}
		else {
			if (groupStitchIndex < (unsigned)header.stitchCount - 1)
				groupStitchIndex++;
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

	if (vclpsid != find) {
		if ((find - vclpsid + sides) % sides < (unsigned)(sides >> 1)) {
			ind = nxt(vclpsid);
			lim = nxt(find);
			while (ind != lim) {
				oseq[sequenceIndex].x = currentFormVertices[ind].x;
				oseq[sequenceIndex++].y = currentFormVertices[ind].y;
				ind = nxt(ind);
			}
		}
		else {
			ind = vclpsid;
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
		ind = clpsegs[activePointIndex].start;
		if (chkMap(TXFIL) && pntd)
			ind++;
		chksid(clpsegs[activePointIndex].asid);
		while (ind <= clpsegs[activePointIndex].finish) {
			oseq[sequenceIndex].x = clipnts[ind].x;
			oseq[sequenceIndex++].y = clipnts[ind++].y;
		}
		vclpsid = clpsegs[activePointIndex].zsid;
	}
	else {
		ind = clpsegs[activePointIndex].finish;
		if (chkMap(TXFIL) && pntd)
			ind--;
		chksid(clpsegs[activePointIndex].zsid);
		if (clpsegs[activePointIndex].start) {
			while (ind >= clpsegs[activePointIndex].start) {
				oseq[sequenceIndex].x = clipnts[ind].x;
				oseq[sequenceIndex++].y = clipnts[ind--].y;
			}
		}
		else {
			while (ind < clpsegs[activePointIndex].start) {
				oseq[sequenceIndex].x = clipnts[ind].x;
				oseq[sequenceIndex++].y = clipnts[ind--].y;
			}
		}
		vclpsid = clpsegs[activePointIndex].asid;
	}
	clpsegs[activePointIndex].dun = 1;
}

unsigned lenref(float* pflt) {
#if	 __UseASM__
	_asm {
		mov		eax, pflt
		sub		eax, clpsegs
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
	unsigned eax = (unsigned)pflt - (unsigned)clpsegs;

	unsigned edx = eax % 29;
	eax /= 29;

	eax <<= 1;

	if ((edx & 0xFF) == 18)
		eax++;

	return eax;
#endif
}

BOOL clpnxt(unsigned sind) {
	unsigned ind;

	ind = 1;
	rstMap(FILDIR);
	while (ind < pcseg) {
		if (toglMap(FILDIR)) {
			outputIndex = (sind + ind) % pcseg2;
			if (!clpsegs[lenref(plens[outputIndex]) >> 1].dun)
				return 0;
			ind++;
		}
		else {
			outputIndex = (sind + pcseg2 - ind) % pcseg2;
			if (!clpsegs[lenref(plens[outputIndex]) >> 1].dun)
				return 0;
		}
	}
	return 1;
}

BOOL nucseg() {
	unsigned	ind;

	if (chkMap(FILDIR))
		ind = clpsegs[activePointIndex].edgeIndex;
	else
		ind = clpsegs[activePointIndex].bind;
	if (clpnxt(ind))
		return 0;
	ind = lenref(plens[outputIndex]);
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
		add		edi, pclpsrt
		mov		edi, [edi]
		mov		esi, src
		shl		esi, 2
		add		esi, pclpsrt
		mov		esi, [esi]
		xor ecx, ecx
		mov		cl, 5
		rep		movsd
	}
#else
	memcpy(pclpsrt[dst], pclpsrt[src], 20);
#endif
}

float getlen(unsigned ind) {
	clipnts[ind].side %= sides;
	return	lengths[clipnts[ind].side] +
		hypot(currentFormVertices[clipnts[ind].side].x - clipnts[ind].x,
			currentFormVertices[clipnts[ind].side].y - clipnts[ind].y);
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
	for (ind = vstrt; ind < vfin; ind++) {
		svrt = vclpx[ind].sid;
		nvrt = nxt(svrt);
		if (isect(svrt, nvrt, &clpsrt[ine].point, &clpsrt[ine].sidlen)) {
			ipnt = &clpsrt[ine].point;
			if (ipnt->x >= lrct.left&&
				ipnt->x <= lrct.right&&
				ipnt->y >= lrct.bottom&&
				ipnt->y <= lrct.top) {
				clpsrt[ine].seglen = hypot(clpsrt[ine].point.x - vpnt0.x, clpsrt[ine].point.y - vpnt0.y);
				clpsrt[ine].lin = svrt;
				pclpsrt[ine] = &clpsrt[ine];
				ine++;
				cnt++;
			}
		}
	}
	if (cnt > 1) {
		qsort((void*)pclpsrt, cnt, 4, lencmp);
		ine = 1;
		for (ind = 0; ind < cnt - 1; ind++) {
			if (fabs(pclpsrt[ind]->seglen - pclpsrt[ind + 1]->seglen) > TINY)
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

	if (pntx < isrct.left)
		return 0;
	if (pntx > isrct.right)
		return 0;
	if (pnty < isrct.bottom)
		return 0;
	if (pnty > isrct.top)
		return 0;
	acnt = 0;
	for (ind = vstrt; ind < vfin; ind++)
	{
		svrt = vclpx[ind].sid;
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
	clpsegs[pcseg].start = strt;
	clpsegs[pcseg].borderLength = getlen(strt);
	clpsegs[pcseg].asid = clipnts[strt].side;
	clpsegs[pcseg].edgeLength = getlen(fin);
	clpsegs[pcseg].zsid = clipnts[fin].side;
	clpsegs[pcseg].finish = fin;
	clpsegs[pcseg++].dun = 0;
	return fin + 1;
}

unsigned vclpfor(unsigned ind) {
	while (!clipnts[ind].flag&&ind < activePointIndex)
		ind++;
	return ind;
}

unsigned vclpbak(unsigned ind) {
	while (!clipnts[ind].flag&&ind)
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
	if (leftEdge < clipboardRectSize.cx) {
		setMap(WASNEG);
		fltof = clipboardRectSize.cx + fabs(leftEdge) + .05;
		for (ind = 0; ind < sides; ind++)
			currentFormVertices[ind].x += fltof;
		SelectedForm->rectangle.left += fltof;
		SelectedForm->rectangle.right += fltof;
	}
	else
		rstMap(WASNEG);
}

void inspnt()
{
	clipnts[activePointIndex + 1].x = clipnts[activePointIndex].x;
	clipnts[activePointIndex + 1].y = clipnts[activePointIndex].y;
	clipnts[activePointIndex].x = midl(clipnts[activePointIndex + 1].x, clipnts[activePointIndex - 1].x);
	clipnts[activePointIndex].y = midl(clipnts[activePointIndex + 1].y, clipnts[activePointIndex - 1].y);
	clipnts[activePointIndex].flag = 1;
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

	duflt();
	clpwid = clipboardRectSize.cx + SelectedForm->fillSpacing;
	if (chkMap(ISUND))
		clpwid = SelectedForm->underlaySpacing;
	if (SelectedForm->fillSpacing < 0)
		clpneg = 1;
	else
		clpneg = 0;
	if (clpwid < CLPMINAUT)
		clpwid = (float)CLPMINAUT;
	if (chkMap(TXFIL))
	{
		if (txad&&SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count <= txad)
			clpwid = SelectedForm->fillSpacing;
		else
			return;
	}
	lengths = new double[sides + 1];
	clplens = new double[sides];
	clpsrt = new CLIPSORT[sides];
	pclpsrt = new CLIPSORT*[sides + 1]();
	ine = leftsid();
	tlen = 0;
	lengths[ine] = 0;
	ine = nxt(ine);
	for (ind = 0; ind <= sides; ind++) {
		inf = nxt(ine);
		lengths[ine] = tlen;
		clplens[ine] = hypot(currentFormVertices[inf].x - currentFormVertices[ine].x, currentFormVertices[inf].y - currentFormVertices[ine].y);
		tlen += clplens[ine];
		ine = inf;
	}
	clpcirc = tlen;
	clpcirc2 = tlen / 2;
	clpseq = (fPOINT*)&stitchBuffer[MAXSEQ];
	//	clpsegs=(CLPSEG*)&bseq;
	clpsegs = (CLPSEG*)&stitchBuffer[MAXSEQ];
	nrct.left = floor(SelectedForm->rectangle.left / clpwid);
	nrct.right = ceil(SelectedForm->rectangle.right / clpwid);
	nrct.bottom = floor(SelectedForm->rectangle.bottom / clipboardRectSize.cy - 1);
	nrct.top = ceil(SelectedForm->rectangle.top / clipboardRectSize.cy + 1) + 2;
	nof = 0;
	if (SelectedForm->wordParam > 1)
		clpnof = SelectedForm->wordParam;
	else
		clpnof = 0;
	if (clpnof) {
		nrct.top++;
		if (SelectedForm->fillSpacing < 0) {
			nrct.bottom--;
			nrct.left -= (float)clipboardRectSize.cx / clpwid;
			nrct.right += (float)clipboardRectSize.cx / clpwid;
		}
	}
	if (clpneg && !clpnof)
		nrct.left -= (float)clipboardRectSize.cx / clpwid;
	if (nrct.bottom < 0) {
		nof = 1 - nrct.bottom;
		nrct.bottom += nof;
		nrct.top += nof;
		fnof = clipboardRectSize.cy*nof;
		for (ind = 0; ind < sides; ind++)
			currentFormVertices[ind].y += fnof;
	}
	clipnts = (CLIPNT*)&bseq;
	segxs = 0;
	for (ind = 0; ind < sides; ind++) {
		strt = floor(currentFormVertices[ind].x / clpwid);
		fin = floor((currentFormVertices[nxt(ind)].x) / clpwid);
		if (strt > fin) {
			ine = strt;
			strt = fin;
			fin = ine;
		}
		if (SelectedForm->fillSpacing < 0)
			fin += clipboardRectSize.cx / clpwid;
		if (fin > (unsigned)nrct.right)
			fin = nrct.right;
		if (clpneg)
			strt -= (float)clipboardRectSize.cx / clpwid;
		for (ine = strt; ine <= fin; ine++) {
			vclpx[segxs].sid = ind;
			vclpx[segxs++].seg = ine;
		}
	}
	qsort((void*)vclpx, segxs, 8, clpcmp);
	iclpx = (unsigned*)&vclpx[segxs];
	ine = 1; inf = vclpx[0].seg;
	iclpx[0] = 0;
	for (ind = 1; ind < segxs; ind++) {
		if (vclpx[ind].seg != inf) {
			iclpx[ine++] = ind;
			inf = vclpx[ind].seg;
		}
	}
	iclpx[ine] = ind;
	isrct.left = isrct.right = currentFormVertices[0].x;
	isrct.top = isrct.bottom = currentFormVertices[0].y;
	for (ind = 1; ind < sides; ind++)
	{
		if (currentFormVertices[ind].x > isrct.right)
			isrct.right = currentFormVertices[ind].x;
		if (currentFormVertices[ind].x < isrct.left)
			isrct.left = currentFormVertices[ind].x;
		if (currentFormVertices[ind].y > isrct.top)
			isrct.top = currentFormVertices[ind].y;
		if (currentFormVertices[ind].y < isrct.bottom)
			isrct.bottom = currentFormVertices[ind].y;
	}
	segps = ine;
	ind = vstrt = cnt = 0;
	seg = vclpx[0].seg;
	clrnum = (nrct.top >> 5) + 1;
	activePointIndex = 0;
	for (ind = 0; ind < segps; ind++) {
		vstrt = iclpx[ind];
		vfin = iclpx[ind + 1];
		ploc.x = clpwid*(ind + nrct.left);
		clpvof = 0;
		if (chkMap(TXFIL))
		{
			tine = (ind + nrct.left) % SelectedForm->fillInfo.texture.lines;
			clipboardStitchCount = txsegs[tine].stitchCount;
			ptx = &txpnts[SelectedForm->fillInfo.texture.index + txsegs[tine].line];
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
				clpvof = (float)(ind%clpnof) / clpnof*clipboardRectSize.cy;
			vpnt0.x = ploc.x + clipBuffer[0].x;
		}
		vpnt0.y = nrct.bottom*clipboardRectSize.cy;
		if (clpnof)
			clpvof = (float)(ind%clpnof) / clpnof*clipboardRectSize.cy;
		for (tine = nrct.bottom; tine < nrct.top; tine++) {
			ploc.y = tine*clipboardRectSize.cy - clpvof;
			vpnt1.x = ploc.x + clipBuffer[0].x;
			vpnt1.y = ploc.y + clipBuffer[0].y;
			if (!activePointIndex) {
				vpnt0.x = vpnt1.x;
				vpnt0.y = vpnt1.y;
			}
			for (inf = 0; inf < clipboardStitchCount; inf++) {
				if (chkMap(TXFIL))
				{
					if (ptx != nullptr) {
						vpnt1.x = ploc.x;
						vpnt1.y = ploc.y + ptx[inf].y;
					}
				}
				else
				{
					vpnt1.x = ploc.x + clipBuffer[inf].x;
					vpnt1.y = ploc.y + clipBuffer[inf].y;
				}

				clipnts[activePointIndex].x = vpnt0.x;
				clipnts[activePointIndex].y = vpnt0.y;
				if (isin(vpnt0.x, vpnt0.y))
				{
					if (activePointIndex&&clipnts[activePointIndex - 1].flag == 2)
						inspnt();
					clipnts[activePointIndex].flag = 0;
				}
				else
				{
					if (activePointIndex && !clipnts[activePointIndex - 1].flag)
						inspnt();
					clipnts[activePointIndex].flag = 2;
				}
				activePointIndex++;
				cnt = insect();
				if (cnt)
				{
					for (ing = 0; ing < cnt; ing++) {
						if (pclpsrt != nullptr) {
							clipnts[activePointIndex].side = pclpsrt[ing]->lin;
							clipnts[activePointIndex].x = pclpsrt[ing]->point.x;
							clipnts[activePointIndex].y = pclpsrt[ing]->point.y;
							clipnts[activePointIndex].flag = 1;
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
		clipnts[activePointIndex - 1].flag = 2;
	}
clpskp:;

	clipnts[activePointIndex].flag = 2;
	if (nof) {
		fnof = nof*clipboardRectSize.cy;
		for (ind = 0; ind < activePointIndex; ind++)
			clipnts[ind].y -= fnof;
		for (ind = 0; ind < sides; ind++)
			currentFormVertices[ind].y -= fnof;
	}
#define CLPVU 0
#define CLPNOP 0

#if CLPVU==1

	goto clp1skp;

#endif

	pcseg = 0;
	regof = vclpx[0].seg;
	rstMap(FILDIR);
	ine = 0;
	if (activePointIndex)
	{
		for (ind = 0; ind < activePointIndex - 1; ind++) {
			switch (clipnts[ind].flag)
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
			delete[] clplens;
			delete[] clpsrt;
			delete[] pclpsrt;

			if (pcseg) {
				clplim = pcseg >> 3;
				clplim = pcseg >> 1;
				if (!clplim)
					clplim = 1;
				if (clplim > 12)
					clplim = 12;
				plens = (float**)&clpsegs[pcseg];
				ine = 0;
				for (ind = 0; ind < pcseg; ind++) {
					plens[ine++] = &clpsegs[ind].borderLength;
					plens[ine++] = &clpsegs[ind].edgeLength;
				}
				qsort((void*)plens, ine, 4, lencmp);
				ind = sizeof(CLPSEG);
				for (ind = 0; ind < ine; ind++) {
					inf = lenref(plens[ind]);
					ing = inf >> 1;
					if (inf & 1)
						clpsegs[ing].edgeIndex = ind;
					else
						clpsegs[ing].bind = ind;
				}

#if CLPVU==1

				for (index = 0; index < activePointIndex; index++) {
					stitchCount[index].x = clipnts[index].x;
					stitchCount[index].y = clipnts[index].y;
					stitchCount[index].attribute = 0;
				}
				header.stitchCount = activePointIndex;
#endif

#if CLPVU==2

				inf = 0;
				for (index = 0; index < pcseg; index++) {
					for (ine = clpsegs[index].start; ine <= clpsegs[index].finish; ine++) {
						stitchCount[inf].x = clipnts[ine].x;
						stitchCount[inf].y = clipnts[ine].y;
						stitchCount[inf++].attribute = index & 0xf;
					}
				}
				header.stitchCount = inf;

#endif

				minx = 1e99;

#if CLPVU==0

				activePointIndex = 0;
				setMap(FILDIR);
				sequenceIndex = 0;
				pcseg2 = pcseg << 1;
				vclpsid = clpsegs[0].asid;
				strtlen = clpsegs[0].edgeLength;
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
						oseq[ind].x -= fltof;
					for (ind = 0; ind < sides; ind++)
						currentFormVertices[ind].x -= fltof;
					SelectedForm->rectangle.left -= fltof;
					SelectedForm->rectangle.right -= fltof;
				}
#endif
			}
}

void vrtsclp() {
	unsigned ind;

	fvars(closestFormToCursor);
	delmclp(closestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = clipboardStitchCount;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->wordParam = iniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = iniFile.clipboardOffset;
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = clipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = clipBuffer[ind].y;
	}
	SelectedForm->fillType = VCLPF;
	SelectedForm->fillColor = activeColor;
	SelectedForm->type = POLI;
	refilfn();
}

void vrtclp() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(hWnd)) {
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {
			redclp();
			CloseClipboard();
			if (clipboardRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != LIN)
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

	if (angfrm.sides) {
		trct = &angfrm.rectangle;
		currentFormVertices = angfrm.vertices;
		trct->left = trct->right = currentFormVertices[0].x;
		trct->bottom = trct->top = currentFormVertices[0].y;
		for (ine = 1; ine < angfrm.sides; ine++) {
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

	frmcpy(&angfrm, &formList[closestFormToCursor]);
	rotationCenter.x = (double)(angfrm.rectangle.right - angfrm.rectangle.left) / 2 + angfrm.rectangle.left;
	rotationCenter.y = (double)(angfrm.rectangle.top - angfrm.rectangle.bottom) / 2 + angfrm.rectangle.bottom;
	angfrm.vertices = angflt;
	rotationAngle = PI / 2;
	for (ind = 0; ind < angfrm.sides; ind++) {
		angfrm.vertices[ind].x = SelectedForm->vertices[ind].x;
		angfrm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&angfrm.vertices[ind]);
	}
	angout();
	SelectedForm = &angfrm;
	currentFormVertices = angfrm.vertices;
	clpcon();
	rotationAngle = -PI / 2;
	rotbak();
	fvars(closestFormToCursor);
}

void horsclp() {
	unsigned ind;

	fvars(closestFormToCursor);
	delmclp(closestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = clipboardStitchCount;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = clipboardStitchCount;
	SelectedForm->wordParam = iniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = iniFile.clipboardOffset;
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = clipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = clipBuffer[ind].y;
	}
	SelectedForm->fillType = HCLPF;
	SelectedForm->fillColor = activeColor;
	SelectedForm->type = POLI;
	currentFormVertices = SelectedForm->vertices;
	refilfn();
}

void horclp() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(hWnd)) {
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {
			redclp();
			CloseClipboard();
			if (clipboardRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != LIN)
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

	frmcpy(&angfrm, &formList[closestFormToCursor]);
	rotationCenter.x = (double)(angfrm.rectangle.right - angfrm.rectangle.left) / 2 + angfrm.rectangle.left;
	rotationCenter.y = (double)(angfrm.rectangle.top - angfrm.rectangle.bottom) / 2 + angfrm.rectangle.bottom;
	angfrm.vertices = angflt;
	if (chkMap(ISUND))
	{
		rotationAngle = PI / 2 - SelectedForm->underlayStitchAngle;
		tflt = insid();
		for (ind = 0; ind < angfrm.sides; ind++)
		{
			angflt[ind].x = tflt[ind].x;
			angflt[ind].y = tflt[ind].y;
			rotflt(&angflt[ind]);
		}
	}
	else
	{
		if (chkMap(TXFIL))
			rotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
		else
			rotationAngle = PI / 2 - SelectedForm->satinOrAngle.angle;
		for (ind = 0; ind < angfrm.sides; ind++)
		{
			angflt[ind].x = SelectedForm->vertices[ind].x;
			angflt[ind].y = SelectedForm->vertices[ind].y;
			rotflt(&angflt[ind]);
		}
	}
	angout();
	SelectedForm = &angfrm;
	currentFormVertices = angflt;
	clpcon();
	rotationAngle = -rotationAngle;
	rotbak();
	fvars(closestFormToCursor);
}

void angsclp() {
	unsigned ind;

	fvars(closestFormToCursor);
	delmclp(closestFormToCursor);
	deltx();
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = clipboardStitchCount;
	SelectedForm->wordParam = iniFile.fillPhase;
	makpoli();
	SelectedForm->satinOrAngle.angle = iniFile.fillAngle;
	SelectedForm->fillSpacing = iniFile.clipboardOffset;
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		SelectedForm->angleOrClipData.clip[ind].x = clipBuffer[ind].x;
		SelectedForm->angleOrClipData.clip[ind].y = clipBuffer[ind].y;
	}
	SelectedForm->fillType = ANGCLPF;
	SelectedForm->fillColor = activeColor;
	SelectedForm->type = POLI;
	refilfn();
}

void angclp() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(hWnd)) {
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {
			redclp();
			CloseClipboard();
			if (clipboardRectSize.cy > CLPMIN) {
				if (selectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < selectedFormCount; ind++) {
						closestFormToCursor = selectedFormList[ind];
						fvars(closestFormToCursor);
						if (SelectedForm->type != LIN)
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
	SelectedForm->edgeType = EGDUB;
	sequenceIndex = 0;
	SelectedForm->borderColor = activeColor;
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
		for (ind = 0; ind < header.stitchCount; ind++) {
			cod = stitchBuffer[ind].attribute & 0x3fff;
			if (stitchBuffer[ind].attribute&(WLKMSK | CWLKMSK | UNDMSK))
				ucnts[cod]++;
			else
			{
				if (stitchBuffer[ind].attribute&FTHMSK)
					fthcnts[cod]++;
				else {
					switch (stitchBuffer[ind].attribute&TYPMSK) {
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
	LoadString(hInst, IDS_NCOLCHG, buf, HBUFSIZ);
	sprintf_s(msgbuf, sizeof(msgbuf), buf, chngcnt);
	shoMsg(msgbuf);
}

BOOL fxpnt() {
	double		len, dif;
	unsigned	ind;

	mvpnt.x = currentFormVertices[nextStart].x;
	mvpnt.y = currentFormVertices[nextStart].y;
	len = hypot(mvpnt.x - selectedPoint.x, mvpnt.y - selectedPoint.y);
	if (len > adjustedSpace) {
		for (ind = 0; ind < 10; ind++) {
			len = hypot(mvpnt.x - selectedPoint.x, mvpnt.y - selectedPoint.y);
			dif = adjustedSpace - len;
			mvpnt.x += dif*listCOSINEs[clpstrt];
			mvpnt.y += dif*listSINEs[clpstrt];
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
		selectedPoint.x = mvpnt.x;
		selectedPoint.y = mvpnt.y;
		beancnt++;
		len = hypot(currentFormVertices[nextStart].x - selectedPoint.x, currentFormVertices[nextStart].y - selectedPoint.y);
		cnt = floor(len / adjustedSpace);
		dif.x = adjustedSpace*listCOSINEs[clpstrt];
		dif.y = adjustedSpace*listSINEs[clpstrt];
		selectedPoint.x += dif.x*cnt;
		selectedPoint.y += dif.y*cnt;
		beancnt += cnt;
	}
}

void fxlin() {
	double		len;
	unsigned	cnt;
	unsigned	ind;
	dPOINT		dif;

	if (fxpnt()) {
		chainEndPoints[activePointIndex].x = selectedPoint.x = mvpnt.x;
		chainEndPoints[activePointIndex].y = selectedPoint.y = mvpnt.y;
		activePointIndex++;
		len = hypot(currentFormVertices[nextStart].x - selectedPoint.x, currentFormVertices[nextStart].y - selectedPoint.y);
		cnt = floor(len / adjustedSpace);
		dif.x = adjustedSpace*listCOSINEs[clpstrt];
		dif.y = adjustedSpace*listSINEs[clpstrt];
		for (ind = 0; ind < cnt; ind++) {
			selectedPoint.x += dif.x;
			selectedPoint.y += dif.y;
			chainEndPoints[activePointIndex].x = selectedPoint.x;
			chainEndPoints[activePointIndex].y = selectedPoint.y;
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
		beancnt = 0;
		selectedPoint.x = currentFormVertices[0].x;
		selectedPoint.y = currentFormVertices[0].y;
		activePointIndex = 1;
		chainEndPoints[0].x = selectedPoint.x;
		chainEndPoints[0].y = selectedPoint.y;
		for (clpstrt = 0; clpstrt < (unsigned)sides - 1; clpstrt++) {
			nextStart = clpstrt + 1;
			fxlit();
		}
		if (SelectedForm->type != LIN) {
			nextStart = 0;
			fxlit();
		}
		else
			nextStart = sides - 1;
		if (!inicnt) {
			inicnt = beancnt;
			loval = adjustedSpace;
			minter = ter = (double)hypot(currentFormVertices[nextStart].x - selectedPoint.x, currentFormVertices[nextStart].y - selectedPoint.y);
			minspac = adjustedSpace;
			ter /= inicnt;
			adjustedSpace += ter / 2;
			hival = loval + ter;
		}
		else {
			ter = hypot(currentFormVertices[nextStart].x - selectedPoint.x, currentFormVertices[nextStart].y - selectedPoint.y);
			if (ter > spac2)
				ter = SelectedForm->edgeSpacing - ter;
			if (ter < minter) {
				minter = ter;
				minspac = adjustedSpace;
			}
			if (inicnt == beancnt)
				loval = adjustedSpace;
			else {
				if (beancnt > inicnt)
					hival = adjustedSpace;
				else
					loval = adjustedSpace;
			}
			adjustedSpace = loval + (hival - loval) / 2;
		}
		lupcnt++;
	}
	selectedPoint.x = currentFormVertices[0].x;
	selectedPoint.y = currentFormVertices[0].y;
	outputIndex = 1;
	adjustedSpace = minspac;
	for (clpstrt = 0; clpstrt < (unsigned)sides - 1; clpstrt++) {
		nextStart = clpstrt + 1;
		fxlin();
	}
	if (SelectedForm->type != LIN) {
		nextStart = 0;
		fxlin();
	}
	ter = hypot(currentFormVertices[nextStart].x - selectedPoint.x, currentFormVertices[nextStart].y - selectedPoint.y);
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
		if (SelectedForm->type == LIN) {
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
	listSINEs = (double*)angflt;
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
	SelectedForm->borderColor = activeColor;
	SelectedForm->edgeSpacing = iniFile.chainSpace;
	SelectedForm->borderSize = borderWidth;
	SelectedForm->edgeStitchLen = iniFile.chainRatio;
	SelectedForm->maxBorderStitchLen = iniFile.maxStitchLength;
	SelectedForm->minBorderStitchLen = minStitchLength;
	if (chkMap(LINCHN))
		SelectedForm->edgeType = EGCHNL;
	else
		SelectedForm->edgeType = EGCHNH;
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
			ritot(header.stitchCount);
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
		vstrt = 0;
		vfin = sides;
		for (ind = 0; ind < header.stitchCount; ind++) {
			if (cisin(stitchBuffer[ind].x, stitchBuffer[ind].y)) {
				stitchBuffer[ine].x = stitchBuffer[ind].x;
				stitchBuffer[ine].y = stitchBuffer[ind].y;
				stitchBuffer[ine++].attribute = stitchBuffer[ind].attribute;
			}
		}
		header.stitchCount = ine;
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

	tflt = (fPOINT*)&stitchBuffer[MAXPCS];
	dif.x = chainEndPoints[fin].x - chainEndPoints[strt].x;
	dif.y = chainEndPoints[fin].y - chainEndPoints[strt].y;
	len = hypot(dif.x, dif.y);
	rat = len / clipboardRectSize.cx;
	rotationAngle = atan2(dif.y, dif.x);
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		tflt[ind].x = rclps[ind].x*rat;
		tflt[ind].y = rclps[ind].y;
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
	rotationCenter.x = rotationCenter.y = 0;
	for (ind = 1; ind < activePointIndex; ind++)
		xclpfn(ind - 1, ind);
	if (SelectedForm->type != LIN) {
		oseq[sequenceIndex].x = chainEndPoints[0].x;
		oseq[sequenceIndex++].y = chainEndPoints[0].y;
	}
}

void dulast() {
	unsigned	ind, ine, mind;
	double		minlen;
	double		len;

	rclps = (fPOINT*)&chainEndPoints[activePointIndex];
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
				rclps[ine].x = chainEndPoints[ind].x;
				rclps[ine++].y = chainEndPoints[ind].y;
			}
			for (ind = 0; ind <= mind; ind++) {
				rclps[ine].x = chainEndPoints[ind].x;
				rclps[ine++].y = chainEndPoints[ind].y;
			}
			MoveMemory(chainEndPoints, rclps, sizeof(fPOINT)*ine);
		}
	}
}

void clpxadj() {
	unsigned	ind;
	double		pivot;

	dulast();
	if (SelectedForm->type == LIN) {
		pivot = clipboardRectSize.cy / 2;
		for (ind = 0; ind < clipboardStitchCount; ind++) {
			rclps[ind].x = clipBuffer[ind].x;
			rclps[ind].y = -clipBuffer[ind].y + pivot;
		}
	}
	else {
		for (ind = 0; ind < clipboardStitchCount; ind++) {
			rclps[ind].x = clipBuffer[ind].x;
			rclps[ind].y = -clipBuffer[ind].y;
		}
	}
}

void fsclpx() {
	unsigned	ind;

	deleclp(closestFormToCursor);
	SelectedForm->edgeType = EGCLPX;
	SelectedForm->clipEntries = clipboardStitchCount;
	SelectedForm->borderClipData = nueclp(closestFormToCursor, clipboardStitchCount);
	SelectedForm->borderSize = clipboardRectSize.cy;
	SelectedForm->edgeSpacing = clipboardRectSize.cx;
	SelectedForm->borderColor = activeColor;
	bsizpar();
	for (ind = 0; ind < clipboardStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = clipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = clipBuffer[ind].y;
	}
	rotationCenter.x = rotationCenter.y = 0;
	duxclp();
	refilfn();
}

void filclpx() {
	unsigned	ind;

	if (filmsgs(FML_CLP))
		return;
	if (OpenClipboard(hWnd)) {
		fvars(closestFormToCursor);
		hClip = RegisterClipboardFormat(pcdClipFormat);
		hClipMem = GetClipboardData(hClip);
		if (hClipMem) {
			savdo();
			redclp();
			CloseClipboard();
			if (clipboardRectSize.cx > CLPMIN) {
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
		sprintf_s(buf, sizeof(buf), "%d", iniFile.wavePoints);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.waveStart);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.waveEnd);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.waveLobes);
		SetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buf);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buf, HBUFSIZ);
			iniFile.wavePoints = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buf, HBUFSIZ);
			iniFile.waveStart = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buf, HBUFSIZ);
			iniFile.waveEnd = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buf, HBUFSIZ);
			iniFile.waveLobes = atoi(buf);
			if (iniFile.wavePoints > 100)
				iniFile.wavePoints = 100;
			if (iniFile.wavePoints < 3)
				iniFile.wavePoints = 3;
			if (iniFile.waveStart == iniFile.waveEnd)
				iniFile.waveEnd += (iniFile.wavePoints >> 2);
			iniFile.waveStart %= iniFile.wavePoints;
			iniFile.waveEnd %= iniFile.wavePoints;
			EndDialog(hwndlg, 1);
			break;

		case IDC_DEFWAV:

			iniFile.wavePoints = IWAVPNTS;
			iniFile.waveStart = IWAVSTRT;
			iniFile.waveEnd = IWAVEND;
			iniFile.waveLobes = IWAVS;
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
	if (DialogBox(hInst, MAKEINTRESOURCE(IDD_WAV), hWnd, (DLGPROC)wavprc)) {
		ind = fltad;
		end = iniFile.waveEnd + 1;
		durpoli(iniFile.wavePoints);
		mdufrm();
		fltad = ind;
		tflt = (fPOINT*)&bseq;
		ine = 0;
		ind = iniFile.waveStart;
		while (ind != iniFile.waveEnd) {
			inf = (ind + 1) % iniFile.wavePoints;
			tflt[ine].x = -currentFormVertices[inf].x + currentFormVertices[ind].x;
			tflt[ine].y = -currentFormVertices[inf].y + currentFormVertices[ind].y;
			ine++;
			ind = inf;
		}
		cnt = ine;
		ine = 0;
		pos.x = pos.y = 0;
		for (inf = 0; inf < iniFile.waveLobes; inf++) {
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
		rotationAngle = -atan2(currentFormVertices[ine - 1].y - currentFormVertices[0].y, currentFormVertices[ine - 1].x - currentFormVertices[0].x);
		for (ind = 0; ind < ine; ind++)
			rotflt(&currentFormVertices[ind]);
		SelectedForm->type = LIN;
		SelectedForm->sides = ine;
		fltad += ine;
		frmout(formIndex);
		rstMap(FORMSEL);
		lfp_Size.x = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		lfp_Size.y = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		hrat = unzoomedRect.x / 4 / lfp_Size.x;
		if (hrat > 1)
			hrat = 1;
		vrat = unzoomedRect.y / 4 / lfp_Size.y;
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
		fmovdif.x = fmovdif.y = 0;
		newFormVertexCount = ine + 1;
		setmfrm();
		mdufrm();
	}
}

void srtfrm() {
	unsigned	ind, ine, inf, tot, sav;
	unsigned	hst[MAXFORMS];
	fPOINTATTRIBUTE*	hstch;

	if (header.stitchCount) {
		savdo();
		FillMemory(hst, sizeof(unsigned)*MAXFORMS, 0);
		for (ind = 0; ind < header.stitchCount; ind++)
			hst[(stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT]++;
		tot = 0;
		for (ind = 0; ind < MAXFORMS; ind++) {
			sav = hst[ind];
			hst[ind] = tot;
			tot += sav;
		}
		hstch = &stitchBuffer[MAXSEQ];
		for (ind = 0; ind < header.stitchCount; ind++) {
			ine = (stitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
			inf = hst[ine]++;
			hstch[inf].x = stitchBuffer[ind].x;
			hstch[inf].y = stitchBuffer[ind].y;
			hstch[inf].attribute = stitchBuffer[ind].attribute;
		}
		MoveMemory(stitchBuffer, hstch, sizeof(fPOINTATTRIBUTE)*header.stitchCount);
		coltab();
		setMap(RESTCH);
	}
}

